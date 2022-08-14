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
if TYPE_CHECKING:
    from data_module.device import Device


class Nexthop(object):
    """
    Represent the Nexthopobject.
    Attrs:
        nexthop_id: Nexthop id
        dest_device: dest device
        rif_id: router interface id
        port_idx: related port idx (optional, need to check if None)
        lag: related lag (optional, need to check if None)
        tunnel_idx: related tunnel idx (optional, need to check if None)
    """

    def __init__(self, nexthop_id=None, dest_device:'Device'=None, rif_id=None, port_idx=None, lag=None, tunnel_idx=None):
        """
        Init Nexthop Object
        Init following attrs:
            nexthop_id
            dest_device
            rif_id
            port_idx
            lag
            tunnel_idx
        """
        self.nexthop_id = nexthop_id
        """
        Nexthop id
        """
        self.dest_device = dest_device
        self.port_idx = port_idx
        """
        related port idx (optional, need to check if None)
        """
        self.rif_id = rif_id
        self.lag = lag
        """
        related lag (optional, need to check if None)
        """
        self.tunnel_idx = tunnel_idx
        """
        related tunnel idx (optional, need to check if None)
        """

class PortType(Enum):
    port = 'port'
    bridge_port = 'bridge'
    vlan = 'vlan'
    lag = 'lag'
    tunnel = 'tunnel'
