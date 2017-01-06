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
        print '----------------------------------------------------------------------------------------------'
        print "Sending packet ptf_intf 1 -> ptf_intf 2, ptf_intf 3 (local mirror)"
        print "Sending packet ptf_intf 2 -> ptf_intf 1, ptf_intf 3 (local mirror)"

        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[1]
        port2 = port_list[2]
        port3 = port_list[3]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        sai_thrift_create_fdb(self.client, vlan_id, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_id, mac2, port2, mac_action)

        # setup local mirror session
        mirror_type = SAI_MIRROR_SESSION_TYPE_LOCAL
        monitor_port = port3
        print "Create mirror session: mirror_type = SAI_MIRROR_TYPE_LOCAL, monitor_port = ptf_intf 3 "
        ingress_mirror_id = sai_thrift_create_mirror_session(self.client,
            mirror_type,
            monitor_port,
            None, None, None,
            None, None, None,
            None, None, None,
            None, None, None)
        print "ingress_mirror_id = %d" %ingress_mirror_id

        attr_value = sai_thrift_attribute_value_t(oid=ingress_mirror_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        try:
            assert ingress_mirror_id > 0, 'ingress_mirror_id is <= 0'

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

            print '#### Sending 00:22:22:22:22:22 | 00:11:11:11:11:11 | 10.0.0.1 | 192.168.0.1 | @ ptf_intf 1 ####'
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

            print '#### Sending 00:11:11:11:11:11 | 00:22:22:22:22:22 | 10.0.0.1 | 192.168.0.1 | @ ptf_intf 2 ####'
            send_packet(self, 2, str(pkt))
            verify_each_packet_on_each_port(self, [exp_pkt, pkt], [1, 3])

        finally:
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_mirror_session(ingress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_id, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_id, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('mirror')
class IngressRSpanMirrorTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print '----------------------------------------------------------------------------------------------'
        print "Sending packet ptf_intf 1 -> ptf_intf 2, ptf_intf 3 (rspan mirror)"
        print "Sending packet ptf_intf 2 -> ptf_intf 1, ptf_intf 3 (rspan mirror)"

        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[1]
        port2 = port_list[2]
        port3 = port_list[3]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        sai_thrift_create_fdb(self.client, vlan_id, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_id, mac2, port2, mac_action)

        # setup remote mirror session
        mirror_type = SAI_MIRROR_SESSION_TYPE_REMOTE
        monitor_port = port3
        vlan = vlan_id
        print "Create mirror session: mirror_type = SAI_MIRROR_TYPE_LOCAL, monitor_port = ptf_intf 3 "
        ingress_mirror_id = sai_thrift_create_mirror_session(self.client,
            mirror_type,
            monitor_port,
            vlan, None, None,
            None, None, None,
            None, None, None,
            None, None, None)
        print "ingress_mirror_id = %d" %ingress_mirror_id

        attr_value = sai_thrift_attribute_value_t(oid=ingress_mirror_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        try:
            assert ingress_mirror_id > 0, 'ingress_mirror_id is <= 0'

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

            print '#### Sending 00:22:22:22:22:22 | 00:11:11:11:11:11 | 10.0.0.1 | 192.168.0.1 | @ ptf_intf 1 ####'
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

            print '#### Sending 00:11:11:11:11:11 | 00:22:22:22:22:22 | 10.0.0.1 | 192.168.0.1 | @ ptf_intf 2 ####'
            send_packet(self, 2, str(pkt))
            verify_each_packet_on_each_port(self, [exp_pkt, pkt], [1, 3])

        finally:
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_mirror_session(ingress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_id, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_id, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('mirror')
class IngressERSpanMirrorTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print '----------------------------------------------------------------------------------------------'
        print "Sending packet ptf_intf 1 -> ptf_intf 2, ptf_intf 3 (erspan mirror)"

        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[1]
        port2 = port_list[2]
        port3 = port_list[3]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        sai_thrift_create_fdb(self.client, vlan_id, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_id, mac2, port2, mac_action)

        # setup enhanced remote mirror session
        mirror_type = SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
        monitor_port = port3
        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        tunnel_src_ip = "1.1.1.1"
        tunnel_dst_ip = "1.1.1.2"
        tunnel_src_mac = router_mac
        tunnel_dst_mac = "00:33:33:33:33:33"
        encap_type = SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL
        protocol = 47

        print "Create mirror session: mirror_type = SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE, monitor_port = ptf_intf 3 "
        ingress_mirror_id = sai_thrift_create_mirror_session(self.client,
            mirror_type,
            monitor_port,
            None, None, None,
            tunnel_src_mac, tunnel_dst_mac,
            addr_family, tunnel_src_ip, tunnel_dst_ip,
            encap_type, protocol, None, None)
        print "ingress_mirror_id = %d" %ingress_mirror_id

        attr_value = sai_thrift_attribute_value_t(oid=ingress_mirror_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        try:
            assert ingress_mirror_id > 0, 'ingress_mirror_id is <= 0'

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
            print '#### Sending 00:22:22:22:22:22 | 00:11:11:11:11:11 | 10.0.0.1 | 192.168.0.1 | @ ptf_intf 1 ####'
            send_packet(self, 1, str(pkt))
            verify_packet(self, exp_pkt, 2)
            verify_packet(self, exp_mirrored_pkt, 3)

        finally:
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)

            self.client.sai_thrift_remove_mirror_session(ingress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_id, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_id, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('mirror')
class EgressLocalMirrorTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print '----------------------------------------------------------------------------------------------'
        print "Sending packet ptf_intf 1 -> ptf_intf 2, ptf_intf 3 (local mirror)"

        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[1]
        port2 = port_list[2]
        port3 = port_list[3]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        sai_thrift_create_fdb(self.client, vlan_id, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_id, mac2, port2, mac_action)

        # setup local mirror session
        mirror_type = SAI_MIRROR_SESSION_TYPE_LOCAL
        monitor_port = port3
        print "Create mirror session: mirror_type = SAI_MIRROR_TYPE_LOCAL, monitor_port = ptf_intf 3 "
        egress_mirror_id = sai_thrift_create_mirror_session(self.client,
            mirror_type,
            monitor_port,
            None, None, None,
            None, None, None,
            None, None, None,
            None, None, None)
        print "egress_mirror_id = %d" %egress_mirror_id

        attr_value = sai_thrift_attribute_value_t(oid=egress_mirror_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        try:
            assert egress_mirror_id > 0, 'egress_mirror_id is <= 0'

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

            print '#### Sending 00:22:22:22:22:22 | 00:11:11:11:11:11 | 10.0.0.1 | 192.168.0.1 | @ ptf_intf 1 ####'
            send_packet(self, 1, str(pkt))
            verify_each_packet_on_each_port(self, [exp_pkt, exp_pkt], [2, 3])

        finally:
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_mirror_session(egress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_id, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_id, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('mirror')
class EgressERSpanMirrorTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print '----------------------------------------------------------------------------------------------'
        print "Sending packet ptf_intf 1 -> ptf_intf 2, ptf_intf 3 (erspan mirror)"

        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[1]
        port2 = port_list[2]
        port3 = port_list[3]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        sai_thrift_create_fdb(self.client, vlan_id, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_id, mac2, port2, mac_action)

        # setup enhanced remote mirror session
        mirror_type = SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
        monitor_port = port3
        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        tunnel_src_ip = "1.1.1.1"
        tunnel_dst_ip = "1.1.1.2"
        tunnel_src_mac = router_mac
        tunnel_dst_mac = "00:33:33:33:33:33"
        encap_type = SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL
        protocol = 47

        print "Create mirror session: mirror_type = SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE, monitor_port = ptf_intf 3 "
        egress_mirror_id = sai_thrift_create_mirror_session(self.client,
            mirror_type,
            monitor_port,
            None, None, None,
            tunnel_src_mac, tunnel_dst_mac,
            addr_family, tunnel_src_ip, tunnel_dst_ip,
            encap_type, protocol, None, None)
        print "egress_mirror_id = %d" %egress_mirror_id

        attr_value = sai_thrift_attribute_value_t(oid=egress_mirror_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        try:
            assert egress_mirror_id > 0, 'egress_mirror_id is <= 0'

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
                mirror_id=(egress_mirror_id & 0x3FFFFFFF),
                inner_frame=pkt);
            print '#### Sending 00:22:22:22:22:22 | 00:11:11:11:11:11 | 10.0.0.1 | 192.168.0.1 | @ ptf_intf 1 ####'
            send_packet(self, 1, str(pkt))
            verify_packet(self, exp_pkt, 2)
            verify_packet(self, exp_mirrored_pkt, 3)

        finally:
            attr_value = sai_thrift_attribute_value_t(oid=SAI_NULL_OBJECT_ID)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_mirror_session(egress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_id, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_id, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)
