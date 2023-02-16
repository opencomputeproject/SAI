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

from typing import List
from typing import TYPE_CHECKING
from data_module.data_obj import auto_str
from constant import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]
if TYPE_CHECKING:
    from sai_test_base import T0TestBase
    from data_module.nexthop import Nexthop
from data_module.routable_item import route_item
from data_module.nexthop_group import NexthopGroup

@auto_str
class Tunnel(route_item):

    def __init__(self,
                 oid=None,
                 uport_indexs: List = [],
                 oport_indexs: List = [],
                 tunnel_type = None,
                 term_type = None,
                 ttl_mode = None,
                 rif_list:List= [],
                 nexthopv4_list:List['Nexthop'] = [],
                 nexthopv6_list:List['Nexthop'] = [],
                 nexthop_groupv4: NexthopGroup = None,
                 nexthop_groupv6: NexthopGroup = None,
                ):
        """
        Init Lag Object
        Init following attrs:
            oid
            uport_indexs
            oport_indexs
            tunnel_type
            term_type
            rif
            nexthopv4
            nexthopv6
            nexthop_groupv4
            nexthop_groupv6
        """
        super().__init__(oid=oid, rif_list=rif_list, nexthopv4_list=nexthopv4_list, nexthopv6_list=nexthopv6_list)
        
        self.oport_devs = oport_indexs
        self.uport_devs = uport_indexs

        self.lpb_ip = LOOPBACK_IPV4
        self.tun_ip = None
        self.tun_lpb_mask = "/32"

        self.inner_dmac = INNER_DMAC
 
        # underlay configuration
        self.uvrf = None

        # overlay configuration
        self.ovrf = None

        # loopback RIFs for tunnel
        self.urif_lpb = None

        self.orif_lpb = None

        self.tunnel_type = tunnel_type

        self.term_type = term_type
        
        self.ttl_mode = ttl_mode
        
        self.tunnel_terms = []

    def create_tunnel_route(self, test_object: 'T0TestBase', vm_ip, vm_ipv6):
        """
        Create vlan interface for this vlan object

        Attrs:
            test_object: test object contains the method for creating the interface
            vm_ip: dst ipv4
            vm_ipv6: dst ipv6
        """
        test_object.create_tunnel_route(self, vm_ip, vm_ipv6)
