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
*    sairoute.h
*
* Abstract:
*
*    This module defines SAI Route Entry API
*
*/

#if !defined (__SAIROUTE_H_)
#define __SAIROUTE_H_

#include <saitypes.h>
/*
*  Attribute data for route packet action
*/
typedef enum _sai_route_packet_action_t
{
    /* Route Packet */
    SAI_ROUTE_PACKET_ACTION_ROUTE,

    /* Trap Packet to CPU */
    SAI_ROUTE_PACKET_ACTION_TRAP,

    /* Log (Trap + Forward) Packet */
    SAI_ROUTE_PACKET_ACTION_LOG,

    /* Drop Packet */
    SAI_ROUTE_PACKET_ACTION_DROP

} sai_route_packet_action_t;


/*
*  Attribute Id in sai_set_route_attribute() and 
*  sai_get_route_attribute() calls
*/
typedef enum _sai_route_attr_t
{
    /* READ-WRITE */

    /* Admin state [bool] */
    SAI_ROUTE_ATTR_ADMIN_STATE,

    /* Packet action [sai_route_packet_action_t] */
    SAI_ROUTE_ATTR_PACKET_ACTION,

    /* Packet priority for trap/log actions [uint8] */
    SAI_ROUTE_ATTR_TRAP_PRIORITY,

    /* -- */

    /* Custom range base value */
    SAI_ROUTE_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_route_attr_t;


/*
*  Unicast route entry 
*/
typedef struct _sai_unicast_route_entry_t
{
    sai_vr_id_t vr_id;
    ip_address_prefix destination;
    sai_next_hop_group_id_t next_hop_group_id;

} sai_unicast_route_entry_t;

 
/*
* Routine Description:
*    Set route attribute value
*
* Arguments:
*    [in] unicast_route_entry - route entry
*    [in] attribute - route attribute
*    [in] value - route attribute value
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_route_attribute_fn)(
    _In_ sai_unicast_route_entry_t* unicast_route_entry,
    _In_ sai_route_attr_t attribute, 
    _In_ uint64_t value
    );

/*
* Routine Description:
*    Get route attribute value
*
* Arguments:
*    [in] unicast_route_entry - route entry
*    [in] attribute - route attribute
*    [out] value - route attribute value
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_route_attribute_fn)(
    _In_ sai_unicast_route_entry_t* unicast_route_entry,
    _In_ sai_route_attr_t attribute, 
    _Out_ uint64_t* value
    );

/*
* Routine Description:
*    Create Route
*
* Arguments:
*    [in] unicast_route_entry - route entry
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_route_fn)(
    _In_ sai_unicast_route_entry_t* unicast_route_entry
    );

/*
* Routine Description:
*    Delete Route
*
* Arguments:
*    [in] unicast_route_entry - route entry
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_delete_route_fn)(
    _In_ sai_unicast_route_entry_t* unicast_route_entry
    );

/*
*  Router entry methods table retrieved with sai_api_query()
*/
typedef struct _sai_route_api_t
{
    sai_create_route_fn         create_route;
    sai_delete_route_fn         delete_route;
    sai_set_route_attribute_fn  set_attribute;
    sai_get_route_attribute_fn  get_attribute;

} sai_route_api_t;

#endif // __SAIROUTE_H_
