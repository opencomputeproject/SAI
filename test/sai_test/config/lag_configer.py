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
from data_module.lag import Lag

if TYPE_CHECKING:
    from sai_test_base import T0TestBase


def t0_lag_config_helper(test_obj: 'T0TestBase', is_create_lag=True):
    """
    Make lag configurations base on the configuration in the test plan.
    set the configuration in test directly.

    set the following test_obj attributes:
        lag object

    """
    lag_configer = LagConfiger(test_obj)

    if is_create_lag:
        test_obj.dut.lag_list.append(lag_configer.create_lag([17, 18]))
        test_obj.dut.lag_list.append(lag_configer.create_lag([19, 20]))
        test_obj.dut.lag_list.append(lag_configer.create_lag([21, 22]))
        test_obj.dut.lag_list.append(lag_configer.create_lag([23, 24]))

    """
    lag_configer.set_lag_hash_algorithm()
    lag_configer.setup_lag_v4_hash()
    lag_configer.set_lag_hash_seed()
    """


class LagConfiger(object):
    """
    Class use to make all the Lag configurations.
    """

    def __init__(self, test_obj: 'T0TestBase') -> None:
        """
        Init Lag configrer.

        Args:
            test_obj: the test object
        """
        self.test_obj = test_obj
        self.client = test_obj.client

    def create_lag(self, lag_port_idxs):
        """
        Create lag and its members.

        Args:
            lag_port_idxs: lag port indexs

        Returns:
            Lag: lag object
        """
        lag: Lag = Lag(None, [], [])
        lag_oid = sai_thrift_create_lag(self.client)
        lag.oid = lag_oid
        self.create_lag_member(lag, lag_port_idxs)
        return lag

    def create_lag_member(self, lag_obj, lag_port_idxs):
        """
        Create lag members for a lag.

        Args:
            lag_obj: lag object
            lag_port_idxs: lag member port indexs

        Returns:
            lag_members: list of lag_member
        """
        lag: Lag = lag_obj

        lag_members = []
        for port_index in lag_port_idxs:
            lag_member = sai_thrift_create_lag_member(self.client,
                                                      lag_id=lag.oid,
                                                      port_id=self.test_obj.dut.port_obj_list[port_index].oid)
            self.test_obj.assertEqual(
                self.test_obj.status(), SAI_STATUS_SUCCESS)
            lag_members.append(lag_member)
            lag.lag_members.append(lag_member)
            lag.member_port_indexs.append(port_index)
        return lag_members

    def set_lag_hash_algorithm(self, algo=SAI_HASH_ALGORITHM_CRC):
        """
        Set lag hash algorithm.

        Args:
            algo (int): hash algorithm id
        """
        sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_algorithm=algo)

    def setup_lag_v4_hash(self, hash_fields_list=None, lag_hash_ipv4=None):
        """
        Setup lag v4 hash.

        SAI_NATIVE_HASH_FIELD_SRC_IP
        SAI_NATIVE_HASH_FIELD_DST_IP
        SAI_NATIVE_HASH_FIELD_IP_PROTOCOL
        SAI_NATIVE_HASH_FIELD_L4_DST_PORT
        SAI_NATIVE_HASH_FIELD_L4_SRC_PORT

        """
        if hash_fields_list is None:
            hash_fields_list = [SAI_NATIVE_HASH_FIELD_SRC_IP,
                                SAI_NATIVE_HASH_FIELD_DST_IP,
                                SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                                SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                                SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]

        if lag_hash_ipv4 is None:
            # create new hash
            s32list = sai_thrift_s32_list_t(
                count=len(hash_fields_list), int32list=hash_fields_list)
            lag_hash_ipv4 = sai_thrift_create_hash(
                self.client, native_hash_field_list=s32list)
            self.test_obj.assertTrue(
                lag_hash_ipv4 != 0, "Failed to create IPv4 lag hash")
            status = sai_thrift_set_switch_attribute(
                self.client, lag_hash_ipv4=lag_hash_ipv4)
            self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)
        else:
            # update existing hash
            s32list = sai_thrift_s32_list_t(
                count=len(hash_fields_list), int32list=hash_fields_list)
            status = sai_thrift_set_hash_attribute(
                self.client, lag_hash_ipv4, native_hash_field_list=s32list)
            self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)

    def set_lag_hash_seed(self, seed=400):
        """
        Set lag hash seed.

        Args:
            seed: hash seed value
        """
        status = sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_seed=seed)
        self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)

    def remove_lag_member_by_port_idx(self, lag_obj, port_idx):
        """
        Remove lag member by port index.

        This method will remove the lag member and port index from lag object.

        Args:
            lag_obj: Lag object.
            port_idx: port index.
        """
        lag: Lag = lag_obj
        index = lag.member_port_indexs.index(port_idx)
        sai_thrift_remove_lag_member(self.client, lag.lag_members[index])
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
        lag.lag_members.remove(lag.lag_members[index])
        lag.member_port_indexs.remove(port_idx)

    def remove_lag(self, lag_oid):
        """
        Remove lag.
        """
        sai_thrift_remove_lag(self.client, lag_oid)
