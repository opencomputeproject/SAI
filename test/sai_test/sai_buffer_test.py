
from sai_test_base import T0TestBase
from multiprocessing import Process
from sai_thrift.sai_headers import *
from ptf import config
from ptf.testutils import *
from ptf.thriftutils import *
from sai_utils import *
import pdb


class BufferStatistics(T0TestBase):
    """
    Test buffer pool and ingress priority group statictics.
    To observe the proper counting of buffer statistics,
    it is recommended to run the test on hardware.
    """

    def setUp(self):
        super().setUp()

        self.tx_cnt = 10000
        self.pkt_len = 700
        self.reserved_buf_size = 1400
        self.buf_size = self.reserved_buf_size * 1000
        self.sleep_time = 2
        self.pkts = []

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC, eth_src=self.servers[1][0].mac,  ip_dst=self.servers[11][0].ipv4, ip_src=self.servers[1][0].ipv4, ip_id=105, ip_ttl=64)
        send_packet(self, self.dut.dev_port_list[1], pkt)
        for i in range(8):
            self.pkt = simple_udp_packet(eth_dst=ROUTER_MAC, eth_src=self.servers[1][0].mac,  ip_dst=self.servers[11][0].ipv4, ip_src=self.servers[1][0].ipv4, ip_id=105, ip_ttl=64,
                pktlen=self.pkt_len - 4, ip_dscp=i)  # account for 4B FCS
            self.pkts.append(self.pkt)

        self.ingr_pool = sai_thrift_create_buffer_pool(
            self.client, type=SAI_BUFFER_POOL_TYPE_INGRESS, size=self.buf_size)
        self.assertGreater(self.ingr_pool, 0)

        self.buffer_profile = sai_thrift_create_buffer_profile(
            self.client, pool_id=self.ingr_pool,
            reserved_buffer_size=self.reserved_buf_size,
            shared_static_th=self.reserved_buf_size,
            threshold_mode=SAI_BUFFER_PROFILE_THRESHOLD_MODE_STATIC)
        self.assertGreater(self.buffer_profile, 0)

        sw_attrs = sai_thrift_get_switch_attribute(
            self.client, ingress_buffer_pool_num=True,
            egress_buffer_pool_num=True,
            total_buffer_size=True)
        ingr_pool_num = sw_attrs["ingress_buffer_pool_num"]
        egr_pool_num = sw_attrs["egress_buffer_pool_num"]
        

        pg_list = sai_thrift_object_list_t(count=100)
        ipg_list = sai_thrift_get_port_attribute(self.client, port_oid=self.dut.port_list[1], ingress_priority_group_list=pg_list)
        self.ipg_list = ipg_list['ingress_priority_group_list'].idlist
        self.ipgs = []

        for i in range(8):
            self.ipg_idx = i
            # self.ipg = sai_thrift_create_ingress_priority_group(
            #     self.client, port=self.dut.port_list[1], index=self.ipg_idx,
            #     buffer_profile=self.buffer_profile)
            ipg = self.ipg_list[self.ipg_idx]
            self.assertGreater(ipg, 0)
            status = sai_thrift_set_ingress_priority_group_attribute(self.client, ingress_priority_group_oid=ipg, buffer_profile=self.buffer_profile)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            print("Assign profile for PG index:{} oid:{}".format(i, ipg))
            self.ipgs.append(ipg)

        # Configure QoS maps.
        dscp_to_tc = []
        tc_to_ipg = []
        prio_to_ipg = []
        tc_to_q = []

        for i in range(8):
            dscp_to_tc.append(
                sai_thrift_qos_map_t(
                    key=sai_thrift_qos_map_params_t(dscp=i),
                    value=sai_thrift_qos_map_params_t(tc=i)))

        for i in range(8):
            tc_to_ipg.append(
                sai_thrift_qos_map_t(
                    key=sai_thrift_qos_map_params_t(tc=i),
                    value=sai_thrift_qos_map_params_t(pg=i)))

            # prio_to_ipg.append(
            #     sai_thrift_qos_map_t(
            #         key=sai_thrift_qos_map_params_t(prio=i),
            #         value=sai_thrift_qos_map_params_t(pg=i)))

        for i in range(egr_pool_num):
            tc_to_q.append(
                sai_thrift_qos_map_t(
                    key=sai_thrift_qos_map_params_t(tc=i),
                    value=sai_thrift_qos_map_params_t(queue_index=i)))

        # prio_to_pg = sai_thrift_qos_map_t(
        #     key=sai_thrift_qos_map_params_t(prio=0),
        #     value=sai_thrift_qos_map_params_t(pg=self.ipg_idx))

        # qos_map_list = sai_thrift_qos_map_list_t(maplist=[prio_to_pg], count=1)

        print("Create DSCP to TC map")
        qos_map_list = sai_thrift_qos_map_list_t(
            maplist=dscp_to_tc, count=len(dscp_to_tc))

        self.dscp_to_tc_map = sai_thrift_create_qos_map(
            self.client, type=SAI_QOS_MAP_TYPE_DSCP_TO_TC,
            map_to_value_list=qos_map_list)
        self.assertGreater(self.dscp_to_tc_map, 0)

        status = sai_thrift_set_port_attribute(
            self.client, self.dut.port_list[1], qos_dscp_to_tc_map=self.dscp_to_tc_map)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        print("Create TC to iCoS map")
        qos_map_list = sai_thrift_qos_map_list_t(
            maplist=tc_to_ipg, count=len(tc_to_ipg))

        self.tc_to_ipg_map = sai_thrift_create_qos_map(
            self.client, type=SAI_QOS_MAP_TYPE_TC_TO_PRIORITY_GROUP,
            map_to_value_list=qos_map_list)
        self.assertGreater(self.tc_to_ipg_map, 0)

        status = sai_thrift_set_port_attribute(
            self.client, self.dut.port_list[1],
            qos_tc_to_priority_group_map=self.tc_to_ipg_map)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        #set all the port as lossless port by enable the FLOW_CONTROL as mention in broadcom doc
        #those attribute relate to the pfc_enable in PORT_QOS_MAP
        # the rule is, pfc_enable is 3,4, then we use the SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL as 24 11000
        # if the pfc_enable is 2,3,4,6, then we use SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL as 92 1011100
        # there we enable all the PG as lossless
        # the range is -128 <= number <= 127
        print("Set all PGs as lossless")
        status = sai_thrift_set_port_attribute(
            self.client, self.dut.port_list[1],
            priority_flow_control=-127)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # status = sai_thrift_set_port_attribute(
        #     self.client, self.dut.port_list[1],
        #     priority_flow_control_tx=-127)
        # self.assertEqual(status, SAI_STATUS_SUCCESS)
        # status = sai_thrift_set_port_attribute(
        #     self.client, self.dut.port_list[1],
        #     priority_flow_control_rx=-127)
        # self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("OK")

        print("Disable port tx")
        status = sai_thrift_set_port_attribute(self.client, self.dut.port_list[18], pkt_tx_enable=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        


        # self.qos_map = sai_thrift_create_qos_map(
        #     self.client, type=SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_PRIORITY_GROUP,
        #     map_to_value_list=qos_map_list)
        # self.assertGreater(self.qos_map, 0)

        # status = sai_thrift_set_port_attribute(
        #     self.client, self.dut.port_list[1],
        #     qos_pfc_priority_to_priority_group_map=self.qos_map)
        # self.assertEqual(status, SAI_STATUS_SUCCESS)

    def sendTraffic(self):
        """
        Send traffic.
        """
        print()
        print("Send {} pkts, pkt size: {} B".format(self.tx_cnt, self.pkt_len))
        for _ in range(self.tx_cnt):
            send_packet(self, self.dut.dev_port_list[1], self.pkts[0])
        print()

    def sendVerify(self, expected_drops, verify_reserved_buffer_size):
        """
        Send traffic in parallel while polling for current occupancy stats.
        Once traffic is sent verifies other stats.

        Args:
            expected_drops (int): Number of expected dropped packets.
            verify_reserved_buffer_size (bool): Whether to verify
                SAI_BUFFER_PROFILE_ATTR_RESERVED_BUFFER_SIZE.
        """

        bp_curr_occupancy_bytes = 0
        ipg_curr_occupancy_bytes = 0
        ipg_shared_curr_occupancy_bytes = 0

        traffic = Process(target=self.sendTraffic)

        traffic.start()

        while traffic.is_alive():
            stats = sai_thrift_get_buffer_pool_stats(
                self.client, self.ingr_pool)

            if (stats["SAI_BUFFER_POOL_STAT_CURR_OCCUPANCY_BYTES"]
                    > bp_curr_occupancy_bytes):
                bp_curr_occupancy_bytes = stats["SAI_BUFFER_POOL_STAT_"
                                                "CURR_OCCUPANCY_BYTES"]           
                
            stats = sai_thrift_get_ingress_priority_group_stats(
                self.client, self.ipgs[0])

            if (stats["SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES"]
                    > ipg_curr_occupancy_bytes):
                ipg_curr_occupancy_bytes = stats["SAI_INGRESS_PRIORITY_GROUP_"
                                                "STAT_CURR_OCCUPANCY_BYTES"]

            if (stats["SAI_INGRESS_PRIORITY_GROUP_STAT_"
                    "SHARED_CURR_OCCUPANCY_BYTES"]
                    > ipg_shared_curr_occupancy_bytes):
                ipg_shared_curr_occupancy_bytes = \
                    stats["SAI_INGRESS_PRIORITY_GROUP_STAT_"
                        "SHARED_CURR_OCCUPANCY_BYTES"]
                

        traffic.join()

        time.sleep(self.sleep_time)

        stats = sai_thrift_get_buffer_pool_stats(self.client, self.ingr_pool)

        if verify_reserved_buffer_size:
            expected_watermark = self.reserved_buf_size
        else:
            expected_watermark = self.pkt_len

        print("SAI_BUFFER_POOL_STAT_CURR_OCCUPANCY_BYTES (max measured)",
              bp_curr_occupancy_bytes)
        print("SAI_BUFFER_POOL_STAT_WATERMARK_BYTES",
              stats["SAI_BUFFER_POOL_STAT_WATERMARK_BYTES"])

        #self.assertGreater(bp_curr_occupancy_bytes, 0)
        # self.assertGreaterEqual(
        #     stats["SAI_BUFFER_POOL_STAT_WATERMARK_BYTES"], expected_watermark)

        #accross all the pgs
        print("Ckeck all the pG stats for the SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS")
        index = 0
        for ipg in self.ipgs:
            stats = sai_thrift_get_ingress_priority_group_stats(self.client, ipg)
            for key in stats:
                print("pg index: {} key:{} value:{} ".format(index, key, stats[key]))
            index = index + 1

        #pdb.set_trace()

        stats = sai_thrift_get_ingress_priority_group_stats(
            self.client, self.ipgs[0])
        print("SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES "
              "(max measured)", ipg_curr_occupancy_bytes)
        print("SAI_INGRESS_PRIORITY_GROUP_STAT_SHARED_CURR_OCCUPANCY_BYTES "
              "(max measured)", ipg_shared_curr_occupancy_bytes)
        print("SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES",
              stats["SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES"])
        print("SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS",
              stats["SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS"])
        print("SAI_INGRESS_PRIORITY_GROUP_STAT_BYTES",
              stats["SAI_INGRESS_PRIORITY_GROUP_STAT_BYTES"])
        print("SAI_INGRESS_PRIORITY_GROUP_STAT_DROPPED_PACKETS",
              stats["SAI_INGRESS_PRIORITY_GROUP_STAT_DROPPED_PACKETS"])

        # self.assertGreater(ipg_curr_occupancy_bytes, 0)
        # self.assertGreater(ipg_shared_curr_occupancy_bytes, 0)
        # self.assertGreaterEqual(
        #     stats["SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES"],
        #     expected_watermark)
        # self.assertEqual(
        #     stats["SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS"], self.tx_cnt)
        # self.assertEqual(
        #     stats["SAI_INGRESS_PRIORITY_GROUP_STAT_BYTES"],
        #     self.tx_cnt * self.pkt_len)
        # self.assertEqual(
        #     stats["SAI_INGRESS_PRIORITY_GROUP_STAT_DROPPED_PACKETS"],
        #     expected_drops)

    def clearVerify(self):
        """
        Clear bufer pool and ingress priority group stats.
        Verify they are cleared.
        """
        print()
        print("Clear bufer pool and ingress priority group stats")

        # status = sai_thrift_clear_buffer_pool_stats(
        #     self.client, self.ingr_pool)
        # self.assertEqual(status, SAI_STATUS_SUCCESS)

        # status = sai_thrift_clear_ingress_priority_group_stats(
        #     self.client, self.ipg)
        # self.assertEqual(status, SAI_STATUS_SUCCESS)

        print("Get stats and verify they are cleared")

        stats = sai_thrift_get_buffer_pool_stats(self.client, self.ingr_pool)

        self.assertEqual(
            stats["SAI_BUFFER_POOL_STAT_WATERMARK_BYTES"], 0)

        stats = sai_thrift_get_ingress_priority_group_stats(
            self.client, self.ipgs[0])

        self.assertEqual(
            stats["SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS"], 0)
        self.assertEqual(
            stats["SAI_INGRESS_PRIORITY_GROUP_STAT_BYTES"], 0)
        self.assertEqual(
            stats["SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES"], 0)
        self.assertEqual(
            stats["SAI_INGRESS_PRIORITY_GROUP_STAT_DROPPED_PACKETS"], 0)

    def runTest(self):
        print()

        # Make sure test starts with cleared counters.
        # status = sai_thrift_clear_buffer_pool_stats(
        #     self.client, self.ingr_pool)
        # self.assertEqual(status, SAI_STATUS_SUCCESS)

        # status = sai_thrift_clear_ingress_priority_group_stats(
        #     self.client, self.ipg)
        # self.assertEqual(status, SAI_STATUS_SUCCESS)

        print("Buffer pool size:", self.buf_size)
        print("Buffer profile reserved_buffer_size:", self.reserved_buf_size)
        print("Buffer profile shared_static_th:", self.reserved_buf_size)

        self.sendVerify(expected_drops=0, verify_reserved_buffer_size=False)
        #self.clearVerify()

        self.pkt_len = 1500
        self.pkt = simple_udp_packet(
            pktlen=self.pkt_len - 4)  # account for 4B FCS

        
        pdb.set_trace()

        print()
        print("Send pkts ({} B) larger than reserved buffer ({} B)".format(
            self.pkt_len, self.reserved_buf_size))
        self.sendVerify(
            expected_drops=self.tx_cnt, verify_reserved_buffer_size=True)
        #self.clearVerify()

    def tearDown(self):
        pass
        # sai_thrift_set_port_attribute(
        #     self.client, self.port0, qos_pfc_priority_to_priority_group_map=0)
        # sai_thrift_remove_qos_map(self.client, self.qos_map)
        # sai_thrift_remove_ingress_priority_group(self.client, self.ipg)
        # sai_thrift_remove_buffer_profile(self.client, self.buffer_profile)
        # sai_thrift_remove_buffer_pool(self.client, self.ingr_pool)

        # super(BufferStatistics, self).tearDown()

