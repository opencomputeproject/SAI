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
Thrift SAI interface Mirror tests
"""
from sai_thrift.sai_headers import *
from ptf.mask import Mask
from sai_base_test import *


@group("draft")
class MirrorConfigData(SaiHelper):
    """
    Mirror Configuration Class
    """

    def setUp(self):
        super(MirrorConfigData, self).setUp()
        self.mac2 = "00:00:00:00:00:22"
        self.mac3 = "00:00:00:00:00:33"
        mac_action = SAI_PACKET_ACTION_FORWARD
        self.status_success = SAI_STATUS_SUCCESS
        vlan_tag = SAI_VLAN_TAGGING_MODE_TAGGED
        port_type = SAI_BRIDGE_PORT_TYPE_PORT
        # Thrift does not support unsigned int notation, that is why
        # u16 is signed number and there is a need to convert values
        # greater than 32767 (15-bit max value) as follows:
        # val = MIN_SIGNED_INT_VAL + X - MAX_SIGNED_INT_VAL - 1
        # where X - the number exceeds 32767 (MAX_SIGNED_INT_VAL)
        # MIN_SIGNED_INT_VAL = -32768
        self.gre_proto_type_0x88be = -30530
        self.vlan_tpid_0x8100 = -32512
#        +------------+------------+-----------+-------------+
#        |    LAG     |   VLAN ID  |   Port    | VLAN member |
#        +------------+------------+-----------+-------------+
#        |            |   vlan40   |   port24  | vlan_mem41  |
#        |            |            |   port25  | vlan_mem42  |
#        |            |            |   port26  | vlan_mem43  |
#        |            |            |   port27  | vlan_mem44  |
#        |   lag10    |            |           | vlan_mem041 |
#        |  lag10_upd |            |           | vlan_mem042 |
#        +------------+------------+-----------+-------------+
#        |            |   vlan50   |   port28  | vlan_mem51  |
#        |            |            |   port29  | vlan_mem52  |
#        |            |            |   port30  | vlan_mem53  |
#        |            |            |   port31  | vlan_mem54  |
#        |   lag11    |            |           | vlan_mem051 |
#        |  lag11_upd |            |           | vlan_mem052 |
#        +------------+------------+-----------+-------------+

        # Port configuration
        self.bridge_ports = []
        self.port24_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=port_type)
        self.assertNotEqual(self.port24_bp, 0)
        self.bridge_ports.append(self.port24_bp)
        self.port25_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port25,
            type=port_type)
        self.assertNotEqual(self.port25_bp, 0)
        self.bridge_ports.append(self.port25_bp)
        self.port26_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port26,
            type=port_type)
        self.assertNotEqual(self.port26_bp, 0)
        self.bridge_ports.append(self.port26_bp)
        self.port27_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port27,
            type=port_type)
        self.assertNotEqual(self.port27_bp, 0)
        self.bridge_ports.append(self.port27_bp)
        self.port28_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port28,
            type=port_type)
        self.assertNotEqual(self.port28_bp, 0)
        self.bridge_ports.append(self.port28_bp)
        self.port29_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port29,
            type=port_type)
        self.assertNotEqual(self.port29_bp, 0)
        self.bridge_ports.append(self.port29_bp)
        self.port30_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port30,
            type=port_type)
        self.assertNotEqual(self.port30_bp, 0)
        self.bridge_ports.append(self.port30_bp)
        self.port31_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port31,
            type=port_type)
        self.assertNotEqual(self.port31_bp, 0)
        self.bridge_ports.append(self.port31_bp)

        # VLAN configuration
        self.vlan40 = sai_thrift_create_vlan(self.client, vlan_id=40)
        self.assertNotEqual(self.vlan40, 0)
        self.vlan50 = sai_thrift_create_vlan(self.client, vlan_id=50)
        self.assertNotEqual(self.vlan50, 0)
        # VLAN members
        self.vlan_members = []
        self.vlan_member41 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan40,
            bridge_port_id=self.port24_bp,
            vlan_tagging_mode=vlan_tag)
        self.assertNotEqual(self.vlan_member41, 0)
        self.vlan_members.append(self.vlan_member41)
        self.vlan_member42 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan40,
            bridge_port_id=self.port25_bp,
            vlan_tagging_mode=vlan_tag)
        self.assertNotEqual(self.vlan_member42, 0)
        self.vlan_members.append(self.vlan_member42)
        self.vlan_member43 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan40,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=vlan_tag)
        self.assertNotEqual(self.vlan_member43, 0)
        self.vlan_members.append(self.vlan_member43)
        self.vlan_member44 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan40,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=vlan_tag)
        self.assertNotEqual(self.vlan_member44, 0)
        self.vlan_members.append(self.vlan_member44)
        self.vlan_member51 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan50,
            bridge_port_id=self.port28_bp,
            vlan_tagging_mode=vlan_tag)
        self.assertNotEqual(self.vlan_member51, 0)
        self.vlan_members.append(self.vlan_member51)
        self.vlan_member52 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan50,
            bridge_port_id=self.port29_bp,
            vlan_tagging_mode=vlan_tag)
        self.assertNotEqual(self.vlan_member52, 0)
        self.vlan_members.append(self.vlan_member52)
        self.vlan_member53 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan50,
            bridge_port_id=self.port30_bp,
            vlan_tagging_mode=vlan_tag)
        self.assertNotEqual(self.vlan_member53, 0)
        self.vlan_members.append(self.vlan_member53)
        self.vlan_member54 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan50,
            bridge_port_id=self.port31_bp,
            vlan_tagging_mode=vlan_tag)
        self.assertNotEqual(self.vlan_member54, 0)
        self.vlan_members.append(self.vlan_member54)

        # FDB configuration
        self.fdb_entries = []
        self.fdb_entry1 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                 mac_address=self.mac2,
                                                 bv_id=self.vlan40)
        status = sai_thrift_create_fdb_entry(self.client,
                                             self.fdb_entry1,
                                             type=SAI_FDB_ENTRY_TYPE_STATIC,
                                             bridge_port_id=self.port25_bp,
                                             packet_action=mac_action)
        self.assertEqual(status, self.status_success)
        self.fdb_entries.append(self.fdb_entry1)
        self.fdb_entry2 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                 mac_address=self.mac3,
                                                 bv_id=self.vlan40)
        status = sai_thrift_create_fdb_entry(self.client,
                                             self.fdb_entry2,
                                             type=SAI_FDB_ENTRY_TYPE_STATIC,
                                             bridge_port_id=self.port26_bp,
                                             packet_action=mac_action)
        self.assertEqual(status, self.status_success)
        self.fdb_entries.append(self.fdb_entry2)

        self.fdb_entry3 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                 mac_address=self.mac2,
                                                 bv_id=self.vlan50)
        status = sai_thrift_create_fdb_entry(self.client,
                                             self.fdb_entry3,
                                             type=SAI_FDB_ENTRY_TYPE_STATIC,
                                             bridge_port_id=self.port29_bp,
                                             packet_action=mac_action)
        self.assertEqual(status, self.status_success)
        self.fdb_entries.append(self.fdb_entry3)
        self.fdb_entry4 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                 mac_address=self.mac3,
                                                 bv_id=self.vlan50)
        status = sai_thrift_create_fdb_entry(self.client,
                                             self.fdb_entry4,
                                             type=SAI_FDB_ENTRY_TYPE_STATIC,
                                             bridge_port_id=self.port30_bp,
                                             packet_action=mac_action)
        self.assertEqual(status, self.status_success)
        self.fdb_entries.append(self.fdb_entry4)

        # Port attributes
        self.ports = []
        sai_thrift_set_port_attribute(self.client,
                                      self.port24,
                                      port_vlan_id=40)
        self.ports.append(self.port24)
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      port_vlan_id=40)
        self.ports.append(self.port25)
        sai_thrift_set_port_attribute(self.client,
                                      self.port26,
                                      port_vlan_id=40)
        self.ports.append(self.port26)
        sai_thrift_set_port_attribute(self.client,
                                      self.port27,
                                      port_vlan_id=40)
        self.ports.append(self.port27)
        sai_thrift_set_port_attribute(self.client,
                                      self.port28,
                                      port_vlan_id=50)
        self.ports.append(self.port28)
        sai_thrift_set_port_attribute(self.client,
                                      self.port29,
                                      port_vlan_id=50)
        self.ports.append(self.port29)
        sai_thrift_set_port_attribute(self.client,
                                      self.port30,
                                      port_vlan_id=50)
        self.ports.append(self.port30)
        sai_thrift_set_port_attribute(self.client,
                                      self.port31,
                                      port_vlan_id=50)
        self.ports.append(self.port31)

        # L3 layer configuration
        self.vr_id = sai_thrift_create_virtual_router(self.client,
                                                      admin_v4_state=True,
                                                      admin_v6_state=True)

        # Neighbor egress configuration

        # Egress RIF port11
        dst_ip = "172.16.1.1"
        dst_mask_ip = "172.16.1.1/24"
        self.neighbor_dmac = "00:11:22:33:44:55"
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.port11_rif,
            sai_ipaddress(dst_ip))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry1,
                                         dst_mac_address=self.neighbor_dmac)

        self.nhop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port11_rif,
            ip=sai_ipaddress(dst_ip))

        # Egress RIF port12
        dst_ip_egr = "192.168.0.1"
        dst_mask_ip_egr = "192.168.0.1/24"
        self.neighbor_dmac_egr = "00:55:44:33:22:11"
        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.port12_rif,
            sai_ipaddress(dst_ip_egr))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry2,
            dst_mac_address=self.neighbor_dmac_egr)

        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port12_rif,
            ip=sai_ipaddress(dst_ip_egr))

        # Route configuration
        self.route = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix(dst_mask_ip),
            vr_id=self.vr_id)
        status = sai_thrift_create_route_entry(self.client,
                                               self.route,
                                               next_hop_id=self.nhop)
        self.assertEqual(status, self.status_success)

        self.route2 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix(dst_mask_ip_egr),
            vr_id=self.vr_id)
        status = sai_thrift_create_route_entry(self.client,
                                               self.route2,
                                               next_hop_id=self.nhop2)
        self.assertEqual(status, self.status_success)

        # IP for ERSPAN ingress mirroring
        src_ipv4 = sai_thrift_ip_addr_t(ip4="17.18.19.0")
        dst_ipv4 = sai_thrift_ip_addr_t(ip4="33.19.20.0")
        addr_fam = SAI_IP_ADDR_FAMILY_IPV4
        self.src_ip_addr = sai_thrift_ip_address_t(addr_family=addr_fam,
                                                   addr=src_ipv4)
        self.dst_ip_addr = sai_thrift_ip_address_t(addr_family=addr_fam,
                                                   addr=dst_ipv4)

        # IP for ERSPAN egress mirroring
        src_ipv4_egr = sai_thrift_ip_addr_t(ip4="33.19.20.0")
        dst_ipv4_egr = sai_thrift_ip_addr_t(ip4="17.18.19.0")
        self.src_ip_addr_egr = sai_thrift_ip_address_t(addr_family=addr_fam,
                                                       addr=src_ipv4_egr)
        self.dst_ip_addr_egr = sai_thrift_ip_address_t(addr_family=addr_fam,
                                                       addr=dst_ipv4_egr)

        # LAG configuration
        self.lags = []
        self.lag10 = sai_thrift_create_lag(self.client)
        self.assertNotEqual(self.lag10, 0)
        self.lags.append(self.lag10)
        self.lag10_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag10,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.assertNotEqual(self.lag10_bp, 0)
        self.bridge_ports.append(self.lag10_bp)
        self.vlan_member041 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan40,
            bridge_port_id=self.lag10_bp,
            vlan_tagging_mode=vlan_tag)
        self.assertNotEqual(self.vlan_member041, 0)
        self.vlan_members.append(self.vlan_member041)
        self.lag10_upd = sai_thrift_create_lag(self.client)
        self.assertNotEqual(self.lag10_upd, 0)
        self.lags.append(self.lag10_upd)
        self.lag10_bp_upd = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag10_upd,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.assertNotEqual(self.lag10_bp_upd, 0)
        self.bridge_ports.append(self.lag10_bp_upd)
        self.vlan_member042 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan40,
            bridge_port_id=self.lag10_bp_upd,
            vlan_tagging_mode=vlan_tag)
        self.assertNotEqual(self.vlan_member042, 0)
        self.vlan_members.append(self.vlan_member042)

        self.lag11 = sai_thrift_create_lag(self.client)
        self.assertNotEqual(self.lag11, 0)
        self.lags.append(self.lag11)
        self.lag11_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag11,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.assertNotEqual(self.lag11_bp, 0)
        self.bridge_ports.append(self.lag11_bp)
        self.vlan_member051 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan50,
            bridge_port_id=self.lag11_bp,
            vlan_tagging_mode=vlan_tag)
        self.assertNotEqual(self.vlan_member051, 0)
        self.vlan_members.append(self.vlan_member051)

        self.lag11_upd = sai_thrift_create_lag(self.client)
        self.assertNotEqual(self.lag11_upd, 0)
        self.lags.append(self.lag11_upd)
        self.lag11_bp_upd = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag11_upd,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.assertNotEqual(self.lag11_bp_upd, 0)
        self.bridge_ports.append(self.lag11_bp_upd)
        self.vlan_member052 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan50,
            bridge_port_id=self.lag11_bp_upd,
            vlan_tagging_mode=vlan_tag)
        self.assertNotEqual(self.vlan_member052, 0)
        self.vlan_members.append(self.vlan_member052)

        # Policer configuration
        self.pol_id = sai_thrift_create_policer(
            self.client,
            meter_type=SAI_METER_TYPE_PACKETS,
            mode=SAI_POLICER_MODE_SR_TCM,
            cir=1,
            pir=1,
            green_packet_action=SAI_PACKET_ACTION_FORWARD,
            red_packet_action=SAI_PACKET_ACTION_FORWARD)

    def runTest(self):
        self.localIngressPortMirroringTest()
        self.localEgressPortMirroringTest()
        self.localIngressLagMirroringTest()
        self.localEgressLagMirroringTest()
        self.ingressEgressMirrorSessionTest()
        self.aclIngressEgressMirrorSessionTest()
        self.egressMirrorDropOnIngress()
        self.mirrorDroppedPacketIngressTest()
        self.mirrorDroppedPacketEgressTest()
        self.mirrorSessionTrafficClassTest()
        self.erspanMirrorSessionTrafficClassTest()
        self.localAclIngressLagMirroringTest()
        self.localAclEgressLagMirroringTest()
        self.aclMirrorDroppedPacketIngressTest()
        self.aclMirrorDroppedPacketEgressTest()
        self.erspanAclEgressGreProto0x22ebTest()
        self.erspanPortMirroringTest()
        self.erspanLagMirroringTest()
        self.erspanVlanPortMirroringTest()
        self.ingressMirrorPolicingTest()
        self.egressMirrorPolicingTest()
        self.erspanIngressMirrorPolicingTest()
        self.erspanEgressMirrorPolicingTest()

    def tearDown(self):
        sai_thrift_remove_policer(self.client, self.pol_id)
        sai_thrift_remove_route_entry(self.client, self.route2)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_route_entry(self.client, self.route)
        sai_thrift_remove_next_hop(self.client, self.nhop)
        sai_thrift_remove_virtual_router(self.client, self.vr_id)

        for vlan_member in self.vlan_members:
            sai_thrift_remove_vlan_member(self.client, vlan_member)

        for fdb_entry in self.fdb_entries:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)

        for port in self.ports:
            sai_thrift_set_port_attribute(self.client, port, port_vlan_id=1)

        sai_thrift_remove_vlan(self.client, self.vlan50)
        sai_thrift_remove_vlan(self.client, self.vlan40)

        for bridge_port in self.bridge_ports:
            sai_thrift_remove_bridge_port(self.client, bridge_port)

        for lag in self.lags:
            sai_thrift_remove_lag(self.client, lag)

        super(MirrorConfigData, self).tearDown()

    def localIngressPortMirroringTest(self):
        """
        Checking local ingress port mirroring
        """
        print("\nLocal Ingress Mirroring Test - Monitor = PORT")
        span_id_ingress = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id_ingress])
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      ingress_mirror_session=obj_list)
        try:
            pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                    eth_src="00:00:00:00:00:22",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64)

            exp_pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                        eth_src="00:00:00:00:00:22",
                                        ip_dst="10.0.0.1",
                                        dl_vlan_enable=True,
                                        vlan_vid=40,
                                        ip_id=101,
                                        ip_ttl=64)
            print("\tChecking Ingress Local Port Mirroring")
            print("\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            verify_packets(self,
                           exp_pkt,
                           ports=[self.dev_port24, self.dev_port26])
            print("\tTest completed successfully - packet received")
        finally:
            obj_list = sai_thrift_object_list_t(count=0,
                                                idlist=[span_id_ingress])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          ingress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, span_id_ingress)

    def localEgressPortMirroringTest(self):
        """
        Checking local egress port mirroring
        """
        print("\nLocal Egress Mirroring Test - Monitor = PORT")
        span_id_egress = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id_egress])
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      egress_mirror_session=obj_list)
        try:
            pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                    eth_src="00:00:00:00:00:33",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64,
                                    pktlen=104)

            exp_pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                        eth_src="00:00:00:00:00:33",
                                        ip_dst="10.0.0.1",
                                        dl_vlan_enable=True,
                                        vlan_vid=40,
                                        ip_id=101,
                                        ip_ttl=64,
                                        pktlen=104)
            print("\tChecking Egress Local Port Mirroring")
            print("\tSending packet PORT26 -> PORT25")
            mask = Mask(exp_pkt)
            mask.set_do_not_care_scapy(ptf.packet.IP, "id")
            mask.set_do_not_care_scapy(ptf.packet.IP, "chksum")
            send_packet(self, self.dev_port26, pkt)
            verify_each_packet_on_each_port(self,
                                            [mask, pkt],
                                            ports=[self.dev_port24,
                                                   self.dev_port25])
            print("\tTest completed successfully - packet received")
        finally:
            obj_list = sai_thrift_object_list_t(count=0,
                                                idlist=[span_id_egress])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          egress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, span_id_egress)

    def localIngressLagMirroringTest(self):
        """
        Checking local ingress lag mirroring
        """
        print("\nLocal Ingress Mirroring Test - Monitor = LAG")
        span_id_ingress = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.lag10,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id_ingress])
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      ingress_mirror_session=obj_list)
        pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                eth_src="00:00:00:00:00:22",
                                ip_dst="10.0.0.1",
                                dl_vlan_enable=True,
                                vlan_vid=40,
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                    eth_src="00:00:00:00:00:22",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64)

        try:
            print("\tTest variant 1 - monitor: empty LAG")
            print("\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            # Packet will not be mirrored
            verify_packet(self, exp_pkt, self.dev_port26)
            verify_no_other_packets(self, timeout=1)
            print("\t\tTest completed successfully - packet not mirrored")

            print("\tTest variant 2 - monitor: non-empty LAG, two lag members")
            lag_member24 = sai_thrift_create_lag_member(self.client,
                                                        lag_id=self.lag10,
                                                        port_id=self.port24)
            self.assertNotEqual(lag_member24, 0)
            lag_member27 = sai_thrift_create_lag_member(self.client,
                                                        lag_id=self.lag10,
                                                        port_id=self.port27)
            self.assertNotEqual(lag_member27, 0)

            print("\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [exp_pkt, exp_pkt],
                [[self.dev_port26], [self.dev_port24, self.dev_port27]])
            print("\t\tTest completed successfully - packet received")

            print("\tTest variant 3 - monitor non-empty LAG: one lag member")
            status = sai_thrift_remove_lag_member(self.client, lag_member24)
            self.assertEqual(status, self.status_success)

            print("\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            verify_each_packet_on_multiple_port_lists(self,
                                                      [exp_pkt, exp_pkt],
                                                      [[self.dev_port26],
                                                       [self.dev_port27]])
            print("\t\tTest completed successfully - packet received")

            print("\tTest variant 4 - monitor empty LAG: no lag members")
            status = sai_thrift_remove_lag_member(self.client, lag_member27)
            self.assertEqual(status, self.status_success)

            print("\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            # Packet will not be mirrored
            verify_packet(self, exp_pkt, self.dev_port26)
            verify_no_other_packets(self, timeout=1)
            print("\t\tTest completed successfully - packet not mirrored")

            print("\tTest variant 5 - updated monitor: new LAG")
            status = sai_thrift_set_mirror_session_attribute(
                self.client,
                span_id_ingress,
                monitor_port=self.lag10_upd)
            self.assertEqual(status, self.status_success)
            print("\t\tTest variant 5a - monitor: empty LAG")
            print("\t\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            # Packet will not be mirrored
            verify_packet(self, exp_pkt, self.dev_port26)
            verify_no_other_packets(self, timeout=1)
            print("\t\t\tTest completed successfully - packet not mirrored")

            print("\t\tTest variant 5b - monitor: non-empty LAG,"
                  " two lag members")
            lag_member24 = sai_thrift_create_lag_member(self.client,
                                                        lag_id=self.lag10_upd,
                                                        port_id=self.port24)
            self.assertNotEqual(lag_member24, 0)
            lag_member27 = sai_thrift_create_lag_member(self.client,
                                                        lag_id=self.lag10_upd,
                                                        port_id=self.port27)
            self.assertNotEqual(lag_member27, 0)
            print("\t\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [exp_pkt, exp_pkt],
                [[self.dev_port26], [self.dev_port24, self.dev_port27]])
            print("\t\t\tTest completed successfully - packet received")

            print("\t\tTest variant 5c - monitor non-empty LAG:"
                  " one lag member")
            status = sai_thrift_remove_lag_member(self.client, lag_member24)
            self.assertEqual(status, self.status_success)
            self.assertNotEqual(lag_member27, 0)

            print("\t\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [exp_pkt, exp_pkt],
                [[self.dev_port26], [self.dev_port27]])
            print("\t\t\tTest completed successfully - packet received")

            print("\t\tTest variant 5d - monitor empty LAG: no lag members")
            status = sai_thrift_remove_lag_member(self.client, lag_member27)
            self.assertEqual(status, self.status_success)

            print("\t\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            # Packet will not be mirrored
            verify_packet(self, exp_pkt, self.dev_port26)
            verify_no_other_packets(self, timeout=1)
            print("\t\t\tTest completed successfully - packet not mirrored")

            print("\tTest variant 6 - updated monitor: new port")
            status = sai_thrift_set_mirror_session_attribute(
                self.client, span_id_ingress, monitor_port=self.port24)
            self.assertEqual(status, self.status_success)
            print("\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            verify_packets(self,
                           exp_pkt,
                           ports=[self.dev_port24, self.dev_port26])
            print("\t\tTest completed successfully - packet received")

            print("\tTest variant 7 - updated monitor: previously created LAG")
            status = sai_thrift_set_mirror_session_attribute(
                self.client, span_id_ingress, monitor_port=self.lag10)
            self.assertEqual(status, self.status_success)
            print("\t\tTest variant 7a - monitor: empty LAG")
            print("\t\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            # Packet will not be mirrored
            verify_packet(self, exp_pkt, self.dev_port26)
            verify_no_other_packets(self, timeout=1)
            print("\t\t\tTest completed successfully - packet not mirrored")

            print("\t\tTest variant 7b - monitor: non-empty LAG,"
                  " two lag members")
            lag_member24 = sai_thrift_create_lag_member(self.client,
                                                        lag_id=self.lag10,
                                                        port_id=self.port24)
            self.assertNotEqual(lag_member24, 0)
            lag_member27 = sai_thrift_create_lag_member(self.client,
                                                        lag_id=self.lag10,
                                                        port_id=self.port27)
            self.assertNotEqual(lag_member27, 0)
            print("\t\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [exp_pkt, exp_pkt],
                [[self.dev_port26], [self.dev_port24, self.dev_port27]])
            print("\t\t\tTest completed successfully - packet received")

            print("\t\tTest variant 7c - monitor non-empty LAG:"
                  " one lag member")
            sai_thrift_remove_lag_member(self.client, lag_member24)

            print("\t\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [exp_pkt, exp_pkt],
                [[self.dev_port26], [self.dev_port27]])
            print("\t\t\tTest completed successfully - packet received")

            print("\t\tTest variant 7d - monitor empty LAG: no lag members")
            status = sai_thrift_remove_lag_member(self.client, lag_member27)
            self.assertEqual(status, self.status_success)

            print("\t\t\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            # Packet will not be mirrored
            verify_packet(self, exp_pkt, self.dev_port26)
            verify_no_other_packets(self, timeout=1)
            print("\t\t\tTest completed successfully - packet not mirrored")

        finally:
            obj_list = sai_thrift_object_list_t(count=0,
                                                idlist=[span_id_ingress])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          ingress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, span_id_ingress)

    def localEgressLagMirroringTest(self):
        """
        Checking local egress lag mirroring
        """
        print("\nLocal Egress Mirroring Test - Monitor = LAG")
        span_id_egress = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.lag10,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id_egress])
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      egress_mirror_session=obj_list)
        pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                eth_src="00:00:00:00:00:33",
                                ip_dst="10.0.0.1",
                                dl_vlan_enable=True,
                                vlan_vid=40,
                                ip_id=101,
                                ip_ttl=64,
                                pktlen=104)

        exp_pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                    eth_src="00:00:00:00:00:33",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64,
                                    pktlen=104)
        mask = Mask(exp_pkt)
        mask.set_do_not_care_scapy(ptf.packet.IP, "id")
        mask.set_do_not_care_scapy(ptf.packet.IP, "chksum")
        try:
            print("\tTest variant 1 - monitor: empty LAG")
            print("\t\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            # Packet will not be mirrored
            verify_packet(self, exp_pkt, self.dev_port25)
            verify_no_other_packets(self, timeout=1)
            print("\t\tTest completed successfully - packet not mirrored")

            print("\tTest variant 2 - monitor non-empty LAG: two lag members")
            lag_member24 = sai_thrift_create_lag_member(self.client,
                                                        lag_id=self.lag10,
                                                        port_id=self.port24)
            self.assertNotEqual(lag_member24, 0)
            lag_member27 = sai_thrift_create_lag_member(self.client,
                                                        lag_id=self.lag10,
                                                        port_id=self.port27)
            self.assertNotEqual(lag_member27, 0)
            print("\t\tSending packet PORT26 -> PORT25")

            send_packet(self, self.dev_port26, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [mask, exp_pkt],
                [[self.dev_port25], [self.dev_port24, self.dev_port26]])
            print("\t\tTest completed successfully - packet received")

            print("\tTest variant 3 - monitor non-empty LAG: one lag member")
            status = sai_thrift_remove_lag_member(self.client, lag_member27)
            self.assertEqual(status, self.status_success)

            print("\t\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            verify_each_packet_on_each_port(
                self, [mask, exp_pkt], ports=[self.dev_port25,
                                              self.dev_port24])
            print("\t\tTest completed successfully - packet received")

            print("\tTest variant 4 - monitor non-empty LAG: no lag members")
            status = sai_thrift_remove_lag_member(self.client, lag_member24)
            self.assertEqual(status, self.status_success)

            print("\t\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            # Packet will not be mirrored
            verify_packet(self, exp_pkt, self.dev_port25)
            verify_no_other_packets(self, timeout=1)
            print("\t\tTest completed successfully - packet not mirrored")

            print("\tTest variant 5 - monitor new LAG")
            status = sai_thrift_set_mirror_session_attribute(
                self.client, span_id_egress, monitor_port=self.lag10_upd)
            self.assertEqual(status, self.status_success)

            print("\t\tTest variant 5a - monitor: empty LAG")
            print("\t\t\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            # Packet will not be mirrored
            verify_packet(self, exp_pkt, self.dev_port25)
            verify_no_other_packets(self, timeout=1)
            print("\t\t\tTest completed successfully - packet not mirrored")

            print("\t\tTest variant 5b - monitor: non-empty LAG,"
                  " two lag members")
            lag_member24 = sai_thrift_create_lag_member(self.client,
                                                        lag_id=self.lag10_upd,
                                                        port_id=self.port24)
            self.assertNotEqual(lag_member24, 0)
            lag_member27 = sai_thrift_create_lag_member(self.client,
                                                        lag_id=self.lag10_upd,
                                                        port_id=self.port27)
            self.assertNotEqual(lag_member27, 0)
            print("\t\t\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [mask, exp_pkt],
                [[self.dev_port25], [self.dev_port24, self.dev_port27]])
            print("\t\t\tTest completed successfully - packet received")

            print("\t\tTest variant 5c - monitor non-empty LAG:"
                  " one lag member")
            status = sai_thrift_remove_lag_member(self.client, lag_member24)
            self.assertEqual(status, self.status_success)

            print("\t\t\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [mask, exp_pkt], [[self.dev_port25], [self.dev_port27]])
            print("\t\t\tTest completed successfully - packet received")

            print("\t\tTest variant 5d - monitor empty LAG: no lag members")
            status = sai_thrift_remove_lag_member(self.client, lag_member27)
            self.assertEqual(status, self.status_success)

            print("\t\t\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)

            verify_packet(self, exp_pkt, self.dev_port25)
            verify_no_other_packets(self, timeout=1)
            print("\t\t\tTest completed successfully - packet not mirrored")

            print("\tTest variant 6 - monitor new port")
            status = sai_thrift_set_mirror_session_attribute(
                self.client, span_id_egress, monitor_port=self.port24)
            self.assertEqual(status, self.status_success)

            print("\t\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            verify_packets(self,
                           exp_pkt,
                           ports=[self.dev_port25, self.dev_port24])
            print("\t\tTest completed successfully - packet received")

            print("\tTest variant 7 - updated monitor: previously created LAG")
            status = sai_thrift_set_mirror_session_attribute(
                self.client, span_id_egress, monitor_port=self.lag10)
            self.assertEqual(status, self.status_success)

            print("\t\tTest variant 7a - monitor: empty LAG")
            print("\t\t\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            # Packet will not be mirrored
            verify_packet(self, exp_pkt, self.dev_port25)
            verify_no_other_packets(self, timeout=1)
            print("\t\t\tTest completed successfully - packet not mirrored")

            print("\t\tTest variant 7b - monitor: non-empty LAG,"
                  " two lag members")
            lag_member24 = sai_thrift_create_lag_member(self.client,
                                                        lag_id=self.lag10,
                                                        port_id=self.port24)
            self.assertNotEqual(lag_member24, 0)
            lag_member27 = sai_thrift_create_lag_member(self.client,
                                                        lag_id=self.lag10,
                                                        port_id=self.port27)
            self.assertNotEqual(lag_member27, 0)
            print("\t\t\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [mask, exp_pkt],
                [[self.dev_port25], [self.dev_port24, self.dev_port27]])
            print("\t\t\tTest completed successfully - packet received")

            print("\t\tTest variant 7c - monitor non-empty LAG:"
                  " one lag member")
            status = sai_thrift_remove_lag_member(self.client, lag_member24)
            self.assertEqual(status, self.status_success)

            print("\t\t\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [mask, exp_pkt], [[self.dev_port25], [self.dev_port27]])
            print("\t\t\tTest completed successfully - packet received")

            print("\t\tTest variant 7d - monitor empty LAG: no lag members")
            status = sai_thrift_remove_lag_member(self.client, lag_member27)
            self.assertEqual(status, self.status_success)

            print("\t\t\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)

            verify_packet(self, exp_pkt, self.dev_port25)
            verify_no_other_packets(self, timeout=1)
            print("\t\t\tTest completed successfully - packet not mirrored")

        finally:
            obj_list = sai_thrift_object_list_t(count=0,
                                                idlist=[span_id_egress])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          egress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, span_id_egress)

    def ingressEgressMirrorSessionTest(self):
        """
        Checking ingress and egress mirroring in single session
        """
        print("\nIngress Egress Port Mirroring Test")
        span_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id])
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      ingress_mirror_session=obj_list)
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      egress_mirror_session=obj_list)

        pkt_ing = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                    eth_src="00:00:00:00:00:22",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64,
                                    pktlen=104)
        pkt_egr = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                    eth_src="00:00:00:00:00:33",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64,
                                    pktlen=104)
        mask = Mask(pkt_egr)
        mask.set_do_not_care_scapy(ptf.packet.IP, "id")
        mask.set_do_not_care_scapy(ptf.packet.IP, "chksum")

        try:
            print("\tSending packet for ingress port mirroring check...")
            send_packet(self, self.dev_port25, pkt_ing)
            verify_each_packet_on_each_port(
                self, [pkt_ing, pkt_ing], ports=[self.dev_port24,
                                                 self.dev_port26])
            print("\tPacket received for INGRESS mirroring")

            print("\tSending packet for egress port mirroring check...")
            send_packet(self, self.dev_port26, pkt_egr)
            verify_each_packet_on_each_port(
                self, [mask, pkt_egr], ports=[self.dev_port24,
                                              self.dev_port25])
            print("\tPacket received for EGRESS mirroring")
            print("\tTest completed successfully - packets received")

        finally:
            obj_list = sai_thrift_object_list_t(count=0, idlist=[span_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          ingress_mirror_session=obj_list)
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          egress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, span_id)

    def egressMirrorDropOnIngress(self):
        """
        Checking the lack of packet mirroring by egress mirror
        when packet is dropped on ingress
        """
        print("\nNot Mirror Dropped Packet On Ingress By Egress Mirror Test")
        span_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id])
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      egress_mirror_session=obj_list)
        # Packet will be dropped due to wrong vlan_id
        pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                eth_src="00:00:00:00:00:33",
                                ip_dst="10.0.0.1",
                                dl_vlan_enable=True,
                                vlan_vid=50,
                                ip_id=101,
                                ip_ttl=64,
                                pktlen=104)

        try:
            print("\tSending packet with destination egress PORT26")
            send_packet(self, self.dev_port26, pkt)
            verify_no_other_packets(self, timeout=1)
            print("\tTest completed successfully - packet dropped"
                  " and not mirrored")
        finally:
            obj_list = sai_thrift_object_list_t(count=0, idlist=[span_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          egress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, span_id)

    def mirrorDroppedPacketIngressTest(self):
        """
        Checking packet mirroring if packet is dropped on ingress
        """
        print("\nMirror Dropped Packet Ingress Test")
        span_id_ingress = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id_ingress])
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      ingress_mirror_session=obj_list)

        # Packet will be dropped in ingress due to wrong vlan_id
        pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                eth_src="00:00:00:00:00:22",
                                ip_dst="10.0.0.1",
                                dl_vlan_enable=True,
                                vlan_vid=50,
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                    eth_src="00:00:00:00:00:22",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=50,
                                    ip_id=101,
                                    ip_ttl=64)

        try:
            print("\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            verify_packet(self, exp_pkt, self.dev_port24)
            verify_no_other_packets(self, timeout=1)
            print("\tTest completed successfully - packet dropped"
                  " and mirrored on ingress")
        finally:
            obj_list = sai_thrift_object_list_t(count=0,
                                                idlist=[span_id_ingress])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          ingress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, span_id_ingress)

    def mirrorDroppedPacketEgressTest(self):
        """
        Checking the lack of mirroring if the packet is dropped on egress
        """
        print("\nMirror Dropped Packet Egress Test")
        span_id_egress = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id_egress])
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      egress_mirror_session=obj_list)

        # Packet will be dropped in egress due to wrong vlan_id
        pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                eth_src="00:00:00:00:00:33",
                                ip_dst="10.0.0.1",
                                dl_vlan_enable=True,
                                vlan_vid=50,
                                ip_id=101,
                                ip_ttl=64)

        try:
            print("\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            # Egress mirror should not include dropped packets
            verify_no_other_packets(self, timeout=1)
            print("\tTest completed successfully - packet dropped"
                  " and not mirrored on egress")
        finally:
            obj_list = sai_thrift_object_list_t(count=0,
                                                idlist=[span_id_egress])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          egress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, span_id_egress)

    def mirrorSessionTrafficClassTest(self):
        """
        Checking egress mirroring with Traffic Class attribute enabled.
        """
        print("\nMirror Session Traffic Class Test")
        traffic_class = 6
        span_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL,
            tc=traffic_class)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id])

        queue_list = sai_thrift_object_list_t(
            count=10)
        attr = sai_thrift_get_port_attribute(self.client,
                                             self.port24,
                                             qos_queue_list=queue_list)

        for _, value in enumerate(attr["qos_queue_list"].idlist):
            q_attr = sai_thrift_get_queue_attribute(self.client, value,
                                                    index=True)
            if q_attr["index"] == traffic_class:
                queue_id = value
                break

        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      egress_mirror_session=obj_list)

        pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                eth_src="00:00:00:00:00:33",
                                ip_dst="10.0.0.1",
                                dl_vlan_enable=True,
                                vlan_vid=40,
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                    eth_src="00:00:00:00:00:33",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64)
        try:
            print("\tSending packet PORT26 -> PORT25")
            sai_thrift_clear_queue_stats(self.client, queue_id)
            stats = sai_thrift_get_queue_stats(self.client, queue_id)
            pkt_cnt_before = stats["SAI_QUEUE_STAT_PACKETS"]

            send_packet(self, self.dev_port26, pkt)
            verify_packets(self,
                           exp_pkt,
                           ports=[self.dev_port24, self.dev_port25])
            stats = sai_thrift_get_queue_stats(self.client, queue_id)
            pkt_cnt_after = stats["SAI_QUEUE_STAT_PACKETS"]
            assert(pkt_cnt_after == pkt_cnt_before + 1), ("Mirrored packet "
                                                          "does not get to the"
                                                          " correct queue")

            print("\tTest completed successfully - packets received")
        finally:
            obj_list = sai_thrift_object_list_t(count=0, idlist=[span_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          egress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, span_id)

    def localAclIngressLagMirroringTest(self):
        """
        Checking local ACL ingress lag mirroring
        """
        print("\nLocal ACL Ingress Mirroring Test - Monitor = LAG")

        span_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.lag10,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)

        # LAG members
        lag_member24 = sai_thrift_create_lag_member(self.client,
                                                    lag_id=self.lag10,
                                                    port_id=self.port24)
        self.assertNotEqual(lag_member24, 0)
        lag_member27 = sai_thrift_create_lag_member(self.client,
                                                    lag_id=self.lag10,
                                                    port_id=self.port27)
        self.assertNotEqual(lag_member27, 0)

        # ACL configuration
        action_type = [SAI_ACL_ACTION_TYPE_MIRROR_INGRESS]
        action = sai_thrift_s32_list_t(count=len(action_type),
                                       int32list=action_type)
        bind_point = [SAI_ACL_BIND_POINT_TYPE_PORT]
        acl_bpt_list = sai_thrift_s32_list_t(count=len(bind_point),
                                             int32list=bind_point)
        acl_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=SAI_ACL_STAGE_INGRESS,
            acl_action_type_list=action,
            acl_bind_point_type_list=acl_bpt_list,
            field_dst_ip=True)

        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      ingress_acl=acl_table_id)

        dst_ip = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="10.0.0.1"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.0"))

        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id])
        param = sai_thrift_acl_action_parameter_t(objlist=obj_list)
        obj_acl_action_data = sai_thrift_acl_action_data_t(parameter=param)
        acl_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_id,
            action_mirror_ingress=obj_acl_action_data,
            field_dst_ip=dst_ip)

        pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                eth_src="00:00:00:00:00:22",
                                ip_dst="10.0.0.1",
                                dl_vlan_enable=True,
                                vlan_vid=40,
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                    eth_src="00:00:00:00:00:22",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64)
        try:
            print("\tChecking ACL Ingress Local LAG Mirroring")
            print("\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [exp_pkt, exp_pkt],
                [[self.dev_port26], [self.dev_port24, self.dev_port27]])
            print("\tTest completed successfully - packets received")

        finally:
            sai_thrift_remove_acl_entry(self.client, acl_entry_id)
            sai_thrift_object_list_t(count=0, idlist=[span_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          ingress_acl=0)
            sai_thrift_remove_mirror_session(self.client, span_id)
            sai_thrift_remove_acl_table(self.client, acl_table_id)
            sai_thrift_remove_lag_member(self.client, lag_member27)
            sai_thrift_remove_lag_member(self.client, lag_member24)

    def localAclEgressLagMirroringTest(self):
        """
        Checking local ACL egress lag mirroring
        """
        print("\nLocal ACL Egress Mirroring Test - Monitor = LAG")

        span_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.lag10,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)

        # LAG members
        lag_member24 = sai_thrift_create_lag_member(self.client,
                                                    lag_id=self.lag10,
                                                    port_id=self.port24)
        self.assertNotEqual(lag_member24, 0)
        lag_member27 = sai_thrift_create_lag_member(self.client,
                                                    lag_id=self.lag10,
                                                    port_id=self.port27)
        self.assertNotEqual(lag_member27, 0)

        # ACL configuration
        action_type = [SAI_ACL_ACTION_TYPE_MIRROR_EGRESS]
        action = sai_thrift_s32_list_t(count=len(action_type),
                                       int32list=action_type)
        bind_point = [SAI_ACL_BIND_POINT_TYPE_PORT]
        acl_bpt_list = sai_thrift_s32_list_t(count=len(bind_point),
                                             int32list=bind_point)
        acl_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=SAI_ACL_STAGE_EGRESS,
            acl_action_type_list=action,
            acl_bind_point_type_list=acl_bpt_list,
            field_dst_ip=True)

        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      egress_acl=acl_table_id)

        dst_ip = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="10.0.0.1"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.0"))

        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id])
        param = sai_thrift_acl_action_parameter_t(objlist=obj_list)
        obj_acl_action_data = sai_thrift_acl_action_data_t(parameter=param)
        acl_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_id,
            action_mirror_egress=obj_acl_action_data,
            field_dst_ip=dst_ip)

        pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                eth_src="00:00:00:00:00:33",
                                ip_dst="10.0.0.1",
                                dl_vlan_enable=True,
                                vlan_vid=40,
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                    eth_src="00:00:00:00:00:33",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64)

        try:
            print("\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [exp_pkt, exp_pkt],
                [[self.dev_port25], [self.dev_port24, self.dev_port26]])
            print("\tTest completed successfully - packet received")
        finally:
            sai_thrift_remove_acl_entry(self.client, acl_entry_id)
            sai_thrift_object_list_t(count=0, idlist=[span_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          egress_acl=0)
            sai_thrift_remove_acl_table(self.client, acl_table_id)
            sai_thrift_remove_lag_member(self.client, lag_member27)
            sai_thrift_remove_lag_member(self.client, lag_member24)
            sai_thrift_remove_mirror_session(self.client, span_id)

    def aclIngressEgressMirrorSessionTest(self):
        """
        Checking ACL ingress and egress mirroring in a single session
        """
        print("\nACL Ingress Egress Port Mirroring Test")
        span_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)

        # Ingress ACL table
        action_type_ingress = [SAI_ACL_ACTION_TYPE_MIRROR_INGRESS]
        action_ingress = sai_thrift_s32_list_t(count=len(action_type_ingress),
                                               int32list=action_type_ingress)
        bind_point = [SAI_ACL_BIND_POINT_TYPE_PORT]
        acl_bpt_list = sai_thrift_s32_list_t(count=len(bind_point),
                                             int32list=bind_point)
        acl_table_id_ingress = sai_thrift_create_acl_table(
            self.client,
            acl_stage=SAI_ACL_STAGE_INGRESS,
            acl_action_type_list=action_ingress,
            acl_bind_point_type_list=acl_bpt_list,
            field_dst_ip=True)

        # Egress ACL table
        action_type_egress = [SAI_ACL_ACTION_TYPE_MIRROR_EGRESS]
        action_egress = sai_thrift_s32_list_t(count=len(action_type_egress),
                                              int32list=action_type_egress)
        bind_point = [SAI_ACL_BIND_POINT_TYPE_PORT]
        acl_bpt_list = sai_thrift_s32_list_t(count=len(bind_point),
                                             int32list=bind_point)
        acl_table_id_egress = sai_thrift_create_acl_table(
            self.client,
            acl_stage=SAI_ACL_STAGE_EGRESS,
            acl_action_type_list=action_egress,
            acl_bind_point_type_list=acl_bpt_list,
            field_dst_ip=True)

        # ACL configuration for the test
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      ingress_acl=acl_table_id_ingress)

        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      egress_acl=acl_table_id_egress)

        dst_ip = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="10.0.0.1"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.0"))

        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id])
        param = sai_thrift_acl_action_parameter_t(objlist=obj_list)
        obj_acl_action_data = sai_thrift_acl_action_data_t(parameter=param)

        # Ingress ACL entry
        acl_entry_id_ingress = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_id_ingress,
            action_mirror_ingress=obj_acl_action_data,
            field_dst_ip=dst_ip)

        # Egress ACL entry
        acl_entry_id_egress = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_id_egress,
            action_mirror_egress=obj_acl_action_data,
            field_dst_ip=dst_ip)

        pkt_ing = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                    eth_src="00:00:00:00:00:22",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64)
        pkt_egr = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                    eth_src="00:00:00:00:00:33",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64)
        mask = Mask(pkt_egr)
        mask.set_do_not_care_scapy(ptf.packet.IP, "id")
        mask.set_do_not_care_scapy(ptf.packet.IP, "chksum")

        try:
            print("\tSending packet for ACL ingress port mirroring check...")
            send_packet(self, self.dev_port25, pkt_ing)
            verify_each_packet_on_each_port(
                self, [pkt_ing, pkt_ing], ports=[self.dev_port24,
                                                 self.dev_port26])
            print("\tPacket received for ACL INGRESS mirroring")

            print("\tSending packet for ACL egress port mirroring check...")
            send_packet(self, self.dev_port26, pkt_egr)
            verify_each_packet_on_each_port(
                self, [mask, pkt_egr], ports=[self.dev_port24,
                                              self.dev_port25])
            print("\tPacket received for ACL EGRESS mirroring")
            print("\tTest completed successfully - packets received")

        finally:
            sai_thrift_remove_acl_entry(self.client, acl_entry_id_ingress)
            sai_thrift_remove_acl_entry(self.client, acl_entry_id_egress)
            sai_thrift_object_list_t(count=0, idlist=[span_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          ingress_acl=0,
                                          egress_acl=0)
            sai_thrift_remove_acl_table(self.client, acl_table_id_ingress)
            sai_thrift_remove_acl_table(self.client, acl_table_id_egress)
            sai_thrift_remove_mirror_session(self.client, span_id)

    def aclMirrorDroppedPacketIngressTest(self):
        """
        Checking ACL packet mirroring if packet is dropped on ingress
        """
        print("\nACL Mirror Dropped Packet Ingress Test")
        span_id_ingress = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)

        # Ingress ACL table
        action_type_ingress = [SAI_ACL_ACTION_TYPE_MIRROR_INGRESS]
        action_ingress = sai_thrift_s32_list_t(count=len(action_type_ingress),
                                               int32list=action_type_ingress)
        bind_point = [SAI_ACL_BIND_POINT_TYPE_PORT]
        acl_bpt_list = sai_thrift_s32_list_t(count=len(bind_point),
                                             int32list=bind_point)
        acl_table_id_ingress = sai_thrift_create_acl_table(
            self.client,
            acl_stage=SAI_ACL_STAGE_INGRESS,
            acl_action_type_list=action_ingress,
            acl_bind_point_type_list=acl_bpt_list,
            field_dst_ip=True)

        # ACL configuration for the test
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      ingress_acl=acl_table_id_ingress)

        dst_ip = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="10.0.0.1"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.0"))

        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id_ingress])
        param = sai_thrift_acl_action_parameter_t(objlist=obj_list)
        obj_acl_action_data = sai_thrift_acl_action_data_t(parameter=param)

        # Ingress ACL entry
        acl_entry_id_ingress = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_id_ingress,
            action_mirror_ingress=obj_acl_action_data,
            field_dst_ip=dst_ip)

        # Packet will be dropped in ingress due to wrong vlan_id
        pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                eth_src="00:00:00:00:00:22",
                                ip_dst="10.0.0.1",
                                dl_vlan_enable=True,
                                vlan_vid=50,
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                    eth_src="00:00:00:00:00:22",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=50,
                                    ip_id=101,
                                    ip_ttl=64)

        try:
            print("\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            verify_packet(self, exp_pkt, self.dev_port24)
            verify_no_other_packets(self, timeout=1)
            print("\tTest completed successfully - packet dropped"
                  " and mirrored on ingress")

        finally:
            sai_thrift_remove_acl_entry(self.client, acl_entry_id_ingress)
            sai_thrift_object_list_t(count=0, idlist=[span_id_ingress])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          ingress_acl=0)
            sai_thrift_remove_acl_table(self.client, acl_table_id_ingress)
            sai_thrift_remove_mirror_session(self.client, span_id_ingress)

    def aclMirrorDroppedPacketEgressTest(self):
        """
        Checking the lack of ACL mirroring if the packet is dropped on egress
        """
        print("\nACL Mirror Dropped Packet Egress Test")
        span_id_egress = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL)

        # Egress ACL table
        action_type_egress = [SAI_ACL_ACTION_TYPE_MIRROR_EGRESS]
        action_egress = sai_thrift_s32_list_t(count=len(action_type_egress),
                                              int32list=action_type_egress)
        bind_point = [SAI_ACL_BIND_POINT_TYPE_PORT]
        acl_bpt_list = sai_thrift_s32_list_t(count=len(bind_point),
                                             int32list=bind_point)
        acl_table_id_egress = sai_thrift_create_acl_table(
            self.client,
            acl_stage=SAI_ACL_STAGE_EGRESS,
            acl_action_type_list=action_egress,
            acl_bind_point_type_list=acl_bpt_list,
            field_dst_ip=True)

        # ACL configuration for the test
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      egress_acl=acl_table_id_egress)

        dst_ip = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="10.0.0.1"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.0"))

        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id_egress])
        param = sai_thrift_acl_action_parameter_t(objlist=obj_list)
        obj_acl_action_data = sai_thrift_acl_action_data_t(parameter=param)

        # Egress ACL entry
        acl_entry_id_egress = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_id_egress,
            action_mirror_egress=obj_acl_action_data,
            field_dst_ip=dst_ip)

        # Packet will be dropped in ingress due to wrong vlan_id
        pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                eth_src="00:00:00:00:00:33",
                                ip_dst="10.0.0.1",
                                dl_vlan_enable=True,
                                vlan_vid=50,
                                ip_id=101,
                                ip_ttl=64)

        try:
            print("\tSending packet PORT26 -> PORT25")
            send_packet(self, self.dev_port26, pkt)
            verify_no_other_packets(self, timeout=1)
            print("\tTest completed successfully - packet dropped"
                  " and not mirrored on egress")

        finally:
            sai_thrift_remove_acl_entry(self.client, acl_entry_id_egress)
            sai_thrift_object_list_t(count=0, idlist=[span_id_egress])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          egress_acl=0)
            sai_thrift_remove_acl_table(self.client, acl_table_id_egress)
            sai_thrift_remove_mirror_session(self.client, span_id_egress)

    def erspanAclEgressGreProto0x22ebTest(self):
        """
        Checking ACL egress mirroring, encapsulated remote SPAN
        GRE protocol 0x22eb
        """
        print("\nERSPAN ACL Egress GRE Protocol 0x22eb Test")
        encap_type = SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL
        erspan_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port28,
            type=SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE,
            erspan_encapsulation_type=encap_type,
            iphdr_version=0x4,
            src_ip_address=self.src_ip_addr_egr,
            dst_ip_address=self.dst_ip_addr_egr,
            src_mac_address="00:00:00:00:11:22",
            dst_mac_address="00:00:00:00:11:33",
            gre_protocol_type=0x22eb,
            ttl=64)

        # Egress ACL table
        action_type_egress = [SAI_ACL_ACTION_TYPE_MIRROR_EGRESS]
        action_egress = sai_thrift_s32_list_t(count=len(action_type_egress),
                                              int32list=action_type_egress)
        bind_point = [SAI_ACL_BIND_POINT_TYPE_PORT]
        acl_bpt_list = sai_thrift_s32_list_t(count=len(bind_point),
                                             int32list=bind_point)
        acl_table_id_egress = sai_thrift_create_acl_table(
            self.client,
            acl_stage=SAI_ACL_STAGE_EGRESS,
            acl_action_type_list=action_egress,
            acl_bind_point_type_list=acl_bpt_list,
            field_dst_ip=True)

        # ACL configuration for the test
        sai_thrift_set_port_attribute(self.client,
                                      self.port12,
                                      egress_acl=acl_table_id_egress)

        dst_ip = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="192.168.0.1"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.0"))

        obj_list = sai_thrift_object_list_t(count=1, idlist=[erspan_id])
        param = sai_thrift_acl_action_parameter_t(objlist=obj_list)
        obj_acl_action_data = sai_thrift_acl_action_data_t(parameter=param)

        # Egress ACL entry
        acl_entry_id_egress = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_id_egress,
            action_mirror_egress=obj_acl_action_data,
            field_dst_ip=dst_ip)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src="00:00:00:00:00:33",
                                ip_src="172.16.1.1",
                                ip_dst="192.168.0.1",
                                ip_id=101,
                                ip_ttl=64)

        exp_inner_pkt = simple_tcp_packet(eth_dst=self.neighbor_dmac_egr,
                                          eth_src=ROUTER_MAC,
                                          ip_src="172.16.1.1",
                                          ip_dst="192.168.0.1",
                                          ip_id=101,
                                          ip_ttl=63)

        exp_mirrored_pkt = ipv4_erspan_pkt(eth_dst="00:00:00:00:11:33",
                                           eth_src="00:00:00:00:11:22",
                                           ip_src="33.19.20.0",
                                           ip_dst="17.18.19.0",
                                           ip_id=0,
                                           ip_ttl=64,
                                           ip_flags=0x2,
                                           version=2,
                                           mirror_id=erspan_id,
                                           inner_frame=exp_inner_pkt)
        # IEEE 1588
        exp_mirrored_pkt["ERSPAN_III"].gra = 2

        exp_mask_mirrored_pkt = Mask(exp_mirrored_pkt)
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "tos")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "frag")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ihl")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "len")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ttl")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "flags")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "chksum")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.GRE, "proto")

        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "session_id")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "direction")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "version")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "vlan")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "priority")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "truncated")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "unknown2")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "timestamp")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "sgt_other")

        try:
            print("\tSending packet PORT13 -> PORT12")
            send_packet(self, self.dev_port13, pkt)
            verify_packet(self, exp_inner_pkt, self.dev_port12)
            print("\tPacket received on port 12")
            verify_packet(self, exp_mask_mirrored_pkt, self.dev_port28)
            print("\tPacket mirrored on port 28")
            verify_no_other_packets(self, timeout=1)
            print("\tTest completed successfully")

        finally:
            sai_thrift_remove_acl_entry(self.client, acl_entry_id_egress)
            sai_thrift_object_list_t(count=0, idlist=[erspan_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port12,
                                          egress_acl=0)
            sai_thrift_remove_acl_table(self.client, acl_table_id_egress)
            sai_thrift_remove_mirror_session(self.client, erspan_id)

    def erspanPortMirroringTest(self):
        """
        Checking ERSPAN mirroring, monitor = PORT
        """
        print("\nERSPAN Port Mirroring Test")

        encap_type = SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL
        erspan_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE,
            erspan_encapsulation_type=encap_type,
            iphdr_version=0x4,
            tos=0,
            src_ip_address=self.src_ip_addr,
            dst_ip_address=self.dst_ip_addr,
            src_mac_address="00:00:00:00:11:22",
            dst_mac_address="00:00:00:00:11:33",
            gre_protocol_type=0x22eb,
            ttl=64)

        obj_list = sai_thrift_object_list_t(count=1, idlist=[erspan_id])
        sai_thrift_set_port_attribute(self.client,
                                      self.port10,
                                      ingress_mirror_session=obj_list)
        try:
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src="00:00:00:00:00:22",
                                    ip_src="192.168.0.1",
                                    ip_dst="172.16.1.1",
                                    ip_id=101,
                                    ip_ttl=64)

            exp_pkt = simple_tcp_packet(eth_dst=self.neighbor_dmac,
                                        eth_src=ROUTER_MAC,
                                        ip_src="192.168.0.1",
                                        ip_dst="172.16.1.1",
                                        ip_id=101,
                                        ip_ttl=63)
            exp_inner_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                              eth_src="00:00:00:00:00:22",
                                              ip_src="192.168.0.1",
                                              ip_dst="172.16.1.1",
                                              ip_id=101,
                                              ip_ttl=64)
            exp_mirrored_pkt = ipv4_erspan_pkt(eth_src="00:00:00:00:11:22",
                                               eth_dst="00:00:00:00:11:33",
                                               ip_src="17.18.19.0",
                                               ip_dst="33.19.20.0",
                                               ip_id=0,
                                               ip_ttl=64,
                                               ip_flags=0x2,
                                               version=2,
                                               mirror_id=erspan_id,
                                               inner_frame=exp_inner_pkt)
            # IEEE 1588
            exp_mirrored_pkt["ERSPAN_III"].gra = 2

            exp_mask_mirrored_pkt = Mask(exp_mirrored_pkt)
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "tos")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ihl")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "len")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "frag")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ttl")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "flags")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP,
                                                        "chksum")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.GRE,
                                                        "proto")

            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "session_id")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "direction")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "timestamp")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "sgt_other")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "version")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "vlan")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "priority")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "truncated")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "unknown2")

            print("\tSending packet PORT10 -> PORT11")
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port11)
            print("\tPacket received on port 11")
            verify_packet(self, exp_mask_mirrored_pkt, self.dev_port24)
            print("\tPacket mirrored on port 24")
            verify_no_other_packets(self, timeout=1)
            print("\tTest completed successfully")

        finally:
            obj_list = sai_thrift_object_list_t(count=0,
                                                idlist=[erspan_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port10,
                                          ingress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, erspan_id)

    def erspanLagMirroringTest(self):
        """
        Checking ERSPAN mirroring, monitor = LAG
        """
        print("\nERSPAN Lag Mirroring Test")
        lag_member28 = sai_thrift_create_lag_member(self.client,
                                                    lag_id=self.lag11,
                                                    port_id=self.port28)
        lag_member31 = sai_thrift_create_lag_member(self.client,
                                                    lag_id=self.lag11,
                                                    port_id=self.port31)

        encap_type = SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL
        erspan_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.lag11,
            type=SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE,
            erspan_encapsulation_type=encap_type,
            iphdr_version=0x4,
            tos=0,
            src_ip_address=self.src_ip_addr,
            dst_ip_address=self.dst_ip_addr,
            src_mac_address="00:00:00:00:11:22",
            dst_mac_address="00:00:00:00:11:33",
            gre_protocol_type=0x22eb,
            ttl=64)

        obj_list = sai_thrift_object_list_t(count=1, idlist=[erspan_id])
        sai_thrift_set_port_attribute(self.client,
                                      self.port10,
                                      ingress_mirror_session=obj_list)
        try:
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src="00:00:00:00:00:22",
                                    ip_src="192.168.0.1",
                                    ip_dst="172.16.1.1",
                                    ip_id=101,
                                    ip_ttl=64)

            exp_pkt = simple_tcp_packet(eth_dst=self.neighbor_dmac,
                                        eth_src=ROUTER_MAC,
                                        ip_src="192.168.0.1",
                                        ip_dst="172.16.1.1",
                                        ip_id=101,
                                        ip_ttl=63)
            exp_inner_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                              eth_src="00:00:00:00:00:22",
                                              ip_src="192.168.0.1",
                                              ip_dst="172.16.1.1",
                                              ip_id=101,
                                              ip_ttl=64)
            exp_mirrored_pkt = ipv4_erspan_pkt(eth_src="00:00:00:00:11:22",
                                               eth_dst="00:00:00:00:11:33",
                                               ip_src="17.18.19.0",
                                               ip_dst="33.19.20.0",
                                               ip_id=0,
                                               ip_ttl=64,
                                               ip_flags=0x2,
                                               version=2,
                                               mirror_id=erspan_id,
                                               inner_frame=exp_inner_pkt)
            # IEEE 1588
            exp_mirrored_pkt["ERSPAN_III"].gra = 2

            exp_mask_mirrored_pkt = Mask(exp_mirrored_pkt)
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "tos")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ihl")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "len")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "frag")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ttl")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "flags")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP,
                                                        "chksum")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.GRE,
                                                        "proto")

            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "session_id")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "direction")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "timestamp")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "sgt_other")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "version")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "vlan")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "priority")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "truncated")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "unknown2")

            print("\tSending packet PORT10 -> PORT11")
            print("\tPORT28 and PORT31 were added to LAG11")
            send_packet(self, self.dev_port10, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [exp_pkt, exp_mask_mirrored_pkt],
                [[self.dev_port11], [self.dev_port28, self.dev_port31]])
            print("\tPacket received on PORT11 and mirrored on LAG11")
            print("\tNow PORT28 is being removed from LAG11")
            sai_thrift_remove_lag_member(self.client, lag_member28)
            send_packet(self, self.dev_port10, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [exp_pkt, exp_mask_mirrored_pkt],
                [[self.dev_port11], [self.dev_port31]])
            print("\tPacket received on PORT11 and mirrored on LAG11 (PORT31)")
            print("\tNow PORT31 is being removed from LAG11 - is empty")
            sai_thrift_remove_lag_member(self.client, lag_member31)
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port11)
            verify_no_other_packets(self, timeout=1)
            print("\tPacket received on PORT11, no mirroring")
            print("\tTest completed successfully")

        finally:
            obj_list = sai_thrift_object_list_t(count=0,
                                                idlist=[erspan_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port10,
                                          ingress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, erspan_id)

    def erspanMirrorSessionTrafficClassTest(self):
        """
        Checking ERSPAN egress mirroring with Traffic Class attribute enabled.
        Some platforms do not support SAI_MIRROR_SESSION_ATTR_TC
        and only support global mirror session traffic class.
        """
        print("\nERSPAN Mirror Session Traffic Class Test")
        encap_type = SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL
        traffic_class = 6

        erspan_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port28,
            type=SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE,
            erspan_encapsulation_type=encap_type,
            iphdr_version=0x4,
            tos=0,
            src_ip_address=self.src_ip_addr_egr,
            dst_ip_address=self.dst_ip_addr_egr,
            src_mac_address="00:00:00:00:11:22",
            dst_mac_address="00:00:00:00:11:33",
            gre_protocol_type=0x22eb,
            tc=traffic_class,
            vlan_pri=6,
            ttl=64)

        obj_list = sai_thrift_object_list_t(count=1, idlist=[erspan_id])

        queue_list = sai_thrift_object_list_t(
            count=10)
        attr = sai_thrift_get_port_attribute(self.client,
                                             self.port28,
                                             qos_queue_list=queue_list)

        for _, value in enumerate(attr["qos_queue_list"].idlist):
            q_attr = sai_thrift_get_queue_attribute(self.client, value,
                                                    index=True)
            if q_attr["index"] == traffic_class:
                queue_id = value
                break

        sai_thrift_set_port_attribute(self.client,
                                      self.port12,
                                      egress_mirror_session=obj_list)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src="00:00:00:00:00:33",
                                ip_src="172.16.1.1",
                                ip_dst="192.168.0.1",
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst=self.neighbor_dmac_egr,
                                    eth_src=ROUTER_MAC,
                                    ip_src="172.16.1.1",
                                    ip_dst="192.168.0.1",
                                    ip_id=101,
                                    ip_ttl=63)

        exp_mirrored_pkt = ipv4_erspan_pkt(eth_dst="00:00:00:00:11:33",
                                           eth_src="00:00:00:00:11:22",
                                           ip_src="33.19.20.0",
                                           ip_dst="17.18.19.0",
                                           ip_id=0,
                                           ip_ttl=64,
                                           ip_flags=0x2,
                                           version=2,
                                           mirror_id=erspan_id,
                                           inner_frame=exp_pkt)
        # IEEE 1588
        exp_mirrored_pkt["ERSPAN_III"].gra = 2

        exp_mask_mirrored_pkt = Mask(exp_mirrored_pkt)
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "tos")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ihl")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "len")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "frag")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ttl")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "flags")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP,
                                                    "chksum")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.GRE,
                                                    "proto")

        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "session_id")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "direction")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "timestamp")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "sgt_other")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "version")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "vlan")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "priority")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "truncated")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "unknown2")
        try:
            print("\tSending packet PORT13 -> PORT12")
            sai_thrift_clear_queue_stats(self.client, queue_id)
            stats = sai_thrift_get_queue_stats(self.client, queue_id)
            pkt_cnt_before = stats["SAI_QUEUE_STAT_PACKETS"]
            send_packet(self, self.dev_port13, pkt)
            verify_packet(self, exp_pkt, self.dev_port12)
            print("\tPacket received on port 12")
            verify_packet(self, exp_mask_mirrored_pkt, self.dev_port28)
            print("\tPacket mirrored on port 28")
            verify_no_other_packets(self, timeout=1)
            stats = sai_thrift_get_queue_stats(self.client, queue_id)
            pkt_cnt_after = stats["SAI_QUEUE_STAT_PACKETS"]
            assert(pkt_cnt_after == pkt_cnt_before + 1), ("Mirrored packet "
                                                          "does not get to the"
                                                          " correct queue")

            print("\tTest completed successfully - packets received")
        finally:
            obj_list = sai_thrift_object_list_t(count=0, idlist=[erspan_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port12,
                                          egress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, erspan_id)

    def erspanVlanPortMirroringTest(self):
        """
        Checking ERSPAN VLAN mirroring, monitor = PORT
        """
        print("\nERSPAN VLAN Port Mirroring Test")

        encap_type = SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL
        erspan_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE,
            erspan_encapsulation_type=encap_type,
            iphdr_version=0x4,
            tos=0,
            src_ip_address=self.src_ip_addr,
            dst_ip_address=self.dst_ip_addr,
            src_mac_address="00:00:00:00:11:22",
            dst_mac_address="00:00:00:00:11:33",
            gre_protocol_type=0x22eb,
            vlan_id=40,
            vlan_pri=6,
            vlan_cfi=1,
            vlan_tpid=self.vlan_tpid_0x8100,
            vlan_header_valid=True,
            ttl=64)

        obj_list = sai_thrift_object_list_t(count=1, idlist=[erspan_id])
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      ingress_mirror_session=obj_list)
        try:
            pkt = simple_tcp_packet(eth_dst="00:11:11:11:11:11",
                                    eth_src="00:00:00:00:00:22",
                                    ip_src="192.168.0.1",
                                    ip_dst="172.16.1.2",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    vlan_pcp=6,
                                    dl_vlan_cfi=1,
                                    ip_id=101,
                                    ip_ttl=64)

            exp_pkt = simple_tcp_packet(eth_dst="00:11:11:11:11:11",
                                        eth_src="00:00:00:00:00:22",
                                        ip_src="192.168.0.1",
                                        ip_dst="172.16.1.2",
                                        dl_vlan_enable=True,
                                        vlan_vid=40,
                                        vlan_pcp=6,
                                        dl_vlan_cfi=0,
                                        ip_id=101,
                                        ip_ttl=64)

            exp_mirrored_pkt = ipv4_erspan_pkt(eth_src="00:00:00:00:11:22",
                                               eth_dst="00:00:00:00:11:33",
                                               ip_src="17.18.19.0",
                                               ip_dst="33.19.20.0",
                                               dl_vlan_enable=True,
                                               vlan_vid=40,
                                               vlan_pcp=6,
                                               ip_id=0,
                                               ip_ttl=64,
                                               ip_flags=0x2,
                                               version=2,
                                               mirror_id=erspan_id,
                                               inner_frame=exp_pkt)
            # IEEE 1588
            exp_mirrored_pkt["ERSPAN_III"].gra = 2

            exp_mask_mirrored_pkt = Mask(exp_mirrored_pkt)
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "tos")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ihl")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "len")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "frag")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ttl")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "flags")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP,
                                                        "chksum")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.GRE,
                                                        "proto")

            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "session_id")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "direction")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "timestamp")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "sgt_other")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "version")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "priority")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "truncated")
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                        "unknown2")

            print("\tSending packet PORT25 -> PORT26")
            print("\tPackets should be received on all ports in VLAN40:")
            send_packet(self, self.dev_port25, pkt)
            verify_packet(self, exp_pkt, self.dev_port26)
            print("\tPacket received on port 26")
            verify_packet(self, exp_pkt, self.dev_port27)
            print("\tPacket received on port 27")
            verify_packet(self, exp_mask_mirrored_pkt, self.dev_port24)
            print("\tPacket mirrored on port 24")
            verify_packet(self, exp_pkt, self.dev_port24)
            print("\tPacket received on port 24")
            verify_no_other_packets(self, timeout=1)
            print("\tTest completed successfully")

        finally:
            obj_list = sai_thrift_object_list_t(count=0,
                                                idlist=[erspan_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          ingress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, erspan_id)

    def ingressMirrorPolicingTest(self):
        """
        Checking ingress mirroring with policers
        """
        print("\nIngress Mirror Policing Test")
        span_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL,
            policer=self.pol_id)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id])
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      ingress_mirror_session=obj_list)

        pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                eth_src="00:00:00:00:00:22",
                                ip_dst="10.0.0.1",
                                dl_vlan_enable=True,
                                vlan_vid=40,
                                ip_id=101,
                                ip_ttl=64,
                                pktlen=1000)

        exp_pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:33",
                                    eth_src="00:00:00:00:00:22",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64,
                                    pktlen=1000)

        try:
            stats = sai_thrift_get_policer_stats(self.client, self.pol_id)
            g_pkts0 = stats["SAI_POLICER_STAT_GREEN_PACKETS"]
            r_pkts0 = stats["SAI_POLICER_STAT_RED_PACKETS"]
            pkt_cnt = 10
            rec_pkt = 0

            print("\tSending 10 packets PORT25 -> PORT26")
            for _ in range(0, pkt_cnt):
                send_packet(self, self.dev_port25, pkt)
                rec_pkt += 1

            time.sleep(3)

            mirrored_pkts = count_matched_packets(self, exp_pkt,
                                                  self.dev_port24, timeout=1)
            print("\tMIRRORED_PACKET_COUNTER = {}".format(mirrored_pkts))
            rcv_pkts = count_matched_packets(self, exp_pkt, self.dev_port26,
                                             timeout=1)
            print("\tTOTAL_RECEIVE_PACKET_COUNTER ={}".format(rcv_pkts))

            print("\tChecking if received packet number equals sent one")
            self.assertEqual(rec_pkt, pkt_cnt)
            self.assertEqual(rcv_pkts, pkt_cnt)
            print("\tReceived packet number is correct")

            print("\tChecking policer statistics...")
            stats = sai_thrift_get_policer_stats(self.client, self.pol_id)
            g_pkts1 = stats["SAI_POLICER_STAT_GREEN_PACKETS"]
            r_pkts1 = stats["SAI_POLICER_STAT_RED_PACKETS"]
            # Only green packets are mirrored that is why total mirrored
            # packets number should be equal to green packets number
            print("\tGREEN_PACKETS = {}".format(g_pkts1))
            g_pkts1_exp = g_pkts0 + mirrored_pkts
            self.assertEqual(g_pkts1, g_pkts1_exp)
            # The number of red packets equals received without mirrored ones.
            print("\tRED_PACKETS = {}".format(r_pkts1))
            r_pkts1_exp = r_pkts0 + rcv_pkts - mirrored_pkts
            self.assertEqual(r_pkts1, r_pkts1_exp)
            assert(g_pkts1 + r_pkts1 == pkt_cnt), ("The policer packet stats"
                                                   " counter does not match")

            print("\tTest completed successfully - packets received")

        finally:
            obj_list = sai_thrift_object_list_t(count=0, idlist=[span_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          ingress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, span_id)

    def egressMirrorPolicingTest(self):
        """
        Checking egress mirroring with policers
        """
        print("\nEgress Mirror Policing Test")
        span_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_LOCAL,
            policer=self.pol_id)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[span_id])
        sai_thrift_set_port_attribute(self.client,
                                      self.port25,
                                      egress_mirror_session=obj_list)

        pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                eth_src="00:00:00:00:00:33",
                                ip_dst="10.0.0.1",
                                dl_vlan_enable=True,
                                vlan_vid=40,
                                ip_id=101,
                                ip_ttl=64,
                                pktlen=1000)

        exp_pkt = simple_tcp_packet(eth_dst="00:00:00:00:00:22",
                                    eth_src="00:00:00:00:00:33",
                                    ip_dst="10.0.0.1",
                                    dl_vlan_enable=True,
                                    vlan_vid=40,
                                    ip_id=101,
                                    ip_ttl=64,
                                    pktlen=1000)

        try:
            stats = sai_thrift_get_policer_stats(self.client, self.pol_id)
            g_pkts0 = stats["SAI_POLICER_STAT_GREEN_PACKETS"]
            r_pkts0 = stats["SAI_POLICER_STAT_RED_PACKETS"]
            pkt_cnt = 10
            rec_pkt = 0

            print("\tSending 10 packets PORT11 -> PORT10")
            for _ in range(0, pkt_cnt):
                send_packet(self, self.dev_port26, pkt)
                rec_pkt += 1

            time.sleep(3)

            mirrored_pkts = count_matched_packets(self, exp_pkt,
                                                  self.dev_port24, timeout=1)
            print("\tMIRRORED_PACKET_COUNTER = {}".format(mirrored_pkts))
            rcv_pkts = count_matched_packets(self, exp_pkt, self.dev_port25,
                                             timeout=1)
            print("\tTOTAL_RECEIVE_PACKET_COUNTER ={}".format(rcv_pkts))

            print("\tChecking if received packet number equals sent one")
            self.assertEqual(rec_pkt, pkt_cnt)
            self.assertEqual(rcv_pkts, pkt_cnt)
            print("\tReceived packet number is correct")

            print("\tChecking policer statistics...")
            stats = sai_thrift_get_policer_stats(self.client, self.pol_id)
            g_pkts1 = stats["SAI_POLICER_STAT_GREEN_PACKETS"]
            r_pkts1 = stats["SAI_POLICER_STAT_RED_PACKETS"]
            # Only green packets are mirrored that is why total mirrored
            # packets number should be equal to green packets number
            print("\tGREEN_PACKETS = {}".format(g_pkts1))
            g_pkts1_exp = g_pkts0 + mirrored_pkts
            self.assertEqual(g_pkts1, g_pkts1_exp)
            # The number of red packets equals received without mirrored ones.
            print("\tRED_PACKETS = {}".format(r_pkts1))
            r_pkts1_exp = r_pkts0 + rcv_pkts - mirrored_pkts
            self.assertEqual(r_pkts1, r_pkts1_exp)
            assert(g_pkts1 + r_pkts1 == pkt_cnt), ("The policer packet stats"
                                                   " counter does not match")

            print("\tTest completed successfully - packets received")

        finally:
            obj_list = sai_thrift_object_list_t(count=0, idlist=[span_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port25,
                                          egress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, span_id)

    def erspanIngressMirrorPolicingTest(self):
        """
        Checking ERSPAN ingress mirroring with policers
        """
        print("\nERSPAN Ingress Mirror Policing Test")
        encap_type = SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL
        erspan_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE,
            erspan_encapsulation_type=encap_type,
            iphdr_version=0x4,
            tos=0,
            src_ip_address=self.src_ip_addr,
            dst_ip_address=self.dst_ip_addr,
            src_mac_address="00:00:00:00:11:22",
            dst_mac_address="00:00:00:00:11:33",
            gre_protocol_type=0x22eb,
            vlan_pri=6,
            ttl=64,
            policer=self.pol_id)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[erspan_id])
        sai_thrift_set_port_attribute(self.client,
                                      self.port10,
                                      ingress_mirror_session=obj_list)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src="00:00:00:00:00:22",
                                ip_src="192.168.0.1",
                                ip_dst="172.16.1.1",
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst=self.neighbor_dmac,
                                    eth_src=ROUTER_MAC,
                                    ip_src="192.168.0.1",
                                    ip_dst="172.16.1.1",
                                    ip_id=101,
                                    ip_ttl=63)
        exp_inner_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                          eth_src="00:00:00:00:00:22",
                                          ip_src="192.168.0.1",
                                          ip_dst="172.16.1.1",
                                          ip_id=101,
                                          ip_ttl=64)
        exp_mirrored_pkt = ipv4_erspan_pkt(eth_src="00:00:00:00:11:22",
                                           eth_dst="00:00:00:00:11:33",
                                           ip_src="17.18.19.0",
                                           ip_dst="33.19.20.0",
                                           ip_id=0,
                                           ip_ttl=64,
                                           ip_flags=0x2,
                                           version=2,
                                           mirror_id=erspan_id,
                                           inner_frame=exp_inner_pkt)
        # IEEE 1588
        exp_mirrored_pkt["ERSPAN_III"].gra = 2

        exp_mask_mirrored_pkt = Mask(exp_mirrored_pkt)
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "tos")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ihl")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "len")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "frag")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ttl")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "flags")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP,
                                                    "chksum")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.GRE,
                                                    "proto")

        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "session_id")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "direction")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "timestamp")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "sgt_other")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "version")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "vlan")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "priority")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "truncated")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "unknown2")

        try:
            stats = sai_thrift_get_policer_stats(self.client, self.pol_id)
            g_pkts0 = stats["SAI_POLICER_STAT_GREEN_PACKETS"]
            r_pkts0 = stats["SAI_POLICER_STAT_RED_PACKETS"]
            pkt_cnt = 10
            rec_pkt = 0

            print("\tSending 10 packets PORT10 -> PORT11")
            for _ in range(0, pkt_cnt):
                send_packet(self, self.dev_port10, pkt)
                rec_pkt += 1

            time.sleep(3)

            mirrored_pkts = count_matched_packets(self, exp_mask_mirrored_pkt,
                                                  self.dev_port24, timeout=1)
            print("\tMIRRORED_PACKET_COUNTER = {}".format(mirrored_pkts))
            rcv_pkts = count_matched_packets(self, exp_pkt, self.dev_port11,
                                             timeout=1)
            print("\tTOTAL_RECEIVE_PACKET_COUNTER ={}".format(rcv_pkts))

            print("\tChecking if received packet number equals sent one")
            self.assertEqual(rec_pkt, pkt_cnt)
            self.assertEqual(rcv_pkts, pkt_cnt)
            print("\tReceived packet number is correct")

            print("\tChecking policer statistics...")
            stats = sai_thrift_get_policer_stats(self.client, self.pol_id)
            g_pkts1 = stats["SAI_POLICER_STAT_GREEN_PACKETS"]
            r_pkts1 = stats["SAI_POLICER_STAT_RED_PACKETS"]
            # Only green packets are mirrored that is why total mirrored
            # packets number should be equal to green packets number
            print("\tGREEN_PACKETS = {}".format(g_pkts1))
            g_pkts1_exp = g_pkts0 + mirrored_pkts
            self.assertEqual(g_pkts1, g_pkts1_exp)
            # The number of red packets equals received without mirrored ones.
            print("\tRED_PACKETS = {}".format(r_pkts1))
            r_pkts1_exp = r_pkts0 + rcv_pkts - mirrored_pkts
            self.assertEqual(r_pkts1, r_pkts1_exp)
            assert(g_pkts1 + r_pkts1 == pkt_cnt), ("The policer packet stats"
                                                   " counter does not match")

            print("\tTest completed successfully - packets received")

        finally:
            obj_list = sai_thrift_object_list_t(count=0, idlist=[erspan_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port10,
                                          ingress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, erspan_id)

    def erspanEgressMirrorPolicingTest(self):
        """
        Checking ERSPAN egress mirroring with policers
        """
        print("\nERSPAN Egress Mirror Policing Test")
        encap_type = SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL
        erspan_id = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port28,
            type=SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE,
            erspan_encapsulation_type=encap_type,
            iphdr_version=0x4,
            tos=0,
            src_ip_address=self.src_ip_addr_egr,
            dst_ip_address=self.dst_ip_addr_egr,
            src_mac_address="00:00:00:00:11:22",
            dst_mac_address="00:00:00:00:11:33",
            gre_protocol_type=0x22eb,
            vlan_pri=6,
            ttl=64,
            policer=self.pol_id)
        obj_list = sai_thrift_object_list_t(count=1, idlist=[erspan_id])
        sai_thrift_set_port_attribute(self.client,
                                      self.port12,
                                      egress_mirror_session=obj_list)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src="00:00:00:00:00:33",
                                ip_src="172.16.1.1",
                                ip_dst="192.168.0.1",
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst=self.neighbor_dmac_egr,
                                    eth_src=ROUTER_MAC,
                                    ip_src="172.16.1.1",
                                    ip_dst="192.168.0.1",
                                    ip_id=101,
                                    ip_ttl=63)
        exp_mirrored_pkt = ipv4_erspan_pkt(eth_dst="00:00:00:00:11:33",
                                           eth_src="00:00:00:00:11:22",
                                           ip_src="33.19.20.0",
                                           ip_dst="17.18.19.0",
                                           ip_id=0,
                                           ip_ttl=64,
                                           ip_flags=0x2,
                                           version=2,
                                           mirror_id=erspan_id,
                                           inner_frame=exp_pkt)
        # IEEE 1588
        exp_mirrored_pkt["ERSPAN_III"].gra = 2

        exp_mask_mirrored_pkt = Mask(exp_mirrored_pkt)
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "tos")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "frag")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ihl")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "len")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "ttl")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, "flags")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP,
                                                    "chksum")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.GRE,
                                                    "proto")

        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "session_id")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "direction")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "timestamp")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "sgt_other")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "version")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "vlan")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "priority")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "truncated")
        exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III,
                                                    "unknown2")

        try:
            stats = sai_thrift_get_policer_stats(self.client, self.pol_id)
            g_pkts0 = stats["SAI_POLICER_STAT_GREEN_PACKETS"]
            r_pkts0 = stats["SAI_POLICER_STAT_RED_PACKETS"]
            pkt_cnt = 10
            rec_pkt = 0

            print("\tSending 10 packets PORT13 -> PORT12")
            for _ in range(0, pkt_cnt):
                send_packet(self, self.dev_port13, pkt)
                rec_pkt += 1

            time.sleep(3)

            mirrored_pkts = count_matched_packets(self, exp_mask_mirrored_pkt,
                                                  self.dev_port28, timeout=1)
            print("\tMIRRORED_PACKET_COUNTER = {}".format(mirrored_pkts))
            rcv_pkts = count_matched_packets(self, exp_pkt, self.dev_port12,
                                             timeout=1)
            print("\tTOTAL_RECEIVE_PACKET_COUNTER ={}".format(rcv_pkts))

            print("\tChecking if received packet number equals sent one")
            self.assertEqual(rec_pkt, pkt_cnt)
            self.assertEqual(rcv_pkts, pkt_cnt)
            print("\tReceived packet number is correct")

            print("\tChecking policer statistics...")
            stats = sai_thrift_get_policer_stats(self.client, self.pol_id)
            g_pkts1 = stats["SAI_POLICER_STAT_GREEN_PACKETS"]
            r_pkts1 = stats["SAI_POLICER_STAT_RED_PACKETS"]
            # Only green packets are mirrored that is why total mirrored
            # packets number should be equal to green packets number
            print("\tGREEN_PACKETS = {}".format(g_pkts1))
            g_pkts1_exp = g_pkts0 + mirrored_pkts
            self.assertEqual(g_pkts1, g_pkts1_exp)
            # The number of red packets equals received without mirrored ones.
            print("\tRED_PACKETS = {}".format(r_pkts1))
            r_pkts1_exp = r_pkts0 + rcv_pkts - mirrored_pkts
            self.assertEqual(r_pkts1, r_pkts1_exp)
            assert(g_pkts1 + r_pkts1 == pkt_cnt), ("The policer packet stats"
                                                   " counter does not match")

            print("\tTest completed successfully - packets received")

        finally:
            obj_list = sai_thrift_object_list_t(count=0, idlist=[erspan_id])
            sai_thrift_set_port_attribute(self.client,
                                          self.port12,
                                          egress_mirror_session=obj_list)
            sai_thrift_remove_mirror_session(self.client, erspan_id)
