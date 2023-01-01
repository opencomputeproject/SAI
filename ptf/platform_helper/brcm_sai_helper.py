# Copyright (c) 2021 Microsoft Open Technologies, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing
#    permissions and limitations under the License.
#
#    Microsoft would like to thank the following companies for their review and
#    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
#    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.

"""
This file contains class for brcm specified functions.
"""
from collections import OrderedDict

from typing import List, Dict
from typing import TYPE_CHECKING

from platform_helper.common_sai_helper import * # pylint: disable=wildcard-import; lgtm[py/polluting-import]

CATCH_EXCEPTIONS=True
# SAI_STATUS_NOT_IMPLEMENTED
ACCEPTED_ERROR_CODE = [SAI_STATUS_NOT_IMPLEMENTED]
#SAI_STATUS_ATTR_NOT_IMPLEMENTED
ACCEPTED_ERROR_CODE += range(SAI_STATUS_ATTR_NOT_IMPLEMENTED_MAX, SAI_STATUS_ATTR_NOT_IMPLEMENTED_0)
#SAI_STATUS_ATTR_NOT_IMPLEMENTED
ACCEPTED_ERROR_CODE += range(SAI_STATUS_UNKNOWN_ATTRIBUTE_MAX, SAI_STATUS_UNKNOWN_ATTRIBUTE_0)
#SAI_STATUS_ATTR_NOT_SUPPORTED
ACCEPTED_ERROR_CODE += range(SAI_STATUS_ATTR_NOT_SUPPORTED_MAX, SAI_STATUS_ATTR_NOT_SUPPORTED_0)


class BrcmSaiHelper(CommonSaiHelper):
    """
    This class contains broadcom(brcm) specified functions for the platform setup and test context configuration.
    """
    platform = 'brcm'

    def set_accepted_exception(self):
        """
        Set accepted exceptions.
        """
        adapter.CATCH_EXCEPTIONS=CATCH_EXCEPTIONS
        adapter.EXPECTED_ERROR_CODE += ACCEPTED_ERROR_CODE


    def __init__(self, *args, **kwargs) -> None:
        """
        Init the Port configer.

        Args:
            test_obj: the test object
        """
        super().__init__(*args, **kwargs)
        config_driver = ConfigDBOpertion()
        self.port_config = config_driver.get_port_config()

        # port
        self.default_trap_group = None
        """
        Local device port index list, 0, 1, ...
        """
        self.host_intf_table_id = None
        self.port_obj_list: List['Port'] = []
        """
        Port object list
        """
        self.port_id_list = []
        """
        port id list, use to present all the port ids
        """
        self.hostif_list = None
        """
        Host interface list
        """
        self.default_bridge_port_list = []
        """
        Default bridge port list
        """
        self.host_if_port_idx_map = []
        """
        list in order of the host interface create sequence, and with the value for port index
        """
        self.host_if_name_list = []
        """
        List of the interface name
        """


    def remove_switch(self):
        '''
        Method to remove the switch.
        '''
        print("BrcmSaiHelperBase::remove_switch does not support. Cannot recreate after remove.")   


    def sai_thrift_create_fdb_entry_allow_mac_move(self,
                                client,
                                fdb_entry,
                                type=None,
                                packet_action=None,
                                user_trap_id=None,
                                bridge_port_id=None,
                                meta_data=None,
                                endpoint_ip=None,
                                counter_id=None,
                                allow_mac_move=None):
        """
        Override the sai_thrift_create_fdb_entry when check the functionality related to allow_mac_move.

        This method will set the allow_mac_move to True.
        """
        #TODO confirm the SPEC. Related to RFC9014 and RFC7432
        #Context: when set SAI_FDB_ENTRY_TYPE_STATIC, allow_mac_move will be checked, and its default value is false
        #         then, when a port get different mac (i.e. arp ack from arp req, port used in previous arp req, also means other ports with different session).
        #         the packet should be dropped.
        #         but some other ASIC can transfer the packet to other ports not used to transfer the packet.

        print("BrcmSaiHelper::sai_thrift_create_fdb_entry_allow_mac_move")
        sai_thrift_create_fdb_entry(
            client=client,
            fdb_entry=fdb_entry,
            type=type,
            packet_action=packet_action,
            user_trap_id=user_trap_id,
            bridge_port_id=bridge_port_id,
            meta_data=meta_data,
            endpoint_ip=endpoint_ip,
            counter_id=counter_id,
            allow_mac_move=True)


    def start_switch(self):
        """
        Start switch and wait seconds for a warm up.
        """
        switch_init_wait = 5

        self.switch_id = sai_thrift_create_switch(
            self.client, init_switch=True, src_mac_address=ROUTER_MAC)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        print("Waiting for switch to get ready, {} seconds ...".format(switch_init_wait))
        time.sleep(switch_init_wait)


    def check_cpu_port_hdl(self):
        """
        Checks cpu port handler, expect the cpu_port_hdl equals to qos_queue port id

        Needs the following class attributes:

            self.cpu_port_hdl - cpu_port_hdl id

        Sets the following class attributes:

            self.cpu_port - cpu_port id

        """
        attr = sai_thrift_get_port_attribute(self.client,
                                             self.cpu_port_hdl,
                                             qos_number_of_queues=True)
        num_queues = attr['qos_number_of_queues']
        q_list = sai_thrift_object_list_t(count=num_queues)
        attr = sai_thrift_get_port_attribute(self.client,
                                             self.cpu_port_hdl,
                                             qos_queue_list=q_list)

        for queue in range(0, num_queues):
            queue_id = attr['qos_queue_list'].idlist[queue]
            setattr(self, 'cpu_queue%s' % queue, queue_id)
            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id,
                port=True,
                index=True,
                parent_scheduler_node=True)
            self.assertEqual(queue, q_attr['index'])
            # in broadcom platform, the q_attr["port"] is not equals to cpu_port_hdl
            # cpu_port_hdl is ahead of the cpu_port list
            self.cpu_port = q_attr["port"]



    # Port setup method below

    def config_port(self):
        self.port_list = self.get_port_list()
        self.get_local_mapped_ports()
        self.parse_port_config(self.test_params['port_config_ini'])
        self.set_port_attr(self.port_list)

        attr = sai_thrift_get_switch_attribute(
            self.client, default_trap_group=True)
        default_trap_group = attr['default_trap_group']
        self.turn_on_port_admin_state_by_port_list(self.port_obj_list)
        self.turn_up_and_check_ports_by_port_list(self.port_obj_list)


        if 'port_config_ini' in self.test_params:
            host_intf_table_id, hostif_list = self.create_port_hostif_by_port_config_ini(
                port_list=self.port_obj_list, trap_group=default_trap_group)
        else:
            host_intf_table_id, hostif_list = self.create_host_intf(
                port_list=self.port_obj_list, trap_group=default_trap_group)
        self.host_intf_table_id = host_intf_table_id
        self.hostif_list = hostif_list

        self.default_1q_bridge = self.get_default_1q_bridge()
        self.reset_1q_bridge_ports()


    #Override methods

    def recreate_ports(self):
        """
        Method to recreate the port.
        """

        #port recreate not support, error happened.
        #Port needs to be init and setup at same time.
        #Make the process happened in turn_up_and_check_ports
        print("BrcmSaiHelperBase::recreate_ports does not support. Just Parse Port Config")
        if 'port_config_ini' in self.test_params:
            self.ports_config = self.parsePortConfig(self.test_params['port_config_ini']) 


    def get_bridge_port_all_attribute(self, bridge_port_id):
        '''
        Gets all the attrbute from bridge port.

        Args:
            bridge_port_id: bridge port object id

        Returns:
            dict: bridge attributes

        '''
        print("BrcmSaiHelperBase::get_bridge_port_all_attribute")

        # Cannot get those three attributes from sai_thrift_get_bridge_port_attribute
        # ingress_filtering=True,
        # egress_filtering=True,
        # isolation_group=True
        sai_thrift_get_bridge_port_attribute(
            self.client,  bridge_port_oid=bridge_port_id, ingress_filtering=True,  egress_filtering=True)
        attr = sai_thrift_get_bridge_port_attribute(
            self.client,
            bridge_port_oid=bridge_port_id,
            type=True,
            port_id=True,
            tagging_mode=True,
            vlan_id=True,
            rif_id=True,
            tunnel_id=True,
            bridge_id=True,
            fdb_learning_mode=True,
            max_learned_addresses=True,
            fdb_learning_limit_violation_packet_action=True,
            admin_state=True
            # Cannot get those three
            # ingress_filtering=True,
            # egress_filtering=True,
            # isolation_group=True
        )
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        return attr


    def load_default_1q_bridge_ports(self):
        """
        Loads default 1q bridge ports and set as class attribute.

        Needs the following class attributes:
            self.default_1q_bridge - default_1q_bridge oid

            self.active_ports_no - number of active ports

            self.portX objects for all active ports

        Sets the following class attributes:

            self.default_1q_bridge_port_list - list of all 1q bridge port objects

            self.portX_bp - objects for all 1q bridge ports
        """
        print("BrcmSaiHelperBase::load_default_1q_bridge_ports")
        attr = sai_thrift_get_bridge_attribute(
                    self.client, 
                    bridge_oid=self.default_1q_bridge,
                    port_list=sai_thrift_object_list_t(
                        idlist=[], count=self.active_ports_no))
        default_1q_bridge_port_list = attr['port_list'].idlist
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        #try to binding the bridge port with the port index here
        for bp in default_1q_bridge_port_list:
            attr = self.get_bridge_port_all_attribute(bp)
            for index in range(0, len(self.port_list)):
                port_id = getattr(self, 'port%s' % index)
                if port_id == attr['port_id']:
                    setattr(self, 'port%s_bp' % index, bp)
                    break
        return default_1q_bridge_port_list


    def reset_1q_bridge_ports(self):
        '''
        Reset all the 1Q bridge ports.
        Needs the following class attributes:
            self.default_1q_bridge - default_1q_bridge oid

            self.active_ports_no - number of active ports

            self.portX objects for all active ports
        '''
        print("BrcmSaiHelperBase::reset_1q_bridge_ports")
        #In case the bridge port will be initalized by default, clear them
        default_1q_bridge_port_list = self.load_default_1q_bridge_ports()
        self.remove_1q_bridge_port(default_1q_bridge_port_list)


    def remove_1q_bridge_port(self, default_1q_bridge_port_list):
        '''
        Removes all the bridge ports.
        '''
        print("BrcmSaiHelperBase::remove_1q_bridge_port")
        for index in range(0, len(default_1q_bridge_port_list)):
            port_bp = getattr(self, 'port%s_bp' % index)
            sai_thrift_remove_bridge_port(self.client, port_bp)
            delattr(self, 'port%s_bp' % index)

    # Local methods


    def set_port_attr(self, port_list):
        for index, oid in enumerate(port_list):
            setattr(self, 'port%s' % index, oid)


    def create_bridge_ports_by_bridge_and_ports(self, bridge_id, port_list: List['Port']):
        """
        Create bridge ports base on port_list.

        Args:
            bridge_id: bridge object id
            port_list: port list oid

        Returns:
            list: bridge port list
        """
        print("Create bridge ports...")
        bp_list = []
        for index, item in enumerate(port_list):

            port_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=bridge_id,
                port_id=item.oid,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            bp_list.append(port_bp)
            item.bridge_port_oid = port_bp
        return bp_list


    def create_host_intf(self, port_list: List['Port'], trap_group=None):
        """
        Create host interface.

        Steps:
         1. create host table entry
         2. create host interface trap
         3. set host interface base on the port_config.int (this file contains the lanes, name and index information.)

        Args:
            ports_config: port configs, which is got from the local config.
            trap_group: host interface trap group(optional)
            port_list: port list 

        Returns:
            host_intf_table_id
            hostif_list
        """
        print("Create Host intfs...")
        host_intf_table_id = sai_thrift_create_hostif_table_entry(
            self.client, type=SAI_HOSTIF_TABLE_ENTRY_TYPE_WILDCARD,
            channel_type=SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_PHYSICAL_PORT)
        sai_thrift_create_hostif_trap(
            self.client, trap_type=SAI_HOSTIF_TRAP_TYPE_TTL_ERROR, packet_action=SAI_PACKET_ACTION_TRAP,
            trap_group=trap_group, trap_priority=0)
        hostif_list = [None]*len(port_list)
        for index, item in enumerate(port_list):
            try:
                hostif = sai_thrift_create_hostif(
                    self.client,
                    type=SAI_HOSTIF_TYPE_NETDEV,
                    obj_id=item.oid,
                    name=item.port_config.name)
                sai_thrift_set_hostif_attribute(
                    self.client, hostif_oid=hostif, oper_status=False)
                hostif_list[index] = hostif
                item.host_itf_idx = hostif
            except BaseException as e:
                print("Cannot create hostif, error : {}".format(e))
        return host_intf_table_id, hostif_list


    def create_port_hostif_by_port_config_ini(self, port_list: List['Port'], trap_group=None):
        lane_orderd_dict = sorted(
            self.ports_config.items(), key=lambda kv: kv[1]['lanes'])
        index_orderd_dict = sorted(
            self.ports_config.items(), key=lambda kv: kv[1]['index'])

        min = int(index_orderd_dict[0][1]['index'])
        for item in lane_orderd_dict:
            self.host_if_port_idx_map.append(
                int(item[1]['index'])-min)
        for index, key in enumerate(self.ports_config):
            self.host_if_name_list.append(key)
        print("Create Host intfs by port_config_ini ...")
        host_intf_table_id = sai_thrift_create_hostif_table_entry(
            self.client, type=SAI_HOSTIF_TABLE_ENTRY_TYPE_WILDCARD,
            channel_type=SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_PHYSICAL_PORT)
        sai_thrift_create_hostif_trap(
            self.client, trap_type=SAI_HOSTIF_TRAP_TYPE_TTL_ERROR, packet_action=SAI_PACKET_ACTION_TRAP,
            trap_group=trap_group, trap_priority=0)
        hostif_list = [None]*len(port_list)
        min = int(index_orderd_dict[0][1]['index'])
        for index, item in enumerate(lane_orderd_dict):
            port_index = self.host_if_port_idx_map[index]
            port = self.port_obj_list[port_index]
            name = item[0]
            try:
                hostif = sai_thrift_create_hostif(
                    self.client,
                    type=SAI_HOSTIF_TYPE_NETDEV,
                    obj_id=port.oid,
                    name=name)
                sai_thrift_set_hostif_attribute(
                    self.client, hostif_oid=hostif, oper_status=False)
                hostif_list[index] = hostif
                port.host_itf_idx = index
            except BaseException as e:
                print("Cannot create hostif, error : {}".format(e))
        return host_intf_table_id, hostif_list


    def get_bridge_port_list(self, bridge_id):
        """
        Get bridge ports.

        Args:
            bridge_id: bridge id.

        Returns:
            list: bridge_port_list

        """
        print("Get bridge ports...")
        bridge_port_list = sai_thrift_object_list_t(count=100)
        bp_list = sai_thrift_get_bridge_attribute(
            self.client, bridge_id, port_list=bridge_port_list)
        for index, item in enumerate(bp_list['port_list'].idlist):
            self.port_obj_list[index].bridge_port_oid = item
            pid = sai_thrift_get_bridge_port_attribute(
                self.client, bridge_port_oid=item, port_id=True)
            print("Get bridge port {} wiht port id".format(item, pid))
            # debug msg
            # print("create bridge for port {} portidx {} dev_index {} eth {} bridge {}".format(
            #             index, port.port_index, port.dev_port_index, port.dev_port_eth, port.bridge_port_oid))
        return bp_list['port_list'].idlist


    def get_default_1q_bridge(self):
        """
        Get defaule 1Q bridge.

        Returns:
            default_1q_bridge_id
        """
        print("Get default 1Q bridge...")
        def_attr = sai_thrift_get_switch_attribute(
            self.client, default_1q_bridge_id=True)
        return def_attr['default_1q_bridge_id']


    def get_local_mapped_ports(self):
        """
        Get device port numbers from ptf config.
        Those port number should map to the remote DUT port base on the configuration file.

        Following the sequence of the parameters: --interface '0-16@eth0' --interface '0-18@eth1'
        item[0]=16, item[1]=18

        Returns:
            list: port numbers
        """
        dev_port_list = []
        for index, item in enumerate(config['interfaces']):
            device, port, eth = item
            self.port_obj_list[index].dev_port_index = port
            self.port_obj_list[index].dev_port_eth = eth
            dev_port_list.append(port)
        return dev_port_list


    def get_port_list(self):
        """
        Set the class variable port_list.

        Returns:
            port_list
        """
        port_obj_list: List[Port] = []
        port_id_list = []
        port_list = sai_thrift_object_list_t(count=100)
        p_list = sai_thrift_get_switch_attribute(
            self.client, port_list=port_list)
        for index, item in enumerate(p_list['port_list'].idlist):
            port: Port = Port(oid=item, port_index=index)
            temp_list = sai_thrift_object_list_t(count=100)
            attr = sai_thrift_get_port_attribute(
                self.client, port_oid=port.oid, hw_lane_list=temp_list)
            port.default_lane_list = attr['hw_lane_list'].uint32list
            port_obj_list.append(port)
        self.port_obj_list = self.sort_port_list_by_config(
            self.ports_config, port_obj_list)
        for item in self.port_obj_list:
            port_id_list.append(item.oid)
        return port_id_list


    def sort_port_list_by_config(self, ports_config, port_list: List[Port]):
        """
        Sort the port list base on the port_config.ini.
        This method will match the default_lane_list in the port object with the lane defined in 
        port config for a ordered port list.

        Attrs:
            ports_config: port config, which gets from the port_config.ini
            port_list: port list
        """
        sorted_port_list: List[Port] = []
        for index, key in enumerate(ports_config):
            lane_list = ports_config[key]['lanes']
            for port in port_list:
                if lane_list == port.default_lane_list:
                    sorted_port_list.append(port)
                    break
        return sorted_port_list


    def parse_port_config(self, port_config_file):
        """
        Parse port_config.ini file

        Example of supported format for port_config.ini:
        # name        lanes       alias       index    speed    autoneg   fec
        Ethernet0       0         Ethernet0     1      25000      off     none
        Ethernet1       1         Ethernet1     1      25000      off     none
        Ethernet2       2         Ethernet2     1      25000      off     none
        Ethernet3       3         Ethernet3     1      25000      off     none
        Ethernet4       4         Ethernet4     2      25000      off     none
        Ethernet5       5         Ethernet5     2      25000      off     none
        Ethernet6       6         Ethernet6     2      25000      off     none
        Ethernet7       7         Ethernet7     2      25000      off     none
        Ethernet8       8         Ethernet8     3      25000      off     none
        Ethernet9       9         Ethernet9     3      25000      off     none
        Ethernet10      10        Ethernet10    3      25000      off     none
        Ethernet11      11        Ethernet11    3      25000      off     none
        etc

        Args:
            port_config_file (string): path to port config file

        Returns:
            dict: port configuation from file

        Raises:
            e: exit if file not found
        """
        portConfigs = OrderedDict()
        try:
            index = 0
            with open(port_config_file) as conf:
                for line in conf:
                    if line.startswith('#'):
                        if "name" in line:
                            titles = line.strip('#').split()
                        continue
                    tokens = line.split()
                    if len(tokens) < 2:
                        continue

                    name_index = titles.index('name')
                    name = tokens[name_index]
                    data = {}
                    portConfig = PortConfig()
                    for i, item in enumerate(tokens):
                        if i == name_index:
                            continue
                        data[titles[i]] = item
                    portConfig.lanes = [int(lane)
                                        for lane in data['lanes'].split(',')]
                    portConfig.speed = int(data['speed'])
                    portConfig.name = name
                    portConfigs[index] = portConfig
                    self.port_obj_list[index].port_config = portConfig
                    index = index + 1
            return portConfigs
        except Exception as e:
            raise e


    def remove_host_inf(self, host_intf_table_id, hostif_list):
        """
        Remove host interface.
         Steps:
         2. remove host interface
         1. remove host table entry
        Args:
            host_intf_table_id
            hostif_list 
        """

        for _, hostif in enumerate(hostif_list):
            sai_thrift_remove_hostif(self.client, hostif)
        sai_thrift_remove_hostif_table_entry(self.client, host_intf_table_id)


    def turn_on_port_admin_state_by_port_list(self, port_list: List['Port']):
        """
        Turn on port admin state

        Args:
            post_list: post list
        """
        print("Set port...")
        for _, port in enumerate(port_list):
            sai_thrift_set_port_attribute(
                self.client, port_oid=port.oid, mtu=self.get_mtu(), admin_state=True,
                fec_mode=self.get_fec_mode())


    def turn_up_and_check_ports_by_port_list(self, port_list: List['Port']):
        '''
        Method to turn up the ports.
        In case some device not init the port after start the switch.

        Args:
            port_list - list of all active port objects
        '''

        # For brcm devices, need to init and setup the ports at once after start the switch.
        retries = 10
        down_port_list = []
        for index, port in enumerate(port_list):
            port_attr = sai_thrift_get_port_attribute(
                self.client, port.oid, oper_status=True)
            print("Turn up port {}".format(index))
            port_up = True
            if port_attr['oper_status'] != SAI_PORT_OPER_STATUS_UP:
                port_up = False
                for num_of_tries in range(retries):
                    port_attr = sai_thrift_get_port_attribute(
                        self.client, port.oid, oper_status=True)
                    if port_attr['oper_status'] == SAI_PORT_OPER_STATUS_UP:
                        port_up = True
                        break
                    time.sleep(3)
                    print("port {} , local index {} id {} is not up, status: {}. Retry. Reset Admin State.".format(
                        index, port.port_index, port.oid, port_attr['oper_status']))
                    sai_thrift_set_port_attribute(
                        self.client,
                        port_oid=port.oid,
                        mtu=self.get_mtu(),
                        admin_state=True,
                        fec_mode=self.get_fec_mode())
            if not port_up:
                down_port_list.append(index)
        if down_port_list:
            print("Ports {} are  down after retries.".format(down_port_list))


    def get_fec_mode(self):
        '''
        get fec mode from config_db.json

        RETURN:
             int: SAI_PORT_FEC_MODE_X
        '''
        fec_mode = self.port_config.get('fec')
        fec_change = {
            None: SAI_PORT_FEC_MODE_NONE,
            'rs': SAI_PORT_FEC_MODE_RS,
            'fc': SAI_PORT_FEC_MODE_FC,
        }
        return fec_change[fec_mode]


    def get_mtu(self):
        '''
        get mtu from config_db.json

        RETURN:
            int: mtu number
        '''
        return int(self.port_config.get('mtu'))

class PortConfig(object):
    """
    Represent the PortConfig Object

    Attrs:
        name: interface name
        lanes: lanes
        speed: port speed
    """

    def __init__(self, name=None, lanes=None, speed=None):
        self.name = name
        self.lanes = lanes
        self.speed = speed
