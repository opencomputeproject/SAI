/*
* Copyright (c) 2015 Dell Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License"); you may
* not use this file except in compliance with the License. You may obtain
* a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
* THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
* CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
* LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
* FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
*
* See the Apache Version 2.0 License for specific language governing
* permissions and limitations under the License.
*
* @file saiqueue.h
*
* @brief This file contains Qos Queue functionality.
************************************************************************/


#if !defined (__SAIQUEUE_H_)
#define __SAIQUEUE_H_

#include "saitypes.h"

/** \defgroup SAIQUEUE SAI - Qos Queue specific API definitions.
 *
 *  \{
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

    /* -- */
    /* Custom range base value */
    SAI_QUEUE_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_queue_type_t;

/**
 * @brief Enum defining queue attributes.
 */
typedef enum _sai_queue_attr_t
{
    /** READ-ONLY */
    /** Queue type [sai_queue_type_t] */
    SAI_QUEUE_ATTR_TYPE = 0x00000000,

    /* READ-WRITE */

    /** Attach WRED ID to queue [sai_object_id_t]
        ID = SAI_NULL_OBJECT_ID to disable WRED. */
    SAI_QUEUE_ATTR_WRED_PROFILE_ID = 0x00000001,

    /** Attach buffer profile to Queue [sai_object_id_t] */
    SAI_QUEUE_ATTR_BUFFER_PROFILE_ID = 0x00000002,

    /** Attach scheduler to Queue [sai_object_id_t]*/
    SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID = 0x00000003,

    /* -- */
    /* Custom range base value */
    SAI_QUEUE_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_queue_attr_t;



/**
 * @brief Enum defining statistics for Queue.
 */

typedef enum _sai_queue_stat_counter_t
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

    /**  get/set green color dropped packets count [uint64_t] */
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

    /* -- */
    /* Custom range base value */
    SAI_QUEUE_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_queue_stat_counter_t;


/**
 * @brief Set attribute to Queue
 * @param[in] queue_id queue id to set the attribute
 * @param[in] attr attribute to set
 *
 * @return  SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t (*sai_set_queue_attribute_fn)(
    _In_ sai_object_id_t queue_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief Get attribute to Queue
 * @param[in] queue_id queue id to set the attribute
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return  SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t (*sai_get_queue_attribute_fn)(
    _In_ sai_object_id_t queue_id,
    _In_ uint32_t        attr_count,
    _Inout_ sai_attribute_t *attr_list
    );


/**
 * @brief   Get queue statistics counters.
 *
 * @param[in] queue_id Queue id
 * @param[in] counter_ids specifies the array of counter ids
 * @param[in] number_of_counters number of counters in the array
 * @param[out] counters array of resulting counter values.
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_get_queue_stats_fn)(
    _In_ sai_object_id_t queue_id,
    _In_ const sai_queue_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters,
    _Out_ uint64_t* counters
    );

/**
 *  @brief Qos methods table retrieved with sai_api_query()
 */
typedef struct _sai_queue_api_t
{
    sai_set_queue_attribute_fn   set_queue_attribute;
    sai_get_queue_attribute_fn   get_queue_attribute;
    sai_get_queue_stats_fn       get_queue_stats;

} sai_queue_api_t;

/**
 *\}
 */

#endif

