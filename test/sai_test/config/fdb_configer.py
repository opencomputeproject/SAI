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


def t0_fdb_config_helper(test_obj, is_create_fdb=True):
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
        test_obj.default_vlan_fdb_list = configer.create_fdb_entries(
            switch_id=test_obj.switch_id,
            mac_list=test_obj.local_server_mac_list[0:1],
            port_oids=test_obj.bridge_port_list[0:1],
            vlan_oid=test_obj.default_vlan_id)
        test_obj.vlan_10_fdb_list = configer.create_fdb_entries(
            switch_id=test_obj.switch_id,
            mac_list=test_obj.local_server_mac_list[1:9],
            port_oids=test_obj.bridge_port_list[1:9],
            vlan_oid=test_obj.vlans[10].vlan_oid)
        test_obj.vlan_20_fdb_list = configer.create_fdb_entries(
            switch_id=test_obj.switch_id,
            mac_list=test_obj.local_server_mac_list[9:17],
            port_oids=test_obj.bridge_port_list[9:17],
            vlan_oid=test_obj.vlans[20].vlan_oid)
    # Todo dynamic use the vlan_member_port_map to add data to fdb
    

def t0_fdb_tear_down_helper(test_obj):
    '''
    Args:
        test_obj: test object
    '''
    for e in test_obj.default_vlan_fdb_list:
        sai_thrift_remove_fdb_entry(test_obj.client, e)
    for e in test_obj.vlan_10_fdb_list:
        sai_thrift_remove_fdb_entry(test_obj.client, e)
    for e in test_obj.vlan_20_fdb_list:
        sai_thrift_remove_fdb_entry(test_obj.client, e)

class FdbConfiger(object):
    """
    Class use to make all the fdb configurations.
    """

    def __init__(self, test_obj) -> None:
        """
        Init the Port configer.

        Args:
            test_obj: the test object
        """
        self.test_obj = test_obj
        self.client = test_obj.client

    def create_fdb_entries(self,
                           switch_id,
                           mac_list,
                           port_oids,
                           type=SAI_FDB_ENTRY_TYPE_STATIC,
                           vlan_oid=None,
                           packet_action=SAI_PACKET_ACTION_FORWARD,
                           allow_mac_move=True,
                           wait_sec=2):
        """
        Create FDB entries.

        Args:
            switch_id: switch id
            mac_list: mac list
            port_oids: port oids
            type: SAI_FDB_ENTRY_ATTR_TYPE
            vlan_oid: vlan id for the mac
            packet_action:SAI_FDB_ENTRY_ATTR_PACKET_ACTION

        """
        print("Add FDBs ...")
        fdb_list = []
        for index, mac in enumerate(mac_list):
            fdb_entry = sai_thrift_fdb_entry_t(
                switch_id=switch_id,
                mac_address=mac,
                bv_id=vlan_oid)
            sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry,
                type=type,
                bridge_port_id=port_oids[index],
                packet_action=packet_action,
                allow_mac_move=allow_mac_move)
            fdb_list.append(fdb_entry)
        print("Waiting for FDB to get refreshed, {} seconds ...".format(
            wait_sec))
        time.sleep(wait_sec)
        return fdb_list
