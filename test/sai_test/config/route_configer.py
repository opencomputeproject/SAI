# Copyright (c) 2021 Microsoft Open Technologies, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing
#    permissions and limitations under the License.
#
#    Microsoft would like to thank the following companies for their review and
#    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
#    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
#
#


from sai_thrift.sai_adapter import *
from sai_utils import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]
from constant import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from sai_test_base import T0TestBase


def t0_route_config_helper(test_obj:'T0TestBase', is_create_default_route=True, is_create_route_for_lag=True, is_ipv4=True):
    route_configer = RouteConfiger(test_obj)
    if is_create_default_route:
        route_configer.create_default_route()
        test_obj.dut.port0_rif = route_configer.create_router_interface_for_port(
            port_id=test_obj.dut.port_list[0])

    if is_create_route_for_lag:
        # config neighbor and route for lag1
        test_obj.dut.lag1_rif = route_configer.create_router_interface_for_port(
            port_id=test_obj.dut.lag1.lag_id)
        if is_ipv4:
            test_obj.dut.lag1_nbr = route_configer.create_neighbor_for_rif(
                rif_id=test_obj.dut.lag1_rif, ip_addr=test_obj.t1_list[1][0].ipv4, mac_addr=test_obj.t1_list[1][0].mac)
            test_obj.dut.lag1_nhop = route_configer.create_next_hop_for_rif(
                ip_addr=test_obj.t1_list[1][0].ipv4, rif=test_obj.dut.lag1_rif)
            test_obj.dut.lag1_route = route_configer.create_route_entry(
                dst_ip=test_obj.servers[11][0].ipv4+'/24', next_hop=test_obj.dut.lag1_nhop)
        else:
            test_obj.dut.lag1_nbr = route_configer.create_neighbor_for_rif(
                rif_id=test_obj.dut.lag1_rif, ip_addr=test_obj.t1_list[1][0].ipv6, mac_addr=test_obj.t1_list[1][0].mac)
            test_obj.dut.lag1_nhop = route_configer.create_next_hop_for_rif(
                ip_addr=test_obj.t1_list[1][0].ipv6, rif=test_obj.dut.lag1_rif)
            test_obj.dut.lag1_route = route_configer.create_route_entry(
                dst_ip=test_obj.servers[11][0].ipv6+'/112', next_hop=test_obj.dut.lag1_nhop)

        # config neighbor and route for lag2
        test_obj.dut.lag2_rif = route_configer.create_router_interface_for_port(
            port_id=test_obj.dut.lag2.lag_id)
        if is_ipv4:
            test_obj.dut.lag2_nbr = route_configer.create_neighbor_for_rif(
                rif_id=test_obj.dut.lag2_rif, ip_addr=test_obj.t1_list[2][0].ipv4, mac_addr=test_obj.t1_list[2][0].mac)
            test_obj.dut.lag2_nhop = route_configer.create_next_hop_for_rif(
                ip_addr=test_obj.t1_list[2][0].ipv4, rif=test_obj.dut.lag2_rif)
            test_obj.dut.lag2_route = route_configer.create_route_entry(
                dst_ip=test_obj.servers[12][0].ipv4+'/24', next_hop=test_obj.dut.lag2_nhop)
        else:
            test_obj.dut.lag2_nbr = route_configer.create_neighbor_for_rif(
                rif_id=test_obj.dut.lag2_rif, ip_addr=test_obj.t1_list[2][0].ipv6, mac_addr=test_obj.t1_list[2][0].mac)
            test_obj.dut.lag2_nhop = route_configer.create_next_hop_for_rif(
                ip_addr=test_obj.t1_list[2][0].ipv6, rif=test_obj.dut.lag2_rif)
            test_obj.dut.lag2_route = route_configer.create_route_entry(
                dst_ip=test_obj.servers[12][0].ipv6+'/112', next_hop=test_obj.dut.lag2_nhop)


class RouteConfiger(object):
    """
    Class use to make all the route configurations.
    """

    def __init__(self, test_obj:'T0TestBase') -> None:
        """
        Init Route configer.

        Args:
            test_obj: the test object
        """
        self.test_obj = test_obj
        self.client = test_obj.client

    def create_default_route(self):
        self.create_default_route_intf()
        self.create_default_v4_v6_route_entry()
        self.create_local_v6_route()

    def create_default_route_intf(self):
        """
        Create default route interface on loop back interface.
        """
        print("Create loop back interface...")
        attr = sai_thrift_get_switch_attribute(
            self.client, default_virtual_router_id=True)
        self.test_obj.assertNotEqual(attr['default_virtual_router_id'], 0)
        self.test_obj.dut.default_vrf = attr['default_virtual_router_id']

        self.test_obj.dut.loopback_intf = sai_thrift_create_router_interface(self.client,
                                                                             type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK, virtual_router_id=self.test_obj.dut.default_vrf)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

    def create_default_v4_v6_route_entry(self):
        """
        Create default v4 and v6 route entry.
        """

        print("Create default v4&v6 route entry...")
        v6_default = sai_thrift_ip_prefix_t(addr_family=1,
                                            addr=sai_thrift_ip_addr_t(
                                                ip6=DEFAULT_IP_V6_PREFIX),
                                            mask=sai_thrift_ip_addr_t(ip6=DEFAULT_IP_V6_PREFIX))
        entry = sai_thrift_route_entry_t(vr_id=self.test_obj.dut.default_vrf,
                                         destination=v6_default)
        self.test_obj.dut.default_ipv6_route_entry = sai_thrift_create_route_entry(
            self.client,
            route_entry=entry,
            packet_action=SAI_PACKET_ACTION_DROP)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

        entry = sai_thrift_route_entry_t(vr_id=self.test_obj.dut.default_vrf,
                                         destination=sai_ipprefix(DEFAULT_IP_V4_PREFIX))
        self.test_obj.dut.default_ipv4_route_entry = sai_thrift_create_route_entry(
            self.client,
            route_entry=entry,
            packet_action=SAI_PACKET_ACTION_DROP)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

    def create_local_v6_route(self):
        """
        Create local v6 route base on the configuration of the actual switch.
        """

        print("Create local v6 route...")
        entry = sai_thrift_route_entry_t(vr_id=self.test_obj.dut.default_vrf,
                                         destination=sai_ipprefix(LOCAL_IP_10V6_PREFIX))
        self.test_obj.dut.local_10v6_route_entry = sai_thrift_create_route_entry(
            self.client,
            route_entry=entry,
            packet_action=SAI_PACKET_ACTION_FORWARD)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

        entry = sai_thrift_route_entry_t(vr_id=self.test_obj.dut.default_vrf,
                                         destination=sai_ipprefix(LOCAL_IP_128V6_PREFIX))
        self.test_obj.dut.local_128v6_route_entry = sai_thrift_create_route_entry(
            self.client,
            route_entry=entry,
            packet_action=SAI_PACKET_ACTION_FORWARD)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

    def create_router_interface_for_port(self, port_id, virtual_router_id=None):
        if virtual_router_id is None:
            virtual_router_id = self.test_obj.dut.default_vrf

        rif_id1 = sai_thrift_create_router_interface(
            self.client, virtual_router_id=virtual_router_id, type=SAI_ROUTER_INTERFACE_TYPE_PORT, port_id=port_id)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

        return rif_id1

    def create_next_hop_for_rif(self, ip_addr, rif):
        nhop = sai_thrift_create_next_hop(self.client, ip=sai_ipaddress(
            ip_addr), router_interface_id=rif, type=SAI_NEXT_HOP_TYPE_IP)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

        return nhop

    def create_neighbor_for_rif(self, rif_id, ip_addr, mac_addr):
        nbr_entry_v4 = sai_thrift_neighbor_entry_t(
            rif_id=rif_id, ip_address=sai_ipaddress(ip_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client, nbr_entry_v4, dst_mac_address=mac_addr)
        self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)

        return nbr_entry_v4

    def create_route_entry(self, dst_ip, next_hop, virtual_router_id=None):
        if virtual_router_id is None:
            virtual_router_id = self.test_obj.dut.default_vrf

        route1 = sai_thrift_route_entry_t(
            vr_id=virtual_router_id, destination=sai_ipprefix(dst_ip))
        status = sai_thrift_create_route_entry(
            self.client, route_entry=route1, next_hop_id=next_hop)
        self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)

        return route1
