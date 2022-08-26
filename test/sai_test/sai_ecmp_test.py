from sai_test_base import T0TestBase
from sai_utils import *


class LoadbalanceOnSrcPortTestV4(T0TestBase):
    """
    Verify loadbalance on NexthopGroup ipv4 by source port.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_create_route_for_nhopgrp=True)
        
    def test_load_balance_on_sportv4(self):
        """
        1. Generate different packets by updating src port
        2. send these packets on port1
        3. Check if packets are received on ports of lag1-4 equally
        """
        print("Ecmp l3 load balancing test based on src port")
        max_itrs = 150
        begin_port = 2000        
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhop_groupv4_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]

        ip_src = self.servers[0][1].ipv4
        ip_dst = self.servers[60][1].ipv4
        for port_index in range(0, max_itrs):
            src_port = begin_port + port_index
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.servers[1][1].mac,
                                    ip_dst=ip_dst,
                                    ip_src=ip_src,
                                    tcp_sport= src_port,
                                    ip_id=105,
                                    ip_ttl=64)

            exp_pkt1 = simple_tcp_packet(eth_dst=self.t1_list[1][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         tcp_sport= src_port,
                                         ip_id=105,
                                         ip_ttl=63)

            exp_pkt2 = simple_tcp_packet(eth_dst=self.t1_list[2][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         tcp_sport= src_port,
                                         ip_id=105,
                                         ip_ttl=63)

            exp_pkt3 = simple_tcp_packet(eth_dst=self.t1_list[3][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         tcp_sport= src_port,
                                         ip_id=105,
                                         ip_ttl=63) 

            exp_pkt4 = simple_tcp_packet(eth_dst=self.t1_list[4][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         tcp_sport= src_port,
                                         ip_id=105,
                                         ip_ttl=63) 

            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx= verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)
        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.6)), "Not all paths are equally balanced")

    def runTest(self):
        self.test_load_balance_on_sportv4()

    def tearDown(self):
        super().tearDown()


class LoadbalanceOnSrcPortTestV6(T0TestBase):
    """
    Verify loadbalance on NexthopGroup ipv6 by source port.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self, is_create_route_for_nhopgrp=True)
        
    def test_load_balance_on_sportv6(self):
        """
        1. Generate different packets by updating src port
        2. send these packets on port1
        3. Check if packets are received on ports of lag1-4 equally
        """
        print("Ecmp l3 load balancing test based on src port")
        max_itrs = 150
        begin_port = 2000
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhop_groupv6_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]

        ip_src = self.servers[0][1].ipv6
        ip_dst = self.servers[60][1].ipv6
        for port_index in range(0, max_itrs):
            src_port = begin_port + port_index
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=self.servers[1][1].mac,
                                      ipv6_dst=ip_dst,
                                      ipv6_src=ip_src,
                                      tcp_sport= src_port,
                                      ipv6_hlim=64)

            exp_pkt1 = simple_tcpv6_packet(eth_dst=self.t1_list[1][100].mac,
                                          eth_src=ROUTER_MAC,
                                          ipv6_dst=ip_dst,
                                          ipv6_src=ip_src,
                                          tcp_sport= src_port,
                                          ipv6_hlim=63)
            
            exp_pkt2 = simple_tcpv6_packet(eth_dst=self.t1_list[2][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_sport= src_port,
                                           ipv6_hlim=63)

            exp_pkt3 = simple_tcpv6_packet(eth_dst=self.t1_list[3][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_sport= src_port,
                                           ipv6_hlim=63)
            
            exp_pkt4 = simple_tcpv6_packet(eth_dst=self.t1_list[4][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_sport= src_port,
                                           ipv6_hlim=63)

            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx= verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)
        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.6)), "Not all paths are equally balanced")

    def runTest(self):
        self.test_load_balance_on_sportv6()

    def tearDown(self):
        super().tearDown()

