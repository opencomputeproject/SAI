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
Thrift SAI interface Neighbor tests
"""
from ptf.packet import *
from ptf.testutils import *
from ptf.thriftutils import *

from sai_base_test import *


@group("draft")
class NeighborAttrTest(SaiHelper):
    '''
    Neighbor entry attributes tests class
    '''

    def setUp(self):
        super(NeighborAttrTest, self).setUp()

        self.test_rif = self.port10_rif
        self.ipv4_addr = "10.10.10.1"
        self.ipv6_addr = "2001:0db8::1:10"
        self.ll_ipv6_addr = "fe80::10"
        self.mac_addr = "00:10:10:10:10:10"

        self.pkt_v4 = simple_udp_packet(eth_dst=ROUTER_MAC,
                                        ip_dst=self.ipv4_addr,
                                        ip_ttl=64)
        self.exp_pkt_v4 = simple_udp_packet(eth_dst=self.mac_addr,
                                            eth_src=ROUTER_MAC,
                                            ip_dst=self.ipv4_addr,
                                            ip_ttl=63)

        self.pkt_v6 = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                          ipv6_dst=self.ipv6_addr,
                                          ipv6_hlim=64)
        self.exp_pkt_v6 = simple_udpv6_packet(eth_dst=self.mac_addr,
                                              eth_src=ROUTER_MAC,
                                              ipv6_dst=self.ipv6_addr,
                                              ipv6_hlim=63)

        self.ll_pkt_v6 = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                             ipv6_dst=self.ll_ipv6_addr,
                                             ipv6_hlim=64)

    def runTest(self):
        self.noHostRouteIpv4NeighborTest()
        self.noHostRouteIpv6NeighborTest()
        self.noHostRouteIpv6LinkLocalNeighborTest()
        self.addHostRouteIpv4NeighborTest()
        self.addHostRouteIpv6NeighborTest()

    def noHostRouteIpv4NeighborTest(self):
        '''
        Verifies if IPv4 host route is not created according to
        SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute value
        '''
        print("\nnoHostRouteIpv4NeighborTest()")

        try:
            nbr_entry_v4 = sai_thrift_neighbor_entry_t(
                rif_id=self.test_rif,
                ip_address=sai_ipaddress(self.ipv4_addr))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                nbr_entry_v4,
                dst_mac_address=self.mac_addr,
                no_host_route=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            print("Sending IPv4 packet when host route not exists")
            send_packet(self, self.dev_port11, self.pkt_v4)
            verify_no_other_packets(self)
            print("Packet dropped")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v4)

    def noHostRouteIpv6NeighborTest(self):
        '''
        Verifies if IPv6 host route is not created according to
        SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute value
        '''
        print("\nnoHostRouteIpv6NeighborTest()")

        try:
            nbr_entry_v6 = sai_thrift_neighbor_entry_t(
                rif_id=self.test_rif,
                ip_address=sai_ipaddress(self.ipv6_addr))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                nbr_entry_v6,
                dst_mac_address=self.mac_addr,
                no_host_route=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            print("Sending IPv6 packet when host route not exists")
            send_packet(self, self.dev_port11, self.pkt_v6)
            verify_no_other_packets(self)
            print("Packet dropped")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v6)

    def noHostRouteIpv6LinkLocalNeighborTest(self):
        '''
        Verifies if host route is not created for link local IPv6 address
        irrespective of SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute
        value
        '''
        print("\nnoHostRouteIpv6LinkLocalNeighborTest()")

        try:
            ll_nbr_entry_1 = sai_thrift_neighbor_entry_t(
                rif_id=self.test_rif,
                ip_address=sai_ipaddress(self.ll_ipv6_addr))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                ll_nbr_entry_1,
                dst_mac_address=self.mac_addr,
                no_host_route=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            print("Sending IPv6 packet - no_host_route was set to True")
            send_packet(self, self.dev_port11, self.ll_pkt_v6)
            verify_no_other_packets(self)
            print("Packet dropped")

            status = sai_thrift_remove_neighbor_entry(
                self.client, ll_nbr_entry_1)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            ll_nbr_entry_2 = sai_thrift_neighbor_entry_t(
                rif_id=self.test_rif,
                ip_address=sai_ipaddress(self.ll_ipv6_addr))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                ll_nbr_entry_2,
                dst_mac_address=self.mac_addr,
                no_host_route=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            print("Sending IPv6 packet - no_host_route was set to False")
            send_packet(self, self.dev_port11, self.ll_pkt_v6)
            verify_no_other_packets(self)
            print("Packet dropped")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, ll_nbr_entry_2)

    def addHostRouteIpv4NeighborTest(self):
        '''
        Verifies if IPv4 host route is created according to
        SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute value
        '''
        print("\naddHostRouteIpv4NeighborTest()")

        try:
            nbr_entry_v4 = sai_thrift_neighbor_entry_t(
                rif_id=self.test_rif,
                ip_address=sai_ipaddress(self.ipv4_addr))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                nbr_entry_v4,
                dst_mac_address=self.mac_addr,
                no_host_route=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            print("Sending IPv4 packet when host route exists")
            send_packet(self, self.dev_port11, self.pkt_v4)
            verify_packet(self, self.exp_pkt_v4, self.dev_port10)
            print("Packet forwarded")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v4)

    def addHostRouteIpv6NeighborTest(self):
        '''
        Verifies if IPv6 host route is created according to
        SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute value
        '''
        print("\naddHostRouteIpv6NeighborTest()")

        try:
            nbr_entry_v6 = sai_thrift_neighbor_entry_t(
                rif_id=self.test_rif,
                ip_address=sai_ipaddress(self.ipv6_addr))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                nbr_entry_v6,
                dst_mac_address=self.mac_addr,
                no_host_route=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            print("Sending IPv6 packet when host route exists")
            send_packet(self, self.dev_port11, self.pkt_v6)
            verify_packet(self, self.exp_pkt_v6, self.dev_port10)
            print("Packet forwarded")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v6)
