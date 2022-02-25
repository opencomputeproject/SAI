# Copyright 2013-present Barefoot Networks, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Thrift SAI interface basic tests
"""

#import switch_sai_thrift
from sai_base_test import *
import time
import sys
sys.path.append('../sai_thrift_src/gen-py')
from switch_sai.ttypes import *
import logging

import unittest
import random

import sai_base_test

from ptf import config
from ptf.testutils import *
from ptf.thriftutils import *
# sys.path.append('../../')
# from sai_types import *
import os

#from switch_sai_thrift.ttypes import  *

# from switch_sai_thrift.sai_headers import  *


this_dir = os.path.dirname(os.path.abspath(__file__))

switch_inited=0
port_list = {}
br_port_list = {}
sai_port_list = []
front_port_list = []
table_attr_list = []
router_mac='00:77:66:55:44:00'
rewrite_mac1='00:77:66:55:44:01'
rewrite_mac2='00:77:66:55:44:02'
default_bridge = 0
default_bridge_type = None


is_bmv2 = ('BMV2_TEST' in os.environ) and (int(os.environ['BMV2_TEST']) == 1)

def switch_init(client):
    global switch_inited
    if switch_inited:
        return

    switch_attr_list = client.sai_thrift_get_switch_attribute()
    attr_list = switch_attr_list.attr_list
    for attribute in attr_list:
        if attribute.id == SAI_SWITCH_ATTR_PORT_NUMBER:
            print "max ports: " + attribute.value.u32
        elif attribute.id == SAI_SWITCH_ATTR_PORT_LIST:
            for port_id in attribute.value.objlist.object_id_list:
                attr_value = sai_thrift_attribute_value_t(booldata=1)
                attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_ADMIN_STATE, value=attr_value)
                client.sai_thrift_set_port_attribute(port_id, attr)
                sai_port_list.append(port_id)
        else:
            print "unknown switch attribute"
    attr_value = sai_thrift_attribute_value_t(mac=router_mac)
    attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_SRC_MAC_ADDRESS, value=attr_value)
    client.sai_thrift_set_switch_attribute(attr)
    all_ports_are_up = True
    for num_of_tries in range(200):
        time.sleep(1)
        # wait till the port are up
        for port in sai_port_list:
            port_attr_list = client.sai_thrift_get_port_attribute(port)
            attr_list = port_attr_list.attr_list
            for attribute in attr_list:
                if attribute.id == SAI_PORT_ATTR_OPER_STATUS:
                    if attribute.value.s32 != SAI_PORT_OPER_STATUS_UP:
                        all_ports_are_up = False
                        print "port 0x%x is down" % port
        if all_ports_are_up:
            break
        else:
            all_ports_are_up = True
    if not all_ports_are_up:
        raise RuntimeError('Not all of the  ports are up')

    thrift_attr = client.sai_thrift_get_port_list_by_front_port()
    if thrift_attr.id == SAI_SWITCH_ATTR_PORT_LIST:
        for port_id in thrift_attr.value.objlist.object_id_list:
            front_port_list.append(port_id)
    for interface,front in interface_to_front_mapping.iteritems():
        sai_port_id = client.sai_thrift_get_port_id_by_front_port(front);
        port_list[int(interface)]=sai_port_id
    switch_inited = 1

def switch_init2(client):
    global switch_inited
    if switch_inited:
        return
    # client.sai_thrift_create_switch([])
    attr_value = sai_thrift_attribute_value_t(oid=0)
    attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_DEFAULT_1Q_BRIDGE_ID, value=attr_value)
    # attr_value2 = sai_thrift_attribute_value_t(objlist=0)
    # attr2 = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_PORT_LIST, value=attr_value)
    # attr_list = client.sai_thrift_get_switch_attribute(thrift_attr_list=[attr, attr2])
    attr_list = client.sai_thrift_get_switch_attribute(thrift_attr_list=[attr])
    default_bridge = attr_list.attr_list[0].value.oid
    for interface,front in interface_to_front_mapping.iteritems():
        sai_port_id = client.sai_thrift_get_port_id_by_front_port(front)
        port_list[int(interface)]=sai_port_id


    attr_list = []
    attr_value = sai_thrift_attribute_value_t(objlist=None)
    attr_list.append(sai_thrift_attribute_t(id= SAI_BRIDGE_ATTR_PORT_LIST, value=attr_value))
    attr_value = sai_thrift_attribute_value_t(s32=None)
    attr_list.append(sai_thrift_attribute_t(id= SAI_BRIDGE_ATTR_TYPE, value=attr_value))
    attr_list = client.sai_thirft_get_bridge_attribute(default_bridge, attr_list)
    default_bridge_type = attr_list.attr_list[1].value.s32
    for br_port in attr_list.attr_list[0].value.objlist.object_id_list:
        attr_value = sai_thrift_attribute_value_t(oid=None)
        attr = sai_thrift_attribute_t(id= SAI_BRIDGE_PORT_ATTR_PORT_ID, value=attr_value)
        port_id = client.sai_thirft_get_bridge_port_attribute(br_port,[attr]).attr_list[0].value.oid
        br_port_list[port_id] = br_port

    switch_inited = 1
    # attr_list = []
    # attr_value = sai_thrift_attribute_value_t(u32list=None)
    # attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_HW_LANE_LIST, value=attr_value)
    # attr_list.append(attr)
    # attr_list = client.sai_thrift_get_port_attribute(port1, attr_list)
    # hw_port1 = attr_list.attr_list[0].value.u32list.u32list[0]
    # attr_list = []
    # attr_value = sai_thrift_attribute_value_t(u32list=None)
    # attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_HW_LANE_LIST, value=attr_value)
    # attr_list.append(attr)
    # attr_list = client.sai_thrift_get_port_attribute(port2, attr_list)
    # hw_port2 = attr_list.attr_list[0].value.u32list.u32list[0]

def sai_thrift_get_cpu_port_id(client):
    cpu_port = client.sai_thrift_get_cpu_port_id()
    return cpu_port

def sai_thrift_get_default_router_id(client):
    default_router_id = client.sai_thrift_get_default_router_id()
    return default_router_id

def sai_thrift_create_bridge_port(client, bridge_port_type, port_id, vlan_id, bridge_id):
    vport_attr_list = []
    
    vport_attr_value = sai_thrift_attribute_value_t(s32=bridge_port_type)
    vport_attr = sai_thrift_attribute_t(id= SAI_BRIDGE_PORT_ATTR_TYPE, value=vport_attr_value)
    vport_attr_list.append(vport_attr)

    vport_attr_value = sai_thrift_attribute_value_t(oid=port_id)
    vport_attr = sai_thrift_attribute_t(id= SAI_BRIDGE_PORT_ATTR_PORT_ID, value=vport_attr_value)
    vport_attr_list.append(vport_attr)

    if (bridge_port_type == SAI_BRIDGE_PORT_TYPE_SUB_PORT):
        vport_attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        vport_attr = sai_thrift_attribute_t(id= SAI_BRIDGE_PORT_ATTR_VLAN_ID, value=vport_attr_value)
        vport_attr_list.append(vport_attr)

    vport_attr_value = sai_thrift_attribute_value_t(oid=bridge_id)
    vport_attr = sai_thrift_attribute_t(id= SAI_BRIDGE_PORT_ATTR_BRIDGE_ID, value=vport_attr_value)
    vport_attr_list.append(vport_attr)

    return client.sai_thrift_create_bridge_port(vport_attr_list)

def sai_thrift_create_fdb(client, mac, bridge_type, vlan_id, bridge_id, bridge_port, mac_action, fdb_entry_type):
    if bridge_type == SAI_BRIDGE_TYPE_1Q:
        fdb_entry = sai_thrift_fdb_entry_t(mac_address=mac, bridge_type=SAI_FDB_ENTRY_BRIDGE_TYPE_1Q, vlan_id=vlan_id)
    else:
        fdb_entry = sai_thrift_fdb_entry_t(mac_address=mac, bridge_type=SAI_FDB_ENTRY_BRIDGE_TYPE_1D, bridge_id=bridge_id)

    #value 0 represents static entry, id=0, represents entry type
    fdb_attribute1_value = sai_thrift_attribute_value_t(s32=fdb_entry_type)
    fdb_attribute1 = sai_thrift_attribute_t(id=SAI_FDB_ENTRY_ATTR_TYPE,
                                            value=fdb_attribute1_value)
    #value oid represents object id, id=1 represents port id
    fdb_attribute2_value = sai_thrift_attribute_value_t(oid=bridge_port)
    fdb_attribute2 = sai_thrift_attribute_t(id=SAI_FDB_ENTRY_ATTR_BRIDGE_PORT_ID,
                                            value=fdb_attribute2_value)
    #value oid represents object id, id=1 represents port id
    fdb_attribute3_value = sai_thrift_attribute_value_t(s32=mac_action)
    fdb_attribute3 = sai_thrift_attribute_t(id=SAI_FDB_ENTRY_ATTR_PACKET_ACTION,
                                            value=fdb_attribute3_value)
    fdb_attr_list = [fdb_attribute1, fdb_attribute2, fdb_attribute3]
    client.sai_thrift_create_fdb_entry(thrift_fdb_entry=fdb_entry, thrift_attr_list=fdb_attr_list)

def sai_thrift_delete_fdb(client, mac, vlan_id, bridge_type, bridge_id):
    if bridge_type == SAI_BRIDGE_TYPE_1Q:
        fdb_entry = sai_thrift_fdb_entry_t(mac_address=mac, bridge_type=SAI_FDB_ENTRY_BRIDGE_TYPE_1Q, vlan_id=vlan_id)
    else:
        fdb_entry = sai_thrift_fdb_entry_t(mac_address=mac, bridge_type=SAI_FDB_ENTRY_BRIDGE_TYPE_1D, bridge_id=bridge_id)
    client.sai_thrift_delete_fdb_entry(thrift_fdb_entry=fdb_entry)

def sai_thrift_flush_fdb_by_vlan(client, vlan_id):
    fdb_attribute1_value = sai_thrift_attribute_value_t(u16=vlan_id)
    fdb_attribute1 = sai_thrift_attribute_t(id=SAI_FDB_FLUSH_ATTR_VLAN_ID,
                                            value=fdb_attribute1_value)
    fdb_attribute2_value = sai_thrift_attribute_value_t(s32=SAI_FDB_FLUSH_ENTRY_DYNAMIC)
    fdb_attribute2 = sai_thrift_attribute_t(id=SAI_FDB_FLUSH_ATTR_ENTRY_TYPE,
                                            value=fdb_attribute2_value)
    fdb_attr_list = [fdb_attribute1, fdb_attribute2]
    client.sai_thrift_flush_fdb_entries(thrift_attr_list=fdb_attr_list)

def sai_thrift_create_virtual_router(client, v4_enabled, v6_enabled):
    #v4 enabled
    vr_attribute1_value = sai_thrift_attribute_value_t(booldata=v4_enabled)
    vr_attribute1 = sai_thrift_attribute_t(id=SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE,
                                           value=vr_attribute1_value)
    #v6 enabled
    vr_attribute2_value = sai_thrift_attribute_value_t(booldata=v6_enabled)
    vr_attribute2 = sai_thrift_attribute_t(id=SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE,
                                           value=vr_attribute2_value)
    vr_attr_list = [vr_attribute1, vr_attribute2]
    vr_id = client.sai_thrift_create_virtual_router(thrift_attr_list=vr_attr_list)
    return vr_id

def sai_thrift_create_router_interface(client, vr_id, is_port, port_id, vlan_id, v4_enabled, v6_enabled, mac):
    #vrf attribute
    rif_attr_list = []
    rif_attribute1_value = sai_thrift_attribute_value_t(oid=vr_id)
    rif_attribute1 = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                            value=rif_attribute1_value)
    rif_attr_list.append(rif_attribute1)
    if is_port:
        #port type and port id
        rif_attribute2_value = sai_thrift_attribute_value_t(s32=SAI_ROUTER_INTERFACE_TYPE_PORT)
        rif_attribute2 = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                                value=rif_attribute2_value)
        rif_attr_list.append(rif_attribute2)
        rif_attribute3_value = sai_thrift_attribute_value_t(oid=port_id)
        rif_attribute3 = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                                value=rif_attribute3_value)
        rif_attr_list.append(rif_attribute3)
    else:
        #vlan type and vlan id
        rif_attribute2_value = sai_thrift_attribute_value_t(s32=SAI_ROUTER_INTERFACE_TYPE_VLAN)
        rif_attribute2 = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                                value=rif_attribute2_value)
        rif_attr_list.append(rif_attribute2)
        rif_attribute3_value = sai_thrift_attribute_value_t(u16=vlan_id)
        rif_attribute3 = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                                value=rif_attribute3_value)
        rif_attr_list.append(rif_attribute3)

    #v4_enabled
    rif_attribute4_value = sai_thrift_attribute_value_t(booldata=v4_enabled)
    rif_attribute4 = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE,
                                            value=rif_attribute4_value)
    rif_attr_list.append(rif_attribute4)
    #v6_enabled
    rif_attribute5_value = sai_thrift_attribute_value_t(booldata=v6_enabled)
    rif_attribute5 = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE,
                                            value=rif_attribute5_value)
    rif_attr_list.append(rif_attribute5)

    if mac:
        rif_attribute6_value = sai_thrift_attribute_value_t(mac=mac)
        rif_attribute6 = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                                                value=rif_attribute6_value)
        rif_attr_list.append(rif_attribute6)

    rif_id = client.sai_thrift_create_router_interface(rif_attr_list)
    return rif_id

def sai_thrift_create_route(client, vr_id, addr_family, ip_addr, ip_mask, nhop, packet_action=None):
    if addr_family == SAI_IP_ADDR_FAMILY_IPV4:
        addr = sai_thrift_ip_t(ip4=ip_addr)
        mask = sai_thrift_ip_t(ip4=ip_mask)
        ip_prefix = sai_thrift_ip_prefix_t(addr_family=SAI_IP_ADDR_FAMILY_IPV4, addr=addr, mask=mask)
    else:
        addr = sai_thrift_ip_t(ip6=ip_addr)
        mask = sai_thrift_ip_t(ip6=ip_mask)
        ip_prefix = sai_thrift_ip_prefix_t(addr_family=SAI_IP_ADDR_FAMILY_IPV6, addr=addr, mask=mask)
    route_attribute1_value = sai_thrift_attribute_value_t(oid=nhop)
    route_attribute1 = sai_thrift_attribute_t(id=SAI_ROUTE_ATTR_NEXT_HOP_ID,
                                              value=route_attribute1_value)

    route = sai_thrift_unicast_route_entry_t(vr_id, ip_prefix)
    route_attr_list = [route_attribute1]

    if packet_action != None:
        route_packet_action_value = sai_thrift_attribute_value_t(s32=packet_action)
        route_packet_action_attr = sai_thrift_attribute_t(id=SAI_ROUTE_ATTR_PACKET_ACTION, value=route_packet_action_value)
        route_attr_list.append(route_packet_action_attr)

    client.sai_thrift_create_route(thrift_unicast_route_entry=route, thrift_attr_list=route_attr_list)
    return

def sai_thrift_remove_route(client, vr_id, addr_family, ip_addr, ip_mask, nhop):
    if addr_family == SAI_IP_ADDR_FAMILY_IPV4:
        addr = sai_thrift_ip_t(ip4=ip_addr)
        mask = sai_thrift_ip_t(ip4=ip_mask)
        ip_prefix = sai_thrift_ip_prefix_t(addr_family=SAI_IP_ADDR_FAMILY_IPV4, addr=addr, mask=mask)
    else:
        addr = sai_thrift_ip_t(ip6=ip_addr)
        mask = sai_thrift_ip_t(ip6=ip_mask)
        ip_prefix = sai_thrift_ip_prefix_t(addr_family=SAI_IP_ADDR_FAMILY_IPV6, addr=addr, mask=mask)
    route = sai_thrift_unicast_route_entry_t(vr_id, ip_prefix)
    client.sai_thrift_remove_route(thrift_unicast_route_entry=route)

def sai_thrift_create_nhop(client, addr_family, ip_addr, rif_id):
    if addr_family == SAI_IP_ADDR_FAMILY_IPV4:
        addr = sai_thrift_ip_t(ip4=ip_addr)
        ipaddr = sai_thrift_ip_address_t(addr_family=SAI_IP_ADDR_FAMILY_IPV4, addr=addr)
    else:
        addr = sai_thrift_ip_t(ip6=ip_addr)
        ipaddr = sai_thrift_ip_address_t(addr_family=SAI_IP_ADDR_FAMILY_IPV6, addr=addr)
    nhop_attribute1_value = sai_thrift_attribute_value_t(ipaddr=ipaddr)
    nhop_attribute1 = sai_thrift_attribute_t(id=SAI_NEXT_HOP_ATTR_IP,
                                             value=nhop_attribute1_value)
    nhop_attribute2_value = sai_thrift_attribute_value_t(oid=rif_id)
    nhop_attribute2 = sai_thrift_attribute_t(id=SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                             value=nhop_attribute2_value)
    nhop_attribute3_value = sai_thrift_attribute_value_t(s32=SAI_NEXT_HOP_IP)
    nhop_attribute3 = sai_thrift_attribute_t(id=SAI_NEXT_HOP_ATTR_TYPE,
                                             value=nhop_attribute3_value)
    nhop_attr_list = [nhop_attribute1, nhop_attribute2, nhop_attribute3]
    nhop = client.sai_thrift_create_next_hop(thrift_attr_list=nhop_attr_list)
    return nhop

def sai_thrift_create_neighbor(client, addr_family, rif_id, ip_addr, dmac):
    if addr_family == SAI_IP_ADDR_FAMILY_IPV4:
        addr = sai_thrift_ip_t(ip4=ip_addr)
        ipaddr = sai_thrift_ip_address_t(addr_family=SAI_IP_ADDR_FAMILY_IPV4, addr=addr)
    else:
        addr = sai_thrift_ip_t(ip6=ip_addr)
        ipaddr = sai_thrift_ip_address_t(addr_family=SAI_IP_ADDR_FAMILY_IPV6, addr=addr)
    neighbor_attribute1_value = sai_thrift_attribute_value_t(mac=dmac)
    neighbor_attribute1 = sai_thrift_attribute_t(id=SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                                 value=neighbor_attribute1_value)
    neighbor_attr_list = [neighbor_attribute1]
    neighbor_entry = sai_thrift_neighbor_entry_t(rif_id=rif_id, ip_address=ipaddr)
    client.sai_thrift_create_neighbor_entry(neighbor_entry, neighbor_attr_list)

def sai_thrift_remove_neighbor(client, addr_family, rif_id, ip_addr, dmac):
    if addr_family == SAI_IP_ADDR_FAMILY_IPV4:
        addr = sai_thrift_ip_t(ip4=ip_addr)
        ipaddr = sai_thrift_ip_address_t(addr_family=SAI_IP_ADDR_FAMILY_IPV4, addr=addr)
    else:
        addr = sai_thrift_ip_t(ip6=ip_addr)
        ipaddr = sai_thrift_ip_address_t(addr_family=SAI_IP_ADDR_FAMILY_IPV6, addr=addr)
    neighbor_entry = sai_thrift_neighbor_entry_t(rif_id=rif_id, ip_address=ipaddr)
    client.sai_thrift_remove_neighbor_entry(neighbor_entry)

def sai_thrift_create_next_hop_group(client, nhop_list):
    nhop_group_attribute1_value = sai_thrift_attribute_value_t(s32=SAI_NEXT_HOP_GROUP_ECMP)
    nhop_group_attribute1 = sai_thrift_attribute_t(id=SAI_NEXT_HOP_GROUP_ATTR_TYPE,
                                                   value=nhop_group_attribute1_value)
    nhop_objlist = sai_thrift_object_list_t(count=len(nhop_list), object_id_list=nhop_list)
    nhop_group_attribute2_value = sai_thrift_attribute_value_t(objlist=nhop_objlist)
    nhop_group_attribute2 = sai_thrift_attribute_t(id=SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST,
                                                   value=nhop_group_attribute2_value)
    nhop_group_attr_list = [nhop_group_attribute1, nhop_group_attribute2]
    nhop_group = client.sai_thrift_create_next_hop_group(thrift_attr_list=nhop_group_attr_list)
    return nhop_group

def sai_thrift_create_lag(client, port_list):
    lag_port_list = sai_thrift_object_list_t(count=len(port_list), object_id_list=port_list)
    lag1_attr_value = sai_thrift_attribute_value_t(objlist=lag_port_list)
    lag1_attr = sai_thrift_attribute_t(id=SAI_LAG_ATTR_PORT_LIST,
                                       value=lag1_attr_value)
    lag_attr_list = [lag1_attr]
    lag = client.sai_thrift_create_lag(lag_attr_list)
    return lag

def sai_thrift_create_lag_member(client, lag_id, port_id):
    lag_member_attr1_value = sai_thrift_attribute_value_t(oid=lag_id)
    lag_member_attr1 = sai_thrift_attribute_t(id=SAI_LAG_MEMBER_ATTR_LAG_ID,
                                              value=lag_member_attr1_value)
    lag_member_attr2_value = sai_thrift_attribute_value_t(oid=port_id)
    lag_member_attr2 = sai_thrift_attribute_t(id=SAI_LAG_MEMBER_ATTR_PORT_ID,
                                              value=lag_member_attr2_value)
    lag_member_attr_list = [lag_member_attr1, lag_member_attr2]
    lag_member_id = client.sai_thrift_create_lag_member(lag_member_attr_list)
    return lag_member_id

def sai_thrift_create_stp_entry(client, vlan_list):
    vlanlist=sai_thrift_vlan_list_t(vlan_count=len(vlan_list), vlan_list=vlan_list)
    stp_attribute1_value = sai_thrift_attribute_value_t(vlanlist=vlanlist)
    stp_attribute1 = sai_thrift_attribute_t(id=SAI_STP_ATTR_VLAN_LIST,
                                            value=stp_attribute1_value)
    stp_attr_list = [stp_attribute1]
    stp_id = client.sai_thrift_create_stp_entry(stp_attr_list)
    return stp_id


def sai_thrift_set_hostif_trap_group(client, trap_group_id, policer_id):
    policer_attr_value = sai_thrift_attribute_value_t(oid=policer_id)
    policer_attr = sai_thrift_attribute_t(id=SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER, value=policer_attr_value)
    status = client.sai_thrift_set_hostif_trap_group(trap_group_id, thrift_attr=policer_attr)
    return status

def sai_thrift_create_hostif_trap_group(client, queue_id, policer_id=None):
    attr_list = []
    attribute_value = sai_thrift_attribute_value_t(u32=queue_id)
    attribute = sai_thrift_attribute_t(id=SAI_HOSTIF_TRAP_GROUP_ATTR_QUEUE, value=attribute_value)
    attr_list.append(attribute)

    if policer_id != None:
        policer_attr_value = sai_thrift_attribute_value_t(oid=policer_id)
        policer_attr = sai_thrift_attribute_t(id=SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER, value=policer_attr_value)
        attr_list.append(policer_attr)

    trap_group_id = client.sai_thrift_create_hostif_trap_group(thrift_attr_list=attr_list)
    return trap_group_id

def sai_thrift_create_port(client, bind_mode, hw_port, vlan_id=None):
    attr_list = []
    bind_attr_value = sai_thrift_attribute_value_t(s32=bind_mode)
    bind_attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_BIND_MODE, value=bind_attr_value)
    attr_list.append(bind_attr)
    hw_port_list = sai_thrift_u32_list_t(u32list=[hw_port], count=1)
    hw_lane_attr_value = sai_thrift_attribute_value_t(u32list=hw_port_list)
    hw_lane_attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_HW_LANE_LIST, value=hw_lane_attr_value)
    attr_list.append(hw_lane_attr)
    if vlan_id:
        vlan_attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        vlan_attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=vlan_attr_value)
        attr_list.append(vlan_attr)
    return client.sai_thrift_create_port(thrift_attr_list=attr_list)

def sai_thrift_create_lag_member(client, port_id, lag_id):
    attr_list = []
    port_attr_value = sai_thrift_attribute_value_t(oid=port_id)
    port_attr = sai_thrift_attribute_t(id=SAI_LAG_MEMBER_ATTR_PORT_ID, value=port_attr_value)
    attr_list.append(port_attr)
    lag_attr_value = sai_thrift_attribute_value_t(oid=lag_id)
    lag_attr = sai_thrift_attribute_t(id=SAI_LAG_MEMBER_ATTR_LAG_ID, value=lag_attr_value)
    attr_list.append(lag_attr)
    return client.sai_thrift_create_lag_member(thrift_attr_list=attr_list)

def sai_thrift_create_policer(client, meter_type, mode, cir, red_action):
    attr_list = []

    meter_attr_value = sai_thrift_attribute_value_t(s32=meter_type)
    meter_attr = sai_thrift_attribute_t(id=SAI_POLICER_ATTR_METER_TYPE, value=meter_attr_value)

    mode_attr_value = sai_thrift_attribute_value_t(s32=mode)
    mode_attr = sai_thrift_attribute_t(id=SAI_POLICER_ATTR_MODE, value=mode_attr_value)

    cir_attr_value = sai_thrift_attribute_value_t(u64=cir)
    cir_attr = sai_thrift_attribute_t(id=SAI_POLICER_ATTR_CIR, value=cir_attr_value)

    red_action_attr_val = sai_thrift_attribute_value_t(s32=red_action)
    red_action_attr = sai_thrift_attribute_t(id=SAI_POLICER_ATTR_RED_PACKET_ACTION, value=red_action_attr_val)

    attr_list.append(meter_attr)
    attr_list.append(mode_attr)
    attr_list.append(cir_attr)
    attr_list.append(red_action_attr)
    policer_id = client.sai_thrift_create_policer(attr_list)

    return policer_id

def sai_thrift_set_hostif_trap(client, trap_id, action, priority=None, channel=None, trap_group_id=None):

    if channel != None:
        attribute3_value = sai_thrift_attribute_value_t(s32=channel)
        attribute3 = sai_thrift_attribute_t(id=SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL, value=attribute3_value)
        client.sai_thrift_set_hostif_trap(trap_id, attribute3)

    if trap_group_id != None:
        attribute4_value = sai_thrift_attribute_value_t(oid=trap_group_id)
        attribute4 = sai_thrift_attribute_t(id=SAI_HOSTIF_TRAP_ATTR_TRAP_GROUP, value=attribute4_value)
        client.sai_thrift_set_hostif_trap(trap_id, attribute4)

    attribute1_value = sai_thrift_attribute_value_t(s32=action)
    attribute1 = sai_thrift_attribute_t(id=SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION, value=attribute1_value)
    client.sai_thrift_set_hostif_trap(trap_id, attribute1)

    if priority != None:
        attribute2_value = sai_thrift_attribute_value_t(u32=priority)
        attribute2 = sai_thrift_attribute_t(id=SAI_HOSTIF_TRAP_ATTR_TRAP_PRIORITY, value=attribute2_value)
        client.sai_thrift_set_hostif_trap(trap_id, attribute2)

def sai_thrift_create_hostif(client, rif_or_port_id, intf_name):
    attribute1_value = sai_thrift_attribute_value_t(s32=SAI_HOSTIF_TYPE_NETDEV)
    attribute1 = sai_thrift_attribute_t(id=SAI_HOSTIF_ATTR_TYPE,
                                        value=attribute1_value)
    attribute2_value = sai_thrift_attribute_value_t(oid=rif_or_port_id)
    attribute2 = sai_thrift_attribute_t(id=SAI_HOSTIF_ATTR_RIF_OR_PORT_ID,
                                        value=attribute2_value)
    attribute3_value = sai_thrift_attribute_value_t(chardata=intf_name)
    attribute3 = sai_thrift_attribute_t(id=SAI_HOSTIF_ATTR_NAME,
                                        value=attribute3_value)
    attr_list = [attribute1, attribute2, attribute3]
    hif_id = client.sai_thrift_create_hostif(attr_list)
    return hif_id

def sai_thrift_create_acl_table(client, addr_family,
                                ip_src, ip_dst,
                                ip_proto,
                                in_ports, out_ports,
                                in_port, out_port):
    #print "aaa"
    acl_attr_list = []
    if ip_src != None:
        attribute_value = sai_thrift_attribute_value_t(booldata=1)
        attribute = sai_thrift_attribute_t(id=SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                                           value=attribute_value)
        acl_attr_list.append(attribute)
    if ip_dst != None:
        attribute_value = sai_thrift_attribute_value_t(booldata=1)
        attribute = sai_thrift_attribute_t(id=SAI_ACL_TABLE_ATTR_FIELD_DST_IP,
                                           value=attribute_value)
        acl_attr_list.append(attribute)
    if ip_proto != None:
        attribute_value = sai_thrift_attribute_value_t(booldata=1)
        attribute = sai_thrift_attribute_t(id=SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL,
                                           value=attribute_value)
        acl_attr_list.append(attribute)
    if in_ports:
        attribute_value = sai_thrift_attribute_value_t(booldata=1)
        attribute = sai_thrift_attribute_t(id=SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS,
                                           value=attribute_value)
        acl_attr_list.append(attribute)
    if out_ports:
        attribute_value = sai_thrift_attribute_value_t(booldata=1)
        attribute = sai_thrift_attribute_t(id=SAI_ACL_TABLE_ATTR_FIELD_OUT_PORTS,
                                           value=attribute_value)
        acl_attr_list.append(attribute)
    if in_port != None:
        attribute_value = sai_thrift_attribute_value_t(booldata=1)
        attribute = sai_thrift_attribute_t(id=SAI_ACL_TABLE_ATTR_FIELD_IN_PORT,
                                           value=attribute_value)
        acl_attr_list.append(attribute)
    if out_port != None:
        attribute_value = sai_thrift_attribute_value_t(booldata=1)
        attribute = sai_thrift_attribute_t(id=SAI_ACL_TABLE_ATTR_FIELD_OUT_PORT,
                                           value=attribute_value)
        acl_attr_list.append(attribute)

    attribute_value = sai_thrift_attribute_value_t(u32=0)   #TODO: Expose stage as function parameter
    attribute = sai_thrift_attribute_t(id=SAI_ACL_TABLE_ATTR_STAGE, value=attribute_value)
    acl_attr_list.append(attribute)
    
    #print "bbb"    
    acl_table_id = client.sai_thrift_create_acl_table(acl_attr_list)
    #print acl_table_id
    return acl_table_id

def sai_thrift_create_acl_entry(client, acl_table_id,
                                action, addr_family,
                                ip_src, ip_src_mask,
                                ip_dst, ip_dst_mask,
                                ip_proto,
                                in_port_list, out_port_list,
                                in_port, out_port,
                                ingress_mirror, egress_mirror):
    acl_attr_list = []

    #OID
    attribute_value = sai_thrift_attribute_value_t(oid=acl_table_id)
    attribute = sai_thrift_attribute_t(id=SAI_ACL_ENTRY_ATTR_TABLE_ID,
                                       value=attribute_value)
    acl_attr_list.append(attribute)

    #Priority
    attribute_value = sai_thrift_attribute_value_t(aclfield=sai_thrift_acl_field_data_t(data = sai_thrift_acl_data_t(u32=10)))
    attribute = sai_thrift_attribute_t(id=SAI_ACL_ENTRY_ATTR_PRIORITY,
                                       value=attribute_value)
    acl_attr_list.append(attribute)

    #Ip source
    if ip_src != None:
        attribute_value = sai_thrift_attribute_value_t(aclfield=sai_thrift_acl_field_data_t(data = sai_thrift_acl_data_t(ip4=ip_src), mask =sai_thrift_acl_mask_t(ip4=ip_src_mask)))
        attribute = sai_thrift_attribute_t(id=SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP,
                                           value=attribute_value)
        acl_attr_list.append(attribute)

    #Input ports
    if in_port_list:
        acl_port_list = sai_thrift_object_list_t(count=len(in_port_list), object_id_list=in_port_list)
        attribute_value = sai_thrift_attribute_value_t(aclfield=sai_thrift_acl_field_data_t(data = sai_thrift_acl_data_t(objlist=acl_port_list)))
        attribute = sai_thrift_attribute_t(id=SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS,
                                           value=attribute_value)
        acl_attr_list.append(attribute)

    #Output ports
    if out_port_list:
        acl_port_list = sai_thrift_object_list_t(count=len(out_port_list), object_id_list=out_port_list)
        attribute_value = sai_thrift_attribute_value_t(aclfield=sai_thrift_acl_field_data_t(data = sai_thrift_acl_data_t(objlist=acl_port_list)))
        attribute = sai_thrift_attribute_t(id=SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORTS,
                                           value=attribute_value)
        acl_attr_list.append(attribute)

    if in_port != None:
        attribute_value = sai_thrift_attribute_value_t(aclfield=sai_thrift_acl_field_data_t(data = sai_thrift_acl_data_t(oid=in_port)))
        attribute = sai_thrift_attribute_t(id=SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT,
                                           value=attribute_value)
        acl_attr_list.append(attribute)

    if out_port != None:
        attribute_value = sai_thrift_attribute_value_t(aclfield=sai_thrift_acl_field_data_t(data = sai_thrift_acl_data_t(oid=out_port)))
        attribute = sai_thrift_attribute_t(id=SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT,
                                           value=attribute_value)
        acl_attr_list.append(attribute)

    #Packet action
    if action == 1:
        #Drop
        attribute_value = sai_thrift_attribute_value_t(aclaction=sai_thrift_acl_action_data_t(parameter = sai_thrift_acl_data_t(u32=0)))
        attribute = sai_thrift_attribute_t(id=SAI_ACL_ENTRY_ATTR_PACKET_ACTION,
                                           value=attribute_value)
        acl_attr_list.append(attribute)
    elif action == 2:
        #Ingress mirroring
        if ingress_mirror != None:
            attribute_value = sai_thrift_attribute_value_t(aclaction=sai_thrift_acl_action_data_t(parameter = sai_thrift_acl_data_t(oid=ingress_mirror)))
            attribute = sai_thrift_attribute_t(id=SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS, value=attribute_value)
            acl_attr_list.append(attribute)
        elif egress_mirror != None:
            attribute_value = sai_thrift_attribute_value_t(aclaction=sai_thrift_acl_action_data_t(parameter = sai_thrift_acl_data_t(oid=egress_mirror)))
            attribute = sai_thrift_attribute_t(id=SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS, value=attribute_value)
            acl_attr_list.append(attribute)

    acl_entry_id = client.sai_thrift_create_acl_entry(acl_attr_list)
    return acl_entry_id

def sai_thrift_create_mirror_session(client, mirror_type, port,
                                     vlan, vlan_priority, vlan_tpid,
                                     src_mac, dst_mac,
                                     addr_family, src_ip, dst_ip,
                                     encap_type, protocol, ttl, tos):
    mirror_attr_list = []

    #Mirror type
    attribute1_value = sai_thrift_attribute_value_t(u8=mirror_type)
    attribute1 = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_TYPE,
                                        value=attribute1_value)
    mirror_attr_list.append(attribute1)

    #Monitor port
    attribute2_value = sai_thrift_attribute_value_t(oid=port)
    attribute2 = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_MONITOR_PORT,
                                        value=attribute2_value)
    mirror_attr_list.append(attribute2)

    if mirror_type == SAI_MIRROR_TYPE_LOCAL:
        attribute4_value = sai_thrift_attribute_value_t(u16=vlan)
        attribute4 = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_VLAN_ID,
                                            value=attribute4_value)
        mirror_attr_list.append(attribute4)
    elif mirror_type == SAI_MIRROR_TYPE_REMOTE:
        #vlan tpid
        attribute3_value = sai_thrift_attribute_value_t(u16=vlan_tpid)
        attribute3 = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_VLAN_TPID,
                                            value=attribute3_value)
        mirror_attr_list.append(attribute3)

        #vlan
        attribute4_value = sai_thrift_attribute_value_t(u16=vlan)
        attribute4 = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_VLAN_ID,
                                            value=attribute4_value)
        mirror_attr_list.append(attribute4)

        #vlan priority
        attribute5_value = sai_thrift_attribute_value_t(u16=vlan_priority)
        attribute4 = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_VLAN_PRI,
                                            value=attribute5_value)
        mirror_attr_list.append(attribute5)
    elif mirror_type == SAI_MIRROR_TYPE_ENHANCED_REMOTE:
        #encap type
        attribute3_value = sai_thrift_attribute_value_t(u8=encap_type)
        attribute3 = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_ENCAP_TYPE,
                                            value=attribute3_value)
        mirror_attr_list.append(attribute3)

        #source ip
        addr = sai_thrift_ip_t(ip4=src_ip)
        src_ip_addr = sai_thrift_ip_address_t(addr_family=addr_family, addr=addr)
        attribute4_value = sai_thrift_attribute_value_t(ipaddr=src_ip_addr)
        attribute4 = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_SRC_IP_ADDRESS,
                                            value=attribute4_value)
        mirror_attr_list.append(attribute4)

        #dst ip
        addr = sai_thrift_ip_t(ip4=dst_ip)
        dst_ip_addr = sai_thrift_ip_address_t(addr_family=addr_family, addr=addr)
        attribute5_value = sai_thrift_attribute_value_t(ipaddr=dst_ip_addr)
        attribute5 = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_DST_IP_ADDRESS,
                                            value=attribute5_value)
        mirror_attr_list.append(attribute5)

        #source mac
        attribute6_value = sai_thrift_attribute_value_t(mac=src_mac)
        attribute6 = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_SRC_MAC_ADDRESS,
                                            value=attribute6_value)
        mirror_attr_list.append(attribute6)

        #dst mac
        attribute7_value = sai_thrift_attribute_value_t(mac=dst_mac)
        attribute7 = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_DST_MAC_ADDRESS,
                                            value=attribute7_value)
        mirror_attr_list.append(attribute7)

    mirror_id = client.sai_thrift_create_mirror_session(mirror_attr_list)
    return mirror_id

def sai_thrift_create_scheduler_profile(client, max_rate, algorithm=0):
    scheduler_attr_list = []
    attribute_value = sai_thrift_attribute_value_t(u64=max_rate)
    attribute = sai_thrift_attribute_t(id=SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE ,
                                       value=attribute_value)
    scheduler_attr_list.append(attribute)
    attribute_value = sai_thrift_attribute_value_t(s32=algorithm)
    attribute = sai_thrift_attribute_t(id=SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM ,
                                       value=attribute_value)
    scheduler_attr_list.append(attribute)
    scheduler_profile_id = client.sai_thrift_create_scheduler_profile(scheduler_attr_list)
    return scheduler_profile_id

def sai_thrift_create_buffer_profile(client, pool_id, size, threshold, xoff_th, xon_th):
    buffer_attr_list = []
    attribute_value = sai_thrift_attribute_value_t(oid=pool_id)
    attribute = sai_thrift_attribute_t(id=SAI_BUFFER_PROFILE_ATTR_POOL_ID ,
                                           value=attribute_value)
    buffer_attr_list.append(attribute)

    attribute_value = sai_thrift_attribute_value_t(u32=size)
    attribute = sai_thrift_attribute_t(id=SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE ,
                                           value=attribute_value)
    buffer_attr_list.append(attribute)

    attribute_value = sai_thrift_attribute_value_t(u8=threshold)
    attribute = sai_thrift_attribute_t(id=SAI_BUFFER_PROFILE_ATTR_SHARED_DYNAMIC_TH ,
                                           value=attribute_value)
    buffer_attr_list.append(attribute)

    attribute_value = sai_thrift_attribute_value_t(u32=xoff_th)
    attribute = sai_thrift_attribute_t(id=SAI_BUFFER_PROFILE_ATTR_XOFF_TH ,
                                           value=attribute_value)
    buffer_attr_list.append(attribute)

    attribute_value = sai_thrift_attribute_value_t(u32=xon_th)
    attribute = sai_thrift_attribute_t(id=SAI_BUFFER_PROFILE_ATTR_XON_TH ,
                                           value=attribute_value)
    buffer_attr_list.append(attribute)

    buffer_profile_id = client.sai_thrift_create_buffer_profile(buffer_attr_list)
    return buffer_profile_id

def sai_thrift_create_pool_profile(client, pool_type, size, threshold_mode):
    pool_attr_list = []
    attribute_value = sai_thrift_attribute_value_t(s32=pool_type)
    attribute = sai_thrift_attribute_t(id=SAI_BUFFER_POOL_ATTR_TYPE ,
                                           value=attribute_value)
    pool_attr_list.append(attribute)

    attribute_value = sai_thrift_attribute_value_t(u32=size)
    attribute = sai_thrift_attribute_t(id=SAI_BUFFER_POOL_ATTR_SIZE ,
                                           value=attribute_value)
    pool_attr_list.append(attribute)

    attribute_value = sai_thrift_attribute_value_t(s32=threshold_mode)
    attribute = sai_thrift_attribute_t(id=SAI_BUFFER_POOL_ATTR_TH_MODE ,
                                           value=attribute_value)
    pool_attr_list.append(attribute)
    pool_id = client.sai_thrift_create_pool_profile(pool_attr_list)
    return pool_id

def sai_thrift_clear_all_counters(client):
    for port in sai_port_list:
        queue_list=[]
        client.sai_thrift_clear_port_all_stats(port)
        port_attr_list = client.sai_thrift_get_port_attribute(port)
        attr_list = port_attr_list.attr_list
        for attribute in attr_list:
            if attribute.id == SAI_PORT_ATTR_QOS_QUEUE_LIST:
                for queue_id in attribute.value.objlist.object_id_list:
                    queue_list.append(queue_id)

        cnt_ids=[]
        cnt_ids.append(SAI_QUEUE_STAT_PACKETS)
        for queue in queue_list:
            client.sai_thrift_clear_queue_stats(queue,cnt_ids,len(cnt_ids))

def sai_thrift_read_port_counters(client,port):
    port_cnt_ids=[]
    port_cnt_ids.append(SAI_PORT_STAT_IF_OUT_DISCARDS)
    port_cnt_ids.append(SAI_PORT_STAT_ETHER_STATS_DROP_EVENTS)
    port_cnt_ids.append(SAI_PORT_STAT_PFC_0_TX_PKTS)
    port_cnt_ids.append(SAI_PORT_STAT_PFC_1_TX_PKTS)
    port_cnt_ids.append(SAI_PORT_STAT_PFC_2_TX_PKTS)
    port_cnt_ids.append(SAI_PORT_STAT_PFC_3_TX_PKTS)
    port_cnt_ids.append(SAI_PORT_STAT_PFC_4_TX_PKTS)
    port_cnt_ids.append(SAI_PORT_STAT_PFC_5_TX_PKTS)
    port_cnt_ids.append(SAI_PORT_STAT_PFC_6_TX_PKTS)
    port_cnt_ids.append(SAI_PORT_STAT_PFC_7_TX_PKTS)
    port_cnt_ids.append(SAI_PORT_STAT_IF_OUT_OCTETS)
    port_cnt_ids.append(SAI_PORT_STAT_IF_OUT_UCAST_PKTS)
    port_cnt_ids.append(SAI_PORT_STAT_IF_IN_UCAST_PKTS)
    counters_results=[]
    counters_results = client.sai_thrift_get_port_stats(port,port_cnt_ids,len(port_cnt_ids))
    queue_list=[]
    port_attr_list = client.sai_thrift_get_port_attribute(port)
    attr_list = port_attr_list.attr_list
    for attribute in attr_list:
        if attribute.id == SAI_PORT_ATTR_QOS_QUEUE_LIST:
            for queue_id in attribute.value.objlist.object_id_list:
                queue_list.append(queue_id)
    cnt_ids=[]
    thrift_results=[]
    queue_counters_results=[]
    cnt_ids.append(SAI_QUEUE_STAT_PACKETS)
    queue1=0
    for queue in queue_list:
        if queue1 <= 7:
            thrift_results=client.sai_thrift_get_queue_stats(queue,cnt_ids,len(cnt_ids))
            queue_counters_results.append(thrift_results[0])
            queue1+=1
    return (counters_results, queue_counters_results)

def sai_thrift_create_vlan_member(client, vlan_oid, bridge_port, tagging_mode):
    vlan_member_attr_list = []
    attribute_value = sai_thrift_attribute_value_t(oid=vlan_oid)
    attribute = sai_thrift_attribute_t(id=SAI_VLAN_MEMBER_ATTR_VLAN_ID,
                                           value=attribute_value)
    vlan_member_attr_list.append(attribute)

    attribute_value = sai_thrift_attribute_value_t(oid=bridge_port)
    attribute = sai_thrift_attribute_t(id=SAI_VLAN_MEMBER_ATTR_BRIDGE_PORT_ID,
                                           value=attribute_value)
    vlan_member_attr_list.append(attribute)

    attribute_value = sai_thrift_attribute_value_t(s32=tagging_mode)
    attribute = sai_thrift_attribute_t(id=SAI_VLAN_MEMBER_ATTR_VLAN_TAGGING_MODE,
                                           value=attribute_value)
    vlan_member_attr_list.append(attribute)
    vlan_member_id = client.sai_thrift_create_vlan_member(vlan_member_attr_list)
    return vlan_member_id

def sai_thrift_vlan_remove_all_ports(client, vid):
        vlan_members_list = []

        vlan_attr_list = client.sai_thrift_get_vlan_attribute(vid)
        attr_list = vlan_attr_list.attr_list
        for attribute in attr_list:
            if attribute.id == SAI_VLAN_ATTR_MEMBER_LIST:
                for vlan_member in attribute.value.objlist.object_id_list:
                    vlan_members_list.append(vlan_member)

        for vlan_member in vlan_members_list:
            client.sai_thrift_remove_vlan_member(vlan_member)

def sai_thrift_set_port_shaper(client, port_id, max_rate):
    sched_prof_id=sai_thrift_create_scheduler_profile(client, max_rate)
    attr_value = sai_thrift_attribute_value_t(oid=sched_prof_id)
    attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID, value=attr_value)
    client.sai_thrift_set_port_attribute(port_id,attr)
