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
 * @file    saigenericprogrammable.h
 *
 * @brief   This module defines SAI Genetic Programmable Extensions (GPE)
 */

#if !defined (__SAIGENERICPROGRAMMABLE_H_)
#define __SAIGENERICPROGRAMMABLE_H_

#include <saitypes.h>

/**
 * @defgroup SAIGENERICPROGRAMMABLE SAI - GPE specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute Id for Generic Programmable extension
 */
typedef enum _sai_generic_programmable_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_GENERIC_PROGRAMMABLE_ATTR_START,

    /**
     * @brief HW block name to program the entry
     *
     * @type sai_s8_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_GENERIC_PROGRAMMABLE_ATTR_OBJECT_NAME = SAI_GENERIC_PROGRAMMABLE_ATTR_START,

    /**
     * @brief JSON string carrying HW block entry information
     *
     * @type sai_json_t
     * @flags CREATE_AND_SET
     * @default vendor
     */
    SAI_GENERIC_PROGRAMMABLE_ATTR_ENTRY,

    /**
     * @brief Attach a counter
     *
     * When it is empty, then packet hits won't be counted
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_GENERIC_PROGRAMMABLE_ATTR_COUNTER_ID,

    /**
     * @brief End of attributes
     */
    SAI_GENERIC_PROGRAMMABLE_ATTR_END,

    /** Custom range base value */
    SAI_GENERIC_PROGRAMMABLE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_GENERIC_PROGRAMMABLE_ATTR_CUSTOM_RANGE_END

} sai_generic_programmable_attr_t;

/**
 * @brief Create a Generic programmable entry
 *
 * @param[out] generic_programmable_id The OID returned per entry per HW block
 * @param[in] switch_id The Switch Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_generic_programmable_fn)(
        _Out_ sai_object_id_t *generic_programmable_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a Generic programmable entry
 *
 * @param[in] generic_programmable_id The table id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_generic_programmable_fn)(
        _In_ sai_object_id_t generic_programmable_id);

/**
 * @brief Set Generic programmable Table entry attribute
 *
 * @param[in] generic_programmable_id The table id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_generic_programmable_attribute_fn)(
        _In_ sai_object_id_t generic_programmable_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Generic programmable entry attribute
 *
 * @param[in] generic_programmable_id The table id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_generic_programmable_attribute_fn)(
        _In_ sai_object_id_t generic_programmable_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Generic extensions methods table retrieved with sai_api_query()
 */
typedef struct _sai_generic_programmable_api_t
{
    sai_create_generic_programmable_fn            create_generic_programmable;
    sai_remove_generic_programmable_fn            remove_generic_programmable;
    sai_set_generic_programmable_attribute_fn     set_generic_programmable_attribute;
    sai_get_generic_programmable_attribute_fn     get_generic_programmable_attribute;
} sai_generic_programmable_api_t;

/**
 * @}
 */
#endif /** __SAIGENERICPROGRAMMABLE_H_ */
