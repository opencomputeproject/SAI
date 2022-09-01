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
from data_module.routable_item import route_item
from data_module.data_obj import auto_str
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from data_module.nexthop import Nexthop

@auto_str
class NexthopGroup(route_item):
    """
    Represent the nexthop_group object.
    Attrs:
        nhp_grp_id: nexthop group id
        nhp_grp_members: next hops
        member_port_indexs: nexthop group port member indexes
    """

    def __init__(self, nhp_grp_id=None, nhp_grp_members: List['Nexthop'] = [], member_port_indexs: List = []):
        """
        Init nexthop_group Object
        Init following attrs:
            nhp_grp_id
            nhp_grp_members
            member_port_indexs
            lags
        """
        self.nhp_grp_id = nhp_grp_id
        """
        nexthop_group id
        """
        self.nhp_grp_members: List[Nexthop] = nhp_grp_members
        """
        nexthop_group members
        """
        self.member_port_indexs: List = member_port_indexs
        """
        nexthop_group port member indexes
        """
