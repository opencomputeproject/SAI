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
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc
 *
 * @file    sairouter.h
 *
 * @brief   This module defines SAI Virtual Router interface
 *
 * @par Abstract
 *
 *    This module defines SAI Virtual Router (VR) API.
 *    Virtual Router should allow VRFs at a minimum, VRRP functionality is
 *    considered.
 */

#if !defined (__SAIROUTER_H_)
#define __SAIROUTER_H_

#include <saitypes.h>

/**
 * @defgroup SAIROUTER SAI - Router specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute Id in sai_set_virtual_router_attribute() and
 * sai_get_virtual_router_attribute() calls
 */
typedef enum _sai_virtual_router_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_VIRTUAL_ROUTER_ATTR_START,

    /* READ-WRITE */

    /**
     * @brief Admin V4 state
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE = SAI_VIRTUAL_ROUTER_ATTR_START,

    /**
     * @brief Admin V6 state
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE,

    /**
     * @brief MAC Address
     *
     * Equal to the #SAI_SWITCH_ATTR_SRC_MAC_ADDRESS by default
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default attrvalue SAI_SWITCH_ATTR_SRC_MAC_ADDRESS
     */
    SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,

    /**
     * @brief Action for Packets with TTL 0 or 1
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_TRAP
     */
    SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_PACKET_ACTION,

    /**
     * @brief Action for Packets with IP options
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_TRAP
     */
    SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS_PACKET_ACTION,

    /**
     * @brief Action for Unknown L3 multicast Packets
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_DROP
     */
    SAI_VIRTUAL_ROUTER_ATTR_UNKNOWN_L3_MULTICAST_PACKET_ACTION,

    /**
     * @brief End of attributes
     */
    SAI_VIRTUAL_ROUTER_ATTR_END,

    /** Custom range base value */
    SAI_VIRTUAL_ROUTER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_VIRTUAL_ROUTER_ATTR_CUSTOM_RANGE_END

} sai_virtual_router_attr_t;

/**
 * @brief Create virtual router
 *
 * @param[out] vr_id Virtual router id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success
 *         #SAI_STATUS_ADDR_NOT_FOUND if neither #SAI_SWITCH_ATTR_SRC_MAC_ADDRESS nor
 *         #SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS is set.
 */
typedef sai_status_t (*sai_create_virtual_router_fn)(
        _Out_ sai_object_id_t *vr_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove virtual router
 *
 * @param[in] vr_id Virtual router id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_virtual_router_fn)(
        _In_ sai_object_id_t vr_id);

/**
 * @brief Set virtual router attribute Value
 *
 * @param[in] vr_id Virtual router id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_virtual_router_attribute_fn)(
        _In_ sai_object_id_t vr_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get virtual router attribute Value
 *
 * @param[in] vr_id Virtual router id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_virtual_router_attribute_fn)(
        _In_ sai_object_id_t vr_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Virtual router methods table retrieved with sai_api_query()
 */
typedef struct _sai_virtual_router_api_t
{
    sai_create_virtual_router_fn        create_virtual_router;
    sai_remove_virtual_router_fn        remove_virtual_router;
    sai_set_virtual_router_attribute_fn set_virtual_router_attribute;
    sai_get_virtual_router_attribute_fn get_virtual_router_attribute;

} sai_virtual_router_api_t;

/**
 * @}
 */
#endif /** __SAIROUTER_H_ */
