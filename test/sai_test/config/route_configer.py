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

def t0_route_config_helper(test_obj, is_create_route_for_lag=True):
    route_configer = RouteConfiger(test_obj)

    if is_create_route_for_lag:
        ip_addr1 = '10.10.10.0'
        mac_addr1 = '02:04:02:01:01:01'
        route_configer.create_route_and_neighbor_entry_for_port(ip_addr=ip_addr1, mac_addr=mac_addr1, port_id=test_obj.lag1.lag_id)

        ip_addr2 = '10.1.2.100'
        mac_addr2 = '02:04:02:01:02:01'
        route_configer.create_route_and_neighbor_entry_for_port(ip_addr=ip_addr1, mac_addr=mac_addr2, port_id=test_obj.lag2.lag_id)

class RouteConfiger(object):
    """
    Class use to make all the route configurations.
    """

    def __init__(self, test_obj) -> None:
        """
        Init Route configer.

        Args:
            test_obj: the test object
        """
        self.test_obj = test_obj
        self.client = test_obj.client

    def get_default_virtual_router_id(self):
        print("Get default router id")
        def_attr = sai_thrift_get_switch_attribute(self.client, default_virtual_router_id=True)
        
        self.test_obj.assertNotEqual(def_attr["SAI_SWITCH_ATTR_DEFAULT_VIRTUAL_ROUTER_ID"], 0)
        return def_attr["SAI_SWITCH_ATTR_DEFAULT_VIRTUAL_ROUTER_ID"]

    def create_route_and_neighbor_entry_for_port(self, ip_addr, mac_addr, port_id, virtual_router_id=None):
        if virtual_router_id is None:
            virtual_router_id = self.get_default_virtual_router_id()
        
        rif_id1 = sai_thrift_create_router_interface(self.client, virtual_router_id=router_id, type=SAI_ROUTER_INTERFACE_TYPE_PORT, port_id=port_id)
        
        nbr_entry_v4 = sai_thrift_neighbor_entry_t(rif_id=rif_id1, ip_address=sai_ipaddress(ip_addr))
        sai_thrift_create_neighbor_entry(self.client, nbr_entry_v4, dst_mac_address=mac_addr)

        nhop = sai_thrift_create_next_hop(self.client, ip=sai_ipaddress(ip_addr), router_interface_id=rif_id1, type=SAI_NEXT_HOP_TYPE_IP)
        route1 =sai_thrift_route_entry_t(vr_id=router_id, destination=sai_ipprefix(ip_addr+'/24'))
        sai_thrift_create_route_entry(self.client, route1, next_hop_id=nhop)
