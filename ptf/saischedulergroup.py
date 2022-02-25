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
Thrift SAI interface Scheduler group tests
"""

from sai_thrift.sai_headers import *

from sai_base_test import *


@group("draft")
class SchGroupParamsTest(SaiHelper):
    '''
    Base scheduler group parameters tests
    '''

    def runTest(self):
        print("\nSchGroupParamsTest()")

        test_port = self.port1

        port_attr = sai_thrift_get_port_attribute(
            self.client, test_port, qos_number_of_scheduler_groups=True)
        schgroup_no = port_attr['qos_number_of_scheduler_groups']
        self.assertNotEqual(schgroup_no, 0)

        schgroup_list = sai_thrift_object_list_t(count=100)
        port_attr = sai_thrift_get_port_attribute(
            self.client, test_port, qos_scheduler_group_list=schgroup_list)
        schgroup_idlist = port_attr['qos_scheduler_group_list'].idlist
        self.assertEqual(len(schgroup_idlist), schgroup_no)

        test_schgroup = schgroup_idlist[1]

        schgroup_attr = sai_thrift_get_scheduler_group_attribute(
            self.client, test_schgroup, port_id=True)
        self.assertEqual(schgroup_attr['port_id'], test_port)

        schgroup_attr = sai_thrift_get_scheduler_group_attribute(
            self.client, test_schgroup, child_count=True)
        child_no = schgroup_attr['child_count']
        self.assertNotEqual(child_no, 0)

        schgroup_child_list = sai_thrift_object_list_t(count=100)
        schgroup_attr = sai_thrift_get_scheduler_group_attribute(
            self.client, test_schgroup, child_list=schgroup_child_list)
        child_idlist = schgroup_attr['child_list'].idlist
        self.assertEqual(len(child_idlist), child_no)

        try:
            sched = sai_thrift_create_scheduler(
                self.client, meter_type=SAI_METER_TYPE_PACKETS,
                scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
            self.assertNotEqual(sched, 0)

            status = sai_thrift_set_scheduler_group_attribute(
                self.client, test_schgroup, scheduler_profile_id=sched)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            schgroup_attr = sai_thrift_get_scheduler_group_attribute(
                self.client, test_schgroup, scheduler_profile_id=True)
            self.assertEqual(schgroup_attr['scheduler_profile_id'], sched)

        finally:
            sai_thrift_set_scheduler_group_attribute(
                self.client, test_schgroup, scheduler_profile_id=0)
            sai_thrift_remove_scheduler(self.client, sched)


@group("draft")
class SchGroupCreateFailTest(SaiHelper):
    '''
    Verify if scheduler group creation fails without mandatory parameters
    '''

    def runTest(self):
        test_port = self.port1

        sched = sai_thrift_create_scheduler(
            self.client, meter_type=SAI_METER_TYPE_PACKETS,
            scheduling_type=SAI_SCHEDULING_TYPE_STRICT)
        self.assertNotEqual(sched, 0)

        try:
            sched_group = sai_thrift_create_scheduler_group(
                self.client, port_id=test_port, scheduler_profile_id=sched)
            # Level is mandatory so creation should fail
            self.assertEqual(sched_group, 0)

            sched_group = sai_thrift_create_scheduler_group(
                self.client, level=0, scheduler_profile_id=sched)
            # Port is mandatory so creation should fail
            self.assertEqual(sched_group, 0)

            sched_group = sai_thrift_create_scheduler_group(
                self.client, port_id=test_port, level=0)
            self.assertNotEqual(sched_group, 0)

        finally:
            sai_thrift_remove_scheduler(self.client, sched_group)
            sai_thrift_remove_scheduler(self.client, sched)
