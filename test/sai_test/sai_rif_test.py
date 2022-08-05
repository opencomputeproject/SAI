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
#
#


from unittest import skip
from sai_test_base import T0TestBase
from sai_utils import *


class IngressMacUpdateTest(T0TestBase):
    """
    Verify the packet will be dropped if the packet dest mac does not match the mac in the route interface
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        T0TestBase.setUp(self)

    def test_ingress_mac_update(self):
        """
        Generate Packets, with SIP:192.168.0.1 DIP:10.1.1.101 DMAC:SWITCH_MAC
        Send packet on Port1
        Verify packet received on one of the LAG1's member
        Set RIF mac to MacX, the RIF related to Port1
        Send packet on Port1
        Verify no packet was received on any LAG1 member
        """
        print("\nmacUpdateTest()")

        new_router_mac = "00:77:66:55:44:44"
        ip_dst = self.lag1_route_dst

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                 eth_src=self.local_server_mac_list[1],
                                 ip_dst=ip_dst,
                                 ip_src=self.local_server_ip_list[1],
                                 ip_id=105,
                                 ip_ttl=64)


        exp_pkt = simple_tcp_packet(eth_dst=self.lag1_nb_mac,
                                     eth_src=ROUTER_MAC,
                                     ip_dst=ip_dst,
                                     ip_src=self.local_server_ip_list[1],
                                     ip_id=105,
                                     ip_ttl=63)

        send_packet(self, 1, pkt)
        verify_packet_any_port(self, exp_pkt, [17, 18])

        print("Updating src_mac_address to %s" % (new_router_mac))
        sai_thrift_set_router_interface_attribute(
            self.client, self.port1_rif, src_mac_address=new_router_mac)
        time.sleep(3)
        attrs = sai_thrift_get_router_interface_attribute(
            self.client, self.port1_rif, src_mac_address=True)
        self.assertEqual(attrs["src_mac_address"], new_router_mac)
    
        send_packet(self, 1, pkt)
        verify_no_other_packets(self, timeout=3)
        
    def runTest(self):
        self.test_ingress_mac_update()
 
    def tearDown(self):
        sai_thrift_set_router_interface_attribute(
            self.client, self.port1_rif, src_mac_address=ROUTER_MAC)
        time.sleep(3)
        attrs = sai_thrift_get_router_interface_attribute(
            self.client, self.port1_rif, src_mac_address=True)
        self.assertEqual(attrs["src_mac_address"], ROUTER_MAC)
        super().tearDown()

class IngressDisableTestV4(T0TestBase):
    """
    Verify turn off the admin state for RIF v4 
    """
      
    def setUp(self):
        """
        Test the basic setup process.
        """

        T0TestBase.setUp(self)
        self.port1 = self.port_list[1]

    def test_ingress_disable_ipv4(self):
        """
        Generate Packets, with SIP:192.168.0.1 DIP:10.1.1.101 DMAC:SWITCH_MAC
        Send packet on Port1
        Verify packet received on one of the LAG1's member
        Set RIF mac to MacX, the RIF related to Port1
        Send packet on Port1
        Verify no packet was received on any LAG1 member
        Verifies if IPv4 packets are dropped when admin_v4_state is false
        """

        print("\ntest_ingress_disable_ipv4()")
   
        ip_dst = self.lag1_route_dst

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                 eth_src=self.local_server_mac_list[1],
                                 ip_dst=ip_dst,
                                 ip_src=self.local_server_ip_list[1],
                                 ip_id=105,
                                 ip_ttl=64)


        exp_pkt = simple_tcp_packet(eth_dst=self.lag1_nb_mac,
                                     eth_src=ROUTER_MAC,
                                     ip_dst=ip_dst,
                                     ip_src=self.local_server_ip_list[1],
                                     ip_id=105,
                                     ip_ttl=63)

        send_packet(self, 1, pkt)
        verify_packet_any_port(self, exp_pkt, [17, 18])

        print("Disable IPv4 on ingress RIF")
        sai_thrift_set_router_interface_attribute(
            self.client, self.port1_rif, admin_v4_state=False)
        time.sleep(3)
        send_packet(self, 1, pkt)
        verify_no_other_packets(self, timeout=3)
    

        print("Enable IPv4 on ingress RIF")
        sai_thrift_set_router_interface_attribute(
            self.client, self.port1_rif, admin_v4_state=True)
        time.sleep(3)
        send_packet(self, 1, pkt)
        verify_packet_any_port(self, exp_pkt, [17, 18])

    def runTest(self):
        self.test_ingress_disable_ipv4()
 
    def tearDown(self):
        super().tearDown()


class IngressMtuTestV4(T0TestBase):
    """
    Verify the packet will be dropped if the packet length exceeds the MTU value
    """
      
    def setUp(self):
        """
        Test the basic setup process.
        """

        T0TestBase.setUp(self)

    def test_ingress_mtu(self):
        """
        Generate Packets, with SIP:192.168.0.1 DIP:10.1.1.101 DMAC:SWITCH_MAC
        Send packet on Port1
        Verify packet received on one of the LAG1's member
        Set RIF MTU to 200, the RIF related to Port1
        Send packet on Port1 with length (200 + 14) ( extra 14 for IPv4, 14 + 40 for IPv6. Bytes from the floor Ethernet layer, It contains the source and destination MAC Address, And the type of agreement)
        Verify packet received on one of the LAG1's member
        Send packet on Port1 with length (201 + 14)
        Verify no packet was received on any LAG1 member
        """
        print("\ntest_ingress_mtu_v4()")

        # set MTU to 200 for port1
        mtu_port10_rif = sai_thrift_get_router_interface_attribute(
            self.client, self.port1_rif, mtu=True)

        sai_thrift_set_router_interface_attribute(
            self.client, self.port1_rif, mtu=200)

        try:   
            print("Max MTU is 200, send pkt size 200, send to port/lag")
            ip_dst = self.lag1_route_dst

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.local_server_mac_list[1],
                                    ip_dst=ip_dst,
                                    ip_src=self.local_server_ip_list[1],
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=200 + 14)

            exp_pkt = simple_tcp_packet(eth_dst=self.lag1_nb_mac,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=ip_dst,
                                        ip_src=self.local_server_ip_list[1],
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=200 + 14)

            send_packet(self, 1, pkt)
            verify_packet_any_port(self, exp_pkt, [17, 18])

            print("Max MTU is 200, send pkt size 201, dropped")          
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.local_server_mac_list[1],
                                    ip_dst=ip_dst,
                                    ip_src=self.local_server_ip_list[1],
                                    ip_id=105,
                                    ip_ttl=64,
                                    pktlen=201 + 14)

            exp_pkt = simple_tcp_packet(eth_dst=self.lag1_nb_mac,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=ip_dst,
                                        ip_src=self.local_server_ip_list[1],
                                        ip_id=105,
                                        ip_ttl=63,
                                        pktlen=201 + 14)

            send_packet(self, 1, pkt)
            verify_no_other_packets(self)

    
        finally:
            sai_thrift_set_router_interface_attribute(
                self.client, self.port1_rif, mtu=mtu_port10_rif['mtu'])

    def runTest(self):
        self.test_ingress_mtu()

    def tearDown(self):
        super().tearDown()
        