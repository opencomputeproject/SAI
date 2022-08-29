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
class Ecmp(object):
    """
    Represent the ecmp(next hop group) object.
    Attrs:
        ecmp_id: ecmp id(nexthop group id)
        ecmp_members: ecmp members(next hops)
        member_port_indexs: ecmp port member indexes
    """

    def __init__(self, ecmp_id=None, ecmp_members: List['Nexthop'] = [], member_port_indexs: List = []):
        """
        Init ecmp Object
        Init following attrs:
            ecmp_id
            ecmp_members
            member_port_indexs
            lags
        """
        self.ecmp_id = None
        """
        ecmp id (nexthop group id)
        """
        self.ecmp_members: List[Nexthop] = ecmp_members
        """
        ecmp members(next hop ids)
        """
        self.member_port_indexs: List = member_port_indexs
        """
        ecmp port member indexes
        """
