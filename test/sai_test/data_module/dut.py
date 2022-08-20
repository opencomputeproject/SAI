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
from typing import Dict, List
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from data_module.vlan import Vlan
    from data_module.lag import Lag
    from data_module.nexthop import Nexthop
    from data_module.ecmp import Ecmp
    from data_module.port import Port


class Dut(object):
    """
    Dut config, represent the dut object in the test structure.
    Class attributes:
            default_vrf 
            default_ipv6_route_entry 
            default_ipv4_route_entry 
            loopback_intf 
            local_10v6_route_entry 
            local_128v6_route_entry
            routev4_list
            routev6_list 

            # vlan
            default_vlan_id 
            vlans

            # switch
            switch_id 

            # fdb
            default_vlan_fdb_list 
            vlan_10_fdb_list 
            vlan_20_fdb_list 

            # port
            default_1q_bridge_id 
            default_trap_group 
            host_intf_table_id 
            port_list
            hostif_list 
            port_rif_list
            rif_list       

            # lag
            lag1 
            lag2

            #L3
            nexthopv4_list: next hop id list
            nexthopv6_list: next hop id list
            neighborv4_list
            neighborv6_list

            # ecmp
            ecmp_list:  Ecmp list, contains ecmp objects
    """

    def __init__(self):
        """
        Init all of the class attributes
        """

        # router
        self.default_vrf = None
        self.default_ipv6_route_entry = None
        self.default_ipv4_route_entry = None
        self.loopback_intf = None
        self.local_10v6_route_entry = None
        self.local_128v6_route_entry = None
        self.routev4_list: List = []
        self.routev6_list: List = []

        # vlan
        self.default_vlan_id = None
        self.vlans: Dict[int, Vlan] = {}
        """
        Vlan object list, key: int, Value: Vlan object
        """

        # switch
        self.switch_id = None

        # fdb
        self.default_vlan_fdb_list: List = None
        self.vlan_10_fdb_list: List = None
        self.vlan_20_fdb_list: List = None

        # port
        self.default_1q_bridge_id = None
        self.default_trap_group = None
        """
        Local device port index list, 0, 1, ...
        """
        self.host_intf_table_id = None
        self.port_obj_list: List['Port'] = []
        """
        Port object list
        """
        self.hostif_list = None
        """
        Host interface list
        """
        self.port_rif_list: List = []
        """
        Port rif list. Size of the rif equals to the ports size, value will be None if not mapping to a rif
        """
        self.rif_list: List = []
        """
        Rif list. save the rif object id.
        """

        # lag
        self.lag1: Lag = None
        self.lag2: Lag = None

        # ecmp
        self.ecmp_list: List[Ecmp] = []
        """
        Ecmp list, contains ecmp objects
        """

        # nexthop
        self.nexthopv4_list: List[Nexthop] = []
        """
        nexthop list, contains nexthop objects
        """
        self.nexthopv6_list: List[Nexthop] = []
        """
        nexthop list, contains nexthop objects
        """

        self.neighborv4_list = []
        self.neighborv6_list = []
