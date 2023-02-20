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

"""
Thrift SAI interface basic utils.
"""

import os
import time
import struct
import socket

from functools import wraps
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import FunctionType 

from ptf.packet import *
from ptf.testutils import *
from config.switch_configer import t0_switch_config_helper
from sai_thrift.sai_adapter import *
import sai_thrift.sai_adapter as adapter
from constant import *


def sai_ipprefix(prefix_str):
    """
    Set IP address prefix and mask and return ip_prefix object

    Args:
        prefix_str (str): IP address and mask string (with slash notation)

    Return:
        sai_thrift_ip_prefix_t: IP prefix object
    """
    addr_mask = prefix_str.split('/')
    if len(addr_mask) != 2:
        print("Invalid IP prefix format")
        return None

    if '.' in prefix_str:
        family = SAI_IP_ADDR_FAMILY_IPV4
        addr = sai_thrift_ip_addr_t(ip4=addr_mask[0])
        mask = num_to_dotted_quad(addr_mask[1])
        mask = sai_thrift_ip_addr_t(ip4=mask)
    if ':' in prefix_str:
        family = SAI_IP_ADDR_FAMILY_IPV6
        addr = sai_thrift_ip_addr_t(ip6=addr_mask[0])
        mask = num_to_dotted_quad(int(addr_mask[1]), ipv4=False)
        mask = sai_thrift_ip_addr_t(ip6=mask)

    ip_prefix = sai_thrift_ip_prefix_t(
        addr_family=family, addr=addr, mask=mask)
    return ip_prefix


def num_to_dotted_quad(address, ipv4=True):
    """
    Helper function to convert the ip address

    Args:
        address (str): IP address
        ipv4 (bool): determines what IP version is handled

    Returns:
        str: formatted IP address
    """
    if ipv4 is True:
        mask = (1 << 32) - (1 << 32 >> int(address))
        return socket.inet_ntop(socket.AF_INET, struct.pack('>L', mask))

    mask = (1 << 128) - (1 << 128 >> int(address))
    i = 0
    result = ''
    for sign in str(hex(mask)[2:]):
        if (i + 1) % 4 == 0:
            result = result + sign + ':'
        else:
            result = result + sign
        i += 1
    return result[:-1]


def sai_ipaddress(addr_str):
    """
    Set SAI IP address, assign appropriate type and return
    sai_thrift_ip_address_t object

    Args:
        addr_str (str): IP address string

    Returns:
        sai_thrift_ip_address_t: object containing IP address family and number
    """

    if '.' in addr_str:
        family = SAI_IP_ADDR_FAMILY_IPV4
        addr = sai_thrift_ip_addr_t(ip4=addr_str)
    if ':' in addr_str:
        family = SAI_IP_ADDR_FAMILY_IPV6
        addr = sai_thrift_ip_addr_t(ip6=addr_str)
    ip_addr = sai_thrift_ip_address_t(addr_family=family, addr=addr)

    return ip_addr


def generate_mac_address_list(role, group, indexes):
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


def generate_ip_address_list(role, group, indexes):
    """
    Generate ip addresses.

    Args:
        role: Role which is represented by the ip address(base on test plan config)
        group: group number for the ip address(base on test plan config)
        indexes: ip indexes

    Returns:
        default_1q_bridge_id
    """
    print("Generate IP ...")
    ip_list = []
    for index in indexes:
        ip_list.append(role.format(group, index))
    return ip_list


def sai_thrift_api_uninitialize(client):
    """
    sai_thrift_api_uninitialize() RPC client function
    implementation
    Args:
        client (Client): SAI RPC client
    Returns:
        uint: object type
    """
    obj_type = client.sai_thrift_api_uninitialize()

    return obj_type


def warm_test(is_test_rebooting:bool=False, time_out=60, interval=1):
    """
    Method decorator for the method on warm testing.
    
    Depends on parameters [test_reboot_mode] and [test_reboot_stage].
    Runs different method, test_starting, setUp_post_start and runTest
    
    args:
        is_test_rebooting: whether running the test case when saiserver container shut down
        time_out: check saiserver contianer restart is complete within a 
                  certain time limit.if time limit if exceeded, raise error
        interval: frequency of check
    """
    def _check_run_case(f):
        def test_director(inst, *args):
            if inst.test_reboot_mode == 'warm':
                print("shutdown the swich in warm mode")
                sai_thrift_set_switch_attribute(inst.client, restart_warm=True)
                sai_thrift_set_switch_attribute(inst.client, pre_shutdown=True)
                sai_thrift_remove_switch(inst.client)
                sai_thrift_api_uninitialize(inst.client)
                # write content to reboot-requested
                print("write rebooting to file")
                warm_file = open('/tmp/warm_reboot','w+')
                warm_file.write(WARM_TEST_REBOOTING)
                warm_file.close()
                times = 0
                try:
                    while 1:
                        print("reading content in the warm_reboot")
                        warm_file = open('/tmp/warm_reboot','r')
                        txt = warm_file.readline()
                        warm_file.close()                
                        if 'post_reboot_done' in txt:
                            print("warm reboot is done, next, we will run the case")
                            break
                        if is_test_rebooting:
                            print("running in the rebooting stage, text is ", txt)
                            f(inst)
                        times = times + 1
                        time.sleep(interval)
                        print("alreay wait for ",times)
                        if times > time_out:
                            raise Exception("time out")
                except Exception as e:
                    print(e)
                
                inst.createRpcClient()
                inst.test_reboot_stage  = WARM_TEST_POST_REBOOT
                t0_switch_config_helper(inst)
            return f(inst)
        return test_director
    return _check_run_case

def query_counter(test, cnt_func, *args, **kwargs):
    """
    Get counter by each counter id for the counter function.
    This method depends on sai_adapater generation pattern.
    The cnt_func name must be with pattern sai_thrift_get_<counter_query_func>
    Then, expect there will be a counter dict with pattern 
        sai_<counter_query_func>_ids_dict
    and a counter list with name pattern
        sai_<counter_query_func>_ids
    Args:
        test: object extends from base test
        cnt_func: counter function
        args: counter function parameters
        kwargs: counter function parameters with name
    return:
        result: dict, counter name and  value
        supported_counters: supported counter name list
        unsupported_counters: unsupported counter name list
    """
    
    fun_name = cnt_func.__name__
    result = {}
    supported_counters = []
    unsupported_counters = []
    if not fun_name.startswith("sai_thrift_get"):
        # cannot get the func name directly
        # it should be a wrapper
        fun_name = inspect.getclosurevars(cnt_func).nonlocals['func'].__name__ 
    if not fun_name.startswith("sai_thrift_get"):
        raise ArgumentError("Cannot get the expected counter query method name")  

    cnt_query_fun_name = fun_name.lstrip("sai_thrift_")
    id_dict_name = "sai_{}_counter_ids_dict".format(cnt_query_fun_name)
    id_list_name = "sai_{}_counter_ids".format(cnt_query_fun_name)
    id_dict = getattr(adapter, id_dict_name)
    id_list = getattr(adapter, id_list_name)

    ignore_api_errors()
    for id in id_list:
        kwargs["counter_ids"] = [id]
        counter = id_dict[id]
        stats = cnt_func(test.client, *args, **kwargs)
        if test.status() == SAI_STATUS_SUCCESS:
            supported_counters.append(counter)
        else:
            unsupported_counters.append(counter)
        result[counter] = stats[counter]
    restore_api_error_code()
    return result


def clear_counter(test, cnt_func, *args, **kwargs):
    """
    Clear counter by each counter id for the counter function.
    This method depends on sai_adapater generation pattern.
    The cnt_func name must be with pattern sai_thrift_clear_<counter_query_func>
    Then, expect there will be a counter dict with pattern 
        sai_<counter_query_func>_ids_dict
    and a counter list with name pattern
        sai_<counter_query_func>_ids
    Args:
        test: object extends from base test
        cnt_func: counter function
        args: counter function parameters
        kwargs: counter function parameters with name
    return:
        supported_counters: supported counter name list
        unsupported_counters: unsupported counter name list
    """
    
    fun_name = cnt_func.__name__
    supported_counters = []
    unsupported_counters = []
    if not fun_name.startswith("sai_thrift_clear"):
        # cannot get the func name directly
        # it should be a wrapper
        fun_name = inspect.getclosurevars(cnt_func).nonlocals['func'].__name__ 
    if not fun_name.startswith("sai_thrift_clear"):
        raise ArgumentError("Cannot get the expected counter clear method name")  

    cnt_clear_fun_name = fun_name.lstrip("sai_thrift_")
    id_dict_name = "sai_{}_counter_ids_dict".format(cnt_clear_fun_name)
    id_list_name = "sai_{}_counter_ids".format(cnt_clear_fun_name)
    id_dict = getattr(adapter, id_dict_name)
    id_list = getattr(adapter, id_list_name)

    ignore_api_errors()
    for id in id_list:
        kwargs["counter_ids"] = [id]
        counter = id_dict[id]
        cnt_func(test.client, *args, **kwargs)
        if test.status() == SAI_STATUS_SUCCESS:
            supported_counters.append(counter)
        else:
            unsupported_counters.append(counter)
    restore_api_error_code()


capture_status = True
expected_code = []
def ignore_api_errors():
    """
    Ignore API errors.
    After run this function, all the API error will be caught
    and will not be raised.

    """
    #print("Ignore all the expect error code and exception captures.")
    global capture_status, expected_code
    capture_status = adapter.CATCH_EXCEPTIONS
    expected_code = adapter.EXPECTED_ERROR_CODE
    adapter.CATCH_EXCEPTIONS = True
    adapter.EXPECTED_ERROR_CODE = []
    return capture_status, expected_code


def restore_api_error_code():
    """
    Restore API error code and catch status.
    """
    #print("Restore all the expect error code and exception captures.")
    global capture_status, expected_code
    adapter.CATCH_EXCEPTIONS = capture_status
    adapter.EXPECTED_ERROR_CODE = expected_code
