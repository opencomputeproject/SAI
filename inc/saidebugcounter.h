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
 * @file    saicounter.h
 *
 * @brief   This module defines SAI Counter interface
 *
 * @par Abstract
 *
 *    This module defines SAI Counter API.
 */

#if !defined (__SAIDEBUGCOUNTER_H_)
#define __SAIDEBUGCOUNTER_H_

#include <saitypes.h>

/**
 * @defgroup SAIDEBUGCOUNTER SAI - Debug counter specific API definitions
 *
 * @{
 */

/**
 * @brief Debug counter type
 */
typedef enum _sai_debug_counter_type_t
{
    /** Port in drop reasons */
    SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,

    /** Port out drop reasons */
    SAI_DEBUG_COUNTER_TYPE_PORT_OUT_DROP_REASONS,

} sai_debug_counter_type_t;

/**
 * @brief Attribute data for port in drop reasons
 */
typedef enum _sai_port_in_drop_reason_t
{
    /* L2 reasons */

    /** Source MAC is multicast */
    SAI_PORT_IN_DROP_REASON_SMAC_MULTICAST,

    /** Source MAC equals Destination MAC */
    SAI_PORT_IN_DROP_REASON_SMAC_EQUALS_DMAC,

    /** Destination MAC is Reserved (DMAC=01-80-C2-00-00-0x) */
    SAI_PORT_IN_DROP_REASON_DMAC_RESERVED,

    /** 
     * @brief VLAN tag not allowed
     *
     * Frame tagged when port is dropping tagged, 
     * or untagged when dropping untagged
     */
    SAI_PORT_IN_DROP_REASON_VLAN_TAG_NOT_ALLOWED,

    /** Ingress VLAN filter */
    SAI_PORT_IN_DROP_REASON_INGRESS_VLAN_FILTER,

    /** Ingress STP filter */
    SAI_PORT_IN_DROP_REASON_INGRESS_STP_FILTER,

    /** Unicast FDB table action discard */
    SAI_PORT_IN_DROP_REASON_FDB_UC_DISCARD,

    /** Multicast FDB table empty tx list */
    SAI_PORT_IN_DROP_REASON_FDB_MC_DISCARD,

    /** Port loopback filter */
    SAI_PORT_IN_DROP_REASON_LOOPBACK_FILTER,

    /* L3 reasons */

} sai_port_in_drop_reason_t;

/**
 * @brief Attribute data for port out drop reasons
 */
typedef enum _sai_port_out_drop_reason_t
{
    /** Egress VLAN filter */
    SAI_PORT_OUT_DROP_REASON_EGRESS_VLAN_FILTER,

} sai_port_out_drop_reason_t;

/**
 * @brief Attribute Id in sai_set_counter_attribute() and
 * sai_get_counter_attribute() calls
 */
typedef enum _sai_debug_counter_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DEBUG_COUNTER_ATTR_START,

    /* READ-WRITE */

    /**
     * @brief Debug counter type
     *
     * @type sai_debug_counter_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isresourcetype true
     */
    SAI_DEBUG_COUNTER_ATTR_TYPE = SAI_DEBUG_COUNTER_ATTR_START,

    /**
     * @brief Object stat index
     * Index is added to base start
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DEBUG_COUNTER_ATTR_INDEX,

    /**
     * @brief List of port in drop reasons that will be counted
     *
     * @type sai_s32_list_t sai_port_in_drop_reason_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_DEBUG_COUNTER_ATTR_TYPE == 
     * SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS
     */
    SAI_DEBUG_COUNTER_ATTR_PORT_IN_DROP_REASON_LIST,

    /**
     * @brief List of port out drop reasons that will be counted
     *
     * @type sai_s32_list_t sai_port_out_drop_reason_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_DEBUG_COUNTER_ATTR_TYPE == 
     * SAI_DEBUG_COUNTER_TYPE_PORT_OUT_DROP_REASONS
     */
    SAI_DEBUG_COUNTER_ATTR_PORT_OUT_DROP_REASON_LIST,

    /**
     * @brief End of attributes
     */
    SAI_DEBUG_COUNTER_ATTR_END,

    /** Custom range base value */
    SAI_DEBUG_COUNTER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DEBUG_COUNTER_ATTR_CUSTOM_RANGE_END

} sai_debug_counter_attr_t;

/**
 * @brief Create debug counter
 *
 * @param[out] debug_counter_id Debug counter id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success
 */
typedef sai_status_t (*sai_create_debug_counter_fn)(
        _Out_ sai_object_id_t *debug_counter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove debug counter
 *
 * @param[in] debug_counter_id Debug counter id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_debug_counter_fn)(
        _In_ sai_object_id_t debug_counter_id);

/**
 * @brief Set debug counter attribute Value
 *
 * @param[in] debug_counter_id Debug counter id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_debug_counter_attribute_fn)(
        _In_ sai_object_id_t debug_counter_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get debug counter attribute Value
 *
 * @param[in] debug_counter_id Debug counter id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_debug_counter_attribute_fn)(
        _In_ sai_object_id_t debug_counter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Counter methods table retrieved with sai_api_query()
 */
typedef struct _sai_counter_api_t
{
    sai_create_debug_counter_fn        create_debug_counter;
    sai_remove_debug_counter_fn        remove_debug_counter;
    sai_set_debug_counter_attribute_fn set_debug_counter_attribute;
    sai_get_debug_counter_attribute_fn get_debug_counter_attribute;

} sai_counter_api_t;

/**
 * @}
 */
#endif /** __SAIDEBUGCOUNTER_H_ */
