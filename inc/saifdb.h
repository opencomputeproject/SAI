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

    /* FDB entry port id [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_AND_SET)
     * The port id here can refer to a generic port object such as SAI port object id,
     * SAI LAG object id and etc. on. */
    SAI_FDB_ENTRY_ATTR_PORT_ID,

    /* FDB entry packet action [sai_packet_action_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_FDB_ENTRY_ATTR_PACKET_ACTION,

    /* -- */

    /* Custom range base value */
    SAI_FDB_ENTRY_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_fdb_entry_attr_t;

/*
*  FDB Flush entry type.
*/
typedef enum _sai_fdb_flush_entry_type_t
{
    /* Flush dynamic FDB entries */
    SAI_FDB_FLUSH_ENTRY_DYNAMIC,

    /* Flush static FDB entries */
    SAI_FDB_FLUSH_ENTRY_STATIC,

} sai_fdb_flush_entry_type_t;

/*
* Attributei for FDB flush API to specify the type of FDB entries being flushed.
* For example, if you want to flush all static entries, set SAI_FDB_FLUSH_ATTR_ENTRY_TYPE
* = SAI_FDB_FLUSH_ENTRY_STATIC. If you want to flush both static and dynamic entries,
* then there is no need to specify the SAI_FDB_FLUSH_ATTR_ENTRY_TYPE attribute.
* The API uses AND operation when multiple attributes are specified. For
* exmaple,
* 1) Flush all entries in fdb table - Do not specify any attribute
* 2) Flush all entries by port - Set SAI_FDB_FLUSH_ATTR_PORT_ID
* 3) Flush all entries by VLAN - Set SAI_FDB_FLUSH_ATTR_VLAN_ID
* 4) Flush all entries by port and VLAN - Set SAI_FDB_FLUSH_ATTR_PORT_ID and
*    SAI_FDB_FLUSH_ATTR_VLAN_ID
* 5) Flush all static entries by port and VLAN - Set SAI_FDB_FLUSH_ATTR_ENTRY_TYPE,
*    SAI_FDB_FLUSH_ATTR_PORT_ID, and SAI_FDB_FLUSH_ATTR_VLAN_ID
*/
typedef enum _sai_fdb_flush_attr {

   /*Flush based on port [sai_object_id_t]*/
   SAI_FDB_FLUSH_ATTR_PORT_ID,

   /*Flush based on VLAN [sai_vlan_id_t]*/
   SAI_FDB_FLUSH_ATTR_VLAN_ID,

   /*Flush based on entry type [sai_fdb_flush_entry_type_t]*/
   SAI_FDB_FLUSH_ATTR_ENTRY_TYPE,

}sai_fdb_flush_attr;

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
    _In_ uint32_t attr_count,
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
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/*
* Routine Description:
*    Remove all FDB entries by attribute set in sai_fdb_flush_attr
*
* Arguments:
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_flush_fdb_entries_fn)(
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
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
    _In_ uint32_t attr_count,
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
    sai_flush_fdb_entries_fn                    flush_fdb_entries;

} sai_fdb_api_t;

#endif // __SAIFDB_H_
