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
*    saineighbor.h
*
* Abstract:
*
*    This module defines SAI neighbor table API
*    The table contains both IPv4 and IPv6 neighbors 
*
*/

#if !defined (__SAINEIGHBOR_H_)
#define __SAINEIGHBOR_H_

#include <saitypes.h>

/*
*  Attribute Id for sai neighbor object
*/
typedef enum _sai_neighbor_attr_t
{
    /* READ-WRITE */

    /* Destination mac address for the neighbor [sai_mac_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS, 

    /* L3 forwarding action for this neighbor [sai_packet_action_t]
    *    (default to SAI_PACKET_ACTION_FORWARD) */
    SAI_NEIGHBOR_ATTR_PACKET_ACTION,

    /* Custom range base value */
    SAI_NEIGHBOR_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_neighbor_attr_t;

/*
*  neighbor entry 
*/
typedef struct _sai_neighbor_entry_t
{
    sai_object_id_t rif_id; 
    sai_ip_address_t ip_address;

} sai_neighbor_entry_t;


/*
* Routine Description:
*    Create neighbor entry 
*
* Arguments:
*    [in] neighbor_entry - neighbor entry 
*    [in] attr_count - number of attributes
*    [in] attrs - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*
* Note: IP address expected in Network Byte Order.
*/
typedef sai_status_t (*sai_create_neighbor_entry_fn)(
    _In_ const sai_neighbor_entry_t* neighbor_entry,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/*
* Routine Description:
*    Remove neighbor entry 
*
* Arguments:
*    [in] neighbor_entry - neighbor entry 
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*
* Note: IP address expected in Network Byte Order.
*/
typedef sai_status_t (*sai_remove_neighbor_entry_fn)(
    _In_ const sai_neighbor_entry_t* neighbor_entry
    );

/*
* Routine Description:
*    Set neighbor attribute value
*
* Arguments:
*    [in] neighbor_entry - neighbor entry
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_neighbor_attribute_fn)(
    _In_ const sai_neighbor_entry_t* neighbor_entry,
    _In_ const sai_attribute_t *attr
    );

/*
* Routine Description:
*    Get neighbor attribute value
*
* Arguments:
*    [in] neighbor_entry - neighbor entry
*    [in] attr_count - number of attributes
*    [inout] attrs - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_neighbor_attribute_fn)(
    _In_ const sai_neighbor_entry_t* neighbor_entry,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/*
* Routine Description:
*    Remove all neighbor entries
*
* Arguments:
*    None
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_remove_all_neighbor_entries_fn)(void);

/*
*  neighbor table methods, retrieved via sai_api_query()
*/
typedef struct _sai_neighbor_api_t
{
    sai_create_neighbor_entry_fn        create_neighbor_entry;
    sai_remove_neighbor_entry_fn        remove_neighbor_entry;
    sai_set_neighbor_attribute_fn       set_neighbor_attribute;
    sai_get_neighbor_attribute_fn       get_neighbor_attribute;
    sai_remove_all_neighbor_entries_fn  remove_all_neighbor_entries;

} sai_neighbor_api_t;

#endif // __SAINEIGHBOR_H_

