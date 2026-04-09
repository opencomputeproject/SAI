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
 * @file    saiperfmon.h
 *
 * @brief   This module defines SAI Performance Monitoring spec
 */

#if !defined (__SAIPERFMON_H_)
#define __SAIPERFMON_H_

#include <saitypes.h>

/**
 * @defgroup SAIPERFMON SAI - Performance Monitoring specific API definitions
 *
 * @{
 */

/**
 * @brief Performance Monitoring Metrics
 */
typedef enum _sai_perfmon_metrics_t
{
    /**
     * @brief None
     */
    SAI_PERFMON_METRICS_NONE,

    /**
     * @brief Maximum latency observed
     */
    SAI_PERFMON_METRICS_MAX_LATENCY,

    /**
     * @brief Average latency observed
     */
    SAI_PERFMON_METRICS_AVERAGE_LATENCY,

    /**
     * @brief Instantaneous latency observed
     */
    SAI_PERFMON_METRICS_INST_LATENCY,

} sai_perfmon_metrics_t;

/**
 * @brief Performance Monitoring Attributes
 */
typedef enum _sai_perfmon_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_PERFMON_ATTR_START,

    /**
     * @brief Object to be monitored
     *
     * @type sai_object_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_PERFMON_ATTR_OBJECT_TYPE = SAI_PERFMON_ATTR_START,

    /**
     * @brief API to be monitored
     *
     * @type sai_common_api_t
     * @flags CREATE_AND_SET
     * @default SAI_COMMON_API_CREATE
     */
    SAI_PERFMON_ATTR_COMMON_API,

    /**
     * @brief Performance metrics to be collected
     *
     * @type sai_perfmon_metrics_t
     * @flags CREATE_AND_SET
     * @default SAI_PERFMON_METRICS_NONE
     */
    SAI_PERFMON_ATTR_PERFMON_METRICS,

    /**
     * @brief Time interval in milliseconds for metrics computation
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1024
     */
    SAI_PERFMON_ATTR_METRICS_TIME_INTERVAL,

    /**
     * @brief Performance data as collected
     *
     * @type sai_perfdata_t
     * @flags READ_ONLY
     */
    SAI_PERFMON_ATTR_PERFDATA,

    /**
     * @brief End of Performance Monitoring attributes
     */
    SAI_PERFMON_ATTR_END,

    /** Custom range base value */
    SAI_PERFMON_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PERFMON_ATTR_CUSTOM_RANGE_END

} sai_perfmon_attr_t;

/**
 * @brief Create performance monitoring object
 *
 * @param[out] perfmon_id Performance Monitoring id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_perfmon_fn)(
        _Out_ sai_object_id_t *perfmon_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove performance monitoring object
 *
 * @param[in] perfmon_id Performance monitoring id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_perfmon_fn)(
        _In_ sai_object_id_t perfmon_id);

/**
 * @brief Set performance monitoring attribute
 *
 * @param[in] perfmon_id Performance monitoring id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_perfmon_attribute_fn)(
        _In_ sai_object_id_t perfmon_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Performance Monitoring attribute
 *
 * @param[in] perfmon_id Performance monitoring ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_perfmon_attribute_fn)(
        _In_ sai_object_id_t perfmon_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Performance Monitoring API methods table retrieved with sai_api_query()
 */
typedef struct _sai_perfmon_api_t
{
    /**
     * @brief SAI Performance Monitoring API set
     */
    sai_create_perfmon_fn                   create_perfmon;
    sai_remove_perfmon_fn                   remove_perfmon;
    sai_set_perfmon_attribute_fn            set_perfmon_attribute;
    sai_get_perfmon_attribute_fn            get_perfmon_attribute;
} sai_perfmon_api_t;

/**
 * @}
 */
#endif /** __SAIPERFMON_H_ */
