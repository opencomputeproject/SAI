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

from switch import *

import sai_base_test

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
            self.client.sai_thrift_remove_next_hop_from_group(nhop_group1, [nhop1, nhop2])
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
            self.client.sai_thrift_remove_next_hop_from_group(nhop_group1, [nhop1, nhop2])
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
            self.client.sai_thrift_remove_next_hop_from_group(nhop_group1, [nhop1, nhop2, nhop3])
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
            self.client.sai_thrift_remove_next_hop_from_group(nhop_group1, [nhop1, nhop2, nhop3])
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
                pkt = simple_tcp_packet(eth_dst='00:77:66:55:44:33',
                        eth_src=src_mac,
                        ip_dst=dst_ip_addr,
                        ip_src='192.168.8.1',
                        ip_id=106,
                        ip_ttl=64)

                exp_pkt1 = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                        eth_src='00:77:66:55:44:33',
                        ip_dst=dst_ip_addr,
                        ip_src='192.168.8.1',
                        ip_id=106,
                        ip_ttl=63)
                exp_pkt2 = simple_tcp_packet(eth_dst='00:11:22:33:44:56',
                        eth_src='00:77:66:55:44:33',
                        ip_dst=dst_ip_addr,
                        ip_src='192.168.8.1',
                        ip_id=106,
                        ip_ttl=63)
                exp_pkt3 = simple_tcp_packet(eth_dst='00:11:22:33:44:57',
                        eth_src='00:77:66:55:44:33',
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

            self.client.sai_thrift_remove_next_hop_from_group(nhop_group1, [nhop1, nhop2, nhop3])
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

            self.client.sai_thrift_remove_next_hop_from_group(nhop_group1, [nhop1, nhop2])
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

        self.client.sai_thrift_create_vlan(vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_id, port1, SAI_VLAN_PORT_UNTAGGED)

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
            self.client.sai_thrift_delete_vlan(vlan_id)
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
        self.client.sai_thrift_create_vlan(vlan_id)
        
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_id, port1, SAI_VLAN_PORT_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_id, port2, SAI_VLAN_PORT_TAGGED)
        attr_value1 = sai_thrift_attribute_value_t(u16=vlan_id)
        attr1 = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value1)
        self.client.sai_thrift_set_port_attribute(port1, attr1)
        self.client.sai_thrift_set_port_attribute(port2, attr1)
        
        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)
        rif_vlan_id = sai_thrift_create_router_interface(self.client, vr_id, 0, 0, vlan_id, v4_enabled, v6_enabled, mac1)
        rif_port_id = sai_thrift_create_router_interface(self.client, vr_id, 1, port3, 0, v4_enabled, v6_enabled, mac2)

        attr_value2 = sai_thrift_attribute_value_t(s32=SAI_PORT_LEARN_MODE_HW)
        attr2 = sai_thrift_attribute_t(id=SAI_PORT_ATTR_FDB_LEARNING, value=attr_value2)
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
            verify_no_other_packets(self)
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
            verify_no_other_packets(self)
        finally:
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_vlan_id)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_vlan_id, ip_port1, mac_port1)
            
            self.client.sai_thrift_remove_router_interface(rif_vlan_id)
            self.client.sai_thrift_remove_router_interface(rif_port_id)
            
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_delete_vlan(vlan_id)
            
            self.client.sai_thrift_remove_virtual_router(vr_id)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            