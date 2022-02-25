# Copyright 2021-present Intel Corporation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Thrift SAI interface Buffer tests
"""
from multiprocessing import Process
from sai_thrift.sai_headers import *
from sai_base_test import *


@group("draft")
class BufferStatistics(MinimalPortVlanConfig):
    """
    Test buffer pool and ingress priority group statictics.
    To observe the proper counting of buffer statistics,
    it is recommended to run the test on hardware.
    """

    def __init__(self):
        super(BufferStatistics, self).__init__(port_num=3)

    def setUp(self):
        super(BufferStatistics, self).setUp()

        self.tx_cnt = 10000
        self.pkt_len = 700
        self.reserved_buf_size = 1400
        self.buf_size = self.reserved_buf_size * 1000
        self.sleep_time = 2
        self.pkt = simple_udp_packet(
            pktlen=self.pkt_len - 4)  # account for 4B FCS

        self.ingr_pool = sai_thrift_create_buffer_pool(
            self.client, type=SAI_BUFFER_POOL_TYPE_INGRESS, size=self.buf_size)
        self.assertGreater(self.ingr_pool, 0)

        self.buffer_profile = sai_thrift_create_buffer_profile(
            self.client, pool_id=self.ingr_pool,
            reserved_buffer_size=self.reserved_buf_size,
            shared_static_th=self.reserved_buf_size,
            threshold_mode=SAI_BUFFER_PROFILE_THRESHOLD_MODE_STATIC)
        self.assertGreater(self.buffer_profile, 0)

        self.ipg_idx = 7
        self.ipg = sai_thrift_create_ingress_priority_group(
            self.client, port=self.port0, index=self.ipg_idx,
            buffer_profile=self.buffer_profile)
        self.assertGreater(self.ipg, 0)

        prio_to_pg = sai_thrift_qos_map_t(
            key=sai_thrift_qos_map_params_t(prio=0),
            value=sai_thrift_qos_map_params_t(pg=self.ipg_idx))

        qos_map_list = sai_thrift_qos_map_list_t(maplist=[prio_to_pg], count=1)

        self.qos_map = sai_thrift_create_qos_map(
            self.client, type=SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_PRIORITY_GROUP,
            map_to_value_list=qos_map_list)
        self.assertGreater(self.qos_map, 0)

        status = sai_thrift_set_port_attribute(
            self.client, self.port0,
            qos_pfc_priority_to_priority_group_map=self.qos_map)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def sendTraffic(self):
        """
        Send traffic.
        """
        print()
        print("Send {} pkts, pkt size: {} B".format(self.tx_cnt, self.pkt_len))
        for _ in range(self.tx_cnt):
            send_packet(self, self.dev_port0, self.pkt)
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
                self.client, self.ipg)

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

        self.assertGreater(bp_curr_occupancy_bytes, 0)
        self.assertGreaterEqual(
            stats["SAI_BUFFER_POOL_STAT_WATERMARK_BYTES"], expected_watermark)

        stats = sai_thrift_get_ingress_priority_group_stats(
            self.client, self.ipg)

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

        self.assertGreater(ipg_curr_occupancy_bytes, 0)
        self.assertGreater(ipg_shared_curr_occupancy_bytes, 0)
        self.assertGreaterEqual(
            stats["SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES"],
            expected_watermark)
        self.assertEqual(
            stats["SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS"], self.tx_cnt)
        self.assertEqual(
            stats["SAI_INGRESS_PRIORITY_GROUP_STAT_BYTES"],
            self.tx_cnt * self.pkt_len)
        self.assertEqual(
            stats["SAI_INGRESS_PRIORITY_GROUP_STAT_DROPPED_PACKETS"],
            expected_drops)

    def clearVerify(self):
        """
        Clear bufer pool and ingress priority group stats.
        Verify they are cleared.
        """
        print()
        print("Clear bufer pool and ingress priority group stats")

        status = sai_thrift_clear_buffer_pool_stats(
            self.client, self.ingr_pool)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        status = sai_thrift_clear_ingress_priority_group_stats(
            self.client, self.ipg)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        print("Get stats and verify they are cleared")

        stats = sai_thrift_get_buffer_pool_stats(self.client, self.ingr_pool)

        self.assertEqual(
            stats["SAI_BUFFER_POOL_STAT_WATERMARK_BYTES"], 0)

        stats = sai_thrift_get_ingress_priority_group_stats(
            self.client, self.ipg)

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
        status = sai_thrift_clear_buffer_pool_stats(
            self.client, self.ingr_pool)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        status = sai_thrift_clear_ingress_priority_group_stats(
            self.client, self.ipg)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        print("Buffer pool size:", self.buf_size)
        print("Buffer profile reserved_buffer_size:", self.reserved_buf_size)
        print("Buffer profile shared_static_th:", self.reserved_buf_size)

        self.sendVerify(expected_drops=0, verify_reserved_buffer_size=False)
        self.clearVerify()

        self.pkt_len = 1500
        self.pkt = simple_udp_packet(
            pktlen=self.pkt_len - 4)  # account for 4B FCS

        print()
        print("Send pkts ({} B) larger than reserved buffer ({} B)".format(
            self.pkt_len, self.reserved_buf_size))
        self.sendVerify(
            expected_drops=self.tx_cnt, verify_reserved_buffer_size=True)
        self.clearVerify()

    def tearDown(self):
        sai_thrift_set_port_attribute(
            self.client, self.port0, qos_pfc_priority_to_priority_group_map=0)
        sai_thrift_remove_qos_map(self.client, self.qos_map)
        sai_thrift_remove_ingress_priority_group(self.client, self.ipg)
        sai_thrift_remove_buffer_profile(self.client, self.buffer_profile)
        sai_thrift_remove_buffer_pool(self.client, self.ingr_pool)

        super(BufferStatistics, self).tearDown()


@group("draft")
class Forwarding(MinimalPortVlanConfig):
    """
    Verify transitioning between different buffer profiles and modyfing buffer
    profile does not break forwarding for ingress priority group.
    To observe the proper counting of buffer statistics,
    it is recommended to run the test on hardware.
    """

    def __init__(self):
        super(Forwarding, self).__init__(port_num=3)

    def setUp(self):
        super(Forwarding, self).setUp()

        self.tx_cnt = 100
        self.pkt_len = 1500
        self.reserved_buf_size = 3200
        self.buf_size = self.reserved_buf_size * 1000
        self.pkt = simple_udp_packet(
            pktlen=self.pkt_len - 4)  # account for 4B FCS

        self.ipg_idx = 0
        self.ipg = sai_thrift_create_ingress_priority_group(
            self.client, port=self.port0, index=self.ipg_idx)
        self.assertGreater(self.ipg, 0)

        prio_to_pg = sai_thrift_qos_map_t(
            key=sai_thrift_qos_map_params_t(prio=0),
            value=sai_thrift_qos_map_params_t(pg=self.ipg_idx))

        qos_map_list = sai_thrift_qos_map_list_t(maplist=[prio_to_pg], count=1)

        self.qos_map = sai_thrift_create_qos_map(
            self.client, type=SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_PRIORITY_GROUP,
            map_to_value_list=qos_map_list)
        self.assertGreater(self.qos_map, 0)

        status = sai_thrift_set_port_attribute(
            self.client, self.port0,
            qos_pfc_priority_to_priority_group_map=self.qos_map)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.ingr_pool = sai_thrift_create_buffer_pool(
            self.client, type=SAI_BUFFER_POOL_TYPE_INGRESS, size=self.buf_size)
        self.assertGreater(self.ingr_pool, 0)

        self.buffer_profile = sai_thrift_create_buffer_profile(
            self.client, pool_id=self.ingr_pool,
            reserved_buffer_size=self.reserved_buf_size, shared_dynamic_th=8,
            threshold_mode=SAI_BUFFER_PROFILE_THRESHOLD_MODE_DYNAMIC)
        self.assertGreater(self.buffer_profile, 0)

    def clearSendVerify(self):
        """
        Clear counters, send traffic, verify counters.
        """

        for i in range(3):
            status = sai_thrift_clear_port_stats(
                self.client, self.port_list[i])
            self.assertEqual(status, SAI_STATUS_SUCCESS)

        status = sai_thrift_clear_ingress_priority_group_stats(
            self.client, self.ipg)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        print("Send {} pkts, pkt size: {} B".format(self.tx_cnt, self.pkt_len))
        for _ in range(self.tx_cnt):
            send_packet(self, self.dev_port0, self.pkt)

        print("Verify traffic\n")
        time.sleep(2)

        stats_ipg = sai_thrift_get_ingress_priority_group_stats(
            self.client, self.ipg)
        stats_p1 = sai_thrift_get_port_stats(self.client, self.port1)
        stats_p2 = sai_thrift_get_port_stats(self.client, self.port2)

        self.assertEqual(
            stats_ipg["SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS"], self.tx_cnt)
        self.assertEqual(
            stats_p1["SAI_PORT_STAT_IF_OUT_UCAST_PKTS"], self.tx_cnt)
        self.assertEqual(
            stats_p2["SAI_PORT_STAT_IF_OUT_UCAST_PKTS"], self.tx_cnt)

    def runTest(self):
        print("\n")
        print("Verify forwarding works with default buffer profile")

        attr = sai_thrift_get_ingress_priority_group_attribute(
            self.client, self.ipg, buffer_profile=True)
        self.assertEqual(attr["buffer_profile"], 0)

        self.clearSendVerify()

        print("Verify forwarding works with custom buffer profile")

        status = sai_thrift_set_ingress_priority_group_attribute(
            self.client, self.ipg, buffer_profile=self.buffer_profile)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        attr = sai_thrift_get_ingress_priority_group_attribute(
            self.client, self.ipg, buffer_profile=True)
        self.assertEqual(attr["buffer_profile"], self.buffer_profile)

        self.clearSendVerify()

        print("Verify forwarding works with updated custom buffer profile")

        status = sai_thrift_set_buffer_profile_attribute(
            self.client, self.buffer_profile, reserved_buffer_size=1600)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_set_buffer_profile_attribute(
            self.client, self.buffer_profile, shared_dynamic_th=7)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        attr = sai_thrift_get_buffer_profile_attribute(
            self.client, self.buffer_profile, reserved_buffer_size=True,
            shared_dynamic_th=True)
        self.assertEqual(attr["reserved_buffer_size"], 1600)
        self.assertEqual(attr["shared_dynamic_th"], 7)

        self.clearSendVerify()

        print("Verify forwarding works with default buffer profile")

        status = sai_thrift_set_ingress_priority_group_attribute(
            self.client, self.ipg, buffer_profile=0)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        attr = sai_thrift_get_ingress_priority_group_attribute(
            self.client, self.ipg, buffer_profile=True)
        self.assertEqual(attr["buffer_profile"], 0)

        self.clearSendVerify()

    def tearDown(self):
        sai_thrift_set_port_attribute(
            self.client, self.port0, qos_pfc_priority_to_priority_group_map=0)
        sai_thrift_remove_qos_map(self.client, self.qos_map)
        sai_thrift_remove_ingress_priority_group(self.client, self.ipg)
        sai_thrift_remove_buffer_profile(self.client, self.buffer_profile)
        sai_thrift_remove_buffer_pool(self.client, self.ingr_pool)

        super(Forwarding, self).tearDown()


@group("draft")
class BufferPoolNumber(MinimalPortVlanConfig):
    """
    Verify buffer pool creation count:
     * Ingress = SAI_SWITCH_ATTR_INGRESS_BUFFER_POOL_NUM,
     * Egress = SAI_SWITCH_ATTR_EGRESS_BUFFER_POOL_NUM.

    Verify pools creation with traffic and stats by binding pool to buffer
    profile then binding buffer profile to ingress priority group / queue.
    """

    def __init__(self):
        super(BufferPoolNumber, self).__init__(port_num=2)

    def setUp(self):
        super(BufferPoolNumber, self).setUp()

        self.ingr_pools = []
        self.egr_pools = []
        self.ingr_profiles = []
        self.egr_profiles = []
        self.ipgs = []
        self.queues = []

        self.dscp_to_tc_map = None
        self.tc_to_ipg_map = None
        self.prio_to_ipg_map = None
        self.tc_to_q_map = None

        self.buf_size = 1024 * 80
        self.profile_buf_size = self.buf_size / 10

    def runTest(self):
        print()

        sw_attrs = sai_thrift_get_switch_attribute(
            self.client, ingress_buffer_pool_num=True,
            egress_buffer_pool_num=True,
            total_buffer_size=True)
        ingr_pool_num = sw_attrs["ingress_buffer_pool_num"]
        egr_pool_num = sw_attrs["egress_buffer_pool_num"]

        print("total_buffer_size:", sw_attrs["total_buffer_size"])
        print("ingress_buffer_pool_num:", ingr_pool_num)
        print("egress_buffer_pool_num:", egr_pool_num)

        print("Create {} ingress buffer pools".format(ingr_pool_num))
        for i in range(ingr_pool_num):
            print("Create ingr_pool", i)

            ingr_pool = sai_thrift_create_buffer_pool(
                self.client, type=SAI_BUFFER_POOL_TYPE_INGRESS,
                size=self.buf_size + i)
            self.assertGreater(ingr_pool, 0)
            self.ingr_pools.append(ingr_pool)
        print("OK")

        print("Create {} egress buffer pools".format(egr_pool_num))
        for i in range(egr_pool_num):
            print("Create egr_pool", i)

            egr_pool = sai_thrift_create_buffer_pool(
                self.client, type=SAI_BUFFER_POOL_TYPE_EGRESS,
                size=self.buf_size + i)
            self.assertGreater(egr_pool, 0)
            self.egr_pools.append(egr_pool)
        print("OK")

        print("Create {0} ingress buffer profiles and {0} IPGs".format(
            ingr_pool_num))

        for i in range(ingr_pool_num):
            profile = sai_thrift_create_buffer_profile(
                self.client, pool_id=self.ingr_pools[i],
                reserved_buffer_size=self.profile_buf_size,
                shared_static_th=self.profile_buf_size,
                threshold_mode=SAI_BUFFER_PROFILE_THRESHOLD_MODE_STATIC)
            self.assertGreater(profile, 0)
            self.ingr_profiles.append(profile)

            ipg = sai_thrift_create_ingress_priority_group(
                self.client, port=self.port0, index=i, buffer_profile=profile)
            self.assertGreater(ipg, 0)
            self.ipgs.append(ipg)
        print("OK")

        print("Create {} egress buffer profiles and bind each to queue".format(
            egr_pool_num))

        attr = sai_thrift_get_port_attribute(
            self.client, self.port1, qos_number_of_queues=True)

        q_list = sai_thrift_object_list_t(count=attr["qos_number_of_queues"])
        attr = sai_thrift_get_port_attribute(
            self.client, self.port1, qos_queue_list=q_list)
        self.queues = attr["qos_queue_list"].idlist

        for i in range(egr_pool_num):
            profile = sai_thrift_create_buffer_profile(
                self.client, pool_id=self.egr_pools[i],
                reserved_buffer_size=self.profile_buf_size,
                shared_static_th=self.profile_buf_size,
                threshold_mode=SAI_BUFFER_PROFILE_THRESHOLD_MODE_STATIC)
            self.assertGreater(profile, 0)
            self.egr_profiles.append(profile)

            status = sai_thrift_set_queue_attribute(
                self.client, self.queues[i], buffer_profile_id=profile)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("OK")

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

        for i in range(ingr_pool_num):
            tc_to_ipg.append(
                sai_thrift_qos_map_t(
                    key=sai_thrift_qos_map_params_t(tc=i),
                    value=sai_thrift_qos_map_params_t(pg=i)))

            prio_to_ipg.append(
                sai_thrift_qos_map_t(
                    key=sai_thrift_qos_map_params_t(prio=i),
                    value=sai_thrift_qos_map_params_t(pg=i)))

        for i in range(egr_pool_num):
            tc_to_q.append(
                sai_thrift_qos_map_t(
                    key=sai_thrift_qos_map_params_t(tc=i),
                    value=sai_thrift_qos_map_params_t(queue_index=i)))

        print("Create DSCP to TC map")
        qos_map_list = sai_thrift_qos_map_list_t(
            maplist=dscp_to_tc, count=len(dscp_to_tc))

        self.dscp_to_tc_map = sai_thrift_create_qos_map(
            self.client, type=SAI_QOS_MAP_TYPE_DSCP_TO_TC,
            map_to_value_list=qos_map_list)
        self.assertGreater(self.dscp_to_tc_map, 0)

        status = sai_thrift_set_port_attribute(
            self.client, self.port0, qos_dscp_to_tc_map=self.dscp_to_tc_map)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        status = sai_thrift_set_port_attribute(
            self.client, self.port1, qos_dscp_to_tc_map=self.dscp_to_tc_map)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("OK")

        print("Create TC to iCoS map")
        qos_map_list = sai_thrift_qos_map_list_t(
            maplist=tc_to_ipg, count=len(tc_to_ipg))

        self.tc_to_ipg_map = sai_thrift_create_qos_map(
            self.client, type=SAI_QOS_MAP_TYPE_TC_TO_PRIORITY_GROUP,
            map_to_value_list=qos_map_list)
        self.assertGreater(self.tc_to_ipg_map, 0)

        status = sai_thrift_set_port_attribute(
            self.client, self.port0,
            qos_tc_to_priority_group_map=self.tc_to_ipg_map)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("OK")

        print("Create iCoS to IPG map")
        qos_map_list = sai_thrift_qos_map_list_t(
            maplist=prio_to_ipg, count=len(prio_to_ipg))

        self.prio_to_ipg_map = sai_thrift_create_qos_map(
            self.client, type=SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_PRIORITY_GROUP,
            map_to_value_list=qos_map_list)
        self.assertGreater(self.prio_to_ipg_map, 0)

        status = sai_thrift_set_port_attribute(
            self.client, self.port0,
            qos_pfc_priority_to_priority_group_map=self.prio_to_ipg_map)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("OK")

        print("Create TC to queue map")
        qos_map_list = sai_thrift_qos_map_list_t(
            maplist=tc_to_q, count=len(tc_to_q))

        self.tc_to_q_map = sai_thrift_create_qos_map(
            self.client, type=SAI_QOS_MAP_TYPE_TC_TO_QUEUE,
            map_to_value_list=qos_map_list)
        self.assertGreater(self.tc_to_q_map, 0)

        status = sai_thrift_set_port_attribute(
            self.client, self.port1, qos_tc_to_queue_map=self.tc_to_q_map)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        print("OK")

        print("Send 1 packet to each IPG and queue")
        for i in range(ingr_pool_num):
            pkt = simple_udpv6_packet(ipv6_dscp=i)
            send_packet(self, self.dev_port0, pkt)

        for i in range(egr_pool_num):
            pkt = simple_udpv6_packet(ipv6_dscp=i)
            send_packet(self, self.dev_port1, pkt)

        print("Verify traffic")
        time.sleep(2)

        for i in range(ingr_pool_num):
            stats = sai_thrift_get_ingress_priority_group_stats(
                self.client, self.ipgs[i])
            self.assertEqual(
                stats["SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS"], 1)

        for i in range(egr_pool_num):
            stats = sai_thrift_get_queue_stats(self.client, self.queues[i])
            self.assertEqual(stats["SAI_QUEUE_STAT_PACKETS"], 1)

    def tearDown(self):
        # Unset QoS maps.
        sai_thrift_set_port_attribute(
            self.client, self.port1, qos_tc_to_queue_map=0)
        sai_thrift_set_port_attribute(
            self.client, self.port1, qos_dscp_to_tc_map=0)

        sai_thrift_set_port_attribute(
            self.client, self.port0, qos_pfc_priority_to_priority_group_map=0)
        sai_thrift_set_port_attribute(
            self.client, self.port0, qos_tc_to_priority_group_map=0)
        sai_thrift_set_port_attribute(
            self.client, self.port0, qos_dscp_to_tc_map=0)

        # Remove QoS maps.
        sai_thrift_remove_qos_map(self.client, self.tc_to_q_map)
        sai_thrift_remove_qos_map(self.client, self.prio_to_ipg_map)
        sai_thrift_remove_qos_map(self.client, self.tc_to_ipg_map)
        sai_thrift_remove_qos_map(self.client, self.dscp_to_tc_map)

        # Unbind buffer profile from queues.
        for q in self.queues:
            sai_thrift_set_queue_attribute(self.client, q, buffer_profile_id=0)

        # Remove IPGs.
        for ipg in self.ipgs:
            sai_thrift_remove_ingress_priority_group(self.client, ipg)

        # Remove buffer profiles.
        for profile in self.egr_profiles:
            sai_thrift_remove_buffer_profile(self.client, profile)

        for profile in self.ingr_profiles:
            sai_thrift_remove_buffer_profile(self.client, profile)

        # Remove buffer pools.
        for pool in self.egr_pools:
            sai_thrift_remove_buffer_pool(self.client, pool)

        for pool in self.ingr_pools:
            sai_thrift_remove_buffer_pool(self.client, pool)

        super(BufferPoolNumber, self).tearDown()
