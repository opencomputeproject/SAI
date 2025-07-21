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
 * @file    saisrv6custom.h
 *
 * @brief   This module defines SAI SRV6 custom interface
 */

#if !defined (__SAISRVCUSTOM_H_)
#define __SAISRVCUSTOM_H_

#include <saisrv6.h>

/**
 * @brief Custom Attribute Id for My SID Entry Endpoint Behavior
 *
 * @flags free
 */
typedef enum _sai_my_sid_entry_endpoint_behavior_custom_t
{
    /**
     * @brief Custom range start of My SID Entry Endpoint Behavior
     */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_CUSTOM_RANGE_START = SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of My SID Entry Endpoint Behavior
     */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_CUSTOM_RANGE_END

} sai_my_sid_entry_endpoint_behavior_custom_t;

/**
 * @brief Custom srv6 sidlist type
 *
 * @flags free
 */
typedef enum _sai_srv6_sidlist_type_custom_t
{
    /**
     * @brief Custom range start of SRV6 SID List Type
     */
    SAI_SRV6_SIDLIST_TYPE_CUSTOM_RANGE_START = SAI_SRV6_SIDLIST_TYPE_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of SRV6 SID List Type
     */
    SAI_SRV6_SIDLIST_TYPE_CUSTOM_RANGE_END

} sai_srv6_sidlist_type_custom_t;

/**
 * @brief Custom Attribute Id for SRV6 SID List
 *
 * @flags free
 */
typedef enum _sai_srv6_sidlist_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_SRV6_SIDLIST_ATTR_CUSTOM_RANGE_START = SAI_SRV6_SIDLIST_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of SRV6 SID List attributes
     */
    SAI_SRV6_SIDLIST_ATTR_CUSTOM_RANGE_END

} sai_srv6_sidlist_attr_custom_t;

/**
 * @brief Custom Attribute Id for My SID Entry
 *
 * @flags free
 */
typedef enum _sai_my_sid_entry_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_MY_SID_ENTRY_ATTR_CUSTOM_RANGE_START = SAI_MY_SID_ENTRY_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of My SID Entry attributes
     */
    SAI_MY_SID_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_my_sid_entry_attr_custom_t;

#endif /* __SAISRVCUSTOM_H_ */