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
 * @file    saiaclextensions.h
 *
 * @brief   This module defines ACL extensions of the Switch Abstraction Interface (SAI)
 */

#if !defined (__SAIACLEXTENSIONS_H_)
#define __SAIACLEXTENSIONS_H_

#include <saiacl.h>
#include <saitypesextensions.h>

/**
 * @brief Attribute Id for sai_acl_table
 *
 * @flags Contains flags
 */
typedef enum _sai_acl_table_attr_extensions_t
{
    SAI_ACL_TABLE_ATTR_EXTENSIONS_RANGE_START = SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_END,

    /**
     * @brief Table priority
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_ACL_TABLE_ATTR_EXTENSIONS_PRIORITY,

    /**
     * @brief Packet is flagged to be dropped in pipeline.
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_EXTENSIONS_FIELD_DROP_MARKED,

    /**
     * @brief Table group id
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_ACL_TABLE_ATTR_EXTENSIONS_GROUP_ID,

    /**
     * @brief Number of used entries for all pipes
     *        1st entry in the list points to pipe-0,next points to pipe-1 and so on.
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_ACL_TABLE_ATTR_EXTENSIONS_USED_ACL_ENTRY_LIST,

    /**
     * @brief Number of free entry space available in
     *        the current table for all pipes
     *        1st entry in the list points to pipe-0,next points to pipe-1 and so on.
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_ACL_TABLE_ATTR_EXTENSIONS_AVAILABLE_ACL_ENTRY_LIST,

    SAI_ACL_TABLE_ATTR_EXTENSIONS_RANGE_END

} sai_acl_table_attr_extensions_t;

#endif /** __SAIACLEXTENSIONS_H_ */
