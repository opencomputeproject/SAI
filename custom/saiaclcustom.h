/**
 * Copyright (c) 2025 Microsoft Open Technologies, Inc.
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
 * @file    saiaclcustom.h
 *
 * @brief   This module defines SAI ACL custom interface
 */

#if !defined (__SAIACL_CUSTOM_H_)
#define __SAIACL_CUSTOM_H_

#include <saiacl.h>

/**
 * @brief Custom acl action type
 *
 * @flags free
 */
typedef enum _sai_acl_action_type_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_ACL_ACTION_TYPE_CUSTOM_RANGE_START = SAI_ACL_ACTION_TYPE_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of acl action type
     */
    SAI_ACL_ACTION_TYPE_CUSTOM_RANGE_END

} sai_acl_action_type_custom_t;

/**
 * @brief Custom Attribute Id for acl_table_group
 *
 * @flags free
 */
typedef enum _sai_acl_table_group_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_ACL_TABLE_GROUP_ATTR_CUSTOM_RANGE_START = SAI_ACL_TABLE_GROUP_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief End of Custom range base
     */
    SAI_ACL_TABLE_GROUP_ATTR_CUSTOM_RANGE_END

} sai_acl_table_group_attr_custom_t;

/** 
 * @brief Custom Attribute Id for acl_table_chain_group
 *
 * @flags free
 */
typedef enum _sai_acl_table_chain_group_attr_custom_t {
    /**
     * @brief Custom range base value start
     */
    SAI_ACL_TABLE_CHAIN_GROUP_ATTR_CUSTOM_RANGE_START = SAI_ACL_TABLE_CHAIN_GROUP_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief End of Custom range base
     */
    SAI_ACL_TABLE_CHAIN_GROUP_ATTR_CUSTOM_RANGE_END

} sai_acl_table_chain_group_attr_custom_t;

/**
 * @brief Custom attribute structure for ACL table group member
 *
 * @flags free
 */
typedef enum _sai_acl_table_group_member_attr_custom_t{
    /**
     * @brief Custom range base value start
     */
    SAI_ACL_TABLE_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START = SAI_ACL_TABLE_GROUP_MEMBER_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief End of Custom range base
     */
    SAI_ACL_TABLE_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END

} sai_acl_table_group_member_attr_custom_t;

/**
 * @brief Custom attribute structure for ACL table chain group
 *
 * @flags free
 */
typedef enum {
    /**
     * @brief Custom range start of ACL table attributes
     */
    SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_START = SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of ACL table attributes
     */
    SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_END,

    /**
     * @brief Custom range start of ACL table field attributes
     */
    SAI_ACL_TABLE_ATTR_FIELD_CUSTOM_RANGE_START = SAI_ACL_TABLE_ATTR_FIELD_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of ACL table field attributes
     */
    SAI_ACL_TABLE_ATTR_FIELD_CUSTOM_RANGE_END

} sai_acl_table_attr_custom_t;

/**
 * @brief Custom attribute structure for ACL entry
 *
 * @flags free
 */
typedef enum _sai_acl_entry_attr_custom_t {
    /**
     * @brief Custom range start of ACL entry attributes
     */
    SAI_ACL_ENTRY_ATTR_CUSTOM_RANGE_START = SAI_ACL_ENTRY_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of ACL entry attributes
     */
    SAI_ACL_ENTRY_ATTR_CUSTOM_RANGE_END,

    /**
     * @brief Custom range start of ACL entry field attributes
     */
    SAI_ACL_ENTRY_ATTR_FIELD_CUSTOM_RANGE_START = SAI_ACL_ENTRY_ATTR_FIELD_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of ACL entry field attributes
     */
    SAI_ACL_ENTRY_ATTR_FIELD_CUSTOM_RANGE_END,

    /**
     * @brief Custom range start of ACL entry action attributes
     */
    SAI_ACL_ENTRY_ATTR_ACTION_CUSTOM_RANGE_START = SAI_ACL_ENTRY_ATTR_ACTION_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of ACL entry action attributes
     */
    SAI_ACL_ENTRY_ATTR_ACTION_CUSTOM_RANGE_END

} sai_acl_entry_attr_custom_t;

/**
 * @brief Custom attribute structure for ACL counter
 */
typedef enum _sai_acl_counter_attr_custom_t {
    /**
     * @brief Custom range base value start
     */
    SAI_ACL_COUNTER_ATTR_CUSTOM_RANGE_START = SAI_ACL_COUNTER_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief End of Custom range base
     */
    SAI_ACL_COUNTER_ATTR_CUSTOM_RANGE_END

} sai_acl_counter_attr_custom_t;

/**
 * @brief Custom attribute structure for ACL range
 */
typedef enum _sai_acl_range_attr_custom_t {
    /**
     * @brief Custom range base value start
     */
    SAI_ACL_RANGE_ATTR_CUSTOM_RANGE_START = SAI_ACL_RANGE_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief End of Custom range base
     */
    SAI_ACL_RANGE_ATTR_CUSTOM_RANGE_END

} sai_acl_range_attr_custom_t;

#endif /** __SAIACL_CUSTOM_H_ */