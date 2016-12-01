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
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc
 *
 * @file    saiqueue.h
 *
 * @brief   This module defines SAI QOS Queue interface
 */

#if !defined (__SAIQUEUE_H_)
#define __SAIQUEUE_H_

#include <saitypes.h>

/**
 * @defgroup SAIQUEUE SAI - Qos Queue specific API definitions.
 *
 * @{
 */

/**
 * @brief Enum defining Queue types.
 */
typedef enum _sai_queue_type_t
{
    /** H/w Queue for all types of traffic */
    SAI_QUEUE_TYPE_ALL = 0x00000000,

    /** H/w Unicast Queue */
    SAI_QUEUE_TYPE_UNICAST = 0x00000001,

    /** H/w Multicast (Broadcast, Unknown unicast, Multicast) Queue */
    SAI_QUEUE_TYPE_MULTICAST = 0x00000002,

    /** Custom range base value */
    SAI_QUEUE_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_queue_type_t;

/**
 * @brief Enum defining queue attributes.
 */
typedef enum _sai_queue_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_QUEUE_ATTR_START = 0x00000000,

    /* READ-ONLY */

    /**
     * @brief Queue type
     *
     * @type sai_queue_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     */
    SAI_QUEUE_ATTR_TYPE = SAI_QUEUE_ATTR_START,

    /**
     * @brief Pord id
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_PORT
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     */
    SAI_QUEUE_ATTR_PORT = 0x00000001,

    /**
     * @brief Queue index
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     */
    SAI_QUEUE_ATTR_INDEX = 0x00000002,

    /**
     * @brief Parent scheduler node
     *
     * In case of Hierarchical Qos not supported, the parent node is the port.
     * Condition on whether Hierarchial Qos is supported or not, need to remove
     * the MANDATORY_ON_CREATE FLAG when HQoS is introduced
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_SCHEDULER_GROUP, SAI_OBJECT_TYPE_PORT
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_QUEUE_ATTR_PARENT_SCHEDULER_NODE = 0x00000003,

    /* READ-WRITE */

    /**
     * @brief Attach WRED ID to queue
     *
     * ID = #SAI_NULL_OBJECT_ID to disable WRED
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_WRED
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_QUEUE_ATTR_WRED_PROFILE_ID = 0x00000004,

    /**
     * @brief Attach buffer profile to Queue
     * Default no profile
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_BUFFER_PROFILE
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_QUEUE_ATTR_BUFFER_PROFILE_ID = 0x00000005,

    /**
     * @brief Attach scheduler to Queue
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_SCHEDULER
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID = 0x00000006,

    /**
     * @brief End of attributes
     */
    SAI_QUEUE_ATTR_END,

    /** Custom range base value */
    SAI_QUEUE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_QUEUE_ATTR_CUSTOM_RANGE_END

} sai_queue_attr_t;

/**
 * @brief Enum defining statistics for Queue.
 */
typedef enum _sai_queue_stat_t
{
    /** get/set tx packets count [uint64_t] */
    SAI_QUEUE_STAT_PACKETS = 0x00000000,

    /** get/set tx bytes count [uint64_t] */
    SAI_QUEUE_STAT_BYTES = 0x00000001,

    /** get/set dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_DROPPED_PACKETS = 0x00000002,

    /** get/set dropped bytes count [uint64_t] */
    SAI_QUEUE_STAT_DROPPED_BYTES = 0x00000003,

    /** get/set green color tx packets count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_PACKETS = 0x00000004,

    /** get/set green color tx bytes count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_BYTES = 0x00000005,

    /** get/set green color dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_DROPPED_PACKETS = 0x00000006,

    /** get/set green color dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_DROPPED_BYTES = 0x00000007,

    /** get/set yellow color tx packets count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_PACKETS = 0x00000008,

    /** get/set yellow color tx bytes count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_BYTES = 0x00000009,

    /** get/set yellow color drooped packets count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_DROPPED_PACKETS = 0x0000000a,

    /** get/set yellow color dropped bytes count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_DROPPED_BYTES = 0x0000000b,

    /** get/set red color tx packets count [uint64_t] */
    SAI_QUEUE_STAT_RED_PACKETS = 0x0000000c,

    /** get/set red color tx bytes count [uint64_t] */
    SAI_QUEUE_STAT_RED_BYTES = 0x0000000d,

    /** get/set red color dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_RED_DROPPED_PACKETS = 0x0000000e,

    /** get/set red color drooped bytes count [uint64_t] */
    SAI_QUEUE_STAT_RED_DROPPED_BYTES = 0x0000000f,

    /** get/set WRED green color dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_DISCARD_DROPPED_PACKETS = 0x00000010,

    /** get/set WRED green color dropped bytes count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_DISCARD_DROPPED_BYTES = 0x00000011,

    /** get/set WRED yellow color dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_DISCARD_DROPPED_PACKETS = 0x00000012,

    /** get/set WRED yellow color dropped bytes count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_DISCARD_DROPPED_BYTES = 0x00000013,

    /** get/set WRED red color dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_RED_DISCARD_DROPPED_PACKETS = 0x00000014,

    /** get/set WRED red color dropped bytes count [uint64_t] */
    SAI_QUEUE_STAT_RED_DISCARD_DROPPED_BYTES = 0x00000015,

    /** get/set WRED dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_DISCARD_DROPPED_PACKETS = 0x00000016,

    /** get/set WRED red dropped bytes count [uint64_t] */
    SAI_QUEUE_STAT_DISCARD_DROPPED_BYTES = 0x00000017,

    /** get current queue occupancy in bytes [uint64_t] */
    SAI_QUEUE_STAT_CURR_OCCUPANCY_BYTES = 0x00000018,

    /** get watermark queue occupancy in bytes [uint64_t] */
    SAI_QUEUE_STAT_WATERMARK_BYTES = 0x00000019,

    /** get current queue shared occupancy in bytes [uint64_t] */
    SAI_QUEUE_STAT_SHARED_CURR_OCCUPANCY_BYTES = 0x0000001a,

    /** get watermark queue shared occupancy in bytes [uint64_t] */
    SAI_QUEUE_STAT_SHARED_WATERMARK_BYTES = 0x0000001b,

    /** Custom range base value */
    SAI_QUEUE_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_queue_stat_t;

/**
 * @brief Create queue
 *
 * @param[out] queue_id Queue id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_queue_fn)(
        _Out_ sai_object_id_t *queue_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove queue
 *
 * @param[in] queue_id Queue id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_queue_fn)(
        _In_ sai_object_id_t queue_id);

/**
 * @brief Set attribute to Queue
 *
 * @param[in] queue_id queue id to set the attribute
 * @param[in] attr attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_queue_attribute_fn)(
        _In_ sai_object_id_t queue_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute to Queue
 *
 * @param[in] queue_id Queue id to set the attribute
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_queue_attribute_fn)(
        _In_ sai_object_id_t queue_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list
        );

/**
 * @brief Get queue statistics counters.
 *
 * @param[in] queue_id Queue id
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] number_of_counters Number of counters in the array
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_queue_stats_fn)(
        _In_ sai_object_id_t queue_id,
        _In_ const sai_queue_stat_t *counter_ids,
        _In_ uint32_t number_of_counters,
        _Out_ uint64_t *counters);

/**
 * @brief Clear queue statistics counters.
 *
 * @param[in] queue_id Queue id
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] number_of_counters Number of counters in the array
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_clear_queue_stats_fn)(
        _In_ sai_object_id_t queue_id,
        _In_ const sai_queue_stat_t *counter_ids,
        _In_ uint32_t number_of_counters);

/**
 * @brief Qos methods table retrieved with sai_api_query()
 */
typedef struct _sai_queue_api_t
{
    sai_create_queue_fn          create_queue;
    sai_remove_queue_fn          remove_queue;
    sai_set_queue_attribute_fn   set_queue_attribute;
    sai_get_queue_attribute_fn   get_queue_attribute;
    sai_get_queue_stats_fn       get_queue_stats;
    sai_clear_queue_stats_fn     clear_queue_stats;

} sai_queue_api_t;

/**
 * @}
 */
#endif /** __SAIQUEUE_H_ */
