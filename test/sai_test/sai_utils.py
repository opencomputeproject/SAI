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

"""
Thrift SAI interface basic utils.
"""

import os
import time
import struct
import socket
import json

from functools import wraps

from ptf.packet import *
from ptf.testutils import *

from sai_thrift.sai_adapter import *
from constant import *


def sai_ipprefix(prefix_str):
    """
    Set IP address prefix and mask and return ip_prefix object

    Args:
        prefix_str (str): IP address and mask string (with slash notation)

    Return:
        sai_thrift_ip_prefix_t: IP prefix object
    """
    addr_mask = prefix_str.split('/')
    if len(addr_mask) != 2:
        print("Invalid IP prefix format")
        return None

    if '.' in prefix_str:
        family = SAI_IP_ADDR_FAMILY_IPV4
        addr = sai_thrift_ip_addr_t(ip4=addr_mask[0])
        mask = num_to_dotted_quad(addr_mask[1])
        mask = sai_thrift_ip_addr_t(ip4=mask)
    if ':' in prefix_str:
        family = SAI_IP_ADDR_FAMILY_IPV6
        addr = sai_thrift_ip_addr_t(ip6=addr_mask[0])
        mask = num_to_dotted_quad(int(addr_mask[1]), ipv4=False)
        mask = sai_thrift_ip_addr_t(ip6=mask)

    ip_prefix = sai_thrift_ip_prefix_t(
        addr_family=family, addr=addr, mask=mask)
    return ip_prefix


def num_to_dotted_quad(address, ipv4=True):
    """
    Helper function to convert the ip address

    Args:
        address (str): IP address
        ipv4 (bool): determines what IP version is handled

    Returns:
        str: formatted IP address
    """
    if ipv4 is True:
        mask = (1 << 32) - (1 << 32 >> int(address))
        return socket.inet_ntop(socket.AF_INET, struct.pack('>L', mask))

    mask = (1 << 128) - (1 << 128 >> int(address))
    i = 0
    result = ''
    for sign in str(hex(mask)[2:]):
        if (i + 1) % 4 == 0:
            result = result + sign + ':'
        else:
            result = result + sign
        i += 1
    return result[:-1]


class ConfigDBOpertion():
    '''
    read config from config_db.json
    '''

    def __init__(self):
        path = os.path.join(os.path.dirname(__file__),
                            "resources/config_db.json")  # REPLACE
        self.config_json = None
        with open(path, mode='r') as f:
            self.config_json = json.load(f)

    def get_port_config(self):
        '''
        RETURN:
            dict: port config
        '''
        port_conf = self.config_json.get('PORT')
        key_0 = list(port_conf.keys())[0]
        return self.config_json.get('PORT').get(key_0)


def sai_ipaddress(addr_str):
    """
    Set SAI IP address, assign appropriate type and return
    sai_thrift_ip_address_t object

    Args:
        addr_str (str): IP address string

    Returns:
        sai_thrift_ip_address_t: object containing IP address family and number
    """

    if '.' in addr_str:
        family = SAI_IP_ADDR_FAMILY_IPV4
        addr = sai_thrift_ip_addr_t(ip4=addr_str)
    if ':' in addr_str:
        family = SAI_IP_ADDR_FAMILY_IPV6
        addr = sai_thrift_ip_addr_t(ip6=addr_str)
    ip_addr = sai_thrift_ip_address_t(addr_family=family, addr=addr)

    return ip_addr


def generate_mac_address_list(role, group, indexes):
    """
    Generate mac addresses.

    Args:
        role: Role which is represented by the mac address(base on test plan config)
        group: group number for the mac address(base on test plan config)
        indexes: mac indexes

    Returns:
        default_1q_bridge_id
    """
    print("Generate MAC ...")
    mac_list = []
    for index in indexes:
        mac = FDB_MAC_PREFIX + ':' + role + ':' + \
            '{:02d}'.format(group) + ':' + '{:02d}'.format(index)
        mac_list.append(mac)
    return mac_list


def generate_ip_address_list(role, group, indexes):
    """
    Generate ip addresses.

    Args:
        role: Role which is represented by the ip address(base on test plan config)
        group: group number for the ip address(base on test plan config)
        indexes: ip indexes

    Returns:
        default_1q_bridge_id
    """
    print("Generate IP ...")
    ip_list = []
    for index in indexes:
        ip_list.append(role.format(group, index))
    return ip_list

