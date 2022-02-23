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
"""
Thrift SAI interface L2 FDB tests
"""

from sai_thrift.sai_headers import *

from sai_base_test import *


@group("draft")
class FdbAttributeTest(SaiHelper):
    '''
    Basic FDB attributes getting and setting test
    '''

    def setUp(self):
        super(FdbAttributeTest, self).setUp()

        self.fdb_entry = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address="00:11:22:33:44:55",
            bv_id=self.vlan10)
        sai_thrift_create_fdb_entry(self.client,
                                    self.fdb_entry,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=self.port0_bp)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

    def runTest(self):
        # get bridge_port_id
        attr = sai_thrift_get_fdb_entry_attribute(self.client,
                                                    self.fdb_entry,
                                                    bridge_port_id=True)
        self.assertEqual(attr['bridge_port_id'], self.port0_bp)

        # get type
        attr = sai_thrift_get_fdb_entry_attribute(
            self.client, self.fdb_entry, type=True)
        self.assertEqual(attr['type'], SAI_FDB_ENTRY_TYPE_STATIC)

        # set packet_action
        sai_thrift_set_fdb_entry_attribute(
            self.client,
            self.fdb_entry,
            packet_action=SAI_PACKET_ACTION_FORWARD)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        # get packet_action
        attr = sai_thrift_get_fdb_entry_attribute(
            self.client, self.fdb_entry, packet_action=True)
        self.assertEqual(attr['packet_action'], SAI_PACKET_ACTION_FORWARD)

    def tearDown(self):
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry)

        super(FdbAttributeTest, self).tearDown()


@group("draft")
class FdbNoLearnTest(SaiHelper):
    '''
    Verify different cases when MAC addresses should not be learned
    '''

    def setUp(self):
        super(FdbNoLearnTest, self).setUp()

        vlan_id = 10
        self.src_mac = "00:11:11:11:11:11"
        self.dst_mac = "00:22:22:22:22:22"

        self.pkt = simple_udp_packet(eth_dst=self.dst_mac,
                                     eth_src=self.src_mac,
                                     pktlen=100)
        self.tag_pkt = simple_udp_packet(eth_dst=self.dst_mac,
                                         eth_src=self.src_mac,
                                         dl_vlan_enable=True,
                                         vlan_vid=vlan_id,
                                         pktlen=104)

        self.chck_pkt = simple_udp_packet(eth_dst=self.src_mac,
                                          eth_src=self.dst_mac,
                                          pktlen=100)
        self.tag_chck_pkt = simple_udp_packet(eth_dst=self.src_mac,
                                              eth_src=self.dst_mac,
                                              dl_vlan_enable=True,
                                              vlan_vid=vlan_id,
                                              pktlen=104)

    def runTest(self):
        self.vlanPortNoLearnTest()
        self.vlanLagNoLearnTest()
        self.bpPortNoLearnTest()
        self.bpLagNoLearnTest()
        self.noBpNoLearnTest()
        self.removedBpNoLearnTest()

    def vlanPortNoLearnTest(self):
        '''
        Verify if MAC addresses are not learned on port when VLAN learning
        is disabled
        Send a packet on port0 with SMAC 00:11:11:11:11:11 and we verify it
        was broadcasted on other ports in VLAN.
        Next send a packet on port1 with DMAC 00:11:11:11:11:11 and verify
        it was also broadcasted on other ports in VLAN
        '''
        print("\nvlanPortNoLearnTest()")

        try:
            # disable VLAN10 learning
            status = sai_thrift_set_vlan_attribute(
                self.client, self.vlan10, learn_disable=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("MAC Learning disabled on VLAN")

            print("Sending packet on port %d, %s -> %s - will flood" %
                  (self.dev_port0, self.src_mac, self.dst_mac))
            send_packet(self, self.dev_port0, self.pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [self.tag_pkt, self.pkt],
                [[self.dev_port1],
                 [self.dev_port4, self.dev_port5, self.dev_port6]])

            print("Sending packet on port %d with DMAC %s - MAC not learned, "
                  "will flood" % (self.dev_port1, self.src_mac))
            send_packet(self, self.dev_port1, self.tag_chck_pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [self.chck_pkt, self.chck_pkt],
                [[self.dev_port0],
                 [self.dev_port4, self.dev_port5, self.dev_port6]])

            print("Verification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            # restore initial VLAN Learning state
            sai_thrift_set_vlan_attribute(
                self.client, self.vlan10, learn_disable=False)

    def vlanLagNoLearnTest(self):
        '''
        Verify if MAC addresses are not learned on LAG when VLAN learning
        is disabled
        Send a packet on LAG1 with SMAC 00:11:11:11:11:11, and verify it was
        broadcasted on other ports in VLAN.
        Next send a packet on port1 with DMAC 00:11:11:11:11:11 and verify
        it was also broadcasted on other ports in VLAN
        '''
        print("\nvlanLagNoLearnTest()")

        try:
            # disable VLAN10 learning
            status = sai_thrift_set_vlan_attribute(
                self.client, self.vlan10, learn_disable=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("MAC Learning disabled on VLAN")

            print("Sending packet on LAG port %d, %s -> %s - will flood" %
                  (self.dev_port5, self.src_mac, self.dst_mac))
            send_packet(self, self.dev_port5, self.pkt)
            verify_each_packet_on_each_port(self, [self.pkt, self.tag_pkt],
                                            [self.dev_port0, self.dev_port1])

            print("Sending packet on port %d with DMAC %s - MAC not learned, "
                  "will flood" % (self.dev_port1, self.src_mac))
            send_packet(self, self.dev_port1, self.tag_chck_pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [self.chck_pkt, self.chck_pkt],
                [[self.dev_port0],
                 [self.dev_port4, self.dev_port5, self.dev_port6]])

            print("Verification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            # restore initial VLAN Learning state
            sai_thrift_set_vlan_attribute(
                self.client, self.vlan10, learn_disable=False)

    def bpPortNoLearnTest(self):
        '''
        Verify if MAC addresses are not learned on port when bridge port
        learning is disabled.
        Disable learning on port0, send a packet on port0 with SMAC
        00:11:11:11:11:11 and verify it was broadcasted on other ports in VLAN.
        Next send a packet on port1 with DMAC 00:11:11:11:11:11 and verify
        it was also broadcasted on other ports in VLAN
        '''
        print("\nbpPortNoLearnTest()")

        try:
            # disable learning on port0
            status = sai_thrift_set_bridge_port_attribute(
                self.client,
                self.port0_bp,
                fdb_learning_mode=SAI_BRIDGE_PORT_FDB_LEARNING_MODE_DISABLE)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("MAC Learning disabled on port %d" % self.dev_port0)

            print("Sending packet on port %d, %s -> %s - will flood" %
                  (self.dev_port0, self.src_mac, self.dst_mac))
            send_packet(self, self.dev_port0, self.pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [self.tag_pkt, self.pkt],
                [[self.dev_port1],
                 [self.dev_port4, self.dev_port5, self.dev_port6]])

            print("Sending packet on port %d with DMAC %s - MAC not learned, "
                  "will flood" % (self.dev_port1, self.src_mac))
            send_packet(self, self.dev_port1, self.tag_chck_pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [self.chck_pkt, self.chck_pkt],
                [[self.dev_port0],
                 [self.dev_port4, self.dev_port5, self.dev_port6]])

            print("Verification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            # restore initial VLAN Learning state
            sai_thrift_set_bridge_port_attribute(
                self.client, self.port0_bp,
                fdb_learning_mode=SAI_BRIDGE_PORT_FDB_LEARNING_MODE_HW)

    def bpLagNoLearnTest(self):
        '''
        Verify if MAC addresses are not learned on port when LAG bridge
        port learning is disabled.
        Disable learning on LAG1, send a packet on port0 with
        SMAC: 00:11:11:11:11:11, and then we verify it was broadcasted on other
        ports in VLAN.
        Next send a packet on port1 with DMAC 00:11:11:11:11:11 and verify
        it was also broadcasted on other ports in VLAN
        '''
        print("\nbpLagNoLearnTest()")

        try:
            # disable learning on LAG1
            status = sai_thrift_set_bridge_port_attribute(
                self.client,
                self.lag1_bp,
                fdb_learning_mode=SAI_BRIDGE_PORT_FDB_LEARNING_MODE_DISABLE)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("MAC Learning disabled on LAG1")

            print("Sending packet on LAG port %d, %s -> %s - will flood" %
                  (self.dev_port5, self.src_mac, self.dst_mac))
            send_packet(self, self.dev_port5, self.pkt)
            verify_each_packet_on_each_port(self, [self.pkt, self.tag_pkt],
                                            [self.dev_port0, self.dev_port1])

            print("Sending packet on port %d with DMAC %s - MAC not learned, "
                  "will flood" % (self.dev_port1, self.src_mac))
            send_packet(self, self.dev_port1, self.tag_chck_pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [self.chck_pkt, self.chck_pkt],
                [[self.dev_port0],
                 [self.dev_port4, self.dev_port5, self.dev_port6]])

            print("Verification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            # restore initial VLAN Learning state
            sai_thrift_set_bridge_port_attribute(
                self.client, self.lag1_bp,
                fdb_learning_mode=SAI_BRIDGE_PORT_FDB_LEARNING_MODE_HW)

    def noBpNoLearnTest(self):
        '''
        Verify if MAC is not learned on port if bridge port is not created
        on that port
        '''
        print("\nnoBpNoLearnTest()")

        test_port = self.port24
        test_dev_port = self.dev_port24
        vlan10_ports = [[self.dev_port0], [self.dev_port1],
                        [self.dev_port4, self.dev_port5, self.dev_port6]]
        chck_port = vlan10_ports[0][0]
        flood_pkt_list = [self.pkt, self.tag_pkt, self.pkt]

        flood_port_list = [[self.dev_port1],
                           [self.dev_port4, self.dev_port5, self.dev_port6]]
        flood_chk_pkt_list = [self.tag_chck_pkt, self.chck_pkt]
        vlan_id = 10

        try:
            # set VLAN on port without bridge port creation
            sai_thrift_set_port_attribute(
                self.client, test_port, port_vlan_id=vlan_id)

            print("Sending packet on port %d - with no bridge port created" %
                  test_dev_port)
            send_packet(self, test_dev_port, self.pkt)
            verify_each_packet_on_multiple_port_lists(
                self, flood_pkt_list, vlan10_ports)
            print("\tOK")

            print("Sending packet to check MAC address was not learned on "
                  "port %d" % test_dev_port)
            send_packet(self, chck_port, self.chck_pkt)
            verify_each_packet_on_multiple_port_lists(
                self, flood_chk_pkt_list, flood_port_list)
            print("\tOK")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            sai_thrift_set_port_attribute(
                self.client, test_port, port_vlan_id=0)

    def removedBpNoLearnTest(self):
        '''
        Verify if MAC address is not learned on port after bridge port
        is removed on that port
        '''
        print("\nremovedBpNoLearnTest()")

        try:
            sai_thrift_remove_bridge_port(self.client, self.port0_bp)

            print("Sending packet on port %d - with removed bridge port" %
                  self.dev_port0)
            print("Packet will be flooded on vlan 10")
            send_packet(self, self.dev_port0, self.pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [self.tag_pkt, self.pkt],
                [[self.dev_port1],
                 [self.dev_port4, self.dev_port5, self.dev_port6]])
            print("\tOK")

            print("Sending packet to check MAC address was not learned on "
                  "port %d" % self.dev_port0)
            send_packet(self, self.dev_port1, self.tag_chck_pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [self.chck_pkt, self.chck_pkt],
                [[self.dev_port0],
                 [self.dev_port4, self.dev_port5, self.dev_port6]])
            print("\tOK")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            self.port0_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=self.port0,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            self.assertNotEqual(self.port0_bp, 0)


@group("draft")
class FdbLearnTest(SaiHelper):
    '''
    Verify different cases of FDB learning
    '''

    def setUp(self):
        super(FdbLearnTest, self).setUp()

        self.vlan_id = 10

        # add (tagged) LAG2 to VLAN 10
        self.vlan10_member3 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.lag2_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        self.src_ports = [self.dev_port0, self.dev_port1, self.dev_port4,
                          self.dev_port5, self.dev_port6, self.dev_port7,
                          self.dev_port8, self.dev_port9]
        self.macs = []
        for i in range(1, len(self.src_ports)):
            self.macs.append("00:%02d:%02d:%02d:%02d:%02d" % (i, i, i, i, i))

        self.utg_lag_ports = [self.dev_port4, self.dev_port5, self.dev_port6]
        self.tg_lag_ports = [self.dev_port7, self.dev_port8, self.dev_port9]
        self.dst_ports = [[self.dev_port0], [self.dev_port1],
                          self.utg_lag_ports, self.tg_lag_ports]

    def runTest(self):
        self.dynamicMacLearnTest()
        self.macLearnErrorTest()

    def tearDown(self):
        # remove LAG2 from VLAN 10
        sai_thrift_remove_vlan_member(self.client, self.vlan10_member3)
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        super(FdbLearnTest, self).tearDown()

    def dynamicMacLearnTest(self):  # noqa pylint: disable=too-many-branches
        '''
        Verify if MAC addresses are learned on tagged and untagged VLAN
        ports and LAGs. The test checks also if MAC addresses are learned on
        newly added VLAN and LAG member.
        Send packets with given SMACs and verify they are broadcasted
        on other ports.
        Next send packets with DMACs == previous SMACs and verify they are
        forwarded only to the proper ports
        '''
        print("\ndynamicMacLearnTest()")

        try:
            # learning phase
            dst_mac = "00:11:22:33:44:55"
            for src_port, src_mac in zip(self.src_ports, self.macs):
                pkt = simple_udp_packet(eth_dst=dst_mac,
                                        eth_src=src_mac,
                                        pktlen=100)
                tag_pkt = simple_udp_packet(eth_dst=dst_mac,
                                            eth_src=src_mac,
                                            dl_vlan_enable=True,
                                            vlan_vid=self.vlan_id,
                                            pktlen=104)

                if src_port == self.dev_port1 or src_port in self.tg_lag_ports:
                    send_pkt = tag_pkt
                else:
                    send_pkt = pkt

                # create port list for flooding verification
                flood_port_list = [
                    self.dst_ports[p] for p in range(len(self.dst_ports))
                    if src_port not in self.dst_ports[p]]
                # create packet list for flooding verification
                flood_pkt_list = []
                tg_lag_set = False
                utg_lag_set = False
                for dst_port in self.dst_ports:
                    if src_port in dst_port:
                        continue
                    if dst_port == [self.dev_port0]:
                        flood_pkt_list.append(pkt)
                    elif dst_port == [self.dev_port1]:
                        flood_pkt_list.append(tag_pkt)
                    elif dst_port == self.utg_lag_ports and not utg_lag_set:
                        flood_pkt_list.append(pkt)
                        utg_lag_set = True
                    elif dst_port == self.tg_lag_ports and not tg_lag_set:
                        flood_pkt_list.append(tag_pkt)
                        tg_lag_set = True

                print("Sending packet on port %d, %s -> %s - will flood" %
                      (src_port, src_mac, dst_mac))
                send_packet(self, src_port, send_pkt)
                verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                          flood_port_list)

            print("\tLearning complete\n")

            # verification phase:
            for dst_port, dst_mac in zip(self.src_ports, self.macs):
                for src_port, src_mac in zip([self.dev_port0, self.dev_port1],
                                             [self.macs[0], self.macs[1]]):
                    if src_port == dst_port:
                        continue

                    pkt = simple_udp_packet(eth_dst=dst_mac,
                                            eth_src=src_mac,
                                            pktlen=100)
                    tag_pkt = simple_udp_packet(eth_dst=dst_mac,
                                                eth_src=src_mac,
                                                dl_vlan_enable=True,
                                                vlan_vid=self.vlan_id,
                                                pktlen=104)

                    if src_port == self.dev_port1 or src_port \
                            in self.tg_lag_ports:
                        send_pkt = tag_pkt
                    else:
                        send_pkt = pkt

                    if dst_port == self.dev_port1 or dst_port \
                            in self.tg_lag_ports:
                        rcv_pkt = tag_pkt
                    else:
                        rcv_pkt = pkt

                    if dst_port in self.utg_lag_ports:
                        rcv_port = self.utg_lag_ports
                    elif dst_port in self.tg_lag_ports:
                        rcv_port = self.tg_lag_ports
                    else:
                        rcv_port = [dst_port]

                    print("Sending packet on port %d, %s -> %s - forwarding" %
                          (src_port, src_mac, dst_mac))
                    send_packet(self, src_port, send_pkt)
                    verify_packet_any_port(self, rcv_pkt, rcv_port)

            print("\tVerification complete")

            # verify learning on new VLAN member
            new_vlan_member = self.port24
            new_vlan_member_dev = self.dev_port24
            new_vlan_member_mac = "00:12:34:56:78:90"
            new_vlan_member_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=new_vlan_member,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)

            vlan10_member4 = sai_thrift_create_vlan_member(
                self.client,
                vlan_id=self.vlan10,
                bridge_port_id=new_vlan_member_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

            sai_thrift_set_port_attribute(self.client,
                                          new_vlan_member,
                                          port_vlan_id=self.vlan_id)

            print("\nVerify learning on new VLAN member (port %d)" %
                  new_vlan_member_dev)
            # check if packet is flooded to the new VLAN port
            pkt = simple_udp_packet(eth_dst=new_vlan_member_mac,
                                    eth_src=self.macs[0],
                                    pktlen=100)
            tag_pkt = simple_udp_packet(eth_dst=new_vlan_member_mac,
                                        eth_src=self.macs[0],
                                        dl_vlan_enable=True,
                                        vlan_vid=self.vlan_id,
                                        pktlen=104)

            flood_port_list = [[self.dev_port1], self.utg_lag_ports,
                               self.tg_lag_ports, [new_vlan_member_dev]]
            flood_pkt_list = [tag_pkt, pkt, tag_pkt, pkt]

            print("Sending packet on port %d, %s -> %s\nchecking if flood "
                  "reaches port %d" % (self.dev_port0, self.macs[0],
                                       new_vlan_member_mac,
                                       new_vlan_member_dev))
            send_packet(self, self.dev_port0, pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tOK")

            # check if MAC is learned on new VLAN member
            dst_mac = "00:11:22:33:44:55"
            pkt = simple_udp_packet(eth_dst=dst_mac,
                                    eth_src=new_vlan_member_mac,
                                    pktlen=100)
            tag_pkt = simple_udp_packet(eth_dst=dst_mac,
                                        eth_src=new_vlan_member_mac,
                                        dl_vlan_enable=True,
                                        vlan_vid=self.vlan_id,
                                        pktlen=104)

            flood_port_list = self.dst_ports
            flood_pkt_list = [pkt, tag_pkt, pkt, tag_pkt]

            print("Sending packet on new VLAN port (%d), %s -> %s - will "
                  "flood" % (new_vlan_member_dev, new_vlan_member_mac,
                             dst_mac))
            send_packet(self, new_vlan_member_dev, pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tOK")
            time.sleep(2)

            print("Checking if MAC was learned")
            pkt = simple_udp_packet(eth_dst=new_vlan_member_mac,
                                    eth_src=self.macs[0],
                                    pktlen=100)

            print("Sending packet on port %d, %s -> %s - forward to port %d" %
                  (self.dev_port0, self.macs[0], new_vlan_member_mac,
                   new_vlan_member_dev))
            send_packet(self, self.dev_port0, pkt)
            verify_packets(self, pkt, [new_vlan_member_dev])

            print("\tVerification complete")

            # verify learning on new LAG member
            new_lag_member = self.port25
            new_lag_member_dev = self.dev_port25
            new_lag_member_mac = "00:09:87:65:43:21"

            lag1_member25 = sai_thrift_create_lag_member(
                self.client, lag_id=self.lag1, port_id=new_lag_member)

            self.utg_lag_ports.append(new_lag_member_dev)

            print("\nVerify learning on new LAG member (port %d in LAG1)" %
                  new_lag_member_dev)
            pkt = simple_udp_packet(eth_dst=dst_mac,
                                    eth_src=new_lag_member_mac,
                                    pktlen=100)
            tag_pkt = simple_udp_packet(eth_dst=dst_mac,
                                        eth_src=new_lag_member_mac,
                                        dl_vlan_enable=True,
                                        vlan_vid=self.vlan_id,
                                        pktlen=104)

            flood_port_list = [[self.dev_port0], [self.dev_port1],
                               self.tg_lag_ports, [new_vlan_member_dev]]
            flood_pkt_list = [pkt, tag_pkt, tag_pkt, pkt]

            print("Sending packet on new LAG member (port %d), %s -> %s - "
                  "will flood" % (new_lag_member_dev, new_lag_member_mac,
                                  dst_mac))
            send_packet(self, new_lag_member_dev, pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tOK")
            time.sleep(2)

            print("Checking if MAC was learned")
            pkt = simple_udp_packet(eth_dst=new_lag_member_mac,
                                    eth_src=self.macs[0],
                                    pktlen=100)

            print("Sending packet on port %d, %s -> %s forward to LAG1" %
                  (self.dev_port0, self.macs[0], new_lag_member_mac))
            send_packet(self, self.dev_port0, pkt)
            verify_packet_any_port(self, pkt, self.utg_lag_ports)

            print("\tVerification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            # remove new LAG member
            sai_thrift_remove_lag_member(self.client, lag1_member25)
            self.utg_lag_ports.remove(new_lag_member_dev)

            # remove new vlan member
            sai_thrift_set_port_attribute(self.client, new_vlan_member,
                                          port_vlan_id=0)
            sai_thrift_remove_vlan_member(self.client, vlan10_member4)
            sai_thrift_remove_bridge_port(self.client, new_vlan_member_bp)

    def macLearnErrorTest(self):
        '''
        Verify if MAC addresses are not learned when different undesirable
        conditions occured:
        1 - invalid VLAN tag
        2 - src_mac is a broadcast address (packet drop)
        3 - src_mac is a multicast address (packet drop)
        4 - src_mac previously added statically
        5 - removed VLAN member
        6 - removed LAG member
        '''
        print("\nmacLearnErrorTest()")

        access_port = self.dev_port0  # untagged
        ap_mac = self.macs[0]
        trunk_port = self.dev_port1  # tagged
        tp_mac = self.macs[1]

        flood_port_list = [[trunk_port], self.utg_lag_ports, self.tg_lag_ports]

        try:
            fdb_entry1 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                mac_address=ap_mac,
                                                bv_id=self.vlan10)
            status = sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry1,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=self.port0_bp)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            fdb_entry2 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                mac_address=tp_mac,
                                                bv_id=self.vlan10)
            status = sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry2,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=self.port1_bp)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            print("Case 1 - invalid VLAN tag")
            inv_vlan_id = 100
            lrn_mac = self.macs[2]

            inv_vlan_tag_pkt = simple_udp_packet(eth_dst=ap_mac,
                                                 eth_src=lrn_mac,
                                                 dl_vlan_enable=True,
                                                 vlan_vid=inv_vlan_id,
                                                 pktlen=104)
            chck_inv_vlan_pkt = simple_udp_packet(eth_dst=lrn_mac,
                                                  eth_src=ap_mac,
                                                  pktlen=100)
            chck_inv_vlan_tag_pkt = simple_udp_packet(eth_dst=lrn_mac,
                                                      eth_src=ap_mac,
                                                      dl_vlan_enable=True,
                                                      vlan_vid=self.vlan_id,
                                                      pktlen=104)

            flood_pkt_list = [chck_inv_vlan_tag_pkt, chck_inv_vlan_pkt,
                              chck_inv_vlan_tag_pkt]

            print("Sending packet with invalid VLAN tag on port %d, %s -> %s" %
                  (trunk_port, lrn_mac, ap_mac))
            send_packet(self, trunk_port, inv_vlan_tag_pkt)
            verify_no_other_packets(self)
            print("\tPacket dropped")

            print("Checking if MAC %s was not learned on port %d" %
                  (lrn_mac, trunk_port))
            print("Sending packet on port %d, %s -> %s - will flood" %
                  (access_port, ap_mac, lrn_mac))
            send_packet(self, access_port, chck_inv_vlan_pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tVerification complete\n")

            print("Case 2 - broadcast src_mac address")
            bcast_mac = "ff:ff:ff:ff:ff:ff"

            bcast_src_tag_pkt = simple_udp_packet(eth_dst=ap_mac,
                                                  eth_src=bcast_mac,
                                                  dl_vlan_enable=True,
                                                  vlan_vid=self.vlan_id,
                                                  pktlen=104)
            chck_bcast_src_pkt = simple_udp_packet(eth_dst=bcast_mac,
                                                   eth_src=ap_mac,
                                                   pktlen=100)
            chck_bcast_src_tag_pkt = simple_udp_packet(eth_dst=bcast_mac,
                                                       eth_src=ap_mac,
                                                       dl_vlan_enable=True,
                                                       vlan_vid=self.vlan_id,
                                                       pktlen=104)

            flood_pkt_list = [chck_bcast_src_tag_pkt, chck_bcast_src_pkt,
                              chck_bcast_src_tag_pkt]

            print("Sending packet with invalid src_mac on port %d, %s -> %s" %
                  (trunk_port, bcast_mac, ap_mac))
            send_packet(self, trunk_port, bcast_src_tag_pkt)
            verify_no_other_packets(self)
            print("\tPacket dropped")

            print("Checking if broadcast MAC %s was not learned on port %d" %
                  (bcast_mac, trunk_port))
            print("Sending packet on port %d, %s -> %s - will flood" %
                  (access_port, ap_mac, bcast_mac))
            send_packet(self, access_port, chck_bcast_src_pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tVerification complete\n")

            print("Case 3 - multicast src_mac address")
            mcast_mac = "01:00:5e:11:22:33"

            mcast_src_tag_pkt = simple_udp_packet(eth_dst=ap_mac,
                                                  eth_src=mcast_mac,
                                                  dl_vlan_enable=True,
                                                  vlan_vid=self.vlan_id,
                                                  pktlen=104)
            chck_mcast_src_pkt = simple_udp_packet(eth_dst=mcast_mac,
                                                   eth_src=ap_mac,
                                                   pktlen=100)
            chck_mcast_src_tag_pkt = simple_udp_packet(eth_dst=mcast_mac,
                                                       eth_src=ap_mac,
                                                       dl_vlan_enable=True,
                                                       vlan_vid=self.vlan_id,
                                                       pktlen=104)

            flood_pkt_list = [chck_mcast_src_tag_pkt, chck_mcast_src_pkt,
                              chck_mcast_src_tag_pkt]

            print("Sending packet with invalid src_mac on port %d, %s -> %s" %
                  (trunk_port, mcast_mac, ap_mac))
            send_packet(self, trunk_port, mcast_src_tag_pkt)
            verify_no_other_packets(self)
            print("\tPacket dropped")

            print("Checking if multicast MAC %s was not learned on port %d" %
                  (mcast_mac, trunk_port))
            print("Sending packet on port %d, %s -> %s - will flood" %
                  (access_port, ap_mac, mcast_mac))
            send_packet(self, access_port, chck_mcast_src_pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tVerification complete\n")

            print("Case 4 - src_mac statically added")
            lrn_mac = self.macs[2]
            bcast_mac = "ff:ff:ff:ff:ff:ff"
            static_src_tag_pkt = simple_udp_packet(eth_dst=bcast_mac,
                                                   eth_src=ap_mac,
                                                   dl_vlan_enable=True,
                                                   vlan_vid=self.vlan_id,
                                                   pktlen=104)
            static_src_pkt = simple_udp_packet(eth_dst=bcast_mac,
                                               eth_src=ap_mac,
                                               pktlen=100)
            chck_static_src_pkt = simple_udp_packet(eth_dst=ap_mac,
                                                    eth_src=lrn_mac,
                                                    pktlen=100)

            flood_pkt_list = [
                static_src_pkt, static_src_pkt, static_src_tag_pkt]
            flood_port_list = [
                [access_port], self.utg_lag_ports, self.tg_lag_ports]

            print("Sending packet on port %d, %s -> %s" %
                  (trunk_port, ap_mac, bcast_mac))
            send_packet(self, trunk_port, static_src_tag_pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tPacket flooded")

            print("Checking if MAC %s was not learned on port %d" %
                  (ap_mac, trunk_port))
            print("Sending packet on port %d, %s -> %s - forward to port %d" %
                  (self.utg_lag_ports[1], lrn_mac, ap_mac, access_port))
            send_packet(self, self.utg_lag_ports[1], chck_static_src_pkt)
            verify_packets(self, chck_static_src_pkt, [access_port])
            print("\tVerification complete\n")

            print("Case 5 - removed VLAN member")
            rm_vlan_member = self.vlan10_member1
            rm_vlan_member_dev = self.dev_port1
            rm_vlan_member_mac = "00:12:34:56:78:90"

            sai_thrift_remove_vlan_member(self.client, rm_vlan_member)

            # check if packet is not flooded to the removed VLAN member port
            pkt = simple_udp_packet(eth_dst="ff:ff:ff:ff:ff:ff",
                                    eth_src=self.macs[0],
                                    pktlen=100)
            tag_pkt = simple_udp_packet(eth_dst="ff:ff:ff:ff:ff:ff",
                                        eth_src=self.macs[0],
                                        dl_vlan_enable=True,
                                        vlan_vid=self.vlan_id,
                                        pktlen=104)

            flood_port_list = [self.utg_lag_ports, self.tg_lag_ports]
            flood_pkt_list = [pkt, tag_pkt]

            print("Sending packet on port %d, %s -> %s\nchecking if flood "
                  "doesn't reach port %d" % (self.dev_port0, self.macs[0],
                                             "ff:ff:ff:ff:ff:ff",
                                             rm_vlan_member_dev))
            send_packet(self, self.dev_port0, pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tOK")

            # check if MAC is not learned on removed VLAN member
            tag_pkt = simple_udp_packet(eth_dst="ff:ff:ff:ff:ff:ff",
                                        eth_src=rm_vlan_member_mac,
                                        dl_vlan_enable=True,
                                        vlan_vid=self.vlan_id,
                                        pktlen=104)

            print("Sending packet on removed VLAN port (%d), %s -> %s - "
                  "will be dropped" % (rm_vlan_member_dev, rm_vlan_member_mac,
                                       "ff:ff:ff:ff:ff:ff"))
            send_packet(self, rm_vlan_member_dev, tag_pkt)
            verify_no_other_packets(self)
            print("\tOK")

            print("Checking if MAC was not learned")
            pkt = simple_udp_packet(eth_dst=rm_vlan_member_mac,
                                    eth_src=self.macs[0],
                                    pktlen=100)
            tag_pkt = simple_udp_packet(eth_dst=rm_vlan_member_mac,
                                        eth_src=self.macs[0],
                                        dl_vlan_enable=True,
                                        vlan_vid=self.vlan_id,
                                        pktlen=104)

            flood_pkt_list = [pkt, tag_pkt]

            print("Sending packet on port %d, %s -> %s - will flood within "
                  "VLAN 10" % (self.dev_port0, self.macs[0],
                               rm_vlan_member_mac))
            send_packet(self, self.dev_port0, pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)

            print("Case 6 - removed LAG member")
            rm_lag_member = self.lag1_member5
            rm_lag_member_dev = self.dev_port5
            rm_lag_member_mac = "00:09:87:65:43:21"

            sai_thrift_remove_lag_member(self.client, rm_lag_member)
            self.utg_lag_ports.remove(rm_lag_member_dev)

            pkt = simple_udp_packet(eth_dst="ff:ff:ff:ff:ff:ff",
                                    eth_src=rm_lag_member_mac,
                                    pktlen=100)

            print("Sending packet on removed LAG member (port %d), %s -> %s - "
                  "will be dropped" % (rm_lag_member_dev, rm_lag_member_mac,
                                       "ff:ff:ff:ff:ff:ff"))
            send_packet(self, rm_lag_member_dev, pkt)
            verify_no_other_packets(self)
            print("\tOK")

            print("Checking if MAC was not learned")
            pkt = simple_udp_packet(eth_dst=rm_lag_member_mac,
                                    eth_src=self.macs[0],
                                    pktlen=100)
            tag_pkt = simple_udp_packet(eth_dst=rm_lag_member_mac,
                                        eth_src=self.macs[0],
                                        dl_vlan_enable=True,
                                        vlan_vid=self.vlan_id,
                                        pktlen=104)

            flood_pkt_list = [pkt, tag_pkt]

            print("Sending packet on port %d, %s -> %s - will flood within "
                  "VLAN 10" % (self.dev_port0, self.macs[0],
                               rm_lag_member_mac))
            send_packet(self, self.dev_port0, pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tVerification complete\n")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            # restore removed LAG member
            self.lag1_member5 = sai_thrift_create_lag_member(
                self.client, lag_id=self.lag1, port_id=self.port5)
            self.utg_lag_ports.insert(1, rm_lag_member_dev)

            # restore removed vlan member
            self.vlan10_member1 = sai_thrift_create_vlan_member(
                self.client, vlan_id=self.vlan10, bridge_port_id=self.port1_bp,
                vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)


@group("draft")
class FdbStaticMacTest(SaiHelper):
    '''
    Verify static MAC entries
    '''

    def setUp(self):
        super(FdbStaticMacTest, self).setUp()

        self.vlan_id = 10

        lag = [self.dev_port4, self.dev_port5, self.dev_port6]
        self.ports = [[self.dev_port0], [self.dev_port1], lag]

        self.macs = []
        for i in range(1, len(self.ports) + 1):
            mac = "00:%02d:%02d:%02d:%02d:%02d" % (i, i, i, i, i)
            self.macs.append(mac)

        # populate FDB
        fdb_entry1 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                            mac_address=self.macs[0],
                                            bv_id=self.vlan10)
        status = sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry1,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port0_bp)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        fdb_entry2 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                            mac_address=self.macs[1],
                                            bv_id=self.vlan10)
        status = sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry2,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port1_bp)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.fdb_entry3 = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                    mac_address=self.macs[2],
                                                    bv_id=self.vlan10)
        status = sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry3,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.lag1_bp)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("FDB entries added")

    def runTest(self):
        self.staticMacForwardTest()
        self.selfForwardingTest()

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        super(FdbStaticMacTest, self).tearDown()

    def staticMacForwardTest(self):
        '''
        Static MAC tests
        Send a packet from one port with DMAC installed on other port and
        verify packet forwarding
        '''
        print("\nstaticMacForwardTest()")

        try:
            for dst_port, dst_mac in zip(self.ports, self.macs):
                for src_port, src_mac in zip([self.dev_port0, self.dev_port1],
                                             self.macs[:1]):
                    if [src_port] == dst_port:
                        continue

                    pkt = simple_udp_packet(eth_dst=dst_mac,
                                            eth_src=src_mac,
                                            pktlen=100)
                    tag_pkt = simple_udp_packet(eth_dst=dst_mac,
                                                eth_src=src_mac,
                                                dl_vlan_enable=True,
                                                vlan_vid=self.vlan_id,
                                                pktlen=104)

                    send_pkt = tag_pkt if src_port == self.dev_port1 else pkt
                    rcv_pkt = tag_pkt if dst_port == [self.dev_port1] else pkt

                    print("Sending packet on port %d, %s -> %s - forwarding" %
                          (src_port, src_mac, dst_mac))
                    send_packet(self, src_port, send_pkt)
                    verify_packet_any_port(self, rcv_pkt, dst_port)
            print("\tVerification complete")

        except BaseException:
            print("Test error occured")

    def selfForwardingTest(self):
        '''
        Verify if packet send with ingress port MAC address is dropped
        '''
        print("\nselfForwardingTest()")

        test_port_dev = self.dev_port0
        test_mac = self.macs[0]

        pkt = simple_udp_packet(eth_dst=test_mac)

        print("Verifying if packet send on ingress port MAC is dropped")
        send_packet(self, test_port_dev, pkt)
        verify_no_other_packets(self)
        print("\tOK")


@group("draft")
class FdbMacMoveTest(SaiHelper):
    '''
    Verify MAC entries moving
    '''

    def setUp(self):
        super(FdbMacMoveTest, self).setUp()

        # change port1 original tagging mode
        sai_thrift_remove_vlan_member(self.client, self.vlan10_member1)
        self.vlan10_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.port1_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_port_attribute(self.client, self.port1, port_vlan_id=10)

        # add one more access port to VLAN 10
        self.port24_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.assertNotEqual(self.port24_bp, 0)
        self.vlan10_member3 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_port_attribute(
            self.client, self.port24, port_vlan_id=10)

        # create additional LAG in VLAN 10
        self.lag10 = sai_thrift_create_lag(self.client)
        self.assertNotEqual(self.lag10, 0)
        self.lag10_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.lag10,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.assertNotEqual(self.lag10_bp, 0)
        self.lag10_member25 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag10, port_id=self.port25)
        self.lag10_member26 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag10, port_id=self.port26)
        self.lag10_member27 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag10, port_id=self.port27)

        self.vlan10_member4 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.lag10_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_lag_attribute(self.client, self.lag10, port_vlan_id=10)

        self.moving_mac = "00:11:22:33:44:55"
        self.chck_mac = "00:11:11:11:11:11"

        # add static MAC for port used for verification
        self.fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                mac_address=self.chck_mac,
                                                bv_id=self.vlan10)
        status = sai_thrift_create_fdb_entry(self.client,
                                             self.fdb_entry,
                                             type=SAI_FDB_ENTRY_TYPE_STATIC,
                                             bridge_port_id=self.port24_bp)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        self.dynamicMacMoveTest()
        self.dynamicMacMoveTest(static_entry=True)
        self.staticMacMoveTest()

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        # remove additionally created LAG
        sai_thrift_set_lag_attribute(self.client, self.lag1, port_vlan_id=0)
        sai_thrift_remove_vlan_member(self.client, self.vlan10_member4)
        sai_thrift_remove_lag_member(self.client, self.lag10_member27)
        sai_thrift_remove_lag_member(self.client, self.lag10_member26)
        sai_thrift_remove_lag_member(self.client, self.lag10_member25)
        sai_thrift_remove_bridge_port(self.client, self.lag10_bp)
        sai_thrift_remove_lag(self.client, self.lag10)

        # remove additional port from VLAN 10
        sai_thrift_set_port_attribute(self.client, self.port24, port_vlan_id=0)
        sai_thrift_remove_bridge_port(self.client, self.port24_bp)
        sai_thrift_remove_vlan_member(self.client, self.vlan10_member3)

        # restore port1 original tagging mode
        sai_thrift_set_port_attribute(self.client, self.port1, port_vlan_id=0)
        sai_thrift_remove_vlan_member(self.client, self.vlan10_member1)
        self.vlan10_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.port1_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        super(FdbMacMoveTest, self).tearDown()

    def dynamicMacMoveTest(self, static_entry=False):
        '''
        Dynamic MAC move test that verify if after receiving packet with known
        SMAC, but from other port that on which it was learned previously,
        next packets with such DMACs are forwarded to the new port.

        The test represents a chain of moves:
            - port -> port
            - port -> LAG
            - LAG  -> LAG
            - LAG  -> port

        Args:
             static_entry (bool): Check move for static fdb entry
        '''
        print("\ndynamicMacMoveTest()")

        # a series of ports representing moving chain:
        # port->port->LAG->LAG->port
        port_chain = [self.dev_port0, self.dev_port1, self.dev_port5,
                      self.dev_port27, self.dev_port1]
        chck_port = self.dev_port24
        lag1_ports = [self.dev_port4, self.dev_port5, self.dev_port6]
        lag3_ports = [self.dev_port25, self.dev_port26, self.dev_port27]

        pkt = simple_udp_packet(eth_dst=self.chck_mac, eth_src=self.moving_mac)
        chck_pkt = simple_udp_packet(eth_dst=self.moving_mac,
                                     eth_src=self.chck_mac)

        try:
            if static_entry:
                # inititally add moving MAC to FDB as static
                moving_fdb_entry = sai_thrift_fdb_entry_t(
                    switch_id=self.switch_id,
                    mac_address=self.moving_mac,
                    bv_id=self.vlan10)
                status = sai_thrift_create_fdb_entry(
                    self.client,
                    moving_fdb_entry,
                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                    bridge_port_id=self.port0_bp,
                    allow_mac_move=True)
                self.assertEqual(status, SAI_STATUS_SUCCESS)

            for src_port in port_chain:
                print("Sending packet on port %d, %s -> %s - learn and "
                      "forward to port %d" % (src_port, self.moving_mac,
                                              self.chck_mac, chck_port))
                send_packet(self, src_port, pkt)
                verify_packets(self, pkt, [chck_port])
                print("MAC learned")

                if src_port in lag1_ports:
                    rcv_port = lag1_ports
                    print("Sending packet on port %d, %s -> %s - forward to "
                          "LAG1" % (chck_port, self.chck_mac, self.moving_mac))
                elif src_port in lag3_ports:
                    rcv_port = lag3_ports
                    print("Sending packet on port %d, %s -> %s - forward to "
                          "LAG3" % (chck_port, self.chck_mac, self.moving_mac))
                else:
                    rcv_port = [src_port]
                    print("Sending packet on port %d, %s -> %s - forward to "
                          "port %d" % (chck_port, self.chck_mac,
                                       self.moving_mac, src_port))

                send_packet(self, chck_port, chck_pkt)
                verify_packet_any_port(self, chck_pkt, rcv_port)

            print("\tVerification complete\n")

        finally:
            if static_entry:
                sai_thrift_remove_fdb_entry(self.client, moving_fdb_entry)
            else:
                # flush dynamic MACs from FDB
                sai_thrift_flush_fdb_entries(
                    self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)

    def staticMacMoveTest(self):
        '''
        static MAC move tests.
        Changee a bridge port id of some static FDB entry and check that
        a packet is forwarded to this new port.
        '''
        print("\nstaticMacMoveTest()")

        # a series of ports representing moving chain:
        # port->port->LAG->LAG->port
        port_chain = [self.dev_port0, self.dev_port1, self.dev_port5,
                      self.dev_port27, self.dev_port1 ]
        bport_chain = [self.port0_bp, self.port1_bp, self.lag1_bp,
                       self.lag10_bp, self.port1_bp]
        chck_port = self.dev_port24
        lag1_ports = [self.dev_port4, self.dev_port5, self.dev_port6]
        lag3_ports = [self.dev_port25, self.dev_port26, self.dev_port27]

        chck_pkt = simple_udp_packet(eth_dst=self.moving_mac,
                                     eth_src=self.chck_mac)

        try:
            # inititally add moving MAC to FDB
            moving_fdb_entry = sai_thrift_fdb_entry_t(
                switch_id=self.switch_id,
                mac_address=self.moving_mac,
                bv_id=self.vlan10)
            status = sai_thrift_create_fdb_entry(
                self.client,
                moving_fdb_entry,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=self.port0_bp)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            for port, bport in zip(port_chain, bport_chain):
                status = sai_thrift_set_fdb_entry_attribute(
                    self.client, moving_fdb_entry, bridge_port_id=bport)
                self.assertEqual(status, SAI_STATUS_SUCCESS)

                if port in lag1_ports:
                    rcv_port = lag1_ports
                    print("Sending packet on port %d, %s -> %s - forward to "
                          "LAG1" % (chck_port, self.chck_mac, self.moving_mac))
                elif port in lag3_ports:
                    rcv_port = lag3_ports
                    print("Sending packet on port %d, %s -> %s - forward to "
                          "LAG3" % (chck_port, self.chck_mac, self.moving_mac))
                else:
                    rcv_port = [port]
                    print("Sending packet on port %d, %s -> %s - forward to "
                          "port %d" % (chck_port, self.chck_mac,
                                       self.moving_mac, port))

                send_packet(self, chck_port, chck_pkt)
                verify_packet_any_port(self, chck_pkt, rcv_port)

            print("\tVerification complete\n")

        finally:
            sai_thrift_remove_fdb_entry(self.client, moving_fdb_entry)


@group("draft")
class FdbFlushTest(SaiHelper):
    '''
    MAC flush test that checks flushing functionality for static and dynamic
    MAC addresses in FDB.
    Test includes flushing per vlan, per bridge port, per vlan/bridge port
    for static and dynamic MACs, all static MACs, all dynamic MACs and all
    MACs in FDB.
    '''

    def setUp(self):
        super(FdbFlushTest, self).setUp()

        # change port1 (VLAN 10) original tagging mode
        sai_thrift_remove_vlan_member(self.client, self.vlan10_member1)
        self.vlan10_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.port1_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_port_attribute(self.client, self.port1, port_vlan_id=10)

        # change port3 (VLAN 20) original tagging mode
        sai_thrift_remove_vlan_member(self.client, self.vlan20_member1)
        self.vlan20_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan20,
            bridge_port_id=self.port3_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_port_attribute(self.client, self.port3, port_vlan_id=20)

        # change lag2 (VLAN 20) original tagging mode
        sai_thrift_remove_vlan_member(self.client, self.vlan20_member2)
        self.vlan20_member2 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan20,
            bridge_port_id=self.lag2_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_lag_attribute(self.client, self.lag2, port_vlan_id=20)

        self.vlan10_id = 10
        self.vlan20_id = 20
        self.tp10_stat_mac = None
        self.tp10_dyn_mac = None
        self.tp20_stat_mac = None
        self.tp20_dyn_mac = None
        self.vlan10_member3 = None
        self.vlan20_member3 = None

        self.vlan10_ports = [self.dev_port0, self.dev_port1, self.dev_port4,
                             self.dev_port5, self.dev_port6]
        self.vlan10_ports_bp = [self.port0_bp, self.port1_bp, self.lag1_bp,
                                self.lag1_bp, self.lag1_bp]
        self.vlan10_lag_ports = [
            self.dev_port4, self.dev_port5, self.dev_port6]
        self.vlan10_stat_macs = []
        self.vlan10_dyn_macs = []
        for i in range(1, len(self.vlan10_ports) + 1):
            mac = "00:10:00:%02d:%02d:%02d" % (i, i, i)
            self.vlan10_stat_macs.append(mac)
            mac = "00:10:ff:%02d:%02d:%02d" % (i, i, i)
            self.vlan10_dyn_macs.append(mac)

        self.vlan20_ports = [self.dev_port2, self.dev_port3, self.dev_port7,
                             self.dev_port8, self.dev_port9]
        self.vlan20_ports_bp = [self.port2_bp, self.port3_bp, self.lag2_bp,
                                self.lag2_bp, self.lag2_bp]
        self.vlan20_lag_ports = [
            self.dev_port7, self.dev_port8, self.dev_port9]
        self.vlan20_stat_macs = []
        self.vlan20_dyn_macs = []
        for i in range(1, len(self.vlan20_ports) + 1):
            mac = "00:20:00:%02d:%02d:%02d" % (i, i, i)
            self.vlan20_stat_macs.append(mac)
            mac = "00:20:ff:%02d:%02d:%02d" % (i, i, i)
            self.vlan20_dyn_macs.append(mac)

        # create trunk port for both: VLAN 10 and VLAN 20
        self.port24_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        self.trunk_port_bp = self.port24_bp
        self.trunk_dev_port = self.dev_port24

    def runTest(self):
        self.flushStaticPerVlanTest()
        self.flushDynamicPerVlanTest()
        self.flushAllPerVlanTest()
        self.flushStaticPerPortTest()
        self.flushDynamicPerPortTest()
        self.flushAllPerPortTest()
        self.flushStaticPerLagTest()
        self.flushDynamicPerLagTest()
        self.flushAllPerLagTest()
        self.flushStaticPerVlanAndPortTest()
        self.flushDynamicPerVlanAndPortTest()
        self.flushAllPerVlanAndPortTest()
        self.flushAllStaticTest()
        self.flushAllDynamicTest()
        self.flushAllMacsTest()

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        sai_thrift_remove_bridge_port(self.client, self.port24_bp)

        # restore port1 (VLAN 10) original tagging mode
        sai_thrift_set_port_attribute(self.client, self.port1, port_vlan_id=0)
        sai_thrift_remove_vlan_member(self.client, self.vlan10_member1)
        self.vlan10_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.port1_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        # restore port3 (VLAN 20) original tagging mode
        sai_thrift_set_port_attribute(self.client, self.port3, port_vlan_id=0)
        sai_thrift_remove_vlan_member(self.client, self.vlan20_member1)
        self.vlan20_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan20,
            bridge_port_id=self.port3_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        # restore lag2 (VLAN 20) original tagging mode
        sai_thrift_set_lag_attribute(self.client, self.lag2, port_vlan_id=0)
        sai_thrift_remove_vlan_member(self.client, self.vlan20_member2)
        self.vlan20_member2 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan20,
            bridge_port_id=self.lag2_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        super(FdbFlushTest, self).tearDown()

    # additional helper functions
    def _prepareFdb(self):
        '''
        Helper function for preparing FDB with a large number of entries
        '''
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        print("Preparing FDB")

        # add static FDB entries
        for i in range(len(self.vlan10_ports)):
            setattr(
                self, 'vlan10_fdb_entry%d' % i,
                sai_thrift_fdb_entry_t(
                    switch_id=self.switch_id,
                    mac_address=self.vlan10_stat_macs[i],
                    bv_id=self.vlan10))
            sai_thrift_create_fdb_entry(
                self.client,
                getattr(self, "vlan10_fdb_entry%d" % i),
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=self.vlan10_ports_bp[i])
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        for i in range(len(self.vlan20_ports)):
            setattr(self, 'vlan20_fdb_entry%d' % i,
                    sai_thrift_fdb_entry_t(
                        switch_id=self.switch_id,
                        mac_address=self.vlan20_stat_macs[i],
                        bv_id=self.vlan20))
            sai_thrift_create_fdb_entry(
                self.client,
                getattr(self, "vlan20_fdb_entry%d" % i),
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=self.vlan20_ports_bp[i])
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        print("Static MAC enries added")

        # learn dynamic MACs
        for mac, port in zip(self.vlan10_dyn_macs, self.vlan10_ports):
            pkt = simple_udp_packet(eth_dst="ff:ff:ff:ff:ff:ff",
                                    eth_src=mac)
            send_packet(self, port, pkt)

        for mac, port in zip(self.vlan20_dyn_macs, self.vlan20_ports):
            pkt = simple_udp_packet(eth_dst="ff:ff:ff:ff:ff:ff",
                                    eth_src=mac)
            send_packet(self, port, pkt)

        print("Dynamic MACs learned")
        time.sleep(2)
        self.dataplane.flush()
        print("\tFDB entries prepared")


    def _setUpTrunkPort(self):
        '''
        Helper function for trunk port setting
        '''
        self.tp10_stat_mac = "00:10:00:66:66:66"
        self.tp10_dyn_mac = "00:10:ff:66:66:66"
        chck_vlan10_mac = self.vlan10_stat_macs[0]
        chck_vlan10_port = self.dev_port0
        self.tp20_stat_mac = "00:20:00:66:66:66"
        self.tp20_dyn_mac = "00:20:ff:66:66:66"
        chck_vlan20_mac = self.vlan20_stat_macs[0]
        chck_vlan20_port = self.dev_port2

        print("Set up common trunk port for VLAN 10 and VLAN 20")
        self.vlan10_member3 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.trunk_port_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.assertNotEqual(self.vlan10_member3, 0)
        self.vlan20_member3 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan20,
            bridge_port_id=self.trunk_port_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.assertNotEqual(self.vlan20_member3, 0)

        # add static MACs on trunk port
        vlan10_tp_fdb_entry = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=self.tp10_stat_mac,
            bv_id=self.vlan10)
        status = sai_thrift_create_fdb_entry(self.client,
                                             vlan10_tp_fdb_entry,
                                             type=SAI_FDB_ENTRY_TYPE_STATIC,
                                             bridge_port_id=self.trunk_port_bp)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        vlan20_tp_fdb_entry = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=self.tp20_stat_mac,
            bv_id=self.vlan20)
        status = sai_thrift_create_fdb_entry(self.client,
                                             vlan20_tp_fdb_entry,
                                             type=SAI_FDB_ENTRY_TYPE_STATIC,
                                             bridge_port_id=self.trunk_port_bp)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # learn trunk port dynamic MACs
        tag_vlan10_pkt = simple_udp_packet(eth_dst="ff:ff:ff:ff:ff:ff",
                                           eth_src=self.tp10_dyn_mac,
                                           dl_vlan_enable=True,
                                           vlan_vid=self.vlan10_id,
                                           pktlen=104)
        tag_vlan20_pkt = simple_udp_packet(eth_dst="ff:ff:ff:ff:ff:ff",
                                           eth_src=self.tp20_dyn_mac,
                                           dl_vlan_enable=True,
                                           vlan_vid=self.vlan20_id,
                                           pktlen=104)

        send_packet(self, self.trunk_dev_port, tag_vlan10_pkt)
        send_packet(self, self.trunk_dev_port, tag_vlan20_pkt)

        time.sleep(2)
        self.dataplane.flush()

        for mac in [self.tp10_stat_mac, self.tp10_dyn_mac]:
            chck_vlan10_pkt = simple_udp_packet(eth_dst=mac,
                                                eth_src=chck_vlan10_mac,
                                                pktlen=100)
            chck_vlan10_tag_pkt = simple_udp_packet(
                eth_dst=mac,
                eth_src=chck_vlan10_mac,
                dl_vlan_enable=True,
                vlan_vid=self.vlan10_id,
                pktlen=104)
            send_packet(self, chck_vlan10_port, chck_vlan10_pkt)
            verify_packets(self, chck_vlan10_tag_pkt,
                            [self.trunk_dev_port])

        for mac in [self.tp20_stat_mac, self.tp20_dyn_mac]:
            chck_vlan20_pkt = simple_udp_packet(eth_dst=mac,
                                                eth_src=chck_vlan20_mac,
                                                pktlen=100)
            chck_vlan20_tag_pkt = simple_udp_packet(
                eth_dst=mac,
                eth_src=chck_vlan20_mac,
                dl_vlan_enable=True,
                vlan_vid=self.vlan20_id,
                pktlen=104)
            send_packet(self, chck_vlan20_port, chck_vlan20_pkt)
            verify_packets(self, chck_vlan20_tag_pkt,
                            [self.trunk_dev_port])

        print("\tTrunk port configured")

    def _tearDownTrunkPort(self):
        '''
        Helper function for trunk port removing
        '''
        # flush trunk_port MACs
        sai_thrift_flush_fdb_entries(self.client,
                                     bridge_port_id=self.trunk_port_bp,
                                     entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        sai_thrift_remove_vlan_member(self.client, self.vlan10_member3)
        sai_thrift_remove_vlan_member(self.client, self.vlan20_member3)

    def _verifyFwd(self, dst_macs, dst_ports, src_macs, src_ports,
                   lag_ports=None, trunk_port=None, vlan_id=None):
        '''
        Verify if packets with DMAC from dst_macs list are forwarded to
        corresponding ports from dst_ports list.
        *Assuming that src and dst ports are access ports.

        Args:
            dst_macs (list): a list of DMACs
            dst_ports (list): a list of ports corresponding to the DMACs
            src_macs (list): a list of SMACs corresponding to ports taken for
                             trying to send packets from
            src_ports (list): a list of ports taken for trying to send packets
                              from
            lag_ports (list): opional list if some ports are in lag
            trunk_port (int): optional trunk port number (if exists in
                              dst_ports)
            vlan_id (int): optional VLAN number required if trunk_port is given
        '''
        for dst_mac, dst_port in zip(dst_macs, dst_ports):
            for src_mac, src_port in zip(src_macs, src_ports):
                if src_port == dst_port:
                    continue

                pkt = simple_udp_packet(eth_dst=dst_mac,
                                        eth_src=src_mac,
                                        pktlen=100)
                rcv_pkt = pkt

                if dst_port == trunk_port:
                    tag_pkt = simple_udp_packet(eth_dst=dst_mac,
                                                eth_src=src_mac,
                                                dl_vlan_enable=True,
                                                vlan_vid=vlan_id,
                                                pktlen=104)
                    rcv_pkt = tag_pkt

                rcv_port = lag_ports if dst_port in lag_ports else [dst_port]

                send_packet(self, src_port, pkt)
                verify_packet_any_port(self, rcv_pkt, rcv_port)

                break

    def _verifyFlood(self, dst_macs, dst_ports, src_macs, src_ports,
                     lag_ports=None, trunk_port=None, vlan_id=None):
        '''
        Verify if packets with DMAC from dst_macs list are flooded to ports
        from dst_ports list.
        *Assuming that src and dst ports are access ports.

        Args:
            dst_macs (list): a list of DMACs
            dst_ports (list): a list of ports corresponding to the DMACs
            src_macs (list): a list of SMACs corresponding to ports taken for
                             trying to send packets from
            src_ports (list): a list of ports taken for trying to send packets
                              from
            lag_ports (list): opional list if some ports are in lag
            trunk_port (int): optional trunk port number (if exists in
                              dst_ports)
            vlan_id (int): optional VLAN number required if trunk_port is given
        '''
        for dst_mac in dst_macs:
            for src_mac, src_port in zip(src_macs, src_ports):
                if src_mac == dst_mac:
                    continue

                pkt = simple_udp_packet(eth_dst=dst_mac,
                                        eth_src=src_mac,
                                        pktlen=100)

                port_list = []
                lag_set = False
                for port in dst_ports:
                    if port == src_port:
                        continue
                    if port in lag_ports:
                        if lag_set:
                            continue
                        port_list.append(lag_ports)
                        lag_set = True
                    else:
                        port_list.append([port])

                pkt_list = [pkt] * len(port_list)

                if trunk_port is not None:
                    port_list.append([trunk_port])

                    tag_pkt = simple_udp_packet(eth_dst=dst_mac,
                                                eth_src=src_mac,
                                                dl_vlan_enable=True,
                                                vlan_vid=vlan_id,
                                                pktlen=104)
                    pkt_list.append(tag_pkt)

                send_packet(self, src_port, pkt)
                verify_each_packet_on_multiple_port_lists(self, pkt_list,
                                                          port_list)

                break

    # tests funcions
    def flushStaticPerVlanTest(self):
        '''
        Verify flushing of static MAC entries by VLAN
        '''
        print("\nflushStaticPerVlanTest()")
        self._prepareFdb()

        print("Flush static MACs by VLAN on VLAN 10")
        sai_thrift_flush_fdb_entries(
            self.client,
            bv_id=self.vlan10,
            entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_STATIC)

        print("Verify flooding on VLAN 10 static FDB entries")
        self._verifyFlood(self.vlan10_stat_macs, self.vlan10_ports,
                          [self.vlan10_dyn_macs[0], self.vlan10_dyn_macs[1]],
                          [self.dev_port0, self.dev_port1],
                          self.vlan10_lag_ports)

        print("Verify forwarding for all other FDB entires")
        # verify VLAN 20 static MACs
        self._verifyFwd(self.vlan20_stat_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)
        # verify VLAN 10 dynamic MACs
        self._verifyFwd(self.vlan10_dyn_macs, self.vlan10_ports,
                        [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 dynamic MACs
        self._verifyFwd(self.vlan20_dyn_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)

        print("\tVerification complete")

    def flushDynamicPerVlanTest(self):
        '''
        Verify flushing of dynamic MAC entries by VLAN
        '''
        print("\nflushDynamicPerVlanTest()")
        self._prepareFdb()

        print("Flush dynamic MACs by VLAN on VLAN 10")
        sai_thrift_flush_fdb_entries(
            self.client,
            bv_id=self.vlan10,
            entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)

        print("Verify flooding on VLAN 10 dynamic FDB entries")
        self._verifyFlood(self.vlan10_dyn_macs, self.vlan10_ports,
                          [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                          [self.dev_port0, self.dev_port1],
                          self.vlan10_lag_ports)

        print("Verify forwarding for all other FDB entires")
        # verify VLAN 10 static MACs
        self._verifyFwd(self.vlan10_stat_macs, self.vlan10_ports,
                        [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 static MACs
        self._verifyFwd(self.vlan20_stat_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)
        # verify VLAN 20 dynamic MACs
        self._verifyFwd(self.vlan20_dyn_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)

        print("\tVerification complete")

    def flushAllPerVlanTest(self):
        '''
        Verify flushing of all kinds of MAC entries by VLAN
        '''
        print("\nflushAllPerVlanTest()")
        self._prepareFdb()

        print("Flush MACs by VLAN on VLAN 10")
        sai_thrift_flush_fdb_entries(self.client,
                                     bv_id=self.vlan10,
                                     entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        chck_mac1 = "00:10:aa:11:11:11"
        chck_mac2 = "00:10:aa:22:22:22"

        print("Verify flooding on VLAN 10 FDB entries")
        self._verifyFlood(self.vlan10_stat_macs, self.vlan10_ports,
                          [chck_mac1, chck_mac2],
                          [self.dev_port0, self.dev_port1],
                          self.vlan10_lag_ports)

        self._verifyFlood(self.vlan10_dyn_macs, self.vlan10_ports,
                          [chck_mac1, chck_mac2],
                          [self.dev_port0, self.dev_port1],
                          self.vlan10_lag_ports)

        print("Verify forwarding for all other FDB entires")
        # verify VLAN 20 static MACs
        self._verifyFwd(self.vlan20_stat_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)
        # verify VLAN 20 dynamic MACs
        self._verifyFwd(self.vlan20_dyn_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)

        print("\tVerification complete")

    def flushStaticPerPortTest(self):
        '''
        Verify flushing of static MAC entries by port
        '''
        print("\nflushStaticPerPortTest()")
        self._prepareFdb()

        flushed_port_bp = self.port0_bp
        flushed_dev_port = self.vlan10_ports[0]
        flushed_mac = self.vlan10_stat_macs[0]

        print("Flush static MAC by port on port %d" % flushed_dev_port)
        sai_thrift_flush_fdb_entries(
            self.client,
            bridge_port_id=flushed_port_bp,
            entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_STATIC)

        print("Verify flooding on flushed port static MAC")
        self._verifyFlood([flushed_mac], self.vlan10_ports,
                          [self.vlan10_dyn_macs[1]], [self.vlan10_ports[1]],
                          self.vlan10_lag_ports)

        not_flushed_ports = [
            self.vlan10_ports[i] for i in range(len(self.vlan10_ports))
            if self.vlan10_ports[i] != flushed_dev_port]
        not_flushed_macs = [
            self.vlan10_stat_macs[i] for i in range(len(self.vlan10_stat_macs))
            if self.vlan10_stat_macs[i] != flushed_mac]

        print("Verify forwarding for all other FDB entires")
        # verify VLAN 10 static MACs
        self._verifyFwd(not_flushed_macs, not_flushed_ports,
                        [self.vlan10_dyn_macs[0], self.vlan10_dyn_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 static MACs
        self._verifyFwd(self.vlan20_stat_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)
        # verify VLAN 10 dynamic MACs
        self._verifyFwd(self.vlan10_dyn_macs, self.vlan10_ports,
                        [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 dynamic MACs
        self._verifyFwd(self.vlan20_dyn_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)

        print("\tVerification complete")

    def flushDynamicPerPortTest(self):
        '''
        Verify flushing of dynamic MAC entries by port
        '''
        print("\nflushDynamicPerPortTest()")
        self._prepareFdb()

        flushed_port_bp = self.port0_bp
        flushed_dev_port = self.vlan10_ports[0]
        flushed_mac = self.vlan10_dyn_macs[0]

        print("Flush dynamic MAC by port on port %d" % flushed_dev_port)
        sai_thrift_flush_fdb_entries(
            self.client,
            bridge_port_id=flushed_port_bp,
            entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)

        print("Verify flooding on flushed port static MAC")
        self._verifyFlood([flushed_mac], self.vlan10_ports,
                          [self.vlan10_stat_macs[1]], [self.vlan10_ports[1]],
                          self.vlan10_lag_ports)

        not_flushed_ports = [
            self.vlan10_ports[i] for i in range(len(self.vlan10_ports))
            if self.vlan10_ports[i] != flushed_dev_port]
        not_flushed_macs = [
            self.vlan10_dyn_macs[i] for i in range(len(self.vlan10_dyn_macs))
            if self.vlan10_dyn_macs[i] != flushed_mac]

        print("Verify forwarding for all other FDB entires")
        # verify VLAN 10 static MACs
        self._verifyFwd(self.vlan10_stat_macs, self.vlan10_ports,
                        [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 static MACs
        self._verifyFwd(self.vlan20_stat_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)
        # verify VLAN 10 dynamic MACs
        self._verifyFwd(not_flushed_macs, not_flushed_ports,
                        [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 dynamic MACs
        self._verifyFwd(self.vlan20_dyn_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)

        print("\tVerification complete")

    def flushAllPerPortTest(self):
        '''
        Verify flushing of all kinds of MAC entries by port
        '''
        print("\nflushAllPerPortTest()")
        self._prepareFdb()

        flushed_port_bp = self.port0_bp
        flushed_dev_port = self.vlan10_ports[0]
        flushed_stat_mac = self.vlan10_stat_macs[0]
        flushed_dyn_mac = self.vlan10_dyn_macs[0]

        print("Flush all MACs by port on port %d" % flushed_dev_port)
        sai_thrift_flush_fdb_entries(self.client,
                                     bridge_port_id=flushed_port_bp,
                                     entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        print("Verify flooding on flushed port MACs")
        self._verifyFlood([flushed_stat_mac], self.vlan10_ports,
                          [self.vlan10_dyn_macs[1]], [self.vlan10_ports[1]],
                          self.vlan10_lag_ports)

        self._verifyFlood([flushed_dyn_mac], self.vlan10_ports,
                          [self.vlan10_stat_macs[1]], [self.vlan10_ports[1]],
                          self.vlan10_lag_ports)

        not_flushed_ports = [
            self.vlan10_ports[i] for i in range(len(self.vlan10_ports))
            if self.vlan10_ports[i] != flushed_dev_port]
        not_flushed_stat_macs = [
            self.vlan10_stat_macs[i] for i in range(len(self.vlan10_stat_macs))
            if self.vlan10_stat_macs[i] != flushed_stat_mac]
        not_flushed_dyn_macs = [
            self.vlan10_dyn_macs[i] for i in range(len(self.vlan10_dyn_macs))
            if self.vlan10_dyn_macs[i] != flushed_dyn_mac]

        print("Verify forwarding for all other FDB entires")
        # verify VLAN 10 static MACs
        self._verifyFwd(not_flushed_stat_macs, not_flushed_ports,
                        [self.vlan10_dyn_macs[0], self.vlan10_dyn_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 static MACs
        self._verifyFwd(self.vlan20_stat_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)
        # verify VLAN 10 dynamic MACs
        self._verifyFwd(not_flushed_dyn_macs, not_flushed_ports,
                        [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 dynamic MACs
        self._verifyFwd(self.vlan20_dyn_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)

        print("\tVerification complete")

    def flushStaticPerLagTest(self):
        '''
        Verify flushing of static MAC entries by LAG
        '''
        print("\nflushStaticPerLagTest()")
        self._prepareFdb()

        flushed_lag_bp = self.lag1_bp
        flushed_dev_ports = self.vlan10_lag_ports
        flushed_macs = [self.vlan10_stat_macs[2], self.vlan10_stat_macs[3],
                        self.vlan10_stat_macs[4]]

        print("Flush static MACs by LAG on LAG1")
        sai_thrift_flush_fdb_entries(
            self.client,
            bridge_port_id=flushed_lag_bp,
            entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_STATIC)

        print("Verify flooding on flushed LAG static MACs")
        self._verifyFlood(flushed_macs, self.vlan10_ports,
                          [self.vlan10_dyn_macs[1]], [self.vlan10_ports[1]],
                          self.vlan10_lag_ports)

        not_flushed_ports = [
            self.vlan10_ports[i] for i in range(len(self.vlan10_ports))
            if self.vlan10_ports[i] not in flushed_dev_ports]
        not_flushed_macs = [
            self.vlan10_stat_macs[i] for i in range(len(self.vlan10_stat_macs))
            if self.vlan10_stat_macs[i] not in flushed_macs]

        print("Verify forwarding for all other FDB entires")
        # verify VLAN 10 static MACs
        self._verifyFwd(not_flushed_macs, not_flushed_ports,
                        [self.vlan10_dyn_macs[0], self.vlan10_dyn_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 static MACs
        self._verifyFwd(self.vlan20_stat_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)
        # verify VLAN 10 dynamic MACs
        self._verifyFwd(self.vlan10_dyn_macs, self.vlan10_ports,
                        [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 dynamic MACs
        self._verifyFwd(self.vlan20_dyn_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)

        print("\tVerification complete")

    def flushDynamicPerLagTest(self):
        '''
        Verify flushing of dynamic MAC entries by LAG
        '''
        print("\nflushDynamicPerLagTest()")
        self._prepareFdb()

        flushed_lag_bp = self.lag1_bp
        flushed_dev_ports = self.vlan10_lag_ports
        flushed_macs = [self.vlan10_dyn_macs[2], self.vlan10_dyn_macs[3],
                        self.vlan10_dyn_macs[4]]

        print("Flush dynamic MACs by LAG on LAG1")
        sai_thrift_flush_fdb_entries(
            self.client,
            bridge_port_id=flushed_lag_bp,
            entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)

        print("Verify flooding on flushed LAG dynamic MACs")
        self._verifyFlood(flushed_macs, self.vlan10_ports,
                          [self.vlan10_stat_macs[1]], [self.vlan10_ports[1]],
                          self.vlan10_lag_ports)

        not_flushed_ports = [
            self.vlan10_ports[i] for i in range(len(self.vlan10_ports))
            if self.vlan10_ports[i] not in flushed_dev_ports]
        not_flushed_macs = [
            self.vlan10_dyn_macs[i] for i in range(len(self.vlan10_dyn_macs))
            if self.vlan10_dyn_macs[i] not in flushed_macs]

        print("Verify forwarding for all other FDB entires")
        # verify VLAN 10 static MACs
        self._verifyFwd(self.vlan10_stat_macs, self.vlan10_ports,
                        [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 static MACs
        self._verifyFwd(self.vlan20_stat_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)
        # verify VLAN 10 dynamic MACs
        self._verifyFwd(not_flushed_macs, not_flushed_ports,
                        [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 dynamic MACs
        self._verifyFwd(self.vlan20_dyn_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)

        print("\tVerification complete")

    def flushAllPerLagTest(self):
        '''
        Verify flushing of all kinds of MAC entries by LAG
        '''
        print("\nflushAllPerLagTest()")
        self._prepareFdb()

        flushed_lag_bp = self.lag1_bp
        flushed_dev_ports = self.vlan10_lag_ports
        flushed_stat_macs = [self.vlan10_stat_macs[2], self.vlan10_stat_macs[3],
                             self.vlan10_stat_macs[4]]
        flushed_dyn_macs = [self.vlan10_dyn_macs[2], self.vlan10_dyn_macs[3],
                            self.vlan10_dyn_macs[4]]

        print("Flush all MACs by LAG on LAG1")
        sai_thrift_flush_fdb_entries(
            self.client,
            bridge_port_id=flushed_lag_bp,
            entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)

        not_flushed_ports = [
            self.vlan10_ports[i] for i in range(len(self.vlan10_ports))
            if self.vlan10_ports[i] not in flushed_dev_ports]
        not_flushed_stat_macs = [
            self.vlan10_stat_macs[i] for i in range(len(self.vlan10_stat_macs))
            if self.vlan10_stat_macs[i] not in flushed_stat_macs]
        not_flushed_dyn_macs = [
            self.vlan10_dyn_macs[i] for i in range(len(self.vlan10_dyn_macs))
            if self.vlan10_dyn_macs[i] not in flushed_dyn_macs]

        print("Verify forwarding for all other FDB entires")
        # verify VLAN 10 static MACs
        self._verifyFwd(not_flushed_stat_macs, not_flushed_ports,
                        [self.vlan10_dyn_macs[0], self.vlan10_dyn_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 static MACs
        self._verifyFwd(self.vlan20_stat_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)
        # verify VLAN 10 dynamic MACs
        self._verifyFwd(not_flushed_dyn_macs, not_flushed_ports,
                        [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 dynamic MACs
        self._verifyFwd(self.vlan20_dyn_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)

        print("\tVerification complete")

    def flushStaticPerVlanAndPortTest(self):
        '''
        Verify flushing of static MAC entries by VLAN and port
        '''
        print("\nflushStaticPerVlanAndPortTest()")
        self._prepareFdb()

        chck_vlan10_port = self.dev_port0
        chck_vlan10_mac = self.vlan10_stat_macs[0]

        try:
            self._setUpTrunkPort()

            print("Flush static MAC by VLAN and port on trunk port")
            sai_thrift_flush_fdb_entries(
                self.client,
                bv_id=self.vlan10,
                bridge_port_id=self.trunk_port_bp,
                entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_STATIC)

            print("Verify flooding on flushed MAC")
            self._verifyFlood([self.tp10_stat_mac], self.vlan10_ports,
                              [chck_vlan10_mac], [chck_vlan10_port],
                              self.vlan10_lag_ports, self.trunk_dev_port,
                              self.vlan10_id)

            print("Verify forwarding for all other FDB entries")
            self.vlan10_ports.append(self.trunk_dev_port)
            self.vlan20_ports.append(self.trunk_dev_port)
            self.vlan20_stat_macs.append(self.tp20_stat_mac)
            self.vlan10_dyn_macs.append(self.tp10_dyn_mac)
            self.vlan20_dyn_macs.append(self.tp20_dyn_mac)
            # verify VLAN 10 static MACs
            self._verifyFwd(
                self.vlan10_stat_macs, self.vlan10_ports,
                [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                [self.dev_port0, self.dev_port1], self.vlan10_lag_ports,
                self.trunk_dev_port, self.vlan10_id)
            # verify VLAN 20 static MACs
            self._verifyFwd(
                self.vlan20_stat_macs, self.vlan20_ports,
                [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                [self.dev_port2, self.dev_port3], self.vlan20_lag_ports,
                self.trunk_dev_port, self.vlan20_id)
            # verify VLAN 10 dynamic MACs
            self._verifyFwd(
                self.vlan10_dyn_macs, self.vlan10_ports,
                [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                [self.dev_port0, self.dev_port1], self.vlan10_lag_ports,
                self.trunk_dev_port, self.vlan10_id)
            # verify VLAN 20 dynamic MACs
            self._verifyFwd(
                self.vlan20_dyn_macs, self.vlan20_ports,
                [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                [self.dev_port2, self.dev_port3], self.vlan20_lag_ports,
                self.trunk_dev_port, self.vlan20_id)

            print("\tVerification complete")

        finally:
            self.vlan10_ports.remove(self.trunk_dev_port)
            self.vlan20_ports.remove(self.trunk_dev_port)
            self.vlan20_stat_macs.remove(self.tp20_stat_mac)
            self.vlan10_dyn_macs.remove(self.tp10_dyn_mac)
            self.vlan20_dyn_macs.remove(self.tp20_dyn_mac)

            self._tearDownTrunkPort()

    def flushDynamicPerVlanAndPortTest(self):
        '''
        Verify flushing of dynamic MAC entries by VLAN and port
        '''
        print("\nflushDynamicPerVlanAndPortTest()")
        self._prepareFdb()

        chck_vlan10_port = self.dev_port0
        chck_vlan10_mac = self.vlan10_stat_macs[0]

        try:
            self._setUpTrunkPort()

            print("Flush dynamic MAC by VLAN and port on trunk port")
            sai_thrift_flush_fdb_entries(
                self.client,
                bv_id=self.vlan10,
                bridge_port_id=self.trunk_port_bp,
                entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)

            print("Verify flooding on flushed MAC")
            self._verifyFlood([self.tp10_dyn_mac], self.vlan10_ports,
                              [chck_vlan10_mac], [chck_vlan10_port],
                              self.vlan10_lag_ports, self.trunk_dev_port,
                              self.vlan10_id)

            print("Verify forwarding for all other FDB entries")
            self.vlan10_ports.append(self.trunk_dev_port)
            self.vlan20_ports.append(self.trunk_dev_port)
            self.vlan10_stat_macs.append(self.tp10_stat_mac)
            self.vlan20_stat_macs.append(self.tp20_stat_mac)
            self.vlan20_dyn_macs.append(self.tp20_dyn_mac)
            # verify VLAN 10 static MACs
            self._verifyFwd(
                self.vlan10_stat_macs, self.vlan10_ports,
                [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                [self.dev_port0, self.dev_port1], self.vlan10_lag_ports,
                self.trunk_dev_port, self.vlan10_id)
            # verify VLAN 20 static MACs
            self._verifyFwd(
                self.vlan20_stat_macs, self.vlan20_ports,
                [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                [self.dev_port2, self.dev_port3], self.vlan20_lag_ports,
                self.trunk_dev_port, self.vlan20_id)
            # verify VLAN 10 dynamic MACs
            self._verifyFwd(
                self.vlan10_dyn_macs, self.vlan10_ports,
                [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                [self.dev_port0, self.dev_port1], self.vlan10_lag_ports,
                self.trunk_dev_port, self.vlan10_id)
            # verify VLAN 20 dynamic MACs
            self._verifyFwd(
                self.vlan20_dyn_macs, self.vlan20_ports,
                [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                [self.dev_port2, self.dev_port3], self.vlan20_lag_ports,
                self.trunk_dev_port, self.vlan20_id)

            print("\tVerification complete")

        finally:
            self.vlan10_ports.remove(self.trunk_dev_port)
            self.vlan20_ports.remove(self.trunk_dev_port)
            self.vlan10_stat_macs.remove(self.tp10_stat_mac)
            self.vlan20_stat_macs.remove(self.tp20_stat_mac)
            self.vlan20_dyn_macs.remove(self.tp20_dyn_mac)

            self._tearDownTrunkPort()

    def flushAllPerVlanAndPortTest(self):
        '''
        Verify flushing of all kinds of MAC entries by VLAN and port
        '''
        print("\nflushAllPerVlanAndPortTest()")
        self._prepareFdb()

        chck_vlan10_port = self.dev_port0
        chck_vlan10_mac = self.vlan10_stat_macs[0]

        try:
            self._setUpTrunkPort()

            print("Flush all MACs by VLAN and port on trunk port")
            sai_thrift_flush_fdb_entries(
                self.client,
                bv_id=self.vlan10,
                bridge_port_id=self.trunk_port_bp,
                entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            print("Verify flooding on flushed MAC")
            self._verifyFlood([self.tp10_stat_mac], self.vlan10_ports,
                              [chck_vlan10_mac], [chck_vlan10_port],
                              self.vlan10_lag_ports, self.trunk_dev_port,
                              self.vlan10_id)

            self._verifyFlood([self.tp10_dyn_mac], self.vlan10_ports,
                              [chck_vlan10_mac], [chck_vlan10_port],
                              self.vlan10_lag_ports, self.trunk_dev_port,
                              self.vlan10_id)

            print("Verify forwarding for all other FDB entries")
            self.vlan10_ports.append(self.trunk_dev_port)
            self.vlan20_ports.append(self.trunk_dev_port)
            self.vlan20_stat_macs.append(self.tp20_stat_mac)
            self.vlan20_dyn_macs.append(self.tp20_dyn_mac)
            # verify VLAN 10 static MACs
            self._verifyFwd(
                self.vlan10_stat_macs, self.vlan10_ports,
                [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                [self.dev_port0, self.dev_port1], self.vlan10_lag_ports,
                self.trunk_dev_port, self.vlan10_id)
            # verify VLAN 20 static MACs
            self._verifyFwd(
                self.vlan20_stat_macs, self.vlan20_ports,
                [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                [self.dev_port2, self.dev_port3], self.vlan20_lag_ports,
                self.trunk_dev_port, self.vlan20_id)
            # verify VLAN 10 dynamic MACs
            self._verifyFwd(
                self.vlan10_dyn_macs, self.vlan10_ports,
                [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                [self.dev_port0, self.dev_port1], self.vlan10_lag_ports,
                self.trunk_dev_port, self.vlan10_id)
            # verify VLAN 20 dynamic MACs
            self._verifyFwd(
                self.vlan20_dyn_macs, self.vlan20_ports,
                [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                [self.dev_port2, self.dev_port3], self.vlan20_lag_ports,
                self.trunk_dev_port, self.vlan20_id)

            print("\tVerification complete")

        finally:
            self.vlan10_ports.remove(self.trunk_dev_port)
            self.vlan20_ports.remove(self.trunk_dev_port)
            self.vlan20_stat_macs.remove(self.tp20_stat_mac)
            self.vlan20_dyn_macs.remove(self.tp20_dyn_mac)

            self._tearDownTrunkPort()

    def flushAllStaticTest(self):
        '''
        Verify flushing of all static MAC entries
        '''
        print("\nflushAllStaticTest()")
        self._prepareFdb()

        print("Flush all static MACs")
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_STATIC)

        print("Verify flooding on VLAN 10 and VLAN 20 static FDB entries")
        self._verifyFlood(self.vlan10_stat_macs, self.vlan10_ports,
                          [self.vlan10_dyn_macs[0], self.vlan10_dyn_macs[1]],
                          [self.dev_port0, self.dev_port1],
                          self.vlan10_lag_ports)

        self._verifyFlood(self.vlan20_stat_macs, self.vlan20_ports,
                          [self.vlan20_dyn_macs[0], self.vlan20_dyn_macs[1]],
                          [self.dev_port2, self.dev_port3],
                          self.vlan20_lag_ports)

        print("Verify forwarding for all other FDB entires")
        # verify VLAN 10 dynamic MACs
        self._verifyFwd(self.vlan10_dyn_macs, self.vlan10_ports,
                        [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 dynamic MACs
        self._verifyFwd(self.vlan20_dyn_macs, self.vlan20_ports,
                        [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)

        print("\tVerification complete")

    def flushAllDynamicTest(self):
        '''
        Verify flushing of all dynamic MAC entries
        '''
        print("\nflushAllDynamicTest()")
        self._prepareFdb()

        print("Flush all dynamic MACs")
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)

        print("Verify flooding on VLAN 10 and VLAN 20 dynamic FDB entries")
        self._verifyFlood(self.vlan10_dyn_macs, self.vlan10_ports,
                          [self.vlan10_stat_macs[0], self.vlan10_stat_macs[1]],
                          [self.dev_port0, self.dev_port1],
                          self.vlan10_lag_ports)

        self._verifyFlood(self.vlan20_dyn_macs, self.vlan20_ports,
                          [self.vlan20_stat_macs[0], self.vlan20_stat_macs[1]],
                          [self.dev_port2, self.dev_port3],
                          self.vlan20_lag_ports)

        print("Verify forwarding for all other FDB entires")
        # verify VLAN 10 static MACs
        self._verifyFwd(self.vlan10_stat_macs, self.vlan10_ports,
                        [self.vlan10_dyn_macs[0], self.vlan10_dyn_macs[1]],
                        [self.dev_port0, self.dev_port1],
                        self.vlan10_lag_ports)
        # verify VLAN 20 static MACs
        self._verifyFwd(self.vlan20_stat_macs, self.vlan20_ports,
                        [self.vlan20_dyn_macs[0], self.vlan20_dyn_macs[1]],
                        [self.dev_port2, self.dev_port3],
                        self.vlan20_lag_ports)

        print("\tVerification complete")

    def flushAllMacsTest(self):
        '''
        Verify flushing of all kinds of MAC entries
        '''
        print("\nflushAllMacsTest()")
        self._prepareFdb()

        chck_vlan10_mac1 = "00:10:aa:11:11:11"
        chck_vlan10_mac2 = "00:10:aa:22:22:22"
        chck_vlan20_mac1 = "00:20:aa:11:11:11"
        chck_vlan20_mac2 = "00:20:aa:22:22:22"

        print("Flush all MACs in FDB")
        sai_thrift_flush_fdb_entries(self.client,
                                     entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        print("Verify flooding for all flushed FDB entires")
        self._verifyFlood(self.vlan10_stat_macs, self.vlan10_ports,
                          [chck_vlan10_mac1, chck_vlan10_mac2],
                          [self.dev_port0, self.dev_port1],
                          self.vlan10_lag_ports)

        self._verifyFlood(self.vlan10_dyn_macs, self.vlan10_ports,
                          [chck_vlan10_mac1, chck_vlan10_mac2],
                          [self.dev_port0, self.dev_port1],
                          self.vlan10_lag_ports)

        self._verifyFlood(self.vlan20_stat_macs, self.vlan20_ports,
                          [chck_vlan20_mac1, chck_vlan20_mac2],
                          [self.dev_port2, self.dev_port3],
                          self.vlan20_lag_ports)

        self._verifyFlood(self.vlan20_dyn_macs, self.vlan20_ports,
                          [chck_vlan20_mac1, chck_vlan20_mac2],
                          [self.dev_port2, self.dev_port3],
                          self.vlan20_lag_ports)

        print("\tVerification complete")


@group("draft")
class FdbAgeTest(SaiHelper):
    '''
    Verify FDB entries aging
    '''

    def setUp(self):
        super(FdbAgeTest, self).setUp()

        # age time used in tests (in sec)
        self.age_time = 10
        status = sai_thrift_set_switch_attribute(self.client,
                                                 fdb_aging_time=self.age_time)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("Aging time set to %d sec for all tests" % self.age_time)

        sw_attr = sai_thrift_get_switch_attribute(self.client,
                                                  fdb_aging_time=True)
        self.assertEqual(sw_attr["fdb_aging_time"], self.age_time)

        self.vlan_id = 10

        # add one more port to VLAN 10
        self.port24_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.assertNotEqual(self.port24_bp, 0)
        self.vlan10_member3 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        sai_thrift_set_port_attribute(self.client,
                                      self.port24,
                                      port_vlan_id=10)

        # add static MAC address for port0 for verification purposes
        self.vrf_mac = "00:12:34:56:78:90"
        vrf_port_bp = self.port24_bp
        self.vrf_port_dev = self.dev_port24
        self.fdb_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                mac_address=self.vrf_mac,
                                                bv_id=self.vlan10)
        sai_thrift_create_fdb_entry(self.client,
                                    self.fdb_entry,
                                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                                    bridge_port_id=vrf_port_bp)

    def runTest(self):
        self.macAgingOnPortTest()
        self.macAgingOnLagTest()
        self.macAgingAfterMoveTest()
        self.macMoveAfterAgingTest()

    def tearDown(self):
        # remove static MAC from FDB
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry)

        # remove additional port from VLAN 10
        sai_thrift_set_port_attribute(self.client, self.port24, port_vlan_id=0)
        sai_thrift_remove_vlan_member(self.client, self.vlan10_member3)
        sai_thrift_remove_bridge_port(self.client, self.port24_bp)

        # disable aging
        status = sai_thrift_set_switch_attribute(self.client, fdb_aging_time=0)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        sw_attr = sai_thrift_get_switch_attribute(self.client,
                                                  fdb_aging_time=True)
        self.assertEqual(sw_attr["fdb_aging_time"], 0)

        super(FdbAgeTest, self).tearDown()

    def macAgingOnPortTest(self):
        '''
        FDB aging test verifying if dynamic FDB entry associated with port
        is removed after the aging interval.
        '''
        print("\nmacAgingOnPortTest()")

        try:
            # learn MAC adress on port1
            lrn_mac = "00:01:01:01:01:01"
            lrn_port = self.dev_port1
            pkt = simple_udp_packet(eth_dst=self.vrf_mac,
                                    eth_src=lrn_mac,
                                    pktlen=100)
            tag_pkt = simple_udp_packet(eth_dst=self.vrf_mac,
                                        eth_src=lrn_mac,
                                        dl_vlan_enable=True,
                                        vlan_vid=self.vlan_id,
                                        pktlen=104)

            print("Sending packet on port %d, %s -> %s to learn MAC address" %
                  (lrn_port, lrn_mac, self.vrf_mac))
            send_packet(self, lrn_port, tag_pkt)
            verify_packets(self, pkt, [self.vrf_port_dev])
            time.sleep(2)

            print("Verifying if MAC address was learned")
            pkt = simple_udp_packet(eth_dst=lrn_mac,
                                    eth_src=self.vrf_mac,
                                    pktlen=100)
            tag_pkt = simple_udp_packet(eth_dst=lrn_mac,
                                        eth_src=self.vrf_mac,
                                        dl_vlan_enable=True,
                                        vlan_vid=self.vlan_id,
                                        pktlen=104)
            print("Sending packet on port %d, %s -> %s - forwarding" %
                  (self.vrf_port_dev, self.vrf_mac, lrn_mac))
            send_packet(self, self.vrf_port_dev, pkt)
            verify_packets(self, tag_pkt, [lrn_port])
            print("\tOK")

            self.saiWaitFdbAge(self.age_time)
            print("Verify if aged MAC address was removed")
            flood_port_list = [[self.dev_port0], [self.dev_port1],
                               [self.dev_port4, self.dev_port5,
                                self.dev_port6]]
            flood_pkt_list = [pkt, tag_pkt, pkt]

            print("Sending packet on port %d, %s -> %s - will flood" %
                  (self.vrf_port_dev, self.vrf_mac, lrn_mac))
            send_packet(self, self.vrf_port_dev, pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tVerification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)

    def macAgingOnLagTest(self):
        '''
        FDB aging test verifying if dynamic FDB entry associated with LAG
        is removed after the aging interval
        '''
        print("\nmacAgingOnLagTest()")

        try:
            # learn MAC adress on LAG1
            lrn_mac = "00:01:01:01:01:01"
            lrn_port = self.dev_port5
            lag_ports = [self.dev_port4, self.dev_port5, self.dev_port6]
            pkt = simple_udp_packet(eth_dst=self.vrf_mac,
                                    eth_src=lrn_mac,
                                    pktlen=100)

            print("Sending packet on port %d, %s -> %s to learn MAC address "
                  "on LAG" % (lrn_port, lrn_mac, self.vrf_mac))
            send_packet(self, lrn_port, pkt)
            verify_packets(self, pkt, [self.vrf_port_dev])
            time.sleep(2)

            print("Verifying if MAC address was learned")
            pkt = simple_udp_packet(eth_dst=lrn_mac,
                                    eth_src=self.vrf_mac,
                                    pktlen=100)
            tag_pkt = simple_udp_packet(eth_dst=lrn_mac,
                                        eth_src=self.vrf_mac,
                                        dl_vlan_enable=True,
                                        vlan_vid=self.vlan_id,
                                        pktlen=104)
            print("Sending packet on port %d, %s -> %s - forwarding" %
                  (self.vrf_port_dev, self.vrf_mac, lrn_mac))
            send_packet(self, self.vrf_port_dev, pkt)
            verify_packet_any_port(self, pkt, lag_ports)
            print("\tOK")

            self.saiWaitFdbAge(self.age_time)

            print("Verify if aged MAC address was removed")
            flood_port_list = [[self.dev_port0], [self.dev_port1],
                               [self.dev_port4, self.dev_port5,
                                self.dev_port6]]
            flood_pkt_list = [pkt, tag_pkt, pkt]

            print("Sending packet on port %d, %s -> %s - will flood" %
                  (self.vrf_port_dev, self.vrf_mac, lrn_mac))
            send_packet(self, self.vrf_port_dev, pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tVerification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)

    def macAgingAfterMoveTest(self):
        '''
        FDB aging test verifying if dynamic FDB entry associated with one port
        and then moved to another port is removed after the aging interval
        counted starting from the moment of moving the address (not the initial
        learning time)
        '''
        print("\nmacAgingAfterMoveTest()")

        try:
            age_time = 25
            status = sai_thrift_set_switch_attribute(self.client,
                                                     fdb_aging_time=age_time)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("Aging time set to %d sec for macAgingAfterMoveTest()" %
                  age_time)

            sw_attr = sai_thrift_get_switch_attribute(self.client,
                                                      fdb_aging_time=True)
            self.assertEqual(sw_attr["fdb_aging_time"], age_time)
            # learn MAC adress on port1
            lrn_mac = "00:01:01:01:01:01"
            lrn_port = self.dev_port1
            mv_port = self.dev_port0
            lrn_pkt = simple_udp_packet(eth_dst=self.vrf_mac,
                                        eth_src=lrn_mac,
                                        pktlen=100)
            lrn_tag_pkt = simple_udp_packet(eth_dst=self.vrf_mac,
                                            eth_src=lrn_mac,
                                            dl_vlan_enable=True,
                                            vlan_vid=self.vlan_id,
                                            pktlen=104)

            print("Sending packet on port %d, %s -> %s to learn MAC address" %
                  (lrn_port, lrn_mac, self.vrf_mac))
            send_packet(self, lrn_port, lrn_tag_pkt)
            verify_packets(self, lrn_pkt, [self.vrf_port_dev])
            time.sleep(2)

            timer_start = time.time()
            print("Verifying if MAC address was learned")
            pkt = simple_udp_packet(eth_dst=lrn_mac,
                                    eth_src=self.vrf_mac,
                                    pktlen=100)
            tag_pkt = simple_udp_packet(eth_dst=lrn_mac,
                                        eth_src=self.vrf_mac,
                                        dl_vlan_enable=True,
                                        vlan_vid=self.vlan_id,
                                        pktlen=104)
            print("Sending packet on port %d, %s -> %s - forwarding" %
                  (self.vrf_port_dev, self.vrf_mac, lrn_mac))
            send_packet(self, self.vrf_port_dev, pkt)
            verify_packets(self, tag_pkt, [lrn_port])
            print("\tOK")

            wait_time = 15
            old_age_out = age_time - wait_time
            print("Waiting for %d seconds before moving MAC" % wait_time)
            while (time.time() - timer_start) < wait_time:
                time.sleep(1)

            print("Moving learned MAC address to port %d" % mv_port)
            print("Sending packet on port %d, %s -> %s to move MAC address" %
                  (mv_port, lrn_mac, self.vrf_mac))
            send_packet(self, mv_port, lrn_pkt)
            verify_packets(self, lrn_pkt, [self.vrf_port_dev])
            time.sleep(1)
            timer_start = time.time()

            print("Verifying if MAC address was moved")
            print("Sending packet on port %d, %s -> %s - forwarding" %
                  (self.vrf_port_dev, self.vrf_mac, lrn_mac))
            send_packet(self, self.vrf_port_dev, pkt)
            verify_packets(self, pkt, [mv_port])
            print("\tOK")

            old_learn_timeout = old_age_out - (time.time() - timer_start)
            print("Age Time remaining from initial mac learning %d seconds" %
                  old_learn_timeout)
            print("Verifying if MAC address was not removed after age_time "
                  "from initial learning")
            self.saiWaitFdbAge(old_learn_timeout)
            print("Sending packet on port %d, %s -> %s - forwarding" %
                  (self.vrf_port_dev, self.vrf_mac, lrn_mac))
            send_packet(self, self.vrf_port_dev, pkt)
            verify_packets(self, pkt, [mv_port])
            print("\tOK")
            new_learn_timeout = age_time - (time.time() - timer_start)
            self.saiWaitFdbAge(new_learn_timeout)

            print("Verify if aged MAC address was removed")
            flood_port_list = [[self.dev_port0], [self.dev_port1],
                               [self.dev_port4, self.dev_port5,
                                self.dev_port6]]
            flood_pkt_list = [pkt, tag_pkt, pkt]

            print("Sending packet on port %d, %s -> %s - will flood" %
                  (self.vrf_port_dev, self.vrf_mac, lrn_mac))
            send_packet(self, self.vrf_port_dev, pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tVerification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)
            sai_thrift_set_switch_attribute(self.client,
                                            fdb_aging_time=self.age_time)

    def macMoveAfterAgingTest(self):
        '''
        FDB aging test verifying if dynamic FDB entry associated with one port
        and then aged and moved to another port is removed after the aging
        interval counted starting from the moment of moving the address (not
        the initial time)
        '''
        print("\nmacMoveAfterAgingTest()")

        try:
            # learn MAC adress on port1
            lrn_mac = "00:01:01:01:01:01"
            lrn_port = self.dev_port1
            mv_port = self.dev_port0
            lrn_pkt = simple_udp_packet(eth_dst=self.vrf_mac,
                                        eth_src=lrn_mac,
                                        pktlen=100)
            lrn_tag_pkt = simple_udp_packet(eth_dst=self.vrf_mac,
                                            eth_src=lrn_mac,
                                            dl_vlan_enable=True,
                                            vlan_vid=self.vlan_id,
                                            pktlen=104)

            print("Sending packet on port %d, %s -> %s to learn MAC address" %
                  (lrn_port, lrn_mac, self.vrf_mac))
            send_packet(self, lrn_port, lrn_tag_pkt)
            verify_packets(self, lrn_pkt, [self.vrf_port_dev])
            time.sleep(2)

            self.saiWaitFdbAge(self.age_time)

            print("Moving learned MAC address to port %d" % mv_port)
            print("Sending packet on port %d, %s -> %s to move MAC address" %
                  (mv_port, lrn_mac, self.vrf_mac))
            send_packet(self, mv_port, lrn_pkt)
            verify_packets(self, lrn_pkt, [self.vrf_port_dev])
            time.sleep(1)

            print("Verifying if MAC address was moved")
            pkt = simple_udp_packet(eth_dst=lrn_mac,
                                    eth_src=self.vrf_mac,
                                    pktlen=100)
            tag_pkt = simple_udp_packet(eth_dst=lrn_mac,
                                        eth_src=self.vrf_mac,
                                        dl_vlan_enable=True,
                                        vlan_vid=self.vlan_id,
                                        pktlen=104)

            print("Sending packet on port %d, %s -> %s - forwarding" %
                  (self.vrf_port_dev, self.vrf_mac, lrn_mac))
            send_packet(self, self.vrf_port_dev, pkt)
            verify_packets(self, pkt, [mv_port])
            print("\tOK")
            self.saiWaitFdbAge(self.age_time)

            print("Verify if aged MAC address was removed")
            flood_port_list = [[self.dev_port0], [self.dev_port1],
                               [self.dev_port4, self.dev_port5,
                                self.dev_port6]]
            flood_pkt_list = [pkt, tag_pkt, pkt]

            print("Sending packet on port %d, %s -> %s - will flood" %
                  (self.vrf_port_dev, self.vrf_mac, lrn_mac))
            send_packet(self, self.vrf_port_dev, pkt)
            verify_each_packet_on_multiple_port_lists(self, flood_pkt_list,
                                                      flood_port_list)
            print("\tVerification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC)


@group("draft")
class FdbMissTest(SaiHelper):
    '''
    Verify actions after missing FDB entry
    '''

    def setUp(self):
        super(FdbMissTest, self).setUp()

        self.vlan_id = 100

        # create VLAN 100 with 3 access ports
        self.port24_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.assertNotEqual(self.port24_bp, 0)
        self.port25_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port25,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.assertNotEqual(self.port25_bp, 0)
        self.port26_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port26,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.assertNotEqual(self.port26_bp, 0)

        self.vlan100 = sai_thrift_create_vlan(self.client, vlan_id=100)
        self.assertNotEqual(self.vlan100, 0)
        self.vlan100_member0 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan100_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port25_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan100_member2 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan100,
            bridge_port_id=self.port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_port_attribute(
            self.client, self.port24, port_vlan_id=100)
        sai_thrift_set_port_attribute(
            self.client, self.port25, port_vlan_id=100)
        sai_thrift_set_port_attribute(
            self.client, self.port26, port_vlan_id=100)

        # create a trap group
        self.trap_group = sai_thrift_create_hostif_trap_group(
            self.client, queue=4)
        self.arp_req_trap = sai_thrift_create_hostif_trap(
            self.client,
            packet_action=SAI_PACKET_ACTION_TRAP,
            trap_type=SAI_HOSTIF_TRAP_TYPE_ARP_REQUEST,
            trap_group=self.trap_group)
        self.lldp_trap = sai_thrift_create_hostif_trap(
            self.client,
            packet_action=SAI_PACKET_ACTION_TRAP,
            trap_type=SAI_HOSTIF_TRAP_TYPE_LLDP,
            trap_group=self.trap_group)

        self.send_port = self.dev_port24
        self.flood_ports = [self.dev_port25, self.dev_port26]

        self.src_mac = "00:11:11:11:11:11"
        self.dst_mac = "00:22:22:22:22:22"
        self.mcast_mac = "01:00:5e:11:22:33"
        self.bcast_mac = "ff:ff:ff:ff:ff:ff"
        self.lldp_mac = "01:80:c2:00:00:0e"

        self.ucast_pkt = simple_udp_packet(eth_dst=self.dst_mac,
                                           eth_src=self.src_mac)
        self.mcast_pkt = simple_udp_packet(eth_dst=self.mcast_mac,
                                           eth_src=self.src_mac)
        self.bcast_pkt = simple_udp_packet(eth_dst=self.bcast_mac,
                                           eth_src=self.src_mac)
        self.arp_pkt = simple_arp_packet(arp_op=1, pktlen=100)
        self.lldp_pkt = simple_eth_packet(eth_dst=self.lldp_mac,
                                          eth_src=self.src_mac,
                                          pktlen=60,
                                          eth_type=0x88cc)

    def runTest(self):
        self.unicastMissDropActionTest()
        self.unicastMissCopyActionTest()
        self.unicastMissTrapActionTest()
        self.multicastMissDropActionTest()
        self.multicastMissCopyActionTest()
        self.multicastMissTrapActionTest()
        self.broadcastMissDropActionTest()
        self.broadcastMissCopyActionTest()
        self.broadcastMissTrapActionTest()

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        # remove trap group
        sai_thrift_remove_hostif_trap(self.client, self.lldp_trap)
        sai_thrift_remove_hostif_trap(self.client, self.arp_req_trap)
        sai_thrift_remove_hostif_trap_group(self.client, self.trap_group)

        # remove VLAN 100
        sai_thrift_set_port_attribute(self.client, self.port24, port_vlan_id=0)
        sai_thrift_set_port_attribute(self.client, self.port25, port_vlan_id=0)
        sai_thrift_set_port_attribute(self.client, self.port26, port_vlan_id=0)

        sai_thrift_remove_vlan_member(self.client, self.vlan100_member0)
        sai_thrift_remove_vlan_member(self.client, self.vlan100_member1)
        sai_thrift_remove_vlan_member(self.client, self.vlan100_member2)

        sai_thrift_remove_vlan(self.client, self.vlan100)

        sai_thrift_remove_bridge_port(self.client, self.port24_bp)
        sai_thrift_remove_bridge_port(self.client, self.port25_bp)
        sai_thrift_remove_bridge_port(self.client, self.port26_bp)

        super(FdbMissTest, self).tearDown()

    def unicastMissDropActionTest(self):
        '''
        Verify if packets, which destination MAC is not stored in FDB,
        are dropped after setting miss packet action to drop
        '''
        print("\nunicastMissDropActionTest()")

        try:
            print("Verifying initial switch behavior")
            print("Sending packet on port %d, %s -> %s - will flood" %
                  (self.send_port, self.src_mac, self.dst_mac))
            send_packet(self, self.send_port, self.ucast_pkt)
            verify_packets(self, self.ucast_pkt, self.flood_ports)
            print("\tOK")

            print("Setting unicast FDB miss packet action to drop")
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_unicast_miss_packet_action=SAI_PACKET_ACTION_DROP)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_unicast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_unicast_miss_packet_action"],
                             SAI_PACKET_ACTION_DROP)

            print("Sending packet on port %d, %s -> %s - will be dropped" %
                  (self.send_port, self.src_mac, self.dst_mac))
            send_packet(self, self.send_port, self.ucast_pkt)
            verify_no_other_packets(self)
            print("\tVerification complete")

        finally:
            # revert default packet acion
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_unicast_miss_packet_action=SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_unicast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_unicast_miss_packet_action"],
                             SAI_PACKET_ACTION_FORWARD)

    def unicastMissCopyActionTest(self):
        '''
        Verify if packets, which destination MAC is not stored in FDB,
        are copied to CPU after setting miss packet action to copy
        '''
        print("\nunicastMissCopyActionTest()")

        try:
            print("Verifying initial switch behavior")
            print("Sending packet on port %d, %s -> %s - will flood" %
                  (self.send_port, self.src_mac, self.dst_mac))
            send_packet(self, self.send_port, self.ucast_pkt)
            verify_packets(self, self.ucast_pkt, self.flood_ports)
            print("\tOK")

            print("Setting unicast FDB miss packet action to copy to CPU")
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_unicast_miss_packet_action=SAI_PACKET_ACTION_COPY)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_unicast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_unicast_miss_packet_action"],
                             SAI_PACKET_ACTION_COPY)

            time.sleep(4)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet on port %d, %s -> %s - will be copied to "
                  "CPU" % (self.send_port, self.src_mac, self.dst_mac))
            send_packet(self, self.send_port, self.ucast_pkt)
            verify_packets(self, self.ucast_pkt, self.flood_ports)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)
            self.dataplane.flush()
            print("\tVerification complete")

        finally:
            # revert default packet acion
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_unicast_miss_packet_action=SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_unicast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_unicast_miss_packet_action"],
                             SAI_PACKET_ACTION_FORWARD)

    def unicastMissTrapActionTest(self):
        '''
        Verify if packets, which destination MAC is not in FDB,
        are redirected to CPU after setting action to trap
        '''
        print("\nunicastMissTrapActionTest()")

        try:
            print("Verifying initial switch behavior")
            print("Sending packet on port %d, %s -> %s - will flood" %
                  (self.send_port, self.src_mac, self.dst_mac))
            send_packet(self, self.send_port, self.ucast_pkt)
            verify_packets(self, self.ucast_pkt, self.flood_ports)
            print("\tOK")

            print("Setting unicast FDB miss packet action to trap")
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_unicast_miss_packet_action=SAI_PACKET_ACTION_TRAP)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_unicast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_unicast_miss_packet_action"],
                             SAI_PACKET_ACTION_TRAP)

            time.sleep(4)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending packet on port %d, %s -> %s - will be redirected "
                  "to CPU" % (self.send_port, self.src_mac, self.dst_mac))
            send_packet(self, self.send_port, self.ucast_pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)
            self.dataplane.flush()
            print("\tVerification complete")

        finally:
            # revert default packet acion
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_unicast_miss_packet_action=SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_unicast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_unicast_miss_packet_action"],
                             SAI_PACKET_ACTION_FORWARD)

    def multicastMissDropActionTest(self):
        '''
        Verify if multicast packets are dropped after setting miss packet
        action to drop.
        Verify also if LLDP packets are still redirected to CPU
        '''
        print("\nmulticastMissDropActionTest()")

        try:
            print("Verifying initial switch behavior")
            print("Sending multicast packet on port %d, %s -> %s - will flood"
                  % (self.send_port, self.src_mac, self.mcast_mac))
            send_packet(self, self.send_port, self.mcast_pkt)
            verify_packets(self, self.mcast_pkt, self.flood_ports)
            print("\tOK")

            print("Setting multicast FDB miss packet action to drop")
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_multicast_miss_packet_action=SAI_PACKET_ACTION_DROP)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_multicast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_multicast_miss_packet_action"],
                             SAI_PACKET_ACTION_DROP)

            print("Sending multicast packet on port %d, %s -> %s - will be "
                  "dropped" % (self.send_port, self.src_mac, self.mcast_mac))
            send_packet(self, self.send_port, self.mcast_pkt)
            verify_no_other_packets(self)
            print("\tOK")

            time.sleep(4)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            print("Checking if LLDP packes are still forwarded to CPU")
            print("Sending multicast LLDP packet on port %d, %s -> %s - will "
                  "be redirected to CPU" % (self.send_port, self.src_mac,
                                            self.lldp_mac))
            send_packet(self, self.send_port, self.lldp_pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)
            print("\nVerificaion complete")

        finally:
            self.dataplane.flush()
            # reveret default multicast packet action
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_multicast_miss_packet_action=SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_multicast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_multicast_miss_packet_action"],
                             SAI_PACKET_ACTION_FORWARD)

    def multicastMissCopyActionTest(self):
        '''
        Verify if multicast packets are copied to CPU after setting miss packet action to copy.
        Verify also if LLDP packets are still redirected to CPU
        '''
        print("\nmulticastMissCopyActionTest()")

        try:
            print("Verifying initial switch behavior")
            print("Sending multicast packet on port %d, %s -> %s - will flood"
                  % (self.send_port, self.src_mac, self.mcast_mac))
            send_packet(self, self.send_port, self.mcast_pkt)
            verify_packets(self, self.mcast_pkt, self.flood_ports)
            print("\tOK")

            print("Setting multicast FDB miss packet action to copy to CPU")
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_multicast_miss_packet_action=SAI_PACKET_ACTION_COPY)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_multicast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_multicast_miss_packet_action"],
                             SAI_PACKET_ACTION_COPY)

            time.sleep(4)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending multicast packet on port %d, %s -> %s - will be "
                  "copied to CPU" % (self.send_port, self.src_mac,
                                     self.mcast_mac))
            send_packet(self, self.send_port, self.mcast_pkt)
            verify_packets(self, self.mcast_pkt, self.flood_ports)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)
            print("\tOK")

            time.sleep(4)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            print("Checking if LLDP packes are still forwarded to CPU")
            print("Sending multicast LLDP packet on port %d, %s -> %s - will "
                  "be redirected to CPU" % (self.send_port, self.src_mac,
                                            self.lldp_mac))
            send_packet(self, self.send_port, self.lldp_pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)
            print("\nVerificaion complete")

        finally:
            self.dataplane.flush()
            # reveret default multicast packet action
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_multicast_miss_packet_action=SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_multicast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_multicast_miss_packet_action"],
                             SAI_PACKET_ACTION_FORWARD)

    def multicastMissTrapActionTest(self):
        '''
        Verify if multicast packets are redirected to CPU after setting miss
        packet action to trap.
        Verify also if LLDP packets are still redirected to CPU.
        '''
        print("\nmulticastMissTrapActionTest()")

        try:
            print("Verifying initial switch behavior")
            print("Sending multicast packet on port %d, %s -> %s - will flood"
                  % (self.send_port, self.src_mac, self.mcast_mac))
            send_packet(self, self.send_port, self.mcast_pkt)
            verify_packets(self, self.mcast_pkt, self.flood_ports)
            print("\tOK")

            print("Setting multicast FDB miss packet action to trap")
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_multicast_miss_packet_action=SAI_PACKET_ACTION_TRAP)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_multicast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_multicast_miss_packet_action"],
                             SAI_PACKET_ACTION_TRAP)

            time.sleep(4)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending multicast packet on port %d, %s -> %s - will be "
                  "redirected to CPU" % (self.send_port, self.src_mac,
                                         self.mcast_mac))
            send_packet(self, self.send_port, self.mcast_pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)
            print("\tOK")

            time.sleep(4)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            print("Checking if LLDP packes are still forwarded to CPU")
            print("Sending multicast LLDP packet on port %d, %s -> %s - will "
                  "be redirected to CPU" % (self.send_port, self.src_mac,
                                            self.lldp_mac))
            send_packet(self, self.send_port, self.lldp_pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)
            print("\nVerificaion complete")

        finally:
            self.dataplane.flush()
            # reveret default multicast packet action
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_multicast_miss_packet_action=SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_multicast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_multicast_miss_packet_action"],
                             SAI_PACKET_ACTION_FORWARD)

    def broadcastMissDropActionTest(self):
        '''
        Verify if broadcast packets are dropped after setting miss packet
        action to drop.
        Verify also if ARP packets are still redirected to CPU.
        '''
        print("\nbroadcastMissDropActionTest()")

        try:
            print("Verifying initial switch behavior")
            print("Sending broadcast packet on port %d, %s -> %s - will flood"
                  % (self.send_port, self.src_mac, self.bcast_mac))
            send_packet(self, self.send_port, self.bcast_pkt)
            verify_packets(self, self.bcast_pkt, self.flood_ports)
            print("\tOK")

            print("Setting broadcast FDB miss packet action to drop")
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_broadcast_miss_packet_action=SAI_PACKET_ACTION_DROP)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_broadcast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_broadcast_miss_packet_action"],
                             SAI_PACKET_ACTION_DROP)

            print("Sending broadcast packet on port %d, %s -> %s - will be "
                  "dropped" % (self.send_port, self.src_mac, self.bcast_mac))
            send_packet(self, self.send_port, self.bcast_pkt)
            verify_no_other_packets(self)
            print("\tOK")

            time.sleep(4)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            print("Checking if ARP packes are still forwarded to CPU")
            print("Sending ARP packet on port %d - will be redirected to CPU" %
                  (self.send_port))
            send_packet(self, self.send_port, self.arp_pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)
            print("\nVerificaion complete")

        finally:
            self.dataplane.flush()
            # revert default broadcast packet action
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_broadcast_miss_packet_action=SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_broadcast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_broadcast_miss_packet_action"],
                             SAI_PACKET_ACTION_FORWARD)

    def broadcastMissCopyActionTest(self):
        '''
        Verify if broadcast packets are copied to CPU after setting miss packet action to copy.
        Verify also if ARP packets are still redirected to CPU.
        '''
        print("\nbroadcastMissCopyActionTest()")

        try:
            print("Verifying initial switch behavior")
            print("Sending broadcast packet on port %d, %s -> %s - will flood"
                  % (self.send_port, self.src_mac, self.bcast_mac))
            send_packet(self, self.send_port, self.bcast_pkt)
            verify_packets(self, self.bcast_pkt, self.flood_ports)
            print("\tOK")

            print("Setting broadcast FDB miss packet action to copy")
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_broadcast_miss_packet_action=SAI_PACKET_ACTION_COPY)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_broadcast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_broadcast_miss_packet_action"],
                             SAI_PACKET_ACTION_COPY)

            time.sleep(4)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending broadcast packet on port %d, %s -> %s - will be "
                  "copied to CPU" % (self.send_port, self.src_mac,
                                     self.bcast_mac))
            send_packet(self, self.send_port, self.bcast_pkt)
            verify_packets(self, self.bcast_pkt, self.flood_ports)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)
            print("\tOK")

            time.sleep(4)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            print("Checking if ARP packes are still forwarded to CPU")
            print("Sending ARP packet on port %d - will be redirected to CPU" %
                  (self.send_port))
            send_packet(self, self.send_port, self.arp_pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)
            print("\nVerificaion complete")

        finally:
            self.dataplane.flush()
            # reveret default multicast packet action
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_broadcast_miss_packet_action=SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_broadcast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_broadcast_miss_packet_action"],
                             SAI_PACKET_ACTION_FORWARD)

    def broadcastMissTrapActionTest(self):
        '''
        Verify if broacast packets are redirected to CPU after setting miss
        packet action to trap.
        Verify also if ARP packets are still redirected to CPU.
        '''
        print("\nbroadcastMissTrapActionTest()")

        try:
            print("Verifying initial switch behavior")
            print("Sending broadcast packet on port %d, %s -> %s - will flood"
                  % (self.send_port, self.src_mac, self.bcast_mac))
            send_packet(self, self.send_port, self.bcast_pkt)
            verify_packets(self, self.bcast_pkt, self.flood_ports)
            print("\tOK")

            print("Setting broadcast FDB miss packet action to trap")
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_broadcast_miss_packet_action=SAI_PACKET_ACTION_TRAP)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_broadcast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_broadcast_miss_packet_action"],
                             SAI_PACKET_ACTION_TRAP)

            time.sleep(4)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            print("Sending broadcast packet on port %d, %s -> %s - will be "
                  "redirected to CPU" % (self.send_port, self.src_mac,
                                         self.bcast_mac))
            send_packet(self, self.send_port, self.bcast_pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue0)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)
            print("\tOK")

            time.sleep(4)
            pre_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            print("Checking if ARP packes are still forwarded to CPU")
            print("Sending ARP packet on port %d - will be redirected to CPU" %
                  (self.send_port))
            send_packet(self, self.send_port, self.arp_pkt)
            time.sleep(4)
            post_stats = sai_thrift_get_queue_stats(
                self.client, self.cpu_queue4)
            self.assertEqual(
                post_stats["SAI_QUEUE_STAT_PACKETS"] -
                pre_stats["SAI_QUEUE_STAT_PACKETS"],
                1)
            print("\nVerificaion complete")

        finally:
            self.dataplane.flush()
            # revert default broadcast packet action
            status = sai_thrift_set_switch_attribute(
                self.client,
                fdb_broadcast_miss_packet_action=SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sw_attr = sai_thrift_get_switch_attribute(
                self.client, fdb_broadcast_miss_packet_action=True)
            self.assertEqual(sw_attr["fdb_broadcast_miss_packet_action"],
                             SAI_PACKET_ACTION_FORWARD)


@group("draft")
class FdbEventTest(SaiHelper):
    '''
    Verify correctness of FDB atributes values after events like:
    learning, aging, moving, flushing, deleting
    '''

    def setUp(self):
        super(FdbEventTest, self).setUp()

        self.src_mac = "00:11:11:11:11:11"
        self.dst_mac = "00:22:22:22:22:22"
        vlan_id = 10

        self.pkt = simple_udp_packet(eth_dst=self.dst_mac,
                                     eth_src=self.src_mac,
                                     pktlen=100)
        self.tag_pkt = simple_udp_packet(eth_dst=self.dst_mac,
                                         eth_src=self.src_mac,
                                         dl_vlan_enable=True,
                                         vlan_vid=vlan_id,
                                         pktlen=104)

        self.src_port = self.dev_port0
        self.flood_ports = [[self.dev_port1],
                            [self.dev_port4, self.dev_port5, self.dev_port6]]
        self.flood_pkts = [self.tag_pkt, self.pkt]

        # expected FDB entry
        self.mac_entry = sai_thrift_fdb_entry_t(switch_id=self.switch_id,
                                                mac_address=self.src_mac,
                                                bv_id=self.vlan10)

    def runTest(self):
        self.macLearnEventTest()
        self.macAgeEvenTest()
        self.macMoveEventTest()
        self.macFlushEventTest()
        self.macDeleteEventTest()

    def tearDown(self):
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

        super(FdbEventTest, self).tearDown()

    def macLearnEventTest(self):
        '''
        Verify MAC learning event for FDB entry
        '''
        print("\nmacLearnEventTest()")

        try:
            print("Sending packet on port %d to learn MAC address, %s -> %s - "
                  "will flood" % (self.src_port, self.src_mac, self.dst_mac))
            send_packet(self, self.src_port, self.pkt)
            verify_each_packet_on_multiple_port_lists(
                self, self.flood_pkts, self.flood_ports)
            print("\tOK")
            time.sleep(2)

            print("Verifying FDB attributes")
            fdb_attr = sai_thrift_get_fdb_entry_attribute(self.client,
                                                          self.mac_entry,
                                                          bridge_port_id=True,
                                                          packet_action=True,
                                                          type=True)

            self.assertEqual(fdb_attr["bridge_port_id"], self.port0_bp)
            print("\tbridge_port_id\tOK")
            self.assertEqual(fdb_attr["packet_action"],
                             SAI_PACKET_ACTION_FORWARD)
            print("\tpacket_action\tOK")
            self.assertEqual(fdb_attr["type"], SAI_FDB_ENTRY_TYPE_DYNAMIC)
            print("\ttype\t\tOK")

            print("\tVerification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

    def macAgeEvenTest(self):
        '''
        Verify MAC aging event for FDB entry
        '''
        print("\nmacAgeEvenTest()")

        age_time = 10

        try:
            status = sai_thrift_set_switch_attribute(self.client,
                                                     fdb_aging_time=age_time)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("Aging time set to %d sec for all tests" % age_time)

            print("Sending packet on port %d to learn MAC address, %s -> %s - "
                  "will flood" % (self.src_port, self.src_mac, self.dst_mac))
            send_packet(self, self.src_port, self.pkt)
            verify_each_packet_on_multiple_port_lists(self, self.flood_pkts,
                                                      self.flood_ports)
            print("\tOK")
            time.sleep(2)

            fdb_attr = sai_thrift_get_fdb_entry_attribute(self.client,
                                                          self.mac_entry,
                                                          bridge_port_id=True,
                                                          packet_action=True,
                                                          type=True)

            self.assertEqual(fdb_attr["bridge_port_id"], self.port0_bp)
            self.assertEqual(fdb_attr["packet_action"],
                             SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(fdb_attr["type"], SAI_FDB_ENTRY_TYPE_DYNAMIC)

            print("Waiting until aging interval is gone")
            self.saiWaitFdbAge(age_time)

            print("Verifying FDB attributes")
            fdb_attr = sai_thrift_get_fdb_entry_attribute(self.client,
                                                          self.mac_entry,
                                                          bridge_port_id=True,
                                                          packet_action=True,
                                                          type=True)

            self.assertEqual(self.status(), SAI_STATUS_ITEM_NOT_FOUND)
            print("\tEntry\t\tclear")
            self.assertEqual(fdb_attr, None)
            print("\tAttrs\t\tclear")

            print("\tVerification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            sai_thrift_set_switch_attribute(self.client, fdb_aging_time=0)

    def macMoveEventTest(self):
        '''
        Verify MAC moving event for FDB entry
        '''
        print("\nmacMoveEventTest()")

        mv_port = self.dev_port4
        flood_ports = [[self.dev_port0], [self.dev_port1]]
        flood_pkts = [self.pkt, self.tag_pkt]

        try:
            print("Sending packet on port %d to learn MAC address, %s -> %s - "
                  "will flood" % (self.src_port, self.src_mac, self.dst_mac))
            send_packet(self, self.src_port, self.pkt)
            verify_each_packet_on_multiple_port_lists(
                self, self.flood_pkts, self.flood_ports)
            print("\tOK")
            time.sleep(2)

            fdb_attr = sai_thrift_get_fdb_entry_attribute(self.client,
                                                          self.mac_entry,
                                                          bridge_port_id=True,
                                                          packet_action=True,
                                                          type=True)

            self.assertEqual(fdb_attr["bridge_port_id"], self.port0_bp)
            self.assertEqual(fdb_attr["packet_action"],
                             SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(fdb_attr["type"], SAI_FDB_ENTRY_TYPE_DYNAMIC)

            print("Sending packet on port %d to move MAC address, %s -> %s - "
                  "will flood" % (mv_port, self.src_mac, self.dst_mac))
            send_packet(self, mv_port, self.pkt)
            verify_each_packet_on_multiple_port_lists(
                self, flood_pkts, flood_ports)
            print("\tOK")
            time.sleep(2)

            print("Verifying FDB attributes")
            fdb_attr = sai_thrift_get_fdb_entry_attribute(self.client,
                                                          self.mac_entry,
                                                          bridge_port_id=True,
                                                          packet_action=True,
                                                          type=True)

            self.assertEqual(fdb_attr["bridge_port_id"], self.lag1_bp)
            print("\tbridge_port_id\tOK")
            self.assertEqual(fdb_attr["packet_action"],
                             SAI_PACKET_ACTION_FORWARD)
            print("\tpacket_action\tOK")
            self.assertEqual(fdb_attr["type"], SAI_FDB_ENTRY_TYPE_DYNAMIC)
            print("\ttype\t\tOK")

            print("\tVerification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

    def macFlushEventTest(self):
        '''
        Verify MAC flushing event for FDB entry
        '''
        print("\nmacFlushEventTest()")

        try:
            print("Sending packet on port %d to learn MAC address, %s -> %s - "
                  "will flood" % (self.src_port, self.src_mac, self.dst_mac))
            send_packet(self, self.src_port, self.pkt)
            verify_each_packet_on_multiple_port_lists(
                self, self.flood_pkts, self.flood_ports)
            print("\tOK")
            time.sleep(2)

            fdb_attr = sai_thrift_get_fdb_entry_attribute(self.client,
                                                          self.mac_entry,
                                                          bridge_port_id=True,
                                                          packet_action=True,
                                                          type=True)

            self.assertEqual(fdb_attr["bridge_port_id"], self.port0_bp)
            self.assertEqual(fdb_attr["packet_action"],
                             SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(fdb_attr["type"], SAI_FDB_ENTRY_TYPE_DYNAMIC)

            print("Flush FDB entry")
            sai_thrift_flush_fdb_entries(self.client,
                                         bridge_port_id=self.port0_bp,
                                         entry_type=SAI_FDB_ENTRY_TYPE_DYNAMIC)

            print("Verifying FDB attributes")
            fdb_attr = sai_thrift_get_fdb_entry_attribute(self.client,
                                                          self.mac_entry,
                                                          bridge_port_id=True,
                                                          packet_action=True,
                                                          type=True)

            self.assertEqual(self.status(), SAI_STATUS_ITEM_NOT_FOUND)
            print("\tEntry\t\tclear")
            self.assertEqual(fdb_attr, None)
            print("\tAttrs\t\tclear")

            print("\tVerification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

    def macDeleteEventTest(self):
        '''
        Verify MAC deletion event for FDB entry
        '''
        print("\nmacDeleteEventTest()")

        try:
            print("Sending packet on port %d to learn MAC address, %s -> %s - "
                  "will flood" % (self.src_port, self.src_mac, self.dst_mac))
            send_packet(self, self.src_port, self.pkt)
            verify_each_packet_on_multiple_port_lists(
                self, self.flood_pkts, self.flood_ports)
            print("\tOK")
            time.sleep(2)

            fdb_attr = sai_thrift_get_fdb_entry_attribute(self.client,
                                                          self.mac_entry,
                                                          bridge_port_id=True,
                                                          packet_action=True,
                                                          type=True)

            self.assertEqual(fdb_attr["bridge_port_id"], self.port0_bp)
            self.assertEqual(fdb_attr["packet_action"],
                             SAI_PACKET_ACTION_FORWARD)
            self.assertEqual(fdb_attr["type"], SAI_FDB_ENTRY_TYPE_DYNAMIC)

            print("Delete FDB entry")
            sai_thrift_remove_fdb_entry(self.client, self.mac_entry)

            print("Verifying FDB attributes")
            fdb_attr = sai_thrift_get_fdb_entry_attribute(self.client,
                                                          self.mac_entry,
                                                          bridge_port_id=True,
                                                          packet_action=True,
                                                          type=True)

            self.assertEqual(self.status(), SAI_STATUS_ITEM_NOT_FOUND)
            print("\tEntry\t\tclear")
            self.assertEqual(fdb_attr, None)
            print("\tAttrs\t\tclear")

            print("\tVerification complete")

        finally:
            sai_thrift_flush_fdb_entries(
                self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
