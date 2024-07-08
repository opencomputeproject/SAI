/**
 * Copyright (c) 2024 Microsoft Open Technologies, Inc.
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
 * @file    saiprefixcompression.h
 *
 * @brief   This module defines SAI prefix compression interface
 */

#if !defined (__SAIPREFIXCOMPRESSION_H_)
#define __SAIPREFIXCOMPRESSION_H_

#include <saitypes.h>

/**
 * @defgroup SAIPREFIXCOMPRESSION SAI - Prefix Compression API definitions
 *
 * @{
 */

/**
 * @brief Attribute Id for SAI prefix compression object
 */
typedef enum _sai_prefix_compression_table_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PREFIX_COMPRESSION_TABLE_ATTR_START,

    /**
     * @brief Label attribute used to unique identify Table.
     *
     * @type char
     * @flags CREATE_AND_SET
     * @default ""
     */
    SAI_PREFIX_COMPRESSION_TABLE_ATTR_LABEL = SAI_PREFIX_COMPRESSION_TABLE_ATTR_START,

    /**
     * @brief Prefix Compression table stage
     *
     * @type sai_prefix_compression_stage_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_PREFIX_COMPRESSION_TABLE_ATTR_PREFIX_COMPRESSION_STAGE,

    /**
     * @brief Prefix Compression table type
     *
     * @type sai_prefix_compression_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_PREFIX_COMPRESSION_TABLE_ATTR_PREFIX_COMPRESSION_TYPE,

    /**
     * @brief End of attributes
     */
    SAI_PREFIX_COMPRESSION_TABLE_ATTR_END,

    /** Custom range base value */
    SAI_PREFIX_COMPRESSION_TABLE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PREFIX_COMPRESSION_TABLE_ATTR_CUSTOM_RANGE_END

} sai_prefix_compression_table_attr_t;

/**
 * @brief Attribute Id for SAI prefix compression object
 */
typedef enum _sai_prefix_compression_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PREFIX_COMPRESSION_ENTRY_ATTR_START,

    /**
     * @brief Prefix Compression entry meta data
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_PREFIX_COMPRESSION_ENTRY_ATTR_META = SAI_PREFIX_COMPRESSION_ENTRY_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_PREFIX_COMPRESSION_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_PREFIX_COMPRESSION_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PREFIX_COMPRESSION_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_prefix_compression_entry_attr_t;

/**
 * @brief Prefix Compression entry
 */
typedef struct _sai_prefix_compression_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Prefix Compression Table ID
     *
     * @objects SAI_OBJECT_TYPE_PREFIX_COMPRESSION_TABLE
     */
    sai_object_id_t prefix_table_id;

    /**
     * @brief IP Prefix Destination
     */
    sai_ip_prefix_t prefix;

} sai_prefix_compression_entry_t;

/**
 * @brief Create prefix compression table
 *
 * @param[out] prefix_compression_table_id Prefix compression table ID
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_prefix_compression_table_fn)(
        _Out_ sai_object_id_t *prefix_compression_table_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove prefix compression table
 *
 * @param[in] prefix_compression_table_id Prefix compression table ID
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_prefix_compression_table_fn)(
        _In_ sai_object_id_t prefix_compression_table_id);

/**
 * @brief Set prefix compression attribute Value
 *
 * @param[in] prefix_compression_table_id Prefix compression table ID
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_prefix_compression_table_attribute_fn)(
        _In_ sai_object_id_t prefix_compression_table_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get prefix compression attribute Value
 *
 * @param[in] prefix_compression_table_id Prefix compression table ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_prefix_compression_table_attribute_fn)(
        _In_ sai_object_id_t prefix_compression_table_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create prefix compression Entry
 *
 * Note: IP prefix/mask expected in Network Byte Order.
 *
 * @param[in] prefix_compression_entry Prefix Compression entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_prefix_compression_entry_fn)(
        _In_ const sai_prefix_compression_entry_t *prefix_compression_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove prefix compression Entry
 *
 * Note: IP prefix/mask expected in Network Byte Order.
 *
 * @param[in] prefix_compression_entry Prefix Compression entry
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_prefix_compression_entry_fn)(
        _In_ const sai_prefix_compression_entry_t *prefix_compression_entry);

/**
 * @brief Set prefix compression attribute value
 *
 * @param[in] prefix_compression_entry Prefix Compression entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_prefix_compression_entry_attribute_fn)(
        _In_ const sai_prefix_compression_entry_t *prefix_compression_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get prefix compression attribute value
 *
 * @param[in] prefix_compression_entry Prefix Compression entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_prefix_compression_entry_attribute_fn)(
        _In_ const sai_prefix_compression_entry_t *prefix_compression_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create prefix compression entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] prefix_compression_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_prefix_compression_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_prefix_compression_entry_t *prefix_compression_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove prefix compression entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] prefix_compression_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_prefix_compression_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_prefix_compression_entry_t *prefix_compression_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Prefix Compression methods table retrieved with sai_api_query()
 */
typedef struct _sai_prefix_compression_api_t
{
    sai_create_prefix_compression_table_fn              create_prefix_compression_table;
    sai_remove_prefix_compression_table_fn              remove_prefix_compression_table;
    sai_set_prefix_compression_table_attribute_fn       set_prefix_compression_table_attribute;
    sai_get_prefix_compression_table_attribute_fn       get_prefix_compression_table_attribute;
    sai_create_prefix_compression_entry_fn              create_prefix_compression_entry;
    sai_remove_prefix_compression_entry_fn              remove_prefix_compression_entry;
    sai_set_prefix_compression_entry_attribute_fn       set_prefix_compression_entry_attribute;
    sai_get_prefix_compression_entry_attribute_fn       get_prefix_compression_entry_attribute;
    sai_bulk_create_prefix_compression_entry_fn         create_prefix_compression_entries;
    sai_bulk_remove_prefix_compression_entry_fn         remove_prefix_compression_entries;
} sai_prefix_compression_api_t;

/**
 * @}
 */
#endif /** __SAIPREFIXCOMPRESSION_H_ */
