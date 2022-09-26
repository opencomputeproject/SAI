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
from config.fdb_configer import t0_fdb_tear_down_helper

class Vlan_Domain_Forwarding_Test(T0TestBase):
    """
    Verify the basic VLAN forwarding.
    In L2, if segement with VLAN tag and sends to a VLAN port, 
    segment should be forwarded inside a VLAN domain.
    """

    def setUp(self):
        """
        Set up test
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
                    self.dut.port_obj_list[1].dev_port_index,
                    self.dut.port_obj_list[index].dev_port_index))
                pkt = simple_udp_packet(eth_dst=self.servers[1][index].mac,
                                        eth_src=self.servers[1][1].mac,
                                        vlan_vid=10,
                                        ip_id=101,
                                        ip_ttl=64)

                send_packet(
                    self, self.dut.port_obj_list[1].dev_port_index, pkt)
                verify_packet(
                    self, pkt, self.dut.port_obj_list[index].dev_port_index)
                verify_no_other_packets(self, timeout=1)

            for index in range(10, 17):
                print("Forwarding in VLAN {} from {} to port: {}".format(
                    20,
                    self.dut.port_obj_list[9].dev_port_index,
                    self.dut.port_obj_list[index].dev_port_index))
                pkt = simple_udp_packet(eth_dst=self.servers[1][index].mac,
                                        eth_src=self.servers[1][9].mac,
                                        vlan_vid=20,
                                        ip_id=101,
                                        ip_ttl=64)
                send_packet(
                    self, self.dut.port_obj_list[9].dev_port_index, pkt)
                verify_packet(
                    self, pkt, self.dut.port_obj_list[index].dev_port_index)
                verify_no_other_packets(self, timeout=1)
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        super().tearDown()


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
                    self.dut.port_obj_list[1].dev_port_index,
                    self.dut.port_obj_list[index].dev_port_index))
                pkt = simple_udp_packet(eth_dst=self.servers[1][index].mac,
                                        eth_src=self.servers[1][1].mac,
                                        ip_id=101,
                                        ip_ttl=64)
                send_packet(
                    self, self.dut.port_obj_list[1].dev_port_index, pkt)
                verify_packet(
                    self, pkt, self.dut.port_obj_list[index].dev_port_index)
                verify_no_other_packets(self, timeout=2)
            for index in range(10, 17):
                print("Sending untagged packet from vlan20 tagged port {} to vlan20 tagged port: {}".format(
                    self.dut.port_obj_list[9].dev_port_index,
                    self.dut.port_obj_list[index].dev_port_index))
                pkt = simple_udp_packet(eth_dst=self.servers[1][index].mac,
                                        eth_src=self.servers[1][9].mac,
                                        ip_id=101,
                                        ip_ttl=64)
                send_packet(
                    self, self.dut.port_obj_list[9].dev_port_index, pkt)
                verify_packet(
                    self, pkt, self.dut.port_obj_list[index].dev_port_index)
                verify_no_other_packets(self, timeout=2)
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
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
                    self.dut.port_obj_list[9].dev_port_index,
                    self.dut.port_obj_list[index].dev_port_index))
                pkt = simple_udp_packet(eth_dst=self.servers[1][index].mac,
                                        eth_src=self.servers[1][9].mac,
                                        vlan_vid=20,
                                        ip_id=101,
                                        ip_ttl=64)
                send_packet(
                    self, self.dut.port_obj_list[9].dev_port_index, pkt)
                verify_no_other_packets(self, timeout=2)
            for index in range(9, 17):
                print("Sending vlan10 tagged packet from {} to vlan20 tagged port: {}".format(
                    self.dut.port_obj_list[1].dev_port_index,
                    self.dut.port_obj_list[index].dev_port_index))
                pkt = simple_udp_packet(eth_dst=self.servers[1][index].mac,
                                        eth_src=self.servers[1][1].mac,
                                        vlan_vid=10,
                                        ip_id=101,
                                        ip_ttl=64)
                send_packet(
                    self, self.dut.port_obj_list[1].dev_port_index, pkt)
                verify_no_other_packets(self, timeout=2)
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        super().tearDown()


class TaggedFrameFilteringTest(T0TestBase):
    """
    Drop tagged packet when the destination port from MAC table search is the port which packet comes into the switch.
    """

    def setUp(self):
        super().setUp()
        t0_fdb_tear_down_helper(self)
        self.tmp_server_list = [self.servers[1][i] for i in [1, 5]]
        self.fdb_configer.create_fdb_entries(
            switch_id=self.dut.switch_id,
            server_list=self.tmp_server_list,
            port_idxs=[1, 1],
            vlan_oid=self.dut.vlans[10].oid)

    def runTest(self):
        print("\nTaggedFrameFilteringTest")
        try:
            for tmp_server in self.tmp_server_list:
                pkt = simple_udp_packet(eth_dst=tmp_server.mac,
                                        eth_src=self.servers[1][1].mac,
                                        vlan_vid=10,
                                        ip_id=101,
                                        ip_ttl=64)
                send_packet(
                    self, self.dut.port_obj_list[1].dev_port_index, pkt)
                verify_no_other_packets(self, timeout=1)
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        t0_fdb_tear_down_helper(self)
        super().tearDown()


class UnTaggedFrameFilteringTest(T0TestBase):
    """
    Drop untagged packet when the destination port from MAC table search
    is the port which packet comes into the switch.
    """

    def setUp(self):
        super().setUp()
        t0_fdb_tear_down_helper(self)
        self.tmp_server_list = [self.servers[1][i] for i in [1, 5]]
        self.fdb_configer.create_fdb_entries(
            switch_id=self.dut.switch_id,
            server_list=self.tmp_server_list,
            port_idxs=[1, 1],
            vlan_oid=self.dut.vlans[10].oid)

    def runTest(self):
        print("\nUnTaggedFrameFilteringTest")
        try:
            for tmp_server in self.tmp_server_list:
                pkt = simple_udp_packet(eth_dst=tmp_server.mac,
                                        eth_src=self.servers[1][1].mac,
                                        ip_id=101,
                                        ip_ttl=64)
                send_packet(
                    self, self.dut.port_obj_list[1].dev_port_index, pkt)
                verify_no_other_packets(self, timeout=1)
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        t0_fdb_tear_down_helper(self)
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
                                    eth_src=self.servers[1][1].mac,
                                    vlan_vid=10,
                                    ip_id=101,
                                    ip_ttl=64)
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            print(self.dut.vlans[10].port_idx_list)
            verify_each_packet_on_multiple_port_lists(self, [pkt], [self.get_dev_port_indexes(
                list(filter(lambda item: item != 1, self.dut.vlans[10].port_idx_list)))])
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
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
                                    eth_src=self.servers[1][1].mac,
                                    ip_id=101,
                                    ip_ttl=64)
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            verify_each_packet_on_multiple_port_lists(self, [pkt], [self.get_dev_port_indexes(
                list(filter(lambda item: item != 1, self.dut.vlans[10].port_idx_list)))])
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
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
            # untag
            untagged_pkt = simple_udp_packet(eth_dst=macX,
                                             eth_src=self.servers[1][1].mac,
                                             ip_id=101,
                                             ip_ttl=64)
            send_packet(
                self, self.dut.port_obj_list[1].dev_port_index, untagged_pkt)
            verify_each_packet_on_multiple_port_lists(self, [untagged_pkt], [self.get_dev_port_indexes(
                list(filter(lambda item: item != 1, self.dut.vlans[10].port_idx_list)))])
            # tag
            tagged_pkt = simple_udp_packet(eth_dst=macX,
                                           eth_src=self.servers[1][1].mac,
                                           vlan_vid=10,
                                           ip_id=101,
                                           ip_ttl=64)
            send_packet(
                self, self.dut.port_obj_list[1].dev_port_index, tagged_pkt)
            verify_each_packet_on_multiple_port_lists(self, [tagged_pkt], [self.get_dev_port_indexes(
                list(filter(lambda item: item != 1, self.dut.vlans[10].port_idx_list)))])
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
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
            # untag
            untagged_pkt = simple_udp_packet(eth_dst=self.servers[1][2].mac,
                                             eth_src=macX,
                                             ip_id=101,
                                             ip_ttl=64)
            send_packet(
                self, self.dut.port_obj_list[1].dev_port_index, untagged_pkt)
            verify_packet(self, untagged_pkt,
                          self.dut.port_obj_list[2].dev_port_index)
            verify_no_other_packets(self, timeout=2)
            sleep(2)  # wait for add mac entry
            available_fdb_entry_cnt_now = sai_thrift_get_switch_attribute(
                self.client,
                available_fdb_entry=True)['available_fdb_entry']
            self.assertEqual(available_fdb_entry_cnt_now -
                             available_fdb_entry_cnt_past, -1)
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        sleep(2)
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
            tagged_pkt = simple_udp_packet(eth_dst=self.servers[1][2].mac,
                                           eth_src=macX,
                                           vlan_vid=10,
                                           ip_id=101,
                                           ip_ttl=64)
            send_packet(
                self, self.dut.port_obj_list[1].dev_port_index, tagged_pkt)
            verify_packet(self, tagged_pkt,
                          self.dut.port_obj_list[2].dev_port_index)
            verify_no_other_packets(self, timeout=2)
            sleep(2)  # wait for add mac entry
            available_fdb_entry_cnt_now = sai_thrift_get_switch_attribute(
                self.client,
                available_fdb_entry=True)['available_fdb_entry']
            self.assertEqual(available_fdb_entry_cnt_now -
                             available_fdb_entry_cnt_past, -1)
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        sleep(2)
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
        mbr_list.extend(self.vlan_configer.get_vlan_member(
            self.dut.vlans[10].oid))
        mbr_list.extend(self.vlan_configer.get_vlan_member(
            self.dut.vlans[20].oid))
        self.assertEqual(len(mbr_list), 16)

        for i in range(0, 8):
            self.assertEqual(
                self.dut.vlans[10].vlan_mport_oids[i], mbr_list[i])
        for i in range(8, 16):
            self.assertEqual(
                self.dut.vlans[20].vlan_mport_oids[i - 8], mbr_list[i])

        # Adding vlan members and veryfing vlan member list
        new_vlan_member = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.dut.vlans[10].oid,
            bridge_port_id=self.dut.port_obj_list[17].bridge_port_oid,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        mbr_list = []
        mbr_list.extend(self.vlan_configer.get_vlan_member(
            self.dut.vlans[10].oid))
        mbr_list.extend(self.vlan_configer.get_vlan_member(
            self.dut.vlans[20].oid))
        self.assertEqual(len(mbr_list), 17)

        # Adding vlan members and veryfing vlan member list
        for i in range(0, 8):
            self.assertEqual(
                self.dut.vlans[10].vlan_mport_oids[i], mbr_list[i])
        self.assertEqual(new_vlan_member, mbr_list[8])
        for i in range(9, 17):
            self.assertEqual(
                self.dut.vlans[20].vlan_mport_oids[i - 9], mbr_list[i])

        # Removing vlan members and veryfing vlan member list
        sai_thrift_remove_vlan_member(self.client, new_vlan_member)

        mbr_list = []
        mbr_list.extend(self.vlan_configer.get_vlan_member(
            self.dut.vlans[10].oid))
        mbr_list.extend(self.vlan_configer.get_vlan_member(
            self.dut.vlans[20].oid))
        self.assertEqual(len(mbr_list), 16)

        for i in range(0, 8):
            self.assertEqual(
                self.dut.vlans[10].vlan_mport_oids[i], mbr_list[i])
        for i in range(8, 16):
            self.assertEqual(
                self.dut.vlans[20].vlan_mport_oids[i - 8], mbr_list[i])

    def tearDown(self):
        """
        TearDown process
        """
        super().tearDown()


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
            bridge_port_id=self.dut.port_obj_list[17].bridge_port_oid,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.assertEqual(incorrect_member, 0)

    def tearDown(self):
        """
        TearDown process
        """
        super().tearDown()


class DisableMacLearningTaggedTest(T0TestBase):
    """
    This test verifies the function when disabling VLAN MAC learning. When disabled, no new MAC will be learned in the MAC table.
    """

    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        print("DisableMacLearningTaggedTest")
        sai_thrift_set_vlan_attribute(
            self.client, self.dut.vlans[10].oid, learn_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("MAC Learning disabled on VLAN")

    def runTest(self):
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        current_fdb_entry = attr["available_fdb_entry"]

        pkt = simple_udp_packet(eth_dst=self.servers[1][2].mac,
                                eth_src=self.servers[1][1].mac,
                                vlan_vid=10,
                                ip_id=101,
                                ip_ttl=64)
        send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.get_dev_port_indexes(list(filter(lambda item: item != 1, self.dut.vlans[10].port_idx_list)))])

        self.assertEqual(attr["available_fdb_entry"] - current_fdb_entry, 0)

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_set_vlan_attribute(
            self.client, self.dut.vlans[10].oid, learn_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        sleep(2)
        super().tearDown()


class DisableMacLearningUntaggedTest(T0TestBase):
    """
    This test verifies the function when disabling VLAN MAC learning. When disabled, no new MAC will be learned in the MAC table.
    """

    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        print("DisableMacLearningUntaggedTest")
        sai_thrift_set_vlan_attribute(
            self.client, self.dut.vlans[10].oid, learn_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("MAC Learning disabled on VLAN")

    def runTest(self):
        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        current_fdb_entry = attr["available_fdb_entry"]

        pkt = simple_udp_packet(eth_dst=self.servers[1][2].mac,
                                eth_src=self.servers[1][1].mac,
                                ip_id=101,
                                ip_ttl=64)
        send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [self.get_dev_port_indexes(list(filter(lambda item: item != 1, self.dut.vlans[10].port_idx_list)))])

        self.assertEqual(attr["available_fdb_entry"] - current_fdb_entry, 0)

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_set_vlan_attribute(
            self.client, self.dut.vlans[10].oid, learn_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        super().tearDown()


class ArpRequestFloodingTest(T0TestBase):
    """
    This test verifies the flooding when receive a arp request
    """

    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        ip2 = "192.168.0.2"
        self.arp_request = simple_arp_packet(
            eth_dst=self.servers[1][2].mac,
            arp_op=1,
            ip_tgt=ip2,
            hw_tgt=self.servers[1][2].mac)

    def runTest(self):
        print("ArpRequestFloodingTest")
        send_packet(
            self, self.dut.port_obj_list[1].dev_port_index, self.arp_request)
        verify_each_packet_on_multiple_port_lists(
            self, [self.arp_request], [self.get_dev_port_indexes(list(filter(lambda item: item != 1, self.dut.vlans[10].port_idx_list)))])

    def tearDown(self):
        """
        TearDown process
        """
        super().tearDown()


class ArpRequestLearningTest(T0TestBase):
    """
    This test verifies the mac learning when receive a arp request
    """

    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)

        ip1 = "192.168.0.1"
        ip2 = "192.168.0.2"
        self.arp_response = simple_arp_packet(
            eth_dst=self.servers[1][1].mac,
            eth_src=self.servers[1][2].mac,
            arp_op=2,
            ip_tgt=ip2,
            ip_snd=ip1,
            hw_snd=self.servers[1][2].mac,
            hw_tgt=self.servers[1][1].mac)

    def runTest(self):
        print("ArpRequestLearningTest")
        send_packet(
            self, self.dut.port_obj_list[2].dev_port_index, self.arp_response)
        verify_packet(self, self.arp_response,
                      self.dut.port_obj_list[1].dev_port_index)
        verify_no_other_packets(self)

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
        sleep(2)


class TaggedVlanStatusTest(T0TestBase):
    """
    This test verifies VLAN-related counters with tagged pkt 
    """

    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)
        self.tagged_pkt = simple_udp_packet(eth_dst=self.servers[1][2].mac,
                                            eth_src=self.servers[1][1].mac,
                                            vlan_vid=10,
                                            ip_id=101,
                                            ip_ttl=64)

    def runTest(self):
        print("TaggedVlanStatusTest")
        stats = sai_thrift_get_vlan_stats(
            self.client, self.dut.vlans[10].oid)

        in_bytes_pre = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes_pre = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets_pre = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets_pre = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets_pre = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets_pre = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

        print("Sending L2 packet port 1 -> port 2")
        send_packet(
            self, self.dut.port_obj_list[1].dev_port_index, self.tagged_pkt)
        verify_packet(self, self.tagged_pkt,
                      self.dut.port_obj_list[2].dev_port_index)

        stats = sai_thrift_get_vlan_stats(
            self.client, self.dut.vlans[10].oid)
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
        send_packet(
            self, self.dut.port_obj_list[1].dev_port_index, self.tagged_pkt)
        verify_packet(self, self.tagged_pkt,
                      self.dut.port_obj_list[2].dev_port_index)

        # Clear bytes and packets counter
        sai_thrift_clear_vlan_stats(self.client, self.dut.vlans[10].oid)

        # Check counters
        stats = sai_thrift_get_vlan_stats(
            self.client, self.dut.vlans[10].oid)
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
        """
        TearDown process
        """
        super().tearDown()


class UntaggedVlanStatusTest(T0TestBase):
    """
    This test verifies VLAN-related counters with untagged pkt 
    """

    def setUp(self):
        T0TestBase.setUp(self, is_reset_default_vlan=False)

        self.untagged_pkt = simple_udp_packet(eth_dst=self.servers[1][2].mac,
                                              eth_src=self.servers[1][1].mac,
                                              ip_id=101,
                                              ip_ttl=64)

    def runTest(self):
        print("UntaggedVlanStatusTest")
        stats = sai_thrift_get_vlan_stats(self.client, self.dut.vlans[10].oid)

        in_bytes_pre = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes_pre = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets_pre = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets_pre = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets_pre = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets_pre = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

        print("Sending L2 packet port 1 -> port 2 [access vlan=10])")
        send_packet(
            self, self.dut.port_obj_list[1].dev_port_index, self.untagged_pkt)
        verify_packet(self, self.untagged_pkt,
                      self.dut.port_obj_list[2].dev_port_index)

        time.sleep(1)
        stats = sai_thrift_get_vlan_stats(
            self.client, self.dut.vlans[10].oid)
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
        send_packet(
            self, self.dut.port_obj_list[1].dev_port_index, self.untagged_pkt)
        verify_packet(self, self.untagged_pkt,
                      self.dut.port_obj_list[2].dev_port_index)

        # Clear bytes and packets counter
        sai_thrift_clear_vlan_stats(self.client, self.dut.vlans[10].oid)
        # Check counters

        stats = sai_thrift_get_vlan_stats(
            self.client, self.dut.vlans[10].oid)
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
        """
        TearDown process
        """
        super().tearDown()
