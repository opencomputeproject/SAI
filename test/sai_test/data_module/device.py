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
from constant import *
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from data_module.nexthop import Nexthop
    from data_module.ecmp import Ecmp
    from data_module.lag import Lag


class DeviceType(Enum):
    server = 'server'
    t0 = 't0'
    t1 = 't1'


class Device(object):
    """
        class attributes:
            type: device type, T1, Server
            id: device id, equals to index
            group_id: device group id
            mac: mac address
            ipv4: ip v4
            ipv6: ip v6
            l2_egress_port_idx: L2 destination port index, defined in fdb configer
            l3_port_idx: L3 destination port index, defined in route configer
            l3_lag_obj: L3 destination port object, defined in route configer
            route_id
            nexthop
            neighbor_id
            fdb_entry
    """

    def __init__(self, device_type, id, group_id=None):
        """
        Init the Device object, different device type  have different attributes

        Set the following class attributes:
            type: device type, T1, Server
            id: device id, equals to index
            group_id: device group id
            mac: mac address
            ipv4: ip v4
            ipv6: ip v6
            l2_egress_port_idx
            l3_port_idx
            l3_lag_obj
            ecmp

        Server:
            self.ip_pattern: SERVER_IPV4_PATTERN
            self.ip_pattern_v6: SERVER_IPV6_PATTERN
            self.fdb_device_num: FDB_SERVER_NUM

        T1:
            self.ip_pattern: T1_IPV4_PATTERN
            self.ip_pattern_v6: T1_IPV6_PATTERN
            self.fdb_device_num: FDB_T1_NUM

        """
        self.type = device_type
        """
        device type, T1, Server
        """
        self.id = id
        """
        device id, equals to index
        """
        self.group_id = group_id
        """
        device group id
        """

        self.ip_prefix = None
        self.ip_prefix_v6 = None
        if self.type == DeviceType.server:
            self.ip_pattern = SERVER_IPV4_PATTERN
            self.ip_pattern_v6 = SERVER_IPV6_PATTERN
            self.fdb_device_num = FDB_SERVER_NUM
        elif self.type == DeviceType.t1:
            self.ip_pattern = T1_IPV4_PATTERN
            self.ip_pattern_v6 = T1_IPV6_PATTERN
            self.fdb_device_num = FDB_T1_NUM

        self.mac = self._generate_mac_address()
        """
        mac address
        """
        self.ipv4 = self._generate_ipv4_address()
        """
        ip v4
        """
        self.ipv6 = self._generate_ipv6_address()
        """
        ip v6
        """

        # L2 forwarding info
        self.l2_egress_port_idx = None
        """
        L2 destination port index, defined in fdb configer
        """

        # L3 route info
        self.l3_port_idx = None
        """
        L3 destination port index, defined in route configer
        """
        self.l3_lag_obj: Lag = None
        """
        L3 destination lag object, defined in route configer
        """
        self.ecmp : Ecmp = None
        """
        L3 destination ecmp object, defined in route configer
        """


    def _generate_ipv4_address(self):
        """
        Generate ipv4 address.
        """
        return self.ip_pattern.format(self.group_id, self.id)

    def _generate_ipv6_address(self):
        """
        Generate ipv6 address.
        """
        return self.ip_pattern_v6.format(self.group_id, self.id)

    def _generate_mac_address(self):
        """
        Generate mac address.
        """
        return FDB_MAC_PREFIX + ':' + self.fdb_device_num + ':' + \
            '{:02x}'.format(self.group_id) + ':' + '{:02x}'.format(self.id)
