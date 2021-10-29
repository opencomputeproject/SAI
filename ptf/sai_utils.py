# Copyright 2020-present Barefoot Networks, Inc.
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
Thrift SAI interface basic utils.
"""

# pylint: disable=too-many-arguments,too-many-branches,line-too-long
# pylint: disable=invalid-name
from sai_adapter import *


def sai_thrift_query_attribute_enum_values_capability(client,
                                                      obj_type,
                                                      attr_id):
    """
    function calls the sai_thrift_query_attribute_enum_values_capability()
    and returns the list of supported aattr_is enum capabilities
    Args:
        client (Client): SAI RPC client
        For the other parameters, see documentation.
    Returns:
        list: list of switch object type enum capabilities.
    """

    enum_cap_list = client.sai_thrift_query_attribute_enum_values_capability(
        obj_type, attr_id, 20)

    return enum_cap_list


def sai_thrift_object_type_get_availability(client,
                                            obj_type,
                                            attr_id=None,
                                            attr_type=None):
    """
    sai_thrift_object_type_get_availability() RPC client function
    implementation.

    Args:
        client (Client): SAI RPC client
        For the other parameters, see documentation.

    Returns:
        uint: availability_cnt, object type switch availability counter.
    """
    availability_cnt = client.sai_thrift_object_type_get_availability(
        obj_type, attr_id, attr_type)

    return availability_cnt


def sai_thrift_get_port_stats_ext_overwrite(client, port_oid, counter_ids,
                                            mode):
    """
    sai_get_port_stats_ext() - RPC client function implementation.
    WARNING: This function overwrites sai_adapter.py function and will be
             removed when th sai_adapter.py function has been fixed.

    Args:
        client (Client): SAI RPC client
        port_oid(sai_thrift_object_id_t): object_id IN argument
        counter_ids(sai_stat_id_t): list of requested counter ids
        mode(sai_thrift_stats_mode_t): stats_mode IN argument

    Returns:
        Dict[str, sai_thrift_uint64_t]: stats
    """

    stats = dict()
    counters = client.sai_thrift_get_port_stats_ext(port_oid, counter_ids,
                                                    mode)
    for i, counter_id in enumerate(counter_ids):
        stats[counter_id] = counters[i]

    return stats


sai_thrift_get_port_stats_ext = sai_thrift_get_port_stats_ext_overwrite


def sai_thrift_get_switch_stats_ext_overwrite(client, counter_ids, mode):
    """
    sai_get_switch_stats_ext() - RPC client function implementation.
    WARNING: This function overwrites sai_adapter.py function and will be
             removed when th sai_adapter.py function has been fixed.

    Args:
        client (Client): SAI RPC client
        counter_ids(sai_stat_id_t): list of requested counter ids
        mode(sai_thrift_stats_mode_t): stats_mode IN argument

    Returns:
        Dict[str, sai_thrift_uint64_t]: stats
    """

    stats = dict()
    counters = client.sai_thrift_get_switch_stats_ext(counter_ids, mode)
    for i, counter_id in enumerate(counter_ids):
        stats[counter_id] = counters[i]

    return stats


sai_thrift_get_switch_stats_ext = sai_thrift_get_switch_stats_ext_overwrite
