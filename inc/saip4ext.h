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
 * @file    saip4ext.h
 *
 * @brief   This module defines SAI P4 Extension interface
 */

#if !defined (__SAIP4EXT_H_)
#define __SAIP4EXT_H_

#include <saitypes.h>

/**
 * @brief Attribute Id for P4 ext
 */
typedef enum _sai_p4ext_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_P4EXT_ENTRY_ATTR_START,

    /**
     * @brief SAI P4 EXT table id
     *
     * @type sai_s8_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     */
    SAI_P4EXT_ENTRY_ATTR_TABLE_ID = SAI_P4EXT_ENTRY_ATTR_START,

    /**
     * @brief SAI P4 EXT Match field
     *
     * @type sai_s8_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     */
    SAI_P4EXT_ENTRY_ATTR_MATCH_FIELD_ID,

    /**
     * @brief SAI P4 EXT Action id
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_P4EXT_ENTRY_ATTR_ACTION_ID,

    /**
     * @brief SAI P4 EXT Action parameters
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_P4EXT_ENTRY_ATTR_PARAMETER_ID,

    /**
     * @brief End of attributes
     */
    SAI_P4EXT_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_P4EXT_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_P4EXT_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_p4ext_entry_attr_t;

/**
 * @brief Create an P4 table entry
 *
 * @param[out] p4ext_entry_id The P4 table id
 * @param[in] switch_id The Switch Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_p4ext_entry_fn)(
        _Out_ sai_object_id_t *p4ext_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete an P4 entry
 *
 * @param[in] p4ext_entry_id The P4 table id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_p4ext_entry_fn)(
        _In_ sai_object_id_t p4ext_entry_id);

/**
 * @brief Set P4 Table entry attribute
 *
 * @param[in] p4ext_entry_id The P4 table id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_p4ext_entry_attribute_fn)(
        _In_ sai_object_id_t p4ext_entry_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get P4 entry attribute
 *
 * @param[in] p4ext_entry_id P4 table id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_p4ext_entry_attribute_fn)(
        _In_ sai_object_id_t p4ext_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief P4Ext  methods table retrieved with sai_api_query()
 */
typedef struct _sai_p4ext_api_t
{
    sai_create_p4ext_entry_fn                     create_p4ext_entry;
    sai_remove_p4ext_entry_fn                     remove_p4ext_entry;
    sai_set_p4ext_entry_attribute_fn              set_p4ext_entry_attribute;
    sai_get_p4ext_entry_attribute_fn              get_p4ext_entry_attribute;
} sai_p4ext_api_t;

/**
 * @}
 */
#endif /** __SAIP4EXT_H_ */
