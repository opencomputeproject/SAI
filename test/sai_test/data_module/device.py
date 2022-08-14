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
from data_module.nexthop import Nexthop
from data_module.ecmp import Ecmp


class DeviceType(Enum):
    server = 'server'
    t0 = 't0'
    t1 = 't1'


class Device():
    """
        Create servers(0-17) ip list.

        server0: IP 192.168.0.1~150
        server1: IP 192.168.1.1~150
        server2: IP 192.168.2.1~150
        .....

        class attributes:
            type: device type, T1, Server
            id: device id, equals to index
            group_id: device group id
            ip_num: numbers of ips
            mac: mac address
            ipv4: ip v4
            ipv6: ip v6
            l2_egress_port_idx: L2 destination port index, defined in fdb configer
            l2_egress_port_id: L2 destination port object id, defined in fdb configer
            l3_egress_port_idx: L3 destination port index, defined in route configer
            l3_egress_port_id: L3 destination port object id, defined in route configer
            l3_egress_lag_obj: L3 destination port object, defined in route configer
            route_id
            nexthop
            ecmp_egress: ecmp object for egress
            neighbor_id
            fdb_entry
    """

    def __init__(self, device_type, id, group_id=None, ip_num=150):
        """
        Init the Device object, different device type  have different attributes

        Set the following class attributes:
            type: device type, T1, Server
            id: device id, equals to index
            group_id: device group id
            ip_num: numbers of ips
            mac: mac address
            ipv4: ip v4
            ipv6: ip v6
            l2_egress_port_idx
            l2_egress_port_id
            l3_egress_port_idx
            l3_egress_port_id
            l3_egress_lag_obj
            routev4
            nexthoproutev4
            nexthopv6
            nexthoproutev6
            neighborv6_id
            local_neighborv6_id            
            neighborv4_id
            local_neighborv4_id
            fdb_entry

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
        self.ip_num = ip_num
        """
        numbers of ips
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
        self.l2_egress_port_id = None
        """
        L2 destination port object id, defined in fdb configer
        """

        # L3 route info
        self.l3_egress_port_idx = None
        """
        L3 destination port index, defined in route configer
        """
        self.l3_egress_port_id = None
        """
        L3 destination port object id, defined in route configer
        """
        self.l3_egress_lag_obj = None
        """
        L3 destination port object, defined in route configer
        """
        self.ecmp_egress: Ecmp = None
        """
        Ecmp object.
        """

        self.routev4 = None
        self.nexthopv4: Nexthop = None
        self.routev6 = None
        self.nexthopv6: Nexthop = None

        self.neighborv4_id = None
        """
        No host neighbor
        """
        self.local_neighborv4_id = None
        """
        Host, direct neighbor
        """
        self.neighborv6_id = None
        """
        No host neighbor
        """
        self.local_neighborv6_id = None
        """
        Host, direct neighbor
        """
        self.fdb_entry = None

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
            '{:02d}'.format(self.group_id) + ':' + '{:02d}'.format(self.id)
