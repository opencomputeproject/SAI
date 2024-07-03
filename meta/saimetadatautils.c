/**
 * Copyright (c) 2014 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
 *
 * @file    saimetadatautils.c
 *
 * @brief   This module defines SAI Metadata Utils
 */

#include <stdio.h>
#include <string.h>
#include <sai.h>
#include "saimetadatautils.h"
#include "saimetadata.h"

bool sai_metadata_is_allowed_object_type(
        _In_ const sai_attr_metadata_t* metadata,
        _In_ sai_object_type_t object_type)
{
    if (metadata == NULL || metadata->allowedobjecttypes == NULL)
    {
        return false;
    }

    size_t i = 0;

    for (; i < metadata->allowedobjecttypeslength; ++i)
    {
        if (metadata->allowedobjecttypes[i] == object_type)
        {
            return true;
        }
    }

    return false;
}

bool sai_metadata_is_allowed_enum_value(
        _In_ const sai_attr_metadata_t* metadata,
        _In_ int value)
{
    if (metadata == NULL || metadata->enummetadata == NULL)
    {
        return false;
    }

    size_t i = 0;

    const sai_enum_metadata_t *emd = metadata->enummetadata;

    for (; i < emd->valuescount; ++i)
    {
        if (emd->values[i] == value)
        {
            return true;
        }
    }

    return false;
}

const sai_attr_metadata_t* sai_metadata_get_attr_metadata(
        _In_ sai_object_type_t objecttype,
        _In_ sai_attr_id_t attrid)
{
    if (sai_metadata_is_object_type_valid(objecttype))
    {
        const sai_attr_metadata_t* const* const md = sai_metadata_attr_by_object_type[objecttype];

        /*
         * Most object attributes are not flags, so we can use direct index to
         * find attribute metadata, this should speed up search.
         */

        const sai_object_type_info_t* oi = sai_metadata_all_object_type_infos[objecttype];

        if (!oi->enummetadata->containsflags && attrid < oi->attridend)
        {
            return md[attrid];
        }

        /* otherwise search one by one */

        size_t index = 0;

        for (; md[index] != NULL; index++)
        {
            if (md[index]->attrid == attrid)
            {
                return md[index];
            }
        }
    }

    return NULL;
}

const sai_attr_metadata_t* sai_metadata_get_attr_metadata_by_attr_id_name(
        _In_ const char *attr_id_name)
{
    if (attr_id_name == NULL)
    {
        return NULL;
    }

    /* use binary search */

    ssize_t first = 0;
    ssize_t last = (ssize_t)(sai_metadata_attr_sorted_by_id_name_count - 1);

    while (first <= last)
    {
        ssize_t middle = (first + last) / 2;

        int res = strcmp(attr_id_name, sai_metadata_attr_sorted_by_id_name[middle]->attridname);

        if (res > 0)
        {
            first = middle + 1;
        }
        else if (res < 0)
        {
            last = middle - 1;
        }
        else
        {
            /* found */

            return sai_metadata_attr_sorted_by_id_name[middle];
        }
    }

    /* not found */

    return NULL;
}

static int sai_metadata_attr_id_name_cmp(
        _In_ const char * str1,
        _In_ const char * str2)
{
    char c1 = 0;
    char c2 = 0;

    while (true)
    {
        c1 = *str1++;
        c2 = *str2++;

        if (sai_serialize_is_char_allowed(c1) || sai_serialize_is_char_allowed(c2) || c1 != c2)
        {
            if (sai_serialize_is_char_allowed(c1))
            {
                c1 = 0;
            }

            if (sai_serialize_is_char_allowed(c2))
            {
                c2 = 0;
            }

            return c1 - c2;
        }
    }
}

const sai_attr_metadata_t* sai_metadata_get_attr_metadata_by_attr_id_name_ext(
        _In_ const char *attr_id_name)
{
    if (attr_id_name == NULL)
    {
        return NULL;
    }

    /* use binary search */

    ssize_t first = 0;
    ssize_t last = (ssize_t)(sai_metadata_attr_sorted_by_id_name_count - 1);

    while (first <= last)
    {
        ssize_t middle = (first + last) / 2;

        int res = sai_metadata_attr_id_name_cmp(attr_id_name, sai_metadata_attr_sorted_by_id_name[middle]->attridname);

        if (res > 0)
        {
            first = middle + 1;
        }
        else if (res < 0)
        {
            last = middle - 1;
        }
        else
        {
            /* found */

            return sai_metadata_attr_sorted_by_id_name[middle];
        }
    }

    /* not found */

    return NULL;
}

const sai_attr_metadata_t* sai_metadata_get_ignored_attr_metadata_by_attr_id_name(
        _In_ const char *attr_id_name)
{
    if (attr_id_name == NULL)
    {
        return NULL;
    }

    sai_object_type_t ot;

    /*
     * Since we don't have list of ignored attributes, enumerate all objects
     * and attribute enums to find ignored values.
     */

    for (ot = SAI_OBJECT_TYPE_NULL; ot < SAI_OBJECT_TYPE_EXTENSIONS_MAX; ot++)
    {
        const sai_object_type_info_t* oti = sai_metadata_get_object_type_info(ot);

        if (oti == NULL)
            continue;

        const sai_enum_metadata_t* em = oti->enummetadata;

        if (em->ignorevaluesnames)
        {
            size_t i;

            for (i = 0; em->ignorevaluesnames[i] != NULL; i++)
            {
                if (strcmp(attr_id_name, em->ignorevaluesnames[i]) == 0)
                {
                    const char* name = sai_metadata_get_enum_value_name(em, em->ignorevalues[i]);

                    return sai_metadata_get_attr_metadata_by_attr_id_name(name);
                }
            }
        }
    }

    return NULL;
}

const char* sai_metadata_get_enum_value_name(
        _In_ const sai_enum_metadata_t* metadata,
        _In_ int value)
{
    if (metadata == NULL)
    {
        return NULL;
    }

    size_t i = 0;

    for (; i < metadata->valuescount; ++i)
    {
        if (metadata->values[i] == value)
        {
            return metadata->valuesnames[i];
        }
    }

    return NULL;
}

const sai_attribute_t* sai_metadata_get_attr_by_id(
        _In_ sai_attr_id_t id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    if (attr_list == NULL)
    {
        return NULL;
    }

    uint32_t i = 0;

    for (; i < attr_count; ++i)
    {
        if (attr_list[i].id == id)
        {
            return &attr_list[i];
        }
    }

    return NULL;
}

const sai_object_type_info_t* sai_metadata_get_object_type_info(
        _In_ sai_object_type_t object_type)
{
    if (sai_metadata_is_object_type_valid(object_type))
    {
        return sai_metadata_all_object_type_infos[object_type];
    }

    return NULL;
}

bool sai_metadata_is_object_type_oid(
        _In_ sai_object_type_t object_type)
{
    const sai_object_type_info_t* oti = sai_metadata_get_object_type_info(object_type);

    if (oti != NULL)
    {
        return oti->isobjectid;
    }

    return false;
}

bool sai_metadata_is_object_type_valid(
        _In_ sai_object_type_t object_type)
{
    return object_type > SAI_OBJECT_TYPE_NULL && object_type < SAI_OBJECT_TYPE_EXTENSIONS_MAX;
}

static bool sai_metadata_is_condition_value_eq(
        _In_ sai_attr_value_type_t attrvaluetype,
        _In_ const sai_attribute_value_t* cvalue,
        _In_ const sai_attribute_value_t* value)
{
    if (cvalue == NULL || value == NULL)
    {
        return false;
    }

    switch (attrvaluetype)
    {
        case SAI_ATTR_VALUE_TYPE_BOOL:
            return cvalue->booldata == value->booldata;

        case SAI_ATTR_VALUE_TYPE_INT8:
            return cvalue->s8 == value->s8;

        case SAI_ATTR_VALUE_TYPE_INT16:
            return cvalue->s16 == value->s16;

        case SAI_ATTR_VALUE_TYPE_INT32:
            return cvalue->s32 == value->s32;

        case SAI_ATTR_VALUE_TYPE_INT64:
            return cvalue->s64 == value->s64;

        case SAI_ATTR_VALUE_TYPE_UINT8:
            return cvalue->u8 == value->u8;

        case SAI_ATTR_VALUE_TYPE_UINT16:
            return cvalue->u16 == value->u16;

        case SAI_ATTR_VALUE_TYPE_UINT32:
            return cvalue->u32 == value->u32;

        case SAI_ATTR_VALUE_TYPE_UINT64:
            return cvalue->u64 == value->u64;

        default:

            /*
             * We should never get here since sanity check tests all
             * attributes and all conditions.
             */

            SAI_META_LOG_ERROR("condition value type %d is not supported, FIXME", attrvaluetype);

            return false;
    }
}

static bool sai_metadata_is_single_condition_met(
        _In_ sai_object_type_t objecttype,
        _In_ const sai_attr_condition_t *condition,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    /*
     * Conditions may only be on the same object type.
     *
     * Default value may not exists if conditional object is marked as
     * MANDATORY_ON_CREATE.
     */

    const sai_attr_metadata_t *cmd = sai_metadata_get_attr_metadata(objecttype, condition->attrid);

    const sai_attribute_t *cattr = sai_metadata_get_attr_by_id(condition->attrid, attr_count, attr_list);

    if (cattr == NULL)
    {
        /*
         * User didn't passed conditional attribute, so check if there is
         * default value.
         */

        return sai_metadata_is_condition_value_eq(cmd->attrvaluetype, &condition->condition, cmd->defaultvalue);
    }
    else
    {
        return sai_metadata_is_condition_value_eq(cmd->attrvaluetype, &condition->condition, &cattr->value);
    }
}

static bool sai_metadata_is_and_condition_list_met(
        _In_ const sai_attr_metadata_t *md,
        _In_ size_t length,
        _In_ const sai_attr_condition_t* const* list,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    size_t idx = 0;

    bool met = length > 0;

    for (; idx < length; ++idx)
    {
        const sai_attr_condition_t *condition = list[idx];

        met &= sai_metadata_is_single_condition_met(md->objecttype, condition, attr_count, attr_list);
    }

    return met;
}

static bool sai_metadata_is_or_condition_list_met(
        _In_ const sai_attr_metadata_t *md,
        _In_ size_t length,
        _In_ const sai_attr_condition_t* const* list,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    size_t idx = 0;

    bool met = false;

    for (; idx < length; ++idx)
    {
        const sai_attr_condition_t *condition = list[idx];

        met |= sai_metadata_is_single_condition_met(md->objecttype, condition, attr_count, attr_list);
    }

    return met;
}

#define STACK_PUSH(val) stack[stack_size++] = (val)
#define STACK_POP() stack[--stack_size]

static bool sai_metadata_is_mixed_condition_list_met(
        _In_ const sai_attr_metadata_t *md,
        _In_ size_t length,
        _In_ const sai_attr_condition_t* const* list,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    int stack_size = 0;

    bool stack[SAI_METADATA_MAX_CONDITIONS_LEN];

    size_t idx = 0;

    for (; idx < length; idx++)
    {
        const sai_attr_condition_t* c = list[idx];

        if (c->type == SAI_ATTR_CONDITION_TYPE_NONE)
        {
            bool value = sai_metadata_is_single_condition_met(md->objecttype, c, attr_count, attr_list);

            STACK_PUSH(value);
        }
        else if (c->type == SAI_ATTR_CONDITION_TYPE_AND)
        {
            bool a = STACK_POP();
            bool b = STACK_POP();

            STACK_PUSH(a & b);
        }
        else if (c->type == SAI_ATTR_CONDITION_TYPE_OR)
        {
            bool a = STACK_POP();
            bool b = STACK_POP();

            STACK_PUSH(a | b);
        }
        else
        {
            SAI_META_LOG_ERROR("%s: wrong condition type on list: %d", md->attridname, c->type);
            return false;
        }
    }

    bool value = STACK_POP();

    if (stack_size)
    {
        SAI_META_LOG_ERROR("FATAL %s: stack not empty after condition list check, RPN condition logic is BROKEN", md->attridname);
        return false;
    }

    return value;
}

bool sai_metadata_is_condition_met(
        _In_ const sai_attr_metadata_t *md,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    /* attr list can be NULL, condition could be based on default value */

    if (md == NULL || !md->isconditional)
    {
        return false;
    }

    switch (md->conditiontype)
    {
        case SAI_ATTR_CONDITION_TYPE_AND:
            return sai_metadata_is_and_condition_list_met(md, md->conditionslength, md->conditions, attr_count, attr_list);

        case SAI_ATTR_CONDITION_TYPE_OR:
            return sai_metadata_is_or_condition_list_met(md, md->conditionslength, md->conditions, attr_count, attr_list);

        case SAI_ATTR_CONDITION_TYPE_MIXED:
            return sai_metadata_is_mixed_condition_list_met(md, md->conditionslength, md->conditions, attr_count, attr_list);

        default:
            SAI_META_LOG_ERROR("condition type %d on %s is not supported yet, FIXME", md->conditiontype, md->attridname);
            return false;
    }
}

bool sai_metadata_is_validonly_met(
        _In_ const sai_attr_metadata_t *md,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    /* attr list can be NULL, condition could be based on default value */

    if (md == NULL || !md->isvalidonly)
    {
        return false;
    }

    switch (md->validonlytype)
    {
        case SAI_ATTR_CONDITION_TYPE_AND:
            return sai_metadata_is_and_condition_list_met(md, md->validonlylength, md->validonly, attr_count, attr_list);

        case SAI_ATTR_CONDITION_TYPE_OR:
            return sai_metadata_is_or_condition_list_met(md, md->validonlylength, md->validonly, attr_count, attr_list);

        case SAI_ATTR_CONDITION_TYPE_MIXED:
            return sai_metadata_is_mixed_condition_list_met(md, md->validonlylength, md->validonly, attr_count, attr_list);

        default:
            SAI_META_LOG_ERROR("validonly type %d on %s is not supported yet, FIXME", md->validonlytype, md->attridname);
            return false;
    }

    return false;
}

sai_api_version_t sai_metadata_query_api_version(void)
{
    return SAI_API_VERSION;
}
