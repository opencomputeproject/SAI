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
 * @file    saineighborcustom.h
 *
 * @brief   This module defines SAI Neighbor custom interface
 */

#if !defined (__SAINEIGHBOR_CUSTOM_H_)
#define __SAINEIGHBOR_CUSTOM_H_

#include <saineighbor.h>

/**
 * @brief Custom Attribute Id for Neighbor Entry
 *
 * @flags free
 */
typedef enum _sai_neighbor_entry_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_NEIGHBOR_ENTRY_ATTR_CUSTOM_RANGE_START = SAI_NEIGHBOR_ENTRY_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of Neighbor Entry attributes
     */
    SAI_NEIGHBOR_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_neighbor_entry_attr_custom_t;

#endif /* __SAINEIGHBOR_CUSTOM_H_ */