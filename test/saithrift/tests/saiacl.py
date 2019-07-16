# Copyright 2013-present Barefoot Networks, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Thrift SAI interface ACL tests
"""

from switch import *
import sai_base_test

@group('acl')
class IPAclTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print '----------------------------------------------------------------------------------------------'
        print "Sending packet ptf_intf 2 -> ptf_intf 1 (192.168.0.1 ---> 10.10.10.1 [id = 105])"

        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_mask1 = '255.255.255.255'
        dmac1 = '00:11:22:33:44:55'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=router_mac,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)
        exp_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=router_mac,
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=63)
        try:
            print '#### NO ACL Applied ####'
            print '#### Sending  ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 2'
            send_packet(self, 1, str(pkt))
            print '#### Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 1'
            verify_packets(self, exp_pkt, [0])
        finally:
            print '----------------------------------------------------------------------------------------------'

        print "Sending packet ptf_intf 2 -[acl]-> ptf_intf 1 (192.168.0.1 -[acl]-> 10.10.10.1 [id = 105])"
        # setup ACL to block based on Source IP
        table_stage = SAI_ACL_STAGE_INGRESS
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        entry_priority = SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY
        action = SAI_PACKET_ACTION_DROP
        in_ports = [port1, port2]
        mac_src = None
        mac_dst = None
        mac_src_mask = None
        mac_dst_mask = "ff:ff:ff:ff:ff:ff"
        ip_src = "192.168.0.1"
        ip_src_mask = "255.255.255.0"
        ip_dst = None
        ip_dst_mask = None
        ip_proto = None
        in_port = None
        out_port = None
        out_ports = None
        src_l4_port = None
        dst_l4_port = None
        ingress_mirror_id = None
        egress_mirror_id = None

        acl_table_id = sai_thrift_create_acl_table(self.client,
            table_stage,
            table_bind_point_list,
            addr_family,
            mac_src,
            mac_dst,
            ip_src,
            ip_dst,
            ip_proto,
            in_ports,
            out_ports,
            in_port,
            out_port,
            src_l4_port,
            dst_l4_port)
        acl_entry_id = sai_thrift_create_acl_entry(self.client,
            acl_table_id,
            entry_priority,
            action, addr_family,
            mac_src, mac_src_mask,
            mac_dst, mac_dst_mask,
            ip_src, ip_src_mask,
            ip_dst, ip_dst_mask,
            ip_proto,
            in_ports, out_ports,
            in_port, out_port,
            src_l4_port, dst_l4_port,
            ingress_mirror_id,
            egress_mirror_id)

        # bind this ACL table to port2s object id
        attr_value = sai_thrift_attribute_value_t(oid=acl_table_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        try:
            assert acl_table_id > 0, 'acl_entry_id is <= 0'
            assert acl_entry_id > 0, 'acl_entry_id is <= 0'

            print '#### ACL \'DROP, src 192.168.0.1/255.255.255.0, in_ports[ptf_intf_1,2]\' Applied ####'
            print '#### Sending      ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 2'
            # send the same packet
            send_packet(self, 1, str(pkt))
            # ensure packet is dropped
            # check for absence of packet here!
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 1'
            verify_no_packet(self, exp_pkt, 0)
        finally:
            # unbind this ACL table from port2s object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            # cleanup ACL
            self.client.sai_thrift_remove_acl_entry(acl_entry_id)
            self.client.sai_thrift_remove_acl_table(acl_table_id)
            # cleanup
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('acl')
class MACSrcAclTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print '----------------------------------------------------------------------------------------------'
        print "Sending packet ptf_intf 2 -> ptf_intf 1 (192.168.0.1 ---> 10.10.10.1 [id = 105])"

        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_mask1 = '255.255.255.255'
        dmac1 = '00:11:22:33:44:55'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=router_mac,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)
        exp_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=router_mac,
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=63)
        try:
            print '#### NO ACL Applied ####'
            print '#### Sending  ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 2'
            send_packet(self, 1, str(pkt))
            print '#### Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 1'
            verify_packets(self, exp_pkt, [0])
        finally:
            print '----------------------------------------------------------------------------------------------'

        print "Sending packet ptf_intf 2 -[acl]-> ptf_intf 1 (192.168.0.1 -[acl]-> 10.10.10.1 [id = 105])"
        # setup ACL to block based on Source MAC
        table_stage = SAI_ACL_STAGE_INGRESS
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        entry_priority = 1
        action = SAI_PACKET_ACTION_DROP
        in_ports = [port1, port2]
        mac_src = '00:22:22:22:22:22'
        mac_dst = None
        mac_src_mask = 'ff:ff:ff:ff:ff:ff'
        mac_dst_mask = None
        ip_src = None
        ip_src_mask = None
        ip_dst = None
        ip_dst_mask = None
        ip_proto = None
        in_port = None
        out_port = None
        out_ports = None
        src_l4_port = None
        dst_l4_port = None
        ingress_mirror_id = None
        egress_mirror_id = None

        acl_table_id = sai_thrift_create_acl_table(self.client,
            table_stage,
            table_bind_point_list,
            addr_family,
            mac_src,
            mac_dst,
            ip_src,
            ip_dst,
            ip_proto,
            in_ports,
            out_ports,
            in_port,
            out_port,
            src_l4_port,
            dst_l4_port)
        acl_entry_id = sai_thrift_create_acl_entry(self.client,
            acl_table_id,
            entry_priority,
            action, addr_family,
            mac_src, mac_src_mask,
            mac_dst, mac_dst_mask,
            ip_src, ip_src_mask,
            ip_dst, ip_dst_mask,
            ip_proto,
            in_ports, out_ports,
            in_port, out_port,
            src_l4_port, dst_l4_port,
            ingress_mirror_id,
            egress_mirror_id)

        # bind this ACL table to port2s object id
        attr_value = sai_thrift_attribute_value_t(oid=acl_table_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        try:
            assert acl_table_id > 0, 'acl_entry_id is <= 0'
            assert acl_entry_id > 0, 'acl_entry_id is <= 0'

            print '#### ACL \'DROP, src mac 00:22:22:22:22:22, in_ports[ptf_intf_1,2]\' Applied ####'
            print '#### Sending      ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 2'
            # send the same packet
            send_packet(self, 1, str(pkt))
            # ensure packet is dropped
            # check for absence of packet here!
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 1'
            verify_no_packet(self, exp_pkt, 0)
        finally:
            # unbind this ACL table from port2s object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            # cleanup ACL
            self.client.sai_thrift_remove_acl_entry(acl_entry_id)
            self.client.sai_thrift_remove_acl_table(acl_table_id)
            # cleanup
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('acl')
class L3AclTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print '----------------------------------------------------------------------------------------------'
        print "Sending packet ptf_intf 2 -> ptf_intf 1 (192.168.100.100 ---> 10.10.10.1 [id = 105])"

        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        L4_SRC_PORT = 1000
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_mask1 = '255.255.255.255'
        dmac1 = '00:11:22:33:44:55'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=router_mac,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.1',
            ip_src='192.168.100.100',
            tcp_sport = L4_SRC_PORT,
            ip_id=105,
            ip_ttl=64)
        exp_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=router_mac,
            ip_dst='10.10.10.1',
            ip_src='192.168.100.100',
            tcp_sport = L4_SRC_PORT,
            ip_id=105,
            ip_ttl=63)
        try:
            print '#### NO ACL Applied ####'
            print '#### Sending  ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 2'
            send_packet(self, 1, str(pkt))
            print '#### Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1'
            verify_packets(self, exp_pkt, [0])
        finally:
            print '----------------------------------------------------------------------------------------------'

        print "Sending packet ptf_intf 2 -[acl]-> ptf_intf 1 (192.168.0.1 -[acl]-> 10.10.10.1 [id = 105])"
        # setup ACL to block based on Source IP and SPORT
        table_stage = SAI_ACL_STAGE_INGRESS
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF]
        entry_priority = 1
        action = SAI_PACKET_ACTION_DROP
        in_ports = [port1, port2]
        mac_src = None
        mac_dst = None
        mac_src_mask = None
        mac_dst_mask = None
        ip_src = "192.168.100.1"
        ip_src_mask = "255.255.255.0"
        ip_dst = None
        ip_dst_mask = None
        ip_proto = None
        in_port = None
        out_port = None
        out_ports = None
        src_l4_port = L4_SRC_PORT
        dst_l4_port = None
        ingress_mirror_id = None
        egress_mirror_id = None

        acl_table_id = sai_thrift_create_acl_table(self.client,
            table_stage,
            table_bind_point_list,
            addr_family,
            mac_src,
            mac_dst,
            ip_src,
            ip_dst,
            ip_proto,
            in_ports,
            out_ports,
            in_port,
            out_port,
            src_l4_port,
            dst_l4_port)
        acl_entry_id = sai_thrift_create_acl_entry(self.client,
            acl_table_id,
            entry_priority,
            action, addr_family,
            mac_src, mac_src_mask,
            mac_dst, mac_dst_mask,
            ip_src, ip_src_mask,
            ip_dst, ip_dst_mask,
            ip_proto,
            in_ports, out_ports,
            in_port, out_port,
            src_l4_port, dst_l4_port,
            ingress_mirror_id,
            egress_mirror_id)

        # bind this ACL table to rif_id2s object id
        attr_value = sai_thrift_attribute_value_t(oid=acl_table_id)
        attr = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_INGRESS_ACL, value=attr_value)
        self.client.sai_thrift_set_router_interface_attribute(rif_id2, attr)

        try:
            assert acl_table_id > 0, 'acl_entry_id is <= 0'
            assert acl_entry_id > 0, 'acl_entry_id is <= 0'

            print '#### ACL \'DROP, src ip 192.168.100.1/255.255.255.0, SPORT 1000, in_ports[ptf_intf_1,2]\' Applied ####'
            print '#### Sending      ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1'
            # send the same packet
            send_packet(self, 1, str(pkt))
            # ensure packet is dropped
            # check for absence of packet here!
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 0'
            verify_no_packet(self, exp_pkt, 0)
        finally:
            # unbind this ACL table from rif_id2s object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_ROUTER_INTERFACE_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_router_interface_attribute(rif_id2, attr)
            # cleanup ACL
            self.client.sai_thrift_remove_acl_entry(acl_entry_id)
            self.client.sai_thrift_remove_acl_table(acl_table_id)
            # cleanup
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('acl')
class SeqAclTableGroupTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print '----------------------------------------------------------------------------------------------'
        print "Sending packet ptf_intf 2 -> ptf_intf 1 (192.168.0.1 ---> 10.10.10.1 [id = 105])"

        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_mask1 = '255.255.255.255'
        dmac1 = '00:11:22:33:44:55'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=router_mac,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)
        exp_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=router_mac,
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=63)
        try:
            print '#### NO ACL Applied ####'
            print '#### Sending  ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 2'
            send_packet(self, 1, str(pkt))
            print '#### Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 1'
            verify_packets(self, exp_pkt, [0])
        finally:
            print '----------------------------------------------------------------------------------------------'

        print "Sending packet ptf_intf 2 -[acl]-> ptf_intf 1 (192.168.0.1 -[acl]-> 10.10.10.1 [id = 105])"

        # setup ACL table group
        group_stage = SAI_ACL_STAGE_INGRESS
        group_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        group_type = SAI_ACL_TABLE_GROUP_TYPE_SEQUENTIAL

        # create ACL table group
        acl_table_group_id = sai_thrift_create_acl_table_group(self.client,
            group_stage,
            group_bind_point_list,
            group_type)

        # setup ACL tables to block based on Source MAC
        table_stage = SAI_ACL_STAGE_INGRESS
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        entry_priority = 1
        action = SAI_PACKET_ACTION_DROP
        in_ports = [port1, port2]
        mac_src = '00:22:22:22:22:22'
        mac_dst = None
        mac_src_mask = 'ff:ff:ff:ff:ff:ff'
        mac_dst_mask = None
        ip_src = None
        ip_src_mask = None
        ip_dst = None
        ip_dst_mask = None
        ip_proto = None
        in_port = None
        out_port = None
        out_ports = None
        src_l4_port = None
        dst_l4_port = None
        ingress_mirror_id = None
        egress_mirror_id = None

        # create ACL table #1
        acl_table_id1 = sai_thrift_create_acl_table(self.client,
            table_stage,
            table_bind_point_list,
            addr_family,
            mac_src,
            mac_dst,
            ip_src,
            ip_dst,
            ip_proto,
            in_ports,
            out_ports,
            in_port,
            out_port,
            src_l4_port,
            dst_l4_port)
        acl_entry_id1 = sai_thrift_create_acl_entry(self.client,
            acl_table_id1,
            entry_priority,
            action, addr_family,
            mac_src, mac_src_mask,
            mac_dst, mac_dst_mask,
            ip_src, ip_src_mask,
            ip_dst, ip_dst_mask,
            ip_proto,
            in_ports, out_ports,
            in_port, out_port,
            src_l4_port, dst_l4_port,
            ingress_mirror_id,
            egress_mirror_id)

        # create ACL table #2
        acl_table_id2 = sai_thrift_create_acl_table(self.client,
            table_stage,
            table_bind_point_list,
            addr_family,
            mac_src,
            mac_dst,
            ip_src,
            ip_dst,
            ip_proto,
            in_ports,
            out_ports,
            in_port,
            out_port,
            src_l4_port,
            dst_l4_port)
        acl_entry_id2 = sai_thrift_create_acl_entry(self.client,
            acl_table_id2,
            entry_priority,
            action, addr_family,
            mac_src, mac_src_mask,
            mac_dst, mac_dst_mask,
            ip_src, ip_src_mask,
            ip_dst, ip_dst_mask,
            ip_proto,
            in_ports, out_ports,
            in_port, out_port,
            src_l4_port, dst_l4_port,
            ingress_mirror_id,
            egress_mirror_id)

        # setup ACL table group members
        group_member_priority1 = 1
        group_member_priority2 = 100

        # create ACL table group members
        acl_table_group_member_id1 = sai_thrift_create_acl_table_group_member(self.client,
            acl_table_group_id,
            acl_table_id1,
            group_member_priority1)
        acl_table_group_member_id2 = sai_thrift_create_acl_table_group_member(self.client,
            acl_table_group_id,
            acl_table_id2,
            group_member_priority2)

        # bind this ACL table group to port2s object id
        attr_value = sai_thrift_attribute_value_t(oid=acl_table_group_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        try:
            assert acl_table_group_id > 0, 'acl_table_group_id is <= 0'
            assert acl_table_id1 > 0, 'acl_entry_id1 is <= 0'
            assert acl_entry_id1 > 0, 'acl_entry_id1 is <= 0'
            assert acl_table_id2 > 0, 'acl_entry_id2 is <= 0'
            assert acl_entry_id2 > 0, 'acl_entry_id2 is <= 0'
            assert acl_table_group_member_id1 > 0, 'acl_table_group_member_id1 is <= 0'
            assert acl_table_group_member_id2 > 0, 'acl_table_group_member_id2 is <= 0'

            print '#### ACL \'DROP, src mac 00:22:22:22:22:22, in_ports[ptf_intf_1,2]\' Applied ####'
            print '#### Sending      ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 2'
            # send the same packet
            send_packet(self, 1, str(pkt))
            # ensure packet is dropped
            # check for absence of packet here!
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 1'
            verify_no_packet(self, exp_pkt, 0)
        finally:
            # unbind this ACL table from port2s object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            # cleanup ACL
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id1)
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id2)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id1)
            self.client.sai_thrift_remove_acl_table(acl_table_id1)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id2)
            self.client.sai_thrift_remove_acl_table(acl_table_id2)
            self.client.sai_thrift_remove_acl_table_group(acl_table_group_id)
            # cleanup
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('acl')
class MultBindAclTableGroupTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print '----------------------------------------------------------------------------------------------'
        print "Sending packet ptf_intf 4 -> [ptf_intf 1, ptf_intf 2, ptf_intf 3] (192.168.0.1 ---> 10.10.10.1 [id = 105])"

        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)
        rif_id3 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)
        rif_id4 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port4, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_mask1 = '255.255.255.255'
        dmac1 = '00:11:22:33:44:55'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id4, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id4)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, rif_id4)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=router_mac,
            eth_src='00:22:22:22:22:22',
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)
        exp_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=router_mac,
            ip_dst='10.10.10.1',
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=63)
        try:
            print '#### NO ACL Applied ####'
            print '#### Sending  ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 1'
            send_packet(self, 0, str(pkt))
            print '#### Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 4'
            verify_packet(self, exp_pkt, 3)
            print '#### Sending  ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 2'
            send_packet(self, 1, str(pkt))
            print '#### Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 4'
            verify_packet(self, exp_pkt, 3)
            print '#### Sending  ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 3'
            send_packet(self, 2, str(pkt))
            print '#### Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 4'
            verify_packet(self, exp_pkt, 3)
        finally:
            print '----------------------------------------------------------------------------------------------'

        print "Sending packet [ptf_intf 1, ptf_intf 2, ptf_intf 3] - [acl]-> ptf_intf 4 (192.168.0.1 -[acl]-> 10.10.10.1 [id = 105])"

        # setup ACL table group
        group_stage = SAI_ACL_STAGE_INGRESS
        group_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        group_type = SAI_ACL_TABLE_GROUP_TYPE_SEQUENTIAL

        # create ACL table group
        acl_table_group_id = sai_thrift_create_acl_table_group(self.client,
            group_stage,
            group_bind_point_list,
            group_type)

        # setup ACL tables to block based on Source MAC
        table_stage = SAI_ACL_STAGE_INGRESS
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        entry_priority = 1
        action = SAI_PACKET_ACTION_DROP
        in_ports = [port1, port2, port3, port4]
        mac_src = '00:22:22:22:22:22'
        mac_dst = None
        mac_src_mask = 'ff:ff:ff:ff:ff:ff'
        mac_dst_mask = None
        ip_src = None
        ip_src_mask = None
        ip_dst = None
        ip_dst_mask = None
        ip_proto = None
        in_port = None
        out_port = None
        out_ports = None
        src_l4_port = None
        dst_l4_port = None
        ingress_mirror_id = None
        egress_mirror_id = None

        # create ACL table #1
        acl_table_id1 = sai_thrift_create_acl_table(self.client,
            table_stage,
            table_bind_point_list,
            addr_family,
            mac_src,
            mac_dst,
            ip_src,
            ip_dst,
            ip_proto,
            in_ports,
            out_ports,
            in_port,
            out_port,
            src_l4_port,
            dst_l4_port)
        acl_entry_id1 = sai_thrift_create_acl_entry(self.client,
            acl_table_id1,
            entry_priority,
            action, addr_family,
            mac_src, mac_src_mask,
            mac_dst, mac_dst_mask,
            ip_src, ip_src_mask,
            ip_dst, ip_dst_mask,
            ip_proto,
            in_ports, out_ports,
            in_port, out_port,
            src_l4_port, dst_l4_port,
            ingress_mirror_id,
            egress_mirror_id)

        # create ACL table #2
        acl_table_id2 = sai_thrift_create_acl_table(self.client,
            table_stage,
            table_bind_point_list,
            addr_family,
            mac_src,
            mac_dst,
            ip_src,
            ip_dst,
            ip_proto,
            in_ports,
            out_ports,
            in_port,
            out_port,
            src_l4_port,
            dst_l4_port)
        acl_entry_id2 = sai_thrift_create_acl_entry(self.client,
            acl_table_id2,
            entry_priority,
            action, addr_family,
            mac_src, mac_src_mask,
            mac_dst, mac_dst_mask,
            ip_src, ip_src_mask,
            ip_dst, ip_dst_mask,
            ip_proto,
            in_ports, out_ports,
            in_port, out_port,
            src_l4_port, dst_l4_port,
            ingress_mirror_id,
            egress_mirror_id)

        # setup ACL table group members
        group_member_priority1 = 1
        group_member_priority2 = 100

        # create ACL table group members
        acl_table_group_member_id1 = sai_thrift_create_acl_table_group_member(self.client,
            acl_table_group_id,
            acl_table_id1,
            group_member_priority1)
        acl_table_group_member_id2 = sai_thrift_create_acl_table_group_member(self.client,
            acl_table_group_id,
            acl_table_id2,
            group_member_priority2)

        # bind this ACL table group to port1, port2, port3 object id
        attr_value = sai_thrift_attribute_value_t(oid=acl_table_group_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        try:
            assert acl_table_group_id > 0, 'acl_table_group_id is <= 0'
            assert acl_table_id1 > 0, 'acl_entry_id1 is <= 0'
            assert acl_entry_id1 > 0, 'acl_entry_id1 is <= 0'
            assert acl_table_id2 > 0, 'acl_entry_id2 is <= 0'
            assert acl_entry_id2 > 0, 'acl_entry_id2 is <= 0'
            assert acl_table_group_member_id1 > 0, 'acl_table_group_member_id1 is <= 0'
            assert acl_table_group_member_id2 > 0, 'acl_table_group_member_id2 is <= 0'

            print '#### ACL \'DROP, src mac 00:22:22:22:22:22, in_ports[ptf_intf_1,2,3,4]\' Applied ####'
            print '#### Sending      ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 1'
            send_packet(self, 0, str(pkt))
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 4'
            verify_no_packet(self, exp_pkt, 3)
            print '#### Sending      ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 2'
            send_packet(self, 1, str(pkt))
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 4'
            verify_no_packet(self, exp_pkt, 3)
            print '#### Sending      ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 3'
            send_packet(self, 2, str(pkt))
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 4'
            verify_no_packet(self, exp_pkt, 3)
        finally:
            # unbind this ACL table from port1, port2, port3 object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)
            # cleanup ACL
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id1)
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id2)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id1)
            self.client.sai_thrift_remove_acl_table(acl_table_id1)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id2)
            self.client.sai_thrift_remove_acl_table(acl_table_id2)
            self.client.sai_thrift_remove_acl_table_group(acl_table_group_id)
            # cleanup
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1, ip_mask1, rif_id4)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id4, ip_addr1, dmac1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_router_interface(rif_id3)
            self.client.sai_thrift_remove_router_interface(rif_id4)
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('acl')
class BindAclTableInGroupTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print '----------------------------------------------------------------------------------------------'
        print "Sending packet [ptf_intf 1, ptf_intf 2, ptf_intf 3] -> ptf_intf 4 (192.168.0.1 ---> 10.10.10.1 [id = 105])"

        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port1, 0, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port2, 0, v4_enabled, v6_enabled, mac)
        rif_id3 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port3, 0, v4_enabled, v6_enabled, mac)
        rif_id4 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port4, 0, v4_enabled, v6_enabled, mac)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_subnet1 = '10.10.10.0'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'
        sai_thrift_create_neighbor(self.client, addr_family, rif_id4, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id4)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_subnet1, ip_mask1, rif_id4)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=router_mac,
            eth_src='00:22:22:22:22:22',
            ip_dst=ip_addr1,
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=64)
        exp_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=router_mac,
            ip_dst=ip_addr1,
            ip_src='192.168.0.1',
            ip_id=105,
            ip_ttl=63)
        try:
            print '#### NO ACL Applied ####'
            print '#### Sending  ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 1'
            send_packet(self, 0, str(pkt))
            print '#### Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 4'
            verify_packet(self, exp_pkt, 3)
            print '#### Sending  ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 2'
            send_packet(self,1, str(pkt))
            print '#### Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 4'
            verify_packet(self, exp_pkt, 3)
            print '#### Sending  ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 3'
            send_packet(self, 2, str(pkt))
            print '#### Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 4'
            verify_packet(self, exp_pkt, 3)
        finally:
            print '----------------------------------------------------------------------------------------------'

        print "Sending packet [ptf_intf 1, ptf_intf 2, ptf_intf 3] -> ptf_intf 4 (192.168.0.1 ---> 10.10.10.1 [id = 105])"

        # setup ACL table group
        group_stage = SAI_ACL_STAGE_INGRESS
        group_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        group_type = SAI_ACL_TABLE_GROUP_TYPE_SEQUENTIAL

        # create ACL table group
        acl_table_group_id = sai_thrift_create_acl_table_group(self.client,
            group_stage,
            group_bind_point_list,
            group_type)

        # setup ACL tables to block based on Source MAC
        table_stage = SAI_ACL_STAGE_INGRESS
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        entry_priority = 1
        action = SAI_PACKET_ACTION_DROP
        in_ports1 = [port1]
        in_ports2 = [port2]
        in_ports3 = [port3]
        mac_src = '00:22:22:22:22:22'
        mac_dst = None
        mac_src_mask = 'ff:ff:ff:ff:ff:ff'
        mac_dst_mask = None
        ip_src = None
        ip_src_mask = None
        ip_dst = None
        ip_dst_mask = None
        ip_proto = None
        in_port = None
        out_port = None
        out_ports = None
        src_l4_port = None
        dst_l4_port = None
        ingress_mirror_id = None
        egress_mirror_id = None

        # create ACL table #1
        acl_table_id1 = sai_thrift_create_acl_table(self.client,
            table_stage,
            table_bind_point_list,
            addr_family,
            mac_src,
            mac_dst,
            ip_src,
            ip_dst,
            ip_proto,
            in_ports1,
            out_ports,
            in_port,
            out_port,
            src_l4_port,
            dst_l4_port)
        acl_entry_id1 = sai_thrift_create_acl_entry(self.client,
            acl_table_id1,
            entry_priority,
            action, addr_family,
            mac_src, mac_src_mask,
            mac_dst, mac_dst_mask,
            ip_src, ip_src_mask,
            ip_dst, ip_dst_mask,
            ip_proto,
            in_ports1, out_ports,
            in_port, out_port,
            src_l4_port, dst_l4_port,
            ingress_mirror_id,
            egress_mirror_id)

        # create ACL table #2
        acl_table_id2 = sai_thrift_create_acl_table(self.client,
            table_stage,
            table_bind_point_list,
            addr_family,
            mac_src,
            mac_dst,
            ip_src,
            ip_dst,
            ip_proto,
            in_ports2,
            out_ports,
            in_port,
            out_port,
            src_l4_port,
            dst_l4_port)
        acl_entry_id2 = sai_thrift_create_acl_entry(self.client,
            acl_table_id2,
            entry_priority,
            action, addr_family,
            mac_src, mac_src_mask,
            mac_dst, mac_dst_mask,
            ip_src, ip_src_mask,
            ip_dst, ip_dst_mask,
            ip_proto,
            in_ports2, out_ports,
            in_port, out_port,
            src_l4_port, dst_l4_port,
            ingress_mirror_id,
            egress_mirror_id)

        # create ACL table #3
        acl_table_id3 = sai_thrift_create_acl_table(self.client,
            table_stage,
            table_bind_point_list,
            addr_family,
            mac_src,
            mac_dst,
            ip_src,
            ip_dst,
            ip_proto,
            in_ports3,
            out_ports,
            in_port,
            out_port,
            src_l4_port,
            dst_l4_port)
        acl_entry_id3 = sai_thrift_create_acl_entry(self.client,
            acl_table_id3,
            entry_priority,
            action, addr_family,
            mac_src, mac_src_mask,
            mac_dst, mac_dst_mask,
            ip_src, ip_src_mask,
            ip_dst, ip_dst_mask,
            ip_proto,
            in_ports3, out_ports,
            in_port, out_port,
            src_l4_port, dst_l4_port,
            ingress_mirror_id,
            egress_mirror_id)

        # setup ACL table group members
        group_member_priority1 = 1
        group_member_priority2 = 100
        group_member_priority3 = 50

        # create ACL table group members
        acl_table_group_member_id1 = sai_thrift_create_acl_table_group_member(self.client,
            acl_table_group_id,
            acl_table_id1,
            group_member_priority1)
        acl_table_group_member_id2 = sai_thrift_create_acl_table_group_member(self.client,
            acl_table_group_id,
            acl_table_id2,
            group_member_priority2)
        acl_table_group_member_id3 = sai_thrift_create_acl_table_group_member(self.client,
            acl_table_group_id,
            acl_table_id3,
            group_member_priority3)

        # bind this ACL table group to port1, port2, port3 object id
        attr_value = sai_thrift_attribute_value_t(oid=acl_table_group_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        try:
            assert acl_table_group_id > 0, 'acl_table_group_id is <= 0'
            assert acl_table_id1 > 0, 'acl_table_id1 is <= 0'
            assert acl_entry_id1 > 0, 'acl_entry_id1 is <= 0'
            assert acl_table_id2 > 0, 'acl_table_id2 is <= 0'
            assert acl_entry_id2 > 0, 'acl_entry_id2 is <= 0'
            assert acl_table_id3 > 0, 'acl_table_id3 is <= 0'
            assert acl_entry_id3 > 0, 'acl_entry_id3 is <= 0'
            assert acl_table_group_member_id1 > 0, 'acl_table_group_member_id1 is <= 0'
            assert acl_table_group_member_id2 > 0, 'acl_table_group_member_id2 is <= 0'
            assert acl_table_group_member_id3 > 0, 'acl_table_group_member_id3 is <= 0'

            print '#### ACL \'DROP, src mac 00:22:22:22:22:22, in_ports[ptf_intf_1,2,3]\' Applied ####'
            print '#### Sending      ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 1'
            send_packet(self, 0, str(pkt))
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 4'
            verify_no_packet(self, exp_pkt, 3)
            print '#### Sending      ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 2'
            send_packet(self, 1, str(pkt))
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 4'
            verify_no_packet(self, exp_pkt, 3)
            print '#### Sending      ', router_mac, '| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 3'
            send_packet(self, 2, str(pkt))
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 192.168.0.1 | @ ptf_intf 4'
            verify_no_packet(self, exp_pkt, 3)
        finally:
            # unbind this ACL table from port1, port2, port3, port4 object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)
            # cleanup ACL
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id1)
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id2)
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id3)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id1)
            self.client.sai_thrift_remove_acl_table(acl_table_id1)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id2)
            self.client.sai_thrift_remove_acl_table(acl_table_id2)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id3)
            self.client.sai_thrift_remove_acl_table(acl_table_id3)
            self.client.sai_thrift_remove_acl_table_group(acl_table_group_id)
            # cleanup
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_subnet1, ip_mask1, rif_id4)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id4, ip_addr1, dmac1)

            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_router_interface(rif_id3)
            self.client.sai_thrift_remove_router_interface(rif_id4)
            self.client.sai_thrift_remove_virtual_router(vr_id)

@group('acl')
class L3AclTableTestI(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        Description:
        Create two router interfaces with one over a LAG and another VLAN.
        Create a nhop, route and neighbor entry for destination and verify
        traffic forwarding. Create ACL table with stage Ingress, destination
        IPv4 and bind point type LAG. Create an ACL entry for the destination
        IP with action drop and verify traffic.

        Steps:
           1. Create VLAN 100 and associate untagged port 2 as the member port of VLAN 100.
           2. Set port attribute value of "Port VLAN ID 100" for the port 2.
           3. Create Virtual router V1 and enable V4.
           4. Create LAG "Id1"
           5. Remove the port 1 from default VLAN and add the port 1 as member of LAG Id1.
           6. Create two virtual router interfaces and set the interface type as "VLAN and "LAG" for port 1 and 2 respectively.
           7. Create IPv4 neighbor entry (192.168.0.1) with MAC1 and associate with "RIF Id2".
           8. Create next hop to reach the neighbor entry.
           9. Create route entry with /24 mask (i.e 11.11.11.0/24) through NHop1
           10. Send IPv4 packet from port 1 (LAG member) with destination IP 11.11.11.1

        Part1:
           11. Create ACL table and entry block the traffic based on Destination IP address (11.11.11.1)
           11. Bind the ACL table with LAG Id1 with attribute SAI_LAG_ATTR_INGRESS_ACL
           12. Send the IP packet again through port 1 and ensure the packet is dropped.
           13. Remove the ACL attribute and send the packet from port 1 (LAG) and verify the packet is received on
               port 2 (VLAN)
        Part2:
           14. Remove the ACL table and entry.
           15. Remove the route and the associated router interface, vlan members and the corresponding VLAN from the database.
        """
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 0
        vlan_id = 100
        mac_action = SAI_PACKET_ACTION_FORWARD
        mac1 = ''
        acl_entry_id = SAI_NULL_OBJECT_ID
        acl_table_id = SAI_NULL_OBJECT_ID

        # "Creating VLAN ID"
        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        #Creating Virtual router
        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        lag_id1 = self.client.sai_thrift_create_lag([])
        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, [port1])
        lag_member_id1 = sai_thrift_create_lag_member(self.client, lag_id1, port1)

        #Creating RIF
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, lag_id1, 0, v4_enabled, v6_enabled, mac1)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan_oid, v4_enabled, v6_enabled, mac1)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'
        ip_addr2 = '192.168.0.1'
        ip_addr2_subnet = '192.168.0.0'
        ip_mask2 = '255.255.0.0'
        ip_addr3_subnet = '11.11.11.0'
        dmac2 = '00:11:22:33:44:56'

        #Creating Neighbor, NHop and route
        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac2)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr2, rif_id2)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr3_subnet, ip_mask1, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr2_subnet, ip_mask2, rif_id2)

        sai_thrift_create_fdb(self.client, vlan_oid, dmac2, port2, mac_action)

        # send the test packet(s)
        pkt = simple_tcp_packet(pktlen=100,
                                eth_dst=router_mac,
                                eth_src=dmac1,
                                ip_src=ip_addr1,
                                ip_dst='11.11.11.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(
                                pktlen=100,
                                eth_dst=dmac2,
                                eth_src=router_mac,
                                ip_src=ip_addr1,
                                ip_dst='11.11.11.1',
                                ip_id=105,
                                ip_ttl=63)
        try:
            print
            print '#### NO ACL Applied ####'
            print "Sending packet port 1 -> port 2 (10.10.10.1 -> 11.11.11.1 [id = 105])"
            send_packet(self, 0, str(pkt))
            print "Verifying packet"
            verify_packets(self, exp_pkt, [1])
            print "Expected packets received"

            # setup ACL to block based on Destination IP
            print "\nSetting up ACL"
            table_stage = SAI_ACL_STAGE_INGRESS
            table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_LAG]
            entry_priority = SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY
            action = SAI_PACKET_ACTION_DROP
            in_ports = None
            mac_src = None
            mac_dst = None
            mac_src_mask = None
            mac_dst_mask = None
            ip_src = None
            ip_src_mask = None
            ip_dst = "11.11.11.1"
            ip_dst_mask = "255.255.255.0"
            ip_proto = None
            in_port = None
            out_port = None
            out_ports = None
            src_l4_port = None
            dst_l4_port = None
            ingress_mirror_id = None
            egress_mirror_id = None

            acl_table_id = sai_thrift_create_acl_table(
                                                self.client,
                                                table_stage,
                                                table_bind_point_list,
                                                addr_family,
                                                mac_src,
                                                mac_dst,
                                                ip_src,
                                                ip_dst,
                                                ip_proto,
                                                in_ports,
                                                out_ports,
                                                in_port,
                                                out_port,
                                                src_l4_port,
                                                dst_l4_port)
            acl_entry_id = sai_thrift_create_acl_entry(
                                                self.client,
                                                acl_table_id,
                                                entry_priority,
                                                action,
                                                addr_family,
                                                mac_src,
                                                mac_src_mask,
                                                mac_dst,
                                                mac_dst_mask,
                                                ip_src,
                                                ip_src_mask,
                                                ip_dst,
                                                ip_dst_mask,
                                                ip_proto,
                                                in_ports,
                                                out_ports,
                                                in_port,
                                                out_port,
                                                src_l4_port,
                                                dst_l4_port,
                                                ingress_mirror_id,
                                                egress_mirror_id)

            # bind this ACL table to LAG object id
            print "Binding ACL to LAG"
            attr_value = sai_thrift_attribute_value_t(oid=acl_table_id)
            attr = sai_thrift_attribute_t(id=SAI_LAG_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_lag_attribute(lag_id1, attr)

            print "Asserting ACL table entry"
            assert acl_table_id > 0, 'acl_entry_id is <= 0'
            assert acl_entry_id > 0, 'acl_entry_id is <= 0'
            print '#### ACL \'DROP, dst 11.11.11.1/255.255.255.0, in_ports[ptf_intf_1]\' Applied ####'
            print '#### Sending      ', router_mac, '| 00:11:22:33:44:55 | 10.10.10.1 | 11.11.11.1 | @ ptf_intf 1'

            send_packet(self, 0, str(pkt))
            print '#### NOT Expecting 00:11:22:33:44:56 |', router_mac, '| 10.10.10.1 | 11.11.11.1 | @ ptf_intf 2'
            verify_no_packet(self, exp_pkt, 1)
            print "ACL blocked packet as expected"
            print "\nUnbind lag attribute and check the traffic"
            # unbind this ACL table from port2s object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_LAG_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_lag_attribute(lag_id1, attr)

            print "Sending packet port 1 -> port 2 (10.10.10.1 -> 11.11.11.1 [id = 105])"
            send_packet(self, 0, str(pkt))
            verify_packets(self, exp_pkt, [1])
            print "Expected packets received"

        finally:
            # unbind this ACL table from port2s object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_LAG_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_lag_attribute(lag_id1, attr)
            # cleanup ACL
            self.client.sai_thrift_remove_acl_entry(acl_entry_id)
            self.client.sai_thrift_remove_acl_table(acl_table_id)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr2_subnet, ip_mask2, rif_id2)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr3_subnet, ip_mask1, nhop1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac1)
            sai_thrift_remove_lag_member(self.client, lag_member_id1)
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_lag(lag_id1)
            self.client.sai_thrift_remove_virtual_router(vr_id)
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan(vlan_oid)


@group('acl')
class L3AclTableGroupTestI(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        Description:
        Create route objects as in "L3AclTableTest_I". Create two ACL tables
        with stage ingress, and one with destination IPv4 and other with
        source IPv4. Create ACL entry in both the tables with source IP address
        action drop and destination IP address action drop. Associate the two
        tables to an ACL group with type "parallel" and bind it to the LAG.
        Send traffic matching the destination IP and source IP and verify output

        Also included L3AclTableGroupTest_III: Remove the group binding and verify
        that traffic is now forwarded for the LAG

        Steps:
          1. Create VLAN 100 and associate untagged port 2 as the member port of VLAN 100.
          2. Set port attribute value of "Port VLAN ID 100" for the port 2.
          3. Create Virtual router V1 and enable V4.
          4. Create LAG "Id1"
          5. Remove the port 1 from default VLAN and add the port 1 as member of LAG Id1.
          6. Create two virtual router interfaces and set the interface type as "LAG" and "VLAN" for port 1 and 2 respectively.
          7. Create IPv4 neighbor entry (192.168.0.1) with MAC1 and associate with "RIF Id1".
          8. Create next hop to reach the neighbor entry.
          9. Create route entry with /24 mask (i.e 11.11.11.0/24) through NHop1
          10. Send IPv4 packet from port 1 (LAG member) with destination IP 11.11.11.1

        Part1:
          11. Create ACL table group "acl_table_group_id"
          12. Create two ACL table to block the traffic based on Dst IP address (11.11.11.1) and Src IP address (10.10.10.1).
          13. Associate the ACL table with the group members.
          14. Bind the ACL table and entry with LAG Id1 with attribute SAI_LAG_ATTR_INGRESS_ACL
          15. Send the IP packet again through port 1 and ensure the packet is dropped.
        Part2:
          16. Remove the ACL attribute and send the packet again from port 1 (LAG member)
          17. Packet will pass through LAG to the VLAN
          18. Remove ACL table and entry.
          19. Remove the route and the associated router interface, vlan members and the corresponding VLAN from the database.
        """

        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 0
        vlan_id = 100
        mac_action = SAI_PACKET_ACTION_FORWARD
        mac1 = ''

        acl_table_group_member_id1 = SAI_NULL_OBJECT_ID
        acl_table_group_member_id2 = SAI_NULL_OBJECT_ID
        acl_entry_id1 = SAI_NULL_OBJECT_ID
        acl_entry_id2 = SAI_NULL_OBJECT_ID
        acl_table_id1 = SAI_NULL_OBJECT_ID
        acl_table_id2 = SAI_NULL_OBJECT_ID
        acl_table_group_id = SAI_NULL_OBJECT_ID

        # Creating VLAN
        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        # Creating VR and LAG
        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        lag_id1 = self.client.sai_thrift_create_lag([])
        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, [port1])
        lag_member_id1 = sai_thrift_create_lag_member(self.client, lag_id1, port1)

        # Creating RIF
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, lag_id1, 0, v4_enabled, v6_enabled, mac1)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan_oid, v4_enabled, v6_enabled, mac1)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_addr_subnet = '11.11.11.0'
        ip_mask1 = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'
        ip_addr2 = '192.168.0.1'
        ip_addr2_subnet = '192.168.0.0'
        ip_mask2 = '255.255.0.0'
        dmac2 = '00:11:22:33:44:56'

        # Creating Neighbor, NHop and Route
        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac2)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr2, rif_id2)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask1, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr2_subnet, ip_mask2, rif_id2)
        sai_thrift_create_fdb(self.client, vlan_oid, dmac2, port2, mac_action)

        # send the test packet(s)
        pkt = simple_tcp_packet(pktlen=100,
                                eth_dst=router_mac,
                                eth_src=dmac1,
                                ip_src=ip_addr1,
                                ip_dst='11.11.11.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(
                                pktlen=100,
                                eth_dst=dmac2,
                                eth_src=router_mac,
                                ip_src=ip_addr1,
                                ip_dst='11.11.11.1',
                                ip_id=105,
                                ip_ttl=63)
        try:
            print "\n\nNo ACL has been applied: Expecting packet to go through"
            print "Sending packet from port 1 to port 2"
            send_packet(self, 0, str(pkt))
            print "Verifying packet on port 2"
            verify_packets(self, exp_pkt, [1])
            print "Packet received as expected\n\n"

            # setup ACL table group
            group_stage = SAI_ACL_STAGE_INGRESS
            group_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_LAG]
            group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

            # create ACL table group
            print "Creating ACL table group"
            acl_table_group_id = sai_thrift_create_acl_table_group(
                                                                self.client,
                                                                group_stage,
                                                                group_bind_point_list,
                                                                group_type)


            # setup ACL to block based on Destination IP
            table_stage = SAI_ACL_STAGE_INGRESS
            table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_LAG]
            entry_priority = SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY
            action = SAI_PACKET_ACTION_DROP
            in_ports = None
            mac_src = None
            mac_dst = None
            mac_src_mask = None
            mac_dst_mask = None
            ip_proto = None
            in_port = None
            out_port = None
            out_ports = None
            src_l4_port = None
            dst_l4_port = None
            ingress_mirror_id = None
            egress_mirror_id = None

            ip_src1 = "10.10.10.1"
            ip_src_mask1 = "255.255.255.0"
            ip_dst1 = None
            ip_dst_mask1 = None

            ip_src2 = None
            ip_src_mask2 = None
            ip_dst2 = "11.11.11.1"
            ip_dst_mask2 = "255.255.255.0"

            group_member_priority = 1

            # create ACL table #1
            acl_table_id1 = sai_thrift_create_acl_table(
                                                        self.client,
                                                        table_stage,
                                                        table_bind_point_list,
                                                        addr_family,
                                                        mac_src,
                                                        mac_dst,
                                                        ip_src1,
                                                        ip_dst1,
                                                        ip_proto,
                                                        in_ports,
                                                        out_ports,
                                                        in_port,
                                                        out_port,
                                                        src_l4_port,
                                                        dst_l4_port)
            # Creating ACL Entry 1
            acl_entry_id1 = sai_thrift_create_acl_entry(
                                                        self.client,
                                                        acl_table_id1,
                                                        entry_priority,
                                                        action,
                                                        addr_family,
                                                        mac_src, mac_src_mask,
                                                        mac_dst, mac_dst_mask,
                                                        ip_src1, ip_src_mask1,
                                                        ip_dst1, ip_dst_mask1,
                                                        ip_proto,
                                                        in_ports, out_ports,
                                                        in_port, out_port,
                                                        src_l4_port, dst_l4_port,
                                                        ingress_mirror_id,
                                                        egress_mirror_id)
            # create ACL table #
            acl_table_id2 = sai_thrift_create_acl_table(
                                                        self.client,
                                                        table_stage,
                                                        table_bind_point_list,
                                                        addr_family,
                                                        mac_src,
                                                        mac_dst,
                                                        ip_src2,
                                                        ip_dst2,
                                                        ip_proto,
                                                        in_ports,
                                                        out_ports,
                                                        in_port,
                                                        out_port,
                                                        src_l4_port,
                                                        dst_l4_port)

            acl_entry_id2 = sai_thrift_create_acl_entry(
                                                        self.client,
                                                        acl_table_id2,
                                                        entry_priority,
                                                        action, addr_family,
                                                        mac_src, mac_src_mask,
                                                        mac_dst, mac_dst_mask,
                                                        ip_src2, ip_src_mask2,
                                                        ip_dst2, ip_dst_mask2,
                                                        ip_proto,
                                                        in_ports, out_ports,
                                                        in_port, out_port,
                                                        src_l4_port, dst_l4_port,
                                                        ingress_mirror_id,
                                                        egress_mirror_id)

            # create ACL table group members
            acl_table_group_member_id1 = sai_thrift_create_acl_table_group_member(self.client,
                                                                                  acl_table_group_id,
                                                                                  acl_table_id1,
                                                                                  group_member_priority)
            acl_table_group_member_id2 = sai_thrift_create_acl_table_group_member(self.client,
                                                                                  acl_table_group_id,
                                                                                  acl_table_id2,
                                                                                  group_member_priority)

            # bind this ACL table to port2s object id
            print "Binding ACL to LAG"
            attr_value = sai_thrift_attribute_value_t(oid=acl_table_group_id)
            attr = sai_thrift_attribute_t(id=SAI_LAG_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_lag_attribute(lag_id1, attr)

            print "Asserting the ACL entries"
            assert acl_table_group_id > 0, 'acl_table_group_id is <= 0'
            assert acl_table_id1 > 0, 'acl_entry_id1 is <= 0'
            assert acl_entry_id1 > 0, 'acl_entry_id1 is <= 0'
            assert acl_table_id2 > 0, 'acl_entry_id2 is <= 0'
            assert acl_entry_id2 > 0, 'acl_entry_id2 is <= 0'
            assert acl_table_group_member_id1 > 0, 'acl_table_group_member_id1 is <= 0'
            assert acl_table_group_member_id2 > 0, 'acl_table_group_member_id2 is <= 0'
            print '#### ACL \'DROP, dst 11.11.11.1/255.255.255.0, in_ports[ptf_intf_1]\' Applied ####'
            print '#### Sending      ', router_mac, '| 00:11:22:33:44:55 | 10.10.10.1 | 11.11.11.1 | @ ptf_intf 1'
            # send the same packet
            send_packet(self, 0, str(pkt))
            print '#### NOT Expecting 00:11:22:33:44:56 |', router_mac, '| 10.10.10.1 | 11.11.11.1 | @ ptf_intf 2\n'
            verify_no_packet(self, exp_pkt, 1)

            # unbind this ACL table from lag object id
            print "Unbind lag attribute and check the traffic"

            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_LAG_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_lag_attribute(lag_id1, attr)

            print "Sending packets from port 1 to port 2"
            send_packet(self, 0, str(pkt))
            verify_packets(self, exp_pkt, [1])
            print "Packet received as expected"

        finally:
            # unbind this ACL table from lag object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_LAG_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_lag_attribute(lag_id1, attr)
            # cleanup ACL
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id1)
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id2)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id1)
            self.client.sai_thrift_remove_acl_table(acl_table_id1)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id2)
            self.client.sai_thrift_remove_acl_table(acl_table_id2)
            self.client.sai_thrift_remove_acl_table_group(acl_table_group_id)
            # Cleanup router config
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr2_subnet, ip_mask2, rif_id2)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr_subnet, ip_mask1, nhop1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac2)
            sai_thrift_remove_lag_member(self.client, lag_member_id1)
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_lag(lag_id1)
            self.client.sai_thrift_remove_virtual_router(vr_id)
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan(vlan_oid)


@group('acl')
class L3AclTableGroupTestII(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        L3AclTableGroupTestI Description:
        Create route objects as in "L3AclTableTest_I". Create two ACL tables
        with stage ingress, and one with destination IPv4 and other with
        source IPv4. Create ACL entry in both the tables with source IP address
        action drop and destination IP address action drop. Associate the two
        tables to an ACL group with type "parallel" and bind it to the LAG.
        Send traffic matching the destination IP and source IP and verify output

        L3AclTableGroupTestII description:
        Same as "L3AclTableGroupTest_I" for bind point type VLAN.

        L3AclTableGroupTestIII description:
        Remove the group binding and verify that traffic is now forwarded for both the LAG and VLAN.

        Steps:
        Part1:
          1. Create VLAN 100 and associate untagged port 1 as the member port of VLAN 100.
          2. Set port attribute value of "Port VLAN ID 100" for the port 1.
          3. Create Virtual router V1 and enable V4.
          4. Create LAG "Id1"
          5. Remove port 2 from default VLAN and add it as member of LAG Id1.
          6. Create two virtual router interfaces and set the interface type as "VLAN and "LAG" for port 1 and 2 respectively.
          7. Create IPv4 neighbor entry (192.168.0.1) with MAC1 and associate with "RIF Id1".
          8. Create IPv4 neighbor entry (10.10.10.1) with MAC2 and associate with "RIF Id2".
          9. Create next hop to reach the neighbor entry.
          10. Create route entry with /24 mask (i.e 11.11.11.0/24) through Nhop1
          11. Create route entry with /24 mask (i.e 12.12.12.0/24) through Nhop2
          12. Send IPv4 packet from port 1 (VLAN member) with destination IP 192.168.0.1
          13. Send IPv4 packet from port 2 (LAG member) with destination IP 10.10.10.1

        Part2:
          11. Create ACL table group "acl_grp_Id1" and "acl_grp_Id2"
          14. Create two ACL table to block the traffic based on Src IP address (192.168.0.1) and Dst IP address (12.12.12.1).
          15. Create two ACL table to block the traffic based on Src IP address (10.10.10.1) and Dst IP address (11.11.11.1).
          16. Associate all the ACL tables with the group members.
          17. Bind the ACL table with VLAN attribute SAI_VLAN_ATTR_INGRESS_ACL and LAG attribute SAI_LAG_ATTR_INGRESS_ACL
          18. Send the IP packet again through port 1 and 2 and ensure the packets are dropped at Ingress of LAG and VLAN.

        Part3:
          19. Remove all the ACL table and the entries bound with VLAN and LAG port
          20. Send the packet from port 1 and 2 and ensure the traffic forwarded for VLAN and LAG.
          21. Remove the route and the associated router interface, vlan members and the corresponding VLAN from the database.
        """

        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        v4_enabled = 1
        v6_enabled = 0
        vlan_id = 100
        mac_action = SAI_PACKET_ACTION_FORWARD
        mac1 = ''

        # Creating VLAN
        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        # Creating VR and LAG
        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        lag_id1 = self.client.sai_thrift_create_lag([])
        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, [port1])
        lag_member_id1 = sai_thrift_create_lag_member(self.client, lag_id1, port1)

        # Creating RIF
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, lag_id1, 0, v4_enabled, v6_enabled, mac1)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan_oid, v4_enabled, v6_enabled, mac1)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_addr1_subnet = '10.10.10.0'
        ip_addr_subnet1 = '11.11.11.0'
        ip_mask = '255.255.255.0'
        dmac1 = '00:11:22:33:44:55'
        ip_addr2 = '192.168.0.1'
        ip_addr2_subnet ='192.168.0.0'
        ip_addr_subnet2 = '12.12.12.0'
        ip_mask2 ='255.255.0.0'
        dmac2 = '00:11:22:33:44:56'

        # Creating Neighbor, NHop and Route
        sai_thrift_create_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac2)
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)

        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr2, rif_id2)
        nhop2 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)

        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr_subnet1, ip_mask, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr_subnet2, ip_mask, nhop2)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr2_subnet, ip_mask2, rif_id2)


        sai_thrift_create_fdb(self.client, vlan_oid, dmac2, port2, mac_action)

        # send the test packet(s)
        pkt = simple_tcp_packet(
                                pktlen=100,
                                eth_dst=router_mac,
                                eth_src=dmac1,
                                ip_src=ip_addr1,
                                ip_dst='11.11.11.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(
                                pktlen=100,
                                eth_dst=dmac2,
                                eth_src=router_mac,
                                ip_src=ip_addr1,
                                ip_dst='11.11.11.1',
                                ip_id=105,
                                ip_ttl=63)
        pkt1 = simple_tcp_packet(
                                pktlen=100,
                                eth_dst=router_mac,
                                eth_src=dmac2,
                                ip_src=ip_addr2,
                                ip_dst='12.12.12.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(
                                pktlen=100,
                                eth_dst=dmac1,
                                eth_src=router_mac,
                                ip_src=ip_addr2,
                                ip_dst='12.12.12.1',
                                ip_id=105,
                                ip_ttl=63)
        acl_table_group_member_id1 = SAI_NULL_OBJECT_ID
        acl_table_group_member_id2 = SAI_NULL_OBJECT_ID
        acl_table_group_member_id3 = SAI_NULL_OBJECT_ID
        acl_table_group_member_id4 = SAI_NULL_OBJECT_ID
        acl_entry_id1 = SAI_NULL_OBJECT_ID
        acl_entry_id2 = SAI_NULL_OBJECT_ID
        acl_entry_id3 = SAI_NULL_OBJECT_ID
        acl_entry_id4 = SAI_NULL_OBJECT_ID
        acl_table_id1 = SAI_NULL_OBJECT_ID
        acl_table_id2 = SAI_NULL_OBJECT_ID
        acl_table_id3 = SAI_NULL_OBJECT_ID
        acl_table_id4 = SAI_NULL_OBJECT_ID
        acl_table_group_id1 = SAI_NULL_OBJECT_ID
        acl_table_group_id2 = SAI_NULL_OBJECT_ID

        try:
            print "No ACL is applied: Expecting packets to go through"
            print "Sending packets from port 1 to port 2"
            send_packet(self, 0, str(pkt))
            print "Verifying packet on port 2"
            verify_packets(self, exp_pkt, [1])
            print "Packet received as expected\n\n"

            print "Sending packets from port 2 to port 1"
            send_packet(self, 1, str(pkt1))
            print "Verifying packet on port 1"
            verify_packets(self, exp_pkt1, [0])
            print "Packet received as expected"

            # setup ACL table group
            group_stage = SAI_ACL_STAGE_INGRESS
            group_bind_point_list1 = [SAI_ACL_BIND_POINT_TYPE_LAG]
            group_bind_point_list2 = [SAI_ACL_BIND_POINT_TYPE_VLAN]
            group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

            # create ACL table group
            print "Creating ACL Table group"
            acl_table_group_id1 = sai_thrift_create_acl_table_group(self.client,
                                                                        group_stage,
                                                                        group_bind_point_list1,
                                                                        group_type)

            acl_table_group_id2 = sai_thrift_create_acl_table_group(self.client,
                                                                        group_stage,
                                                                        group_bind_point_list2,
                                                                        group_type)

            # setup ACL to block based on Destination IP
            table_stage = SAI_ACL_STAGE_INGRESS
            table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_LAG, SAI_ACL_BIND_POINT_TYPE_VLAN]
            entry_priority = SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY
            action = SAI_PACKET_ACTION_DROP
            in_ports = None
            in_ports1 = None

            mac_src = None
            mac_dst = None
            mac_src_mask = None
            mac_dst_mask = None
            ip_proto = None
            in_port = None
            out_port = None
            out_ports = None
            src_l4_port = None
            dst_l4_port = None
            ingress_mirror_id = None
            egress_mirror_id = None

            ip_src1 = "10.10.10.1"
            ip_src_mask1 = "255.255.255.255"
            ip_dst_mask1 = None
            ip_dst1 = None
            ip_src3 = None
            ip_dst3 = "12.12.12.1"
            ip_src_mask2 = None
            ip_src2 = "192.168.0.1"
            ip_dst2 = None
            ip_src4 = None
            ip_dst4 = "11.11.11.1"
            ip_dst_mask2 = "255.255.255.255"

            group_member_priority = 1

            # create ACL table #1
            print "Creating ACL Table 1 for LAG src based"
            acl_table_id1 = sai_thrift_create_acl_table(
                                                        self.client,
                                                        table_stage,
                                                        table_bind_point_list,
                                                        addr_family,
                                                        mac_src,
                                                        mac_dst,
                                                        ip_src1,
                                                        ip_dst1,
                                                        ip_proto,
                                                        in_ports,
                                                        out_ports,
                                                        in_port,
                                                        out_port,
                                                        src_l4_port,
                                                        dst_l4_port)
            print "Creating ACL Entry 1"
            acl_entry_id1 = sai_thrift_create_acl_entry(
                                                        self.client,
                                                        acl_table_id1,
                                                        entry_priority,
                                                        action,
                                                        addr_family,
                                                        mac_src, mac_src_mask,
                                                        mac_dst, mac_dst_mask,
                                                        ip_src1, ip_src_mask1,
                                                        ip_dst1, ip_dst_mask1,
                                                        ip_proto,
                                                        in_ports, out_ports,
                                                        in_port, out_port,
                                                        src_l4_port, dst_l4_port,
                                                        ingress_mirror_id,
                                                        egress_mirror_id)

            # create ACL table #2
            print "Creating ACL Table 2 for VLAN for src based"
            acl_table_id2 = sai_thrift_create_acl_table(
                                                        self.client,
                                                        table_stage,
                                                        table_bind_point_list,
                                                        addr_family,
                                                        mac_src,
                                                        mac_dst,
                                                        ip_src2,
                                                        ip_dst2,
                                                        ip_proto,
                                                        in_ports1,
                                                        out_ports,
                                                        in_port,
                                                        out_port,
                                                        src_l4_port,
                                                        dst_l4_port)
            print "Creating ACL Entry 2 VLAN"
            acl_entry_id2 = sai_thrift_create_acl_entry(
                                                        self.client,
                                                        acl_table_id2,
                                                        entry_priority,
                                                        action, addr_family,
                                                        mac_src, mac_src_mask,
                                                        mac_dst, mac_dst_mask,
                                                        ip_src2, ip_src_mask1,
                                                        ip_dst2, ip_dst_mask1,
                                                        ip_proto,
                                                        in_ports1, out_ports,
                                                        in_port, out_port,
                                                        src_l4_port, dst_l4_port,
                                                        ingress_mirror_id,
                                                        egress_mirror_id)

            # create ACL table #3
            print "Creating ACL Table 3 for LAG for dst based"
            acl_table_id3 = sai_thrift_create_acl_table(
                                                        self.client,
                                                        table_stage,
                                                        table_bind_point_list,
                                                        addr_family,
                                                        mac_src, mac_dst,
                                                        ip_dst3, ip_src3,
                                                        ip_proto,
                                                        in_ports, out_ports,
                                                        in_port, out_port,
                                                        src_l4_port, dst_l4_port)
            print "Creating ACL Entry 3 for LAG"
            acl_entry_id3 = sai_thrift_create_acl_entry(
                                                        self.client,
                                                        acl_table_id3,
                                                        entry_priority,
                                                        action, addr_family,
                                                        mac_src, mac_src_mask,
                                                        mac_dst, mac_dst_mask,
                                                        ip_dst3, ip_dst_mask2,
                                                        ip_src3, ip_src_mask2,
                                                        ip_proto,
                                                        in_ports, out_ports,
                                                        in_port, out_port,
                                                        src_l4_port, dst_l4_port,
                                                        ingress_mirror_id,
                                                        egress_mirror_id)

            # create ACL table #4
            print "Creating ACL Table 4 for VLAN for dst based"
            acl_table_id4 = sai_thrift_create_acl_table(
                                                        self.client,
                                                        table_stage,
                                                        table_bind_point_list,
                                                        addr_family,
                                                        mac_src,
                                                        mac_dst,
                                                        ip_dst4,
                                                        ip_src4,
                                                        ip_proto,
                                                        in_ports1,
                                                        out_ports,
                                                        in_port,
                                                        out_port,
                                                        src_l4_port,
                                                        dst_l4_port)
            print "Creating ACL Entry 4"
            acl_entry_id4 = sai_thrift_create_acl_entry(
                                                        self.client,
                                                        acl_table_id4,
                                                        entry_priority,
                                                        action, addr_family,
                                                        mac_src, mac_src_mask,
                                                        mac_dst, mac_dst_mask,
                                                        ip_dst4, ip_dst_mask2,
                                                        ip_src4, ip_src_mask2,
                                                        ip_proto,
                                                        in_ports1, out_ports,
                                                        in_port, out_port,
                                                        src_l4_port, dst_l4_port,
                                                        ingress_mirror_id,
                                                        egress_mirror_id)

            # create ACL table group members
            print "Creating ACL Group members"
            acl_table_group_member_id1 = sai_thrift_create_acl_table_group_member(self.client,  # lag src based
                                                                                  acl_table_group_id1,
                                                                                  acl_table_id1,
                                                                                  group_member_priority)
            acl_table_group_member_id2 = sai_thrift_create_acl_table_group_member(self.client,  # vlan dst based
                                                                                  acl_table_group_id2,
                                                                                  acl_table_id4,
                                                                                  group_member_priority)
            acl_table_group_member_id3 = sai_thrift_create_acl_table_group_member(self.client,  # lag dst based
                                                                                  acl_table_group_id1,
                                                                                  acl_table_id3,
                                                                                  group_member_priority)
            acl_table_group_member_id4 = sai_thrift_create_acl_table_group_member(self.client,  # vlan src based
                                                                                  acl_table_group_id2,
                                                                                  acl_table_id2,
                                                                                  group_member_priority)

            print "Binding ACL to LAG"
            attr_value = sai_thrift_attribute_value_t(oid=acl_table_group_id1)
            attr = sai_thrift_attribute_t(id=SAI_LAG_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_lag_attribute(lag_id1, attr)

            # bind this ACL table to port1s object id
            print "Binding ACL to VLAN"
            attr_value = sai_thrift_attribute_value_t(oid=acl_table_group_id2)
            attr = sai_thrift_attribute_t(id=SAI_VLAN_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_vlan_attribute(vlan_oid, attr)

            print "Asserting ACL table entries, need to add more assertions"
            assert acl_table_group_id1 > 0, 'acl_table_group_id1 is <= 0'
            assert acl_table_group_id2 > 0, 'acl_table_group_id2 is <= 0'
            assert acl_table_id1 > 0, 'acl_entry_id1 is <= 0'
            assert acl_entry_id1 > 0, 'acl_entry_id1 is <= 0'
            assert acl_table_id2 > 0, 'acl_entry_id2 is <= 0'
            assert acl_entry_id2 > 0, 'acl_entry_id2 is <= 0'
            assert acl_table_group_member_id1 > 0, 'acl_table_group_member_id1 is <= 0'
            assert acl_table_group_member_id2 > 0, 'acl_table_group_member_id2 is <= 0'

            print '#### ACL \'DROP, dst 11.11.11.1/255.255.255.0, in_ports[ptf_intf_1]\' Applied ####'
            print '#### Sending      ', router_mac, '| 00:11:22:33:44:55 | 10.10.10.1 | 11.11.11.1 | @ ptf_intf 1'
            # send the same packet
            send_packet(self, 0, str(pkt))
            # ensure packet is dropped
            # check for absence of packet here!
            print '#### NOT Expecting 00:11:22:33:44:56 |', router_mac, '| 10.10.10.1 | 11.11.11.1 | @ ptf_intf 2\n'
            verify_no_packet(self, exp_pkt, 1)

            print '#### Sending      ', router_mac, '| 00:11:22:33:44:56 | 10.10.10.1 | 11.11.11.1 | @ ptf_intf 2'
            send_packet(self, 1, str(pkt1))
            # ensure packet is dropped
            # check for absence of packet here!
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 10.10.10.1 | 11.11.11.1 | @ ptf_intf 1'
            verify_no_packet(self, exp_pkt1, 0)

            print "Unconfig ACL part"
            # unbind this ACL table from port2s object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_VLAN_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_vlan_attribute(vlan_oid, attr)
            # unbind this ACL table from port1s object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_LAG_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_lag_attribute(lag_id1, attr)

            print "Verify the traffic after ACL is removed: Expecting packets to go through"
            print "Sending packets from port 1 to port 2"
            send_packet(self, 0, str(pkt))
            print "Verifying packet"
            verify_packets(self, exp_pkt, [1])
            print "Packet received as expected"

            print "Sending packets from port 2 to port 1"
            send_packet(self, 1, str(pkt1))
            print "Verifying packet"
            verify_packets(self, exp_pkt1, [0])
            print "Packet received as expected"

        finally:
            # cleanup ACL
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id1)
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id2)
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id3)
            self.client.sai_thrift_remove_acl_table_group_member(acl_table_group_member_id4)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id1)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id2)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id3)
            self.client.sai_thrift_remove_acl_entry(acl_entry_id4)
            self.client.sai_thrift_remove_acl_table(acl_table_id1)
            self.client.sai_thrift_remove_acl_table(acl_table_id2)
            self.client.sai_thrift_remove_acl_table(acl_table_id3)
            self.client.sai_thrift_remove_acl_table(acl_table_id4)
            self.client.sai_thrift_remove_acl_table_group(acl_table_group_id1)
            self.client.sai_thrift_remove_acl_table_group(acl_table_group_id2)

            sai_thrift_delete_fdb(self.client, vlan_oid, dmac2, port2)

            # Cleanup router config
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr_subnet1, ip_mask, nhop1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr_subnet2, ip_mask, nhop2)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask, rif_id1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr2_subnet, ip_mask2, rif_id2)

            self.client.sai_thrift_remove_next_hop(nhop1)
            self.client.sai_thrift_remove_next_hop(nhop2)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id2, ip_addr2, dmac2)
            sai_thrift_remove_lag_member(self.client, lag_member_id1)
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_lag(lag_id1)
            self.client.sai_thrift_remove_virtual_router(vr_id)
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('acl')
class L3AclTableTestII(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        Description:
        Same as "L3AclTableTest_I" for bind point type VLAN. Create ACL
        entry for the traffic over VLAN in this case.

        After "L3AclTableTest_II", bind the same ACL table to a port.
        Test is to verify dynamically changing the bind point type of an ACL table from VLAN to a PORT.
        Verify that the bind point can be changed, and the ACL action is only applied on the associated port but not the VLAN.
        Remove all the table bindings and verify that traffic is now forwarded for the VLAN and PORT

        Steps:
           1. Create VLAN 100 and associate untagged port 2, 3 as the member port of VLAN 100.
           2. Set port attribute value of "Port VLAN ID 100" for the port 2, 3.
           3. Create Virtual router V1 and enable V4.
           4. Create LAG "Id1"
           5. Remove the port 1 from default VLAN and add the port 1 as member of LAG Id1.
           6. Create two virtual router interfaces and set the interface type as "LAG" and "VLAN" for port 1 and (2,3) respectively.
           7. Create IPv4 neighbor entry (10.10.10.1) with MAC1 and associate with "RIF Id1".
           8. Create next hop to reach the neighbor entry.
           9. Create route entry with /24 mask (i.e 12.12.12.0/24) through NHop1
           10. Send IPv4 packet from port 2 (VLAN member) with destination IP 12.12.12.1

        Part1:
           11. Create ACL table and entry block the traffic based on Destination IP address (12.12.12.1)
           12. Bind the ACL table with VLAN with attribute SAI_VLAN_ATTR_INGRESS_ACL
           13. Send the IP packet again through port 2 and ensure the packet is dropped.
        Part2:
           14. Change the bind point from VLAN to the port 2
           15. Send the IP packet again through port 2 and ensure the packet is dropped.
           16. Send the IP packet again through port 3 and ensure the packet is received on port 1 (LAG)
           17. Remove the ACL attribute and send the packet from port 2 and from port 3 (VLAN) and verify the packet is received on
               port 1 (LAG)
        Part3:
           18. Remove the ACL table and entry.
           19. Remove the route and the associated router interface, vlan members and the corresponding VLAN from the database.
        """
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        v4_enabled = 1
        v6_enabled = 0
        vlan_id = 100
        mac_action = SAI_PACKET_ACTION_FORWARD
        mac1 = ''

        # Creating VLAN ID
        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        # Creating Virtual router
        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        lag_id1 = self.client.sai_thrift_create_lag([])
        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, [port1])
        lag_member_id1 = sai_thrift_create_lag_member(self.client, lag_id1, port1)

        # Creating RIF
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, lag_id1, 0, v4_enabled, v6_enabled, mac1)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan_oid, v4_enabled, v6_enabled, mac1)

        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '10.10.10.1'
        ip_mask1 = '255.255.255.0'
        ip_addr1_subnet = '10.10.0.0'
        ip_mask = '255.255.0.0'
        dmac1 = '00:11:22:33:44:55'
        ip_addr2 = '192.168.0.1'
        ip_addr3_subnet = '12.12.12.0'
        dmac2 = '00:11:22:33:44:56'

        # Creating Neighbor, NHop and route
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        nhop1 = sai_thrift_create_nhop(self.client, addr_family, ip_addr1, rif_id1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr3_subnet, ip_mask1, nhop1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask, rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(pktlen=100,
                                eth_dst=router_mac,
                                eth_src=dmac2,
                                ip_src=ip_addr2,
                                ip_dst='12.12.12.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(
                                pktlen=100,
                                eth_dst=dmac1,
                                eth_src=router_mac,
                                ip_src=ip_addr2,
                                ip_dst='12.12.12.1',
                                ip_id=105,
                                ip_ttl=63)

        acl_entry_id = SAI_NULL_OBJECT_ID
        acl_table_id = SAI_NULL_OBJECT_ID

        try:
            print "Sending packet port 2 -> port 1 (192.168.0.1 -> 12.12.12.1 [id = 105])"
            send_packet(self, 1, str(pkt))
            print "Verifying packet"
            verify_packets(self, exp_pkt, [0])
            print "Expected packets received"

            print "Sending packet port 3 -> port 1"
            send_packet(self, 2, str(pkt))
            print "Verifying packet at port 1"
            verify_packets(self, exp_pkt, [0])

            # setup ACL to block based on Destination IP
            print "Setting up ACL"
            table_stage = SAI_ACL_STAGE_INGRESS
            table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_VLAN, SAI_ACL_BIND_POINT_TYPE_PORT]
            entry_priority = SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY
            action = SAI_PACKET_ACTION_DROP
            in_ports = None
            mac_src = None
            mac_dst = None
            mac_src_mask = None
            mac_dst_mask = None
            ip_src = None
            ip_src_mask = None
            ip_dst = "12.12.12.1"
            ip_dst_mask = "255.255.255.0"
            ip_proto = None
            in_port = None
            out_port = None
            out_ports = None
            src_l4_port = None
            dst_l4_port = None
            ingress_mirror_id = None
            egress_mirror_id = None


            acl_table_id = sai_thrift_create_acl_table(
                                                self.client,
                                                table_stage,
                                                table_bind_point_list,
                                                addr_family,
                                                mac_src,
                                                mac_dst,
                                                ip_src,
                                                ip_dst,
                                                ip_proto,
                                                in_ports,
                                                out_ports,
                                                in_port,
                                                out_port,
                                                src_l4_port,
                                                dst_l4_port)
            acl_entry_id = sai_thrift_create_acl_entry(
                                                self.client,
                                                acl_table_id,
                                                entry_priority,
                                                action,
                                                addr_family,
                                                mac_src,
                                                mac_src_mask,
                                                mac_dst,
                                                mac_dst_mask,
                                                ip_src,
                                                ip_src_mask,
                                                ip_dst,
                                                ip_dst_mask,
                                                ip_proto,
                                                in_ports,
                                                out_ports,
                                                in_port,
                                                out_port,
                                                src_l4_port,
                                                dst_l4_port,
                                                ingress_mirror_id,
                                                egress_mirror_id)

            # bind this ACL table to VLAN object id
            print "Binding ACL to VLAN"
            attr_value = sai_thrift_attribute_value_t(oid=acl_table_id)
            attr = sai_thrift_attribute_t(id=SAI_VLAN_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_vlan_attribute(vlan_oid, attr)

            print "Asserting ACL table entry"
            assert acl_table_id > 0, 'acl_entry_id is <= 0'
            assert acl_entry_id > 0, 'acl_entry_id is <= 0'
            print '#### ACL \'DROP, src 12.12.12.1/255.255.255.0, in_ports[ptf_intf_1]\' Applied ####'
            print '#### Sending      ', router_mac, '| 00:11:22:33:44:56 | 192.168.0.1 | 12.12.12.1 | @ ptf_intf 2'
            # send the same packet
            send_packet(self, 1, str(pkt))
            # ensure packet is dropped
            # check for absence of packet here!
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 12.12.12.1 | 192.168.0.1 | @ ptf_intf 1'
            verify_no_packet(self, exp_pkt, 0)
            print "ACL blocked packet as expected"

            print "Sending packet port 3 -> port 1"
            send_packet(self, 2, str(pkt))
            print "Verifying packet at port 1"
            verify_no_packet(self, exp_pkt, 0)


            # ===== L3AclTableTest_III verification =======
            print "\n\nChanging bind type from VLAN to Port"
            # Unbinding from vlan
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_VLAN_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_vlan_attribute(vlan_oid, attr)

            #binding acl  to port
            attr_value = sai_thrift_attribute_value_t(oid=acl_table_id)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            print "Asserting ACL table entry"
            assert acl_table_id > 0, 'acl_entry_id is <= 0'
            assert acl_entry_id > 0, 'acl_entry_id is <= 0'
            print '#### ACL \'DROP, src 12.12.12.1/255.255.255.0, in_ports[ptf_intf_2]\' Applied ####'

            print "Sending packet from port 3 to port 1"
            send_packet(self, 2, str(pkt))
            print '#### Expecting 00:11:22:33:44:55 |', router_mac, '| 12.12.12.1 | 192.168.0.1 | @ ptf_intf 1'
            verify_packets(self, exp_pkt, [0])

            print "Sending packet from port 2 to port 1"
            send_packet(self, 1, str(pkt))
            print '#### NOT Expecting 00:11:22:33:44:55 |', router_mac, '| 12.12.12.1 | 192.168.0.1 | @ ptf_intf 1'
            verify_no_packet(self, exp_pkt, 0)

            print "Removing the ACL entry and table"
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            # unbind this ACL table from vlan object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_VLAN_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_vlan_attribute(vlan_oid, attr)

            print "Sending packet port 2 -> port 1 (192.168.0.1 -> 12.12.12.1 [id = 105])"
            send_packet(self, 1, str(pkt))
            print "Verifying packet"
            verify_packets(self, exp_pkt, [0])

            print "Sending packet port 3 -> port 1"
            send_packet(self, 2, str(pkt))
            print "Verifying packet at port 1"
            verify_packets(self, exp_pkt, [0])

        finally:
            # unbind this ACL table from port and vlan object id
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_VLAN_ATTR_INGRESS_ACL, value=attr_value)
            self.client.sai_thrift_set_vlan_attribute(vlan_oid, attr)

            sai_thrift_delete_fdb(self.client, vlan_oid, dmac2, port1)

            # cleanup ACL
            self.client.sai_thrift_remove_acl_entry(acl_entry_id)
            self.client.sai_thrift_remove_acl_table(acl_table_id)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr3_subnet, ip_mask1, nhop1)
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask, rif_id1)
            self.client.sai_thrift_remove_next_hop(nhop1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
            sai_thrift_remove_lag_member(self.client, lag_member_id1)
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)
            self.client.sai_thrift_remove_lag(lag_id1)
            self.client.sai_thrift_remove_virtual_router(vr_id)
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)


