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

#undef  __MODULE__
#define __MODULE__ SAI_ROUTE

static const sai_attribute_entry_t route_attribs[] = {
    { SAI_ROUTE_ATTR_PACKET_ACTION, false, true, true, true,
      "Route packet action", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_ROUTE_ATTR_TRAP_PRIORITY, false, true, true, true,
      "Route trap priority", SAI_ATTR_VAL_TYPE_U8 },
    { SAI_ROUTE_ATTR_NEXT_HOP_ID, false, true, true, true,
      "Route next hop ID", SAI_ATTR_VAL_TYPE_OID },
    { END_FUNCTIONALITY_ATTRIBS_ID, false, false, false, false,
      "", SAI_ATTR_VAL_TYPE_UNDETERMINED }
};

sai_status_t stub_route_packet_action_get(_In_ const sai_object_key_t   *key,
                                          _Inout_ sai_attribute_value_t *value,
                                          _In_ uint32_t                  attr_index,
                                          _Inout_ vendor_cache_t        *cache,
                                          void                          *arg);
sai_status_t stub_route_trap_priority_get(_In_ const sai_object_key_t   *key,
                                          _Inout_ sai_attribute_value_t *value,
                                          _In_ uint32_t                  attr_index,
                                          _Inout_ vendor_cache_t        *cache,
                                          void                          *arg);
sai_status_t stub_route_next_hop_id_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg);
sai_status_t stub_route_packet_action_set(_In_ const sai_object_key_t      *key,
                                          _In_ const sai_attribute_value_t *value,
                                          void                             *arg);
sai_status_t stub_route_trap_priority_set(_In_ const sai_object_key_t      *key,
                                          _In_ const sai_attribute_value_t *value,
                                          void                             *arg);
sai_status_t stub_route_next_hop_id_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg);

static const sai_vendor_attribute_entry_t route_vendor_attribs[] = {
    { SAI_ROUTE_ATTR_PACKET_ACTION,
      { true, false, true, true },
      { true, false, true, true },
      stub_route_packet_action_get, NULL,
      stub_route_packet_action_set, NULL },
    { SAI_ROUTE_ATTR_TRAP_PRIORITY,
      { true, false, true, true },
      { true, false, true, true },
      stub_route_trap_priority_get, NULL,
      stub_route_trap_priority_set, NULL },
    { SAI_ROUTE_ATTR_NEXT_HOP_ID,
      { true, false, true, true },
      { true, false, true, true },
      stub_route_next_hop_id_get, NULL,
      stub_route_next_hop_id_set, NULL },
};
static void route_key_to_str(_In_ const sai_unicast_route_entry_t* unicast_route_entry, _Out_ char *key_str)
{
    int res;

    res = snprintf(key_str, MAX_KEY_STR_LEN, "route ");
    sai_ipprefix_to_str(unicast_route_entry->destination, MAX_KEY_STR_LEN - res, key_str + res);
}

/*
 * Routine Description:
 *    Create Route
 *
 * Arguments:
 *    [in] unicast_route_entry - route entry
 *    [in] attr_count - number of attributes
 *    [in] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 *
 * Note: IP prefix/mask expected in Network Byte Order.
 *
 */
sai_status_t stub_create_route(_In_ const sai_unicast_route_entry_t* unicast_route_entry,
                               _In_ uint32_t                         attr_count,
                               _In_ const sai_attribute_t           *attr_list)
{
    sai_status_t status;
    char         list_str[MAX_LIST_VALUE_STR_LEN];
    char         key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == unicast_route_entry) {
        STUB_LOG_ERR("NULL unicast_route_entry param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (SAI_STATUS_SUCCESS !=
        (status =
             check_attribs_metadata(attr_count, attr_list, route_attribs, route_vendor_attribs,
                                    SAI_OPERATION_CREATE))) {
        STUB_LOG_ERR("Failed attribs check\n");
        return status;
    }

    route_key_to_str(unicast_route_entry, key_str);
    sai_attr_list_to_str(attr_count, attr_list, route_attribs, MAX_LIST_VALUE_STR_LEN, list_str);
    STUB_LOG_NTC("Create route %s\n", key_str);
    STUB_LOG_NTC("Attribs %s\n", list_str);

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Remove Route
 *
 * Arguments:
 *    [in] unicast_route_entry - route entry
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 *
 * Note: IP prefix/mask expected in Network Byte Order.
 */
sai_status_t stub_remove_route(_In_ const sai_unicast_route_entry_t* unicast_route_entry)
{
    char key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == unicast_route_entry) {
        STUB_LOG_ERR("NULL unicast_route_entry param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    route_key_to_str(unicast_route_entry, key_str);
    STUB_LOG_NTC("Remove route %s\n", key_str);

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Set route attribute value
 *
 * Arguments:
 *    [in] unicast_route_entry - route entry
 *    [in] attr - attribute
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_set_route_attribute(_In_ const sai_unicast_route_entry_t* unicast_route_entry,
                                      _In_ const sai_attribute_t           *attr)
{
    const sai_object_key_t key = { .unicast_route_entry = unicast_route_entry };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == unicast_route_entry) {
        STUB_LOG_ERR("NULL unicast_route_entry param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    route_key_to_str(unicast_route_entry, key_str);
    return sai_set_attribute(&key, key_str, route_attribs, route_vendor_attribs, attr);
}

/*
 * Routine Description:
 *    Get route attribute value
 *
 * Arguments:
 *    [in] unicast_route_entry - route entry
 *    [in] attr_count - number of attributes
 *    [inout] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_get_route_attribute(_In_ const sai_unicast_route_entry_t* unicast_route_entry,
                                      _In_ uint32_t                         attr_count,
                                      _Inout_ sai_attribute_t              *attr_list)
{
    const sai_object_key_t key = { .unicast_route_entry = unicast_route_entry };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == unicast_route_entry) {
        STUB_LOG_ERR("NULL unicast_route_entry param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    route_key_to_str(unicast_route_entry, key_str);
    return sai_get_attributes(&key, key_str, route_attribs, route_vendor_attribs, attr_count, attr_list);
}

/* Packet action [sai_packet_action_t] */
sai_status_t stub_route_packet_action_get(_In_ const sai_object_key_t   *key,
                                          _Inout_ sai_attribute_value_t *value,
                                          _In_ uint32_t                  attr_index,
                                          _Inout_ vendor_cache_t        *cache,
                                          void                          *arg)
{
    STUB_LOG_ENTER();

    value->s32 = SAI_PACKET_ACTION_DROP;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Packet priority for trap/log actions [uint8_t] */
sai_status_t stub_route_trap_priority_get(_In_ const sai_object_key_t   *key,
                                          _Inout_ sai_attribute_value_t *value,
                                          _In_ uint32_t                  attr_index,
                                          _Inout_ vendor_cache_t        *cache,
                                          void                          *arg)
{
    STUB_LOG_ENTER();

    value->u8 = 0;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Next hop or next hop group id for the packet [sai_object_id_t]
 * The next hop id can be a generic next hop object, such as next hop,
 * next hop group. */
sai_status_t stub_route_next_hop_id_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg)
{
    sai_status_t status;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_NEXT_HOP, 0, &value->oid))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Packet action [sai_packet_action_t] */
sai_status_t stub_route_packet_action_set(_In_ const sai_object_key_t      *key,
                                          _In_ const sai_attribute_value_t *value,
                                          void                             *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Packet priority for trap/log actions [uint8_t] */
sai_status_t stub_route_trap_priority_set(_In_ const sai_object_key_t      *key,
                                          _In_ const sai_attribute_value_t *value,
                                          void                             *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Next hop or next hop group id for the packet [sai_object_id_t]
 * The next hop id can be a generic next hop object, such as next hop,
 * next hop group. */
sai_status_t stub_route_next_hop_id_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

const sai_route_api_t route_api = {
    stub_create_route,
    stub_remove_route,
    stub_set_route_attribute,
    stub_get_route_attribute,
};
