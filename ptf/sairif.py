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
Thrift SAI interface RIF tests
"""

import binascii

from sai_thrift.sai_headers import *

from ptf.mask import Mask
from ptf.packet import *
from ptf.testutils import *
from ptf.thriftutils import *

from sai_base_test import *

@group("draft")
class L3InterfaceTestHelper(PlatformSaiHelper):
    """
    This class contains base router interface tests for regular L3 port RIFs
    """

    def setUp(self):
        super(L3InterfaceTestHelper, self).setUp()

        self.port10_rif_counter_in = 0
        self.port10_rif_counter_out = 0
        self.port11_rif_counter_in = 0
        self.port11_rif_counter_out = 0
        self.port12_rif_counter_out = 0
        self.port13_rif_counter_out = 0
        self.lag1_rif_counter_in = 0
        self.lag1_rif_counter_out = 0
        self.port10_rif_counter_out_octects = 0
        self.port11_rif_counter_in_octects = 0

        dmac1 = '00:11:22:33:44:55'
        dmac3 = '00:33:22:33:44:55'

        self.lag1_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag1)

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.10.2'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('10.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=dmac1)

        self.neighbor_entry3 = sai_thrift_neighbor_entry_t(
            rif_id=self.lag1_rif, ip_address=sai_ipaddress('12.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry3, dst_mac_address=dmac3)
        self.nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('12.10.10.2'),
            router_interface_id=self.lag1_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.route_entry0 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry0, next_hop_id=self.nhop1)

        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:99aa/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1, next_hop_id=self.nhop1)

        self.route_lag0 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('12.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_lag0, next_hop_id=self.nhop3)

        self.route_lag1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:1122:3344:5566:7788/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_lag1, next_hop_id=self.nhop3)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry0)
        sai_thrift_remove_route_entry(self.client, self.route_entry1)

        sai_thrift_remove_route_entry(self.client, self.route_lag0)
        sai_thrift_remove_route_entry(self.client, self.route_lag1)

        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_next_hop(self.client, self.nhop3)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry3)
        sai_thrift_remove_router_interface(self.client, self.lag1_rif)

        super(L3InterfaceTestHelper, self).tearDown()
@group("draft")
class Ipv4FibLagTest(L3InterfaceTestHelper):
    """
    Verifies basic  forwarding on RIF using LAG and with new lag member
    and if packet is dropped on ingress port after being removed from LAG
    """
    def setUp(self):
        super(Ipv4FibLagTest, self).setUp()

    def runTest(self):
        print("\nipv4FibLagTest()")

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='12.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:33:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='12.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)
        pkt_lag = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_src='12.10.10.1',
                                    ip_id=105,
                                    ip_ttl=64)
        exp_pkt_lag = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.10.1',
                                        ip_src='12.10.10.1',
                                        ip_id=105,
                                        ip_ttl=63)

        print("Sending packet port %d -> lag 1 (192.168.0.1 -> "
              "12.10.10.1)" % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet_any_port(
            self, exp_pkt, [self.dev_port4, self.dev_port5,
                            self.dev_port6])
        self.port11_rif_counter_in += 1
        self.lag1_rif_counter_out += 1
        self.port11_rif_counter_in_octects += len(pkt) + 4

        print("Sending packet port %d -> port %d (12.10.10.1 -> "
              "10.10.10.1)" % (self.dev_port4, self.dev_port10))
        send_packet(self, self.dev_port4, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d (12.10.10.1 -> "
              "10.10.10.1)" % (self.dev_port5, self.dev_port10))
        send_packet(self, self.dev_port5, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d (12.10.10.1 -> "
              "10.10.10.1)" % (self.dev_port6, self.dev_port10))
        send_packet(self, self.dev_port6, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d dropped" % self.dev_port24)
        send_packet(self, self.dev_port24, pkt_lag)
        verify_no_other_packets(self, timeout=1)

        lag_member = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag1, port_id=self.port24)

        print("Sending packet port %d-> lag 1 (192.168.0.1 -> 12.10.10.1)"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet_any_port(
            self, exp_pkt, [self.dev_port4, self.dev_port5,
                            self.dev_port6, self.dev_port24])
        self.port11_rif_counter_in += 1
        self.lag1_rif_counter_out += 1
        self.port11_rif_counter_in_octects += len(pkt) + 4

        print("Sending packet port %d -> port %d (12.10.10.1 -> "
              "10.10.10.1)" % (self.dev_port4, self.dev_port10))
        send_packet(self, self.dev_port4, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d (12.10.10.1 -> "
              "10.10.10.1)" % (self.dev_port5, self.dev_port10))
        send_packet(self, self.dev_port5, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d (12.10.10.1 -> "
              "10.10.10.1)" % (self.dev_port6, self.dev_port10))
        send_packet(self, self.dev_port6, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d (12.10.10.1 -> "
              "10.10.10.1)" % (self.dev_port24, self.dev_port10))
        send_packet(self, self.dev_port24, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        sai_thrift_remove_lag_member(self.client, lag_member)

        print("Sending packet port %d -> lag 1 (192.168.0.1 -> 12.10.10.1)"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet_any_port(
            self, exp_pkt, [self.dev_port4, self.dev_port5,
                            self.dev_port6])
        self.port11_rif_counter_in += 1
        self.lag1_rif_counter_out += 1
        self.port11_rif_counter_in_octects += len(pkt) + 4

        print("Sending packet port %d -> port %d (12.10.10.1 -> "
              "10.10.10.1)" % (self.dev_port4, self.dev_port10))
        send_packet(self, self.dev_port4, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d (12.10.10.1 -> "
              "10.10.10.1)" % (self.dev_port5, self.dev_port10))
        send_packet(self, self.dev_port5, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d (12.10.10.1 -> "
              "10.10.10.1)" % (self.dev_port6, self.dev_port10))
        send_packet(self, self.dev_port6, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d dropped" % self.dev_port24)
        send_packet(self, self.dev_port24, pkt_lag)
        verify_no_other_packets(self, timeout=1)

    def tearDown(self):
        super(Ipv4FibLagTest, self).tearDown()

@group("draft")
class Ipv6FibLagTest(L3InterfaceTestHelper):
    """
    Verifies basic IPv6 forwarding on RIF using LAG, with new lag member
    and if packet is dropped on ingress port after being removed from LAG
    """
    def setUp(self):
        super(Ipv6FibLagTest, self).setUp()

    def runTest(self):
        print("\nipv6FibLagTest()")

        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:1122:3344:5566:7788',
            ipv6_src='2000::1',
            ipv6_hlim=64)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:33:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:1122:3344:5566:7788',
            ipv6_src='2000::1',
            ipv6_hlim=63)
        pkt_lag = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_src='1234:5678:9abc:def0:1122:3344:5566:7788',
            ipv6_hlim=64)
        exp_pkt_lag = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_src='1234:5678:9abc:def0:1122:3344:5566:7788',
            ipv6_hlim=63)

        print("Sending packet port %d -> lag 1 (2000::1 -> "
              "1234:5678:9abc:def0:1122:3344:5566:7788)"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet_any_port(
            self, exp_pkt, [self.dev_port4, self.dev_port5,
                            self.dev_port6])
        self.port11_rif_counter_in += 1
        self.lag1_rif_counter_out += 1
        self.port11_rif_counter_in_octects += len(pkt) + 4

        print("Sending packet port %d -> port %d "
              "(1234:5678:9abc:def0:1122:3344:5566:7788 -> "
              "1234:5678:9abc:def0:4422:1133:5577:99aa"
              % (self.dev_port4, self.dev_port10))
        send_packet(self, self.dev_port4, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d "
              "(1234:5678:9abc:def0:1122:3344:5566:7788 -> "
              "1234:5678:9abc:def0:4422:1133:5577:99aa"
              % (self.dev_port5, self.dev_port10))
        send_packet(self, self.dev_port5, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d "
              "(1234:5678:9abc:def0:1122:3344:5566:7788 -> "
              "1234:5678:9abc:def0:4422:1133:5577:99aa"
              % (self.dev_port6, self.dev_port10))
        send_packet(self, self.dev_port6, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d" % self.dev_port24, " dropped")
        send_packet(self, self.dev_port24, pkt_lag)
        verify_no_other_packets(self, timeout=1)

        lag_member = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag1, port_id=self.port24)

        print("Sending packet port %d -> lag 1 (2000::1 -> "
              "1234:5678:9abc:def0:1122:3344:5566:7788)"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet_any_port(
            self, exp_pkt, [self.dev_port4, self.dev_port5,
                            self.dev_port6, self.dev_port24])
        self.port11_rif_counter_in += 1
        self.lag1_rif_counter_out += 1
        self.port11_rif_counter_in_octects += len(pkt) + 4

        print("Sending packet port %d -> port %d "
              "(1234:5678:9abc:def0:1122:3344:5566:7788 -> "
              "1234:5678:9abc:def0:4422:1133:5577:99aa"
              % (self.dev_port4, self.dev_port10))
        send_packet(self, self.dev_port4, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d "
              "(1234:5678:9abc:def0:1122:3344:5566:7788 -> "
              "1234:5678:9abc:def0:4422:1133:5577:99aa"
              % (self.dev_port5, self.dev_port10))
        send_packet(self, self.dev_port5, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d "
              "(1234:5678:9abc:def0:1122:3344:5566:7788 -> "
              "1234:5678:9abc:def0:4422:1133:5577:99aa"
              % (self.dev_port6, self.dev_port10))
        send_packet(self, self.dev_port6, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d "
              "(1234:5678:9abc:def0:1122:3344:5566:7788 -> "
              "1234:5678:9abc:def0:4422:1133:5577:99aa"
              % (self.dev_port24, self.dev_port10))
        send_packet(self, self.dev_port24, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        sai_thrift_remove_lag_member(self.client, lag_member)

        print("Sending packet port %d -> lag 1 (2000::1 -> "
              "1234:5678:9abc:def0:1122:3344:5566:7788)"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet_any_port(
            self, exp_pkt, [self.dev_port4, self.dev_port5,
                            self.dev_port6])
        self.port11_rif_counter_in += 1
        self.lag1_rif_counter_out += 1
        self.port11_rif_counter_in_octects += len(pkt) + 4

        print("Sending packet port %d -> port %d "
              "(1234:5678:9abc:def0:1122:3344:5566:7788 -> "
              "1234:5678:9abc:def0:4422:1133:5577:99aa"
              % (self.dev_port4, self.dev_port10))
        send_packet(self, self.dev_port4, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d "
              "(1234:5678:9abc:def0:1122:3344:5566:7788 -> "
              "1234:5678:9abc:def0:4422:1133:5577:99aa"
              % (self.dev_port5, self.dev_port10))
        send_packet(self, self.dev_port5, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d -> port %d "
              "(1234:5678:9abc:def0:1122:3344:5566:7788 -> "
              "1234:5678:9abc:def0:4422:1133:5577:99aa"
              % (self.dev_port6, self.dev_port10))
        send_packet(self, self.dev_port6, pkt_lag)
        verify_packet(self, exp_pkt_lag, self.dev_port10)
        self.lag1_rif_counter_in += 1
        self.port10_rif_counter_out += 1
        self.port10_rif_counter_out_octects += len(exp_pkt_lag) + 4

        print("Sending packet port %d" % self.dev_port24, " dropped")
        send_packet(self, self.dev_port24, pkt_lag)
        verify_no_other_packets(self, timeout=1)

    def tearDown(self):
        super(Ipv6FibLagTest, self).tearDown()

@group("draft")
class RifSharedMtuTest(L3InterfaceTestHelper):
    """
    Verifies same MTU value shared between RIF and if MTU check works
    after deleting and adding another RIF with same MTU value
    """
    def setUp(self):
        super(RifSharedMtuTest, self).setUp()

    def runTest(self):
        print("\nrifSharedMtuTest()")

        port24_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port24,
            admin_v4_state=True)

        mac_action = SAI_PACKET_ACTION_FORWARD
        dmac1 = '00:11:22:33:44:66'

        neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=port24_rif, ip_address=sai_ipaddress('20.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, neighbor_entry1, dst_mac_address=dmac1)
        nhop1 = sai_thrift_create_next_hop(self.client,
                                           ip=sai_ipaddress('20.10.10.2'),
                                           router_interface_id=port24_rif,
                                           type=SAI_NEXT_HOP_TYPE_IP)
        route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('20.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, route_entry1, next_hop_id=nhop1)

        mac_action = SAI_PACKET_ACTION_FORWARD
        dmac2 = '00:11:22:33:44:77'
        neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan30_rif, ip_address=sai_ipaddress('30.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, neighbor_entry2, dst_mac_address=dmac2)
        nhop2 = sai_thrift_create_next_hop(self.client,
                                           ip=sai_ipaddress('30.10.10.2'),
                                           router_interface_id=self.vlan30_rif,
                                           type=SAI_NEXT_HOP_TYPE_IP)
        route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('30.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, route_entry2, next_hop_id=nhop2)

        fdb_entry = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id, mac_address=dmac2, bv_id=self.vlan30)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.port20_bp,
                                    packet_action=mac_action)
        sai_thrift_set_port_attribute(
            self.client, self.port20, port_vlan_id=30)

        print("MTU is 200, packet size is 199")
        sai_thrift_set_router_interface_attribute(
            self.client, port24_rif, mtu=200)
        sai_thrift_set_router_interface_attribute(
            self.client, self.vlan30_rif, mtu=200)

        try:
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='20.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=199 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:66',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='20.10.10.1',
                                        ip_src='192.168.0.1',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=199 + 14)
            vlan_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                         eth_src='00:22:22:22:22:22',
                                         ip_dst='30.10.10.1',
                                         ip_src='192.168.0.1',
                                         ip_id=105,
                                         ip_ttl=64,
                                         pktlen=199 + 14)
            vlan_exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:77',
                                             eth_src=ROUTER_MAC,
                                             ip_dst='30.10.10.1',
                                             ip_src='192.168.0.1',
                                             ip_id=105,
                                             ip_ttl=63,
                                             pktlen=199 + 14)

            print("Sending packet port %d -> port %d (192.168.0.1 -> "
                  "20.10.10.1)" % (self.dev_port11, self.dev_port24))
            send_packet(self, self.dev_port11, pkt)
            verify_packet(self, exp_pkt, self.dev_port24)
            self.port11_rif_counter_in += 1
            self.port11_rif_counter_in_octects += len(pkt) + 4

            print("Sending vlan packet port %d -> vlan 30 (port %d) "
                  "(192.168.0.1 -> 30.10.10.1)"
                  % (self.dev_port11, self.dev_port20))
            send_packet(self, self.dev_port11, vlan_pkt)
            verify_packet(self, vlan_exp_pkt, self.dev_port20)
            self.port11_rif_counter_in += 1
            self.port11_rif_counter_in_octects += len(vlan_pkt) + 4

            # update mtu on port10_rif
            print("Setting port24_rif MTU to 190")
            sai_thrift_set_router_interface_attribute(
                self.client, port24_rif, mtu=190)
            print("Sending packet port %d, droppped" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port11_rif_counter_in += 1
            self.port11_rif_counter_in_octects += len(pkt) + 4

            print("Sending vlan packet port %d -> vlan 30 (port %d) "
                  "(192.168.0.1 -> 30.10.10.1)"
                  % (self.dev_port11, self.dev_port20))
            send_packet(self, self.dev_port11, vlan_pkt)
            verify_packet(self, vlan_exp_pkt, self.dev_port20)
            self.port11_rif_counter_in += 1
            self.port11_rif_counter_in_octects += len(vlan_pkt) + 4

            # update mtu on vlan30_rif
            print("Setting vlan30_rif MTU to 190")
            sai_thrift_set_router_interface_attribute(
                self.client, self.vlan30_rif, mtu=190)
            print("Sending packet port %d, droppped" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port11_rif_counter_in += 1
            self.port11_rif_counter_in_octects += len(pkt) + 4

            print("Sending vlan packet port %d, droppped" % self.dev_port11)
            send_packet(self, self.dev_port11, vlan_pkt)
            verify_no_other_packets(self, timeout=1)
            self.port11_rif_counter_in += 1
            self.port11_rif_counter_in_octects += len(vlan_pkt) + 4

            # remove port24_rif, vlan30_rif should not be affected
            print("Removing port24_rif")
            sai_thrift_remove_route_entry(self.client, route_entry1)
            sai_thrift_remove_next_hop(self.client, nhop1)
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry1)
            sai_thrift_remove_router_interface(self.client, port24_rif)

            print("Sending vlan packet port %d, droppped" % self.dev_port11)
            send_packet(self, self.dev_port11, vlan_pkt)
            verify_no_other_packets(self, timeout=1)
            self.port11_rif_counter_in += 1
            self.port11_rif_counter_in_octects += len(vlan_pkt) + 4

            print("Adding port24_rif")
            port24_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                virtual_router_id=self.default_vrf,
                port_id=self.port24,
                admin_v4_state=True)

            mac_action = SAI_PACKET_ACTION_FORWARD
            dmac1 = '00:11:22:33:44:66'
            neighbor_entry1 = sai_thrift_neighbor_entry_t(
                rif_id=port24_rif, ip_address=sai_ipaddress('20.10.10.2'))
            sai_thrift_create_neighbor_entry(
                self.client, neighbor_entry1, dst_mac_address=dmac1)
            nhop1 = sai_thrift_create_next_hop(self.client,
                                               ip=sai_ipaddress('20.10.10.2'),
                                               router_interface_id=port24_rif,
                                               type=SAI_NEXT_HOP_TYPE_IP)
            route_entry1 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('20.10.10.1/32'))
            sai_thrift_create_route_entry(
                self.client, route_entry1, next_hop_id=nhop1)

            print("Sending packet port %d -> port %d (192.168.0.1 -> "
                  "20.10.10.1)" % (self.dev_port11, self.dev_port24))
            send_packet(self, self.dev_port11, pkt)
            verify_packet(self, exp_pkt, self.dev_port24)
            self.port11_rif_counter_in += 1
            self.port11_rif_counter_in_octects += len(pkt) + 4

            print("Sending vlan packet port %d, droppped" % self.dev_port11)
            send_packet(self, self.dev_port11, vlan_pkt)
            verify_no_other_packets(self, timeout=1)
            self.port11_rif_counter_in += 1
            self.port11_rif_counter_in_octects += len(vlan_pkt) + 4

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry1)
            sai_thrift_remove_next_hop(self.client, nhop1)
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry1)
            sai_thrift_remove_router_interface(self.client, port24_rif)
            sai_thrift_set_port_attribute(
                self.client, self.port20, port_vlan_id=0)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)
            sai_thrift_remove_route_entry(self.client, route_entry2)
            sai_thrift_remove_next_hop(self.client, nhop2)
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry2)

    def tearDown(self):
        super(RifSharedMtuTest, self).tearDown()

@group("draft")
class McastDisableTest(L3InterfaceTestHelper):
    """
    Verify IPv4 multicast packets are dropped when V4_MCAST_ENABLE
    or V6_MCAST_ENABLE is set to False
    """
    def setUp(self):
        super(McastDisableTest, self).setUp()

    def runTest(self):
        print("\nmcastDisableTest()")

        dst_mcast_ip = "225.0.0.1"
        dst_mcast_ipv6 = "ff11::1111:1"
        src_ip = "10.10.10.1"
        src_ipv6 = "2001:0db8::1111"

        try:
            ipmc_group = sai_thrift_create_ipmc_group(self.client)

            rpf_group = sai_thrift_create_rpf_group(self.client)

            ipmc_group_member1 = sai_thrift_create_ipmc_group_member(
                self.client,
                ipmc_group_id=ipmc_group,
                ipmc_output_id=self.port10_rif)

            ipmc_group_member2 = sai_thrift_create_ipmc_group_member(
                self.client,
                ipmc_group_id=ipmc_group,
                ipmc_output_id=self.port12_rif)

            ipmc_group_member3 = sai_thrift_create_ipmc_group_member(
                self.client,
                ipmc_group_id=ipmc_group,
                ipmc_output_id=self.port13_rif)

            ipmc_entry = sai_thrift_ipmc_entry_t(
                switch_id=self.switch_id,
                vr_id=self.default_vrf,
                type=SAI_IPMC_ENTRY_TYPE_SG,
                source=sai_ipaddress(src_ip),
                destination=sai_ipaddress(dst_mcast_ip))
            sai_thrift_create_ipmc_entry(
                self.client,
                ipmc_entry,
                packet_action=SAI_PACKET_ACTION_FORWARD,
                rpf_group_id=rpf_group,
                output_group_id=ipmc_group)

            ipmc_entry_v6 = sai_thrift_ipmc_entry_t(
                switch_id=self.switch_id,
                vr_id=self.default_vrf,
                type=SAI_IPMC_ENTRY_TYPE_SG,
                source=sai_ipaddress(src_ipv6),
                destination=sai_ipaddress(dst_mcast_ipv6))
            sai_thrift_create_ipmc_entry(
                self.client,
                ipmc_entry_v6,
                packet_action=SAI_PACKET_ACTION_FORWARD,
                rpf_group_id=rpf_group,
                output_group_id=ipmc_group)

            mcast_pkt = simple_tcp_packet(eth_dst='01:00:5e:11:22:33',
                                          eth_src='00:22:22:22:22:22',
                                          ip_dst=dst_mcast_ip,
                                          ip_src=src_ip,
                                          ip_id=105,
                                          ip_ttl=64)

            print("Enabling IPv4 multicast on port %d" % self.dev_port11)
            sai_thrift_set_router_interface_attribute(
                self.client, self.port11_rif, v4_mcast_enable=True)

            print("Sending IPv4 multicast packet on port %d -> ports "
                  "%d, %d, %d" % (self.dev_port11, self.dev_port10,
                                  self.dev_port12, self.dev_port13))
            send_packet(self, self.dev_port11, mcast_pkt)
            verify_packets(self, mcast_pkt, [self.dev_port10, self.dev_port12,
                                             self.dev_port13])
            print("\tOK")
            self.port11_rif_counter_in += 1
            self.port10_rif_counter_out += 1
            self.port12_rif_counter_out += 1
            self.port13_rif_counter_out += 1
            self.port11_rif_counter_in_octects += len(mcast_pkt) + 4
            self.port10_rif_counter_out_octects += len(mcast_pkt) + 4

            print("Disabling IPv4 multicast on port %d" % self.dev_port11)
            sai_thrift_set_router_interface_attribute(
                self.client, self.port11_rif, v4_mcast_enable=False)

            print("Sending IPv4 multicast packet on port %d"
                  % self.dev_port11)
            send_packet(self, self.dev_port11, mcast_pkt)
            verify_no_other_packets(self)
            print("\tDropped")
            self.port11_rif_counter_in += 1
            self.port11_rif_counter_in_octects += len(mcast_pkt) + 4

            mcast_pkt_v6 = simple_tcpv6_packet(eth_dst='33:33:00:11:22:33',
                                               eth_src='00:22:22:22:22:22',
                                               ipv6_dst=dst_mcast_ipv6,
                                               ipv6_src=src_ipv6,
                                               ipv6_hlim=64)

            print("Enabling IPv6 multicast on port %d" % self.dev_port11)
            sai_thrift_set_router_interface_attribute(
                self.client, self.port11_rif, v6_mcast_enable=True)

            print("Sending IPv6 multicast packet on port %d -> ports "
                  "%d, %d, %d" % (self.dev_port11, self.dev_port10,
                                  self.dev_port12, self.dev_port13))
            send_packet(self, self.dev_port11, mcast_pkt_v6)
            verify_packets(self, mcast_pkt_v6, [self.dev_port10,
                                                self.dev_port12,
                                                self.dev_port13])
            print("\tOK")
            self.port11_rif_counter_in += 1
            self.port10_rif_counter_out += 1
            self.port12_rif_counter_out += 1
            self.port13_rif_counter_out += 1
            self.port11_rif_counter_in_octects += len(mcast_pkt_v6) + 4
            self.port10_rif_counter_out_octects += len(mcast_pkt_v6) + 4

            print("Disabling IPv6 multicast on port %d" % self.dev_port11)
            sai_thrift_set_router_interface_attribute(
                self.client, self.port11_rif, v6_mcast_enable=False)

            print("Sending IPv6 multicast packet on port %d"
                  % self.dev_port11)
            send_packet(self, self.dev_port11, mcast_pkt_v6)
            verify_no_other_packets(self)
            print("\tDropped")
            self.port11_rif_counter_in += 1
            self.port11_rif_counter_in_octects += len(mcast_pkt_v6) + 4

        finally:
            sai_thrift_remove_ipmc_entry(self.client, ipmc_entry_v6)
            sai_thrift_remove_ipmc_entry(self.client, ipmc_entry)
            sai_thrift_remove_ipmc_group_member(
                self.client, ipmc_group_member3)
            sai_thrift_remove_ipmc_group_member(
                self.client, ipmc_group_member2)
            sai_thrift_remove_ipmc_group_member(
                self.client, ipmc_group_member1)
            sai_thrift_remove_rpf_group(self.client, rpf_group)
            sai_thrift_remove_ipmc_group(self.client, ipmc_group)

    def tearDown(self):
        super(McastDisableTest, self).tearDown()

@group("draft")
class DuplicatePortRifCreationTest(L3InterfaceTestHelper):
    """
    Verifies if duplicate L3 RIF creation fails
    """
    def setUp(self):
        super(DuplicatePortRifCreationTest, self).setUp()

    def runTest(self):
        print("\nduplicatePortRifCreationTest()")

        existing_rif_port = self.port10
        existing_rif_lag = self.lag3

        try:
            print("Trying to create duplicated port RIF")
            dupl_port_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                virtual_router_id=self.default_vrf,
                port_id=existing_rif_port)
            self.assertFalse(dupl_port_rif != 0)
            print("Creation failed")

            print("Trying to create duplicated LAG RIF")
            dupl_lag_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                virtual_router_id=self.default_vrf,
                port_id=existing_rif_lag)
            self.assertFalse(dupl_lag_rif != 0)
            print("Creation failed")

        finally:
            if dupl_port_rif:
                sai_thrift_remove_router_interface(self.client, dupl_port_rif)
            if dupl_lag_rif:
                sai_thrift_remove_router_interface(self.client, dupl_lag_rif)

    def tearDown(self):
        super(DuplicatePortRifCreationTest, self).tearDown()


@group("draft")
class L3InterfaceSimplifiedTestHelper(PlatformSaiHelper):
    """
    Configuration
    +----------+-----------+
    | port0    | port0_rif |
    +----------+-----------+
    | port1    | port1_rif |
    +----------+-----------+
    """
    def setUp(self):
        super(L3InterfaceSimplifiedTestHelper, self).setUp()

        dmac1 = '00:11:22:33:44:55'

        self.create_routing_interfaces(ports=[0, 1])
        self.port1_rif_counter_in = 0
        self.port0_rif_counter_out = 0
        self.port0_rif_counter_out_octects = 0
        self.port1_rif_counter_in_octects = 0

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.10.2'),
            router_interface_id=self.port0_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port0_rif, ip_address=sai_ipaddress('10.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=dmac1)

        self.route_entry0 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry0, next_hop_id=self.nhop1)

        self.route_entry0_lpm = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('11.11.11.0/24'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry0_lpm, next_hop_id=self.nhop1)

        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:99aa/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1, next_hop_id=self.nhop1)

        self.route_entry1_lpm = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('4000::0/65'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1_lpm, next_hop_id=self.nhop1)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry0)
        sai_thrift_remove_route_entry(self.client, self.route_entry0_lpm)

        sai_thrift_remove_route_entry(self.client, self.route_entry1)
        sai_thrift_remove_route_entry(self.client, self.route_entry1_lpm)

        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)

        self.destroy_routing_interfaces()

        super(L3InterfaceSimplifiedTestHelper, self).tearDown()

@group("draft")
class MacUpdateTest(L3InterfaceSimplifiedTestHelper):
    """
    Verifies if packet is forwarded correctly after MAC address updated
    and if packet is dropped for old MAC address after update
    """
    def setUp(self):
        super(MacUpdateTest, self).setUp()

    def runTest(self):
        print("\nMacUpdateTest()")

        new_router_mac = "00:77:66:55:44:44"
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='11.11.11.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='11.11.11.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)
        pkt1 = simple_tcp_packet(eth_dst=new_router_mac,
                                 eth_src='00:22:22:22:22:22',
                                 ip_dst='11.11.11.1',
                                 ip_src='192.168.0.1',
                                 ip_id=105,
                                 ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='11.11.11.1',
                                     ip_src='192.168.0.1',
                                     ip_id=105,
                                     ip_ttl=63)

        print("Sending packet on port %d with mac %s, forward"
              % (self.dev_port1, ROUTER_MAC))
        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4
        self.port0_rif_counter_out_octects += len(exp_pkt) + 4

        print("Updating src_mac_address to %s" % (new_router_mac))
        sai_thrift_set_router_interface_attribute(
            self.client, self.port1_rif, src_mac_address=new_router_mac)
        attrs = sai_thrift_get_router_interface_attribute(
            self.client, self.port1_rif, src_mac_address=True)
        self.assertEqual(attrs["src_mac_address"], new_router_mac)
        # still forwarded since rmac is same as switch default rmac
        print("Sending packet on port %d with mac %s, forwarded"
              % (self.dev_port1, ROUTER_MAC))
        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4
        self.port0_rif_counter_out_octects += len(exp_pkt) + 4

        print("Sending packet on port %d with mac %s, forward"
              % (self.dev_port1, new_router_mac))
        send_packet(self, self.dev_port1, pkt1)
        verify_packet(self, exp_pkt1, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(pkt1) + 4
        self.port0_rif_counter_out_octects += len(exp_pkt1) + 4

        print("Reverting src_mac_address to %s" % (ROUTER_MAC))
        sai_thrift_set_router_interface_attribute(
            self.client, self.port1_rif, src_mac_address=ROUTER_MAC)
        attrs = sai_thrift_get_router_interface_attribute(
            self.client, self.port1_rif, src_mac_address=True)
        self.assertEqual(attrs["src_mac_address"], ROUTER_MAC)
        print("Sending packet on port %d with mac %s, dropped"
              % (self.dev_port1, new_router_mac))
        send_packet(self, self.dev_port1, pkt1)
        verify_no_other_packets(self, timeout=1)
        self.port1_rif_counter_in += 1
        self.port1_rif_counter_in_octects += len(pkt1) + 4

        print("Sending packet on port %d with mac %s, forward"
              % (self.dev_port1, ROUTER_MAC))
        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4
        self.port0_rif_counter_out_octects += len(exp_pkt) + 4

    def tearDown(self):
        super(MacUpdateTest, self).tearDown()

@group("draft")
class Ipv4FibLPMTest(L3InterfaceSimplifiedTestHelper):
    """
    Verifies basic forwarding for IPv4 LPM routes
    """
    def setUp(self):
        super(Ipv4FibLPMTest, self).setUp()

    def runTest(self):
        print("\nipv4FibLPMTest()")

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='11.11.11.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='11.11.11.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)

        print("Sending packet port %d -> port %d (192.168.0.1 -> "
              "11.11.11.0/24) LPM" % (self.dev_port1, self.dev_port0))
        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4
        self.port0_rif_counter_out_octects += len(exp_pkt) + 4

    def tearDown(self):
        super(Ipv4FibLPMTest, self).tearDown()

@group("draft")
class Ipv4FibTest(L3InterfaceSimplifiedTestHelper):
    """
    Verifies basic forwarding for IPv4 host
    """
    def setUp(self):
        super(Ipv4FibTest, self).setUp()

    def runTest(self):
        print("\nipv4FibTest()")

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='11.11.11.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='11.11.11.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)
        gre_pkt = ipv4_erspan_pkt(eth_dst=ROUTER_MAC,
                                  eth_src='00:22:22:22:22:22',
                                  ip_dst='11.11.11.1',
                                  ip_src='192.168.0.1',
                                  ip_id=0,
                                  ip_ttl=64,
                                  ip_flags=0x0,
                                  version=2,  # ERSPAN_III
                                  mirror_id=1,
                                  sgt_other=4,
                                  inner_frame=pkt)
        exp_gre_pkt = ipv4_erspan_pkt(eth_dst='00:11:22:33:44:55',
                                      eth_src=ROUTER_MAC,
                                      ip_dst='11.11.11.1',
                                      ip_src='192.168.0.1',
                                      ip_id=0,
                                      ip_ttl=63,
                                      ip_flags=0x0,
                                      version=2,  # ERSPAN_III
                                      mirror_id=1,
                                      sgt_other=4,
                                      inner_frame=pkt)

        print("Sending packet port %d -> port %d (192.168.0.1 -> "
              "11.11.11.1)" % (self.dev_port1, self.dev_port0))
        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4
        self.port0_rif_counter_out_octects += len(exp_pkt) + 4

        print("Sending gre encapsulated packet port %d -> port %d "
              "(192.168.0.1 -> 11.11.11.1)"
              % (self.dev_port1, self.dev_port0))
        send_packet(self, self.dev_port1, gre_pkt)
        verify_packet(self, exp_gre_pkt, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(gre_pkt) + 4
        self.port0_rif_counter_out_octects += len(exp_gre_pkt) + 4

    def tearDown(self):
        super(Ipv4FibTest, self).tearDown()

@group("draft")
class Ipv4DisableTest(L3InterfaceSimplifiedTestHelper):
    """
    Verifies if IPv4 packets are dropped when admin_v4_state is false
    """
    def setUp(self):
        super(Ipv4DisableTest, self).setUp()

    def runTest(self):
        print("\nipv4DisableTest()")

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)

        print("Sending packet on port %d, forward" % self.dev_port1)
        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4
        self.port0_rif_counter_out_octects += len(exp_pkt) + 4

        print("Disable IPv4 on ingress RIF")
        sai_thrift_set_router_interface_attribute(
            self.client, self.port1_rif, admin_v4_state=False)
        time.sleep(3)

        initial_stats = sai_thrift_get_port_stats(self.client, self.port1)
        if_in_discards_pre = initial_stats['SAI_PORT_STAT_IF_IN_DISCARDS']

        print("Sending packet on port %d, discard" % self.dev_port1)
        send_packet(self, self.dev_port1, pkt)
        verify_no_other_packets(self, timeout=3)
        stats = sai_thrift_get_port_stats(self.client, self.port1)
        if_in_discards = stats['SAI_PORT_STAT_IF_IN_DISCARDS']
        self.assertTrue(if_in_discards_pre + 1 == if_in_discards)
        self.port1_rif_counter_in += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4

        print("Enable IPv4 on ingress RIF")
        sai_thrift_set_router_interface_attribute(
            self.client, self.port1_rif, admin_v4_state=True)

        print("Sending packet on port %d, forward" % self.dev_port1)
        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4
        self.port0_rif_counter_out_octects += len(exp_pkt) + 4

    def tearDown(self):
        super(Ipv4DisableTest, self).tearDown()

@group("draft")
class RifMyIPTest(L3InterfaceSimplifiedTestHelper):
    """
    Verifies if MYIP works for subnet routes
    """
    def setUp(self):
        super(RifMyIPTest, self).setUp()

    def runTest(self):
        print("\nrifMyIPTest()")

        try:
            sw_attr = sai_thrift_get_switch_attribute(self.client,
                                                      cpu_port=True)
            cpu_port = sw_attr['cpu_port']

            ip2me_route = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('10.10.20.1/32'))
            sai_thrift_create_route_entry(
                self.client, ip2me_route, next_hop_id=cpu_port)

            myip_trap_group = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True, queue=4)
            myip_trap = sai_thrift_create_hostif_trap(
                self.client,
                trap_group=myip_trap_group,
                packet_action=SAI_PACKET_ACTION_TRAP,
                trap_type=SAI_HOSTIF_TRAP_TYPE_IP2ME)
            self.assertTrue(myip_trap != 0)

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.20.1',
                                    ip_src='30.30.0.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=100)

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            print("Sending MYIP packet")
            send_packet(self, self.dev_port1, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)
            print("Forwarded to CPU")

        finally:
            sai_thrift_remove_hostif_trap(self.client, myip_trap)
            sai_thrift_remove_hostif_trap_group(self.client, myip_trap_group)
            sai_thrift_remove_route_entry(self.client, ip2me_route)

    def tearDown(self):
        super(RifMyIPTest, self).tearDown()
@group("draft")
class Ipv6DisableTest(L3InterfaceSimplifiedTestHelper):
    """
    Verifies if IPv6 packets are dropped when admin_v6_state is False
    """
    def setUp(self):
        super(Ipv6DisableTest, self).setUp()

    def runTest(self):
        print("\nipv6DisableTest()")

        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_src='2000::1',
            ipv6_hlim=64)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_src='2000::1',
            ipv6_hlim=63)

        print("Sending packet on port %d, forward" % self.dev_port1)
        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4
        self.port0_rif_counter_out_octects += len(exp_pkt) + 4

        print("Disable IPv6 on ingress RIF")
        sai_thrift_set_router_interface_attribute(
            self.client, self.port1_rif, admin_v6_state=False)
        initial_stats = sai_thrift_get_port_stats(self.client, self.port1)
        if_in_discards_pre = initial_stats['SAI_PORT_STAT_IF_IN_DISCARDS']

        print("Sending packet on port %d, discard" % self.dev_port1)
        send_packet(self, self.dev_port1, pkt)
        verify_no_other_packets(self, timeout=1)
        stats = sai_thrift_get_port_stats(self.client, self.port1)
        if_in_discards = stats['SAI_PORT_STAT_IF_IN_DISCARDS']
        self.assertTrue(if_in_discards_pre + 1 == if_in_discards)
        self.port1_rif_counter_in += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4

        print("Enable IPv6 on ingress RIF")
        sai_thrift_set_router_interface_attribute(
            self.client, self.port1_rif, admin_v6_state=True)

        print("Sending packet on port %d, forward" % self.dev_port1)
        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4
        self.port0_rif_counter_out_octects += len(exp_pkt) + 4

    def tearDown(self):
        super(Ipv6DisableTest, self).tearDown()

@group("draft")
class Ipv6FibTest(L3InterfaceSimplifiedTestHelper):
    """
    Verifies basic forwarding for IPv6 host
    """
    def setUp(self):
        super(Ipv6FibTest, self).setUp()

    def runTest(self):
        print("\nipv6FibTest()")

        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_src='2000::1',
            ipv6_hlim=64)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_src='2000::1',
            ipv6_hlim=63)

        print("Sending packet port %d -> port %d (2000::1 -> "
              "1234:5678:9abc:def0:4422:1133:5577:99aa)"
              % (self.dev_port1, self.dev_port0))
        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4
        self.port0_rif_counter_out_octects += len(exp_pkt) + 4

    def tearDown(self):
        super(Ipv6FibTest, self).tearDown()

@group("draft")
class Ipv6FibLpmTest(L3InterfaceSimplifiedTestHelper):
    """
    Verifies basic forwarding for IPv6 LPM route
    """
    def setUp(self):
        super(Ipv6FibLpmTest, self).setUp()

    def runTest(self):
        print("\nipv6FibLpmTest()")

        pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                  eth_src='00:22:22:22:22:22',
                                  ipv6_dst='4000::1',
                                  ipv6_src='2000::1',
                                  ipv6_hlim=64)
        exp_pkt = simple_tcpv6_packet(eth_dst='00:11:22:33:44:55',
                                      eth_src=ROUTER_MAC,
                                      ipv6_dst='4000::1',
                                      ipv6_src='2000::1',
                                      ipv6_hlim=63)

        print("Sending packet port %d -> port %d (2000::1 -> 4000::1) "
              "LPM entry 4000::0/65" % (self.dev_port1, self.dev_port0))
        send_packet(self, self.dev_port1, pkt)
        verify_packet(self, exp_pkt, self.dev_port0)
        self.port1_rif_counter_in += 1
        self.port0_rif_counter_out += 1
        self.port1_rif_counter_in_octects += len(pkt) + 4
        self.port0_rif_counter_out_octects += len(exp_pkt) + 4

    def tearDown(self):
        super(Ipv6FibLpmTest, self).tearDown()


@group("draft")
class L3InterfaceAclTestHelper(PlatformSaiHelper):
    """
    Configuration
    +----------+-----------+
    | port0    | port0_rif |
    +----------+-----------+
    | port1    | port1_rif |
    +----------+-----------+
    """
    def setUp(self):
        super(L3InterfaceAclTestHelper, self).setUp()

        dmac1 = '00:11:22:33:44:55'

        self.create_routing_interfaces(ports=[0, 1])
        self.port1_rif_counter_in = 0
        self.port0_rif_counter_out = 0

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.10.2'),
            router_interface_id=self.port0_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port0_rif, ip_address=sai_ipaddress('10.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=dmac1)

        self.route_entry0 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry0, next_hop_id=self.nhop1)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry0)

        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)

        self.destroy_routing_interfaces()

        super(L3InterfaceAclTestHelper, self).tearDown()

@group("draft")
class Ipv4IngressAclTest(L3InterfaceAclTestHelper):
    """
    Verifies Ingress ACL table and group bind to RIF
    """
    def setUp(self):
        super(Ipv4IngressAclTest, self).setUp()

    def runTest(self):
        print("\nipv4IngressAclTest()")

        stage = SAI_ACL_STAGE_INGRESS
        bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE]
        action_types = [SAI_ACL_ACTION_TYPE_PACKET_ACTION]
        action_drop = SAI_PACKET_ACTION_DROP
        dst_ip = '10.10.10.1'
        dst_ip_mask = '255.255.255.0'

        acl_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(bind_points), int32list=bind_points)
        acl_action_type_list = sai_thrift_s32_list_t(
            count=len(action_types), int32list=action_types)
        acl_table = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=acl_bind_point_type_list,
            acl_action_type_list=acl_action_type_list,
            field_dst_ip=True)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(s32=action_drop))
        ip_addr = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=dst_ip),
            mask=sai_thrift_acl_field_data_mask_t(ip4=dst_ip_mask))

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)
        try:
            acl_entry = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table,
                action_packet_action=packet_action,
                field_dst_ip=ip_addr)

            print("Sending packet on port %d, forward" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1

            sai_thrift_set_router_interface_attribute(
                self.client, self.port1_rif, ingress_acl=acl_table)
            print("Sending packet on port %d, drop" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1

            sai_thrift_set_router_interface_attribute(
                self.client, self.port1_rif, ingress_acl=0)
            print("Sending packet on port %d, forward" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1

            acl_table_group = sai_thrift_create_acl_table_group(
                self.client,
                acl_stage=stage,
                acl_bind_point_type_list=acl_bind_point_type_list,
                type=SAI_ACL_TABLE_GROUP_TYPE_PARALLEL)
            acl_group_member = sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=acl_table_group,
                acl_table_id=acl_table)

            print("Sending packet on port %d, forward" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1

            sai_thrift_set_router_interface_attribute(
                self.client, self.port1_rif, ingress_acl=acl_table_group)
            print("Sending packet on port %d, drop" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1

            sai_thrift_set_router_interface_attribute(
                self.client, self.port1_rif, ingress_acl=0)
            print("Sending packet on port %d, forward" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1

        finally:
            sai_thrift_remove_acl_table_group_member(
                self.client, acl_group_member)
            sai_thrift_remove_acl_table_group(self.client, acl_table_group)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)

    def tearDown(self):
        super(Ipv4IngressAclTest, self).tearDown()

@group("draft")
class Ipv4EgressAclTest(L3InterfaceAclTestHelper):
    # TODO: test requires additional verification
    """
    Verifies Egress ACL table and group bind to RIF
    """
    def setUp(self):
        super(Ipv4EgressAclTest, self).setUp()

    def runTest(self):
        print("\nipv4EgressAclTest()")

        stage = SAI_ACL_STAGE_EGRESS
        bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE]
        action_types = [SAI_ACL_ACTION_TYPE_PACKET_ACTION]
        action_drop = SAI_PACKET_ACTION_DROP
        dst_ip = '10.10.10.1'
        dst_ip_mask = '255.255.255.0'

        acl_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(bind_points), int32list=bind_points)
        acl_action_type_list = sai_thrift_s32_list_t(
            count=len(action_types), int32list=action_types)
        acl_table = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=acl_bind_point_type_list,
            acl_action_type_list=acl_action_type_list,
            field_dst_ip=True)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(s32=action_drop))
        ip_addr = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=dst_ip),
            mask=sai_thrift_acl_field_data_mask_t(ip4=dst_ip_mask))

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)

        try:
            acl_entry = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table,
                action_packet_action=packet_action,
                field_dst_ip=ip_addr)

            print("Sending packet on port %d, forward" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1

            sai_thrift_set_router_interface_attribute(
                self.client, self.port0_rif, egress_acl=acl_table)
            print("Sending packet on port %d, drop" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1

            sai_thrift_set_router_interface_attribute(
                self.client, self.port0_rif, egress_acl=0)
            print("Sending packet on port %d, forward" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1

            acl_table_group = sai_thrift_create_acl_table_group(
                self.client,
                acl_stage=stage,
                acl_bind_point_type_list=acl_bind_point_type_list,
                type=SAI_ACL_TABLE_GROUP_TYPE_PARALLEL)
            acl_group_member = sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=acl_table_group,
                acl_table_id=acl_table)

            print("Sending packet on port %d, forward" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1

            sai_thrift_set_router_interface_attribute(
                self.client, self.port0_rif, egress_acl=acl_table_group)
            print("Sending packet on port %d, drop" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1

            sai_thrift_set_router_interface_attribute(
                self.client, self.port0_rif, egress_acl=0)
            print("Sending packet on port %d, forward" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1

        finally:
            sai_thrift_remove_acl_table_group_member(
                self.client, acl_group_member)
            sai_thrift_remove_acl_table_group(self.client, acl_table_group)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)

    def tearDown(self):
        super(Ipv4EgressAclTest, self).tearDown()


@group("draft")
class NegativeRifTest(PlatformSaiHelper):
    """
    Verifies if creating fails when RIF type is port and port_id is 0,
    if getting, removal and setting of non existent RIF fails
    and if creating fails with invalid vrf_id
    """
    def setUp(self):
        super(NegativeRifTest, self).setUp()

    def runTest(self):
        print("\nNegativeRifTest()")

        port0_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port0,
            admin_v4_state=True)

        self.assertTrue(sai_thrift_remove_router_interface(
            self.client, port0_rif) == 0)
        self.assertTrue(sai_thrift_remove_router_interface(
            self.client, port0_rif) != 0)

        # Non existing RIF
        rif_attr = sai_thrift_get_router_interface_attribute(
            self.client, router_interface_oid=port0_rif, mtu=True)
        self.assertEqual(self.status(), SAI_STATUS_ITEM_NOT_FOUND)
        self.assertEqual(rif_attr, None)

        self.assertNotEqual(
            sai_thrift_set_router_interface_attribute(
                self.client, router_interface_oid=port0_rif, mtu=200),
            SAI_STATUS_SUCCESS)

        rif_attr = sai_thrift_get_router_interface_attribute(
            self.client, router_interface_oid=port0_rif, mtu=True)
        self.assertEqual(self.status(), SAI_STATUS_ITEM_NOT_FOUND)
        self.assertEqual(rif_attr, None)

        # Incorrect RIF attributes
        invalid_port = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=-1)
        self.assertTrue(invalid_port == 0)

        invalid_vrf = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=-1,
            port_id=self.port1)
        self.assertTrue(invalid_vrf == 0)

    def tearDown(self):
        super(NegativeRifTest, self).tearDown()

@group("draft")
class LoopbackRifTest(PlatformSaiHelper):
    """
    Verifies multiple loopback RIF on same VRF is allowed
    """
    def setUp(self):
        super(LoopbackRifTest, self).setUp()

    def runTest(self):
        print("\nLoopbackRifTest()")

        lbk_rif1 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.default_vrf)
        self.assertTrue(lbk_rif1 != 0)

        lbk_rif2 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.default_vrf)
        self.assertTrue(lbk_rif2 != 0)

        self.assertTrue(lbk_rif1 != lbk_rif2)

        sai_thrift_remove_router_interface(self.client, lbk_rif1)
        sai_thrift_remove_router_interface(self.client, lbk_rif2)

    def tearDown(self):
        super(LoopbackRifTest, self).tearDown()

@group("draft")
class RifCreateOrUpdateRmacTest(PlatformSaiHelper):
    """
    Verifies RIF can be created or updated with custom rmac
    """
    def setUp(self):
        super(RifCreateOrUpdateRmacTest, self).setUp()

    def runTest(self):
        print("rifCreateOrUpdateRmacTest()")
        irif = e1_rif = e2_rif = None
        iport_route = e1_port_route = e2_port_route = None
        iport_nbor = e1_port_nbor = e2_port_nbor = None
        iport_nbor_nhop = e1_port_nbor_nhop = e2_port_nbor_nhop = None

        iport = self.port0
        dev_iport = self.dev_port0

        iport_nbor_mac = '00:11:ab:ab:ab:ab'
        iport_ipv4_nbor = '10.1.1.2'
        iport_ipv4 = '10.1.1.1'

        e1_port = self.port1
        dev_e1_port = self.dev_port1

        e1_port_nbor_mac = '00:11:ac:ac:ac:ac'
        e1_port_ipv4 = '10.2.2.1'
        e1_port_ipv4_nbor = '10.2.2.2'

        e2_port = self.port2
        dev_e2_port = self.dev_port2
        e2_mac = '00:11:dd:dd:dd:dd'
        e2_new_mac = '00:11:ee:ee:ee:ee'

        e2_port_nbor_mac = '00:11:ad:ad:ad:ad'
        e2_port_ipv4 = '10.3.3.1'
        e2_port_ipv4_nbor = '10.3.3.2'

        try:
            irif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                virtual_router_id=self.default_vrf,
                port_id=iport,
            )
            self.assertGreater(irif, 0)

            iport_nbor = sai_thrift_neighbor_entry_t(
                rif_id=irif,
                ip_address=sai_ipaddress(iport_ipv4_nbor),
            )
            sai_thrift_create_neighbor_entry(
                self.client,
                iport_nbor,
                dst_mac_address=iport_nbor_mac,
            )

            iport_nbor_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(iport_ipv4_nbor),
                router_interface_id=irif,
                type=SAI_NEXT_HOP_TYPE_IP,
            )
            self.assertGreater(iport_nbor_nhop, 0)

            iport_route = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(iport_ipv4 + '/24'),
            )
            sai_thrift_create_route_entry(
                self.client,
                iport_route,
                next_hop_id=iport_nbor_nhop,
            )

            e1_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                virtual_router_id=self.default_vrf,
                port_id=e1_port,
            )
            self.assertGreater(e1_rif, 0)

            e1_rif_attr = sai_thrift_get_router_interface_attribute(
                self.client,
                router_interface_oid=e1_rif,
                src_mac_address=True,
            )
            self.assertIsNotNone(e1_rif_attr)
            src_mac_address = e1_rif_attr.get('src_mac_address')
            self.assertEqual(src_mac_address, ROUTER_MAC)

            e1_port_nbor = sai_thrift_neighbor_entry_t(
                rif_id=e1_rif,
                ip_address=sai_ipaddress(e1_port_ipv4_nbor),
            )
            sai_thrift_create_neighbor_entry(
                self.client,
                e1_port_nbor,
                dst_mac_address=e1_port_nbor_mac,
            )

            e1_port_nbor_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(e1_port_ipv4_nbor),
                router_interface_id=e1_rif,
                type=SAI_NEXT_HOP_TYPE_IP,
            )
            self.assertGreater(e1_port_nbor_nhop, 0)

            e1_port_route = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(e1_port_ipv4 + '/24'),
            )
            sai_thrift_create_route_entry(
                self.client,
                e1_port_route,
                next_hop_id=e1_port_nbor_nhop,
            )

            e2_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                virtual_router_id=self.default_vrf,
                port_id=e2_port,
                src_mac_address=e2_mac,
            )
            self.assertGreater(e2_rif, 0)

            e2_rif_attr = sai_thrift_get_router_interface_attribute(
                self.client,
                router_interface_oid=e2_rif,
                src_mac_address=True,
            )
            self.assertIsNotNone(e2_rif_attr)
            src_mac_address = e2_rif_attr.get('src_mac_address')
            self.assertEqual(src_mac_address, e2_mac)

            e2_port_nbor = sai_thrift_neighbor_entry_t(
                rif_id=e2_rif,
                ip_address=sai_ipaddress(e2_port_ipv4_nbor),
            )
            sai_thrift_create_neighbor_entry(
                self.client,
                e2_port_nbor,
                dst_mac_address=e2_port_nbor_mac,
            )

            e2_port_nbor_nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(e2_port_ipv4_nbor),
                router_interface_id=e2_rif,
                type=SAI_NEXT_HOP_TYPE_IP,
            )
            self.assertGreater(e2_port_nbor_nhop, 0)

            e2_port_route = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(e2_port_ipv4 + '/24'),
            )
            sai_thrift_create_route_entry(
                self.client,
                e1_port_route,
                next_hop_id=e2_port_nbor_nhop,
            )

            pkt_in1 = simple_udp_packet(
                eth_dst=ROUTER_MAC,
                eth_src=iport_nbor_mac,
                ip_dst=e1_port_ipv4_nbor,
                ip_src=iport_ipv4_nbor,
                ip_ttl=64,
            )
            pkt_e1 = simple_udp_packet(
                eth_dst=e1_port_nbor_mac,
                eth_src=ROUTER_MAC,
                ip_dst=e1_port_ipv4_nbor,
                ip_src=iport_ipv4_nbor,
                ip_ttl=63,
            )
            send_packet(self, dev_iport, pkt_in1)
            verify_packets(self, pkt_e1, [dev_e1_port])
            verify_no_other_packets(self, timeout=1)

            pkt_in2 = simple_udp_packet(
                eth_dst=ROUTER_MAC,
                eth_src=iport_nbor_mac,
                ip_dst=e2_port_ipv4_nbor,
                ip_src=iport_ipv4_nbor,
                ip_ttl=64,
            )
            pkt_e2 = simple_udp_packet(
                eth_dst=e2_port_nbor_mac,
                eth_src=e2_mac,
                ip_dst=e2_port_ipv4_nbor,
                ip_src=iport_ipv4_nbor,
                ip_ttl=63,
            )

            send_packet(self, dev_iport, pkt_in2)
            verify_packets(self, pkt_e2, [dev_e2_port])
            verify_no_other_packets(self, timeout=1)

            sai_thrift_set_router_interface_attribute(
                self.client,
                router_interface_oid=e2_rif,
                src_mac_address=e2_new_mac,
            )

            e2_rif_attr = sai_thrift_get_router_interface_attribute(
                self.client,
                router_interface_oid=e2_rif,
                src_mac_address=True,
            )
            self.assertIsNotNone(e2_rif_attr)
            src_mac_address = e2_rif_attr.get('src_mac_address')
            self.assertEqual(src_mac_address, e2_new_mac)

            pkt_e2 = simple_udp_packet(
                eth_dst=e2_port_nbor_mac,
                eth_src=e2_new_mac,
                ip_dst=e2_port_ipv4_nbor,
                ip_src=iport_ipv4_nbor,
                ip_ttl=63,
            )

            send_packet(self, dev_iport, pkt_in2)
            verify_packets(self, pkt_e2, [dev_e2_port])
            verify_no_other_packets(self, timeout=1)

        finally:
            for route in iport_route, e1_port_route, e2_port_route:
                if route is not None:
                    sai_thrift_remove_route_entry(self.client, route)

            for nbor in iport_nbor, e1_port_nbor, e2_port_nbor:
                if nbor is not None:
                    sai_thrift_remove_neighbor_entry(self.client, nbor)

            for nhop in iport_nbor_nhop, e1_port_nbor_nhop, e2_port_nbor_nhop:
                if nhop is not None:
                    sai_thrift_remove_next_hop(self.client, nhop)

            for rif in irif, e1_rif, e2_rif:
                if rif is not None:
                    sai_thrift_remove_router_interface(self.client, rif)

    def tearDown(self):
        super(RifCreateOrUpdateRmacTest, self).tearDown()


@group("draft")
class L3InterfaceMtuTestHelper(PlatformSaiHelper):
    """
    Configuration
    +----------+-----------+
    | port0    |           |
    +----------+-----------+
    | port1    |           |
    +----------+-----------+
    | lag1     | port2     |
    |          | port3     |
    |          | port4     |
    +----------+-----------+
    """
    def setUp(self):
        super(L3InterfaceMtuTestHelper, self).setUp()
        dmac1 = '00:11:22:33:44:55'
        dmac3 = '00:33:22:33:44:55'

        self.port1_rif_counter_in = 0
        self.port0_rif_counter_out = 0
        self.lag1_rif_counter_out = 0
        self.port0_rif_counter_out_octects = 0
        self.port1_rif_counter_in_octects = 0

        self.create_routing_interfaces(ports=[0, 1])
        self.create_lag_with_members(lag_index=1, ports=[2, 3, 4])
        self.create_routing_interfaces(lags=[1])

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.10.2'),
            router_interface_id=self.port0_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port0_rif, ip_address=sai_ipaddress('10.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=dmac1)

        self.neighbor_entry3 = sai_thrift_neighbor_entry_t(
            rif_id=self.lag1_rif, ip_address=sai_ipaddress('12.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry3, dst_mac_address=dmac3)
        self.nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('12.10.10.2'),
            router_interface_id=self.lag1_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.route_entry0 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry0, next_hop_id=self.nhop1)

        self.route_lag0 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('12.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_lag0, next_hop_id=self.nhop3)

        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:99aa/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1, next_hop_id=self.nhop1)

        self.route_lag1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:1122:3344:5566:7788/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_lag1, next_hop_id=self.nhop3)

    def tearDown(self):

        sai_thrift_remove_route_entry(self.client, self.route_entry0)
        sai_thrift_remove_route_entry(self.client, self.route_lag0)

        sai_thrift_remove_route_entry(self.client, self.route_entry1)
        sai_thrift_remove_route_entry(self.client, self.route_lag1)

        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)

        sai_thrift_remove_next_hop(self.client, self.nhop3)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry3)

        self.destroy_routing_interfaces()
        self.destroy_bridge_ports()
        self.destroy_lags_with_members()

        super(L3InterfaceMtuTestHelper, self).tearDown()

@group("draft")
class Ipv4MtuTest(L3InterfaceMtuTestHelper):
    """
    Verifies if IPv4 packet is forwarded, dropped and punted to CPU
    depending on the MTU with and without a trap for regular L3 port
    """
    def setUp(self):
        super(Ipv4MtuTest, self).setUp()

    def runTest(self):
        print("\nipv4MtuTest()")

        # set MTU to 200 for port 0 and lag 1
        mtu_port0_rif = sai_thrift_get_router_interface_attribute(
            self.client, self.port0_rif, mtu=True)
        mtu_lag1_rif = sai_thrift_get_router_interface_attribute(
            self.client, self.lag1_rif, mtu=True)

        sai_thrift_set_router_interface_attribute(
            self.client, self.port0_rif, mtu=200)
        sai_thrift_set_router_interface_attribute(
            self.client, self.lag1_rif, mtu=200)

        try:
            print("Max MTU is 200, send pkt size 199, send to port/lag")
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=199 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.10.1',
                                        ip_src='192.168.0.1',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=199 + 14)

            print("Sending packet port %d -> port %d (192.168.0.1 -> "
                  "10.10.10.1)" % (self.dev_port1, self.dev_port0))
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4
            self.port0_rif_counter_out_octects += len(exp_pkt) + 4

            pkt['IP'].dst = '12.10.10.1'
            exp_pkt['IP'].dst = '12.10.10.1'
            exp_pkt['Ethernet'].dst = '00:33:22:33:44:55'

            print("Sending packet port %d -> lag 1 (192.168.0.1 -> "
                  "12.10.10.1)" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port2, self.dev_port3,
                                self.dev_port4])
            self.port1_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4

            print("Max MTU is 200, send pkt size 200, send to port/lag")
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=200 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.10.1',
                                        ip_src='192.168.0.1',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=200 + 14)

            print("Sending packet port %d -> port %d (192.168.0.1 -> "
                  "10.10.10.1)" % (self.dev_port1, self.dev_port0))
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4
            self.port0_rif_counter_out_octects += len(exp_pkt) + 4

            pkt['IP'].dst = '12.10.10.1'
            exp_pkt['IP'].dst = '12.10.10.1'
            exp_pkt['Ethernet'].dst = '00:33:22:33:44:55'

            print("Sending packet port %d -> lag 1 (192.168.0.1 -> "
                  "12.10.10.1)" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port2, self.dev_port3,
                                self.dev_port4])
            self.port1_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4

            print("Max MTU is 200, send pkt size 201, dropped")
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=201 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.10.1',
                                        ip_src='192.168.0.1',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=201 + 14)

            print("Sending packet port %d" % self.dev_port1, " dropped")
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4
            self.port0_rif_counter_out_octects += len(exp_pkt) + 4

            pkt['IP'].dst = '12.10.10.1'
            exp_pkt['IP'].dst = '12.10.10.1'
            exp_pkt['Ethernet'].dst = '00:33:22:33:44:55'

            print("Sending packet port %d" % self.dev_port1, " dropped")
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4

            print("Changing MTU to 201, send pkt size 201, send to port/lag")
            sai_thrift_set_router_interface_attribute(
                self.client, self.port0_rif, mtu=201)
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=201 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.10.1',
                                        ip_src='192.168.0.1',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=201 + 14)

            print("Sending packet port %d -> port %d (192.168.0.1 -> "
                  "10.10.10.1)" % (self.dev_port1, self.dev_port0))
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4
            self.port0_rif_counter_out_octects += len(exp_pkt) + 4

            pkt['IP'].dst = '12.10.10.1'
            exp_pkt['IP'].dst = '12.10.10.1'
            exp_pkt['Ethernet'].dst = '00:33:22:33:44:55'

            print("Sending packet port %d" % self.dev_port1, " dropped")
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4

            sai_thrift_set_router_interface_attribute(
                self.client, self.lag1_rif, mtu=201)

            print("Sending packet port %d -> lag 1 (192.168.0.1 -> "
                  "12.10.10.1)" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port2, self.dev_port3,
                                self.dev_port4])
            self.port1_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4

            print("Max MTU is 201, send pkt size 202, dropped")
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=202 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.10.1',
                                        ip_src='192.168.0.1',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=202 + 14)

            print("Sending packet port %d" % self.dev_port1, " dropped")
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4
            self.port0_rif_counter_out_octects += len(exp_pkt) + 4

            pkt['IP'].dst = '12.10.10.1'
            exp_pkt['IP'].dst = '12.10.10.1'
            exp_pkt['Ethernet'].dst = '00:33:22:33:44:55'

            print("Sending packet port %d" % self.dev_port1, " dropped")
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4

        finally:
            sai_thrift_set_router_interface_attribute(
                self.client, self.port0_rif, mtu=mtu_port0_rif['mtu'])
            sai_thrift_set_router_interface_attribute(
                self.client, self.lag1_rif, mtu=mtu_lag1_rif['mtu'])

    def tearDown(self):
        super(Ipv4MtuTest, self).tearDown()

@group("draft")
class Ipv6MtuTest(L3InterfaceMtuTestHelper):
    """
    Verifies if IPv6 packet is forwarded, dropped and punted to CPU
    depending on the MTU with and without a trap for regular L3 port
    """
    def setUp(self):
        super(Ipv6MtuTest, self).setUp()

    def runTest(self):
        print("\nipv6MtuTest()")

        mtu_port0_rif = sai_thrift_get_router_interface_attribute(
            self.client, self.port0_rif, mtu=True)
        mtu_lag1_rif = sai_thrift_get_router_interface_attribute(
            self.client, self.lag1_rif, mtu=True)

        # set MTU to 200 for port 0 and lag 1
        sai_thrift_set_router_interface_attribute(
            self.client, self.port0_rif, mtu=200)
        sai_thrift_set_router_interface_attribute(
            self.client, self.lag1_rif, mtu=200)

        try:
            print("Max MTU is 200, send pkt size 199, send to port/lag")
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=199 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:22:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=199 + 14 + 40)

            print("Sending packet port %d -> port %d "
                  "(2000::1 -> 1234:5678:9abc:def0:4422:1133:5577:99aa')"
                  % (self.dev_port1, self.dev_port0))
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4
            self.port0_rif_counter_out_octects += len(exp_pkt) + 4

            pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'
            exp_pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'
            exp_pkt['Ethernet'].dst = '00:33:22:33:44:55'

            print("Sending packet port %d -> lag 1 (2000::1 -> "
                  "1234:5678:9abc:def0:1122:3344:5566:7788)"
                  % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port2, self.dev_port3,
                                self.dev_port4])
            self.port1_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4

            print("Max MTU is 200, send pkt size 200, send to port/lag")
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=200 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:22:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=200 + 14 + 40)

            print("Sending packet port %d -> port %d "
                  "(2000::1 -> 1234:5678:9abc:def0:4422:1133:5577:99aa')"
                  % (self.dev_port1, self.dev_port0))
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4
            self.port0_rif_counter_out_octects += len(exp_pkt) + 4

            pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'
            exp_pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'
            exp_pkt['Ethernet'].dst = '00:33:22:33:44:55'

            print("Sending packet port %d -> lag 1 (2000::1 -> "
                  "1234:5678:9abc:def0:1122:3344:5566:7788)"
                  % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port2, self.dev_port3,
                                self.dev_port4])
            self.port1_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4

            print("Max MTU is 200, send pkt size 201, dropped")
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=201 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:22:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=201 + 14 + 40)

            print("Sending packet port %d" % self.dev_port1, " dropped")
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4
            self.port0_rif_counter_out_octects += len(exp_pkt) + 4

            pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'
            exp_pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'
            exp_pkt['Ethernet'].dst = '00:33:22:33:44:55'

            print("Sending packet port %d" % self.dev_port1, " dropped")
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4

            print("Changing MTU to 201, send pkt size 201, send to port/lag")
            sai_thrift_set_router_interface_attribute(
                self.client, self.port0_rif, mtu=201)
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=201 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:22:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=201 + 14 + 40)
            print("Sending packet port %d -> port %d "
                  "(2000::1 -> 1234:5678:9abc:def0:4422:1133:5577:99aa')"
                  % (self.dev_port1, self.dev_port0))
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4
            self.port0_rif_counter_out_octects += len(exp_pkt) + 4

            pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'
            exp_pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'
            exp_pkt['Ethernet'].dst = '00:33:22:33:44:55'

            print("Sending packet port %d" % self.dev_port1, " dropped")
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4

            sai_thrift_set_router_interface_attribute(
                self.client, self.lag1_rif, mtu=201)

            print("Sending packet port %d -> lag 1 (2000::1 ->"
                  "1234:5678:9abc:def0:1122:3344:5566:7788)"
                  % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port2, self.dev_port3,
                                self.dev_port4])
            self.port1_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4

            print("Max MTU is 201, send pkt size 202, dropped")
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=202 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:22:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=202 + 14 + 40)

            print("Sending packet port %d" % self.dev_port1, " dropped")
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1
            self.port0_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4
            self.port0_rif_counter_out_octects += len(exp_pkt) + 4

            pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'
            exp_pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'
            exp_pkt['Ethernet'].dst = '00:33:22:33:44:55'

            print("Sending packet port %d" % self.dev_port1, " dropped")
            send_packet(self, self.dev_port1, pkt)
            verify_no_other_packets(self, timeout=1)
            self.port1_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            self.port1_rif_counter_in_octects += len(pkt) + 4

        finally:
            sai_thrift_set_router_interface_attribute(
                self.client, self.port0_rif, mtu=mtu_port0_rif['mtu'])
            sai_thrift_set_router_interface_attribute(
                self.client, self.lag1_rif, mtu=mtu_lag1_rif['mtu'])

    def tearDown(self):
        super(Ipv6MtuTest, self).tearDown()


@group("draft")
class L3SubPortTestHelper(PlatformSaiHelper):
    """
    This class contains base router interface tests for L3 subport RIFs
    """

    def setUp(self):
        super(L3SubPortTestHelper, self).setUp()

        self.port10_rif_in = 0
        self.port10_rif_out = 0
        self.port11_rif_in = 0
        self.port11_rif_out = 0
        self.port30_bp = 0
        self.lag3_rif_in = 0
        self.lag3_rif_out = 0
        self.vlan600_rif_in = 0
        self.vlan600_rif_out = 0
        self.vlan700_rif_in = 0
        self.vlan700_rif_out = 0
        self.subport10_100_in = 0
        self.subport10_200_in = 0
        self.subport11_200_in = 0
        self.subport11_300_in = 0
        self.sublag3_400_in = 0
        self.sublag3_500_in = 0
        self.subport24_600_in = 0
        self.subport25_400_in = 0
        self.subport25_500_in = 0
        self.subport10_100_out = 0
        self.subport10_200_out = 0
        self.subport11_200_out = 0
        self.subport11_300_out = 0
        self.sublag3_400_out = 0
        self.sublag3_500_out = 0
        self.subport24_600_out = 0
        self.subport25_400_out = 0
        self.subport25_500_out = 0

        dmac0 = '00:11:22:33:44:55'
        dmac1 = '00:22:22:33:44:55'
        dmac2 = '00:33:22:33:44:55'
        dmac3 = '00:44:22:33:44:55'
        dmac4 = '00:55:22:33:44:55'
        mac_action = SAI_PACKET_ACTION_FORWARD

        # L3 RIF on port10, port11 and lag3
        self.nhop0 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.10.1'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry0 = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('10.10.10.1'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry0, dst_mac_address=dmac0)

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.10.2'),
            router_interface_id=self.port11_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port11_rif, ip_address=sai_ipaddress('10.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=dmac1)

        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.10.3'),
            router_interface_id=self.lag3_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=self.lag3_rif, ip_address=sai_ipaddress('10.10.10.3'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry2, dst_mac_address=dmac2)

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

        # add SVI on vlan600, port24 and vlan700, port 25
        self.vlan600 = sai_thrift_create_vlan(self.client, vlan_id=600)
        self.vlan_member601 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan600,
            bridge_port_id=self.port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan600_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan600)
        self.nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.10.4'),
            router_interface_id=self.vlan600_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry3 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan600_rif, ip_address=sai_ipaddress('10.10.10.4'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry3, dst_mac_address=dmac3)
        self.fdb_entry3 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id, mac_address=dmac3, bv_id=self.vlan600)
        sai_thrift_create_fdb_entry(self.client,
                                    self.fdb_entry3,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.port24_bp,
                                    packet_action=mac_action)
        sai_thrift_set_port_attribute(
            self.client, self.port24, port_vlan_id=600)

        self.vlan700 = sai_thrift_create_vlan(self.client, vlan_id=700)
        self.vlan_member701 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan700,
            bridge_port_id=self.port25_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan700_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan700)
        self.nhop4 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.10.5'),
            router_interface_id=self.vlan700_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry4 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan700_rif, ip_address=sai_ipaddress('10.10.10.5'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry4, dst_mac_address=dmac4)
        self.fdb_entry4 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id, mac_address=dmac4, bv_id=self.vlan700)
        sai_thrift_create_fdb_entry(self.client,
                                    self.fdb_entry4,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.port25_bp,
                                    packet_action=mac_action)

        # add subport 100, 200 on port10
        self.subport10_100 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port10,
            admin_v4_state=True,
            outer_vlan_id=100)
        self.nhop_sp10_100 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.20.0.10'),
            router_interface_id=self.subport10_100,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_sp10_100 = sai_thrift_neighbor_entry_t(
            rif_id=self.subport10_100, ip_address=sai_ipaddress('20.20.0.10'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry_sp10_100,
                                         dst_mac_address="00:33:33:33:01:00")

        self.subport10_200 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port10,
            admin_v4_state=True,
            outer_vlan_id=200)
        self.nhop_sp10_200 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.20.0.20'),
            router_interface_id=self.subport10_200,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_sp10_200 = sai_thrift_neighbor_entry_t(
            rif_id=self.subport10_200, ip_address=sai_ipaddress('20.20.0.20'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry_sp10_200,
                                         dst_mac_address="00:33:33:33:02:00")

        # add subport 200, 300 on port11
        self.subport11_200 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port11,
            admin_v4_state=True,
            outer_vlan_id=200)
        self.nhop_sp11_200 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.20.1.20'),
            router_interface_id=self.subport11_200,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_sp11_200 = sai_thrift_neighbor_entry_t(
            rif_id=self.subport11_200, ip_address=sai_ipaddress('20.20.1.20'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry_sp11_200,
                                         dst_mac_address="00:33:33:33:12:00")

        self.subport11_300 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port11,
            admin_v4_state=True,
            outer_vlan_id=300)
        self.nhop_sp11_300 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.20.1.30'),
            router_interface_id=self.subport11_300,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_sp11_300 = sai_thrift_neighbor_entry_t(
            rif_id=self.subport11_300, ip_address=sai_ipaddress('20.20.1.30'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry_sp11_300,
                                         dst_mac_address="00:33:33:33:13:00")

        # add subport 400, 500 on lag3
        self.sublag3_400 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag3,
            admin_v4_state=True,
            outer_vlan_id=400)
        self.nhop_sl3_400 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.20.0.40'),
            router_interface_id=self.sublag3_400,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_sl3_400 = sai_thrift_neighbor_entry_t(
            rif_id=self.sublag3_400, ip_address=sai_ipaddress('20.20.0.40'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry_sl3_400,
                                         dst_mac_address="00:33:33:33:04:00")

        self.sublag3_500 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag3,
            admin_v4_state=True,
            outer_vlan_id=500)
        self.nhop_sl3_500 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.20.0.50'),
            router_interface_id=self.sublag3_500,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.neighbor_entry_sl3_500 = sai_thrift_neighbor_entry_t(
            rif_id=self.sublag3_500, ip_address=sai_ipaddress('20.20.0.50'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry_sl3_500,
                                         dst_mac_address="00:33:33:33:05:00")

        # add subport 600 on port24, this port is untagged on vlan600
        self.subport24_600 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port24,
            admin_v4_state=True,
            outer_vlan_id=600)
        self.nhop_sp24_600 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.20.3.60'),
            router_interface_id=self.subport24_600,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_sp24_600 = sai_thrift_neighbor_entry_t(
            rif_id=self.subport24_600, ip_address=sai_ipaddress('20.20.3.60'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry_sp24_600,
                                         dst_mac_address="00:33:33:33:36:00")

        # add subport 400, 500 on port25, this port is tagged on vlan700
        self.subport25_400 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port25,
            admin_v4_state=True,
            outer_vlan_id=400)
        self.nhop_sp25_400 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.20.4.40'),
            router_interface_id=self.subport25_400,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_sp25_400 = sai_thrift_neighbor_entry_t(
            rif_id=self.subport25_400, ip_address=sai_ipaddress('20.20.4.40'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry_sp25_400,
                                         dst_mac_address="00:33:33:33:44:00")

        self.subport25_500 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port25,
            admin_v4_state=True,
            outer_vlan_id=500)
        self.nhop_sp25_500 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.20.4.50'),
            router_interface_id=self.subport25_500,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_sp25_500 = sai_thrift_neighbor_entry_t(
            rif_id=self.subport25_500, ip_address=sai_ipaddress('20.20.4.50'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry_sp25_500,
                                         dst_mac_address="00:33:33:33:45:00")

        self.route_entry0 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('30.30.0.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry0, next_hop_id=self.nhop0)

        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('30.30.1.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1, next_hop_id=self.nhop1)

        self.route_entry1_ipv6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:99aa/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1_ipv6, next_hop_id=self.nhop0)

        self.route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('30.30.2.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry2, next_hop_id=self.nhop2)

        self.route_entry3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('30.30.3.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry3, next_hop_id=self.nhop3)

        self.route_entry4 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('30.30.4.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry4, next_hop_id=self.nhop4)

        self.route_entry_sp10_100 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('40.40.0.10/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sp10_100,
                                      next_hop_id=self.nhop_sp10_100)

        self.route_entry_sp10_100_ipv6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:8899/128'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sp10_100_ipv6,
                                      next_hop_id=self.nhop_sp10_100)

        self.route_entry_sp10_200 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('40.40.0.20/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sp10_200,
                                      next_hop_id=self.nhop_sp10_200)

        self.route_entry_sp11_200 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('40.40.1.20/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sp11_200,
                                      next_hop_id=self.nhop_sp11_200)

        self.route_entry_sp11_300 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('40.40.1.30/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sp11_300,
                                      next_hop_id=self.nhop_sp11_300)

        self.route_entry_sl3_400 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('40.40.0.40/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sl3_400,
                                      next_hop_id=self.nhop_sl3_400)

        self.route_entry_sl3_500 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('40.40.0.50/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sl3_500,
                                      next_hop_id=self.nhop_sl3_500)

        self.route_entry_sp24_600 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('40.40.3.60/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sp24_600,
                                      next_hop_id=self.nhop_sp24_600)

        self.route_entry_sp25_400 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('40.40.4.40/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sp25_400,
                                      next_hop_id=self.nhop_sp25_400)

        self.route_entry_sp25_500 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('40.40.4.50/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sp25_500,
                                      next_hop_id=self.nhop_sp25_500)

        # ECMP
        self.nhop_group = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nhop_group_member1 = sai_thrift_create_next_hop_group_member(
            self.client, next_hop_group_id=self.nhop_group,
            next_hop_id=self.nhop_sp10_200)
        self.nhop_group_member2 = sai_thrift_create_next_hop_group_member(
            self.client, next_hop_group_id=self.nhop_group,
            next_hop_id=self.nhop_sp11_200)
        self.nhop_group_member3 = sai_thrift_create_next_hop_group_member(
            self.client, next_hop_group_id=self.nhop_group,
            next_hop_id=self.nhop_sl3_400)
        self.nhop_group_member4 = sai_thrift_create_next_hop_group_member(
            self.client, next_hop_group_id=self.nhop_group,
            next_hop_id=self.nhop_sp24_600)
        self.nhop_group_member5 = sai_thrift_create_next_hop_group_member(
            self.client, next_hop_group_id=self.nhop_group,
            next_hop_id=self.nhop_sp25_500)

        self.route_entry_ecmp = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('60.60.60.0/16'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry_ecmp, next_hop_id=self.nhop_group)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry_ecmp)
        sai_thrift_remove_next_hop_group_member(
            self.client, self.nhop_group_member1)
        sai_thrift_remove_next_hop_group_member(
            self.client, self.nhop_group_member2)
        sai_thrift_remove_next_hop_group_member(
            self.client, self.nhop_group_member3)
        sai_thrift_remove_next_hop_group_member(
            self.client, self.nhop_group_member4)
        sai_thrift_remove_next_hop_group_member(
            self.client, self.nhop_group_member5)
        sai_thrift_remove_next_hop_group(self.client, self.nhop_group)

        sai_thrift_remove_route_entry(self.client, self.route_entry_sp25_500)
        sai_thrift_remove_route_entry(self.client, self.route_entry_sp25_400)
        sai_thrift_remove_route_entry(self.client, self.route_entry_sp24_600)
        sai_thrift_remove_route_entry(self.client, self.route_entry_sp10_100)
        sai_thrift_remove_route_entry(
            self.client, self.route_entry_sp10_100_ipv6)
        sai_thrift_remove_route_entry(self.client, self.route_entry_sp10_200)
        sai_thrift_remove_route_entry(self.client, self.route_entry_sp11_200)
        sai_thrift_remove_route_entry(self.client, self.route_entry_sp11_300)
        sai_thrift_remove_route_entry(self.client, self.route_entry_sl3_400)
        sai_thrift_remove_route_entry(self.client, self.route_entry_sl3_500)
        sai_thrift_remove_route_entry(self.client, self.route_entry4)
        sai_thrift_remove_route_entry(self.client, self.route_entry3)
        sai_thrift_remove_route_entry(self.client, self.route_entry2)
        sai_thrift_remove_route_entry(self.client, self.route_entry1)
        sai_thrift_remove_route_entry(self.client, self.route_entry1_ipv6)
        sai_thrift_remove_route_entry(self.client, self.route_entry0)

        sai_thrift_remove_neighbor_entry(
            self.client, self.neighbor_entry_sp24_600)
        sai_thrift_remove_next_hop(self.client, self.nhop_sp24_600)
        sai_thrift_remove_router_interface(self.client, self.subport24_600)

        sai_thrift_remove_neighbor_entry(
            self.client, self.neighbor_entry_sp25_500)
        sai_thrift_remove_neighbor_entry(
            self.client, self.neighbor_entry_sp25_400)
        sai_thrift_remove_next_hop(self.client, self.nhop_sp25_500)
        sai_thrift_remove_next_hop(self.client, self.nhop_sp25_400)
        sai_thrift_remove_router_interface(self.client, self.subport25_500)
        sai_thrift_remove_router_interface(self.client, self.subport25_400)

        sai_thrift_remove_neighbor_entry(
            self.client, self.neighbor_entry_sl3_500)
        sai_thrift_remove_neighbor_entry(
            self.client, self.neighbor_entry_sl3_400)
        sai_thrift_remove_next_hop(self.client, self.nhop_sl3_500)
        sai_thrift_remove_next_hop(self.client, self.nhop_sl3_400)
        sai_thrift_remove_router_interface(self.client, self.sublag3_500)
        sai_thrift_remove_router_interface(self.client, self.sublag3_400)

        sai_thrift_remove_neighbor_entry(
            self.client, self.neighbor_entry_sp11_300)
        sai_thrift_remove_neighbor_entry(
            self.client, self.neighbor_entry_sp11_200)
        sai_thrift_remove_next_hop(self.client, self.nhop_sp11_300)
        sai_thrift_remove_next_hop(self.client, self.nhop_sp11_200)
        sai_thrift_remove_router_interface(self.client, self.subport11_300)
        sai_thrift_remove_router_interface(self.client, self.subport11_200)

        sai_thrift_remove_neighbor_entry(
            self.client, self.neighbor_entry_sp10_200)
        sai_thrift_remove_neighbor_entry(
            self.client, self.neighbor_entry_sp10_100)
        sai_thrift_remove_next_hop(self.client, self.nhop_sp10_200)
        sai_thrift_remove_next_hop(self.client, self.nhop_sp10_100)
        sai_thrift_remove_router_interface(self.client, self.subport10_200)
        sai_thrift_remove_router_interface(self.client, self.subport10_100)

        sai_thrift_set_port_attribute(self.client, self.port24, port_vlan_id=1)

        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry4)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry3)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry4)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry3)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry0)

        sai_thrift_remove_next_hop(self.client, self.nhop4)
        sai_thrift_remove_next_hop(self.client, self.nhop3)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_next_hop(self.client, self.nhop0)

        sai_thrift_remove_router_interface(self.client, self.vlan700_rif)
        sai_thrift_remove_router_interface(self.client, self.vlan600_rif)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member701)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member601)
        sai_thrift_remove_vlan(self.client, self.vlan700)
        sai_thrift_remove_vlan(self.client, self.vlan600)

        sai_thrift_remove_bridge_port(self.client, self.port25_bp)
        sai_thrift_remove_bridge_port(self.client, self.port24_bp)

        super(L3SubPortTestHelper, self).tearDown()

@group("draft")
class PvMissTest(L3SubPortTestHelper):
    """
    Verifies packet dropped when invalid vlan tag on port
    """
    def setUp(self):
        super(PvMissTest, self).setUp()

    def runTest(self):
        print("\npvMissTest()")

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='30.30.0.1',
                                ip_ttl=64,
                                dl_vlan_enable=True,
                                vlan_vid=100,
                                pktlen=104)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='30.30.0.1',
                                    ip_ttl=63,
                                    pktlen=100)

        pkt[Dot1Q].vlan = 400
        send_packet(self, self.dev_port25, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.subport25_400_in += 1
        self.port10_rif_out += 1

        pkt[Dot1Q].vlan = 500
        send_packet(self, self.dev_port25, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.subport25_500_in += 1
        self.port10_rif_out += 1

        pkt[Dot1Q].vlan = 700
        send_packet(self, self.dev_port25, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.vlan700_rif_in += 1
        self.port10_rif_out += 1

        pkt[Dot1Q].vlan = 40
        send_packet(self, self.dev_port25, pkt)
        verify_no_other_packets(self, timeout=1)

        pkt[Dot1Q].vlan = 100
        send_packet(self, self.dev_port25, pkt)
        verify_no_other_packets(self, timeout=1)

        pkt[Dot1Q].vlan = 200
        send_packet(self, self.dev_port25, pkt)
        verify_no_other_packets(self, timeout=1)

    def tearDown(self):
        super(PvMissTest, self).tearDown()

@group("draft")
class SubPortToSubPortTest(L3SubPortTestHelper):
    """
    Verifies routing between sub-ports and between sub-ports
    on the same physical port or LAG
    """
    def setUp(self):
        super(SubPortToSubPortTest, self).setUp()

    def runTest(self):
        print("\nsubPortToSubPortTest()")

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='40.40.0.10',
                                ip_src='30.30.0.1',
                                ip_id=105,
                                ip_ttl=64,
                                dl_vlan_enable=True,
                                vlan_vid=100,
                                pktlen=104)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='40.40.0.10',
                                    ip_src='30.30.0.1',
                                    ip_id=105,
                                    ip_ttl=63,
                                    dl_vlan_enable=True,
                                    vlan_vid=100,
                                    pktlen=104)

        pkt_data = [
            ['40.40.0.10', '00:33:33:33:01:00', 100, [10],
             self.subport10_100],
            ['40.40.0.20', '00:33:33:33:02:00', 200, [10],
             self.subport10_200],
            ['40.40.1.20', '00:33:33:33:12:00', 200, [11],
             self.subport11_200],
            ['40.40.1.30', '00:33:33:33:13:00', 300, [11],
             self.subport11_300],
            ['40.40.0.40', '00:33:33:33:04:00', 400, [14, 15, 16],
             self.sublag3_400],
            ['40.40.0.50', '00:33:33:33:05:00', 500, [14, 15, 16],
             self.sublag3_500],
            ['40.40.3.60', '00:33:33:33:36:00', 600, [24],
             self.subport24_600],
            ['40.40.4.40', '00:33:33:33:44:00', 400, [25],
             self.subport25_400],
            ['40.40.4.50', '00:33:33:33:45:00', 500, [25],
             self.subport25_500],
        ]
        for irif in pkt_data:
            for erif in pkt_data:
                if irif[4] == erif[4]:
                    continue
                pkt[IP].dst = erif[0]
                pkt[Dot1Q].vlan = irif[2]
                exp_pkt[IP].dst = erif[0]
                exp_pkt[Ether].dst = erif[1]
                exp_pkt[Dot1Q].vlan = erif[2]
                iport = getattr(self, 'dev_port%s' % irif[3][0])
                eport = [getattr(self, 'dev_port%s' % i) for i in erif[3]]
                send_packet(self, iport, pkt)
                verify_packet_any_port(self, exp_pkt, eport)
        self.subport10_100_in += 8
        self.subport10_200_in += 8
        self.subport11_200_in += 8
        self.subport11_300_in += 8
        self.sublag3_400_in += 8
        self.sublag3_500_in += 8
        self.subport24_600_in += 8
        self.subport25_400_in += 8
        self.subport25_500_in += 8
        self.subport10_100_out += 8
        self.subport10_200_out += 8
        self.subport11_200_out += 8
        self.subport11_300_out += 8
        self.sublag3_400_out += 8
        self.sublag3_500_out += 8
        self.subport24_600_out += 8
        self.subport25_400_out += 8
        self.subport25_500_out += 8

    def tearDown(self):
        super(SubPortToSubPortTest, self).tearDown()

@group("draft")
class SubPortToRifTest(L3SubPortTestHelper):
    """
    Verifies routing between SVI and sub-port
    """
    def setUp(self):
        super(SubPortToRifTest, self).setUp()

    def runTest(self):
        print("\nsubPortToRifTest()")

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='30.30.0.1',
                                ip_id=105,
                                ip_ttl=64,
                                dl_vlan_enable=True,
                                vlan_vid=100,
                                pktlen=104)
        exp_pkt = simple_tcp_packet(eth_dst='00:33:33:33:01:00',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='30.30.0.1',
                                    ip_id=105,
                                    ip_ttl=63,
                                    pktlen=100)
        exp_tagged_pkt = simple_tcp_packet(eth_dst='00:33:33:33:01:00',
                                           eth_src=ROUTER_MAC,
                                           ip_dst='30.30.0.1',
                                           ip_id=105,
                                           ip_ttl=63,
                                           dl_vlan_enable=True,
                                           vlan_vid=700,
                                           pktlen=104)

        pkt_data = [
            ['30.30.0.1', '00:11:22:33:44:55', 10],
            ['30.30.1.1', '00:22:22:33:44:55', 11],
            ['30.30.2.1', '00:33:22:33:44:55', 15],
            ['30.30.3.1', '00:44:22:33:44:55', 24],
        ]
        ingress_rifs = [[10, 100], [10, 200], [11, 200], [11, 300],
                        [14, 400], [14, 500],
                        [24, 600], [25, 400], [25, 500]]
        for item in ingress_rifs:
            for content in pkt_data:
                pkt[IP].dst = content[0]
                pkt[Dot1Q].vlan = item[1]
                exp_pkt[IP].dst = content[0]
                exp_pkt[Ether].dst = content[1]
                iport = getattr(self, 'dev_port%s' % item[0])
                eport = getattr(self, 'dev_port%s' % content[2])
                send_packet(self, iport, pkt)
                verify_packet(self, exp_pkt, eport)
        pkt_data = [
            ['30.30.4.1', '00:55:22:33:44:55', 25],
        ]
        for item in ingress_rifs:
            for content in pkt_data:
                pkt[IP].dst = content[0]
                pkt[Dot1Q].vlan = item[1]
                exp_tagged_pkt[IP].dst = content[0]
                exp_tagged_pkt[Ether].dst = content[1]
                iport = getattr(self, 'dev_port%s' % item[0])
                eport = getattr(self, 'dev_port%s' % content[2])
                send_packet(self, iport, pkt)
                verify_packet(self, exp_tagged_pkt, eport)
        self.subport10_100_in += 5
        self.subport10_200_in += 5
        self.subport11_200_in += 5
        self.subport11_300_in += 5
        self.sublag3_400_in += 5
        self.sublag3_500_in += 5
        self.subport24_600_in += 5
        self.subport25_400_in += 5
        self.subport25_500_in += 5
        self.port10_rif_out += 9
        self.port11_rif_out += 9
        self.lag3_rif_out += 9
        self.vlan600_rif_out += 9
        self.vlan700_rif_out += 9

    def tearDown(self):
        super(SubPortToRifTest, self).tearDown()

@group("draft")
class RifToSubPortTest(L3SubPortTestHelper):
    """
    Verifies packet routed with valid vlan on sub-port
    and routing between L3 RIF and sub-port
    """
    def setUp(self):
        super(RifToSubPortTest, self).setUp()

    def runTest(self):
        print("\nrifToSubPortTest()")

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='40.40.0.10',
                                ip_src='30.30.0.1',
                                ip_id=105,
                                ip_ttl=64,
                                pktlen=100)
        tagged_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                       eth_src='00:22:22:22:22:22',
                                       ip_dst='40.40.0.10',
                                       ip_src='30.30.0.1',
                                       ip_id=105,
                                       ip_ttl=64,
                                       dl_vlan_enable=True,
                                       vlan_vid=700,
                                       pktlen=104)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='40.40.0.10',
                                    ip_src='30.30.0.1',
                                    ip_id=105,
                                    ip_ttl=63,
                                    dl_vlan_enable=True,
                                    vlan_vid=100,
                                    pktlen=104)

        pkt_data = [
            ['40.40.0.10', '00:33:33:33:01:00', 100, [10],
             'subport10_100'],
            ['40.40.0.20', '00:33:33:33:02:00', 200, [10],
             'subport10_200'],
            ['40.40.1.20', '00:33:33:33:12:00', 200, [11],
             'subport11_200'],
            ['40.40.1.30', '00:33:33:33:13:00', 300, [11],
             'subport11_300'],
            ['40.40.0.40', '00:33:33:33:04:00', 400, [14, 15, 16],
             'sublag3_400'],
            ['40.40.0.50', '00:33:33:33:05:00', 500, [14, 15, 16],
             'sublag3_500'],
            ['40.40.3.60', '00:33:33:33:36:00', 600, [24],
             'subport24_600'],
            ['40.40.4.40', '00:33:33:33:44:00', 400, [25],
             'subport25_400'],
            ['40.40.4.50', '00:33:33:33:45:00', 500, [25],
             'subport25_500'],
        ]
        ingress_rifs = [10, 11, 15, 24]
        for port in ingress_rifs:
            for content in pkt_data:
                pkt[IP].dst = content[0]
                exp_pkt[IP].dst = content[0]
                exp_pkt[Ether].dst = content[1]
                exp_pkt[Dot1Q].vlan = content[2]
                iport = getattr(self, 'dev_port%s' % port)
                eport = [getattr(self, 'dev_port%s' % i)
                         for i in content[3]]
                send_packet(self, iport, pkt)
                verify_packet_any_port(self, exp_pkt, eport)

        for content in pkt_data:
            tagged_pkt[IP].dst = content[0]
            exp_pkt[IP].dst = content[0]
            exp_pkt[Ether].dst = content[1]
            exp_pkt[Dot1Q].vlan = content[2]
            eport = [getattr(self, 'dev_port%s' % i) for i in content[3]]
            send_packet(self, self.dev_port25, tagged_pkt)
            verify_packet_any_port(self, exp_pkt, eport)
        self.subport10_100_out += 5
        self.subport10_200_out += 5
        self.subport11_200_out += 5
        self.subport11_300_out += 5
        self.sublag3_400_out += 5
        self.sublag3_500_out += 5
        self.subport24_600_out += 5
        self.subport25_400_out += 5
        self.subport25_500_out += 5
        self.port10_rif_in += 9
        self.port11_rif_in += 9
        self.lag3_rif_in += 9
        self.vlan600_rif_in += 9
        self.vlan700_rif_in += 9

    def tearDown(self):
        super(RifToSubPortTest, self).tearDown()

@group("draft")
class NoFloodTest(L3SubPortTestHelper):
    """
    Verifies if packet is not flooded on the tagged VLAN when no route hit
    """
    def setUp(self):
        super(NoFloodTest, self).setUp()

    def runTest(self):
        print("\nnoFloodTest()")

        pkt = simple_tcp_packet(eth_dst='00:66:66:55:44:33',
                                eth_src='00:22:22:22:22:22',
                                ip_dst='50.50.0.1',
                                ip_ttl=64)
        tagged_pkt = simple_tcp_packet(eth_dst='00:66:66:55:44:33',
                                       eth_src='00:22:22:22:22:22',
                                       ip_dst='50.50.0.1',
                                       ip_ttl=64,
                                       dl_vlan_enable=True,
                                       vlan_vid=600,
                                       pktlen=104)

        self.port30_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port30,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        port31_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port31,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        vlan_member602 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan600,
            bridge_port_id=self.port30_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member603 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan600,
            bridge_port_id=port31_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        try:
            # ingress on vlan rif 600
            send_packet(self, self.dev_port24, pkt)
            verify_packets(self, pkt, [self.dev_port30, self.dev_port31])
            self.vlan600_rif_in += 1
            self.vlan600_rif_out += 2

            # ingress on subport rif 600 with rmac
            send_packet(self, self.dev_port24, tagged_pkt)
            verify_no_other_packets(self, timeout=1)
            self.subport24_600_in += 1

            # ingress on subport rif 600 with unknown unicast
            tagged_pkt[Ether].dst = '00:66:66:55:44:33'
            send_packet(self, self.dev_port24, tagged_pkt)
            verify_no_other_packets(self, timeout=1)
            self.subport24_600_in += 1

        finally:
            sai_thrift_remove_vlan_member(self.client, vlan_member602)
            sai_thrift_remove_vlan_member(self.client, vlan_member603)
            sai_thrift_remove_bridge_port(self.client, self.port30_bp)
            sai_thrift_remove_bridge_port(self.client, port31_bp)

    def tearDown(self):
        super(NoFloodTest, self).tearDown()

@group("draft")
class VlanConflictTest(L3SubPortTestHelper):
    """
    Verifies if deleting VLAN RIF affects sub-port RIF with same
    vlan number
    """
    def setUp(self):
        super(VlanConflictTest, self).setUp()

    def runTest(self):
        print("\nvlanConflictTest()")

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='30.30.0.1',
                                ip_ttl=64,
                                pktlen=100)
        tagged_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                       eth_src='00:22:22:22:22:22',
                                       ip_dst='30.30.0.1',
                                       ip_ttl=64,
                                       dl_vlan_enable=True,
                                       vlan_vid=800,
                                       pktlen=104)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='30.30.0.1',
                                    ip_ttl=63,
                                    pktlen=100)

        try:
            port29_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=self.port29,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)

            print("Add vlan 800 on port 29")
            vlan800 = sai_thrift_create_vlan(self.client, vlan_id=800)
            vlan_member801 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=vlan800,
                bridge_port_id=port29_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan800_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
                virtual_router_id=self.default_vrf,
                vlan_id=vlan800)
            sai_thrift_set_port_attribute(
                self.client, self.port29, port_vlan_id=800)

            print("Send untagged pkt on vlan 800 to port %d" % self.dev_port10)
            send_packet(self, self.dev_port29, pkt)
            verify_packet(self, exp_pkt, self.dev_port10)
            self.port10_rif_out += 1

            print("Add subport 800 on port 25")
            subport8_800 = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
                virtual_router_id=self.default_vrf,
                port_id=self.port25,
                admin_v4_state=True,
                outer_vlan_id=800)

            print("Send untagged pkt on vlan 800 to port %d" % self.dev_port10)
            send_packet(self, self.dev_port29, pkt)
            verify_packet(self, exp_pkt, self.dev_port10)
            self.port10_rif_out += 1

            print("Send tagged pkt on subport 800 to port %d"
                  % self.dev_port10)
            send_packet(self, self.dev_port25, tagged_pkt)
            verify_packet(self, exp_pkt, self.dev_port10)
            self.port10_rif_out += 1

            sai_thrift_remove_router_interface(self.client, subport8_800)

            print("Send untagged pkt on vlan 800 to port %d" % self.dev_port10)
            send_packet(self, self.dev_port29, pkt)
            verify_packet(self, exp_pkt, self.dev_port10)
            self.port10_rif_out += 1

            sai_thrift_set_port_attribute(
                self.client, self.port29, port_vlan_id=0)
            sai_thrift_remove_router_interface(self.client, vlan800_rif)
            sai_thrift_remove_vlan_member(self.client, vlan_member801)

            print("Add subport 800 on port 25")
            subport8_800 = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
                virtual_router_id=self.default_vrf,
                port_id=self.port25,
                admin_v4_state=True,
                outer_vlan_id=800)

            print("Send tagged pkt on subport 800 to port %d"
                  % self.dev_port10)
            send_packet(self, self.dev_port25, tagged_pkt)
            verify_packet(self, exp_pkt, self.dev_port10)
            self.port10_rif_out += 1

            print("Add vlan 800 on port 29")
            vlan_member801 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=vlan800,
                bridge_port_id=port29_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan800_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
                virtual_router_id=self.default_vrf,
                vlan_id=vlan800)
            sai_thrift_set_port_attribute(
                self.client, self.port29, port_vlan_id=800)

            print("Send untagged pkt on vlan 800 to port %d" % self.dev_port10)
            send_packet(self, self.dev_port29, pkt)
            verify_packet(self, exp_pkt, self.dev_port10)
            self.port10_rif_out += 1

            print("Send tagged pkt on subport 800 to port %d"
                  % self.dev_port10)
            send_packet(self, self.dev_port25, tagged_pkt)
            verify_packet(self, exp_pkt, self.dev_port10)
            self.port10_rif_out += 1

            sai_thrift_set_port_attribute(
                self.client, self.port29, port_vlan_id=0)
            sai_thrift_remove_router_interface(self.client, vlan800_rif)
            sai_thrift_remove_vlan_member(self.client, vlan_member801)

            print("Send tagged pkt on subport 800 to port %d"
                  % self.dev_port10)
            send_packet(self, self.dev_port25, tagged_pkt)
            verify_packet(self, exp_pkt, self.dev_port10)
            self.port10_rif_out += 1

        finally:
            sai_thrift_remove_bridge_port(self.client, port29_bp)
            sai_thrift_remove_vlan(self.client, vlan800)
            sai_thrift_remove_router_interface(self.client, subport8_800)

    def tearDown(self):
        super(VlanConflictTest, self).tearDown()

@group("draft")
class SubPortAdminV4StatusTest(L3SubPortTestHelper):
    """
    Verifies if admin status DOWN disables packet forwarding on ingress
    and if IPv4 packets are dropped when admin_v4_state is False
    """
    def setUp(self):
        super(SubPortAdminV4StatusTest, self).setUp()

    def runTest(self):
        print("\nsubPortAdminV4StatusTest()")

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='30.30.0.1',
                                ip_ttl=64,
                                dl_vlan_enable=True,
                                vlan_vid=200,
                                pktlen=104)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='30.30.0.1',
                                    ip_ttl=63,
                                    pktlen=100)

        print("Sending packet on sub port %d vlan 200, forward"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.subport11_200_in += 1
        self.port10_rif_out += 1

        print("Disable IPv4 on subport")
        sai_thrift_set_router_interface_attribute(
            self.client, self.subport11_200, admin_v4_state=False)
        print("Sending packet on sub port %d vlan 200, drop"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_no_other_packets(self, timeout=1)
        self.subport11_200_in += 1

        print("Enable IPv4 on subport")
        sai_thrift_set_router_interface_attribute(
            self.client, self.subport11_200, admin_v4_state=True)
        print("Sending packet on sub port %d vlan 200, forward"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.subport11_200_in += 1
        self.port10_rif_out += 1

    def tearDown(self):
        super(SubPortAdminV4StatusTest, self).tearDown()

@group("draft")
class SubPortAdminV6StatusTest(L3SubPortTestHelper):
    """
    Verifies if admin status DOWN disables packet forwarding on ingress
    and if IPv6 packets are dropped when admin_v6_state is False
    """
    def setUp(self):
        super(SubPortAdminV6StatusTest, self).setUp()

    def runTest(self):
        print("\nsubPortAdminV6StatusTest()")

        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_src='2000::1',
            dl_vlan_enable=True,
            vlan_vid=200,
            ipv6_hlim=64,
            pktlen=40 + 14)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_src='2000::1',
            ipv6_hlim=63,
            pktlen=40 + 14)

        print("Sending packet on sub port %d vlan 200, forward"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.subport11_200_in += 1
        self.port10_rif_out += 1

        print("Disable IPv6 on subport")
        sai_thrift_set_router_interface_attribute(
            self.client, self.subport11_200, admin_v6_state=False)
        print("Sending packet on sub port %d vlan 200, drop"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_no_other_packets(self, timeout=1)
        self.subport11_200_in += 1

        print("Enable IPv6 on subport")
        sai_thrift_set_router_interface_attribute(
            self.client, self.subport11_200, admin_v6_state=True)
        print("Sending packet on sub port %d vlan 200, forward"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.subport11_200_in += 1
        self.port10_rif_out += 1

    def tearDown(self):
        super(SubPortAdminV6StatusTest, self).tearDown()

@group("draft")
class SubPortV4MtuTest(L3SubPortTestHelper):
    """
    Verifies if IPv4 packet is forwarded, dropped and punted to CPU
    depending on the MTU with and without a trap for L3 subport
    """
    def setUp(self):
        super(SubPortV4MtuTest, self).setUp()

    def runTest(self):
        print("\nsubPortV4MtuTest()")

        # set MTU to 200 for subport 10_100
        sai_thrift_set_router_interface_attribute(
            self.client, self.subport10_100, mtu=200)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='40.40.0.10',
                                ip_id=105,
                                ip_ttl=64,
                                dl_vlan_enable=True,
                                vlan_vid=200,
                                pktlen=199 + 18)
        exp_pkt = simple_tcp_packet(eth_dst='00:33:33:33:01:00',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='40.40.0.10',
                                    ip_id=105,
                                    ip_ttl=63,
                                    dl_vlan_enable=True,
                                    vlan_vid=100,
                                    pktlen=199 + 18)
        print("Max MTU is 200, send pkt size 199, send to port")
        print("Sending packet port %d -> port %d"
              % (self.dev_port11, self.dev_port10))
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.subport11_200_in += 1
        self.subport10_100_out += 1

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='40.40.0.10',
                                ip_id=105,
                                ip_ttl=64,
                                dl_vlan_enable=True,
                                vlan_vid=200,
                                pktlen=200 + 18)
        exp_pkt = simple_tcp_packet(eth_dst='00:33:33:33:01:00',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='40.40.0.10',
                                    ip_id=105,
                                    ip_ttl=63,
                                    dl_vlan_enable=True,
                                    vlan_vid=100,
                                    pktlen=200 + 18)
        print("Max MTU is 200, send pkt size 200, send to port")
        print("Sending packet port %d -> port %d"
              % (self.dev_port11, self.dev_port10))
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.subport11_200_in += 1
        self.subport10_100_out += 1

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='40.40.0.10',
                                ip_id=105,
                                ip_ttl=64,
                                dl_vlan_enable=True,
                                vlan_vid=200,
                                pktlen=201 + 18)
        exp_pkt = simple_tcp_packet(eth_dst='00:33:33:33:01:00',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='40.40.0.10',
                                    ip_id=105,
                                    ip_ttl=63,
                                    dl_vlan_enable=True,
                                    vlan_vid=100,
                                    pktlen=201 + 18)
        print("Max MTU is 200, send pkt size 201, drop")
        print("Sending packet port %d -> port %d"
              % (self.dev_port11, self.dev_port10))
        send_packet(self, self.dev_port11, pkt)
        verify_no_other_packets(self, timeout=1)
        self.subport11_200_in += 1
        self.subport10_100_out += 1

        print("Setting MTU to 201")
        sai_thrift_set_router_interface_attribute(
            self.client, self.subport10_100, mtu=201)

        print("Max MTU is 201, send pkt size 201, send to port")
        print("Sending packet port %d -> port %d"
              % (self.dev_port11, self.dev_port10))
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.subport11_200_in += 1
        self.subport10_100_out += 1

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='40.40.0.10',
                                ip_id=105,
                                ip_ttl=64,
                                dl_vlan_enable=True,
                                vlan_vid=200,
                                pktlen=202 + 18)
        exp_pkt = simple_tcp_packet(eth_dst='00:33:33:33:01:00',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='40.40.0.10',
                                    ip_id=105,
                                    ip_ttl=63,
                                    dl_vlan_enable=True,
                                    vlan_vid=100,
                                    pktlen=202 + 18)
        print("Max MTU is 201, send pkt size 202, drop")
        print("Sending packet port %d -> port %d"
              % (self.dev_port11, self.dev_port10))
        send_packet(self, self.dev_port11, pkt)
        verify_no_other_packets(self, timeout=1)
        self.subport11_200_in += 1
        self.subport10_100_out += 1

    def tearDown(self):
        super(SubPortV4MtuTest, self).tearDown()

@group("draft")
class SubPortV6MtuTest(L3SubPortTestHelper):
    """
    Verifies if IPv6 packet is forwarded, dropped and punted to CPU
    depending on the MTU with and without a trap for L3 subport
    """
    def setUp(self):
        super(SubPortV6MtuTest, self).setUp()

    def runTest(self):
        print("\nsubPortV6MtuTest()")

        # set MTU to 200 for subport 10_100
        sai_thrift_set_router_interface_attribute(
            self.client, self.subport10_100, mtu=200)

        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:8899',
            ipv6_src='2000::1',
            dl_vlan_enable=True,
            vlan_vid=200,
            ipv6_hlim=64,
            pktlen=199 + 40 + 18)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:33:33:33:01:00',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:8899',
            ipv6_src='2000::1',
            ipv6_hlim=63,
            dl_vlan_enable=True,
            vlan_vid=100,
            pktlen=199 + 40 + 18)

        print("Max MTU is 200, send pkt size 199, send to port")
        print("Sending packet port %d -> port %d"
              % (self.dev_port11, self.dev_port10))
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.subport11_200_in += 1
        self.subport10_100_out += 1

        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:8899',
            ipv6_src='2000::1',
            dl_vlan_enable=True,
            vlan_vid=200,
            ipv6_hlim=64,
            pktlen=200 + 40 + 18)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:33:33:33:01:00',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:8899',
            ipv6_src='2000::1',
            ipv6_hlim=63,
            dl_vlan_enable=True,
            vlan_vid=100,
            pktlen=200 + 40 + 18)

        print("Max MTU is 200, send pkt size 200, send to port")
        print("Sending packet port %d -> port %d"
              % (self.dev_port11, self.dev_port10))
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.subport11_200_in += 1
        self.subport10_100_out += 1

        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:8899',
            ipv6_src='2000::1',
            dl_vlan_enable=True,
            vlan_vid=200,
            ipv6_hlim=64,
            pktlen=201 + 40 + 18)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:33:33:33:01:00',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:8899',
            ipv6_src='2000::1',
            ipv6_hlim=63,
            dl_vlan_enable=True,
            vlan_vid=100,
            pktlen=201 + 40 + 18)

        print("Max MTU is 200, send pkt size 201, drop")
        print("Sending packet port %d -> port %d"
              % (self.dev_port11, self.dev_port10))
        send_packet(self, self.dev_port11, pkt)
        verify_no_other_packets(self, timeout=1)
        self.subport11_200_in += 1
        self.subport10_100_out += 1

        print("Setting MTU to 201")
        sai_thrift_set_router_interface_attribute(
            self.client, self.subport10_100, mtu=201)

        print("Max MTU is 201, send pkt size 201, send to port")
        print("Sending packet port %d -> port %d"
              % (self.dev_port11, self.dev_port10))
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)
        self.subport11_200_in += 1
        self.subport10_100_out += 1

        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:8899',
            ipv6_src='2000::1',
            dl_vlan_enable=True,
            vlan_vid=200,
            ipv6_hlim=64,
            pktlen=202 + 40 + 18)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:33:33:33:01:00',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:8899',
            ipv6_src='2000::1',
            ipv6_hlim=63,
            dl_vlan_enable=True,
            vlan_vid=100,
            pktlen=202 + 40 + 18)
        print("Max MTU is 201, send pkt size 202, drop")
        print("Sending packet port %d -> port %d"
              % (self.dev_port11, self.dev_port10))
        send_packet(self, self.dev_port11, pkt)
        verify_no_other_packets(self, timeout=1)
        self.subport11_200_in += 1
        self.subport10_100_out += 1

    def tearDown(self):
        super(SubPortV6MtuTest, self).tearDown()

@group("draft")
class SubPortIngressAclTest(L3SubPortTestHelper):
    """
    Verifies ingress ACL table and group is bound correctly to sub-port
    """
    def setUp(self):
        super(SubPortIngressAclTest, self).setUp()

    def runTest(self):
        print("\nsubPortIngressAclTest()")

        stage = SAI_ACL_STAGE_INGRESS
        bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE]
        action_types = [SAI_ACL_ACTION_TYPE_PACKET_ACTION]
        action_drop = SAI_PACKET_ACTION_DROP
        dst_ip = '40.40.1.20'
        dst_ip_mask = '255.255.255.0'

        acl_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(bind_points), int32list=bind_points)
        acl_action_type_list = sai_thrift_s32_list_t(
            count=len(action_types), int32list=action_types)
        acl_table = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=acl_bind_point_type_list,
            acl_action_type_list=acl_action_type_list,
            field_dst_ip=True)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(s32=action_drop))
        ip_addr = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=dst_ip),
            mask=sai_thrift_acl_field_data_mask_t(ip4=dst_ip_mask))

        acl_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table,
            action_packet_action=packet_action,
            field_dst_ip=ip_addr)

        try:
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='40.40.1.20',
                                    ip_ttl=64,
                                    dl_vlan_enable=True,
                                    vlan_vid=100)
            exp_pkt = simple_tcp_packet(eth_dst='00:33:33:33:12:00',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='40.40.1.20',
                                        ip_ttl=63,
                                        dl_vlan_enable=True,
                                        vlan_vid=200)
            print("Allow packet before binding ingress ACL to RIF")
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port11)
            self.subport10_100_in += 1
            self.subport11_200_out += 1

            print("Bind ACL to subport10_100")
            sai_thrift_set_router_interface_attribute(
                self.client, self.subport10_100, ingress_acl=acl_table)
            send_packet(self, self.dev_port10, pkt)
            verify_no_other_packets(self, timeout=1)
            self.subport10_100_in += 1

            print("Remove binding, packet should be forwarded")
            sai_thrift_set_router_interface_attribute(
                self.client, self.subport10_100, ingress_acl=0)
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port11)
            self.subport10_100_in += 1
            self.subport11_200_out += 1

            acl_table_group = sai_thrift_create_acl_table_group(
                self.client,
                acl_stage=stage,
                acl_bind_point_type_list=acl_bind_point_type_list,
                type=SAI_ACL_TABLE_GROUP_TYPE_PARALLEL)
            acl_group_member = sai_thrift_create_acl_table_group_member(
                self.client, acl_table_group_id=acl_table_group,
                acl_table_id=acl_table)

            print("Allow packet before binding ingress ACL group to RIF")
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port11)
            self.subport10_100_in += 1
            self.subport11_200_out += 1

            print("Bind ACL group to subport10_100")
            sai_thrift_set_router_interface_attribute(
                self.client, self.subport10_100, ingress_acl=acl_table_group)
            send_packet(self, self.dev_port10, pkt)
            verify_no_other_packets(self, timeout=1)
            self.subport10_100_in += 1

            print("Remove binding, packet should be forwarded")
            sai_thrift_set_router_interface_attribute(
                self.client, self.subport10_100, ingress_acl=0)
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port11)
            self.subport10_100_in += 1
            self.subport11_200_out += 1

        finally:
            sai_thrift_remove_acl_table_group_member(
                self.client, acl_group_member)
            sai_thrift_remove_acl_table_group(self.client, acl_table_group)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)

    def tearDown(self):
        super(SubPortIngressAclTest, self).tearDown()

@group("draft")
class SubPortEgressAclTest(L3SubPortTestHelper):
    """
    Verifies egress ACL table and group is bound correctly to sub-port
    """
    def setUp(self):
        super(SubPortEgressAclTest, self).setUp()

    def runTest(self):
        print("\nsubPortEgressAclTest()")

        stage = SAI_ACL_STAGE_EGRESS
        bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE]
        action_types = [SAI_ACL_ACTION_TYPE_PACKET_ACTION]
        action_drop = SAI_PACKET_ACTION_DROP
        dst_ip = '40.40.1.20'
        dst_ip_mask = '255.255.255.0'

        acl_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(bind_points), int32list=bind_points)
        acl_action_type_list = sai_thrift_s32_list_t(
            count=len(action_types), int32list=action_types)
        acl_table = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=acl_bind_point_type_list,
            acl_action_type_list=acl_action_type_list,
            field_dst_ip=True)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(s32=action_drop))
        ip_addr = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=dst_ip),
            mask=sai_thrift_acl_field_data_mask_t(ip4=dst_ip_mask))

        acl_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table,
            action_packet_action=packet_action,
            field_dst_ip=ip_addr)

        try:
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='40.40.1.20',
                                    ip_ttl=64,
                                    dl_vlan_enable=True,
                                    vlan_vid=100)
            exp_pkt = simple_tcp_packet(eth_dst='00:33:33:33:12:00',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='40.40.1.20',
                                        ip_ttl=63,
                                        dl_vlan_enable=True,
                                        vlan_vid=200)
            print("Allow packet before binding ingress ACL to RIF")
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port11)
            self.subport10_100_in += 1
            self.subport11_200_out += 1

            print("Bind ACL to subport11_200")
            sai_thrift_set_router_interface_attribute(
                self.client, self.subport11_200, egress_acl=acl_table)
            send_packet(self, self.dev_port10, pkt)
            verify_no_other_packets(self, timeout=1)
            self.subport10_100_in += 1
            self.subport11_200_out += 1

            print("Remove binding, packet should be forwarded")
            sai_thrift_set_router_interface_attribute(
                self.client, self.subport11_200, egress_acl=0)
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port11)
            self.subport10_100_in += 1
            self.subport11_200_out += 1

            acl_table_group = sai_thrift_create_acl_table_group(
                self.client,
                acl_stage=stage,
                acl_bind_point_type_list=acl_bind_point_type_list,
                type=SAI_ACL_TABLE_GROUP_TYPE_PARALLEL)
            acl_group_member = sai_thrift_create_acl_table_group_member(
                self.client, acl_table_group_id=acl_table_group,
                acl_table_id=acl_table)

            print("Allow packet before binding ingress ACL group to RIF")
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port11)
            self.subport10_100_in += 1
            self.subport11_200_out += 1

            print("Bind ACL group to subport0_100")
            sai_thrift_set_router_interface_attribute(
                self.client, self.subport11_200, egress_acl=acl_table_group)
            send_packet(self, self.dev_port10, pkt)
            verify_no_other_packets(self, timeout=1)
            self.subport10_100_in += 1
            self.subport11_200_out += 1

            print("Remove binding, packet should be forwarded")
            sai_thrift_set_router_interface_attribute(
                self.client, self.subport11_200, egress_acl=0)
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port11)
            self.subport10_100_in += 1
            self.subport11_200_out += 1

        finally:
            sai_thrift_remove_acl_table_group_member(
                self.client, acl_group_member)
            sai_thrift_remove_acl_table_group(self.client, acl_table_group)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)

    def tearDown(self):
        super(SubPortEgressAclTest, self).tearDown()

@group("draft")
class SubPortECMPTest(L3SubPortTestHelper):
    """
    Verifies load-balancing when sub-port is part of ECMP
    """
    def setUp(self):
        super(SubPortECMPTest, self).setUp()

    def runTest(self):
        print("\nsubPortECMPTest()")

        count = [0, 0, 0, 0, 0, 0, 0]
        dst_ip = int(binascii.hexlify(socket.inet_aton('60.60.60.1')), 16)
        src_ip = int(binascii.hexlify(socket.inet_aton('192.168.8.1')), 16)
        max_itrs = 200
        for i in range(0, max_itrs):
            dst_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(format(dst_ip, 'x').zfill(8)))
            src_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(format(src_ip, 'x').zfill(8)))

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst=dst_ip_addr,
                                    ip_src=src_ip_addr,
                                    ip_ttl=64,
                                    pktlen=100)

            exp_pkt1 = simple_tcp_packet(eth_dst='00:33:33:33:02:00',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src=src_ip_addr,
                                         dl_vlan_enable=True,
                                         vlan_vid=200,
                                         ip_ttl=63,
                                         pktlen=104)

            exp_pkt2 = simple_tcp_packet(eth_dst='00:33:33:33:12:00',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src=src_ip_addr,
                                         dl_vlan_enable=True,
                                         vlan_vid=200,
                                         ip_ttl=63,
                                         pktlen=104)

            exp_pkt3 = simple_tcp_packet(eth_dst='00:33:33:33:04:00',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src=src_ip_addr,
                                         dl_vlan_enable=True,
                                         vlan_vid=400,
                                         ip_ttl=63,
                                         pktlen=104)

            exp_pkt4 = simple_tcp_packet(eth_dst='00:33:33:33:36:00',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src=src_ip_addr,
                                         dl_vlan_enable=True,
                                         vlan_vid=600,
                                         ip_ttl=63,
                                         pktlen=104)

            exp_pkt5 = simple_tcp_packet(eth_dst='00:33:33:33:45:00',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src=src_ip_addr,
                                         dl_vlan_enable=True,
                                         vlan_vid=500,
                                         ip_ttl=63,
                                         pktlen=104)

            send_packet(self, self.dev_port10, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4, exp_pkt5],
                [self.dev_port10, self.dev_port11, self.dev_port24,
                 self.dev_port25, self.dev_port14, self.dev_port15,
                 self.dev_port16],
                timeout=2)
            count[rcv_idx] += 1
            dst_ip += 1
            src_ip += 1

        print('count: ', count)
        ecmp_count = [count[0], count[1], count[2], count[3],
                      count[4] + count[5] + count[6]]
        for i in range(0, 5):
            self.assertTrue((ecmp_count[i] >= ((max_itrs / 5) * 0.6)),
                            "ECMP paths are not equally balanced")
        self.port10_rif_in += 200
        self.subport10_200_out += ecmp_count[0]
        self.subport11_200_out += ecmp_count[1]
        self.subport24_600_out += ecmp_count[2]
        self.subport25_500_out += ecmp_count[3]
        self.sublag3_400_out += ecmp_count[4]

    def tearDown(self):
        super(SubPortECMPTest, self).tearDown()

@group("draft")
class SubPortMyIPTest(L3SubPortTestHelper):
    """
    Verifies if MYIP works for subnet routes
    """
    def setUp(self):
        super(SubPortMyIPTest, self).setUp()

    def runTest(self):
        print("\nsubPortMyIPTest()")

        try:
            # add IP2ME routes
            sw_attr = sai_thrift_get_switch_attribute(
                self.client, cpu_port=True)
            cpu_port = sw_attr['cpu_port']

            ip2me_route0 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('40.40.0.11/32'))
            sai_thrift_create_route_entry(
                self.client, ip2me_route0, next_hop_id=cpu_port)
            ip2me_route1 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('40.40.0.21/32'))
            sai_thrift_create_route_entry(
                self.client, ip2me_route1, next_hop_id=cpu_port)
            ip2me_route2 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('40.40.1.21/32'))
            sai_thrift_create_route_entry(
                self.client, ip2me_route2, next_hop_id=cpu_port)
            ip2me_route3 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('40.40.1.31/32'))
            sai_thrift_create_route_entry(
                self.client, ip2me_route3, next_hop_id=cpu_port)
            ip2me_route4 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('40.40.0.41/32'))
            sai_thrift_create_route_entry(
                self.client, ip2me_route4, next_hop_id=cpu_port)
            ip2me_route5 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('40.40.0.51/32'))
            sai_thrift_create_route_entry(
                self.client, ip2me_route5, next_hop_id=cpu_port)
            ip2me_route6 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('40.40.3.61/32'))
            sai_thrift_create_route_entry(
                self.client, ip2me_route6, next_hop_id=cpu_port)
            ip2me_route7 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('40.40.4.41/32'))
            sai_thrift_create_route_entry(
                self.client, ip2me_route7, next_hop_id=cpu_port)
            ip2me_route8 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('40.40.4.51/32'))
            sai_thrift_create_route_entry(
                self.client, ip2me_route8, next_hop_id=cpu_port)

            myip_trap_group = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True, queue=4)
            myip_trap = sai_thrift_create_hostif_trap(
                self.client,
                trap_group=myip_trap_group,
                packet_action=SAI_PACKET_ACTION_TRAP,
                trap_type=SAI_HOSTIF_TRAP_TYPE_IP2ME)
            self.assertTrue(myip_trap != 0)

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='40.40.0.11',
                                    ip_src='30.30.0.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    dl_vlan_enable=True,
                                    vlan_vid=100,
                                    pktlen=104)

            pkt_data = [
                ['40.40.0.11', 100, 10],  # self.subport10_100
                ['40.40.0.21', 200, 10],  # self.subport10_200
                ['40.40.1.21', 200, 11],  # self.subport11_200
                ['40.40.1.31', 300, 11],  # self.subport11_300
                ['40.40.0.41', 400, 14],  # self.sublag3_400
                ['40.40.0.51', 500, 15],  # self.sublag3_500
                ['40.40.3.61', 600, 24],  # self.subport24_600
                ['40.40.4.41', 400, 25],  # self.subport25_400
                ['40.40.4.51', 500, 25]   # self.subport25_500
            ]

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            for data in pkt_data:
                ingress_port = getattr(self, 'dev_port%s' % data[2])

                pkt[IP].dst = data[0]
                pkt[Dot1Q].vlan = data[1]

                send_packet(self, ingress_port, pkt)

            time.sleep(6)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + len(pkt_data))

            self.subport10_100_in += 1
            self.subport10_200_in += 1
            self.subport11_200_in += 1
            self.subport11_300_in += 1
            self.sublag3_400_in += 1
            self.sublag3_500_in += 1
            self.subport24_600_in += 1
            self.subport25_400_in += 1
            self.subport25_500_in += 1
            self.subport10_100_out += 1
            self.subport10_200_out += 1
            self.subport11_200_out += 1
            self.subport11_300_out += 1
            self.sublag3_400_out += 1
            self.sublag3_500_out += 1
            self.subport24_600_out += 1
            self.subport25_400_out += 1
            self.subport25_500_out += 1

        finally:
            sai_thrift_remove_hostif_trap(self.client, myip_trap)
            sai_thrift_remove_hostif_trap_group(self.client, myip_trap_group)
            sai_thrift_remove_route_entry(self.client, ip2me_route0)
            sai_thrift_remove_route_entry(self.client, ip2me_route1)
            sai_thrift_remove_route_entry(self.client, ip2me_route2)
            sai_thrift_remove_route_entry(self.client, ip2me_route3)
            sai_thrift_remove_route_entry(self.client, ip2me_route4)
            sai_thrift_remove_route_entry(self.client, ip2me_route5)
            sai_thrift_remove_route_entry(self.client, ip2me_route6)
            sai_thrift_remove_route_entry(self.client, ip2me_route7)
            sai_thrift_remove_route_entry(self.client, ip2me_route8)

    def tearDown(self):
        super(SubPortMyIPTest, self).tearDown()

@group("draft")
class SubPortNoTest(L3SubPortTestHelper):
    '''
    SONiC enforces a minimum scalability requirement on the number
    of sub port interfaces that shall be supported on a SONiC switch.
    Scaling value for number of sub port interfaces per physical port
    or port channel is 250.
    '''
    def setUp(self):
        super(SubPortNoTest, self).setUp()

    def runTest(self):
        print("\nsubPortNoTest()")

        req_sub_port_no = 256
        test_port = self.port26
        test_lag = self.lag1

        try:
            vlan_id = 100
            rif_port_sub_port_list = []
            for _ in range(req_sub_port_no):
                sub_port_rif = sai_thrift_create_router_interface(
                    self.client,
                    type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
                    virtual_router_id=self.default_vrf,
                    port_id=test_port,
                    admin_v4_state=True,
                    outer_vlan_id=vlan_id)
                self.assertTrue(sub_port_rif != 0)
                rif_port_sub_port_list.append(sub_port_rif)
                vlan_id += 1

            rif_lag_sub_port_list = []
            for _ in range(req_sub_port_no):
                sub_lag_rif = sai_thrift_create_router_interface(
                    self.client,
                    type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
                    virtual_router_id=self.default_vrf,
                    port_id=test_lag,
                    admin_v4_state=True,
                    outer_vlan_id=vlan_id)
                self.assertTrue(sub_lag_rif != 0)
                rif_port_sub_port_list.append(sub_lag_rif)
                vlan_id += 1

        finally:
            for sub_lag_rif in rif_lag_sub_port_list:
                sai_thrift_remove_router_interface(self.client, sub_lag_rif)

            for sub_port_rif in rif_port_sub_port_list:
                sai_thrift_remove_router_interface(self.client, sub_port_rif)

    def tearDown(self):
        super(SubPortNoTest, self).tearDown()

@group("draft")
class SubPortQosGroupTest(L3SubPortTestHelper):
    """
    Verifies QoS group setting inherited from parent port or LAG
    """
    def setUp(self):
        super(SubPortQosGroupTest, self).setUp()

    def runTest(self):
        print("\nsubPortQosGroupTest()")

        try:
            dscp_to_tc = sai_thrift_qos_map_t(
                key=sai_thrift_qos_map_params_t(dscp=1),
                value=sai_thrift_qos_map_params_t(tc=20))

            ingr_qos_map_list = sai_thrift_qos_map_list_t(
                count=1, maplist=[dscp_to_tc])

            ingr_qos_map = sai_thrift_create_qos_map(
                self.client,
                type=SAI_QOS_MAP_TYPE_DSCP_TO_TC,
                map_to_value_list=ingr_qos_map_list)

            tc_col_to_dscp = sai_thrift_qos_map_t(
                key=sai_thrift_qos_map_params_t(tc=20, color=0),
                value=sai_thrift_qos_map_params_t(dscp=9))

            egr_qos_map_list = sai_thrift_qos_map_list_t(
                count=1, maplist=[tc_col_to_dscp])

            egr_qos_map = sai_thrift_create_qos_map(
                self.client,
                type=SAI_QOS_MAP_TYPE_TC_AND_COLOR_TO_DSCP,
                map_to_value_list=egr_qos_map_list)

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:33:44:55',
                                    ip_dst='40.40.1.20',
                                    ip_src='30.30.1.1',
                                    ip_id=105,
                                    ip_tos=4,   # dscp 1
                                    ip_ttl=64,
                                    dl_vlan_enable=True,
                                    vlan_vid=200)

            qos_exp_pkt = simple_tcp_packet(eth_dst='00:33:33:33:12:00',
                                            eth_src=ROUTER_MAC,
                                            ip_dst='40.40.1.20',
                                            ip_src='30.30.1.1',
                                            ip_id=105,
                                            ip_tos=36,  # dscp 9
                                            ip_ttl=63,
                                            dl_vlan_enable=True,
                                            vlan_vid=200)

            no_qos_exp_pkt = simple_tcp_packet(eth_dst='00:33:33:33:12:00',
                                               eth_src=ROUTER_MAC,
                                               ip_dst='40.40.1.20',
                                               ip_src='30.30.1.1',
                                               ip_id=105,
                                               ip_tos=4,  # dscp 1
                                               ip_ttl=63,
                                               dl_vlan_enable=True,
                                               vlan_vid=200)

            # before setting QoS - packet routed without changes
            print("Sending packet before QoS settings")
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, no_qos_exp_pkt, [self.dev_port11])
            print("OK")

            sai_thrift_set_port_attribute(
                self.client, self.port10, qos_dscp_to_tc_map=ingr_qos_map)

            sai_thrift_set_port_attribute(
                self.client,
                self.port11,
                qos_tc_and_color_to_dscp_map=egr_qos_map)

            # after setting QoS - DSCP rewrited
            print("Sending packet after QoS settings")
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, qos_exp_pkt, [self.dev_port11])
            print("OK - DSCP value rewrited")

        finally:
            sai_thrift_set_port_attribute(
                self.client, self.port11, qos_tc_and_color_to_dscp_map=0)
            sai_thrift_remove_qos_map(self.client, egr_qos_map)
            sai_thrift_set_port_attribute(
                self.client, self.port10, qos_dscp_to_tc_map=0)
            sai_thrift_remove_qos_map(self.client, ingr_qos_map)

    def tearDown(self):
        super(SubPortQosGroupTest, self).tearDown()


@group("draft")
@group("tunnel")
class SubPortTunnelTest(L3SubPortTestHelper):
    """
    Verifies tunnel encap-decap over sub-port
    """
    def setUp(self):
        super(SubPortTunnelTest, self).setUp()

    def runTest(self):
        print("\nsubPortTunnelTest()")

        iport = self.port26
        iport_dev = self.dev_port26
        eport = self.port27
        eport_dev = self.dev_port27

        vlan_id = 999

        tun_vni = 2000
        tun_ip = "30.30.30.1"
        lpb_ip = "30.30.30.10"
        vm_ip = "100.100.1.1"
        customer_ip = "100.100.2.1"
        tun_mac = "00:aa:aa:aa:aa:aa"
        tun_term_mac = "00:bb:bb:bb:bb:bb"
        customer_mac = "00:ee:ee:ee:ee:ee"

        try:
            # underlay configuration
            uvrf = sai_thrift_create_virtual_router(self.client)

            # overlay configuration
            ovrf = sai_thrift_create_virtual_router(self.client)

            # underlay loopback RIF for tunnel
            urif_lpb = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
                virtual_router_id=uvrf)

            # route to tunnel termination IP
            urif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
                virtual_router_id=uvrf,
                port_id=eport,
                outer_vlan_id=vlan_id)

            # overlay RIF
            orif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                virtual_router_id=ovrf,
                port_id=iport)

            # encapsulation configuration
            encap_tunnel_map = sai_thrift_create_tunnel_map(
                self.client, type=SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI)

            decap_tunnel_map = sai_thrift_create_tunnel_map(
                self.client, type=SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID)

            encap_tunnel_map_entry = sai_thrift_create_tunnel_map_entry(
                self.client,
                tunnel_map=encap_tunnel_map,
                tunnel_map_type=SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI,
                virtual_router_id_key=ovrf,
                vni_id_value=tun_vni)

            decap_tunnel_map_entry = sai_thrift_create_tunnel_map_entry(
                self.client,
                tunnel_map=encap_tunnel_map,
                tunnel_map_type=SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID,
                virtual_router_id_value=ovrf,
                vni_id_key=tun_vni)

            encap_maps = sai_thrift_object_list_t(
                count=1, idlist=[encap_tunnel_map])

            decap_maps = sai_thrift_object_list_t(
                count=1, idlist=[decap_tunnel_map])

            tunnel = sai_thrift_create_tunnel(
                self.client,
                type=SAI_TUNNEL_TYPE_VXLAN,
                encap_src_ip=sai_ipaddress(lpb_ip),
                encap_mappers=encap_maps,
                decap_mappers=decap_maps,
                encap_ttl_mode=SAI_TUNNEL_TTL_MODE_PIPE_MODEL,
                decap_ttl_mode=SAI_TUNNEL_TTL_MODE_PIPE_MODEL,
                underlay_interface=urif_lpb)

            tunnel_term = sai_thrift_create_tunnel_term_table_entry(
                self.client,
                tunnel_type=SAI_TUNNEL_TYPE_VXLAN,
                action_tunnel_id=tunnel,
                vr_id=uvrf,
                type=SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P,
                src_ip=sai_ipaddress(tun_ip),
                dst_ip=sai_ipaddress(lpb_ip))

            # route from VM to customer
            onhop = sai_thrift_create_next_hop(self.client,
                                               ip=sai_ipaddress(customer_ip),
                                               router_interface_id=orif,
                                               type=SAI_NEXT_HOP_TYPE_IP)

            onbor = sai_thrift_neighbor_entry_t(
                rif_id=orif, ip_address=sai_ipaddress(customer_ip))
            sai_thrift_create_neighbor_entry(self.client,
                                             onbor,
                                             dst_mac_address=customer_mac,
                                             no_host_route=True)

            customer_route = sai_thrift_route_entry_t(
                vr_id=ovrf, destination=sai_ipprefix(customer_ip + '/32'))
            sai_thrift_create_route_entry(
                self.client, customer_route, next_hop_id=onhop)

            # route from customer to VM
            tunnel_nhop = sai_thrift_create_next_hop(
                self.client,
                type=SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP,
                tunnel_id=tunnel,
                ip=sai_ipaddress(tun_ip),
                tunnel_mac=tun_mac,
                tunnel_vni=tun_vni)

            vm_route = sai_thrift_route_entry_t(
                vr_id=ovrf, destination=sai_ipprefix(vm_ip + '/32'))
            sai_thrift_create_route_entry(
                self.client, vm_route, next_hop_id=tunnel_nhop)

            unhop = sai_thrift_create_next_hop(self.client,
                                               ip=sai_ipaddress(tun_ip),
                                               router_interface_id=urif,
                                               type=SAI_NEXT_HOP_TYPE_IP)

            unbor = sai_thrift_neighbor_entry_t(
                rif_id=urif, ip_address=sai_ipaddress(tun_ip))
            sai_thrift_create_neighbor_entry(self.client,
                                             unbor,
                                             dst_mac_address=tun_term_mac,
                                             no_host_route=True)

            tunnel_route = sai_thrift_route_entry_t(
                vr_id=uvrf, destination=sai_ipprefix(tun_ip + '/32'))
            sai_thrift_create_route_entry(
                self.client, tunnel_route, next_hop_id=unhop)

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=customer_mac,
                                    ip_dst=vm_ip,
                                    ip_src=customer_ip,
                                    ip_id=108,
                                    ip_ttl=64)
            inner_pkt = simple_tcp_packet(eth_dst=tun_mac,
                                          eth_src=ROUTER_MAC,
                                          ip_dst=vm_ip,
                                          ip_src=customer_ip,
                                          ip_id=108,
                                          ip_ttl=63)
            vxlan_pkt = Mask(
                simple_vxlan_packet(eth_dst=tun_term_mac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=tun_ip,
                                    ip_src=lpb_ip,
                                    ip_id=0,
                                    ip_ttl=64,
                                    ip_flags=0x2,
                                    dl_vlan_enable=True,
                                    vlan_vid=vlan_id,
                                    with_udp_chksum=False,
                                    vxlan_vni=tun_vni,
                                    inner_frame=inner_pkt))
            vxlan_pkt.set_do_not_care_scapy(UDP, 'sport')

            print("Sending packet to VM")
            send_packet(self, iport_dev, pkt)
            verify_packet(self, vxlan_pkt, eport_dev)
            print("\tOK")

            pkt = simple_tcp_packet(eth_dst=customer_mac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=customer_ip,
                                    ip_src=vm_ip,
                                    ip_id=108,
                                    ip_ttl=63)
            inner_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                          eth_src=tun_mac,
                                          ip_dst=customer_ip,
                                          ip_src=vm_ip,
                                          ip_id=108,
                                          ip_ttl=64)
            vxlan_pkt = simple_vxlan_packet(eth_dst=ROUTER_MAC,
                                            eth_src=tun_term_mac,
                                            ip_dst=lpb_ip,
                                            ip_src=tun_ip,
                                            ip_id=0,
                                            ip_ttl=64,
                                            ip_flags=0x2,
                                            dl_vlan_enable=True,
                                            vlan_vid=vlan_id,
                                            with_udp_chksum=False,
                                            vxlan_vni=tun_vni,
                                            inner_frame=inner_pkt)

            print("Sending packet to customer")
            send_packet(self, eport_dev, vxlan_pkt)
            verify_packet(self, pkt, iport_dev)
            print("\tOK")

        finally:
            sai_thrift_remove_route_entry(self.client, tunnel_route)
            sai_thrift_remove_neighbor_entry(self.client, unbor)
            sai_thrift_remove_next_hop(self.client, unhop)

            sai_thrift_remove_route_entry(self.client, vm_route)
            sai_thrift_remove_next_hop(self.client, tunnel_nhop)

            sai_thrift_remove_route_entry(self.client, customer_route)
            sai_thrift_remove_neighbor_entry(self.client, onbor)
            sai_thrift_remove_next_hop(self.client, onhop)

            sai_thrift_remove_tunnel_term_table_entry(self.client, tunnel_term)
            sai_thrift_remove_tunnel(self.client, tunnel)

            sai_thrift_remove_tunnel_map_entry(
                self.client, decap_tunnel_map_entry)
            sai_thrift_remove_tunnel_map_entry(
                self.client, encap_tunnel_map_entry)

            sai_thrift_remove_tunnel_map(self.client, decap_tunnel_map)
            sai_thrift_remove_tunnel_map(self.client, encap_tunnel_map)

            sai_thrift_remove_router_interface(self.client, orif)
            sai_thrift_remove_router_interface(self.client, urif)
            sai_thrift_remove_router_interface(self.client, urif_lpb)

            sai_thrift_remove_virtual_router(self.client, ovrf)
            sai_thrift_remove_virtual_router(self.client, uvrf)

    def tearDown(self):
        super(SubPortTunnelTest, self).tearDown()


@group("draft")
class L3SviTestHelper(PlatformSaiHelper):
    """
    This class contains base router interface tests for SVI RIFs

    Topology
    L3 intf  - 10, 11
    vlan 100 - 24, 25, 26
    vlan 200 - lag10(30, 31), lag11(28, 29)
    27 extra port used for testing
    """

    def setUp(self):
        super(L3SviTestHelper, self).setUp()

        self.vlan100_rif_counter_in = 0
        self.vlan100_rif_counter_out = 0
        self.vlan200_rif_counter_in = 0
        self.vlan200_rif_counter_out = 0
        self.vlan100_bcast_in = 0
        self.vlan100_bcast_out = 0
        self.vlan200_bcast_in = 0
        self.vlan200_bcast_out = 0

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

        # vlan100 with members port24, port25 and port26
        self.vlan100 = sai_thrift_create_vlan(self.client, vlan_id=100)
        self.vlan_member100 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_member101 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port25_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_member102 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_port_attribute(
            self.client, self.port24, port_vlan_id=100)
        sai_thrift_set_port_attribute(
            self.client, self.port25, port_vlan_id=100)
        sai_thrift_set_port_attribute(
            self.client, self.port26, port_vlan_id=100)

        # create vlan100_rif
        self.vlan100_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan100)

        self.dmac1 = '00:11:22:33:44:55'  # 10.10.10.1
        self.dmac2 = '00:22:22:33:44:55'  # 10.10.10.2
        self.dmac3 = '00:33:22:33:44:55'  # 10.10.10.3
        self.dmac4 = '00:44:22:33:44:55'  # 11.11.11.1
        self.dmac5 = '00:11:33:33:44:55'  # 20.10.10.1
        self.dmac6 = '00:22:33:33:44:55'  # 20.10.10.2
        self.dmac7 = '00:44:33:33:44:55'  # 20.11.11.1

        # create nhop1, nhop2 & nhop3 on SVI
        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.1'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, ip_address=sai_ipaddress('10.10.0.1'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=self.dmac1)
        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1, next_hop_id=self.nhop1)
        self.route_entry1_v6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:0000/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1_v6, next_hop_id=self.nhop1)

        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, ip_address=sai_ipaddress('10.10.0.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry2, dst_mac_address=self.dmac2)
        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.2'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.2/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry2, next_hop_id=self.nhop2)
        self.route_entry2_v6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:2222/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry2_v6, next_hop_id=self.nhop2)

        self.nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.3'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry3 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, ip_address=sai_ipaddress('10.10.0.3'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry3, dst_mac_address=self.dmac3)
        self.route_entry3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.3/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry3, next_hop_id=self.nhop3)

        # create nhop and route to L2 intf
        self.nhop4 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('11.11.0.2'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry4 = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('11.11.0.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry4, dst_mac_address=self.dmac4)
        self.route_entry4 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('11.11.11.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry4, next_hop_id=self.nhop4)
        self.route_entry4_v6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:1111/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry4_v6, next_hop_id=self.nhop4)

        self.lag10 = sai_thrift_create_lag(self.client)
        self.lag10_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag10,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.lag10_member30 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag10, port_id=self.port30)
        self.lag10_member31 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag10, port_id=self.port31)

        self.lag11 = sai_thrift_create_lag(self.client)
        self.lag11_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag11,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.lag11_member28 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag11, port_id=self.port28)
        self.lag11_member29 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag11, port_id=self.port29)

        # vlan200 with members lag10 and lag11
        self.vlan200 = sai_thrift_create_vlan(self.client, vlan_id=200)
        self.vlan_member200 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan200,
            bridge_port_id=self.lag10_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_member201 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan200,
            bridge_port_id=self.lag11_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_lag_attribute(self.client, self.lag10, port_vlan_id=200)
        sai_thrift_set_lag_attribute(self.client, self.lag11, port_vlan_id=200)

        self.vlan200_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan200)

        # Create nhop5 and nhop6 on SVI
        self.nhop5 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.10.0.1'),
            router_interface_id=self.vlan200_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry5 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan200_rif, ip_address=sai_ipaddress('20.10.0.1'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry5, dst_mac_address=self.dmac5)
        self.route_entry5 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('20.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry5, next_hop_id=self.nhop5)
        self.route_entry5_v6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:99aa/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry5_v6, next_hop_id=self.nhop5)

        self.nhop6 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.10.0.2'),
            router_interface_id=self.vlan200_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry6 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan200_rif, ip_address=sai_ipaddress('20.10.0.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry6, dst_mac_address=self.dmac6)
        self.route_entry6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('20.10.10.2/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry6, next_hop_id=self.nhop6)
        self.route_entry6_v6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:1122:3344:5566:7788/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry6_v6, next_hop_id=self.nhop6)

        self.nhop7 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('21.11.0.2'),
            router_interface_id=self.port11_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry7 = sai_thrift_neighbor_entry_t(
            rif_id=self.port11_rif, ip_address=sai_ipaddress('21.11.0.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry7, dst_mac_address=self.dmac7)
        self.route_entry7 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('21.11.11.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry7, next_hop_id=self.nhop7)
        self.route_entry7_v6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:1122:3344:5566:6677/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry7_v6, next_hop_id=self.nhop7)

    def tearDown(self):
        # When removing nhop, neighbor and route, calling api in this order:
        # route -> nhop -> neighbor
        # Related Issue: https://github.com/opencomputeproject/SAI/issues/1607
        sai_thrift_remove_route_entry(self.client, self.route_entry5)
        sai_thrift_remove_route_entry(self.client, self.route_entry5_v6)
        sai_thrift_remove_route_entry(self.client, self.route_entry6)
        sai_thrift_remove_route_entry(self.client, self.route_entry6_v6)
        sai_thrift_remove_route_entry(self.client, self.route_entry7)
        sai_thrift_remove_route_entry(self.client, self.route_entry7_v6)

        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry5)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry6)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry7)

        sai_thrift_remove_next_hop(self.client, self.nhop5)
        sai_thrift_remove_next_hop(self.client, self.nhop6)
        sai_thrift_remove_next_hop(self.client, self.nhop7)

        sai_thrift_set_lag_attribute(self.client, self.lag10, port_vlan_id=0)
        sai_thrift_set_lag_attribute(self.client, self.lag11, port_vlan_id=0)

        sai_thrift_remove_router_interface(self.client, self.vlan200_rif)

        sai_thrift_remove_vlan_member(self.client, self.vlan_member200)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member201)

        sai_thrift_remove_vlan(self.client, self.vlan200)

        sai_thrift_remove_lag_member(self.client, self.lag10_member30)
        sai_thrift_remove_lag_member(self.client, self.lag10_member31)
        sai_thrift_remove_lag_member(self.client, self.lag11_member28)
        sai_thrift_remove_lag_member(self.client, self.lag11_member29)
        sai_thrift_remove_bridge_port(self.client, self.lag10_bp)
        sai_thrift_remove_bridge_port(self.client, self.lag11_bp)
        sai_thrift_remove_lag(self.client, self.lag10)
        sai_thrift_remove_lag(self.client, self.lag11)

        sai_thrift_remove_route_entry(self.client, self.route_entry1)
        sai_thrift_remove_route_entry(self.client, self.route_entry1_v6)
        sai_thrift_remove_route_entry(self.client, self.route_entry2)
        sai_thrift_remove_route_entry(self.client, self.route_entry2_v6)
        sai_thrift_remove_route_entry(self.client, self.route_entry3)
        sai_thrift_remove_route_entry(self.client, self.route_entry4)
        sai_thrift_remove_route_entry(self.client, self.route_entry4_v6)

        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry3)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry4)

        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_next_hop(self.client, self.nhop3)
        sai_thrift_remove_next_hop(self.client, self.nhop4)

        sai_thrift_remove_router_interface(self.client, self.vlan100_rif)

        sai_thrift_set_port_attribute(self.client, self.port24, port_vlan_id=0)
        sai_thrift_set_port_attribute(self.client, self.port25, port_vlan_id=0)
        sai_thrift_set_port_attribute(self.client, self.port26, port_vlan_id=0)

        sai_thrift_remove_vlan_member(self.client, self.vlan_member100)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member101)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member102)

        sai_thrift_remove_vlan(self.client, self.vlan100)

        sai_thrift_remove_bridge_port(self.client, self.port24_bp)
        sai_thrift_remove_bridge_port(self.client, self.port25_bp)
        sai_thrift_remove_bridge_port(self.client, self.port26_bp)

        super(L3SviTestHelper, self).tearDown()

@group("draft")
class SviRifIPv4DisableTest(L3SviTestHelper):
    """
    Verifies if IPv4 packets are dropped when admin_v4_state is False
    """
    def setUp(self):
        super(SviRifIPv4DisableTest, self).setUp()

    def runTest(self):
        print("\nsviRifIPv4DisableTest()")

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='11.11.11.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64,
                                pktlen=100)
        exp_pkt = simple_tcp_packet(eth_dst='00:44:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='11.11.11.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63,
                                    pktlen=100)
        vlan_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                     eth_src='00:22:22:22:22:22',
                                     ip_dst='11.11.11.1',
                                     ip_src='192.168.0.1',
                                     dl_vlan_enable=True,
                                     vlan_vid=100,
                                     ip_id=105,
                                     ip_ttl=64,
                                     pktlen=104)

        try:
            print("Sending packet on port %d, routed" % self.dev_port24)
            send_packet(self, self.dev_port24, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

            print("Sending packet on port %d, routed" % self.dev_port25)
            send_packet(self, self.dev_port25, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

            print("Sending packet on port %d, routed" % self.dev_port26)
            send_packet(self, self.dev_port26, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

            print("Disable IPv4 on ingress RIF")
            sai_thrift_set_router_interface_attribute(
                self.client, self.vlan100_rif, admin_v4_state=False)
            print("Sending packet on port %d, dropped" % self.dev_port24)
            send_packet(self, self.dev_port24, pkt)
            verify_no_other_packets(self, timeout=1)
            self.vlan100_rif_counter_in += 1

            print("Sending packet on port %d, dropped" % self.dev_port25)
            send_packet(self, self.dev_port25, pkt)
            verify_no_other_packets(self, timeout=1)
            self.vlan100_rif_counter_in += 1

            print("Sending packet on port %d, dropped" % self.dev_port26)
            send_packet(self, self.dev_port26, pkt)
            verify_no_other_packets(self, timeout=1)
            self.vlan100_rif_counter_in += 1

            port27_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=self.port27,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)

            print("Verify setting for new vlan member")
            vlan_member103 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan100,
                bridge_port_id=port27_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

            print("Sending packet on port %d, dropped" % self.dev_port27)
            send_packet(self, self.dev_port27, vlan_pkt)
            verify_no_other_packets(self, timeout=1)
            self.vlan100_rif_counter_in += 1

            sai_thrift_remove_vlan_member(self.client, vlan_member103)
            sai_thrift_set_router_interface_attribute(
                self.client, self.vlan100_rif, admin_v4_state=True)

            print("Sending packet on port %d, routed" % self.dev_port24)
            send_packet(self, self.dev_port24, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

            print("Sending packet on port %d, routed" % self.dev_port25)
            send_packet(self, self.dev_port25, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

            print("Sending packet on port %d, routed" % self.dev_port26)
            send_packet(self, self.dev_port26, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

            print("Verify setting for new vlan member")
            vlan_member103 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan100,
                bridge_port_id=port27_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

            print("Sending packet on port %d, routed" % self.dev_port27)
            send_packet(self, self.dev_port27, vlan_pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

        finally:
            sai_thrift_remove_vlan_member(self.client, vlan_member103)
            sai_thrift_remove_bridge_port(self.client, port27_bp)

    def tearDown(self):
        super(SviRifIPv4DisableTest, self).tearDown()

@group("draft")
class SviRifIPv6DisableTest(L3SviTestHelper):
    """
    Verifies IPv6 packets are dropped when admin_v6_state is False
    """
    def setUp(self):
        super(SviRifIPv6DisableTest, self).setUp()

    def runTest(self):
        print("\nsviRifIPv6DisableTest()")

        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:1111',
            ipv6_src='2000::1',
            ipv6_hlim=64,
            pktlen=100)
        vlan_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:1111',
            ipv6_src='2000::1',
            dl_vlan_enable=True,
            vlan_vid=100,
            ipv6_hlim=64,
            pktlen=104)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:44:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:1111',
            ipv6_src='2000::1',
            ipv6_hlim=63,
            pktlen=100)

        try:
            print("Sending packet on port %d, routed" % self.dev_port24)
            send_packet(self, self.dev_port24, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

            print("Disable IPv4 on ingress RIF")
            sai_thrift_set_router_interface_attribute(
                self.client, self.vlan100_rif, admin_v6_state=False)

            print("Sending packet on port %d, dropped" % self.dev_port24)
            send_packet(self, self.dev_port24, pkt)
            verify_no_other_packets(self, timeout=1)
            self.vlan100_rif_counter_in += 1

            port27_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=self.port27,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)

            print("Verify setting for new vlan member")
            vlan_member103 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan100,
                bridge_port_id=port27_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

            print("Sending packet on port %d, dropped" % self.dev_port27)
            send_packet(self, self.dev_port27, vlan_pkt)
            verify_no_other_packets(self, timeout=1)
            self.vlan100_rif_counter_in += 1

            sai_thrift_remove_vlan_member(self.client, vlan_member103)
            sai_thrift_set_router_interface_attribute(
                self.client, self.vlan100_rif, admin_v6_state=True)

            print("Sending packet on port %d, routed" % self.dev_port24)
            send_packet(self, self.dev_port24, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

            print("Verify setting for new vlan member")
            vlan_member103 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan100,
                bridge_port_id=port27_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

            print("Sending packet on port %d, routed" % self.dev_port27)
            send_packet(self, self.dev_port27, vlan_pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

        finally:
            sai_thrift_remove_vlan_member(self.client, vlan_member103)
            sai_thrift_remove_bridge_port(self.client, port27_bp)

    def tearDown(self):
        super(SviRifIPv6DisableTest, self).tearDown()

@group("draft")
class SviBridgingTest(L3SviTestHelper):
    """
    Verifies L2 bridging on an SVI port if rmac miss and L2 flooding
    on an SVI if broadcast packet is received
    """
    def setUp(self):
        super(SviBridgingTest, self).setUp()

    def runTest(self):
        print("\nsviBridgingTest()")

        sai_thrift_set_vlan_attribute(
            self.client, self.vlan100, learn_disable=True)
        mac_action = SAI_PACKET_ACTION_FORWARD

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='11.11.11.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:44:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='11.11.11.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)
        pkt2 = simple_tcp_packet(eth_dst='00:77:66:55:44:44',
                                 eth_src='00:22:22:22:22:22',
                                 ip_dst='11.11.11.1',
                                 ip_src='192.168.0.1',
                                 ip_id=105,
                                 ip_ttl=64,
                                 pktlen=104)
        pkt3 = simple_tcp_packet(eth_dst='FF:FF:FF:FF:FF:FF',
                                 eth_src='00:22:22:22:22:22',
                                 ip_dst='255.255.225.255',
                                 ip_src='192.168.0.1',
                                 ip_id=105,
                                 ip_ttl=64,
                                 pktlen=104)

        try:
            print("Sending packet on port %d to port %d, (192.168.0.1 -> "
                  "11.11.11.1) Routed" % (self.dev_port24, self.dev_port10))
            send_packet(self, self.dev_port24, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

            print("Sending packet on port %d to port %d, (192.168.0.1 -> "
                  "11.11.11.1) Routed" % (self.dev_port25, self.dev_port10))
            send_packet(self, self.dev_port25, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

            print("Sending packet on port %d to port %d, (192.168.0.1 -> "
                  "11.11.11.1) Routed" % (self.dev_port26, self.dev_port10))
            send_packet(self, self.dev_port26, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

            print("Sending packet on port %d, (192.168.0.1 -> 11.11.11.1) "
                  "flood (missed RMAC)" % (self.dev_port24))
            send_packet(self, self.dev_port24, pkt2)
            verify_packets(self, pkt2, [self.dev_port25, self.dev_port26])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2

            print("Sending packet on port %d, (192.168.0.1 -> 11.11.11.1) "
                  "flood (missed RMAC)" % (self.dev_port25))
            send_packet(self, self.dev_port25, pkt2)
            verify_packets(self, pkt2, [self.dev_port24, self.dev_port26])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2

            print("Sending packet on port %d, (192.168.0.1 -> 11.11.11.1) "
                  "flood (missed RMAC)" % (self.dev_port26))
            send_packet(self, self.dev_port26, pkt2)
            verify_packets(self, pkt2, [self.dev_port25, self.dev_port24])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2

            fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                               mac_address='00:77:66:55:44:44',
                                               bv_id=self.vlan100)
            sai_thrift_create_fdb_entry(self.client,
                                        fdb_entry,
                                        type=SAI_FDB_ENTRY_TYPE_STATIC,
                                        bridge_port_id=self.port24_bp,
                                        packet_action=mac_action)

            print("Sending packet on port %d to port %d, (192.168.0.1 -> "
                  "11.11.11.1) Routed" % (self.dev_port25, self.dev_port24))
            send_packet(self, self.dev_port25, pkt2)
            verify_packets(self, pkt2, [self.dev_port24])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 1

            print("Sending packet on port %d to port %d, (192.168.0.1 -> "
                  "11.11.11.1) Routed" % (self.dev_port26, self.dev_port24))
            send_packet(self, self.dev_port26, pkt2)
            verify_packets(self, pkt2, [self.dev_port24])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 1

            print("Sending broadcast packet on port %d, (192.168.0.1 -> "
                  "255.255.255.255) flood" % (self.dev_port24))
            send_packet(self, self.dev_port24, pkt3)
            verify_packets(self, pkt3, [self.dev_port25, self.dev_port26])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2
            self.vlan100_bcast_in += 1
            self.vlan100_bcast_out += 2

            print("Sending broadcast packet on port %d, (192.168.0.1 -> "
                  "255.255.255.255) flood" % (self.dev_port25))
            send_packet(self, self.dev_port25, pkt3)
            verify_packets(self, pkt3, [self.dev_port24, self.dev_port26])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2
            self.vlan100_bcast_in += 1
            self.vlan100_bcast_out += 2

            print("Sending broadcast packet on port %d, (192.168.0.1 -> "
                  "255.255.255.255) flood" % (self.dev_port26))
            send_packet(self, self.dev_port26, pkt3)
            verify_packets(self, pkt3, [self.dev_port25, self.dev_port24])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2
            self.vlan100_bcast_in += 1
            self.vlan100_bcast_out += 2

        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)
            sai_thrift_set_vlan_attribute(
                self.client, self.vlan100, learn_disable=False)

    def tearDown(self):
        super(SviBridgingTest, self).tearDown()

@group("draft")
class SviHostTest(L3SviTestHelper):
    """
    Verifies routing after NHOP resolved via static MAC entry
    """
    def setUp(self):
        super(SviHostTest, self).setUp()

    def runTest(self):
        print("\nsviHostTest()")

        mac_action = SAI_PACKET_ACTION_FORWARD
        fdb_entry1 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                            mac_address='00:11:22:33:44:55',
                                            bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry1,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.port24_bp,
                                    packet_action=mac_action)

        fdb_entry2 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                            mac_address='00:22:22:33:44:55',
                                            bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry2,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.port25_bp,
                                    packet_action=mac_action)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)
        pkt1 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                 eth_src='00:22:22:22:22:22',
                                 ip_dst='10.10.10.2',
                                 ip_src='192.168.0.1',
                                 ip_id=105,
                                 ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(eth_dst='00:22:22:33:44:55',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.2',
                                     ip_src='192.168.0.1',
                                     ip_id=105,
                                     ip_ttl=63)
        pkt2 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                 eth_src='00:22:22:22:22:22',
                                 ip_dst='11.11.11.1',
                                 ip_src='192.168.0.1',
                                 ip_id=105,
                                 ip_ttl=64)
        exp_pkt2 = simple_tcp_packet(eth_dst='00:44:22:33:44:55',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='11.11.11.1',
                                     ip_src='192.168.0.1',
                                     ip_id=105,
                                     ip_ttl=63)

        try:
            print("Sending packet port %d to port %d, (192.168.0.1 -> "
                  "10.10.10.1) Routed" % (self.dev_port10, self.dev_port24))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt, [self.dev_port24])
            self.vlan100_rif_counter_out += 1

            print("Sending packet port %d to port %d, (192.168.0.1 -> "
                  "10.10.10.2) Routed" % (self.dev_port10, self.dev_port25))
            send_packet(self, self.dev_port10, pkt1)
            verify_packets(self, exp_pkt1, [self.dev_port25])
            self.vlan100_rif_counter_out += 1

            print("Sending packet port %d to port %d, (192.168.0.1 -> "
                  "11.11.11.1) Routed" % (self.dev_port24, self.dev_port10))
            send_packet(self, self.dev_port24, pkt2)
            verify_packets(self, exp_pkt2, [self.dev_port10])
            self.vlan100_rif_counter_in += 1

        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry1)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry2)

    def tearDown(self):
        super(SviHostTest, self).tearDown()

@group("draft")
class SviHostVlanTaggingTest(L3SviTestHelper):
    """
    Verifies routing after NHOP resolved via static MAC entry
    on tagged member
    """
    def setUp(self):
        super(SviHostVlanTaggingTest, self).setUp()

    def runTest(self):
        print("\nsviHostVlanTaggingTest()")
        print("Test routing after NHOP resolved via static MAC entry")

        mac_action = SAI_PACKET_ACTION_FORWARD

        port27_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port27,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        vlan_member103 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                           mac_address='00:33:22:33:44:55',
                                           bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=port27_bp,
                                    packet_action=mac_action)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.3',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:33:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='10.10.10.3',
                                    ip_src='192.168.0.1',
                                    dl_vlan_enable=True,
                                    vlan_vid=100,
                                    ip_id=105,
                                    ip_ttl=63,
                                    pktlen=104)

        try:
            print("Sending packet port %d to port %d, (192.168.0.1 -> "
                  "10.10.10.3)" % (self.dev_port10, self.dev_port27))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt, [self.dev_port27])
            self.vlan100_rif_counter_out += 1

        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)
            sai_thrift_remove_vlan_member(self.client, vlan_member103)
            sai_thrift_remove_bridge_port(self.client, port27_bp)

    def tearDown(self):
        super(SviHostVlanTaggingTest, self).tearDown()

@group("draft")
class SviToSviRoutingTest(L3SviTestHelper):
    '''
    vlan100/24 -> vlan200/lag10
    vlan100/27 -> vlan200/lag10
    vlan200/lag10 -> vlan100/24
    vlan200/lag10 -> vlan100/27
    '''
    def setUp(self):
        super(SviToSviRoutingTest, self).setUp()

    def runTest(self):
        print("\nsviToSviRoutingTest()")
        print("Test routing between SVI")

        mac_action = SAI_PACKET_ACTION_FORWARD

        port27_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port27,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        vlan_member103 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=port27_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        fdb_entry27 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                             mac_address=self.dmac2,
                                             bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry27,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=port27_bp,
                                    packet_action=mac_action)

        fdb_entry24 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                             mac_address=self.dmac1,
                                             bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry24,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.port24_bp,
                                    packet_action=mac_action)

        fdb_entry_lag10 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                 mac_address=self.dmac5,
                                                 bv_id=self.vlan200)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry_lag10,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.lag10_bp,
                                    packet_action=mac_action)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.dmac1,
                                ip_dst='20.10.10.1',
                                ip_src='192.168.0.1',
                                ip_ttl=64)
        tagged_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                       eth_src=self.dmac2,
                                       ip_dst='20.10.10.1',
                                       ip_src='192.168.0.1',
                                       dl_vlan_enable=True,
                                       vlan_vid=100,
                                       ip_ttl=64,
                                       pktlen=104)
        exp_pkt = simple_tcp_packet(eth_dst=self.dmac5,
                                    eth_src=ROUTER_MAC,
                                    ip_dst='20.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_ttl=63)
        tagged_exp_pkt = simple_tcp_packet(eth_dst=self.dmac2,
                                           eth_src=ROUTER_MAC,
                                           ip_dst='10.10.10.2',
                                           ip_src='192.168.0.1',
                                           dl_vlan_enable=True,
                                           vlan_vid=100,
                                           ip_ttl=63,
                                           pktlen=104)

        try:
            print("Sending packet port %d to LAG, (192.168.0.1 -> 20.10.10.1)"
                  % (self.dev_port24))
            send_packet(self, self.dev_port24, pkt)
            verify_packet_any_port(self, exp_pkt,
                                   [self.dev_port30, self.dev_port31])
            self.vlan100_rif_counter_in += 1
            self.vlan200_rif_counter_out += 1

            print("Sending packet port %d to LAG, (192.168.0.1 -> 20.10.10.1)"
                  % (self.dev_port27))
            send_packet(self, self.dev_port27, tagged_pkt)
            verify_packet_any_port(self, exp_pkt,
                                   [self.dev_port30, self.dev_port31])
            self.vlan100_rif_counter_in += 1
            self.vlan200_rif_counter_out += 1

            pkt['Ethernet'].src = self.dmac5
            pkt['IP'].dst = '10.10.10.1'
            exp_pkt['Ethernet'].dst = self.dmac1
            exp_pkt['IP'].dst = '10.10.10.1'
            tagged_exp_pkt['Ethernet'].dst = self.dmac1
            tagged_exp_pkt['IP'].dst = '10.10.10.1'

            print("Sending packet LAG to port %d, (192.168.0.1 -> 10.10.10.1)"
                  % (self.dev_port24))
            send_packet(self, self.dev_port30, pkt)
            verify_packets(self, exp_pkt, [self.dev_port24])
            self.vlan200_rif_counter_in += 1
            self.vlan100_rif_counter_out += 1

            pkt['Ethernet'].src = self.dmac5
            pkt['IP'].dst = '10.10.10.2'
            tagged_exp_pkt['Ethernet'].dst = self.dmac2
            tagged_exp_pkt['IP'].dst = '10.10.10.2'

            print("Sending packet LAG to port %d, (192.168.0.1 -> 10.10.10.2)"
                  % (self.dev_port27))
            send_packet(self, self.dev_port30, pkt)
            verify_packets(self, tagged_exp_pkt, [self.dev_port27])
            self.vlan200_rif_counter_in += 1
            self.vlan100_rif_counter_out += 1

        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry_lag10)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry27)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry24)
            sai_thrift_remove_vlan_member(self.client, vlan_member103)
            sai_thrift_remove_bridge_port(self.client, port27_bp)
            sai_thrift_flush_fdb_entries(self.client)

    def tearDown(self):
        super(SviToSviRoutingTest, self).tearDown()

@group("draft")
class SviIPv4HostPostRoutedFloodTest(L3SviTestHelper):
    """
    Verifies post routed flood when static mac entry is missing
    """
    def setUp(self):
        super(SviIPv4HostPostRoutedFloodTest, self).setUp()

    def runTest(self):
        print("\nsviIPv4HostPostRoutedFloodTest()")
        print("Test post routed flood when static mac entry is missing")

        mac_action = SAI_PACKET_ACTION_FORWARD

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)

        print("Sending packet from port %d, (192.168.0.1 -> 10.10.10.1)"
              % self.dev_port10)
        send_packet(self, self.dev_port10, pkt)
        verify_packets(self, exp_pkt, [self.dev_port24, self.dev_port25,
                                       self.dev_port26])
        self.vlan100_rif_counter_out += 3

        print("Install static mac on port %d, should not flood now"
              % self.dev_port24)
        fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                           mac_address='00:11:22:33:44:55',
                                           bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.port24_bp,
                                    packet_action=mac_action)

        print("Sending packet from port %d to port %d, (192.168.0.1 -> "
              "10.10.10.1)" % (self.dev_port10, self.dev_port24))
        send_packet(self, self.dev_port10, pkt)
        verify_packets(self, exp_pkt, [self.dev_port24])
        self.vlan100_rif_counter_out += 1

        print("Removed static mac and now packet "
              "will be flooded after routed")
        sai_thrift_remove_fdb_entry(self.client, fdb_entry)

        print("Sending packet from port %d, (192.168.0.1 -> 10.10.10.1)"
              % self.dev_port10)
        send_packet(self, self.dev_port10, pkt)
        verify_packets(self, exp_pkt, [self.dev_port24, self.dev_port25,
                                       self.dev_port26])
        self.vlan100_rif_counter_out += 3

    def tearDown(self):
        super(SviIPv4HostPostRoutedFloodTest, self).tearDown()

@group("draft")
class SviIPv6HostPostRoutedFloodTest(L3SviTestHelper):
    """
    Verifies IPv6 host post routed flood when static mac entry is missing
    """
    def setUp(self):
        super(SviIPv6HostPostRoutedFloodTest, self).setUp()

    def runTest(self):
        print("\nsviIPv6HostPostRoutedFloodTest()")
        print("Test post routed flood when static mac entry is missing")

        mac_action = SAI_PACKET_ACTION_FORWARD

        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:0000',
            ipv6_src='2000::1',
            ipv6_hlim=64)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:0000',
            ipv6_src='2000::1',
            ipv6_hlim=63)

        print("Sending packet from port %d" % self.dev_port10)
        send_packet(self, self.dev_port10, pkt)
        verify_packets(self, exp_pkt, [
            self.dev_port24,
            self.dev_port25,
            self.dev_port26])
        self.vlan100_rif_counter_out += 3

        print("Install static mac on port %d, "
              "should not flood now" % self.dev_port24)
        fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                           mac_address='00:11:22:33:44:55',
                                           bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.port24_bp,
                                    packet_action=mac_action)

        print("Sending packet from port %d to port %d"
              % (self.dev_port10, self.dev_port24))
        send_packet(self, self.dev_port10, pkt)
        verify_packets(self, exp_pkt, [self.dev_port24])
        self.vlan100_rif_counter_out += 1

        print("Removed static mac and now packet will be "
              "flooded after routed")
        sai_thrift_remove_fdb_entry(self.client, fdb_entry)

        print("Sending packet from port %d" % self.dev_port10)
        send_packet(self, self.dev_port10, pkt)
        verify_packets(self, exp_pkt, [
            self.dev_port24,
            self.dev_port25,
            self.dev_port26])
        self.vlan100_rif_counter_out += 3

    def tearDown(self):
        super(SviIPv6HostPostRoutedFloodTest, self).tearDown()

@group("draft")
class SviIPv4HostStaticMacMoveTest(L3SviTestHelper):
    """
    Verifies static configuration of MAC address on port and change
    of this configuration
    """
    def setUp(self):
        super(SviIPv4HostStaticMacMoveTest, self).setUp()

    def runTest(self):
        print("\nsviIPv4HostStaticMacMoveTest()")

        mac_action = SAI_PACKET_ACTION_FORWARD
        fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                           mac_address='00:11:22:33:44:55',
                                           bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.port24_bp,
                                    packet_action=mac_action)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)

        try:
            print("MAC installed on port %d" % (self.dev_port24))

            print("Sending packet from port %d to port %d, (192.168.0.1 -> "
                  "10.10.10.1)" % (self.dev_port10, self.dev_port24))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt, [self.dev_port24])
            self.vlan100_rif_counter_out += 1

            print("Move MAC to port %d from port %d"
                  % (self.dev_port25, self.dev_port24))
            sai_thrift_set_fdb_entry_attribute(
                self.client, fdb_entry, bridge_port_id=self.port25_bp)

            print("Sending packet from port %d to port %d, (192.168.0.1 -> "
                  "10.10.10.1)" % (self.dev_port10, self.dev_port25))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt, [self.dev_port25])
            self.vlan100_rif_counter_out += 1

        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)

    def tearDown(self):
        super(SviIPv4HostStaticMacMoveTest, self).tearDown()

@group("draft")
class SviIPv6HostStaticMacMoveTest(L3SviTestHelper):
    """
    Verifies static configuration of MAC address on port and change
    of this configuration
    """
    def setUp(self):
        super(SviIPv6HostStaticMacMoveTest, self).setUp()

    def runTest(self):
        print("\nsviIPv6HostStaticMacMoveTest()")

        mac_action = SAI_PACKET_ACTION_FORWARD
        fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                           mac_address='00:11:22:33:44:55',
                                           bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.port24_bp,
                                    packet_action=mac_action)

        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:0000',
            ipv6_src='2000::1',
            ipv6_hlim=64,)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:0000',
            ipv6_src='2000::1',
            ipv6_hlim=63)

        try:
            print("MAC installed on port %d" % (self.dev_port24))

            print("Sending packet from port %d to port %d"
                  % (self.dev_port10, self.dev_port24))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt, [self.dev_port24])
            self.vlan100_rif_counter_out += 1

            print("Move MAC to port %d from port %d"
                  % (self.dev_port25, self.dev_port24))
            sai_thrift_set_fdb_entry_attribute(
                self.client, fdb_entry, bridge_port_id=self.port25_bp)

            print("Sending packet from port %d to port %d"
                  % (self.dev_port10, self.dev_port25))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt, [self.dev_port25])
            self.vlan100_rif_counter_out += 1

        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)

    def tearDown(self):
        super(SviIPv6HostStaticMacMoveTest, self).tearDown()

@group("draft")
class SviRouteDynamicMacTest(L3SviTestHelper):
    """
    Verifies dynamic MAC address managment test for the VLAN router
    interface
    """
    def setUp(self):
        super(SviRouteDynamicMacTest, self).setUp()

    def runTest(self):
        print("\nsviRouteDynamicMacTest()")

        arp_pkt = simple_arp_packet(eth_dst='FF:FF:FF:FF:FF:FF',
                                    eth_src='00:22:22:33:44:55',
                                    arp_op=1,
                                    ip_tgt='10.10.10.1',
                                    ip_snd='10.10.10.2',
                                    hw_snd='00:22:22:33:44:55',
                                    pktlen=100)
        arp_pkt2 = simple_arp_packet(eth_dst='FF:FF:FF:FF:FF:FF',
                                     eth_src='00:22:22:33:44:66',
                                     arp_op=1,
                                     ip_tgt='10.10.10.1',
                                     ip_snd='10.10.10.12',
                                     hw_snd='00:22:22:33:44:55',
                                     pktlen=100)
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.2',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:22:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='10.10.10.2',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)
        pkt2 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                 eth_src='00:22:22:22:22:22',
                                 ip_dst='10.10.10.12',
                                 ip_src='192.168.0.1',
                                 ip_id=105,
                                 ip_ttl=64)
        exp_pkt2 = simple_tcp_packet(eth_dst='00:22:22:33:44:66',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.12',
                                     ip_src='192.168.0.1',
                                     ip_id=105,
                                     ip_ttl=63)

        try:
            print("Sending ARP request on port %d (10.10.10.2 -> "
                  "10.10.10.1) Flood" % self.dev_port25)
            send_packet(self, self.dev_port25, arp_pkt)
            verify_packets(self, arp_pkt, [self.dev_port24, self.dev_port26])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2
            self.vlan100_bcast_in += 1
            self.vlan100_bcast_out += 2
            time.sleep(3)

            print("Nexthop 2 resolved on port %d 00:22:22:33:44:55"
                  % self.dev_port25)
            print("Sending packet port %d -> port %d192.168.0.1 -> "
                  "10.10.10.2) Routed" % (self.dev_port10, self.dev_port25))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt, [self.dev_port25])
            self.vlan100_rif_counter_out += 1

            print("Learn new MAC before creating neighbor")
            print("Sending ARP request on port %d (10.10.10.12 -> "
                  "10.10.10.1) Flood" % self.dev_port25)
            send_packet(self, self.dev_port25, arp_pkt2)
            verify_packets(self, arp_pkt2, [self.dev_port24, self.dev_port26])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2
            self.vlan100_bcast_in += 1
            self.vlan100_bcast_out += 2
            time.sleep(3)

            print("Create neighbor entry for new MAC")
            dmac = '00:22:22:33:44:66'
            neighbor_entry = sai_thrift_neighbor_entry_t(
                rif_id=self.vlan100_rif,
                ip_address=sai_ipaddress('10.10.0.12'))
            sai_thrift_create_neighbor_entry(
                self.client, neighbor_entry, dst_mac_address=dmac)
            nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress('10.10.0.12'),
                router_interface_id=self.vlan100_rif,
                type=SAI_NEXT_HOP_TYPE_IP)
            route_entry = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('10.10.10.12/32'))
            sai_thrift_create_route_entry(
                self.client, route_entry, next_hop_id=nhop)

            print("Nexthop 2 resolved on port %d 00:22:22:33:44:66"
                  % self.dev_port25)
            print("Sending packet port %d -> port %d 192.168.0.1 -> "
                  "10.10.10.12) Routed" % (self.dev_port10, self.dev_port25))
            send_packet(self, self.dev_port10, pkt2)
            verify_packets(self, exp_pkt2, [self.dev_port25])
            self.vlan100_rif_counter_out += 1

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop)
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry)
            sai_thrift_flush_fdb_entries(self.client)

    def tearDown(self):
        super(SviRouteDynamicMacTest, self).tearDown()

@group("draft")
class SviRouteDynamicMacMoveTest(L3SviTestHelper):
    """
    Verifies routing after NHOP resolved via dynamically learn MAC entry
    is moved
    """
    def setUp(self):
        super(SviRouteDynamicMacMoveTest, self).setUp()

    def runTest(self):
        print("\nsviRouteDynamicMacMoveTest()")

        arp_pkt = simple_arp_packet(eth_dst='FF:FF:FF:FF:FF:FF',
                                    eth_src='00:22:22:33:44:55',
                                    arp_op=1,
                                    ip_tgt='10.10.10.1',
                                    ip_snd='10.10.10.2',
                                    hw_snd='00:22:22:33:44:55',
                                    pktlen=100)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.2',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:22:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='10.10.10.2',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)

        try:
            print("Sending ARP request on port %d (10.10.10.2 -> "
                  "10.10.10.1) Flood" % self.dev_port25)
            send_packet(self, self.dev_port25, arp_pkt)
            verify_packets(self, arp_pkt, [self.dev_port24, self.dev_port26])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2
            self.vlan100_bcast_in += 1
            self.vlan100_bcast_out += 2
            time.sleep(3)

            print("Nexthop 2 resolved on port %d 00:22:22:33:44:55"
                  % self.dev_port25)
            print("Sending packet port %d -> port %d 192.168.0.1 -> "
                  "10.10.10.2) Routed" % (self.dev_port10, self.dev_port25))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt, [self.dev_port25])
            self.vlan100_rif_counter_out += 1

            # Src MAC move to port 4
            print("Sending ARP request on port %d (10.10.10.2 -> "
                  "10.10.10.1) Flood" % self.dev_port26,)
            send_packet(self, self.dev_port26, arp_pkt)
            verify_packets(self, arp_pkt, [self.dev_port24, self.dev_port25])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2
            self.vlan100_bcast_in += 1
            self.vlan100_bcast_out += 2
            time.sleep(3)

            print("Nexthop 2 resolved on port %d 00:22:22:33:44:55"
                  % self.dev_port25)
            print("Sending packet port %d -> port %d 192.168.0.1 -> "
                  "10.10.10.2) Routed" % (self.dev_port10, self.dev_port26))
            pkt[IP].dst = '10.10.10.2'
            exp_pkt[IP].dst = '10.10.10.2'
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt, [self.dev_port26])
            self.vlan100_rif_counter_out += 1

        finally:
            sai_thrift_flush_fdb_entries(self.client)

    def tearDown(self):
        super(SviRouteDynamicMacMoveTest, self).tearDown()

@group("draft")
class SviIPv4ArpMoveTest(L3SviTestHelper):
    """
    Verifies routing after NHOP resolved via dynamically learn MAC entry
    for the IPv4 standard
    """
    def setUp(self):
        super(SviIPv4ArpMoveTest, self).setUp()

    def runTest(self):
        print("\nsviIPv4ArpMoveTest()")
        print("Test routing after NHOP resolved via dynamically learn "
              "MAC entry")

        arp_pkt1 = simple_arp_packet(eth_dst='FF:FF:FF:FF:FF:FF',
                                     eth_src='00:22:22:33:44:55',
                                     arp_op=1,
                                     ip_tgt='10.10.10.1',
                                     ip_snd='10.10.10.2',
                                     hw_snd='00:22:22:33:44:55',
                                     pktlen=100)
        arp_pkt2 = simple_arp_packet(eth_dst='FF:FF:FF:FF:FF:FF',
                                     eth_src='00:22:22:33:44:66',
                                     arp_op=1,
                                     ip_tgt='10.10.10.1',
                                     ip_snd='10.10.10.2',
                                     hw_snd='00:22:22:33:44:66',
                                     pktlen=100)
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.2',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(eth_dst='00:22:22:33:44:55',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.2',
                                     ip_src='192.168.0.1',
                                     ip_id=105,
                                     ip_ttl=63)
        exp_pkt2 = simple_tcp_packet(eth_dst='00:22:22:33:44:66',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.2',
                                     ip_src='192.168.0.1',
                                     ip_id=105,
                                     ip_ttl=63)

        try:
            # Learn 00:22:22:33:44:55
            print("Sending ARP request on port %d (10.10.10.2 -> "
                  "10.10.10.1) Flood" % self.dev_port25)
            send_packet(self, self.dev_port25, arp_pkt1)
            verify_packets(self, arp_pkt1, [self.dev_port24, self.dev_port26])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2
            self.vlan100_bcast_in += 1
            self.vlan100_bcast_out += 2

            # Learn 00:22:22:33:44:66
            print("Sending ARP request on port %d (10.10.10.2 -> "
                  "10.10.10.1) Flood" % self.dev_port26,)
            send_packet(self, self.dev_port26, arp_pkt2)
            verify_packets(self, arp_pkt2, [self.dev_port24, self.dev_port25])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2
            self.vlan100_bcast_in += 1
            self.vlan100_bcast_out += 2
            time.sleep(3)

            print("Sending packet port %d -> port %d (192.168.0.1 -> "
                  "10.10.10.2) Routed" % (self.dev_port10, self.dev_port25))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt1, [self.dev_port25])
            self.vlan100_rif_counter_out += 1

            print("Update neighbor to 00:22:22:33:44:66")
            sai_thrift_set_neighbor_entry_attribute(
                self.client,
                self.neighbor_entry2,
                dst_mac_address="00:22:22:33:44:66")

            print("Sending packet port %d -> port %d (192.168.0.1 -> "
                  "10.10.10.2) Routed" % (self.dev_port10, self.dev_port26))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt2, [self.dev_port26])
            self.vlan100_rif_counter_out += 1

            print("Update neighbor back to 00:22:22:33:44:55")
            sai_thrift_set_neighbor_entry_attribute(
                self.client,
                self.neighbor_entry2,
                dst_mac_address="00:22:22:33:44:55")

            print("Sending packet port %d -> port %d (192.168.0.1 -> "
                  "10.10.10.2) Routed" % (self.dev_port10, self.dev_port25))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt1, [self.dev_port25])
            self.vlan100_rif_counter_out += 1

        finally:
            sai_thrift_flush_fdb_entries(self.client)

    def tearDown(self):
        super(SviIPv4ArpMoveTest, self).tearDown()

@group("draft")
class SviIPv6IcmpMoveTest(L3SviTestHelper):
    """
    Verifies routing after NHOP resolved via dynamically learn MAC entry
    for the IPv6 standard
    """
    def setUp(self):
        super(SviIPv6IcmpMoveTest, self).setUp()

    def runTest(self):
        print("\nsviIPv6IcmpMoveTest()")
        print("Test routing after NHOP resolved via dynamically learn "
              "MAC entry")

        icmp_pkt1 = simple_icmpv6_packet(
            eth_dst='33:33:00:FF:FF:FF',
            eth_src='00:22:22:33:44:55',
            ipv6_src='1234:5678:9abc:def0:4422:1133:5577:2222',
            ipv6_dst='FF02::1',
            ipv6_hlim=64)
        icmp_pkt2 = simple_icmpv6_packet(
            eth_dst='33:33:00:FF:FF:FF',
            eth_src='00:22:22:33:44:66',
            ipv6_src='1234:5678:9abc:def0:4422:1133:5577:2222',
            ipv6_dst='FF02::1',
            ipv6_hlim=64)
        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:2222',
            ipv6_src='2000::1',
            ipv6_hlim=64,)
        exp_pkt1 = simple_tcpv6_packet(
            eth_dst='00:22:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:2222',
            ipv6_src='2000::1',
            ipv6_hlim=63)
        exp_pkt2 = simple_tcpv6_packet(
            eth_dst='00:22:22:33:44:66',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:2222',
            ipv6_src='2000::1',
            ipv6_hlim=63)

        try:
            # Learn 00:22:22:33:44:55
            print("Sending ICMP request on port %d" % self.dev_port25,
                  " Flood")
            send_packet(self, self.dev_port25, icmp_pkt1)
            verify_packets(self, icmp_pkt1, [self.dev_port24, self.dev_port26])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2
            self.vlan100_bcast_in += 1
            self.vlan100_bcast_out += 2

            # Learn 00:22:22:33:44:66
            print("Sending ICMP request on port %d" % self.dev_port26,
                  " Flood")
            send_packet(self, self.dev_port26, icmp_pkt2)
            verify_packets(self, icmp_pkt2, [self.dev_port24, self.dev_port25])
            self.vlan100_rif_counter_in += 1
            self.vlan100_rif_counter_out += 2
            self.vlan100_bcast_in += 1
            self.vlan100_bcast_out += 2
            time.sleep(3)

            print("Sending packet port %d" % self.dev_port10,
                  " -> port %d" % self.dev_port25,
                  " Routed")
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt1, [self.dev_port25])
            self.vlan100_rif_counter_out += 1

            print("Update neighbor to 00:22:22:33:44:66")
            sai_thrift_set_neighbor_entry_attribute(
                self.client,
                self.neighbor_entry2,
                dst_mac_address="00:22:22:33:44:66")
            print("Sending packet port %d" % self.dev_port10,
                  " -> port %d" % self.dev_port26,
                  " Routed")
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt2, [self.dev_port26])
            self.vlan100_rif_counter_out += 1

            print("Update neighbor back to 00:22:22:33:44:55")
            sai_thrift_set_neighbor_entry_attribute(
                self.client,
                self.neighbor_entry2,
                dst_mac_address="00:22:22:33:44:55")
            print("Sending packet port %d" % self.dev_port10,
                  " -> port %d" % self.dev_port25,
                  " Routed")
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt1, [self.dev_port25])
            self.vlan100_rif_counter_out += 1

        finally:
            sai_thrift_flush_fdb_entries(self.client)

    def tearDown(self):
        super(SviIPv6IcmpMoveTest, self).tearDown()

@group("draft")
class SviLagHostTest(L3SviTestHelper):
    """
    Verifies routing after NHOP resolved via static MAC entry on LAG
    """
    def setUp(self):
        super(SviLagHostTest, self).setUp()

    def runTest(self):
        print("\nsviLagHostTest()")
        print("Test routing after NHOP resolved via static MAC entry")

        mac_action = SAI_PACKET_ACTION_FORWARD
        fdb_entry1 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                            mac_address='00:11:33:33:44:55',
                                            bv_id=self.vlan200)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry1,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.lag10_bp,
                                    packet_action=mac_action)

        fdb_entry2 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                            mac_address='00:22:33:33:44:55',
                                            bv_id=self.vlan200)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry2,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.lag11_bp,
                                    packet_action=mac_action)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='20.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:33:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='20.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)
        pkt1 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                 eth_src='00:22:22:22:22:22',
                                 ip_dst='20.10.10.2',
                                 ip_src='192.168.0.1',
                                 ip_id=105,
                                 ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(eth_dst='00:22:33:33:44:55',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='20.10.10.2',
                                     ip_src='192.168.0.1',
                                     ip_id=105,
                                     ip_ttl=63)
        pkt2 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                 eth_src='00:22:22:22:22:22',
                                 ip_dst='21.11.11.1',
                                 ip_src='192.168.0.1',
                                 ip_id=105,
                                 ip_ttl=64)
        exp_pkt2 = simple_tcp_packet(eth_dst='00:44:33:33:44:55',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='21.11.11.1',
                                     ip_src='192.168.0.1',
                                     ip_id=105,
                                     ip_ttl=63)

        try:
            print("Sending packet port %d to lag 3, (192.168.0.1 -> "
                  "20.10.10.1) Routed" % (self.dev_port11))
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port30, self.dev_port31])
            self.vlan200_rif_counter_out += 1

            print("Sending packet port %d to lag 4, (192.168.0.1 -> "
                  "20.10.10.2) Routed" % (self.dev_port11))
            send_packet(self, self.dev_port11, pkt1)
            verify_packet_any_port(
                self, exp_pkt1, [self.dev_port28, self.dev_port29])
            self.vlan200_rif_counter_out += 1

            print("Sending packet port %d to port %d (192.168.0.1 -> "
                  "21.11.11.1) Routed" % (self.dev_port15, self.dev_port11))
            send_packet(self, self.dev_port30, pkt2)
            verify_packet(self, exp_pkt2, self.dev_port11)
            self.vlan200_rif_counter_in += 1

            print("Sending packet port %d to port %d, (192.168.0.1 -> "
                  "21.11.11.1) Routed" % (self.dev_port16, self.dev_port11))
            send_packet(self, self.dev_port31, pkt2)
            verify_packet(self, exp_pkt2, self.dev_port11)
            self.vlan200_rif_counter_in += 1

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

    def tearDown(self):
        super(SviLagHostTest, self).tearDown()

@group("draft")
class SviIPv4LagHostStaticMacMoveTest(L3SviTestHelper):
    """
    Verifies moving MAC address between ports on LAG for the IPv4
    """
    def setUp(self):
        super(SviIPv4LagHostStaticMacMoveTest, self).setUp()

    def runTest(self):
        print("\nsviIPv4LagHostStaticMacMoveTest()")

        mac_action = SAI_PACKET_ACTION_FORWARD
        fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                           mac_address='00:11:33:33:44:55',
                                           bv_id=self.vlan200)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.lag10_bp,
                                    packet_action=mac_action)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='20.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:33:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='20.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)

        try:
            print("Sending packet port %d to lag 10, (192.168.0.1 -> "
                  "20.10.10.1) Routed" % (self.dev_port11))
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port30, self.dev_port31])
            self.vlan200_rif_counter_out += 1

            print("Move MAC to lag 11 from lag 10")
            sai_thrift_set_fdb_entry_attribute(
                self.client, fdb_entry, bridge_port_id=self.lag11_bp)

            print("Sending packet port %d to lag 11, (192.168.0.1 -> "
                  "20.10.10.1) Routed" % (self.dev_port11))
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port28, self.dev_port29])
            self.vlan200_rif_counter_out += 1

        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)

    def tearDown(self):
        super(SviIPv4LagHostStaticMacMoveTest, self).tearDown()

@group("draft")
class SviIPv6LagHostStaticMacMoveTest(L3SviTestHelper):
    """
    Verfies moving MAC address between ports on LAG for the IPv6
    """
    def setUp(self):
        super(SviIPv6LagHostStaticMacMoveTest, self).setUp()

    def runTest(self):
        print("\nsviIPv6LagHostStaticMacMoveTest()")

        mac_action = SAI_PACKET_ACTION_FORWARD
        fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                           mac_address='00:11:33:33:44:55',
                                           bv_id=self.vlan200)
        sai_thrift_create_fdb_entry(self.client,
                                    fdb_entry,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.lag10_bp,
                                    packet_action=mac_action)

        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_src='2000::1',
            ipv6_hlim=64,)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:11:33:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_src='2000::1',
            ipv6_hlim=63)

        try:
            print("Sending packet port %d to lag 10, Routed"
                  % (self.dev_port11))
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port30, self.dev_port31])
            self.vlan200_rif_counter_out += 1

            print("Move MAC to lag 11 from lag 10")
            sai_thrift_set_fdb_entry_attribute(
                self.client, fdb_entry, bridge_port_id=self.lag11_bp)

            print("Sending packet port %d to lag 11, Routed"
                  % (self.dev_port11))
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port28, self.dev_port29])
            self.vlan200_rif_counter_out += 1

        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)

    def tearDown(self):
        super(SviIPv6LagHostStaticMacMoveTest, self).tearDown()

@group("draft")
class SviLagHostDynamicMacTest(L3SviTestHelper):
    """
    Verifies routing after NHOP resolved via dynamically learn
    MAC entry for LAG
    """
    def setUp(self):
        super(SviLagHostDynamicMacTest, self).setUp()

    def runTest(self):
        print("\nsviLagHostDynamicMacTest()")
        print("Test routing after NHOP resolved via dynamically learn"
              "MAC entry")

        arp_pkt = simple_arp_packet(eth_dst="FF:FF:FF:FF:FF:FF",
                                    eth_src='00:22:33:33:44:55',
                                    arp_op=1,
                                    ip_tgt='20.10.10.1',
                                    ip_snd='20.10.10.2',
                                    hw_snd='00:22:22:33:44:55',
                                    pktlen=100)
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='20.10.10.2',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:22:33:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='20.10.10.2',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)

        try:
            # src MAC learnt on port 2 resulting in nhop2 resolution
            print("Sending ARP request on port %d" % self.dev_port30,
                  " (10.10.10.2 -> 20.10.10.1) Flood on lag 4")
            send_packet(self, self.dev_port30, arp_pkt)
            verify_packet_any_port(
                self, arp_pkt, [self.dev_port28, self.dev_port29])
            self.vlan200_rif_counter_in += 1
            self.vlan200_rif_counter_out += 1
            self.vlan200_bcast_in += 1
            self.vlan200_bcast_out += 1
            time.sleep(3)

            print("Nexthop 2 resolved on lag 10", "00:22:33:33:44:55")
            print("Sending ARP request on port %d" % self.dev_port11,
                  " (10.10.10.2 -> 20.10.10.1) Flood on lag 3")
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port30, self.dev_port31])
            self.vlan200_rif_counter_out += 1

        finally:
            sai_thrift_flush_fdb_entries(self.client)

    def tearDown(self):
        super(SviLagHostDynamicMacTest, self).tearDown()

@group("draft")
class SviIPv4LagHostDynamicMacMoveTest(L3SviTestHelper):
    """
    Verifies routing after NHOP resolved via dynamically learn MAC entry
    is moved for the IPv4 on LAG
    """
    def setUp(self):
        super(SviIPv4LagHostDynamicMacMoveTest, self).setUp()

    def runTest(self):
        print("\nSVIIPv4LagHostDynamicMacMoveTest()")
        print("Test routing after NHOP resolved via dynamically learn "
              "MAC entry")

        arp_pkt = simple_arp_packet(eth_dst="FF:FF:FF:FF:FF:FF",
                                    eth_src='00:22:33:33:44:55',
                                    arp_op=1,
                                    ip_tgt='20.10.10.1',
                                    ip_snd='20.10.10.2',
                                    hw_snd='00:22:22:33:44:55',
                                    pktlen=100)
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='20.10.10.2',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:22:33:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='20.10.10.2',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)

        try:
            # src MAC learnt on port 14 resulting in nhop2 resolution
            print("Sending ARP request on port %d" % self.dev_port30,
                  " (10.10.10.2 -> 20.10.10.1) Flood on lag 4")
            send_packet(self, self.dev_port30, arp_pkt)
            verify_packet_any_port(
                self, arp_pkt, [self.dev_port28, self.dev_port29])
            self.vlan200_rif_counter_in += 1
            self.vlan200_rif_counter_out += 1
            self.vlan200_bcast_in += 1
            self.vlan200_bcast_out += 1
            time.sleep(3)

            print("Nexthop 2 resolved on lag 10", "00:22:33:33:44:55")
            print("Sending ARP request on port %d" % self.dev_port11,
                  " (10.10.10.2 -> 20.10.10.1) Flood on lag 3")
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port30, self.dev_port31])
            self.vlan200_rif_counter_out += 1

            # src MAC move to port 17
            print("Sending ARP request on port %d" % self.dev_port28,
                  " (10.10.10.2 -> 20.10.10.1) Flood on lag 3")
            send_packet(self, self.dev_port28, arp_pkt)
            verify_packet_any_port(
                self, arp_pkt, [self.dev_port30, self.dev_port31])
            self.vlan200_rif_counter_in += 1
            self.vlan200_rif_counter_out += 1
            self.vlan200_bcast_in += 1
            self.vlan200_bcast_out += 1
            time.sleep(3)

            print("Nexthop 2 resolved on lag 10", "00:22:33:33:44:55")
            print("Sending ARP request on port %d" % self.dev_port11,
                  " (10.10.10.2 -> 20.10.10.1) Flood on lag 11")
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port28, self.dev_port29])
            self.vlan200_rif_counter_out += 1

        finally:
            sai_thrift_flush_fdb_entries(self.client)

    def tearDown(self):
        super(SviIPv4LagHostDynamicMacMoveTest, self).tearDown()

@group("draft")
class SviIPv6LagHostDynamicMacMoveTest(L3SviTestHelper):
    """
    Verifies routing after NHOP resolved via dynamically learn MAC entry
    is moved for the IPv6 on LAG
    """
    def setUp(self):
        super(SviIPv6LagHostDynamicMacMoveTest, self).setUp()

    def runTest(self):
        print("\nsviIPv6LagHostDynamicMacMoveTest()")
        print("Test routing after NHOP resolved via dynamically learn"
              "MAC entry")

        icmp_pkt = simple_icmpv6_packet(
            eth_dst='33:33:00:FF:FF:FF',
            eth_src='00:22:33:33:44:55',
            ipv6_src='1234:5678:9abc:def0:1122:3344:5566:7788',
            ipv6_dst='FF02::1',
            ipv6_hlim=64)
        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:1122:3344:5566:7788',
            ipv6_src='2000::1',
            ipv6_hlim=64,)
        exp_pkt = simple_tcpv6_packet(
            eth_dst='00:22:33:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:1122:3344:5566:7788',
            ipv6_src='2000::1',
            ipv6_hlim=63)

        try:
            # src MAC learnt on port 14 resulting in nhop2 resolution
            print("Sending ICMP request on port %d" % self.dev_port30,
                  " Flood on lag 11")
            send_packet(self, self.dev_port30, icmp_pkt)
            verify_packet_any_port(
                self, icmp_pkt, [self.dev_port28, self.dev_port29])
            self.vlan200_rif_counter_in += 1
            self.vlan200_rif_counter_out += 1
            self.vlan200_bcast_in += 1
            self.vlan200_bcast_out += 1
            time.sleep(3)

            print("Nexthop 2 resolved on lag 10", "00:22:33:33:44:55")
            print("Sending ICMP request on port %d" % self.dev_port11,
                  " Flood on lag 10")
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port30, self.dev_port31])
            self.vlan200_rif_counter_out += 1

            # src MAC move to port 17
            print("Sending ICMP request on port %d" % self.dev_port28,
                  " Flood on lag 10")
            send_packet(self, self.dev_port28, icmp_pkt)
            verify_packet_any_port(
                self, icmp_pkt, [self.dev_port30, self.dev_port31])
            self.vlan200_rif_counter_in += 1
            self.vlan200_rif_counter_out += 1
            self.vlan200_bcast_in += 1
            self.vlan200_bcast_out += 1
            time.sleep(3)

            print("Nexthop 2 resolved on lag 10", "00:22:33:33:44:55")
            print("Sending ICMP request on port %d" % self.dev_port11,
                  " Flood on lag 11")
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port28, self.dev_port29])
            self.vlan200_rif_counter_out += 1

        finally:
            sai_thrift_flush_fdb_entries(self.client)

    def tearDown(self):
        super(SviIPv6LagHostDynamicMacMoveTest, self).tearDown()
@group("draft")
class SviIPv4MtuTest(L3SviTestHelper):
    """
    Verifies if IPv4 packet is forwarded, dropped and punted to CPU
    depending on the MTU with and without a trap for SVI
    """
    def setUp(self):
        super(SviIPv4MtuTest, self).setUp()

    def runTest(self):
        print("\nsviIPv4MtuTest()")

        # set MTU to 200 for vlan100_rif
        sai_thrift_set_router_interface_attribute(
            self.client, self.vlan100_rif, mtu=200)

        try:
            mac_action = SAI_PACKET_ACTION_FORWARD
            fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                               mac_address='00:11:22:33:44:55',
                                               bv_id=self.vlan100)
            sai_thrift_create_fdb_entry(self.client,
                                        fdb_entry,
                                        type=SAI_FDB_ENTRY_TYPE_STATIC,
                                        bridge_port_id=self.port24_bp,
                                        packet_action=mac_action)

            print("Max MTU is 200, send pkt size 199, send to port")
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=199 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.10.1',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=199 + 14)
            print("Sending packet port %d -> port %d"
                  % (self.dev_port10, self.dev_port24))
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port24)
            self.vlan100_rif_counter_out += 1

            print("Max MTU is 200, send pkt size 200, send to port")
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=200 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.10.1',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=200 + 14)
            print("Sending packet port %d -> port %d"
                  % (self.dev_port10, self.dev_port24))
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port24)
            self.vlan100_rif_counter_out += 1

            print("Max MTU is 200, send pkt size 201, drop")
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=201 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.10.1',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=201 + 14)
            print("Sending packet port %d" % self.dev_port10, " dropped")
            send_packet(self, self.dev_port10, pkt)
            verify_no_other_packets(self, timeout=1)
            self.vlan100_rif_counter_out += 1

            # set MTU to 201 for vlan100_rif
            print("Setting MTU to 201, send pkt size 201, sent to port")
            sai_thrift_set_router_interface_attribute(
                self.client, self.vlan100_rif, mtu=201)

            print("Sending packet port %d -> port %d"
                  % (self.dev_port10, self.dev_port24))
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port24)
            self.vlan100_rif_counter_out += 1

            print("Max MTU is 201, send pkt size 202, drop")
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=202 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.10.1',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=202 + 14)
            print("Sending packet port %d" % self.dev_port10, " dropped")
            send_packet(self, self.dev_port10, pkt)
            verify_no_other_packets(self, timeout=1)
            self.vlan100_rif_counter_out += 1

        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)

    def tearDown(self):
        super(SviIPv4MtuTest, self).tearDown()

@group("draft")
class SviIPv6MtuTest(L3SviTestHelper):
    """
    Verifies if IPv6 packet is forwarded, dropped and punted to CPU
    depending on the MTU with and without a trap for SVI
    """
    def setUp(self):
        super(SviIPv6MtuTest, self).setUp()

    def runTest(self):
        print("\nsviIPv6MtuTest()")

        # set MTU to 200 for vlan200_rif
        sai_thrift_set_router_interface_attribute(
            self.client, self.vlan200_rif, mtu=200)

        try:
            mac_action = SAI_PACKET_ACTION_FORWARD
            fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                               mac_address='00:11:33:33:44:55',
                                               bv_id=self.vlan200)
            sai_thrift_create_fdb_entry(self.client,
                                        fdb_entry,
                                        type=SAI_FDB_ENTRY_TYPE_STATIC,
                                        bridge_port_id=self.lag10_bp,
                                        packet_action=mac_action)

            print("Max MTU is 200, send pkt size 199, send to lag")
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=199 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:33:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=199 + 14 + 40)

            print("Sending packet port %d" % self.dev_port11, " -> lag 10")
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port30, self.dev_port31])
            self.vlan200_rif_counter_out += 1

            print("Max MTU is 200, send pkt size 200, send to lag")
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=200 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:33:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=200 + 14 + 40)

            print("Sending packet port %d" % self.dev_port11, " -> lag 10")
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port30, self.dev_port31])
            self.vlan200_rif_counter_out += 1

            print("Max MTU is 200, send pkt size 201, drop")
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=201 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:33:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=201 + 14 + 40)

            print("Sending packet port %d" % self.dev_port11, " drop")
            send_packet(self, self.dev_port11, pkt)
            verify_no_other_packets(self, timeout=1)
            self.vlan200_rif_counter_out += 1

            # set MTU to 200 for vlan200_rif
            sai_thrift_set_router_interface_attribute(
                self.client, self.vlan200_rif, mtu=201)

            print("Setting MTU to 201, send pkt size 201, sent to lag")
            print("Sending packet port %d" % self.dev_port11, " -> lag 10")
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port30, self.dev_port31])
            self.vlan200_rif_counter_out += 1

            print("Max MTU is 201, send pkt size 202, drop")
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=202 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:33:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=202 + 14 + 40)

            print("Sending packet port %d" % self.dev_port11, " drop")
            send_packet(self, self.dev_port11, pkt)
            verify_no_other_packets(self, timeout=1)
            self.vlan200_rif_counter_out += 1

        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)

    def tearDown(self):
        super(SviIPv6MtuTest, self).tearDown()

@group("draft")
class SviTaggingTest(L3SviTestHelper):
    """
    Verifies if packet is not routed if tagged packet ingresses on
    untagged SVI and if packet is dropped if unknown vlan tagged packet
    ingresses on tagged SVI
    """
    def setUp(self):
        super(SviTaggingTest, self).setUp()

    def runTest(self):
        print("\nsviTaggingTest()")

        vlan_id = 100

        nhop_ip = "30.30.30.1"
        dst_ip = "30.30.30.10"
        src_ip = "10.10.10.10"
        dst_mac = "00:12:34:56:78:90"
        src_mac = "00:09:87:65:43:21"

        untagged_port = self.dev_port24
        tagged_port = self.dev_port27

        test_erif = self.port12_rif

        tag_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip,
                                    ip_src=src_ip,
                                    dl_vlan_enable=True,
                                    vlan_vid=vlan_id + 1,
                                    ip_ttl=64,
                                    pktlen=104)

        try:
            # add tagged VLAN member
            port27_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=self.port27,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)

            vlan_member103 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan100,
                bridge_port_id=port27_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

            nbor = sai_thrift_neighbor_entry_t(
                rif_id=test_erif,
                ip_address=sai_ipaddress(nhop_ip))
            sai_thrift_create_neighbor_entry(
                self.client, nbor, dst_mac_address=dst_mac)

            nhop = sai_thrift_create_next_hop(self.client,
                                              ip=sai_ipaddress(nhop_ip),
                                              router_interface_id=test_erif,
                                              type=SAI_NEXT_HOP_TYPE_IP)

            route_entry = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(nhop_ip + '/24'))
            sai_thrift_create_route_entry(
                self.client, route_entry, next_hop_id=nhop)

            print("Sending tagged packet on untagged SVI port")
            send_packet(self, untagged_port, tag_pkt)
            verify_no_other_packets(self)
            print("Dropped")

            print("Sending unknown tagged packet on tagged SVI port")
            send_packet(self, tagged_port, tag_pkt)
            verify_no_other_packets(self)
            print("Dropped")

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop)
            sai_thrift_remove_neighbor_entry(self.client, nbor)
            sai_thrift_remove_vlan_member(self.client, vlan_member103)
            sai_thrift_remove_bridge_port(self.client, port27_bp)

    def tearDown(self):
        super(SviTaggingTest, self).tearDown()

@group("draft")
class SviMyIPTest(L3SviTestHelper):
    """
    Verifies if MYIP works for subnet routes
    """
    def setUp(self):
        super(SviMyIPTest, self).setUp()

    def runTest(self):
        print("\nsviMyIPTest()")

        vlan_id = 10

        test_vlan = self.vlan10
        untagged_port = self.dev_port0
        tagged_port = self.dev_port1

        try:
            # create SVI on VLAN 10
            vlan10_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
                virtual_router_id=self.default_vrf,
                vlan_id=test_vlan)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, cpu_port=True)
            cpu_port = sw_attr['cpu_port']

            ip2me_route = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix('10.10.20.1/32'))
            sai_thrift_create_route_entry(
                self.client, ip2me_route, next_hop_id=cpu_port)

            myip_trap_group = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True, queue=4)
            myip_trap = sai_thrift_create_hostif_trap(
                self.client,
                trap_group=myip_trap_group,
                packet_action=SAI_PACKET_ACTION_TRAP,
                trap_type=SAI_HOSTIF_TRAP_TYPE_IP2ME)
            self.assertTrue(myip_trap != 0)

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.20.1',
                                    ip_src='30.30.0.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=100)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            send_packet(self, untagged_port, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.20.1',
                                    ip_src='30.30.0.1',
                                    dl_vlan_enable=True,
                                    vlan_vid=vlan_id,
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=100)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            send_packet(self, tagged_port, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

        finally:
            sai_thrift_remove_hostif_trap(self.client, myip_trap)
            sai_thrift_remove_hostif_trap_group(self.client, myip_trap_group)
            sai_thrift_remove_route_entry(self.client, ip2me_route)
            sai_thrift_remove_router_interface(self.client, vlan10_rif)

    def tearDown(self):
        super(SviMyIPTest, self).tearDown()

@group("draft")
class SviStatsTest(L3SviTestHelper):
    """
    Verifies Ingress and Egress SVI packets stats
    """
    def setUp(self):
        super(SviStatsTest, self).setUp()

    def runTest(self):
        print("\nsviStatsTest()")

        vlan100_rif_stats = sai_thrift_get_router_interface_stats(
            self.client, self.vlan100_rif)
        vlan200_rif_stats = sai_thrift_get_router_interface_stats(
            self.client, self.vlan200_rif)
        vlan100_stats = sai_thrift_get_vlan_stats(self.client, self.vlan100)
        vlan200_stats = sai_thrift_get_vlan_stats(self.client, self.vlan200)

        self.assertTrue(self.vlan100_rif_counter_in == vlan100_rif_stats[
            'SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'])
        self.assertTrue(self.vlan100_rif_counter_out == vlan100_rif_stats[
            'SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS'])
        self.assertTrue(self.vlan200_rif_counter_in == vlan200_rif_stats[
            'SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'])
        self.assertTrue(self.vlan200_rif_counter_out == vlan200_rif_stats[
            'SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS'])
        self.assertTrue(self.vlan100_bcast_in == vlan100_stats[
            'SAI_VLAN_STAT_IN_NON_UCAST_PKTS'])
        self.assertTrue(self.vlan100_bcast_out == vlan100_stats[
            'SAI_VLAN_STAT_OUT_NON_UCAST_PKTS'])

        self.assertTrue((self.vlan100_rif_counter_in - self.vlan100_bcast_in)
                        == vlan100_stats['SAI_VLAN_STAT_IN_UCAST_PKTS'])
        self.assertTrue((self.vlan100_rif_counter_out - self.vlan100_bcast_out)
                        == vlan100_stats['SAI_VLAN_STAT_OUT_UCAST_PKTS'])
        self.assertTrue(self.vlan200_bcast_in ==
                        vlan200_stats['SAI_VLAN_STAT_IN_NON_UCAST_PKTS'])
        self.assertTrue(self.vlan200_bcast_out ==
                        vlan200_stats['SAI_VLAN_STAT_OUT_NON_UCAST_PKTS'])
        self.assertTrue((self.vlan200_rif_counter_in - self.vlan200_bcast_in)
                        == vlan200_stats['SAI_VLAN_STAT_IN_UCAST_PKTS'])
        self.assertTrue((self.vlan200_rif_counter_out - self.vlan200_bcast_out)
                        == vlan200_stats['SAI_VLAN_STAT_OUT_UCAST_PKTS'])

    def tearDown(self):
        super(SviStatsTest, self).tearDown()

@group("draft")
class IncorrectVlanIdTest(L3SviTestHelper):
    """
    Verifies if RIF creation fails when TYPE is VLAN and vlan_id is 0
    """
    def setUp(self):
        super(IncorrectVlanIdTest, self).setUp()

    def runTest(self):
        print("\nincorrectVlanIdTest()")

        try:
            vlan_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
                virtual_router_id=self.default_vrf,
                vlan_id=0)
            self.assertFalse(vlan_rif != 0)

        finally:
            if vlan_rif:
                sai_thrift_remove_router_interface(self.client, vlan_rif)

    def tearDown(self):
        super(IncorrectVlanIdTest, self).tearDown()

@group("draft")
class DuplicateVlanRifCreationTest(L3SviTestHelper):
    """
    Verifies if duplicate VLAN interface creation fails
    """
    def setUp(self):
        super(DuplicateVlanRifCreationTest, self).setUp()

    def runTest(self):
        print("duplicateVlanRifCreationTest")

        existing_rif_vlan = self.vlan30

        try:
            dupl_vlan_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
                virtual_router_id=self.default_vrf,
                vlan_id=existing_rif_vlan)
            self.assertFalse(dupl_vlan_rif != 0)

        finally:
            if dupl_vlan_rif:
                sai_thrift_remove_router_interface(self.client, dupl_vlan_rif)

    def tearDown(self):
        super(DuplicateVlanRifCreationTest, self).tearDown()

@group("draft")
class SviArpReplyTest(L3SviTestHelper):
    """
    Verifies ARP replies from linux interface on tagged and untagged RIF
    """
    def setUp(self):
        super(SviArpReplyTest, self).setUp()

    def runTest(self):
        print("\nsviArpReplyTest()")

        untagged_port = self.port24
        untagged_dev_port = self.dev_port24
        utg_hostif_name = "utg_hostif"
        utg_port_ip = "20.20.0.10"
        utg_port_mac = "00:01:01:01:01:01"
        src_mac1 = "00:aa:aa:aa:aa:01"

        tagged_port = self.port27
        tagged_dev_port = self.dev_port27
        tg_hostif_name = "tg_hostif"
        tg_port_ip = "20.20.0.20"
        tg_port_mac = "00:02:02:02:02:02"
        src_mac2 = "00:aa:aa:aa:aa:02"

        src_ip = "20.20.0.1"

        arp_req_pkt = simple_arp_packet(arp_op=1,
                                        pktlen=100,
                                        eth_src=src_mac1,
                                        hw_snd=src_mac1,
                                        ip_snd=src_ip,
                                        ip_tgt=utg_port_ip)

        arp_resp_pkt = simple_arp_packet(arp_op=2,
                                         pktlen=42,
                                         eth_src=utg_port_mac,
                                         eth_dst=src_mac1,
                                         hw_snd=utg_port_mac,
                                         hw_tgt=src_mac1,
                                         ip_snd=utg_port_ip,
                                         ip_tgt=src_ip)

        tg_arp_req_pkt = simple_arp_packet(arp_op=1,
                                           pktlen=100,
                                           eth_src=src_mac2,
                                           hw_snd=src_mac2,
                                           ip_snd=src_ip,
                                           ip_tgt=tg_port_ip,
                                           vlan_vid=100)

        tg_arp_resp_pkt = simple_arp_packet(arp_op=2,
                                            pktlen=28,
                                            eth_src=tg_port_mac,
                                            eth_dst=src_mac2,
                                            hw_snd=tg_port_mac,
                                            hw_tgt=src_mac2,
                                            ip_snd=tg_port_ip,
                                            ip_tgt=src_ip)

        try:
            # add tagged VLAN member
            port27_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=self.port27,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)

            vlan_member103 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan100,
                bridge_port_id=port27_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

            arp_trap_group = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True, queue=4)
            arp_trap = sai_thrift_create_hostif_trap(
                self.client,
                packet_action=SAI_PACKET_ACTION_TRAP,
                trap_type=SAI_HOSTIF_TRAP_TYPE_ARP_REQUEST)
            self.assertTrue(arp_trap != 0)

            utg_hostif = sai_thrift_create_hostif(self.client,
                                                  name=utg_hostif_name,
                                                  obj_id=untagged_port,
                                                  type=SAI_HOSTIF_TYPE_NETDEV)
            self.assertTrue(utg_hostif != 0)

            utg_hif_socket = open_packet_socket(utg_hostif_name)

            # set untagged port host interface IP address
            os.system("ifconfig %s %s/24" % (utg_hostif_name, utg_port_ip))
            os.system("ifconfig %s hw ether %s"
                      % (utg_hostif_name, utg_port_mac))
            os.system("sudo ifconfig %s up" % utg_hostif_name)
            hostif_ip = os.popen("ifconfig %s | grep 'inet addr' | "
                                 "cut -d: -f2 | awk '{print $1}'"
                                 % utg_hostif_name).read()
            self.assertEqual(hostif_ip.rstrip(), utg_port_ip)

            tg_hostif = sai_thrift_create_hostif(self.client,
                                                 name=tg_hostif_name,
                                                 obj_id=tagged_port,
                                                 type=SAI_HOSTIF_TYPE_NETDEV)
            self.assertTrue(tg_hostif != 0)

            tg_hif_socket = open_packet_socket(tg_hostif_name)

            # set tagged port host interface IP address
            os.system("ifconfig %s %s/24" % (tg_hostif_name, tg_port_ip))
            os.system("ifconfig %s hw ether %s"
                      % (tg_hostif_name, tg_port_mac))
            os.system("sudo ifconfig %s up" % tg_hostif_name)
            hostif_ip = os.popen("ifconfig %s | grep 'inet addr' | "
                                 "cut -d: -f2 | awk '{print $1}'"
                                 % tg_hostif_name).read()
            self.assertEqual(hostif_ip.rstrip(), tg_port_ip)

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            print("Sending ARP request on untagged port")
            send_packet(self, untagged_dev_port, arp_req_pkt)
            self.assertTrue(socket_verify_packet(arp_req_pkt, utg_hif_socket))
            verify_packet(self, arp_resp_pkt, untagged_dev_port)
            print("Response received on the untagged port")
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)
            print("Request received on CPU port")

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            print("Sending ARP request on tagged port")
            send_packet(self, tagged_dev_port, tg_arp_req_pkt)
            self.assertTrue(
                socket_verify_packet(tg_arp_req_pkt, tg_hif_socket))
            verify_packet(self, tg_arp_resp_pkt, tagged_dev_port)
            print("Response received on the tagged port")
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)
            print("Request received on CPU port")

        finally:
            sai_thrift_flush_fdb_entries(self.client)
            sai_thrift_remove_hostif(self.client, tg_hostif)
            sai_thrift_remove_hostif(self.client, utg_hostif)
            sai_thrift_remove_hostif_trap(self.client, arp_trap)
            sai_thrift_remove_hostif_trap_group(self.client, arp_trap_group)
            sai_thrift_remove_vlan_member(self.client, vlan_member103)
            sai_thrift_remove_bridge_port(self.client, port27_bp)

    def tearDown(self):
        super(SviArpReplyTest, self).tearDown()


@group("draft")
@group("mtu-trap")
class L3MtuTrapTestHelper(PlatformSaiHelper):
    """
    This class contains router interface tests where packets are forwarded
    or punted to CPU depending on the MTU
    """

    def setUp(self):
        super(L3MtuTrapTestHelper, self).setUp()

        self.port10_rif_counter_in = 0
        self.port10_rif_counter_out = 0
        self.port11_rif_counter_in = 0
        self.port11_rif_counter_out = 0
        self.lag1_rif_counter_in = 0
        self.lag1_rif_counter_out = 0
        self.subport10_100_in = 0
        self.subport10_100_out = 0
        self.subport11_200_in = 0
        self.subport11_200_out = 0
        self.vlan100_rif_counter_in = 0
        self.vlan100_rif_counter_out = 0

        dmac1 = '00:11:22:33:44:55'
        dmac3 = '00:33:22:33:44:55'

        self.lag1_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag1)

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.10.2'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('10.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=dmac1)

        self.neighbor_entry3 = sai_thrift_neighbor_entry_t(
            rif_id=self.lag1_rif, ip_address=sai_ipaddress('12.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry3, dst_mac_address=dmac3)
        self.nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('12.10.10.2'),
            router_interface_id=self.lag1_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.route_entry0 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry0, next_hop_id=self.nhop1)

        self.route_entry0_lpm = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('11.11.11.0/24'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry0_lpm, next_hop_id=self.nhop1)

        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:99aa/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1, next_hop_id=self.nhop1)

        self.route_entry1_lpm = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('4000::0/65'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1_lpm, next_hop_id=self.nhop1)

        self.route_lag0 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('12.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_lag0, next_hop_id=self.nhop3)

        self.route_lag1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:1122:3344:5566:7788/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_lag1, next_hop_id=self.nhop3)

        # add subport 100 on port10
        self.subport10_100 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port10,
            admin_v4_state=True,
            outer_vlan_id=100)
        self.nhop_sp10_100 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.20.0.10'),
            router_interface_id=self.subport10_100,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_sp10_100 = sai_thrift_neighbor_entry_t(
            rif_id=self.subport10_100, ip_address=sai_ipaddress('20.20.0.10'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry_sp10_100,
                                         dst_mac_address="00:33:33:33:01:00")

        # add subport 200 on port11
        self.subport11_200 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port11,
            admin_v4_state=True,
            outer_vlan_id=200)
        self.nhop_sp11_200 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.20.1.20'),
            router_interface_id=self.subport11_200,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_sp11_200 = sai_thrift_neighbor_entry_t(
            rif_id=self.subport11_200, ip_address=sai_ipaddress('20.20.1.20'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry_sp11_200,
                                         dst_mac_address="00:33:33:33:12:00")

        # add subport routes
        self.route_entry_sp10_100 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('40.40.0.10/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sp10_100,
                                      next_hop_id=self.nhop_sp10_100)

        self.route_entry_sp10_100_ipv6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:8899/128'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sp10_100_ipv6,
                                      next_hop_id=self.nhop_sp10_100)

        self.route_entry_sp11_200 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('40.40.1.20/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry_sp11_200,
                                      next_hop_id=self.nhop_sp11_200)

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

        # vlan100 with members port24 and port25
        self.vlan100 = sai_thrift_create_vlan(self.client, vlan_id=100)
        self.vlan_member100 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_member101 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port25_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_port_attribute(
            self.client, self.port24, port_vlan_id=100)
        sai_thrift_set_port_attribute(
            self.client, self.port25, port_vlan_id=100)

        # create vlan100_rif
        self.vlan100_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan100)

        # create nhop100 and routes on SVI
        self.nhop100 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.1'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry100 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, ip_address=sai_ipaddress('10.10.0.1'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry100, dst_mac_address=dmac1)
        self.route_entry100 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.0.100/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry100, next_hop_id=self.nhop100)
        self.route_entry100_v6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:0000/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry100_v6, next_hop_id=self.nhop100)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry100)
        sai_thrift_remove_route_entry(self.client, self.route_entry100_v6)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry100)
        sai_thrift_remove_next_hop(self.client, self.nhop100)
        sai_thrift_remove_router_interface(self.client, self.vlan100_rif)

        sai_thrift_set_port_attribute(self.client, self.port24, port_vlan_id=0)
        sai_thrift_set_port_attribute(self.client, self.port25, port_vlan_id=0)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member100)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member101)
        sai_thrift_remove_vlan(self.client, self.vlan100)
        sai_thrift_remove_bridge_port(self.client, self.port24_bp)
        sai_thrift_remove_bridge_port(self.client, self.port25_bp)

        sai_thrift_remove_route_entry(self.client, self.route_entry_sp10_100)
        sai_thrift_remove_route_entry(
            self.client, self.route_entry_sp10_100_ipv6)
        sai_thrift_remove_route_entry(self.client, self.route_entry_sp11_200)

        sai_thrift_remove_neighbor_entry(
            self.client, self.neighbor_entry_sp11_200)
        sai_thrift_remove_next_hop(self.client, self.nhop_sp11_200)
        sai_thrift_remove_router_interface(self.client, self.subport11_200)
        sai_thrift_remove_neighbor_entry(
            self.client, self.neighbor_entry_sp10_100)
        sai_thrift_remove_next_hop(self.client, self.nhop_sp10_100)
        sai_thrift_remove_router_interface(self.client, self.subport10_100)

        sai_thrift_remove_route_entry(self.client, self.route_entry0)
        sai_thrift_remove_route_entry(self.client, self.route_entry0_lpm)
        sai_thrift_remove_route_entry(self.client, self.route_entry1)
        sai_thrift_remove_route_entry(self.client, self.route_entry1_lpm)

        sai_thrift_remove_route_entry(self.client, self.route_lag0)
        sai_thrift_remove_route_entry(self.client, self.route_lag1)

        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_next_hop(self.client, self.nhop3)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry3)
        sai_thrift_remove_router_interface(self.client, self.lag1_rif)

        super(L3MtuTrapTestHelper, self).tearDown()

@group("draft")
class Ipv4MtuTrapTest(L3MtuTrapTestHelper):
    """
    Verifies if IPv4 packet is forwarded or punted to CPU as a trap,
    depending on the MTU
    """
    def setUp(self):
        super(Ipv4MtuTrapTest, self).setUp()

    def runTest(self):
        print("\nipv4MtuTrapTest()")

        # set MTU to 200 for port 10 and lag 1
        mtu_port10_rif = sai_thrift_get_router_interface_attribute(
            self.client, self.port10_rif, mtu=True)
        mtu_lag1_rif = sai_thrift_get_router_interface_attribute(
            self.client, self.lag1_rif, mtu=True)

        sai_thrift_set_router_interface_attribute(
            self.client, self.port10_rif, mtu=200)
        sai_thrift_set_router_interface_attribute(
            self.client, self.lag1_rif, mtu=200)

        try:
            trap_group = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True, queue=4)
            trap = sai_thrift_create_hostif_trap(
                self.client,
                trap_group=trap_group,
                trap_type=SAI_HOSTIF_TRAP_TYPE_L3_MTU_ERROR,
                packet_action=SAI_PACKET_ACTION_TRAP)

            print("Max MTU is 200, send pkt size 200, send to port/lag")
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=200 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.10.1',
                                        ip_src='192.168.0.1',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=200 + 14)

            print("Sending packet port %d -> port %d (192.168.0.1 -> "
                  "10.10.10.1)" % (self.dev_port11, self.dev_port10))
            send_packet(self, self.dev_port11, pkt)
            verify_packet(self, exp_pkt, self.dev_port10)
            self.port11_rif_counter_in += 1
            self.port10_rif_counter_out += 1

            pkt['IP'].dst = '12.10.10.1'
            exp_pkt['IP'].dst = '12.10.10.1'
            exp_pkt['Ethernet'].dst = '00:33:22:33:44:55'

            print("Sending packet port %d -> lag 1 (192.168.0.1 -> "
                  "12.10.10.1)" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port4, self.dev_port5,
                                self.dev_port6])
            self.port11_rif_counter_in += 1
            self.lag1_rif_counter_out += 1

            print("Max MTU is 200, send pkt size 201, punted to cpu")
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=201 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.10.1',
                                        ip_src='192.168.0.1',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=201 + 14)

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet port %d" % self.dev_port11, " -> cpu")
            send_packet(self, self.dev_port11, pkt)
            self.port11_rif_counter_in += 1
            self.port10_rif_counter_out += 1
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

            pkt['IP'].dst = '12.10.10.1'

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet port %d" % self.dev_port11, " -> cpu")
            send_packet(self, self.dev_port11, pkt)
            self.port11_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

        finally:
            sai_thrift_remove_hostif_trap(self.client, trap)
            sai_thrift_remove_hostif_trap_group(self.client, trap_group)

            sai_thrift_set_router_interface_attribute(
                self.client, self.port10_rif, mtu=mtu_port10_rif['mtu'])
            sai_thrift_set_router_interface_attribute(
                self.client, self.lag1_rif, mtu=mtu_lag1_rif['mtu'])

    def tearDown(self):
        super(Ipv4MtuTrapTest, self).tearDown()

@group("draft")
class Ipv6MtuTrapTest(L3MtuTrapTestHelper):
    """
    Verifies if IPv6 packet is forwarded or punted to CPU as a trap,
    depending on the MTU
    """
    def setUp(self):
        super(Ipv6MtuTrapTest, self).setUp()

    def runTest(self):
        print("\nipv6MtuTrapTest()")

        mtu_port10_rif = sai_thrift_get_router_interface_attribute(
            self.client, self.port10_rif, mtu=True)
        mtu_lag1_rif = sai_thrift_get_router_interface_attribute(
            self.client, self.lag1_rif, mtu=True)

        # set MTU to 200 for port 10 and lag 1
        sai_thrift_set_router_interface_attribute(
            self.client, self.port10_rif, mtu=200)
        sai_thrift_set_router_interface_attribute(
            self.client, self.lag1_rif, mtu=200)

        try:
            trap_group = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True, queue=4)
            trap = sai_thrift_create_hostif_trap(
                self.client,
                trap_group=trap_group,
                trap_type=SAI_HOSTIF_TRAP_TYPE_L3_MTU_ERROR,
                packet_action=SAI_PACKET_ACTION_TRAP)

            print("Max MTU is 200, send pkt size 200, send to port/lag")
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=200 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:22:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=200 + 14 + 40)

            print("Sending packet port %d -> port %d "
                  "(2000::1 -> 1234:5678:9abc:def0:4422:1133:5577:99aa')"
                  % (self.dev_port11, self.dev_port10))
            send_packet(self, self.dev_port11, pkt)
            verify_packet(self, exp_pkt, self.dev_port10)
            self.port11_rif_counter_in += 1
            self.port10_rif_counter_out += 1

            pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'
            exp_pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'
            exp_pkt['Ethernet'].dst = '00:33:22:33:44:55'

            print("Sending packet port %d -> lag 1 (2000::1 -> "
                  "1234:5678:9abc:def0:1122:3344:5566:7788)"
                  % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            verify_packet_any_port(
                self, exp_pkt, [self.dev_port4, self.dev_port5,
                                self.dev_port6])
            self.port11_rif_counter_in += 1
            self.lag1_rif_counter_out += 1

            print("Max MTU is 200, send pkt size 201, punted to cpu")
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=201 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:22:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=201 + 14 + 40)

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet port %d" % self.dev_port11, " -> cpu")
            send_packet(self, self.dev_port11, pkt)
            self.port11_rif_counter_in += 1
            self.port10_rif_counter_out += 1
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

            pkt['IPv6'].dst = '1234:5678:9abc:def0:1122:3344:5566:7788'

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet port %d" % self.dev_port11, " -> cpu")
            send_packet(self, self.dev_port11, pkt)
            self.port11_rif_counter_in += 1
            self.lag1_rif_counter_out += 1
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

        finally:
            sai_thrift_remove_hostif_trap(self.client, trap)
            sai_thrift_remove_hostif_trap_group(self.client, trap_group)

            sai_thrift_set_router_interface_attribute(
                self.client, self.port10_rif, mtu=mtu_port10_rif['mtu'])
            sai_thrift_set_router_interface_attribute(
                self.client, self.lag1_rif, mtu=mtu_lag1_rif['mtu'])

    def tearDown(self):
        super(Ipv6MtuTrapTest, self).tearDown()

@group("draft")
class SubPortIpv4MtuTrapTest(L3MtuTrapTestHelper):
    """
    Verifies if IPv4 packet is forwarded or punted to CPU as a trap,
    depending on the MTU
    """
    def setUp(self):
        super(SubPortIpv4MtuTrapTest, self).setUp()

    def runTest(self):
        print("\nsubPortIpv4MtuTrapTest()")

        # set MTU to 200 for subport 10_100
        sai_thrift_set_router_interface_attribute(
            self.client, self.subport10_100, mtu=200)

        try:
            trap_group = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True, queue=0)
            trap = sai_thrift_create_hostif_trap(
                self.client,
                trap_group=trap_group,
                trap_type=SAI_HOSTIF_TRAP_TYPE_L3_MTU_ERROR,
                packet_action=SAI_PACKET_ACTION_TRAP)

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='40.40.0.10',
                                    ip_id=105,
                                    ip_ttl=64,
                                    dl_vlan_enable=True,
                                    vlan_vid=200,
                                    pktlen=200 + 18)
            exp_pkt = simple_tcp_packet(eth_dst='00:33:33:33:01:00',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='40.40.0.10',
                                        ip_id=105,
                                        ip_ttl=63,
                                        dl_vlan_enable=True,
                                        vlan_vid=100,
                                        pktlen=200 + 18)
            print("Max MTU is 200, send pkt size 200, send to port")
            print("Sending packet port %d -> port %d"
                  % (self.dev_port11, self.dev_port10))
            send_packet(self, self.dev_port11, pkt)
            verify_packet(self, exp_pkt, self.dev_port10)
            self.subport11_200_in += 1
            self.subport10_100_out += 1

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='40.40.0.10',
                                    ip_id=105,
                                    ip_ttl=64,
                                    dl_vlan_enable=True,
                                    vlan_vid=200,
                                    pktlen=201 + 18)
            exp_pkt = simple_tcp_packet(eth_dst='00:33:33:33:01:00',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='40.40.0.10',
                                        ip_id=105,
                                        ip_ttl=63,
                                        dl_vlan_enable=True,
                                        vlan_vid=100,
                                        pktlen=201 + 18)
            print("Max MTU is 200, send pkt size 201, punt to cpu")
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet port %d" % self.dev_port11, " -> cpu")
            send_packet(self, self.dev_port11, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)
            self.subport11_200_in += 1
            self.subport10_100_out += 1

        finally:
            sai_thrift_remove_hostif_trap(self.client, trap)
            sai_thrift_remove_hostif_trap_group(self.client, trap_group)

    def tearDown(self):
        super(SubPortIpv4MtuTrapTest, self).tearDown()

@group("draft")
class SubPortIpv6MtuTrapTest(L3MtuTrapTestHelper):
    """
    Verifies if IPv6 packet is forwarded or punted to CPU as a trap,
    depending on the MTU
    """
    def setUp(self):
        super(SubPortIpv6MtuTrapTest, self).setUp()

    def runTest(self):
        print("\nsubPortIpv6MtuTrapTest()")

        # set MTU to 200 for subport 10_100
        sai_thrift_set_router_interface_attribute(
            self.client, self.subport10_100, mtu=200)

        try:
            trap_group = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True, queue=4)
            trap = sai_thrift_create_hostif_trap(
                self.client,
                trap_group=trap_group,
                trap_type=SAI_HOSTIF_TRAP_TYPE_L3_MTU_ERROR,
                packet_action=SAI_PACKET_ACTION_TRAP)

            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:8899',
                ipv6_src='2000::1',
                dl_vlan_enable=True,
                vlan_vid=200,
                ipv6_hlim=64,
                pktlen=200 + 40 + 18)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:33:33:33:01:00',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:8899',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                dl_vlan_enable=True,
                vlan_vid=100,
                pktlen=200 + 40 + 18)
            print("Max MTU is 200, send pkt size 200, send to port")
            print("Sending packet port %d -> port %d"
                  % (self.dev_port11, self.dev_port10))
            send_packet(self, self.dev_port11, pkt)
            verify_packet(self, exp_pkt, self.dev_port10)
            self.subport11_200_in += 1
            self.subport10_100_out += 1

            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:8899',
                ipv6_src='2000::1',
                dl_vlan_enable=True,
                vlan_vid=200,
                ipv6_hlim=64,
                pktlen=201 + 40 + 18)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:33:33:33:01:00',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:8899',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                dl_vlan_enable=True,
                vlan_vid=100,
                pktlen=201 + 40 + 18)
            print("Max MTU is 200, send pkt size 201, punt to cpu")
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet port %d" % self.dev_port11, " -> cpu")
            send_packet(self, self.dev_port11, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)
            self.subport11_200_in += 1
            self.subport10_100_out += 1

        finally:
            sai_thrift_remove_hostif_trap(self.client, trap)
            sai_thrift_remove_hostif_trap_group(self.client, trap_group)

    def tearDown(self):
        super(SubPortIpv6MtuTrapTest, self).tearDown()

@group("draft")
class SviIpv4MtuTrapTest(L3MtuTrapTestHelper):
    """
    Verifies if IPv4 packet is forwarded or punted to CPU as a trap,
    depending on the MTU
    """
    def setUp(self):
        super(SviIpv4MtuTrapTest, self).setUp()

    def runTest(self):
        print("\nsviIpv4MtuTrapTest()")

        # set MTU to 200 for vlan100_rif
        sai_thrift_set_router_interface_attribute(
            self.client, self.vlan100_rif, mtu=200)

        try:
            trap_group = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True, queue=4)
            trap = sai_thrift_create_hostif_trap(
                self.client,
                trap_group=trap_group,
                trap_type=SAI_HOSTIF_TRAP_TYPE_L3_MTU_ERROR,
                packet_action=SAI_PACKET_ACTION_TRAP)

            mac_action = SAI_PACKET_ACTION_FORWARD
            fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                               mac_address='00:11:22:33:44:55',
                                               bv_id=self.vlan100)
            sai_thrift_create_fdb_entry(self.client,
                                        fdb_entry,
                                        type=SAI_FDB_ENTRY_TYPE_STATIC,
                                        bridge_port_id=self.port24_bp,
                                        packet_action=mac_action)

            print("Max MTU is 200, send pkt size 200, send to port")
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.0.100',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=200 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.0.100',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=200 + 14)
            print("Sending packet port %d -> port %d"
                  % (self.dev_port10, self.dev_port24))
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port24)
            self.port10_rif_counter_in += 1
            self.vlan100_rif_counter_out += 1

            print("Max MTU is 200, send pkt size 201, send to CPU")
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.10.0.100',
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=201 + 14)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                        eth_src=ROUTER_MAC,
                                        ip_dst='10.10.0.100',
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=201 + 14)

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet port %d" % self.dev_port10, " -> cpu")
            send_packet(self, self.dev_port10, pkt)
            self.port10_rif_counter_in += 1
            self.vlan100_rif_counter_out += 1
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

        finally:
            sai_thrift_remove_hostif_trap(self.client, trap)
            sai_thrift_remove_hostif_trap_group(self.client, trap_group)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)

    def tearDown(self):
        super(SviIpv4MtuTrapTest, self).tearDown()

@group("draft")
class SviIpv6MtuTrapTest(L3MtuTrapTestHelper):
    """
    Verifies if IPv6 packet is forwarded or punted to CPU as a trap,
    depending on the MTU
    """
    def setUp(self):
        super(SviIpv6MtuTrapTest, self).setUp()

    def runTest(self):
        print("\nsviIpv6MtuTrapTest()")

        # set MTU to 200 for vlan100_rif
        sai_thrift_set_router_interface_attribute(
            self.client, self.vlan100_rif, mtu=200)

        try:
            trap_group = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True, queue=4)
            trap = sai_thrift_create_hostif_trap(
                self.client,
                trap_group=trap_group,
                trap_type=SAI_HOSTIF_TRAP_TYPE_L3_MTU_ERROR,
                packet_action=SAI_PACKET_ACTION_TRAP)

            mac_action = SAI_PACKET_ACTION_FORWARD
            fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                               mac_address='00:11:22:33:44:55',
                                               bv_id=self.vlan100)
            sai_thrift_create_fdb_entry(self.client,
                                        fdb_entry,
                                        type=SAI_FDB_ENTRY_TYPE_STATIC,
                                        bridge_port_id=self.port24_bp,
                                        packet_action=mac_action)

            print("Max MTU is 200, send pkt size 200, send to port")
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:0000',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=200 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:22:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:0000',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=200 + 14 + 40)
            print("Sending packet port %d -> port %d"
                  % (self.dev_port10, self.dev_port24))
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port24)
            self.port10_rif_counter_in += 1
            self.vlan100_rif_counter_out += 1

            print("Max MTU is 200, send pkt size 201, send to cpu")
            pkt = simple_tcpv6_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:0000',
                ipv6_src='2000::1',
                ipv6_hlim=64,
                pktlen=201 + 14 + 40)
            exp_pkt = simple_tcpv6_packet(
                eth_dst='00:11:22:33:44:55',
                eth_src=ROUTER_MAC,
                ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:0000',
                ipv6_src='2000::1',
                ipv6_hlim=63,
                pktlen=201 + 14 + 40)

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet port %d" % self.dev_port10, " -> cpu")
            send_packet(self, self.dev_port10, pkt)
            self.port10_rif_counter_in += 1
            self.vlan100_rif_counter_out += 1
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

        finally:
            sai_thrift_remove_hostif_trap(self.client, trap)
            sai_thrift_remove_hostif_trap_group(self.client, trap_group)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)

    def tearDown(self):
        super(SviIpv6MtuTrapTest, self).tearDown()

@group("draft")
class MtuPacketStatsTest(L3MtuTrapTestHelper):
    """
    Verifies Ingress and Egress RIF stats for unicast packets
    """
    def setUp(self):
        super(MtuPacketStatsTest, self).setUp()

    def runTest(self):
        print("\nmtuPacketStatsTest()")

        time.sleep(4)
        port10_rif_stats = sai_thrift_get_router_interface_stats(
            self.client, self.port10_rif)
        port11_rif_stats = sai_thrift_get_router_interface_stats(
            self.client, self.port11_rif)
        lag1_rif_stats = sai_thrift_get_router_interface_stats(
            self.client, self.lag1_rif)
        subport10_100_stats = sai_thrift_get_router_interface_stats(
            self.client, self.subport10_100)
        subport11_200_stats = sai_thrift_get_router_interface_stats(
            self.client, self.subport11_200)
        vlan100_rif_stats = sai_thrift_get_router_interface_stats(
            self.client, self.vlan100_rif)

        self.assertTrue(self.port10_rif_counter_in == port10_rif_stats[
            'SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'])
        self.assertTrue(self.port10_rif_counter_out == port10_rif_stats[
            'SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS'])
        self.assertTrue(self.port11_rif_counter_in == port11_rif_stats[
            'SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'])
        self.assertTrue(self.port11_rif_counter_out == port11_rif_stats[
            'SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS'])
        self.assertTrue(self.lag1_rif_counter_in == lag1_rif_stats[
            'SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'])
        self.assertTrue(self.lag1_rif_counter_out == lag1_rif_stats[
            'SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS'])
        self.assertTrue(self.subport10_100_in == subport10_100_stats[
            'SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'])
        self.assertTrue(self.subport10_100_out == subport10_100_stats[
            'SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS'])
        self.assertTrue(self.subport11_200_in == subport11_200_stats[
            'SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'])
        self.assertTrue(self.subport11_200_out == subport11_200_stats[
            'SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS'])
        self.assertTrue(self.vlan100_rif_counter_in == vlan100_rif_stats[
            'SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'])
        self.assertTrue(self.vlan100_rif_counter_out == vlan100_rif_stats[
            'SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS'])

    def tearDown(self):
        super(MtuPacketStatsTest, self).tearDown()

@group("draft")
class VirtualInterfaceTestHelper(PlatformSaiHelper):
    """
    This class contains virtual router interface tests
    """
    def setUp(self):
        super(VirtualInterfaceTestHelper, self).setUp()
        dmac1 = '00:11:22:33:44:55'

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.10.2'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('10.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=dmac1)

        self.route_entry0 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry0, next_hop_id=self.nhop1)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry0)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        super(VirtualInterfaceTestHelper, self).tearDown()

@group("draft")
class VirtualInterfaceType(VirtualInterfaceTestHelper):
    """
    Verifies if packet is forwarded correctly before
    and after MAC address is updated for virtual RIF.
    """
    def setUp(self):
        super(VirtualInterfaceType, self).setUp()

    def runTest(self):
        print("\nVirtualInterfaceType()")

        new_router_mac = "00:77:66:55:44:44"
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='10.10.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63)
        pkt1 = simple_tcp_packet(eth_dst=new_router_mac,
                                 eth_src='00:22:22:22:22:22',
                                 ip_dst='10.10.10.1',
                                 ip_src='192.168.0.1',
                                 ip_id=105,
                                 ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.1',
                                     ip_src='192.168.0.1',
                                     ip_id=105,
                                     ip_ttl=63)

        try:
            print("Sending packet on port %d with mac %s, forward"
                  % (self.dev_port11, ROUTER_MAC))
            send_packet(self, self.dev_port11, pkt)
            verify_packet(self, exp_pkt, self.dev_port10)

            print("Updating src_mac_address to %s" % (new_router_mac))

            port11_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                virtual_router_id=self.default_vrf,
                port_id=self.port11,
                is_virtual=True,
                src_mac_address=new_router_mac)

            port11_rif_attr = sai_thrift_get_router_interface_attribute(
                self.client,
                router_interface_oid=port11_rif,
                src_mac_address=True,
            )
            self.assertIsNotNone(port11_rif_attr)
            src_mac_address = port11_rif_attr.get('src_mac_address')
            self.assertEqual(src_mac_address, new_router_mac)

            print("Sending packet on port %d with mac %s, forwarded"
                  % (self.dev_port11, new_router_mac))
            send_packet(self, self.dev_port11, pkt1)
            verify_packet(self, exp_pkt1, self.dev_port10)
            self.assertTrue(sai_thrift_remove_router_interface(
                self.client, port11_rif) == 0)
        finally:
            pass

    def tearDown(self):
        super(VirtualInterfaceType, self).tearDown()

@group("draft")
class VirtualRIFType(VirtualInterfaceTestHelper):
    """
    Verifies the type of RIF created.
    """
    def setUp(self):
        super(VirtualRIFType, self).setUp()

    def runTest(self):
        print("\nVirtualRIFType()")
        try:
            new_router_mac = "00:77:66:55:44:44"
            v_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                virtual_router_id=self.default_vrf,
                port_id=self.port11,
                is_virtual=True,
                src_mac_address=new_router_mac)
            self.assertGreater(v_rif, 0)
            v_rif_attr = sai_thrift_get_router_interface_attribute(
                self.client,
                router_interface_oid=v_rif,
                is_virtual=True)
            is_virtual = None
            if v_rif_attr is not None:
                is_virtual = v_rif_attr.get('is_virtual')
            self.assertEqual(is_virtual, True)
            self.assertTrue(sai_thrift_remove_router_interface(
                self.client, v_rif) == 0)
        finally:
            pass

    def tearDown(self):
        super(VirtualRIFType, self).tearDown()

@group("draft")
class EnumQueryCapabilityHelper(PlatformSaiHelper):
    """
    This class contains tests that verify enum query
    capability for supported SAI RIF types.
    """

    def setUp(self):
        super(EnumQueryCapabilityHelper, self).setUp()

        self.enum_capab = sai_thrift_query_attribute_enum_values_capability(
            self.client,
            SAI_OBJECT_TYPE_ROUTER_INTERFACE,
            SAI_ROUTER_INTERFACE_ATTR_TYPE)

    def tearDown(self):
        super(EnumQueryCapabilityHelper, self).tearDown()

@group("draft")
class RifTypePort(EnumQueryCapabilityHelper):
    """
    Verifies enum query capability for RIF of type PORT.
    """
    def setUp(self):
        super(RifTypePort, self).setUp()

    def runTest(self):
        print("\nrifTypePort()")

        rif = self.port10_rif
        rif_attr = sai_thrift_get_router_interface_attribute(
            self.client,
            router_interface_oid=rif,
            type=True)
        rif_type = rif_attr.get('type')
        self.assertIn(rif_type, self.enum_capab)

    def tearDown(self):
        super(RifTypePort, self).tearDown()

@group("draft")
class RifTypeVlan(EnumQueryCapabilityHelper):
    """
    Verifies enum query capability for RIF of type VLAN.
    """
    def setUp(self):
        super(RifTypeVlan, self).setUp()

    def runTest(self):
        print("\nrifTypeVlan()")

        rif = self.vlan30_rif
        rif_attr = sai_thrift_get_router_interface_attribute(
            self.client,
            router_interface_oid=rif,
            type=True)
        rif_type = rif_attr.get('type')
        self.assertIn(rif_type, self.enum_capab)

    def tearDown(self):
        super(RifTypeVlan, self).tearDown()

@group("draft")
class RifTypeLoopback(EnumQueryCapabilityHelper):
    """
    Verifies enum query capability for RIF of type LOOPBACK.
    """
    def setUp(self):
        super(RifTypeLoopback, self).setUp()

    def runTest(self):
        print("\nrifTypeLoopback()")

        try:
            rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
                virtual_router_id=self.default_vrf)
            rif_attr = sai_thrift_get_router_interface_attribute(
                self.client,
                router_interface_oid=rif,
                type=True)
            rif_type = rif_attr.get('type')
            self.assertIn(rif_type, self.enum_capab)
        finally:
            sai_thrift_remove_router_interface(self.client, rif)

    def tearDown(self):
        super(RifTypeLoopback, self).tearDown()

@group("draft")
class RifTypeSubPort(EnumQueryCapabilityHelper):
    """
    Verifies enum query capability for RIF of type SUB_PORT.
    """
    def setUp(self):
        super(RifTypeSubPort, self).setUp()

    def runTest(self):
        print("\nrifTypeSubPort()")

        try:
            rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
                virtual_router_id=self.default_vrf,
                port_id=self.port24,
                outer_vlan_id=100)
            rif_attr = sai_thrift_get_router_interface_attribute(
                self.client,
                router_interface_oid=rif,
                type=True)
            rif_type = rif_attr.get('type')
            self.assertIn(rif_type, self.enum_capab)
        finally:
            sai_thrift_remove_router_interface(self.client, rif)

    def tearDown(self):
        super(RifTypeSubPort, self).tearDown()

@group("draft")
class SVIRifVlanTestHelper(PlatformSaiHelper):
    """
    This class contains virtual router interface tests for SVIs
    """
    def setUp(self):
        super(SVIRifVlanTestHelper, self).setUp()
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

        self.vlan100 = sai_thrift_create_vlan(self.client, vlan_id=100)
        self.vlan_member100 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_member101 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port25_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan_member102 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_port_attribute(
            self.client, self.port24, port_vlan_id=100)
        sai_thrift_set_port_attribute(
            self.client, self.port25, port_vlan_id=100)
        sai_thrift_set_port_attribute(
            self.client, self.port26, port_vlan_id=100)

        # create vlan100_rif
        self.vlan100_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan100)

        self.dmac1 = '00:11:22:33:44:55'  # 10.10.10.1
        self.dmac2 = '00:22:22:33:44:55'  # 10.10.10.2
        self.dmac3 = '00:33:22:33:44:55'  # 10.10.10.3
        self.dmac4 = '00:44:22:33:44:55'  # 11.11.11.1

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.1'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, ip_address=sai_ipaddress('10.10.0.1'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=self.dmac1)
        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1, next_hop_id=self.nhop1)

        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, ip_address=sai_ipaddress('10.10.0.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry2, dst_mac_address=self.dmac2)
        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.2'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.2/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry2, next_hop_id=self.nhop2)

        self.nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.3'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry3 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, ip_address=sai_ipaddress('10.10.0.3'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry3, dst_mac_address=self.dmac3)
        self.route_entry3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.3/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry3, next_hop_id=self.nhop3)

        # create nhop and route to L2 intf
        self.nhop4 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('11.11.0.2'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry4 = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('11.11.0.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry4, dst_mac_address=self.dmac4)
        self.route_entry4 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('11.11.11.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry4, next_hop_id=self.nhop4)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry4)
        sai_thrift_remove_route_entry(self.client, self.route_entry3)
        sai_thrift_remove_route_entry(self.client, self.route_entry2)
        sai_thrift_remove_route_entry(self.client, self.route_entry1)

        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry3)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry4)

        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_next_hop(self.client, self.nhop3)
        sai_thrift_remove_next_hop(self.client, self.nhop4)

        sai_thrift_remove_router_interface(self.client, self.vlan100_rif)
        sai_thrift_set_port_attribute(self.client, self.port24, port_vlan_id=0)
        sai_thrift_set_port_attribute(self.client, self.port25, port_vlan_id=0)
        sai_thrift_set_port_attribute(self.client, self.port26, port_vlan_id=0)

        sai_thrift_remove_vlan_member(self.client, self.vlan_member100)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member101)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member102)

        sai_thrift_remove_vlan(self.client, self.vlan100)
        sai_thrift_remove_bridge_port(self.client, self.port24_bp)
        sai_thrift_remove_bridge_port(self.client, self.port25_bp)
        sai_thrift_remove_bridge_port(self.client, self.port26_bp)

        super(SVIRifVlanTestHelper, self).tearDown()

@group("draft")
class SviRifIpv4Test(SVIRifVlanTestHelper):
    """
    Verifies if packet is forwarded correctly before
    and after MAC address is updated for virtual RIF for SVI's.
    """
    def setUp(self):
        super(SviRifIpv4Test, self).setUp()

    def runTest(self):
        print("\nsviRifIpv4Test()")
        new_router_mac = "00:77:66:55:44:44"
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='11.11.11.1',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64,
                                pktlen=100)
        exp_pkt = simple_tcp_packet(eth_dst='00:44:22:33:44:55',
                                    eth_src=ROUTER_MAC,
                                    ip_dst='11.11.11.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=63,
                                    pktlen=100)
        tagged_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                       eth_src='00:22:22:22:22:22',
                                       ip_dst='11.11.11.1',
                                       ip_src='192.168.0.1',
                                       ip_id=105,
                                       ip_ttl=64,
                                       dl_vlan_enable=True,
                                       vlan_vid=100,
                                       pktlen=104)
        pkt1 = simple_tcp_packet(eth_dst=new_router_mac,
                                 eth_src='00:22:22:22:22:22',
                                 ip_dst='11.11.11.1',
                                 ip_src='192.168.0.1',
                                 ip_id=105,
                                 ip_ttl=64)
        try:
            print("Sending packet on port %d, routed" % self.dev_port24)
            send_packet(self, self.dev_port24, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])

            print("Adding new virtual SVI rif with new ingress rif rmac")
            vlan100_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
                virtual_router_id=self.default_vrf,
                vlan_id=self.vlan100,
                is_virtual=True,
                src_mac_address=new_router_mac)

            print("Sending packet on port %d with mac %s, forwarded"
                  % (self.dev_port24, new_router_mac))
            send_packet(self, self.dev_port24, pkt1)
            verify_packet(self, exp_pkt, self.dev_port10)

            print("Sending tagged packet on port %d with mac %s, forwarded"
                  % (self.dev_port25, new_router_mac))
            send_packet(self, self.dev_port25, tagged_pkt)
            verify_packet(self, exp_pkt, self.dev_port10)

            print("Sending packet on port %d with mac %s, forwarded"
                  % (self.dev_port26, new_router_mac))
            send_packet(self, self.dev_port26, pkt1)
            verify_packet(self, exp_pkt, self.dev_port10)

            self.assertTrue(sai_thrift_remove_router_interface(
                self.client, vlan100_rif) == 0)
        finally:
            pass

    def tearDown(self):
        super(SviRifIpv4Test, self).tearDown()
