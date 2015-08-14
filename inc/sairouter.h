/*
* Copyright (c) 2014 Microsoft Open Technologies, Inc. 
*   
*    Licensed under the Apache License, Version 2.0 (the "License"); you may 
*    not use this file except in compliance with the License. You may obtain 
*    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
*    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR 
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
* Module Name:
*
*    sairouter.h
*
* Abstract:
*
*    This module defines SAI Virtual Router (VR) API.
*    Virtual Router should allow VRFs at a minimum, VRRP functionality is 
*    considered.
*
*/

#if !defined (__SAIROUTER_H_)
#define __SAIROUTER_H_

#include <saitypes.h>

/** \defgroup SAIROUTER SAI - Router specific API definitions.
 *
 *  \{
 */
 
/**
 *  Attribute Id in sai_set_virtual_router_attribute() and 
 *  sai_get_virtual_router_attribute() calls
 */
typedef enum _sai_virtual_router_attr_t
{
    /** READ-WRITE */

    /** Admin V4 state [bool] (default to TRUE) */
    SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE,

    /** Admin V6 state [bool] (default to TRUE) */
    SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE,

    /** MAC Address [sai_mac_t]  
      (equal to the SAI_SWITCH_ATTR_SRC_MAC_ADDRESS by default) */
    SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,

    /** Action for Packets with TTL 0 or 1 [sai_packet_action_t]
      (default to SAI_PACKET_ACTION_TRAP) */
    SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION,

    /** Action for Packets with IP options [sai_packet_action_t] 
     * (default to SAI_PACKET_ACTION_TRAP) */
    SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS,

    /* -- */

    /** Custom range base value */
    SAI_VIRTUAL_ROUTER_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_virtual_router_attr_t;

/**
 * Routine Description:
 *    @brief Create virtual router
 *    
 * Arguments:
 *    @param[out] vr_id - virtual router id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 * 
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            SAI_STATUS_ADDR_NOT_FOUND if neither SAI_SWITCH_ATTR_SRC_MAC_ADDRESS nor 
 *            SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS is set.
 */
typedef sai_status_t (*sai_create_virtual_router_fn)(
    _Out_ sai_object_id_t *vr_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Remove virtual router
 *
 * Arguments:
 *    @param[in] vr_id - virtual router id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_remove_virtual_router_fn)(
    _In_ sai_object_id_t vr_id
    );

/**
 * Routine Description:
 *    @brief Set virtual router attribute Value
 *
 * Arguments:
 *    @param[in] vr_id - virtual router id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_virtual_router_attribute_fn)(
    _In_ sai_object_id_t vr_id, 
    _In_ const sai_attribute_t *attr
    );

/**
 * Routine Description:
 *    @brief Get virtual router attribute Value
 *
 * Arguments:
 *    @param[in] vr_id - virtual router id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_virtual_router_attribute_fn)(
    _In_ sai_object_id_t vr_id, 
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 *  @brief Virtual router methods table retrieved with sai_api_query()
 */
typedef struct _sai_virtual_router_api_t
{
    sai_create_virtual_router_fn        create_virtual_router;
    sai_remove_virtual_router_fn        remove_virtual_router;
    sai_set_virtual_router_attribute_fn set_virtual_router_attribute;
    sai_get_virtual_router_attribute_fn get_virtual_router_attribute;

} sai_virtual_router_api_t;

/**
 * \}
 */
#endif // __SAIROUTER_H_
