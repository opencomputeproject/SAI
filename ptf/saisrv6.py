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
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, wither express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

'''
Thrift SAI SRv6 tests
'''

import random

from sai_thrift.sai_headers import *

from sai_base_test import *


@group("draft")
class Srv6SrcEncapTest(SaiHelper):
    '''
    SRv6 source encapsulation tests
    '''
    def setUp(self):
        super(Srv6SrcEncapTest, self).setUp()

        self.client_ip_src = '11.0.0.10'
        self.client_ip_dest = '11.0.0.11'
        self.client_ipv6_src = '1100:1001::10'
        self.client_ipv6_dest = '1100:1001::11'
        self.srv6_src_ip = '2001:1001:0:10::1'

        self.seg1 = 'baba:1001:0:10::'
        self.seg2 = 'baba:1001:0:20::'
        self.seg3 = 'baba:1001:0:30::'
        self.seg4 = 'baba:1001:0:40::'
        self.seg5 = 'baba:1001:0:50::'

        self.client_dmac = '00:22:33:44:55:66'
        self.srv6_mac = '00:aa:bb:cc:dd:ee'

        self.ovrf = sai_thrift_create_virtual_router(self.client)
        self.port24_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.ovrf,
            port_id=self.port24)

        self.ip_nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.client_ip_dest),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.nbr_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif,
            ip_address=sai_ipaddress(self.client_ip_dest))
        sai_thrift_create_neighbor_entry(
            self.client, self.nbr_entry, dst_mac_address=self.client_dmac)

        self.route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.client_ip_dest + '/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry, next_hop_id=self.ip_nhop)

        self.urif_lb = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.default_vrf)

        self.sr_tunnel = sai_thrift_create_tunnel(
            self.client,
            type=SAI_TUNNEL_TYPE_SRV6,
            encap_src_ip=sai_ipaddress(self.srv6_src_ip),
            underlay_interface=self.urif_lb,
            encap_ttl_mode=SAI_TUNNEL_TTL_MODE_PIPE_MODEL,
            decap_ttl_mode=SAI_TUNNEL_TTL_MODE_PIPE_MODEL)

        sai_segment_list_1 = sai_thrift_segment_list_t(
            count=1, ip6list=[self.seg1])
        self.sidlist1 = sai_thrift_create_srv6_sidlist(
            self.client,
            type=SAI_SRV6_SIDLIST_TYPE_ENCAPS_RED,
            segment_list=sai_segment_list_1)

        sai_segment_list_2 = sai_thrift_segment_list_t(
            count=2, ip6list=[self.seg2, self.seg1])
        self.sidlist2 = sai_thrift_create_srv6_sidlist(
            self.client,
            type=SAI_SRV6_SIDLIST_TYPE_ENCAPS_RED,
            segment_list=sai_segment_list_2)

        sai_segment_list_3 = sai_thrift_segment_list_t(
            count=3, ip6list=[self.seg3, self.seg2, self.seg1])
        self.sidlist3 = sai_thrift_create_srv6_sidlist(
            self.client,
            type=SAI_SRV6_SIDLIST_TYPE_ENCAPS_RED,
            segment_list=sai_segment_list_3)

        sai_segment_list_4 = sai_thrift_segment_list_t(
            count=1, ip6list=[self.seg4])
        self.sidlist4 = sai_thrift_create_srv6_sidlist(
            self.client,
            type=SAI_SRV6_SIDLIST_TYPE_INSERT_RED,
            segment_list=sai_segment_list_4)

        sai_segment_list_5 = sai_thrift_segment_list_t(
            count=2, ip6list=[self.seg5, self.seg4])
        self.sidlist5 = sai_thrift_create_srv6_sidlist(
            self.client,
            type=SAI_SRV6_SIDLIST_TYPE_INSERT_RED,
            segment_list=sai_segment_list_5)

        self.und_nhop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            ip=sai_ipaddress('20.20.20.20'),
            router_interface_id=self.port12_rif)

        self.und_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.port12_rif, ip_address=sai_ipaddress('20.20.20.20'))
        sai_thrift_create_neighbor_entry(
            self.client, self.und_nbor, dst_mac_address=self.srv6_mac)

        self.und_route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.seg1 + '/64'))
        sai_thrift_create_route_entry(
            self.client, self.und_route_entry1, next_hop_id=self.und_nhop)

        self.und_route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.seg2 + '/64'))
        sai_thrift_create_route_entry(
            self.client, self.und_route_entry2, next_hop_id=self.und_nhop)

        self.und_route_entry3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.seg3 + '/64'))
        sai_thrift_create_route_entry(
            self.client, self.und_route_entry3, next_hop_id=self.und_nhop)

        self.und_route_entry4 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.seg4 + '/64'))
        sai_thrift_create_route_entry(
            self.client, self.und_route_entry4, next_hop_id=self.und_nhop)

        self.und_route_entry5 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.seg5 + '/64'))
        sai_thrift_create_route_entry(
            self.client, self.und_route_entry5, next_hop_id=self.und_nhop)

        self.sr_nhop_sid1 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_SRV6_SIDLIST,
            srv6_sidlist_id=self.sidlist1,
            tunnel_id=self.sr_tunnel)

        self.sr_nhop_sid2 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_SRV6_SIDLIST,
            srv6_sidlist_id=self.sidlist2,
            tunnel_id=self.sr_tunnel)

        self.sr_nhop_sid3 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_SRV6_SIDLIST,
            srv6_sidlist_id=self.sidlist3,
            tunnel_id=self.sr_tunnel)

        self.sr_nhop_sid4 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_SRV6_SIDLIST,
            srv6_sidlist_id=self.sidlist4,
            tunnel_id=self.sr_tunnel)

        self.sr_nhop_sid5 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_SRV6_SIDLIST,
            srv6_sidlist_id=self.sidlist5,
            tunnel_id=self.sr_tunnel)

        self.ecmp_sid_nhop = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.ecmp_nhop1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.ecmp_sid_nhop,
            next_hop_id=self.sr_nhop_sid1)
        self.ecmp_nhop2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.ecmp_sid_nhop,
            next_hop_id=self.sr_nhop_sid2)

        self.over_route_entry = sai_thrift_route_entry_t(
            vr_id=self.ovrf,
            destination=sai_ipprefix(self.client_ip_dest + '/32'))
        sai_thrift_create_route_entry(
            self.client, self.over_route_entry, next_hop_id=self.sr_nhop_sid1)

        self.over_v6_route_entry = sai_thrift_route_entry_t(
            vr_id=self.ovrf,
            destination=sai_ipprefix(self.client_ipv6_dest + '/128'))
        sai_thrift_create_route_entry(self.client,
                                      self.over_v6_route_entry,
                                      next_hop_id=self.sr_nhop_sid1)

        # test packets
        self.pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                     eth_src='00:22:22:22:22:21',
                                     ip_dst=self.client_ip_dest,
                                     ip_src=self.client_ip_src,
                                     ip_id=105,
                                     ip_ttl=64)

        self.v6_pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src='00:22:22:22:22:21',
                                          ipv6_dst=self.client_ipv6_dest,
                                          ipv6_src=self.client_ipv6_src,
                                          ipv6_hlim=64)
        self.exp_v6_pkt = simple_tcpv6_packet(ipv6_dst=self.client_ipv6_dest,
                                              ipv6_src=self.client_ipv6_src,
                                              ipv6_hlim=63)

        self.inner_v4_pkt = simple_tcp_packet(ip_dst=self.client_ip_dest,
                                              ip_src=self.client_ip_src,
                                              ip_id=105,
                                              ip_ttl=63)

    def runTest(self):
        self.clientIpRouteTest()
        self.sourceEncapOneSidTest()
        self.sourceEncapTwoSidTest()
        self.sourceEncapThreeSidTest()
        self.sourceEncapEcmpSidTest()
        self.insertOneSidTest()
        self.insertTwoSidTest()
        self.getSetSidlistTest()

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.over_v6_route_entry)
        sai_thrift_remove_route_entry(self.client, self.over_route_entry)
        sai_thrift_remove_next_hop_group_member(self.client, self.ecmp_nhop2)
        sai_thrift_remove_next_hop_group_member(self.client, self.ecmp_nhop1)
        sai_thrift_remove_next_hop_group(self.client, self.ecmp_sid_nhop)
        sai_thrift_remove_next_hop(self.client, self.sr_nhop_sid5)
        sai_thrift_remove_next_hop(self.client, self.sr_nhop_sid4)
        sai_thrift_remove_next_hop(self.client, self.sr_nhop_sid3)
        sai_thrift_remove_next_hop(self.client, self.sr_nhop_sid2)
        sai_thrift_remove_next_hop(self.client, self.sr_nhop_sid1)
        sai_thrift_remove_route_entry(self.client, self.und_route_entry5)
        sai_thrift_remove_route_entry(self.client, self.und_route_entry4)
        sai_thrift_remove_route_entry(self.client, self.und_route_entry3)
        sai_thrift_remove_route_entry(self.client, self.und_route_entry2)
        sai_thrift_remove_route_entry(self.client, self.und_route_entry1)
        sai_thrift_remove_neighbor_entry(self.client, self.und_nbor)
        sai_thrift_remove_next_hop(self.client, self.und_nhop)
        sai_thrift_remove_srv6_sidlist(self.client, self.sidlist5)
        sai_thrift_remove_srv6_sidlist(self.client, self.sidlist4)
        sai_thrift_remove_srv6_sidlist(self.client, self.sidlist3)
        sai_thrift_remove_srv6_sidlist(self.client, self.sidlist2)
        sai_thrift_remove_srv6_sidlist(self.client, self.sidlist1)
        sai_thrift_remove_tunnel(self.client, self.sr_tunnel)
        sai_thrift_remove_router_interface(self.client, self.urif_lb)
        sai_thrift_remove_route_entry(self.client, self.route_entry)
        sai_thrift_remove_neighbor_entry(self.client, self.nbr_entry)
        sai_thrift_remove_next_hop(self.client, self.ip_nhop)
        sai_thrift_remove_router_interface(self.client, self.port24_rif)
        sai_thrift_remove_virtual_router(self.client, self.ovrf)

        super(Srv6SrcEncapTest, self).tearDown()

    def clientIpRouteTest(self):
        '''
        Verify client IP route forwarding
        '''
        print("\nclientIpRouteTest()")

        exp_pkt = simple_tcp_packet(eth_dst=self.client_dmac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=self.client_ip_dest,
                                    ip_src=self.client_ip_src,
                                    ip_id=105,
                                    ip_ttl=63)
        print("Sending packet on port %d to port %d, forward from %s to %s"
                % (self.dev_port11, self.dev_port10, self.client_ip_src,
                    self.client_ip_dest))
        send_packet(self, self.dev_port11, self.pkt)
        verify_packets(self, exp_pkt, [self.dev_port10])

    def sourceEncapOneSidTest(self):
        '''
        Verify SRv6 source encapsulation with one SID
        '''
        print("\nsourceEncapOneSidTest()")

        sr6_v4_pkt = simple_ipv6ip_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.seg1,
            ipv6_hlim=64,
            inner_frame=self.inner_v4_pkt[IP])
        sr6_v6_pkt = simple_ipv6ip_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.seg1,
            ipv6_hlim=64,
            inner_frame=self.exp_v6_pkt[IPv6])

        print("Sending IPv4 packet - encap with one SID")
        send_packet(self, self.dev_port24, self.pkt)
        verify_packets(self, sr6_v4_pkt, [self.dev_port12])

        print("Sending IPv6 packet - encap with one SID")
        send_packet(self, self.dev_port24, self.v6_pkt)
        verify_packets(self, sr6_v6_pkt, [self.dev_port12])

    def sourceEncapTwoSidTest(self):
        '''
        Verify SRv6 source encapsulation with two SIDs
        '''
        print("\nsourceEncapTwoSidTest()")

        seglist2 = [self.seg2, self.seg1]
        sr6_v4_pkt = simple_ipv6_sr_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=seglist2[0],
            ipv6_hlim=64,
            inner_frame=self.inner_v4_pkt[IP],
            srh_seg_left=len(seglist2) - 1,
            srh_first_seg=len(seglist2) - 2,
            srh_nh=0x4,
            srh_seg_list=[seglist2[1]])

        sr6_v6_pkt = simple_ipv6_sr_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=seglist2[0],
            ipv6_hlim=64,
            inner_frame=self.exp_v6_pkt[IPv6],
            srh_seg_left=len(seglist2) - 1,
            srh_first_seg=len(seglist2) - 2,
            srh_nh=0x29,
            srh_seg_list=[seglist2[1]])

        try:
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_route_entry,
                                                 next_hop_id=self.sr_nhop_sid2)
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_v6_route_entry,
                                                 next_hop_id=self.sr_nhop_sid2)

            print("Sending IPv4 packet - encap with two SIDs")
            send_packet(self, self.dev_port24, self.pkt)
            verify_packets(self, sr6_v4_pkt, [self.dev_port12])

            print("Sending IPv6 packet - encap with two SIDs")
            send_packet(self, self.dev_port24, self.v6_pkt)
            verify_packets(self, sr6_v6_pkt, [self.dev_port12])

        finally:
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_route_entry,
                                                 next_hop_id=self.sr_nhop_sid1)
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_v6_route_entry,
                                                 next_hop_id=self.sr_nhop_sid1)

    def sourceEncapThreeSidTest(self):
        '''
        Verify SRv6 source encapsulation with three SIDs
        '''
        print("\nsourceEncapThreeSidTest()")

        seglist3 = [self.seg3, self.seg2, self.seg1]
        sr6_v4_pkt = simple_ipv6_sr_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=seglist3[0],
            ipv6_hlim=64,
            inner_frame=self.inner_v4_pkt[IP],
            srh_seg_left=len(seglist3) - 1,
            srh_first_seg=len(seglist3) - 2,
            srh_nh=0x4,
            srh_seg_list=[seglist3[2], seglist3[1]])
        sr6_v6_pkt = simple_ipv6_sr_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=seglist3[0],
            ipv6_hlim=64,
            inner_frame=self.exp_v6_pkt[IPv6],
            srh_seg_left=len(seglist3) - 1,
            srh_first_seg=len(seglist3) - 2,
            srh_nh=0x29,
            srh_seg_list=[seglist3[2], seglist3[1]])

        try:
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_route_entry,
                                                 next_hop_id=self.sr_nhop_sid3)
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_v6_route_entry,
                                                 next_hop_id=self.sr_nhop_sid3)

            print("Sending IPv4 packet - encap with three SIDs")
            send_packet(self, self.dev_port24, self.pkt)
            verify_packets(self, sr6_v4_pkt, [self.dev_port12])

            print("Sending IPv6 packet - encap with three SIDs")
            send_packet(self, self.dev_port24, self.v6_pkt)
            verify_packets(self, sr6_v6_pkt, [self.dev_port12])

        finally:
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_route_entry,
                                                 next_hop_id=self.sr_nhop_sid1)
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_v6_route_entry,
                                                 next_hop_id=self.sr_nhop_sid1)

    def sourceEncapEcmpSidTest(self):
        '''
        Verify SRv6 source encapsulation with ECMP SID
        '''
        print("\nsourceEncapEcmpSidTest()")

        send_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                     eth_src='00:22:22:22:22:21',
                                     ip_dst=self.client_ip_dest,
                                     ip_src=self.client_ip_src,
                                     ip_id=105,
                                     ip_ttl=64)
        inner_v4_pkt = simple_tcp_packet(ip_dst=self.client_ip_dest,
                                         ip_src=self.client_ip_src,
                                         ip_id=105,
                                         ip_ttl=63)

        try:
            nhop_13 = sai_thrift_create_next_hop(
                self.client,
                type=SAI_NEXT_HOP_TYPE_IP,
                ip=sai_ipaddress('20.20.20.21'),
                router_interface_id=self.port13_rif)

            nbor_13 = sai_thrift_neighbor_entry_t(
                rif_id=self.port13_rif,
                ip_address=sai_ipaddress('20.20.20.21'))
            sai_thrift_create_neighbor_entry(
                self.client, nbor_13, dst_mac_address=self.srv6_mac)

            sai_thrift_set_route_entry_attribute(
                self.client, self.und_route_entry2, next_hop_id=nhop_13)

            sai_thrift_set_route_entry_attribute(
                self.client,
                self.over_route_entry,
                next_hop_id=self.ecmp_sid_nhop)

            max_iter = 30
            count = [0, 0]
            for i in range(0, max_iter):
                tcp_sport = 2000 + i

                send_pkt['TCP'].sport = tcp_sport
                inner_v4_pkt['TCP'].sport = tcp_sport
                seglist2 = [self.seg2, self.seg1]
                sid1_sr6_pkt = simple_ipv6ip_packet(
                    eth_src=ROUTER_MAC,
                    eth_dst=self.srv6_mac,
                    ipv6_src=self.srv6_src_ip,
                    ipv6_dst=self.seg1,
                    ipv6_hlim=64,
                    inner_frame=inner_v4_pkt[IP])
                sid2_sr6_pkt = simple_ipv6_sr_packet(
                    eth_src=ROUTER_MAC,
                    eth_dst=self.srv6_mac,
                    ipv6_src=self.srv6_src_ip,
                    ipv6_dst=seglist2[0],
                    ipv6_hlim=64,
                    inner_frame=inner_v4_pkt[IP],
                    srh_seg_left=len(seglist2) - 1,
                    srh_first_seg=len(seglist2) - 2,
                    srh_nh=0x4,
                    srh_seg_list=[seglist2[1]])

                send_packet(self, self.dev_port24, send_pkt)
                rcv_index = verify_any_packet_any_port(
                    self,
                    [sid1_sr6_pkt, sid2_sr6_pkt],
                    [self.dev_port12, self.dev_port13])
                count[rcv_index] += 1
            print("Encap ECMP count: ", count)

            for i in range(0, 2):
                self.assertNotEqual(count[i], 0)

        finally:
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_route_entry,
                                                 next_hop_id=self.sr_nhop_sid3)
            sai_thrift_remove_neighbor_entry(self.client, nbor_13)
            sai_thrift_remove_next_hop(self.client, nhop_13)

    def insertOneSidTest(self):
        '''
        Verify SRv6 insert headend operation with a single SID
        '''
        print("\ninsertOneSidTest()")
        seglist4 = [self.seg4]
        sr6_v6_pkt = simple_ipv6_sr_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.client_ipv6_src,
            ipv6_dst=seglist4[0],
            ipv6_hlim=63,
            srh_seg_left=1,
            srh_first_seg=0,
            srh_nh=0x6,  # TCP
            srh_seg_list=[self.client_ipv6_dest],
            inner_frame=self.v6_pkt['TCP'])

        try:
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_v6_route_entry,
                                                 next_hop_id=self.sr_nhop_sid4)

            print("Sending IPv6 packet - insert with one SID")
            send_packet(self, self.dev_port24, self.v6_pkt)
            verify_packets(self, sr6_v6_pkt, [self.dev_port12])

        finally:
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_v6_route_entry,
                                                 next_hop_id=self.sr_nhop_sid1)

    def insertTwoSidTest(self):
        '''
        Verify SRv6 insert headend operation with two SIDs
        '''
        print("\ninsertTwoSidTest()")
        v6_pkt = self.v6_pkt.copy()
        v6_pkt['TCP'].chksum = 0
        seglist5 = [self.seg5, self.seg4]
        sr6_v6_pkt = simple_ipv6_sr_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.client_ipv6_src,
            ipv6_dst=seglist5[0],
            ipv6_hlim=63,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x6,  # TCP
            srh_seg_list=[self.client_ipv6_dest, seglist5[1]],
            inner_frame=v6_pkt['TCP'])

        try:
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_v6_route_entry,
                                                 next_hop_id=self.sr_nhop_sid5)

            print("Sending IPv6 packet - insert with two SIDs")
            send_packet(self, self.dev_port24, v6_pkt)
            verify_packets(self, sr6_v6_pkt, [self.dev_port12])

        finally:
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_v6_route_entry,
                                                 next_hop_id=self.sr_nhop_sid1)

    def getSetSidlistTest(self):
        '''
        Verify getting and setting SRv6 sidlist members
        '''
        print("\ngetSetSidlistTest()")

        new_seg1 = 'cece:f00f:0:10::'
        new_seg2 = 'cece:f00f:0:20::'

        # traffic test doesn't work - received packets have an old sidlist
        sr6_v4_pkt = simple_ipv6_sr_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=new_seg2,
            ipv6_hlim=64,
            inner_frame=self.inner_v4_pkt[IP],
            srh_seg_left=1,
            srh_first_seg=0,
            srh_nh=0x4,
            srh_seg_list=[new_seg1])

        sr6_v6_pkt = simple_ipv6_sr_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=new_seg2,
            ipv6_hlim=64,
            inner_frame=self.exp_v6_pkt[IPv6],
            srh_seg_left=1,
            srh_first_seg=0,
            srh_nh=0x29,
            srh_seg_list=[new_seg1])

        try:
            seglist = sai_thrift_segment_list_t(count=3)
            attr = sai_thrift_get_srv6_sidlist_attribute(
                self.client, self.sidlist3, segment_list=seglist)

            print("Total segments: %d" % (attr["segment_list"].count))
            self.assertEqual(attr["segment_list"].count, 3)

            print(" segment 1 %s" % (attr["segment_list"].ip6list[0]))
            self.assertEqual(attr["segment_list"].ip6list[0], self.seg3)
            print(" segment 2 %s" % (attr["segment_list"].ip6list[1]))
            self.assertEqual(attr["segment_list"].ip6list[1], self.seg2)
            print(" segment 3 %s" % (attr["segment_list"].ip6list[2]))
            self.assertEqual(attr["segment_list"].ip6list[2], self.seg1)

            new_segment_list = sai_thrift_segment_list_t(
                count=2, ip6list=[new_seg2, new_seg1])

            print("Set new segment list")
            sai_thrift_set_srv6_sidlist_attribute(
                self.client, self.sidlist3, segment_list=new_segment_list)

            seglist = sai_thrift_segment_list_t(count=2)
            attr = sai_thrift_get_srv6_sidlist_attribute(
                self.client, self.sidlist3, segment_list=seglist)

            print("Total segments: %d" % (attr["segment_list"].count))
            self.assertEqual(attr["segment_list"].count, 2)

            print(" segment 1 %s" % (attr["segment_list"].ip6list[0]))
            self.assertEqual(attr["segment_list"].ip6list[0], new_seg2)
            print(" segment 2 %s" % (attr["segment_list"].ip6list[1]))
            self.assertEqual(attr["segment_list"].ip6list[1], new_seg1)

            # verify packets are handled with new sidlist
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_route_entry,
                                                 next_hop_id=self.sr_nhop_sid3)
            sai_thrift_set_route_entry_attribute(self.client,
                                                 self.over_v6_route_entry,
                                                 next_hop_id=self.sr_nhop_sid3)

            und_route = sai_thrift_route_entry_t(
                vr_id=self.default_vrf,
                destination=sai_ipprefix(new_seg2 + '/64'))
            sai_thrift_create_route_entry(
                self.client, und_route, next_hop_id=self.und_nhop)

            print("Sending IPv4 packet - encap with two SIDs")
            send_packet(self, self.dev_port24, self.pkt)
            verify_packets(self, sr6_v4_pkt, [self.dev_port12])

            print("Sending IPv6 packet - encap with two SIDs")
            send_packet(self, self.dev_port24, self.v6_pkt)
            verify_packets(self, sr6_v6_pkt, [self.dev_port12])

        finally:
            sai_thrift_remove_route_entry(self.client, und_route)

            sai_segment_list_3 = sai_thrift_segment_list_t(
                count=3, ip6list=[self.seg3, self.seg2, self.seg1])
            sai_thrift_set_srv6_sidlist_attribute(
                self.client, self.sidlist3, segment_list=sai_segment_list_3)


@group("draft")
class Srv6MySidTest(SaiHelper):
    '''
    SRv6 local my SID behaviors tests
    '''
    def setUp(self):
        super(Srv6MySidTest, self).setUp()

        self.node0_prefix_sid = 'baba:1001:0:10::'
        self.node0_t_sid = 'baba:1001:0:10:f::'
        self.node0_dt46_sid1 = 'baba:1001:0:10:df46:1::'
        self.node0_dt46_sid2 = 'baba:1001:0:10:df46:2::'
        self.node0_dt4_sid = 'baba:1001:0:10:df4::'
        self.node0_dt6_sid = 'baba:1001:0:10:df6::'
        self.node0_adj_sid = 'baba:1001:0:10:ad::'
        self.node0_b6_encap_sid = 'baba:1001:0:10::100'
        self.node0_b6_insert_sid = 'baba:1001:0:10::200'
        self.node1_prefix_sid = 'baba:1001:0:20::'
        self.node2_prefix_sid = 'baba:1001:0:30::'
        self.sid_list = [self.node2_prefix_sid, self.node1_prefix_sid]

        self.srv6_mac = '00:aa:bb:cc:dd:ee'
        self.unbor_ip = '100.100.100.100'
        self.srv6_src_ip = '2001:1001:0:10::1'

        # overlay configuration
        self.client_ip_src = '10.10.10.10'
        self.client_ip_dest = '20.20.20.20'
        self.nbor_ip = '30.30.30.30'
        self.xconn_nbor_ip = '40.40.40.40'
        self.client_ipv6_src = '1100:1001::10'
        self.client_ipv6_dest = '1100:1001::20'
        self.client_smac = "00:11:11:11:11:11"
        self.client_dmac = '00:22:22:22:22:22'

        # client route
        self.ip_nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.nbor_ip),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.ip_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif,
            ip_address=sai_ipaddress(self.nbor_ip))
        sai_thrift_create_neighbor_entry(
            self.client, self.ip_nbor, dst_mac_address=self.client_dmac)

        self.ip_route = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.client_ip_dest + '/32'))
        sai_thrift_create_route_entry(
            self.client, self.ip_route, next_hop_id=self.ip_nhop)

        self.ipv6_route = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.client_ipv6_dest + '/128'))
        sai_thrift_create_route_entry(
            self.client, self.ipv6_route, next_hop_id=self.ip_nhop)

        self.urif_lb = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.default_vrf)

        self.sr_tunnel = sai_thrift_create_tunnel(
            self.client,
            type=SAI_TUNNEL_TYPE_SRV6,
            encap_src_ip=sai_ipaddress(self.srv6_src_ip),
            underlay_interface=self.urif_lb,
            encap_ttl_mode=SAI_TUNNEL_TTL_MODE_PIPE_MODEL,
            decap_ttl_mode=SAI_TUNNEL_TTL_MODE_PIPE_MODEL)

        self.und_nhop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            ip=sai_ipaddress(self.unbor_ip),
            router_interface_id=self.port12_rif)

        self.und_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.port12_rif, ip_address=sai_ipaddress(self.unbor_ip))
        sai_thrift_create_neighbor_entry(
            self.client, self.und_nbor, dst_mac_address=self.srv6_mac)

        self.tunnel_route1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.node0_prefix_sid + '/64'))
        sai_thrift_create_route_entry(
            self.client, self.tunnel_route1, next_hop_id=self.und_nhop)

        self.tunnel_route2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.node1_prefix_sid + '/64'))
        sai_thrift_create_route_entry(
            self.client, self.tunnel_route2, next_hop_id=self.und_nhop)

        self.tunnel_route3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.node2_prefix_sid + '/64'))
        sai_thrift_create_route_entry(
            self.client, self.tunnel_route3, next_hop_id=self.und_nhop)

        self.t_vrf_id = sai_thrift_create_virtual_router(self.client)
        self.t_route = sai_thrift_route_entry_t(
            vr_id=self.t_vrf_id,
            destination=sai_ipprefix(self.node1_prefix_sid + '/80'))

        # port 13 cross-connect nhop
        self.xconn_nhop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            ip=sai_ipaddress(self.xconn_nbor_ip),
            router_interface_id=self.port13_rif)

        self.xconn_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.port13_rif,
            ip_address=sai_ipaddress(self.xconn_nbor_ip))
        sai_thrift_create_neighbor_entry(
            self.client, self.xconn_nbor, dst_mac_address=self.srv6_mac)

        self.end_sid_counter = sai_thrift_create_counter(self.client)
        self.end_sid_stats = 0

        # my_sid entries
        end_bf = SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP_AND_USD
        # End SID
        self.end_sid = sai_thrift_my_sid_entry_t(
            sid=self.node0_prefix_sid, vr_id=self.default_vrf)
        sai_thrift_create_my_sid_entry(
            self.client,
            self.end_sid,
            endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_E,
            endpoint_behavior_flavor=end_bf,
            counter_id=self.end_sid_counter)

        # End.T SID
        sai_thrift_create_route_entry(
            self.client, self.t_route, next_hop_id=self.und_nhop)
        self.end_t_sid = sai_thrift_my_sid_entry_t(
            sid=self.node0_t_sid, vr_id=self.default_vrf)
        sai_thrift_create_my_sid_entry(
            self.client,
            self.end_t_sid,
            endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_T,
            vrf=self.t_vrf_id)

        # End.DT46 SIDs
        # default VRF
        self.end_dt46_sid1 = sai_thrift_my_sid_entry_t(
            sid=self.node0_dt46_sid1, vr_id=self.default_vrf)
        sai_thrift_create_my_sid_entry(
            self.client,
            self.end_dt46_sid1,
            vrf=self.default_vrf,
            endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT46)
        # non-default VRF
        self.end_dt46_sid2 = sai_thrift_my_sid_entry_t(
            sid=self.node0_dt46_sid2, vr_id=self.default_vrf)
        sai_thrift_create_my_sid_entry(
            self.client,
            self.end_dt46_sid2,
            vrf=self.t_vrf_id,
            endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT46)

        # End.DT4 SID with non-default VRF
        self.end_dt4_sid = sai_thrift_my_sid_entry_t(
            sid=self.node0_dt4_sid, vr_id=self.default_vrf)
        sai_thrift_create_my_sid_entry(
            self.client,
            self.end_dt4_sid,
            vrf=self.t_vrf_id,
            endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT4)

        # End.DT6 SID with non-default VRF
        self.end_dt6_sid = sai_thrift_my_sid_entry_t(
            sid=self.node0_dt6_sid, vr_id=self.default_vrf)
        sai_thrift_create_my_sid_entry(
            self.client,
            self.end_dt6_sid,
            vrf=self.t_vrf_id,
            endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT6)

        # End.X SID
        self.end_x_sid = sai_thrift_my_sid_entry_t(
            sid=self.node0_adj_sid, vr_id=self.default_vrf)
        sai_thrift_create_my_sid_entry(
            self.client,
            self.end_x_sid,
            vrf=self.default_vrf,
            endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_X,
            next_hop_id=self.xconn_nhop)

        # End.B6.Encaps.Red SID config
        my_encap_sid_list = [self.node2_prefix_sid,
                             self.node1_prefix_sid,
                             self.node0_prefix_sid]
        sai_segment_list1 = sai_thrift_segment_list_t(
            count=2,
            ip6list=my_encap_sid_list[1:])
        self.sidlist1 = sai_thrift_create_srv6_sidlist(
            self.client,
            type=SAI_SRV6_SIDLIST_TYPE_ENCAPS_RED,
            segment_list=sai_segment_list1)

        sai_segment_list_2 = sai_thrift_segment_list_t(
            count=3,
            ip6list=my_encap_sid_list)
        self.sidlist2 = sai_thrift_create_srv6_sidlist(
            self.client,
            type=SAI_SRV6_SIDLIST_TYPE_ENCAPS_RED,
            segment_list=sai_segment_list_2)

        self.sr_nhop1 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_SRV6_SIDLIST,
            srv6_sidlist_id=self.sidlist1,
            tunnel_id=self.sr_tunnel)

        self.sr_nhop2 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_SRV6_SIDLIST,
            srv6_sidlist_id=self.sidlist2,
            tunnel_id=self.sr_tunnel)

        self.end_b6_encap_sid = sai_thrift_my_sid_entry_t(
            sid=self.node0_b6_encap_sid, vr_id=self.default_vrf)
        sai_thrift_create_my_sid_entry(
            self.client,
            self.end_b6_encap_sid,
            vrf=self.default_vrf,
            endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_B6_ENCAPS_RED,
            next_hop_id=self.sr_nhop1)

        # End.B6.Insert.Red SID config
        my_insert_sid_list = [self.node0_prefix_sid,
                              self.node1_prefix_sid]
        sai_segment_list3 = sai_thrift_segment_list_t(
            count=2,
            ip6list=my_insert_sid_list)
        self.sidlist3 = sai_thrift_create_srv6_sidlist(
            self.client,
            type=SAI_SRV6_SIDLIST_TYPE_INSERT_RED,
            segment_list=sai_segment_list3)

        self.sr_nhop3 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_SRV6_SIDLIST,
            srv6_sidlist_id=self.sidlist3,
            tunnel_id=self.sr_tunnel)

        self.end_b6_insert_sid = sai_thrift_my_sid_entry_t(
            sid=self.node0_b6_insert_sid, vr_id=self.default_vrf)
        sai_thrift_create_my_sid_entry(
            self.client,
            self.end_b6_insert_sid,
            vrf=self.default_vrf,
            endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_B6_INSERT_RED,
            next_hop_id=self.sr_nhop3)

        # test packets
        self.inner_v4_pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                              eth_src='00:22:22:22:22:21',
                                              ip_dst=self.client_ip_dest,
                                              ip_src=self.client_ip_src,
                                              ip_id=105,
                                              ip_ttl=64)
        self.v4_exp_pkt = simple_tcp_packet(eth_src=ROUTER_MAC,
                                            eth_dst=self.client_dmac,
                                            ip_dst=self.client_ip_dest,
                                            ip_src=self.client_ip_src,
                                            ip_id=105,
                                            ip_ttl=63)
        self.inner_v6_pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                                eth_src='00:22:22:22:22:21',
                                                ipv6_dst=self.client_ipv6_dest,
                                                ipv6_src=self.client_ipv6_src,
                                                ipv6_hlim=64)
        self.v6_exp_pkt = simple_tcpv6_packet(eth_src=ROUTER_MAC,
                                              eth_dst=self.client_dmac,
                                              ipv6_dst=self.client_ipv6_dest,
                                              ipv6_src=self.client_ipv6_src,
                                              ipv6_hlim=63)

    def runTest(self):
        self.mySidEndTest()
        self.mySidEndTTest()
        self.mySidEndDT46Test()
        self.mySidEndDT4Test()
        self.mySidEndDT6Test()
        self.mySidXConnectTest()
        self.mySidEndDT4ReEncapTest()
        self.mySidEndDT6ReEncapTest()
        self.mySidEndDT46ReEncapTest()
        self.mySidB6EncapTest()
        self.mySidB6InsertTest()
        self.mySidCounterTest()
        self.getSetMySidEntryTest()

    def tearDown(self):
        sai_thrift_remove_my_sid_entry(self.client, self.end_b6_insert_sid)
        sai_thrift_remove_next_hop(self.client, self.sr_nhop3)
        sai_thrift_remove_srv6_sidlist(self.client, self.sidlist3)
        sai_thrift_remove_my_sid_entry(self.client, self.end_b6_encap_sid)
        sai_thrift_remove_next_hop(self.client, self.sr_nhop2)
        sai_thrift_remove_next_hop(self.client, self.sr_nhop1)
        sai_thrift_remove_srv6_sidlist(self.client, self.sidlist2)
        sai_thrift_remove_srv6_sidlist(self.client, self.sidlist1)
        sai_thrift_remove_my_sid_entry(self.client, self.end_x_sid)
        sai_thrift_remove_my_sid_entry(self.client, self.end_dt6_sid)
        sai_thrift_remove_my_sid_entry(self.client, self.end_dt4_sid)
        sai_thrift_remove_my_sid_entry(self.client, self.end_dt46_sid2)
        sai_thrift_remove_my_sid_entry(self.client, self.end_dt46_sid1)
        sai_thrift_remove_my_sid_entry(self.client, self.end_t_sid)
        sai_thrift_remove_my_sid_entry(self.client, self.end_sid)
        sai_thrift_remove_counter(self.client, self.end_sid_counter)
        sai_thrift_remove_neighbor_entry(self.client, self.xconn_nbor)
        sai_thrift_remove_next_hop(self.client, self.xconn_nhop)
        sai_thrift_remove_route_entry(self.client, self.t_route)
        sai_thrift_remove_virtual_router(self.client, self.t_vrf_id)
        sai_thrift_remove_route_entry(self.client, self.tunnel_route3)
        sai_thrift_remove_route_entry(self.client, self.tunnel_route2)
        sai_thrift_remove_route_entry(self.client, self.tunnel_route1)
        sai_thrift_remove_neighbor_entry(self.client, self.und_nbor)
        sai_thrift_remove_next_hop(self.client, self.und_nhop)
        sai_thrift_remove_tunnel(self.client, self.sr_tunnel)
        sai_thrift_remove_router_interface(self.client, self.urif_lb)
        sai_thrift_remove_route_entry(self.client, self.ipv6_route)
        sai_thrift_remove_route_entry(self.client, self.ip_route)
        sai_thrift_remove_neighbor_entry(self.client, self.ip_nbor)
        sai_thrift_remove_next_hop(self.client, self.ip_nhop)

        super(Srv6MySidTest, self).tearDown()

    def mySidEndTest(self):
        '''
        Verify SRv6 End endpoint behavior
        '''
        print("\nmySidEndTest()")

        sr6_more_seg_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_prefix_sid,
            ipv6_hlim=64,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.sid_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt1 = simple_ipv6_sr_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node1_prefix_sid,
            ipv6_hlim=63,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.sid_list,
            inner_frame=self.inner_v4_pkt[IP])

        sr6_one_seg_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_prefix_sid,
            ipv6_hlim=64,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.sid_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt2 = simple_ipv6ip_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node2_prefix_sid,
            ipv6_hlim=63,
            inner_frame=self.inner_v4_pkt[IP])

        sr6_zero_seg_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_prefix_sid,
            ipv6_hlim=64,
            srh_seg_left=0,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.sid_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt3 = simple_tcp_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.client_dmac,
            ip_dst=self.client_ip_dest,
            ip_src=self.client_ip_src,
            ip_id=105,
            ip_ttl=63)

        print("END: Send packet with seg_left > 1")
        send_packet(self, self.dev_port11, sr6_more_seg_pkt)
        verify_packet(self, exp_pkt1, self.dev_port12)
        self.end_sid_stats += 1

        print("END with PSP: Send packet with seg_left == 1")
        send_packet(self, self.dev_port11, sr6_one_seg_pkt)
        verify_packet(self, exp_pkt2, self.dev_port12)
        self.end_sid_stats += 1

        print("END with USD: Send packet with seg_left == 0")
        send_packet(self, self.dev_port11, sr6_zero_seg_pkt)
        verify_packet(self, exp_pkt3, self.dev_port10)
        self.end_sid_stats += 1

    def mySidEndTTest(self):
        '''
        Verify SRv6 End.T endpoint behavior
        '''
        print("\nmySidEndTTest()")

        sr6_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_t_sid,
            ipv6_hlim=64,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.sid_list,
            inner_frame=self.inner_v4_pkt[IP])

        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.srv6_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node1_prefix_sid,
            ipv6_hlim=63,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.sid_list,
            inner_frame=self.inner_v4_pkt[IP])

        print("END.T: Sending packet on port %d to port %d - "
                "forward through another vrf"
                % (self.dev_port11, self.dev_port12))
        send_packet(self, self.dev_port11, sr6_pkt)
        verify_packet(self, exp_pkt, self.dev_port12)


    def mySidEndDT46Test(self):
        '''
        Verify SRv6 End.DT46 endpoint behavior
        '''
        print("\nmySidEndDT46Test()")

        # default VRF
        sr6_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt46_sid1,
            ipv6_hlim=64,
            inner_frame=self.inner_v4_pkt[IP])

        sr6_zero_seg_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt46_sid1,
            ipv6_hlim=64,
            srh_seg_left=0,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=[self.node2_prefix_sid, self.node1_prefix_sid],
            inner_frame=self.inner_v4_pkt[IP])

        sr6_one_seg_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt46_sid1,
            ipv6_hlim=64,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=[self.node2_prefix_sid, self.node1_prefix_sid],
            inner_frame=self.inner_v4_pkt[IP])

        # non-default VRF
        sr6_v6_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt46_sid2,
            ipv6_hlim=64,
            inner_frame=self.inner_v6_pkt[IPv6])

        try:
            # Route entry with non-default VRF for DT.46
            dt46_v6_route_entry = sai_thrift_route_entry_t(
                vr_id=self.t_vrf_id,
                destination=sai_ipprefix(self.client_ipv6_dest + '/128'))
            sai_thrift_create_route_entry(
                self.client, dt46_v6_route_entry, next_hop_id=self.ip_nhop)

            print("DT46: Sending SRv6 packet on port %d to port %d, "
                  "forward from %s to %s"
                  % (self.dev_port11, self.dev_port10, self.srv6_src_ip,
                     self.node0_prefix_sid))
            send_packet(self, self.dev_port11, sr6_pkt)
            verify_packets(self, self.v4_exp_pkt, [self.dev_port10])

            print("DT46: Sending SRv6 packet on port %d to port %d, "
                  "forward from %s to %s"
                  % (self.dev_port11, self.dev_port10, self.srv6_src_ip,
                     self.node1_prefix_sid))
            send_packet(self, self.dev_port11, sr6_v6_pkt)
            verify_packets(self, self.v6_exp_pkt, [self.dev_port10])

            print("DT46: Sending SRv6 packet with SL=0, forward to %s"
                  % (self.dev_port10))
            send_packet(self, self.dev_port11, sr6_zero_seg_pkt)
            verify_packets(self, self.v4_exp_pkt, [self.dev_port10])

            print("DT46: Sending SRv6 packet with SL=1, drop packet")
            send_packet(self, self.dev_port11, sr6_one_seg_pkt)
            verify_no_other_packets(self)

        finally:
            sai_thrift_remove_route_entry(self.client, dt46_v6_route_entry)

    def mySidEndDT6Test(self):
        '''
        Verify SRv6 End.DT6 endpoint behavior
        '''
        print("\nmySidEndDT6Test()")

        # non-default VRF
        sr6_v6_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt6_sid,
            ipv6_hlim=64,
            inner_frame=self.inner_v6_pkt[IPv6])

        sr6_zero_seg_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt6_sid,
            ipv6_hlim=64,
            srh_seg_left=0,
            srh_first_seg=1,
            srh_nh=0x29,
            srh_seg_list=[self.node2_prefix_sid, self.node1_prefix_sid],
            inner_frame=self.inner_v6_pkt[IPv6])

        sr6_one_seg_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt6_sid,
            ipv6_hlim=64,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x29,
            srh_seg_list=[self.node2_prefix_sid, self.node1_prefix_sid],
            inner_frame=self.inner_v6_pkt[IPv6])

        try:
            # Route entry with non-default VRF for DT.6
            dt6_v6_route_entry = sai_thrift_route_entry_t(
                vr_id=self.t_vrf_id,
                destination=sai_ipprefix(self.client_ipv6_dest + '/128'))
            sai_thrift_create_route_entry(
                self.client, dt6_v6_route_entry, next_hop_id=self.ip_nhop)

            print("DT6: Sending SRv6 packet on port %d to port %d, "
                  "forward from %s to %s"
                  % (self.dev_port11, self.dev_port10, self.srv6_src_ip,
                     self.node1_prefix_sid))
            send_packet(self, self.dev_port11, sr6_v6_pkt)
            verify_packets(self, self.v6_exp_pkt, [self.dev_port10])

            print("DT6: Sending SRv6 packet with SL=0, forward to %s"
                  % (self.dev_port10))
            send_packet(self, self.dev_port11, sr6_zero_seg_pkt)
            verify_packets(self, self.v6_exp_pkt, [self.dev_port10])

            print("DT6: Sending SRv6 packet with SL=1, drop packet")
            send_packet(self, self.dev_port11, sr6_one_seg_pkt)
            verify_no_other_packets(self)

        finally:
            sai_thrift_remove_route_entry(self.client, dt6_v6_route_entry)

    def mySidEndDT4Test(self):
        '''
        Verify SRv6 End.DT4 endpoint behavior
        '''
        print("\nmySidEndDT4Test()")

        sr6_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt4_sid,
            ipv6_hlim=64,
            inner_frame=self.inner_v4_pkt[IP])

        sr6_zero_seg_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt4_sid,
            ipv6_hlim=64,
            srh_seg_left=0,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=[self.node2_prefix_sid, self.node1_prefix_sid],
            inner_frame=self.inner_v4_pkt[IP])

        sr6_one_seg_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt4_sid,
            ipv6_hlim=64,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=[self.node2_prefix_sid, self.node1_prefix_sid],
            inner_frame=self.inner_v4_pkt[IP])

        try:
            # Route entry with non-default VRF for DT.4
            dt4_v4_route_entry = sai_thrift_route_entry_t(
                vr_id=self.t_vrf_id,
                destination=sai_ipprefix(self.client_ip_dest + '/32'))
            sai_thrift_create_route_entry(
                self.client, dt4_v4_route_entry, next_hop_id=self.ip_nhop)

            print("DT4: Sending SRv6 packet on port %d to port %d, "
                  "forward from %s to %s"
                  % (self.dev_port11, self.dev_port10, self.srv6_src_ip,
                     self.node0_prefix_sid))
            send_packet(self, self.dev_port11, sr6_pkt)
            verify_packets(self, self.v4_exp_pkt, [self.dev_port10])

            print("DT4: Sending SRv6 packet with SL=0, forward to %s"
                  % (self.dev_port10))
            send_packet(self, self.dev_port11, sr6_zero_seg_pkt)
            verify_packets(self, self.v4_exp_pkt, [self.dev_port10])

            print("DT4: Sending SRv6 packet with SL=1, drop packet")
            send_packet(self, self.dev_port11, sr6_one_seg_pkt)
            verify_no_other_packets(self)

        finally:
            sai_thrift_remove_route_entry(self.client, dt4_v4_route_entry)

    def mySidXConnectTest(self):
        '''
        Verify SRv6 End.X endpoint behavior
        '''
        print("\nmySidXConnectTest()")

        sr6_more_seg_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_adj_sid,
            ipv6_hlim=64,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.sid_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt1 = simple_ipv6_sr_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node1_prefix_sid,
            ipv6_hlim=63,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.sid_list,
            inner_frame=self.inner_v4_pkt[IP])

        sr6_one_seg_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_adj_sid,
            ipv6_hlim=64,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.sid_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt2 = simple_ipv6ip_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node2_prefix_sid,
            ipv6_hlim=63,
            inner_frame=self.inner_v4_pkt[IP])

        sr6_zero_seg_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_adj_sid,
            ipv6_hlim=64,
            srh_seg_left=0,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.sid_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt3 = simple_tcp_packet(
            eth_src=ROUTER_MAC,
            eth_dst=self.srv6_mac,
            ip_dst=self.client_ip_dest,
            ip_src=self.client_ip_src,
            ip_id=105,
            ip_ttl=63)

        try:
            print("END.X: Send packet with seg_left > 1, "
                  "Cross-connect to port 13")
            send_packet(self, self.dev_port11, sr6_more_seg_pkt)
            verify_packets(self, exp_pkt1, [self.dev_port13])

            print("END.X with PSP: Send packet with seg_left == 1, "
                  "Cross-connect to port 13")
            send_packet(self, self.dev_port11, sr6_one_seg_pkt)
            verify_packets(self, exp_pkt2, [self.dev_port13])

            print("END.X with USD: Send packet with seg_left == 0, "
                  "Cross-connect to port 13")
            send_packet(self, self.dev_port11, sr6_zero_seg_pkt)
            verify_packets(self, exp_pkt3, [self.dev_port13])

            xconnect_ecmp = sai_thrift_create_next_hop_group(
                self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
            xconnect_ecmp1 = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=xconnect_ecmp,
                next_hop_id=self.und_nhop)
            xconnect_ecmp2 = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=xconnect_ecmp,
                next_hop_id=self.xconn_nhop)

            print("Set XConnect NHOP to ECMP")
            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_x_sid,
                next_hop_id=xconnect_ecmp)

            count = [0, 0]
            max_iter = 60
            sip_address = self.srv6_src_ip
            inner_pkt = self.inner_v4_pkt
            for i in range(0, max_iter):
                tcp_sport = 1000 + i
                tcp_dport = 1000 + i
                inner_pkt['TCP'].sport = tcp_sport
                inner_pkt['TCP'].dport = tcp_dport
                inner_frame = inner_pkt
                exp_pkt3['TCP'].sport = tcp_sport
                exp_pkt3['TCP'].dport = tcp_dport

                sr6_zero_seg_pkt = simple_ipv6_sr_packet(
                    eth_dst=ROUTER_MAC,
                    eth_src=self.srv6_mac,
                    ipv6_src=sip_address,  # varying
                    ipv6_dst=self.node0_adj_sid,
                    ipv6_hlim=64,
                    srh_seg_left=0,
                    srh_first_seg=1,
                    srh_nh=0x4,
                    srh_seg_list=[self.node2_prefix_sid,
                                  self.node1_prefix_sid],
                    inner_frame=inner_frame[IP])

                send_packet(self, self.dev_port11, sr6_zero_seg_pkt)
                recv_index = verify_any_packet_any_port(
                    self,
                    [exp_pkt3],
                    [self.dev_port12, self.dev_port13])
                count[recv_index] += 1
            print("ECMP count: ", count)

            for i in range(0, 2):
                self.assertTrue(count[i] > 0.3*max_iter)

            print("Set XConnect NHOP back to port and packet action to drop")
            sai_thrift_set_my_sid_entry_attribute(self.client,
                                                  self.end_x_sid,
                                                  next_hop_id=self.xconn_nhop)
            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_x_sid,
                packet_action=SAI_PACKET_ACTION_DROP)

            print("END.X: Send packet with seg_left > 1, Drop the packet")
            send_packet(self, self.dev_port11, sr6_more_seg_pkt)
            verify_no_other_packets(self)

            print("END.X: Send packet with seg_left == 1, Drop the packet")
            send_packet(self, self.dev_port11, sr6_one_seg_pkt)
            verify_no_other_packets(self)

            print("END.X: Send packet with seg_left == 0, Drop the packet")
            send_packet(self, self.dev_port11, sr6_zero_seg_pkt)
            verify_no_other_packets(self)

        finally:
            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_x_sid,
                packet_action=SAI_PACKET_ACTION_FORWARD)
            sai_thrift_remove_next_hop_group_member(
                self.client, xconnect_ecmp2)
            sai_thrift_remove_next_hop_group_member(
                self.client, xconnect_ecmp1)
            sai_thrift_remove_next_hop_group(self.client, xconnect_ecmp)

    def mySidEndDT4ReEncapTest(self):
        '''
        Verify a following scenario:
        1) A packet come with the DT4 SID and hit the my_sid entry
        2) Packet is decapsulated and inner packet is forwarded
           using inner IP and VRF from my_sid entry
        3) The route lookup points to a nexthop with sidlist,
           so packet is encapsulated again with a new SRH header
        '''
        print("\nmySidEndDT4ReEncapTest()")

        inner_dst_ip = '192.168.1.1'

        inner_packet = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                         eth_src='00:22:22:22:22:21',
                                         ip_dst=inner_dst_ip,
                                         ip_src=self.client_ip_src,
                                         ip_id=105,
                                         ip_ttl=64)

        sr6_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt4_sid,
            ipv6_hlim=64,
            inner_frame=inner_packet[IP])

        inner_packet[IP].ttl = 63

        exp_sr6_pkt = simple_ipv6_sr_packet(
            eth_dst=self.srv6_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node1_prefix_sid,
            ipv6_hlim=64,
            srh_first_seg=0,
            srh_seg_left=1,
            srh_nh=0x4,
            srh_seg_list=[self.node0_prefix_sid],
            inner_frame=inner_packet[IP])

        try:
            sr_route = sai_thrift_route_entry_t(
                vr_id=self.t_vrf_id,
                destination=sai_ipprefix(inner_dst_ip + '/32'))
            sai_thrift_create_route_entry(self.client,
                                          sr_route,
                                          next_hop_id=self.sr_nhop1)

            print("DT4: Sending SRv6 packet on port %d, reencap and forward "
                  "to port %d" % (self.dev_port11, self.dev_port12))
            send_packet(self, self.dev_port11, sr6_pkt)
            verify_packets(self, exp_sr6_pkt, [self.dev_port12])

        finally:
            sai_thrift_remove_route_entry(self.client, sr_route)

    def mySidEndDT6ReEncapTest(self):
        '''
        Verify a following scenario:
        1) A packet come with the DT6 SID and hit the my_sid entry
        2) Packet is decapsulated and inner packet is forwarded
           using inner IP and VRF from my_sid entry
        3) The route lookup points to a nexthop with sidlist,
           so packet is encapsulated again with a new SRH header
        '''
        print("\nmySidEndDT6ReEncapTest()")

        inner_dst_ipv6 = '1111:1111::1'

        inner_packet = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                           eth_src='00:22:22:22:22:21',
                                           ipv6_dst=inner_dst_ipv6,
                                           ipv6_src=self.client_ipv6_src,
                                           ipv6_hlim=64)

        sr6_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt6_sid,
            ipv6_hlim=64,
            inner_frame=inner_packet[IPv6])

        inner_packet[IPv6].hlim = 63

        exp_sr6_pkt = simple_ipv6_sr_packet(
            eth_dst=self.srv6_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node1_prefix_sid,
            ipv6_hlim=64,
            srh_first_seg=0,
            srh_seg_left=1,
            srh_nh=0x29,
            srh_seg_list=[self.node0_prefix_sid],
            inner_frame=inner_packet[IPv6])

        try:
            sr_route = sai_thrift_route_entry_t(
                vr_id=self.t_vrf_id,
                destination=sai_ipprefix(inner_dst_ipv6 + '/128'))
            sai_thrift_create_route_entry(self.client,
                                          sr_route,
                                          next_hop_id=self.sr_nhop1)

            print("DT6: Sending SRv6 packet on port %d, reencap and forward "
                  "to port %d" % (self.dev_port11, self.dev_port12))
            send_packet(self, self.dev_port11, sr6_pkt)
            verify_packets(self, exp_sr6_pkt, [self.dev_port12])

        finally:
            sai_thrift_remove_route_entry(self.client, sr_route)

    def mySidEndDT46ReEncapTest(self):
        '''
        Verify a following scenario (for both: IPv4 and IPv6 inner packet):
        1) A packet come with the DT46 SID and hit the my_sid entry
        2) Packet is decapsulated and inner packet is forwarded
           using inner IP and VRF from my_sid entry
        3) The route lookup points to a nexthop with sidlist,
           so packet is encapsulated again with a new SRH header
        '''
        print("\nmySidEndDT46ReEncapTest()")

        # IPv4 inner packet
        inner_dst_ipv4 = '192.168.1.1'

        inner_v4_packet = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                            eth_src='00:22:22:22:22:21',
                                            ip_dst=inner_dst_ipv4,
                                            ip_src=self.client_ip_src,
                                            ip_id=105,
                                            ip_ttl=64)

        sr6_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt46_sid2,
            ipv6_hlim=64,
            inner_frame=inner_v4_packet[IP])

        inner_v4_packet[IP].ttl = 63

        exp_sr6_pkt = simple_ipv6_sr_packet(
            eth_dst=self.srv6_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node1_prefix_sid,
            ipv6_hlim=64,
            srh_first_seg=0,
            srh_seg_left=1,
            srh_nh=0x4,
            srh_seg_list=[self.node0_prefix_sid],
            inner_frame=inner_v4_packet[IP])

        try:
            sr_route = sai_thrift_route_entry_t(
                vr_id=self.t_vrf_id,
                destination=sai_ipprefix(inner_dst_ipv4 + '/32'))
            sai_thrift_create_route_entry(self.client,
                                          sr_route,
                                          next_hop_id=self.sr_nhop1)

            print("DT46: Sending SRv6 packet with IPv4 inner packet "
                  "on port %d, reencap and forward to port %d"
                  % (self.dev_port11, self.dev_port12))
            send_packet(self, self.dev_port11, sr6_pkt)
            verify_packets(self, exp_sr6_pkt, [self.dev_port12])

        finally:
            sai_thrift_remove_route_entry(self.client, sr_route)

        # IPv6 inner packet
        inner_dst_ipv6 = '1111:1111::1'

        inner_v6_packet = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                              eth_src='00:22:22:22:22:21',
                                              ipv6_dst=inner_dst_ipv6,
                                              ipv6_src=self.client_ipv6_src,
                                              ipv6_hlim=64)
        sr6_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_dt46_sid2,
            ipv6_hlim=64,
            inner_frame=inner_v6_packet[IPv6])

        inner_v6_packet[IPv6].hlim = 63

        exp_sr6_pkt = simple_ipv6_sr_packet(
            eth_dst=self.srv6_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node1_prefix_sid,
            ipv6_hlim=64,
            srh_first_seg=0,
            srh_seg_left=1,
            srh_nh=0x29,
            srh_seg_list=[self.node0_prefix_sid],
            inner_frame=inner_v6_packet[IPv6])

        try:
            sr_route = sai_thrift_route_entry_t(
                vr_id=self.t_vrf_id,
                destination=sai_ipprefix(inner_dst_ipv6 + '/128'))
            sai_thrift_create_route_entry(self.client,
                                          sr_route,
                                          next_hop_id=self.sr_nhop1)

            print("DT46: Sending SRv6 packet with IPv6 inner packet "
                  "on port %d, reencap and forward to port %d"
                  % (self.dev_port11, self.dev_port12))
            send_packet(self, self.dev_port11, sr6_pkt)
            verify_packets(self, exp_sr6_pkt, [self.dev_port12])

        finally:
            sai_thrift_remove_route_entry(self.client, sr_route)

    def mySidB6EncapTest(self):
        '''
        Verify SRv6 End.B6.Encaps.Red endpoint beahvior
        '''
        print("\nmySidB6EncapTest()")

        sr6_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_b6_encap_sid,
            ipv6_hlim=64,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=[self.node2_prefix_sid, self.node1_prefix_sid],
            inner_frame=self.inner_v4_pkt[IP])

        inner_sr_pkt = simple_ipv6_sr_packet(
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node2_prefix_sid,
            srh_seg_left=0,
            srh_first_seg=1,
            srh_nh=0x4,
            ipv6_hlim=63,
            srh_seg_list=[self.node2_prefix_sid, self.node1_prefix_sid],
            inner_frame=self.inner_v4_pkt[IP])

        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.srv6_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node1_prefix_sid,
            ipv6_hlim=64,
            srh_first_seg=0,
            srh_seg_left=1,
            srh_nh=0x29,
            srh_seg_list=[self.node0_prefix_sid],
            inner_frame=inner_sr_pkt[IPv6])

        try:
            print("Verify my_sid entry with B6.Encap with 2 segment NHOPs")
            send_packet(self, self.dev_port11, sr6_pkt)
            verify_packets(self, exp_pkt, [self.dev_port12])

            print("Verify my_sid entry with B6.Encap with 3 segment NHOPs")
            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_b6_encap_sid,
                next_hop_id=self.sr_nhop2)

            exp_pkt = simple_ipv6_sr_packet(
                eth_dst=self.srv6_mac,
                eth_src=ROUTER_MAC,
                ipv6_src=self.srv6_src_ip,
                ipv6_dst=self.node2_prefix_sid,
                ipv6_hlim=64,
                srh_first_seg=1,
                srh_seg_left=2,
                srh_nh=0x29,
                srh_seg_list=[self.node0_prefix_sid, self.node1_prefix_sid],
                inner_frame=inner_sr_pkt[IPv6])
            send_packet(self, self.dev_port11, sr6_pkt)
            verify_packets(self, exp_pkt, [self.dev_port12])

            print("Set my_sid entry packet action to Drop")
            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_b6_encap_sid,
                packet_action=SAI_PACKET_ACTION_DROP)

            send_packet(self, self.dev_port11, sr6_pkt)
            verify_no_other_packets(self)

            print("Set my_sid entry packet action to Trap")
            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_b6_encap_sid,
                packet_action=SAI_PACKET_ACTION_TRAP)

            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            send_packet(self, self.dev_port11, sr6_pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"],
                pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

        finally:
            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_b6_encap_sid,
                packet_action=SAI_PACKET_ACTION_FORWARD)

            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_b6_encap_sid,
                next_hop_id=self.sr_nhop1)

    def mySidB6InsertTest(self):
        '''
        Verify SRv6 End.B6.Insert.Red endpoint beahvior
        '''
        print("\nmySidB6InsertTest()")

        sr6_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_b6_insert_sid,
            ipv6_hlim=64,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=[self.node2_prefix_sid, self.node1_prefix_sid],
            inner_frame=self.inner_v4_pkt[IP])

        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.srv6_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_prefix_sid,
            ipv6_hlim=63,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x2b,  # = 43 => SRH
            srh_seg_list=[self.node0_b6_insert_sid, self.node1_prefix_sid],
            inner_frame=None)
        exp_pkt /= IPv6ExtHdrRouting(
            nh=0x4,
            type=4,
            segleft=2,
            reserved=(1 << 24),  # to set lastentry field
            addresses=[self.node2_prefix_sid, self.node1_prefix_sid])
        exp_pkt /= self.inner_v4_pkt[IP]

        print("Verify my_sid entry with B6.Insert with 2 segment NHOPs")
        send_packet(self, self.dev_port11, sr6_pkt)
        verify_packets(self, exp_pkt, [self.dev_port12])

        sr6_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_b6_insert_sid,
            ipv6_hlim=64,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.srv6_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_prefix_sid,
            ipv6_hlim=63,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=[self.node0_b6_insert_sid, self.node1_prefix_sid],
            inner_frame=self.inner_v4_pkt[IP])

        print("Verify my_sid entry with B6.Insert with 2 segment NHOPs "
                "and IPinIP inner packet")
        send_packet(self, self.dev_port11, sr6_pkt)
        verify_packets(self, exp_pkt, [self.dev_port12])

    def mySidCounterTest(self):
        '''
        Verify statistics of a counter attached to my_sid object
        Also verify getting counter_id of my_sid object and statistics clearing
        '''
        print("\nmySidCounterTest()")

        my_sid_cntr = sai_thrift_get_my_sid_entry_attribute(
            self.client, self.end_sid, counter_id=True)
        self.assertEqual(my_sid_cntr['counter_id'], self.end_sid_counter)

        counter_stats = sai_thrift_get_counter_stats(
            self.client, self.end_sid_counter)
        self.assertEqual(counter_stats['SAI_COUNTER_STAT_PACKETS'],
                         self.end_sid_stats)
        self.asserNotEqual(counter_stats['SAI_COUNTER_STAT_BYTES'], 0)

        print("My SID counter correct")

        sai_thrift_clear_counter_stats(self.client, self.end_sid_counter)

        counter_stats = sai_thrift_get_counter_stats(
            self.client, self.end_sid_counter)
        self.assertEqual(counter_stats['SAI_COUNTER_STAT_PACKETS'], 0)
        self.assertEqual(counter_stats['SAI_COUNTER_STAT_BYTES'], 0)

        print("My SID counter clear")

    def getSetMySidEntryTest(self):
        '''
        Verify getting and setting my SID entry attributes
        '''
        print("\ngetSetMySidEntryTest()")

        sr6_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.srv6_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node0_prefix_sid,
            ipv6_hlim=64,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.sid_list,
            inner_frame=self.inner_v4_pkt[IP])

        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.srv6_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.node1_prefix_sid,
            ipv6_hlim=63,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.sid_list,
            inner_frame=self.inner_v4_pkt[IP])

        try:
            attr = sai_thrift_get_my_sid_entry_attribute(
                self.client,
                self.end_sid,
                endpoint_behavior=True,
                endpoint_behavior_flavor=True,
                packet_action=True)

            self.assertEqual(
                attr['endpoint_behavior'],
                SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_E)
            self.assertEqual(
                attr['endpoint_behavior_flavor'],
                SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP_AND_USD)
            self.assertEqual(
                attr['packet_action'], SAI_PACKET_ACTION_FORWARD)

            print("Set new attributes values")
            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_sid,
                endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_T)

            end_bf = SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP
            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_sid,
                endpoint_behavior_flavor=end_bf)

            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_sid,
                vrf=self.t_vrf_id)

            attr = sai_thrift_get_my_sid_entry_attribute(
                self.client,
                self.end_sid,
                endpoint_behavior=True,
                endpoint_behavior_flavor=True,
                vrf=True)

            self.assertEqual(
                attr['endpoint_behavior'],
                SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_T)
            self.assertEqual(
                attr['endpoint_behavior_flavor'],
                SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP)
            self.assertEqual(
                attr['vrf'], self.t_vrf_id)

            print("END -> END.T: Sending packet on port %d to port %d - "
                  "forward through another vrf"
                  % (self.dev_port11, self.dev_port12))
            send_packet(self, self.dev_port11, sr6_pkt)
            verify_packet(self, exp_pkt, self.dev_port12)

            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_sid,
                packet_action=SAI_PACKET_ACTION_DROP)

            attr = sai_thrift_get_my_sid_entry_attribute(
                self.client,
                self.end_sid,
                packet_action=True)

            self.assertEqual(attr['packet_action'], SAI_PACKET_ACTION_DROP)

            print("END -> END.T: Sending packet on port %d - "
                  "drop the packet" % self.dev_port11)
            send_packet(self, self.dev_port11, sr6_pkt)
            verify_no_other_packets(self)

        finally:
            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_sid,
                endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_E)

            end_bf = SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP_AND_USD
            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_sid,
                endpoint_behavior_flavor=end_bf)

            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_sid,
                packet_action=SAI_PACKET_ACTION_FORWARD)

            sai_thrift_set_my_sid_entry_attribute(
                self.client,
                self.end_sid,
                vrf=None)


@group("draft")
class Srv6MySidUsidTest(SaiHelper):
    '''
    SRv6 local my SID behaviors with compressed SIDs tests
    '''
    def setUp(self):
        super(Srv6MySidUsidTest, self).setUp()

        self.client_ip_src = '11.0.0.10'
        self.client_ip_dest = '11.0.0.11'
        self.srv6_src_ip = '2001:1001:0:10::1'
        self.client_dmac = '00:22:33:44:55:66'
        self.und_nbor_mac = '00:aa:bb:cc:dd:ee'
        self.und_xconn_nbor_mac = '00:aa:bb:cc:dd:ee'

        self.in_un_local_usid = '2001:db8:100::'
        self.in_un_usid = '2001:db8:100:200:300:400:500:600'
        self.out_un_usid = '2001:db8:200:300:400:500:600::'
        self.in_ua_local_usid = '2001:db8:700:800::'
        self.in_ua_usid = '2001:db8:700:800:900:a00:b00:c00'
        self.out_ua_usid = '2001:db8:800:900:a00:b00:c00::'
        self.next_usid1 = '2001:db8:1100:1200:1300:1400:1500:1600'
        self.next_usid2 = '2001:db8:2100:2200:2300:2400:2500:2600'
        un_usid_mask = '/48'  # 32-bit block len + 16-bit id len

        self.usid_segment_list = [self.next_usid2, self.next_usid1]

        self.urif_lb = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.default_vrf)

        self.sr_tunnel = sai_thrift_create_tunnel(
            self.client,
            type=SAI_TUNNEL_TYPE_SRV6,
            encap_src_ip=sai_ipaddress(self.srv6_src_ip),
            underlay_interface=self.urif_lb,
            encap_ttl_mode=SAI_TUNNEL_TTL_MODE_PIPE_MODEL,
            decap_ttl_mode=SAI_TUNNEL_TTL_MODE_PIPE_MODEL)

        self.und_nhop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            ip=sai_ipaddress('20.20.20.20'),
            router_interface_id=self.port12_rif)

        self.und_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.port12_rif, ip_address=sai_ipaddress('20.20.20.20'))
        sai_thrift_create_neighbor_entry(
            self.client, self.und_nbor, dst_mac_address=self.und_nbor_mac)

        self.und_route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.in_un_local_usid + un_usid_mask))
        sai_thrift_create_route_entry(
            self.client, self.und_route_entry1, next_hop_id=self.und_nhop)

        self.und_route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.out_un_usid + un_usid_mask))
        sai_thrift_create_route_entry(
            self.client, self.und_route_entry2, next_hop_id=self.und_nhop)

        self.und_route_entry3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.next_usid1 + un_usid_mask))
        sai_thrift_create_route_entry(
            self.client, self.und_route_entry3, next_hop_id=self.und_nhop)

        self.und_route_entry4 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.next_usid2 + un_usid_mask))
        sai_thrift_create_route_entry(
            self.client, self.und_route_entry4, next_hop_id=self.und_nhop)

        self.client_route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.client_ip_dest + '/32'))
        sai_thrift_create_route_entry(
            self.client, self.client_route_entry, next_hop_id=self.und_nhop)

        self.und_xnhop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            ip=sai_ipaddress('30.30.30.30'),
            router_interface_id=self.port10_rif)

        self.und_xnbor = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('30.30.30.30'))
        sai_thrift_create_neighbor_entry(
            self.client, self.und_xnbor,
            dst_mac_address=self.und_xconn_nbor_mac)

        self.end_un_sid = sai_thrift_my_sid_entry_t(
            sid=self.in_un_local_usid, vr_id=self.default_vrf)
        sai_thrift_create_my_sid_entry(
            self.client,
            self.end_un_sid,
            endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_UN)

        self.end_ua_sid = sai_thrift_my_sid_entry_t(
            sid=self.in_ua_local_usid, vr_id=self.default_vrf)
        sai_thrift_create_my_sid_entry(
            self.client,
            self.end_ua_sid,
            next_hop_id=self.und_xnhop,
            endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_UA)

        self.inner_v4_pkt = simple_tcp_packet(ip_dst=self.client_ip_dest,
                                              ip_src=self.client_ip_src,
                                              ip_id=105,
                                              ip_ttl=63)

    def runTest(self):
        self.mySidEndUNTest()
        self.mySidEndUNPSPTest()
        self.mySidEndUNUSDTest()
        self.mySidEndUNUSDNoSRHTest()
        self.mySidEndUATest()
        self.mySidEndUAPSPTest()
        self.mySidEndUAUSDTest()
        self.mySidEndUAUSDNoSRHTest()

    def tearDown(self):
        sai_thrift_remove_my_sid_entry(self.client, self.end_ua_sid)
        sai_thrift_remove_my_sid_entry(self.client, self.end_un_sid)
        sai_thrift_remove_neighbor_entry(self.client, self.und_xnbor)
        sai_thrift_remove_next_hop(self.client, self.und_xnhop)
        sai_thrift_remove_route_entry(self.client, self.und_route_entry4)
        sai_thrift_remove_route_entry(self.client, self.und_route_entry3)
        sai_thrift_remove_route_entry(self.client, self.und_route_entry2)
        sai_thrift_remove_route_entry(self.client, self.und_route_entry1)
        sai_thrift_remove_route_entry(self.client, self.client_route_entry)
        sai_thrift_remove_neighbor_entry(self.client, self.und_nbor)
        sai_thrift_remove_next_hop(self.client, self.und_nhop)
        sai_thrift_remove_tunnel(self.client, self.sr_tunnel)
        sai_thrift_remove_router_interface(self.client, self.urif_lb)

        super(Srv6MySidUsidTest, self).tearDown()

    def mySidEndUNTest(self):
        '''
        Verify SRv6 End.uN uSID behavior
        '''
        print("\nmySidEndUNTest()")

        sr_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_un_usid,
            ipv6_hlim=64,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.out_un_usid,
            ipv6_hlim=63,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])

        print("Verifying End.uN function with SL>1 and >1 uSIDs left")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port12)

        sr_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_un_local_usid,
            ipv6_hlim=64,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.next_usid1,
            ipv6_hlim=63,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])

        print("Verifying End.uN function with SL>1 and last uSID in use")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port12)

    def mySidEndUNPSPTest(self):
        '''
        Verify SRv6 End.uN uSID behavior with PSP flavor
        '''
        print("\nmySidEndUNPSPTest()")

        sr_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_un_usid,
            ipv6_hlim=63,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.out_un_usid,
            ipv6_hlim=62,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])

        print("Verifying End.uN function with SL=1 and >1 uSIDs left")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port12)

        sr_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_un_local_usid,
            ipv6_hlim=63,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])
        ipip_pkt = simple_ipv6ip_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.next_usid2,
            ipv6_hlim=62,
            inner_frame=self.inner_v4_pkt[IP])

        print("Verifying End.uN function with SL=1 and last uSID in use")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, ipip_pkt, self.dev_port12)

    def mySidEndUNUSDTest(self):
        '''
        Verify SRv6 End.uN uSID behavior with USD flavor
        '''
        print("\nmySidEndUNUSDTest()")

        sr_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_un_usid,
            ipv6_hlim=63,
            srh_seg_left=0,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.out_un_usid,
            ipv6_hlim=62,
            srh_seg_left=0,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])

        print("Verifying End.uN function with SL=0 and >1 uSIDs left")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port12)

        sr_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_un_local_usid,
            ipv6_hlim=63,
            srh_seg_left=0,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_tcp_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ip_src=self.client_ip_src,
            ip_dst=self.client_ip_dest,
            ip_id=105,
            ip_ttl=62)

        print("Verifying End.uN function with SL=1 and last uSID in use")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port12)

    def mySidEndUNUSDNoSRHTest(self):
        '''
        Verify SRv6 End.uN uSID behavior with USD flavor and no SRH
        '''
        print("\nmySidEndUNUSDNoSRHTest()")

        sr_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_un_usid,
            ipv6_hlim=62,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_ipv6ip_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.out_un_usid,
            ipv6_hlim=61,
            inner_frame=self.inner_v4_pkt[IP])

        print("Verifying End.uN function with SL=0 and >1 uSIDs left")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port12)

        sr_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_un_local_usid,
            ipv6_hlim=62,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_tcp_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ip_src=self.client_ip_src,
            ip_dst=self.client_ip_dest,
            ip_id=105,
            ip_ttl=62)

        print("Verifying End.uN function with SL=1 and last uSID in use")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port12)

    def mySidEndUATest(self):
        '''
        Verify SRv6 End.uA uSID behavior
        '''
        print("\nmySidEndUATest()")

        sr_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_ua_usid,
            ipv6_hlim=64,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.out_ua_usid,
            ipv6_hlim=63,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])

        print("Verifying End.uA function with SL>1 and >1 uSIDs left")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port10)

        sr_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_ua_local_usid,
            ipv6_hlim=64,
            srh_seg_left=2,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.next_usid1,
            ipv6_hlim=63,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])

        print("Verifying End.uA function with SL>1 and last uSID in use")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port10)

    def mySidEndUAPSPTest(self):
        '''
        Verify SRv6 End.uA uSID behavior with PSP flavor
        '''
        print("\nmySidEndUAPSPTest()")

        sr_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_ua_usid,
            ipv6_hlim=63,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.out_ua_usid,
            ipv6_hlim=62,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])

        print("Verifying End.uA function with SL=1 and >1 uSIDs left")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port10)

        sr_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_ua_local_usid,
            ipv6_hlim=63,
            srh_seg_left=1,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])
        ipip_pkt = simple_ipv6ip_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.next_usid2,
            ipv6_hlim=62,
            inner_frame=self.inner_v4_pkt[IP])

        print("Verifying End.uA function with SL=1 and last uSID in use")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, ipip_pkt, self.dev_port10)

    def mySidEndUAUSDTest(self):
        '''
        Verify SRv6 End.uA uSID behavior with USD flavor
        '''
        print("\nmySidEndUAUSDTest()")

        sr_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_ua_usid,
            ipv6_hlim=63,
            srh_seg_left=0,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_ipv6_sr_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.out_ua_usid,
            ipv6_hlim=62,
            srh_seg_left=0,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])

        print("Verifying End.uA function with SL=0 and >1 uSIDs left")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port10)

        sr_pkt = simple_ipv6_sr_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_ua_local_usid,
            ipv6_hlim=63,
            srh_seg_left=0,
            srh_first_seg=1,
            srh_nh=0x4,
            srh_seg_list=self.usid_segment_list,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_tcp_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ip_src=self.client_ip_src,
            ip_dst=self.client_ip_dest,
            ip_id=105,
            ip_ttl=62)

        print("Verifying End.uA function with SL=1 and last uSID in use")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port10)

    def mySidEndUAUSDNoSRHTest(self):
        '''
        Verify SRv6 End.uA uSID behavior with USD flavor and no SRH
        '''
        print("\nmySidEndUAUSDNoSRHTest()")

        sr_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_ua_usid,
            ipv6_hlim=62,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_ipv6ip_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.out_ua_usid,
            ipv6_hlim=61,
            inner_frame=self.inner_v4_pkt[IP])

        print("Verifying End.uA function with SL=0 and >1 uSIDs left")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port10)

        sr_pkt = simple_ipv6ip_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.und_nbor_mac,
            ipv6_src=self.srv6_src_ip,
            ipv6_dst=self.in_ua_local_usid,
            ipv6_hlim=62,
            inner_frame=self.inner_v4_pkt[IP])
        exp_pkt = simple_tcp_packet(
            eth_dst=self.und_nbor_mac,
            eth_src=ROUTER_MAC,
            ip_src=self.client_ip_src,
            ip_dst=self.client_ip_dest,
            ip_id=105,
            ip_ttl=62)

        print("Verifying End.uA function with SL=1 and last uSID in use")
        send_packet(self, self.dev_port11, sr_pkt)
        verify_packet(self, exp_pkt, self.dev_port10)


@group("draft")
class Srv6MySidDropTest(SaiHelper):
    '''
    SRv6 drop cases tests
    Verify if debug counter is hit while SRv6 packets are dropped
    '''
    def setUp(self):
        super(Srv6MySidDropTest, self).setUp()

        self.ip_src = '10.10.10.1'
        self.ip_dst = '10.10.10.2'
        self.ipv6_src = '2001:db8::1:1'
        self.ipv6_dst = '2001:db8::2:1'
        self.srv6_src_ip = '2001:1001:0:10::1'

        self.und_ip = '20.20.20.1'

        self.node0_prefix_sid = 'baba:1001:0:10::'
        self.node1_prefix_sid = 'baba:1001:0:20::'

        self.seg1 = 'baba:1001:0:10::'
        self.seg2 = 'baba:1001:0:20::'
        self.seg3 = 'baba:1001:0:30::'
        self.my_sid_list = [self.seg1, self.seg2, self.seg3]

        self.src_mac = '00:10:10:10:10:10'
        self.dst_mac = '00:20:20:20:20:20'
        self.srv6_mac = '00:aa:aa:aa:aa:aa'

        self.urif_lb = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.default_vrf)

        self.sr_tunnel = sai_thrift_create_tunnel(
            self.client,
            type=SAI_TUNNEL_TYPE_SRV6,
            encap_src_ip=sai_ipaddress(self.srv6_src_ip),
            underlay_interface=self.urif_lb,
            encap_ttl_mode=SAI_TUNNEL_TTL_MODE_PIPE_MODEL,
            decap_ttl_mode=SAI_TUNNEL_TTL_MODE_PIPE_MODEL)

        self.und_nhop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            ip=sai_ipaddress(self.und_ip),
            router_interface_id=self.port12_rif)

        self.und_nbor = sai_thrift_neighbor_entry_t(
            rif_id=self.port12_rif, ip_address=sai_ipaddress(self.und_ip))
        sai_thrift_create_neighbor_entry(
            self.client, self.und_nbor, dst_mac_address=self.srv6_mac)

        self.und_route3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.seg1 + '/64'))
        sai_thrift_create_route_entry(
            self.client, self.und_route3, next_hop_id=self.und_nhop)

        self.und_route4 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.seg2 + '/64'))
        sai_thrift_create_route_entry(
            self.client, self.und_route4, next_hop_id=self.und_nhop)

        # debug counters config
        drop_reason = sai_thrift_s32_list_t(
            count=1, int32list=[SAI_IN_DROP_REASON_SRV6_LOCAL_SID_DROP])
        self.debug_cnt = sai_thrift_create_debug_counter(
            self.client,
            type=SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,
            in_drop_reason_list=drop_reason)
        self.assertNotEqual(self.debug_cnt, 0)

        dc_attr = sai_thrift_get_debug_counter_attribute(
            self.client, self.debug_cnt, index=True)
        self.dc_index = dc_attr['index'] \
            + SAI_PORT_STAT_IN_DROP_REASON_RANGE_BASE

        self.drop_stats = 0

        sai_thrift_clear_port_stats(self.client, self.port11)

        # tests packets
        self.inner_v4_pkt = simple_tcp_packet(ip_dst=self.ip_dst,
                                              ip_src=self.ip_src,
                                              ip_id=105,
                                              ip_ttl=63)
        self.sr6_inner_v6_pkt = simple_tcpv6_packet(ipv6_dst=self.ipv6_dst,
                                                    ipv6_src=self.ipv6_src,
                                                    ipv6_hlim=63)

    def runTest(self):
        self.packetActionDropTest()
        self.nonZeroSlEndDTxDropTest()

    def tearDown(self):
        sai_thrift_clear_port_stats(self.client, self.port11)

        sai_thrift_remove_debug_counter(self.client, self.debug_cnt)
        sai_thrift_remove_route_entry(self.client, self.und_route4)
        sai_thrift_remove_route_entry(self.client, self.und_route3)
        sai_thrift_remove_neighbor_entry(self.client, self.und_nbor)
        sai_thrift_remove_next_hop(self.client, self.und_nhop)
        sai_thrift_remove_tunnel(self.client, self.sr_tunnel)
        sai_thrift_remove_router_interface(self.client, self.urif_lb)

        super(Srv6MySidDropTest, self).tearDown()

    def packetActionDropTest(self):
        '''
        Verify if packets are dropped when my SID packet action is
        SAI_PACKET_ACTION_DROP
        '''
        print("\npacketActionDropTest()")

        try:
            my_sid = sai_thrift_my_sid_entry_t(
                sid=self.seg1, vr_id=self.default_vrf)
            sai_thrift_create_my_sid_entry(
                self.client,
                my_sid,
                endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_E,
                vrf=self.default_vrf,
                packet_action=SAI_PACKET_ACTION_FORWARD)

            sr6_pkt = simple_ipv6_sr_packet(
                eth_dst=ROUTER_MAC,
                eth_src=self.srv6_mac,
                ipv6_src=self.srv6_src_ip,
                ipv6_dst=self.seg1,
                ipv6_hlim=64,
                srh_seg_left=2,
                srh_first_seg=1,
                srh_nh=0x4,
                srh_seg_list=[self.seg3, self.seg2],
                inner_frame=self.inner_v4_pkt[IP])

            exp_pkt = simple_ipv6_sr_packet(
                eth_dst=self.srv6_mac,
                eth_src=ROUTER_MAC,
                ipv6_src=self.srv6_src_ip,
                ipv6_dst=self.seg2,
                ipv6_hlim=63,
                srh_seg_left=1,
                srh_first_seg=1,
                srh_nh=0x4,
                srh_seg_list=[self.seg3, self.seg2],
                inner_frame=self.inner_v4_pkt[IP])

            print("Sending packet on port %d to port %d with my SID packet "
                  "action FORWARD" % (self.dev_port11, self.dev_port12))
            send_packet(self, self.dev_port11, sr6_pkt)
            verify_packet(self, exp_pkt, self.dev_port12)

            print("Changing my SID packet action to DROP")
            sai_thrift_set_my_sid_entry_attribute(
                self.client, my_sid, packet_action=SAI_PACKET_ACTION_DROP)

            drop_pkt_no = 5
            print("Sending packet on port %d with my SID action DROP"
                  % (self.dev_port11))
            send_packet(self, self.dev_port11, sr6_pkt, drop_pkt_no)
            verify_no_other_packets(self)
            self.drop_stats += drop_pkt_no

            print("Checking SRv6 debug counter")
            dc_stats = sai_thrift_get_debug_counter_port_stats(
                self.client, self.port11, [self.dc_index])
            self.assertEqual(dc_stats[self.dc_index], self.drop_stats,
                             "SRv6 debug counter value = %d incorrect! "
                             "Should be: %d"
                             % (dc_stats[self.dc_index], self.drop_stats))

        finally:
            sai_thrift_remove_my_sid_entry(self.client, my_sid)

    def nonZeroSlEndDTxDropTest(self):
        '''
        Verify if packets with SL!=0 are dropped for End.D* endpoints
        '''
        print("\nnonZeroSlEndDTxDropTest()")

        try:
            my_sid = sai_thrift_my_sid_entry_t(
                sid=self.seg1, vr_id=self.default_vrf)

            sr6_pkt = simple_ipv6_sr_packet(
                eth_dst=ROUTER_MAC,
                eth_src=self.srv6_mac,
                ipv6_src=self.srv6_src_ip,
                ipv6_dst=self.seg1,
                ipv6_hlim=64,
                srh_seg_left=2,
                srh_first_seg=1,
                srh_nh=0x4,
                srh_seg_list=[self.seg3, self.seg2],
                inner_frame=self.inner_v4_pkt[IP])

            drop_pkt_no = 5

            # End.DT46
            sai_thrift_create_my_sid_entry(
                self.client,
                my_sid,
                endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT46,
                vrf=self.default_vrf,
                packet_action=SAI_PACKET_ACTION_FORWARD)
            print("Sending packet with SL!=0 with local End.DT46 SID")
            send_packet(self, self.dev_port11, sr6_pkt, drop_pkt_no)
            verify_no_other_packets(self)
            self.drop_stats += drop_pkt_no

            print("Checking SRv6 debug counter")
            dc_stats = sai_thrift_get_debug_counter_port_stats(
                self.client, self.port11, [self.dc_index])
            self.assertEqual(dc_stats[self.dc_index], self.drop_stats,
                             "SRv6 debug counter value = %d incorrect! "
                             "Should be: %d"
                             % (dc_stats[self.dc_index], self.drop_stats))

            # End.DT4
            sai_thrift_remove_my_sid_entry(self.client, my_sid)
            sai_thrift_create_my_sid_entry(
                self.client,
                my_sid,
                endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT4,
                vrf=self.default_vrf,
                packet_action=SAI_PACKET_ACTION_FORWARD)

            print("Sending packet with SL!=0 with local End.DT4 SID")
            send_packet(self, self.dev_port11, sr6_pkt, drop_pkt_no)
            verify_no_other_packets(self)
            self.drop_stats += drop_pkt_no

            print("Checking SRv6 debug counter")
            dc_stats = sai_thrift_get_debug_counter_port_stats(
                self.client, self.port11, [self.dc_index])
            self.assertEqual(dc_stats[self.dc_index], self.drop_stats,
                             "SRv6 debug counter value = %d incorrect! "
                             "Should be: %d"
                             % (dc_stats[self.dc_index], self.drop_stats))

            # End.DT6
            sai_thrift_remove_my_sid_entry(self.client, my_sid)
            sai_thrift_create_my_sid_entry(
                self.client,
                my_sid,
                endpoint_behavior=SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT6,
                vrf=self.default_vrf,
                packet_action=SAI_PACKET_ACTION_FORWARD)

            sr6_pkt.inner_frame = self.sr6_inner_v6_pkt[IPv6]

            print("Sending packet with SL!=0 with local End.DT6 SID")
            send_packet(self, self.dev_port11, sr6_pkt, drop_pkt_no)
            verify_no_other_packets(self)
            self.drop_stats += drop_pkt_no

            print("Checking SRv6 debug counter")
            dc_stats = sai_thrift_get_debug_counter_port_stats(
                self.client, self.port11, [self.dc_index])
            self.assertEqual(dc_stats[self.dc_index], self.drop_stats,
                             "SRv6 debug counter value = %d incorrect! "
                             "Should be: %d"
                             % (dc_stats[self.dc_index], self.drop_stats))

        finally:
            sai_thrift_remove_my_sid_entry(self.client, my_sid)


@group("draft")
class MySidObjectsAvailibilityTest(SaiHelperBase):
    '''
    CRM objects availability verification class
    '''
    def runTest(self):
        print("\nMySidObjectsAvailibilityTest()")

        my_sid_entry_list = []
        sid_prefix = '2001:db8::100:'

        # how many entries each behavior (with specific flavor) occupies
        ep_types = [
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_E,
             SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP_AND_USD, 3],
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_E,
             SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP, 2],
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_E,
             SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_USD, 2],
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_X,
             SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP_AND_USD, 3],
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_X,
             SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP, 2],
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_X,
             SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_USD, 2],
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_T, None, 1],
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DX4, None, 1],
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DX6, None, 1],
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT4, None, 3],
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT6, None, 3],
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT46, None, 3],
            [SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_B6_ENCAPS_RED, None, 1]
        ]

        for idx, ep_type in enumerate(ep_types):
            no_of_objects = random.randint(10, 100)

            try:
                obj_avail_before = sai_thrift_object_type_get_availability(
                    self.client,
                    obj_type=SAI_OBJECT_TYPE_MY_SID_ENTRY)

                print("%d my_sid entries available\n"
                      "Creating a random number of my_sid entries (%d)"
                      % (obj_avail_before, no_of_objects))

                for i in range(no_of_objects):
                    sid_ip = sid_prefix + str(idx) + ":" + str(i)
                    my_sid = sai_thrift_my_sid_entry_t(
                        sid=sid_ip, vr_id=self.default_vrf)
                    sai_thrift_create_my_sid_entry(
                        self.client,
                        my_sid,
                        vrf=self.default_vrf,
                        endpoint_behavior=ep_type[0],
                        endpoint_behavior_flavor=ep_type[1])
                    my_sid_entry_list.append(my_sid)

                time.sleep(1)
                obj_avail_after = sai_thrift_object_type_get_availability(
                    self.client,
                    obj_type=SAI_OBJECT_TYPE_MY_SID_ENTRY)
                print("%d my_sid entries available after creating "
                      "%d entries" % (obj_avail_after, no_of_objects))

                self.assertEqual(obj_avail_before,
                                 obj_avail_after + no_of_objects * ep_type[2])

            finally:
                for my_sid in my_sid_entry_list:
                    sai_thrift_remove_my_sid_entry(self.client, my_sid)
