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
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc
 *
 * @file    saiqosmaps.h
 *
 * @brief   This module defines SAI QOS Maps interface
 */

#if !defined (__SAIQOSMAPS_H_)
#define __SAIQOSMAPS_H_

#include <saitypes.h>

/**
 * @defgroup SAIQOSMAPS SAI - Qos Maps specific API definitions.
 *
 * @{
 */

/**
 * @brief Enum defining qos map types.
 */
typedef enum _sai_qos_map_type_t
{
    /** Qos Map to set DOT1P to Traffic class*/
    SAI_QOS_MAP_TYPE_DOT1P_TO_TC = 0x00000000,

    /** Qos Map to set DOT1P to color*/
    SAI_QOS_MAP_TYPE_DOT1P_TO_COLOR = 0x00000001,

    /** Qos Map to set DSCP to Traffic class*/
    SAI_QOS_MAP_TYPE_DSCP_TO_TC = 0x00000002,

    /** Qos Map to set DSCP to color*/
    SAI_QOS_MAP_TYPE_DSCP_TO_COLOR = 0x00000003,

    /** Qos Map to set traffic class to queue */
    SAI_QOS_MAP_TYPE_TC_TO_QUEUE = 0x00000004,

    /** Qos Map to set traffic class and color to DSCP */
    SAI_QOS_MAP_TYPE_TC_AND_COLOR_TO_DSCP = 0x00000005,

    /** Qos Map to set traffic class and color to DSCP */
    SAI_QOS_MAP_TYPE_TC_AND_COLOR_TO_DOT1P = 0x00000006,

    /** Qos Map to set traffic class to priority group */
    SAI_QOS_MAP_TYPE_TC_TO_PRIORITY_GROUP = 0x00000007,

    /** Qos Map to set PFC priority to priority group */
    SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_PRIORITY_GROUP = 0x00000008,

    /** Qos Map to set PFC priority to queue */
    SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_QUEUE = 0x00000009,

    /** Custom range base value */
    SAI_QOS_MAP_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_qos_map_type_t;

/**
 * @brief Enum defining attributes for Qos Maps.
 */
typedef enum _sai_qos_map_attr_t
{
    /**
     * Start of attributes
     */
    SAI_QOS_MAP_ATTR_START,

    /**
     * @brief Qos Map type
     *
     * @type sai_qos_map_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_QOS_MAP_ATTR_TYPE = SAI_QOS_MAP_ATTR_START,

    /**
     * @brief Dot1p to TC Mapping
     *
     * Defaults:
     * - All Dot1p/DSCP maps to traffic class 0
     * - All Dot1p/DSCP maps to color #SAI_PACKET_COLOR_GREEN
     * - All traffic class maps to queue 0
     *
     *
     * @type sai_qos_map_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_QOS_MAP_ATTR_MAP_TO_VALUE_LIST = 0x00000001,

    /**
     * @brief End of attributes
     */
    SAI_QOS_MAP_ATTR_END,

    /** Custom range base value */
    SAI_QOS_MAP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** Endo of custom range base */
    SAI_QOS_MAP_ATTR_CUSTOM_RANGE_END

} sai_qos_map_attr_t ;

/**
 * @brief Create Qos Map
 *
 * @param[out] qos_map_id Qos Map Id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_qos_map_fn)(
        _Out_ sai_object_id_t *qos_map_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Qos Map
 *
 * @param[in] qos_map_id Qos Map id to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_qos_map_fn) (
        _In_ sai_object_id_t qos_map_id);

/**
 * @brief Set attributes for qos map
 *
 * @param[in] qos_map_id Qos Map Id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_qos_map_attribute_fn)(
        _In_ sai_object_id_t qos_map_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attrbutes of qos map
 *
 * @param[in] qos_map_id Map id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_qos_map_attribute_fn)(
        _In_ sai_object_id_t qos_map_id ,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Qos Map methods table retrieved with sai_api_query()
 */
typedef struct _sai_qos_map_api_t
{
    sai_create_qos_map_fn         create_qos_map;
    sai_remove_qos_map_fn         remove_qos_map;
    sai_set_qos_map_attribute_fn  set_qos_map_attribute;
    sai_get_qos_map_attribute_fn  get_qos_map_attribute;

} sai_qos_map_api_t;

/**
 * @}
 */
#endif /** __SAIQOSMAPS_H_ */
