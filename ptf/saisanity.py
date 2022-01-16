# Copyright (c) 2021 Microsoft Open Technologies, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing
#    permissions and limitations under the License.
#
#    Microsoft would like to thank the following companies for their review and
#    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
#    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.

"""
This file contains some Test classes which are used to the basic functionality of the switch.
"""

from sai_thrift.sai_headers import *

from ptf.dataplane import DataPlane

from sai_base_test import *

mac1 = '00:11:11:11:11:11'
mac2 = '00:22:22:22:22:22'
mac3 = '00:33:33:33:33:33'
mac4 = '00:12:12:12:12:13'


class L2TrunkToTrunkVlanTest(PlatformSaiHelper):
    """
    Test for L2 Vlan Trunk to Trunk transport.
    """

    def setUp(self):
        #this process contains the switch_init process
        SaiHelperBase.setUp(self)

        self.create_bridge_ports()

        print("Sending L2 packet port 1 -> port 2 [access vlan=10])")
        self.vlan_id = 10

        mac_action = SAI_PACKET_ACTION_FORWARD

        self.vlan_oid = sai_thrift_create_vlan(self.client, vlan_id=self.vlan_id)
        self.assertNotEqual(self.vlan_oid, 0)
        self.vlan_member1 = sai_thrift_create_vlan_member(
            self.client, vlan_id=self.vlan_oid, bridge_port_id=self.port0_bp, vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_member2 = sai_thrift_create_vlan_member(
            self.client, vlan_id=self.vlan_oid, bridge_port_id=self.port1_bp, vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_port_attribute(self.client, port_oid=self.port0, port_vlan_id=self.vlan_id)
        sai_thrift_set_port_attribute(self.client, port_oid=self.port1, port_vlan_id=self.vlan_id)

        self.fdb_entry1 = sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac1, bv_id=self.vlan_oid)
        self.fdb_entry2 = sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac2, bv_id=self.vlan_oid)

        #need the bridge port inactually
        sai_thrift_create_fdb_entry(self.client, fdb_entry=self.fdb_entry1, bridge_port_id=self.port0_bp, packet_action=mac_action)
        sai_thrift_create_fdb_entry(self.client, fdb_entry=self.fdb_entry2, bridge_port_id=self.port1_bp, packet_action=mac_action)


    def runTest(self):
        pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src=mac4,
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)
        try:
            time.sleep(5)
            send_packet(self, self.dev_port0, pkt)
            verify_packet(self, pkt, self.dev_port1)
        finally:
            pass


    def tearDown(self):
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry1)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry2)

        sai_thrift_set_port_attribute(self.client, port_oid=self.port0, port_vlan_id=1)
        sai_thrift_set_port_attribute(self.client, port_oid=self.port1, port_vlan_id=1)

        sai_thrift_remove_vlan_member(self.client,self.vlan_member1)
        sai_thrift_remove_vlan_member(self.client,self.vlan_member2)
        sai_thrift_remove_vlan(self.client, self.vlan_oid)
        self.remove_bridge_port()
        #TODO resove the error for fdb entry not equals to init one
        SaiHelperBase.tearDown(self)


class L2TrunkToAccessVlanTest(PlatformSaiHelper):
    """
    Test for L2 Vlan Trunk to Access transport.
    """

    def setUp(self):
        #this process contains the switch_init process
        SaiHelperBase.setUp(self)

        self.create_bridge_ports()

        print("Sending L2 packet port 1 -> port 2 [trunk vlan=10])")
        self.vlan_id = 10

        mac_action = SAI_PACKET_ACTION_FORWARD

        self.vlan_oid = sai_thrift_create_vlan(self.client, vlan_id=self.vlan_id)
        self.assertNotEqual(self.vlan_oid, 0)
        self.vlan_member1 = sai_thrift_create_vlan_member(
            self.client, vlan_id=self.vlan_oid, bridge_port_id=self.port0_bp, vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan_member2 = sai_thrift_create_vlan_member(
            self.client, vlan_id=self.vlan_oid, bridge_port_id=self.port1_bp, vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_port_attribute(self.client, port_oid=self.port0, port_vlan_id=self.vlan_id)
        sai_thrift_set_port_attribute(self.client, port_oid=self.port1, port_vlan_id=self.vlan_id)

        self.fdb_entry1 = sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac1, bv_id=self.vlan_oid)
        self.fdb_entry2 = sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac2, bv_id=self.vlan_oid)

        #need the bridge port inactually
        sai_thrift_create_fdb_entry(self.client, fdb_entry=self.fdb_entry1, bridge_port_id=self.port0_bp, packet_action=mac_action)
        sai_thrift_create_fdb_entry(self.client, fdb_entry=self.fdb_entry2, bridge_port_id=self.port1_bp, packet_action=mac_action)


    def runTest(self):
        pkt = simple_udp_packet(eth_dst=mac2,
                                eth_src=mac4,
                                dl_vlan_enable=True,
                                vlan_vid=10,
                                ip_dst='172.16.0.1',
                                ip_id=102,
                                ip_ttl=64)
        exp_pkt = simple_udp_packet(eth_dst=mac2,
                                    eth_src=mac4,
                                    ip_dst='172.16.0.1',
                                    ip_id=102,
                                    ip_ttl=64,
                                    pktlen=96)
        try:
            time.sleep(5)
            send_packet(self, self.dev_port0, pkt)
            verify_packet(self, exp_pkt, self.dev_port1)
        finally:
            pass


    def tearDown(self):
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry1)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry2)

        sai_thrift_set_port_attribute(self.client, port_oid=self.port0, port_vlan_id=1)
        sai_thrift_set_port_attribute(self.client, port_oid=self.port1, port_vlan_id=1)

        sai_thrift_remove_vlan_member(self.client,self.vlan_member1)
        sai_thrift_remove_vlan_member(self.client,self.vlan_member2)
        sai_thrift_remove_vlan(self.client, self.vlan_oid)
        self.remove_bridge_port()
        #TODO resove the error for fdb entry not equals to init one
        SaiHelperBase.tearDown(self)


class L2SanityTest(PlatformSaiHelper):
    """
    Test for L2 trunk and access port access, all ports scanning.
    """

    def gen_mac(self):
         #Gets self.portX objects for all active ports
        for index in range(0, len(self.port_list)):
            mac = "00"
            if index < 9 :
                section = ":" + "0" + str(index+1)
            else:
                section= ":" + str(index+1)
            mac += (section*5)
            setattr(self, 'mac%s' % index, mac)


    def create_vlan_ports(self, vlanid, vlan_oid):
        for index in range(0, len(self.port_list)):
            port_bp = getattr(self, 'port%s_bp' % index)
            if index%2 == 0:
                vlan_member = sai_thrift_create_vlan_member(
                    self.client,
                    vlan_id=self.vlan_oid,
                    bridge_port_id=port_bp,
                    vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            else:
                vlan_member = sai_thrift_create_vlan_member(
                    self.client,
                    vlan_id=self.vlan_oid,
                    bridge_port_id=port_bp,
                    vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

            setattr(self, 'vlan%s_member%s' % (vlanid, index), vlan_member)


    def set_port_vlan(self, vlan_id):
        for index in range(0, len(self.port_list)):
            port_id=getattr(self, 'port%s' % index)
            sai_thrift_set_port_attribute(self.client, port_id, port_vlan_id=vlan_id)


    def create_port_fdb(self, vlan_id, vlan_oid, mac_action):
        for index in range(0, len(self.port_list)):
            mac=getattr(self, 'mac%s' % index)
            port_bp = getattr(self, 'port%s_bp' % index)
            fdb_entry = sai_thrift_fdb_entry_t(
                switch_id=self.switch_id, mac_address=mac, bv_id=vlan_oid)
            sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=port_bp,
                packet_action=mac_action)
            setattr(self, 'fdb_entry%s' % index, fdb_entry)


    def create_pkt(self, vlan_id):
        for index in range(0, len(self.port_list)):
            target_mac = getattr(self, 'mac%s' % index)
            if index%2 == 0:
                pkt = simple_tcp_packet(eth_src=self.src_mac,
                                        eth_dst=target_mac,
                                        ip_dst='172.16.0.1',
                                        ip_id=101,
                                        ip_ttl=64)
            else:
                pkt = simple_tcp_packet(eth_dst=target_mac,
                                        eth_src=self.src_mac,
                                        dl_vlan_enable=True,
                                        vlan_vid=vlan_id,
                                        ip_dst='172.16.0.1',
                                        ip_id=102,
                                        ip_ttl=64)
            setattr(self, 'pkt%s' % index, pkt)

    def create_exp_pkt(self, vlan_id):
        for index in range(0, len(self.port_list)):
            target_mac = getattr(self, 'mac%s' % index)
            if index%2 == 0:
                exp_pkt = getattr(self, 'pkt%s' % index)
            else:
                exp_pkt = simple_tcp_packet(eth_dst=target_mac,
                                        eth_src=self.src_mac,
                                        ip_dst='172.16.0.1',
                                        ip_id=102,
                                        dl_vlan_enable=True,
                                        vlan_vid=vlan_id,
                                        ip_ttl=64)
            setattr(self, 'exp_pkt%s' % index, exp_pkt)


    def setUp(self):
        #Init switch
        SaiHelperBase.setUp(self)

        mac4=  '00:12:12:12:12:13'

        self.vlan_id = 10
        self.gen_mac()
        self.src_mac=mac4
        mac_action = SAI_PACKET_ACTION_FORWARD

        self.src_port = self.port0
        self.dst_port = self.port1

        self.create_bridge_ports()

        # create vlan 10 with ports
        self.vlan_oid = sai_thrift_create_vlan(self.client, vlan_id=self.vlan_id)
        self.assertNotEqual(self.vlan_oid, 0)
        self.create_vlan_ports(self.vlan_id, self.vlan_oid)

        #set port vlan attribute
        self.set_port_vlan(self.vlan_id)

        #set fdb
        self.create_port_fdb(self.vlan_id, self.vlan_oid, mac_action)

        #create send pkt and rcv pkt
        self.create_pkt(self.vlan_id)
        self.create_exp_pkt(self.vlan_id)


    def runTest(self):
        try:
            for index in range(1, len(self.port_list)):
                self.dataplane.flush()
                print("Check port{} forwarding...".format(index))
                target_pkt = getattr(self, 'pkt%s' % index)
                exp_pkt = getattr(self, 'exp_pkt%s' % index)
                send_packet(self, self.dev_port0, target_pkt)
                verify_packet(self, exp_pkt, index)
        finally:
            pass


    def tearDown(self):
        #reset port vlan id
        #?reset to 0 or 1?
        self.reset_port_vlan(1)

        #remove fdb
        self.remove_fdb()

        #remove vlan member
        self.remove_vlan_member(self.vlan_id)

        #remove bridge port
        self.remove_bridge_port()

        sai_thrift_remove_vlan(self.client, self.vlan_oid)
        SaiHelperBase.tearDown(self)

    def reset_port_vlan(self, reset_vlan_id):
        for index in range(0, len(self.port_list)):
            port_id=getattr(self, 'port%s' % index)
            sai_thrift_set_port_attribute(self.client, port_id, port_vlan_id=reset_vlan_id)

    def remove_fdb(self):
        for index in range(0, len(self.port_list)):
            fdb_entry=getattr(self, 'fdb_entry%s' % index)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)


    def remove_vlan_member(self, vlan_id):
        for index in range(0, len(self.port_list)):
            vlan_member=getattr(self, 'vlan%s_member%s' % (vlan_id, index))
            sai_thrift_remove_vlan_member(self.client, vlan_member)


def set_vlan_data(vlan_id=0, ports=None, untagged=None, large_port=0):
    """
    Creates dictionary with vlan data

    Args:
        vlan_id (int): VLAN ID number
        ports (list): ports numbers
        untagged (list): list of untagged ports
        large_port (int): the largest port in vlan

    Return:
        dictionary: vlan_data_dict
    """
    vlan_data_dict = {
        "vlan_id": vlan_id,
        "ports": ports,
        "untagged": untagged,
        "large_port": large_port
    }
    return vlan_data_dict
