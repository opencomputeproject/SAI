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
if TYPE_CHECKING:
    from sai_test_base import T0TestBase
    from data_module.nexthop import Nexthop
from data_module.routable_item import route_item
from data_module.nexthop_group import NexthopGroup

@auto_str
class Lag(route_item):
    """
    Represent the lag object.
    Attrs:        
        lag_members: lag members
        member_port_indexs: lag port member indexes
    Attrs from super:
        oid: object id
        rif_list: lag related route interface
        nexthopv4: related nexthop list
        nexthopv6: related nexthop list
    """

    def __init__(self,
                 oid=None,
                 lag_members: List = [],
                 member_port_indexs: List = [],
                 rif_list:List=[],
                 nexthopv4_list:List['Nexthop'] = [],
                 nexthopv6_list:List['Nexthop'] = [],
                 nexthop_groupv4: NexthopGroup = None,
                 nexthop_groupv6: NexthopGroup = None
                ):
        """
        Init Lag Object
        Init following attrs:
            oid
            lag_members
            member_port_indexs
            rif
            nexthopv4
            nexthopv6
            nexthop_groupv4
            nexthop_groupv6
        """
        super().__init__(oid=oid, rif_list=rif_list, nexthopv4_list=nexthopv4_list, nexthopv6_list=nexthopv6_list)
        self.lag_members: List = lag_members
        """
        lag members
        """
        self.member_port_indexs: List = member_port_indexs
        """
        lag port member indexes
        """
        self.nexthop_groupv4 = nexthop_groupv4
        """
        lag belongs to which nexthop group ipv4
        """
        self.nexthop_groupv6 = nexthop_groupv6
        """
        lag belongs to which nexthop group ipv6
        """

    def create_lag_interface(self, test_object: 'T0TestBase', reuse=True):
        """
        Create vlan interface for this vlan object

        Attrs:
            test_object: test object contains the method for creating the interface
        """
        test_object.create_lag_interface(self, reuse)
