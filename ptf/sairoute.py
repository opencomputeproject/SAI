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
from sai_thrift.sai_headers import *

from ptf.testutils import *
from ptf.packet import *
from ptf.thriftutils import *

from sai_base_test import *

@group("draft")
class multipleRoutesTest(PlatformSaiHelper):
    '''
    Verify forwarding with multiple route to the same nhop.
    '''
    def setUp(self):
        super(multipleRoutesTest, self).setUp()

    def runTest(self):
        print("multipleRoutesTest")
        dmac = '00:11:22:33:44:55'

        neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('10.10.10.2'))
        sai_thrift_create_neighbor_entry(self.client, neighbor_entry,
                                         dst_mac_address=dmac)
        nhop1 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress('10.10.10.2'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(self.client, route_entry1,
                                      next_hop_id=nhop1)

        route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.2/32'))
        sai_thrift_create_route_entry(self.client, route_entry2,
                                      next_hop_id=nhop1)

        pkt1 = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)
        pkt2 = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.2',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=63)
        exp_pkt2 = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='10.10.10.2',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=63)
        try:
            print("Sending packet on port %d, forward" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt1)
            verify_packet(self, exp_pkt1, self.dev_port10)

            print("Sending packet on port %d, forward" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt2)
            verify_packet(self, exp_pkt2, self.dev_port10)

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry1)
            sai_thrift_remove_route_entry(self.client, route_entry2)
            sai_thrift_remove_next_hop(self.client, nhop1)
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry)

    def tearDown(self):
        super(multipleRoutesTest, self).tearDown()


@group("draft")
class dropRouteTest(PlatformSaiHelper):
    '''
    Verify drop route.
    '''
    def setUp(self):
        super(dropRouteTest, self).setUp()

    def runTest(self):
        print("dropRouteTest")
        dmac = '00:11:22:33:44:55'

        neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('10.10.10.2'))
        sai_thrift_create_neighbor_entry(self.client, neighbor_entry,
                                         dst_mac_address=dmac)
        nhop = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress('10.10.10.2'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, route_entry, next_hop_id=nhop,
            packet_action=SAI_PACKET_ACTION_TRAP)

        pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)
        try:
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet on port %d, forward" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop)
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry)

    def tearDown(self):
        super(dropRouteTest, self).tearDown()


@group("draft")
class routeUpdateTest(PlatformSaiHelper):
    '''
    Verify correct forwarding after route update.
    '''
    def setUp(self):
        super(routeUpdateTest, self).setUp()

    def runTest(self):
        print("routeUpdateTest")
        dmac = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:66'

        neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('10.10.10.2'))
        sai_thrift_create_neighbor_entry(self.client, neighbor_entry,
                                         dst_mac_address=dmac)
        nhop1 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress('10.10.10.2'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(self.client, route_entry,
                                      next_hop_id=nhop1)

        pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)
        exp_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=63)
        exp_pkt2 = simple_tcp_packet(
            eth_dst='00:11:22:33:44:66',
            eth_src=ROUTER_MAC,
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=63)
        try:
            print("Sending packet on port %d, forward" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            verify_packet(self, exp_pkt, self.dev_port10)

            print("Updating route nexthop to different nexthop")
            neighbor_entry2 = sai_thrift_neighbor_entry_t(
                rif_id=self.port10_rif, ip_address=sai_ipaddress('10.10.10.3'))
            sai_thrift_create_neighbor_entry(self.client, neighbor_entry2,
                                             dst_mac_address=dmac2)
            nhop2 = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress('10.10.10.3'),
                router_interface_id=self.port10_rif,
                type=SAI_NEXT_HOP_TYPE_IP)
            sai_thrift_set_route_entry_attribute(self.client, route_entry,
                                                 next_hop_id=nhop2)

            print("Sending packet on port %d, forward" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            verify_packet(self, exp_pkt2, self.dev_port10)

            print("Updating route nexthop to drop nexthop")
            sai_thrift_set_route_entry_attribute(
                self.client, route_entry, packet_action=SAI_PACKET_ACTION_DROP)

            print("Sending packet on port %d, drop" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            verify_no_other_packets(self, timeout=3)

            print("Updating route nexthop to regular nexthop")
            sai_thrift_set_route_entry_attribute(
                self.client, route_entry,
                packet_action=SAI_PACKET_ACTION_FORWARD)

            print("Sending packet on port %d, forward" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            verify_packet(self, exp_pkt2, self.dev_port10)

            print("Updating route nexthop to CPU nexthop")
            sai_thrift_set_route_entry_attribute(
                self.client, route_entry, packet_action=SAI_PACKET_ACTION_TRAP)

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet on port %d, forward" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

            print("Updating route nexthop to regular nexthop")
            sai_thrift_set_route_entry_attribute(
                self.client, route_entry,
                packet_action=SAI_PACKET_ACTION_FORWARD)

            print("Sending packet on port %d, forward" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            verify_packet(self, exp_pkt2, self.dev_port10)

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop1)
            sai_thrift_remove_next_hop(self.client, nhop2)
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry)
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry2)

    def tearDown(self):
        super(routeUpdateTest, self).tearDown()


@group("draft")
class routeIngressRifTest(PlatformSaiHelper):
    '''
    Verify forwarding to ingress rif.
    '''
    def setUp(self):
        super(routeIngressRifTest, self).setUp()

    def runTest(self):
        print("routeIngressRifTest")
        dmac = '00:11:22:33:44:55'

        neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('10.10.10.2'))
        sai_thrift_create_neighbor_entry(self.client, neighbor_entry,
                                         dst_mac_address=dmac)
        nhop1 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress('10.10.10.2'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(self.client, route_entry,
                                      next_hop_id=nhop1)

        pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)
        exp_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=63)
        try:
            print("Sending packet on port %d, forward" % self.dev_port10)
            send_packet(self, self.dev_port10, pkt)
            verify_packet(self, exp_pkt, self.dev_port10)

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop1)
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry)

    def tearDown(self):
        super(routeIngressRifTest, self).tearDown()


@group("draft")
class emptyECMPGroupTest(PlatformSaiHelper):
    '''
    Verify drop on empty ECMP group.
    '''
    def setUp(self):
        super(emptyECMPGroupTest, self).setUp()

    def runTest(self):
        print("emptyECMPGroupTest")

        nhop_group = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)

        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(self.client, route_entry,
                                      next_hop_id=nhop_group)

        pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)
        try:
            print("Sending packet on port %d, drop" % self.dev_port10)
            send_packet(self, self.dev_port10, pkt)
            verify_no_other_packets(self, timeout=3)

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop_group(self.client, nhop_group)

    def tearDown(self):
        super(emptyECMPGroupTest, self).tearDown()


@group("draft")
class sviNeighborTest(PlatformSaiHelper):
    '''
    Function verifying correct SVI neighbor forwarding.
    '''
    def setUp(self):
        super(sviNeighborTest, self).setUp()

    def runTest(self):
        print("sviNeighborTest")

        port24_bp = sai_thrift_create_bridge_port(
            self.client, bridge_id=self.default_1q_bridge, port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        port25_bp = sai_thrift_create_bridge_port(
            self.client, bridge_id=self.default_1q_bridge, port_id=self.port25,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        port26_bp = sai_thrift_create_bridge_port(
            self.client, bridge_id=self.default_1q_bridge, port_id=self.port26,
            type=SAI_BRIDGE_PORT_TYPE_PORT)

        # vlan100 with members port24, port25 and port26
        vlan100 = sai_thrift_create_vlan(self.client, vlan_id=100)
        vlan_member100 = sai_thrift_create_vlan_member(
            self.client, vlan_id=vlan100, bridge_port_id=port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member101 = sai_thrift_create_vlan_member(
            self.client, vlan_id=vlan100, bridge_port_id=port25_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member102 = sai_thrift_create_vlan_member(
            self.client, vlan_id=vlan100, bridge_port_id=port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_port_attribute(self.client, self.port24,
                                      port_vlan_id=100)
        sai_thrift_set_port_attribute(self.client, self.port25,
                                      port_vlan_id=100)
        sai_thrift_set_port_attribute(self.client, self.port26,
                                      port_vlan_id=100)

        # create vlan100_rif
        vlan100_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf, vlan_id=vlan100)

        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:22:22:33:44:55'
        dmac3 = '00:33:22:33:44:55'
        # create nhop1, nhop2 & nhop3 on SVI
        neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=vlan100_rif, ip_address=sai_ipaddress('10.10.0.1'))
        sai_thrift_create_neighbor_entry(self.client, neighbor_entry1,
                                         dst_mac_address=dmac1)
        nhop1 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress('10.10.0.1'),
            router_interface_id=vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(self.client, route_entry1,
                                      next_hop_id=nhop1)

        neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=vlan100_rif, ip_address=sai_ipaddress('10.10.0.2'))
        sai_thrift_create_neighbor_entry(self.client, neighbor_entry2,
                                         dst_mac_address=dmac2)
        nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.2'), router_interface_id=vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.2/32'))
        sai_thrift_create_route_entry(self.client, route_entry2,
                                      next_hop_id=nhop2)

        neighbor_entry3 = sai_thrift_neighbor_entry_t(
            rif_id=vlan100_rif, ip_address=sai_ipaddress('10.10.0.3'))
        sai_thrift_create_neighbor_entry(self.client, neighbor_entry3,
                                         dst_mac_address=dmac3)
        nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.3'), router_interface_id=vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        route_entry3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.3/32'))
        sai_thrift_create_route_entry(self.client, route_entry3,
                                      next_hop_id=nhop3)

        pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)
        exp_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=63)

        try:
            mac_action = SAI_PACKET_ACTION_FORWARD
            fdb_entry1 = sai_thrift_fdb_entry_t(
                switch_id=self.switch_id, mac_address=dmac1, bv_id=vlan100)
            sai_thrift_create_fdb_entry(
                self.client, fdb_entry1, type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=port24_bp, packet_action=mac_action)
            fdb_entry2 = sai_thrift_fdb_entry_t(
                switch_id=self.switch_id, mac_address=dmac2, bv_id=vlan100)
            sai_thrift_create_fdb_entry(
                self.client, fdb_entry2, type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=port25_bp, packet_action=mac_action)
            fdb_entry3 = sai_thrift_fdb_entry_t(
                switch_id=self.switch_id, mac_address=dmac3, bv_id=vlan100)
            sai_thrift_create_fdb_entry(
                self.client, fdb_entry3, type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=port26_bp, packet_action=mac_action)

            print("Sending packet port %d to port %d " %
                  (self.dev_port10, self.dev_port24) +
                  "(192.168.0.1 -> 10.10.10.1) Routed")
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt, [self.dev_port24])

            sai_thrift_remove_fdb_entry(self.client, fdb_entry3)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry2)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry1)

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry3)
            sai_thrift_remove_route_entry(self.client, route_entry2)
            sai_thrift_remove_route_entry(self.client, route_entry1)

            sai_thrift_remove_next_hop(self.client, nhop3)
            sai_thrift_remove_next_hop(self.client, nhop2)
            sai_thrift_remove_next_hop(self.client, nhop1)

            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry3)
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry2)
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry1)

            sai_thrift_remove_router_interface(self.client, vlan100_rif)
            sai_thrift_set_port_attribute(self.client, self.port24,
                                          port_vlan_id=0)
            sai_thrift_set_port_attribute(self.client, self.port25,
                                          port_vlan_id=0)
            sai_thrift_set_port_attribute(self.client, self.port26,
                                          port_vlan_id=0)
            sai_thrift_remove_vlan_member(self.client, vlan_member102)
            sai_thrift_remove_vlan_member(self.client, vlan_member101)
            sai_thrift_remove_vlan_member(self.client, vlan_member100)
            sai_thrift_remove_vlan(self.client, vlan100)

            sai_thrift_remove_bridge_port(self.client, port26_bp)
            sai_thrift_remove_bridge_port(self.client, port25_bp)
            sai_thrift_remove_bridge_port(self.client, port24_bp)

    def tearDown(self):
        super(sviNeighborTest, self).tearDown()


@group("draft")
class cpuForwardTest(PlatformSaiHelper):
    '''
    Function verifying forwading to CPU.
    '''
    def setUp(self):
        super(cpuForwardTest, self).setUp()

    def runTest(self):
        print("cpuForwardTest")

        cpu_port = sai_thrift_get_switch_attribute(self.client,
                                                   cpu_port=True)['cpu_port']

        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(self.client, route_entry,
                                      next_hop_id=cpu_port)

        pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)

        try:
            print("Sending packet on port %d, drop" % self.dev_port0)
            send_packet(self, self.dev_port0, pkt)
            verify_no_other_packets(self, timeout=3)

            print("Creating hostif trap for IP2ME")
            trap_group = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True, queue=4)
            trap = sai_thrift_create_hostif_trap(
                self.client,
                trap_group=trap_group, trap_type=SAI_HOSTIF_TRAP_TYPE_IP2ME,
                packet_action=SAI_PACKET_ACTION_TRAP)

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            print("Sending packet on port %d, forward to CPU" %
                  self.dev_port10)
            send_packet(self, self.dev_port10, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_hostif_trap(self.client, trap)
            sai_thrift_remove_hostif_trap_group(self.client, trap_group)

    def tearDown(self):
        super(cpuForwardTest, self).tearDown()


@group("draft")
class routeNbrColisionTest(PlatformSaiHelper):
    '''
    Verfies if packet is gleaned to CPU when nexthop id is RIF
    for cases with and without a neighbor
    '''
    def setUp(self):
        super(routeNbrColisionTest, self).setUp()

    def runTest(self):
        print("routeNbrColisionTest")

        ip_addr = '10.10.10.1'
        ip_addr_subnet = '10.10.10.1/32'
        dmac = '00:11:22:33:44:55'
        pkt_ip_src = '192.168.0.1'
        pkt_ip_dst = '10.10.10.1'

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst=pkt_ip_dst,
                                ip_src=pkt_ip_src,
                                ip_id=105,
                                ip_ttl=64)

        exp_pkt1 = simple_tcp_packet(eth_dst=dmac,
                                     eth_src=ROUTER_MAC,
                                     ip_dst=pkt_ip_dst,
                                     ip_src=pkt_ip_src,
                                     ip_id=105,
                                     ip_ttl=63)

        print("Creates neighbor with %s ip address, %d router interface id and"
              " %s destination mac" % (ip_addr, self.port10_rif, dmac))
        nbr_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress(ip_addr))
        sai_thrift_create_neighbor_entry(
            self.client, nbr_entry, dst_mac_address=dmac)

        print("Creates nhop with %s ip address and %d router interface id"
              % (ip_addr, self.port10_rif))
        nhop = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(ip_addr),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        try:
            print("Sending packet on port %d to port %d, forward from %s to %s"
                  % (self.dev_port11, self.dev_port10, pkt_ip_src, pkt_ip_dst))
            send_packet(self, self.dev_port11, pkt)
            verify_packets(self, exp_pkt1, [self.dev_port10])

            print("Creates route with %s ip prefix and %d router interface id"
                  % (ip_addr_subnet, self.port10_rif))
            route_entry = sai_thrift_route_entry_t(
                vr_id=self.default_vrf, destination=sai_ipprefix(
                    ip_addr_subnet))
            sai_thrift_create_route_entry(self.client, route_entry,
                                          next_hop_id=self.port10_rif)

            print("Sending packet on port %d to port %d, forward from %s to %s"
                  % (self.dev_port11, self.dev_port10, pkt_ip_src, pkt_ip_dst))
            send_packet(self, self.dev_port11, pkt)
            verify_packets(self, exp_pkt1, [self.dev_port10])

            print("Removes route")
            sai_thrift_remove_route_entry(self.client, route_entry)

            print("Sending packet on port %d to port %d, forward from %s to %s"
                  % (self.dev_port11, self.dev_port10, pkt_ip_src, pkt_ip_dst))
            send_packet(self, self.dev_port11, pkt)
            verify_packets(self, exp_pkt1, [self.dev_port10])

            print("Creates route with %s ip prefix and %d router interface id"
                  % (ip_addr_subnet, self.port10_rif))
            route_entry = sai_thrift_route_entry_t(
                vr_id=self.default_vrf, destination=sai_ipprefix(
                    ip_addr_subnet))
            sai_thrift_create_route_entry(self.client, route_entry,
                                          next_hop_id=self.port10_rif)

            print("Sending packet on port %d to port %d, forward from %s to %s"
                  % (self.dev_port11, self.dev_port10, pkt_ip_src, pkt_ip_dst))
            send_packet(self, self.dev_port11, pkt)
            verify_packets(self, exp_pkt1, [self.dev_port10])

            print("Removes neighbor")
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry)

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet on port %d, glean to cpu" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

            print("Creates neighbor with %s ip address, %d router interface id"
                  " and %s destination mac" % (ip_addr, self.port10_rif, dmac))
            nbr_entry = sai_thrift_neighbor_entry_t(
                rif_id=self.port10_rif, ip_address=sai_ipaddress(ip_addr))
            sai_thrift_create_neighbor_entry(
                self.client, nbr_entry, dst_mac_address=dmac)

            print("Sending packet on port %d to port %d, forward from %s to %s"
                  % (self.dev_port11, self.dev_port10, pkt_ip_src, pkt_ip_dst))
            send_packet(self, self.dev_port11, pkt)
            verify_packets(self, exp_pkt1, [self.dev_port10])

            print("Removes route")
            sai_thrift_remove_route_entry(self.client, route_entry)
            print("Removes neighbor")
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry)

            print("Sending packet on port %d, dropped" % (self.dev_port11))
            send_packet(self, self.dev_port11, pkt)
            verify_no_other_packets(self)

            print("Creates route with %s ip prefix and %d router interface id"
                  % (ip_addr_subnet, self.port10_rif))
            route_entry = sai_thrift_route_entry_t(
                vr_id=self.default_vrf, destination=sai_ipprefix(
                    ip_addr_subnet))
            sai_thrift_create_route_entry(self.client, route_entry,
                                          next_hop_id=self.port10_rif)

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet on port %d, glean to cpu" % self.dev_port11)
            send_packet(self, self.dev_port11, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

            print("Creates neighbor with %s ip address, %d router interface id"
                  " and %s destination mac" % (ip_addr, self.port10_rif, dmac))
            nbr_entry = sai_thrift_neighbor_entry_t(
                rif_id=self.port10_rif, ip_address=sai_ipaddress(ip_addr))
            sai_thrift_create_neighbor_entry(
                self.client, nbr_entry, dst_mac_address=dmac)

            print("Sending packet on port %d to port %d, forward from %s to %s"
                  % (self.dev_port11, self.dev_port10, pkt_ip_src, pkt_ip_dst))
            send_packet(self, self.dev_port11, pkt)
            verify_packets(self, exp_pkt1, [self.dev_port10])

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry)

    def tearDown(self):
        super(routeNbrColisionTest, self).tearDown()


@group("draft")
class L3DirBcastRouteTestHelper(PlatformSaiHelper):
    '''
    Verifies direct broadcast routing
    '''
    def setUp(self):
        super(L3DirBcastRouteTestHelper, self).setUp()

        vlan100_id = 100

        self.ip_addr1 = '10.10.10.1'
        self.ip_addr1_subnet = '10.10.10.0/24'
        self.dmac1 = '00:11:22:33:44:55'
        self.dir_bcast_ip_addr1 = '10.10.10.255'
        self.dir_bcast_dmac1 = 'ff:ff:ff:ff:ff:ff'
        self.ip_addr2 = '20.20.20.1'
        self.ip_addr2_subnet = '20.20.20.0/24'
        self.dmac2 = '22:11:22:33:44:55'
        self.mac_action = SAI_PACKET_ACTION_FORWARD

        self.port24_bp = sai_thrift_create_bridge_port(
            self.client, bridge_id=self.default_1q_bridge, port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.port25_bp = sai_thrift_create_bridge_port(
            self.client, bridge_id=self.default_1q_bridge, port_id=self.port25,
            type=SAI_BRIDGE_PORT_TYPE_PORT)

        self.vlan100 = sai_thrift_create_vlan(self.client, vlan_id=vlan100_id)

        self.vlan100_member1 = sai_thrift_create_vlan_member(
            self.client, vlan_id=self.vlan100, bridge_port_id=self.port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        self.vlan100_member2 = sai_thrift_create_vlan_member(
            self.client, vlan_id=self.vlan100, bridge_port_id=self.port25_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_port_attribute(self.client, self.port24,
                                      port_vlan_id=vlan100_id)
        sai_thrift_set_port_attribute(self.client, self.port25,
                                      port_vlan_id=vlan100_id)

        self.fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                mac_address=self.dmac1,
                                                bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(self.client,
                                    self.fdb_entry,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.port24_bp,
                                    packet_action=self.mac_action)

        self.vlan100_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan100)

    def tearDown(self):
        print("Removes router interface %d" % self.vlan100_rif)
        sai_thrift_remove_router_interface(self.client, self.vlan100_rif)

        print("Removes fdb")
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry)

        print("Sets port attributes")
        sai_thrift_set_port_attribute(self.client, self.port24,
                                      port_vlan_id=0)
        sai_thrift_set_port_attribute(self.client, self.port25,
                                      port_vlan_id=0)

        print("Removes vlan member 1 %d" % self.vlan100_member1)
        sai_thrift_remove_vlan_member(self.client, self.vlan100_member1)
        print("Removes vlan member 2 %d" % self.vlan100_member2)
        sai_thrift_remove_vlan_member(self.client, self.vlan100_member2)

        print("Removes vlan %d" % self.vlan100)
        sai_thrift_remove_vlan(self.client, self.vlan100)

        print("Removes bridge port %d" % self.port24_bp)
        sai_thrift_remove_bridge_port(self.client, self.port24_bp)
        print("Removes bridge port %d" % self.port25_bp)
        sai_thrift_remove_bridge_port(self.client, self.port25_bp)

        super(L3DirBcastRouteTestHelper, self).tearDown()

    def trafficTrapTest1(self):
        """
        Verifies the test packets are gleaned to CPU when neighbors don't exist
        """
        try:
            pkt_ip_src = '192.168.0.1'
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:21',
                                    ip_dst=self.ip_addr1,
                                    ip_src=pkt_ip_src,
                                    ip_id=105,
                                    ip_ttl=64)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet on port %d, glean to cpu" % self.dev_port10)
            send_packet(self, self.dev_port10, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst=self.ip_addr2,
                                    ip_src=pkt_ip_src,
                                    ip_id=105,
                                    ip_ttl=64)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet on port %d, glean to cpu" % self.dev_port24)
            send_packet(self, self.dev_port24, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

        finally:
            pass

    def trafficTrapTest2(self):
        """
        Verifies the test packets are gleaned to CPU when neighbors don't exist
        """
        try:
            pkt_ip_src = '192.168.0.1'
            pkt_ip_dst = '10.10.10.2'
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:21',
                                    ip_dst=pkt_ip_dst,
                                    ip_src=pkt_ip_src,
                                    ip_id=105,
                                    ip_ttl=64)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet on port %d, glean to cpu" % self.dev_port10)
            send_packet(self, self.dev_port10, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

            pkt_ip_dst = '20.20.20.2'
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst=pkt_ip_dst,
                                    ip_src=pkt_ip_src,
                                    ip_id=105,
                                    ip_ttl=64)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet on port %d, glean to cpu" % self.dev_port24)
            send_packet(self, self.dev_port24, pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

        finally:
            pass

    def trafficTest(self):
        """
        Verfies if test packets are properly forwarded
        """
        try:
            pkt_ip_src = '192.168.0.1'
            # send the test packet(s)
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:21',
                                    ip_dst=self.ip_addr1,
                                    ip_src=pkt_ip_src,
                                    ip_id=105,
                                    ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_dst=self.dmac1,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=self.ip_addr1,
                                        ip_src=pkt_ip_src,
                                        ip_id=105,
                                        ip_ttl=63)
            print("Sending packet on port %d to port %d, forward from %s to %s"
                  % (self.dev_port10, self.dev_port24, pkt_ip_src,
                     self.ip_addr1))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt, [self.dev_port24])

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst=self.dir_bcast_ip_addr1,
                                    ip_src=pkt_ip_src,
                                    ip_id=105,
                                    ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_dst=self.dir_bcast_dmac1,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=self.dir_bcast_ip_addr1,
                                        ip_src=pkt_ip_src,
                                        ip_id=105,
                                        ip_ttl=63)
            print("Sending packet on port %d to port %d and %d, forward from "
                  "%s to %s" % (self.dev_port10, self.dev_port24,
                                self.dev_port25, pkt_ip_src,
                                self.dir_bcast_ip_addr1))
            send_packet(self, self.dev_port10, pkt)
            verify_packets(self, exp_pkt, ports=[self.dev_port24,
                                                 self.dev_port25])

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:23',
                                    ip_dst=self.ip_addr2,
                                    ip_src=pkt_ip_src,
                                    ip_id=105,
                                    ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_dst=self.dmac2,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=self.ip_addr2,
                                        ip_src=pkt_ip_src,
                                        ip_id=105,
                                        ip_ttl=63)
            print("Sending packet on port %d to port %d, forward from %s to %s"
                  % (self.dev_port25, self.dev_port10, pkt_ip_src,
                     self.ip_addr2))
            send_packet(self, self.dev_port25, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])

        finally:
            pass

@group("draft")
class DirBcastGleanAndForwardTest(L3DirBcastRouteTestHelper):
    """
    Verifies if frame is frowarded to cpu when there is only route without
    neighbor then if frame is forwarded properly after creating neighbor
    and nhop
    """
    def setUp(self):
        super(DirBcastGleanAndForwardTest, self).setUp()

    def runTest(self):
        nhop1 = 0
        nhop2 = 0

        try:
            # create subnet routes
            print("Creates route 1 with %s ip prefix and %d router"
                  " interface id" % (self.ip_addr1_subnet, self.vlan100_rif))
            route_entry1 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf, destination=sai_ipprefix(
                    self.ip_addr1_subnet))
            sai_thrift_create_route_entry(self.client, route_entry1,
                                          next_hop_id=self.vlan100_rif)

            print("Creates route 2 with %s ip prefix and %d router"
                  " interface id" % (self.ip_addr2_subnet, self.port10_rif))
            route_entry2 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(self.ip_addr2_subnet))
            sai_thrift_create_route_entry(self.client, route_entry2,
                                          next_hop_id=self.port10_rif)
            self.trafficTrapTest1()
            self.trafficTrapTest2()

            # create directed broadcast neighbor on SVI rif
            print("Creates directed broadcast neighbor with %s ip address, "
                  "%d router interface id and %s destination mac"
                  % (self.dir_bcast_ip_addr1, self.vlan100_rif,
                     self.dir_bcast_dmac1))
            nbr_entry0 = sai_thrift_neighbor_entry_t(
                rif_id=self.vlan100_rif, ip_address=sai_ipaddress(
                    self.dir_bcast_ip_addr1))
            sai_thrift_create_neighbor_entry(
                self.client, nbr_entry0, dst_mac_address=self.dir_bcast_dmac1)

            # simulate arp entry add of rif1 and 2
            print("Creates neighbor 1 with %s ip address, %d router interface"
                  " id and %s destination mac" % (
                      self.ip_addr1, self.vlan100_rif, self.dmac1))
            nbr_entry1 = sai_thrift_neighbor_entry_t(
                rif_id=self.vlan100_rif,
                ip_address=sai_ipaddress(self.ip_addr1))
            sai_thrift_create_neighbor_entry(
                self.client, nbr_entry1, dst_mac_address=self.dmac1)

            print("Creates nhop 1 with %s ip address and %d router"
                  " interface id" % (self.ip_addr1, self.vlan100_rif))
            nhop1 = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress(self.ip_addr1),
                router_interface_id=self.vlan100_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            print("Creates neighbor 2 with %s ip address, %d router interface"
                  " id and %s destination mac" % (
                      self.ip_addr2, self.port10_rif, self.dmac2))
            nbr_entry2 = sai_thrift_neighbor_entry_t(
                rif_id=self.port10_rif,
                ip_address=sai_ipaddress(self.ip_addr2))
            sai_thrift_create_neighbor_entry(
                self.client, nbr_entry2, dst_mac_address=self.dmac2)

            print("Creates nhop 2 with %s ip address and %d router "
                  "interface id" % (self.ip_addr2, self.port10_rif))
            nhop2 = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress(self.ip_addr2),
                router_interface_id=self.port10_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            self.trafficTest()
            self.trafficTrapTest2()

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry1)
            sai_thrift_remove_route_entry(self.client, route_entry2)

            if nhop1:
                sai_thrift_remove_next_hop(self.client, nhop1)
            if nhop2:
                sai_thrift_remove_next_hop(self.client, nhop2)
            if 'nbr_entry1' in locals():
                sai_thrift_remove_neighbor_entry(self.client, nbr_entry1)
            if 'nbr_entry2' in locals():
                sai_thrift_remove_neighbor_entry(self.client, nbr_entry2)
            if 'nbr_entry0' in locals():
                sai_thrift_remove_neighbor_entry(self.client, nbr_entry0)

    def tearDown(self):
        super(DirBcastGleanAndForwardTest, self).tearDown()


@group("draft")
class DirBcastForwardTest(L3DirBcastRouteTestHelper):
    """
    Verifies if frame is forwarded properly when configuration is correct
    """

    def setUp(self):
        super(DirBcastForwardTest, self).setUp()

    def runTest(self):
        nhop1 = 0
        nhop2 = 0

        try:
            # simulate arp entry add
            print("Creates neighbor 1 with %s ip address, %d router interface"
                  "id and %s destination mac" % (
                      self.ip_addr1, self.vlan100_rif, self.dmac1))
            nbr_entry1 = sai_thrift_neighbor_entry_t(
                rif_id=self.vlan100_rif,
                ip_address=sai_ipaddress(self.ip_addr1))
            sai_thrift_create_neighbor_entry(
                self.client, nbr_entry1, dst_mac_address=self.dmac1)

            print("Creates nhop 1 with %s ip address and %d router"
                  "interface id" % (self.ip_addr1, self.vlan100_rif))
            nhop1 = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress(self.ip_addr1),
                router_interface_id=self.vlan100_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            print("Creates neighbor 2 with %s ip address, %d router interface"
                  "id and %s destination mac" % (
                      self.ip_addr2, self.port10_rif, self.dmac2))
            nbr_entry2 = sai_thrift_neighbor_entry_t(
                rif_id=self.port10_rif,
                ip_address=sai_ipaddress(self.ip_addr2))
            sai_thrift_create_neighbor_entry(
                self.client, nbr_entry2, dst_mac_address=self.dmac2)

            print("Creates nhop 2 with %s ip address and %d router"
                  "interface id" % (self.ip_addr2, self.port10_rif))
            nhop2 = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress(self.ip_addr2),
                router_interface_id=self.port10_rif,
                type=SAI_NEXT_HOP_TYPE_IP)

            # create directed broadcast neighbor on SVI rif
            print("Creates directed broadcast neighbor with %s ip address,"
                  "%d router interface id and %s destination mac"
                  % (self.dir_bcast_ip_addr1, self.vlan100_rif,
                     self.dir_bcast_dmac1))
            nbr_entry0 = sai_thrift_neighbor_entry_t(
                rif_id=self.vlan100_rif,
                ip_address=sai_ipaddress(self.dir_bcast_ip_addr1))
            sai_thrift_create_neighbor_entry(
                self.client, nbr_entry0, dst_mac_address=self.dir_bcast_dmac1)

            # create subnet routes
            print("Creates route 1 with %s ipprefix and %d router interface id"
                  % (self.ip_addr1_subnet, self.vlan100_rif))
            route_entry1 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(self.ip_addr1_subnet))
            sai_thrift_create_route_entry(self.client, route_entry1,
                                          next_hop_id=self.vlan100_rif)

            print("Creates route 2 with %s ipprefix and %d router interface id"
                  % (self.ip_addr2_subnet, self.port10_rif))
            route_entry2 = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(self.ip_addr2_subnet))
            sai_thrift_create_route_entry(self.client, route_entry2,
                                          next_hop_id=self.port10_rif)

            self.trafficTest()
            self.trafficTrapTest2()

        finally:
            sai_thrift_remove_route_entry(self.client, route_entry1)
            sai_thrift_remove_route_entry(self.client, route_entry2)

            if nhop1:
                sai_thrift_remove_next_hop(self.client, nhop1)
            if nhop2:
                sai_thrift_remove_next_hop(self.client, nhop2)

            sai_thrift_remove_neighbor_entry(self.client, nbr_entry1)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry2)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry0)

    def tearDown(self):
        super(DirBcastForwardTest, self).tearDown()
