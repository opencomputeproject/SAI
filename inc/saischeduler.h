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
 * @file    saischeduler.h
 *
 * @brief   This module defines SAI QOS Scheduler interface
 */

#if !defined (__SAISCHEDULER_H_)
#define __SAISCHEDULER_H_

#include <saitypes.h>

/**
 * @defgroup SAISCHEDULER SAI - Qos scheduler specific API definitions
 *
 * @{
 */

/**
 * @brief Enum defining scheduling algorithm.
 */
typedef enum _sai_scheduling_type_t
{
    /** Strict Scheduling */
    SAI_SCHEDULING_TYPE_STRICT = 0x00000000,

    /** Weighted Round-Robin Scheduling */
    SAI_SCHEDULING_TYPE_WRR = 0x00000001,

    /** Deficit Weighted Round-Robin Scheduling */
    SAI_SCHEDULING_TYPE_DWRR = 0x00000002,

} sai_scheduling_type_t;

/**
 * @brief Enum defining scheduler attributes.
 */
typedef enum _sai_scheduler_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SCHEDULER_ATTR_START = 0x00000000,

    /**
     * @brief Scheduling algorithm
     *
     * @type sai_scheduling_type_t
     * @flags CREATE_AND_SET
     * @default SAI_SCHEDULING_TYPE_WRR
     */
    SAI_SCHEDULER_ATTR_SCHEDULING_TYPE = SAI_SCHEDULER_ATTR_START,

    /**
     * @brief Scheduling algorithm weight
     *
     * Range [1 - 100]
     * Valid when #SAI_SCHEDULER_ATTR_SCHEDULING_TYPE = #SAI_SCHEDULING_TYPE_DWRR
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 1
     */
    SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT = 0x00000001,

    /**
     * @brief Sharper
     *
     * @type sai_meter_type_t
     * @flags CREATE_AND_SET
     * @default SAI_METER_TYPE_BYTES
     */
    SAI_SCHEDULER_ATTR_METER_TYPE = 0x00000002,

    /**
     * @brief Guaranteed Bandwidth shape rate [bytes/sec or PPS]
     *
     * Value 0 to No Limit
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE = 0x00000003,

    /**
     * @brief Guaranteed Burst for Bandwidth shape rate [Bytes or Packets]
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE = 0x00000004,

    /**
     * @brief Maximum Bandwidth shape rate [bytes/sec or PPS]
     *
     * Value 0 to No limit
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE = 0x00000005,

    /**
     * @brief Maximum Burst for Bandwidth shape rate [bytes or Packets]
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE = 0x00000006,

    /**
     * @brief End of attributes
     */
    SAI_SCHEDULER_ATTR_END,

    /** Custom range base value */
    SAI_SCHEDULER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SCHEDULER_ATTR_CUSTOM_RANGE_END

} sai_scheduler_attr_t;

/**
 * @brief Create Scheduler Profile
 *
 * @param[out] scheduler_id Scheduler id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_scheduler_fn)(
        _Out_ sai_object_id_t *scheduler_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Scheduler profile
 *
 * @param[in] scheduler_id Scheduler id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_scheduler_fn)(
        _In_ sai_object_id_t scheduler_id);

/**
 * @brief Set Scheduler Attribute
 *
 * @param[in] scheduler_id Scheduler id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_scheduler_attribute_fn)(
        _In_ sai_object_id_t scheduler_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Scheduler attribute
 *
 * @param[in] scheduler_id Scheduler id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_scheduler_attribute_fn)(
        _In_ sai_object_id_t scheduler_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Scheduler methods table retrieved with sai_api_query()
 */
typedef struct _sai_scheduler_api_t
{
    sai_create_scheduler_fn        create_scheduler_profile;
    sai_remove_scheduler_fn        remove_scheduler_profile;
    sai_set_scheduler_attribute_fn set_scheduler_attribute;
    sai_get_scheduler_attribute_fn get_scheduler_attribute;

} sai_scheduler_api_t;

/**
 * @}
 */
#endif /** __SAISCHEDULER_H_ */
