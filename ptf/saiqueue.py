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
Thrift SAI interface queue tests
"""
from sai_thrift.sai_headers import *
from sai_base_test import *


@group("draft")
class QueueConfigData(SaiHelper):
    """
    Queue Configuration Class
    """

    def setUp(self):
        super(QueueConfigData, self).setUp()
        attr = sai_thrift_get_port_attribute(self.client,
                                             self.port25,
                                             qos_number_of_queues=True)
        self.num_queues = attr["qos_number_of_queues"]
        self.q_list = sai_thrift_object_list_t(count=self.num_queues)

        # L3 layer configuration
        self.vr_id = sai_thrift_create_virtual_router(self.client,
                                                      admin_v4_state=True,
                                                      admin_v6_state=True)
        self.assertNotEqual(self.vr_id, 0)

        # RIFs
        self.rif_id25 = sai_thrift_create_router_interface(
            self.client,
            virtual_router_id=self.vr_id,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port25,
            admin_v4_state=True,
            admin_v6_state=True)
        self.assertNotEqual(self.rif_id25, 0)
        self.rif_id26 = sai_thrift_create_router_interface(
            self.client,
            virtual_router_id=self.vr_id,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            port_id=self.port26,
            admin_v4_state=True,
            admin_v6_state=True)
        self.assertNotEqual(self.rif_id26, 0)

        # Neighbor
        dst_ip = "172.16.1.1"
        dst_mask_ip = "172.16.1.1/24"
        self.neighbor_dmac = "00:11:22:33:44:55"
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.rif_id26,
            sai_ipaddress(dst_ip))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.neighbor_entry1,
                                         dst_mac_address=self.neighbor_dmac)

        # Nexthop
        self.nhop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.rif_id26,
            ip=sai_ipaddress(dst_ip))

        # Route
        self.route = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix(dst_mask_ip),
            vr_id=self.vr_id)
        status = sai_thrift_create_route_entry(self.client,
                                               self.route,
                                               next_hop_id=self.nhop)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        self.queueCreateTest()
        self.portQueueQueryTest()
        self.bufferQueueTest()
        self.schedulerQueueTest()
        self.pfcPriorityQueueTest()
        self.cpuPortQueueObjectTest()
        self.wredQueueTest()

    def tearDown(self):
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_route_entry(self.client, self.route)
        sai_thrift_remove_next_hop(self.client, self.nhop)
        sai_thrift_remove_router_interface(self.client, self.rif_id26)
        sai_thrift_remove_router_interface(self.client, self.rif_id25)
        sai_thrift_remove_virtual_router(self.client, self.vr_id)
        super(QueueConfigData, self).tearDown()

    def queueCreateTest(self):
        """
        The test verifies a queue creation.
        """
        print("\nQueue Create Test")

        test_port = self.port1

        queue = sai_thrift_create_queue(
            self.client, type=SAI_QUEUE_TYPE_UNICAST,
            index=1)
        # The port is missing, the creation should fail.
        self.assertEqual(queue, 0)

        queue = sai_thrift_create_queue(
            self.client,
            port=test_port, index=1)
        # The type is missing, the creation should fail.
        self.assertEqual(queue, 0)

        queue = sai_thrift_create_queue(
            self.client, type=SAI_QUEUE_TYPE_ALL,
            port=test_port, index=1)
        # The type is wrong, the creation should fail.
        self.assertEqual(queue, 0)

        queue = sai_thrift_create_queue(
            self.client, type=SAI_QUEUE_TYPE_UNICAST,
            port=test_port)
        # The index is missing, the creation should fail.
        self.assertEqual(queue, 0)

        queue = sai_thrift_create_queue(
            self.client, type=SAI_QUEUE_TYPE_UNICAST,
            port=test_port, index=60)
        # The index is wrong, the creation should fail.
        self.assertEqual(queue, 0)

        queue = sai_thrift_create_queue(
            self.client, type=SAI_QUEUE_TYPE_UNICAST,
            port=test_port, index=1)
        # The queue with such index already exists.
        self.assertEqual(queue, 0)

        print("\tTest completed successfully")

    def portQueueQueryTest(self):
        """
        The test queries SAI_QUEUE_ATTR_PORT and SAI_QUEUE_ATTR_INDEX
        attributes for all physical ports configured by default.
        NOTE: If you modify this test, remember to apply the changes to
        cpuPortQueueObjectTest too.
        """
        print("Port Queue Query Test")
        for port in self.port_list:
            attr = sai_thrift_get_port_attribute(self.client,
                                                 port,
                                                 qos_number_of_queues=True)
            num_queues = attr["qos_number_of_queues"]
            q_list = sai_thrift_object_list_t(count=num_queues)
            attr = sai_thrift_get_port_attribute(self.client,
                                                 port,
                                                 qos_queue_list=q_list)
            for queue in range(num_queues):
                queue_id = attr["qos_queue_list"].idlist[queue]
                q_attr = sai_thrift_get_queue_attribute(
                    self.client,
                    queue_id,
                    port=True,
                    index=True,
                    parent_scheduler_node=True)
                self.assertEqual(queue, q_attr["index"])
                self.assertEqual(port, q_attr["port"])
                # If SDK does not support hierarhical QoS,
                # the port handle should be returned.
                self.assertEqual(port, q_attr["parent_scheduler_node"])

        print("\tTest completed successfully")

    def bufferQueueTest(self):
        """
        The test is dedicated to working buffers with queues. After creating
        buffer pool and buffer profile, the last one is assigned to a queue.
        Later the buffer profile is being removed and the default buffer
        profile should be assigned to the queue.
        NOTE: If you modify this test, remember to apply the changes to
        cpuPortQueueObjectTest too.
        """
        print("Buffer Queue Test")
        mode = SAI_BUFFER_POOL_THRESHOLD_MODE_DYNAMIC
        buff_pool = sai_thrift_create_buffer_pool(
            self.client,
            type=SAI_BUFFER_POOL_TYPE_EGRESS,
            size=1024,
            threshold_mode=mode)
        self.assertNotEqual(buff_pool, 0)
        buff_prof = sai_thrift_create_buffer_profile(self.client,
                                                     pool_id=buff_pool,
                                                     reserved_buffer_size=1024,
                                                     threshold_mode=mode,
                                                     shared_dynamic_th=1,
                                                     xoff_th=100,
                                                     xon_th=10)
        self.assertNotEqual(buff_prof, 0)

        attr = sai_thrift_get_port_attribute(self.client,
                                             self.port26,
                                             qos_queue_list=self.q_list)

        queue_id = attr["qos_queue_list"].idlist

        attr = sai_thrift_get_queue_attribute(self.client,
                                              queue_id[0],
                                              buffer_profile_id=True)
        default_buff_prof = attr["buffer_profile_id"]

        status = sai_thrift_set_queue_attribute(self.client,
                                                queue_id[0],
                                                buffer_profile_id=buff_prof)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        sai_thrift_clear_queue_stats(self.client, queue_id[0])
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src="00:00:00:00:00:22",
                                ip_dst="172.16.1.1",
                                ip_src="192.168.0.1",
                                ip_id=105,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst="00:11:22:33:44:55",
                                    eth_src=ROUTER_MAC,
                                    ip_dst="172.16.1.1",
                                    ip_src="192.168.0.1",
                                    ip_id=105,
                                    ip_ttl=63)

        send_packet(self, self.dev_port25, pkt)
        verify_packet(self, exp_pkt, self.dev_port26)
        print("\tPacket received on PORT26")
        stats = sai_thrift_get_queue_stats(self.client, queue_id[0])
        cnt = stats["SAI_QUEUE_STAT_PACKETS"]
        self.assertEqual(cnt, 1)

        # Now the buffer profile is being detached.
        status = sai_thrift_set_queue_attribute(
            self.client,
            queue_id[0],
            buffer_profile_id=int(SAI_NULL_OBJECT_ID))
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_remove_buffer_profile(self.client, buff_prof)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_remove_buffer_pool(self.client, buff_pool)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # The queue should be assigned to the default buffer profile.
        attr = sai_thrift_get_queue_attribute(self.client,
                                              queue_id[0],
                                              buffer_profile_id=True)
        self.assertEqual(attr["buffer_profile_id"], default_buff_prof)

        sai_thrift_clear_queue_stats(self.client, queue_id[0])
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src="00:00:00:00:00:22",
                                ip_dst="172.16.1.1",
                                ip_src="192.168.0.1",
                                ip_id=105,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst="00:11:22:33:44:55",
                                    eth_src=ROUTER_MAC,
                                    ip_dst="172.16.1.1",
                                    ip_src="192.168.0.1",
                                    ip_id=105,
                                    ip_ttl=63)

        send_packet(self, self.dev_port25, pkt)
        verify_packet(self, exp_pkt, self.dev_port26)
        print("\tPacket received on PORT26")
        stats = sai_thrift_get_queue_stats(self.client, queue_id[0])
        cnt = stats["SAI_QUEUE_STAT_PACKETS"]
        self.assertEqual(cnt, 1)
        print("\tTest completed successfully")

    def schedulerQueueTest(self):
        """
        The test scenario consists of attaching scheduler profile to a queue
        and validate following parameters: queue priority, weight,
        min and max rate.
        NOTE: If you modify this test, remember to apply the changes to
        cpuPortQueueObjectTest too.
        """
        print("Scheduler Queue Test")
        weight = 4
        min_rate = 100
        max_rate = 1000
        min_burst = 200
        max_burst = 800
        sched1 = sai_thrift_create_scheduler(
            self.client,
            meter_type=SAI_METER_TYPE_PACKETS,
            scheduling_type=SAI_SCHEDULING_TYPE_DWRR,
            scheduling_weight=weight)
        self.assertNotEqual(sched1, 0)
        sched2 = sai_thrift_create_scheduler(
            self.client,
            meter_type=SAI_METER_TYPE_PACKETS,
            scheduling_type=SAI_SCHEDULING_TYPE_STRICT,
            min_bandwidth_rate=min_rate,
            max_bandwidth_rate=max_rate)
        self.assertNotEqual(sched2, 0)
        sched3 = sai_thrift_create_scheduler(
            self.client,
            meter_type=SAI_METER_TYPE_PACKETS,
            scheduling_type=SAI_SCHEDULING_TYPE_STRICT,
            min_bandwidth_burst_rate=min_burst,
            max_bandwidth_burst_rate=max_burst)
        self.assertNotEqual(sched3, 0)
        print("\tScheduler profiles created successfully")
        try:
            attr = sai_thrift_get_port_attribute(
                self.client,
                self.port25,
                qos_queue_list=self.q_list)

            queue_id = attr["qos_queue_list"].idlist

            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[1],
                scheduler_profile_id=sched1)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id[1],
                scheduler_profile_id=True)
            self.assertEqual(q_attr["scheduler_profile_id"], sched1)

            s_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched1,
                scheduling_weight=True)
            self.assertEqual(s_attr["scheduling_weight"], weight)
            print("\tScheduling weight validated successfully")

            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[2],
                scheduler_profile_id=sched2)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id[2],
                scheduler_profile_id=True)
            self.assertEqual(q_attr["scheduler_profile_id"], sched2)

            s_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched2,
                min_bandwidth_rate=True,
                max_bandwidth_rate=True)
            self.assertEqual(s_attr["min_bandwidth_rate"], min_rate)
            self.assertEqual(s_attr["max_bandwidth_rate"], max_rate)
            print("\tBandwidth rates validated successfully")

            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[3],
                scheduler_profile_id=sched3)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id[3],
                scheduler_profile_id=True)
            self.assertEqual(q_attr["scheduler_profile_id"], sched3)

            s_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched3,
                min_bandwidth_burst_rate=True,
                max_bandwidth_burst_rate=True)
            self.assertEqual(s_attr["min_bandwidth_burst_rate"], min_burst)
            self.assertEqual(s_attr["max_bandwidth_burst_rate"], max_burst)
            print("\tBandwidth burst rates validated successfully")

            print("\tNow the parameters will be modified")
            weight = 3
            min_rate = 120
            max_rate = 980
            min_burst = 300
            max_burst = 800
            status = sai_thrift_set_scheduler_attribute(
                self.client,
                sched1,
                scheduling_weight=weight)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            s_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched1,
                scheduling_weight=True)
            self.assertEqual(s_attr["scheduling_weight"], weight)
            print("\tScheduling weight validated successfully")

            status = sai_thrift_set_scheduler_attribute(
                self.client,
                sched2,
                min_bandwidth_rate=min_rate)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_set_scheduler_attribute(
                self.client,
                sched2,
                max_bandwidth_rate=max_rate)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            s_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched2,
                min_bandwidth_rate=True,
                max_bandwidth_rate=True)
            self.assertEqual(s_attr["min_bandwidth_rate"], min_rate)
            self.assertEqual(s_attr["max_bandwidth_rate"], max_rate)
            print("\tBandwidth rates validated successfully")

            status = sai_thrift_set_scheduler_attribute(
                self.client,
                sched3,
                min_bandwidth_burst_rate=min_burst)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_set_scheduler_attribute(
                self.client,
                sched3,
                max_bandwidth_burst_rate=max_burst)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            s_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched3,
                min_bandwidth_burst_rate=True,
                max_bandwidth_burst_rate=True)
            self.assertEqual(s_attr["min_bandwidth_burst_rate"], min_burst)
            self.assertEqual(s_attr["max_bandwidth_burst_rate"], max_burst)
            print("\tBandwidth burst rates validated successfully")
            print("\tTest completed successfully")

        finally:
            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[3],
                scheduler_profile_id=0)
            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[2],
                scheduler_profile_id=0)
            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[1],
                scheduler_profile_id=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            sai_thrift_remove_scheduler(self.client, sched3)
            sai_thrift_remove_scheduler(self.client, sched2)
            sai_thrift_remove_scheduler(self.client, sched1)

    def pfcPriorityQueueTest(self):
        """
        The test verifies the correct configuration of mapping
        between Priority-based Flow Control (PFC) and a queue.
        NOTE: If you modify this test, remember to apply the changes to
        cpuPortQueueObjectTest too.
        """
        print("PFC Priority Queue Test")
        p_attr = sai_thrift_get_port_attribute(self.client,
                                               self.port25,
                                               qos_queue_list=self.q_list)
        queue_id = p_attr["qos_queue_list"].idlist

        q_attr = sai_thrift_get_queue_attribute(
            self.client,
            queue_id[4],
            index=True)
        q_idx = q_attr["index"]

        try:
            # The PFC Priority -> Queue map table configuration.
            pfc_to_queue_map = sai_thrift_qos_map_t(
                key=sai_thrift_qos_map_params_t(prio=4),
                value=sai_thrift_qos_map_params_t(queue_index=q_idx))
            qos_map_list = sai_thrift_qos_map_list_t(
                count=1,
                maplist=[pfc_to_queue_map])
            qos_map = sai_thrift_create_qos_map(
                self.client,
                type=SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_QUEUE,
                map_to_value_list=qos_map_list)
            self.assertNotEqual(qos_map, 0)

            # The queue index modification in the previously created QoS map.
            p_attr = sai_thrift_get_port_attribute(self.client,
                                                   self.port26,
                                                   qos_queue_list=self.q_list)
            new_queue_id = p_attr["qos_queue_list"].idlist

            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                new_queue_id[3],
                index=True)
            new_q_idx = q_attr["index"]
            new_pfc_to_queue_map = sai_thrift_qos_map_t(
                key=sai_thrift_qos_map_params_t(prio=4),
                value=sai_thrift_qos_map_params_t(queue_index=new_q_idx))
            new_qos_map_list = sai_thrift_qos_map_list_t(
                count=1,
                maplist=[new_pfc_to_queue_map])

            status = sai_thrift_set_qos_map_attribute(
                self.client,
                qos_map,
                map_to_value_list=new_qos_map_list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            # Verify if the correct queue is applied to the QoS map.
            map_lists = sai_thrift_qos_map_list_t(count=1)
            attr = sai_thrift_get_qos_map_attribute(
                self.client,
                qos_map,
                map_to_value_list=map_lists)
            queue_idx = attr["map_to_value_list"].maplist[0].value.queue_index
            self.assertEqual(queue_idx, new_q_idx)
            print("\tTest completed successfully")

        finally:
            sai_thrift_remove_qos_map(self.client, qos_map)

    def cpuPortQueueObjectTest(self):
        """
        The test uses CPU port instead of external port for
        all test cases above: portQueueQueryTest, bufferQueueTest
        and schedulerQueueTest, pfcPriorityQueueTest.
        """
        print("CPU Port Queue Object Test")
        attr = sai_thrift_get_switch_attribute(self.client, cpu_port=True)
        cpu_port_id = attr["cpu_port"]

        print("\tPART1: Port Queue Query Test")
        attr = sai_thrift_get_port_attribute(self.client,
                                             cpu_port_id,
                                             qos_number_of_queues=True)
        num_queues = attr["qos_number_of_queues"]
        q_list = sai_thrift_object_list_t(count=num_queues)
        try:
            for port in self.port_list:
                attr = sai_thrift_get_port_attribute(self.client,
                                                     port,
                                                     qos_queue_list=q_list)
                for queue in range(num_queues):
                    queue_id = attr["qos_queue_list"].idlist[queue]
                    q_attr = sai_thrift_get_queue_attribute(
                        self.client,
                        queue_id,
                        port=True,
                        index=True,
                        parent_scheduler_node=True)
                    self.assertEqual(queue, q_attr["index"])
                    self.assertEqual(port, q_attr["port"])
                    # If SDK does not support hierarhical QoS,
                    # the port handle should be returned.
                    self.assertEqual(port, q_attr["parent_scheduler_node"])
            print("\t\tPART1 completed successfully")

            print("\tPART2: Buffer Queue Test")
            # Get the queue for the CPU port - for the buffer queue test.
            attr = sai_thrift_get_port_attribute(
                self.client,
                cpu_port_id,
                qos_queue_list=self.q_list)

            queue_id = attr["qos_queue_list"].idlist

            mode = SAI_BUFFER_POOL_THRESHOLD_MODE_DYNAMIC
            buff_pool = sai_thrift_create_buffer_pool(
                self.client,
                type=SAI_BUFFER_POOL_TYPE_EGRESS,
                size=1024,
                threshold_mode=mode)
            self.assertNotEqual(buff_pool, 0)
            buff_prof = sai_thrift_create_buffer_profile(
                self.client,
                pool_id=buff_pool,
                reserved_buffer_size=1024,
                threshold_mode=mode,
                shared_dynamic_th=1,
                xoff_th=100,
                xon_th=10)
            self.assertNotEqual(buff_prof, 0)

            attr = sai_thrift_get_queue_attribute(self.client,
                                                  queue_id[0],
                                                  buffer_profile_id=True)
            default_buff_prof = attr["buffer_profile_id"]

            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[0],
                buffer_profile_id=buff_prof)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            # Now the buffer profile is being detached.
            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[0],
                buffer_profile_id=int(SAI_NULL_OBJECT_ID))
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_remove_buffer_profile(self.client, buff_prof)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_remove_buffer_pool(self.client, buff_pool)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            # The queue should be assigned to the default buffer profile.
            attr = sai_thrift_get_queue_attribute(self.client,
                                                  queue_id[0],
                                                  buffer_profile_id=True)
            self.assertEqual(attr["buffer_profile_id"], default_buff_prof)
            print("\t\tPART2 completed successfully")

            print("\tPART3: Scheduler Queue Test")
            weight = 4
            min_rate = 100
            max_rate = 1000
            min_burst = 200
            max_burst = 800

            # Get the queue for the CPU port - for the scheduler queue test.
            attr = sai_thrift_get_port_attribute(
                self.client,
                cpu_port_id,
                qos_queue_list=self.q_list)

            queue_id = attr["qos_queue_list"].idlist
            sched1 = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                scheduling_type=SAI_SCHEDULING_TYPE_DWRR,
                scheduling_weight=weight)
            self.assertNotEqual(sched1, 0)
            sched2 = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT,
                min_bandwidth_rate=min_rate,
                max_bandwidth_rate=max_rate)
            self.assertNotEqual(sched2, 0)
            sched3 = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT,
                min_bandwidth_burst_rate=min_burst,
                max_bandwidth_burst_rate=max_burst)
            self.assertNotEqual(sched3, 0)
            print("\t\tScheduler profiles created successfully")

            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[1],
                scheduler_profile_id=sched1)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id[1],
                scheduler_profile_id=True)
            self.assertEqual(q_attr["scheduler_profile_id"], sched1)

            s_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched1,
                scheduling_weight=True)
            self.assertEqual(s_attr["scheduling_weight"], weight)
            print("\t\tScheduling weight validated successfully")

            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[2],
                scheduler_profile_id=sched2)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id[2],
                scheduler_profile_id=True)
            self.assertEqual(q_attr["scheduler_profile_id"], sched2)

            s_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched2,
                min_bandwidth_rate=True,
                max_bandwidth_rate=True)
            self.assertEqual(s_attr["min_bandwidth_rate"], min_rate)
            self.assertEqual(s_attr["max_bandwidth_rate"], max_rate)
            print("\t\tBandwidth rates validated successfully")

            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[3],
                scheduler_profile_id=sched3)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id[3],
                scheduler_profile_id=True)
            self.assertEqual(q_attr["scheduler_profile_id"], sched3)

            s_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched3,
                min_bandwidth_burst_rate=True,
                max_bandwidth_burst_rate=True)
            self.assertEqual(s_attr["min_bandwidth_burst_rate"], min_burst)
            self.assertEqual(s_attr["max_bandwidth_burst_rate"], max_burst)
            print("\t\tBandwidth burst rates validated successfully")

            print("\t\tNow the parameters will be modified")
            weight = 3
            min_rate = 120
            max_rate = 980
            min_burst = 300
            max_burst = 800
            status = sai_thrift_set_scheduler_attribute(
                self.client,
                sched1,
                scheduling_weight=weight)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            s_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched1,
                scheduling_weight=True)
            self.assertEqual(s_attr["scheduling_weight"], weight)
            print("\t\tScheduling weight validated successfully")

            status = sai_thrift_set_scheduler_attribute(
                self.client,
                sched2,
                min_bandwidth_rate=min_rate)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_set_scheduler_attribute(
                self.client,
                sched2,
                max_bandwidth_rate=max_rate)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            s_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched2,
                min_bandwidth_rate=True,
                max_bandwidth_rate=True)
            self.assertEqual(s_attr["min_bandwidth_rate"], min_rate)
            self.assertEqual(s_attr["max_bandwidth_rate"], max_rate)
            print("\t\tBandwidth rates validated successfully")

            status = sai_thrift_set_scheduler_attribute(
                self.client,
                sched3,
                min_bandwidth_burst_rate=min_burst)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_set_scheduler_attribute(
                self.client,
                sched3,
                max_bandwidth_burst_rate=max_burst)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            s_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched3,
                min_bandwidth_burst_rate=True,
                max_bandwidth_burst_rate=True)
            self.assertEqual(s_attr["min_bandwidth_burst_rate"], min_burst)
            self.assertEqual(s_attr["max_bandwidth_burst_rate"], max_burst)
            print("\t\tBandwidth burst rates validated successfully")
            print("\t\tPART3 completed successfully")

            print("\tPART4: PFC Priority Queue Test")
            # Get the queue for the CPU port.
            p_attr = sai_thrift_get_port_attribute(
                self.client,
                cpu_port_id,
                qos_queue_list=self.q_list)
            queue_id = p_attr["qos_queue_list"].idlist

            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id[4],
                index=True)
            q_idx = q_attr["index"]

            # The PFC Priority -> Queue map table configuration.
            pfc_to_queue_map = sai_thrift_qos_map_t(
                key=sai_thrift_qos_map_params_t(prio=4),
                value=sai_thrift_qos_map_params_t(queue_index=q_idx))
            qos_map_list = sai_thrift_qos_map_list_t(
                count=1,
                maplist=[pfc_to_queue_map])
            qos_map = sai_thrift_create_qos_map(
                self.client,
                type=SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_QUEUE,
                map_to_value_list=qos_map_list)
            self.assertNotEqual(qos_map, 0)

            # The queue index modification in the previously created QoS map.
            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id[3],
                index=True)
            new_q_idx = q_attr["index"]
            new_pfc_to_queue_map = sai_thrift_qos_map_t(
                key=sai_thrift_qos_map_params_t(prio=4),
                value=sai_thrift_qos_map_params_t(queue_index=new_q_idx))
            new_qos_map_list = sai_thrift_qos_map_list_t(
                count=1,
                maplist=[new_pfc_to_queue_map])

            status = sai_thrift_set_qos_map_attribute(
                self.client,
                qos_map,
                map_to_value_list=new_qos_map_list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            # Verify if the correct queue is applied to the QoS map.
            map_lists = sai_thrift_qos_map_list_t(count=1)
            attr = sai_thrift_get_qos_map_attribute(
                self.client,
                qos_map,
                map_to_value_list=map_lists)
            queue_idx = attr["map_to_value_list"].maplist[0].value.queue_index
            self.assertEqual(queue_idx, new_q_idx)
            print("\t\tPART4 completed successfully")

            print("\tTest completed successfully")

        finally:
            sai_thrift_remove_qos_map(self.client, qos_map)
            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[3],
                scheduler_profile_id=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[2],
                scheduler_profile_id=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_set_queue_attribute(
                self.client,
                queue_id[1],
                scheduler_profile_id=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            sai_thrift_remove_scheduler(self.client, sched3)
            sai_thrift_remove_scheduler(self.client, sched2)
            sai_thrift_remove_scheduler(self.client, sched1)

    def wredQueueTest(self):
        """
        The test verifies WRED attaching and removing for a particular queue.
        """
        print("\nWRED Queue Test")
        wred_id = sai_thrift_create_wred(self.client,
                                         green_enable=True,
                                         green_min_threshold=0,
                                         green_max_threshold=4000,
                                         green_drop_probability=100)
        self.assertNotEqual(wred_id, 0)
        attr = sai_thrift_get_port_attribute(
            self.client,
            self.port25,
            qos_queue_list=self.q_list)
        queue_id = attr["qos_queue_list"].idlist
        status = sai_thrift_set_queue_attribute(
            self.client,
            queue_id[0],
            wred_profile_id=wred_id)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src="00:00:00:00:00:22",
                                ip_dst="172.16.1.1",
                                ip_src="192.168.0.1",
                                ip_id=105,
                                ip_ttl=64)

        exp_pkt = simple_tcp_packet(eth_dst="00:11:22:33:44:55",
                                    eth_src=ROUTER_MAC,
                                    ip_dst="172.16.1.1",
                                    ip_src="192.168.0.1",
                                    ip_id=105,
                                    ip_ttl=63)

        try:
            print("\tSending packet PORT25 -> PORT26")
            send_packet(self, self.dev_port25, pkt)
            verify_packet(self, exp_pkt, self.dev_port26)
            print("\tPacket received on PORT26")

            print("\tTest completed successfully")

        finally:
            status = sai_thrift_set_queue_attribute(self.client,
                                                    queue_id[0],
                                                    wred_profile_id=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            status = sai_thrift_remove_wred(self.client, wred_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
