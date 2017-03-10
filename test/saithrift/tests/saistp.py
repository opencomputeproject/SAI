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

@group('stp')
class L2StpTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending L2 packet - port 1 -> port 2 [trunk vlan=10])"
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[1]
        port2 = port_list[2]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        vlan_list = [vlan_id]
        mac_action = SAI_PACKET_ACTION_FORWARD

        self.client.sai_thrift_create_vlan(vlan_id)
        vlan_port1 = sai_thrift_vlan_port_t(port_id=port1, tagging_mode=SAI_VLAN_PORT_UNTAGGED)
        vlan_port2 = sai_thrift_vlan_port_t(port_id=port2, tagging_mode=SAI_VLAN_PORT_UNTAGGED)
        self.client.sai_thrift_add_ports_to_vlan(vlan_id, [vlan_port1, vlan_port2])

        stp_id = sai_thrift_create_stp_entry(self.client, vlan_list)
        self.client.sai_thrift_set_stp_port_state(stp_id, port1, SAI_PORT_STP_STATE_FORWARDING)
        self.client.sai_thrift_set_stp_port_state(stp_id, port2, SAI_PORT_STP_STATE_FORWARDING)

        sai_thrift_create_fdb(self.client, vlan_id, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_id, mac2, port2, mac_action)

        try:
            pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.0.0.1',
                                ip_id=113,
                                ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                eth_src='00:22:22:22:22:22',
                                ip_dst='10.0.0.1',
                                ip_id=113,
                                ip_ttl=64)
            send_packet(self, 2, str(pkt))
            verify_packets(self, exp_pkt, [1])

            self.client.sai_thrift_set_stp_port_state(stp_id, port2, SAI_PORT_STP_STATE_BLOCKING)
            print "Sending packet port 1 (blocked) -> port 2 (192.168.0.1 -> 10.0.0.1 [id = 101])"
            pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.0.0.1',
                                    ip_id=113,
                                    ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='10.0.0.1',
                                    ip_id=113,
                                    ip_ttl=64)
            send_packet(self, 2, str(pkt))
            verify_packets(self, exp_pkt, [])
        finally:
            sai_thrift_delete_fdb(self.client, vlan_id, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_id, mac2, port2)

            self.client.sai_thrift_remove_stp_entry(stp_id)

            self.client.sai_thrift_remove_ports_from_vlan(vlan_id, [vlan_port1, vlan_port2])
            self.client.sai_thrift_delete_vlan(vlan_id)


