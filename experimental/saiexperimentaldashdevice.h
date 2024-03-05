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
 * @file    saiexperimentaldashdevice.h
 *
 * @brief   This module defines SAI P4 extension  interface
 */

#if !defined (__SAIEXPERIMENTALDASHDEVICE_H_)
#define __SAIEXPERIMENTALDASHDEVICE_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALDASH_DEVICE SAI - Extension specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for DASH device
 */
typedef enum _sai_dash_device_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DASH_DEVICE_ATTR_START,

    /**
     * @brief Any non-VNET VNI or GRE keys that need to be allowed
     *
     * @par PrivateLink GRE key is an example
     *
     * @type sai_vni_range_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_DASH_DEVICE_ATTR_TRUSTED_VNI_LIST = SAI_DASH_DEVICE_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_DASH_DEVICE_ATTR_END,

    /** Custom range base value */
    SAI_DASH_DEVICE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DASH_DEVICE_ATTR_CUSTOM_RANGE_END

} sai_dash_device_attr_t;

/**
 * @brief Create and return a DASH device object
 *
 * @param[out] dash_device_id Device id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_dash_device_fn)(
        _Out_ sai_object_id_t *dash_device_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove DASH device object
 *
 * @param[in] dash_device_id Device id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_dash_device_fn)(
        _In_ sai_object_id_t dash_device_id);

/**
 * @brief Set DASH device attribute
 *
 * @param[in] dash_device_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_dash_device_attribute_fn)(
        _In_ sai_object_id_t dash_device_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get DASH device attribute
 *
 * @param[in] dash_device_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_dash_device_attribute_fn)(
        _In_ sai_object_id_t dash_device_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_dash_device_api_t
{
    sai_create_dash_device_fn                           create_dash_device;
    sai_remove_dash_device_fn                           remove_dash_device;
    sai_set_dash_device_attribute_fn                    set_dash_device_attribute;
    sai_get_dash_device_attribute_fn                    get_dash_device_attribute;

} sai_dash_device_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHDEVICE_H_ */
