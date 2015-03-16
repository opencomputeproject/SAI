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
*    saivlan.h
*
* Abstract:
*
*    This module defines SAI VLAN API
*
*/

#if !defined (__SAIVLAN_H_)
#define __SAIVLAN_H_

#include <saitypes.h>

#define VLAN_COUNTER_SET_DEFAULT    0

/*
*  Attribute data for tagging_mode parameter
*/
typedef enum _sai_vlan_tagging_mode_t
{
    SAI_VLAN_PORT_UNTAGGED,

    SAI_VLAN_PORT_TAGGED,

    SAI_VLAN_PORT_PRIORITY_TAGGED

} sai_vlan_tagging_mode_t;

/*
*   Port/vlan membership structure
*/
typedef struct _sai_vlan_port_t
{
    sai_object_id_t port_id;

    sai_vlan_tagging_mode_t tagging_mode;

} sai_vlan_port_t;

/*
*  Attribute Id in sai_set_vlan_attribute() and 
*  sai_get_vlan_attribute() calls
*/
typedef enum _sai_vlan_attr_t 
{
    /* READ-WRITE */
    
    /* Maximum number of learned MAC addresses [uint32_t]
     * zero means learning limit disable. (default to zero).
     */
    SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES,

    /* Custom range base value */
    SAI_VLAN_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_vlan_attr_t;


/*
*  VLAN counter IDs in sai_get_vlan_stat_counters() call
*/
typedef enum _sai_vlan_stat_counter_t
{
    SAI_VLAN_STAT_IN_OCTETS,
    SAI_VLAN_STAT_IN_UCAST_PKTS,
    SAI_VLAN_STAT_IN_NON_UCAST_PKTS,
    SAI_VLAN_STAT_IN_DISCARDS,
    SAI_VLAN_STAT_IN_ERRORS,
    SAI_VLAN_STAT_IN_UNKNOWN_PROTOS,
    SAI_VLAN_STAT_OUT_OCTETS,
    SAI_VLAN_STAT_OUT_UCAST_PKTS,
    SAI_VLAN_STAT_OUT_NON_UCAST_PKTS,
    SAI_VLAN_STAT_OUT_DISCARDS,
    SAI_VLAN_STAT_OUT_ERRORS,
    SAI_VLAN_STAT_OUT_QLEN

} sai_vlan_stat_counter_t;

/*
* Routine Description:
*    Create a VLAN
*
* Arguments:
*    [in] vlan_id - VLAN id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_vlan_fn)(
    _In_ sai_vlan_id_t vlan_id
    );

/*
* Routine Description:
*    Remove a VLAN
*
* Arguments:
*    [in] vlan_id - VLAN id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_remove_vlan_fn)(
    _In_ sai_vlan_id_t vlan_id
    );

/*
* Routine Description:
*    Set VLAN attribute Value
*
* Arguments:
*    [in] vlan_id - VLAN id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_vlan_attribute_fn)(
    _In_ sai_vlan_id_t vlan_id, 
    _In_ const sai_attribute_t *attr
    );

/*
* Routine Description:
*    Get VLAN attribute Value
*
* Arguments:
*    [in] vlan_id - VLAN id
*    [in] attr_count - number of attributes
*    [inout] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_vlan_attribute_fn)(
    _In_ sai_vlan_id_t vlan_id, 
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/*
* Routine Description:
*    Remove VLAN configuration (remove all VLANs).
*
* Arguments:
*    None
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_remove_all_vlans_fn)(
    void
    );

/*
* Routine Description:
*    Add Port to VLAN
*
* Arguments:
*    [in] vlan_id - VLAN id
*    [in] port_count - number of ports
*    [in] port_list - pointer to membership structures
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_add_ports_to_vlan_fn)(
    _In_ sai_vlan_id_t vlan_id, 
    _In_ uint32_t port_count,
    _In_ const sai_vlan_port_t* port_list
    );

/*
* Routine Description:
*    Remove Port from VLAN
*
* Arguments:
*    [in] vlan_id - VLAN id
*    [in] port_count - number of ports
*    [in] port_list - pointer to membership structures
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_remove_ports_from_vlan_fn)(
    _In_ sai_vlan_id_t vlan_id, 
    _In_ uint32_t port_count,
    _In_ const sai_vlan_port_t* port_list
    );

/*
* Routine Description:
*   Get vlan statistics counters.
*
* Arguments:
*    [in] vlan_id - VLAN id
*    [in] counter_ids - specifies the array of counter ids
*    [in] number_of_counters - number of counters in the array
*    [out] counters - array of resulting counter values.
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/ 
typedef sai_status_t (*sai_get_vlan_stats_fn)(
    _In_ sai_vlan_id_t vlan_id, 
    _In_ const sai_vlan_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters,
    _Out_ uint64_t* counters
    );

/*
* VLAN methods table retrieved with sai_api_query()
*/
typedef struct _sai_vlan_api_t
{
    sai_create_vlan_fn              create_vlan;
    sai_remove_vlan_fn              remove_vlan;
    sai_set_vlan_attribute_fn       set_vlan_attribute;
    sai_get_vlan_attribute_fn       get_vlan_attribute;
    sai_add_ports_to_vlan_fn        add_ports_to_vlan;
    sai_remove_ports_from_vlan_fn   remove_ports_from_vlan;
    sai_remove_all_vlans_fn         remove_all_vlans;
    sai_get_vlan_stats_fn           get_vlan_stats;
    
} sai_vlan_api_t;

#endif // __SAIVLAN_H_
