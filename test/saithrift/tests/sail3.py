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

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port2, 0, v4_enabled, v6_enabled, mac)

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

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port2, 0, v4_enabled, v6_enabled, mac)

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

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port2, 0, v4_enabled, v6_enabled, mac)

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
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port2, 0, v4_enabled, v6_enabled, mac)
        addr_family = SAI_IP_ADDR_FAMILY_IPV6
        
        #Create default route
        sai_thrift_create_route(self.client, vr_id, addr_family, '::', '::', rif_id2, SAI_PACKET_ACTION_DROP)
        
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
                
                pkt = simple_tcpv6_packet( 
                                        eth_dst=router_mac,
                                        eth_src='00:22:22:22:22:22',
                                        ipv6_dst=dest,
                                        ipv6_src='2000:bbbb::1',
                                        ipv6_hlim=64)
                exp_pkt = simple_tcpv6_packet(
                                        eth_dst='00:11:22:33:44:55',
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
            sai_thrift_remove_route(self.client, vr_id, addr_family, '::', '::', None)
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

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port2, 0, v4_enabled, v6_enabled, mac)

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

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port2, 0, v4_enabled, v6_enabled, mac)
        rif_id3 = sai_thrift_create_router_interface(self.client, vr_id, 1, port3, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_addr1_subnet = '10.10.10.0'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, ip_addr1, dmac2)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id2)
        nhop_group1 = sai_thrift_create_next_hop_group(self.client, [nhop1, nhop2])
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id2)

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
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id2)
            sai_thrift_remove_next_hop_from_group(self.client, [nhop1, nhop2])
            self.client.sai_thrift_remove_next_hop_group(nhop_group1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            self.client.sai_thrift_remove_next_hop(nhop2)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, ip_addr1, dmac2)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_router_interface(rif_id3)

            self.client.sai_thrift_remove_virtual_router(vr_id)

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

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port2, 0, v4_enabled, v6_enabled, mac)
        rif_id3 = sai_thrift_create_router_interface(self.client, vr_id, 1, port3, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV6
        ip_addr1 = '5000:1:1:0:0:0:0:1'
        ip_mask1 = 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, ip_addr1, dmac2)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id2)
        nhop_group1 = sai_thrift_create_next_hop_group(self.client, [nhop1, nhop2])
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop_group1)

        # send the test packet(s)
        try:
            pkt = simple_tcpv6_packet(
                                    eth_dst=router_mac,
                                    eth_src='00:22:22:22:22:22',
                                    ipv6_dst='5000:1:1:0:0:0:0:1',
                                    ipv6_src='2000:1:1:0:0:0:0:1',
                                    tcp_sport=0x1234,
                                    ipv6_hlim=64)

            exp_pkt1 = simple_tcpv6_packet(
                                    eth_dst='00:11:22:33:44:55',
                                    eth_src=router_mac,
                                    ipv6_dst='5000:1:1:0:0:0:0:1',
                                    ipv6_src='2000:1:1:0:0:0:0:1',
                                    tcp_sport=0x1234,
                                    ipv6_hlim=63)
            exp_pkt2 = simple_tcpv6_packet(
                                    eth_dst='00:11:22:33:44:56',
                                    eth_src=router_mac,
                                    ipv6_dst='5000:1:1:0:0:0:0:1',
                                    ipv6_src='2000:1:1:0:0:0:0:1',
                                    tcp_sport=0x1234,
                                    ipv6_hlim=63)

            send_packet(self, 2, str(pkt))
            verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2], [0, 1])

            pkt = simple_tcpv6_packet(
                                    eth_dst=router_mac,
                                    eth_src='00:22:22:22:22:45',
                                    ipv6_dst='5000:1:1:0:0:0:0:1',
                                    ipv6_src='2000:1:1:0:0:0:0:1',
                                    tcp_sport=0x1248,
                                    ipv6_hlim=64)

            exp_pkt1 = simple_tcpv6_packet(
                                    eth_dst='00:11:22:33:44:55',
                                    eth_src=router_mac,
                                    ipv6_dst='5000:1:1:0:0:0:0:1',
                                    ipv6_src='2000:1:1:0:0:0:0:1',
                                    tcp_sport=0x1248,
                                    ipv6_hlim=63)
            exp_pkt2 = simple_tcpv6_packet(
                                    eth_dst='00:11:22:33:44:56',
                                    eth_src=router_mac,
                                    ipv6_dst='5000:1:1:0:0:0:0:1',
                                    ipv6_src='2000:1:1:0:0:0:0:1',
                                    tcp_sport=0x1248,
                                    ipv6_hlim=63)

            send_packet(self, 2, str(pkt))
            verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2], [0, 1])
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop_group1)
            sai_thrift_remove_next_hop_from_group(self.client, [nhop1, nhop2])
            self.client.sai_thrift_remove_next_hop_group(nhop_group1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            self.client.sai_thrift_remove_next_hop(nhop2)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, ip_addr1, dmac2)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_router_interface(rif_id3)

            self.client.sai_thrift_remove_virtual_router(vr_id)

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

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port2, 0, v4_enabled, v6_enabled, mac)
        rif_id3 = sai_thrift_create_router_interface(self.client, vr_id, 1, port3, 0, v4_enabled, v6_enabled, mac)
        rif_id4 = sai_thrift_create_router_interface(self.client, vr_id, 1, port4, 0, v4_enabled, v6_enabled, mac)


        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, nhop_ip2, dmac2)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id3, nhop_ip3, dmac3)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip1, rif_id1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip2, rif_id2)
        nhop3 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip3, rif_id3)
        nhop_group1 = sai_thrift_create_next_hop_group(self.client, [nhop1, nhop2, nhop3])
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop_group1)
        sai_thrift_create_route(self.client, vr_id, addr_family, nhop_ip1_subnet, ip_mask2, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, nhop_ip2_subnet, ip_mask2, rif_id2)
        sai_thrift_create_route(self.client, vr_id, addr_family, nhop_ip3_subnet, ip_mask2, rif_id3)

        # send the test packet(s)
        try:
            count = [0, 0, 0]
            dst_ip = int(socket.inet_aton('10.10.10.1').encode('hex'),16)
            max_itrs = 200
            for i in range(0, max_itrs):
                dst_ip_addr = socket.inet_ntoa(hex(dst_ip)[2:].zfill(8).decode('hex'))
                pkt = simple_tcp_packet(eth_dst=router_mac,
                        eth_src='00:22:22:22:22:22',
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
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop_group1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, nhop_ip1_subnet, ip_mask2, rif_id1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, nhop_ip2_subnet, ip_mask2, rif_id2)
            sai_thrift_remove_route(self.client, vr_id, addr_family, nhop_ip3_subnet, ip_mask2, rif_id3)
            sai_thrift_remove_next_hop_from_group(self.client, [nhop1, nhop2, nhop3])
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

            self.client.sai_thrift_remove_virtual_router(vr_id)

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

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port2, 0, v4_enabled, v6_enabled, mac)
        rif_id3 = sai_thrift_create_router_interface(self.client, vr_id, 1, port3, 0, v4_enabled, v6_enabled, mac)
        rif_id4 = sai_thrift_create_router_interface(self.client, vr_id, 1, port4, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV6
        ip_addr1 = '6000:1:1:0:0:0:0:0'
        ip_mask1 = 'ffff:ffff:ffff:ffff:0:0:0:0'
        nhop_ip1 = '2000:1:1:0:0:0:0:1'
        nhop_ip2 = '3000:1:1:0:0:0:0:1'
        nhop_ip3 = '4000:1:1:0:0:0:0:1'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'
        dmac3 = '00:11:22:33:44:57'

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, nhop_ip2, dmac2)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id3, nhop_ip3, dmac3)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip1, rif_id1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip2, rif_id2)
        nhop3 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip3, rif_id3)
        nhop_group1 = sai_thrift_create_next_hop_group(self.client, [nhop1, nhop2, nhop3])
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop_group1)

        # send the test packet(s)
        try:
            count = [0, 0, 0]
            dst_ip = socket.inet_pton(socket.AF_INET6, '6000:1:1:0:0:0:0:1')
            dst_ip_arr = list(dst_ip)
            max_itrs = 200
            sport = 0x1234
            dport = 0x50
            for i in range(0, max_itrs):
                dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
                #HACK: sport is a hack for hashing since the ecmp hash does not
                #include ipv6 sa and da.
                pkt = simple_tcpv6_packet(
                        eth_dst=router_mac,
                        eth_src='00:22:22:22:22:22',
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
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop_group1)
            sai_thrift_remove_next_hop_from_group(self.client, [nhop1, nhop2, nhop3])
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

            self.client.sai_thrift_remove_virtual_router(vr_id)

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
        lag_member_id1 = sai_thrift_create_lag_member(self.client, lag_id1, port1)
        lag_member_id2 = sai_thrift_create_lag_member(self.client, lag_id1, port2)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, lag_id1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port3, 0, v4_enabled, v6_enabled, mac)

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

            self.client.sai_thrift_remove_lag_member(lag_member_id1)
            self.client.sai_thrift_remove_lag_member(lag_member_id2)
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
        lag_member_id1 = sai_thrift_create_lag_member(self.client, lag_id1, port1)
        lag_member_id2 = sai_thrift_create_lag_member(self.client, lag_id1, port2)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, lag_id1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port3, 0, v4_enabled, v6_enabled, mac)

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

            self.client.sai_thrift_remove_lag_member(lag_member_id1)
            self.client.sai_thrift_remove_lag_member(lag_member_id2)
            self.client.sai_thrift_remove_lag(lag_id1)
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('l3')
@group('ecmp')
@group('lag')
class L3EcmpLagTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        switch_init(self.client)
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

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, lag_id1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, lag_id2, 0, v4_enabled, v6_enabled, mac)
        rif_id3 = sai_thrift_create_router_interface(self.client, vr_id, 1, port6, 0, v4_enabled, v6_enabled, mac)
        rif_id4 = sai_thrift_create_router_interface(self.client, vr_id, 1, port7, 0, v4_enabled, v6_enabled, mac)

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, nhop_ip2, dmac2)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id3, nhop_ip3, dmac3)

        nhop1 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip1, rif_id1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip2, rif_id2)
        nhop3 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip3, rif_id3)

        nhop_group1 = sai_thrift_create_next_hop_group(self.client, [nhop1, nhop2, nhop3])
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, nhop_group1)

        try:
            count = [0, 0, 0, 0, 0, 0]
            dst_ip = int(socket.inet_aton('10.10.10.1').encode('hex'), 16)
            src_mac_start = '00:22:22:22:23:'
            max_itrs = 500
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

                send_packet(self, 6, str(pkt))
                rcv_idx = verify_any_packet_any_port(self,
                              [exp_pkt1, exp_pkt2, exp_pkt3],
                              [0, 1, 2, 3, 4, 5])
                count[rcv_idx] += 1
                dst_ip += 1

            print count
            ecmp_count = [count[0]+count[1]+count[2], count[3]+count[4],
                    count[5]]
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

            sai_thrift_remove_next_hop_from_group(self.client, [nhop1, nhop2, nhop3])
            self.client.sai_thrift_remove_next_hop_group(nhop_group1)

            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)

            self.client.sai_thrift_remove_next_hop(nhop2)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, nhop_ip2, dmac2)

            self.client.sai_thrift_remove_next_hop(nhop3)
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

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        lag_id1 = self.client.sai_thrift_create_lag([])
        lag_member11 = sai_thrift_create_lag_member(self.client, lag_id1, port1)
        lag_member12 = sai_thrift_create_lag_member(self.client, lag_id1, port2)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, lag_id1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port3, 0, v4_enabled, v6_enabled, mac)
        rif_id3 = sai_thrift_create_router_interface(self.client, vr_id, 1, port4, 0, v4_enabled, v6_enabled, mac)

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

        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, nhop_ip2, dmac2)

        nhop1 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip1, rif_id1)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, nhop_ip2, rif_id2)

        nhop_group1 = sai_thrift_create_next_hop_group(self.client, [nhop1, nhop2])
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

            sai_thrift_remove_next_hop_from_group(self.client, [nhop1, nhop2])
            self.client.sai_thrift_remove_next_hop_group(nhop_group1)

            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, nhop_ip1, dmac1)

            self.client.sai_thrift_remove_next_hop(nhop2)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, nhop_ip2, dmac2)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_router_interface(rif_id3)

            self.client.sai_thrift_remove_lag_member(lag_member11)
            self.client.sai_thrift_remove_lag_member(lag_member12)
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

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 0, 0, vlan_id, v4_enabled, v6_enabled, mac1)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port2, 0, v4_enabled, v6_enabled, mac2)

        sai_thrift_create_fdb(self.client, vlan_id, dmac1, port1, mac_action)
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
            sai_thrift_delete_fdb(self.client, vlan_id, dmac1, port1)

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

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, 1, port1, 0, v4_enabled, v6_enabled, mac1)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, 1, port2, 0, v4_enabled, v6_enabled, mac2)

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
        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)
        attr_value1 = sai_thrift_attribute_value_t(u16=vlan_id)
        attr1 = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value1)
        self.client.sai_thrift_set_port_attribute(port1, attr1)
        self.client.sai_thrift_set_port_attribute(port2, attr1)
        
        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)
        rif_vlan_id = sai_thrift_create_router_interface(self.client, vr_id, 0, 0, vlan_id, v4_enabled, v6_enabled, mac1)
        rif_port_id = sai_thrift_create_router_interface(self.client, vr_id, 1, port3, 0, v4_enabled, v6_enabled, mac2)

        attr_value2 = sai_thrift_attribute_value_t(s32=SAI_PORT_FDB_LEARNING_MODE_HW)
        attr2 = sai_thrift_attribute_t(id=SAI_PORT_ATTR_FDB_LEARNING_MODE, value=attr_value2)
        self.client.sai_thrift_set_port_attribute(port1, attr2)
        self.client.sai_thrift_set_port_attribute(port2, attr2)
        
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
            self.client.sai_thrift_remove_vlan(vlan_oid)

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
            self.lags_rifs.append(sai_thrift_create_router_interface(self.client, self.vr_id, 1, self.lags[i], 0, self.v4_enabled, self.v6_enabled, ''))
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
        src_mac = self.dataplane.get_mac(0, 16)
        NUM_OF_PKT_TO_EACH_PORT = 254
        NUM_OF_PKTS_TO_SEND = NUM_OF_PKT_TO_EACH_PORT * self.total_lag_port
        for i in xrange(NUM_OF_PKTS_TO_SEND):
                ip_src = '10.0.' + str(i % 255) + '.' + str(i % 255)
                ip_dst = '10.10.' + str((i % num_of_lags) + 1) + '.1'
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
                                        eth_dst=self.mac_pool[i % num_of_lags],
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
        self.src_port = port_list[self.total_lag_port]
        if (len(port_list) < (self.total_lag_port + 1) ) : 
            print "skip this test as it requires 17 ports"
            return
        
        self.vr_id = sai_thrift_create_virtual_router(self.client, self.v4_enabled, self.v6_enabled)
        rif_port_id = sai_thrift_create_router_interface(self.client, self.vr_id, 1, self.src_port, 0, self.v4_enabled, self.v6_enabled, '')
        num_of_lags = self.total_lag_port
        try:
            while (num_of_lags > 0):
                print "testing with " +str(num_of_lags) + " lags"
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
    routes = []
    vr_id = 0
    mac_action = SAI_PACKET_ACTION_FORWARD
    src_port = 0
    mac_pool = []

    def setup_ecmp_lag_group(self, first_rif_port):
        self.lag = self.client.sai_thrift_create_lag([])
        #adding lag members
        sai_thrift_create_lag_member(self.client, self.lag, port_list[1])
        for i in range(self.first_changing_port,first_rif_port):
            self.lag_members.append(sai_thrift_create_lag_member(self.client, self.lag, port_list[i]))
        self.lag_rif = sai_thrift_create_router_interface(self.client, self.vr_id, 1, self.lag, 0, self.v4_enabled, self.v6_enabled, '')
        sai_thrift_create_neighbor(self.client, self.addr_family, self.lag_rif, "10.10.0.1", self.mac_pool[15])
        sai_thrift_create_route(self.client, self.vr_id,self.addr_family, "10.10.0.1", '255.255.255.0', self.lag_rif)        
        self.nhops.append(sai_thrift_create_nhop(self.client, self.addr_family, "10.10.0.1" , self.lag_rif))
        for i in range(first_rif_port,self.total_changing_ports):
            self.port_rifs.append(sai_thrift_create_router_interface(self.client, self.vr_id, 1, port_list[i], 0, self.v4_enabled, self.v6_enabled, ''))
        for i in range(len(self.port_rifs)):
            sai_thrift_create_neighbor(self.client, self.addr_family, self.port_rifs[i], "10.10.%s.1" % str(i+1), self.mac_pool[i])
            self.nhops.append(sai_thrift_create_nhop(self.client, self.addr_family, "10.10.%s.1" % str(i+1), self.port_rifs[i]))
            sai_thrift_create_route(self.client, self.vr_id, self.addr_family, "10.10.%s.1" % str(i+1), '255.255.255.0', self.port_rifs[i])
        self.nhop_group = sai_thrift_create_next_hop_group(self.client, self.nhops)
        sai_thrift_create_route(self.client, self.vr_id, self.addr_family, "10.20.0.0", self.ip_mask, self.nhop_group)

    def teardown_ecmp_lag_group(self, first_rif_port):
        sai_thrift_remove_route(self.client, self.vr_id, self.addr_family, "10.20.0.0", self.ip_mask, self.nhop_group)
        sai_thrift_remove_route(self.client, self.vr_id, self.addr_family, "10.10.0.1", '255.255.255.0', self.lag_rif)        
        for i in range(self.total_changing_ports - first_rif_port):
            sai_thrift_remove_route(self.client, self.vr_id, self.addr_family, "10.10.%s.1" % str(i+1), '255.255.255.0', self.port_rifs[i])
        sai_thrift_remove_next_hop_from_group(self.client, self.nhops)
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
        sai_thrift_remove_neighbor(self.client, self.addr_family, self.lag_rif, "10.10.0.1", self.mac_pool[15])
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
        router_mac = '00:02:03:04:05:00'
        sport = 0x1234
        dport = 0x50
        src_mac = self.dataplane.get_mac(0, 0)
        IP_LAST_WORD_RANGE = 254
        IP_2ND_LAST_WORD_RANGE = 16
        for i in xrange(IP_LAST_WORD_RANGE):
                for j in xrange(IP_2ND_LAST_WORD_RANGE):
                    ip_src = '10.0.' + str(j) + '.' + str(i+1)
                    ip_dst = '10.20.' + str(j+1) + '.1'
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
                    logging.debug("found expected packet from port %d" % destanation_ports[match_index])
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
        self.src_port = port_list[0]
        for i in range (self.total_dst_port+1):
            self.mac_pool.append('00:11:22:33:44:'+str(50+i))

        self.vr_id = sai_thrift_create_virtual_router(self.client, self.v4_enabled, self.v6_enabled)
        rif_port_id = sai_thrift_create_router_interface(self.client, self.vr_id, 1, self.src_port, 0, self.v4_enabled, self.v6_enabled, '')
        
        try:
            # the first iteration will configure port #1 as a lag with only one member
            #and will configure port #2 to port #15 as rifs, 
            #the rif will advance until all of the ports will be in lag and only one if port 
            for first_rif_port in range(self.first_changing_port,self.total_changing_ports):
                print "testing with " +str(first_rif_port - 1) + " lag members"
                self.setup_ecmp_lag_group(first_rif_port)
                self.send_and_verify_packets(first_rif_port)
                self.teardown_ecmp_lag_group(first_rif_port)
        finally:

            #in case of an exception in the send_and_verify_packets
            self.teardown_ecmp_lag_group(self.total_dst_port)#check what number to send for tear down
            self.client.sai_thrift_remove_router_interface(rif_port_id)
            self.client.sai_thrift_remove_virtual_router(self.vr_id)
            print "END OF TEST"
