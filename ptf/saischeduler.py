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
Thrift SAI interface Scheduler tests
"""

from sai_thrift.sai_headers import *

from sai_base_test import *


@group("draft")
class SchedulerParamsTest(SaiHelper):
    '''
    Basic scheduler parameters tests
    '''

    def setUp(self):
        super(SchedulerParamsTest, self).setUp()

        queue_list = sai_thrift_object_list_t(count=10)
        port_attr = sai_thrift_get_port_attribute(
            self.client, self.port1, qos_queue_list=queue_list)

        self.test_queue = port_attr['qos_queue_list'].idlist[0]

    def runTest(self):
        self.schedulerWeightTest()
        self.schedulerStictPriorityTest()
        self.schedulerMinBwidthRateTest()
        self.schedulerMaxBwidthRateTest()
        self.schedulerMinBwidthBurstRateTest()
        self.schedulerMaxBwidthBurstRateTest()

    def schedulerWeightTest(self):
        '''
        Verify creation of scheduler with scheduling type DWRR
        '''
        print("\nschedulerWeightTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                scheduling_type=SAI_SCHEDULING_TYPE_DWRR,
                scheduling_weight=2)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched,
                meter_type=True,
                scheduling_type=True,
                scheduling_weight=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_DWRR)
            self.assertEqual(sched_attr['scheduling_weight'], 2)

            status = sai_thrift_set_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            queue_attr = sai_thrift_get_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=True)
            self.assertEqual(queue_attr['scheduler_profile_id'], sched)

            status = sai_thrift_set_scheduler_attribute(
                self.client, sched, scheduling_weight=4)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, scheduling_weight=True)
            self.assertEqual(sched_attr['scheduling_weight'], 4)

        finally:
            sai_thrift_set_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)

    def schedulerStictPriorityTest(self):
        '''
        Verify creation of scheduler with priority set
        '''
        print("\nschedulerStrictPriorityTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, meter_type=True, scheduling_type=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_STRICT)

            status = sai_thrift_set_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            queue_attr = sai_thrift_get_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=True)
            self.assertEqual(queue_attr['scheduler_profile_id'], sched)

        finally:
            sai_thrift_set_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)

    def schedulerMinBwidthRateTest(self):
        '''
        Verify creation of scheduler with min bandwidth rate set
        '''
        print("\nschedulerMinBwidthRateTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                min_bandwidth_rate=100,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched,
                meter_type=True,
                min_bandwidth_rate=True,
                scheduling_type=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['min_bandwidth_rate'], 100)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_STRICT)

            status = sai_thrift_set_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            queue_attr = sai_thrift_get_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=True)
            self.assertEqual(queue_attr['scheduler_profile_id'], sched)

            status = sai_thrift_set_scheduler_attribute(
                self.client, sched, min_bandwidth_rate=200)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, min_bandwidth_rate=True)
            self.assertEqual(sched_attr['min_bandwidth_rate'], 200)

        finally:
            sai_thrift_set_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)

    def schedulerMaxBwidthRateTest(self):
        '''
        Verify creation of scheduler with max bandwidth rate set
        '''
        print("\nschedulerMaxBwidthRateTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                max_bandwidth_rate=1000,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched,
                meter_type=True,
                max_bandwidth_rate=True,
                scheduling_type=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['max_bandwidth_rate'], 1000)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_STRICT)

            status = sai_thrift_set_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            queue_attr = sai_thrift_get_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=True)
            self.assertEqual(queue_attr['scheduler_profile_id'], sched)

            status = sai_thrift_set_scheduler_attribute(
                self.client, sched, max_bandwidth_rate=2000)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, max_bandwidth_rate=True)
            self.assertEqual(sched_attr['max_bandwidth_rate'], 2000)

        finally:
            sai_thrift_set_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)

    def schedulerMinBwidthBurstRateTest(self):
        '''
        Verify creation of scheduler with min bandwidth burst rate set
        '''
        print("\nschedulerMinBwidthBurstRateTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                min_bandwidth_burst_rate=100,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched,
                meter_type=True,
                min_bandwidth_burst_rate=True,
                scheduling_type=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['min_bandwidth_burst_rate'], 100)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_STRICT)

            status = sai_thrift_set_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            queue_attr = sai_thrift_get_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=True)
            self.assertEqual(queue_attr['scheduler_profile_id'], sched)

            status = sai_thrift_set_scheduler_attribute(
                self.client, sched, min_bandwidth_burst_rate=200)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, min_bandwidth_burst_rate=True)
            self.assertEqual(sched_attr['min_bandwidth_burst_rate'], 200)

        finally:
            sai_thrift_set_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)

    def schedulerMaxBwidthBurstRateTest(self):
        '''
        Verify creation of scheduler with max bandwidth burst rate set
        '''
        print("\nschedulerMaxBwidthBurstRateTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                max_bandwidth_burst_rate=1000,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched,
                meter_type=True,
                max_bandwidth_burst_rate=True,
                scheduling_type=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['max_bandwidth_burst_rate'], 1000)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_STRICT)

            status = sai_thrift_set_queue_attribute(self.client,
                                                    self.test_queue,
                                                    scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            queue_attr = sai_thrift_get_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=True)
            self.assertEqual(queue_attr['scheduler_profile_id'], sched)

            status = sai_thrift_set_scheduler_attribute(
                self.client, sched, max_bandwidth_burst_rate=2000)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, max_bandwidth_burst_rate=True)
            self.assertEqual(sched_attr['max_bandwidth_burst_rate'], 2000)

        finally:
            sai_thrift_set_queue_attribute(
                self.client, self.test_queue, scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)


@group("draft")
class SchedulerGroupAttachTest(SaiHelper):
    '''
    Verify possibilities of attaching scheduler with different params set
    to a scheduler group
    '''

    def setUp(self):
        super(SchedulerGroupAttachTest, self).setUp()

        try:
            self.schgroup = sai_thrift_create_scheduler_group(
                self.client, level=0, port_id=self.port1)
            self.assertNotEqual(self.schgroup, 0)

        except BaseException:
            print("Failed to create scheduler group")

    def runTest(self):
        self.schedulerWeightGroupAttachTest()
        self.schedulerStrictPriorityGroupAttachTest()
        self.schedulerMinBwidthRateGroupAttachTest()
        self.schedulerMaxBwidthRateGroupAttachTest()
        self.schedulerMinBwidthBurstRateGroupAttachTest()
        self.schedulerMaxBwidthBurstRateGroupAttachTest()

    def tearDown(self):
        sai_thrift_remove_scheduler_group(self.client, self.schgroup)

        super(SchedulerGroupAttachTest, self).tearDown()

    def schedulerWeightGroupAttachTest(self):
        '''
        Verify creation of scheduler with scheduling type DWRR and attaching
        it to a scheduler group
        '''
        print("\nschedulerWeightGroupAttachTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                scheduling_type=SAI_SCHEDULING_TYPE_DWRR,
                scheduling_weight=2)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched,
                meter_type=True,
                scheduling_type=True,
                scheduling_weight=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_DWRR)
            self.assertEqual(sched_attr['scheduling_weight'], 2)

            status = sai_thrift_set_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            schgroup_attr = sai_thrift_get_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=True)
            self.assertEqual(schgroup_attr['scheduler_profile_id'], sched)

            status = sai_thrift_set_scheduler_attribute(
                self.client, sched, scheduling_weight=4)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, scheduling_weight=True)
            self.assertEqual(sched_attr['scheduling_weight'], 4)

        finally:
            sai_thrift_set_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)

    def schedulerStrictPriorityGroupAttachTest(self):
        '''
        Verify creation of scheduler with priority set and attaching it
        to a scheduler group
        '''
        print("\nschedulerStrictPriorityGroupAttachTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, meter_type=True, scheduling_type=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_STRICT)

            status = sai_thrift_set_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            schgroup_attr = sai_thrift_get_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=True)
            self.assertEqual(schgroup_attr['scheduler_profile_id'], sched)

            status = sai_thrift_set_scheduler_attribute(
                self.client, sched, scheduling_weight=4)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, scheduling_weight=True)
            self.assertEqual(sched_attr['scheduling_weight'], 4)

        finally:
            sai_thrift_set_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)

    def schedulerMinBwidthRateGroupAttachTest(self):
        '''
        Verify creation of scheduler with min bandwidth rate set and attaching
        it to a scheduler group
        '''
        print("\nschedulerMinBwidthRateGroupAttachTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                min_bandwidth_rate=100,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched,
                meter_type=True,
                min_bandwidth_rate=True,
                scheduling_type=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['min_bandwidth_rate'], 100)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_STRICT)

            status = sai_thrift_set_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            schgroup_attr = sai_thrift_get_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=True)
            self.assertEqual(schgroup_attr['scheduler_profile_id'], sched)

            status = sai_thrift_set_scheduler_attribute(
                self.client, sched, min_bandwidth_rate=200)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, min_bandwidth_rate=True)
            self.assertEqual(sched_attr['min_bandwidth_rate'], 200)

        finally:
            sai_thrift_set_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)

    def schedulerMaxBwidthRateGroupAttachTest(self):
        '''
        Verify creation of scheduler with max bandwidth rate set and attaching
        it to a scheduler group
        '''
        print("\nschedulerMaxBwidthRateGroupAttachTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                max_bandwidth_rate=1000,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched,
                meter_type=True,
                max_bandwidth_rate=True,
                scheduling_type=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['max_bandwidth_rate'], 1000)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_STRICT)

            status = sai_thrift_set_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            schgroup_attr = sai_thrift_get_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=True)
            self.assertEqual(schgroup_attr['scheduler_profile_id'], sched)

            status = sai_thrift_set_scheduler_attribute(
                self.client, sched, max_bandwidth_rate=2000)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, max_bandwidth_rate=True)
            self.assertEqual(sched_attr['max_bandwidth_rate'], 2000)

        finally:
            sai_thrift_set_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)

    def schedulerMinBwidthBurstRateGroupAttachTest(self):
        '''
        Verify creation of scheduler with min bandwidth burst rate set
        and attaching it to a scheduler group
        '''
        print("\nschedulerMinBwidthBurstRateGroupAttachTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                min_bandwidth_burst_rate=100,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched,
                meter_type=True,
                min_bandwidth_burst_rate=True,
                scheduling_type=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['min_bandwidth_burst_rate'], 100)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_STRICT)

            status = sai_thrift_set_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            schgroup_attr = sai_thrift_get_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=True)
            self.assertEqual(schgroup_attr['scheduler_profile_id'], sched)

            status = sai_thrift_set_scheduler_attribute(
                self.client, sched, min_bandwidth_burst_rate=200)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, min_bandwidth_burst_rate=True)
            self.assertEqual(sched_attr['min_bandwidth_burst_rate'], 200)

        finally:
            sai_thrift_set_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)

    def schedulerMaxBwidthBurstRateGroupAttachTest(self):
        '''
        Verify creation of scheduler with max bandwidth burst rate set
        and attaching it to a scheduler group
        '''
        print("\nschedulerMaxBwidthBurstRateGroupAttachTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                max_bandwidth_burst_rate=1000,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched,
                meter_type=True,
                max_bandwidth_burst_rate=True,
                scheduling_type=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['max_bandwidth_burst_rate'], 1000)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_STRICT)

            status = sai_thrift_set_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            schgroup_attr = sai_thrift_get_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=True)
            self.assertEqual(schgroup_attr['scheduler_profile_id'], sched)

            status = sai_thrift_set_scheduler_attribute(
                self.client, sched, max_bandwidth_burst_rate=2000)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client, sched, max_bandwidth_burst_rate=True)
            self.assertEqual(sched_attr['max_bandwidth_burst_rate'], 2000)

        finally:
            sai_thrift_set_scheduler_group_attribute(
                self.client, self.schgroup, scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)


@group("draft")
class SchedulerPortAttachTest(SaiHelper):
    '''
    Verify possibilities of attaching scheduler with different params set
    to a port
    '''

    def runTest(self):
        self.schedulerMaxBwidthRatePortAttachTest()
        self.schedulerMaxBwidthBurstRatePortAttachTest()

    def schedulerMaxBwidthRatePortAttachTest(self):
        '''
        Verify creation of scheduler with max bandwidth rate set and attaching
        it to a scheduler group
        '''
        print("\nschedulerMaxBwidthRatePortAttachTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                max_bandwidth_rate=1000,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched,
                meter_type=True,
                max_bandwidth_rate=True,
                scheduling_type=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['max_bandwidth_rate'], 1000)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_STRICT)

            status = sai_thrift_set_port_attribute(
                self.client, self.port1, qos_scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            port_attr = sai_thrift_get_port_attribute(
                self.client, self.port1, qos_scheduler_profile_id=True)
            self.assertEqual(port_attr['qos_scheduler_profile_id'], sched)

        finally:
            sai_thrift_set_port_attribute(
                self.client, self.port1, qos_scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)

    def schedulerMaxBwidthBurstRatePortAttachTest(self):
        '''
        Verify creation of scheduler with max bandwidth burst rate set
        and attaching it to a scheduler group
        '''
        print("\nschedulerMaxBwidthBurstRatePortAttachTest()")

        try:
            sched = sai_thrift_create_scheduler(
                self.client,
                meter_type=SAI_METER_TYPE_PACKETS,
                max_bandwidth_burst_rate=1000,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            sched_attr = sai_thrift_get_scheduler_attribute(
                self.client,
                sched,
                meter_type=True,
                max_bandwidth_burst_rate=True,
                scheduling_type=True)
            self.assertEqual(sched_attr['meter_type'], SAI_METER_TYPE_PACKETS)
            self.assertEqual(sched_attr['max_bandwidth_burst_rate'], 1000)
            self.assertEqual(sched_attr['scheduling_type'],
                             SAI_SCHEDULING_TYPE_STRICT)

            status = sai_thrift_set_port_attribute(
                self.client, self.port1, qos_scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            port_attr = sai_thrift_get_port_attribute(
                self.client, self.port1, qos_scheduler_profile_id=True)
            self.assertEqual(port_attr['qos_scheduler_profile_id'], sched)

        finally:
            sai_thrift_set_port_attribute(
                self.client, self.port1, qos_scheduler_profile_id=0)

            sai_thrift_remove_scheduler(self.client, sched)
