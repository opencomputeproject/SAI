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
*  neighbor entry 
*/
typedef struct _sai_neighbor_entry_t
{
    sai_vr_id_t vr_id;
    sai_router_interface_id_t router_interface_id;
    sockaddr_inet ip_address;
    sai_mac_t mac_address;

} sai_neighbor_entry_t;


/*
* Routine Description:
*    Create neighbor entry 
*
* Arguments:
*    [in] neighbor_entry - neighbor entry 
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_neighbor_entry_fn)(
    _In_ sai_neighbor_entry_t* neighbor_entry
    );

/*
* Routine Description:
*    Delete neighbor entry 
*
* Arguments:
*    [in] neighbor_entry - neighbor entry 
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_delete_neighbor_entry_fn)(
    _In_ sai_neighbor_entry_t* neighbor_entry
    );


/*
* Routine Description:
*    Delete all neighbor entries
*
* Arguments:
*    None
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_delete_all_neighbor_entries_fn)(void);


/*
* Routine Description:
*    Check if neighbor entry was used
*
* Arguments:
*    [in] neighbor_entry - neighbor entry 
*    [in] reset_used_flag - reset the used flag after reading 
*    [out] is_used - true if neighbor entry was used 
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_query_neighbor_entry_fn)(
    _In_ sai_neighbor_entry_t* neighbor_entry,
    _In_ bool reset_used_flag,
    _Out_ bool* is_used
    );

/*
*  neighbor table methods, retrieved via sai_api_query()
*/
typedef struct _sai_neighbor_api_t
{
    sai_create_neighbor_entry_fn        create_neighbor_entry;
    sai_delete_neighbor_entry_fn        delete_neighbor_entry;
    sai_delete_all_neighbor_entries_fn  delete_all_neighbor_entries;
    sai_query_neighbor_entry_fn         query_neighbor_entry;

} sai_neighbor_api_t;

#endif // __SAINEIGHBOR_H_

