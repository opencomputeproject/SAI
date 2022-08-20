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

if TYPE_CHECKING:
    from sai_test_base import T0TestBase
    from data_module.nexthop import Nexthop

from data_module.routable_item import route_item

class Vlan(route_item):
    """
    Represent the vlan object

    Attrs:
        oid: vlan ojbect id
        vlan_moids: vlan member object ids
    Attrs from super:
        oid: object id
        rif: lag related route interface
        nexthopv4: related nexthop list
        nexthopv6: related nexthop list

    """

    def __init__(self, vlan_id=None, oid=None, vlan_moids: List = [], rif_list:List=[], nexthopv4_list:List['Nexthop'] = [], nexthopv6_list:List['Nexthop'] = []):
        """
        Init Vlan object.

        Init following attrs:
            vlan_id
            oid
            vlan_mport_oids
            rif_list
            nexthopv4_list
            nexthopv6_list
        """
        super().__init__(oid=oid, rif_list=rif_list, nexthopv4_list=nexthopv4_list, nexthopv6_list=nexthopv6_list)
        self.vlan_id = vlan_id
        """
        vlan id
        """
        self.oid = oid
        """
        vlan ojbect id
        """
        self.vlan_mport_oids: List = vlan_moids
        """
        vlan member object ids
        """

    def create_vlan_interface(self, test_object: 'T0TestBase', reuse=True):
        """
        Create vlan interface for this vlan object

        Attrs:
            test_object: test object contains the method for creating the interface
        """

        test_object.create_vlan_interface(self)
