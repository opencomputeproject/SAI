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
 * @file    saiaclextensions.h
 *
 * @brief       This file extends SAI ACL for data plane telemetry.
 * @description Supported by: Barefoot Networks, Inc.
 * @warning     Attributes defined in this file are experimental.
 */

#if !defined (__SAIACL_EXPERIMENTAL_H_)
#define __SAIACL_EXPERIMENTAL_H_

#include <saiacl.h>

/**
 * @defgroup SAIACL EXPERIMENTAL SAI - ACL experimental API definitions
 *
 * @{
 */

/**
 * @brief DTel flow operation
 */
typedef enum _sai_acl_dtel_flow_op_t
{
    /** No operation */
    SAI_ACL_DTEL_FLOW_OP_NOP,

    /** Packet Postcard */
    SAI_ACL_DTEL_FLOW_OP_POSTCARD,

    /** In-band Network Telemetry */
    SAI_ACL_DTEL_FLOW_OP_INT,

    /** In-band OAM */
    SAI_ACL_DTEL_FLOW_OP_IOAM,

} sai_acl_dtel_flow_op_t;

typedef enum _sai_acl_action_experimental_type_t
{
    /** Start of experimental types */
    SAI_ACL_ACTION_TYPE_EXPERIMENTAL_START = SAI_ACL_ACTION_TYPE_CUSTOM_RANGE_END + 1,

    /** DTel flow operation */
    SAI_ACL_ACTION_TYPE_DTEL_FLOW_OP,

    /** INT configuration session */
    SAI_ACL_ACTION_TYPE_DTEL_INT_SESSION,

    /** Enable drop report */
    SAI_ACL_ACTION_TYPE_DTEL_DROP_REPORT_ENABLE,

    /** DTel flow sample percent within matched flow space */
    SAI_ACL_ACTION_TYPE_DTEL_FLOW_SAMPLE_PERCENT,

    /** Report every packet for the matched flow */
    SAI_ACL_ACTION_TYPE_DTEL_REPORT_ALL_PACKETS,

} sai_acl_action_experimental_type_t;

typedef enum _sai_acl_table_experimental_attr_t
{
    /**
     * @brief Start of experimental attributes
     */
    SAI_ACL_TABLE_ATTR_EXPERIMENTAL_START = SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_END + 1,

    /**
     * @brief Tunnel VNI
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_TUNNEL_VNI,

    /**
     * @brief Inner EtherType
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_INNER_ETHER_TYPE,

    /**
     * @brief Inner IP Protocol
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_INNER_IP_PROTOCOL,

    /**
     * @brief Inner L4 Src Port
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_INNER_L4_SRC_PORT,

    /**
     * @brief Inner L4 Dst Port
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_INNER_L4_DST_PORT,

} sai_acl_table_experimental_attr_t;

typedef enum _sai_acl_entry_experimental_attr_t
{
    /**
     * @brief Start of experimental attributes
     */
    SAI_ACL_ENTRY_ATTR_EXPERIMENTAL_START = SAI_ACL_ENTRY_ATTR_CUSTOM_RANGE_END + 1,

    /**
     * @brief Tunnel VNI
     *
     * @type sai_acl_field_data_t sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_ACL_ENTRY_ATTR_FIELD_TUNNEL_VNI,

    /**
     * @brief Inner EtherType
     *
     * @type sai_acl_field_data_t sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_ACL_ENTRY_ATTR_FIELD_INNER_ETHER_TYPE,

    /**
     * @brief Inner IP Protocol
     *
     * @type sai_acl_field_data_t sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_ACL_ENTRY_ATTR_FIELD_INNER_IP_PROTOCOL,

    /**
     * @brief Inner L4 Src Port
     *
     * @type sai_acl_field_data_t sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_ACL_ENTRY_ATTR_FIELD_INNER_L4_SRC_PORT,

    /**
     * @brief Inner L4 Dst Port
     *
     * @type sai_acl_field_data_t sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_ACL_ENTRY_ATTR_FIELD_INNER_L4_DST_PORT,

    /**
     * @brief DTel flow operation
     *
     * @type sai_acl_dtel_flow_op_t
     * @flags CREATE_AND_SET
     * @default SAI_ACL_DTEL_FLOW_OP_NOP
     */
    SAI_ACL_ENTRY_ATTR_ACTION_ACL_DTEL_FLOW_OP,

    /**
     * @brief INT session ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DTEL_INT_SESSION
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ACL_ENTRY_ATTR_ACTION_DTEL_INT_SESSION,

    /**
     * @brief Enable drop report
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ACL_ENTRY_ATTR_ACTION_DTEL_DROP_REPORT_ENABLE,

    /**
     * @brief Telemetry flow sample percent within matched flow space
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 100
     */
    SAI_ACL_ENTRY_ATTR_ACTION_DTEL_FLOW_SAMPLE_PERCENT,

    /**
     * @brief Report every packet for the matched flow
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ACL_ENTRY_ATTR_ACTION_DTEL_REPORT_ALL_PACKETS,

} sai_acl_entry_experimental_attr_t;


/**
 * @}
 */
#endif /** __SAIACL_EXPERIMENTAL_H_ */
