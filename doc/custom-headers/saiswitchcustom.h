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
 * @file    saiswitchcustom.h
 *
 * @brief   This module defines switch custom of the Switch Abstraction Interface (SAI)
 */

#ifndef __SAISWITCHCUSTOM_H_
#define __SAISWITCHCUSTOM_H_

#include <saiswitch.h>
#include <saitypescustom.h>

/**
 * @brief SAI switch attribute custom,
 *
 * @flags free
 */
typedef enum _sai_switch_attr_custom_t
{
    /**
     * @brief Custom 1
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_CUSTOM1 = SAI_SWITCH_ATTR_CUSTOM_RANGE_START,

    /**
     * @brief Custom 2
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_CUSTOM2,

} sai_switch_attr_custom_t;

#endif /* __SAISWITCHCUSTOM_H_ */
