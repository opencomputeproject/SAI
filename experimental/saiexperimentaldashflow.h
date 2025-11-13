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
 * @file    saiexperimentaldashflow.h
 *
 * @brief   This module defines SAI extensions for DASH flow
 *
 * @warning This module is a SAI experimental module
 */

#if !defined (__SAIEXPERIMENTALDASHFLOW_H_)
#define __SAIEXPERIMENTALDASHFLOW_H_

#include <saitypesextensions.h>

/**
 * @defgroup SAIEXPERIMENTALDASHFLOW SAI - Experimental: DASH flow specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_FLOW_ENTRY_ATTR_ACTION
 */
typedef enum _sai_flow_entry_action_t
{
    SAI_FLOW_ENTRY_ACTION_SET_FLOW_ENTRY_ATTR,

} sai_flow_entry_action_t;

/**
 * @brief Attribute ID for flow table
 */
typedef enum _sai_flow_table_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FLOW_TABLE_ATTR_START,

    /**
     * @brief Action parameter max flow count
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_TABLE_ATTR_MAX_FLOW_COUNT = SAI_FLOW_TABLE_ATTR_START,

    /**
     * @brief Action parameter DASH flow enabled key
     *
     * @type sai_dash_flow_enabled_key_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_FLOW_ENABLED_KEY_ENI_MAC
     */
    SAI_FLOW_TABLE_ATTR_DASH_FLOW_ENABLED_KEY,

    /**
     * @brief Action parameter flow TTL in milliseconds
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_TABLE_ATTR_FLOW_TTL_IN_MILLISECONDS,

    /**
     * @brief End of attributes
     */
    SAI_FLOW_TABLE_ATTR_END,

    /** Custom range base value */
    SAI_FLOW_TABLE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_FLOW_TABLE_ATTR_CUSTOM_RANGE_END,

} sai_flow_table_attr_t;

/**
 * @brief Entry for flow_entry
 */
typedef struct _sai_flow_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key eni_mac
     */
    sai_mac_t eni_mac;

    /**
     * @brief Exact matched key vnet_id
     */
    sai_uint16_t vnet_id;

    /**
     * @brief Exact matched key ip_proto
     */
    sai_uint8_t ip_proto;

    /**
     * @brief Exact matched key src_ip
     */
    sai_ip_address_t src_ip;

    /**
     * @brief Exact matched key dst_ip
     */
    sai_ip_address_t dst_ip;

    /**
     * @brief Exact matched key src_port
     */
    sai_uint16_t src_port;

    /**
     * @brief Exact matched key dst_port
     */
    sai_uint16_t dst_port;

} sai_flow_entry_t;

/**
 * @brief Attribute ID for flow entry
 */
typedef enum _sai_flow_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FLOW_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_flow_entry_action_t
     * @flags CREATE_AND_SET
     * @default SAI_FLOW_ENTRY_ACTION_SET_FLOW_ENTRY_ATTR
     */
    SAI_FLOW_ENTRY_ATTR_ACTION = SAI_FLOW_ENTRY_ATTR_START,

    /**
     * @brief Action parameter version
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_VERSION,

    /**
     * @brief Action parameter DASH direction
     *
     * @type sai_dash_direction_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_DIRECTION_INVALID
     */
    SAI_FLOW_ENTRY_ATTR_DASH_DIRECTION,

    /**
     * @brief Action parameter DASH flow action
     *
     * @type sai_dash_flow_action_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_FLOW_ACTION_NONE
     */
    SAI_FLOW_ENTRY_ATTR_DASH_FLOW_ACTION,

    /**
     * @brief Action parameter meter class
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_METER_CLASS,

    /**
     * @brief Action parameter is unidirectional flow
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_FLOW_ENTRY_ATTR_IS_UNIDIRECTIONAL_FLOW,

    /**
     * @brief Action parameter reverse flow ENI MAC
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default vendor
     */
    SAI_FLOW_ENTRY_ATTR_REVERSE_FLOW_ENI_MAC,

    /**
     * @brief Action parameter reverse flow VNET id
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_REVERSE_FLOW_VNET_ID,

    /**
     * @brief Action parameter reverse flow IP protocol
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_REVERSE_FLOW_IP_PROTO,

    /**
     * @brief Action parameter reverse flow src IP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FLOW_ENTRY_ATTR_REVERSE_FLOW_SRC_IP,

    /**
     * @brief Action parameter reverse flow dst IP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FLOW_ENTRY_ATTR_REVERSE_FLOW_DST_IP,

    /**
     * @brief Action parameter reverse flow src port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_REVERSE_FLOW_SRC_PORT,

    /**
     * @brief Action parameter reverse flow dst port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_REVERSE_FLOW_DST_PORT,

    /**
     * @brief Action parameter underlay0 VNET id
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_UNDERLAY0_VNET_ID,

    /**
     * @brief Action parameter underlay0 sip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FLOW_ENTRY_ATTR_UNDERLAY0_SIP,

    /**
     * @brief Action parameter underlay0 dip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FLOW_ENTRY_ATTR_UNDERLAY0_DIP,

    /**
     * @brief Action parameter underlay0 DASH encapsulation
     *
     * @type sai_dash_encapsulation_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_ENCAPSULATION_INVALID
     */
    SAI_FLOW_ENTRY_ATTR_UNDERLAY0_DASH_ENCAPSULATION,

    /**
     * @brief Action parameter underlay1 VNET id
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_UNDERLAY1_VNET_ID,

    /**
     * @brief Action parameter underlay1 sip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FLOW_ENTRY_ATTR_UNDERLAY1_SIP,

    /**
     * @brief Action parameter underlay1 dip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FLOW_ENTRY_ATTR_UNDERLAY1_DIP,

    /**
     * @brief Action parameter underlay1 source MAC
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default vendor
     */
    SAI_FLOW_ENTRY_ATTR_UNDERLAY1_SMAC,

    /**
     * @brief Action parameter underlay1 destination MAC
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default vendor
     */
    SAI_FLOW_ENTRY_ATTR_UNDERLAY1_DMAC,

    /**
     * @brief Action parameter underlay1 DASH encapsulation
     *
     * @type sai_dash_encapsulation_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_ENCAPSULATION_INVALID
     */
    SAI_FLOW_ENTRY_ATTR_UNDERLAY1_DASH_ENCAPSULATION,

    /**
     * @brief Action parameter dst MAC
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default vendor
     */
    SAI_FLOW_ENTRY_ATTR_DST_MAC,

    /**
     * @brief Action parameter sip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FLOW_ENTRY_ATTR_SIP,

    /**
     * @brief Action parameter dip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FLOW_ENTRY_ATTR_DIP,

    /**
     * @brief Action parameter sip mask
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FLOW_ENTRY_ATTR_SIP_MASK,

    /**
     * @brief Action parameter dip mask
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FLOW_ENTRY_ATTR_DIP_MASK,

    /**
     * @brief Action parameter vendor metadata
     *
     * @type sai_u8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_FLOW_ENTRY_ATTR_VENDOR_METADATA,

    /**
     * @brief Action parameter flow data protocol buffer
     *
     * @type sai_u8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_FLOW_ENTRY_ATTR_FLOW_DATA_PB,

    /**
     * @brief IP address family for resource accounting
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_FLOW_ENTRY_ATTR_IP_ADDR_FAMILY,

    /**
     * @brief Action parameter DASH flow sync state
     *
     * @type sai_dash_flow_sync_state_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_FLOW_SYNC_STATE_FLOW_MISS
     */
    SAI_FLOW_ENTRY_ATTR_DASH_FLOW_SYNC_STATE,

    /**
     * @brief Action parameter underlay0 source MAC
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default vendor
     */
    SAI_FLOW_ENTRY_ATTR_UNDERLAY0_SMAC,

    /**
     * @brief Action parameter underlay0 destination MAC
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default vendor
     */
    SAI_FLOW_ENTRY_ATTR_UNDERLAY0_DMAC,

    /**
     * @brief End of attributes
     */
    SAI_FLOW_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_FLOW_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_FLOW_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_flow_entry_attr_t;

/**
 * @brief Attribute ID for flow entry bulk get session filter
 */
typedef enum _sai_flow_entry_bulk_get_session_filter_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ATTR_START,

    /**
     * @brief Action parameter DASH flow entry bulk get session filter key
     *
     * @type sai_dash_flow_entry_bulk_get_session_filter_key_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_KEY_INVALID
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ATTR_DASH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_KEY = SAI_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ATTR_START,

    /**
     * @brief Action parameter DASH flow entry bulk get session op key
     *
     * @type sai_dash_flow_entry_bulk_get_session_op_key_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_OP_KEY_FILTER_OP_INVALID
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ATTR_DASH_FLOW_ENTRY_BULK_GET_SESSION_OP_KEY,

    /**
     * @brief Action parameter int value
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ATTR_INT_VALUE,

    /**
     * @brief Action parameter IP value
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ATTR_IP_VALUE,

    /**
     * @brief Action parameter MAC value
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default vendor
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ATTR_MAC_VALUE,

    /**
     * @brief End of attributes
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ATTR_END,

    /** Custom range base value */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ATTR_CUSTOM_RANGE_END,

} sai_flow_entry_bulk_get_session_filter_attr_t;

/**
 * @brief Attribute ID for flow entry bulk get session
 */
typedef enum _sai_flow_entry_bulk_get_session_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_START,

    /**
     * @brief Action parameter DASH flow entry bulk get session mode
     *
     * @type sai_dash_flow_entry_bulk_get_session_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_MODE_SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_MODE_GRPC
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_DASH_FLOW_ENTRY_BULK_GET_SESSION_MODE = SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_START,

    /**
     * @brief Action parameter bulk get entry limitation
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_BULK_GET_ENTRY_LIMITATION,

    /**
     * @brief Action parameter bulk get session server IP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_BULK_GET_SESSION_SERVER_IP,

    /**
     * @brief Action parameter bulk get session server port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_BULK_GET_SESSION_SERVER_PORT,

    /**
     * @brief Action parameter first flow entry bulk get session filter id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION_FILTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_FIRST_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ID,

    /**
     * @brief Action parameter second flow entry bulk get session filter id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION_FILTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_SECOND_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ID,

    /**
     * @brief Action parameter third flow entry bulk get session filter id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION_FILTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_THIRD_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ID,

    /**
     * @brief Action parameter fourth flow entry bulk get session filter id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION_FILTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_FOURTH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ID,

    /**
     * @brief Action parameter fifth flow entry bulk get session filter id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION_FILTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_FIFTH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_ID,

    /**
     * @brief End of attributes
     */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_END,

    /** Custom range base value */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_FLOW_ENTRY_BULK_GET_SESSION_ATTR_CUSTOM_RANGE_END,

} sai_flow_entry_bulk_get_session_attr_t;

/**
 * @brief Create flow table
 *
 * @param[out] flow_table_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_flow_table_fn)(
        _Out_ sai_object_id_t *flow_table_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove flow table
 *
 * @param[in] flow_table_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_flow_table_fn)(
        _In_ sai_object_id_t flow_table_id);

/**
 * @brief Set attribute for flow table
 *
 * @param[in] flow_table_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_flow_table_attribute_fn)(
        _In_ sai_object_id_t flow_table_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for flow table
 *
 * @param[in] flow_table_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_flow_table_attribute_fn)(
        _In_ sai_object_id_t flow_table_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create flow entry
 *
 * @param[in] flow_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_flow_entry_fn)(
        _In_ const sai_flow_entry_t *flow_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove flow entry
 *
 * @param[in] flow_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_flow_entry_fn)(
        _In_ const sai_flow_entry_t *flow_entry);

/**
 * @brief Set attribute for flow entry
 *
 * @param[in] flow_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_flow_entry_attribute_fn)(
        _In_ const sai_flow_entry_t *flow_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for flow entry
 *
 * @param[in] flow_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_flow_entry_attribute_fn)(
        _In_ const sai_flow_entry_t *flow_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create flow entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] flow_entry List of object to create
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to create.
 * @param[in] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are created or
 * #SAI_STATUS_FAILURE when any of the objects fails to create. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_create_flow_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_flow_entry_t *flow_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove flow entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] flow_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_flow_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_flow_entry_t *flow_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create flow entry bulk get session filter
 *
 * @param[out] flow_entry_bulk_get_session_filter_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_flow_entry_bulk_get_session_filter_fn)(
        _Out_ sai_object_id_t *flow_entry_bulk_get_session_filter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove flow entry bulk get session filter
 *
 * @param[in] flow_entry_bulk_get_session_filter_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_flow_entry_bulk_get_session_filter_fn)(
        _In_ sai_object_id_t flow_entry_bulk_get_session_filter_id);

/**
 * @brief Set attribute for flow entry bulk get session filter
 *
 * @param[in] flow_entry_bulk_get_session_filter_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_flow_entry_bulk_get_session_filter_attribute_fn)(
        _In_ sai_object_id_t flow_entry_bulk_get_session_filter_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for flow entry bulk get session filter
 *
 * @param[in] flow_entry_bulk_get_session_filter_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_flow_entry_bulk_get_session_filter_attribute_fn)(
        _In_ sai_object_id_t flow_entry_bulk_get_session_filter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create flow entry bulk get session
 *
 * @param[out] flow_entry_bulk_get_session_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_flow_entry_bulk_get_session_fn)(
        _Out_ sai_object_id_t *flow_entry_bulk_get_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove flow entry bulk get session
 *
 * @param[in] flow_entry_bulk_get_session_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_flow_entry_bulk_get_session_fn)(
        _In_ sai_object_id_t flow_entry_bulk_get_session_id);

/**
 * @brief Set attribute for flow entry bulk get session
 *
 * @param[in] flow_entry_bulk_get_session_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_flow_entry_bulk_get_session_attribute_fn)(
        _In_ sai_object_id_t flow_entry_bulk_get_session_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for flow entry bulk get session
 *
 * @param[in] flow_entry_bulk_get_session_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_flow_entry_bulk_get_session_attribute_fn)(
        _In_ sai_object_id_t flow_entry_bulk_get_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_dash_flow_api_t
{
    sai_create_flow_table_fn                                   create_flow_table;
    sai_remove_flow_table_fn                                   remove_flow_table;
    sai_set_flow_table_attribute_fn                            set_flow_table_attribute;
    sai_get_flow_table_attribute_fn                            get_flow_table_attribute;
    sai_bulk_object_create_fn                                  create_flow_tables;
    sai_bulk_object_remove_fn                                  remove_flow_tables;

    sai_create_flow_entry_fn                                   create_flow_entry;
    sai_remove_flow_entry_fn                                   remove_flow_entry;
    sai_set_flow_entry_attribute_fn                            set_flow_entry_attribute;
    sai_get_flow_entry_attribute_fn                            get_flow_entry_attribute;
    sai_bulk_create_flow_entry_fn                              create_flow_entries;
    sai_bulk_remove_flow_entry_fn                              remove_flow_entries;

    sai_create_flow_entry_bulk_get_session_filter_fn           create_flow_entry_bulk_get_session_filter;
    sai_remove_flow_entry_bulk_get_session_filter_fn           remove_flow_entry_bulk_get_session_filter;
    sai_set_flow_entry_bulk_get_session_filter_attribute_fn    set_flow_entry_bulk_get_session_filter_attribute;
    sai_get_flow_entry_bulk_get_session_filter_attribute_fn    get_flow_entry_bulk_get_session_filter_attribute;
    sai_bulk_object_create_fn                                  create_flow_entry_bulk_get_session_filters;
    sai_bulk_object_remove_fn                                  remove_flow_entry_bulk_get_session_filters;

    sai_create_flow_entry_bulk_get_session_fn                  create_flow_entry_bulk_get_session;
    sai_remove_flow_entry_bulk_get_session_fn                  remove_flow_entry_bulk_get_session;
    sai_set_flow_entry_bulk_get_session_attribute_fn           set_flow_entry_bulk_get_session_attribute;
    sai_get_flow_entry_bulk_get_session_attribute_fn           get_flow_entry_bulk_get_session_attribute;
    sai_bulk_object_create_fn                                  create_flow_entry_bulk_get_sessions;
    sai_bulk_object_remove_fn                                  remove_flow_entry_bulk_get_sessions;

} sai_dash_flow_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHFLOW_H_ */
