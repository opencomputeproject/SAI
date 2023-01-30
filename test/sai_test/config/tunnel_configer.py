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
from typing import TYPE_CHECKING
from data_module.tunnel import Tunnel

if TYPE_CHECKING:
    from sai_test_base import T0TestBase


def t0_tunnel_config_helper(test_obj: 'T0TestBase', is_create_tunnel=True):
    """
    Make tunnel configurations base on the configuration in the test plan.
    set the configuration in test directly.

    set the following test_obj attributes:
        tunnel object

    """
    tunnel_configer = TunnelConfiger(test_obj)

    if is_create_tunnel:
        tunnel = tunnel_configer.create_tunnel([1], [17,18])
        tunnel.tun_ip = test_obj.servers[11][1].ipv4
        tunnel_configer.create_tunnel_term(tunnel)
        test_obj.dut.tunnel_list.append(tunnel)

    """
    tunnel_configer.create_tunnel()
    tunnel_configer.create_tunnel_term()
    """


class TunnelConfiger(object):
    """
    Class use to make all the Lag configurations.
    """

    def __init__(self, test_obj: 'T0TestBase') -> None:
        """
        Init Tunnel configrer.

        Args:
            test_obj: the test object
        """
        self.test_obj = test_obj
        self.client = test_obj.client

    def create_tunnel(self, oports, uports):
        """
        Create tunnel.

        Args:
            oports:  oport indexs
            uports:  oport indexs

        Returns:
            Tunnel: tunnel object
        """
        tunnel: Tunnel = Tunnel(uport_indexs=uports, 
                                oport_indexs=oports, 
                                tunnel_type = SAI_TUNNEL_TYPE_IPINIP,
                                term_type = SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P)
        
        # underlay configuration
        self.uvrf = self.test_obj.dut.default_vrf

        # overlay configuration
        self.ovrf = self.test_obj.dut.default_vrf

        tunnel.urif_lpb = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=tunnel.uvrf)

        tunnel.orif_lpb = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=tunnel.ovrf)

        # tunnel
        tunnel.oid = sai_thrift_create_tunnel(
            self.client,
            type=tunnel.tunnel_type,
            encap_src_ip=sai_ipaddress(tunnel.lpb_ip),
            underlay_interface=tunnel.urif_lpb,
            overlay_interface=tunnel.orif_lpb)
        
        return tunnel

    def create_tunnel_term(self, tunnel):
        """
        Create tunnel term.

        Args:
            tunnel:  tunnel object
            tun_ip:  tun_ip
        """
        tunnel_term = sai_thrift_create_tunnel_term_table_entry(
            self.client,
            tunnel_type=tunnel.tunnel_type,
            vr_id=tunnel.uvrf,
            action_tunnel_id=tunnel.oid,
            type=tunnel.term_type,
            dst_ip=sai_ipaddress(tunnel.lpb_ip),
            src_ip=sai_ipaddress(tunnel.tun_ip))
        tunnel.tunnel_terms.append(tunnel_term)

    def create_tunnel_route_v4_v6(self, tunnel, vm_ip, vm_ipv6):
        """
        Create tunnel special route v4 and v6.

        Args:
            tunnel:  tunnel object
            vm_ip: dst ipv4
            vm_ipv6: dst ipv6
        """
        # tunnel nexthop for VM
        tunnel_nhop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP,
            tunnel_id=tunnel.oid,
            ip=sai_ipaddress(tunnel.tun_ip),
            tunnel_mac=tunnel.inner_dmac)

        # routes to VM via tunnel nexthop
        vm_route = sai_thrift_route_entry_t(
            vr_id=tunnel.ovrf, destination=sai_ipprefix(vm_ip + '/32'))
        sai_thrift_create_route_entry(self.client,
                                      vm_route,
                                      next_hop_id=tunnel_nhop)

        vm_v6_route = sai_thrift_route_entry_t(
            vr_id=tunnel.ovrf, destination=sai_ipprefix(vm_ipv6 + '/128'))
        sai_thrift_create_route_entry(self.client,
                                      vm_v6_route,
                                      next_hop_id=tunnel_nhop)
