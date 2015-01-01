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
    /* Dynamic FDB Entry */
    SAI_FDB_ENTRY_DYNAMIC,

    /* Static FDB Entry */
    SAI_FDB_ENTRY_STATIC,

} sai_fdb_entry_type_t;

/*
*  FDB entry key
*/
typedef struct _sai_fdb_entry_t
{
    sai_mac_t mac_address;
    sai_vlan_id_t vlan_id;

} sai_fdb_entry_t;


/*
*  FDB event type
*/
typedef enum sai_fdb_event_t
{
    /* New FDB entry learned */
    SAI_FDB_EVENT_LEARNED,

    /* FDB entry aged */
    SAI_FDB_EVENT_AGED,

    /* FDB entry flushd */
    SAI_FDB_EVENT_FLUSHED,

} sai_fdb_event_t;

/*
*  Attribute Id for fdb entry
*/
typedef enum _sai_fdb_entry_attr_t 
{
    /* READ-ONLY */

    /* READ-WRITE */

    /* FDB entry type [sai_fdb_entry_type_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_FDB_ENTRY_ATTR_TYPE,

    /* FDB entry port id [sai_port_id_t] (MANDATORY_ON_CREATE|CREATE_AND_SET)*/
    SAI_FDB_ENTRY_ATTR_PORT_ID,

    /* FDB entry packet action [sai_packet_action_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_FDB_ENTRY_ATTR_PACKET_ACTION,

    /* -- */

    /* Custom range base value */
    SAI_FDB_ENTRY_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_fdb_entry_attr_t;

/*
* Routine Description:
*    Create FDB entry
*
* Arguments:
*    [in] fdb_entry - fdb entry
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_fdb_entry_fn)(
    _In_ const sai_fdb_entry_t* fdb_entry,
    _In_ int attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/*
* Routine Description:
*    Remove FDB entry
*
* Arguments:
*    [in] fdb_entry - fdb entry
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_remove_fdb_entry_fn)(
    _In_ const sai_fdb_entry_t* fdb_entry
    );

/*
* Routine Description:
*    Set fdb entry attribute value
*
* Arguments:
*    [in] fdb_entry - fdb entry
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_fdb_entry_attribute_fn)(
    _In_ const sai_fdb_entry_t* fdb_entry,
    _In_ const sai_attribute_t *attr
    );

/*
* Routine Description:
*    Get fdb entry attribute value
*
* Arguments:
*    [in] fdb_entry - fdb entry
*    [in] attr_count - number of attributes
*    [inout] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_fdb_entry_attribute_fn)(
    _In_ const sai_fdb_entry_t* fdb_entry,
    _In_ int attr_count,
    _Inout_ sai_attribute_t *attr_list
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
*    Remove all FDB entries by port
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
*    Remove all FDB entries by vlan
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
*    Remove all FDB entries by port + vlan combination
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
*     FDB notifications
*
* Arguments:
*    [in] event_type - FDB event type
*    [in] fdb entry - fdb entry
*    [in] attr_count - number of attributes
*    [in] attr - array of attributes
*
* Return Values:
*    None
*/
typedef void (*sai_fdb_event_notification_fn)(
    _In_ sai_fdb_event_t event_type,
    _In_ sai_fdb_entry_t* fdb_entry,
    _In_ int attr_count,
    _In_ sai_attribute_t *attr
    );

/*
* FDB method table retrieved with sai_api_query()
*/
typedef struct _sai_fdb_api_t
{
    sai_create_fdb_entry_fn                     create_fdb_entry;
    sai_remove_fdb_entry_fn                     remove_fdb_entry;
    sai_set_fdb_entry_attribute_fn              set_fdb_entry_attribute;
    sai_get_fdb_entry_attribute_fn              get_fdb_entry_attribute;
    sai_flush_all_fdb_entries_fn                flush_all_fdb_entries;
    sai_flush_all_fdb_entries_by_port_fn        flush_all_fdb_entries_by_port;
    sai_flush_all_fdb_entries_by_vlan_fn        flush_all_fdb_entries_by_vlan;
    sai_flush_all_fdb_entries_by_port_vlan_fn   flush_all_fdb_entries_by_port_vlan;

} sai_fdb_api_t;

#endif // __SAIFDB_H_
