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
Thrift SAI interface MPLS tests
"""

import random

from sai_thrift.sai_headers import *

from sai_base_test import *


@group("draft")
class MplsCpuTrapTest(SaiHelper):
    ''' Basic MPLS trap test class '''
    def setUp(self):
        super(MplsCpuTrapTest, self).setUp()

        status = sai_thrift_clear_port_stats(self.client, self.port10)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        status = sai_thrift_clear_router_interface_stats(
            self.client, self.port10_rif)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        stats = sai_thrift_get_port_stats(self.client, self.port10)
        self.port10_disc_stats = stats['SAI_PORT_STAT_IF_IN_DISCARDS']
        self.assertEqual(self.port10_disc_stats, 0)

        stats = sai_thrift_get_router_interface_stats(
            self.client, self.port10_rif)
        self.port10_rif_in_stats = \
            stats['SAI_ROUTER_INTERFACE_STAT_IN_PACKETS']
        self.assertEqual(self.port10_rif_in_stats, 0)
        self.port10_rif_out_stats = \
            stats['SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS']
        self.assertEqual(self.port10_rif_out_stats, 0)

        self.inseg_entry = sai_thrift_inseg_entry_t(label=3)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry,
            packet_action=SAI_PACKET_ACTION_TRAP)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        self.mplsImplicitNullLabelTrapDropTest()
        self.mplsLabelLookupMissTest()

    def tearDown(self):
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry)

        super(MplsCpuTrapTest, self).tearDown()

    def _verifyStats(self):
        '''
            Additional helper function for statistics verification

            Return:
                bool: True if statistics are correct, otherwise False
        '''
        rif_stats = sai_thrift_get_router_interface_stats(
            self.client, self.port10_rif)

        port_stats = sai_thrift_get_port_stats(self.client, self.port10)

        if self.port10_rif_in_stats != \
                rif_stats['SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'] or \
                self.port10_rif_out_stats != \
                rif_stats['SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS'] or \
                self.port10_disc_stats != \
                port_stats['SAI_PORT_STAT_IF_IN_DISCARDS']:
            return False

        print("\tStatistics correct")
        return True

    def mplsImplicitNullLabelTrapDropTest(self):
        '''
        Verify if MPLS packet with implicit null label (3) is trapped
        or dropped as defined.
        '''
        print("\nmplsImplicitNullLabelTrapDropTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='40.40.40.1',
            ip_ttl=64)
        mpls_tag = {'label': 3, 'ttl': 63, 'tc': 0, 's': 1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])

        try:
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Implicit null label action trap")
            send_packet(self, self.dev_port10, mpls_pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

            self.port10_rif_in_stats += 1
            self.port10_rif_out_stats += 1

            self.assertTrue(self._verifyStats())

            print("Set implicit null label to drop action")
            sai_thrift_set_inseg_entry_attribute(
                self.client, self.inseg_entry,
                packet_action=SAI_PACKET_ACTION_DROP)
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_no_other_packets(self)
            self.port10_rif_in_stats += 1
            self.port10_disc_stats += 1

            self.assertTrue(self._verifyStats())

            print("Add second inseg_entry for label stack")
            inseg_entry_4 = sai_thrift_inseg_entry_t(label=4)
            status = sai_thrift_create_inseg_entry(
                self.client, inseg_entry_4,
                packet_action=SAI_PACKET_ACTION_TRAP)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            mpls_tag = {'label': 3, 'ttl': 63, 'tc': 0, 's': 0}
            mpls_tag_2 = {'label': 4, 'ttl': 63, 'tc': 0, 's': 1}
            mpls_tag_list = [mpls_tag, mpls_tag_2]
            mpls_pkt = simple_mpls_packet(
                eth_dst=ROUTER_MAC,
                mpls_tags=mpls_tag_list,
                inner_frame=send_pkt['IP'])

            send_packet(self, self.dev_port10, mpls_pkt)
            verify_no_other_packets(self)
            self.port10_rif_in_stats += 1
            self.port10_disc_stats += 1

            self.assertTrue(self._verifyStats())

        finally:
            sai_thrift_set_inseg_entry_attribute(
                self.client, self.inseg_entry,
                packet_action=SAI_PACKET_ACTION_TRAP)
            sai_thrift_remove_inseg_entry(self.client, inseg_entry_4)

    def mplsLabelLookupMissTest(self):
        '''
        Verify if MPLS packet with unknown label is dropped and checks
        that associated debug counter is hit.
        '''
        print("\nmplsLabelLookupMissTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='40.40.40.1',
            ip_ttl=64)
        mpls_tag = {'label': 4, 'ttl': 63, 'tc': 0, 's': 1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])

        try:
            drop_reason = sai_thrift_s32_list_t(
                count=1, int32list=[SAI_IN_DROP_REASON_MPLS_MISS])
            debug_cnt = sai_thrift_create_debug_counter(
                self.client,
                type=SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,
                in_drop_reason_list = drop_reason)
            self.assertNotEqual(debug_cnt, 0)

            print("Sending packet with unknown label - should be dropped")
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_no_other_packets(self)
            self.port10_rif_in_stats += 1
            self.port10_disc_stats += 1

            self.assertTrue(self._verifyStats())

            print("Add second inseg_entry for bottom label")
            inseg_entry_4 = sai_thrift_inseg_entry_t(label=4)
            status = sai_thrift_create_inseg_entry(
                self.client, inseg_entry_4,
                packet_action=SAI_PACKET_ACTION_TRAP)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            mpls_tag = {'label': 5, 'ttl': 63, 'tc': 0, 's': 0}
            mpls_tag_2 = {'label': 4, 'ttl': 63, 'tc': 0, 's': 1}
            mpls_tag_list = [mpls_tag, mpls_tag_2]
            mpls_pkt = simple_mpls_packet(
                eth_dst=ROUTER_MAC,
                mpls_tags=mpls_tag_list,
                inner_frame=send_pkt['IP'])

            print("Sending packet with unknown top label - should be dropped")
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_no_other_packets(self)
            self.port10_rif_in_stats += 1
            self.port10_disc_stats += 1

            self.assertTrue(self._verifyStats())

            dc_attr = sai_thrift_get_debug_counter_attribute(
                self.client, debug_cnt, index=True)
            dc_index = dc_attr['index']

            dc_stats = sai_thrift_get_debug_counter_port_stat(
                self.client, self.port10, [dc_index])

        finally:
            sai_thrift_remove_inseg_entry(self.client, inseg_entry_4)
            sai_thrift_remove_debug_counter(self.client, debug_cnt)


@group("draft")
class MplsIpv6Test(SaiHelper):
    ''' Basic IPv6 MPLS test class '''
    def setUp(self):
        super(MplsIpv6Test, self).setUp()

        ttl_value = 63
        exp_value = 5
        ipv6_1 = '100::1'
        ipv6_2 = '100::2'
        ipv6_3 = '100::3'
        ipv6_4 = '220::1'
        ipv6_5 = '330::1'
        ipv6_6 = '550::1'
        ipv6_7 = '700::1'
        ipv6_8 = '710::1'
        ipv6_9 = '720::1'
        ipv6_10 = '800::1'

        ipv6_subnet_1 = '110::1/128'
        ipv6_subnet_2 = '120::1/128'
        ipv6_subnet_3 = '130::1/128'
        ipv6_subnet_4 = '200::1/128'
        dmac = '00:11:22:33:44:55'

        # Ingress LER config
        self.ingress_rif = self.port10_rif
        self.egress_rif_1 = self.port11_rif

        label_list = [1000]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_1),
            router_interface_id=self.egress_rif_1,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_PIPE,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        label_list.append(2000)
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_2),
            router_interface_id=self.egress_rif_1,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_PIPE,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        label_list.append(3000)
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_3),
            router_interface_id=self.egress_rif_1,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_PIPE,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        # note: neighbor with the same dst_ip as MPLS nexthop has to be created
        # after MPLS nexthop is created,
        # if not it will create second nexthop of type IP with this dst_ip
        # and neither of them will be functioning correctly - packets
        # will be gleaned to CPU
        self.mpls_neighbor_entry_1 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_1, ip_address=sai_ipaddress(ipv6_1))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_1, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_neighbor_entry_2 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_1, ip_address=sai_ipaddress(ipv6_2))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_2, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_neighbor_entry_3 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_1, ip_address=sai_ipaddress(ipv6_3))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_3, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_encap_route_1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv6_subnet_1))
        sai_thrift_create_route_entry(
            self.client, self.mpls_encap_route_1,
            next_hop_id=self.nhop_label_1)

        self.mpls_encap_route_2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv6_subnet_2))
        sai_thrift_create_route_entry(
            self.client, self.mpls_encap_route_2,
            next_hop_id=self.nhop_label_2)

        self.mpls_encap_route_3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv6_subnet_3))
        sai_thrift_create_route_entry(
            self.client, self.mpls_encap_route_3,
            next_hop_id=self.nhop_label_3)

        # MPLS egress LER term config
        self.egress_rif_2 = self.port12_rif

        self.mpls_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_MPLS_ROUTER,
            virtual_router_id=self.default_vrf)

        self.nhop_1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_4),
            router_interface_id=self.egress_rif_2,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_1 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_2, ip_address=sai_ipaddress(ipv6_4))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry_1, dst_mac_address=dmac,
            no_host_route=True)
        self.route_entry_1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv6_subnet_4))
        sai_thrift_create_route_entry(
            self.client, self.route_entry_1, next_hop_id=self.nhop_1)

        self.inseg_entry_1000 = sai_thrift_inseg_entry_t(label=1000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_1000,
            num_of_pop=1,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE,
            next_hop_id=self.mpls_rif)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Egress LER Null Term configs
        self.inseg_entry_0 = sai_thrift_inseg_entry_t(label=0)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_0,
            num_of_pop=1,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE,
            next_hop_id=self.mpls_rif)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Egress PHP configs
        self.egress_rif_3 = self.port13_rif

        self.nhop_2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_4),
            router_interface_id=self.egress_rif_3,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.neighbor_entry_2 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_3, ip_address=sai_ipaddress(ipv6_4))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry_2, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_2000 = sai_thrift_inseg_entry_t(label=2000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_2000,
            num_of_pop=1,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.nhop_2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_2111 = sai_thrift_inseg_entry_t(label=2111)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_2111,
            num_of_pop=2,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.nhop_2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_2222 = sai_thrift_inseg_entry_t(label=2222)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_2222,
            num_of_pop=3,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.nhop_2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Egress PHP Swap Null configs
        self.egress_rif_4 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port25,
            virtual_router_id=self.default_vrf)

        label_list = [0]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_4 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_5),
            router_interface_id=self.egress_rif_4,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_PIPE,
            outseg_ttl_value=ttl_value,
            outseg_type=SAI_OUTSEG_TYPE_SWAP)

        self.mpls_neighbor_entry_4 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_4, ip_address=sai_ipaddress(ipv6_5))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_4, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_3000 = sai_thrift_inseg_entry_t(label=3000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_3000,
            next_hop_id=self.nhop_label_4)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Transit Swap configs
        self.egress_rif_5 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port26,
            virtual_router_id=self.default_vrf)

        label_list = [0]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_5 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_6),
            router_interface_id=self.egress_rif_5,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_type=SAI_OUTSEG_TYPE_SWAP)

        self.mpls_neighbor_entry_5 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_5, ip_address=sai_ipaddress(ipv6_6))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_5, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_5000 = sai_thrift_inseg_entry_t(label=5000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_5000,
            next_hop_id=self.nhop_label_5)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MLPS ECMP Swap configs
        self.egress_rif_6 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port27,
            virtual_router_id=self.default_vrf)

        self.egress_rif_7 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port28,
            virtual_router_id=self.default_vrf)

        self.egress_rif_8 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port29,
            virtual_router_id=self.default_vrf)

        label_list = [7070]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_6 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_7),
            router_interface_id=self.egress_rif_6,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_type=SAI_OUTSEG_TYPE_SWAP)

        label_list = [7171]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_7 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_8),
            router_interface_id=self.egress_rif_7,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_type=SAI_OUTSEG_TYPE_SWAP)

        label_list = [7272]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_8 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_9),
            router_interface_id=self.egress_rif_8,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_type=SAI_OUTSEG_TYPE_SWAP)

        self.ecmp_group = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.ecmp_mbr1 = sai_thrift_create_next_hop_group_member(
            self.client, next_hop_group_id=self.ecmp_group,
            next_hop_id=self.nhop_label_6)
        self.ecmp_mbr2 = sai_thrift_create_next_hop_group_member(
            self.client, next_hop_group_id=self.ecmp_group,
            next_hop_id=self.nhop_label_7)
        self.ecmp_mbr3 = sai_thrift_create_next_hop_group_member(
            self.client, next_hop_group_id=self.ecmp_group,
            next_hop_id=self.nhop_label_8)

        self.mpls_neighbor_entry_6 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_6, ip_address=sai_ipaddress(ipv6_7))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_6, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_neighbor_entry_7 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_7, ip_address=sai_ipaddress(ipv6_8))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_7, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_neighbor_entry_8 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_8, ip_address=sai_ipaddress(ipv6_9))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_8, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_7000 = sai_thrift_inseg_entry_t(label=7000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_7000,
            next_hop_id=self.ecmp_group)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Transit Push configs
        self.egress_rif_9 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port30,
            virtual_router_id=self.default_vrf)

        label_list = [9000]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_9 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_10),
            router_interface_id=self.egress_rif_9,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_PIPE,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        self.mpls_neighbor_entry_9 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_9, ip_address=sai_ipaddress(ipv6_10))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_9, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_8000 = sai_thrift_inseg_entry_t(label=8000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_8000,
            next_hop_id=self.nhop_label_9)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # packet counters settings
        for rif in [self.ingress_rif, self.egress_rif_1, self.egress_rif_2,
                    self.egress_rif_3, self.egress_rif_4, self.egress_rif_5,
                    self.egress_rif_6, self.egress_rif_7, self.egress_rif_8,
                    self.egress_rif_9, self.mpls_rif]:
            status = sai_thrift_clear_router_interface_stats(self.client, rif)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.ingress_rif_in_stats = 0     # port 10
        self.ingress_rif_out_stats = 0
        self.egress_rif_1_in_stats = 0    # port 11
        self.egress_rif_1_out_stats = 0
        self.egress_rif_2_in_stats = 0    # port 12
        self.egress_rif_2_out_stats = 0
        self.egress_rif_3_in_stats = 0    # port 13
        self.egress_rif_3_out_stats = 0
        self.egress_rif_4_in_stats = 0    # port 25
        self.egress_rif_4_out_stats = 0
        self.egress_rif_5_in_stats = 0    # port 26
        self.egress_rif_5_out_stats = 0
        self.egress_rif_9_in_stats = 0    # port 30
        self.egress_rif_9_out_stats = 0
        self.mpls_rif_in_stats = 0
        self.mpls_rif_out_stats = 0

    def runTest(self):
        self.mplsIngressLERTest()
        self.mplsEgressLERTermTest()
        self.mplsEgressLERNullTermTest()
        self.mplsEgressPhpTest()
        self.mplsEgressPhpSwapNullTest()
        self.mplsTransitSwapTest()
        self.mplsTransitSwapEcmpHashTest()
        self.mplsTransitPushTest()

    def tearDown(self):
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_8000)
        sai_thrift_remove_neighbor_entry(
            self.client, self.mpls_neighbor_entry_9)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_9)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_9)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_7000)

        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_8)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_7)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_6)

        sai_thrift_remove_next_hop_group_member(self.client, self.ecmp_mbr3)
        sai_thrift_remove_next_hop_group_member(self.client, self.ecmp_mbr2)
        sai_thrift_remove_next_hop_group_member(self.client, self.ecmp_mbr1)
        sai_thrift_remove_next_hop_group(self.client, self.ecmp_group)

        sai_thrift_remove_next_hop(self.client, self.nhop_label_8)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_7)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_6)

        sai_thrift_remove_router_interface(self.client, self.egress_rif_8)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_7)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_6)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_5000)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_5)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_5)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_5)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_3000)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_4)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_4)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_4)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_2222)
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_2111)
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_2000)

        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry_2)
        sai_thrift_remove_next_hop(self.client, self.nhop_2)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_0)
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_1000)

        sai_thrift_remove_route_entry(self.client, self.route_entry_1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry_1)
        sai_thrift_remove_next_hop(self.client, self.nhop_1)

        sai_thrift_remove_router_interface(self.client, self.mpls_rif)

        sai_thrift_remove_route_entry(self.client, self.mpls_encap_route_1)
        sai_thrift_remove_route_entry(self.client, self.mpls_encap_route_2)
        sai_thrift_remove_route_entry(self.client, self.mpls_encap_route_3)

        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_1)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_2)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_3)

        sai_thrift_remove_next_hop(self.client, self.nhop_label_1)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_2)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_3)

        super(MplsIpv6Test, self).tearDown()

    def _verifyStats(self):
        '''
        Additional helper function for statistics verification

        Return:
            bool: True if statistics are correct, otherwise False
        '''
        for rif, in_cnt, out_cnt in zip(
                [self.ingress_rif, self.egress_rif_1, self.egress_rif_2,
                 self.egress_rif_3, self.egress_rif_4, self.egress_rif_5,
                 self.egress_rif_9, self.mpls_rif],
                [self.ingress_rif_in_stats, self.egress_rif_1_in_stats,
                 self.egress_rif_2_in_stats, self.egress_rif_3_in_stats,
                 self.egress_rif_4_in_stats, self.egress_rif_5_in_stats,
                 self.egress_rif_9_in_stats, self.mpls_rif_in_stats],
                [self.ingress_rif_out_stats, self.egress_rif_1_out_stats,
                 self.egress_rif_2_out_stats, self.egress_rif_3_out_stats,
                 self.egress_rif_4_out_stats, self.egress_rif_5_out_stats,
                 self.egress_rif_9_out_stats, self.mpls_rif_out_stats]):
            stats = sai_thrift_get_router_interface_stats(self.client, rif)

            if in_cnt != stats['SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'] or \
                    out_cnt != stats['SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS']:
                print("Counters doesn't match!")
                return False

        print("\tStatistics correct")
        return True

    def mplsIngressLERTest(self):
        '''
        Verify if MPLS labels are added to packet in ingress LER.
        '''
        print("\nmplsIngressLERTest()")

        send_pkt_1 = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='110::1',
            ipv6_hlim=64)
        send_pkt_2 = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='120::1',
            ipv6_hlim=64)
        send_pkt_3 = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='130::1',
            ipv6_hlim=64)
        exp_pkt_1 = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='110::1',
            ipv6_hlim=63)
        exp_pkt_2 = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='120::1',
            ipv6_hlim=63)
        exp_pkt_3 = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='130::1',
            ipv6_hlim=63)
        mpls_tag_list = []

        mpls_tag_1 = {'label': 1000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_2 = {'label': 2000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_3 = {'label': 3000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_1['s'] = 1
        mpls_tag_list.append(mpls_tag_1)
        mpls_pkt_label_1 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_1['IPv6'])

        mpls_tag_1['s'] = 0
        mpls_tag_2['s'] = 1
        del mpls_tag_list[:]
        mpls_tag_list.append(mpls_tag_1)
        mpls_tag_list.append(mpls_tag_2)
        mpls_pkt_label_2 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_2['IPv6'])

        mpls_tag_1['s'] = 0
        mpls_tag_2['s'] = 0
        mpls_tag_3['s'] = 1
        del mpls_tag_list[:]
        mpls_tag_list.append(mpls_tag_1)
        mpls_tag_list.append(mpls_tag_2)
        mpls_tag_list.append(mpls_tag_3)
        mpls_pkt_label_3 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_3['IPv6'])

        print("Send ip packet to add one MPLS label 1000")
        send_packet(self, self.dev_port10, send_pkt_1)
        verify_packet(self, mpls_pkt_label_1, self.dev_port11)
        self.ingress_rif_in_stats += 1
        self.egress_rif_1_out_stats += 1

        print("Send ip packet to add two MPLS label stack - 1000, 2000")
        send_packet(self, self.dev_port10, send_pkt_2)
        verify_packet(self, mpls_pkt_label_2, self.dev_port11)
        self.ingress_rif_in_stats += 1
        self.egress_rif_1_out_stats += 1

        print("Send ip packet to add three MPLS label stack - "
              "1000, 2000, 3000")
        send_packet(self, self.dev_port10, send_pkt_3)
        verify_packet(self, mpls_pkt_label_3, self.dev_port11)
        self.ingress_rif_in_stats += 1
        self.egress_rif_1_out_stats += 1

        self.assertTrue(self._verifyStats())

    def mplsEgressLERTermTest(self):
        '''
        Verify if MPLS label is popped in Egress LER and packet is forwarded
        based on IP lookup.
        '''
        print("\nmplsIngressLERTermTest()")

        send_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='200::1',
            ipv6_hlim=64)
        mpls_tag = {'label': 1000, 'ttl': 63, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IPv6'])
        recv_pkt = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='200::1',
            ipv6_hlim=63)

        print("Send MPLS tag packet with label 1000 - term and IP lookup")
        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_pkt, self.dev_port12)
        self.mpls_rif_in_stats += 1
        self.egress_rif_2_out_stats += 1

        self.assertTrue(self._verifyStats())

    def mplsEgressLERNullTermTest(self):
        '''
        Verify if MPLS null label is popped in Egress LER and packet
        is forwarded based on IP lookup.
        '''
        print("\nmplsIngressLERNullTermTest()")

        send_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='200::1',
            ipv6_hlim=64)
        mpls_tag = {'label': 0, 'ttl': 63, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IPv6'])
        recv_pkt = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='200::1',
            ipv6_hlim=63)

        print("Send MPLS tag packet with label 0 - term and IP lookup")
        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_pkt, self.dev_port12)
        self.mpls_rif_in_stats += 1
        self.egress_rif_2_out_stats += 1

        self.assertTrue(self._verifyStats())

    def mplsEgressPhpTest(self):
        '''
        Verify PHP pops label and forwards packet.
        '''
        print("\nmplsEgressPhpTest()")

        send_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='300::1',
            ipv6_hlim=64)

        mpls_tag = {'label': 2000, 'ttl': 63, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IPv6'])

        mpls_tag_2 = {'label': 2111, 'ttl': 63, 'tc':  0, 's':  0}
        mpls_tag_list_2 = [mpls_tag_2, mpls_tag]
        mpls_tag_2_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list_2,
            inner_frame=send_pkt['IPv6'])

        mpls_tag_3 = {'label': 2222, 'ttl': 63, 'tc':  0, 's':  0}
        mpls_tag_list_3 = [mpls_tag_3, mpls_tag_2, mpls_tag]
        mpls_tag_3_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list_3,
            inner_frame=send_pkt['IPv6'])

        recv_pkt = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='300::1',
            ipv6_hlim=62)
        recv_mpls_tag_list = [mpls_tag]
        recv_mpls_pkt = simple_mpls_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            mpls_tags=recv_mpls_tag_list,
            inner_frame=send_pkt['IPv6'])

        try:
            print("Send MPLS tag pakcet with label 2000 - "
                  "PHP and forward IP packet")
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_packet(self, recv_pkt, self.dev_port13)
            self.ingress_rif_in_stats += 1
            self.egress_rif_3_out_stats += 1

            print("Send MPLS tag pakcet with labels - 2111, 2000 - "
                  "PHP and forward IP packet")
            send_packet(self, self.dev_port10, mpls_tag_2_pkt)
            verify_packet(self, recv_pkt, self.dev_port13)
            self.ingress_rif_in_stats += 1
            self.egress_rif_3_out_stats += 1

            print("Send MPLS tag pakcet with labels - 2222, 2111, 2000 - "
                  "PHP and forward IP packet")
            send_packet(self, self.dev_port10, mpls_tag_3_pkt)
            verify_packet(self, recv_pkt, self.dev_port13)
            self.ingress_rif_in_stats += 1
            self.egress_rif_3_out_stats += 1

            print("Set MPLS object label 2222 to pop only 2 labels")
            sai_thrift_set_inseg_entry_attribute(
                self.client, self.inseg_entry_2222, num_of_pop=2)
            print("Send MPLS tag pakcet with labels - 2222, 2111, 2000 - "
                  "Pop 2 labels and forward MPLS with 2000")
            send_packet(self, self.dev_port10, mpls_tag_3_pkt)
            verify_packet(self, recv_mpls_pkt, self.dev_port13)
            self.ingress_rif_in_stats += 1
            self.egress_rif_3_out_stats += 1

            self.assertTrue(self._verifyStats())

        finally:
            sai_thrift_set_inseg_entry_attribute(
                self.client, self.inseg_entry_2222, num_of_pop=3)

    def mplsEgressPhpSwapNullTest(self):
        '''
        Verify PHP swaps label with explicit null and forwards packet.
        '''
        print("\nmplsEgressPhpSwapNullTest()")

        send_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='400::1',
            ipv6_hlim=64)

        mpls_tag = {'label': 3000, 'ttl': 64, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IPv6'])

        null_tag = {'label': 0, 'ttl': 63, 'tc':  0, 's':  1}
        null_mpls_tag_list = [null_tag]
        recv_mpls_pkt = simple_mpls_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            mpls_tags=null_mpls_tag_list,
            inner_frame=send_pkt['IPv6'])

        print("Send MPLS tag pakcet with label 3000 - "
              "PHP and forward packet with explicit NULL label")
        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_mpls_pkt, self.dev_port25)
        self.ingress_rif_in_stats += 1
        self.egress_rif_4_out_stats += 1

        self.assertTrue(self._verifyStats())

    def mplsTransitSwapTest(self):
        '''
        Verify MPLS label is swapped with another label or explicit null label
        and if only top label is swapped in Transit LSR.
        '''
        print("\nmplsTransitSwapTest()")

        send_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='660::1',
            ipv6_hlim=64)

        mpls_tag = {'label': 5000, 'ttl': 64, 'tc':  3, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IPv6'])

        mpls_tag = {'label': 0, 'ttl': 63, 'tc':  3, 's':  1}
        mpls_tag_list = [mpls_tag]
        recv_mpls_pkt = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IPv6'])

        mpls_tag = {'label': 6000, 'ttl': 63, 'tc':  3, 's':  1}
        new_mpls_tag_list = [mpls_tag]
        recv_mpls_pkt_6000 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=new_mpls_tag_list,
            inner_frame=send_pkt['IPv6'])

        mpls_tag_1 = {'label': 5000, 'ttl': 64, 'tc':  5, 's':  0}
        mpls_tag_2 = {'label': 4000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_3 = {'label': 3000, 'ttl': 63, 'tc':  5, 's':  1}
        mpls_3_tag_list = [mpls_tag_1, mpls_tag_2, mpls_tag_3]
        mpls_3_tag_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_3_tag_list,
            inner_frame=send_pkt['IPv6'])

        mpls_swap_tag = {'label': 6000, 'ttl': 63, 'tc':  5, 's':  0}
        recv_mpls_3_tag_list = [mpls_swap_tag, mpls_tag_2, mpls_tag_3]
        recv_mpls_3_tag_pkt = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=recv_mpls_3_tag_list,
            inner_frame=send_pkt['IPv6'])

        try:
            print("Send MPLS tag 5000 packet and swap with explicit NULL")
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_packet(self, recv_mpls_pkt, self.dev_port26)
            self.ingress_rif_in_stats += 1
            self.egress_rif_5_out_stats += 1

            print("Set MPLS nexthop to swap with label 6000")
            sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_5000)
            sai_thrift_remove_neighbor_entry(self.client,
                                             self.mpls_neighbor_entry_5)
            sai_thrift_remove_next_hop(self.client, self.nhop_label_5)
            label_list = [6000]
            label_list_t = sai_thrift_u32_list_t(
                count=len(label_list),
                uint32list=label_list)
            self.nhop_label_5 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress('550::1'),
                router_interface_id=self.egress_rif_5,
                type=SAI_NEXT_HOP_TYPE_MPLS,
                labelstack=label_list_t,
                outseg_type=SAI_OUTSEG_TYPE_SWAP)

            self.mpls_neighbor_entry_5 = sai_thrift_neighbor_entry_t(
                rif_id=self.egress_rif_5,
                ip_address=sai_ipaddress('550::1'))
            status = sai_thrift_create_neighbor_entry(
                self.client, self.mpls_neighbor_entry_5,
                dst_mac_address='00:11:22:33:44:55',
                no_host_route=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            self.inseg_entry_5000 = sai_thrift_inseg_entry_t(label=5000)
            status = sai_thrift_create_inseg_entry(
                self.client, self.inseg_entry_5000,
                next_hop_id=self.nhop_label_5)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            print("Send MPLS tag 5000 packet and swap with 6000")
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_packet(self, recv_mpls_pkt_6000, self.dev_port26)
            self.ingress_rif_in_stats += 1
            self.egress_rif_5_out_stats += 1

            print("Send 3-tagged packet and swap top label 5000 with 6000")
            send_packet(self, self.dev_port10, mpls_3_tag_pkt)
            verify_packet(self, recv_mpls_3_tag_pkt, self.dev_port26)
            self.ingress_rif_in_stats += 1
            self.egress_rif_5_out_stats += 1

            self.assertTrue(self._verifyStats())

        finally:
            sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_5000)
            sai_thrift_remove_neighbor_entry(self.client,
                                             self.mpls_neighbor_entry_5)
            sai_thrift_remove_next_hop(self.client, self.nhop_label_5)
            label_list = [0]
            label_list_t = sai_thrift_u32_list_t(
                count=len(label_list),
                uint32list=label_list)
            self.nhop_label_5 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress('550::1'),
                router_interface_id=self.egress_rif_5,
                type=SAI_NEXT_HOP_TYPE_MPLS,
                labelstack=label_list_t,
                outseg_type=SAI_OUTSEG_TYPE_SWAP)

            self.mpls_neighbor_entry_5 = sai_thrift_neighbor_entry_t(
                rif_id=self.egress_rif_5,
                ip_address=sai_ipaddress('550::1'))
            status = sai_thrift_create_neighbor_entry(
                self.client, self.mpls_neighbor_entry_5,
                dst_mac_address='00:11:22:33:44:55',
                no_host_route=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            self.inseg_entry_5000 = sai_thrift_inseg_entry_t(label=5000)
            status = sai_thrift_create_inseg_entry(
                self.client, self.inseg_entry_5000,
                next_hop_id=self.nhop_label_5)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def mplsTransitSwapEcmpHashTest(self):
        '''
        Verify ECMP forwarding with MPLS Transit nhops.
        '''
        print("\nmplsTransitSwapEcmpHashTest()")

        send_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='660::1',
            ipv6_hlim=64)

        mpls_tag = {'label': 7000, 'ttl': 64, 'tc':  3, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IPv6'])

        mpls_swap_tag_7070 = {'label': 7070, 'ttl': 63, 'tc':  3, 's':  1}
        mpls_tag_7070 = [mpls_swap_tag_7070]
        mpls_swap_tag_7171 = {'label': 7171, 'ttl': 63, 'tc':  3, 's':  1}
        mpls_tag_7171 = [mpls_swap_tag_7171]
        mpls_swap_tag_7272 = {'label': 7272, 'ttl': 63, 'tc':  3, 's':  1}
        mpls_tag_7272 = [mpls_swap_tag_7272]

        recv_mpls_7070 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_7070,
            inner_frame=send_pkt['IPv6'])

        recv_mpls_7171 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_7171,
            inner_frame=send_pkt['IPv6'])

        recv_mpls_7272 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_7272,
            inner_frame=send_pkt['IPv6'])

        rif_cntr_id = 'SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS'
        rif6_stats_start = sai_thrift_get_router_interface_stats(
            self.client, self.egress_rif_6)
        rif7_stats_start = sai_thrift_get_router_interface_stats(
            self.client, self.egress_rif_7)
        rif8_stats_start = sai_thrift_get_router_interface_stats(
            self.client, self.egress_rif_8)

        print("ECMP - Send MPLS tag packet 7000")
        send_packet(self, self.dev_port10, mpls_pkt)
        self.ingress_rif_in_stats += 1
        verify_any_packet_any_port(
            self, [recv_mpls_7070, recv_mpls_7171, recv_mpls_7272],
            [self.dev_port27, self.dev_port28, self.dev_port29])
        rif6_stats_end = sai_thrift_get_router_interface_stats(
            self.client, self.egress_rif_6)
        rif7_stats_end = sai_thrift_get_router_interface_stats(
            self.client, self.egress_rif_7)
        rif8_stats_end = sai_thrift_get_router_interface_stats(
            self.client, self.egress_rif_8)

        if (rif6_stats_end[rif_cntr_id] -
                rif6_stats_start[rif_cntr_id] == 1):
            final_egress_rif = self.egress_rif_6
        if (rif7_stats_end[rif_cntr_id] -
                rif7_stats_start[rif_cntr_id] == 1):
            final_egress_rif = self.egress_rif_7
        if (rif8_stats_end[rif_cntr_id] -
                rif8_stats_start[rif_cntr_id] == 1):
            final_egress_rif = self.egress_rif_8
        else:
            final_egress_rif = 0

        iter_count = 10
        rif_lb_start_count = sai_thrift_get_router_interface_stats(
            self.client, final_egress_rif)

        for _ in range(0, iter_count):
            l4_sport = random.randint(5000, 65535)
            l4_dport = random.randint(5000, 65535)
            mpls_pkt['TCP'].sport = l4_sport
            mpls_pkt['TCP'].dport = l4_dport
            send_packet(self, self.dev_port10, mpls_pkt)
            self.ingress_rif_in_stats += 1

        time.sleep(2)
        rif_lb_end_count = sai_thrift_get_router_interface_stats(
            self.client, final_egress_rif)
        diff = (rif_lb_end_count[rif_cntr_id] -
                rif_lb_start_count[rif_cntr_id])
        self.assertEqual(diff, iter_count)

        self.assertTrue(self._verifyStats())

    def mplsTransitPushTest(self):
        '''
        Verify MPLS label is pushed on stack in transit LSR.
        '''
        print("\nmplsTransitPushTest()")

        send_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='660::1',
            ipv6_hlim=64)

        mpls_tag = {'label': 8000, 'ttl': 64, 'tc':  3, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IPv6'])

        mpls_push_tag = {'label': 9000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_list = [mpls_push_tag, mpls_tag]
        recv_mpls_pkt = simple_mpls_packet(
            eth_dst="00:11:22:33:44:55",
            eth_src=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IPv6'])

        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_mpls_pkt, self.dev_port30)
        self.ingress_rif_in_stats += 1
        self.egress_rif_9_out_stats += 1

        self.assertTrue(self._verifyStats())


@group("draft")
class MplsIpv4Test(SaiHelper):
    ''' Basic IPv4 MPLS test class '''
    def setUp(self):
        super(MplsIpv4Test, self).setUp()

        ttl_value = 63
        exp_value = 5
        ipv4_1 = '10.10.10.1'
        ipv4_2 = '10.10.10.2'
        ipv4_3 = '10.10.10.3'
        ipv4_4 = '20.20.20.1'
        ipv4_5 = '30.30.30.1'
        ipv4_6 = '50.50.50.1'
        ipv4_7 = '70.70.70.1'
        ipv4_8 = '71.71.71.1'
        ipv4_9 = '72.72.72.1'
        ipv4_10 = '80.80.80.1'

        ipv4_subnet_1 = '10.10.10.1/32'
        ipv4_subnet_2 = '10.10.10.2/32'
        ipv4_subnet_3 = '10.10.10.3/32'
        ipv4_subnet_4 = '20.20.20.1/24'
        dmac = '00:11:22:33:44:55'

        # Ingress LER config
        self.ingress_rif = self.port10_rif
        self.egress_rif_1 = self.port11_rif

        label_list = [1000]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_1),
            router_interface_id=self.egress_rif_1,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_PIPE,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        label_list.append(2000)
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_2),
            router_interface_id=self.egress_rif_1,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_PIPE,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        label_list.append(3000)
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_3),
            router_interface_id=self.egress_rif_1,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_PIPE,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        # note: neighbor with the same dst_ip as MPLS nexthop has to be created
        # after MPLS nexthop is created,
        # if not it will create second nexthop of type IP with this dst_ip
        # and neither of them will be functioning correctly - packets
        # will be gleaned to CPU
        self.mpls_neighbor_entry_1 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_1, ip_address=sai_ipaddress(ipv4_1))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_1, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_neighbor_entry_2 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_1, ip_address=sai_ipaddress(ipv4_2))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_2, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_neighbor_entry_3 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_1, ip_address=sai_ipaddress(ipv4_3))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_3, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_encap_route_1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv4_subnet_1))
        sai_thrift_create_route_entry(
            self.client, self.mpls_encap_route_1,
            next_hop_id=self.nhop_label_1)

        self.mpls_encap_route_2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv4_subnet_2))
        sai_thrift_create_route_entry(
            self.client, self.mpls_encap_route_2,
            next_hop_id=self.nhop_label_2)

        self.mpls_encap_route_3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv4_subnet_3))
        sai_thrift_create_route_entry(
            self.client, self.mpls_encap_route_3,
            next_hop_id=self.nhop_label_3)

        # MPLS egress LER term config
        self.egress_rif_2 = self.port12_rif

        self.vrf = sai_thrift_create_virtual_router(self.client)

        self.egress_rif_vrf = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port24,
            virtual_router_id=self.default_vrf)

        self.mpls_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_MPLS_ROUTER,
            virtual_router_id=self.default_vrf)

        self.nhop_vrf = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_4),
            router_interface_id=self.egress_rif_vrf,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_vrf = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_vrf, ip_address=sai_ipaddress(ipv4_4))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry_vrf, dst_mac_address=dmac,
            no_host_route=True)
        self.route_entry_vrf = sai_thrift_route_entry_t(
            vr_id=self.vrf, destination=sai_ipprefix(ipv4_subnet_4))
        sai_thrift_create_route_entry(
            self.client, self.route_entry_vrf, next_hop_id=self.nhop_vrf)

        self.nhop_1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_4),
            router_interface_id=self.egress_rif_2,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_1 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_2, ip_address=sai_ipaddress(ipv4_4))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry_1, dst_mac_address=dmac,
            no_host_route=True)
        self.route_entry_1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv4_subnet_4))
        sai_thrift_create_route_entry(
            self.client, self.route_entry_1, next_hop_id=self.nhop_1)

        self.inseg_entry_1000 = sai_thrift_inseg_entry_t(label=1000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_1000,
            num_of_pop=1,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE,
            next_hop_id=self.mpls_rif)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Egress LER Null Term configs
        self.inseg_entry_0 = sai_thrift_inseg_entry_t(label=0)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_0,
            num_of_pop=1,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE,
            next_hop_id=self.mpls_rif)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Egress PHP configs
        self.egress_rif_3 = self.port13_rif

        self.nhop_2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_4),
            router_interface_id=self.egress_rif_3,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.neighbor_entry_2 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_3, ip_address=sai_ipaddress(ipv4_4))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry_2, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_2000 = sai_thrift_inseg_entry_t(label=2000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_2000,
            num_of_pop=1,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.nhop_2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_2111 = sai_thrift_inseg_entry_t(label=2111)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_2111,
            num_of_pop=2,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.nhop_2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_2222 = sai_thrift_inseg_entry_t(label=2222)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_2222,
            num_of_pop=3,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.nhop_2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Egress PHP Swap Null configs
        self.egress_rif_4 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port25,
            virtual_router_id=self.default_vrf)

        label_list = [0]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_4 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_5),
            router_interface_id=self.egress_rif_4,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_PIPE,
            outseg_ttl_value=ttl_value,
            outseg_type=SAI_OUTSEG_TYPE_SWAP)

        self.mpls_neighbor_entry_4 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_4, ip_address=sai_ipaddress(ipv4_5))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_4, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_3000 = sai_thrift_inseg_entry_t(label=3000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_3000,
            next_hop_id=self.nhop_label_4)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Transit Swap configs
        self.egress_rif_5 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port26,
            virtual_router_id=self.default_vrf)

        label_list = [0]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_5 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_6),
            router_interface_id=self.egress_rif_5,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_type=SAI_OUTSEG_TYPE_SWAP)

        self.mpls_neighbor_entry_5 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_5, ip_address=sai_ipaddress(ipv4_6))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_5, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_5000 = sai_thrift_inseg_entry_t(label=5000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_5000,
            next_hop_id=self.nhop_label_5)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MLPS ECMP Swap configs
        self.egress_rif_6 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port27,
            virtual_router_id=self.default_vrf)

        self.egress_rif_7 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port28,
            virtual_router_id=self.default_vrf)

        self.egress_rif_8 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port29,
            virtual_router_id=self.default_vrf)

        label_list = [7070]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_6 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_7),
            router_interface_id=self.egress_rif_6,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_type=SAI_OUTSEG_TYPE_SWAP)

        label_list = [7171]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_7 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_8),
            router_interface_id=self.egress_rif_7,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_type=SAI_OUTSEG_TYPE_SWAP)

        label_list = [7272]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_8 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_9),
            router_interface_id=self.egress_rif_8,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_type=SAI_OUTSEG_TYPE_SWAP)

        self.ecmp_group = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.ecmp_mbr1 = sai_thrift_create_next_hop_group_member(
            self.client, next_hop_group_id=self.ecmp_group,
            next_hop_id=self.nhop_label_6)
        self.ecmp_mbr2 = sai_thrift_create_next_hop_group_member(
            self.client, next_hop_group_id=self.ecmp_group,
            next_hop_id=self.nhop_label_7)
        self.ecmp_mbr3 = sai_thrift_create_next_hop_group_member(
            self.client, next_hop_group_id=self.ecmp_group,
            next_hop_id=self.nhop_label_8)

        self.mpls_neighbor_entry_6 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_6, ip_address=sai_ipaddress(ipv4_7))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_6, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_neighbor_entry_7 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_7, ip_address=sai_ipaddress(ipv4_8))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_7, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_neighbor_entry_8 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_8, ip_address=sai_ipaddress(ipv4_9))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_8, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_7000 = sai_thrift_inseg_entry_t(label=7000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_7000,
            next_hop_id=self.ecmp_group)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Transit Push configs
        self.egress_rif_9 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port30,
            virtual_router_id=self.default_vrf)

        label_list = [9000]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_9 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_10),
            router_interface_id=self.egress_rif_9,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_PIPE,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        self.mpls_neighbor_entry_9 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_9, ip_address=sai_ipaddress(ipv4_10))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_9, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_8000 = sai_thrift_inseg_entry_t(label=8000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_8000,
            next_hop_id=self.nhop_label_9)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # packet counters settings
        for rif in [self.ingress_rif, self.egress_rif_1, self.egress_rif_2,
                    self.egress_rif_3, self.egress_rif_4, self.egress_rif_5,
                    self.egress_rif_9, self.egress_rif_vrf, self.mpls_rif]:
            status = sai_thrift_clear_router_interface_stats(self.client, rif)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.ingress_rif_in_stats = 0     # port 10
        self.ingress_rif_out_stats = 0
        self.egress_rif_1_in_stats = 0    # port 11
        self.egress_rif_1_out_stats = 0
        self.egress_rif_2_in_stats = 0    # port 12
        self.egress_rif_2_out_stats = 0
        self.egress_rif_3_in_stats = 0    # port 13
        self.egress_rif_3_out_stats = 0
        self.egress_rif_4_in_stats = 0    # port 25
        self.egress_rif_4_out_stats = 0
        self.egress_rif_5_in_stats = 0    # port 26
        self.egress_rif_5_out_stats = 0
        self.egress_rif_vrf_in_stats = 0  # port 24
        self.egress_rif_vrf_out_stats = 0
        self.egress_rif_9_in_stats = 0    # port 30
        self.egress_rif_9_out_stats = 0
        self.mpls_rif_in_stats = 0
        self.mpls_rif_out_stats = 0

    def runTest(self):
        self.mplsIngressLERTest()
        self.mplsEgressLERTermTest()
        self.mplsEgressLERTermUpdateMplsRifVrfTest()
        self.mplsEgressLERNullTermTest()
        self.mplsEgressPhpTest()
        self.mplsEgressPhpSwapNullTest()
        self.mplsTransitSwapTest()
        self.mplsTransitSwapEcmpHashTest()
        self.mplsTransitPushTest()

    def tearDown(self):
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_8000)
        sai_thrift_remove_neighbor_entry(
            self.client, self.mpls_neighbor_entry_9)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_9)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_9)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_7000)

        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_8)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_7)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_6)

        sai_thrift_remove_next_hop_group_member(self.client, self.ecmp_mbr3)
        sai_thrift_remove_next_hop_group_member(self.client, self.ecmp_mbr2)
        sai_thrift_remove_next_hop_group_member(self.client, self.ecmp_mbr1)
        sai_thrift_remove_next_hop_group(self.client, self.ecmp_group)

        sai_thrift_remove_next_hop(self.client, self.nhop_label_8)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_7)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_6)

        sai_thrift_remove_router_interface(self.client, self.egress_rif_8)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_7)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_6)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_5000)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_5)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_5)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_5)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_3000)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_4)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_4)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_4)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_2222)
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_2111)
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_2000)

        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry_2)
        sai_thrift_remove_next_hop(self.client, self.nhop_2)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_0)
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_1000)

        sai_thrift_remove_route_entry(self.client, self.route_entry_vrf)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry_vrf)
        sai_thrift_remove_next_hop(self.client, self.nhop_vrf)

        sai_thrift_remove_router_interface(self.client, self.egress_rif_vrf)
        sai_thrift_remove_virtual_router(self.client, self.vrf)

        sai_thrift_remove_route_entry(self.client, self.route_entry_1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry_1)
        sai_thrift_remove_next_hop(self.client, self.nhop_1)

        sai_thrift_remove_router_interface(self.client, self.mpls_rif)

        sai_thrift_remove_route_entry(self.client, self.mpls_encap_route_1)
        sai_thrift_remove_route_entry(self.client, self.mpls_encap_route_2)
        sai_thrift_remove_route_entry(self.client, self.mpls_encap_route_3)

        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_1)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_2)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_3)

        sai_thrift_remove_next_hop(self.client, self.nhop_label_1)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_2)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_3)

        super(MplsIpv4Test, self).tearDown()

    def _verifyStats(self):
        '''
        Additional helper function for statistics verification

        Return:
            bool: True if statistics are correct, otherwise False
        '''
        for rif, in_cnt, out_cnt in zip(
                [self.ingress_rif, self.egress_rif_1, self.egress_rif_2,
                 self.egress_rif_3, self.egress_rif_4, self.egress_rif_5,
                 self.egress_rif_9, self.egress_rif_vrf, self.mpls_rif],
                [self.ingress_rif_in_stats, self.egress_rif_1_in_stats,
                 self.egress_rif_2_in_stats, self.egress_rif_3_in_stats,
                 self.egress_rif_4_in_stats, self.egress_rif_5_in_stats,
                 self.egress_rif_9_in_stats, self.egress_rif_vrf_in_stats,
                 self.mpls_rif_in_stats],
                [self.ingress_rif_out_stats, self.egress_rif_1_out_stats,
                 self.egress_rif_2_out_stats, self.egress_rif_3_out_stats,
                 self.egress_rif_4_out_stats, self.egress_rif_5_out_stats,
                 self.egress_rif_9_out_stats, self.egress_rif_vrf_out_stats,
                 self.mpls_rif_out_stats]):
            stats = sai_thrift_get_router_interface_stats(self.client, rif)

            if in_cnt != stats['SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'] or \
                    out_cnt != stats['SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS']:
                print("Counters doesn't match!")
                return False

        print("\tStatistics correct")
        return True

    def mplsIngressLERTest(self):
        '''
        Verify if MPLS labels are added to packet in ingress LER.
        '''
        print("\nmplsIngressLERTest()")

        send_pkt_1 = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='10.10.10.1',
            ip_ttl=64)
        send_pkt_2 = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='10.10.10.2',
            ip_ttl=64)
        send_pkt_3 = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='10.10.10.3',
            ip_ttl=64)
        exp_pkt_1 = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='10.10.10.1',
            ip_ttl=63)
        exp_pkt_2 = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='10.10.10.2',
            ip_ttl=63)
        exp_pkt_3 = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='10.10.10.3',
            ip_ttl=63)
        mpls_tag_list = []

        mpls_tag_1 = {'label': 1000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_2 = {'label': 2000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_3 = {'label': 3000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_1['s'] = 1
        mpls_tag_list.append(mpls_tag_1)
        mpls_pkt_label_1 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_1['IP'])

        mpls_tag_1['s'] = 0
        mpls_tag_2['s'] = 1
        del mpls_tag_list[:]
        mpls_tag_list.append(mpls_tag_1)
        mpls_tag_list.append(mpls_tag_2)
        mpls_pkt_label_2 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_2['IP'])

        mpls_tag_1['s'] = 0
        mpls_tag_2['s'] = 0
        mpls_tag_3['s'] = 1
        del mpls_tag_list[:]
        mpls_tag_list.append(mpls_tag_1)
        mpls_tag_list.append(mpls_tag_2)
        mpls_tag_list.append(mpls_tag_3)
        mpls_pkt_label_3 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_3['IP'])

        print("Send ip packet to add one MPLS label 1000")
        send_packet(self, self.dev_port10, send_pkt_1)
        verify_packet(self, mpls_pkt_label_1, self.dev_port11)
        self.ingress_rif_in_stats += 1
        self.egress_rif_1_out_stats += 1

        print("Send ip packet to add two MPLS label stack - 1000, 2000")
        send_packet(self, self.dev_port10, send_pkt_2)
        verify_packet(self, mpls_pkt_label_2, self.dev_port11)
        self.ingress_rif_in_stats += 1
        self.egress_rif_1_out_stats += 1

        print("Send ip packet to add three MPLS label stack - "
              "1000, 2000, 3000")
        send_packet(self, self.dev_port10, send_pkt_3)
        verify_packet(self, mpls_pkt_label_3, self.dev_port11)
        self.ingress_rif_in_stats += 1
        self.egress_rif_1_out_stats += 1

        self.assertTrue(self._verifyStats())

    def mplsEgressLERTermTest(self):
        '''
        Verify if MPLS label is popped in Egress LER and packet is forwarded
        based on IP lookup.
        '''
        print("\nmplsIngressLERTermTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='20.20.20.1',
            ip_ttl=64)
        mpls_tag = {'label': 1000, 'ttl': 63, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])
        recv_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='20.20.20.1',
            ip_ttl=63)

        print("Send MPLS tag packet with label 1000 - term and IP lookup")
        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_pkt, self.dev_port12)
        self.mpls_rif_in_stats += 1
        self.egress_rif_2_out_stats += 1

        self.assertTrue(self._verifyStats())

    def mplsEgressLERTermUpdateMplsRifVrfTest(self):
        '''
        Verify if MPLS label is popped in Egress LER and packet is forwarded
        based on IP lookup afert changing VRF on MPLS RIF.
        '''
        print("\nmplsIngressLERTermUpdateMplsRifVrfTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='20.20.20.1',
            ip_ttl=64)
        mpls_tag = {'label': 1000, 'ttl': 63, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])
        recv_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='20.20.20.1',
            ip_ttl=63)

        try:
            print("Send MPLS tag packet with label 1000 - term and IP lookup"
                  "forwarded to port", self.dev_port12)
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_packet(self, recv_pkt, self.dev_port12)
            self.egress_rif_2_out_stats += 1

            print("Remove existing MPLS RIF and create new with different VRF")
            sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_0)
            sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_1000)
            sai_thrift_remove_router_interface(self.client, self.mpls_rif)

            self.mpls_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_MPLS_ROUTER,
                virtual_router_id=self.vrf)

            self.inseg_entry_1000 = sai_thrift_inseg_entry_t(label=1000)
            sai_thrift_create_inseg_entry(
                self.client, self.inseg_entry_1000,
                num_of_pop=1,
                pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE,
                next_hop_id=self.mpls_rif)

            self.inseg_entry_0 = sai_thrift_inseg_entry_t(label=0)
            sai_thrift_create_inseg_entry(
                self.client, self.inseg_entry_0,
                num_of_pop=1,
                pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE,
                next_hop_id=self.mpls_rif)

            print("Send MPLS tag packet with label 1000 - term and IP lookup"
                  "forwarded to port", self.dev_port24)
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_packet(self, recv_pkt, self.dev_port24)
            self.mpls_rif_in_stats = 1
            self.egress_rif_vrf_out_stats += 1

            self.assertTrue(self._verifyStats())

        finally:
            sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_0)
            sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_1000)
            sai_thrift_remove_router_interface(self.client, self.mpls_rif)
            self.mpls_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_MPLS_ROUTER,
                virtual_router_id=self.default_vrf)

            self.inseg_entry_1000 = sai_thrift_inseg_entry_t(label=1000)
            sai_thrift_create_inseg_entry(
                self.client, self.inseg_entry_1000,
                num_of_pop=1,
                pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE,
                next_hop_id=self.mpls_rif)

            self.inseg_entry_0 = sai_thrift_inseg_entry_t(label=0)
            sai_thrift_create_inseg_entry(
                self.client, self.inseg_entry_0,
                num_of_pop=1,
                pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE,
                next_hop_id=self.mpls_rif)

    def mplsEgressLERNullTermTest(self):
        '''
        Verify if MPLS null label is popped in Egress LER and packet
        is forwarded based on IP lookup.
        '''
        print("\nmplsIngressLERNullTermTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='20.20.20.1',
            ip_ttl=64)
        mpls_tag = {'label': 0, 'ttl': 63, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])
        recv_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='20.20.20.1',
            ip_ttl=63)

        print("Send MPLS tag packet with label 0 - term and IP lookup")
        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_pkt, self.dev_port12)
        self.mpls_rif_in_stats = 1
        self.egress_rif_2_out_stats += 1

        self.assertTrue(self._verifyStats())

    def mplsEgressPhpTest(self):
        '''
        Verify PHP pops label and forwards packet.
        '''
        print("\nmplsEgressPhpTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='30.30.30.1',
            ip_ttl=64)

        mpls_tag = {'label': 2000, 'ttl': 63, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])

        mpls_tag_2 = {'label': 2111, 'ttl': 63, 'tc':  0, 's':  0}
        mpls_tag_list_2 = [mpls_tag_2, mpls_tag]
        mpls_tag_2_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list_2,
            inner_frame=send_pkt['IP'])

        mpls_tag_3 = {'label': 2222, 'ttl': 63, 'tc':  0, 's':  0}
        mpls_tag_list_3 = [mpls_tag_3, mpls_tag_2, mpls_tag]
        mpls_tag_3_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list_3,
            inner_frame=send_pkt['IP'])

        recv_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='30.30.30.1',
            ip_ttl=62)
        recv_mpls_tag_list = [mpls_tag]
        recv_mpls_pkt = simple_mpls_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            mpls_tags=recv_mpls_tag_list,
            inner_frame=send_pkt['IP'])

        try:
            print("Send MPLS tag pakcet with label 2000 - "
                  "PHP and forward IP packet")
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_packet(self, recv_pkt, self.dev_port13)
            self.ingress_rif_in_stats += 1
            self.egress_rif_3_out_stats += 1

            print("Send MPLS tag pakcet with labels - 2111, 2000 - "
                  "PHP and forward IP packet")
            send_packet(self, self.dev_port10, mpls_tag_2_pkt)
            verify_packet(self, recv_pkt, self.dev_port13)
            self.ingress_rif_in_stats += 1
            self.egress_rif_3_out_stats += 1

            print("Send MPLS tag pakcet with labels - 2222, 2111, 2000 - "
                  "PHP and forward IP packet")
            send_packet(self, self.dev_port10, mpls_tag_3_pkt)
            verify_packet(self, recv_pkt, self.dev_port13)
            self.ingress_rif_in_stats += 1
            self.egress_rif_3_out_stats += 1

            print("Set MPLS object label 2222 to pop only 2 labels")
            sai_thrift_set_inseg_entry_attribute(
                self.client, self.inseg_entry_2222, num_of_pop=2)
            print("Send MPLS tag pakcet with labels - 2222, 2111, 2000 - "
                  "Pop 2 labels and forward MPLS with 2000")
            send_packet(self, self.dev_port10, mpls_tag_3_pkt)
            verify_packet(self, recv_mpls_pkt, self.dev_port13)
            self.ingress_rif_in_stats += 1
            self.egress_rif_3_out_stats += 1

            self.assertTrue(self._verifyStats())

        finally:
            sai_thrift_set_inseg_entry_attribute(
                self.client, self.inseg_entry_2222, num_of_pop=3)

    def mplsEgressPhpSwapNullTest(self):
        '''
        Verify PHP swaps label with explicit null and forwards packet.
        '''
        print("\nmplsEgressPhpSwapNullTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='40.40.40.1',
            ip_ttl=64)

        mpls_tag = {'label': 3000, 'ttl': 64, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])

        null_tag = {'label': 0, 'ttl': 63, 'tc':  0, 's':  1}
        null_mpls_tag_list = [null_tag]
        recv_mpls_pkt = simple_mpls_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            mpls_tags=null_mpls_tag_list,
            inner_frame=send_pkt['IP'])

        print("Send MPLS tag pakcet with label 3000 - "
              "PHP and forward packet with explicit NULL label")
        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_mpls_pkt, self.dev_port25)
        self.ingress_rif_in_stats += 1
        self.egress_rif_4_out_stats += 1

        self.assertTrue(self._verifyStats())

    def mplsTransitSwapTest(self):
        '''
        Verify MPLS label is swapped with another label or explicit null label
        and if only top label is swapped in Transit LSR.
        '''
        print("\nmplsTransitSwapTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='60.60.60.1',
            ip_ttl=64)

        mpls_tag = {'label': 5000, 'ttl': 64, 'tc':  3, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])

        mpls_tag = {'label': 0, 'ttl': 63, 'tc':  3, 's':  1}
        mpls_tag_list = [mpls_tag]
        recv_mpls_pkt = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])

        mpls_tag = {'label': 6000, 'ttl': 63, 'tc':  3, 's':  1}
        new_mpls_tag_list = [mpls_tag]
        recv_mpls_pkt_6000 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=new_mpls_tag_list,
            inner_frame=send_pkt['IP'])

        mpls_tag_1 = {'label': 5000, 'ttl': 64, 'tc':  5, 's':  0}
        mpls_tag_2 = {'label': 4000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_3 = {'label': 3000, 'ttl': 63, 'tc':  5, 's':  1}
        mpls_3_tag_list = [mpls_tag_1, mpls_tag_2, mpls_tag_3]
        mpls_3_tag_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_3_tag_list,
            inner_frame=send_pkt['IP'])

        mpls_swap_tag = {'label': 6000, 'ttl': 63, 'tc':  5, 's':  0}
        recv_mpls_3_tag_list = [mpls_swap_tag, mpls_tag_2, mpls_tag_3]
        recv_mpls_3_tag_pkt = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=recv_mpls_3_tag_list,
            inner_frame=send_pkt['IP'])

        try:
            print("Send MPLS tag 5000 packet and swap with explicit NULL")
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_packet(self, recv_mpls_pkt, self.dev_port26)
            self.ingress_rif_in_stats += 1
            self.egress_rif_5_out_stats += 1

            print("Set MPLS nexthop to swap with label 6000")
            sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_5000)
            sai_thrift_remove_neighbor_entry(self.client,
                                             self.mpls_neighbor_entry_5)
            sai_thrift_remove_next_hop(self.client, self.nhop_label_5)
            label_list = [6000]
            label_list_t = sai_thrift_u32_list_t(
                count=len(label_list),
                uint32list=label_list)
            self.nhop_label_5 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress('50.50.50.1'),
                router_interface_id=self.egress_rif_5,
                type=SAI_NEXT_HOP_TYPE_MPLS,
                labelstack=label_list_t,
                outseg_type=SAI_OUTSEG_TYPE_SWAP)

            self.mpls_neighbor_entry_5 = sai_thrift_neighbor_entry_t(
                rif_id=self.egress_rif_5,
                ip_address=sai_ipaddress('50.50.50.1'))
            status = sai_thrift_create_neighbor_entry(
                self.client, self.mpls_neighbor_entry_5,
                dst_mac_address='00:11:22:33:44:55',
                no_host_route=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            self.inseg_entry_5000 = sai_thrift_inseg_entry_t(label=5000)
            status = sai_thrift_create_inseg_entry(
                self.client, self.inseg_entry_5000,
                next_hop_id=self.nhop_label_5)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            print("Send MPLS tag 5000 packet and swap with 6000")
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_packet(self, recv_mpls_pkt_6000, self.dev_port26)
            self.ingress_rif_in_stats += 1
            self.egress_rif_5_out_stats += 1

            print("Send 3-tagged packet and swap top label 5000 with 6000")
            send_packet(self, self.dev_port10, mpls_3_tag_pkt)
            verify_packet(self, recv_mpls_3_tag_pkt, self.dev_port26)
            self.ingress_rif_in_stats += 1
            self.egress_rif_5_out_stats += 1

            self.assertTrue(self._verifyStats())

        finally:
            sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_5000)
            sai_thrift_remove_neighbor_entry(self.client,
                                             self.mpls_neighbor_entry_5)
            sai_thrift_remove_next_hop(self.client, self.nhop_label_5)
            label_list = [0]
            label_list_t = sai_thrift_u32_list_t(
                count=len(label_list),
                uint32list=label_list)
            self.nhop_label_5 = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress('50.50.50.1'),
                router_interface_id=self.egress_rif_5,
                type=SAI_NEXT_HOP_TYPE_MPLS,
                labelstack=label_list_t,
                outseg_type=SAI_OUTSEG_TYPE_SWAP)

            self.mpls_neighbor_entry_5 = sai_thrift_neighbor_entry_t(
                rif_id=self.egress_rif_5,
                ip_address=sai_ipaddress('50.50.50.1'))
            status = sai_thrift_create_neighbor_entry(
                self.client, self.mpls_neighbor_entry_5,
                dst_mac_address='00:11:22:33:44:55',
                no_host_route=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            self.inseg_entry_5000 = sai_thrift_inseg_entry_t(label=5000)
            status = sai_thrift_create_inseg_entry(
                self.client, self.inseg_entry_5000,
                next_hop_id=self.nhop_label_5)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def mplsTransitSwapEcmpHashTest(self):
        '''
        Verify ECMP group with MPLS Transit nhops.
        '''
        print("\nmplsTransitSwapEcmpHashTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='60.60.60.1',
            ip_ttl=64)

        mpls_tag = {'label': 7000, 'ttl': 64, 'tc':  3, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])

        mpls_swap_tag_7070 = {'label': 7070, 'ttl': 63, 'tc':  3, 's':  1}
        mpls_tag_7070 = [mpls_swap_tag_7070]
        mpls_swap_tag_7171 = {'label': 7171, 'ttl': 63, 'tc':  3, 's':  1}
        mpls_tag_7171 = [mpls_swap_tag_7171]
        mpls_swap_tag_7272 = {'label': 7272, 'ttl': 63, 'tc':  3, 's':  1}
        mpls_tag_7272 = [mpls_swap_tag_7272]

        recv_mpls_7070 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_7070,
            inner_frame=send_pkt['IP'])

        recv_mpls_7171 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_7171,
            inner_frame=send_pkt['IP'])

        recv_mpls_7272 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_7272,
            inner_frame=send_pkt['IP'])

        rif_cntr_id = 'SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS'
        rif6_stats_start = sai_thrift_get_router_interface_stats(
            self.client, self.egress_rif_6)
        rif7_stats_start = sai_thrift_get_router_interface_stats(
            self.client, self.egress_rif_7)
        rif8_stats_start = sai_thrift_get_router_interface_stats(
            self.client, self.egress_rif_8)

        print("ECMP - Send MPLS tag packet 7000")
        send_packet(self, self.dev_port10, mpls_pkt)
        self.ingress_rif_in_stats += 1
        verify_any_packet_any_port(
            self, [recv_mpls_7070, recv_mpls_7171, recv_mpls_7272],
            [self.dev_port27, self.dev_port28, self.dev_port29])
        rif6_stats_end = sai_thrift_get_router_interface_stats(
            self.client, self.egress_rif_6)
        rif7_stats_end = sai_thrift_get_router_interface_stats(
            self.client, self.egress_rif_7)
        rif8_stats_end = sai_thrift_get_router_interface_stats(
            self.client, self.egress_rif_8)

        if (rif6_stats_end[rif_cntr_id] -
                rif6_stats_start[rif_cntr_id] == 1):
            final_egress_rif = self.egress_rif_6
        if (rif7_stats_end[rif_cntr_id] -
                rif7_stats_start[rif_cntr_id] == 1):
            final_egress_rif = self.egress_rif_7
        if (rif8_stats_end[rif_cntr_id] -
                rif8_stats_start[rif_cntr_id] == 1):
            final_egress_rif = self.egress_rif_8
        else:
            final_egress_rif = 0

        iter_count = 10
        rif_lb_start_count = sai_thrift_get_router_interface_stats(
            self.client, final_egress_rif)

        for _ in range(0, iter_count):
            l4_sport = random.randint(5000, 65535)
            l4_dport = random.randint(5000, 65535)
            mpls_pkt['TCP'].sport = l4_sport
            mpls_pkt['TCP'].dport = l4_dport
            send_packet(self, self.dev_port10, mpls_pkt)
            self.ingress_rif_in_stats += 1

        time.sleep(2)
        rif_lb_end_count = sai_thrift_get_router_interface_stats(
            self.client, final_egress_rif)
        diff = (rif_lb_end_count[rif_cntr_id] -
                rif_lb_start_count[rif_cntr_id])
        self.assertEqual(diff, iter_count)

        self._verifyStats()

    def mplsTransitPushTest(self):
        '''
        Verify MPLS label is pushed on stack in transit LSR.
        '''
        print("\nmplsTransitPushTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='60.60.60.10',
            ip_ttl=64)

        mpls_tag = {'label': 8000, 'ttl': 64, 'tc':  3, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])

        mpls_push_tag = {'label': 9000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_list = [mpls_push_tag, mpls_tag]
        recv_mpls_pkt = simple_mpls_packet(
            eth_dst="00:11:22:33:44:55",
            eth_src=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])

        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_mpls_pkt, self.dev_port30)
        self.ingress_rif_in_stats += 1
        self.egress_rif_9_out_stats += 1

        self.assertTrue(self._verifyStats())


@group("draft")
class MplsCreationTest(SaiHelper):
    ''' Basic MPLS creation test class '''
    def runTest(self):
        self.mplsCreateAndRemoveTest()
        self.mplsAttributesTest()
        self.mplsCreateTheSameEntryTest()

    def mplsCreateAndRemoveTest(self):
        ''' Test creates and removes several MPLS entries in loop. '''
        print("\nmplsCreateAndRemoveTest()")

        for _ in range(0, 5):
            inseg_entry = sai_thrift_inseg_entry_t(label=200)
            status = sai_thrift_create_inseg_entry(self.client, inseg_entry)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            status = sai_thrift_remove_inseg_entry(self.client, inseg_entry)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def mplsAttributesTest(self):
        '''
        Verify set and get operations on MPLS attributes.
        '''
        print("\nmplsAttributesTest()")

        num_of_pop = 2
        packet_action = SAI_PACKET_ACTION_DROP
        next_hop_id = self.port10_rif

        inseg_entry = sai_thrift_inseg_entry_t(label=200)
        status = sai_thrift_create_inseg_entry(
            self.client, inseg_entry,
            num_of_pop=num_of_pop,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE,
            packet_action=packet_action,
            next_hop_id=next_hop_id)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        try:
            attr = sai_thrift_get_inseg_entry_attribute(
                self.client, inseg_entry, packet_action=True)
            self.assertEqual(attr['packet_action'], packet_action)
            print(attr)

            attr = sai_thrift_get_inseg_entry_attribute(
                self.client, inseg_entry, num_of_pop=True)
            self.assertEqual(attr['num_of_pop'], num_of_pop)
            print(attr)

            attr = sai_thrift_get_inseg_entry_attribute(
                self.client, inseg_entry, next_hop_id=True)
            self.assertEqual(attr['next_hop_id'], next_hop_id)
            print(attr)

            status = sai_thrift_remove_inseg_entry(self.client, inseg_entry)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            inseg_entry = sai_thrift_inseg_entry_t(label=200)
            status = sai_thrift_create_inseg_entry(self.client, inseg_entry)

            sai_thrift_set_inseg_entry_attribute(
                self.client, inseg_entry, packet_action=packet_action)
            attr = sai_thrift_get_inseg_entry_attribute(
                self.client, inseg_entry, packet_action=True)
            self.assertEqual(attr['packet_action'], packet_action)
            print(attr)

            sai_thrift_set_inseg_entry_attribute(
                self.client, inseg_entry, num_of_pop=num_of_pop)
            attr = sai_thrift_get_inseg_entry_attribute(
                self.client, inseg_entry, num_of_pop=True)
            self.assertEqual(attr['num_of_pop'], num_of_pop)
            print(attr)

            sai_thrift_set_inseg_entry_attribute(
                self.client, inseg_entry, next_hop_id=next_hop_id)
            attr = sai_thrift_get_inseg_entry_attribute(
                self.client, inseg_entry, next_hop_id=True)
            self.assertEqual(attr['next_hop_id'], next_hop_id)
            print(attr)

        finally:
            status = sai_thrift_remove_inseg_entry(self.client, inseg_entry)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def mplsCreateTheSameEntryTest(self):
        '''
        Verify it is not possible to create duplicated MPLS entry.
        '''
        print("\nmplsCreateTheSameEntryTest()")

        try:
            inseg_entry = sai_thrift_inseg_entry_t(label=200)
            status = sai_thrift_create_inseg_entry(self.client, inseg_entry)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            inseg_entry = sai_thrift_inseg_entry_t(label=200)
            status = sai_thrift_create_inseg_entry(self.client, inseg_entry)
            self.assertNotEqual(status, SAI_STATUS_SUCCESS)

        finally:
            status = sai_thrift_remove_inseg_entry(self.client, inseg_entry)
            self.assertEqual(status, SAI_STATUS_SUCCESS)


@group("draft")
class MplsObjectsAvailabilityTest(SaiHelperBase):
    ''' CRM objects availability verification class '''
    def runTest(self):
        print("\nMplsObjectsAvailabilityTest()")

        no_of_objects = random.randint(10, 100)
        label = 100
        inseg_entry_list = []

        try:
            mpls_rif = sai_thrift_create_router_interface(
                self.client,
                type=SAI_ROUTER_INTERFACE_TYPE_MPLS_ROUTER,
                virtual_router_id=self.default_vrf)

            obj_avail_before = sai_thrift_object_type_get_availability(
                self.client,
                obj_type=SAI_OBJECT_TYPE_INSEG_ENTRY)

            print("%d Inseg entries available\n"
                  "Creating a random number of Inseg entries (%d)"
                  % (obj_avail_before, no_of_objects))

            for _ in range(no_of_objects):
                inseg_entry = sai_thrift_inseg_entry_t(label=label)

                status = sai_thrift_create_inseg_entry(
                    self.client, inseg_entry,
                    next_hop_id=mpls_rif)
                self.assertEqual(status, SAI_STATUS_SUCCESS)

                inseg_entry_list.append(inseg_entry)
                label += 1

            obj_avail_after = sai_thrift_object_type_get_availability(
                self.client,
                obj_type=SAI_OBJECT_TYPE_INSEG_ENTRY)

            self.assertEqual(obj_avail_before,
                             obj_avail_after + no_of_objects)

            print("%d objects available after creation of %d Inseg entries"
                  % (obj_avail_after, no_of_objects))

        finally:
            for inseg_entry in inseg_entry_list:
                sai_thrift_remove_inseg_entry(self.client, inseg_entry)
            sai_thrift_remove_router_interface(self.client, mpls_rif)


@group("draft")
class MplsIpv6TtlModeTest(SaiHelper):
    '''
    This class contains tests of the ttl_mode for IPv6 MPLS packets,
    focusing mostly on the uniform mode.
    Note that the pipe mode is covered in more detail in MplsIpv6Test.
    '''
    def setUp(self):
        super(MplsIpv6TtlModeTest, self).setUp()

        ttl_value = 63
        exp_value = 5
        ipv6_1 = '100::1'
        ipv6_2 = '100::2'
        ipv6_3 = '100::3'
        ipv6_4 = '220::1'
        ipv6_5 = '330::1'
        ipv6_10 = '800::1'

        ipv6_subnet_1 = '110::1/128'
        ipv6_subnet_2 = '120::1/128'
        ipv6_subnet_3 = '130::1/128'
        ipv6_subnet_4 = '200::1/128'
        dmac = '00:11:22:33:44:55'

        # Ingress LER config
        self.ingress_rif = self.port10_rif
        self.egress_rif_1 = self.port11_rif

        label_list = [1000]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_1),
            router_interface_id=self.egress_rif_1,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_UNIFORM,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        label_list.append(2000)
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_2),
            router_interface_id=self.egress_rif_1,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_UNIFORM,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        label_list.append(3000)
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_3),
            router_interface_id=self.egress_rif_1,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_UNIFORM,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        # note: neighbor with the same dst_ip as MPLS nexthop has to be created
        # after MPLS nexthop is created,
        # if not it will create second nexthop of type IP with this dst_ip
        # and neither of them will be functioning correctly - packets
        # will be gleaned to CPU
        self.mpls_neighbor_entry_1 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_1, ip_address=sai_ipaddress(ipv6_1))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_1, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_neighbor_entry_2 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_1, ip_address=sai_ipaddress(ipv6_2))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_2, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_neighbor_entry_3 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_1, ip_address=sai_ipaddress(ipv6_3))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_3, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_encap_route_1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv6_subnet_1))
        sai_thrift_create_route_entry(
            self.client, self.mpls_encap_route_1,
            next_hop_id=self.nhop_label_1)

        self.mpls_encap_route_2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv6_subnet_2))
        sai_thrift_create_route_entry(
            self.client, self.mpls_encap_route_2,
            next_hop_id=self.nhop_label_2)

        self.mpls_encap_route_3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv6_subnet_3))
        sai_thrift_create_route_entry(
            self.client, self.mpls_encap_route_3,
            next_hop_id=self.nhop_label_3)

        # MPLS egress LER term config
        self.egress_rif_2 = self.port12_rif

        self.mpls_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_MPLS_ROUTER,
            virtual_router_id=self.default_vrf)

        self.nhop_1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_4),
            router_interface_id=self.egress_rif_2,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_1 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_2, ip_address=sai_ipaddress(ipv6_4))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry_1, dst_mac_address=dmac,
            no_host_route=True)
        self.route_entry_1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv6_subnet_4))
        sai_thrift_create_route_entry(
            self.client, self.route_entry_1, next_hop_id=self.nhop_1)

        self.inseg_entry_1000 = sai_thrift_inseg_entry_t(label=1000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_1000,
            num_of_pop=1,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.mpls_rif)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Egress LER Null Term configs
        self.inseg_entry_0 = sai_thrift_inseg_entry_t(label=0)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_0,
            num_of_pop=1,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.mpls_rif)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Egress PHP configs
        self.egress_rif_3 = self.port13_rif

        self.nhop_2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_4),
            router_interface_id=self.egress_rif_3,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.neighbor_entry_2 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_3, ip_address=sai_ipaddress(ipv6_4))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry_2, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_2000 = sai_thrift_inseg_entry_t(label=2000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_2000,
            num_of_pop=1,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.nhop_2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_2111 = sai_thrift_inseg_entry_t(label=2111)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_2111,
            num_of_pop=2,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.nhop_2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_2222 = sai_thrift_inseg_entry_t(label=2222)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_2222,
            num_of_pop=3,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.nhop_2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Egress PHP Swap Null configs
        self.egress_rif_4 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port25,
            virtual_router_id=self.default_vrf)

        label_list = [0]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_4 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_5),
            router_interface_id=self.egress_rif_4,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_UNIFORM,
            outseg_ttl_value=ttl_value,
            outseg_type=SAI_OUTSEG_TYPE_SWAP)

        self.mpls_neighbor_entry_4 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_4, ip_address=sai_ipaddress(ipv6_5))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_4, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_3000 = sai_thrift_inseg_entry_t(label=3000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_3000,
            next_hop_id=self.nhop_label_4)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Transit Push configs
        self.egress_rif_9 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port30,
            virtual_router_id=self.default_vrf)

        label_list = [9000]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_9 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv6_10),
            router_interface_id=self.egress_rif_9,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_UNIFORM,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        self.mpls_neighbor_entry_9 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_9, ip_address=sai_ipaddress(ipv6_10))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_9, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_8000 = sai_thrift_inseg_entry_t(label=8000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_8000,
            next_hop_id=self.nhop_label_9)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        self.mplsIngressLERTtlModeTest()
        self.mplsEgressLERTermTtlModeTest()
        self.mplsEgressLERNullTermTtlModeTest()
        self.mplsEgressPhpTtlModeTest()

    def tearDown(self):
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_8000)
        sai_thrift_remove_neighbor_entry(
            self.client, self.mpls_neighbor_entry_9)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_9)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_9)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_3000)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_4)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_4)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_4)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_2222)
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_2111)
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_2000)

        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry_2)
        sai_thrift_remove_next_hop(self.client, self.nhop_2)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_0)
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_1000)

        sai_thrift_remove_route_entry(self.client, self.route_entry_1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry_1)
        sai_thrift_remove_next_hop(self.client, self.nhop_1)

        sai_thrift_remove_router_interface(self.client, self.mpls_rif)

        sai_thrift_remove_route_entry(self.client, self.mpls_encap_route_1)
        sai_thrift_remove_route_entry(self.client, self.mpls_encap_route_2)
        sai_thrift_remove_route_entry(self.client, self.mpls_encap_route_3)

        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_1)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_2)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_3)

        sai_thrift_remove_next_hop(self.client, self.nhop_label_1)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_2)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_3)

        super(MplsIpv6TtlModeTest, self).tearDown()

    def mplsIngressLERTtlModeTest(self):
        '''
        Verify if MPLS labels are added to packet in ingress LER
        using TTL values from the incoming IPv6 packets.
        '''
        print("\nmplsIngressLERTtlModeTest()")

        send_pkt_1 = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='110::1',
            ipv6_hlim=58)
        send_pkt_2 = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='120::1',
            ipv6_hlim=60)
        send_pkt_3 = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='130::1',
            ipv6_hlim=62)
        exp_pkt_1 = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='110::1',
            ipv6_hlim=57)
        exp_pkt_2 = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='120::1',
            ipv6_hlim=59)
        exp_pkt_3 = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='130::1',
            ipv6_hlim=61)
        mpls_tag_list = []

        mpls_tag_1 = {'label': 1000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_2 = {'label': 2000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_3 = {'label': 3000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_1['s'] = 1
        mpls_tag_1['ttl'] = 57
        mpls_tag_list.append(mpls_tag_1)
        mpls_pkt_label_1 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_1['IPv6'])

        mpls_tag_1['s'] = 0
        mpls_tag_2['s'] = 1
        mpls_tag_1['ttl'] = 59
        mpls_tag_2['ttl'] = 59
        del mpls_tag_list[:]
        mpls_tag_list.append(mpls_tag_1)
        mpls_tag_list.append(mpls_tag_2)
        mpls_pkt_label_2 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_2['IPv6'])

        mpls_tag_1['s'] = 0
        mpls_tag_2['s'] = 0
        mpls_tag_3['s'] = 1
        mpls_tag_1['ttl'] = 61
        mpls_tag_2['ttl'] = 61
        mpls_tag_3['ttl'] = 61
        del mpls_tag_list[:]
        mpls_tag_list.append(mpls_tag_1)
        mpls_tag_list.append(mpls_tag_2)
        mpls_tag_list.append(mpls_tag_3)
        mpls_pkt_label_3 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_3['IPv6'])

        mpls_tag_1['ttl'] = 63
        mpls_tag_2['ttl'] = 63
        mpls_tag_3['ttl'] = 63
        del mpls_tag_list[:]
        mpls_tag_list.append(mpls_tag_1)
        mpls_tag_list.append(mpls_tag_2)
        mpls_tag_list.append(mpls_tag_3)
        mpls_pkt_label_4 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_3['IPv6'])

        mpls_tag_1['ttl'] = 32
        mpls_tag_2['ttl'] = 32
        mpls_tag_3['ttl'] = 32
        del mpls_tag_list[:]
        mpls_tag_list.append(mpls_tag_1)
        mpls_tag_list.append(mpls_tag_2)
        mpls_tag_list.append(mpls_tag_3)
        mpls_pkt_label_5 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_3['IPv6'])

        print("Send ip packet to add one MPLS label 1000")
        send_packet(self, self.dev_port10, send_pkt_1)
        verify_packet(self, mpls_pkt_label_1, self.dev_port11)

        print("Send ip packet to add two MPLS label stack - 1000, 2000")
        send_packet(self, self.dev_port10, send_pkt_2)
        verify_packet(self, mpls_pkt_label_2, self.dev_port11)

        print("Send ip packet to add three MPLS label stack - "
              "1000, 2000, 3000")
        send_packet(self, self.dev_port10, send_pkt_3)
        verify_packet(self, mpls_pkt_label_3, self.dev_port11)

        sai_thrift_set_next_hop_attribute(
            self.client, self.nhop_label_3,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_PIPE)

        print("Change nexthop to pipe mode and send 3 label ip packet - "
              "1000, 2000, 3000")
        send_packet(self, self.dev_port10, send_pkt_3)
        verify_packet(self, mpls_pkt_label_4, self.dev_port11)

        sai_thrift_set_next_hop_attribute(
            self.client, self.nhop_label_3,
            outseg_ttl_value=32)

        print("Change nexthop ttl value to 32 and send 3 label ip packet "
              "- 1000, 2000, 3000")
        send_packet(self, self.dev_port10, send_pkt_3)
        verify_packet(self, mpls_pkt_label_5, self.dev_port11)

        sai_thrift_set_next_hop_attribute(
            self.client, self.nhop_label_3,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_UNIFORM)

        print("Change nexthop to uniform mode and send 3 label ip packet "
              "- 1000, 2000, 3000")
        send_packet(self, self.dev_port10, send_pkt_3)
        verify_packet(self, mpls_pkt_label_3, self.dev_port11)

    def mplsEgressLERTermTtlModeTest(self):
        '''
        Verify if MPLS label is popped in Egress LER and packet is forwarded
        based on IP lookup using the TTL value from the popped MPLS label.
        '''
        print("\nmplsEgressLERTermTtlModeTest()")

        send_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='200::1',
            ipv6_hlim=64)
        mpls_tag = {'label': 1000, 'ttl': 55, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IPv6'])
        recv_pkt = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='200::1',
            ipv6_hlim=54)

        print("Send MPLS tag packet with label 1000 - term and IP lookup")
        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_pkt, self.dev_port12)

        sai_thrift_set_inseg_entry_attribute(
            self.client, self.inseg_entry_1000,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE)

        recv_pkt['IPv6'].hlim = 63
        print("Send MPLS tag packet after changing ttl mode to pipe")
        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_pkt, self.dev_port12)

        sai_thrift_set_inseg_entry_attribute(
            self.client, self.inseg_entry_1000,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM)

        recv_pkt['IPv6'].hlim = 54
        print("Send MPLS tag packet after changing ttl mode to uniform")
        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_pkt, self.dev_port12)

    def mplsEgressLERNullTermTtlModeTest(self):
        '''
        Verify if MPLS null label is popped in Egress LER and packet
        is forwarded based on IP lookup using the TTL value from the
        popped MPLS label.
        '''
        print("\nmplsEgressLERNullTermTtlModeTest()")

        send_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='200::1',
            ipv6_hlim=64)
        mpls_tag = {'label': 0, 'ttl': 60, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IPv6'])
        recv_pkt = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='200::1',
            ipv6_hlim=59)

        print("Send MPLS tag packet with label 0 - term and IP lookup")
        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_pkt, self.dev_port12)

    def mplsEgressPhpTtlModeTest(self):
        '''
        Verify PHP pops label and forwards packet using TTL
        value from the top label in the stack.
        '''
        print("\nmplsEgressPhpTtlModeTest()")

        send_pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='300::1',
            ipv6_hlim=64)

        mpls_tag = {'label': 2000, 'ttl': 60, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IPv6'])

        mpls_tag_2 = {'label': 2111, 'ttl': 58, 'tc':  0, 's':  0}
        mpls_tag_list_2 = [mpls_tag_2, mpls_tag]
        mpls_tag_2_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list_2,
            inner_frame=send_pkt['IPv6'])

        mpls_tag_3 = {'label': 2222, 'ttl': 56, 'tc':  0, 's':  0}
        mpls_tag_list_3 = [mpls_tag_3, mpls_tag_2, mpls_tag]
        mpls_tag_3_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list_3,
            inner_frame=send_pkt['IPv6'])

        recv_pkt = simple_tcpv6_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ipv6_dst='300::1',
            ipv6_hlim=63)
        
        mpls_tag['ttl'] = 60
        recv_mpls_tag_list = [mpls_tag]
        recv_mpls_pkt_2 = simple_mpls_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            mpls_tags=recv_mpls_tag_list,
            inner_frame=send_pkt['IPv6'])

        try:
            print("Send MPLS tag packet with label 2000 - "
                  "PHP and forward IP packet")
            recv_pkt['IPv6'].hlim = 59
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_packet(self, recv_pkt, self.dev_port13)

            print("Send MPLS tag packet with labels - 2111, 2000 - "
                  "PHP and forward IP packet")
            recv_pkt['IPv6'].hlim = 57
            send_packet(self, self.dev_port10, mpls_tag_2_pkt)
            verify_packet(self, recv_pkt, self.dev_port13)

            print("Send MPLS tag packet with labels - 2222, 2111, 2000 - "
                  "PHP and forward IP packet")
            recv_pkt['IPv6'].hlim = 55
            send_packet(self, self.dev_port10, mpls_tag_3_pkt)
            verify_packet(self, recv_pkt, self.dev_port13)

            sai_thrift_set_inseg_entry_attribute(
                self.client, self.inseg_entry_2222,
                pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE)

            print("Set MPLS object label 2222 to pop only 2 labels")
            sai_thrift_set_inseg_entry_attribute(
                self.client, self.inseg_entry_2222, num_of_pop=2)
            print("Send MPLS tag packet with labels - 2222, 2111, 2000 - "
                  "Pop 2 labels and forward MPLS with 2000")
            send_packet(self, self.dev_port10, mpls_tag_3_pkt)
            verify_packet(self, recv_mpls_pkt_2, self.dev_port13)

        finally:
            sai_thrift_set_inseg_entry_attribute(
                self.client, self.inseg_entry_2222, num_of_pop=3)


@group("draft")
class MplsIpv4TtlModeTest(SaiHelper):
    '''
    This class contains tests of the ttl_mode for IPv4 MPLS packets,
    focusing mostly on the uniform mode.
    Note that the pipe mode is covered in more detail in MplsIpv4Test.
    '''
    def setUp(self):
        super(MplsIpv4TtlModeTest, self).setUp()

        ttl_value = 63
        exp_value = 5
        ipv4_1 = '10.10.10.1'
        ipv4_2 = '10.10.10.2'
        ipv4_3 = '10.10.10.3'
        ipv4_4 = '20.20.20.1'
        ipv4_5 = '30.30.30.1'
        ipv4_10 = '80.80.80.1'

        ipv4_subnet_1 = '10.10.10.1/32'
        ipv4_subnet_2 = '10.10.10.2/32'
        ipv4_subnet_3 = '10.10.10.3/32'
        ipv4_subnet_4 = '20.20.20.1/24'
        dmac = '00:11:22:33:44:55'

        # Ingress LER config
        self.ingress_rif = self.port10_rif
        self.egress_rif_1 = self.port11_rif

        label_list = [1000]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_1),
            router_interface_id=self.egress_rif_1,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_UNIFORM,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        label_list.append(2000)
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_2),
            router_interface_id=self.egress_rif_1,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_UNIFORM,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        label_list.append(3000)
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_3),
            router_interface_id=self.egress_rif_1,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_UNIFORM,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        # note: neighbor with the same dst_ip as MPLS nexthop has to be created
        # after MPLS nexthop is created,
        # if not it will create second nexthop of type IP with this dst_ip
        # and neither of them will be functioning correctly - packets
        # will be gleaned to CPU
        self.mpls_neighbor_entry_1 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_1, ip_address=sai_ipaddress(ipv4_1))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_1, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_neighbor_entry_2 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_1, ip_address=sai_ipaddress(ipv4_2))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_2, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_neighbor_entry_3 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_1, ip_address=sai_ipaddress(ipv4_3))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_3, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.mpls_encap_route_1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv4_subnet_1))
        sai_thrift_create_route_entry(
            self.client, self.mpls_encap_route_1,
            next_hop_id=self.nhop_label_1)

        self.mpls_encap_route_2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv4_subnet_2))
        sai_thrift_create_route_entry(
            self.client, self.mpls_encap_route_2,
            next_hop_id=self.nhop_label_2)

        self.mpls_encap_route_3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv4_subnet_3))
        sai_thrift_create_route_entry(
            self.client, self.mpls_encap_route_3,
            next_hop_id=self.nhop_label_3)

        # MPLS egress LER term config
        self.egress_rif_2 = self.port12_rif

        self.mpls_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_MPLS_ROUTER,
            virtual_router_id=self.default_vrf)

        self.nhop_1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_4),
            router_interface_id=self.egress_rif_2,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry_1 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_2, ip_address=sai_ipaddress(ipv4_4))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry_1, dst_mac_address=dmac,
            no_host_route=True)
        self.route_entry_1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(ipv4_subnet_4))
        sai_thrift_create_route_entry(
            self.client, self.route_entry_1, next_hop_id=self.nhop_1)

        self.inseg_entry_1000 = sai_thrift_inseg_entry_t(label=1000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_1000,
            num_of_pop=1,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.mpls_rif)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Egress LER Null Term configs
        self.inseg_entry_0 = sai_thrift_inseg_entry_t(label=0)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_0,
            num_of_pop=1,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.mpls_rif)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Egress PHP configs
        self.egress_rif_3 = self.port13_rif

        self.nhop_2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_4),
            router_interface_id=self.egress_rif_3,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.neighbor_entry_2 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_3, ip_address=sai_ipaddress(ipv4_4))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry_2, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_2000 = sai_thrift_inseg_entry_t(label=2000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_2000,
            num_of_pop=1,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.nhop_2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_2111 = sai_thrift_inseg_entry_t(label=2111)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_2111,
            num_of_pop=2,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.nhop_2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_2222 = sai_thrift_inseg_entry_t(label=2222)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_2222,
            num_of_pop=3,
            pop_ttl_mode=SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,
            next_hop_id=self.nhop_2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Egress PHP Swap Null configs
        self.egress_rif_4 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port25,
            virtual_router_id=self.default_vrf)

        label_list = [0]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_4 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_5),
            router_interface_id=self.egress_rif_4,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_UNIFORM,
            outseg_ttl_value=ttl_value,
            outseg_type=SAI_OUTSEG_TYPE_SWAP)

        self.mpls_neighbor_entry_4 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_4, ip_address=sai_ipaddress(ipv4_5))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_4, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_3000 = sai_thrift_inseg_entry_t(label=3000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_3000,
            next_hop_id=self.nhop_label_4)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # MPLS Transit Push configs
        self.egress_rif_9 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port30,
            virtual_router_id=self.default_vrf)

        label_list = [9000]
        label_list_t = sai_thrift_u32_list_t(
            count=len(label_list),
            uint32list=label_list)

        self.nhop_label_9 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ipv4_10),
            router_interface_id=self.egress_rif_9,
            type=SAI_NEXT_HOP_TYPE_MPLS,
            labelstack=label_list_t,
            outseg_ttl_mode=SAI_OUTSEG_TTL_MODE_UNIFORM,
            outseg_ttl_value=ttl_value,
            outseg_exp_value=exp_value,
            outseg_type=SAI_OUTSEG_TYPE_PUSH)

        self.mpls_neighbor_entry_9 = sai_thrift_neighbor_entry_t(
            rif_id=self.egress_rif_9, ip_address=sai_ipaddress(ipv4_10))
        status = sai_thrift_create_neighbor_entry(
            self.client, self.mpls_neighbor_entry_9, dst_mac_address=dmac,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.inseg_entry_8000 = sai_thrift_inseg_entry_t(label=8000)
        status = sai_thrift_create_inseg_entry(
            self.client, self.inseg_entry_8000,
            next_hop_id=self.nhop_label_9)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        self.mplsIngressLERTtlModeTest()
        self.mplsEgressLERTermTtlModeTest()
        self.mplsEgressLERNullTermTtlModeTest()
        self.mplsEgressPhpTtlModeTest()

    def tearDown(self):
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_8000)
        sai_thrift_remove_neighbor_entry(
            self.client, self.mpls_neighbor_entry_9)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_9)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_9)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_3000)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_4)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_4)
        sai_thrift_remove_router_interface(self.client, self.egress_rif_4)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_2222)
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_2111)
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_2000)

        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry_2)
        sai_thrift_remove_next_hop(self.client, self.nhop_2)

        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_0)
        sai_thrift_remove_inseg_entry(self.client, self.inseg_entry_1000)

        sai_thrift_remove_route_entry(self.client, self.route_entry_1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry_1)
        sai_thrift_remove_next_hop(self.client, self.nhop_1)

        sai_thrift_remove_router_interface(self.client, self.mpls_rif)

        sai_thrift_remove_route_entry(self.client, self.mpls_encap_route_1)
        sai_thrift_remove_route_entry(self.client, self.mpls_encap_route_2)
        sai_thrift_remove_route_entry(self.client, self.mpls_encap_route_3)

        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_1)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_2)
        sai_thrift_remove_neighbor_entry(self.client,
                                         self.mpls_neighbor_entry_3)

        sai_thrift_remove_next_hop(self.client, self.nhop_label_1)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_2)
        sai_thrift_remove_next_hop(self.client, self.nhop_label_3)

        super(MplsIpv4TtlModeTest, self).tearDown()

    def mplsIngressLERTtlModeTest(self):
        '''
        Verify if MPLS labels are added to packet in ingress LER
        using TTL values from the incoming IPv4 packets.
        '''
        print("\nmplsIngressLERTtlModeTest()")

        send_pkt_1 = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='10.10.10.1',
            ip_ttl=58)
        send_pkt_2 = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='10.10.10.2',
            ip_ttl=60)
        send_pkt_3 = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='10.10.10.3',
            ip_ttl=62)
        exp_pkt_1 = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='10.10.10.1',
            ip_ttl=57)
        exp_pkt_2 = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='10.10.10.2',
            ip_ttl=59)
        exp_pkt_3 = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='10.10.10.3',
            ip_ttl=61)
        mpls_tag_list = []

        mpls_tag_1 = {'label': 1000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_2 = {'label': 2000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_3 = {'label': 3000, 'ttl': 63, 'tc':  5, 's':  0}
        mpls_tag_1['s'] = 1
        mpls_tag_1['ttl'] = 57
        mpls_tag_list.append(mpls_tag_1)
        mpls_pkt_label_1 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_1['IP'])

        mpls_tag_1['s'] = 0
        mpls_tag_2['s'] = 1
        mpls_tag_1['ttl'] = 59
        mpls_tag_2['ttl'] = 59
        del mpls_tag_list[:]
        mpls_tag_list.append(mpls_tag_1)
        mpls_tag_list.append(mpls_tag_2)
        mpls_pkt_label_2 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_2['IP'])

        mpls_tag_1['s'] = 0
        mpls_tag_2['s'] = 0
        mpls_tag_3['s'] = 1
        mpls_tag_1['ttl'] = 61
        mpls_tag_2['ttl'] = 61
        mpls_tag_3['ttl'] = 61
        del mpls_tag_list[:]
        mpls_tag_list.append(mpls_tag_1)
        mpls_tag_list.append(mpls_tag_2)
        mpls_tag_list.append(mpls_tag_3)
        mpls_pkt_label_3 = simple_mpls_packet(
            eth_src=ROUTER_MAC,
            eth_dst='00:11:22:33:44:55',
            mpls_tags=mpls_tag_list,
            inner_frame=exp_pkt_3['IP'])

        print("Send ip packet to add one MPLS label 1000")
        send_packet(self, self.dev_port10, send_pkt_1)
        verify_packet(self, mpls_pkt_label_1, self.dev_port11)

        print("Send ip packet to add two MPLS label stack - 1000, 2000")
        send_packet(self, self.dev_port10, send_pkt_2)
        verify_packet(self, mpls_pkt_label_2, self.dev_port11)

        print("Send ip packet to add three MPLS label stack - "
              "1000, 2000, 3000")
        send_packet(self, self.dev_port10, send_pkt_3)
        verify_packet(self, mpls_pkt_label_3, self.dev_port11)

    def mplsEgressLERTermTtlModeTest(self):
        '''
        Verify if MPLS label is popped in Egress LER and packet is forwarded
        based on IP lookup using the TTL value from the popped MPLS label.
        '''
        print("\nmplsIngressLERTermTtlModeTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='20.20.20.1',
            ip_ttl=64)
        mpls_tag = {'label': 1000, 'ttl': 55, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])
        recv_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='20.20.20.1',
            ip_ttl=54)

        print("Send MPLS tag packet with label 1000 - term and IP lookup")
        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_pkt, self.dev_port12)

    def mplsEgressLERNullTermTtlModeTest(self):
        '''
        Verify if MPLS null label is popped in Egress LER and packet
        is forwarded based on IP lookup using the TTL value from the
        popped MPLS label.
        '''
        print("\nmplsIngressLERNullTermTtlModeTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='20.20.20.1',
            ip_ttl=64)
        mpls_tag = {'label': 0, 'ttl': 60, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])
        recv_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='20.20.20.1',
            ip_ttl=59)

        print("Send MPLS tag packet with label 0 - term and IP lookup")
        send_packet(self, self.dev_port10, mpls_pkt)
        verify_packet(self, recv_pkt, self.dev_port12)

    def mplsEgressPhpTtlModeTest(self):
        '''
        Verify PHP pops label and forwards packet using TTL value
        from the top label in the stack.
        '''
        print("\nmplsEgressPhpTtlModeTest()")

        send_pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            ip_dst='30.30.30.1',
            ip_ttl=64)

        mpls_tag = {'label': 2000, 'ttl': 60, 'tc':  0, 's':  1}
        mpls_tag_list = [mpls_tag]
        mpls_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list,
            inner_frame=send_pkt['IP'])

        mpls_tag_2 = {'label': 2111, 'ttl': 58, 'tc':  0, 's':  0}
        mpls_tag_list_2 = [mpls_tag_2, mpls_tag]
        mpls_tag_2_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list_2,
            inner_frame=send_pkt['IP'])

        mpls_tag_3 = {'label': 2222, 'ttl': 56, 'tc':  0, 's':  0}
        mpls_tag_list_3 = [mpls_tag_3, mpls_tag_2, mpls_tag]
        mpls_tag_3_pkt = simple_mpls_packet(
            eth_dst=ROUTER_MAC,
            mpls_tags=mpls_tag_list_3,
            inner_frame=send_pkt['IP'])

        recv_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='30.30.30.1',
            ip_ttl=63)
        
        try:
            print("Send MPLS tag pakcet with label 2000 - "
                  "PHP and forward IP packet")
            recv_pkt['IP'].ttl = 59
            send_packet(self, self.dev_port10, mpls_pkt)
            verify_packet(self, recv_pkt, self.dev_port13)

            print("Send MPLS tag pakcet with labels - 2111, 2000 - "
                  "PHP and forward IP packet")
            recv_pkt['IP'].ttl = 57
            send_packet(self, self.dev_port10, mpls_tag_2_pkt)
            verify_packet(self, recv_pkt, self.dev_port13)

            print("Send MPLS tag pakcet with labels - 2222, 2111, 2000 - "
                  "PHP and forward IP packet")
            recv_pkt['IP'].ttl = 55
            send_packet(self, self.dev_port10, mpls_tag_3_pkt)
            verify_packet(self, recv_pkt, self.dev_port13)

        finally:
            sai_thrift_set_inseg_entry_attribute(
                self.client, self.inseg_entry_2222, num_of_pop=3)
