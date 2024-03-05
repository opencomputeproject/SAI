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
 * @brief   This module defines SAI P4 extension  interface
 */

#if !defined (__SAIEXPERIMENTALDASHFLOW_H_)
#define __SAIEXPERIMENTALDASHFLOW_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALDASH_FLOW SAI - Extension specific API definitions
 *
 * @{
 */

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
     * @brief Exact matched key dip
     */
    sai_ip_address_t dip;

    /**
     * @brief Exact matched key sip
     */
    sai_ip_address_t sip;

    /**
     * @brief Exact matched key protocol
     */
    sai_uint16_t protocol;

    /**
     * @brief Exact matched key src_port
     */
    sai_uint16_t src_port;

    /**
     * @brief Exact matched key dst_port
     */
    sai_uint16_t dst_port;

    /**
     * @brief Exact matched key direction
     */
    sai_uint32_t direction;

    /**
     * @brief Exact matched key eni_id
     *
     * @objects SAI_OBJECT_TYPE_ENI
     */
    sai_object_id_t eni_id;

} sai_flow_entry_t;

/**
 * @brief Attribute ID for dash_flow_flow_entry
 */
typedef enum _sai_flow_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FLOW_ENTRY_ATTR_START,

    /**
     * @brief Action flow_entry_action parameter FLOW_TABLE_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_FLOW_TABLE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_FLOW_ENTRY_ATTR_FLOW_TABLE_ID = SAI_FLOW_ENTRY_ATTR_START,

    /**
     * @brief Action flow_entry_action parameter FLOW_VERSION
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_FLOW_VERSION,

    /**
     * @brief Action flow_entry_action parameter FLOW_PROTOBUF
     *
     * @type sai_u8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_FLOW_ENTRY_ATTR_FLOW_PROTOBUF,

    /**
     * @brief Action flow_entry_action parameter FLOW_BIDIRECTIONAL
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_FLOW_BIDIRECTIONAL,

    /**
     * @brief Action flow_entry_action parameter FLOW_DIRECTION
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_FLOW_DIRECTION,

    /**
     * @brief Action flow_entry_action parameter FLOW_REVERSE_KEY
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_FLOW_REVERSE_KEY,

    /**
     * @brief Action flow_entry_action parameter FLOW_POLICY_RESULT
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_FLOW_POLICY_RESULT,

    /**
     * @brief Action flow_entry_action parameter FLOW_DEST_PA
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_FLOW_DEST_PA,

    /**
     * @brief Action flow_entry_action parameter FLOW_METERING_CLASS
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_FLOW_METERING_CLASS,

    /**
     * @brief Action flow_entry_action parameter FLOW_REWRITE_INFO
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_FLOW_REWRITE_INFO,

    /**
     * @brief Action flow_entry_action parameter FLOW_VENDOR_METADATA
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_ENTRY_ATTR_FLOW_VENDOR_METADATA,

    /**
     * @brief IP address family for resource accounting
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_FLOW_ENTRY_ATTR_IP_ADDR_FAMILY,

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
 * @brief Bulk Get Op filter keywords for flow_entry in get_flow_entries_attribute call
 */
typedef enum _sai_flow_entry_bulk_get_filter_t
{
    /** Bulk get filter key word for sai_ip_address_t dip */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_DIP,

    /** Bulk get filter key word for sai_ip_address_t sip */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_SIP,

    /** Bulk get filter key word for sai_uint16_t protocol */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_PROTOCOL,

    /** Bulk get filter key word for sai_uint16_t src_port */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_SRC_PORT,

    /** Bulk get filter key word for sai_uint16_t dst_port */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_DST_PORT,

    /** Bulk get filter key word for sai_uint32_t direction */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_DIRECTION,

    /** Bulk get filter key word for sai_object_id_t eni_id */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_ENI_ID,

    /** Bulk get filter key word for SAI_FLOW_ENTRY_ATTR_FLOW_TABLE_ID */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_FLOW_TABLE_ID,

    /** Bulk get filter key word for SAI_FLOW_ENTRY_ATTR_FLOW_VERSION */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_FLOW_VERSION,

    /** Bulk get filter key word for SAI_FLOW_ENTRY_ATTR_FLOW_PROTOBUF */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_FLOW_PROTOBUF,

    /** Bulk get filter key word for SAI_FLOW_ENTRY_ATTR_FLOW_BIDIRECTIONAL */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_FLOW_BIDIRECTIONAL,

    /** Bulk get filter key word for SAI_FLOW_ENTRY_ATTR_FLOW_DIRECTION */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_FLOW_DIRECTION,

    /** Bulk get filter key word for SAI_FLOW_ENTRY_ATTR_FLOW_REVERSE_KEY */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_FLOW_REVERSE_KEY,

    /** Bulk get filter key word for SAI_FLOW_ENTRY_ATTR_FLOW_POLICY_RESULT */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_FLOW_POLICY_RESULT,

    /** Bulk get filter key word for SAI_FLOW_ENTRY_ATTR_FLOW_DEST_PA */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_FLOW_DEST_PA,

    /** Bulk get filter key word for SAI_FLOW_ENTRY_ATTR_FLOW_METERING_CLASS */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_FLOW_METERING_CLASS,

    /** Bulk get filter key word for SAI_FLOW_ENTRY_ATTR_FLOW_REWRITE_INFO */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_FLOW_REWRITE_INFO,

    /** Bulk get filter key word for SAI_FLOW_ENTRY_ATTR_FLOW_VENDOR_METADATA */
    SAI_FLOW_ENTRY_BULK_GET_FILTER_T_FLOW_VENDOR_METADATA,

} sai_flow_entry_bulk_get_filter_t;

/**
 * @brief Bulk Get Op for flow_entry in get_flow_entries_attribute call
 */
typedef enum _sai_flow_entry_bulk_get_op_t
{
    /**  Indicate the last OP of Bulk Get */
    SAI_FLOW_ENTRY_BULK_GET_OP_LAST_ITEM,

    /** Operation parameter for normal return */
    SAI_FLOW_ENTRY_BULK_GET_OP_NORMAL_RETURN,

    /** Operation parameter for GRPC return (server IP address) */
    SAI_FLOW_ENTRY_BULK_GET_OP_GRPC_SERVER_IP,

    /**  Operation parameter for GRPC return (server port) */
    SAI_FLOW_ENTRY_BULK_GET_OP_GRPC_SERVER_PORT,

    /** Operation parameter for get filter operator */
    SAI_FLOW_ENTRY_BULK_GET_OP_FILTER_OP,

} sai_flow_entry_bulk_get_op_t;

/**
 * @brief Attribute ID for dash_flow_flow_table
 */
typedef enum _sai_flow_table_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FLOW_TABLE_ATTR_START,

    /**
     * @brief Action flow_table_action parameter TABLE_SIZE
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_TABLE_ATTR_TABLE_SIZE = SAI_FLOW_TABLE_ATTR_START,

    /**
     * @brief Action flow_table_action parameter TABLE_EXPIRE_TIME
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_TABLE_ATTR_TABLE_EXPIRE_TIME,

    /**
     * @brief Action flow_table_action parameter TABLE_VERSION
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_TABLE_ATTR_TABLE_VERSION,

    /**
     * @brief Action flow_table_action parameter TABLE_KEY_FLAG
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FLOW_TABLE_ATTR_TABLE_KEY_FLAG,

    /**
     * @brief Action flow_table_action parameter TABLE_TCP_TRACK_STATE
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_FLOW_TABLE_ATTR_TABLE_TCP_TRACK_STATE,

    /**
     * @brief Action flow_table_action parameter TABLE_TCP_RESET_ILLEGAL
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_FLOW_TABLE_ATTR_TABLE_TCP_RESET_ILLEGAL,

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
 * @brief Create dash_flow_flow_entry
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
 * @brief Remove dash_flow_flow_entry
 *
 * @param[in] flow_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_flow_entry_fn)(
        _In_ const sai_flow_entry_t *flow_entry);

/**
 * @brief Set attribute for dash_flow_flow_entry
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
 * @brief Get attribute for dash_flow_flow_entry
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
 * @brief Bulk create dash_flow_flow_entry
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
 * @brief Bulk remove dash_flow_flow_entry
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
 * @brief Bulk get dash_flow_flow_entry
 *
 * @param[in] object_count Max number of objects to get
 * @param[in] flow_entry List of object to get
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to create.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses Status for each object.
 *    If the allocated attribute count is not large enough,
 *    set the status to #SAI_STATUS_BUFFER_OVERFLOW.
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are created or
 * #SAI_STATUS_FAILURE when any of the objects fails to create. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_flow_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_flow_entry_t *flow_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create dash_flow_flow_table
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
 * @brief Remove dash_flow_flow_table
 *
 * @param[in] flow_table_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_flow_table_fn)(
        _In_ sai_object_id_t flow_table_id);

/**
 * @brief Set attribute for dash_flow_flow_table
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
 * @brief Get attribute for dash_flow_flow_table
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

typedef struct _sai_dash_flow_api_t
{
    sai_create_flow_entry_fn           create_flow_entry;
    sai_remove_flow_entry_fn           remove_flow_entry;
    sai_set_flow_entry_attribute_fn    set_flow_entry_attribute;
    sai_get_flow_entry_attribute_fn    get_flow_entry_attribute;
    sai_bulk_create_flow_entry_fn      create_flow_entries;
    sai_bulk_remove_flow_entry_fn      remove_flow_entries;
    sai_bulk_get_flow_entry_fn         get_flow_entries_attribute;

    sai_create_flow_table_fn           create_flow_table;
    sai_remove_flow_table_fn           remove_flow_table;
    sai_set_flow_table_attribute_fn    set_flow_table_attribute;
    sai_get_flow_table_attribute_fn    get_flow_table_attribute;
    sai_bulk_object_create_fn          create_flow_tables;
    sai_bulk_object_remove_fn          remove_flow_tables;

} sai_dash_flow_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHFLOW_H_ */
