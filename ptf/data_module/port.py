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

from typing import List, Dict
from typing import TYPE_CHECKING
from data_module.data_obj import auto_str
from data_module.data_obj import data_item


@auto_str
class Port(data_item):
    """
    Represent the port object.
    Attrs:
        port_index: port index
        dev_port_index: device port, local device port index
        port_oid: port object id
        bridge_port_oid: bridge port object id
    Attrs from super:
        rif: port related route interface
        nexthopv4: related nexthop list
        nexthopv6: related nexthop list
    """

    def __init__(
            self,
            oid=None,
            port_index=None,
            dev_port_index=None,
            dev_port_eth=None,
            bridge_port_oid=None):
        """
        Init Port Object
        Init following attrs:
            oid: port object id
            port_index: port index
            dev_port_index: device port, local device port index
            dev_port_eth: local device port eth name
            bridge_port_oid: bridge port object id
        """
        super().__init__(oid=oid)
        self.port_index = port_index
        """
        port index
        """
        self.dev_port_index = dev_port_index
        """
        device port, local device port index
        """
        self.dev_port_eth = dev_port_eth
        """
        local device port eth name
        """
        self.bridge_port_oid = bridge_port_oid
        """
        bridge port object id
        """
        self.port_config:Dict = {}
        self.host_itf_idx = None
        """
        Port binded host interface index, the object saved in dut.hostif_list
        """
        self.default_lane_list = []
        """
        default lane list after switch init.
        """
