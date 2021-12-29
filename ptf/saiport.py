# Copyright 2021-present Intel Corporation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

'''
Thrift SAI Port interface tester
'''

from sai_thrift.sai_headers import *

from sai_base_test import *

TEST_QOS_DEFAULT_TC = 7
TEST_DEFAULT_SPEED = 25000
TEST_QOS_MAP_ON_CREATE_PORT = False
TEST_SLEEP_TIME = 2

QOS_TYPE_DICT = {
    SAI_QOS_MAP_TYPE_DSCP_TO_TC: (
        "dscp", "tc"),
    SAI_QOS_MAP_TYPE_DSCP_TO_COLOR: (
        "dscp", "color"),
    SAI_QOS_MAP_TYPE_TC_TO_QUEUE: (
        "tc", "queue_index"),
    SAI_QOS_MAP_TYPE_DOT1P_TO_COLOR: (
        "dot1p", "color"),
    SAI_QOS_MAP_TYPE_DOT1P_TO_TC: (
        "dot1p", "tc"),
    SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_QUEUE: (
        "prio", "queue_index"),
    SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_PRIORITY_GROUP: (
        "prio", "pg"),
    SAI_QOS_MAP_TYPE_TC_TO_PRIORITY_GROUP: (
        "tc", "pg"),
    SAI_QOS_MAP_TYPE_TC_AND_COLOR_TO_DOT1P: (
        "tc", "dot1p", "color"),
    SAI_QOS_MAP_TYPE_TC_AND_COLOR_TO_DSCP: (
        "tc", "dscp", "color")}


def fec_to_str(fec):
    '''
    Convert fec mode to string

    Args:
        fec (int): fec mode

    Returns:
        str: fec mode in string format
    '''
    if fec == 0:
        return "NONE"
    elif fec == 1:
        return "RS"
    elif fec == 2:
        return "FC"
    return "<unknown>"


def speed_to_num_lanes(speed):
    '''
    Convert speed to lanes number

    Args:
        speed (int): speed value

    Returns:
        str: lanes number
    '''
    if speed == 1000 or speed == 10000 or speed == 25000:
        return 1
    elif speed == 40000 or speed == 100000:
        return 4
    elif speed == 50000:
        return 2

    return -1


def num_lanes_to_lane_list(num_lanes):
    '''
    Return lane list depending on the lanes number value

    Args:
        num_lanes (int): number of lanes

    Returns:
        sai_thrift_u32_list_t: lane list
    '''
    if num_lanes == 1:
        lane_list = sai_thrift_u32_list_t(count=num_lanes,
                                          uint32list=[34])
    elif num_lanes == 2:
        lane_list = sai_thrift_u32_list_t(count=num_lanes,
                                          uint32list=[34, 35])
    elif num_lanes == 4:
        lane_list = sai_thrift_u32_list_t(
            count=num_lanes,
            uint32list=[34, 35, 36, 37])
    return lane_list


def acl_test_case(name, add_remove_bind=None, use_acl_group=None):
    '''
    Print test case name

    Args:
        name(str): base test case name
        add_remove_bind(bool): if True adds "add/remove_bind"
        to the test case name
        use_acl_group (bool): if True adds "ACL table group"
        to the test case name
    '''
    if add_remove_bind is True:
        name += " add/remove_bind"
    if use_acl_group:
        name += " ACL table group"
    else:
        name += " ACL table"
    print(name)


@group("draft")
class PortAttributeTest(SaiHelper):
    ''' Test port attributes '''

    def setUp(self):
        super(PortAttributeTest, self).setUp()

        sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
        self.port = sai_thrift_create_port(
            self.client,
            hw_lane_list=sai_list,
            speed=25000,
            global_flow_control_mode=SAI_PORT_FLOW_CONTROL_MODE_RX_ONLY,
            internal_loopback_mode=SAI_PORT_INTERNAL_LOOPBACK_MODE_MAC,
            media_type=SAI_PORT_MEDIA_TYPE_COPPER)

    def runTest(self):
        self.portAttributeTest()
        self.portAttributeIngressSamplePacket()
        self.switchAttributePortListTest()

    def tearDown(self):
        sai_thrift_remove_port(self.client, self.port)
        super(PortAttributeTest, self).tearDown()

    def switchAttributePortListTest(self):
        ''' Test switch attribute port list '''
        print("switchAttributePortListTest")

        # Get active port list based on number_of_active_ports
        attr = sai_thrift_get_switch_attribute(
            self.client, number_of_active_ports=True)
        number_of_active_ports = attr['number_of_active_ports']
        attr = sai_thrift_get_switch_attribute(
            self.client, port_list=sai_thrift_object_list_t(
                idlist=[], count=number_of_active_ports))
        self.assertEqual(number_of_active_ports, attr['port_list'].count)

        # Get active port list based on unknown number set to 100
        attr = sai_thrift_get_switch_attribute(
            self.client, port_list=sai_thrift_object_list_t(
                idlist=[], count=100))
        self.assertEqual(number_of_active_ports, attr['port_list'].count)

    def portAttributeIngressSamplePacket(self):
        ''' Verify the creation of sample packet '''
        print("portAttributeIngressSamplePacket")
        sample_packet = 0
        try:
            attr = sai_thrift_get_port_attribute(
                self.client, self.port, ingress_samplepacket_enable=True)
            self.assertEqual(attr['ingress_samplepacket_enable'],
                             SAI_NULL_OBJECT_ID)
            sample_packet = sai_thrift_create_samplepacket(
                self.client,
                sample_rate=2,
                mode=SAI_SAMPLEPACKET_MODE_SHARED)
            self.assertTrue(sample_packet != 0,
                            "Failed to create samplepacket")
            status = sai_thrift_set_port_attribute(
                self.client,
                self.port,
                ingress_samplepacket_enable=sample_packet)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
        finally:
            if sample_packet != 0:
                sai_thrift_set_port_attribute(
                    self.client,
                    self.port,
                    ingress_samplepacket_enable=0)
                sai_thrift_remove_samplepacket(self.client, sample_packet)

    def portBufferProfileList(self):
        ''' Verify the creation of buffer profile list '''
        print("portBufferProfileList")
        try:
            ingress_pool_size = 1024
            ig_buffer_pool_id = sai_thrift_create_buffer_pool(
                self.client,
                type=SAI_BUFFER_POOL_TYPE_INGRESS,
                size=ingress_pool_size)
            self.assertTrue(ig_buffer_pool_id != 0,
                            "Failed to create buffer pool")

            ig_profile = sai_thrift_create_buffer_profile(
                self.client,
                pool_id=ig_buffer_pool_id,
                reserved_buffer_size=1024,
                threshold_mode=0,
                shared_dynamic_th=30)
            self.assertTrue(ig_profile != 0, "Failed to create buffer profile")

            sai_list = sai_thrift_object_list_t(count=10, idlist=[])
            sai_thrift_get_port_attribute(
                self.client,
                self.port,
                qos_ingress_buffer_profile_list=sai_list)
            sai_list = sai_thrift_object_list_t(count=1, idlist=[ig_profile])
            status = sai_thrift_set_port_attribute(
                self.client,
                self.port,
                qos_ingress_buffer_profile_list=sai_list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sai_list = sai_thrift_object_list_t(count=1, idlist=[])
            attr = sai_thrift_get_port_attribute(
                self.client,
                self.port,
                qos_ingress_buffer_profile_list=sai_list)
            self.assertEqual(
                attr['qos_ingress_buffer_profile_list'].idlist.count,
                1)
        finally:
            if ig_profile != 0:
                sai_thrift_remove_buffer_profile(self.client, ig_profile)
            if ig_buffer_pool_id != 0:
                sai_thrift_remove_buffer_pool(self.client, ig_buffer_pool_id)

    def portAttributeTest(self):
        ''' Test port attributes '''
        print("portAttributeTest")
        attr = sai_thrift_get_port_attribute(
            self.client, self.port, speed=True)
        self.assertEqual(attr['speed'], 25000)

        sai_list = sai_thrift_u32_list_t(count=1, uint32list=[])
        attr = sai_thrift_get_port_attribute(
            self.client, self.port, hw_lane_list=sai_list)
        self.assertEqual(attr['hw_lane_list'].count, 1)
        self.assertEqual(attr['hw_lane_list'].uint32list[0], 34)
        attr = sai_thrift_get_port_attribute(
            self.client, self.port, global_flow_control_mode=True)
        self.assertEqual(attr['global_flow_control_mode'],
                         SAI_PORT_FLOW_CONTROL_MODE_RX_ONLY)

        status = sai_thrift_set_port_attribute(
            self.client,
            self.port,
            global_flow_control_mode=SAI_PORT_FLOW_CONTROL_MODE_TX_ONLY)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_port_attribute(
            self.client, self.port, global_flow_control_mode=True)
        self.assertEqual(attr['global_flow_control_mode'],
                         SAI_PORT_FLOW_CONTROL_MODE_TX_ONLY)

        attr = sai_thrift_get_port_attribute(
            self.client, self.port, internal_loopback_mode=True)
        self.assertEqual(attr['internal_loopback_mode'],
                         SAI_PORT_INTERNAL_LOOPBACK_MODE_MAC)
        status = sai_thrift_set_port_attribute(
            self.client,
            self.port,
            internal_loopback_mode=SAI_PORT_INTERNAL_LOOPBACK_MODE_PHY)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_port_attribute(
            self.client, self.port, internal_loopback_mode=True)
        self.assertEqual(attr['internal_loopback_mode'],
                         SAI_PORT_INTERNAL_LOOPBACK_MODE_PHY)

        attr = sai_thrift_get_port_attribute(
            self.client, self.port, media_type=True)
        self.assertEqual(attr['media_type'], SAI_PORT_MEDIA_TYPE_COPPER)

        status = sai_thrift_set_port_attribute(
            self.client, self.port, media_type=SAI_PORT_MEDIA_TYPE_FIBER)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_port_attribute(
            self.client, self.port, media_type=True)
        self.assertEqual(attr['media_type'], SAI_PORT_MEDIA_TYPE_FIBER)

        attr = sai_thrift_get_port_attribute(
            self.client, self.port, number_of_ingress_priority_groups=True)
        number_of_pg = attr['number_of_ingress_priority_groups']

        sai_list = sai_thrift_u32_list_t(count=20, uint32list=[])
        attr = sai_thrift_get_port_attribute(
            self.client, self.port, ingress_priority_group_list=sai_list)
        self.assertEqual(attr['ingress_priority_group_list'].count,
                         number_of_pg)

        attr = sai_thrift_get_port_attribute(
            self.client, self.port, qos_number_of_queues=True)
        qos_number_of_queues = attr['qos_number_of_queues']

        sai_list = sai_thrift_u32_list_t(count=20, uint32list=[])
        attr = sai_thrift_get_port_attribute(
            self.client, self.port, qos_queue_list=sai_list)
        self.assertEqual(
            attr['qos_queue_list'].count,
            qos_number_of_queues)

        attr = sai_thrift_get_port_attribute(
            self.client, self.port, qos_number_of_scheduler_groups=True)
        number_of_schg = attr['qos_number_of_scheduler_groups']

        sai_list = sai_thrift_u32_list_t(count=20, uint32list=[])
        attr = sai_thrift_get_port_attribute(
            self.client, self.port, qos_scheduler_group_list=sai_list)
        self.assertEqual(
            attr['qos_scheduler_group_list'].count,
            number_of_schg)


@group("draft")
class ListPortAttributesTest(SaiHelperBase):
    ''' Test list of port attributes '''
    test_scenario = []

    test_scenario.append({'attribute': 'mtu',
                          'attr_name': 'SAI_PORT_ATTR_MTU',
                          'test_create': 100,
                          'test_set': 1000})
    test_scenario.append({'attribute': 'mtu',
                          'attr_name': 'SAI_PORT_ATTR_MTU',
                          'test_create': 1000,
                          'test_set': 10000})

    test_scenario.append({'attribute': 'qos_default_tc',
                          'attr_name': 'SAI_PORT_ATTR_QOS_DEFAULT_TC',
                          'test_create': TEST_QOS_DEFAULT_TC,
                          'test_set': 9})
    test_scenario.append({'attribute': 'qos_default_tc',
                          'attr_name': 'SAI_PORT_ATTR_QOS_DEFAULT_TC',
                          'test_create': TEST_QOS_DEFAULT_TC,
                          'test_set': 0})

    test_scenario.append({'attribute': 'media_type',
                          'attr_name': 'SAI_PORT_ATTR_MEDIA_TYPE',
                          'test_create': SAI_PORT_MEDIA_TYPE_COPPER,
                          'test_set': SAI_PORT_MEDIA_TYPE_FIBER})

    test_scenario.append({'attribute': 'internal_loopback_mode',
                          'attr_name': 'SAI_PORT_INTERNAL_LOOPBACK_MODE',
                          'test_create': SAI_PORT_INTERNAL_LOOPBACK_MODE_NONE,
                          'test_set': SAI_PORT_INTERNAL_LOOPBACK_MODE_PHY})
    test_scenario.append({'attribute': 'internal_loopback_mode',
                          'attr_name': 'SAI_PORT_INTERNAL_LOOPBACK_MODE',
                          'test_create': SAI_PORT_INTERNAL_LOOPBACK_MODE_PHY,
                          'test_set': SAI_PORT_INTERNAL_LOOPBACK_MODE_MAC})
    test_scenario.append({'attribute': 'internal_loopback_mode',
                          'attr_name': 'SAI_PORT_INTERNAL_LOOPBACK_MODE',
                          'test_create': SAI_PORT_INTERNAL_LOOPBACK_MODE_MAC,
                          'test_set': SAI_PORT_INTERNAL_LOOPBACK_MODE_NONE})

    def setUp(self):
        self.portx = None
        self.qos_default_tc = None
        self.mtu = None
        self.media_type = None
        self.internal_loopback_mode = None
        super(ListPortAttributesTest, self).setUp()

    def runTest(self):
        print("ListPortAttributesTest")
        try:

            for scenario in self.test_scenario:
                # Set all test attributes from the test_scenario list to 'None'
                for scen in self.test_scenario:
                    setattr(self, scen['attribute'], None)

                # For attribute under test set the value to
                # scenario'test_create'. It will be the only not 'None' value
                setattr(self, scenario['attribute'], scenario['test_create'])

                sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
                self.portx = sai_thrift_create_port(
                    self.client,
                    hw_lane_list=sai_list,
                    speed=TEST_DEFAULT_SPEED,
                    qos_default_tc=self.qos_default_tc,
                    mtu=self.mtu,
                    media_type=self.media_type,
                    internal_loopback_mode=self.internal_loopback_mode
                )
                self.assertTrue(self.portx != 0, "Failed to create port")

                # Verify Create/Get port attribute
                if scenario['test_create'] is not None:
                    # Verify the create attribute value

                    attr = sai_thrift_get_port_attribute(
                        self.client,
                        self.portx,
                        qos_default_tc=self.qos_default_tc,
                        mtu=self.mtu,
                        media_type=self.media_type,
                        internal_loopback_mode=self.internal_loopback_mode
                    )

                    # Verify if value on get is the same used for create
                    result = "Failed"
                    if (attr[scenario['attribute']] ==
                            scenario['test_create']):
                        result = "OK"
                    print("Verify Create/Get attribute %s (%s) value=%d: %s"
                          % (scenario['attribute'],
                             scenario['attr_name'],
                             scenario['test_create'],
                             result))
                    self.assertEqual(attr[scenario['attribute']],
                                     scenario['test_create'],
                                     "Failed to verify Create/Get attribute")

                # Verify Set/Get port attribute
                if scenario['test_set'] is not None:
                    setattr(self, scenario['attribute'], scenario['test_set'])
                    status = sai_thrift_set_port_attribute(
                        self.client,
                        self.portx,
                        qos_default_tc=self.qos_default_tc,
                        mtu=self.mtu,
                        media_type=self.media_type,
                        internal_loopback_mode=self.internal_loopback_mode)
                    self.assertEqual(status, SAI_STATUS_SUCCESS)

                    attr = sai_thrift_get_port_attribute(
                        self.client,
                        self.portx,
                        qos_default_tc=self.qos_default_tc,
                        mtu=self.mtu,
                        media_type=self.media_type,
                        internal_loopback_mode=self.internal_loopback_mode)
                    # Verify if Set value is the same as we get on Get
                    result = "Failed"
                    if attr[scenario['attribute']] == scenario['test_set']:
                        result = "OK"

                    print("Verify Set/Get attribute %s (%s) value=%d: %s"
                          % (scenario['attribute'],
                             scenario['attr_name'],
                             scenario['test_set'],
                             result))
                    self.assertEqual(
                        attr[scenario['attribute']],
                        scenario['test_set'],
                        "Failed to verify Set/Get attribute")

                status = sai_thrift_remove_port(self.client, self.portx)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
                self.portx = 0
        finally:
            if self.portx != 0:
                status = sai_thrift_remove_port(self.client, self.portx)
                self.assertEqual(status, SAI_STATUS_SUCCESS)

    def tearDown(self):
        print("ListPortAttributesTest tearDown")
        super(ListPortAttributesTest, self).tearDown()


@group("draft")
class PortQOSAttributeTest(SaiHelperBase):
    ''' Test port QOS attributes '''

    def setUp(self):

        super(PortQOSAttributeTest, self).setUp()

        sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
        self.portx = sai_thrift_create_port(
            self.client,
            hw_lane_list=sai_list,
            speed=TEST_DEFAULT_SPEED,
            qos_default_tc=TEST_QOS_DEFAULT_TC)
        self.assertTrue(self.portx != 0,
                        "Failed to create port")

    def runTest(self):
        print("PortQOSAttributeTest")
        # SAI_PORT_ATTR_QOS_NUMBER_OF_QUEUES - qos_number_of_queues
        # get read_only attribute
        attr = sai_thrift_get_port_attribute(
            self.client,
            self.portx,
            qos_number_of_queues=True,
            number_of_ingress_priority_groups=True)
        self.assertNotEqual(attr['qos_number_of_queues'], 0)
        qos_number_of_queues = attr['qos_number_of_queues']
        number_of_pg = attr['number_of_ingress_priority_groups']

        # SAI_PORT_ATTR_QOS_QUEUE_LIST - qos_queue_list
        # get read_only attribute
        attr = sai_thrift_get_port_attribute(
            self.client, self.portx,
            qos_queue_list=sai_thrift_object_list_t(
                idlist=[], count=qos_number_of_queues))
        self.assertEqual(attr['qos_queue_list'].count,
                         qos_number_of_queues)

        # SAI_PORT_ATTR_INGRESS_PRIORITY_GROUP_LIST
        #  - ingress_priority_group_list
        # get read_only attribute
        attr = sai_thrift_get_port_attribute(
            self.client, self.portx,
            ingress_priority_group_list=sai_thrift_object_list_t(
                idlist=[], count=number_of_pg))
        self.assertEqual(attr['ingress_priority_group_list'].count,
                         number_of_pg)

        # SAI_PORT_ATTR_QOS_NUMBER_OF_SCHEDULER_GROUPS
        #  - qos_number_of_scheduler_groups
        # READ_ONLY
        attr = sai_thrift_get_port_attribute(
            self.client, self.portx, qos_number_of_scheduler_groups=True)
        number_of_schg = attr['qos_number_of_scheduler_groups']

        # SAI_PORT_ATTR_QOS_SCHEDULER_GROUP_LIST
        #  - qos_scheduler_group_list
        # READ_ONLY
        attr = sai_thrift_get_port_attribute(
            self.client, self.portx,
            qos_scheduler_group_list=sai_thrift_object_list_t(
                idlist=[], count=number_of_schg))
        self.assertEqual(attr['qos_scheduler_group_list'].count,
                         number_of_schg)

        # SAI_PORT_ATTR_QOS_DEFAULT_TC - qos_default_tc
        # create/set
        attr = sai_thrift_get_port_attribute(
            self.client, self.portx, qos_default_tc=True)
        self.assertEqual(attr['qos_default_tc'], TEST_QOS_DEFAULT_TC)

    def tearDown(self):
        sai_thrift_remove_port(self.client, self.portx)
        super(PortQOSAttributeTest, self).tearDown()


def create_qos_map(client, map_type, key_list, data_list):
    '''
    Create qos map

    Args:
        client (sai_thrift.sai_rpc.Client): RPC client
        map_type (int): map type
        key_list (list): list of keys
        data_list (list): list of data

    Returns:
        int: qos map id
    '''
    map_list = []
    i = 0

    for qos_type_dict_key in QOS_TYPE_DICT:
        if ((map_type == qos_type_dict_key) and
                (len(QOS_TYPE_DICT[qos_type_dict_key]) == 2)):
            for key_list_data in key_list:
                value_list_data = data_list[i]
                mapping = sai_thrift_qos_map_t(
                    key=sai_thrift_qos_map_params_t(
                        **{QOS_TYPE_DICT[qos_type_dict_key][0]:
                           key_list_data}),
                    value=sai_thrift_qos_map_params_t(
                        **{QOS_TYPE_DICT[qos_type_dict_key][1]:
                           value_list_data}))
                map_list.append(mapping)
                i += 1
        elif ((map_type == qos_type_dict_key) and
              (len(QOS_TYPE_DICT[qos_type_dict_key]) == 3)):
            for key_list_data1, key_list_data2 in key_list:
                value_list_data = data_list[i]
                mapping = sai_thrift_qos_map_t(
                    key=sai_thrift_qos_map_params_t(
                        **{QOS_TYPE_DICT[qos_type_dict_key][0]:
                           key_list_data1,
                           QOS_TYPE_DICT[qos_type_dict_key][2]:
                           key_list_data2}),
                    value=sai_thrift_qos_map_params_t(
                        **{QOS_TYPE_DICT[qos_type_dict_key][1]:
                           value_list_data}))
                map_list.append(mapping)
                i += 1

    qos_map_list = sai_thrift_qos_map_list_t(
        count=len(map_list), maplist=map_list)
    qos_map_id = sai_thrift_create_qos_map(client,
                                           type=map_type,
                                           map_to_value_list=qos_map_list)
    return qos_map_id


@group("draft")
class PortQosMapAttributeTest(SaiHelperBase):
    ''' Test port qos map attributes '''

    def setUp(self):
        self.portx = None
        super(PortQosMapAttributeTest, self).setUp()

    def runTest(self):
        print("PortQosMapAttributeTest")
        self.portQosDscpToTcMapAttributeTest()
        self.portQosDot1pToColorMapAttributeTest()
        self.portQosDot1pToTcMapAttributeTest()
        self.portQosDscpToColorMapAttributeTest()
        self.portQosTcToQueueMapAttributeTest()
        self.portQosTcToPriorityGroupMapAttributeTest()
        self.portQosTcAndColorToDscpMapAttributeTest()
        self.portQosTcAndColorToDot1pMapAttributeTest()
        self.portQosPfcPriorityToQueueMapAttributeTest()

    def tearDown(self):
        print("PortQosMapAttributeTest tearDown")
        super(PortQosMapAttributeTest, self).tearDown()

    def portQosDscpToTcMapAttributeTest(self):
        ''' Test port qos dscp to tc map attribute '''
        print("portQosDscpToTcMapAttributeTest")
        # SAI_PORT_ATTR_QOS_DSCP_TO_TC_MAP - qos_dscp_to_tc_map
        # create/set
        ingress_key_list = [1, 2, 3, 4]
        ingress_data_list = [11, 12, 13, 14]
        qos_map_id = create_qos_map(
            self.client,
            SAI_QOS_MAP_TYPE_DSCP_TO_TC,
            ingress_key_list,
            ingress_data_list)
        self.assertTrue(qos_map_id != 0,
                        "Failed to create qos_map")
        try:
            # create/get
            qos_dscp_to_tc_map = None
            initial_qos_map_id = 0
            if TEST_QOS_MAP_ON_CREATE_PORT is True:
                initial_qos_map_id = qos_map_id
                qos_dscp_to_tc_map = qos_map_id
            sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
            self.portx = sai_thrift_create_port(
                self.client,
                hw_lane_list=sai_list,
                speed=TEST_DEFAULT_SPEED,
                qos_default_tc=TEST_QOS_DEFAULT_TC,
                qos_dscp_to_tc_map=qos_dscp_to_tc_map)
            self.assertTrue(self.portx != 0, "Failed to create port")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_dscp_to_tc_map=True)
            self.assertEqual(attr['qos_dscp_to_tc_map'], initial_qos_map_id)

            status = sai_thrift_set_port_attribute(self.client,
                                                   self.portx,
                                                   qos_dscp_to_tc_map=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")
            status = sai_thrift_set_port_attribute(
                self.client,
                self.portx,
                qos_dscp_to_tc_map=qos_map_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_dscp_to_tc_map=True)
            self.assertEqual(attr['qos_dscp_to_tc_map'], qos_map_id)

        finally:
            # Remove the qos_map
            sai_thrift_set_port_attribute(self.client,
                                          self.portx,
                                          qos_dscp_to_tc_map=0)
            sai_thrift_remove_qos_map(self.client, qos_map_id)
            sai_thrift_remove_port(self.client, self.portx)

    def portQosDot1pToColorMapAttributeTest(self):
        ''' Test port qos dot1p to color map attribute '''
        print("portQosDot1pToColorMapAttributeTest")
        # SAI_PORT_ATTR_QOS_DOT1P_TO_COLOR_MAP - qos_dot1p_to_color_map
        # create/set
        ingress_key_list = [1, 2, 3, 4]
        ingress_data_list = [11, 12, 13, 14]
        qos_map_id = create_qos_map(
            self.client,
            SAI_QOS_MAP_TYPE_DOT1P_TO_COLOR,
            ingress_key_list,
            ingress_data_list)
        self.assertTrue(qos_map_id != 0,
                        "Failed to create qos_map")
        try:
            # create/get
            qos_dot1p_to_color_map = None
            initial_qos_map_id = 0
            if TEST_QOS_MAP_ON_CREATE_PORT is True:
                initial_qos_map_id = qos_map_id
                qos_dot1p_to_color_map = qos_map_id
            sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
            self.portx = sai_thrift_create_port(
                self.client,
                hw_lane_list=sai_list,
                speed=TEST_DEFAULT_SPEED,
                qos_default_tc=TEST_QOS_DEFAULT_TC,
                qos_dot1p_to_color_map=qos_dot1p_to_color_map)
            self.assertTrue(self.portx != 0, "Failed to create port")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_dot1p_to_color_map=True)
            self.assertEqual(attr['qos_dot1p_to_color_map'],
                             initial_qos_map_id)

            status = sai_thrift_set_port_attribute(self.client,
                                                   self.portx,
                                                   qos_dot1p_to_color_map=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")
            # set/get
            status = sai_thrift_set_port_attribute(
                self.client,
                self.portx,
                qos_dot1p_to_color_map=qos_map_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_dot1p_to_color_map=True)
            self.assertEqual(attr['qos_dot1p_to_color_map'], qos_map_id)

        finally:
            # Remove the qos_map
            sai_thrift_set_port_attribute(self.client, self.portx,
                                          qos_dot1p_to_color_map=0)
            sai_thrift_remove_qos_map(self.client, qos_map_id)
            sai_thrift_remove_port(self.client, self.portx)

    def portQosDot1pToTcMapAttributeTest(self):
        ''' Test port qos dot1p to tc map attribute '''
        print("portQosDot1pToTcMapAttributeTest")
        # SAI_PORT_ATTR_QOS_DOT1P_TO_TC_MAP - qos_dot1p_to_tc_map
        # create/set
        ingress_key_list = [1, 2, 3, 4]
        ingress_data_list = [11, 12, 13, 14]
        qos_map_id = create_qos_map(
            self.client,
            SAI_QOS_MAP_TYPE_DOT1P_TO_TC,
            ingress_key_list,
            ingress_data_list)
        self.assertTrue(qos_map_id != 0,
                        "Failed to create qos_map")
        try:
            # create/get
            qos_dot1p_to_tc_map = None
            initial_qos_map_id = 0
            if TEST_QOS_MAP_ON_CREATE_PORT is True:
                initial_qos_map_id = qos_map_id
                qos_dot1p_to_tc_map = qos_map_id
            sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
            self.portx = sai_thrift_create_port(
                self.client,
                hw_lane_list=sai_list,
                speed=TEST_DEFAULT_SPEED,
                qos_default_tc=TEST_QOS_DEFAULT_TC,
                qos_dot1p_to_tc_map=qos_dot1p_to_tc_map)
            self.assertTrue(self.portx != 0, "Failed to create port")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_dot1p_to_tc_map=True)
            self.assertEqual(attr['qos_dot1p_to_tc_map'], initial_qos_map_id)

            status = sai_thrift_set_port_attribute(self.client, self.portx,
                                                   qos_dot1p_to_tc_map=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")
            # set/get
            status = sai_thrift_set_port_attribute(
                self.client,
                self.portx,
                qos_dot1p_to_tc_map=qos_map_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_dot1p_to_tc_map=True)
            self.assertEqual(attr['qos_dot1p_to_tc_map'], qos_map_id)

        finally:
            # Remove the qos_map
            sai_thrift_set_port_attribute(self.client, self.portx,
                                          qos_dot1p_to_tc_map=0)
            sai_thrift_remove_qos_map(self.client, qos_map_id)
            sai_thrift_remove_port(self.client, self.portx)

    def portQosDscpToColorMapAttributeTest(self):
        ''' Test port qos dscp to color map attribute '''
        print("portQosDscpToColorMapAttributeTest")
        # SAI_PORT_ATTR_QOS_DSCP_TO_COLOR_MAP - qos_dscp_to_color_map
        # create/set

        ingress_key_list = [1, 2, 3, 4]
        ingress_data_list = [11, 12, 13, 14]
        qos_map_id = create_qos_map(
            self.client,
            SAI_QOS_MAP_TYPE_DSCP_TO_COLOR,
            ingress_key_list,
            ingress_data_list)
        self.assertTrue(qos_map_id != 0,
                        "Failed to create qos_map")
        try:
            # create/get
            qos_dscp_to_color_map = None
            initial_qos_map_id = 0
            if TEST_QOS_MAP_ON_CREATE_PORT is True:
                initial_qos_map_id = qos_map_id
                qos_dscp_to_color_map = qos_map_id
            sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
            self.portx = sai_thrift_create_port(
                self.client,
                hw_lane_list=sai_list,
                speed=TEST_DEFAULT_SPEED,
                qos_default_tc=TEST_QOS_DEFAULT_TC,
                qos_dscp_to_color_map=qos_dscp_to_color_map)
            self.assertTrue(self.portx != 0, "Failed to create port")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_dscp_to_color_map=True)
            self.assertEqual(attr['qos_dscp_to_color_map'], initial_qos_map_id)

            status = sai_thrift_set_port_attribute(self.client, self.portx,
                                                   qos_dscp_to_color_map=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")
            # set/get
            status = sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_dscp_to_color_map=qos_map_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_dscp_to_color_map=True)
            self.assertEqual(attr['qos_dscp_to_color_map'], qos_map_id)

        finally:
            # Remove the qos_map
            sai_thrift_set_port_attribute(self.client, self.portx,
                                          qos_dscp_to_color_map=0)
            sai_thrift_remove_qos_map(self.client, qos_map_id)
            sai_thrift_remove_port(self.client, self.portx)

    def portQosTcToQueueMapAttributeTest(self):
        ''' Test port qos tc to queue map attribute '''
        print("portQosTcToQueueMapAttributeTest")
        # SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP - qos_tc_to_queue_map
        # create/set
        ingress_key_list = [1, 2, 3, 4]
        ingress_data_list = [1, 2, 3, 4]
        qos_map_id = create_qos_map(
            self.client,
            SAI_QOS_MAP_TYPE_TC_TO_QUEUE,
            ingress_key_list,
            ingress_data_list)
        self.assertTrue(qos_map_id != 0,
                        "Failed to create qos_map")
        try:
            # create/get
            qos_tc_to_queue_map = None
            initial_qos_map_id = 0
            if TEST_QOS_MAP_ON_CREATE_PORT is True:
                initial_qos_map_id = qos_map_id
                qos_tc_to_queue_map = qos_map_id
            sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
            self.portx = sai_thrift_create_port(
                self.client,
                hw_lane_list=sai_list,
                speed=TEST_DEFAULT_SPEED,
                qos_default_tc=TEST_QOS_DEFAULT_TC,
                qos_tc_to_queue_map=qos_tc_to_queue_map)

            self.assertTrue(self.portx != 0, "Failed to create port")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_tc_to_queue_map=True)
            self.assertEqual(attr['qos_tc_to_queue_map'], initial_qos_map_id)

            status = sai_thrift_set_port_attribute(self.client, self.portx,
                                                   qos_tc_to_queue_map=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")
            # set/get
            status = sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_tc_to_queue_map=qos_map_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_tc_to_queue_map=True)
            self.assertEqual(attr['qos_tc_to_queue_map'], qos_map_id)

        finally:
            # Remove the qos_map
            sai_thrift_set_port_attribute(self.client, self.portx,
                                          qos_tc_to_queue_map=0)
            sai_thrift_remove_qos_map(self.client, qos_map_id)
            sai_thrift_remove_port(self.client, self.portx)

    def portQosTcToPriorityGroupMapAttributeTest(self):
        ''' Test port qos tc to priority group map attribute '''
        print("portQosTcToPriorityGroupMapAttributeTest")
        # SAI_PORT_ATTR_QOS_TC_TO_PRIORITY_GROUP_MAP
        #  - qos_tc_to_priority_group_map
        # create/set
        map_list = []
        tc_to_pg_list = [[1, 1], [2, 1], [3, 1], [4, 1]]
        for tc, priority_group in tc_to_pg_list:
            tc_to_queue = sai_thrift_qos_map_t(
                key=sai_thrift_qos_map_params_t(
                    tc=tc), value=sai_thrift_qos_map_params_t(
                        pg=priority_group))
            map_list.append(tc_to_queue)
        qos_map_list = sai_thrift_qos_map_list_t(
            count=len(map_list), maplist=map_list)
        qos_map_id = sai_thrift_create_qos_map(
            self.client,
            type=SAI_QOS_MAP_TYPE_TC_TO_PRIORITY_GROUP,
            map_to_value_list=qos_map_list)
        self.assertTrue(qos_map_id != 0,
                        "Failed to create qos_map")
        try:
            # create/get
            qos_tc_to_priority_group_map = None
            initial_qos_map_id = 0
            if TEST_QOS_MAP_ON_CREATE_PORT is True:
                initial_qos_map_id = qos_map_id
                qos_tc_to_priority_group_map = qos_map_id
            sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
            self.portx = sai_thrift_create_port(
                self.client,
                hw_lane_list=sai_list,
                speed=TEST_DEFAULT_SPEED,
                qos_default_tc=TEST_QOS_DEFAULT_TC,
                qos_tc_to_priority_group_map=qos_tc_to_priority_group_map)
            self.assertTrue(self.portx != 0, "Failed to create port")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_tc_to_priority_group_map=True)
            self.assertEqual(attr['qos_tc_to_priority_group_map'],
                             initial_qos_map_id)

            status = sai_thrift_set_port_attribute(
                self.client,
                self.portx,
                qos_tc_to_priority_group_map=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")
            # set/get
            status = sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_tc_to_priority_group_map=qos_map_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_tc_to_priority_group_map=True)
            self.assertEqual(attr['qos_tc_to_priority_group_map'], qos_map_id)

        finally:
            # Remove the qos_map
            sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_tc_to_priority_group_map=0)
            sai_thrift_remove_qos_map(self.client, qos_map_id)
            sai_thrift_remove_port(self.client, self.portx)

    def portQosTcAndColorToDscpMapAttributeTest(self):
        ''' Test port qos tc and color to dscp map attribute '''
        print("portQosTcAndColorToDscpMapAttributeTest")
        # SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DSCP_MAP
        #  - qos_tc_and_color_to_dscp_map
        # create/set
        ingress_key_list = [[1, 1], [2, 1], [3, 1], [4, 1]]
        ingress_data_list = [1, 1, 1, 2]
        qos_map_id = create_qos_map(
            self.client,
            SAI_QOS_MAP_TYPE_TC_AND_COLOR_TO_DSCP,
            ingress_key_list,
            ingress_data_list)
        self.assertTrue(qos_map_id != 0,
                        "Failed to create qos_map")
        try:
            # create/get
            qos_tc_and_color_to_dscp_map = None
            initial_qos_map_id = 0
            if TEST_QOS_MAP_ON_CREATE_PORT is True:
                initial_qos_map_id = qos_map_id
                qos_tc_and_color_to_dscp_map = qos_map_id
            sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
            self.portx = sai_thrift_create_port(
                self.client,
                hw_lane_list=sai_list,
                speed=TEST_DEFAULT_SPEED,
                qos_default_tc=TEST_QOS_DEFAULT_TC,
                qos_tc_and_color_to_dscp_map=qos_tc_and_color_to_dscp_map)
            self.assertTrue(self.portx != 0, "Failed to create port")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_tc_and_color_to_dscp_map=True)
            self.assertEqual(attr['qos_tc_and_color_to_dscp_map'],
                             initial_qos_map_id)

            status = sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_tc_and_color_to_dscp_map=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")

            # set/get
            status = sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_tc_and_color_to_dscp_map=qos_map_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")
            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_tc_and_color_to_dscp_map=True)
            self.assertEqual(attr['qos_tc_and_color_to_dscp_map'], qos_map_id)

        finally:
            # Remove the qos_map
            sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_tc_and_color_to_dscp_map=0)
            sai_thrift_remove_qos_map(self.client, qos_map_id)
            sai_thrift_remove_port(self.client, self.portx)

    def portQosTcAndColorToDot1pMapAttributeTest(self):
        ''' Test port qos tc and color to dot1p map attribute '''
        print("portQosTcAndColorToDot1pMapAttributeTest")
        # SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP
        #   - qos_tc_and_color_to_dot1p_map
        # create/set
        ingress_key_list = [[1, 1], [2, 1], [3, 1], [4, 1]]
        ingress_data_list = [1, 1, 0, 0]
        qos_map_id = create_qos_map(
            self.client,
            SAI_QOS_MAP_TYPE_TC_AND_COLOR_TO_DOT1P,
            ingress_key_list,
            ingress_data_list)
        self.assertTrue(qos_map_id != 0,
                        "Failed to create qos_map")
        try:
            # create/get
            qos_tc_and_color_to_dot1p_map = None
            initial_qos_map_id = 0
            if TEST_QOS_MAP_ON_CREATE_PORT is True:
                initial_qos_map_id = qos_map_id
                qos_tc_and_color_to_dot1p_map = qos_map_id
            sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
            self.portx = sai_thrift_create_port(
                self.client,
                hw_lane_list=sai_list,
                speed=TEST_DEFAULT_SPEED,
                qos_default_tc=TEST_QOS_DEFAULT_TC,
                qos_tc_and_color_to_dot1p_map=qos_tc_and_color_to_dot1p_map)
            self.assertTrue(self.portx != 0, "Failed to create port")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_tc_and_color_to_dot1p_map=True)
            self.assertEqual(attr['qos_tc_and_color_to_dot1p_map'],
                             initial_qos_map_id)

            status = sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_tc_and_color_to_dot1p_map=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")

            # set/get
            status = sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_tc_and_color_to_dot1p_map=qos_map_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")
            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_tc_and_color_to_dot1p_map=True)
            self.assertEqual(attr['qos_tc_and_color_to_dot1p_map'], qos_map_id)

        finally:
            # Remove the qos_map
            sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_tc_and_color_to_dot1p_map=0)
            sai_thrift_remove_qos_map(self.client, qos_map_id)
            sai_thrift_remove_port(self.client, self.portx)

    def portQosPfcPriorityToQueueMapAttributeTest(self):
        ''' Test port qos pfc priority to queue map attribute '''
        print("portQosPfcPriorityToQueueMapAttributeTest")
        # SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_QUEUE_MAP
        #  - qos_pfc_priority_to_queue_map
        # create/set
        ingress_key_list = [1, 2, 3, 4]
        ingress_data_list = [1, 1, 1, 1]
        qos_map_id = create_qos_map(
            self.client,
            SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_QUEUE,
            ingress_key_list,
            ingress_data_list)
        self.assertTrue(qos_map_id != 0,
                        "Failed to create qos_map")
        try:
            # create/get
            qos_pfc_priority_to_queue_map = None
            initial_qos_map_id = 0
            if TEST_QOS_MAP_ON_CREATE_PORT is True:
                initial_qos_map_id = qos_map_id
                qos_pfc_priority_to_queue_map = qos_map_id
            sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
            self.portx = sai_thrift_create_port(
                self.client,
                hw_lane_list=sai_list,
                speed=TEST_DEFAULT_SPEED,
                qos_default_tc=TEST_QOS_DEFAULT_TC,
                qos_pfc_priority_to_queue_map=qos_pfc_priority_to_queue_map)
            self.assertTrue(self.portx != 0, "Failed to create port")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_pfc_priority_to_queue_map=True)
            self.assertEqual(attr['qos_pfc_priority_to_queue_map'],
                             initial_qos_map_id)

            status = sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_pfc_priority_to_queue_map=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")
            # set/get
            status = sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_pfc_priority_to_queue_map=qos_map_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, qos_pfc_priority_to_queue_map=True)
            self.assertEqual(attr['qos_pfc_priority_to_queue_map'], qos_map_id)

        finally:
            # Remove the qos_map
            sai_thrift_set_port_attribute(
                self.client, self.portx,
                qos_pfc_priority_to_queue_map=0)
            sai_thrift_remove_qos_map(self.client, qos_map_id)
            sai_thrift_remove_port(self.client, self.portx)

    def portQosPfcPriorityToPriorityGroupMapAttributeTest(self):
        ''' Test port qos pfc priority to priority group map attribute '''
        print("portQosPfcPriorityToPriorityGroupMapAttributeTest")
        # SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_PRIORITY_GROUP_MAP
        #  - qos_pfc_priority_to_priority_group_map
        # create/set
        ingress_key_list = [1]
        ingress_data_list = [0]
        qos_map_id = create_qos_map(
            self.client,
            SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_PRIORITY_GROUP,
            ingress_key_list,
            ingress_data_list)
        self.assertTrue(qos_map_id != 0,
                        "Failed to create qos_map")
        try:
            # create/get
            qos_pfc_ppg_map = None
            initial_qos_map_id = 0
            if TEST_QOS_MAP_ON_CREATE_PORT is True:
                initial_qos_map_id = qos_map_id

            sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
            self.portx = sai_thrift_create_port(
                self.client,
                hw_lane_list=sai_list,
                speed=TEST_DEFAULT_SPEED,
                qos_default_tc=TEST_QOS_DEFAULT_TC,
                qos_pfc_priority_to_priority_group_map=qos_pfc_ppg_map)
            self.assertTrue(self.portx != 0, "Failed to create port")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx,
                qos_pfc_priority_to_priority_group_map=True)
            self.assertEqual(attr['qos_pfc_priority_to_priority_group_map'],
                             initial_qos_map_id)

            if TEST_QOS_MAP_ON_CREATE_PORT is True:
                status = sai_thrift_set_port_attribute(
                    self.client, self.portx,
                    qos_pfc_priority_to_priority_group_map=0)
                self.assertEqual(status, SAI_STATUS_SUCCESS,
                                 "Failed to set port attribute")

            # set/get
            status = sai_thrift_set_port_attribute(
                self.client,
                self.portx,
                qos_pfc_priority_to_priority_group_map=qos_map_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port attribute")

            attr = sai_thrift_get_port_attribute(
                self.client, self.portx,
                qos_pfc_priority_to_priority_group_map=True)
            self.assertEqual(attr['qos_pfc_priority_to_priority_group_map'],
                             qos_map_id)

        finally:
            # Remove the qos_map
            sai_thrift_set_port_attribute(
                self.client, self.portx, qos_pfc_priority_to_priority_group_map=0)
            sai_thrift_remove_qos_map(self.client, qos_map_id)
            sai_thrift_remove_port(self.client, self.portx)


@group("draft")
class PortFloodClass(SaiHelperBase):
    ''' Verify SAI bridge port flood test case '''

    def setUp(self):
        super(PortFloodClass, self).setUp()
        self.test_port = None
        self.bp_port_list = []
        self.vlan_member_list = []
        self.test_port_list = [self.port0, self.port1, self.port2, self.port3]
        self.dev_test_port_list = [
            self.dev_port0,
            self.dev_port1,
            self.dev_port2,
            self.dev_port3]
        vlan_id = 1
        i = 0
        for port in self.test_port_list:
            attr = sai_thrift_set_port_attribute(self.client, port,
                                                 port_vlan_id=vlan_id)
            print(attr)

            bp_port = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=port,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            setattr(self, 'port%d_bp' % (i), bp_port)
            self.bp_port_list.append(bp_port)

            vlan_member = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.default_vlan_id,
                bridge_port_id=bp_port,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            self.vlan_member_list.append(vlan_member)
            i += 1

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client,
            entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        for vlan_member in self.vlan_member_list:
            sai_thrift_remove_vlan_member(self.client, vlan_member)
        for bp_port in self.bp_port_list:
            sai_thrift_remove_bridge_port(self.client, bp_port)
        for port in self.test_port_list:
            sai_thrift_set_port_attribute(
                self.client, port, port_vlan_id=0)
        super(PortFloodClass, self).tearDown()


@group("draft")
class PortFecModeAttributeTest(SaiHelper):
    ''' Test port fec mode attribute '''

    def setUp(self):
        self.portx = 0
        super(PortFecModeAttributeTest, self).setUp()

    def runTest(self):
        print("PortFecModeAttributeTest")
        try:
            test_config = []
            test_config.append({'speed': 10000,
                                'fec_mode': SAI_PORT_FEC_MODE_NONE,
                                'transit_fec': SAI_PORT_FEC_MODE_FC,
                                'auto_neg_mode': True})
            test_config.append({'speed': 10000,
                                'fec_mode': SAI_PORT_FEC_MODE_FC,
                                'transit_fec': SAI_PORT_FEC_MODE_NONE,
                                'auto_neg_mode': True})
            test_config.append({'speed': 25000,
                                'fec_mode': SAI_PORT_FEC_MODE_NONE,
                                'transit_fec': SAI_PORT_FEC_MODE_RS,
                                'auto_neg_mode': True})
            test_config.append({'speed': 25000,
                                'fec_mode': SAI_PORT_FEC_MODE_NONE,
                                'transit_fec': SAI_PORT_FEC_MODE_FC,
                                'auto_neg_mode': True})
            test_config.append({'speed': 25000,
                                'fec_mode': SAI_PORT_FEC_MODE_RS,
                                'transit_fec': SAI_PORT_FEC_MODE_FC,
                                'auto_neg_mode': True})
            test_config.append({'speed': 25000,
                                'fec_mode': SAI_PORT_FEC_MODE_RS,
                                'transit_fec': SAI_PORT_FEC_MODE_NONE,
                                'auto_neg_mode': True})
            test_config.append({'speed': 25000,
                                'fec_mode': SAI_PORT_FEC_MODE_FC,
                                'transit_fec': SAI_PORT_FEC_MODE_NONE,
                                'auto_neg_mode': True})
            test_config.append({'speed': 25000,
                                'fec_mode': SAI_PORT_FEC_MODE_FC,
                                'transit_fec': SAI_PORT_FEC_MODE_RS,
                                'auto_neg_mode': True})
            test_config.append({'speed': 50000,
                                'fec_mode': SAI_PORT_FEC_MODE_NONE,
                                'transit_fec': None,
                                'auto_neg_mode': True})
            test_config.append({'speed': 50000,
                                'fec_mode': SAI_PORT_FEC_MODE_FC,
                                'transit_fec': None,
                                'auto_neg_mode': True})
            test_config.append({'speed': 50000,
                                'fec_mode': SAI_PORT_FEC_MODE_RS,
                                'transit_fec': None,
                                'auto_neg_mode': True})
            for test in test_config:
                if test['transit_fec'] is not None:
                    print("Verify Port speed %d: FEC mode=%s --> FEC mode=%s"
                          % (test['speed'],
                             fec_to_str(test['fec_mode']),
                             fec_to_str(test['transit_fec'])))
                else:
                    print("Verify Port speed %d: FEC mode=%s"
                          % (test['speed'], fec_to_str(test['fec_mode'])))
                num_lanes = speed_to_num_lanes(test['speed'])
                if num_lanes == 1:
                    lane_list = sai_thrift_u32_list_t(count=num_lanes,
                                                      uint32list=[34])
                elif num_lanes == 2:
                    lane_list = sai_thrift_u32_list_t(count=num_lanes,
                                                      uint32list=[34, 35])
                elif num_lanes == 4:
                    lane_list = sai_thrift_u32_list_t(
                        count=num_lanes,
                        uint32list=[34, 35, 36, 37])
                global_flow = SAI_PORT_FLOW_CONTROL_MODE_RX_ONLY
                self.portx = sai_thrift_create_port(
                    self.client,
                    hw_lane_list=lane_list,
                    speed=test['speed'],
                    global_flow_control_mode=global_flow,
                    internal_loopback_mode=SAI_PORT_INTERNAL_LOOPBACK_MODE_MAC,
                    media_type=SAI_PORT_MEDIA_TYPE_COPPER,
                    auto_neg_mode=test['auto_neg_mode'],
                    admin_state=True,
                    fec_mode=test['fec_mode'])
                self.assertNotEqual(self.portx, 0)

                lane_list = sai_thrift_u32_list_t(count=10, uint32list=[])
                supported_fec_modes = sai_thrift_u32_list_t(count=10,
                                                            uint32list=[])
                print(supported_fec_modes)
                attr = sai_thrift_get_port_attribute(self.client,
                                                     self.portx,
                                                     oper_status=True,
                                                     speed=True,
                                                     fec_mode=True,
                                                     admin_state=True,
                                                     auto_neg_mode=True)

                self.assertEqual(attr['speed'], test['speed'])
                self.assertEqual(attr['fec_mode'], test['fec_mode'])
                self.assertEqual(attr['auto_neg_mode'], test['auto_neg_mode'])
                if test['transit_fec'] is not None:
                    sai_thrift_set_port_attribute(
                        self.client,
                        self.portx,
                        fec_mode=test['transit_fec'])
                    attr = sai_thrift_get_port_attribute(
                        self.client,
                        self.portx,
                        oper_status=True,
                        speed=True,
                        fec_mode=True,
                        admin_state=True,
                        auto_neg_mode=True)
                    self.assertEqual(attr['fec_mode'], test['transit_fec'])
                sai_thrift_set_port_attribute(self.client, self.portx,
                                              admin_state=False)
                attr = sai_thrift_get_port_attribute(
                    self.client,
                    self.portx,
                    admin_state=True,
                    oper_status=True,
                    speed=True,
                    auto_neg_mode=True)
                self.assertIs(attr['admin_state'], False)

                sai_thrift_set_port_attribute(self.client, self.portx,
                                              admin_state=True)
                attr = sai_thrift_get_port_attribute(self.client, self.portx,
                                                     admin_state=True,
                                                     oper_status=True,
                                                     speed=True,
                                                     auto_neg_mode=True)
                self.assertIs(attr['admin_state'], True)

                sai_thrift_remove_port(self.client, self.portx)
                self.portx = 0

        finally:
            if self.portx != 0:
                sai_thrift_remove_port(self.client, self.portx)

    def tearDown(self):
        print("PortFecModeAttributeTes tearDown")
        super(PortFecModeAttributeTest, self).tearDown()


@group("draft")
class PortSpeedAttributeTest(SaiHelperBase):
    ''' Test port speed attribute '''

    def setUp(self):
        self.portx = 0
        super(PortSpeedAttributeTest, self).setUp()

    def runTest(self):
        print("PortSpeedAttributeTest")
        try:
            test_config = []
            # Supported speeds
            test_config.append({'speed': 10000,
                                'speed_lanes': 1,
                                'transit_speed': 25000})
            test_config.append({'speed': 25000,
                                'speed_lanes': 1,
                                'transit_speed': 10000})

            for test in test_config:
                if test['transit_speed'] is not None:
                    num_lanes = speed_to_num_lanes(test['transit_speed'])
                    print("Verify speed: %d(lanes=%d) --> %d(lanes=%d)"
                          % (test['speed'],
                             test['speed_lanes'],
                             test['transit_speed'],
                             num_lanes))
                else:
                    print("Verify speed: %d(lanes=%d)"
                          % (test['speed'], test['speed_lanes']))
                num_lanes = test['speed_lanes']
                lane_list = num_lanes_to_lane_list(num_lanes)
                control_mode = SAI_PORT_FLOW_CONTROL_MODE_RX_ONLY
                self.portx = sai_thrift_create_port(
                    self.client,
                    hw_lane_list=lane_list,
                    speed=test['speed'],
                    global_flow_control_mode=control_mode,
                    internal_loopback_mode=SAI_PORT_INTERNAL_LOOPBACK_MODE_MAC,
                    media_type=SAI_PORT_MEDIA_TYPE_COPPER,
                    admin_state=True)
                self.assertNotEqual(self.portx, 0)
                lane_list = sai_thrift_u32_list_t(count=10, uint32list=[])
                attr = sai_thrift_get_port_attribute(self.client,
                                                     self.portx,
                                                     oper_status=True,
                                                     speed=True,
                                                     hw_lane_list=lane_list)
                self.assertEqual(attr['hw_lane_list'].count,
                                 test['speed_lanes'])
                self.assertEqual(attr['speed'], test['speed'])

                if test['transit_speed'] is not None:
                    num_lanes = speed_to_num_lanes(test['transit_speed'])
                    lane_list = num_lanes_to_lane_list(num_lanes)
                    sai_thrift_set_port_attribute(
                        self.client, self.portx,
                        speed=test['transit_speed'])
                    attr = sai_thrift_get_port_attribute(
                        self.client,
                        self.portx,
                        speed=True,
                        hw_lane_list=lane_list)
                    self.assertEqual(attr['speed'], test['transit_speed'])
                    self.assertEqual(attr['hw_lane_list'].count, num_lanes)
                sai_thrift_remove_port(self.client, self.portx)
                self.portx = 0

        finally:
            if self.portx != 0:
                sai_thrift_remove_port(self.client, self.portx)

    def tearDown(self):
        print("PortSpeedAttributeTest tearDown")
        super(PortSpeedAttributeTest, self).tearDown()


@group("draft")
class PortAutoNegAttributeTest(SaiHelperBase):
    ''' Test auto negation attribute '''

    def setUp(self):
        self.portx = None
        super(PortAutoNegAttributeTest, self).setUp()

    def runTest(self):
        print("PortAutoNegAttributeTest")

        test_config = []
        supported_speed = [1000, 10000, 25000]
        advertised_fec_mode = [SAI_PORT_FEC_MODE_NONE,
                               SAI_PORT_FEC_MODE_FC,
                               SAI_PORT_FEC_MODE_RS]
        advertised_fec_mode_list = sai_thrift_s32_list_t(
            count=10,
            int32list=advertised_fec_mode)
        print(advertised_fec_mode_list)
        self.portx = 0

        try:
            test_config.append({'speed': 10000,
                                'transit_speed': None,
                                'autoneg': True,
                                'supported_speed': supported_speed})
            test_config.append({'speed': 25000,
                                'transit_speed': None,
                                'autoneg': True,
                                'supported_speed': supported_speed})

            # Start the test loop
            for test in test_config:
                if test['transit_speed'] is not None:
                    print("Verify speed transit: %d --> %d"
                          % (test['speed'], test['transit_speed']))
                else:
                    print("Verify speed: ", test['speed'])

                num_lanes = speed_to_num_lanes(test['speed'])
                lane_list = num_lanes_to_lane_list(num_lanes)
                flow_mode = SAI_PORT_FLOW_CONTROL_MODE_RX_ONLY
                loopback_mode = SAI_PORT_INTERNAL_LOOPBACK_MODE_MAC
                self.portx = sai_thrift_create_port(
                    self.client,
                    hw_lane_list=lane_list,
                    speed=test['speed'],
                    global_flow_control_mode=flow_mode,
                    internal_loopback_mode=loopback_mode,
                    media_type=SAI_PORT_MEDIA_TYPE_COPPER,
                    admin_state=True,
                    auto_neg_mode=test['autoneg'])

                self.assertTrue(self.portx != 0, "Failed to create the port")

                supported_speed_list = sai_thrift_s32_list_t(
                    count=10, int32list=[])
                advertised_speed_list = sai_thrift_s32_list_t(
                    count=10, int32list=[])

                attr = sai_thrift_get_port_attribute(
                    self.client,
                    self.portx,
                    speed=True,
                    auto_neg_mode=True,
                    supported_speed=supported_speed_list,
                    advertised_speed=advertised_speed_list)

                self.assertEqual(attr['speed'], test['speed'])
                self.assertIs(attr['auto_neg_mode'], True)

                supported_speed_list = attr['supported_speed']
                supported_speeds = ""
                for speed in supported_speed_list.uint32list:
                    supported_speeds += str(speed) + ", "
                verified_all = True
                for speed in supported_speed_list.uint32list:
                    # Check if we can set the port supported speed
                    verified = set_port_speed(
                        self.client,
                        self.portx,
                        speed,
                        verify=True)
                    if verified:
                        s = "OK"
                    else:
                        s = "Failed"
                    print(" Verify port supported speed: ",
                          "%d (suported_speeds=%s) %s"
                          % (speed, supported_speeds, s))
                    verified_all &= verified

                self.assertTrue(verified_all,
                                "Failed to verify port supported speeds")
                sai_thrift_remove_port(self.client, self.portx)
                self.portx = 0

        finally:
            if self.portx != 0:
                sai_thrift_remove_port(self.client, self.portx)

    def tearDown(self):
        print("PortAutoNegAttributeTest tearDown")
        super(PortAutoNegAttributeTest, self).tearDown()


def set_port_speed(client, port, requested_speed, verify=None):
    '''
    Set the port speed

    Args:
        client (sai_thrift.sai_rpc.Client): RPC client
        port (int): port number
        requested_speed (int): speed value
        verify (bool): True if succesfully verified

    Returns:
        bool: True if verify is True
    '''
    sai_thrift_set_port_attribute(client, port,
                                  speed=requested_speed)
    if verify is True:
        attr = sai_thrift_get_port_attribute(client, port, speed=True)
        if attr['speed'] != requested_speed:
            return False
    return True


@group("draft")
class PortAclBindingClass(PortFloodClass):
    ''' Port ACL binding class '''

    def setUp(self):
        super(PortAclBindingClass, self).setUp()

        self.port_pairs = []
        # Create port object and port_dev pairs.
        # This allows to mapping the:
        #   - dev_port -> port_obj and
        #   - port_obj -> dev_port
        for dev_no in range(0, self.active_ports):
            dev_port = getattr(self, 'dev_port%d' % dev_no)
            port_obj = getattr(self, 'port%d' % dev_no)
            self.port_pairs.append([port_obj, dev_port])

    def tearDown(self):
        print("PortAclBindingClass tearDown")
        super(PortAclBindingClass, self).tearDown()

    def findPortObj(self, port):
        '''
        Find port object in the port_pairs array based on dev_port number.
        Returns 0 if not found.

        Args:
            port (int): port number

        Returns:
            int: port object number
        '''
        for portobj, port_dev in self.port_pairs:
            if port == port_dev:
                return portobj
        return 0

    def portsToPortObjs(self, ports):
        '''
        Convert ports to port objects

        Args:
            ports (list): list of ports

        Returns:
            list: list of port objects
        '''
        port_objs = []
        for port in ports:
            port_obj = self.findPortObj(port)
            port_objs.append(port_obj)
        return port_objs

    def getPortStats(self, ports, port_stats):
        '''
        Get port statistics

        Args:
            ports (list): list of ports
            port_stats (list):  list of port statistics

        Returns:
            int: total cnt value
        '''
        total_cnt = 0
        for port in ports:
            # Find the port object for a given port
            port_obj = self.findPortObj(port)
            for port_stat in port_stats:
                initial_stats = sai_thrift_get_port_stats(self.client,
                                                          port_obj)
                stat = initial_stats[port_stat]
                total_cnt += stat
        return total_cnt

    def assignPortIngressAcl(self, ports, acl=0):
        '''
        Assign port ingress acl

        Args:
            ports (list): list of ports
            acl (int): acl number
        '''
        print("assign ingress acl=0x%x on ports" % (acl), ports)
        port_objs = self.portsToPortObjs(ports)
        for port_obj in port_objs:
            status = sai_thrift_set_port_attribute(
                self.client,
                port_obj,
                ingress_acl=acl)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def assignPortEgressAcl(self, ports, acl=0):
        '''
        Assign port egress acl

        Args:
            ports (list): list of ports
            acl (int): acl number
        '''
        print("assign egress acl=0x%x on ports" % (acl), ports)
        port_objs = self.portsToPortObjs(ports)
        for port_obj in port_objs:
            status = sai_thrift_set_port_attribute(
                self.client,
                port_obj,
                egress_acl=acl)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def clearAcl(self, acl_list):
        '''
        Clear acl

        Args:
            acl_list (list): list of acls
        '''
        for acl in acl_list:
            sai_thrift_remove_acl_table_group_member(self.client,
                                                     acl['acl_group_member'])
            sai_thrift_remove_acl_table_group(self.client,
                                              acl['acl_table_group'])
            sai_thrift_remove_acl_entry(self.client, acl['acl_entry'])
            sai_thrift_remove_acl_table(self.client, acl['acl_table'])

    def setupPortIngresDropAcl(
            self,
            dmac=None,
            dip=None,
            action=SAI_PACKET_ACTION_DROP,
            mac_mask='FF:FF:FF:FF:FF:FF'):
        '''
        Set up port ingress drop acl

        Args:
            dmac (str): destination mac address
            dip (str): destination ip address
            action (int): sai packet action value
            mac_mask (str): mask value

        Returns:
            list: list of acls
        '''

        stage = SAI_ACL_STAGE_INGRESS
        bind_points = [SAI_ACL_BIND_POINT_TYPE_PORT]
        action_types = [SAI_ACL_ACTION_TYPE_PACKET_ACTION]
        dip_mask = '255.255.255.0'

        acl_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(bind_points), int32list=bind_points)
        acl_action_type_list = sai_thrift_s32_list_t(
            count=len(action_types), int32list=action_types)

        if dip is not None:
            dip_ind = True
            ip = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip4=dip),
                mask=sai_thrift_acl_field_data_mask_t(ip4=dip_mask))
        else:
            dip_ind = None
            ip = None
        if dmac is not None:
            dmac_ind = True
            mac = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(mac=dmac),
                mask=sai_thrift_acl_field_data_mask_t(mac=mac_mask))
        else:
            dmac_ind = None
            mac = None

        acl_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=acl_bind_point_type_list,
            acl_action_type_list=acl_action_type_list,
            field_dst_ip=dip_ind,
            field_dst_mac=dmac_ind)
        self.assertNotEqual(acl_table_id, 0)

        action_drop = action
        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(s32=action_drop))
        acl_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_id,
            action_packet_action=packet_action,
            field_dst_ip=ip,
            field_dst_mac=mac)
        self.assertNotEqual(acl_entry, 0)

        acl_table_group = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=acl_bind_point_type_list,
            type=SAI_ACL_TABLE_GROUP_TYPE_PARALLEL)
        acl_group_member = sai_thrift_create_acl_table_group_member(
            self.client,
            acl_table_group_id=acl_table_group,
            acl_table_id=acl_table_id)

        acl = []
        acl.append({
            'acl_table': acl_table_id,
            'acl_entry': acl_entry,
            'acl_table_group': acl_table_group,
            'acl_group_member': acl_group_member})
        return acl

    def setupPortEgressDropAcl(
            self,
            dmac=None,
            dip=None,
            action=SAI_PACKET_ACTION_DROP,
            mac_mask='FF:FF:FF:FF:FF:FF'):
        '''
        Set up port egress drop acl

        Args:
            dmac (str): destination mac address
            dip (str): destination ip address
            action (int): sai packet action value
            mac_mask (str): mask value

        Returns:
            list: list of acls
        '''

        stage = SAI_ACL_STAGE_EGRESS
        bind_points = [SAI_ACL_BIND_POINT_TYPE_PORT]
        action_types = [SAI_ACL_ACTION_TYPE_PACKET_ACTION]
        dip_mask = '255.255.255.0'

        acl_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(bind_points), int32list=bind_points)
        acl_action_type_list = sai_thrift_s32_list_t(
            count=len(action_types), int32list=action_types)

        if dip is not None:
            dip_ind = True
            ip = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip4=dip),
                mask=sai_thrift_acl_field_data_mask_t(ip4=dip_mask))
        else:
            dip_ind = None
            ip = None
        if dmac is not None:
            dmac_ind = True
            mac = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(mac=dmac),
                mask=sai_thrift_acl_field_data_mask_t(mac=mac_mask))
        else:
            dmac_ind = None
            mac = None

        acl_table = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=acl_bind_point_type_list,
            acl_action_type_list=acl_action_type_list,
            field_dst_ip=dip_ind,
            field_dst_mac=dmac_ind)
        self.assertNotEqual(acl_table, 0)

        action_drop = action
        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(s32=action_drop))
        acl_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table,
            action_packet_action=packet_action,
            field_dst_ip=ip,
            field_dst_mac=mac)
        self.assertNotEqual(acl_entry, 0)

        acl_table_group = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=acl_bind_point_type_list,
            type=SAI_ACL_TABLE_GROUP_TYPE_PARALLEL)
        acl_group_member = sai_thrift_create_acl_table_group_member(
            self.client,
            acl_table_group_id=acl_table_group,
            acl_table_id=acl_table)

        acl = []
        acl.append({
            'acl_table': acl_table,
            'acl_entry': acl_entry,
            'acl_table_group': acl_table_group,
            'acl_group_member': acl_group_member})
        return acl

    def portIngressAclBindingSinglePortTest(self, use_acl_group=True):
        '''
        Test port ingress acl binding single port

        Args:
            use_acl_group (bool): if True uses acl group
        '''
        name = "portIngressAclBindingSinglePortTest"
        if use_acl_group:
            name += " ACL table group"
        else:
            name += " ACL table"
        print(name)

        dst_ip = '10.10.10.1'
        src_mac = "00:11:11:11:11:11"
        dst_mac = "00:22:22:22:22:22"

        pkt = simple_tcp_packet(
            eth_dst=dst_mac,
            eth_src=src_mac,
            ip_dst='10.10.10.2',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)

        test_port = self.dev_port1
        flood_port_list = []
        flood_pkt_list = []
        for port in self.dev_test_port_list:
            if port == test_port:
                continue
            flood_pkt_list.append(pkt)
            flood_port_list.append([port])
        print("flood_port_list=", flood_port_list)
        print("Sending packet on port%d with bridge port, no ACL enabled"
              % (test_port))
        send_packet(self, test_port, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, flood_pkt_list, flood_port_list)
        print("\tPacket flooded. OK")

        acl_list = []
        try:
            # Configure ACL and assign to port ingress_acl
            ports_with_acl = [self.dev_port0, self.dev_port1]
            acl_list = self.setupPortIngresDropAcl(dip=dst_ip)
            if use_acl_group is True:
                ingress_acl = acl_list[0]['acl_table_group']
            else:
                ingress_acl = acl_list[0]['acl_table']
            self.assignPortIngressAcl(ports_with_acl, acl=ingress_acl)

            for port in ports_with_acl:
                time.sleep(TEST_SLEEP_TIME)
                if_in_discards_pre = self.getPortStats(
                    [port],
                    ['SAI_PORT_STAT_IF_IN_DISCARDS'])
                print("Sending packet on bridge port%d, ingress_acl enabled"
                      % (port))
                send_packet(self, port, pkt)
                verify_no_other_packets(self, timeout=1)
                print("\tPacket dropped. OK")
                time.sleep(TEST_SLEEP_TIME)
                if_in_discards = self.getPortStats(
                    [port],
                    ['SAI_PORT_STAT_IF_IN_DISCARDS'])
                print("if_in_discards_pre=", if_in_discards_pre,
                      "if_in_discards", if_in_discards)
                assert if_in_discards_pre + 1 == if_in_discards

            self.assignPortIngressAcl(ports_with_acl, acl=0)

            print("Sending packet on bridge port %d, ingress_acl disabled"
                  % (self.dev_port0))
            send_packet(self, self.dev_port1, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, flood_port_list)
            print("\tPacket flooded. OK")

        finally:
            self.assignPortIngressAcl(
                [self.dev_port0, self.dev_port1,
                 self.dev_port2, self.dev_port3], acl=0)
            self.clearAcl(acl_list)

    def portIngressAclBindingTest(self, add_remove_bind=None,
                                  use_acl_group=None):
        '''
        Test port ingress acl binding

        Args:
            add_remove_bind(bool): if True adds/removes bind
            use_acl_group (bool): if True uses acl group
        '''
        name = "portIngressAclBindingTest"
        acl_test_case(name, add_remove_bind, use_acl_group)

        dst_ip = '10.10.10.1'
        src_mac = "00:11:11:11:11:11"
        dst_mac = "00:22:22:22:22:22"
        pkt = simple_tcp_packet(
            eth_dst=dst_mac,
            eth_src=src_mac,
            ip_dst='10.10.10.2',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)

        test_port = self.dev_port1
        flood_port_list = []
        flood_pkt_list = []
        for port in self.dev_test_port_list:
            if port == test_port:
                continue
            flood_pkt_list.append(pkt)
            flood_port_list.append([port])

        print("Sending packet on port with bridge port, no ACL enabled")
        send_packet(self, test_port, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, flood_pkt_list, flood_port_list)
        print("\tPacket flooded. OK")

        acl_list = []
        try:
            # Configure ACL and assign to port egress_acl
            ports_with_acl = [self.dev_port0, self.dev_port3]
            acl_list = self.setupPortIngresDropAcl(dip=dst_ip)

            if use_acl_group is True:
                ingress_acl = acl_list[0]['acl_table_group']
            else:
                ingress_acl = acl_list[0]['acl_table']
            status = self.assignPortIngressAcl(ports_with_acl, acl=ingress_acl)
            print("Status: ", status)

            print("Sending packet on bridge port, ingress_acl disabled")
            send_packet(self, self.dev_port1, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, flood_port_list)
            print("\tPacket flooded. OK")

            flood_port_list = [[self.dev_port0],
                               [self.dev_port1],
                               [self.dev_port3]]
            flood_pkt_list = [pkt, pkt, pkt]
            print("Sending packet on bridge port, ingress_acl disabled")
            send_packet(self, self.dev_port2, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, flood_port_list)
            print("\tPacket flooded. OK")

            # Verify ingress_acl drops for all ports with acl binded
            for port in ports_with_acl:
                # Get initial discard counter
                time.sleep(TEST_SLEEP_TIME)
                if_in_discards_pre = self.getPortStats(
                    [port], ['SAI_PORT_STAT_IF_IN_DISCARDS'])

                print("Sending packet on bridge port%d, ingress_acl enabled"
                      % (port))
                send_packet(self, port, pkt)
                verify_no_other_packets(self, timeout=1)
                print("\tPacket dropped. OK")
                # Get after drop discard counter
                time.sleep(TEST_SLEEP_TIME)
                if_in_discards = self.getPortStats(
                    [port], ['SAI_PORT_STAT_IF_IN_DISCARDS'])
                print("if_in_discards_pre=", if_in_discards_pre,
                      "if_in_discards", if_in_discards)
                assert if_in_discards_pre + 1 == if_in_discards

            # Verify add and remove binding to/from single port
            if add_remove_bind is True:
                print("Checking add/remove-bind. Bind ingress_acl to port 2")
                self.assignPortIngressAcl([self.dev_port2],
                                          acl=ingress_acl)

                ports_with_acl = [self.dev_port0, self.dev_port2,
                                  self.dev_port3]
                for port in ports_with_acl:
                    # Port with ingress acl enabled
                    print("Sending packet on bridge port%d,"
                          " ingress_acl enabled" % (port))
                    send_packet(self, port, pkt)
                    verify_no_other_packets(self, timeout=1)
                    print("\tPacket Dropped. OK")

                flood_port_list = [[self.dev_port0],
                                   [self.dev_port2],
                                   [self.dev_port3]]
                flood_pkt_list = [pkt, pkt, pkt]

                print("Sending packet on bridged port%d,"
                      " ingress_acl disabled" % (test_port))
                send_packet(self, test_port, pkt)
                verify_each_packet_on_multiple_port_lists(
                    self, flood_pkt_list, flood_port_list)
                print("\tPacket flooded. OK")

                # Clear port 2 binding
                self.assignPortIngressAcl([self.dev_port2], acl=0)

                ports_with_acl = [self.dev_port0, self.dev_port3]
                ports_without_acl = [self.dev_port1, self.dev_port2]
                for port in ports_with_acl:
                    print("Sending packet on bridged port%d,"
                          " ingress_acl enabled" % (port))
                    send_packet(self, port, pkt)
                    verify_no_other_packets(self, timeout=1)
                    print("\tPacket Dropped. OK")

                self.floodTest(pkt, ports_without_acl, self.dev_test_port_list)

            # Clear all ports binding
            self.assignPortIngressAcl([self.dev_port0, self.dev_port3], acl=0)

            ports_without_acl = [self.dev_port0,
                                 self.dev_port1,
                                 self.dev_port2,
                                 self.dev_port3]

            self.floodTest(pkt, ports_without_acl, self.dev_test_port_list)

        finally:
            # Clear all ports binding
            self.assignPortIngressAcl(
                [self.dev_port0, self.dev_port1,
                 self.dev_port2, self.dev_port3], acl=0)
            self.clearAcl(acl_list)

    def portEgressAclBindingTest(self, add_remove_bind=None,
                                 use_acl_group=None):
        '''
        Test port egress acl binding

        Args:
            add_remove_bind(bool): if True adds/removes bind
            use_acl_group (bool): if True uses acl group
        '''
        name = "portEgressAclBindingTest"
        acl_test_case(name, add_remove_bind, use_acl_group)

        dst_ip = '10.10.10.1'
        src_mac = "00:11:11:11:11:11"
        dst_mac = "00:22:22:22:22:22"

        pkt = simple_tcp_packet(
            eth_dst=dst_mac,
            eth_src=src_mac,
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)

        dev_port_list = [self.dev_port0,
                         self.dev_port1,
                         self.dev_port2,
                         self.dev_port3]

        test_port = self.dev_port1
        flood_port_list = []
        flood_pkt_list = []
        for port in self.dev_test_port_list:
            if port == test_port:
                continue
            flood_pkt_list.append(pkt)
            flood_port_list.append([port])

        print("Sending packet on breidged port%d, no ACL enabled"
              % (self.dev_port1))
        send_packet(self, self.dev_port1, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, flood_pkt_list, flood_port_list)
        print("\tPacket flooded. OK")

        acl_list = []
        try:
            # Configure ACL and assign to port egress_acl
            acl_list = self.setupPortEgressDropAcl(dip=dst_ip)
            if use_acl_group is True:
                egress_acl = acl_list[0]['acl_table_group']
            else:
                egress_acl = acl_list[0]['acl_table']
            status = self.assignPortEgressAcl([self.dev_port0, self.dev_port3],
                                              acl=egress_acl)
            print("Status: ", status)

            flood_port_list = [[self.dev_port2]]
            flood_pkt_list = [pkt]

            port0_if_out_discards_pre = self.getPortStats(
                [self.dev_port0],
                ['SAI_PORT_STAT_IF_OUT_DISCARDS'])
            port3_if_out_discards_pre = self.getPortStats(
                [self.dev_port3],
                ['SAI_PORT_STAT_IF_OUT_DISCARDS'])
            print("Sending packet on bridged port%d, egress_acl enabled"
                  % (self.dev_port1))
            send_packet(self, self.dev_port1, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, flood_port_list)
            print("\tPacket flooded. OK")

            # Check port out drop statistics
            if_out_discards = self.getPortStats(
                [self.dev_port0],
                ['SAI_PORT_STAT_IF_OUT_DISCARDS'])
            assert port0_if_out_discards_pre + 1 == if_out_discards
            if_out_discards = self.getPortStats(
                [self.dev_port3],
                ['SAI_PORT_STAT_IF_OUT_DISCARDS'])
            assert port3_if_out_discards_pre + 1 == if_out_discards

            if add_remove_bind:
                # Bind egress_acl to port 2
                print("\nChecking add/remove-bind. Bind egress_acl to port 2")
                self.assignPortEgressAcl([self.dev_port2],
                                         acl=egress_acl)

                ports_with_acl = [
                    self.dev_port0,
                    self.dev_port2,
                    self.dev_port3]
                print(ports_with_acl)

                print("Sending packet on bridged port%d, egress_acl enabled"
                      % (self.dev_port1))
                send_packet(self, self.dev_port1, pkt)
                verify_no_other_packets(self, timeout=1)
                print("\tPacket Dropped. OK")

                # Remove egress_acl from port 2
                self.assignPortEgressAcl([self.dev_port2], acl=0)
                ports_without_acl = [self.dev_port1, self.dev_port2]
                self.floodTest(pkt, dev_port_list, ports_without_acl)

            # Remove egress_acl from all ports
            self.assignPortEgressAcl([self.dev_port0, self.dev_port3], acl=0)

            # Verify that all packets are forwarded to all ports after
            # egress_acl was removed from all ports
            ports_without_acl = [self.dev_port0,
                                 self.dev_port1,
                                 self.dev_port2,
                                 self.dev_port3]

            self.floodTest(pkt, dev_port_list, ports_without_acl)

        finally:
            # Make sure ACL is not associated with any port
            self.assignPortEgressAcl(dev_port_list, acl=0)
            self.clearAcl(acl_list)

    def floodTest(self, pkt, port_list1, port_list2):
        '''
        Test flooding

        Args:
            pkt (Ether): simple tcp test packet
            port_list1 (list): test port list
            port_list2 (list): test port list
        '''
        for port in port_list1:
            flood_port_list = []
            flood_pkt_list = []
            for flood_port in port_list2:
                if flood_port == port:
                    continue
                flood_pkt_list.append(pkt)
                flood_port_list.append([flood_port])

            print("Sending packet on bridged port%d,"
                  " all acl disabled" % (port))
            send_packet(self, port, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, flood_port_list)
            print("\tPacket flooded. OK")

    def portEgressAclBindingSinglePortTest(self, use_acl_group=None):
        '''
        Test port egress acl binding single port

        Args:
            use_acl_group (bool): if True uses acl group
        '''
        name = "portEgressAclBindingSinglePortTest"
        if use_acl_group:
            name += " ACL table group"
        else:
            name += " ACL table"
        print(name)

        dst_ip = '10.10.10.1'
        src_mac = "00:11:11:11:11:11"
        dst_mac = "00:22:22:22:22:22"

        pkt = simple_tcp_packet(
            eth_dst=dst_mac,
            eth_src=src_mac,
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)

        test_port = self.dev_port1
        flood_port_list = []
        flood_pkt_list = []
        for port in self.dev_test_port_list:
            if port == test_port:
                continue
            flood_pkt_list.append(pkt)
            flood_port_list.append([port])

        print("Sending packet on port with bridge port, no ACL enabled")
        send_packet(self, test_port, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, flood_pkt_list, flood_port_list)
        print("\tPacket flooded. OK", flood_port_list)

        acl_list = []
        ports_with_acl = [self.dev_port0, self.dev_port1]
        try:
            # Configure ACL and assign to port egress_acl
            acl_list = self.setupPortEgressDropAcl(dip=dst_ip)
            if use_acl_group is True:
                egress_acl = acl_list[0]['acl_table_group']
            else:
                egress_acl = acl_list[0]['acl_table']
            status = self.assignPortEgressAcl(ports_with_acl, acl=egress_acl)
            print("Status: ", status)

            flood_port_list = [[self.dev_port2], [self.dev_port3]]
            flood_pkt_list = [pkt, pkt]
            port0_if_out_discards_pre = self.getPortStats(
                [self.dev_port0], ['SAI_PORT_STAT_IF_OUT_DISCARDS'])
            print("Sending packet on bridged port, egress_acl enabled")
            send_packet(self, test_port, pkt)

            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, flood_port_list)

            # Check port out drop statistics
            if_out_discards = self.getPortStats(
                [self.dev_port0],
                ['SAI_PORT_STAT_IF_OUT_DISCARDS'])
            assert port0_if_out_discards_pre + 1 == if_out_discards

            self.assignPortEgressAcl(ports_with_acl, acl=0)
            flood_port_list = [[self.dev_port0],
                               [self.dev_port2],
                               [self.dev_port3]]
            flood_pkt_list = [pkt, pkt, pkt]
            print("Sending packet on bridged port, egress_acl disabled")
            send_packet(self, self.dev_port1, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, flood_port_list)
            print("\tPacket flooded. OK")

        finally:
            # Make sure ACL is not associated with any port
            self.assignPortEgressAcl(ports_with_acl, acl=0)

            self.clearAcl(acl_list)


@group("draft")
class PortEgressMirrorSessionTest(PortFloodClass):
    ''' Test port PortEgressMirrorSessionTest attributes '''

    def setUp(self):
        self.fdb_entry = None
        self.portx = None
        super(PortEgressMirrorSessionTest, self).setUp()

    def runTest(self):

        try:
            empty_obj_list = sai_thrift_object_list_t(count=0, idlist=[])

            # Create an egress_mirror_session for port0
            mirror_id = sai_thrift_create_mirror_session(
                self.client, type=SAI_MIRROR_SESSION_TYPE_LOCAL,
                monitor_port=self.port0)
            self.assertTrue(mirror_id != 0, "Failed to create port mirror_id")

            egress_mirror_obj_list = [mirror_id]
            egress_mirror_session = sai_thrift_object_list_t(
                count=len(egress_mirror_obj_list),
                idlist=egress_mirror_obj_list)

            # create/get
            sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
            self.portx = sai_thrift_create_port(
                self.client,
                hw_lane_list=sai_list,
                speed=TEST_DEFAULT_SPEED,
                egress_mirror_session=egress_mirror_session)
            self.assertTrue(self.portx != 0, "Failed to create port")

            sai_list = sai_thrift_object_list_t(count=20, idlist=[])
            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, egress_mirror_session=sai_list)

            self.assertEqual(attr['egress_mirror_session'].count,
                             len(egress_mirror_obj_list))
            self.assertEqual(attr['egress_mirror_session'].idlist[0],
                             mirror_id)

            # Clear egress_mirror_session
            status = sai_thrift_set_port_attribute(
                self.client,
                self.portx,
                egress_mirror_session=empty_obj_list)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to clear port egress_mirror_session")
            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, egress_mirror_session=sai_list)
            self.assertEqual(attr['egress_mirror_session'].count, 0)

            # Trafic test:
            # set/get
            status = sai_thrift_set_port_attribute(
                self.client, self.port3,
                egress_mirror_session=egress_mirror_session)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port egress_mirror_session")
            sai_list = sai_thrift_object_list_t(count=20, idlist=[])
            attr = sai_thrift_get_port_attribute(
                self.client, self.port3, egress_mirror_session=sai_list)

            # Must be exactly 1
            self.assertEqual(attr['egress_mirror_session'].count, 1)
            self.assertEqual(attr['egress_mirror_session'].count,
                             len(egress_mirror_obj_list))
            self.assertEqual(attr['egress_mirror_session'].idlist[0],
                             mirror_id)

            # Check traffic
            self.fdb_entry = sai_thrift_fdb_entry_t(
                switch_id=self.switch_id,
                mac_address='00:00:00:00:00:33',
                bv_id=self.default_vlan_id)
            self.assertTrue(self.fdb_entry != 0, "Failed to create fdb_entry")
            status = sai_thrift_create_fdb_entry(
                self.client,
                self.fdb_entry,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=self.port3_bp)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            pkt = simple_tcp_packet(
                eth_dst='00:00:00:00:00:33',
                eth_src='00:22:22:22:22:22',
                ip_ttl=64)

            pkt_ingress_mirror = simple_tcp_packet(
                eth_dst='00:00:00:00:00:33',
                eth_src='00:22:22:22:22:22',
                ip_ttl=64)

            flood_port_list = [[self.dev_port0],
                               [self.dev_port3]]
            flood_pkt_list = [pkt_ingress_mirror, pkt]

            print("Sending packet on port%d" % (self.dev_port1))
            send_packet(self, self.dev_port1, pkt)

            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, flood_port_list)
            print("\tPacket forwarded to port%d and mirrored to port%d. OK"
                  % (self.dev_port3, self.dev_port0))

        finally:
            sai_thrift_flush_fdb_entries(
                self.client,
                entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
            sai_thrift_remove_fdb_entry(self.client, self.fdb_entry)
            sai_thrift_set_port_attribute(self.client, self.portx,
                                          egress_mirror_session=empty_obj_list)
            sai_thrift_set_port_attribute(
                self.client, self.port3,
                egress_mirror_session=empty_obj_list)
            sai_thrift_remove_mirror_session(self.client, mirror_id)
            sai_thrift_remove_port(self.client, self.portx)

    def tearDown(self):
        print("PortEgressMirrorSessionTest tearDown")
        super(PortEgressMirrorSessionTest, self).tearDown()


@group("draft")
class PortIngressMirrorSessionTest(PortFloodClass):
    ''' Test port PortIngressMirrorSessionTest attributes '''

    def setUp(self):
        self.portx = None
        self.fdb_entry = None
        super(PortIngressMirrorSessionTest, self).setUp()

    def runTest(self):
        try:
            empty_obj_list = sai_thrift_object_list_t(count=0, idlist=[])
            # Create ingress_mirror_session
            mirror_id = sai_thrift_create_mirror_session(
                self.client,
                type=SAI_MIRROR_SESSION_TYPE_LOCAL,
                monitor_port=self.port0)
            self.assertTrue(mirror_id != 0, "Failed to create port mirror_id")

            ingress_mirror_obj_list = [mirror_id]
            ingress_mirror_session = sai_thrift_object_list_t(
                count=len(ingress_mirror_obj_list),
                idlist=ingress_mirror_obj_list)

            # create/get
            sai_list = sai_thrift_u32_list_t(count=1, uint32list=[34])
            self.portx = sai_thrift_create_port(
                self.client,
                hw_lane_list=sai_list,
                speed=TEST_DEFAULT_SPEED,
                ingress_mirror_session=ingress_mirror_session)
            self.assertTrue(self.portx != 0, "Failed to create port")

            sai_list = sai_thrift_object_list_t(count=20, idlist=[])
            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, ingress_mirror_session=sai_list)

            self.assertEqual(attr['ingress_mirror_session'].count, 1)
            self.assertEqual(attr['ingress_mirror_session'].count,
                             len(ingress_mirror_obj_list))
            self.assertEqual(attr['ingress_mirror_session'].idlist[0],
                             mirror_id)

            # Clear ingress_mirror_session
            status = sai_thrift_set_port_attribute(
                self.client, self.portx,
                ingress_mirror_session=empty_obj_list)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to clear port ingress_mirror_session")
            attr = sai_thrift_get_port_attribute(
                self.client, self.portx, ingress_mirror_session=sai_list)
            self.assertEqual(attr['ingress_mirror_session'].count, 0)

            # set/get use port1
            status = sai_thrift_set_port_attribute(
                self.client, self.port1,
                ingress_mirror_session=ingress_mirror_session)
            self.assertEqual(status, SAI_STATUS_SUCCESS,
                             "Failed to set port ingress_mirror_session")
            sai_list = sai_thrift_object_list_t(count=20, idlist=[])
            attr = sai_thrift_get_port_attribute(
                self.client, self.port1, ingress_mirror_session=sai_list)
            self.assertEqual(attr['ingress_mirror_session'].count,
                             len(ingress_mirror_obj_list))
            self.assertEqual(attr['ingress_mirror_session'].idlist[0],
                             mirror_id)

            # Check traffic
            self.fdb_entry = sai_thrift_fdb_entry_t(
                switch_id=self.switch_id,
                mac_address='00:00:00:00:00:33',
                bv_id=self.default_vlan_id)
            self.assertTrue(self.fdb_entry != 0, "Failed to create fdb_entry")
            status = sai_thrift_create_fdb_entry(
                self.client,
                self.fdb_entry,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=self.port2_bp)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            pkt = simple_tcp_packet(
                eth_dst='00:00:00:00:00:33',
                eth_src='00:22:22:22:22:22',
                ip_ttl=64)

            pkt_ingress_mirror = simple_tcp_packet(
                eth_dst='00:00:00:00:00:33',
                eth_src='00:22:22:22:22:22',
                ip_ttl=64)

            flood_port_list = [[self.dev_port0],
                               [self.dev_port2]]
            flood_pkt_list = [pkt_ingress_mirror, pkt]

            print("Sending packet on port%d" % (self.dev_port1))
            send_packet(self, self.dev_port1, pkt)

            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, flood_port_list)
            print("\tPacket forwarded to port%d and mirrored to port%d. OK"
                  % (self.dev_port2, self.dev_port0))

        finally:
            sai_thrift_flush_fdb_entries(
                self.client,
                entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
            sai_thrift_set_port_attribute(
                self.client, self.portx,
                ingress_mirror_session=empty_obj_list)
            sai_thrift_set_port_attribute(
                self.client, self.port1,
                ingress_mirror_session=empty_obj_list)
            sai_thrift_remove_mirror_session(self.client, mirror_id)
            sai_thrift_remove_port(self.client, self.portx)

    def tearDown(self):
        print("PortIngressMirrorSessionTest tearDown")
        super(PortIngressMirrorSessionTest, self).tearDown()


# PORT ingress_acl tests
@group("draft")
class SinglePortIngressAclTableBindingTest(PortAclBindingClass):
    ''' Test single port ingress acl table binding '''

    def runTest(self):
        self.portIngressAclBindingSinglePortTest()


@group("draft")
class SinglePortIngressAclGroupBindingTest(PortAclBindingClass):
    ''' Test single port ingress acl group binding '''

    def runTest(self):
        self.portIngressAclBindingSinglePortTest(use_acl_group=True)


@group("draft")
class PortIngressAclTableBindingTest(PortAclBindingClass):
    ''' Test port ingress acl table binding '''

    def runTest(self):
        self.portIngressAclBindingTest()


@group("draft")
class PortIngressAclTableAddRemoveBindingTest(PortAclBindingClass):
    ''' Test port ingress acl table add remove binding '''

    def runTest(self):
        self.portIngressAclBindingTest(add_remove_bind=True)


@group("draft")
class PortIngressAclGroupBindingTest(PortAclBindingClass):
    ''' Test port ingress acl group binding '''

    def runTest(self):
        self.portIngressAclBindingTest(use_acl_group=True)


@group("draft")
class PortIngressAclGroupAddRemoveBindingTest(PortAclBindingClass):
    ''' Test port ingress acl group add remove binding '''

    def runTest(self):
        self.portIngressAclBindingTest(add_remove_bind=True,
                                       use_acl_group=True)


# PORT egress_acl tests
@group("draft")
class SinglePortEgressAclTableBindingTest(PortAclBindingClass):
    ''' Test single port egress acl table binding '''

    def runTest(self):
        self.portEgressAclBindingSinglePortTest()


@group("draft")
class SinglePortEgressAclGroupBindingTest(PortAclBindingClass):
    ''' Test single port egress acl group binding '''

    def runTest(self):
        self.portEgressAclBindingSinglePortTest(use_acl_group=True)


@group("draft")
class PortEgressAclTableBindingTest(PortAclBindingClass):
    ''' Test port egress acl table binding '''

    def runTest(self):
        self.portEgressAclBindingTest()


@group("draft")
class PortEgressAclTableAddRemoveBindingTest(PortAclBindingClass):
    ''' Test port egress acl table add remove binding '''

    def runTest(self):
        self.portEgressAclBindingTest(add_remove_bind=True)


@group("draft")
class PortEgressAclGroupBindingTest(PortAclBindingClass):
    ''' Test port egress acl group binding '''

    def runTest(self):
        self.portEgressAclBindingTest(use_acl_group=True)


@group("draft")
class PortEgressAclGroupAddRemoveBindingTest(PortAclBindingClass):
    ''' Test port egress acl group add remove binding '''

    def runTest(self):
        self.portEgressAclBindingTest(add_remove_bind=True, use_acl_group=True)
