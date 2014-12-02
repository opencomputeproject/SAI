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
*    sainexthop.h
*
* Abstract:
*
*    This module defines SAI Next Hop API
*
*/

#if !defined (__SAINEXTHOP_H_)
#define __SAINEXTHOP_H_

#include <saitypes.h>


/*
*  Next hop type
*/
typedef enum _sai_next_hop_type_t
{
    SAI_NEXT_HOP_UNSPECIFIED,
    SAI_NEXT_HOP_IP

    /* 
    Tunneling to be added later
    */

} sai_next_hop_type_t;


/*
*  Next hop group type
*/
typedef enum _sai_next_hop_group_type_t
{
    SAI_NEXT_HOP_GROUP_UNSPECIFIED,

    /* Next hop group contains single next hop */
    SAI_NEXT_HOP_GROUP_SIMPLE,

    /* Next hop group is ECMP */
    SAI_NEXT_HOP_GROUP_ECMP,

    /* Next hop group is WCMP */
    SAI_NEXT_HOP_GROUP_WCMP

} sai_next_hop_group_type_t;

/*
*  Next Hop
*/
typedef struct _sai_next_hop_t
{
    sai_next_hop_type_t next_hop_type;

    union _data
    {
        /*
        *  IP address next nop
        */
        struct _ip
        {
            sockaddr_inet address;
            sai_router_interface_id_t interface_id;
        } ip;

    } data;

    uint32_t weight;

} sai_next_hop_t;


/*
* Routine Description:
*    Create next hop group 
*
* Arguments:
*    [in,out] next_hop_group_id - next hop group id
*    [in] next_hop_group_type - next hop group type
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_next_hop_group_fn)(
    _Inout_ sai_next_hop_group_id_t* next_hop_group_id,
    _In_ sai_next_hop_group_type_t next_hop_group_type
    );


/*
* Routine Description:
*    Delete next hop group 
*
* Arguments:
*    [in] next_hop_group_id - next hop group id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_delete_next_hop_group_fn)(
    _In_ sai_next_hop_group_id_t next_hop_group_id
    );

/*
* Routine Description:
*    Set Next Hop Group attribute
*
* Arguments:
     [in] sai_next_hop_group_id_t - next_hop_group_id
*    [in] attribute - FDB attribute
*    [in] value - FDB attribute value
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_next_hop_group_attribute_fn)(
    _In_ sai_next_hop_group_id_t next_hop_group_id,
    _In_ sai_fdb_attr_t attribute, 
    _In_ uint64_t value
    );


/*
* Routine Description:
*    Get Next Hop Group attribute
*
* Arguments:
     [in] sai_next_hop_group_id_t - next_hop_group_id
*    [in] attribute - FDB attribute
*    [out] value - FDB attribute value
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_next_hop_group_attribute_fn)(
    _In_ sai_next_hop_group_id_t next_hop_group_id,
    _In_ sai_fdb_attr_t attribute, 
    _Out_ uint64_t* value
    );

/*
* Routine Description:
*    Add next hop to a group 
*
* Arguments:
*    [in] next_hop_group_id - next hop group id
*    [in] next_hop - next hop to add
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_add_next_hop_to_group_fn)(
    _In_ sai_next_hop_group_id_t next_hop_group_id,
    _In_ sai_next_hop_t* next_hop
    );


/*
* Routine Description:
*    Remove next hop from a group 
*
* Arguments:
*    [in] next_hop_group_id - next hop group id
*    [in] next_hop - next hop to remove
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_remove_next_hop_from_group_fn)(
    _In_ sai_next_hop_group_id_t next_hop_group_id,
    _In_ sai_next_hop_t* next_hop
    );


/*
*  Next Hop methods table retrieved with sai_api_query()
*/
typedef struct _sai_next_hop_api_t
{
    sai_create_next_hop_group_fn        create_next_hop_group;
    sai_delete_next_hop_group_fn        delete_next_hop_group;
    sai_add_next_hop_to_group_fn        add_next_hop_to_group;
    sai_remove_next_hop_from_group_fn   remove_next_hop_from_group;

} sai_next_hop_api_t;

#endif // __SAINEXTHOP_H_

