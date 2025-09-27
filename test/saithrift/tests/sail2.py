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

@group('l2')
class L2AccessToAccessVlanTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending L2 packet port 1 -> port 2 [access vlan=10])"
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        sai_thrift_create_fdb(self.client, vlan_oid, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_oid, mac2, port2, mac_action)

        pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        try:
            send_packet(self, 0, str(pkt))
            verify_packets(self, pkt, [1])
        finally:
            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_oid, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('l2')
class L2TrunkToTrunkVlanTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending L2 packet - port 1 -> port 2 [trunk vlan=10])"
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_TAGGED)

        sai_thrift_create_fdb(self.client, vlan_oid, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_oid, mac2, port2, mac_action)

        pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                dl_vlan_enable=True,
                                vlan_vid=10,
                                ip_dst='10.0.0.1',
                                ip_id=102,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.0.1',
                                ip_id=102,
                                dl_vlan_enable=True,
                                vlan_vid=10,
                                ip_ttl=64)

        try:
            send_packet(self, 0, str(pkt))
            verify_packets(self, exp_pkt, [1])
        finally:
            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_oid, mac2, port2)
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('l2')
class L2AccessToTrunkVlanTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending L2 packet - port 1 -> port 2 [trunk vlan=10])"
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
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

        pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.0.1',
                                ip_id=102,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.0.1',
                                dl_vlan_enable=True,
                                vlan_vid=10,
                                ip_id=102,
                                ip_ttl=64,
                                pktlen=104)

        try:
            send_packet(self, 0, str(pkt))
            verify_packets(self, exp_pkt, [1])
        finally:
            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_oid, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('l2')
class L2TrunkToAccessVlanTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "Sending L2 packet - port 1 -> port 2 [trunk vlan=10])"
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)

        sai_thrift_create_fdb(self.client, vlan_oid, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_oid, mac2, port2, mac_action)

        pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                dl_vlan_enable=True,
                                vlan_vid=10,
                                ip_dst='10.0.0.1',
                                ip_id=102,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.0.1',
                                ip_id=102,
                                ip_ttl=64,
                                pktlen=96)
        try:
            send_packet(self, 0, str(pkt))
            verify_packets(self, exp_pkt, [1])
        finally:
            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_oid, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)

            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('l2')
class L2FloodTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print 'Flood test on ports 1, 2 and 3'
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member3 = sai_thrift_create_vlan_member(self.client, vlan_oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.0.1',
                                ip_id=107,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.0.1',
                                ip_id=107,
                                ip_ttl=64)

        try:
            send_packet(self, 0, str(pkt))
            verify_packets(self, exp_pkt, [1, 2])
            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [0, 2])
            send_packet(self, 2, str(pkt))
            verify_packets(self, exp_pkt, [0, 1])
        finally:
            sai_thrift_flush_fdb_by_vlan(self.client, vlan_oid)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan_member(vlan_member3)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('l2')
@group('lag')
class L2LagTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)

        lag_id1 = sai_thrift_create_lag(self.client, [])

        sai_thrift_vlan_remove_all_ports(self.client, switch.default_vlan.oid)

        lag_member_id1 = sai_thrift_create_lag_member(self.client, lag_id1, port1)
        lag_member_id2 = sai_thrift_create_lag_member(self.client, lag_id1, port2)
        lag_member_id3 = sai_thrift_create_lag_member(self.client, lag_id1, port3)

        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, lag_id1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port4, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_LAG_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_lag_attribute(lag_id1, attr)

        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port4, attr)

        sai_thrift_create_fdb(self.client, vlan_oid, mac1, lag_id1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_oid, mac2, port4, mac_action)

        try:
            count = [0, 0, 0]
            dst_ip = int(socket.inet_aton('10.10.10.1').encode('hex'),16)
            max_itrs = 200
            for i in range(0, max_itrs):
                dst_ip_addr = socket.inet_ntoa(hex(dst_ip)[2:].zfill(8).decode('hex'))
                pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                        eth_src='00:22:22:22:22:22',
                                        ip_dst=dst_ip_addr,
                                        ip_src='192.168.8.1',
                                        ip_id=109,
                                        ip_ttl=64)

                exp_pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                            eth_src='00:22:22:22:22:22',
                                            ip_dst=dst_ip_addr,
                                            ip_src='192.168.8.1',
                                            ip_id=109,
                                            ip_ttl=64)

                send_packet(self, 3, str(pkt))
                rcv_idx = verify_any_packet_any_port(self, [exp_pkt], [0, 1, 2])
                count[rcv_idx] += 1
                dst_ip += 1

            print count
            for i in range(0, 3):
                self.assertTrue((count[i] >= ((max_itrs / 3) * 0.8)),
                        "Not all paths are equally balanced")

            pkt = simple_tcp_packet(eth_src='00:11:11:11:11:11',
                                    eth_dst='00:22:22:22:22:22',
                                    ip_dst='10.0.0.1',
                                    ip_id=109,
                                    ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_src='00:11:11:11:11:11',
                                    eth_dst='00:22:22:22:22:22',
                                    ip_dst='10.0.0.1',
                                    ip_id=109,
                                    ip_ttl=64)
            print "Sending packet port 1 (lag member) -> port 4"
            send_packet(self, 0, str(pkt))
            verify_packets(self, exp_pkt, [3])
            print "Sending packet port 2 (lag member) -> port 4"
            send_packet(self, 1, str(pkt))
            verify_packets(self, exp_pkt, [3])
            print "Sending packet port 3 (lag member) -> port 4"
            send_packet(self, 2, str(pkt))
            verify_packets(self, exp_pkt, [3])
        finally:

            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, lag_id1)
            sai_thrift_delete_fdb(self.client, vlan_oid, mac2, port4)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)

            sai_thrift_remove_lag_member(self.client, lag_member_id1)
            sai_thrift_remove_lag_member(self.client, lag_member_id2)
            sai_thrift_remove_lag_member(self.client, lag_member_id3)
            sai_thrift_remove_lag(self.client, lag_id1)
            self.client.sai_thrift_remove_vlan(vlan_oid)

            for port in sai_port_list:
                sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port, SAI_VLAN_TAGGING_MODE_UNTAGGED)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port4, attr)

@group('l2')
@group('lag')
class LagHashseedTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        
	"""
        Create a LAG group with 4 ports 1 through 4. Setup static FDB entries for the LAG and send packet to this destination MAC address. 
        Send 100 packets with varying 5-tuple and check order/sequence of the distribution of packets received on ports 1 through 4. 
        Change the LAG Hash seed value to 10 and compare the order/sequence of the distribution of packets received for the same set of 100 packets on ports 1 through 4. 
        Verify that it is different after changing the hash seed.
        """
        
	switch_init(self.client)
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        port5 = port_list[4]
        mac1 = '00:11:11:11:11:11'
        mac_action = SAI_PACKET_ACTION_FORWARD
        lag_hashseed_value=10

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        
        lag_id1 = sai_thrift_create_lag(self.client, [])

        sai_thrift_vlan_remove_all_ports(self.client, switch.default_vlan.oid)

        lag_member_id1 = sai_thrift_create_lag_member(self.client, lag_id1, port1)
        lag_member_id2 = sai_thrift_create_lag_member(self.client, lag_id1, port2)
        lag_member_id3 = sai_thrift_create_lag_member(self.client, lag_id1, port3)
        lag_member_id4 = sai_thrift_create_lag_member(self.client, lag_id1, port4)

        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, lag_id1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port5, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_LAG_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_lag_attribute(lag_id1, attr)

        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port5, attr)

        sai_thrift_create_fdb(self.client, vlan_oid, mac1, lag_id1, mac_action)

        try:    
	    max_itrs = 101
            count1 = [0, 0, 0, 0]
            laglist1=list()
            src_mac_start = '00:22:22:22:22:'
            ip_src_start = '192.168.12.'
            ip_dst_start = '10.10.10.'
            dport = 0x80
            sport = 0x1234
            print ("sending 100 packets to verify the order/sequence of distribution ")
                        
            for i in range(0, max_itrs):
                src_mac = src_mac_start + str(i % 99).zfill(2)
                ip_src = ip_src_start + str(i % 99).zfill(3)
                ip_dst = ip_dst_start + str(i % 99).zfill(3)    
                                
                pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
				        eth_src=src_mac,
					ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_sport=sport,
					tcp_dport=dport,
                                        ip_id=109,
                                        ip_ttl=64)

                exp_pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                            eth_src=src_mac,
                                            ip_dst=ip_dst,
                                            ip_src=ip_src,
                                            tcp_sport=sport,
                                            tcp_dport=dport,
                                            ip_id=109,
                                            ip_ttl=64)

                send_packet(self, 4, str(pkt))
                rcv_idx = verify_any_packet_any_port(self, [exp_pkt], [0, 1, 2, 3])
                count1[rcv_idx] += 1
                laglist1.append(rcv_idx)
                sport += 1
                dport += 1
        
            print ("The distribution of packets with default hash seed value:",count1)

            attr_value = sai_thrift_attribute_value_t(u32=lag_hashseed_value)
            attr = sai_thrift_attribute_t(id= SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_SEED, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)
                
            #sending packets again		
            count2 = [0, 0, 0, 0]
            laglist2=list()
            max_itrs = 101
            src_mac_start = '00:22:22:22:22:'
            ip_src_start = '192.168.12.'
            ip_dst_start = '10.10.10.'
            dport = 0x80
            sport = 0x1234
			
            print ("sending 100 packets to verify the order/sequence of distribution ")
                        
            for i in range(0, max_itrs):
	        src_mac = src_mac_start + str(i % 99).zfill(2)
                ip_src = ip_src_start + str(i % 99).zfill(3)
                ip_dst = ip_dst_start + str(i % 99).zfill(3)    
                                
                pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                        eth_src=src_mac,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_sport=sport,
                                        tcp_dport=dport,
                                        ip_id=109,
                                        ip_ttl=64)

                exp_pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                            eth_src=src_mac,
                                            ip_dst=ip_dst,
                                            ip_src=ip_src,
                                            tcp_sport=sport,
                                            tcp_dport=dport,
                                            ip_id=109,
                                            ip_ttl=64)

                send_packet(self, 4, str(pkt))
                rcv_idx = verify_any_packet_any_port(self, [exp_pkt], [0, 1, 2, 3])
                count2[rcv_idx] += 1
                laglist2.append(rcv_idx)
                sport += 1
                dport += 1
                 
            print ("The distribution of packet after changing hash seed:" , count2)

            order_check=0		
            for i in range(0,max_itrs):
                if(laglist1[i] != laglist2[i]):
                    order_check+=1
                
	    print ("checking the difference in order/sequence before and after changing hash seed value:" ,order_check)

        finally:

            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, lag_id1)
			
	    attr_value = sai_thrift_attribute_value_t(u32=0)
            attr = sai_thrift_attribute_t(id= SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_SEED, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)

            sai_thrift_remove_lag_member(self.client, lag_member_id1)
            sai_thrift_remove_lag_member(self.client, lag_member_id2)
            sai_thrift_remove_lag_member(self.client, lag_member_id3)
            sai_thrift_remove_lag_member(self.client, lag_member_id4)	
            sai_thrift_remove_lag(self.client, lag_id1)
            self.client.sai_thrift_remove_vlan(vlan_oid)

            for port in sai_port_list:
                sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port, SAI_VLAN_TAGGING_MODE_UNTAGGED)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port5, attr)
            
@group('l2')
@group('sonic')
class L2VlanBcastUcastTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        For SONiC
        Vlan broadcast and known unicast test. Verify the broacast packet reaches all ports in the vlan and unicast packet reach specific port. 
        Steps:
        1. remove all testing ports from default vlan
        2. create vlan 10
        3. add n-1 ports to vlan 10
        4. add mac for each port
        5. send untagged broadcast packet from port 1, verify all n-1 ports receive the packet except the last port
        6. send untagged unicast packets from port 1 to the rest of the vlan members ports. Verify only one port at a time receives the packet and port n does not.
        7. clean up.
        """

        switch_init(self.client)
        vlan_id = 10
        mac_list = []
        vlan_member_list = []
        ingress_port = 0

        for i in range (1, len(port_list)):
            mac_list.append("00:00:00:00:00:%02x" %(i+1))
        mac_action = SAI_PACKET_ACTION_FORWARD

        sai_thrift_vlan_remove_ports(self.client, switch.default_vlan.oid, port_list)

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        for i in range (0, len(port_list)-1):
            vlan_member_list.append(sai_thrift_create_vlan_member(self.client, vlan_oid, port_list[i], SAI_VLAN_TAGGING_MODE_UNTAGGED))

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        for i in range (0, len(port_list)-1):
            self.client.sai_thrift_set_port_attribute(port_list[i], attr)
            sai_thrift_create_fdb(self.client, vlan_oid, mac_list[i], port_list[i], mac_action)

        bcast_pkt = simple_tcp_packet(eth_dst='ff:ff:ff:ff:ff:ff',
                                eth_src='00:00:00:00:00:01',
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        try:
            expected_ports = []
            for i in range (1, len(port_list)-1):
                expected_ports.append(i)

            send_packet(self, ingress_port, str(bcast_pkt))
            verify_packets(self, bcast_pkt, expected_ports)

            for i in range (1, len(port_list)-1):
                ucast_pkt = simple_tcp_packet(eth_dst=mac_list[i],
                                    eth_src='00:00:00:00:00:01',
                                    ip_dst='10.0.0.1',
                                    ip_id=101,
                                    ip_ttl=64)
                send_packet(self, ingress_port, str(ucast_pkt))
                verify_packets(self, ucast_pkt, [i])

        finally:
            attr_value = sai_thrift_attribute_value_t(u16=switch.default_vlan.vid)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)

            for i in range (0, len(port_list)-1):
                sai_thrift_delete_fdb(self.client, vlan_oid, mac_list[i], port_list[i])
                self.client.sai_thrift_set_port_attribute(port_list[i], attr)

            #'00:00:00:00:00:01' is learned on port 1, but not be removed after test
            sai_thrift_delete_fdb(self.client, vlan_oid,'00:00:00:00:00:01', port_list[0])

            for vlan_member in vlan_member_list:
                self.client.sai_thrift_remove_vlan_member(vlan_member)

            self.client.sai_thrift_remove_vlan(vlan_oid)

            for port in sai_port_list:
                sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port, SAI_VLAN_TAGGING_MODE_UNTAGGED)

@group('l2')
class L2FdbAgingTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        print "PTF L2 FDB aging test ..."
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        fdb_aging_time = 10

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member3 = sai_thrift_create_vlan_member(self.client, vlan_oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        attr_value = sai_thrift_attribute_value_t(u32=fdb_aging_time)
        attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_AGING_TIME, value=attr_value)
        self.client.sai_thrift_set_switch_attribute(attr)

        pkt = simple_tcp_packet(eth_dst=mac2,
                                eth_src=mac1,
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        pkt1 = simple_tcp_packet(eth_dst=mac1,
                                 eth_src=mac2,
                                 ip_dst='10.0.0.1',
                                 ip_id=101,
                                 ip_ttl=64)

        try:
            print "Send packet from port1 to port2 and verify on each of ports"
            print '#### Sending 00:11:11:11:11:11| 00:22:22:22:22:22 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 1 ####'
            send_packet(self, 0, str(pkt))
            verify_each_packet_on_each_port(self, [pkt, pkt], [1, 2])
            print "Send packet from port2 to port1 and verify only on port1"
            print '#### Sending 00:22:22:22:22:22| 00:11:11:11:11:11 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 2 ####'
            send_packet(self, 1, str(pkt1))
            verify_packets(self, pkt1, [0])
            print "Wait when the aging time for FDB entries in the FDB table expires, and the entries are removed ..."
            time.sleep(fdb_aging_time + 2)
            print "Send packet from port2 to port1 and verify on each of ports"
            print '#### Sending 00:22:22:22:22:22| 00:11:11:11:11:11 | 10.10.10.1 | 192.168.0.1 | @ ptf_intf 2 ####'
            send_packet(self, 1, str(pkt1))
            verify_each_packet_on_each_port(self, [pkt1, pkt1], [0, 2])
        finally:
            sai_thrift_flush_fdb_by_vlan(self.client, vlan_id)

            attr_value = sai_thrift_attribute_value_t(u32=0)
            attr = sai_thrift_attribute_t(id=SAI_SWITCH_ATTR_FDB_AGING_TIME, value=attr_value)
            self.client.sai_thrift_set_switch_attribute(attr)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan_member(vlan_member3)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('l2')
class L2ARPRequestReplyFDBLearningTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print
        switch_init(self.client)
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        v4_enabled = 1
        v6_enabled = 1
        vlan_id = 10

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member3 = sai_thrift_create_vlan_member(self.client, vlan_oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan_oid, v4_enabled, v6_enabled, router_mac)

        try:
            # send the test packet(s)
            print "Send ARP request packet from port1 ..."
            arp_req_pkt = simple_arp_packet(eth_dst='ff:ff:ff:ff:ff:ff',
                                            eth_src='00:11:11:11:11:11',
                                            vlan_vid=10,
                                            arp_op=1, #ARP request
                                            ip_snd='10.10.10.1',
                                            ip_tgt='10.10.10.2',
                                            hw_snd='00:11:11:11:11:11')

            send_packet(self, 0, str(arp_req_pkt))

            time.sleep(1)
            print "Send ARP reply packet from port2 ..."
            arp_rpl_pkt = simple_arp_packet(eth_dst=router_mac,
                                            eth_src='00:11:22:33:44:55',
                                            vlan_vid=10,
                                            arp_op=2, #ARP reply
                                            ip_snd='10.10.10.2',
                                            ip_tgt='10.10.10.1',
                                            hw_snd=router_mac,
                                            hw_tgt='00:11:22:33:44:55')

            send_packet(self, 1, str(arp_rpl_pkt))

            pkt = simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                                    eth_src='00:11:22:33:44:55',
                                    ip_dst='10.10.10.1',
                                    ip_id=101,
                                    ip_ttl=64)

            time.sleep(1)
            print "Send packet from port2 to port1 and verify only on port1 (src_mac and dst_mac addresses are learned)"
            print '#### Sending 00:11:11:11:11:11 | 00:11:22:33:44:55 | 10.10.10.1 | 10.10.10.2 | @ ptf_intf 2 ####'
            send_packet(self, 1, str(pkt))
            verify_packets(self, pkt, [0])

        finally:
            sai_thrift_flush_fdb_by_vlan(self.client, vlan_oid)
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_virtual_router(vr_id)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan_member(vlan_member3)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('l2')
@group('1D')
class L2BridgeSubPortFloodTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print ""
        switch_init(self.client)
        vlan_id1 = 10
        vlan_id2 = 15
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]

        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'

        sai_thrift_vlan_remove_all_ports(self.client, switch.default_vlan.oid)

        vlan_oid1 = sai_thrift_create_vlan(self.client, vlan_id1)
        vlan_oid2 = sai_thrift_create_vlan(self.client, vlan_id2)

        bridge_id1 = sai_thrift_create_bridge(self.client, SAI_BRIDGE_TYPE_1D)
        bridge_id2 = sai_thrift_create_bridge(self.client, SAI_BRIDGE_TYPE_1D)

        bport1_id = sai_thrift_create_bridge_sub_port(self.client, port1, bridge_id1, vlan_id1)
        bport2_id = sai_thrift_create_bridge_sub_port(self.client, port2, bridge_id1, vlan_id2)
        bport3_id = sai_thrift_create_bridge_sub_port(self.client, port3, bridge_id2, vlan_id1)
        bport4_id = sai_thrift_create_bridge_sub_port(self.client, port4, bridge_id2, vlan_id2)

        pkt1 = simple_tcp_packet(eth_dst=mac1,
                                eth_src=mac2,
                                ip_dst='10.0.0.1',
                                ip_id=102,
                                ip_ttl=64,
                                dl_vlan_enable=True,
                                vlan_vid=vlan_id1)

        pkt2 = simple_tcp_packet(eth_dst=mac1,
                                 eth_src=mac2,
                                 ip_dst='10.0.0.1',
                                 ip_id=102,
                                 ip_ttl=64,
                                 dl_vlan_enable=True,
                                 vlan_vid=vlan_id2)

        try:
            print "Sending packet to Sub-port [port 1 : vlan {}] (bridge 1)".format(vlan_id1)
            send_packet(self, 0, str(pkt1))
            verify_packets(self, pkt2, [1])
            print "Success"
            print "Sending packet to Sub-port [port 3 : vlan {}] (bridge 2)".format(vlan_id1)
            send_packet(self, 2, str(pkt1))
            verify_packets(self, pkt2, [3])
            print "Success"

        finally:
            sai_thrift_remove_bridge_sub_port(self.client, bport1_id, port1)
            sai_thrift_remove_bridge_sub_port(self.client, bport2_id, port2)
            sai_thrift_remove_bridge_sub_port(self.client, bport3_id, port3)
            sai_thrift_remove_bridge_sub_port(self.client, bport4_id, port4)
            self.client.sai_thrift_remove_bridge(bridge_id1)
            self.client.sai_thrift_remove_bridge(bridge_id2)
            self.client.sai_thrift_remove_vlan(vlan_oid1)
            self.client.sai_thrift_remove_vlan(vlan_oid2)

            for port in sai_port_list:
                sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port, SAI_VLAN_TAGGING_MODE_UNTAGGED)

@group('l2')
@group('1D')
class L2BridgePortTestI(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        '''
        Create a bridge-port and verify traffic forwarding.
        Disable bridge-port admin state and verify packets are dropped.
        '''
        switch_init(self.client)
        vlan_id = 10
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        port1 = port_list[0]
        port2 = port_list[1]
	
	sai_thrift_vlan_remove_all_ports(self.client, switch.default_vlan.oid)

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
         
        # Create 1D Bridge
        bridge_id = sai_thrift_create_bridge(self.client, SAI_BRIDGE_TYPE_1D)
        
        # Create Bridge ports
        bport1_id = sai_thrift_create_bridge_sub_port(self.client, port1, bridge_id, vlan_id)
        bport2_id = sai_thrift_create_bridge_sub_port(self.client, port2, bridge_id, vlan_id)
        
        pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64,
			        dl_vlan_enable=True,
                                vlan_vid=vlan_id)
        
        try:
	    #sending packet
            send_packet(self, 0, str(pkt))
            verify_packets(self, pkt, [1])
	    
            #setting admin state value to false
            bport_attr_admin_state_value = sai_thrift_attribute_value_t(booldata=False)
            bport_attr_admin_state = sai_thrift_attribute_t(id=SAI_BRIDGE_PORT_ATTR_ADMIN_STATE,
                                                            value=bport_attr_admin_state_value)
            client.sai_thrift_set_bridge_port_attribute(bport1_id, bport_attr_admin_state)
	    
	    bport_attr_admin_state_value = sai_thrift_attribute_value_t(booldata=False)
            bport_attr_admin_state = sai_thrift_attribute_t(id=SAI_BRIDGE_PORT_ATTR_ADMIN_STATE,
                                                            value=bport_attr_admin_state_value)
            client.sai_thrift_set_bridge_port_attribute(bport2_id, bport_attr_admin_state)
	    
            send_packet(self, 0, str(pkt))
            verify_no_packet(self, pkt, 1)
        
	finally:
            sai_thrift_remove_bridge_sub_port(self.client, bport1_id, port1)
            sai_thrift_remove_bridge_sub_port(self.client, bport2_id, port2)
            self.client.sai_thrift_remove_bridge(bridge_id)
            self.client.sai_thrift_remove_vlan(vlan_oid)
            
            for port in sai_port_list:
                sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port, SAI_VLAN_TAGGING_MODE_UNTAGGED)            
     
@group('l2')
@group('1D')
class L2BridgeSubPortFDBTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print ""
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]

        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac3 = '00:33:33:33:33:33'

        sai_thrift_vlan_remove_all_ports(self.client, switch.default_vlan.oid)

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)

        bridge_id = sai_thrift_create_bridge(self.client, SAI_BRIDGE_TYPE_1D)

        bport1_id = sai_thrift_create_bridge_sub_port(self.client, port1, bridge_id, vlan_id)
        bport2_id = sai_thrift_create_bridge_sub_port(self.client, port2, bridge_id, vlan_id)
        bport3_id = sai_thrift_create_bridge_sub_port(self.client, port3, bridge_id, vlan_id)

        sai_thrift_create_fdb_bport(self.client, bridge_id, mac3, bport3_id, SAI_PACKET_ACTION_FORWARD)

        pkt1 = simple_tcp_packet(eth_dst=mac1,
                                 eth_src=mac2,
                                 ip_dst='10.0.0.1',
                                 ip_id=102,
                                 ip_ttl=64,
                                 dl_vlan_enable=True,
                                 vlan_vid=vlan_id)

        pkt2 = simple_tcp_packet(eth_dst=mac2,
                                 eth_src=mac1,
                                 ip_dst='10.0.0.2',
                                 ip_id=102,
                                 ip_ttl=64,
                                 dl_vlan_enable=True,
                                 vlan_vid=vlan_id)

        pkt3 = simple_tcp_packet(eth_dst=mac3,
                                 eth_src=mac1,
                                 ip_dst='10.0.0.3',
                                 ip_id=102,
                                 ip_ttl=64,
                                 dl_vlan_enable=True,
                                 vlan_vid=vlan_id)

        try:
            print "Sending unknown UC [{} -> {}] to port 1".format(mac2, mac1)
            send_packet(self, 0, str(pkt1))
            verify_packets(self, pkt1, [1, 2])
            print "Success"

            print "Sending packet [{} -> {}] to port 2 (forwarded via dynamic FDB entry)".format(mac1, mac2)
            send_packet(self, 1, str(pkt2))
            verify_packets(self, pkt2, [0])
            print "Success"

            print "Sending packet [{} -> {}] to port 1 (forwarded via static FDB entry)".format(mac1, mac3)
            send_packet(self, 0, str(pkt3))
            verify_packets(self, pkt3, [2])
            print "Success"
        finally:
            sai_thrift_delete_fdb(self.client, bridge_id, mac3, bport3_id)
            sai_thrift_remove_bridge_sub_port(self.client, bport1_id, port1)
            sai_thrift_remove_bridge_sub_port(self.client, bport2_id, port2)
            sai_thrift_remove_bridge_sub_port(self.client, bport3_id, port3)
            self.client.sai_thrift_remove_bridge(bridge_id)
            self.client.sai_thrift_remove_vlan(vlan_oid)

            for port in sai_port_list:
                sai_thrift_create_vlan_member(self.client, switch.default_vlan.oid, port, SAI_VLAN_TAGGING_MODE_UNTAGGED)

###########################################################
###                 L2MtuTest                           ###
###########################################################

@group('l2')
@group('mtu')
class L2MtuTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        """
        Description:
        Send untagged packet with max MTU size and verify forwarding. Add VLAN tag so
        that the total packet size exceeds the MTU value and verify behavior. Default
        action is to drop packets exceeding configured MTU value.

        Steps:
        1. Create VLAN 10 in the database.
        2. Fetch the MTU for the port P1, P2, P3
        3. Create three member port (port P1 and P2) as untagged port and port P3 as tagged port of vlan 10
        4. Set the Port VLAN ID 10 for port P1, P2 and P3
        5. Set MTU size 1500 for port P1, P2 and P3
        6. Send untagged TCP packet of size 1400 bytes on port 1.
        7. Untagged packet of size 1400 should be received at port 2 and tagged packet of vlan 10 should be received at port 2.
        8. Send untagged TCP packet of size 1500 bytes on port 1.
        9. Untagged packet of size 1500 should be received at port 2 and tagged packet of vlan 10 should not be received at port 2.
        """

        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        mac_action = SAI_PACKET_ACTION_FORWARD
        port_mtu = 1500
        port_mtu = int(port_mtu)
        port_default_mtu = []
        sai_port_list = [port1, port2, port3]

        for port in sai_port_list:
            port_attr_list = self.client.sai_thrift_get_port_attribute(port)
            attr_list = port_attr_list.attr_list
            for attribute in attr_list:
                if attribute.id == SAI_PORT_ATTR_MTU:
                    port_default_mtu.append(attribute.value.u32)

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member3 = sai_thrift_create_vlan_member(self.client, vlan_oid, port3, SAI_VLAN_TAGGING_MODE_TAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        for port in sai_port_list:
            self.client.sai_thrift_set_port_attribute(port, attr)
        """
        Setting the MTU 1500 to port p1, p2,p3
        """
        attr_mtu_value = sai_thrift_attribute_value_t(u32=port_mtu)
        attr_mtu = sai_thrift_attribute_t(id=SAI_PORT_ATTR_MTU, value=attr_mtu_value)
        for port in sai_port_list:
            self.client.sai_thrift_set_port_attribute(port, attr_mtu)

        pkt = simple_tcp_packet(pktlen=1400,
                                eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        tag_pkt = simple_tcp_packet(pktlen=1404,
                                eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                dl_vlan_enable=True,
                                vlan_vid=vlan_id,
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        pkt1 = simple_tcp_packet(pktlen=1500,
                                eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        tag_pkt1 = simple_tcp_packet(pktlen=1504,
                                eth_dst='00:22:22:22:22:22',
                                eth_src='00:11:11:11:11:11',
                                dl_vlan_enable=True,
                                vlan_vid=vlan_id,
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)

        try:
            send_packet(self, 0, str(pkt))
            verify_packet(self, pkt, 1)
            verify_packet(self, tag_pkt, 2)

            send_packet(self, 0, str(pkt1))
            verify_packet(self, pkt1, 1)
            verify_no_packet(self, tag_pkt1, 2)

        finally:
            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            for port in sai_port_list:
                self.client.sai_thrift_set_port_attribute(port, attr)

            for index, def_mtu in enumerate(port_default_mtu):
                attr_mtu_value = sai_thrift_attribute_value_t(u32=def_mtu)
                attr_mtu = sai_thrift_attribute_t(id=SAI_PORT_ATTR_MTU, value=attr_mtu_value)
                self.client.sai_thrift_set_port_attribute(sai_port_list[index], attr_mtu)

            sai_thrift_flush_fdb_by_vlan(self.client, vlan_oid)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan_member(vlan_member3)
            self.client.sai_thrift_remove_vlan(vlan_oid)

###########################################################
###                 L2MacMoveTest-I                     ###
###########################################################
@group('l2')
class L2MacMoveTestI (sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        '''
        Description:
        Create VLAN, e.g. 100 and add member ports 1, 2 and 3. Send packet on port 1 with
        src_mac=MAC1 and dst_mac= MAC2. Verify MAC1 is learnt on port 1 and packet is flooded
        to other member ports (2 and 3 in this example). Send packet on port 2 with src_mac=MAC2
        and dst_mac= MAC1. Verify MAC2 is learnt on port 2. After learning, verify that packet
        from port 1 is only forwarded to port 2 and not to port 3. Repeat the test by sending
        same packet (src_mac=MAC2 and dst_mac= MAC1), on port 3. Verify that stationmovement
        occurred and MAC2 is learnt on port 3. Packet from port 1 destined to MAC2 must be forwarded
        to port 3 and not to port 2 after the MAC-movement.

        Steps:
        1. Create VLAN 100 and associate 3 untagged ports (port 1,2,3) as the member port of VLAN 100.
        2. Set attribute value of "Port VLAN ID 100" for the ports 1, 2, 3
        3. Send packet from port 1 with src_mac=MAC1 and dst_mac= MAC2
        4. Send packet from port 2 with src_mac=MAC2 and dst_mac= MAC1.
        5. After MAC learning, repeat step 3 and verify that packet from port 1 is only forwarded to port 2 and not to port 3.
        6. Send packet (src_mac=MAC2 and dst_mac= MAC1) on port 3
        7. Send packet (src_mac=MAC1 and dst_mac=MAC2) from port 1.
        8. Clean up by remove the vlan members and the VLAN from the database.

        '''
        print 'L2 Mac Move test on Access ports 1,2 and 3'
        switch_init(self.client)
        vlan_id = 100
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        ### create vlan
        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)

        ### create vlan membership with ports
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member3 = sai_thrift_create_vlan_member(self.client, vlan_oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        ### Assign PVLAN ID with ports
        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        pkt1 = simple_tcp_packet(eth_dst=mac2,
                                eth_src=mac1,
                                ip_dst='192.168.0.2',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(eth_dst=mac2,
                                eth_src=mac1,
                                ip_dst='192.168.0.2',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)

        pkt2 = simple_tcp_packet(eth_dst=mac1,
                                eth_src=mac2,
                                ip_dst='192.168.0.1',
                                ip_src='192.168.0.2',
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt2 = simple_tcp_packet(eth_dst=mac1,
                                eth_src=mac2,
                                ip_dst='192.168.0.1',
                                ip_src='192.168.0.2',
                                ip_id=105,
                                ip_ttl=64)
        try:

            ### Sending packet from Port1
            send_packet(self, 0, str(pkt1))

            ### verify the packet @ Port2 and Port3
            verify_packets(self, exp_pkt1, [1, 2])

            ### Sending packet from Port2
            send_packet(self, 1, str(pkt2))

            ### verify the packet @ Port1
            verify_packets(self, exp_pkt2, [0])

            time.sleep(1)

            ### Sending packet from Port1
            send_packet(self, 0, str(pkt1))

            ### verify the packet @ Port2
            verify_packets(self, exp_pkt1, [1])

            ### verify no packet @ Port3
            verify_no_packet(self, exp_pkt1, 2)

            time.sleep(1)

            ### Send packet (src_mac=MAC2 and dst_mac= MAC1) on port3
            send_packet(self, 2, str(pkt2))

            ### verify the packet @ Port1
            verify_packets(self, exp_pkt2, [0])

            time.sleep(1)

            ### Send packet (src_mac=MAC1 and dst_mac=MAC2) from port 1
            send_packet(self, 0, str(pkt1))

            ### verify the packet @ Port3 & no packet @ Port2
            verify_packets(self, exp_pkt1, [2])
            verify_no_packet(self, exp_pkt1, 1)

        finally:
            ### Assign ports into default vlan
            sai_thrift_flush_fdb_by_vlan(self.client, vlan_oid)
            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)

            ### remove vlan membership from ports
            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan_member(vlan_member3)

            ### remove valn
            self.client.sai_thrift_remove_vlan(vlan_oid)

################################################################
###        L2MacMoveTest-II (With IPv4 Neighbor entry)       ###
################################################################
@group('l2')
class L2MacMoveTestII (sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        '''
        Description:
        Create a neighbor entry with MAC1. Let MAC1 be learnt on port 1 and observe traffic
        forwarding. Execute "L2MacMoveTest" and let MAC1 be learnt on 2nd port. Verify L3
        forwarding and ensure that traffic is now forwarded to the newly learnt port.

        Steps:
        1. Create VLAN 10 and associate 3 untagged ports (port 1,2,3) as the member port of VLAN 10.
        2. Set attribute value of "Port VLAN ID 10" for the ports 1,2,3
        3. Create Virtual router V1 and enable V4.
        4. Create  virtual router interfaces and set the interface type as VLAN for VLAN 10
           and create one more port4 (RIF Id2).
        5. Create IPv4 neighbor entry with MAC1 and asociate with "RIF Id1".
        6. Send test packet from port 1 with src_mac = MAC1 address.
        7. Send IP packet from port 3 with dst_mac= MAC1 and observe traffic forwarding to port 1.
        8. Send IP packet from port 2 with src_mac = MAC1 and verify the FDB entry, which have learnt via Port 2.
        9. Send again the test packet from port 3 with dst_mac = MAC1 address and verify it
            forward to port 2 and not port 1.
        10. Remove the router interface and change the port attribute to default VLAN.
        11. Remove the vlan members and VLAN 10 from the database.
        '''

        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        dmac1 = '00:33:33:33:33:33'
        dmac2 = '00:44:44:44:44:44'
        v4_enabled = 1
        v6_enabled = 0
        mac = ''
        addr_family = SAI_IP_ADDR_FAMILY_IPV4
        ip_addr1 = '192.168.0.1'
        ip_addr2 = '10.10.10.1'
        ip_addr1_subnet = '192.168.0.0'
        ip_mask1 = '255.255.0.0'

        ### create vlan
        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)

        ### create vlan membership with ports
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member3 = sai_thrift_create_vlan_member(self.client, vlan_oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        ### Assign PVLAN ID with ports
        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        ### create virtual router
        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        ### create router interfaces
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan_oid, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port4, 0, v4_enabled, v6_enabled, mac)

        ### create neighbor
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)

        try:
            # send the test packet(s)
            pkt1 = simple_tcp_packet(eth_dst=dmac2,
                                eth_src=dmac1,
                                ip_dst='192.168.0.2',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)
            exp_pkt1 = simple_tcp_packet(
                                eth_dst=dmac2,
                                eth_src=dmac1,
                                ip_dst='192.168.0.2',
                                ip_src='192.168.0.1',
                                ip_id=105,
                                ip_ttl=64)

            pkt2 = simple_tcp_packet(eth_dst=router_mac,
                                eth_src=dmac1,
                                ip_dst='192.168.0.1',
                                ip_src='10.10.10.1',
                                ip_id=105,
                                ip_ttl=64)

            exp_pkt2 = simple_tcp_packet(
                                eth_src=router_mac,
                                eth_dst=dmac1,
                                ip_dst='192.168.0.1',
                                ip_src='10.10.10.1',
                                ip_id=105,
                                ip_ttl=63)


            ### Send test packet from port 1 with src_mac = MAC1 address.
            send_packet(self, 0, str(pkt1))

            ### Verifying the traffic received @ Port2 and Port3
            verify_packets(self, exp_pkt1, [1 ,2])

            time.sleep(1)

            ### Send IP packet from port4 with dst_mac= MAC1 and observe traffic forwarding to port1.
            send_packet(self, 3, str(pkt2))

            ### Verifying the traffic @ Port1
            verify_packets(self, exp_pkt2, [0])

            time.sleep(1)

            ### Send IP packet from port 2 with src_mac = MAC1 and verify the FDB entry.
            send_packet(self, 1, str(pkt1))

            ### Verifying the traffic @ Port1 and Port3
            verify_packets(self, exp_pkt1, [0, 2])

            time.sleep(1)

            ### Send again the test packet from port 4 with dst_mac = MAC1 address and verify it forward to port 2.
            send_packet(self, 3, str(pkt2))

            ### Verifying the traffic @ Port2
            verify_packets(self, exp_pkt2, [1])

        finally:
            sai_thrift_flush_fdb_by_vlan(self.client, vlan_oid)

            ### remove neighbor and next hop entries
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)

            ### remove router interface
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)

            ### remove viratual router
            self.client.sai_thrift_remove_virtual_router(vr_id)

            ### Set the ports to default VLAN
            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan_member(vlan_member3)
            self.client.sai_thrift_remove_vlan(vlan_oid)


################################################################
###        L2MacMoveTest-III (With IPv6 Neighbor entry)       ###
################################################################

@group('l2')
class L2MacMoveTestIII (sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        '''
        Description:
        Same as L2MacMoveTest II but for IPv6 neighbor.

        Steps:
        1. Create VLAN 10 and associate 3 untagged ports (port 1,2,3) as the member port of VLAN 10.
        2. Set attribute value of "Port VLAN ID 10" for the ports 1, 2, 3
        3. Create Virtual router V1 and enable V6.
        4. Create  virtual router interfaces and set the interface type as VLAN
        5. Create IPv6 neighbor entry with MAC1 and asociate with "RIF Id1".
        6. Send test packet from port 1 with src_mac = MAC1 address.
        7. Send IP packet from port 3 with dst_mac= MAC1 and observe traffic forwarding to port 1.
        8. Send IP packet from port 2 with src_mac = MAC1 and verify the FDB entry.
        9. Send again the test packet from port 3 with dst_mac = MAC1 address and verify it forward to port 2.
        10.. Remove the router interface and change the port attribute to default VLAN.
        11. Remove the vlan members and VLAN 10 from the database.

        '''
        print 'L2 Mac Move test on Access ports 1 and 2'
        switch_init(self.client)
        vlan_id = 10
        port1 = port_list[0]
        port2 = port_list[1]
        port3 = port_list[2]
        port4 = port_list[3]
        dmac1 = '00:0a:0a:0a:00:01'
        dmac2 = '00:0b:0b:0b:00:01'
        mac=''
        v4_enabled = 0
        v6_enabled = 1

        addr_family = SAI_IP_ADDR_FAMILY_IPV6
        ip_addr1 = '2001:1000::1'
        ip_addr2 = '2001:1111::1'
        ip_addr1_subnet='2001:1000::0'
        ip_mask1 = 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:0000'


        ### create vlan
        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)

        ### create vlan membership with ports
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)
        vlan_member3 = sai_thrift_create_vlan_member(self.client, vlan_oid, port3, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        ### Assign PVLAN ID with ports
        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)
        self.client.sai_thrift_set_port_attribute(port3, attr)

        ### create virtual router
        vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        ### create router interfaces
        rif_id1 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_VLAN, 0, vlan_oid, v4_enabled, v6_enabled, mac)
        rif_id2 = sai_thrift_create_router_interface(self.client, vr_id, SAI_ROUTER_INTERFACE_TYPE_PORT, port4, 0, v4_enabled, v6_enabled, mac)

        ### create neighbor
        sai_thrift_create_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)
        sai_thrift_create_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)

        try:
            # send the test packet(s)
            pkt1 = simple_tcpv6_packet(eth_dst=dmac2,
                                eth_src=dmac1,
                                ipv6_dst='2001:1000::2',
                                ipv6_src='2001:1000::1',
                                ipv6_hlim=64)
            exp_pkt1 = simple_tcpv6_packet(
                                eth_src=dmac1,
                                eth_dst=dmac2,
                                ipv6_dst='2001:1000::2',
                                ipv6_src='2001:1000::1',
                                ipv6_hlim=64)

            pkt2 = simple_tcpv6_packet(eth_dst=router_mac,
                                eth_src=dmac1,
                                ipv6_dst='2001:1000::1',
                                ipv6_src='2001:1111::1',
                                ipv6_hlim=64)

            exp_pkt2 = simple_tcpv6_packet(
                                eth_src=router_mac,
                                eth_dst=dmac1,
                                ipv6_dst='2001:1000::1',
                                ipv6_src='2001:1111::1',
                                ipv6_hlim=63)

            ### Send test packet from port 1 with src_mac = MAC1 address.
            send_packet(self, 0, str(pkt1))

            ### Verifying the traffic @ Port2 and 3
            verify_packets(self, exp_pkt1, [1, 2])

            time.sleep(1)

            ### Send IP packet from port4 with dst_mac= MAC1 and observe traffic forwarding to port1.
            send_packet(self, 3, str(pkt2))

            ### Verifying the traffic @ Port1
            verify_packets(self, exp_pkt2, [0])

            time.sleep(1)

            ### Send IP packet from port 2 with src_mac = MAC1 and verify the FDB entry.
            send_packet(self, 1, str(pkt1))

            ### Verifying the traffic @ Port 1 and 3
            verify_packets(self, exp_pkt1, [0, 2])

            time.sleep(1)

            ### Send again the IP  packet from port 4 with dst_mac = MAC1 address and verify it forward to port 2.
            send_packet(self, 3, str(pkt2))

            ### Verifying the traffic @ Port2
            verify_packets(self, exp_pkt2, [1])

        finally:
            sai_thrift_flush_fdb_by_vlan(self.client, vlan_oid)

            ### remove neighbor and next hop entries
            sai_thrift_remove_route(self.client, vr_id, addr_family, ip_addr1_subnet, ip_mask1, rif_id1)
            sai_thrift_remove_neighbor(self.client, addr_family, rif_id1, ip_addr1, dmac1)

            ### remove router interface
            self.client.sai_thrift_remove_router_interface(rif_id1)
            self.client.sai_thrift_remove_router_interface(rif_id2)

            ### remove viratual router
            self.client.sai_thrift_remove_virtual_router(vr_id)

            ### Set the ports to default VLAN
            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)
            self.client.sai_thrift_set_port_attribute(port3, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan_member(vlan_member3)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('l2')
class L2TripleTaggedTest(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print ""
        switch_init(self.client)
        pcp_list=[0,0,0]
        cfi_list=[0,0,0]
        tpid_list=[0x8100, 0x88a8, 0x8100]
        vlan_list=[1000, 100, 10]

        vlan_id = vlan_list[0]
        tagged_packet_size = 100
        untagged_packet_size = tagged_packet_size - 4

        port1 = port_list[0]
        port2 = port_list[1]

        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        sai_thrift_create_fdb(self.client, vlan_oid, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_oid, mac2, port2, mac_action)

        pkt1 = simple_tcp_packet_ext_taglist(pktlen=tagged_packet_size,
                                             eth_dst=mac2,
                                             eth_src=mac1,
                                             dl_taglist_enable=True,
                                             dl_vlan_pcp_list=pcp_list,
                                             dl_vlan_cfi_list=cfi_list,
                                             dl_tpid_list=tpid_list,
                                             dl_vlanid_list=vlan_list,
                                             ip_dst='10.0.0.1',
                                             ip_id=101,
                                             ip_ttl=64)

        exp1 = simple_tcp_packet_ext_taglist(pktlen=untagged_packet_size,
                                             eth_dst=mac2,
                                             eth_src=mac1,
                                             dl_taglist_enable=True,
                                             dl_vlan_pcp_list=pcp_list[1:],
                                             dl_vlan_cfi_list=cfi_list[1:],
                                             dl_tpid_list=tpid_list[1:],
                                             dl_vlanid_list=vlan_list[1:],
                                             ip_dst='10.0.0.1',
                                             ip_id=101,
                                             ip_ttl=64)

        pkt2 = simple_tcp_packet_ext_taglist(pktlen=untagged_packet_size,
                                             eth_dst=mac1,
                                             eth_src=mac2,
                                             dl_taglist_enable=True,
                                             dl_vlan_pcp_list=pcp_list[1:],
                                             dl_vlan_cfi_list=cfi_list[1:],
                                             dl_tpid_list=tpid_list[1:],
                                             dl_vlanid_list=vlan_list[1:],
                                             ip_dst='10.0.0.1',
                                             ip_id=101,
                                             ip_ttl=64)

        exp2 = simple_tcp_packet_ext_taglist(pktlen=tagged_packet_size,
                                             eth_dst=mac1,
                                             eth_src=mac2,
                                             dl_taglist_enable=True,
                                             dl_vlan_pcp_list=pcp_list,
                                             dl_vlan_cfi_list=cfi_list,
                                             dl_tpid_list=tpid_list,
                                             dl_vlanid_list=vlan_list,
                                             ip_dst='10.0.0.1',
                                             ip_id=101,
                                             ip_ttl=64)

        try:
            print "Sending tagged(vlan%d) packet port 1 -> port 2)" % vlan_id
            send_packet(self, 0, str(pkt1))
            verify_packets(self, exp1, [1])
            print "Success1"

            print "Sending untaged packet port 2 -> port 1)"
            send_packet(self, 1, str(pkt2))
            verify_packets(self, exp2, [0])
            print "Success2"

        finally:
            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_oid, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)

@group('l2')
class L2TripleTaggedTest2(sai_base_test.ThriftInterfaceDataPlane):
    def runTest(self):
        print ""

        switch_init(self.client)
        pcp_list = [0,0,0]
        cfi_list = [0,0,0]
        tpid_list = [0x8100, 0x88a8, 0x8100]
        vlan_list = [1000, 100, 10]
        vlan_id = vlan_list[0]

        tagged_packet_size = 0x50
        untagged_packet_size = tagged_packet_size - 4

        port1 = port_list[0]
        port2 = port_list[1]

        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        mac_action = SAI_PACKET_ACTION_FORWARD

        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        vlan_member1 = sai_thrift_create_vlan_member(self.client, vlan_oid, port1, SAI_VLAN_TAGGING_MODE_TAGGED)
        vlan_member2 = sai_thrift_create_vlan_member(self.client, vlan_oid, port2, SAI_VLAN_TAGGING_MODE_UNTAGGED)

        attr_value = sai_thrift_attribute_value_t(u16=vlan_id)
        attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
        self.client.sai_thrift_set_port_attribute(port1, attr)
        self.client.sai_thrift_set_port_attribute(port2, attr)

        sai_thrift_create_fdb(self.client, vlan_oid, mac1, port1, mac_action)
        sai_thrift_create_fdb(self.client, vlan_oid, mac2, port2, mac_action)

        pkt1 = simple_eth_raw_packet_with_taglist(pktlen=tagged_packet_size,
                                                  eth_dst=mac2,
                                                  eth_src=mac1,
                                                  dl_taglist_enable=True,
                                                  dl_vlan_pcp_list=pcp_list,
                                                  dl_vlan_cfi_list=cfi_list,
                                                  dl_tpid_list=tpid_list,
                                                  dl_vlanid_list=vlan_list)

        exp1 = simple_eth_raw_packet_with_taglist(pktlen=untagged_packet_size,
                                                  eth_dst=mac2,
                                                  eth_src=mac1,
                                                  dl_taglist_enable=True,
                                                  dl_vlan_pcp_list=pcp_list[1:],
                                                  dl_vlan_cfi_list=cfi_list[1:],
                                                  dl_tpid_list=tpid_list[1:],
                                                  dl_vlanid_list=vlan_list[1:])

        pkt2 = simple_eth_raw_packet_with_taglist(pktlen=untagged_packet_size,
                                                  eth_dst=mac1,
                                                  eth_src=mac2,
                                                  dl_taglist_enable=True,
                                                  dl_vlan_pcp_list=pcp_list[1:],
                                                  dl_vlan_cfi_list=cfi_list[1:],
                                                  dl_tpid_list=tpid_list[1:],
                                                  dl_vlanid_list=vlan_list[1:])

        exp2 = simple_eth_raw_packet_with_taglist(pktlen=tagged_packet_size,
                                                  eth_dst=mac1,
                                                  eth_src=mac2,
                                                  dl_taglist_enable=True,
                                                  dl_vlan_pcp_list=pcp_list,
                                                  dl_vlan_cfi_list=cfi_list,
                                                  dl_tpid_list=tpid_list,
                                                  dl_vlanid_list=vlan_list)

        try:
            print "Sending tagged(vlan%d) packet port 1 -> port 2)" % vlan_id
            send_packet(self, 0, str(pkt1))
            verify_packets(self, exp1, [1])
            print "Success1"

            print "Sending untaged packet port 2 -> port 1)"
            send_packet(self, 1, str(pkt2))
            verify_packets(self, exp2, [0])
            print "Success2"

        finally:
            sai_thrift_delete_fdb(self.client, vlan_oid, mac1, port1)
            sai_thrift_delete_fdb(self.client, vlan_oid, mac2, port2)

            attr_value = sai_thrift_attribute_value_t(u16=1)
            attr = sai_thrift_attribute_t(id=SAI_PORT_ATTR_PORT_VLAN_ID, value=attr_value)
            self.client.sai_thrift_set_port_attribute(port1, attr)
            self.client.sai_thrift_set_port_attribute(port2, attr)

            self.client.sai_thrift_remove_vlan_member(vlan_member1)
            self.client.sai_thrift_remove_vlan_member(vlan_member2)
            self.client.sai_thrift_remove_vlan(vlan_oid)

