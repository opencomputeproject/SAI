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
 * @file    saiexperimentaldashappliance.h
 *
 * @brief   This module defines SAI extensions for DASH appliance
 *
 * @warning This module is a SAI experimental module
 */

#if !defined (__SAIEXPERIMENTALDASHAPPLIANCE_H_)
#define __SAIEXPERIMENTALDASHAPPLIANCE_H_

#include <saitypesextensions.h>

/**
 * @defgroup SAIEXPERIMENTALDASHAPPLIANCE SAI - Experimental: DASH appliance specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for DASH appliance
 */
typedef enum _sai_dash_appliance_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DASH_APPLIANCE_ATTR_START,

    /**
     * @brief Action parameter local region id
     *
     * @type sai_uint8_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_DASH_APPLIANCE_ATTR_LOCAL_REGION_ID = SAI_DASH_APPLIANCE_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_DASH_APPLIANCE_ATTR_END,

    /** Custom range base value */
    SAI_DASH_APPLIANCE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DASH_APPLIANCE_ATTR_CUSTOM_RANGE_END,

} sai_dash_appliance_attr_t;

/**
 * @brief Create DASH appliance
 *
 * @param[out] dash_appliance_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_dash_appliance_fn)(
        _Out_ sai_object_id_t *dash_appliance_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove DASH appliance
 *
 * @param[in] dash_appliance_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_dash_appliance_fn)(
        _In_ sai_object_id_t dash_appliance_id);

/**
 * @brief Set attribute for DASH appliance
 *
 * @param[in] dash_appliance_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_dash_appliance_attribute_fn)(
        _In_ sai_object_id_t dash_appliance_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for DASH appliance
 *
 * @param[in] dash_appliance_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_dash_appliance_attribute_fn)(
        _In_ sai_object_id_t dash_appliance_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_dash_appliance_api_t
{
    sai_create_dash_appliance_fn           create_dash_appliance;
    sai_remove_dash_appliance_fn           remove_dash_appliance;
    sai_set_dash_appliance_attribute_fn    set_dash_appliance_attribute;
    sai_get_dash_appliance_attribute_fn    get_dash_appliance_attribute;
    sai_bulk_object_create_fn              create_dash_appliances;
    sai_bulk_object_remove_fn              remove_dash_appliances;

} sai_dash_appliance_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHAPPLIANCE_H_ */
