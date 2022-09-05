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
from typing import TYPE_CHECKING, Tuple

from data_module.device import Device, DeviceType
from data_module.vlan import Vlan
from data_module.lag import Lag
from data_module.port import Port
from data_module.routable_item import route_item
from typing import Dict, List

from data_module.nexthop import Nexthop
from data_module.nexthop_group import NexthopGroup

if TYPE_CHECKING:
    from sai_test_base import T0TestBase


def t0_route_config_helper(
        test_obj: 'T0TestBase',
        is_create_default_route=True,
        is_create_default_loopback_interface=False,
        is_create_route_for_lag=True,
        is_create_vlan_interface=True,
        is_create_route_for_vlan=True,
        is_create_route_for_nhopgrp=False,
        ):
    """
    Make t0 route configurations base on the configuration in the test plan.
    Set the configuration in test directly.

    Attrs:
        test_obj: Sub class from T0TestBase
        is_create_default_route: defaule is true
        is_create_default_loopback_interface: defaule is true
        is_create_route_for_lag: defaule is true
        is_create_vlan_interface: defaule is true
        is_create_route_for_vlan: defaule is True
        is_create_route_for_nhopgrp: defaule is false

    Set the following test_obj attributes:
        int: default_vrf
        default_ipv6_route_entry
        default_ipv4_route_entry
        neighbor and route for lag
    """
    route_configer = RouteConfiger(test_obj)
    if is_create_default_route:
        print("Create default route")
        route_configer.create_default_route()
        route_configer.create_router_interface_by_port_idx(port_idx=0)

    if is_create_default_loopback_interface:
        print("Create loopback interface")
        route_configer.create_default_loopback_interface()

    if is_create_vlan_interface:
        for vlan_name in test_obj.dut.vlans:
            print("Create vlan interface for vlan {}".format(vlan_name))
            route_configer.create_router_interface(
                net_interface=test_obj.dut.vlans[vlan_name])

    if is_create_route_for_lag:
        print(
            "Create route for server with in ip {}/{}".format(test_obj.servers[11][0].ipv4, 24))
        test_obj.servers[11][0].ip_prefix = '24'
        test_obj.servers[11][0].ip_prefix_v6 = '112'
        rif = route_configer.create_router_interface(
            net_interface=test_obj.dut.lag_list[0])
        route_configer.create_neighbor_by_rif(rif=rif,
                                              nexthop_device=test_obj.t1_list[1][100],
                                              no_host=False)
        nhv4, nhv6 = route_configer.create_nexthop_by_rif(rif=rif,
                                                          nexthop_device=test_obj.t1_list[1][100])
        test_obj.dut.lag_list[0].nexthopv4_list.append(nhv4)
        test_obj.dut.lag_list[0].nexthopv6_list.append(nhv6)
        route_configer.create_route_by_nexthop(
            dest_device=test_obj.servers[11][0],
            nexthopv4=nhv4,
            nexthopv6=nhv6)
        # set expected dest server
        for item in test_obj.servers[11]:
            item.l3_lag_obj = test_obj.dut.lag_list[0]
            item.l3_lag_obj.neighbor_mac = test_obj.t1_list[1][100].mac
        # set expected dest T1
        test_obj.t1_list[1][100].l3_lag_obj = test_obj.dut.lag_list[0]

        print(
            "Create route for server with in ip {}/{}".format(test_obj.servers[21][0].ipv4, 24))
        test_obj.servers[21][0].ip_prefix = '24'
        test_obj.servers[21][0].ip_prefix_v6 = '112'
        route_configer.create_route_by_nexthop(
            dest_device=test_obj.servers[21][0],
            nexthopv4=nhv4,
            nexthopv6=nhv6)
        # set expected dest server
        for item in test_obj.servers[21]:
            item.l3_lag_obj = test_obj.dut.lag_list[0]
            item.l3_lag_obj.neighbor_mac = test_obj.t1_list[1][100].mac

        print(
            "Create route for server with in ip {}/{}".format(test_obj.servers[12][0].ipv4, 24))
        test_obj.servers[12][0].ip_prefix = '24'
        test_obj.servers[12][0].ip_prefix_v6 = '112'
        rif = route_configer.create_router_interface(
            net_interface=test_obj.dut.lag_list[1])
        route_configer.create_neighbor_by_rif(rif=rif,
                                              nexthop_device=test_obj.t1_list[2][100],
                                              no_host=False)
        nhv4, nhv6 = route_configer.create_nexthop_by_rif(rif=rif,
                                                          nexthop_device=test_obj.t1_list[2][100])
        test_obj.dut.lag_list[1].nexthopv4_list.append(nhv4)
        test_obj.dut.lag_list[1].nexthopv6_list.append(nhv6)
        route_configer.create_route_by_nexthop(
            dest_device=test_obj.servers[12][0],
            nexthopv4=nhv4,
            nexthopv6=nhv6)
        # set expected dest server
        for item in test_obj.servers[12]:
            item.l3_lag_obj = test_obj.dut.lag_list[1]
            item.l3_lag_obj.neighbor_mac = test_obj.t1_list[2][100].mac
        # set expected dest T1
        test_obj.t1_list[2][100].l3_lag_obj = test_obj.dut.lag_list[1]

    if is_create_route_for_vlan:
        print("Config route for vlan...")
        test_obj.dut.vlans[10].broadcast_neighbor_device = Device(
            device_type=DeviceType.server, id=255, group_id=1)
        test_obj.dut.vlans[10].broadcast_neighbor_device.mac = BROADCAST_MAC
        test_obj.dut.vlans[10].broadcast_neighbor_device.ipv6 = None
        route_configer.create_neighbor_by_rif(
            nexthop_device=test_obj.dut.vlans[10].broadcast_neighbor_device,
            rif=test_obj.dut.vlans[10].rif_list[0],
            no_host=False)
        for index in range(1, 9):
            route_configer.create_neighbor_by_rif(
                nexthop_device=test_obj.servers[1][index],
                rif=test_obj.dut.vlans[10].rif_list[0])
            route_configer.create_neighbor_by_rif(
                nexthop_device=test_obj.servers[1][90+index],
                rif=test_obj.dut.vlans[10].rif_list[0])

        test_obj.servers[1][0].ip_prefix = '24'
        test_obj.servers[1][0].ip_prefix_v6 = '112'
        test_obj.servers[1][1].ip_prefix = '24'
        test_obj.servers[1][1].ip_prefix_v6 = '112'
        nhopv4, nhopv6 = route_configer.create_nexthop_by_rif(
            rif=test_obj.dut.vlans[10].rif_list[0],
            nexthop_device=test_obj.servers[1][1])
        test_obj.dut.vlans[10].nexthopv4_list.append(nhopv4)
        test_obj.dut.vlans[10].nexthopv6_list.append(nhopv6)
        route_configer.create_route_by_nexthop(
            dest_device=test_obj.servers[1][0],
            nexthopv4=nhopv4,
            nexthopv6=nhopv6)
        print(
            "Create route for server with in ip {}/{}".format(test_obj.servers[1][0].ipv4, 24))

        test_obj.dut.vlans[20].broadcast_neighbor_device = Device(
            device_type=DeviceType.server, id=255, group_id=2)
        test_obj.dut.vlans[20].broadcast_neighbor_device.mac = BROADCAST_MAC
        test_obj.dut.vlans[20].broadcast_neighbor_device.ipv6 = None
        route_configer.create_neighbor_by_rif(
            nexthop_device=test_obj.dut.vlans[20].broadcast_neighbor_device,
            rif=test_obj.dut.vlans[20].rif_list[0],
            no_host=False)
        for index in range(0, 8):
            route_configer.create_neighbor_by_rif(
                nexthop_device=test_obj.servers[2][9+index],
                rif=test_obj.dut.vlans[20].rif_list[0])
            route_configer.create_neighbor_by_rif(
                nexthop_device=test_obj.servers[2][91+index],
                rif=test_obj.dut.vlans[20].rif_list[0])

        test_obj.servers[2][0].ip_prefix = '24'
        test_obj.servers[2][0].ip_prefix_v6 = '112'
        route_configer.create_route_by_rif(
            dest_device=test_obj.servers[2][0],
            rif=test_obj.dut.vlans[20].rif_list[0])
        test_obj.dut.vlans[20].nexthopv4_list.append(Nexthop(
            nexthop_device=test_obj.dut.vlans[20], rif_id=test_obj.dut.vlans[20].rif_list[0]))
        test_obj.dut.vlans[20].nexthopv6_list.append(Nexthop(
            nexthop_device=test_obj.dut.vlans[20], rif_id=test_obj.dut.vlans[20].rif_list[0]))
        print(
            "Create route for server with in ip {}/{}".format(test_obj.servers[2][0].ipv4, 24))

    if is_create_route_for_nhopgrp:
        nhpv4_list, nhpv6_list = [], []
        lag_num = 4
        for lag_idx in range(lag_num):
            t1_idx = lag_idx + 1
            print("Create nexthop for port with in ip {}/{}".format(test_obj.t1_list[t1_idx][100].ipv4, 24))
            rif = route_configer.create_router_interface(
                net_interface=test_obj.dut.lag_list[lag_idx])
            route_configer.create_neighbor_by_rif(rif=rif,
                                                nexthop_device=test_obj.t1_list[t1_idx][100],
                                                no_host=False)
            nhv4, nhv6 = route_configer.create_nexthop_by_rif(rif=rif,
                                                            nexthop_device=test_obj.t1_list[t1_idx][100])
            nhpv4_list.append(nhv4)
            nhpv6_list.append(nhv6)

        print("Create nexthop group for server with in ip {}/{}".format(test_obj.servers[60][0].ipv4, 24))
        test_obj.servers[60][0].ip_prefix = '24'
        test_obj.servers[60][0].ip_prefix_v6 = '112'
        nhp_grpv4, nhp_grpv6 = route_configer.create_nexthop_group_by_nexthops(
            nexthopv4_list=nhpv4_list,
            nexthopv6_list=nhpv6_list,
            dest_device=test_obj.servers[60][0])

        # set expected dest lag
        for lag in test_obj.dut.lag_list:
            lag.nexthop_groupv4 = nhp_grpv4
            lag.nexthop_groupv6 = nhp_grpv6

class RouteConfiger(object):
    """
    Class use to make all the route configurations.
    """

    def __init__(self, test_obj: 'T0TestBase') -> None:
        """
        Init Route configer.

        Args:
            test_obj: the test object
        """
        self.test_obj = test_obj
        self.client = test_obj.client

    def create_default_route(self):
        self.get_default_virtual_router()
        self.create_default_v4_v6_route_entry()

    def get_default_virtual_router(self):
        """
        Get default virtual_router_id
        """
        print("Get default virtual router id...")
        attr = sai_thrift_get_switch_attribute(
            self.client, default_virtual_router_id=True)
        self.test_obj.assertNotEqual(attr['default_virtual_router_id'], 0)
        self.test_obj.dut.default_vrf = attr['default_virtual_router_id']

    def create_default_loopback_interface(self):
        """
        Create loopback interface on default virtual router.
        """
        self.test_obj.dut.loopback_intf = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.test_obj.dut.default_vrf)
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
        self.test_obj.dut.default_ipv6_route_entry = sai_thrift_route_entry_t(vr_id=self.test_obj.dut.default_vrf,
                                                                              destination=v6_default)
        status = sai_thrift_create_route_entry(
            self.client,
            route_entry=self.test_obj.dut.default_ipv6_route_entry,
            packet_action=SAI_PACKET_ACTION_DROP)
        self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)

        self.test_obj.dut.default_ipv4_route_entry = sai_thrift_route_entry_t(vr_id=self.test_obj.dut.default_vrf,
                                                                              destination=sai_ipprefix(DEFAULT_IP_V4_PREFIX))
        status = sai_thrift_create_route_entry(
            self.client,
            route_entry=self.test_obj.dut.default_ipv4_route_entry,
            packet_action=SAI_PACKET_ACTION_DROP)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

    def create_route_by_rif(
            self, dest_device: Device, rif, virtual_router=None):
        """
        Create a complete route path to a dest_device device, via route interface.

        Set Device attribute: routev4, routev6

        Attrs:
            dest_device: Simulating the destinate device that this dut direct connect to.
            rif: route_interface
            virtual_router_id: virtual route id, if not defined, will use default route

        Return: routev4, routev6
        """
        vr_id = self.choice_virtual_route(virtual_router)
        if dest_device.ip_prefix:
            net_routev4 = sai_thrift_route_entry_t(
                vr_id=vr_id, destination=sai_ipprefix(dest_device.ipv4+'/'+dest_device.ip_prefix))
        else:
            # destination cannot use sai_ipaddress
            net_routev4 = sai_thrift_route_entry_t(
                vr_id=vr_id, destination=sai_ipprefix(dest_device.ipv4+'/32'))
        status = sai_thrift_create_route_entry(
            self.client, net_routev4, next_hop_id=rif)
        self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)

        if dest_device.ip_prefix_v6:
            net_routev6 = sai_thrift_route_entry_t(
                vr_id=vr_id, destination=sai_ipprefix(dest_device.ipv6+'/'+dest_device.ip_prefix_v6))
        else:
            # destination cannot use sai_ipaddress
            net_routev6 = sai_thrift_route_entry_t(
                vr_id=vr_id, destination=sai_ipprefix(dest_device.ipv6+'/128'))
        status = sai_thrift_create_route_entry(
            self.client, net_routev6, next_hop_id=rif)
        self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)

        self.test_obj.dut.routev4_list.append(net_routev4)
        self.test_obj.dut.routev6_list.append(net_routev6)

        return net_routev4, net_routev6

    def create_route_by_nexthop(
            self, dest_device: Device, nexthopv4: Nexthop, nexthopv6: Nexthop, virtual_router=None):
        """
        Create a complete route path to a dest_device device, via from nexthop.

        Set Device attribute: routev4, routev6

        Attrs:
            dest_device: Simulating the destinate device that this dut direct connect to.
            nexthopv4: nexthopv4
            nexthopv6: nexthopv6
            virtual_router_id: virtual route id, if not defined, will use default route

        Return: routev4, routev6
        """
        vr_id = self.choice_virtual_route(virtual_router)
        if dest_device.ip_prefix:
            net_routev4 = sai_thrift_route_entry_t(
                vr_id=vr_id, destination=sai_ipprefix(dest_device.ipv4+'/'+dest_device.ip_prefix))
        else:
            # destination cannot use sai_ipaddress
            net_routev4 = sai_thrift_route_entry_t(
                vr_id=vr_id, destination=sai_ipprefix(dest_device.ipv4+'/32'))
        status = sai_thrift_create_route_entry(
            self.client, net_routev4, next_hop_id=nexthopv4.oid)
        self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)

        if dest_device.ip_prefix_v6:
            net_routev6 = sai_thrift_route_entry_t(
                vr_id=vr_id, destination=sai_ipprefix(dest_device.ipv6+'/'+dest_device.ip_prefix_v6))
        else:
            # destination cannot use sai_ipaddress
            net_routev6 = sai_thrift_route_entry_t(
                vr_id=vr_id, destination=sai_ipprefix(dest_device.ipv6+'/128'))
        status = sai_thrift_create_route_entry(
            self.client, net_routev6, next_hop_id=nexthopv6.oid)
        self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)

        self.test_obj.dut.routev4_list.append(net_routev4)
        self.test_obj.dut.routev6_list.append(net_routev6)

        return net_routev4, net_routev6

    def create_route_by_nexthop_group(
            self, dest_device: Device, nexthop_groupv4: NexthopGroup, nexthop_groupv6: NexthopGroup, virtual_router=None):
        """
        Create a complete route path to a dest_device device, via nexthop group.
        Set Device attribute: routev4, routev6
        Attrs:
            dest_device: Simulating the destinate device that this dut direct connect to.
            nexthop_groupv4: nexthop group ipv4
            nexthop_groupv6: nexthop group ipv6
            virtual_router_id: virtual route id, if not defined, will use default route
        Return: routev4, routev6
        """
        vr_id = self.choice_virtual_route(virtual_router)
        if dest_device.ip_prefix:
            net_routev4 = sai_thrift_route_entry_t(
                vr_id=vr_id, destination=sai_ipprefix(dest_device.ipv4+'/'+dest_device.ip_prefix))
        else:
            # destination cannot use sai_ipaddress
            net_routev4 = sai_thrift_route_entry_t(
                vr_id=vr_id, destination=sai_ipprefix(dest_device.ipv4+'/32'))
        status = sai_thrift_create_route_entry(
            self.client, net_routev4, next_hop_id=nexthop_groupv4.nhp_grp_id)
        self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)

        if dest_device.ip_prefix_v6:
            net_routev6 = sai_thrift_route_entry_t(
                vr_id=vr_id, destination=sai_ipprefix(dest_device.ipv6+'/'+dest_device.ip_prefix_v6))
        else:
            # destination cannot use sai_ipaddress
            net_routev6 = sai_thrift_route_entry_t(
                vr_id=vr_id, destination=sai_ipprefix(dest_device.ipv6+'/128'))
        status = sai_thrift_create_route_entry(
            self.client, net_routev6, next_hop_id=nexthop_groupv6.nhp_grp_id)
        self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)

        self.test_obj.dut.routev4_list.append(net_routev4)
        self.test_obj.dut.routev6_list.append(net_routev6)

        return net_routev4, net_routev6

    def create_neighbor_by_rif(self, nexthop_device: Device, rif, no_host=True):
        """
        Create neighbor.

        Set Device attribtue: neighborv4_id, neighborv6_id
        Set Dut attribute: neighborv4_list, neighborv6_list

        Attrs:
            nexthop_device: Simulating the bypass device that the packet will be forwarded to.
            rif: the router interface this neighbor mapping.
            no_host: Neighbor in no_host (neighbor direct) mode

        return neighborv4, neighborv6
        """
        if nexthop_device.ipv4:
            nbr_entry_v4 = sai_thrift_neighbor_entry_t(
                rif_id=rif,
                ip_address=sai_ipaddress(nexthop_device.ipv4))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                nbr_entry_v4,
                dst_mac_address=nexthop_device.mac,
                no_host_route=no_host)
            self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)
        else:
            nbr_entry_v4 = None

        if nexthop_device.ipv6:
            nbr_entry_v6 = sai_thrift_neighbor_entry_t(
                rif_id=rif,
                ip_address=sai_ipaddress(nexthop_device.ipv6))
            status = sai_thrift_create_neighbor_entry(
                self.client,
                nbr_entry_v6,
                dst_mac_address=nexthop_device.mac,
                no_host_route=no_host)
            self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)
        else:
            nbr_entry_v6 = None

        self.test_obj.dut.neighborv4_list.append(nbr_entry_v4)
        if nbr_entry_v6:
            self.test_obj.dut.neighborv6_list.append(nbr_entry_v6)
        return nbr_entry_v4, nbr_entry_v6

    def create_router_interface_by_port_idx(self, port_idx, virtual_router=None, reuse=True, is_bridge=False):
        """
        Create route interface by port index for a port.
        It will check if the port already created on a route interface. If 'reuse',
        it will return the last one created for this port, if not 'reuse', it will create a new one,
        and store it with this port and dut object.

        Attrs:
            port_idx: port index
            virtual_router_id: virtual route id, if not defined, will use default route
            reuse: reuse the existing rif which is binding to the port
            is_bridge: is a bridge port, only used for port
        return: route interface
        """
        net_intf = self.test_obj.dut.port_obj_list[port_idx]
        return self.create_router_interface(net_interface=net_intf, virtual_router=virtual_router, reuse=reuse, is_bridge=is_bridge)

    def create_router_interface(self, net_interface: route_item, virtual_router=None, reuse=True, is_bridge=False):
        """
        Create intreface.
        It will check if the net interface already created on a route interface. If 'reuse',
        it will return the last one created for this net interface, if not 'reuse', it will create a new one,
        and store it with this net interface and dut object.

        If net_interface is None, then will create a loopback rif.

        Set net_interface attribute oid.

        Attrs:
            net_interface: route_item object that this interface mapping
            virtual_router_id: virtual route id, if not defined, will use default route
            reuse: reuse the existing rif which is binding to the net interfaces
            is_bridge: is a bridge port, only used for port
        return rif
        """
        if not reuse or not (net_interface.rif_list and len(net_interface.rif_list) != 0):
            vr_id = self.choice_virtual_route(virtual_router)
            if not net_interface:
                rif = sai_thrift_create_router_interface(self.client,
                                                         virtual_router_id=vr_id,
                                                         type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK)
            else:
                # Checks
                if is_bridge:
                    if not isinstance(net_interface, Port):
                        raise ValueError(
                            'is_bridge attribute can only be True when net interface is a Port!')
                if isinstance(net_interface, Vlan):
                    rif = sai_thrift_create_router_interface(self.client,
                                                             virtual_router_id=vr_id,
                                                             type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                                             vlan_id=net_interface.oid)
                else:  # include port, vlan and lag
                    if is_bridge:
                        rif = sai_thrift_create_router_interface(self.client,
                                                                 virtual_router_id=vr_id,
                                                                 type=SAI_ROUTER_INTERFACE_TYPE_BRIDGE,
                                                                 bridge_id=net_interface.oid)
                    else:
                        rif = sai_thrift_create_router_interface(self.client,
                                                                 virtual_router_id=vr_id,
                                                                 type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                                                                 port_id=net_interface.oid)

            self.test_obj.assertEqual(
                self.test_obj.status(), SAI_STATUS_SUCCESS)
            if not net_interface.rif_list:
                net_interface.rif_list = []
            net_interface.rif_list.append(rif)
            if not self.test_obj.dut.rif_list:
                self.test_obj.dut.rif_list = []
            self.test_obj.dut.rif_list.append(rif)
        return net_interface.rif_list[-1]

    def create_nexthop_by_rif(self, rif, nexthop_device: Device):
        """
        Create nexthop by bridge port index for a port.

        Set dut attribute: nexthopv4_list, nexthopv6_list
        Set device attribute: nexthopv4, nexthopv6

        Attrs:
            rif: route interface id
            nexthop_device: Simulating the bypass device, use this device to get the ipaddress, ipprefix_v4 and ipprefix_v6
            virtual_router_id: virtual route id, if not defined, will use default route

        return nexthop object for v4 and nexthop object for v6 
        """
        if nexthop_device.ip_prefix:
            nhopv4_id = sai_thrift_create_next_hop(self.client, ip=sai_ipprefix(
                nexthop_device.ipv4 + '/' + nexthop_device.ip_prefix), router_interface_id=rif, type=SAI_NEXT_HOP_TYPE_IP)
        else:
            nhopv4_id = sai_thrift_create_next_hop(self.client, ip=sai_ipaddress(
                nexthop_device.ipv4), router_interface_id=rif, type=SAI_NEXT_HOP_TYPE_IP)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

        if nexthop_device.ip_prefix_v6:
            nhopv6_id = sai_thrift_create_next_hop(self.client, ip=sai_ipprefix(
                nexthop_device.ipv6 + '/' + nexthop_device.ip_prefix_v6), router_interface_id=rif, type=SAI_NEXT_HOP_TYPE_IP)
        else:
            nhopv6_id = sai_thrift_create_next_hop(self.client, ip=sai_ipaddress(
                nexthop_device.ipv6), router_interface_id=rif, type=SAI_NEXT_HOP_TYPE_IP)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
        nhopv4: Nexthop = Nexthop(
            oid=nhopv4_id, nexthop_device=nexthop_device, rif_id=rif)
        nhopv6: Nexthop = Nexthop(
            oid=nhopv6_id, nexthop_device=nexthop_device, rif_id=rif)
        self.test_obj.dut.nexthopv4_list.append(nhopv4)
        self.test_obj.dut.nexthopv6_list.append(nhopv6)

        return nhopv4, nhopv6

    def create_nexthop_group_by_nexthops(self, nexthopv4_list: List[Nexthop], nexthopv6_list: List[Nexthop], dest_device: Device):
        """
        Create nexthop group by nexthops. Each element in all lists corresponds one by one.
        Set dut attribute: nexthopv4_list, nexthopv6_list
        Set lag attribute: nexthop_groupv4, nexthop_groupv6
        Set device attribute: nexthopv4, nexthopv6
        Attrs:
            nexthopv4_list: A list of ipv4 nexthops to form a nexthop group
            nexthopv6_list: A list of ipv6 nexthops to form a nexthop group
            dest_device: Simulating the bypass device, use this device to get the ipaddress, ipprefix_v4 and ipprefix_v6
        return nexthop group for v4 and nexthop group for v6 
        """
        nhop_groupv4_id = sai_thrift_create_next_hop_group(self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
        nhop_groupv6_id = sai_thrift_create_next_hop_group(self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

        nhp_grpv4_members, nhp_grpv6_members = [], []
        for nexthopv4, nexthopv6 in zip(nexthopv4_list, nexthopv6_list):
            nhp_grpv4_member = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=nhop_groupv4_id,
                next_hop_id=nexthopv4.oid)
            self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
            nhp_grpv6_member = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=nhop_groupv6_id,
                next_hop_id=nexthopv6.oid)
            self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

            nhp_grpv4_members.append(nhp_grpv4_member)
            nhp_grpv6_members.append(nhp_grpv6_member)

        member_port_indexs = [17, 18, 19, 20, 21, 22, 23, 24]
        nhp_grpv4: NexthopGroup = NexthopGroup(nhop_groupv4_id, nhp_grpv4_members, member_port_indexs)
        nhp_grpv6: NexthopGroup = NexthopGroup(nhop_groupv6_id, nhp_grpv6_members, member_port_indexs)

        self.test_obj.dut.nhp_grpv4_list.append(nhp_grpv4)
        self.test_obj.dut.nhp_grpv6_list.append(nhp_grpv6)
        self.create_route_by_nexthop_group(dest_device, nhp_grpv4, nhp_grpv6)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

        return nhp_grpv4, nhp_grpv6

    def choice_virtual_route(self, virtual_router=None):
        """
        Depends on if virtual_router_id is None, return the default virtual or deinded vr.

        Attrs
            virtual_router_id: defined vr
        """

        if virtual_router is None:
            return self.test_obj.dut.default_vrf

        return virtual_router

    def remove_nhop_member_by_lag_idx(self, nhp_grp_obj, lag_idx):
        """
        Remove nexthop member by lag index.
        This method will remove the nexthop member and port index from nexthop group object.
        Args:
            nhp_grp_obj: nexthop group object.
            lag_idx: lag index.
        """
        nhp_grp: NexthopGroup = nhp_grp_obj
        index = lag_idx - 1
        port_indexs = self.test_obj.dut.lag_list[index].member_port_indexs
        sai_thrift_remove_next_hop_group_member(self.client, nhp_grp.nhp_grp_members[index])
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
        nhp_grp.nhp_grp_members.remove(nhp_grp.nhp_grp_members[index])
        for port_index in port_indexs:
            nhp_grp.member_port_indexs.remove(port_index)

    def create_nhop_member_by_lag_port_idxs(self, nhp_grp_obj, lag_idx):
        """
        Create nexthop members for a nexthop group.
        This method will create the nexthop member and port index from nexthop group object.
        Args:
            nhp_grp_obj: nexthop group object
            lag_idx: lag index
        Returns:
            nhop_member: nexthop member
        """
        nhp_grp: NexthopGroup = nhp_grp_obj
        index = lag_idx - 1
        nhop_member = sai_thrift_create_next_hop_group_member(self.client,
                                                              next_hop_group_id=nhp_grp_obj.nhp_grp_id,
                                                              next_hop_id=self.test_obj.dut.nexthopv4_list[index].oid)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
        nhp_grp.nhp_grp_members.insert(index, nhop_member)
        port_indexs = self.test_obj.dut.lag_list[index].member_port_indexs
        for offset, port_index in enumerate(port_indexs):
            nhp_grp.member_port_indexs.insert(index * 2 + offset, port_index)
        return nhop_member
