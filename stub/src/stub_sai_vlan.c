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
#define __MODULE__ SAI_VLAN

#define vlan_id_range_ok(vlan_id) ((vlan_id)>=1 && (vlan_id)<=4095)


/* Storage layer data structures / variables to store the states */
static int number_of_vlans;
struct __vlan* vlans = NULL;

struct __vlan {
    sai_vlan_id_t id;
    int number_of_ports;
    sai_vlan_port_t* port_list;
};


static const sai_attribute_entry_t vlan_attribs[] = {
    {   SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES, false, false, true, true,
        "Vlan Maximum number of learned MAC addresses", SAI_ATTR_VAL_TYPE_U32
    },
    {   SAI_VLAN_ATTR_STP_INSTANCE, false, false, true, true,
        "Vlan associated STP instance", SAI_ATTR_VAL_TYPE_U64
    },
    {   END_FUNCTIONALITY_ATTRIBS_ID, false, false, false, false,
        "", SAI_ATTR_VAL_TYPE_UNDETERMINED
    }
};

sai_status_t stub_vlan_max_learned_addr_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg);
sai_status_t stub_vlan_max_learned_addr_set(_In_ const sai_object_key_t      *key,
                                            _In_ const sai_attribute_value_t *value,
                                            void                             *arg);
sai_status_t stub_vlan_stp_get(_In_ const sai_object_key_t   *key,
                               _Inout_ sai_attribute_value_t *value,
                               _In_ uint32_t                  attr_index,
                               _Inout_ vendor_cache_t        *cache,
                               void                          *arg);
sai_status_t stub_vlan_stp_set(_In_ const sai_object_key_t      *key,
                               _In_ const sai_attribute_value_t *value,
                               void                             *arg);

static const sai_vendor_attribute_entry_t vlan_vendor_attribs[] = {
    {   SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES,
        { false, false, true, true },
        { false, false, true, true },
        stub_vlan_max_learned_addr_get, NULL,
        stub_vlan_max_learned_addr_set, NULL
    },
    {   SAI_VLAN_ATTR_STP_INSTANCE,
        { false, false, true, true },
        { false, false, true, true },
        stub_vlan_stp_get, NULL,
        stub_vlan_stp_set, NULL
    },
};

/*
 * Routine Description:
 *    Create default VLAN and add all port into it.
 */
void db_init_vlan()
{
    int ii;
    sai_object_id_t port;

    if (NULL != vlans) {
        free(vlans);
    }

    vlans = malloc(sizeof(struct __vlan));
    STUB_ASSERT(NULL != vlans);

    vlans[0].id = 1;
    vlans[0].number_of_ports = PORT_NUMBER;
    vlans[0].port_list = malloc(sizeof(sai_vlan_port_t) * PORT_NUMBER);

    for (ii = 0; ii < PORT_NUMBER; ++ii) {
        stub_create_object(SAI_OBJECT_TYPE_PORT, ii, &port);
        vlans[0].port_list[ii].port_id = port;
        vlans[0].port_list[ii].tagging_mode = SAI_VLAN_PORT_UNTAGGED;
    }

    number_of_vlans = 1;
}

static void vlan_key_to_str(_In_ sai_vlan_id_t vlan_id, _Out_ char *key_str)
{
    snprintf(key_str, MAX_KEY_STR_LEN, "vlan %u", vlan_id);
}

/*
 * Routine Description:
 *    Set VLAN attribute Value
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *    [in] attr - attribute
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_set_vlan_attribute(_In_ sai_vlan_id_t vlan_id, _In_ const sai_attribute_t *attr)
{
    const sai_object_key_t key = { .vlan_id = vlan_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    vlan_key_to_str(vlan_id, key_str);
    return sai_set_attribute(&key, key_str, vlan_attribs, vlan_vendor_attribs, attr);
}


/*
 * Routine Description:
 *    Get VLAN attribute Value
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *    [in] attr_count - number of attributes
 *    [inout] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_get_vlan_attribute(_In_ sai_vlan_id_t       vlan_id,
                                     _In_ uint32_t            attr_count,
                                     _Inout_ sai_attribute_t *attr_list)
{
    const sai_object_key_t key = { .vlan_id = vlan_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    vlan_key_to_str(vlan_id, key_str);
    return sai_get_attributes(&key, key_str, vlan_attribs, vlan_vendor_attribs, attr_count, attr_list);
}


/*
 * Routine Description:
 *    Remove VLAN configuration (remove all VLANs).
 *
 * Arguments:
 *    None
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_remove_all_vlans(void)
{
    STUB_LOG_NTC("Remove all vlan\n");

    return SAI_STATUS_SUCCESS;
}


/*
 * Routine Description:
 *    Create a VLAN
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_create_vlan(_In_ sai_vlan_id_t vlan_id)
{
    char key_str[MAX_KEY_STR_LEN];
    int i;

    vlan_key_to_str(vlan_id, key_str);
    STUB_LOG_NTC("Create vlan %s\n", key_str);

    // make sure the given vlan_id satisfies the spec
    if (!vlan_id_range_ok(vlan_id)) {
        STUB_LOG_ERR("vlan_id (%d) must satisfy 1 <= vlan_id <= 4094\n", vlan_id);
        return SAI_STATUS_INVALID_VLAN_ID;
    }

    // make sure the given vlan_id is available
    for (i = 0; i < number_of_vlans; i++) {
        if (vlans[i].id == vlan_id) {
            STUB_LOG_WRN("Warning: given vlan_id (%d) already exsits.\n", vlan_id);
            return SAI_STATUS_INVALID_VLAN_ID;
        }
    }

    // add new vlan
    number_of_vlans++;
    vlans = realloc(vlans, number_of_vlans * sizeof(struct __vlan));

    if (vlans == NULL) {
        STUB_LOG_ERR("Error: memory allocation for creating a new vlan failed.\n");
        return SAI_STATUS_NO_MEMORY;
    }

    struct __vlan* v = &(vlans[number_of_vlans - 1]);
    v->id = vlan_id;
    v->number_of_ports = 0;
    v->port_list = NULL;

    return SAI_STATUS_SUCCESS;
}


/*
 * Routine Description:
 *    Remove a VLAN
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_remove_vlan(_In_ sai_vlan_id_t vlan_id)
{
    char key_str[MAX_KEY_STR_LEN];
    int i, index_removed_vlan = -1;

    vlan_key_to_str(vlan_id, key_str);

    // make sure the given vlan_id exists
    for (i = 0; i < number_of_vlans; i++) {
        if (vlans[i].id == vlan_id) {
            index_removed_vlan = i;
            break;
        }
    }
    if (index_removed_vlan == -1) {
        STUB_LOG_NTC("the given vlan id (%d) does not exist.\n", vlan_id);
        return SAI_STATUS_INVALID_VLAN_ID;
    }

    // delete the vlans[index_removed_vlan]
    for (i = 0; i < number_of_vlans; i++) {
        if (i > index_removed_vlan) {
            vlans[i - 1] = vlans[i];
        }
    }
    number_of_vlans--;
    vlans = realloc(vlans, sizeof(struct __vlan) * number_of_vlans);
    // Note: realloc(ptr, 0) returns NULL, which is not an error
    if (vlans == NULL && number_of_vlans > 0) {
        STUB_LOG_ERR("Error: memory allocation for removing a vlan failed.\n");
        return SAI_STATUS_NO_MEMORY;
    }

    STUB_LOG_NTC("Remove vlan %s\n", key_str);

    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Add Port to VLAN
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *    [in] port_count - number of ports
 *    [in] port_list - pointer to membership structures
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_add_ports_to_vlan(_In_ sai_vlan_id_t          vlan_id,
                                    _In_ uint32_t               port_count,
                                    _In_ const sai_vlan_port_t* port_list)
{
    STUB_LOG_ENTER();

    char key_str[MAX_KEY_STR_LEN];
    int i, index_target_vlan = -1;

    vlan_key_to_str(vlan_id, key_str);

    for (i = 0; i < number_of_vlans; i++) {
        if (vlans[i].id == vlan_id) {
            index_target_vlan = i;
            break;
        }
    }

    if (index_target_vlan == -1) {
        STUB_LOG_WRN("the given vlan id (%d) does not exist.\n", vlan_id);
        return SAI_STATUS_INVALID_VLAN_ID;
    }

    struct __vlan* v = &vlans[index_target_vlan];
    int old_size = v->number_of_ports, new_size = old_size + port_count;
    v->port_list = realloc(v->port_list, new_size * sizeof(sai_vlan_port_t));

    if (v->port_list == NULL) {
        STUB_LOG_ERR("Error: memory allocation for removing a vlan failed.\n");
        return SAI_STATUS_NO_MEMORY;
    }

    v->number_of_ports = new_size;
    memcpy(v->port_list + old_size, port_list, port_count * sizeof(sai_vlan_port_t));

    return SAI_STATUS_SUCCESS;
}

static int remove_a_port_from_vlan(struct __vlan* v, sai_vlan_port_t port)
{
    int i, j;
    int deleted = 0;

    for (i = 0; i < v->number_of_ports; i++) {
        if (port.port_id == v->port_list[i].port_id) {
            // delete this port
            deleted = 1;

            for (j = i + 1; j < v->number_of_ports; j++) {
                v->port_list[j - 1] = v->port_list[j];
            }

            (v->number_of_ports)--;
            v->port_list = realloc(v->port_list, sizeof(sai_vlan_port_t) * v->number_of_ports);

            break;
        }
    }

    return deleted;
}

/*
 * Routine Description:
 *    Remove Port from VLAN
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *    [in] port_count - number of ports
 *    [in] port_list - pointer to membership structures
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_remove_ports_from_vlan(_In_ sai_vlan_id_t          vlan_id,
                                         _In_ uint32_t               port_count,
                                         _In_ const sai_vlan_port_t* port_list)
{
    STUB_LOG_ENTER();

    char key_str[MAX_KEY_STR_LEN];
    struct __vlan* v = NULL;
    int i;

    vlan_key_to_str(vlan_id, key_str);

    for (i = 0; i < number_of_vlans; i++) {
        if (vlans[i].id == vlan_id) {
            v = &vlans[i];
            break;
        }
    }

    if (v == NULL) {
        STUB_LOG_WRN("the given vlan id (%d) does not exist.\n", vlan_id);
        return SAI_STATUS_INVALID_VLAN_ID;
    }

    for (i = 0; i < (int)port_count; i++) {
        int ret = remove_a_port_from_vlan(v, port_list[i]);
        if (ret == 0) {
            STUB_LOG_NTC("the given port (%d) does not belong to the given vlan (%d)\n", port_list[i].port_id, vlan_id);
        }
    }

    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *   Get vlan statistics counters.
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *    [in] counter_ids - specifies the array of counter ids
 *    [in] number_of_counters - number of counters in the array
 *    [out] counters - array of resulting counter values.
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_get_vlan_stats(_In_ sai_vlan_id_t                  vlan_id,
                                 _In_ const sai_vlan_stat_counter_t *counter_ids,
                                 _In_ uint32_t                       number_of_counters,
                                 _Out_ uint64_t                    * counters)
{
    uint32_t ii;

    UNREFERENCED_PARAMETER(vlan_id);
    UNREFERENCED_PARAMETER(number_of_counters);

    STUB_LOG_ENTER();

    if (NULL == counter_ids) {
        STUB_LOG_ERR("NULL counter ids array param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == counters) {
        STUB_LOG_ERR("NULL counters array param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    for (ii = 0; ii < number_of_counters; ii++) {
        counters[ii] = 0;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Maximum number of learned MAC addresses [uint32_t]
 * zero means learning limit disable. (default to zero). */
sai_status_t stub_vlan_max_learned_addr_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg)
{
    STUB_LOG_ENTER();

    value->u32 = 0;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Maximum number of learned MAC addresses [uint32_t]
 * zero means learning limit disable. (default to zero). */
sai_status_t stub_vlan_max_learned_addr_set(_In_ const sai_object_key_t      *key,
                                            _In_ const sai_attribute_value_t *value,
                                            void                             *arg)
{
    STUB_LOG_ENTER();

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* STP Instance that the VLAN is associated to [sai_object_id_t]
 * (default to default stp instance id)*/
sai_status_t stub_vlan_stp_get(_In_ const sai_object_key_t   *key,
                               _Inout_ sai_attribute_value_t *value,
                               _In_ uint32_t                  attr_index,
                               _Inout_ vendor_cache_t        *cache,
                               void                          *arg)
{
    sai_status_t status;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_create_object(SAI_OBJECT_TYPE_STP_INSTANCE, 1, &value->oid))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* STP Instance that the VLAN is associated to [sai_object_id_t]
 * (default to default stp instance id)*/
sai_status_t stub_vlan_stp_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value, void *arg)
{
    sai_status_t status;
    uint32_t     stp_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(value->oid, SAI_OBJECT_TYPE_STP_INSTANCE, &stp_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

const sai_vlan_api_t vlan_api = {
    stub_create_vlan,
    stub_remove_vlan,
    stub_set_vlan_attribute,
    stub_get_vlan_attribute,
    stub_add_ports_to_vlan,
    stub_remove_ports_from_vlan,
    stub_remove_all_vlans,
    stub_get_vlan_stats,
    NULL
};
