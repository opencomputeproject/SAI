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
 * @file    saiportextensions.h
 *
 * @brief   This module defines port extensions of the Switch Abstraction Interface (SAI)
 */

#ifndef __SAIPORTEXTENSIONS_H_
#define __SAIPORTEXTENSIONS_H_

#include <saiport.h>
#include <saitypes.h>

/**
 * @brief SAI port attribute extensions.
 *
 * @flags free
 */
typedef enum _sai_port_attr_extensions_t
{
    SAI_PORT_ATTR_EXTENSIONS_RANGE_START = 0x20000000,

    /* Add new experimental port attributes above this line */

    SAI_PORT_ATTR_EXTENSIONS_RANGE_END

} sai_port_attr_extensions_t;

/**
 * @brief SAI port stat extensions.
 *
 * @flags free
 */
typedef enum _sai_port_stat_extensions_t
{
    SAI_PORT_STAT_EXTENSIONS_RANGE_START = 0x20000000,

    /** DASH port LB_FAST_PATH_ICMP_IN_BYTES stat count */
    SAI_PORT_STAT_LB_FAST_PATH_ICMP_IN_BYTES = SAI_PORT_STAT_EXTENSIONS_RANGE_START,

    /** DASH port LB_FAST_PATH_ICMP_IN_PACKETS stat count */
    SAI_PORT_STAT_LB_FAST_PATH_ICMP_IN_PACKETS,

    /** DASH port LB_FAST_PATH_ENI_MISS_BYTES stat count */
    SAI_PORT_STAT_LB_FAST_PATH_ENI_MISS_BYTES,

    /** DASH port LB_FAST_PATH_ENI_MISS_PACKETS stat count */
    SAI_PORT_STAT_LB_FAST_PATH_ENI_MISS_PACKETS,

    /* Add new experimental port stats above this line */

    SAI_PORT_STAT_EXTENSIONS_RANGE_END

} sai_port_stat_extensions_t;

#endif /* __SAIPORTEXTENSIONS_H_ */
