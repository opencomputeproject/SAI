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
Thrift SAI interface L2 tests
"""
import socket
from switch import *
import sai_base_test
import sys
sys.path.append('../sai_thrift_src/gen-py/')
from switch_sai.ttypes import *
import random
from sai_types import *
 
@group('l2_wip')
class L2WIP(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        vlan_id = 1
        switch_init2(self.client)
        hw_port1 = 0
        hw_port2 = 1
        port1 = port_list[hw_port1]
        port2 = port_list[hw_port2]
        bridge_port1 = br_port_list[port1]
        bridge_port2 = br_port_list[port2]
        
        # port2 drops tagged. port1 drops untagged
        attr_value = sai_thrift_attribute_value_t(booldata=True)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_DROP_UNTAGGED, value=attr_value) 
        self.client.sai_thrift_set_port_attribute(port1, attr)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_DROP_TAGGED, value=attr_value) 
        self.client.sai_thrift_set_port_attribute(port2, attr)

        # Create FDB Entries:
        mac_action = SAI_PACKET_ACTION_FORWARD
        fdb_entry_type = SAI_FDB_ENTRY_TYPE_STATIC
        sai_thrift_create_fdb(self.client, mac1, default_bridge_type, vlan_id, default_bridge, bridge_port1, mac_action, fdb_entry_type)
        sai_thrift_create_fdb(self.client, mac2, default_bridge_type, vlan_id, default_bridge, bridge_port2, mac_action, fdb_entry_type)
        
        untagged_pkt1 = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                          eth_src='00:11:11:11:11:11',
                                          ip_dst='10.0.0.1',
                                          ip_id=101,
                                          ip_ttl=64)
        tagged_pkt1 = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                        eth_src='00:11:11:11:11:11',
                                        ip_dst='10.0.0.1',
                                        ip_id=101,
                                        dl_vlan_enable=True,
                                        vlan_vid=vlan_id,
                                        ip_ttl=64,
                                        pktlen=104)
        untagged_pkt2 = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                          eth_src='00:22:22:22:22:22',
                                          ip_dst='10.0.0.1',
                                          ip_id=101,
                                          ip_ttl=64)
        tagged_pkt2 = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                        eth_src='00:22:22:22:22:22',
                                        ip_dst='10.0.0.1',
                                        ip_id=101,
                                        dl_vlan_enable=True,
                                        vlan_vid=vlan_id,
                                        ip_ttl=64,
                                        pktlen=104)

        try:
            print "Sending tagged packet port 0 -> port 1"
            send_packet(self, hw_port1, str(tagged_pkt1))
            verify_packets(self, tagged_pkt1, [hw_port2])
            print "Sending tagged packet port 1 -> port 0"
            send_packet(self, hw_port2, str(tagged_pkt2))
            verify_no_packet_any(self, tagged_pkt2, port_list.keys())
            print "Sending untagged packet port 0 -> port 1"
            send_packet(self, hw_port1, str(untagged_pkt1))
            verify_no_packet_any(self, untagged_pkt1, port_list.keys())
            print "Sending untagged packet port 1 -> port 0"
            send_packet(self, hw_port2, str(untagged_pkt2))
            verify_packets(self, untagged_pkt2, [hw_port1])
        finally:
            sai_thrift_delete_fdb(self.client, mac1, default_bridge)
            sai_thrift_delete_fdb(self.client, mac2, default_bridge)
            attr_value = sai_thrift_attribute_value_t(booldata=False)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_DROP_UNTAGGED, value=attr_value) 
            self.client.sai_thrift_set_port_attribute(port1, attr)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_DROP_TAGGED, value=attr_value) 
            self.client.sai_thrift_set_port_attribute(port2, attr)