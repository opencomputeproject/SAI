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

from sai_thrift.sai_adapter import *
from sai_utils import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]
from typing import TYPE_CHECKING
from data_module.vlan import Vlan

if TYPE_CHECKING:
    from sai_test_base import T0TestBase


def t0_vlan_config_helper(test_obj: 'T0TestBase', is_reset_default_vlan=False, is_create_vlan=True):
    """
    Make t0 Vlan configurations base on the configuration in the test plan.
    Set the configuration in test directly.

    Set the following test_obj attributes:
        int: default_vlan_id
        dict: vlans - vid_id: vlan_object

    """
    configer = VlanConfiger(test_obj)
    vlans = {}

    # Todo add port to vlan member map and vise versa
    # Todo maintain the two map (port <-> vlan member) when add or remove
    default_vlan_id = configer.get_default_vlan()

    if is_reset_default_vlan:
        members = configer.get_vlan_member(default_vlan_id)
        configer.remove_vlan_members(members)

    if is_create_vlan:
        vlan = configer.create_vlan(10, [1, 2, 3, 4, 5, 6, 7, 8])
        vlans.update({vlan.vlan_id: vlan})
        vlan = configer.create_vlan(20, [9, 10, 11, 12, 13, 14, 15, 16])
        vlans.update({vlan.vlan_id: vlan})
    # todo check and get vlan when skip create vlan

    if not hasattr(test_obj, 'vlans'):
        test_obj.dut.vlans = {}
    for key in vlans:
        test_obj.dut.vlans.update({key: vlans[key]})
    test_obj.dut.default_vlan_id = default_vlan_id


def remove_default_vlan(test_obj: 'T0TestBase'):
    """
    Remove default Vlan
    test_obj: test object from a test class
    """
    configer = VlanConfiger(test_obj)
    default_vlan_id = configer.get_default_vlan()
    members = configer.get_vlan_member(default_vlan_id)
    configer.remove_vlan_members(members)

def t0_vlan_tear_down_helper(test_obj: 'T0TestBase'):
    '''
    Args:
        test_obj: test object
    remove vlan
    '''
    configer = VlanConfiger(test_obj)
    # remove default vlan
    default_vlan_id = configer.get_default_vlan()
    members = configer.get_vlan_member(default_vlan_id)
    configer.remove_vlan_members(members)
   # configer.remove_vlan(default_vlan_id)

    for _, vlan in test_obj.dut.vlans.items():
        members = configer.get_vlan_member(vlan.oid)
        configer.remove_vlan_members(members)
        configer.remove_vlan(vlan.oid)
    test_obj.dut.vlans.clear()


class VlanConfiger(object):
    """
    Class use to make all the vlan configurations.
    """

    def __init__(self, test_obj: 'T0TestBase') -> None:
        """
        Init the Vlan configer.

        Args:
            test_obj: the test object
        """

        self.test_obj = test_obj
        self.client = test_obj.client

    def create_vlan(self, vlan_id, vlan_port_idxs, vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED):
        """
        Create vlan and its members.

        Args:
            vlan_id: vlan id
            vlan_port_idxs: vlan member ports index
            vlan_tagging_mode: SAI_VLAN_MEMBER_ATTR_VLAN_TAGGING_MODE

        Returns:
            Vlan: vlan object
        """

        vlan = Vlan(None, None, [], None, [], [])
        print("Create vlan {} and it memmber port at {} ...".format(
            vlan_id, vlan_port_idxs))
        vlan_oid = sai_thrift_create_vlan(self.client, vlan_id=vlan_id)
        members = self.create_vlan_member(
            vlan_oid, vlan_port_idxs, vlan_tagging_mode)
        vlan.vlan_id = vlan_id
        vlan.vlan_mport_oids = members
        vlan.oid = vlan_oid
        vlan.port_idx_list = vlan_port_idxs
        return vlan

    def create_vlan_member(self, vlan_oid, vlan_ports, vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED):
        """
        Create vlan members for a vlan.

        Args:
            vlan_oid:   vlan oid
            vlan_ports: vlan member ports index 
            vlan_tagging_mode: SAI_VLAN_MEMBER_ATTR_VLAN_TAGGING_MODE

        Returns:
            list: vlan members oid
        """
        vlan_members = []
        attr = sai_thrift_get_vlan_attribute(
            self.client, vlan_oid, vlan_id=True)
        vlan_id = attr['vlan_id']
        for port_index in vlan_ports:
            vlan_member = sai_thrift_create_vlan_member(self.client,
                                                        vlan_id=vlan_oid,
                                                        bridge_port_id=self.test_obj.dut.port_obj_list[port_index].bridge_port_oid,
                                                        vlan_tagging_mode=vlan_tagging_mode)
            vlan_members.append(vlan_member)
            sai_thrift_set_port_attribute(
                self.client, self.test_obj.dut.port_obj_list[port_index].oid, port_vlan_id=vlan_id)
        return vlan_members

    def get_default_vlan(self):
        """
        Get defaule vlan.

        Returns:
            default_vlan_id
        """
        print("Get default vlan...")
        def_attr = sai_thrift_get_switch_attribute(self.client, default_vlan_id=True)
        return def_attr['default_vlan_id']

    def get_vlan_member(self, vlan_id):
        """
        Get vlan member by vlan_id.

        Args:
            vlan_id: vlan id

        Returns:
            list: vlan member oid list
        """
        vlan_member_size = self.test_obj.dut.active_ports_no + self.test_obj.dut.system_port_no
        vlan_member_list = sai_thrift_object_list_t(count=vlan_member_size)
        mbr_list = sai_thrift_get_vlan_attribute(self.client, vlan_id, member_list=vlan_member_list)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
        return mbr_list['SAI_VLAN_ATTR_MEMBER_LIST'].idlist

    def remove_vlan(self, vlan_oid):
        """
        Remove vlan and its members.

        Args:
            vlan_ports: vlan member ports index

        Returns:
            dict: vlan_list[vlan_id][vlan_members]
        """
        print("Remove vlan {} and its members ...".format(vlan_oid))
        sai_thrift_remove_vlan(self.client, vlan_oid)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

    def remove_vlan_members(self, vlan_members):
        """
        Remove vlan members.
        Args:
            vlan_members: vlan member oids
        """

        for vlan_member in vlan_members:
            sai_thrift_remove_vlan_member(self.client, vlan_member)
            self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
