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
from data_module.nexthop import Nexthop
if TYPE_CHECKING:
    from sai_test_base import T0TestBase


class Vlan(object):
    """
    Represent the vlan object

    Attrs:
        vlan_id: vlan id
        vlan_oid: vlan ojbect id
        vlan_moids: vlan member object ids
        rif: vlan related route interface
        nexthopv4: vlan related nexthop
        nexthopv6: vlan related nexthop

    """

    def __init__(self, vlan_id=None, vlan_oid=None, vlan_moids: List = [], rif=None, nexthopv4:Nexthop=None, nexthopv6:Nexthop=None):
        """
        Init Vlan object.

        Init following attrs:
            vlan_id
            vlan_oid
            vlan_mport_oids
            rif
            nexthopv4
            nexthopv6
        """
        self.vlan_id = vlan_id
        self.vlan_oid = vlan_oid
        self.vlan_mport_oids: List = vlan_moids
        self.rif = None
        self.nexthopv4 = nexthopv4
        self.nexthopv6 = nexthopv6

    def create_vlan_interface(self, test_object: 'T0TestBase'):
        """
        Create vlan interface for this vlan object

        Attrs:
            test_object: test object contains the method for creating the interface
        """

        self.rif = test_object.create_vlan_interface(self)
