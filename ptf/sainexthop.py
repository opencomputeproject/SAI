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

from ptf.mask import *
from ptf.packet import *
from ptf.testutils import *
from ptf.thriftutils import *

from sai_base_test import *


class L3NexthopTestHelper(PlatformSaiHelper):
    """
    Basic L3 nexthop tests.
    Configuration
    +----------+-----------+
    | port0    | port0_rif |
    +----------+-----------+
    | port1    | port1_rif |
    +----------+-----------+
    """
    def setUp(self):
        super(L3NexthopTestHelper, self).setUp()

        self.create_routing_interfaces(ports=[0, 1])

    def tearDown(self):
        self.destroy_routing_interfaces()

        super(L3NexthopTestHelper, self).tearDown()


class removeNexthopTest(L3NexthopTestHelper):
    """
        Test verifies correct nexthop removal.
    """
    def setUp(self):
        super(removeNexthopTest, self).setUp()

    def runTest(self):
        print("RemoveNexthopTest")
        neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.port0_rif, ip_address=sai_ipaddress('10.10.10.10'))
        sai_thrift_create_neighbor_entry(self.client, neighbor_entry,
                                         dst_mac_address='00:99:99:99:99:99')
        nhop = sai_thrift_create_next_hop(self.client,
                                          ip=sai_ipaddress('10.10.10.10'),
                                          router_interface_id=self.port0_rif,
                                          type=SAI_NEXT_HOP_TYPE_IP)

        route1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.2/32'))
        sai_thrift_create_route_entry(self.client, route1, next_hop_id=nhop)
        route2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.3/32'))
        sai_thrift_create_route_entry(self.client, route2, next_hop_id=nhop)
        route3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.4/32'))
        sai_thrift_create_route_entry(self.client, route3, next_hop_id=nhop)

        try:
            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ip_dst='10.10.10.2',
                ip_src='192.168.0.1',
                ip_id=105,
                ip_ttl=64)
            exp_pkt = simple_tcp_packet(
                eth_dst='00:99:99:99:99:99',
                eth_src=ROUTER_MAC,
                ip_dst='10.10.10.2',
                ip_src='192.168.0.1',
                ip_id=105,
                ip_ttl=63)
            print("Sending packet on port %d, forward" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            verify_packet(self, exp_pkt, self.dev_port0)

            sai_thrift_remove_next_hop(self.client, nhop)
            pre_stats = query_counter(
                    self, sai_thrift_get_queue_stats, self.cpu_queue0)
            print("Sending packet on port %d, forward" % self.dev_port1)
            send_packet(self, self.dev_port1, pkt)
            time.sleep(4)
            post_stats = query_counter(
                    self, sai_thrift_get_queue_stats, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)

        finally:
            sai_thrift_remove_route_entry(self.client, route3)
            sai_thrift_remove_route_entry(self.client, route2)
            sai_thrift_remove_route_entry(self.client, route1)
            sai_thrift_remove_next_hop(self.client, nhop)
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry)

    def tearDown(self):
        super(removeNexthopTest, self).tearDown()


class cpuNexthopTest(L3NexthopTestHelper):
    '''
        Test verifies nexthop to CPU.
    '''
    def setUp(self):
        super(cpuNexthopTest, self).setUp()

    def runTest(self):
        print("cpuNexthopTest")

        print("Creating hostif trap for IP2ME")
        trap_group = sai_thrift_create_hostif_trap_group(
            self.client, admin_state=True, queue=4)
        trap = sai_thrift_create_hostif_trap(
            self.client, trap_group=trap_group,
            trap_type=SAI_HOSTIF_TRAP_TYPE_IP2ME,
            packet_action=SAI_PACKET_ACTION_TRAP,
            trap_priority=1)

        attr = sai_thrift_get_switch_attribute(self.client, cpu_port=True)
        cpu_port = attr['cpu_port']

        host_ipv4 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(self.client, host_ipv4,
                                      next_hop_id=cpu_port)

        lpm_ipv4 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('20.20.20.0/24'))
        sai_thrift_create_route_entry(self.client, lpm_ipv4,
                                      next_hop_id=cpu_port)

        ipv6 = '1234:5678:9abc:def0:1122:3344:5566:7788/128'
        host_ipv6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(ipv6))
        sai_thrift_create_route_entry(self.client, host_ipv6,
                                      next_hop_id=cpu_port)

        lpm_ipv6 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('4000::0/65'))
        sai_thrift_create_route_entry(self.client, lpm_ipv6,
                                      next_hop_id=cpu_port)

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
            ip_dst='20.20.20.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)

        pkt3 = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='1234:5678:9abc:def0:1122:3344:5566:7788',
            ipv6_src='2000::1',
            ipv6_hlim=64)

        pkt4 = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ipv6_dst='4000::1',
            ipv6_src='2000::1',
            ipv6_hlim=64)

        try:
            pre_stats = query_counter(
                    self, sai_thrift_get_queue_stats, self.cpu_queue4)

            print("Sending packet on port %d, forward to CPU, host ipv4" %
                  self.dev_port0)
            send_packet(self, self.dev_port0, pkt1)

            print("Sending packet on port %d, forward to CPU, lpm ipv4" %
                  self.dev_port0)
            send_packet(self, self.dev_port0, pkt2)

            print("Sending packet on port %d, forward to CPU, host ipv6" %
                  self.dev_port0)
            send_packet(self, self.dev_port0, pkt3)

            print("Sending packet on port %d, forward to CPU, lpm ipv6" %
                  self.dev_port0)
            send_packet(self, self.dev_port0, pkt4)

            time.sleep(4)
            post_stats = query_counter(
                    self, sai_thrift_get_queue_stats, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                4)
        finally:
            sai_thrift_remove_route_entry(self.client, host_ipv6)
            sai_thrift_remove_route_entry(self.client, lpm_ipv6)
            sai_thrift_remove_route_entry(self.client, host_ipv4)
            sai_thrift_remove_route_entry(self.client, lpm_ipv4)
            sai_thrift_remove_hostif_trap(self.client, trap)
            sai_thrift_remove_hostif_trap_group(self.client, trap_group)

    def tearDown(self):
        super(cpuNexthopTest, self).tearDown()


class NhopTunnelEncapDecapTestHelper(PlatformSaiHelper):
    '''
        Nexthop tunnel encapsulation and decapsulation tests.
    '''
    def setUp(self):
        super(NhopTunnelEncapDecapTestHelper, self).setUp()
        # underlay config
        self.uvrf = sai_thrift_create_virtual_router(self.client)

        self.vm_ip4 = '100.100.1.1'
        self.vm_ip6 = '1234:5678:9abc:def0:4422:1133:5577:9951'
        self.customer_ip4 = '100.100.0.1'
        self.customer_ip6 = '1234:5678:9abc:def0:4422:1133:5577:0011'
        self.my_lb_ip = '10.10.10.10'
        self.tunnel_ip = '10.10.10.1'
        self.tunnel_ip_mask = '10.10.10.1/32'
        self.inner_dmac = '00:33:33:33:33:33'
        self.underlay_neighbor_mac = '00:11:11:11:11:11'
        self.default_rmac = '00:BA:7E:F0:00:00'

        # overlay config
        self.ovrf = sai_thrift_create_virtual_router(self.client)
        self.tunnel_type = SAI_TUNNEL_TYPE_IPINIP
        self.term_type = SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P
        self.ttl_mode = SAI_TUNNEL_TTL_MODE_PIPE_MODEL

        self.urif_lb = sai_thrift_create_router_interface(
            self.client, type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.uvrf)

        # create underlay loopback rif for tunnel
        self.orif_lb = sai_thrift_create_router_interface(
            self.client, type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.ovrf)

        # create route to tunnel ip 10.10.10.1
        self.urif = sai_thrift_create_router_interface(
            self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.uvrf, port_id=self.port25)
        self.uneighbor = sai_thrift_neighbor_entry_t(
            rif_id=self.urif, ip_address=sai_ipaddress(self.tunnel_ip))
        sai_thrift_create_neighbor_entry(
            self.client, self.uneighbor,
            dst_mac_address=self.underlay_neighbor_mac)
        self.unhop = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(self.tunnel_ip),
            router_interface_id=self.urif, type=SAI_NEXT_HOP_TYPE_IP)
        self.tunnel_entry1 = sai_thrift_route_entry_t(
            vr_id=self.uvrf, destination=sai_ipprefix(self.tunnel_ip_mask))
        sai_thrift_create_route_entry(self.client, self.tunnel_entry1,
                                      next_hop_id=self.unhop)

        # create tunnel
        self.tunnel = sai_thrift_create_tunnel(
            self.client, type=self.tunnel_type,
            encap_src_ip=sai_ipaddress(self.my_lb_ip),
            encap_ttl_mode=self.ttl_mode,
            decap_ttl_mode=self.ttl_mode,
            underlay_interface=self.urif_lb, overlay_interface=self.orif_lb)

        # create tunnel termination entry
        self.tunnel_term = sai_thrift_create_tunnel_term_table_entry(
            self.client, tunnel_type=self.tunnel_type, type=self.term_type,
            vr_id=self.uvrf, action_tunnel_id=self.tunnel,
            src_ip=sai_ipaddress(self.tunnel_ip),
            dst_ip=sai_ipaddress(self.my_lb_ip))

        # create tunnel nhop for VM 100.100.1.1
        self.tunnel_nexthop = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP,
            tunnel_id=self.tunnel, tunnel_mac=self.inner_dmac,
            ip=sai_ipaddress(self.tunnel_ip))

        # add route to tunnel_nexthop
        self.customer_route1 = sai_thrift_route_entry_t(
            vr_id=self.ovrf, destination=sai_ipprefix(self.vm_ip4+'/32'))
        sai_thrift_create_route_entry(self.client, self.customer_route1,
                                      next_hop_id=self.tunnel_nexthop)
        self.customer_route2 = sai_thrift_route_entry_t(
            vr_id=self.ovrf, destination=sai_ipprefix(self.vm_ip6+'/128'))
        sai_thrift_create_route_entry(self.client, self.customer_route2,
                                      next_hop_id=self.tunnel_nexthop)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.customer_route2)
        sai_thrift_remove_route_entry(self.client, self.customer_route1)

        sai_thrift_remove_next_hop(self.client, self.tunnel_nexthop)

        sai_thrift_remove_tunnel_term_table_entry(self.client,
                                                  self.tunnel_term)
        sai_thrift_remove_tunnel(self.client, self.tunnel)

        sai_thrift_remove_route_entry(self.client, self.tunnel_entry1)
        sai_thrift_remove_next_hop(self.client, self.unhop)
        sai_thrift_remove_neighbor_entry(self.client, self.uneighbor)
        sai_thrift_remove_router_interface(self.client, self.urif)
        sai_thrift_remove_router_interface(self.client, self.orif_lb)
        sai_thrift_remove_router_interface(self.client, self.urif_lb)

        sai_thrift_remove_virtual_router(self.client, self.ovrf)
        sai_thrift_remove_virtual_router(self.client, self.uvrf)

        super(NhopTunnelEncapDecapTestHelper, self).tearDown()


class l3InterfaceTunnelTest(NhopTunnelEncapDecapTestHelper):
    '''
        Test verifies l3 interface tunnel nexthop.
    '''
    def setUp(self):
        super(l3InterfaceTunnelTest, self).setUp()

    def runTest(self):
        print("l3InterfaceTunnelTest")
        try:
            orif = sai_thrift_create_router_interface(
                self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                port_id=self.port24, virtual_router_id=self.ovrf)
            oneighbor4 = sai_thrift_neighbor_entry_t(
                rif_id=orif, ip_address=sai_ipaddress(self.customer_ip4))
            sai_thrift_create_neighbor_entry(
                self.client, oneighbor4, dst_mac_address='00:22:22:22:22:33')
            onhop4 = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress(self.customer_ip4),
                router_interface_id=orif, type=SAI_NEXT_HOP_TYPE_IP)
            customer_route3 = sai_thrift_route_entry_t(
                vr_id=self.ovrf,
                destination=sai_ipprefix(self.customer_ip4+'/32'))
            sai_thrift_create_route_entry(self.client, customer_route3,
                                          next_hop_id=onhop4)

            print("Verifying IP 4in4 (encap)")
            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:33',
                ip_dst=self.vm_ip4,
                ip_src=self.customer_ip4,
                ip_id=108,
                ip_ttl=64)
            pkt2 = simple_tcp_packet(
                eth_dst=self.inner_dmac,
                eth_src=self.default_rmac,
                ip_dst=self.vm_ip4,
                ip_src=self.customer_ip4,
                ip_id=108,
                ip_ttl=63)
            ipip_pkt = simple_ipv4ip_packet(
                eth_dst=self.underlay_neighbor_mac,
                eth_src=ROUTER_MAC,
                ip_id=0,
                ip_src='10.10.10.10',
                ip_dst='10.10.10.1',
                ip_ttl=64,
                inner_frame=pkt2['IP'])
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, ipip_pkt, self.dev_port25)

            print("Verifying IP 4in4 (decap)")
            pkt1 = simple_tcp_packet(
                eth_src=ROUTER_MAC,
                eth_dst=self.inner_dmac,
                ip_dst=self.customer_ip4,
                ip_src=self.vm_ip4,
                ip_id=108,
                ip_ttl=64)
            pkt2 = simple_tcp_packet(
                eth_src=ROUTER_MAC,
                eth_dst='00:22:22:22:22:33',
                ip_dst=self.customer_ip4,
                ip_src=self.vm_ip4,
                ip_id=108,
                ip_ttl=63)
            ipip_pkt = simple_ipv4ip_packet(
                eth_dst=ROUTER_MAC,
                eth_src=self.underlay_neighbor_mac,
                ip_id=0,
                ip_src='10.10.10.1',
                ip_dst='10.10.10.10',
                ip_ttl=64,
                inner_frame=pkt1['IP'])
            send_packet(self, self.dev_port25, ipip_pkt)
            verify_packet(self, pkt2, self.dev_port24)

        finally:
            sai_thrift_remove_route_entry(self.client, customer_route3)
            sai_thrift_remove_next_hop(self.client, onhop4)
            sai_thrift_remove_neighbor_entry(self.client, oneighbor4)
            sai_thrift_remove_router_interface(self.client, orif)

    def tearDown(self):
        super(l3InterfaceTunnelTest, self).tearDown()


class subPortTunnelTest(NhopTunnelEncapDecapTestHelper):
    '''
        Test verifies subport tunnel nexthop.
    '''
    def setUp(self):
        super(subPortTunnelTest, self).setUp()

    def runTest(self):
        print("subPortTunnelTest")
        try:
            osubport = sai_thrift_create_router_interface(
                self.client, type=SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,
                virtual_router_id=self.ovrf, port_id=self.port24,
                admin_v4_state=True, outer_vlan_id=500)
            oneighbor = sai_thrift_neighbor_entry_t(
                rif_id=osubport, ip_address=sai_ipaddress(self.customer_ip4))
            sai_thrift_create_neighbor_entry(
                self.client, oneighbor, dst_mac_address="00:22:22:22:22:33")
            onhop = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress(self.customer_ip4),
                router_interface_id=osubport, type=SAI_NEXT_HOP_TYPE_IP)
            customer_route3 = sai_thrift_route_entry_t(
                vr_id=self.ovrf,
                destination=sai_ipprefix(self.customer_ip4+'/32'))
            sai_thrift_create_route_entry(self.client, customer_route3,
                                          next_hop_id=onhop)

            print("Verifying IP 4in4 (encap)")
            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:33',
                ip_dst=self.vm_ip4,
                ip_src=self.customer_ip4,
                dl_vlan_enable=True,
                vlan_vid=500,
                ip_id=108,
                ip_ttl=64)
            pkt2 = simple_tcp_packet(
                eth_dst=self.inner_dmac,
                eth_src=self.default_rmac,
                ip_dst=self.vm_ip4,
                ip_src=self.customer_ip4,
                dl_vlan_enable=True,
                vlan_vid=500,
                ip_id=108,
                ip_ttl=63)
            ipip_pkt = simple_ipv4ip_packet(
                eth_dst=self.underlay_neighbor_mac,
                eth_src=ROUTER_MAC,
                ip_id=0,
                ip_src='10.10.10.10',
                ip_dst='10.10.10.1',
                ip_ttl=64,
                inner_frame=pkt2['IP'])
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, ipip_pkt, self.dev_port25)

            print("Verifying IP 4in4 (decap)")
            pkt1 = simple_tcp_packet(
                eth_src=ROUTER_MAC,
                eth_dst=self.inner_dmac,
                ip_dst=self.customer_ip4,
                ip_src=self.vm_ip4,
                dl_vlan_enable=True,
                vlan_vid=500,
                ip_id=108,
                ip_ttl=64)
            pkt2 = simple_tcp_packet(
                eth_src=ROUTER_MAC,
                eth_dst='00:22:22:22:22:33',
                ip_dst=self.customer_ip4,
                ip_src=self.vm_ip4,
                dl_vlan_enable=True,
                vlan_vid=500,
                ip_id=108,
                ip_ttl=63)
            ipip_pkt = simple_ipv4ip_packet(
                eth_dst=ROUTER_MAC,
                eth_src=self.underlay_neighbor_mac,
                ip_id=0,
                ip_src='10.10.10.1',
                ip_dst='10.10.10.10',
                ip_ttl=64,
                inner_frame=pkt1['IP'])
            send_packet(self, self.dev_port25, ipip_pkt)
            verify_packet(self, pkt2, self.dev_port24)

        finally:
            sai_thrift_remove_route_entry(self.client, customer_route3)
            sai_thrift_remove_next_hop(self.client, onhop)
            sai_thrift_remove_neighbor_entry(self.client, oneighbor)
            sai_thrift_remove_router_interface(self.client, osubport)

    def tearDown(self):
        super(subPortTunnelTest, self).tearDown()


class sviTunnelTest(NhopTunnelEncapDecapTestHelper):
    '''
        Test verifies SVI tunnel nexthop.
    '''
    def setUp(self):
        super(sviTunnelTest, self).setUp()

    def runTest(self):
        print("sviTunnelTest")
        try:
            port24_bp = sai_thrift_create_bridge_port(
                self.client, bridge_id=self.default_1q_bridge,
                port_id=self.port24, type=SAI_BRIDGE_PORT_TYPE_PORT)
            port26_bp = sai_thrift_create_bridge_port(
                self.client, bridge_id=self.default_1q_bridge,
                port_id=self.port26, type=SAI_BRIDGE_PORT_TYPE_PORT)

            # vlan100 with members port24 and port26
            vlan100 = sai_thrift_create_vlan(self.client, vlan_id=100)
            vlan_member100 = sai_thrift_create_vlan_member(
                self.client, vlan_id=vlan100, bridge_port_id=port24_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan_member101 = sai_thrift_create_vlan_member(
                self.client, vlan_id=vlan100, bridge_port_id=port26_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            sai_thrift_set_port_attribute(self.client, self.port24,
                                          port_vlan_id=100)
            sai_thrift_set_port_attribute(self.client, self.port26,
                                          port_vlan_id=100)

            # create vlan100_rif
            osvi = sai_thrift_create_router_interface(
                self.client, type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
                virtual_router_id=self.ovrf, vlan_id=vlan100)

            oneighbor = sai_thrift_neighbor_entry_t(
                rif_id=osvi, ip_address=sai_ipaddress(self.customer_ip4))
            sai_thrift_create_neighbor_entry(
                self.client, oneighbor, dst_mac_address="00:22:22:22:22:33")
            onhop = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress(self.customer_ip4),
                router_interface_id=osvi, type=SAI_NEXT_HOP_TYPE_IP)
            customer_route3 = sai_thrift_route_entry_t(
                vr_id=self.ovrf,
                destination=sai_ipprefix(self.customer_ip4+'/32'))
            sai_thrift_create_route_entry(self.client, customer_route3,
                                          next_hop_id=onhop)
            test_fdb = sai_thrift_fdb_entry_t(
                switch_id=self.switch_id,
                mac_address="00:22:22:22:22:33",
                bv_id=vlan100)
            sai_thrift_create_fdb_entry(self.client,
                                        test_fdb,
                                        type=SAI_FDB_ENTRY_TYPE_STATIC,
                                        bridge_port_id=port24_bp)

            print("Verifying IP 4in4 (encap)")
            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:33',
                ip_dst=self.vm_ip4,
                ip_src=self.customer_ip4,
                ip_id=108,
                ip_ttl=64)
            pkt2 = simple_tcp_packet(
                eth_dst=self.inner_dmac,
                eth_src=self.default_rmac,
                ip_dst=self.vm_ip4,
                ip_src=self.customer_ip4,
                ip_id=108,
                ip_ttl=63)
            ipip_pkt = simple_ipv4ip_packet(
                eth_dst=self.underlay_neighbor_mac,
                eth_src=ROUTER_MAC,
                ip_id=0,
                ip_src='10.10.10.10',
                ip_dst='10.10.10.1',
                ip_ttl=64,
                inner_frame=pkt2['IP'])
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, ipip_pkt, self.dev_port25)

            print("Verifying IP 4in4 (decap)")
            pkt1 = simple_tcp_packet(
                eth_src=ROUTER_MAC,
                eth_dst=self.inner_dmac,
                ip_dst=self.customer_ip4,
                ip_src=self.vm_ip4,
                ip_id=108,
                ip_ttl=64)
            pkt2 = simple_tcp_packet(
                eth_src=ROUTER_MAC,
                eth_dst='00:22:22:22:22:33',
                ip_dst=self.customer_ip4,
                ip_src=self.vm_ip4,
                ip_id=108,
                ip_ttl=63)
            ipip_pkt = simple_ipv4ip_packet(
                eth_dst=ROUTER_MAC,
                eth_src=self.underlay_neighbor_mac,
                ip_id=0,
                ip_src='10.10.10.1',
                ip_dst='10.10.10.10',
                ip_ttl=64,
                inner_frame=pkt1['IP'])
            send_packet(self, self.dev_port25, ipip_pkt)
            verify_packet(self, pkt2, self.dev_port24)

        finally:
            sai_thrift_remove_fdb_entry(self.client, test_fdb)
            sai_thrift_remove_route_entry(self.client, customer_route3)
            sai_thrift_remove_next_hop(self.client, onhop)
            sai_thrift_remove_neighbor_entry(self.client, oneighbor)
            sai_thrift_remove_router_interface(self.client, osvi)
            sai_thrift_set_port_attribute(self.client, self.port24,
                                          port_vlan_id=0)
            sai_thrift_set_port_attribute(self.client, self.port26,
                                          port_vlan_id=0)
            sai_thrift_remove_vlan_member(self.client, vlan_member100)
            sai_thrift_remove_vlan_member(self.client, vlan_member101)
            sai_thrift_remove_vlan(self.client, vlan100)
            sai_thrift_remove_bridge_port(self.client, port24_bp)
            sai_thrift_remove_bridge_port(self.client, port26_bp)

    def tearDown(self):
        super(sviTunnelTest, self).tearDown()


class NhopTunnelVNITestHelper(PlatformSaiHelper):
    '''
        Nexthop tunnel VNI tests.
    '''
    def setUp(self):
        super(NhopTunnelVNITestHelper, self).setUp()
        # underlay config
        self.uvrf = sai_thrift_create_virtual_router(self.client)

        self.vm_ip = '100.100.1.1'
        self.vm_ip2 = '100.100.2.1'
        self.customer_ip = '100.100.3.1'
        self.vni = 2000
        self.my_lb_ip = '10.10.10.10'
        self.tunnel_ip = '10.10.10.1'
        self.tunnel_ip2 = '10.10.10.3'
        self.tunnel_ip3 = '10.10.10.4'
        self.inner_dmac = '00:33:33:33:33:33'
        self.inner_dmac2 = '00:33:33:33:33:44'
        self.inner_dmac3 = '00:33:33:33:33:55'
        self.inner_dmac4 = '00:33:33:33:33:66'
        self.underlay_neighbor_mac = '00:11:11:11:11:11'
        self.underlay_neighbor_mac2 = '00:11:11:11:11:22'
        self.underlay_neighbor_mac3 = '00:11:11:11:11:33'
        self.default_rmac = '00:BA:7E:F0:00:00'  # from bf_switch_device_add

        # overlay config
        self.ovrf = sai_thrift_create_virtual_router(self.client)
        self.tunnel_type = SAI_TUNNEL_TYPE_VXLAN
        self.term_type = SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2MP
        self.ttl_mode = SAI_TUNNEL_TTL_MODE_PIPE_MODEL

        # create underlay loopback rif for tunnel
        self.urif_lb = sai_thrift_create_router_interface(
            self.client, type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.uvrf)

        self.orif = sai_thrift_create_router_interface(
            self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.ovrf,
            port_id=self.port24)

        # Encap configuration follows
        # create Encap/Decap mappers
        self.encap_tunnel_map = sai_thrift_create_tunnel_map(
            self.client, type=SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI)
        self.decap_tunnel_map = sai_thrift_create_tunnel_map(
            self.client, type=SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID)

        # create Encap/Decap mapper entries for ovrf
        self.encap_tunnel_map_entry = sai_thrift_create_tunnel_map_entry(
            self.client,
            tunnel_map_type=SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI,
            tunnel_map=self.encap_tunnel_map, virtual_router_id_key=self.ovrf,
            virtual_router_id_value=self.ovrf, vni_id_key=self.vni,
            vni_id_value=self.vni)
        self.decap_tunnel_map_entry = sai_thrift_create_tunnel_map_entry(
            self.client,
            tunnel_map_type=SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID,
            tunnel_map=self.decap_tunnel_map, virtual_router_id_key=self.ovrf,
            virtual_router_id_value=self.ovrf, vni_id_key=self.vni,
            vni_id_value=self.vni)
        encap_mappers_list = [self.encap_tunnel_map]
        decap_mappers_list = [self.decap_tunnel_map]
        self.encap_mappers_objlist = sai_thrift_object_list_t(
            count=1, idlist=encap_mappers_list)
        self.decap_mappers_objlist = sai_thrift_object_list_t(
            count=1, idlist=decap_mappers_list)
        self.tunnel = sai_thrift_create_tunnel(
            self.client, type=self.tunnel_type,
            encap_src_ip=sai_ipaddress(self.my_lb_ip),
            decap_mappers=self.decap_mappers_objlist,
            encap_mappers=self.encap_mappers_objlist,
            encap_ttl_mode=self.ttl_mode,
            decap_ttl_mode=self.ttl_mode,
            underlay_interface=self.urif_lb)

    def tearDown(self):
        sai_thrift_remove_tunnel(self.client, self.tunnel)
        sai_thrift_remove_tunnel_map_entry(self.client,
                                           self.decap_tunnel_map_entry)
        sai_thrift_remove_tunnel_map_entry(self.client,
                                           self.encap_tunnel_map_entry)
        sai_thrift_remove_tunnel_map(self.client, self.decap_tunnel_map)
        sai_thrift_remove_tunnel_map(self.client, self.encap_tunnel_map)
        sai_thrift_remove_router_interface(self.client, self.orif)
        sai_thrift_remove_router_interface(self.client, self.urif_lb)
        sai_thrift_remove_virtual_router(self.client, self.ovrf)
        sai_thrift_remove_virtual_router(self.client, self.uvrf)

        super(NhopTunnelVNITestHelper, self).tearDown()


class tunnelVniTest(NhopTunnelVNITestHelper):
    '''
        Test verifies correct tunnel VNI behavior.
    '''
    def setUp(self):
        super(tunnelVniTest, self).setUp()

    def runTest(self):
        print("tunnelVniTest")
        vni2 = 5000
        service_vm_ip = "200.200.200.2"
        service_vtep_ip = "30.30.30.3"
        service_vni = 3000
        service_vni2 = 4000

        try:
            urif1 = sai_thrift_create_router_interface(
                self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                port_id=self.port25, virtual_router_id=self.uvrf)
            uneighbor1 = sai_thrift_neighbor_entry_t(
                rif_id=urif1, ip_address=sai_ipaddress(self.tunnel_ip))
            sai_thrift_create_neighbor_entry(
                self.client, uneighbor1,
                dst_mac_address=self.underlay_neighbor_mac)
            unhop1 = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress('10.10.10.1'),
                router_interface_id=urif1, type=SAI_NEXT_HOP_TYPE_IP)
            tunnel_route1 = sai_thrift_route_entry_t(
                vr_id=self.uvrf, destination=sai_ipprefix('10.10.10.1/32'))
            sai_thrift_create_route_entry(self.client, tunnel_route1,
                                          next_hop_id=unhop1)

            urif2 = sai_thrift_create_router_interface(
                self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                port_id=self.port26, virtual_router_id=self.uvrf)
            uneighbor2 = sai_thrift_neighbor_entry_t(
                rif_id=urif2, ip_address=sai_ipaddress(service_vtep_ip))
            sai_thrift_create_neighbor_entry(
                self.client, uneighbor2,
                dst_mac_address=self.underlay_neighbor_mac2)
            unhop2 = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress('30.30.30.3'),
                router_interface_id=urif2, type=SAI_NEXT_HOP_TYPE_IP)
            tunnel_route2 = sai_thrift_route_entry_t(
                vr_id=self.uvrf, destination=sai_ipprefix('30.30.30.3/32'))
            sai_thrift_create_route_entry(self.client, tunnel_route2,
                                          next_hop_id=unhop2)

            tunnel_nexthop1 = sai_thrift_create_next_hop(
                self.client, type=SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP,
                tunnel_id=self.tunnel, ip=sai_ipaddress(self.tunnel_ip),
                tunnel_vni=self.vni, tunnel_mac=self.inner_dmac)

            tunnel_nexthop2 = sai_thrift_create_next_hop(
                self.client, type=SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP,
                tunnel_id=self.tunnel, ip=sai_ipaddress(self.tunnel_ip),
                tunnel_vni=vni2, tunnel_mac=self.inner_dmac)
            self.assertNotEqual(
                tunnel_nexthop2, 0,
                "Failed to create nexthop to same dest ip with different vni")

            tunnel_nexthop3 = sai_thrift_create_next_hop(
                self.client, type=SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP,
                tunnel_id=self.tunnel, ip=sai_ipaddress(service_vtep_ip),
                tunnel_vni=service_vni, tunnel_mac=self.inner_dmac3)

            customer_route1 = sai_thrift_route_entry_t(
                vr_id=self.ovrf, destination=sai_ipprefix(self.vm_ip+'/32'))
            sai_thrift_create_route_entry(self.client, customer_route1,
                                          next_hop_id=tunnel_nexthop1)

            customer_route2 = sai_thrift_route_entry_t(
                vr_id=self.ovrf, destination=sai_ipprefix(self.vm_ip2+'/32'))
            sai_thrift_create_route_entry(self.client, customer_route2,
                                          next_hop_id=tunnel_nexthop2)

            customer_route3 = sai_thrift_route_entry_t(
                vr_id=self.ovrf, destination=sai_ipprefix(service_vm_ip+'/32'))
            sai_thrift_create_route_entry(self.client, customer_route3,
                                          next_hop_id=tunnel_nexthop3)

            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ip_dst=self.vm_ip,
                ip_src=self.customer_ip,
                ip_id=108,
                ip_ttl=64)
            inner_pkt = simple_tcp_packet(
                eth_dst=self.inner_dmac,
                eth_src=ROUTER_MAC,
                ip_dst=self.vm_ip,
                ip_src=self.customer_ip,
                ip_id=108,
                ip_ttl=63)
            vxlan_pkt = Mask(simple_vxlan_packet(
                eth_src=ROUTER_MAC,
                eth_dst=self.underlay_neighbor_mac,
                ip_id=0,
                ip_src=self.my_lb_ip,
                ip_dst=self.tunnel_ip,
                ip_ttl=64,
                ip_flags=0x2,
                with_udp_chksum=False,
                vxlan_vni=self.vni,
                inner_frame=inner_pkt))
            vxlan_pkt.set_do_not_care_scapy(UDP, 'sport')
            print("Sending packet on port %d to vni %d" %
                  (self.dev_port24, self.vni))
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, vxlan_pkt, self.dev_port25)

            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ip_dst=self.vm_ip2,
                ip_src=self.customer_ip,
                ip_id=108,
                ip_ttl=64)
            inner_pkt = simple_tcp_packet(
                eth_dst=self.inner_dmac,
                eth_src=ROUTER_MAC,
                ip_dst=self.vm_ip2,
                ip_src=self.customer_ip,
                ip_id=108,
                ip_ttl=63)
            vxlan_pkt = Mask(simple_vxlan_packet(
                eth_src=ROUTER_MAC,
                eth_dst=self.underlay_neighbor_mac,
                ip_id=0,
                ip_src=self.my_lb_ip,
                ip_dst=self.tunnel_ip,
                ip_ttl=64,
                ip_flags=0x2,
                with_udp_chksum=False,
                vxlan_vni=vni2,
                inner_frame=inner_pkt))
            vxlan_pkt.set_do_not_care_scapy(UDP, 'sport')
            print("Sending packet on port %d to vni %d" %
                  (self.dev_port24, vni2))
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, vxlan_pkt, self.dev_port25)

            print("Updating nexthop tunnel mac {} -> {}".format(
                self.inner_dmac3,
                self.inner_dmac4))
            sai_thrift_set_next_hop_attribute(
                self.client, tunnel_nexthop3, tunnel_mac=self.inner_dmac4)
            print(sai_thrift_get_next_hop_attribute(
                self.client, tunnel_nexthop3, tunnel_mac=True))

            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ip_dst=service_vm_ip,
                ip_src=self.customer_ip,
                ip_id=108,
                ip_ttl=64)
            inner_pkt = simple_tcp_packet(
                eth_dst=self.inner_dmac4,
                eth_src=ROUTER_MAC,
                ip_dst=service_vm_ip,
                ip_src=self.customer_ip,
                ip_id=108,
                ip_ttl=63)
            vxlan_pkt = Mask(simple_vxlan_packet(
                eth_src=ROUTER_MAC,
                eth_dst=self.underlay_neighbor_mac2,
                ip_id=0,
                ip_src=self.my_lb_ip,
                ip_dst=service_vtep_ip,
                ip_ttl=64,
                ip_flags=0x2,
                with_udp_chksum=False,
                vxlan_vni=service_vni,
                inner_frame=inner_pkt))
            vxlan_pkt.set_do_not_care_scapy(UDP, 'sport')
            print("Sending packet on port %d to vni %d" %
                  (self.dev_port24, service_vni))
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, vxlan_pkt, self.dev_port26)

            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ip_dst=service_vm_ip,
                ip_src=self.customer_ip,
                ip_id=108,
                ip_ttl=64)
            inner_pkt = simple_tcp_packet(
                eth_dst=self.inner_dmac4,
                eth_src=ROUTER_MAC,
                ip_dst=service_vm_ip,
                ip_src=self.customer_ip,
                ip_id=108,
                ip_ttl=63)
            vxlan_pkt = Mask(simple_vxlan_packet(
                eth_src=ROUTER_MAC,
                eth_dst=self.underlay_neighbor_mac2,
                ip_id=0,
                ip_src=self.my_lb_ip,
                ip_dst=service_vtep_ip,
                ip_ttl=64,
                ip_flags=0x2,
                with_udp_chksum=False,
                vxlan_vni=service_vni2,
                inner_frame=inner_pkt))
            print("Updating nexthop tunnel vni {} -> {}".format(service_vni,
                                                                service_vni2))

            sai_thrift_set_next_hop_attribute(self.client, tunnel_nexthop3,
                                              tunnel_vni=4000)
            print(sai_thrift_get_next_hop_attribute(self.client,
                                                    tunnel_nexthop3,
                                                    tunnel_vni=True))

            vxlan_pkt.set_do_not_care_scapy(UDP, 'sport')
            print("Sending packet on port %d to vni %d" %
                  (self.dev_port24, service_vni2))
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, vxlan_pkt, self.dev_port26)

        finally:
            if 'customer_route3' in locals() and customer_route3:
                sai_thrift_remove_route_entry(self.client, customer_route3)
            if 'customer_route2' in locals() and customer_route2:
                sai_thrift_remove_route_entry(self.client, customer_route2)
            if 'customer_route1' in locals() and customer_route1:
                sai_thrift_remove_route_entry(self.client, customer_route1)
            if 'tunnel_nexthop3' in locals() and tunnel_nexthop3:
                sai_thrift_remove_next_hop(self.client, tunnel_nexthop3)
            if 'tunnel_nexthop2' in locals() and tunnel_nexthop2:
                sai_thrift_remove_next_hop(self.client, tunnel_nexthop2)
            if 'tunnel_nexthop1' in locals() and tunnel_nexthop1:
                sai_thrift_remove_next_hop(self.client, tunnel_nexthop1)
            sai_thrift_remove_route_entry(self.client, tunnel_route2)
            sai_thrift_remove_route_entry(self.client, tunnel_route1)
            sai_thrift_remove_next_hop(self.client, unhop2)
            sai_thrift_remove_next_hop(self.client, unhop1)
            sai_thrift_remove_neighbor_entry(self.client, uneighbor2)
            sai_thrift_remove_neighbor_entry(self.client, uneighbor1)
            sai_thrift_remove_router_interface(self.client, urif2)
            sai_thrift_remove_router_interface(self.client, urif1)

    def tearDown(self):
        super(tunnelVniTest, self).tearDown()


class tunnelVrfTest(NhopTunnelVNITestHelper):
    '''
        Test verifies correct tunnel behavior with multiple nexthops
        in different VRFs.
    '''
    def setUp(self):
        super(tunnelVrfTest, self).setUp()

    def runTest(self):
        print("tunnelVniTest")

        uvrf2 = sai_thrift_create_virtual_router(self.client)

        # overlay config
        ovrf2 = sai_thrift_create_virtual_router(self.client)

        # create underlay loopback rif for tunnel
        urif_lb2 = sai_thrift_create_router_interface(
            self.client, type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=uvrf2)

        orif2 = sai_thrift_create_router_interface(
            self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=ovrf2,
            port_id=self.port27)

        # Encap configuration follows
        tunnel2 = sai_thrift_create_tunnel(
            self.client, type=self.tunnel_type,
            encap_src_ip=sai_ipaddress(self.my_lb_ip),
            decap_mappers=self.decap_mappers_objlist,
            encap_mappers=self.encap_mappers_objlist,
            encap_ttl_mode=self.ttl_mode,
            decap_ttl_mode=self.ttl_mode,
            underlay_interface=urif_lb2)

        try:
            urif1 = sai_thrift_create_router_interface(
                self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                port_id=self.port25, virtual_router_id=self.uvrf)
            uneighbor1 = sai_thrift_neighbor_entry_t(
                rif_id=urif1, ip_address=sai_ipaddress(self.tunnel_ip))
            sai_thrift_create_neighbor_entry(
                self.client, uneighbor1,
                dst_mac_address=self.underlay_neighbor_mac)
            unhop1 = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress('10.10.10.1'),
                router_interface_id=urif1, type=SAI_NEXT_HOP_TYPE_IP)
            tunnel_route1 = sai_thrift_route_entry_t(
                vr_id=self.uvrf, destination=sai_ipprefix('10.10.10.1/32'))
            sai_thrift_create_route_entry(self.client, tunnel_route1,
                                          next_hop_id=unhop1)

            urif2 = sai_thrift_create_router_interface(
                self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                port_id=self.port26, virtual_router_id=uvrf2)
            uneighbor2 = sai_thrift_neighbor_entry_t(
                rif_id=urif2, ip_address=sai_ipaddress(self.tunnel_ip))
            sai_thrift_create_neighbor_entry(
                self.client, uneighbor2,
                dst_mac_address=self.underlay_neighbor_mac)
            unhop2 = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress('10.10.10.1'),
                router_interface_id=urif2, type=SAI_NEXT_HOP_TYPE_IP)
            tunnel_route2 = sai_thrift_route_entry_t(
                vr_id=uvrf2, destination=sai_ipprefix('10.10.10.1/32'))
            sai_thrift_create_route_entry(self.client, tunnel_route2,
                                          next_hop_id=unhop2)

            tunnel_nexthop1 = sai_thrift_create_next_hop(
                self.client, type=SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP,
                tunnel_id=self.tunnel, ip=sai_ipaddress(self.tunnel_ip),
                tunnel_vni=self.vni, tunnel_mac=self.inner_dmac)

            tunnel_nexthop2 = sai_thrift_create_next_hop(
                self.client, type=SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP,
                tunnel_id=tunnel2, ip=sai_ipaddress(self.tunnel_ip),
                tunnel_vni=self.vni, tunnel_mac=self.inner_dmac)
            self.assertNotEqual(
                tunnel_nexthop2, 0,
                "Failed to create nexthop to same dest ip in different vrf")

            customer_route1 = sai_thrift_route_entry_t(
                vr_id=self.ovrf, destination=sai_ipprefix(self.vm_ip+'/32'))
            sai_thrift_create_route_entry(self.client, customer_route1,
                                          next_hop_id=tunnel_nexthop1)

            customer_route2 = sai_thrift_route_entry_t(
                vr_id=ovrf2, destination=sai_ipprefix(self.vm_ip+'/32'))
            sai_thrift_create_route_entry(self.client, customer_route2,
                                          next_hop_id=tunnel_nexthop2)

            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:22:22:22:22:22',
                ip_dst=self.vm_ip,
                ip_src=self.customer_ip,
                ip_id=108,
                ip_ttl=64)
            inner_pkt = simple_tcp_packet(
                eth_dst=self.inner_dmac,
                eth_src=ROUTER_MAC,
                ip_dst=self.vm_ip,
                ip_src=self.customer_ip,
                ip_id=108,
                ip_ttl=63)
            vxlan_pkt = Mask(simple_vxlan_packet(
                eth_src=ROUTER_MAC,
                eth_dst=self.underlay_neighbor_mac,
                ip_id=0,
                ip_src=self.my_lb_ip,
                ip_dst=self.tunnel_ip,
                ip_ttl=64,
                ip_flags=0x2,
                with_udp_chksum=False,
                vxlan_vni=self.vni,
                inner_frame=inner_pkt))
            vxlan_pkt.set_do_not_care_scapy(UDP, 'sport')
            print("Sending packet on port %d to vni %d" %
                  (self.dev_port24, self.vni))
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, vxlan_pkt, self.dev_port25)

            print("Sending packet on port %d to vni %d" %
                  (self.dev_port27, self.vni))
            send_packet(self, self.dev_port27, pkt)
            verify_packet(self, vxlan_pkt, self.dev_port26)

        finally:
            if 'customer_route2' in locals() and customer_route2:
                sai_thrift_remove_route_entry(self.client, customer_route2)
            if 'customer_route1' in locals() and customer_route1:
                sai_thrift_remove_route_entry(self.client, customer_route1)
            if 'tunnel_nexthop2' in locals() and tunnel_nexthop2:
                sai_thrift_remove_next_hop(self.client, tunnel_nexthop2)
            if 'tunnel_nexthop1' in locals() and tunnel_nexthop1:
                sai_thrift_remove_next_hop(self.client, tunnel_nexthop1)
            sai_thrift_remove_route_entry(self.client, tunnel_route2)
            sai_thrift_remove_route_entry(self.client, tunnel_route1)
            sai_thrift_remove_next_hop(self.client, unhop2)
            sai_thrift_remove_next_hop(self.client, unhop1)
            sai_thrift_remove_neighbor_entry(self.client, uneighbor2)
            sai_thrift_remove_neighbor_entry(self.client, uneighbor1)
            sai_thrift_remove_router_interface(self.client, urif2)
            sai_thrift_remove_router_interface(self.client, urif1)
            sai_thrift_remove_tunnel(self.client, tunnel2)
            sai_thrift_remove_router_interface(self.client, orif2)
            sai_thrift_remove_router_interface(self.client, urif_lb2)
            sai_thrift_remove_virtual_router(self.client, ovrf2)
            sai_thrift_remove_virtual_router(self.client, uvrf2)

    def tearDown(self):
        super(tunnelVrfTest, self).tearDown()
