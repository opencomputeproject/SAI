/**
 * Copyright (c) 2025 Microsoft Open Technologies, Inc.
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
 * @file    saitamcustom.h
 *
 * @brief   This module defines SAI TAM custom interface
 */

#if !defined (__SAITAM_CUSTOM_H_)
#define __SAITAM_CUSTOM_H_

#include <saitam.h>

/**
 * @brief Custom Attribute Id for TAM
 *
 * @flags free
 */
typedef enum _sai_tam_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_TAM_ATTR_CUSTOM_RANGE_START = SAI_TAM_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of TAM attributes
     */
    SAI_TAM_ATTR_CUSTOM_RANGE_END

} sai_tam_attr_custom_t;

/**
 * @brief Custom Attribute Id for TAM Math Function
 *
 * @flags free
 */
typedef enum _sai_tam_math_func_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_TAM_MATH_FUNC_ATTR_CUSTOM_RANGE_START = SAI_TAM_MATH_FUNC_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of TAM Math Function attributes
     */
    SAI_TAM_MATH_FUNC_ATTR_CUSTOM_RANGE_END

} sai_tam_math_func_attr_custom_t;

/**
 * @brief Custom Attribute Id for TAM Event Threshold
 *
 * @flags free
 */
typedef enum _sai_tam_event_threshold_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_CUSTOM_RANGE_START = SAI_TAM_EVENT_THRESHOLD_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of TAM Event Threshold attributes
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_CUSTOM_RANGE_END

} sai_tam_event_threshold_attr_custom_t;

/**
 * @brief Custom Attribute Id for TAM INT
 *
 * @flags free
 */
typedef enum _sai_tam_int_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_TAM_INT_ATTR_CUSTOM_RANGE_START = SAI_TAM_INT_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of TAM INT attributes
     */
    SAI_TAM_INT_ATTR_CUSTOM_RANGE_END

} sai_tam_int_attr_custom_t;

/**
 * @brief Custom Attribute Id for TAM Telemetry Type
 *
 * @flags free
 */
typedef enum _sai_tam_tel_type_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_TAM_TEL_TYPE_ATTR_CUSTOM_RANGE_START = SAI_TAM_TEL_TYPE_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of TAM Telemetry Type attributes
     */
    SAI_TAM_TEL_TYPE_ATTR_CUSTOM_RANGE_END

} sai_tam_tel_type_attr_custom_t;

/**
 * @brief Custom Attribute Id for TAM Report
 *
 * @flags free
 */
typedef enum _sai_tam_report_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_TAM_REPORT_ATTR_CUSTOM_RANGE_START = SAI_TAM_REPORT_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of TAM Report attributes
     */
    SAI_TAM_REPORT_ATTR_CUSTOM_RANGE_END

} sai_tam_report_attr_custom_t;

/**
 * @brief Custom Attribute Id for TAM Telemetry
 *
 * @flags free
 */
typedef enum _sai_tam_telemetry_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_TAM_TELEMETRY_ATTR_CUSTOM_RANGE_START = SAI_TAM_TELEMETRY_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of TAM Telemetry attributes
     */
    SAI_TAM_TELEMETRY_ATTR_CUSTOM_RANGE_END

} sai_tam_telemetry_attr_custom_t;

/**
 * @brief Custom Attribute Id for TAM Transport
 *
 * @flags free
 */
typedef enum _sai_tam_transport_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_TAM_TRANSPORT_ATTR_CUSTOM_RANGE_START = SAI_TAM_TRANSPORT_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of TAM Transport attributes
     */
    SAI_TAM_TRANSPORT_ATTR_CUSTOM_RANGE_END

} sai_tam_transport_attr_custom_t;

/**
 * @brief Custom Attribute Id for TAM Collector
 *
 * @flags free
 */
typedef enum _sai_tam_collector_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_TAM_COLLECTOR_ATTR_CUSTOM_RANGE_START = SAI_TAM_COLLECTOR_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of TAM Collector attributes
     */
    SAI_TAM_COLLECTOR_ATTR_CUSTOM_RANGE_END

} sai_tam_collector_attr_custom_t;

/**
 * @brief Custom Attribute Id for TAM Event Action
 *
 * @flags free
 */
typedef enum _sai_tam_event_action_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_TAM_EVENT_ACTION_ATTR_CUSTOM_RANGE_START = SAI_TAM_EVENT_ACTION_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of TAM Event Action attributes
     */
    SAI_TAM_EVENT_ACTION_ATTR_CUSTOM_RANGE_END

} sai_tam_event_action_attr_custom_t;

/**
 * @brief Custom Attribute Id for TAM Event
 *
 * @flags free
 */
typedef enum _sai_tam_event_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_TAM_EVENT_ATTR_CUSTOM_RANGE_START = SAI_TAM_EVENT_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of TAM Event attributes
     */
    SAI_TAM_EVENT_ATTR_CUSTOM_RANGE_END

} sai_tam_event_attr_custom_t;

/**
 * @brief Custom Attribute Id for TAM Counter Subscription
 *
 * @flags free
 */
typedef enum _sai_tam_counter_subscription_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_CUSTOM_RANGE_START = SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of TAM Counter Subscription attributes
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_CUSTOM_RANGE_END

} sai_tam_counter_subscription_attr_custom_t;

#endif /* __SAITAM_CUSTOM_H_ */