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
Thrift SAI interface NextHopGroup tests
"""

import binascii

from sai_thrift.sai_headers import *

from sai_base_test import *

TEST_ECMP_SEED = 37
TEST_LAG_SEED = 127
ROUTER_MAC = '00:77:66:55:44:00'
MAX_ITRS = 120


def nhg_members_count(client, next_hop_group):
    """
    Gets nexthop group members counter value

    Args:
        client (sai_thrift.sai_rpc.Client): RPC client
        next_hop_group (oid): next hop group object ID

    Returns:
        int: ngh members counter
    """
    attr_list = sai_thrift_get_next_hop_group_attribute(client,
                                                        next_hop_group,
                                                        next_hop_count=True)
    next_hop_count = attr_list["SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_COUNT"]
    return next_hop_count


def setup_hash(self):
    """
    Creates hashes
    Returns:
        int: IPv4 hash ID
        int: IPv6 hash ID
    """
    hash_fields_list = [SAI_NATIVE_HASH_FIELD_SRC_IP,
                        SAI_NATIVE_HASH_FIELD_DST_IP,
                        SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                        SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                        SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]
    s32list = sai_thrift_s32_list_t(
        count=len(hash_fields_list), int32list=hash_fields_list)
    # create ECMP hashes
    ipv4_hash_id = sai_thrift_create_hash(
        self.client, native_hash_field_list=s32list)
    self.assertTrue(ipv4_hash_id != 0, "Failed to create IPv4 hash")
    ipv6_hash_id = sai_thrift_create_hash(
        self.client, native_hash_field_list=s32list)
    self.assertTrue(ipv6_hash_id != 0, "Failed to create IPv6 hash")
    # create Lag hashes
    self.lag_ipv4_hash_id = sai_thrift_create_hash(
        self.client, native_hash_field_list=s32list)
    self.assertTrue(self.lag_ipv4_hash_id != 0,
                    "Failed to create LAG IPv4 hash")
    self.lag_ipv6_hash_id = sai_thrift_create_hash(
        self.client, native_hash_field_list=s32list)
    self.assertTrue(self.lag_ipv6_hash_id != 0,
                    "Failed to create LAG IPv6 hash")
    sai_thrift_set_switch_attribute(
        self.client, ecmp_hash_ipv4=ipv4_hash_id)
    sai_thrift_set_switch_attribute(
        self.client, ecmp_hash_ipv6=ipv6_hash_id)
    sai_thrift_set_switch_attribute(
        self.client, lag_hash_ipv4=self.lag_ipv4_hash_id)
    sai_thrift_set_switch_attribute(
        self.client, lag_hash_ipv6=self.lag_ipv6_hash_id)
    return ipv4_hash_id, ipv6_hash_id


def release_hash(self, ipv4_hash_id, ipv6_hash_id):
    """
    Releases hashes
    Args:
        ipv4_hash_id (int): IPv4 hash ID
        ipv6_hash_id (int): IPv6 hash ID
    """
    sai_thrift_set_switch_attribute(self.client, ecmp_hash_ipv4=0)
    sai_thrift_set_switch_attribute(self.client, ecmp_hash_ipv6=0)
    sai_thrift_set_switch_attribute(self.client, lag_hash_ipv4=0)
    sai_thrift_set_switch_attribute(self.client, lag_hash_ipv6=0)
    if self.lag_ipv6_hash_id != 0:
        sai_thrift_remove_hash(self.client, self.lag_ipv6_hash_id)
        self.lag_ipv6_hash_id = 0
    if self.lag_ipv4_hash_id != 0:
        sai_thrift_remove_hash(self.client, self.lag_ipv4_hash_id)
        self.lag_ipv4_hash_id = 0
    if ipv6_hash_id != 0:
        sai_thrift_remove_hash(self.client, ipv6_hash_id)
    if ipv4_hash_id != 0:
        sai_thrift_remove_hash(self.client, ipv4_hash_id)


def print_number_of_available_nhg_resources(self):
    """
    Prints the number of available ngh resources
    """
    print("***** Number of available NHG resources")
    print("self.available_next_hop_group_entry=",
          self.available_next_hop_group_entry)
    print("self.available_next_hop_group_member_entry=",
          self.available_next_hop_group_member_entry)
    print("self.available_ipv4_nexthop_entry=",
          self.available_ipv4_nexthop_entry)
    print("self.available_ipv6_nexthop_entry=",
          self.available_ipv6_nexthop_entry)
    print("self.available_fdb_entry=", self.available_fdb_entry)
    print("self.available_ipv6_route_entry=",
          self.available_ipv6_route_entry)
    print("self.available_ipv4_route_entry=",
          self.available_ipv4_route_entry)


def save_number_of_available_nhg_resources(self, debug=False):
    """
    Saves number of available ngh resources

    Args:
        debug (boolean): debug option indicator
    """
    attr_list = sai_thrift_get_switch_attribute(
        self.client,
        number_of_ecmp_groups=True,
        ecmp_members=True,
        available_next_hop_group_entry=True,
        available_next_hop_group_member_entry=True,
        available_ipv6_nexthop_entry=True,
        available_ipv4_nexthop_entry=True,
        available_fdb_entry=True,
        available_ipv6_route_entry=True,
        available_ipv4_route_entry=True)
    self.available_next_hop_group_entry = attr_list[
        "SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_ENTRY"]
    self.available_next_hop_group_member_entry = attr_list[
        "SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_MEMBER_ENTRY"]
    self.available_ipv4_nexthop_entry = attr_list[
        "SAI_SWITCH_ATTR_AVAILABLE_IPV4_NEXTHOP_ENTRY"]
    self.available_ipv6_nexthop_entry = attr_list[
        "SAI_SWITCH_ATTR_AVAILABLE_IPV6_NEXTHOP_ENTRY"]
    self.available_fdb_entry = attr_list[
        "SAI_SWITCH_ATTR_AVAILABLE_FDB_ENTRY"]
    self.available_ipv6_route_entry = attr_list[
        "SAI_SWITCH_ATTR_AVAILABLE_IPV6_ROUTE_ENTRY"]
    self.available_ipv4_route_entry = attr_list[
        "SAI_SWITCH_ATTR_AVAILABLE_IPV4_ROUTE_ENTRY"]
    if debug:
        print_number_of_available_nhg_resources(self)


@group("draft")
class L3IPv4EcmpHost(SaiHelper):
    """
    Base ECMP tests for IPv4 and ECMP members as regular L3 port RIFs
    """
    def setUp(self):

        super(L3IPv4EcmpHost, self).setUp()

        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'
        # save a number of available resources
        save_number_of_available_nhg_resources(self, False)
        # set switch src mac address
        sai_thrift_set_switch_attribute(
            self.client, src_mac_address=ROUTER_MAC)
        sai_thrift_set_switch_attribute(
            self.client, ecmp_default_hash_seed=TEST_ECMP_SEED)
        sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_seed=TEST_LAG_SEED)
        # test neighbor creation
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port11_rif, sai_ipaddress('10.10.10.1'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=dmac1)
        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port12_rif, sai_ipaddress('10.10.10.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry2, dst_mac_address=dmac2)
        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port11_rif,
            ip=sai_ipaddress('10.10.10.1'))
        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port12_rif,
            ip=sai_ipaddress('10.10.10.2'))
        self.nhop_group1 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop1)
        self.nh_group_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop2)
        # create route entries
        self.route0 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('10.10.10.1/16'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route0, next_hop_id=self.nhop_group1)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # define IPv4 IPv6 LagIPv4Hash and LagIPv6Hash
        self.ipv4_hash_id, self.ipv6_hash_id = setup_hash(self)

    def runTest(self):
        self.l3SaiNhgSetGetTest()
        self.l3IPv4EcmpHostTest()

    def tearDown(self):
        try:
            sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
            sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
            sai_thrift_remove_next_hop_group_member(self.client,
                                                    self.nh_group_member1)
            sai_thrift_remove_next_hop_group_member(self.client,
                                                    self.nh_group_member2)
            self.assertEqual(nhg_members_count(
                self.client, self.nhop_group1), 0)
            sai_thrift_remove_route_entry(self.client, self.route0)
            sai_thrift_remove_next_hop_group(self.client, self.nhop_group1)
            sai_thrift_remove_next_hop(self.client, self.nhop1)
            sai_thrift_remove_next_hop(self.client, self.nhop2)
            resources_valid = self.verifyNumberOfAvaiableResources(debug=True)
            self.assertEqual(resources_valid, True)
            release_hash(self, self.ipv4_hash_id, self.ipv6_hash_id)
        finally:
            super(L3IPv4EcmpHost, self).tearDown()

    def l3SaiNhgSetGetTest(self):
        """
        Checks SAI switch ECMP attributes and validates
        get and set attributes
        """
        print("l3SaiNhgSetGetTest")
        try:
            # predefined NHG self.nhop_group1 with 2 memners
            nhg1_size = 2
            attr_list = sai_thrift_get_switch_attribute(
                self.client,
                number_of_ecmp_groups=True,
                ecmp_members=True,
                available_next_hop_group_entry=True,
                available_next_hop_group_member_entry=True,
                available_ipv6_nexthop_entry=True,
                available_ipv4_nexthop_entry=True)
            available_next_hop_group_entry = attr_list[
                "SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_ENTRY"]
            available_next_hop_group_member_entry = attr_list[
                "SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_MEMBER_ENTRY"]
            available_ipv4_nexthop_entry = attr_list[
                "SAI_SWITCH_ATTR_AVAILABLE_IPV4_NEXTHOP_ENTRY"]
            # Create new empty nhg and check the size of zero
            nhg_xx_size = 0
            nhop_group_xx = sai_thrift_create_next_hop_group(
                self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
            self.assertEqual(nhg_members_count(
                self.client, nhop_group_xx), nhg_xx_size)
            attr_list = sai_thrift_get_switch_attribute(
                self.client,
                number_of_ecmp_groups=True,
                ecmp_members=True,
                available_next_hop_group_member_entry=True,
                available_next_hop_group_entry=True)
            available_next_hop_group_entry2 = attr_list[
                "SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_ENTRY"]
            self.assertEqual(
                available_next_hop_group_entry2,
                (available_next_hop_group_entry - 1))
            self.assertEqual(nhg_members_count(
                self.client, self.nhop_group1), nhg1_size)
            # Add new next hop group member
            nhop3 = sai_thrift_create_next_hop(
                self.client,
                type=SAI_NEXT_HOP_TYPE_IP,
                router_interface_id=self.port10_rif,
                ip=sai_ipaddress('10.10.10.10'))
            nh_group_member3 = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=self.nhop_group1,
                next_hop_id=nhop3)
            nhg1_size += 1
            attr_list = sai_thrift_get_switch_attribute(
                self.client,
                number_of_ecmp_groups=True,
                ecmp_members=True,
                available_next_hop_group_entry=True,
                available_next_hop_group_member_entry=True,
                available_ipv6_nexthop_entry=True,
                available_ipv4_nexthop_entry=True)
            available_next_hop_group_member_entry2 = attr_list[
                "SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_MEMBER_ENTRY"]
            available_ipv4_nexthop_entry2 = attr_list[
                "SAI_SWITCH_ATTR_AVAILABLE_IPV4_NEXTHOP_ENTRY"]
            # The next_group and next_group member both decreases the
            # SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_MEMBER_ENTRY
            self.assertEqual(available_next_hop_group_member_entry2,
                             (available_next_hop_group_member_entry - 2))
            self.assertEqual(available_ipv4_nexthop_entry2,
                             (available_ipv4_nexthop_entry - 1))
            self.assertEqual(nhg_members_count(
                self.client, self.nhop_group1), nhg1_size)
            sai_thrift_remove_next_hop_group_member(
                self.client, nh_group_member3)
            sai_thrift_remove_next_hop(self.client, nhop3)
            nhg1_size -= 1
            self.assertEqual(nhg_members_count(
                self.client, self.nhop_group1), nhg1_size)
        finally:
            # Add new next hop group member to nhop_group_xx
            nhg_xx_nhop1 = sai_thrift_create_next_hop(
                self.client, type=SAI_NEXT_HOP_TYPE_IP,
                router_interface_id=self.port10_rif,
                ip=sai_ipaddress('10.10.10.10'))
            nh_group_xx_member1 = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=nhop_group_xx,
                next_hop_id=nhg_xx_nhop1)
            nhg_xx_size += 1
            self.assertEqual(nhg_members_count(
                self.client, nhop_group_xx), nhg_xx_size)
            # Remove next hop group member
            sai_thrift_remove_next_hop_group_member(
                self.client, nh_group_xx_member1)
            nhg_xx_size -= 1
            self.assertEqual(nhg_members_count(
                self.client, nhop_group_xx), nhg_xx_size)
            self.assertEqual(nhg_xx_size, 0)
            sai_thrift_remove_next_hop(self.client, nhg_xx_nhop1)
            # remove next hop group
            sai_thrift_remove_next_hop_group(self.client, nhop_group_xx)
            attr_list = sai_thrift_get_switch_attribute(
                self.client,
                available_next_hop_group_entry=True,
                available_next_hop_group_member_entry=True,
                available_ipv6_nexthop_entry=True,
                available_ipv4_nexthop_entry=True,
                number_of_ecmp_groups=True,
                ecmp_members=True)
            available_next_hop_group_entry2 = attr_list[
                "SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_ENTRY"]
            available_next_hop_group_member_entry2 = attr_list[
                "SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_MEMBER_ENTRY"]
            available_ipv4_nexthop_entry2 = attr_list[
                "SAI_SWITCH_ATTR_AVAILABLE_IPV4_NEXTHOP_ENTRY"]
            self.assertEqual(available_next_hop_group_entry2,
                             (available_next_hop_group_entry))
            self.assertEqual(available_next_hop_group_member_entry2,
                             (available_next_hop_group_member_entry))
            self.assertEqual(available_ipv4_nexthop_entry2,
                             (available_ipv4_nexthop_entry))
            # check the Next hop group members
            attr_list = sai_thrift_get_next_hop_group_member_attribute(
                self.client,
                self.nh_group_member1,
                next_hop_group_id=True,
                next_hop_id=True)
            self.assertEqual(
                attr_list["SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID"],
                self.nhop_group1)
            self.assertEqual(
                attr_list["SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID"],
                self.nhop1)
            attr_list = sai_thrift_get_next_hop_group_member_attribute(
                self.client,
                self.nh_group_member2,
                next_hop_group_id=True,
                next_hop_id=True)
            self.assertEqual(
                attr_list["SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID"],
                self.nhop_group1)
            self.assertEqual(
                attr_list["SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID"],
                self.nhop2)

    def l3IPv4EcmpHostTest(self):
        """
        IPv4 ECMP tests with all members which are port RIFs
        """
        print("l3IPv4EcmpHostTest")
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.1',
                                ip_src='192.168.0.1',
                                ip_id=106,
                                ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.1',
                                     ip_src='192.168.0.1',
                                     ip_id=106,
                                     # ip_tos=3,
                                     ip_ttl=63)
        exp_pkt2 = simple_tcp_packet(eth_dst='00:11:22:33:44:56',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.2',
                                     ip_src='192.168.0.1',
                                     ip_id=106,
                                     # ip_tos=3,
                                     ip_ttl=63)
        print("Sending packet port %d -> port [%d,%d]"
              "(192.168.100.3 -> 10.10.10.[1,2] [id = 101])" % (
                  self.dev_port13, self.dev_port11, self.dev_port12))
        send_packet(self, self.dev_port13, pkt)
        verify_any_packet_any_port(
            self, [exp_pkt1, exp_pkt2], [self.dev_port11, self.dev_port12])
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.10.10.2',
                                ip_src='192.168.100.3',
                                ip_id=106,
                                ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.1',
                                     ip_src='192.168.100.3',
                                     ip_id=106,
                                     # ip_tos=3,
                                     ip_ttl=63)
        exp_pkt2 = simple_tcp_packet(eth_dst='00:11:22:33:44:56',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.2',
                                     ip_src='192.168.100.3',
                                     ip_id=106,
                                     # ip_tos=3,
                                     ip_ttl=63)
        print("Sending packet port %d -> port [%d,%d]"
              "(192.168.100.3 -> 10.10.10.[1,2] [id = 101])" % (
                  self.dev_port13, self.dev_port11, self.dev_port12))
        send_packet(self, self.dev_port13, pkt)
        verify_any_packet_any_port(
            self, [exp_pkt2, exp_pkt1], [self.dev_port11, self.dev_port12])


@group("draft")
class L3ipv6EcmpHost(SaiHelper):
    """
    Basic ECMP tests for IPv6 and regular L3 port RIFs
    """
    def setUp(self):

        super(L3ipv6EcmpHost, self).setUp()

        ip_addr1 = '5000:1:1:0:0:0:0:1'
        ip_addr2 = '5000:1:1:0:0:0:0:2'
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'
        # save a number of available resources
        save_number_of_available_nhg_resources(self, False)
        # set switch src mac address
        sai_thrift_set_switch_attribute(
            self.client, src_mac_address=ROUTER_MAC)
        sai_thrift_set_switch_attribute(
            self.client, ecmp_default_hash_seed=TEST_ECMP_SEED)
        sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_seed=TEST_LAG_SEED)
        # test neighbor creation
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port11_rif, sai_ipaddress(ip_addr1))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=dmac1)
        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port12_rif, sai_ipaddress(ip_addr2))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry2, dst_mac_address=dmac2)
        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port11_rif,
            ip=sai_ipaddress(ip_addr1))
        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port12_rif,
            ip=sai_ipaddress(ip_addr1))
        self.nhop_group1 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop1)
        self.nh_group_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop2)
        # create route entries
        self.route0 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix(ip_addr1),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route0, next_hop_id=self.nhop_group1)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # define IPv4 IPv6 LagIPv4Hash and LagIPv6Hash
        self.ipv4_hash_id, self.ipv6_hash_id = setup_hash(self)

    def runTest(self):
        self.saiL3ipv6EcmpHostTest()

    def tearDown(self):
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group_member1)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group_member2)
        sai_thrift_remove_route_entry(self.client, self.route0)
        self.assertEqual(nhg_members_count(self.client, self.nhop_group1), 0)
        sai_thrift_remove_next_hop_group(self.client, self.nhop_group1)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        self.assertEqual(True, self.verifyNumberOfAvaiableResources())
        release_hash(self, self.ipv4_hash_id, self.ipv6_hash_id)

        super(L3ipv6EcmpHost, self).tearDown()

    def saiL3ipv6EcmpHostTest(self):
        """
        IPv6 ECMP tests with all members as regular L3 port RIFs
        """
        print("saiL3ipv6EcmpHostTest")
        # send the test packet(s)
        pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                  eth_src='00:22:22:22:22:22',
                                  ipv6_dst='5000:1:1:0:0:0:0:1',
                                  ipv6_src='2000:1:1:0:0:0:0:1',
                                  tcp_sport=0x1234,
                                  ipv6_hlim=64)
        exp_pkt1 = simple_tcpv6_packet(eth_dst='00:11:22:33:44:55',
                                       eth_src=ROUTER_MAC,
                                       ipv6_dst='5000:1:1:0:0:0:0:1',
                                       ipv6_src='2000:1:1:0:0:0:0:1',
                                       tcp_sport=0x1234,
                                       ipv6_hlim=63)
        exp_pkt2 = simple_tcpv6_packet(eth_dst='00:11:22:33:44:56',
                                       eth_src=ROUTER_MAC,
                                       ipv6_dst='5000:1:1:0:0:0:0:1',
                                       ipv6_src='2000:1:1:0:0:0:0:1',
                                       tcp_sport=0x1234,
                                       ipv6_hlim=63)
        send_packet(self, self.dev_port13, pkt)
        verify_any_packet_any_port(
            self, [exp_pkt2, exp_pkt1], [self.dev_port11, self.dev_port12])
        pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                  eth_src='00:22:22:22:22:45',
                                  ipv6_dst='5000:1:1:0:0:0:0:1',
                                  ipv6_src='2000:1:1:0:0:0:0:1',
                                  tcp_sport=0x1248,
                                  ipv6_hlim=64)
        exp_pkt1 = simple_tcpv6_packet(eth_dst='00:11:22:33:44:55',
                                       eth_src=ROUTER_MAC,
                                       ipv6_dst='5000:1:1:0:0:0:0:1',
                                       ipv6_src='2000:1:1:0:0:0:0:1',
                                       tcp_sport=0x1248,
                                       ipv6_hlim=63)
        exp_pkt2 = simple_tcpv6_packet(eth_dst='00:11:22:33:44:56',
                                       eth_src=ROUTER_MAC,
                                       ipv6_dst='5000:1:1:0:0:0:0:1',
                                       ipv6_src='2000:1:1:0:0:0:0:1',
                                       tcp_sport=0x1248,
                                       ipv6_hlim=63)
        send_packet(self, self.dev_port13, pkt)
        verify_any_packet_any_port(
            self, [exp_pkt2, exp_pkt1], [self.dev_port11, self.dev_port12])


@group("draft")
class L3IPv4EcmpLpmTest(SaiHelper):
    """
    Base ECMP tests with LPM routes for IPv4
    """
    def setUp(self):

        super(L3IPv4EcmpLpmTest, self).setUp()

        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:11:22:33:44:56'
        dmac3 = '00:11:22:33:44:57'
        dmac4 = '00:11:22:33:44:58'
        nhop_ip1 = '11.11.11.11'
        nhop_ip1_subnet = '11.11.11.0/16'
        nhop_ip2 = '22.22.22.22'
        nhop_ip2_subnet = '22.22.22.0/16'
        nhop_ip3 = '33.33.33.33'
        nhop_ip3_subnet = '33.33.33.0/16'
        nhop_ip4 = '44.44.44.44'
        # set switch src mac address
        sai_thrift_set_switch_attribute(
            self.client, src_mac_address=ROUTER_MAC)
        sai_thrift_set_switch_attribute(
            self.client, ecmp_default_hash_seed=TEST_ECMP_SEED)
        sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_seed=TEST_LAG_SEED)
        self.port14_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port14,
            admin_v4_state=True)
        self.port15_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port15,
            admin_v4_state=True)
        # test neighbor creation
        self.neighbor_entry11 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port11_rif, sai_ipaddress(nhop_ip1))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry11, dst_mac_address=dmac1)
        self.neighbor_entry12 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port12_rif, sai_ipaddress(nhop_ip2))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry12, dst_mac_address=dmac2)
        self.neighbor_entry13 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port13_rif, sai_ipaddress(nhop_ip3))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry13, dst_mac_address=dmac3)
        self.neighbor_entry14 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port14_rif, sai_ipaddress(nhop_ip4))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry14, dst_mac_address=dmac4)
        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port11_rif,
            ip=sai_ipaddress(nhop_ip1))
        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port12_rif,
            ip=sai_ipaddress(nhop_ip2))
        self.nhop3 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port13_rif,
            ip=sai_ipaddress(nhop_ip3))
        self.nhop_group1 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop1)
        self.nh_group_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop2)
        self.nh_group_member3 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop3)
        # create route entries
        self.route0 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('10.10.10.1/16'),
            vr_id=self.default_vrf)
        self.assertNotEqual(self.route0, 0)
        status = sai_thrift_create_route_entry(
            self.client, self.route0, next_hop_id=self.nhop_group1)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        self.route1 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix(nhop_ip1_subnet),
            vr_id=self.default_vrf)
        self.assertNotEqual(self.route1, 0)
        status = sai_thrift_create_route_entry(
            self.client, self.route1, next_hop_id=self.nhop1)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        self.route2 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix(nhop_ip2_subnet),
            vr_id=self.default_vrf)
        self.assertNotEqual(self.route2, 0)
        status = sai_thrift_create_route_entry(
            self.client, self.route2, next_hop_id=self.nhop2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        self.route3 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix(nhop_ip3_subnet),
            vr_id=self.default_vrf)
        self.assertNotEqual(self.route3, 0)
        status = sai_thrift_create_route_entry(
            self.client, self.route3, next_hop_id=self.nhop3)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # define IPv4 IPv6 LagIPv4Hash and LagIPv6Hash
        self.ipv4_hash_id, self.ipv6_hash_id = setup_hash(self)

    def runTest(self):
        self.l3IPv4EcmpLpmTest()
        self.l3Ipv4EcmpLpmAddRemoveNhopTest()

    def tearDown(self):

        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry11)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry12)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry13)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry14)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group_member1)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group_member2)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group_member3)
        sai_thrift_remove_route_entry(self.client, self.route0)
        sai_thrift_remove_route_entry(self.client, self.route1)
        sai_thrift_remove_route_entry(self.client, self.route2)
        sai_thrift_remove_route_entry(self.client, self.route3)
        sai_thrift_remove_next_hop_group(self.client, self.nhop_group1)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_next_hop(self.client, self.nhop3)
        sai_thrift_remove_router_interface(self.client, self.port14_rif)
        sai_thrift_remove_router_interface(self.client, self.port15_rif)
        release_hash(self, self.ipv4_hash_id, self.ipv6_hash_id)

        super(L3IPv4EcmpLpmTest, self).tearDown()

    def l3IPv4EcmpLpmTest(self):
        """
        Verifies ECMP load balancing with LPM routes configured
        """
        print("l3IPv4EcmpLpmTest")
        count = [0, 0, 0]
        dst_ip = int(binascii.hexlify(socket.inet_aton('10.10.10.1')), 16)
        src_mac_start = '00:22:22:22:22:'
        for i in range(0, MAX_ITRS):
            dst_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
            src_mac = src_mac_start + str(i % 99).zfill(2)
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip_addr,
                                    ip_src='192.168.8.1',
                                    ip_id=106,
                                    ip_ttl=64)
            exp_pkt1 = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            exp_pkt2 = simple_tcp_packet(eth_dst='00:11:22:33:44:56',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            exp_pkt3 = simple_tcp_packet(eth_dst='00:11:22:33:44:57',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            send_packet(self, self.dev_port14, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3],
                [self.dev_port11, self.dev_port12, self.dev_port13])
            count[rcv_idx] += 1
            dst_ip += 1
        for i in range(0, 3):
            self.assertTrue(
                (count[i] >= ((MAX_ITRS / 3) * 0.8)),
                "Not all paths are equally balanced, %s" % count)

    def l3Ipv4EcmpLpmAddRemoveNhopTest(self):  # to be removed from here
        """
        IPv4 ECMP rebalance test with removal of a nexthop member
        """
        print("l3Ipv4EcmpLpmAddRemoveNhopTest")
        src_mac_start = '00:22:22:22:22:'
        nhop_ip4 = '44.44.44.44'
        # Case 1 ECMP 3 members
        # ECMP add new next hop
        nhop4 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port14_rif,
            ip=sai_ipaddress(nhop_ip4))
        nh_group_member4 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=nhop4)
        status = sai_thrift_create_route_entry(
            self.client, self.route3, next_hop_id=nhop4)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        try:
            count = [0, 0, 0, 0]
            dst_ip = int(binascii.hexlify(socket.inet_aton('10.10.10.1')), 16)
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntoa(
                    binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
                src_mac = src_mac_start + str(i % 99).zfill(2)
                pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                        eth_src=src_mac,
                                        ip_dst=dst_ip_addr,
                                        ip_src='192.168.8.1',
                                        ip_id=106,
                                        ip_ttl=64)
                exp_pkt1 = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt2 = simple_tcp_packet(eth_dst='00:11:22:33:44:56',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt3 = simple_tcp_packet(eth_dst='00:11:22:33:44:57',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt4 = simple_tcp_packet(eth_dst='00:11:22:33:44:58',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                send_packet(self, self.dev_port15, pkt)
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], [
                        self.dev_port11, self.dev_port12, self.dev_port13,
                        self.dev_port14
                    ])
                count[rcv_idx] += 1
                dst_ip += 1
            attr_list = sai_thrift_get_next_hop_group_attribute(
                self.client, self.nhop_group1, next_hop_count=True)
            nhg_size = attr_list["SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_COUNT"]
            self.assertEqual(nhg_size, 4)
            for i in range(0, 4):
                self.assertTrue(
                    (count[i] >= ((MAX_ITRS / 4) * 0.5)),
                    "Not all paths are equally balanced, %s" % count)
            # Case 3 Remove next_hop grpup member
            sai_thrift_remove_next_hop_group_member(self.client,
                                                    nh_group_member4)
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntoa(
                    binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
                src_mac = src_mac_start + str(i % 99).zfill(2)
                pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                        eth_src=src_mac,
                                        ip_dst=dst_ip_addr,
                                        ip_src='192.168.8.1',
                                        ip_id=106,
                                        ip_ttl=64)
                exp_pkt1 = simple_tcp_packet(eth_dst='00:11:22:33:44:55',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt2 = simple_tcp_packet(eth_dst='00:11:22:33:44:56',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt3 = simple_tcp_packet(eth_dst='00:11:22:33:44:57',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                send_packet(self, self.dev_port15, pkt)
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2, exp_pkt3],
                    [self.dev_port11, self.dev_port12, self.dev_port13])
                count[rcv_idx] += 1
                dst_ip += 1
            attr_list = sai_thrift_get_next_hop_group_attribute(
                self.client, self.nhop_group1, next_hop_count=True)
            nhg_size = attr_list["SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_COUNT"]
            self.assertEqual(nhg_size, 3)
            for i in range(0, nhg_size):
                self.assertTrue(
                    (count[i] >= ((MAX_ITRS / nhg_size) * 0.8)),
                    "Not all paths are equally balanced, %s" % count)
        finally:
            sai_thrift_remove_next_hop(self.client, nhop4)


@group("draft")
class L3IPv4EcmpLagTest(SaiHelper):
    """
    Base ECMP tests with lag for IPv4
    """
    def setUp(self):

        super(L3IPv4EcmpLagTest, self).setUp()

        dmac1 = '00:11:11:11:11:11'
        dmac2 = '00:22:22:22:22:22'
        dmac3 = '00:33:33:33:33:33'
        dmac4 = '00:44:44:44:44:44'
        dmac5 = '00:55:55:55:55:55'
        dmac6 = '00:66:66:66:66:66'
        nhop_ip1 = '11.11.11.11'
        nhop_ip2 = '22.22.22.22'
        nhop_ip3 = '33.33.33.33'
        nhop_ip4 = '44.44.44.44'
        nhop_ip5 = '44.55.55.55'
        nhop_ip6 = '44.66.66.66'
        # set switch src mac address
        sai_thrift_set_switch_attribute(
            self.client, src_mac_address=ROUTER_MAC)
        sai_thrift_set_switch_attribute(
            self.client, ecmp_default_hash_seed=TEST_ECMP_SEED)
        sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_seed=TEST_LAG_SEED)
        self.lag1_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag1,
            admin_v4_state=True)
        self.lag2_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag2,
            admin_v4_state=True)
        self.port15_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port15,
            admin_v4_state=True)
        # test neighbor creation
        self.neighbor_entry11 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port11_rif, sai_ipaddress(nhop_ip1))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry11, dst_mac_address=dmac1)
        self.neighbor_entry12 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port12_rif, sai_ipaddress(nhop_ip2))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry12, dst_mac_address=dmac2)
        self.neighbor_entry13 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.lag1_rif, sai_ipaddress(nhop_ip3))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry13, dst_mac_address=dmac3)
        self.neighbor_entry14 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.lag2_rif, sai_ipaddress(nhop_ip4))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry14, dst_mac_address=dmac4)
        self.neighbor_entry15 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.lag1_rif, sai_ipaddress(nhop_ip5))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry15, dst_mac_address=dmac5)
        self.neighbor_entry16 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.lag2_rif, sai_ipaddress(nhop_ip6))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry16, dst_mac_address=dmac6)
        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port11_rif,
            ip=sai_ipaddress(nhop_ip1))
        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port12_rif,
            ip=sai_ipaddress(nhop_ip2))
        self.nhop3_lag1 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag1_rif,
            ip=sai_ipaddress(nhop_ip3))
        self.nhop4_lag2 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag2_rif,
            ip=sai_ipaddress(nhop_ip4))
        self.nhop5_lag1 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag1_rif,
            ip=sai_ipaddress(nhop_ip5))
        self.nhop6_lag2 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag2_rif,
            ip=sai_ipaddress(nhop_ip6))
        self.nhop_group1 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group1_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop1)
        self.nh_group1_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop2)
        self.nh_group1_member3 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop3_lag1)
        self.nh_group1_member4 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop4_lag2)
        self.nhop_group2 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group2_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop5_lag1)
        self.nh_group2_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop6_lag2)
        # create route entries
        self.route0 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('10.10.10.1/16'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route0, next_hop_id=self.nhop_group1)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        self.route1 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('20.20.20.1/16'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route1, next_hop_id=self.nhop_group2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # define IPv4 IPv6 LagIPv4Hash and LagIPv6Hash
        self.ipv4_hash_id, self.ipv6_hash_id = setup_hash(self)

    def runTest(self):
        self.l3IPv4EcmpHostTwoLagsTest()
        self.l3IPv4EcmpHostTwoLagsDisabledLagMembersTest()
        self.l3IPv4EcmpHostPortLagTest()
        self.l3IPv4EcmpHostPortLagSharedMembersTest()
        self.l3IPv4EcmpHashPortLagTest()

    def tearDown(self):
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry11)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry12)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry13)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry14)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry15)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry16)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member1)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member2)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member3)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member4)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group2_member1)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group2_member2)
        self.assertEqual(nhg_members_count(self.client, self.nhop_group1), 0)
        self.assertEqual(nhg_members_count(self.client, self.nhop_group2), 0)
        sai_thrift_remove_route_entry(self.client, self.route0)
        sai_thrift_remove_route_entry(self.client, self.route1)
        sai_thrift_remove_next_hop_group(self.client, self.nhop_group1)
        sai_thrift_remove_next_hop_group(self.client, self.nhop_group2)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_next_hop(self.client, self.nhop3_lag1)
        sai_thrift_remove_next_hop(self.client, self.nhop4_lag2)
        sai_thrift_remove_next_hop(self.client, self.nhop5_lag1)
        sai_thrift_remove_next_hop(self.client, self.nhop6_lag2)
        sai_thrift_remove_router_interface(self.client, self.lag1_rif)
        sai_thrift_remove_router_interface(self.client, self.lag2_rif)
        sai_thrift_remove_router_interface(self.client, self.port15_rif)
        release_hash(self, self.ipv4_hash_id, self.ipv6_hash_id)

        super(L3IPv4EcmpLagTest, self).tearDown()

    def l3IPv4EcmpHostTwoLagsDisabledLagMembersTest(self):
        """
        IPv4 ECMP tests with LAG RIF and some LAG member in disable state
        """
        print("l3IPv4EcmpHostTwoLagsDisabledLagMembersTest")
        print("Disable LAG1 member 4 and 5")
        status = sai_thrift_set_lag_member_attribute(
            self.client, self.lag1_member4, egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_lag_member_attribute(
            self.client, self.lag1_member5, egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        try:
            count = [0, 0, 0, 0, 0, 0]
            dst_ip = int(binascii.hexlify(socket.inet_aton('20.20.20.1')), 16)
            src_mac_start = '00:22:22:22:{0}:{1}'
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntoa(
                    binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
                src_mac = src_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
                pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                        eth_src=src_mac,
                                        ip_dst=dst_ip_addr,
                                        ip_src='192.168.8.1',
                                        ip_id=106,
                                        ip_ttl=64)
                exp_pkt1 = simple_tcp_packet(eth_dst='00:55:55:55:55:55',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt2 = simple_tcp_packet(eth_dst='00:66:66:66:66:66',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                send_packet(self, self.dev_port15, pkt)
                ports_to_verify = [
                    self.dev_port4,  # LAG1 ports
                    self.dev_port5,
                    self.dev_port6,
                    self.dev_port7,  # LAG2 ports
                    self.dev_port8,
                    self.dev_port9
                ]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2], ports_to_verify)
                count[rcv_idx] += 1
                dst_ip += 1
            print("PORT lb counts", count)
            ecmp_count = [(count[0] + count[1] + count[2]),
                          (count[3] + count[4] + count[5])]
            # check LAG1 traffic
            self.assertEqual(count[0], 0)  # LAG1 member 1 port4 disabled
            self.assertEqual(count[1], 0)  # LAG1 member 2 port5 disabled
            self.assertTrue((count[2] >= ((MAX_ITRS / 2) * 0.6)),
                            "Lag path1 is not equally balanced")
            print("ECMP count:", ecmp_count)
            for i in range(0, 2):
                self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 2) * 0.75)),
                                "Ecmp paths are not equally balanced")
            # check LAG2 traffic
            for i in range(3, 6):
                self.assertTrue((count[i] >= ((MAX_ITRS / 6) * 0.6)),
                                "Lag path2 is not equally balanced")
        finally:
            print("Enable LAG1 member 4 and 5")
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member4, egress_disable=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member5, egress_disable=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def l3IPv4EcmpHostPortLagSharedMembersTest(self):
        """
        IPv4 multiples ECMP with shared nexthop members
        """
        print("l3IPv4EcmpHostPortLagSharedMembersTest")
        src_mac = '00:01:01:01:01:01'
        # verify NG1 route
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=src_mac,
                                ip_dst='10.10.10.1',
                                ip_src='192.168.8.1',
                                ip_id=106,
                                ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.1',
                                     ip_src='192.168.8.1',
                                     ip_id=106,
                                     ip_ttl=63)
        exp_pkt2 = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.1',
                                     ip_src='192.168.8.1',
                                     ip_id=106,
                                     ip_ttl=63)
        exp_pkt3 = simple_tcp_packet(eth_dst='00:33:33:33:33:33',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.1',
                                     ip_src='192.168.8.1',
                                     ip_id=106,
                                     ip_ttl=63)
        exp_pkt4 = simple_tcp_packet(eth_dst='00:44:44:44:44:44',
                                     eth_src=ROUTER_MAC,
                                     ip_dst='10.10.10.1',
                                     ip_src='192.168.8.1',
                                     ip_id=106,
                                     ip_ttl=63)
        send_packet(self, self.dev_port15, pkt)
        ports_to_verify = [
            self.dev_port11,
            self.dev_port12,
            self.dev_port4,  # LAG1 ports
            self.dev_port5,
            self.dev_port6,
            self.dev_port7,  # LAG2 ports
            self.dev_port8,
            self.dev_port9
        ]
        verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2, exp_pkt3,
                                          exp_pkt4], ports_to_verify)
        # verify NG2 traffic
        dst_ip = "20.20.1.21"
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=src_mac,
                                ip_dst=dst_ip,
                                ip_src='192.168.1.1',
                                ip_id=106,
                                ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(eth_dst='00:55:55:55:55:55',
                                     eth_src=ROUTER_MAC,
                                     ip_dst=dst_ip,
                                     ip_src='192.168.1.1',
                                     ip_id=106,
                                     ip_ttl=63)
        exp_pkt2 = simple_tcp_packet(eth_dst='00:66:66:66:66:66',
                                     eth_src=ROUTER_MAC,
                                     ip_dst=dst_ip,
                                     ip_src='192.168.1.1',
                                     ip_id=106,
                                     ip_ttl=63)
        send_packet(self, self.dev_port15, pkt)
        ports_to_verify = [
            self.dev_port4,  # LAG1 ports
            self.dev_port5,
            self.dev_port6,
            self.dev_port7,  # LAG2 ports
            self.dev_port8,
            self.dev_port9
        ]
        verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2],
                                   ports_to_verify)

    def setupECMPIPv4Hash(self, hash_field_list=None):
        """
        Setups ECMP IPv4 hash

        Args:
            hash_field_list (list): list of hash fields
        """
        print("Setting the ECMP IPv4 hash fields..")
        if hash_field_list is None:
            hash_field_list = [
                SAI_NATIVE_HASH_FIELD_SRC_IP,
                SAI_NATIVE_HASH_FIELD_DST_IP,
                SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                SAI_NATIVE_HASH_FIELD_L4_SRC_PORT
            ]
        s32list = sai_thrift_s32_list_t(
            count=len(hash_field_list), int32list=hash_field_list)
        if self.ipv4_hash_id == 0:
            self.ipv4_hash_id = sai_thrift_create_hash(
                self.client, native_hash_field_list=s32list)
        else:
            status = sai_thrift_set_hash_attribute(
                self.client, self.ipv4_hash_id, native_hash_field_list=s32list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def setupECMPHash(self, hash_fields_list):
        """
        Setups ECMP hash

        Args:
            hash_fields_list (list): list of hash fields
        """
        print("Setting the ECMP hash fields..")
        # Set ECMP HASH
        attr_list = sai_thrift_get_switch_attribute(
            self.client, ecmp_hash=True)
        hash_id = attr_list['SAI_SWITCH_ATTR_ECMP_HASH']
        print("ECMP hash id =0x%x" % (hash_id))

        hash_attr_list = []
        s32list = sai_thrift_s32_list_t(count=100, int32list=hash_attr_list)
        hash_data = sai_thrift_get_hash_attribute(
            self.client, hash_id, native_hash_field_list=s32list)
        data_val = hash_data['native_hash_field_list'].int32list
        print('hash_data: ', hash_data)
        print('hash_val: ', data_val)
        hash_attr_list = hash_fields_list
        hash_field_list = sai_thrift_s32_list_t(
            count=len(hash_attr_list), int32list=hash_attr_list)
        status = sai_thrift_set_hash_attribute(
            self.client, hash_id, native_hash_field_list=hash_field_list)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def l3IPv4EcmpHostTwoLagsTest(self):
        """
        IPv4 ECMP tests with all LAG RIFs members
        """
        print("l3IPv4EcmpHostTwoLagsTest")
        count = [0, 0, 0, 0, 0, 0]
        dst_ip = int(binascii.hexlify(socket.inet_aton('20.20.20.1')), 16)
        src_mac_start = '00:22:22:22:{0}:{1}'
        for i in range(0, MAX_ITRS):
            dst_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
            src_mac = src_mac_start.format(
                str(i).zfill(4)[:2],
                str(i).zfill(4)[2:])
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip_addr,
                                    ip_src='192.168.8.1',
                                    ip_id=106,
                                    ip_ttl=64)
            exp_pkt1 = simple_tcp_packet(eth_dst='00:55:55:55:55:55',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            exp_pkt2 = simple_tcp_packet(eth_dst='00:66:66:66:66:66',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            send_packet(self, self.dev_port15, pkt)
            ports_to_verify = [
                self.dev_port4,  # LAG1 ports
                self.dev_port5,
                self.dev_port6,
                self.dev_port7,  # LAG2 ports
                self.dev_port8,
                self.dev_port9
            ]
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2], ports_to_verify)
            count[rcv_idx] += 1
            dst_ip += 1
        print("PORT lb counts", count)
        ecmp_count = [(count[0] + count[1] + count[2]),
                      (count[3] + count[4] + count[5])]
        print("ECMP count:", ecmp_count)
        for i in range(0, 2):
            self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 2) * 0.7)),
                            "Ecmp paths are not equally balanced")
        for i in range(0, 3):
            self.assertTrue((count[i] >= ((MAX_ITRS / 6) * 0.6)),
                            "Lag path1 is not equally balanced")

        for i in range(3, 6):
            self.assertTrue((count[i] >= ((MAX_ITRS / 6) * 0.6)),
                            "Lag path2 is not equally balanced")

    def l3IPv4EcmpHostPortLagTest(self):
        """
        IPv4 ECMP load balance tests to check fair share on all members
        """
        print("l3IPv4EcmpHostPortLagTest")
        count = [0, 0, 0, 0, 0, 0, 0, 0]
        dst_ip = int(binascii.hexlify(socket.inet_aton('10.10.10.1')), 16)
        for i in range(0, MAX_ITRS):
            dst_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
            src_mac = '00:22:22:22:00:00'
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip_addr,
                                    ip_src='192.168.8.1',
                                    ip_id=106,
                                    ip_ttl=64)
            exp_pkt1 = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            exp_pkt2 = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            exp_pkt3 = simple_tcp_packet(eth_dst='00:33:33:33:33:33',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            exp_pkt4 = simple_tcp_packet(eth_dst='00:44:44:44:44:44',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            send_packet(self, self.dev_port15, pkt)
            ports_to_verify = [
                self.dev_port11,
                self.dev_port12,
                self.dev_port4,  # LAG1 ports
                self.dev_port5,
                self.dev_port6,
                self.dev_port7,  # LAG2 ports
                self.dev_port8,
                self.dev_port9
            ]
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4],
                ports_to_verify)
            count[rcv_idx] += 1
            dst_ip += 1
        print("PORT lb counts", count)
        ecmp_count = [
            count[0],
            count[1],
            (count[2] + count[3] + count[4]),
            (count[5] + count[6] + count[7])]
        print("ECMP count:", ecmp_count)
        for i in range(0, 4):
            self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 4) * 0.5)),
                            "Ecmp paths are not equally balanced")
        for i in range(2, 5):
            self.assertTrue((count[i] >= ((MAX_ITRS / 12) * 0.5)),
                            "Lag path1 is not equally balanced")
        for i in range(5, 8):
            self.assertTrue((count[i] >= ((MAX_ITRS / 12) * 0.5)),
                            "Lag path2 is not equally balanced")

    def l3IPv4EcmpHashPortLagTest(self):
        """
        Creates hash object and sends multiple packets
        Validates that there is no load balancing with various fields
        in ECMP hash
        """
        print("l3IPv4EcmpHashPortLagTest")
        # setup the ECMP hash to SRC IP
        print("Limit ECMP IPv4 hash to SRC_IP only.")
        # for our test it will disable LB.
        self.setupECMPIPv4Hash([SAI_NATIVE_HASH_FIELD_SRC_IP])
        count = [0, 0, 0, 0, 0, 0, 0, 0]
        dst_ip = int(binascii.hexlify(socket.inet_aton('10.10.10.1')), 16)
        for i in range(0, MAX_ITRS):
            dst_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
            src_mac = '00:22:22:22:22:22'
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip_addr,
                                    ip_src='192.168.8.1',
                                    ip_id=106,
                                    ip_ttl=64)
            exp_pkt1 = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            exp_pkt2 = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            exp_pkt3 = simple_tcp_packet(eth_dst='00:33:33:33:33:33',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            exp_pkt4 = simple_tcp_packet(eth_dst='00:44:44:44:44:44',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            send_packet(self, self.dev_port15, pkt)
            ports_to_verify = [
                self.dev_port11,
                self.dev_port12,
                self.dev_port4,  # LAG1 ports
                self.dev_port5,
                self.dev_port6,
                self.dev_port7,  # LAG2 ports
                self.dev_port8,
                self.dev_port9
            ]
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4],
                ports_to_verify)
            count[rcv_idx] += 1
            dst_ip += 1
        print("PORT lb counts:", count)
        ecmp_count = [
            count[0],
            count[1],
            (count[2] + count[3] + count[4]),
            (count[5] + count[6] + count[7])]
        print("ECMP count:", ecmp_count)
        # Traffic should not be ballanced, should apear on single port or
        # LAG
        if ecmp_count[0] != 0:
            self.assertTrue(ecmp_count[0] == MAX_ITRS,
                            "100% expected on this port")
            self.assertTrue(ecmp_count[1] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[2] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[3] == 0,
                            "No traffic expected on this port")
        elif ecmp_count[1] != 0:
            self.assertTrue(ecmp_count[0] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[1] == MAX_ITRS,
                            "100% expected on this port")
            self.assertTrue(ecmp_count[2] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[3] == 0,
                            "No traffic expected on this port")
        elif ecmp_count[2] != 0:
            self.assertTrue(ecmp_count[0] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[1] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[2] == MAX_ITRS,
                            "100% expected on this port")
            self.assertTrue(ecmp_count[3] == 0,
                            "No traffic expected on this port")
        elif ecmp_count[3] != 0:
            self.assertTrue(ecmp_count[0] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[1] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[2] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[3] == MAX_ITRS,
                            "100% expected on this port")
        # enable LB back to IPv4 full fields list
        print("Enable ECMP IPv4 LB")
        self.setupECMPIPv4Hash()
        count = [0, 0, 0, 0, 0, 0, 0, 0]
        dst_ip = int(binascii.hexlify(socket.inet_aton('10.10.10.1')), 16)
        for i in range(0, MAX_ITRS):
            dst_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
            src_mac = '00:22:22:22:22:22'
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip_addr,
                                    ip_src='192.168.8.1',
                                    ip_id=106,
                                    ip_ttl=64)
            exp_pkt1 = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            exp_pkt2 = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            exp_pkt3 = simple_tcp_packet(eth_dst='00:33:33:33:33:33',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            exp_pkt4 = simple_tcp_packet(eth_dst='00:44:44:44:44:44',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src='192.168.8.1',
                                         ip_id=106,
                                         ip_ttl=63)
            send_packet(self, self.dev_port15, pkt)
            ports_to_verify = [
                self.dev_port11,
                self.dev_port12,
                self.dev_port4,  # LAG1 ports
                self.dev_port5,
                self.dev_port6,
                self.dev_port7,  # LAG2 ports
                self.dev_port8,
                self.dev_port9
            ]
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4],
                ports_to_verify)
            count[rcv_idx] += 1
            dst_ip += 1
        print("PORT lb counts:", count)
        ecmp_count = [
            count[0],
            count[1],
            (count[2] + count[3] + count[4]),
            (count[5] + count[6] + count[7])]
        print("ECMP count:", ecmp_count)
        # Traffic should equally be ballanced, should apear on all  port or
        # LAG
        for i in range(0, 4):
            self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 4) * 0.5)),
                            "Ecmp paths are not equally balanced")

    def l3IPv4EcmpHashSeedPortLag(self):
        """
        Changes ECMP seed attribute and checks for rebalancing
        """
        print("l3IPv4EcmpHashSeedPortLag")
        # setup the SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_SEED
        attr = sai_thrift_get_switch_attribute(
            self.client, ecmp_default_hash_seed=200)
        print(attr)
        ecmp_default_hash_seed = attr['ecmp_default_hash_seed']
        status = sai_thrift_set_switch_attribute(
            self.client, ecmp_default_hash_seed=200)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_switch_attribute(
            self.client, ecmp_default_hash_seed=True)
        print(attr)
        self.assertEqual(attr['ecmp_default_hash_seed'], 200)
        try:
            count = [0, 0, 0, 0, 0, 0, 0, 0]
            dst_ip = int(binascii.hexlify(socket.inet_aton('10.10.10.1')), 16)
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntoa(
                    binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
                src_mac = '00:22:22:22:22:22'
                pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                        eth_src=src_mac,
                                        ip_dst=dst_ip_addr,
                                        ip_src='192.168.8.1',
                                        ip_id=106,
                                        ip_ttl=64)
                exp_pkt1 = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt2 = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt3 = simple_tcp_packet(eth_dst='00:33:33:33:33:33',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt4 = simple_tcp_packet(eth_dst='00:44:44:44:44:44',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                send_packet(self, self.dev_port15, pkt)
                ports_to_verify = [
                    self.dev_port11,
                    self.dev_port12,
                    self.dev_port4,  # LAG1 ports
                    self.dev_port5,
                    self.dev_port6,
                    self.dev_port7,  # LAG2 ports
                    self.dev_port8,
                    self.dev_port9
                ]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4],
                    ports_to_verify)
                count[rcv_idx] += 1
                dst_ip += 1
            print("PORT lb counts:", count)
            ecmp_count = [
                count[0],
                count[1],
                (count[2] + count[3] + count[4]),
                (count[5] + count[6] + count[7])]
            print("ECMP count:", ecmp_count)
            # Traffic should equally be ballanced, should apear on all  port or
            # LAG
            for i in range(0, 4):
                self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 4) * 0.5)),
                                "Ecmp paths are not equally balanced")
            # reconfugure the default ECMP hash seed
            sai_thrift_set_switch_attribute(
                self.client, ecmp_default_hash_seed=400)
            self.assertEqual(attr['ecmp_default_hash_seed'], 400)
            count = [0, 0, 0, 0, 0, 0, 0, 0]
            dst_ip = int(binascii.hexlify(socket.inet_aton('10.10.10.1')), 16)
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntoa(
                    binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
                src_mac = '00:22:22:22:22:22'
                pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                        eth_src=src_mac,
                                        ip_dst=dst_ip_addr,
                                        ip_src='192.168.8.1',
                                        ip_id=106,
                                        ip_ttl=64)
                exp_pkt1 = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt2 = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt3 = simple_tcp_packet(eth_dst='00:33:33:33:33:33',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                exp_pkt4 = simple_tcp_packet(eth_dst='00:44:44:44:44:44',
                                             eth_src=ROUTER_MAC,
                                             ip_dst=dst_ip_addr,
                                             ip_src='192.168.8.1',
                                             ip_id=106,
                                             ip_ttl=63)
                send_packet(self, self.dev_port15, pkt)
                ports_to_verify = [
                    self.dev_port11,
                    self.dev_port12,
                    self.dev_port4,  # LAG1 ports
                    self.dev_port5,
                    self.dev_port6,
                    self.dev_port7,  # LAG2 ports
                    self.dev_port8,
                    self.dev_port9
                ]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4],
                    ports_to_verify)
                count[rcv_idx] += 1
                dst_ip += 1
            print("PORT lb counts:", count)
            ecmp_count = [
                count[0],
                count[1],
                (count[2] + count[3] + count[4]),
                (count[5] + count[6] + count[7])]
            print("ECMP count:", ecmp_count)
            # Traffic should equally be ballanced, should apear on all  port or
            # LAG
            for i in range(0, 4):
                self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 4) * 0.75)),
                                "Ecmp paths are not equally balanced")
        finally:
            sai_thrift_set_switch_attribute(
                self.client,
                ecmp_default_hash_seed=ecmp_default_hash_seed)


@group("draft")
class L3IPv6EcmpLagTest(SaiHelper):
    """
    Base ECMP tests with ECMP members as LAG RIFs
    """
    def setUp(self):

        super(L3IPv6EcmpLagTest, self).setUp()

        dmac1 = '00:11:11:11:11:11'
        dmac2 = '00:22:22:22:22:22'
        dmac3 = '00:33:33:33:33:33'
        dmac4 = '00:44:44:44:44:44'
        dmac5 = '00:55:55:55:55:55'
        dmac6 = '00:66:66:66:66:66'
        nhop_ip1 = '1000:1:1:0:0:0:0:1'
        nhop_ip2 = '2000:1:1:0:0:0:0:1'
        nhop_ip3 = '3000:1:1:0:0:0:0:1'
        nhop_ip4 = '4000:1:1:0:0:0:0:1'
        nhop_ip5 = '5000:1:1:0:0:0:0:1'
        nhop_ip6 = '6000:1:1:0:0:0:0:1'
        # set switch src mac address
        sai_thrift_set_switch_attribute(
            self.client, src_mac_address=ROUTER_MAC)
        sai_thrift_set_switch_attribute(
            self.client, ecmp_default_hash_seed=TEST_ECMP_SEED)
        sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_seed=TEST_LAG_SEED)
        self.lag1_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag1,
            admin_v6_state=True)
        self.lag2_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag2,
            admin_v6_state=True)
        self.port15_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port15,
            admin_v6_state=True)
        # test neighbor creation
        self.neighbor_entry11 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port11_rif, sai_ipaddress(nhop_ip1))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry11, dst_mac_address=dmac1)
        self.neighbor_entry12 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port12_rif, sai_ipaddress(nhop_ip2))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry12, dst_mac_address=dmac2)
        self.neighbor_entry13 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.lag1_rif, sai_ipaddress(nhop_ip3))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry13, dst_mac_address=dmac3)
        self.neighbor_entry14 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.lag2_rif, sai_ipaddress(nhop_ip4))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry14, dst_mac_address=dmac4)
        self.neighbor_entry15 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.lag1_rif, sai_ipaddress(nhop_ip5))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry15, dst_mac_address=dmac5)
        self.neighbor_entry16 = sai_thrift_neighbor_entry_t(
            self.switch_id, self.lag2_rif, sai_ipaddress(nhop_ip6))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry16, dst_mac_address=dmac6)
        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port11_rif,
            ip=sai_ipaddress(nhop_ip1))
        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port12_rif,
            ip=sai_ipaddress(nhop_ip2))
        self.nhop3_lag1 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag1_rif,
            ip=sai_ipaddress(nhop_ip3))
        self.nhop4_lag2 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag2_rif,
            ip=sai_ipaddress(nhop_ip4))
        self.nhop5_lag1 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag1_rif,
            ip=sai_ipaddress(nhop_ip5))
        self.nhop6_lag2 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag2_rif,
            ip=sai_ipaddress(nhop_ip6))
        self.nhop_group1 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group1_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop1)
        self.nh_group1_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop2)
        self.nh_group1_member3 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop3_lag1)
        self.nh_group1_member4 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop4_lag2)
        self.nhop_group2 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group2_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop5_lag1)
        self.nh_group2_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop6_lag2)
        # create route entries
        self.route0 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('1000:1:1:0:0:0:0:0/65'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route0, next_hop_id=self.nhop_group1)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        self.route1 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('5500:1:1:0:0:0:0:1/65'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route1, next_hop_id=self.nhop_group2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # define IPv4 IPv6 LagIPv4Hash and LagIPv6Hash
        self.ipv4_hash_id, self.ipv6_hash_id = setup_hash(self)

    def runTest(self):
        self.l3IPv6EcmpHostTwoLagsTest()
        self.l3IPv6EcmpHostTwoLagsDisabledLagMembersTest()
        self.l3IPv6EcmpHostPortLagTest()
        self.l3IPv6EcmpHostPortLagSharedMembersTest()
        self.l3IPv6EcmpHashPortLagTest()
        self.l3Ipv6EcmpAddRemoveNhopTest()
        self.l3Ipv6EcmpLpmTest()

    def tearDown(self):
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry11)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry12)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry13)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry14)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry15)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry16)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member1)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member2)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member3)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member4)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group2_member1)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group2_member2)
        sai_thrift_remove_route_entry(self.client, self.route0)
        sai_thrift_remove_route_entry(self.client, self.route1)
        self.assertEqual(nhg_members_count(self.client, self.nhop_group1), 0)
        self.assertEqual(nhg_members_count(self.client, self.nhop_group2), 0)
        sai_thrift_remove_next_hop_group(self.client, self.nhop_group1)
        sai_thrift_remove_next_hop_group(self.client, self.nhop_group2)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_next_hop(self.client, self.nhop3_lag1)
        sai_thrift_remove_next_hop(self.client, self.nhop4_lag2)
        sai_thrift_remove_next_hop(self.client, self.nhop5_lag1)
        sai_thrift_remove_next_hop(self.client, self.nhop6_lag2)
        sai_thrift_remove_router_interface(self.client, self.lag1_rif)
        sai_thrift_remove_router_interface(self.client, self.lag2_rif)
        sai_thrift_remove_router_interface(self.client, self.port15_rif)
        release_hash(self, self.ipv4_hash_id, self.ipv6_hash_id)

        super(L3IPv6EcmpLagTest, self).tearDown()

    def setupECMPIPv6Hash(self, hash_field_list=None):
        """
        Setups ECMP IPv6 hash
        Args:
            hash_field_list (list): list of hash fields
        """
        print("Setting the ECMP IPv6 hash fields..")
        if hash_field_list is None:
            hash_field_list = [
                SAI_NATIVE_HASH_FIELD_SRC_IP, SAI_NATIVE_HASH_FIELD_DST_IP,
                SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                SAI_NATIVE_HASH_FIELD_L4_SRC_PORT
            ]
        s32list = sai_thrift_s32_list_t(
            count=len(hash_field_list), int32list=hash_field_list)
        if self.ipv6_hash_id == 0:
            self.ipv6_hash_id = sai_thrift_create_hash(
                self.client, native_hash_field_list=s32list)
            self.assertTrue(self.ipv6_hash_id != 0,
                            "Failed to create IPv6 hash")
        else:
            status = sai_thrift_set_hash_attribute(
                self.client, self.ipv6_hash_id, native_hash_field_list=s32list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def l3Ipv6EcmpLpmTest(self):
        """
        Base ECMP tests with LPM routes for IPv6
        """
        print("l3Ipv6EcmpLpmTest")
        dmac7 = '00:77:77:77:77:77'
        nhop_ip7 = '7000:1:1:0:0:0:0:1'
        try:
            # create the IPv6 LPM route entry
            neighbor_entry17 = sai_thrift_neighbor_entry_t(
                self.switch_id, self.lag2_rif, sai_ipaddress(nhop_ip7))
            sai_thrift_create_neighbor_entry(
                self.client, neighbor_entry17, dst_mac_address=dmac7)
            nhop7 = sai_thrift_create_next_hop(
                self.client,
                type=SAI_NEXT_HOP_TYPE_IP,
                router_interface_id=self.lag2_rif,
                ip=sai_ipaddress(nhop_ip7))
            nhop_group3 = sai_thrift_create_next_hop_group(
                self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
            nh_group3_member1 = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=nhop_group3,
                next_hop_id=nhop7)
            route3 = sai_thrift_route_entry_t(
                switch_id=self.switch_id,
                destination=sai_ipprefix('5500:1:1:0:0:0:0:1/70'),
                vr_id=self.default_vrf)
            status = sai_thrift_create_route_entry(
                self.client, route3, next_hop_id=nhop_group3)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            count = [0, 0, 0]
            dst_ip = socket.inet_pton(socket.AF_INET6, '5500:1:1:0:0:0:0:1')
            dst_ip_arr = list(dst_ip)
            src_mac_start = '00:22:22:22:22:22'
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
                src_mac = src_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
                pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src=src_mac,
                                          ipv6_dst=dst_ip_addr,
                                          ipv6_src='7000:1:1:0:0:0:0:1',
                                          ipv6_hlim=64)
                exp_pkt1 = simple_tcpv6_packet(eth_dst='00:77:77:77:77:77',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='7000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                send_packet(self, self.dev_port15, pkt)
                ports_to_verify = [
                    self.dev_port7,  # LAG2 ports
                    self.dev_port8,
                    self.dev_port9]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1], ports_to_verify)
                count[rcv_idx] += 1
                dst_ip_arr[15] = dst_ip_arr[15] + 1
                dst_ip = bytearray(dst_ip_arr)
            print("PORT lb counts", count)
            ecmp_count = [(count[0] + count[1] + count[2])]
            print("ECMP count:", ecmp_count)
            for i in range(0, 3):
                self.assertTrue((count[i] >= ((MAX_ITRS / 3) * 0.75)),
                                "Ecmp paths are not equally balanced")
        finally:
            sai_thrift_remove_neighbor_entry(
                self.client, neighbor_entry17)
            sai_thrift_remove_next_hop_group_member(self.client,
                                                    nh_group3_member1)
            sai_thrift_remove_route_entry(self.client, route3)
            sai_thrift_remove_next_hop_group(self.client, nhop_group3)
            sai_thrift_remove_next_hop(self.client, nhop7)

    def l3IPv6EcmpHostTwoLagsTest(self):
        """
        IPv6 ECMP tests with all members with RIF as LAG
        """
        print("l3IPv6EcmpHostTwoLagsTest")
        count = [0, 0, 0, 0, 0, 0]
        dst_ip = socket.inet_pton(socket.AF_INET6, '5500:1:1:0:0:0:0:1')
        dst_ip_arr = list(dst_ip)
        src_mac_start = '00:22:22:22:22:22'
        for i in range(0, MAX_ITRS):
            dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
            src_mac = src_mac_start.format(
                str(i).zfill(4)[:2],
                str(i).zfill(4)[2:])
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=src_mac,
                                      ipv6_dst=dst_ip_addr,
                                      ipv6_src='6000:1:1:0:0:0:0:1',
                                      ipv6_hlim=64)
            exp_pkt1 = simple_tcpv6_packet(eth_dst='00:55:55:55:55:55',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='6000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            exp_pkt2 = simple_tcpv6_packet(eth_dst='00:66:66:66:66:66',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='6000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            send_packet(self, self.dev_port15, pkt)
            ports_to_verify = [
                self.dev_port4,  # LAG1 ports
                self.dev_port5,
                self.dev_port6,
                self.dev_port7,  # LAG2 ports
                self.dev_port8,
                self.dev_port9
            ]
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2], ports_to_verify)
            count[rcv_idx] += 1
            dst_ip_arr[15] = dst_ip_arr[15] + 1
            dst_ip = bytearray(dst_ip_arr)
        print("PORT lb counts", count)
        ecmp_count = [(count[0] + count[1] + count[2]),
                      (count[3] + count[4] + count[5])]
        print("ECMP count:", ecmp_count)
        for i in range(0, 2):
            self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 2) * 0.5)),
                            "Ecmp paths are not equally balanced")

    def l3IPv6EcmpHostPortLagTest(self):
        """
        IPv6 ECMP tests with members combination of port and LAG RIFs
        """
        print("l3IPv6EcmpHostPortLagTest")
        count = [0, 0, 0, 0, 0, 0, 0, 0]
        dst_ip = socket.inet_pton(socket.AF_INET6, '1000:1:1:0:0:0:0:1')
        dst_ip_arr = list(dst_ip)
        src_mac_start = '00:22:22:22:{0}:{1}'
        for i in range(0, MAX_ITRS):
            dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
            src_mac = src_mac_start.format(
                str(i).zfill(4)[:2],
                str(i).zfill(4)[2:])
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=src_mac,
                                      ipv6_dst=dst_ip_addr,
                                      ipv6_src='5000:1:1:0:0:0:0:1',
                                      ipv6_hlim=64)
            exp_pkt1 = simple_tcpv6_packet(eth_dst='00:11:11:11:11:11',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='5000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            exp_pkt2 = simple_tcpv6_packet(eth_dst='00:22:22:22:22:22',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='5000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            exp_pkt3 = simple_tcpv6_packet(eth_dst='00:33:33:33:33:33',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='5000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            exp_pkt4 = simple_tcpv6_packet(eth_dst='00:44:44:44:44:44',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='5000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            send_packet(self, self.dev_port15, pkt)
            ports_to_verify = [
                self.dev_port11,
                self.dev_port12,
                self.dev_port4,  # LAG1 ports
                self.dev_port5,
                self.dev_port6,
                self.dev_port7,  # LAG2 ports
                self.dev_port8,
                self.dev_port9
            ]
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4],
                ports_to_verify)
            count[rcv_idx] += 1
            dst_ip_arr[15] = dst_ip_arr[15] + 1
            dst_ip = bytearray(dst_ip_arr)
        print("PORT lb counts", count)
        ecmp_count = [
            count[0],
            count[1],
            (count[2] + count[3] + count[4]),
            (count[5] + count[6] + count[7])]
        print("ECMP counts", ecmp_count)
        for i in range(0, 4):
            self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 4) * 0.5)),
                            "Ecmp paths are not equally balanced")

    def l3IPv6EcmpHashPortLagTest(self):
        """
        IPv6 ECMP loads balance tests to check fair share on all members
        """
        print("l3IPv6EcmpHashPortLagTest")
        print("Limit ECMP IPv6 hash to SRC_IP only.")
        # for our test it will disable LB.
        self.setupECMPIPv6Hash([SAI_NATIVE_HASH_FIELD_SRC_IP])
        count = [0, 0, 0, 0, 0, 0, 0, 0]
        dst_ip = socket.inet_pton(socket.AF_INET6, '1000:1:1:0:0:0:0:1')
        dst_ip_arr = list(dst_ip)
        for i in range(0, MAX_ITRS):
            dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
            src_mac = '00:22:22:22:22:22'
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=src_mac,
                                      ipv6_dst=dst_ip_addr,
                                      ipv6_src='5000:1:1:0:0:0:0:1',
                                      ipv6_hlim=64)
            exp_pkt1 = simple_tcpv6_packet(eth_dst='00:11:11:11:11:11',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='5000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            exp_pkt2 = simple_tcpv6_packet(eth_dst='00:22:22:22:22:22',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='5000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            exp_pkt3 = simple_tcpv6_packet(eth_dst='00:33:33:33:33:33',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='5000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            exp_pkt4 = simple_tcpv6_packet(eth_dst='00:44:44:44:44:44',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='5000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            send_packet(self, self.dev_port15, pkt)
            ports_to_verify = [
                self.dev_port11,
                self.dev_port12,
                self.dev_port4,  # LAG1 ports
                self.dev_port5,
                self.dev_port6,
                self.dev_port7,  # LAG2 ports
                self.dev_port8,
                self.dev_port9
            ]
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4],
                ports_to_verify)
            count[rcv_idx] += 1
            dst_ip_arr[15] = dst_ip_arr[15] + 1
            dst_ip = bytearray(dst_ip_arr)
        print("PORT lb counts:", count)
        ecmp_count = [
            count[0],
            count[1],
            (count[2] + count[3] + count[4]),
            (count[5] + count[6] + count[7])]
        print("ECMP count:", ecmp_count)
        # Traffic should not be ballanced, should apear on single port or
        # LAG
        if ecmp_count[0] != 0:
            self.assertTrue(ecmp_count[0] == MAX_ITRS,
                            "100% expected on this port")
            self.assertTrue(ecmp_count[1] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[2] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[3] == 0,
                            "No traffic expected on this port")
        elif ecmp_count[1] != 0:
            self.assertTrue(ecmp_count[0] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[1] == MAX_ITRS,
                            "100% expected on this port")
            self.assertTrue(ecmp_count[2] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[3] == 0,
                            "No traffic expected on this port")
        elif ecmp_count[2] != 0:
            self.assertTrue(ecmp_count[0] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[1] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[2] == MAX_ITRS,
                            "100% expected on this port")
            self.assertTrue(ecmp_count[3] == 0,
                            "No traffic expected on this port")
        elif ecmp_count[3] != 0:
            self.assertTrue(ecmp_count[0] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[1] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[2] == 0,
                            "No traffic expected on this port")
            self.assertTrue(ecmp_count[3] == MAX_ITRS,
                            "100% expected on this port")
        # enable LB back to IPv4 full fields list
        print("Enable ECMP IPv6 LB")
        self.setupECMPIPv6Hash()
        count = [0, 0, 0, 0, 0, 0, 0, 0]
        dst_ip = socket.inet_pton(socket.AF_INET6, '1000:1:1:0:0:0:0:1')
        dst_ip_arr = list(dst_ip)
        for i in range(0, MAX_ITRS):
            dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
            src_mac = '00:22:22:22:22:22'
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=src_mac,
                                      ipv6_dst=dst_ip_addr,
                                      ipv6_src='5000:1:1:0:0:0:0:1',
                                      ipv6_hlim=64)
            exp_pkt1 = simple_tcpv6_packet(eth_dst='00:11:11:11:11:11',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='5000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            exp_pkt2 = simple_tcpv6_packet(eth_dst='00:22:22:22:22:22',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='5000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            exp_pkt3 = simple_tcpv6_packet(eth_dst='00:33:33:33:33:33',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='5000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            exp_pkt4 = simple_tcpv6_packet(eth_dst='00:44:44:44:44:44',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src='5000:1:1:0:0:0:0:1',
                                           ipv6_hlim=63)
            send_packet(self, self.dev_port15, pkt)
            ports_to_verify = [
                self.dev_port11,
                self.dev_port12,
                self.dev_port4,
                self.dev_port5,
                self.dev_port6,
                self.dev_port7,
                self.dev_port8,
                self.dev_port9
            ]
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4],
                ports_to_verify)
            count[rcv_idx] += 1
            dst_ip_arr[15] = dst_ip_arr[15] + 1
            dst_ip = bytearray(dst_ip_arr)
        print("PORT lb counts:", count)
        ecmp_count = [
            count[0],
            count[1],
            (count[2] + count[3] + count[4]),
            (count[5] + count[6] + count[7])]
        print("ECMP count:", ecmp_count)
        # Traffic should equally be ballanced, should apear on all  port or
        # LAG
        for i in range(0, 4):
            print("ecmp_count=", ecmp_count, (MAX_ITRS / 4) * 0.5)
            self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 4) * 0.5)),
                            "Ecmp paths are not equally balanced")

    def l3IPv6EcmpHostTwoLagsDisabledLagMembersTest(self):
        """
        IPv6 ECMP tests with LAG RIF and some LAG member in disable state
        """
        print("l3IPv6EcmpHostTwoLagsDisabledLagMembersTest")
        print("Disable LAG1 member 4 and 5")
        print("Disable LAG2 member 7")
        status = sai_thrift_set_lag_member_attribute(
            self.client, self.lag1_member4, egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_lag_member_attribute(
            self.client, self.lag1_member5, egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_lag_member_attribute(
            self.client, self.lag2_member7, egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        try:
            count = [0, 0, 0, 0, 0, 0]
            dst_ip = socket.inet_pton(socket.AF_INET6, '5500:1:1:0:0:0:0:1')
            dst_ip_arr = list(dst_ip)
            src_mac_start = '00:22:22:22:{0}:{1}'
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
                src_mac = src_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
                pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src=src_mac,
                                          ipv6_dst=dst_ip_addr,
                                          ipv6_src='6000:1:1:0:0:0:0:1',
                                          ipv6_hlim=64)
                exp_pkt1 = simple_tcpv6_packet(eth_dst='00:55:55:55:55:55',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='6000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                exp_pkt2 = simple_tcpv6_packet(eth_dst='00:66:66:66:66:66',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='6000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                send_packet(self, self.dev_port15, pkt)
                ports_to_verify = [
                    self.dev_port4,
                    self.dev_port5,
                    self.dev_port6,
                    self.dev_port7,
                    self.dev_port8,
                    self.dev_port9
                ]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2], ports_to_verify)
                count[rcv_idx] += 1
                dst_ip_arr[15] = dst_ip_arr[15] + 1
                dst_ip = bytearray(dst_ip_arr)
            print("PORT lb counts:", count)
            ecmp_count = [(count[0] + count[1] + count[2]),
                          (count[3] + count[4] + count[5])]
            print("ECMP count:", ecmp_count)
            # check LAG1 traffic
            self.assertEqual(count[0], 0)
            self.assertEqual(count[1], 0)
            self.assertTrue((count[2] >= ((MAX_ITRS / 2) * 0.7)),
                            "Lag path1 is not equally balanced")
            # check LAG2 traffic
            self.assertEqual(count[3], 0)
            self.assertTrue((count[4] >= ((MAX_ITRS / 4) * 0.5)),
                            "Lag path2 is not equally balanced")
            self.assertTrue((count[5] >= ((MAX_ITRS / 4) * 0.5)),
                            "Lag path2 is not equally balanced")
            print(ecmp_count)
            for i in range(0, 2):
                self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 2) * 0.5)),
                                "Ecmp paths are not equally balanced")
        finally:
            print("Enable LAG1 member 4 and 5")
            print("Enable LAG2 member 7")
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member4, egress_disable=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member5, egress_disable=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag2_member7, egress_disable=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def l3Ipv6EcmpAddRemoveNhopTest(self):
        """
        IPv6 ECMP rebalance test with removal of a nexthop member
        """
        print("l3Ipv6EcmpAddRemoveNhopTest")
        print("Add new nhg member")
        dmac7 = '00:77:77:77:77:77'
        nhop_new_ip = '7000:1:1:0:0:0:0:1'
        neighbor_entry = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port13_rif, sai_ipaddress(nhop_new_ip))
        sai_thrift_create_neighbor_entry(
            self.client, neighbor_entry, dst_mac_address=dmac7)
        new_next_hop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port13_rif,
            ip=sai_ipaddress(nhop_new_ip))
        new_nhg1_member = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=new_next_hop)
        try:
            count = [0, 0, 0, 0, 0, 0, 0, 0, 0]
            dst_ip = socket.inet_pton(socket.AF_INET6, '1000:1:1:0:0:0:0:1')
            dst_ip_arr = list(dst_ip)
            src_mac_start = '00:22:22:22:{0}:{1}'
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
                src_mac = src_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
                pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src=src_mac,
                                          ipv6_dst=dst_ip_addr,
                                          ipv6_src='5000:1:1:0:0:0:0:1',
                                          ipv6_hlim=64)
                exp_pkt1 = simple_tcpv6_packet(eth_dst='00:11:11:11:11:11',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                exp_pkt2 = simple_tcpv6_packet(eth_dst='00:22:22:22:22:22',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                exp_pkt3 = simple_tcpv6_packet(eth_dst='00:33:33:33:33:33',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                exp_pkt4 = simple_tcpv6_packet(eth_dst='00:44:44:44:44:44',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                exp_pkt5 = simple_tcpv6_packet(eth_dst='00:77:77:77:77:77',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                send_packet(self, self.dev_port15, pkt)
                ports_to_verify = [
                    self.dev_port11,
                    self.dev_port12,
                    self.dev_port4,
                    self.dev_port5,
                    self.dev_port6,
                    self.dev_port7,
                    self.dev_port8,
                    self.dev_port9,
                    self.dev_port13
                ]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4, exp_pkt5],
                    ports_to_verify)
                count[rcv_idx] += 1
                dst_ip_arr[15] = dst_ip_arr[15] + 1
                dst_ip = bytearray(dst_ip_arr)
            print("PORT lb counts:", count)
            ecmp_count = [
                count[0],
                count[1],
                (count[2] + count[3] + count[4]),
                (count[5] + count[6] + count[7]),
                count[8]]
            print("ECMP count :", ecmp_count)
            for i in range(0, 5):
                self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 5) * 0.5)),
                                "Ecmp paths are not equally balanced")
        finally:
            print("Remove newly created nhg member")
            sai_thrift_remove_neighbor_entry(self.client, neighbor_entry)
            sai_thrift_remove_next_hop_group_member(self.client,
                                                    new_nhg1_member)
            sai_thrift_remove_next_hop(self.client, new_next_hop)
            count = [0, 0, 0, 0, 0, 0, 0, 0, 0]
            dst_ip = socket.inet_pton(socket.AF_INET6, '1000:1:1:0:0:0:0:1')
            dst_ip_arr = list(dst_ip)
            src_mac_start = '00:22:22:22:{0}:{1}'
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
                src_mac = src_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
                pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src=src_mac,
                                          ipv6_dst=dst_ip_addr,
                                          ipv6_src='5000:1:1:0:0:0:0:1',
                                          ipv6_hlim=64)
                exp_pkt1 = simple_tcpv6_packet(eth_dst='00:11:11:11:11:11',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                exp_pkt2 = simple_tcpv6_packet(eth_dst='00:22:22:22:22:22',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                exp_pkt3 = simple_tcpv6_packet(eth_dst='00:33:33:33:33:33',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                exp_pkt4 = simple_tcpv6_packet(eth_dst='00:44:44:44:44:44',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                send_packet(self, self.dev_port15, pkt)
                ports_to_verify = [
                    self.dev_port11,
                    self.dev_port12,
                    self.dev_port4,
                    self.dev_port5,
                    self.dev_port6,
                    self.dev_port7,
                    self.dev_port8,
                    self.dev_port9,
                    self.dev_port13
                ]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4],
                    ports_to_verify)
                count[rcv_idx] += 1
                dst_ip_arr[15] = dst_ip_arr[15] + 1
                dst_ip = bytearray(dst_ip_arr)
            print("Port lb counts:", count)
            ecmp_count = [
                count[0],
                count[1],
                (count[2] + count[3] + count[4]),
                (count[5] + count[6] + count[7]),
                count[8]]
            print("ECMP count :", ecmp_count)
            self.assertTrue(ecmp_count[4] == 0,
                            "No traffic expected on removed nhg member")
            for i in range(0, 4):
                self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 4) * 0.5)),
                                "Ecmp paths are not equally balanced")

    def l3IPv6EcmpHostPortLagSharedMembersTest(self):
        """
        IPv6 Multiple ECMP with shared nexthop members
        """
        print("l3IPv6EcmpHostPortLagSharedMembersTest")
        try:
            count = [0, 0, 0, 0, 0, 0]
            dst_ip = socket.inet_pton(socket.AF_INET6, '5500:1:1:0:0:0:0:1')
            dst_ip_arr = list(dst_ip)
            src_mac_start = '00:22:22:22:{0}:{1}'
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
                src_mac = src_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
                pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src=src_mac,
                                          ipv6_dst=dst_ip_addr,
                                          ipv6_src='6000:1:1:0:0:0:0:1',
                                          ipv6_hlim=64)
                exp_pkt1 = simple_tcpv6_packet(eth_dst='00:55:55:55:55:55',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='6000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                exp_pkt2 = simple_tcpv6_packet(eth_dst='00:66:66:66:66:66',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='6000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                send_packet(self, self.dev_port15, pkt)
                ports_to_verify = [
                    self.dev_port4,
                    self.dev_port5,
                    self.dev_port6,
                    self.dev_port7,
                    self.dev_port8,
                    self.dev_port9
                ]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2], ports_to_verify)
                count[rcv_idx] += 1
                dst_ip_arr[15] = dst_ip_arr[15] + 1
                dst_ip = bytearray(dst_ip_arr)
            print("PORT lb counts:", count)
            ecmp_count = [(count[0] + count[1] + count[2]),
                          (count[3] + count[4] + count[5])]
            print("ECMP count:", ecmp_count)
            # check LAG1 traffic
            for i in range(0, 3):
                self.assertTrue((count[i] >= ((MAX_ITRS / 6) * 0.75)),
                                "Lag path1 is not equally balanced")
            # check LAG2 traffic
            for i in range(3, 6):
                self.assertTrue((count[i] >= ((MAX_ITRS / 6) * 0.5)),
                                "Lag path2 is not equally balanced")
            print(ecmp_count)
            for i in range(0, 2):
                self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 2) * 0.5)),
                                "Ecmp paths are not equally balanced")
        finally:
            count = [0, 0, 0, 0, 0, 0, 0, 0]
            dst_ip = socket.inet_pton(socket.AF_INET6, '1000:1:1:0:0:0:0:1')
            dst_ip_arr = list(dst_ip)
            src_mac_start = '00:22:22:22:{0}:{1}'
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
                src_mac = src_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
                pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src=src_mac,
                                          ipv6_dst=dst_ip_addr,
                                          ipv6_src='5000:1:1:0:0:0:0:1',
                                          ipv6_hlim=64)
                exp_pkt1 = simple_tcpv6_packet(eth_dst='00:11:11:11:11:11',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                exp_pkt2 = simple_tcpv6_packet(eth_dst='00:22:22:22:22:22',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                exp_pkt3 = simple_tcpv6_packet(eth_dst='00:33:33:33:33:33',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                exp_pkt4 = simple_tcpv6_packet(eth_dst='00:44:44:44:44:44',
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=dst_ip_addr,
                                               ipv6_src='5000:1:1:0:0:0:0:1',
                                               ipv6_hlim=63)
                send_packet(self, self.dev_port15, pkt)
                ports_to_verify = [
                    self.dev_port11,
                    self.dev_port12,
                    self.dev_port4,
                    self.dev_port5,
                    self.dev_port6,
                    self.dev_port7,
                    self.dev_port8,
                    self.dev_port9
                ]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4],
                    ports_to_verify)
                count[rcv_idx] += 1
                dst_ip_arr[15] = dst_ip_arr[15] + 1
                dst_ip = bytearray(dst_ip_arr)
            print("Port lb counts:", count)
            ecmp_count = [
                count[0],
                count[1],
                (count[2] + count[3] + count[4]),
                (count[5] + count[6] + count[7])]
            print("ECMP count :", ecmp_count)
            for i in range(0, 4):
                self.assertTrue((ecmp_count[i] >= ((MAX_ITRS / 4) * 0.5)),
                                "Ecmp paths are not equally balanced")


@group("draft")
class L3IPv4SVIEcmpTest(SaiHelper):
    """
    Base ECMP tests for IPv4 and ECMP members as SVI RIFs
    """
    def setUp(self):

        super(L3IPv4SVIEcmpTest, self).setUp()

        self.vlan100_rif_counter_in = 0
        self.vlan100_rif_counter_out = 0
        self.vlan200_rif_counter_in = 0
        self.vlan200_rif_counter_out = 0
        self.vlan100_bcast_in = 0
        self.vlan100_bcast_out = 0
        self.vlan200_bcast_in = 0
        self.vlan200_bcast_out = 0
        # set switch src mac address
        sai_thrift_set_switch_attribute(
            self.client, src_mac_address=ROUTER_MAC)
        sai_thrift_set_switch_attribute(
            self.client, ecmp_default_hash_seed=TEST_ECMP_SEED)
        sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_seed=TEST_LAG_SEED)
        self.port24_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.port25_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port25,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.port26_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port26,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
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
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:22:22:33:44:55'
        dmac3 = '00:33:22:33:44:55'
        dmac4 = '00:44:22:33:44:55'
        dmac5 = '00:11:33:33:44:55'
        dmac6 = '00:22:33:33:44:55'
        dmac7 = '00:44:33:33:44:55'
        # create nhop1, nhop2 & nhop3 on SVI
        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.1'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, ip_address=sai_ipaddress('10.10.0.1'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=dmac1)
        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.2'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, ip_address=sai_ipaddress('10.10.0.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry2, dst_mac_address=dmac2)
        self.nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('10.10.0.3'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry3 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, ip_address=sai_ipaddress('10.10.0.3'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry3, dst_mac_address=dmac3)
        self.nhop_group1 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group1_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop1)
        self.nh_group1_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop2)
        self.nh_group1_member3 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop3)
        # create route entries
        self.route0 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('10.20.30.1/16'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route0, next_hop_id=self.nhop_group1)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # create nhop and route to L2 intf
        self.nhop4 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('11.11.0.2'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry4 = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('11.11.0.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry4, dst_mac_address=dmac4)
        self.lag10 = sai_thrift_create_lag(self.client)
        self.lag10_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag10,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.lag10_member30 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag10, port_id=self.port30)
        self.lag10_member31 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag10, port_id=self.port31)
        self.lag11 = sai_thrift_create_lag(self.client)
        self.lag11_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag11,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.lag11_member28 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag11, port_id=self.port28)
        self.lag11_member29 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag11, port_id=self.port29)
        # create vlan200_rif
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
            self.client, self.neighbor_entry5, dst_mac_address=dmac5)
        self.nhop6 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('20.10.0.2'),
            router_interface_id=self.vlan200_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry6 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan200_rif, ip_address=sai_ipaddress('20.10.0.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry6, dst_mac_address=dmac6)
        self.nhop7 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('21.11.0.2'),
            router_interface_id=self.port11_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry7 = sai_thrift_neighbor_entry_t(
            rif_id=self.port11_rif, ip_address=sai_ipaddress('21.11.0.2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry7, dst_mac_address=dmac7)
        self.nhop_group2 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group2_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop4)
        self.nh_group2_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop5)
        self.nh_group2_member3 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop6)
        self.nh_group2_member4 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop7)
        self.route1 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('10.40.40.1/16'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route1, next_hop_id=self.nhop_group2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # define IPv4 IPv6 LagIPv4Hash and LagIPv6Hash
        self.ipv4_hash_id, self.ipv6_hash_id = setup_hash(self)

    def runTest(self):
        self.l3IPv4EcmpSVIHostTest()
        self.l3IPv4EcmpSVILagHostTest()

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry4)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry5)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry6)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry7)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group2_member1)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group2_member2)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group2_member3)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group2_member4)
        self.assertEqual(nhg_members_count(self.client, self.nhop_group2), 0)
        sai_thrift_remove_next_hop_group(self.client, self.nhop_group2)
        sai_thrift_remove_next_hop(self.client, self.nhop4)
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
        sai_thrift_remove_route_entry(self.client, self.route0)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry3)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member1)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member2)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member3)
        sai_thrift_remove_next_hop_group(self.client, self.nhop_group1)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_next_hop(self.client, self.nhop3)
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
        release_hash(self, self.ipv4_hash_id, self.ipv6_hash_id)

        super(L3IPv4SVIEcmpTest, self).tearDown()

    def l3IPv4EcmpSVIHostTest(self):
        """
        IPv4 ECMP tests with SVI RIF as member
        """
        print("l3IPv4EcmpSVIHostTest")
        mac_action = SAI_PACKET_ACTION_FORWARD
        fdb_entry1 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address='00:11:22:33:44:55',
            bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry1,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port24_bp,
            packet_action=mac_action)
        fdb_entry2 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address='00:22:22:33:44:55',
            bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry2,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port25_bp,
            packet_action=mac_action)
        fdb_entry3 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address='00:33:22:33:44:55',
            bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry3,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port26_bp,
            packet_action=mac_action)
        try:
            count = [0, 0, 0]
            dst_ip = int(binascii.hexlify(socket.inet_aton('10.20.30.1')), 16)
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntoa(
                    binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
                pkt = simple_tcp_packet(
                    eth_dst=ROUTER_MAC,
                    eth_src='00:22:22:22:22:22',
                    ip_dst=dst_ip_addr,
                    ip_src='192.168.0.1',
                    ip_id=105,
                    ip_ttl=64)
                exp_pkt1 = simple_tcp_packet(
                    eth_dst='00:11:22:33:44:55',
                    eth_src=ROUTER_MAC,
                    ip_dst=dst_ip_addr,
                    ip_src='192.168.0.1',
                    ip_id=105,
                    ip_ttl=63)
                exp_pkt2 = simple_tcp_packet(
                    eth_dst='00:22:22:33:44:55',
                    eth_src=ROUTER_MAC,
                    ip_dst=dst_ip_addr,
                    ip_src='192.168.0.1',
                    ip_id=105,
                    ip_ttl=63)
                exp_pkt3 = simple_tcp_packet(
                    eth_dst='00:33:22:33:44:55',
                    eth_src=ROUTER_MAC,
                    ip_dst=dst_ip_addr,
                    ip_src='192.168.0.1',
                    ip_id=105,
                    ip_ttl=63)
                send_packet(self, self.dev_port10, pkt)
                self.vlan100_rif_counter_out += 1
                ports_to_verify = [
                    self.dev_port24, self.dev_port25, self.dev_port26
                ]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2, exp_pkt3], ports_to_verify)
                count[rcv_idx] += 1
                dst_ip += 1
            print("Port LB counts:", count)
            for i in range(0, 3):
                self.assertTrue(
                    (count[i] >= ((MAX_ITRS / 3) * 0.5)),
                    "Not all paths are equally balanced, %s" % count)
        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry1)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry2)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry3)

    def l3IPv4EcmpSVILagHostTest(self):
        """
        IPv4 ECMP tests with Port, LAG and SVI RIFs as nexthop members
        """
        print("l3IPv4EcmpSVILagHostTest")
        mac_action = SAI_PACKET_ACTION_FORWARD
        fdb_entry1 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address='00:11:33:33:44:55',
            bv_id=self.vlan200)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry1,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.lag10_bp,
            packet_action=mac_action)
        fdb_entry2 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address='00:22:33:33:44:55',
            bv_id=self.vlan200)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry2,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.lag11_bp,
            packet_action=mac_action)
        try:
            count = [0, 0, 0, 0, 0, 0]
            dst_ip = int(binascii.hexlify(socket.inet_aton('10.40.40.1')), 16)
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntoa(
                    binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
                pkt = simple_tcp_packet(
                    eth_dst=ROUTER_MAC,
                    eth_src='00:22:22:22:22:22',
                    ip_dst=dst_ip_addr,
                    ip_src='192.168.0.1',
                    ip_id=105,
                    ip_ttl=64)
                exp_pkt1 = simple_tcp_packet(
                    eth_dst='00:44:22:33:44:55',
                    eth_src=ROUTER_MAC,
                    ip_dst=dst_ip_addr,
                    ip_src='192.168.0.1',
                    ip_id=105,
                    ip_ttl=63)
                exp_pkt2 = simple_tcp_packet(
                    eth_dst='00:11:33:33:44:55',
                    eth_src=ROUTER_MAC,
                    ip_dst=dst_ip_addr,
                    ip_src='192.168.0.1',
                    ip_id=105,
                    ip_ttl=63)
                exp_pkt3 = simple_tcp_packet(
                    eth_dst='00:22:33:33:44:55',
                    eth_src=ROUTER_MAC,
                    ip_dst=dst_ip_addr,
                    ip_src='192.168.0.1',
                    ip_id=105,
                    ip_ttl=63)
                exp_pkt4 = simple_tcp_packet(
                    eth_dst='00:44:33:33:44:55',
                    eth_src=ROUTER_MAC,
                    ip_dst=dst_ip_addr,
                    ip_src='192.168.0.1',
                    ip_id=105,
                    ip_ttl=63)
                send_packet(self, self.dev_port10, pkt)
                ports_to_verify = [
                    self.dev_port10,
                    self.dev_port11,
                    self.dev_port28,
                    self.dev_port29,
                    self.dev_port30,
                    self.dev_port31]
                rcv_idx = verify_any_packet_any_port(
                    self,
                    [exp_pkt1,
                     exp_pkt2,
                     exp_pkt3,
                     exp_pkt4],
                    ports_to_verify)
                count[rcv_idx] += 1
                dst_ip += 1
            ecmp_count = [
                count[0], count[1], (count[2] + count[3]),
                (count[4] + count[5])
            ]
            print("PORT lb counts", count)
            for i in range(0, 4):
                print("ECMP count LB[%d], %d, %d%%" % (
                    i, ecmp_count[i], ecmp_count[i] * 100 / MAX_ITRS))
            for i in range(0, 4):
                self.assertTrue(
                    (ecmp_count[i] >= ((MAX_ITRS / 4) * 0.5)),
                    "Not all paths are equally balanced, %s" % count)
        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry1)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry2)


@group("draft")
class L3IPv6SVIEcmpTest(SaiHelper):
    """
    Base ECMP tests for IPv6 and ECMP members as SVI RIFs
    """
    def setUp(self):

        super(L3IPv6SVIEcmpTest, self).setUp()

        self.vlan100_rif_counter_in = 0
        self.vlan100_rif_counter_out = 0
        self.vlan200_rif_counter_in = 0
        self.vlan200_rif_counter_out = 0
        self.vlan100_bcast_in = 0
        self.vlan100_bcast_out = 0
        self.vlan200_bcast_in = 0
        self.vlan200_bcast_out = 0
        # set switch src mac address
        sai_thrift_set_switch_attribute(
            self.client, src_mac_address=ROUTER_MAC)
        sai_thrift_set_switch_attribute(
            self.client, ecmp_default_hash_seed=TEST_ECMP_SEED)
        sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_seed=TEST_LAG_SEED)
        self.port24_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.port25_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port25,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.port26_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port26,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.port10_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port10,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.port11_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port11,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
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
            vlan_id=self.vlan100,
            admin_v6_state=True)
        dmac1 = '00:11:22:33:44:55'
        dmac2 = '00:22:22:33:44:55'
        dmac3 = '00:33:22:33:44:55'
        dmac4 = '00:11:00:00:00:04'
        dmac5 = '00:11:00:00:00:05'
        dmac6 = '00:11:00:00:00:06'
        dmac7 = '00:11:00:00:00:07'
        # create nhop1, nhop2 & nhop3 on SVI
        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('1000:1:1:0:0:0:0:1'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif,
            ip_address=sai_ipaddress('1000:1:1:0:0:0:0:1'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=dmac1)
        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('1000:1:1:0:0:0:0:2'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif,
            ip_address=sai_ipaddress('1000:1:1:0:0:0:0:2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry2, dst_mac_address=dmac2)
        self.nhop3 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('1000:1:1:0:0:0:0:3'),
            router_interface_id=self.vlan100_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry3 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif,
            ip_address=sai_ipaddress('1000:1:1:0:0:0:0:3'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry3, dst_mac_address=dmac3)
        self.nhop_group1 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group1_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop1)
        self.nh_group1_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop2)
        self.nh_group1_member3 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop3)
        # create route entries
        self.route0 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('4000:1:1:0:0:0:0:1/16'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route0, next_hop_id=self.nhop_group1)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # create nhop and route to L2 intf
        self.lag10 = sai_thrift_create_lag(self.client)
        self.lag10_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag10,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.lag10_member30 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag10, port_id=self.port30)
        self.lag10_member31 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag10, port_id=self.port31)
        self.lag11 = sai_thrift_create_lag(self.client)
        self.lag11_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag11,
            type=SAI_BRIDGE_PORT_TYPE_PORT)
        self.lag11_member28 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag11, port_id=self.port28)
        self.lag11_member29 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag11, port_id=self.port29)
        # create vlan200_rif
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
        self.vlan_member202 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan200,
            bridge_port_id=self.port10_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_member203 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan200,
            bridge_port_id=self.port11_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_lag_attribute(self.client, self.lag10, port_vlan_id=200)
        sai_thrift_set_lag_attribute(self.client, self.lag11, port_vlan_id=200)
        sai_thrift_set_port_attribute(
            self.client, self.port10, port_vlan_id=200)
        sai_thrift_set_port_attribute(
            self.client, self.port11, port_vlan_id=200)
        self.vlan200_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan200,
            admin_v6_state=True)
        self.nhop4 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('2010:1:1:0:0:0:0:1'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry4 = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif,
            ip_address=sai_ipaddress('2010:1:1:0:0:0:0:1'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry4, dst_mac_address=dmac4)
        # Create nhop5 and nhop6 on SVI
        self.nhop5 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('2010:1:1:0:0:0:0:2'),
            router_interface_id=self.vlan200_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry5 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan200_rif,
            ip_address=sai_ipaddress('2010:1:1:0:0:0:0:2'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry5, dst_mac_address=dmac5)
        self.nhop6 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('2010:1:1:0:0:0:0:3'),
            router_interface_id=self.vlan200_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry6 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan200_rif,
            ip_address=sai_ipaddress('2010:1:1:0:0:0:0:3'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry6, dst_mac_address=dmac6)
        self.nhop7 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress('2010:1:1:0:0:0:0:4'),
            router_interface_id=self.port11_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry7 = sai_thrift_neighbor_entry_t(
            rif_id=self.port11_rif,
            ip_address=sai_ipaddress('2010:1:1:0:0:0:0:4'))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry7, dst_mac_address=dmac7)
        self.nhop_group2 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group2_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop4)
        self.nh_group2_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop5)
        self.nh_group2_member3 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop6)
        self.nh_group2_member4 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop7)
        self.route1 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('5500:1:1:0:0:0:0:1/65'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route1, next_hop_id=self.nhop_group2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # define IPv4 IPv6 LagIPv4Hash and LagIPv6Hash
        self.ipv4_hash_id, self.ipv6_hash_id = setup_hash(self)

    def runTest(self):
        self.l3IPv6EcmpSVIPortLagHostTest()
        self.l3IPv6EcmpSVIHostTest()

    def tearDown(self):

        sai_thrift_remove_route_entry(self.client, self.route1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry4)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry5)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry6)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry7)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group2_member1)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group2_member2)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group2_member3)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group2_member4)
        self.assertEqual(nhg_members_count(self.client, self.nhop_group2), 0)
        sai_thrift_remove_next_hop_group(self.client, self.nhop_group2)
        sai_thrift_remove_next_hop(self.client, self.nhop4)
        sai_thrift_remove_next_hop(self.client, self.nhop5)
        sai_thrift_remove_next_hop(self.client, self.nhop6)
        sai_thrift_remove_next_hop(self.client, self.nhop7)
        sai_thrift_set_lag_attribute(self.client, self.lag10, port_vlan_id=0)
        sai_thrift_set_lag_attribute(self.client, self.lag11, port_vlan_id=0)
        sai_thrift_set_port_attribute(self.client, self.port10, port_vlan_id=0)
        sai_thrift_set_port_attribute(self.client, self.port11, port_vlan_id=0)
        sai_thrift_remove_router_interface(self.client, self.vlan200_rif)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member200)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member201)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member202)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member203)
        sai_thrift_remove_vlan(self.client, self.vlan200)
        sai_thrift_remove_lag_member(self.client, self.lag10_member30)
        sai_thrift_remove_lag_member(self.client, self.lag10_member31)
        sai_thrift_remove_lag_member(self.client, self.lag11_member28)
        sai_thrift_remove_lag_member(self.client, self.lag11_member29)
        sai_thrift_remove_bridge_port(self.client, self.lag10_bp)
        sai_thrift_remove_bridge_port(self.client, self.lag11_bp)
        sai_thrift_remove_lag(self.client, self.lag10)
        sai_thrift_remove_lag(self.client, self.lag11)
        sai_thrift_remove_bridge_port(self.client, self.port10_bp)
        sai_thrift_remove_bridge_port(self.client, self.port11_bp)
        sai_thrift_remove_route_entry(self.client, self.route0)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry3)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member1)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member2)
        sai_thrift_remove_next_hop_group_member(self.client,
                                                self.nh_group1_member3)
        sai_thrift_remove_next_hop_group(self.client, self.nhop_group1)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_next_hop(self.client, self.nhop3)
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
        release_hash(self, self.ipv4_hash_id, self.ipv6_hash_id)

        super(L3IPv6SVIEcmpTest, self).tearDown()

    def l3IPv6EcmpSVIHostTest(self):
        """
        IPv6 ECMP tests with SVI RIF members
        """
        print("l3IPv6EcmpSVIHostTest")
        mac_action = SAI_PACKET_ACTION_FORWARD
        fdb_entry1 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address='00:11:22:33:44:55',
            bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry1,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port24_bp,
            packet_action=mac_action)
        fdb_entry2 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address='00:22:22:33:44:55',
            bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry2,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port25_bp,
            packet_action=mac_action)
        fdb_entry3 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address='00:33:22:33:44:55',
            bv_id=self.vlan100)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry3,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port26_bp,
            packet_action=mac_action)
        try:
            count = [0, 0, 0]
            dst_ip = socket.inet_pton(socket.AF_INET6, '4000:1:1:0:0:0:0:1')
            dst_ip_arr = list(dst_ip)
            src_mac_start = '00:22:22:22:22:22'
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
                src_mac = src_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
                pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src=src_mac,
                                          ipv6_dst=dst_ip_addr,
                                          ipv6_src='7000:1:1:0:0:0:0:1',
                                          ipv6_hlim=64)
                exp_pkt1 = simple_tcpv6_packet(
                    eth_dst='00:11:22:33:44:55',
                    eth_src=ROUTER_MAC,
                    ipv6_dst=dst_ip_addr,
                    ipv6_src='7000:1:1:0:0:0:0:1',
                    ipv6_hlim=63)
                exp_pkt2 = simple_tcpv6_packet(
                    eth_dst='00:22:22:33:44:55',
                    eth_src=ROUTER_MAC,
                    ipv6_dst=dst_ip_addr,
                    ipv6_src='7000:1:1:0:0:0:0:1',
                    ipv6_hlim=63)
                exp_pkt3 = simple_tcpv6_packet(
                    eth_dst='00:33:22:33:44:55',
                    eth_src=ROUTER_MAC,
                    ipv6_dst=dst_ip_addr,
                    ipv6_src='7000:1:1:0:0:0:0:1',
                    ipv6_hlim=63)
                send_packet(self, self.dev_port10, pkt)
                self.vlan100_rif_counter_out += 1
                ports_to_verify = [
                    self.dev_port24, self.dev_port25, self.dev_port26
                ]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2, exp_pkt3], ports_to_verify)
                count[rcv_idx] += 1
                dst_ip_arr[15] = dst_ip_arr[15] + 1
                dst_ip = bytearray(dst_ip_arr)
            print("Port LB counts:", count)
            for i in range(0, 3):
                self.assertTrue(
                    (count[i] >= ((MAX_ITRS / 3) * 0.6)),
                    "Not all paths are equally balanced, %s" % count)
        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry1)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry2)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry3)

    def l3IPv6EcmpSVIPortLagHostTest(self):
        """
        IPv6 ECMP tests with Port, LAG and SVI RIFs as ECMP members
        """
        print("l3IPv6EcmpSVIPortLagHostTest")
        mac_action = SAI_PACKET_ACTION_FORWARD
        fdb_entry1 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address='00:11:00:00:00:05',
            bv_id=self.vlan200)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry1,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.lag11_bp,
            packet_action=mac_action)
        fdb_entry2 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address='00:11:00:00:00:06',
            bv_id=self.vlan200)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry2,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.lag10_bp,
            packet_action=mac_action)
        try:
            count = [0] * 6
            dst_ip = socket.inet_pton(socket.AF_INET6, '5500:1:1:0:0:0:0:1')
            dst_ip_arr = list(dst_ip)
            src_mac_start = '00:22:22:22:22:22'
            for i in range(0, MAX_ITRS):
                dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
                src_mac = src_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
                pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src=src_mac,
                                          ipv6_dst=dst_ip_addr,
                                          ipv6_src='7000:1:1:0:0:0:0:1',
                                          ipv6_hlim=64)
                exp_pkt1 = simple_tcpv6_packet(
                    eth_dst='00:11:00:00:00:04',
                    eth_src=ROUTER_MAC,
                    ipv6_dst=dst_ip_addr,
                    ipv6_src='7000:1:1:0:0:0:0:1',
                    ipv6_hlim=63)
                exp_pkt2 = simple_tcpv6_packet(
                    eth_dst='00:11:00:00:00:05',
                    eth_src=ROUTER_MAC,
                    ipv6_dst=dst_ip_addr,
                    ipv6_src='7000:1:1:0:0:0:0:1',
                    ipv6_hlim=63)
                exp_pkt3 = simple_tcpv6_packet(
                    eth_dst='00:11:00:00:00:06',
                    eth_src=ROUTER_MAC,
                    ipv6_dst=dst_ip_addr,
                    ipv6_src='7000:1:1:0:0:0:0:1',
                    ipv6_hlim=63)
                exp_pkt4 = simple_tcpv6_packet(
                    eth_dst='00:11:00:00:00:07',
                    eth_src=ROUTER_MAC,
                    ipv6_dst=dst_ip_addr,
                    ipv6_src='7000:1:1:0:0:0:0:1',
                    ipv6_hlim=63)
                send_packet(self, self.dev_port10, pkt)
                ports_to_verify = [
                    self.dev_port10,
                    self.dev_port28, self.dev_port29,  # Lag11
                    self.dev_port30, self.dev_port31,  # Lag10
                    self.dev_port11]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt1, exp_pkt2, exp_pkt3,
                           exp_pkt4], ports_to_verify)
                count[rcv_idx] += 1
                dst_ip_arr[15] = dst_ip_arr[15] + 1
                dst_ip = bytearray(dst_ip_arr)
            ecmp_count = [count[0], count[1] + count[2],
                          count[3] + count[4], count[5]]
            print("Port ECMP counts:", ecmp_count)
            for i in range(0, 4):
                self.assertTrue(
                    (ecmp_count[i] >= ((MAX_ITRS / 4) * 0.5)),
                    "Not all paths are equally balanced, %s" % ecmp_count)
        finally:
            sai_thrift_remove_fdb_entry(self.client, fdb_entry1)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry2)
