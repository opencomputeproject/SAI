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
from data_module.data_obj import data_item
if TYPE_CHECKING:
    from sai_test_base import T0TestBase
    from data_module.nexthop import Nexthop


class route_item(data_item):
    """
    Represent the route item data object.
    Attrs:
        rif: related route interface
    """

    def __init__(self, oid=None, rif_list:List=[], nexthopv4_list:List['Nexthop'] = [], nexthopv6_list:List['Nexthop'] = []):
        """
        Init Lag Object
        Init following attrs:
            rif
        """
        super().__init__(oid=oid)
        self.rif_list = rif_list
        self.nexthopv4_list = nexthopv4_list
        """
        Next hop device for v4 ip, use to retrieve the nexthop ipv4
        """
        self.nexthopv6_list = nexthopv6_list
        """
        Next hop device for v6 ip, use to retrieve the nexthop ipv6
        """
        self.neighbor_mac = None
        """
        Next hop device mac, expect it should be a unique one
        """
