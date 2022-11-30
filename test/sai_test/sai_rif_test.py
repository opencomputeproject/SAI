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
    Verify the packet will be dropped if the packet dest mac does not match the mac in the route interface v4
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

    def test_ingress_mac_update(self):
        """
        1. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:10.1.1.100`` ``DMAC:SWITCH_MAC``
        2. Send packet on Port5
        3. Verify packet received on one of the LAG1's member
        4. Set RIF mac to ``MacX``, the RIF related to Port5 VLAN interface
        5. Send packet on Port5 
        6. Verify no packet was received on any LAG1 member
        """
        print("\nmacUpdateTest()")

        new_router_mac = "00:77:66:55:44:44"
        src_dev = self.servers[0][1]
        target_dev = self.t1_list[1][100]
        send_port = self.dut.port_obj_list[5]
        recv_dev_ports = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, target_dev.l3_lag_obj.member_port_indexs)))

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=src_dev.mac,
                                ip_dst=target_dev.ipv4,
                                ip_src=src_dev.ipv4,
                                ip_id=105,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst=target_dev.l3_lag_obj.neighbor_mac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=target_dev.ipv4,
                                    ip_src=src_dev.ipv4,
                                    ip_id=105,
                                    ip_ttl=63)

        send_packet(self, send_port.dev_port_index, pkt)
        verify_packet_any_port(self, exp_pkt, recv_dev_ports)

        print("Updating src_mac_address to %s" % (new_router_mac))
        sai_thrift_set_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], src_mac_address=new_router_mac)
        time.sleep(3)
        attrs = sai_thrift_get_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], src_mac_address=True)
        self.assertEqual(attrs["src_mac_address"], new_router_mac)

        send_packet(self, send_port.dev_port_index, pkt)
        verify_no_other_packets(self, timeout=3)

    def runTest(self):
        self.test_ingress_mac_update()

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_set_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], src_mac_address=ROUTER_MAC)
        time.sleep(3)
        attrs = sai_thrift_get_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], src_mac_address=True)
        self.assertEqual(attrs["src_mac_address"], ROUTER_MAC)
        super().tearDown()


class IngressMacUpdateTestV6(T0TestBase):
    """
    Verify the packet will be dropped if the packet dest mac does not match the mac in the route interface v6
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

    def test_ingress_mac_update(self):
        """
        1. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:10.1.1.100`` ``DMAC:SWITCH_MAC``
        2. Send packet on Port5
        3. Verify packet received on one of the LAG1's member
        4. Set RIF mac to ``MacX``, the RIF related to Port5 VLAN interface
        5. Send packet on Port5 
        6. Verify no packet was received on any LAG1 member
        """
        print("\nmacUpdateTest()")

        new_router_mac = "00:10:10:10:10:10"
        src_dev = self.servers[0][1]
        target_dev = self.t1_list[1][100]
        send_port = self.dut.port_obj_list[5]
        recv_dev_ports = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, target_dev.l3_lag_obj.member_port_indexs)))

        pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                  eth_src=src_dev.mac,
                                  ipv6_dst=target_dev.ipv6,
                                  ipv6_src=src_dev.ipv6,
                                  ipv6_hlim=64)

        exp_pkt = simple_tcpv6_packet(eth_dst=target_dev.l3_lag_obj.neighbor_mac,
                                      eth_src=ROUTER_MAC,
                                      ipv6_dst=target_dev.ipv6,
                                      ipv6_src=src_dev.ipv6,
                                      ipv6_hlim=63)

        send_packet(self, send_port.dev_port_index, pkt)
        verify_packet_any_port(self, exp_pkt, recv_dev_ports)

        print("Updating src_mac_address to %s" % (new_router_mac))
        sai_thrift_set_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], src_mac_address=new_router_mac)
        time.sleep(3)
        attrs = sai_thrift_get_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], src_mac_address=True)
        self.assertEqual(attrs["src_mac_address"], new_router_mac)

        send_packet(self, send_port.dev_port_index, pkt)
        verify_no_other_packets(self, timeout=3)

    def runTest(self):
        self.test_ingress_mac_update()

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_set_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], src_mac_address=ROUTER_MAC)
        time.sleep(3)
        attrs = sai_thrift_get_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], src_mac_address=True)
        self.assertEqual(attrs["src_mac_address"], ROUTER_MAC)
        super().tearDown()


class IngressDisableTestV4(T0TestBase):
    """
    Verify turn off the admin state for RIF v4 
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

    def test_ingress_disable_ipv4(self):
        """
        1. Set the admin state to False on route interface binds to Port5
        2. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:10.1.1.100`` ``DMAC:SWITCH_MAC``
        3. Send packet on Port5
        4. Verify no packet was received on one of the LAG1's member
        """
        print("\ntest_ingress_disable_ipv4()")

        src_dev = self.servers[0][1]
        target_dev = self.t1_list[1][100]
        send_port = self.dut.port_obj_list[5]

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=src_dev.mac,
                                ip_dst=target_dev.ipv4,
                                ip_src=src_dev.ipv4,
                                ip_id=105,
                                ip_ttl=64)

        send_packet(self, send_port.dev_port_index, pkt)
        verify_no_other_packets(self, timeout=3)

    def runTest(self):
        self.test_ingress_disable_ipv4()

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        super().tearDown()


class IngressDisableTestV6(T0TestBase):
    """
    Verify turn off the admin state for RIF v6
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

    def test_ingress_disable_ipv6(self):
        """
        1. Set the admin state to False on route interface binds to Port5
        2. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:10.1.1.100`` ``DMAC:SWITCH_MAC``
        3. Send packet on Port5
        4. Verify no packet was received on one of the LAG1's member
        """
        print("\ntest_ingress_disable_ipv6()")

        src_dev = self.servers[0][1]
        target_dev = self.t1_list[1][100]
        send_port = self.dut.port_obj_list[5]

        pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                  eth_src=src_dev.mac,
                                  ipv6_dst=target_dev.ipv6,
                                  ipv6_src=src_dev.ipv6,
                                  ipv6_hlim=64)

        send_packet(self, send_port.dev_port_index, pkt)
        verify_no_other_packets(self, timeout=3)

    def runTest(self):
        self.test_ingress_disable_ipv6()

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        super().tearDown()


class IngressMtuTestV4(T0TestBase):
    """
    Verify the packet will be dropped if the packet length exceeds the MTU value.
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

    def test_ingress_mtu(self):
        """
        1. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:10.1.1.100`` ``DMAC:SWITCH_MAC``
        2. Send packet on Port5
        3. Verify packet received on one of the LAG1's member
        4. Set RIF MTU to ``200``, the RIF related to Port5 VLAN interface
        5. Send packet on Port5 with length (200 + 14) (extra 14 for IPv4, 14 + 40 for IPv6. Bytes from the floor Ethernet layer, It contains the source and destination MAC Address, And the type of agreement)
        6. Verify packet received on one of the LAG1's member
        7. Send packet on Port5 with length (201 + 14)
        8. Verify no packet was received on any LAG1 member
        """
        print("\ntest_ingress_mtu_v4()")
        print("Max MTU is 200, send pkt size 200, send to port/lag")

        src_dev = self.servers[0][1]
        target_dev = self.t1_list[1][100]
        send_port = self.dut.port_obj_list[5]
        recv_dev_ports = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, target_dev.l3_lag_obj.member_port_indexs)))

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=src_dev.mac,
                                ip_dst=target_dev.ipv4,
                                ip_src=src_dev.ipv4,
                                ip_id=105,
                                ip_ttl=64,
                                pktlen=200 + 14)

        exp_pkt = simple_tcp_packet(eth_dst=target_dev.l3_lag_obj.neighbor_mac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=target_dev.ipv4,
                                    ip_src=src_dev.ipv4,
                                    ip_id=105,
                                    ip_ttl=63,
                                    pktlen=200 + 14)

        send_packet(self, send_port.dev_port_index, pkt)
        verify_packet_any_port(self, exp_pkt, recv_dev_ports)

        print("set MTU to 200 for Port5 VLAN interface")
        self.mtu_Vlan10_rif = sai_thrift_get_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], mtu=True)
        sai_thrift_set_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], mtu=200)

        print("Max MTU is 200, send pkt size 201, dropped")
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=src_dev.mac,
                                ip_dst=target_dev.ipv4,
                                ip_src=src_dev.ipv4,
                                ip_id=105,
                                ip_ttl=64,
                                pktlen=201 + 14)

        exp_pkt = simple_tcp_packet(eth_dst=target_dev.l3_lag_obj.neighbor_mac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=target_dev.ipv4,
                                    ip_src=src_dev.ipv4,
                                    ip_id=105,
                                    ip_ttl=63,
                                    pktlen=201 + 14)

        send_packet(self, send_port.dev_port_index, pkt)
        verify_no_other_packets(self)
            

    def runTest(self):
        self.test_ingress_mtu()

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_set_router_interface_attribute(
                self.client, self.dut.vlans[10].rif_list[0], mtu=self.mtu_Vlan10_rif['mtu'])
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        super().tearDown()


class IngressMtuTestV6(T0TestBase):
    """
    Verify the packet will be dropped if the packet length exceeds the MTU value.
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

    def test_ingress_mtu_v6(self):
        """
        1. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:10.1.1.100`` ``DMAC:SWITCH_MAC``
        2. Send packet on Port5
        3. Verify packet received on one of the LAG1's member
        4. Set RIF MTU to ``200``, the RIF related to Port5 VLAN interface
        5. Send packet on Port5 with length (200 + 14) (extra 14 for IPv4, 14 + 40 for IPv6. Bytes from the floor Ethernet layer, It contains the source and destination MAC Address, And the type of agreement)
        6. Verify packet received on one of the LAG1's member
        7. Send packet on Port5 with length (201 + 14)
        8. Verify no packet was received on any LAG1 member
        """
        print("\ntest_ingress_mtu_v6()")
        print("Max MTU is 200, send pkt size 200, send to port/lag")

        src_dev = self.servers[0][1]
        target_dev = self.t1_list[1][100]
        send_port = self.dut.port_obj_list[5]
        recv_dev_ports = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, target_dev.l3_lag_obj.member_port_indexs)))

        pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_dev.mac,
                                    ipv6_dst=target_dev.ipv6,
                                    ipv6_src=src_dev.ipv6,
                                    ipv6_hlim=64,
                                    pktlen=200 + 14 + 40)

        exp_pkt = simple_tcpv6_packet(eth_dst=target_dev.l3_lag_obj.neighbor_mac,
                                        eth_src=ROUTER_MAC,
                                        ipv6_dst=target_dev.ipv6,
                                        ipv6_src=src_dev.ipv6,
                                        ipv6_hlim=63,
                                        pktlen=200 + 14 + 40)

        send_packet(self, send_port.dev_port_index, pkt)
        verify_packet_any_port(self, exp_pkt, recv_dev_ports)

        print("set MTU to 200 for Port5 VLAN interface")
        self.mtu_port10_rif = sai_thrift_get_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], mtu=True)
        sai_thrift_set_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], mtu=200)

        print("Max MTU is 200, send pkt size 201, dropped")
        pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_dev.mac,
                                    ipv6_dst=target_dev.ipv6,
                                    ipv6_src=src_dev.ipv6,
                                    ipv6_hlim=64,
                                    pktlen=201 + 14)

        exp_pkt = simple_tcpv6_packet(eth_dst=target_dev.l3_lag_obj.neighbor_mac,
                                        eth_src=ROUTER_MAC,
                                        ipv6_dst=target_dev.ipv6,
                                        ipv6_src=src_dev.ipv6,
                                        ipv6_hlim=63,
                                        pktlen=201 + 14)

        send_packet(self, send_port.dev_port_index, pkt)
        verify_no_other_packets(self)

    def runTest(self):
        self.test_ingress_mtu_v6()

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_set_router_interface_attribute(
                self.client, self.dut.vlans[10].rif_list[0], mtu=self.mtu_port10_rif['mtu'])
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        super().tearDown()
