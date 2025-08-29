/**
 * Copyright (c) 2018 Microsoft Open Technologies, Inc.
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
 * @brief   This module defines ACL custom of the Switch Abstraction Interface (SAI)
 */

#ifndef __SAIACLCUSTOM_H_
#define __SAIACLCUSTOM_H_

#include <saiacl.h>

/**
 * @brief Custom ACL Action Type
 *
 * @flags free
 */
typedef enum _sai_acl_action_type_custom_t {
    /** Start of custom action type. */
    SAI_ACL_ACTION_TYPE_CUSTOM_RANGE_START = SAI_ACL_ACTION_TYPE_CUSTOM_RANGE_BASE,
    /**
     * @brief ACTION TYPE 1
     */
    SAI_ACL_ACTION_TYPE_1 = SAI_ACL_ACTION_TYPE_CUSTOM_RANGE_START,

    /** End of custom action type. */
    SAI_ACL_ACTION_TYPE_CUSTOM_RANGE_END
} sai_acl_action_type_custom_t;

/**
 * @brief SAI ACL table attribute custom
 *
 * @flags free
 */
typedef enum _sai_acl_table_attr_custom_t {
    /**
     * @brief Start of ACL table custom attributes
     */
    SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_START = SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_BASE,
    /**
     * @brief Custom 1
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_1 = SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_START,

    SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_END,

    /**
     * @brief Start of ACL table custom field attributes
     */
    SAI_ACL_TABLE_ATTR_FIELD_CUSTOM_RANGE_START = SAI_ACL_TABLE_ATTR_FIELD_CUSTOM_RANGE_BASE,
    /**
     * @brief Custom 1
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_1 = SAI_ACL_TABLE_ATTR_FIELD_CUSTOM_RANGE_START,
    /**
     * @brief Custom 2
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_2,

    /** End of custom range base */
    SAI_ACL_TABLE_ATTR_FIELD_CUSTOM_RANGE_END
} sai_acl_table_attr_custom_t;

/**
 * @brief SAI ACL entry attribute custom,
 *
 * @flags free
 */
typedef enum _sai_acl_entry_attr_custom_t {
    /**
     * @brief Start of ACL entry custom field attributes
     */
    SAI_ACL_ENTRY_ATTR_FIELD_CUSTOM_RANGE_START = SAI_ACL_ENTRY_ATTR_FIELD_CUSTOM_RANGE_BASE,
    /**
     * @brief Custom 1
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_ENTRY_ATTR_FIELD_1 = SAI_ACL_ENTRY_ATTR_FIELD_CUSTOM_RANGE_START,
    /**
     * @brief Custom 2
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_ENTRY_ATTR_FIELD_2,

    /** End of custom range base */
    SAI_ACL_ENTRY_ATTR_FIELD_CUSTOM_RANGE_END,

    /**
     * @brief Start of ACL entry custom action attributes
     */
    SAI_ACL_ENTRY_ATTR_ACTION_CUSTOM_RANGE_START = SAI_ACL_ENTRY_ATTR_ACTION_CUSTOM_RANGE_BASE,
    /**
     * @brief Custom 1
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_ENTRY_ATTR_ACTION_1 = SAI_ACL_ENTRY_ATTR_ACTION_CUSTOM_RANGE_START,

    /** End of ACL entry custom action attributes */
    SAI_ACL_ENTRY_ATTR_ACTION_CUSTOM_RANGE_END
} sai_acl_entry_attr_custom_t;

#endif /* __SAIACLCUSTOM_H_ */