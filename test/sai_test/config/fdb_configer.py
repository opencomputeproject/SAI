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

from sai_utils import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]
from sai_thrift.sai_adapter import *
from data_module.device import Device
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from sai_test_base import T0TestBase


def t0_fdb_config_helper(test_obj: 'T0TestBase', is_create_fdb=True):
    """
    Make t0 FDB configurations base on the configuration in the test plan.
    Set the configuration in test directly.

    Set the following test_obj attributes:
        list: default_vlan_fdb_list
        list: vlan_10_fdb_list
        list: vlan_20_fdb_list

    """
    configer = FdbConfiger(test_obj)

    if is_create_fdb:
        configer.create_fdb_entries(
            switch_id=test_obj.dut.switch_id,
            server_list=test_obj.servers[0][0:1],
            port_idxs=range(0, 1),
            vlan_oid=test_obj.dut.default_vlan_id)
        configer.create_fdb_entries(
            switch_id=test_obj.dut.switch_id,
            server_list=test_obj.servers[1][1:9],
            port_idxs=range(1, 9),
            vlan_oid=test_obj.dut.vlans[10].oid)
        configer.create_fdb_entries(
            switch_id=test_obj.dut.switch_id,
            server_list=test_obj.servers[2][1:9],
            port_idxs=range(9, 17),
            vlan_oid=test_obj.dut.vlans[20].oid)
    # Todo dynamic use the vlan_member_port_map to add data to fdb


def t0_fdb_tear_down_helper(test_obj: 'T0TestBase'):
    '''
    Args:
        test_obj: test object
    '''
    for item in test_obj.dut.fdb_entry_list:
        sai_thrift_remove_fdb_entry(test_obj.client, item)
        test_obj.dut.fdb_entry_list.remove(item)


class FdbConfiger(object):
    """
    Class use to make all the fdb configurations.
    """

    def __init__(self, test_obj: 'T0TestBase') -> None:
        """
        Init the Port configer.

        Args:
            test_obj: the test object
        """
        self.test_obj = test_obj
        self.client = test_obj.client

    def create_fdb_entries(self,
                           switch_id,
                           server_list,
                           port_idxs,
                           type=SAI_FDB_ENTRY_TYPE_STATIC,
                           vlan_oid=None,
                           packet_action=SAI_PACKET_ACTION_FORWARD,
                           allow_mac_move=True,
                           wait_sec=2):
        """
        Create FDB entries.

        Args:
            switch_id: switch id
            server_list: server list
            port_idxs: local port indexes
            type: SAI_FDB_ENTRY_ATTR_TYPE
            vlan_oid: vlan id for the mac
            packet_action:SAI_FDB_ENTRY_ATTR_PACKET_ACTION

        """
        print("Add FDBs ...")
        fdb_list = []
        for index, server in enumerate(server_list):
            srv: Device = server
            fdb_entry = sai_thrift_fdb_entry_t(
                switch_id=switch_id,
                mac_address=srv.mac,
                bv_id=vlan_oid)
            port_index = port_idxs[index]
            status = sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry,
                type=type,
                bridge_port_id=self.test_obj.dut.port_obj_list[port_index].bridge_port_oid,
                packet_action=packet_action,
                allow_mac_move=allow_mac_move)
            self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)
            fdb_list.append(fdb_entry)
            srv.l2_egress_port_idx = port_index
            self.test_obj.dut.fdb_entry_list.append(fdb_entry)
        print("Waiting for FDB to get refreshed, {} seconds ...".format(
            wait_sec))
        time.sleep(wait_sec)
        return fdb_list
