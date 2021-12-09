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
Thrift SAI interface tester
"""

from sai_thrift.sai_headers import *

from ptf.packet import *
from ptf.testutils import *

from sai_base_test import *


class FrameworkTester(SaiHelper):
    """
    Test auto-generated framework itself
    The intention of this test is to check if basic features works as expected
    """

    def setUp(self):
        super(FrameworkTester, self).setUp()

        self.route_entry_list = []
        self.nhop_list = []

        # test fdb creation
        self.fdb_entry = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address="00:11:22:33:44:55",
            bv_id=self.vlan10)
        status = sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port0_bp)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # test nhop creation
        self.nhop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port10_rif,
            ip=sai_ipaddress('10.10.10.1'))
        self.nhop_list.append(self.nhop)

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port10_rif,
            ip=sai_ipaddress('4444::1'))
        self.nhop_list.append(self.nhop1)

        # test neighbor creation
        self.neigh_entry = sai_thrift_neighbor_entry_t(
            self.switch_id, self.port10_rif, sai_ipaddress('10.10.10.1'))
        self.neigh = sai_thrift_create_neighbor_entry(
            self.client, self.neigh_entry, dst_mac_address='00:11:22:33:44:66')

        # test route creation
        self.route0 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('20.20.20.1/16'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route0, next_hop_id=self.nhop)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        self.route_entry_list.append(self.route0)

        self.route1 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('4441::1/64'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route1, next_hop_id=self.nhop)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        self.route_entry_list.append(self.route1)

        self.route2 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('4411::1/63'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route2, next_hop_id=self.nhop)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        self.route_entry_list.append(self.route2)

        self.route3 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('4423::1/65'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route3, next_hop_id=self.nhop)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        self.route_entry_list.append(self.route3)

        self.route4 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('4444::1/127'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client, self.route4, next_hop_id=self.nhop)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        self.route_entry_list.append(self.route4)

        # test qos map creation
        dscp_to_tc_1 = sai_thrift_qos_map_t(
            key=sai_thrift_qos_map_params_t(dscp=2),
            value=sai_thrift_qos_map_params_t(tc=16))
        dscp_to_tc_2 = sai_thrift_qos_map_t(
            key=sai_thrift_qos_map_params_t(dscp=4),
            value=sai_thrift_qos_map_params_t(tc=24))
        self.qos_map_list = sai_thrift_qos_map_list_t(
            count=2, maplist=[dscp_to_tc_1, dscp_to_tc_2])
        self.qos_map = sai_thrift_create_qos_map(
            self.client,
            SAI_QOS_MAP_TYPE_DSCP_TO_TC,
            map_to_value_list=self.qos_map_list)

    def runTest(self):
        attr = sai_thrift_get_fdb_entry_attribute(
            self.client, self.fdb_entry, bridge_port_id=True)
        self.assertEqual(attr['bridge_port_id'], self.port0_bp)

        for route in [self.route0, self.route1, self.route2,
                      self.route3, self.route4]:
            attr = sai_thrift_get_route_entry_attribute(
                self.client, route, next_hop_id=True)
            self.assertEqual(attr['next_hop_id'], self.nhop)

        attr = sai_thrift_get_next_hop_attribute(
            self.client, self.nhop, ip='0.0.0.0')
        self.assertEqual(attr['ip'].addr.ip4, '10.10.10.1')
        attr = sai_thrift_get_next_hop_attribute(
            self.client, self.nhop1, ip='0.0.0.0')
        self.assertEqual(attr['ip'].addr.ip6, '4444::1')

        attr = sai_thrift_get_neighbor_entry_attribute(
            self.client, self.neigh_entry, dst_mac_address=True)
        self.assertEqual(attr['dst_mac_address'], '00:11:22:33:44:66')

        attr = sai_thrift_get_qos_map_attribute(
            self.client,
            self.qos_map,
            map_to_value_list=sai_thrift_qos_map_list_t(
                count=2, maplist=[]))
        self.assertEqual(attr['map_to_value_list'].count,
                            self.qos_map_list.count)
        for i in range(0, self.qos_map_list.count):
            self.assertEqual(attr['map_to_value_list'].maplist[i].key.dscp,
                                self.qos_map_list.maplist[i].key.dscp)
            self.assertEqual(attr['map_to_value_list'].maplist[i].value.tc,
                                self.qos_map_list.maplist[i].value.tc)

    def tearDown(self):
        sai_thrift_remove_qos_map(self.client, self.qos_map)

        for route_entry in self.route_entry_list:
            sai_thrift_remove_route_entry(self.client, route_entry)

        sai_thrift_remove_neighbor_entry(self.client, self.neigh_entry)

        for nhop in self.nhop_list:
            sai_thrift_remove_next_hop(self.client, nhop)

        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry)

        super(FrameworkTester, self).tearDown()
