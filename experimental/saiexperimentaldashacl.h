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
 * @brief Attribute data for #SAI_DASH_ACL_RULE_ATTR_ACTION
 */
typedef enum _sai_dash_acl_rule_action_t
{
    SAI_DASH_ACL_RULE_ACTION_PERMIT,

    SAI_DASH_ACL_RULE_ACTION_PERMIT_AND_CONTINUE,

    SAI_DASH_ACL_RULE_ACTION_DENY,

    SAI_DASH_ACL_RULE_ACTION_DENY_AND_CONTINUE,

} sai_dash_acl_rule_action_t;

/**
 * @brief Attribute ID for dash_acl_dash_acl_group
 */
typedef enum _sai_dash_acl_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DASH_ACL_GROUP_ATTR_START,

    /**
     * @brief Action set_acl_group_attrs parameter IP_ADDR_FAMILY
     *
     * @type sai_ip_addr_family_t
     * @flags CREATE_AND_SET
     * @default SAI_IP_ADDR_FAMILY_IPV4
     * @isresourcetype true
     */
    SAI_DASH_ACL_GROUP_ATTR_IP_ADDR_FAMILY = SAI_DASH_ACL_GROUP_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_DASH_ACL_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_DASH_ACL_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DASH_ACL_GROUP_ATTR_CUSTOM_RANGE_END,

} sai_dash_acl_group_attr_t;

/**
 * @brief Attribute ID for dash_acl_dash_acl_rule
 */
typedef enum _sai_dash_acl_rule_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DASH_ACL_RULE_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_dash_acl_rule_action_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_ACL_RULE_ACTION_PERMIT
     */
    SAI_DASH_ACL_RULE_ATTR_ACTION = SAI_DASH_ACL_RULE_ATTR_START,

    /**
     * @brief Exact matched key dash_acl_group_id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @isresourcetype true
     */
    SAI_DASH_ACL_RULE_ATTR_DASH_ACL_GROUP_ID,

    /**
     * @brief Ternary matched key dst_tag
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_RULE_ATTR_DST_TAG,

    /**
     * @brief Ternary matched mask dst_tag
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_RULE_ATTR_DST_TAG_MASK,

    /**
     * @brief Ternary matched key src_tag
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_RULE_ATTR_SRC_TAG,

    /**
     * @brief Ternary matched mask src_tag
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_RULE_ATTR_SRC_TAG_MASK,

    /**
     * @brief List matched key dip
     *
     * @type sai_ip_prefix_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_RULE_ATTR_DIP,

    /**
     * @brief List matched key sip
     *
     * @type sai_ip_prefix_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_RULE_ATTR_SIP,

    /**
     * @brief List matched key protocol
     *
     * @type sai_u8_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_RULE_ATTR_PROTOCOL,

    /**
     * @brief Range_list matched key src_port
     *
     * @type sai_u16_range_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_RULE_ATTR_SRC_PORT,

    /**
     * @brief Range_list matched key dst_port
     *
     * @type sai_u16_range_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_RULE_ATTR_DST_PORT,

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
    SAI_DASH_ACL_RULE_ATTR_COUNTER_ID,

    /**
     * @brief Rule priority in table
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_RULE_ATTR_PRIORITY,

    /**
     * @brief IP address family for resource accounting
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_DASH_ACL_RULE_ATTR_IP_ADDR_FAMILY,

    /**
     * @brief End of attributes
     */
    SAI_DASH_ACL_RULE_ATTR_END,

    /** Custom range base value */
    SAI_DASH_ACL_RULE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DASH_ACL_RULE_ATTR_CUSTOM_RANGE_END,

} sai_dash_acl_rule_attr_t;

/**
 * @brief Create dash_acl_dash_acl_group
 *
 * @param[out] dash_acl_group_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_dash_acl_group_fn)(
        _Out_ sai_object_id_t *dash_acl_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_acl_dash_acl_group
 *
 * @param[in] dash_acl_group_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_dash_acl_group_fn)(
        _In_ sai_object_id_t dash_acl_group_id);

/**
 * @brief Set attribute for dash_acl_dash_acl_group
 *
 * @param[in] dash_acl_group_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_dash_acl_group_attribute_fn)(
        _In_ sai_object_id_t dash_acl_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_acl_dash_acl_group
 *
 * @param[in] dash_acl_group_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_dash_acl_group_attribute_fn)(
        _In_ sai_object_id_t dash_acl_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create dash_acl_dash_acl_rule
 *
 * @param[out] dash_acl_rule_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_dash_acl_rule_fn)(
        _Out_ sai_object_id_t *dash_acl_rule_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_acl_dash_acl_rule
 *
 * @param[in] dash_acl_rule_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_dash_acl_rule_fn)(
        _In_ sai_object_id_t dash_acl_rule_id);

/**
 * @brief Set attribute for dash_acl_dash_acl_rule
 *
 * @param[in] dash_acl_rule_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_dash_acl_rule_attribute_fn)(
        _In_ sai_object_id_t dash_acl_rule_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_acl_dash_acl_rule
 *
 * @param[in] dash_acl_rule_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_dash_acl_rule_attribute_fn)(
        _In_ sai_object_id_t dash_acl_rule_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_dash_acl_api_t
{
    sai_create_dash_acl_group_fn           create_dash_acl_group;
    sai_remove_dash_acl_group_fn           remove_dash_acl_group;
    sai_set_dash_acl_group_attribute_fn    set_dash_acl_group_attribute;
    sai_get_dash_acl_group_attribute_fn    get_dash_acl_group_attribute;
    sai_bulk_object_create_fn              create_dash_acl_groups;
    sai_bulk_object_remove_fn              remove_dash_acl_groups;

    sai_create_dash_acl_rule_fn            create_dash_acl_rule;
    sai_remove_dash_acl_rule_fn            remove_dash_acl_rule;
    sai_set_dash_acl_rule_attribute_fn     set_dash_acl_rule_attribute;
    sai_get_dash_acl_rule_attribute_fn     get_dash_acl_rule_attribute;
    sai_bulk_object_create_fn              create_dash_acl_rules;
    sai_bulk_object_remove_fn              remove_dash_acl_rules;

} sai_dash_acl_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHACL_H_ */
