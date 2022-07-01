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


def t0_lag_config_helper(test_obj, is_create_lag=True):
    """
    Make lag configurations base on the configuration in the test plan.
    set the configuration in test directly.

    set the following test_obj attributes:
        lag object
    
    """
    lag_configer = LagConfiger(test_obj)

    if is_create_lag:
        lag1 = lag_configer.create_lag([17, 18])
        test_obj.lag1 = lag1
        ip_addr1 = '10.10.10.0'
        mac_addr1 = '02:04:02:01:01:01'
        lag_configer.create_route_and_neighbor_entry_for_lag(
            lag_id=lag1.lag_id, 
            ip_addr=ip_addr1, 
            mac_addr=mac_addr1, 
            port_id=21)
        
        lag2 = lag_configer.create_lag([19, 20])
        test_obj.lag2 = lag2
        ip_addr2 = '10.1.2.100'
        mac_addr2 = '02:04:02:01:02:01'
        lag_configer.create_route_and_neighbor_entry_for_lag(
            lag_id=lag2.lag_id, 
            ip_addr=ip_addr2, 
            mac_addr=mac_addr2, 
            port_id=22)

    lag_configer.set_lag_hash_algorithm()
    lag_configer.setup_lag_v4_hash()
    # lag_configer.set_lag_hash_attribute()
    lag_configer.set_lag_hash_seed()


class LagConfiger(object):
    """
    Class use to make all the Lag configurations.
    """

    def __init__(self, test_obj) -> None:
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

        lag = Lag()
        lag_id = sai_thrift_create_lag(self.client)
        lag_members = self.create_lag_member(lag_id, lag_port_idxs)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
        lag.lag_id = lag_id
        lag.lag_members = lag_members
        return lag
    
    def create_lag_member(self, lag_id, lag_port_idxs):
        """
        Create lag members for a lag.

        Args:
            lag: lag object
            lag_port_idxs: lag member port indexs

        Returns:
            lag_members: list of lag_member
        """

        lag_members = []
        for port_index in lag_port_idxs:
            lag_member = sai_thrift_create_lag_member(self.client, 
                                                      lag_id=lag_id, 
                                                      port_id=self.test_obj.port_list[port_index])
            self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
            lag_members.append(lag_member)
        return lag_members
    
    def set_lag_hash_algorithm(self, algo=SAI_HASH_ALGORITHM_CRC):
        """
        Set lag hash algorithm.

        Args:
            algo (int): hash algorithm id
        """
        sai_thrift_set_switch_attribute(self.client, lag_default_hash_algorithm=algo)

    def setup_lag_v4_hash(self, hash_fields_list=None):
        if hash_fields_list is None:
            hash_fields_list = [SAI_NATIVE_HASH_FIELD_SRC_IP,
                                SAI_NATIVE_HASH_FIELD_DST_IP,
                                SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                                SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                                SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]
        attr_list = sai_thrift_get_switch_attribute(self.client, lag_hash_ipv4=True)
        lag_hash_ipv4 = attr_list['SAI_SWITCH_ATTR_LAG_HASH_IPV4']

        if lag_hash_ipv4 == 0:
            # create new hash
            s32list = sai_thrift_s32_list_t(count=len(hash_fields_list), int32list=hash_fields_list)
            lag_hash_ipv4 = sai_thrift_create_hash(self.client, native_hash_field_list=s32list)
            self.test_obj.assertTrue(lag_hash_ipv4 != 0, "Failed to create IPv4 lag hash")
            sai_thrift_set_switch_attribute(self.client, lag_hash_ipv4=lag_hash_ipv4)
        else:
            # update existing hash
            s32list = sai_thrift_s32_list_t(count=len(hash_fields_list), int32list=hash_fields_list)
            status = sai_thrift_set_hash_attribute(self.client, lag_hash_ipv4, native_hash_field_list=s32list)
            self.test_obj.assertEqual(status, SAI_STATUS_SUCCESS)

    def set_lag_hash_attribute(self):
        """
        Set lag hash attributes.

        Args:
            hash_fields_list: hash fields list
        """
        attr_list = sai_thrift_get_switch_attribute(self.client, lag_hash=True)
        lag_hash_id = attr_list['SAI_SWITCH_ATTR_LAG_HASH']

        hash_fields_list = [SAI_NATIVE_HASH_FIELD_SRC_IP,
                            SAI_NATIVE_HASH_FIELD_DST_IP,
                            SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                            SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                            SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]

        hash_attr_list = sai_thrift_s32_list_t(count=len(hash_fields_list), int32list=hash_fields_list)
        sai_thrift_set_hash_attribute(self.client, lag_hash_id, native_hash_field_list=hash_attr_list)

    def set_lag_hash_seed(self, seed=400):
        """
        Set lag hash seed.

        Args:
            seed: hash seed value
        """
        sai_thrift_set_switch_attribute(self.client, lag_default_hash_seed=seed)

    def create_route_and_neighbor_entry_for_lag(self, lag_id, ip_addr, mac_addr, port_id):
        vr_id = sai_thrift_create_virtual_router(self.client)
        port2 = self.test_obj.port_list[port_id]

        rif_id1 = sai_thrift_create_router_interface(self.client, virtual_router_id=vr_id, type=SAI_ROUTER_INTERFACE_TYPE_PORT, port_id=lag_id)
        rif_id2 = sai_thrift_create_router_interface(self.client, virtual_router_id=vr_id, type=SAI_ROUTER_INTERFACE_TYPE_PORT, port_id=port2)
        
        nbr_entry_v4 = sai_thrift_neighbor_entry_t(rif_id=rif_id1, ip_address=sai_ipaddress(ip_addr))
        sai_thrift_create_neighbor_entry(self.client, nbr_entry_v4, dst_mac_address=mac_addr)

        nhop = sai_thrift_create_next_hop(self.client, ip=sai_ipaddress(ip_addr), router_interface_id=rif_id1, type=SAI_NEXT_HOP_TYPE_IP)
        route1 = sai_thrift_route_entry_t(vr_id=vr_id, destination=sai_ipprefix(ip_addr+'/24'))
        sai_thrift_create_route_entry(self.client, route1, next_hop_id=nhop)

    def remove_lag_member(self, lag_member):
        sai_thrift_remove_lag_member(self, lag_member)

    def remove_all_lag_members(self, lag_members):
        for lag_member in lag_members:
            sai_thrift_remove_lag_member(self.client, lag_member)
    
    def remove_lag(self, lag_id):
        sai_thrift_remove_lag(self.client, lag_id)

class Lag(object):
    """
    Represent the lag object.
    Attrs:
        lag_id: lag id
        lag_members: lag members
    """
    def __init__(self, lag_id=None, lag_members=None):
        self.lag_id = lag_id
        self.lag_members = lag_members