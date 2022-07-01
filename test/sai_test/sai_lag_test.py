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

from sai_test_base import T0TestBase
from sai_utils import *

class LagLoadBalanceTest(T0TestBase):
    """
    Verify the load-balance of l3
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        T0TestBase.setUp(self)

    def load_balance_on_src_ip(self):
        router_mac = '00:77:66:55:44:00'
        ip_src1 = '192.168.0.1'
        ip_src2 = '192.168.0.2'
        ip_dst = '10.10.10.1'
        pkt1 = simple_tcp_packet(eth_dst=router_mac,
                                 eth_src='00:22:22:22:22:22',
                                 ip_dst=ip_dst,
                                 ip_src=ip_src1,
                                 ip_id=105,
                                 ip_ttl=64)
        pkt2 = simple_tcp_packet(eth_dst=router_mac,
                                 eth_src='00:22:22:22:22:22',
                                 ip_dst=ip_dst,
                                 ip_src=ip_src2,
                                 ip_id=105,
                                 ip_ttl=64)
        exp_pkt1 = simple_tcp_packet(eth_dst='02:04:02:01:01:01',
                                    eth_src=router_mac,
                                    ip_dst=ip_dst,
                                    ip_src=ip_src1,
                                    ip_id=105,
                                    ip_ttl=63)
        exp_pkt2 = simple_tcp_packet(eth_dst='02:04:02:01:01:01',
                                    eth_src=router_mac,
                                    ip_dst=ip_dst,
                                    ip_src=ip_src2,
                                    ip_id=105,
                                    ip_ttl=63)
        
        send_packet(self, 21, pkt1)
        verify_packet_any_port(self, exp_pkt1, [17, 18])
        send_packet(self, 21, pkt2)
        verify_packet_any_port(self, exp_pkt2, [17, 18])

    def runTest(self):
        try:
            print("simple lag test")
            self.load_balance_on_src_ip()
        finally:
            pass

class LoadbalanceOnSrcPortTest(T0TestBase):
    """
    Test load balance of l3 by source port.
    """
    def setUp(self):
        T0TestBase.setUp(self)

    def runTest(self):
        try:
            print("Lag l3 load balancing based on src port")
            eth_src = '00:22:22:22:22:22'
            eth_dst = '00:77:66:55:44:00'
            ip_src = '192.168.0.1'
            ip_dst = '10.10.10.1'

            max_itrs = 150
            begin_port = 2000
            rcv_count = [0, 0]
            for i in range(0, max_itrs):
                src_port = begin_port + i
                pkt = simple_tcp_packet(eth_dst=eth_dst,
                                        eth_src=eth_src,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_sport=src_port,
                                        ip_id=105,
                                        ip_ttl=64)
                exp_pkt = simple_tcp_packet(eth_dst='02:04:02:01:01:01',
                                        eth_src=eth_dst,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_sport=src_port,
                                        ip_id=105,
                                        ip_ttl=63)                                                            
                send_packet(self, 21, pkt)
                rcv_idx, _ = verify_packet_any_port(self, exp_pkt, [17, 18])
                print('src_port={}, rcv_port={}'.format(src_port, rcv_idx))
                rcv_count[rcv_idx%17] += 1

            print(rcv_count)
            for i in range(0, 2):
                self.assertTrue((rcv_count[i] >= ((max_itrs/2) * 0.8)), "Not all paths are equally balanced")
        finally:
            pass

class DisableEgressTest(T0TestBase):
    def setUp(self):
        T0TestBase.setUp(self)

    def runTest(self):
        try:
            print("Lag disable egress lag member test")

            eth_src = '00:22:22:22:22:22'
            eth_dst = '00:77:66:55:44:00'
            ip_src = '192.168.0.1'
            ip_dst = '10.10.10.1'

            pkts_num = 10
            begin_port = 2000
            exp_drop = []
            for i in range(0, pkts_num):
                src_port = begin_port + i
                pkt = simple_tcp_packet(eth_dst=eth_dst,
                                        eth_src=eth_src,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_sport=src_port,
                                        ip_id=105,
                                        ip_ttl=64)
                exp_pkt = simple_tcp_packet(eth_dst='02:04:02:01:01:01',
                                        eth_src=eth_dst,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_sport=src_port,
                                        ip_id=105,
                                        ip_ttl=63)
                send_packet(self, 21, pkt)
                rcv_idx, _ = verify_packet_any_port(self, exp_pkt, [17, 18])
                if rcv_idx == 18:
                    exp_drop.append(src_port)
                
            # disable egress of lag member: port18
            print("disable port18 egress")
            status = sai_thrift_set_lag_member_attribute(self.client, self.lag1.lag_members[1], egress_disable=True, ingress_disable=True)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            for i in range(0, pkts_num):
                src_port = begin_port + i
                pkt = simple_tcp_packet(eth_dst=eth_dst,
                                        eth_src=eth_src,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_sport=src_port,
                                        ip_id=105,
                                        ip_ttl=64)
                exp_pkt = simple_tcp_packet(eth_dst='02:04:02:01:01:01',
                                        eth_src=eth_dst,
                                        ip_dst=ip_dst,
                                        ip_src=ip_src,
                                        tcp_sport=src_port,
                                        ip_id=105,
                                        ip_ttl=63)
                send_packet(self, 21, pkt)
                if src_port in exp_drop:
                    # should drop
                    verify_no_packet(self, exp_pkt, 18)
                else:
                    verify_packet(self, exp_pkt, 17)
        finally:
            pass

class IndifferenceIngressPortTest(T0TestBase):
    def setUp(self):
        T0TestBase.setUp(self)

    def runTest(self):
        try:
            eth_src = '00:22:22:22:22:22'
            eth_dst = '00:77:66:55:44:00'
            ip_src = '192.168.0.1'
            ip_dst = '10.10.10.0'

            pkt = simple_tcp_packet(eth_dst=eth_dst,
                                    eth_src=eth_src,
                                    ip_dst=ip_dst,
                                    ip_src=ip_src,
                                    ip_id=105,
                                    ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_dst='02:04:02:01:01:01',
                                    eth_src=eth_dst,
                                    ip_dst=ip_dst,
                                    ip_src=ip_src,
                                    ip_id=105,
                                    ip_ttl=63)

            exp_port = 0
            for i in range(1, 17):
                send_packet(self, i, pkt)
                if exp_port == 0:
                    exp_port, _ = verify_packet_any_port(self, exp_pkt, [17, 18])
                else:
                    verify_packet(self, exp_pkt, exp_port)
        finally:
            pass
