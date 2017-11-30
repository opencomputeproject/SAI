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
 *    Dell Products, L.P., Facebook, Inc
 *
 * @file    saiswitchextensions.h
 *
 * @brief       This file extends SAI Switch for data plane telemetry.
 * @description Supported by: Barefoot Networks, Inc.
 * @warning     Attributes defined in this file are experimental.
 */

#if !defined (__SAISWITCH_EXPERIMENTAL_H_)
#define __SAISWITCH_EXPERIMENTAL_H_

#include <saiswitch.h>

/**
 * @defgroup SAISWITCH EXPERIMENTAL SAI - Switch experimental API definitions
 *
 * @{
 */

typedef enum _sai_switch_experimental_attr_t
{
    /** Start of experimental types */
    SAI_SWITCH_ATTR_EXPERIMENTAL_START = SAI_SWITCH_ATTR_CUSTOM_RANGE_END + 1,

    /**
     * @brief DTEL INT endpoint
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_DTEL_INT_ENDPOINT_ENABLE,

    /**
     * @brief DTel INT transit
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_DTEL_INT_TRANSIT_ENABLE,

    /**
     * @brief Packet postcard
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_DTEL_POSTCARD_ENABLE,

    /**
     * @brief Drop Report
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_DTEL_DROP_REPORT_ENABLE,

    /**
     * @brief Queue Report
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_DTEL_QUEUE_REPORT_ENABLE,

    /**
     * @brief Globally unique switch ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_SWITCH_ATTR_DTEL_SWITCH_ID,

    /**
     * @brief DTel flow state clear cycle
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_SWITCH_ATTR_DTEL_FLOW_STATE_CLEAR_CYCLE,

    /**
     * @brief Latency sensitivity for flow state change detection
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_SWITCH_ATTR_DTEL_LATENCY_SENSITIVITY,

    /**
     * @brief DTel sink ports
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_SWITCH_ATTR_DTEL_SINK_PORT_LIST,

    /**
     * @brief Reserved DSCP value for INT over L4
     *
     * @type sai_ternary_field_t
     * @flags CREATE_AND_SET
     */
    SAI_SWITCH_ATTR_DTEL_INT_L4_DSCP

} sai_switch_experimental_attr_t;

/**
 * @}
 */
#endif /** __SAISWITCH_EXPERIMENTAL_H_ */
