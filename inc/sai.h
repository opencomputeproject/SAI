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
*    sai.h
*     
* Abstract:
*
*   This module defines an entry point into Switch Abstraction Interfrace (SAI)
*
*/
#if !defined (__SAI_H_)
#define __SAI_H_

#include <saitypes.h>
#include <saistatus.h>
#include <saiswitch.h>
#include <saiport.h>
#include <saivlan.h>
#include <saifdb.h>
#include <sairouter.h>
#include <sairouterintf.h>
#include <saineighbor.h>
#include <sainexthop.h>
#include <sairoute.h>
#include <saiqos.h>
#include <saiacl.h>

/*
*
* Defined API sets have assigned ID's. If specific api method table changes
* in any way (method signature, number of methods), a new ID needs to be 
* created (e.g. VLAN2) and old API still may need to be supported for 
* compatibility with older adapter hosts.
*
*/
typedef enum _sai_api_t
{    
    SAI_API_UNSPECIFIED      =  0, 
    SAI_API_SWITCH           =  1,  /* sai_switch_api_t */
    SAI_API_PORT             =  2,  /* sai_port_api_t */
    SAI_API_FDB              =  3,  /* sai_fdb_api_t */
    SAI_API_VLAN             =  4,  /* sai_vlan_api_t */
    SAI_API_VR               =  5,  /* sai_vr_api_t */
    SAI_API_ROUTE            =  6,  /* sai_route_api_t */
    SAI_API_NEXT_HOP         =  7,  /* sai_next_hop_api_t */
    SAI_API_ROUTER_INTERFACE =  8,  /* sai_router_interface_api_t */
    SAI_API_NEIGHBOR         =  9,  /* sai_neighbor_api_t */
    SAI_API_QOS              = 10,  /* sai_qos_api_t */
    SAI_API_ACL              = 11,  /* sai_acl_api_t */
} sai_api_t;

/*
*   Method table that contains function pointers for services exposed by adapter
*   host for adapter.
*/

typedef struct _service_method_table_t
{
    /* Get variable value given its name */
    const char* (*profile_get_value)(_In_ sai_switch_profile_id_t profile_id,
                                     _In_ const char* variable);

    /* Enumerate all the K/V pairs in a profile. 
       Pointer to NULL passed as variable restarts enumeration.
       Function returns 0 if next value exists, -1 at the end of the list. */
    int (*profile_get_next_value)(_In_ sai_switch_profile_id_t profile_id,
                                  _Out_ const char** variable,
                                  _Out_ const char** value);

} service_method_table_t;



/*
* Routine Description:
*     Adapter module initialization call. This is NOT for SDK initialization.
*
* Arguments:
*     [in] flags - reserved for future use, must be zero
*     [in] services - methods table with services provided by adapter host
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t
sai_api_initialize(
    _In_ uint64_t flags,
    _In_ service_method_table_t* services
    );


/*
* Routine Description:
*     Retrieve a pointer to the C-style method table for desired SAI 
*     functionality as specified by the given sai_api_id.
*
* Arguments:
*     [in] sai_api_id - SAI api ID
*     [out] api_method_table - Caller allocated method table
*           The table must remain valid until the sai_api_shutdown() is called 
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t
sai_api_query(
    _In_ sai_api_t sai_api_id,
    _Out_ void** api_method_table
    );


/*
* Routine Description:
*   Uninitialization of the adapter module. SAI functionalities, retrieved via 
*   sai_api_query() cannot be used after this call.
*
* Arguments:
*   None
*
* Return Values:
*   SAI_STATUS_SUCCESS on success
*   Failure status code on error
*/
sai_status_t
sai_api_unitialize(
    void
    );

#endif  // __SAI_H_
