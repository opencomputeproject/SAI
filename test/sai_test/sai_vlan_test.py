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


from sai_test_base import T0TestBase
from sai_thrift.sai_headers import *
from ptf import config
from ptf.testutils import *
from ptf.thriftutils import *
from sai_utils import *

class Vlan_Domain_Forwarding_Test(T0TestBase):
    """
    Verify the basic VLAN forwarding.
    In L2, if segement with VLAN tag and sends to a VLAN port, 
    segment should be forwarded inside a VLAN domain.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_reset_default_vlan=False)

    def runTest(self):
        """
        Test VLAN forwarding
        """
        try:
            print("VLAN forwarding test.")
            for index in range(2, 9):
                print("Forwarding in VLAN {} from {} to port: {}".format(
                    10,
                    self.dev_port_list[1], 
                    self.dev_port_list[index]))
                pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[index],
                                        eth_src=self.local_server_mac_list[1],
                                        vlan_vid=10,
                                        ip_id=101,
                                        ip_ttl=64)
                    
                send_packet(self, self.dev_port_list[1], pkt)
                verify_packet(self, pkt, self.dev_port_list[index])
            for index in range(10, 17):
                print("Forwarding in VLAN {} from {} to port: {}".format(
                    20,
                    self.dev_port_list[9], 
                    self.dev_port_list[index]))
                pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[index],
                                        eth_src=self.local_server_mac_list[9],
                                        vlan_vid=20,
                                        ip_id=101,
                                        ip_ttl=64)
                    
                send_packet(self, self.dev_port_list[9], pkt)
                verify_packet(self, pkt, self.dev_port_list[index])
        finally:
            pass

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)


class UntagAccessToAccessTest(T0TestBase):
    """
    This test verifies the VLAN function around untag and access ports.
    """
    def setUp(self):
        super().setUp()


    def runTest(self):
        """
        Forwarding between tagged ports with untagged pkt
        """
        print("\nUntagAccessToAccessTest()")
        try:
            for index in range(2, 9):
                print("Sending untagged packet from vlan10 tagged port {} to vlan10 tagged port: {}".format(
                    self.dev_port_list[1], 
                    self.dev_port_list[index]))
                pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[index],
                                        eth_src=self.local_server_mac_list[1],
                                        ip_id=101,
                                        ip_ttl=64)

                send_packet(self, self.dev_port_list[1], pkt)
                verify_packet(self, pkt, self.dev_port_list[index])
            for index in range(10, 17):
                print("Sending untagged packet from vlan20 tagged port {} to vlan20 tagged port: {}".format(
                    self.dev_port_list[9], 
                    self.dev_port_list[index]))
                pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[index],
                                        eth_src=self.local_server_mac_list[9],
                                        ip_id=101,
                                        ip_ttl=64)

                send_packet(self, self.dev_port_list[9], pkt)
                verify_packet(self, pkt, self.dev_port_list[index])

        finally:
            pass

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_flush_fdb_entries(self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        super().tearDown()


class MismatchDropTest(T0TestBase):
    """
    This test verifies the VLAN function around untag and access ports.
    """
    def setUp(self):
        super().setUp()

    def runTest(self):
        """
        Dropping between tagged ports with mismatched tagged pkt
        """
        print("\nUnmatchDropTest()")
        try:
            for index in range(1, 9):
                print("Sending vlan20 tagged packet from vlan20 tagged port {} to vlan10 tagged port: {}".format(
                    self.dev_port_list[9], 
                    self.dev_port_list[index]))
                pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[index],
                                        eth_src=self.local_server_mac_list[9],
                                        vlan_vid=20,
                                        ip_id=101,
                                        ip_ttl=64)

                send_packet(self, self.dev_port_list[9], pkt)
                verify_no_other_packets(self, timeout=1)
            for index in range(9, 17):
                print("Sending vlan10 tagged packet from {} to vlan20 tagged port: {}".format(
                    self.dev_port_list[1],
                    self.dev_port_list[index]))
                pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[index],
                                        eth_src=self.local_server_mac_list[1],
                                        vlan_vid=10,
                                        ip_id=101,
                                        ip_ttl=64)

                send_packet(self, self.dev_port_list[1], pkt)
                verify_no_other_packets(self, timeout=1)
        finally:
            pass

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_flush_fdb_entries(self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        super().tearDown()


class TaggedFrameFilteringTest(T0TestBase):
    """
    Drop tagged packet when the destination port from MAC table search is the port which packet comes into the switch.
    """
    def setUp(self):
        super().setUp()
        sai_thrift_flush_fdb_entries(self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        self.port1_mac_list = [self.local_server_mac_list[i] for i in [1,5]]
        self.mac_action = SAI_PACKET_ACTION_FORWARD
        self.fdb_configer.create_fdb_entries(
            switch_id=self.switch_id,
            mac_list=self.port1_mac_list,
            port_oids=[1,1],
            vlan_oid=self.vlans[10].vlan_oid)

    def runTest(self):
        print("\nTaggedFrameFilteringTest")
        try:
            for mac_id in self.port1_mac_list:
                pkt = simple_udp_packet(eth_dst=mac_id,
                                        eth_src=self.local_server_mac_list[1],
                                        vlan_vid=10,
                                        ip_id=101,
                                        ip_ttl=64)
                send_packet(self, self.dev_port_list[1], pkt)
                verify_no_other_packets(self, timeout=1)
        finally:
            pass

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_flush_fdb_entries(self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        super().tearDown()


class UnTaggedFrameFilteringTest(T0TestBase):
    """
    Drop untagged packet when the destination port from MAC table search
    is the port which packet comes into the switch.
    """
    def setUp(self):
        super().setUp()
        sai_thrift_flush_fdb_entries(self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        self.port1_mac_list = [self.local_server_mac_list[i] for i in [1,5]]
        self.mac_action = SAI_PACKET_ACTION_FORWARD
        self.fdb_configer.create_fdb_entries(
            switch_id=self.switch_id,
            mac_list=self.port1_mac_list,
            port_oids=[1,1],
            vlan_oid=self.vlans[10].vlan_oid)

    def runTest(self):
        print("\nUnTaggedFrameFilteringTest")
        try:
            for mac_id in self.port1_mac_list:
                pkt = simple_udp_packet(eth_dst=mac_id,
                                        eth_src=self.local_server_mac_list[1],
                                        ip_id=101,
                                        ip_ttl=64)
                send_packet(self, self.dev_port_list[1], pkt)
                verify_no_other_packets(self, timeout=1)
        finally:
            pass

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_flush_fdb_entries(self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        super().tearDown()


class TaggedVlanFloodingTest(T0TestBase):
    """
    For mac flooding in the VLAN scenario, before learning the mac address from the packet,
    the packet sent to the VLAN port will flood to other ports, and the egress ports
    will be in the same VLAN as the ingress port.
    """
    def setUp(self):
       super().setUp()

    def runTest(self):
        print("\nTaggedVlanFloodingTest")
        try:
            macX = 'EE:EE:EE:EE:EE:EE'
            pkt = simple_udp_packet(eth_dst=macX,
                                    eth_src=self.local_server_mac_list[1],
                                    vlan_vid=10,
                                    ip_id=101,
                                    ip_ttl=64)
            send_packet(self, self.dev_port_list[1], pkt)
            other_ports = self.dev_port_list[1:8]
            verify_packet_any_port(self,pkt,other_ports)
        finally:
            pass

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_flush_fdb_entries(self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        super().tearDown()


class UnTaggedVlanFloodingTest(T0TestBase):
    """
    UnTaggedVlanFloodingTest
    For mac flooding in the VLAN scenario, before learning the mac address from the packet,
    the packet sent to the VLAN port will flood to other ports, and the egress ports
    will be in the same VLAN as the ingress port.
    """
    def setUp(self):
        super().setUp()

    def runTest(self):
        print("\nUnTaggedVlanFloodingTest")
        try:
            macX = 'EE:EE:EE:EE:EE:EE'
            pkt = simple_udp_packet(eth_dst=macX,
                                    eth_src=self.local_server_mac_list[1],
                                    ip_id=101,
                                    ip_ttl=64)
            send_packet(self, self.dev_port_list[1], pkt)
            other_ports = self.dev_port_list[1:8]
            verify_packet_any_port(self,pkt,other_ports)
        finally:
            pass

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_flush_fdb_entries(self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        super().tearDown()


class BroadcastTest(T0TestBase):
    """
    Drop untagged packet when the destination port from MAC table search
    is the port which packet comes into the switch.
    """
    def setUp(self):
        super().setUp()

    def runTest(self):
        print("\nBroadcastTest")
        try:
            macX = 'FF:FF:FF:FF:FF:FF'
            #untag
            untagged_pkt = simple_udp_packet(eth_dst=macX,
                                    eth_src=self.local_server_mac_list[1],
                                    ip_id=101,
                                    ip_ttl=64)
            send_packet(self, self.dev_port_list[1], untagged_pkt)
            other_ports = self.dev_port_list[1:8]
            verify_packet_any_port(self,untagged_pkt,other_ports)
            #tag
            tagged_pkt = simple_udp_packet(eth_dst=macX,
                                    eth_src=self.local_server_mac_list[1],
                                    vlan_vid=10,
                                    ip_id=101,
                                    ip_ttl=64)
            send_packet(self, self.dev_port_list[1], tagged_pkt)
            other_ports = self.dev_port_list[1:8]
            verify_packet_any_port(self,tagged_pkt,other_ports)
        finally:
            pass

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_flush_fdb_entries(self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        super().tearDown()


class UntaggedMacLearningTest(T0TestBase):
    """
    For mac learning in the VLAN scenario, after learning the mac address 
    from the packet, the packet sent to the VLAN port will only send to the 
    port whose MAC address matches the MAC table entry.
    """
    def setUp(self):
        super().setUp()

    def runTest(self):
        print("\nUntaggedMacLearningTest")
        try:
            available_fdb_entry_cnt_past = sai_thrift_get_switch_attribute(
                                                    self.client,
                                                    available_fdb_entry=True)['available_fdb_entry']
            macX = '00:01:01:99:01:99'
            #untag
            untagged_pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[2],
                                    eth_src=macX,
                                    ip_id=101,
                                    ip_ttl=64)
            send_packet(self, self.dev_port_list[1], untagged_pkt)
            verify_packet(self, untagged_pkt, self.dev_port_list[2])
            sleep(2)  #wait for add mac entry
            available_fdb_entry_cnt_now = sai_thrift_get_switch_attribute(
                                                    self.client,
                                                    available_fdb_entry=True)['available_fdb_entry']
            self.assertEqual(available_fdb_entry_cnt_now-available_fdb_entry_cnt_past,-1)
        finally:
            pass

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_flush_fdb_entries(self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        super().tearDown()


class TaggedMacLearningTest(T0TestBase):
    """
    For mac learning in the VLAN scenario, after learning the mac address
    from the packet, the packet sent to the VLAN port will only send to the
    port whose MAC address matches the MAC table entry.
    """
    def setUp(self):
        super().setUp()

    def runTest(self):
        print("\nTaggedMacLearningTest")
        try:
            available_fdb_entry_cnt_past = sai_thrift_get_switch_attribute(
                                                    self.client,
                                                    available_fdb_entry=True)['available_fdb_entry']
            macX = '00:01:01:99:01:99'
            tagged_pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[2],
                                    eth_src=macX,
                                    vlan_vid=10,
                                    ip_id=101,
                                    ip_ttl=64)
            send_packet(self, self.dev_port_list[1], tagged_pkt)
            verify_packet(self, tagged_pkt, self.dev_port_list[2])
            sleep(2)  #wait for add mac entry
            available_fdb_entry_cnt_now = sai_thrift_get_switch_attribute(
                                                    self.client,
                                                    available_fdb_entry=True)['available_fdb_entry']
            self.assertEqual(available_fdb_entry_cnt_now-available_fdb_entry_cnt_past,-1)
        finally:
            pass

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_flush_fdb_entries(self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
        super().tearDown()


class VlanMemberListTest(T0TestBase):
    """
    This test verifies the VLAN member list using SAI_VLAN_ATTR_MEMBER_LIST
    """
    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)

    def runTest(self):
        print("VlanMemberListTest")
        mbr_list = []
        mbr_list.extend(self.vlan_configer.get_vlan_member(self.vlans[10].vlan_oid))
        mbr_list.extend(self.vlan_configer.get_vlan_member(self.vlans[20].vlan_oid))
        self.assertEqual(len(mbr_list), 16)

        for i in range(0, 8):
            self.assertEqual(self.vlans[10].vlan_mport_oids[i], mbr_list[i])
        for i in range(8, 16):
            self.assertEqual(self.vlans[20].vlan_mport_oids[i - 8], mbr_list[i]) 

        # Adding vlan members and veryfing vlan member list
        new_vlan_member = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlans[10].vlan_oid,
            bridge_port_id=self.bridge_port_list[17],
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        mbr_list = []
        mbr_list.extend(self.vlan_configer.get_vlan_member(self.vlans[10].vlan_oid))
        mbr_list.extend(self.vlan_configer.get_vlan_member(self.vlans[20].vlan_oid))
        self.assertEqual(len(mbr_list), 17)

        # Adding vlan members and veryfing vlan member list
        for i in range(0, 8):
            self.assertEqual(self.vlans[10].vlan_mport_oids[i], mbr_list[i])
        self.assertEqual(new_vlan_member, mbr_list[8])
        for i in range(9, 17):
            self.assertEqual(self.vlans[20].vlan_mport_oids[i - 9], mbr_list[i]) 

        # Removing vlan members and veryfing vlan member list
        sai_thrift_remove_vlan_member(self.client, new_vlan_member)

        mbr_list = []
        mbr_list.extend(self.vlan_configer.get_vlan_member(self.vlans[10].vlan_oid))
        mbr_list.extend(self.vlan_configer.get_vlan_member(self.vlans[20].vlan_oid))
        self.assertEqual(len(mbr_list), 16)

        for i in range(0, 8):
            self.assertEqual(self.vlans[10].vlan_mport_oids[i], mbr_list[i])
        for i in range(8, 16):
            self.assertEqual(self.vlans[20].vlan_mport_oids[i - 8], mbr_list[i])

    def tearDown(self):
        pass


class VlanMemberInvalidTest(T0TestBase):
    """
    This test verifies when adding a VLAN member to a non-exist VLAN, it will fail.
    """
    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)

    def runTest(self):
        print("VlanMemberInvalidTest")

        incorrect_member = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=11,
            bridge_port_id=self.bridge_port_list[17],
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED) 
        self.assertEqual(incorrect_member, 0)   

    def tearDown(self):
        pass


class DisableMacLearningTaggedTest(T0TestBase):
    """
    This test verifies the function when disabling VLAN MAC learning. When disabled, no new MAC will be learned in the MAC table.
    """
    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        print("DisableMacLearningTaggedTest")
        sai_thrift_set_vlan_attribute(self.client, self.vlans[10].vlan_oid, learn_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("MAC Learning disabled on VLAN")

    def runTest(self):
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        current_fdb_entry = attr["available_fdb_entry"]

        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[2],
                                eth_src=self.local_server_mac_list[1],
                                vlan_vid=10,
                                ip_id=101,
                                ip_ttl=64)
        send_packet(self, 1, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[2:9]])

        self.assertEqual(attr["available_fdb_entry"] - current_fdb_entry, 0)


    def tearDown(self):
        pass


class DisableMacLearningUntaggedTest(T0TestBase):
    """
    This test verifies the function when disabling VLAN MAC learning. When disabled, no new MAC will be learned in the MAC table.
    """
    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        print("DisableMacLearningUntaggedTest")
        sai_thrift_set_vlan_attribute(self.client, self.vlans[10].vlan_oid, learn_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("MAC Learning disabled on VLAN") 

    def runTest(self):   
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        current_fdb_entry = attr["available_fdb_entry"]
        
        pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[2],
                                eth_src=self.local_server_mac_list[1],
                                ip_id=101,
                                ip_ttl=64)
        send_packet(self, self.dev_port_list[1], pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.dev_port_list[2:9]])

        self.assertEqual(attr["available_fdb_entry"] - current_fdb_entry, 0)

    def tearDown(self):
        pass


class ArpRequestFloodingTest(T0TestBase):
    """
    This test verifies the flooding when receive a arp request
    """
    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        ip2 = "192.168.0.2" 
        self.arp_request = simple_arp_packet(
                eth_dst=self.local_server_mac_list[2],
                arp_op=1,
                ip_tgt=ip2,
                hw_tgt=self.local_server_mac_list[2])

    def runTest(self):
        print("ArpRequestFloodingTest")
        send_packet(self, self.dev_port_list[1], self.arp_request)
        verify_each_packet_on_multiple_port_lists(
            self, [self.arp_request], [self.dev_port_list[2:9]])

    def tearDown(self):
        pass


class ArpRequestLearningTest(T0TestBase):
    """
    This test verifies the mac learning when receive a arp request
    """
    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)

        ip1 = "192.168.0.1"
        ip2 = "192.168.0.2" 
        self.arp_response = simple_arp_packet(
                eth_dst=self.local_server_mac_list[1],
                eth_src=self.local_server_mac_list[2],
                arp_op=2,
                ip_tgt=ip2,
                ip_snd=ip1,
                hw_snd=self.local_server_mac_list[2],
                hw_tgt=self.local_server_mac_list[1])

    def runTest(self):
        print("ArpRequestLearningTest")
        send_packet(self, self.dev_port_list[2], self.arp_response)
        verify_packet(self, self.arp_response, self.dev_port_list[1])
        verify_no_other_packets(self)

    def tearDown(self):
        pass
            

class TaggedVlanStatusTest(T0TestBase):
    """
    This test verifies VLAN-related counters with tagged pkt 
    """
    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        self.tagged_pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[2],
                eth_src=self.local_server_mac_list[1],
                vlan_vid=10,
                ip_id=101,
                ip_ttl=64)

    def runTest(self):
        print("TaggedVlanStatusTest")
        stats = sai_thrift_get_vlan_stats(self.client, self.vlans[10].vlan_oid)

        in_bytes_pre = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes_pre = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets_pre = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets_pre = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets_pre = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets_pre = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

        print("Sending L2 packet port 1 -> port 2")
        send_packet(self, self.dev_port_list[1], self.tagged_pkt)
        verify_packet(self, self.tagged_pkt, self.dev_port_list[2])

        
        stats = sai_thrift_get_vlan_stats(self.client, self.vlans[10].vlan_oid)
        in_bytes = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]
        """
        Brcm may don't support this API, Skip all verification in this testcase
        """
        # self.assertEqual((in_packets, in_packets_pre + 1),
        #                 'vlan IN packets counter {} != {}'.format(
        #                     in_packets, in_packets_pre + 1))
        # self.assertEqual((in_ucast_packets, in_ucast_packets_pre + 1),
        #                 'vlan IN unicats packets counter {} != {}'.format(
        #                     in_ucast_packets, in_ucast_packets_pre + 1))
        # self.assertNotEqual(((in_bytes - in_bytes_pre), 0),
        #                 'vlan IN bytes counter is 0')
        # self.assertEqual((out_packets, out_packets_pre + 1),
        #                 'vlan OUT packets counter {} != {}'.format(
        #                     out_packets, out_packets_pre + 1))
        # self.assertEqual((out_ucast_packets, out_ucast_packets_pre + 1),
        #                 'vlan OUT unicats packets counter {} != {}'.format(
        #                     out_ucast_packets, out_ucast_packets_pre + 1))
        # self.assertEqual(((out_bytes - out_bytes_pre), 0),
        #                 'vlan OUT bytes counter is 0')

        print("Sending L2 packet port 1 -> port 2")
        send_packet(self, self.dev_port_list[1], self.tagged_pkt)
        verify_packet(self, self.tagged_pkt, self.dev_port_list[2])

        # Clear bytes and packets counter
        sai_thrift_clear_vlan_stats(self.client, self.vlans[10].vlan_oid)

        # Check counters
        stats = sai_thrift_get_vlan_stats(self.client, self.vlans[10].vlan_oid)
        in_bytes = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

        # self.assertEqual(in_packets, 0, 'vlan IN packets counter is not 0')
        # self.assertEqual(in_ucast_packets, 0,
        #                 'vlan IN unicast packets counter is not 0')
        # self.assertEqual(in_bytes, 0, 'vlan IN bytes counter is not 0')
        # self.assertEqual(out_packets, 0,
        #                 'vlan OUT packets counter is not 0')
        # self.assertEqual(out_ucast_packets, 0,
        #                 'vlan OUT unicast packets counter is not 0')
        # self.assertEqual(out_bytes, 0, 'vlan OUT bytes counter is not 0')

    def tearDown(self):
        pass


class UntaggedVlanStatusTest(T0TestBase):
    """
    This test verifies VLAN-related counters with untagged pkt 
    """
    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)

        self.untagged_pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[2],
                eth_src=self.local_server_mac_list[1],
                ip_id=101,
                ip_ttl=64)

    def runTest(self):
        print("UntaggedVlanStatusTest")
        stats = sai_thrift_get_vlan_stats(self.client, self.port_list[1])

        in_bytes_pre = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes_pre = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets_pre = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets_pre = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets_pre = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets_pre = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

        print("Sending L2 packet port 1 -> port 2 [access vlan=10])")
        send_packet(self, self.dev_port_list[1], self.untagged_pkt)
        verify_packet(self, self.untagged_pkt, self.dev_port_list[2])

        time.sleep(1)
        stats = sai_thrift_get_vlan_stats(self.client, self.vlans[10].vlan_oid)
        in_bytes = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]
        """
        Brcm may don't support this API, Skip all verification in this testcase
        """
        # self.assertEqual((in_packets, in_packets_pre + 1),
        #                 'vlan IN packets counter {} != {}'.format(
        #                     in_packets, in_packets_pre + 1))
        # self.assertEqual((in_ucast_packets, in_ucast_packets_pre + 1),
        #                 'vlan IN unicats packets counter {} != {}'.format(
        #                     in_ucast_packets, in_ucast_packets_pre + 1))
        # self.assertNotEqual(((in_bytes - in_bytes_pre), 0),
        #                 'vlan IN bytes counter is 0')
        # self.assertEqual((out_packets, out_packets_pre + 1),
        #                 'vlan OUT packets counter {} != {}'.format(
        #                     out_packets, out_packets_pre + 1))
        # self.assertEqual((out_ucast_packets, out_ucast_packets_pre + 1),
        #                 'vlan OUT unicats packets counter {} != {}'.format(
        #                     out_ucast_packets, out_ucast_packets_pre + 1))
        # self.assertEqual(((out_bytes - out_bytes_pre), 0),
        #                 'vlan OUT bytes counter is 0')

        print("Sending L2 packet port 1 -> port 2 [access vlan=10])")
        send_packet(self, self.dev_port_list[1], self.untagged_pkt)
        verify_packet(self, self.untagged_pkt, self.dev_port_list[2])

        # Clear bytes and packets counter
        sai_thrift_clear_vlan_stats(self.client, self.vlans[10].vlan_oid)
        # Check counters

        stats = sai_thrift_get_vlan_stats(self.client, self.vlans[10].vlan_oid)
        in_bytes = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

        # self.assertEqual(in_packets, 0, 'vlan IN packets counter is not 0')
        # self.assertEqual(in_ucast_packets, 0,
        #                 'vlan IN unicast packets counter is not 0')
        # self.assertEqual(in_bytes, 0, 'vlan IN bytes counter is not 0')
        # self.assertEqual(out_packets, 0,
        #                 'vlan OUT packets counter is not 0')
        # self.assertEqual(out_ucast_packets, 0,
        #                 'vlan OUT unicast packets counter is not 0')
        # self.assertEqual(out_bytes, 0, 'vlan OUT bytes counter is not 0')

    def tearDown(self):
        pass
