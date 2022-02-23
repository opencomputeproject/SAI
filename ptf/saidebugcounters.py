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

'''
Thrift SAI interface DebugCounters tests
'''

from sai_thrift.sai_headers import *

from sai_base_test import *

TEST_GET_STATS_DELAY = 3


def map_drop_reason_to_string(drop_reason_list):
    ''' Maps DebugCounter drop reason to a string.
    Args:
        drop_reason_list (list): Drop reasons list

    Returns:
        string: drop reasons list names
    '''

    if not isinstance(drop_reason_list, list):
        drop_reason_list = [drop_reason_list]
    names = []
    drop_reason_map = {}
    drop_reason_map[SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER] = "VLAN_FILTER"
    drop_reason_map[SAI_IN_DROP_REASON_L2_ANY] = "L2_ANY"
    drop_reason_map[SAI_IN_DROP_REASON_SMAC_MULTICAST] = "SMAC_MC"
    drop_reason_map[SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC] = "SMAC_EQUALS_DMAC"
    drop_reason_map[SAI_IN_DROP_REASON_TTL] = "TTL"
    drop_reason_map[SAI_IN_DROP_REASON_IP_HEADER_ERROR] = "IP_HEADER_ERROR"
    drop_reason_map[SAI_IN_DROP_REASON_SIP_MC] = "SIP_MC"
    drop_reason_map[SAI_IN_DROP_REASON_L3_ANY] = "L3_ANY"
    drop_reason_map[SAI_IN_DROP_REASON_IRIF_DISABLED] = "IRIF_DISABLED"
    drop_reason_map[SAI_IN_DROP_REASON_ACL_ANY] = "ACL_ANY"
    drop_reason_map[SAI_IN_DROP_REASON_DIP_LOOPBACK] = "DIP_LOOPBACK"
    drop_reason_map[SAI_IN_DROP_REASON_SIP_LOOPBACK] = "SIP_LOOPBACK"
    drop_reason_map[SAI_IN_DROP_REASON_SIP_CLASS_E] = "SIP_CLASS_E"
    drop_reason_map[SAI_IN_DROP_REASON_DIP_LINK_LOCAL] = "DIP_LINK_LOCAL"
    drop_reason_map[SAI_IN_DROP_REASON_SIP_LINK_LOCAL] = "SIP_LINK_LOCAL"
    drop_reason_map[SAI_IN_DROP_REASON_SIP_UNSPECIFIED] = "SIP_UNSPECIFIED"
    drop_reason_map[SAI_IN_DROP_REASON_UC_DIP_MC_DMAC] = "UC_DIP_MC_DMAC"
    drop_reason_map[SAI_IN_DROP_REASON_NON_ROUTABLE] = "IGMP_NON_ROUTABLE"
    drop_reason_map[SAI_IN_DROP_REASON_MPLS_MISS] = "MPLS_MISS"
    drop_reason_map[SAI_IN_DROP_REASON_SRV6_LOCAL_SID_DROP] = \
        "SRV6_LOCAL_SID_DROP"
    drop_reason_map[SAI_IN_DROP_REASON_LPM4_MISS] = "LPM4_MISS"
    drop_reason_map[SAI_IN_DROP_REASON_LPM6_MISS] = "LPM6_MISS"
    drop_reason_map[SAI_IN_DROP_REASON_BLACKHOLE_ROUTE] = "BLACKHOLE_ROUTE"

    for drop_reason in drop_reason_list:
        names.append(drop_reason_map[drop_reason])
    names = ",".join(names)

    return names


def debug_counter_type_to_index_base(dc_type):
    ''' Converts the DebugCounter type into its index base
    Args:
        dc_type (sai_debug_counter_type_t): debug counter type.

    Returns:
        string: drop reasons list names
    '''

    index_base = 0
    if dc_type == SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS:
        index_base = SAI_PORT_STAT_IN_DROP_REASON_RANGE_BASE
    elif dc_type == SAI_DEBUG_COUNTER_TYPE_PORT_OUT_DROP_REASONS:
        index_base = SAI_PORT_STAT_OUT_DROP_REASON_RANGE_BASE
    elif dc_type == SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS:
        index_base = SAI_SWITCH_STAT_IN_DROP_REASON_RANGE_BASE
    elif dc_type == SAI_DEBUG_COUNTER_TYPE_SWITCH_OUT_DROP_REASONS:
        index_base = SAI_SWITCH_STAT_OUT_DROP_REASON_RANGE_BASE

    return index_base


def get_port_stats(port, counters):
    ''' Returns the port stats counters.

    Args:
        port (sai_object_id_t): port object id
        counters (list) : requested counters list

    Returns:
        list: counter stats for requested counters list.
    '''

    if not isinstance(counters, list):
        counters = [counters]

    time.sleep(TEST_GET_STATS_DELAY)
    stats = sai_thrift_get_debug_counter_port_stats(port, counters)
    return stats


def get_switch_stats(counters):
    ''' Returns the switch stats counters.

    Args:
        counters (list) : requested counters list

    Returns:
        list: stats counter stats for requested counters list.
    '''
    if not isinstance(counters, list):
        counters = [counters]

    time.sleep(TEST_GET_STATS_DELAY)
    stats = sai_thrift_get_debug_counter_switch_stats(counters)

    return stats


@group("draft")
class BaseDebugCounterClass(SaiHelperBase):
    ''' SAI DebugCounters test class. '''

    default_mac_1 = '00:11:11:11:11:11'
    default_mac_2 = '00:22:22:22:22:22'
    neighbor_mac = '00:11:22:33:44:55'
    neighbor_mac_v6 = '00:11:22:33:44:66'
    reserved_mac = '01:80:C2:00:00:01'
    mc_mac_1 = '01:00:5E:AA:AA:AA'
    mc_mac_2 = '33:33:5E:AA:AA:AA'

    neighbor_ip = '10.10.10.1'
    neighbor_ip_prefix = '10.10.10.0/24'
    neighbor_ip_2 = '10.10.10.2'
    loopback_ip = '127.0.0.1'
    unknown_neighbor_ipv4 = '12.12.12.1'
    blackhole_ip = '13.13.13.1/32'
    mc_ip = '224.0.0.1'
    class_e_ip = '240.0.0.1'
    unspec_ipv4 = '0.0.0.0'
    unspec_ipv6 = '::0'
    bc_ip = '255.255.255.255'
    link_local_ip = '169.254.0.1'
    link_local_ip_prefix = '169.254.0.0/16'

    neighbor_ipv6 = '1234:5678:9abc:def0:4422:1133:5577:99aa'
    neighbor_ipv6_prefix = '1234:5678:9abc:def0:4422:1133:5577:99aa/64'
    unknown_neighbor_ipv6 = '4444:5678:9abc:def0:4422:1133:5577:3333'
    mc_scope_0_ipv6 = 'FF00:0:0:0:0:0:0:1'
    mc_scope_1_ipv6 = 'FF01:0:0:0:0:0:0:1'

    addr_family = SAI_IP_ADDR_FAMILY_IPV4
    ip_addr_subnet = '10.10.10.0'
    ip_mask = '255.255.255.0'
    addr_family_v6 = SAI_IP_ADDR_FAMILY_IPV6
    ip_addr_subnet_v6 = '1234:5678:9abc:def0:4422:1133:5577:0'
    ip_mask_v6 = 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:0'
    v4_enabled = 1
    v6_enabled = 1
    igmp_type_query = 0x11
    igmp_type_leave = 0x17
    igmp_type_v1_report = 0x12
    igmp_type_v2_report = 0x16
    igmp_type_v3_report = 0x22

    dc_oid = None
    second_dc_oid = None
    vr_ids = []
    rif_ids = []
    neighbors = []
    routes = []
    fdbs = []

    def setUp(self):

        super(BaseDebugCounterClass, self).setUp()
        self.cleanup()

        self.test_dev_ports = [
            self.dev_port0,
            self.dev_port1,
            self.dev_port2,
            self.dev_port3,
            self.dev_port4,
            self.dev_port5,
            self.dev_port6,
            self.dev_port7]

        self.test_ports = [
            self.port0,
            self.port1,
            self.port2,
            self.port3,
            self.port4,
            self.port5,
            self.port6,
            self.port7]

        self.ingress_port = self.dev_port0
        self.egress_port = self.dev_port1

        self.ihl_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ip_ihl=1)
        self.src_ip_mc_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ip_src=self.mc_ip)
        self.smac_equals_dmac_pkt = simple_tcp_packet(
            eth_dst=self.default_mac_1,
            eth_src=self.default_mac_1)
        self.ttl_zero_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ip_dst=self.neighbor_ip,
            ip_ttl=0)
        self.mc_smac_pkt = simple_tcp_packet(
            eth_dst=self.default_mac_1,
            eth_src=self.mc_mac_1)
        self.zero_smac_pkt = simple_tcp_packet(
            eth_dst=self.default_mac_1,
            eth_src='00:00:00:00:00:00')
        self.reserved_dmac_pkt = simple_tcp_packet(
            eth_dst='01:80:C2:00:00:00',
            eth_src=self.default_mac_1,)
        self.vlan_discard_pkt = simple_tcp_packet(
            eth_dst=self.default_mac_2,
            eth_src=self.default_mac_1,
            vlan_vid=42, dl_vlan_enable=True)
        self.src_ip_class_e_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ip_src=self.class_e_ip)
        self.ip_dst_loopback_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ip_dst=self.loopback_ip)
        self.ip_src_loopback_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ip_src=self.loopback_ip)
        self.dst_ip_link_local_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ip_dst=self.link_local_ip)
        self.src_ip_link_local_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ip_dst=self.neighbor_ip,
            ip_src=self.link_local_ip)
        self.dst_ipv4_unspec_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ip_dst=self.unspec_ipv4)
        self.dst_ipv6_unspec_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ipv6_dst=self.unspec_ipv6)
        self.src_ipv4_unspec_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ip_dst=self.neighbor_ip,
            ip_src=self.unspec_ipv4)
        self.uc_dipv4_mc_dmac_pkt = simple_tcp_packet(
            eth_dst=self.mc_mac_1,
            eth_src=self.default_mac_1)
        self.uc_dipv6_mc_dmac_pkt = simple_tcpv6_packet(
            eth_dst=self.mc_mac_2,
            eth_src=self.default_mac_1)
        self.lpm4_miss_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ip_dst=self.unknown_neighbor_ipv4)
        self.lpm6_miss_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ipv6_dst=self.unknown_neighbor_ipv6)
        self.blackhole_route_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.default_mac_1,
            ip_dst=self.blackhole_ip)

        self.map_drop_reasons = []
        self.map_drop_reasons.append(
            [SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER, [self.vlan_discard_pkt]])
        self.map_drop_reasons.append(
            [SAI_IN_DROP_REASON_IP_HEADER_ERROR, [self.ihl_pkt]])
        self.map_drop_reasons.append(
            [SAI_IN_DROP_REASON_SIP_MC, [self.src_ip_mc_pkt]])
        self.map_drop_reasons.append(
            [SAI_IN_DROP_REASON_SMAC_MULTICAST, [self.mc_smac_pkt]])
        self.map_drop_reasons.append(
            [SAI_IN_DROP_REASON_TTL, [self.ttl_zero_pkt]])
        self.map_drop_reasons.append(
            [SAI_IN_DROP_REASON_LPM4_MISS, [self.lpm4_miss_pkt]])
        self.map_drop_reasons.append(
            [SAI_IN_DROP_REASON_LPM6_MISS, [self.lpm6_miss_pkt]])
        self.map_drop_reasons.append(
            [SAI_IN_DROP_REASON_BLACKHOLE_ROUTE, [self.blackhole_route_pkt]])
        self.map_drop_reasons.append(
            [SAI_IN_DROP_REASON_L3_ANY, [self.ttl_zero_pkt,
                                         self.ihl_pkt,
                                         self.src_ip_mc_pkt,
                                         self.ip_dst_loopback_pkt,
                                         self.ip_src_loopback_pkt,
                                         self.src_ip_class_e_pkt,
                                         self.lpm4_miss_pkt,
                                         self.lpm6_miss_pkt,
                                         self.blackhole_route_pkt,
                                         self.dst_ipv4_unspec_pkt,
                                         self.dst_ipv6_unspec_pkt]])
        self.map_drop_reasons.append(
            [SAI_IN_DROP_REASON_SIP_CLASS_E, [self.src_ip_class_e_pkt]])
        self.map_drop_reasons.append(
            [SAI_IN_DROP_REASON_SIP_UNSPECIFIED, [self.src_ipv4_unspec_pkt]])

        if (self.isInDropReasonSupported(
                [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC])):
            self.map_drop_reasons.append(
                [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC,
                 [self.smac_equals_dmac_pkt]])
            self.map_drop_reasons.append(
                [SAI_IN_DROP_REASON_L2_ANY,
                 [self.mc_smac_pkt, self.zero_smac_pkt,
                  self.vlan_discard_pkt]])
        else:
            self.map_drop_reasons.append(
                [SAI_IN_DROP_REASON_L2_ANY,
                 [self.mc_smac_pkt, self.vlan_discard_pkt]])

    def setDebugCounterDropReasons(self, dc_oid, in_drop_reasons):
        ''' Sets the drop reason list for a given DebugCounter oid.
        Args:
            dc_oid (sai_object_id_t): DebugCounter object id
            in_drop_reasons (list): drop reason list
        '''

        if not isinstance(in_drop_reasons, list):
            in_drop_reasons = [in_drop_reasons]

        in_drop_reason_list = sai_thrift_u32_list_t(
            count=len(in_drop_reasons), uint32list=in_drop_reasons)
        sai_thrift_set_debug_counter_attribute(
            self.client,
            dc_oid,
            in_drop_reason_list=in_drop_reason_list)

    def createDebugCounter(self, dc_type, drop_reasons):
        ''' Creates DebugCounter for a given dc_type with drop_reason list
        Args:
            dc_type (sai_debug_counter_type_t): debug counter type.
            drop_reasons (list): drop reason list

        Returns:
            dc_oid(sai_object_id_t): debug counter object id
        '''

        if not isinstance(drop_reasons, list):
            drop_reasons = [drop_reasons]

        dc_oid = sai_thrift_create_debug_counter(
            self.client,
            type=dc_type,
            bind_method=None,
            in_drop_reason_list=sai_thrift_s32_list_t(
                count=len(drop_reasons),
                int32list=drop_reasons),
            out_drop_reason_list=None)
        if dc_oid:
            self.assertTrue(self.verifyDebugCounterDropList(
                dc_oid, drop_reasons), "Failed to verify debug counter")

        return dc_oid

    def createRouter(self):
        ''' Creates a router '''

        self.rif_ids.append(sai_thrift_create_router_interface(
            self.client,
            virtual_router_id=self.default_vrf,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port0,
            admin_v4_state=self.v4_enabled,
            admin_v6_state=self.v6_enabled))
        self.rif_ids.append(sai_thrift_create_router_interface(
            self.client,
            virtual_router_id=self.default_vrf,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port1,
            admin_v4_state=self.v4_enabled,
            admin_v6_state=self.v6_enabled))

    def setRifAttribute(self, rif_id, attr_id, attr_value):
        ''' Sets rif attribure
        Args:
            rif_id (sai_object_id_t): rif object id
            attr_id (sai_attribute_t): attribute id
            attr_value (sai_attribute_value_t): attribute value
        '''

        rif_attribute_value = None
        if attr_id == SAI_ROUTER_INTERFACE_ATTR_MTU:
            rif_attribute_value = sai_thrift_attribute_value_t(u32=attr_value)
        elif attr_id == SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE:
            rif_attribute_value = sai_thrift_attribute_value_t(
                booldata=attr_value)
        elif attr_id == SAI_ROUTER_INTERFACE_ATTR_LOOPBACK_PACKET_ACTION:
            rif_attribute_value = sai_thrift_attribute_value_t(s32=attr_value)
        else:
            return

        rif_attribute = sai_thrift_attribute_t(id=attr_id,
                                               value=rif_attribute_value)
        self.client.sai_thrift_set_router_interface_attribute(
            rif_id, rif_attribute)

    def createRoute(self, packet_action=None, nbr_ip_pfx=neighbor_ip_prefix):
        ''' Creates a single route
        Args:
            packet_action (enum): route packer action
            nbr_ip_pfx (str): neighbor IP prefix
        '''
        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(nbr_ip_pfx))
        status = sai_thrift_create_route_entry(
            self.client, route_entry, packet_action=packet_action)
        self.assertEqual(status, SAI_STATUS_SUCCESS, "Failed to create route")
        self.routes.append(route_entry)

    def createRouteV6(self):
        ''' Creates a single IPv6 route. '''

        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.neighbor_ipv6_prefix))
        status = sai_thrift_create_route_entry(self.client, route_entry)
        self.assertEqual(status, SAI_STATUS_SUCCESS, "Failed to create route")
        self.routes.append(route_entry)

    def createNeighbor(self, nbr_ip=neighbor_ip):
        ''' Creates a neighbor entry.
        Args:
            nbr_ip (str): neighbor IP address
        '''

        neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.rif_ids[-1],
            ip_address=sai_ipaddress(nbr_ip))
        self.neighbors.append(neighbor_entry1)
        sai_thrift_create_neighbor_entry(
            self.client,
            neighbor_entry1,
            dst_mac_address=self.neighbor_mac)

    def createNeighborV6(self):
        ''' Creates an IPv6 neighbor entry. '''
        neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.rif_ids[-1],
            ip_address=sai_ipaddress(self.neighbor_ipv6))

        self.neighbors.append(neighbor_entry1)
        sai_thrift_create_neighbor_entry(
            self.client,
            neighbor_entry1,
            dst_mac_address=self.neighbor_mac_v6)

    def cleanup(self):
        ''' Cleanups the class parameters. '''

        self.ingress_port = 0
        self.egress_port = 1
        del self.fdbs[:]
        del self.vr_ids[:]
        del self.rif_ids[:]
        del self.neighbors[:]
        del self.routes[:]
        self.dc_oid = None
        self.second_dc_oid = None

    def isInDropReasonSupported(self, drop_reason_list):
        ''' Verifies if drop reasons from the list are supported.
        Args:
            drop_reason_list (list): list of in drop reasons

        Returns:
            boolean: True if supported
        '''

        # get the supported IN drop reason capabilities
        caps_list = sai_thrift_query_attribute_enum_values_capability(
            self.client,
            SAI_OBJECT_TYPE_DEBUG_COUNTER,
            SAI_DEBUG_COUNTER_ATTR_IN_DROP_REASON_LIST)
        for drop_reason in drop_reason_list:
            found = False
            for in_drop_reason in caps_list:
                if drop_reason == in_drop_reason:
                    found = True
            if found is False:
                return False

        return True

    def tearDown(self):
        if self.dc_oid is not None:
            sai_thrift_remove_debug_counter(self.client, self.dc_oid)
        if self.second_dc_oid is not None:
            sai_thrift_remove_debug_counter(self.client, self.second_dc_oid)

        for route_data in self.routes:
            sai_thrift_remove_route_entry(self.client, route_data)

        for neighbor_data in self.neighbors:
            sai_thrift_remove_neighbor_entry(self.client, neighbor_data)

        for rif in self.rif_ids:
            sai_thrift_remove_router_interface(self.client, rif)

        for vr_id in self.vr_ids:
            self.client.sai_thrift_remove_virtual_router(vr_id)

        self.cleanup()

        super(BaseDebugCounterClass, self).tearDown()

    def testPortDebugCounter(self,
                             port,
                             dc_oid,
                             in_drop_reasons,
                             pkts,
                             drop_expected=True):
        ''' Executes port debug counter test.
        Args:
            port (sai_object_id_t): port object id
            dc_oid (sai_object_id_t): debugc counter object id
            in_drop_reasons (list): in drop reasons list
            pkts (list): list of test  packets
            drop_expected (bool): if test drops expected
        '''

        if not isinstance(pkts, list):
            pkts = [pkts]
        if not isinstance(dc_oid, list):
            dc_oid = [dc_oid]

        print("Test port debug counter for drop_reasons=%s"
              % (map_drop_reason_to_string(in_drop_reasons)))
        dc_counter_before = self.getDebugDounterPortStatsByOid(port, dc_oid)
        stats = get_port_stats(port, SAI_PORT_STAT_IF_IN_DISCARDS)
        port_drop_counter_before = stats[SAI_PORT_STAT_IF_IN_DISCARDS]
        packets_requested = len(pkts)
        expected_drops = 0
        print("Request %d packet(s)" % (len(pkts)))
        for pkt in pkts:
            send_packet(self, self.ingress_port, pkt)
            if drop_expected:
                verify_no_packet(self, pkt, self.egress_port)

        if drop_expected:
            expected_drops = packets_requested

        dc_counter_after = self.getDebugDounterPortStatsByOid(port, dc_oid)
        stats = get_port_stats(port, SAI_PORT_STAT_IF_IN_DISCARDS)
        port_drop_counter_after = stats[SAI_PORT_STAT_IF_IN_DISCARDS]
        print("Debug Counter before test=%d, DC counter after test=%d "
              "packets requested=%d expected_drops =%d"
              % (dc_counter_before, dc_counter_after, packets_requested,
                 expected_drops))
        print("Port drop counter before=%d, Port drop counter after=%d, "
              "packets requested=%d"
              % (port_drop_counter_before, port_drop_counter_after,
                 packets_requested))

        self.assertEqual(dc_counter_after,
                         dc_counter_before + expected_drops)
        if drop_expected:
            self.assertEqual(port_drop_counter_after,
                             port_drop_counter_before + expected_drops)
        print("\tok")

    def testSwitchDebugCounter(self,
                               dc_oid,
                               in_drop_reasons,
                               pkts,
                               drop_expected=True):
        ''' Executes switch debug counter test.
        Args:
            dc_oid (sai_object_id_t): debugc counter object id
            in_drop_reasons (list): in drop reasons list
            pkts (list): list of test  packets
            drop_expected (bool): if test drops expected
        '''

        if not isinstance(pkts, list):
            pkts = [pkts]
        if not isinstance(dc_oid, list):
            dc_oid = [dc_oid]

        print("Test switch debug counter for drop_reasons=%s"
              % (map_drop_reason_to_string(in_drop_reasons)))
        dc_counter_before = self.getDebugCounterSwitchStatsByOid(dc_oid)
        print("dc counter_before=%d" % dc_counter_before)

        stats = get_switch_stats(SAI_PORT_STAT_IF_IN_DISCARDS)
        switch_counter_before = stats[SAI_PORT_STAT_IF_IN_DISCARDS]

        packets_requested = 0
        expected_drops = 0
        try:
            for test_port in self.test_dev_ports:
                for pkt in pkts:
                    packets_requested += 1
                    send_packet(self, test_port, pkt)
                    if drop_expected:
                        verify_no_packet(self, pkt, self.egress_port)
            if drop_expected:
                expected_drops = packets_requested
        finally:
            dc_counter_after = self.getDebugCounterSwitchStatsByOid(dc_oid)
            stats = get_switch_stats(SAI_PORT_STAT_IF_IN_DISCARDS)
            switch_counter_after = stats[SAI_PORT_STAT_IF_IN_DISCARDS]
            print("Debug Counter before test=%d, DC counter after test=%d "
                  "packets requested=%d, drop_expected =%d"
                  % (dc_counter_before, dc_counter_after, packets_requested,
                     drop_expected))
            print("Switch drop counter before=%d, "
                  "Switch drop counter after=%d, packets requested=%d"
                  % (switch_counter_before, switch_counter_after,
                     packets_requested))
            self.assertEqual(dc_counter_after,
                             dc_counter_before + expected_drops)
            if drop_expected:
                self.assertEqual(switch_counter_after,
                                 switch_counter_before + expected_drops)

    def verifyPortDebugCounterDropPackets(
            self, port, dc_oid, drop_reason_list):
        ''' Verifies the port debug counters packets drops
            for a given drop reason list.

        Args:
            port (sai_object_id_t): port object id
            dc_oid (sai_object_id_t): debugc counter object id
            drop_reason_list (list): in drop reasons list

        Returns:
            boolean: if succesfully verified
        '''

        dc_pkt_cnt = 0
        found_pkt = False
        print("Verify drop reasons = %s" %
              (map_drop_reason_to_string(drop_reason_list)))
        counter = self.getDebugDounterPortStatsByOid(
            port, dc_oid)
        for dc_drop_reason in drop_reason_list:
            pkts = []
            for map_drop_reason, list_of_packets in self.map_drop_reasons:
                if map_drop_reason == dc_drop_reason:
                    # found corresponding test packet(s)
                    found_pkt = True
                    pkts = list_of_packets
                    break
            if found_pkt:
                dc_pkt_cnt += len(pkts)
                print("Sending %d packets for %s" %
                      (len(pkts), map_drop_reason_to_string(dc_drop_reason)))
                for pkt in pkts:
                    send_packet(self, self.ingress_port, pkt)
                    verify_no_packet(self, pkt, self.egress_port)
            else:
                # skip check as there is no packet sent
                continue
        counter_after = self.getDebugDounterPortStatsByOid(
            port, dc_oid)
        print("counter=%d, counter_after=%d, dc_pkt_cnt=%d" %
              (counter, counter_after, dc_pkt_cnt))
        if (counter + dc_pkt_cnt) != counter_after:
            return False

        return True

    def verifySwitchDebugCounterDropPackets(self, dc_oid, drop_reason_list):
        ''' Verifies the switch debug counters packets drops
            for a given drop reason list.

        Args:
            dc_oid (sai_object_id_t): debugc counter object id
            drop_reason_list (list): in drop reasons list

        Returns:
            boolean: if succesfully verified
        '''

        dc_pkt_cnt = 0
        found_pkt = False
        print("Verify drop reasons = %s" %
              (map_drop_reason_to_string(drop_reason_list)))
        counter = self.getDebugCounterSwitchStatsByOid(dc_oid)
        for dc_drop_reason in drop_reason_list:
            pkts = []
            for map_drop_reason, list_of_packets in self.map_drop_reasons:
                if map_drop_reason == dc_drop_reason:
                    # Found corresponding test packet(s)
                    found_pkt = True
                    pkts = list_of_packets
                    break
            if found_pkt:
                print("Sending %d packets for %s" %
                      (len(pkts), map_drop_reason_to_string(dc_drop_reason)))
                for port in self.test_dev_ports:
                    dc_pkt_cnt += len(pkts)
                    for pkt in pkts:
                        send_packet(self, port, pkt)
                        verify_no_packet(self, pkt, self.egress_port)
            else:
                # Skip check as there is no packet sent
                continue
        counter_after = self.getDebugCounterSwitchStatsByOid(dc_oid)
        print("counter=%d, counter_after=%d, dc_pkt_cnt=%d" %
              (counter, counter_after, dc_pkt_cnt))
        if (counter + dc_pkt_cnt) != counter_after:
            return False

        return True

    def verifyDebugCounterDropList(self, dc_oid, drop_reasons):
        ''' Verifies the debug counters drop list for given
            debug counter.

        Args:
            dc_oid (sai_object_id_t): debugc counter object id
            drop_reasons (list): in drop reasons list

        Returns:
            boolean: if successfully verified
        '''

        attr = sai_thrift_get_debug_counter_attribute(
            self.client, dc_oid,
            in_drop_reason_list=sai_thrift_u32_list_t(count=10, uint32list=[]))

        if len(drop_reasons) != attr['in_drop_reason_list'].count:
            print(
                "SAI_DEBUG_COUNTER_ATTR_IN_DROP_REASON_LIST incorrect count=%d"
                % attr['in_drop_reason_list'].count)
            return False

        # Verify if IN list is correct
        found = False
        for drop_reason in drop_reasons:
            found = False
            for i in range(0, attr['in_drop_reason_list'].count):
                if attr['in_drop_reason_list'].int32list[i] == drop_reason:
                    found = True
            if found is not True:
                return False
        return True

    def getDebugCounterSwitchStatsByOid(self, dc_oid):
        ''' Returns the switch stats for given DebugCounter.

        Args:
            dc_oid (sai_object_id_t): debugc counter object id

        Returns:
            uint: debug counter stats value.
        '''

        if not isinstance(dc_oid, list):
            dc_oid = [dc_oid]
        counters = []
        for debug_counter in dc_oid:
            attr = sai_thrift_get_debug_counter_attribute(
                self.client, debug_counter, index=True, type=True)
            dc_index = attr['index'] + \
                debug_counter_type_to_index_base(attr['type'])
            counters.append(dc_index)
        stats = sai_thrift_get_debug_counter_switch_stats(counters)

        return stats[dc_index]

    def getDebugDounterPortStatsByOid(self, port, dc_oid):
        ''' Returns the port stats for given DebugCounter.

        Args:
            port (sai_object_id_t): port object id
            dc_oid (sai_object_id_t): debugc counter object id

        Returns:
            uint: debug counter stats value.
        '''

        if not isinstance(dc_oid, list):
            dc_oid = [dc_oid]
        counters = []
        for debug_counter in dc_oid:
            attr = sai_thrift_get_debug_counter_attribute(
                self.client, debug_counter, index=True, type=True)
            dc_index = attr['index'] + \
                debug_counter_type_to_index_base(attr['type'])
            counters.append(dc_index)

        stats = sai_thrift_get_debug_counter_port_stats(port, counters)
        # Returns the stats for the first DebugCounter in the list
        return stats[dc_index]


@group("draft")
class PortDebugCounterRemoveDropReason(BaseDebugCounterClass):
    ''' Port debug counter - remove drop reasons test. '''

    def runTest(self):

        port_dc_oid = 0
        try:
            drop_reason_cap = [
                SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER,
                SAI_IN_DROP_REASON_L2_ANY,
                SAI_IN_DROP_REASON_L3_ANY,
                SAI_IN_DROP_REASON_SMAC_MULTICAST,
                SAI_IN_DROP_REASON_TTL,
                SAI_IN_DROP_REASON_IP_HEADER_ERROR,
                SAI_IN_DROP_REASON_SIP_MC]

            if (self.isInDropReasonSupported(
                    [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC])):
                drop_reason_cap.append(SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC)

            drop_reason_list = [SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER]
            port_dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,
                drop_reason_list)

            self.assertTrue(port_dc_oid != 0, "Failed to Create DebugCounter")

            self.setDebugCounterDropReasons(port_dc_oid, [])
            self.assertTrue(self.verifyDebugCounterDropList(
                port_dc_oid, []))

            drop_reason_list = drop_reason_cap
            self.setDebugCounterDropReasons(port_dc_oid, drop_reason_list)
            self.assertTrue(self.verifyDebugCounterDropList(
                port_dc_oid, drop_reason_list))

            for i in range(0, len(drop_reason_cap)):
                if i == 0:
                    print("Verify initial drop_reason_list with traffic: %s" %
                          (map_drop_reason_to_string(drop_reason_list)))
                    self.assertTrue(self.verifyPortDebugCounterDropPackets(
                        self.port0, port_dc_oid, drop_reason_list))

                # Remove drop reason
                drop_reason_list.pop(0)
                print("Setting DebugCounter drop_reason_list to: %s" %
                      (map_drop_reason_to_string(drop_reason_list)))
                self.setDebugCounterDropReasons(
                    port_dc_oid, drop_reason_list)

                print("Verify updated drop_reason_list")
                self.assertTrue(self.verifyDebugCounterDropList(
                    port_dc_oid, drop_reason_list))
                print("\tok")

                print("Verify updated drop_reason_list with traffic")
                # Verify packets after removing the drop_reason
                self.assertTrue(self.verifyPortDebugCounterDropPackets(
                    self.port0, port_dc_oid, drop_reason_list))
                print("\tok")
        finally:
            if port_dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, port_dc_oid)


@group("draft")
class SwitchDebugCounterRemoveDropReason(BaseDebugCounterClass):
    ''' Switch Debug Counter Remove drop reasons test. '''

    def runTest(self):

        dc_oid = 0
        try:
            drop_reason_cap = [
                SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER,
                SAI_IN_DROP_REASON_L2_ANY,
                SAI_IN_DROP_REASON_L3_ANY,
                SAI_IN_DROP_REASON_SMAC_MULTICAST,
                SAI_IN_DROP_REASON_TTL,
                SAI_IN_DROP_REASON_IP_HEADER_ERROR,
                SAI_IN_DROP_REASON_SIP_MC]

            if (self.isInDropReasonSupported(
                    [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC])):
                drop_reason_cap.append(SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC)

            drop_reason_list = [SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,
                drop_reason_list)

            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.setDebugCounterDropReasons(dc_oid, [])
            self.assertTrue(self.verifyDebugCounterDropList(
                dc_oid, []))

            drop_reason_list = drop_reason_cap
            self.setDebugCounterDropReasons(dc_oid, drop_reason_list)
            self.assertTrue(self.verifyDebugCounterDropList(
                dc_oid, drop_reason_list))

            for i in range(0, len(drop_reason_cap)):
                if i == 0:
                    print(
                        "Verify initial drop_reason_list with traffic: %s" %
                        (map_drop_reason_to_string(drop_reason_list)))
                    self.assertTrue(self.verifySwitchDebugCounterDropPackets(
                        dc_oid, drop_reason_list))

                # Remove drop reason
                drop_reason_list.pop(0)
                print("Setting DebugCounter drop_reason_list to: %s" %
                      (map_drop_reason_to_string(drop_reason_list)))
                self.setDebugCounterDropReasons(dc_oid, drop_reason_list)

                print("Verify updated drop_reason_list")
                self.assertTrue(self.verifyDebugCounterDropList(
                    dc_oid, drop_reason_list))
                print("\tok")

                print("Verify updated drop_reason_list with traffic")
                # Verify packets after removing the drop_reason
                self.assertTrue(self.verifySwitchDebugCounterDropPackets(
                    dc_oid, drop_reason_list))
                print("\tok")
        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDebugCounterAddDropReason(BaseDebugCounterClass):
    ''' Port Debug Counter add drop reasons test. '''

    def runTest(self):

        port_dc_oid = 0
        try:
            port_dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,
                [SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER])
            self.assertTrue(port_dc_oid != 0, "Failed to Create DebugCounter")

            drop_reason_cap = [
                SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER,
                SAI_IN_DROP_REASON_L2_ANY,
                SAI_IN_DROP_REASON_L3_ANY,
                SAI_IN_DROP_REASON_SMAC_MULTICAST,
                SAI_IN_DROP_REASON_TTL,
                SAI_IN_DROP_REASON_IP_HEADER_ERROR,
                SAI_IN_DROP_REASON_SIP_MC]
            if (self.isInDropReasonSupported(
                    [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC])):
                drop_reason_cap.append(SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC)

            drop_reason_list = []
            for drop_reason in drop_reason_cap:
                drop_reason_list.append(drop_reason)
                print("Setting DebugCounter drop_reason_list to: %s" %
                      (map_drop_reason_to_string(drop_reason_list)))
                self.setDebugCounterDropReasons(
                    port_dc_oid, drop_reason_list)
                print("\tok")
                print("Verify updated drop_reason_list")
                self.assertTrue(self.verifyDebugCounterDropList(
                    port_dc_oid, drop_reason_list))
                print("\tok")
                print("Verify updated drop_reason_list with traffic")
                self.assertTrue(self.verifyPortDebugCounterDropPackets(
                    self.port0,
                    port_dc_oid,
                    drop_reason_list))
                print("\tok")

        finally:
            if port_dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, port_dc_oid)


@group("draft")
class SwitchDebugCounterAddDropReason(BaseDebugCounterClass):
    ''' Switch Debug Counter add drop reasons test. '''

    def runTest(self):

        dc_oid = 0
        try:
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS,
                [SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER])
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            drop_reason_cap = [
                SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER,
                SAI_IN_DROP_REASON_L2_ANY,
                SAI_IN_DROP_REASON_L3_ANY,
                SAI_IN_DROP_REASON_SMAC_MULTICAST,
                SAI_IN_DROP_REASON_TTL,
                SAI_IN_DROP_REASON_IP_HEADER_ERROR,
                SAI_IN_DROP_REASON_SIP_MC,
                SAI_IN_DROP_REASON_SIP_CLASS_E]
            if (self.isInDropReasonSupported(
                    [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC])):
                drop_reason_cap.append(SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC)

            drop_reason_list = []
            for drop_reason in drop_reason_cap:
                drop_reason_list.append(drop_reason)
                print("Setting DebugCounter drop_reason_list to: %s" %
                      (map_drop_reason_to_string(drop_reason_list)))
                self.setDebugCounterDropReasons(dc_oid, drop_reason_list)
                print("\tok")
                print("Verify updated drop_reason_list")
                self.assertTrue(self.verifyDebugCounterDropList(
                    dc_oid, drop_reason_list))
                print("\tok")
                print("Verify updated drop_reason_list with traffic")
                self.assertTrue(self.verifySwitchDebugCounterDropPackets(
                    dc_oid,
                    drop_reason_list))
                print("\tok")

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropMCSMAC(BaseDebugCounterClass):
    ''' Port Debug Counter for SMAC Multicast test. '''

    def runTest(self):

        dc_oid = 0
        try:
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,
                [SAI_IN_DROP_REASON_SMAC_MULTICAST])
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testPortDebugCounter(
                self.port0,
                dc_oid,
                [SAI_IN_DROP_REASON_SMAC_MULTICAST],
                [self.mc_smac_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)



class SwitchDropMCSMAC(BaseDebugCounterClass):
    ''' Switch Debug Counter for SMAC Multicast test. '''

    def runTest(self):

        dc_oid = 0
        try:
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS,
                [SAI_IN_DROP_REASON_SMAC_MULTICAST])
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testSwitchDebugCounter(
                dc_oid,
                [SAI_IN_DROP_REASON_SMAC_MULTICAST],
                [self.mc_smac_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropL2Any(BaseDebugCounterClass):
    ''' Port Debug Counter for L2 any drop reason test. '''

    def runTest(self):

        pkt_list = [self.zero_smac_pkt, self.vlan_discard_pkt]
        if (self.isInDropReasonSupported(
                [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC])):
            pkt_list.append(self.mc_smac_pkt)
        dc_oid = 0
        try:
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,
                [SAI_IN_DROP_REASON_L2_ANY])
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testPortDebugCounter(
                self.port0, dc_oid,
                [SAI_IN_DROP_REASON_L2_ANY],
                pkt_list)

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class SwitchDropL2Any(BaseDebugCounterClass):
    ''' Switch Debug Counter for L2 any drop reason test. '''

    def runTest(self):

        drop_list = [SAI_IN_DROP_REASON_L2_ANY]
        pkt_list = [self.zero_smac_pkt, self.vlan_discard_pkt]
        if (self.isInDropReasonSupported(
                [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC])):
            pkt_list.append(self.mc_smac_pkt)
        dc_oid = 0

        try:

            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS,
                drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testSwitchDebugCounter(dc_oid, drop_list, pkt_list)

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropSMACequalsDMAC(BaseDebugCounterClass):
    ''' Port Debug Counter for SMAC equals DMAC drop reason test. '''

    def runTest(self):
        dc_oid = 0
        try:
            drop_list = [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC]
            if (self.isInDropReasonSupported(
                    [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC])):
                dc_oid = self.createDebugCounter(
                    SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS, drop_list)
                self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

                self.testPortDebugCounter(
                    self.port0, dc_oid,
                    [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC],
                    [self.smac_equals_dmac_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class SwitchDropSMACequalsDMAC(BaseDebugCounterClass):
    ''' Switch Debug Counter for SMAC equals DMAC drop reason test. '''

    def runTest(self):
        dc_oid = 0
        try:
            drop_list = [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC]
            if (self.isInDropReasonSupported(
                    [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC])):
                dc_oid = self.createDebugCounter(
                    SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS, drop_list)
                self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

                self.testSwitchDebugCounter(
                    dc_oid,
                    drop_list,
                    [self.smac_equals_dmac_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropIngressVLANFilter(BaseDebugCounterClass):
    ''' Port Debug Counter for Ingress VLAN filter drop reason test. '''

    def runTest(self):
        dc_oid = 0
        try:
            drop_list = [SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testPortDebugCounter(
                self.port0, dc_oid,
                [SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER],
                [self.vlan_discard_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class SwitchDropIngressVLANFilter(BaseDebugCounterClass):
    ''' Switch Debug Counter for Ingress VLAN filter drop reason test. '''

    def runTest(self):
        dc_oid = 0
        try:
            drop_list = [SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testSwitchDebugCounter(
                dc_oid,
                drop_list,
                [self.vlan_discard_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropSIPMCTest(BaseDebugCounterClass):
    ''' Port Debug Counter for source IP Multicast drop reason test. '''

    def runTest(self):
        dc_oid = 0
        self.createRouter()
        try:
            drop_list = [SAI_IN_DROP_REASON_SIP_MC]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testPortDebugCounter(
                self.port0, dc_oid,
                drop_list,
                [self.src_ip_mc_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class SwitchDropSIPMCTest(BaseDebugCounterClass):
    ''' Switch Debug Counter for source IP Multicast drop reason test. '''

    def runTest(self):
        dc_oid = 0
        self.createRouter()
        try:
            drop_list = [SAI_IN_DROP_REASON_SIP_MC]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testSwitchDebugCounter(
                dc_oid,
                drop_list,
                [self.src_ip_mc_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropReasonTTLTest(BaseDebugCounterClass):
    ''' Port Debug Counter for TTL zero  drop reason test. '''

    def runTest(self):
        dc_oid = 0
        self.createRouter()
        try:
            drop_list = [SAI_IN_DROP_REASON_TTL]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testPortDebugCounter(
                self.port0, dc_oid,
                drop_list,
                [self.ttl_zero_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class SwitchDropReasonTTLTest(BaseDebugCounterClass):
    ''' Switch Debug Counter for TTL zero drop reason test. '''

    def runTest(self):
        dc_oid = 0
        self.createRouter()
        try:
            drop_list = [SAI_IN_DROP_REASON_TTL]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testSwitchDebugCounter(
                dc_oid,
                drop_list,
                [self.ttl_zero_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropSIPClassETest(BaseDebugCounterClass):
    ''' Port Debug Counter for SIP Class E drop reason test. '''

    def runTest(self):
        dc_oid = 0

        for test_port in self.test_ports:
            self.rif_ids.append(sai_thrift_create_router_interface(
                self.client,
                virtual_router_id=self.default_vrf,
                type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                port_id=test_port,
                admin_v4_state=self.v4_enabled,
                admin_v6_state=self.v6_enabled))

        try:
            drop_list = [SAI_IN_DROP_REASON_SIP_CLASS_E]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testPortDebugCounter(
                self.port0, dc_oid,
                drop_list,
                [self.src_ip_class_e_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class SwitchDropSIPClassETest(BaseDebugCounterClass):
    ''' Switch Debug Counter for SIP Class E drop reason test. '''

    def runTest(self):
        dc_oid = 0

        for test_port in self.test_ports:
            self.rif_ids.append(sai_thrift_create_router_interface(
                self.client,
                virtual_router_id=self.default_vrf,
                type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                port_id=test_port,
                admin_v4_state=self.v4_enabled,
                admin_v6_state=self.v6_enabled))

        try:
            drop_list = [SAI_IN_DROP_REASON_SIP_CLASS_E]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testSwitchDebugCounter(
                dc_oid,
                drop_list,
                [self.src_ip_class_e_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropUCDIPMCDMACTest(BaseDebugCounterClass):
    ''' Port Debug Counter for UC DIP with MC DMAC drop reason test. '''

    def runTest(self):
        dc_oid = 0

        self.createRouter()
        self.createNeighbor()
        self.createRoute()

        try:
            drop_list = [SAI_IN_DROP_REASON_UC_DIP_MC_DMAC]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            print("\nTest IPv4")
            self.testPortDebugCounter(
                self.port0, dc_oid,
                drop_list,
                [self.uc_dipv4_mc_dmac_pkt])

            print("Test IPv6")
            self.testPortDebugCounter(
                self.port0, dc_oid,
                drop_list,
                [self.uc_dipv6_mc_dmac_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropReasonIPHeaderErrorTest(BaseDebugCounterClass):
    ''' Port Debug Counter for IP header error drop reason test. '''

    def runTest(self):

        dc_oid = 0
        self.createRouter()
        try:
            drop_list = [SAI_IN_DROP_REASON_IP_HEADER_ERROR]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testPortDebugCounter(
                self.port0, dc_oid,
                drop_list,
                [self.ihl_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class SwitchDropReasonIPHeaderErrorTest(BaseDebugCounterClass):
    ''' Switch Debug Counter for IP header error drop reason test. '''

    def runTest(self):

        dc_oid = 0
        self.createRouter()
        try:
            drop_list = [SAI_IN_DROP_REASON_IP_HEADER_ERROR]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testSwitchDebugCounter(
                dc_oid,
                drop_list,
                [self.ihl_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortMultiDebugCounters(BaseDebugCounterClass):
    ''' Port multi Debug Counter test. '''

    def runTest(self):
        dc_oid1 = 0
        dc_oid2 = 0
        dc_oid3 = 0
        try:
            drop_list1 = [SAI_IN_DROP_REASON_IP_HEADER_ERROR,
                          SAI_IN_DROP_REASON_L2_ANY]
            dc_oid1 = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,
                drop_list1)
            self.assertTrue(dc_oid1 != 0, "Failed to Create DebugCounter")
            self.assertTrue(self.verifyDebugCounterDropList(
                dc_oid1, drop_list1))

            drop_list2 = [SAI_IN_DROP_REASON_L2_ANY]
            dc_oid2 = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,
                drop_list2)
            self.assertTrue(dc_oid2 != 0, "Failed to Create DebugCounter")
            self.assertTrue(self.verifyDebugCounterDropList(
                dc_oid2,
                drop_list2))

            if (self.isInDropReasonSupported(
                    [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC])):
                drop_list3 = [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC]
                dc_oid3 = self.createDebugCounter(
                    SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,
                    drop_list3)
                self.assertTrue(dc_oid3 != 0, "Failed to Create DebugCounter")
                self.assertTrue(self.verifyDebugCounterDropList(
                    dc_oid3,
                    drop_list3))

                self.testPortDebugCounter(
                    self.port0, dc_oid3,
                    drop_list3,
                    [self.smac_equals_dmac_pkt])

                self.testPortDebugCounter(
                    self.port0, dc_oid1, drop_list2, [self.zero_smac_pkt])
            else:
                self.testPortDebugCounter(
                    self.port0, dc_oid1, drop_list2,
                    [self.vlan_discard_pkt, self.zero_smac_pkt])
        finally:
            if dc_oid1 != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid1)
            if dc_oid2 != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid2)
            if dc_oid3 != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid3)


@group("draft")
class SwitchMultiDebugCounters(BaseDebugCounterClass):
    ''' Switch multi Debug Counter test. '''

    def runTest(self):
        dc_oid1 = 0
        dc_oid2 = 0
        dc_oid3 = 0
        try:
            drop_list1 = [SAI_IN_DROP_REASON_IP_HEADER_ERROR,
                          SAI_IN_DROP_REASON_L2_ANY]
            dc_oid1 = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS,
                drop_list1)
            self.assertTrue(dc_oid1 != 0, "Failed to Create DebugCounter")
            self.assertTrue(self.verifyDebugCounterDropList(
                dc_oid1, drop_list1))

            drop_list2 = [SAI_IN_DROP_REASON_L2_ANY]
            dc_oid2 = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS,
                drop_list2)
            self.assertTrue(dc_oid2 != 0, "Failed to Create DebugCounter")
            self.assertTrue(self.verifyDebugCounterDropList(
                dc_oid2,
                drop_list2))

            if (self.isInDropReasonSupported(
                    [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC])):
                drop_list3 = [SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC]
                dc_oid3 = self.createDebugCounter(
                    SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS,
                    drop_list3)
                self.assertTrue(dc_oid3 != 0, "Failed to Create DebugCounter")
                self.assertTrue(self.verifyDebugCounterDropList(
                    dc_oid3,
                    drop_list3))

                self.testSwitchDebugCounter(
                    dc_oid3,
                    drop_list3,
                    [self.smac_equals_dmac_pkt])
                self.testSwitchDebugCounter(
                    dc_oid1,
                    drop_list2,
                    [self.vlan_discard_pkt, self.zero_smac_pkt, self.ihl_pkt])
            else:
                self.testSwitchDebugCounter(
                    dc_oid1,
                    drop_list2,
                    [self.vlan_discard_pkt, self.zero_smac_pkt, self.ihl_pkt])
                self.testSwitchDebugCounter(
                    dc_oid1,
                    drop_list2,
                    [self.vlan_discard_pkt, self.zero_smac_pkt])
        finally:
            if dc_oid1 != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid1)
            if dc_oid2 != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid2)
            if dc_oid3 != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid3)


@group("draft")
class PortDropDIPLinkLocalTest(BaseDebugCounterClass):
    ''' Port Debug Counter for DIP Link Local drop reason test. '''

    def runTest(self):
        dc_oid = 0

        self.createRouter()
        self.createNeighbor(nbr_ip=self.link_local_ip)
        self.createRoute(nbr_ip_pfx=self.link_local_ip_prefix)

        try:
            drop_list = [SAI_IN_DROP_REASON_DIP_LINK_LOCAL]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testPortDebugCounter(
                self.port0, dc_oid,
                drop_list,
                [self.dst_ip_link_local_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropSIPLinkLocalTest(BaseDebugCounterClass):
    ''' Port Debug Counter for SIP Link Local drop reason test. '''

    def runTest(self):
        dc_oid = 0

        self.createRouter()
        self.createNeighbor()
        self.createRoute()

        try:
            drop_list = [SAI_IN_DROP_REASON_SIP_LINK_LOCAL]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testPortDebugCounter(
                self.port0, dc_oid,
                drop_list,
                [self.src_ip_link_local_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropSIPUnspecifiedTest(BaseDebugCounterClass):
    ''' Port Debug Counter for SIP Unspecified drop reason test. '''

    def runTest(self):
        dc_oid = 0

        self.createRouter()
        self.createNeighbor()
        self.createRoute()

        try:
            drop_list = [SAI_IN_DROP_REASON_SIP_UNSPECIFIED]
            dc_oid = self.createDebugCounter(
                SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testPortDebugCounter(
                self.port0, dc_oid,
                drop_list,
                [self.src_ipv4_unspec_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropIPv4RIFDisabled(BaseDebugCounterClass):
    ''' Port Debug Counter for IPv4 disabled rif drop reason test. '''

    def runTest(self):

        dc_oid = 0
        try:
            self.createRouter()
            self.createNeighbor()
            self.createRoute()

            drop_list = [SAI_IN_DROP_REASON_IRIF_DISABLED]
            dc_type = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS

            dc_oid = self.createDebugCounter(dc_type, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src=self.default_mac_1,
                ip_dst=self.neighbor_ip)

            # Interface enabled
            sai_thrift_set_router_interface_attribute(
                self.client,
                self.rif_ids[0],
                admin_v4_state=True)
            self.testPortDebugCounter(self.port0,
                                      dc_oid, drop_list,
                                      [pkt],
                                      drop_expected=False)

            # Interface disabled
            sai_thrift_set_router_interface_attribute(
                self.client,
                self.rif_ids[0],
                admin_v4_state=False)
            self.testPortDebugCounter(self.port0, dc_oid, drop_list, [pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropIPv4L3AnyRIFDisabled(BaseDebugCounterClass):
    ''' Port Debug Counter for IPv4 disabled rif and L3 any
        drop reasons test.
    '''

    def runTest(self):

        dc_oid = 0
        try:
            self.createRouter()
            self.createNeighbor()
            self.createRoute()

            drop_list = [SAI_IN_DROP_REASON_L3_ANY]
            dc_type = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS

            dc_oid = self.createDebugCounter(dc_type, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src=self.default_mac_1,
                ip_dst=self.neighbor_ip)

            # Interface enabled
            sai_thrift_set_router_interface_attribute(
                self.client,
                self.rif_ids[0],
                admin_v4_state=True)
            self.testPortDebugCounter(self.port0,
                                      dc_oid,
                                      drop_list,
                                      [pkt],
                                      drop_expected=False)

            # Interface disabled
            sai_thrift_set_router_interface_attribute(
                self.client,
                self.rif_ids[0],
                admin_v4_state=False)
            self.testPortDebugCounter(self.port0, dc_oid, drop_list, [pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropIPv6RIFDisabled(BaseDebugCounterClass):
    ''' Port Debug Counter for IPv6 disabled RIF drop reason test. '''

    def runTest(self):

        dc_oid = 0
        try:
            drop_list = [SAI_IN_DROP_REASON_IRIF_DISABLED]
            dc_type = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS

            self.createRouter()
            self.createNeighborV6()
            self.createRouteV6()
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      ipv6_dst=self.neighbor_ipv6)

            dc_oid = self.createDebugCounter(dc_type, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            # Verify traffic for rif interface enabled
            sai_thrift_set_router_interface_attribute(
                self.client,
                self.rif_ids[0],
                admin_v6_state=True)
            self.testPortDebugCounter(self.port0, dc_oid, drop_list, [pkt],
                                      drop_expected=False)

            # Verify traffic for rif interface disabled
            sai_thrift_set_router_interface_attribute(
                self.client,
                self.rif_ids[0],
                admin_v6_state=False)
            self.testPortDebugCounter(self.port0, dc_oid, drop_list, [pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropIPv6L3AnyRIFDisabled(BaseDebugCounterClass):
    ''' Port Debug Counter for IPv6 disabled rif and L3 any
        drop reasons test.
    '''

    def runTest(self):

        dc_oid = 0
        try:
            drop_list = [SAI_IN_DROP_REASON_L3_ANY]
            dc_type = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS

            self.createRouter()
            self.createNeighborV6()
            self.createRouteV6()

            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      ipv6_dst=self.neighbor_ipv6,
                                      ipv6_hlim=64)

            dc_oid = self.createDebugCounter(dc_type, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            # Verify traffic for rif interface enabled
            sai_thrift_set_router_interface_attribute(
                self.client,
                self.rif_ids[0],
                admin_v6_state=True)
            self.testPortDebugCounter(self.port0, dc_oid, drop_list, [pkt],
                                      drop_expected=False)

            # Verify traffic for rif interface disabled
            sai_thrift_set_router_interface_attribute(
                self.client,
                self.rif_ids[0],
                admin_v6_state=False)
            self.testPortDebugCounter(self.port0, dc_oid, drop_list, [pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class SwitchDropDIPLoopback(BaseDebugCounterClass):
    ''' Switch Debug Counter for DIP loopback drop reasons test.
    '''

    def runTest(self):

        dc_oid = 0

        for test_port in self.test_ports:
            self.rif_ids.append(sai_thrift_create_router_interface(
                self.client,
                virtual_router_id=self.default_vrf,
                type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                port_id=test_port,
                admin_v4_state=self.v4_enabled,
                admin_v6_state=self.v6_enabled))

        try:
            drop_list = [SAI_IN_DROP_REASON_DIP_LOOPBACK]
            dc_type = SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.default_mac_1,
                                    ip_dst=self.loopback_ip)

            dc_oid = self.createDebugCounter(dc_type, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            # Verify traffic for rif interface enabled
            self.testSwitchDebugCounter(dc_oid, drop_list, pkt)

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class SwitchDropSIPLoopback(BaseDebugCounterClass):
    ''' Switch Debug Counter for SIP loopback drop reasons test.
    '''

    def runTest(self):

        dc_oid = 0
        self.createRouter()
        try:
            drop_list = [SAI_IN_DROP_REASON_SIP_LOOPBACK]
            dc_type = SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.default_mac_1,
                                    ip_src=self.loopback_ip)

            dc_oid = self.createDebugCounter(dc_type, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            # Verify traffic for rif interface enabled
            self.testSwitchDebugCounter(dc_oid, drop_list, pkt)

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropIPv4Miss(BaseDebugCounterClass):
    ''' Port Debug Counter for IPv4 route miss drop reasons test. '''

    def runTest(self):

        dc_oid = 0
        self.createRouter()
        try:
            drop_list = [SAI_IN_DROP_REASON_LPM4_MISS]
            dc_type = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS

            dc_oid = self.createDebugCounter(dc_type, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            self.testPortDebugCounter(
                self.port0, dc_oid, drop_list, [self.lpm4_miss_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropIPv6Miss(BaseDebugCounterClass):
    ''' Port Debug Counter for IPv6 route miss drop reasons test. '''

    def runTest(self):

        dc_oid = 0
        self.createRouter()
        try:
            drop_list = [SAI_IN_DROP_REASON_LPM6_MISS]
            dc_type = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS

            dc_oid = self.createDebugCounter(dc_type, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")
            self.testPortDebugCounter(
                self.port0, dc_oid, drop_list, [self.lpm6_miss_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropBlackHoleRoute(BaseDebugCounterClass):
    ''' Port Debug Counter for blackhole route drop reasons test. '''

    def runTest(self):

        dc_oid = 0
        try:
            self.createRouter()
            self.createRoute(
                packet_action=SAI_PACKET_ACTION_DROP,
                nbr_ip_pfx=self.blackhole_ip)

            drop_list = [SAI_IN_DROP_REASON_BLACKHOLE_ROUTE]
            dc_type = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS

            dc_oid = self.createDebugCounter(dc_type, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")
            self.testPortDebugCounter(
                self.port0, dc_oid, drop_list, [self.blackhole_route_pkt])

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropL3AnyTest(BaseDebugCounterClass):
    ''' Port Debug Counter for L3 any drop reasons test. '''

    def runTest(self):

        dc_oid = 0
        try:
            self.createRouter()
            self.createNeighborV6()
            self.createRouteV6()
            self.createRoute(
                packet_action=SAI_PACKET_ACTION_DROP,
                nbr_ip_pfx=self.blackhole_ip)

            drop_list = [SAI_IN_DROP_REASON_L3_ANY]
            dc_type = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS

            dc_oid = self.createDebugCounter(dc_type, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            pkt_list = [self.ihl_pkt,
                        self.src_ip_mc_pkt,
                        self.ttl_zero_pkt,
                        self.ip_dst_loopback_pkt,
                        self.ip_src_loopback_pkt,
                        self.src_ip_class_e_pkt,
                        self.lpm4_miss_pkt,
                        self.lpm6_miss_pkt,
                        self.blackhole_route_pkt,
                        self.dst_ipv4_unspec_pkt,
                        self.dst_ipv6_unspec_pkt]

            self.testPortDebugCounter(self.port0, dc_oid, drop_list, pkt_list)

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)


@group("draft")
class PortDropAclAnyTest(BaseDebugCounterClass):
    ''' Port Debug Counter for ACL any drop reason test. '''

    def runTest(self):

        dc_oid = 0
        acl_list = []
        try:
            self.createRouter()
            self.createNeighborV6()
            self.createRouteV6()

            drop_list = [SAI_IN_DROP_REASON_ACL_ANY]
            dc_type = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS

            dc_oid = self.createDebugCounter(dc_type, drop_list)
            self.assertTrue(dc_oid != 0, "Failed to Create DebugCounter")

            acl_list = self.setupPortIngresDropAcl(dip=self.neighbor_ip)
            ingress_acl = acl_list[0]['acl_table']

            status = sai_thrift_set_port_attribute(
                self.client,
                self.port0,
                ingress_acl=ingress_acl)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src=self.default_mac_1,
                ip_dst=self.neighbor_ip)

            self.testPortDebugCounter(self.port0, dc_oid, drop_list, pkt)

        finally:
            if dc_oid != 0:
                sai_thrift_remove_debug_counter(self.client, dc_oid)

            sai_thrift_set_port_attribute(
                self.client,
                self.port0,
                ingress_acl=0)

            for acl in acl_list:
                sai_thrift_remove_acl_table_group_member(
                    self.client,
                    acl['acl_group_member'])
                sai_thrift_remove_acl_table_group(self.client,
                                                  acl['acl_table_group'])
                sai_thrift_remove_acl_entry(self.client, acl['acl_entry'])
                sai_thrift_remove_acl_table(self.client, acl['acl_table'])

    def setupPortIngresDropAcl(
            self,
            dmac=None,
            dip=None,
            action=SAI_PACKET_ACTION_DROP,
            mac_mask='FF:FF:FF:FF:FF:FF'):
        ''' Function creates the port ingress ACL.

            Args:
                dmac (mac): acl configured dst mac address
                dip (ip): acl configured dst ip address
                action (action): acl configured action
                mac_mask (mac): acl configured dst mac mask

            Returns:
                list: list of acl values
        '''

        acl = []
        try:
            stage = SAI_ACL_STAGE_INGRESS
            bind_points = [SAI_ACL_BIND_POINT_TYPE_PORT]
            action_types = [SAI_ACL_ACTION_TYPE_PACKET_ACTION]
            dip_mask = '255.255.255.0'

            acl_bind_point_type_list = sai_thrift_s32_list_t(
                count=len(bind_points), int32list=bind_points)
            acl_action_type_list = sai_thrift_s32_list_t(
                count=len(action_types), int32list=action_types)

            if dip is not None:
                dip_ind = True
                dst_ip = sai_thrift_acl_field_data_t(
                    data=sai_thrift_acl_field_data_data_t(ip4=dip),
                    mask=sai_thrift_acl_field_data_mask_t(ip4=dip_mask))
            else:
                dip_ind = None
                dst_ip = None
            if dmac is not None:
                dmac_ind = True
                dst_mac = sai_thrift_acl_field_data_t(
                    data=sai_thrift_acl_field_data_data_t(mac=dmac),
                    mask=sai_thrift_acl_field_data_mask_t(mac=mac_mask))
            else:
                dmac_ind = None
                dst_mac = None

            acl_table_id = sai_thrift_create_acl_table(
                self.client,
                acl_stage=stage,
                acl_bind_point_type_list=acl_bind_point_type_list,
                acl_action_type_list=acl_action_type_list,
                field_dst_ip=dip_ind,
                field_dst_mac=dmac_ind)
            self.assertTrue(acl_table_id != 0, "Failed to Create ACL table")

            action_drop = action
            packet_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(s32=action_drop))
            acl_entry = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table_id,
                action_packet_action=packet_action,
                field_dst_ip=dst_ip,
                field_dst_mac=dst_mac)
            self.assertTrue(acl_entry != 0, "Failed to Create ACL entry")

            acl_table_group = sai_thrift_create_acl_table_group(
                self.client,
                acl_stage=stage,
                acl_bind_point_type_list=acl_bind_point_type_list,
                type=SAI_ACL_TABLE_GROUP_TYPE_PARALLEL)
            acl_group_member = sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=acl_table_group,
                acl_table_id=acl_table_id)

        finally:
            acl.append({
                'acl_table': acl_table_id,
                'acl_entry': acl_entry,
                'acl_table_group': acl_table_group,
                'acl_group_member': acl_group_member})
        return acl


@group("draft")
class GetDebugCounterEnumValuesCapabilities(BaseDebugCounterClass):
    ''' SAI query DebugCounter enum values capabilities. '''

    def runTest(self):
        # Supported SAI IN drop reason list
        in_caps_list = [
            SAI_IN_DROP_REASON_L2_ANY,
            SAI_IN_DROP_REASON_SMAC_MULTICAST,
            SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC,
            SAI_IN_DROP_REASON_DMAC_RESERVED,
            SAI_IN_DROP_REASON_VLAN_TAG_NOT_ALLOWED,
            SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER,
            SAI_IN_DROP_REASON_INGRESS_STP_FILTER,
            SAI_IN_DROP_REASON_FDB_UC_DISCARD,
            SAI_IN_DROP_REASON_FDB_MC_DISCARD,
            SAI_IN_DROP_REASON_L2_LOOPBACK_FILTER,
            SAI_IN_DROP_REASON_EXCEEDS_L2_MTU,
            SAI_IN_DROP_REASON_L3_ANY,
            SAI_IN_DROP_REASON_EXCEEDS_L3_MTU,
            SAI_IN_DROP_REASON_TTL,
            SAI_IN_DROP_REASON_L3_LOOPBACK_FILTER,
            SAI_IN_DROP_REASON_NON_ROUTABLE,
            SAI_IN_DROP_REASON_NO_L3_HEADER,
            SAI_IN_DROP_REASON_IP_HEADER_ERROR,
            SAI_IN_DROP_REASON_UC_DIP_MC_DMAC,
            SAI_IN_DROP_REASON_DIP_LOOPBACK,
            SAI_IN_DROP_REASON_SIP_LOOPBACK,
            SAI_IN_DROP_REASON_SIP_MC,
            SAI_IN_DROP_REASON_SIP_CLASS_E,
            SAI_IN_DROP_REASON_SIP_UNSPECIFIED,
            SAI_IN_DROP_REASON_MC_DMAC_MISMATCH,
            SAI_IN_DROP_REASON_SIP_EQUALS_DIP,
            SAI_IN_DROP_REASON_SIP_BC,
            SAI_IN_DROP_REASON_DIP_LOCAL,
            SAI_IN_DROP_REASON_DIP_LINK_LOCAL,
            SAI_IN_DROP_REASON_SIP_LINK_LOCAL,
            SAI_IN_DROP_REASON_IPV6_MC_SCOPE0,
            SAI_IN_DROP_REASON_IPV6_MC_SCOPE1,
            SAI_IN_DROP_REASON_IRIF_DISABLED,
            SAI_IN_DROP_REASON_ERIF_DISABLED,
            SAI_IN_DROP_REASON_LPM4_MISS,
            SAI_IN_DROP_REASON_LPM6_MISS,
            SAI_IN_DROP_REASON_BLACKHOLE_ROUTE,
            SAI_IN_DROP_REASON_BLACKHOLE_ARP,
            SAI_IN_DROP_REASON_UNRESOLVED_NEXT_HOP,
            SAI_IN_DROP_REASON_L3_EGRESS_LINK_DOWN,
            SAI_IN_DROP_REASON_DECAP_ERROR,
            SAI_IN_DROP_REASON_ACL_ANY,
            SAI_IN_DROP_REASON_ACL_INGRESS_PORT,
            SAI_IN_DROP_REASON_ACL_INGRESS_LAG,
            SAI_IN_DROP_REASON_ACL_INGRESS_VLAN,
            SAI_IN_DROP_REASON_ACL_INGRESS_RIF,
            SAI_IN_DROP_REASON_ACL_INGRESS_SWITCH,
            SAI_IN_DROP_REASON_ACL_EGRESS_PORT,
            SAI_IN_DROP_REASON_ACL_EGRESS_LAG,
            SAI_IN_DROP_REASON_ACL_EGRESS_VLAN,
            SAI_IN_DROP_REASON_ACL_EGRESS_RIF,
            SAI_IN_DROP_REASON_ACL_EGRESS_SWITCH,
            SAI_IN_DROP_REASON_FDB_AND_BLACKHOLE_DISCARDS,
            SAI_IN_DROP_REASON_NON_ROUTABLE,
            SAI_IN_DROP_REASON_MPLS_MISS,
            SAI_IN_DROP_REASON_SRV6_LOCAL_SID_DROP]

        supp = self.client.sai_thrift_query_attribute_enum_values_capability(
            SAI_OBJECT_TYPE_DEBUG_COUNTER,
            SAI_DEBUG_COUNTER_ATTR_IN_DROP_REASON_LIST,
            len(in_caps_list))
        print("\nSupported IN drop reasons")
        for drop_reason in in_caps_list:
            for in_drop_reason in supp:
                if drop_reason == in_drop_reason:
                    print(map_drop_reason_to_string([drop_reason]))
        print("\tok")

        # supported SAI OUT drop reason list
        out_caps_list = [
            SAI_OUT_DROP_REASON_L2_ANY,
            SAI_OUT_DROP_REASON_EGRESS_VLAN_FILTER,
            SAI_OUT_DROP_REASON_L3_ANY,
            SAI_OUT_DROP_REASON_L3_EGRESS_LINK_DOWN,
            SAI_OUT_DROP_REASON_TUNNEL_LOOPBACK_PACKET_DROP]

        supp = self.client.sai_thrift_query_attribute_enum_values_capability(
            SAI_OBJECT_TYPE_DEBUG_COUNTER,
            SAI_DEBUG_COUNTER_ATTR_OUT_DROP_REASON_LIST,
            len(out_caps_list))
        print("\nSupported OUT drop reasons")
        for drop_reason in out_caps_list:
            for out_drop_reason in supp:
                if drop_reason == out_drop_reason:
                    print(map_drop_reason_to_string([drop_reason]))
        print("\tok")


@group("draft")
class GetDebugCounterAvailability(BaseDebugCounterClass):
    ''' SAI query DebugCounter availability. '''

    def runTest(self):

        # Supported debug counter types
        dc_types = [SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,
                    SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS,
                    SAI_DEBUG_COUNTER_TYPE_PORT_OUT_DROP_REASONS,
                    SAI_DEBUG_COUNTER_TYPE_SWITCH_OUT_DROP_REASONS]
        # Verify supported debug counter types
        for dc_type in dc_types:
            dc_avail = sai_thrift_object_type_get_availability(
                self.client,
                SAI_OBJECT_TYPE_DEBUG_COUNTER,
                SAI_DEBUG_COUNTER_ATTR_TYPE, dc_type)

            print("verify DC Type = %s, dc_availability=%d"
                  % (dc_type, dc_avail))
            self.assertEqual(dc_avail, 0x400)
            print("\tok")

        # Verify unknown(random) debug counter types, should return value of 0
        for dc_type in [123, 777, 0x400]:
            dc_avail = sai_thrift_object_type_get_availability(
                self.client,
                SAI_OBJECT_TYPE_DEBUG_COUNTER,
                SAI_DEBUG_COUNTER_ATTR_TYPE,
                dc_type)
            print("verify unsupported DC Type = %s, dc_availability=%d"
                  % (dc_type, dc_avail))
            self.assertEqual(dc_avail, 0)
            print("\tok")
