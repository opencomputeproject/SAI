/*
 *  Copyright (C) 2014. Mellanox Technologies, Ltd. ALL RIGHTS RESERVED.
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
 */

#include "sai.h"
#include "stub_sai.h"
#include "assert.h"

#undef  __MODULE__
#define __MODULE__ SAI_FDB

sai_status_t stub_fdb_type_set(_In_ const sai_object_key_t      *key,
                               _In_ const sai_attribute_value_t *value,
                               void                             *arg);
sai_status_t stub_fdb_port_set(_In_ const sai_object_key_t      *key,
                               _In_ const sai_attribute_value_t *value,
                               void                             *arg);
sai_status_t stub_fdb_action_set(_In_ const sai_object_key_t      *key,
                                 _In_ const sai_attribute_value_t *value,
                                 void                             *arg);
sai_status_t stub_fdb_type_get(_In_ const sai_object_key_t   *key,
                               _Inout_ sai_attribute_value_t *value,
                               _In_ uint32_t                  attr_index,
                               _Inout_ vendor_cache_t        *cache,
                               void                          *arg);
sai_status_t stub_fdb_port_get(_In_ const sai_object_key_t   *key,
                               _Inout_ sai_attribute_value_t *value,
                               _In_ uint32_t                  attr_index,
                               _Inout_ vendor_cache_t        *cache,
                               void                          *arg);
sai_status_t stub_fdb_action_get(_In_ const sai_object_key_t   *key,
                                 _Inout_ sai_attribute_value_t *value,
                                 _In_ uint32_t                  attr_index,
                                 _Inout_ vendor_cache_t        *cache,
                                 void                          *arg);

static const sai_attribute_entry_t        fdb_attribs[] = {
    { SAI_FDB_ENTRY_ATTR_TYPE, true, true, true, true,
      "FDB entry type", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_FDB_ENTRY_ATTR_PORT_ID, true, true, true, true,
      "FDB entry port id", SAI_ATTR_VAL_TYPE_OID},
    { SAI_FDB_ENTRY_ATTR_PACKET_ACTION, true, true, true, true,
      "FDB entry packet action", SAI_ATTR_VAL_TYPE_S32 },
    { END_FUNCTIONALITY_ATTRIBS_ID, false, false, false, false,
      "", SAI_ATTR_VAL_TYPE_UNDETERMINED }
};
static const sai_vendor_attribute_entry_t fdb_vendor_attribs[] = {
    { SAI_FDB_ENTRY_ATTR_TYPE,
      { true, false, true, true },
      { true, false, true, true },
      stub_fdb_type_get, NULL,
      stub_fdb_type_set, NULL },
    { SAI_FDB_ENTRY_ATTR_PORT_ID,
      { true, false, true, true },
      { true, false, true, true },
      stub_fdb_port_get, NULL,
      stub_fdb_port_set, NULL },
    { SAI_FDB_ENTRY_ATTR_PACKET_ACTION,
      { true, false, true, true },
      { true, false, true, true },
      stub_fdb_action_get, NULL,
      stub_fdb_action_set, NULL }
};
static void fdb_key_to_str(_In_ const sai_fdb_entry_t* fdb_entry, _Out_ char *key_str)
{
    snprintf(key_str, MAX_KEY_STR_LEN, "fdb entry mac [%02x:%02x:%02x:%02x:%02x:%02x] vlan %u",
             fdb_entry->mac_address[0],
             fdb_entry->mac_address[1],
             fdb_entry->mac_address[2],
             fdb_entry->mac_address[3],
             fdb_entry->mac_address[4],
             fdb_entry->mac_address[5],
             fdb_entry->vlan_id);
}

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
sai_status_t stub_create_fdb_entry(_In_ const sai_fdb_entry_t* fdb_entry,
                                   _In_ uint32_t               attr_count,
                                   _In_ const sai_attribute_t *attr_list)
{
    sai_status_t                 status;
    const sai_attribute_value_t *type, *action, *port;
    uint32_t                     type_index, action_index, port_index, port_id;
    char                         key_str[MAX_KEY_STR_LEN];
    char                         list_str[MAX_LIST_VALUE_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == fdb_entry) {
        STUB_LOG_ERR("NULL fdb entry param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (SAI_STATUS_SUCCESS !=
        (status =
             check_attribs_metadata(attr_count, attr_list, fdb_attribs, fdb_vendor_attribs, SAI_OPERATION_CREATE))) {
        STUB_LOG_ERR("Failed attribs check\n");
        return status;
    }

    fdb_key_to_str(fdb_entry, key_str);
    sai_attr_list_to_str(attr_count, attr_list, fdb_attribs, MAX_LIST_VALUE_STR_LEN, list_str);
    STUB_LOG_NTC("Create FDB entry %s\n", key_str);
    STUB_LOG_NTC("Attribs %s\n", list_str);

    assert(SAI_STATUS_SUCCESS == find_attrib_in_list(attr_count,
                                                     attr_list,
                                                     SAI_FDB_ENTRY_ATTR_TYPE,
                                                     &type,
                                                     &type_index));
    assert(SAI_STATUS_SUCCESS ==
           find_attrib_in_list(attr_count, attr_list, SAI_FDB_ENTRY_ATTR_PACKET_ACTION, &action, &action_index));
    assert(SAI_STATUS_SUCCESS ==
           find_attrib_in_list(attr_count, attr_list, SAI_FDB_ENTRY_ATTR_PORT_ID, &port, &port_index));

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(port->oid, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

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
sai_status_t stub_remove_fdb_entry(_In_ const sai_fdb_entry_t* fdb_entry)
{
    char key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == fdb_entry) {
        STUB_LOG_ERR("NULL fdb entry param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    fdb_key_to_str(fdb_entry, key_str);
    STUB_LOG_NTC("Remove FDB entry %s\n", key_str);

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

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
sai_status_t stub_set_fdb_entry_attribute(_In_ const sai_fdb_entry_t* fdb_entry, _In_ const sai_attribute_t *attr)
{
    const sai_object_key_t key = {.fdb_entry = fdb_entry };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == fdb_entry) {
        STUB_LOG_ERR("NULL fdb entry param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    fdb_key_to_str(fdb_entry, key_str);
    return sai_set_attribute(&key, key_str, fdb_attribs, fdb_vendor_attribs, attr);
}

/* Set FDB entry type [sai_fdb_entry_type_t] */
sai_status_t stub_fdb_type_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value, void *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* FDB entry port id [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_AND_SET)
 * The port id here can refer to a generic port object such as SAI port object id,
 * SAI LAG object id and etc. on. */
sai_status_t stub_fdb_port_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value, void *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(value->oid, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Set FDB entry packet action [sai_packet_action_t] */
sai_status_t stub_fdb_action_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value, void *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

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
sai_status_t stub_get_fdb_entry_attribute(_In_ const sai_fdb_entry_t* fdb_entry,
                                          _In_ uint32_t               attr_count,
                                          _Inout_ sai_attribute_t    *attr_list)
{
    const sai_object_key_t key = { .fdb_entry = fdb_entry };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == fdb_entry) {
        STUB_LOG_ERR("NULL fdb entry param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    fdb_key_to_str(fdb_entry, key_str);
    return sai_get_attributes(&key, key_str, fdb_attribs, fdb_vendor_attribs, attr_count, attr_list);
}

/* Get FDB entry type [sai_fdb_entry_type_t] */
sai_status_t stub_fdb_type_get(_In_ const sai_object_key_t   *key,
                               _Inout_ sai_attribute_value_t *value,
                               _In_ uint32_t                  attr_index,
                               _Inout_ vendor_cache_t        *cache,
                               void                          *arg)
{
    STUB_LOG_ENTER();

    value->s32 = SAI_FDB_ENTRY_STATIC;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* FDB entry port id [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_AND_SET)
 * The port id here can refer to a generic port object such as SAI port object id,
 * SAI LAG object id and etc. on.
 * Port 0 is returned for entries with action = drop or action = trap
 * Since port is irrelevant for these actions, even if actual port is set
 * In case the action is changed from drop/trap to forward/log, need to also set port
 */
sai_status_t stub_fdb_port_get(_In_ const sai_object_key_t   *key,
                               _Inout_ sai_attribute_value_t *value,
                               _In_ uint32_t                  attr_index,
                               _Inout_ vendor_cache_t        *cache,
                               void                          *arg)
{
    sai_status_t status;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_PORT, 1, &value->oid))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Get FDB entry packet action [sai_packet_action_t] */
sai_status_t stub_fdb_action_get(_In_ const sai_object_key_t   *key,
                                 _Inout_ sai_attribute_value_t *value,
                                 _In_ uint32_t                  attr_index,
                                 _Inout_ vendor_cache_t        *cache,
                                 void                          *arg)
{
    STUB_LOG_ENTER();

    value->s32 = SAI_PACKET_ACTION_FORWARD;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

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
sai_status_t stub_flush_fdb_entries(_In_ uint32_t attr_count, _In_ const sai_attribute_t *attr_list)
{
    sai_status_t                 status;
    const sai_attribute_value_t *port, *vlan, *type;
    uint32_t                     port_index, vlan_index, type_index;
    uint32_t                     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS ==
        (status =
             find_attrib_in_list(attr_count, attr_list, SAI_FDB_FLUSH_ATTR_PORT_ID,
                                 &port, &port_index))) {
        if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(port->oid, SAI_OBJECT_TYPE_PORT, &port_id))) {
            return status;
        }
    }

    if (SAI_STATUS_SUCCESS ==
        (status =
             find_attrib_in_list(attr_count, attr_list, SAI_FDB_FLUSH_ATTR_VLAN_ID,
                                 &vlan, &vlan_index))) {
    }

    if (SAI_STATUS_SUCCESS ==
        (status =
             find_attrib_in_list(attr_count, attr_list, SAI_FDB_FLUSH_ATTR_ENTRY_TYPE,
                                 &type, &type_index))) {
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

const sai_fdb_api_t fdb_api = {
    stub_create_fdb_entry,
    stub_remove_fdb_entry,
    stub_set_fdb_entry_attribute,
    stub_get_fdb_entry_attribute,
    stub_flush_fdb_entries
};
