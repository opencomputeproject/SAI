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
 * @file    saiexperimentaldashvnet.h
 *
 * @brief   This module defines SAI P4 extension  interface
 */

#if !defined (__SAIEXPERIMENTALDASHVNET_H_)
#define __SAIEXPERIMENTALDASHVNET_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALDASH_VNET SAI - Extension specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for dash_vnet_vnet
 */
typedef enum _sai_vnet_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_VNET_ATTR_START,

    /**
     * @brief Action set_vnet_attrs parameter VNI
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_VNET_ATTR_VNI = SAI_VNET_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_VNET_ATTR_END,

    /** Custom range base value */
    SAI_VNET_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_VNET_ATTR_CUSTOM_RANGE_END,

} sai_vnet_attr_t;

/**
 * @brief Create dash_vnet_vnet
 *
 * @param[out] vnet_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_vnet_fn)(
        _Out_ sai_object_id_t *vnet_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_vnet_vnet
 *
 * @param[in] vnet_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_vnet_fn)(
        _In_ sai_object_id_t vnet_id);

/**
 * @brief Set attribute for dash_vnet_vnet
 *
 * @param[in] vnet_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_vnet_attribute_fn)(
        _In_ sai_object_id_t vnet_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_vnet_vnet
 *
 * @param[in] vnet_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_vnet_attribute_fn)(
        _In_ sai_object_id_t vnet_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_dash_vnet_api_t
{
    sai_create_vnet_fn           create_vnet;
    sai_remove_vnet_fn           remove_vnet;
    sai_set_vnet_attribute_fn    set_vnet_attribute;
    sai_get_vnet_attribute_fn    get_vnet_attribute;
    sai_bulk_object_create_fn    create_vnets;
    sai_bulk_object_remove_fn    remove_vnets;

} sai_dash_vnet_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHVNET_H_ */
