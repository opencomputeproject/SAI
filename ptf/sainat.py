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
Thrift SAI interface NAT tests
'''

from sai_thrift.sai_headers import *

from sai_base_test import *


@group("draft")
class NatTranslationTest(SaiHelper):
    '''
    NAT translation tests including ACL rules coexistence.
    '''

    def setUp(self):
        super(NatTranslationTest, self).setUp()

        # object lists
        self.route_list = []
        self.nbor_list = []
        self.nhop_list = []
        self.acl_entry_list = []
        self.acl_cntr_list = []
        self.acl_table_list = []
        self.rif_list = []
        self.vlan_mbr_list = []
        self.vlan_list = []
        self.bp_list = []

        # regular L3 ports configuration
        self.ingr_port_rif = self.port10_rif
        self.egr_port_rif = self.port11_rif

        self.ingr_port = self.port10
        self.egr_port = self.port11

        self.ingr_port_dev = self.dev_port10
        self.egr_port_dev = self.dev_port11

        self.port_nbor_ip = "10.10.10.1"
        self.port_nbor_mac = "00:11:11:11:11:11"
        self.nat_ip_to_port = "30.30.30.10"

        self.nat_port_nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.port_nbor_ip),
            router_interface_id=self.egr_port_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nhop_list.append(self.nat_port_nhop)

        self.nat_port_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.egr_port_rif,
            ip_address=sai_ipaddress(self.port_nbor_ip))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.nat_port_nbor,
                                         dst_mac_address=self.port_nbor_mac,
                                         no_host_route=True)
        self.nbor_list.append(self.nat_port_nbor)

        self.nat_port_route = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.port_nbor_ip + '/32'))
        sai_thrift_create_route_entry(
            self.client, self.nat_port_route, next_hop_id=self.nat_port_nhop)
        self.route_list.append(self.nat_port_route)

        # L3 LAGs configuration
        self.ingr_lag_rif = self.lag3_rif
        self.egr_lag_rif = self.lag4_rif

        self.ingr_lag = [self.port14, self.port15, self.port16]
        self.egr_lag = [self.port17, self.port18, self.port19]

        self.ingr_lag_dev = [self.dev_port14, self.dev_port15, self.dev_port16]
        self.egr_lag_dev = [self.dev_port17, self.dev_port18, self.dev_port19]

        self.lag_nbor_ip = "10.10.20.1"
        self.lag_nbor_mac = "00:22:22:22:22:22"
        self.nat_ip_to_lag = "30.30.30.20"

        self.nat_lag_nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.lag_nbor_ip),
            router_interface_id=self.egr_lag_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nhop_list.append(self.nat_lag_nhop)

        self.nat_lag_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.egr_lag_rif,
            ip_address=sai_ipaddress(self.lag_nbor_ip))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.nat_lag_nbor,
                                         dst_mac_address=self.lag_nbor_mac,
                                         no_host_route=True)
        self.nbor_list.append(self.nat_lag_nbor)

        self.nat_lag_route = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.lag_nbor_ip + '/32'))
        sai_thrift_create_route_entry(
            self.client, self.nat_lag_route, next_hop_id=self.nat_lag_nhop)
        self.route_list.append(self.nat_lag_route)

        # SVI configuration
        self.port24_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.bp_list.append(self.port24_bp)

        self.port25_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port25,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.bp_list.append(self.port25_bp)

        self.vlan100 = sai_thrift_create_vlan(self.client, vlan_id=100)
        self.vlan_list.append(self.vlan100)
        self.vlan100_member0 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_mbr_list.append(self.vlan100_member0)
        self.vlan100_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port25_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_mbr_list.append(self.vlan100_member1)

        sai_thrift_set_port_attribute(
            self.client, self.port24, port_vlan_id=100)
        sai_thrift_set_port_attribute(
            self.client, self.port25, port_vlan_id=100)

        self.port26_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port26,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.bp_list.append(self.port26_bp)

        self.port27_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port27,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.bp_list.append(self.port27_bp)

        self.vlan200 = sai_thrift_create_vlan(self.client, vlan_id=200)
        self.vlan_list.append(self.vlan200)
        self.vlan200_member0 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan200,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_mbr_list.append(self.vlan200_member0)
        self.vlan200_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan200,
            bridge_port_id=self.port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_mbr_list.append(self.vlan200_member1)

        sai_thrift_set_port_attribute(
            self.client, self.port26, port_vlan_id=200)
        sai_thrift_set_port_attribute(
            self.client, self.port27, port_vlan_id=200)

        self.ingr_svi_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan100)
        self.rif_list.append(self.ingr_svi_rif)

        self.egr_svi_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan200)
        self.rif_list.append(self.egr_svi_rif)

        self.ingr_svi = [self.port24, self.port25]
        self.egr_svi = [self.port26, self.port27]

        self.ingr_svi_dev = [self.dev_port24, self.dev_port25]
        self.egr_svi_dev = [self.dev_port26, self.dev_port27]

        self.svi_nbor_ip = "10.10.30.1"
        self.svi_nbor_mac = "00:33:33:33:33:33"
        self.nat_ip_to_svi = "30.30.30.30"

        self.nat_svi_nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.svi_nbor_ip),
            router_interface_id=self.egr_svi_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nhop_list.append(self.nat_svi_nhop)

        self.nat_svi_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.egr_svi_rif,
            ip_address=sai_ipaddress(self.svi_nbor_ip))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.nat_svi_nbor,
                                         dst_mac_address=self.svi_nbor_mac,
                                         no_host_route=True)
        self.nbor_list.append(self.nat_svi_nbor)

        self.nat_svi_route = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.svi_nbor_ip + '/32'))
        sai_thrift_create_route_entry(
            self.client, self.nat_svi_route, next_hop_id=self.nat_svi_nhop)
        self.route_list.append(self.nat_svi_route)

        # no-NAT ACL configuration
        action_types = [SAI_ACL_ACTION_TYPE_NO_NAT]
        action_types_list = sai_thrift_s32_list_t(count=len(action_types),
                                                  int32list=action_types)

        acl_action = sai_thrift_acl_action_data_t(
            enable=True,
            parameter=sai_thrift_acl_action_parameter_t(booldata=True))

        bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE]
        bind_points_list = sai_thrift_s32_list_t(count=len(bind_points),
                                                 int32list=bind_points)

        self.ingr_acl_table = sai_thrift_create_acl_table(
            self.client,
            acl_stage=SAI_ACL_STAGE_INGRESS,
            acl_bind_point_type_list=bind_points_list,
            acl_action_type_list=action_types_list,
            field_dst_ip=True)
        self.acl_table_list.append(self.ingr_acl_table)

        self.ingr_acl_counter = sai_thrift_create_acl_counter(
            self.client,
            self.ingr_acl_table,
            enable_packet_count=True)
        self.acl_cntr_list.append(self.ingr_acl_counter)

        ingr_acl_cnt_action = sai_thrift_acl_action_data_t(
            enable=True,
            parameter=sai_thrift_acl_action_parameter_t(
                oid=self.ingr_acl_counter))

        port_nbor_ip_addr = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=self.port_nbor_ip),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        self.ingr_acl_port_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=self.ingr_acl_table,
            action_no_nat=acl_action,
            action_counter=ingr_acl_cnt_action,
            field_dst_ip=port_nbor_ip_addr)
        self.acl_entry_list.append(self.ingr_acl_port_entry)

        lag_nbor_ip_addr = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=self.lag_nbor_ip),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        self.ingr_acl_lag_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=self.ingr_acl_table,
            action_no_nat=acl_action,
            action_counter=ingr_acl_cnt_action,
            field_dst_ip=lag_nbor_ip_addr)
        self.acl_entry_list.append(self.ingr_acl_lag_entry)

        svi_nbor_ip_addr = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=self.svi_nbor_ip),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        self.ingr_acl_svi_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=self.ingr_acl_table,
            action_no_nat=acl_action,
            action_counter=ingr_acl_cnt_action,
            field_dst_ip=svi_nbor_ip_addr)
        self.acl_entry_list.append(self.ingr_acl_svi_entry)

        self.egr_acl_table = sai_thrift_create_acl_table(
            self.client,
            acl_stage=SAI_ACL_STAGE_INGRESS,
            acl_bind_point_type_list=bind_points_list,
            acl_action_type_list=action_types_list,
            field_dst_ip=True)
        self.acl_table_list.append(self.egr_acl_table)

        self.egr_acl_counter = sai_thrift_create_acl_counter(
            self.client,
            self.egr_acl_table,
            enable_packet_count=True)
        self.acl_cntr_list.append(self.egr_acl_counter)

        egr_acl_cnt_action = sai_thrift_acl_action_data_t(
            enable=True,
            parameter=sai_thrift_acl_action_parameter_t(
                oid=self.egr_acl_counter))

        to_port_ip_addr = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=self.nat_ip_to_port),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        self.egr_acl_port_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=self.egr_acl_table,
            action_no_nat=acl_action,
            action_counter=egr_acl_cnt_action,
            field_dst_ip=to_port_ip_addr)
        self.acl_entry_list.append(self.egr_acl_port_entry)

        to_lag_ip_addr = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=self.nat_ip_to_lag),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        self.egr_acl_lag_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=self.egr_acl_table,
            action_no_nat=acl_action,
            action_counter=egr_acl_cnt_action,
            field_dst_ip=to_lag_ip_addr)
        self.acl_entry_list.append(self.egr_acl_lag_entry)

        to_svi_ip_addr = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=self.nat_ip_to_svi),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        self.egr_acl_svi_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=self.egr_acl_table,
            action_no_nat=acl_action,
            action_counter=egr_acl_cnt_action,
            field_dst_ip=to_svi_ip_addr)
        self.acl_entry_list.append(self.egr_acl_svi_entry)

        # no-NAT route configuration
        no_nat_rif = self.port12_rif
        self.no_nat_eport = self.dev_port12

        no_nat_ip = "30.30.30.0"
        self.no_nat_nbor_mac = "00:33:33:33:33:33"

        self.no_nat_nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(no_nat_ip),
            router_interface_id=no_nat_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nhop_list.append(self.no_nat_nhop)

        self.no_nat_nbor = sai_thrift_neighbor_entry_t(
            rif_id=no_nat_rif,
            ip_address=sai_ipaddress(no_nat_ip))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.no_nat_nbor,
                                         dst_mac_address=self.no_nat_nbor_mac,
                                         no_host_route=True)
        self.nbor_list.append(self.no_nat_nbor)

        self.no_nat_route = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(no_nat_ip + '/24'))
        sai_thrift_create_route_entry(
            self.client, self.no_nat_route, next_hop_id=self.no_nat_nhop)
        self.route_list.append(self.no_nat_route)

    def runTest(self):
        self.srcNatAclTranslationDisableTest()
        self.dstNatAclTranslationDisableTest()

    def tearDown(self):
        for route in self.route_list:
            sai_thrift_remove_route_entry(self.client, route)

        for nbor in self.nbor_list:
            sai_thrift_remove_neighbor_entry(self.client, nbor)

        for nhop in self.nhop_list:
            sai_thrift_remove_next_hop(self.client, nhop)

        for acl_entry in self.acl_entry_list:
            sai_thrift_remove_acl_entry(self.client, acl_entry)

        for acl_cntr in self.acl_cntr_list:
            sai_thrift_remove_acl_counter(self.client, acl_cntr)

        for acl_table in self.acl_table_list:
            sai_thrift_remove_acl_table(self.client, acl_table)

        for rif in self.rif_list:
            sai_thrift_remove_router_interface(self.client, rif)

        for port in [self.port24, self.port25, self.port26, self.port27]:
            sai_thrift_set_port_attribute(self.client, port, port_vlan_id=0)

        for vlan_mbr in self.vlan_mbr_list:
            sai_thrift_remove_vlan_member(self.client, vlan_mbr)

        for vlan in self.vlan_list:
            sai_thrift_remove_vlan(self.client, vlan)

        for bp in self.bp_list:
            sai_thrift_remove_bridge_port(self.client, bp)

        super(NatTranslationTest, self).tearDown()

    def _verifyAclCounter(self, acl_counter, prev_value):
        '''
        Helper function for verifying if given ACL counter was incremented.

        Args:
            acl_counter (oid): object ID of ACL counter to be read
            prev_value (int): przevious counter value

        Return:
            bool: True if counter was incremented; False otherwise
        '''
        counter = sai_thrift_get_acl_counter_attribute(
            self.client, acl_counter, packets=True)
        counter = counter['packets']

        if counter == prev_value + 1:
            print("ACL hit")
            return True

        return False

    def _verifyNatHit(self, nat_entry):
        '''
        Helper function for verifying if given NAT entry was hit.

        Args:
            nat_entry (oid): object ID of NAT entry for which counter is
                             to be read

        Return:
            bool: True if NAT was hit; False otherwise
        '''
        counter = sai_thrift_get_nat_entry_attribute(
            self.client, nat_entry, packet_count=True)
        counter = counter['packet_count']

        if counter != 1:
            return False

        print("NAT hit")
        sai_thrift_set_nat_entry_attribute(
            self.client, nat_entry, packet_count=0)

        return True

    def srcNatAclTranslationDisableTest(self):
        '''
        Verifies if translation doesn't occur when source NAT entry exists
        but ACL is configured to disable NAT translation.
        Test is performed for different RIFs (regular L3 port, L3 LAG and SVI)
        and different nexhtops (regular L3 port, L3 LAG and SVI)
        with two kinds of packets - TCP and UDP.
        '''
        print("\nsrcNatAclTranslationDisableTest()")

        src_ip = "20.20.20.1"
        nat_src_ip = "150.10.10.10"

        def verify_translation(src_rif, src_port_dev):
            '''
            Additional helper function for translation verification.
            Verifies if translation doesn't occur when ACL is configured
            to disable it.

            Args:
                src_rif (oid): object ID of source RIF
                src_port_dev (int): source device port number
            '''
            acl_counter = sai_thrift_get_acl_counter_attribute(
                self.client, self.ingr_acl_counter, packets=True)['packets']

            # use route to L3 port
            print("   -> Egress L3 port")
            tcp_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                        ip_src=src_ip,
                                        ip_dst=self.port_nbor_ip,
                                        ip_ttl=64,
                                        pktlen=100,
                                        with_tcp_chksum=True)
            nat_tcp_pkt = simple_tcp_packet(eth_dst=self.port_nbor_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_src=nat_src_ip,
                                            ip_dst=self.port_nbor_ip,
                                            ip_ttl=63,
                                            pktlen=100,
                                            with_tcp_chksum=True)
            no_nat_tcp_pkt = simple_tcp_packet(eth_dst=self.port_nbor_mac,
                                               eth_src=ROUTER_MAC,
                                               ip_src=src_ip,
                                               ip_dst=self.port_nbor_ip,
                                               ip_ttl=63,
                                               pktlen=100,
                                               with_tcp_chksum=True)

            udp_pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                        ip_src=src_ip,
                                        ip_dst=self.port_nbor_ip,
                                        ip_ttl=64,
                                        pktlen=100)
            nat_udp_pkt = simple_udp_packet(eth_dst=self.port_nbor_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_src=nat_src_ip,
                                            ip_dst=self.port_nbor_ip,
                                            ip_ttl=63,
                                            pktlen=100)
            no_nat_udp_pkt = simple_udp_packet(eth_dst=self.port_nbor_mac,
                                               eth_src=ROUTER_MAC,
                                               ip_src=src_ip,
                                               ip_dst=self.port_nbor_ip,
                                               ip_ttl=63,
                                               pktlen=100)

            print("Sending TCP packet with NAT enabled")
            send_packet(self, src_port_dev, tcp_pkt)
            verify_packet(self, nat_tcp_pkt, self.egr_port_dev)
            self.assertTrue(self._verifyNatHit(snat))
            print("\tOK")

            print("Sending UDP packet with NAT enabled")
            send_packet(self, src_port_dev, udp_pkt)
            verify_packet(self, nat_udp_pkt, self.egr_port_dev)
            self.assertTrue(self._verifyNatHit(snat))
            print("\tOK")

            print("Disabling NAT on src RIF")
            sai_thrift_set_router_interface_attribute(
                self.client, src_rif, ingress_acl=self.ingr_acl_table)

            print("Sending TCP packet with NAT disabled by ACL")
            send_packet(self, src_port_dev, tcp_pkt)
            verify_packet(self, no_nat_tcp_pkt, self.egr_port_dev)
            self.assertTrue(self._verifyAclCounter(self.ingr_acl_counter,
                                                   acl_counter))
            acl_counter += 1
            print("\tOK")

            print("Sending UDP packet with NAT disabled by ACL")
            send_packet(self, src_port_dev, udp_pkt)
            verify_packet(self, no_nat_udp_pkt, self.egr_port_dev)
            self.assertTrue(self._verifyAclCounter(self.ingr_acl_counter,
                                                   acl_counter))
            acl_counter += 1
            print("\tOK")

            print("Enabling NAT on src RIF")
            sai_thrift_set_router_interface_attribute(
                self.client, src_rif, ingress_acl=0)

            # use route to L3 LAG
            print("   -> Egress L3 LAG")
            tcp_pkt[IP].dst = self.lag_nbor_ip

            nat_tcp_pkt[Ether].dst = self.lag_nbor_mac
            nat_tcp_pkt[IP].dst = self.lag_nbor_ip
            no_nat_tcp_pkt[Ether].dst = self.lag_nbor_mac
            no_nat_tcp_pkt[IP].dst = self.lag_nbor_ip

            udp_pkt[IP].dst = self.lag_nbor_ip
            nat_udp_pkt[Ether].dst = self.lag_nbor_mac
            nat_udp_pkt[IP].dst = self.lag_nbor_ip
            no_nat_udp_pkt[Ether].dst = self.lag_nbor_mac
            no_nat_udp_pkt[IP].dst = self.lag_nbor_ip

            print("Sending TCP packet with NAT enabled")
            send_packet(self, src_port_dev, tcp_pkt)
            verify_packet_any_port(self, nat_tcp_pkt, self.egr_lag_dev)
            self.assertTrue(self._verifyNatHit(snat))
            print("\tOK")

            print("Sending UDP packet with NAT enabled")
            send_packet(self, src_port_dev, udp_pkt)
            verify_packet_any_port(self, nat_udp_pkt, self.egr_lag_dev)
            self.assertTrue(self._verifyNatHit(snat))
            print("\tOK")

            print("Disabling NAT on src RIF")
            sai_thrift_set_router_interface_attribute(
                self.client, src_rif, ingress_acl=self.ingr_acl_table)

            print("Sending TCP packet with NAT disabled by ACL")
            send_packet(self, src_port_dev, tcp_pkt)
            verify_packet_any_port(self, no_nat_tcp_pkt, self.egr_lag_dev)
            self.assertTrue(self._verifyAclCounter(self.ingr_acl_counter,
                                                   acl_counter))
            acl_counter += 1
            print("\tOK")

            print("Sending UDP packet with NAT disabled by ACL")
            send_packet(self, src_port_dev, udp_pkt)
            verify_packet_any_port(self, no_nat_udp_pkt, self.egr_lag_dev)
            self.assertTrue(self._verifyAclCounter(self.ingr_acl_counter,
                                                   acl_counter))
            acl_counter += 1
            print("\tOK")

            print("Enabling NAT on src RIF")
            sai_thrift_set_router_interface_attribute(
                self.client, src_rif, ingress_acl=0)

            # use route to SVI
            print("   -> Egress SVI")
            tcp_pkt[IP].dst = self.svi_nbor_ip

            nat_tcp_pkt[Ether].dst = self.svi_nbor_mac
            nat_tcp_pkt[IP].dst = self.svi_nbor_ip
            no_nat_tcp_pkt[Ether].dst = self.svi_nbor_mac
            no_nat_tcp_pkt[IP].dst = self.svi_nbor_ip

            udp_pkt[IP].dst = self.svi_nbor_ip
            nat_udp_pkt[Ether].dst = self.svi_nbor_mac
            nat_udp_pkt[IP].dst = self.svi_nbor_ip
            no_nat_udp_pkt[Ether].dst = self.svi_nbor_mac
            no_nat_udp_pkt[IP].dst = self.svi_nbor_ip

            print("Sending TCP packet with NAT enabled")
            send_packet(self, src_port_dev, tcp_pkt)
            verify_packets(self, nat_tcp_pkt, self.egr_svi_dev)
            self.assertTrue(self._verifyNatHit(snat))
            print("\tOK")

            print("Sending UDP packet with NAT enabled")
            send_packet(self, src_port_dev, udp_pkt)
            verify_packets(self, nat_udp_pkt, self.egr_svi_dev)
            self.assertTrue(self._verifyNatHit(snat))
            print("\tOK")

            print("Disabling NAT on src RIF")
            sai_thrift_set_router_interface_attribute(
                self.client, src_rif, ingress_acl=self.ingr_acl_table)

            print("Sending TCP packet with NAT disabled by ACL")
            send_packet(self, src_port_dev, tcp_pkt)
            verify_packets(self, no_nat_tcp_pkt, self.egr_svi_dev)
            self.assertTrue(self._verifyAclCounter(self.ingr_acl_counter,
                                                   acl_counter))
            acl_counter += 1
            print("\tOK")

            print("Sending UDP packet with NAT disabled by ACL")
            send_packet(self, src_port_dev, udp_pkt)
            verify_packets(self, no_nat_udp_pkt, self.egr_svi_dev)
            self.assertTrue(self._verifyAclCounter(self.ingr_acl_counter,
                                                   acl_counter))
            acl_counter += 1
            print("\tOK")

            print("Enabling NAT on src RIF")
            sai_thrift_set_router_interface_attribute(
                self.client, src_rif, ingress_acl=0)

        try:
            nat_data = sai_thrift_nat_entry_data_t(
                key=sai_thrift_nat_entry_key_t(
                    src_ip=src_ip),
                mask=sai_thrift_nat_entry_mask_t(
                    src_ip='255.255.255.255'))

            snat = sai_thrift_nat_entry_t(vr_id=self.default_vrf,
                                          data=nat_data,
                                          nat_type=SAI_NAT_TYPE_SOURCE_NAT)
            sai_thrift_create_nat_entry(self.client,
                                        snat,
                                        src_ip=nat_src_ip,
                                        nat_type=SAI_NAT_TYPE_SOURCE_NAT,
                                        enable_packet_count=True)

            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_port_rif, nat_zone_id=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.egr_port_rif, nat_zone_id=1)

            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_lag_rif, nat_zone_id=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.egr_lag_rif, nat_zone_id=1)

            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_svi_rif, nat_zone_id=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.egr_svi_rif, nat_zone_id=1)

            print("\n***Ingress L3 port***")
            verify_translation(self.ingr_port_rif, self.ingr_port_dev)

            print("\n***Ingress L3 LAG***")
            for lag_port in self.ingr_lag_dev:
                verify_translation(self.ingr_lag_rif, lag_port)

            print("\n***Ingress SVI***")
            for svi_port in self.ingr_svi_dev:
                verify_translation(self.ingr_svi_rif, svi_port)

        finally:
            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_svi_rif, ingress_acl=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_lag_rif, ingress_acl=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_port_rif, ingress_acl=0)

            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_svi_rif, nat_zone_id=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.egr_svi_rif, nat_zone_id=0)

            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_lag_rif, nat_zone_id=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.egr_lag_rif, nat_zone_id=0)

            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_port_rif, nat_zone_id=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.egr_port_rif, nat_zone_id=0)

            sai_thrift_remove_nat_entry(self.client, snat)

    def dstNatAclTranslationDisableTest(self):
        '''
        Verifies if translation doesn't occur when destination NAT entries
        exist but ACL is configured to disable NAT translation.
        Test is performed for different RIFs (regular L3 port, L3 LAG and SVI)
        and different nexhtops (regular L3 port, L3 LAG and SVI)
        with two kinds of packets - TCP and UDP.
        '''
        print("\ndstNatAclTranslationDisableTest()")

        def verify_translation(dst_rif):
            '''
            Additional helper function for translation verification.
            Verifies if translation doesn't occur when ACL is configured
            to disable it.

            Args:
                dst_rif (oid): object ID of destination RIF
            '''
            acl_counter = sai_thrift_get_acl_counter_attribute(
                self.client, self.egr_acl_counter, packets=True)['packets']

            src_ip = "20.20.20.1"

            verify_fn = verify_packet
            dst_port_dev = self.egr_port_dev
            dst_ip = self.nat_ip_to_port
            dst_mac = self.port_nbor_mac
            nat_dst_ip = self.port_nbor_ip
            dnat = port_dnat
            if dst_rif == self.egr_lag_rif:
                verify_fn = verify_packet_any_port
                dst_port_dev = self.egr_lag_dev
                dst_ip = self.nat_ip_to_lag
                dst_mac = self.lag_nbor_mac
                nat_dst_ip = self.lag_nbor_ip
                dnat = lag_dnat
            elif dst_rif == self.egr_svi_rif:
                verify_fn = verify_packets
                dst_port_dev = self.egr_svi_dev
                dst_ip = self.nat_ip_to_svi
                dst_mac = self.svi_nbor_mac
                nat_dst_ip = self.svi_nbor_ip
                dnat = svi_dnat

            tcp_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                        ip_src=src_ip,
                                        ip_dst=dst_ip,
                                        ip_ttl=64,
                                        pktlen=100,
                                        with_tcp_chksum=True)
            nat_tcp_pkt = simple_tcp_packet(eth_dst=dst_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_src=src_ip,
                                            ip_dst=nat_dst_ip,
                                            ip_ttl=63,
                                            pktlen=100,
                                            with_tcp_chksum=True)
            no_nat_tcp_pkt = simple_tcp_packet(eth_dst=self.no_nat_nbor_mac,
                                               eth_src=ROUTER_MAC,
                                               ip_src=src_ip,
                                               ip_dst=dst_ip,
                                               ip_ttl=63,
                                               pktlen=100,
                                               with_tcp_chksum=True)

            udp_pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                        ip_src=src_ip,
                                        ip_dst=dst_ip,
                                        ip_ttl=64,
                                        pktlen=100)
            nat_udp_pkt = simple_udp_packet(eth_dst=dst_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_src=src_ip,
                                            ip_dst=nat_dst_ip,
                                            ip_ttl=63,
                                            pktlen=100)
            no_nat_udp_pkt = simple_udp_packet(eth_dst=self.no_nat_nbor_mac,
                                               eth_src=ROUTER_MAC,
                                               ip_src=src_ip,
                                               ip_dst=dst_ip,
                                               ip_ttl=63,
                                               pktlen=100)

            print("   Inress L3 port")
            print("Sending TCP packet with NAT enabled on L3 Port")
            send_packet(self, self.ingr_port_dev, tcp_pkt)
            verify_fn(self, nat_tcp_pkt, dst_port_dev)
            self.assertTrue(self._verifyNatHit(dnat))
            print("\tOK")

            print("Sending UDP packet with NAT enabled on L3 Port")
            send_packet(self, self.ingr_port_dev, udp_pkt)
            verify_fn(self, nat_udp_pkt, dst_port_dev)
            self.assertTrue(self._verifyNatHit(dnat))
            print("\tOK")

            print("Disabling NAT on src RIF")
            sai_thrift_set_router_interface_attribute(
                self.client,
                self.ingr_port_rif,
                ingress_acl=self.egr_acl_table)

            print("Sending TCP packet with NAT disabled by ACL on L3 Port")
            send_packet(self, self.ingr_port_dev, tcp_pkt)
            verify_packet(self, no_nat_tcp_pkt, self.no_nat_eport)
            self.assertTrue(self._verifyAclCounter(self.egr_acl_counter,
                                                   acl_counter))
            acl_counter += 1
            print("\tOK")

            print("Sending UDP packet with NAT disabled by ACL on L3 Port")
            send_packet(self, self.ingr_port_dev, udp_pkt)
            verify_packet(self, no_nat_udp_pkt, self.no_nat_eport)
            self.assertTrue(self._verifyAclCounter(self.egr_acl_counter,
                                                   acl_counter))
            acl_counter += 1
            print("\tOK")

            print("Enabling NAT on src RIF")
            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_port_rif, ingress_acl=0)

            print("   Inress L3 LAG")
            for src_port in self.ingr_lag_dev:
                print("Sending TCP packet with NAT enabled on L3 LAG")
                send_packet(self, src_port, tcp_pkt)
                verify_fn(self, nat_tcp_pkt, dst_port_dev)
                self.assertTrue(self._verifyNatHit(dnat))
                print("\tOK")

                print("Sending UDP packet with NAT enabled on L3 LAG")
                send_packet(self, src_port, udp_pkt)
                verify_fn(self, nat_udp_pkt, dst_port_dev)
                self.assertTrue(self._verifyNatHit(dnat))
                print("\tOK")

                print("Disabling NAT on src RIF")
                sai_thrift_set_router_interface_attribute(
                    self.client,
                    self.ingr_lag_rif,
                    ingress_acl=self.egr_acl_table)

                print("Sending TCP packet with NAT disabled by ACL on L3 LAG")
                send_packet(self, src_port, tcp_pkt)
                verify_packet(self, no_nat_tcp_pkt, self.no_nat_eport)
                self.assertTrue(self._verifyAclCounter(self.egr_acl_counter,
                                                       acl_counter))
                acl_counter += 1
                print("\tOK")

                print("Sending UDP packet with NAT disabled by ACL on L3 LAG")
                send_packet(self, src_port, udp_pkt)
                verify_packet(self, no_nat_udp_pkt, self.no_nat_eport)
                self.assertTrue(self._verifyAclCounter(self.egr_acl_counter,
                                                       acl_counter))
                acl_counter += 1
                print("\tOK")

                print("Enabling NAT on src RIF")
                sai_thrift_set_router_interface_attribute(
                    self.client, self.ingr_lag_rif, ingress_acl=0)

            print("   Inress SVI")
            for src_port in self.ingr_svi_dev:
                print("Sending TCP packet with NAT enabled on SVI")
                send_packet(self, src_port, tcp_pkt)
                verify_fn(self, nat_tcp_pkt, dst_port_dev)
                self.assertTrue(self._verifyNatHit(dnat))
                print("\tOK")

                print("Sending UDP packet with NAT enabled on SVI")
                send_packet(self, src_port, udp_pkt)
                verify_fn(self, nat_udp_pkt, dst_port_dev)
                self.assertTrue(self._verifyNatHit(dnat))
                print("\tOK")

                print("Disabling NAT on src RIF")
                sai_thrift_set_router_interface_attribute(
                    self.client,
                    self.ingr_svi_rif,
                    ingress_acl=self.egr_acl_table)

                print("Sending TCP packet with NAT disabled by ACL on SVI")
                send_packet(self, src_port, tcp_pkt)
                verify_packet(self, no_nat_tcp_pkt, self.no_nat_eport)
                self.assertTrue(self._verifyAclCounter(self.egr_acl_counter,
                                                       acl_counter))
                acl_counter += 1
                print("\tOK")

                print("Sending UDP packet with NAT disabled by ACL on SVI")
                send_packet(self, src_port, udp_pkt)
                verify_packet(self, no_nat_udp_pkt, self.no_nat_eport)
                self.assertTrue(self._verifyAclCounter(self.egr_acl_counter,
                                                       acl_counter))
                acl_counter += 1
                print("\tOK")

                print("Enabling NAT on src RIF")
                sai_thrift_set_router_interface_attribute(
                    self.client, self.ingr_svi_rif, ingress_acl=0)

        try:
            # NAT configuration
            port_nat_data = sai_thrift_nat_entry_data_t(
                key=sai_thrift_nat_entry_key_t(
                    dst_ip=self.nat_ip_to_port),
                mask=sai_thrift_nat_entry_mask_t(
                    dst_ip='255.255.255.255'))

            port_dnat = sai_thrift_nat_entry_t(
                vr_id=self.default_vrf, data=port_nat_data,
                nat_type=SAI_NAT_TYPE_DESTINATION_NAT)
            sai_thrift_create_nat_entry(
                self.client, port_dnat, dst_ip=self.port_nbor_ip,
                nat_type=SAI_NAT_TYPE_DESTINATION_NAT,
                enable_packet_count=True)

            port_dnat_pool = sai_thrift_nat_entry_t(
                vr_id=self.default_vrf, data=port_nat_data,
                nat_type=SAI_NAT_TYPE_DESTINATION_NAT_POOL)
            sai_thrift_create_nat_entry(
                self.client, port_dnat_pool, dst_ip=self.port_nbor_ip,
                nat_type=SAI_NAT_TYPE_DESTINATION_NAT_POOL)

            lag_nat_data = sai_thrift_nat_entry_data_t(
                key=sai_thrift_nat_entry_key_t(
                    dst_ip=self.nat_ip_to_lag),
                mask=sai_thrift_nat_entry_mask_t(
                    dst_ip='255.255.255.255'))

            lag_dnat = sai_thrift_nat_entry_t(
                vr_id=self.default_vrf, data=lag_nat_data,
                nat_type=SAI_NAT_TYPE_DESTINATION_NAT)
            sai_thrift_create_nat_entry(
                self.client, lag_dnat, dst_ip=self.lag_nbor_ip,
                nat_type=SAI_NAT_TYPE_DESTINATION_NAT,
                enable_packet_count=True)

            lag_dnat_pool = sai_thrift_nat_entry_t(
                vr_id=self.default_vrf, data=lag_nat_data,
                nat_type=SAI_NAT_TYPE_DESTINATION_NAT_POOL)
            sai_thrift_create_nat_entry(
                self.client, lag_dnat_pool, dst_ip=self.lag_nbor_ip,
                nat_type=SAI_NAT_TYPE_DESTINATION_NAT_POOL)

            svi_nat_data = sai_thrift_nat_entry_data_t(
                key=sai_thrift_nat_entry_key_t(
                    dst_ip=self.nat_ip_to_svi),
                mask=sai_thrift_nat_entry_mask_t(
                    dst_ip='255.255.255.255'))

            svi_dnat = sai_thrift_nat_entry_t(
                vr_id=self.default_vrf, data=svi_nat_data,
                nat_type=SAI_NAT_TYPE_DESTINATION_NAT)
            sai_thrift_create_nat_entry(
                self.client, svi_dnat, dst_ip=self.svi_nbor_ip,
                nat_type=SAI_NAT_TYPE_DESTINATION_NAT,
                enable_packet_count=True)

            svi_dnat_pool = sai_thrift_nat_entry_t(
                vr_id=self.default_vrf, data=svi_nat_data,
                nat_type=SAI_NAT_TYPE_DESTINATION_NAT_POOL)
            sai_thrift_create_nat_entry(
                self.client, svi_dnat_pool, dst_ip=self.svi_nbor_ip,
                nat_type=SAI_NAT_TYPE_DESTINATION_NAT_POOL)

            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_port_rif, nat_zone_id=1)
            sai_thrift_set_router_interface_attribute(
                self.client, self.egr_port_rif, nat_zone_id=0)

            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_lag_rif, nat_zone_id=1)
            sai_thrift_set_router_interface_attribute(
                self.client, self.egr_lag_rif, nat_zone_id=0)

            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_svi_rif, nat_zone_id=1)
            sai_thrift_set_router_interface_attribute(
                self.client, self.egr_svi_rif, nat_zone_id=0)

            print("\n***Egress L3 port <-***")
            verify_translation(self.egr_port_rif)
            print("\n***Egress L3 LAG <-***")
            verify_translation(self.egr_lag_rif)
            print("\n***Egress SVI <-***")
            verify_translation(self.egr_svi_rif)

        finally:
            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_svi_rif, ingress_acl=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_lag_rif, ingress_acl=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_port_rif, ingress_acl=0)

            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_svi_rif, nat_zone_id=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.egr_svi_rif, nat_zone_id=0)

            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_lag_rif, nat_zone_id=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.egr_lag_rif, nat_zone_id=0)

            sai_thrift_set_router_interface_attribute(
                self.client, self.ingr_port_rif, nat_zone_id=0)
            sai_thrift_set_router_interface_attribute(
                self.client, self.egr_port_rif, nat_zone_id=0)

            sai_thrift_remove_nat_entry(self.client, svi_dnat_pool)
            sai_thrift_remove_nat_entry(self.client, svi_dnat)

            sai_thrift_remove_nat_entry(self.client, lag_dnat_pool)
            sai_thrift_remove_nat_entry(self.client, lag_dnat)

            sai_thrift_remove_nat_entry(self.client, port_dnat_pool)
            sai_thrift_remove_nat_entry(self.client, port_dnat)


@group("draft")
class NatTest(SaiHelper):
    '''
    Basic NAT configuration test
    Test topology:
                         +--------------DUT--------------+
                         |   zone 0              zone 1  |
           Nbr2          | server_port          wan_port |          Nbr1
      (192.168.0.1)   ---|  (port24)            (port25) |---   (20.20.20.1)
    00:33:44:55:66:77    |                               |    00:11:22:33:44:55
                         | 192.168.0.1 <-> 200.200.200.1 |
                         +-------------------------------+
    '''

    def setUp(self):
        super(NatTest, self).setUp()

        self.rif_list = []
        self.nhop_list = []
        self.nbor_list = []
        self.route_list = []
        self.nat_list = []

        self.nbor_mac = '00:11:22:33:44:55'
        self.dmac = '00:22:22:22:22:22'
        self.src_ip = '192.168.0.10'
        self.dst_ip = '10.10.10.1'
        self.nhop_ip = '20.20.20.1'
        self.server_ip = '192.168.0.1'
        self.server_dmac = '00:33:44:55:66:77'
        self.translate_server_ip = '200.200.200.1'
        mask = '/24'

        # Route configuration
        server_port = self.port24
        wan_port = self.port25

        server_rif = sai_thrift_create_router_interface(
            client=self.client, virtual_router_id=self.default_vrf,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT, port_id=server_port,
            nat_zone_id=0)
        self.rif_list.append(server_rif)
        wan_rif = sai_thrift_create_router_interface(
            client=self.client, virtual_router_id=self.default_vrf,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT, port_id=wan_port,
            nat_zone_id=1)
        self.rif_list.append(wan_rif)

        print("Create nieghbor with %s ip address, %d router interface id and "
              "%s destination mac" % (self.nhop_ip, wan_rif, self.nbor_mac))
        nbr_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=wan_rif,
            ip_address=sai_ipaddress(self.nhop_ip))
        sai_thrift_create_neighbor_entry(client=self.client,
                                         neighbor_entry=nbr_entry1,
                                         dst_mac_address=self.nbor_mac)
        self.nbor_list.append(nbr_entry1)

        print("Create nhop with %s ip address and %d router interface id"
              % (self.nhop_ip, wan_rif))
        nhop1 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(self.nhop_ip),
            router_interface_id=wan_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nhop_list.append(nhop1)

        # 10.10.10.0/24 --> NHOP1(WAN)
        route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.dst_ip + mask))
        sai_thrift_create_route_entry(client=self.client,
                                      route_entry=route_entry1,
                                      next_hop_id=nhop1)
        self.route_list.append(route_entry1)

        route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.nhop_ip + mask))
        sai_thrift_create_route_entry(client=self.client,
                                      route_entry=route_entry2,
                                      next_hop_id=wan_rif)
        self.route_list.append(route_entry2)

        # 192.168.0.1/24 --> NHOP(SERVER)
        print("Create nieghbor with %s ip address, %d router interface id and "
              "%s destination mac"
              % (self.server_ip, server_rif, self.server_dmac))
        nbr_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=server_rif,
            ip_address=sai_ipaddress(self.server_ip))
        sai_thrift_create_neighbor_entry(client=self.client,
                                         neighbor_entry=nbr_entry2,
                                         dst_mac_address=self.server_dmac)
        self.nbor_list.append(nbr_entry2)

        print("Create nhop with %s ip address and %d router interface id"
              % (self.server_ip, server_rif))
        nhop2 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(self.server_ip),
            router_interface_id=server_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nhop_list.append(nhop2)

        route_entry3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.server_ip + mask))
        sai_thrift_create_route_entry(client=self.client,
                                      route_entry=route_entry3,
                                      next_hop_id=nhop2)
        self.route_list.append(route_entry3)

        # NAT configuration
        # Translated server IP - 20.20.20.1
        # WAN IP - 10.10.10.1
        # self.server_ip = '192.168.0.1'

        self.l4_src_port = 100
        self.l4_dst_port = 1234
        proto = 6

        self.translate_sport = 500
        self.translate_dport = 2000

        nat_type = SAI_NAT_TYPE_DESTINATION_NAT
        # set NAT output dst_port
        nat_data = sai_thrift_nat_entry_data_t(
            key=sai_thrift_nat_entry_key_t(
                dst_ip=self.translate_server_ip,
                proto=proto,
                l4_dst_port=self.l4_dst_port))
        dnat1 = sai_thrift_nat_entry_t(vr_id=self.default_vrf,
                                       data=nat_data,
                                       nat_type=nat_type)
        # set SAI_NAT_ENTRY_ATTR_DST_IP and SAI_NAT_ENTRY_ATTR_L4_DST_PORT
        # (match values)
        sai_thrift_create_nat_entry(self.client,
                                    nat_entry=dnat1,
                                    nat_type=nat_type,
                                    dst_ip=self.server_ip,
                                    l4_dst_port=self.translate_dport)
        self.nat_list.append(dnat1)

        # set NAT output dst_ip
        nat_data = sai_thrift_nat_entry_data_t(
            key=sai_thrift_nat_entry_key_t(
                dst_ip=self.translate_server_ip))
        dnat2 = sai_thrift_nat_entry_t(vr_id=self.default_vrf,
                                       data=nat_data,
                                       nat_type=nat_type)
        # set SAI_NAT_ENTRY_ATTR_DST_IP (match value)
        sai_thrift_create_nat_entry(self.client,
                                    nat_entry=dnat2,
                                    nat_type=nat_type,
                                    dst_ip=self.server_ip)
        self.nat_list.append(dnat2)

        # set NAT output dst_ip
        nat_type = SAI_NAT_TYPE_DESTINATION_NAT_POOL
        nat_data = sai_thrift_nat_entry_data_t(
            key=sai_thrift_nat_entry_key_t(
                dst_ip=self.translate_server_ip))
        dnat3 = sai_thrift_nat_entry_t(vr_id=self.default_vrf,
                                       data=nat_data,
                                       nat_type=nat_type)
        # set SAI_NAT_ENTRY_ATTR_DST_IP (match value)
        sai_thrift_create_nat_entry(self.client,
                                    nat_entry=dnat3,
                                    nat_type=nat_type,
                                    dst_ip=self.server_ip)
        self.nat_list.append(dnat3)

        nat_type = SAI_NAT_TYPE_SOURCE_NAT
        # set NAT output l4_src_port
        nat_data = sai_thrift_nat_entry_data_t(
            key=sai_thrift_nat_entry_key_t(
                src_ip=self.server_ip,
                proto=proto,
                l4_src_port=self.l4_src_port))
        snat4 = sai_thrift_nat_entry_t(vr_id=self.default_vrf,
                                       data=nat_data,
                                       nat_type=nat_type)
        # set SAI_NAT_ENTRY_ATTR_SRC_IP and SAI_NAT_ENTRY_ATTR_L4_SRC_PORT
        # (match values)
        sai_thrift_create_nat_entry(self.client,
                                    nat_entry=snat4,
                                    nat_type=nat_type,
                                    src_ip=self.translate_server_ip,
                                    l4_src_port=self.translate_sport)
        self.nat_list.append(snat4)

        # set NAT output src_ip
        nat_data = sai_thrift_nat_entry_data_t(
            key=sai_thrift_nat_entry_key_t(
                src_ip=self.server_ip))
        snat5 = sai_thrift_nat_entry_t(vr_id=self.default_vrf,
                                       data=nat_data,
                                       nat_type=nat_type)
        # set SAI_NAT_ENTRY_ATTR_SRC_IP and (match value)
        sai_thrift_create_nat_entry(self.client,
                                    nat_entry=snat5,
                                    nat_type=nat_type,
                                    src_ip=self.translate_server_ip)
        self.nat_list.append(snat5)

    def runTest(self):
        self.natRouteTest()
        self.natTrapTest()
        self.srcNatTest()
        self.dstNatTest()

    def tearDown(self):
        # Routing
        for route in list(self.route_list):
            sai_thrift_remove_route_entry(self.client, route)

        for nhop in list(self.nhop_list):
            sai_thrift_remove_next_hop(self.client, nhop)

        for nbr in list(self.nbor_list):
            sai_thrift_remove_neighbor_entry(self.client, nbr)

        for rif in list(self.rif_list):
            sai_thrift_remove_router_interface(self.client, rif)

        # NAT
        for nat in list(self.nat_list):
            sai_thrift_remove_nat_entry(self.client, nat)

        super(NatTest, self).tearDown()

    def natTrapTest(self):
        '''
        Verifies trap configuration
        '''
        print("\nnatTrapTest()")
        try:
            trap_group = sai_thrift_create_hostif_trap_group(
                self.client, queue=4)

            dnat_trap = sai_thrift_create_hostif_trap(
                client=self.client,
                trap_type=SAI_HOSTIF_TRAP_TYPE_DNAT_MISS,
                packet_action=SAI_PACKET_ACTION_TRAP,
                trap_group=trap_group)

            snat_trap = sai_thrift_create_hostif_trap(
                client=self.client,
                trap_type=SAI_HOSTIF_TRAP_TYPE_SNAT_MISS,
                packet_action=SAI_PACKET_ACTION_TRAP,
                trap_group=trap_group)

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.dmac,
                                    ip_dst=self.dst_ip,
                                    ip_src=self.src_ip,
                                    ip_id=105,
                                    ip_ttl=64)

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            send_packet(self, self.dev_port24, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

        finally:
            sai_thrift_remove_hostif_trap(self.client, dnat_trap)
            sai_thrift_remove_hostif_trap(self.client, snat_trap)
            sai_thrift_remove_hostif_trap_group(self.client, trap_group)

    def natRouteTest(self):
        '''
        Verifies routing configuration
        '''
        print("\nnatRouteTest()")

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.dmac,
                                ip_dst=self.dst_ip,
                                ip_src=self.src_ip,
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst=self.nbor_mac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=self.dst_ip,
                                    ip_src=self.src_ip,
                                    ip_id=105,
                                    ip_ttl=63)
        send_packet(self, self.dev_port24, pkt)
        verify_packets(self, exp_pkt, [self.dev_port25])

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.dmac,
                                ip_dst=self.nhop_ip,
                                ip_src=self.src_ip,
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst=self.nbor_mac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=self.nhop_ip,
                                    ip_src=self.src_ip,
                                    ip_id=105,
                                    ip_ttl=63)
        send_packet(self, self.dev_port24, pkt)
        verify_packets(self, exp_pkt, [self.dev_port25])

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.dmac,
                                ip_dst=self.server_ip,
                                ip_src=self.nhop_ip,
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst=self.server_dmac,
                                    eth_src=ROUTER_MAC,
                                    ip_src=self.nhop_ip,
                                    ip_dst=self.server_ip,
                                    ip_id=105,
                                    ip_ttl=63)
        send_packet(self, self.dev_port25, pkt)
        verify_packets(self, exp_pkt, [self.dev_port24])

    def srcNatTest(self):
        '''
        Verifies source NAT
        '''
        print("\nsrcNatTest()")

        # Validate server to WAN
        # Translate server-ip to public ip
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.dmac,
                                ip_dst=self.dst_ip,
                                ip_src=self.server_ip,
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst=self.nbor_mac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=self.dst_ip,
                                    ip_src=self.translate_server_ip,
                                    ip_id=105,
                                    ip_ttl=63)
        send_packet(self, self.dev_port24, pkt)
        verify_packets(self, exp_pkt, [self.dev_port25])

        # Translate server-ip + port to public ip + port
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.dmac,
                                ip_dst=self.dst_ip,
                                ip_src=self.server_ip,
                                ip_id=105,
                                ip_ttl=64,
                                tcp_sport=self.l4_src_port)
        exp_pkt = simple_tcp_packet(eth_dst=self.nbor_mac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=self.dst_ip,
                                    ip_src=self.translate_server_ip,
                                    ip_id=105,
                                    ip_ttl=63,
                                    tcp_sport=self.translate_sport)
        send_packet(self, self.dev_port24, pkt)
        verify_packets(self, exp_pkt, [self.dev_port25])

        ret_attr = sai_thrift_get_nat_entry_attribute(
            client=self.client,
            nat_entry=self.nat_list[3],
            packet_count=True)
        self.assertEqual(ret_attr["packet_count"], 1)
        print("Packet count %d" % (ret_attr["packet_count"]))

        ret_attr = sai_thrift_get_nat_entry_attribute(
            client=self.client,
            nat_entry=self.nat_list[3],
            hit_bit=True)
        self.assertEqual(ret_attr["hit_bit"], True)
        print("1st query - Hit bit %r" % (ret_attr["hit_bit"]))

        ret_attr = sai_thrift_get_nat_entry_attribute(
            client=self.client,
            nat_entry=self.nat_list[3],
            hit_bit=True)
        self.assertEqual(ret_attr["hit_bit"], False)
        print("2nd query - Hit bit %r" % (ret_attr["hit_bit"]))

        sai_thrift_set_nat_entry_attribute(
            client=self.client,
            nat_entry=self.nat_list[3],
            packet_count=0)
        ret_attr = sai_thrift_get_nat_entry_attribute(
            client=self.client,
            nat_entry=self.nat_list[3],
            packet_count=True)
        self.assertEqual(ret_attr["packet_count"], 0)
        print("Packet count %d" % (ret_attr["packet_count"]))

    def dstNatTest(self):
        '''
        Verifies destination NAT
        '''
        print("\ndstNatTest()")

        # Translate public IP to server IP
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.dmac,
                                ip_dst=self.translate_server_ip,
                                ip_src=self.dst_ip,
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst=self.server_dmac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=self.server_ip,
                                    ip_src=self.dst_ip,
                                    ip_id=105,
                                    ip_ttl=63)
        send_packet(self, self.dev_port25, pkt)
        verify_packets(self, exp_pkt, [self.dev_port24])

        # Translate public IP + port to server IP + port
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.dmac,
                                ip_dst=self.translate_server_ip,
                                ip_src=self.dst_ip,
                                ip_id=105,
                                ip_ttl=64,
                                tcp_dport=self.l4_dst_port)
        exp_pkt = simple_tcp_packet(eth_dst=self.server_dmac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=self.server_ip,
                                    ip_src=self.dst_ip,
                                    ip_id=105,
                                    ip_ttl=63,
                                    tcp_dport=self.translate_dport)
        send_packet(self, self.dev_port25, pkt)
        verify_packets(self, exp_pkt, [self.dev_port24])

        ret_attr = sai_thrift_get_nat_entry_attribute(
            client=self.client,
            nat_entry=self.nat_list[0],
            packet_count=True)
        self.assertEqual(ret_attr["packet_count"], 1)
        print("Packet count %d" % (ret_attr["packet_count"]))
