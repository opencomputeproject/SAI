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
 * @file    saiars.h
 *
 * @brief   This module defines SAI interface for adaptive routing and switching
 */

#if !defined (__SAIARS_H_)
#define __SAIARS_H_

#include <saitypes.h>

/**
 * @defgroup SAIARS SAI - Adaptive Routing and Switching specific API definitions
 *
 * @{
 */

/**
 * @brief Adaptive routing and switching path (re)assignment mode
 */
typedef enum _sai_ars_mode_t
{
    /** Per flow-let quality based path (re)assignment */
    SAI_ARS_MODE_FLOWLET_QUALITY,

    /** Per flow-let random path (re)assignment */
    SAI_ARS_MODE_FLOWLET_RANDOM,

    /** Per packet quality based path (re)assignment */
    SAI_ARS_MODE_PER_PACKET_QUALITY,

    /** Per packet random path (re)assignment */
    SAI_ARS_MODE_PER_PACKET_RANDOM,

    /** Fixed path assignment */
    SAI_ARS_MODE_FIXED,

} sai_ars_mode_t;

/**
 * @brief Attribute id for adaptive routing and switching
 */
typedef enum _sai_ars_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_ARS_ATTR_START,

    /**
     * @brief ARS path assignment mode
     *
     * @type sai_ars_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_ARS_MODE_FLOWLET_QUALITY
     */
    SAI_ARS_ATTR_MODE = SAI_ARS_ATTR_START,

    /**
     * @brief Idle duration in microseconds. This duration is to classifying a flow-let in a macro flow.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 256
     */
    SAI_ARS_ATTR_IDLE_TIME,

    /**
     * @brief Maximum number of flow states that can be maintained per this ARS object
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 512
     */
    SAI_ARS_ATTR_MAX_FLOWS,

    /**
     * @brief ARS monitoring
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ARS_ATTR_MON_ENABLE,

    /**
     * @brief Enable/Disable ARS Samplepacket session
     *
     * Enable ARS sampling by assigning samplepacket object id.
     * Disable ARS sampling by assigning #SAI_NULL_OBJECT_ID as attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_SAMPLEPACKET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ARS_ATTR_SAMPLEPACKET_ENABLE,

    /**
     * @brief Maximum number of alternate members per adaptive routing group
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 16
     */
    SAI_ARS_ATTR_MAX_ALT_MEMEBERS_PER_GROUP,

    /**
     * @brief Maximum number of primary members per adaptive routing group
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 16
     */
    SAI_ARS_ATTR_MAX_PRIMARY_MEMEBERS_PER_GROUP,

    /**
     * @brief Quality threshold for least cost ARS paths. Crossing down the threshold will result in using the non least cost sub optimal path.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 16
     */
    SAI_ARS_ATTR_PRIMARY_PATH_QUALITY_THRESHOLD,

    /**
     * @brief Cost of switching over to non least cost ARS paths
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_ATTR_ALTERNATE_PATH_COST,

    /**
     * @brief Indicates the bias in favor of alternate path
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_ATTR_ALTERNATE_PATH_BIAS,

    /**
     * @brief End of attributes
     */
    SAI_ARS_ATTR_END,

    /** Custom range base value */
    SAI_ARS_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_ARS_ATTR_CUSTOM_RANGE_END

} sai_ars_attr_t;

/**
 * @brief Create adaptive routing and switching object
 *
 * @param[out] ars_id Adaptive routing and switching id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_ars_fn)(
        _Out_ sai_object_id_t *ars_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove adaptive routing and switching object
 *
 * @param[in] ars_id Adaptive routing and switching id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_ars_fn)(
        _In_ sai_object_id_t ars_id);

/**
 * @brief Set Adaptive routing and switching attribute
 *
 * @param[in] ars_id Adaptive routing and switching id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_ars_attribute_fn)(
        _In_ sai_object_id_t ars_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Adaptive routing and switching attribute
 *
 * @param[in] ars_id Adaptive routing and switching id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ars_attribute_fn)(
        _In_ sai_object_id_t ars_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Adaptive routing and switching methods table retrieved with sai_api_query()
 */
typedef struct _sai_ars_api_t
{
    sai_create_ars_fn               create_ars;
    sai_remove_ars_fn               remove_ars;
    sai_set_ars_attribute_fn        set_ars_attribute;
    sai_get_ars_attribute_fn        get_ars_attribute;
} sai_ars_api_t;

/**
 * @}
 */
#endif /** __SAIARS_H_ */
