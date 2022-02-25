# Copyright (C) 2018-present. Mellanox Technologies, Ltd.
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
Thrift SAI interface FDB tests
"""
import socket
from switch import *
import sai_base_test

@group('l2')
class L2FDBMissUnicastTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print 'Test FDB unicast miss action'
        switch_init(self.client)

        flood_list = port_list.keys()
        flood_list.remove(0)

        self.client.sai_thrift_flush_fdb_entries(thrift_attr_list=[])

        pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        print 'Test unicast packet with unicast miss action=DROP'
        attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_DROP)
        attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION, value=attr_value)
        self.client.sai_thrift_set_switch_attribute(attr)

        send_packet(self, 0, str(pkt))
        verify_no_other_packets(self)

        print 'Test unicast packet with unicast miss action=FORWARD'
        attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_FORWARD)
        attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION, value=attr_value)
        self.client.sai_thrift_set_switch_attribute(attr)

        send_packet(self, 0, str(pkt))
        verify_packets(self, pkt, flood_list)

        print 'Test unicast packet with broadcast miss action=DROP'
        attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_DROP)
        attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_PACKET_ACTION, value=attr_value)
        self.client.sai_thrift_set_switch_attribute(attr)

        self.client.sai_thrift_flush_fdb_entries(thrift_attr_list=[])

        send_packet(self, 0, str(pkt))
        verify_packets(self, pkt, flood_list)

    def tearDown(self):
        self.client.sai_thrift_flush_fdb_entries(thrift_attr_list=[])

        attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_FORWARD)
        attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION, value=attr_value)
        self.client.sai_thrift_set_switch_attribute(attr)

        attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_FORWARD)
        attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_PACKET_ACTION, value=attr_value)
        self.client.sai_thrift_set_switch_attribute(attr)

        sai_base_test.ThriftInterfaceDataPlane.tearDown(self)

@group('l2')
class L2FDBMissBroadcastTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print 'Test FDB broadcast miss action'
        switch_init(self.client)

        flood_list = port_list.keys()
        flood_list.remove(0)

        self.client.sai_thrift_flush_fdb_entries(thrift_attr_list=[])

        pkt = simple_tcp_packet(eth_dst='FF:FF:FF:FF:FF:FF',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        print 'Test broadcast packet with broadcast miss action=DROP'
        attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_DROP)
        attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_PACKET_ACTION, value=attr_value)
        self.client.sai_thrift_set_switch_attribute(attr)

        send_packet(self, 0, str(pkt))
        verify_no_other_packets(self)

        print 'Test broadcast packet with unicast miss action=FORWARD'
        attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_FORWARD)
        attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION, value=attr_value)
        self.client.sai_thrift_set_switch_attribute(attr)

        send_packet(self, 0, str(pkt))
        verify_no_other_packets(self)

        print 'Test broadcast packet with broadcast miss action=FORWARD'
        attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_FORWARD)
        attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_PACKET_ACTION, value=attr_value)
        self.client.sai_thrift_set_switch_attribute(attr)

        send_packet(self, 0, str(pkt))
        verify_packets(self, pkt, flood_list)

    def tearDown(self):
        self.client.sai_thrift_flush_fdb_entries(thrift_attr_list=[])

        attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_FORWARD)
        attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION, value=attr_value)
        self.client.sai_thrift_set_switch_attribute(attr)

        attr_value = sai_thrift_attribute_value_t(s32=SAI_PACKET_ACTION_FORWARD)
        attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_PACKET_ACTION, value=attr_value)
        self.client.sai_thrift_set_switch_attribute(attr)
        sai_base_test.ThriftInterfaceDataPlane.tearDown(self)

@group('l2')
class L2FDBFloodRoutingNoVlan(sai_base_test.ThriftInterfaceDataPlane):
    ingress_rif_id = 0
    egress_rif_id = 0
    vr_id = 0

    # Verify no packet will be flooded if port is not added to any vlan
    def runTest(self):
        print
        print 'Test FDB flood blocking with routing but w/o vlans'

        switch_init(self.client)

        print "Remove ports from vlan=1 ..."
        sai_thrift_vlan_remove_all_ports(self.client, switch.default_vlan.oid)

        print "Setup routing ..."
        self.vr_id = sai_thrift_create_virtual_router(self.client, 1, 1)

        ingress_port = port_list[0]
        egress_port = port_list[1]

        self.ingress_rif_id = sai_thrift_create_router_interface(self.client, vr_oid=self.vr_id, type=SAI_ROUTER_INTERFACE_TYPE_PORT, port_oid=ingress_port, vlan_oid=0, v4_enabled=1, v6_enabled=1, mac='')
        self.egress_rif_id = sai_thrift_create_router_interface(self.client, vr_oid=self.vr_id, type=SAI_ROUTER_INTERFACE_TYPE_PORT, port_oid=egress_port, vlan_oid=0, v4_enabled=1, v6_enabled=1, mac='')

        family = SAI_IP_ADDR_FAMILY_IPV4
        subnet = '10.0.1.0'
        addr = '10.0.1.1'
        mask = '255.255.255.0'
        dmac = '00:11:22:33:44:01'
        sai_thrift_create_neighbor(self.client, family, self.egress_rif_id, addr, dmac)
        sai_thrift_create_route(self.client, self.vr_id, family, subnet, mask, self.egress_rif_id)

        print "Verify no FDB unicast miss flood ..."
        pkt = simple_tcp_packet(eth_dst='22:33:44:55:66:77',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.10.10.10',
                                ip_id=101,
                                ip_ttl=64)

        send_packet(self, 0, str(pkt))
        verify_no_other_packets(self)

        print "Verify no FDB broadcast miss flood ..."
        pkt = simple_tcp_packet(eth_dst='FF:FF:FF:FF:FF:FF',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.10.10.10',
                                ip_id=101,
                                ip_ttl=64)

        send_packet(self, 0, str(pkt))
        verify_no_other_packets(self)

        print "Verify routed packet ..."
        pkt2 = simple_tcp_packet(eth_dst=router_mac,
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.1.1',
                                ip_id=101,
                                ip_ttl=63)

        match = simple_tcp_packet(eth_dst='00:11:22:33:44:01',
                                  eth_src=router_mac,
                                  ip_dst='10.0.1.1',
                                  ip_id=101,
                                  ip_ttl=62)

        send_packet(self, 0, str(pkt2))
        verify_packets(self, match, [1])

    def tearDown(self):
        family = SAI_IP_ADDR_FAMILY_IPV4
        subnet = '10.0.1.0'
        addr = '10.0.1.1'
        mask = '255.255.255.0'
        dmac = '00:11:22:33:44:01'

        sai_thrift_remove_route(self.client, self.vr_id, family, subnet, mask, self.egress_rif_id)
        sai_thrift_remove_neighbor(self.client, family, self.egress_rif_id, addr, dmac)
        self.client.sai_thrift_remove_router_interface(self.ingress_rif_id)
        self.client.sai_thrift_remove_router_interface(self.egress_rif_id)

        self.client.sai_thrift_remove_virtual_router(self.vr_id)

        for i,p in port_list.iteritems():
            sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, p, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_base_test.ThriftInterfaceDataPlane.tearDown(self)
