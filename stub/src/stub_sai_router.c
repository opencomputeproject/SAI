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
#define __MODULE__ SAI_ROUTER

static const sai_attribute_entry_t router_attribs[] = {
    { SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE, false, true, true, true,
      "Router admin V4 state", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE, false, true, true, true,
      "Router admin V6 state", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS, false, true, true, true,
      "Router source MAC address", SAI_ATTR_VAL_TYPE_MAC },
    { SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION, false, true, true, true,
      "Router action for TTL0/1", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS, false, true, true, true,
      "Router action for IP options", SAI_ATTR_VAL_TYPE_S32 },
    { END_FUNCTIONALITY_ATTRIBS_ID, false, false, false, false,
      "", SAI_ATTR_VAL_TYPE_UNDETERMINED }
};

sai_status_t stub_router_admin_get(_In_ const sai_object_key_t   *key,
                                   _Inout_ sai_attribute_value_t *value,
                                   _In_ uint32_t                  attr_index,
                                   _Inout_ vendor_cache_t        *cache,
                                   void                          *arg);
sai_status_t stub_router_admin_set(_In_ const sai_object_key_t      *key,
                                   _In_ const sai_attribute_value_t *value,
                                   void                             *arg);
sai_status_t stub_router_mac_get(_In_ const sai_object_key_t   *key,
                                 _Inout_ sai_attribute_value_t *value,
                                 _In_ uint32_t                  attr_index,
                                 _Inout_ vendor_cache_t        *cache,
                                 void                          *arg);
sai_status_t stub_router_mac_set(_In_ const sai_object_key_t      *key,
                                 _In_ const sai_attribute_value_t *value,
                                 void                             *arg);
sai_status_t stub_router_violation_get(_In_ const sai_object_key_t   *key,
                                       _Inout_ sai_attribute_value_t *value,
                                       _In_ uint32_t                  attr_index,
                                       _Inout_ vendor_cache_t        *cache,
                                       void                          *arg);
sai_status_t stub_router_violation_set(_In_ const sai_object_key_t      *key,
                                       _In_ const sai_attribute_value_t *value,
                                       void                             *arg);

static const sai_vendor_attribute_entry_t router_vendor_attribs[] = {
    { SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE,
      { true, false, true, true },
      { true, false, true, true },
      stub_router_admin_get, (void*)SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE,
      stub_router_admin_set, (void*)SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE },
    { SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE,
      { true, false, true, true },
      { true, false, true, true },
      stub_router_admin_get, (void*)SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE,
      stub_router_admin_set, (void*)SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE },
    { SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
      { false, false, true, true },
      { false, false, true, true },
      stub_router_mac_get, NULL,
      stub_router_mac_set, NULL },
    { SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION,
      { false, false, true, true },
      { false, false, true, true },
      stub_router_violation_get, (void*)SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION,
      stub_router_violation_set, (void*)SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION },
    { SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS,
      { false, false, true, true },
      { false, false, true, true },
      stub_router_violation_get, (void*)SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS,
      stub_router_violation_set, (void*)SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS }
};
static void router_key_to_str(_In_ sai_object_id_t vr_id, _Out_ char *key_str)
{
    uint32_t vrid;

    if (SAI_STATUS_SUCCESS != stub_object_to_type(vr_id, SAI_OBJECT_TYPE_VIRTUAL_ROUTER, &vrid)) {
        snprintf(key_str, MAX_KEY_STR_LEN, "Invalid vr ID");
    } else {
        snprintf(key_str, MAX_KEY_STR_LEN, "vr ID %u", vrid);
    }
}

/*
 * Routine Description:
 *    Set virtual router attribute Value
 *
 * Arguments:
 *    [in] vr_id - virtual router id
 *    [in] attr - attribute
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_set_virtual_router_attribute(_In_ sai_object_id_t vr_id, _In_ const sai_attribute_t *attr)
{
    const sai_object_key_t key = { .object_id = vr_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    router_key_to_str(vr_id, key_str);
    return sai_set_attribute(&key, key_str, router_attribs, router_vendor_attribs, attr);
}

/*
 * Routine Description:
 *    Get virtual router attribute Value
 *
 * Arguments:
 *    [in] vr_id - virtual router id
 *    [in] attr_count - number of attributes
 *    [in] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_get_virtual_router_attribute(_In_ sai_object_id_t     vr_id,
                                               _In_ uint32_t            attr_count,
                                               _Inout_ sai_attribute_t *attr_list)
{
    const sai_object_key_t key = { .object_id = vr_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    router_key_to_str(vr_id, key_str);
    return sai_get_attributes(&key, key_str, router_attribs, router_vendor_attribs, attr_count, attr_list);
}

/* Admin V4, V6 State [bool] */
sai_status_t stub_router_admin_get(_In_ const sai_object_key_t   *key,
                                   _Inout_ sai_attribute_value_t *value,
                                   _In_ uint32_t                  attr_index,
                                   _Inout_ vendor_cache_t        *cache,
                                   void                          *arg)
{
    sai_status_t status;
    uint32_t     router_id;

    STUB_LOG_ENTER();

    assert((SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE == (int64_t)arg) ||
           (SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE == (int64_t)arg));

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_VIRTUAL_ROUTER, &router_id))) {
        return status;
    }

    value->booldata = true;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Admin V4, V6 State [bool] */
sai_status_t stub_router_admin_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value,
                                   void *arg)
{
    sai_status_t status;
    uint32_t     router_id;

    STUB_LOG_ENTER();

    assert((SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE == (int64_t)arg) ||
           (SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE == (int64_t)arg));

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &router_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* MAC Address [sai_mac_t] */
sai_status_t stub_router_mac_get(_In_ const sai_object_key_t   *key,
                                 _Inout_ sai_attribute_value_t *value,
                                 _In_ uint32_t                  attr_index,
                                 _Inout_ vendor_cache_t        *cache,
                                 void                          *arg)
{
    sai_status_t status;
    uint32_t     router_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_VIRTUAL_ROUTER, &router_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* MAC Address [sai_mac_t] */
sai_status_t stub_router_mac_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value, void *arg)
{
    sai_status_t status;
    uint32_t     router_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &router_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Action for Packets with TTL 0 or 1 [sai_packet_action_t]
 * Action for Packets with IP options [sai_packet_action_t]
 * (default to SAI_PACKET_ACTION_TRAP) */
sai_status_t stub_router_violation_get(_In_ const sai_object_key_t   *key,
                                       _Inout_ sai_attribute_value_t *value,
                                       _In_ uint32_t                  attr_index,
                                       _Inout_ vendor_cache_t        *cache,
                                       void                          *arg)
{
    sai_status_t status;
    uint32_t     router_id;

    STUB_LOG_ENTER();

    assert((SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS == (int64_t)arg) ||
           (SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION == (int64_t)arg));

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_VIRTUAL_ROUTER, &router_id))) {
        return status;
    }

    value->s32 = SAI_PACKET_ACTION_TRAP;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Action for Packets with TTL 0 or 1 [sai_packet_action_t]
 * Action for Packets with IP options [sai_packet_action_t]
 * (default to SAI_PACKET_ACTION_TRAP) */
sai_status_t stub_router_violation_set(_In_ const sai_object_key_t      *key,
                                       _In_ const sai_attribute_value_t *value,
                                       void                             *arg)
{
    sai_status_t status;
    uint32_t     router_id;

    STUB_LOG_ENTER();

    assert((SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS == (int64_t)arg) ||
           (SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION == (int64_t)arg));

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &router_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}
/*
 * Routine Description:
 *    Create virtual router
 *
 * Arguments:
 *    [out] vr_id - virtual router id
 *    [in] attr_count - number of attributes
 *    [in] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_create_virtual_router(_Out_ sai_object_id_t      *vr_id,
                                        _In_ uint32_t               attr_count,
                                        _In_ const sai_attribute_t *attr_list)
{
    sai_status_t    status;
    char            list_str[MAX_LIST_VALUE_STR_LEN];
    char            key_str[MAX_KEY_STR_LEN];
    static uint32_t next_id = 0;

    STUB_LOG_ENTER();

    if (NULL == vr_id) {
        STUB_LOG_ERR("NULL vr_id param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (SAI_STATUS_SUCCESS !=
        (status =
             check_attribs_metadata(attr_count, attr_list, router_attribs, router_vendor_attribs,
                                    SAI_OPERATION_CREATE))) {
        STUB_LOG_ERR("Failed attribs check\n");
        return status;
    }

    sai_attr_list_to_str(attr_count, attr_list, router_attribs, MAX_LIST_VALUE_STR_LEN, list_str);
    STUB_LOG_NTC("Create router, %s\n", list_str);

    if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_VIRTUAL_ROUTER, next_id++, vr_id))) {
        return status;
    }
    router_key_to_str(*vr_id, key_str);
    STUB_LOG_NTC("Created router %s\n", key_str);

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Remove virtual router
 *
 * Arguments:
 *    [in] vr_id - virtual router id
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_remove_virtual_router(_In_ sai_object_id_t vr_id)
{
    sai_status_t status;
    uint32_t     data;
    char         key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    router_key_to_str(vr_id, key_str);
    STUB_LOG_NTC("Remove router %s\n", key_str);

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(vr_id, SAI_OBJECT_TYPE_VIRTUAL_ROUTER, &data))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

const sai_virtual_router_api_t router_api = {
    stub_create_virtual_router,
    stub_remove_virtual_router,
    stub_set_virtual_router_attribute,
    stub_get_virtual_router_attribute
};
