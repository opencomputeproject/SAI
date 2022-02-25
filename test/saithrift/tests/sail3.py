# Copyright 2013-present Barefoot Networks, Inc.
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
Thrift SAI interface L3 tests
"""
import socket
import sys
from struct import pack, unpack

from switch import *

import sai_base_test
from ptf.mask import Mask

@group('l3')
class L3IPv4HostTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending packet port 1 -> port 2 (192.168.0.1 -> 10.10.10.1 [id = 101])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 1
        mac_valid = 0
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_addr1_subnet = '10.10.10.0'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(
                                eth_dst='00:11:22:33:44:55',
                                eth_src=router_mac,
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=63)
        try:
            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0])
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)

            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('l3')
class L3IPv4LpmTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending packet port 1 -> port 2 (192.168.0.1 -> 10.10.10.1 [id = 101])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.0'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'
        nhop_ip1 = '20.20.20.1'
        nhop_ip1_subnet = '20.20.20.0'
        ip_mask2 = '255.255.255.0'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, nhop_ip1_subnet, ip_mask2, rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(
                                eth_dst='00:11:22:33:44:55',
                                eth_src=router_mac,
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=63)
        try:
            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0])
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, nhop_ip1_subnet, ip_mask2, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)

            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('l3')
class L3IPv6HostTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending packet port 1 -> port 2 (2000::1 -> 3000::1)"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV6
        ip_addr1 = '1234:5678:9abc:def0:4422:1133:5577:99aa'
        ip_addr1_subnet = '1234:5678:9abc:def0:4422:1133:5577:0'
        ip_mask1 = 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:0'
        dmac1 = '00:11:22:33:44:55'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)

        # send the test packet(s)
        pkt = simple_tcpv6_packet( eth_dst=router_mac,
                                eth_src='00:22:22:22:22:22',
                                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                                ipv6_src='2000::1',
                                ipv6_hlim=64)
        exp_pkt = simple_tcpv6_packet(
                                eth_dst='00:11:22:33:44:55',
                                eth_src=router_mac,
                                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                                ipv6_src='2000::1',
                                ipv6_hlim=63)
        try:
            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0])
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)

            self.client.sai_thrift_remove_virtual_router(vr_id)

def ip6_to_integer(ip6):
    ip6 = socket.inet_pton(socket.AF_INET6, ip6)
    a, b = unpack(">QQ", ip6)
    return (a << 64) | b

def integer_to_ip6(ip6int):
    a = (ip6int >> 64) & ((1 << 64) - 1)
    b = ip6int & ((1 << 64) - 1)
    return socket.inet_ntop(socket.AF_INET6, pack(">QQ", a, b))

@group('l3')
class L3IPv6PrefixTest(sai_base_test.ThriftInterfaceDataPlane):
    #Test packet forwarding for all IPv6 prefix lenghs (from 127 to 1)
    def runTest(self):
        print
        switch_init(self.client)
        port1 = port_list[1]
        port2 = port_list[2]
        port3 = port_list[3]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)
        addr_family = SAI_IP_ADDR_FAMILY_IPV6

        #Create neighbor and neighbor subnet
        ip_addr1 = '2000:aaaa::1'
        dmac1 = '00:11:22:33:44:55'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, '2000:aaaa::', 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:fff0', rif_id1)

        dest = '1234:5678:9abc:def0:4422:1133:5577:99aa'
        dest_int = ip6_to_integer(dest)

        try:
            for i in range(128):
                mask_int = ( ( 1 << (128-i) ) - 1 ) << i
                net_int = dest_int & mask_int
                mask = integer_to_ip6(mask_int)
                net = integer_to_ip6(net_int)

                pkt = simple_tcpv6_packet(eth_dst=router_mac,
                                          eth_src='00:22:22:22:22:22',
                                          ipv6_dst=dest,
                                          ipv6_src='2000:bbbb::1',
                                          ipv6_hlim=64)
                exp_pkt = simple_tcpv6_packet(eth_dst='00:11:22:33:44:55',
                                              eth_src=router_mac,
                                              ipv6_dst=dest,
                                              ipv6_src='2000:bbbb::1',
                                              ipv6_hlim=63)

                print "Test packet with dstaddr " + dest + ' sent to ' + net + '/' + str(128-i)
                sai_thrift_create_route(self.client, vr_id, addr_family, net, mask, nhop1)
                send_packet(self, 2, str(pkt))
                verify_packets(self, exp_pkt, [1])
                sai_thrift_remove_route(self.client, vr_id, addr_family, net, mask, None)
                mask=""
                send_packet(self, 2, str(pkt))
                verify_no_packet(self, exp_pkt, 1)
        finally:
            if mask!="":
                sai_thrift_remove_route(self.client, vr_id, addr_family, net, mask, None)
            sai_thrift_remove_route(self.client, vr_id, addr_family, '2000:aaaa::', 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:fff0', None)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('l3')
class L3IPv6LpmTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "IPv6 Lpm Test"
        print "Sending packet port 1 -> port 2 (2000::1 -> 3000::1, routing with 3000::0/120 route"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV6
        ip_addr1 = '1234:5678:9abc:def0:0000:0000:0000:0000'
        ip_mask1 = 'ffff:ffff:ffff:ffff:0000:0000:0000:0000'
        ip_mask2 = 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:0000'
        dmac1 = '00:11:22:33:44:55'
        nhop_ip1 = '3000::1'
        nhop_ip1_subnet = '3000::0'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, nhop_ip1_subnet, ip_mask2, rif_id1)

        # send the test packet(s)
        pkt = simple_tcpv6_packet( eth_dst=router_mac,
                                eth_src='00:22:22:22:22:22',
                                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                                ipv6_src='2000::1',
                                ipv6_hlim=64)
        exp_pkt = simple_tcpv6_packet(
                                eth_dst='00:11:22:33:44:55',
                                eth_src=router_mac,
                                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                                ipv6_src='2000::1',
                                ipv6_hlim=63)
        try:
            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0])
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, nhop_ip1_subnet, ip_mask2, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)

            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('l3')
@group('ecmp')
class L3IPv4EcmpHostTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending packet port 1 -> port 2 (192.168.0.1 -> 10.10.10.1 [id = 101])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_addr1_subnet = '10.10.10.0'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'

        vr1 = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif1 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif2 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)
        rif3 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family, rif1, ip_addr1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif2, ip_addr1, dmac2)

        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif2)

        nhop_group1 = sai_thrift_create_next_hop_group(self.client)

        nhop_gmember1 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop1)
        nhop_gmember2 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop2)

        sai_thrift_create_route(self.client, vr1, addr_family, ip_addr1_subnet, ip_mask1, rif1)
        sai_thrift_create_route(self.client, vr1, addr_family, ip_addr1_subnet, ip_mask1, rif2)

        # send the test packet(s)
        try:
            pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=106,
                                ip_ttl=64)

            exp_pkt1 = simple_tcp_packet(
                                eth_dst='00:11:22:33:44:55',
                                eth_src=router_mac,
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=106,
                                #ip_tos=3,
                                ip_ttl=63)
            exp_pkt2 = simple_tcp_packet(
                                eth_dst='00:11:22:33:44:56',
                                eth_src=router_mac,
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=106,
                                #ip_tos=3,
                                ip_ttl=63)

            send_packet(self, 2, str(pkt))
            verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2], [0, 1])

            pkt = simple_tcp_packet(eth_dst=router_mac,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.100.3',
                                    ip_id=106,
                                    ip_ttl=64)

            exp_pkt1 = simple_tcp_packet(
                                    eth_dst='00:11:22:33:44:55',
                                    eth_src=router_mac,
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.100.3',
                                    ip_id=106,
                                    #ip_tos=3,
                                    ip_ttl=63)
            exp_pkt2 = simple_tcp_packet(
                                    eth_dst='00:11:22:33:44:56',
                                    eth_src=router_mac,
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.100.3',
                                    ip_id=106,
                                    #ip_tos=3,
                                    ip_ttl=63)

            send_packet(self, 2, str(pkt))
            verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2], [0, 1])
        finally:
            sai_thrift_remove_route(self.client, vr1, addr_family, ip_addr1_subnet, ip_mask1, rif1)
            sai_thrift_remove_route(self.client, vr1, addr_family, ip_addr1_subnet, ip_mask1, rif2)

            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember1)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember2)

            self.client.sai_thrift_remove_next_hop_group(nhop_group1)

            self.client.sai_thrift_remove_next_hop(nhop1)
            self.client.sai_thrift_remove_next_hop(nhop2)

            sai_thrift_remove_neighbor(self.client, addr_family, rif1, ip_addr1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif2, ip_addr1, dmac2)

            self.client.sai_thrift_remove_router_interface(rif1)
            self.client.sai_thrift_remove_router_interface(rif2)
            self.client.sai_thrift_remove_router_interface(rif3)

            self.client.sai_thrift_remove_virtual_router(vr1)

@group('l3')
@group('ecmp')
class L3IPv6EcmpHostTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending packet port 1 -> port 2 (192.168.0.1 -> 10.10.10.1 [id = 101])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        addr_family = SAI_IP_ADDR_FAMILY_IPV6
        ip_addr1 = '5000:1:1:0:0:0:0:1'
        ip_mask1 = 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'

        vr1 = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif1 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif2 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)
        rif3 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family, rif1, ip_addr1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif2, ip_addr1, dmac2)

        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif2)

        nhop_group1 = sai_thrift_create_next_hop_group(self.client)

        nhop_gmember1 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop1)
        nhop_gmember2 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop2)

        sai_thrift_create_route(self.client, vr1, addr_family, ip_addr1, ip_mask1, nhop_group1)

        # send the test packet(s)
        try:
            pkt = simple_tcpv6_packet(eth_dst=router_mac,
                                      eth_src='00:22:22:22:22:22',
                                      ipv6_dst='5000:1:1:0:0:0:0:1',
                                      ipv6_src='2000:1:1:0:0:0:0:1',
                                      tcp_sport=0x1234,
                                      ipv6_hlim=64)

            exp_pkt1 = simple_tcpv6_packet(eth_dst='00:11:22:33:44:55',
                                           eth_src=router_mac,
                                           ipv6_dst='5000:1:1:0:0:0:0:1',
                                           ipv6_src='2000:1:1:0:0:0:0:1',
                                           tcp_sport=0x1234,
                                           ipv6_hlim=63)
            exp_pkt2 = simple_tcpv6_packet(eth_dst='00:11:22:33:44:56',
                                           eth_src=router_mac,
                                           ipv6_dst='5000:1:1:0:0:0:0:1',
                                           ipv6_src='2000:1:1:0:0:0:0:1',
                                           tcp_sport=0x1234,
                                           ipv6_hlim=63)

            send_packet(self, 2, str(pkt))
            verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2], [0, 1])

            pkt = simple_tcpv6_packet(eth_dst=router_mac,
                                      eth_src='00:22:22:22:22:45',
                                      ipv6_dst='5000:1:1:0:0:0:0:1',
                                      ipv6_src='2000:1:1:0:0:0:0:1',
                                      tcp_sport=0x1248,
                                      ipv6_hlim=64)

            exp_pkt1 = simple_tcpv6_packet(eth_dst='00:11:22:33:44:55',
                                           eth_src=router_mac,
                                           ipv6_dst='5000:1:1:0:0:0:0:1',
                                           ipv6_src='2000:1:1:0:0:0:0:1',
                                           tcp_sport=0x1248,
                                           ipv6_hlim=63)
            exp_pkt2 = simple_tcpv6_packet(eth_dst='00:11:22:33:44:56',
                                           eth_src=router_mac,
                                           ipv6_dst='5000:1:1:0:0:0:0:1',
                                           ipv6_src='2000:1:1:0:0:0:0:1',
                                           tcp_sport=0x1248,
                                           ipv6_hlim=63)

            send_packet(self, 2, str(pkt))
            verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2], [0, 1])
        finally:
            sai_thrift_remove_route(self.client, vr1, addr_family, ip_addr1, ip_mask1, nhop_group1)

            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember1)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember2)

            self.client.sai_thrift_remove_next_hop_group(nhop_group1)

            self.client.sai_thrift_remove_next_hop(nhop1)
            self.client.sai_thrift_remove_next_hop(nhop2)

            sai_thrift_remove_neighbor(self.client, addr_family, rif1, ip_addr1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif2, ip_addr1, dmac2)

            self.client.sai_thrift_remove_router_interface(rif1)
            self.client.sai_thrift_remove_router_interface(rif2)
            self.client.sai_thrift_remove_router_interface(rif3)

            self.client.sai_thrift_remove_virtual_router(vr1)

@group('l3')
@group('ecmp')
class L3IPv4EcmpLpmTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending packet port 3 -> port [0,1,2] (192.168.0.1 -> 10.10.10.1 [id = 101])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.0.0'
        ip_mask1 = '255.255.0.0'
        ip_mask2 = '255.255.255.0'
        nhop_ip1 = '11.11.11.11'
        nhop_ip1_subnet = '11.11.11.0'
        nhop_ip2 = '22.22.22.22'
        nhop_ip2_subnet = '22.22.22.0'
        nhop_ip3 = '33.33.33.33'
        nhop_ip3_subnet = '33.33.33.0'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'
        dmac3 = '00:11:22:33:44:57'

        vr1 = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif1 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif2 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)
        rif3 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)
        rif4 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port4, 0, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family, rif1, nhop_ip1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif2, nhop_ip2, dmac2)
        sai_thrift_create_neighbor(self.client, addr_family, rif3, nhop_ip3, dmac3)

        nhop1 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip1, rif1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip2, rif2)
        nhop3 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip3, rif3)

        nhop_group1 = sai_thrift_create_next_hop_group(self.client)

        nhop_gmember1 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop1)
        nhop_gmember2 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop2)
        nhop_gmember3 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop3)

        sai_thrift_create_route(self.client, vr1, addr_family, ip_addr1, ip_mask1, nhop_group1)
        sai_thrift_create_route(self.client, vr1, addr_family, nhop_ip1_subnet, ip_mask2, rif1)
        sai_thrift_create_route(self.client, vr1, addr_family, nhop_ip2_subnet, ip_mask2, rif2)
        sai_thrift_create_route(self.client, vr1, addr_family, nhop_ip3_subnet, ip_mask2, rif3)

        # send the test packet(s)
        try:
            count = [0, 0, 0]
            dst_ip = int(socket.inet_aton('10.10.10.1').encode('hex'),16)
            max_itrs = 200
            src_mac_start = '00:22:22:22:22:'
            for i in range(0, max_itrs):
                dst_ip_addr = socket.inet_ntoa(hex(dst_ip)[2:].zfill(8).decode('hex'))
                src_mac = src_mac_start + str(i%99).zfill(2)
                pkt = simple_tcp_packet(eth_dst=router_mac,
                                        eth_src=src_mac,
                                        ip_dst=dst_ip_addr,
                                        ip_src='192.168.8.1',
                                        ip_id=106,
                                        ip_ttl=64)
                exp_pkt1 = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                             eth_src=router_mac,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt2 = simple_tcp_packet(eth_dst='00:11:22:33:44:56',
                                             eth_src=router_mac,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt3 = simple_tcp_packet(eth_dst='00:11:22:33:44:57',
                                             eth_src=router_mac,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)

                send_packet(self, 3, str(pkt))
                rcv_idx = verify_any_packet_any_port(self,
                                                     [exp_pkt1, exp_pkt2, exp_pkt3],
                                                     [0, 1, 2])
                count[rcv_idx] += 1
                dst_ip += 1

            for i in range(0, 3):
                self.assertTrue((count[i] >= ((max_itrs / 3) * 0.8)),
                                "Not all paths are equally balanced, %s" % count)
        finally:
            sai_thrift_remove_route(self.client, vr1, addr_family, ip_addr1, ip_mask1, nhop_group1)
            sai_thrift_remove_route(self.client, vr1, addr_family, nhop_ip1_subnet, ip_mask2, rif1)
            sai_thrift_remove_route(self.client, vr1, addr_family, nhop_ip2_subnet, ip_mask2, rif2)
            sai_thrift_remove_route(self.client, vr1, addr_family, nhop_ip3_subnet, ip_mask2, rif3)

            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember1)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember2)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember3)

            self.client.sai_thrift_remove_next_hop_group(nhop_group1)

            self.client.sai_thrift_remove_next_hop(nhop1)
            self.client.sai_thrift_remove_next_hop(nhop2)
            self.client.sai_thrift_remove_next_hop(nhop3)

            sai_thrift_remove_neighbor(self.client, addr_family, rif1, nhop_ip1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif2, nhop_ip2, dmac2)
            sai_thrift_remove_neighbor(self.client, addr_family, rif3, nhop_ip3, dmac3)

            self.client.sai_thrift_remove_router_interface(rif1)
            self.client.sai_thrift_remove_router_interface(rif2)
            self.client.sai_thrift_remove_router_interface(rif3)
            self.client.sai_thrift_remove_router_interface(rif4)

            self.client.sai_thrift_remove_virtual_router(vr1)

@group('l3')
@group('ecmp')
class L3IPv6EcmpLpmTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending packet port 1 -> port 2 (192.168.0.1 -> 10.10.10.1 [id = 101])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        addr_family = SAI_IP_ADDR_FAMILY_IPV6
        ip_addr1 = '6000:1:1:0:0:0:0:0'
        ip_mask1 = 'ffff:ffff:ffff:ffff:0:0:0:0'
        nhop_ip1 = '2000:1:1:0:0:0:0:1'
        nhop_ip2 = '3000:1:1:0:0:0:0:1'
        nhop_ip3 = '4000:1:1:0:0:0:0:1'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'
        dmac3 = '00:11:22:33:44:57'

        vr1 = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif1 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif2 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)
        rif3 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)
        rif4 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port4, 0, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family, rif1, nhop_ip1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif2, nhop_ip2, dmac2)
        sai_thrift_create_neighbor(self.client, addr_family, rif3, nhop_ip3, dmac3)

        nhop1 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip1, rif1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip2, rif2)
        nhop3 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip3, rif3)

        nhop_group1 = sai_thrift_create_next_hop_group(self.client)

        nhop_gmember1 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop1)
        nhop_gmember2 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop2)
        nhop_gmember3 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop3)

        sai_thrift_create_route(self.client, vr1, addr_family, ip_addr1, ip_mask1, nhop_group1)

        # send the test packet(s)
        try:
            count = [0, 0, 0]
            dst_ip = socket.inet_pton(socket.AF_INET6, '6000:1:1:0:0:0:0:1')
            dst_ip_arr = list(dst_ip)
            src_mac_start = '00:22:22:22:22:'
            max_itrs = 200
            sport = 0x1234
            dport = 0x50
            for i in range(0, max_itrs):
                dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
                src_mac = src_mac_start + str(i%99).zfill(2)
                #HACK: sport is a hack for hashing since the ecmp hash does not
                #include ipv6 sa and da.
                pkt = simple_tcpv6_packet(
                        eth_dst=router_mac,
                        eth_src=src_mac,
                        ipv6_dst=dst_ip_addr,
                        ipv6_src='1001:1:1:0:0:0:0:2',
                        tcp_sport=sport,
                        tcp_dport=dport,
                        ipv6_hlim=64)
                exp_pkt1 = simple_tcpv6_packet(
                        eth_dst='00:11:22:33:44:55',
                        eth_src=router_mac,
                        ipv6_dst=dst_ip_addr,
                        ipv6_src='1001:1:1:0:0:0:0:2',
                        tcp_sport=sport,
                        tcp_dport=dport,
                        ipv6_hlim=63)
                exp_pkt2 = simple_tcpv6_packet(
                        eth_dst='00:11:22:33:44:56',
                        eth_src=router_mac,
                        ipv6_dst=dst_ip_addr,
                        ipv6_src='1001:1:1:0:0:0:0:2',
                        tcp_sport=sport,
                        tcp_dport=dport,
                        ipv6_hlim=63)
                exp_pkt3 = simple_tcpv6_packet(
                        eth_dst='00:11:22:33:44:57',
                        eth_src=router_mac,
                        ipv6_dst=dst_ip_addr,
                        ipv6_src='1001:1:1:0:0:0:0:2',
                        tcp_sport=sport,
                        tcp_dport=dport,
                        ipv6_hlim=63)
                exp_pkt4 = simple_tcpv6_packet(
                        eth_dst='00:11:22:33:44:58',
                        eth_src=router_mac,
                        ipv6_dst=dst_ip_addr,
                        ipv6_src='1001:1:1:0:0:0:0:2',
                        tcp_sport=sport,
                        tcp_dport=dport,
                        ipv6_hlim=63)

                send_packet(self, 3, str(pkt))
                rcv_idx = verify_any_packet_any_port(self,
                                                     [exp_pkt1, exp_pkt2, exp_pkt3],
                                                     [0, 1, 2])
                count[rcv_idx] += 1
                dst_ip_arr[15] = chr(ord(dst_ip_arr[15]) + 1)
                dst_ip = ''.join(dst_ip_arr)
                sport += 15
                dport += 20

            print "Count = %s" % str(count)
            for i in range(0, 3):
                self.assertTrue((count[i] >= ((max_itrs / 3) * 0.75)),
                                "Not all paths are equally balanced")
        finally:
            sai_thrift_remove_route(self.client, vr1, addr_family, ip_addr1, ip_mask1, nhop_group1)

            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember1)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember2)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember3)

            self.client.sai_thrift_remove_next_hop_group(nhop_group1)

            self.client.sai_thrift_remove_next_hop(nhop1)
            self.client.sai_thrift_remove_next_hop(nhop2)
            self.client.sai_thrift_remove_next_hop(nhop3)

            sai_thrift_remove_neighbor(self.client, addr_family, rif1, nhop_ip1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif2, nhop_ip2, dmac2)
            sai_thrift_remove_neighbor(self.client, addr_family, rif3, nhop_ip3, dmac3)

            self.client.sai_thrift_remove_router_interface(rif1)
            self.client.sai_thrift_remove_router_interface(rif2)
            self.client.sai_thrift_remove_router_interface(rif3)
            self.client.sai_thrift_remove_router_interface(rif4)

            self.client.sai_thrift_remove_virtual_router(vr1)

            
@group('l3')
@group('ecmp')
class L3IPv4EcmpHashSeedTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        '''
	Create a VRF with IPv4 and IPv6 enabled. Create 4 router interfaces in the same VRF.
        Create a route (/24 mask) with nhop and neighbor entry in three router interfaces. 
        Send 100 streams with varying 5-tuple combinations to the destination IP on one port and verify distribution on the 
        three router interfaces for which the nhops are present. Change the ECMP hash seed value to 10 and verify distribution.
	   
	'''
	   
        print "Sending packet port4 -> port1,port2,port3 (192.168.6.1 to 10.10.10.1) "
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
	port4 = port_list[3]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''
        hash_seed = 10

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        
        ip_addr1 = '192.168.1.1'
        ip_addr2 = '192.168.2.1'
        ip_addr3 = '192.168.3.1'
        ip_addr4 = '10.10.10.0'
        
        ip_addr1_subnet = '192.168.1.0'
        ip_addr2_subnet = '192.168.2.0'
        ip_addr3_subnet = '192.168.3.0'
       
        ip_mask = '255.255.255.0'
        destmac1 = '00:11:22:33:44:51'
        destmac2 = '00:11:22:33:44:52'
	destmac3 = '00:11:22:33:44:53'

        virtual_router = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        router_interface1 = sai_thrift_create_router_interface(self.client, virtual_router, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        router_interface2 = sai_thrift_create_router_interface(self.client, virtual_router, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)
        router_interface3 = sai_thrift_create_router_interface(self.client, virtual_router, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)
	router_interface4 = sai_thrift_create_router_interface(self.client, virtual_router, SAI_ROUTER_INTERFACE_TYPE_PORT, port4, 0, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family,  router_interface1, ip_addr1, destmac1)
        sai_thrift_create_neighbor(self.client, addr_family,  router_interface2, ip_addr2, destmac2)
        sai_thrift_create_neighbor(self.client, addr_family,  router_interface3, ip_addr3, destmac3)

        next_hop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, router_interface1)
        next_hop2 = sai_thrift_create_nhop(self.client, addr_family, ip_addr2, router_interface2)
	next_hop3 = sai_thrift_create_nhop(self.client, addr_family, ip_addr3, router_interface3)


        nexthop_group = sai_thrift_create_next_hop_group(self.client)

        nexthop_gmember1 = sai_thrift_create_next_hop_group_member(self.client, nexthop_group, next_hop1)
        nexthop_gmember2 = sai_thrift_create_next_hop_group_member(self.client, nexthop_group, next_hop2)
	nexthop_gmember3 = sai_thrift_create_next_hop_group_member(self.client, nexthop_group, next_hop3)
	
        sai_thrift_create_route(self.client, virtual_router, addr_family, ip_addr4, ip_mask, nexthop_group)

        try:   
            count = [0, 0, 0]
            maximum_packets = 101
            src_mac_start = '00:22:22:22:22:'
            ip_src_start = '192.168.6.'
            ip_dst_start = '10.10.10.'
            destination_port = 0x80
            source_port = 0x1234
               
            for i in range(1, maximum_packets):
               
                src_mac = src_mac_start + str(i % 99).zfill(2)
                ip_source = ip_src_start + str(i % 99).zfill(3)
                ip_destination= ip_dst_start + str(i % 99).zfill(3)
                if (i%100) == 0:
                    print " %s packets are sent ...." % (i)

                packet = simple_tcp_packet(eth_dst=router_mac,
                                           eth_src=src_mac,
                                           ip_dst=ip_destination,
                                           ip_src=ip_source,
                                           tcp_sport=source_port,
                                           tcp_dport=destination_port,
                                           ip_id=106,
                                           ip_ttl=64) 
                #expected packet at port1 
                expected_packet1 = simple_tcp_packet(eth_dst='00:11:22:33:44:51',   
                                                     eth_src=router_mac,
                                                     ip_dst= ip_destination,
                                                     ip_src=ip_source,
                                                     tcp_sport=source_port,
                                                     tcp_dport=destination_port,
                                                     ip_id=106,
                                                     ip_ttl=63)  
                #expected packet at port2 
                expected_packet2 = simple_tcp_packet(eth_dst='00:11:22:33:44:52',
                                                     eth_src = router_mac,     
                                                     ip_dst= ip_destination,
                                                     ip_src=ip_source,
                                                     tcp_sport=source_port,
                                                     tcp_dport=destination_port,     
                                                     ip_id =106,       
                                                     ip_ttl=63) 
                #expected packet at port3  
                expected_packet3 = simple_tcp_packet(eth_dst='00:11:22:33:44:53',
                                                     eth_src = router_mac,     
                                                     ip_dst= ip_destination,
                                                     ip_src=ip_source,
                                                     tcp_sport=source_port,
                                                     tcp_dport=destination_port,   
                                                     ip_id =106,       
                                                     ip_ttl=63) 


                send_packet(self, 3, str(packet)) 
                rcv_idx = verify_any_packet_any_port(self, [expected_packet1, expected_packet2, expected_packet3], [0, 1, 2])
                count[rcv_idx] += 1
                source_port += 1
                destination_port += 1

            print "Packet distribution With default HashSeed value"

            for i in range(0,3):
                print "packets received at port interface : " + i  
                print (count[i])
                if (count[i] >= ( maximum_packets/ 3) * 0.8):
                    print "Not all paths are equally balanced, %s" % count
                print "ALL paths are balanced with three members"    


          

            #HASHING SECTION:

            print "Changing HASH SEED value from default to 10"

            attr_value = sai_thrift_attribute_value_t(u32=hash_seed)
            attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_SEED, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)
			
	    
            count = [0, 0, 0]
            maximum_packets = 101	   
	    src_mac_start = '00:22:22:22:22:'
            ip_src_start = '192.168.6.'
            ip_dst_start = '10.10.10.'
            destination_port = 0x80
            source_port = 0x1234
	    
	    for i in range(1, maximum_packets):
                packet = simple_tcp_packet(eth_dst=router_mac,
                                           eth_src='00:22:22:22:22:22',
                                           ip_dst= ip_destination,
                                           ip_src=ip_source,
                                           tcp_sport=source_port,
                                           tcp_dport=destination_port,
                                           ip_id=106,
                                           ip_ttl=64) 
                #packet at port1 
                expected_packet1 = simple_tcp_packet(eth_dst='00:11:22:33:44:51',   
                                                     eth_src=router_mac,
                                                     ip_dst= ip_destination,
                                                     ip_src=ip_source,
                                                     tcp_sport=source_port,
                                                     tcp_dport=destination_port,
                                                     ip_id=106,
                                                     ip_ttl=63)  
                #packet at port2 
                expected_packet2 = simple_tcp_packet(eth_dst='00:11:22:33:44:52',
                                                     eth_src = router_mac,     
                                                     ip_dst= ip_destination,
                                                     ip_src=ip_source,
                                                     tcp_sport=source_port,
                                                     tcp_dport=destination_port,   
                                                     ip_id =106,       
                                                     ip_ttl=63) 
                #packet at port3  
                expected_packet3 = simple_tcp_packet(eth_dst='00:11:22:33:44:53',
                                                     eth_src = router_mac,     
                                                     ip_dst= ip_destination,
                                                     ip_src=ip_source,
                                                     tcp_sport=source_port,
                                                     tcp_dport=destination_port,     
                                                     ip_id =106,       
                                                     ip_ttl=63) 


                send_packet(self, 3, str(packet)) 
                rcv_idx = verify_any_packet_any_port(self, [expected_packet1, expected_packet2, expected_packet3], [0, 1, 2])
                count[rcv_idx] += 1
                source_port += 1
                destination_port += 1
            
            print "Packet distribution After changing HashSeed value"

            for i in range(0,3):
                print ("packets received at port interface : " + i)  
                print (count[i])
                if (count[i] >= ( maximum_packets/ 3) * 0.8):
                    print "Not all paths are equally balanced, %s" % count
                print "ALL paths are balanced with three members"    
	
	finally:
            
            sai_thrift_remove_route(self.client, virtual_router, addr_family, ip_addr4, ip_mask, nexthop_group)

            attr_value = sai_thrift_attribute_value_t(u32=0)
            attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_SEED, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)


            self.client.sai_thrift_remove_next_hop_group_member(nexthop_gmember1)
            self.client.sai_thrift_remove_next_hop_group_member(nexthop_gmember2)
	    self.client.sai_thrift_remove_next_hop_group_member(nexthop_gmember3)

            self.client.sai_thrift_remove_next_hop_group(nexthop_group)

            self.client.sai_thrift_remove_next_hop(next_hop1)
            self.client.sai_thrift_remove_next_hop(next_hop2)
	    self.client.sai_thrift_remove_next_hop(next_hop3)

            sai_thrift_remove_neighbor(self.client, addr_family, router_interface1, ip_addr1, destmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, router_interface2, ip_addr2, destmac2)
	    sai_thrift_remove_neighbor(self.client, addr_family, router_interface3, ip_addr3, destmac3)

            self.client.sai_thrift_remove_router_interface(router_interface1)
            self.client.sai_thrift_remove_router_interface(router_interface2)
            self.client.sai_thrift_remove_router_interface(router_interface3)
	    self.client.sai_thrift_remove_router_interface(router_interface4)

            self.client.sai_thrift_remove_virtual_router(virtual_router)

          
            
@group('l3')
@group('lag')
class L3IPv4LagTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_addr1_subnet = '10.10.10.0'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        lag_id1 = self.client.sai_thrift_create_lag([])

        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, [port1, port2])

        lag_member_id1 = sai_thrift_create_lag_member(self.client, lag_id1, port1)
        lag_member_id2 = sai_thrift_create_lag_member(self.client, lag_id1, port2)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, lag_id1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)

        # send the test packet(s)
        try:
            pkt = simple_tcp_packet(eth_dst=router_mac,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=110,
                                    ip_ttl=64)

            exp_pkt = simple_tcp_packet(
                                    eth_dst='00:11:22:33:44:55',
                                    eth_src=router_mac,
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=110,
                                    ip_ttl=63)
            send_packet(self, 2, str(pkt))
            verify_packets_any(self, exp_pkt, [0, 1])
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)

            sai_thrift_remove_lag_member(self.client, lag_member_id1)
            sai_thrift_remove_lag_member(self.client, lag_member_id2)
            self.client.sai_thrift_remove_lag(lag_id1)
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('l3')
@group('lag')
class L3IPv6LagTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        lag_id1 = self.client.sai_thrift_create_lag([])
        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, [port1, port2])
        lag_member_id1 = sai_thrift_create_lag_member(self.client, lag_id1, port1)
        lag_member_id2 = sai_thrift_create_lag_member(self.client, lag_id1, port2)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, lag_id1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV6
        ip_addr1 = '4001::1'
        ip_addr1_subnet = '4001::0'
        ip_mask1 = 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:0'
        dmac1 = '00:11:22:33:44:55'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)

        # send the test packet(s)
        try:
            pkt = simple_tcpv6_packet(eth_dst=router_mac,
                                    eth_src='00:22:22:22:22:22',
                                    ipv6_dst='4001::1',
                                    ipv6_src='5001::1',
                                    ipv6_hlim=64)

            exp_pkt = simple_tcpv6_packet(
                                    eth_dst='00:11:22:33:44:55',
                                    eth_src=router_mac,
                                    ipv6_dst='4001::1',
                                    ipv6_src='5001::1',
                                    ipv6_hlim=63)
            send_packet(self, 2, str(pkt))
            verify_packets_any(self, exp_pkt, [0, 1])
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)

            sai_thrift_remove_lag_member(self.client, lag_member_id1)
            sai_thrift_remove_lag_member(self.client, lag_member_id2)
            self.client.sai_thrift_remove_lag(lag_id1)
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('l3')
@group('ecmp')
@group('lag')
class L3EcmpLagTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        switch_init(self.client)
        if len(port_list) < 7: 
            assert False, "skip this test as it requires 7 ports"

        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        port5 = port_list[4]
        port6 = port_list[5]
        port7 = port_list[6]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.0.0'
        ip_mask1 = '255.255.0.0'
        nhop_ip1 = '11.11.11.11'
        nhop_ip2 = '22.22.22.22'
        nhop_ip3 = '33.33.33.33'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'
        dmac3 = '00:11:22:33:44:57'

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        lag_id1 = self.client.sai_thrift_create_lag([])
        lag_id2 = self.client.sai_thrift_create_lag([])

        lag_member_id1 = sai_thrift_create_lag_member(self.client, lag_id1, port1)
        lag_member_id2 = sai_thrift_create_lag_member(self.client, lag_id1, port2)
        lag_member_id3 = sai_thrift_create_lag_member(self.client, lag_id1, port3)
        lag_member_id4 = sai_thrift_create_lag_member(self.client, lag_id2, port4)
        lag_member_id5 = sai_thrift_create_lag_member(self.client, lag_id2, port5)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, lag_id1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, lag_id2, 0, v4_enabled, v6_enabled, mac)
        rif_id3 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port6, 0, v4_enabled, v6_enabled, mac)
        rif_id4 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port7, 0, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, nhop_ip2, dmac2)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id3, nhop_ip3, dmac3)

        nhop1 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip1, rif_id1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip2, rif_id2)
        nhop3 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip3, rif_id3)

        nhop_group1 = sai_thrift_create_next_hop_group(self.client)

        nhop_gmember1 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop1)
        nhop_gmember2 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop2)
        nhop_gmember3 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop3)

        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop_group1)

        try:
            count = [0, 0, 0, 0, 0, 0]
            dst_ip = int(socket.inet_aton('10.10.10.1').encode('hex'), 16)
            src_mac_start = '00:22:22:22:{0}:{1}'
            max_itrs = 500
            for i in range(0, max_itrs):
                dst_ip_addr = socket.inet_ntoa(hex(dst_ip)[2:].zfill(8).decode('hex'))
                src_mac = src_mac_start.format(str(i).zfill(4)[:2], str(i).zfill(4)[2:])
                pkt = simple_tcp_packet(eth_dst=router_mac,
                        eth_src=src_mac,
                        ip_dst=dst_ip_addr,
                        ip_src='192.168.8.1',
                        ip_id=106,
                        ip_ttl=64)

                exp_pkt1 = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                        eth_src=router_mac,
                        ip_dst=dst_ip_addr,
                        ip_src='192.168.8.1',
                        ip_id=106,
                        ip_ttl=63)
                exp_pkt2 = simple_tcp_packet(eth_dst='00:11:22:33:44:56',
                        eth_src=router_mac,
                        ip_dst=dst_ip_addr,
                        ip_src='192.168.8.1',
                        ip_id=106,
                        ip_ttl=63)
                exp_pkt3 = simple_tcp_packet(eth_dst='00:11:22:33:44:57',
                        eth_src=router_mac,
                        ip_dst=dst_ip_addr,
                        ip_src='192.168.8.1',
                        ip_id=106,
                        ip_ttl=63)

                send_packet(self, 6, str(pkt))
                rcv_idx = verify_any_packet_any_port(self,
                                                     [exp_pkt1, exp_pkt2, exp_pkt3],
                                                     [0, 1, 2, 3, 4, 5])
                count[rcv_idx] += 1
                dst_ip += 1

            print count
            ecmp_count = [count[0]+count[1]+count[2], count[3]+count[4], count[5]]
            for i in range(0, 3):
                self.assertTrue((ecmp_count[i] >= ((max_itrs / 3) * 0.75)),
                                "Ecmp paths are not equally balanced")
            for i in range(0, 3):
                self.assertTrue((count[i] >= ((max_itrs / 9) * 0.75)),
                                "Lag path1 is not equally balanced")
            for i in range(3, 5):
                self.assertTrue((count[i] >= ((max_itrs / 6) * 0.75)),
                                "Lag path2 is not equally balanced")
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop_group1)

            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember1)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember2)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember3)

            self.client.sai_thrift_remove_next_hop_group(nhop_group1)

            self.client.sai_thrift_remove_next_hop(nhop1)
            self.client.sai_thrift_remove_next_hop(nhop2)
            self.client.sai_thrift_remove_next_hop(nhop3)

            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, nhop_ip2, dmac2)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id3, nhop_ip3, dmac3)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_router_interface(rif_id3)
            self.client.sai_thrift_remove_router_interface(rif_id4)

            self.client.sai_thrift_remove_lag_member(lag_member_id1)
            self.client.sai_thrift_remove_lag_member(lag_member_id2)
            self.client.sai_thrift_remove_lag_member(lag_member_id3)
            self.client.sai_thrift_remove_lag_member(lag_member_id4)
            self.client.sai_thrift_remove_lag_member(lag_member_id5)

            self.client.sai_thrift_remove_lag(lag_id1)
            self.client.sai_thrift_remove_lag(lag_id2)

            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('l3')
class L3EcmpLagTestMini(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.0.0'
        ip_mask1 = '255.255.0.0'
        ip_mask2 = '255.255.255.0'
        nhop_ip1 = '11.11.11.11'
        nhop_ip1_subnet = '11.11.11.0'
        nhop_ip2 = '22.22.22.22'
        nhop_ip2_subnet = '22.22.22.0'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        lag_id1 = self.client.sai_thrift_create_lag([])

        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, [port1, port2])

        lag_member11 = sai_thrift_create_lag_member(self.client, lag_id1, port1)
        lag_member12 = sai_thrift_create_lag_member(self.client, lag_id1, port2)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, lag_id1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)
        rif_id3 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port4, 0, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, nhop_ip2, dmac2)

        nhop1 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip1, rif_id1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip2, rif_id2)

        nhop_group1 = sai_thrift_create_next_hop_group(self.client)

        nhop_gmember1 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop1)
        nhop_gmember2 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop2)

        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop_group1)
        sai_thrift_create_route(self.client, vr_id, addr_family, nhop_ip1_subnet, ip_mask2, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, nhop_ip2_subnet, ip_mask2, rif_id2)

        try:
            count = [0, 0, 0]
            dst_ip = int(socket.inet_aton('10.10.10.1').encode('hex'), 16)
            src_mac_start = '00:22:22:22:23:'
            max_itrs = 500
            dport = 0x50
            for i in range(0, max_itrs):
                dst_ip_addr = socket.inet_ntoa(hex(dst_ip)[2:].zfill(8).decode('hex'))
                src_mac = src_mac_start + str(i%99).zfill(2)
                pkt = simple_tcp_packet(eth_dst=router_mac,
                        eth_src=src_mac,
                        ip_dst=dst_ip_addr,
                        ip_src='192.168.8.1',
                        ip_id=106,
                        tcp_dport=dport,
                        ip_ttl=64)

                exp_pkt1 = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                        eth_src=router_mac,
                        ip_dst=dst_ip_addr,
                        ip_src='192.168.8.1',
                        ip_id=106,
                        tcp_dport=dport,
                        ip_ttl=63)
                exp_pkt2 = simple_tcp_packet(eth_dst='00:11:22:33:44:56',
                        eth_src=router_mac,
                        ip_dst=dst_ip_addr,
                        ip_src='192.168.8.1',
                        ip_id=106,
                        tcp_dport=dport,
                        ip_ttl=63)

                send_packet(self, 3, str(pkt))
                rcv_idx = verify_any_packet_any_port(self,
                              [exp_pkt1, exp_pkt2],
                              [0, 1, 2])
                count[rcv_idx] += 1
                dst_ip += 1
                dport += 20

            print count
            ecmp_count = [count[0] + count[1], count[2]]
            for i in range(0, 2):
                self.assertTrue((ecmp_count[i] >= ((max_itrs / 2) * 0.75)),
                        "Ecmp paths are not equally balanced")
            for i in range(0, 2):
                self.assertTrue((count[i] >= ((max_itrs / 4) * 0.75)),
                        "Lag path1 is not equally balanced")
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop_group1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, nhop_ip1_subnet, ip_mask2, rif_id1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, nhop_ip2_subnet, ip_mask2, rif_id2)

            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember1)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember2)

            self.client.sai_thrift_remove_next_hop_group(nhop_group1)

            self.client.sai_thrift_remove_next_hop(nhop1)
            self.client.sai_thrift_remove_next_hop(nhop2)

            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, nhop_ip2, dmac2)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_router_interface(rif_id3)

            sai_thrift_remove_lag_member(self.client, lag_member11)
            sai_thrift_remove_lag_member(self.client, lag_member12)

            self.client.sai_thrift_remove_lag(lag_id1)

            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('l3')
class L3VIIPv4HostTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending packet port 1 -> port 2 (192.168.0.1 -> 10.10.10.1 [id = 101])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 1
        vlan_id = 10
        mac_action = SAI_PACKET_ACTION_FORWARD

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_addr1_subnet = '10.10.10.0'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:0a:00:00:00:01'
        ip_addr2 = '11.11.11.1'
        ip_addr2_subnet = '11.11.11.0'
        ip_mask2 = '255.255.255.0'
        dmac2 = '00:0b:00:00:00:01'
        mac1 = ''
        mac2 = ''

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan_oid, v4_enabled, v6_enabled, mac1)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac2)

        sai_thrift_create_fdb(self.client, vlan_oid, dmac1, port1, mac_action)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)

        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac2)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, ip_addr2, rif_id2)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr2_subnet, ip_mask2, rif_id2)

        try:
            # send the test packet(s)
            pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src='00:0a:00:00:00:01',
                                ip_dst='11.11.11.1',
                                ip_src='10.10.10.1',
                                ip_id=105,
                                ip_ttl=64)
            exp_pkt = simple_tcp_packet(
                                eth_dst='00:0b:00:00:00:01',
                                eth_src=router_mac,
                                ip_dst='11.11.11.1',
                                ip_src='10.10.10.1',
                                ip_id=105,
                                ip_ttl=63)
            send_packet(self, 0, str(pkt))
            verify_packets(self, exp_pkt, [1])

            # send the test packet(s)
            pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src='00:0b:00:00:00:01',
                                ip_dst='10.10.10.1',
                                ip_src='11.11.11.1',
                                ip_id=105,
                                ip_ttl=64)
            exp_pkt = simple_tcp_packet(
                                eth_dst='00:0a:00:00:00:01',
                                eth_src=router_mac,
                                ip_dst='10.10.10.1',
                                ip_src='11.11.11.1',
                                ip_id=105,
                                ip_ttl=63)
            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0])
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr2_subnet, ip_mask2, rif_id2)
            self.client.sai_thrift_remove_next_hop(nhop1)
            self.client.sai_thrift_remove_next_hop(nhop2)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac2)
            sai_thrift_delete_fdb(self.client, vlan_oid, dmac1, port1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan(vlan_oid)
            self.client.sai_thrift_remove_virtual_router(vr_id)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)

@group('l3')
class L3IPv4MacRewriteTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending packet port 1 -> port 2 (192.168.0.1 -> 10.10.10.1 [id = 101])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 1

        mac1 = rewrite_mac1
        mac2 = rewrite_mac2

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac1)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac2)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        nhop_ip1 = '11.11.11.11'
        nhop_ip1_subnet = '11.11.11.0'
        nhop_ip1_mask = '255.255.255.0'
        ip_mask1 = '255.255.255.255'
        ip_addr1 = '10.10.10.1'
        dmac1 = '00:11:22:33:44:55'

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, nhop_ip1_subnet, nhop_ip1_mask, rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=rewrite_mac2,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(
                                eth_dst='00:11:22:33:44:55',
                                eth_src=rewrite_mac1,
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=63)
        try:
            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0])
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, nhop_ip1_subnet, nhop_ip1_mask, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('l3')
class L3VlanNeighborMacUpdateTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        For sai server for testing learning inside vlan and fwd packet to vlan through l3
        Steps
        1. Create VLAN
        2. Create two VLAN members
        3. Set port VLAN IDs
        4. Create virtual router
        5. Create router interface for VLAN and an extra port 3
        6. Set SAI PORT LEARN MODE HW
        7. Send packet from one port of the VLAN to the DUT to update the FDB entry
        8. Create neighbor and route
        9. Send L3 packet from port to the VLAN with the destination IP and verify that only the targeted port receives the packet and the MAC in the packet is updated 
        10. clean up.
        """

        print
        print "Sending packet port 1 -> switch (for learning purpuse)"
        print "and then sending packet from port 3 -> port 1 (through the router)"
        switch_init(self.client)
        sai_thrift_clear_all_counters(self.client)
        v4_enabled = 1
        v6_enabled = 1
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1_subnet = '10.10.10.0'
        ip_mask1 = '255.255.255.0'
        mac_port1 = '00:0a:00:00:00:01'
        ip_port1 = '10.10.10.2'
        mac_port3 = '00:0b:00:00:00:01'
        mac1 = ''
        mac2 = ''

        vlan1 = sai_thrift_create_vlan(self.client, vlan_id)

        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan1, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan1, port2, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value1 = sai_thrift_attribute_value_t(u16=vlan_id)
        attr1 = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value1)
        self.client.sai_thrift_set_port_attribute(port1, attr1)
        self.client.sai_thrift_set_port_attribute(port2, attr1)

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_vlan_id = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan1, v4_enabled, v6_enabled, mac1)
        rif_port_id = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac2)

        attr_value2 = sai_thrift_attribute_value_t(s32=SAI_BRIDGE_PORT_FDB_LEARNING_MODE_HW)
        attr2 = sai_thrift_attribute_t(id=SAI_BRIDGE_PORT_ATTR_FDB_LEARNING_MODE, value=attr_value2)
        self.client.sai_thrift_set_port_attribute(sai_thrift_get_bridge_port_by_port(self.client, port1), attr2)
        self.client.sai_thrift_set_port_attribute(sai_thrift_get_bridge_port_by_port(self.client, port2), attr2)

        sai_thrift_create_neighbor(self.client, addr_family, rif_vlan_id, ip_port1, mac_port1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_vlan_id)

        local_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src=mac_port1,
                                dl_vlan_enable=True,
                                vlan_vid=vlan_id,
                                ip_dst='10.0.0.1',
                                ip_src=ip_port1,
                                ip_id=102,
                                ip_ttl=64)

        try:
            #sending unkown UC for learning the ports mac and expecting flooding only on the vlan
            send_packet(self, 0, str(local_pkt))
            verify_packets(self, local_pkt, [1])

            #sending L3 packet from port 3 through router to port 1 that update the fdb with is MAC
            L3_pkt = simple_tcp_packet(pktlen=100,
                                       eth_dst=router_mac,
                                       eth_src=mac_port3,
                                       ip_src='11.11.11.1',
                                       ip_dst=ip_port1,
                                       ip_id=105,
                                       ip_ttl=64)
            exp_pkt = simple_tcp_packet(pktlen=104,#additional 4bytes because of the vlan
                                        eth_dst=mac_port1,
                                        eth_src=router_mac,
                                        ip_dst=ip_port1,
                                        ip_src='11.11.11.1',
                                        dl_vlan_enable=True,
                                        vlan_vid=vlan_id,
                                        ip_id=105,
                                        ip_ttl=63)

            send_packet(self, 2, str(L3_pkt))
            verify_packets(self, exp_pkt, [0])
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_vlan_id)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_vlan_id, ip_port1, mac_port1)

            self.client.sai_thrift_remove_router_interface(rif_vlan_id)
            self.client.sai_thrift_remove_router_interface(rif_port_id)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)

            self.client.sai_thrift_remove_vlan(vlan1)

            self.client.sai_thrift_remove_virtual_router(vr_id)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)

@group('lag')
@group('l3')
class L3MultipleLagTest(sai_base_test.ThriftInterfaceDataPlane):
    total_lag_port = 16
    v4_enabled = 1
    v6_enabled = 1
    ip_mask = '255.255.255.0'
    addr_family = SAI_IP_ADDR_FAMILY_IPV4
    lag_members = []
    lags = []
    lags_rifs = []
    neighbors = []
    routes = []
    vr_id = 0
    mac_action = SAI_PACKET_ACTION_FORWARD
    src_port = 0
    mac_pool = ['00:11:22:33:44:50',
               '00:11:23:33:44:51',
               '00:11:24:33:44:52',
               '00:11:25:33:44:53',
               '00:11:26:33:44:54',
               '00:11:27:33:44:55',
               '00:11:28:33:44:56',
               '00:11:29:33:44:57',
               '00:11:30:33:44:58',
               '00:11:31:33:44:59',
               '00:11:32:33:44:60',
               '00:11:33:33:44:61',
               '00:11:34:33:44:62',
               '00:11:35:33:44:63',
               '00:11:36:33:44:64',
               '00:11:37:33:44:65',
               '00:11:38:33:44:66']

    def setup_lags(self, num_of_lags, port_list):
        for i in xrange(num_of_lags):
            self.lags.append(self.client.sai_thrift_create_lag([]))
        for i in xrange(self.total_lag_port):
            self.lag_members.append(sai_thrift_create_lag_member(self.client, self.lags[i % num_of_lags], port_list[i]))
        for i in xrange(num_of_lags):
            self.lags_rifs.append(sai_thrift_create_router_interface(self.client, self.vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, self.lags[i], 0, self.v4_enabled, self.v6_enabled, ''))
        for i in xrange(num_of_lags):
            sai_thrift_create_neighbor(self.client, self.addr_family, self.lags_rifs[i], "10.10.%s.1" % str(i+1), self.mac_pool[i])
            sai_thrift_create_route(self.client, self.vr_id, self.addr_family, "10.10.%s.0" % str(i+1), self.ip_mask, self.lags_rifs[i])

    def teardown_lags(self, num_of_lags, port_list):
        if (num_of_lags == 0 ): return
        for i in xrange(num_of_lags):
            sai_thrift_remove_neighbor(self.client, self.addr_family, self.lags_rifs[i], "10.10.%s.1" % str(i+1), self.mac_pool[i])
            sai_thrift_remove_route(self.client, self.vr_id, self.addr_family, "10.10.%s.0" % str(i+1), self.ip_mask, self.lags_rifs[i])
        for rif in self.lags_rifs:
            self.client.sai_thrift_remove_router_interface(rif)
        del self.lags_rifs[:]
        for lag_member in self.lag_members:
            self.client.sai_thrift_remove_lag_member(lag_member)
        del self.lag_members[:]
        for lag in self.lags:
            self.client.sai_thrift_remove_lag(lag)
        del self.lags[:]

    def send_and_verify_packets(self, num_of_lags, port_list):
        exp_pkts = [0]*self.total_lag_port
        pkt_counter = [0] * self.total_lag_port
        destanation_ports = range(self.total_lag_port)
        sport = 0x1234
        dport = 0x50
        src_mac_start = '00:22:22:22:{0}:{1}'
        NUM_OF_PKT_TO_EACH_PORT = 254
        NUM_OF_PKTS_TO_SEND = NUM_OF_PKT_TO_EACH_PORT * self.total_lag_port
        for i in xrange(NUM_OF_PKTS_TO_SEND):
                ip_src = '10.0.' + str(i % 255) + '.' + str(i % 255)
                ip_dst = '10.10.' + str((i % num_of_lags) + 1) + '.1'
                src_mac = src_mac_start.format(str(i).zfill(4)[:2], str(i).zfill(4)[2:])
                pkt = simple_tcp_packet(eth_dst=router_mac,
                                        eth_src=src_mac,
                                        ip_src=ip_src,
                                        ip_dst=ip_dst,
                                        ip_id=i,
                                        tcp_sport=sport,
                                        tcp_dport=dport,
                                        ip_ttl=64)
                exp_pkt = simple_tcp_packet(eth_dst=self.mac_pool[i % num_of_lags],
                                            eth_src=router_mac,
                                            ip_src=ip_src,
                                            ip_dst=ip_dst,
                                            ip_id=i,
                                            tcp_sport=sport,
                                            tcp_dport=dport,
                                            ip_ttl=63)

                send_packet(self, self.total_lag_port, str(pkt))
                (match_index,rcv_pkt) = verify_packet_any_port(self,exp_pkt,destanation_ports)
                logging.debug("found expected packet from port %d" % destanation_ports[match_index])
                pkt_counter[match_index] += 1
                sport = random.randint(0,0xffff)
                dport = random.randint(0,0xffff)

        #final uniform distribution check
        for stat_port in xrange(self.total_lag_port):
            logging.debug( "PORT #"+str(hex(port_list[stat_port]))+":")
            logging.debug(str(pkt_counter[stat_port]))
            self.assertTrue((pkt_counter[stat_port] >= ((NUM_OF_PKT_TO_EACH_PORT ) * 0.8)),
                    "Not all paths are equally balanced, %s" % pkt_counter[stat_port])
            self.assertTrue((pkt_counter[stat_port] <= ((NUM_OF_PKT_TO_EACH_PORT ) * 1.2)),
                    "Not all paths are equally balanced, %s" % pkt_counter[stat_port])

    def runTest(self):
        """
        For sai server, testing different lags with router
        ---- Test for 17 ports minimun ----
        Steps
        1. Create virtual router
        2. Reserve port 16 for sending packets
        3. Create router interfaces 1-for all the lags, 2-for the source port 
        4. Create sixteen LAGs with each hash one member
        5. Config neighbors and routes 
        6. Send packet and check for arrivals balanced traffic
        7. Repeat steps 3-6 with 8 lags with each has 2 members, 4 lags with 4 members, 2 lags with 8 members and 1 lag with 16 members
        8. clean up.
        """

        print
        print "L3MultipleLagTest"
        #general configuration 
        random.seed(1)
        switch_init(self.client)
        if (len(port_list) < (self.total_lag_port + 1) ) : 
            assert False, "skip this test as it requires 17 ports"

        self.src_port = port_list[self.total_lag_port]
        self.vr_id = sai_thrift_create_virtual_router(self.client, self.v4_enabled, self.v6_enabled)
        rif_port_id = sai_thrift_create_router_interface(self.client, self.vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, self.src_port, 0, self.v4_enabled, self.v6_enabled, '')
        num_of_lags = self.total_lag_port

        try:
            while (num_of_lags > 0):
                print "Testing with " + str(num_of_lags) + " lags."
                self.setup_lags(num_of_lags,port_list)
                self.send_and_verify_packets(num_of_lags,port_list)
                self.teardown_lags(num_of_lags,port_list)
                num_of_lags /= 2
        finally:
            #in case of an exception in the send_and_verify_packets
            self.teardown_lags(num_of_lags,port_list)
            self.client.sai_thrift_remove_router_interface(rif_port_id)
            self.client.sai_thrift_remove_virtual_router(self.vr_id)
            print "END OF TEST"

@group('lag')
@group('l3')
class L3MultipleEcmpLagTest(sai_base_test.ThriftInterfaceDataPlane):
    # ports that will change from rif to lag member
    total_changing_ports = 15
    #the first port that will start as rif, that means that the first iteratio will only have port  #1
    first_changing_port = 2
    total_dst_port = 16
    v4_enabled = 1
    v6_enabled = 1
    ip_mask = '255.255.0.0'
    addr_family = SAI_IP_ADDR_FAMILY_IPV4
    lag_members = []
    nhop_group =  0
    lag = 0
    lag_rif = 0
    port_rifs = []
    neighbors = []
    nhops = []
    nhop_gmembers = []
    routes = []
    vr_id = 0
    mac_action = SAI_PACKET_ACTION_FORWARD
    src_port = 0
    mac_pool = []

    def setup_ecmp_lag_group(self, first_rif_port):
        self.lag = self.client.sai_thrift_create_lag([])
        #adding lag members
        self.lag_members.append(sai_thrift_create_lag_member(self.client, self.lag, port_list[1]))
        for i in range(self.first_changing_port,first_rif_port):
            self.lag_members.append(sai_thrift_create_lag_member(self.client, self.lag, port_list[i]))
        self.lag_rif = sai_thrift_create_router_interface(self.client, self.vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, self.lag, 0, self.v4_enabled, self.v6_enabled, '')
        sai_thrift_create_neighbor(self.client, self.addr_family, self.lag_rif, "10.10.0.1", self.mac_pool[self.total_changing_ports])
        sai_thrift_create_route(self.client, self.vr_id,self.addr_family, "10.10.0.1", '255.255.255.0', self.lag_rif)
        self.nhops.append(sai_thrift_create_nhop(self.client, self.addr_family, "10.10.0.1" , self.lag_rif))
        for i in range(first_rif_port,self.total_changing_ports):
            self.port_rifs.append(sai_thrift_create_router_interface(self.client, self.vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port_list[i], 0, self.v4_enabled, self.v6_enabled, ''))
        for i in range(len(self.port_rifs)):
            sai_thrift_create_neighbor(self.client, self.addr_family, self.port_rifs[i], "10.10.%s.1" % str(i+1), self.mac_pool[i])
            self.nhops.append(sai_thrift_create_nhop(self.client, self.addr_family, "10.10.%s.1" % str(i+1), self.port_rifs[i]))
            sai_thrift_create_route(self.client, self.vr_id, self.addr_family, "10.10.%s.1" % str(i+1), '255.255.255.0', self.port_rifs[i])
        self.nhop_group = sai_thrift_create_next_hop_group(self.client)
        for nhop in self.nhops:
            self.nhop_gmembers.append(sai_thrift_create_next_hop_group_member(self.client, self.nhop_group, nhop))
        sai_thrift_create_route(self.client, self.vr_id, self.addr_family, "10.20.0.0", self.ip_mask, self.nhop_group)

    def teardown_ecmp_lag_group(self, first_rif_port):
        sai_thrift_remove_route(self.client, self.vr_id, self.addr_family, "10.20.0.0", self.ip_mask, self.nhop_group)
        sai_thrift_remove_route(self.client, self.vr_id, self.addr_family, "10.10.0.1", '255.255.255.0', self.lag_rif)
        for i in range(self.total_changing_ports - first_rif_port):
            sai_thrift_remove_route(self.client, self.vr_id, self.addr_family, "10.10.%s.1" % str(i+1), '255.255.255.0', self.port_rifs[i])
        for nhop_gmember in self.nhop_gmembers:
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember)
        self.client.sai_thrift_remove_next_hop_group(self.nhop_group)
        for nhop in self.nhops:
            self.client.sai_thrift_remove_next_hop(nhop)
        del self.nhops[:]
        for i in range(self.total_changing_ports - first_rif_port):
            sai_thrift_remove_neighbor(self.client, self.addr_family, self.port_rifs[i], "10.10.%s.1" % str(i+1), self.mac_pool[i])
        print self.port_rifs
        for rif in self.port_rifs:
            self.client.sai_thrift_remove_router_interface(rif)
        del self.port_rifs[:]
        for lag_member in self.lag_members:
            self.client.sai_thrift_remove_lag_member(lag_member)
        del self.lag_members[:]
        sai_thrift_remove_neighbor(self.client, self.addr_family, self.lag_rif, "10.10.0.1", self.mac_pool[self.total_changing_ports])
        self.client.sai_thrift_remove_router_interface(self.lag_rif)
        self.client.sai_thrift_remove_lag(self.lag)

    def polarizationCheck(self,packets,avg):
        if (avg < 150):
            self.assertTrue((packets >= (avg * 0.65)),"Not all paths are equally balanced, %s" % packets)
            self.assertTrue((packets <= (avg * 1.35)),"Not all paths are equally balanced, %s" % packets)
        else:
            self.assertTrue((packets >= (avg * 0.8)),"Not all paths are equally balanced, %s" % packets)
            self.assertTrue((packets <= (avg * 1.2)),"Not all paths are equally balanced, %s" % packets)

    def send_and_verify_packets(self, first_rif_port):
        exp_pkts = [0]*self.total_dst_port
        pkt_counter = [0] * self.total_dst_port
        destanation_ports = range(self.total_dst_port + 1)
        sport = 0x1234
        dport = 0x50
        src_mac_start = '00:22:22:22:{0}:{1}'
        IP_LAST_WORD_RANGE = 254
        IP_2ND_LAST_WORD_RANGE = 16
        for i in xrange(IP_LAST_WORD_RANGE):
                for j in xrange(IP_2ND_LAST_WORD_RANGE):
                    ip_src = '10.0.' + str(j) + '.' + str(i+1)
                    ip_dst = '10.20.' + str(j+1) + '.1'
                    src_mac = src_mac_start.format(str(i).zfill(4)[:2], str(i).zfill(4)[2:])
                    pkt = simple_tcp_packet(
                                            eth_dst=router_mac,
                                            eth_src=src_mac,
                                            ip_src=ip_src,
                                            ip_dst=ip_dst,
                                            ip_id=i,
                                            tcp_sport=sport,
                                            tcp_dport=dport,
                                            ip_ttl=64)
                    exp_pkt = simple_tcp_packet(
                                            eth_dst=self.mac_pool[0],
                                            eth_src=router_mac,
                                            ip_src=ip_src,
                                            ip_dst=ip_dst,
                                            ip_id=i,
                                            tcp_sport=sport,
                                            tcp_dport=dport,
                                            ip_ttl=63)
                    masked_exp_pkt = Mask(exp_pkt)
                    masked_exp_pkt.set_do_not_care_scapy(ptf.packet.Ether,"dst")

                    send_packet(self, 0, str(pkt))
                    (match_index,rcv_pkt) = verify_packet_any_port(self,masked_exp_pkt,destanation_ports)
                    logging.debug("Found expected packet from port %d" % destanation_ports[match_index])
                    pkt_counter[match_index] += 1
                    sport = random.randint(0,0xffff)
                    dport = random.randint(0,0xffff)

        #final uniform distribution check
        logging.debug(pkt_counter)
        logging.debug(first_rif_port)
        lag_packets = sum(pkt_counter[1:first_rif_port])
        lag_average = lag_packets/(len(self.lag_members) + 1)
        logging.debug("the sum of packets through the lag is " + str(lag_packets))
        logging.debug("the lag average for the lag is " + str(lag_average))
        for stat_port in range(1,first_rif_port):
            logging.debug( "PORT #"+str(stat_port)+":")
            logging.debug(str(pkt_counter[stat_port]))
            self.polarizationCheck(pkt_counter[stat_port],lag_average)
        rifs_average = sum(pkt_counter)/(len(self.port_rifs) + 1)
        logging.debug("lag average " + str(lag_average))
        self.polarizationCheck(lag_packets,rifs_average)
        for stat_port in range(first_rif_port,self.total_changing_ports):
            logging.debug( "PORT #"+str(stat_port)+":")
            logging.debug(str(pkt_counter[stat_port]))
            self.polarizationCheck(pkt_counter[stat_port],rifs_average)

    def runTest(self):
        """
        For sai server, testing different lags with router
        ---- Test for 16 ports minimun ----
        Steps
        1. Create virtual router, and rif for src port
        2. create a lag and lag rif,add ports to the lag and the rest of the ports connect to rifs
        3. configure neighbors, nhops for all of the rifs
        4. make ecmp route with all of the nhops
        5. send packets from src port
        6. check polarization check in the lag and in the ecmp
        7. remove rifs, neighbors, nhops, lag members, lag and route
        8. repeat steps 3-7 with differnt numbers of lag members and rifs
        8. clean up.
        """

        print
        print "L3MultipleEcmpLagTest"
        #general configuration
        random.seed(1)
        switch_init(self.client)
        if (len(port_list) < (self.total_dst_port + 1) ) :
            assert False, "skip this test as it requires 17 ports"

        self.src_port = port_list[0]
        for i in range (self.total_dst_port+1):
            self.mac_pool.append('00:11:22:33:44:'+str(50+i))

        self.vr_id = sai_thrift_create_virtual_router(self.client, self.v4_enabled, self.v6_enabled)
        rif_port_id = sai_thrift_create_router_interface(self.client, self.vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, self.src_port, 0, self.v4_enabled, self.v6_enabled, '')

        try:
            # The first iteration will configure port #1 as a lag with only one member
            # and will configure port #2 to port #15 as rifs,
            # the rif will advance until all of the ports will be in lag and only one if port
            for first_rif_port in range(self.first_changing_port,self.total_changing_ports):
                print "Testing with " + str(first_rif_port - 1) + " lag members."
                self.setup_ecmp_lag_group(first_rif_port)
                self.send_and_verify_packets(first_rif_port)
                self.teardown_ecmp_lag_group(first_rif_port)
        finally:

            #in case of an exception in the send_and_verify_packets
            self.teardown_ecmp_lag_group(self.total_dst_port)#check what number to send for tear down
            self.client.sai_thrift_remove_router_interface(rif_port_id)
            self.client.sai_thrift_remove_virtual_router(self.vr_id)
            print "END OF TEST"

@group('l3')
@group('1D')
class L3BridgeAndSubPortRifTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print ""
        switch_init(self.client)
        v4_enabled = 1
        v6_enabled = 1
        vlan1_id = 10
        vlan2_id = 100
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]

        mac1 = '00:01:01:01:01:01'
        ip1 = '11.11.11.1'

        mac2 = '00:02:02:02:02:02'
        ip2 = '10.10.10.2'
        ip_addr_subnet = '10.10.10.0'
        ip_mask = '255.255.255.0'

        mac3 = '00:22:22:22:22:22'
        ip3 = '10.0.0.1'
        
        vlan1_oid = sai_thrift_create_vlan(self.client, vlan1_id)
        vlan2_oid = sai_thrift_create_vlan(self.client, vlan2_id)

        bridge_id = sai_thrift_create_bridge(self.client, SAI_BRIDGE_TYPE_1D)
        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, [port2, port3, port4])
        bport1_id = sai_thrift_create_bridge_sub_port(self.client, port2, bridge_id, vlan2_id)
        bport2_id = sai_thrift_create_bridge_sub_port(self.client, port3, bridge_id, vlan2_id)
        bport3_id = sai_thrift_create_bridge_sub_port(self.client, port4, bridge_id, vlan2_id)

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)
        
        sub_port_rif_oid = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_SUB_PORT, port1, vlan1_id, v4_enabled, v6_enabled, '')
        bridge_rif_oid = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_BRIDGE, 0, 0, v4_enabled, v6_enabled, '')
        bridge_rif_bp = sai_thrift_create_bridge_rif_port(self.client, bridge_id, bridge_rif_oid)

        sai_thrift_create_neighbor(self.client, SAI_IP_ADDR_FAMILY_IPV4, bridge_rif_oid, ip2, mac2)
        sai_thrift_create_route(self.client, vr_id, SAI_IP_ADDR_FAMILY_IPV4, ip_addr_subnet, ip_mask, bridge_rif_oid)

        local_pkt = simple_tcp_packet(eth_src=mac2,
                                      eth_dst=mac3,
                                      dl_vlan_enable=True,
                                      vlan_vid=vlan2_id,
                                      ip_src=ip2,
                                      ip_dst=ip3,
                                      ip_id=102,
                                      ip_ttl=64)

        L3_pkt = simple_tcp_packet(eth_src=mac1,
                                   eth_dst=router_mac,
                                   ip_src=ip1,
                                   ip_dst=ip2,
                                   dl_vlan_enable=True,
                                   vlan_vid=vlan1_id,
                                   ip_id=105,
                                   ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_src=router_mac,
                                    eth_dst=mac2,
                                    ip_src=ip1,
                                    ip_dst=ip2,
                                    dl_vlan_enable=True,
                                    vlan_vid=vlan2_id,
                                    ip_id=105,
                                    ip_ttl=63)

        try:
            print "Sending packet ({} -> {}) : Sub-port rif (port 1 : vlan {}) -> Bridge rif (flooded)".format(ip1, ip2, vlan1_id)
            send_packet(self, 0, str(L3_pkt))
            verify_packets(self, exp_pkt, [1, 2, 3])
            print "Success"

            print "Sending unknown L2 packet [{} -> {}] to learn FDB and flood within a .1D bridge".format(mac1, mac3)
            send_packet(self, 1, str(local_pkt))
            verify_packets(self, local_pkt, [2, 3])
            print "Success"

            print "Sending packet ({} -> {}) : Sub-port rif (port 1 : vlan {}) -> Bridge rif".format(ip1, ip2, vlan1_id)
            send_packet(self, 0, str(L3_pkt))
            verify_packets(self, exp_pkt, [1])
            print "Success"

        finally:
            sai_thrift_remove_route(self.client, vr_id, SAI_IP_ADDR_FAMILY_IPV4, ip_addr_subnet, ip_mask, bridge_rif_oid)
            sai_thrift_remove_neighbor(self.client, SAI_IP_ADDR_FAMILY_IPV4, bridge_rif_oid, ip2, mac2)
            self.client.sai_thrift_remove_router_interface(sub_port_rif_oid)
            self.client.sai_thrift_remove_bridge_port(bridge_rif_bp)
            self.client.sai_thrift_remove_router_interface(bridge_rif_oid)
            self.client.sai_thrift_remove_virtual_router(vr_id)

            sai_thrift_remove_bridge_sub_port(self.client, bport1_id, port2)
            sai_thrift_remove_bridge_sub_port(self.client, bport2_id, port3)
            sai_thrift_remove_bridge_sub_port(self.client, bport3_id, port4)
            self.client.sai_thrift_remove_bridge(bridge_id)

            self.client.sai_thrift_remove_vlan(vlan1_oid)
            self.client.sai_thrift_remove_vlan(vlan2_oid)

            sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port4, SAI_VLAN_TAGGING_MODE_UNTAGGED)

@group('l3')
@group('1D')
class L3SubPortAndVLANRifTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print ""
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 1
        vlan1_id = 10
        vlan2_id = 100
        mac_action = SAI_PACKET_ACTION_FORWARD

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_addr1_subnet = '10.10.10.0'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:0a:00:00:00:01'

        ip_addr2 = '11.11.11.1'
        ip_addr2_subnet = '11.11.11.0'
        ip_mask2 = '255.255.255.0'
        dmac2 = '00:0b:00:00:00:01'

        vlan1_oid = sai_thrift_create_vlan(self.client, vlan1_id)
        vlan2_oid = sai_thrift_create_vlan(self.client, vlan2_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan1_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan1_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan1_oid, v4_enabled, v6_enabled, '')
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_SUB_PORT, port2, vlan2_id, v4_enabled, v6_enabled, '')

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)

        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac2)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr2_subnet, ip_mask2, rif_id2)

        try:
            print "Sending packet ({} -> {}) : VLAN {} rif -> Sub-port (port 2 : vlan {}) rif".format(ip_addr1, ip_addr2, vlan1_id, vlan2_id)
            pkt = simple_tcp_packet(eth_dst=router_mac,
                                    eth_src=dmac1,
                                    ip_dst=ip_addr2,
                                    ip_src=ip_addr1,
                                    ip_id=105,
                                    ip_ttl=64,
                                    dl_vlan_enable=True,
                                    vlan_vid=vlan1_id)
            exp_pkt = simple_tcp_packet(eth_dst=dmac2,
                                        eth_src=router_mac,
                                        ip_dst=ip_addr2,
                                        ip_src=ip_addr1,
                                        ip_id=105,
                                        ip_ttl=63,
                                        dl_vlan_enable=True,
                                        vlan_vid=vlan2_id)

            send_packet(self, 0, str(pkt))
            verify_packets(self, exp_pkt, [1])
            print "Success"

            print "Sending packet ({} -> {}) : Sub-port (port 2 : vlan {}) rif -> VLAN {} rif".format(ip_addr2, ip_addr1, vlan2_id, vlan1_id)
            pkt = simple_tcp_packet(eth_dst=router_mac,
                                    eth_src=dmac2,
                                    ip_dst=ip_addr1,
                                    ip_src=ip_addr2,
                                    ip_id=105,
                                    ip_ttl=64,
                                    dl_vlan_enable=True,
                                    vlan_vid=vlan2_id)
            exp_pkt = simple_tcp_packet(eth_dst=dmac1,
                                        eth_src=router_mac,
                                        ip_dst=ip_addr1,
                                        ip_src=ip_addr2,
                                        ip_id=105,
                                        ip_ttl=63,
                                        dl_vlan_enable=True,
                                        vlan_vid=vlan1_id)

            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0])
            print "Success"
            
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr2_subnet, ip_mask2, rif_id2)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac2)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan(vlan1_oid)
            self.client.sai_thrift_remove_vlan(vlan2_oid)
            self.client.sai_thrift_remove_virtual_router(vr_id)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)

@group('mtu')
class L3MtuTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        Description:
        Send L3 destined packet with max interface MTU and verify forwarding. With L3
        destined packet, verifying the forwarding behavior also ensures IP MTU value
        configured for the router interface. Increase the packet size beyond the
        configured MTU value and verify the behavior. Default action is to drop
        packets exceeding configured MTU

        Steps:
        Part 1:
           1. Create virtual router V1.
           2. Create two  router interface R1 and R2.
           3. Create next-hop and route.
           4. Set IP mtu 1500 for router interface R1 and R2.
           5. Send L3 IP packet of size 1500 bytes. (or packet size as per MTU size.) to port R2

        Part 2:
           6. Send Oversized packet (Packet size > MTU) to port R2.

        """
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''
        port_mtu = 1500

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)

        attr_mtu_value = sai_thrift_attribute_value_t(u32=port_mtu)
        attr_mtu = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_MTU, value=attr_mtu_value)
        self.client.sai_thrift_set_router_interface_attribute(rif_id1, attr_mtu)
        self.client.sai_thrift_set_router_interface_attribute(rif_id2, attr_mtu)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_addr1_subnet = '10.10.10.0'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)

        # send the test packet(s)
        pkt1 = simple_ip_packet(pktlen=1500,
                                eth_dst=router_mac,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)

        exp_pkt1 = simple_ip_packet(pktlen=1500,
                                eth_dst='00:11:22:33:44:55',
                                eth_src=router_mac,
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=63)

        pkt2 = simple_ip_packet(pktlen=1600,
                                eth_dst=router_mac,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)

        exp_pkt2 = simple_ip_packet(pktlen=1600,
                                eth_dst='00:11:22:33:44:55',
                                eth_src=router_mac,
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=63)
        try:
            send_packet(self, 1, str(pkt1))
            verify_packet(self, exp_pkt1, 0)
            # send ip packet to interface 1 to interface 0 (Packet size > MTU)
            send_packet(self, 1, str(pkt2))
            verify_no_packet(self, exp_pkt2, 0)
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)

            self.client.sai_thrift_remove_virtual_router(vr_id)


@group('l3')
class L3IPv4NeighborMacTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        Description:
        Create two VLAN router interfaces in the same VRF. Create nhop, route and
        neighbor entry for destination. Send packet on one router interface and
        verify packet received on the other router interface. Simulate a MAC address
        change for the neighbor entry and verify traffic. The packet must now be
        forwarded with the new MAC address as destination MAC

        Steps:
        1. Create VLAN 100, 200 in the database
        2. Associate 2 tagged ports (port 1, 2) as the member port of VLAN 100 and 200 respectively.
        3. Create Virtual router V1 and enable IPv4.
        4. Create two virtual router interfaces and set the interface type as VLAN
        5. Create IPv4 neighbor entry with MAC1 and asociate with ""RIF Id1"".
        6. Create next hop, route to reach the neighbor (MAC1).
        7. Send test packet from port 1 with src_mac = MAC1 address and dst_mac = router MAC.
        8. Send traffic on port 2 and observe traffic forwarding to port 1.
        9. Change the neighbor attribute to update with new MAC (MAC3)
        10. Send IPv4 packet from port 1 with src_mac = MAC1 address and ensure its dropped by DUT.
        11. Change the source MAC address (MAC3) and send the ARP packet from port 1 and learn FDB entry with MAC3.
        12. Send IP packet from port 2 and verify the traffic forwarded to port 1 with new MAC address (MAC3) as destination MAC.
        13. Remove the router interface and change the port attribute to default VLAN.
        14. Remove the vlan members and VLAN 100,200 from the database.
        """

        print "Sending packet port 1 -> switch (for learning purpuse)"
        print "Sending packet from port 2 (11.11.11.1) -> port 1 (192.168.0.1 through the router)"

        switch_init(self.client)
        v4_enabled = 1
        v6_enabled = 0
        vlan_id1 = 100
        vlan_id2 = 200
        port1 = port_list[0]
        port2 = port_list[1]
        mac_action = SAI_PACKET_ACTION_FORWARD

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_addr1_subnet = '10.10.10.0'
        ip_addr_subnet = '192.168.0.0'
        ip_mask1 = '255.255.255.0'
        mac_port1 = '00:0a:00:00:00:01'
        mac_port2 = '00:0a:00:00:00:02'
        mac_port3 = '00:0a:00:00:00:03'
        mac1 = ''
        mac2 = ''
        mac3 = ''

        vlan1 = sai_thrift_create_vlan(self.client, vlan_id1)
        vlan2 = sai_thrift_create_vlan(self.client, vlan_id2)

        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan1, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan2, port2, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value1 = sai_thrift_attribute_value_t(u16=vlan_id1)
        attr1 = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value1)
        self.client.sai_thrift_set_port_attribute(port1, attr1)

        attr_value2 = sai_thrift_attribute_value_t(u16=vlan_id2)
        attr2 = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value2)
        self.client.sai_thrift_set_port_attribute(port2, attr2)

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_vlan_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan1, v4_enabled, v6_enabled, mac1)
        rif_vlan_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan2, v4_enabled, v6_enabled, mac2)

        sai_thrift_create_neighbor(self.client, addr_family, rif_vlan_id1, ip_addr1, mac_port1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_vlan_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask1, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_vlan_id1)

        arp_req_pkt = simple_arp_packet(eth_dst='ff:ff:ff:ff:ff:ff',
                                        eth_src=mac_port1,
                                        vlan_vid=100,
                                        arp_op=1,     # ARP request
                                        ip_snd='10.10.10.1',
                                        ip_tgt='10.10.10.2',
                                        hw_snd=mac_port1,
                                        hw_tgt='00:00:00:00:00:00')


        print "Send ARP request packet from port1 ..."
        send_packet(self, 0, str(arp_req_pkt))

        try:

            #sending L3 packet from port 2 through router to port 1 that update the fdb with is MAC
            L3_pkt = simple_tcp_packet(pktlen=144,
                                       eth_dst=router_mac,
                                       eth_src=mac_port2,
                                       ip_src='11.11.11.1',
                                       ip_dst='192.168.0.1',
                                       dl_vlan_enable=True,
                                       vlan_vid=vlan_id2,
                                       ip_id=105,
                                       ip_ttl=64)

            exp_pkt = simple_tcp_packet(pktlen=144,
                                       eth_dst=mac_port1,
                                       eth_src=router_mac,
                                       ip_dst='192.168.0.1',
                                       ip_src='11.11.11.1',
                                       dl_vlan_enable=True,
                                       vlan_vid=vlan_id1,
                                       ip_id=105,
                                       ip_ttl=63)

            send_packet(self, 1, str(L3_pkt))

            verify_packets(self, exp_pkt, [0])

            # Change the neighbor attribute to update with new MAC
            sai_thrift_set_neighbor_attribute(self.client, addr_family, rif_vlan_id1, ip_addr1, mac_port3)

            print "Expect no packet with Dst MAC1 after neighbor attribute set to MAC3"
            send_packet(self, 1, str(L3_pkt))
            verify_no_packet(self, exp_pkt, 0)

            # learn mac MAC3
            arp_req_pkt2 = simple_arp_packet(eth_dst='ff:ff:ff:ff:ff:ff',
                                        eth_src=mac_port3,
                                        vlan_vid=vlan_id1,
                                        arp_op=1,     # ARP request
                                        ip_snd='10.10.10.1',
                                        ip_tgt='10.10.10.2',
                                        hw_snd=mac_port3,
                                        hw_tgt='00:00:00:00:00:00')

            send_packet(self, 0, str(arp_req_pkt2))

            L3_pkt1 = simple_tcp_packet(pktlen=104,
                                       eth_dst=router_mac,
                                       eth_src=mac_port2,
                                       ip_dst='192.168.0.1',
                                       ip_src='11.11.11.1',
                                       dl_vlan_enable=True,
                                       vlan_vid=vlan_id2,
                                       ip_id=105,
                                       ip_ttl=64)
            exp_pkt1 = simple_tcp_packet(pktlen=104,
                                       eth_dst=mac_port3,
                                       eth_src=router_mac,
                                       dl_vlan_enable=True,
                                       vlan_vid=vlan_id1,
                                       ip_dst='192.168.0.1',
                                       ip_src='11.11.11.1',
                                       ip_id=105,
                                       ip_ttl=63)

            send_packet(self, 1, str(L3_pkt1))
            verify_packets(self, exp_pkt1, [0])

        finally:
            sai_thrift_flush_fdb_by_vlan(self.client, vlan1)
            sai_thrift_flush_fdb_by_vlan(self.client, vlan2)

            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_vlan_id1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask1, nhop1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_vlan_id1, ip_addr1, mac_port3)

            self.client.sai_thrift_remove_router_interface(rif_vlan_id1)
            self.client.sai_thrift_remove_router_interface(rif_vlan_id2)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)

            self.client.sai_thrift_remove_vlan(vlan1)
            self.client.sai_thrift_remove_vlan(vlan2)

            self.client.sai_thrift_remove_virtual_router(vr_id)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)

@group('l3')
class L3IPv6NeighborMacTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        Description:
        Same as "L3IPv4NeighborMacTest" but for IPv6 neighbor

        Steps:
        1. Create VLAN 100, 200 in the database
        2. Associate 2 tagged ports (port 1, 2) as the member port of VLAN 100 and 200 respectively.
        3. Create Virtual router V1 and enable IPv6.
        4. Create two virtual router interfaces and set the interface type as VLAN
        5. Create IPv6 neighbor entry with MAC1 and asociate with ""RIF Id1"".
        6. Create next hop, route to reach the neighbor (MAC1).
        7. Send test packet from port 1 with src_mac = MAC1 address and dst_mac = router MAC.
        8. Send traffic on port 2 and observe traffic forwarding to port 1.
        9. Change the neighbor attribute to update with new MAC (MAC3)
        10. Send IPv6 packet from port 1 with src_mac = MAC1 address and ensure its dropped by DUT.
        11. Change the source MAC address (MAC3) and send the broadcast packet from port 1 and learn FDB entry with MAC3.
        12. Send IPv6 packet from port 2 and verify the traffic forwarded to port 1 with new MAC address (MAC3) as destination MAC.
        13. Remove the router interface and change the port attribute to default VLAN.
        14. Remove the vlan members and VLAN 100,200 from the database.
        """
        print
        print "Sending packet port 2 -> port 1 (2001:1111::1 -> 3001:1000::1) \n"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        v4_enabled = 0
        v6_enabled = 1
        vlan1_id = 100
        vlan2_id = 200

        addr_family = SAI_IP_ADDR_FAMILY_IPV6
        ip_addr1 = '2001:1000::1'
        ip_addr1_subnet = '2001:1000::0'
        ip_addr_subnet = '3001:1000::0'
        ip_mask1 = 'ffff:ffff:ffff:ffff:0000:0000:0000:0000'
        dmac1 = '00:0a:00:00:00:01'
        dmac2 = '00:0b:00:00:00:01'
        mac1 = ''
        dmac3 = '00:0c:00:00:00:01'

        vlan1_oid = sai_thrift_create_vlan(self.client, vlan1_id)
        vlan2_oid = sai_thrift_create_vlan(self.client, vlan2_id)

        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan1_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan2_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan1_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        attr_value1 = sai_thrift_attribute_value_t(u16=vlan2_id)
        attr1 = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value1)
        self.client.sai_thrift_set_port_attribute(port2, attr1)

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan1_oid, v4_enabled, v6_enabled, mac1)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan2_oid, v4_enabled, v6_enabled, mac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask1, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)

        try:

            test_pkt = simple_tcpv6_packet(eth_dst='ff:ff:ff:ff:ff:ff',
                                 eth_src=dmac1,
                                 ipv6_dst='2001:1000::2',
                                 ipv6_src='2001:1000::1',
                                 dl_vlan_enable=True,
                                 vlan_vid=100,
                                 ipv6_hlim=64)

            print "Send Broadcast packet from port1 to learn MAC1 using test packet..."
            print "Sending packet from port 2 -> port 1 (through the router) \n"
            send_packet(self, 0, str(test_pkt))

            time.sleep(1)

            pkt = simple_tcpv6_packet(
                                eth_dst=router_mac,
                                eth_src=dmac2,
                                ipv6_dst='3001:1000::1',
                                ipv6_src='2001:1111::1',
                                dl_vlan_enable=True,
                                vlan_vid=vlan2_id,
                                ipv6_hlim=64)

            exp_pkt = simple_tcpv6_packet(
                                eth_dst=dmac1,
                                eth_src=router_mac,
                                ipv6_dst='3001:1000::1',
                                ipv6_src='2001:1111::1',
                                dl_vlan_enable=True,
                                vlan_vid=vlan1_id,
                                ipv6_hlim=63)

            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0])
            # Update the neighbor MAC entry
            sai_thrift_set_neighbor_attribute(self.client, addr_family, rif_id1, ip_addr1, dmac3)

            print "Expect no packet with Dst MAC1 after neighbor attribute set to MAC3"
            send_packet(self, 1, str(pkt))
            verify_no_packet(self, exp_pkt, 0)

            # Construct test packet with new MAC
            test_pkt = simple_tcpv6_packet(eth_dst='ff:ff:ff:ff:ff:ff',
                                 eth_src=dmac3,
                                 ipv6_dst='2001:1000::2',
                                 ipv6_src='2001:1000::1',
                                 dl_vlan_enable=True,
                                 vlan_vid=100,
                                 ipv6_hlim=64)

            print "Send Broadcast packet from port1 to learn MAC3 using test packet..."

            send_packet(self, 0, str(test_pkt))
            time.sleep(1)
            exp_pkt1 = simple_tcpv6_packet(
                                eth_dst=dmac3,
                                eth_src=router_mac,
                                ipv6_dst='3001:1000::1',
                                ipv6_src='2001:1111::1',
                                dl_vlan_enable=True,
                                vlan_vid=vlan1_id,
                                ipv6_hlim=63)

            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt1, [0])

        finally:
            sai_thrift_flush_fdb_by_vlan(self.client, vlan1_oid)
            sai_thrift_flush_fdb_by_vlan(self.client, vlan2_oid)

            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask1, nhop1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac3)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)

            self.client.sai_thrift_remove_vlan(vlan1_oid)
            self.client.sai_thrift_remove_vlan(vlan2_oid)

            self.client.sai_thrift_remove_virtual_router(vr_id)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            
@group('l3')
class L3DirectedBroadcast (sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        Description:
        i) L3DirectedBroadcast I:
        Send IPv4 packet to a broadcast IP. The packet must have unicast
        destination MAC (Gateway MAC) and a broadcast destination IP (like 192.168.0.255).
        Verify that the packet is sent out on all member ports of VLAN if the outgoing
        interface is a VLAN RIF.

        ii) L3DirectedBroadcast II:
        After executing L3DirectedBroadcast I, add and remove ports to the VLAN.
        When a port is added, the L3 broadcast traffic must also be forwarded on
        the newly added member port and similar test to be executed by removing
        existing member port from VLAN.

        Steps:
        Part 1 : Directed Broadcast in Transit Switch:
        1. Create virtual router V1 and enable IPv4.
        2. Create two virtual router interfaces RIF Id1 and RIF Id2 and set the interface type as PORT.
        3. Create IPv4 neighbor entry (12.0.0.2) with MAC1 and associate with "RIF id 1".
        4. Create a nexthop (nhop), route to reach the 192.168.0.255 entry
        5. Send the IPv4 broadcast packet from port 4, and verify the packet received with unicast destination mac on port 1
        6. Clean up by remove route, nhop1, neighbor and RIF Id1 (router interface)

        Part 2 : Directed Broadcast for Penultimate Switches:
        1. Create VLAN 10 and associate 3 untagged ports (port 1,2,3) as the member port of VLAN 10.
        2. Set the Ports attribute value of "Port VLAN ID 10" for the port 1, 2, 3
        3. Create VLAN virtual interfaces "RIF Id1" and set the interface type as VLAN
        4. Create IPv4 neighbor entry with IP (192.168.0.255) and dmac1(ff:ff:ff:ff:ff:ff) and asociate with "RIF Id1".
        5. Send the IPv4 broadcast packet from port 4, and verify the packet received on all the VLAN member ports (1,2,3)
           with broadcast destination mac and broadcast destination IP.

        Part 3 to verify L3DirectedBroadcast II sequence:
        6. Remove the port 2 from VLAN 10 and send IPv4 broadcast packet, and verify it received on remaining ports (1,3).
        7. Re-add the port 2 to VLAN 10 and again send IPv4 broadcast packet, and verify it received all the VLAN member ports (1,2,3).
        8. Clean up by remove the neighbor, router interface and VLAN member ports.
        9. Associate all the ports to default VLAN.
        """

        print "\n"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        v4_enabled = 1
        v6_enabled = 0
        vlan_id = 10
        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '192.168.0.255'
        ip_addr2 = '12.0.0.2'
        dmac1 = 'ff:ff:ff:ff:ff:ff'
        dmac2 = '00:0a:00:00:00:01'
        ip_mask1 = '255.255.255.0'
        ip_mask2 = '255.255.255.255'
        mac_action = SAI_PACKET_ACTION_FORWARD
        mac = ''
        penultimate_conf = False

        ### Part 1 - Transit Switch Configuration
        # create virtual router
        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        # create router interface
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port4, 0, v4_enabled, v6_enabled, mac)

        # create neighbor and nhop
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr2, dmac2)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr2, rif_id1)

        # Create route for the broadcast address with unicast nexthop
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask2, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr2, ip_mask2, rif_id1)

        time.sleep(1)
        try:
            # send the directed broadcat packet(s)
            pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src='00:0b:00:00:00:01',
                                ip_dst='192.168.0.255',
                                ip_src='10.10.10.1',
                                ip_id=105,
                                ip_ttl=64)
            exp_pkt = simple_tcp_packet(
                                eth_src=router_mac,
                                eth_dst='00:0a:00:00:00:01',
                                ip_dst='192.168.0.255',
                                ip_src='10.10.10.1',
                                ip_id=105,
                                ip_ttl=63)
            # Verifying the traffic @ Port1
            print "Send packet from port 4 -> port 1 (10.10.10.1 -> 192.168.0.255) through transit configuration"
            send_packet(self, 3, str(pkt))
            verify_packets(self, exp_pkt, [0])

            ### Cleanup transit configuration
            # Remove route having nhop1, nhop1 and neighbor
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask2, nhop1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr2, ip_mask2, rif_id1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr2, dmac2)
            # remove router interface
            self.client.sai_thrift_remove_router_interface(rif_id1)

            # Part 2 - Penultimate Configuration 
            # create vlan
            penultimate_conf = True

            vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)

            # Add port 2, 3 as a member of VLAN 10
            vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan_member3 = sai_thrift_create_vlan_member(self.client, vlan_oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)

            # Assign PVLAN ID with ports 1,2 and 3
            attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)

            rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan_oid, v4_enabled, v6_enabled, mac)

            # Create neighbor for the target subnet with broadcast IP when penultimate switch
            sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask2, rif_id1)

            exp_pkt1 = simple_tcp_packet(
                                eth_src=router_mac,
                                eth_dst='ff:ff:ff:ff:ff:ff',
                                ip_dst='192.168.0.255',
                                ip_src='10.10.10.1',
                                ip_id=105,
                                ip_ttl=63)

            # Sending the traffic from Port4
            print "Send packet from 10.10.10.1 -> 192.168.0.255 (directed broadcast IP) through penultimate configuration"
            send_packet(self, 3, str(pkt))
            # Verifying the traffic @ Port1,Port2 & Port3
            verify_packets(self, exp_pkt1, [0, 1, 2])

            ### L3DirectedBroadcast_II test case steps
            # remove vlan membership from Port2
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            time.sleep(2)

            # Sending the traffic from Port4
            send_packet(self, 3, str(pkt))
            # Verify traffic @ Port1 and Port3
            verify_packets(self, exp_pkt1, [0, 2])

            # Re-add vlan membership from Port2
            vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            time.sleep(2)

            # Sending the traffic from Port4
            send_packet(self, 3, str(pkt))
            # Verifying the traffic @ Port1,Port2 & Port3
            verify_packets(self, exp_pkt1, [0, 1, 2])

        finally:

            if penultimate_conf:
                # remove neighbor
                sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask2, rif_id1)
                sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
                # remove vlan membership ports
                self.client.sai_thrift_remove_vlan_member(vlan_member1)
                self.client.sai_thrift_remove_vlan_member(vlan_member2)
                self.client.sai_thrift_remove_vlan_member(vlan_member3)
                self.client.sai_thrift_set_port_attribute(port2, attr)
                self.client.sai_thrift_set_port_attribute(port3, attr)

                # remove vlan
                self.client.sai_thrift_remove_vlan(vlan_oid)
                # Assign ports into default vlan
                attr_value = sai_thrift_attribute_value_t(u16=1)
                attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
                self.client.sai_thrift_set_port_attribute(port1, attr)
                self.client.sai_thrift_set_port_attribute(port2, attr)
                self.client.sai_thrift_set_port_attribute(port3, attr)

            # remove router interface
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)

            ### remove virtual router
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('l3')
class L3IPv4NeighborFdbAgeoutTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        Description:
        Create two VLAN router interfaces in same VRF. Create nhop and route for
        destination. Learn ARP for the nhop and check FDB table for the MAC learnt from
        the ARP packet in that VLAN. Simulate FDB age-out by clearing the FDB entry or
        wait for age-out time. After the FDB entry is cleared, the layer 3 traffic via
        this nhop must take the action as specified by SAI switch attribute
        SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION
        Steps:
        1. Create VLAN 100, 200 in the database
        2. Associate 2 untagged ports (port 1, 2) as the member port of VLAN 100 and 200 respectively.
        3. Set port attribute values of "Port VLAN ID 100, 200" for the ports 1 and 2 respectively.
        4. Create Virtual router V1 and enable IPv4.
        5. Create two virtual router interfaces and set the interface type as "PORT".
        6. Send ARP packet from port 1 and learn the MAC1 in VLAN 100.
        7. Create IPv4 neighbor entry with MAC1 and asociate with "RIF Id1".
        8. Create next hop, route to reach the neighbor (MAC1).
        9. Send traffic on port 2 and observe traffic forwarding to port 1.
        10. Change the port attribute of port 1 to SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION
            with value SAI_PACKET_ACTION_DROP
        Part1:
        11. Flush the FDB table entry and send the packet.
        12. Observe the SAI switch take the action according to the switch attribute.
        Part2:
        13. Send ARP packet from port 1 to learn the MAC entry in the FDB table.
        14. Wait for the FDB entry age out time and resend the traffic from Port 2
        15. Observe the SAI switch take the action according to the switch attribute.
        16. Restore the Switch attribute to FORWARD and remove the router interface and
            change the port attribute to default VLAN.
        """

        print "\nSending packet port 1 -> port 2 (11.11.11.1 -> 10.10.10.1 [id = 101])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 0
        vlan1_id = 100
        vlan2_id = 200
        mac_action = SAI_PACKET_ACTION_FORWARD
        fdb_default_aging_time = 0

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_addr1_subnet = '10.10.10.0'
        ip_addr_subnet = '192.168.0.0'
        ip_mask = '255.255.255.0'
        dmac1 = '00:0a:0a:0a:00:01'
        dmac2 = '00:0b:0b:0b:00:01'
        fdb_aging_time = 10
        mac = ''

        vlan1_oid = sai_thrift_create_vlan(self.client, vlan1_id)
        vlan2_oid = sai_thrift_create_vlan(self.client, vlan2_id)

        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan1_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan2_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan1_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        attr_value1 = sai_thrift_attribute_value_t(u16=vlan2_id)
        attr1 = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value1)
        self.client.sai_thrift_set_port_attribute(port2, attr1)

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan1_oid, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan2_oid, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask, rif_id1)

        arp_req_pkt = simple_arp_packet(eth_dst='ff:ff:ff:ff:ff:ff',
                                        eth_src=dmac1,
                                        vlan_vid=100,
                                        arp_op=1,     # ARP request
                                        ip_snd='10.10.10.1',
                                        ip_tgt='10.10.10.2',
                                        hw_snd=dmac1,
                                        hw_tgt='00:00:00:00:00:00')

        print "Send ARP request packet from port1 for mac learning and then send test packet..."
        send_packet(self, 0, str(arp_req_pkt))

        # Wait for the MAC learning
        time.sleep(1)

        try:
            # Send IP packet from port 2 to 1 that have FDB table update with MAC1 of nhop1
            pkt = simple_tcp_packet(pktlen=144,
                                eth_dst=router_mac,
                                eth_src=dmac2,
                                ip_src='11.11.11.1',
                                ip_dst='192.168.0.1',
                                dl_vlan_enable=True,
                                vlan_vid=200,
                                ip_id=105,
                                ip_ttl=64)

            exp_pkt = simple_tcp_packet(pktlen=144,
                                eth_dst=dmac1,
                                eth_src=router_mac,
                                ip_dst='192.168.0.1',
                                ip_src='11.11.11.1',
                                dl_vlan_enable=True,
                                vlan_vid=100,
                                ip_id=105,
                                ip_ttl=63)

            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0])
            print "Test unicast packet with unicast miss action=DROP"
            attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_DROP)
            attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)

            # Flush FDB entry learned on the Router interface Port 1
            self.client.sai_thrift_flush_fdb_entries(thrift_attr_list=[])
            time.sleep(1)

            send_packet(self, 1, str(pkt))
            verify_no_packet(self, exp_pkt, 1)

            # Again learn FDB entry by send ARP request packet from port 1
            send_packet(self, 0, str(arp_req_pkt))

            time.sleep(1)
            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0])

            attr_value = sai_thrift_attribute_value_t(u32=fdb_aging_time)
            attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_AGING_TIME, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)

            # Wait for the aging timer (2 times of fdb age out) to expiry the FDB entry.
            time.sleep(fdb_aging_time * 2 + 1)

            send_packet(self, 1, str(pkt))
            verify_no_packet(self, exp_pkt, 1)

        finally:
            sai_thrift_flush_fdb_by_vlan(self.client, vlan1_oid)
            sai_thrift_flush_fdb_by_vlan(self.client, vlan2_oid)

            # Restore the unicast miss action=FORWARD'
            attr_value = sai_thrift_attribute_value_t(s32=mac_action)
            attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)

            # Restore the FDB age out to default value to zero
            attr_value = sai_thrift_attribute_value_t(u32=fdb_default_aging_time)
            attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_AGING_TIME, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)

            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask, rif_id1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask, nhop1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan1_oid)
            self.client.sai_thrift_remove_vlan(vlan2_oid)

            self.client.sai_thrift_remove_virtual_router(vr_id)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)

@group('l3')
class L3IPv6NeighborFdbAgeoutTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        Description:
        Same as "L3IPv4NeighborFdbAgeoutTest" for IPv6 neighbor by clearing/deleting
        the learnt FDB entry for the IPv6 neighbor
        Steps:
        1. Create VLAN 100, 200 in the database
        2. Associate 2 untagged ports (port 1, 2) as the member port of VLAN 100 and 200 respectively.
        3. Set port attribute values of "Port VLAN ID 100, 200" for the ports 1 and 2 respectively.
        4. Create Virtual router V1 and enable IPv6.
        5. Create two virtual router interfaces and set the interface type as "PORT".
        6. Send Broadcast packet from port 1 and verify the FDB entry for the MAC1 in VLAN 100.
        7. Create IPv6 neighbor entry with MAC1 and asociate with ""RIF Id1"".
        8. Create next hop, route to reach the neighbor (MAC1).
        9. Send traffic on port 2 and observe traffic forwarding to port 1.
        10. Change the port attribute of port 1 to SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION
            with value SAI_PACKET_ACTION_DROP
        Part1:
        11. Flush the FDB table entry and send the packet
        12. Observer the SAI switch take the action according to the switch attribute.
        Part2:
        13. Send ARP packet from port 1 to learn the MAC entry in the FDB table.
        14. Wait for the FDB entry age out time and resend the traffic from Port 2
        15. Observer the SAI switch take the action according to the switch attribute.
        16. Restore the Switch attribute to FORWARD and remove the router interface and
            change the port attribute to default VLAN.
        """
        print
        print "Sending packet port 1 -> port 2 (2001:1111::1 -> 2001:1000::1 [id = 101])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 0
        v6_enabled = 1
        vlan1_id = 100
        vlan2_id = 200
        mac_action = SAI_PACKET_ACTION_FORWARD

        addr_family = SAI_IP_ADDR_FAMILY_IPV6
        ip_addr1 = '2001:1000::1'
        ip_addr1_subnet = '2001:1000::0'
        ip_addr_subnet = '3001:1000::0'
        ip_mask = 'ffff:ffff:ffff:ffff:0000:0000:0000:0000'

        ip_addr2 = '2001:1111::1'
        ip_addr2_subnet = '2001:1111::0'
        ip_mask2 = 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:0000'
        dmac1 = '00:0a:0a:0a:00:01'
        dmac2 = '00:0b:0b:0b:00:01'
        mac = ''
        fdb_aging_time = 15
        fdb_default_aging_time = 0

        vlan1_oid = sai_thrift_create_vlan(self.client, vlan1_id)
        vlan2_oid = sai_thrift_create_vlan(self.client, vlan2_id)

        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan1_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan2_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan1_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        attr_value1 = sai_thrift_attribute_value_t(u16=vlan2_id)
        attr1 = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value1)
        self.client.sai_thrift_set_port_attribute(port2, attr1)

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan1_oid, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan2_oid, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask, rif_id1)
        pkt1 = simple_tcpv6_packet(pktlen=104,
                                 eth_dst='ff:ff:ff:ff:ff:ff',
                                 eth_src=dmac1,
                                 ipv6_dst='2001:1000::2',
                                 ipv6_src='2001:1000::1',
                                 dl_vlan_enable=True,
                                 vlan_vid=100,
                                 ipv6_hlim=64)

        try:
            print "Send Broadcast packet from port1  for mac leanring and then send  test packet..."
            send_packet(self, 0, str(pkt1))
            # Wait for the MAC learning
            time.sleep(1)

            # Send IP packet from port 2 to 1 that have FDB table update with MAC1 of nhop1

            pkt = simple_tcpv6_packet(pktlen=104,
                                eth_dst=router_mac,
                                eth_src=dmac2,
                                ipv6_src='2001:1111::1',
                                ipv6_dst='3001:1000::1',
                                dl_vlan_enable=True,
                                vlan_vid=200,
                                ipv6_hlim=64)

            exp_pkt = simple_tcpv6_packet(pktlen=104,
                                eth_dst=dmac1,
                                eth_src=router_mac,
                                ipv6_dst='3001:1000::1',
                                ipv6_src='2001:1111::1',
                                dl_vlan_enable=True,
                                vlan_vid=100,
                                ipv6_hlim=63)

            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0])
            print "Test unicast packet with unicast miss action=DROP"
            attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_DROP)
            attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)

            # Flush FDB entry learned on the Router interface Port 1
            self.client.sai_thrift_flush_fdb_entries(thrift_attr_list=[])
            time.sleep(1)
            send_packet(self, 1, str(pkt))
            verify_no_other_packets(self)

            # Again learn FDB entry by send broadcast packet from port 1
            send_packet(self, 0, str(pkt1))

            time.sleep(2)
            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0])

            attr_value = sai_thrift_attribute_value_t(u32=fdb_aging_time)
            attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_AGING_TIME, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)

            # Wait for the aging timer (2 times of the FDB ageout value) to expiry the FDB entry.
            time.sleep(fdb_aging_time * 2 + 1)

            send_packet(self, 1, str(pkt))
            #verify_no_other_packets(self)
            verify_no_packet(self, exp_pkt, 0)

        finally:
            sai_thrift_flush_fdb_by_vlan(self.client, vlan1_oid)
            sai_thrift_flush_fdb_by_vlan(self.client, vlan2_oid)

            # Restore the unicast miss action=FORWARD'
            attr_value = sai_thrift_attribute_value_t(s32=mac_action)
            attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)

            # Restore the FDB age out to default value to zero
            attr_value = sai_thrift_attribute_value_t(u32=fdb_default_aging_time)
            attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_AGING_TIME, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)

            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask, rif_id1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask, nhop1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac2)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)

            self.client.sai_thrift_remove_vlan(vlan1_oid)
            self.client.sai_thrift_remove_vlan(vlan2_oid)

            self.client.sai_thrift_remove_virtual_router(vr_id)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)


@group('l3')
@group('ecmp')
class L3IPv4EcmpGroupMemberTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        '''
        Description:
        i) L3IPv4EcmpGroupMemberTest I:
        Create a VRF with IPv4 and IPv6 enabled. Create three router interfaces in
        the same VRF. Create a route (24 mask) with nhop and neighbor entry in two
        router interfaces. Send 10,000 streams with varying 5 tuple combinations to
        the destination IP on one port and verify distribution on the two router
        interfaces for which the nhops are present. Create another router interface
        and add nhop and neighbor entry over this interface to the route configured above.
        NH group object shall be updated and verify traffic distribution to the newly added nhop.

        ii) L3IPv4EcmpGroupMemberTest II:
        After executing L3IPv4EcmpGroupMemberTest  I, remove the nhop entry on router interface 1
        from the route while traffic flow is still active. Verify the traffic is not forwarded to
        removed nhop and distribution happens on the remaining set of nhops.

        iii) L3IPv4EcmpGroupMemberTest III (Zero member):
        Same as L3IPv4EcmpGroupMemberTest but the ECMP group object to which the route is
        pointing has zero members. Verify that traffic is dropped

        Steps:
        1. Create Virtual router V1 and enable IPv4 and IPv6.
        2. Create three virtual router interfaces and set the interface type as "PORT" for ports 1, 2, 3.
        3. Create IPv4 neighbor entry (192.168.0.1, 192.168.1.1) with MAC1, MAC2 and associate with "RIF ids (id1 to 2)".
        4. Create two next hops (nhop1 & nhop2) and associate with RIFs (Id 1 & 2) respectively.
        5. Create next-hop group "nhop_group1".
        6. Create group members (nhop_gmember1, nhop_gmember2) for "nhop_group1" ,associated with  next-hops (nhop1 & nho2) respectively.
        7. Create route entry with /24 mask (i.e 10.10.10.1/255.255.255.0) through "nhop_group1".

        Part1:
        1. Send 10,000 streams from port 3 with destination IP (10.10.10.1/24) and varying src_mac,src_ip,dst_ip, sport
        and dport ,Observe the traffic distribution on all the two router interface, which have next hop present.

        Part2:
        1. Create another router interface and interface type as PORT.
        2. Create new nhop and neighbor entry (192.168.2.1) over the router interface (port 3) towards the ECMP route (i.e 10.10.10.1/24).

        Clean up by remove the nhop, neighbor, route and the router interface.
        '''

        print "Sending packet port 3 -> port 1 ,2 (192.168.12.1  -> 10.10.10.1 [id = 106])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        v4_enabled = 1
        v6_enabled = 0
        mac = ''

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '192.168.0.1'
        ip_addr2 = '192.168.1.1'
        ip_addr3 = '192.168.2.1'
        ip_addr4 = '10.10.10.0'
        ip_addr1_subnet = '192.168.0.0'
        ip_addr2_subnet = '192.168.1.0'
        ip_addr3_subnet = '192.168.2.0'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'
        dmac3 = '00:11:22:33:44:57'

        vr1 = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif1 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif2 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)
        rif3 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family, rif1, ip_addr1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif2, ip_addr2, dmac2)

        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, ip_addr2, rif2)

        nhop_group1 = sai_thrift_create_next_hop_group(self.client)

        nhop_gmember1 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop1)
        nhop_gmember2 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop2)

        sai_thrift_create_route(self.client, vr1, addr_family, ip_addr4, ip_mask1, nhop_group1)
        # send the test packet(s)
        try:
            max_itrs = 10001
            ports = [0,1]
            member = 2
            var_gmember3 = False
            print "sending %s packets to verify distribution on 2 ecmp group members" %(max_itrs - 1)
            self.verifyEcmpPktDistribution(max_itrs,ports,member)

            #  Create another router interface and interface type as PORT.
            rif4 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port4, 0, v4_enabled, v6_enabled, mac)

            #  Create new nhop and neighbor entry (192.168.2.1) over the router interface (port 4) towards the ECMP route (i.e 10.10.10.1/24).
            sai_thrift_create_neighbor(self.client, addr_family, rif4, ip_addr3, dmac3)
            nhop3 = sai_thrift_create_nhop(self.client, addr_family, ip_addr3, rif4)

            nhop_gmember3 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop3)
            sai_thrift_create_route(self.client, vr1, addr_family, ip_addr4, ip_mask1, nhop_group1)

            var_gmember3 = True
            max_itrs = 101
            ports = [0,1,3]
            member=3
            print "sending %s packets to verify distribution on 3 ports after adding one more ecmp group member" %(max_itrs - 1)
            self.verifyEcmpPktDistribution(max_itrs,ports,member)

            # L3IPv4EcmpGroupMemberTest_II test case
            # Remove the nhop bound with router interface 1, and verify the traffic flows towards the ECMP route.
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember3)
            time.sleep(2)
            max_itrs = 101
            ports = [0,1]
            member=2

            print "sending %s packets to verify distribution on 2 ports after removing one ecmp group member" %(max_itrs - 1 )
            self.verifyEcmpPktDistribution(max_itrs,ports,member)

            # Ecmp - iii
            # Remove next hop group members

            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember1)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember2)
            time.sleep(2)

            # Send traffic from port 3 with destination IP (10.10.10.1/24) and observe the traffic should drop
            max_itrs = 101
            ports = [1,2]
            member=0
            self.verifyEcmpPktDistribution(max_itrs,ports,member)

        finally:
            sai_thrift_remove_route(self.client, vr1, addr_family, ip_addr4, ip_mask1, nhop_group1)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember1)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember2)
            if var_gmember3:
                self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember3)
                self.client.sai_thrift_remove_next_hop(nhop3)
                sai_thrift_remove_neighbor(self.client, addr_family, rif2, ip_addr2, dmac3)
                self.client.sai_thrift_remove_next_hop_group(nhop_group1)
                self.client.sai_thrift_remove_router_interface(rif4)

            self.client.sai_thrift_remove_next_hop(nhop1)
            self.client.sai_thrift_remove_next_hop(nhop2)

            sai_thrift_remove_neighbor(self.client, addr_family, rif1, ip_addr1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif2, ip_addr2, dmac2)

            self.client.sai_thrift_remove_router_interface(rif1)
            self.client.sai_thrift_remove_router_interface(rif2)
            self.client.sai_thrift_remove_router_interface(rif3)

            self.client.sai_thrift_remove_virtual_router(vr1)

    def verifyEcmpPktDistribution(self,max_itrs,ports,member):
        src_mac_start = '00:22:22:22:22:'
        ip_src_start = '192.168.12.'
        ip_dst_start = '10.10.10.'
        dport = 0x80
        sport = 0x1234
        eth_dst_ip1 = '00:11:22:33:44:55'
        eth_dst_ip2 = '00:11:22:33:44:56'
        eth_dst_ip3 = '00:11:22:33:44:57'
        if member==2:
            count=[0,0]
        elif(member==3):
            count=[0,0,0]

        for i in range(0, max_itrs):
            src_mac = src_mac_start + str(i % 99).zfill(2)
            ip_src = ip_src_start + str(i % 99).zfill(3)
            ip_dst = ip_dst_start + str(i % 99).zfill(3)
            pkt = simple_tcp_packet(
                             eth_dst=router_mac,
                             eth_src=src_mac,
                             ip_dst=ip_dst,
                             ip_src=ip_src,
                             tcp_sport=sport,
                             tcp_dport=dport,
                             ip_id=106,
                             ip_ttl=64)
            exp_pkt1 = simple_tcp_packet(
                             eth_dst=eth_dst_ip1,
                             eth_src=router_mac,
                             ip_dst=ip_dst,
                             ip_src=ip_src,
                             tcp_sport=sport,
                             tcp_dport=dport,
                             ip_id=106,
                             ip_ttl=63)
            exp_pkt2 = simple_tcp_packet(
                             eth_dst=eth_dst_ip2,
                             eth_src=router_mac,
                             ip_dst=ip_dst,
                             ip_src=ip_src,
                             tcp_sport=sport,
                             tcp_dport=dport,
                             ip_id=106,
                             ip_ttl=63)
            exp_pkt3 = simple_tcp_packet(
                             eth_dst=eth_dst_ip3,
                             eth_src=router_mac,
                             ip_dst=ip_dst,
                             ip_src=ip_src,
                             tcp_sport=sport,
                             tcp_dport=dport,
                             ip_id=106,
                             ip_ttl=63)

            send_packet(self, 2, str(pkt))
            if member==0:
                verify_no_other_packets(self)
            elif member==2:
                rcv_idx = verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2],ports)
                count[rcv_idx] += 1
            elif member==3:
                rcv_idx = verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2, exp_pkt3],ports)
                count[rcv_idx] += 1
            sport += 1
            dport += 1
        if member!=0:
            for i in range(0,member):
                print "Packet received on port %s  >> %s" % (i, count[i])
                self.assertTrue((count[i] >= ((max_itrs / member) * 0.8)),
                                      "Not all paths are equally balanced, %s" % count)
            print "All paths are  balanced with %s members" % member




@group('l3')
@group('ecmp')
class L3IPv6EcmpGroupMemberTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        '''
        Description:
        i) L3IPv6EcmpGroupMemberTest I:
        Same as L3IPv4EcmpGroupMemberTest I for IPv6 destination

        ii) L3IPv6EcmpGroupMemberTest II:
        Same as L3IPv4EcmpGroupMemberTest II for IPv6 destination

        Steps:
        1. Create Virtual router V1 and enable IPv4 and IPv6.
        2. Create three virtual router interfaces and set the interface type as "PORT" for ports 1, 2, 3.
        3. Create IPv6 neighbor entry (2001:2001::1, 2002:2002::1) with MAC1, MAC2 and associate with "RIF ids (id1 to 2)".
        4. Create two next hops (nhop1 & nhop2) and associate with RIFs (Id 1 & 2) respectively.
        5. Create next-hop group "nhop_group1".
        6. Create group members (nhop_gmember1, nhop_gmember2) for "nhop_group1" ,associated with  next-hops (nhop1 & nho2) respectively.
        7. Create route entry with /64 mask (i.e 3000:1000::1/64) through "nhop_group1".

        Part1:
        1. Send 10,000 streams from port 3 with destination IP (3000:1000::1/64) and observe the
        traffic distribution on all the two router interface, which have next hop present.

        Part2:
        1. Create another router interface and interface type as PORT.
        2. Create new nhop and neighbor entry (2003:2003::1) over the router interface (port 3) towards the ECMP route (i.e 3000:1000::1/64).
        3. Observe the nexthop entry should be updated and traffic should distribute through the newly added nhop (2003:2003::1).

        Part1 (L3IPv6EcmpGroupMemberTest II) :
        1. Send traffic from port 3 with destination IP (3000:1000::1/64) and varying src_mac, sport,
        src_ip and observe the traffic distribution on all the three router interface, which have next hop present.
        2. Remove the nhop bound with router interface 1, while the traffic flows towards the ECMP route.
        3. Observe the traffic is not distributed over the removed nexthop (2001:2001::1) interface.

        Clean up by remove the nhop, neighbor, route and the router interface.
        '''

        print "Sending packet port 3 -> port 1 ,2 (2010:2010::1  -> 3000:1000::1 [id = 101])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        v4_enabled = 0
        v6_enabled = 1
        mac = ''
        addr_family = SAI_IP_ADDR_FAMILY_IPV6
        ip_addr1 = '2001:2001::1'
        ip_addr2 = '2002:2002::1'
        ip_addr3 = '2003:2003::1'

        ip_addr1_subnet = '2001:2001::0'
        ip_addr2_subnet = '2002:2002::0'
        ip_addr3_subnet = '2003:2003::0'
        ip_addr4_subnet = '3000:1000::0'

        ip_mask1 = 'ffff:ffff:ffff:ffff:0000:0000:0000:0000'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'
        dmac3 = '00:11:22:33:44:57'

        vr1 = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif1 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif2 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)
        rif3 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family, rif1, ip_addr1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif2, ip_addr2, dmac2)

        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, ip_addr2, rif2)

        nhop_group1 = sai_thrift_create_next_hop_group(self.client)

        nhop_gmember1 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop1)
        nhop_gmember2 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop2)

        sai_thrift_create_route(self.client, vr1, addr_family, ip_addr4_subnet, ip_mask1, nhop_group1)

        # send the test packet(s)

        try:
            max_itrs = 10001
            dport = 0x80
            sport = 0x1234
            ports = [0,1]
            member=2
            var_gmember3 = False
            print "sending %s packets to verify distribution on 2 ecmp group members" %(max_itrs - 1)
            self.verifyEcmpPktDistributionIpv6(max_itrs,ports,member)

            rif4 = sai_thrift_create_router_interface(self.client, vr1, SAI_ROUTER_INTERFACE_TYPE_PORT, port4, 0, v4_enabled, v6_enabled, mac)
            sai_thrift_create_neighbor(self.client, addr_family, rif4, ip_addr3, dmac3)
            nhop3 = sai_thrift_create_nhop(self.client, addr_family, ip_addr3, rif4)
            nhop_gmember3 = sai_thrift_create_next_hop_group_member(self.client, nhop_group1, nhop3)
            sai_thrift_create_route(self.client, vr1, addr_family, ip_addr4_subnet, ip_mask1, nhop_group1)

            var_gmember3 = True
            max_itrs = 121
            ports = [0,1,3]
            member=3
            print "sending %s packets to verify distribution on 3 ports after adding one more ecmp group member" %(max_itrs - 1)
            self.verifyEcmpPktDistributionIpv6(max_itrs,ports,member)

            # Remove the nhop bound with router interface 1, and verify the traffic flows towards the ECMP route.
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember3)
            time.sleep(5)
            # Reset the count value after removal of the nhop member
            max_itrs = 101
            ports = [0,1]
            member=2
            print "sending %s packets to verify distribution on 2 ports after removing one ecmp group member" %(max_itrs - 1 )
            self.verifyEcmpPktDistributionIpv6(max_itrs,ports,member)

        finally:
            sai_thrift_remove_route(self.client, vr1, addr_family, ip_addr4_subnet, ip_mask1, nhop_group1)
            self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember2)
            self.client.sai_thrift_remove_next_hop_group(nhop_group1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            self.client.sai_thrift_remove_next_hop(nhop2)

            sai_thrift_remove_neighbor(self.client, addr_family, rif1, ip_addr1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif2, ip_addr2, dmac2)
            self.client.sai_thrift_remove_router_interface(rif1)
            self.client.sai_thrift_remove_router_interface(rif2)
            self.client.sai_thrift_remove_router_interface(rif3)
            if var_gmember3:
                self.client.sai_thrift_remove_next_hop_group_member(nhop_gmember3)
                self.client.sai_thrift_remove_next_hop(nhop3)
                sai_thrift_remove_neighbor(self.client, addr_family, rif2, ip_addr2, dmac3)
                self.client.sai_thrift_remove_router_interface(rif4)

            self.client.sai_thrift_remove_virtual_router(vr1)

    def verifyEcmpPktDistributionIpv6(self,max_itrs,ports,member):
        src_mac_start = '00:22:22:22:22:'
        ipv6_src_start = '2010:2010::'
        ipv6_dst_start = '3000:1000::'
        dport = 0x80
        sport = 0x1234
        eth_dst_ip1 = '00:11:22:33:44:55'
        eth_dst_ip2 = '00:11:22:33:44:56'
        eth_dst_ip3 = '00:11:22:33:44:57'
        if member==2:
            count=[0,0]
        elif(member==3):
            count=[0,0,0]

        for i in range(0, max_itrs):
            src_mac = src_mac_start + str(i % 99).zfill(2)
            ipv6_src = ipv6_src_start + str(i % 99).zfill(3)
            ipv6_dst = ipv6_dst_start + str(i % 99).zfill(3)
            pkt = simple_tcpv6_packet(
                              eth_dst=router_mac,
                              eth_src=src_mac,
                              ipv6_dst=ipv6_dst,
                              ipv6_src=ipv6_src,
                              dl_vlan_enable=False,
                              tcp_sport=sport,
                              tcp_dport=dport,
                              ipv6_hlim=64)
            exp_pkt1 = simple_tcpv6_packet(
                              eth_dst=eth_dst_ip1,
                              eth_src=router_mac,
                              ipv6_dst=ipv6_dst,
                              ipv6_src=ipv6_src,
                              dl_vlan_enable=False,
                              tcp_sport=sport,
                              tcp_dport=dport,
                              ipv6_hlim=63)
            exp_pkt2 = simple_tcpv6_packet(
                              eth_dst=eth_dst_ip2,
                              eth_src=router_mac,
                              ipv6_dst=ipv6_dst,
                              ipv6_src=ipv6_src,
                              dl_vlan_enable=False,
                              tcp_sport=sport,
                              tcp_dport=dport,
                              ipv6_hlim=63)
            exp_pkt3 = simple_tcpv6_packet(
                              eth_dst=eth_dst_ip3,
                              eth_src=router_mac,
                              ipv6_dst=ipv6_dst,
                              ipv6_src=ipv6_src,
                              dl_vlan_enable=False,
                              tcp_sport=sport,
                              tcp_dport=dport,
                              ipv6_hlim=63)

            send_packet(self, 2, str(pkt))
            if member==0:
                verify_no_other_packets(self)
            elif member==2:
                rcv_idx = verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2],ports)
                count[rcv_idx] += 1
            elif member==3:
                rcv_idx = verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2, exp_pkt3],ports)
                count[rcv_idx] += 1
            sport += 1
            dport += 1
        if member!=0:
            for i in range(0,member):
                print "Packet received on port %s  >> %s" % (i, count[i])
                self.assertTrue((count[i] >= ((max_itrs / member) * 0.8)),
                                      "Not all paths are equally balanced, %s" % count)
            print "All paths are  balanced with %s members" % member

@group('l3')
class L3IPv4_32Test (sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        '''
        Description:
        i) Add a route entry with /32 prefix (e.g. 10.1.1.1/32 via NH1) and a neighbor
        entry for the same IP address (e.g. 10.1.1.1 MAC1 Port1). Send traffic and verify that
        the packet takes the path specified by the /32 route entry and not the neighbor path.
        ii) After executing "L3IPv4/32Test I", delete the route entry with /32.
        Ensure the packet must be now be forwarded using the neighbor/host entry.
        iii) After executing "L3IPv4/32Test  II", readd the same route entry with /32.
        Ensure that the packet now take the path as specified by the route and not by the
        previously programmed neighbor entry path.

        Steps:
        1. Create a Virtual router V1 and enable V4.
        2. Create two router interfaces "RIF R1, R2, R3" and set the interface type as SAI_ROUTER_INTERFACE_TYPE_PORT.
        3. Create route entry with /32 prefix (i.e 10.1.1.1/255.255.255.255) through NH1
        4. Create IPv4 neighbor entry (NH1  10.1.1.1) with MAC1 and associate with RIF R1
        5. Send traffic traffic from Port3 and verify traffic forwarded at port 2.
        6. Remove the route/32 through NH1.
        7. Send the traffic from Port3 and verify the forwarded to Port1.
        8. Add the route/32 through NH1. it followed the route path instead next hop neighbour path.
        9. Remove the route entry with /32 through NH1
        10. Send the traffic from Port3 and verify the forwarded to Port2.
        11. Remove the route and next hop NH1.
        12. Remove the neighbour.
        13. Remove router interface RIF R1, R2 and R3.
        14. Remove Virtual router V1.
        '''

        print "Sending packet port 2 -> port 1 (192.168.0.1 -> 10.1.1.1/32 [id = 101])"
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        v4_enabled = 1
        v6_enabled = 0
        mac = ''

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.1.1.1'
        ip_addr2 = '20.20.20.1'
        ip_addr1_subnet = '10.1.1.1'
        ip_addr2_subnet = '20.20.20.0'
        ip_addr_subnet = '10.1.1.0'
        ip_mask1 = '255.255.255.255'
        ip_mask  = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'

        ### create virtual router
        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        ### create router interfaces
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)
        rif_id3 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)

        ### create neighbor and next hop
        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac2)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr2, rif_id2)

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)

        ### create route
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr2_subnet, ip_mask, rif_id2)

        time.sleep(5)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.1.1.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(
                                eth_dst='00:11:22:33:44:56',
                                eth_src=router_mac,
                                ip_dst='10.1.1.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=63)
        exp_pkt2 = simple_tcp_packet(
                                eth_dst='00:11:22:33:44:55',
                                eth_src=router_mac,
                                ip_dst='10.1.1.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=63)

        try:
            ### Sending the traffic from Port3
            send_packet(self, 2, str(pkt))

            ### Verifying the traffic @ Port2 via  route path
            verify_packets(self, exp_pkt1, [1])

            ### Remove the route/32 through nhop1
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, nhop1)
            time.sleep(3)

            ### Sending the traffic from Port3
            send_packet(self, 2, str(pkt))

            ### Verifying the traffic @ Port1 , via neighbor path
            verify_packets(self, exp_pkt2, [0])

            ### Add the route/32 through nhop1
            sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, nhop1)
            time.sleep(3)

            ### Sending the traffic from Port3
            send_packet(self, 2, str(pkt))

            ### Verifying the traffic @ Port2, via route path
            verify_packets(self, exp_pkt1, [1])

        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, nhop1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr_subnet,  ip_mask, rif_id1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr2_subnet, ip_mask, rif_id2)

            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac2)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_router_interface(rif_id3)
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('l3')
class L3LpbkSubnetTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        Description:
        Send traffic on a layer 3 router interface destined to an IPv4 address within
        the same subnet. Have the nhop entry learnt on the same interface as incoming
        interface. Verify that packet is send out on the same outgoing interface as
        the incoming interface. Set the SAI_ROUTER_INTERFACE_ATTR_LOOPBACK_PACKET_ACTION
        attribute to drop and verify that packet is not send out on the same outgoing
        RIF as incoming RIF.

        Steps:
        Part 1:
          1. Create Virtual Router V1 and enable v4 and v6.
          2. Create a virtual router interface (rif_id1) and set the interface type as "PORT" for port1.
          3. Create IPv4 neighbor entry (10.10.10.1) with MAC1 and associate with "RIF id 1".
          4. Create next hop entry as rif_id1 (Loopback).
          5. Send traffic on router interface rif_id1.
          6. Verify the packet on port1.
          7. Packet should received at port1.
        Part 2:
          1. Set router interface rif_id1 attribute as SAI_ROUTER_INTERFACE_ATTR_LOOPBACK_PACKET_ACTION
          2. set the action as drop.
          3. Send traffic on router interface rif_id1.
          4. Verify the packet on port1.
          5. Packet should not received at port1.

        Clean up by remove the nhop, neighbor, route and the router interface.
        """

        switch_init(self.client)
        port1 = port_list[0]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_addr1_subnet = '10.10.10.0'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'
        ip_addr_subnet = '10.10.0.0'
        ip_mask = '255.255.0.0'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask, rif_id1)


        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src='00:22:22:33:44:55',
                                ip_dst='10.10.10.1',
                                ip_src='10.10.10.2',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(
                                eth_dst='00:11:22:33:44:55',
                                eth_src=router_mac,
                                ip_src='10.10.10.2',
                                ip_dst='10.10.10.1',
                                ip_id=105,
                                ip_ttl=63)
        try:
            send_packet(self, 0, str(pkt))
            verify_packets(self, exp_pkt, [0])

            attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_DROP)
            attr = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_LOOPBACK_PACKET_ACTION, value=attr_value)
            self.client.sai_thrift_set_router_interface_attribute(rif_id1, attr)

            send_packet(self, 0, str(pkt))
            verify_no_packet(self, exp_pkt, 0)
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, nhop1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_FORWARD)
            attr = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_LOOPBACK_PACKET_ACTION, value=attr_value)
            self.client.sai_thrift_set_router_interface_attribute(rif_id1, attr)
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_virtual_router(vr_id)