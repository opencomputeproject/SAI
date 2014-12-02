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
*    sairouterintf.h
*
* Abstract:
*
*    This module defines SAI Router Interface
*
*/

#if !defined (__SAIROUTERINTF_H_)
#define __SAIROUTERINTF_H_

#include <saitypes.h>

#define ROUTER_INTERFACE_COUNTER_SET_DEFAULT    0

/*
*  Attribute data for SAI_ROUTER_INTERFACE_ATTR_TYPE
*/
typedef enum _sai_router_interface_type_t 
{
    /* Unknown Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_UNKNOWN = 0,

    /* Port Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_PORT,

    /* VLAN Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_VLAN

} sai_router_interface_type_t;


/*
*  Routing interface attribute IDs 
*/
typedef enum _sai_router_interface_attr_t
{
    /* READ-ONLY */

    /* Type [sai_router_interface_type_t] */
    SAI_ROUTER_INTERFACE_ATTR_TYPE,

    /* Assosiated Port [sai_port_id_t] 
    *   (when SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_PORT) 
    */
    SAI_ROUTER_INTERFACE_ATTR_PORT_ID,

    /* Assosiated Vlan [sai_vlan_id_t] 
    *   (when SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_VLAN) 
    */
    SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,

    /* READ-WRITE */

    /* Admin State [bool] */
    SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE,

    /* Admin State [bool] */
    SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE,

    /* MTU [int] */
    SAI_ROUTER_INTERFACE_ATTR_MTU,

    /* MAC Address [mac_t] (equal to the SAI_ROUTER_ATTR_MAC_ADDRESS by 
        default) */
    SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,

    /* -- */

    /* Custom range base value */
    SAI_ROUTER_INTERFACE_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_router_interface_attr_t;


/*
* Routine Description:
*    Set router interface attribute value
*
* Arguments:
*    [in] interface_id - router interface id
*    [in] attribute - router interface attribute
*    [in] value - router interface attribute value
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_router_interface_attribute_fn)(
    _In_ sai_router_interface_id_t router_interface_id,
    _In_ sai_router_interface_attr_t attribute, 
    _In_ uint64_t value
    );

/*
* Routine Description:
*    Get router interface attribute value
*
* Arguments:
*    [in] router_interface_id - router interface id
*    [in] attribute - router interface attribute
*    [out] value - router interface attribute value
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_get_router_interface_attribute_fn)(
    _In_ sai_router_interface_id_t router_interface_id,
    _In_ sai_router_interface_attr_t attribute, 
    _Out_ uint64_t* value
    );


/*
* Routine Description:
*    Create router interface. Interface is created in DOWN mode.
*    After all the required attributes are set and at least one ip address is 
*    added, it can be brought UP.
*
* Arguments:
*    [in,out] interface_id - router interface id
*    [in] vr_id - virtual router id
*    [in] interface_type - interface type (port, VLAN)
*    [in] attachment_id - Id of the corresponding port or VLAN 
*                         according to the interface type
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_create_router_interface_fn)(
    _Inout_ sai_router_interface_id_t* router_interface_id,
    _In_ sai_vr_id_t vr_id, 
    _In_ sai_router_interface_type_t router_interface_type,
    _In_ uint32_t attachment_id
    );


/*
* Routine Description:
*    Delete router interface
*
* Arguments:
*    [in] router_interface_id - router interface id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_delete_router_interface_fn)(
    _In_ sai_router_interface_id_t router_interface_id
    );

/*
* Routine Description:
*    Add IP address to router interface
*
* Arguments:
*    [in] router_interface_id - router interface id
*    [in] address - IP address (IPv4 or IPv6)
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_add_router_interface_address_fn)(
    _In_ sai_router_interface_id_t router_interface_id,
    _In_ sockaddr_inet* address  
    );

/*
* Routine Description:
*    Remove IP address from router interface
*
* Arguments:
*    [in] router_interface_id - router interface id
*    [in] address - IP address (IPv4 or IPv6)
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_delete_router_interface_address_fn)(
    _In_ sai_router_interface_id_t router_interface_id,
    _In_ sockaddr_inet* address 
    );

/*
* Routine Description:
*   Enable/disable statistics counters for router interface.
*
* Arguments:
*    [in] router_interface_id - router interface id
*    [in] counter_set_id - specifies the counter set
*    [in] enable - TRUE to enable, FALSE to disable
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/ 
typedef sai_status_t (*sai_ctl_router_interface_stats_fn)(
    _In_ sai_router_interface_id_t router_interface_id,
    _In_ uint32_t counter_set_id,
    _In_ bool enable
    );

/*
* Routine Description:
*   Get router interface statistics counters.
*
* Arguments:
*    [in] router_interface_id - router interface id
*    [in] counter_set_id - specifies the counter set
*    [in] number_of_counters - number of counters in the array
*    [out] counters - array of resulting counter values.
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/ 
typedef sai_status_t (*sai_get_router_interface_stats_fn)(
    _In_ sai_router_interface_id_t router_interface_id,
    _In_ uint32_t counter_set_id,
    _In_ uint32_t number_of_counters,
    _Out_ uint64_t* counters
    );

/*
*  Routing interface methods table retrieved with sai_api_query()
*/
typedef struct _sai_router_interface_api_t
{
    sai_create_router_interface_fn          create_router_interface;
    sai_delete_router_interface_fn          delete_router_interface;
    sai_set_router_interface_attribute_fn   set_attribute;
    sai_get_router_interface_attribute_fn   get_attribute;
    sai_add_router_interface_address_fn     add_address;
    sai_delete_router_interface_address_fn  delete_address;
    sai_ctl_router_interface_stats_fn       ctl_stats;
    sai_get_router_interface_stats_fn       get_stats;

} sai_router_interface_api_t;

#endif // __SAIROUTERINTF_H_

