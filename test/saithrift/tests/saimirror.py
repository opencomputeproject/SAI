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
Thrift SAI interface Mirror tests
"""

import socket

from switch import *
from ptf.mask import Mask
import sai_base_test

@group('mirror')
class spanmonitor(sai_base_test.ThriftInterfaceDataPlane):
    '''
    This performs Local mirroring
    We set port2 traffic to be monitored(both ingress and egress) on port1
    We send a packet from port 2 to port 3
    We expect the same packet on port 1 which is a mirror packet
    '''
    def runTest(self):
        print
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        monitor_port=port1
        source_port=port2
        mac3='00:00:00:00:00:33'
        mac2='00:00:00:00:00:22'
        mirror_type=SAI_MIRROR_SESSION_TYPE_LOCAL
        mac_action = SAI_PACKET_ACTION_FORWARD
        vlan_remote_id = 2

        # Put ports under test in VLAN 2
        vlan_remote_oid = sai_thrift_create_vlan(self.client, vlan_remote_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_remote_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_remote_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member3 = sai_thrift_create_vlan_member(self.client, vlan_remote_oid, port3, SAI_VLAN_TAGGING_MODE_TAGGED)

        sai_thrift_create_fdb(self.client, vlan_remote_oid, mac3, port3, mac_action)
        sai_thrift_create_fdb(self.client, vlan_remote_oid, mac2, port2, mac_action)

        # Remove ports from default VLAN
        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, [port1, port2, port3])

        # Set PVID
        attr_value = sai_thrift_attribute_value_t(u16=vlan_remote_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        spanid=sai_thrift_create_mirror_session(self.client,mirror_type=mirror_type,port=monitor_port,vlan=None,vlan_priority=0,vlan_tpid=0,vlan_header_valid=False,src_mac=None,dst_mac=None,src_ip=None,dst_ip=None,encap_type=0,iphdr_version=0,ttl=0,tos=0,gre_type=0)
        attrb_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=1,object_id_list=[spanid]))

        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attrb_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attrb_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        pkt = simple_tcp_packet(eth_dst='00:00:00:00:00:33',
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.0.0.1',
                                dl_vlan_enable=True,
                                vlan_vid=2,
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst='00:00:00:00:00:33',
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.0.0.1',
                                dl_vlan_enable=True,
                                vlan_vid=2,
                                ip_id=101,
                                ip_ttl=64)

        pkt2 = simple_tcp_packet(eth_dst='00:00:00:00:00:22',
                                eth_src='00:33:33:33:33:33',
                                ip_dst='10.0.0.1',
                                dl_vlan_enable=True,
                                vlan_vid=2,
                                ip_id=101,
                                ip_ttl=64,
                                pktlen=104)

        exp_pkt2 = simple_tcp_packet(eth_dst='00:00:00:00:00:22',
                                eth_src='00:33:33:33:33:33',
                                ip_dst='10.0.0.1',
                                dl_vlan_enable=True,
                                vlan_vid=2,#use vlan_vid field if packets are expected to be monitored on client side otherwise not needed 
                                ip_id=101,
                                ip_ttl=64,
                                pktlen=104)

        m=Mask(exp_pkt2)
        m.set_do_not_care_scapy(ptf.packet.IP,'id')
        m.set_do_not_care_scapy(ptf.packet.IP,'chksum')
        try:
            # in tuple: 0 is device number, 2 is port number
            # this tuple uniquely identifies a port
            # for ingress mirroring
            print "Checking INGRESS Local Mirroring"
            print "Sending packet port 2 -> port 3 (00:22:22:22:22:22 -> 00:00:00:00:00:33)"
            send_packet(self, 1, pkt)
            verify_packets(self, exp_pkt, ports=[0,2])
            # for egress mirroring
            print "Checking EGRESS Local Mirroring"
            print "Sending packet port 3 -> port 2 (00:33:33:33:33:33 -> 00:00:00:00:00:22)"
            send_packet(self, 2, pkt2)
            verify_each_packet_on_each_port(self, [m,pkt2], ports=[0,1])
        finally:
            sai_thrift_delete_fdb(self.client, vlan_remote_oid, mac3, port3)
            sai_thrift_delete_fdb(self.client, vlan_remote_oid, mac2, port2)

            # Remove ports from mirror destination
            attrb_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=0,object_id_list=[spanid]))
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attrb_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attrb_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            # Now you can remove destination
            self.client.sai_thrift_remove_mirror_session(spanid)

            # Remove ports from VLAN 2
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan_member(vlan_member3)
            self.client.sai_thrift_remove_vlan(vlan_remote_oid)

            # Add ports back to default VLAN
            vlan_member1a = sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan_member2a = sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan_member3a = sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)



@group('mirror')
class erspanmonitor(sai_base_test.ThriftInterfaceDataPlane):
    '''
    This test performs erspan monitoring
    From port2(source port) we send traffic to port 3
    erspan mirror packets are expected on port 1(monitor port)
    '''
    def runTest(self):
        print
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        mac3='00:00:00:00:00:33'
        mac2='00:00:00:00:00:22'
        monitor_port=port1
        source_port=port2
        mirror_type=SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
        vlan=0x2
        vlan_tpid=0x8100
        vlan_pri=0x6
        src_mac='00:00:00:00:11:22'
        dst_mac='00:00:00:00:11:33'
        encap_type=SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL
        ip_version=0x4
        tos=0x3c
        ttl=0xf0
        gre_type=0x88be
        src_ip='17.18.19.0'
        dst_ip='33.19.20.0'
        addr_family=0
        vlan_remote_id = 3
        mac_action = SAI_PACKET_ACTION_FORWARD

        # Put ports under test in VLAN 3
        vlan_remote_oid = sai_thrift_create_vlan(self.client, vlan_remote_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_remote_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_remote_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member3 = sai_thrift_create_vlan_member(self.client, vlan_remote_oid, port3, SAI_VLAN_TAGGING_MODE_TAGGED)

        sai_thrift_create_fdb(self.client, vlan_remote_oid, mac3, port3, mac_action)
        sai_thrift_create_fdb(self.client, vlan_remote_oid, mac2, port2, mac_action)

        # Remove ports from default VLAN
        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, [port1, port2, port3])

        # Set PVID
        attr_value = sai_thrift_attribute_value_t(u16=vlan_remote_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        erspanid=sai_thrift_create_mirror_session(self.client,mirror_type=mirror_type,port=monitor_port,vlan=vlan,vlan_priority=vlan_pri,vlan_tpid=vlan_tpid,vlan_header_valid=True,src_mac=src_mac,dst_mac=dst_mac,src_ip=src_ip,dst_ip=dst_ip,encap_type=encap_type,iphdr_version=ip_version,ttl=ttl,tos=tos,gre_type=gre_type)

        attrb_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=1,object_id_list=[erspanid]))

        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attrb_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)


        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attrb_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        pkt = simple_tcp_packet(eth_dst='00:00:00:00:00:33',
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.0.0.1',
                                dl_vlan_enable=True,
                                vlan_vid=3,
                                ip_id=101,
                                ip_ttl=64)

        pkt2 = simple_tcp_packet(eth_dst='00:00:00:00:00:22',
                                eth_src='00:33:33:33:33:33',
                                dl_vlan_enable=True,
                                vlan_vid=3,
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        pkt3 = simple_tcp_packet(eth_dst='00:00:00:00:00:22',
                                eth_src='00:33:33:33:33:33',
                                dl_vlan_enable=True,
                                vlan_vid=3,
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt1= ipv4_erspan_platform_pkt(pktlen=142,
                                    eth_dst='00:00:00:00:11:33',
                                    eth_src='00:00:00:00:11:22',
                                    dl_vlan_enable=True,
                                    vlan_vid=2,
                                    ip_id=0,
                                    ip_ttl=240,
                                    ip_tos=0x3c,
                                    ip_ihl=5,
                                    ip_src='17.18.19.0',
                                    ip_dst='33.19.20.0',
                                    version=2,
                                    mirror_id=(erspanid & 0x3FFFFFFF),
                                    inner_frame=pkt
                                    )

        exp_pkt2= ipv4_erspan_platform_pkt(pktlen=142,
                                    eth_dst='00:00:00:00:11:33',
                                    eth_src='00:00:00:00:11:22',
                                    dl_vlan_enable=True,
                                    vlan_vid=2,
                                    ip_id=0,
                                    ip_ttl=240,
                                    ip_tos=0x3c,
                                    ip_ihl=5,
                                    ip_src='17.18.19.0',
                                    ip_dst='33.19.20.0',
                                    version=2,
                                    mirror_id=(erspanid & 0x3FFFFFFF),
                                    inner_frame=pkt3
                                    )
        m1=Mask(exp_pkt1)
        m1.set_do_not_care_scapy(ptf.packet.IP,'tos')
        m1.set_do_not_care_scapy(ptf.packet.IP,'frag')
        m1.set_do_not_care_scapy(ptf.packet.IP,'flags')
        m1.set_do_not_care_scapy(ptf.packet.IP,'chksum')
        m1.set_do_not_care_scapy(ptf.packet.GRE,'proto')
        m1.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'platf_id')
        m1.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info1')
        m1.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info2')
            
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'span_id')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'timestamp')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'sgt_other')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'direction')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'version')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'vlan')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'priority')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'truncated')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'unknown2')

        m2=Mask(exp_pkt2)
        m2.set_do_not_care_scapy(ptf.packet.IP,'tos')
        m2.set_do_not_care_scapy(ptf.packet.IP,'frag')
        m2.set_do_not_care_scapy(ptf.packet.IP,'flags')
        m2.set_do_not_care_scapy(ptf.packet.IP,'chksum')
        m2.set_do_not_care_scapy(ptf.packet.GRE,'proto')
        m2.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'platf_id')
        m2.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info1')
        m2.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info2')
            
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'span_id')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'timestamp')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'sgt_other')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'direction')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'version')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'vlan')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'priority')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'truncated')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'unknown2')

        n=Mask(pkt2)
        n.set_do_not_care_scapy(ptf.packet.IP,'len')
        n.set_do_not_care_scapy(ptf.packet.IP,'chksum')

        try:
            # in tuple: 0 is device number, 2 is port number
            # this tuple uniquely identifies a port
            # for ingress mirroring
            print "Checking INGRESS ERSPAN Mirroring"
            print "Sending packet port 2 -> port 3 (00:22:22:22:22:22 -> 00:00:00:00:00:33)"
            send_packet(self, 1, pkt)
            verify_each_packet_on_each_port(self, [m1,pkt], ports=[0,2])#FIXME need to properly implement
            # for egress mirroring
            print "Checking EGRESS ERSPAN Mirroring"
            print "Sending packet port 3 -> port 2 (00:33:33:33:33:33 -> 00:00:00:00:00:22)"
            send_packet(self, 2, pkt2)
            verify_each_packet_on_each_port(self, [pkt2,m2], ports=[1,0])#FIXME need to properly implement
        finally:
            sai_thrift_delete_fdb(self.client, vlan_remote_oid, mac2, port2)
            sai_thrift_delete_fdb(self.client, vlan_remote_oid, mac3, port3)

            # Remove ports from mirror destination
            attrb_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=0,object_id_list=[erspanid]))
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attrb_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attrb_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            # Now you can remove destination
            self.client.sai_thrift_remove_mirror_session(erspanid)

            # Remove ports from VLAN 3
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan_member(vlan_member3)
            self.client.sai_thrift_remove_vlan(vlan_remote_oid)

            # Add ports back to default VLAN
            vlan_member1a = sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan_member2a = sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan_member3a = sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)

@group('mirror')
class erspan_novlan_monitor(sai_base_test.ThriftInterfaceDataPlane):
    '''
    This test performs erspan monitoring
    From port2(source port) we send traffic to port 3
    erspan mirror packets are expected on port 1(monitor port)
    '''
    def runTest(self):
        print
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        mac3='00:00:00:00:00:33'
        mac2='00:00:00:00:00:22'
        monitor_port=port1
        source_port=port2
        mirror_type=SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
        src_mac='00:00:00:00:11:22'
        dst_mac='00:00:00:00:11:33'
        encap_type=SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL
        ip_version=0x4
        tos=0x3c
        ttl=0xf0
        gre_type=0x88be
        src_ip='17.18.19.0'
        dst_ip='33.19.20.0'
        addr_family=0
        vlan_remote_id = 3
        mac_action = SAI_PACKET_ACTION_FORWARD

        # Put ports under test in VLAN 3
        vlan_remote_oid = sai_thrift_create_vlan(self.client, vlan_remote_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_remote_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_remote_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member3 = sai_thrift_create_vlan_member(self.client, vlan_remote_oid, port3, SAI_VLAN_TAGGING_MODE_TAGGED)

        sai_thrift_create_fdb(self.client, vlan_remote_oid, mac3, port3, mac_action)
        sai_thrift_create_fdb(self.client, vlan_remote_oid, mac2, port2, mac_action)

        # Remove ports from default VLAN
        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, [port1, port2, port3])

        # Set PVID
        attr_value = sai_thrift_attribute_value_t(u16=vlan_remote_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        erspanid=sai_thrift_create_mirror_session(self.client,mirror_type=mirror_type,port=monitor_port,vlan=None,vlan_priority=None,vlan_tpid=None,vlan_header_valid=False,src_mac=src_mac,dst_mac=dst_mac,src_ip=src_ip,dst_ip=dst_ip,encap_type=encap_type,iphdr_version=ip_version,ttl=ttl,tos=tos,gre_type=gre_type)

        attrb_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=1,object_id_list=[erspanid]))

        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attrb_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)


        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attrb_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        pkt = simple_tcp_packet(eth_dst='00:00:00:00:00:33',
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.0.0.1',
                                dl_vlan_enable=True,
                                vlan_vid=3,
                                ip_id=101,
                                ip_ttl=64)

        pkt2 = simple_tcp_packet(eth_dst='00:00:00:00:00:22',
                                eth_src='00:33:33:33:33:33',
                                dl_vlan_enable=True,
                                vlan_vid=3,
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        pkt3 = simple_tcp_packet(eth_dst='00:00:00:00:00:22',
                                eth_src='00:33:33:33:33:33',
                                dl_vlan_enable=True,
                                vlan_vid=3,
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt1= ipv4_erspan_platform_pkt(pktlen=142,
                                    eth_dst='00:00:00:00:11:33',
                                    eth_src='00:00:00:00:11:22',
                                    ip_id=0,
                                    ip_ttl=240,
                                    ip_tos=0x3c,
                                    ip_ihl=5,
                                    ip_src='17.18.19.0',
                                    ip_dst='33.19.20.0',
                                    version=2,
                                    mirror_id=(erspanid & 0x3FFFFFFF),
                                    inner_frame=pkt
                                    )

        exp_pkt2= ipv4_erspan_platform_pkt(pktlen=142,
                                    eth_dst='00:00:00:00:11:33',
                                    eth_src='00:00:00:00:11:22',
                                    ip_id=0,
                                    ip_ttl=240,
                                    ip_tos=0x3c,
                                    ip_ihl=5,
                                    ip_src='17.18.19.0',
                                    ip_dst='33.19.20.0',
                                    version=2,
                                    mirror_id=(erspanid & 0x3FFFFFFF),
                                    inner_frame=pkt3
                                    )

        m1=Mask(exp_pkt1)
        m1.set_do_not_care_scapy(ptf.packet.IP,'tos')
        m1.set_do_not_care_scapy(ptf.packet.IP,'frag')
        m1.set_do_not_care_scapy(ptf.packet.IP,'flags')
        m1.set_do_not_care_scapy(ptf.packet.IP,'chksum')
        m1.set_do_not_care_scapy(ptf.packet.GRE,'proto')
        m1.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'platf_id')
        m1.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info1')
        m1.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info2')
            
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'span_id')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'timestamp')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'sgt_other')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'direction')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'version')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'vlan')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'priority')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'truncated')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'unknown2')

        m2=Mask(exp_pkt2)
        m2.set_do_not_care_scapy(ptf.packet.IP,'tos')
        m2.set_do_not_care_scapy(ptf.packet.IP,'frag')
        m2.set_do_not_care_scapy(ptf.packet.IP,'flags')
        m2.set_do_not_care_scapy(ptf.packet.IP,'chksum')
        m2.set_do_not_care_scapy(ptf.packet.GRE,'proto')
        m2.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'platf_id')
        m2.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info1')
        m2.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info2')
            
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'span_id')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'timestamp')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'sgt_other')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'direction')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'version')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'vlan')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'priority')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'truncated')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'unknown2')

        n=Mask(pkt2)
        n.set_do_not_care_scapy(ptf.packet.IP,'len')
        n.set_do_not_care_scapy(ptf.packet.IP,'chksum')

        try:
            # in tuple: 0 is device number, 2 is port number
            # this tuple uniquely identifies a port
            # for ingress mirroring
            print "Checking INGRESS ERSPAN Mirroring"
            print "Sending packet port 2 -> port 3 (00:22:22:22:22:22 -> 00:00:00:00:00:33)"
            send_packet(self, 1, pkt)
            verify_each_packet_on_each_port(self, [m1,pkt], ports=[0,2])#FIXME need to properly implement
            # for egress mirroring
            print "Checking EGRESS ERSPAN Mirroring"
            print "Sending packet port 3 -> port 2 (00:33:33:33:33:33 -> 00:00:00:00:00:22)"
            send_packet(self, 2, pkt2)
            verify_each_packet_on_each_port(self, [pkt2,m2], ports=[1,0])#FIXME need to properly implement
        finally:
            sai_thrift_delete_fdb(self.client, vlan_remote_oid, mac2, port2)
            sai_thrift_delete_fdb(self.client, vlan_remote_oid, mac3, port3)

            # Remove ports from mirror destination
            attrb_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=0,object_id_list=[erspanid]))
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attrb_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attrb_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            # Now you can remove destination
            self.client.sai_thrift_remove_mirror_session(erspanid)

            # Remove ports from VLAN 3
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan_member(vlan_member3)
            self.client.sai_thrift_remove_vlan(vlan_remote_oid)

            # Add ports back to default VLAN
            vlan_member1a = sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan_member2a = sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan_member3a = sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)

@group('mirror')
class erspan_vlan_set_monitor(sai_base_test.ThriftInterfaceDataPlane):
    '''
    This test performs erspan monitoring
    From port2(source port) we send traffic to port 3
    erspan mirror packets are expected on port 1(monitor port)
    '''
    def runTest(self):
        print
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        mac3='00:00:00:00:00:33'
        mac2='00:00:00:00:00:22'
        monitor_port=port1
        source_port=port2
        mirror_type=SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
        src_mac='00:00:00:00:11:22'
        dst_mac='00:00:00:00:11:33'
        encap_type=SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL
        ip_version=0x4
        tos=0x3c
        ttl=0xf0
        gre_type=0x88be
        src_ip='17.18.19.0'
        dst_ip='33.19.20.0'
        addr_family=0
        vlan_id = 2
        vlan_remote_id = 3
        mac_action = SAI_PACKET_ACTION_FORWARD

        # Put ports under test in VLAN 3
        vlan_remote_oid = sai_thrift_create_vlan(self.client, vlan_remote_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_remote_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_remote_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member3 = sai_thrift_create_vlan_member(self.client, vlan_remote_oid, port3, SAI_VLAN_TAGGING_MODE_TAGGED)

        sai_thrift_create_fdb(self.client, vlan_remote_oid, mac3, port3, mac_action)
        sai_thrift_create_fdb(self.client, vlan_remote_oid, mac2, port2, mac_action)

        # Remove ports from default VLAN
        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, [port1, port2, port3])

        # Set PVID
        attr_value = sai_thrift_attribute_value_t(u16=vlan_remote_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        erspanid=sai_thrift_create_mirror_session(self.client,mirror_type=mirror_type,port=monitor_port,vlan=None,vlan_priority=None,vlan_tpid=None,vlan_header_valid=None,src_mac=src_mac,dst_mac=dst_mac,src_ip=src_ip,dst_ip=dst_ip,encap_type=encap_type,iphdr_version=ip_version,ttl=ttl,tos=tos,gre_type=gre_type)

        attrb_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=1,object_id_list=[erspanid]))

        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attrb_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attrb_value)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        pkt = simple_tcp_packet(eth_dst='00:00:00:00:00:33',
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.0.0.1',
                                dl_vlan_enable=True,
                                vlan_vid=3,
                                ip_id=101,
                                ip_ttl=64)

        pkt2 = simple_tcp_packet(eth_dst='00:00:00:00:00:22',
                                eth_src='00:33:33:33:33:33',
                                dl_vlan_enable=True,
                                vlan_vid=3,
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        pkt3 = simple_tcp_packet(eth_dst='00:00:00:00:00:22',
                                eth_src='00:33:33:33:33:33',
                                dl_vlan_enable=True,
                                vlan_vid=3,
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        exp_pkt1= ipv4_erspan_platform_pkt(pktlen=142,
                                    eth_dst='00:00:00:00:11:33',
                                    eth_src='00:00:00:00:11:22',
                                    ip_id=0,
                                    ip_ttl=240,
                                    ip_tos=0x3c,
                                    ip_ihl=5,
                                    ip_src='17.18.19.0',
                                    ip_dst='33.19.20.0',
                                    version=2,
                                    mirror_id=(erspanid & 0x3FFFFFFF),
                                    inner_frame=pkt
                                    )

        exp_pkt2= ipv4_erspan_platform_pkt(pktlen=142,
                                    eth_dst='00:00:00:00:11:33',
                                    eth_src='00:00:00:00:11:22',
                                    ip_id=0,
                                    ip_ttl=240,
                                    ip_tos=0x3c,
                                    ip_ihl=5,
                                    ip_src='17.18.19.0',
                                    ip_dst='33.19.20.0',
                                    version=2,
                                    mirror_id=(erspanid & 0x3FFFFFFF),
                                    inner_frame=pkt3
                                    )

        exp_vlan_pkt1= ipv4_erspan_platform_pkt(pktlen=142,
                                    eth_dst='00:00:00:00:11:33',
                                    eth_src='00:00:00:00:11:22',
                                    dl_vlan_enable=True,
                                    vlan_vid=2,
                                    ip_id=0,
                                    ip_ttl=240,
                                    ip_tos=0x3c,
                                    ip_ihl=5,
                                    ip_src='17.18.19.0',
                                    ip_dst='33.19.20.0',
                                    version=2,
                                    mirror_id=(erspanid & 0x3FFFFFFF),
                                    inner_frame=pkt
                                    )

        exp_vlan_pkt2= ipv4_erspan_platform_pkt(pktlen=142,
                                    eth_dst='00:00:00:00:11:33',
                                    eth_src='00:00:00:00:11:22',
                                    dl_vlan_enable=True,
                                    vlan_vid=2,
                                    ip_id=0,
                                    ip_ttl=240,
                                    ip_tos=0x3c,
                                    ip_ihl=5,
                                    ip_src='17.18.19.0',
                                    ip_dst='33.19.20.0',
                                    version=2,
                                    mirror_id=(erspanid & 0x3FFFFFFF),
                                    inner_frame=pkt3
                                    )

        m1=Mask(exp_pkt1)
        m1.set_do_not_care_scapy(ptf.packet.IP,'tos')
        m1.set_do_not_care_scapy(ptf.packet.IP,'frag')
        m1.set_do_not_care_scapy(ptf.packet.IP,'flags')
        m1.set_do_not_care_scapy(ptf.packet.IP,'chksum')
        m1.set_do_not_care_scapy(ptf.packet.GRE,'proto')
        m1.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'platf_id')
        m1.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info1')
        m1.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info2')
            
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'span_id')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'timestamp')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'sgt_other')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'direction')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'version')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'vlan')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'priority')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'truncated')
        m1.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'unknown2')

        m2=Mask(exp_pkt2)
        m2.set_do_not_care_scapy(ptf.packet.IP,'tos')
        m2.set_do_not_care_scapy(ptf.packet.IP,'frag')
        m2.set_do_not_care_scapy(ptf.packet.IP,'flags')
        m2.set_do_not_care_scapy(ptf.packet.IP,'chksum')
        m2.set_do_not_care_scapy(ptf.packet.GRE,'proto')
        m2.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'platf_id')
        m2.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info1')
        m2.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info2')
            
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'span_id')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'timestamp')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'sgt_other')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'direction')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'version')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'vlan')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'priority')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'truncated')
        m2.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'unknown2')

        m1_vlan=Mask(exp_vlan_pkt1)
        m1_vlan.set_do_not_care_scapy(ptf.packet.IP,'tos')
        m1_vlan.set_do_not_care_scapy(ptf.packet.IP,'frag')
        m1_vlan.set_do_not_care_scapy(ptf.packet.IP,'flags')
        m1_vlan.set_do_not_care_scapy(ptf.packet.IP,'chksum')
        m1_vlan.set_do_not_care_scapy(ptf.packet.GRE,'proto')
        m1_vlan.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'platf_id')
        m1_vlan.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info1')
        m1_vlan.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info2')
            
        m1_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'span_id')
        m1_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'timestamp')
        m1_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'sgt_other')
        m1_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'direction')
        m1_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'version')
        m1_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'vlan')
        m1_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'priority')
        m1_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'truncated')
        m1_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'unknown2')

        m2_vlan=Mask(exp_vlan_pkt2)
        m2_vlan.set_do_not_care_scapy(ptf.packet.IP,'tos')
        m2_vlan.set_do_not_care_scapy(ptf.packet.IP,'frag')
        m2_vlan.set_do_not_care_scapy(ptf.packet.IP,'flags')
        m2_vlan.set_do_not_care_scapy(ptf.packet.IP,'chksum')
        m2_vlan.set_do_not_care_scapy(ptf.packet.GRE,'proto')
        m2_vlan.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'platf_id')
        m2_vlan.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info1')
        m2_vlan.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info2')
            
        m2_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'span_id')
        m2_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'timestamp')
        m2_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'sgt_other')
        m2_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'direction')
        m2_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'version')
        m2_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'vlan')
        m2_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'priority')
        m2_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'truncated')
        m2_vlan.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'unknown2')

        n=Mask(pkt2)
        n.set_do_not_care_scapy(ptf.packet.IP,'len')
        n.set_do_not_care_scapy(ptf.packet.IP,'chksum')

        try:
            # in tuple: 0 is device number, 2 is port number
            # this tuple uniquely identifies a port
            # for ingress mirroring
            print "Checking INGRESS ERSPAN Mirroring"
            print "Sending packet port 2 -> port 3 (00:22:22:22:22:22 -> 00:00:00:00:00:33)"
            send_packet(self, 1, pkt)
            verify_each_packet_on_each_port(self, [m1,pkt], ports=[0,2])#FIXME need to properly implement
            # for egress mirroring
            print "Checking EGRESS ERSPAN Mirroring"
            print "Sending packet port 3 -> port 2 (00:33:33:33:33:33 -> 00:00:00:00:00:22)"
            send_packet(self, 2, pkt2)
            verify_each_packet_on_each_port(self, [pkt2,m2], ports=[1,0])#FIXME need to properly implement

            # Set vlan header valid to true
            attr_value = sai_thrift_attribute_value_t(booldata=True)
            attr = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_VLAN_HEADER_VALID, value=attr_value)
            self.client.sai_thrift_set_mirror_session_attribute(erspanid, attr)

            # Set vlan id
            attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
            attr = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_mirror_session_attribute(erspanid, attr)

            print "Checking INGRESS ERSPAN Mirroring"
            print "Sending packet port 2 -> port 3 (00:22:22:22:22:22 -> 00:00:00:00:00:33)"
            send_packet(self, 1, pkt)
            verify_each_packet_on_each_port(self, [m1_vlan,pkt], ports=[0,2])#FIXME need to properly implement

            print "Checking EGRESS ERSPAN Mirroring"
            print "Sending packet port 3 -> port 2 (00:33:33:33:33:33 -> 00:00:00:00:00:22)"
            send_packet(self, 2, pkt2)
            verify_each_packet_on_each_port(self, [pkt2,m2_vlan], ports=[1,0])#FIXME need to properly implement

            # Set vlan header valid to false
            attr_value = sai_thrift_attribute_value_t(booldata=False)
            attr = sai_thrift_attribute_t(id=SAI_MIRROR_SESSION_ATTR_VLAN_HEADER_VALID, value=attr_value)
            self.client.sai_thrift_set_mirror_session_attribute(erspanid, attr)

            print "Checking INGRESS ERSPAN Mirroring"
            print "Sending packet port 2 -> port 3 (00:22:22:22:22:22 -> 00:00:00:00:00:33)"
            send_packet(self, 1, pkt)
            verify_each_packet_on_each_port(self, [m1,pkt], ports=[0,2])#FIXME need to properly implement

            print "Checking EGRESS ERSPAN Mirroring"
            print "Sending packet port 3 -> port 2 (00:33:33:33:33:33 -> 00:00:00:00:00:22)"
            send_packet(self, 2, pkt2)
            verify_each_packet_on_each_port(self, [pkt2,m2], ports=[1,0])#FIXME need to properly implement

        finally:
            sai_thrift_delete_fdb(self.client, vlan_remote_oid, mac2, port2)
            sai_thrift_delete_fdb(self.client, vlan_remote_oid, mac3, port3)

            # Remove ports from mirror destination
            attrb_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=0,object_id_list=[erspanid]))
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attrb_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attrb_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            # Now you can remove destination
            self.client.sai_thrift_remove_mirror_session(erspanid)

            # Remove ports from VLAN 3
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan_member(vlan_member3)
            self.client.sai_thrift_remove_vlan(vlan_remote_oid)

            # Add ports back to default VLAN
            vlan_member1a = sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan_member2a = sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
            vlan_member3a = sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)

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

        sai_thrift_create_fdb(self.client, vlan_oid, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_oid, mac2, port2, mac_action)

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
            None, None, None,
            None)
        print "ingress_mirror_id = %d" %ingress_mirror_id

        attr_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=1,object_id_list=[ingress_mirror_id]))
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
            attr_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=0,object_id_list=[]))
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_mirror_session(ingress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_oid, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('mirror')
class IngressRSpanMirrorTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print '----------------------------------------------------------------------------------------------'
        print "Sending packet ptf_intf 1 -> ptf_intf 2, ptf_intf 3 (rspan mirror)"

        switch_init(self.client)
        vlan_id = 10
        vlan_remote_id = 110
        port1 = port_list[1]
        port2 = port_list[2]
        port3 = port_list[3]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_remote_oid = sai_thrift_create_vlan(self.client, vlan_remote_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        sai_thrift_create_fdb(self.client, vlan_oid, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_oid, mac2, port2, mac_action)

        # setup remote mirror session
        mirror_type = SAI_MIRROR_SESSION_TYPE_REMOTE
        monitor_port = port3
        vlan = vlan_remote_id
        print "Create mirror session: mirror_type = SAI_MIRROR_SESSION_TYPE_REMOTE, monitor_port = ptf_intf 3 "
        ingress_mirror_id = sai_thrift_create_mirror_session(self.client,
            mirror_type,
            monitor_port,
            vlan, None, None,
            None, None, None,
            None, None, None,
            None, None, None,
            None)
        print "ingress_mirror_id ' %x" %ingress_mirror_id

        attr_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=1,object_id_list=[ingress_mirror_id]))
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
            exp_mirrored_pkt = simple_tcp_packet(eth_dst=mac2,
                eth_src=mac1,
                ip_dst='10.0.0.1',
                ip_src='192.168.0.1',
                dl_vlan_enable=True,
                vlan_vid=110,
                ip_id=102,
                ip_ttl=64,
                pktlen=104)
            print '#### Sending 00:22:22:22:22:22 | 00:11:11:11:11:11 | 10.0.0.1 | 192.168.0.1 | @ ptf_intf 1 ####'
            send_packet(self, 1, str(pkt))
            verify_each_packet_on_each_port(self, [exp_pkt, exp_mirrored_pkt], [2, 3])

        finally:
            attr_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=0,object_id_list=[]))
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)

            self.client.sai_thrift_remove_mirror_session(ingress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_oid, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)
            self.client.sai_thrift_remove_vlan(vlan_remote_oid)

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

        sai_thrift_create_fdb(self.client, vlan_oid, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_oid, mac2, port2, mac_action)

        # setup enhanced remote mirror session
        mirror_type = SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
        monitor_port = port3
        vlan = vlan_id
        vlan_header_valid = True
        iphdr_version = 4
        tunnel_src_ip = "1.1.1.1"
        tunnel_dst_ip = "1.1.1.2"
        tunnel_src_mac = router_mac
        tunnel_dst_mac = "00:33:33:33:33:33"
        gre_protocol = 47
        ttl = 64

        print "Create mirror session: mirror_type = SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE, monitor_port = ptf_intf 3 "
        ingress_mirror_id = sai_thrift_create_mirror_session(self.client,
            mirror_type,
            monitor_port,
            vlan, None, None, vlan_header_valid,
            tunnel_src_mac, tunnel_dst_mac,
            tunnel_src_ip, tunnel_dst_ip,
            None, iphdr_version, ttl, None, gre_protocol)
        print "ingress_mirror_id = %d" %ingress_mirror_id

        attr_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=1,object_id_list=[ingress_mirror_id]))
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
            exp_mirrored_pkt = ipv4_erspan_platform_pkt(eth_dst=tunnel_dst_mac,
                eth_src=tunnel_src_mac,
                ip_src=tunnel_src_ip,
                ip_dst=tunnel_dst_ip,
                dl_vlan_enable=True,
                vlan_vid=10,
                ip_id=0,
                ip_ttl=64,
                version=2,
                mirror_id=(ingress_mirror_id & 0x3FFFFFFF),
                inner_frame=pkt)

            exp_mask_mirrored_pkt = Mask(exp_mirrored_pkt)
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, 'len')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, 'flags')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, 'chksum')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.GRE, 'proto')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'platf_id')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info1')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info2')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'span_id')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'timestamp')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'sgt_other')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'direction')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'version')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'vlan')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'priority')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'truncated')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'unknown2')

            print '#### Sending 00:22:22:22:22:22 | 00:11:11:11:11:11 | 10.0.0.1 | 192.168.0.1 | @ ptf_intf 1 ####'
            send_packet(self, 1, str(pkt))
            verify_packet(self, exp_pkt, 2)
            verify_packet(self, exp_mask_mirrored_pkt, 3)

        finally:
            attr_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=0,object_id_list=[]))
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)

            self.client.sai_thrift_remove_mirror_session(ingress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_oid, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)

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

        sai_thrift_create_fdb(self.client, vlan_oid, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_oid, mac2, port2, mac_action)

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
            None, None, None,
            None)
        print "egress_mirror_id = %d" %egress_mirror_id

        attr_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=1,object_id_list=[egress_mirror_id]))
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
            attr_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=0,object_id_list=[]))
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_mirror_session(egress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_oid, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)

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

        sai_thrift_create_fdb(self.client, vlan_oid, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_oid, mac2, port2, mac_action)

        # setup enhanced remote mirror session
        mirror_type = SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
        monitor_port = port3
        vlan = vlan_id
        vlan_header_valid = True
        iphdr_version = 4
        tunnel_src_ip = "1.1.1.1"
        tunnel_dst_ip = "1.1.1.2"

        tunnel_src_mac = router_mac
        tunnel_dst_mac = "00:33:33:33:33:33"
        gre_protocol = 47
        ttl = 64

        print "Create mirror session: mirror_type = SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE, monitor_port = ptf_intf 3 "
        egress_mirror_id = sai_thrift_create_mirror_session(self.client,
            mirror_type,
            monitor_port,
            vlan, None, None, vlan_header_valid,
            tunnel_src_mac, tunnel_dst_mac,
            tunnel_src_ip, tunnel_dst_ip,
            None, iphdr_version, ttl, None, gre_protocol)
        print "egress_mirror_id = %d" %egress_mirror_id

        attr_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=1,object_id_list=[egress_mirror_id]))
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
            exp_mirrored_pkt = ipv4_erspan_platform_pkt(eth_dst=tunnel_dst_mac,
                eth_src=tunnel_src_mac,
                ip_src=tunnel_src_ip,
                ip_dst=tunnel_dst_ip,
                dl_vlan_enable=True,
                vlan_vid=10,
                ip_id=0,
                ip_ttl=64,
                version=2,
                mirror_id=(egress_mirror_id & 0x3FFFFFFF),
                inner_frame=exp_pkt)

            exp_mask_mirrored_pkt = Mask(exp_mirrored_pkt)
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, 'len')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, 'flags')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.IP, 'chksum')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.GRE, 'proto')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'platf_id')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info1')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.PlatformSpecific, 'info2')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'span_id')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'timestamp')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'sgt_other')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'direction')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'version')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'vlan')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'priority')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'truncated')
            exp_mask_mirrored_pkt.set_do_not_care_scapy(ptf.packet.ERSPAN_III, 'unknown2')

            print '#### Sending 00:22:22:22:22:22 | 00:11:11:11:11:11 | 10.0.0.1 | 192.168.0.1 | @ ptf_intf 1 ####'
            send_packet(self, 1, str(pkt))
            verify_packet(self, exp_pkt, 2)
            verify_packet(self, exp_mask_mirrored_pkt, 3)

        finally:
            attr_value = sai_thrift_attribute_value_t(objlist=sai_thrift_object_list_t(count=0,object_id_list=[]))
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_mirror_session(egress_mirror_id)

            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_oid, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)
