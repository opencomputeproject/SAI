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
 * @file    saicustomone.h
 *
 * @brief   This module defines SAI custom for ONE
 *
 * @warning This module is a SAI custom module
 */

#if !defined (__SAICUSTOMONE_H_)
#define __SAICUSTOMONE_H_

#include <saitypescustom.h>

/**
 * @defgroup SAICUSTOMONE SAI - Custom ONE specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for ONE
 */
typedef enum _sai_one_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_ONE_ATTR_START,

    /**
     * @brief First parameter
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ONE_ATTR_FIRST = SAI_ONE_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_ONE_ATTR_END,

    /* ironic */

    /** Custom range base value */
    SAI_ONE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_ONE_ATTR_CUSTOM_RANGE_END,

} sai_one_attr_t;

/**
 * @brief Create ONE
 *
 * @param[out] one_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_one_fn)(
        _Out_ sai_object_id_t *one_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove ONE
 *
 * @param[in] one_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_one_fn)(
        _In_ sai_object_id_t one_id);

/**
 * @brief Set attribute for ONE
 *
 * @param[in] one_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_one_attribute_fn)(
        _In_ sai_object_id_t one_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for ONE
 *
 * @param[in] one_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_one_attribute_fn)(
        _In_ sai_object_id_t one_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_one_api_t
{
    sai_create_one_fn           create_one;
    sai_remove_one_fn           remove_one;
    sai_set_one_attribute_fn    set_one_attribute;
    sai_get_one_attribute_fn    get_one_attribute;

} sai_one_api_t;

/**
 * @}
 */
#endif /** __CUSTOMONE_H_ */
