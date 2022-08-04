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

        Add those following attribute to this class:
        self.local_server_ip_list for all the local server mac
    """
    def __init__(self,device_type,id,group_id=None,ip_num=150):
        self.type = device_type
        self.id = id
        self.group_id = group_id
        self.ip_num = ip_num
        if self.type == DeviceType.server:
            self.ip_prefix = SERVER_IPV4_PREFIX
            self.fdb_device_num = FDB_SERVER_NUM
        elif self.type == DeviceType.t1:
            self.ip_prefix = T1_IPV4_PREFIX
            self.fdb_device_num = FDB_T1_NUM
        elif self.type == DeviceType.t0:
            self.ip_prefix = T0_IPV4_PREFIX
            self.fdb_device_num = FDB_T0_NUM

        self.mac = self._generate_mac_address()
        self.ipv4 = self._generate_ipv4_address()

    def _generate_ipv4_address(self):
        """
        Generate ip address.
        """
        return self.ip_prefix.format(self.group_id,self.id)

    
    def _generate_mac_address(self):
        """
        Generate mac address.
        """
        return FDB_MAC_PREFIX + ':' + self.fdb_device_num + ':' + \
            '{:02d}'.format(self.group_id) + ':' + '{:02d}'.format(self.id)
