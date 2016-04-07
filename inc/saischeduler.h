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
* @file saischeduler.h
*
* @brief This file contains Qos Scheduler functionality.
************************************************************************/

#if !defined (__SAISCHEDULER_H_)
#define __SAISCHEDULER_H_

#include "saitypes.h"

/** \defgroup SAISCHEDULER SAI - Qos scheduler specific API definitions.
 *
 *  \{
 */

/**
 * @brief Enum defining scheduling algorithm.
 */
typedef enum _sai_scheduling_type_t
{
    /** Strict Scheduling */
    SAI_SCHEDULING_STRICT = 0x00000000,

    /** Weighted Round-Robin Scheduling */
    SAI_SCHEDULING_WRR = 0x00000001,

    /** Deficit Weighted Round-Robin Scheduling */
    SAI_SCHEDULING_DWRR = 0x00000002,

} sai_scheduling_type_t;

/**
 * @brief Enum defining scheduler attributes.
 */
typedef enum _sai_scheduler_attr_t
{
    /** READ-ONLY */

    /** READ-WRITE */

    /** Scheduling algorithm [sai_scheduling_type_t ], Default WRR*/
    SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM = 0x00000000,

    /** [uint8_t] scheduling algorithm weight, Range [1 - 100]
        Valid SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM = SAI_SCHEDULING_DWRR,
        Default Weight = 1 */
    SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT = 0x00000001,

    /** [sai_meter_type_t], Default bytes/sec */
    SAI_SCHEDULER_ATTR_SHAPER_TYPE = 0x00000002,

    /** [uint64_t] Guaranteed Bandwidth shape rate [bytes/sec or PPS]
        Value 0 to No Limit, Default 0 */
    SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE = 0x00000003,

    /** [uint64_t] Guaranteed Burst for Bandwidth shape rate [Bytes or Packets]
     */
    SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE = 0x00000004,

    /** [uint64_t] Maximum Bandwidth shape rate [bytes/sec or PPS]
        Value 0 to No limit, Default 0 */
    SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE = 0x00000005,

    /** [uint64_t] Maximum Burst for Bandwidth shape rate [bytes or Packets]
     */
    SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE = 0x00000006,

    /* -- */
    /* Custom range base value */
    SAI_SCHEDULER_ATTR_CUSTOM_RANGE_BASE  = 0x10000000
} sai_scheduler_attr_t;


/**
 * @brief  Create Scheduler Profile
 *
 * @param[out] scheduler_id Scheduler id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
typedef sai_status_t (*sai_create_scheduler_fn)(
    _Out_ sai_object_id_t  *scheduler_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * @brief  Remove Scheduler profile
 *
 * @param[in] scheduler_id Scheduler id
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
typedef sai_status_t (*sai_remove_scheduler_fn)(
    _In_ sai_object_id_t scheduler_id
    );


/**
 * @brief  Set Scheduler Attribute
 *
 * @param[in] scheduler_id Scheduler id
 * @param[in] attr attribute to set
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
typedef sai_status_t (*sai_set_scheduler_attribute_fn)(
    _In_ sai_object_id_t scheduler_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief  Get Scheduler attribute
 *
 * @param[in] scheduler_id - scheduler id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success
 *        Failure status code on error
 */

typedef sai_status_t (*sai_get_scheduler_attribute_fn)(
    _In_ sai_object_id_t scheduler_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );


/**
 * @brief  Scheduler methods table retrieved with sai_api_query()
 */
typedef struct _sai_scheduler_api_t
{
    sai_create_scheduler_fn        create_scheduler_profile;
    sai_remove_scheduler_fn        remove_scheduler_profile;
    sai_set_scheduler_attribute_fn set_scheduler_attribute;
    sai_get_scheduler_attribute_fn get_scheduler_attribute;
} sai_scheduler_api_t;


/**
 * \}
 */

#endif
