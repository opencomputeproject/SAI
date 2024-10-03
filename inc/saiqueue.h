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
 * @file    saiqueue.h
 *
 * @brief   This module defines SAI QOS Queue interface
 */

#if !defined (__SAIQUEUE_H_)
#define __SAIQUEUE_H_

#include <saitypes.h>

/**
 * @defgroup SAIQUEUE SAI - QOS Queue specific API definitions.
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

    /** H/w Egress Unicast Queue */
    SAI_QUEUE_TYPE_UNICAST = 0x00000001,

    /** H/w Multicast Egress (Broadcast, Unknown unicast, Multicast) Queue */
    SAI_QUEUE_TYPE_MULTICAST = 0x00000002,

    /** H/w Virtual Output Queue (VOQ). This queue is ingress unicast queue */
    SAI_QUEUE_TYPE_UNICAST_VOQ = 0x00000003,

    /** H/w Virtual Output Queue (VOQ). This queue is fabric multicast queue */
    SAI_QUEUE_TYPE_MULTICAST_VOQ = 0x00000004,

    /** H/w Fabric Queue. */
    SAI_QUEUE_TYPE_FABRIC_TX = 0x00000005,

    /** Custom range base value */
    SAI_QUEUE_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_queue_type_t;

/**
 * @brief Enum defining queue PFC continuous deadlock state.
 */
typedef enum _sai_queue_pfc_continuous_deadlock_state_t
{
    /**
     * @brief PFC continuous deadlock state not paused.
     *
     * H/w queue PFC state is not paused.
     * Queue can forward packets.
     */
    SAI_QUEUE_PFC_CONTINUOUS_DEADLOCK_STATE_NOT_PAUSED = 0x00000000,

    /**
     * @brief PFC continuous deadlock state paused.
     *
     * H/w queue is paused off and has not resumed or
     * forwarded packets since the last time the
     * SAI_QUEUE_ATTR_PFC_CONTINUOUS_DEADLOCK_STATE
     * attribute for this queue was polled.
     */
    SAI_QUEUE_PFC_CONTINUOUS_DEADLOCK_STATE_PAUSED = 0x00000001,

    /**
     * @brief PFC continuous deadlock state paused, but not continuously.
     *
     * H/w queue is paused off, but was not paused
     * off for the full interval that the
     * SAI_QUEUE_ATTR_PFC_CONTINUOUS_DEADLOCK_STATE
     * attribute for this queue was last polled.
     */
    SAI_QUEUE_PFC_CONTINUOUS_DEADLOCK_STATE_PAUSED_NOT_CONTINUOUS = 0x00000002

} sai_queue_pfc_continuous_deadlock_state_t;

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
     * @brief Port id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     * @objects SAI_OBJECT_TYPE_PORT
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
     * In case of Hierarchical QOS not supported, the parent node is the port.
     * Condition on whether Hierarchical QOS is supported or not, need to remove
     * the MANDATORY_ON_CREATE FLAG when HQoS is introduced.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_SCHEDULER_GROUP, SAI_OBJECT_TYPE_PORT
     */
    SAI_QUEUE_ATTR_PARENT_SCHEDULER_NODE = 0x00000003,

    /* READ-WRITE */

    /**
     * @brief Attach WRED ID to queue
     *
     * ID = #SAI_NULL_OBJECT_ID to disable WRED
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_WRED
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_QUEUE_ATTR_WRED_PROFILE_ID = 0x00000004,

    /**
     * @brief Attach buffer profile to queue
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_BUFFER_PROFILE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_QUEUE_ATTR_BUFFER_PROFILE_ID = 0x00000005,

    /**
     * @brief Attach scheduler to queue
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_SCHEDULER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID = 0x00000006,

    /**
     * @brief Queue pause status
     *
     * This attribute represents the queue internal hardware state and is
     * updated upon receiving PFC frames. True indicates the queue is paused.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_QUEUE_ATTR_PAUSE_STATUS = 0x00000007,

    /**
     * @brief Enable PFC Deadlock Detection and Recovery (DLDR) on a lossless queue.
     *
     * A deadlock is assumed to have occurred when a queue is in a XOFF
     * state for more than a configurable (SAI_SWITCH_ATTR_PFC_TC_DLD_INTERVAL)
     * amount of time.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_QUEUE_ATTR_ENABLE_PFC_DLDR = 0x00000008,

    /**
     * @brief Start PFC deadlock recovery on a lossless queue.
     *
     * If the attribute is true, start the recovery and ignore if recovery has been started.
     * If the attribute is false, stop the recovery and ignore if recovery hasn't been started.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_QUEUE_ATTR_PFC_DLR_INIT = 0x00000009,

    /**
     * @brief Queue bind point for TAM object
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     * @default empty
     */
    SAI_QUEUE_ATTR_TAM_OBJECT,

    /**
     * @brief Control for buffered and incoming packets on a queue undergoing PFC Deadlock Recovery.
     *
     * This control applies to all packets on the queue.
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_DROP
     */
    SAI_QUEUE_ATTR_PFC_DLR_PACKET_ACTION,

    /**
     * @brief Queue PFC continuous deadlock state
     *
     * This attribute represents the queue's internal hardware PFC
     * continuous deadlock state. It is an aggregation of all HW state used
     * to determine if a queue is in PFC deadlock based on state
     * cached/maintained by the SDK. Consecutive queries of this
     * attribute provide the PFC state for the queue for the interval
     * period between the queries.
     *
     * This attribute should only be queried as part of the PFC deadlock
     * and recovery detection processing.
     *
     * @type sai_queue_pfc_continuous_deadlock_state_t
     * @flags READ_ONLY
     */
    SAI_QUEUE_ATTR_PFC_CONTINUOUS_DEADLOCK_STATE,

    /**
     * @brief Set queue statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_QUEUE_ATTR_STATS_COUNT_MODE,

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
    SAI_QUEUE_ATTR_SELECTIVE_COUNTER_LIST,

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
    /** Get/set tx packets count [uint64_t] */
    SAI_QUEUE_STAT_PACKETS = 0x00000000,

    /** Get/set tx bytes count [uint64_t] */
    SAI_QUEUE_STAT_BYTES = 0x00000001,

    /** Get/set dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_DROPPED_PACKETS = 0x00000002,

    /** Get/set dropped bytes count [uint64_t] */
    SAI_QUEUE_STAT_DROPPED_BYTES = 0x00000003,

    /** Get/set green color tx packets count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_PACKETS = 0x00000004,

    /** Get/set green color tx bytes count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_BYTES = 0x00000005,

    /** Get/set green color dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_DROPPED_PACKETS = 0x00000006,

    /** Get/set green color dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_DROPPED_BYTES = 0x00000007,

    /** Get/set yellow color tx packets count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_PACKETS = 0x00000008,

    /** Get/set yellow color tx bytes count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_BYTES = 0x00000009,

    /** Get/set yellow color drooped packets count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_DROPPED_PACKETS = 0x0000000a,

    /** Get/set yellow color dropped bytes count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_DROPPED_BYTES = 0x0000000b,

    /** Get/set red color tx packets count [uint64_t] */
    SAI_QUEUE_STAT_RED_PACKETS = 0x0000000c,

    /** Get/set red color tx bytes count [uint64_t] */
    SAI_QUEUE_STAT_RED_BYTES = 0x0000000d,

    /** Get/set red color dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_RED_DROPPED_PACKETS = 0x0000000e,

    /** Get/set red color drooped bytes count [uint64_t] */
    SAI_QUEUE_STAT_RED_DROPPED_BYTES = 0x0000000f,

    /** Get/set WRED green color dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_WRED_DROPPED_PACKETS = 0x00000010,

    /** Get/set WRED green color dropped bytes count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_WRED_DROPPED_BYTES = 0x00000011,

    /** Get/set WRED yellow color dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_WRED_DROPPED_PACKETS = 0x00000012,

    /** Get/set WRED yellow color dropped bytes count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_WRED_DROPPED_BYTES = 0x00000013,

    /** Get/set WRED red color dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_RED_WRED_DROPPED_PACKETS = 0x00000014,

    /** Get/set WRED red color dropped bytes count [uint64_t] */
    SAI_QUEUE_STAT_RED_WRED_DROPPED_BYTES = 0x00000015,

    /** Get/set WRED dropped packets count [uint64_t] */
    SAI_QUEUE_STAT_WRED_DROPPED_PACKETS = 0x00000016,

    /** Get/set WRED red dropped bytes count [uint64_t] */
    SAI_QUEUE_STAT_WRED_DROPPED_BYTES = 0x00000017,

    /** Get current queue occupancy in bytes [uint64_t] */
    SAI_QUEUE_STAT_CURR_OCCUPANCY_BYTES = 0x00000018,

    /** Get watermark queue occupancy in bytes [uint64_t] */
    SAI_QUEUE_STAT_WATERMARK_BYTES = 0x00000019,

    /** Get current queue shared occupancy in bytes [uint64_t] */
    SAI_QUEUE_STAT_SHARED_CURR_OCCUPANCY_BYTES = 0x0000001a,

    /** Get watermark queue shared occupancy in bytes [uint64_t] */
    SAI_QUEUE_STAT_SHARED_WATERMARK_BYTES = 0x0000001b,

    /** Get/set WRED green color marked packets count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_WRED_ECN_MARKED_PACKETS = 0x0000001c,

    /** Get/set WRED green color marked bytes count [uint64_t] */
    SAI_QUEUE_STAT_GREEN_WRED_ECN_MARKED_BYTES = 0x0000001d,

    /** Get/set WRED yellow color marked packets count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_WRED_ECN_MARKED_PACKETS = 0x0000001e,

    /** Get/set WRED yellow color marked bytes count [uint64_t] */
    SAI_QUEUE_STAT_YELLOW_WRED_ECN_MARKED_BYTES = 0x0000001f,

    /** Get/set WRED red color marked packets count [uint64_t] */
    SAI_QUEUE_STAT_RED_WRED_ECN_MARKED_PACKETS = 0x00000020,

    /** Get/set WRED red color marked bytes count [uint64_t] */
    SAI_QUEUE_STAT_RED_WRED_ECN_MARKED_BYTES = 0x00000021,

    /** Get/set WRED marked packets count [uint64_t] */
    SAI_QUEUE_STAT_WRED_ECN_MARKED_PACKETS = 0x00000022,

    /** Get/set WRED red marked bytes count [uint64_t] */
    SAI_QUEUE_STAT_WRED_ECN_MARKED_BYTES = 0x00000023,

    /** Get current queue occupancy percentage [uint64_t] */
    SAI_QUEUE_STAT_CURR_OCCUPANCY_LEVEL = 0x00000024,

    /** Get watermark queue occupancy percentage [uint64_t] */
    SAI_QUEUE_STAT_WATERMARK_LEVEL = 0x00000025,

    /** Get packets deleted when the credit watch dog expires for VOQ System [uint64_t] */
    SAI_QUEUE_STAT_CREDIT_WD_DELETED_PACKETS = 0x00000026,

    /** Queue delay watermark in nanoseconds [uint64_t] */
    SAI_QUEUE_STAT_DELAY_WATERMARK_NS = 0x00000027,

    /** Custom range base value */
    SAI_QUEUE_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_queue_stat_t;

/**
 * @brief Enum defining Queue deadlock event state.
 */
typedef enum _sai_queue_pfc_deadlock_event_type_t
{
    /** PFC deadlock detected */
    SAI_QUEUE_PFC_DEADLOCK_EVENT_TYPE_DETECTED,

    /** PFC deadlock recovery ended */
    SAI_QUEUE_PFC_DEADLOCK_EVENT_TYPE_RECOVERED

} sai_queue_pfc_deadlock_event_type_t;

/**
 * @brief Notification data format received from SAI queue deadlock event callback
 */
typedef struct _sai_queue_deadlock_notification_data_t
{
    /**
     * @brief Queue id
     *
     * @objects SAI_OBJECT_TYPE_QUEUE
     */
    sai_object_id_t queue_id;

    /** Deadlock event */
    sai_queue_pfc_deadlock_event_type_t event;

    /**
     * @brief Application based recovery management indicator.
     *
     * This is a return value from host adapter.
     * If set to TRUE then host application will manage deadlock recovery
     * else SAI adapter or SDK will manage deadlock recovery
     * and also generate recovery ended notification.
     * Applicable only when event is == SAI_QUEUE_PFC_DEADLOCK_EVENT_TYPE_DETECTED.
     */
    bool app_managed_recovery;

} sai_queue_deadlock_notification_data_t;

/**
 * @brief Create queue
 *
 * @param[out] queue_id Queue id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
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
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_queue_fn)(
        _In_ sai_object_id_t queue_id);

/**
 * @brief Set attribute to Queue
 *
 * @param[in] queue_id Queue ID to set the attribute
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
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
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_queue_attribute_fn)(
        _In_ sai_object_id_t queue_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get queue statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] queue_id Queue id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_queue_stats_fn)(
        _In_ sai_object_id_t queue_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get queue statistics counters extended.
 *
 * @param[in] queue_id Queue id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_queue_stats_ext_fn)(
        _In_ sai_object_id_t queue_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear queue statistics counters.
 *
 * @param[in] queue_id Queue id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_queue_stats_fn)(
        _In_ sai_object_id_t queue_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Queue PFC deadlock event notification
 *
 * Passed as a parameter into sai_initialize_switch()
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Array of queue event types
 */
typedef void (*sai_queue_pfc_deadlock_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_queue_deadlock_notification_data_t *data);

/**
 * @brief QOS methods table retrieved with sai_api_query()
 */
typedef struct _sai_queue_api_t
{
    sai_create_queue_fn          create_queue;
    sai_remove_queue_fn          remove_queue;
    sai_set_queue_attribute_fn   set_queue_attribute;
    sai_get_queue_attribute_fn   get_queue_attribute;
    sai_get_queue_stats_fn       get_queue_stats;
    sai_get_queue_stats_ext_fn   get_queue_stats_ext;
    sai_clear_queue_stats_fn     clear_queue_stats;

} sai_queue_api_t;

/**
 * @}
 */
#endif /** __SAIQUEUE_H_ */
