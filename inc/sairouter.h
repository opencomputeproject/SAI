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

/*
*  Attribute data for route action attributes
*/
typedef enum _sai_vr_action_t
{
    /* Drop Packet */
    SAI_VR_TTL_ACTION_DROP,

    /* Forward Packet */
    SAI_VR_TTL_ACTION_FORWARD,

    /* Trap Packet to CPU */
    SAI_VR_TTL_ACTION_TRAP,

    /* Log (Trap + Forward) Packet */
    SAI_VR_TTL_ACTION_LOG

} sai_vr_action_t;


/*
*  Attribute Id in sai_set_vr_attribute() and 
*  sai_get_vr_attribute() calls
*/
typedef enum _sai_vr_attr_t
{
    /* READ-WRITE */

    /* Admin V4 state [bool] */
    SAI_VR_ATTR_ADMIN_V4_STATE,

    /* Admin V6 state [bool] */
    SAI_VR_ATTR_ADMIN_V6_STATE,

    /* MAC Address [sai_mac_t] */
    SAI_VR_ATTR_MAC_ADDRESS,

    /* Action for Packets with TTL 0 or 1 [sai_vr_action_t] */
    SAI_VR_ATTR_VIOLATION_TTL1_ACTION,

    /* Action for Packets with TTL 2 (drop early) [sai_vr_action_t] */
    SAI_VR_ATTR_VIOLATION_TTL2_ACTION,

    /* Action for Packets with IP options [route_packet_action_e] */
    SAI_VR_ATTR_VIOLATION_IP_OPTIONS,

    /* Minumum MTU of all router interfaces */
    SAI_VR_ATTR_MIN_MTU,

    /* -- */

    /* Custom range base value */
    SAI_VR_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_vr_attr_t;


/*
* Routine Description:
*    Set virtual router attribute Value
*
* Arguments:
*    [in] vr_id - virtual router id
*    [in] attribute - virtual router attribute
*    [in] value - virtual router attribute value
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_vr_attribute_fn)(
    _In_ sai_vr_id_t vr_id, 
    _In_ sai_vr_attr_t attribute, 
    _In_ uint64_t value
    );

/*
* Routine Description:
*    Get virtual router attribute Value
*
* Arguments:
*    [in] vr_id - virtual router id
*    [in] attribute - virtual router attribute
*    [out] value - virtual router attribute value
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_vr_attribute_fn)(
    _In_ sai_vr_id_t vr_id, 
    _In_ sai_vr_attr_t attribute, 
    _In_ uint64_t* value
    );

/*
* Routine Description:
*    Create virtual router
*
* Arguments:
*    [in] vr_id - virtual router id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_vr_fn)(
    _In_ sai_vr_id_t vr_id
    );


/*
* Routine Description:
*    Delete virtual router
*
* Arguments:
*    [in] vr_id - virtual router id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_delete_vr_fn)(
    _In_ sai_vr_id_t vr_id
    );

/*
*  Virtual router methods table retrieved with sai_api_query()
*/
typedef struct _sai_vr_api_t
{
    sai_create_vr_fn           create_router;
    sai_delete_vr_fn           delete_router;
    sai_set_vr_attribute_fn    set_attribute;
    sai_get_vr_attribute_fn    get_attribute;

} sai_vr_api_t;

#endif // __SAIROUTER_H_
