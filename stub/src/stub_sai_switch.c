/*
 *  Copyright (C) 2014. Mellanox Technologies, Ltd. ALL RIGHTS RESERVED.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 */

#include "sai.h"
#include "stub_sai.h"

#undef  __MODULE__
#define __MODULE__ SAI_SWITCH

sai_switch_notification_t g_notification_callbacks;
uint32_t                  gh_sdk = 0;

sai_status_t stub_switch_port_number_get(_In_ const sai_object_key_t   *key,
                                         _Inout_ sai_attribute_value_t *value,
                                         _In_ uint32_t                  attr_index,
                                         _Inout_ vendor_cache_t        *cache,
                                         void                          *arg);
sai_status_t stub_switch_port_list_get(_In_ const sai_object_key_t   *key,
                                       _Inout_ sai_attribute_value_t *value,
                                       _In_ uint32_t                  attr_index,
                                       _Inout_ vendor_cache_t        *cache,
                                       void                          *arg);
sai_status_t stub_switch_cpu_port_get(_In_ const sai_object_key_t   *key,
                                      _Inout_ sai_attribute_value_t *value,
                                      _In_ uint32_t                  attr_index,
                                      _Inout_ vendor_cache_t        *cache,
                                      void                          *arg);
sai_status_t stub_switch_max_vr_get(_In_ const sai_object_key_t   *key,
                                    _Inout_ sai_attribute_value_t *value,
                                    _In_ uint32_t                  attr_index,
                                    _Inout_ vendor_cache_t        *cache,
                                    void                          *arg);
sai_status_t stub_switch_fdb_size_get(_In_ const sai_object_key_t   *key,
                                      _Inout_ sai_attribute_value_t *value,
                                      _In_ uint32_t                  attr_index,
                                      _Inout_ vendor_cache_t        *cache,
                                      void                          *arg);
sai_status_t stub_switch_max_temp_get(_In_ const sai_object_key_t   *key,
                                      _Inout_ sai_attribute_value_t *value,
                                      _In_ uint32_t                  attr_index,
                                      _Inout_ vendor_cache_t        *cache,
                                      void                          *arg);
sai_status_t stub_switch_default_stp_get(_In_ const sai_object_key_t   *key,
                                         _Inout_ sai_attribute_value_t *value,
                                         _In_ uint32_t                  attr_index,
                                         _Inout_ vendor_cache_t        *cache,
                                         void                          *arg);
sai_status_t stub_switch_on_link_get(_In_ const sai_object_key_t   *key,
                                     _Inout_ sai_attribute_value_t *value,
                                     _In_ uint32_t                  attr_index,
                                     _Inout_ vendor_cache_t        *cache,
                                     void                          *arg);
sai_status_t stub_switch_oper_status_get(_In_ const sai_object_key_t   *key,
                                         _Inout_ sai_attribute_value_t *value,
                                         _In_ uint32_t                  attr_index,
                                         _Inout_ vendor_cache_t        *cache,
                                         void                          *arg);
sai_status_t stub_switch_acl_table_min_prio_get(_In_ const sai_object_key_t   *key,
                                                _Inout_ sai_attribute_value_t *value,
                                                _In_ uint32_t                  attr_index,
                                                _Inout_ vendor_cache_t        *cache,
                                                void                          *arg);
sai_status_t stub_switch_acl_table_max_prio_get(_In_ const sai_object_key_t   *key,
                                                _Inout_ sai_attribute_value_t *value,
                                                _In_ uint32_t                  attr_index,
                                                _Inout_ vendor_cache_t        *cache,
                                                void                          *arg);
sai_status_t stub_switch_acl_entry_min_prio_get(_In_ const sai_object_key_t   *key,
                                                _Inout_ sai_attribute_value_t *value,
                                                _In_ uint32_t                  attr_index,
                                                _Inout_ vendor_cache_t        *cache,
                                                void                          *arg);
sai_status_t stub_switch_acl_entry_max_prio_get(_In_ const sai_object_key_t   *key,
                                                _Inout_ sai_attribute_value_t *value,
                                                _In_ uint32_t                  attr_index,
                                                _Inout_ vendor_cache_t        *cache,
                                                void                          *arg);
sai_status_t stub_switch_mode_get(_In_ const sai_object_key_t   *key,
                                  _Inout_ sai_attribute_value_t *value,
                                  _In_ uint32_t                  attr_index,
                                  _Inout_ vendor_cache_t        *cache,
                                  void                          *arg);
sai_status_t stub_switch_default_port_vlan_get(_In_ const sai_object_key_t   *key,
                                               _Inout_ sai_attribute_value_t *value,
                                               _In_ uint32_t                  attr_index,
                                               _Inout_ vendor_cache_t        *cache,
                                               void                          *arg);
sai_status_t stub_switch_src_mac_get(_In_ const sai_object_key_t   *key,
                                     _Inout_ sai_attribute_value_t *value,
                                     _In_ uint32_t                  attr_index,
                                     _Inout_ vendor_cache_t        *cache,
                                     void                          *arg);
sai_status_t stub_switch_aging_time_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg);
sai_status_t stub_switch_ecmp_hash_seed_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg);
sai_status_t stub_switch_ecmp_hash_algo_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg);
sai_status_t stub_switch_ecmp_hash_fields_get(_In_ const sai_object_key_t   *key,
                                              _Inout_ sai_attribute_value_t *value,
                                              _In_ uint32_t                  attr_index,
                                              _Inout_ vendor_cache_t        *cache,
                                              void                          *arg);
sai_status_t stub_switch_counter_refresh_get(_In_ const sai_object_key_t   *key,
                                             _Inout_ sai_attribute_value_t *value,
                                             _In_ uint32_t                  attr_index,
                                             _Inout_ vendor_cache_t        *cache,
                                             void                          *arg);
sai_status_t stub_switch_default_trap_channel_get(_In_ const sai_object_key_t   *key,
                                                  _Inout_ sai_attribute_value_t *value,
                                                  _In_ uint32_t                  attr_index,
                                                  _Inout_ vendor_cache_t        *cache,
                                                  void                          *arg);
sai_status_t stub_switch_default_trap_channel_fd_get(_In_ const sai_object_key_t   *key,
                                                     _Inout_ sai_attribute_value_t *value,
                                                     _In_ uint32_t                  attr_index,
                                                     _Inout_ vendor_cache_t        *cache,
                                                     void                          *arg);
sai_status_t stub_switch_default_trap_group_get(_In_ const sai_object_key_t   *key,
                                                _Inout_ sai_attribute_value_t *value,
                                                _In_ uint32_t                  attr_index,
                                                _Inout_ vendor_cache_t        *cache,
                                                void                          *arg);
sai_status_t stub_switch_mode_set(_In_ const sai_object_key_t      *key,
                                  _In_ const sai_attribute_value_t *value,
                                  void                             *arg);
sai_status_t stub_switch_default_port_vlan_set(_In_ const sai_object_key_t      *key,
                                               _In_ const sai_attribute_value_t *value,
                                               void                             *arg);
sai_status_t stub_switch_aging_time_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg);
sai_status_t stub_switch_ecmp_hash_seed_set(_In_ const sai_object_key_t      *key,
                                            _In_ const sai_attribute_value_t *value,
                                            void                             *arg);
sai_status_t stub_switch_ecmp_hash_algo_set(_In_ const sai_object_key_t      *key,
                                            _In_ const sai_attribute_value_t *value,
                                            void                             *arg);
sai_status_t stub_switch_ecmp_hash_fields_set(_In_ const sai_object_key_t      *key,
                                              _In_ const sai_attribute_value_t *value,
                                              void                             *arg);
sai_status_t stub_switch_counter_refresh_set(_In_ const sai_object_key_t      *key,
                                             _In_ const sai_attribute_value_t *value,
                                             void                             *arg);
sai_status_t stub_switch_default_trap_channel_set(_In_ const sai_object_key_t      *key,
                                                  _In_ const sai_attribute_value_t *value,
                                                  void                             *arg);
sai_status_t stub_switch_default_trap_channel_fd_set(_In_ const sai_object_key_t      *key,
                                                     _In_ const sai_attribute_value_t *value,
                                                     void                             *arg);
sai_status_t stub_switch_default_trap_group_set(_In_ const sai_object_key_t      *key,
                                                _In_ const sai_attribute_value_t *value,
                                                void                             *arg);

static const sai_attribute_entry_t        switch_attribs[] = {
    { SAI_SWITCH_ATTR_PORT_NUMBER, false, false, false, true,
      "Switch ports number", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_SWITCH_ATTR_PORT_LIST, false, false, false, true,
      "Switch ports list", SAI_ATTR_VAL_TYPE_OBJLIST },
    { SAI_SWITCH_ATTR_CPU_PORT, false, false, false, true,
      "Switch CPU port", SAI_ATTR_VAL_TYPE_OID },
    { SAI_SWITCH_ATTR_MAX_VIRTUAL_ROUTERS, false, false, false, true,
      "Switch max virtual routers", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_SWITCH_ATTR_FDB_TABLE_SIZE, false, false, false, true,
      "Switch FDB table size", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_SWITCH_ATTR_ON_LINK_ROUTE_SUPPORTED, false, false, false, true,
      "Switch on link route supported", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_SWITCH_ATTR_OPER_STATUS, false, false, false, true,
      "Switch operational status", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_SWITCH_ATTR_MAX_TEMP, false, false, false, true,
      "Switch maximum temperature", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY, false, false, false, true,
      "Switch ACL table min prio", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_SWITCH_ATTR_ACL_TABLE_MAXIMUM_PRIORITY, false, false, false, true,
      "Switch ACL table max prio", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY, false, false, false, true,
      "Switch ACL entry min prio", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_SWITCH_ATTR_ACL_ENTRY_MAXIMUM_PRIORITY, false, false, false, true,
      "Switch ACL entry max prio", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID, false, false, false, true,
      "Switch maximum temperature", SAI_ATTR_VAL_TYPE_OID },
    { SAI_SWITCH_ATTR_SWITCHING_MODE, false, false, true, true,
      "Switch switching mode", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_SWITCH_ATTR_BCAST_CPU_FLOOD_ENABLE, false, false, true, true,
      "Switch broadcast flood control to cpu", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_SWITCH_ATTR_MCAST_CPU_FLOOD_ENABLE, false, false, true, true,
      "Switch multicast flood control to cpu", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_SWITCH_ATTR_SRC_MAC_ADDRESS, false, false, true, true,
      "Switch source MAC address", SAI_ATTR_VAL_TYPE_MAC },
    { SAI_SWITCH_ATTR_MAX_LEARNED_ADDRESSES, false, false, true, true,
      "Switch maximum number of learned MAC addresses", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_SWITCH_ATTR_FDB_AGING_TIME, false, false, true, true,
      "Switch FDB aging time", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_SWITCH_ATTR_FDB_UNICAST_MISS_ACTION, false, false, true, true,
      "Switch flood control for unknown unicast address", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_ACTION, false, false, true, true,
      "Switch flood control for unknown broadcast address", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_ACTION, false, false, true, true,
      "Switch flood control for unknown multicast address", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_SEED, false, false, true, true,
      "Switch LAG hash seed", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_ALGORITHM, false, false, true, true,
      "Switch LAG hash algorithm", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_SWITCH_ATTR_LAG_HASH, false, false, true, true,
      "Switch LAG hash fields", SAI_ATTR_VAL_TYPE_S32LIST },
    { SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_SEED, false, false, true, true,
      "Switch ECMP hash seed", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM, false, false, true, true,
      "Switch ECMP hash algorithm", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_SWITCH_ATTR_ECMP_HASH, false, false, true, true,
      "Switch ECMP hash fields", SAI_ATTR_VAL_TYPE_S32LIST },
    { SAI_SWITCH_ATTR_COUNTER_REFRESH_INTERVAL, false, false, true, true,
      "Switch counter refresh interval", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP, false, false, true, true,
      "Switch default trap group", SAI_ATTR_VAL_TYPE_OID },
    { SAI_SWITCH_ATTR_PORT_BREAKOUT, false, false, true, false,
      "Switch port breakout mode", SAI_ATTR_VAL_TYPE_OID },
    { END_FUNCTIONALITY_ATTRIBS_ID, false, false, false, false,
      "", SAI_ATTR_VAL_TYPE_UNDETERMINED }
};
static const sai_vendor_attribute_entry_t switch_vendor_attribs[] = {
    { SAI_SWITCH_ATTR_PORT_NUMBER,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_port_number_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_PORT_LIST,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_port_list_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_CPU_PORT,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_cpu_port_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_MAX_VIRTUAL_ROUTERS,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_max_vr_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_FDB_TABLE_SIZE,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_fdb_size_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_ON_LINK_ROUTE_SUPPORTED,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_on_link_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_OPER_STATUS,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_oper_status_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_MAX_TEMP,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_max_temp_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_acl_table_min_prio_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_ACL_TABLE_MAXIMUM_PRIORITY,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_acl_table_max_prio_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_acl_entry_min_prio_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_ACL_ENTRY_MAXIMUM_PRIORITY,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_acl_entry_max_prio_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID,
      { false, false, false, true },
      { false, false, false, true },
      stub_switch_default_stp_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_SWITCHING_MODE,
      { false, false, true, true },
      { false, false, true, true },
      stub_switch_mode_get, NULL,
      stub_switch_mode_set, NULL },
    { SAI_SWITCH_ATTR_BCAST_CPU_FLOOD_ENABLE,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_MCAST_CPU_FLOOD_ENABLE,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_SRC_MAC_ADDRESS,
      { false, false, false, true },
      { false, false, true, true },
      stub_switch_src_mac_get, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_MAX_LEARNED_ADDRESSES,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_FDB_AGING_TIME,
      { false, false, true, true },
      { false, false, true, true },
      stub_switch_aging_time_get, NULL,
      stub_switch_aging_time_set, NULL },
    { SAI_SWITCH_ATTR_FDB_UNICAST_MISS_ACTION,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_ACTION,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_ACTION,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_SEED,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_ALGORITHM,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_LAG_HASH,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_SEED,
      { false, false, true, true },
      { false, false, true, true },
      stub_switch_ecmp_hash_seed_get, NULL,
      stub_switch_ecmp_hash_seed_set, NULL },
    { SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM,
      { false, false, true, true },
      { false, false, true, true },
      stub_switch_ecmp_hash_algo_get, NULL,
      stub_switch_ecmp_hash_algo_set, NULL },
    { SAI_SWITCH_ATTR_ECMP_HASH,
      { false, false, true, true },
      { false, false, true, true },
      stub_switch_ecmp_hash_fields_get, NULL,
      stub_switch_ecmp_hash_fields_set, NULL },
    { SAI_SWITCH_ATTR_COUNTER_REFRESH_INTERVAL,
      { false, false, true, true },
      { false, false, true, true },
      stub_switch_counter_refresh_get, NULL,
      stub_switch_counter_refresh_set, NULL },
    { SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP,
      { false, false, true, true },
      { false, false, true, true },
      stub_switch_default_trap_group_get, NULL,
      stub_switch_default_trap_group_set, NULL },
    { SAI_SWITCH_ATTR_PORT_BREAKOUT,
      { false, false, false, false },
      { false, false, true, false },
      NULL, NULL,
      NULL, NULL },
};


/*
 * Routine Description:
 *   SDK initialization. After the call the capability attributes should be
 *   ready for retrieval via sai_get_switch_attribute().
 *
 * Arguments:
 *   [in] profile_id - Handle for the switch profile.
 *   [in] switch_hardware_id - Switch hardware ID to open
 *   [in/opt] firmware_path_name - Vendor specific path name of the firmware
 *                                     to load
 *   [in] switch_notifications - switch notification table
 * Return Values:
 *   SAI_STATUS_SUCCESS on success
 *   Failure status code on error
 */
sai_status_t stub_initialize_switch(_In_ sai_switch_profile_id_t                           profile_id,
                                    _In_reads_z_(SAI_MAX_HARDWARE_ID_LEN) char           * switch_hardware_id,
                                    _In_reads_opt_z_(SAI_MAX_FIRMWARE_PATH_NAME_LEN) char* firmware_path_name,
                                    _In_ sai_switch_notification_t                       * switch_notifications)
{
    if (NULL == switch_hardware_id) {
        fprintf(stderr, "NULL switch hardware ID passed to SAI switch initialize\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == switch_notifications) {
        fprintf(stderr, "NULL switch notifications passed to SAI switch initialize\n");

        return SAI_STATUS_INVALID_PARAMETER;
    }

    gh_sdk = 1;
    memcpy(&g_notification_callbacks, switch_notifications, sizeof(g_notification_callbacks));

#ifndef _WIN32
    openlog("SAI", 0, LOG_USER);
#endif

    STUB_LOG_NTC("Initialize switch\n");

    db_init_vlan();
    db_init_next_hop_group();

    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *   Release all resources associated with currently opened switch
 *
 * Arguments:
 *   [in] warm_restart_hint - hint that indicates controlled warm restart.
 *                            Since warm restart can be caused by crash
 *                            (therefore there are no guarantees for this call),
 *                            this hint is really a performance optimization.
 *
 * Return Values:
 *   None
 */
void stub_shutdown_switch(_In_ bool warm_restart_hint)
{
    STUB_LOG_NTC("Shutdown switch\n");
    gh_sdk = 0;
}

/*
 * Routine Description:
 *   SDK connect. This API connects library to the initialized SDK.
 *   After the call the capability attributes should be ready for retrieval
 *   via sai_get_switch_attribute().
 *
 * Arguments:
 *   [in] profile_id - Handle for the switch profile.
 *   [in] switch_hardware_id - Switch hardware ID to open
 *   [in] switch_notifications - switch notification table
 * Return Values:
 *   SAI_STATUS_SUCCESS on success
 *   Failure status code on error
 */
sai_status_t stub_connect_switch(_In_ sai_switch_profile_id_t                profile_id,
                                 _In_reads_z_(SAI_MAX_HARDWARE_ID_LEN) char* switch_hardware_id,
                                 _In_ sai_switch_notification_t            * switch_notifications)
{
    if (NULL == switch_hardware_id) {
        fprintf(stderr, "NULL switch hardware ID passed to SAI switch connect\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == switch_notifications) {
        fprintf(stderr, "NULL switch notifications passed to SAI switch connect\n");

        return SAI_STATUS_INVALID_PARAMETER;
    }

    memcpy(&g_notification_callbacks, switch_notifications, sizeof(g_notification_callbacks));

    /* Open an handle if not done already on init for init agent */
    if (0 == gh_sdk) {
#ifndef _WIN32
        openlog("SAI", 0, LOG_USER);
#endif
    }

    db_init_next_hop_group();

    STUB_LOG_NTC("Connect switch\n");

    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *   Disconnect this SAI library from the SDK.
 *
 * Arguments:
 *   None
 * Return Values:
 *   None
 */
void stub_disconnect_switch(void)
{
    STUB_LOG_NTC("Disconnect switch\n");

    memset(&g_notification_callbacks, 0, sizeof(g_notification_callbacks));
}

/*
 * Routine Description:
 *    Set switch attribute value
 *
 * Arguments:
 *    [in] attr - switch attribute
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_set_switch_attribute(_In_ const sai_attribute_t *attr)
{
    STUB_LOG_ENTER();

    return sai_set_attribute(NULL, "", switch_attribs, switch_vendor_attribs, attr);
}

/* Switching mode [sai_switch_switching_mode_t]
 *  (default to SAI_SWITCHING_MODE_STORE_AND_FORWARD) */
sai_status_t stub_switch_mode_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value, void *arg)
{
    STUB_LOG_ENTER();

    switch (value->s32) {
    case SAI_SWITCHING_MODE_CUT_THROUGH:
        break;

    case SAI_SWITCHING_MODE_STORE_AND_FORWARD:
        break;

    default:
        STUB_LOG_ERR("Invalid rif port object type %s", "aa");
        /* STUB_LOG_ERR("Invalid switching mode value %d\n", value->s32); */
        return SAI_STATUS_INVALID_ATTR_VALUE_0;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default VlanID for ports that are not members of
*  any vlans [sai_vlan_id_t]  (default to vlan 1)*/
sai_status_t stub_switch_default_port_vlan_set(_In_ const sai_object_key_t      *key,
                                               _In_ const sai_attribute_value_t *value,
                                               void                             *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Dynamic FDB entry aging time in seconds [uint32_t]
 *   Zero means aging is disabled.
 *  (default to zero)
 */
sai_status_t stub_switch_aging_time_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* ECMP hashing seed  [uint32_t] */
sai_status_t stub_switch_ecmp_hash_seed_set(_In_ const sai_object_key_t      *key,
                                            _In_ const sai_attribute_value_t *value,
                                            void                             *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Hash algorithm for all ECMP in the switch[sai_switch_hash_algo_t] */
sai_status_t stub_switch_ecmp_hash_algo_set(_In_ const sai_object_key_t      *key,
                                            _In_ const sai_attribute_value_t *value,
                                            void                             *arg)
{
    STUB_LOG_ENTER();

    switch (value->s32) {
    case SAI_HASH_ALGORITHM_XOR:
        break;

    case SAI_HASH_ALGORITHM_CRC:
        break;

    case SAI_HASH_ALGORITHM_RANDOM:
        break;

    default:
        STUB_LOG_ERR("Invalid hash type value %d\n", value->s32);
        return SAI_STATUS_INVALID_ATTR_VALUE_0;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Hash fields for all ECMP in the switch[sai_s32_list_t] */
sai_status_t stub_switch_ecmp_hash_fields_set(_In_ const sai_object_key_t      *key,
                                              _In_ const sai_attribute_value_t *value,
                                              void                             *arg)
{
    uint32_t ii;

    STUB_LOG_ENTER();

    for (ii = 0; ii < value->s32list.count; ii++) {
        switch (value->s32list.list[ii]) {
        case SAI_NATIVE_HASH_FIELD_SRC_IP:
            break;

        case SAI_NATIVE_HASH_FIELD_DST_IP:
            break;

        case SAI_NATIVE_HASH_FIELD_VLAN_ID:
            break;

        case SAI_NATIVE_HASH_FIELD_IP_PROTOCOL:
            break;

        case SAI_NATIVE_HASH_FIELD_ETHERTYPE:
            break;

        case SAI_NATIVE_HASH_FIELD_L4_SRC_PORT:
            break;

        case SAI_NATIVE_HASH_FIELD_L4_DST_PORT:
            break;

        case SAI_NATIVE_HASH_FIELD_SRC_MAC:
            break;

        case SAI_NATIVE_HASH_FIELD_DST_MAC:
            break;

        case SAI_NATIVE_HASH_FIELD_IN_PORT:
            break;

        default:
            STUB_LOG_ERR("Invalid ecmp hash field , element %d, value %d\n", ii, value->s32list.list[ii]);
            return SAI_STATUS_INVALID_ATTR_VALUE_0;
        }
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* The SDK can
 * 1 - Read the counters directly from HW (or)
 * 2 - Cache the counters in SW. Caching is typically done if
 * retrieval of counters directly from HW for each counter
 * read is CPU intensive
 * This setting can be used to
 * 1 - Move from HW based to SW based or Vice versa
 * 2 - Configure the SW counter cache refresh rate
 * Setting a value of 0 enables direct HW based counter read. A
 * non zero value enables the SW cache based and the counter
 * refresh rate.
 * A NPU may support both or one of the option. It would return
 * error for unsupported options. [uint32_t]
 */
sai_status_t stub_switch_counter_refresh_set(_In_ const sai_object_key_t      *key,
                                             _In_ const sai_attribute_value_t *value,
                                             void                             *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default trap channel [sai_hostif_trap_channel_t] */
sai_status_t stub_switch_default_trap_channel_set(_In_ const sai_object_key_t      *key,
                                                  _In_ const sai_attribute_value_t *value,
                                                  void                             *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default file descriptor for SAI_HOSTIF_TRAP_CHANNEL_FD [sai_object_id_t] */
sai_status_t stub_switch_default_trap_channel_fd_set(_In_ const sai_object_key_t      *key,
                                                     _In_ const sai_attribute_value_t *value,
                                                     void                             *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default trap group [sai_object_id_t] */
sai_status_t stub_switch_default_trap_group_set(_In_ const sai_object_key_t      *key,
                                                _In_ const sai_attribute_value_t *value,
                                                void                             *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}
/*
 * Routine Description:
 *    Get switch attribute value
 *
 * Arguments:
 *    [in] attr_count - number of switch attributes
 *    [inout] attr_list - array of switch attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_get_switch_attribute(_In_ uint32_t attr_count, _Inout_ sai_attribute_t *attr_list)
{
    STUB_LOG_ENTER();

    return sai_get_attributes(NULL, "", switch_attribs, switch_vendor_attribs, attr_count, attr_list);
}

/* The number of ports on the switch [uint32_t] */
sai_status_t stub_switch_port_number_get(_In_ const sai_object_key_t   *key,
                                         _Inout_ sai_attribute_value_t *value,
                                         _In_ uint32_t                  attr_index,
                                         _Inout_ vendor_cache_t        *cache,
                                         void                          *arg)
{
    STUB_LOG_ENTER();

    value->u32 = PORT_NUMBER;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Get the port list [sai_object_list_t] */
sai_status_t stub_switch_port_list_get(_In_ const sai_object_key_t   *key,
                                       _Inout_ sai_attribute_value_t *value,
                                       _In_ uint32_t                  attr_index,
                                       _Inout_ vendor_cache_t        *cache,
                                       void                          *arg)
{
    sai_object_id_t ports[PORT_NUMBER];
    uint32_t        ii;
    sai_status_t    status;

    STUB_LOG_ENTER();

    for (ii = 0; ii < PORT_NUMBER; ii++) {
        if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_PORT, ii, &ports[ii]))) {
            return status;
        }
    }

    status = stub_fill_objlist(ports, PORT_NUMBER, &value->objlist);

    STUB_LOG_EXIT();

    return status;
}

/* Get the CPU Port [sai_object_id_t] */
sai_status_t stub_switch_cpu_port_get(_In_ const sai_object_key_t   *key,
                                      _Inout_ sai_attribute_value_t *value,
                                      _In_ uint32_t                  attr_index,
                                      _Inout_ vendor_cache_t        *cache,
                                      void                          *arg)
{
    sai_status_t status;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_PORT, 1, &value->oid))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Max number of virtual routers supported [uint32_t] */
sai_status_t stub_switch_max_vr_get(_In_ const sai_object_key_t   *key,
                                    _Inout_ sai_attribute_value_t *value,
                                    _In_ uint32_t                  attr_index,
                                    _Inout_ vendor_cache_t        *cache,
                                    void                          *arg)
{
    STUB_LOG_ENTER();

    value->u32 = 0;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* The size of the FDB Table in bytes [uint32_t] */
sai_status_t stub_switch_fdb_size_get(_In_ const sai_object_key_t   *key,
                                      _Inout_ sai_attribute_value_t *value,
                                      _In_ uint32_t                  attr_index,
                                      _Inout_ vendor_cache_t        *cache,
                                      void                          *arg)
{
    STUB_LOG_ENTER();

    value->u32 = 100000;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* The current value of the maximum temperature
 * retrieved from the switch sensors, in Celsius [int32_t] */
sai_status_t stub_switch_max_temp_get(_In_ const sai_object_key_t   *key,
                                      _Inout_ sai_attribute_value_t *value,
                                      _In_ uint32_t                  attr_index,
                                      _Inout_ vendor_cache_t        *cache,
                                      void                          *arg)
{
    STUB_LOG_ENTER();

    value->s32 = 50;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default SAI STP instance ID [sai_object_id_t] */
sai_status_t stub_switch_default_stp_get(_In_ const sai_object_key_t   *key,
                                         _Inout_ sai_attribute_value_t *value,
                                         _In_ uint32_t                  attr_index,
                                         _Inout_ vendor_cache_t        *cache,
                                         void                          *arg)
{
    sai_status_t status;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_STP_INSTANCE, 1, &value->oid))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 *   Local subnet routing supported [bool]
 *   Routes with next hop set to "on-link"
 */
sai_status_t stub_switch_on_link_get(_In_ const sai_object_key_t   *key,
                                     _Inout_ sai_attribute_value_t *value,
                                     _In_ uint32_t                  attr_index,
                                     _Inout_ vendor_cache_t        *cache,
                                     void                          *arg)
{
    STUB_LOG_ENTER();

    value->booldata = true;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Oper state [sai_switch_oper_status_t] */
sai_status_t stub_switch_oper_status_get(_In_ const sai_object_key_t   *key,
                                         _Inout_ sai_attribute_value_t *value,
                                         _In_ uint32_t                  attr_index,
                                         _Inout_ vendor_cache_t        *cache,
                                         void                          *arg)
{
    STUB_LOG_ENTER();

    value->s32 = SAI_SWITCH_OPER_STATUS_UP;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* minimum priority for ACL table [sai_uint32_t] */
sai_status_t stub_switch_acl_table_min_prio_get(_In_ const sai_object_key_t   *key,
                                                _Inout_ sai_attribute_value_t *value,
                                                _In_ uint32_t                  attr_index,
                                                _Inout_ vendor_cache_t        *cache,
                                                void                          *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* maximum priority for ACL table [sai_uint32_t] */
sai_status_t stub_switch_acl_table_max_prio_get(_In_ const sai_object_key_t   *key,
                                                _Inout_ sai_attribute_value_t *value,
                                                _In_ uint32_t                  attr_index,
                                                _Inout_ vendor_cache_t        *cache,
                                                void                          *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* minimum priority for ACL entry [sai_uint32_t] */
sai_status_t stub_switch_acl_entry_min_prio_get(_In_ const sai_object_key_t   *key,
                                                _Inout_ sai_attribute_value_t *value,
                                                _In_ uint32_t                  attr_index,
                                                _Inout_ vendor_cache_t        *cache,
                                                void                          *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* maximum priority for ACL entry [sai_uint32_t] */
sai_status_t stub_switch_acl_entry_max_prio_get(_In_ const sai_object_key_t   *key,
                                                _Inout_ sai_attribute_value_t *value,
                                                _In_ uint32_t                  attr_index,
                                                _Inout_ vendor_cache_t        *cache,
                                                void                          *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Switching mode [sai_switch_switching_mode_t]
 *  (default to SAI_SWITCHING_MODE_STORE_AND_FORWARD) */
sai_status_t stub_switch_mode_get(_In_ const sai_object_key_t   *key,
                                  _Inout_ sai_attribute_value_t *value,
                                  _In_ uint32_t                  attr_index,
                                  _Inout_ vendor_cache_t        *cache,
                                  void                          *arg)
{
    STUB_LOG_ENTER();

    value->s32 = SAI_SWITCHING_MODE_CUT_THROUGH;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default VlanID for ports that are not members of
*  any vlans [sai_vlan_id_t]  (default to vlan 1)*/
sai_status_t stub_switch_default_port_vlan_get(_In_ const sai_object_key_t   *key,
                                               _Inout_ sai_attribute_value_t *value,
                                               _In_ uint32_t                  attr_index,
                                               _Inout_ vendor_cache_t        *cache,
                                               void                          *arg)
{
    STUB_LOG_ENTER();

    value->u16 = 1;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default switch MAC Address [sai_mac_t] */
sai_status_t stub_switch_src_mac_get(_In_ const sai_object_key_t   *key,
                                     _Inout_ sai_attribute_value_t *value,
                                     _In_ uint32_t                  attr_index,
                                     _Inout_ vendor_cache_t        *cache,
                                     void                          *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Dynamic FDB entry aging time in seconds [uint32_t]
 *   Zero means aging is disabled.
 *  (default to zero)
 */
sai_status_t stub_switch_aging_time_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg)
{
    STUB_LOG_ENTER();

    value->u32 = 0;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* ECMP hashing seed  [uint32_t] */
sai_status_t stub_switch_ecmp_hash_seed_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg)
{
    STUB_LOG_ENTER();

    value->u32 = 0;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Hash algorithm for all ECMP in the switch[sai_switch_hash_algo_t] */
sai_status_t stub_switch_ecmp_hash_algo_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg)
{
    STUB_LOG_ENTER();

    value->s32 = SAI_HASH_ALGORITHM_XOR;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Hash fields for all ECMP in the switch [sai_s32_list_t] */
sai_status_t stub_switch_ecmp_hash_fields_get(_In_ const sai_object_key_t   *key,
                                              _Inout_ sai_attribute_value_t *value,
                                              _In_ uint32_t                  attr_index,
                                              _Inout_ vendor_cache_t        *cache,
                                              void                          *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* The SDK can
 * 1 - Read the counters directly from HW (or)
 * 2 - Cache the counters in SW. Caching is typically done if
 * retrieval of counters directly from HW for each counter
 * read is CPU intensive
 * This setting can be used to
 * 1 - Move from HW based to SW based or Vice versa
 * 2 - Configure the SW counter cache refresh rate
 * Setting a value of 0 enables direct HW based counter read. A
 * non zero value enables the SW cache based and the counter
 * refresh rate.
 * A NPU may support both or one of the option. It would return
 * error for unsupported options. [uint32_t]
 */
sai_status_t stub_switch_counter_refresh_get(_In_ const sai_object_key_t   *key,
                                             _Inout_ sai_attribute_value_t *value,
                                             _In_ uint32_t                  attr_index,
                                             _Inout_ vendor_cache_t        *cache,
                                             void                          *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default trap channel [sai_hostif_trap_channel_t] */
sai_status_t stub_switch_default_trap_channel_get(_In_ const sai_object_key_t   *key,
                                                  _Inout_ sai_attribute_value_t *value,
                                                  _In_ uint32_t                  attr_index,
                                                  _Inout_ vendor_cache_t        *cache,
                                                  void                          *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default file descriptor for SAI_HOSTIF_TRAP_CHANNEL_FD [sai_object_id_t] */
sai_status_t stub_switch_default_trap_channel_fd_get(_In_ const sai_object_key_t   *key,
                                                     _Inout_ sai_attribute_value_t *value,
                                                     _In_ uint32_t                  attr_index,
                                                     _Inout_ vendor_cache_t        *cache,
                                                     void                          *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default trap group [sai_object_id_t] */
sai_status_t stub_switch_default_trap_group_get(_In_ const sai_object_key_t   *key,
                                                _Inout_ sai_attribute_value_t *value,
                                                _In_ uint32_t                  attr_index,
                                                _Inout_ vendor_cache_t        *cache,
                                                void                          *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

const sai_switch_api_t switch_api = {
    stub_initialize_switch,
    stub_shutdown_switch,
    stub_connect_switch,
    stub_disconnect_switch,
    stub_set_switch_attribute,
    stub_get_switch_attribute,
};
