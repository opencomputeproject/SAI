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
Thrift SAI interface Weighted random early detection (WRED) tests
"""
from sai_thrift.sai_headers import *

from sai_base_test import *


@group("draft")
class WredBaseTest(SaiHelper):
    """
    WRED base test
    """
    def setUp(self):
        """
        Configure WredTest:

        1. Call SaiHelper setUp()
        2. Configure routing.
           - route traffic destined for 172.20.20.11 into port 11
           - route traffic destined for 172.20.20.12 into port 12
           - route traffic destined for 4000::2 into port 13
        3. Get queues so that they can be used within tests
           - self.queues10 of port 10
           - self.queues11 of port 11
           - self.queues12 of port 12
           - self.queues13 of port 13
        4. Create wred profiles. they will be assigned to queues
           in separate tests
           - self.wred_ecn - "mark all" profile
           - self.wred_drop - "drop all" profile
        """

        super(WredBaseTest, self).setUp()

        ##########################
        # Configure routes
        ##########################
        print("Configure routing...")
        self.nexthops = []
        self.neighbors = []
        self.routes = []
        ##########################
        # 172.20.20.11 -> port 11
        # 4000::1      -> port 11
        ##########################
        # NextHop
        nexthop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress("172.20.10.11"),
            router_interface_id=self.port11_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nexthops.append(nexthop)
        # Neighbor entry
        neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.port11_rif, ip_address=sai_ipaddress("172.20.10.11"))
        sai_thrift_create_neighbor_entry(
            self.client, neighbor_entry, dst_mac_address="00:11:22:33:44:55")
        self.neighbors.append(neighbor_entry)
        # Route entry
        route = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix("172.20.10.11/32"))
        sai_thrift_create_route_entry(self.client, route, next_hop_id=nexthop)
        self.routes.append(route)
        # Route entry (IPv6)
        route = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix("4000::1/128"))
        sai_thrift_create_route_entry(self.client, route, next_hop_id=nexthop)
        self.routes.append(route)
        ##########################
        # 172.20.20.12 -> port 12
        # 4000::2      -> port 12
        ##########################
        # NextHop
        nexthop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress("172.20.10.12"),
            router_interface_id=self.port12_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nexthops.append(nexthop)
        # Neighbor entry
        neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.port12_rif, ip_address=sai_ipaddress("172.20.10.12"))
        sai_thrift_create_neighbor_entry(
            self.client, neighbor_entry, dst_mac_address="00:11:22:33:44:55")
        self.neighbors.append(neighbor_entry)
        # Route entry
        route = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix("172.20.10.12/32"))
        sai_thrift_create_route_entry(self.client, route, next_hop_id=nexthop)
        self.routes.append(route)
        # Route entry (IPv6)
        route = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix("4000::2/128"))
        sai_thrift_create_route_entry(self.client, route, next_hop_id=nexthop)
        self.routes.append(route)
        ##########################
        # 172.20.20.13 -> port 13
        # 4000::3      -> port 13
        ##########################
        # NextHop
        nexthop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress("172.20.10.13"),
            router_interface_id=self.port13_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.nexthops.append(nexthop)
        # Neighbor entry
        neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.port13_rif, ip_address=sai_ipaddress("172.20.10.13"))
        sai_thrift_create_neighbor_entry(
            self.client, neighbor_entry, dst_mac_address="00:11:22:33:44:55")
        self.neighbors.append(neighbor_entry)
        # Route entry
        route = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix("172.20.10.13/32"))
        sai_thrift_create_route_entry(self.client, route, next_hop_id=nexthop)
        self.routes.append(route)
        # Route entry (IPv6)
        route = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix("4000::3/128"))
        sai_thrift_create_route_entry(self.client, route, next_hop_id=nexthop)
        self.routes.append(route)
        ##########################
        # Get queues
        ##########################
        print("Get queues...")
        queue_list = sai_thrift_object_list_t(count=10)
        attr = sai_thrift_get_port_attribute(
            self.client, self.port10, qos_queue_list=queue_list)
        self.queues10 = attr["SAI_PORT_ATTR_QOS_QUEUE_LIST"].idlist
        queue_list = sai_thrift_object_list_t(count=10)
        attr = sai_thrift_get_port_attribute(
            self.client, self.port11, qos_queue_list=queue_list)
        self.queues11 = attr["SAI_PORT_ATTR_QOS_QUEUE_LIST"].idlist
        queue_list = sai_thrift_object_list_t(count=10)
        attr = sai_thrift_get_port_attribute(
            self.client, self.port12, qos_queue_list=queue_list)
        self.queues12 = attr["SAI_PORT_ATTR_QOS_QUEUE_LIST"].idlist
        queue_list = sai_thrift_object_list_t(count=10)
        attr = sai_thrift_get_port_attribute(
            self.client, self.port13, qos_queue_list=queue_list)
        self.queues13 = attr["SAI_PORT_ATTR_QOS_QUEUE_LIST"].idlist
        ##########################
        # Configure WRED profiles
        ##########################
        print("Create WRED profiles...")
        max_thr = 50 * 80
        # Create ECN marking WRED profile (WredIPv4Test)
        self.wred_ecn = sai_thrift_create_wred(
            self.client,
            ecn_mark_mode=True,
            green_min_threshold=0,
            green_max_threshold=max_thr,
            green_drop_probability=100)
        self.assertTrue(self.wred_ecn != 0)
        # Create dropping WRED profile (WredDropIPv4Test)
        self.wred_drop = sai_thrift_create_wred(
            self.client,
            green_enable=True,
            green_min_threshold=0,
            green_max_threshold=max_thr,
            green_drop_probability=100)
        self.assertTrue(self.wred_drop != 0)
        print("-----------------------")

    def tearDown(self):
        """
        Deconfigure WredTest:

        1. Remove wred profiles
           - self.wred_ecn - "mark all" profile
           - self.wred_drop - "drop all" profile
        2. Deconfigure routing
           - remove all created route entries
           - remove all created neighbor entries
           - remove all created nexthops
        1. Call SaiHelper tearDown()
        """
        print("\n\n-----------------------")
        print("Tear down WRED tests...")
        #######################
        # Remove WRED profiles
        #######################
        print("Remove WRED profiles...")
        status = sai_thrift_remove_wred(self.client, self.wred_ecn)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        status = sai_thrift_remove_wred(self.client, self.wred_drop)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        ################
        # Remove routes
        ################
        print("Deconfigure routing...")
        for route in self.routes:
            status = sai_thrift_remove_route_entry(self.client, route)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
        for neighbor in self.neighbors:
            status = sai_thrift_remove_neighbor_entry(self.client, neighbor)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
        for nexthop in self.nexthops:
            status = sai_thrift_remove_next_hop(self.client, nexthop)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

        super(WredBaseTest, self).tearDown()

    @staticmethod
    def _statsDiff(i_stats, e_stats):
        """ Returns a dict off stats that differ
        Args:
            i_stats(dict): i stats
            e_stats(dict): e stats

        Returns:
            dect: diff
        """
        return {stat: e_stats[stat] - i_stats[stat]
                for stat in e_stats
                if e_stats[stat] - i_stats[stat]}

    @staticmethod
    def _printStats(*stats):
        for (name, i_stats, e_stats) in stats:
            print("\t", name, "stats:", WredBaseTest._statsDiff(i_stats,
                                                                e_stats))

    def _verifyStat(self, i_stats, e_stats, stat, delta):
        self.assertEqual(e_stats[stat]-i_stats[stat], delta)

    def _verifyStats(self, i_stats, e_stats, *stats):
        print("Verify stats:")
        for (name, delta) in stats:
            print("|", name, "=>", delta)
            self._verifyStat(i_stats, e_stats, name, delta)


@group("draft")
class WredTest(WredBaseTest):
    """
    Test WRED interface, also with traffic not affected by WRED
    """
    def setUp(self):
        print("\n==========")
        print("WRED tests")
        print("==========\n")

        super(WredTest, self).setUp()

    def runTest(self):
        self._WredBindProfileTest()
        self._WredReplaceProfileTest()
        self._WredBindProfileMultipleQueuesTest()
        self._WredIPv4NonECNTest()

    def _WredBindProfileTest(self):
        """
        wred.1 - Verify wred profile bind to a queue with no prior WRED profile
          1. Add the WRED profile to all queues of a single port.
        Cleanup:
          1. Detach all WRED profiles
        """
        print("\n\n_WredBindProfileTest()")
        print("----------------------")
        try:
            # Assign WRED profile to all queues
            print("Attach the WRED profile to queues.")
            for queue in self.queues11:
                status = sai_thrift_set_queue_attribute(
                    self.client, queue, wred_profile_id=self.wred_ecn)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
        finally:
            for queue in self.queues11:
                status = sai_thrift_set_queue_attribute(self.client,
                                                        queue,
                                                        wred_profile_id=0)
                self.assertEqual(status, SAI_STATUS_SUCCESS)

    def _WredReplaceProfileTest(self):
        """
        wred.2 - Verify WRED profile bind to a queue with an existing
                 WRED profile
          1. Add the WRED profile to all queues of a single port.
          2. Add the another WRED profile to all off them.
        Cleanup:
          1. Detach all WRED profiles
        """
        print("\n\n_WredReplaceProfileTest()")
        print("-------------------------")
        try:
            # Assign WRED profile to all queues
            print("Attach the WRED profile to queues.")
            for queue in self.queues12:
                status = sai_thrift_set_queue_attribute(
                    self.client, queue, wred_profile_id=self.wred_ecn)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
            # Assign WRED profile to all queues
            print("Attach the another WRED profile to the same queues.")
            for queue in self.queues12:
                status = sai_thrift_set_queue_attribute(
                    self.client, queue, wred_profile_id=self.wred_drop)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
        finally:
            # Clean up all queues
            for queue in self.queues12:
                status = sai_thrift_set_queue_attribute(
                    self.client, queue, wred_profile_id=0)
                self.assertEqual(status, SAI_STATUS_SUCCESS)

    def _WredBindProfileMultipleQueuesTest(self):
        """
        wred.3 - Verify WRED profile can be bound to multiple queues
          1. Add the WRED profile to all queues of a single port.
          2. Add the same WRED profile to all queues of a another port.
          3. Add the same WRED profile to all queues of a another port.
        Cleanup:
          1. Detach all WRED profiles (of all three ports)
        """
        print("\n\n_WredBindProfileMultipleQueuesTest()")
        print("------------------------------------")
        try:
            # Assign WRED profile to all queues (port11)
            print("Attach the WRED profile to queues.")
            for queue in self.queues11:
                status = sai_thrift_set_queue_attribute(
                    self.client, queue, wred_profile_id=self.wred_ecn)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
            # Assign WRED profile to all queues (port12)
            print("Attach the WRED profile to another port queues.")
            for queue in self.queues12:
                status = sai_thrift_set_queue_attribute(
                    self.client, queue, wred_profile_id=self.wred_ecn)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
            # Assign WRED profile to all queues (port13)
            print("Attach the WRED profile to yet another port queues.")
            for queue in self.queues13:
                status = sai_thrift_set_queue_attribute(
                    self.client, queue, wred_profile_id=self.wred_ecn)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
        finally:
            # Clean up all queues
            for queue in self.queues11:
                status = sai_thrift_set_queue_attribute(self.client,
                                                        queue,
                                                        wred_profile_id=0)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
            for queue in self.queues12:
                status = sai_thrift_set_queue_attribute(self.client,
                                                        queue,
                                                        wred_profile_id=0)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
            for queue in self.queues13:
                status = sai_thrift_set_queue_attribute(self.client,
                                                        queue,
                                                        wred_profile_id=0)
                self.assertEqual(status, SAI_STATUS_SUCCESS)

    def _WredIPv4NonECNTest(self):
        """
        wred.4 - Verify non-ECN traffic is unaffected by WRED profile
                 on a queue
          1. Add the WRED (ECN) profile to the queue of a single port.
          2. Send a non-ECN packet. Verify, that the packet was not affected
             by ECN profile
        Cleanup:
          1. Detach the WRED profile
        """
        print("\n\n_WredIPv4NonECNTest()")
        print("---------------------")
        i_port_stats = sai_thrift_get_port_stats(self.client,
                                                 self.port11)
        i_queue_stats = sai_thrift_get_queue_stats(self.client,
                                                   self.queues11[0])
        try:
            pkt1 = simple_tcp_packet(
                eth_dst='00:77:66:55:44:00',
                eth_src='00:22:22:22:22:22',
                ip_dst='172.20.10.11',
                ip_src='192.168.0.1',
                ip_id=105,
                ip_ttl=64)
            # ECN unaffected packet
            exp_pkt1 = simple_tcp_packet(
                eth_dst='00:11:22:33:44:55',
                eth_src='00:77:66:55:44:00',
                ip_dst='172.20.10.11',
                ip_src='192.168.0.1',
                ip_id=105,
                ip_ttl=63)
            # Assign WRED profile to the queue
            status = sai_thrift_set_queue_attribute(
                self.client, self.queues11[0], wred_profile_id=self.wred_ecn)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            print("ECT is NOT set.")
            print("Send and receive a packet.")
            send_packet(self, self.dev_port10, pkt1)
            verify_packet(self, exp_pkt1, self.dev_port11)
            time.sleep(2)
            e_port_stats = sai_thrift_get_port_stats(self.client,
                                                     self.port11)
            e_queue_stats = sai_thrift_get_queue_stats(self.client,
                                                       self.queues11[0])

            # Verify stats
            self._verifyStats(
                i_port_stats, e_port_stats,
                ('SAI_PORT_STAT_ECN_MARKED_PACKETS', 0))
            self._verifyStats(
                i_queue_stats, e_queue_stats,
                ('SAI_QUEUE_STAT_PACKETS', 1))
        finally:
            status = sai_thrift_set_queue_attribute(self.client,
                                                    self.queues11[0],
                                                    wred_profile_id=0)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
