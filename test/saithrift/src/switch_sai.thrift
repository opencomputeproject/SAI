/*
Copyright 2013-present Barefoot Networks, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

namespace py switch_sai
namespace cpp switch_sai

typedef i64 sai_thrift_object_id_t
typedef i64 sai_thrift_uint64_t
typedef i16 sai_thrift_vlan_id_t
typedef string sai_thrift_mac_t
typedef byte sai_thrift_vlan_tagging_mode_t
typedef i32 sai_thrift_status_t
typedef string sai_thrift_ip4_t
typedef string sai_thrift_ip6_t
typedef byte sai_thrift_ip_addr_family_t
typedef byte sai_thrift_port_stp_port_state_t
typedef i32 sai_thrift_hostif_trap_id_t
typedef i32 sai_thrift_next_hop_type_t
typedef i32 sai_thrift_vlan_stat_counter_t
typedef i32 sai_thrift_policer_stat_counter_t
typedef i32 sai_thrift_port_stat_counter_t
typedef i32 sai_thrift_queue_stat_counter_t
typedef i32 sai_thrift_pg_stat_counter_t
typedef i32 sai_thrift_buffer_pool_stat_counter_t
typedef i32 sai_thrift_policer_stat_t
typedef i32 sai_thrift_stat_id_t

struct sai_thrift_fdb_entry_t {
    1: sai_thrift_mac_t mac_address;
    2: sai_thrift_object_id_t bv_id;
}

struct sai_thrift_vlan_port_t {
    1: sai_thrift_object_id_t port_id;
    2: sai_thrift_vlan_tagging_mode_t tagging_mode;
}

struct sai_thrift_ip_t {
    1: sai_thrift_ip4_t ip4;
    2: sai_thrift_ip6_t ip6;
}

struct sai_thrift_ip_address_t {
    1: sai_thrift_ip_addr_family_t addr_family;
    2: sai_thrift_ip_t addr;
}

struct sai_thrift_ip_prefix_t {
    1: sai_thrift_ip_addr_family_t addr_family;
    2: sai_thrift_ip_t addr;
    3: sai_thrift_ip_t mask;
}

struct sai_thrift_object_list_t {
    1: i32 count;
    2: list<sai_thrift_object_id_t> object_id_list;
}

struct sai_thrift_vlan_list_t {
    1: i32 vlan_count;
    2: list<sai_thrift_vlan_id_t> vlan_list;
}

struct sai_thrift_s32_list_t {
    1: i32 count;
    2: list<i32> s32list;
}

struct sai_thrift_acl_mask_t {
    1: byte u8;
    2: byte s8;
    3: i16 u16;
    4: i16 s16;
    5: i32 u32;
    6: i32 s32;
    7: sai_thrift_mac_t mac;
    8: sai_thrift_ip4_t ip4;
    9: sai_thrift_ip6_t ip6;
}

struct sai_thrift_acl_data_t {
    1: byte u8;
    2: byte s8;
    3: i16 u16;
    4: i16 s16;
    5: i32 u32;
    6: i32 s32;
    7: sai_thrift_mac_t mac;
    8: sai_thrift_ip4_t ip4;
    9: sai_thrift_ip6_t ip6;
    10: sai_thrift_object_id_t oid;
    11: sai_thrift_object_list_t objlist;
}

struct sai_thrift_acl_field_data_t
{
    1: bool enable;
    2: sai_thrift_acl_mask_t mask;
    3: sai_thrift_acl_data_t data;
}

struct sai_thrift_acl_parameter_t {
    1: byte u8;
    2: byte s8;
    3: i16 u16;
    4: i16 s16;
    5: i32 u32;
    6: i32 s32;
    7: sai_thrift_mac_t mac;
    8: sai_thrift_ip4_t ip4;
    9: sai_thrift_ip6_t ip6;
    10: sai_thrift_object_id_t oid;
    11: sai_thrift_object_list_t objlist;
}

struct sai_thrift_acl_action_data_t {
    1: bool enable;
    2: sai_thrift_acl_parameter_t parameter;
}

struct sai_thrift_u32_list_t {
    1: i32 count;
    2: list<i32> u32list;
}

struct sai_thrift_qos_map_params_t {
    1: byte tc;
    2: byte dscp;
    3: byte dot1p;
    4: byte prio;
    5: byte pg;
    6: byte queue_index;
    7: byte color;
}

struct sai_thrift_qos_map_t {
    1: sai_thrift_qos_map_params_t key;
    2: sai_thrift_qos_map_params_t value;
}

struct sai_thrift_qos_map_list_t {
    1: i32 count;
    2: list<sai_thrift_qos_map_t> map_list;
}

struct sai_thrift_fdb_values_t {
    1: sai_thrift_object_id_t bport_id;
    2: sai_thrift_fdb_entry_t thrift_fdb_entry;
}

struct sai_thrift_attribute_value_t {
    1:  bool booldata;
    2:  string chardata;
    3:  byte u8;
    4:  byte s8;
    5:  i16 u16;
    6:  i16 s16;
    7:  i32 u32;
    8:  i32 s32;
    9:  i64 u64;
    10: i64 s64;
    11: sai_thrift_mac_t mac;
    12: sai_thrift_object_id_t oid;
    13: sai_thrift_ip4_t ip4;
    14: sai_thrift_ip6_t ip6;
    15: sai_thrift_ip_address_t ipaddr;
    16: sai_thrift_object_list_t objlist;
    17: sai_thrift_vlan_list_t vlanlist;
    18: sai_thrift_acl_field_data_t aclfield;
    19: sai_thrift_acl_action_data_t aclaction;
    20: sai_thrift_u32_list_t u32list;
    21: sai_thrift_s32_list_t s32list;
    22: sai_thrift_qos_map_list_t qosmap;
    23: sai_thrift_fdb_values_t fdb_values;
}

struct sai_thrift_attribute_t {
    1: i32 id;
    2: sai_thrift_attribute_value_t value;
}

struct sai_thrift_route_entry_t {
    1: sai_thrift_object_id_t vr_id;
    2: sai_thrift_ip_prefix_t destination;
}

struct sai_thrift_neighbor_entry_t {
    1: sai_thrift_object_id_t rif_id;
    2: sai_thrift_ip_address_t ip_address;
}

struct sai_thrift_attribute_list_t {
    1: list<sai_thrift_attribute_t> attr_list;
    2: i32 attr_count; // redundant
}

struct sai_thrift_result_data_t {
    1: sai_thrift_object_list_t objlist;
    2: sai_thrift_object_id_t oid;
    3: i16 u16;
}

struct sai_thrift_result_t {
    1: sai_thrift_result_data_t data;
    2: sai_thrift_status_t status;
}

service switch_sai_rpc {
    //port API
    sai_thrift_status_t sai_thrift_set_port_attribute(1: sai_thrift_object_id_t port_id, 2: sai_thrift_attribute_t thrift_attr);
    sai_thrift_attribute_list_t sai_thrift_get_port_attribute(1: sai_thrift_object_id_t port_id);
    list<i64> sai_thrift_get_port_stats(
                             1: sai_thrift_object_id_t port_id,
                             2: list<sai_thrift_port_stat_counter_t> counter_ids,
                             3: i32 number_of_counters);
    sai_thrift_status_t sai_thrift_clear_port_all_stats(1: sai_thrift_object_id_t port_id)

    //fdb API
    sai_thrift_status_t sai_thrift_create_fdb_entry(1: sai_thrift_fdb_entry_t thrift_fdb_entry, 2: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_delete_fdb_entry(1: sai_thrift_fdb_entry_t thrift_fdb_entry);
    sai_thrift_status_t sai_thrift_flush_fdb_entries(1: list <sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_attribute_list_t sai_thrift_get_fdb_entries();

    //vlan API
    sai_thrift_object_id_t sai_thrift_create_vlan(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_vlan(1: sai_thrift_object_id_t vlan_oid);
    list<i64> sai_thrift_get_vlan_stats(
                             1: sai_thrift_vlan_id_t vlan_id,
                             2: list<sai_thrift_vlan_stat_counter_t> counter_ids,
                             3: i32 number_of_counters);
    sai_thrift_object_id_t sai_thrift_create_vlan_member(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_attribute_list_t sai_thrift_get_vlan_member_attribute(1: sai_thrift_object_id_t vlan_member_id);
    sai_thrift_status_t sai_thrift_remove_vlan_member(1: sai_thrift_object_id_t vlan_member_id);
    sai_thrift_attribute_list_t sai_thrift_get_vlan_attribute(1: sai_thrift_object_id_t vlan_id);
    sai_thrift_result_t sai_thrift_get_vlan_id(1: sai_thrift_object_id_t vlan_id);
    sai_thrift_status_t sai_thrift_set_vlan_attribute(1: sai_thrift_object_id_t vlan_oid,
                                                      2: sai_thrift_attribute_t thrift_attr);

    //virtual router API
    sai_thrift_object_id_t sai_thrift_create_virtual_router(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_virtual_router(1: sai_thrift_object_id_t vr_id);

    //route API
    sai_thrift_status_t sai_thrift_create_route(1: sai_thrift_route_entry_t thrift_route_entry, 2: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_route(1: sai_thrift_route_entry_t thrift_route_entry);

    //router interface API
    sai_thrift_object_id_t sai_thrift_create_router_interface(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_router_interface(1: sai_thrift_object_id_t rif_id);
    sai_thrift_status_t sai_thrift_set_router_interface_attribute(1: sai_thrift_object_id_t rif_id, 2: sai_thrift_attribute_t thrift_attr);

    //next hop API
    sai_thrift_object_id_t sai_thrift_create_next_hop(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_next_hop(1: sai_thrift_object_id_t next_hop_id);

    // Next Hop Group API.
    sai_thrift_object_id_t sai_thrift_create_next_hop_group(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_next_hop_group(1: sai_thrift_object_id_t nhop_group_oid);
    sai_thrift_object_id_t sai_thrift_create_next_hop_group_member(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_next_hop_group_member(1: sai_thrift_object_id_t nhop_group_member_oid);

    //lag API
    sai_thrift_object_id_t sai_thrift_create_lag(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_lag(1: sai_thrift_object_id_t lag_id);
    sai_thrift_status_t sai_thrift_set_lag_attribute(1: sai_thrift_object_id_t lag_id,
                                                     2: sai_thrift_attribute_t thrift_attr);
    sai_thrift_object_id_t sai_thrift_create_lag_member(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_lag_member(1: sai_thrift_object_id_t lag_member_id);
    sai_thrift_attribute_list_t sai_thrift_get_lag_member_attribute(1: sai_thrift_object_id_t lag_member_id);

    //stp API
    sai_thrift_object_id_t sai_thrift_create_stp_entry(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_stp_entry(1: sai_thrift_object_id_t stp_id);
    sai_thrift_status_t sai_thrift_set_stp_port_state(1: sai_thrift_object_id_t stp_id, 2: sai_thrift_object_id_t port_id, 3: sai_thrift_port_stp_port_state_t stp_port_state);
    sai_thrift_port_stp_port_state_t sai_thrift_get_stp_port_state(1: sai_thrift_object_id_t stp_id, 2: sai_thrift_object_id_t port_id);

    //neighbor API
    sai_thrift_status_t sai_thrift_create_neighbor_entry(1: sai_thrift_neighbor_entry_t thrift_neighbor_entry, 2: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_neighbor_entry(1: sai_thrift_neighbor_entry_t thrift_neighbor_entry);
    sai_thrift_status_t sai_thrift_set_neighbor_entry_attribute(1: sai_thrift_neighbor_entry_t thrift_neighbor_entry, 2: list<sai_thrift_attribute_t> thrift_attr);

    //switch API
    sai_thrift_attribute_list_t sai_thrift_get_switch_attribute();
    sai_thrift_attribute_t sai_thrift_get_port_list_by_front_port();
    sai_thrift_object_id_t sai_thrift_get_cpu_port_id();
    sai_thrift_object_id_t sai_thrift_get_default_trap_group();
    sai_thrift_object_id_t sai_thrift_get_default_router_id();
    sai_thrift_object_id_t sai_thrift_get_default_1q_bridge_id();
    sai_thrift_result_t sai_thrift_get_default_vlan_id();
    sai_thrift_object_id_t sai_thrift_get_port_id_by_front_port(1: string port_name);
    sai_thrift_status_t sai_thrift_set_switch_attribute(1: sai_thrift_attribute_t attribute);
    i64 sai_thrift_get_switch_stats_by_oid(1: sai_thrift_object_id_t thrift_counter_id);

    //bridge API
    sai_thrift_result_t sai_thrift_create_bridge_port(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_bridge_port(1: sai_thrift_object_id_t bridge_port_id);
    sai_thrift_result_t sai_thrift_get_bridge_port_list(1: sai_thrift_object_id_t bridge_id);
    sai_thrift_attribute_list_t sai_thrift_get_bridge_port_attribute(1: sai_thrift_object_id_t bridge_port_id);
    sai_thrift_status_t sai_thrift_set_bridge_port_attribute(1: sai_thrift_object_id_t bridge_port_id,
                                                             2: sai_thrift_attribute_t thrift_attr);
    sai_thrift_result_t sai_thrift_create_bridge(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_bridge(1: sai_thrift_object_id_t bridge_id);

    //Trap API
    sai_thrift_object_id_t sai_thrift_create_hostif(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_hostif(1: sai_thrift_object_id_t thrift_hif_id);
    sai_thrift_status_t sai_thrift_set_hostif_attribute(1: sai_thrift_object_id_t thrift_hif_id,
                                                        2: sai_thrift_attribute_t thrift_attr);

    sai_thrift_object_id_t sai_thrift_create_hostif_table_entry(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_hostif_table_entry(1: sai_thrift_object_id_t thrift_hif_table_entry_id);
    sai_thrift_status_t sai_thrift_set_hostif_table_entry_attribute(1: sai_thrift_object_id_t thrift_hif_table_entry_id,
                                                                    2: sai_thrift_attribute_t thrift_attr);

    sai_thrift_object_id_t sai_thrift_create_hostif_trap_group(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_hostif_trap_group(1: sai_thrift_object_id_t thrift_hostif_trap_group_id);
    sai_thrift_status_t sai_thrift_set_hostif_trap_group_attribute(1: sai_thrift_object_id_t thrift_hostif_trap_group_id,
                                                                   2: sai_thrift_attribute_t thrift_attr);

    sai_thrift_object_id_t sai_thrift_create_hostif_trap(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_hostif_trap(1: sai_thrift_object_id_t thrift_hostif_trap_id);
    sai_thrift_status_t sai_thrift_set_hostif_trap_attribute(1: sai_thrift_object_id_t thrift_hostif_trap_id,
                                                             2: sai_thrift_attribute_t thrift_attr);

    // ACL API
    sai_thrift_object_id_t sai_thrift_create_acl_table(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_acl_table(1: sai_thrift_object_id_t acl_table_id);

    sai_thrift_object_id_t sai_thrift_create_acl_entry(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_acl_entry(1: sai_thrift_object_id_t acl_entry);

    sai_thrift_object_id_t sai_thrift_create_acl_table_group(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_acl_table_group(1: sai_thrift_object_id_t acl_table_group_id);

    sai_thrift_object_id_t sai_thrift_create_acl_table_group_member(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_acl_table_group_member(1: sai_thrift_object_id_t acl_table_group_member_id);

    sai_thrift_object_id_t sai_thrift_create_acl_counter(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_acl_counter(1: sai_thrift_object_id_t acl_counter_id);
    list<sai_thrift_attribute_value_t> sai_thrift_get_acl_counter_attribute(
                             1: sai_thrift_object_id_t acl_counter_id,
                             2: list<i32> thrift_attr_ids);

    // Mirror API
    sai_thrift_object_id_t sai_thrift_create_mirror_session(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_mirror_session(1: sai_thrift_object_id_t session_id);
    sai_thrift_status_t sai_thrift_set_mirror_session_attribute(1: sai_thrift_object_id_t session_id, 2: sai_thrift_attribute_t thrift_attr);

    // Policer API
    sai_thrift_object_id_t sai_thrift_create_policer(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_policer(1: sai_thrift_object_id_t thrift_policer_id);

    sai_thrift_status_t sai_thrift_set_policer_attribute(1: sai_thrift_object_id_t thrift_policer_id,
                                                         2: sai_thrift_attribute_t thrift_attr);
    list<sai_thrift_uint64_t> sai_thrift_get_policer_stats(1: sai_thrift_object_id_t thrift_policer_id,
                                                           2: list<sai_thrift_policer_stat_t> thrift_counter_ids);

    sai_thrift_status_t sai_thrift_clear_policer_stats(1: sai_thrift_object_id_t thrift_policer_id,
                                                       2: list<sai_thrift_policer_stat_t> thrift_counter_ids);

    // Scheduler API
    sai_thrift_object_id_t sai_thrift_create_scheduler_profile(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_scheduler_profile(1: sai_thrift_object_id_t scheduler_id);

    // Queue API
    list<i64> sai_thrift_get_queue_stats(
                             1: sai_thrift_object_id_t queue_id,
                             2: list<sai_thrift_queue_stat_counter_t> counter_ids,
                             3: i32 number_of_counters);
    sai_thrift_status_t sai_thrift_clear_queue_stats(
                             1: sai_thrift_object_id_t queue_id,
                             2: list<sai_thrift_queue_stat_counter_t> counter_ids,
                             3: i32 number_of_counters);
    sai_thrift_status_t sai_thrift_set_queue_attribute(1: sai_thrift_object_id_t queue_id,
                                                       2: sai_thrift_attribute_t thrift_attr)

    // Buffer API
    sai_thrift_object_id_t sai_thrift_create_buffer_profile(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_object_id_t sai_thrift_create_pool_profile(1: list<sai_thrift_attribute_t> thrift_attr_list);
    list<i64> sai_thrift_get_buffer_pool_stats(
                        1: sai_thrift_object_id_t buffer_pool_id,
                        2: list<sai_thrift_buffer_pool_stat_counter_t> counter_ids);
    sai_thrift_status_t sai_thrift_clear_buffer_pool_stats(
                        1: sai_thrift_object_id_t buffer_pool_id,
                        2: list<sai_thrift_buffer_pool_stat_counter_t> counter_ids);
    sai_thrift_status_t sai_thrift_set_priority_group_attribute(1: sai_thrift_object_id_t pg_id,
                                                                2: sai_thrift_attribute_t thrift_attr)
    list<i64> sai_thrift_get_pg_stats(
                        1: sai_thrift_object_id_t pg_id,
                        2: list<sai_thrift_pg_stat_counter_t> counter_ids,
                        3: i32 number_of_counters);

    // WRED API
    sai_thrift_object_id_t sai_thrift_create_wred_profile(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_wred_profile(1: sai_thrift_object_id_t wred_id);

    // Tunnel API
    sai_thrift_object_id_t sai_thrift_create_tunnel(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_tunnel(1: sai_thrift_object_id_t thrift_tunnel_id);
    sai_thrift_object_id_t sai_thrift_create_tunnel_term_table_entry(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_tunnel_term_table_entry(1: sai_thrift_object_id_t thrift_tunnel_entry_id);

    // QoS Map API
    sai_thrift_object_id_t sai_thrift_create_qos_map(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_qos_map(1: sai_thrift_object_id_t qos_map_id);

    // Debug counters API
    sai_thrift_object_id_t sai_thrift_create_debug_counter(1: list<sai_thrift_attribute_t> thrift_attr_list);
    sai_thrift_status_t sai_thrift_remove_debug_counter(1: sai_thrift_object_id_t thrift_debug_counter_id);
    sai_thrift_status_t sai_thrift_set_debug_counter_attribute(1: sai_thrift_object_id_t dc_id,
                                                               2: sai_thrift_attribute_t thrift_attr);
    // VOQ API
    sai_thrift_object_id_t sai_thrift_get_sys_port_obj_id_by_port_id(1: i32 sys_port_id);
    sai_thrift_attribute_list_t sai_thrift_get_system_port_attribute(1: sai_thrift_object_id_t sys_port_object_id);
}
