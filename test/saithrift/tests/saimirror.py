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
Thrift SAI interface MIRROR tests
"""

from switch import *
import sai_base_test

@group('mirror')
class IngressLocalMirrorTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[1]
        port2 = port_list[2]
        port3 = port_list[3]
        v4_enabled = 1
        v6_enabled = 1
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        self.client.sai_thrift_create_vlan(vlan_id)

        vlan_port1 = sai_thrift_vlan_port_t(port_id=port1, tagging_mode=SAI_VLAN_PORT_UNTAGGED)
        vlan_port2 = sai_thrift_vlan_port_t(port_id=port2, tagging_mode=SAI_VLAN_PORT_TAGGED)
        self.client.sai_thrift_add_ports_to_vlan(vlan_id, [vlan_port1, vlan_port2])

        sai_thrift_create_fdb(self.client, vlan_id, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_id, mac2, port2, mac_action)

        action = 2 #Ingress Mirror
        in_ports = [port1, port2]
        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_src = "192.168.0.1"
        ip_src_mask = "255.255.255.255"

        ip_dst = None
        ip_dst_mask = None
        ip_proto = None
        in_port = None
        out_port = None
        out_ports = None
        egress_mirror_id = None

        mirror_type = SAI_MIRROR_TYPE_LOCAL
        ingress_mirror_id = sai_thrift_create_mirror_session(self.client, mirror_type, port3,
                                                     0, 0, 0,
                                                     None, None,
                                                     0, None, None,
                                                     0, 0, 0, 0)

        acl_table_id = sai_thrift_create_acl_table(self.client, addr_family,
                                                   ip_src, ip_dst,
                                                   ip_proto,
                                                   in_ports, out_ports,
                                                   in_port, out_port)

        acl_entry_id = sai_thrift_create_acl_entry(self.client, acl_table_id,
                                                   action, addr_family,
                                                   ip_src, ip_src_mask,
                                                   ip_dst, ip_dst_mask,
                                                   ip_proto,
                                                   in_ports, out_ports,
                                                   in_port, out_port,
                                                   ingress_mirror_id,
                                                   egress_mirror_id)

        try:
            pkt = simple_tcp_packet(eth_dst=mac2,
                                eth_src=mac1,
                                ip_dst='10.0.0.1',
                                ip_src='192.168.0.1',
                                ip_id=102,
                                ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_dst=mac2,
                                eth_src=mac1,
                                ip_dst='10.0.0.1',
                                ip_src='192.168.0.1',
                                dl_vlan_enable=True,
                                vlan_vid=10,
                                ip_id=102,
                                ip_ttl=64,
                                pktlen=104)

            print "Sending packet port 1 -> port 2 and port 3 (local mirror)"
            send_packet(self, 1, str(pkt))
            verify_each_packet_on_each_port(self, [exp_pkt, pkt], [2, 3])

            time.sleep(1)

            pkt = simple_tcp_packet(eth_dst=mac1,
                                eth_src=mac2,
                                ip_dst='10.0.0.1',
                                ip_src='192.168.0.1',
                                vlan_vid=10,
                                dl_vlan_enable=True,
                                ip_id=102,
                                ip_ttl=64,
                                pktlen=104)
            exp_pkt = simple_tcp_packet(eth_dst=mac1,
                                eth_src=mac2,
                                ip_dst='10.0.0.1',
                                ip_src='192.168.0.1',
                                ip_id=102,
                                ip_ttl=64,
                                pktlen=100)

            print "Sending packet port 2 -> port 1 and port 3 (local mirror)"
            send_packet(self, 2, str(pkt))
            verify_each_packet_on_each_port(self, [exp_pkt, pkt], [1, 3])

        finally:
            self.client.sai_thrift_delete_acl_entry(acl_entry_id)
            self.client.sai_thrift_delete_acl_table(acl_table_id)

            self.client.sai_thrift_remove_mirror_session(ingress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_id, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_id, mac2, port2)

            self.client.sai_thrift_remove_ports_from_vlan(vlan_id, [vlan_port1, vlan_port2])
            self.client.sai_thrift_delete_vlan(vlan_id)

@group('mirror')
class IngressERSpanMirrorTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[1]
        port2 = port_list[2]
        port3 = port_list[3]
        v4_enabled = 1
        v6_enabled = 1
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        self.client.sai_thrift_create_vlan(vlan_id)

        vlan_port1 = sai_thrift_vlan_port_t(port_id=port1, tagging_mode=SAI_VLAN_PORT_UNTAGGED)
        vlan_port2 = sai_thrift_vlan_port_t(port_id=port2, tagging_mode=SAI_VLAN_PORT_TAGGED)
        self.client.sai_thrift_add_ports_to_vlan(vlan_id, [vlan_port1, vlan_port2])

        sai_thrift_create_fdb(self.client, vlan_id, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_id, mac2, port2, mac_action)

        action = 2 #Ingress Mirror
        in_ports = [port1, port2]
        ip_src = "192.168.0.1"
        ip_src_mask = "255.255.255.255"

        mirror_type = SAI_MIRROR_TYPE_ENHANCED_REMOTE
        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        tunnel_src_ip = "1.1.1.1"
        tunnel_dst_ip = "1.1.1.2"
        tunnel_src_mac = "00:77:66:55:44:33"
        tunnel_dst_mac = "00:33:33:33:33:33"
        encap_type = SAI_MIRROR_L3_GRE_TUNNEL
        protocol = 47

        ip_dst = None
        ip_dst_mask = None
        ip_proto = None
        in_port = None
        out_port = None
        out_ports = []
        egress_mirror_id = None

        ingress_mirror_id = sai_thrift_create_mirror_session(self.client, mirror_type, port3,
                                                     0, 0, 0,
                                                     tunnel_src_mac, tunnel_dst_mac,
                                                     addr_family, tunnel_src_ip, tunnel_dst_ip,
                                                     encap_type, protocol, 0, 0)

        acl_table_id = sai_thrift_create_acl_table(self.client, addr_family,
                                                   ip_src, ip_dst,
                                                   ip_proto,
                                                   in_ports, out_ports,
                                                   in_port, out_port)

        acl_entry_id = sai_thrift_create_acl_entry(self.client, acl_table_id,
                                                   action, addr_family,
                                                   ip_src, ip_src_mask,
                                                   ip_dst, ip_dst_mask,
                                                   ip_proto,
                                                   in_ports, out_ports,
                                                   in_port, out_port,
                                                   ingress_mirror_id,
                                                   egress_mirror_id)

        try:
            pkt = simple_tcp_packet(eth_dst=mac2,
                                eth_src=mac1,
                                ip_dst='10.0.0.1',
                                ip_src='192.168.0.1',
                                ip_id=102,
                                ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_dst=mac2,
                                eth_src=mac1,
                                ip_dst='10.0.0.1',
                                ip_src='192.168.0.1',
                                dl_vlan_enable=True,
                                vlan_vid=10,
                                ip_id=102,
                                ip_ttl=64,
                                pktlen=104)
            exp_mirrored_pkt = ipv4_erspan_pkt(eth_dst=tunnel_dst_mac,
                                           eth_src=tunnel_src_mac,
                                           ip_src=tunnel_src_ip,
                                           ip_dst=tunnel_dst_ip,
                                           ip_id=0,
                                           ip_ttl=64,
                                           version=2,
                                           mirror_id=(ingress_mirror_id & 0x3FFFFFFF),
                                           inner_frame=pkt);

            print "Sending packet port 1 -> port 2 and port 3 (erspan mirror)"
            send_packet(self, 1, str(pkt))
            verify_erspan3_packet(self, exp_mirrored_pkt, 3)
            verify_packets(self, exp_pkt, [2])
            verify_no_other_packets(self)

            time.sleep(1)

        finally:
            self.client.sai_thrift_delete_acl_entry(acl_entry_id)
            self.client.sai_thrift_delete_acl_table(acl_table_id)

            self.client.sai_thrift_remove_mirror_session(ingress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_id, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_id, mac2, port2)

            self.client.sai_thrift_remove_ports_from_vlan(vlan_id, [vlan_port1, vlan_port2])
            self.client.sai_thrift_delete_vlan(vlan_id)

@group('mirror')
class EgressLocalMirrorTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[1]
        port2 = port_list[2]
        port3 = port_list[3]
        v4_enabled = 1
        v6_enabled = 1
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        self.client.sai_thrift_create_vlan(vlan_id)

        vlan_port1 = sai_thrift_vlan_port_t(port_id=port1, tagging_mode=SAI_VLAN_PORT_UNTAGGED)
        vlan_port2 = sai_thrift_vlan_port_t(port_id=port2, tagging_mode=SAI_VLAN_PORT_TAGGED)
        self.client.sai_thrift_add_ports_to_vlan(vlan_id, [vlan_port1, vlan_port2])

        sai_thrift_create_fdb(self.client, vlan_id, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_id, mac2, port2, mac_action)

        action = 2 #Ingress Mirror
        addr_family = SAI_IP_ADDR_FAMILY_IPV4

        mirror_type = SAI_MIRROR_TYPE_LOCAL
        ip_src = None
        ip_src_mask = None
        ip_dst = None
        ip_dst_mask = None
        ip_proto = None
        in_port = None
        in_ports = None
        out_port = port2
        in_ports = []
        out_ports = []
        ingress_mirror_id = None

        egress_mirror_id = sai_thrift_create_mirror_session(self.client, mirror_type, port3,
                                                     0, 0, 0,
                                                     None, None,
                                                     0, None, None,
                                                     0, 0, 0, 0)

        acl_table_id = sai_thrift_create_acl_table(self.client, addr_family,
                                                   ip_src, ip_dst,
                                                   ip_proto,
                                                   in_ports, out_ports,
                                                   in_port, out_port)

        acl_entry_id = sai_thrift_create_acl_entry(self.client, acl_table_id,
                                                   action, addr_family,
                                                   ip_src, ip_src_mask,
                                                   ip_dst, ip_dst_mask,
                                                   ip_proto,
                                                   in_ports, out_ports,
                                                   in_port, out_port,
                                                   ingress_mirror_id,
                                                   egress_mirror_id)

        try:
            pkt = simple_tcp_packet(eth_dst=mac2,
                                eth_src=mac1,
                                ip_dst='10.0.0.1',
                                ip_src='192.168.0.1',
                                ip_id=102,
                                ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_dst=mac2,
                                eth_src=mac1,
                                ip_dst='10.0.0.1',
                                ip_src='192.168.0.1',
                                dl_vlan_enable=True,
                                vlan_vid=10,
                                ip_id=102,
                                ip_ttl=64,
                                pktlen=104)

            print "Sending packet port 1 -> port 2 and port 3 (local mirror)"
            send_packet(self, 1, str(pkt))
            verify_each_packet_on_each_port(self, [exp_pkt, exp_pkt], [2, 3])

            time.sleep(1)

        finally:
            self.client.sai_thrift_delete_acl_entry(acl_entry_id)
            self.client.sai_thrift_delete_acl_table(acl_table_id)

            self.client.sai_thrift_remove_mirror_session(egress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_id, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_id, mac2, port2)

            self.client.sai_thrift_remove_ports_from_vlan(vlan_id, [vlan_port1, vlan_port2])
            self.client.sai_thrift_delete_vlan(vlan_id)

@group('mirror')
class EgressERSpanMirrorTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[1]
        port2 = port_list[2]
        port3 = port_list[3]
        v4_enabled = 1
        v6_enabled = 1
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        self.client.sai_thrift_create_vlan(vlan_id)

        vlan_port1 = sai_thrift_vlan_port_t(port_id=port1, tagging_mode=SAI_VLAN_PORT_UNTAGGED)
        vlan_port2 = sai_thrift_vlan_port_t(port_id=port2, tagging_mode=SAI_VLAN_PORT_UNTAGGED)
        self.client.sai_thrift_add_ports_to_vlan(vlan_id, [vlan_port1, vlan_port2])

        sai_thrift_create_fdb(self.client, vlan_id, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_id, mac2, port2, mac_action)

        action = 2 #Ingress Mirror

        mirror_type = SAI_MIRROR_TYPE_ENHANCED_REMOTE
        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        tunnel_src_ip = "1.1.1.1"
        tunnel_dst_ip = "1.1.1.2"
        tunnel_src_mac = "00:77:66:55:44:33"
        tunnel_dst_mac = "00:33:33:33:33:33"
        encap_type = SAI_MIRROR_L3_GRE_TUNNEL
        protocol = 47

        ip_src = None
        ip_src_mask = None
        ip_dst = None
        ip_dst_mask = None
        ip_proto = None
        in_port = None
        out_port = None
        out_port = port2
        in_ports = []
        out_ports = []
        ingress_mirror_id = None

        egress_mirror_id = sai_thrift_create_mirror_session(self.client, mirror_type, port3,
                                                     0, 0, 0,
                                                     tunnel_src_mac, tunnel_dst_mac,
                                                     addr_family, tunnel_src_ip, tunnel_dst_ip,
                                                     encap_type, protocol, 0, 0)

        acl_table_id = sai_thrift_create_acl_table(self.client, addr_family,
                                                   ip_src, ip_dst,
                                                   ip_proto,
                                                   in_ports, out_ports,
                                                   in_port, out_port)

        acl_entry_id = sai_thrift_create_acl_entry(self.client, acl_table_id,
                                                   action, addr_family,
                                                   ip_src, ip_src_mask,
                                                   ip_dst, ip_dst_mask,
                                                   ip_proto,
                                                   in_ports, out_ports,
                                                   in_port, out_port,
                                                   ingress_mirror_id,
                                                   egress_mirror_id)

        try:
            pkt = simple_tcp_packet(eth_dst=mac2,
                                eth_src=mac1,
                                ip_dst='10.0.0.1',
                                ip_src='192.168.0.1',
                                ip_id=102,
                                ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_dst=mac2,
                                eth_src=mac1,
                                ip_dst='10.0.0.1',
                                ip_src='192.168.0.1',
                                ip_id=102,
                                ip_ttl=64)
            exp_mirrored_pkt = ipv4_erspan_pkt(eth_dst=tunnel_dst_mac,
                                           eth_src=tunnel_src_mac,
                                           ip_src=tunnel_src_ip,
                                           ip_dst=tunnel_dst_ip,
                                           ip_id=0,
                                           ip_ttl=64,
                                           version=2,
                                           mirror_id=(egress_mirror_id & 0x3FFFFFFF),
                                           inner_frame=pkt);

            print "Sending packet port 1 -> port 2 and port 3 (erspan mirror)"
            send_packet(self, 1, str(pkt))
            verify_erspan3_packet(self, exp_mirrored_pkt, 3)
            verify_packets(self, exp_pkt, [2])
            verify_no_other_packets(self)

            time.sleep(1)

        finally:
            self.client.sai_thrift_delete_acl_entry(acl_entry_id)
            self.client.sai_thrift_delete_acl_table(acl_table_id)

            self.client.sai_thrift_remove_mirror_session(egress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_id, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_id, mac2, port2)

            self.client.sai_thrift_remove_ports_from_vlan(vlan_id, [vlan_port1, vlan_port2])
            self.client.sai_thrift_delete_vlan(vlan_id)
