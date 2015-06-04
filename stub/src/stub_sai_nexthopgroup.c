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
#define __MODULE__ SAI_NEXT_HOP_GROUP

static const sai_attribute_entry_t next_hop_group_attribs[] = {
    { SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_COUNT, false, false, false, true,
      "Next hop group entries count", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_NEXT_HOP_GROUP_ATTR_TYPE, true, true, false, true,
      "Next hop group type", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST, true, true, true, true,
      "Next hop group hop list", SAI_ATTR_VAL_TYPE_OBJLIST },
    { END_FUNCTIONALITY_ATTRIBS_ID, false, false, false, false,
      "", SAI_ATTR_VAL_TYPE_UNDETERMINED }
};

sai_status_t stub_next_hop_group_count_get(_In_ const sai_object_key_t   *key,
                                           _Inout_ sai_attribute_value_t *value,
                                           _In_ uint32_t                  attr_index,
                                           _Inout_ vendor_cache_t        *cache,
                                           void                          *arg);
sai_status_t stub_next_hop_group_type_get(_In_ const sai_object_key_t   *key,
                                          _Inout_ sai_attribute_value_t *value,
                                          _In_ uint32_t                  attr_index,
                                          _Inout_ vendor_cache_t        *cache,
                                          void                          *arg);
sai_status_t stub_next_hop_group_hop_list_get(_In_ const sai_object_key_t   *key,
                                              _Inout_ sai_attribute_value_t *value,
                                              _In_ uint32_t                  attr_index,
                                              _Inout_ vendor_cache_t        *cache,
                                              void                          *arg);
sai_status_t stub_next_hop_group_hop_list_set(_In_ const sai_object_key_t      *key,
                                              _In_ const sai_attribute_value_t *value,
                                              void                             *arg);

static const sai_vendor_attribute_entry_t next_hop_group_vendor_attribs[] = {
    { SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_COUNT,
      { false, false, false, true },
      { false, false, false, true },
      stub_next_hop_group_count_get, NULL,
      NULL, NULL },
    { SAI_NEXT_HOP_GROUP_ATTR_TYPE,
      { true, false, false, true },
      { true, false, false, true },
      stub_next_hop_group_type_get, NULL,
      NULL, NULL },
    { SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST,
      { true, false, true, true },
      { true, false, true, true },
      stub_next_hop_group_hop_list_get, NULL,
      stub_next_hop_group_hop_list_set, NULL },
};

/* State DB *************/
#define ECMP_MAX_PATHS 64

typedef struct _stub_next_hop_group_t {
    uint32_t        next_hop_count;
    sai_object_id_t next_hop_list[ECMP_MAX_PATHS];
    bool            is_valid;
} stub_next_hop_group_t;

#define MAX_NEXT_HOP_GROUP_NUMBER 1000
static stub_next_hop_group_t next_hop_group_db[MAX_NEXT_HOP_GROUP_NUMBER];

void db_init_next_hop_group()
{
    memset(next_hop_group_db, 0, sizeof(next_hop_group_db));
}

sai_status_t db_get_next_hop_group(_In_ uint32_t next_hop_group_id, _Out_ sai_object_list_t   *next_hop_list)
{
    if (NULL == next_hop_list) {
        STUB_LOG_ERR("NULL next hop list param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if ((next_hop_group_id >= MAX_NEXT_HOP_GROUP_NUMBER) ||
        (!next_hop_group_db[next_hop_group_id].is_valid)) {
        STUB_LOG_ERR("Invalid next hop group ID %u\n", next_hop_group_id);
        return SAI_STATUS_INVALID_PARAMETER;
    }

    next_hop_list->count = next_hop_group_db[next_hop_group_id].next_hop_count;
    next_hop_list->list  = next_hop_group_db[next_hop_group_id].next_hop_list;

    return SAI_STATUS_SUCCESS;
}

static sai_status_t db_find_free_index(_Out_ uint32_t *free_index)
{
    uint32_t ii;

    for (ii = 0; ii < MAX_NEXT_HOP_GROUP_NUMBER; ii++) {
        if (false == next_hop_group_db[ii].is_valid) {
            *free_index = ii;
            return SAI_STATUS_SUCCESS;
        }
    }

    STUB_LOG_ERR("Next hop group table full\n");
    return SAI_STATUS_TABLE_FULL;
}

static sai_status_t validate_next_hop_list(_In_ uint32_t               next_hop_count,
                                           _In_ const sai_object_id_t* nexthops,
                                           _In_ uint32_t               param_index)
{
    uint32_t ii;

    for (ii = 0; ii < next_hop_count; ii++) {
        if (SAI_OBJECT_TYPE_NEXT_HOP != sai_object_type_query(nexthops[ii])) {
            STUB_LOG_ERR("Invalid next hop object type %s, index %u\n",
                         SAI_TYPE_STR(sai_object_type_query(nexthops[ii])), ii);
            return SAI_STATUS_INVALID_ATTR_VALUE_0 + param_index;
        }
    }

    return SAI_STATUS_SUCCESS;
}

static sai_status_t db_create_next_hop_group(_Out_ uint32_t               *next_hop_group_id,
                                             _In_ const sai_object_list_t *next_hop_list,
                                             _In_ uint32_t                 param_index)
{
    sai_status_t status;

    if (NULL == next_hop_group_id) {
        STUB_LOG_ERR("NULL next hop group id param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == next_hop_list) {
        STUB_LOG_ERR("NULL next hop list param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (next_hop_list->count > ECMP_MAX_PATHS) {
        STUB_LOG_ERR("Next hop count %u bigger than maximum %u\n", next_hop_list->count, ECMP_MAX_PATHS);
        return SAI_STATUS_INVALID_ATTR_VALUE_0 + param_index;
    }

    if (0 == next_hop_list->count) {
        STUB_LOG_ERR("Can't create empty next hop group\n");
        return SAI_STATUS_INVALID_ATTR_VALUE_0 + param_index;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = db_find_free_index(next_hop_group_id))) {
        return status;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = validate_next_hop_list(next_hop_list->count, next_hop_list->list, param_index))) {
        return status;
    }

    next_hop_group_db[*next_hop_group_id].next_hop_count = next_hop_list->count;
    memcpy(next_hop_group_db[*next_hop_group_id].next_hop_list,
           next_hop_list->list,
           sizeof(sai_object_id_t) * next_hop_list->count);
    next_hop_group_db[*next_hop_group_id].is_valid = true;

    return SAI_STATUS_SUCCESS;
}

static sai_status_t db_remove_next_hop_group(_In_ uint32_t next_hop_group_id)
{
    if ((next_hop_group_id >= MAX_NEXT_HOP_GROUP_NUMBER) ||
        (!next_hop_group_db[next_hop_group_id].is_valid)) {
        STUB_LOG_ERR("Invalid next hop group ID %u\n", next_hop_group_id);
        return SAI_STATUS_INVALID_PARAMETER;
    }

    next_hop_group_db[next_hop_group_id].is_valid = false;

    return SAI_STATUS_SUCCESS;
}

sai_status_t db_update_next_hop_group_list(_In_ uint32_t next_hop_group_id, _In_ sai_object_list_t next_hop_list)
{
    sai_status_t status;

    if ((next_hop_group_id >= MAX_NEXT_HOP_GROUP_NUMBER) ||
        (!next_hop_group_db[next_hop_group_id].is_valid)) {
        STUB_LOG_ERR("Invalid next hop group ID %u\n", next_hop_group_id);
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (next_hop_list.count > ECMP_MAX_PATHS) {
        STUB_LOG_ERR("Next hop count %u bigger than maximum %u\n", next_hop_list.count, ECMP_MAX_PATHS);
        return SAI_STATUS_INVALID_ATTR_VALUE_0;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = validate_next_hop_list(next_hop_list.count, next_hop_list.list, 0))) {
        return status;
    }

    next_hop_group_db[next_hop_group_id].next_hop_count = next_hop_list.count;
    memcpy(next_hop_group_db[next_hop_group_id].next_hop_list,
           next_hop_list.list,
           sizeof(sai_object_id_t) * next_hop_list.count);

    return SAI_STATUS_SUCCESS;
}

sai_status_t db_add_members_next_hop_group_list(_In_ uint32_t               next_hop_group_id,
                                                _In_ uint32_t               next_hop_count,
                                                _In_ const sai_object_id_t* nexthops)
{
    stub_next_hop_group_t *group;
    sai_status_t           status;

    if ((next_hop_group_id >= MAX_NEXT_HOP_GROUP_NUMBER) ||
        (!next_hop_group_db[next_hop_group_id].is_valid)) {
        STUB_LOG_ERR("Invalid next hop group ID %u\n", next_hop_group_id);
        return SAI_STATUS_INVALID_PARAMETER;
    }

    group = &next_hop_group_db[next_hop_group_id];

    if (next_hop_count + group->next_hop_count > ECMP_MAX_PATHS) {
        STUB_LOG_ERR("Next hop count %u bigger than maximum %u\n",
                     next_hop_count + group->next_hop_count, ECMP_MAX_PATHS);
        return SAI_STATUS_INVALID_ATTR_VALUE_0;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = validate_next_hop_list(next_hop_count, nexthops, 0))) {
        return status;
    }

    memcpy(&group->next_hop_list[group->next_hop_count],
           nexthops,
           sizeof(sai_object_id_t) * next_hop_count);
    group->next_hop_count += next_hop_count;

    return SAI_STATUS_SUCCESS;
}

bool next_hop_in_list(sai_object_id_t nexthop, _In_ uint32_t next_hop_count, _In_ const sai_object_id_t* nexthops)
{
    uint32_t ii;

    for (ii = 0; ii < next_hop_count; ii++) {
        if (nexthop == nexthops[ii]) {
            return true;
        }
    }

    return false;
}

sai_status_t db_remove_members_next_hop_group_list(_In_ uint32_t               next_hop_group_id,
                                                   _In_ uint32_t               next_hop_count,
                                                   _In_ const sai_object_id_t* nexthops)
{
    stub_next_hop_group_t *group;
    uint32_t               ii = 0;

    if ((next_hop_group_id >= MAX_NEXT_HOP_GROUP_NUMBER) ||
        (!next_hop_group_db[next_hop_group_id].is_valid)) {
        STUB_LOG_ERR("Invalid next hop group ID %u\n", next_hop_group_id);
        return SAI_STATUS_INVALID_PARAMETER;
    }

    group = &next_hop_group_db[next_hop_group_id];

    while (ii < group->next_hop_count) {
        if (next_hop_in_list(group->next_hop_list[ii], next_hop_count, nexthops)) {
            group->next_hop_count--;
            group->next_hop_list[ii] = group->next_hop_list[group->next_hop_count];
            continue;
        }
        ii++;
    }

    return SAI_STATUS_SUCCESS;
}

/*************************/

static void next_hop_group_key_to_str(_In_ sai_object_id_t next_hop_group_id, _Out_ char *key_str)
{
    uint32_t groupid;

    if (SAI_STATUS_SUCCESS != stub_object_to_type(next_hop_group_id, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, &groupid)) {
        snprintf(key_str, MAX_KEY_STR_LEN, "invalid next hop group id");
    } else {
        snprintf(key_str, MAX_KEY_STR_LEN, "next hop group id %u", groupid);
    }
}

/*
 * Routine Description:
 *    Create next hop group
 *
 * Arguments:
 *    [out] next_hop_group_id - next hop group id
 *    [in] attr_count - number of attributes
 *    [in] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_create_next_hop_group(_Out_ sai_object_id_t     * next_hop_group_id,
                                        _In_ uint32_t               attr_count,
                                        _In_ const sai_attribute_t *attr_list)
{
    sai_status_t                 status;
    const sai_attribute_value_t *type, *hop_list;
    uint32_t                     type_index, hop_list_index, group_id = 0;
    char                         list_str[MAX_LIST_VALUE_STR_LEN];
    char                         key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == next_hop_group_id) {
        STUB_LOG_ERR("NULL next hop group id param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (SAI_STATUS_SUCCESS !=
        (status =
             check_attribs_metadata(attr_count, attr_list, next_hop_group_attribs, next_hop_group_vendor_attribs,
                                    SAI_OPERATION_CREATE))) {
        STUB_LOG_ERR("Failed attribs check\n");
        return status;
    }

    sai_attr_list_to_str(attr_count, attr_list, next_hop_group_attribs, MAX_LIST_VALUE_STR_LEN, list_str);
    STUB_LOG_NTC("Create next hop group, %s\n", list_str);

    assert(SAI_STATUS_SUCCESS ==
           find_attrib_in_list(attr_count, attr_list, SAI_NEXT_HOP_GROUP_ATTR_TYPE, &type, &type_index));
    assert(SAI_STATUS_SUCCESS ==
           find_attrib_in_list(attr_count,
                               attr_list,
                               SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST,
                               &hop_list,
                               &hop_list_index));

    if (SAI_NEXT_HOP_GROUP_ECMP != type->s32) {
        STUB_LOG_ERR("Invalid next hop group type %d on create\n", type->s32);
        return SAI_STATUS_INVALID_ATTR_VALUE_0 + type_index;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = db_create_next_hop_group(&group_id, &(hop_list->objlist), hop_list_index))) {
        return status;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = stub_create_object(SAI_OBJECT_TYPE_NEXT_HOP_GROUP, group_id, next_hop_group_id))) {
        return status;
    }
    next_hop_group_key_to_str(*next_hop_group_id, key_str);
    STUB_LOG_NTC("Created next hop group %s\n", key_str);

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Remove next hop group
 *
 * Arguments:
 *    [in] next_hop_group_id - next hop group id
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_remove_next_hop_group(_In_ sai_object_id_t next_hop_group_id)
{
    char         key_str[MAX_KEY_STR_LEN];
    sai_status_t status;
    uint32_t     group_id;

    STUB_LOG_ENTER();

    next_hop_group_key_to_str(next_hop_group_id, key_str);
    STUB_LOG_NTC("Remove next hop group %s\n", key_str);

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(next_hop_group_id, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, &group_id))) {
        return status;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = db_remove_next_hop_group(group_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Set Next Hop Group attribute
 *
 * Arguments:
 *    [in] sai_next_hop_group_id_t - next_hop_group_id
 *    [in] attr - attribute
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_set_next_hop_group_attribute(_In_ sai_object_id_t        next_hop_group_id,
                                               _In_ const sai_attribute_t *attr)
{
    const sai_object_key_t key = { .object_id = next_hop_group_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    next_hop_group_key_to_str(next_hop_group_id, key_str);
    return sai_set_attribute(&key, key_str, next_hop_group_attribs, next_hop_group_vendor_attribs, attr);
}

/*
 * Routine Description:
 *    Get Next Hop Group attribute
 *
 * Arguments:
 *    [in] sai_next_hop_group_id_t - next_hop_group_id
 *    [in] attr_count - number of attributes
 *    [inout] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_get_next_hop_group_attribute(_In_ sai_object_id_t     next_hop_group_id,
                                               _In_ uint32_t            attr_count,
                                               _Inout_ sai_attribute_t *attr_list)
{
    const sai_object_key_t key = { .object_id = next_hop_group_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    next_hop_group_key_to_str(next_hop_group_id, key_str);
    return sai_get_attributes(&key,
                              key_str,
                              next_hop_group_attribs,
                              next_hop_group_vendor_attribs,
                              attr_count,
                              attr_list);
}

/* Next hop group type [sai_next_hop_group_type_t] */
sai_status_t stub_next_hop_group_type_get(_In_ const sai_object_key_t   *key,
                                          _Inout_ sai_attribute_value_t *value,
                                          _In_ uint32_t                  attr_index,
                                          _Inout_ vendor_cache_t        *cache,
                                          void                          *arg)
{
    STUB_LOG_ENTER();

    value->s32 = SAI_NEXT_HOP_GROUP_ECMP;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Number of next hops in the group [uint32_t] */
sai_status_t stub_next_hop_group_count_get(_In_ const sai_object_key_t   *key,
                                           _Inout_ sai_attribute_value_t *value,
                                           _In_ uint32_t                  attr_index,
                                           _Inout_ vendor_cache_t        *cache,
                                           void                          *arg)
{
    sai_status_t      status;
    uint32_t          group_id;
    sai_object_list_t hop_list;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, &group_id))) {
        return status;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = db_get_next_hop_group(group_id, &hop_list))) {
        return status;
    }

    value->u32 = hop_list.count;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Next hop list [sai_object_list_t] */
sai_status_t stub_next_hop_group_hop_list_get(_In_ const sai_object_key_t   *key,
                                              _Inout_ sai_attribute_value_t *value,
                                              _In_ uint32_t                  attr_index,
                                              _Inout_ vendor_cache_t        *cache,
                                              void                          *arg)
{
    sai_status_t      status;
    uint32_t          group_id;
    sai_object_list_t hop_list;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, &group_id))) {
        return status;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = db_get_next_hop_group(group_id, &hop_list))) {
        return status;
    }

    if (SAI_STATUS_SUCCESS != (status = stub_fill_objlist(hop_list.list, hop_list.count, &value->objlist))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Next hop list [sai_object_list_t] */
sai_status_t stub_next_hop_group_hop_list_set(_In_ const sai_object_key_t      *key,
                                              _In_ const sai_attribute_value_t *value,
                                              void                             *arg)
{
    sai_status_t status;
    uint32_t     group_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, &group_id))) {
        return status;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = db_update_next_hop_group_list(group_id, value->objlist))) {
        return status;
    }

    /* TODO : update route with next hop list changes */

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Add next hop to a group
 *
 * Arguments:
 *    [in] next_hop_group_id - next hop group id
 *    [in] next_hop_count - number of next hops
 *    [in] nexthops - array of next hops
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_add_next_hop_to_group(_In_ sai_object_id_t        next_hop_group_id,
                                        _In_ uint32_t               next_hop_count,
                                        _In_ const sai_object_id_t* nexthops)
{
    sai_status_t status;
    char         value[MAX_LIST_VALUE_STR_LEN];
    uint32_t     group_id;
    char         key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == nexthops) {
        STUB_LOG_ERR("NULL nexthops param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    next_hop_group_key_to_str(next_hop_group_id, key_str);
    sai_nexthops_to_str(next_hop_count, nexthops, MAX_LIST_VALUE_STR_LEN, value);
    STUB_LOG_NTC("Add next hops {%s} to %s\n", value, key_str);

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(next_hop_group_id, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, &group_id))) {
        return status;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = db_add_members_next_hop_group_list(group_id, next_hop_count, nexthops))) {
        return status;
    }

    /* TODO : update route with next hop list changes */

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Remove next hop from a group
 *
 * Arguments:
 *    [in] next_hop_group_id - next hop group id
 *    [in] next_hop_count - number of next hops
 *    [in] nexthops - array of next hops
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_remove_next_hop_from_group(_In_ sai_object_id_t        next_hop_group_id,
                                             _In_ uint32_t               next_hop_count,
                                             _In_ const sai_object_id_t* nexthops)
{
    sai_status_t status;
    char         value[MAX_LIST_VALUE_STR_LEN];
    uint32_t     group_id;
    char         key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == nexthops) {
        STUB_LOG_ERR("NULL nexthops param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    next_hop_group_key_to_str(next_hop_group_id, key_str);
    sai_nexthops_to_str(next_hop_count, nexthops, MAX_LIST_VALUE_STR_LEN, value);
    STUB_LOG_NTC("Remove next hops {%s} from %s\n", value, key_str);

    if (SAI_STATUS_SUCCESS !=
        (status = stub_object_to_type(next_hop_group_id, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, &group_id))) {
        return status;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = db_remove_members_next_hop_group_list(group_id, next_hop_count, nexthops))) {
        return status;
    }

    /* TODO : update route with next hop list changes */

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

const sai_next_hop_group_api_t next_hop_group_api = {
    stub_create_next_hop_group,
    stub_remove_next_hop_group,
    stub_set_next_hop_group_attribute,
    stub_get_next_hop_group_attribute,
    stub_add_next_hop_to_group,
    stub_remove_next_hop_from_group
};
