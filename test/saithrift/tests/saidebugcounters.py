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
Thrift SAI interface Debug Counters tests
"""
import socket
from switch import *
import sai_base_test

class BaseDebugCounterTest(sai_base_test.ThriftInterfaceDataPlane):
    default_mac_1 = '00:11:11:11:11:11'
    default_mac_2 = '00:22:22:22:22:22'
    neighbor_mac  = '00:11:22:33:44:55'
    reserved_mac  = '01:80:C2:00:00:01'
    mc_mac        = '01:00:5E:AA:AA:AA'

    neighbor_ip   = '10.10.10.1'
    neighbor_ip_2 = '10.10.10.2'
    loopback_ip   = '127.0.0.1'
    mc_ip         = '224.0.0.1'
    class_e_ip    = '240.0.0.1'
    unspec_ip     = '0.0.0.0'
    bc_ip         = '255.255.255.255'
    link_local_ip = '169.254.0.1'

    neighbor_ipv6   = '1234:5678:9abc:def0:4422:1133:5577:99aa'
    mc_scope_0_ipv6 = 'FF00:0:0:0:0:0:0:1'
    mc_scope_1_ipv6 = 'FF01:0:0:0:0:0:0:1'

    ingress_port  = 0
    egress_port   = 1

    addr_family        = SAI_IP_ADDR_FAMILY_IPV4
    ip_addr_subnet     = '10.10.10.0'
    ip_mask            = '255.255.255.0'
    addr_family_v6     = SAI_IP_ADDR_FAMILY_IPV6
    ip_addr_subnet_v6  = '1234:5678:9abc:def0:4422:1133:5577:0'
    ip_mask_v6         = 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:0'
    v4_enabled = 1
    v6_enabled = 1

    dc_oid=None
    vr_ids=[]
    rif_ids=[]
    neighbors=[]
    routes=[]
    fdbs=[]

    def set_counter_reasons(self, in_drop_reasons):
        if not isinstance(in_drop_reasons, list):
            in_drop_reasons = [in_drop_reasons]

        dc_reasons_list = sai_thrift_s32_list_t(count=len(in_drop_reasons), s32list=in_drop_reasons)
        dc_reasons_value = sai_thrift_attribute_value_t(s32list=dc_reasons_list)
        dc_reasons = sai_thrift_attribute_t(id=SAI_DEBUG_COUNTER_ATTR_IN_DROP_REASON_LIST, value=dc_reasons_value)

        self.client.sai_thrift_set_debug_counter_attribute(self.dc_oid, dc_reasons)

        return

    def create_counter(self, in_drop_reasons):
        if not isinstance(in_drop_reasons, list):
            in_drop_reasons = [in_drop_reasons]

        if self.dc_oid is not None:
            self.set_counter_reasons(in_drop_reasons)
            return

        dc_type_value = sai_thrift_attribute_value_t(s32=SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS)
        dc_type = sai_thrift_attribute_t(id=SAI_DEBUG_COUNTER_ATTR_TYPE, value=dc_type_value)

        dc_reasons_list = sai_thrift_s32_list_t(count=len(in_drop_reasons), s32list=in_drop_reasons)
        dc_reasons_value = sai_thrift_attribute_value_t(s32list=dc_reasons_list)
        dc_reasons = sai_thrift_attribute_t(id=SAI_DEBUG_COUNTER_ATTR_IN_DROP_REASON_LIST, value=dc_reasons_value)

        self.dc_oid = self.client.sai_thrift_create_debug_counter([dc_type, dc_reasons])

        return


    def create_router(self):
        self.vr_ids.append(sai_thrift_create_virtual_router(self.client, self.v4_enabled, self.v6_enabled))

        self.rif_ids.append(sai_thrift_create_router_interface(self.client, self.vr_ids[-1], SAI_ROUTER_INTERFACE_TYPE_PORT, port_list[self.ingress_port], 0, self.v4_enabled, self.v6_enabled, router_mac))
        self.rif_ids.append(sai_thrift_create_router_interface(self.client, self.vr_ids[-1], SAI_ROUTER_INTERFACE_TYPE_PORT, port_list[self.egress_port], 0, self.v4_enabled, self.v6_enabled, router_mac))

        return

    def set_rif_attribute(self, rif_id, attr_id, attr_value):
        rif_attribute_value=None
        if attr_id == SAI_ROUTER_INTERFACE_ATTR_MTU:
            rif_attribute_value = sai_thrift_attribute_value_t(u32=attr_value)
        elif attr_id == SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE:
            rif_attribute_value = sai_thrift_attribute_value_t(booldata=attr_value)
        elif attr_id == SAI_ROUTER_INTERFACE_ATTR_LOOPBACK_PACKET_ACTION:
            rif_attribute_value = sai_thrift_attribute_value_t(s32=attr_value)
        else:
            return

        rif_attribute = sai_thrift_attribute_t(id    = attr_id,
                                               value = rif_attribute_value)
        self.client.sai_thrift_set_router_interface_attribute(rif_id, rif_attribute)
        return

    def create_route(self, packet_action=None):
        self.routes.append([self.vr_ids[-1], self.addr_family, self.ip_addr_subnet, self.ip_mask, self.rif_ids[-1], packet_action])
        sai_thrift_create_route(self.client, *self.routes[-1])

        return

    def create_route_v6(self):
        self.routes.append([self.vr_ids[-1], self.addr_family_v6, self.ip_addr_subnet_v6, self.ip_mask_v6, self.rif_ids[-1]])
        sai_thrift_create_route(self.client, *self.routes[-1])

        return

    def create_neighbor(self):
        self.neighbors.append([self.addr_family, self.rif_ids[-1], self.neighbor_ip, self.neighbor_mac])
        sai_thrift_create_neighbor(self.client, *self.neighbors[-1])

        return

    def create_neighbor_v6(self):
        self.neighbors.append([self.addr_family_v6, self.rif_ids[-1], self.neighbor_ip_v6, self.neighbor_mac])
        sai_thrift_create_neighbor(self.client, *self.neighbors[-1])

        return

    def create_fdb(self, port, mac, action):
        self.fdbs.append([switch.default_vlan.oid, mac, port, action])
        sai_thrift_create_fdb(self.client, *self.fdbs[-1])

        return

    def test_counter(self, in_drop_reasons, pkts, result=1):
        if not isinstance(pkts, list):
            pkts = [pkts]
        self.create_counter(in_drop_reasons)
        counter_before = self.client.sai_thrift_get_switch_stats_by_oid(self.dc_oid)
        self.assertTrue(0 == counter_before)
        try:
            for pkt in pkts:
                send_packet(self, self.ingress_port, str(pkt))
                if result != 0:
                    verify_no_packet(self, pkt, self.egress_port)
        finally:
            counter_after = self.client.sai_thrift_get_switch_stats_by_oid(self.dc_oid)
            self.assertTrue(counter_after == result)

        return

    def cleanup(self):
        self.ingress_port  = 0
        self.egress_port   = 1
        del self.fdbs[:]
        del self.vr_ids[:]
        del self.rif_ids[:]
        del self.neighbors[:]
        del self.routes[:]
        self.dc_oid=None

        return

    def setUp(self):
        ThriftInterfaceDataPlane.setUp(self)
        self.cleanup()

        return

    def tearDown(self):
        if self.dc_oid is not None:
            self.client.sai_thrift_remove_debug_counter(self.dc_oid)

        for route_data in self.routes:
            sai_thrift_remove_route(self.client, *route_data[:-1])

        for neighbor_data in self.neighbors:
            sai_thrift_remove_neighbor(self.client, *neighbor_data)

        for fdb in self.fdbs:
            sai_thrift_delete_fdb(self.client, *fdb[:-1])

        for rif in self.rif_ids:
            self.client.sai_thrift_remove_router_interface(rif)

        for vr in self.vr_ids:
            self.client.sai_thrift_remove_virtual_router(vr)

        self.cleanup()

        ThriftInterfaceDataPlane.tearDown(self)

        return

""" Debug counters API tests """

""" L2 reasons """

@group('debug_counters')
class DropMCSMAC(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        pkt = simple_tcp_packet(eth_dst=self.default_mac_1,
                                eth_src=self.mc_mac)

        self.test_counter(SAI_IN_DROP_REASON_SMAC_MULTICAST, pkt);

        return

@group('debug_counters')
class DropSMACequalsDMAC(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        pkt = simple_tcp_packet(eth_dst=self.default_mac_1,
                                eth_src=self.default_mac_1)

        self.test_counter(SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC, pkt);

        return

@group('debug_counters')
class DropDMACReserved(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        pkt = simple_tcp_packet(eth_dst=self.reserved_mac,
                                eth_src=self.default_mac_1)

        self.test_counter(SAI_IN_DROP_REASON_DMAC_RESERVED, pkt);

        return

@group('debug_counters')
class DropIngressVLANFilter(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        pkt = simple_tcp_packet(eth_dst=self.default_mac_2,
                                eth_src=self.default_mac_1,
                                vlan_vid=42, dl_vlan_enable=True)

        self.test_counter(SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER, pkt);

        return

@group('debug_counters')
class DropL2LoopbackFilter(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_fdb(port_list[0], self.default_mac_2, SAI_PACKET_ACTION_FORWARD)

        pkt = simple_tcp_packet(eth_dst=self.default_mac_2,
                                eth_src=self.default_mac_1)

        self.test_counter(SAI_IN_DROP_REASON_L2_LOOPBACK_FILTER, pkt);

        return

""" L3 reasons """

@group('debug_counters')
class DropL3LoopbackFilter(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()
        self.set_rif_attribute(self.rif_ids[1], SAI_ROUTER_INTERFACE_ATTR_LOOPBACK_PACKET_ACTION, SAI_PACKET_ACTION_DROP)
        self.create_neighbor()
        self.create_route()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_dst=self.neighbor_ip,
                                ip_src=self.neighbor_ip_2)
        self.ingress_port = 1
        self.egress_port = 0
        self.test_counter(SAI_IN_DROP_REASON_L3_LOOPBACK_FILTER, pkt)

        return

@group('debug_counters')
class DropNonRoutable(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_igmp_packet(eth_dst=router_mac,
                                 eth_src=self.default_mac_1)

        self.test_counter(SAI_IN_DROP_REASON_NON_ROUTABLE, pkt)

        return

@group('debug_counters')
class DropNoL3Header(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_arp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1)

        self.test_counter(SAI_IN_DROP_REASON_NO_L3_HEADER, pkt)

        return

@group('debug_counters')
class DropIPHeaderError(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_ihl=1)

        self.test_counter(SAI_IN_DROP_REASON_IP_HEADER_ERROR, pkt)

        return

@group('debug_counters')
class DropUCDIPMCDMAC(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=self.mc_mac,
                                eth_src=self.default_mac_1)

        self.test_counter(SAI_IN_DROP_REASON_UC_DIP_MC_DMAC, pkt)

        return

@group('debug_counters')
class DropDIPLoopback(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_dst=self.loopback_ip)

        self.test_counter(SAI_IN_DROP_REASON_DIP_LOOPBACK, pkt)

        return

@group('debug_counters')
class DropSIPLoopback(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_src=self.loopback_ip)

        self.test_counter(SAI_IN_DROP_REASON_SIP_LOOPBACK, pkt)

        return

@group('debug_counters')
class DropMulticastSIP(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_src=self.mc_ip)

        self.test_counter(SAI_IN_DROP_REASON_SIP_MC, pkt)

        return

@group('debug_counters')
class DropSIPClassE(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_src=self.class_e_ip)

        self.test_counter(SAI_IN_DROP_REASON_SIP_CLASS_E, pkt)

        return

@group('debug_counters')
class DropSIPUnspecified(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_src=self.unspec_ip)

        self.test_counter(SAI_IN_DROP_REASON_SIP_UNSPECIFIED, pkt)

        return

@group('debug_counters')
class DropMCDMACMismatch(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=self.mc_mac,
                                eth_src=self.default_mac_1,
                                ip_dst=self.mc_ip)

        self.test_counter(SAI_IN_DROP_REASON_MC_DMAC_MISMATCH, pkt)

        return

@group('debug_counters')
class DropSIPEqualsDIP(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_dst=self.neighbor_ip,
                                ip_src=self.neighbor_ip)

        self.test_counter(SAI_IN_DROP_REASON_SIP_EQUALS_DIP, pkt)

        return

@group('debug_counters')
class DropSIPBC(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_src=self.bc_ip)

        self.test_counter(SAI_IN_DROP_REASON_SIP_BC, pkt)

        return

@group('debug_counters')
class DropDIPLocal(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_dst=self.unspec_ip)

        self.test_counter(SAI_IN_DROP_REASON_DIP_LOCAL, pkt)

        return

@group('debug_counters')
class DropDIPLinkLocal(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_dst=self.link_local_ip)

        self.test_counter(SAI_IN_DROP_REASON_DIP_LINK_LOCAL, pkt)

        return

@group('debug_counters')
class DropSIPLinkLocal(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_src=self.link_local_ip)

        self.test_counter(SAI_IN_DROP_REASON_SIP_LINK_LOCAL, pkt)

        return

@group('debug_counters')
class DropIPv6MCScope0(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcpv6_packet(eth_dst=router_mac,
                                  eth_src=self.default_mac_1,
                                  ipv6_dst=self.mc_scope_0_ipv6)

        self.test_counter(SAI_IN_DROP_REASON_IPV6_MC_SCOPE0, pkt)

        return

@group('debug_counters')
class DropIPv6MCScope1(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcpv6_packet(eth_dst=router_mac,
                                  eth_src=self.default_mac_1,
                                  ipv6_dst=self.mc_scope_1_ipv6)

        self.test_counter(SAI_IN_DROP_REASON_IPV6_MC_SCOPE1, pkt)

        return

@group('debug_counters')
class DropIRIFDisabled(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()
        self.set_rif_attribute(self.rif_ids[0], SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE, False)
        self.create_neighbor()
        self.create_route()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_dst=self.neighbor_ip)

        self.test_counter(SAI_IN_DROP_REASON_IRIF_DISABLED, pkt)

        return

@group('debug_counters')
class DropERIFDisabled(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()
        self.set_rif_attribute(self.rif_ids[1], SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE, False)
        self.create_neighbor()
        self.create_route()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_dst=self.neighbor_ip)

        self.test_counter(SAI_IN_DROP_REASON_ERIF_DISABLED, pkt)

        return

@group('debug_counters')
class DropLPM4Miss(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1)

        self.test_counter(SAI_IN_DROP_REASON_LPM4_MISS, pkt)

        return

@group('debug_counters')
class DropLPM6Miss(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt = simple_tcpv6_packet(eth_dst=router_mac,
                                  eth_src=self.default_mac_1)

        self.test_counter(SAI_IN_DROP_REASON_LPM6_MISS, pkt)

        return

@group('debug_counters')
class DropBlackholeRoute(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()
        self.create_route(SAI_PACKET_ACTION_DROP)

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_dst=self.neighbor_ip)

        self.test_counter(SAI_IN_DROP_REASON_BLACKHOLE_ROUTE, pkt)

        return

""" ACL reasons """

@group('debug_counters')
class DropACLAny(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()
        self.create_neighbor()
        self.create_route()

        table_stage           = SAI_ACL_STAGE_INGRESS
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        entry_priority        = SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY
        action                = SAI_PACKET_ACTION_DROP
        in_ports              = port_list[0], port_list[1]
        mac_src               = None
        mac_dst               = None
        mac_src_mask          = None
        mac_dst_mask          = "ff:ff:ff:ff:ff:ff"
        ip_src                = "192.168.0.1"
        ip_src_mask           = "255.255.255.0"
        ip_dst                = None
        ip_dst_mask           = None
        ip_proto              = None
        in_port               = None
        out_port              = None
        out_ports             = None
        src_l4_port           = None
        dst_l4_port           = None
        ingress_mirror_id     = None
        egress_mirror_id      = None

        acl_table_id = sai_thrift_create_acl_table(self.client,
            table_stage,
            table_bind_point_list,
            self.addr_family,
            mac_src,
            mac_dst,
            ip_src,
            ip_dst,
            ip_proto,
            in_ports,
            out_ports,
            in_port,
            out_port,
            src_l4_port,
            dst_l4_port)
        acl_entry_id = sai_thrift_create_acl_entry(self.client,
            acl_table_id,
            entry_priority,
            action, self.addr_family,
            mac_src, mac_src_mask,
            mac_dst, mac_dst_mask,
            ip_src, ip_src_mask,
            ip_dst, ip_dst_mask,
            ip_proto,
            in_ports, out_ports,
            in_port, out_port,
            src_l4_port, dst_l4_port,
            ingress_mirror_id,
            egress_mirror_id)

        attr_value = sai_thrift_attribute_value_t(oid=acl_table_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port_list[0], attr)

        pkt = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=self.default_mac_1,
                                ip_dst=self.neighbor_ip)

        self.test_counter(SAI_IN_DROP_REASON_ACL_ANY, pkt)

        attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port_list[0], attr)

        return

""" Negative cases """

@group('debug_counters')
class NoDropIngressVLANFilter(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        pkt = simple_tcp_packet(eth_dst=self.default_mac_2,
                                eth_src=self.default_mac_1)

        self.test_counter(SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER, pkt, 0);

        return

""" Multiple reasons """

@group('debug_counters')
class DropMultipleReasons(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt_ipv6_miss = simple_tcpv6_packet(eth_dst=router_mac,
                                            eth_src=self.default_mac_1)

        pkt_vlan = simple_tcp_packet(eth_dst=self.default_mac_2,
                                     eth_src=self.default_mac_1,
                                     vlan_vid=42, dl_vlan_enable=True)

        self.test_counter([SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER, SAI_IN_DROP_REASON_LPM6_MISS],
                [pkt_vlan, pkt_ipv6_miss], 2)

        return

""" Editing drop reasons """

@group('debug_counters')
class EditingDropReasons(BaseDebugCounterTest):
    def runTest(self):
        switch_init(self.client)

        self.create_router()

        pkt_ipv6_miss = simple_tcpv6_packet(eth_dst=router_mac,
                                            eth_src=self.default_mac_1)

        pkt_vlan = simple_tcp_packet(eth_dst=self.default_mac_2,
                                     eth_src=self.default_mac_1,
                                     vlan_vid=42, dl_vlan_enable=True)

        self.create_counter([SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER])
        counter = self.client.sai_thrift_get_switch_stats_by_oid(self.dc_oid)
        self.assertTrue(0 == counter)
        try:
            send_packet(self, self.ingress_port, str(pkt_vlan))
            verify_no_packet(self, pkt_vlan, self.egress_port)
        finally:
            counter = self.client.sai_thrift_get_switch_stats_by_oid(self.dc_oid)
            self.assertTrue(counter == 1)

        self.set_counter_reasons([SAI_IN_DROP_REASON_LPM6_MISS])
        try:
            send_packet(self, self.ingress_port, str(pkt_ipv6_miss))
            verify_no_packet(self, pkt_ipv6_miss, self.egress_port)
        finally:
            counter = self.client.sai_thrift_get_switch_stats_by_oid(self.dc_oid)
            self.assertTrue(counter == 2)

        try:
            send_packet(self, self.ingress_port, str(pkt_vlan))
            verify_no_packet(self, pkt_vlan, self.egress_port)
        finally:
            counter = self.client.sai_thrift_get_switch_stats_by_oid(self.dc_oid)
            self.assertTrue(counter == 2)

        return
