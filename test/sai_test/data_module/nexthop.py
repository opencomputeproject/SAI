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

from enum import Enum
from typing import List
from data_module.lag import Lag


class PortType(Enum):
    port = 'port'
    vlan = 'vlan'
    lag = 'lag'
    tunnel = 'tunnel'

class Nexthop(object):
    """
    Represent the Nexthopobject.
    Attrs:
        nexthop_id: Nexthop id
        port_idx: related port idx (optional, need to check if None)
        lag: related lag (optional, need to check if None)
        tunnel_idx: related tunnel idx (optional, need to check if None)
    """

    def __init__(self, nexthop_id=None, port_idx=None, lag:Lag=None, tunnel_idx=None):
        """
        Init Nexthop Object
        Init following attrs:
            nexthop_id
            port_idx
            lag
            tunnel_idx
        """
        self.nexthop_id = None
        """
        Nexthop id
        """
        self.port_idx = port_idx
        """
        related port idx (optional, need to check if None)
        """
        self.lag = port_idx
        """
        related lag (optional, need to check if None)
        """
        self.tunnel_idx = tunnel_idx
        """
        related tunnel idx (optional, need to check if None)
        """