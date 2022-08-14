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


class Lag(object):
    """
    Represent the lag object.
    Attrs:
        lag_id: lag id
        lag_members: lag members
        member_port_indexs: lag port member indexes
        rif: lag related route interface
        nexthopv4: lag related nexthop
        nexthopv6: lag related nexthop
    """

    def __init__(self, lag_id=None, lag_members: List = [], member_port_indexs: List = [], rif=None, nexthopv4:'Nexthop'=None, nexthopv6:'Nexthop'=None):
        """
        Init Lag Object
        Init following attrs:
            lag_id
            lag_members
            member_port_indexs
            rif
            nexthopv4
            nexthopv6
        """
        self.lag_id = lag_id
        self.lag_members: List = lag_members
        self.member_port_indexs: List = member_port_indexs
        self.rif = rif
        self.nexthopv4 = nexthopv4
        self.nexthopv6 = nexthopv6

    def create_lag_interface(self, test_object: 'T0TestBase'):
        """
        Create vlan interface for this vlan object

        Attrs:
            test_object: test object contains the method for creating the interface
        """        
        self.rif = test_object.create_lag_interface(self)        
