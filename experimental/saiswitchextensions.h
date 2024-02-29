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
 * @file    saiswitchextensions.h
 *
 * @brief   This module defines switch extensions of the Switch Abstraction Interface (SAI)
 */

#ifndef __SAISWITCHEXTENSIONS_H_
#define __SAISWITCHEXTENSIONS_H_

#include <saiswitch.h>
#include <saitypes.h>

/**
 * @brief SAI switch attribute extensions.
 *
 * @flags free
 */
typedef enum _sai_switch_attr_extensions_t
{
    SAI_SWITCH_ATTR_EXTENSIONS_RANGE_START = SAI_SWITCH_ATTR_END,

    /**
     * @brief Set Switch ICMP_ECHO session state change event notification callback function passed to the adapter.
     *
     * Use sai_icmp_echo_session_state_change_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_icmp_echo_session_state_change_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_ICMP_ECHO_SESSION_STATE_CHANGE_NOTIFY = SAI_SWITCH_ATTR_EXTENSIONS_RANGE_START,

    SAI_SWITCH_ATTR_EXTENSIONS_RANGE_END

} sai_switch_attr_extensions_t;

#endif /* __SAISWITCHEXTENSIONS_H_ */
