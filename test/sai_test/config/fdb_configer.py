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


def t0_fdb_config_helper(test_obj):
    """
    Make t0 FDB configurations base on the configuration in the test plan.
    Set the configuration in test directly.

    Set the following test_obj attributes:
        list: local_server_mac_list

    """
    configer = FdbConfiger(test_obj)
    local_server_mac_list = []
    mac_list_temp = []

    mac_list_temp = configer.generate_mac_address_list(FDB_SERVER_NUM, 0, range(0, 1))
    local_server_mac_list.extend(mac_list_temp)
    mac_list_temp = configer.generate_mac_address_list(FDB_SERVER_NUM, 1, range(1,9))
    local_server_mac_list.extend(mac_list_temp)
    mac_list_temp = configer.generate_mac_address_list(FDB_SERVER_NUM, 2, range(9,17))
    local_server_mac_list.extend(mac_list_temp)
    configer.create_fdb_entries(
        switch_id=test_obj.switch_id, 
        mac_list=local_server_mac_list,
        port_oids=test_obj.bridge_port_list[0:len(local_server_mac_list)], 
        vlan_oid=test_obj.default_vlan_id)
    test_obj.local_server_mac_list = local_server_mac_list

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
                           packet_action=SAI_PACKET_ACTION_FORWARD):
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
                packet_action=packet_action)

    def generate_mac_address_list(self, role, group, indexes):
        """
        Generate mac addresses.

        Args:
            role: Role which is represented by the mac address(base on test plan config)
            group: group number for the mac address(base on test plan config)
            indexes: mac indexes

        Returns:
            default_1q_bridge_id
        """
        print("Generate MAC ...")
        mac_list = []
        for index in indexes:
            mac = FDB_MAC_PREFIX + ':' + role + ':' + \
                '{:02d}'.format(group) + ':' + '{:02d}'.format(index)
            mac_list.append(mac)
        return mac_list
