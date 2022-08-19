from sai_test_base import T0TestBase
from sai_utils import *

class EcmpLagTestV4(T0TestBase):
    """
    Verify loadbalance on ECMP members.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self)
        
    def test_ecmp_lags(self):
        """
        1. Generate Packets, with different source IP in range SIP:192.168.0.1-192.168.0.10 to match the exiting config
        2. Change other elements in the packets as well, including DIP:192.168.60.1-192.168.60.10 and L4_port
        3. Send packets with different protocols
        4. Verify packets received on different lags and their members
        """
        try:
            print("Lag l3 load balancing test based on src IP")
            self.port1_rif = sai_thrift_create_router_interface(self.client,
                                                            virtual_router_id=self.dut.default_vrf,
                                                            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                                                            port_id=self.dut.port_list[1])
            max_itrs = 150
            rcv_count = [0, 0, 0, 0]
            begin_port = 2000
            for i in range(1, 11):
                ip_src = self.servers[0][i].ipv4
                ip_dst = self.servers[60][i].ipv4
                for port_index in range(0, max_itrs):
                    src_port = begin_port + port_index
                    pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.servers[1][1].mac,
                                    ip_dst=ip_dst,
                                    ip_src=ip_src,
                                    tcp_sport= src_port,
                                    ip_id=105,
                                    ip_ttl=64)

                    exp_pkt1 = simple_tcp_packet(eth_dst=self.t1_list[2][100].mac,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_sport= src_port,
                                        ip_id=105,
                                        ip_ttl=63) 

                    exp_pkt2 = simple_tcp_packet(eth_dst=self.t1_list[3][100].mac,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_sport= src_port,
                                        ip_id=105,
                                        ip_ttl=63)   

                    send_packet(self, 1, pkt)
                    rcv_idx= verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2], [19, 20, 21, 22])
                    rcv_count[rcv_idx] += 1

            print(rcv_count)
            for i in range(0, 4):
                self.assertTrue((rcv_count[i] >= ((max_itrs*10/4.0) * 0.6)), "Not all paths are equally balanced")
        finally:
            pass
    
    def runTest(self):
        self.test_ecmp_lags()

    def tearDown(self):
        sai_thrift_remove_router_interface(self.client, self.port1_rif)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        super().tearDown()


class EcmpHashFieldSrcIPTestV4(T0TestBase):
    """
    Verify ECMP loadbalance with different hash field, source ip in this case.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self)
        
    def test_ecmp_hash_field_src_ip(self):
        """
        1. Generate Packets, with different source IPs as SIP:192.168.0.1-192.168.0.10 to match the exiting config
        2. For each case, set only one of the four ECMP hash fields (exclude SAI_NATIVE_HASH_FIELD_DST_IP)
        3. For each field just change the corresponding field, for example, for SAI_NATIVE_HASH_FIELD_IP_PROTOCOL, use UDP or TCP
        4. Verify packets received on different lags and their members (check loadbalanced in LAG and ECMP)
        5. Change other un-related fields in the packet
        6. Verify packet received on a certain Lag member, not changed
        """
        try:
            print("Lag l3 load balancing test based on src IP")
            self.port1_rif = sai_thrift_create_router_interface(self.client,
                                                            virtual_router_id=self.dut.default_vrf,
                                                            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                                                            port_id=self.dut.port_list[1])
            max_itrs = 150
            rcv_count = [0, 0, 0, 0]
            begin_port = 2000
            for i in range(1, 11):
                ip_dst = self.servers[60][i].ipv4
                for port_index in range(0, max_itrs):
                    src_port = begin_port + port_index
                    pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.servers[1][1].mac,
                                    ip_dst=ip_dst,
                                    tcp_sport= src_port,
                                    ip_id=105,
                                    ip_ttl=64)

                    exp_pkt1 = simple_tcp_packet(eth_dst=self.t1_list[2][100].mac,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=ip_dst,
                                        tcp_sport= src_port,
                                        ip_id=105,
                                        ip_ttl=63) 

                    exp_pkt2 = simple_tcp_packet(eth_dst=self.t1_list[3][100].mac,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=ip_dst,
                                        tcp_sport= src_port,
                                        ip_id=105,
                                        ip_ttl=63)   

                    send_packet(self, 1, pkt)
                    rcv_idx= verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2], [19, 20, 21, 22])
                    rcv_count[rcv_idx] += 1

            print(rcv_count)
            for i in range(0, 4):
                self.assertTrue((rcv_count[i] >= ((max_itrs*10/4.0) * 0.6)), "Not all paths are equally balanced")
        finally:
            pass
    
    def runTest(self):
        self.test_ecmp_hash_field_src_ip()

    def tearDown(self):
        sai_thrift_remove_router_interface(self.client, self.port1_rif)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        super().tearDown()


class EcmpHashFieldDstPortTestV4(T0TestBase):
    """
    Verify ECMP loadbalance with different hash field, destination port in this case.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        T0TestBase.setUp(self)
        
    def test_ecmp_hash_field_dst_port(self):
        """
        1. Generate Packets, with different source IP in range SIP:192.168.0.1-192.168.0.10 to match the exiting config
        2. Change other elements in the packets as well, including DIP:192.168.60.1-192.168.60.10 and L4_port
        3. Send packets with different protocols
        4. Verify packets received on different lags and their members
        """
        try:
            print("Lag l3 load balancing test based on src IP")
            self.port1_rif = sai_thrift_create_router_interface(self.client,
                                                            virtual_router_id=self.dut.default_vrf,
                                                            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                                                            port_id=self.dut.port_list[1])
            max_itrs = 150
            rcv_count = [0, 0, 0, 0]
            begin_port = 2000
            for i in range(1, 11):
                ip_src = self.servers[0][i].ipv4
                ip_dst = self.servers[60][i].ipv4
                for port_index in range(0, max_itrs):
                    src_port = begin_port + port_index
                    pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=self.servers[1][1].mac,
                                    ip_dst=ip_dst,
                                    ip_src=ip_src,
                                    tcp_dport= src_port,
                                    ip_id=105,
                                    ip_ttl=64)

                    exp_pkt1 = simple_tcp_packet(eth_dst=self.t1_list[2][100].mac,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_dport= src_port,
                                        ip_id=105,
                                        ip_ttl=63) 

                    exp_pkt2 = simple_tcp_packet(eth_dst=self.t1_list[3][100].mac,
                                        eth_src=ROUTER_MAC,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_dport= src_port,
                                        ip_id=105,
                                        ip_ttl=63)   

                    send_packet(self, 1, pkt)
                    rcv_idx= verify_any_packet_any_port(self, [exp_pkt1, exp_pkt2], [19, 20, 21, 22])
                    rcv_count[rcv_idx] += 1

            print(rcv_count)
            for i in range(0, 4):
                self.assertTrue((rcv_count[i] >= ((max_itrs*10/4.0) * 0.6)), "Not all paths are equally balanced")
        finally:
            pass
    
    def runTest(self):
        self.test_ecmp_hash_field_dst_port()

    def tearDown(self):
        sai_thrift_remove_router_interface(self.client, self.port1_rif)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        super().tearDown()
