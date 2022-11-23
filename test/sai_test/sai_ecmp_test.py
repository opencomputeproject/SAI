from sai_test_base import T0TestBase
from sai_utils import *


class EcmpHashFieldSportTestV4(T0TestBase):
    """
    Verify loadbalance on NexthopGroup ipv4 by source port.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        
    def test_load_balance_on_sportv4(self):
        """
        1. Generate different packets by updating src port
        2. Send these packets on port1
        3. Check if packets are received on ports of lag1-4 equally
        """
        print("Ecmp l3 load balancing test based on src port")
        max_itrs = 400
        begin_port = 2000
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv4_list[0].member_port_indexs)))
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
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)
        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.8)), "Not all paths are equally balanced")

    def runTest(self):
        self.test_load_balance_on_sportv4()

    def tearDown(self):
        super().tearDown()


class EcmpHashFieldSportTestV6(T0TestBase):
    """
    Verify loadbalance on NexthopGroup ipv6 by source port.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        
    def test_load_balance_on_sportv6(self):
        """
        1. Generate different packets by updating src port
        2. Send these packets on port1
        3. Check if packets are received on ports of lag1-4 equally
        """
        print("Ecmp l3 load balancing test based on src port")
        max_itrs = 400
        begin_port = 2000
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv6_list[0].member_port_indexs)))
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
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)
        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.8)), "Not all paths are equally balanced")

    def runTest(self):
        self.test_load_balance_on_sportv6()

    def tearDown(self):
        super().tearDown()


class EcmpHashFieldDportTestV4(T0TestBase):
    """
    Verify loadbalance on NexthopGroup ipv4 by destination port.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        
    def test_load_balance_on_dportv4(self):
        """
        1. Generate different packets by updating dst port
        2. Send these packets on port1
        3. Check if packets are received on ports of lag1-4 equally
        """
        print("Ecmp l3 load balancing test based on dst port")
        max_itrs = 400
        begin_port = 2000
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv4_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]

        ip_src = self.servers[0][1].ipv4
        ip_dst = self.servers[60][1].ipv4
        for port_index in range(0, max_itrs):
            dst_port = begin_port + port_index
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.servers[1][1].mac,
                                    ip_dst=ip_dst,
                                    ip_src=ip_src,
                                    tcp_dport= dst_port,
                                    ip_id=105,
                                    ip_ttl=64)

            exp_pkt1 = simple_tcp_packet(eth_dst=self.t1_list[1][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         tcp_dport= dst_port,
                                         ip_id=105,
                                         ip_ttl=63)

            exp_pkt2 = simple_tcp_packet(eth_dst=self.t1_list[2][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         tcp_dport= dst_port,
                                         ip_id=105,
                                         ip_ttl=63)

            exp_pkt3 = simple_tcp_packet(eth_dst=self.t1_list[3][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         tcp_dport= dst_port,
                                         ip_id=105,
                                         ip_ttl=63) 

            exp_pkt4 = simple_tcp_packet(eth_dst=self.t1_list[4][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         tcp_dport= dst_port,
                                         ip_id=105,
                                         ip_ttl=63) 
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)
        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.8)), "Not all paths are equally balanced")

    def runTest(self):
        self.test_load_balance_on_dportv4()

    def tearDown(self):
        super().tearDown()


class EcmpHashFieldDportTestV6(T0TestBase):
    """
    Verify loadbalance on NexthopGroup ipv6 by destination port.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        
    def test_load_balance_on_dportv6(self):
        """
        1. Generate different packets by updating dst port
        2. Send these packets on port1
        3. Check if packets are received on ports of lag1-4 equally
        """
        print("Ecmp l3 load balancing test based on dst port")
        max_itrs = 400
        begin_port = 2000
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv6_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]

        ip_src = self.servers[0][1].ipv6
        ip_dst = self.servers[60][1].ipv6
        for port_index in range(0, max_itrs):
            dst_port = begin_port + port_index
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=self.servers[1][1].mac,
                                      ipv6_dst=ip_dst,
                                      ipv6_src=ip_src,
                                      tcp_dport= dst_port,
                                      ipv6_hlim=64)

            exp_pkt1 = simple_tcpv6_packet(eth_dst=self.t1_list[1][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63)

            exp_pkt2 = simple_tcpv6_packet(eth_dst=self.t1_list[2][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63)

            exp_pkt3 = simple_tcpv6_packet(eth_dst=self.t1_list[3][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63) 

            exp_pkt4 = simple_tcpv6_packet(eth_dst=self.t1_list[4][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63) 
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)
        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.8)), "Not all paths are equally balanced")

    def runTest(self):
        self.test_load_balance_on_dportv6()

    def tearDown(self):
        super().tearDown()


class EcmpHashFieldSIPTestV4(T0TestBase):
    """
    Verify loadbalance on NexthopGroup ipv4 by source port.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        
    def test_load_balance_on_sipv4(self):
        """
        1. Generate different packets by updating src ip (192.168.0.1-192.168.0.10)
        2. Send these packets on port1
        3. Check if packets are received on ports of lag1-4 equally
        """
        print("Ecmp l3 load balancing test based on src ip")
        max_itrs = 400
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv4_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]

        ip_dst = self.servers[60][1].ipv4
        for index in range(0, max_itrs):
            ip_index = index % 10
            ip_src = self.servers[0][ip_index+1].ipv4
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.servers[1][1].mac,
                                    ip_dst=ip_dst,
                                    ip_src=ip_src,
                                    ip_id=105,
                                    ip_ttl=64)

            exp_pkt1 = simple_tcp_packet(eth_dst=self.t1_list[1][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63)

            exp_pkt2 = simple_tcp_packet(eth_dst=self.t1_list[2][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63)

            exp_pkt3 = simple_tcp_packet(eth_dst=self.t1_list[3][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63) 

            exp_pkt4 = simple_tcp_packet(eth_dst=self.t1_list[4][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63) 
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)
        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.8)), "Not all paths are equally balanced")

    def runTest(self):
        self.test_load_balance_on_sipv4()

    def tearDown(self):
        super().tearDown()


class EcmpHashFieldSIPTestV6(T0TestBase):
    """
    Verify loadbalance on NexthopGroup ipv6 by source port.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        
    def test_load_balance_on_sipv6(self):
        """
        1. Generate different packets by updating src ip (192.168.0.1-192.168.0.10)
        2. Send these packets on port1
        3. Check if packets are received on ports of lag1-4 equally
        """
        print("Ecmp l3 load balancing test based on src ip")
        max_itrs = 400
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv6_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]

        ip_dst = self.servers[60][1].ipv6
        for index in range(0, max_itrs):
            ip_index = index % 10
            ip_src = self.servers[0][ip_index+1].ipv6
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=self.servers[1][1].mac,
                                      ipv6_dst=ip_dst,
                                      ipv6_src=ip_src,
                                      ipv6_hlim=64)

            exp_pkt1 = simple_tcpv6_packet(eth_dst=self.t1_list[1][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           ipv6_hlim=63)

            exp_pkt2 = simple_tcpv6_packet(eth_dst=self.t1_list[2][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           ipv6_hlim=63)

            exp_pkt3 = simple_tcpv6_packet(eth_dst=self.t1_list[3][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           ipv6_hlim=63) 

            exp_pkt4 = simple_tcpv6_packet(eth_dst=self.t1_list[4][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           ipv6_hlim=63) 
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)
        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.8)), "Not all paths are equally balanced")

    def runTest(self):
        self.test_load_balance_on_sipv6()

    def tearDown(self):
        super().tearDown()


"""
Skip test for broadcom, can't load balance on protocol such as tcp and udp. Item: 15023123
"""

class EcmpHashFieldProtoTestV4(T0TestBase):
    """
    Verify loadbalance on NexthopGroup ipv4 by protocol.
    """
    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                         skip_reason ="SKIP! Skip test for broadcom, can't load balance on protocol such as tcp and udp. Item: 15023123",
                        )
        
    def test_load_balance_on_protocolv4(self):
        """
        1. Generate different packets with tcp and udp
        2. Send these packets on port1
        3. Check if packets are received on ports of lag1-4 equally
        """
        print("Ecmp l3 load balancing test based on protocol")
        max_itrs = 400
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv4_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]

        ip_src = self.servers[0][1].ipv4
        ip_dst = self.servers[60][1].ipv4
        for index in range(0, max_itrs):
            if index % 2 == 0:
                pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                        eth_src=self.servers[1][1].mac,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        ip_id=105,
                                        ip_ttl=64)

                exp_pkt1 = simple_tcp_packet(eth_dst=self.t1_list[1][100].mac,
                                             eth_src=ROUTER_MAC,
                                             ip_dst=ip_dst,
                                             ip_src=ip_src,
                                             ip_id=105,
                                             ip_ttl=63)

                exp_pkt2 = simple_tcp_packet(eth_dst=self.t1_list[2][100].mac,
                                             eth_src=ROUTER_MAC,
                                             ip_dst=ip_dst,
                                             ip_src=ip_src,
                                             ip_id=105,
                                             ip_ttl=63)

                exp_pkt3 = simple_tcp_packet(eth_dst=self.t1_list[3][100].mac,
                                             eth_src=ROUTER_MAC,
                                             ip_dst=ip_dst,
                                             ip_src=ip_src,
                                             ip_id=105,
                                             ip_ttl=63) 

                exp_pkt4 = simple_tcp_packet(eth_dst=self.t1_list[4][100].mac,
                                             eth_src=ROUTER_MAC,
                                             ip_dst=ip_dst,
                                             ip_src=ip_src,
                                             ip_id=105,
                                             ip_ttl=63) 
            else:
                pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                        eth_src=self.servers[1][1].mac,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        ip_id=105,
                                        ip_ttl=64)

                exp_pkt1 = simple_udp_packet(eth_dst=self.t1_list[1][100].mac,
                                             eth_src=ROUTER_MAC,
                                             ip_dst=ip_dst,
                                             ip_src=ip_src,
                                             ip_id=105,
                                             ip_ttl=63)

                exp_pkt2 = simple_udp_packet(eth_dst=self.t1_list[2][100].mac,
                                             eth_src=ROUTER_MAC,
                                             ip_dst=ip_dst,
                                             ip_src=ip_src,
                                             ip_id=105,
                                             ip_ttl=63)

                exp_pkt3 = simple_udp_packet(eth_dst=self.t1_list[3][100].mac,
                                             eth_src=ROUTER_MAC,
                                             ip_dst=ip_dst,
                                             ip_src=ip_src,
                                             ip_id=105,
                                             ip_ttl=63) 

                exp_pkt4 = simple_udp_packet(eth_dst=self.t1_list[4][100].mac,
                                             eth_src=ROUTER_MAC,
                                             ip_dst=ip_dst,
                                             ip_src=ip_src,
                                             ip_id=105,
                                             ip_ttl=63) 
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)
        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.8)), "Not all paths are equally balanced")

    def runTest(self):
        self.test_load_balance_on_protocolv4()

    def tearDown(self):
        super().tearDown()


"""
Skip test for broadcom, can't load balance on protocol such as tcp and udp. Item: 15023123
"""

class EcmpHashFieldProtoTestV6(T0TestBase):
    """
    Verify loadbalance on NexthopGroup ipv6 by protocol.
    """
    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                         skip_reason ="SKIP! Skip test for broadcom, can't load balance on protocol such as tcp and udp. Item: 15023123",
                        )
        
    def test_load_balance_on_protocolv6(self):
        """
        1. Generate different packets with tcp and udp
        2. Send these packets on port1
        3. Check if packets are received on ports of lag1-4 equally
        """
        print("Ecmp l3 load balancing test based on protocol")
        max_itrs = 400
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv6_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]

        ip_src = self.servers[0][1].ipv6
        ip_dst = self.servers[60][1].ipv6
        for index in range(0, max_itrs):
            if index % 2 == 0:
                pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src=self.servers[1][1].mac,
                                          ipv6_dst=ip_dst,
                                          ipv6_src=ip_src,
                                          ipv6_hlim=64)

                exp_pkt1 = simple_tcpv6_packet(eth_dst=self.t1_list[1][100].mac,
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=ip_dst,
                                               ipv6_src=ip_src,
                                               ipv6_hlim=63)

                exp_pkt2 = simple_tcpv6_packet(eth_dst=self.t1_list[2][100].mac,
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=ip_dst,
                                               ipv6_src=ip_src,
                                               ipv6_hlim=63)

                exp_pkt3 = simple_tcpv6_packet(eth_dst=self.t1_list[3][100].mac,
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=ip_dst,
                                               ipv6_src=ip_src,
                                               ipv6_hlim=63)

                exp_pkt4 = simple_tcpv6_packet(eth_dst=self.t1_list[4][100].mac,
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=ip_dst,
                                               ipv6_src=ip_src,
                                               ipv6_hlim=63)
            else:
                pkt = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                          eth_src=self.servers[1][1].mac,
                                          ipv6_dst=ip_dst,
                                          ipv6_src=ip_src,
                                          ipv6_hlim=64)

                exp_pkt1 = simple_udpv6_packet(eth_dst=self.t1_list[1][100].mac,
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=ip_dst,
                                               ipv6_src=ip_src,
                                               ipv6_hlim=63)

                exp_pkt2 = simple_udpv6_packet(eth_dst=self.t1_list[2][100].mac,
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=ip_dst,
                                               ipv6_src=ip_src,
                                               ipv6_hlim=63)

                exp_pkt3 = simple_udpv6_packet(eth_dst=self.t1_list[3][100].mac,
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=ip_dst,
                                               ipv6_src=ip_src,
                                               ipv6_hlim=63)

                exp_pkt4 = simple_udpv6_packet(eth_dst=self.t1_list[4][100].mac,
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=ip_dst,
                                               ipv6_src=ip_src,
                                               ipv6_hlim=63)
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)
        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.8)), "Not all paths are equally balanced")

    def runTest(self):
        self.test_load_balance_on_protocolv6()

    def tearDown(self):
        super().tearDown()


class IngressNoDiffTestV4(T0TestBase):
    """
    Verify if different ingress ports will not impact the loadbalance (not change to other egress ports).
    """
    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        
    def test_ingress_no_diff(self):
        """
        1. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:192.168.60.1`` to match the exiting config
        2. Check vlan interface(svi added)
        3. Send packets from Port5 - Port8
        4. Verify packet received on a certain LAG's member, with corresponding SIP, DIP, and ``SMAC: SWITCH_MAC``
        5. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:192.168.60.2``
        6. Send packets from Port5 - Port8
        7. Verify packet received on a certain LAG's member but different from step4
        """
        print("Ecmp l3 ingress no diff test")

        ip_src = self.servers[0][1].ipv4
        ip_dst1 = self.servers[60][1].ipv4
        ip_dst2 = self.servers[60][2].ipv4

        pkt1 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                 eth_src=self.servers[1][1].mac,
                                 ip_dst=ip_dst1,
                                 ip_src=ip_src,
                                 ip_id=105,
                                 ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(eth_dst=self.t1_list[3][100].mac,
                                     eth_src=ROUTER_MAC,
                                     ip_dst=ip_dst1,
                                     ip_src=ip_src,
                                     ip_id=105,
                                     ip_ttl=63)

        pkt2 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                 eth_src=self.servers[1][1].mac,
                                 ip_dst=ip_dst2,
                                 ip_src=ip_src,
                                 ip_id=105,
                                 ip_ttl=64)
        exp_pkt2 = simple_tcp_packet(eth_dst=self.t1_list[2][100].mac,
                                     eth_src=ROUTER_MAC,
                                     ip_dst=ip_dst2,
                                     ip_src=ip_src,
                                     ip_id=105,
                                     ip_ttl=63)
        
        exp_port_idx1 = -1
        exp_port_idx2 = -1
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv4_list[0].member_port_indexs)))
        
        for i in range(5, 9):
            # step 4
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[i].dev_port_index, pkt1)
            if exp_port_idx1 == -1:
                exp_port_idx1, _ = verify_packet_any_port(self, exp_pkt1, recv_dev_port_idxs)
            else:
                verify_packet(self, exp_pkt1, recv_dev_port_idxs[exp_port_idx1])

            # step 6
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[i].dev_port_index, pkt2)
            if exp_port_idx2 == -1:
                exp_port_idx2, _ = verify_packet_any_port(self, exp_pkt2, recv_dev_port_idxs)
            else:
                verify_packet(self, exp_pkt2, recv_dev_port_idxs[exp_port_idx2])
            
            # step 7
            self.assertTrue(exp_port_idx1 != exp_port_idx2, "Recived packets from the same port")
    
    def runTest(self):
        self.test_ingress_no_diff()

    def tearDown(self):
        super().tearDown()


class RemoveLagEcmpTestV4(T0TestBase):
    """
    When remove nexthop member, we expect traffic drop on the removed nexthop member.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        self.route_configer.remove_nhop_member_by_lag_idx(
            nhp_grp_obj=self.dut.nhp_grpv4_list[0], lag_idx=3)

    def test_lag_ecmp_remove(self):
        """
        1. Remove the next hop from next-hop group in test_ecmp: next-hop with IP ``DIP:10.1.3.100`` on LAG3 
        2. Generate Packets, with different source IPs as ``SIP:192.168.0.1-192.168.0.10`` 
        3. Change other elements in the packets, including ``DIP:192.168.60.1`` ``L4_port``
        4. Verify Packets only can be received on LAG1 and LAG2, with ``SMAC: SWITCH_MAC_2`` (check loadbalanced in LAG and ECMP)
        """
        print("Ecmp remove lag test")

        ip_dst = self.servers[60][1].ipv4
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv4_list[0].member_port_indexs)))
        pkts_num = 10
        for index in range(pkts_num):
            ip_src = self.servers[0][index + 1].ipv4
            
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.servers[1][1].mac,
                                    ip_dst=ip_dst,
                                    ip_src=ip_src,
                                    ip_id=105,
                                    ip_ttl=64)
            
            exp_pkt1 = simple_tcp_packet(eth_dst=self.t1_list[1][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63)
            exp_pkt2 = simple_tcp_packet(eth_dst=self.t1_list[2][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63)
            exp_pkt3 = simple_tcp_packet(eth_dst=self.t1_list[4][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63)
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2, exp_pkt3], recv_dev_port_idxs)

    def runTest(self):
        self.test_lag_ecmp_remove()

    def tearDown(self):
        self.route_configer.create_nhop_member_by_lag_port_idxs(
            nhp_grp_obj=self.dut.nhp_grpv4_list[0], lag_idx=3)
        super().tearDown()

class RemoveLagEcmpTestV6(T0TestBase):
    """
    When remove nexthop member, we expect traffic drop on the removed nexthop member.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        self.route_configer.remove_nhop_member_by_lag_idx(
            nhp_grp_obj=self.dut.nhp_grpv6_list[0], lag_idx=3)

    def test_lag_ecmp_remove_v6(self):
        """
        1. Remove the next hop from next-hop group in test_ecmp: next-hop on LAG3 
        2. Generate Packets, with different source IPs
        3. Change other elements in the packets
        4. Verify Packets only can be received on LAG1 and LAG2, with ``SMAC: SWITCH_MAC_2`` (check loadbalanced in LAG and ECMP)
        """
        print("Ecmp remove lag test")
        max_itrs = 50
        begin_port = 2000
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv6_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]

        ip_src = self.servers[0][1].ipv6
        ip_dst = self.servers[60][1].ipv6
        for port_index in range(0, max_itrs):
            dst_port = begin_port + port_index
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=self.servers[1][1].mac,
                                      ipv6_dst=ip_dst,
                                      ipv6_src=ip_src,
                                      tcp_dport= dst_port,
                                      ipv6_hlim=64)

            exp_pkt1 = simple_tcpv6_packet(eth_dst=self.t1_list[1][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63)

            exp_pkt2 = simple_tcpv6_packet(eth_dst=self.t1_list[2][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63)

            exp_pkt3 = simple_tcpv6_packet(eth_dst=self.t1_list[4][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63) 
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)

    def runTest(self):
        self.test_lag_ecmp_remove_v6()

    def tearDown(self):
        self.route_configer.create_nhop_member_by_lag_port_idxs(
            nhp_grp_obj=self.dut.nhp_grpv6_list[0], lag_idx=3)
        super().tearDown()

class RemoveAllNextHopMemeberTestV4(T0TestBase):
    """
    When remove all of nexthop group members, we expect traffic drop.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        
    def test_nexthopgroup_remove(self):
        """
        1. Remove all next hops from next-hop group in test_ecmp
        2. Generate Packets
        3. Verify no Packets  can be received on LAG1, LAG2 and LAG3
        """
        print("Ecmp remove nexthop group test")

        for nhp_member in self.dut.nhp_grpv4_list[0].nhp_grp_members:
            sai_thrift_remove_next_hop_group_member(self.client, nhp_member)
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        ip_src = self.servers[0][1].ipv4
        ip_dst = self.servers[60][1].ipv4
        src_port = 1000 
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
        self.dataplane.flush()
        send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
        verify_no_other_packets(self)

            

    def runTest(self):
        self.test_nexthopgroup_remove()

    def tearDown(self):
        super().tearDown()

class RemoveNexthopGroupTestV4(T0TestBase):
    """
    When remove nexthop group, we expect traffic drop.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        
    def test_nexthopgroup_remove(self):
        """
        1. Remove all next hops from next-hop group in test_ecmp
        2. Remove nexthop group v4
        3. Generate Packets
        4. Verify no Packets  can be received on LAG1, LAG2 and LAG3
        """
        print("Ecmp remove nexthop group test")

        for nhp_member in self.dut.nhp_grpv4_list[0].nhp_grp_members:
            sai_thrift_remove_next_hop_group_member(self.client, nhp_member)
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        sai_thrift_remove_next_hop_group(self.client, self.dut.nhp_grpv4_list[0].nhp_grp_id)

        ip_src = self.servers[0][1].ipv4
        ip_dst = self.servers[60][1].ipv4
        src_port = 1000 
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
        self.dataplane.flush()
        send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
        verify_no_other_packets(self)

            

    def runTest(self):
        self.test_nexthopgroup_remove()

    def tearDown(self):
        super().tearDown()

class ReAddLagEcmpTestV4(T0TestBase):
    """
    When readd nexthop member, we expect traffic received on the readded nexthop member.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        self.route_configer.remove_nhop_member_by_lag_idx(
            nhp_grp_obj=self.dut.nhp_grpv4_list[0], lag_idx=3)

    def test_lag_ecmp_readd(self):
        """
       Remove lag3 nexthop member run steps in test_ecmp
       add already existing next hop on IP 10.1.2.100 for LAG3 to the next-hop group in test_ecmp
       Generate Packets, with different source IP SIP:192.168.0.1-192.168.0.10
       Change other elements in the packets, including DIP:192.168.60.1 L4_port
       Verify Packets can be received on LAG1 LAG2 and LAG3, LAG4, with SMAC: SWITCH_MAC_2 (check loadbalanced in LAG and ECMP)

        """
        print("Ecmp readd lag test")

        ip_dst = self.servers[60][1].ipv4
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv4_list[0].member_port_indexs)))
        pkts_num = 10
        for index in range(pkts_num):
            ip_src = self.servers[0][index + 1].ipv4
            
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.servers[1][1].mac,
                                    ip_dst=ip_dst,
                                    ip_src=ip_src,
                                    ip_id=105,
                                    ip_ttl=64)
            
            exp_pkt1 = simple_tcp_packet(eth_dst=self.t1_list[1][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63)
            exp_pkt2 = simple_tcp_packet(eth_dst=self.t1_list[2][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63)
            exp_pkt3 = simple_tcp_packet(eth_dst=self.t1_list[4][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63)
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2, exp_pkt3], recv_dev_port_idxs)

        self.route_configer.create_nhop_member_by_lag_port_idxs(
            nhp_grp_obj=self.dut.nhp_grpv4_list[0], lag_idx=3)
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv4_list[0].member_port_indexs)))
        for index in range(pkts_num):
            ip_src = self.servers[0][index + 1].ipv4
            
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.servers[1][1].mac,
                                    ip_dst=ip_dst,
                                    ip_src=ip_src,
                                    ip_id=105,
                                    ip_ttl=64)
            
            exp_pkt1 = simple_tcp_packet(eth_dst=self.t1_list[1][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63)
            exp_pkt2 = simple_tcp_packet(eth_dst=self.t1_list[2][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63)
            exp_pkt3 = simple_tcp_packet(eth_dst=self.t1_list[3][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63)
            exp_pkt4 = simple_tcp_packet(eth_dst=self.t1_list[4][100].mac,
                                         eth_src=ROUTER_MAC,
                                         ip_dst=ip_dst,
                                         ip_src=ip_src,
                                         ip_id=105,
                                         ip_ttl=63)
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
    def runTest(self):
        self.test_lag_ecmp_readd()

    def tearDown(self):
        super().tearDown()

class ReAddLagEcmpTestV6(T0TestBase):
    """
    When readd nexthop member, we expect traffic received on the readded nexthop member.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        self.route_configer.remove_nhop_member_by_lag_idx(
            nhp_grp_obj=self.dut.nhp_grpv6_list[0], lag_idx=3)
        
    def test_lag_ecmp_readdv6(self):

        max_itrs = 50
        begin_port = 2000
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv6_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]

        ip_src = self.servers[0][1].ipv6
        ip_dst = self.servers[60][1].ipv6
        for port_index in range(0, max_itrs):
            dst_port = begin_port + port_index
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=self.servers[1][1].mac,
                                      ipv6_dst=ip_dst,
                                      ipv6_src=ip_src,
                                      tcp_dport= dst_port,
                                      ipv6_hlim=64)

            exp_pkt1 = simple_tcpv6_packet(eth_dst=self.t1_list[1][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63)

            exp_pkt2 = simple_tcpv6_packet(eth_dst=self.t1_list[2][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63)

            exp_pkt3 = simple_tcpv6_packet(eth_dst=self.t1_list[4][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63) 
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1
        print(rcv_count)

        
        self.route_configer.create_nhop_member_by_lag_port_idxs(
            nhp_grp_obj=self.dut.nhp_grpv6_list[0], lag_idx=3)

        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv6_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]
        ip_src = self.servers[0][1].ipv6
        ip_dst = self.servers[60][1].ipv6
        for port_index in range(0, max_itrs):
            dst_port = begin_port + port_index
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=self.servers[1][1].mac,
                                      ipv6_dst=ip_dst,
                                      ipv6_src=ip_src,
                                      tcp_dport= dst_port,
                                      ipv6_hlim=64)

            exp_pkt1 = simple_tcpv6_packet(eth_dst=self.t1_list[1][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63)

            exp_pkt2 = simple_tcpv6_packet(eth_dst=self.t1_list[2][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63)

            exp_pkt3 = simple_tcpv6_packet(eth_dst=self.t1_list[3][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63) 

            exp_pkt4 = simple_tcpv6_packet(eth_dst=self.t1_list[4][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63) 
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)

    def runTest(self):
        self.test_lag_ecmp_readdv6()

    def tearDown(self):
        super().tearDown()
        
class EcmpLagDisableTestV4(T0TestBase):
    """
      Verify traffic drop on lag1 when ecmp lag1 member egress is disable.
    """
    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        
    def test_lag_ecmp_disable(self):
        """
        Disable LAG1 members(member attribute)
        Generate Packets, with different source IPs as SIP:192.168.0.1-192.168.0.10
        Change other elements in the packets, including DIP:192.168.60.1 L4_port
        Verify Packets no packet lost and only can be received on LAG2 and LAG3, LAG4, with SMAC: SWITCH_MAC_2 (check loadbalanced in LAG and ECMP)
        """
         # disable egress of lag member: port18, 17
        print("disable port17,18 egress")
        status = sai_thrift_set_lag_member_attribute(self.client,
                                                         self.dut.lag_list[0].lag_members[1],
                                                         egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_lag_member_attribute(self.client,
                                                         self.dut.lag_list[0].lag_members[0],
                                                         egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        max_itrs = 10
        begin_port = 2000
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv4_list[0].member_port_indexs)))
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

            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            verify_no_packet_any(self, exp_pkt1, self.dut.lag_list[0].member_port_indexs)

        
    def runTest(self):
        self.test_lag_ecmp_disable()

    def tearDown(self):
        status = sai_thrift_set_lag_member_attribute(self.client,
                                                      self.dut.lag_list[0].lag_members[0],
                                                     egress_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_lag_member_attribute(self.client,
                                                     self.dut.lag_list[0].lag_members[1],
                                                     egress_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        super().tearDown()

class EcmpLagDisableTestV6(T0TestBase):
    """
    Verify traffic drop on lag1 when ecmp lag1 member egress is disable
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        # disable egress of lag member: port18, 17
        print("disable port17,18 egress")
        status = sai_thrift_set_lag_member_attribute(self.client,
                                                         self.dut.lag_list[0].lag_members[1],
                                                         egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_lag_member_attribute(self.client,
                                                         self.dut.lag_list[0].lag_members[0],
                                                         egress_disable=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        
    def test_lag_ecmp_disablev6(self):
        """
        Disable LAG1 members(member attribute)
        Generate Packets, with different source IPs 
        Change other elements in the packets
        Verify Packets no packet lost and only can be received on LAG2 and LAG3, LAG4, with SMAC: SWITCH_MAC_2 (check loadbalanced in LAG and ECMP)
        """
        max_itrs = 10
        begin_port = 2000

        ip_src = self.servers[0][1].ipv6
        ip_dst = self.servers[60][1].ipv6
        for port_index in range(0, max_itrs):
            dst_port = begin_port + port_index
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=self.servers[1][1].mac,
                                      ipv6_dst=ip_dst,
                                      ipv6_src=ip_src,
                                      tcp_dport= dst_port,
                                      ipv6_hlim=64)

            exp_pkt1 = simple_tcpv6_packet(eth_dst=self.t1_list[1][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63)
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            verify_no_packet_any(self, exp_pkt1, self.dut.lag_list[0].member_port_indexs)

    def runTest(self):
        self.test_lag_ecmp_disablev6()

    def tearDown(self):
        status = sai_thrift_set_lag_member_attribute(self.client,
                                                      self.dut.lag_list[0].lag_members[0],
                                                     egress_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_lag_member_attribute(self.client,
                                                     self.dut.lag_list[0].lag_members[1],
                                                     egress_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        super().tearDown()


class EcmpIngressDisableTestV4(T0TestBase):
    """
    Verify ingress RIF disable.
    """
    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )
        print("Disable vlan 10  ingress RIF")
        sai_thrift_set_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], admin_v4_state=False)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        time.sleep(3)

    def test_ecmp_ingress_disable_v4(self):
        """
        Generate Packets, with SIP:192.168.0.1 DIP:192.168.60.1
        Check vlan interface(svi added)
        Disable the ingress for Port1 related RIF (VLAN Interface)
        Send packest from Port1
        Verify packet drop
        """
        max_itrs = 10
        begin_port = 2000
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv4_list[0].member_port_indexs)))
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

            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            verify_no_packet_any(self, exp_pkt1, self.dut.lag_list[0].member_port_indexs)

        
    def runTest(self):
        self.test_ecmp_ingress_disable_v4()

    def tearDown(self):
        status = sai_thrift_set_lag_member_attribute(self.client,
                                                      self.dut.lag_list[0].lag_members[0],
                                                     egress_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_lag_member_attribute(self.client,
                                                     self.dut.lag_list[0].lag_members[1],
                                                     egress_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        super().tearDown()

class EcmpIngressDisableTestV6(T0TestBase):
    """
    Verify ingress RIF disable
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )

        print("Disable vlan 10  ingress RIF")
        sai_thrift_set_router_interface_attribute(
            self.client, self.dut.vlans[10].rif_list[0], admin_v6_state=False)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        time.sleep(3)
        
    def test_ecmp_ingress_disable_v6(self):
        """
        Generate Packets, with SIP:192.168.0.1 DIP:192.168.60.1
        Check vlan interface(svi added)
        Disable the ingress for Port1 related RIF (VLAN Interface)
        Send packest from Port1
        Verify packet drop
        """
        max_itrs = 10
        begin_port = 2000

        ip_src = self.servers[0][1].ipv6
        ip_dst = self.servers[60][1].ipv6
        for port_index in range(0, max_itrs):
            dst_port = begin_port + port_index
            pkt = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=self.servers[1][1].mac,
                                      ipv6_dst=ip_dst,
                                      ipv6_src=ip_src,
                                      tcp_dport= dst_port,
                                      ipv6_hlim=64)

            exp_pkt1 = simple_tcpv6_packet(eth_dst=self.t1_list[1][100].mac,
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=ip_dst,
                                           ipv6_src=ip_src,
                                           tcp_dport= dst_port,
                                           ipv6_hlim=63)
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            verify_no_packet_any(self, exp_pkt1, self.dut.lag_list[0].member_port_indexs)

    def runTest(self):
        self.test_ecmp_ingress_disable_v6()

    def tearDown(self):
        status = sai_thrift_set_lag_member_attribute(self.client,
                                                      self.dut.lag_list[0].lag_members[0],
                                                     egress_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_lag_member_attribute(self.client,
                                                     self.dut.lag_list[0].lag_members[1],
                                                     egress_disable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        super().tearDown()


class EcmpBaseTestV4(T0TestBase):
    """
    Verify with in two layers (leaf and spine) if different hash offset in LAG can take effect for the loadbalncing.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )

    def setupLagSeed(self, seed=200):
        """
        Sets Lag seed

        Args:
            seed (int): seed value
        """
        sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_seed=seed)
    
    def setupECMPSeed(self, seed=200):
        """
        Sets ECMP seed

        Args:
            seed (int): seed value
        """
        sai_thrift_set_switch_attribute(
            self.client, ecmp_default_hash_seed=seed)

    def sendAndVerifyPacket(self):
        max_itrs = 400
        begin_port = 2000
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv4_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]
        packet_rcvport = []
        packet_rcvgroup = []

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
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            packet_rcvport.append(rcv_idx)
            packet_rcvgroup.append(round(rcv_idx/2))
            rcv_count[rcv_idx] += 1
        
        print(rcv_count)
        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.8)), "Not all paths are equally balanced")
        return packet_rcvport,packet_rcvgroup
        
    def tearDown(self):
        super().tearDown()

class EcmpBaseTestV6(T0TestBase):
    """
    Verify with in two layers (leaf and spine) if different hash offset in LAG can take effect for the loadbalncing.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self,
                         is_create_route_for_nhopgrp=True,
                         is_create_route_for_lag=False,
                        )

    def setupLagSeed(self, seed=200):
        """
        Sets Lag seed

        Args:
            seed (int): seed value
        """
        sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_seed=seed)
    
    def setupECMPSeed(self, seed=200):
        """
        Sets ECMP seed

        Args:
            seed (int): seed value
        """
        sai_thrift_set_switch_attribute(
            self.client, ecmp_default_hash_seed=seed)

    def sendAndVerifyPacket(self):
        max_itrs = 400
        begin_port = 3000
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv6_list[0].member_port_indexs)))
        cnt_ports = len(recv_dev_port_idxs)
        rcv_count = [0 for _ in range(cnt_ports)]
        packet_rcvport = []
        packet_rcvgroup= []
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
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)

            packet_rcvgroup.append(round(rcv_idx/2))
            packet_rcvport.append(rcv_idx)
            rcv_count[rcv_idx] += 1
            
        print(rcv_count)

        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.8)), "Not all paths are equally balanced")

        return packet_rcvport,packet_rcvgroup

    def tearDown(self):
        super().tearDown()


class LagTwoLayersWithDiffHashOffsetTestV4(EcmpBaseTestV4):
    """
    Verify with in two layers (leaf and spine) if different hash offset in LAG can take effect for the loadbalncing.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        super().setUp()

    def test_lag_only_two_layers_with_diff_hash_offset_v4(self):
        """
        Check the basic config, make sure there are four lags
        Make each lag contains two members(add extra member with available ports)
        Send 4K packets with different SIP, DIP(192.168.60.0/24), DEST_L4_port,SRC_L4_PORT, PROTO
        Save the 8 ports receive sequence for all those 4K packet
        Send those 4K packet again
        Received 4K packets with the same sequence in step4
        Change the lag hash offset
        Send those 4K packet
        Received 4K packets with the different sequence and those change only happened amoung lag ports within the same lag
        """
        super().setupLagSeed(seed=211)
        time.sleep(3)
        pre_rcv_port,pre_rcv_group = super().sendAndVerifyPacket()
        super().setupLagSeed(seed=411)
        time.sleep(3)
        post_rcv_port,post_rcv_group = super().sendAndVerifyPacket()
        #self.assertTrue(post_rcv_group == pre_rcv_group)
        self.assertFalse(post_rcv_port == pre_rcv_port)

    def runTest(self):
        self.test_lag_only_two_layers_with_diff_hash_offset_v4()

    def tearDown(self):
        super().tearDown()

class LagTwoLayersWithDiffHashOffsetTestV6(EcmpBaseTestV6):
    """
    Verify with in two layers (leaf and spine) if different hash offset in LAG can take effect for the loadbalncing.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        super().setUp()

    def test_lag_only_two_layers_with_diff_hash_offset_v6(self):
        """
        Check the basic config, make sure there are four lags
        Make each lag contains two members(add extra member with available ports)
        Send 4K packets with different SIP, DIP, DEST_L4_port,SRC_L4_PORT, PROTO
        Save the 8 ports receive sequence for all those 4K packet
        Send those 4K packet again
        Received 4K packets with the same sequence in step4
        Change the lag hash offset
        Send those 4K packet
        Received 4K packets with the different sequence and those change only happened amoung lag ports within the same lag
        """
        super().setupLagSeed(seed=211)
        time.sleep(3)
        pre_rcv_port,pre_rcv_group = super().sendAndVerifyPacket()
        super().setupLagSeed(seed=411)
        time.sleep(3)
        post_rcv_port,post_rcv_group = super().sendAndVerifyPacket()
        #self.assertTrue(post_rcv_group == pre_rcv_group)
        self.assertFalse(post_rcv_port == pre_rcv_port)

    def runTest(self):
        self.test_lag_only_two_layers_with_diff_hash_offset_v6()

    def tearDown(self):
        super().tearDown()

class EcmpTwoLayersWithDiffHashOffsetTestV4(EcmpBaseTestV4):
    """
    Verify with in two layers (leaf and spine) if different hash offset in ECMP can take effect for the loadbalncing.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        super().setUp()

    def  test_ecmp_two_layer_with_diff_hash_offset_v4(self):
        """
        Check the basic config, make sure there are four lags
        Make each lag just contains one member(remove extra member from lag)
        Send 4K packets with different SIP, DIP(192.168.60.0/24), DEST_L4_port,SRC_L4_PORT, PROTO
        Save the four ports receive sequence for all those 4K packet
        Send those 4K packet again
        Received 4K packets with the same sequence in step4
        Change the ecmp hash offset
        Send those 4K packet
        Received 4K packets with the different sequence (range for hash collision)
        """
        super().setupECMPSeed(seed=211)
        time.sleep(3)
        pre_rcv_port,pre_rcv_group = super().sendAndVerifyPacket()
        super().setupECMPSeed(seed=411)
        time.sleep(3)
        post_rcv_port,post_rcv_group = super().sendAndVerifyPacket()
        self.assertFalse(post_rcv_group == pre_rcv_group) 

    def runTest(self):
        self.test_ecmp_two_layer_with_diff_hash_offset_v4()

    def tearDown(self):
        super().tearDown()

class EcmpTwoLayersWithDiffHashOffsetTestV6(EcmpBaseTestV6):
    """
    Verify with in two layers (leaf and spine) if different hash offset in ECMP can take effect for the loadbalncing.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        super().setUp()

    def test_ecmp_only_two_layers_with_diff_hash_offset_v6(self):
        """
        Check the basic config, make sure there are four lags
        Make each lag just contains one member(remove extra member from lag)
        Send 4K packets with different SIP, DIP(192.168.60.0/24), DEST_L4_port,SRC_L4_PORT, PROTO
        Save the four ports receive sequence for all those 4K packet
        Send those 4K packet again
        Received 4K packets with the same sequence in step4
        Change the ecmp hash offset
        Send those 4K packet
        Received 4K packets with the different sequence (range for hash collision)
        """
        super().setupECMPSeed(seed=211)
        time.sleep(3)
        pre_rcv_port,pre_rcv_group = super().sendAndVerifyPacket()
        super().setupECMPSeed(seed=411)
        time.sleep(3)
        post_rcv_port,post_rcv_group = super().sendAndVerifyPacket()
        self.assertFalse(post_rcv_group == pre_rcv_group)
        
    def runTest(self):
        self.test_ecmp_only_two_layers_with_diff_hash_offset_v6()

    def tearDown(self):
        super().tearDown()

class EcmpLagTwoLayersWithDiffHashOffsetTestV4(EcmpBaseTestV4):
    """
    Verify with in two layers (leaf and spine) if different hash offset in ECMP and Lag can take effect for the loadbalncing.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        super().setUp()

    def  test_ecmp_lag_two_layer_with_diff_hash_offset_v4(self):
        """
        Check the basic config, make sure there are four lags
        Make each lag just contains one member(remove extra member from lag)
        Send 4K packets with different SIP, DIP(192.168.60.0/24), DEST_L4_port,SRC_L4_PORT, PROTO
        Save the four ports receive sequence for all those 4K packet
        Send those 4K packet again
        Received 4K packets with the same sequence in step4
        Change the lag hash and ecmp hash offset
        Send those 4K packet
        Received 4K packets with the different sequence (range for hash collision)
        """
        super().setupECMPSeed(seed=211)
        super().setupLagSeed(seed=211)
        time.sleep(3)
        pre_rcv_port,pre_rcv_group = super().sendAndVerifyPacket()
        super().setupECMPSeed(seed=411)
        super().setupLagSeed(seed=411)
        time.sleep(3)
        post_rcv_port,post_rcv_group = super().sendAndVerifyPacket()
        self.assertFalse(post_rcv_group == pre_rcv_group) 
        self.assertFalse(post_rcv_port == pre_rcv_port)

    def runTest(self):
        self.test_ecmp_lag_two_layer_with_diff_hash_offset_v4()

    def tearDown(self):
        super().tearDown()

class EcmpLagTwoLayersWithDiffHashOffsetTestV6(EcmpBaseTestV6):
    """
    Verify with in two layers (leaf and spine) if different hash offset in ECMP and Lag can take effect for the loadbalncing.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        super().setUp()

    def test_ecmp_lag_two_layers_with_diff_hash_offset_v6(self):
        """
        Check the basic config, make sure there are four lags
        Make each lag just contains one member(remove extra member from lag)
        Send 4K packets with different SIP, DIP(192.168.60.0/24), DEST_L4_port,SRC_L4_PORT, PROTO
        Save the four ports receive sequence for all those 4K packet
        Send those 4K packet again
        Received 4K packets with the same sequence in step4
        Change the lag hash and ecmp hash offset
        Send those 4K packet
        Received 4K packets with the different sequence (range for hash collision)
        """
        super().setupECMPSeed(seed=211)
        super().setupLagSeed(seed=211)
        time.sleep(3)
        pre_rcv_port,pre_rcv_group = super().sendAndVerifyPacket()
        super().setupECMPSeed(seed=411)
        super().setupLagSeed(seed=411)
        time.sleep(3)
        post_rcv_port,post_rcv_group = super().sendAndVerifyPacket()
        self.assertFalse(post_rcv_group == pre_rcv_group)
        self.assertFalse(post_rcv_port == pre_rcv_port)

    def runTest(self):
        self.test_ecmp_lag_two_layers_with_diff_hash_offset_v6()

    def tearDown(self):
        super().tearDown()

class EcmpCoExistLagRouteV4(T0TestBase):
    """
    Verify loadbalance on NexthopGroup ipv4 by source port.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self)
        nhop_groupv4_id = sai_thrift_create_next_hop_group(self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        for lag_idx in range(lag_num):
            t1_idx = lag_idx + 1
            print("Create nexthop for port with in ip {}/{}".format(self.t1_list[t1_idx][101].ipv4, 24))
            route_configer.create_neighbor_by_rif(rif=self.dut.rif_list[lag_idx],
                                                  nexthop_device=self.t1_list[t1_idx][101].ipv4,
                                                  no_host=False)
            nhopv4_id = sai_thrift_create_next_hop(self.client, 
                                                   ip=sai_ipaddress(self.t1_list[t1_idx][101].ipv4), 
                                                   router_interface_id=self.dut.rif_list[lag_idx], 
                                                   type=SAI_NEXT_HOP_TYPE_IP)
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

            nhp_grpv4_member = sai_thrift_create_next_hop_group_member(
                self.client,
                next_hop_group_id=nhop_groupv4_id,
                next_hop_id=nhopv4_id)
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

    def test_ecmp_coexist_lag_routev4(self):
        """
        1. Generate different packets by updating src port
        2. Send these packets on port1
        3. Check if packets are received on ports of lag1-4 equally
        """
        print("Ecmp l3 load balancing test based on src port")
        max_itrs = 400
        begin_port = 2000
        recv_dev_port_idxs = self.get_dev_port_indexes(
            list(filter(lambda item: item != 1, self.dut.nhp_grpv4_list[0].member_port_indexs)))
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
            self.dataplane.flush()
            send_packet(self, self.dut.port_obj_list[1].dev_port_index, pkt)
            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3, exp_pkt4], recv_dev_port_idxs)
            rcv_count[rcv_idx] += 1

        print(rcv_count)
        for i in range(0, cnt_ports):
            self.assertTrue((rcv_count[i] >= (max_itrs / cnt_ports * 0.8)), "Not all paths are equally balanced")

    def runTest(self):
        self.test_load_balance_on_sportv4()

    def tearDown(self):
        super().tearDown()
