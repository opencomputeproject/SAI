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
*    saihostintf.h
*
* Abstract:
*
*    This module defines SAI Host Interface which is responsbile for
*    creating/deleting linux netdev corresponding to the host interface type.
*    All the management operations of the netdevs such as changing IP address
*    are outside of SAI. 
*
*/

#if !defined (__SAIHOSTINTF_H_)
#define __SAIHOSTINTF_H_

#include <saitypes.h>

#define HOST_INTERFACE_NAME_SIZE    16

/*
*  Host interface attribute IDs 
*/
typedef enum _sai_host_interface_attr_t
{
    /* READ-ONLY */

    /* READ-WRITE */

    /* Assosiated port or router interface [sai_object_id_t] 
     * (MACDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_HOST_INTERFACE_ATTR_PORT_RIF_ID,

    /* Name [char[HOST_INTERFACE_NAME_SIZE]] (MANDATORY_ON_CREATE) 
     * The maximum number of charactars for the name is HOST_INTERFACE_NAME_SIZE - 1 since
     * it needs the terminating null byte ('\0') at the end.  */
    SAI_HOST_INTERFACE_ATTR_NAME,

    /* Custom range base value */
    SAI_HOST_INTERFACE_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_host_interface_attr_t;

/*
* Routine Description:
*    Create host interface. 
*
* Arguments:
*    [out] hif_id - host interface id
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_create_host_interface_fn)(
    _Out_ sai_object_id_t* hif_id,
    _In_ uint32_t attr_count,
    _In_ sai_attribute_t *attr_list
    );

/*
* Routine Description:
*    Remove host interface
*
* Arguments:
*    [in] hif_id - host interface id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_remove_host_interface_fn)(
    _In_ sai_object_id_t hif_id
    );

/*
* Routine Description:
*    Set host interface attribute
*
* Arguments:
*    [in] hif_id - host interface id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_host_interface_attribute_fn)(
    _In_ sai_object_id_t hif_id,
    _In_ const sai_attribute_t *attr
    );


/*
* Routine Description:
*    Get host interface attribute
*
* Arguments:
*    [in] hif_id - host interface id
*    [in] attr_count - number of attributes
*    [inout] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_host_interface_attribute_fn)(
    _In_ sai_object_id_t hif_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/*
*  Host interface methods table retrieved with sai_api_query()
*/
typedef struct _sai_host_interface_api_t
{
    sai_create_host_interface_fn          create_host_interface;
    sai_remove_host_interface_fn          remove_host_interface;
    sai_set_host_interface_attribute_fn   set_host_interface_attribute;
    sai_get_host_interface_attribute_fn   get_host_interface_attribute;

} sai_host_interface_api_t;

#endif // __SAIHOSTINTF_H_
