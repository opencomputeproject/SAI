from sai_test_base import T0TestBase
from sai_utils import *


class EcmpLagTest(T0TestBase):

    """
    Test load balance of l3 by source IP.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self)

    def runTest(self):
        """
        1. Generate different packets by updating src ip
        2. send these packets on port1
        3. Check if packets are received on ports of lag1 equally
        """
        try:
            print("Lag l3 load balancing test based on src IP")

            max_itrs = 150
            rcv_count = [0, 0, 0, 0]
            begin_port = 2000
            for i in range(1, 10):
                ip_src = '192.168.0.{}'.format(i)
                ip_dst = '192.168.60.{}'.format(i)
                for port_index in range(1, max_itrs):
                     src_port = begin_port + i
                     pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                        eth_src=self.local_server_mac_list[1],
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_sport= src_port,
                                        ip_id=105,
                                        ip_ttl=64)

                     exp_pkt1 = simple_tcp_packet(eth_dst=self.lag1_nb_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_dst=ip_dst,
                                            ip_src=ip_src,
                                            tcp_sport= src_port,
                                            ip_id=105,
                                            ip_ttl=63) 

                     exp_pkt2 = simple_tcp_packet(eth_dst=self.lag2_nb_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_dst=ip_dst,
                                            ip_src=ip_src,
                                            tcp_sport= src_port,
                                            ip_id=105,
                                            ip_ttl=63)   

                     send_packet(self, 1, pkt)
                     rcv_idx, _ = verify_packet_any_port(self, [exp_pkt1, exp_pkt2], [17, 18, 19, 20])
                     print('ip_src={}, rcv_port={}'.format(ip_src, rcv_idx))
                     rcv_count[rcv_idx] += 1

            print(rcv_count)
            for i in range(0, 4):
                self.assertTrue((rcv_count[i] >= ((max_itrs/2) * 0.6)), "Not all paths are equally balanced")
        finally:
            pass
  