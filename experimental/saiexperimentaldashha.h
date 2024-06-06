/**
 * Copyright (c) 2014 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
 *
 * @file    saiexperimentaldashha.h
 *
 * @brief   This module defines SAI extensions for DASH HA
 *
 * @warning This module is a SAI experimental module
 */

#if !defined (__SAIEXPERIMENTALDASHHA_H_)
#define __SAIEXPERIMENTALDASHHA_H_

#include <saitypesextensions.h>

/**
 * @defgroup SAIEXPERIMENTALDASH_HA SAI - Experimental: DASH HA specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for dash_ha_ha_set
 */
typedef enum _sai_ha_set_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_HA_SET_ATTR_START,

    /**
     * @brief Action set_ha_set_attr parameter LOCAL_IP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_HA_SET_ATTR_LOCAL_IP = SAI_HA_SET_ATTR_START,

    /**
     * @brief Action set_ha_set_attr parameter PEER_IP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_HA_SET_ATTR_PEER_IP,

    /**
     * @brief Action set_ha_set_attr parameter CP_DATA_CHANNEL_PORT
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_HA_SET_ATTR_CP_DATA_CHANNEL_PORT,

    /**
     * @brief Action set_ha_set_attr parameter DP_CHANNEL_DST_PORT
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_HA_SET_ATTR_DP_CHANNEL_DST_PORT,

    /**
     * @brief Action set_ha_set_attr parameter DP_CHANNEL_MIN_SRC_PORT
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_HA_SET_ATTR_DP_CHANNEL_MIN_SRC_PORT,

    /**
     * @brief Action set_ha_set_attr parameter DP_CHANNEL_MAX_SRC_PORT
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_HA_SET_ATTR_DP_CHANNEL_MAX_SRC_PORT,

    /**
     * @brief Action set_ha_set_attr parameter DP_CHANNEL_PROBE_INTERVAL_MS
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_HA_SET_ATTR_DP_CHANNEL_PROBE_INTERVAL_MS,

    /**
     * @brief Action set_ha_set_attr parameter DP_CHANNEL_PROBE_FAIL_THRESHOLD
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_HA_SET_ATTR_DP_CHANNEL_PROBE_FAIL_THRESHOLD,

    /**
     * @brief Action set_ha_set_attr parameter DP_CHANNEL_IS_ALIVE
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_HA_SET_ATTR_DP_CHANNEL_IS_ALIVE,

    /**
     * @brief End of attributes
     */
    SAI_HA_SET_ATTR_END,

    /** Custom range base value */
    SAI_HA_SET_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_HA_SET_ATTR_CUSTOM_RANGE_END,

} sai_ha_set_attr_t;

/**
 * @brief Counter IDs for HA_SET in sai_get_ha_set_stats() call
 */
typedef enum _sai_ha_set_stat_t
{
    /** DASH HA_SET DP_PROBE_REQ_RX_BYTES stat count */
    SAI_HA_SET_STAT_DP_PROBE_REQ_RX_BYTES,

    /** DASH HA_SET DP_PROBE_REQ_RX_PACKETS stat count */
    SAI_HA_SET_STAT_DP_PROBE_REQ_RX_PACKETS,

    /** DASH HA_SET DP_PROBE_REQ_TX_BYTES stat count */
    SAI_HA_SET_STAT_DP_PROBE_REQ_TX_BYTES,

    /** DASH HA_SET DP_PROBE_REQ_TX_PACKETS stat count */
    SAI_HA_SET_STAT_DP_PROBE_REQ_TX_PACKETS,

    /** DASH HA_SET DP_PROBE_ACK_RX_BYTES stat count */
    SAI_HA_SET_STAT_DP_PROBE_ACK_RX_BYTES,

    /** DASH HA_SET DP_PROBE_ACK_RX_PACKETS stat count */
    SAI_HA_SET_STAT_DP_PROBE_ACK_RX_PACKETS,

    /** DASH HA_SET DP_PROBE_ACK_TX_BYTES stat count */
    SAI_HA_SET_STAT_DP_PROBE_ACK_TX_BYTES,

    /** DASH HA_SET DP_PROBE_ACK_TX_PACKETS stat count */
    SAI_HA_SET_STAT_DP_PROBE_ACK_TX_PACKETS,

    /** DASH HA_SET DP_PROBE_FAILED stat count */
    SAI_HA_SET_STAT_DP_PROBE_FAILED,

    /** DASH HA_SET CP_DATA_CHANNEL_CONNECT_ATTEMPTED stat count */
    SAI_HA_SET_STAT_CP_DATA_CHANNEL_CONNECT_ATTEMPTED,

    /** DASH HA_SET CP_DATA_CHANNEL_CONNECT_RECEIVED stat count */
    SAI_HA_SET_STAT_CP_DATA_CHANNEL_CONNECT_RECEIVED,

    /** DASH HA_SET CP_DATA_CHANNEL_CONNECT_SUCCEEDED stat count */
    SAI_HA_SET_STAT_CP_DATA_CHANNEL_CONNECT_SUCCEEDED,

    /** DASH HA_SET CP_DATA_CHANNEL_CONNECT_FAILED stat count */
    SAI_HA_SET_STAT_CP_DATA_CHANNEL_CONNECT_FAILED,

    /** DASH HA_SET CP_DATA_CHANNEL_CONNECT_REJECTED stat count */
    SAI_HA_SET_STAT_CP_DATA_CHANNEL_CONNECT_REJECTED,

    /** DASH HA_SET CP_DATA_CHANNEL_TIMEOUT_COUNT stat count */
    SAI_HA_SET_STAT_CP_DATA_CHANNEL_TIMEOUT_COUNT,

    /** DASH HA_SET BULK_SYNC_MESSAGE_RECEIVED stat count */
    SAI_HA_SET_STAT_BULK_SYNC_MESSAGE_RECEIVED,

    /** DASH HA_SET BULK_SYNC_MESSAGE_SENT stat count */
    SAI_HA_SET_STAT_BULK_SYNC_MESSAGE_SENT,

    /** DASH HA_SET BULK_SYNC_MESSAGE_SEND_FAILED stat count */
    SAI_HA_SET_STAT_BULK_SYNC_MESSAGE_SEND_FAILED,

    /** DASH HA_SET BULK_SYNC_FLOW_RECEIVED stat count */
    SAI_HA_SET_STAT_BULK_SYNC_FLOW_RECEIVED,

    /** DASH HA_SET BULK_SYNC_FLOW_SENT stat count */
    SAI_HA_SET_STAT_BULK_SYNC_FLOW_SENT,

} sai_ha_set_stat_t;

/**
 * @brief Attribute ID for dash_ha_ha_scope
 */
typedef enum _sai_ha_scope_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_HA_SCOPE_ATTR_START,

    /**
     * @brief Action set_ha_scope_attr parameter HA_SET_ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_HA_SCOPE_ATTR_HA_SET_ID = SAI_HA_SCOPE_ATTR_START,

    /**
     * @brief Action set_ha_scope_attr parameter DASH_HA_ROLE
     *
     * @type sai_dash_ha_role_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_HA_ROLE_DEAD
     */
    SAI_HA_SCOPE_ATTR_DASH_HA_ROLE,

    /**
     * @brief Action set_ha_scope_attr parameter FLOW_VERSION
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_HA_SCOPE_ATTR_FLOW_VERSION,

    /**
     * @brief Action set_ha_scope_attr parameter FLOW_RECONCILE_REQUESTED
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_HA_SCOPE_ATTR_FLOW_RECONCILE_REQUESTED,

    /**
     * @brief Action set_ha_scope_attr parameter FLOW_RECONCILE_NEEDED
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_HA_SCOPE_ATTR_FLOW_RECONCILE_NEEDED,

    /**
     * @brief End of attributes
     */
    SAI_HA_SCOPE_ATTR_END,

    /** Custom range base value */
    SAI_HA_SCOPE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_HA_SCOPE_ATTR_CUSTOM_RANGE_END,

} sai_ha_scope_attr_t;

/**
 * @brief Create dash_ha_ha_set
 *
 * @param[out] ha_set_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_ha_set_fn)(
        _Out_ sai_object_id_t *ha_set_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_ha_ha_set
 *
 * @param[in] ha_set_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_ha_set_fn)(
        _In_ sai_object_id_t ha_set_id);

/**
 * @brief Set attribute for dash_ha_ha_set
 *
 * @param[in] ha_set_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_ha_set_attribute_fn)(
        _In_ sai_object_id_t ha_set_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_ha_ha_set
 *
 * @param[in] ha_set_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_ha_set_attribute_fn)(
        _In_ sai_object_id_t ha_set_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get HA_SET statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] ha_set_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ha_set_stats_fn)(
        _In_ sai_object_id_t ha_set_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get HA_SET statistics counters extended.
 *
 * @param[in] ha_set_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ha_set_stats_ext_fn)(
        _In_ sai_object_id_t ha_set_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear HA_SET statistics counters.
 *
 * @param[in] ha_set_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_ha_set_stats_fn)(
        _In_ sai_object_id_t ha_set_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Create dash_ha_ha_scope
 *
 * @param[out] ha_scope_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_ha_scope_fn)(
        _Out_ sai_object_id_t *ha_scope_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_ha_ha_scope
 *
 * @param[in] ha_scope_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_ha_scope_fn)(
        _In_ sai_object_id_t ha_scope_id);

/**
 * @brief Set attribute for dash_ha_ha_scope
 *
 * @param[in] ha_scope_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_ha_scope_attribute_fn)(
        _In_ sai_object_id_t ha_scope_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_ha_ha_scope
 *
 * @param[in] ha_scope_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_ha_scope_attribute_fn)(
        _In_ sai_object_id_t ha_scope_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_dash_ha_api_t
{
    sai_create_ha_set_fn             create_ha_set;
    sai_remove_ha_set_fn             remove_ha_set;
    sai_set_ha_set_attribute_fn      set_ha_set_attribute;
    sai_get_ha_set_attribute_fn      get_ha_set_attribute;
    sai_get_ha_set_stats_fn          get_ha_set_stats;
    sai_get_ha_set_stats_ext_fn      get_ha_set_stats_ext;
    sai_clear_ha_set_stats_fn        clear_ha_set_stats;
    sai_bulk_object_create_fn        create_ha_sets;
    sai_bulk_object_remove_fn        remove_ha_sets;

    sai_create_ha_scope_fn           create_ha_scope;
    sai_remove_ha_scope_fn           remove_ha_scope;
    sai_set_ha_scope_attribute_fn    set_ha_scope_attribute;
    sai_get_ha_scope_attribute_fn    get_ha_scope_attribute;
    sai_bulk_object_create_fn        create_ha_scopes;
    sai_bulk_object_remove_fn        remove_ha_scopes;

} sai_dash_ha_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHHA_H_ */
