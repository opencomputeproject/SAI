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
Thrift SAI interface Policer tests
"""
from collections import namedtuple
import time
from sai_thrift.sai_headers import *
from sai_base_test import *


def banner(string):
    """
    Prints banner

    Args:
        string (str): String to print in banner form.
    """
    print()
    print("-" * 70)
    print("{}\n".format(string))


@group("draft")
class PolicerApiTests(SaiHelperBase):
    """
    Policer tests without traffic
    """

    def policerCreate(self):
        """
        Verify policer creation for every combination of mode, meter type and
        color source
        """
        banner("policerCreate")

        pol_ids = []

        TestCase = namedtuple(
            "TestCase", ["mode", "meter_type", "color_source"])

        test_cases = [
            TestCase(mode, meter_type, color_source)
            for mode in range(SAI_POLICER_MODE_STORM_CONTROL + 1)
            for meter_type in range(SAI_METER_TYPE_BYTES + 1)
            for color_source in range(SAI_POLICER_COLOR_SOURCE_AWARE + 1)]

        try:
            for test_case in test_cases:
                print("Create policer for", test_case)

                pol_id = sai_thrift_create_policer(
                    self.client, mode=test_case.mode,
                    meter_type=test_case.meter_type,
                    color_source=test_case.color_source)
                pol_ids.append(pol_id)

                print("Created policer with id:", pol_id)
                self.assertGreater(pol_id, 0)

                get_attr = sai_thrift_get_policer_attribute(
                    self.client, pol_id, mode=True, meter_type=True,
                    color_source=True)

                print("Got attributes: (mode={}, meter_type={}, "
                      "color_source={})\n".format(
                          get_attr["mode"], get_attr["meter_type"],
                          get_attr["color_source"]))
                self.assertEqual(test_case.mode, get_attr["mode"])
                self.assertEqual(test_case.meter_type, get_attr["meter_type"])
                self.assertEqual(
                    test_case.color_source, get_attr["color_source"])

        finally:
            for pol_id in pol_ids:
                sai_thrift_remove_policer(self.client, pol_id)

    def policerMode(self):
        """
        Verify that policer mode correctly affects pir and pbs
        """
        banner("policerMode")

        pol_ids = []

        try:
            for mode in range(SAI_POLICER_MODE_STORM_CONTROL + 1):
                print("Create policer with mode", mode)

                cir = 10
                pir = 30
                cbs = 40
                pbs = 60

                pol_id = sai_thrift_create_policer(
                    self.client, mode=mode,
                    meter_type=SAI_METER_TYPE_PACKETS,
                    color_source=SAI_POLICER_COLOR_SOURCE_BLIND,
                    cir=cir,
                    pir=pir,
                    cbs=cbs,
                    pbs=pbs)
                pol_ids.append(pol_id)

                print("Created policer with id:", pol_id)
                self.assertGreater(pol_id, 0)

                if mode == SAI_POLICER_MODE_SR_TCM:
                    exp_pir = cir
                    exp_pbs = pbs
                elif mode == SAI_POLICER_MODE_STORM_CONTROL:
                    exp_pir = cir
                    exp_pbs = cbs
                else:
                    exp_pir = pir
                    exp_pbs = pbs

                get_attr = sai_thrift_get_policer_attribute(
                    self.client, pol_id, mode=True,
                    cir=True, pir=True, cbs=True, pbs=True)

                print("Got attributes: (mode={}, cir={}, pir={}, "
                      "cbs={}, pbs={})\n".format(
                          get_attr["mode"], get_attr["cir"], get_attr["pir"],
                          get_attr["cbs"], get_attr["pbs"]))
                self.assertEqual(mode, get_attr["mode"])
                self.assertEqual(cir, get_attr["cir"])
                self.assertEqual(exp_pir, get_attr["pir"])
                self.assertEqual(cbs, get_attr["cbs"])
                self.assertEqual(exp_pbs, get_attr["pbs"])

                new_cir = 20
                sai_thrift_set_policer_attribute(
                    self.client, pol_id, cir=new_cir)
                print("Set cir to new value {}", new_cir)

                if mode in (SAI_POLICER_MODE_SR_TCM,
                            SAI_POLICER_MODE_STORM_CONTROL):
                    exp_pir = new_cir

                get_attr = sai_thrift_get_policer_attribute(
                    self.client, pol_id, mode=True,
                    cir=True, pir=True, cbs=True, pbs=True)

                print("Got attributes: (mode={}, cir={}, pir={}, "
                      "cbs={}, pbs={})\n".format(
                          get_attr["mode"], get_attr["cir"], get_attr["pir"],
                          get_attr["cbs"], get_attr["pbs"]))
                self.assertEqual(mode, get_attr["mode"])
                self.assertEqual(new_cir, get_attr["cir"])
                self.assertEqual(exp_pir, get_attr["pir"])
                self.assertEqual(cbs, get_attr["cbs"])
                self.assertEqual(exp_pbs, get_attr["pbs"])

                new_cbs = 50
                sai_thrift_set_policer_attribute(
                    self.client, pol_id, cbs=new_cbs)
                print("Set cbs to new value {}", new_cbs)

                if mode == SAI_POLICER_MODE_STORM_CONTROL:
                    exp_pbs = new_cbs

                get_attr = sai_thrift_get_policer_attribute(
                    self.client, pol_id, mode=True,
                    cir=True, pir=True, cbs=True, pbs=True)

                print("Got attributes: (mode={}, cir={}, pir={}, "
                      "cbs={}, pbs={})\n".format(
                          get_attr["mode"], get_attr["cir"], get_attr["pir"],
                          get_attr["cbs"], get_attr["pbs"]))
                self.assertEqual(mode, get_attr["mode"])
                self.assertEqual(new_cir, get_attr["cir"])
                self.assertEqual(exp_pir, get_attr["pir"])
                self.assertEqual(new_cbs, get_attr["cbs"])
                self.assertEqual(exp_pbs, get_attr["pbs"])

            # Test single rate three color mode when PBS is not specified.
            print("Created SR_TCM policer with only cir, cbs with id:", pol_id)
            pol_id = sai_thrift_create_policer(
                self.client,
                mode=SAI_POLICER_MODE_SR_TCM,
                meter_type=SAI_METER_TYPE_PACKETS,
                color_source=SAI_POLICER_COLOR_SOURCE_BLIND,
                cir=cir,
                cbs=cbs)
            pol_ids.append(pol_id)

            get_attr = sai_thrift_get_policer_attribute(
                self.client, pol_id, mode=True,
                cir=True, pir=True, cbs=True, pbs=True)

            print("Got attributes: (mode={}, cir={}, pir={}, "
                  "cbs={}, pbs={})\n".format(
                      get_attr["mode"], get_attr["cir"], get_attr["pir"],
                      get_attr["cbs"], get_attr["pbs"]))
            self.assertEqual(SAI_POLICER_MODE_SR_TCM, get_attr["mode"])
            self.assertEqual(cir, get_attr["cir"])
            self.assertEqual(cir, get_attr["pir"])
            self.assertEqual(cbs, get_attr["cbs"])
            self.assertEqual(cbs, get_attr["pbs"])

        finally:
            for pol_id in pol_ids:
                sai_thrift_remove_policer(self.client, pol_id)

    def noPolicerTrapGroup(self):
        """
        Verify policer binds to hostif_trap_group with no existing policer
        """
        banner("noPolicerTrapGroup")

        try:
            trap_group_id = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True)

            print("Created hostif_trap_group with id:", trap_group_id)
            self.assertGreater(trap_group_id, 0)

            pol_id = sai_thrift_create_policer(
                self.client, mode=SAI_POLICER_MODE_SR_TCM,
                meter_type=SAI_METER_TYPE_PACKETS)

            print("Created policer with id:", pol_id)
            self.assertGreater(pol_id, 0)

            print("Bind policer to hostif_trap_group")
            status = sai_thrift_set_hostif_trap_group_attribute(
                self.client, trap_group_id, policer=pol_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            get_attr = sai_thrift_get_hostif_trap_group_attribute(
                self.client, trap_group_id, policer=True)
            self.assertEqual(pol_id, get_attr["policer"])

        finally:
            sai_thrift_remove_hostif_trap_group(self.client, trap_group_id)
            sai_thrift_remove_policer(self.client, pol_id)

    def policerOverwriteTrapGroup(self):
        """
        Verify policer binds to hostif_trap_group with existing policer
        """
        banner("policerOverwriteTrapGroup")

        try:
            pol_id = sai_thrift_create_policer(
                self.client, mode=SAI_POLICER_MODE_TR_TCM,
                meter_type=SAI_METER_TYPE_BYTES,
                color_source=SAI_POLICER_COLOR_SOURCE_AWARE)

            print("Created policer with id:", pol_id)
            self.assertGreater(pol_id, 0)

            trap_group_id = sai_thrift_create_hostif_trap_group(
                self.client, admin_state=True, policer=pol_id)

            print("Created hostif_trap_group with id:", trap_group_id)
            self.assertGreater(trap_group_id, 0)

            get_attr = sai_thrift_get_hostif_trap_group_attribute(
                self.client, trap_group_id, policer=True)
            self.assertEqual(pol_id, get_attr["policer"])

            pol2_id = sai_thrift_create_policer(
                self.client, mode=SAI_POLICER_MODE_STORM_CONTROL,
                meter_type=SAI_METER_TYPE_PACKETS)

            print("Created second policer with id:", pol2_id)
            self.assertGreater(pol2_id, 0)

            print("Bind second policer to hostif_trap_group")
            status = sai_thrift_set_hostif_trap_group_attribute(
                self.client, trap_group_id, policer=pol2_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            get_attr = sai_thrift_get_hostif_trap_group_attribute(
                self.client, trap_group_id, policer=True)
            self.assertEqual(pol2_id, get_attr["policer"])

        finally:
            sai_thrift_remove_hostif_trap_group(self.client, trap_group_id)
            sai_thrift_remove_policer(self.client, pol_id)
            sai_thrift_remove_policer(self.client, pol2_id)

    def runTest(self):
        self.policerCreate()
        self.policerMode()
        self.noPolicerTrapGroup()
        self.policerOverwriteTrapGroup()


@group("draft")
class BindPolicerToPort(SaiHelperBase):
    """
    Verify policer binds to port.

    Port meters need to be enabled.
    """

    def setUp(self):
        super(BindPolicerToPort, self).setUp()

        self.pol_id = sai_thrift_create_policer(
            self.client, mode=SAI_POLICER_MODE_TR_TCM,
            meter_type=SAI_METER_TYPE_PACKETS, cbs=9, cir=1, pbs=9, pir=1)

    def runTest(self):
        banner("BindPolicerToPort")

        print("Check port0 has no policer")
        attr = sai_thrift_get_port_attribute(
            self.client, self.port0, policer_id=True)
        self.assertEqual(attr["policer_id"], 0)

        print("Bind policer to port0")
        status = sai_thrift_set_port_attribute(
            self.client, self.port0, policer_id=self.pol_id)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        attr = sai_thrift_get_port_attribute(
            self.client, self.port0, policer_id=True)
        self.assertEqual(attr["policer_id"], self.pol_id)

    def tearDown(self):
        sai_thrift_set_port_attribute(self.client, self.port0, policer_id=0)
        sai_thrift_remove_policer(self.client, self.pol_id)

        super(BindPolicerToPort, self).tearDown()


@group("draft")
class BindPolicerToAclEntry(MinimalPortVlanConfig):
    """
    Verify policer binds to ACL entry by sending traffic which should be
    colored red and dropped. Verify if traffic was dropped and policer
    statistics.

    Ingress ACL meters need to be enabled.
    """

    def __init__(self):
        super(BindPolicerToAclEntry, self).__init__(port_num=3)

    def setUp(self):
        super(BindPolicerToAclEntry, self).setUp()

        self.pol_id = sai_thrift_create_policer(
            self.client, mode=SAI_POLICER_MODE_TR_TCM,
            meter_type=SAI_METER_TYPE_PACKETS, cbs=5, cir=1, pbs=5, pir=1,
            red_packet_action=SAI_PACKET_ACTION_DROP)

        self.acl_table_id = None
        self.acl_entry_id = None

    def runTest(self):
        banner("BindPolicerToAclEntry")

        print("Create ACL table")
        action_type = [SAI_ACL_ACTION_TYPE_SET_POLICER]
        action = sai_thrift_s32_list_t(
            count=len(action_type), int32list=action_type)

        bind_point = [SAI_ACL_BIND_POINT_TYPE_PORT]
        acl_bpt_list = sai_thrift_s32_list_t(count=len(bind_point),
                                             int32list=bind_point)

        self.acl_table_id = sai_thrift_create_acl_table(
            self.client, acl_stage=SAI_ACL_STAGE_INGRESS, field_dst_ip=True,
            acl_action_type_list=action, acl_bind_point_type_list=acl_bpt_list)

        print("Bind ACL table to port")
        status = sai_thrift_set_port_attribute(
            self.client, self.port0, ingress_acl=self.acl_table_id)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        print("Create ACL entry with action_set_policer")
        dst_ip = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="192.168.0.0"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.0"))

        param = sai_thrift_acl_action_parameter_t(oid=self.pol_id)
        obj_acl_action_data = sai_thrift_acl_action_data_t(parameter=param)

        self.acl_entry_id = sai_thrift_create_acl_entry(
            self.client, table_id=self.acl_table_id, field_dst_ip=dst_ip,
            action_set_policer=obj_acl_action_data, admin_state=True)
        self.assertGreater(self.acl_entry_id, 0)

        print("Send traffic")
        pkt = simple_udp_packet(ip_dst="192.168.0.2")
        tx_cnt = 10
        for i in range(tx_cnt):
            print("Sending UDP packet", i)
            send_packet(self, self.dev_port0, pkt)
            time.sleep(0.1)
        print()

        print("Verify traffic")
        rx_cnt = count_matched_packets_all_ports(
            self, pkt, [self.dev_port1, self.dev_port2])
        print("Flooded packets:", rx_cnt)
        self.assertLess(rx_cnt, tx_cnt * 2)
        verify_no_other_packets(self)

        stats = sai_thrift_get_policer_stats(self.client, self.pol_id)
        # Remove entry which is not a statistic.
        del stats["SAI_POLICER_STAT_CUSTOM_RANGE_BASE"]
        for stat, value in stats.items():
            print(stat, value)

        self.assertEqual(stats["SAI_POLICER_STAT_PACKETS"], tx_cnt)
        self.assertLess(stats["SAI_POLICER_STAT_GREEN_PACKETS"], tx_cnt)
        yellow_red_packets = (stats["SAI_POLICER_STAT_YELLOW_PACKETS"] +
                              stats["SAI_POLICER_STAT_RED_PACKETS"])
        self.assertGreater(yellow_red_packets, 0)

    def tearDown(self):
        sai_thrift_remove_acl_entry(self.client, self.acl_entry_id)
        sai_thrift_set_port_attribute(self.client, self.port0, ingress_acl=0)
        sai_thrift_remove_acl_table(self.client, self.acl_table_id)
        sai_thrift_remove_policer(self.client, self.pol_id)

        super(BindPolicerToAclEntry, self).tearDown()


@group("draft")
class PolicerTrafficTests(MinimalPortVlanConfig):
    """
    Policer tests with traffic
    """

    def __init__(self):
        super(PolicerTrafficTests, self).__init__(port_num=3)

    def setUp(self):
        super(PolicerTrafficTests, self).setUp()

        self.arp_pkt = simple_arp_packet(arp_op=1, pktlen=100)

        self.lldp_pkt = simple_eth_packet(
            eth_dst="01:80:c2:00:00:0e", eth_src="00:11:11:11:11:11",
            pktlen=60, eth_type=0x88cc)

        self.pol_id = sai_thrift_create_policer(
            self.client, mode=SAI_POLICER_MODE_TR_TCM,
            meter_type=SAI_METER_TYPE_PACKETS, cbs=1, cir=1, pbs=1, pir=1)
        self.assertGreater(self.pol_id, 0)

        self.arp_trap_group = sai_thrift_create_hostif_trap_group(
            self.client, admin_state=True, policer=self.pol_id, queue=2)
        self.assertGreater(self.arp_trap_group, 0)

        self.arp_trap_id = sai_thrift_create_hostif_trap(
            self.client, packet_action=SAI_PACKET_ACTION_TRAP,
            trap_type=SAI_HOSTIF_TRAP_TYPE_ARP_REQUEST,
            trap_group=self.arp_trap_group)
        self.assertGreater(self.arp_trap_id, 0)

        self.lldp_trap_group = sai_thrift_create_hostif_trap_group(
            self.client, admin_state=True, queue=4)
        self.assertGreater(self.lldp_trap_group, 0)

        self.lldp_trap_id = sai_thrift_create_hostif_trap(
            self.client, packet_action=SAI_PACKET_ACTION_TRAP,
            trap_type=SAI_HOSTIF_TRAP_TYPE_LLDP,
            trap_group=self.lldp_trap_group)
        self.assertGreater(self.lldp_trap_id, 0)

    def tearDown(self):
        sai_thrift_remove_hostif_trap(self.client, self.lldp_trap_id)
        sai_thrift_remove_hostif_trap_group(self.client, self.lldp_trap_group)

        sai_thrift_remove_hostif_trap(self.client, self.arp_trap_id)
        sai_thrift_remove_hostif_trap_group(self.client, self.arp_trap_group)

        sai_thrift_remove_policer(self.client, self.pol_id)

        super(PolicerTrafficTests, self).tearDown()


@group("draft")
class NoIncrementAfterUnbind(PolicerTrafficTests):
    """
    Verify policer counter not incrementing after unbinding from
    hostif_trap_group.
    """

    def runTest(self):
        print()

        pre_stats = sai_thrift_get_queue_stats(self.client, self.cpu_queue2)
        tx_cnt = 5
        for i in range(tx_cnt):
            print("Sending ARP packet", i)
            send_packet(self, self.dev_port0, self.arp_pkt)
            time.sleep(0.1)
        print()

        time.sleep(4)
        print("Check SAI_POLICER_STAT_PACKETS incremented by {}".format(
            tx_cnt))
        stats = sai_thrift_get_policer_stats(self.client, self.pol_id)
        self.assertEqual(tx_cnt, stats["SAI_POLICER_STAT_PACKETS"])
        print("Check SAI_QUEUE_STAT_PACKETS incremented by {}".format(tx_cnt))
        print("Check number of packets that ingressed on CPU port "
              "equals number of green packets ({})".format(
                  stats["SAI_POLICER_STAT_GREEN_PACKETS"]))
        post_stats = sai_thrift_get_queue_stats(self.client, self.cpu_queue2)
        self.assertEqual(
            post_stats["SAI_QUEUE_STAT_PACKETS"],
            pre_stats["SAI_QUEUE_STAT_PACKETS"] +
            stats["SAI_POLICER_STAT_GREEN_PACKETS"])

        print("Unbind policer from hostif_trap_group")
        status = sai_thrift_set_hostif_trap_group_attribute(
            self.client, self.arp_trap_group, policer=0)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        attr = sai_thrift_get_hostif_trap_group_attribute(
            self.client, self.arp_trap_group, policer=True)
        self.assertEqual(0, attr["policer"])

        pre_stats = sai_thrift_get_queue_stats(self.client, self.cpu_queue2)
        print("Send traffic again, policer counters shall not change\n")
        for i in range(tx_cnt):
            print("Sending ARP packet", i)
            send_packet(self, self.dev_port0, self.arp_pkt)
            time.sleep(0.1)
        print()

        print("Check policer counters didn\"t change")
        stats_new = sai_thrift_get_policer_stats(self.client, self.pol_id)
        # Remove entry which is not a statistic.
        del stats["SAI_POLICER_STAT_CUSTOM_RANGE_BASE"]
        del stats_new["SAI_POLICER_STAT_CUSTOM_RANGE_BASE"]
        self.assertEqual(stats_new, stats)

        time.sleep(4)
        post_stats = sai_thrift_get_queue_stats(self.client, self.cpu_queue2)
        self.assertEqual(
            post_stats["SAI_QUEUE_STAT_PACKETS"],
            pre_stats["SAI_QUEUE_STAT_PACKETS"] + tx_cnt)


@group("draft")
class Overflow1Policer2TrapGroups(PolicerTrafficTests):
    """
    Verify policer can be bound to >1 hostif_trap_group.
    Verify policer is being applied for all hostif_trap objects if bound to
    >1 hostif_trap_group.
    Verify policer unbind from one hostif_trap_group does not affect other
    when traffic exceeds policer parameters.
    """

    def runTest(self):
        print()

        status = sai_thrift_set_policer_attribute(
            self.client, self.pol_id, cbs=7, cir=1, pbs=7, pir=1)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        status = sai_thrift_set_hostif_trap_group_attribute(
            self.client, self.lldp_trap_group, policer=self.pol_id)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        q2_pre_stat = sai_thrift_get_queue_stats(self.client, self.cpu_queue2)
        q4_pre_stat = sai_thrift_get_queue_stats(self.client, self.cpu_queue4)

        tx_cnt = 5
        for i in range(tx_cnt):
            print("Sending ARP packet", i)
            send_packet(self, self.dev_port0, self.arp_pkt)
            time.sleep(0.1)
        print()

        for i in range(tx_cnt):
            print("Sending LLDP packet", i)
            send_packet(self, self.dev_port0, self.lldp_pkt)
            time.sleep(0.1)
        print()

        print("Unbind policer from arp_trap_group\n")
        status = sai_thrift_set_hostif_trap_group_attribute(
            self.client, self.arp_trap_group, policer=0)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        get_attr = sai_thrift_get_hostif_trap_group_attribute(
            self.client, self.arp_trap_group, policer=True)
        self.assertEqual(0, get_attr["policer"])

        for i in range(tx_cnt):
            print("Sending ARP packet", i)
            send_packet(self, self.dev_port0, self.arp_pkt)
            time.sleep(0.1)
        print()

        for i in range(tx_cnt):
            print("Sending LLDP packet", i)
            send_packet(self, self.dev_port0, self.lldp_pkt)
            time.sleep(0.1)
        print()

        time.sleep(4)
        print("Check SAI_POLICER_STAT_PACKETS incremented by {}".format(
            tx_cnt * 3))
        stats = sai_thrift_get_policer_stats(self.client, self.pol_id)
        self.assertEqual(tx_cnt * 3, stats["SAI_POLICER_STAT_PACKETS"])

        rx_cnt_cpu = stats["SAI_POLICER_STAT_GREEN_PACKETS"]

        q2_post_stat = sai_thrift_get_queue_stats(self.client, self.cpu_queue2)
        q4_post_stat = sai_thrift_get_queue_stats(self.client, self.cpu_queue4)
        t = "SAI_QUEUE_STAT_PACKETS"
        q_stats = (q4_post_stat[t] - q4_pre_stat[t]) + \
            (q2_post_stat[t] - q2_pre_stat[t])

        print("Check number of queue packets ({}) equals number of "
              "packets tx\"d to CPU port ({})".format(
                  q_stats, rx_cnt_cpu + tx_cnt))
        self.assertEqual(q_stats, tx_cnt + rx_cnt_cpu)


@group("draft")
class Underflow1Policer2TrapGroups(PolicerTrafficTests):
    """
    Verify policer can be bound to >1 hostif_trap_group.
    Verify policer is being applied for all hostif_trap object
    if bound to >1 hostif_trap_group.
    Verify policer unbind from one hostif_trap_group does not affect other
    when traffic doesn"t exceeds policer parameters.
    """

    def runTest(self):
        print()

        status = sai_thrift_set_policer_attribute(
            self.client, self.pol_id, cbs=15, cir=1, pbs=15, pir=1)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        status = sai_thrift_set_hostif_trap_group_attribute(
            self.client, self.lldp_trap_group, policer=self.pol_id)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        q2_pre_stat = sai_thrift_get_queue_stats(self.client, self.cpu_queue2)
        q4_pre_stat = sai_thrift_get_queue_stats(self.client, self.cpu_queue4)

        tx_cnt = 5
        for i in range(tx_cnt):
            print("Sending ARP packet", i)
            send_packet(self, self.dev_port0, self.arp_pkt)
            time.sleep(0.1)
        print()

        for i in range(tx_cnt):
            print("Sending LLDP packet", i)
            send_packet(self, self.dev_port0, self.lldp_pkt)
            time.sleep(0.1)
        print()

        print("Unbind policer from arp_trap_group\n")
        status = sai_thrift_set_hostif_trap_group_attribute(
            self.client, self.arp_trap_group, policer=0)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        get_attr = sai_thrift_get_hostif_trap_group_attribute(
            self.client, self.arp_trap_group, policer=True)
        self.assertEqual(0, get_attr["policer"])

        stats_unbind = sai_thrift_get_policer_stats(self.client, self.pol_id)

        for i in range(tx_cnt):
            print("Sending ARP packet", i)
            send_packet(self, self.dev_port0, self.arp_pkt)
            time.sleep(0.1)
        print()

        self.assertEqual(
            stats_unbind,
            sai_thrift_get_policer_stats(self.client, self.pol_id))

        for i in range(tx_cnt):
            print("Sending LLDP packet", i)
            send_packet(self, self.dev_port0, self.lldp_pkt)
            time.sleep(0.1)
        print()

        stats = sai_thrift_get_policer_stats(self.client, self.pol_id)

        print("Check SAI_POLICER_STAT_PACKETS incremented by", tx_cnt * 3)
        self.assertEqual(tx_cnt * 3, stats["SAI_POLICER_STAT_PACKETS"])

        rx_cnt_cpu = stats["SAI_POLICER_STAT_GREEN_PACKETS"]

        q2_post_stat = sai_thrift_get_queue_stats(self.client, self.cpu_queue2)
        q4_post_stat = sai_thrift_get_queue_stats(self.client, self.cpu_queue4)
        t = "SAI_QUEUE_STAT_PACKETS"
        q_stats = (q4_post_stat[t] - q4_pre_stat[t]) + \
            (q2_post_stat[t] - q2_pre_stat[t])

        print("Check number of queue packets ({}) equals number of "
              "packets tx\"d to CPU port ({})".format(
                  q_stats, rx_cnt_cpu + tx_cnt))
        self.assertEqual(q_stats, tx_cnt + rx_cnt_cpu)


@group("draft")
class StormControlTests(MinimalPortVlanConfig):
    """
    Verify policer works for SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID
    Verify policer works for SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID
    Verify policer works for SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID
    """

    def __init__(self):
        super(StormControlTests, self).__init__(port_num=3)

    def setUp(self):
        super(StormControlTests, self).setUp()

        # Create policers.
        self.bc_sc = sai_thrift_create_policer(
            self.client, meter_type=SAI_METER_TYPE_PACKETS,
            mode=SAI_POLICER_MODE_STORM_CONTROL, cbs=2, cir=2)
        self.assertGreater(self.bc_sc, 0)

        self.mc_sc = sai_thrift_create_policer(
            self.client, meter_type=SAI_METER_TYPE_PACKETS,
            mode=SAI_POLICER_MODE_STORM_CONTROL, cbs=2, cir=2)
        self.assertGreater(self.mc_sc, 0)

        self.uc_sc = sai_thrift_create_policer(
            self.client, meter_type=SAI_METER_TYPE_PACKETS,
            mode=SAI_POLICER_MODE_STORM_CONTROL, cbs=2, cir=2)
        self.assertGreater(self.uc_sc, 0)

        # Set storm controllers.
        status = sai_thrift_set_port_attribute(
            self.client, self.port0,
            broadcast_storm_control_policer_id=self.bc_sc)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_port_attribute(
            self.client, self.port0,
            broadcast_storm_control_policer_id=True)
        self.assertEqual(
            attr["broadcast_storm_control_policer_id"], self.bc_sc)

        status = sai_thrift_set_port_attribute(
            self.client, self.port0,
            multicast_storm_control_policer_id=self.mc_sc)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_port_attribute(
            self.client, self.port0,
            multicast_storm_control_policer_id=True)
        self.assertEqual(
            attr["multicast_storm_control_policer_id"], self.mc_sc)

        status = sai_thrift_set_port_attribute(
            self.client, self.port0,
            flood_storm_control_policer_id=self.uc_sc)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        attr = sai_thrift_get_port_attribute(
            self.client, self.port0,
            flood_storm_control_policer_id=True)
        self.assertEqual(
            attr["flood_storm_control_policer_id"], self.uc_sc)

    def test(self, name, pkt, pol_id):
        """
        Runs test

        Args:
            name (str): Name of the test
            pkt (Packet): Packet sent in the test
            pol_id (int): ID of policer which stats will be checked
        """
        banner(name)

        tx_cnt = 5
        for i in range(tx_cnt):
            print("Sending frame", i)
            send_packet(self, self.dev_port0, pkt)
            time.sleep(0.1)
        print()

        rx_cnt = count_matched_packets_all_ports(
            self, pkt, [self.dev_port1, self.dev_port2])
        verify_no_other_packets(self)

        stats = sai_thrift_get_policer_stats(self.client, pol_id)

        print("Packets flooded to ports 1 and 2:", rx_cnt)
        print("SAI_POLICER_STAT_PACKETS:",
              stats["SAI_POLICER_STAT_PACKETS"])
        print("SAI_POLICER_STAT_GREEN_PACKETS:",
              stats["SAI_POLICER_STAT_GREEN_PACKETS"])
        print("SAI_POLICER_STAT_RED_PACKETS:",
              stats["SAI_POLICER_STAT_RED_PACKETS"])

        self.assertLess(rx_cnt / 2, tx_cnt)
        self.assertEqual(stats["SAI_POLICER_STAT_PACKETS"], tx_cnt)
        self.assertEqual(stats["SAI_POLICER_STAT_GREEN_PACKETS"], rx_cnt / 2)
        self.assertGreater(stats["SAI_POLICER_STAT_GREEN_PACKETS"], 0)
        self.assertGreater(stats["SAI_POLICER_STAT_RED_PACKETS"], 0)
        self.assertEqual(stats["SAI_POLICER_STAT_YELLOW_PACKETS"], 0)

    def runTest(self):
        pkt = simple_udp_packet(
            eth_dst="ff:ff:ff:ff:ff:ff", ip_dst="192.168.0.255")
        self.test("broadcastStormControl", pkt, self.bc_sc)

        pkt = simple_udp_packet(
            eth_dst="01:00:5e:00:00:01", ip_dst="224.0.0.1")
        self.test("multicastStormControl", pkt, self.mc_sc)

        pkt = simple_udp_packet()
        self.test("floodStormControl", pkt, self.uc_sc)

    def tearDown(self):
        # Unset storm controllers.
        sai_thrift_set_port_attribute(
            self.client, self.port0, broadcast_storm_control_policer_id=0)
        sai_thrift_set_port_attribute(
            self.client, self.port0, multicast_storm_control_policer_id=0)
        sai_thrift_set_port_attribute(
            self.client, self.port0, flood_storm_control_policer_id=0)

        sai_thrift_remove_policer(self.client, self.bc_sc)
        sai_thrift_remove_policer(self.client, self.mc_sc)
        sai_thrift_remove_policer(self.client, self.uc_sc)

        super(StormControlTests, self).tearDown()


@group("draft")
class VerifyColors(MinimalPortVlanConfig):
    """
    Verify counters per color - it is recommended
    to verify this functionality on hardware only.
    """

    def __init__(self):
        super(VerifyColors, self).__init__(port_num=3)

    def setUp(self):
        super(VerifyColors, self).setUp()
        self.arp_pkt = simple_arp_packet(arp_op=1, pktlen=100)

        self.pol_id = sai_thrift_create_policer(
            self.client, mode=SAI_POLICER_MODE_TR_TCM,
            meter_type=SAI_METER_TYPE_PACKETS, cbs=5, cir=10, pbs=10, pir=10)
        self.assertGreater(self.pol_id, 0)

        self.trap_group_id = sai_thrift_create_hostif_trap_group(
            self.client, admin_state=True, policer=self.pol_id)
        self.assertGreater(self.trap_group_id, 0)

        self.arp_trap_id = sai_thrift_create_hostif_trap(
            self.client, packet_action=SAI_PACKET_ACTION_TRAP,
            trap_type=SAI_HOSTIF_TRAP_TYPE_ARP_REQUEST,
            trap_group=self.trap_group_id)
        self.assertGreater(self.arp_trap_id, 0)

    def sendTrafficGetStats(self, tx_cnt):
        """
        Sends traffic and returns policer stats.

        Args:
            tx_cnt (int): Number of packets to send

        Returns:
            dict: policer statistics
        """
        print("Send {} ARP packets".format(tx_cnt))
        for _ in range(tx_cnt):
            send_packet(self, self.dev_port0, self.arp_pkt)
        print()

        time.sleep(2.0)
        stats = sai_thrift_get_policer_stats(self.client, self.pol_id)
        print("SAI_POLICER_STAT_GREEN_PACKETS:",
              stats["SAI_POLICER_STAT_GREEN_PACKETS"])
        print("SAI_POLICER_STAT_YELLOW_PACKETS:",
              stats["SAI_POLICER_STAT_YELLOW_PACKETS"])
        print("SAI_POLICER_STAT_RED_PACKETS:",
              stats["SAI_POLICER_STAT_RED_PACKETS"])
        print("SAI_POLICER_STAT_PACKETS:",
              stats["SAI_POLICER_STAT_PACKETS"])

        return stats

    def runTest(self):
        banner("VerifyColors")

        attr = sai_thrift_get_policer_attribute(
            self.client, self.pol_id, cbs=True, cir=True, pbs=True, pir=True)
        print("Buckets information")
        print("cbs:", attr["cbs"], "cir:", attr["cir"])
        print("pbs:", attr["pbs"], "pir:", attr["pir"])
        print()

        stats = self.sendTrafficGetStats(15)

        self.assertAlmostEqual(
            stats["SAI_POLICER_STAT_GREEN_PACKETS"], 6, delta=1)
        self.assertAlmostEqual(
            stats["SAI_POLICER_STAT_YELLOW_PACKETS"], 5, delta=1)
        self.assertAlmostEqual(
            stats["SAI_POLICER_STAT_RED_PACKETS"], 4, delta=1)
        self.assertEqual(stats["SAI_POLICER_STAT_PACKETS"], 15)

        refill_time = 1.0
        print("\nSleep for {}s to allow buckets to fully refill\n".format(
            refill_time))
        time.sleep(refill_time)

        stats = self.sendTrafficGetStats(15)

        self.assertAlmostEqual(
            stats["SAI_POLICER_STAT_GREEN_PACKETS"], 12, delta=2)
        self.assertAlmostEqual(
            stats["SAI_POLICER_STAT_YELLOW_PACKETS"], 10, delta=2)
        self.assertAlmostEqual(
            stats["SAI_POLICER_STAT_RED_PACKETS"], 8, delta=2)
        self.assertEqual(stats["SAI_POLICER_STAT_PACKETS"], 30)

    def tearDown(self):
        sai_thrift_remove_hostif_trap(self.client, self.arp_trap_id)
        sai_thrift_remove_hostif_trap_group(self.client, self.trap_group_id)
        sai_thrift_remove_policer(self.client, self.pol_id)

        super(VerifyColors, self).tearDown()
