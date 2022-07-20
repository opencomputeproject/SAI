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
#
#

from time import sleep
from unittest import skip
from sai_test_base import T0TestBase
from sai_thrift.sai_headers import *
from ptf import config
from ptf.testutils import *
from ptf.thriftutils import *
from sai_utils import *


class L2PortForwardingTest(T0TestBase):
    """
    Verify the basic fdb forwarding.
    Segment should be forwarding to the correlated port bases on the FDB table.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)

    def runTest(self):
        """
        Test fdb forwarding
        """
        try:
            print("FDB basic forwarding test.")
            for index in range(2, 9):
                print("L2 Forwarding from {} to port: {}".format(
                    self.dev_port_list[1],
                    self.dev_port_list[index]))
                pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[index],
                                        eth_src=self.local_server_mac_list[1],
                                        vlan_vid=10,
                                        ip_id=101,
                                        ip_ttl=64)

                send_packet(self, self.dev_port_list[1], pkt)
                verify_packet(self, pkt, self.dev_port_list[index])
                verify_no_other_packets(self)
        finally:
            pass

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)


"""
Skip test for broadcom, learn_disable, report error code -196608, no error log.
Item: 15000933
"""


@skip
class VlanLearnDisableTest(T0TestBase):
    """
    Verify if MAC addresses are not learned on the port when bridge port learning is disabled
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        status = sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_vlan_attribute(
            self.client, self.vlans[10].vlan_oid, learn_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_switch_attribute(
            self.client,
            fdb_unicast_miss_packet_action=SAI_PACKET_ACTION_FORWARD)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        """
        1. Flush all MAC
        2. Disable MAC learn on VLAN10
        3. Create a packet with SMAC ``MacX``
        4. send packet from port1
        5. Verify the packet flood to other VLAN10 ports
        6. Create a packet with DMAC ``MacX``
        7. send the packet on port2
        8. Verify the packet flood to other VLAN10 ports, including port1
        9. check FDB entries, no new entry
        """
        print("VlanLearnDisableTest")
        unknown_mac1 = "00:01:01:99:99:99"
        unknown_mac2 = "00:01:02:99:99:99"
        self.pkt = simple_udp_packet(eth_dst=unknown_mac2,
                                     eth_src=unknown_mac1)
        self.chck_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                          eth_src=unknown_mac2)

        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        stored_fdb_entry = attr["available_fdb_entry"]

        send_packet(self, 1, self.pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [self.pkt], [self.dev_port_list[2:9]], timeout=1, n_timeout=1)
        verify_no_other_packets(self)

        send_packet(self, 2, self.chck_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [self.chck_pkt, self.chck_pkt], [self.dev_port_list[1:2], self.dev_port_list[3:9]])
        verify_no_other_packets(self)

        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        self.assertEqual(attr["available_fdb_entry"] - stored_fdb_entry, 0)
        print("Verification complete")

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        # restore initial VLAN Learning state
        sai_thrift_set_vlan_attribute(
            self.client, self.vlans[10].vlan_oid, learn_disable=False)


class BridgePortLearnDisableTest(T0TestBase):
    """
    Verify if MAC addresses are not learned on the port when the port is not a bridge port.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        status = sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        status = sai_thrift_set_bridge_port_attribute(
            self.client,
            self.bridge_port_list[1],
            fdb_learning_mode=SAI_BRIDGE_PORT_FDB_LEARNING_MODE_DISABLE)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        """
        1. Flush all MAC
        2. Disable MAC learn on Port1(Bridge Port1)
        3. Create a packet with SMAC ``MacX``
        4. send packet from port1
        5. Verify the packet flood to other VLAN10 ports
        6. Create a packet with DMAC ``MacX``
        7. send the packet on port2
        8. Verify the packet flood to other VLAN10 ports, including port1
        9. check FDB entries, no new entry
        """
        print("BridgePortLearnDisableTest")
        unknown_mac1 = "00:01:01:99:99:99"
        unknown_mac2 = "00:01:02:99:99:99"
        self.pkt = simple_udp_packet(eth_dst=unknown_mac2,
                                     eth_src=unknown_mac1,
                                     pktlen=100)
        self.chck_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                          eth_src=unknown_mac2,
                                          pktlen=100)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        stored_fdb_entry = attr["available_fdb_entry"]

        self.dataplane.flush()
        send_packet(self, 1, self.pkt)
        print("sleep 5 sec for mac learning entry write into db.")
        time.sleep(5)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        # unstable, cannot get the expected packet in a certain time
        verify_each_packet_on_multiple_port_lists(
            self, [self.pkt], [self.dev_port_list[2:9]])
        verify_no_other_packets(self)

        self.dataplane.flush()
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        self.assertEqual(attr["available_fdb_entry"] - stored_fdb_entry, 0)

        send_packet(self, 2, self.chck_pkt)
        print("sleep 1 sec for mac learning entry write into db.")
        time.sleep(1)

        # unstable, cannot get the expected packet in a certain time, need to wait up to 300 sec
        #Item: 15000918
        #verify_packet(self, self.pkt, self.dev_port_list[4])
        #verify_each_packet_on_multiple_port_lists(self, [self.chck_pkt], [self.dev_port_list[3:8]])
        # verify_no_other_packets(self)

        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        self.assertEqual(attr["available_fdb_entry"] - stored_fdb_entry, -1)

        print("Verification complete")

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        # restore initial bridge port Learning state
        status = sai_thrift_set_bridge_port_attribute(
            self.client,
            self.bridge_port_list[1],
            fdb_learning_mode=SAI_BRIDGE_PORT_FDB_LEARNING_MODE_HW)


"""
Skip test for broadcom, non bridge port still can learn.
"""


@skip
class NonBridgePortNoLearnTest(T0TestBase):
    """
    Verify if MAC addresses are not learned on the non-bridge port
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        status = sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        status = sai_thrift_remove_bridge_port(
            self.client, self.bridge_port_list[0])
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_remove_bridge_port(
            self.client, self.bridge_port_list[1])
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_remove_bridge_port(
            self.client, self.bridge_port_list[2])
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        """
        1. Flush all MAC
        2. Removed Port1 from Bridge Port1
        3. Create a packet with SMAC ``MacX``
        4. send packet from port1
        5. Verify the packet flood to other VLAN10 ports
        6. check FDB entries, no new entry
        7. Create a packet with DMAC ``MacX``
        8. send the packet on port2
        9. Verify the packet flood to other VLAN10 ports, including port1
        """
        print("NonBridgePortNoLearnTest")
        unknown_mac1 = "00:01:01:99:99:99"
        unknown_mac2 = "00:01:02:99:99:99"
        self.pkt = simple_udp_packet(eth_dst=unknown_mac2,
                                     eth_src=unknown_mac1,
                                     pktlen=100)
        self.chck_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                          eth_src=unknown_mac2,
                                          pktlen=100)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        stored_fdb_entry = attr["available_fdb_entry"]

        send_packet(self, 1, self.pkt)
        print("sleep 5 sec for mac learning entry write into db.")
        time.sleep(5)
        verify_each_packet_on_multiple_port_lists(
            self, [self.pkt], [self.dev_port_list[2:9]])
        verify_no_other_packets(self)

        # Case failed, mac learning happened
        #Item: 15000950
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        self.assertEqual(attr["available_fdb_entry"] - stored_fdb_entry, 0)

        send_packet(self, 2, self.chck_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [self.chck_pkt, self.chck_pkt], [self.dev_port_list[1:2], [self.dev_port_list[3:9]]])
        verify_no_other_packets(self)
        print("Verification complete")

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        # restore initial bridge port
        self.bridge_port_list[1] = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge_id,
            port_id=self.port_list[0],
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.bridge_port_list[1] = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge_id,
            port_id=self.port_list[1],
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.bridge_port_list[1] = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge_id,
            port_id=self.port_list[2],
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)


class NewVlanmemberLearnTest(T0TestBase):
    """
    Verify if MAC addresses are learned on the new add vlan member
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        self.new_vlan10_member = sai_thrift_create_vlan_member(self.client,
                                                               vlan_id=self.vlans[10].vlan_oid,
                                                               bridge_port_id=self.bridge_port_list[24])

        sai_thrift_set_port_attribute(
            self.client, self.port_list[24], port_vlan_id=10)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        """
        1. Add Port24 to VLAN10
        2. Create Packet with SMAC=``MacX`` DMAC=``Port1 MAC``
        3. Send packet on port24
        4. verify only receive a packet on port1
        5. Create a packet with DMAC=``MacX``
        6. Send packet on port1
        7. Verify only receive a packet on port24
        8. check FDB entries, new entry ``MacX`` on Port24 learned
        """
        print("NewVlanmemberLearnTest")
        unknown_mac1 = "00:01:01:99:99:99"
        self.pkt1 = simple_udp_packet(eth_dst=self.local_server_mac_list[1],
                                      eth_src=unknown_mac1)
        self.pkt2 = simple_udp_packet(eth_dst=unknown_mac1,
                                      eth_src=self.local_server_mac_list[1])
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        saved_fdb_entry = attr["available_fdb_entry"]

        send_packet(self, 24, self.pkt1)
        verify_packet(self, self.pkt1, self.dev_port_list[1])
        verify_no_other_packets(self)

        send_packet(self, 1, self.pkt2)
        verify_packet(self, self.pkt2, self.dev_port_list[24])
        verify_no_other_packets(self)

        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        # Dx010 counter is not right
        #Item: 15012803
        #self.assertEqual(attr["available_fdb_entry"] - saved_fdb_entry, -1)
        print("Verification complete")

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        sai_thrift_remove_vlan_member(self.client, self.new_vlan10_member)


class RemoveVlanmemberLearnTest(T0TestBase):
    """
    Verify no MAC addresses are learned on the removed vlan member
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        sai_thrift_remove_vlan_member(
            self.client, self.vlans[10].vlan_mport_oids[1])
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        """
        1. Remove Port2 from VLAN10
        2. Create a flood Packet with SMAC=``MacX`` and VLAN10 tag
        3. Send packet on port2
        4. Verify no packet was received on any port
        5. Create a packet with DMAC=``MacX`` and VLAN10 tag
        6. Send packet on port1
        7. Verify flooding to VLAN10 ports, no packet on port2
        8. check FDB entries, no new entry
        """
        print("RemoveVlanmemberLearnTest")
        unknown_mac1 = "00:01:01:99:99:99"
        self.pkt1 = simple_udp_packet(eth_dst=self.local_server_mac_list[1],
                                      eth_src=unknown_mac1,
                                      vlan_vid=10)
        self.pkt2 = simple_udp_packet(eth_dst=unknown_mac1,
                                      eth_src=self.local_server_mac_list[1],
                                      vlan_vid=10)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        stored_fdb_entry = attr["available_fdb_entry"]

        self.dataplane.flush()
        send_packet(self, 2, self.pkt1)
        verify_no_other_packets(self)

        self.dataplane.flush()
        send_packet(self, 1, self.pkt2)
        verify_each_packet_on_multiple_port_lists(
            self, [self.pkt2], [self.dev_port_list[3:9]])
        verify_no_other_packets(self)

        self.assertEqual(attr["available_fdb_entry"] - stored_fdb_entry, 0)
        print("Verification complete")

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        self.vlans[10].vlan_mport_oids[1] = sai_thrift_create_vlan_member(self.client,
                                                                          vlan_id=self.vlans[10].vlan_oid,
                                                                          bridge_port_id=self.bridge_port_list[1])


class InvalidateVlanmemberNoLearnTest(T0TestBase):
    """
    Verify no MAC addresses are learned on invalidate vlan member
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        """
        1. Create a packet with vlan_id=``VLAN11``
        2. Send packet on port2
        3. Verify no packet was received on any port
        4. Create a packet with vlan_id=``VLAN11`` DMAC=``MacX``
        5. Send packet on port1
        6. Dropped for ``VLAN11``
        7. check FDB entries, no new entry
        """
        print("InvalidateVlanmemberNoLearnTest")
        unknown_mac1 = "00:01:01:99:99:99"
        self.pkt1 = simple_udp_packet(eth_dst=self.local_server_mac_list[1],
                                      eth_src=unknown_mac1,
                                      vlan_vid=11)
        self.pkt2 = simple_udp_packet(eth_dst=unknown_mac1,
                                      eth_src=self.local_server_mac_list[1],
                                      vlan_vid=11)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        stored_fdb_entry = attr["available_fdb_entry"]

        send_packet(self, 2, self.pkt1)
        verify_no_other_packets(self)

        send_packet(self, 1, self.pkt2)
        verify_no_other_packets(self)

        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        self.assertEqual(attr["available_fdb_entry"] - stored_fdb_entry, 0)
        print("Verification complete")

    def tearDown(self):
        pass


class BroadcastNoLearnTest(T0TestBase):
    """
    Verify broadcast mac address is learned.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)

    def runTest(self):
        """
        1. Create a packet with SMAC=``broadcast address``
        2. Send packet on port2
        3. Verify no packet was received on any port
        4. Create a packet with DMAC=``broadcast address``
        5. Send packet on port1
        6. For broadcast and multicast address, flooding on all vlan10 ports, except port1
        7. check FDB entries, no new entry
        """
        print("BroadcastNoLearnTest")

        self.pkt1 = simple_udp_packet(eth_src=BROADCAST_MAC,
                                      pktlen=100)

        self.pkt2 = simple_udp_packet(eth_dst=BROADCAST_MAC,
                                      pktlen=100)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        stored_fdb_entry = attr["available_fdb_entry"]

        send_packet(self, 2, self.pkt1)
        verify_no_other_packets(self)

        send_packet(self, 1, self.pkt2)
        verify_each_packet_on_multiple_port_lists(
            self, [self.pkt2], [self.dev_port_list[2:9]])
        verify_no_other_packets(self)

        self.assertEqual(attr["available_fdb_entry"] - stored_fdb_entry, 0)
        print("Verification complete")

    def tearDown(self):
        pass


class MulticastNoLearnTest(T0TestBase):
    """
    Verify multicast mac address is learned.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)

    def runTest(self):
        """
        1. Create a packet with SMAC=``multicast address``
        2. Send packet on port2
        3. Verify no packet was received on any port
        4. Create a packet with DMAC=``multicast address``
        5. Send packet on port1
        6. For broadcast and multicast address, flooding on all vlan10 ports, except port1
        7. check FDB entries, no new entry
        """
        print("MulticastNoLearnTest")

        self.pkt1 = simple_udp_packet(eth_src=MULTICAST_MAC,
                                      pktlen=100)

        self.pkt2 = simple_udp_packet(eth_dst=MULTICAST_MAC,
                                      pktlen=100)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        stored_fdb_entry = attr["available_fdb_entry"]

        send_packet(self, 2, self.pkt1)
        verify_no_other_packets(self)

        print("sleep 1 sec for mac learning entry write into db.")
        time.sleep(1)
        self.dataplane.flush()
        send_packet(self, 1, self.pkt2)
        verify_each_packet_on_multiple_port_lists(
            self, [self.pkt2], [self.dev_port_list[2:9]])
        verify_no_other_packets(self)

        self.assertEqual(attr["available_fdb_entry"] - stored_fdb_entry, 0)
        print("Verification complete")

    def tearDown(self):
        pass


class FdbAgingTest(T0TestBase):
    """
    Verifying if the dynamic FDB entry associated with the port is removed after the aging interval.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        sw_attr = sai_thrift_get_switch_attribute(
            self.client, fdb_aging_time=True)
        self.default_wait_time = sw_attr["fdb_aging_time"]
        print("Default aging time is {} sec".format(self.default_wait_time))
        # age time used in tests (in sec)
        self.age_time = 10
        status = sai_thrift_set_switch_attribute(self.client,
                                                 fdb_aging_time=self.age_time)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        sw_attr = sai_thrift_get_switch_attribute(self.client,
                                                  fdb_aging_time=True)
        self.assertEqual(sw_attr["fdb_aging_time"], self.age_time)
        print("Set aging time to {} sec".format(self.age_time))

    def runTest(self):
        """
        1. Set FDB aging time=10
        2. Create Packet with SMAC=``MacX`` DMAC=``Port1 MAC`` 
        3. Send packet on port2
        4. verify only receive a packet on port1
        5. Create a packet with DMAC=``MacX``
        6. Send packet on port1
        7. Verify only receive a packet on port2
        8. Wait for the ``aging`` time
        9. Send packet on port1
        10. Verify flooding packet to VLAN10 ports, except port1
        """
        print("FdbAgingTest")
        unknown_mac1 = "00:01:01:99:99:99"
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[1],
                                eth_src=unknown_mac1,
                                pktlen=100)
        tag_pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[1],
                                    eth_src=unknown_mac1,
                                    dl_vlan_enable=True,
                                    vlan_vid=10,
                                    pktlen=104)
        send_packet(self, 2, tag_pkt)
        verify_packets(self, pkt, [self.dev_port_list[1]])
        verify_no_other_packets(self)
        time.sleep(1)

        print("Verifying if MAC address was learned")
        pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                eth_src=self.local_server_mac_list[1],
                                pktlen=100)
        tag_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                    eth_src=self.local_server_mac_list[1],
                                    dl_vlan_enable=True,
                                    vlan_vid=10,
                                    pktlen=104)

        send_packet(self, 1, tag_pkt)
        verify_packets(self, pkt, [self.dev_port_list[2]])
        verify_no_other_packets(self)
        print("OK. Mac learnt.")

        self.saiWaitFdbAge(self.age_time)
        print("Verify if aged MAC address was removed")
        send_packet(self, self.dev_port_list[1], pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[2:9]])
        verify_no_other_packets(self)
        print("\tVerification complete")

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        status = sai_thrift_set_switch_attribute(
            self.client, fdb_aging_time=self.default_wait_time)
        self.assertEqual(status, SAI_STATUS_SUCCESS)


class FdbAgingAfterMoveTest(T0TestBase):
    """
    Verifying the aging time refreshed if dynamic FDB entry associated with one port and then moved to another port (not the initial learning time).
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        sw_attr = sai_thrift_get_switch_attribute(
            self.client, fdb_aging_time=True)
        self.default_wait_time = sw_attr["fdb_aging_time"]
        print("Default aging time is {} sec".format(self.default_wait_time))

        # age time used in tests (in sec)
        self.age_time = 10
        status = sai_thrift_set_switch_attribute(self.client,
                                                 fdb_aging_time=self.age_time)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        sw_attr = sai_thrift_get_switch_attribute(self.client,
                                                  fdb_aging_time=True)
        self.assertEqual(sw_attr["fdb_aging_time"], self.age_time)
        print("Set aging time to {} sec".format(self.age_time))

    def runTest(self):
        """
        1. Set FDB aging time=10
        2. Create Packet with SMAC=``MacX`` DMAC=``Port1 MAC`` 
        3. Send packet on port2
        4. verify only receive a packet on port1
        5. Create a packet with DMAC=``MacX``
        6. Send packet on port1
        7. Verify only receive a packet on port2
        8. Send packet on port3
        7. Verify only receive a packet on port2
        9. Wait for the ``aging`` time
        10. Send packet on port3
        11. Verify flooding packet to VLAN10 ports, except port1
        """
        print("FdbAgingAfterMoveTest")
        unknown_mac1 = "00:01:01:99:99:99"
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[1],
                                eth_src=unknown_mac1,
                                pktlen=100)
        tag_pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[1],
                                    eth_src=unknown_mac1,
                                    dl_vlan_enable=True,
                                    vlan_vid=10,
                                    pktlen=104)
        chk_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                    eth_src=self.local_server_mac_list[1],
                                    pktlen=100)
        chk_tag_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                        eth_src=self.local_server_mac_list[1],
                                        dl_vlan_enable=True,
                                        vlan_vid=10,
                                        pktlen=104)
        send_packet(self, 2, tag_pkt)
        verify_packets(self, pkt, [self.dev_port_list[1]])
        verify_no_other_packets(self)
        print("Wait for 1 sec.")
        time.sleep(1)

        print("Verifying if MAC address was learned")

        self.dataplane.flush()
        send_packet(self, 1, chk_tag_pkt)
        verify_packets(self, chk_pkt, [self.dev_port_list[2]])
        verify_no_other_packets(self)
        print("OK. Mac learnt.")

        print("Verifying if MAC address moved")
        print("Wait for age time {} - {}".format(self.age_time, 5))
        time.sleep(self.age_time - 5)
        self.dataplane.flush()
        send_packet(self, 3, tag_pkt)
        verify_packets(self, pkt, [self.dev_port_list[1]])
        verify_no_other_packets(self)
        print("Wait for 2 sec.")
        time.sleep(2)

        print("Wait for age time {} - {}".format(self.age_time, 5))
        time.sleep(self.age_time - 5)
        self.dataplane.flush()
        send_packet(self, self.dev_port_list[1], chk_tag_pkt)
        verify_packet(self, chk_pkt, self.dev_port_list[3])
        print("OK. Mac Moved.")

        print("Wait for 1 sec.")
        time.sleep(1)
        print("Verify if aged MAC address was removed")
        self.saiWaitFdbAge(self.age_time)
        self.dataplane.flush()
        send_packet(self, self.dev_port_list[1], chk_tag_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [chk_pkt], [self.dev_port_list[2:9]])
        verify_no_other_packets(self)
        print("\tVerification complete")

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        status = sai_thrift_set_switch_attribute(
            self.client, fdb_aging_time=self.default_wait_time)
        self.assertEqual(status, SAI_STATUS_SUCCESS)


class FdbMacMovingAfterAgingTest(T0TestBase):
    """
    Verifying the mac can be learnt if the mac aging reached.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        sw_attr = sai_thrift_get_switch_attribute(
            self.client, fdb_aging_time=True)
        self.default_wait_time = sw_attr["fdb_aging_time"]
        print("Default aging time is {} sec".format(self.default_wait_time))

        # age time used in tests (in sec)
        self.age_time = 10
        status = sai_thrift_set_switch_attribute(self.client,
                                                 fdb_aging_time=self.age_time)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        sw_attr = sai_thrift_get_switch_attribute(self.client,
                                                  fdb_aging_time=True)
        self.assertEqual(sw_attr["fdb_aging_time"], self.age_time)
        print("Set aging time to {} sec".format(self.age_time))

    def runTest(self):
        """
        1. Set FDB aging time=10
        2. Create Packet with SMAC=``MacX`` DMAC=``Port1 MAC`` 
        3. Send packet on port2
        4. verify only receive a packet on port1
        5. Create a packet with DMAC=``MacX``
        6. Send packet on port1
        7. Verify only receive a packet on port2
        8. Wait for the ``aging`` time
        9. Send packet on port3
        10. Verify only receive a packet on port2
        """
        print("FdbMacMovingAfterAgingTest")
        unknown_mac1 = "00:01:01:99:99:99"
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[1],
                                eth_src=unknown_mac1,
                                pktlen=100)
        tag_pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[1],
                                    eth_src=unknown_mac1,
                                    dl_vlan_enable=True,
                                    vlan_vid=10,
                                    pktlen=104)
        chk_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                    eth_src=self.local_server_mac_list[1],
                                    pktlen=100)
        chk_tag_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                        eth_src=self.local_server_mac_list[1],
                                        dl_vlan_enable=True,
                                        vlan_vid=10,
                                        pktlen=104)
        send_packet(self, 2, tag_pkt)
        verify_packets(self, pkt, [self.dev_port_list[1]])
        verify_no_other_packets(self)
        print("Wait for 1 sec.")
        time.sleep(1)

        print("Verifying if MAC address was learned")

        self.dataplane.flush()
        send_packet(self, 1, chk_tag_pkt)
        verify_packets(self, chk_pkt, [self.dev_port_list[2]])
        verify_no_other_packets(self)
        print("OK. Mac learnt.")

        self.saiWaitFdbAge(self.age_time)

        print("Verifying if MAC address moved")
        self.dataplane.flush()
        send_packet(self, 3, tag_pkt)
        verify_packets(self, pkt, [self.dev_port_list[1]])
        verify_no_other_packets(self)
        print("Wait for 2 sec.")
        time.sleep(2)

        print("Wait for age time {} - {}".format(self.age_time, 5))
        time.sleep(self.age_time - 5)
        self.dataplane.flush()
        send_packet(self, self.dev_port_list[1], chk_tag_pkt)
        verify_packet(self, chk_pkt, self.dev_port_list[3])
        print("OK. Mac Moved.")
        print("\tVerification complete")

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        status = sai_thrift_set_switch_attribute(
            self.client, fdb_aging_time=self.default_wait_time)
        self.assertEqual(status, SAI_STATUS_SUCCESS)


"""
SKIP: Static flush Not support by broadcom
"""


@skip
class FdbFlushVlanStaticTest(T0TestBase):
    '''
    Verify flushing of static MAC entries on VLAN
    '''

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        status = sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_STATIC)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        """
        1. Flush with conditions : ``Static`` flush on ``VLAN10`` 
        2. Send packets : ``port1`` DMAC=``Port2 MAC``
        3. Verify flooding happened, packets received in related VLAN, except the ingress port.
        4. Send packets :  ``Port9`` DMAC=``Port10 MAC``
        5. Verify flush happens in a certain domain, unicast to the corresponding port.
        """
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[2],
                                pktlen=100)
        send_packet(self, 1, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[2:9]])
        print("\tVerified the flooding happend")
        time.sleep(1)
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[10],
                                pktlen=100)
        send_packet(self, 9, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[10]])
        print("\tVerified the flooding happend")

        print("\tVerification complete")

    def tearDown(self):
        self.t0_fdb_tear_down_helper()
        self.t0_fdb_config_helper()


"""
SKIP: Static flush Not support by broadcom
"""


@skip
class FdbFlushPortStaticTest(T0TestBase):
    '''
    Verify flushing of static MAC entries on Port
    '''

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        sai_thrift_flush_fdb_entries(
            self.client, bridge_port_id=self.bridge_port_list[1], entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_STATIC)

    def runTest(self):
        """
        1. Flush with conditions : ``Static`` flush on ``Port1``
        2. Send packets  ``Port2`` DMAC=``Port1 MAC``
        3. Verify flooding happened, packets received in related VLAN, except the ingress port.
        4. Send packets  ``Port10`` DMAC=``Port9 MAC``
        5. Verify flush happens in a certain domain, unicast to the corresponding port.
        """
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[1],
                                pktlen=100)
        send_packet(self, 2, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[2:9]])
        print("\tVerified the flooding happend")
        time.sleep(1)
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[9],
                                pktlen=100)
        send_packet(self, 10, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[9]])
        print("\tVerified the flooding happend")

        print("\tVerification complete")

    def tearDown(self):
        self.restore_fdb_config()


"""
SKIP: Static flush Not support by broadcom
"""


@skip
class FdbFlushAllStaticTest(T0TestBase):
    '''
    Verify flushing all of the static MAC entries.
    '''

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_STATIC)

    def runTest(self):
        """
        1. Flush with conditions : flush for all ``Static`` 
        2. Send packets :  ``port1`` DMAC=``Port2 MAC``
        3. Verify flooding happened, packets received in related VLAN, except the ingress port.
        4. Send packets :  ``Port9`` DMAC=``Port10 MAC``
        5. Verify flush happens in a certain domain, unicast to the corresponding port.
        """
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[2],
                                pktlen=100)
        send_packet(self, 1, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[2:9]])
        print("\tVerified the flooding happend")
        time.sleep(1)
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[10],
                                pktlen=100)
        send_packet(self, 9, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[10]])
        print("\tVerified the flooding happend")

        print("\tVerification complete")

    def tearDown(self):
        self.restore_fdb_config()


class FdbFlushVlanDynamicTest(T0TestBase):
    '''
    Verify flushing of dynamic MAC entries on VLAN
    '''

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)

    def runTest(self):
        """
        1. create packet with src mac ``MacX``, vlan tag 20
        2. Send packets on port9:  ``port9`` DMAC=``Port10 MAC``
        3. Verify unicast happen
        4. Send packet on port10: DMAC=``MacX``
        5. Verify unicast happen
        6. flush vlan dynamic fdb
        7. check the fdb entries, no new entries get from previsou step
        8. send packet same as step 4
        9. verify flooding happen
        """
        unknown_mac1 = "00:01:01:99:99:99"
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[1],
                                eth_src=unknown_mac1,
                                pktlen=100)
        tag_pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[1],
                                    eth_src=unknown_mac1,
                                    dl_vlan_enable=True,
                                    vlan_vid=20,
                                    pktlen=104)
        chk_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                    eth_src=self.local_server_mac_list[1],
                                    pktlen=100)
        chk_tag_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                        eth_src=self.local_server_mac_list[1],
                                        dl_vlan_enable=True,
                                        vlan_vid=20,
                                        pktlen=104)
        time.sleep(2)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        saved_fdb_entry = attr["available_fdb_entry"]
        self.dataplane.flush()
        send_packet(self, 9, tag_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[10:16]])
        print("\tVerified the flooding happend")
        time.sleep(1)
        send_packet(self, 10, chk_tag_pkt)
        verify_packets(self, chk_pkt, [self.dev_port_list[9]])

        status = sai_thrift_flush_fdb_entries(
            self.client, bv_id=self.vlans[20].vlan_oid, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        # DX010, Counter not flush on vlan
        #Item: 15012831
        #self.assertEqual(attr["available_fdb_entry"] - saved_fdb_entry, 0)
        #Item: 15002648
        # Unstable, flood cannot be recovered
        # After 300 more seconds, flooding happened
        # self.dataplane.flush()
        # send_packet(self, 10, chk_tag_pkt)
        # verify_packet(self, chk_pkt, self.dev_port_list[9])
        # verify_packets(self, chk_pkt, self.dev_port_list[11:17])
        print("\tVerification complete")

    def tearDown(self):
        pass


class FdbFlushPortDynamicTest(T0TestBase):
    '''
    Verify flushing of dynamic MAC entries on Port.
    '''

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)

    def runTest(self):
        """
        1. create packet with src mac ``MacX``, vlan tag 20
        2. Send packets on port9:  ``port9`` DMAC=``Port10 MAC``
        3. Verify unicast happen
        4. Send packet on port10: DMAC=``MacX``
        5. Verify unicast happen
        6. flush dynamic fdb on port9
        7. check the fdb entries, no new entries get from previsou step
        8. send packet same as step 4
        9. verify flooding happen
        """
        unknown_mac1 = "00:01:01:99:99:99"
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[10],
                                eth_src=unknown_mac1,
                                pktlen=100)
        tag_pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[10],
                                    eth_src=unknown_mac1,
                                    dl_vlan_enable=True,
                                    vlan_vid=20,
                                    pktlen=104)
        chk_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                    eth_src=self.local_server_mac_list[10],
                                    pktlen=100)
        chk_tag_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                        eth_src=self.local_server_mac_list[10],
                                        dl_vlan_enable=True,
                                        vlan_vid=20,
                                        pktlen=104)
        status = sai_thrift_flush_fdb_entries(
            self.client, bridge_port_id=self.bridge_port_list[9], entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        saved_fdb_entry = attr["available_fdb_entry"]
        self.dataplane.flush()
        send_packet(self, 9, tag_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[10:16]])
        print("\tVerified the flooding happend")
        time.sleep(1)
        send_packet(self, 10, chk_tag_pkt)
        verify_packets(self, chk_pkt, [self.dev_port_list[9]])
        status = sai_thrift_flush_fdb_entries(
            self.client, bridge_port_id=self.bridge_port_list[9], entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        # Item: 15003334, counter is not right
        #self.assertEqual(attr["available_fdb_entry"] - saved_fdb_entry, 0)
        #Item: 15002648
        # Unstable, flood cannot be recovered
        # After 300 more seconds, flooding happened
        # self.dataplane.flush()
        # send_packet(self, 10, chk_tag_pkt)
        # verify_packet(self, chk_pkt, self.dev_port_list[9])
        # verify_packets(self, chk_pkt, self.dev_port_list[11:17])
        print("\tVerification complete")

    def tearDown(self):
        pass


class FdbFlushAllDynamicTest(T0TestBase):
    '''
    Verify flushing all of the dynamic MAC entries.
    '''

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)

    def runTest(self):
        """
        1. create packet with src mac ``MacX``, vlan tag 20
        2. Send packets on port9:  ``port9`` DMAC=``Port10 MAC``
        3. Verify unicast happen
        4. Send packet on port10: DMAC=``MacX``
        5. Verify unicast happen
        6. flush all dynamic fdb
        7. check the fdb entries, no new entries get from previsou step
        8. send packet same as step 4
        9. verify flooding happen
        """
        unknown_mac1 = "00:01:01:99:99:99"
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[10],
                                eth_src=unknown_mac1,
                                pktlen=100)
        tag_pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[10],
                                    eth_src=unknown_mac1,
                                    dl_vlan_enable=True,
                                    vlan_vid=20,
                                    pktlen=104)
        chk_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                    eth_src=self.local_server_mac_list[10],
                                    pktlen=100)
        chk_tag_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                        eth_src=self.local_server_mac_list[10],
                                        dl_vlan_enable=True,
                                        vlan_vid=20,
                                        pktlen=104)
        status = sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        saved_fdb_entry = attr["available_fdb_entry"]
        self.dataplane.flush()
        send_packet(self, 9, tag_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[10:16]])
        print("\tVerified the flooding happend")
        time.sleep(1)
        send_packet(self, 10, chk_tag_pkt)
        verify_packets(self, chk_pkt, [self.dev_port_list[9]])
        status = sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)

        self.assertEqual(attr["available_fdb_entry"] - saved_fdb_entry, 0)
        #Item: 15002648
        # Unstable, flood cannot be recovered
        # After 300 more seconds, flooding happened
        # self.dataplane.flush()
        # send_packet(self, 10, chk_tag_pkt)
        # verify_packet(self, chk_pkt, self.dev_port_list[9])
        # verify_packets(self, chk_pkt, self.dev_port_list[11:17])
        print("\tVerification complete")

    def tearDown(self):
        pass


"""
SKIP: static not support
"""


@skip
class FdbFlushAllTest(T0TestBase):
    '''
    Verify flushing all  MAC entries.
    '''

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)

    def runTest(self):
        """
        1. create packet with src mac ``MacX``, vlan tag 20
        2. Send packets on port9:  ``port9`` DMAC=``Port10 MAC``
        3. Verify unicast happen
        4. Send packet on port10: DMAC=``MacX``
        5. Verify unicast happen
        6. flush all dynamic fdb
        7. check the fdb entries, no new entries get from previsou step
        8. send packet same as step 4
        9. verify flooding happen
        """
        unknown_mac1 = "00:01:01:99:99:99"
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[10],
                                eth_src=unknown_mac1,
                                pktlen=100)
        tag_pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[10],
                                    eth_src=unknown_mac1,
                                    dl_vlan_enable=True,
                                    vlan_vid=20,
                                    pktlen=104)
        chk_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                    eth_src=self.local_server_mac_list[10],
                                    pktlen=100)
        chk_tag_pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                        eth_src=self.local_server_mac_list[10],
                                        dl_vlan_enable=True,
                                        vlan_vid=20,
                                        pktlen=104)
        status = sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_STATIC)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        saved_fdb_entry = attr["available_fdb_entry"]
        self.dataplane.flush()
        send_packet(self, 9, tag_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[10:16]])
        print("\tVerified the flooding happend")
        time.sleep(1)
        send_packet(self, 10, chk_tag_pkt)
        verify_packets(self, chk_pkt, [self.dev_port_list[9]])
        status = sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_STATIC)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)

        self.assertEqual(attr["available_fdb_entry"] - saved_fdb_entry, 0)
        #Item: 15002648
        # Unstable, flood cannot be recovered
        # After 300 more seconds, flooding happened
        # self.dataplane.flush()
        # send_packet(self, 10, chk_tag_pkt)
        # verify_packet(self, chk_pkt, self.dev_port_list[9])
        # verify_packets(self, chk_pkt, self.dev_port_list[11:17])
        print("\tVerification complete")

    def tearDown(self):
        self.restore_fdb_config()


class FdbDisableMacMoveDropTest(T0TestBase):
    '''
    Verify if disable MAC move, drop packet with known SMAC if the SMAC was already learnt on other port.
    '''

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        self.fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                mac_address=self.local_server_mac_list[1],
                                                bv_id=self.vlans[10].vlan_oid)
        status = sai_thrift_create_fdb_entry(self.client,
                                             self.fdb_entry,
                                             type=SAI_FDB_ENTRY_TYPE_DYNAMIC,
                                             bridge_port_id=self.bridge_port_list[1],
                                             allow_mac_move=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        """
        1. Disable mac move for ``Port1 MAC`` on Port1
        2. Create a packet with SMAC=``Port1 MAC`` DMAC=``Port2 MAC``
        3. Send packet on port1
        4. Verify packet received on port2
        5. Send packet in step2 on port3
        6. Verify the packet gets dropped
        """
        self.pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[2],
                                     eth_src=self.local_server_mac_list[1],
                                     pktlen=100)
        send_packet(self, 1, self.pkt)
        verify_packet(self, self.pkt, self.dev_port_list[2])

        send_packet(self, 3, self.pkt)
        verify_no_other_packets(self)
        print("\tVerification complete")

    def tearDown(self):
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry)
        status = sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        self.assertEqual(status, SAI_STATUS_SUCCESS)


class FdbDynamicMacMoveTest(T0TestBase):
    '''
    Verify when enabling MAC move, previous learnt mac(SMAC) on a port can be learnt on other port
    '''

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        sai_thrift_flush_fdb_entries(
            self.client,
            entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        """
        1. Flush All MAC
        2. Install static FDB entry for port2 with ``Port2 MAC``
        3. Send Packet on Port1 with SMAC=``Port1 MAC`` DMAC=``Port2 MAC``
        4. Create packet with SMAC=``Port1 MAC`` DMAC=``Port2 MAC``
        5. Send packet on port1
        6. Install static mac move for ``Port1 MAC`` on Port1 and enable mac move
        7. Verify packet received on port2
        8. Send packet in step2 on port3
        9. Verify packet received on port2
        """
        self.pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[2],
                                     eth_src=self.local_server_mac_list[1],
                                     pktlen=100)
        send_packet(self, 1, self.pkt)
        verify_packet(self, self.pkt, self.dev_port_list[2])
        # inititally add moving MAC to FDB
        self.moving_fdb_entry = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=self.local_server_mac_list[1])
        status = sai_thrift_create_fdb_entry(
            self.client,
            self.moving_fdb_entry,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.bridge_port_list[1],
            allow_mac_move=True)
        status = sai_thrift_create_fdb_entry(
            self.client,
            self.moving_fdb_entry,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.bridge_port_list[3],
            allow_mac_move=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        time.sleep(1)
        self.dataplane.flush()
        send_packet(self, 3, self.pkt)
        verify_packet(self, self.pkt, self.dev_port_list[2])
        print("\tVerification complete")

    def tearDown(self):
        sai_thrift_remove_fdb_entry(self.client, self.moving_fdb_entry)
        status = sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        self.assertEqual(status, SAI_STATUS_SUCCESS)


class FdbStaticMacMoveTest(T0TestBase):
    '''
    Verify when enabling MAC move, previous installed mac(static SMAC) on a port can be set to other port
    '''

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        sai_thrift_flush_fdb_entries(
            self.client,
            entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        self.fdb_entry1 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                 mac_address=self.local_server_mac_list[2],
                                                 bv_id=self.vlans[10].vlan_oid)
        status = sai_thrift_create_fdb_entry(self.client,
                                             self.fdb_entry1,
                                             type=SAI_FDB_ENTRY_TYPE_STATIC,
                                             bridge_port_id=self.bridge_port_list[2])
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.fdb_entry2 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                 mac_address=self.local_server_mac_list[1],
                                                 bv_id=self.vlans[10].vlan_oid)
        status = sai_thrift_create_fdb_entry(self.client,
                                             self.fdb_entry2,
                                             type=SAI_FDB_ENTRY_TYPE_DYNAMIC,
                                             bridge_port_id=self.bridge_port_list[1],
                                             allow_mac_move=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        """
        1. Flush All MAC
        2. Install static FDB entry for port2 with ``Port2 MAC``
        3. Install static FDB entry for port1 with ``Port1 MAC``  
        4. Enable mac move for ``Port1 MAC`` on Port1
        5. Create packet with SMAC=``Port1 MAC`` DMAC=``Port2 MAC``
        6. Send packet on port1
        7. Verify packet received on port2
        8. Install static FDB entry for port3 with ``Port1 MAC``  
        9. Send packet in step2 on port3
        10. Verify packet received on port2
        """
        self.pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[2],
                                     eth_src=self.local_server_mac_list[1],
                                     pktlen=100)
        send_packet(self, 1, self.pkt)
        verify_packet(self, self.pkt, self.dev_port_list[2])

        # inititally add moving MAC to FDB
        self.moving_fdb_entry = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=self.local_server_mac_list[1])
        status = sai_thrift_create_fdb_entry(
            self.client,
            self.moving_fdb_entry,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.bridge_port_list[3],
            allow_mac_move=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        send_packet(self, 3, self.pkt)
        verify_packet(self, self.pkt, self.dev_port_list[2])

        print("\tVerification complete")

    def tearDown(self):
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry1)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry2)
        sai_thrift_remove_fdb_entry(self.client, self.moving_fdb_entry)
        status = sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
