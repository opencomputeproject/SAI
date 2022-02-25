# Copyright 2021-present Intel Corporation.
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

'''
Thrift SAI interface LAG tests
'''

import binascii

from sai_thrift.sai_headers import *
from sai_base_test import *


@group("draft")
class LAGCreateLagMember(SaiHelper):
    '''
    Test LAG member creation
    '''

    def runTest(self):
        self.createRemoveLagMemberTest()

    def createRemoveLagMemberTest(self):
        '''
        Create and remove LAG members test
        '''

        print("createRemoveLagMemberTest()")

        portlist = sai_thrift_object_list_t(count=100)
        lag3 = sai_thrift_create_lag(self.client)

        # verify LAG and LAG members
        attr_list = sai_thrift_get_lag_attribute(
            self.client, lag3, port_list=portlist)
        lag_members = attr_list["SAI_LAG_ATTR_PORT_LIST"].idlist
        self.assertTrue(len(lag_members) == 0,
                        "List of lag members should be empty")
        count = attr_list["SAI_LAG_ATTR_PORT_LIST"].count
        self.assertTrue(count == 0, "Counter should be equal to 0")

        lag3_member24 = sai_thrift_create_lag_member(
            self.client, lag_id=lag3, port_id=self.port24)
        # verify LAG and LAG members
        attr_list = sai_thrift_get_lag_attribute(
            self.client, lag3, port_list=portlist)
        lag_members = attr_list["SAI_LAG_ATTR_PORT_LIST"].idlist
        self.assertTrue(lag_members[0] == lag3_member24,
                        "Lag member has not been created properly")
        count = attr_list["SAI_LAG_ATTR_PORT_LIST"].count
        self.assertTrue(count == 1, "Counter should be equal to 1")
        attr_list = sai_thrift_get_lag_member_attribute(
            self.client, lag3_member24, lag_id=True, port_id=True)
        assert attr_list["SAI_LAG_MEMBER_ATTR_LAG_ID"] == lag3
        assert attr_list["SAI_LAG_MEMBER_ATTR_PORT_ID"] == self.port24

        # should fail as member already exists
        lag_member_fail = sai_thrift_create_lag_member(
            self.client, lag_id=lag3, port_id=self.port24)
        self.assertEqual(lag_member_fail, 0)
        # verify LAG and LAG members
        attr_list = sai_thrift_get_lag_attribute(
            self.client, lag3, port_list=portlist)
        lag_members = attr_list["SAI_LAG_ATTR_PORT_LIST"].idlist
        self.assertTrue(lag_members[0] == lag3_member24,
                        "New lag member shouldn't be created")
        count = attr_list["SAI_LAG_ATTR_PORT_LIST"].count
        self.assertTrue(count == 1, "Counter should be equal to 1")

        status = sai_thrift_remove_lag_member(self.client, lag3_member24)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # verify LAG and LAG members
        attr_list = sai_thrift_get_lag_attribute(
            self.client, lag3, port_list=portlist)
        lag_members = attr_list["SAI_LAG_ATTR_PORT_LIST"].idlist
        self.assertTrue(len(lag_members) == 0,
                        "Lag member has not been deleted properly")
        count = attr_list["SAI_LAG_ATTR_PORT_LIST"].count
        self.assertTrue(count == 0, "Counter should be equal to 0")

        status = sai_thrift_remove_lag(self.client, lag3)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        status = sai_thrift_remove_lag(self.client, lag3)
        self.assertNotEqual(status, SAI_STATUS_SUCCESS)


def print_ports_stats(client, ports):
    '''
    Print ports statistics

    Args:
        client (obj): client object
        ports (list): list of ports to print statistics
    '''

    for port in ports:
        counter_results = sai_thrift_get_port_stats(client, port)

        print("PORT%d if_in_discards=%d if_out_discards=%d "
              "if_in_ucast_pakts=%d if_out_ucast_pkts=%d" % (
                  (0xFF & port),
                  counter_results["SAI_PORT_STAT_IF_IN_DISCARDS"],
                  counter_results["SAI_PORT_STAT_IF_IN_DISCARDS"],
                  counter_results["SAI_PORT_STAT_IF_IN_UCAST_PKTS"],
                  counter_results["SAI_PORT_STAT_IF_OUT_UCAST_PKTS"]))
        sum = 0
        i = 0
    for cnt in counter_results:
        sum = +counter_results[cnt]
        i = i + 1
    print("SUM=%d (i=%d)" % (sum, i))


@group("draft")
class LAGDisableIngressLagMember(SaiHelper):
    '''
    Test Disable Ingress LAG member feature
    '''

    def runTest(self):
        print("disableIngressLagMember")

        vlan_id = 10
        src_mac = '00:11:11:11:11:11'
        dst_mac = '00:22:22:22:22:22'

        pkt = simple_udp_packet(eth_dst=dst_mac, eth_src=src_mac, pktlen=100)

        tag_pkt = simple_udp_packet(eth_dst=dst_mac,
                                    eth_src=src_mac,
                                    dl_vlan_enable=True,
                                    vlan_vid=vlan_id,
                                    pktlen=104)

        print("Sending packet on port %d; %s -> %s - will flood"
              % (self.dev_port0, src_mac, dst_mac))
        send_packet(self, self.dev_port0, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [tag_pkt, pkt],
            [[self.dev_port1],
             [self.dev_port4, self.dev_port5, self.dev_port6]])

        print("Sending packet on port %d; %s -> %s - will flood"
              % (self.dev_port1, src_mac, dst_mac))
        send_packet(self, self.dev_port1, tag_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt, pkt],
            [[self.dev_port0],
             [self.dev_port4, self.dev_port5, self.dev_port6]])

        print("Sending packet on port %d; %s -> %s - will flood"
              % (self.dev_port4, src_mac, dst_mac))
        send_packet(self, self.dev_port4, tag_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [tag_pkt, pkt], [[self.dev_port1], [self.dev_port0]])

        # disable LAG Member
        print("Set Disable_Ingress_LAG_member 4 to True")
        status = sai_thrift_set_lag_member_attribute(
            self.client,
            self.lag1_member4,
            ingress_disable=True,
            egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        print("Sending packet on port %d; %s -> %s - "
              "should drop on ingress" % (self.dev_port4, src_mac, dst_mac))

        send_packet(self, self.dev_port4, pkt)
        verify_no_other_packets(self)
        print("\tPacket dropped")

        print("LAG ingress_disable=false of the LAG member")

        # Enable LAG Member again
        print("Set Disable_Ingress_LAG_member 4 to False")
        status = sai_thrift_set_lag_member_attribute(
            self.client,
            self.lag1_member4,
            ingress_disable=False,
            egress_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        print("Sending packet on port %d; %s -> %s - will flood"
              % (self.dev_port4, src_mac, dst_mac))
        send_packet(self, self.dev_port4, pkt)
        print_ports_stats(self.client,
                          [self.port0,
                           self.port1,
                           self.port2,
                           self.port3,
                           self.port4,
                           self.port5,
                           self.port6])
        verify_each_packet_on_multiple_port_lists(
            self, [tag_pkt, tag_pkt, pkt],
            [[self.dev_port0], [self.dev_port1]])

    def tearDown(self):
        sai_thrift_set_lag_member_attribute(
            self.client,
            self.lag1_member4,
            ingress_disable=False,
            egress_disable=False)

        super(LAGDisableIngressLagMember, self).tearDown()


@group("draft")
class LAGDisableEgressLagMember(SaiHelper):
    '''
    Test DisableEgress LAG member feature
    '''

    def runTest(self):
        self.disableEgressLagMemberTest()
        self.multipleVlanTest()
        self.lagMemberActivateFloodTest()
        self.lagMemberActivateBridgeTest()

    def lagMemberActivateBridgeTest(self):
        '''
        The LAG mamber activate Bridge test
        '''

        print("lagMemberActivateBridgeTest()")

        try:
            lag1_dev_ports = [
                self.dev_port4,
                self.dev_port5,
                self.dev_port6,
                self.dev_port1]
            lag1_member1 = sai_thrift_create_lag_member(
                self.client, lag_id=self.lag1, port_id=self.port1)
            fdb_entry = sai_thrift_fdb_entry_t(
                switch_id=self.switch_id,
                mac_address='00:22:22:22:22:22',
                bv_id=self.vlan10)
            sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=self.lag1_bp,
                packet_action=SAI_PACKET_ACTION_FORWARD)

            max_itrs = 48

            def packet_test():
                '''
                Packet test function that requests max_itrs packets
                and verifies the number of packets received per LAG member

                Returns:
                    list: list of packets counts per LAG member
                '''
                pkt = simple_tcp_packet(
                    eth_src='00:11:11:11:11:11',
                    eth_dst='00:22:22:22:22:22',
                    ip_dst='10.10.10.1',
                    ip_src='192.168.8.1',
                    ip_id=109,
                    ip_ttl=64)
                exp_pkt = simple_tcp_packet(
                    eth_src='00:11:11:11:11:11',
                    eth_dst='00:22:22:22:22:22',
                    ip_dst='10.10.10.1',
                    ip_src='192.168.8.1',
                    ip_id=109,
                    ip_ttl=64)
                count = [0, 0, 0, 0]
                dst_ip = int(
                    binascii.hexlify(
                        socket.inet_aton('10.10.10.1')), 16)
                src_ip = int(
                    binascii.hexlify(
                        socket.inet_aton('192.168.8.1')), 16)
                for _ in range(0, max_itrs):
                    dst_ip_addr = socket.inet_ntoa(
                        binascii.unhexlify(format(dst_ip, 'x').zfill(8)))
                    src_ip_addr = socket.inet_ntoa(
                        binascii.unhexlify(format(src_ip, 'x').zfill(8)))
                    pkt['IP'].src = src_ip_addr
                    pkt['IP'].dst = dst_ip_addr
                    exp_pkt['IP'].src = src_ip_addr
                    exp_pkt['IP'].dst = dst_ip_addr
                    send_packet(self, self.dev_port0, pkt)
                    rcv_idx = verify_any_packet_any_port(
                        self, [exp_pkt, exp_pkt, exp_pkt, exp_pkt],
                        lag1_dev_ports)
                    count[rcv_idx] += 1
                    dst_ip += 1
                    src_ip += 1
                return count

            count = packet_test()
            print('Test with 4 mbrs enabled:', count)

            for i in range(0, 4):
                self.assertTrue((count[i] >= ((max_itrs / 4) * 0.5)),
                                "Not all paths are equally balanced")
            status = sai_thrift_set_lag_member_attribute(
                self.client,
                lag1_member1,
                egress_disable=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            count = packet_test()
            print('Test with 3 mbrs enabled after 4th member disabled:', count)
            for i in range(0, 3):
                self.assertTrue((count[i] >= ((max_itrs / 3) * 0.5)),
                                "Not all paths are equally balanced")
            self.assertEqual(count[3], 0)
            status = sai_thrift_set_lag_member_attribute(
                self.client,
                self.lag1_member6,
                egress_disable=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            count = packet_test()
            print('Test with 2 mbrs enabled after 3rd member disabled:', count)
            for i in range(0, 2):
                self.assertTrue((count[i] >= ((max_itrs / 2) * 0.5)),
                                "Not all paths are equally balanced")
            self.assertEqual(count[2], 0)
            self.assertEqual(count[3], 0)
            status = sai_thrift_set_lag_member_attribute(
                self.client,
                self.lag1_member6,
                egress_disable=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            count = packet_test()
            print('Test with 3 mbrs enabled after 3rd member enabled:', count)
            for i in [0, 1, 2]:
                self.assertTrue((count[i] >= ((max_itrs / 3) * 0.5)),
                                "Not all paths are equally balanced")
            self.assertEqual(count[3], 0)
            status = sai_thrift_set_lag_member_attribute(
                self.client,
                lag1_member1,
                egress_disable=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            count = packet_test()
            print('Test with 4 mbrs enabled after 4th member enabled:', count)
            for i in range(0, 4):
                self.assertTrue((count[i] >= ((max_itrs / 4) * 0.5)),
                                "Not all paths are equally balanced")
        finally:
            sai_thrift_remove_lag_member(self.client, lag1_member1)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)

    def lagMemberActivateFloodTest(self):
        '''
        The LAG mamber activate Flood test
        '''
        print("lagMemberActivateFloodTest()")

        try:
            l1 = [self.dev_port4, self.dev_port5, self.dev_port6]
            max_itrs = 4

            def packet_test():
                '''
                Packet test function that requests max_itrs packets
                and verifies the number of packets received per LAG member

                Returns:
                    list: list of packets counts per LAG member
                '''
                pkt = simple_arp_packet(
                    arp_op=1,
                    pktlen=100,
                    eth_src='00:11:11:11:11:11',
                    hw_snd='00:11:11:11:11:11',
                    ip_snd='10.10.10.1',
                    ip_tgt='10.10.10.2')
                tagged_pkt = simple_arp_packet(
                    arp_op=1,
                    vlan_vid=10,
                    pktlen=104,
                    eth_src='00:11:11:11:11:11',
                    hw_snd='00:11:11:11:11:11',
                    ip_snd='10.10.10.1',
                    ip_tgt='10.10.10.2')
                count = [0, 0, 0, 0]
                for _ in range(0, max_itrs):
                    send_packet(self, self.dev_port0, pkt)
                    rcv_idx = verify_each_packet_on_multiple_port_lists(
                        self,
                        [pkt, tagged_pkt],
                        [l1, [self.dev_port1]])
                    for v in rcv_idx[0]:
                        count[v] += 1
                return count

            mbrs = {
                0: self.lag1_member4,
                1: self.lag1_member5,
                2: self.lag1_member6}
            disabled_mbrs = set()
            # for each iteration
            #  - check if packets are recieved on only LAG member
            #  - check if packets are not recieved on disabled LAG member
            count = packet_test()
            self.assertEqual(len([i for i in count if i != 0]), 1)
            print('Test with 3 mbrs enabled:', count)
            index = [i for i in range(len(count)) if count[i] != 0]
            status = sai_thrift_set_lag_member_attribute(
                self.client,
                mbrs[index[0]],
                egress_disable=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print('Disabled LAG member %d' % (index[0]))
            disabled_mbrs.add(index[0])

            count = packet_test()
            self.assertEqual(len([i for i in count if i != 0]), 1)
            print('Test with 2 mbrs enabled:', count)
            index = [i for i in range(len(count)) if count[i] != 0]
            self.assertNotIn(index[0], disabled_mbrs)
            status = sai_thrift_set_lag_member_attribute(
                self.client,
                mbrs[index[0]],
                egress_disable=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print('Disabled LAG member %d' % (index[0]))
            disabled_mbrs.add(index[0])

            count = packet_test()
            self.assertEqual(len([i for i in count if i != 0]), 1)
            print('Test with 1 mbrs enabled:', count)
            index = [i for i in range(len(count)) if count[i] != 0]
            self.assertNotIn(index[0], disabled_mbrs)
            status = sai_thrift_set_lag_member_attribute(
                self.client,
                mbrs[index[0]],
                egress_disable=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

        finally:
            for mbr in mbrs.values():
                sai_thrift_set_lag_member_attribute(
                    self.client,
                    mbr,
                    egress_disable=False)

    def disableEgressLagMemberTest(self):
        '''
        LAG disable egress LAG member test
        '''

        print("disableEgressLagMemberTest")
        vlan_id = 10
        src_mac = '00:11:11:11:11:11'
        dst_mac = '00:22:22:22:22:22'

        pkt = simple_udp_packet(eth_dst=dst_mac, eth_src=src_mac, pktlen=100)

        tag_pkt = simple_udp_packet(eth_dst=dst_mac,
                                    eth_src=src_mac,
                                    dl_vlan_enable=True,
                                    vlan_vid=vlan_id,
                                    pktlen=104)

        print("Sending packet on port %d; %s -> %s - will flood"
              % (self.dev_port0, src_mac, dst_mac))
        send_packet(self, self.dev_port0, pkt)

        verify_each_packet_on_multiple_port_lists(
            self, [tag_pkt, pkt],
            [[self.dev_port1],
             [self.dev_port4, self.dev_port5, self.dev_port6]])

        print("Sending packet on port %d; %s -> %s - will flood"
              % (self.dev_port1, src_mac, dst_mac))
        send_packet(self, self.dev_port1, tag_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt, pkt],
            [[self.dev_port0],
             [self.dev_port4, self.dev_port5, self.dev_port6]])

        print("Sending packet on port %d; %s -> %s - will flood"
              % (self.dev_port4, src_mac, dst_mac))
        send_packet(self, self.dev_port4, tag_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [tag_pkt, pkt], [[self.dev_port1], [self.dev_port0]])

        # disable LAG Member
        print("Set Disable_Egress_LAG_member 4 and 5 to True")
        status = sai_thrift_set_lag_member_attribute(
            self.client,
            self.lag1_member4,
            ingress_disable=True,
            egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_lag_member_attribute(
            self.client,
            self.lag1_member5,
            ingress_disable=True,
            egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        print("Sending packet on port %d; %s -> %s - will flood to enabled "
              "ports" % (self.dev_port1, src_mac, dst_mac))
        send_packet(self, self.dev_port1, tag_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt, pkt], [[self.dev_port0], [self.dev_port6]])

        status = sai_thrift_set_lag_member_attribute(
            self.client,
            self.lag1_member6,
            ingress_disable=True,
            egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("Sending packet on port %d; %s -> %s - will flood to enabled "
              "ports" % (self.dev_port1, src_mac, dst_mac))
        send_packet(self, self.dev_port1, tag_pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [pkt], [[self.dev_port0]])

        print("LAG Egress_disable=false of the LAG member ")

        # Enable LAG Member again
        print("Set Disable_Ingress_LAG_member 4,5,6 to False")
        status = sai_thrift_set_lag_member_attribute(
            self.client,
            self.lag1_member4,
            ingress_disable=False,
            egress_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_lag_member_attribute(
            self.client,
            self.lag1_member5,
            ingress_disable=False,
            egress_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_lag_member_attribute(
            self.client,
            self.lag1_member6,
            ingress_disable=False,
            egress_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        print("Sending packet on port %d; %s -> %s - will flood"
              % (self.dev_port0, src_mac, dst_mac))
        send_packet(self, self.dev_port0, pkt)
        verify_each_packet_on_multiple_port_lists(
            self, [tag_pkt, pkt],
            [[self.dev_port1],
             [self.dev_port4, self.dev_port5, self.dev_port6]])

    def multipleVlanTest(self):
        '''
        Verify LAG members assigned to multiple vlans
        '''

        print("multipleVlanTest")
        vlan10_id = 10
        vlan20_id = 20
        src_mac = '00:11:11:11:11:11'
        dst_mac = '00:22:22:22:22:22'

        pkt = simple_udp_packet(eth_dst=dst_mac, eth_src=src_mac, pktlen=100)

        tag_pkt_vlan10 = simple_udp_packet(eth_dst=dst_mac,
                                           eth_src=src_mac,
                                           dl_vlan_enable=True,
                                           vlan_vid=vlan10_id,
                                           pktlen=104)
        tag_pkt_vlan20 = simple_udp_packet(eth_dst=dst_mac,
                                           eth_src=src_mac,
                                           dl_vlan_enable=True,
                                           vlan_vid=vlan20_id,
                                           pktlen=104)
        tag_pkt_vlan50 = simple_udp_packet(eth_dst=dst_mac,
                                           eth_src=src_mac,
                                           dl_vlan_enable=True,
                                           vlan_vid=50,
                                           pktlen=104)

        sai_thrift_set_lag_attribute(
            self.client, self.lag1, port_vlan_id=vlan20_id)
        vlan20_member3 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan20,
            bridge_port_id=self.lag1_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        try:
            print("Sending untagged packet on port %d; %s -> %s - "
                  "will flood" % (self.dev_port0, src_mac, dst_mac))
            send_packet(self, self.dev_port0, pkt)

            verify_each_packet_on_multiple_port_lists(
                self, [tag_pkt_vlan10, pkt],
                [[self.dev_port1],
                 [self.dev_port4, self.dev_port5, self.dev_port6]])

            print("Sending vlan10 tagged packet on port %d; %s -> %s - "
                  "will flood" % (self.dev_port0, src_mac, dst_mac))
            send_packet(self, self.dev_port0, tag_pkt_vlan10)

            verify_each_packet_on_multiple_port_lists(
                self, [tag_pkt_vlan10, pkt],
                [[self.dev_port1],
                 [self.dev_port4, self.dev_port5, self.dev_port6]])

            print("Sending untagged packet on port %d; %s -> %s - "
                  "will flood" % (self.dev_port2, src_mac, dst_mac))
            send_packet(self, self.dev_port2, pkt)

            verify_each_packet_on_multiple_port_lists(
                self, [tag_pkt_vlan20, tag_pkt_vlan20, tag_pkt_vlan20],
                [[self.dev_port3],
                 [self.dev_port4, self.dev_port5, self.dev_port6],
                 [self.dev_port7, self.dev_port8, self.dev_port9]])

            print("Sending vlan20 tagged packet on port %d; %s -> %s - "
                  "will flood" % (self.dev_port2, src_mac, dst_mac))
            send_packet(self, self.dev_port2, tag_pkt_vlan20)

            verify_each_packet_on_multiple_port_lists(
                self, [tag_pkt_vlan20, tag_pkt_vlan20, tag_pkt_vlan20],
                [[self.dev_port3],
                 [self.dev_port4, self.dev_port5, self.dev_port6],
                 [self.dev_port7, self.dev_port8, self.dev_port9]])

            print("Sending vlan50 tagged packet on port %d; %s -> %s - "
                  "should drop" % (self.dev_port2, src_mac, dst_mac))
            send_packet(self, self.dev_port2, tag_pkt_vlan50)

            verify_no_other_packets(self, timeout=1)

            print("Sending vlan50 tagged packet on port %d; %s -> %s - "
                  "will drop" % (self.dev_port0, src_mac, dst_mac))
            send_packet(self, self.dev_port0, tag_pkt_vlan50)

            verify_no_other_packets(self, timeout=1)

        finally:
            sai_thrift_remove_vlan_member(self.client, vlan20_member3)
            sai_thrift_set_lag_attribute(
                self.client, self.lag1, port_vlan_id=0)


@group("draft")
class LAGAttrPortList(SaiHelper):
    '''
    Test LAG port list attribute
    '''

    def runTest(self):
        print("LAGAttrPortList")
        portlist = sai_thrift_object_list_t(count=100)
        # LAG1
        attr_list = sai_thrift_get_lag_attribute(
            self.client, self.lag1, port_list=portlist)
        lag_members = attr_list["SAI_LAG_ATTR_PORT_LIST"].idlist
        count = attr_list["SAI_LAG_ATTR_PORT_LIST"].count
        assert count == 3
        assert self.lag1_member4 == lag_members[0]
        assert self.lag1_member5 == lag_members[1]
        assert self.lag1_member6 == lag_members[2]

        # LAG2
        attr_list = sai_thrift_get_lag_attribute(
            self.client, self.lag2, port_list=portlist)
        lag_members = attr_list["SAI_LAG_ATTR_PORT_LIST"].idlist
        count = attr_list["SAI_LAG_ATTR_PORT_LIST"].count
        assert count == 3
        assert self.lag2_member7 == lag_members[0]
        assert self.lag2_member8 == lag_members[1]
        assert self.lag2_member9 == lag_members[2]


@group("draft")
class LAGL2LoadBalancing(SaiHelper):
    '''
    Test LAG L2 load balancing
    '''

    def setUp(self):
        super(LAGL2LoadBalancing, self).setUp()

        sai_thrift_remove_vlan_member(self.client, self.vlan10_member1)
        sai_thrift_remove_bridge_port(self.client, self.port1_bp)

        print("Setting the LAG hash fields..")
        attr_list = sai_thrift_get_switch_attribute(self.client, lag_hash=True)
        print('switch lag_hash attr:', attr_list)
        hash_id = attr_list['SAI_SWITCH_ATTR_LAG_HASH']
        print("hash id =0x%x" % (hash_id))

        print("Getting the attribute list..")
        hash_attr_list = []
        s32list = sai_thrift_s32_list_t(count=100, int32list=hash_attr_list)
        hash_data = sai_thrift_get_hash_attribute(
            self.client, hash_id, native_hash_field_list=s32list)
        data_val = hash_data['native_hash_field_list'].int32list
        print('hash_data: ', hash_data)
        print('hash_data: ', data_val)

        hash_attr_list = [SAI_NATIVE_HASH_FIELD_DST_MAC]
        hash_field_list = sai_thrift_s32_list_t(
            count=len(hash_attr_list), int32list=hash_attr_list)
        status = sai_thrift_set_hash_attribute(
            self.client, hash_id, native_hash_field_list=hash_field_list)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        hash_field_list = sai_thrift_s32_list_t(
            count=len(hash_attr_list), int32list=hash_attr_list)
        hash_data = sai_thrift_get_hash_attribute(
            self.client, hash_id, native_hash_field_list=hash_field_list)
        data_val = hash_data['native_hash_field_list'].int32list
        print('hash_data', hash_data)
        print('hash_val: ', data_val)

    def runTest(self):
        print("LAGL2LoadBalancing")

        try:
            mac_list = []
            max_itrs = 150
            for i in range(0, max_itrs):
                mac_list.append("00:11:11:%02x:%02x:%02x" %
                                (0xFF & (i * 17), i + 1, 0xFF & (i + 17)))

            count = [0, 0, 0]
            for i in range(0, max_itrs):
                dmac = mac_list[i]
                pkt = simple_udp_packet(eth_dst=dmac,
                                        eth_src='00:22:22:22:22:22',
                                        vlan_vid=10,
                                        pktlen=100)

                exp_pkt = simple_udp_packet(eth_dst=dmac,
                                            eth_src='00:22:22:22:22:22',
                                            pktlen=100)

                send_packet(self, self.dev_port0, pkt)
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt],
                    [self.dev_port4, self.dev_port5, self.dev_port6],
                    timeout=5)
                count[rcv_idx] += 1

            print(count)
            for i in range(0, 3):
                self.assertTrue((count[i] >= ((max_itrs / 3) * 0.8)),
                                "Not all paths are equally balanced")

        finally:
            self.port1_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=self.port1,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            self.vlan10_member1 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan10,
                bridge_port_id=self.port1_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)


@group("draft")
class LAGL3LoadBalancing(SaiHelper):
    '''
    Test LAG L3 load balancing
    '''

    max_itrs = 100

    def setUp(self):
        super(LAGL3LoadBalancing, self).setUp()

        sai_thrift_remove_vlan_member(self.client, self.vlan10_member1)
        sai_thrift_remove_bridge_port(self.client, self.port1_bp)

    def runTest(self):
        self.l3LoadBalancingDisableMembersTest()
        self.l3LoadBalancingRemovedMembersTest()

    # Try all LAG members
    def lagL3LoadBalancePacketTest(self, exp_ports, traffic=True):
        '''
        LAG L3 IPv4 load balancing packet test function that
        requests max_itrs packets and verifies the number of
        packets received per LAG member

        Args:
            exp_ports (list): list of expected egress ports
            traffic (bool): an argument if egress traffic is expected
        Returns:
            list: list of packets counts per LAG member
        '''

        dst_ip = int(binascii.hexlify(socket.inet_aton('10.10.10.1')), 16)
        count = [0, 0, 0]
        for _ in range(0, self.max_itrs):
            dst_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
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

            send_packet(self, self.dev_port0, pkt)
            if traffic:
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt], exp_ports, timeout=5)
                count[rcv_idx] += 1
            else:
                verify_no_other_packets(self)
            dst_ip += 1
        return count

    def l3LoadBalancingDisableMembersTest(self):
        '''
        L3 Load balancing simple for disabled egress LAG members
        '''

        print("l3LoadBalancingDisableMembersTest")
        try:
            print("Verify L3 load balancing with all members active")
            count = self.lagL3LoadBalancePacketTest(
                [self.dev_port4, self.dev_port5, self.dev_port6])
            print(count)
            for i in range(0, 3):
                self.assertTrue((count[i] >= ((self.max_itrs / 3) * 0.7)),
                                "Not all paths are equally balanced")

            # Disable one member
            print("Disable LAG member 6")
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member6, egress_disable=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("Verify L3 load balancing with 2 members active")
            count = self.lagL3LoadBalancePacketTest(
                [self.dev_port4, self.dev_port5])
            print(count)
            for i in range(0, 2):
                self.assertTrue((count[i] >= ((self.max_itrs / 2) * 0.7)),
                                "Not all paths are equally balanced")
            self.assertTrue(count[2] == 0,
                            "Disabled LAG member should not allow traffic")

            # Disable one member
            print("Disable LAG member 4")
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member4, egress_disable=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("Verify L3 load balancing with 1 member active")
            count = self.lagL3LoadBalancePacketTest([self.dev_port5])
            print(count)
            self.assertTrue(
                count[0] == self.max_itrs,
                "Enabled LAG member should take 100% traffic allow traffic")
            self.assertTrue(count[1] == 0,
                            "Disabled LAG member should not allow traffic")
            self.assertTrue(count[2] == 0,
                            "Disabled LAG member should not allow traffic")

            # Disable one member
            print("Disable LAG member 5, No traffic expected")
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member5, egress_disable=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("Verify L3 load balancing with 0 member active")
            count = self.lagL3LoadBalancePacketTest([], traffic=False)
            print(count)
            self.assertTrue(count[0] == 0,
                            "Disabled LAG member should not allow traffic")
            self.assertTrue(count[1] == 0,
                            "Disabled LAG member should not allow traffic")
            self.assertTrue(count[2] == 0,
                            "Disabled LAG member should not allow traffic")

            # Enable 2 members
            print("Enable LAG member 4 and 5")
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member4, egress_disable=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member5, egress_disable=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("Verify L3 load balancing with 2 members active")
            count = self.lagL3LoadBalancePacketTest(
                [self.dev_port4, self.dev_port5])
            print(count)
            for i in range(0, 2):
                self.assertTrue((count[i] >= ((self.max_itrs / 2) * 0.8)),
                                "Not all paths are equally balanced")
            self.assertTrue(count[2] == 0,
                            "Disabled LAG member should not allow traffic")

        finally:
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member4, egress_disable=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member5, egress_disable=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member6, egress_disable=False)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    # Try all LAG members
    def l3LoadBalancingRemovedMembersTest(self):
        '''
            L3 Load balancing - remove members

            Test verifies the LAG load balancing for scenario when
            LAG members are removed.
        '''

        print("l3LoadBalancingRemovedMembersTest")
        print("Verify L3 load balancing with all members")
        count = self.lagL3LoadBalancePacketTest(
            [self.dev_port4, self.dev_port5, self.dev_port6])
        print(count)
        for i in range(0, 3):
            self.assertTrue((count[i] >= ((self.max_itrs / 3) * 0.8)),
                            "Not all paths are equally balanced")

        # Remove one member
        print("Remove LAG member 6")
        status = sai_thrift_remove_lag_member(self.client, self.lag1_member6)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("Verify L3 load balancing with 2 members")
        count = self.lagL3LoadBalancePacketTest(
            [self.dev_port4, self.dev_port5])
        print(count)
        for i in range(0, 2):
            self.assertTrue((count[i] >= ((self.max_itrs / 2) * 0.8)),
                            "Not all paths are equally balanced")
        self.assertTrue(count[2] == 0,
                        "Disabled LAG member should not allow traffic")

        # Remove one member
        print("Remove LAG member 4")
        status = sai_thrift_remove_lag_member(self.client, self.lag1_member4)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("Verify L3 load balancing with 1 member")
        count = self.lagL3LoadBalancePacketTest([self.dev_port5])
        print(count)
        self.assertTrue(
            count[0] == self.max_itrs,
            "Disabled LAG member should not allow traffic")
        self.assertTrue(
            count[1] == 0,
            "Enabled LAG member should take 100% traffic allow traffic")
        self.assertTrue(
            count[2] == 0,
            "Disabled LAG member should not allow traffic")

        # Remove one member
        print("Remove LAG member 5, No traffic expected")
        status = sai_thrift_remove_lag_member(self.client, self.lag1_member5)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("Verify L3 load balancing with 0 member")
        count = self.lagL3LoadBalancePacketTest([], traffic=False)
        print(count)
        self.assertTrue(count[0] == 0,
                        "Disabled LAG member should not allow traffic")
        self.assertTrue(count[1] == 0,
                        "Disabled LAG member should not allow traffic")
        self.assertTrue(count[2] == 0,
                        "Disabled LAG member should not allow traffic")

        self.lag1_member4 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag1, port_id=self.port4)
        self.lag1_member6 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag1, port_id=self.port6)

        print("Verify L3 load balancing with 2 member")
        count = self.lagL3LoadBalancePacketTest(
            [self.dev_port4, self.dev_port6])
        print(count)
        for i in range(0, 2):
            self.assertTrue((count[i] >= ((self.max_itrs / 2) * 0.8)),
                            "Not all paths are equally balanced")
        self.assertTrue(count[2] == 0,
                        "Disabled LAG member should not allow traffic")

        self.lag1_member5 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag1, port_id=self.port5)
        print("Verify L3 load balancing with all member")
        count = self.lagL3LoadBalancePacketTest(
            [self.dev_port4, self.dev_port5, self.dev_port6])
        print(count)
        for i in range(0, 3):
            self.assertTrue((count[i] >= ((self.max_itrs / 3) * 0.8)),
                            "Not all paths are equally balanced")

        # Verify LAG member ingress traffic
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
        print("Sending packet port 4 (LAG member) -> port 0")
        send_packet(self, self.dev_port4, pkt)
        verify_packets(self, exp_pkt, [self.dev_port0])
        print("Sending packet port 5 (LAG member) -> port 0")
        send_packet(self, self.dev_port5, pkt)
        verify_packets(self, exp_pkt, [self.dev_port0])
        print("Sending packet port 6 (LAG member) -> port 0")
        send_packet(self, self.dev_port6, pkt)
        verify_packets(self, exp_pkt, [self.dev_port0])

    def tearDown(self):
        self.port1_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port1,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.vlan10_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.port1_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        super(LAGL3LoadBalancing, self).tearDown()


@group("draft")
class LagL3Nhop(SaiHelper):
    '''
    LAG L3 Next Hop Group test
    '''

    def setUp(self):
        super(LagL3Nhop, self).setUp()

        self.max_itrs = 50

        self.nhop10 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress('11.11.11.1'),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry10 = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress('11.11.11.1'))
        sai_thrift_create_neighbor_entry(self.client, self.neighbor_entry10,
                                         dst_mac_address='00:11:11:11:11:11')
        self.route_entry10 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('11.11.11.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry10, next_hop_id=self.nhop10)

        self.lag1_rif = sai_thrift_create_router_interface(
            self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag1)
        self.nhop1 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress('22.22.22.1'),
            router_interface_id=self.lag1_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.lag1_rif, ip_address=sai_ipaddress('22.22.22.1'))
        sai_thrift_create_neighbor_entry(self.client, self.neighbor_entry1,
                                         dst_mac_address='00:22:22:22:22:22')
        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('22.22.0.0/16'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1, next_hop_id=self.nhop1)

        self.lag2_rif = sai_thrift_create_router_interface(
            self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag2)
        self.nhop2 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress('33.33.33.1'),
            router_interface_id=self.lag2_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=self.lag2_rif, ip_address=sai_ipaddress('33.33.33.1'))
        sai_thrift_create_neighbor_entry(self.client, self.neighbor_entry2,
                                         dst_mac_address='00:33:33:33:33:33')
        self.route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('33.33.0.0/16'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry2, next_hop_id=self.nhop2)

        self.nhop3 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress('44.44.44.1'),
            router_interface_id=self.lag3_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry3 = sai_thrift_neighbor_entry_t(
            rif_id=self.lag3_rif, ip_address=sai_ipaddress('44.44.44.1'))
        sai_thrift_create_neighbor_entry(self.client, self.neighbor_entry3,
                                         dst_mac_address='00:44:44:44:44:44')
        self.route_entry3 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('44.44.0.0/16'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry3, next_hop_id=self.nhop3)

        self.nhop4 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress('55.55.55.1'),
            router_interface_id=self.lag4_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry4 = sai_thrift_neighbor_entry_t(
            rif_id=self.lag4_rif, ip_address=sai_ipaddress('55.55.55.1'))
        sai_thrift_create_neighbor_entry(self.client, self.neighbor_entry4,
                                         dst_mac_address='00:55:55:55:55:55')
        self.route_entry4 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('55.55.0.0/16'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry4, next_hop_id=self.nhop4)

    def runTest(self):
        self.runPreTest()
        self.runPostTest()

    def packetTest(self, port_list, dst_ip_prefix, dmac):
        '''
        Packet test function that requests max_itrs packets
        and verifies the number of packets received per LAG member

        Args:
            port_list (list): expected egress port list
            dst_ip_prefix (address): dst ip address prefix
            dmac (mac): test dmac
        Returns:
            list: list of packets counts per LAG member
        '''
        count = [0] * len(port_list)
        dst_ip = int(binascii.hexlify(socket.inet_aton(dst_ip_prefix)), 16)
        for _ in range(0, self.max_itrs):
            dst_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(format(dst_ip, 'x').zfill(8)))
            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src='00:11:11:11:11:11',
                ip_dst=dst_ip_addr,
                ip_src='11.11.11.1',
                ip_ttl=64)
            exp_pkt = simple_tcp_packet(
                eth_dst=dmac,
                eth_src=ROUTER_MAC,
                ip_dst=dst_ip_addr,
                ip_src='11.11.11.1',
                ip_ttl=63)
            exp_pkt_list = [exp_pkt] * len(port_list)
            send_packet(self, self.dev_port10, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, exp_pkt_list, port_list)
            count[rcv_idx] += 1
            dst_ip += 1
        return count

    def runPreTest(self):
        '''
        Runs verification tests for number of different LAG ports
        '''
        print('Pre warm reboot tests')
        plist = [self.dev_port4, self.dev_port5, self.dev_port6]
        count = self.packetTest(plist, "22.22.22.2", "00:22:22:22:22:22")
        print(plist, count)
        self.assertEqual(sum(count), self.max_itrs)
        plist = [self.dev_port7, self.dev_port8, self.dev_port9]
        count = self.packetTest(plist, "33.33.33.2", "00:33:33:33:33:33")
        print(plist, count)
        self.assertEqual(sum(count), self.max_itrs)
        plist = [self.dev_port14, self.dev_port15, self.dev_port16]
        count = self.packetTest(plist, "44.44.44.2", "00:44:44:44:44:44")
        print(plist, count)
        self.assertEqual(sum(count), self.max_itrs)
        plist = [self.dev_port17, self.dev_port18, self.dev_port19]
        count = self.packetTest(plist, "55.55.55.2", "00:55:55:55:55:55")
        print(plist, count)
        self.assertEqual(sum(count), self.max_itrs)

    def runPostTest(self):
        '''
        Runs verification tests for number of different LAG ports
        while removing and disabling the LAG members
        '''
        print('Post warm reboot tests')
        plist = [self.dev_port17, self.dev_port18, self.dev_port19]
        count = self.packetTest(plist, "55.55.55.2", "00:55:55:55:55:55")
        print(plist, count)
        for c in count:
            self.assertTrue(c >= (self.max_itrs * 0.2),
                            "LAG paths are not equally balanced")
        self.assertEqual(sum(count), self.max_itrs)

        print("Removing port 17 from LAG 4")
        sai_thrift_remove_lag_member(self.client, self.lag4_member17)
        plist = [self.dev_port18, self.dev_port19]
        count = self.packetTest(plist, "55.55.55.2", "00:55:55:55:55:55")
        print(plist, count)
        for _, c in enumerate(count):
            self.assertTrue(c >= (self.max_itrs * 0.2),
                            "LAG paths are not equally balanced")
        self.assertEqual(sum(count), self.max_itrs)

        print("Removing port 18 from LAG 4")
        sai_thrift_remove_lag_member(self.client, self.lag4_member18)
        plist = [self.dev_port19]
        count = self.packetTest(plist, "55.55.55.2", "00:55:55:55:55:55")
        print(plist, count)
        for _, c in enumerate(count):
            self.assertTrue(c >= (self.max_itrs * 0.2),
                            "LAG paths are not equally balanced")
        self.assertEqual(sum(count), self.max_itrs)

        print("Adding port 17 and port 18 to LAG 4")
        self.lag4_member17 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag4, port_id=self.port17)
        self.lag4_member18 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag4, port_id=self.port18)
        plist = [self.dev_port17, self.dev_port18, self.dev_port19]
        count = self.packetTest(plist, "55.55.55.2", "00:55:55:55:55:55")
        print(plist, count)
        for _, c in enumerate(count):
            self.assertTrue(c >= (self.max_itrs * 0.2),
                            "LAG paths are not equally balanced")
        self.assertEqual(sum(count), self.max_itrs)

        print("Adding port 24 and port 25 to LAG 1")
        print("Disabling egress on lag1 port 25")
        lag1_member24 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag1, port_id=self.port24)
        lag1_member25 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag1, port_id=self.port25,
            egress_disable=True)
        plist = [self.dev_port4, self.dev_port5, self.dev_port6,
                 self.dev_port24]
        count = self.packetTest(plist, "22.22.22.2", "00:22:22:22:22:22")
        print(plist, count)
        for _, c in enumerate(count):
            self.assertTrue(c >= (self.max_itrs * 0.15),
                            "LAG paths are not equally balanced")
        self.assertEqual(sum(count), self.max_itrs)

        print("Removing port 24 and port 25 from LAG 1")
        sai_thrift_remove_lag_member(self.client, lag1_member24)
        sai_thrift_remove_lag_member(self.client, lag1_member25)
        plist = [self.dev_port4, self.dev_port5, self.dev_port6]
        count = self.packetTest(plist, "22.22.22.2", "00:22:22:22:22:22")
        print(plist, count)
        for _, c in enumerate(count):
            self.assertTrue(c >= (self.max_itrs * 0.2),
                            "LAG paths are not equally balanced")
        self.assertEqual(sum(count), self.max_itrs)

        print("Disabling egress on lag2 port7")
        sai_thrift_set_lag_member_attribute(self.client, self.lag2_member7,
                                            egress_disable=True)
        plist = [self.dev_port8, self.dev_port9]
        count = self.packetTest(plist, "33.33.33.2", "00:33:33:33:33:33")
        print(plist, count)
        for _, c in enumerate(count):
            self.assertTrue(c >= (self.max_itrs * 0.2),
                            "LAG paths are not equally balanced")
        self.assertEqual(sum(count), self.max_itrs)

        print("Enabling egress on lag2 port7")
        sai_thrift_set_lag_member_attribute(self.client, self.lag2_member7,
                                            egress_disable=False)
        plist = [self.dev_port7, self.dev_port8, self.dev_port9]
        count = self.packetTest(plist, "33.33.33.2", "00:33:33:33:33:33")
        print(plist, count)
        for _, c in enumerate(count):
            self.assertTrue(c >= (self.max_itrs * 0.2),
                            "LAG paths are not equally balanced")
        self.assertEqual(sum(count), self.max_itrs)

        print("Enabling egress on lag3 port14 and port15")
        sai_thrift_set_lag_member_attribute(self.client, self.lag3_member14,
                                            egress_disable=False)
        sai_thrift_set_lag_member_attribute(self.client, self.lag3_member15,
                                            egress_disable=False)
        plist = [self.dev_port14, self.dev_port15, self.dev_port16]
        count = self.packetTest(plist, "44.44.44.2", "00:44:44:44:44:44")
        print(plist, count)
        for _, c in enumerate(count):
            self.assertTrue(c >= (self.max_itrs * 0.2),
                            "LAG paths are not equally balanced")
        self.assertEqual(sum(count), self.max_itrs)

        print("Adding port 24 and port 25 to LAG 3")
        lag1_member24 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag3, port_id=self.port24)
        lag1_member25 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag3, port_id=self.port25)
        plist = [self.dev_port14, self.dev_port15, self.dev_port16,
                 self.dev_port24, self.dev_port25]
        count = self.packetTest(plist, "44.44.44.2", "00:44:44:44:44:44")
        print(plist, count)
        for _, c in enumerate(count):
            self.assertTrue(c >= (self.max_itrs * 0.1),
                            "LAG paths are not equally balanced")
        self.assertEqual(sum(count), self.max_itrs)

        print("Removing port 24 and port 25 from LAG 3")
        sai_thrift_remove_lag_member(self.client, lag1_member24)
        sai_thrift_remove_lag_member(self.client, lag1_member25)
        plist = [self.dev_port14, self.dev_port15, self.dev_port16]
        count = self.packetTest(plist, "44.44.44.2", "00:44:44:44:44:44")
        print(plist, count)
        for _, c in enumerate(count):
            self.assertTrue(c >= (self.max_itrs * 0.2),
                            "LAG paths are not equally balanced")
        self.assertEqual(sum(count), self.max_itrs)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry4)
        sai_thrift_remove_route_entry(self.client, self.route_entry3)
        sai_thrift_remove_route_entry(self.client, self.route_entry2)
        sai_thrift_remove_route_entry(self.client, self.route_entry1)
        sai_thrift_remove_route_entry(self.client, self.route_entry10)

        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry4)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry3)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry10)

        sai_thrift_remove_next_hop(self.client, self.nhop4)
        sai_thrift_remove_next_hop(self.client, self.nhop3)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_next_hop(self.client, self.nhop10)

        sai_thrift_remove_router_interface(self.client, self.lag2_rif)
        sai_thrift_remove_router_interface(self.client, self.lag1_rif)

        super(LagL3Nhop, self).tearDown()
