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
#define __MODULE__ SAI_RIF

static const sai_attribute_entry_t rif_attribs[] = {
    { SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID, true, true, false, true,
      "Router interface virtual router ID", SAI_ATTR_VAL_TYPE_OID },
    { SAI_ROUTER_INTERFACE_ATTR_TYPE, true, true, false, true,
      "Router interface type", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_ROUTER_INTERFACE_ATTR_PORT_ID, false, true, false, true,
      "Router interface port ID", SAI_ATTR_VAL_TYPE_OID },
    { SAI_ROUTER_INTERFACE_ATTR_VLAN_ID, false, true, false, true,
      "Router interface vlan ID", SAI_ATTR_VAL_TYPE_U16 },
    { SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS, false, true, true, true,
      "Router interface source MAC address", SAI_ATTR_VAL_TYPE_MAC },
    { SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE, false, true, true, true,
      "Router interface admin v4 state", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE, false, true, true, true,
      "Router interface admin v6 state", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_ROUTER_INTERFACE_ATTR_MTU, false, true, true, true,
      "Router interface mtu", SAI_ATTR_VAL_TYPE_U32 },
    { END_FUNCTIONALITY_ATTRIBS_ID, false, false, false, false,
      "", SAI_ATTR_VAL_TYPE_UNDETERMINED }
};

sai_status_t stub_rif_attrib_get(_In_ const sai_object_key_t   *key,
                                 _Inout_ sai_attribute_value_t *value,
                                 _In_ uint32_t                  attr_index,
                                 _Inout_ vendor_cache_t        *cache,
                                 void                          *arg);
sai_status_t stub_rif_admin_get(_In_ const sai_object_key_t   *key,
                                _Inout_ sai_attribute_value_t *value,
                                _In_ uint32_t                  attr_index,
                                _Inout_ vendor_cache_t        *cache,
                                void                          *arg);
sai_status_t stub_rif_attrib_set(_In_ const sai_object_key_t      *key,
                                 _In_ const sai_attribute_value_t *value,
                                 void                             *arg);
sai_status_t stub_rif_admin_set(_In_ const sai_object_key_t      *key,
                                _In_ const sai_attribute_value_t *value,
                                void                             *arg);

static const sai_vendor_attribute_entry_t rif_vendor_attribs[] = {
    { SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
      { true, false, false, true },
      { true, false, false, true },
      stub_rif_attrib_get, (void*)SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
      NULL, NULL },
    { SAI_ROUTER_INTERFACE_ATTR_TYPE,
      { true, false, false, true },
      { true, false, false, true },
      stub_rif_attrib_get, (void*)SAI_ROUTER_INTERFACE_ATTR_TYPE,
      NULL, NULL },
    { SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
      { true, false, false, true },
      { true, false, false, true },
      stub_rif_attrib_get, (void*)SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
      NULL, NULL },
    { SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
      { true, false, false, true },
      { true, false, false, true },
      stub_rif_attrib_get, (void*)SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
      NULL, NULL },
    { SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
      { true, false, true, true },
      { true, false, true, true },
      stub_rif_attrib_get, (void*)SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
      stub_rif_attrib_set, (void*)SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS },
    { SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE,
      { true, false, true, true },
      { true, false, true, true },
      stub_rif_admin_get, (void*)SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE,
      stub_rif_admin_set, (void*)SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE },
    { SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE,
      { true, false, true, true },
      { true, false, true, true },
      stub_rif_admin_get, (void*)SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE,
      stub_rif_admin_set, (void*)SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE },
    { SAI_ROUTER_INTERFACE_ATTR_MTU,
      { true, false, true, true },
      { true, false, true, true },
      stub_rif_attrib_get, (void*)SAI_ROUTER_INTERFACE_ATTR_MTU,
      stub_rif_attrib_set, (void*)SAI_ROUTER_INTERFACE_ATTR_MTU }
};
static void rif_key_to_str(_In_ sai_object_id_t rif_id, _Out_ char *key_str)
{
    uint32_t rifid;

    if (SAI_STATUS_SUCCESS != stub_object_to_type(rif_id, SAI_OBJECT_TYPE_ROUTER_INTERFACE, &rifid)) {
        snprintf(key_str, MAX_KEY_STR_LEN, "invalid rif");
    } else {
        snprintf(key_str, MAX_KEY_STR_LEN, "rif %u", rifid);
    }
}

/*
 * Routine Description:
 *    Create router interface.
 *
 * Arguments:
 *    [out] rif_id - router interface id
 *    [in] attr_count - number of attributes
 *    [in] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_create_router_interface(_Out_ sai_object_id_t* rif_id,
                                          _In_ uint32_t          attr_count,
                                          _In_ const sai_attribute_t  *attr_list)
{
    sai_status_t                 status;
    const sai_attribute_value_t *type, *vrid, *port, *vlan;
    uint32_t                     type_index, vrid_index, port_index, vlan_index, vrid_data, port_data;
    char                         list_str[MAX_LIST_VALUE_STR_LEN];
    char                         key_str[MAX_KEY_STR_LEN];
    static uint32_t              next_id = 0;

    STUB_LOG_ENTER();

    if (NULL == rif_id) {
        STUB_LOG_ERR("NULL rif id param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (SAI_STATUS_SUCCESS !=
        (status =
             check_attribs_metadata(attr_count, attr_list, rif_attribs, rif_vendor_attribs, SAI_OPERATION_CREATE))) {
        STUB_LOG_ERR("Failed attribs check\n");
        return status;
    }

    sai_attr_list_to_str(attr_count, attr_list, rif_attribs, MAX_LIST_VALUE_STR_LEN, list_str);
    STUB_LOG_NTC("Create rif, %s\n", list_str);

    assert(SAI_STATUS_SUCCESS ==
           find_attrib_in_list(attr_count, attr_list, SAI_ROUTER_INTERFACE_ATTR_TYPE, &type, &type_index));
    assert(SAI_STATUS_SUCCESS ==
           find_attrib_in_list(attr_count, attr_list, SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID, &vrid,
                               &vrid_index));
    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(vrid->oid, SAI_OBJECT_TYPE_VIRTUAL_ROUTER, &vrid_data))) {
        return status;
    }

    if (SAI_ROUTER_INTERFACE_TYPE_VLAN == type->s32) {
        if (SAI_STATUS_SUCCESS !=
            (status =
                 find_attrib_in_list(attr_count, attr_list, SAI_ROUTER_INTERFACE_ATTR_VLAN_ID, &vlan, &vlan_index))) {
            STUB_LOG_ERR("Missing mandatory attribute vlan id on create\n");
            return SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING;
        }
        if (SAI_STATUS_ITEM_NOT_FOUND !=
            (status =
                 find_attrib_in_list(attr_count, attr_list, SAI_ROUTER_INTERFACE_ATTR_PORT_ID, &port, &port_index))) {
            STUB_LOG_ERR("Invalid attribute port id for rif vlan on create\n");
            return SAI_STATUS_INVALID_ATTRIBUTE_0 + port_index;
        }
    } else if (SAI_ROUTER_INTERFACE_TYPE_PORT == type->s32) {
        if (SAI_STATUS_SUCCESS !=
            (status =
                 find_attrib_in_list(attr_count, attr_list, SAI_ROUTER_INTERFACE_ATTR_PORT_ID, &port, &port_index))) {
            STUB_LOG_ERR("Missing mandatory attribute port id on create\n");
            return SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING;
        }
        if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(port->oid, SAI_OBJECT_TYPE_PORT, &port_data))) {
            return status;
        }
        if (SAI_STATUS_ITEM_NOT_FOUND !=
            (status =
                 find_attrib_in_list(attr_count, attr_list, SAI_ROUTER_INTERFACE_ATTR_VLAN_ID, &vlan, &vlan_index))) {
            STUB_LOG_ERR("Invalid attribute vlan id for rif port on create\n");
            return SAI_STATUS_INVALID_ATTRIBUTE_0 + vlan_index;
        }
    } else {
        STUB_LOG_ERR("Invalid router interface type %d\n", type->s32);
        return SAI_STATUS_INVALID_ATTR_VALUE_0 + type_index;
    }

    if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_ROUTER_INTERFACE, next_id++, rif_id))) {
        return status;
    }
    rif_key_to_str(*rif_id, key_str);
    STUB_LOG_NTC("Created rif %s\n", key_str);

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Remove router interface
 *
 * Arguments:
 *    [in] rif_id - router interface id
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_remove_router_interface(_In_ sai_object_id_t rif_id)
{
    sai_status_t status;
    uint32_t     data;
    char         key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    rif_key_to_str(rif_id, key_str);
    STUB_LOG_NTC("Remove rif %s\n", key_str);

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(rif_id, SAI_OBJECT_TYPE_ROUTER_INTERFACE, &data))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Set router interface attribute
 *
 * Arguments:
 *    [in] rif_id - router interface id
 *    [in] attr - attribute
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_set_router_interface_attribute(_In_ sai_object_id_t rif_id, _In_ const sai_attribute_t *attr)
{
    const sai_object_key_t key = { .object_id = rif_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    rif_key_to_str(rif_id, key_str);
    return sai_set_attribute(&key, key_str, rif_attribs, rif_vendor_attribs, attr);
}

/*
 * Routine Description:
 *    Get router interface attribute
 *
 * Arguments:
 *    [in] rif_id - router interface id
 *    [in] attr_count - number of attributes
 *    [inout] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_get_router_interface_attribute(_In_ sai_object_id_t     rif_id,
                                                 _In_ uint32_t            attr_count,
                                                 _Inout_ sai_attribute_t *attr_list)
{
    const sai_object_key_t key = { .object_id = rif_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    rif_key_to_str(rif_id, key_str);
    return sai_get_attributes(&key, key_str, rif_attribs, rif_vendor_attribs, attr_count, attr_list);
}

/* MAC Address [sai_mac_t] */
/* MTU [uint32_t] */
sai_status_t stub_rif_attrib_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value, void *arg)
{
    uint32_t     data;
    sai_status_t status;

    STUB_LOG_ENTER();

    assert((SAI_ROUTER_INTERFACE_ATTR_MTU == (int64_t)arg) ||
           (SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS == (int64_t)arg));

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_ROUTER_INTERFACE, &data))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Admin State V4, V6 [bool] */
sai_status_t stub_rif_admin_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value, void *arg)
{
    sai_status_t status;
    uint32_t     data;

    STUB_LOG_ENTER();

    assert((SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE == (int64_t)arg) ||
           (SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE == (int64_t)arg));

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_ROUTER_INTERFACE, &data))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Virtual router id [sai_object_id_t] */
/* Type [sai_router_interface_type_t] */
/* Assosiated Port or Lag object id [sai_object_id_t] */
/* Assosiated Vlan [sai_vlan_id_t] */
/* MAC Address [sai_mac_t] */
/* MTU [uint32_t] */
sai_status_t stub_rif_attrib_get(_In_ const sai_object_key_t   *key,
                                 _Inout_ sai_attribute_value_t *value,
                                 _In_ uint32_t                  attr_index,
                                 _Inout_ vendor_cache_t        *cache,
                                 void                          *arg)
{
    sai_status_t status;
    uint32_t     data;

    STUB_LOG_ENTER();

    assert((SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID == (int64_t)arg) ||
           (SAI_ROUTER_INTERFACE_ATTR_TYPE == (int64_t)arg) ||
           (SAI_ROUTER_INTERFACE_ATTR_PORT_ID == (int64_t)arg) || (SAI_ROUTER_INTERFACE_ATTR_VLAN_ID == (int64_t)arg) ||
           (SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS == (int64_t)arg) ||
           (SAI_ROUTER_INTERFACE_ATTR_MTU == (int64_t)arg));

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_ROUTER_INTERFACE, &data))) {
        return status;
    }

    switch ((int64_t)arg) {
    case SAI_ROUTER_INTERFACE_ATTR_PORT_ID:
        if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_PORT, 0, &value->oid))) {
            return status;
        }
        break;

    case SAI_ROUTER_INTERFACE_ATTR_VLAN_ID:
        value->u16 = 1;
        break;

    case SAI_ROUTER_INTERFACE_ATTR_MTU:
        value->u32 = 1514;
        break;

    case SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS:
        break;

    case SAI_ROUTER_INTERFACE_ATTR_TYPE:
        value->s32 = SAI_ROUTER_INTERFACE_TYPE_PORT;
        break;

    case SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID:
        if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_VIRTUAL_ROUTER,
                                                               0, &value->oid))) {
            return status;
        }
        break;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Admin State V4, V6 [bool] */
sai_status_t stub_rif_admin_get(_In_ const sai_object_key_t   *key,
                                _Inout_ sai_attribute_value_t *value,
                                _In_ uint32_t                  attr_index,
                                _Inout_ vendor_cache_t        *cache,
                                void                          *arg)
{
    sai_status_t status;
    uint32_t     data;

    STUB_LOG_ENTER();

    assert((SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE == (int64_t)arg) ||
           (SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE == (int64_t)arg));

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_ROUTER_INTERFACE, &data))) {
        return status;
    }

    value->booldata = true;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

const sai_router_interface_api_t router_interface_api = {
    stub_create_router_interface,
    stub_remove_router_interface,
    stub_set_router_interface_attribute,
    stub_get_router_interface_attribute,
};
