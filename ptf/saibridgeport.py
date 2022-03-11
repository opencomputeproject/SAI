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
"""
Thrift SAI interface Bridge Port tests
"""

from sai_thrift.sai_headers import *

from sai_base_test import *


@group("draft")
class BridgePortAttributeTest(SaiHelper):
    '''
    Verify bridge port attributes getting and setting
    '''

    def setUp(self):
        super(BridgePortAttributeTest, self).setUp()

        self.port24_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True,
            fdb_learning_mode=SAI_BRIDGE_PORT_FDB_LEARNING_MODE_HW)
        self.assertNotEqual(self.port24_bp, 0)

    def runTest(self):
        # bridge_id
        # get
        attr = sai_thrift_get_bridge_port_attribute(
            self.client, self.port24_bp, bridge_id=True)
        self.assertEqual(attr['bridge_id'], self.default_1q_bridge)

        # type
        # get
        attr = sai_thrift_get_bridge_port_attribute(
            self.client, self.port24_bp, type=True)
        self.assertEqual(attr['type'], SAI_BRIDGE_PORT_TYPE_PORT)

        # port_id
        # get
        attr = sai_thrift_get_bridge_port_attribute(
            self.client, self.port24_bp, port_id=True)
        self.assertEqual(attr['port_id'], self.port24)

        # fdb_learning_mode
        # get
        attr = sai_thrift_get_bridge_port_attribute(
            self.client, self.port24_bp, fdb_learning_mode=True)
        self.assertEqual(attr['fdb_learning_mode'],
                         SAI_BRIDGE_PORT_FDB_LEARNING_MODE_HW)
        # set
        status = sai_thrift_set_bridge_port_attribute(
            self.client,
            self.port24_bp,
            fdb_learning_mode=SAI_BRIDGE_PORT_FDB_LEARNING_MODE_DISABLE)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_bridge_port_attribute(
            self.client, self.port24_bp, fdb_learning_mode=True)
        self.assertEqual(attr['fdb_learning_mode'],
                         SAI_BRIDGE_PORT_FDB_LEARNING_MODE_DISABLE)

        # admin_state
        # get
        attr = sai_thrift_get_bridge_port_attribute(
            self.client, self.port24_bp, admin_state=True)
        self.assertEqual(attr['admin_state'], True)
        # set
        status = sai_thrift_set_bridge_port_attribute(
            self.client, self.port24_bp, admin_state=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_bridge_port_attribute(
            self.client, self.port24_bp, admin_state=True)
        self.assertEqual(attr['admin_state'], False)

    def tearDown(self):
        sai_thrift_remove_bridge_port(self.client, self.port24_bp)

        super(BridgePortAttributeTest, self).tearDown()


@group("draft")
class BridgePortCreationTest(SaiHelper):
    '''
    Verify bridge port creation
    '''

    def runTest(self):
        self.noBpDropTest()
        self.bpTypePortCreationTest()

    def noBpDropTest(self):
        '''
        This verifies if packets are dropped on port when no bridge port is
        created
        '''
        print("\nnoBpDropTest")

        no_bp_port = self.dev_port24

        src_mac = "00:11:11:11:11:11"
        dst_mac = "00:22:22:22:22:22"

        pkt = simple_udp_packet(eth_src=src_mac, eth_dst=dst_mac, pktlen=100)

        print("Sending packet on port with no bridge port created")
        send_packet(self, no_bp_port, pkt)
        verify_no_other_packets(self)
        print("\tPacket dropped. OK")

    def bpTypePortCreationTest(self):
        '''
        This verifies creation of bridge port of type port
        Bridge must be 1Q bridge
        '''
        print("\nbpTypePortCreationTest")

        try:
            print("Verifying bp type Port creation for physical port")
            port24_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=self.port24,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            self.assertNotEqual(port24_bp, 0)
            print("\tOK")

            print("Verifying bp type Port creation for LAG logical port")
            test_lag = sai_thrift_create_lag(self.client)
            self.assertNotEqual(test_lag, 0)
            test_lag_member1 = sai_thrift_create_lag_member(
                self.client, lag_id=test_lag, port_id=self.port25)
            self.assertNotEqual(test_lag_member1, 0)
            test_lag_member2 = sai_thrift_create_lag_member(
                self.client, lag_id=test_lag, port_id=self.port26)
            self.assertNotEqual(test_lag_member2, 0)

            lag_bp = sai_thrift_create_bridge_port(
                port_id=test_lag,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            self.assertNotEqual(lag_bp, 0)
            print("\tOK")

        finally:
            sai_thrift_remove_bridge_port(self.client, lag_bp)
            sai_thrift_remove_lag_member(self.client, test_lag_member1)
            sai_thrift_remove_lag_member(self.client, test_lag_member2)
            sai_thrift_remove_lag(self.client, test_lag)
            sai_thrift_remove_bridge_port(self.client, port24_bp)


@group("draft")
class BridgePortStateTest(SaiHelper):
    '''
    Verify switch behavior in particular bridge port state cases
    '''

    def setUp(self):
        super(BridgePortStateTest, self).setUp()

        self.vlan_id = 10
        self.src_mac = "00:11:11:11:11:11"
        self.dst_mac = "00:22:22:22:22:22"

        self.pkt = simple_udp_packet(eth_dst=self.dst_mac,
                                     eth_src=self.src_mac,
                                     pktlen=100)
        self.tag_pkt = simple_udp_packet(eth_dst=self.dst_mac,
                                         eth_src=self.src_mac,
                                         dl_vlan_enable=True,
                                         vlan_vid=self.vlan_id,
                                         pktlen=104)

        self.chck_pkt = simple_udp_packet(eth_dst=self.src_mac,
                                          eth_src=self.dst_mac,
                                          pktlen=100)
        self.chck_tag_pkt = simple_udp_packet(eth_dst=self.src_mac,
                                              eth_src=self.dst_mac,
                                              dl_vlan_enable=True,
                                              vlan_vid=self.vlan_id,
                                              pktlen=104)

    def runTest(self):
        self.bpStateDownFlushTest()
        self.bpStateDownNoLearnTest()

    def bpStateDownFlushTest(self):
        '''
        This verifies if FDB entries related to MAC addresses learned on port
        corresponding to bridge port are flushed while bridge port admin state
        is setting to false
        '''
        print("\nbpStateDownFlushTest")

        test_dev_port = self.dev_port0
        chck_dev_port = self.dev_port1
        test_bp = self.port0_bp

        # other VLAN 10 ports
        flood_port_list = [[chck_dev_port],
                           [self.dev_port4, self.dev_port5, self.dev_port6]]
        chck_flood_port_list = [[test_dev_port],
                                [self.dev_port4, self.dev_port5,
                                 self.dev_port6]]

        fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                           mac_address=self.src_mac,
                                           bv_id=self.vlan10)

        flood_pkt_list = [self.tag_pkt, self.pkt]
        chck_flood_pkt_list = [self.chck_pkt, self.chck_pkt]

        try:
            print("Sending packet on port %d to learn MAC address - "
                  "will flood" % test_dev_port)
            send_packet(self, test_dev_port, self.pkt)
            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, flood_port_list)
            time.sleep(2)

            print("Checking if MAC address was learned")
            status = sai_thrift_set_fdb_entry_attribute(
                self.client,
                fdb_entry,
                packet_action=SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("\tOK")

            print("Sending packet on port %d, %s -> %s - forwarding" %
                  (chck_dev_port, self.dst_mac, self.src_mac))
            send_packet(self, chck_dev_port, self.chck_tag_pkt)
            verify_packets(self, self.chck_pkt, [test_dev_port])
            print("\tOK")

            print("Setting bridge port state to down")
            status = sai_thrift_set_bridge_port_attribute(
                self.client, test_bp, admin_state=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            print("Checking if FDB entry was removed")
            status = sai_thrift_set_fdb_entry_attribute(
                self.client,
                fdb_entry,
                packet_action=SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(status, SAI_STATUS_ITEM_NOT_FOUND)

            print("Sending packet on port %d, %s -> %s - will flood" %
                  (chck_dev_port, self.dst_mac, self.src_mac))
            send_packet(self, chck_dev_port, self.chck_tag_pkt)
            verify_each_packet_on_multiple_port_lists(
                self, chck_flood_pkt_list, chck_flood_port_list)
            print("\tOK")

            print("\tVerification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
            sai_thrift_set_bridge_port_attribute(
                self.client, test_bp, admin_state=True)

    def bpStateDownNoLearnTest(self):
        '''
        This checks if MAC addresses are not learned on port when bridge port
        state is down
        We set the down state on port0 and send a packet form port0 with given
        SMAC: 00:11:11:11:11:11, and then we verify it was broadcasted on other
        ports in VLAN.
        Next we send a packet from port1 with DMAC 00:11:11:11:11:11
        and we verify it was also broadcasted on other ports in VLAN
        '''
        print("\nbpStateDownNoLearnTest()")

        test_dev_port = self.dev_port0
        chck_dev_port = self.dev_port1
        test_bp = self.port0_bp
        new_bp_port = self.port24
        new_bp_dev_port = self.dev_port24

        try:
            # set port0_bp state to DOWN
            status = sai_thrift_set_bridge_port_attribute(
                self.client, test_bp, admin_state=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("Port %d state set to DOWN" % test_dev_port)

            flood_pkt_list = [self.tag_pkt, self.pkt]
            flood_port_list = [[chck_dev_port],
                               [self.dev_port4, self.dev_port5,
                                self.dev_port6]]
            print("Sending packet on port %d, %s -> %s - will flood" %
                  (test_dev_port, self.src_mac, self.dst_mac))
            send_packet(self, test_dev_port, self.pkt)
            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, flood_port_list)
            print("\tOK")

            chck_flood_pkt_list = [self.chck_pkt, self.chck_pkt]
            chck_flood_port_list = [[test_dev_port],
                                    [self.dev_port4, self.dev_port5,
                                     self.dev_port6]]
            print("Sending packet on port %d with DMAC %s - "
                  "MAC not learned, will flood" %
                  (chck_dev_port, self.src_mac))
            send_packet(self, chck_dev_port, self.chck_tag_pkt)
            verify_each_packet_on_multiple_port_lists(
                self, chck_flood_pkt_list, chck_flood_port_list)
            print("\tOK")

            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            # create new bridge port with admin state down
            new_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=new_bp_port,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=False)
            self.assertNotEqual(new_bp, 0)
            new_vlan_member = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan10,
                bridge_port_id=new_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            self.assertNotEqual(new_vlan_member, 0)
            sai_thrift_set_port_attribute(
                self.client, new_bp_port, port_vlan_id=self.vlan_id)

            flood_pkt_list = [self.pkt, self.tag_pkt, self.pkt]
            flood_port_list = [[test_dev_port], [chck_dev_port],
                               [self.dev_port4, self.dev_port5,
                                self.dev_port6]]
            print("Sending packet on newly created bp port %d, %s -> %s - "
                  "will flood" % (new_bp_dev_port, self.src_mac, self.dst_mac))
            send_packet(self, new_bp_dev_port, self.pkt)
            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, flood_port_list)
            print("\tOK")

            chck_flood_pkt_list = [self.chck_pkt, self.chck_pkt, self.chck_pkt]
            chck_flood_port_list = [[new_bp_dev_port], [test_dev_port],
                                    [self.dev_port4, self.dev_port5,
                                     self.dev_port6]]
            print("Sending packet on port %d with DMAC %s - "
                  "MAC not learned, will flood" %
                  (chck_dev_port, self.src_mac))
            send_packet(self, chck_dev_port, self.chck_tag_pkt)
            verify_each_packet_on_multiple_port_lists(
                self, chck_flood_pkt_list, chck_flood_port_list)
            print("\tOK")

            print("Verification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
            sai_thrift_set_port_attribute(
                self.client, new_bp_port, port_vlan_id=0)
            sai_thrift_remove_vlan_member(self.client, new_vlan_member)
            sai_thrift_remove_bridge_port(self.client, new_bp)

            # restore initial port0 admin state
            sai_thrift_set_bridge_port_attribute(
                self.client, test_bp, admin_state=True)


@group("draft")
class BridgePortNoFloodTest(SaiHelperBase):
    '''
    Verify bridge port no flood test case
    '''

    def setUp(self):
        super(BridgePortNoFloodTest, self).setUp()

        self.test_port_list = [self.port0, self.port1, self.port2, self.port3]
        self.dev_test_port_list = [
            self.dev_port0,
            self.dev_port1,
            self.dev_port2,
            self.dev_port3]
        vlan_id = 1
        for port in self.test_port_list:
            sai_thrift_set_port_attribute(
                self.client, port, port_vlan_id=vlan_id)
            attr = sai_thrift_get_port_attribute(
                self.client, port, port_vlan_id=True)
            self.assertEqual(attr['SAI_PORT_ATTR_PORT_VLAN_ID'], vlan_id)

    def runTest(self):
        print("SAIBridgePortNoFloodTest")

        src_mac = "00:11:11:11:11:11"
        dst_mac = "00:22:22:22:22:22"

        pkt = simple_udp_packet(eth_dst=dst_mac,
                                eth_src=src_mac,
                                pktlen=100)
        print("Sending packet on port with no bridge port created")
        send_packet(self, self.dev_port1, pkt)
        verify_no_other_packets(self)
        print("\tPacket dropped. OK")

    def tearDown(self):
        # revert original port's VLAN id
        vlan_id = 10
        for port in self.test_port_list:
            sai_thrift_set_port_attribute(
                self.client, port, port_vlan_id=vlan_id)
            attr = sai_thrift_get_port_attribute(
                self.client, port, port_vlan_id=True)
            self.assertTrue(attr['SAI_PORT_ATTR_PORT_VLAN_ID'], vlan_id)

        super(BridgePortNoFloodTest, self).tearDown()
