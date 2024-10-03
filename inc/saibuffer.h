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
 * @file    saibuffer.h
 *
 * @brief   This module defines SAI Buffer interface
 */

#if !defined (__SAIBUFFER_H_)
#define __SAIBUFFER_H_

#include <saitypes.h>

/**
 * @defgroup SAIBUFFER SAI - Buffer specific API definitions
 *
 * @{
 */

/**
 * @brief Enum defining ingress priority group attributes.
 */
typedef enum _sai_ingress_priority_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_START,

    /**
     * @brief Buffer profile pointer
     *
     * Default no profile
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_BUFFER_PROFILE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE = SAI_INGRESS_PRIORITY_GROUP_ATTR_START,

    /**
     * @brief Port id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_PORT,

    /**
     * @brief TAM id
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     * @default empty
     */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_TAM,

    /**
     * @brief PG index
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_INDEX,

    /**
     * @brief Set ingress priority group statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_SELECTIVE_COUNTER_LIST,

    /**
     * @brief End of attributes
     */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_CUSTOM_RANGE_END

} sai_ingress_priority_group_attr_t;

/**
 * @brief Enum defining statistics for ingress priority group.
 */
typedef enum _sai_ingress_priority_group_stat_t
{
    /** Get rx packets count [uint64_t] */
    SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS = 0x00000000,

    /** Get rx bytes count [uint64_t] */
    SAI_INGRESS_PRIORITY_GROUP_STAT_BYTES = 0x00000001,

    /** Get current pg occupancy in bytes [uint64_t] */
    SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES = 0x00000002,

    /** Get watermark pg occupancy in bytes [uint64_t] */
    SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES = 0x00000003,

    /** Get current pg shared occupancy in bytes [uint64_t] */
    SAI_INGRESS_PRIORITY_GROUP_STAT_SHARED_CURR_OCCUPANCY_BYTES = 0x00000004,

    /** Get watermark pg shared occupancy in bytes [uint64_t] */
    SAI_INGRESS_PRIORITY_GROUP_STAT_SHARED_WATERMARK_BYTES = 0x00000005,

    /** Get current pg XOFF room occupancy in bytes [uint64_t] */
    SAI_INGRESS_PRIORITY_GROUP_STAT_XOFF_ROOM_CURR_OCCUPANCY_BYTES = 0x00000006,

    /** Get watermark pg XOFF room occupancy in bytes [uint64_t] */
    SAI_INGRESS_PRIORITY_GROUP_STAT_XOFF_ROOM_WATERMARK_BYTES = 0x00000007,

    /** Get dropped packets count [uint64_t] */
    SAI_INGRESS_PRIORITY_GROUP_STAT_DROPPED_PACKETS = 0x00000008,

    /** Custom range base value */
    SAI_INGRESS_PRIORITY_GROUP_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_ingress_priority_group_stat_t;

/**
 * @brief Create ingress priority group
 *
 * @param[out] ingress_priority_group_id Ingress priority group
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_ingress_priority_group_fn)(
        _Out_ sai_object_id_t *ingress_priority_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove ingress priority group
 *
 * @param[in] ingress_priority_group_id Ingress priority group id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_ingress_priority_group_fn)(
        _In_ sai_object_id_t ingress_priority_group_id);

/**
 * @brief Set ingress priority group attribute
 *
 * @param[in] ingress_priority_group_id Ingress priority group id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_ingress_priority_group_attribute_fn)(
        _In_ sai_object_id_t ingress_priority_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get ingress priority group attributes
 *
 * @param[in] ingress_priority_group_id Ingress priority group id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ingress_priority_group_attribute_fn)(
        _In_ sai_object_id_t ingress_priority_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get ingress priority group statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] ingress_priority_group_id Ingress priority group id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ingress_priority_group_stats_fn)(
        _In_ sai_object_id_t ingress_priority_group_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get ingress priority group statistics counters extended.
 *
 * @param[in] ingress_priority_group_id Ingress priority group id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ingress_priority_group_stats_ext_fn)(
        _In_ sai_object_id_t ingress_priority_group_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear ingress priority group statistics counters.
 *
 * @param[in] ingress_priority_group_id Ingress priority group id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_ingress_priority_group_stats_fn)(
        _In_ sai_object_id_t ingress_priority_group_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Enum defining buffer pool types.
 */
typedef enum _sai_buffer_pool_type_t
{
    /** Ingress buffer pool */
    SAI_BUFFER_POOL_TYPE_INGRESS,

    /** Egress buffer pool */
    SAI_BUFFER_POOL_TYPE_EGRESS,

    /** Buffer pool used by both ingress and egress */
    SAI_BUFFER_POOL_TYPE_BOTH

} sai_buffer_pool_type_t;

/**
 * @brief Enum defining buffer pool threshold modes
 */
typedef enum _sai_buffer_pool_threshold_mode_t
{
    /** Static maximum */
    SAI_BUFFER_POOL_THRESHOLD_MODE_STATIC,

    /** Dynamic maximum (relative) */
    SAI_BUFFER_POOL_THRESHOLD_MODE_DYNAMIC,

} sai_buffer_pool_threshold_mode_t;

/**
 * @brief Enum defining buffer pool attributes.
 */
typedef enum _sai_buffer_pool_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_BUFFER_POOL_ATTR_START,

    /**
     * @brief Shared buffer size in bytes
     *
     * This is derived from subtracting all reversed buffers of queue/port
     * from the total pool size.
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_BUFFER_POOL_ATTR_SHARED_SIZE = SAI_BUFFER_POOL_ATTR_START,

    /**
     * @brief Buffer pool type
     *
     * @type sai_buffer_pool_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_BUFFER_POOL_ATTR_TYPE,

    /**
     * @brief Buffer pool size in bytes
     *
     * @type sai_uint64_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_BUFFER_POOL_ATTR_SIZE,

    /**
     * @brief Shared threshold mode for the buffer
     *
     * @type sai_buffer_pool_threshold_mode_t
     * @flags CREATE_ONLY
     * @default SAI_BUFFER_POOL_THRESHOLD_MODE_DYNAMIC
     */
    SAI_BUFFER_POOL_ATTR_THRESHOLD_MODE,

    /**
     * @brief TAM id
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     * @default empty
     */
    SAI_BUFFER_POOL_ATTR_TAM,

    /**
     * @brief Shared headroom pool size in bytes for lossless traffic.
     *
     * Only valid for the ingress buffer pool.
     * If shared headroom pool size is not zero, its size is included in
     * the corresponding ingress buffer pool size SAI_BUFFER_POOL_ATTR_SIZE
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_BUFFER_POOL_ATTR_XOFF_SIZE,

    /**
     * @brief Attach WRED ID to pool
     *
     * WRED Drop/ECN marking based on pool thresholds will happen only
     * when one of queue referring to this buffer pool configured
     * with non default value for SAI_QUEUE_ATTR_WRED_PROFILE_ID.
     * ID = #SAI_NULL_OBJECT_ID to disable WRED
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_WRED
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_BUFFER_POOL_ATTR_WRED_PROFILE_ID,

    /**
     * @brief Set buffer pool statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_BUFFER_POOL_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_BUFFER_POOL_ATTR_SELECTIVE_COUNTER_LIST,

    /**
     * @brief End of attributes
     */
    SAI_BUFFER_POOL_ATTR_END,

    /** Custom range base value */
    SAI_BUFFER_POOL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_BUFFER_POOL_ATTR_CUSTOM_RANGE_END

} sai_buffer_pool_attr_t;

/**
 * @brief Enum defining statistics for buffer pool.
 */
typedef enum _sai_buffer_pool_stat_t
{
    /** Get current pool occupancy in bytes [uint64_t] */
    SAI_BUFFER_POOL_STAT_CURR_OCCUPANCY_BYTES = 0x00000000,

    /** Get watermark pool occupancy in bytes [uint64_t] */
    SAI_BUFFER_POOL_STAT_WATERMARK_BYTES = 0x00000001,

    /** Get count of packets dropped in this pool [uint64_t] */
    SAI_BUFFER_POOL_STAT_DROPPED_PACKETS = 0x00000002,

    /** Get/set WRED green dropped packet count [uint64_t] */
    SAI_BUFFER_POOL_STAT_GREEN_WRED_DROPPED_PACKETS = 0x00000003,

    /** Get/set WRED green dropped byte count [uint64_t] */
    SAI_BUFFER_POOL_STAT_GREEN_WRED_DROPPED_BYTES = 0x00000004,

    /** Get/set WRED yellow dropped packet count [uint64_t] */
    SAI_BUFFER_POOL_STAT_YELLOW_WRED_DROPPED_PACKETS = 0x00000005,

    /** Get/set WRED yellow dropped byte count [uint64_t] */
    SAI_BUFFER_POOL_STAT_YELLOW_WRED_DROPPED_BYTES = 0x00000006,

    /** Get/set WRED red dropped packet count [uint64_t] */
    SAI_BUFFER_POOL_STAT_RED_WRED_DROPPED_PACKETS = 0x00000007,

    /** Get/set WRED red dropped byte count [uint64_t] */
    SAI_BUFFER_POOL_STAT_RED_WRED_DROPPED_BYTES = 0x00000008,

    /** Get/set WRED dropped packets count [uint64_t] */
    SAI_BUFFER_POOL_STAT_WRED_DROPPED_PACKETS = 0x00000009,

    /** Get/set WRED dropped bytes count [uint64_t] */
    SAI_BUFFER_POOL_STAT_WRED_DROPPED_BYTES = 0x0000000a,

    /** Get/set WRED green marked packet count [uint64_t] */
    SAI_BUFFER_POOL_STAT_GREEN_WRED_ECN_MARKED_PACKETS = 0x0000000b,

    /** Get/set WRED green marked byte count [uint64_t] */
    SAI_BUFFER_POOL_STAT_GREEN_WRED_ECN_MARKED_BYTES = 0x0000000c,

    /** Get/set WRED yellow marked packet count [uint64_t] */
    SAI_BUFFER_POOL_STAT_YELLOW_WRED_ECN_MARKED_PACKETS = 0x0000000d,

    /** Get/set WRED yellow marked byte count [uint64_t] */
    SAI_BUFFER_POOL_STAT_YELLOW_WRED_ECN_MARKED_BYTES = 0x0000000e,

    /** Get/set WRED red marked packet count [uint64_t] */
    SAI_BUFFER_POOL_STAT_RED_WRED_ECN_MARKED_PACKETS = 0x0000000f,

    /** Get/set WRED red marked byte count [uint64_t] */
    SAI_BUFFER_POOL_STAT_RED_WRED_ECN_MARKED_BYTES = 0x00000010,

    /** Get/set WRED marked packets count [uint64_t] */
    SAI_BUFFER_POOL_STAT_WRED_ECN_MARKED_PACKETS = 0x00000011,

    /** Get/set WRED marked bytes count [uint64_t] */
    SAI_BUFFER_POOL_STAT_WRED_ECN_MARKED_BYTES = 0x00000012,

    /** Get current headroom pool occupancy in bytes [uint64_t] */
    SAI_BUFFER_POOL_STAT_XOFF_ROOM_CURR_OCCUPANCY_BYTES = 0x00000013,

    /** Get headroom pool occupancy in bytes [uint64_t] */
    SAI_BUFFER_POOL_STAT_XOFF_ROOM_WATERMARK_BYTES = 0x00000014,

    /** Custom range base value */
    SAI_BUFFER_POOL_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_buffer_pool_stat_t;

/**
 * @brief Create buffer pool
 *
 * @param[out] buffer_pool_id Buffer pool id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_buffer_pool_fn)(
        _Out_ sai_object_id_t *buffer_pool_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove buffer pool
 *
 * @param[in] buffer_pool_id Buffer pool id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_buffer_pool_fn)(
        _In_ sai_object_id_t buffer_pool_id);

/**
 * @brief Set buffer pool attribute
 *
 * @param[in] buffer_pool_id Buffer pool id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_buffer_pool_attribute_fn)(
        _In_ sai_object_id_t buffer_pool_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get buffer pool attributes
 *
 * @param[in] buffer_pool_id Buffer pool id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_buffer_pool_attribute_fn)(
        _In_ sai_object_id_t buffer_pool_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get buffer pool statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] buffer_pool_id Buffer pool id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_buffer_pool_stats_fn)(
        _In_ sai_object_id_t buffer_pool_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get buffer pool statistics counters extended.
 *
 * @param[in] buffer_pool_id Buffer pool id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_buffer_pool_stats_ext_fn)(
        _In_ sai_object_id_t buffer_pool_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear buffer pool statistics counters.
 *
 * @param[in] buffer_pool_id Buffer pool id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_buffer_pool_stats_fn)(
        _In_ sai_object_id_t buffer_pool_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Enum defining buffer profile threshold modes
 */
typedef enum _sai_buffer_profile_threshold_mode_t
{
    /** Static maximum */
    SAI_BUFFER_PROFILE_THRESHOLD_MODE_STATIC,

    /** Dynamic maximum (relative) */
    SAI_BUFFER_PROFILE_THRESHOLD_MODE_DYNAMIC,

} sai_buffer_profile_threshold_mode_t;

/**
 * @brief Enum defining buffer profile attributes.
 */
typedef enum _sai_buffer_profile_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_BUFFER_PROFILE_ATTR_START,

    /**
     * @brief Pointer to buffer pool object id
     *
     * Pool id = #SAI_NULL_OBJECT_ID can be used when profile is not associated
     * with specific pool, for example for global port buffer. Not applicable
     * to priority group or queue buffer profile.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BUFFER_POOL
     * @allownull true
     */
    SAI_BUFFER_PROFILE_ATTR_POOL_ID = SAI_BUFFER_PROFILE_ATTR_START,

    /**
     * @brief Reserved buffer size in bytes
     *
     * @type sai_uint64_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_BUFFER_PROFILE_ATTR_RESERVED_BUFFER_SIZE,

    /** @ignore - for backward compatibility */
    SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE = SAI_BUFFER_PROFILE_ATTR_RESERVED_BUFFER_SIZE,

    /**
     * @brief Shared threshold mode for the buffer profile
     *
     * If set, this overrides #SAI_BUFFER_POOL_ATTR_THRESHOLD_MODE.
     *
     * @type sai_buffer_profile_threshold_mode_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_BUFFER_PROFILE_ATTR_THRESHOLD_MODE,

    /**
     * @brief Dynamic threshold for the shared usage
     *
     * The threshold is set to the 2^n of available buffer of the pool.
     *
     * @type sai_int8_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_BUFFER_PROFILE_ATTR_THRESHOLD_MODE == SAI_BUFFER_PROFILE_THRESHOLD_MODE_DYNAMIC
     */
    SAI_BUFFER_PROFILE_ATTR_SHARED_DYNAMIC_TH,

    /**
     * @brief Static threshold for the shared usage in bytes
     *
     * When set to zero there is no limit for the shared usage.
     *
     * @type sai_uint64_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_BUFFER_PROFILE_ATTR_THRESHOLD_MODE == SAI_BUFFER_PROFILE_THRESHOLD_MODE_STATIC
     */
    SAI_BUFFER_PROFILE_ATTR_SHARED_STATIC_TH,

    /**
     * @brief Set the buffer profile XOFF threshold in bytes
     *
     * Valid only for ingress PG.
     *
     * Specifies the maximum available buffer for a PG after XOFF is
     * generated (i.e. headroom buffer). Note that the available
     * headroom buffer is dependent on SAI_BUFFER_POOL_ATTR_XOFF_SIZE. If the user has
     * set SAI_BUFFER_POOL_ATTR_XOFF_SIZE = 0, the PG headroom buffer is equal to XOFF_TH
     * and it is not shared. If the user has set SAI_BUFFER_POOL_ATTR_XOFF_SIZE > 0, the
     * total headroom pool buffer for all PGs is equal to SAI_BUFFER_POOL_ATTR_XOFF_SIZE
     * and XOFF_TH specifies the maximum amount of headroom pool
     * buffer one PG can use.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_BUFFER_PROFILE_ATTR_XOFF_TH,

    /**
     * @brief Set the buffer profile XON non-hysteresis threshold in byte
     *
     * Valid only for ingress PG.
     *
     * Generate XON when the total buffer usage of this PG is less than the maximum of XON_TH
     * and the total buffer limit minus XON_OFFSET_TH, and available buffer in the PG buffer
     * is larger than the XOFF_TH.
     * The XON trigger condition is governed by:
     * total buffer usage <= max(XON_TH, total buffer limit - XON_OFFSET_TH)
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_BUFFER_PROFILE_ATTR_XON_TH,

    /**
     * @brief Set the buffer profile XON hysteresis threshold in byte
     *
     * Valid only for ingress PG
     *
     * Generate XON when the total buffer usage of this PG is less than the maximum of XON_TH
     * and the total buffer limit minus XON_OFFSET_TH, and available buffer in the PG buffer
     * is larger than the XOFF_TH.
     * The XON trigger condition is governed by:
     * total buffer usage <= max(XON_TH, total buffer limit - XON_OFFSET_TH)
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_BUFFER_PROFILE_ATTR_XON_OFFSET_TH,

    /**
     * @brief End of attributes
     */
    SAI_BUFFER_PROFILE_ATTR_END,

    /** Custom range base value */
    SAI_BUFFER_PROFILE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_BUFFER_PROFILE_ATTR_CUSTOM_RANGE_END

} sai_buffer_profile_attr_t;

/**
 * @brief Create buffer profile
 *
 * @param[out] buffer_profile_id Buffer profile id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_buffer_profile_fn)(
        _Out_ sai_object_id_t *buffer_profile_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove buffer profile
 *
 * @param[in] buffer_profile_id Buffer profile id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_buffer_profile_fn)(
        _In_ sai_object_id_t buffer_profile_id);

/**
 * @brief Set buffer profile attribute
 *
 * @param[in] buffer_profile_id Buffer profile id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_buffer_profile_attribute_fn)(
        _In_ sai_object_id_t buffer_profile_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get buffer profile attributes
 *
 * @param[in] buffer_profile_id Buffer profile id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_buffer_profile_attribute_fn)(
        _In_ sai_object_id_t buffer_profile_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Buffer methods table retrieved with sai_api_query()
 */
typedef struct _sai_buffer_api_t
{
    sai_create_buffer_pool_fn                       create_buffer_pool;
    sai_remove_buffer_pool_fn                       remove_buffer_pool;
    sai_set_buffer_pool_attribute_fn                set_buffer_pool_attribute;
    sai_get_buffer_pool_attribute_fn                get_buffer_pool_attribute;
    sai_get_buffer_pool_stats_fn                    get_buffer_pool_stats;
    sai_get_buffer_pool_stats_ext_fn                get_buffer_pool_stats_ext;
    sai_clear_buffer_pool_stats_fn                  clear_buffer_pool_stats;
    sai_create_ingress_priority_group_fn            create_ingress_priority_group;
    sai_remove_ingress_priority_group_fn            remove_ingress_priority_group;
    sai_set_ingress_priority_group_attribute_fn     set_ingress_priority_group_attribute;
    sai_get_ingress_priority_group_attribute_fn     get_ingress_priority_group_attribute;
    sai_get_ingress_priority_group_stats_fn         get_ingress_priority_group_stats;
    sai_get_ingress_priority_group_stats_ext_fn     get_ingress_priority_group_stats_ext;
    sai_clear_ingress_priority_group_stats_fn       clear_ingress_priority_group_stats;
    sai_create_buffer_profile_fn                    create_buffer_profile;
    sai_remove_buffer_profile_fn                    remove_buffer_profile;
    sai_set_buffer_profile_attribute_fn             set_buffer_profile_attribute;
    sai_get_buffer_profile_attribute_fn             get_buffer_profile_attribute;
} sai_buffer_api_t;

/**
 * @}
 */
#endif /** __SAIBUFFER_H_ */
