# Copyright 2021-present Barefoot Networks, Inc.
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
Thrift SAI interface ACL tests
"""
from __future__ import print_function

from bf_switcht_api_thrift.model_headers import *
from sai_thrift.sai_headers import *

from ptf.testutils import *
from ptf.packet import *
from ptf.thriftutils import *

from sai_base_test import *

THIS_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(THIS_DIR, '..'))
from common.utils import *  # noqa pylint: disable=wrong-import-position


class AclGroupTest(SaiHelper):
    '''
    Acl Group test class
    '''

    def setUp(self):
        super(AclGroupTest, self).setUp()

        self.mac_port = '00:11:22:33:44:55'
        self.mac_lag = '00:11:22:33:44:56'

        self.fdb_entry_port = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=self.mac_port,
            bv_id=self.vlan10)
        sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry_port,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port0_bp)

        self.fdb_entry_lag = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id,
            mac_address=self.mac_lag,
            bv_id=self.vlan10)
        sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry_lag,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.lag1_bp)

    def runTest(self):
        try:
            self.portLagIngressAclTableGroupTest()
            self.portLagEgressAclTableGroupTest()
        finally:
            pass

    def tearDown(self):
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry_port)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry_lag)

        super(AclGroupTest, self).tearDown()

    def portLagIngressAclTableGroupTest(self):
        '''
        Test verifying combination of port and LAG as
        bind points to ingress acl table group.
        '''
        print("portLagIngressAclTableGroupTest")

        # create ACL table group
        group_stage = SAI_ACL_STAGE_INGRESS
        group_bind_points = [SAI_ACL_BIND_POINT_TYPE_PORT,
                             SAI_ACL_BIND_POINT_TYPE_LAG]
        group_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(group_bind_points), int32list=group_bind_points)
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

        acl_group = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=group_stage,
            acl_bind_point_type_list=group_bind_point_type_list,
            type=group_type)

        # create ACL table
        table_stage = SAI_ACL_STAGE_INGRESS
        table_bind_points = [SAI_ACL_BIND_POINT_TYPE_PORT,
                             SAI_ACL_BIND_POINT_TYPE_LAG]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_points), int32list=table_bind_points)
        acl_table = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        # create ACL table entry
        src_ip = '10.0.0.1'
        src_ip_mask = '255.255.255.255'
        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=src_ip),
            mask=sai_thrift_acl_field_data_mask_t(ip4=src_ip_mask))

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))
        acl_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table,
            priority=10,
            field_src_ip=src_ip_t,
            action_packet_action=packet_action)

        # add ACL table group member
        member1 = sai_thrift_create_acl_table_group_member(
            self.client,
            acl_table_group_id=acl_group,
            acl_table_id=acl_table)

        # create ACL counter
        acl_counter = sai_thrift_create_acl_counter(
            self.client, table_id=acl_table)

        # attach ACL counter to ACL entry
        action_counter_t = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                oid=acl_counter),
            enable=True)
        sai_thrift_set_acl_entry_attribute(
            self.client, acl_entry,
            action_counter=action_counter_t)

        try:
            pkt1 = simple_udp_packet(
                eth_dst=self.mac_port,
                eth_src=self.mac_lag,
                ip_src=src_ip,
                pktlen=100)
            pkt2 = simple_udp_packet(
                eth_dst=self.mac_lag,
                eth_src=self.mac_port,
                ip_src=src_ip,
                pktlen=100)

            print("Sending packet without acl table group")
            print("Sending packet from lag to port")
            send_packet(self, self.dev_port4, pkt1)
            verify_packet(self, pkt1, self.dev_port0)

            print("Sending packet from port to lag")
            send_packet(self, self.dev_port0, pkt2)
            verify_any_packet_any_port(
                self, [pkt2, pkt2, pkt2],
                [self.dev_port4, self.dev_port5, self.dev_port6])

            packets = sai_thrift_get_acl_counter_attribute(
                self.client, acl_counter, packets=True)
            self.assertEqual(packets['packets'], 0)

            print("Attach acl table group to port")
            sai_thrift_set_port_attribute(self.client, self.port0,
                                          ingress_acl=acl_group)
            print("Sending packet from port to lag, drop")
            send_packet(self, self.dev_port0, pkt2)
            verify_no_other_packets(self)

            packets = sai_thrift_get_acl_counter_attribute(
                self.client, acl_counter, packets=True)
            self.assertEqual(packets['packets'], 1)

            print("Attach acl table group to lag")
            sai_thrift_set_lag_attribute(self.client, self.lag1,
                                         ingress_acl=acl_group)
            print("Sending packet from lag to port, drop")
            send_packet(self, self.dev_port4, pkt1)
            verify_no_other_packets(self)

            packets = sai_thrift_get_acl_counter_attribute(
                self.client, acl_counter, packets=True)
            self.assertEqual(packets['packets'], 2)

        finally:
            action_counter_t = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    oid=0),
                enable=True)
            sai_thrift_set_acl_entry_attribute(
                self.client, acl_entry,
                action_counter=action_counter_t)
            sai_thrift_remove_acl_counter(self.client, acl_counter)

            sai_thrift_set_port_attribute(self.client, self.port0,
                                          ingress_acl=0)
            sai_thrift_set_lag_attribute(self.client, self.lag1,
                                         ingress_acl=0)

            sai_thrift_remove_acl_table_group_member(self.client, member1)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)
            sai_thrift_remove_acl_table_group(self.client, acl_group)

    def portLagEgressAclTableGroupTest(self):
        '''
        Test verifying combination of port and LAG as
        bind points to egress acl table group.
        '''
        print("portLagEgressAclTableGroupTest")
        # create ACL table group
        group_stage = SAI_ACL_STAGE_EGRESS
        group_bind_points = [SAI_ACL_BIND_POINT_TYPE_PORT,
                             SAI_ACL_BIND_POINT_TYPE_LAG]
        group_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(group_bind_points), int32list=group_bind_points)
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

        acl_group = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=group_stage,
            acl_bind_point_type_list=group_bind_point_type_list,
            type=group_type)

        # create ACL table
        table_stage = SAI_ACL_STAGE_EGRESS
        table_bind_points = [SAI_ACL_BIND_POINT_TYPE_PORT,
                             SAI_ACL_BIND_POINT_TYPE_LAG]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_points), int32list=table_bind_points)
        acl_table = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        # create ACL table entry
        src_ip = '10.0.0.1'
        src_ip_mask = '255.255.255.255'
        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=src_ip),
            mask=sai_thrift_acl_field_data_mask_t(ip4=src_ip_mask))

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))
        acl_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table,
            priority=10,
            field_src_ip=src_ip_t,
            action_packet_action=packet_action)

        # add ACL table group member
        member1 = sai_thrift_create_acl_table_group_member(
            self.client,
            acl_table_group_id=acl_group,
            acl_table_id=acl_table)

        # create ACL counter
        acl_counter = sai_thrift_create_acl_counter(
            self.client, table_id=acl_table)

        # attach ACL counter to ACL entry
        action_counter_t = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                oid=acl_counter),
            enable=True)
        sai_thrift_set_acl_entry_attribute(
            self.client, acl_entry,
            action_counter=action_counter_t)

        try:
            pkt1 = simple_udp_packet(
                eth_dst=self.mac_port,
                eth_src=self.mac_lag,
                ip_src=src_ip,
                pktlen=100)
            pkt2 = simple_udp_packet(
                eth_dst=self.mac_lag,
                eth_src=self.mac_port,
                ip_src=src_ip,
                pktlen=100)

            print("Sending packet without acl table group")
            print("Sending packet from lag to port")
            send_packet(self, self.dev_port4, pkt1)
            verify_packet(self, pkt1, self.dev_port0)

            print("Sending packet from port to lag")
            send_packet(self, self.dev_port0, pkt2)
            verify_any_packet_any_port(
                self, [pkt2, pkt2, pkt2],
                [self.dev_port4, self.dev_port5, self.dev_port6])

            packets = sai_thrift_get_acl_counter_attribute(
                self.client, acl_counter, packets=True)
            self.assertEqual(packets['packets'], 0)

            print("Attach acl table group to port")
            sai_thrift_set_port_attribute(self.client, self.port0,
                                          egress_acl=acl_group)
            print("Sending packet from lag to port, drop")
            send_packet(self, self.dev_port4, pkt1)
            verify_no_other_packets(self)

            packets = sai_thrift_get_acl_counter_attribute(
                self.client, acl_counter, packets=True)
            self.assertEqual(packets['packets'], 1)

            print("Attach acl table group to lag")
            sai_thrift_set_lag_attribute(self.client, self.lag1,
                                         egress_acl=acl_group)
            print("Sending packet from port to lag, drop")
            send_packet(self, self.dev_port0, pkt2)
            verify_no_other_packets(self)

            packets = sai_thrift_get_acl_counter_attribute(
                self.client, acl_counter, packets=True)
            self.assertEqual(packets['packets'], 2)

        finally:
            action_counter_t = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    oid=0),
                enable=True)
            sai_thrift_set_acl_entry_attribute(
                self.client, acl_entry,
                action_counter=action_counter_t)
            sai_thrift_remove_acl_counter(self.client, acl_counter)

            sai_thrift_set_port_attribute(self.client, self.port0,
                                          egress_acl=0)
            sai_thrift_set_lag_attribute(self.client, self.lag1,
                                         egress_acl=0)

            sai_thrift_remove_acl_table_group_member(self.client, member1)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)
            sai_thrift_remove_acl_table_group(self.client, acl_group)


@group('acl')
@group('acl-ocp')
class SrcIpAclTest(SaiHelper):
    """
    Matching on src ip address field
    """

    def runTest(self):
        print("Testing SrcIpAclTest")

        print('--------------------------------------------------------------')
        print("Sending packet ptf_intf 2 -> ptf_intf 1 (192.168.100.100 "
              "--->172.16.10.1 [id = 105])")

        l4_src_port = 1000

        rif_id1 = self.port10_rif
        rif_id2 = self.port11_rif

        ip_addr_subnet = '172.16.10.0'
        ip_addr = '172.16.10.1'
        dmac = '00:11:22:33:44:55'
        mac_src = '00:22:22:22:22:22'
        ip_addr_src = '192.168.100.100'

        print("Creates nieghbor with %s ip address, %d router interface"
              " id and %s destination mac" % (
                  ip_addr, rif_id1, dmac))
        nbr_entry = sai_thrift_neighbor_entry_t(
            rif_id=rif_id1,
            ip_address=sai_ipaddress(ip_addr))
        sai_thrift_create_neighbor_entry(
            self.client, nbr_entry, dst_mac_address=dmac)

        print("Creates nhop with %s ip address and %d router"
              " interface id" % (ip_addr, rif_id1))
        nhop = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(ip_addr),
            router_interface_id=rif_id1,
            type=SAI_NEXT_HOP_TYPE_IP)

        print("Creates route with %s ip prefix and %d router"
              " interface id" % (ip_addr_subnet, rif_id1))
        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(
                ip_addr_subnet))
        sai_thrift_create_route_entry(self.client, route_entry,
                                      next_hop_id=rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=mac_src,
                                ip_dst=ip_addr,
                                ip_src=ip_addr_src,
                                tcp_sport=l4_src_port,
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst=dmac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=ip_addr,
                                    ip_src=ip_addr_src,
                                    tcp_sport=l4_src_port,
                                    ip_id=105,
                                    ip_ttl=63)

        try:
            print('#### NO ACL Applied ####')
            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 172.16'
                  '.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| 172.16'
                  '.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            verify_packets(self, exp_pkt, [self.dev_port10])
        finally:
            print('----------------------------------------------------------'
                  '------------------------------------')

        print("Sending packet ptf_intf 2 -[acl]-> ptf_intf 1 (192.168.0.1"
              "-[acl]-> 172.16.10.1 [id = 105])")
        # setup ACL to block based on Source IP and SPORT

        table_stage_ingress = SAI_ACL_STAGE_INGRESS
        table_stage_egress = SAI_ACL_STAGE_EGRESS
        entry_priority = 1
        ip_src = "192.168.100.1"
        ip_src_mask = "255.255.255.0"

        table_bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_points), int32list=table_bind_points)

        acl_ingress_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_ingress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        self.assertNotEqual(acl_ingress_table_id, 0)

        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ip_src),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ip_src_mask))

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        u32range = sai_thrift_u32_range_t(min=1000, max=1000)
        acl_range_id = sai_thrift_create_acl_range(
            self.client,
            type=SAI_ACL_RANGE_TYPE_L4_SRC_PORT_RANGE,
            limit=u32range)
        range_list = [acl_range_id]

        range_list_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(
                objlist=sai_thrift_object_list_t(
                    count=len(range_list),
                    idlist=range_list)))

        acl_ingress_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_table_id,
            priority=entry_priority,
            field_src_ip=src_ip_t,
            action_packet_action=packet_action,
            field_acl_range_type=range_list_t)

        self.assertNotEqual(acl_ingress_entry_id, 0)

        # bind this ACL table to rif_id2s object id
        sai_thrift_set_router_interface_attribute(
            self.client, rif_id2, ingress_acl=acl_ingress_table_id)

        try:
            print('#### ACL \'DROP, src ip 192.168.100.1/255.255.255.0, SPORT'
                  ' 1000, in_ports[ptf_intf_1,2]\' Applied ####')
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            send_packet(self, self.dev_port11, pkt)
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 0')
            verify_no_other_packets(self, timeout=1)

            # unbind this ACL table from rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, ingress_acl=int(SAI_NULL_OBJECT_ID))

            # cleanup ACL
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_ingress_table_id)

            acl_egress_table_id = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage_egress,
                acl_bind_point_type_list=table_bind_point_type_list,
                field_src_ip=True)

            self.assertNotEqual(acl_egress_table_id, 0)

            acl_egress_entry_id = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_egress_table_id,
                priority=entry_priority,
                field_src_ip=src_ip_t,
                action_packet_action=packet_action,
                field_acl_range_type=range_list_t)

            self.assertNotEqual(acl_egress_entry_id, 0)

            # bind this ACL table to rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, egress_acl=acl_egress_table_id)

            print('#### ACL \'DROP, src ip 192.168.100.1/255.255.255.0, SPORT'
                  ' 1000, in_ports[ptf_intf_1,2]\' Applied ####')
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            send_packet(self, self.dev_port11, pkt)
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 0')
            verify_no_other_packets(self, timeout=1)

        finally:
            # unbind this ACL table from rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, egress_acl=int(SAI_NULL_OBJECT_ID))

            # cleanup ACL
            sai_thrift_remove_acl_entry(self.client, acl_egress_entry_id)
            sai_thrift_remove_acl_range(self.client, acl_range_id)
            sai_thrift_remove_acl_table(self.client, acl_egress_table_id)

            # cleanup
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry)


@group('acl')
@group('acl-ocp')
class DstIpAclTest(SaiHelper):
    """
    Matching on dst ip address field
    """

    def runTest(self):
        print("Testing DstIpAclTest")

        print('--------------------------------------------------------------'
              '--------------------------------')
        print("Sending packet ptf_intf 2 -> ptf_intf 1 (192.168.100.100 "
              "--->172.16.10.1 [id = 105])")

        l4_dst_port = 1000

        rif_id1 = self.port10_rif
        rif_id2 = self.port11_rif

        ip_addr_subnet = '172.16.10.0'
        ip_addr = '172.16.10.1'
        dmac = '00:11:22:33:44:55'
        mac_src = '00:22:22:22:22:22'
        ip_addr_src = '192.168.100.100'

        print("Creates nieghbor with %s ip address, %d router interface"
              " id and %s destination mac" % (
                  ip_addr, rif_id1, dmac))
        nbr_entry = sai_thrift_neighbor_entry_t(
            rif_id=rif_id1,
            ip_address=sai_ipaddress(ip_addr))
        sai_thrift_create_neighbor_entry(
            self.client, nbr_entry, dst_mac_address=dmac)

        print("Creates nhop with %s ip address and %d router"
              " interface id" % (ip_addr, rif_id1))
        nhop = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(ip_addr),
            router_interface_id=rif_id1,
            type=SAI_NEXT_HOP_TYPE_IP)

        print("Creates route with %s ip prefix and %d router"
              " interface id" % (ip_addr_subnet, rif_id1))
        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(
                ip_addr_subnet))
        sai_thrift_create_route_entry(self.client, route_entry,
                                      next_hop_id=rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=mac_src,
                                ip_dst=ip_addr,
                                ip_src=ip_addr_src,
                                tcp_sport=l4_dst_port,
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst=dmac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=ip_addr,
                                    ip_src=ip_addr_src,
                                    tcp_sport=l4_dst_port,
                                    ip_id=105,
                                    ip_ttl=63)

        try:
            print('#### NO ACL Applied ####')
            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 172.16'
                  '.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| 172.16'
                  '.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            verify_packets(self, exp_pkt, [self.dev_port10])
        finally:
            print('----------------------------------------------------------')

        print("Sending packet ptf_intf 2 -[acl]-> ptf_intf 1 (192.168.0.1"
              "-[acl]-> 172.16.10.1 [id = 105])")
        # setup ACL to block based on Source IP and SPORT

        table_stage_ingress = SAI_ACL_STAGE_INGRESS
        table_stage_egress = SAI_ACL_STAGE_EGRESS
        entry_priority = 1
        ip_dst = "172.16.10.1"
        ip_dst_mask = "255.255.255.0"

        table_bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_points), int32list=table_bind_points)

        acl_ingress_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_ingress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        self.assertNotEqual(acl_ingress_table_id, 0)

        dst_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ip_dst),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ip_dst_mask))

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        u32range = sai_thrift_u32_range_t(min=1000, max=1000)
        acl_range_id = sai_thrift_create_acl_range(
            self.client,
            type=SAI_ACL_RANGE_TYPE_L4_SRC_PORT_RANGE,
            limit=u32range)
        range_list = [acl_range_id]

        range_list_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(
                objlist=sai_thrift_object_list_t(
                    count=len(range_list),
                    idlist=range_list)))

        acl_ingress_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_table_id,
            priority=entry_priority,
            field_dst_ip=dst_ip_t,
            action_packet_action=packet_action,
            field_acl_range_type=range_list_t)

        self.assertNotEqual(acl_ingress_entry_id, 0)

        # bind this ACL table to rif_id2s object id
        sai_thrift_set_router_interface_attribute(
            self.client, rif_id2, ingress_acl=acl_ingress_table_id)

        try:
            print('#### ACL \'DROP, src ip 192.168.100.1/255.255.255.0, SPORT'
                  ' 1000, in_ports[ptf_intf_1,2]\' Applied ####')
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            send_packet(self, self.dev_port11, pkt)
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 0')
            verify_no_other_packets(self, timeout=1)

            # unbind this ACL table from rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, ingress_acl=int(SAI_NULL_OBJECT_ID))

            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_ingress_table_id)

            acl_egress_table_id = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage_egress,
                acl_bind_point_type_list=table_bind_point_type_list,
                field_dst_ip=True)

            self.assertNotEqual(acl_egress_table_id, 0)

            acl_egress_entry_id = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_egress_table_id,
                priority=entry_priority,
                field_dst_ip=dst_ip_t,
                action_packet_action=packet_action,
                field_acl_range_type=range_list_t)

            self.assertNotEqual(acl_egress_entry_id, 0)

            # bind this ACL table to rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, egress_acl=acl_egress_table_id)

            print('#### ACL \'DROP, src ip 192.168.100.1/255.255.255.0, SPORT'
                  ' 1000, in_ports[ptf_intf_1,2]\' Applied ####')
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            send_packet(self, self.dev_port11, pkt)
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 0')
            verify_no_other_packets(self, timeout=1)

        finally:
            # unbind this ACL table from rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, egress_acl=int(SAI_NULL_OBJECT_ID))

            # cleanup ACL
            sai_thrift_remove_acl_entry(self.client, acl_egress_entry_id)
            sai_thrift_remove_acl_range(self.client, acl_range_id)
            sai_thrift_remove_acl_table(self.client, acl_egress_table_id)

            # cleanup
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry)


@group('acl')
@group('acl-ocp')
class MACSrcAclTest(SaiHelper):
    """
    Matching on src mac address field
    """

    def runTest(self):
        print("This test is not supported. MAC lookup cannot be done for IP "
              "Packets in switch.p4")
        # noqa pylint: disable=unreachable
        return

        print('--------------------------------------------------------------')
        print("Sending packet ptf_intf 2 -> ptf_intf 1 (192.168.0.1 --->"
              " 172.16.10.1 [id = 105])")

        rif_id1 = self.port10_rif
        rif_id2 = self.port11_rif

        ip_addr_subnet = '172.16.10.0'
        ip_addr = '172.16.10.1'
        dmac = '00:11:22:33:44:55'
        mac_src = '00:22:22:22:22:22'
        ip_addr_src = '192.168.0.1'

        print("Creates nieghbor with %s ip address, %d router interface"
              " id and %s destination mac" % (
                  ip_addr, rif_id1, dmac))
        nbr_entry = sai_thrift_neighbor_entry_t(
            rif_id=rif_id1,
            ip_address=sai_ipaddress(ip_addr))
        sai_thrift_create_neighbor_entry(
            self.client, nbr_entry, dst_mac_address=dmac)

        print("Creates nhop with %s ip address and %d router"
              " interface id" % (ip_addr, rif_id1))
        nhop = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(ip_addr),
            router_interface_id=rif_id1,
            type=SAI_NEXT_HOP_TYPE_IP)

        print("Creates route with %s ip prefix and %d router"
              " interface id" % (ip_addr_subnet, rif_id1))
        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(
                ip_addr_subnet))
        sai_thrift_create_route_entry(self.client, route_entry,
                                      next_hop_id=rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=mac_src,
                                ip_dst=ip_addr,
                                ip_src=ip_addr_src,
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst=dmac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=ip_addr,
                                    ip_src=ip_addr_src,
                                    ip_id=105,
                                    ip_ttl=63)

        try:
            print('#### NO ACL Applied ####')
            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 172.16'
                  '.10.1 | 192.168.100.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| 172.16'
                  '.10.1 | 192.168.100.1 | @ ptf_intf 1')
            verify_packets(self, exp_pkt, [self.dev_port10])
        finally:
            print('----------------------------------------------------------')

            print("Sending packet ptf_intf 2 -[acl]-> ptf_intf 1 (192.168.0.1-"
                  "[acl]-> 172.16.10.1 [id = 105])")
            # setup ACL to block based on Source MAC

            table_stage_ingress = SAI_ACL_STAGE_INGRESS
            table_stage_egress = SAI_ACL_STAGE_EGRESS
            entry_priority = 1
            mac_src_mask = 'ff:ff:ff:ff:ff:ff'

            table_bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF]
            table_bind_point_type_list = sai_thrift_s32_list_t(
                count=len(table_bind_points), int32list=table_bind_points)

            acl_ingress_table_id = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage_ingress,
                acl_bind_point_type_list=table_bind_point_type_list,
                field_src_mac=True)

            self.assertNotEqual(acl_ingress_table_id, 0)

            src_mac_t = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(mac=mac_src),
                mask=sai_thrift_acl_field_data_mask_t(mac=mac_src_mask))

            packet_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    s32=SAI_PACKET_ACTION_DROP))

            u32range = sai_thrift_u32_range_t(min=1000, max=1000)
            acl_range_id = sai_thrift_create_acl_range(
                self.client, type=SAI_ACL_RANGE_TYPE_L4_SRC_PORT_RANGE,
                limit=u32range)
            range_list = [acl_range_id]

            range_list_t = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(
                    objlist=sai_thrift_object_list_t(
                        count=len(range_list),
                        idlist=range_list)))

            acl_ingress_entry_id = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_ingress_table_id,
                priority=entry_priority,
                field_src_mac=src_mac_t,
                action_packet_action=packet_action,
                field_acl_range_type=range_list_t)

            self.assertNotEqual(acl_ingress_entry_id, 0)

            # bind this ACL table to rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, ingress_acl=acl_ingress_table_id)

        try:
            print('#### ACL \'DROP, src mac 00:22:22:22:22:22, '
                  'in_ports[ptf_intf_1,2]\' Applied ####')
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 2')
            # send the same packet
            send_packet(self, self.dev_port11, pkt)
            # ensure packet is dropped
            # check for absence of packet here!
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC,
                  '| 172.16.10.1 | 192.168.0.1 | @ ptf_intf 1')
            verify_no_other_packets(self, timeout=2)

            # unbind this ACL table from rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, ingress_acl=int(SAI_NULL_OBJECT_ID))

            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_ingress_table_id)

            acl_egress_table_id = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage_egress,
                acl_bind_point_type_list=table_bind_point_type_list,
                field_src_mac=True)

            self.assertNotEqual(acl_egress_table_id, 0)

            acl_egress_entry_id = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_egress_table_id,
                priority=entry_priority,
                field_src_mac=src_mac_t,
                action_packet_action=packet_action,
                field_acl_range_type=range_list_t)

            self.assertNotEqual(acl_egress_entry_id, 0)

            print('#### ACL \'DROP, src mac 00:22:22:22:22:22, '
                  'in_ports[ptf_intf_1,2]\' Applied ####')
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 2')
            # send the same packet
            send_packet(self, self.dev_port11, pkt)
            # ensure packet is dropped
            # check for absence of packet here!
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC,
                  '| 172.16.10.1 | 192.168.0.1 | @ ptf_intf 1')
            verify_no_other_packets(self, timeout=2)

        finally:
            # unbind this ACL table from rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, egress_acl=int(SAI_NULL_OBJECT_ID))

            # cleanup ACL
            sai_thrift_remove_acl_entry(self.client, acl_egress_entry_id)
            sai_thrift_remove_acl_range(self.client, acl_range_id)
            sai_thrift_remove_acl_table(self.client, acl_egress_table_id)

            # cleanup
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry)


@group('acl')
@group('acl-ocp')
class L3L4PortTest(SaiHelper):
    """
    Tests matching on l4_dst_port and l4_src_port fields
    """

    def runTest(self):
        print("Testing L4 src/dest port acl filter")
        print('--------------------------------------------------------------'
              '--------------------------------')
        print("Sending packet ptf_intf 2 -> ptf_intf 1 (192.168.100.100 --->"
              " 172.16.10.1 [id = 105])")

        l4_dst_port = 1000
        l4_src_port = 500

        rif_id1 = self.port10_rif
        rif_id2 = self.port11_rif

        ip_addr_subnet = '172.16.10.0'
        ip_addr = '172.16.10.1'
        dmac = '00:11:22:33:44:55'
        mac_src = '00:22:22:22:22:22'
        ip_addr_src = '192.168.100.100'

        print("Creates nieghbor with %s ip address, %d router interface"
              " id and %s destination mac" % (
                  ip_addr, rif_id1, dmac))
        nbr_entry = sai_thrift_neighbor_entry_t(
            rif_id=rif_id1,
            ip_address=sai_ipaddress(ip_addr))
        sai_thrift_create_neighbor_entry(
            self.client, nbr_entry, dst_mac_address=dmac)

        print("Creates nhop with %s ip address and %d router"
              " interface id" % (ip_addr, rif_id1))
        nhop = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(ip_addr),
            router_interface_id=rif_id1,
            type=SAI_NEXT_HOP_TYPE_IP)

        print("Creates route with %s ip prefix and %d router"
              " interface id" % (ip_addr_subnet, rif_id1))
        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(
                ip_addr_subnet))
        sai_thrift_create_route_entry(self.client, route_entry,
                                      next_hop_id=rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=mac_src,
                                ip_dst=ip_addr,
                                ip_src=ip_addr_src,
                                tcp_sport=l4_src_port,
                                tcp_dport=l4_dst_port,
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst=dmac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=ip_addr,
                                    ip_src=ip_addr_src,
                                    tcp_sport=l4_src_port,
                                    tcp_dport=l4_dst_port,
                                    ip_id=105,
                                    ip_ttl=63)
        try:
            print('#### NO ACL Applied ####')
            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 172.'
                  '16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| 172.'
                  '16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            verify_packets(self, exp_pkt, [self.dev_port10])
        finally:
            print('----------------------------------------------------------'
                  '------------------------------------')

        print("Sending packet ptf_intf 2 -[acl]-> ptf_intf 1 (192.168.0.1"
              "-[a]cl]-> 172.16.10.1 [id = 105])")
        # setup ACL to block based on Source IP and SPORT
        table_stage_ingress = SAI_ACL_STAGE_INGRESS
        table_stage_egress = SAI_ACL_STAGE_EGRESS
        entry_priority = 1

        ip_src = "192.168.100.1"
        ip_src_mask = "255.255.255.0"

        table_bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_points), int32list=table_bind_points)

        acl_ingress_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_ingress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        self.assertNotEqual(acl_ingress_table_id, 0)

        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ip_src),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ip_src_mask))

        l4_dst_port_mask = 32759
        l4_dst_port_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u16=l4_dst_port),
            mask=sai_thrift_acl_field_data_mask_t(u16=l4_dst_port_mask))

        l4_src_port_mask = 32759
        l4_src_port_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u16=l4_src_port),
            mask=sai_thrift_acl_field_data_mask_t(u16=l4_src_port_mask))

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        acl_ingress_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_table_id,
            priority=entry_priority,
            field_src_ip=src_ip_t,
            action_packet_action=packet_action,
            field_l4_dst_port=l4_dst_port_t,
            field_l4_src_port=l4_src_port_t)

        self.assertNotEqual(acl_ingress_entry_id, 0)

        # bind this ACL table to rif_id2s object id
        sai_thrift_set_router_interface_attribute(
            self.client, rif_id2, ingress_acl=acl_ingress_table_id)

        try:
            print('#### ACL \'DROP, src ip 192.168.100.1/255.255.255.0, SPORT'
                  ' 1000, in_ports[ptf_intf_1,2]\' Applied ####')
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            # send the same packet
            send_packet(self, self.dev_port11, pkt)
            # ensure packet is dropped
            # check for absence of packet here!
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 0')
            verify_no_other_packets(self, timeout=1)

            # unbind this ACL table from rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, ingress_acl=int(SAI_NULL_OBJECT_ID))

            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_ingress_table_id)

            acl_egress_table_id = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage_egress,
                acl_bind_point_type_list=table_bind_point_type_list,
                field_src_ip=True)

            self.assertNotEqual(acl_egress_table_id, 0)

            acl_egress_entry_id = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_egress_table_id,
                priority=entry_priority,
                field_src_ip=src_ip_t,
                action_packet_action=packet_action,
                field_l4_dst_port=l4_dst_port_t,
                field_l4_src_port=l4_src_port_t)

            self.assertNotEqual(acl_egress_entry_id, 0)

            # bind this ACL table to rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, egress_acl=acl_egress_table_id)

        finally:
            # unbind this ACL table from rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, egress_acl=int(SAI_NULL_OBJECT_ID))

            # cleanup ACL
            sai_thrift_remove_acl_entry(self.client, acl_egress_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_egress_table_id)

            # cleanup
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry)


@group('acl')
@group('acl-ocp')
class L3AclRangeTest(SaiHelper):
    """
    Matching on acl range
    """

    def runTest(self):
        print('--------------------------------------------------------------')
        print("Sending packet ptf_intf 2 -> ptf_intf 1 (192.168.100.100 ---> "
              "172.16.10.1 [id = 105])")

        l4_dst_port = 1000

        rif_id1 = self.port10_rif
        rif_id2 = self.port11_rif

        ip_addr_subnet = '172.16.10.0'
        ip_addr = '172.16.10.1'
        dmac = '00:11:22:33:44:55'
        mac_src = '00:22:22:22:22:22'
        ip_addr_src = '192.168.100.100'

        print("Creates nieghbor with %s ip address, %d router interface"
              " id and %s destination mac" % (
                  ip_addr, rif_id1, dmac))
        nbr_entry = sai_thrift_neighbor_entry_t(
            rif_id=rif_id1,
            ip_address=sai_ipaddress(ip_addr))
        sai_thrift_create_neighbor_entry(
            self.client, nbr_entry, dst_mac_address=dmac)

        print("Creates nhop with %s ip address and %d router"
              " interface id" % (ip_addr, rif_id1))
        nhop = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(ip_addr),
            router_interface_id=rif_id1,
            type=SAI_NEXT_HOP_TYPE_IP)

        print("Creates route with %s ip prefix and %d router"
              " interface id" % (ip_addr_subnet, rif_id1))
        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(
                ip_addr_subnet))
        sai_thrift_create_route_entry(self.client, route_entry,
                                      next_hop_id=rif_id1)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=mac_src,
                                ip_dst=ip_addr,
                                ip_src=ip_addr_src,
                                tcp_dport=l4_dst_port,
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst=dmac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=ip_addr,
                                    ip_src=ip_addr_src,
                                    tcp_dport=l4_dst_port,
                                    ip_id=105,
                                    ip_ttl=63)

        try:
            print('#### NO ACL Applied ####')
            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 172.16'
                  '.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| 172.16'
                  '.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            verify_packets(self, exp_pkt, [self.dev_port10])
        finally:
            print('----------------------------------------------------------')

        print("Sending packet ptf_intf 2 -[acl]-> ptf_intf 1 (192.168.0.1-"
              "[acl]-> 172.16.10.1 [id = 105])")
        # setup ACL to block based on Source IP and SPORT
        table_stage_ingress = SAI_ACL_STAGE_INGRESS
        table_stage_egress = SAI_ACL_STAGE_EGRESS
        entry_priority = 1

        table_bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_points), int32list=table_bind_points)

        acl_ingress_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_ingress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        self.assertNotEqual(acl_ingress_table_id, 0)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        u32range = sai_thrift_u32_range_t(min=1000, max=1000)
        acl_range_id = sai_thrift_create_acl_range(
            self.client,
            type=SAI_ACL_RANGE_TYPE_L4_DST_PORT_RANGE,
            limit=u32range)
        range_list = [acl_range_id]
        print("ACL range created 0x%lx" % (acl_range_id))

        range_list_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(
                objlist=sai_thrift_object_list_t(
                    count=len(range_list),
                    idlist=range_list)))

        acl_ingress_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_table_id,
            priority=entry_priority,
            action_packet_action=packet_action,
            field_acl_range_type=range_list_t)
        print("ACL igress table created 0x%lx" % (acl_ingress_table_id))

        self.assertNotEqual(acl_ingress_entry_id, 0)

        # bind this ACL table to rif_id2s object id
        sai_thrift_set_router_interface_attribute(
            self.client, rif_id2, ingress_acl=acl_ingress_table_id)

        try:
            print('#### ACL \'DROP, src ip 192.168.100.1/255.255.255.0, SPORT'
                  ' 1000, in_ports[ptf_intf_1,2]\' Applied ####')
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            # send the same packet
            send_packet(self, self.dev_port11, pkt)
            # ensure packet is dropped
            # check for absence of packet here!
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 0')
            verify_no_other_packets(self, timeout=1)

            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, ingress_acl=int(SAI_NULL_OBJECT_ID))

            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_ingress_table_id)

            acl_egress_table_id = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage_egress,
                acl_bind_point_type_list=table_bind_point_type_list,
                field_src_ip=True)

            self.assertNotEqual(acl_egress_table_id, 0)

            acl_egress_entry_id = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_egress_table_id,
                priority=entry_priority,
                action_packet_action=packet_action,
                field_acl_range_type=range_list_t)
            print("ACL egress table created 0x%lx" % (acl_egress_table_id))

            self.assertNotEqual(acl_egress_entry_id, 0)

            # bind this ACL table to rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, egress_acl=acl_egress_table_id)

            print('#### ACL \'DROP, src ip 192.168.100.1/255.255.255.0, SPORT'
                  ' 1000, in_ports[ptf_intf_1,2]\' Applied ####')
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            # send the same packet
            send_packet(self, self.dev_port11, pkt)
            # ensure packet is dropped
            # check for absence of packet here!
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 0')
            verify_no_other_packets(self, timeout=1)

        finally:
            # unbind this ACL table from rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, rif_id2, egress_acl=int(SAI_NULL_OBJECT_ID))

            # cleanup ACL
            sai_thrift_remove_acl_entry(self.client, acl_egress_entry_id)
            sai_thrift_remove_acl_range(self.client, acl_range_id)
            sai_thrift_remove_acl_table(self.client, acl_egress_table_id)

            # cleanup
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry)


@group('acl-m1')
class ACLGroupSeveralMembersTest(SaiHelper):
    """
    Tests matching on ACL groups
    """

    def runTest(self):

        mirror_type = SAI_MIRROR_SESSION_TYPE_LOCAL

        rif_id1 = self.port10_rif

        ipv4_addr = '192.168.0.1'
        ipv4_mask = '255.255.255.255'
        ipv6_addr = '4000::1'
        ipv6_addr_src = '2000::1'
        ipv6_mask = "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"
        dmac = '00:22:22:22:22:22'
        ipv4_addr_src1 = "20.0.0.1"
        ipv4_addr_src2 = "20.0.0.3"

        print("Creates nieghbor with %s ip address, %d router interface"
              " id and %s destination mac" % (
                  ipv4_addr, rif_id1, dmac))
        nbr_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=rif_id1,
            ip_address=sai_ipaddress(ipv4_addr))
        sai_thrift_create_neighbor_entry(
            self.client, nbr_entry1, dst_mac_address=dmac)

        print("Creates nhop with %s ip address and %d router"
              " interface id" % (ipv4_addr, rif_id1))
        nhop1 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(ipv4_addr),
            router_interface_id=rif_id1,
            type=SAI_NEXT_HOP_TYPE_IP)

        print("Creates nieghbor with %s ip address, %d router interface"
              " id and %s destination mac" % (
                  ipv6_addr, rif_id1, dmac))
        nbr_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=rif_id1,
            ip_address=sai_ipaddress(ipv6_addr))
        sai_thrift_create_neighbor_entry(
            self.client, nbr_entry2, dst_mac_address=dmac)

        print("Creates nhop with %s ip address and %d router"
              " interface id" % (ipv6_addr, rif_id1))
        nhop2 = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(ipv6_addr),
            router_interface_id=rif_id1,
            type=SAI_NEXT_HOP_TYPE_IP)

        print("Creates mirror session")
        spanid = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port24,
            type=mirror_type)

        # setup ACL table groups
        group_stage_ingress = SAI_ACL_STAGE_INGRESS
        group_stage_egress = SAI_ACL_STAGE_EGRESS
        group_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL
        table_stage_ingress = SAI_ACL_STAGE_INGRESS
        table_stage_egress = SAI_ACL_STAGE_EGRESS

        group_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(group_bind_point_list), int32list=group_bind_point_list)

        print("Creates ACL tables groups")
        acl_group_ingress = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=group_stage_ingress,
            acl_bind_point_type_list=group_bind_point_type_list,
            type=group_type)

        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list), int32list=table_bind_point_list)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        print("Creates ACL field data")
        src_ip_t_ipv4 = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ipv4_addr_src1),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ipv4_mask))

        src_ip_t_ipv6 = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip6=ipv6_addr_src),
            mask=sai_thrift_acl_field_data_mask_t(
                ip6=ipv6_mask))

        src_ip_t_mirror = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ipv4_addr_src2),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ipv4_mask))

        # create ACL tables
        print("Creates ACL tables")
        acl_ingress_ipv4_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_ingress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        acl_ingress_ipv6_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_ingress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ipv6=True)

        # create ACL table group members
        print("Creates ACL group members")
        acl_group_ingress_ipv4_member_id = \
            sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=acl_group_ingress,
                acl_table_id=acl_ingress_ipv4_table_id)

        acl_group_ingress_ipv6_member_id = \
            sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=acl_group_ingress,
                acl_table_id=acl_ingress_ipv6_table_id)

        # create ACL entries
        print("Creates ACL entries")
        ipv4_acl_ingress_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id,
            priority=9999,
            field_src_ip=src_ip_t_ipv4,
            action_packet_action=packet_action)

        ipv6_acl_ingress_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv6_table_id,
            priority=9998,
            field_src_ipv6=src_ip_t_ipv6,
            action_packet_action=packet_action)

        mirror_acl_ingress_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id,
            priority=9997,
            field_src_ip=src_ip_t_mirror,
            action_packet_action=packet_action)

        try:
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=dmac,
                                    ip_src=ipv4_addr_src1,
                                    ip_dst=ipv4_addr,
                                    tcp_sport=0x4321,
                                    tcp_dport=0x51,
                                    ip_ttl=64)
            exp_pkt = simple_tcp_packet(eth_dst=dmac,
                                        eth_src=ROUTER_MAC,
                                        ip_src=ipv4_addr_src1,
                                        ip_dst=ipv4_addr,
                                        tcp_sport=0x4321,
                                        tcp_dport=0x51,
                                        ip_ttl=63)

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 20.0'
                  '.0.1 | 192.168.0.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, '| 20.0.0.1'
                  ' | 192.168.0.1 | @ ptf_intf 1')
            verify_packets(self, exp_pkt, [self.dev_port10])

            pktv6 = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                        eth_src=dmac,
                                        ipv6_dst=ipv6_addr,
                                        ipv6_src=ipv6_addr_src,
                                        ipv6_hlim=64)
            exp_pktv6 = simple_tcpv6_packet(eth_dst=dmac,
                                            eth_src=ROUTER_MAC,
                                            ipv6_dst=ipv6_addr,
                                            ipv6_src=ipv6_addr_src,
                                            ipv6_hlim=63)

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 4000::1'
                  ' | 2000::1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pktv6)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, '| 4000::1'
                  ' | 2000::1 | @ ptf_intf 1')
            verify_packets(self, exp_pktv6, [self.dev_port10])

            pkt2 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                     eth_src=dmac,
                                     ip_src=ipv4_addr_src2,
                                     ip_dst=ipv4_addr,
                                     tcp_sport=0x4321,
                                     tcp_dport=0x51,
                                     ip_ttl=64)
            exp_pkt2 = simple_tcp_packet(eth_dst=dmac,
                                         eth_src=ROUTER_MAC,
                                         ip_src=ipv4_addr_src2,
                                         ip_dst=ipv4_addr,
                                         tcp_sport=0x4321,
                                         tcp_dport=0x51,
                                         ip_ttl=63)

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 20.0.0.'
                  '3 | 192.168.0.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt2)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, '| 20.0.0.3'
                  ' | 192.168.0.1 | @ ptf_intf 1')
            verify_packets(self, exp_pkt2, [self.dev_port10])

            # Bind ACL group to port and verify ACLs work
            sai_thrift_set_port_attribute(
                self.client, self.port11, ingress_acl=acl_group_ingress)

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 20.0'
                  '.0.1 | 192.168.0.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### NOT Expecting ', dmac, ' | ', ROUTER_MAC, '| 20.0.0.1'
                  ' | 192.168.0.1 | @ ptf_intf 1')
            verify_no_other_packets(self, timeout=2)

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 4000::1'
                  ' | 2000::1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pktv6)
            print('#### NOT Expecting ', dmac, ' | ', ROUTER_MAC, '| 4000::1'
                  ' | 2000::1 | @ ptf_intf 1')
            verify_no_other_packets(self, timeout=2)

            # Unbind ACL group from port - ACLs sholdn't have any effect
            sai_thrift_set_port_attribute(
                self.client, self.port11, ingress_acl=int(SAI_NULL_OBJECT_ID))

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 20.0'
                  '.0.1 | 192.168.0.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, '| 20.0.0.1'
                  ' | 192.168.0.1 | @ ptf_intf 1')
            verify_packets(self, exp_pkt, [self.dev_port10])

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 4000::1'
                  ' | 2000::1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pktv6)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, '| 4000::1'
                  ' | 2000::1 | @ ptf_intf 1')
            verify_packets(self, exp_pktv6, [self.dev_port10])

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 20.0.0.'
                  '3 | 192.168.0.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt2)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, '| 20.0.0.3'
                  ' | 192.168.0.1 | @ ptf_intf 1')
            verify_packets(self, exp_pkt2, [self.dev_port10])

            sai_thrift_remove_acl_table_group_member(
                self.client, acl_group_ingress_ipv4_member_id)
            sai_thrift_remove_acl_table_group_member(
                self.client, acl_group_ingress_ipv6_member_id)
            sai_thrift_remove_acl_table_group(self.client, acl_group_ingress)
            sai_thrift_remove_acl_entry(self.client, ipv4_acl_ingress_entry_id)
            sai_thrift_remove_acl_entry(self.client, ipv6_acl_ingress_entry_id)
            sai_thrift_remove_acl_entry(self.client,
                                        mirror_acl_ingress_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_ingress_ipv4_table_id)
            sai_thrift_remove_acl_table(self.client, acl_ingress_ipv6_table_id)

            print("Creates ACL tables groups")
            acl_group_egress = sai_thrift_create_acl_table_group(
                self.client,
                acl_stage=group_stage_egress,
                acl_bind_point_type_list=group_bind_point_type_list,
                type=group_type)

            # create ACL tables
            print("Creates ACL tables")
            acl_egress_ipv4_table_id = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage_egress,
                acl_bind_point_type_list=table_bind_point_type_list,
                field_src_ip=True)

            acl_egress_ipv6_table_id = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage_egress,
                acl_bind_point_type_list=table_bind_point_type_list,
                field_src_ipv6=True)

            # create ACL table group members
            print("Creates ACL group members")
            acl_group_egress_ipv4_member_id = \
                sai_thrift_create_acl_table_group_member(
                    self.client,
                    acl_table_group_id=acl_group_egress,
                    acl_table_id=acl_egress_ipv4_table_id)

            acl_group_egress_ipv6_member_id = \
                sai_thrift_create_acl_table_group_member(
                    self.client,
                    acl_table_group_id=acl_group_egress,
                    acl_table_id=acl_egress_ipv6_table_id)

            # create ACL entries
            print("Creates ACL entries")
            ipv4_acl_egress_entry_id = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_egress_ipv4_table_id,
                priority=9999,
                field_src_ip=src_ip_t_ipv4,
                action_packet_action=packet_action)

            ipv6_acl_egress_entry_id = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_egress_ipv6_table_id,
                priority=9998,
                field_src_ipv6=src_ip_t_ipv6,
                action_packet_action=packet_action)

            mirror_acl_egress_entry_id = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_egress_ipv4_table_id,
                priority=9997,
                field_src_ip=src_ip_t_mirror,
                action_packet_action=packet_action)

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 20.0'
                  '.0.1 | 192.168.0.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, '| 20.0.0.1'
                  ' | 192.168.0.1 | @ ptf_intf 1')
            verify_packets(self, exp_pkt, [self.dev_port10])

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 4000::1'
                  ' | 2000::1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pktv6)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, '| 4000::1'
                  ' | 2000::1 | @ ptf_intf 1')
            verify_packets(self, exp_pktv6, [self.dev_port10])

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 20.0.0.'
                  '3 | 192.168.0.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt2)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, '| 20.0.0.3'
                  ' | 192.168.0.1 | @ ptf_intf 1')
            verify_packets(self, exp_pkt2, [self.dev_port10])

            # Bind ACL group to port and verify ACLs work
            sai_thrift_set_port_attribute(
                self.client, self.port10, egress_acl=acl_group_egress)

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 20.0'
                  '.0.1 | 192.168.0.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### NOT Expecting ', dmac, ' | ', ROUTER_MAC, '| 20.0.0.1'
                  ' | 192.168.0.1 | @ ptf_intf 1')
            verify_no_other_packets(self, timeout=2)

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 4000::1'
                  ' | 2000::1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pktv6)
            print('#### NOT Expecting ', dmac, ' | ', ROUTER_MAC, '| 4000::1'
                  ' | 2000::1 | @ ptf_intf 1')
            verify_no_other_packets(self, timeout=2)

            # Unbind ACL group from port - ACLs sholdn't have any effect
            sai_thrift_set_port_attribute(
                self.client, self.port10, egress_acl=int(SAI_NULL_OBJECT_ID))

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 20.0'
                  '.0.1 | 192.168.0.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, '| 20.0.0.1'
                  ' | 192.168.0.1 | @ ptf_intf 1')
            verify_packets(self, exp_pkt, [self.dev_port10])

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 4000::1'
                  ' | 2000::1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pktv6)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, '| 4000::1'
                  ' | 2000::1 | @ ptf_intf 1')
            verify_packets(self, exp_pktv6, [self.dev_port10])

            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 20.0.0.'
                  '3 | 192.168.0.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt2)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, '| 20.0.0.3'
                  ' | 192.168.0.1 | @ ptf_intf 1')
            verify_packets(self, exp_pkt2, [self.dev_port10])

        finally:
            # cleanup ACL
            sai_thrift_remove_acl_table_group_member(
                self.client, acl_group_egress_ipv4_member_id)
            sai_thrift_remove_acl_table_group_member(
                self.client, acl_group_egress_ipv6_member_id)
            sai_thrift_remove_acl_table_group(self.client, acl_group_egress)
            sai_thrift_remove_acl_entry(self.client, ipv4_acl_egress_entry_id)
            sai_thrift_remove_acl_entry(self.client, ipv6_acl_egress_entry_id)
            sai_thrift_remove_acl_entry(self.client,
                                        mirror_acl_egress_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_egress_ipv4_table_id)
            sai_thrift_remove_acl_table(self.client, acl_egress_ipv6_table_id)

            # cleanup
            sai_thrift_remove_mirror_session(self.client, spanid)
            sai_thrift_remove_next_hop(self.client, nhop1)
            sai_thrift_remove_next_hop(self.client, nhop2)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry1)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry2)


@group('mirror-acl')
class MultAclTableGroupBindTest(SaiHelper):
    """
    Tests matching on ACL table groups
    """

    def runTest(self):
        print('--------------------------------------------------------------')
        print('Testing both IPV4, MIRROR ACL table within a acl table group on'
              ' same set of ports')
        print("Sending packet ptf_intf 4 -> [ptf_intf 1, ptf_intf 2, ptf_intf "
              "3] (192.168.0.1 ---> 172.16.10.1 [id = 105])")

        rif_id = self.port13_rif

        ip_addr_subnet = '172.16.10.0'
        ip_addr = '172.16.10.1'
        dmac = '00:11:22:33:44:55'
        mac_src = '00:22:22:22:22:22'
        ip_mask = '255.255.255.0'
        ipv4_addr = '192.168.0.1'

        print("Creates nieghbor with %s ip address, %d router interface"
              " id and %s destination mac" % (
                  ip_addr, rif_id, dmac))
        nbr_entry = sai_thrift_neighbor_entry_t(
            rif_id=rif_id,
            ip_address=sai_ipaddress(ip_addr))
        sai_thrift_create_neighbor_entry(
            self.client, nbr_entry, dst_mac_address=dmac)

        print("Creates nhop with %s ip address and %d router"
              " interface id" % (ip_addr, rif_id))
        nhop = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(ip_addr),
            router_interface_id=rif_id,
            type=SAI_NEXT_HOP_TYPE_IP)

        print("Creates route with %s ip prefix and %d router"
              " interface id" % (ip_addr_subnet, rif_id))
        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(
                ip_addr_subnet))
        sai_thrift_create_route_entry(self.client, route_entry,
                                      next_hop_id=rif_id)

        # send the test packet(s)
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=mac_src,
                                ip_dst=ip_addr,
                                ip_src=ipv4_addr,
                                ip_id=105,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst=dmac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=ip_addr,
                                    ip_src=ipv4_addr,
                                    ip_id=105,
                                    ip_ttl=63)

        print('#### NO ACL Applied ####')
        print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 172.16.10.1'
              ' | 192.168.0.1 | @ ptf_intf 1')
        send_packet(self, self.dev_port10, pkt)
        print('#### Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| 172.16.10.1'
              ' | 192.168.0.1 | @ ptf_intf 4')
        verify_packet(self, exp_pkt, self.dev_port13)
        print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 172.16.10.1'
              ' | 192.168.0.1 | @ ptf_intf 2')
        send_packet(self, self.dev_port11, pkt)
        print('#### Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| 172.16.10.1'
              ' | 192.168.0.1 | @ ptf_intf 4')
        verify_packet(self, exp_pkt, self.dev_port13)
        print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | 172.16.10.1'
              ' | 192.168.0.1 | @ ptf_intf 3')
        send_packet(self, self.dev_port12, pkt)
        print('#### Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| 172.16.10.1'
              ' | 192.168.0.1 | @ ptf_intf 4')
        verify_packet(self, exp_pkt, self.dev_port13)

        # setup ACL table group
        group_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL
        group_stage_ingress = SAI_ACL_STAGE_INGRESS
        group_stage_egress = SAI_ACL_STAGE_EGRESS

        # setup ACL table 1
        table_stage_ingress = SAI_ACL_STAGE_INGRESS
        table_stage_egress = SAI_ACL_STAGE_EGRESS
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]

        group_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(group_bind_point_list), int32list=group_bind_point_list)

        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list), int32list=table_bind_point_list)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        print("Creates ACL field data")
        src_ip_t_ipv4 = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ipv4_addr),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ip_mask))

        # create ACL tables
        print("Creates ACL tables")
        acl_ingress_ip_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_ingress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        # Setup Mirror ACL table
        mirror_type = SAI_MIRROR_SESSION_TYPE_LOCAL
        print("Creates mirror session")
        span_session = sai_thrift_create_mirror_session(
            self.client,
            monitor_port=self.port10,
            type=mirror_type,
            vlan_header_valid=False)
        print(span_session)

        print("Sending packet ptf_intf 2 -[acl]-> ptf_intf 1 (20.20.20.1-[acl]"
              "-> 172.16.10.1 [id = 105])")
        # setup ACL table to block on below matching param
        ip_src = "192.168.0.1"
        ip_src_mask = "255.255.255.0"
        ip_dst = "172.16.10.1"
        ip_dst_mask = "255.255.255.0"
        ip_proto = 6

        src_ip_t_mirror = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ip_src),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ip_src_mask))

        dst_ip_t_mirror = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ip_dst),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ip_dst_mask))

        # create ACL tables
        print("Creates ACL tables")
        acl_ingress_mirror_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_ingress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True,
            field_dst_ip=True,
            field_ip_protocol=ip_proto)

        # setup ACL table group members
        group_member_priority = 1

        acl_group_ingress_list = []
        acl_group_member_ingress_list = []
        in_ports = [self.port10, self.port11, self.port12]

        for port in in_ports:
            # ACL table group
            print("Creates ACL tables groups for", port, " port")
            acl_table_group_ingress = sai_thrift_create_acl_table_group(
                self.client,
                acl_stage=group_stage_ingress,
                acl_bind_point_type_list=group_bind_point_type_list,
                type=group_type)

            # Acreates ACL table group member 1 - v4 tables
            acl_group_ingress_ip_member_id = \
                sai_thrift_create_acl_table_group_member(
                    self.client,
                    acl_table_group_id=acl_table_group_ingress,
                    acl_table_id=acl_ingress_ip_table_id,
                    priority=group_member_priority)

            # creates ACL table group members 2 - mirror tables
            print("Creates ACL group members")
            acl_group_ingress_mirror_member_id = \
                sai_thrift_create_acl_table_group_member(
                    self.client,
                    acl_table_group_id=acl_table_group_ingress,
                    acl_table_id=acl_ingress_mirror_table_id,
                    priority=group_member_priority)

            acl_group_ingress_list.append(acl_table_group_ingress)
            acl_group_member_ingress_list.append(
                acl_group_ingress_ip_member_id)
            acl_group_member_ingress_list.append(
                acl_group_ingress_mirror_member_id)

        for i, ports in enumerate(in_ports):
            # attach this ACL table group to port1, port2, port3
            print("Bind ACL ingress group 0x % lx to port 0x % lx" % (
                acl_group_ingress_list[i], ports))
            sai_thrift_set_port_attribute(
                self.client, ports,
                ingress_acl=acl_group_ingress_list[i])

        action = SAI_PACKET_ACTION_DROP
        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=action))

        # create ACL entries
        print("Creates ACL entries")
        acl_ingress_ip_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ip_table_id,
            priority=1,
            field_src_ip=src_ip_t_ipv4,
            action_packet_action=packet_action)

        src_l4_port = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u16=4000),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        dst_l4_port = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u16=5000),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        mirror_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                objlist=sai_thrift_object_list_t(
                    count=len([span_session]),
                    idlist=[span_session])))

        # create ACL entries
        print("Creates ACL entries")
        mirror_acl_ingress_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_mirror_table_id,
            priority=1,
            field_src_ip=src_ip_t_mirror,
            field_dst_ip=dst_ip_t_mirror,
            field_l4_src_port=src_l4_port,
            field_l4_dst_port=dst_l4_port,
            action_mirror_ingress=mirror_action)

        try:
            print('#### ACL \'DROP, src mac 00:22:22:22:22:22, '
                  'in_ports[ptf_intf_1,2,3,4]\' Applied ####')
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 1')
            time.sleep(5)
            send_packet(self, self.dev_port10, pkt)
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 4')
            verify_no_other_packets(self, timeout=1)
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 4')
            verify_no_other_packets(self, timeout=1)
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 3')
            send_packet(self, self.dev_port12, pkt)
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 4')
            verify_no_other_packets(self, timeout=1)
            print("Verify Mirror ACL")
            time.sleep(5)

            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=mac_src,
                                    ip_src=ipv4_addr,
                                    ip_dst=ip_addr,
                                    ip_id=105,
                                    ip_ttl=64,
                                    tcp_sport=4000,
                                    tcp_dport=5000)

            print("TX packet port 12 -> port 13, ipv4 acl blocks route pkt but"
                  " mirror acl mirrors pkt to port 10")
            send_packet(self, self.dev_port12, pkt)
            verify_packets(self, pkt, ports=[self.dev_port10])

            # cleanup acl, remove acl group member
            for mbr in acl_group_member_ingress_list:
                sai_thrift_remove_acl_table_group_member(self.client, mbr)

            #  unlink this acl table from port10, port12, port13 object
            for i, ports in enumerate(in_ports):
                sai_thrift_set_port_attribute(
                    self.client, ports,
                    ingress_acl=int(SAI_NULL_OBJECT_ID))

            # cleanup acl group, entries, tables
            for grp in acl_group_ingress_list:
                sai_thrift_remove_acl_table_group(self.client, grp)

            sai_thrift_remove_acl_entry(
                self.client, acl_ingress_ip_entry_id)
            sai_thrift_remove_acl_table(
                self.client, acl_ingress_ip_table_id)
            sai_thrift_remove_acl_entry(
                self.client, mirror_acl_ingress_entry_id)
            sai_thrift_remove_acl_table(
                self.client, acl_ingress_mirror_table_id)

            # create ACL tables
            print("Creates ACL tables")
            acl_egress_ip_table_id = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage_egress,
                acl_bind_point_type_list=table_bind_point_type_list,
                field_src_ip=True)

            # ACL table group
            print("Creates ACL egress table groups")
            acl_table_group_egress = sai_thrift_create_acl_table_group(
                self.client,
                acl_stage=group_stage_egress,
                acl_bind_point_type_list=group_bind_point_type_list,
                type=group_type)

            # Acreates ACL table group member 1 - v4 tables
            acl_group_egress_ip_member_id = \
                sai_thrift_create_acl_table_group_member(
                    self.client,
                    acl_table_group_id=acl_table_group_egress,
                    acl_table_id=acl_egress_ip_table_id,
                    priority=group_member_priority)

            # attach this ACL table group to port4
            print("Bind ACL egress group to port4")
            sai_thrift_set_port_attribute(
                self.client, self.port13, egress_acl=acl_table_group_egress)

            # create ACL entries
            print("Creates ACL entries")
            acl_egress_ip_entry_id = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_egress_ip_table_id,
                priority=1,
                field_src_ip=src_ip_t_ipv4,
                action_packet_action=packet_action)

            # send the test packet(s)
            pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                    eth_src='00:22:22:22:22:22',
                                    ip_dst='172.16.10.1',
                                    ip_src='192.168.0.1',
                                    ip_id=105,
                                    ip_ttl=64)

            print('#### ACL \'DROP, src mac 00:22:22:22:22:22, '
                  'in_ports[ptf_intf_1,2,3,4]\' Applied ####')
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 1')
            time.sleep(5)
            send_packet(self, self.dev_port10, pkt)
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 4')
            verify_no_other_packets(self, timeout=1)
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, pkt)
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 4')
            verify_no_other_packets(self, timeout=1)
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 3')
            send_packet(self, self.dev_port12, pkt)
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.0.1 | @ ptf_intf 4')
            verify_no_other_packets(self, timeout=1)
            time.sleep(5)

        finally:
            # cleanup acl, remove acl group member
            sai_thrift_remove_acl_table_group_member(
                self.client, acl_group_egress_ip_member_id)

            #  unlink this acl table from port4 object
            sai_thrift_set_port_attribute(self.client, self.port13,
                                          egress_acl=int(SAI_NULL_OBJECT_ID))

            # cleanup acl group, entries, tables
            sai_thrift_remove_acl_table_group(self.client,
                                              acl_table_group_egress)
            sai_thrift_remove_acl_entry(
                self.client, acl_egress_ip_entry_id)
            sai_thrift_remove_acl_table(
                self.client, acl_egress_ip_table_id)

            # cleanup mirror sess
            sai_thrift_remove_mirror_session(self.client, span_session)
            # l3 part
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry)


class SonicACLTest(SaiHelper):
    """
    Simulates the ACL setup used in ACL community tests
    Creates an ACL group per port
    Creates 3 ACL tables for ipv4 and mirror types
    Creates an ACL group member per table and per group
    Adds entries in each of the tables and test ACLs work as expected
    """

    def runTest(self):

        acl_ingress_groups_list = []
        acl_ingress_group_members_list = []

        rif_id1 = self.port10_rif

        ip_addr_subnet = '192.168.0.0'
        ip_addr = '192.168.0.1'
        dmac = '00:22:22:22:22:22'
        ip_mask = '255.255.255.0'
        ipv4_addr = '192.168.0.1'

        print("Creates nieghbor with %s ip address, %d router interface"
              " id and %s destination mac" % (
                  ip_addr, rif_id1, dmac))
        nbr_entry = sai_thrift_neighbor_entry_t(
            rif_id=rif_id1,
            ip_address=sai_ipaddress(ip_addr))
        sai_thrift_create_neighbor_entry(
            self.client, nbr_entry, dst_mac_address=dmac)

        print("Creates nhop with %s ip address and %d router"
              " interface id" % (ip_addr, rif_id1))
        nhop = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(ip_addr),
            router_interface_id=rif_id1,
            type=SAI_NEXT_HOP_TYPE_IP)

        print("Creates route with %s ip prefix and %d router"
              " interface id" % (ip_addr_subnet, rif_id1))
        route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(
                ip_addr_subnet))
        sai_thrift_create_route_entry(self.client, route_entry,
                                      next_hop_id=rif_id1)

        # setup ACL table group
        group_stage_ingress = SAI_ACL_STAGE_INGRESS
        group_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

        group_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(group_bind_point_list), int32list=group_bind_point_list)

        ports_list = [self.port0, self.port1, self.port2, self.port3,
                      self.port4, self.port5, self.port6, self.port7,
                      self.port8, self.port9, self.port10, self.port11,
                      self.port12, self.port13, self.port14, self.port15,
                      self.port16, self.port17, self.port18, self.port19,
                      self.port20, self.port21, self.port22, self.port23,
                      self.port24, self.port25, self.port26, self.port27,
                      self.port28, self.port29, self.port30, self.port31]

        for index, port in enumerate(ports_list):
            # create ACL table group
            acl_table_group_ingress = sai_thrift_create_acl_table_group(
                self.client,
                acl_stage=group_stage_ingress,
                acl_bind_point_type_list=group_bind_point_type_list,
                type=group_type)

            print(index, port)
            acl_ingress_groups_list.append(acl_table_group_ingress)
            # bind this ACL group to ports object id
            print("Bind ACL ingress group 0x % lx to port 0x % lx" % (
                acl_ingress_groups_list[index], port))
            sai_thrift_set_port_attribute(
                self.client, port,
                ingress_acl=acl_ingress_groups_list[index])

        table_stage_ingress = SAI_ACL_STAGE_INGRESS
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list), int32list=table_bind_point_list)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        print("Creates ACL field data")
        src_ip_t_ipv4 = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ipv4_addr),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ip_mask))

        # create ACL tables
        print("Creates ACL tables")
        acl_ingress_ipv4_table_id2 = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_ingress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        acl_ingress_mirror_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_ingress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True,
            field_dscp=True)

        acl_ingress_ipv4_table_id1 = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_ingress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        # create ACL ingress table group members
        for acl_group in acl_ingress_groups_list:
            # Acreates ACL table group member 2 - v4 tables
            acl_group_ingress_ip_member_id2 = \
                sai_thrift_create_acl_table_group_member(
                    self.client,
                    acl_table_group_id=acl_group,
                    acl_table_id=acl_ingress_ipv4_table_id2,
                    priority=1)

            # creates ACL table group members - mirror tables
            print("Creates ACL group members")
            acl_group_ingress_mirror_member_id = \
                sai_thrift_create_acl_table_group_member(
                    self.client,
                    acl_table_group_id=acl_group,
                    acl_table_id=acl_ingress_mirror_table_id,
                    priority=1)

            # Acreates ACL table group member 1 - v4 tables
            acl_group_ingress_ip_member_id1 = \
                sai_thrift_create_acl_table_group_member(
                    self.client,
                    acl_table_group_id=acl_group,
                    acl_table_id=acl_ingress_ipv4_table_id1,
                    priority=1)

            acl_ingress_group_members_list.append(
                acl_group_ingress_ip_member_id2)
            acl_ingress_group_members_list.append(
                acl_group_ingress_mirror_member_id)
            acl_ingress_group_members_list.append(
                acl_group_ingress_ip_member_id1)

        # create acl entries in secondary ipv4 acl

        # "ACL_RULE|DATAACL|DEFAULT_RULE": {
        #     "type": "hash",
        #     "value": {
        #         "ETHER_TYPE": "2048",
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "1"
        #     }
        # },
        ether_type = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u16=2048),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        acl_ingress_entry_id_default = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id2,
            priority=1,
            field_ether_type=ether_type,
            action_packet_action=packet_action)

        # "ACL_RULE|DATAACL|RULE_1": {
        #     "type": "hash",
        #     "value": {
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9999",
        #         "SRC_IP": "20.0.0.2/32"
        #     }
        # },
        print("Creates ACL field data")
        src_ip_t_ipv4 = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="20.0.0.2"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_FORWARD))\

        acl_ingress_entry_id1 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id2,
            priority=9999,
            field_src_ip=src_ip_t_ipv4,
            action_packet_action=packet_action)

        # remove the entries just created in a secondary table
        # to simulate behaviour of SONiC CT
        sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id1)
        sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id_default)

        # create acl entries in ipv4 acl
        # catch all drop rule
        # "ACL_RULE|DATAINGRESS|DEFAULT_RULE": {
        #     "type": "hash",
        #     "value": {
        #         "ETHER_TYPE": "2048",
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "1"
        #     }
        # },
        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        acl_ingress_entry_id_default = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=1,
            field_ether_type=ether_type,
            action_packet_action=packet_action)

        # "ACL_RULE|DATAINGRESS|RULE_1": {
        #     "type": "hash",
        #     "value": {
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9999",
        #         "SRC_IP": "20.0.0.2/32"
        #     }
        # },
        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_FORWARD))

        acl_ingress_entry_id1 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9999,
            field_src_ip=src_ip_t_ipv4,
            action_packet_action=packet_action)

        # "ACL_RULE|DATAINGRESS|RULE_10": {
        #     "type": "hash",
        #     "value": {
        #         "L4_SRC_PORT_RANGE": "4656-4671",
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9990"
        #     }
        # },
        u32range = sai_thrift_u32_range_t(min=4656, max=4671)
        acl_range_id1 = sai_thrift_create_acl_range(
            self.client,
            type=SAI_ACL_RANGE_TYPE_L4_SRC_PORT_RANGE,
            limit=u32range)
        range_list = [acl_range_id1]
        print("ACL range created 0x%lx" % (acl_range_id1))

        range_list_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(
                objlist=sai_thrift_object_list_t(
                    count=len(range_list),
                    idlist=range_list)))

        acl_ingress_entry_id10 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9990,
            action_packet_action=packet_action,
            field_acl_range_type=range_list_t)

        # "ACL_RULE|DATAINGRESS|RULE_11": {
        #     "type": "hash",
        #     "value": {
        #         "L4_DST_PORT_RANGE": "4640-4687",
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9989"
        #     }
        # },
        u32range = sai_thrift_u32_range_t(min=4640, max=4687)
        acl_range_id2 = sai_thrift_create_acl_range(
            self.client,
            type=SAI_ACL_RANGE_TYPE_L4_DST_PORT_RANGE,
            limit=u32range)
        range_list = [acl_range_id2]
        print("ACL range created 0x%lx" % (acl_range_id2))

        range_list_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(
                objlist=sai_thrift_object_list_t(
                    count=len(range_list),
                    idlist=range_list)))

        acl_ingress_entry_id11 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9989,
            action_packet_action=packet_action,
            field_acl_range_type=range_list_t)

        # "ACL_RULE|DATAINGRESS|RULE_12": {
        #     "type": "hash",
        #     "value": {
        #         "IP_PROTOCOL": "1",
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9988",
        #         "SRC_IP": "20.0.0.4/32"
        #     }
        # },
        src_ip_t_ipv4 = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="20.0.0.4"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        ip_protocol = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u8=1),
            mask=sai_thrift_acl_field_data_mask_t(u8=127))

        acl_ingress_entry_id12 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9988,
            action_packet_action=packet_action,
            field_src_ip=src_ip_t_ipv4,
            field_ip_protocol=ip_protocol)

        # "ACL_RULE|DATAINGRESS|RULE_13": {
        #     "type": "hash",
        #     "value": {
        #         "IP_PROTOCOL": "17",
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9987",
        #         "SRC_IP": "20.0.0.4/32"
        #     }
        # }
        ip_protocol = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u8=17),
            mask=sai_thrift_acl_field_data_mask_t(u8=127))

        acl_ingress_entry_id13 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9987,
            action_packet_action=packet_action,
            field_src_ip=src_ip_t_ipv4,
            field_ip_protocol=ip_protocol)

        # "ACL_RULE|DATAINGRESS|RULE_14": {
        #     "type": "hash",
        #     "value": {
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9986",
        #         "SRC_IP": "20.0.0.6/32"
        #     }
        # },
        src_ip_t_ipv4 = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="20.0.0.6"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        acl_ingress_entry_id14 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9986,
            action_packet_action=packet_action,
            field_src_ip=src_ip_t_ipv4)

        # "ACL_RULE|DATAINGRESS|RULE_15": {
        #     "type": "hash",
        #     "value": {
        #         "DST_IP": "192.168.0.17/32",
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9985"
        #     }
        # },
        dst_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="192.168.0.17"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        acl_ingress_entry_id15 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9985,
            action_packet_action=packet_action,
            field_dst_ip=dst_ip_t)

        # "ACL_RULE|DATAINGRESS|RULE_16": {
        #     "type": "hash",
        #     "value": {
        #         "DST_IP": "172.16.3.0/32",
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9984"
        #     }
        # },
        dst_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="172.16.3.0"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        acl_ingress_entry_id16 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9984,
            action_packet_action=packet_action,
            field_dst_ip=dst_ip_t)

        # "ACL_RULE|DATAINGRESS|RULE_17": {
        #     "type": "hash",
        #     "value": {
        #         "L4_SRC_PORT": "4721",
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9983"
        #     }
        # },
        src_l4_port = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u16=4721),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        acl_ingress_entry_id17 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9983,
            action_packet_action=packet_action,
            field_l4_src_port=src_l4_port)

        # "ACL_RULE|DATAINGRESS|RULE_18": {
        #     "type": "hash",
        #     "value": {
        #         "IP_PROTOCOL": "127",
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9982"
        #     }
        # },
        ip_protocol = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u8=127),
            mask=sai_thrift_acl_field_data_mask_t(u8=127))

        acl_ingress_entry_id18 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9982,
            action_packet_action=packet_action,
            field_ip_protocol=ip_protocol)

        # "ACL_RULE|DATAINGRESS|RULE_19": {
        #     "type": "hash",
        #     "value": {
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9981",
        #         "TCP_FLAGS": "0x24/0x24"
        #     }
        # },
        tcp_flags = sai_thrift_acl_field_data_t(
            enable=True,
            mask=sai_thrift_acl_field_data_mask_t(u8=127),
            data=sai_thrift_acl_field_data_data_t(u8=36))

        acl_ingress_entry_id19 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9981,
            action_packet_action=packet_action,
            field_tcp_flags=tcp_flags)

        # "ACL_RULE|DATAINGRESS|RULE_2": {
        #     "type": "hash",
        #     "value": {
        #         "DST_IP": "192.168.0.16/32",
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9998"
        #     }
        # },
        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_FORWARD))

        dst_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="192.168.0.16"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        acl_ingress_entry_id2 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9998,
            action_packet_action=packet_action,
            field_dst_ip=dst_ip_t)

        # "ACL_RULE|DATAINGRESS|RULE_20": {
        #     "type": "hash",
        #     "value": {
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9980",
        #         "SRC_IP": "20.0.0.7/32"
        #     }
        # },
        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="20.0.0.7"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        acl_ingress_entry_id20 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9980,
            action_packet_action=packet_action,
            field_src_ip=src_ip_t)

        # "ACL_RULE|DATAINGRESS|RULE_21": {
        #     "type": "hash",
        #     "value": {
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9979",
        #         "SRC_IP": "20.0.0.7/32"
        #     }
        # },
        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        acl_ingress_entry_id21 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9979,
            action_packet_action=packet_action,
            field_src_ip=src_ip_t)

        # "ACL_RULE|DATAINGRESS|RULE_22": {
        #     "type": "hash",
        #     "value": {
        #         "L4_DST_PORT": "4731",
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9978"
        #     }
        # },
        dst_l4_port = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u16=4731),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        acl_ingress_entry_id22 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9978,
            action_packet_action=packet_action,
            field_l4_dst_port=dst_l4_port)

        # "ACL_RULE|DATAINGRESS|RULE_23": {
        #     "type": "hash",
        #     "value": {
        #         "L4_SRC_PORT_RANGE": "4756-4771",
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9977"
        #     }
        # },
        u32range = sai_thrift_u32_range_t(min=4756, max=4771)
        acl_range_id3 = sai_thrift_create_acl_range(
            self.client,
            type=SAI_ACL_RANGE_TYPE_L4_SRC_PORT_RANGE,
            limit=u32range)
        range_list = [acl_range_id3]
        print("ACL range created 0x%lx" % (acl_range_id3))

        range_list_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(
                objlist=sai_thrift_object_list_t(
                    count=len(range_list),
                    idlist=range_list)))

        acl_ingress_entry_id23 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9977,
            action_packet_action=packet_action,
            field_acl_range_type=range_list_t)

        # "ACL_RULE|DATAINGRESS|RULE_24": {
        #     "type": "hash",
        #     "value": {
        #         "L4_DST_PORT_RANGE": "4740-4787",
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9976"
        #     }
        # },
        u32range = sai_thrift_u32_range_t(min=4740, max=4787)
        acl_range_id4 = sai_thrift_create_acl_range(
            self.client,
            type=SAI_ACL_RANGE_TYPE_L4_SRC_PORT_RANGE,
            limit=u32range)
        range_list = [acl_range_id4]
        print("ACL range created 0x%lx" % (acl_range_id4))

        range_list_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(
                objlist=sai_thrift_object_list_t(
                    count=len(range_list),
                    idlist=range_list)))

        acl_ingress_entry_id24 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9976,
            action_packet_action=packet_action,
            field_acl_range_type=range_list_t)

        # "ACL_RULE|DATAINGRESS|RULE_25": {
        #     "type": "hash",
        #     "value": {
        #         "IP_PROTOCOL": "1",
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9975",
        #         "SRC_IP": "20.0.0.8/32"
        #     }
        # },
        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="20.0.0.8"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        ip_protocol = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u8=1),
            mask=sai_thrift_acl_field_data_mask_t(u8=127))

        acl_ingress_entry_id25 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9975,
            action_packet_action=packet_action,
            field_src_ip=src_ip_t,
            field_ip_protocol=ip_protocol)

        # "ACL_RULE|DATAINGRESS|RULE_26": {
        #     "type": "hash",
        #     "value": {
        #         "IP_PROTOCOL": "17",
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9974",
        #         "SRC_IP": "20.0.0.8/32"
        #     }
        # },
        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="20.0.0.8"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        ip_protocol = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u8=17),
            mask=sai_thrift_acl_field_data_mask_t(u8=127))

        acl_ingress_entry_id26 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9974,
            action_packet_action=packet_action,
            field_src_ip=src_ip_t,
            field_ip_protocol=ip_protocol)

        # "ACL_RULE|DATAINGRESS|RULE_27": {
        #     "type": "hash",
        #     "value": {
        #         "L4_SRC_PORT": "179",
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9973"
        #     }
        # },
        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_FORWARD))

        src_l4_port = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u16=179),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        acl_ingress_entry_id27 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9973,
            action_packet_action=packet_action,
            field_l4_src_port=src_l4_port)

        # "ACL_RULE|DATAINGRESS|RULE_28": {
        #     "type": "hash",
        #     "value": {
        #         "L4_DST_PORT": "179",
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9972"
        #     }
        # },
        dst_l4_port = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u16=179),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        acl_ingress_entry_id28 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9972,
            action_packet_action=packet_action,
            field_l4_dst_port=dst_l4_port)

        # "ACL_RULE|DATAINGRESS|RULE_3": {
        #     "type": "hash",
        #     "value": {
        #         "DST_IP": "172.16.2.0/32",
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9997"
        #     }
        # },
        dst_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="172.16.2.0"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        acl_ingress_entry_id3 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9997,
            action_packet_action=packet_action,
            field_dst_ip=dst_ip_t)

        # "ACL_RULE|DATAINGRESS|RULE_4": {
        #     "type": "hash",
        #     "value": {
        #         "L4_SRC_PORT": "4621",
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9996"
        #     }
        # },
        src_l4_port = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u16=4621),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        acl_ingress_entry_id4 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9996,
            action_packet_action=packet_action,
            field_l4_src_port=src_l4_port)

        # "ACL_RULE|DATAINGRESS|RULE_5": {
        #     "type": "hash",
        #     "value": {
        #         "IP_PROTOCOL": "126",
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9995"
        #     }
        # },
        ip_protocol = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u8=126),
            mask=sai_thrift_acl_field_data_mask_t(u8=127))

        acl_ingress_entry_id5 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9995,
            action_packet_action=packet_action,
            field_ip_protocol=ip_protocol)

        # "ACL_RULE|DATAINGRESS|RULE_6": {
        #     "type": "hash",
        #     "value": {
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9994",
        #         "TCP_FLAGS": "0x1b/0x1b"
        #     }
        # },
        tcp_flags = sai_thrift_acl_field_data_t(
            enable=True,
            mask=sai_thrift_acl_field_data_mask_t(u8=127),
            data=sai_thrift_acl_field_data_data_t(u8=27))

        acl_ingress_entry_id6 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9994,
            action_packet_action=packet_action,
            field_tcp_flags=tcp_flags)

        # "ACL_RULE|DATAINGRESS|RULE_7": {
        #     "type": "hash",
        #     "value": {
        #         "PACKET_ACTION": "DROP",
        #         "PRIORITY": "9993",
        #         "SRC_IP": "20.0.0.3/32"
        #     }
        # },
        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4="20.0.0.3"),
            mask=sai_thrift_acl_field_data_mask_t(ip4="255.255.255.255"))

        acl_ingress_entry_id7 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9993,
            action_packet_action=packet_action,
            field_src_ip=src_ip_t)

        # "ACL_RULE|DATAINGRESS|RULE_8": {
        #     "type": "hash",
        #     "value": {
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9992",
        #         "SRC_IP": "20.0.0.3/32"
        #     }
        # },
        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_FORWARD))

        acl_ingress_entry_id8 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9992,
            action_packet_action=packet_action,
            field_src_ip=src_ip_t)

        # "ACL_RULE|DATAINGRESS|RULE_9": {
        #     "type": "hash",
        #     "value": {
        #         "L4_DST_PORT": "4631",
        #         "PACKET_ACTION": "FORWARD",
        #         "PRIORITY": "9991"
        #     }
        # },
        dst_l4_port = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u16=4631),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        acl_ingress_entry_id9 = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_ingress_ipv4_table_id1,
            priority=9991,
            action_packet_action=packet_action,
            field_l4_dst_port=dst_l4_port)

        try:
            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src=dmac,
                ip_src="20.0.0.1",
                ip_dst=ipv4_addr,
                tcp_sport=0x4321,
                tcp_dport=0x51,
                ip_ttl=64)
            send_packet(self, self.dev_port11, pkt)
            verify_no_other_packets(self, timeout=2)

            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src=dmac,
                ip_src="20.0.0.2",
                ip_dst=ipv4_addr,
                tcp_sport=0x4321,
                tcp_dport=0x51,
                ip_ttl=64)
            exp_pkt = simple_tcp_packet(
                eth_dst=dmac,
                eth_src=ROUTER_MAC,
                ip_src="20.0.0.2",
                ip_dst=ipv4_addr,
                tcp_sport=0x4321,
                tcp_dport=0x51,
                ip_ttl=63)
            send_packet(self, self.dev_port11, pkt)
            verify_packets(self, exp_pkt, [self.dev_port10])

        finally:
            print("Unbind ACL groups from ports object id:")
            for i, ports in enumerate(ports_list):
                print(i, ports)
                sai_thrift_set_port_attribute(
                    self.client, ports,
                    ingress_acl=int(SAI_NULL_OBJECT_ID))

            for acl_group_member in acl_ingress_group_members_list:
                sai_thrift_remove_acl_table_group_member(self.client,
                                                         acl_group_member)

            for acl_group in acl_ingress_groups_list:
                sai_thrift_remove_acl_table_group(self.client, acl_group)

            # cleanup ACL
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id28)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id27)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id26)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id25)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id24)
            sai_thrift_remove_acl_range(self.client, acl_range_id4)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id23)
            sai_thrift_remove_acl_range(self.client, acl_range_id3)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id22)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id21)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id20)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id19)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id18)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id17)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id16)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id15)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id14)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id13)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id12)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id11)
            sai_thrift_remove_acl_range(self.client, acl_range_id2)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id10)
            sai_thrift_remove_acl_range(self.client, acl_range_id1)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id9)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id8)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id7)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id6)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id5)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id4)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id3)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id2)
            sai_thrift_remove_acl_entry(self.client, acl_ingress_entry_id1)
            sai_thrift_remove_acl_entry(self.client,
                                        acl_ingress_entry_id_default)

            sai_thrift_remove_acl_table(self.client,
                                        acl_ingress_ipv4_table_id1)
            sai_thrift_remove_acl_table(self.client,
                                        acl_ingress_mirror_table_id)
            sai_thrift_remove_acl_table(self.client,
                                        acl_ingress_ipv4_table_id2)

            # cleanup
            sai_thrift_remove_route_entry(self.client, route_entry)
            sai_thrift_remove_next_hop(self.client, nhop)
            sai_thrift_remove_neighbor_entry(self.client, nbr_entry)


class TCPFlagsACLTest(SaiHelper):
    """ ACL: Test TCP Flags """

    acl_table = None
    acl_entry = None

    def setUp(self):
        super(TCPFlagsACLTest, self).setUp()

        self.dmac = '00:11:22:33:44:55'
        self.ip_addr1 = '10.10.10.1'
        self.ip_addr2 = '10.10.10.2'
        self.src_mac = '00:22:22:22:22:22'
        self.ip_addr_src = '192.168.0.1'

        self.nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr2),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress(self.ip_addr2))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry,
            dst_mac_address=self.dmac)

        self.route_entry = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.0/24'))
        sai_thrift_create_route_entry(
            self.client,
            self.route_entry,
            next_hop_id=self.nhop)

    def runTest(self):
        print("TCPFlagsAclTest")
        stage = SAI_ACL_STAGE_INGRESS
        bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE]
        action_types = [SAI_ACL_ACTION_TYPE_PACKET_ACTION]
        action_drop = SAI_PACKET_ACTION_DROP

        acl_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(bind_points), int32list=bind_points)
        acl_action_type_list = sai_thrift_s32_list_t(
            count=len(action_types), int32list=action_types)
        self.acl_table = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=acl_bind_point_type_list,
            acl_action_type_list=acl_action_type_list,
            field_dst_ip=True)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(s32=action_drop))

        tcp_flag = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u8=0x17),
            mask=sai_thrift_acl_field_data_mask_t(u8=0x10))

        self.acl_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=self.acl_table,
            action_packet_action=packet_action,
            field_tcp_flags=tcp_flag
        )

        sai_thrift_set_router_interface_attribute(
            self.client, self.port11_rif, ingress_acl=self.acl_table)

        pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.src_mac,
            ip_dst=self.ip_addr1,
            ip_src=self.ip_addr_src,
            tcp_flags=0x2,
            ip_ttl=64)
        exp_pkt = simple_tcp_packet(
            eth_dst=self.dmac,
            eth_src=ROUTER_MAC,
            ip_dst=self.ip_addr1,
            ip_src=self.ip_addr_src,
            tcp_flags=0x2,
            ip_ttl=63)

        print("Sending tcp packet (ACK bit 0) on port %d, forward"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)

        pkt = simple_udp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.src_mac,
            ip_dst=self.ip_addr1,
            ip_src=self.ip_addr_src,
            pktlen=100,
            ip_ttl=64)
        exp_pkt = simple_udp_packet(
            eth_dst=self.dmac,
            eth_src=ROUTER_MAC,
            ip_dst=self.ip_addr1,
            ip_src=self.ip_addr_src,
            pktlen=100,
            ip_ttl=63)

        print("Sending udp packet on port %d, forward" % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)

        pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.src_mac,
            ip_dst=self.ip_addr1,
            ip_src=self.ip_addr_src,
            ip_id=105,
            tcp_flags=0x10,
            ip_ttl=64)

        print("Sending tcp packet (ACK bit 1) on port %d, drop"
              % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_no_other_packets(self, timeout=1)

        pkt = simple_udp_packet(
            eth_dst=ROUTER_MAC,
            eth_src=self.src_mac,
            ip_dst=self.ip_addr1,
            ip_src=self.ip_addr_src,
            pktlen=100,
            ip_ttl=64)
        exp_pkt = simple_udp_packet(
            eth_dst=self.dmac,
            eth_src=ROUTER_MAC,
            ip_dst=self.ip_addr1,
            ip_src=self.ip_addr_src,
            pktlen=100,
            ip_ttl=63)

        print("Sending udp packet on port %d, forward" % self.dev_port11)
        send_packet(self, self.dev_port11, pkt)
        verify_packet(self, exp_pkt, self.dev_port10)

    def tearDown(self):
        sai_thrift_set_router_interface_attribute(
            self.client, self.port11_rif, ingress_acl=0)

        sai_thrift_remove_acl_entry(self.client, self.acl_entry)
        sai_thrift_remove_acl_table(self.client, self.acl_table)

        sai_thrift_remove_route_entry(self.client, self.route_entry)
        sai_thrift_remove_next_hop(self.client, self.nhop)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry)

        super(TCPFlagsACLTest, self).tearDown()


class AclTableTypeTest(SaiHelper):
    '''
    Acl Type class. This test creates tables with various match fields
    '''
    acl_range_type = sai_thrift_s32_list_t(count=0, int32list=[])

    def setUp(self):
        super(AclTableTypeTest, self).setUp()

        self.dmac = '00:11:22:33:44:55'
        self.src_mac = '00:22:22:22:22:22'
        self.ip_addr1 = '10.0.0.1'
        self.ip_addr2 = '10.10.10.2'

        self.port24_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port24)

        self.port25_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port25)

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr2),
            router_interface_id=self.port25_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port25_rif, ip_address=sai_ipaddress(self.ip_addr2))
        sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=self.dmac)

        self.route_entry0 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.0.0.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry0, next_hop_id=self.nhop1)

        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(
                '1234:5678:9abc:def0:4422:1133:5577:99aa/128'))
        sai_thrift_create_route_entry(
            self.client, self.route_entry1, next_hop_id=self.nhop1)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry0)
        sai_thrift_remove_route_entry(self.client, self.route_entry1)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_router_interface(self.client, self.port25_rif)
        sai_thrift_remove_router_interface(self.client, self.port24_rif)
        super(AclTableTypeTest, self).tearDown()

    def testIPv4Acl(self):
        '''
        Test various IPv4 field combinations for table creation
        '''
        print("testIPv4Acl")
        pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.src_mac,
                                ip_dst=self.ip_addr1,
                                ip_ttl=64)
        exp_pkt = simple_tcp_packet(eth_dst=self.dmac,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=self.ip_addr1,
                                    ip_ttl=63)
        try:
            # verify packet forwarding without ACL
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, exp_pkt, self.dev_port25)

            # create ACL table
            table_stage = SAI_ACL_STAGE_INGRESS
            table_bind_points = [SAI_ACL_BIND_POINT_TYPE_PORT,
                                 SAI_ACL_BIND_POINT_TYPE_LAG]
            table_bind_point_type_list = sai_thrift_s32_list_t(
                count=len(table_bind_points), int32list=table_bind_points)
            acl_table = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage,
                acl_bind_point_type_list=table_bind_point_type_list,
                # field_dst_mac=True,
                field_src_ip=True,
                field_dst_ip=True,
                field_ip_protocol=True,
                field_dscp=True,
                field_l4_src_port=True,
                field_l4_dst_port=True,
                field_ttl=True,
                field_tcp_flags=True,
                field_ether_type=True,
                field_acl_range_type=self.acl_range_type,
                field_icmp_code=True,
                field_icmp_type=True,
                field_acl_ip_frag=True,
                field_acl_ip_type=True,
                field_outer_vlan_id=True)

            # create ACL table entry
            dst_ip_mask = '255.255.255.255'
            dst_ip_t = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip4=self.ip_addr1),
                mask=sai_thrift_acl_field_data_mask_t(ip4=dst_ip_mask))

            packet_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    s32=SAI_PACKET_ACTION_DROP))
            acl_entry = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table,
                priority=10,
                field_dst_ip=dst_ip_t,
                action_packet_action=packet_action)

            # bind acl table to ingress port 24
            sai_thrift_set_port_attribute(self.client, self.port24,
                                          ingress_acl=acl_table)

            # verify packet dropped after ACL apply
            send_packet(self, self.dev_port24, pkt)
            verify_no_other_packets(self, timeout=2)
        finally:
            sai_thrift_set_port_attribute(self.client, self.port24,
                                          ingress_acl=0)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)

    def testIPv6Acl(self):
        '''
        Test various IPv6 field combinations for table creation
        '''
        print("testIPv6Acl")
        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_hlim=64)
        exp_pkt = simple_tcpv6_packet(
            eth_dst=self.dmac,
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_hlim=63)
        try:
            # verify packet forwarding without ACL
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, exp_pkt, self.dev_port25)

            # create ACL table
            table_stage = SAI_ACL_STAGE_INGRESS
            table_bind_points = [SAI_ACL_BIND_POINT_TYPE_PORT,
                                 SAI_ACL_BIND_POINT_TYPE_LAG]
            table_bind_point_type_list = sai_thrift_s32_list_t(
                count=len(table_bind_points), int32list=table_bind_points)
            acl_table = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage,
                acl_bind_point_type_list=table_bind_point_type_list,
                # field_dst_mac=True,
                field_src_ipv6=True,
                field_dst_ipv6=True,
                field_ip_protocol=True,
                field_ipv6_next_header=True,
                field_dscp=True,
                field_l4_src_port=True,
                field_l4_dst_port=True,
                field_ttl=True,
                field_tcp_flags=True,
                field_ether_type=True,
                field_ipv6_flow_label=True,
                field_acl_range_type=self.acl_range_type,
                field_icmpv6_code=True,
                field_icmpv6_type=True,
                field_acl_ip_frag=True,
                field_acl_ip_type=True,
                field_outer_vlan_id=True)

            # create ACL table entry
            dst_ip = '1234:5678:9abc:def0:4422:1133:5577:99aa'
            dst_ip_mask = 'FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF'
            dst_ip_t = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip6=dst_ip),
                mask=sai_thrift_acl_field_data_mask_t(ip6=dst_ip_mask))

            packet_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    s32=SAI_PACKET_ACTION_DROP))
            acl_entry = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table,
                priority=10,
                field_dst_ipv6=dst_ip_t,
                action_packet_action=packet_action)

            # bind acl table to ingress port 24
            sai_thrift_set_port_attribute(self.client, self.port24,
                                          ingress_acl=acl_table)

            # verify packet dropped after ACL apply
            send_packet(self, self.dev_port24, pkt)
            verify_no_other_packets(self, timeout=2)
        finally:
            sai_thrift_set_port_attribute(self.client, self.port24,
                                          ingress_acl=0)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)

    def testIPMirrorAcl(self):
        '''
        Test various IP mirror functionality
        '''
        print("testIPMirrorAcl")
        pkt = simple_tcpv6_packet(
            eth_dst=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_hlim=64)
        exp_pkt = simple_tcpv6_packet(
            eth_dst=self.dmac,
            eth_src=ROUTER_MAC,
            ipv6_dst='1234:5678:9abc:def0:4422:1133:5577:99aa',
            ipv6_hlim=63)
        try:
            # verify packet forwarding without ACL
            send_packet(self, self.dev_port24, pkt)
            verify_packet(self, exp_pkt, self.dev_port25)

            mirror_session = sai_thrift_create_mirror_session(
                self.client,
                monitor_port=self.port26,
                type=SAI_MIRROR_SESSION_TYPE_LOCAL)

            # create ACL table
            table_stage = SAI_ACL_STAGE_INGRESS
            table_bind_points = [SAI_ACL_BIND_POINT_TYPE_PORT,
                                 SAI_ACL_BIND_POINT_TYPE_LAG]
            table_bind_point_type_list = sai_thrift_s32_list_t(
                count=len(table_bind_points), int32list=table_bind_points)
            actions = [SAI_ACL_ACTION_TYPE_MIRROR_INGRESS]
            action_type_list = sai_thrift_s32_list_t(
                count=len(actions), int32list=actions)
            acl_table = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage,
                acl_bind_point_type_list=table_bind_point_type_list,
                acl_action_type_list=action_type_list,
                # field_dst_mac=True,
                field_src_ipv6=True,
                field_dst_ipv6=True,
                field_ip_protocol=True,
                field_ipv6_next_header=True,
                field_dscp=True,
                field_l4_src_port=True,
                field_l4_dst_port=True,
                field_ttl=True,
                field_tcp_flags=True,
                field_ether_type=True,
                field_acl_range_type=self.acl_range_type,
                field_icmpv6_code=True,
                field_icmpv6_type=True,
                field_acl_ip_frag=True,
                field_acl_ip_type=True,
                field_outer_vlan_id=True)

            # create ACL table entry
            dst_ip = '1234:5678:9abc:def0:4422:1133:5577:99aa'
            dst_ip_mask = 'FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF'
            dst_ip_t = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(ip6=dst_ip),
                mask=sai_thrift_acl_field_data_mask_t(ip6=dst_ip_mask))

            mirror_session_list = sai_thrift_object_list_t(
                count=1, idlist=[mirror_session])
            mirror_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    objlist=mirror_session_list))
            acl_entry = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table,
                priority=10,
                field_dst_ipv6=dst_ip_t,
                action_mirror_ingress=mirror_action)

            # bind acl table to ingress port 24
            sai_thrift_set_port_attribute(self.client, self.port24,
                                          ingress_acl=acl_table)

            # verify packet dropped after ACL apply
            send_packet(self, self.dev_port24, pkt)
            verify_each_packet_on_each_port(self, [exp_pkt, pkt],
                                            [self.dev_port25, self.dev_port26])
        finally:
            sai_thrift_set_port_attribute(self.client, self.port24,
                                          ingress_acl=0)
            sai_thrift_remove_acl_entry(self.client, acl_entry)
            sai_thrift_remove_acl_table(self.client, acl_table)
            sai_thrift_remove_mirror_session(self.client, mirror_session)

    def runTest(self):
        try:
            self.testIPv4Acl()
            self.testIPv6Acl()
            self.testIPMirrorAcl()
        finally:
            pass


@group('acl-redirect')
class AclRedirectTest(SaiHelper):
    """
    Verifies Acl redirection test cases
    """
    acl_grp_members = []
    acl_grps = []
    acl_rules = []
    acl_tables = []
    vlan_members = []
    vlans = []
    vlan_ports = []
    bridge_ports = []
    fdbs = []
    lags = []
    lag_members = []
    vrs = []

    def aclCleanup(self):
        """Clean up ACL configuration"""
        sai_thrift_set_port_attribute(
            self.client, self.port25, ingress_acl=int(SAI_NULL_OBJECT_ID))
        for acl_grp_member in list(self.acl_grp_members):
            sai_thrift_remove_acl_table_group_member(self.client,
                                                     acl_grp_member)
            self.acl_grp_members.remove(acl_grp_member)
        for acl_grp in list(self.acl_grps):
            sai_thrift_remove_acl_table_group(self.client, acl_grp)
            self.acl_grps.remove(acl_grp)
        for acl_rule in list(self.acl_rules):
            sai_thrift_remove_acl_entry(self.client, acl_rule)
            self.acl_rules.remove(acl_rule)
        for acl_table in list(self.acl_tables):
            sai_thrift_remove_acl_table(self.client, acl_table)
            self.acl_tables.remove(acl_table)

    def cleanup(self):
        """Clean up network configuration"""
        for fdb in list(self.fdbs):
            sai_thrift_remove_fdb_entry(self.client, fdb)
            self.fdbs.remove(fdb)
        for vlan_port in list(self.vlan_ports):
            print("vlan_ports")
            self.vlan_ports.remove(vlan_port)
        for vlan_member in list(self.vlan_members):
            print("vlan_members")
            sai_thrift_remove_vlan_member(self.client, vlan_member)
            self.vlan_members.remove(vlan_member)
        for lag_member in list(self.lag_members):
            print("lag_members")
            sai_thrift_remove_lag_member(self.client, lag_member)
            self.lag_members.remove(lag_member)
        for port in list(self.bridge_ports):
            print("bridge_ports")
            sai_thrift_remove_bridge_port(self.client, port)
            self.bridge_ports.remove(port)
        for lag in list(self.lags):
            print("lags")
            sai_thrift_remove_lag(self.client, lag)
            self.lags.remove(lag)
        sai_thrift_set_port_attribute(
            self.client, self.port24, port_vlan_id=int(SAI_NULL_OBJECT_ID))
        sai_thrift_set_port_attribute(
            self.client, self.port25, port_vlan_id=int(SAI_NULL_OBJECT_ID))
        sai_thrift_set_port_attribute(
            self.client, self.port26, port_vlan_id=int(SAI_NULL_OBJECT_ID))
        print("vlans")
        for vlan in list(self.vlans):
            sai_thrift_remove_vlan(self.client, vlan)
            self.vlans.remove(vlan)

    def runTest(self):
        print("Testing AclRedirectTest")

        print('-------------------------------------------------------------')

        mac = '00:11:11:11:11:11'
        mac_action = SAI_PACKET_ACTION_FORWARD

        # Add port 24, 25, 26 to Vlan100
        vlan_id = 100
        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)
        self.vlans.append(vlan_oid)

        port24_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        self.bridge_ports.append(port24_bp)

        vlan_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan_oid,
            bridge_port_id=port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_members.append(vlan_member1)
        self.vlan_ports.append(self.port24)

        port25_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port25,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        self.bridge_ports.append(port25_bp)

        vlan_member2 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan_oid,
            bridge_port_id=port25_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_members.append(vlan_member2)
        self.vlan_ports.append(self.port25)

        port26_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port26,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        self.bridge_ports.append(port26_bp)

        vlan_member3 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan_oid,
            bridge_port_id=port26_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_members.append(vlan_member3)
        self.vlan_ports.append(self.port26)

        sai_thrift_set_port_attribute(
            self.client, self.port24, port_vlan_id=vlan_id)
        sai_thrift_set_port_attribute(
            self.client, self.port25, port_vlan_id=vlan_id)
        sai_thrift_set_port_attribute(
            self.client, self.port26, port_vlan_id=vlan_id)

        # Create Lag (port 27, 28) and add it to Vlan100
        lag_id1 = sai_thrift_create_lag(self.client)
        self.lags.append(lag_id1)
        lag1_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=lag_id1,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.bridge_ports.append(lag1_bp)
        lag_member_id1 = sai_thrift_create_lag_member(
            self.client, lag_id=lag_id1, port_id=self.port27)
        self.lag_members.append(lag_member_id1)

        lag_member_id2 = sai_thrift_create_lag_member(
            self.client, lag_id=lag_id1, port_id=self.port28)
        self.lag_members.append(lag_member_id2)

        vlan_member4 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan_oid,
            bridge_port_id=lag1_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        self.vlan_members.append(vlan_member4)
        self.vlan_ports.append(self.port27)
        self.vlan_ports.append(self.port28)

        fdb_entry = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id, mac_address=mac, bv_id=vlan_oid)
        sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=port24_bp,
            packet_action=mac_action)
        self.fdbs.append(fdb_entry)

        eth_pkt1 = simple_eth_packet(pktlen=100,
                                     eth_dst=mac,
                                     eth_src='00:06:07:08:09:0a',
                                     eth_type=0x8137)
        eth_pkt2 = simple_eth_packet(pktlen=100,
                                     eth_dst=mac,
                                     eth_src='00:06:07:08:09:0a',
                                     eth_type=0x8136)
        eth_pkt3 = simple_eth_packet(pktlen=100,
                                     eth_dst=mac,
                                     eth_src='00:06:07:08:09:0a',
                                     eth_type=0x8135)
        eth_pkt4 = simple_eth_packet(pktlen=100,
                                     eth_dst=mac,
                                     eth_src='00:06:07:08:09:0a',
                                     eth_type=0x8134)
        neg_test_pkt = simple_eth_packet(pktlen=100,
                                         eth_dst=mac,
                                         eth_src='00:06:07:08:09:0a',
                                         eth_type=0x1111)

        try:
            print('#### NO ACL Applied ####')
            # send the test packet(s)
            print("Sending Test packet EthType:0x%lx port 25 -> port 24" % (
                eth_pkt1[Ether].type))
            send_packet(self, self.dev_port25, eth_pkt1)
            verify_packets(self, eth_pkt1, [self.dev_port24])

            print("Sending Test packet EthType:0x%lx port 25 -> port 24" % (
                eth_pkt2[Ether].type))
            send_packet(self, self.dev_port25, eth_pkt2)
            verify_packets(self, eth_pkt2, [self.dev_port24])

            print("Sending Test packet EthType:0x%lx port 25 -> port 24" % (
                eth_pkt3[Ether].type))
            send_packet(self, self.dev_port25, eth_pkt3)
            verify_packets(self, eth_pkt3, [self.dev_port24])

            print("Sending Test packet EthType:0x%lx port 25 -> port 24" % (
                eth_pkt4[Ether].type))
            send_packet(self, self.dev_port25, eth_pkt4)
            verify_packets(self, eth_pkt4, [self.dev_port24])

            print("Sending Test(negative test) packet EthType:0x%lx port 25 ->"
                  " port 24" % (neg_test_pkt[Ether].type))
            send_packet(self, self.dev_port25, neg_test_pkt)
            verify_packets(self, neg_test_pkt, [self.dev_port24])

            # setup ACL to redirect based on Ether type
            entry_priority = 1
            acl_action = SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT

            # setup ACL table group
            group_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
            group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL
            group_stage_ingress = SAI_ACL_STAGE_INGRESS
            group_member_priority = 100

            # setup ACL table
            table_stage_ingress = SAI_ACL_STAGE_INGRESS
            table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]

            group_bind_point_type_list = sai_thrift_s32_list_t(
                count=len(group_bind_point_list),
                int32list=group_bind_point_list)

            table_bind_point_type_list = sai_thrift_s32_list_t(
                count=len(table_bind_point_list),
                int32list=table_bind_point_list)

            # create ACL table group
            acl_table_group_ingress = sai_thrift_create_acl_table_group(
                self.client,
                acl_stage=group_stage_ingress,
                acl_bind_point_type_list=group_bind_point_type_list,
                type=group_type)
            self.acl_grps.append(acl_table_group_ingress)

            # Creates ACL tables
            print("Creates ACL tables")
            acl_table_id_ingress = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage_ingress,
                acl_bind_point_type_list=table_bind_point_type_list)

            self.acl_tables.append(acl_table_id_ingress)
            self.assertTrue((acl_table_id_ingress != 0),
                            "ACL table create failed")
            print("IPV4 ACL Table created 0x%lx" % (acl_table_id_ingress))

            # Creates ACL table group members
            acl_group_member_id_ingress = \
                sai_thrift_create_acl_table_group_member(
                    self.client,
                    acl_table_group_id=acl_table_group_ingress,
                    acl_table_id=acl_table_id_ingress,
                    priority=group_member_priority)

            self.assertTrue(acl_group_member_id_ingress != 0,
                            "ACL group member add failed for acl table 0x%lx, "
                            "acl group 0x%lx" % (
                                acl_table_id_ingress, acl_table_group_ingress))
            self.acl_grp_members.append(acl_group_member_id_ingress)

            eth_type = 0x8137 - 0x10000

            ether_type = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(u16=eth_type),
                mask=sai_thrift_acl_field_data_mask_t(u16=32767))

            redirect_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    s32=acl_action,
                    oid=self.port26))

            # create ACL entries
            print("Creates ACL entries")
            acl_ip_entry_id_ingress = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table_id_ingress,
                priority=entry_priority,
                field_ether_type=ether_type,
                action_redirect=redirect_action)

            self.acl_rules.append(acl_ip_entry_id_ingress)
            self.assertTrue((acl_ip_entry_id_ingress != 0), 'ACL entry Match: '
                            'EthType-0x%lx Action: Redirect-0x%lx, create '
                            'failed for acl table 0x%lx' % (
                                eth_type, self.port26, acl_table_id_ingress))
            print("ACL entry Match: EthType-0x%lx Action: Redirect-0x%lx "
                  "created 0x%lx" % (eth_pkt1[Ether].type, self.port26,
                                     acl_ip_entry_id_ingress))

            entry_priority += 1
            eth_type = 0x8136 - 0x10000

            ether_type = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(u16=eth_type),
                mask=sai_thrift_acl_field_data_mask_t(u16=32767))

            redirect_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    s32=acl_action,
                    oid=lag_id1))

            acl_ip_entry_id_ingress = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table_id_ingress,
                priority=entry_priority,
                field_ether_type=ether_type,
                action_redirect=redirect_action)

            self.acl_rules.append(acl_ip_entry_id_ingress)
            self.assertTrue((acl_ip_entry_id_ingress != 0), 'ACL entry Match: '
                            'EthType-0x%lx Action: Redirect-0x%lx, create '
                            'failed for acl table 0x%lx' % (
                                eth_type, lag_id1, acl_table_id_ingress))
            print("ACL entry Match: EthType-0x%lx Action: Redirect-0x%lx "
                  "created 0x%lx" % (eth_pkt2[Ether].type, lag_id1,
                                     acl_ip_entry_id_ingress))

            # Creates ACL table group members
            acl_group_member_id_ingress = \
                sai_thrift_create_acl_table_group_member(
                    self.client,
                    acl_table_group_id=acl_table_group_ingress,
                    acl_table_id=acl_table_id_ingress,
                    priority=200)

            self.assertTrue(acl_group_member_id_ingress != 0,
                            "ACL group member add failed for acl table 0x%lx, "
                            "acl group 0x%lx" % (
                                acl_table_id_ingress, acl_table_group_ingress))
            self.acl_grp_members.append(acl_group_member_id_ingress)

            eth_type = 0x8135 - 0x10000

            ether_type = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(u16=eth_type),
                mask=sai_thrift_acl_field_data_mask_t(u16=32767))

            redirect_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    s32=acl_action,
                    oid=self.port26))

            print("Creates ACL entries")
            acl_ip_entry_id_ingress = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table_id_ingress,
                priority=entry_priority,
                field_ether_type=ether_type,
                action_redirect=redirect_action)

            self.acl_rules.append(acl_ip_entry_id_ingress)
            self.assertTrue((acl_ip_entry_id_ingress != 0), 'ACL entry Match: '
                            'EthType-0x%lx Action: Redirect-0x%lx, create '
                            'failed for acl table 0x%lx' % (
                                eth_type, self.port26, acl_table_id_ingress))
            print("ACL entry Match: EthType-0x%lx Action: Redirect-0x%lx "
                  "created 0x%lx" % (eth_pkt3[Ether].type, self.port26,
                                     acl_ip_entry_id_ingress))

            eth_type = 0x8134 - 0x10000

            ether_type = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(u16=eth_type),
                mask=sai_thrift_acl_field_data_mask_t(u16=32767))

            redirect_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    s32=acl_action,
                    oid=lag_id1))

            print("Creates ACL entries")
            acl_ip_entry_id_ingress = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table_id_ingress,
                priority=entry_priority,
                field_ether_type=ether_type,
                action_redirect=redirect_action)

            self.acl_rules.append(acl_ip_entry_id_ingress)
            self.assertTrue((acl_ip_entry_id_ingress != 0), 'ACL entry Match: '
                            'EthType-0x%lx Action: Redirect-0x%lx, create '
                            'failed for acl table 0x%lx' % (
                                eth_type, lag_id1, acl_table_id_ingress))
            print("ACL entry Match: EthType-0x%lx Action: Redirect-0x%lx "
                  "created 0x%lx" % (eth_pkt3[Ether].type, lag_id1,
                                     acl_ip_entry_id_ingress))

            print("Binding ACL grp 0x%lx to Port25" % (
                acl_table_group_ingress))
            # bind ACL GRP to Port25
            sai_thrift_set_port_attribute(
                self.client, self.port25, ingress_acl=acl_table_group_ingress)

            print("Sending Test packet EthType:0x%lx port 25 -> [ACL REDIRECT]"
                  " -> port 26" % (eth_pkt1[Ether].type))
            # ensure packet is redirected!
            send_packet(self, self.dev_port25, eth_pkt1)
            verify_packets(self, eth_pkt1, [self.dev_port26])

            # ensure packet is redirected!
            print("Sending Test packet EthType:0x%lx port 25 -> [ACL REDIRECT]"
                  " -> Lag1 (Port 27/Port 28)" % (eth_pkt2[Ether].type))
            send_packet(self, self.dev_port25, eth_pkt2)
            verify_packets_any(self, eth_pkt2, [self.dev_port27,
                                                self.dev_port28])

            # ensure packet is redirected!
            print("Sending Test packet EthType:0x%lx port 25 -> [ACL REDIRECT]"
                  " -> port 26" % (eth_pkt3[Ether].type))
            send_packet(self, self.dev_port25, eth_pkt3)
            verify_packets(self, eth_pkt3, [self.dev_port26])

            # ensure packet is redirected!
            print("Sending Test packet EthType:0x%lx port 25 -> [ACL REDIRECT]"
                  " -> Lag1 (Port 27/Port 28)" % (eth_pkt4[Ether].type))
            send_packet(self, self.dev_port25, eth_pkt4)
            verify_packets_any(self, eth_pkt4, [self.dev_port27,
                                                self.dev_port28])

            # ensure packet is not redirected!
            print("Sending Test(negative test) packet EthType:0x%lx port 25 ->"
                  " port 24" % (neg_test_pkt[Ether].type))
            send_packet(self, self.dev_port25, neg_test_pkt)
            verify_packets(self, neg_test_pkt, [self.dev_port24])

        finally:
            self.aclCleanup()
            self.cleanup()


@group("acl-ip-type")
class AclIpTypeTrapTest(SaiHelper):
    '''
    Test ACL IP type field
    '''

    def setUp(self):
        super(AclIpTypeTrapTest, self).setUp()

        hostif_name = "hostif1"
        self.port = self.port24
        self.dev_port = self.dev_port24

        self.hostif_oid = sai_thrift_create_hostif(self.client,
                                                   name=hostif_name,
                                                   obj_id=self.port,
                                                   type=SAI_HOSTIF_TYPE_NETDEV)

        self.assertNotEqual(self.hostif_oid, 0)

        self.hostif_socket = open_packet_socket(hostif_name)

        self.udt_oid = sai_thrift_create_hostif_user_defined_trap(
            self.client,
            type=SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_ACL)
        self.assertNotEqual(self.udt_oid, 0)

        channel = SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_PHYSICAL_PORT
        self.hostif_table_entry_oid = \
            sai_thrift_create_hostif_table_entry(
                self.client,
                channel_type=channel,
                host_if=self.hostif_oid,
                trap_id=self.udt_oid,
                type=SAI_HOSTIF_TABLE_ENTRY_TYPE_TRAP_ID)
        self.assertNotEqual(self.hostif_table_entry_oid, 0)

        table_stage = SAI_ACL_STAGE_INGRESS
        table_bind_points = [SAI_ACL_BIND_POINT_TYPE_SWITCH]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_points), int32list=table_bind_points)
        self.acl_table_oid = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_acl_ip_type=True)
        self.assertNotEqual(self.acl_table_oid, 0)

        self.set_user_trap_id = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(oid=self.udt_oid))

        self.packet_action_trap = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_TRAP))

        self.ip4_pkt = simple_ip_packet()
        self.ip6_pkt = simple_ipv6ip_packet()
        self.lldp_pkt = simple_eth_packet(eth_type=0x88cc)

    def tearDown(self):
        sai_thrift_remove_acl_table(self.client, self.acl_table_oid)
        sai_thrift_remove_hostif_table_entry(
            self.client, self.hostif_table_entry_oid)
        sai_thrift_remove_hostif_user_defined_trap(
            self.client, self.udt_oid)
        sai_thrift_remove_hostif(self.client, self.hostif_oid)
        super(AclIpTypeTrapTest, self).tearDown()

    def runTest(self):
        try:
            self.testAclTypeAny()
            self.testAclTypeIpv4Any()
            # self.testAclTypeIpv6Any()
        finally:
            pass

    def testAclTypeAny(self):
        '''
        Test SAI_ACL_IP_TYPE_ANY
        '''
        print("testAclTypeAny")
        acl_entry_oid = None
        try:
            acl_ip_type = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(s32=SAI_ACL_IP_TYPE_ANY))

            acl_entry_oid = sai_thrift_create_acl_entry(
                self.client,
                table_id=self.acl_table_oid,
                priority=10,
                field_acl_ip_type=acl_ip_type,
                action_set_user_trap_id=self.set_user_trap_id,
                action_packet_action=self.packet_action_trap)
            self.assertNotEqual(acl_entry_oid, 0)

            print("Sending IPv4 packet")
            send_packet(self, self.dev_port, self.ip4_pkt)
            print("Verifying IPv4 packet on port host interface")
            self.assertTrue(
                socket_verify_packet(
                    self.ip4_pkt,
                    self.hostif_socket))

            print("Sending IPv6 packet")
            send_packet(self, self.dev_port, self.ip6_pkt)
            print("Verifying IPv6 packet on port host interface")
            self.assertTrue(socket_verify_packet(self.ip6_pkt,
                                                 self.hostif_socket))

            print("Sending LLDP packet")
            send_packet(self, self.dev_port, self.lldp_pkt)
            print("Verifying LLDP packet on port host interface")
            self.assertTrue(
                socket_verify_packet(
                    self.lldp_pkt,
                    self.hostif_socket))

            print("\tOK")
        finally:
            if acl_entry_oid:
                sai_thrift_remove_acl_entry(self.client, acl_entry_oid)

    def testAclTypeIpv4Any(self):
        '''
        Test SAI_ACL_IP_TYPE_IPV4ANY
        '''
        print("testAclTypeIpv4Any")
        acl_entry_oid = None
        try:
            acl_ip_type = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(
                    s32=SAI_ACL_IP_TYPE_IPV4ANY))

            acl_entry_oid = sai_thrift_create_acl_entry(
                self.client,
                table_id=self.acl_table_oid,
                priority=10,
                field_acl_ip_type=acl_ip_type,
                action_set_user_trap_id=self.set_user_trap_id,
                action_packet_action=self.packet_action_trap)
            self.assertNotEqual(acl_entry_oid, 0)

            print("Sending IPv4 packet")
            send_packet(self, self.dev_port, self.ip4_pkt)
            print("Verifying IPv4 packet on port host interface")
            self.assertTrue(
                socket_verify_packet(
                    self.ip4_pkt,
                    self.hostif_socket))

            print("Sending IPv6 packet")
            send_packet(self, self.dev_port, self.ip6_pkt)
            print("Verifying no IPv6 packet on port host interface")
            self.assertTrue(not socket_verify_packet(self.ip6_pkt,
                                                     self.hostif_socket))

            print("Sending LLDP packet")
            send_packet(self, self.dev_port, self.lldp_pkt)
            print("Verifying no LLDP packet on port host interface")
            self.assertTrue(
                not socket_verify_packet(
                    self.lldp_pkt,
                    self.hostif_socket))

            print("\tOK")
        finally:
            if acl_entry_oid:
                sai_thrift_remove_acl_entry(self.client, acl_entry_oid)

    def testAclTypeIpv6Any(self):
        '''
        Test SAI_ACL_IP_TYPE_IPV6ANY
        '''
        print("testAclTypeIpv6Any")
        acl_entry_oid = None
        try:
            acl_ip_type = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(
                    s32=SAI_ACL_IP_TYPE_IPV6ANY))

            acl_entry_oid = sai_thrift_create_acl_entry(
                self.client,
                table_id=self.acl_table_oid,
                priority=10,
                field_acl_ip_type=acl_ip_type,
                action_set_user_trap_id=self.set_user_trap_id,
                action_packet_action=self.packet_action_trap)
            self.assertNotEqual(acl_entry_oid, 0)

            print("Sending IPv4 packet")
            send_packet(self, self.dev_port, self.ip4_pkt)
            print("Verifying no IPv4 packet on port host interface")
            self.assertTrue(
                socket_verify_packet(
                    self.ip4_pkt,
                    self.hostif_socket))

            print("Sending IPv6 packet")
            send_packet(self, self.dev_port, self.ip6_pkt)
            print("Verifying IPv6 packet on port host interface")
            self.assertTrue(
                not socket_verify_packet(
                    self.ip6_pkt,
                    self.hostif_socket))

            print("Sending LLDP packet")
            send_packet(self, self.dev_port, self.lldp_pkt)
            print("Verifying no LLDP packet on port host interface")
            self.assertTrue(
                not socket_verify_packet(
                    self.lldp_pkt,
                    self.hostif_socket))

            print("\tOK")
        finally:
            if acl_entry_oid:
                sai_thrift_remove_acl_entry(self.client, acl_entry_oid)


@group('acl-pre-ingress')
class AclPreIngressTest(AclTableTypeTest):
    '''
    Test pre-ingress ACL
    '''

    def setUp(self):
        super(AclPreIngressTest, self).setUp()

        self.dmac1 = '00:11:22:33:44:55'
        self.dmac2 = '00:11:22:33:44:56'
        self.ip_addr1 = '10.0.0.1'
        self.ip_addr2 = '10.10.10.2'
        self.vrf1 = sai_thrift_create_virtual_router(self.client)
        self.vrf1_port26_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.vrf1,
            port_id=self.port26)
        self.vrf1_nhop0 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr2),
            router_interface_id=self.vrf1_port26_rif,
            type=SAI_NEXT_HOP_TYPE_IP)
        self.vrf1_neighbor_entry0 = sai_thrift_neighbor_entry_t(
            rif_id=self.vrf1_port26_rif,
            ip_address=sai_ipaddress(self.ip_addr2))
        sai_thrift_create_neighbor_entry(
            self.client, self.vrf1_neighbor_entry0, dst_mac_address=self.dmac2)
        self.vrf1_route_entry0 = sai_thrift_route_entry_t(
            vr_id=self.vrf1, destination=sai_ipprefix('10.0.0.1/32'))
        sai_thrift_create_route_entry(
            self.client, self.vrf1_route_entry0, next_hop_id=self.vrf1_nhop0)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.vrf1_route_entry0)
        sai_thrift_remove_next_hop(self.client, self.vrf1_nhop0)
        sai_thrift_remove_neighbor_entry(
            self.client, self.vrf1_neighbor_entry0)
        sai_thrift_remove_router_interface(self.client, self.vrf1_port26_rif)
        sai_thrift_remove_virtual_router(self.client, self.vrf1)
        super(AclPreIngressTest, self).tearDown()

    def testPreIngressAcl(self):
        '''
        Test pre-ingress matching and VRF assignment
        '''
        print("testPreIngressAcl")
        acl_table_oid = None
        acl_entry_oid = None
        try:
            table_stage = SAI_ACL_STAGE_PRE_INGRESS
            table_bind_points = [SAI_ACL_BIND_POINT_TYPE_SWITCH]
            table_bind_point_type_list = sai_thrift_s32_list_t(
                count=len(table_bind_points), int32list=table_bind_points)
            acl_table_oid = sai_thrift_create_acl_table(
                self.client,
                acl_stage=table_stage,
                acl_bind_point_type_list=table_bind_point_type_list,
                field_src_mac=True,
                field_dst_mac=True,
                field_ether_type=True,
                field_src_ip=True,
                field_dst_ip=True,
                field_tos=True)
            self.assertNotEqual(acl_table_oid, 0)

            src_mac = '00:26:dd:14:c4:ee'
            src_mac_mask = 'ff:ff:ff:ff:ff:ff'
            src_mac_t = sai_thrift_acl_field_data_t(
                data=sai_thrift_acl_field_data_data_t(mac=src_mac),
                mask=sai_thrift_acl_field_data_mask_t(mac=src_mac_mask))

            action_set_vrf = sai_thrift_acl_action_data_t(
                enable=True,
                parameter=sai_thrift_acl_action_parameter_t(
                    oid=self.vrf1))

            packet_action = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    s32=SAI_PACKET_ACTION_FORWARD))

            acl_entry_oid = sai_thrift_create_acl_entry(
                self.client,
                table_id=acl_table_oid,
                priority=10,
                field_src_mac=src_mac_t,
                action_packet_action=packet_action,
                action_set_vrf=action_set_vrf)
            self.assertNotEqual(acl_entry_oid, 0)

            pkt = simple_ip_packet(
                eth_src=src_mac,
                eth_dst=ROUTER_MAC,
                ip_dst=self.ip_addr1,
                ip_ttl=64)

            exp_pkt_default_vrf = simple_ip_packet(
                eth_src=ROUTER_MAC,
                eth_dst=self.dmac1,
                ip_dst=self.ip_addr1,
                ip_ttl=63)

            exp_pkt_vrf1 = simple_ip_packet(
                eth_src=ROUTER_MAC,
                eth_dst=self.dmac2,
                ip_dst=self.ip_addr1,
                ip_ttl=63)

            # Send to port in default vrf, expect in default vrf -
            # pre ingress is not enabled on switch
            send_packet(self, self.dev_port24, pkt)
            verify_any_packet_on_ports_list(self, pkts=[exp_pkt_default_vrf],
                                            ports=[[self.dev_port25]])

            # bind pre-ingress table to switch
            sai_thrift_set_switch_attribute(self.client,
                                            pre_ingress_acl=acl_table_oid)

            # Send to port in default vrf, expect in vrf1
            send_packet(self, self.dev_port24, pkt)
            verify_any_packet_on_ports_list(self, pkts=[exp_pkt_vrf1],
                                            ports=[[self.dev_port26]])

            sai_thrift_set_switch_attribute(self.client,
                                            pre_ingress_acl=0)

            send_packet(self, self.dev_port24, pkt)
            verify_any_packet_on_ports_list(self, pkts=[exp_pkt_default_vrf],
                                            ports=[[self.dev_port25]])
        finally:
            sai_thrift_set_switch_attribute(self.client, pre_ingress_acl=0)
            if acl_entry_oid:
                sai_thrift_remove_acl_entry(self.client, acl_entry_oid)
            if acl_table_oid:
                sai_thrift_remove_acl_table(self.client, acl_table_oid)

    def runTest(self):
        try:
            self.testPreIngressAcl()
        finally:
            pass


class IPv6NextHdrTest(SaiHelper):
    """Verifies ACL blocking TCP traffic"""
    def setUp(self):
        super(IPv6NextHdrTest, self).setUp()
        self.ip_addr1 = '1234:5678:9abc:def0:4422:1133:5577:99aa'
        self.ip_addr2 = '2000::1'
        self.mac1 = '00:11:22:33:44:55'
        self.mac2 = '00:22:22:22:22:22'
        mask = '/112'
        self.table_stage_ingress = SAI_ACL_STAGE_INGRESS
        self.table_stage_egress = SAI_ACL_STAGE_EGRESS

        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress(self.ip_addr1))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry1,
            dst_mac_address=self.mac1)

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr1),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(
                self.ip_addr1 + mask))
        sai_thrift_create_route_entry(
            self.client,
            self.route_entry1,
            next_hop_id=self.nhop1)

        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=self.port11_rif, ip_address=sai_ipaddress(self.ip_addr2))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry2,
            dst_mac_address=self.mac2)

        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr2),
            router_interface_id=self.port11_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix(
                self.ip_addr2 + mask))
        sai_thrift_create_route_entry(
            self.client,
            self.route_entry2,
            next_hop_id=self.nhop2)

        self.tcpv6_1 = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                           eth_src=self.mac2,
                                           ipv6_dst=self.ip_addr1,
                                           ipv6_src=self.ip_addr2,
                                           ipv6_hlim=64)
        self.exp_tcpv6_1 = simple_tcpv6_packet(eth_dst=self.mac1,
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=self.ip_addr1,
                                               ipv6_src=self.ip_addr2,
                                               ipv6_hlim=63)

        self.udpv6_1 = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                           eth_src=self.mac2,
                                           ipv6_dst=self.ip_addr1,
                                           ipv6_src=self.ip_addr2,
                                           ipv6_hlim=64)
        self.exp_udpv6_1 = simple_udpv6_packet(eth_dst=self.mac1,
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=self.ip_addr1,
                                               ipv6_src=self.ip_addr2,
                                               ipv6_hlim=63)

        self.tcpv6_2 = simple_tcpv6_packet(eth_dst=ROUTER_MAC,
                                           eth_src=self.mac1,
                                           ipv6_dst=self.ip_addr2,
                                           ipv6_src=self.ip_addr1,
                                           ipv6_hlim=64)
        self.exp_tcpv6_2 = simple_tcpv6_packet(eth_dst=self.mac2,
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=self.ip_addr2,
                                               ipv6_src=self.ip_addr1,
                                               ipv6_hlim=63)

        self.udpv6_2 = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                           eth_src=self.mac1,
                                           ipv6_dst=self.ip_addr2,
                                           ipv6_src=self.ip_addr1,
                                           ipv6_hlim=64)
        self.exp_udpv6_2 = simple_udpv6_packet(eth_dst=self.mac2,
                                               eth_src=ROUTER_MAC,
                                               ipv6_dst=self.ip_addr2,
                                               ipv6_src=self.ip_addr1,
                                               ipv6_hlim=63)

    def runTest(self):
        self.aclRoutingTest()
        self.aclIPv6NextHdrTest(self.table_stage_ingress)
        self.aclIPv6NextHdrTest(self.table_stage_egress)

    def aclRoutingTest(self):
        """Tests routing without ACL"""
        try:
            print('----------------------------------------------------------')
            print("Sending packet ptf_intf 2 -> ptf_intf 1 (", self.ip_addr2,
                  " ---> ", self.ip_addr1, ")")
            print('#### NO ACL Applied: sending TCP packets ####')
            print('#### Sending  ', ROUTER_MAC, ' | ', self.mac2, ' | ',
                  self.ip_addr2, ' | ', self.ip_addr1, ' | @ ptf_intf 2')
            send_packet(self, self.dev_port11, self.tcpv6_1)
            print('#### Expecting ', self.mac1, ' | ', ROUTER_MAC, ' | ',
                  self.ip_addr2, ' | ', self.ip_addr1, ' | @ ptf_intf 1')
            verify_packets(self, self.exp_tcpv6_1, [self.dev_port10])
            print('#### NO ACL Applied: sending UDP packets ####')
            send_packet(self, self.dev_port11, self.udpv6_1)
            verify_packets(self, self.exp_udpv6_1, [self.dev_port10])

            print('----------------------------------------------------------')
            print("Sending packet ptf_intf 2 -> ptf_intf 1 (", self.ip_addr1,
                  " ---> ", self.ip_addr2, ")")
            print('#### NO ACL Applied: sending TCP packets ####')
            print('#### Sending  ', ROUTER_MAC, ' | ', self.mac1, ' | ',
                  self.ip_addr1, ' | ', self.ip_addr2, ' | @ ptf_intf 2')
            send_packet(self, self.dev_port10, self.tcpv6_2)
            print('#### Expecting ', self.mac2, ' | ', ROUTER_MAC, ' | ',
                  self.ip_addr1, ' | ', self.ip_addr2, ' | @ ptf_intf 1')
            verify_packets(self, self.exp_tcpv6_2, [self.dev_port11])
            print('#### NO ACL Applied: sending UDP packets ####')
            send_packet(self, self.dev_port10, self.udpv6_2)
            verify_packets(self, self.exp_udpv6_2, [self.dev_port11])
        finally:
            print('----------------------------------------------------------')

    def aclIPv6NextHdrTest(self, table_stage):
        """
        Tests ACL with next header field
        Args:
            table_stage (int): specifies ingress or egress type of ACL
        """
        # setup ACL to block based on Source IP
        acl_mask = 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff'
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        # next level protocol is TCP
        ipv6_next_header = 0x06

        if table_stage == SAI_ACL_STAGE_INGRESS:
            src_ip = self.ip_addr2
            dst_ip = self.ip_addr1
        elif table_stage == SAI_ACL_STAGE_EGRESS:
            src_ip = self.ip_addr1
            dst_ip = self.ip_addr2

        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list), int32list=table_bind_point_list)

        acl_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ipv6=True,
            field_ipv6_next_header=True)

        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip6=src_ip),
            mask=sai_thrift_acl_field_data_mask_t(
                ip6=acl_mask))

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        field_ipv6_next_header = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u8=ipv6_next_header),
            mask=sai_thrift_acl_field_data_mask_t(u8=0x0F))

        # Add drop ACL entry to IPv6 ACL Table
        acl_entry_id = sai_thrift_create_acl_entry(
            self.client,
            priority=9999,
            table_id=acl_table_id,
            field_src_ipv6=src_ip_t,
            action_packet_action=packet_action,
            field_ipv6_next_header=field_ipv6_next_header)

        if table_stage == SAI_ACL_STAGE_INGRESS:
            # bind this ACL table to ports object id
            sai_thrift_set_port_attribute(
                self.client, self.port11, ingress_acl=acl_table_id)
            sport = self.dev_port11
            dport = self.dev_port10
            pkt_udp = self.udpv6_1
            exp_pkt_udp = self.exp_udpv6_1
            pkt_tcp = self.tcpv6_1
            exp_pkt_tcp = self.exp_tcpv6_1
            dmac = self.mac1
            smac = self.mac2
            print("Ingress test")
        elif table_stage == SAI_ACL_STAGE_EGRESS:
            # bind this ACL table to ports object id
            sai_thrift_set_port_attribute(
                self.client, self.port11, egress_acl=acl_table_id)
            sport = self.dev_port10
            dport = self.dev_port11
            pkt_udp = self.udpv6_2
            exp_pkt_udp = self.exp_udpv6_2
            pkt_tcp = self.tcpv6_2
            exp_pkt_tcp = self.exp_tcpv6_2
            dmac = self.mac2
            smac = self.mac1
            print("Egress test")

        try:
            assert acl_table_id != 0, 'acl_table_id == 0'
            assert acl_entry_id != 0, 'acl_entry_id == 0'

            print("Sending packet ptf_intf 2-[acl]-> ptf_intf 1 (", src_ip,
                  " -[acl]-> ", dst_ip, ")")

            print('#### Sending   TCP', ROUTER_MAC, ' | ', smac, ' | ',
                  src_ip, ' | ', dst_ip, ' | @ ptf_intf 2')
            send_packet(self, sport, pkt_tcp)
            # ensure the TCP packet is dropped and check for absence
            # of packet here
            print('#### NOT Expecting TCP ', dmac, ' | ', ROUTER_MAC, ' | ',
                  src_ip, ' | ', dst_ip, ' | @ ptf_intf 1')
            verify_no_other_packets(self, timeout=2)

            print('#### Sending   UDP', ROUTER_MAC, ' | ', smac, ' | ',
                  src_ip, ' | ', dst_ip, ' | @ ptf_intf 2')
            send_packet(self, sport, pkt_udp)
            # ensure the UDP packet is forwarded
            print('#### Expecting UDP ', dmac, ' | ', ROUTER_MAC, ' | ',
                  src_ip, ' | ', dst_ip, ' | @ ptf_intf 1')
            verify_packets(self, exp_pkt_udp, [dport])

            # change action_type of ACL entry from ACL_DROP to ACL_PERMIT
            aclaction_data = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    s32=SAI_PACKET_ACTION_FORWARD), enable=True)
            sai_thrift_set_acl_entry_attribute(
                client=self.client,
                acl_entry_oid=acl_entry_id,
                action_packet_action=aclaction_data)

            print('#### Sending      ', ROUTER_MAC, ' | ', smac, ' | ', src_ip,
                  ' | ', dst_ip, ' | @ ptf_intf 2')
            # send the same packet
            send_packet(self, sport, pkt_tcp)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, ' | ', src_ip,
                  ' | ', dst_ip, ' | @ ptf_intf 1')
            # check that TCP packet is forwarded
            verify_packets(self, exp_pkt_tcp, [dport])

        finally:
            # unbind this ACL table from ports object id
            if table_stage == SAI_ACL_STAGE_INGRESS:
                sai_thrift_set_port_attribute(
                    self.client, self.port11, ingress_acl=0)
            elif table_stage == SAI_ACL_STAGE_EGRESS:
                sai_thrift_set_port_attribute(
                    self.client, self.port11, egress_acl=0)
            # cleanup ACL
            sai_thrift_remove_acl_entry(self.client, acl_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_table_id)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry1)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_route_entry(self.client, self.route_entry2)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        super(IPv6NextHdrTest, self).tearDown()


@group('acl')
@group('acl-ocp')
class IPAclFragmentTest(SaiHelper):
    """Verifies ACL with IP fragmentation"""
    def setUp(self):
        super(IPAclFragmentTest, self).setUp()
        print('--------------------------------------------------------------')
        print("Sending packet ptf_intf 2 -> ptf_intf 1 (192.168.0.1 ---> "
              "172.16.10.1 [id = 105])")

        self.ip_addr1 = '172.16.10.1'
        self.ip_addr2 = '192.168.0.1'
        self.dmac1 = '00:11:22:33:44:55'
        self.dmac2 = '00:22:22:22:22:22'
        mask = '/24'
        self.table_stage_ingress = SAI_ACL_STAGE_INGRESS
        self.table_stage_egress = SAI_ACL_STAGE_EGRESS

        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress(self.ip_addr1))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry1,
            dst_mac_address=self.dmac1)

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr1),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.ip_addr1 + mask))
        sai_thrift_create_route_entry(
            self.client,
            self.route_entry1,
            next_hop_id=self.nhop1)

        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=self.port11_rif, ip_address=sai_ipaddress(self.ip_addr2))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry2,
            dst_mac_address=self.dmac2)

        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr2),
            router_interface_id=self.port11_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.ip_addr2 + mask))
        sai_thrift_create_route_entry(
            self.client,
            self.route_entry2,
            next_hop_id=self.nhop2)

        self.pkt1 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                      eth_src=self.dmac2,
                                      ip_dst=self.ip_addr1,
                                      ip_src=self.ip_addr2,
                                      ip_id=105,
                                      ip_tos=0xc8,
                                      ip_ttl=64)
        self.exp_pkt1 = simple_tcp_packet(eth_dst=self.dmac1,
                                          eth_src=ROUTER_MAC,
                                          ip_dst=self.ip_addr1,
                                          ip_src=self.ip_addr2,
                                          ip_id=105,
                                          ip_tos=0xc8,
                                          ip_ttl=63)

        self.pkt2 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                      eth_src=self.dmac1,
                                      ip_dst=self.ip_addr2,
                                      ip_src=self.ip_addr1,
                                      ip_id=105,
                                      ip_tos=0xc8,
                                      ip_ttl=64)
        self.exp_pkt2 = simple_tcp_packet(eth_dst=self.dmac2,
                                          eth_src=ROUTER_MAC,
                                          ip_dst=self.ip_addr2,
                                          ip_src=self.ip_addr1,
                                          ip_id=105,
                                          ip_tos=0xc8,
                                          ip_ttl=63)

    def runTest(self):
        try:
            self.aclRoutingTest()
            self.aclIPFragmentTest(self.table_stage_ingress)
            # self.aclIPFragmentTest(self.table_stage_egress)
        finally:
            pass

    def aclRoutingTest(self):
        """Tests routing"""
        # send the test packet(s)
        try:
            print('#### NO ACL Applied ####')
            print('#### Sending  ', ROUTER_MAC, '| ', self.dmac2, ' | ',
                  self.ip_addr1, ' | ', self.ip_addr2, ' | @ ptf_intf 2')
            send_packet(self, self.dev_port11, self.pkt1)
            print('#### Expecting ', self.dmac1, ' |', ROUTER_MAC, '| ',
                  self.ip_addr1, ' | ', self.ip_addr2, ' | @ ptf_intf 1')
            verify_packets(self, self.exp_pkt1, [self.dev_port10])

            print('#### NO ACL Applied ####')
            print('#### Sending  ', ROUTER_MAC, '| ', self.dmac1, ' | ',
                  self.ip_addr2, ' | ', self.ip_addr1, ' | @ ptf_intf 2')
            send_packet(self, self.dev_port10, self.pkt2)
            print('#### Expecting ', self.dmac2, ' |', ROUTER_MAC, '| ',
                  self.ip_addr2, ' | ', self.ip_addr1, ' | @ ptf_intf 1')
            verify_packets(self, self.exp_pkt2, [self.dev_port11])
        finally:
            print('----------------------------------------------------------')

    def aclIPFragmentTest(self, table_stage):
        """
        Tests ACL with IP frgamentation
        Args:
            table_stage (int): specifies ingress or egress type of ACL
        """
        # setup ACL to block based on Source IP
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        entry_priority = SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY

        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list), int32list=table_bind_point_list)

        acl_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_acl_ip_frag=True)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=SAI_PACKET_ACTION_DROP))

        acl_ip_frag = sai_thrift_acl_field_data_t(
            sai_thrift_acl_field_data_data_t(
                s32=SAI_ACL_IP_FRAG_ANY))

        # Add drop ACL entry to IPv6 ACL Table
        acl_entry_id = sai_thrift_create_acl_entry(
            self.client,
            priority=entry_priority,
            table_id=acl_table_id,
            action_packet_action=packet_action,
            field_acl_ip_frag=acl_ip_frag)

        try:
            if table_stage == SAI_ACL_STAGE_INGRESS:
                sai_thrift_set_port_attribute(
                    self.client, self.port11, ingress_acl=acl_table_id)
                sport = self.dev_port11
                dport = self.dev_port10
                pkt = self.pkt1
                exp_pkt = self.exp_pkt1
                ip_addr1 = self.ip_addr1
                ip_addr2 = self.ip_addr2
                dmac = self.dmac1
                smac = self.dmac2
            elif table_stage == SAI_ACL_STAGE_EGRESS:
                sai_thrift_set_port_attribute(
                    self.client, self.port11, egress_acl=acl_table_id)
                sport = self.dev_port10
                dport = self.dev_port11
                pkt = self.pkt2
                exp_pkt = self.exp_pkt2
                ip_addr1 = self.ip_addr2
                ip_addr2 = self.ip_addr1
                dmac = self.dmac2
                smac = self.dmac1

            assert acl_table_id != 0, 'acl_table_id == 0'
            assert acl_entry_id != 0, 'acl_entry_id == 0'

            print('#### ACL Applied, but non frag ####')
            print('#### Sending  ', ROUTER_MAC, ' | ', smac, ' | ',
                  ip_addr1, ' | ', ip_addr2, ' | @ ptf_intf 2')
            send_packet(self, sport, pkt)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, ' | ',
                  ip_addr1, ' | ', ip_addr2, ' | @ ptf_intf 1')
            verify_packets(self, exp_pkt, [dport])
            print('#### ACL no Drop, DF=1, offset = 0, Applied ####')
            print('#### Sending      ', ROUTER_MAC, ' | ', smac, ' | ',
                  ip_addr1, ' | ', ip_addr2, ' | @ ptf_intf 2')
            # send the same packet
            pkt['IP'].flags = 2
            exp_pkt['IP'].flags = 2
            pkt['IP'].frag = 0
            send_packet(self, sport, pkt)
            print('#### Expecting ', dmac, ' | ', ROUTER_MAC, ' | ',
                  ip_addr1, ' | ', ip_addr2, ' | @ ptf_intf 1')
            verify_packets(self, exp_pkt, [dport])
            exp_pkt['IP'].flags = 0
            print('#### ACL Drop, MF=1, offset = 0, '
                  'first fragment, Applied ####')
            print('#### Sending      ', ROUTER_MAC, ' | ', smac, ' | ',
                  ip_addr1, ' | ', ip_addr2, ' | @ ptf_intf 2')
            # send the same packet
            pkt['IP'].flags = 1
            pkt['IP'].frag = 0
            send_packet(self, sport, pkt)
            print('#### NOT Expecting ', dmac, ' |', ROUTER_MAC, ' | ',
                  ip_addr1, ' | ', ip_addr2, ' | @ ptf_intf 1')
            verify_no_other_packets(self, timeout=2)
            print('#### ACL Drop, MF=1, offset = 20, '
                  'non head fragment, Applied ####')
            print('#### Sending      ', ROUTER_MAC, ' | ', smac, ' | ',
                  ip_addr1, ' | ', ip_addr2, ' | @ ptf_intf 2')
            # send the same packet
            pkt['IP'].flags = 1
            pkt['IP'].frag = 20
            send_packet(self, sport, pkt)
            print('#### NOT Expecting ', dmac, ' | ', ROUTER_MAC, ' | ',
                  ip_addr1, ' | ', ip_addr2, ' | @ ptf_intf 1')
            verify_no_other_packets(self, timeout=2)
            print('#### ACL Drop, MF=0, offset = 20, last fragment,'
                  ' Applied ####')
            print('#### Sending      ', ROUTER_MAC, ' | ', smac, ' | ',
                  ip_addr1, ' | ', ip_addr2, ' | @ ptf_intf 2')
            # send the same packet
            pkt['IP'].flags = 0
            pkt['IP'].frag = 20
            send_packet(self, sport, pkt)
            print('#### NOT Expecting ', dmac, ' | ', ROUTER_MAC, ' | ',
                  ip_addr1, ' | ', ip_addr2, ' | @ ptf_intf 1')
            verify_no_other_packets(self, timeout=2)

        finally:
            # unbind this ACL table from ports object id
            if table_stage == SAI_ACL_STAGE_INGRESS:
                sai_thrift_set_port_attribute(
                    self.client, self.port11, ingress_acl=0)
            elif table_stage == SAI_ACL_STAGE_EGRESS:
                sai_thrift_set_port_attribute(
                    self.client, self.port11, egress_acl=0)
            # cleanup ACL
            sai_thrift_remove_acl_entry(self.client, acl_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_table_id)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry1)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_route_entry(self.client, self.route_entry2)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        super(IPAclFragmentTest, self).tearDown()


@group('acl')
class L3AclCounterTest(SaiHelper):
    """Verifies ACL counter test case"""
    def setUp(self):
        super(L3AclCounterTest, self).setUp()

        l4_src_port = 1000
        mask = '/24'
        self.ip_addr1 = '172.16.10.1'
        self.dmac1 = '00:11:22:33:44:55'
        self.ip_addr2 = '192.168.100.100'
        self.dmac2 = '00:22:22:22:22:22'

        self.table_stage_ingress = SAI_ACL_STAGE_INGRESS
        self.table_stage_egress = SAI_ACL_STAGE_EGRESS

        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress(self.ip_addr1))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry1,
            dst_mac_address=self.dmac1)

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr1),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.ip_addr1 + mask))
        sai_thrift_create_route_entry(
            self.client,
            self.route_entry1,
            next_hop_id=self.nhop1)

        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=self.port11_rif, ip_address=sai_ipaddress(self.ip_addr2))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry2,
            dst_mac_address=self.dmac2)

        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr2),
            router_interface_id=self.port11_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf,
            destination=sai_ipprefix(self.ip_addr2 + mask))
        sai_thrift_create_route_entry(
            self.client,
            self.route_entry2,
            next_hop_id=self.nhop2)

        # send the test packet(s)
        self.pkt1 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                      eth_src=self.dmac2,
                                      ip_dst=self.ip_addr1,
                                      ip_src=self.ip_addr2,
                                      tcp_sport=l4_src_port,
                                      ip_id=105,
                                      ip_ttl=64)
        self.exp_pkt1 = simple_tcp_packet(eth_dst=self.dmac1,
                                          eth_src=ROUTER_MAC,
                                          ip_dst=self.ip_addr1,
                                          ip_src=self.ip_addr2,
                                          tcp_sport=l4_src_port,
                                          ip_id=105,
                                          ip_ttl=63)

        self.pkt2 = simple_tcp_packet(eth_dst=ROUTER_MAC,
                                      eth_src=self.dmac1,
                                      ip_dst=self.ip_addr2,
                                      ip_src=self.ip_addr1,
                                      tcp_sport=l4_src_port,
                                      ip_id=105,
                                      ip_ttl=64)
        self.exp_pkt2 = simple_tcp_packet(eth_dst=self.dmac2,
                                          eth_src=ROUTER_MAC,
                                          ip_dst=self.ip_addr2,
                                          ip_src=self.ip_addr1,
                                          tcp_sport=l4_src_port,
                                          ip_id=105,
                                          ip_ttl=63)

    def runTest(self):
        try:
            self.aclRoutingTest()
            self.l3AclCounterTest(self.table_stage_ingress)
            self.l3AclCounterTest(self.table_stage_egress)
        finally:
            pass

    def aclRoutingTest(self):
        """Tests ACL configuration"""
        print('--------------------------------------------------------------')
        print("Sending packet ptf_intf 2 -> ptf_intf 1 (", self.ip_addr2,
              " ---> ", self.ip_addr1, " [id = 105])")
        try:
            print('#### NO ACL Applied ####')
            print('#### Sending  ', ROUTER_MAC, ' | ', self.dmac2, ' | ',
                  self.ip_addr2, ' | ', self.ip_addr1, ' | SPORT 1000 | '
                  '@ ptf_intf 2')
            send_packet(self, self.dev_port11, self.pkt1)
            print('#### Expecting ', self.dmac1, ' | ', ROUTER_MAC, ' | ',
                  self.ip_addr2, ' | ', self.ip_addr1, ' | SPORT 1000 | '
                  '@ ptf_intf 1')
            verify_packets(self, self.exp_pkt1, [self.dev_port10])

            print('----------------------------------------------------------')
            print("Sending packet ptf_intf 2 -> ptf_intf 1 (", self.ip_addr1,
                  " ---> ", self.ip_addr2, " [id = 105])")
            print('#### NO ACL Applied ####')
            print('#### Sending  ', ROUTER_MAC, '| ', self.dmac1, ' | ',
                  self.ip_addr1, ' | ', self.ip_addr2, ' | SPORT 1000 | '
                  '@ ptf_intf 2')
            send_packet(self, self.dev_port10, self.pkt2)
            print('#### Expecting ', self.dmac2, ' |', ROUTER_MAC, '| ',
                  self.ip_addr1, ' | ', self.ip_addr2, ' | SPORT 1000 | '
                  '@ ptf_intf 1')
            verify_packets(self, self.exp_pkt2, [self.dev_port11])
        finally:
            print('----------------------------------------------------------')

    def l3AclCounterTest(self, table_stage):
        """
        Tests acl with action counter
        Args:
            table_stage (int): specifies the type of ACL table stage
        """
        print("Testing L3AclCounterTest")

        # setup ACL to block based on Source IP and SPORT
        entry_priority = 1
        action = SAI_PACKET_ACTION_DROP
        ip_src_mask = "255.255.255.0"

        if table_stage == SAI_ACL_STAGE_INGRESS:
            ip_src = "192.168.100.1"
            ip_src_addr = "192.168.100.100"
            ip_dst = self.ip_addr1
            ip_dst_addr = "172.16.10.1"
            dmac = self.dmac1
            smac = self.dmac2
        elif table_stage == SAI_ACL_STAGE_EGRESS:
            ip_src = "172.16.10.1"
            ip_src_addr = "172.16.10.1"
            ip_dst = "192.168.100.1"
            ip_dst_addr = "192.168.100.100"
            dmac = self.dmac2
            smac = self.dmac1

        print("Sending packet ptf_intf 2 -[acl]-> ptf_intf 1 "
              "(", ip_src, "-[acl]-> ", ip_dst, " [id = 105])")

        table_bind_points = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_points), int32list=table_bind_points)

        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ip_src),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ip_src_mask))

        u32range = sai_thrift_u32_range_t(min=1000, max=1000)
        acl_range_id = sai_thrift_create_acl_range(
            self.client,
            type=SAI_ACL_RANGE_TYPE_L4_SRC_PORT_RANGE,
            limit=u32range)
        range_list = [acl_range_id]

        range_list_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(
                objlist=sai_thrift_object_list_t(
                    count=len(range_list),
                    idlist=range_list)))

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=action))

        acl_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        acl_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_id,
            priority=entry_priority,
            field_src_ip=src_ip_t,
            action_packet_action=packet_action,
            field_acl_range_type=range_list_t)

        # bind this ACL table to rif_id2s object id
        if table_stage == SAI_ACL_STAGE_INGRESS:
            sai_thrift_set_router_interface_attribute(
                self.client, self.port11_rif, ingress_acl=acl_table_id)
            pkt = self.pkt1
            sport = self.dev_port11
        elif table_stage == SAI_ACL_STAGE_EGRESS:
            sai_thrift_set_router_interface_attribute(
                self.client, self.port11_rif, egress_acl=acl_table_id)
            pkt = self.pkt2
            sport = self.dev_port10

        # create ACL counter and bind it to the ACL entry
        acl_counter_id = sai_thrift_create_acl_counter(
            client=self.client, table_id=acl_table_id)
        action_counter_t = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(oid=acl_counter_id),
            enable=True)
        sai_thrift_set_acl_entry_attribute(
            self.client, acl_entry_id,
            action_counter=action_counter_t)

        try:
            assert acl_table_id != 0, 'acl_table_id == 0'
            assert acl_entry_id != 0, 'acl_entry_id == 0'
            assert acl_counter_id != 0, 'acl_counter_id == 0'
            pkt_cnt = 5

            attr_values = sai_thrift_get_acl_counter_attribute(
                self.client, acl_counter_id, packets=True, bytes=True)

            initial_pkts_cnt = attr_values["packets"]
            initial_bytes_cnt = attr_values["bytes"]

            print('#### ACL \'DROP, src ip ', ip_src, '/', ip_src_mask, ', '
                  'SPORT 1000, in_ports[ptf_intf_1,2]\' Applied ####')
            print('#### Sending  ', ROUTER_MAC, '| ', smac, ' | ', ip_src_addr,
                  ' | ', ip_dst_addr, ' | SPORT 1000 | @ ptf_intf 1')
            # send the same packet
            for i in range(0, pkt_cnt):
                print(i, pkt_cnt)
                send_packet(self, sport, pkt)
            # ensure packets are dropped
            # check for absence of packets here!
            print('#### NOT Expecting ', dmac, ' | ', ROUTER_MAC, ' | ',
                  ip_src_addr, ' | ', ip_dst_addr, ' | SPORT 1000 | '
                  '@ ptf_intf 0')
            verify_no_other_packets(self, timeout=1)

            time.sleep(2)

            attr_values = sai_thrift_get_acl_counter_attribute(
                self.client, acl_counter_id, packets=True, bytes=True)

            actual_pkts_cnt = (attr_values["packets"] - initial_pkts_cnt)
            print(actual_pkts_cnt)
            actual_bytes_cnt = (attr_values["bytes"] - initial_bytes_cnt)
            print(actual_bytes_cnt)

            self.assertEqual(actual_pkts_cnt, pkt_cnt, "packets counter value "
                             "actual_pkts_cnt is not pkt_cnt")

        finally:
            # unbind this ACL table from rif_id object id
            if table_stage == SAI_ACL_STAGE_INGRESS:
                sai_thrift_set_router_interface_attribute(
                    self.client, self.port11_rif, ingress_acl=0)
            elif table_stage == SAI_ACL_STAGE_EGRESS:
                sai_thrift_set_router_interface_attribute(
                    self.client, self.port11_rif, egress_acl=0)
            # cleanup ACL
            sai_thrift_remove_acl_entry(self.client, acl_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_table_id)
            sai_thrift_remove_acl_counter(self.client, acl_counter_id)
            sai_thrift_remove_acl_range(self.client, acl_range_id)

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry1)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_route_entry(self.client, self.route_entry2)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        super(L3AclCounterTest, self).tearDown()


@group('acl')
@group('acl-ocp')
class VlanAclTest(SaiHelper):
    """Verifies acl vlan test case"""
    def setUp(self):
        super(VlanAclTest, self).setUp()
        print("Sending L2 packet - port 24 -> port 25 [trunk vlan=100])")
        vlan_id = 100
        mac1 = '00:11:11:11:11:11'
        mac2 = '00:22:22:22:22:22'
        self.ip_addr1 = '192.168.100.1'
        self.ip_addr2 = '172.16.0.1'
        mac_action = SAI_PACKET_ACTION_FORWARD
        self.table_stage_ingress = SAI_ACL_STAGE_INGRESS
        self.table_stage_egress = SAI_ACL_STAGE_EGRESS
        self.group_stage_ingress = SAI_ACL_STAGE_INGRESS
        self.group_stage_egress = SAI_ACL_STAGE_EGRESS

        self.vlan_oid = sai_thrift_create_vlan(self.client, vlan_id)

        self.port24_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        self.vlan_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan_oid,
            bridge_port_id=self.port24_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        self.port25_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port25,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)

        self.vlan_member2 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan_oid,
            bridge_port_id=self.port25_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

        self.fdb_entry1 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id, mac_address=mac1, bv_id=self.vlan_oid)
        sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry1,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port24_bp,
            packet_action=mac_action)

        self.fdb_entry2 = sai_thrift_fdb_entry_t(
            switch_id=self.switch_id, mac_address=mac2, bv_id=self.vlan_oid)
        sai_thrift_create_fdb_entry(
            self.client,
            self.fdb_entry2,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=self.port25_bp,
            packet_action=mac_action)

        self.pkt1 = simple_tcp_packet(eth_dst=mac2,
                                      eth_src=mac1,
                                      dl_vlan_enable=True,
                                      vlan_vid=100,
                                      ip_src=self.ip_addr1,
                                      ip_dst=self.ip_addr2,
                                      ip_id=102,
                                      ip_ttl=64)
        self.exp_pkt1 = simple_tcp_packet(eth_dst=mac2,
                                          eth_src=mac1,
                                          ip_dst=self.ip_addr2,
                                          ip_src=self.ip_addr1,
                                          ip_id=102,
                                          dl_vlan_enable=True,
                                          vlan_vid=100,
                                          ip_ttl=64)

        self.pkt2 = simple_tcp_packet(eth_dst=mac1,
                                      eth_src=mac2,
                                      dl_vlan_enable=True,
                                      vlan_vid=100,
                                      ip_src=self.ip_addr2,
                                      ip_dst=self.ip_addr1,
                                      ip_id=102,
                                      ip_ttl=64)
        self.exp_pkt2 = simple_tcp_packet(eth_dst=mac1,
                                          eth_src=mac2,
                                          ip_dst=self.ip_addr1,
                                          ip_src=self.ip_addr2,
                                          ip_id=102,
                                          dl_vlan_enable=True,
                                          vlan_vid=100,
                                          ip_ttl=64)

    def runTest(self):
        try:
            self.noAclTest()
            self.aclVlanTest(self.table_stage_ingress,
                             self.group_stage_ingress)
            self.aclVlanTest(self.table_stage_egress,
                             self.group_stage_egress)
        finally:
            pass

    def tearDown(self):
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry1)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry2)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member1)
        sai_thrift_remove_vlan_member(self.client, self.vlan_member2)
        sai_thrift_remove_bridge_port(self.client, self.port24_bp)
        sai_thrift_remove_bridge_port(self.client, self.port25_bp)
        sai_thrift_remove_vlan(self.client, self.vlan_oid)
        super(VlanAclTest, self).tearDown()

    def noAclTest(self):
        """Tests communication without acl"""
        print('#### NO ACL Applied ####')
        # send the test packet(s)
        print("Sending TCP type test packet port 24 -> port 25")
        send_packet(self, self.dev_port24, self.pkt1)
        verify_packets(self, self.exp_pkt1, [self.dev_port25])
        print("Sending TCP type test packet port 25 -> port 24")
        send_packet(self, self.dev_port25, self.pkt2)
        verify_packets(self, self.exp_pkt2, [self.dev_port24])

    def aclVlanTest(self, table_stage, group_stage):
        """
        Tests acl with vlan
        Args:
            table_stage (int): specifies the type of ACL table stage
            group_stage (int): specifies the type of ACL group stage
        """
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_VLAN]
        entry_priority = 1

        group_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_VLAN]
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL
        group_member_priority = 100

        acl_action = SAI_PACKET_ACTION_DROP
        ip_src_mask = "255.255.255.0"

        if table_stage == SAI_ACL_STAGE_INGRESS:
            ip_src = self.ip_addr1
            sport = self.dev_port24
            pkt = self.pkt1
        elif table_stage == SAI_ACL_STAGE_EGRESS:
            ip_src = self.ip_addr2
            sport = self.dev_port25
            pkt = self.pkt2

        group_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(group_bind_point_list),
            int32list=group_bind_point_list)

        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=acl_action))

        acl_table_group_id = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=group_stage,
            acl_bind_point_type_list=group_bind_point_type_list,
            type=group_type)

        acl_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ip_src),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ip_src_mask))

        acl_table_group_member_id = sai_thrift_create_acl_table_group_member(
            self.client,
            acl_table_group_id=acl_table_group_id,
            acl_table_id=acl_table_id,
            priority=group_member_priority)

        acl_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_id,
            priority=entry_priority,
            field_src_ip=src_ip_t,
            action_packet_action=packet_action)

        if table_stage == SAI_ACL_STAGE_INGRESS:
            sai_thrift_set_vlan_attribute(self.client,
                                          vlan_oid=self.vlan_oid,
                                          ingress_acl=acl_table_group_id)
        elif table_stage == SAI_ACL_STAGE_EGRESS:
            sai_thrift_set_vlan_attribute(self.client,
                                          vlan_oid=self.vlan_oid,
                                          egress_acl=acl_table_group_id)

        try:
            send_packet(self, sport, pkt)
            verify_no_other_packets(self, timeout=1)
        finally:
            if table_stage == SAI_ACL_STAGE_INGRESS:
                sai_thrift_set_vlan_attribute(self.client,
                                              vlan_oid=self.vlan_oid,
                                              ingress_acl=0)
            elif table_stage == SAI_ACL_STAGE_EGRESS:
                sai_thrift_set_vlan_attribute(self.client,
                                              vlan_oid=self.vlan_oid,
                                              egress_acl=0)
            sai_thrift_remove_acl_table_group_member(
                self.client, acl_table_group_member_id)
            sai_thrift_remove_acl_entry(self.client, acl_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_table_id)
            sai_thrift_remove_acl_table_group(self.client, acl_table_group_id)


class AclLagTest(SaiHelper):
    """Verifies acl with lag test case"""
    def setUp(self):
        super(AclLagTest, self).setUp()

        self.lag_id = sai_thrift_create_lag(self.client)

        self.lag_member_id1 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag_id, port_id=self.port24)

        self.vrf = sai_thrift_create_virtual_router(self.client)
        self.rif_id1 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.vrf,
            port_id=self.lag_id)

        self.rif_id2 = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.vrf,
            port_id=self.port26)

        self.ip_addr1 = "20.0.0.2"
        self.ip_addr_subnet1 = '20.0.0.0'
        self.dmac1 = '00:22:22:22:22:22'
        self.ip_addr_subnet2 = '192.168.0.0'
        self.ip_addr2 = '192.168.0.1'
        self.dmac2 = '00:11:22:33:44:55'
        self.ip_src_mask = "255.255.255.255"
        mask = '/16'

        self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.rif_id1, ip_address=sai_ipaddress(self.ip_addr2))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry1,
            dst_mac_address=self.dmac1)

        self.nhop1 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr2),
            router_interface_id=self.rif_id1,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.vrf,
            destination=sai_ipprefix(self.ip_addr_subnet2 + mask))
        sai_thrift_create_route_entry(
            self.client,
            self.route_entry1,
            next_hop_id=self.nhop1)

        self.neighbor_entry2 = sai_thrift_neighbor_entry_t(
            rif_id=self.rif_id2, ip_address=sai_ipaddress(self.ip_addr1))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry2,
            dst_mac_address=self.dmac2)

        self.nhop2 = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(self.ip_addr1),
            router_interface_id=self.rif_id2,
            type=SAI_NEXT_HOP_TYPE_IP)

        self.route_entry2 = sai_thrift_route_entry_t(
            vr_id=self.vrf,
            destination=sai_ipprefix(self.ip_addr_subnet1 + mask))
        sai_thrift_create_route_entry(
            self.client,
            self.route_entry2,
            next_hop_id=self.nhop2)

    def runTest(self):
        self.lagAclEgressTest()
        self.lagAclIngressTest()

    def tearDown(self):
        sai_thrift_remove_route_entry(self.client, self.route_entry2)
        sai_thrift_remove_next_hop(self.client, self.nhop2)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry2)
        sai_thrift_remove_route_entry(self.client, self.route_entry1)
        sai_thrift_remove_next_hop(self.client, self.nhop1)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry1)
        sai_thrift_remove_router_interface(self.client, self.rif_id2)
        sai_thrift_remove_router_interface(self.client, self.rif_id1)
        sai_thrift_remove_lag_member(self.client, self.lag_member_id1)
        sai_thrift_remove_lag(self.client, self.lag_id)
        sai_thrift_remove_virtual_router(self.client, self.vrf)
        super(AclLagTest, self).tearDown()

    def lagAclEgressTest(self):
        '''
        Tests egress acl with lag
        "ACL_RULE|ACL_TABLE_IPV4_ID|RULE_1": {
            "type": "hash",
            "value": {
                "PACKET_ACTION": "DROP",
                "PRIORITY": "9999",
                "SRC_IP": "20.0.0.2/32"
            }
        },
        '''
        table_stage_egress = SAI_ACL_STAGE_EGRESS
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        action = SAI_PACKET_ACTION_DROP

        ip_src = self.ip_addr1
        ip_dst = self.ip_addr_subnet2
        sport = self.dev_port26
        dport1 = self.dev_port24
        dport2 = self.dev_port25

        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=action))

        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ip_src),
            mask=sai_thrift_acl_field_data_mask_t(ip4=self.ip_src_mask))

        acl_table_ipv4_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_egress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        acl_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_ipv4_id,
            priority=9999,
            field_src_ip=src_ip_t,
            action_packet_action=packet_action)

        try:
            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src=self.dmac1,
                ip_src=ip_src,
                ip_dst=ip_dst,
                tcp_sport=0x4321,
                tcp_dport=0x51,
                ip_ttl=64)
            exp_pkt = simple_tcp_packet(
                eth_dst=self.dmac1,
                eth_src=ROUTER_MAC,
                ip_src=ip_src,
                ip_dst=ip_dst,
                tcp_sport=0x4321,
                tcp_dport=0x51,
                ip_ttl=63)

            send_packet(self, sport, pkt)
            verify_packets(self, exp_pkt, ports=[dport1])

            # Now bind the ACL table - the packet should be dropped
            sai_thrift_set_lag_attribute(self.client,
                                         lag_oid=self.lag_id,
                                         egress_acl=acl_table_ipv4_id)

            send_packet(self, sport, pkt)
            verify_no_other_packets(self, timeout=1)

            # Add one more LAG member and verify the packet
            # is not forwarded to it
            lag_member_id2 = sai_thrift_create_lag_member(
                self.client, lag_id=self.lag_id, port_id=self.port25)
            send_packet(self, sport, pkt)
            verify_no_other_packets(self, timeout=1)

            # Now unbind the ACL table
            sai_thrift_set_lag_attribute(self.client,
                                         lag_oid=self.lag_id,
                                         egress_acl=0)

            send_packet(self, sport, pkt)
            verify_any_packet_any_port(
                self, [exp_pkt], [dport1, dport2], timeout=2)

        finally:
            if lag_member_id2:
                sai_thrift_remove_lag_member(self.client, lag_member_id2)
            sai_thrift_remove_acl_entry(self.client, acl_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_table_ipv4_id)

    def lagAclIngressTest(self):
        '''
        Tests ingress acl with lag
        "ACL_RULE|ACL_TABLE_IPV4_ID|RULE_1": {
            "type": "hash",
            "value": {
                "PACKET_ACTION": "DROP",
                "PRIORITY": "9999",
                "SRC_IP": "192.168.0.1/32"
            }
        },
        '''
        table_stage_ingress = SAI_ACL_STAGE_INGRESS
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        action = SAI_PACKET_ACTION_DROP

        ip_src = self.ip_addr2
        ip_dst = self.ip_addr_subnet1
        dport = self.dev_port26
        sport1 = self.dev_port24
        sport2 = self.dev_port27

        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=action))

        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ip_src),
            mask=sai_thrift_acl_field_data_mask_t(ip4=self.ip_src_mask))

        acl_table_ipv4_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage_ingress,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True)

        acl_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_ipv4_id,
            priority=9999,
            field_src_ip=src_ip_t,
            action_packet_action=packet_action)

        try:
            pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src=self.dmac2,
                ip_src=ip_src,
                ip_dst=ip_dst,
                tcp_sport=0x4321,
                tcp_dport=0x51,
                ip_ttl=64)
            exp_pkt = simple_tcp_packet(
                eth_dst=self.dmac2,
                eth_src=ROUTER_MAC,
                ip_src=ip_src,
                ip_dst=ip_dst,
                tcp_sport=0x4321,
                tcp_dport=0x51,
                ip_ttl=63)

            # Add one more LAG member
            lag_member_id2 = sai_thrift_create_lag_member(
                self.client, lag_id=self.lag_id, port_id=self.port27)

            send_packet(self, sport1, pkt)
            verify_packets(self, exp_pkt, ports=[dport])

            send_packet(self, sport2, pkt)
            verify_packets(self, exp_pkt, ports=[dport])

            # Now bind the ACL table - the packet should be dropped
            sai_thrift_set_lag_attribute(self.client,
                                         lag_oid=self.lag_id,
                                         ingress_acl=acl_table_ipv4_id)

            send_packet(self, sport1, pkt)
            verify_no_other_packets(self, timeout=1)

            send_packet(self, sport2, pkt)
            verify_no_other_packets(self, timeout=1)

            # Now unbind the ACL table
            sai_thrift_set_lag_attribute(self.client,
                                         lag_oid=self.lag_id,
                                         ingress_acl=0)

        finally:
            if lag_member_id2:
                sai_thrift_remove_lag_member(self.client, lag_member_id2)
            sai_thrift_remove_acl_entry(self.client, acl_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_table_ipv4_id)


class IngressL3AclDscp(SaiHelper):
    """Verifies acl test case with the dscp field"""
    def setUp(self):
        super(IngressL3AclDscp, self).setUp()

        l4_dst_port = 1000
        ip_addr = '172.16.10.1'
        dmac = '00:11:22:33:44:55'

        self.neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.port10_rif, ip_address=sai_ipaddress(ip_addr))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry,
            dst_mac_address=dmac)

        self.nhop = sai_thrift_create_next_hop(
            self.client,
            ip=sai_ipaddress(ip_addr),
            router_interface_id=self.port10_rif,
            type=SAI_NEXT_HOP_TYPE_IP)

        # send the test packet(s)
        self.pkt = simple_tcp_packet(
            eth_dst=ROUTER_MAC,
            eth_src='00:22:22:22:22:22',
            ip_dst='172.16.10.1',
            ip_src='192.168.100.100',
            tcp_dport=l4_dst_port,
            ip_id=105,
            ip_ttl=64,
            ip_tos=200)
        self.exp_pkt = simple_tcp_packet(
            eth_dst='00:11:22:33:44:55',
            eth_src=ROUTER_MAC,
            ip_dst='172.16.10.1',
            ip_src='192.168.100.100',
            tcp_dport=l4_dst_port,
            ip_id=105,
            ip_ttl=63,
            ip_tos=200)

    def runTest(self):
        self.routingTest()
        self.ingressL3AclDscpTest()

    def tearDown(self):
        sai_thrift_remove_next_hop(self.client, self.nhop)
        sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry)
        super(IngressL3AclDscp, self).tearDown()

    def routingTest(self):
        """Tests basic routing"""
        print('--------------------------------------------------------------')
        print("Sending packet ptf_intf 2 -> ptf_intf 1 (192.168.100.100 ---> "
              "172.16.10.1 [id = 105])")
        try:
            print('#### NO ACL Applied ####')
            print('#### Sending  ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 2')
            send_packet(self, self.dev_port11, self.pkt)
            print('#### Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| 172.16.'
                  '10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            verify_packets(self, self.exp_pkt, [self.dev_port10])
        finally:
            print('----------------------------------------------------------')

    def ingressL3AclDscpTest(self):
        """Tests acl with the dscp field"""
        print("Sending packet ptf_intf 2 -[acl]-> ptf_intf 1 "
              "(192.168.0.1-[acl]-> 172.16.10.1 [id = 105])")
        # setup ACL to block based on Source IP and SPORT
        table_stage = SAI_ACL_STAGE_INGRESS
        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF]
        entry_priority = 1
        action = SAI_PACKET_ACTION_DROP
        ip_src = "192.168.100.1"
        ip_src_mask = "255.255.255.0"

        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=action))

        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ip_src),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ip_src_mask))

        u32range = sai_thrift_u32_range_t(min=1000, max=1000)
        acl_range_id = sai_thrift_create_acl_range(
            self.client,
            type=SAI_ACL_RANGE_TYPE_L4_DST_PORT_RANGE,
            limit=u32range)
        range_list = [acl_range_id]

        print("ACL range created 0x%lx" % (acl_range_id))

        range_list_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(
                objlist=sai_thrift_object_list_t(
                    count=len(range_list),
                    idlist=range_list)))

        field_dscp = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u8=50))

        acl_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=table_stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ip=True,
            field_dscp=True)
        print("ACL Table created 0x%lx" % (acl_table_id))

        acl_entry_id = sai_thrift_create_acl_entry(
            self.client,
            table_id=acl_table_id,
            priority=entry_priority,
            field_src_ip=src_ip_t,
            action_packet_action=packet_action,
            field_acl_range_type=range_list_t,
            field_dscp=field_dscp)

        # bind this ACL table to rif_id2s object id
        sai_thrift_set_router_interface_attribute(
            self.client, self.port11_rif, ingress_acl=acl_table_id)

        try:
            assert acl_table_id != 0, 'acl_table_id is == 0'
            assert acl_entry_id != 0, 'acl_entry_id is == 0'

            print('#### ACL \'DROP, src ip 192.168.100.1/255.255.255.0, '
                  'SPORT 1000, in_ports[ptf_intf_1,2]\' Applied ####')
            print('#### Sending      ', ROUTER_MAC, '| 00:22:22:22:22:22 | '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 1')
            # send the same packet
            send_packet(self, self.dev_port11, self.pkt)
            # ensure packet is dropped
            # check for absence of packet here!
            print('#### NOT Expecting 00:11:22:33:44:55 |', ROUTER_MAC, '| '
                  '172.16.10.1 | 192.168.100.100 | SPORT 1000 | @ ptf_intf 0')
            verify_no_other_packets(self, timeout=1)
        finally:
            # unbind this ACL table from rif_id2s object id
            sai_thrift_set_router_interface_attribute(
                self.client, self.port11_rif, ingress_acl=0)
            # cleanup ACL
            sai_thrift_remove_acl_entry(self.client, acl_entry_id)
            sai_thrift_remove_acl_table(self.client, acl_table_id)
            sai_thrift_remove_acl_range(self.client, acl_range_id)


# WIP
@disabled
class IPEthAclTest(SaiHelper):
    """
    This test mimics SONiC/SAI ACL behavior. SONiC does not have notion
    of MAC ACL. It programs entries with non ip eth type match entries
    into IP ACL. This is different than the SDE behavior which expects
    this entries to be programmed into MAC ACL. This test verifies
    the SDE behavior to be able to identify such acl rules and program
    it correctly in the corresponding H/W ACL tables.
    """
    def setUp(self):
        super(IPEthAclTest, self).setUp()
        self.traps = []
        self.acl_grp_members = []
        self.acl_grps = []
        self.acl_rules = []
        self.acl_tables = []
        self.acl_counters = []
        self.vlans = []
        self.vlan_members = []
        self.bridge_ports = []
        self.vlan_ports = []
        self.bp_list = []

        self.rif_id = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port28)

        # Add port 24, 25 to Vlan100
        vlan_id1 = 100
        vlan_oid1 = sai_thrift_create_vlan(self.client, vlan_id1)
        self.vlans.append(vlan_oid1)

        port1_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port24,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.bridge_ports.append(port1_bp)
        vlan_member1 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan_oid1,
            bridge_port_id=port1_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_members.append(vlan_member1)
        self.vlan_ports.append(self.port24)

        port2_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port25,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.bridge_ports.append(port2_bp)
        vlan_member2 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan_oid1,
            bridge_port_id=port2_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_members.append(vlan_member2)
        self.vlan_ports.append(self.port25)

        sai_thrift_set_port_attribute(
            self.client, self.port24, port_vlan_id=vlan_id1)
        sai_thrift_set_port_attribute(
            self.client, self.port25, port_vlan_id=vlan_id1)

        # Add port 26, 27 to Vlan200
        vlan_id2 = 200
        vlan_oid2 = sai_thrift_create_vlan(self.client, vlan_id2)
        self.vlans.append(vlan_oid2)

        port3_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port26,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.bridge_ports.append(port3_bp)
        vlan_member3 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan_oid2,
            bridge_port_id=port3_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_members.append(vlan_member3)
        self.vlan_ports.append(self.port26)

        port4_bp = sai_thrift_create_bridge_port(
            self.client,
            bridge_id=self.default_1q_bridge,
            port_id=self.port27,
            type=SAI_BRIDGE_PORT_TYPE_PORT,
            admin_state=True)
        self.bridge_ports.append(port4_bp)
        vlan_member4 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=vlan_oid2,
            bridge_port_id=port4_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_members.append(vlan_member4)
        self.vlan_ports.append(self.port27)

        sai_thrift_set_port_attribute(
            self.client, self.port26, port_vlan_id=vlan_id2)
        sai_thrift_set_port_attribute(
            self.client, self.port27, port_vlan_id=vlan_id2)

        self.arp_pkt = simple_arp_packet(arp_op=1, pktlen=100)
        self.exp_arp_pkt = self.arp_pkt

        # Setup Mirror ACL
        monitor_port = self.port28
        mirror_type = SAI_MIRROR_SESSION_TYPE_LOCAL
        self.spanid = sai_thrift_create_mirror_session(
            self.client,
            type=mirror_type,
            monitor_port=monitor_port)
        print(self.spanid)
        print('Mirror session created 0x%lx' % self.spanid)
        mirror_session_list = sai_thrift_object_list_t(
            count=1, idlist=[self.spanid])

        self.mirror_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                objlist=mirror_session_list))

        self.ipx_pkt = simple_eth_packet(pktlen=100,
                                         eth_dst='00:01:02:03:04:05',
                                         eth_src='00:06:07:08:09:0a',
                                         eth_type=0x8137)
        self.exp_ipx_pkt = self.ipx_pkt

        self.ipv4_pkt = simple_ip_packet(pktlen=100,
                                         eth_dst='00:1B:19:00:00:00')
        self.exp_ipv4_pkt = self.ipv4_pkt

        self.lldp_pkt = simple_eth_packet(pktlen=100,
                                          eth_dst='01:80:C2:00:00:0E',
                                          eth_type=0x88cc)
        self.exp_lldp_pkt = self.lldp_pkt

        self.tcpv6_pkt = simple_tcpv6_packet(pktlen=100, ipv6_src='2000::4')
        self.exp_tcpv6_pkt = self.tcpv6_pkt

        self.tcpv4_pkt = simple_tcp_packet(pktlen=100, ip_src='172.0.0.17')
        self.exp_tcpv4_pkt = self.tcpv4_pkt

        self.tcpv6_pkt2 = simple_tcpv6_packet(pktlen=100, ipv6_src='3000::1')
        self.exp_tcpv6_pkt2 = self.tcpv6_pkt2

        self.udpv6_pkt = simple_udpv6_packet(pktlen=100, ipv6_src='5000::1')
        self.exp_udpv6_pkt = self.udpv6_pkt

        self.udpv4_pkt = simple_udp_packet(pktlen=100, ip_src='10.0.0.1')
        self.exp_udpv4_pkt = self.udpv4_pkt

        self.mac_pkt = simple_eth_packet(pktlen=100,
                                         eth_dst='00:01:02:03:04:05',
                                         eth_src='00:06:07:08:09:0a',
                                         eth_type=0x1234)
        self.exp_mac_pkt = self.mac_pkt

        self.mac_pkt2 = simple_eth_packet(pktlen=100,
                                          eth_dst='00:01:02:03:04:05',
                                          eth_src='00:06:07:08:09:0a',
                                          eth_type=0x4321)
        self.exp_mac_pkt2 = self.mac_pkt2

        self.exp_counters = {"arp": 0, "ipx": 0, "ipv4": 0, "lldp": 0,
                             "ipv6": 0, "mirror_ip4": 0, "mirror_ip6": 0,
                             "mirror_udp": 0, "mirror_mac": 0}

    def runTest(self):
        self.noAclTest()
        self.arpAclTest()
        self.ipxAclTest()
        self.ipv4AclTest()
        self.lldpAclTest()
        self.ipv4MirrorAclTest()
        self.ipv6MirrorAclTest()
        self.udpMirrorAclTest()
        self.macMirrorAclTest()

    def tearDown(self):
        sai_thrift_remove_mirror_session(self.client, self.spanid)
        for vlan_port in list(self.vlan_ports):
            sai_thrift_set_port_attribute(
                self.client, vlan_port, port_vlan_id=int(SAI_NULL_OBJECT_ID))
            self.vlan_ports.remove(vlan_port)
        for vlan_member in list(self.vlan_members):
            sai_thrift_remove_vlan_member(self.client, vlan_member)
            self.vlan_members.remove(vlan_member)
        for vlan in list(self.vlans):
            sai_thrift_remove_vlan(self.client, vlan)
            self.vlans.remove(vlan)
        for bridge_port in self.bridge_ports:
            sai_thrift_remove_bridge_port(self.client, bridge_port)
            self.bridge_ports.remove(bridge_port)
        sai_thrift_remove_router_interface(self.client, self.rif_id)
        super(IPEthAclTest, self).tearDown()

    def noAclTest(self):
        """Tests routing without ACL"""
        print("Sending ARP packet - port 24 -> port 25 [access vlan=100])")
        print("Sending ARP packet - port 26 -> port 27 [access vlan=200])")
        # send & verify test packet(s) : ARP, IPX, IPv4, LLDP,
        # IPv6 without any ACL entries
        send_packet(self, self.dev_port24, self.arp_pkt)
        verify_packets(self, self.exp_arp_pkt, [self.dev_port25])
        send_packet(self, self.dev_port26, self.arp_pkt)
        verify_packets(self, self.exp_arp_pkt, [self.dev_port27])

        print("Sending IPX packet - port 24 -> port 25 [access vlan=100])")
        print("Sending IPX packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port24, self.ipx_pkt)
        verify_packets(self, self.exp_ipx_pkt, [self.dev_port25])
        send_packet(self, self.dev_port26, self.ipx_pkt)
        verify_packets(self, self.exp_ipx_pkt, [self.dev_port27])

        print("Sending IPv4 packet - port 24 -> port 25 [access vlan=100])")
        print("Sending IPv4 packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port24, self.ipv4_pkt)
        verify_packets(self, self.exp_ipv4_pkt, [self.dev_port25])
        send_packet(self, self.dev_port26, self.ipv4_pkt)
        verify_packets(self, self.exp_ipv4_pkt, [self.dev_port27])

        print("Sending LLDP packet - port 24 -> port 25 [access vlan=100])")
        print("Sending LLDP packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port24, self.lldp_pkt)
        verify_packets(self, self.exp_lldp_pkt, [self.dev_port25])
        send_packet(self, self.dev_port26, self.lldp_pkt)
        verify_packets(self, self.exp_lldp_pkt, [self.dev_port27])

        print("Sending IPv6/TCP(SRC_IP 2000::4) packet - port 24 -> "
              "port 25 [access vlan=100])")
        print("Sending IPv6/TCP(SRC_IP 2000::4) packet - port 26 -> "
              "port 27 [access vlan=200])")
        send_packet(self, self.dev_port24, self.tcpv6_pkt)
        verify_packets(self, self.exp_tcpv6_pkt, [self.dev_port25])
        send_packet(self, self.dev_port26, self.tcpv6_pkt)
        verify_packets(self, self.exp_tcpv6_pkt, [self.dev_port27])

        print("Sending IPv4/TCP packet(SRC_IP 172.0.0.17) - port 24 -> "
              "port 25 [access vlan=100])")
        print("Sending IPv4/TCP packet(SRC_IP 172.0.0.17) - port 26 -> "
              "port 27 [access vlan=200])")
        send_packet(self, self.dev_port24, self.tcpv4_pkt)
        verify_packets(self, self.exp_tcpv4_pkt, [self.dev_port25])
        send_packet(self, self.dev_port26, self.tcpv4_pkt)
        verify_packets(self, self.exp_tcpv4_pkt, [self.dev_port27])

        print("Sending IPv6/TCP packet(SRC_IP 3000::1) - port 24 -> "
              "port 25 [access vlan=100])")
        print("Sending IPv6/TCP packet(SRC_IP 3000::1) - port 26 -> "
              "port 27 [access vlan=200])")
        send_packet(self, self.dev_port24, self.tcpv6_pkt2)
        verify_packets(self, self.exp_tcpv6_pkt2, [self.dev_port25])
        send_packet(self, self.dev_port26, self.tcpv6_pkt2)
        verify_packets(self, self.exp_tcpv6_pkt2, [self.dev_port27])

        print("Sending IPv6/UDP packet - port 24 -> "
              "port 25 [access vlan=100])")
        print("Sending IPv6/UDP packet - port 26 -> "
              "port 27 [access vlan=200])")
        send_packet(self, self.dev_port24, self.udpv6_pkt)
        verify_packets(self, self.exp_udpv6_pkt, [self.dev_port25])
        send_packet(self, self.dev_port26, self.udpv6_pkt)
        verify_packets(self, self.exp_udpv6_pkt, [self.dev_port27])

        print("Sending IPv4/UDP packet - port 24 -> "
              "port 25 [access vlan=100])")
        print("Sending IPv4/UDP packet - port 26 -> "
              "port 27 [access vlan=200])")
        send_packet(self, self.dev_port24, self.udpv4_pkt)
        verify_packets(self, self.exp_udpv4_pkt, [self.dev_port25])
        send_packet(self, self.dev_port26, self.udpv4_pkt)
        verify_packets(self, self.exp_udpv4_pkt, [self.dev_port27])

        print("Sending EthType(0x1234) packet - port 24 -> "
              "port 25 [access vlan=100])")
        print("Sending EthType(0x1234) packet - port 26 -> "
              "port 27 [access vlan=200])")
        send_packet(self, self.dev_port24, self.mac_pkt)
        verify_packets(self, self.exp_mac_pkt, [self.dev_port25])
        send_packet(self, self.dev_port26, self.mac_pkt)
        verify_packets(self, self.exp_mac_pkt, [self.dev_port27])

        # Used for negative test cases verification
        print("Sending EthType(0x4321) packet - port 24 -> "
              "port 25 [access vlan=100])")
        print("Sending EthType(0x4321) packet - port 26 -> "
              "port 27 [access vlan=200])")
        send_packet(self, self.dev_port24, self.mac_pkt2)
        verify_packets(self, self.exp_mac_pkt2, [self.dev_port25])
        send_packet(self, self.dev_port26, self.mac_pkt2)
        verify_packets(self, self.exp_mac_pkt2, [self.dev_port27])

    def arpAclTest(self):
        """Tests ACL dropping ARP packets"""
        # Create ACL Table, Group and Rules
        stage = SAI_ACL_STAGE_INGRESS
        entry_priority = 1000
        action = SAI_PACKET_ACTION_DROP
        eth_type = 0x0806
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=action))

        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_VLAN]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        vlan_acl_table_group_id = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            type=group_type)
        self.acl_grps.append(vlan_acl_table_group_id)
        print("VLAN ACL Table Group created 0x%lx" % (
            vlan_acl_table_group_id))

        aclv4_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_ether_type=True)
        self.acl_tables.append(aclv4_table_id)
        print("IPv4 ACL Table created 0x%lx" % (aclv4_table_id))

        aclv4_table_group_member_id = \
            sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=vlan_acl_table_group_id,
                acl_table_id=aclv4_table_id,
                priority=100)
        self.acl_grp_members.append(aclv4_table_group_member_id)

        # Add ARP drop ACL entry to IPv4 ACL Table
        arp_acl_counter = sai_thrift_create_acl_counter(
            client=self.client, table_id=aclv4_table_id)
        action_counter_t = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                oid=arp_acl_counter),
            enable=True)
        self.acl_counters.append(arp_acl_counter)

        ether_type = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u16=eth_type),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        arp_drop_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=aclv4_table_id,
            priority=entry_priority,
            action_packet_action=packet_action,
            field_ether_type=ether_type,
            action_counter=action_counter_t)

        sai_thrift_set_acl_entry_attribute(
            self.client, arp_drop_entry,
            action_counter=action_counter_t)

        self.acl_rules.append(arp_drop_entry)
        print("ARP ACL DROP entry created 0x%lx" % (arp_drop_entry))

        print("Binding ACL grp 0x%lx to Vlan100" % (vlan_acl_table_group_id))
        # bind ACL GRP to VLAN 100
        sai_thrift_set_vlan_attribute(self.client,
                                      vlan_oid=self.vlans[0],
                                      ingress_acl=vlan_acl_table_group_id)
        self.bp_list.append(self.vlans[0])

        # Initialize Counters
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=arp_acl_counter,
            packets=True)
        self.exp_counters["arp"] = counter["packets"]

        print("Sending Packets matching ACL rules on VLAN100. "
              "Packets should be dropped")
        print("Sending ARP packet - port 24 -> port 25 [access vlan=100])")
        send_packet(self, self.dev_port24, self.arp_pkt)
        verify_no_other_packets(self, timeout=1)
        self.exp_counters["arp"] += 1

        # Verifying ACL counters
        print("Verifying ACL counters")
        time.sleep(10)
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=arp_acl_counter,
            packets=True)
        self.assertTrue(self.exp_counters["arp"] == counter["packets"],
                        "ARP ACL DROP Counter Mismatch")

        print("Sending Packets matching ACL rules on VLAN200. "
              "Packets should not be dropped")
        print("Sending ARP packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port26, self.arp_pkt)
        verify_packets(self, self.exp_arp_pkt, [self.dev_port27])

        self.trapAclCleanup()

        print("Verifying ACL rules were correctly removed")
        print("Sending Packets matching ACL rules on VLAN100. "
              "Packets should not be dropped")
        print("Sending ARP packet - port 24 -> port 25 [access vlan=100])")
        send_packet(self, self.dev_port24, self.arp_pkt)
        verify_packets(self, self.exp_arp_pkt, [self.dev_port25])

        print("Sending Packets matching ACL rules on VLAN200. "
              "Packets should not be dropped")
        print("Sending ARP packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port26, self.arp_pkt)
        verify_packets(self, self.exp_arp_pkt, [self.dev_port27])

    def ipxAclTest(self):
        """Tests ACL dropping IPX packets"""
        # Create ACL Table, Group and Rules
        stage = SAI_ACL_STAGE_INGRESS
        entry_priority = 1000
        action = SAI_PACKET_ACTION_DROP
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=action))

        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_VLAN]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        vlan_acl_table_group_id = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            type=group_type)
        self.acl_grps.append(vlan_acl_table_group_id)
        print("VLAN ACL Table Group created 0x%lx" % (
            vlan_acl_table_group_id))

        aclv4_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_ether_type=True)
        self.acl_tables.append(aclv4_table_id)
        print("IPv4 ACL Table created 0x%lx" % (aclv4_table_id))

        aclv4_table_group_member_id = \
            sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=vlan_acl_table_group_id,
                acl_table_id=aclv4_table_id,
                priority=100)
        self.acl_grp_members.append(aclv4_table_group_member_id)

        if entry_priority >= 1:
            entry_priority -= 1

        # Add IPX drop ACL entry to IPv4 ACL Table
        ipx_acl_counter = sai_thrift_create_acl_counter(
            client=self.client, table_id=aclv4_table_id)
        action_counter_t = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                oid=ipx_acl_counter),
            enable=True)
        self.acl_counters.append(ipx_acl_counter)

        eth_type = 0x8137
        if eth_type > 0x7fff:
            eth_type -= 0x10000

        ether_type = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u16=eth_type),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        ipx_drop_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=aclv4_table_id,
            priority=entry_priority,
            action_packet_action=packet_action,
            field_ether_type=ether_type,
            action_counter=action_counter_t)

        sai_thrift_set_acl_entry_attribute(
            self.client, ipx_drop_entry,
            action_counter=action_counter_t)

        self.acl_rules.append(ipx_drop_entry)
        print("IPX ACL DROP entry created 0x%lx" % (ipx_drop_entry))

        print("Binding ACL grp 0x%lx to Vlan100" % (vlan_acl_table_group_id))
        # bind ACL GRP to VLAN 100
        sai_thrift_set_vlan_attribute(self.client,
                                      vlan_oid=self.vlans[0],
                                      ingress_acl=vlan_acl_table_group_id)
        self.bp_list.append(self.vlans[0])

        # Initialize Counters
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=ipx_acl_counter,
            packets=True)
        self.exp_counters["ipx"] = counter["packets"]

        print("Sending Packets matching ACL rules on VLAN100. "
              "Packets should be dropped")
        print("Sending IPX packet - port 24 -> port 25 [access vlan=100])")
        send_packet(self, self.dev_port24, self.ipx_pkt)
        verify_no_other_packets(self, timeout=1)
        self.exp_counters["ipx"] += 1

        # Verifying ACL counters
        print("Verifying ACL counters")
        time.sleep(10)
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=ipx_acl_counter,
            packets=True)
        self.assertTrue(self.exp_counters["ipx"] == counter["packets"],
                        "IPX ACL DROP Counter Mismatch")

        print("Sending Packets matching ACL rules on VLAN200. "
              "Packets should not be dropped")
        print("Sending IPX packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port26, self.ipx_pkt)
        verify_packets(self, self.exp_ipx_pkt, [self.dev_port27])

        self.trapAclCleanup()

        print("Verifying ACL rules were correctly removed")
        print("Sending Packets matching ACL rules on VLAN100. "
              "Packets should not be dropped")
        print("Sending IPX packet - port 24 -> port 25 [access vlan=100])")
        send_packet(self, self.dev_port24, self.ipx_pkt)
        verify_packets(self, self.exp_ipx_pkt, [self.dev_port25])

        print("Sending Packets matching ACL rules on VLAN200. "
              "Packets should not be dropped")
        print("Sending IPX packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port26, self.ipx_pkt)
        verify_packets(self, self.exp_ipx_pkt, [self.dev_port27])

    def ipv4AclTest(self):
        """Tests ACL dropping all packets with IPv4 address"""
        # Create ACL Table, Group and Rules
        stage = SAI_ACL_STAGE_INGRESS
        entry_priority = 1000
        action = SAI_PACKET_ACTION_DROP
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=action))

        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_VLAN]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        vlan_acl_table_group_id = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            type=group_type)
        self.acl_grps.append(vlan_acl_table_group_id)
        print("VLAN ACL Table Group created 0x%lx" % (
            vlan_acl_table_group_id))

        aclv4_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_ether_type=True)
        self.acl_tables.append(aclv4_table_id)
        print("IPv4 ACL Table created 0x%lx" % (aclv4_table_id))

        aclv4_table_group_member_id = \
            sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=vlan_acl_table_group_id,
                acl_table_id=aclv4_table_id,
                priority=100)
        self.acl_grp_members.append(aclv4_table_group_member_id)

        if entry_priority >= 1:
            entry_priority -= 1

        ipv4_acl_counter = sai_thrift_create_acl_counter(
            client=self.client, table_id=aclv4_table_id)
        action_counter_t = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                oid=ipv4_acl_counter),
            enable=True)
        self.acl_counters.append(ipv4_acl_counter)

        eth_type = 0x0800
        if eth_type > 0x7fff:
            eth_type -= 0x10000

        ether_type = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u16=eth_type),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        ipv4_drop_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=aclv4_table_id,
            priority=entry_priority,
            action_packet_action=packet_action,
            field_ether_type=ether_type,
            action_counter=action_counter_t)

        sai_thrift_set_acl_entry_attribute(
            self.client, ipv4_drop_entry,
            action_counter=action_counter_t)

        self.acl_rules.append(ipv4_drop_entry)
        print("IPv4 ACL DROP (catch all) entry created 0x%lx" % (
            ipv4_drop_entry))

        print("Binding ACL grp 0x%lx to Vlan100" % (vlan_acl_table_group_id))
        # bind ACL GRP to VLAN 100
        sai_thrift_set_vlan_attribute(self.client,
                                      vlan_oid=self.vlans[0],
                                      ingress_acl=vlan_acl_table_group_id)
        self.bp_list.append(self.vlans[0])

        # Initialize Counters
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=ipv4_acl_counter,
            packets=True)
        self.exp_counters["ipv4"] = counter["packets"]

        print("Sending Packets matching ACL rules on VLAN100. "
              "Packets should be dropped")
        print("Sending IPv4 packet - port 24 -> port 25 [access vlan=100])")
        send_packet(self, self.dev_port24, self.ipv4_pkt)
        verify_no_other_packets(self, timeout=1)
        self.exp_counters["ipv4"] += 1

        # Verifying ACL counters
        print("Verifying ACL counters")
        time.sleep(10)
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=ipv4_acl_counter,
            packets=True)
        self.assertTrue(self.exp_counters["ipv4"] == counter["packets"],
                        "IPv4 ACL DROP Counter Mismatch")

        print("Sending Packets matching ACL rules on VLAN200. "
              "Packets should not be dropped")
        print("Sending IPv4 packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port26, self.ipv4_pkt)
        verify_packets(self, self.exp_ipv4_pkt, [self.dev_port27])

        self.trapAclCleanup()

        print("Verifying ACL rules were correctly removed")
        print("Sending Packets matching ACL rules on VLAN100. "
              "Packets should not be dropped")
        print("Sending IPv4 packet - port 24 -> port 25 [access vlan=100])")
        send_packet(self, self.dev_port24, self.ipv4_pkt)
        verify_packets(self, self.exp_ipv4_pkt, [self.dev_port25])

        print("Sending Packets matching ACL rules on VLAN200. "
              "Packets should not be dropped")
        print("Sending IPv4 packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port26, self.ipv4_pkt)
        verify_packets(self, self.exp_ipv4_pkt, [self.dev_port27])

    def lldpAclTest(self):
        """Tests ACL dropping LLDP packets"""
        # Create ACL Table, Group and Rules
        stage = SAI_ACL_STAGE_INGRESS
        entry_priority = 1000
        action = SAI_PACKET_ACTION_DROP
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=action))

        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_VLAN]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        vlan_acl_table_group_id = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            type=group_type)
        self.acl_grps.append(vlan_acl_table_group_id)
        print("VLAN ACL Table Group created 0x%lx" % (
            vlan_acl_table_group_id))

        aclv6_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_ether_type=True,)
        self.acl_tables.append(aclv6_table_id)
        print("IPv6 ACL Table created 0x%lx" % (aclv6_table_id))

        aclv6_table_group_member_id = \
            sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=vlan_acl_table_group_id,
                acl_table_id=aclv6_table_id,
                priority=200)
        self.acl_grp_members.append(aclv6_table_group_member_id)

        if entry_priority >= 1:
            entry_priority -= 1

        # Add LLDP drop ACL entry to IPv6 ACL Table
        lldp_acl_counter = sai_thrift_create_acl_counter(
            client=self.client, table_id=aclv6_table_id)
        action_counter_t = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                oid=lldp_acl_counter),
            enable=True)
        self.acl_counters.append(lldp_acl_counter)

        eth_type = 0x88cc
        if eth_type > 0x7fff:
            eth_type -= 0x10000

        ether_type = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u16=eth_type),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        lldp_drop_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=aclv6_table_id,
            priority=entry_priority,
            action_packet_action=packet_action,
            field_ether_type=ether_type,
            action_counter=action_counter_t)

        sai_thrift_set_acl_entry_attribute(
            self.client, lldp_drop_entry,
            action_counter=action_counter_t)

        self.acl_rules.append(lldp_drop_entry)
        print("Installing user-acl-rule to drop LLDP 0x%lx" % (
            lldp_drop_entry))

        print("Binding ACL grp 0x%lx to Vlan100" % (vlan_acl_table_group_id))
        # bind ACL GRP to VLAN 100
        sai_thrift_set_vlan_attribute(self.client,
                                      vlan_oid=self.vlans[0],
                                      ingress_acl=vlan_acl_table_group_id)
        self.bp_list.append(self.vlans[0])

        # Create LLDP trap entry
        lldp_trap = sai_thrift_create_hostif_trap(
            client=self.client,
            trap_type=SAI_HOSTIF_TRAP_TYPE_LLDP,
            packet_action=SAI_PACKET_ACTION_TRAP)
        self.traps.append(lldp_trap)
        print("Installing LLDP hostif trap rule 0x%lx" % (lldp_trap))

        # Initialize Counters
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=lldp_acl_counter,
            packets=True)
        self.exp_counters["lldp"] = counter["packets"]

        print("Sending Packets matching ACL rules on VLAN100. "
              "Packets should be dropped")
        # User ACL entries will have higher priority
        # than hostif trap entries
        print("Have Hostif entry to trap LLDP, have User-acl to drop "
              "LLDP ingressing on port 1")
        print("User-acl entry must take priority over hostif entry")
        print("TX LLDP packet - port 24 -> drop")
        send_packet(self, self.dev_port24, self.lldp_pkt)
        verify_no_other_packets(self, timeout=1)
        self.exp_counters["lldp"] += 1

        # Verifying ACL counters
        print("Verifying ACL counters")
        time.sleep(10)
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=lldp_acl_counter,
            packets=True)
        self.assertTrue(self.exp_counters["lldp"] == counter["packets"],
                        "LLDP ACL DROP Counter Mismatch")

        # This packet will be trapped to the cpu
        pre_stats = sai_thrift_get_queue_stats(
            self.client, self.cpu_queue0)
        print("Sending Packets matching ACL rules on VLAN200. "
              "Packets should not be dropped")
        print("Sending LLDP packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port26, self.lldp_pkt)
        time.sleep(10)
        post_stats = sai_thrift_get_queue_stats(
            self.client, self.cpu_queue0)
        self.assertEqual(
            post_stats["SAI_QUEUE_STAT_PACKETS"],
            pre_stats["SAI_QUEUE_STAT_PACKETS"] + 1)

        self.trapAclCleanup()

        print("Verifying ACL rules were correctly removed")
        print("Sending Packets matching ACL rules on VLAN100. "
              "Packets should not be dropped")
        print("Sending LLDP packet - port 24 -> port 25 [access vlan=100])")
        send_packet(self, self.dev_port24, self.lldp_pkt)
        verify_packets(self, self.exp_lldp_pkt, [self.dev_port25])

        print("Sending Packets matching ACL rules on VLAN200. "
              "Packets should not be dropped")
        print("Sending LLDP packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port26, self.lldp_pkt)
        verify_packets(self, self.exp_lldp_pkt, [self.dev_port27])

    def ipv6AclTest(self):
        """Tests ACL dropping all packets with IPv6 address"""
        # Create ACL Table, Group and Rules
        stage = SAI_ACL_STAGE_INGRESS
        entry_priority = 1000
        action = SAI_PACKET_ACTION_DROP
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

        packet_action = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                s32=action))

        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_VLAN]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        vlan_acl_table_group_id = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            type=group_type)
        self.acl_grps.append(vlan_acl_table_group_id)
        print("VLAN ACL Table Group created 0x%lx" % (
            vlan_acl_table_group_id))

        aclv6_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_src_ipv6=True,
            field_ether_type=True)
        self.acl_tables.append(aclv6_table_id)
        print("IPv6 ACL Table created 0x%lx" % (aclv6_table_id))

        aclv6_table_group_member_id = \
            sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=vlan_acl_table_group_id,
                acl_table_id=aclv6_table_id,
                priority=200)
        self.acl_grp_members.append(aclv6_table_group_member_id)

        if entry_priority >= 1:
            entry_priority -= 1

        # Add IPv6 drop ACL entry based on SRC_IPv6 Addr to IPv6 ACL Table
        ipv6_acl_counter = sai_thrift_create_acl_counter(
            client=self.client, table_id=aclv6_table_id)
        action_counter_t = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                oid=ipv6_acl_counter),
            enable=True)
        self.acl_counters.append(ipv6_acl_counter)

        ip_src = "2000::1"
        ip_src_mask = "ffff:ffff:ffff:ffff:ffff:ffff:ffff:0"
        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip6=ip_src),
            mask=sai_thrift_acl_field_data_mask_t(ip6=ip_src_mask))

        eth_type = 0x86dd
        if eth_type > 0x7fff:
            eth_type -= 0x10000

        ether_type = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u16=eth_type),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        ipv6_drop_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=aclv6_table_id,
            priority=entry_priority,
            field_src_ipv6=src_ip_t,
            action_packet_action=packet_action,
            field_ether_type=ether_type,
            action_counter=action_counter_t)

        sai_thrift_set_acl_entry_attribute(
            self.client, ipv6_drop_entry,
            action_counter=action_counter_t)
        self.acl_rules.append(ipv6_drop_entry)
        print("IPv6 ACL DROP entry created for SRC_IP:2000::0:x 0x%lx" % (
            ipv6_drop_entry))

        print("Binding ACL grp 0x%lx to Vlan100" % (vlan_acl_table_group_id))
        # bind ACL GRP to VLAN 100
        sai_thrift_set_vlan_attribute(self.client,
                                      vlan_oid=self.vlans[0],
                                      ingress_acl=vlan_acl_table_group_id)
        self.bp_list.append(self.vlans[0])

        # Initialize Counters
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=ipv6_acl_counter,
            packets=True)
        self.exp_counters["ipv6"] = counter["packets"]

        print("Sending Packets matching ACL rules on VLAN100. "
              "Packets should be dropped")
        print("Sending TCPv6 packet - port 24 -> port 25 [access vlan=100])")
        send_packet(self, self.dev_port24, self.tcpv6_pkt)
        verify_no_other_packets(self, timeout=1)
        self.exp_counters["ipv6"] += 1

        # Verifying ACL counters
        print("Verifying ACL counters")
        time.sleep(10)
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=ipv6_acl_counter,
            packets=True)
        self.assertTrue(self.exp_counters["ipv6"] == counter["packets"],
                        "IPv6 SRC_IP ACL DROP Counter Mismatch")

        print("Sending Packets matching ACL rules on VLAN200. "
              "Packets should not be dropped")
        print("Sending TCPv6 packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port26, self.tcpv6_pkt)
        verify_packets(self, self.exp_tcpv6_pkt, [self.dev_port27])

        self.trapAclCleanup()

        print("Verifying ACL rules were correctly removed")
        print("Sending TCPv6 packet - port 24 -> port 25 [access vlan=100])")
        send_packet(self, self.dev_port24, self.tcpv6_pkt)
        verify_packets(self, self.exp_tcpv6_pkt, [self.dev_port25])

        print("Sending Packets matching ACL rules on VLAN200. "
              "Packets should not be dropped")
        print("Sending TCPv6 packet - port 26 -> port 27 [access vlan=200])")
        send_packet(self, self.dev_port26, self.tcpv6_pkt)
        verify_packets(self, self.exp_tcpv6_pkt, [self.dev_port27])

    def ipv4MirrorAclTest(self):
        """Tests ACL mirroring based on IPv4 address"""
        # Create ACL Table, Group and Rules
        stage = SAI_ACL_STAGE_INGRESS
        entry_priority = 1000
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        port_acl_table_group_id = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            type=group_type)
        self.acl_grps.append(port_acl_table_group_id)
        print("VLAN ACL Table Group created 0x%lx" % (
            port_acl_table_group_id))

        if entry_priority >= 1:
            entry_priority -= 1

        # Create Mirror ACL table
        ip_src = "172.0.0.17"
        ip_src_mask = "255.255.255.255"
        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip4=ip_src),
            mask=sai_thrift_acl_field_data_mask_t(ip4=ip_src_mask))

        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        field_dscp = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u8=1))

        mirror_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_dscp=True,
            field_src_ip=True)
        self.acl_tables.append(mirror_table_id)

        mirror_table_group_member_id = \
            sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=port_acl_table_group_id,
                acl_table_id=mirror_table_id,
                priority=300)
        self.acl_grp_members.append(mirror_table_group_member_id)

        # Add IPV4 SRC IP Mirror ACL entry to Mirror ACL Table
        v4src_mirror_acl_counter = sai_thrift_create_acl_counter(
            client=self.client, table_id=mirror_table_id)
        action_counter_t = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                oid=v4src_mirror_acl_counter),
            enable=True)
        self.acl_counters.append(v4src_mirror_acl_counter)

        v4src_mirror_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=mirror_table_id,
            priority=entry_priority,
            field_src_ip=src_ip_t,
            action_mirror_ingress=self.mirror_action,
            action_counter=action_counter_t,
            field_dscp=field_dscp)
        self.acl_rules.append(v4src_mirror_entry)
        print("v4src ACL Mirror entry created 0x%lx" % (v4src_mirror_entry))

        print("Binding ACL grp 0x%lx to Port3" % (port_acl_table_group_id))
        # bind ACL GRP to VLAN 100
        sai_thrift_set_port_attribute(
            self.client, self.port26, ingress_acl=port_acl_table_group_id)
        self.bp_list.append(self.port26)

        # Initialize Counters
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=v4src_mirror_acl_counter,
            packets=True)
        self.exp_counters["mirror_ip4"] = counter["packets"]

        print("Sending Packets matching Mirror ACL rules on Port26. "
              "Packets should be mirrored")
        print("Sending TCPv4 (SRC IP 172.0.0.17) packet - port 26 -> "
              "port 27 [access vlan=200])")
        send_packet(self, self.dev_port26, self.tcpv4_pkt)
        verify_packets(self, self.exp_tcpv4_pkt, [self.dev_port27,
                                                  self.dev_port28])
        self.exp_counters["mirror_ip4"] += 1

        # Verifying ACL counters
        print("Verifying ACL counters")
        time.sleep(10)
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=v4src_mirror_acl_counter,
            packets=True)
        self.assertTrue(self.exp_counters["mirror_ip4"] == counter["packets"],
                        "IPv4 SRC_IP ACL Mirror Counter Mismatch")

        self.trapAclCleanup()

        print("Sending Packets matching Mirror ACL rules on Port26. "
              "Packets should not be mirrored")
        print("Sending TCPv4 (SRC IP 172.0.0.17) packet - port 26 -> "
              "port 4 [access vlan=200])")
        send_packet(self, self.dev_port26, self.tcpv4_pkt)
        verify_packets(self, self.exp_tcpv4_pkt, [self.dev_port27])

    def ipv6MirrorAclTest(self):
        """Tests ACL mirroring based on IPv6 address"""
        # Create ACL Table, Group and Rules
        stage = SAI_ACL_STAGE_INGRESS
        entry_priority = 1000
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        port_acl_table_group_id = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            type=group_type)
        self.acl_grps.append(port_acl_table_group_id)
        print("VLAN ACL Table Group created 0x%lx" % (
            port_acl_table_group_id))

        # Create Mirror ACL table
        field_dscp = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u8=1))

        mirror_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_dscp=True,
            field_src_ipv6=True)
        self.acl_tables.append(mirror_table_id)

        mirror_table_group_member_id = \
            sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=port_acl_table_group_id,
                acl_table_id=mirror_table_id,
                priority=300)
        self.acl_grp_members.append(mirror_table_group_member_id)

        if entry_priority >= 1:
            entry_priority -= 1

        # Add IPV6 SRC IP Mirror ACL entry to Mirror ACL Table
        v6src_mirror_acl_counter = sai_thrift_create_acl_counter(
            client=self.client, table_id=mirror_table_id)
        action_counter_t = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                oid=v6src_mirror_acl_counter),
            enable=True)
        self.acl_counters.append(v6src_mirror_acl_counter)

        ip_src = "3000::1"
        ip_src_mask = "ffff:ffff:ffff:ffff:ffff:ffff:ffff:0"
        src_ip_t = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(ip6=ip_src),
            mask=sai_thrift_acl_field_data_mask_t(ip6=ip_src_mask))

        v6src_mirror_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=mirror_table_id,
            priority=entry_priority,
            field_src_ipv6=src_ip_t,
            action_mirror_ingress=self.mirror_action,
            action_counter=action_counter_t,
            field_dscp=field_dscp)
        self.acl_rules.append(v6src_mirror_entry)
        print("v6src ACL Mirror entry created 0x%lx" % (v6src_mirror_entry))

        print("Binding ACL grp 0x%lx to Port26" % (port_acl_table_group_id))
        # bind ACL GRP to VLAN 100
        sai_thrift_set_port_attribute(
            self.client, self.port26, ingress_acl=port_acl_table_group_id)
        self.bp_list.append(self.port26)

        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=v6src_mirror_acl_counter,
            packets=True)
        self.exp_counters["mirror_ip6"] = counter["packets"]

        print("Sending Packets matching Mirror ACL rules on Port26. "
              "Packets should be mirrored")
        print("Sending TCPv6 (SRC IP 3000::1) packet - port 26 -> port 27 "
              "[access vlan=200])")
        send_packet(self, self.dev_port26, self.tcpv6_pkt2)
        verify_packets(self, self.exp_tcpv6_pkt2, [self.dev_port27,
                                                   self.dev_port28])
        self.exp_counters["mirror_ip6"] += 1

        # Verifying ACL counters
        print("Verifying ACL counters")
        time.sleep(10)
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=v6src_mirror_acl_counter,
            packets=True)
        self.assertTrue(self.exp_counters["mirror_ip6"] == counter["packets"],
                        "IPv6 SRC_IP ACL Mirror Counter Mismatch")

        self.trapAclCleanup()

        print("Sending Packets matching Mirror ACL rules on Port26. "
              "Packets should not be mirrored")
        print("Sending TCPv6 (SRC IP 3000::1) packet - port 26 -> port 27 "
              "[access vlan=200])")
        send_packet(self, self.dev_port26, self.tcpv6_pkt2)
        verify_packets(self, self.exp_tcpv6_pkt2, [self.dev_port27])

    def udpMirrorAclTest(self):
        """Tests mirroring UDP packets"""
        # Create ACL Table, Group and Rules
        stage = SAI_ACL_STAGE_INGRESS
        entry_priority = 1000
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        port_acl_table_group_id = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            type=group_type)
        self.acl_grps.append(port_acl_table_group_id)
        print("VLAN ACL Table Group created 0x%lx" % (
            port_acl_table_group_id))

        # Create Mirror ACL table
        field_dscp = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u8=1))

        mirror_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_dscp=True)
        self.acl_tables.append(mirror_table_id)

        mirror_table_group_member_id = \
            sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=port_acl_table_group_id,
                acl_table_id=mirror_table_id,
                priority=300)
        self.acl_grp_members.append(mirror_table_group_member_id)

        if entry_priority >= 1:
            entry_priority -= 1

        # Add UDP (IPv4/IPv6 common) ACL entry to Mirror ACL Table
        udp_mirror_acl_counter = sai_thrift_create_acl_counter(
            client=self.client, table_id=mirror_table_id)
        action_counter_t = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                oid=udp_mirror_acl_counter),
            enable=True)
        self.acl_counters.append(udp_mirror_acl_counter)

        ip_protocol = sai_thrift_acl_field_data_t(
            enable=True,
            data=sai_thrift_acl_field_data_data_t(u8=17),
            mask=sai_thrift_acl_field_data_mask_t(u8=127))

        udp_mirror_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=mirror_table_id,
            priority=entry_priority,
            field_ip_protocol=ip_protocol,
            action_mirror_ingress=self.mirror_action,
            action_counter=action_counter_t,
            field_dscp=field_dscp)
        self.acl_rules.append(udp_mirror_entry)
        print("TCP ACL Mirror entry created 0x%lx" % (udp_mirror_entry))

        print("Binding ACL grp 0x%lx to Port26" % (port_acl_table_group_id))
        # bind ACL GRP to VLAN 100
        sai_thrift_set_port_attribute(
            self.client, self.port26, ingress_acl=port_acl_table_group_id)
        self.bp_list.append(self.port26)

        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=udp_mirror_acl_counter,
            packets=True)
        self.exp_counters["mirror_udp"] = counter["packets"]

        print("Sending Packets matching Mirror ACL rules on Port3. "
              "Packets should be mirrored")
        print("Sending UDPv6  packet - port 26 -> port 27 "
              "[access vlan=200])")
        send_packet(self, self.dev_port26, self.udpv6_pkt)
        verify_packets(self, self.exp_udpv6_pkt, [self.dev_port27,
                                                  self.dev_port28])
        self.exp_counters["mirror_udp"] += 1

        print("Sending UDPv4  packet - port 26 -> port 27 "
              "[access vlan=200])")
        send_packet(self, self.dev_port26, self.udpv4_pkt)
        verify_packets(self, self.exp_udpv4_pkt, [self.dev_port27,
                                                  self.dev_port28])
        self.exp_counters["mirror_udp"] += 1

        # Verifying ACL counters
        print("Verifying ACL counters")
        time.sleep(10)
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=udp_mirror_acl_counter,
            packets=True)
        self.assertTrue(self.exp_counters["mirror_udp"] == counter["packets"],
                        "UDP ACL Mirror Counter Mismatch")

        self.trapAclCleanup()

        print("Sending Packets matching Mirror ACL rules on Port26. "
              "Packets should not be mirrored")
        print("Sending UDPv6  packet - port 26 -> port 27 "
              "[access vlan=200])")
        send_packet(self, self.dev_port26, self.udpv6_pkt)
        verify_packets(self, self.exp_udpv6_pkt, [self.dev_port27])

        print("Sending UDPv4  packet - port 26 -> port 27 "
              "[access vlan=200])")
        send_packet(self, self.dev_port26, self.udpv4_pkt)
        verify_packets(self, self.exp_udpv4_pkt, [self.dev_port27])

    def macMirrorAclTest(self):
        """Tests mirroring MAC packets"""
        # Create ACL Table, Group and Rules
        stage = SAI_ACL_STAGE_INGRESS
        entry_priority = 1000
        group_type = SAI_ACL_TABLE_GROUP_TYPE_PARALLEL

        table_bind_point_list = [SAI_ACL_BIND_POINT_TYPE_PORT]
        table_bind_point_type_list = sai_thrift_s32_list_t(
            count=len(table_bind_point_list),
            int32list=table_bind_point_list)

        port_acl_table_group_id = sai_thrift_create_acl_table_group(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            type=group_type)
        self.acl_grps.append(port_acl_table_group_id)
        print("VLAN ACL Table Group created 0x%lx" % (
            port_acl_table_group_id))

        # Create Mirror ACL table
        field_dscp = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u8=1))

        mirror_table_id = sai_thrift_create_acl_table(
            self.client,
            acl_stage=stage,
            acl_bind_point_type_list=table_bind_point_type_list,
            field_ether_type=True,
            field_dscp=True)
        self.acl_tables.append(mirror_table_id)

        mirror_table_group_member_id = \
            sai_thrift_create_acl_table_group_member(
                self.client,
                acl_table_group_id=port_acl_table_group_id,
                acl_table_id=mirror_table_id,
                priority=300)
        self.acl_grp_members.append(mirror_table_group_member_id)

        if entry_priority >= 1:
            entry_priority -= 1

        # Add Eth type MAC ACL entry to Mirror ACL Table
        mac_mirror_acl_counter = sai_thrift_create_acl_counter(
            client=self.client, table_id=mirror_table_id)
        action_counter_t = sai_thrift_acl_action_data_t(
            parameter=sai_thrift_acl_action_parameter_t(
                oid=mac_mirror_acl_counter),
            enable=True)
        self.acl_counters.append(mac_mirror_acl_counter)

        eth_type = 0x1234
        if eth_type > 0x7fff:
            eth_type -= 0x10000

        ether_type = sai_thrift_acl_field_data_t(
            data=sai_thrift_acl_field_data_data_t(u16=eth_type),
            mask=sai_thrift_acl_field_data_mask_t(u16=32767))

        mac_mirror_entry = sai_thrift_create_acl_entry(
            self.client,
            table_id=mirror_table_id,
            priority=entry_priority,
            action_mirror_ingress=self.mirror_action,
            action_counter=action_counter_t,
            field_ether_type=ether_type,
            field_dscp=field_dscp)
        self.acl_rules.append(mac_mirror_entry)
        print("MAC ACL Mirror entry created 0x%lx" % (mac_mirror_entry))

        print("Binding ACL grp 0x%lx to Port26" % (port_acl_table_group_id))
        # bind ACL GRP to VLAN 100
        sai_thrift_set_port_attribute(
            self.client, self.port26, ingress_acl=port_acl_table_group_id)
        self.bp_list.append(self.port26)

        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=mac_mirror_acl_counter,
            packets=True)
        self.exp_counters["mirror_mac"] = counter["packets"]

        print("Sending Packets matching Mirror ACL rules on Port26. "
              "Packets should be mirrored")
        print("Sending EthType(0x1234) packet - port 26 -> port 27 "
              "[access vlan=200])")
        send_packet(self, self.dev_port26, self.mac_pkt)
        verify_packets(self, self.exp_mac_pkt, [self.dev_port27,
                                                self.dev_port28])
        self.exp_counters["mirror_mac"] += 1
        print("Sending EthType(0x4321) packet - port 26 -> port 27 "
              "[access vlan=200])")
        send_packet(self, self.dev_port26, self.mac_pkt2)
        verify_packets(self, self.exp_mac_pkt2, [self.dev_port27])

        print("Sending EthType(0x4321) packet - port 24 -> "
              "port 25 [access vlan=100])")
        send_packet(self, self.dev_port24, self.mac_pkt2)
        verify_packets(self, self.exp_mac_pkt2, [self.dev_port25])

        # Verifying ACL counters
        print("Verifying ACL counters")
        time.sleep(10)
        counter = sai_thrift_get_acl_counter_attribute(
            client=self.client,
            acl_counter_oid=mac_mirror_acl_counter,
            packets=True)
        self.assertTrue(self.exp_counters["mirror_mac"] == counter["packets"],
                        "MAC ACL Mirror Counter Mismatch")

        self.trapAclCleanup()

        print("Sending Packets matching Mirror ACL rules on Port26. "
              "Packets should not be mirrored")
        print("Sending EthType(0x1234) packet - port 26 -> port 27 "
              "[access vlan=200])")
        send_packet(self, self.dev_port26, self.mac_pkt)
        verify_packets(self, self.exp_mac_pkt, [self.dev_port27])

        print("Sending EthType(0x4321) packet - port 24 -> "
              "port 25 [access vlan=100])")
        send_packet(self, self.dev_port24, self.mac_pkt2)
        verify_packets(self, self.exp_mac_pkt2, [self.dev_port25])

    def trapAclCleanup(self):
        """ Cleans up ACL"""
        print("Cleans up")
        for bp in self.bp_list:
            if bp in self.vlans:
                print("Unbinding ACL grp from Vlan")
                sai_thrift_set_vlan_attribute(
                    self.client, vlan_oid=bp, ingress_acl=0)
            else:
                sai_thrift_set_port_attribute(
                    self.client, bp, ingress_acl=0)
                print("Unbinding ACL grp from Port")
            self.bp_list.remove(bp)
        for trap in self.traps:
            sai_thrift_remove_hostif_trap(self.client, trap)
            self.traps.remove(trap)
        for acl_rule in self.acl_rules:
            action_counter_t = sai_thrift_acl_action_data_t(
                parameter=sai_thrift_acl_action_parameter_t(
                    oid=0),
                enable=True)
            sai_thrift_set_acl_entry_attribute(
                self.client, acl_rule,
                action_counter=action_counter_t)
        for acl_counter in self.acl_counters:
            sai_thrift_remove_acl_counter(self.client, acl_counter)
            self.acl_counters.remove(acl_counter)
        for acl_grp_member in self.acl_grp_members:
            sai_thrift_remove_acl_table_group_member(self.client,
                                                     acl_grp_member)
            self.acl_grp_members.remove(acl_grp_member)
        for acl_rule in self.acl_rules:
            sai_thrift_remove_acl_entry(self.client, acl_rule)
            self.acl_rules.remove(acl_rule)
        for acl_table in self.acl_tables:
            sai_thrift_remove_acl_table(self.client, acl_table)
            self.acl_tables.remove(acl_table)
        for acl_grp in self.acl_grps:
            sai_thrift_remove_acl_table_group(self.client, acl_grp)
            self.acl_grps.remove(acl_grp)
