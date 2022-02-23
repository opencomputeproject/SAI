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
Thrift SAI interface Port Isolation tests
'''

from sai_thrift.sai_headers import *

from sai_base_test import *


@group("draft")
class PortIsolationTest(SaiHelper):
    '''
    Port isolation test class
    '''

    def setUp(self):
        super(PortIsolationTest, self).setUp()
        self.mac11 = '00:11:11:11:11:11:11'
        self.mac22 = '00:22:22:22:22:22:22'
        self.mac33 = '00:33:33:33:33:33:33'

        # Create ingress RIFs, use port10_rif and port24_rif as ingress
        self.port24_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port24)

        # Create nhop for port11_rif
        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.2'),
            router_interface_id=self.port11_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port11_rif, ip_address=sai_ipaddress('10.10.0.2'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry1,
                                         dst_mac_address=self.mac11)

        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.20.0.2'),
            router_interface_id=self.port12_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=self.port12_rif, ip_address=sai_ipaddress('20.20.0.2'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry2,
                                         dst_mac_address=self.mac22)

        self.nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('30.30.0.3'),
            router_interface_id=self.port13_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry3 = sai_thrift_neighbor_entry_t(
            rif_id=self.port13_rif, ip_address=sai_ipaddress('30.30.0.3'))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry3,
                                         dst_mac_address=self.mac33)

        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry1,
                                      next_hop_id=self.nhop1)

        self.route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('20.20.20.1/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry2,
                                      next_hop_id=self.nhop2)

        self.route_entry3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('30.30.30.1/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.route_entry3,
                                      next_hop_id=self.nhop3)

        self.isolation_group1 = sai_thrift_create_isolation_group(
            self.client, type=SAI_ISOLATION_GROUP_TYPE_PORT)
        self.isolation_group_mbr1 = sai_thrift_create_isolation_group_member(
            self.client,
            isolation_group_id=self.isolation_group1,
            isolation_object=self.port11)
        self.isolation_group_mbr2 = sai_thrift_create_isolation_group_member(
            self.client,
            isolation_group_id=self.isolation_group1,
            isolation_object=self.port12)

        self.isolation_group2 = sai_thrift_create_isolation_group(
            self.client, type=SAI_ISOLATION_GROUP_TYPE_PORT)
        self.isolation_group_mbr3 = sai_thrift_create_isolation_group_member(
            self.client,
            isolation_group_id=self.isolation_group2,
            isolation_object=self.port12)
        self.isolation_group_mbr4 = sai_thrift_create_isolation_group_member(
            self.client,
            isolation_group_id=self.isolation_group2,
            isolation_object=self.port13)

    def runTest(self):
        self.attributeTest()
        self.forwardingTest()

    def tearDown(self):
        sai_thrift_remove_isolation_group_member(self.client,
                                                 self.isolation_group_mbr4)
        sai_thrift_remove_isolation_group_member(self.client,
                                                 self.isolation_group_mbr3)
        sai_thrift_remove_isolation_group(self.client, self.isolation_group2)
        sai_thrift_remove_isolation_group_member(self.client,
                                                 self.isolation_group_mbr2)
        sai_thrift_remove_isolation_group_member(self.client,
                                                 self.isolation_group_mbr1)
        sai_thrift_remove_isolation_group(self.client, self.isolation_group1)

        sai_thrift_remove_route_entry(self.client, self.route_entry3)
        sai_thrift_remove_route_entry(self.client, self.route_entry2)
        sai_thrift_remove_route_entry(self.client, self.route_entry1)

        sai_thrift_remove_next_hop(self.client, self.nhop3)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry3)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)

        sai_thrift_remove_router_interface(self.client, self.port24_rif)
        super(PortIsolationTest, self).tearDown()

    def attributeTest(self):
        '''
        Verify isolation group CRUD operations
        '''
        print("attributeTest")
        attrs = sai_thrift_get_isolation_group_attribute(self.client,
                                                         self.isolation_group1,
                                                         type=True)
        self.assertEqual(attrs["type"], SAI_ISOLATION_GROUP_TYPE_PORT)

        attrs = sai_thrift_get_isolation_group_member_attribute(
            self.client,
            self.isolation_group_mbr1,
            isolation_group_id=True,
            isolation_object=True)
        self.assertEqual(attrs["isolation_group_id"], self.isolation_group1)
        self.assertEqual(attrs["isolation_object"], self.port11)

    def forwardingTest(self):
        '''
        Test forwarding between ports with isolation groups attached
        Ingress ports: port10 and port24
        Isolation groups:
          grp1: port 11 and port12
          grp2: port 12 and port13
        '''
        print("forwardingTest")
        try:
            pkt1 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                     ip_dst='10.10.10.1',
                                     ip_ttl=64)
            exp_pkt1 = simple_tcp_packet(eth_dst=self.mac11,
                                         eth_src=ROUTER_MAC,
                                         ip_dst='10.10.10.1',
                                         ip_ttl=63)
            pkt2 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                     ip_dst='20.20.20.1',
                                     ip_ttl=64)
            exp_pkt2 = simple_tcp_packet(eth_dst=self.mac22,
                                         eth_src=ROUTER_MAC,
                                         ip_dst='20.20.20.1',
                                         ip_ttl=63)
            pkt3 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                     ip_dst='30.30.30.1',
                                     ip_ttl=64)
            exp_pkt3 = simple_tcp_packet(eth_dst=self.mac33,
                                         eth_src=ROUTER_MAC,
                                         ip_dst='30.30.30.1',
                                         ip_ttl=63)

            print("Sending packet from port %d" % self.dev_port10)
            send_packet(self, self.dev_port10, pkt1)
            verify_packet(self, exp_pkt1, self.dev_port11)
            send_packet(self, self.dev_port10, pkt2)
            verify_packet(self, exp_pkt2, self.dev_port12)
            send_packet(self, self.dev_port10, pkt3)
            verify_packet(self, exp_pkt3, self.dev_port13)

            print("Sending packet from port %d" % self.dev_port24)
            send_packet(self, self.dev_port24, pkt1)
            verify_packet(self, exp_pkt1, self.dev_port11)
            send_packet(self, self.dev_port24, pkt2)
            verify_packet(self, exp_pkt2, self.dev_port12)
            send_packet(self, self.dev_port24, pkt3)
            verify_packet(self, exp_pkt3, self.dev_port13)

            # Attach isolation groups
            print("Attach isolation groups to ingress ports")
            sai_thrift_set_port_attribute(
                self.client,
                self.port10,
                isolation_group=self.isolation_group1)
            sai_thrift_set_port_attribute(
                self.client,
                self.port24,
                isolation_group=self.isolation_group2)
            print("Sending packet from port %d" % self.dev_port10)
            send_packet(self, self.dev_port10, pkt1)
            verify_no_other_packets(self, timeout=1)
            send_packet(self, self.dev_port10, pkt2)
            verify_no_other_packets(self, timeout=1)
            send_packet(self, self.dev_port10, pkt3)
            verify_packet(self, exp_pkt3, self.dev_port13)

            print("Sending packet from port %d" % self.dev_port24)
            send_packet(self, self.dev_port24, pkt1)
            verify_packet(self, exp_pkt1, self.dev_port11)
            send_packet(self, self.dev_port24, pkt2)
            verify_no_other_packets(self, timeout=1)
            send_packet(self, self.dev_port24, pkt3)
            verify_no_other_packets(self, timeout=1)

            # Detach isolation groups
            print("Detach isolation groups from ingress ports")
            sai_thrift_set_port_attribute(self.client,
                                          self.port10,
                                          isolation_group=SAI_NULL_OBJECT_ID)
            sai_thrift_set_port_attribute(self.client,
                                          self.port24,
                                          isolation_group=SAI_NULL_OBJECT_ID)
            print("Sending packet from port %d" % self.dev_port10)
            send_packet(self, self.dev_port10, pkt1)
            verify_packet(self, exp_pkt1, self.dev_port11)
            send_packet(self, self.dev_port10, pkt2)
            verify_packet(self, exp_pkt2, self.dev_port12)
            send_packet(self, self.dev_port10, pkt3)
            verify_packet(self, exp_pkt3, self.dev_port13)

            print("Sending packet from port %d" % self.dev_port24)
            send_packet(self, self.dev_port24, pkt1)
            verify_packet(self, exp_pkt1, self.dev_port11)
            send_packet(self, self.dev_port24, pkt2)
            verify_packet(self, exp_pkt2, self.dev_port12)
            send_packet(self, self.dev_port24, pkt3)
            verify_packet(self, exp_pkt3, self.dev_port13)

        finally:
            sai_thrift_set_port_attribute(self.client,
                                          self.port10,
                                          isolation_group=SAI_NULL_OBJECT_ID)
            sai_thrift_set_port_attribute(self.client,
                                          self.port24,
                                          isolation_group=SAI_NULL_OBJECT_ID)
