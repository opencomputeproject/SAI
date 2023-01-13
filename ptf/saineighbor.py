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


class NeighborAttrIpv6TestHelper(PlatformSaiHelper):
    """
    Neighbor entry attributes IPv6 tests class
    Configuration
    +----------+-----------+
    | port0    | port0_rif |
    +----------+-----------+
    | port1    | port1_rif |
    +----------+-----------+
    """

    def setUp(self):
        super(NeighborAttrIpv6TestHelper, self).setUp()

        self.create_routing_interfaces(ports=[0, 1])

        self.test_rif = self.port0_rif
        self.ipv6_addr = "2001:0db8::1:10"
        self.ll_ipv6_addr = "fe80::10"
        self.mac_addr = "00:10:10:10:10:10"

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

    def tearDown(self):
        self.destroy_routing_interfaces()

        super(NeighborAttrIpv6TestHelper, self).tearDown()


class noHostRouteIpv6NeighborTest(NeighborAttrIpv6TestHelper):
    '''
    Verifies if IPv6 host route is not created according to
    SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute value
    '''
    def setUp(self):
        super(noHostRouteIpv6NeighborTest, self).setUp()

    def runTest(self):
        print("\nnoHostRouteIpv6NeighborTest()")

        nbr_entry_v6 = sai_thrift_neighbor_entry_t(
            rif_id=self.test_rif,
            ip_address=sai_ipaddress(self.ipv6_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            nbr_entry_v6,
            dst_mac_address=self.mac_addr,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        try:
            print("Sending IPv6 packet when host route not exists")
            send_packet(self, self.dev_port1, self.pkt_v6)
            verify_no_other_packets(self)
            print("Packet dropped")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v6)

    def tearDown(self):
        super(noHostRouteIpv6NeighborTest, self).tearDown()


class noHostRouteIpv6LinkLocalNeighborTest(NeighborAttrIpv6TestHelper):
    '''
    Verifies if host route is not created for link local IPv6 address
    irrespective of SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute
    value
    '''
    def setUp(self):
        super(noHostRouteIpv6LinkLocalNeighborTest, self).setUp()

    def runTest(self):
        print("\nnoHostRouteIpv6LinkLocalNeighborTest()")

        ll_nbr_entry_1 = sai_thrift_neighbor_entry_t(
            rif_id=self.test_rif,
            ip_address=sai_ipaddress(self.ll_ipv6_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            ll_nbr_entry_1,
            dst_mac_address=self.mac_addr,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        try:
            print("Sending IPv6 packet - no_host_route was set to True")
            send_packet(self, self.dev_port1, self.ll_pkt_v6)
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
            send_packet(self, self.dev_port1, self.ll_pkt_v6)
            verify_no_other_packets(self)
            print("Packet dropped")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, ll_nbr_entry_2)

    def tearDown(self):
        super(noHostRouteIpv6LinkLocalNeighborTest, self).tearDown()


class addHostRouteIpv6NeighborTest(NeighborAttrIpv6TestHelper):
    '''
    Verifies if IPv6 host route is created according to
    SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute value
    '''
    def setUp(self):
        super(addHostRouteIpv6NeighborTest, self).setUp()

    def runTest(self):
        print("\naddHostRouteIpv6NeighborTest()")

        nbr_entry_v6 = sai_thrift_neighbor_entry_t(
            rif_id=self.test_rif,
            ip_address=sai_ipaddress(self.ipv6_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            nbr_entry_v6,
            dst_mac_address=self.mac_addr,
            no_host_route=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        try:
            print("Sending IPv6 packet when host route exists")
            send_packet(self, self.dev_port1, self.pkt_v6)
            verify_packet(self, self.exp_pkt_v6, self.dev_port0)
            print("Packet forwarded")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v6)

    def tearDown(self):
        super(addHostRouteIpv6NeighborTest, self).tearDown()


class NeighborAttrIpv4TestHelper(PlatformSaiHelper):
    """
    Neighbor entry attributes IPv4 tests class
    Configuration
    +----------+-----------+
    | port0    | port0_rif |
    +----------+-----------+
    | port1    | port1_rif |
    +----------+-----------+
    """
    def setUp(self):
        super(NeighborAttrIpv4TestHelper, self).setUp()

        self.create_routing_interfaces(ports=[0, 1])

        self.test_rif = self.port0_rif
        self.ipv4_addr = "10.10.10.1"
        self.mac_addr = "00:10:10:10:10:10"
        self.mac_update_addr = "00:22:22:33:44:66"

        self.pkt_v4 = simple_udp_packet(eth_dst=ROUTER_MAC,
                                        ip_dst=self.ipv4_addr,
                                        ip_ttl=64)
        self.exp_pkt_v4 = simple_udp_packet(eth_dst=self.mac_addr,
                                            eth_src=ROUTER_MAC,
                                            ip_dst=self.ipv4_addr,
                                            ip_ttl=63)
        self.exp_updt_mac_pkt = simple_udp_packet(eth_dst=self.mac_update_addr,
                                                  eth_src=ROUTER_MAC,
                                                  ip_dst=self.ipv4_addr,
                                                  ip_ttl=63)

    def tearDown(self):
        self.destroy_routing_interfaces()

        super(NeighborAttrIpv4TestHelper, self).tearDown()


class noHostRouteIpv4NeighborTest(NeighborAttrIpv4TestHelper):
    '''
    Verifies if IPv4 host route is not created according to
    SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute value
    '''

    def setUp(self):
        super(noHostRouteIpv4NeighborTest, self).setUp()

    def runTest(self):
        print("\nnoHostRouteIpv4NeighborTest()")

        nbr_entry_v4 = sai_thrift_neighbor_entry_t(
            rif_id=self.test_rif,
            ip_address=sai_ipaddress(self.ipv4_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            nbr_entry_v4,
            dst_mac_address=self.mac_addr,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        try:
            print("Sending IPv4 packet when host route not exists")
            send_packet(self, self.dev_port1, self.pkt_v4)
            verify_no_other_packets(self)
            print("Packet dropped")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v4)

    def tearDown(self):
        super(noHostRouteIpv4NeighborTest, self).tearDown()


class addHostRouteIpv4NeighborTest(NeighborAttrIpv4TestHelper):
    '''
    Verifies if IPv4 host route is created according to
    SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute value
    '''

    def setUp(self):
        super(addHostRouteIpv4NeighborTest, self).setUp()

    def runTest(self):
        print("\naddHostRouteIpv4NeighborTest()")

        nbr_entry_v4 = sai_thrift_neighbor_entry_t(
            rif_id=self.test_rif,
            ip_address=sai_ipaddress(self.ipv4_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            nbr_entry_v4,
            dst_mac_address=self.mac_addr,
            no_host_route=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        try:
            print("Sending IPv4 packet when host route exists")
            send_packet(self, self.dev_port1, self.pkt_v4)
            verify_packet(self, self.exp_pkt_v4, self.dev_port0)
            print("Packet forwarded")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v4)

    def tearDown(self):
        super(addHostRouteIpv4NeighborTest, self).tearDown()


class updateNeighborEntryAttributeDstMacAddr(NeighborAttrIpv4TestHelper):
    '''
    Verifies if SAI_NEIGHBOR_ENTRY_ATTR_DST_MAC_ADDRESS is updated
    '''

    def setUp(self):
        super(updateNeighborEntryAttributeDstMacAddr, self).setUp()

    def runTest(self):
        print("\nupdateNeighborEntryAttributeDstMacAddr()")

        nbr_entry_v4 = sai_thrift_neighbor_entry_t(
            rif_id=self.test_rif,
            ip_address=sai_ipaddress(self.ipv4_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            nbr_entry_v4,
            dst_mac_address=self.mac_addr)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        try:
            print("Sending IPv4 packet before updating the destination mac")
            send_packet(self, self.dev_port1, self.pkt_v4)
            verify_packet(self, self.exp_pkt_v4, self.dev_port0)
            print("Packet forwarded")
            print("Update neighbor to 00:22:22:33:44:66")
            status = sai_thrift_set_neighbor_entry_attribute(
                self.client,
                nbr_entry_v4,
                dst_mac_address="00:22:22:33:44:66")

            self.assertEqual(status, SAI_STATUS_SUCCESS)

            attr = sai_thrift_get_neighbor_entry_attribute(
                self.client, nbr_entry_v4, dst_mac_address=True)
            self.assertEqual(attr['dst_mac_address'], '00:22:22:33:44:66')

            print("Sending IPv4 packet after updating the destination mac")
            send_packet(self, self.dev_port1, self.pkt_v4)
            verify_packet(self, self.exp_updt_mac_pkt, self.dev_port0)
            print("Packet forwarded")

        finally:
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v4)

    def tearDown(self):
        super(updateNeighborEntryAttributeDstMacAddr, self).tearDown()


class addNeighborEntryAttrIPv4addrFamily(NeighborAttrIpv4TestHelper):
    '''
    check NeighborEntryAttrIPaddrFamily is SAI_IP_ADDR_FAMILY_IPV4
    '''

    def setUp(self):
        super(addNeighborEntryAttrIPv4addrFamily, self).setUp()

    def runTest(self):
        print("\naddNeighborEntryAttrIPv4addrFamily()")

        try:
            before_ipv4_neighbor_entry = sai_thrift_get_switch_attribute(
                self.client, available_ipv4_neighbor_entry=True)
            nbr_entry_v4 = sai_thrift_neighbor_entry_t(
                rif_id=self.test_rif,
                ip_address=sai_ipaddress(self.ipv4_addr))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                nbr_entry_v4,
                dst_mac_address=self.mac_addr)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            nbr_entry_v4_1 = sai_thrift_neighbor_entry_t(
                rif_id=self.test_rif,
                ip_address=sai_ipaddress("10.10.10.2"))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                nbr_entry_v4_1,
                dst_mac_address="00:22:22:33:44:66")
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            nbr_entry_v4_2 = sai_thrift_neighbor_entry_t(
                rif_id=self.test_rif,
                ip_address=sai_ipaddress("10.10.10.3"))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                nbr_entry_v4_2,
                dst_mac_address="00:22:22:33:44:88")
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            after_ipv4_neighbor_entry = sai_thrift_get_switch_attribute(
                self.client, available_ipv4_neighbor_entry=True)
            available_ipv4_entries = before_ipv4_neighbor_entry[
                'available_ipv4_neighbor_entry'] - after_ipv4_neighbor_entry[
                    'available_ipv4_neighbor_entry']
            print("available_ipv4_entries: ", available_ipv4_entries)
            self.assertEqual(available_ipv4_entries, 3)

        finally:
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v4)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v4_1)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v4_2)
    def tearDown(self):
        super(addNeighborEntryAttrIPv4addrFamily, self).tearDown()


class addNeighborEntryAttrIPv6addrFamily(NeighborAttrIpv6TestHelper):
    '''
    check NeighborEntryAttrIPaddrFamily is SAI_IP_ADDR_FAMILY_IPV6
    '''

    def setUp(self):
        super(addNeighborEntryAttrIPv6addrFamily, self).setUp()

    def runTest(self):
        print("\naddNeighborEntryAttrIPv6addrFamily()")

        try:
            before_ipv6_neighbor_entry = sai_thrift_get_switch_attribute(
                self.client, available_ipv6_neighbor_entry=True)
            nbr_entry_v6 = sai_thrift_neighbor_entry_t(
                rif_id=self.test_rif,
                ip_address=sai_ipaddress(self.ipv6_addr))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                nbr_entry_v6,
                dst_mac_address=self.mac_addr)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            nbr_entry_v6_1 = sai_thrift_neighbor_entry_t(
                rif_id=self.test_rif,
                ip_address=sai_ipaddress("2001:0db8::1:11"))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                nbr_entry_v6_1,
                dst_mac_address="00:22:22:33:44:66")
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            after_ipv6_neighbor_entry = sai_thrift_get_switch_attribute(
                self.client, available_ipv6_neighbor_entry=True)

            available_ipv6_entries = before_ipv6_neighbor_entry[
                'available_ipv6_neighbor_entry'] - after_ipv6_neighbor_entry[
                    'available_ipv6_neighbor_entry']
            print("available_ipv6_entries: ", available_ipv6_entries)
            self.assertEqual(available_ipv6_entries, 2)

        finally:
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v6)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry_v6_1)

    def tearDown(self):
        super(addNeighborEntryAttrIPv6addrFamily, self).tearDown()
