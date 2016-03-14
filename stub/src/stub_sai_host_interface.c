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
#ifndef _WIN32
#include <net/if.h>
#endif

#undef  __MODULE__
#define __MODULE__ SAI_HOST_INTERFACE

static const sai_attribute_entry_t host_interface_attribs[] = {
    { SAI_HOSTIF_ATTR_TYPE, true, true, false, true,
      "Host interface type", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_HOSTIF_ATTR_RIF_OR_PORT_ID, false, true, false, true,
      "Host interface associated port or router interface", SAI_ATTR_VAL_TYPE_OID },
    { SAI_HOSTIF_ATTR_NAME, true, true, true, true,
      "Host interface name", SAI_ATTR_VAL_TYPE_CHARDATA },
    { END_FUNCTIONALITY_ATTRIBS_ID, false, false, false, false,
      "", SAI_ATTR_VAL_TYPE_UNDETERMINED }
};

sai_status_t stub_host_interface_type_get(_In_ const sai_object_key_t   *key,
                                          _Inout_ sai_attribute_value_t *value,
                                          _In_ uint32_t                  attr_index,
                                          _Inout_ vendor_cache_t        *cache,
                                          void                          *arg);
sai_status_t stub_host_interface_rif_port_get(_In_ const sai_object_key_t   *key,
                                              _Inout_ sai_attribute_value_t *value,
                                              _In_ uint32_t                  attr_index,
                                              _Inout_ vendor_cache_t        *cache,
                                              void                          *arg);
sai_status_t stub_host_interface_name_get(_In_ const sai_object_key_t   *key,
                                          _Inout_ sai_attribute_value_t *value,
                                          _In_ uint32_t                  attr_index,
                                          _Inout_ vendor_cache_t        *cache,
                                          void                          *arg);
sai_status_t stub_host_interface_name_set(_In_ const sai_object_key_t      *key,
                                          _In_ const sai_attribute_value_t *value,
                                          void                             *arg);

static const sai_vendor_attribute_entry_t host_interface_vendor_attribs[] = {
    { SAI_HOSTIF_ATTR_TYPE,
      { true, false, false, true },
      { true, false, false, true },
      stub_host_interface_type_get, NULL,
      NULL, NULL },
    { SAI_HOSTIF_ATTR_RIF_OR_PORT_ID,
      { true, false, false, true },
      { true, false, false, true },
      stub_host_interface_rif_port_get, NULL,
      NULL, NULL },
    { SAI_HOSTIF_ATTR_NAME,
      { true, false, true, true },
      { true, false, true, true },
      stub_host_interface_name_get, NULL,
      stub_host_interface_name_set, NULL },
};
static void host_interface_key_to_str(_In_ sai_object_id_t hif_id, _Out_ char *key_str)
{
    uint32_t hif_data;

    if (SAI_STATUS_SUCCESS != stub_object_to_type(hif_id, SAI_OBJECT_TYPE_HOST_INTERFACE, &hif_data)) {
        snprintf(key_str, MAX_KEY_STR_LEN, "invalid host interface");
    } else {
        snprintf(key_str, MAX_KEY_STR_LEN, "host interface %u", hif_data);
    }
}

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
sai_status_t stub_create_host_interface(_Out_ sai_object_id_t     * hif_id,
                                        _In_ uint32_t               attr_count,
                                        _In_ const sai_attribute_t *attr_list)
{
    sai_status_t                 status;
    int                          ret;
    const sai_attribute_value_t *type, *rif_port, *name;
    uint32_t                     type_index, rif_port_index, name_index, rif_data;
    char                         key_str[MAX_KEY_STR_LEN];
    char                         list_str[MAX_LIST_VALUE_STR_LEN];
    static uint32_t              next_id = 0;
    char                         system_cmd[1024];

    STUB_LOG_ENTER();

    if (NULL == hif_id) {
        STUB_LOG_ERR("NULL host interface ID param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (SAI_STATUS_SUCCESS !=
        (status =
             check_attribs_metadata(attr_count, attr_list, host_interface_attribs, host_interface_vendor_attribs,
                                    SAI_OPERATION_CREATE))) {
        STUB_LOG_ERR("Failed attribs check\n");
        return status;
    }

    sai_attr_list_to_str(attr_count, attr_list, host_interface_attribs, MAX_LIST_VALUE_STR_LEN, list_str);
    STUB_LOG_NTC("Create host interface, %s\n", list_str);

    assert(SAI_STATUS_SUCCESS ==
           find_attrib_in_list(attr_count, attr_list, SAI_HOSTIF_ATTR_TYPE, &type, &type_index));
    assert(SAI_STATUS_SUCCESS ==
           find_attrib_in_list(attr_count, attr_list, SAI_HOSTIF_ATTR_NAME, &name, &name_index));

    if (SAI_HOSTIF_TYPE_NETDEV == type->s32) {
        if (SAI_STATUS_SUCCESS !=
            (status =
                 find_attrib_in_list(attr_count, attr_list, SAI_HOSTIF_ATTR_RIF_OR_PORT_ID, &rif_port,
                                     &rif_port_index))) {
            STUB_LOG_ERR("Missing mandatory attribute rif port id on create of host if netdev type\n");
            return SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING;
        }

        if (SAI_OBJECT_TYPE_ROUTER_INTERFACE == sai_object_type_query(rif_port->oid)) {
            if (SAI_STATUS_SUCCESS !=
                (status = stub_object_to_type(rif_port->oid, SAI_OBJECT_TYPE_ROUTER_INTERFACE, &rif_data))) {
                return status;
            }
        } else if (SAI_OBJECT_TYPE_PORT == sai_object_type_query(rif_port->oid)) {
            if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(rif_port->oid, SAI_OBJECT_TYPE_PORT, &rif_data))) {
                return status;
            }
        } else {
            STUB_LOG_ERR("Invalid rif port object type %s", SAI_TYPE_STR(sai_object_type_query(rif_port->oid)));
            return SAI_STATUS_INVALID_ATTR_VALUE_0 + rif_port_index;
        }
        snprintf(system_cmd, sizeof(system_cmd), "ip link add name %s type dummy", name->chardata);
        ret = system(system_cmd);
        if (0 != ret) {
            STUB_LOG_INF("Error on attempt to create dummy interface. Possibly interface already exists");
            return SAI_STATUS_SUCCESS;
        }

    } else if (SAI_HOSTIF_TYPE_FD == type->s32) {
    } else {
        STUB_LOG_ERR("Invalid host interface type %d\n", type->s32);
        return SAI_STATUS_INVALID_ATTR_VALUE_0 + type_index;
    }

    if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_HOST_INTERFACE, next_id++, hif_id))) {
        return status;
    }
    host_interface_key_to_str(*hif_id, key_str);
    STUB_LOG_NTC("Created host interface %s\n", key_str);

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

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
sai_status_t stub_remove_host_interface(_In_ sai_object_id_t hif_id)
{
    char         key_str[MAX_KEY_STR_LEN];
    uint32_t     hif_data;
    sai_status_t status;

    STUB_LOG_ENTER();

    host_interface_key_to_str(hif_id, key_str);
    STUB_LOG_NTC("Remove host interface %s\n", key_str);

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(hif_id, SAI_OBJECT_TYPE_HOST_INTERFACE, &hif_data))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

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
sai_status_t stub_set_host_interface_attribute(_In_ sai_object_id_t hif_id, _In_ const sai_attribute_t *attr)
{
    const sai_object_key_t key = { .object_id = hif_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    host_interface_key_to_str(hif_id, key_str);
    return sai_set_attribute(&key, key_str, host_interface_attribs, host_interface_vendor_attribs, attr);
}

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
sai_status_t stub_get_host_interface_attribute(_In_ sai_object_id_t     hif_id,
                                               _In_ uint32_t            attr_count,
                                               _Inout_ sai_attribute_t *attr_list)
{
    const sai_object_key_t key = { .object_id = hif_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    host_interface_key_to_str(hif_id, key_str);
    return sai_get_attributes(&key,
                              key_str,
                              host_interface_attribs,
                              host_interface_vendor_attribs,
                              attr_count,
                              attr_list);
}

/* Type [sai_host_interface_type_t] */
sai_status_t stub_host_interface_type_get(_In_ const sai_object_key_t   *key,
                                          _Inout_ sai_attribute_value_t *value,
                                          _In_ uint32_t                  attr_index,
                                          _Inout_ vendor_cache_t        *cache,
                                          void                          *arg)
{
    STUB_LOG_ENTER();

    value->s32 = SAI_HOSTIF_TYPE_NETDEV;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Assosiated port or router interface [sai_object_id_t] */
sai_status_t stub_host_interface_rif_port_get(_In_ const sai_object_key_t   *key,
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

/* Name [char[HOST_INTERFACE_NAME_SIZE]] (MANDATORY_ON_CREATE)
 * The maximum number of charactars for the name is HOST_INTERFACE_NAME_SIZE - 1 since
 * it needs the terminating null byte ('\0') at the end.  */
sai_status_t stub_host_interface_name_get(_In_ const sai_object_key_t   *key,
                                          _Inout_ sai_attribute_value_t *value,
                                          _In_ uint32_t                  attr_index,
                                          _Inout_ vendor_cache_t        *cache,
                                          void                          *arg)
{
    uint32_t     hif_id;
    sai_status_t status;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_HOST_INTERFACE, &hif_id))) {
        return status;
    }

    strncpy(value->chardata, "name", HOSTIF_NAME_SIZE);

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Name [char[HOST_INTERFACE_NAME_SIZE]]
 * The maximum number of charactars for the name is HOST_INTERFACE_NAME_SIZE - 1 since
 * it needs the terminating null byte ('\0') at the end.  */
sai_status_t stub_host_interface_name_set(_In_ const sai_object_key_t      *key,
                                          _In_ const sai_attribute_value_t *value,
                                          void                             *arg)
{
    uint32_t     hif_id;
    sai_status_t status;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_HOST_INTERFACE, &hif_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

sai_status_t stub_set_host_interface_trap_attribute(_In_ sai_hostif_trap_id_t  hostif_trapid,
                                                    _In_ const sai_attribute_t *attr)
{
    STUB_LOG_ENTER();
    STUB_LOG_EXIT();

    return SAI_STATUS_SUCCESS;
}


const sai_hostif_api_t host_interface_api = {
    stub_create_host_interface,
    stub_remove_host_interface,
    stub_set_host_interface_attribute,
    stub_get_host_interface_attribute,
    NULL,
    NULL,
    NULL,
    NULL,
    stub_set_host_interface_trap_attribute,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};
