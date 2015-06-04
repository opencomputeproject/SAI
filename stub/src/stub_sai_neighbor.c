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
#define __MODULE__ SAI_NEIGHBOR

static const sai_attribute_entry_t neighbor_attribs[] = {
    { SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS, true, true, true, true,
      "Neighbor destination MAC", SAI_ATTR_VAL_TYPE_MAC },
    { SAI_NEIGHBOR_ATTR_PACKET_ACTION, false, true, true, true,
      "Neighbor L3 forwarding action", SAI_ATTR_VAL_TYPE_S32 },
    { END_FUNCTIONALITY_ATTRIBS_ID, false, false, false, false,
      "", SAI_ATTR_VAL_TYPE_UNDETERMINED }
};

sai_status_t stub_neighbor_mac_get(_In_ const sai_object_key_t   *key,
                                   _Inout_ sai_attribute_value_t *value,
                                   _In_ uint32_t                  attr_index,
                                   _Inout_ vendor_cache_t        *cache,
                                   void                          *arg);
sai_status_t stub_neighbor_action_get(_In_ const sai_object_key_t   *key,
                                      _Inout_ sai_attribute_value_t *value,
                                      _In_ uint32_t                  attr_index,
                                      _Inout_ vendor_cache_t        *cache,
                                      void                          *arg);
sai_status_t stub_neighbor_mac_set(_In_ const sai_object_key_t      *key,
                                   _In_ const sai_attribute_value_t *value,
                                   void                             *arg);
sai_status_t stub_neighbor_action_set(_In_ const sai_object_key_t      *key,
                                      _In_ const sai_attribute_value_t *value,
                                      void                             *arg);

static const sai_vendor_attribute_entry_t neighbor_vendor_attribs[] = {
    { SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
      { true, false, true, true },
      { true, false, true, true },
      stub_neighbor_mac_get, NULL,
      stub_neighbor_mac_set, NULL },
    { SAI_NEIGHBOR_ATTR_PACKET_ACTION,
      { true, false, true, true },
      { true, false, true, true },
      stub_neighbor_action_get, NULL,
      stub_neighbor_action_set, NULL },
};
static void neighbor_key_to_str(_In_ const sai_neighbor_entry_t* neighbor_entry, _Out_ char *key_str)
{
    int      res1, res2;
    uint32_t rifid;

    res1 = snprintf(key_str, MAX_KEY_STR_LEN, "neighbor ip ");
    sai_ipaddr_to_str(neighbor_entry->ip_address, MAX_KEY_STR_LEN - res1, key_str + res1, &res2);
    if (SAI_STATUS_SUCCESS != stub_object_to_type(neighbor_entry->rif_id, SAI_OBJECT_TYPE_ROUTER_INTERFACE, &rifid)) {
        snprintf(key_str + res1 + res2, MAX_KEY_STR_LEN - res1 - res2, " invalid rif");
    } else {
        snprintf(key_str + res1 + res2, MAX_KEY_STR_LEN - res1 - res2, " rif %u", rifid);
    }
}

/*
 * Routine Description:
 *    Create neighbor entry
 *
 * Arguments:
 *    [in] neighbor_entry - neighbor entry
 *    [in] attr_count - number of attributes
 *    [in] attrs - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 *
 * Note: IP address expected in Network Byte Order.
 */
sai_status_t stub_create_neighbor_entry(_In_ const sai_neighbor_entry_t* neighbor_entry,
                                        _In_ uint32_t                    attr_count,
                                        _In_ const sai_attribute_t      *attr_list)
{
    sai_status_t status;
    uint32_t     rif_data;
    char         key_str[MAX_KEY_STR_LEN];
    char         list_str[MAX_LIST_VALUE_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == neighbor_entry) {
        STUB_LOG_ERR("NULL neighbor entry param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (SAI_STATUS_SUCCESS !=
        (status =
             check_attribs_metadata(attr_count, attr_list, neighbor_attribs, neighbor_vendor_attribs,
                                    SAI_OPERATION_CREATE))) {
        STUB_LOG_ERR("Failed attribs check\n");
        return status;
    }

    neighbor_key_to_str(neighbor_entry, key_str);
    sai_attr_list_to_str(attr_count, attr_list, neighbor_attribs, MAX_LIST_VALUE_STR_LEN, list_str);
    STUB_LOG_NTC("Create neighbor entry %s\n", key_str);
    STUB_LOG_NTC("Attribs %s\n", list_str);

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(neighbor_entry->rif_id, SAI_OBJECT_TYPE_ROUTER_INTERFACE, &rif_data))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Remove neighbor entry
 *
 * Arguments:
 *    [in] neighbor_entry - neighbor entry
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 *
 * Note: IP address expected in Network Byte Order.
 */
sai_status_t stub_remove_neighbor_entry(_In_ const sai_neighbor_entry_t* neighbor_entry)
{
    char key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == neighbor_entry) {
        STUB_LOG_ERR("NULL neighbor entry param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    neighbor_key_to_str(neighbor_entry, key_str);
    STUB_LOG_NTC("Remove neighbor entry %s\n", key_str);

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Set neighbor attribute value
 *
 * Arguments:
 *    [in] neighbor_entry - neighbor entry
 *    [in] attr - attribute
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_set_neighbor_attribute(_In_ const sai_neighbor_entry_t* neighbor_entry,
                                         _In_ const sai_attribute_t      *attr)
{
    const sai_object_key_t key = { .neighbor_entry = neighbor_entry };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == neighbor_entry) {
        STUB_LOG_ERR("NULL neighbor entry param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    neighbor_key_to_str(neighbor_entry, key_str);
    return sai_set_attribute(&key, key_str, neighbor_attribs, neighbor_vendor_attribs, attr);
}

/*
 * Routine Description:
 *    Get neighbor attribute value
 *
 * Arguments:
 *    [in] neighbor_entry - neighbor entry
 *    [in] attr_count - number of attributes
 *    [inout] attrs - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_get_neighbor_attribute(_In_ const sai_neighbor_entry_t* neighbor_entry,
                                         _In_ uint32_t                    attr_count,
                                         _Inout_ sai_attribute_t         *attr_list)
{
    const sai_object_key_t key = { .neighbor_entry = neighbor_entry };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == neighbor_entry) {
        STUB_LOG_ERR("NULL neighbor entry param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    neighbor_key_to_str(neighbor_entry, key_str);
    return sai_get_attributes(&key, key_str, neighbor_attribs, neighbor_vendor_attribs, attr_count, attr_list);
}

/* Destination mac address for the neighbor [sai_mac_t] */
sai_status_t stub_neighbor_mac_get(_In_ const sai_object_key_t   *key,
                                   _Inout_ sai_attribute_value_t *value,
                                   _In_ uint32_t                  attr_index,
                                   _Inout_ vendor_cache_t        *cache,
                                   void                          *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* L3 forwarding action for this neighbor [sai_packet_action_t] */
sai_status_t stub_neighbor_action_get(_In_ const sai_object_key_t   *key,
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

/* Destination mac address for the neighbor [sai_mac_t] */
sai_status_t stub_neighbor_mac_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value,
                                   void *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* L3 forwarding action for this neighbor [sai_packet_action_t] */
sai_status_t stub_neighbor_action_set(_In_ const sai_object_key_t      *key,
                                      _In_ const sai_attribute_value_t *value,
                                      void                             *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}


/*
 * Routine Description:
 *    Remove all neighbor entries
 *
 * Arguments:
 *    None
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_remove_all_neighbor_entries(void)
{
    STUB_LOG_ENTER();

    STUB_LOG_NTC("Remove all neighbor entries\n");

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

const sai_neighbor_api_t neighbor_api = {
    stub_create_neighbor_entry,
    stub_remove_neighbor_entry,
    stub_set_neighbor_attribute,
    stub_get_neighbor_attribute,
    stub_remove_all_neighbor_entries
};
