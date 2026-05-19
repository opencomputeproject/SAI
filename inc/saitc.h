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
 * @file    saitc.h
 *
 * @brief   This module defines SAI QOS TC interface
 */

#if !defined (__SAITC_H_)
#define __SAITC_H_

#include <saitypes.h>

/**
 * @defgroup SAITC SAI - TC specific API definitions
 *
 * @{
 */

/**
 * @brief Enum defining TC attributes.
 */
typedef enum _sai_tc_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_TC_ATTR_START = 0x00000000,

    /**
     * @brief Traffic class index
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     */
    SAI_TC_ATTR_INDEX = SAI_TC_ATTR_START,

    /**
     * @brief Flood Control Enable
     * Enable flood control on traffic class for broadcast,
     * unknown unicast and multicast traffic
     *
     * Use SAI_HOSTIF_TRAP_TYPE_TC_FLOOD_CONTROL trap type
     * to apply flood control on the TC
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TC_ATTR_FLOOD_CONTROL_ENABLE,

    /**
     * @brief TC Bind point for TAM object
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     * @default empty
     */
    SAI_TC_ATTR_ATTR_TAM_OBJECT,

    /**
     * @brief End of attributes
     */
    SAI_TC_ATTR_END,

    /** Custom range base value */
    SAI_TC_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TC_ATTR_CUSTOM_RANGE_END

} sai_tc_attr_t;

/**
 * @brief Create traffic class
 *
 * @param[out] tc_id TC id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tc_fn)(
        _Out_ sai_object_id_t *tc_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove traffic class
 *
 * @param[in] tc_id TC id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tc_fn)(
        _In_ sai_object_id_t tc_id);

/**
 * @brief Set TC attribute value.
 *
 * @param[in] tc_id TC id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tc_attribute_fn)(
        _In_ sai_object_id_t tc_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get TC attribute value.
 *
 * @param[in] tc_id TC id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tc_attribute_fn)(
        _In_ sai_object_id_t tc_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Traffic class methods table retrieved with sai_api_query()
 */
typedef struct _sai_tc_api_t
{
    sai_create_tc_fn                     create_tc;
    sai_remove_tc_fn                     remove_tc;
    sai_set_tc_attribute_fn              set_tc_attribute;
    sai_get_tc_attribute_fn              get_tc_attribute;

} sai_tc_api_t;

/**
 * @}
 */
#endif /** __SAITC_H_ */
