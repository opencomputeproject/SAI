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
Thrift SAI interface Switch tests
"""
from ipaddress import ip_address

from sai_thrift.sai_headers import *

from ptf.mask import Mask
from lpm import LpmDict

from sai_base_test import *

THIS_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(THIS_DIR, '..'))


def generate_ip_addr(no_of_addr, ipv6=False):
    '''
    An IP address generator using generic way of addresses randomization
    and ensuring address is unique

    Args:
        no_of_addr (int): number of requested IP addresses
        ipv6 (bool): IP version indicator

    Yield:
        str: unique IP address
    '''
    if not ipv6:
        ip_range = [addr for addr in
                    ["1.0.0.1", "254.255.255.254"]]
    else:
        ip_range = [addr for addr in
                    ["2001::0", "2001:0db8::ffff:ffff:ffff"]]

    ip_interval = IpInterval(
        ip_address(ip_range[0]), ip_address(ip_range[1]))

    for _ in range(no_of_addr):
        ip_addr = ip_interval.get_random_ip()

        yield ip_addr


def generate_mac_list(no_of_addr):
    '''
    Generate list of different mac addresses

    Args:
        no_of_addr (int): number of requested MAC addresses (max 256^4)

    Return:
        list: mac_list with generated MAC addresses
    '''

    mac_list = []
    i = 0
    for first_grp in range(0, 256):
        for second_grp in range(0, 256):
            for third_grp in range(0, 256):
                for fourth_grp in range(0, 256):
                    mac_list.append('00:00:' +
                                    ('%02x' % first_grp) + ':' +
                                    ('%02x' % second_grp) + ':' +
                                    ('%02x' % third_grp) + ':' +
                                    ('%02x' % fourth_grp))
                    i += 1
                    if i == no_of_addr:
                        return mac_list
    return mac_list


@group("draft")
class SwitchAttrTest(SaiHelper):
    '''
    Switch attributes tests
    '''

    def setUp(self):
        super(SwitchAttrTest, self).setUp()

        # values required by neighbor entries tests set by route entries test
        # Note: routes entries tests should be run as first
        self.available_v4_host_routes = None
        self.available_v6_host_routes = None

    def runTest(self):
        self.availableIPv4RouteEntryTest()
        self.availableIPv6RouteEntryTest()
        self.availableIPv4NexthopEntryTest()
        self.availableIPv6NexthopEntryTest()
        self.availableIPv4NeighborEntryTest()
        self.availableIPv6NeighborEntryTest()
        self.availableNexthopGroupEntryTest()
        self.availableNexthopGroupMemberEntryTest()
        self.availableFdbEntryTest()
        self.availableAclTableTest()
        self.readOnlyAttributesTest()
        self.refreshIntervalTest()
        self.availableSnatEntryTest()
        self.availableDnatEntryTest()

    def availableIPv4RouteEntryTest(self):
        '''
        Verifies creation of maximum number of IPv4 route entries.
        '''
        print("\navailableIPv4RouteEntryTest()")

        attr = sai_thrift_get_switch_attribute(
            self.client, available_ipv4_route_entry=True)
        max_route_entry = attr["available_ipv4_route_entry"]
        print("Available IPv4 route entries: %d" % max_route_entry)

        routes = dict()
        mask = '/32'
        ip_add = generate_ip_addr(max_route_entry + 100)
        try:
            nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress('10.10.10.1'),
                router_interface_id=self.port10_rif,
                type=SAI_NEXT_HOP_TYPE_IP)
            self.assertNotEqual(nhop, SAI_NULL_OBJECT_ID)

            route_number = 0
            max_host_route = 0
            while route_number < max_route_entry:
                ip_p_m = sai_ipprefix(next(ip_add) + mask)

                # check if ip repeat, then get next ip
                if str(ip_p_m) in routes:
                    continue

                route_entry = sai_thrift_route_entry_t(
                    vr_id=self.default_vrf,
                    destination=ip_p_m)
                status = sai_thrift_create_route_entry(
                    self.client, route_entry, next_hop_id=nhop)

                if status == SAI_STATUS_SUCCESS:
                    routes.update({str(ip_p_m): route_entry})
                    route_number += 1
                elif status == SAI_STATUS_ITEM_ALREADY_EXISTS:
                    continue
                elif mask == '/32':  # when host table is full change to LPM
                    print("%s host routes have been created" % route_number)
                    max_host_route = route_number
                    self.available_v4_host_routes = max_host_route
                    mask = '/30'
                    continue
                else:
                    self.fail("Route creation failed after creating %d "
                              "entries, status %u" % (route_number, status))

                attr = sai_thrift_get_switch_attribute(
                    self.client, available_ipv4_route_entry=True)
                self.assertEqual(attr["available_ipv4_route_entry"],
                                 max_route_entry - route_number)

            print("%s LPM routes have been created"
                  % (max_route_entry - max_host_route))
            self.assertEqual(attr["available_ipv4_route_entry"], 0)

            ip_add.close()

        finally:
            for ip_p_m in routes:
                sai_thrift_remove_route_entry(self.client, routes.get(ip_p_m))
            sai_thrift_remove_next_hop(self.client, nhop)

    def availableIPv6RouteEntryTest(self):
        '''
        Verifies creation of maximum number of IPv6 route entries.
        '''
        print("\navailableIPv6RouteEntryTest()")

        attr = sai_thrift_get_switch_attribute(
            self.client, available_ipv6_route_entry=True)
        max_route_entry = attr["available_ipv6_route_entry"]
        print("Available IPv6 route entries: %d" % max_route_entry)

        routes = dict()
        mask = '/128'
        ip_add = generate_ip_addr(max_route_entry + 100, ipv6=True)
        try:
            nhop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress('2001:0db8:1::1'),
                router_interface_id=self.port10_rif,
                type=SAI_NEXT_HOP_TYPE_IP)
            self.assertNotEqual(nhop, SAI_NULL_OBJECT_ID)

            route_number = 0
            max_host_route = 0
            while route_number < max_route_entry:
                ip_p_m = sai_ipprefix(next(ip_add) + mask)

                #  check if ip repeat, then get next ip
                if str(ip_p_m) in routes:
                    continue

                route_entry = sai_thrift_route_entry_t(
                    vr_id=self.default_vrf,
                    destination=ip_p_m)
                status = sai_thrift_create_route_entry(
                    self.client, route_entry, next_hop_id=nhop)

                if status == SAI_STATUS_SUCCESS:
                    routes.update({str(ip_p_m): route_entry})
                    route_number += 1
                elif status == SAI_STATUS_ITEM_ALREADY_EXISTS:
                    continue
                elif mask == '/128':  # when host table is full change to LPM
                    print("%s host routes have been created" % route_number)
                    max_host_route = route_number
                    self.available_v6_host_routes = max_host_route
                    mask = '/120'
                    continue
                elif mask == '/120':  # when LPM table is full change to LPM64
                    print("%s host + LPM routes have been created so far"
                          % route_number)
                    mask = '/64'
                    continue
                else:
                    self.fail("Route creation failed after creating %d "
                              "entries, status %u" % (route_number, status))

                attr = sai_thrift_get_switch_attribute(
                    self.client, available_ipv6_route_entry=True)
                self.assertEqual(attr["available_ipv6_route_entry"],
                                 max_route_entry - route_number)

            print("%s LPM routes have been created"
                  % (max_route_entry - max_host_route))
            self.assertEqual(attr["available_ipv6_route_entry"], 0)

            ip_add.close()

        finally:
            for ip_p_m in routes:
                sai_thrift_remove_route_entry(self.client, routes.get(ip_p_m))
            sai_thrift_remove_next_hop(self.client, nhop)

    def availableIPv4NexthopEntryTest(self):
        '''
        Verifies creation of maximum number of IPv4 nexthop entries.
        '''
        print("\navailableIPv4NexthopEntryTest()")

        attr = sai_thrift_get_switch_attribute(
            self.client, available_ipv4_nexthop_entry=True)
        max_nhop_entry = attr["available_ipv4_nexthop_entry"]
        print("Available IPv4 nexthop entries: %d" % max_nhop_entry)

        nhop = dict()
        ip_add = generate_ip_addr(max_nhop_entry + 100)
        try:
            nhop_number = 0
            while nhop_number < max_nhop_entry:
                ip_p = sai_ipaddress(next(ip_add))

                if str(ip_p) in nhop:
                    continue

                nexthop = sai_thrift_create_next_hop(
                    self.client,
                    ip=ip_p,
                    router_interface_id=self.port10_rif,
                    type=SAI_NEXT_HOP_TYPE_IP)
                self.assertNotEqual(nexthop, SAI_NULL_OBJECT_ID)
                nhop.update({str(ip_p): nexthop})
                nhop_number += 1

                attr = sai_thrift_get_switch_attribute(
                    self.client, available_ipv4_nexthop_entry=True)
                self.assertEqual(attr["available_ipv4_nexthop_entry"],
                                 max_nhop_entry - nhop_number)

            self.assertEqual(attr["available_ipv4_nexthop_entry"], 0)

        finally:
            for ip_p in nhop:
                sai_thrift_remove_next_hop(self.client, nhop.get(ip_p))

    def availableIPv6NexthopEntryTest(self):
        '''
        Verifies creation of maximum number of IPv6 nexthop entries.
        '''
        print("\navailableIPv6NexthopEntryTest()")

        attr = sai_thrift_get_switch_attribute(
            self.client, available_ipv6_nexthop_entry=True)
        max_nhop_entry = attr["available_ipv6_nexthop_entry"]
        print("Available IPv6 nexthop entries: %d" % max_nhop_entry)

        nhop = dict()
        ip_add = generate_ip_addr(max_nhop_entry + 100, ipv6=True)
        try:
            nhop_number = 0
            while nhop_number < max_nhop_entry:
                ip_p = sai_ipaddress(next(ip_add))

                if str(ip_p) in nhop:
                    continue

                nexthop = sai_thrift_create_next_hop(
                    self.client,
                    ip=ip_p,
                    router_interface_id=self.port10_rif,
                    type=SAI_NEXT_HOP_TYPE_IP)
                self.assertNotEqual(nexthop, SAI_NULL_OBJECT_ID)
                nhop.update({str(ip_p): nexthop})
                nhop_number += 1

                attr = sai_thrift_get_switch_attribute(
                    self.client, available_ipv6_nexthop_entry=True)
                self.assertEqual(attr["available_ipv6_nexthop_entry"],
                                 max_nhop_entry - nhop_number)

            self.assertEqual(attr["available_ipv6_nexthop_entry"], 0)

        finally:
            for ip_p in nhop:
                sai_thrift_remove_next_hop(self.client, nhop.get(ip_p))

    def availableIPv4NeighborEntryTest(self):
        '''
        Verifies creation of maximum number of IPv4 neighbor entries.
        '''
        print("\navailableIPv4NeighborEntryTest()")

        if self.available_v4_host_routes is None:
            print("availableIPv4RouteEntryTest must be run first")
            return

        attr = sai_thrift_get_switch_attribute(
            self.client, available_ipv4_neighbor_entry=True)
        available_nbr_entry = attr["available_ipv4_neighbor_entry"]
        print("Available IPv4 neighbor entries: %d" % available_nbr_entry)

        if available_nbr_entry > self.available_v4_host_routes:
            print("Cannot create more neighbor entries than available host "
                  "routes which is %d" % self.available_v4_host_routes)
            max_nbr_entry = self.available_v4_host_routes
        else:
            max_nbr_entry = available_nbr_entry

        nbrs = dict()
        ip_add = generate_ip_addr(max_nbr_entry + 100)
        try:
            nbr_number = 0
            while nbr_number < max_nbr_entry:
                ip_p = sai_ipaddress(next(ip_add))

                #  check if ip repeat, then get next ip
                if str(ip_p) in nbrs:
                    continue

                nbr_entry = sai_thrift_neighbor_entry_t(
                    rif_id=self.port10_rif,
                    ip_address=ip_p)
                status = sai_thrift_create_neighbor_entry(
                    self.client,
                    nbr_entry,
                    dst_mac_address='00:00:00:00:00:01',
                    no_host_route=False)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
                nbrs.update({str(ip_p): nbr_entry})
                nbr_number += 1

                attr = sai_thrift_get_switch_attribute(
                    self.client, available_ipv4_neighbor_entry=True)
                self.assertEqual(attr["available_ipv4_neighbor_entry"],
                                 available_nbr_entry - nbr_number,
                                 "Failed after %d entries" % nbr_number)

        finally:
            for ip_p in nbrs:
                sai_thrift_remove_neighbor_entry(self.client, nbrs.get(ip_p))

    def availableIPv6NeighborEntryTest(self):
        '''
        Verifies creation of maximum number of IPv6 neighbor entries.
        '''
        print("\navailableIPv6NeighborEntryTest()")

        if self.available_v6_host_routes is None:
            print("availableIPv6RouteEntryTest must be run first")
            return

        attr = sai_thrift_get_switch_attribute(
            self.client, available_ipv6_neighbor_entry=True)
        available_nbr_entry = attr["available_ipv6_neighbor_entry"]
        print("Available IPv6 neighbor entries: %d" % available_nbr_entry)

        if available_nbr_entry > self.available_v6_host_routes:
            print("Cannot create more neighbor entries than available host "
                  "routes which is %d" % self.available_v6_host_routes)
            max_nbr_entry = self.available_v6_host_routes
        else:
            max_nbr_entry = available_nbr_entry

        nbrs = dict()
        ip_add = generate_ip_addr(max_nbr_entry + 100, ipv6=True)
        try:
            nbr_number = 0
            while nbr_number < max_nbr_entry:
                ip_p = sai_ipaddress(next(ip_add))

                #  check if ip repeat, then get next ip
                if str(ip_p) in nbrs:
                    continue

                nbr_entry = sai_thrift_neighbor_entry_t(
                    rif_id=self.port10_rif,
                    ip_address=ip_p)
                status = sai_thrift_create_neighbor_entry(
                    self.client,
                    nbr_entry,
                    dst_mac_address='00:00:00:00:00:01',
                    no_host_route=False)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
                nbrs.update({str(ip_p): nbr_entry})
                nbr_number += 1

                attr = sai_thrift_get_switch_attribute(
                    self.client, available_ipv6_neighbor_entry=True)
                self.assertEqual(attr["available_ipv6_neighbor_entry"],
                                 available_nbr_entry - nbr_number,
                                 "Failed after %d entries" % nbr_number)

        finally:
            for ip_p in nbrs:
                sai_thrift_remove_neighbor_entry(self.client, nbrs.get(ip_p))

    def availableNexthopGroupEntryTest(self):
        '''
        Verifies creation of maximum number of nexthop group entries.
        '''
        print("\navailableNexthopGroupEntryTest()")

        attr = sai_thrift_get_switch_attribute(
            self.client, available_next_hop_group_entry=True)
        max_nhg_entry = attr["available_next_hop_group_entry"]
        print("Available nexthop group entries: %d" % max_nhg_entry)

        nhg = []
        try:
            for nhg_number in range(1, max_nhg_entry + 1):
                nexthop_group = sai_thrift_create_next_hop_group(
                    self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
                self.assertNotEqual(nexthop_group, SAI_NULL_OBJECT_ID)
                nhg.append(nexthop_group)

                attr = sai_thrift_get_switch_attribute(
                    self.client, available_next_hop_group_entry=True)
                self.assertEqual(attr["available_next_hop_group_entry"],
                                 max_nhg_entry - nhg_number)

            self.assertEqual(attr["available_next_hop_group_entry"], 0)

            # try to create one more nexthop group - should not be possible:
            try:
                nexthop_group = sai_thrift_create_next_hop_group(
                    self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
                self.assertEqual(nexthop_group, SAI_NULL_OBJECT_ID)
                print("No more nexthop group may be created")
            except AssertionError:
                sai_thrift_remove_next_hop(self.client, nexthop_group)
                self.fail("Number of available nexthop groups "
                          "may be exceeded")

        finally:
            for nhg_id in nhg:
                sai_thrift_remove_next_hop_group(self.client, nhg_id)

    def availableNexthopGroupMemberEntryTest(self):
        '''
        Verifies creation of maximum number of nexthop group member entries.
        '''
        print("\navailableNexthopGroupMemberEntryTest()")

        attr = sai_thrift_get_switch_attribute(
            self.client, available_next_hop_group_entry=True)
        max_nhg_entry = attr["available_next_hop_group_entry"]
        print("Available nexthop group entries: %d" % max_nhg_entry)

        attr = sai_thrift_get_switch_attribute(
            self.client, available_next_hop_group_member_entry=True)
        max_member_entry = attr["available_next_hop_group_member_entry"]
        print("Available nexthop group member entries: %d" % max_member_entry)

        max_member_per_group = 64

        nhg = []
        nhop = dict()
        members = []
        member_number = 0
        group_number = 0
        ip_add = generate_ip_addr(max_member_entry + 100)
        try:
            while member_number < max_member_entry and \
                    group_number < max_nhg_entry:
                nexthop_group = sai_thrift_create_next_hop_group(
                    self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
                self.assertNotEqual(nexthop_group, SAI_NULL_OBJECT_ID)
                nhg.append(nexthop_group)

                group_number += 1

                nhop_per_group_number = 0
                while nhop_per_group_number < max_member_per_group:
                    ip_p = sai_ipaddress(next(ip_add))

                    if str(ip_p) in nhop:
                        continue

                    nexthop = sai_thrift_create_next_hop(
                        self.client,
                        ip=ip_p,
                        router_interface_id=self.port10_rif,
                        type=SAI_NEXT_HOP_TYPE_IP)
                    self.assertNotEqual(nexthop, SAI_NULL_OBJECT_ID)
                    nhop.update({str(ip_p): nexthop})
                    nhop_per_group_number += 1

                    nhop_member = sai_thrift_create_next_hop_group_member(
                        self.client,
                        next_hop_group_id=nexthop_group,
                        next_hop_id=nexthop)
                    self.assertNotEqual(nhop_member, SAI_NULL_OBJECT_ID)
                    members.append(nhop_member)
                    member_number += 1

                    attr = sai_thrift_get_switch_attribute(
                        self.client,
                        available_next_hop_group_member_entry=True)
                    self.assertEqual(
                        attr["available_next_hop_group_member_entry"],
                        max_member_entry - member_number)

            attr = sai_thrift_get_switch_attribute(
                self.client, available_next_hop_group_entry=True)
            self.assertEqual(attr["available_next_hop_group_entry"], 0)

        finally:
            for member_id in members:
                sai_thrift_remove_next_hop_group_member(self.client, member_id)
            for ip_p in nhop:
                sai_thrift_remove_next_hop(self.client, nhop.get(ip_p))
            for nhg_id in nhg:
                sai_thrift_remove_next_hop_group(self.client, nhg_id)

    def availableFdbEntryTest(self):
        '''
        Verifies creation of maximum number of FDB entries.
        '''
        print("\navailableFdbEntryTest()")

        attr = sai_thrift_get_switch_attribute(
            self.client, available_fdb_entry=True)
        max_fdb_entry = attr["available_fdb_entry"]
        print("Available FDB entries: %d" % max_fdb_entry)

        # Verifying only up to 90% of FDB table capacity
        available_fdb_entry = int(max_fdb_entry * 0.9)

        mac_list = generate_mac_list(max_fdb_entry)
        fdb = []
        try:
            for fdb_number in range(1, available_fdb_entry + 1):
                fdb_entry = sai_thrift_fdb_entry_t(
                    switch_id=self.switch_id,
                    mac_address=mac_list[fdb_number - 1],
                    bv_id=self.vlan10)
                status = sai_thrift_create_fdb_entry(
                    self.client,
                    fdb_entry,
                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                    bridge_port_id=self.port0_bp,
                    packet_action=SAI_PACKET_ACTION_FORWARD)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
                fdb.append(fdb_entry)

                attr = sai_thrift_get_switch_attribute(
                    self.client, available_fdb_entry=True)
                self.assertEqual(attr["available_fdb_entry"],
                                 max_fdb_entry - fdb_number)

        finally:
            for fdb_id in fdb:
                sai_thrift_remove_fdb_entry(self.client, fdb_id)

    def availableAclTableTest(self):
        '''
        Verifies creation of maximum number of acl tables.
        '''
        print("\navailableAclTableTest()")

        acl_resource = sai_thrift_acl_resource_t(
            stage=SAI_SWITCH_ATTR_ACL_STAGE_INGRESS)
        attr = sai_thrift_get_switch_attribute(
            self.client, available_acl_table=acl_resource)
        available_acl_tables = attr["available_acl_table"]

        resource_list = available_acl_tables.resourcelist
        try:
            acl_table_list_list = []
            for resource in resource_list:
                stage = resource.stage
                bind_point = resource.bind_point
                avail_num = resource.avail_num
                print("Available tables on stage %d: %d"
                      % (stage, avail_num))

                acl_table_list = []
                for _ in range(1, avail_num + 1):
                    acl_table = sai_thrift_create_acl_table(
                        self.client,
                        acl_stage=stage,
                        acl_bind_point_type_list=sai_thrift_s32_list_t(
                            count=1, int32list=[bind_point]))

                    acl_table_list.append(acl_table)

                    # check remained entries
                    attr = sai_thrift_get_switch_attribute(
                        self.client, available_acl_table=acl_resource)

                    for res in attr["available_acl_table"].resourcelist:
                        print(res)
                        if res.stage == stage and \
                           res.bind_point == bind_point:
                            self.assertEqual(res.avail_num, -
                                             avail_num - table_number)
                            break

                try to create one more table - should not be possible
                try:
                    acl_table = sai_thrift_create_acl_table(
                        self.client,
                        acl_stage=stage,
                        acl_bind_point_type_list=sai_thrift_s32_list_t(
                            count=1, int32list=[bind_point]))
                    self.assertEqual(acl_table, SAI_NULL_OBJECT_ID)
                except AssertionError:
                    sai_thrift_remove_acl_table(self.client, acl_table)
                    self.fail("Number of available ACL table entries "
                              "may be exceeded")

                print("Required number of ACL tables created")

                acl_table_list_list.append(acl_table_list)

        finally:
            for acl_table_list in acl_table_list_list:
                for acl_table in acl_table_list:
                    sai_thrift_remove_acl_table(self.client, acl_table)

    def readOnlyAttributesTest(self):
        '''
        Verifies get on read only attributes.
        '''
        print("\nreadOnlyAttributesTest()")

        attr = sai_thrift_get_switch_attribute(self.client,
                                               number_of_active_ports=True)
        print(attr)
        self.assertNotEqual(attr["number_of_active_ports"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_NUMBER_OF_ACTIVE_PORTS"], 0)
        active_ports = attr["number_of_active_ports"]

        attr = sai_thrift_get_switch_attribute(
            self.client, max_number_of_supported_ports=True)
        print(attr)
        self.assertNotEqual(attr["max_number_of_supported_ports"], 0)
        self.assertNotEqual(
            attr["SAI_SWITCH_ATTR_MAX_NUMBER_OF_SUPPORTED_PORTS"], 0)

        attr = sai_thrift_get_switch_attribute(
            self.client,
            port_list=sai_thrift_object_list_t(idlist=[], count=active_ports))
        print(attr)
        self.assertNotEqual(attr["port_list"].count, 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_PORT_LIST"].count, 0)

        attr = sai_thrift_get_switch_attribute(self.client, port_max_mtu=True)
        print(attr)
        self.assertNotEqual(attr["port_max_mtu"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_PORT_MAX_MTU"], 0)

        attr = sai_thrift_get_switch_attribute(self.client, cpu_port=True)
        print(attr)
        self.assertNotEqual(attr["cpu_port"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_CPU_PORT"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               max_virtual_routers=True)
        print(attr)
        self.assertNotEqual(attr["max_virtual_routers"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_MAX_VIRTUAL_ROUTERS"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               fdb_table_size=True)
        print(attr)
        self.assertNotEqual(attr["fdb_table_size"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_FDB_TABLE_SIZE"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               l3_neighbor_table_size=True)
        print(attr)
        self.assertNotEqual(attr["l3_neighbor_table_size"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_L3_NEIGHBOR_TABLE_SIZE"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               l3_route_table_size=True)
        print(attr)
        self.assertNotEqual(attr["l3_route_table_size"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_L3_ROUTE_TABLE_SIZE"], 0)

        attr = sai_thrift_get_switch_attribute(self.client, lag_members=True)
        print(attr)
        self.assertNotEqual(attr["lag_members"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_LAG_MEMBERS"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               number_of_lags=True)
        print(attr)
        self.assertNotEqual(attr["number_of_lags"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_NUMBER_OF_LAGS"], 0)

        attr = sai_thrift_get_switch_attribute(self.client, ecmp_members=True)
        print(attr)
        self.assertNotEqual(attr["ecmp_members"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_ECMP_MEMBERS"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               number_of_ecmp_groups=True)
        print(attr)
        self.assertNotEqual(attr["number_of_ecmp_groups"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_NUMBER_OF_ECMP_GROUPS"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               number_of_unicast_queues=True)
        print(attr)
        self.assertNotEqual(attr["number_of_unicast_queues"], 0)
        self.assertNotEqual(
            attr["SAI_SWITCH_ATTR_NUMBER_OF_UNICAST_QUEUES"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               number_of_multicast_queues=True)
        print(attr)
        self.assertNotEqual(attr["number_of_multicast_queues"], 0)
        self.assertNotEqual(
            attr["SAI_SWITCH_ATTR_NUMBER_OF_MULTICAST_QUEUES"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               number_of_queues=True)
        print(attr)
        self.assertNotEqual(attr["number_of_queues"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_NUMBER_OF_QUEUES"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               number_of_cpu_queues=True)
        print(attr)
        self.assertNotEqual(attr["number_of_cpu_queues"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_NUMBER_OF_CPU_QUEUES"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               acl_table_minimum_priority=True)
        print(attr)
        self.assertEqual(attr["acl_table_minimum_priority"], 0)
        self.assertEqual(
            attr["SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY"], 0)

        attr = sai_thrift_get_switch_attribute(
            self.client, acl_table_maximum_priority=True)
        print(attr)
        self.assertNotEqual(attr["acl_table_maximum_priority"], 0)
        self.assertNotEqual(
            attr["SAI_SWITCH_ATTR_ACL_TABLE_MAXIMUM_PRIORITY"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               acl_entry_minimum_priority=True)
        print(attr)
        self.assertEqual(attr["acl_entry_minimum_priority"], 0)
        self.assertEqual(attr["SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               acl_entry_maximum_priority=True)
        print(attr)
        self.assertNotEqual(attr["acl_entry_maximum_priority"], 0)
        self.assertNotEqual(
            attr["SAI_SWITCH_ATTR_ACL_ENTRY_MAXIMUM_PRIORITY"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               default_vlan_id=True)
        print(attr)
        self.assertNotEqual(attr["default_vlan_id"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_DEFAULT_VLAN_ID"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               default_stp_inst_id=True)
        print(attr)
        self.assertEqual(attr["default_stp_inst_id"], SAI_NULL_OBJECT_ID)
        self.assertEqual(
            attr["SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID"], SAI_NULL_OBJECT_ID)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               max_stp_instance=True)
        print(attr)
        self.assertNotEqual(attr["max_stp_instance"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_MAX_STP_INSTANCE"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               default_virtual_router_id=True)
        print(attr)
        self.assertNotEqual(attr["default_virtual_router_id"], 0)
        self.assertNotEqual(
            attr["SAI_SWITCH_ATTR_DEFAULT_VIRTUAL_ROUTER_ID"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               default_1q_bridge_id=True)
        print(attr)
        self.assertNotEqual(attr["default_1q_bridge_id"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_DEFAULT_1Q_BRIDGE_ID"], 0)

        attr = sai_thrift_get_switch_attribute(
            self.client, qos_max_number_of_traffic_classes=True)
        print(attr)
        self.assertNotEqual(attr["qos_max_number_of_traffic_classes"], 0)
        self.assertNotEqual(
            attr["SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_TRAFFIC_CLASSES"], 0)

        attr_name = "SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUP_" + \
            "HIERARCHY_LEVELS"
        attr = sai_thrift_get_switch_attribute(
            self.client,
            qos_max_number_of_scheduler_group_hierarchy_levels=True)
        print(attr)
        self.assertNotEqual(
            attr["qos_max_number_of_scheduler_group_hierarchy_levels"], 0)
        self.assertNotEqual(attr[attr_name], 0)

        scheduler_group_levels = attr[
            "qos_max_number_of_scheduler_group_hierarchy_levels"]
        value = sai_thrift_u32_list_t(count=scheduler_group_levels,
                                      uint32list=[])
        attr_name = "SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUPS_" + \
            "PER_HIERARCHY_LEVEL"
        attr = sai_thrift_get_switch_attribute(
            self.client,
            qos_max_number_of_scheduler_groups_per_hierarchy_level=value)
        print(attr)
        self.assertNotEqual(
            attr["qos_max_number_of_scheduler_groups_per_hierarchy_level"], 0)
        self.assertNotEqual(attr[attr_name], 0)

        attr_name = "SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_CHILDS_" + \
            "PER_SCHEDULER_GROUP"
        attr = sai_thrift_get_switch_attribute(
            self.client, qos_max_number_of_childs_per_scheduler_group=True)
        print(attr)
        self.assertNotEqual(
            attr["qos_max_number_of_childs_per_scheduler_group"], 0)
        self.assertNotEqual(attr[attr_name], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               total_buffer_size=True)
        print(attr)
        self.assertNotEqual(attr["total_buffer_size"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_TOTAL_BUFFER_SIZE"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               ingress_buffer_pool_num=True)
        print(attr)
        self.assertNotEqual(attr["ingress_buffer_pool_num"], 0)
        self.assertNotEqual(
            attr["SAI_SWITCH_ATTR_INGRESS_BUFFER_POOL_NUM"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               egress_buffer_pool_num=True)
        print(attr)
        self.assertNotEqual(attr["egress_buffer_pool_num"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_EGRESS_BUFFER_POOL_NUM"], 0)

        not supported
        attr = sai_thrift_get_switch_attribute(self.client, ecmp_hash=True)
        print(attr)
        self.assertNotEqual(attr["ecmp_hash"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_ECMP_HASH"], 0)

        attr = sai_thrift_get_switch_attribute(self.client, lag_hash=True)
        print(attr)
        self.assertNotEqual(attr["lag_hash"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_LAG_HASH"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               max_acl_action_count=True)
        print(attr)
        self.assertNotEqual(attr["max_acl_action_count"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_MAX_ACL_ACTION_COUNT"], 0)
        max_acl_action_count = attr["max_acl_action_count"]

        attr = sai_thrift_get_switch_attribute(self.client,
                                               max_acl_range_count=True)
        print(attr)
        self.assertNotEqual(attr["max_acl_range_count"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_MAX_ACL_RANGE_COUNT"], 0)

        s32 = sai_thrift_s32_list_t(int32list=[], count=max_acl_action_count)
        cap = sai_thrift_acl_capability_t(action_list=s32)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               acl_capability=cap)
        print(attr)
        self.assertNotEqual(attr["acl_capability"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_ACL_CAPABILITY"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               max_mirror_session=True)
        print(attr)
        self.assertNotEqual(attr["max_mirror_session"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_MAX_MIRROR_SESSION"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               default_trap_group=True)
        print(attr)
        self.assertNotEqual(attr["default_trap_group"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               acl_stage_ingress=cap)
        print(attr)
        self.assertNotEqual(attr["acl_stage_ingress"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_ACL_STAGE_INGRESS"], 0)

        attr = sai_thrift_get_switch_attribute(self.client,
                                               acl_stage_egress=cap)
        print(attr)
        self.assertNotEqual(attr["acl_stage_egress"], 0)
        self.assertNotEqual(attr["SAI_SWITCH_ATTR_ACL_STAGE_EGRESS"], 0)

    def refreshIntervalTest(self):
        '''
        Verifies SAI_SWITCH_ATTR_COUNTER_REFRESH_INTERVAL switch attribute
        applied to VLAN and RIF stats
        '''
        print("\nrefreshIntervalTest()")

        attr = sai_thrift_get_switch_attribute(
            self.client, counter_refresh_interval=True)
        init_interval = attr["counter_refresh_interval"]
        print("Counters refresh interval initially set to %d sec"
              % init_interval)

        test_vlan = self.vlan10
        test_vlan_port = self.dev_port0
        test_rif = self.port10_rif
        test_rif_port = self.dev_port10
        test_interval = 10

        vlan_stats = sai_thrift_get_vlan_stats(self.client, test_vlan)
        init_vlan_counter = vlan_stats['SAI_VLAN_STAT_IN_PACKETS']

        rif_stats = sai_thrift_get_router_interface_stats(
            self.client, test_rif)
        init_rif_counter = rif_stats['SAI_ROUTER_INTERFACE_STAT_IN_PACKETS']

        pkt = simple_udp_packet()

        try:
            print("\nTesting VLAN stats counters refresh time")
            # compensate refresh time shift
            send_packet(self, test_vlan_port, pkt)
            while sai_thrift_get_vlan_stats(self.client, test_vlan)[
                    'SAI_VLAN_STAT_IN_PACKETS'] == init_vlan_counter:
                time.sleep(0.1)
            init_vlan_counter += 1

            send_packet(self, test_vlan_port, pkt)
            vlan_stats = sai_thrift_get_vlan_stats(self.client, test_vlan)
            counter = vlan_stats['SAI_VLAN_STAT_IN_PACKETS']
            self.assertEqual(counter, init_vlan_counter)

            # determine refresh time
            timer_start = time.time()
            timer_end = time.time()
            while counter == init_vlan_counter:
                vlan_stats = sai_thrift_get_vlan_stats(self.client, test_vlan)
                counter = vlan_stats['SAI_VLAN_STAT_IN_PACKETS']
                timer_end = time.time()
            init_vlan_counter += 1

            interval = int(round(timer_end - timer_start))
            print("VLAN stats refreshed after %d sec" % interval)
            self.assertEqual(init_interval, int(round(interval)))

            vlan_stats = sai_thrift_get_vlan_stats(self.client, test_vlan)
            counter = vlan_stats['SAI_VLAN_STAT_IN_PACKETS']
            self.assertEqual(counter, init_vlan_counter)

            print("Setting refresh interval to %d sec" % test_interval)
            sai_thrift_set_switch_attribute(
                self.client, counter_refresh_interval=test_interval)

            attr = sai_thrift_get_switch_attribute(
                self.client, counter_refresh_interval=True)
            set_interval = attr["counter_refresh_interval"]
            print("Refresh interval set to %d sec" % set_interval)
            self.assertEqual(set_interval, test_interval)

            # compensate refresh time shift
            send_packet(self, test_vlan_port, pkt)
            while sai_thrift_get_vlan_stats(self.client, test_vlan)[
                    'SAI_VLAN_STAT_IN_PACKETS'] == init_vlan_counter:
                time.sleep(0.1)
            init_vlan_counter += 1

            send_packet(self, test_vlan_port, pkt)
            vlan_stats = sai_thrift_get_vlan_stats(self.client, test_vlan)
            counter = vlan_stats['SAI_VLAN_STAT_IN_PACKETS']
            self.assertEqual(counter, init_vlan_counter)

            # determine refresh time
            timer_start = time.time()
            timer_end = time.time()
            while counter == init_vlan_counter:
                vlan_stats = sai_thrift_get_vlan_stats(self.client, test_vlan)
                counter = vlan_stats['SAI_VLAN_STAT_IN_PACKETS']
                timer_end = time.time()
            init_vlan_counter += 1

            interval = int(round(timer_end - timer_start))
            print("VLAN stats refreshed after %d sec" % interval)
            self.assertEqual(test_interval, interval)

            vlan_stats = sai_thrift_get_vlan_stats(self.client, test_vlan)
            counter = vlan_stats['SAI_VLAN_STAT_IN_PACKETS']
            self.assertEqual(counter, init_vlan_counter)

        finally:
            sai_thrift_set_switch_attribute(
                self.client, counter_refresh_interval=init_interval)

        try:
            print("\nTesting RIF stats counters refresh time")
            # compensate refresh time shift
            send_packet(self, test_rif_port, pkt)
            while sai_thrift_get_router_interface_stats(self.client, test_rif)[
                    'SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'] == \
                    init_rif_counter:
                time.sleep(0.1)
            init_rif_counter += 1

            send_packet(self, test_rif_port, pkt)
            rif_stats = sai_thrift_get_router_interface_stats(
                self.client, test_rif)
            counter = rif_stats['SAI_ROUTER_INTERFACE_STAT_IN_PACKETS']
            self.assertEqual(counter, init_rif_counter)

            # determine refresh time
            timer_start = time.time()
            timer_end = time.time()
            while counter == init_rif_counter:
                rif_stats = sai_thrift_get_router_interface_stats(
                    self.client, test_rif)
                counter = rif_stats['SAI_ROUTER_INTERFACE_STAT_IN_PACKETS']
                timer_end = time.time()
            init_rif_counter += 1

            interval = int(round(timer_end - timer_start))
            print("RIF stats refreshed after %d sec" % interval)
            self.assertEqual(init_interval, int(round(interval)))

            rif_stats = sai_thrift_get_router_interface_stats(
                self.client, test_rif)
            counter = rif_stats['SAI_ROUTER_INTERFACE_STAT_IN_PACKETS']
            self.assertEqual(counter, init_rif_counter)

            print("Setting refresh interval to %d sec" % test_interval)
            sai_thrift_set_switch_attribute(
                self.client, counter_refresh_interval=test_interval)

            attr = sai_thrift_get_switch_attribute(
                self.client, counter_refresh_interval=True)
            set_interval = attr["counter_refresh_interval"]
            print("Refresh interval set to %d sec" % set_interval)
            self.assertEqual(set_interval, test_interval)

            # compensate refresh time shift
            send_packet(self, test_rif_port, pkt)
            while sai_thrift_get_router_interface_stats(self.client, test_rif)[
                    'SAI_ROUTER_INTERFACE_STAT_IN_PACKETS'] == \
                    init_rif_counter:
                time.sleep(0.1)
            init_rif_counter += 1

            send_packet(self, test_rif_port, pkt)
            rif_stats = sai_thrift_get_router_interface_stats(
                self.client, test_rif)
            counter = rif_stats['SAI_ROUTER_INTERFACE_STAT_IN_PACKETS']
            self.assertEqual(counter, init_rif_counter)

            # determine refresh time
            timer_start = time.time()
            timer_end = time.time()
            while counter == init_rif_counter:
                rif_stats = sai_thrift_get_router_interface_stats(
                    self.client, test_rif)
                counter = rif_stats['SAI_ROUTER_INTERFACE_STAT_IN_PACKETS']
                timer_end = time.time()
            init_rif_counter += 1

            interval = int(round(timer_end - timer_start))
            print("RIF stats refreshed after %d sec" % interval)
            self.assertEqual(test_interval, interval)

            rif_stats = sai_thrift_get_router_interface_stats(
                self.client, test_rif)
            counter = rif_stats['SAI_VLAN_STAT_IN_PACKETS']
            self.assertEqual(counter, init_vlan_counter)

        finally:
            sai_thrift_set_switch_attribute(
                self.client, counter_refresh_interval=init_interval)

    def availableSnatEntryTest(self):
        '''
        Verifies creation of maximum number of snat entries.
        '''
        print("\navailableSnatEntryTest()")

        attr = sai_thrift_get_switch_attribute(
            self.client, available_snat_entry=True)
        max_snat_entry = attr["available_snat_entry"]
        print("Available SNAT entries: %d" % max_snat_entry)

        snat_list = []
        addr = generate_ip_addr(max_snat_entry + 1)
        try:
            for snat_number in range(1, max_snat_entry + 1):
                nat_data = sai_thrift_nat_entry_data_t(
                    key=sai_thrift_nat_entry_key_t(
                        src_ip=next(addr), proto=6),
                    mask=sai_thrift_nat_entry_mask_t(
                        src_ip='255.255.255.255', proto=63))

                snat = sai_thrift_nat_entry_t(
                    vr_id=self.default_vrf,
                    data=nat_data,
                    nat_type=SAI_NAT_TYPE_SOURCE_NAT)

                status = sai_thrift_create_nat_entry(
                    self.client, snat, nat_type=SAI_NAT_TYPE_SOURCE_NAT)
                self.assertEqual(status, SAI_STATUS_SUCCESS)

                snat_list.append(snat)

                attr = sai_thrift_get_switch_attribute(
                    self.client, available_snat_entry=True)
                self.assertEqual(attr["available_snat_entry"],
                                 max_snat_entry - snat_number)

            # checking if no more SNAT entry may be created
            nat_data = sai_thrift_nat_entry_data_t(
                key=sai_thrift_nat_entry_key_t(
                    src_ip=next(addr),
                    proto=6),
                mask=sai_thrift_nat_entry_mask_t(
                    src_ip='255.255.255.255',
                    proto=63))
            try:
                snat = sai_thrift_nat_entry_t(
                    vr_id=self.default_vrf,
                    data=nat_data,
                    nat_type=SAI_NAT_TYPE_SOURCE_NAT)

                stat = sai_thrift_create_nat_entry(
                    self.client, snat,
                    nat_type=SAI_NAT_TYPE_SOURCE_NAT)
                self.assertNotEqual(stat, SAI_STATUS_SUCCESS)

            finally:
                if not stat:
                    sai_thrift_remove_nat_entry(self.client, snat)

        finally:
            for snat in snat_list:
                sai_thrift_remove_nat_entry(self.client, snat)

    def availableDnatEntryTest(self):
        '''
        Verifies creation of maximum number of dnat entries.
        '''
        print("\navailableDnatEntryTest()")

        attr = sai_thrift_get_switch_attribute(
            self.client, available_dnat_entry=True)
        max_dnat_entry = attr["available_dnat_entry"]
        print("Available DNAT entries: %d" % max_dnat_entry)

        dnat_list = []
        addr = generate_ip_addr(max_dnat_entry + 1)
        try:
            for dnat_number in range(1, max_dnat_entry + 1):
                nat_data = sai_thrift_nat_entry_data_t(
                    key=sai_thrift_nat_entry_key_t(
                        dst_ip=next(addr),
                        proto=6,
                        l4_src_port=100),
                    mask=sai_thrift_nat_entry_mask_t(
                        dst_ip='255.255.255.255', proto=63, l4_src_port=255))

                dnat = sai_thrift_nat_entry_t(
                    vr_id=self.default_vrf,
                    data=nat_data,
                    nat_type=SAI_NAT_TYPE_DESTINATION_NAT)

                status = sai_thrift_create_nat_entry(
                    self.client, dnat, nat_type=SAI_NAT_TYPE_DESTINATION_NAT)
                self.assertEqual(status, SAI_STATUS_SUCCESS)

                dnat_list.append(dnat)

                attr = sai_thrift_get_switch_attribute(
                    self.client, available_dnat_entry=True)
                self.assertEqual(attr["available_dnat_entry"],
                                 max_dnat_entry - dnat_number)

            # checking if no more DNAT entry may be created
            nat_data = sai_thrift_nat_entry_data_t(
                key=sai_thrift_nat_entry_key_t(
                    dst_ip=next(addr),
                    proto=6),
                mask=sai_thrift_nat_entry_mask_t(
                    dst_ip='255.255.255.255',
                    proto=63))
            try:
                dnat = sai_thrift_nat_entry_t(
                    vr_id=self.default_vrf,
                    data=nat_data,
                    nat_type=SAI_NAT_TYPE_DESTINATION_NAT)

                stat = sai_thrift_create_nat_entry(
                    self.client, dnat,
                    nat_type=SAI_NAT_TYPE_DESTINATION_NAT)
                self.assertNotEqual(stat, SAI_STATUS_SUCCESS)

            finally:
                if not stat:
                    sai_thrift_remove_nat_entry(self.client, dnat)

        finally:
            for dnat in dnat_list:
                sai_thrift_remove_nat_entry(self.client, dnat)


@group("draft")
class SwitchVxlanTest(SaiHelper):
    '''
    Switch VXLAN attributes tests
    '''

    def setUp(self):
        super(SwitchVxlanTest, self).setUp()

        # underlay config
        self.uvrf = sai_thrift_create_virtual_router(self.client)

        self.vm_ip = '100.100.1.1'
        self.customer_ip = '100.100.3.1'
        self.vni = 2000
        self.lpb_ip = '10.10.10.10'
        self.tunnel_ip = '10.10.10.1'
        self.inner_dmac = '00:33:33:33:33:33'
        self.unbor_mac = '00:11:11:11:11:11'

        # overlay config
        self.ovrf = sai_thrift_create_virtual_router(self.client)
        tunnel_type = SAI_TUNNEL_TYPE_VXLAN
        ttl_mode = SAI_TUNNEL_TTL_MODE_PIPE_MODEL

        # create underlay loopback rif for tunnel
        self.urif_lb = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.uvrf)

        self.orif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.ovrf,
            port_id=self.port24)

        # Encap configuration follows
        # create Encap/Decap mappers
        self.encap_tunnel_map = sai_thrift_create_tunnel_map(
            self.client, type=SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI)
        self.decap_tunnel_map = sai_thrift_create_tunnel_map(
            self.client, type=SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID)

        # create Encap/Decap mapper entries for self.ovrf
        self.encap_tunnel_map_entry = sai_thrift_create_tunnel_map_entry(
            self.client,
            tunnel_map_type=SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI,
            tunnel_map=self.encap_tunnel_map,
            virtual_router_id_key=self.ovrf,
            virtual_router_id_value=self.ovrf,
            vni_id_key=self.vni,
            vni_id_value=self.vni)
        self.decap_tunnel_map_entry = sai_thrift_create_tunnel_map_entry(
            self.client,
            tunnel_map_type=SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID,
            tunnel_map=self.decap_tunnel_map,
            virtual_router_id_key=self.ovrf,
            virtual_router_id_value=self.ovrf,
            vni_id_key=self.vni,
            vni_id_value=self.vni)

        encap_mappers_objlist = sai_thrift_object_list_t(
            count=1, idlist=[self.encap_tunnel_map])
        decap_mappers_objlist = sai_thrift_object_list_t(
            count=1, idlist=[self.decap_tunnel_map])

        self.tunnel = sai_thrift_create_tunnel(
            self.client,
            type=tunnel_type,
            encap_src_ip=sai_ipaddress(self.lpb_ip),
            decap_mappers=decap_mappers_objlist,
            encap_mappers=encap_mappers_objlist,
            encap_ttl_mode=ttl_mode,
            decap_ttl_mode=ttl_mode,
            underlay_interface=self.urif_lb)

        self.urif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port25,
            virtual_router_id=self.uvrf)

        self.unbor = sai_thrift_neighbor_entry_t(
            rif_id=self.urif, ip_address=sai_ipaddress(self.tunnel_ip))
        sai_thrift_create_neighbor_entry(
            self.client, self.unbor, dst_mac_address=self.unbor_mac)
        self.unhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.tunnel_ip),
            router_interface_id=self.urif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.tunnel_route = sai_thrift_route_entry_t(
            vr_id=self.uvrf, destination=sai_ipprefix('10.10.10.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.tunnel_route, next_hop_id=self.unhop)

        self.tunnel_nexthop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP,
            tunnel_id=self.tunnel,
            ip=sai_ipaddress(self.tunnel_ip),
            tunnel_vni=self.vni)

        self.customer_route = sai_thrift_route_entry_t(
            vr_id=self.ovrf, destination=sai_ipprefix(self.vm_ip + '/32'))
        sai_thrift_create_route_entry(
            self.client, self.customer_route, next_hop_id=self.tunnel_nexthop)

        print("Setting default VxLan router MAC to %s" % self.inner_dmac)
        sai_thrift_set_switch_attribute(
            self.client, vxlan_default_router_mac=self.inner_dmac)

    def runTest(self):
        self.defaultPortTest()
        self.defaultRouterMacTest()

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.customer_route)
        sai_thrift_remove_route_entry(self.client, self.tunnel_route)
        sai_thrift_remove_next_hop(self.client, self.tunnel_nexthop)
        sai_thrift_remove_next_hop(self.client, self.unhop)
        sai_thrift_remove_neighbor_entry(self.client, self.unbor)
        sai_thrift_remove_router_interface(self.client, self.urif)
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

        super(SwitchVxlanTest, self).tearDown()

    def defaultPortTest(self):
        '''
        Verifies SAI_SWITCH_ATTR_VXLAN_DEFAULT_PORT attribute setting
        '''
        print("\ndefaultPortTest()")

        vxlan_port = 4000

        try:
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst=self.vm_ip,
                                    ip_src=self.customer_ip,
                                    ip_id=108,
                                    ip_ttl=64)
            inner_pkt = simple_tcp_packet(eth_dst=self.inner_dmac,
                                          eth_src=ROUTER_MAC,
                                          ip_dst=self.vm_ip,
                                          ip_src=self.customer_ip,
                                          ip_id=108,
                                          ip_ttl=63)
            vxlan_pkt = Mask(simple_vxlan_packet(eth_src=ROUTER_MAC,
                                                 eth_dst=self.unbor_mac,
                                                 ip_id=0,
                                                 ip_src=self.lpb_ip,
                                                 ip_dst=self.tunnel_ip,
                                                 ip_ttl=64,
                                                 ip_flags=0x2,
                                                 with_udp_chksum=False,
                                                 vxlan_vni=self.vni,
                                                 inner_frame=inner_pkt))
            vxlan_pkt.set_do_not_care_scapy(UDP, 'sport')

            print("Sending packet with initial VxLan port number")
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, vxlan_pkt, self.dev_port25)
            print("\tOK")

            attr = sai_thrift_get_switch_attribute(self.client,
                                                   vxlan_default_port=True)
            init_vxlan_port = attr['vxlan_default_port']

            print("Setting default VxLan port to %d" % vxlan_port)
            sai_thrift_set_switch_attribute(self.client,
                                            vxlan_default_port=vxlan_port)

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst=self.vm_ip,
                                    ip_src=self.customer_ip,
                                    ip_id=108,
                                    ip_ttl=64)
            inner_pkt = simple_tcp_packet(eth_dst=self.inner_dmac,
                                          eth_src=ROUTER_MAC,
                                          ip_dst=self.vm_ip,
                                          ip_src=self.customer_ip,
                                          ip_id=108,
                                          ip_ttl=63)
            vxlan_pkt = Mask(simple_vxlan_packet(eth_src=ROUTER_MAC,
                                                 eth_dst=self.unbor_mac,
                                                 ip_id=0,
                                                 udp_dport=vxlan_port,
                                                 ip_src=self.lpb_ip,
                                                 ip_dst=self.tunnel_ip,
                                                 ip_ttl=64,
                                                 ip_flags=0x2,
                                                 with_udp_chksum=False,
                                                 vxlan_vni=self.vni,
                                                 inner_frame=inner_pkt))
            vxlan_pkt.set_do_not_care_scapy(UDP, 'sport')

            print("Sending packet with new VxLan port number")
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, vxlan_pkt, self.dev_port25)
            print("\tOK")

        finally:
            sai_thrift_set_switch_attribute(self.client,
                                            vxlan_default_port=init_vxlan_port)

    def defaultRouterMacTest(self):
        '''
        Verifies SAI_SWITCH_ATTR_VXLAN_DEFAULT_ROUTER_MAC attribute setting
        '''
        print("\vdefaultRouterMacTest()")

        vxlan_mac = "01:23:45:67:89:90"

        try:
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst=self.vm_ip,
                                    ip_src=self.customer_ip,
                                    ip_id=108,
                                    ip_ttl=64)
            inner_pkt = simple_tcp_packet(eth_dst=self.inner_dmac,
                                          eth_src=ROUTER_MAC,
                                          ip_dst=self.vm_ip,
                                          ip_src=self.customer_ip,
                                          ip_id=108,
                                          ip_ttl=63)
            vxlan_pkt = Mask(simple_vxlan_packet(eth_src=ROUTER_MAC,
                                                 eth_dst=self.unbor_mac,
                                                 ip_id=0,
                                                 ip_src=self.lpb_ip,
                                                 ip_dst=self.tunnel_ip,
                                                 ip_ttl=64,
                                                 ip_flags=0x2,
                                                 with_udp_chksum=False,
                                                 vxlan_vni=self.vni,
                                                 inner_frame=inner_pkt))
            vxlan_pkt.set_do_not_care_scapy(UDP, 'sport')

            print("Sending packet with initial VxLan router MAC")
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, vxlan_pkt, self.dev_port25)
            print("\tOK")

            attr = sai_thrift_get_switch_attribute(
                self.client, vxlan_default_router_mac=True)
            init_mac = attr['vxlan_default_router_mac']

            print("Setting default VxLan router MAC to %s" % vxlan_mac)
            sai_thrift_set_switch_attribute(
                self.client, vxlan_default_router_mac=vxlan_mac)

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst=self.vm_ip,
                                    ip_src=self.customer_ip,
                                    ip_id=108,
                                    ip_ttl=64)
            inner_pkt = simple_tcp_packet(eth_dst=vxlan_mac,
                                          eth_src=ROUTER_MAC,
                                          ip_dst=self.vm_ip,
                                          ip_src=self.customer_ip,
                                          ip_id=108,
                                          ip_ttl=63)
            vxlan_pkt = Mask(simple_vxlan_packet(eth_src=ROUTER_MAC,
                                                 eth_dst=self.unbor_mac,
                                                 ip_id=0,
                                                 udp_dport=4000,
                                                 ip_src=self.lpb_ip,
                                                 ip_dst=self.tunnel_ip,
                                                 ip_ttl=64,
                                                 ip_flags=0x2,
                                                 with_udp_chksum=False,
                                                 vxlan_vni=self.vni,
                                                 inner_frame=inner_pkt))
            vxlan_pkt.set_do_not_care_scapy(UDP, 'sport')
            vxlan_pkt.set_do_not_care_scapy(UDP, 'dport')

            print("Sending packet with new VxLan router MAC")
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, vxlan_pkt, self.dev_port25)
            print("\tOK")

        finally:
            sai_thrift_set_switch_attribute(
                self.client, vxlan_default_router_mac=init_mac)


@group("draft")
class SwitchDefaultVlanTest(SaiHelper):
    """
    The class runs VLAN test cases for default vlan returned by SAI
    """

    def setUp(self):
        super(SwitchDefaultVlanTest, self).setUp()

        self.pkt = 0
        self.tagged_pkt = 0
        self.arp_resp = 0
        self.tagged_arp_resp = 0
        self.i_pkt_count = 0
        self.e_pkt_count = 0
        self.mac0 = '00:11:11:11:11:11:11'
        self.mac1 = '00:22:22:22:22:22:22'
        self.mac2 = '00:33:33:33:33:33:33'
        mac_action = SAI_PACKET_ACTION_FORWARD

        # delete any pre-existing vlan members on vlan 1
        vlan_member_list = sai_thrift_object_list_t(count=100)
        mbr_list = sai_thrift_get_vlan_attribute(
            self.client, self.default_vlan_id, member_list=vlan_member_list)
        vlan_members = mbr_list['SAI_VLAN_ATTR_MEMBER_LIST'].idlist
        for mbr in vlan_members:
            sai_thrift_remove_vlan_member(self.client, mbr)

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
        # for negative learning test
        self.port27_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port27,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        self.default_vlan_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.default_vlan_id,
            bridge_port_id=self.port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_port_attribute(self.client, self.port24, port_vlan_id=1)
        self.default_vlan_member2 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.default_vlan_id,
            bridge_port_id=self.port25_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.default_vlan_member3 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.default_vlan_id,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_port_attribute(self.client, self.port26, port_vlan_id=1)

        self.fdb_entry0 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=self.mac0,
            bv_id=self.default_vlan_id)
        sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry0,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port24_bp,
            packet_action=mac_action)
        self.fdb_entry1 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=self.mac1,
            bv_id=self.default_vlan_id)
        sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry1,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port25_bp,
            packet_action=mac_action)
        self.fdb_entry2 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=self.mac2,
            bv_id=self.default_vlan_id)
        sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry2,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port26_bp,
            packet_action=mac_action)

    def runTest(self):
        try:
            self.forwardingTest()
            self.vlanLearnTest()
        finally:
            pass

    def tearDown(self):
        sai_thrift_set_port_attribute(self.client, self.port24, port_vlan_id=0)
        sai_thrift_set_port_attribute(self.client, self.port26, port_vlan_id=0)

        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry2)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry1)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry0)

        sai_thrift_remove_vlan_member(self.client, self.default_vlan_member3)
        sai_thrift_remove_vlan_member(self.client, self.default_vlan_member2)
        sai_thrift_remove_vlan_member(self.client, self.default_vlan_member1)

        sai_thrift_remove_bridge_port(self.client, self.port27_bp)
        sai_thrift_remove_bridge_port(self.client, self.port26_bp)
        sai_thrift_remove_bridge_port(self.client, self.port25_bp)
        sai_thrift_remove_bridge_port(self.client, self.port24_bp)
        super(SwitchDefaultVlanTest, self).tearDown()

    def forwardingTest(self):
        """
        Forwarding between ports with different tagging mode
        """
        print("\nforwardingTest()")
        try:
            print("\tAccessToAccessTest")
            print("Sending L2 packet port 24 -> port 26 [access vlan=1])")
            pkt = simple_tcp_packet(eth_dst='00:33:33:33:33:33',
                                    eth_src='00:11:11:11:11:11',
                                    ip_dst='172.16.0.1',
                                    ip_id=101,
                                    ip_ttl=64)

            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, pkt, self.dev_port26)

            print("\tAccessToTrunkTest")
            print("Sending L2 packet port 24 -> port 25 [trunk vlan=1])")
            pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                    eth_src='00:11:11:11:11:11',
                                    ip_dst='172.16.0.1',
                                    ip_id=102,
                                    ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                        eth_src='00:11:11:11:11:11',
                                        ip_dst='172.16.0.1',
                                        dl_vlan_enable=True,
                                        vlan_vid=1,
                                        ip_id=102,
                                        ip_ttl=64,
                                        pktlen=104)

            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, exp_pkt, self.dev_port25)
        finally:
            pass

    def vlanLearnTest(self):
        """
        Verifies learning on default vlan
        """
        print("\nvlanLearnTest()")
        try:
            pkt = simple_arp_packet(
                eth_src='00:22:22:33:44:55',
                arp_op=1,  # ARP request
                hw_snd='00:22:22:33:44:55',
                pktlen=100)
            tagged_pkt = simple_arp_packet(
                eth_src='00:22:22:33:44:55',
                arp_op=1,  # ARP request
                hw_snd='00:22:22:33:44:55',
                vlan_vid=1,
                pktlen=104)
            uc_pkt = simple_tcp_packet(
                eth_dst='00:22:22:33:44:55',
                eth_src='00:33:33:33:33:33')

            print("Sending ARP packet port 27 -> drop")
            send_packet(self, self.dev_port27, pkt)
            verify_no_other_packets(self, timeout=2)

            print("Sending ARP packet port 24 -> flood to port 25, 26")
            send_packet(self, self.dev_port24, pkt)
            verify_each_packet_on_each_port(
                self, [tagged_pkt, pkt], [self.dev_port25, self.dev_port26])

            time.sleep(4)

            print("Sending uc packet port 26 -> to learnt port 24")
            send_packet(self, self.dev_port26, uc_pkt)
            verify_packets(self, uc_pkt, [self.dev_port24])
        finally:
            time.sleep(2)
            sai_thrift_flush_fdb_entries(
                self.client,
                bv_id=self.default_vlan_id,
                entry_type=SAI_FDB_ENTRY_TYPE_DYNAMIC)
