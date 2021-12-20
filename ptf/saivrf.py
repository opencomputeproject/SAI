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
Thrift SAI interface VRF tests
"""

import binascii

from sai_thrift.sai_headers import *

from sai_base_test import *


@group("draft")
class VrfForwardingTest(SaiHelper):
    '''
    Base inter-VRF forwarding tests
    '''

    def setUp(self):
        super(VrfForwardingTest, self).setUp()

        self.route_list = []
        self.nbor_list = []
        self.nhop_list = []

        self.test_vrf = sai_thrift_create_virtual_router(self.client)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        # create and configure ingress RIF
        self.iport = self.port24
        self.iport_dev = self.dev_port24
        self.test_irif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.test_vrf,
            port_id=self.iport)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.iport_nbor_mac = "00:11:11:11:11:11"
        iport_ipv4 = "10.10.10.1"
        iport_ipv6 = "2001:0db8::1:1"

        self.iport_nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(iport_ipv4),
            router_interface_id=self.test_irif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nhop_list.append(self.iport_nhop)
        self.iport_nhop_v6 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(iport_ipv6),
            router_interface_id=self.test_irif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nhop_list.append(self.iport_nhop_v6)

        self.iport_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.test_irif, ip_address=sai_ipaddress(iport_ipv4))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.iport_nbor,
                                         dst_mac_address=self.iport_nbor_mac)
        self.nbor_list.append(self.iport_nbor)
        self.iport_nbor_v6 = sai_thrift_neighbor_entry_t(
            rif_id=self.test_irif, ip_address=sai_ipaddress(iport_ipv6))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.iport_nbor_v6,
                                         dst_mac_address=self.iport_nbor_mac)
        self.nbor_list.append(self.iport_nbor_v6)

        self.iport_route = sai_thrift_route_entry_t(
            vr_id=self.test_vrf, destination=sai_ipprefix(iport_ipv4 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.iport_route,
                                      next_hop_id=self.iport_nhop)
        self.route_list.append(self.iport_route)
        self.iport_route_v6 = sai_thrift_route_entry_t(
            vr_id=self.test_vrf, destination=sai_ipprefix(iport_ipv6 + '/112'))
        sai_thrift_create_route_entry(self.client,
                                      self.iport_route_v6,
                                      next_hop_id=self.iport_nhop_v6)
        self.route_list.append(self.iport_route_v6)

        # create and configure egress RIF
        self.eport = self.port25
        self.eport_dev = self.dev_port25
        self.test_erif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.test_vrf,
            port_id=self.eport)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.eport_nbor_mac = "00:22:22:22:22:22"
        eport_ipv4 = "20.20.20.2"
        eport_ipv6 = "2001:0db8::2:2"

        self.eport_nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(eport_ipv4),
            router_interface_id=self.test_erif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nhop_list.append(self.eport_nhop)
        self.eport_nhop_v6 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(eport_ipv6),
            router_interface_id=self.test_erif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nhop_list.append(self.eport_nhop_v6)

        self.eport_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.test_erif, ip_address=sai_ipaddress(eport_ipv4))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.eport_nbor,
                                         dst_mac_address=self.eport_nbor_mac)
        self.nbor_list.append(self.eport_nbor)
        self.eport_nbor_v6 = sai_thrift_neighbor_entry_t(
            rif_id=self.test_erif, ip_address=sai_ipaddress(eport_ipv6))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.eport_nbor_v6,
                                         dst_mac_address=self.eport_nbor_mac)
        self.nbor_list.append(self.eport_nbor_v6)

        self.eport_route = sai_thrift_route_entry_t(
            vr_id=self.test_vrf, destination=sai_ipprefix(eport_ipv4 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.eport_route,
                                      next_hop_id=self.eport_nhop)
        self.route_list.append(self.eport_route)
        self.eport_route_v6 = sai_thrift_route_entry_t(
            vr_id=self.test_vrf, destination=sai_ipprefix(eport_ipv6 + '/112'))
        sai_thrift_create_route_entry(self.client,
                                      self.eport_route_v6,
                                      next_hop_id=self.eport_nhop_v6)
        self.route_list.append(self.eport_route_v6)

    def runTest(self):
        self.vrfStateTest()
        self.interVrfFwdL3NhopTest()
        self.interVrfFwdL3LagNhopTest()
        self.interVrfFwdSviNhopTest()
        self.interVrfFwdSubportNhopTest()
        self.interVrfFwdEcmpNhopTest()

    def tearDown(self):
        for route in self.route_list:
            sai_thrift_remove_route_entry(self.client, route)
        for nbor in self.nbor_list:
            sai_thrift_remove_neighbor_entry(self.client, nbor)
        for nhop in self.nhop_list:
            sai_thrift_remove_next_hop(self.client, nhop)

        sai_thrift_remove_router_interface(self.client, self.test_erif)
        sai_thrift_remove_router_interface(self.client, self.test_irif)
        sai_thrift_remove_virtual_router(self.client, self.test_vrf)

        super(VrfForwardingTest, self).tearDown()

    def vrfStateTest(self):
        '''
        Verify forwarding for different IPv4 and IPv6 states on VRF
        '''
        print("\nvrfStateTest()")

        src_mac = self.iport_nbor_mac
        src_ip = "10.10.10.10"
        src_ipv6 = "2001:0db8::1:10"
        dst_ip = "20.20.20.20"
        dst_ipv6 = "2001:0db8::2:20"

        pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                eth_src=src_mac,
                                ip_dst=dst_ip,
                                ip_src=src_ip,
                                ip_ttl=64)
        exp_pkt = simple_udp_packet(eth_dst=self.eport_nbor_mac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=dst_ip,
                                    ip_src=src_ip,
                                    ip_ttl=63)

        ipv6_pkt = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                       eth_src=src_mac,
                                       ipv6_dst=dst_ipv6,
                                       ipv6_src=src_ipv6,
                                       ipv6_hlim=64)
        exp_ipv6_pkt = simple_udpv6_packet(eth_dst=self.eport_nbor_mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ipv6,
                                           ipv6_src=src_ipv6,
                                           ipv6_hlim=63)

        try:
            # IPv4 disabled, IPv6 disabled
            sai_thrift_set_virtual_router_attribute(
                self.client, self.test_vrf, admin_v4_state=False)
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
            sai_thrift_set_virtual_router_attribute(
                self.client, self.test_vrf, admin_v6_state=False)
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

            print("IPv4 disabled, IPv6 disabled")
            send_packet(self, self.iport_dev, pkt)
            verify_no_other_packets(self)
            print("\tIPv4 dropped")
            send_packet(self, self.iport_dev, ipv6_pkt)
            verify_no_other_packets(self)
            print("\tIPv6 dropped")

            # IPv4 enabled, IPv6 disabled
            sai_thrift_set_virtual_router_attribute(
                self.client, self.test_vrf, admin_v4_state=True)
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

            print("IPv4 enabled, IPv6 disabled")
            send_packet(self, self.iport_dev, pkt)
            verify_packet(self, exp_pkt, self.eport_dev)
            print("\tIPv4 forwarded")
            send_packet(self, self.iport_dev, ipv6_pkt)
            verify_no_other_packets(self)
            print("\tIPv6 dropped")

            # IPv4 disabled, IPv6 enabled
            sai_thrift_set_virtual_router_attribute(
                self.client, self.test_vrf, admin_v4_state=False)
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
            sai_thrift_set_virtual_router_attribute(
                self.client, self.test_vrf, admin_v6_state=True)
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

            print("IPv4 disabled, IPv6 enabled")
            send_packet(self, self.iport_dev, pkt)
            verify_no_other_packets(self)
            print("\tIPv4 dropped")
            send_packet(self, self.iport_dev, ipv6_pkt)
            verify_packet(self, exp_ipv6_pkt, self.eport_dev)
            print("\tIPv6 forwarded")

            # IPv4 enabled, IPv6 enabled
            sai_thrift_set_virtual_router_attribute(
                self.client, self.test_vrf, admin_v4_state=True)
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

            print("IPv4 enabled, IPv6 enabled")
            send_packet(self, self.iport_dev, pkt)
            verify_packet(self, exp_pkt, self.eport_dev)
            print("\tIPv4 forwarded")
            send_packet(self, self.iport_dev, ipv6_pkt)
            verify_packet(self, exp_ipv6_pkt, self.eport_dev)
            print("\tIPv6 forwarded")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            sai_thrift_set_virtual_router_attribute(self.client,
                                                    self.test_vrf,
                                                    admin_v4_state=True)
            sai_thrift_set_virtual_router_attribute(self.client,
                                                    self.test_vrf,
                                                    admin_v6_state=True)

    def interVrfFwdL3NhopTest(self):
        '''
        Verify inter VRF forwarding with regular L3 nexthop
        '''
        print("\ninterVrfFwdL3NhopTest()")

        test_dev_port = self.dev_port10
        test_rif = self.port10_rif

        test_mac = "00:33:33:33:33:33"
        test_ip = "30.30.30.1"

        try:
            test_nbor = sai_thrift_neighbor_entry_t(
                rif_id=test_rif, ip_address=sai_ipaddress(test_ip))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor,
                                             dst_mac_address=test_mac)

            test_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip),
                router_interface_id=test_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            test_route = sai_thrift_route_entry_t(
                vr_id=self.test_vrf, destination=sai_ipprefix(test_ip + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          test_route,
                                          next_hop_id=test_nhop)

            test_route2 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(test_ip + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          test_route2,
                                          next_hop_id=test_nhop)

            src_mac = self.iport_nbor_mac
            src_ip = "10.10.10.10"
            dst_ip = "30.30.30.30"

            pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip,
                                    ip_src=src_ip,
                                    ip_ttl=64)
            exp_pkt = simple_udp_packet(eth_dst=test_mac,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=dst_ip,
                                        ip_src=src_ip,
                                        ip_ttl=63)

            print("Sending inter-VRF packet to regular L3 nexthop")
            send_packet(self, self.iport_dev, pkt)
            verify_packet(self, exp_pkt, test_dev_port)
            print("\tOK")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            sai_thrift_remove_route_entry(self.client, test_route)
            sai_thrift_remove_route_entry(self.client, test_route2)
            sai_thrift_remove_next_hop(self.client, test_nhop)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor)

    def interVrfFwdL3LagNhopTest(self):
        '''
        Verify inter VRF forwarding with regular L3 LAG nexthop
        '''
        print("\ninterVrfFwdL3LagNhopTest()")

        test_lag_dev_ports = [self.dev_port14, self.dev_port15,
                              self.dev_port16]
        test_lag_rif = self.lag3_rif

        test_mac = "00:33:33:33:33:33"
        test_ip = "30.30.30.1"

        try:
            test_nbor = sai_thrift_neighbor_entry_t(
                rif_id=test_lag_rif, ip_address=sai_ipaddress(test_ip))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor,
                                             dst_mac_address=test_mac)

            test_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip),
                router_interface_id=test_lag_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            test_route = sai_thrift_route_entry_t(
                vr_id=self.test_vrf, destination=sai_ipprefix(test_ip + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          test_route,
                                          next_hop_id=test_nhop)

            test_route2 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(test_ip + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          test_route2,
                                          next_hop_id=test_nhop)

            src_mac = self.iport_nbor_mac
            src_ip = "10.10.10.10"
            dst_ip = "30.30.30.30"

            pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip,
                                    ip_src=src_ip,
                                    ip_ttl=64)
            exp_pkt = simple_udp_packet(eth_dst=test_mac,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=dst_ip,
                                        ip_src=src_ip,
                                        ip_ttl=63)

            print("Sending inter-VRF packet to L3 LAG nexthop")
            send_packet(self, self.iport_dev, pkt)
            verify_packet_any_port(self, exp_pkt, test_lag_dev_ports)
            print("\tOK")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            sai_thrift_remove_route_entry(self.client, test_route)
            sai_thrift_remove_route_entry(self.client, test_route2)
            sai_thrift_remove_next_hop(self.client, test_nhop)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor)

    def interVrfFwdSviNhopTest(self):
        '''
        Verify inter VRF forwarding with SVI nexthop
        '''
        print("\ninterVrfFwdSviNhopTest()")

        vlan_id = 30
        test_vlan_rif = self.vlan30_rif
        test_vlan_lag_dev_ports = [self.dev_port22, self.dev_port23]
        test_vlan_dev_ports = [[self.dev_port20], [self.dev_port21],
                               test_vlan_lag_dev_ports]

        test_mac = "00:33:33:33:33:33"
        test_ip = "30.30.30.1"

        try:
            test_nbor = sai_thrift_neighbor_entry_t(
                rif_id=test_vlan_rif, ip_address=sai_ipaddress(test_ip))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor,
                                             dst_mac_address=test_mac)

            test_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip),
                router_interface_id=test_vlan_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            test_route = sai_thrift_route_entry_t(
                vr_id=self.test_vrf, destination=sai_ipprefix(test_ip + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          test_route,
                                          next_hop_id=test_nhop)

            test_route2 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(test_ip + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          test_route2,
                                          next_hop_id=test_nhop)

            src_mac = self.iport_nbor_mac
            src_ip = "10.10.10.10"
            dst_ip = "30.30.30.30"

            pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip,
                                    ip_src=src_ip,
                                    ip_ttl=64,
                                    pktlen=100)
            exp_pkt = simple_udp_packet(eth_dst=test_mac,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=dst_ip,
                                        ip_src=src_ip,
                                        ip_ttl=63,
                                        pktlen=100)
            exp_tag_pkt = simple_udp_packet(eth_dst=test_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_dst=dst_ip,
                                            ip_src=src_ip,
                                            dl_vlan_enable=True,
                                            vlan_vid=vlan_id,
                                            ip_ttl=63,
                                            pktlen=104)

            exp_pkt_list = [exp_pkt, exp_tag_pkt, exp_tag_pkt]

            print("Sending inter-VRF packet to SVI nexthop")
            send_packet(self, self.iport_dev, pkt)
            verify_each_packet_on_multiple_port_lists(self, exp_pkt_list,
                                                      test_vlan_dev_ports)
            print("\tOK")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            sai_thrift_remove_route_entry(self.client, test_route)
            sai_thrift_remove_route_entry(self.client, test_route2)
            sai_thrift_remove_next_hop(self.client, test_nhop)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor)

    def interVrfFwdSubportNhopTest(self):
        '''
        Verify inter VRF forwarding with subport nexthop
        '''
        print("\ninterVrfFwdSubportNhopTest()")

        test_port = self.port26
        test_dev_port = self.dev_port26

        test_mac1 = "00:33:33:33:33:1"
        test_ip1 = "30.30.10.1"
        vlan_id1 = 100
        test_mac2 = "00:33:33:33:33:2"
        test_ip2 = "30.30.20.2"
        vlan_id2 = 200

        try:
            subport1_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
                virtual_router_id=self.default_vrf,
                port_id=test_port,
                outer_vlan_id=vlan_id1,
                admin_v4_state=True)

            test_nbor1 = sai_thrift_neighbor_entry_t(
                rif_id=subport1_rif, ip_address=sai_ipaddress(test_ip1))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor1,
                                             dst_mac_address=test_mac1)

            test_nhop1 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip1),
                router_interface_id=subport1_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            test_route11 = sai_thrift_route_entry_t(
                vr_id=self.test_vrf,
                destination=sai_ipprefix(test_ip1 + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          test_route11,
                                          next_hop_id=test_nhop1)

            test_route12 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(test_ip1 + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          test_route12,
                                          next_hop_id=test_nhop1)

            subport2_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
                virtual_router_id=self.default_vrf,
                port_id=test_port,
                outer_vlan_id=vlan_id2,
                admin_v4_state=True)

            test_nbor2 = sai_thrift_neighbor_entry_t(
                rif_id=subport2_rif, ip_address=sai_ipaddress(test_ip2))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor2,
                                             dst_mac_address=test_mac2)

            test_nhop2 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip2),
                router_interface_id=subport2_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            test_route21 = sai_thrift_route_entry_t(
                vr_id=self.test_vrf,
                destination=sai_ipprefix(test_ip2 + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          test_route21,
                                          next_hop_id=test_nhop2)

            test_route22 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(test_ip2 + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          test_route22,
                                          next_hop_id=test_nhop2)

            src_mac = self.iport_nbor_mac
            src_ip = "10.10.10.10"
            dst_ip1 = "30.30.10.10"
            dst_ip2 = "30.30.20.20"

            pkt1 = simple_udp_packet(eth_dst=ROUTER_MAC,
                                     eth_src=src_mac,
                                     ip_dst=dst_ip1,
                                     ip_src=src_ip,
                                     ip_ttl=64,
                                     pktlen=100)
            exp_pkt1 = simple_udp_packet(eth_dst=test_mac1,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip1,
                                         ip_src=src_ip,
                                         dl_vlan_enable=True,
                                         vlan_vid=vlan_id1,
                                         ip_ttl=63,
                                         pktlen=104)

            pkt2 = simple_udp_packet(eth_dst=ROUTER_MAC,
                                     eth_src=src_mac,
                                     ip_dst=dst_ip2,
                                     ip_src=src_ip,
                                     ip_ttl=64,
                                     pktlen=100)
            exp_pkt2 = simple_udp_packet(eth_dst=test_mac2,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip2,
                                         ip_src=src_ip,
                                         dl_vlan_enable=True,
                                         vlan_vid=vlan_id2,
                                         ip_ttl=63,
                                         pktlen=104)

            print("Sending inter-VRF packets to 2 subport nexthops")
            send_packet(self, self.iport_dev, pkt1)
            verify_packet(self, exp_pkt1, test_dev_port)

            send_packet(self, self.iport_dev, pkt2)
            verify_packet(self, exp_pkt2, test_dev_port)
            print("\tOK")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            sai_thrift_remove_route_entry(self.client, test_route11)
            sai_thrift_remove_route_entry(self.client, test_route12)
            sai_thrift_remove_route_entry(self.client, test_route21)
            sai_thrift_remove_route_entry(self.client, test_route22)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor1)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor2)
            sai_thrift_remove_next_hop(self.client, test_nhop1)
            sai_thrift_remove_next_hop(self.client, test_nhop2)
            sai_thrift_remove_router_interface(self.client, subport1_rif)
            sai_thrift_remove_router_interface(self.client, subport2_rif)

    def interVrfFwdEcmpNhopTest(self):
        '''
        Verify inter VRF forwarding with ECMP nexthop
        '''
        print("\ninterVrfFwdEcmpNhopTest()")

        test_dev_ports = [self.dev_port10, self.dev_port11, self.dev_port12]
        test_rifs = [self.port10_rif, self.port11_rif, self.port12_rif]

        test_mac1 = "00:33:33:33:33:11"
        test_mac2 = "00:33:33:33:33:22"
        test_mac3 = "00:33:33:33:33:33"
        test_ip1 = "30.30.30.1"
        test_ip2 = "30.30.30.2"
        test_ip3 = "30.30.30.3"

        try:
            test_nbor1 = sai_thrift_neighbor_entry_t(
                rif_id=test_rifs[0], ip_address=sai_ipaddress(test_ip1))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor1,
                                             dst_mac_address=test_mac1)

            test_nbor2 = sai_thrift_neighbor_entry_t(
                rif_id=test_rifs[1], ip_address=sai_ipaddress(test_ip2))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor2,
                                             dst_mac_address=test_mac2)

            test_nbor3 = sai_thrift_neighbor_entry_t(
                rif_id=test_rifs[2], ip_address=sai_ipaddress(test_ip3))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor3,
                                             dst_mac_address=test_mac3)

            test_nhop1 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip1),
                router_interface_id=test_rifs[0],
                type=SAI_NEXT_HOP_TYPE_IP)

            test_nhop2 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip2),
                router_interface_id=test_rifs[1],
                type=SAI_NEXT_HOP_TYPE_IP)

            test_nhop3 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip3),
                router_interface_id=test_rifs[2],
                type=SAI_NEXT_HOP_TYPE_IP)

            test_nhop_group = sai_thrift_create_next_hop_group(
                self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
            nhop_group_mmbr1 = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=test_nhop_group,
                next_hop_id=test_nhop1)
            nhop_group_mmbr2 = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=test_nhop_group,
                next_hop_id=test_nhop2)
            nhop_group_mmbr3 = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=test_nhop_group,
                next_hop_id=test_nhop3)

            test_ecmp_route = sai_thrift_route_entry_t(
                vr_id=self.test_vrf,
                destination=sai_ipprefix(test_ip1 + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          test_ecmp_route,
                                          next_hop_id=test_nhop_group)

            src_mac = "00:aa:aa:aa:aa:aa"
            src_ip_addr = "10.10.10.1"
            dst_ip_addr = "30.30.30.30"

            print("Sending set of inter-VRF packets to ECMP nexthop")
            max_iter = 100
            count = [0, 0, 0]
            src_ip_addr = int(
                binascii.hexlify(
                    socket.inet_aton(src_ip_addr)), 16)
            dst_ip_addr = int(
                binascii.hexlify(
                    socket.inet_aton(dst_ip_addr)), 16)
            for _ in range(max_iter):
                src_ip = socket.inet_ntoa(
                    binascii.unhexlify(format(src_ip_addr, 'x').zfill(8)))
                dst_ip = socket.inet_ntoa(
                    binascii.unhexlify(format(dst_ip_addr, 'x').zfill(8)))

                pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                        eth_src=src_mac,
                                        ip_dst=dst_ip,
                                        ip_src=src_ip,
                                        ip_ttl=64)
                exp_pkt1 = simple_udp_packet(eth_dst=test_mac1,
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip,
                                             ip_src=src_ip,
                                             ip_ttl=63)
                exp_pkt2 = simple_udp_packet(eth_dst=test_mac2,
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip,
                                             ip_src=src_ip,
                                             ip_ttl=63)
                exp_pkt3 = simple_udp_packet(eth_dst=test_mac3,
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip,
                                             ip_src=src_ip,
                                             ip_ttl=63)

                send_packet(self, self.iport_dev, pkt)
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2, exp_pkt3], test_dev_ports)

                count[rcv_idx] += 1
                src_ip_addr += 1
                dst_ip_addr += 1

            print("\tOK")

            for _ in range(len(count)):
                self.assertTrue(count[1] >= ((max_iter / len(count)) * 0.7),
                                "Ecmp paths are not equally balanced")

        finally:
            sai_thrift_remove_route_entry(self.client, test_ecmp_route)
            sai_thrift_remove_next_hop_group_member(
                self.client, nhop_group_mmbr1)
            sai_thrift_remove_next_hop_group_member(
                self.client, nhop_group_mmbr2)
            sai_thrift_remove_next_hop_group_member(
                self.client, nhop_group_mmbr3)
            sai_thrift_remove_next_hop_group(self.client, test_nhop_group)
            sai_thrift_remove_next_hop(self.client, test_nhop1)
            sai_thrift_remove_next_hop(self.client, test_nhop2)
            sai_thrift_remove_next_hop(self.client, test_nhop3)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor1)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor2)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor3)


@group("draft")
class VrfIsolationTest(SaiHelper):
    '''
    Verify forwarding with overlapping IP addresses - when the same
    addresses in different VRFs point to different nexthops
    '''

    def setUp(self):
        super(VrfIsolationTest, self).setUp()

        self.rif_list = []

        self.test_vrf = sai_thrift_create_virtual_router(self.client)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.iport_ipv4 = "10.10.10.1"
        self.iport_ipv6 = "2001:0db8::1:1"
        self.eport_ipv4 = "20.20.20.2"
        self.eport_ipv6 = "2001:0db8::2:2"

        self.def_iport_mac = "00:aa:aa:11:11:11"
        self.def_eport_mac = "00:aa:aa:22:22:22"
        self.test_iport_mac = "00:bb:bb:11:11:11"
        self.test_eport_mac = "00:bb:bb:22:22:22"

        # ports in default VRF (24 and 25)
        self.def_iport = self.port24
        self.def_iport_dev = self.dev_port24
        self.def_irif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.def_iport)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.def_irif)

        self.def_eport = self.port25
        self.def_eport_dev = self.dev_port25
        self.def_erif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.def_eport)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.def_erif)

        # ports in test VRF (26 and 27)
        self.test_iport = self.port26
        self.test_iport_dev = self.dev_port26
        self.test_irif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.test_vrf,
            port_id=self.test_iport)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.test_irif)

        self.test_eport = self.port27
        self.test_eport_dev = self.dev_port27
        self.test_erif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.test_vrf,
            port_id=self.test_eport)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.test_erif)

    def runTest(self):
        self.overlappingIPv4LpmTest()
        self.overlappingIPv4HostTest()
        self.overlappingIPv6LpmTest()
        self.overlappingIPv6HostTest()

    def tearDown(self):
        for rif in self.rif_list:
            sai_thrift_remove_router_interface(self.client, rif)

        sai_thrift_remove_virtual_router(self.client, self.test_vrf)

        super(VrfIsolationTest, self).tearDown()

    def overlappingIPv4LpmTest(self):
        '''
        Verify overlapping IPv4 address with LPM match
        '''
        print("\noverlappingIPv4LpmTest()")

        src_ipv4 = "10.10.10.10"
        dst_ipv4 = "20.20.20.20"

        def_v4_pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                       eth_src=self.def_iport_mac,
                                       ip_dst=dst_ipv4,
                                       ip_src=src_ipv4,
                                       ip_ttl=64)
        def_v4_exp_pkt = simple_udp_packet(eth_dst=self.def_eport_mac,
                                           eth_src=ROUTER_MAC,
                                           ip_dst=dst_ipv4,
                                           ip_src=src_ipv4,
                                           ip_ttl=63)

        test_v4_pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                        eth_src=self.test_iport_mac,
                                        ip_dst=dst_ipv4,
                                        ip_src=src_ipv4,
                                        ip_ttl=64)
        test_v4_exp_pkt = simple_udp_packet(eth_dst=self.test_eport_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_dst=dst_ipv4,
                                            ip_src=src_ipv4,
                                            ip_ttl=63)

        try:
            # configure default VRF nexthop
            def_eport_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(self.eport_ipv4),
                router_interface_id=self.def_erif,
                type=SAI_NEXT_HOP_TYPE_IP)

            def_eport_nbor = sai_thrift_neighbor_entry_t(
                rif_id=self.def_erif,
                ip_address=sai_ipaddress(self.eport_ipv4))
            sai_thrift_create_neighbor_entry(
                self.client,
                def_eport_nbor,
                dst_mac_address=self.def_eport_mac)

            def_eport_route = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(self.eport_ipv4 + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          def_eport_route,
                                          next_hop_id=def_eport_nhop)

            # configure test VRF nexthop
            test_eport_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(self.eport_ipv4),
                router_interface_id=self.test_erif,
                type=SAI_NEXT_HOP_TYPE_IP)

            test_eport_nbor = sai_thrift_neighbor_entry_t(
                rif_id=self.test_erif,
                ip_address=sai_ipaddress(self.eport_ipv4))
            sai_thrift_create_neighbor_entry(
                self.client,
                test_eport_nbor,
                dst_mac_address=self.test_eport_mac)

            test_eport_route = sai_thrift_route_entry_t(
                vr_id=self.test_vrf,
                destination=sai_ipprefix(self.eport_ipv4 + '/24'))
            sai_thrift_create_route_entry(self.client,
                                          test_eport_route,
                                          next_hop_id=test_eport_nhop)

            print("Verifying forwarding within default VRF")
            send_packet(self, self.def_iport_dev, def_v4_pkt)
            verify_packet(self, def_v4_exp_pkt, self.def_eport_dev)
            print("\tOK")

            print("Verifying forwarding within test VRF")
            send_packet(self, self.test_iport_dev, test_v4_pkt)
            verify_packet(self, test_v4_exp_pkt, self.test_eport_dev)
            print("\tOK")

        finally:
            sai_thrift_remove_route_entry(self.client, test_eport_route)
            sai_thrift_remove_neighbor_entry(self.client, test_eport_nbor)
            sai_thrift_remove_next_hop(self.client, test_eport_nhop)

            sai_thrift_remove_route_entry(self.client, def_eport_route)
            sai_thrift_remove_neighbor_entry(self.client, def_eport_nbor)
            sai_thrift_remove_next_hop(self.client, def_eport_nhop)

    def overlappingIPv4HostTest(self):
        '''
        Verify overlapping IPv4 address with exact match
        '''
        print("\noverlappingIPv4HostTest()")

        def_v4_pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                       eth_src=self.def_iport_mac,
                                       ip_dst=self.eport_ipv4,
                                       ip_src=self.iport_ipv4,
                                       ip_ttl=64)
        def_v4_exp_pkt = simple_udp_packet(eth_dst=self.def_eport_mac,
                                           eth_src=ROUTER_MAC,
                                           ip_dst=self.eport_ipv4,
                                           ip_src=self.iport_ipv4,
                                           ip_ttl=63)

        test_v4_pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                        eth_src=self.test_iport_mac,
                                        ip_dst=self.eport_ipv4,
                                        ip_src=self.iport_ipv4,
                                        ip_ttl=64)
        test_v4_exp_pkt = simple_udp_packet(eth_dst=self.test_eport_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_dst=self.eport_ipv4,
                                            ip_src=self.iport_ipv4,
                                            ip_ttl=63)

        try:
            def_eport_nbor = sai_thrift_neighbor_entry_t(
                rif_id=self.def_erif,
                ip_address=sai_ipaddress(self.eport_ipv4))
            sai_thrift_create_neighbor_entry(
                self.client,
                def_eport_nbor,
                dst_mac_address=self.def_eport_mac)

            test_eport_nbor = sai_thrift_neighbor_entry_t(
                rif_id=self.test_erif,
                ip_address=sai_ipaddress(self.eport_ipv4))
            sai_thrift_create_neighbor_entry(
                self.client,
                test_eport_nbor,
                dst_mac_address=self.test_eport_mac)

            print("Verifying forwarding within default VRF")
            send_packet(self, self.def_iport_dev, def_v4_pkt)
            verify_packet(self, def_v4_exp_pkt, self.def_eport_dev)
            print("\tOK")

            print("Verifying forwarding within test VRF")
            send_packet(self, self.test_iport_dev, test_v4_pkt)
            verify_packet(self, test_v4_exp_pkt, self.test_eport_dev)
            print("\tOK")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, test_eport_nbor)
            sai_thrift_remove_neighbor_entry(self.client, def_eport_nbor)

    def overlappingIPv6LpmTest(self):
        '''
        Verify overlapping IPv6 address with LPM match
        '''
        print("\noverlappingIPv6LpmTest()")

        src_ipv6 = "2001:0db8::1:10"
        dst_ipv6 = "2001:0db8::2:20"

        def_v6_pkt = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                         eth_src=self.def_iport_mac,
                                         ipv6_dst=dst_ipv6,
                                         ipv6_src=src_ipv6,
                                         ipv6_hlim=64)
        def_v6_exp_pkt = simple_udpv6_packet(eth_dst=self.def_eport_mac,
                                             eth_src=ROUTER_MAC,
                                             ipv6_dst=dst_ipv6,
                                             ipv6_src=src_ipv6,
                                             ipv6_hlim=63)

        test_v6_pkt = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src=self.test_iport_mac,
                                          ipv6_dst=dst_ipv6,
                                          ipv6_src=src_ipv6,
                                          ipv6_hlim=64)
        test_v6_exp_pkt = simple_udpv6_packet(eth_dst=self.test_eport_mac,
                                              eth_src=ROUTER_MAC,
                                              ipv6_dst=dst_ipv6,
                                              ipv6_src=src_ipv6,
                                              ipv6_hlim=63)

        try:
            # configure default VRF nexthop
            def_eport_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(self.eport_ipv6),
                router_interface_id=self.def_erif,
                type=SAI_NEXT_HOP_TYPE_IP)

            def_eport_nbor = sai_thrift_neighbor_entry_t(
                rif_id=self.def_erif,
                ip_address=sai_ipaddress(self.eport_ipv6))
            sai_thrift_create_neighbor_entry(
                self.client,
                def_eport_nbor,
                dst_mac_address=self.def_eport_mac)

            def_eport_route = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(self.eport_ipv6 + '/112'))
            sai_thrift_create_route_entry(self.client,
                                          def_eport_route,
                                          next_hop_id=def_eport_nhop)

            # configure test VRF nexthop
            test_eport_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(self.eport_ipv6),
                router_interface_id=self.test_erif,
                type=SAI_NEXT_HOP_TYPE_IP)

            test_eport_nbor = sai_thrift_neighbor_entry_t(
                rif_id=self.test_erif,
                ip_address=sai_ipaddress(self.eport_ipv6))
            sai_thrift_create_neighbor_entry(
                self.client,
                test_eport_nbor,
                dst_mac_address=self.test_eport_mac)

            test_eport_route = sai_thrift_route_entry_t(
                vr_id=self.test_vrf,
                destination=sai_ipprefix(self.eport_ipv6 + '/112'))
            sai_thrift_create_route_entry(self.client,
                                          test_eport_route,
                                          next_hop_id=test_eport_nhop)

            print("Verifying forwarding within default VRF")
            send_packet(self, self.def_iport_dev, def_v6_pkt)
            verify_packet(self, def_v6_exp_pkt, self.def_eport_dev)
            print("\tOK")

            print("Verifying forwarding within test VRF")
            send_packet(self, self.test_iport_dev, test_v6_pkt)
            verify_packet(self, test_v6_exp_pkt, self.test_eport_dev)
            print("\tOK")

        finally:
            sai_thrift_remove_route_entry(self.client, test_eport_route)
            sai_thrift_remove_neighbor_entry(self.client, test_eport_nbor)
            sai_thrift_remove_next_hop(self.client, test_eport_nhop)

            sai_thrift_remove_route_entry(self.client, def_eport_route)
            sai_thrift_remove_neighbor_entry(self.client, def_eport_nbor)
            sai_thrift_remove_next_hop(self.client, def_eport_nhop)

    def overlappingIPv6HostTest(self):
        '''
        Verify overlapping IPv6 address with exact match
        '''
        print("\noverlappingIPv6HostTest()")

        def_v6_pkt = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                         eth_src=self.def_iport_mac,
                                         ipv6_dst=self.eport_ipv6,
                                         ipv6_src=self.iport_ipv6,
                                         ipv6_hlim=64)
        def_v6_exp_pkt = simple_udpv6_packet(eth_dst=self.def_eport_mac,
                                             eth_src=ROUTER_MAC,
                                             ipv6_dst=self.eport_ipv6,
                                             ipv6_src=self.iport_ipv6,
                                             ipv6_hlim=63)

        test_v6_pkt = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src=self.test_iport_mac,
                                          ipv6_dst=self.eport_ipv6,
                                          ipv6_src=self.iport_ipv6,
                                          ipv6_hlim=64)
        test_v6_exp_pkt = simple_udpv6_packet(eth_dst=self.test_eport_mac,
                                              eth_src=ROUTER_MAC,
                                              ipv6_dst=self.eport_ipv6,
                                              ipv6_src=self.iport_ipv6,
                                              ipv6_hlim=63)

        try:
            # configure default VRF nexthop
            def_eport_nbor = sai_thrift_neighbor_entry_t(
                rif_id=self.def_erif,
                ip_address=sai_ipaddress(self.eport_ipv6))
            sai_thrift_create_neighbor_entry(
                self.client,
                def_eport_nbor,
                dst_mac_address=self.def_eport_mac)

            # configure test VRF nexthop
            test_eport_nbor = sai_thrift_neighbor_entry_t(
                rif_id=self.test_erif,
                ip_address=sai_ipaddress(self.eport_ipv6))
            sai_thrift_create_neighbor_entry(
                self.client,
                test_eport_nbor,
                dst_mac_address=self.test_eport_mac)

            print("Verifying forwarding within default VRF")
            send_packet(self, self.def_iport_dev, def_v6_pkt)
            verify_packet(self, def_v6_exp_pkt, self.def_eport_dev)
            print("\tOK")

            print("Verifying forwarding within test VRF")
            send_packet(self, self.test_iport_dev, test_v6_pkt)
            verify_packet(self, test_v6_exp_pkt, self.test_eport_dev)
            print("\tOK")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, test_eport_nbor)
            sai_thrift_remove_neighbor_entry(self.client, def_eport_nbor)


@group("draft")
class VrfMultipleRifCreationTest(SaiHelper):
    '''
    Verify multiple RIF creation of type PORT, LAG, VLAN (with tagged
    and untagged members), SUB_PORT and LOOPBACK for three different VRFs,
    with routes pointing to each RIF type
    '''

    def setUp(self):
        super(VrfMultipleRifCreationTest, self).setUp()

        self.route_list = []
        self.nhop_list = []
        self.nbor_list = []
        self.rif_list = []

        # VRF 1 configuration
        self.vrf1 = sai_thrift_create_virtual_router(self.client)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.port_rif1 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.vrf1,
            port_id=self.port24)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.port_rif1)

        self.lag_rif1 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.vrf1,
            port_id=self.lag1)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.lag_rif1)

        self.sub_port_rif1 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.vrf1,
            port_id=self.port10,
            outer_vlan_id=100,
            admin_v4_state=True)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.sub_port_rif1)

        self.lpb_rif1 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.vrf1)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.lpb_rif1)

        self.svi_rif1 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.vrf1,
            vlan_id=self.vlan10)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.svi_rif1)

        self.ip_addr11 = "10.10.1.1"
        self.mac11 = "00:11:11:11:11:11"
        self.port_nbor1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port_rif1, ip_address=sai_ipaddress(self.ip_addr11))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.port_nbor1,
                                         dst_mac_address=self.mac11)
        self.nbor_list.append(self.port_nbor1)

        self.port_nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr11),
            router_interface_id=self.port_rif1,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.port_nhop1)

        self.port_route1 = sai_thrift_route_entry_t(
            vr_id=self.vrf1, destination=sai_ipprefix(self.ip_addr11 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                               self.port_route1,
                                               next_hop_id=self.port_nhop1)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.port_route1)

        self.ip_addr12 = "10.10.2.1"
        self.mac12 = "00:11:11:11:11:22"
        self.lag_nbor1 = sai_thrift_neighbor_entry_t(
            rif_id=self.lag_rif1, ip_address=sai_ipaddress(self.ip_addr12))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.lag_nbor1,
                                         dst_mac_address=self.mac12)
        self.nbor_list.append(self.lag_nbor1)

        self.lag_nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr12),
            router_interface_id=self.lag_rif1,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.lag_nhop1)

        self.lag_route1 = sai_thrift_route_entry_t(
            vr_id=self.vrf1, destination=sai_ipprefix(self.ip_addr12 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                               self.lag_route1,
                                               next_hop_id=self.lag_nhop1)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.lag_route1)

        self.ip_addr13 = "10.10.3.1"
        self.mac13 = "00:11:11:11:11:33"
        self.sub_port_nbor1 = sai_thrift_neighbor_entry_t(
            rif_id=self.sub_port_rif1,
            ip_address=sai_ipaddress(self.ip_addr13))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.sub_port_nbor1,
                                         dst_mac_address=self.mac13)
        self.nbor_list.append(self.sub_port_nbor1)

        self.sub_port_nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr13),
            router_interface_id=self.sub_port_rif1,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.sub_port_nhop1)

        self.sub_port_route1 = sai_thrift_route_entry_t(
            vr_id=self.vrf1, destination=sai_ipprefix(self.ip_addr13 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                               self.sub_port_route1,
                                               next_hop_id=self.sub_port_nhop1)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.sub_port_route1)

        self.ip_addr14 = "10.10.4.1"
        self.lpb_nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr14),
            router_interface_id=self.lpb_rif1,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.lpb_nhop1)

        self.lpb_route1 = sai_thrift_route_entry_t(
            vr_id=self.vrf1, destination=sai_ipprefix(self.ip_addr14 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                               self.lpb_route1,
                                               next_hop_id=self.lpb_nhop1)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.lpb_route1)

        self.ip_addr15 = "10.10.5.1"
        self.mac15 = "00:11:11:11:11:55"
        self.svi_nbor1 = sai_thrift_neighbor_entry_t(
            rif_id=self.svi_rif1, ip_address=sai_ipaddress(self.ip_addr15))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.svi_nbor1,
                                         dst_mac_address=self.mac15)
        self.nbor_list.append(self.svi_nbor1)

        self.svi_nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr15),
            router_interface_id=self.svi_rif1,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.svi_nhop1)

        self.svi_route1 = sai_thrift_route_entry_t(
            vr_id=self.vrf1, destination=sai_ipprefix(self.ip_addr15 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                               self.svi_route1,
                                               next_hop_id=self.svi_nhop1)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.svi_route1)

        # VRF 2 configuration
        self.vrf2 = sai_thrift_create_virtual_router(self.client)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.port_rif2 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.vrf2,
            port_id=self.port25)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.port_rif2)

        self.lag_rif2 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.vrf2,
            port_id=self.lag2)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.lag_rif2)

        self.sub_port_rif2 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.vrf2,
            port_id=self.port10,
            outer_vlan_id=200,
            admin_v4_state=True)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.sub_port_rif2)

        self.lpb_rif2 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.vrf2)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.lpb_rif2)

        self.svi_rif2 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.vrf2,
            vlan_id=self.vlan20)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.svi_rif2)

        ip_addr21 = "10.20.1.1"
        self.port_nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ip_addr21),
            router_interface_id=self.port_rif2,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.port_nhop2)

        self.port_route2 = sai_thrift_route_entry_t(
            vr_id=self.vrf2, destination=sai_ipprefix(ip_addr21 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.port_route2,
                                      next_hop_id=self.port_nhop2)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.port_route2)

        ip_addr22 = "10.20.2.1"
        self.lag_nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ip_addr22),
            router_interface_id=self.lag_rif2,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.lag_nhop2)

        self.lag_route2 = sai_thrift_route_entry_t(
            vr_id=self.vrf2, destination=sai_ipprefix(ip_addr22 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.lag_route2,
                                      next_hop_id=self.lag_nhop2)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.lag_route2)

        ip_addr23 = "10.20.3.1"
        self.sub_port_nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ip_addr23),
            router_interface_id=self.sub_port_rif2,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.sub_port_nhop2)

        self.sub_port_route2 = sai_thrift_route_entry_t(
            vr_id=self.vrf2, destination=sai_ipprefix(ip_addr23 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.sub_port_route2,
                                      next_hop_id=self.sub_port_nhop2)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.sub_port_route2)

        ip_addr24 = "10.20.4.1"
        self.lpb_nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ip_addr24),
            router_interface_id=self.lpb_rif2,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.lpb_nhop2)

        self.lpb_route2 = sai_thrift_route_entry_t(
            vr_id=self.vrf2, destination=sai_ipprefix(ip_addr24 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.lpb_route2,
                                      next_hop_id=self.lpb_nhop2)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.lpb_route2)

        ip_addr25 = "10.20.5.1"
        self.svi_nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ip_addr25),
            router_interface_id=self.svi_rif2,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.svi_nhop2)

        self.svi_route2 = sai_thrift_route_entry_t(
            vr_id=self.vrf2, destination=sai_ipprefix(ip_addr25 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.svi_route2,
                                      next_hop_id=self.svi_nhop2)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.svi_route2)

        # VRF 3 configuration
        self.vrf3 = sai_thrift_create_virtual_router(self.client)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.port_rif3 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.vrf3,
            port_id=self.port26)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.port_rif3)

        # additional LAG
        self.lag10 = sai_thrift_create_lag(self.client)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.lag10_member1 = sai_thrift_create_lag_member(self.client,
                                                          lag_id=self.lag10,
                                                          port_id=self.port27)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.lag10_member2 = sai_thrift_create_lag_member(self.client,
                                                          lag_id=self.lag10,
                                                          port_id=self.port28)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.lag10_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag10,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.lag_rif3 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.vrf3,
            port_id=self.lag10)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.lag_rif3)

        self.sub_port_rif3 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.vrf3,
            port_id=self.port10,
            outer_vlan_id=300,
            admin_v4_state=True)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.sub_port_rif3)

        self.lpb_rif3 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.vrf3)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.lpb_rif3)

        # additional VLAN
        self.vlan100 = sai_thrift_create_vlan(self.client, vlan_id=100)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.port29_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port29,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.vlan100_member0 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port29_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        sai_thrift_set_port_attribute(self.client,
                                      self.port29,
                                      port_vlan_id=100)
        self.port30_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port30,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.vlan100_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port30_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.svi_rif3 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.vrf3,
            vlan_id=self.vlan100)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.rif_list.append(self.svi_rif3)

        ip_addr31 = "10.30.1.1"
        self.port_nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ip_addr31),
            router_interface_id=self.port_rif3,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.port_nhop3)

        self.port_route3 = sai_thrift_route_entry_t(
            vr_id=self.vrf3, destination=sai_ipprefix(ip_addr31 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.port_route3,
                                      next_hop_id=self.port_nhop3)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.port_route3)

        ip_addr32 = "10.30.2.1"
        self.lag_nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ip_addr32),
            router_interface_id=self.lag_rif3,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.lag_nhop3)

        self.lag_route3 = sai_thrift_route_entry_t(
            vr_id=self.vrf3, destination=sai_ipprefix(ip_addr32 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.lag_route3,
                                      next_hop_id=self.lag_nhop3)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.lag_route3)

        ip_addr33 = "10.30.3.1"
        self.sub_port_nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ip_addr33),
            router_interface_id=self.sub_port_rif3,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.sub_port_nhop3)

        self.sub_port_route3 = sai_thrift_route_entry_t(
            vr_id=self.vrf3, destination=sai_ipprefix(ip_addr33 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.sub_port_route3,
                                      next_hop_id=self.sub_port_nhop3)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.sub_port_route3)

        ip_addr34 = "10.30.4.1"
        self.lpb_nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ip_addr34),
            router_interface_id=self.lpb_rif3,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.lpb_nhop3)

        self.lpb_route3 = sai_thrift_route_entry_t(
            vr_id=self.vrf3, destination=sai_ipprefix(ip_addr34 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.lpb_route3,
                                      next_hop_id=self.lpb_nhop3)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.lpb_route3)

        ip_addr35 = "10.30.5.1"
        self.svi_nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ip_addr35),
            router_interface_id=self.svi_rif3,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.nhop_list.append(self.svi_nhop3)

        self.svi_route3 = sai_thrift_route_entry_t(
            vr_id=self.vrf3, destination=sai_ipprefix(ip_addr35 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.svi_route3,
                                      next_hop_id=self.svi_nhop3)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        self.route_list.append(self.svi_route3)

    def runTest(self):
        self.vrfInterfacesTest()

    def tearDown(self):
        for route in self.route_list:
            sai_thrift_remove_route_entry(self.client, route)
        for nhop in self.nhop_list:
            sai_thrift_remove_next_hop(self.client, nhop)
        for nbor in self.nbor_list:
            sai_thrift_remove_neighbor_entry(self.client, nbor)

        for rif in self.rif_list:
            sai_thrift_remove_router_interface(self.client, rif)

        sai_thrift_remove_vlan_member(self.client, self.vlan100_member1)
        sai_thrift_remove_bridge_port(self.client, self.port30_bp)
        sai_thrift_set_port_attribute(self.client, self.port29, port_vlan_id=0)
        sai_thrift_remove_vlan_member(self.client, self.vlan100_member0)
        sai_thrift_remove_bridge_port(self.client, self.port29_bp)
        sai_thrift_remove_vlan(self.client, self.vlan100)
        sai_thrift_remove_bridge_port(self.client, self.lag10_bp)
        sai_thrift_remove_lag_member(self.client, self.lag10_member1)
        sai_thrift_remove_lag_member(self.client, self.lag10_member2)
        sai_thrift_remove_lag(self.client, self.lag10)

        sai_thrift_remove_virtual_router(self.client, self.vrf3)
        sai_thrift_remove_virtual_router(self.client, self.vrf2)
        sai_thrift_remove_virtual_router(self.client, self.vrf1)

        super(VrfMultipleRifCreationTest, self).tearDown()

    def vrfInterfacesTest(self):
        '''
        Verify VRF basic forwarding between different types of RIFs
        within one of created VRF (VRF1)
        '''
        print("\nvrfInterfacesTest()")

        test_port = self.dev_port24
        test_lag_ports = [self.dev_port4, self.dev_port5, self.dev_port6]
        test_subport = self.dev_port10
        test_vlan_ports = [[self.dev_port0], [self.dev_port1], test_lag_ports]

        src_mac = "00:aa:aa:aa:aa:aa"
        src_ip = "20.20.20.20"
        vlan_id = 10
        subport_vlan_id = 100

        pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                eth_src=src_mac,
                                ip_src=src_ip,
                                ip_id=105,
                                ip_ttl=64,
                                pktlen=100)
        exp_pkt = simple_udp_packet(eth_src=ROUTER_MAC,
                                    ip_src=src_ip,
                                    ip_id=105,
                                    ip_ttl=63,
                                    pktlen=100)
        exp_tag_pkt = simple_udp_packet(eth_src=ROUTER_MAC,
                                        ip_src=src_ip,
                                        dl_vlan_enable=True,
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=104)

        try:
            # port -> LAG (10.10.2.0/24)
            pkt[IP].dst = "10.10.2.10"
            exp_pkt[IP].dst = pkt[IP].dst
            exp_pkt[Ether].dst = self.mac12

            print("Sending packet port->LAG in the same VRF")
            send_packet(self, test_port, pkt)
            verify_packet_any_port(self, exp_pkt, test_lag_ports)
            print("\tOK")

            # port -> subport (10.10.3.0/24)
            pkt[IP].dst = "10.10.3.10"
            exp_tag_pkt[IP].dst = pkt[IP].dst
            exp_tag_pkt[Ether].dst = self.mac13
            exp_tag_pkt[Ether][Dot1Q].vlan = subport_vlan_id

            print("Sending packet port->subport in the same VRF")
            send_packet(self, test_port, pkt)
            verify_packet(self, exp_tag_pkt, test_subport)
            print("\tOK")

            # port -> SVI (10.10.5.0/24)
            pkt[IP].dst = "10.10.5.10"
            exp_pkt[IP].dst = pkt[IP].dst
            exp_tag_pkt[IP].dst = pkt[IP].dst
            exp_pkt[Ether].dst = self.mac15
            exp_tag_pkt[Ether].dst = self.mac15
            exp_tag_pkt[Ether][Dot1Q].vlan = vlan_id

            exp_pkt_list = [exp_pkt, exp_tag_pkt, exp_pkt]

            print("Sending packet port->SVI in the same VRF")
            send_packet(self, test_port, pkt)
            verify_each_packet_on_multiple_port_lists(self, exp_pkt_list,
                                                      test_vlan_ports)
            print("\tOK")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)


@group("draft")
class VrfAclRedirectTest(SaiHelper):
    '''
    Verify inter-VRF ACL redirection
    '''

    def setUp(self):
        super(VrfAclRedirectTest, self).setUp()

        self.test_vrf = sai_thrift_create_virtual_router(self.client)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        # create and configure ingress RIF
        self.iport = self.port24
        self.iport_dev = self.dev_port24
        self.test_irif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.test_vrf,
            port_id=self.iport)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.iport_mac = "00:11:11:11:11:11"
        iport_ipv4 = "10.10.10.1"
        iport_ipv6 = "2001:0db8::1:1"

        self.iport_nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(iport_ipv4),
            router_interface_id=self.test_irif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.iport_nhop_v6 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(iport_ipv6),
            router_interface_id=self.test_irif,
            type=SAI_NEXT_HOP_TYPE_IP)

    def runTest(self):
        self.aclFwdL3NhopTest()
        self.aclFwdL3LagNhopTest()
        self.aclFwdSviNhopTest()
        self.aclFwdSubportNhopTest()
        self.aclFwdEcmpNhopTest()

    def tearDown(self):
        sai_thrift_remove_next_hop(self.client, self.iport_nhop)
        sai_thrift_remove_next_hop(self.client, self.iport_nhop_v6)
        sai_thrift_remove_router_interface(self.client, self.test_irif)
        sai_thrift_remove_virtual_router(self.client, self.test_vrf)

        super(VrfAclRedirectTest, self).tearDown()

    def aclFwdL3NhopTest(self):
        '''
        Verify ACL redirect to regular L3 nexthop in different VRF
        '''
        print("\naclFwdL3NhopTest()")

        test_dev_port = self.dev_port10
        test_rif = self.port10_rif

        test_mac = "00:33:33:33:33:33"
        test_ip = "30.30.30.1"
        ip_mask = "255.255.255.0"

        bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE]
        action_types = [SAI_ACL_ACTION_TYPE_REDIRECT]

        try:
            test_nbor = sai_thrift_neighbor_entry_t(
                rif_id=test_rif, ip_address=sai_ipaddress(test_ip))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor,
                                             dst_mac_address=test_mac)

            test_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip),
                router_interface_id=test_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            bind_points_list = sai_thrift_s32_list_t(count=len(bind_points),
                                                     int32list=bind_points)

            action_types_list = sai_thrift_s32_list_t(count=len(action_types),
                                                      int32list=action_types)

            acl_table = sai_thrift_create_acl_table(
                self.client,
                acl_stage=SAI_ACL_STAGE_INGRESS,
                acl_bind_point_type_list=bind_points_list,
                acl_action_type_list=action_types_list,
                field_dst_ip=True)

            acl_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(oid=test_nhop))

            ip_addr = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip4=test_ip),
                mask=sai_thrift_acl_field_data_mask_t(ip4=ip_mask))

            acl_entry = sai_thrift_create_acl_entry(self.client,
                                                    table_id=acl_table,
                                                    action_redirect=acl_action,
                                                    field_dst_ip=ip_addr)

            sai_thrift_set_router_interface_attribute(self.client,
                                                      self.test_irif,
                                                      ingress_acl=acl_table)

            src_mac = "00:aa:aa:aa:aa:aa"
            src_ip = "10.10.10.10"
            dst_ip = "30.30.30.30"

            pkt = simple_udp_packet(eth_dst=test_mac,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip,
                                    ip_src=src_ip,
                                    ip_ttl=64)

            print("Sending packet with ACL action: "
                  "redirect to regular L3 nexthop")
            send_packet(self, self.iport_dev, pkt)
            verify_packet(self, pkt, test_dev_port)
            print("\tOK")

        finally:
            sai_thrift_set_router_interface_attribute(self.client,
                                                      self.test_irif,
                                                      ingress_acl=0)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)
            sai_thrift_remove_next_hop(self.client, test_nhop)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor)

    def aclFwdL3LagNhopTest(self):
        '''
        Verify ACL redirect to regular L3 nexthop in different VRF
        '''
        print("\naclFwdL3LagNhopTest()")

        test_lag_dev_ports = [
            self.dev_port14, self.dev_port15, self.dev_port16
        ]
        test_lag_rif = self.lag3_rif

        test_mac = "00:33:33:33:33:33"
        test_ip = "30.30.30.1"
        ip_mask = "255.255.255.0"

        bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE]
        action_types = [SAI_ACL_ACTION_TYPE_REDIRECT]

        try:
            test_nbor = sai_thrift_neighbor_entry_t(
                rif_id=test_lag_rif, ip_address=sai_ipaddress(test_ip))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor,
                                             dst_mac_address=test_mac)

            test_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip),
                router_interface_id=test_lag_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            bind_points_list = sai_thrift_s32_list_t(count=len(bind_points),
                                                     int32list=bind_points)

            action_types_list = sai_thrift_s32_list_t(count=len(action_types),
                                                      int32list=action_types)

            acl_table = sai_thrift_create_acl_table(
                self.client,
                acl_stage=SAI_ACL_STAGE_INGRESS,
                acl_bind_point_type_list=bind_points_list,
                acl_action_type_list=action_types_list,
                field_dst_ip=True)

            acl_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(oid=test_nhop))

            ip_addr = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip4=test_ip),
                mask=sai_thrift_acl_field_data_mask_t(ip4=ip_mask))

            acl_entry = sai_thrift_create_acl_entry(self.client,
                                                    table_id=acl_table,
                                                    action_redirect=acl_action,
                                                    field_dst_ip=ip_addr)

            sai_thrift_set_router_interface_attribute(self.client,
                                                      self.test_irif,
                                                      ingress_acl=acl_table)

            src_mac = "00:aa:aa:aa:aa:aa"
            src_ip = "10.10.10.10"
            dst_ip = "30.30.30.30"

            pkt = simple_udp_packet(eth_dst=test_mac,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip,
                                    ip_src=src_ip,
                                    ip_ttl=64)

            print("Sending packet with ACL action: redirect to L3 LAG nexthop")
            send_packet(self, self.iport_dev, pkt)
            verify_packet_any_port(self, pkt, test_lag_dev_ports)
            print("\tOK")

        finally:
            sai_thrift_set_router_interface_attribute(self.client,
                                                      self.test_irif,
                                                      ingress_acl=0)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)
            sai_thrift_remove_next_hop(self.client, test_nhop)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor)

    def aclFwdSviNhopTest(self):
        '''
        Verify ACL redirect to SVI nexthop in different VRF
        '''
        print("\naclFwdSviNhopTest()")

        test_vlan_rif = self.vlan30_rif
        test_vlan_lag_dev_ports = [self.dev_port21, self.dev_port22]
        test_vlan_dev_ports = [[self.dev_port20], [self.dev_port21],
                               test_vlan_lag_dev_ports]

        test_mac = "00:33:33:33:33:33"
        test_ip = "30.30.30.1"
        ip_mask = "255.255.255.0"

        bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE]
        action_types = [SAI_ACL_ACTION_TYPE_REDIRECT]

        try:
            test_nbor = sai_thrift_neighbor_entry_t(
                rif_id=test_vlan_rif, ip_address=sai_ipaddress(test_ip))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor,
                                             dst_mac_address=test_mac)

            test_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip),
                router_interface_id=test_vlan_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            bind_points_list = sai_thrift_s32_list_t(count=len(bind_points),
                                                     int32list=bind_points)

            action_types_list = sai_thrift_s32_list_t(count=len(action_types),
                                                      int32list=action_types)

            acl_table = sai_thrift_create_acl_table(
                self.client,
                acl_stage=SAI_ACL_STAGE_INGRESS,
                acl_bind_point_type_list=bind_points_list,
                acl_action_type_list=action_types_list,
                field_dst_ip=True)

            acl_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(oid=test_nhop))

            ip_addr = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip4=test_ip),
                mask=sai_thrift_acl_field_data_mask_t(ip4=ip_mask))

            acl_entry = sai_thrift_create_acl_entry(self.client,
                                                    table_id=acl_table,
                                                    action_redirect=acl_action,
                                                    field_dst_ip=ip_addr)

            sai_thrift_set_router_interface_attribute(self.client,
                                                      self.test_irif,
                                                      ingress_acl=acl_table)

            src_mac = "00:aa:aa:aa:aa:aa"
            src_ip = "10.10.10.10"
            dst_ip = "30.30.30.30"

            pkt = simple_udp_packet(eth_dst=test_mac,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip,
                                    ip_src=src_ip,
                                    ip_ttl=64)

            exp_pkt_list = [pkt, pkt, pkt]

            print("Sending packet with ACL action: redirect to SVI nexthop")
            send_packet(self, self.iport_dev, pkt)
            verify_each_packet_on_multiple_port_lists(self, exp_pkt_list,
                                                      test_vlan_dev_ports)
            print("\tOK")

        finally:
            sai_thrift_set_router_interface_attribute(self.client,
                                                      self.test_irif,
                                                      ingress_acl=0)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)
            sai_thrift_remove_next_hop(self.client, test_nhop)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor)

    def aclFwdSubportNhopTest(self):
        '''
        Verify ACL redirect to SVI nexthop in different VRF
        '''
        print("\naclFwdSubportNhopTest()")

        test_port = self.port10
        test_dev_port = self.dev_port10

        test_mac1 = "00:33:33:33:33:1"
        test_ip1 = "30.30.10.1"
        vlan_id1 = 100
        test_mac2 = "00:33:33:33:33:2"
        test_ip2 = "30.30.20.2"
        vlan_id2 = 200
        ip_mask = "255.255.255.0"

        bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE]
        action_types = [SAI_ACL_ACTION_TYPE_REDIRECT]

        try:
            subport1_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
                virtual_router_id=self.default_vrf,
                port_id=test_port,
                outer_vlan_id=vlan_id1,
                admin_v4_state=True)

            test_nbor1 = sai_thrift_neighbor_entry_t(
                rif_id=subport1_rif, ip_address=sai_ipaddress(test_ip1))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor1,
                                             dst_mac_address=test_mac1)

            test_nhop1 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip1),
                router_interface_id=subport1_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            subport2_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
                virtual_router_id=self.default_vrf,
                port_id=test_port,
                outer_vlan_id=vlan_id2,
                admin_v4_state=True)

            test_nbor2 = sai_thrift_neighbor_entry_t(
                rif_id=subport2_rif, ip_address=sai_ipaddress(test_ip2))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor2,
                                             dst_mac_address=test_mac2)

            test_nhop2 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip2),
                router_interface_id=subport2_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            bind_points_list = sai_thrift_s32_list_t(count=len(bind_points),
                                                     int32list=bind_points)

            action_types_list = sai_thrift_s32_list_t(count=len(action_types),
                                                      int32list=action_types)

            acl_table = sai_thrift_create_acl_table(
                self.client,
                acl_stage=SAI_ACL_STAGE_INGRESS,
                acl_bind_point_type_list=bind_points_list,
                acl_action_type_list=action_types_list,
                field_dst_ip=True)

            acl_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(oid=test_nhop1))

            ip_addr = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip4=test_ip1),
                mask=sai_thrift_acl_field_data_mask_t(ip4=ip_mask))

            acl_entry1 = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table,
                action_redirect=acl_action,
                field_dst_ip=ip_addr)

            acl_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(oid=test_nhop2))

            ip_addr = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip4=test_ip2),
                mask=sai_thrift_acl_field_data_mask_t(ip4=ip_mask))

            acl_entry2 = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table,
                action_redirect=acl_action,
                field_dst_ip=ip_addr)

            sai_thrift_set_router_interface_attribute(self.client,
                                                      self.test_irif,
                                                      ingress_acl=acl_table)

            src_mac = "00:aa:aa:aa:aa:aa"
            src_ip = "10.10.10.10"
            dst_ip1 = "30.30.10.10"
            dst_ip2 = "30.30.20.20"

            pkt1 = simple_udp_packet(eth_dst=ROUTER_MAC,
                                     eth_src=src_mac,
                                     ip_dst=dst_ip1,
                                     ip_src=src_ip,
                                     ip_ttl=64)
            pkt2 = simple_udp_packet(eth_dst=ROUTER_MAC,
                                     eth_src=src_mac,
                                     ip_dst=dst_ip2,
                                     ip_src=src_ip,
                                     ip_ttl=64)

            print("Sending packest with ACL action: "
                  "redirect to 2 subport nexthops")
            send_packet(self, self.iport_dev, pkt1)
            verify_packet(self, pkt1, test_dev_port)

            send_packet(self, self.iport_dev, pkt2)
            verify_packet(self, pkt2, test_dev_port)
            print("\tOK")

        finally:
            sai_thrift_set_router_interface_attribute(self.client,
                                                      self.test_irif,
                                                      ingress_acl=0)
            sai_thrift_remove_acl_entry(self.client, acl_entry1)
            sai_thrift_remove_acl_entry(self.client, acl_entry2)
            sai_thrift_remove_acl_table(self.client, acl_table)
            sai_thrift_remove_next_hop(self.client, test_nhop1)
            sai_thrift_remove_next_hop(self.client, test_nhop2)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor1)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor2)
            sai_thrift_remove_router_interface(self.client, subport1_rif)
            sai_thrift_remove_router_interface(self.client, subport2_rif)

    def aclFwdEcmpNhopTest(self):
        '''
        Verify ACL redirect to ECMP nexthop in different VRF
        '''
        print("\naclFwdEcmpNhopTest()")

        test_dev_ports = [self.dev_port10, self.dev_port11, self.dev_port12]
        test_rifs = [self.port10_rif, self.port11_rif, self.port12_rif]

        test_mac1 = "00:33:33:33:33:11"
        test_mac2 = "00:33:33:33:33:22"
        test_mac3 = "00:33:33:33:33:33"
        test_ip1 = "30.30.30.1"
        test_ip2 = "30.30.30.2"
        test_ip3 = "30.30.30.3"
        ip_mask = "255.255.255.0"

        bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE]
        action_types = [SAI_ACL_ACTION_TYPE_REDIRECT]

        try:
            test_nbor1 = sai_thrift_neighbor_entry_t(
                rif_id=test_rifs[0], ip_address=sai_ipaddress(test_ip1))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor1,
                                             dst_mac_address=test_mac1)

            test_nbor2 = sai_thrift_neighbor_entry_t(
                rif_id=test_rifs[1], ip_address=sai_ipaddress(test_ip2))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor2,
                                             dst_mac_address=test_mac2)

            test_nbor3 = sai_thrift_neighbor_entry_t(
                rif_id=test_rifs[2], ip_address=sai_ipaddress(test_ip3))
            sai_thrift_create_neighbor_entry(self.client,
                                             test_nbor3,
                                             dst_mac_address=test_mac3)

            test_nhop1 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip1),
                router_interface_id=test_rifs[0],
                type=SAI_NEXT_HOP_TYPE_IP)

            test_nhop2 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip2),
                router_interface_id=test_rifs[1],
                type=SAI_NEXT_HOP_TYPE_IP)

            test_nhop3 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(test_ip3),
                router_interface_id=test_rifs[2],
                type=SAI_NEXT_HOP_TYPE_IP)

            test_nhop_group = sai_thrift_create_next_hop_group(
                self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
            nhop_group_mmbr1 = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=test_nhop_group,
                next_hop_id=test_nhop1)
            nhop_group_mmbr2 = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=test_nhop_group,
                next_hop_id=test_nhop2)
            nhop_group_mmbr3 = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=test_nhop_group,
                next_hop_id=test_nhop3)

            bind_points_list = sai_thrift_s32_list_t(count=len(bind_points),
                                                     int32list=bind_points)

            action_types_list = sai_thrift_s32_list_t(count=len(action_types),
                                                      int32list=action_types)

            acl_table = sai_thrift_create_acl_table(
                self.client,
                acl_stage=SAI_ACL_STAGE_INGRESS,
                acl_bind_point_type_list=bind_points_list,
                acl_action_type_list=action_types_list,
                field_dst_ip=True)

            acl_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    oid=test_nhop_group))

            ip_addr = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip4=test_ip1),
                mask=sai_thrift_acl_field_data_mask_t(ip4=ip_mask))

            acl_entry = sai_thrift_create_acl_entry(self.client,
                                                    table_id=acl_table,
                                                    action_redirect=acl_action,
                                                    field_dst_ip=ip_addr)

            sai_thrift_set_router_interface_attribute(self.client,
                                                      self.test_irif,
                                                      ingress_acl=acl_table)

            src_mac = "00:aa:aa:aa:aa:aa"
            src_ip_addr = "10.10.10.1"
            dst_ip_addr = "30.30.30.30"

            print("Sending packet with ACL action: redirect to ECMP nexthop")
            max_iter = 100
            count = [0, 0, 0]
            src_ip_addr = int(
                binascii.hexlify(
                    socket.inet_aton(src_ip_addr)), 16)
            dst_ip_addr = int(
                binascii.hexlify(
                    socket.inet_aton(dst_ip_addr)), 16)
            for _ in range(max_iter):
                src_ip = socket.inet_ntoa(
                    binascii.unhexlify(format(src_ip_addr, 'x').zfill(8)))
                dst_ip = socket.inet_ntoa(
                    binascii.unhexlify(format(dst_ip_addr, 'x').zfill(8)))

                pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                        eth_src=src_mac,
                                        ip_dst=dst_ip,
                                        ip_src=src_ip,
                                        ip_ttl=64)

                send_packet(self, self.iport_dev, pkt)
                rcv_idx = verify_packet_any_port(self, pkt, test_dev_ports)

                count[rcv_idx[0]] += 1
                src_ip_addr += 1
                dst_ip_addr += 1

            print("\tOK")

            for value in count:
                self.assertTrue(value >= ((max_iter / len(count)) * 0.7),
                                "Ecmp paths are not equally balanced")

        finally:
            sai_thrift_set_router_interface_attribute(self.client,
                                                      self.test_irif,
                                                      ingress_acl=0)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)
            sai_thrift_remove_next_hop_group_member(self.client,
                                                    nhop_group_mmbr1)
            sai_thrift_remove_next_hop_group_member(self.client,
                                                    nhop_group_mmbr2)
            sai_thrift_remove_next_hop_group_member(self.client,
                                                    nhop_group_mmbr3)
            sai_thrift_remove_next_hop_group(self.client, test_nhop_group)
            sai_thrift_remove_next_hop(self.client, test_nhop1)
            sai_thrift_remove_next_hop(self.client, test_nhop2)
            sai_thrift_remove_next_hop(self.client, test_nhop3)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor1)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor2)
            sai_thrift_remove_neighbor_entry(self.client, test_nbor3)


@group("draft")
class VrfScaleTest(SaiHelper):
    '''
    Verify if it is possible to create the number of VRFs declared as
    maximum allowed
    '''

    def runTest(self):
        '''
        Verify if VRF Scale matches maximum number of VRF entries
        available on switch and if it is possible to add routes to them
        '''
        print("\nVrfScaleTest")

        try:
            sw_attr = sai_thrift_get_switch_attribute(
                self.client, max_virtual_routers=True)
            max_vr_no = sw_attr['max_virtual_routers']

            ip_addr = "10.10.10.1"
            ipv6_addr = "2001:0db8::1"

            nexthop_v4 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(ip_addr),
                router_interface_id=self.port10_rif,
                type=SAI_NEXT_HOP_TYPE_IP)
            nexthop_v6 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(ipv6_addr),
                router_interface_id=self.port10_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            vrf_list = []
            lpm_route_v4_list = []
            lpm_route_v6_list = []
            host_route_v4_list = []
            host_route_v6_list = []

            print("Creating %d virtual routers with routes added" % max_vr_no)
            for _ in range(max_vr_no - 2):
                vrf = sai_thrift_create_virtual_router(self.client)
                self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
                vrf_list.append(vrf)

                lpm_route_v4 = sai_thrift_route_entry_t(
                    vr_id=vrf, destination=sai_ipprefix(ip_addr + '/24'))
                sai_thrift_create_route_entry(self.client,
                                              lpm_route_v4,
                                              next_hop_id=nexthop_v4)
                self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
                lpm_route_v4_list.append(lpm_route_v4)

                host_route_v4 = sai_thrift_route_entry_t(
                    vr_id=vrf, destination=sai_ipprefix(ip_addr + '/32'))
                sai_thrift_create_route_entry(self.client,
                                              host_route_v4,
                                              next_hop_id=nexthop_v4)
                self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
                host_route_v4_list.append(host_route_v4)

                lpm_route_v6 = sai_thrift_route_entry_t(
                    vr_id=vrf, destination=sai_ipprefix(ipv6_addr + '/48'))
                sai_thrift_create_route_entry(self.client,
                                              lpm_route_v6,
                                              next_hop_id=nexthop_v6)
                self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
                lpm_route_v6_list.append(lpm_route_v6)

                host_route_v6 = sai_thrift_route_entry_t(
                    vr_id=vrf, destination=sai_ipprefix(ipv6_addr + '/128'))
                sai_thrift_create_route_entry(self.client,
                                              host_route_v6,
                                              next_hop_id=nexthop_v6)
                self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
                host_route_v6_list.append(host_route_v6)

            print("\tOK")

            print("Checking if no more virtual routers can be created")
            vrf = sai_thrift_create_virtual_router(self.client)
            self.assertEgual(vrf, 0)

        finally:
            for i, _ in enumerate(vrf_list):
                sai_thrift_remove_route_entry(
                    self.client, host_route_v6_list[i])
                sai_thrift_remove_route_entry(
                    self.client, lpm_route_v6_list[i])
                sai_thrift_remove_route_entry(
                    self.client, host_route_v4_list[i])
                sai_thrift_remove_route_entry(
                    self.client, lpm_route_v4_list[i])
                sai_thrift_remove_virtual_router(self.client, vrf_list[i])

            sai_thrift_remove_next_hop(self.client, nexthop_v4)
            sai_thrift_remove_next_hop(self.client, nexthop_v6)


@group("draft")
class VrfSMACTest(SaiHelper):
    '''
    VRF SMAC test
    '''

    def setUp(self):
        super(VrfSMACTest, self).setUp()

        self.vrf_mac_create = '00:44:55:66:77:00'
        self.vrf_mac_set = '00:77:55:33:22:11'

        # to be set using tested VRF
        self.iport_dev = None
        self.iport_route = None
        self.iport = None
        self.test_erif = None
        self.eport_route = None
        self.eport_nbor_mac = None
        self.iport_nbor = None
        self.eport_dev = None
        self.iport_nhop = None
        self.iport_nbor_mac = None
        self.eport_nbor = None
        self.eport_nhop = None
        self.test_vrf = None
        self.test_vrf = None
        self.test_irif = None
        self.eport = None

    def runTest(self):
        self.testSMACCreateSet()
        self.testSMACSet()

    def _configureRifs(self, vrf):
        # create and configure ingress RIF
        self.iport = self.port24
        self.iport_dev = self.dev_port24
        self.test_irif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=vrf,
            port_id=self.iport)

        self.iport_nbor_mac = "00:11:11:11:11:11"
        iport_ipv4 = "10.10.10.1"
        self.iport_nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(iport_ipv4),
            router_interface_id=self.test_irif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.iport_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.test_irif, ip_address=sai_ipaddress(iport_ipv4))

        sai_thrift_create_neighbor_entry(self.client,
                                         self.iport_nbor,
                                         dst_mac_address=self.iport_nbor_mac)

        self.iport_route = sai_thrift_route_entry_t(
            vr_id=self.test_vrf, destination=sai_ipprefix(iport_ipv4 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.iport_route,
                                      next_hop_id=self.iport_nhop)

        # create and configure egress RIF
        self.eport = self.port25
        self.eport_dev = self.dev_port25
        self.eport_nbor_mac = "00:22:22:22:22:22"
        eport_ipv4 = "20.20.20.2"

        self.test_erif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.test_vrf,
            port_id=self.eport)

        self.eport_nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(eport_ipv4),
            router_interface_id=self.test_erif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.eport_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.test_erif, ip_address=sai_ipaddress(eport_ipv4))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.eport_nbor,
                                         dst_mac_address=self.eport_nbor_mac)

        self.eport_route = sai_thrift_route_entry_t(
            vr_id=self.test_vrf, destination=sai_ipprefix(eport_ipv4 + '/24'))
        sai_thrift_create_route_entry(self.client,
                                      self.eport_route,
                                      next_hop_id=self.eport_nhop)

    def _runTraffic(self, vrf_mac):
        src_mac = self.iport_nbor_mac
        src_ip = "10.10.10.10"
        dst_ip = "20.20.20.20"
        pkt = simple_udp_packet(eth_dst=vrf_mac,
                                eth_src=src_mac,
                                ip_dst=dst_ip,
                                ip_src=src_ip,
                                ip_ttl=64)
        exp_pkt = simple_udp_packet(eth_dst=self.eport_nbor_mac,
                                    eth_src=vrf_mac,
                                    ip_dst=dst_ip,
                                    ip_src=src_ip,
                                    ip_ttl=63)
        try:
            send_packet(self, self.iport_dev, pkt)
            verify_packet(self, exp_pkt, self.eport_dev)
            print("\tIPv4 forwarded")
        except Exception as e:
            self._removeRifs()
            raise e
        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

    def _removeRifs(self):
        sai_thrift_remove_route_entry(self.client, self.eport_route)
        sai_thrift_remove_neighbor_entry(self.client, self.eport_nbor)
        sai_thrift_remove_next_hop(self.client, self.eport_nhop)
        sai_thrift_remove_router_interface(self.client, self.test_erif)
        sai_thrift_remove_route_entry(self.client, self.iport_route)
        sai_thrift_remove_neighbor_entry(self.client, self.iport_nbor)
        sai_thrift_remove_next_hop(self.client, self.iport_nhop)
        sai_thrift_remove_router_interface(self.client, self.test_irif)

    def testSMACCreateSet(self):
        '''
        Verify SMAC configuration on VRF create
        '''
        print("\ntestSMACCreateSet()")

        self.test_vrf = sai_thrift_create_virtual_router(
            self.client, src_mac_address=self.vrf_mac_create)
        attrs = sai_thrift_get_virtual_router_attribute(
            self.client, self.test_vrf, src_mac_address=True)
        self.assertEqual(attrs["src_mac_address"], self.vrf_mac_create)

        self._configureRifs(self.test_vrf)
        print("Create VRF test")
        self._runTraffic(self.vrf_mac_create)

        sai_thrift_set_virtual_router_attribute(
            self.client, self.test_vrf, src_mac_address=self.vrf_mac_set)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        attrs = sai_thrift_get_virtual_router_attribute(
            self.client, self.test_vrf, src_mac_address=True)
        self.assertEqual(attrs["src_mac_address"], self.vrf_mac_set)

        print("Set VRF src_mac test. src_mac of existing RIF's "
              "should not change")
        self._runTraffic(self.vrf_mac_create)

        self._removeRifs()
        print("Set VRF src_mac test. New RIF.")
        self._configureRifs(self.test_vrf)
        self._runTraffic(self.vrf_mac_set)
        self._removeRifs()

        sai_thrift_remove_virtual_router(self.client, self.test_vrf)

    def testSMACSet(self):
        '''
        Verify SMAC configuration on VRF creted with default mac
        '''
        print("\ntestSMACSet()")

        self.test_vrf = sai_thrift_create_virtual_router(self.client)

        attrs = sai_thrift_get_virtual_router_attribute(
            self.client, self.test_vrf, src_mac_address=True)
        self.assertEqual(attrs["src_mac_address"], ROUTER_MAC)

        self._configureRifs(self.test_vrf)
        print("Creaete VRF test")
        self._runTraffic(ROUTER_MAC)

        sai_thrift_set_virtual_router_attribute(
            self.client, self.test_vrf, src_mac_address=self.vrf_mac_set)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        attrs = sai_thrift_get_virtual_router_attribute(
            self.client, self.test_vrf, src_mac_address=True)
        self.assertEqual(attrs["src_mac_address"], self.vrf_mac_set)

        print("Set VRF src_mac test. src_mac of existing RIF's "
              "should not change")
        self._runTraffic(ROUTER_MAC)

        self._removeRifs()
        print("Set VRF src_mac test. New RIF.")
        self._configureRifs(self.test_vrf)
        self._runTraffic(self.vrf_mac_set)
        self._removeRifs()

        sai_thrift_remove_virtual_router(self.client, self.test_vrf)
