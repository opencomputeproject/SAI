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
 * @file    saihostifcustom.h
 *
 * @brief   This module defines SAI Host Interface custom interface
 */

#if !defined (__SAIHOSTIF_CUSTOM_H_)
#define __SAIHOSTIF_CUSTOM_H_

#include <saihostif.h>

/**
 * @brief Custom Attribute Id for HostIF Trap Group
 *
 * @flags free
 */
typedef enum _sai_hostif_trap_group_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_HOSTIF_TRAP_GROUP_ATTR_CUSTOM_RANGE_START = SAI_HOSTIF_TRAP_GROUP_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of HostIF Trap Group attributes
     */
    SAI_HOSTIF_TRAP_GROUP_ATTR_CUSTOM_RANGE_END

} sai_hostif_trap_group_attr_custom_t;

/**
 * @brief Custom Attribute Id for HostIF Trap
 *
 * @flags free
 */
typedef enum _sai_hostif_trap_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_HOSTIF_TRAP_ATTR_CUSTOM_RANGE_START = SAI_HOSTIF_TRAP_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of HostIF Trap attributes
     */
    SAI_HOSTIF_TRAP_ATTR_CUSTOM_RANGE_END

} sai_hostif_trap_attr_custom_t;

/**
 * @brief Custom Attribute Id for HostIF User Defined Trap
 *
 * @flags free
 */
typedef enum _sai_hostif_user_defined_trap_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_CUSTOM_RANGE_START = SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of HostIF User Defined Trap attributes
     */
    SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_CUSTOM_RANGE_END

} sai_hostif_user_defined_trap_attr_custom_t;

/**
 * @brief Custom Attribute Id for HostIF
 *
 * @flags free
 */
typedef enum _sai_hostif_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_HOSTIF_ATTR_CUSTOM_RANGE_START = SAI_HOSTIF_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of HostIF attributes
     */
    SAI_HOSTIF_ATTR_CUSTOM_RANGE_END

} sai_hostif_attr_custom_t;

/**
 * @brief Custom Attribute Id for HostIF Table Entry
 *
 * @flags free
 */
typedef enum _sai_hostif_table_entry_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_HOSTIF_TABLE_ENTRY_ATTR_CUSTOM_RANGE_START = SAI_HOSTIF_TABLE_ENTRY_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of HostIF Table Entry attributes
     */
    SAI_HOSTIF_TABLE_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_hostif_table_entry_attr_custom_t;

/**
 * @brief Custom Attribute Id for HostIF Packet
 *
 * @flags free
 */
typedef enum _sai_hostif_packet_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_HOSTIF_PACKET_ATTR_CUSTOM_RANGE_START = SAI_HOSTIF_PACKET_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of HostIF Packet attributes
     */
    SAI_HOSTIF_PACKET_ATTR_CUSTOM_RANGE_END

} sai_hostif_packet_attr_custom_t;

#endif /* __SAIHOSTIF_CUSTOM_H_ */