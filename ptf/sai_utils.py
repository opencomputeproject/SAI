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
Thrift SAI interface basic utils.
"""


def sai_thrift_query_attribute_enum_values_capability(client,
                                                      obj_type,
                                                      attr_id=None):
    """
    Call the sai_thrift_query_attribute_enum_values_capability() function
    and return the list of supported aattr_is enum capabilities

    Args:
        client (Client): SAI RPC client
        obj_type (enum): SAI object type
        attr_id (attr): SAI attribute name

    Returns:
        list: list of switch object type enum capabilities
    """
    max_cap_no = 20

    enum_cap_list = client.sai_thrift_query_attribute_enum_values_capability(
        obj_type, attr_id, max_cap_no)

    return enum_cap_list


def sai_thrift_object_type_get_availability(client,
                                            obj_type,
                                            attr_id=None,
                                            attr_type=None):
    """
    sai_thrift_object_type_get_availability() RPC client function
    implementation

    Args:
        client (Client): SAI RPC client
        obj_type (enum): SAI object type
        attr_id (attr): SAI attribute name
        attr_type (type): SAI attribute type

    Returns:
        uint: number of available resources with given parameters
    """
    availability_cnt = client.sai_thrift_object_type_get_availability(
        obj_type, attr_id, attr_type)

    return availability_cnt


def sai_thrift_get_debug_counter_port_stats(client, port_oid, counter_ids):
    """
    Get port statistics for given debug counters

    Args:
        client (Client): SAI RPC client
        port_oid (sai_thrift_object_id_t): object_id IN argument
        counter_ids (sai_stat_id_t): list of requested counters

    Returns:
        Dict[str, sai_thrift_uint64_t]: stats
    """

    stats = dict()
    counters = client.sai_thrift_get_port_stats(port_oid, counter_ids)

    for i, counter_id in enumerate(counter_ids):
        stats[counter_id] = counters[i]

    return stats


def sai_thrift_get_debug_counter_switch_stats(client, counter_ids):
    """
    Get switch statistics for given debug counters

    Args:
        client (Client): SAI RPC client
        counter_ids (sai_stat_id_t): list of requested counters

    Returns:
        Dict[str, sai_thrift_uint64_t]: stats
    """

    stats = dict()
    counters = client.sai_thrift_get_switch_stats(counter_ids)

    for i, counter_id in enumerate(counter_ids):
        stats[counter_id] = counters[i]

    return stats
