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
 * @file    saiexperimentaldashacl.h
 *
 * @brief   This module defines SAI P4 extension  interface
 */

#if !defined (__SAIEXPERIMENTALDASHACL_H_)
#define __SAIEXPERIMENTALDASHACL_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALDASH_ACL SAI - Extension specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_DASH_ACL_ATTR_ACTION
 */
typedef enum _sai_dash_acl_action_t
{
    SAI_DASH_ACL_ACTION_PERMIT,

    SAI_DASH_ACL_ACTION_PERMIT_AND_CONTINUE,

    SAI_DASH_ACL_ACTION_DENY,

    SAI_DASH_ACL_ACTION_DENY_AND_CONTINUE,

} sai_dash_acl_action_t;

/**
 * @brief Attribute data for #SAI_DASH_ACL_ATTR_STAGE
 */
typedef enum _sai_dash_acl_stage_t
{
    SAI_DASH_ACL_STAGE_INBOUND_ACL_STAGE1,

    SAI_DASH_ACL_STAGE_INBOUND_ACL_STAGE2,

    SAI_DASH_ACL_STAGE_INBOUND_ACL_STAGE3,

    SAI_DASH_ACL_STAGE_OUTBOUND_ACL_STAGE1,

    SAI_DASH_ACL_STAGE_OUTBOUND_ACL_STAGE2,

    SAI_DASH_ACL_STAGE_OUTBOUND_ACL_STAGE3,

} sai_dash_acl_stage_t;

/**
 * @brief Attribute ID for dash_acl_dash_acl
 */
typedef enum _sai_dash_acl_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DASH_ACL_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_dash_acl_action_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_ACL_ACTION_PERMIT
     */
    SAI_DASH_ACL_ATTR_ACTION = SAI_DASH_ACL_ATTR_START,

    /**
     * @brief Exact matched key eni_id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_ENI
     */
    SAI_DASH_ACL_ATTR_ENI_ID,

    /**
     * @brief List matched key dip
     *
     * @type sai_ip_address_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_ATTR_DIP,

    /**
     * @brief List matched key sip
     *
     * @type sai_ip_address_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_ATTR_SIP,

    /**
     * @brief List matched key protocol
     *
     * @type sai_u8_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_ATTR_PROTOCOL,

    /**
     * @brief Range_list matched key src_port
     *
     * @type sai_u16_range_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_ATTR_SRC_PORT,

    /**
     * @brief Range_list matched key dst_port
     *
     * @type sai_u16_range_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_ATTR_DST_PORT,

    /**
     * @brief Stage
     *
     * @type sai_dash_acl_stage_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_ACL_STAGE_INBOUND_ACL_STAGE1
     */
    SAI_DASH_ACL_ATTR_STAGE,

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
    SAI_DASH_ACL_ATTR_COUNTER_ID,

    /**
     * @brief Rule priority in table
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_ATTR_PRIORITY,

    /**
     * @brief End of attributes
     */
    SAI_DASH_ACL_ATTR_END,

    /** Custom range base value */
    SAI_DASH_ACL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DASH_ACL_ATTR_CUSTOM_RANGE_END,

} sai_dash_acl_attr_t;

/**
 * @brief Create dash_acl_dash_acl
 *
 * @param[out] dash_acl_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_dash_acl_fn)(
        _Out_ sai_object_id_t *dash_acl_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_acl_dash_acl
 *
 * @param[in] dash_acl_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_dash_acl_fn)(
        _In_ sai_object_id_t dash_acl_id);

/**
 * @brief Set attribute for dash_acl_dash_acl
 *
 * @param[in] dash_acl_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_dash_acl_attribute_fn)(
        _In_ sai_object_id_t dash_acl_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_acl_dash_acl
 *
 * @param[in] dash_acl_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_dash_acl_attribute_fn)(
        _In_ sai_object_id_t dash_acl_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_dash_acl_api_t
{
    sai_create_dash_acl_fn           create_dash_acl;
    sai_remove_dash_acl_fn           remove_dash_acl;
    sai_set_dash_acl_attribute_fn    set_dash_acl_attribute;
    sai_get_dash_acl_attribute_fn    get_dash_acl_attribute;
    sai_bulk_object_create_fn        create_dash_acls;
    sai_bulk_object_remove_fn        remove_dash_acls;

} sai_dash_acl_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHACL_H_ */
