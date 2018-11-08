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
 * @file    saiextensionaclslice.h
 *
 * @brief   This module defines ACL slice extensions of the Switch Abstraction Interface (SAI)
 */

#if !defined (__SAIEXTENSIONACLSLICE_H_)
#define __SAIEXTENSIONACLSLICE_H_

#include <saitypesextensions.h>

/**
 * @brief Attribute Id for sai_acl_slice
 *
 * @flags Contains flags
 */
typedef enum _sai_acl_slice_attr_extensions_t
{
    /**
     * @brief Table attributes start
     */
    SAI_ACL_SLICE_ATTR_EXTENSIONS_RANGE_START,

    /**
     * @brief Get the ACL slice id
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_ACL_SLICE_ATTR_EXTENSIONS_SLICE_ID,

    /**
     * @brief Get the ACL slice pipe id
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_ACL_SLICE_ATTR_EXTENSIONS_SLICE_PIPE_ID,

    /**
     * @brief Get the ACL slice stage
     * @type sai_acl_stage_t
     * @flags READ_ONLY
     */
    SAI_ACL_SLICE_ATTR_EXTENSIONS_ACL_STAGE,

    /**
     * @brief Get the object_id list of the ACL table present
     *        in the current slice
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_ACL_TABLE
     * @default internal
     */
    SAI_ACL_SLICE_ATTR_EXTENSIONS_ACL_TABLE_LIST,

    /**
     * @brief Number of entries used in the slice
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_ACL_SLICE_ATTR_EXTENSIONS_USED_ACL_ENTRY,

    /**
     * @brief Number of free entry space available in
     *        the current slice
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_ACL_SLICE_ATTR_EXTENSIONS_AVAILABLE_ACL_ENTRY,

    /**
     * @brief End of ACL slice attributes
     */
    SAI_ACL_SLICE_ATTR_EXTENSIONS_RANGE_END,

} sai_acl_slice_attr_extensions_t;

#endif /** __SAIEXTENSIONACLSLICE_H_ */
