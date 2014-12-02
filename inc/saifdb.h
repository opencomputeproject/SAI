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
*    saifdb.h
*
* Abstract:
*
*    This module defines SAI FDB API
*    FDB: Forwarding Database a.k.a. MAC Address Table 
*
*/

#if !defined (__SAIFDB_H_)
#define __SAIFDB_H_

#include <saitypes.h>


/*
*  FDB entry type.
*/
typedef enum _sai_fdb_entry_type_t
{
    SAI_FDB_ENTRY_UNSPECIFIED,

    /* Dynamic FDB Entry */
    SAI_FDB_ENTRY_DYNAMIC,

    /* Static FDB Entry */
    SAI_FDB_ENTRY_STATIC,

} sai_fdb_entry_type_t;


/*
*  Attribute data for fdb entry packet action
*/
typedef enum _sai_fdb_entry_packet_action_t
{
    /* Forward Packet */
    SAI_FDB_ENTRY_PACKET_ACTION_FORWARD,

    /* Trap Packet to CPU */
    SAI_FDB_ENTRY_PACKET_ACTION_TRAP,

    /* Log (Trap + Forward) Packet */
    SAI_FDB_ENTRY_PACKET_ACTION_LOG,

    /* Drop Packet */
    SAI_FDB_ENTRY_PACKET_ACTION_DROP

} sai_fdb_entry_packet_action_t;


/*
*  FDB entry information 
*/
typedef struct _sai_fdb_entry_t
{
    sai_mac_t mac_address;
    sai_vlan_id_t vlan_id;
    sai_port_id_t port_id;
    sai_fdb_entry_type_t entry_type;
    sai_fdb_entry_packet_action_t action;
} sai_fdb_entry_t;


/*
*  FDB event type
*/
typedef enum sai_fdb_event_t
{
    SAI_FDN_EVENT_UNSPEFICIED,

    /* New FDB entry learned */
    SAI_FDN_EVENT_LEARNED,

    /* FDB entry aged */
    SAI_FDN_EVENT_AGED,

    /* FDB entry flushd */
    SAI_FDN_EVENT_FLUSHED,
    
} sai_fdb_event_t;

/*
*  Attribute Id in sai_set_fdb_attribute() and 
*  sai_get_fdb_attribute() calls
*/
typedef enum _sai_fdb_attr_t 
{
    /* READ-WRITE (global attributes) */

    /* Dynamic FDB entry aging time in seconds [uint32_t] 
    *   Zero means aging is disabled.
    */
    SAI_FDB_ATTR_AGING_TIME,


    /* READ-ONLY */

    /* The size of the FDB Table in bytes [uint32_t] */
    SAI_FDB_ATTR_TABLE_SIZE,

    /* -- */

    /* Custom range base value */
    SAI_FDB_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_fdb_attr_t;


/*
* Routine Description:
*    Set FDB table attribute
*
* Arguments:
*    [in] attribute - FDB attribute
*    [in] value - FDB attribute value
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_fdb_attribute_fn)(
    _In_ sai_fdb_attr_t attribute, 
    _In_ uint64_t value
    );


/*
* Routine Description:
*    Get FDB table attribute
*
* Arguments:
*    [in] attribute - FDB attribute
*    [out] value - FDB attribute value
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_fdb_attribute_fn)(
    _In_ sai_fdb_attr_t attribute, 
    _Out_ uint64_t* value
    );


/*
* Routine Description:
*    Initiate deletion of all FDB entries
*
* Arguments:
*    None
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_flush_all_fdb_entries_fn)(void);


/*
* Routine Description:
*    Delete all FDB entries by port
*
* Arguments:
*    [in] port_id - port id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_flush_all_fdb_entries_by_port_fn)(
    _In_ sai_port_id_t port_id
    );

/*
* Routine Description:
*    Delete all FDB entries by vlan
*
* Arguments:
*    [in] vlan_id - vlan id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_flush_all_fdb_entries_by_vlan_fn)(
    _In_ sai_vlan_id_t vlan_id
    );

/*
* Routine Description:
*    Delete all FDB entries by port + vlan combination
*
* Arguments:
*    [in] port_id - port id
*    [in] vlan_id - vlan id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_flush_all_fdb_entries_by_port_vlan_fn)(
    _In_ sai_port_id_t port_id,
    _In_ sai_vlan_id_t vlan_id
    );

/*
* Routine Description:
*     Add FDB entry to the table
*
* Arguments:
*    [in] fdb_count - number of fdb entries to add
*    [in] fdb_entries - array of fdb entries
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_fdb_entries_fn)(
    _In_ uint32_t fdb_count,
    _In_ sai_fdb_entry_t* fdb_entries
    );

/*
* Routine Description:
*     Delete FDB entry from the table (by MAC address and VLAN id)
*
* Arguments:
*    [in] fdb_count - number of fdb entries to flush
*    [in] fdb_entries - array of fdb entries
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_flush_fdb_entries_fn)(
    _In_ uint32_t fdb_count,
    _In_ sai_fdb_entry_t* fdb_entries
    );


/*
* Routine Description:
*     FDB notifications
*
* Arguments:
*    [in] event_type - FDB event type
*    [in] fdb_count - number of fdb entries
*    [in] fdb_entries - array of fdb entries
*
* Return Values:
*    None
*/
typedef void (*sai_fdb_event_notification_fn)(
    _In_ sai_fdb_event_t event_type,
    _In_ uint32_t fdb_count,
    _In_ sai_fdb_entry_t* fdb_entries
    );

/*
* FDB method table retrieved with sai_api_query()
*/
typedef struct _sai_fdb_api_t
{
    sai_create_fdb_entries_fn                   create_fdb_entries;
    sai_flush_fdb_entries_fn                    flush_fdb_entries;
    sai_flush_all_fdb_entries_fn                flush_all_fdb_entries;
    sai_flush_all_fdb_entries_by_port_fn        flush_all_fdb_entries_by_port;
    sai_flush_all_fdb_entries_by_vlan_fn        flush_all_fdb_entries_by_vlan;
    sai_flush_all_fdb_entries_by_port_vlan_fn   flush_all_fdb_entries_by_port_vlan;
    sai_set_fdb_attribute_fn                    set_attribute;
    sai_get_fdb_attribute_fn                    get_attribute;

} sai_fdb_api_t;

#endif // __SAIFDB_H_
