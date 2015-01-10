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
    SAI_NEXT_HOP_IP,

    /* 
    Tunneling to be added later
    */

} sai_next_hop_type_t;

/*
*  Attribute id for next hop
*/
typedef enum _sai_next_hop_attr_t
{
    /* READ-ONLY */

    /* READ-WRITE */

    /* Next hop entry type [sai_next_hop_type_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_NEXT_HOP_ATTR_TYPE,

    /* Next hop entry ipv4 address [sai_ip_address_t]
     * (MANDATORY_ON_CREATE when SAI_NEXT_HOP_ATTR_TYPE = SAI_NEXT_HOP_IP)
     * (CREATE_ONLY) */
    SAI_NEXT_HOP_ATTR_IP,

    /* Next hop entry router interface id [sai_router_interface_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,

    /* -- */

    /* Custom range base value */
    SAI_NEXT_HOP_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_next_hop_attr_t;

/*
* Routine Description:
*    Create next hop
*
* Arguments:
*    [out] next_hop_id - next hop id
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_next_hop_fn)(
    _Out_ sai_next_hop_id_t* next_hop_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/*
* Routine Description:
*    Remove next hop
*
* Arguments:
*    [in] next_hop_id - next hop id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_remove_next_hop_fn)(
    _In_ sai_next_hop_id_t next_hop_id
    );

/*
* Routine Description:
*    Set Next Hop attribute
*
* Arguments:
*    [in] sai_next_hop_id_t - next_hop_id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_next_hop_attribute_fn)(
    _In_ sai_next_hop_id_t next_hop_id,
    _In_ const sai_attribute_t *attr
    );


/*
* Routine Description:
*    Get Next Hop attribute
*
* Arguments:
*    [in] sai_next_hop_id_t - next_hop_id
*    [in] attr_count - number of attributes
*    [inout] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_next_hop_attribute_fn)(
    _In_ sai_next_hop_id_t next_hop_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/*
*  Next Hop methods table retrieved with sai_api_query()
*/
typedef struct _sai_next_hop_api_t
{
    sai_create_next_hop_fn        create_next_hop;
    sai_remove_next_hop_fn        remove_next_hop;
    sai_set_next_hop_attribute_fn set_next_hop_attribute;
    sai_get_next_hop_attribute_fn get_next_hop_attribute;

} sai_next_hop_api_t;

#endif // __SAINEXTHOP_H_
