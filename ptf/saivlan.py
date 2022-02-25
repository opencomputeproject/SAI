#  Copyright 2021-present Intel Corporation.
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
Thrift SAI interface VLAN tests
"""

from sai_thrift.sai_headers import *

from ptf.dataplane import DataPlane

from sai_base_test import *


def set_vlan_data(vlan_id=0, ports=None, untagged=None, large_port=0):
    """
    Creates dictionary with vlan data

    Args:
        vlan_id (int): VLAN ID number
        ports (list): ports numbers
        untagged (list): list of untagged ports
        large_port (int): the largest port in vlan

    Return:
        dictionary: vlan_data_dict
    """
    vlan_data_dict = {
        "vlan_id": vlan_id,
        "ports": ports,
        "untagged": untagged,
        "large_port": large_port
    }
    return vlan_data_dict


@group("draft")
class L2VlanTest(SaiHelper):
    """
    The class runs VLAN test cases
    """

    def setUp(self):
        super(L2VlanTest, self).setUp()

        self.pkt = 0
        self.tagged_pkt = 0
        self.arp_resp = 0
        self.tagged_arp_resp = 0
        self.i_pkt_count = 0
        self.e_pkt_count = 0
        self.mac0 = '00:11:11:11:11:11:11'
        self.mac1 = '00:22:22:22:22:22:22'
        self.mac2 = '00:33:33:33:33:33:33'
        self.mac3 = '00:44:44:44:44:44:44'
        mac_action = SAI_PACKET_ACTION_FORWARD

        self.port24_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.port25_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port25,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.port26_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port26,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.port27_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port27,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.port28_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port28,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.port29_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port29,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.port30_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port30,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.port31_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port31,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        self.vlan10_member3 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan10_member4 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.port25_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        sai_thrift_set_port_attribute(
            self.client, self.port24, port_vlan_id=10)

        self.fdb_entry0 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id, mac_address=self.mac0, bv_id=self.vlan10)
        sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry0,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port0_bp,
            packet_action=mac_action)
        self.fdb_entry1 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id, mac_address=self.mac1, bv_id=self.vlan10)
        sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry1,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port1_bp,
            packet_action=mac_action)
        self.fdb_entry24 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id, mac_address=self.mac2, bv_id=self.vlan10)
        sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry24,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port24_bp,
            packet_action=mac_action)
        self.fdb_entry25 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id, mac_address=self.mac3, bv_id=self.vlan10)
        sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry25,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port25_bp,
            packet_action=mac_action)

        self.lag10 = sai_thrift_create_lag(self.client)
        self.lag_mbr31 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag10, port_id=self.port28)

        self.lag11 = sai_thrift_create_lag(self.client)
        self.lag_mbr41 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag11, port_id=self.port29)

        self.lag10_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag10,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.lag11_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag11,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        # vlan40
        self.vlan40 = sai_thrift_create_vlan(self.client, vlan_id=40)
        self.vlan_member41 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan40,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_member42 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan40,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan_member43 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan40,
            bridge_port_id=self.lag10_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan_member44 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan40,
            bridge_port_id=self.lag11_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        # vlan50
        self.vlan50 = sai_thrift_create_vlan(self.client, vlan_id=50)
        self.vlan_member51 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan50,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan_member52 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan50,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_member53 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan50,
            bridge_port_id=self.lag10_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan_member54 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan50,
            bridge_port_id=self.lag11_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        # vlan60
        self.vlan60 = sai_thrift_create_vlan(self.client, vlan_id=60)
        self.vlan_member61 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan60,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan_member62 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan60,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan_member63 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan60,
            bridge_port_id=self.lag10_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_member64 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan60,
            bridge_port_id=self.lag11_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        # vlan70
        self.vlan70 = sai_thrift_create_vlan(self.client, vlan_id=70)
        self.vlan_member71 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan70,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan_member72 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan70,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan_member73 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan70,
            bridge_port_id=self.lag10_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan_member74 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan70,
            bridge_port_id=self.lag11_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

    def runTest(self):
        self.forwardingTest()
        self.nativeVlanTest()
        self.priorityTaggingTest()
        self.pvDropTest()
        self.lagPVMissTest()
        self.vlanFloodTest()
        self.vlanFloodEnhancedTest()
        self.vlanFloodDisableTest()
        self.vlanStatsTest()
        self.vlanFloodPruneTest()
        self.countersClearTest()
        self.vlanMemberList()
        self.vlanNegativeTest()
        self.singleVlanMemberTest()
        self.vlanIngressAclTest()
        self.vlanEgressAclTest()
        self.vlanLearningTest()
        self.vlanMaxLearnedAddressesTest()

    def tearDown(self):
        sai_thrift_set_port_attribute(self.client, self.port24, port_vlan_id=1)

        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry0)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry1)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry24)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry25)

        sai_thrift_remove_vlan_member(self.client, self.vlan10_member3)
        sai_thrift_remove_vlan_member(self.client, self.vlan10_member4)

        sai_thrift_remove_vlan_member(self.client, self.vlan_member41)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member42)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member43)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member44)

        sai_thrift_remove_vlan_member(self.client, self.vlan_member51)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member52)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member53)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member54)

        sai_thrift_remove_vlan_member(self.client, self.vlan_member61)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member62)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member63)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member64)

        sai_thrift_remove_vlan_member(self.client, self.vlan_member71)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member72)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member73)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member74)

        sai_thrift_remove_lag_member(self.client, self.lag_mbr31)
        sai_thrift_remove_lag_member(self.client, self.lag_mbr41)

        sai_thrift_remove_bridge_port(self.client, self.lag10_bp)
        sai_thrift_remove_bridge_port(self.client, self.lag11_bp)
        sai_thrift_remove_lag(self.client, self.lag10)
        sai_thrift_remove_lag(self.client, self.lag11)

        sai_thrift_remove_vlan(self.client, self.vlan40)
        sai_thrift_remove_vlan(self.client, self.vlan50)
        sai_thrift_remove_vlan(self.client, self.vlan60)
        sai_thrift_remove_vlan(self.client, self.vlan70)

        sai_thrift_remove_bridge_port(self.client, self.port24_bp)
        sai_thrift_remove_bridge_port(self.client, self.port25_bp)
        sai_thrift_remove_bridge_port(self.client, self.port26_bp)
        sai_thrift_remove_bridge_port(self.client, self.port27_bp)
        sai_thrift_remove_bridge_port(self.client, self.port28_bp)
        sai_thrift_remove_bridge_port(self.client, self.port29_bp)
        super(L2VlanTest, self).tearDown()

    def forwardingTest(self):
        """
        Forwarding between ports with different tagging mode
        """
        print("\nforwardingTest()")
        print("\tAccessToAccessTest")
        print("Sending L2 packet port 0 -> port 24 [access vlan=10])")
        pkt = simple_tcp_packet(eth_dst='00:33:33:33:33:33',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='172.16.0.1',
                                ip_id=101,
                                ip_ttl=64)

        send_packet(self, self.dev_port0, pkt)
        verify_packet(self, pkt, self.dev_port24)
        self.i_pkt_count += 1
        self.e_pkt_count += 1

        print("\tAccessToTrunkTest")
        print("Sending L2 packet - port 0 -> port 1 [trunk vlan=10])")
        pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='172.16.0.1',
                                ip_id=102,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                    eth_src='00:11:11:11:11:11',
                                    ip_dst='172.16.0.1',
                                    dl_vlan_enable=True,
                                    vlan_vid=10,
                                    ip_id=102,
                                    ip_ttl=64,
                                    pktlen=104)

        send_packet(self, self.dev_port0, pkt)
        verify_packet(self, exp_pkt, self.dev_port1)
        self.i_pkt_count += 1
        self.e_pkt_count += 1

        print("\tTrunkToTrunkTest")
        print("Sending L2 packet - port 1 -> port 25 [trunk vlan=10])")
        pkt = simple_tcp_packet(eth_dst='00:44:44:44:44:44',
                                eth_src='00:22:22:22:22:22',
                                dl_vlan_enable=True,
                                vlan_vid=10,
                                ip_dst='172.16.0.1',
                                ip_id=102,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:44:44:44:44:44',
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='172.16.0.1',
                                    ip_id=102,
                                    dl_vlan_enable=True,
                                    vlan_vid=10,
                                    ip_ttl=64)

        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port25)
        self.i_pkt_count += 1
        self.e_pkt_count += 1

        print("\tTrunkToAccessTest")
        print("Sending L2 packet - port 1 -> port 0 [trunk vlan=10])")
        pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                eth_src='00:22:22:22:22:22',
                                dl_vlan_enable=True,
                                vlan_vid=10,
                                ip_dst='172.16.0.1',
                                ip_id=102,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='172.16.0.1',
                                    ip_id=102,
                                    ip_ttl=64,
                                    pktlen=96)

        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port0)
        self.i_pkt_count += 1
        self.e_pkt_count += 1

    def nativeVlanTest(self):
        """
        Verifies forwarding of native vlan on tagged port
        and on tagged LAG
        """
        print("\nnativeVlanTest()")
        try:
            print("Configure vlan 10 with access/trunk members")
            tag_pkt = simple_udp_packet(eth_dst='00:44:44:44:44:44',
                                        eth_src='00:22:22:22:22:22',
                                        dl_vlan_enable=True,
                                        vlan_vid=10,
                                        ip_ttl=64,
                                        pktlen=104)
            untag_pkt = simple_udp_packet(eth_dst='00:44:44:44:44:44',
                                          eth_src='00:22:22:22:22:22',
                                          ip_ttl=64,
                                          pktlen=100)

            print("Tx tag packet on trunk port %d -> trunk port %d" % (
                self.dev_port1, self.dev_port25))
            send_packet(self, self.dev_port1, tag_pkt)
            verify_packet(self, tag_pkt, self.dev_port25)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            tag_pkt1 = simple_udp_packet(eth_dst='00:11:11:11:11:11',
                                         eth_src='00:33:33:33:33:33',
                                         dl_vlan_enable=True,
                                         vlan_vid=10,
                                         ip_ttl=64,
                                         pktlen=104)
            untag_pkt1 = simple_udp_packet(eth_dst='00:11:11:11:11:11',
                                           eth_src='00:33:33:33:33:33',
                                           ip_ttl=64,
                                           pktlen=100)

            print("Tx tag packet on trunk port %d -> access port %d" % (
                self.dev_port1, self.dev_port0))
            send_packet(self, self.dev_port1, tag_pkt1)
            verify_packet(self, untag_pkt1, self.dev_port0)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            print("Tx untag packet on trunk port %d, "
                  "drop because no native vlan is set" % (
                      self.dev_port1))
            send_packet(self, self.dev_port1, untag_pkt)
            verify_no_other_packets(self, timeout=1)

            tag_pkt_40 = simple_udp_packet(eth_dst='00:44:44:44:44:44',
                                           eth_src='00:33:33:33:33:33',
                                           dl_vlan_enable=True,
                                           vlan_vid=40,
                                           ip_ttl=64,
                                           pktlen=104)
            print("Tx incorrect tag [i.e. vlan 40] packet on trunk port %d,"
                  "dropped" % (self.dev_port1))
            send_packet(self, self.dev_port1, tag_pkt_40)
            verify_no_other_packets(self, timeout=1)

            # Enable port's native vlan as vlan 10, which is same as port's
            # trunk vlan
            sai_thrift_set_port_attribute(
                self.client, self.port1, port_vlan_id=10)

            print("Tx packet on trunk port %d -> trunk port %d" % (
                self.dev_port1, self.dev_port25))
            send_packet(self, self.dev_port1, tag_pkt)
            verify_packet(self, tag_pkt, self.dev_port25)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            print("Tx packet on trunk port %d -> access port %d" % (
                self.dev_port1, self.dev_port0))
            send_packet(self, self.dev_port1, tag_pkt1)
            verify_packet(self, untag_pkt1, self.dev_port0)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            print("Tx packet on trunk port %d -> trunk port %d" % (
                self.dev_port1, self.dev_port25))
            send_packet(self, self.dev_port1, untag_pkt)
            verify_packet(self, tag_pkt, self.dev_port25)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            print("Tx packet on trunk port %d -> access port %d" % (
                self.dev_port1, self.dev_port0))
            send_packet(self, self.dev_port1, untag_pkt1)
            verify_packet(self, untag_pkt1, self.dev_port0)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            print("Tx incorrect tag [i.e. vlan 40] packet on trunk port %d, "
                  "dropped" % (self.dev_port1))
            send_packet(self, self.dev_port1, tag_pkt_40)
            verify_no_other_packets(self, timeout=1)

            sai_thrift_set_vlan_attribute(
                self.client, self.vlan20, learn_disable=True)
            sai_thrift_set_port_attribute(
                self.client, self.port2, port_vlan_id=20)
            sai_thrift_set_port_attribute(
                self.client, self.port1, port_vlan_id=20)

            tag_pkt_20 = simple_udp_packet(eth_dst='00:44:44:44:44:44',
                                           eth_src='00:22:22:22:22:22',
                                           dl_vlan_enable=True,
                                           vlan_vid=20,
                                           ip_ttl=64,
                                           pktlen=104)
            lag1_ports = [self.dev_port7, self.dev_port8, self.dev_port9]
            print("Tx untag packet on trunk port 2 -> "
                  "Flood to all members of vlan 20 i.e. lag2, port2, port3")
            send_packet(self, self.dev_port1, untag_pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [tag_pkt_20, untag_pkt, tag_pkt_20],
                [lag1_ports, [self.dev_port2], [self.dev_port3]])

            mac = '00:55:55:55:55:55'
            mac_action = SAI_PACKET_ACTION_FORWARD

            fdb_entry = sai_thrift_fdb_entry_t(
                switch_id=self.switch_id, mac_address=mac, bv_id=self.vlan20)
            sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=self.lag2_bp,
                packet_action=mac_action)

            sai_thrift_set_lag_attribute(
                self.client, self.lag2, port_vlan_id=20)

            tag_pkt_lag = simple_udp_packet(eth_dst='00:44:44:44:44:44',
                                            eth_src='00:55:55:55:55:55',
                                            dl_vlan_enable=True,
                                            vlan_vid=20,
                                            ip_ttl=64,
                                            pktlen=104)

            untag_pkt_lag = simple_udp_packet(eth_dst='00:44:44:44:44:44',
                                              eth_src='00:55:55:55:55:55',
                                              ip_ttl=64,
                                              pktlen=100)

            print("Tx untag packet on trunk lag2 -> "
                  "Flood to all members of vlan 20 i.e. lag2, port2, port3")
            send_packet(self, self.dev_port7, untag_pkt_lag)
            verify_each_packet_on_multiple_port_lists(
                self, [untag_pkt_lag, tag_pkt_lag],
                [[self.dev_port2], [self.dev_port3]])

            sai_thrift_set_lag_attribute(
                self.client, self.lag2, port_vlan_id=1)

            print("Tx tag [vlan 10] packet on trunk port %d -> "
                  "trunk port %d of vlan 10" % (
                      self.dev_port1, self.dev_port25))
            send_packet(self, self.dev_port1, tag_pkt)
            verify_packet(self, tag_pkt, self.dev_port25)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            print("Update native vlan of trunk port %d - reset to one" % (
                self.dev_port1))
            sai_thrift_set_port_attribute(
                self.client, self.port1, port_vlan_id=1)

            print("Tx untag packet on trunk port 1, "
                  "drop because no native vlan is set")
            send_packet(self, self.dev_port1, untag_pkt)
            verify_no_other_packets(self, timeout=1)

            print("Tx tag [vlan 10] packet on trunk port %d -> "
                  "trunk port %d of vlan 10" % (
                      self.dev_port1, self.dev_port25))
            send_packet(self, self.dev_port1, tag_pkt)
            verify_packet(self, tag_pkt, self.dev_port25)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

        finally:
            sai_thrift_set_port_attribute(
                self.client, self.port1, port_vlan_id=1)

            sai_thrift_remove_fdb_entry(self.client, fdb_entry)

            sai_thrift_set_vlan_attribute(
                self.client, self.vlan20, learn_disable=False)

    def priorityTaggingTest(self):
        """
        Verifies forwarding of priority tagged packets on port and LAG
        """
        print("\npriorityTaggingTest()")
        mac5 = '00:55:55:55:55:55'
        mac6 = '00:66:66:66:66:66'
        mac7 = '00:77:77:77:77:77'
        mac8 = '00:88:88:88:88:88'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan20,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan20,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        fdb_entry1 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=mac5,
            bv_id=self.vlan10)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry1,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.lag1_bp,
            packet_action=mac_action)
        fdb_entry2 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=mac6,
            bv_id=self.vlan20)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry2,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.lag2_bp,
            packet_action=mac_action)
        fdb_entry3 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=mac7,
            bv_id=self.vlan20)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry3,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port26_bp,
            packet_action=mac_action)
        fdb_entry4 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=mac8,
            bv_id=self.vlan20)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry4,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port27_bp,
            packet_action=mac_action)

        try:
            # access port testing
            pkt = simple_udp_packet(eth_dst='00:33:33:33:33:33',
                                    eth_src='00:11:11:11:11:11',
                                    dl_vlan_enable=True,
                                    vlan_vid=0,
                                    ip_ttl=64,
                                    pktlen=104)
            exp_pkt = simple_udp_packet(eth_dst='00:33:33:33:33:33',
                                        eth_src='00:11:11:11:11:11',
                                        ip_ttl=64,
                                        pktlen=100)
            print("Sending priority tagged packet on access port %d -> "
                  "access port %d" % (self.dev_port0, self.dev_port24))
            send_packet(self, self.dev_port0, pkt)
            verify_packet(self, exp_pkt, self.dev_port24)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            pkt = simple_udp_packet(eth_dst='00:22:22:22:22:22',
                                    eth_src='00:11:11:11:11:11',
                                    dl_vlan_enable=True,
                                    vlan_vid=0,
                                    ip_ttl=64)
            exp_pkt = simple_udp_packet(eth_dst='00:22:22:22:22:22',
                                        eth_src='00:11:11:11:11:11',
                                        dl_vlan_enable=True,
                                        vlan_vid=10,
                                        ip_ttl=64)
            print("Sending priority tagged packet on access port %d -> "
                  "trunk port %d" % (self.dev_port0, self.dev_port1))
            send_packet(self, self.dev_port0, pkt)
            verify_packet(self, exp_pkt, self.dev_port1)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            # access port testing LAG
            pkt = simple_udp_packet(eth_dst='00:33:33:33:33:33',
                                    eth_src='00:55:55:55:55:55',
                                    dl_vlan_enable=True,
                                    vlan_vid=0,
                                    ip_ttl=64,
                                    pktlen=104)
            exp_pkt = simple_udp_packet(eth_dst='00:33:33:33:33:33',
                                        eth_src='00:55:55:55:55:55',
                                        ip_ttl=64,
                                        pktlen=100)
            print("Sending priority tagged packet on access lag1 -> "
                  "access port %d" % (self.dev_port24))
            send_packet(self, self.dev_port4, pkt)
            verify_packet(self, exp_pkt, self.dev_port24)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            pkt = simple_udp_packet(eth_dst='00:22:22:22:22:22',
                                    eth_src='00:55:55:55:55:55',
                                    dl_vlan_enable=True,
                                    vlan_vid=0,
                                    ip_ttl=64)
            exp_pkt = simple_udp_packet(eth_dst='00:22:22:22:22:22',
                                        eth_src='00:55:55:55:55:55',
                                        dl_vlan_enable=True,
                                        vlan_vid=10,
                                        ip_ttl=64)
            print("Sending priority tagged packet on access lag1 -> "
                  "trunk port %d" % (self.dev_port1))
            send_packet(self, self.dev_port4, pkt)
            verify_packet(self, exp_pkt, self.dev_port1)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            # trunk port testing
            pkt = simple_udp_packet(eth_dst='00:44:44:44:44:44',
                                    eth_src='00:22:22:22:22:22',
                                    dl_vlan_enable=True,
                                    vlan_vid=0,
                                    ip_ttl=64)
            print("Sending priority tagged packet on trunk port %d to trunk, "
                  "dropped" % (self.dev_port1))
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)

            pkt = simple_udp_packet(eth_dst='00:11:11:11:11:11',
                                    eth_src='00:22:22:22:22:22',
                                    dl_vlan_enable=True,
                                    vlan_vid=0,
                                    ip_ttl=64)
            print("Sending priority tagged packet on trunk port %d to access, "
                  "dropped" % (self.dev_port1))
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)

            # trunk port testing LAG
            pkt = simple_udp_packet(eth_dst='00:44:44:44:44:44',
                                    eth_src='00:66:66:66:66:66',
                                    dl_vlan_enable=True,
                                    vlan_vid=0,
                                    ip_ttl=64)
            print("Sending priority tagged packet on trunk lag2 to trunk, "
                  "dropped")
            send_packet(self, self.dev_port7, pkt)
            verify_no_other_packets(self, timeout=1)

            pkt = simple_udp_packet(eth_dst='00:11:11:11:11:11',
                                    eth_src='00:66:66:66:66:66',
                                    dl_vlan_enable=True,
                                    vlan_vid=0,
                                    ip_ttl=64)
            print("Sending priority tagged packet on trunk lag2 to access, "
                  "dropped")
            send_packet(self, self.dev_port7, pkt)
            verify_no_other_packets(self, timeout=1)

            print("Update native vlan of trunk lag2 with vlan 20")
            sai_thrift_set_lag_attribute(
                self.client, self.lag2, port_vlan_id=20)

            print("Update native vlan of trunk port %d with vlan 10" % (
                self.dev_port1))
            sai_thrift_set_port_attribute(
                self.client, self.port1, port_vlan_id=10)

            pkt = simple_udp_packet(eth_dst='00:44:44:44:44:44',
                                    eth_src='00:22:22:22:22:22',
                                    dl_vlan_enable=True,
                                    vlan_vid=0,
                                    ip_ttl=64)
            exp_pkt = simple_udp_packet(eth_dst='00:44:44:44:44:44',
                                        eth_src='00:22:22:22:22:22',
                                        dl_vlan_enable=True,
                                        vlan_vid=10,
                                        ip_ttl=64)
            print("Sending priority tagged packet on trunk port %d -> "
                  "trunk port %d" % (self.dev_port1, self.dev_port25))
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port25)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            pkt = simple_udp_packet(eth_dst='00:77:77:77:77:77',
                                    eth_src='00:66:66:66:66:66',
                                    dl_vlan_enable=True,
                                    vlan_vid=0,
                                    ip_ttl=64)
            exp_pkt = simple_udp_packet(eth_dst='00:77:77:77:77:77',
                                        eth_src='00:66:66:66:66:66',
                                        dl_vlan_enable=True,
                                        vlan_vid=20,
                                        ip_ttl=64)
            print("Sending priority tagged packet on trunk lag2 -> "
                  "trunk port %d" % (self.dev_port26))
            send_packet(self, self.dev_port7, pkt)
            verify_packet(self, exp_pkt, self.dev_port26)

            pkt = simple_udp_packet(eth_dst='00:11:11:11:11:11',
                                    eth_src='00:22:22:22:22:22',
                                    dl_vlan_enable=True,
                                    vlan_vid=0,
                                    ip_ttl=64,
                                    pktlen=104)
            exp_pkt = simple_udp_packet(eth_dst='00:11:11:11:11:11',
                                        eth_src='00:22:22:22:22:22',
                                        ip_ttl=64,
                                        pktlen=100)
            print("Sending priority tagged packet on trunk port %d -> "
                  "access port %d" % (self.dev_port1, self.dev_port0))
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.i_pkt_count += 1
            self.e_pkt_count += 1

            pkt = simple_udp_packet(eth_dst='00:88:88:88:88:88',
                                    eth_src='00:66:66:66:66:66',
                                    dl_vlan_enable=True,
                                    vlan_vid=0,
                                    ip_ttl=64,
                                    pktlen=104)
            exp_pkt = simple_udp_packet(eth_dst='00:88:88:88:88:88',
                                        eth_src='00:66:66:66:66:66',
                                        ip_ttl=64,
                                        pktlen=100)
            print("Sending priority tagged packet on trunk lag2 -> "
                  "access port %d" % (self.dev_port27))
            send_packet(self, self.dev_port7, pkt)
            verify_packet(self, exp_pkt, self.dev_port27)
        finally:
            sai_thrift_set_port_attribute(
                self.client, self.port1, port_vlan_id=1)
            sai_thrift_set_lag_attribute(
                self.client, self.lag2, port_vlan_id=1)

            sai_thrift_remove_fdb_entry(self.client, fdb_entry1)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry2)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry3)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry4)

            sai_thrift_remove_vlan_member(self.client, vlan_member1)
            sai_thrift_remove_vlan_member(self.client, vlan_member2)

    def pvDropTest(self):
        """
        Verifies drops for invalid port-vlan packet on untagged port
        and tagged port
        """
        print("\npvDropTest()")
        v100_pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                     eth_src='00:22:22:22:22:22',
                                     dl_vlan_enable=True,
                                     vlan_vid=100,
                                     ip_dst='10.0.0.1',
                                     ip_ttl=64)
        untagged_pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                         eth_src='00:22:22:22:22:22',
                                         ip_dst='10.0.0.1',
                                         ip_ttl=64)
        v10_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                    eth_src='00:11:11:11:11:11',
                                    ip_dst='10.0.0.1',
                                    dl_vlan_enable=True,
                                    vlan_vid=10,
                                    ip_ttl=64)
        exp_at_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                       eth_src='00:11:11:11:11:11',
                                       ip_dst='10.0.0.1',
                                       dl_vlan_enable=True,
                                       vlan_vid=10,
                                       ip_ttl=64)
        v11_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                    eth_src='00:11:11:11:11:11',
                                    ip_dst='10.0.0.1',
                                    dl_vlan_enable=True,
                                    vlan_vid=11,
                                    ip_ttl=64)

        print("Sending L2 vlan100 tagged packet to vlan10 tagged port %d, "
              "dropped" % (self.dev_port1))
        send_packet(self, self.dev_port1, v100_pkt)
        verify_no_other_packets(self, timeout=1)

        print("Sending L2 untagged packet to vlan10 tagged port %d, "
              "dropped" % (self.dev_port1))
        send_packet(self, self.dev_port1, untagged_pkt)
        verify_no_other_packets(self, timeout=1)

        print("Sending L2 vlan10 tagged packet to vlan10 untagged "
              "port %d, forwarded" % (self.dev_port0))
        send_packet(self, self.dev_port0, v10_pkt)
        verify_packet(self, exp_at_pkt, self.dev_port1)
        self.i_pkt_count += 1
        self.e_pkt_count += 1

        stats = sai_thrift_get_port_stats(self.client, self.port0)
        if_in_vlan_discards_pre = \
            stats['SAI_PORT_STAT_IF_IN_VLAN_DISCARDS']

        print("Sending L2 vlan11 tagged packet to vlan 10 untagged"
              "port %d, dropped" % (self.dev_port0))
        send_packet(self, self.dev_port0, v11_pkt)
        verify_no_other_packets(self, timeout=1)

        stats = sai_thrift_get_port_stats(self.client, self.port0)
        if_in_vlan_discards = stats['SAI_PORT_STAT_IF_IN_VLAN_DISCARDS']
        self.assertEqual(if_in_vlan_discards_pre + 1, if_in_vlan_discards)

    def lagPVMissTest(self):
        """
        Verifies drops for invalid port-vlan packet on
        untagged LAG all members and tagged LAG all members
        """
        print("\nlagPVMissTest()")
        mac_action = SAI_PACKET_ACTION_FORWARD
        mac7 = '00:77:77:77:77:77'
        mac8 = '00:88:88:88:88:88'
        mac9 = '00:99:99:99:99:99'

        fdb_entry1 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=mac7,
            bv_id=self.vlan20)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry1,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port26_bp,
            packet_action=mac_action)
        fdb_entry2 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=mac8,
            bv_id=self.vlan40)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry2,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port26_bp,
            packet_action=mac_action)
        fdb_entry3 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=mac9,
            bv_id=self.vlan20)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry3,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.lag2_bp,
            packet_action=mac_action)

        try:
            print("Send packet with vlan 20 from port %d to port %d, valid" % (
                self.dev_port8, self.dev_port26))
            pkt = simple_tcp_packet(eth_dst='00:77:77:77:77:77',
                                    eth_src='00:99:99:99:99:99',
                                    dl_vlan_enable=True,
                                    vlan_vid=20,
                                    ip_dst='10.0.0.1',
                                    ip_ttl=64)
            send_packet(self, self.dev_port8, pkt)
            verify_packet(self, pkt, self.dev_port26)

            print("Send packet with vlan 40 from port %d, dropped" % (
                self.dev_port8))
            pkt = simple_tcp_packet(eth_dst='00:88:88:88:88:88',
                                    eth_src='00:99:99:99:99:99',
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_dst='10.0.0.1',
                                    ip_ttl=64)
            send_packet(self, self.dev_port8, pkt)
            verify_no_other_packets(self, timeout=1)

            print("Send packet with vlan 50 from port %d, dropped" % (
                self.dev_port7))
            pkt = simple_tcp_packet(eth_dst='00:88:88:88:88:88',
                                    eth_src='00:99:99:99:99:99',
                                    dl_vlan_enable=True,
                                    vlan_vid=50,
                                    ip_dst='10.0.0.1',
                                    ip_ttl=64)
            send_packet(self, self.dev_port7, pkt)
            verify_no_other_packets(self, timeout=1)
        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry1)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry2)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry3)

    def vlanIngressAclTest(self):
        '''
        Test sai get vlan ingress ACL attribute
        '''
        print("\nvlanIngressAclTest()")
        v10_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                    eth_src='00:11:11:11:11:11',
                                    ip_dst='10.0.0.1',
                                    dl_vlan_enable=True,
                                    vlan_vid=10,
                                    ip_ttl=64)
        exp_at_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                       eth_src='00:11:11:11:11:11',
                                       ip_dst='10.0.0.1',
                                       dl_vlan_enable=True,
                                       vlan_vid=10,
                                       ip_ttl=64)
        send_packet(self, self.dev_port0, v10_pkt)
        verify_packet(self, exp_at_pkt, self.dev_port1)
        try:
            # create ACL table
            table_stage = SAI_ACL_STAGE_INGRESS
            table_bind_points = [SAI_ACL_BIND_POINT_TYPE_VLAN]
            table_bind_point_type_list = sai_thrift_s32_list_t(
                count=len(table_bind_points), int32list=table_bind_points)

            acl_table = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage,
                acl_bind_point_type_list=table_bind_point_type_list)

            # create ACL table entry
            dst_ip = '10.0.0.1'
            dst_ip_mask = '255.255.255.255'
            dst_ip_t = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip4=dst_ip),
                mask=sai_thrift_acl_field_data_mask_t(ip4=dst_ip_mask))

            packet_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    s32=SAI_PACKET_ACTION_DROP))
            acl_entry = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table,
                priority=10,
                field_dst_ip=dst_ip_t,
                action_packet_action=packet_action)

            # bind acl table to ingress vlan 10
            sai_thrift_set_vlan_attribute(self.client, self.vlan10,
                                          ingress_acl=acl_table)
            id = sai_thrift_get_vlan_attribute(self.client, self.vlan10,
                                               ingress_acl=acl_table)

            self.assertEqual(acl_table, id.get('ingress_acl'))
            send_packet(self, self.dev_port0, v10_pkt)
            verify_no_other_packets(self, timeout=2)
        finally:
            sai_thrift_set_vlan_attribute(self.client,
                                          self.vlan10,
                                          ingress_acl=0)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)

    def vlanEgressAclTest(self):
        '''
        Test sai get vlan egress ACL attribute
        '''
        print("\nvlanEgressAclTest()")
        v10_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                    eth_src='00:11:11:11:11:11',
                                    ip_dst='10.0.0.1',
                                    dl_vlan_enable=True,
                                    vlan_vid=10,
                                    ip_ttl=64)
        exp_at_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                       eth_src='00:11:11:11:11:11',
                                       ip_dst='10.0.0.1',
                                       dl_vlan_enable=True,
                                       vlan_vid=10,
                                       ip_ttl=64)
        send_packet(self, self.dev_port0, v10_pkt)
        verify_packet(self, exp_at_pkt, self.dev_port1)
        try:
            # create ACL table
            table_stage = SAI_ACL_STAGE_EGRESS
            table_bind_points = [SAI_ACL_BIND_POINT_TYPE_VLAN]
            table_bind_point_type_list = sai_thrift_s32_list_t(
                count=len(table_bind_points), int32list=table_bind_points)

            acl_table = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage,
                acl_bind_point_type_list=table_bind_point_type_list)

            # create ACL table entry
            dst_ip = '10.0.0.1'
            dst_ip_mask = '255.255.255.255'
            dst_ip_t = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip4=dst_ip),
                mask=sai_thrift_acl_field_data_mask_t(ip4=dst_ip_mask))

            packet_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    s32=SAI_PACKET_ACTION_DROP))
            acl_entry = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table,
                priority=10,
                field_dst_ip=dst_ip_t,
                action_packet_action=packet_action)

            # bind acl table to egress vlan 20
            sai_thrift_set_vlan_attribute(self.client, self.vlan10,
                                          egress_acl=acl_table)
            id = sai_thrift_get_vlan_attribute(self.client, self.vlan10,
                                               egress_acl=acl_table)

            self.assertEqual(acl_table, id.get('egress_acl'))
            send_packet(self, self.dev_port0, v10_pkt)
            verify_no_other_packets(self, timeout=2)
        finally:
            sai_thrift_set_vlan_attribute(self.client,
                                          self.vlan10,
                                          egress_acl=0)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)

    def vlanFloodTest(self):
        """
        Verifies different cases of flooding
        """
        print("\nvlanFloodTest()")
        sai_thrift_set_port_attribute(
            self.client, self.port26, port_vlan_id=40)
        sai_thrift_set_port_attribute(
            self.client, self.port27, port_vlan_id=50)
        sai_thrift_set_lag_attribute(self.client, self.lag10, port_vlan_id=60)
        sai_thrift_set_lag_attribute(self.client, self.lag11, port_vlan_id=70)

        try:
            self.pkt = simple_arp_packet(
                eth_src='00:22:22:33:44:55',
                arp_op=1,  # ARP request
                hw_snd='00:22:22:33:44:55',
                pktlen=100)
            self.tagged_pkt = simple_arp_packet(
                eth_src='00:22:22:33:44:55',
                arp_op=1,  # ARP request
                hw_snd='00:22:22:33:44:55',
                vlan_vid=30,
                pktlen=104)
            self.arp_resp = simple_arp_packet(
                eth_dst='00:22:22:33:44:55',
                arp_op=2,  # ARP request
                hw_tgt='00:22:22:33:44:55',
                pktlen=100)
            self.tagged_arp_resp = simple_arp_packet(
                eth_dst='00:22:22:33:44:55',
                arp_op=2,  # ARP request
                hw_tgt='00:22:22:33:44:55',
                vlan_vid=30,
                pktlen=104)

            vlan_ports = [
                self.dev_port26,
                self.dev_port27,
                self.dev_port28,
                self.dev_port29]
            vlan_data = {
                self.vlan40: set_vlan_data(40, vlan_ports, [
                    self.dev_port26], self.dev_port29),
                self.vlan50: set_vlan_data(50, vlan_ports, [
                    self.dev_port27], self.dev_port29),
                self.vlan60: set_vlan_data(60, vlan_ports, [
                    self.dev_port28], self.dev_port29),
                self.vlan70: set_vlan_data(70, vlan_ports, [
                    self.dev_port29], self.dev_port29)}
            self._basicVlanFloodTest(vlan_data)

            self.client.sai_thrift_remove_vlan_member(self.vlan_member44)
            self.client.sai_thrift_remove_vlan_member(self.vlan_member52)
            self.client.sai_thrift_remove_vlan_member(self.vlan_member63)
            self.client.sai_thrift_remove_vlan_member(self.vlan_member71)

            vlan_data = {
                self.vlan40: set_vlan_data(40, [
                    self.dev_port26, self.dev_port27, self.dev_port28], [
                        self.dev_port26], self.dev_port28),
                self.vlan50: set_vlan_data(50, [
                    self.dev_port26, self.dev_port28, self.dev_port29], [
                        self.dev_port0], self.dev_port29),
                self.vlan60: set_vlan_data(60, [
                    self.dev_port26, self.dev_port27, self.dev_port29], [
                        self.dev_port0], self.dev_port29),
                self.vlan70: set_vlan_data(70, [
                    self.dev_port27, self.dev_port28, self.dev_port29], [
                        self.dev_port29], self.dev_port29)}
            self._basicVlanFloodTest(vlan_data)

            self.vlan_member44 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan40,
                bridge_port_id=self.lag11_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
            self.vlan_member52 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan50,
                bridge_port_id=self.port27_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            self.vlan_member63 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan60,
                bridge_port_id=self.lag10_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            self.vlan_member71 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan70,
                bridge_port_id=self.port26_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

            vlan_data = {
                self.vlan40: set_vlan_data(
                    40, vlan_ports, [
                        self.dev_port26], self.dev_port29),
                self.vlan50: set_vlan_data(
                    50, vlan_ports, [
                        self.dev_port27], self.dev_port29),
                self.vlan60: set_vlan_data(
                    60, vlan_ports, [
                        self.dev_port28], self.dev_port29),
                self.vlan70: set_vlan_data(
                    70, vlan_ports, [
                        self.dev_port29], self.dev_port29)}
            self._basicVlanFloodTest(vlan_data)

        finally:
            sai_thrift_set_port_attribute(
                self.client, self.port26, port_vlan_id=1)
            sai_thrift_set_port_attribute(
                self.client, self.port27, port_vlan_id=1)
            sai_thrift_set_lag_attribute(
                self.client, self.lag10, port_vlan_id=1)
            sai_thrift_set_lag_attribute(
                self.client, self.lag11, port_vlan_id=1)

    def _basicVlanFloodTest(self, vlan_data):
        """
        Checks flooding for the chosen vlan_data

        Args:
            vlan_data (dictionary): dictionary which contains dictionaries
            with VLANs data
        """
        try:
            for vlan_key in vlan_data.keys():
                vlan = vlan_data[vlan_key]
                print("#### Flooding on %s ####" % (str(vlan["vlan_id"])))
                self.tagged_pkt[Dot1Q].vlan = vlan["vlan_id"]
                self.tagged_arp_resp[Dot1Q].vlan = vlan["vlan_id"]
                pkt_list = [None] * vlan["large_port"]
                arp_pkt_list = [None] * vlan["large_port"]
                for port in vlan["ports"]:
                    if port not in vlan["untagged"]:
                        pkt_list[port - 1] = self.tagged_pkt
                        arp_pkt_list[port - 1] = self.tagged_arp_resp
                    else:
                        pkt_list[port - 1] = self.pkt
                        arp_pkt_list[port - 1] = self.arp_resp
                for port in vlan["ports"]:
                    print("Testing flooding and learning on port%d" % (port))
                    other_ports = [p for p in vlan["ports"] if p != port]
                    verify_pkt_list = [pkt_list[pl - 1] for pl in other_ports]
                    print("Sending arp request from port ", port, " flooding on ", other_ports)
                    send_packet(self, port, pkt_list[port - 1])
                    verify_each_packet_on_each_port(
                        self, verify_pkt_list, other_ports)
                    time.sleep(2)
                    for send_port in other_ports:
                        print("Sending arp response from ", send_port, "->", port)
                        send_packet(self, send_port, arp_pkt_list[send_port - 1])
                        verify_packets(
                            self, arp_pkt_list[port - 1], [port])
                    sai_thrift_flush_fdb_entries(
                        self.client, bv_id=vlan_key, entry_type=SAI_FDB_ENTRY_TYPE_DYNAMIC)
        finally:
            for vlan_key in vlan_data.keys():
                sai_thrift_flush_fdb_entries(
                    self.client, bv_id=vlan_key, entry_type=SAI_FDB_ENTRY_TYPE_DYNAMIC)

    def vlanFloodEnhancedTest(self):
        """
        Verifies flooding for the vlan which contains ports and LAGs
        """
        print("\nvlanFloodEnhancedTest()")
        print("Flood test on ports 1,2, lag 1 and 2")

        vlan100 = sai_thrift_create_vlan(self.client, vlan_id=100)
        vlan_member101 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member102 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member103 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.lag10_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member104 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.lag11_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member105 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port30_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        vlan200 = sai_thrift_create_vlan(self.client, vlan_id=200)
        vlan_member201 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan200,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member202 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan200,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member203 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan200,
            bridge_port_id=self.lag10_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member204 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan200,
            bridge_port_id=self.lag11_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_vlan_attribute(self.client, vlan100, learn_disable=True)
        sai_thrift_set_vlan_attribute(self.client, vlan200, learn_disable=True)

        sai_thrift_set_port_attribute(
            self.client, self.port26, port_vlan_id=200)
        sai_thrift_set_port_attribute(
            self.client, self.port27, port_vlan_id=200)
        sai_thrift_set_lag_attribute(self.client, self.lag10, port_vlan_id=200)
        sai_thrift_set_lag_attribute(self.client, self.lag11, port_vlan_id=200)

        pkt200 = simple_arp_packet(
            eth_dst='FF:FF:FF:FF:FF:FF',
            eth_src='00:22:22:33:44:55',
            arp_op=1,  # ARP request
            ip_tgt='10.10.10.1',
            ip_snd='10.10.10.2',
            hw_snd='00:22:22:33:44:55',
            pktlen=100)
        pkt100 = simple_arp_packet(
            eth_dst='FF:FF:FF:FF:FF:FF',
            eth_src='00:22:22:33:44:55',
            arp_op=1,  # ARP request
            ip_tgt='10.10.10.1',
            ip_snd='10.10.10.2',
            hw_snd='00:22:22:33:44:55',
            vlan_vid=100,
            pktlen=100)

        try:
            print("Add ports 24 and 25 to lag10 and lag11")
            lag_mbr32 = sai_thrift_create_lag_member(
                self.client, lag_id=self.lag10, port_id=self.port24)
            lag_mbr42 = sai_thrift_create_lag_member(
                self.client, lag_id=self.lag11, port_id=self.port25)

            lag1_ports = [self.dev_port28, self.dev_port24]
            lag2_ports = [self.dev_port29, self.dev_port25]
            send_packet(self, self.dev_port26, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 4,
                [lag1_ports, lag2_ports,
                 [self.dev_port27], [self.dev_port30]])
            send_packet(self, self.dev_port28, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 4,
                [[self.dev_port26], lag2_ports,
                 [self.dev_port27], [self.dev_port30]])
            send_packet(self, self.dev_port29, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 4,
                [[self.dev_port26], lag1_ports,
                 [self.dev_port27], [self.dev_port30]])
            send_packet(self, self.dev_port30, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 4,
                [[self.dev_port26], lag1_ports,
                 lag2_ports, [self.dev_port27]])
            
            print("Remove ports 24 and 25 from lag10 and lag11")
            sai_thrift_remove_lag_member(self.client, lag_mbr32)
            sai_thrift_remove_lag_member(self.client, lag_mbr42)

            lag1_ports = [self.dev_port28]
            lag2_ports = [self.dev_port29]
            send_packet(self, self.dev_port26, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 4,
                [lag1_ports, lag2_ports,
                 [self.dev_port27], [self.dev_port30]])
            send_packet(self, self.dev_port28, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 4,
                [[self.dev_port26], lag2_ports,
                 [self.dev_port27], [self.dev_port30]])
            send_packet(self, self.dev_port29, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 4,
                [[self.dev_port26], lag1_ports,
                 [self.dev_port27], [self.dev_port30]])
            send_packet(self, self.dev_port30, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 4,
                [[self.dev_port26], lag1_ports,
                 lag2_ports, [self.dev_port27]])

            print("Add ports 24 and 25 to lag10 and lag11")
            lag_mbr32 = sai_thrift_create_lag_member(
                self.client, lag_id=self.lag10, port_id=self.port24)
            lag_mbr42 = sai_thrift_create_lag_member(
                self.client, lag_id=self.lag11, port_id=self.port25)

            lag1_ports = [self.dev_port28, self.dev_port24]
            lag2_ports = [self.dev_port29, self.dev_port25]
            send_packet(self, self.dev_port26, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 4,
                [lag1_ports, lag2_ports,
                 [self.dev_port27], [self.dev_port30]])
            send_packet(self, self.dev_port28, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 4,
                [[self.dev_port26], lag2_ports,
                 [self.dev_port27], [self.dev_port30]])
            send_packet(self, self.dev_port29, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 4,
                [[self.dev_port26], lag1_ports,
                 [self.dev_port27], [self.dev_port30]])
            send_packet(self, self.dev_port30, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 4,
                [[self.dev_port26], lag1_ports,
                 lag2_ports, [self.dev_port27]])

            print("Remove port 30 from vlan 100")
            sai_thrift_remove_vlan_member(self.client, vlan_member105)
            send_packet(self, self.dev_port26, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 3,
                [lag1_ports, lag2_ports, [self.dev_port27]])
            send_packet(self, self.dev_port28, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 3,
                [[self.dev_port26], lag2_ports, [self.dev_port27]])
            send_packet(self, self.dev_port29, pkt100)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt100] * 3,
                [[self.dev_port26], lag1_ports, [self.dev_port27]])

            send_packet(self, self.dev_port27, pkt200)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt200] * 3,
                [lag1_ports, lag2_ports, [self.dev_port26]])
            send_packet(self, self.dev_port24, pkt200)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt200] * 3,
                [[self.dev_port26], lag2_ports, [self.dev_port27]])
            send_packet(self, self.dev_port25, pkt200)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt200] * 3,
                [[self.dev_port26], lag1_ports, [self.dev_port27]])

            print("Add port 30 to vlan 200")
            vlan_member205 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=vlan200,
                bridge_port_id=self.port30_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            sai_thrift_set_port_attribute(
                self.client, self.port30, port_vlan_id=200)
            send_packet(self, self.dev_port27, pkt200)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt200] * 4,
                [lag1_ports, lag2_ports,
                 [self.dev_port26], [self.dev_port30]])
            send_packet(self, self.dev_port24, pkt200)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt200] * 4,
                [[self.dev_port26], lag2_ports,
                 [self.dev_port27], [self.dev_port30]])
            send_packet(self, self.dev_port25, pkt200)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt200] * 4,
                [[self.dev_port26], lag1_ports,
                 [self.dev_port27], [self.dev_port30]])
            send_packet(self, self.dev_port30, pkt200)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt200] * 4,
                [[self.dev_port26], lag1_ports,
                 lag2_ports, [self.dev_port27]])

        finally:
            sai_thrift_set_port_attribute(
                self.client, self.port26, port_vlan_id=1)
            sai_thrift_set_port_attribute(
                self.client, self.port27, port_vlan_id=1)
            sai_thrift_set_port_attribute(
                self.client, self.port30, port_vlan_id=1)
            sai_thrift_set_lag_attribute(
                self.client, self.lag10, port_vlan_id=1)
            sai_thrift_set_lag_attribute(
                self.client, self.lag11, port_vlan_id=1)

            sai_thrift_set_vlan_attribute(
                self.client, vlan100, learn_disable=False)
            sai_thrift_set_vlan_attribute(
                self.client, vlan200, learn_disable=False)

            sai_thrift_remove_vlan_member(self.client, vlan_member101)
            sai_thrift_remove_vlan_member(self.client, vlan_member102)
            sai_thrift_remove_vlan_member(self.client, vlan_member103)
            sai_thrift_remove_vlan_member(self.client, vlan_member104)

            sai_thrift_remove_vlan_member(self.client, vlan_member201)
            sai_thrift_remove_vlan_member(self.client, vlan_member202)
            sai_thrift_remove_vlan_member(self.client, vlan_member203)
            sai_thrift_remove_vlan_member(self.client, vlan_member204)
            sai_thrift_remove_vlan_member(self.client, vlan_member205)

            sai_thrift_remove_vlan(self.client, vlan100)
            sai_thrift_remove_vlan(self.client, vlan200)

            sai_thrift_remove_lag_member(self.client, lag_mbr32)
            sai_thrift_remove_lag_member(self.client, lag_mbr42)

    def vlanFloodDisableTest(self):
        """
        Verifies disable flooding for
        unknown_unicast/unknown_multicast/broadcast
        """
        print("\nvlanFloodDisableTest()")

        vlan100 = sai_thrift_create_vlan(self.client, vlan_id=100)
        vlan_member101 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port24_bp)
        vlan_member102 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port25_bp)
        vlan_member103 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port26_bp)
        vlan_member104 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port27_bp)

        sai_thrift_set_vlan_attribute(self.client, vlan100, learn_disable=True)

        sai_thrift_set_port_attribute(
            self.client, self.port24, port_vlan_id=100)

        sai_thrift_set_port_attribute(
            self.client, self.port25, port_vlan_id=100)

        sai_thrift_set_port_attribute(
            self.client, self.port26, port_vlan_id=100)

        sai_thrift_set_port_attribute(
            self.client, self.port27, port_vlan_id=100)

        try:
            flood_control_type_none = SAI_VLAN_FLOOD_CONTROL_TYPE_NONE
            ucast_pkt = simple_tcp_packet(
                eth_dst='00:11:11:11:11:11',
                eth_src='00:22:22:22:22:22',
                ip_dst='10.0.0.1',
                ip_id=107,
                ip_ttl=64)
            mcast_pkt = simple_tcp_packet(
                eth_dst='01:11:11:11:11:11',
                eth_src='00:22:22:22:22:22',
                ip_dst='231.0.0.1',
                ip_id=107,
                ip_ttl=64)
            bcast_pkt = simple_tcp_packet(
                eth_dst='FF:FF:FF:FF:FF:FF',
                eth_src='00:22:22:22:22:22',
                ip_dst='10.0.0.1',
                ip_id=107,
                ip_ttl=64)

            send_packet(self, self.dev_port26, ucast_pkt)
            verify_packets(
                self, ucast_pkt, [
                    self.dev_port24, self.dev_port25, self.dev_port27])
            send_packet(self, self.dev_port26, mcast_pkt)
            verify_packets(
                self, mcast_pkt, [
                    self.dev_port24, self.dev_port25, self.dev_port27])
            send_packet(self, self.dev_port26, bcast_pkt)
            verify_packets(
                self, bcast_pkt, [
                    self.dev_port24, self.dev_port25, self.dev_port27])

            print("Disable unknown unicast flooding on vlan 100")
            sai_thrift_set_vlan_attribute(
                self.client,
                vlan100,
                unknown_unicast_flood_control_type=flood_control_type_none)
            send_packet(self, self.dev_port26, ucast_pkt)
            verify_no_other_packets(self, timeout=1)

            print("Disable unknown multicast flooding on vlan 100")
            sai_thrift_set_vlan_attribute(
                self.client,
                vlan100,
                unknown_multicast_flood_control_type=flood_control_type_none)
            send_packet(self, self.dev_port26, mcast_pkt)
            verify_no_other_packets(self, timeout=1)

            print("Disable broadcast flooding on vlan 100")
            sai_thrift_set_vlan_attribute(
                self.client,
                vlan100,
                broadcast_flood_control_type=flood_control_type_none)
            send_packet(self, self.dev_port26, bcast_pkt)
            verify_no_other_packets(self, timeout=1)

        finally:
            sai_thrift_set_port_attribute(
                self.client, self.port24, port_vlan_id=1)
            sai_thrift_set_port_attribute(
                self.client, self.port25, port_vlan_id=1)
            sai_thrift_set_port_attribute(
                self.client, self.port26, port_vlan_id=1)
            sai_thrift_set_port_attribute(
                self.client, self.port27, port_vlan_id=1)

            sai_thrift_set_vlan_attribute(
                self.client, vlan100, learn_disable=False)

            sai_thrift_remove_vlan_member(self.client, vlan_member101)
            sai_thrift_remove_vlan_member(self.client, vlan_member102)
            sai_thrift_remove_vlan_member(self.client, vlan_member103)
            sai_thrift_remove_vlan_member(self.client, vlan_member104)

            sai_thrift_remove_vlan(self.client, vlan100)

    def countersClearTest(self):
        """
        Verifies clear statistics for VLAN
        """
        print("\ncountersClearTest()")
        pkt = simple_tcp_packet(eth_dst='00:33:33:33:33:33',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='172.16.0.1',
                                ip_id=101,
                                ip_ttl=64)
        stats = sai_thrift_get_vlan_stats(self.client, self.vlan10)
        in_bytes_pre = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes_pre = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets_pre = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets_pre = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets_pre = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets_pre = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

        print("Sending L2 packet port 0 -> port 24 [access vlan=10])")
        send_packet(self, self.dev_port0, pkt)
        verify_packet(self, pkt, self.dev_port24)

        # Check counters
        stats = sai_thrift_get_vlan_stats(self.client, self.vlan10)
        in_bytes = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]
        self.assertEqual((in_packets, in_packets_pre + 1),
                        'vlan IN packets counter {} != {}'.format(
                            in_packets, in_packets_pre + 1))
        self.assertEqual((in_ucast_packets, in_ucast_packets_pre + 1),
                        'vlan IN unicats packets counter {} != {}'.format(
                            in_ucast_packets, in_ucast_packets_pre + 1))
        self.assertNotEqual(((in_bytes - in_bytes_pre), 0),
                        'vlan IN bytes counter is 0')
        self.assertEqual((out_packets, out_packets_pre + 1),
                        'vlan OUT packets counter {} != {}'.format(
                            out_packets, out_packets_pre + 1))
        self.assertEqual((out_ucast_packets, out_ucast_packets_pre + 1),
                        'vlan OUT unicats packets counter {} != {}'.format(
                            out_ucast_packets, out_ucast_packets_pre + 1))
        self.assertEqual(((out_bytes - out_bytes_pre), 0),
                        'vlan OUT bytes counter is 0')

        print("Sending L2 packet port 0 -> port 24 [access vlan=10])")
        send_packet(self, self.dev_port0, pkt)
        verify_packet(self, pkt, self.dev_port24)

        # Clear bytes and packets counter
        sai_thrift_clear_vlan_stats(self.client, self.vlan10)

        # Check counters
        stats = sai_thrift_get_vlan_stats(self.client, self.vlan10)
        in_bytes = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

        self.assertEqual(in_packets, 0, 'vlan IN packets counter is not 0')
        self.assertEqual(in_ucast_packets, 0,
                        'vlan IN unicast packets counter is not 0')
        self.assertEqual(in_bytes, 0, 'vlan IN bytes counter is not 0')
        self.assertEqual(out_packets, 0,
                        'vlan OUT packets counter is not 0')
        self.assertEqual(out_ucast_packets, 0,
                        'vlan OUT unicast packets counter is not 0')
        self.assertEqual(out_bytes, 0, 'vlan OUT bytes counter is not 0')

    def vlanFloodPruneTest(self):
        """
        Verifies ingress port pruning on ports and LAG when flooding
        """
        print("\nvlanFloodPruneTest()")
        sai_thrift_set_vlan_attribute(
            self.client, self.vlan10, learn_disable=True)
        print("Mac Learning disabled on vlan 10")

        self.port31_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port31,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        lag = sai_thrift_create_lag(self.client)
        lag_mbr1 = sai_thrift_create_lag_member(
            self.client, lag_id=lag, port_id=self.port26)
        lag_mbr2 = sai_thrift_create_lag_member(
            self.client, lag_id=lag, port_id=self.port27)
        lag_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=lag,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        vlan_member = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=lag_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_lag_attribute(self.client, lag, port_vlan_id=10)

        pkt = simple_tcp_packet(eth_dst='00:66:66:66:66:66',
                                ip_dst='10.0.0.1',
                                ip_id=107,
                                ip_ttl=64,
                                pktlen=96)
        exp_pkt = simple_tcp_packet(eth_dst='00:66:66:66:66:66',
                                    ip_dst='10.0.0.1',
                                    ip_id=107,
                                    ip_ttl=64,
                                    pktlen=96)
        exp_pkt_tag = simple_tcp_packet(eth_dst='00:66:66:66:66:66',
                                        ip_dst='10.0.0.1',
                                        dl_vlan_enable=True,
                                        vlan_vid=10,
                                        ip_id=107,
                                        ip_ttl=64,
                                        pktlen=100)
        try:
            lag0_ports = [self.dev_port4, self.dev_port5, self.dev_port6]
            lag1_ports = [self.dev_port26, self.dev_port27]
            print("Sending packet from port30 -> "
                  "lag1, lag, port1, port24, port25")
            send_packet(self, self.dev_port0, pkt)
            verify_each_packet_on_multiple_port_lists(
                self,
                [exp_pkt] * 3 + [exp_pkt_tag] * 2,
                [lag0_ports, lag1_ports, [self.dev_port24],
                 [self.dev_port1], [self.dev_port25]])

            print("Add a new lag with no memmbers to vlan 10. "
                  "Packet output will not change")
            lag2 = sai_thrift_create_lag(self.client)
            lag2_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=lag2,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            vlan_member2 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan10,
                bridge_port_id=lag2_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            sai_thrift_set_lag_attribute(self.client, lag2, port_vlan_id=10)
            send_packet(self, self.dev_port0, pkt)
            verify_each_packet_on_multiple_port_lists(
                self,
                exp_pkt * 3 + exp_pkt_tag * 2,
                [lag0_ports, lag1_ports, [self.dev_port24],
                 [self.dev_port1], [self.dev_port25]])

            print("Add port %d to the new lag" % (self.dev_port30))
            lag_mbr3 = sai_thrift_create_lag_member(
                self.client, lag_id=lag2, port_id=self.port30)
            lag2_ports = [self.dev_port30]
            print("Sending packet from port0 -> "
                  "lag1, lag, new lag, port1, port24, port25")
            send_packet(self, self.dev_port0, pkt)
            verify_each_packet_on_multiple_port_lists(
                self,
                exp_pkt * 4 + exp_pkt_tag * 2,
                [lag0_ports, lag1_ports, lag2_ports, [self.dev_port24],
                 [self.dev_port1], [self.dev_port25]])

            print("Sending packet from port30 -> "
                  "lag1, lag, port1, port0, port24, port25")
            send_packet(self, self.dev_port30, pkt)
            verify_each_packet_on_multiple_port_lists(
                self,
                exp_pkt * 4 + exp_pkt_tag * 2,
                [lag0_ports, lag1_ports, [self.dev_port0], [self.dev_port24],
                 [self.dev_port1], [self.dev_port25]])

            lag_mbr4 = sai_thrift_create_lag_member(
                self.client, lag_id=lag2, port_id=self.port31)
            lag2_ports = [self.dev_port30, self.dev_port31]
            print("Sending packet from port0 -> "
                  "lag1, lag, new lag, port1, port24, port25")
            send_packet(self, self.dev_port0, pkt)
            verify_each_packet_on_multiple_port_lists(
                self,
                exp_pkt * 4 + exp_pkt_tag * 2,
                [lag0_ports, lag1_ports, lag2_ports, [self.dev_port24],
                 [self.dev_port1], [self.dev_port25]])

            print("Sending packet from port31 -> "
                  "lag1, lag, port1, port0, port24, port25")
            send_packet(self, self.dev_port31, pkt)
            verify_each_packet_on_multiple_port_lists(
                self,
                exp_pkt * 4 + exp_pkt_tag * 2,
                [lag0_ports, lag1_ports, [self.dev_port0], [self.dev_port24],
                 [self.dev_port1], [self.dev_port25]])

            print("Remove port %d from new lag" % (self.dev_port31))
            sai_thrift_remove_lag_member(self.client, lag_mbr4)
            lag2_ports = [self.dev_port30]
            print("Sending packet from port0 -> "
                  "lag1, lag, new lag, port1, port24, port25")
            send_packet(self, self.dev_port0, pkt)
            verify_each_packet_on_multiple_port_lists(
                self,
                exp_pkt * 4 + exp_pkt_tag * 2,
                [lag0_ports, lag1_ports, lag2_ports, [self.dev_port24],
                 [self.dev_port1], [self.dev_port25]])

            print("Add port %d to vlan 10. Verify if it is now part of flood "
                  "list" % (self.dev_port31))
            vlan_member3 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan10,
                bridge_port_id=self.port31_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            sai_thrift_set_port_attribute(
                self.client, self.port31, port_vlan_id=10)
            print("Sending packet from port0 -> "
                  "lag1, lag, new lag, port1, port24, port25, port31")
            send_packet(self, self.dev_port0, pkt)
            verify_each_packet_on_multiple_port_lists(
                self,
                exp_pkt * 5 + exp_pkt_tag * 2,
                [lag0_ports, lag1_ports, lag2_ports,
                 [self.dev_port24], [self.dev_port31],
                 [self.dev_port1], [self.dev_port25]])

            print("Sending packet from port31 -> "
                  "lag1, lag, new lag, port1, port0, port24, port25")
            send_packet(self, self.dev_port31, pkt)
            verify_each_packet_on_multiple_port_lists(
                self,
                exp_pkt * 5 + exp_pkt_tag * 2,
                [lag0_ports, lag1_ports, lag2_ports,
                 [self.dev_port0], [self.dev_port24],
                 [self.dev_port1], [self.dev_port25]])

            sai_thrift_set_port_attribute(
                self.client, self.port31, port_vlan_id=1)
            self.client.sai_thrift_remove_vlan_member(
                vlan_member3)  # Remove port 31 from vlan 10
            sai_thrift_remove_lag_member(
                self.client, lag_mbr3)  # Remove port 30 from new lag
            print("Remove port %d from new lag. "
                  "No more packets should be seen now" % (self.dev_port30))
            print("Sending packet from port 0 -> "
                  "lag0, lag, port1, port24, port25")
            send_packet(self, self.dev_port0, pkt)
            verify_each_packet_on_multiple_port_lists(
                self,
                exp_pkt * 3 + exp_pkt_tag * 2,
                [lag0_ports, lag1_ports, [self.dev_port24],
                 [self.dev_port1], [self.dev_port25]])
        finally:
            sai_thrift_set_lag_attribute(self.client, lag, port_vlan_id=1)
            sai_thrift_set_lag_attribute(self.client, lag2, port_vlan_id=1)

            self.client.sai_thrift_remove_vlan_member(vlan_member)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)

            sai_thrift_remove_lag_member(self.client, lag_mbr1)
            sai_thrift_remove_lag_member(self.client, lag_mbr2)

            sai_thrift_remove_bridge_port(self.client, lag_bp)
            sai_thrift_remove_bridge_port(self.client, lag2_bp)
            sai_thrift_remove_lag(self.client, lag)
            sai_thrift_remove_lag(self.client, lag2)

            sai_thrift_set_vlan_attribute(
                self.client, self.vlan10, learn_disable=False)
            sai_thrift_remove_bridge_port(self.client, self.port31_bp)

    def vlanStatsTest(self):
        """
        Verifies ingress and egress Unicast/Multicast/Broadcast
        statistics for VLAN
        """
        print("\nvlanStatsTest()")
        stats = sai_thrift_get_vlan_stats(self.client, self.vlan10)
        in_bytes = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

        self.assertEqual((in_packets, self.i_pkt_count),
                        'vlan IN packets counter {} != {}'.format(
                            in_packets, self.i_pkt_count))
        self.assertEqual((in_ucast_packets, self.i_pkt_count),
                        'vlan IN unicast packets counter {} != {}'.format(
                            in_ucast_packets, self.i_pkt_count))
        self.assertNotEqual((in_bytes, 0), 'vlan IN bytes counter is 0')
        self.assertEqual((out_packets, self.e_pkt_count),
                        'vlan OUT packets counter {} != {}'.format(
                            out_packets, self.e_pkt_count))
        self.assertEqual((out_ucast_packets, self.e_pkt_count),
                        'vlan OUT unicast packets counter {} != {}'.format(
                            out_ucast_packets, self.e_pkt_count))
        self.assertNotEqual((out_bytes, 0), 'vlan OUT bytes counter is 0')

    def vlanMemberList(self):
        """
        Verifies VLAN member list using SAI_VLAN_ATTR_MEMBER_LIST
        """
        print("vlanMemberList")
        vlan_member_list = sai_thrift_object_list_t(count=100)
        mbr_list = sai_thrift_get_vlan_attribute(
            self.client, self.vlan10, member_list=vlan_member_list)
        vlan_members = mbr_list['SAI_VLAN_ATTR_MEMBER_LIST'].idlist
        count = mbr_list['SAI_VLAN_ATTR_MEMBER_LIST'].count
        self.assertEqual(count, 5)
        self.assertEqual(self.vlan10_member0, vlan_members[0])
        self.assertEqual(self.vlan10_member1, vlan_members[1])
        self.assertEqual(self.vlan10_member2, vlan_members[2])
        self.assertEqual(self.vlan10_member3, vlan_members[3])
        self.assertEqual(self.vlan10_member4, vlan_members[4])

        # Adding vlan members and veryfing vlan member list
        vlan10_member5 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan10_member6 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        vlan_member_list = sai_thrift_object_list_t(count=100)
        mbr_list = sai_thrift_get_vlan_attribute(
            self.client, self.vlan10, member_list=vlan_member_list)
        vlan_members = mbr_list['SAI_VLAN_ATTR_MEMBER_LIST'].idlist
        count = mbr_list['SAI_VLAN_ATTR_MEMBER_LIST'].count
        self.assertEqual(count, 7)
        self.assertEqual(self.vlan10_member0, vlan_members[0])
        self.assertEqual(self.vlan10_member1, vlan_members[1])
        self.assertEqual(self.vlan10_member2, vlan_members[2])
        self.assertEqual(self.vlan10_member3, vlan_members[3])
        self.assertEqual(self.vlan10_member4, vlan_members[4])
        self.assertEqual(vlan10_member5, vlan_members[5])
        self.assertEqual(vlan10_member6, vlan_members[6])

        # Removing vlan members and veryfing vlan member list
        sai_thrift_remove_vlan_member(self.client, vlan10_member5)
        sai_thrift_remove_vlan_member(self.client, vlan10_member6)

        vlan_member_list = sai_thrift_object_list_t(count=100)
        mbr_list = sai_thrift_get_vlan_attribute(
            self.client, self.vlan10, member_list=vlan_member_list)
        vlan_members = mbr_list['SAI_VLAN_ATTR_MEMBER_LIST'].idlist
        count = mbr_list['SAI_VLAN_ATTR_MEMBER_LIST'].count
        self.assertEqual(count, 5)
        self.assertEqual(self.vlan10_member0, vlan_members[0])
        self.assertEqual(self.vlan10_member1, vlan_members[1])
        self.assertEqual(self.vlan10_member2, vlan_members[2])
        self.assertEqual(self.vlan10_member3, vlan_members[3])
        self.assertEqual(self.vlan10_member4, vlan_members[4])

    def vlanNegativeTest(self):
        """
        Verifies VLAN fails under inappropriate conditions
        """
        print("\nvlanNegativeTest()")
        # Create duplicate vlan
        duplicate_vlan = sai_thrift_create_vlan(self.client, vlan_id=10)
        self.assertNotEqual(self.vlan10, duplicate_vlan)
        self.assertEqual(duplicate_vlan, 0)

        # Non existing vlan - attribute not set
        vlan_attr = sai_thrift_get_vlan_attribute(
            self.client, vlan_oid=11, vlan_id=True)
        self.assertEqual(self.status(), SAI_STATUS_INVALID_OBJECT_TYPE)
        self.assertEqual(vlan_attr, None)

        self.assertNotEqual(sai_thrift_set_vlan_attribute(
            self.client,
            vlan_oid=11,
            learn_disable=True), 0)

        vlan_attr = sai_thrift_get_vlan_attribute(
            self.client, vlan_oid=11, learn_disable=True)
        self.assertEqual(self.status(), SAI_STATUS_INVALID_OBJECT_TYPE)
        self.assertEqual(vlan_attr, None)

        self.assertNotEqual(sai_thrift_remove_vlan(self.client, vlan_oid=11),
                            0)

        # Adding incorrect vlan members
        incorrect_member = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=11,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.assertEqual(incorrect_member, 0)
        incorrect_member = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.port0,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.assertEqual(incorrect_member, 0)

    def singleVlanMemberTest(self):
        """
        Verifies packet is dropped when ingress on single vlan member
        """
        print("\nsingleVlanMemberTest()")
        vlan100 = sai_thrift_create_vlan(self.client, vlan_id=100)
        vlan_member = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port0_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        try:
            # Single port in vlan
            pkt = simple_tcp_packet(eth_dst='00:44:44:44:44:44',
                                    eth_src='00:22:22:22:22:22',
                                    dl_vlan_enable=True,
                                    vlan_vid=100,
                                    ip_dst='172.16.0.1',
                                    ip_id=102,
                                    ip_ttl=64)
            send_packet(self, self.dev_port0, pkt)
            verify_no_other_packets(self, timeout=1)

            # Removing port and adding lag to vlan
            sai_thrift_remove_vlan_member(self.client, vlan_member)
            vlan_member = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=vlan100,
                bridge_port_id=self.lag1_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

            # Single LAG in vlan
            pkt = simple_tcp_packet(eth_dst='00:44:44:44:44:44',
                                    eth_src='00:22:22:22:22:22',
                                    dl_vlan_enable=True,
                                    vlan_vid=100,
                                    ip_dst='172.16.0.1',
                                    ip_id=102,
                                    ip_ttl=64)
            send_packet(self, self.dev_port4, pkt)
            verify_no_other_packets(self, timeout=1)

        finally:
            sai_thrift_remove_vlan_member(self.client, vlan_member)
            sai_thrift_remove_vlan(self.client, vlan100)

    def vlanLearningTest(self):
        """
        Verifies disable learn on vlan
        """
        print("\nvlanLearningTest()")

        vlan100 = sai_thrift_create_vlan(self.client,
                                         vlan_id=100,
                                         learn_disable=True)
        vlan_member101 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member102 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member103 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port30_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        vlan200 = sai_thrift_create_vlan(self.client,
                                         vlan_id=200,
                                         learn_disable=True)
        vlan_member201 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan200,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member202 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan200,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member203 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan200,
            bridge_port_id=self.port30_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_port_attribute(
            self.client, self.port26, port_vlan_id=200)
        sai_thrift_set_port_attribute(
            self.client, self.port27, port_vlan_id=200)

        pkt = simple_arp_packet(
            eth_src='00:22:22:33:44:55',
            arp_op=1,  # ARP request
            hw_snd='00:22:22:33:44:55',
            pktlen=100)
        arp_resp = simple_arp_packet(
            eth_dst='00:22:22:33:44:55',
            arp_op=2,  # ARP request
            hw_tgt='00:22:22:33:44:55',
            pktlen=100)

        try:

            send_packet(self, self.dev_port26, pkt)
            verify_packets(self, pkt, [self.dev_port27, self.dev_port30])

            send_packet(self, self.dev_port27, arp_resp)
            verify_packets(self, arp_resp, [self.dev_port26, self.dev_port30])

            lan_attr = sai_thrift_get_vlan_attribute(
                self.client, vlan100, learn_disable=True)
            self.assertTrue(lan_attr.get('learn_disable'))
            sai_thrift_set_vlan_attribute(self.client,
                                          vlan100,
                                          learn_disable=False)
            sai_thrift_set_vlan_attribute(self.client,
                                          vlan200,
                                          learn_disable=False)

            lan_attr = sai_thrift_get_vlan_attribute(
                self.client, vlan100, learn_disable=True)
            self.assertFalse(lan_attr.get('learn_disable'))

        finally:
            sai_thrift_set_port_attribute(
                self.client, self.port26, port_vlan_id=1)
            sai_thrift_set_port_attribute(
                self.client, self.port27, port_vlan_id=1)
            sai_thrift_set_port_attribute(
                self.client, self.port30, port_vlan_id=1)

            sai_thrift_remove_vlan_member(self.client, vlan_member101)
            sai_thrift_remove_vlan_member(self.client, vlan_member102)
            sai_thrift_remove_vlan_member(self.client, vlan_member103)

            sai_thrift_remove_vlan_member(self.client, vlan_member201)
            sai_thrift_remove_vlan_member(self.client, vlan_member202)
            sai_thrift_remove_vlan_member(self.client, vlan_member203)

            sai_thrift_remove_vlan(self.client, vlan100)
            sai_thrift_remove_vlan(self.client, vlan200)

    def vlanMaxLearnedAddressesTest(self):
        """
        Verifies disable learn on vlan if mac entries in fdb table
        equals or more then MaxLearnedAddresses attribute
        """
        print("\nvlanMaxLearnedAddressesTest()")

        vlan100 = sai_thrift_create_vlan(self.client,
                                         vlan_id=100,
                                         max_learned_addresses=3)
        vlan_member101 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member102 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member103 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan100,
            bridge_port_id=self.port30_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        vlan200 = sai_thrift_create_vlan(self.client,
                                         vlan_id=200,
                                         max_learned_addresses=3)
        vlan_member201 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan200,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member202 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan200,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member203 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan200,
            bridge_port_id=self.port30_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_port_attribute(
            self.client, self.port26, port_vlan_id=200)
        sai_thrift_set_port_attribute(
            self.client, self.port27, port_vlan_id=200)

        pkt = simple_arp_packet(
            eth_src='00:22:22:33:44:55',
            arp_op=1,  # ARP request
            hw_snd='00:22:22:33:44:55',
            pktlen=100)
        pkt_2 = simple_arp_packet(
            eth_src='00:33:22:33:44:55',
            arp_op=1,  # ARP request
            hw_snd='00:33:22:33:44:55',
            pktlen=100)
        pkt_3 = simple_arp_packet(
            eth_src='00:44:22:33:44:55',
            arp_op=1,  # ARP request
            hw_snd='00:44:22:33:44:55',
            pktlen=100)

        arp_resp = simple_arp_packet(
            eth_dst='00:22:22:33:44:55',
            arp_op=2,  # ARP response
            hw_tgt='00:22:22:33:44:55',
            pktlen=100)
        arp_resp_2 = simple_arp_packet(
            eth_dst='00:33:22:33:44:55',
            arp_op=2,  # ARP response
            hw_tgt='00:33:22:33:44:55',
            pktlen=100)
        arp_resp_3 = simple_arp_packet(
            eth_dst='00:44:22:33:44:55',
            arp_op=2,  # ARP response
            hw_tgt='00:44:22:33:44:55',
            pktlen=100)

        try:

            send_packet(self, self.dev_port26, pkt)
            verify_packets(self, pkt, [self.dev_port27, self.dev_port30])

            send_packet(self, self.dev_port27, arp_resp)
            verify_packet(self, arp_resp, self.dev_port26)

            send_packet(self, self.dev_port26, pkt_2)
            verify_packets(self, pkt_2, [self.dev_port27, self.dev_port30])

            send_packet(self, self.dev_port27, arp_resp_2)
            verify_packet(self, arp_resp_2, self.dev_port26)

            mac_fdb28 = '00:44:22:33:44:55'
            fdb_entry28 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                 mac_address=mac_fdb28,
                                                 bv_id=vlan200)

            sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry28,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=self.port26_bp,
                packet_action=SAI_PACKET_ACTION_FORWARD)

            send_packet(self, self.dev_port26, pkt_3)
            verify_packets(self, pkt_3, [self.dev_port27, self.dev_port30])

            send_packet(self, self.dev_port27, arp_resp_3)
            verify_packets(
                self, arp_resp_3, [self.dev_port26, self.dev_port30])

            sai_thrift_set_vlan_attribute(self.client,
                                          vlan100,
                                          max_learned_addresses=0)

            sai_thrift_set_vlan_attribute(self.client,
                                          vlan200,
                                          max_learned_addresses=0)

            send_packet(self, self.dev_port26, pkt_3)
            verify_packets(self, pkt_3, [self.dev_port27, self.dev_port30])

            send_packet(self, self.dev_port27, arp_resp_3)
            verify_packet(self, arp_resp_3, self.dev_port26)

        finally:
            sai_thrift_flush_fdb_entries(
                self.client,
                bv_id=vlan200,
                entry_type=SAI_FDB_ENTRY_TYPE_DYNAMIC)

            sai_thrift_flush_fdb_entries(
                self.client,
                bv_id=vlan100,
                entry_type=SAI_FDB_ENTRY_TYPE_DYNAMIC)

            sai_thrift_set_port_attribute(
                self.client, self.port26, port_vlan_id=1)
            sai_thrift_set_port_attribute(
                self.client, self.port27, port_vlan_id=1)
            sai_thrift_set_port_attribute(
                self.client, self.port30, port_vlan_id=1)

            sai_thrift_remove_vlan_member(self.client, vlan_member101)
            sai_thrift_remove_vlan_member(self.client, vlan_member102)
            sai_thrift_remove_vlan_member(self.client, vlan_member103)

            sai_thrift_remove_vlan_member(self.client, vlan_member201)
            sai_thrift_remove_vlan_member(self.client, vlan_member202)
            sai_thrift_remove_vlan_member(self.client, vlan_member203)

            sai_thrift_remove_vlan(self.client, vlan200)
            sai_thrift_remove_vlan(self.client, vlan100)
