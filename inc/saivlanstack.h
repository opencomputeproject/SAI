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
 * @file    saivlanstack.h
 *
 * @brief   This module defines SAI VLAN STACK interface
 */

#if !defined (__SAIVLANSTACK_H_)
#define __SAIVLANSTACK_H_

#include <saitypes.h>

/**
 * @defgroup SAIVLANSTACK SAI - VLAN STACK specific API definitions
 *
 * @{
 */

/**
 * @brief VLAN Stack stage
 */
typedef enum _sai_vlan_stack_stage_t
{
    SAI_VLAN_STACK_STAGE_INGRESS,

    SAI_VLAN_STACK_STAGE_EGRESS,

} sai_vlan_stack_stage_t;

/**
 * @brief VLAN Stack action
 *
 * If the action is SAI_VLAN_STACK_ACTION_SWAP, the original vlan id of the frame will be replaced
 * with new vlan id and the new vlan id will be placed in the original tag position.
 * If the action is SAI_VLAN_STACK_ACTION_PUSH, the original vlan id of the frame will be kept if
 * exists and the new vlan id will be added as the outer vlan tag.
 * If the action is SAI_VLAN_STACK_ACTION_POP, the original vlan id of the frame will be removed
 * if exists.
 */
typedef enum _sai_vlan_stack_action_t
{
    SAI_VLAN_STACK_ACTION_SWAP,

    SAI_VLAN_STACK_ACTION_PUSH,

    SAI_VLAN_STACK_ACTION_POP,

} sai_vlan_stack_action_t;

/**
 * @brief Attributes for VLAN Stack
 */
typedef enum _sai_vlan_stack_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_VLAN_STACK_ATTR_START,

    /**
     * @brief Stage type
     * If action type is PUSH, this should set to INGRESS
     * If action type is POP, this should set to EGRESS
     *
     * @type sai_vlan_stack_stage_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_VLAN_STACK_ATTR_STAGE = SAI_VLAN_STACK_ATTR_START,

    /**
     * @brief Action type
     *
     * @type sai_vlan_stack_action_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_VLAN_STACK_ATTR_ACTION,

    /**
     * @brief Original Vlan ID of the inner
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     */
    SAI_VLAN_STACK_ATTR_ORIGINAL_VLAN_ID_INNER,

    /**
     * @brief Original Vlan ID of the outer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     */
    SAI_VLAN_STACK_ATTR_ORIGINAL_VLAN_ID_OUTER,

    /**
     * @brief VLAN Stack port object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG
     */
    SAI_VLAN_STACK_ATTR_PORT,

    /**
     * @brief Applied Vlan ID of the inner
     * If action type is POP, should not set this attribute
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     * @validonly SAI_VLAN_STACK_ATTR_ACTION == SAI_VLAN_STACK_ACTION_PUSH or SAI_VLAN_STACK_ATTR_ACTION == SAI_VLAN_STACK_ACTION_SWAP
     */
    SAI_VLAN_STACK_ATTR_APPLIED_VLAN_ID_INNER,

    /**
     * @brief Applied Vlan ID of the outer
     * If action type is POP, should not set this attribute
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     * @validonly SAI_VLAN_STACK_ATTR_ACTION == SAI_VLAN_STACK_ACTION_PUSH or SAI_VLAN_STACK_ATTR_ACTION == SAI_VLAN_STACK_ACTION_SWAP
     */
    SAI_VLAN_STACK_ATTR_APPLIED_VLAN_ID_OUTER,

    /**
     * @brief The packet priority (3 bits) in the vlan tag
     * Range from 0 to 7, priority from low to high.
     * Default 0xFF will inherit the original vlan tag priority
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0xFF
     */
    SAI_VLAN_STACK_ATTR_VLAN_APPLIED_PRI,

    /**
     * @brief End of attributes
     */
    SAI_VLAN_STACK_ATTR_END,

    /** Custom range base value */
    SAI_VLAN_STACK_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_VLAN_STACK_ATTR_CUSTOM_RANGE_END

} sai_vlan_stack_attr_t;

/**
 * @brief Create and apply a single VLAN Stack rule
 *
 * @param[out] vlan_stack_id VLAN Stack ID
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_vlan_stack_fn)(
        _Out_ sai_object_id_t *vlan_stack_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove a VLAN Stack rule
 *
 * @param[in] vlan_stack_id VLAN Stack ID
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_vlan_stack_fn)(
        _In_ sai_object_id_t vlan_stack_id);

/**
 * @brief Modify VLAN Stack rule attribute
 *
 * @param[in] vlan_stack_id VLAN Stack ID
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_vlan_stack_attribute_fn)(
        _In_ sai_object_id_t vlan_stack_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get VLAN Stack rule attribute
 *
 * @param[in] vlan_stack_id VLAN Stack ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_vlan_stack_attribute_fn)(
        _In_ sai_object_id_t vlan_stack_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief VLAN STACK methods table retrieved with sai_api_query()
 */
typedef struct _sai_vlan_stack_api_t
{
    sai_create_vlan_stack_fn            create_vlan_stack;
    sai_remove_vlan_stack_fn            remove_vlan_stack;
    sai_set_vlan_stack_attribute_fn     set_vlan_stack_attribute;
    sai_get_vlan_stack_attribute_fn     get_vlan_stack_attribute;
} sai_vlan_stack_api_t;

/**
 * @}
 */
#endif /** __SAIVLANSTACK_H_ */
