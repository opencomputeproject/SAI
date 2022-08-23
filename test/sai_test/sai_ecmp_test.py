from sai_test_base import T0TestBase
from sai_utils import *

class EcmpLagTestV4(T0TestBase):
    """
    Verify packets received on different lags and their members.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self)
        
    def test_ecmp_lags(self):
        """
        1. Generate Packets, with different source IP in range ``SIP:192.168.0.1-192.168.0.10`` to match the exiting config
        2. Check vlan interface (svi added)
        3. Change other elements in the packets as well, including ``DIP:192.168.60.1-192.168.60.10`` and ``L4_port``
        4. send packets with different protocols
        5. Verify packets received on different lags and their members
        """
        recv_dev_ports = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.lag_list[1].member_port_indexs)))
        for index in range(1, 11):
            ip_src = self.servers[0][index].ipv4
            ip_dst = self.servers[60][index].ipv4
            
            # TCP Protocol
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                            eth_src=self.servers[1][1].mac,
                            ip_dst=ip_dst,
                            ip_src=ip_src,
                            ip_id=105,
                            ip_ttl=64)

            exp_pkt = simple_tcp_packet(eth_dst=self.t1_list[2][100].mac,
                                eth_src=ROUTER_MAC,
                                ip_dst=ip_dst,
                                ip_src=ip_src,
                                ip_id=105,
                                ip_ttl=63)   

            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            verify_any_packet_any_port(self, exp_pkt, recv_dev_ports)

            # UDP Protocol
            pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                            eth_src=self.servers[1][1].mac,
                            ip_dst=ip_dst,
                            ip_src=ip_src,
                            ip_id=105,
                            ip_ttl=64)

            exp_pkt = simple_udp_packet(eth_dst=self.t1_list[2][100].mac,
                                eth_src=ROUTER_MAC,
                                ip_dst=ip_dst,
                                ip_src=ip_src,
                                ip_id=105,
                                ip_ttl=63)   

            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            verify_any_packet_any_port(self, exp_pkt, recv_dev_ports)

    def runTest(self):
        self.test_ecmp_lags()

    def tearDown(self):
        super().tearDown()


class EcmpLagTestV6(T0TestBase):
    """
    Verify packets received on different lags and their members.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self)
        
    def test_ecmp_lags(self):
        """
        1. Generate Packets, with different source IP in range ``SIP:192.168.0.1-192.168.0.10`` to match the exiting config
        2. Check vlan interface (svi added)
        3. Change other elements in the packets as well, including ``DIP:192.168.60.1-192.168.60.10`` and ``L4_port``
        4. send packets with different protocols
        5. Verify packets received on different lags and their members
        """
        recv_dev_ports = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.lag_list[1].member_port_indexs)))
        for index in range(1, 11):
            ip_src = self.servers[0][index].ipv6
            ip_dst = self.servers[60][index].ipv6
            
            # TCP Protocol
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                            eth_src=self.servers[1][1].mac,
                            ipv6_dst=ip_dst,
                            ipv6_src=ip_src,
                            ipv6_hlim=64)

            exp_pkt = simple_tcpv6_packet(eth_dst=self.t1_list[2][100].mac,
                                eth_src=ROUTER_MAC,
                                ipv6_dst=ip_dst,
                                ipv6_src=ip_src,
                                ipv6_hlim=63)   

            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            verify_any_packet_any_port(self, exp_pkt, recv_dev_ports)

            # UDP Protocol
            pkt = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                            eth_src=self.servers[1][1].mac,
                            ipv6_dst=ip_dst,
                            ipv6_src=ip_src,
                            ipv6_hlim=64)

            exp_pkt = simple_udpv6_packet(eth_dst=self.t1_list[2][100].mac,
                                eth_src=ROUTER_MAC,
                                ipv6_dst=ip_dst,
                                ipv6_src=ip_src,
                                ipv6_hlim=63)   

            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            verify_any_packet_any_port(self, exp_pkt, recv_dev_ports)

    def runTest(self):
        self.test_ecmp_lags()

    def tearDown(self):
        super().tearDown()

