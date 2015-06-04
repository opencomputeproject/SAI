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
#define __MODULE__ SAI_NEXT_HOP

static const sai_attribute_entry_t next_hop_attribs[] = {
    { SAI_NEXT_HOP_ATTR_TYPE, true, true, false, true,
      "Next hop entry type", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_NEXT_HOP_ATTR_IP, true, true, false, true,
      "Next hop entry IP address", SAI_ATTR_VAL_TYPE_IPADDR },
    { SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID, true, true, false, true,
      "Next hop entry router interface ID", SAI_ATTR_VAL_TYPE_OID },
    { END_FUNCTIONALITY_ATTRIBS_ID, false, false, false, false,
      "", SAI_ATTR_VAL_TYPE_UNDETERMINED }
};

sai_status_t stub_next_hop_type_get(_In_ const sai_object_key_t   *key,
                                    _Inout_ sai_attribute_value_t *value,
                                    _In_ uint32_t                  attr_index,
                                    _Inout_ vendor_cache_t        *cache,
                                    void                          *arg);
sai_status_t stub_next_hop_ip_get(_In_ const sai_object_key_t   *key,
                                  _Inout_ sai_attribute_value_t *value,
                                  _In_ uint32_t                  attr_index,
                                  _Inout_ vendor_cache_t        *cache,
                                  void                          *arg);
sai_status_t stub_next_hop_rif_get(_In_ const sai_object_key_t   *key,
                                   _Inout_ sai_attribute_value_t *value,
                                   _In_ uint32_t                  attr_index,
                                   _Inout_ vendor_cache_t        *cache,
                                   void                          *arg);

static const sai_vendor_attribute_entry_t next_hop_vendor_attribs[] = {
    { SAI_NEXT_HOP_ATTR_TYPE,
      { true, false, false, true },
      { true, false, false, true },
      stub_next_hop_type_get, NULL,
      NULL, NULL },
    { SAI_NEXT_HOP_ATTR_IP,
      { true, false, false, true },
      { true, false, false, true },
      stub_next_hop_ip_get, NULL,
      NULL, NULL },
    { SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
      { true, false, false, true },
      { true, false, false, true },
      stub_next_hop_rif_get, NULL,
      NULL, NULL },
};
static void next_hop_key_to_str(_In_ sai_object_id_t next_hop_id, _Out_ char *key_str)
{
    uint32_t nexthop_data;

    if (SAI_STATUS_SUCCESS != stub_object_to_type(next_hop_id, SAI_OBJECT_TYPE_NEXT_HOP, &nexthop_data)) {
        snprintf(key_str, MAX_KEY_STR_LEN, "invalid next hop id");
    } else {
        snprintf(key_str, MAX_KEY_STR_LEN, "next hop id %u", nexthop_data);
    }
}

/*
 * Routine Description:
 *    Create next hop
 *
 * Arguments:
 *    [out] next_hop_id - next hop id
 *    [in] attr_count - number of attributes
 *    [in] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 *
 * Note: IP address expected in Network Byte Order.
 */
sai_status_t stub_create_next_hop(_Out_ sai_object_id_t      *next_hop_id,
                                  _In_ uint32_t               attr_count,
                                  _In_ const sai_attribute_t *attr_list)
{
    sai_status_t                 status;
    const sai_attribute_value_t *type, *ip, *rif;
    uint32_t                     type_index, ip_index, rif_index;
    char                         list_str[MAX_LIST_VALUE_STR_LEN];
    char                         key_str[MAX_KEY_STR_LEN];
    static uint32_t              next_id = 0;

    STUB_LOG_ENTER();

    if (NULL == next_hop_id) {
        STUB_LOG_ERR("NULL next hop id param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (SAI_STATUS_SUCCESS !=
        (status =
             check_attribs_metadata(attr_count, attr_list, next_hop_attribs, next_hop_vendor_attribs,
                                    SAI_OPERATION_CREATE))) {
        STUB_LOG_ERR("Failed attribs check\n");
        return status;
    }

    sai_attr_list_to_str(attr_count, attr_list, next_hop_attribs, MAX_LIST_VALUE_STR_LEN, list_str);
    STUB_LOG_NTC("Create next hop, %s\n", list_str);

    assert(SAI_STATUS_SUCCESS ==
           find_attrib_in_list(attr_count, attr_list, SAI_NEXT_HOP_ATTR_TYPE, &type, &type_index));
    assert(SAI_STATUS_SUCCESS == find_attrib_in_list(attr_count, attr_list, SAI_NEXT_HOP_ATTR_IP, &ip, &ip_index));
    assert(SAI_STATUS_SUCCESS ==
           find_attrib_in_list(attr_count, attr_list, SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID, &rif, &rif_index));

    if (SAI_NEXT_HOP_IP != type->s32) {
        STUB_LOG_ERR("Invalid next hop type %d on create\n", type->s32);
        return SAI_STATUS_INVALID_ATTR_VALUE_0 + type_index;
    }

    if (SAI_IP_ADDR_FAMILY_IPV4 == ip->ipaddr.addr_family) {
    } else if (SAI_IP_ADDR_FAMILY_IPV6 == ip->ipaddr.addr_family) {
    } else {
        STUB_LOG_ERR("Invalid ip addr family %d on create\n", ip->ipaddr.addr_family);
        return SAI_STATUS_INVALID_ATTR_VALUE_0 + ip_index;
    }

    if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_NEXT_HOP, next_id++, next_hop_id))) {
        return status;
    }
    next_hop_key_to_str(*next_hop_id, key_str);
    STUB_LOG_NTC("Created next hop %s\n", key_str);

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Remove next hop
 *
 * Arguments:
 *    [in] next_hop_id - next hop id
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_remove_next_hop(_In_ sai_object_id_t next_hop_id)
{
    char key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    next_hop_key_to_str(next_hop_id, key_str);
    STUB_LOG_NTC("Remove next hop %s\n", key_str);

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Set Next Hop attribute
 *
 * Arguments:
 *    [in] next_hop_id - next hop id
 *    [in] attr - attribute
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_set_next_hop_attribute(_In_ sai_object_id_t next_hop_id, _In_ const sai_attribute_t *attr)
{
    const sai_object_key_t key = { .object_id = next_hop_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    next_hop_key_to_str(next_hop_id, key_str);
    return sai_set_attribute(&key, key_str, next_hop_attribs, next_hop_vendor_attribs, attr);
}


/*
 * Routine Description:
 *    Get Next Hop attribute
 *
 * Arguments:
 *    [in] next_hop_id - next hop id
 *    [in] attr_count - number of attributes
 *    [inout] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_get_next_hop_attribute(_In_ sai_object_id_t     next_hop_id,
                                         _In_ uint32_t            attr_count,
                                         _Inout_ sai_attribute_t *attr_list)
{
    const sai_object_key_t key = { .object_id = next_hop_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    next_hop_key_to_str(next_hop_id, key_str);
    return sai_get_attributes(&key, key_str, next_hop_attribs, next_hop_vendor_attribs, attr_count, attr_list);
}

/* Next hop entry type [sai_next_hop_type_t] */
sai_status_t stub_next_hop_type_get(_In_ const sai_object_key_t   *key,
                                    _Inout_ sai_attribute_value_t *value,
                                    _In_ uint32_t                  attr_index,
                                    _Inout_ vendor_cache_t        *cache,
                                    void                          *arg)
{
    STUB_LOG_ENTER();

    value->s32 = SAI_NEXT_HOP_IP;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Next hop entry ipv4 address [sai_ip_address_t] */
sai_status_t stub_next_hop_ip_get(_In_ const sai_object_key_t   *key,
                                  _Inout_ sai_attribute_value_t *value,
                                  _In_ uint32_t                  attr_index,
                                  _Inout_ vendor_cache_t        *cache,
                                  void                          *arg)
{
    uint32_t     nexthop_data;
    sai_status_t status;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_NEXT_HOP, &nexthop_data))) {
        return status;
    }

    value->ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    value->ipaddr.addr.ip4    = 0;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Next hop entry router interface id [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
sai_status_t stub_next_hop_rif_get(_In_ const sai_object_key_t   *key,
                                   _Inout_ sai_attribute_value_t *value,
                                   _In_ uint32_t                  attr_index,
                                   _Inout_ vendor_cache_t        *cache,
                                   void                          *arg)
{
    uint32_t     nexthop_data;
    sai_status_t status;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_NEXT_HOP, &nexthop_data))) {
        return status;
    }

    if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_ROUTER_INTERFACE, 0, &value->oid))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

const sai_next_hop_api_t next_hop_api = {
    stub_create_next_hop,
    stub_remove_next_hop,
    stub_set_next_hop_attribute,
    stub_get_next_hop_attribute
};
