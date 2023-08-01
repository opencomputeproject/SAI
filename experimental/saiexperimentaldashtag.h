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
 * @file    saiexperimentaldashtag.h
 *
 * @brief   This module defines SAI P4 extension  interface
 */

#if !defined (__SAIEXPERIMENTALDASHTAG_H_)
#define __SAIEXPERIMENTALDASHTAG_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALDASH_TAG SAI - Extension specific API definitions
 *
 * @{
 */

/**
 * @brief Entry for dst_tag_entry
 */
typedef struct _sai_dst_tag_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief LPM matched key dip
     */
    sai_ip_prefix_t dip;

} sai_dst_tag_entry_t;

/**
 * @brief Attribute ID for dash_tag_dst_tag_entry
 */
typedef enum _sai_dst_tag_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DST_TAG_ENTRY_ATTR_START,

    /**
     * @brief Action set_dst_tag parameter TAG_MAP
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_DST_TAG_ENTRY_ATTR_TAG_MAP = SAI_DST_TAG_ENTRY_ATTR_START,

    /**
     * @brief IP address family for resource accounting
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_DST_TAG_ENTRY_ATTR_IP_ADDR_FAMILY,

    /**
     * @brief End of attributes
     */
    SAI_DST_TAG_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_DST_TAG_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DST_TAG_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_dst_tag_entry_attr_t;

/**
 * @brief Entry for src_tag_entry
 */
typedef struct _sai_src_tag_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief LPM matched key sip
     */
    sai_ip_prefix_t sip;

} sai_src_tag_entry_t;

/**
 * @brief Attribute ID for dash_tag_src_tag_entry
 */
typedef enum _sai_src_tag_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SRC_TAG_ENTRY_ATTR_START,

    /**
     * @brief Action set_src_tag parameter TAG_MAP
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SRC_TAG_ENTRY_ATTR_TAG_MAP = SAI_SRC_TAG_ENTRY_ATTR_START,

    /**
     * @brief IP address family for resource accounting
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_SRC_TAG_ENTRY_ATTR_IP_ADDR_FAMILY,

    /**
     * @brief End of attributes
     */
    SAI_SRC_TAG_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_SRC_TAG_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SRC_TAG_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_src_tag_entry_attr_t;

/**
 * @brief Create dash_tag_dst_tag_entry
 *
 * @param[in] dst_tag_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_dst_tag_entry_fn)(
        _In_ const sai_dst_tag_entry_t *dst_tag_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_tag_dst_tag_entry
 *
 * @param[in] dst_tag_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_dst_tag_entry_fn)(
        _In_ const sai_dst_tag_entry_t *dst_tag_entry);

/**
 * @brief Set attribute for dash_tag_dst_tag_entry
 *
 * @param[in] dst_tag_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_dst_tag_entry_attribute_fn)(
        _In_ const sai_dst_tag_entry_t *dst_tag_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_tag_dst_tag_entry
 *
 * @param[in] dst_tag_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_dst_tag_entry_attribute_fn)(
        _In_ const sai_dst_tag_entry_t *dst_tag_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create dash_tag_dst_tag_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] dst_tag_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_dst_tag_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_dst_tag_entry_t *dst_tag_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove dash_tag_dst_tag_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] dst_tag_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_dst_tag_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_dst_tag_entry_t *dst_tag_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create dash_tag_src_tag_entry
 *
 * @param[in] src_tag_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_src_tag_entry_fn)(
        _In_ const sai_src_tag_entry_t *src_tag_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_tag_src_tag_entry
 *
 * @param[in] src_tag_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_src_tag_entry_fn)(
        _In_ const sai_src_tag_entry_t *src_tag_entry);

/**
 * @brief Set attribute for dash_tag_src_tag_entry
 *
 * @param[in] src_tag_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_src_tag_entry_attribute_fn)(
        _In_ const sai_src_tag_entry_t *src_tag_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_tag_src_tag_entry
 *
 * @param[in] src_tag_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_src_tag_entry_attribute_fn)(
        _In_ const sai_src_tag_entry_t *src_tag_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create dash_tag_src_tag_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] src_tag_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_src_tag_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_src_tag_entry_t *src_tag_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove dash_tag_src_tag_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] src_tag_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_src_tag_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_src_tag_entry_t *src_tag_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

typedef struct _sai_dash_tag_api_t
{
    sai_create_dst_tag_entry_fn           create_dst_tag_entry;
    sai_remove_dst_tag_entry_fn           remove_dst_tag_entry;
    sai_set_dst_tag_entry_attribute_fn    set_dst_tag_entry_attribute;
    sai_get_dst_tag_entry_attribute_fn    get_dst_tag_entry_attribute;
    sai_bulk_create_dst_tag_entry_fn      create_dst_tag_entries;
    sai_bulk_remove_dst_tag_entry_fn      remove_dst_tag_entries;

    sai_create_src_tag_entry_fn           create_src_tag_entry;
    sai_remove_src_tag_entry_fn           remove_src_tag_entry;
    sai_set_src_tag_entry_attribute_fn    set_src_tag_entry_attribute;
    sai_get_src_tag_entry_attribute_fn    get_src_tag_entry_attribute;
    sai_bulk_create_src_tag_entry_fn      create_src_tag_entries;
    sai_bulk_remove_src_tag_entry_fn      remove_src_tag_entries;

} sai_dash_tag_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHTAG_H_ */
