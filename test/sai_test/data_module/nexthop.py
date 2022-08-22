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
from enum import Enum
from typing import List

from typing import TYPE_CHECKING
from data_module.routable_item import data_item
if TYPE_CHECKING:
    from data_module.device import Device


class Nexthop(data_item):
    """
    Represent the Nexthopobject.
    Attrs:
        oid: Nexthop object id
        nexthop_device: nexthop_device
        rif_id: router referenced rif object id
    """

    def __init__(self, oid=None, nexthop_device: 'Device' = None, rif_id=None):
        """
        Init Nexthop Object
        Init following attrs:
            oid
            nexthop_device
            rif_id
        """
        super().__init__(oid=oid)
        self.nexthop_device = nexthop_device
        """
        nexthop_device
        """
        self.rif_id = rif_id
        """
        referenced rif object id
        """


class PortType(Enum):
    port = 'port'
    bridge_port = 'bridge'
    vlan = 'vlan'
    lag = 'lag'
    tunnel = 'tunnel'
