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
 * @file    saisanitycheck.c
 *
 * @brief   Defines SAI metadata sanity check
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloca.h>
#include <sai.h>
#include <saiversion.h>
#include "saimetadatautils.h"
#include "saimetadata.h"
#include "saimetadatalogger.h"

typedef struct _defined_attr_t {

    const sai_attr_metadata_t* metadata;

    const struct _defined_attr_t* next;

} defined_attr_t;

bool debug = false;

defined_attr_t* defined_attributes = NULL;

#define META_LOG_DEBUG(format, ...)\
    if (debug) { printf("DEBUG: " format "\n", ##__VA_ARGS__); }

#define META_LOG_WARN(format, ...)\
    fprintf(stderr, "WARN: " format "\n", ##__VA_ARGS__);

#define META_LOG_INFO(format, ...)\
    fprintf(stderr, "INFO: " format "\n", ##__VA_ARGS__);

#define META_LOG_ENTER() \
    META_LOG_DEBUG(":> %s", __FUNCTION__);

#define META_ENUM_LOG_WARN(emd, format, ...)\
    META_LOG_WARN("%s: " format, emd->name, ##__VA_ARGS__);

#define META_MD_LOG_WARN(md, format, ...)\
    META_LOG_WARN("%s: " format, md->attridname, ##__VA_ARGS__);

#define META_ASSERT_FAIL(format, ...)                       \
{                                                           \
    fprintf(stderr,                                         \
            " ASSERT FAILED (on line %d): " format "\n",    \
            __LINE__, ##__VA_ARGS__);                       \
    exit(1);                                                \
}

#define META_MD_ASSERT_FAIL(md, format, ...)\
    META_ASSERT_FAIL("%s: " format, md->attridname, ##__VA_ARGS__);

#define META_ENUM_ASSERT_FAIL(emd, format, ...)\
    META_ASSERT_FAIL("%s: " format, emd->name, ##__VA_ARGS__);

#define META_ASSERT_NOT_NULL(x)\
    if ((x) == NULL) { META_ASSERT_FAIL("not expected NULL: " #x); }

#define META_ASSERT_NULL(x)\
    if ((x) != NULL) { META_ASSERT_FAIL("expected NULL: " #x); }

#define META_ASSERT_TRUE(x, format, ...)\
    if ((x) == false) { META_ASSERT_FAIL("expected true '" #x "': " format, ##__VA_ARGS__); }

#define META_ASSERT_FALSE(x, format, ...)\
    if ((x) == true) { META_ASSERT_FAIL("expected false '" #x "': " format, ##__VA_ARGS__); }

/* custom ranges start are the same for all objects */

#define CUSTOM_ATTR_RANGE_START SAI_PORT_ATTR_CUSTOM_RANGE_START

bool is_extensions_enum(
        _In_ const sai_enum_metadata_t* emd)
{
    META_LOG_ENTER();

    return strstr(emd->name, "_extensions_t") != NULL;
}

void check_all_enums_name_pointers()
{
    META_LOG_ENTER();

    size_t i = 0;

    META_ASSERT_TRUE(sai_metadata_all_enums_count > 100, "we need to have some enums");

    for (; i < sai_metadata_all_enums_count; ++i)
    {
        const sai_enum_metadata_t* emd = sai_metadata_all_enums[i];

        META_ASSERT_NOT_NULL(emd);

        META_LOG_DEBUG("enum: %s", emd->name);

        META_ASSERT_NOT_NULL(emd->name);
        META_ASSERT_NOT_NULL(emd->values);
        META_ASSERT_NOT_NULL(emd->valuesnames);
        META_ASSERT_NOT_NULL(emd->valuesshortnames);

        if (is_extensions_enum(emd))
        {
            /* allow empty extensions enums */

            if (emd->valuescount == 0)
                META_LOG_WARN("enum %s has no values", emd->name);
        }
        else
        {
            META_ASSERT_TRUE(emd->valuescount > 0, "enum must have some values");
        }

        size_t j = 0;

        for (; j < emd->valuescount; ++j)
        {
            META_LOG_DEBUG(" value: %s", emd->valuesnames[j]);

            META_ASSERT_NOT_NULL(emd->valuesnames[j]);
            META_ASSERT_NOT_NULL(emd->valuesshortnames[j]);
        }

        META_ASSERT_TRUE(emd->valuesnames[j] == NULL, "missing null pointer after enum names");
        META_ASSERT_TRUE(emd->valuesshortnames[j] == NULL, "missing null pointer after enum short names");
    }
}

bool is_flag_enum(const sai_enum_metadata_t* emd)
{
    META_LOG_ENTER();

    return emd->containsflags;
}

void check_all_enums_values()
{
    META_LOG_ENTER();

    size_t i = 0;

    for (; i < sai_metadata_all_enums_count; ++i)
    {
        const sai_enum_metadata_t* emd = sai_metadata_all_enums[i];

        META_LOG_DEBUG("enum: %s", emd->name);

        bool flags = false;

        size_t j = 0;

        int last = -1;

        if (strcmp(emd->name, "sai_status_t") == 0)
        {
            /* status values are negative */
            continue;
        }

        for (; j < emd->valuescount; ++j)
        {
            META_LOG_DEBUG(" value: %s", emd->valuesnames[j]);

            int value = emd->values[j];

            META_ASSERT_FALSE(value < 0, "enum values are negative");

            META_ASSERT_TRUE(last < value, "enum values are not increasing");

            if (j == 0)
            {
                if (value != 0)
                {
                    flags = true;

                    if (is_flag_enum(emd))
                    {
                        /* ok, flags not need zero enum */
                    }
                    else
                    {
                        META_ENUM_ASSERT_FAIL(emd, "first enum should start with zero");
                    }
                }
            }

            if (value != last + 1)
            {
                flags = true;

                if (is_flag_enum(emd))
                {
                    /* flags, ok */
                }
                else
                {
                    META_ENUM_ASSERT_FAIL(emd, "values are not increasing by 1: last: %d current: %d, should be marked as @flags?", last, value);
                }
            }

            last = emd->values[j];

            if (value >= CUSTOM_ATTR_RANGE_START && value < (2 * CUSTOM_ATTR_RANGE_START))
            {
                /* value is in custom range */
            }
            else
            {
                META_ASSERT_TRUE(value < 0x10000, "enum value is too big, range?");
            }
        }

        META_ASSERT_TRUE(emd->values[j] == -1, "missing guard at the end of enum");

        if (emd->valuescount > 0 && flags != emd->containsflags)
        {
            META_ENUM_ASSERT_FAIL(emd, "enum flags: %d but declared as %d", flags, emd->containsflags);
        }
    }
}

void check_enums_ignore_values()
{
    META_LOG_ENTER();

    size_t i = 0;

    for (; i < sai_metadata_all_enums_count; ++i)
    {
        const sai_enum_metadata_t* emd = sai_metadata_all_enums[i];

        META_LOG_DEBUG("enum: %s", emd->name);

        if (emd->ignorevalues == NULL)
        {
            META_ASSERT_TRUE(emd->ignorevaluesnames == NULL, "names must be NULL when values are NULL");
            continue;
        }

        META_ASSERT_TRUE(emd->ignorevaluesnames != NULL, "names must be NOT NULL when values are defined");

        size_t j = 0;

        for (; emd->ignorevalues[j] != -1; ++j)
        {
            META_LOG_DEBUG(" value: %s", emd->ignorevaluesnames[j]);

            META_ASSERT_TRUE(emd->ignorevaluesnames[j] != NULL, "names must be NOT NULL when values are defined");

            int value = emd->ignorevalues[j];

            META_ASSERT_FALSE(value < 0, "enum values are negative");

            bool contains = false;

            size_t k = 0;

            for (; k < emd->valuescount; k++)
            {
                if (emd->values[k] == value)
                {
                    contains = true;
                    break;
                }
            }

            META_ASSERT_TRUE(contains, "ignored enum value must be defined in enum values");
        }

        META_ASSERT_TRUE(emd->ignorevalues[j] == -1, "missing guard at the end of enum");
        META_ASSERT_TRUE(emd->ignorevaluesnames[j] == NULL, "missing guard at the end of enum");
    }
}

void check_sai_status()
{
    META_LOG_ENTER();

    META_ASSERT_TRUE(SAI_STATUS_SUCCESS == 0, "success must be zero");
    META_ASSERT_TRUE(sai_metadata_enum_sai_status_t.valuescount > 1, "there must be error codes");

    size_t i = 0;

    int last = 1;

    for (; i < sai_metadata_enum_sai_status_t.valuescount; ++i)
    {
        META_LOG_DEBUG("status: %s", sai_metadata_enum_sai_status_t.valuesnames[i]);

        int value = sai_metadata_enum_sai_status_t.values[i];

        if (i == 0)
        {
            META_ASSERT_TRUE(value == 0, "first must be status success");
        }
        else
        {
            META_ASSERT_TRUE(last > value, "status codes are not decreasing");
        }

        last = value;
    }

    META_ASSERT_TRUE(sai_metadata_enum_sai_status_t.containsflags, "sai_status_t must be marked as containsflags");
}

void check_object_type()
{
    META_LOG_ENTER();

    META_ASSERT_TRUE(SAI_OBJECT_TYPE_NULL == 0, "sai object type null must be zero");

    size_t i = 0;

    int last = -1; /* will enforce NULL be first */

    for (; i < sai_metadata_enum_sai_object_type_t.valuescount; ++i)
    {
        META_LOG_DEBUG("object_type: %s", sai_metadata_enum_sai_object_type_t.valuesnames[i]);

        int value = sai_metadata_enum_sai_object_type_t.values[i];

        META_ASSERT_TRUE(value == last + 1, "object type values must be consecutive numbers");

        last = value;
    }
}

void check_attr_by_object_type()
{
    META_LOG_ENTER();

    META_ASSERT_TRUE(SAI_OBJECT_TYPE_EXTENSIONS_MAX - SAI_OBJECT_TYPE_MAX < 50, "too many experimental object types");

    META_ASSERT_TRUE(SAI_OBJECT_TYPE_MAX <= SAI_OBJECT_TYPE_EXTENSIONS_MAX, "invalid object type count in metadata");
    META_ASSERT_TRUE(sai_metadata_attr_by_object_type_count == SAI_OBJECT_TYPE_EXTENSIONS_MAX, "invalid object type count in metadata");

    size_t i = 0;

    for (; i < sai_metadata_attr_by_object_type_count; ++i)
    {
        META_LOG_DEBUG("processing %zu, %s", i, sai_metadata_get_object_type_name((sai_object_type_t)i));

        META_ASSERT_NOT_NULL(sai_metadata_attr_by_object_type[i]);

        const sai_attr_metadata_t * const* const ot = sai_metadata_attr_by_object_type[i];

        size_t index = 0;

        while (ot[index] != NULL)
        {
            sai_object_type_t current = ot[index]->objecttype;

            META_ASSERT_TRUE(current == i, "object type must be equal on object type list");

            /*
             * For Switch Attribute we have crossed > 300 with Vendor extension
             * for SAI v1.8.0 so increasing threshold.
             */

            META_ASSERT_TRUE(index < 300, "object defines > 300 attributes, metadata bug?");
            META_ASSERT_TRUE(current > SAI_OBJECT_TYPE_NULL, "object type must be > NULL");
            META_ASSERT_TRUE(current < SAI_OBJECT_TYPE_EXTENSIONS_MAX, "object type must be < MAX");

            /* META_LOG_DEBUG("processing indexer %lu", index); */

            index++;
        }

        META_LOG_DEBUG("attr index %zu for %s", index, sai_metadata_get_object_type_name((sai_object_type_t)i));
    }

    META_ASSERT_NULL(sai_metadata_attr_by_object_type[i]);
}

bool is_valid_object_type(
        _In_ sai_object_type_t ot)
{
    META_LOG_ENTER();

    return (ot > SAI_OBJECT_TYPE_NULL) && (ot < SAI_OBJECT_TYPE_EXTENSIONS_MAX);
}

void check_attr_object_type(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (!is_valid_object_type(md->objecttype))
    {
        META_MD_ASSERT_FAIL(md, "invalid object type value %d", md->objecttype);
    }
}

void check_attr_value_type_range(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    META_ASSERT_NOT_NULL(sai_metadata_get_enum_value_name(&sai_metadata_enum_sai_attr_value_type_t, md->attrvaluetype));
}

bool sai_metadata_is_acl_field_or_action(
        _In_ const sai_attr_metadata_t* metadata)
{
    if (metadata == NULL)
    {
        return false;
    }

    if (metadata->objecttype == SAI_OBJECT_TYPE_ACL_ENTRY)
    {
        if (metadata->attrid >= SAI_ACL_ENTRY_ATTR_FIELD_START &&
                metadata->attrid <= SAI_ACL_ENTRY_ATTR_FIELD_END)
        {
            return true;
        }

        if (metadata->attrid >= SAI_ACL_ENTRY_ATTR_ACTION_START &&
                metadata->attrid <= SAI_ACL_ENTRY_ATTR_ACTION_END)
        {
            return true;
        }

        if (metadata->isextensionattr)
        {
            return true;
        }
    }

    return false;
}

void check_attr_flags(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    switch ((int)md->flags)
    {
        case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY | SAI_ATTR_FLAGS_KEY:
        case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY:
        case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_AND_SET:

            if (md->validonlytype != SAI_ATTR_CONDITION_TYPE_NONE)
            {
                META_MD_ASSERT_FAIL(md, "valid only attribute can't be mandatory on create, use condition");
            }

            /*
             * Currently we don't use RANGE type so this check can be disabled.
             * Range idea was introduces so attribute can be in specific range
             * of other MIN/MAX attributes. This is not supported yet.
             *
             * if (md->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT32 &&
             *         md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_ATTR_RANGE)
             * {
             *     break;
             * }
             */

            if (md->defaultvaluetype != SAI_DEFAULT_VALUE_TYPE_NONE)
            {
                META_MD_ASSERT_FAIL(md, "no default value expected, but type provided: %s",
                        sai_metadata_get_default_value_type_name(md->defaultvaluetype));
            }

            break;

        case SAI_ATTR_FLAGS_CREATE_ONLY:
        case SAI_ATTR_FLAGS_CREATE_AND_SET:

            if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_NONE)
            {
                /*
                 * Default value of object id can be internal or NULL and it
                 * needs to be specified explicitly.
                 */

                /*
                 * Default value for pointer must be specified and must be NULL.
                 */

                /*
                 * Default value for acl field or action is provided and is
                 * disabled by default.
                 */

                META_MD_ASSERT_FAIL(md, "expected default value, but none provided");
            }

            break;

        case SAI_ATTR_FLAGS_READ_ONLY:

            if (md->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE)
            {
                META_MD_ASSERT_FAIL(md, "read only attribute can't be conditional");
            }

            if (md->validonlytype != SAI_ATTR_CONDITION_TYPE_NONE)
            {
                META_MD_ASSERT_FAIL(md, "read only attribute can't be valid only");
            }

            if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_SWITCH_INTERNAL)
            {
                if (md->attrvaluetype == SAI_ATTR_VALUE_TYPE_OBJECT_ID ||
                        md->attrvaluetype == SAI_ATTR_VALUE_TYPE_OBJECT_LIST)
                {
                    /*
                     * Read only object id/list can be marked as internal like
                     * default virtual router, cpu port id, default queues on
                     * ports, etc.
                     */

                    break;
                }
            }

            if (md->defaultvaluetype != SAI_DEFAULT_VALUE_TYPE_NONE)
            {
                META_MD_ASSERT_FAIL(md, "no default value expected, but type provided: %s",
                        sai_metadata_get_default_value_type_name(md->defaultvaluetype));
            }

            break;

        default:

            META_MD_ASSERT_FAIL(md, "invalid creation flags: 0x%x", md->flags);
    }

    META_ASSERT_TRUE(SAI_HAS_FLAG_MANDATORY_ON_CREATE(md->flags) == md->ismandatoryoncreate, "wrong ismandatoryoncreate");
    META_ASSERT_TRUE(SAI_HAS_FLAG_CREATE_ONLY(md->flags) == md->iscreateonly, "wrong iscreateonly");
    META_ASSERT_TRUE(SAI_HAS_FLAG_CREATE_AND_SET(md->flags) == md->iscreateandset, "wrong iscreateandset");
    META_ASSERT_TRUE(SAI_HAS_FLAG_READ_ONLY(md->flags) == md->isreadonly, "wrong isreadonly");
    META_ASSERT_TRUE(SAI_HAS_FLAG_KEY(md->flags) == md->iskey, "wrong iskey");
}

void check_attr_object_id_allownull(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->attrvaluetype != SAI_ATTR_VALUE_TYPE_OBJECT_ID)
    {
        /*
         * We don't care about ACL entry data/field, in that when we set NULL
         * as object id on acl data/field it should mean as disable, or it
         * should be not allowed at all.
         */

        return;
    }

    /* attribute is object id type */

    switch ((int)md->flags)
    {
        case SAI_ATTR_FLAGS_CREATE_ONLY:
        case SAI_ATTR_FLAGS_CREATE_AND_SET:

            /* default value is required */

            switch (md->defaultvaluetype)
            {
                case SAI_DEFAULT_VALUE_TYPE_CONST:

                    if (md->allownullobjectid == false)
                    {
                        /*
                         * since attribute require default value and default value is
                         * set to SAI_NULL_OBJECT_ID then allownull should be true
                         */

                        META_MD_ASSERT_FAIL(md, "allow null object id should be set to true since default value is required");
                    }

                    break;

                case SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE:
                    /* default attr value from another attr may not support null */
                    break;

                default:
                    META_MD_ASSERT_FAIL(md, "invalid default value type on object id when default is required");
                    break;
            }

            break;

        default:
            break;
    }
}

void check_attr_object_type_provided(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    switch (md->attrvaluetype)
    {
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:
        case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_OBJECT_ID:

            if (md->allowedobjecttypes == NULL)
            {
                META_MD_ASSERT_FAIL(md, "object types list is required but it's empty");
            }

            break;

        case SAI_ATTR_VALUE_TYPE_BOOL:
        case SAI_ATTR_VALUE_TYPE_INT8:
        case SAI_ATTR_VALUE_TYPE_INT32:
        case SAI_ATTR_VALUE_TYPE_INT8_LIST:
        case SAI_ATTR_VALUE_TYPE_UINT8_LIST:
        case SAI_ATTR_VALUE_TYPE_UINT16_LIST:
        case SAI_ATTR_VALUE_TYPE_INT32_LIST:
        case SAI_ATTR_VALUE_TYPE_UINT8:
        case SAI_ATTR_VALUE_TYPE_UINT16:
        case SAI_ATTR_VALUE_TYPE_VLAN_LIST:
        case SAI_ATTR_VALUE_TYPE_UINT32:
        case SAI_ATTR_VALUE_TYPE_UINT64:
        case SAI_ATTR_VALUE_TYPE_MAC:
        case SAI_ATTR_VALUE_TYPE_POINTER:
        case SAI_ATTR_VALUE_TYPE_IP_ADDRESS:
        case SAI_ATTR_VALUE_TYPE_IP_PREFIX:
        case SAI_ATTR_VALUE_TYPE_PRBS_RX_STATE:
        case SAI_ATTR_VALUE_TYPE_CHARDATA:
        case SAI_ATTR_VALUE_TYPE_UINT32_RANGE:
        case SAI_ATTR_VALUE_TYPE_UINT16_RANGE_LIST:
        case SAI_ATTR_VALUE_TYPE_UINT32_LIST:
        case SAI_ATTR_VALUE_TYPE_QOS_MAP_LIST:
        case SAI_ATTR_VALUE_TYPE_MAP_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_CAPABILITY:
        case SAI_ATTR_VALUE_TYPE_ACL_RESOURCE_LIST:
        case SAI_ATTR_VALUE_TYPE_TLV_LIST:
        case SAI_ATTR_VALUE_TYPE_SEGMENT_LIST:
        case SAI_ATTR_VALUE_TYPE_IP_ADDRESS_LIST:
        case SAI_ATTR_VALUE_TYPE_PORT_EYE_VALUES_LIST:
        case SAI_ATTR_VALUE_TYPE_TIMESPEC:

        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_BOOL:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT64:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST:

        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_BOOL:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IP_ADDRESS:
        case SAI_ATTR_VALUE_TYPE_IPV4:
        case SAI_ATTR_VALUE_TYPE_IPV6:

        case SAI_ATTR_VALUE_TYPE_ENCRYPT_KEY:
        case SAI_ATTR_VALUE_TYPE_AUTH_KEY:

        case SAI_ATTR_VALUE_TYPE_MACSEC_SAK:
        case SAI_ATTR_VALUE_TYPE_MACSEC_AUTH_KEY:
        case SAI_ATTR_VALUE_TYPE_MACSEC_SALT:

        case SAI_ATTR_VALUE_TYPE_SYSTEM_PORT_CONFIG:
        case SAI_ATTR_VALUE_TYPE_SYSTEM_PORT_CONFIG_LIST:
        case SAI_ATTR_VALUE_TYPE_FABRIC_PORT_REACHABILITY:
        case SAI_ATTR_VALUE_TYPE_PORT_ERR_STATUS_LIST:

            if (md->allowedobjecttypes != NULL)
            {
                META_MD_ASSERT_FAIL(md, "allowed object types defined for non object type");
            }

            break;

        default:
            META_MD_ASSERT_FAIL(md, "attr value type is not supported, FIXME");
    }
}

void check_attr_allowed_object_types(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->allowedobjecttypeslength != 0 && md->allowedobjecttypes == NULL)
    {
        META_MD_ASSERT_FAIL(md, "allowed object type len is specified but pointer is NULL");
    }

    if (md->allowedobjecttypeslength == 0 && md->allowedobjecttypes != NULL)
    {
        META_MD_ASSERT_FAIL(md, "allowed object type len zero, but but pointer to objects is specified");
    }

    if (md->allowedobjecttypes == NULL)
    {
        return;
    }

    if (md->isoidattribute)
    {
        META_ASSERT_TRUE(md->allowedobjecttypeslength > 0, "object len should be at least 1");
    }
    else
    {
        META_ASSERT_TRUE(md->allowedobjecttypeslength == 0, "object len should be 0");
    }

    switch (md->attrvaluetype)
    {
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:
        case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_OBJECT_ID:
            break;

        default:

            META_MD_ASSERT_FAIL(md, "allowed object types should be empty on this attr value type");
    }

    /*
     * check if allowed object types are in range
     * they may repeat, but we can also check that
     */

    size_t i = 0;

    for (; i < md->allowedobjecttypeslength; ++i)
    {
        sai_object_type_t ot = md->allowedobjecttypes[i];

        if (!is_valid_object_type(ot))
        {
            META_MD_ASSERT_FAIL(md, "invalid allowed object type: %d", ot);
        }

        const sai_object_type_info_t* info = sai_metadata_all_object_type_infos[ot];

        META_ASSERT_NOT_NULL(info);

        if (info->isnonobjectid)
        {
            META_MD_ASSERT_FAIL(md, "non object id can't be used as object id: %d", ot);
        }

        if (ot == SAI_OBJECT_TYPE_SWITCH ||
                ot == SAI_OBJECT_TYPE_FDB_FLUSH ||
                ot == SAI_OBJECT_TYPE_HOSTIF_PACKET)
        {
            /* switch object type is meant to be used only in non object id struct types */

            META_MD_ASSERT_FAIL(md, "switch object type can't be used as object type in any attribute");
        }
    }
}

void check_attr_default_required(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    bool requiredefault = (!SAI_HAS_FLAG_MANDATORY_ON_CREATE(md->flags)) &&
        (SAI_HAS_FLAG_CREATE_ONLY(md->flags) || SAI_HAS_FLAG_CREATE_AND_SET(md->flags));

    if (requiredefault == false)
    {
        return;
    }

    if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_NONE)
    {
        /*
         * If default value type is NONE, then default value must be NULL.
         */

        META_ASSERT_NULL(md->defaultvalue);

        /*
         * By default we assume that default acl field or action is
         * disabled and default value is not provided.
         */

        META_MD_ASSERT_FAIL(md, "expected default value, but none provided");
    }

    switch (md->defaultvaluetype)
    {
        case SAI_DEFAULT_VALUE_TYPE_CONST:

            if (md->objecttype == SAI_OBJECT_TYPE_UDF && md->attrid == SAI_UDF_ATTR_HASH_MASK)
            {
                if (md->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT8_LIST)
                {
                    /*
                     * Const on list, this is exception for UDF object since
                     * it's default value is 2 bytes 0xFF,0xFF and it's special
                     * case.
                     */

                    break;
                }
            }

            if ((md->objecttype == SAI_OBJECT_TYPE_PORT) || (md->objecttype == SAI_OBJECT_TYPE_PORT_SERDES))
            {
                /*
                 * Allow PORT non object list attributes to be set to internal switch values.
                 */
                break;
            }

            if (md->defaultvalue == NULL)
            {
                META_MD_ASSERT_FAIL(md, "default value type is provided, but default value pointer is NULL");
            }

            break;

        case SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE:
        case SAI_DEFAULT_VALUE_TYPE_ATTR_RANGE:
        case SAI_DEFAULT_VALUE_TYPE_SWITCH_INTERNAL:
        case SAI_DEFAULT_VALUE_TYPE_VENDOR_SPECIFIC:
        case SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST:

            if (md->defaultvalue != NULL)
            {
                META_MD_ASSERT_FAIL(md, "default value type is provided, but default value pointer is not NULL");
            }

            break;

        default:

            META_MD_ASSERT_FAIL(md, "unknown default value type %d", md->defaultvaluetype);
    }

    /* default value is required */

    switch (md->attrvaluetype)
    {
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_BOOL:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT64:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID:

        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_BOOL:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IP_ADDRESS:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:

            if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_CONST)
            {
                break;
            }

            META_MD_ASSERT_FAIL(md, "default value on acl field/action must be const/disabled");

        case SAI_ATTR_VALUE_TYPE_OBJECT_ID:
        case SAI_ATTR_VALUE_TYPE_BOOL:
        case SAI_ATTR_VALUE_TYPE_INT32:
        case SAI_ATTR_VALUE_TYPE_UINT8:
        case SAI_ATTR_VALUE_TYPE_UINT16:
        case SAI_ATTR_VALUE_TYPE_UINT32:
        case SAI_ATTR_VALUE_TYPE_UINT64:
        case SAI_ATTR_VALUE_TYPE_MAC:
        case SAI_ATTR_VALUE_TYPE_IP_ADDRESS:
        case SAI_ATTR_VALUE_TYPE_IP_PREFIX:
        case SAI_ATTR_VALUE_TYPE_PRBS_RX_STATE:
        case SAI_ATTR_VALUE_TYPE_TIMESPEC:
        case SAI_ATTR_VALUE_TYPE_IPV4:
        case SAI_ATTR_VALUE_TYPE_SYSTEM_PORT_CONFIG:
        case SAI_ATTR_VALUE_TYPE_IPV6:
            break;

        case SAI_ATTR_VALUE_TYPE_CHARDATA:

            if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_CONST)
            {
                break;
            }

            META_MD_ASSERT_FAIL(md, "default value on chardata const");

        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST:

            /* even if this is list, on acl field/action we require disabled */

            if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_CONST)
            {
                break;
            }

            META_MD_ASSERT_FAIL(md, "default value on acl field/action must be const/disabled");

        case SAI_ATTR_VALUE_TYPE_INT8_LIST:
        case SAI_ATTR_VALUE_TYPE_UINT32_LIST:
        case SAI_ATTR_VALUE_TYPE_INT32_LIST:
        case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_TLV_LIST:
        case SAI_ATTR_VALUE_TYPE_SEGMENT_LIST:
        case SAI_ATTR_VALUE_TYPE_MAP_LIST:
        case SAI_ATTR_VALUE_TYPE_IP_ADDRESS_LIST:
        case SAI_ATTR_VALUE_TYPE_PORT_EYE_VALUES_LIST:
        case SAI_ATTR_VALUE_TYPE_SYSTEM_PORT_CONFIG_LIST:

            if (((md->objecttype == SAI_OBJECT_TYPE_PORT) || (md->objecttype == SAI_OBJECT_TYPE_PORT_SERDES))
                 && md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_SWITCH_INTERNAL)
            {
                /*
                 * Allow non object lists on PORT to be set to internal default value.
                 */
                break;
            }

            if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST)
            {
                break;
            }

            META_MD_ASSERT_FAIL(md, "default value list is needed on this attr value type but list is NULL");

        case SAI_ATTR_VALUE_TYPE_UINT8_LIST:

            if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST)
            {
                break;
            }

            if (md->objecttype == SAI_OBJECT_TYPE_UDF && md->attrid == SAI_UDF_ATTR_HASH_MASK)
            {
                if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_CONST)
                {
                    /*
                     * Again this is exception only for this one UDF attribute
                     * to support CONST on list.
                     */

                    break;
                }
            }

            META_MD_ASSERT_FAIL(md, "default value list is needed on this attr value type but list is NULL");

        case SAI_ATTR_VALUE_TYPE_UINT16_LIST:

            if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST)
            {
                break;
            }

            META_MD_ASSERT_FAIL(md, "default value list is needed on this attr value type but list is NULL");

        case SAI_ATTR_VALUE_TYPE_POINTER:

            /*
             * Gearbox exception for mandatory pointer attribute
             * to support CONST on list.
             */

            break;

        default:

            META_MD_ASSERT_FAIL(md, "default value is required but this attr value type is not supported yet");
    }
}

void check_attr_enums(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->isenum)
    {
        switch (md->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_INT32:
            case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32:
            case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32:
                break;

            default:
                META_MD_ASSERT_FAIL(md, "attribute is marked as enum, but attr value type is not enum compatible");
        }
    }

    if (md->isenum && md->isenumlist)
    {
        META_MD_ASSERT_FAIL(md, "attribute can't be marked as enum and enum list");
    }

    if ((md->isenum || md->isenumlist) && md->enummetadata == NULL)
    {
        META_MD_ASSERT_FAIL(md, "is marked enum but missing enum metadata");
    }

    if (!(md->isenum || md->isenumlist) && md->enummetadata != NULL)
    {
        META_MD_ASSERT_FAIL(md, "is not marked enum but has defined enum type string");
    }

    if ((md->isenum || md->isenumlist) && md->enummetadata->valuescount == 0)
    {
        META_MD_ASSERT_FAIL(md, "is marked enum but missing enum allowed values");
    }

    bool requiredefault = (!SAI_HAS_FLAG_MANDATORY_ON_CREATE(md->flags)) &&
        (SAI_HAS_FLAG_CREATE_ONLY(md->flags) || SAI_HAS_FLAG_CREATE_AND_SET(md->flags));

    if (requiredefault && md->isenum)
    {
        if (md->defaultvalue == NULL)
        {
            META_MD_ASSERT_FAIL(md, "marked as enum, and require default, but not provided");
        }

        if (sai_metadata_is_acl_field_or_action(md))
        {
            /*
             * Default value for acl action is disabled, so enum value can't be
             * compared since it's not there.
             */

            return;
        }

        int32_t enumdefault = md->defaultvalue->s32;

        if (sai_metadata_get_enum_value_name(md->enummetadata, enumdefault) == NULL)
        {
            META_MD_ASSERT_FAIL(md, "default enum value %d is not present on enum allowed values (%s)", enumdefault, md->enummetadata->name);
        }
    }

    if (requiredefault && md->isenumlist)
    {
        if (md->defaultvalue != NULL)
        {
            META_MD_ASSERT_FAIL(md, "default values on enum list are not supported yet");
        }
    }
}

void check_attr_default_value_type(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    switch (md->defaultvaluetype)
    {
        case SAI_DEFAULT_VALUE_TYPE_NONE:
        case SAI_DEFAULT_VALUE_TYPE_CONST:

            /* check conditions/creation flags? */
            break;

        case SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE:

            {
                const sai_attr_metadata_t* def = sai_metadata_get_attr_metadata(md->defaultvalueobjecttype, md->defaultvalueattrid);

                if (def == NULL)
                {
                    META_MD_ASSERT_FAIL(md, "attr value can't be found");
                }

                if (md->attrvaluetype != def->attrvaluetype)
                {
                    META_MD_ASSERT_FAIL(md, "default attr value type is different");
                }

                break;
            }

        case SAI_DEFAULT_VALUE_TYPE_ATTR_RANGE:

            {
                const sai_attr_metadata_t* def = sai_metadata_get_attr_metadata(md->defaultvalueobjecttype, md->defaultvalueattrid);

                if (def == NULL)
                {
                    META_MD_ASSERT_FAIL(md, "attr range can't be found");
                }

                META_MD_ASSERT_FAIL(md, "attr value attribute value range not supported yet");

                break;
            }

        case SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST:

            switch (md->attrvaluetype)
            {
                case SAI_ATTR_VALUE_TYPE_UINT32_LIST:
                case SAI_ATTR_VALUE_TYPE_INT32_LIST:
                case SAI_ATTR_VALUE_TYPE_UINT8_LIST:
                case SAI_ATTR_VALUE_TYPE_UINT16_LIST:
                case SAI_ATTR_VALUE_TYPE_INT8_LIST:
                case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
                case SAI_ATTR_VALUE_TYPE_TLV_LIST:
                case SAI_ATTR_VALUE_TYPE_SEGMENT_LIST:
                case SAI_ATTR_VALUE_TYPE_MAP_LIST:
                case SAI_ATTR_VALUE_TYPE_IP_ADDRESS_LIST:
                case SAI_ATTR_VALUE_TYPE_PORT_EYE_VALUES_LIST:
                case SAI_ATTR_VALUE_TYPE_SYSTEM_PORT_CONFIG_LIST:
                    break;

                default:

                    META_MD_ASSERT_FAIL(md, "default empty list specified, but attribute is not list");
            }

            break;

        case SAI_DEFAULT_VALUE_TYPE_VENDOR_SPECIFIC:

            switch (md->attrvaluetype)
            {
                case SAI_ATTR_VALUE_TYPE_MAC:
                case SAI_ATTR_VALUE_TYPE_UINT64:
                    break;

                default:

                    /*
                     * Vendor specific attribute should be used only on
                     * primitive types and not on object id types (OIDs).
                     */

                    META_MD_ASSERT_FAIL(md, "vendor specific not allowed on this type");
            }

            break;

        case SAI_DEFAULT_VALUE_TYPE_SWITCH_INTERNAL:

            if ((md->objecttype == SAI_OBJECT_TYPE_PORT) ||
                (md->objecttype == SAI_OBJECT_TYPE_PORT_SERDES) ||
                (md->objecttype == SAI_OBJECT_TYPE_NEIGHBOR_ENTRY))
            {
                /*
                 * Allow PORT, NEIGHBOR attribute list's to be set to internal.
                 */
                break;
            }

            if (md->flags != SAI_ATTR_FLAGS_READ_ONLY)
            {
                META_MD_ASSERT_FAIL(md, "default internal currently can be set only on read only objects");
            }

            if (md->objecttype != SAI_OBJECT_TYPE_SWITCH)
            {
                /*
                 * This can be later relaxed to be set on ports since they have
                 * by default queues created.
                 */

                META_MD_ASSERT_FAIL(md, "default internal can be only set on switch object type");
            }

            switch (md->attrvaluetype)
            {
                case SAI_ATTR_VALUE_TYPE_OBJECT_ID:
                case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
                    break;

                default:

                    META_MD_ASSERT_FAIL(md, "invalid attribute value type specified: %d", md->attrvaluetype);
            }

            break;

        default:

            META_MD_ASSERT_FAIL(md, "invalid default value type specified: %d", md->defaultvaluetype);
    }
}

void check_attr_conditions(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    switch (md->conditiontype)
    {
        case SAI_ATTR_CONDITION_TYPE_NONE:
        case SAI_ATTR_CONDITION_TYPE_OR:
        case SAI_ATTR_CONDITION_TYPE_AND:
        case SAI_ATTR_CONDITION_TYPE_MIXED:
            break;

        default:

            META_MD_ASSERT_FAIL(md, "invalid condition type specified: %d", md->conditiontype);
    }

    bool conditional = md->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE;

    if (!conditional && md->conditions != NULL)
    {
        META_MD_ASSERT_FAIL(md, "not conditional but conditions specified");
    }

    if (!conditional)
    {
        META_ASSERT_FALSE(md->isconditional, "marked conditional but is not");
        return;
    }

    META_ASSERT_TRUE(md->isconditional, "marked not conditional but is");

    if (md->conditions == NULL)
    {
        META_MD_ASSERT_FAIL(md, "marked as conditional but no conditions specified");
    }

    switch ((int)md->flags)
    {
        case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_AND_SET:
        case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY:

            /*
             * If attribute is marked as conditional then it must have flags
             * mandatory on create, otherwise use validonly condition.
             */

            break;

        default:

            META_MD_ASSERT_FAIL(md, "marked as conditional, but invalid creation flags: 0x%x", md->flags);
    }

    /* condition must be the same object type as attribute we check */

    size_t index = 0;

    for (; index < md->conditionslength; ++index)
    {
        const sai_attr_condition_t* c = md->conditions[index];

        if (c->attrid == md->attrid)
        {
            META_MD_ASSERT_FAIL(md, "conditional attr id %d is the same as condition attribute", c->attrid);
        }

        if (md->conditiontype == SAI_ATTR_CONDITION_TYPE_MIXED && c->attrid == SAI_INVALID_ATTRIBUTE_ID)
        {
            switch (c->type)
            {
                case SAI_ATTR_CONDITION_TYPE_OR:
                case SAI_ATTR_CONDITION_TYPE_AND:
                    break;

                default:

                    META_MD_ASSERT_FAIL(md, "conditionwrong sub condition type: %d (expected AND/OR)", c->type);
            }

            continue;
        }

        const sai_attr_metadata_t* cmd = sai_metadata_get_attr_metadata(md->objecttype, c->attrid);

        if (cmd == NULL)
        {
            META_MD_ASSERT_FAIL(md, "conditional attribute id %d was not defined yet in metadata", c->attrid);
        }

        switch (cmd->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_BOOL:

                META_LOG_DEBUG("attr id: %d cond.bool: %d", c->attrid, c->condition.booldata);

                break;

            case SAI_ATTR_VALUE_TYPE_INT32:

                /*
                 * Currently force conditional int32 attributes to be enum.
                 * This can be relaxed later when needed.
                 */

                if (!cmd->isenum)
                {
                    META_MD_ASSERT_FAIL(md, "conditional attribute %s is not enum type", cmd->attridname);
                }

                if (cmd->isenum)
                {
                    /* condition value can be a number or enum */

                    META_LOG_DEBUG("attr id: %d cond.s32: %d ", c->attrid, c->condition.s32);

                    /* check if condition enum is in condition attribute range */

                    if (sai_metadata_get_enum_value_name(cmd->enummetadata, c->condition.s32) == NULL)
                    {
                        META_MD_ASSERT_FAIL(md, "condition enum %d not found on condition attribute enum range", c->condition.s32);
                    }
                }

                break;

            case SAI_ATTR_VALUE_TYPE_INT8:
            case SAI_ATTR_VALUE_TYPE_INT16:
            case SAI_ATTR_VALUE_TYPE_INT64:
            case SAI_ATTR_VALUE_TYPE_UINT8:
            case SAI_ATTR_VALUE_TYPE_UINT16:
            case SAI_ATTR_VALUE_TYPE_UINT32:
            case SAI_ATTR_VALUE_TYPE_UINT64:

                /* number conditions */

                break;

            default:

                META_MD_ASSERT_FAIL(md, "attr value type %d of conditional attribute is not supported yet", cmd->attrvaluetype);

        }

        if (cmd->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE)
        {
            META_MD_ASSERT_FAIL(md, "conditional attribute is also conditional, not allowed");
        }

        switch ((int)cmd->flags)
        {
            case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY | SAI_ATTR_FLAGS_KEY:
            case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY:
            case SAI_ATTR_FLAGS_CREATE_ONLY:

                /*
                 * Condition attribute must be create only since if it could
                 * change then other object may be required to pass on creation
                 * time that was not passed.
                 */

                break;

            default:

                META_MD_ASSERT_FAIL(cmd, "attribute must be create only since used in condition for %s", md->attridname);
        }
    }
}

void check_attr_validonly(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    switch (md->validonlytype)
    {
        case SAI_ATTR_CONDITION_TYPE_NONE:
        case SAI_ATTR_CONDITION_TYPE_OR:
        case SAI_ATTR_CONDITION_TYPE_AND:
        case SAI_ATTR_CONDITION_TYPE_MIXED:
            break;

        default:

            META_MD_ASSERT_FAIL(md, "invalid validonly type specified: %d", md->validonlytype);
    }

    bool conditional = md->validonlytype != SAI_ATTR_CONDITION_TYPE_NONE;

    if (!conditional && md->validonly != NULL)
    {
        META_MD_ASSERT_FAIL(md, "not validonly but validonly specified");
        META_ASSERT_FALSE(md->isvalidonly, "marked validonly but is not");
    }

    if (!conditional)
    {
        return;
    }

    if (md->validonly == NULL)
    {
        META_MD_ASSERT_FAIL(md, "marked as validonly but no validonly specified");
    }

    META_ASSERT_TRUE(md->isvalidonly, "marked not validonly but is");

    switch ((int)md->flags)
    {
        case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY | SAI_ATTR_FLAGS_KEY:
        case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY:
        case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_AND_SET:

            META_MD_ASSERT_FAIL(md, "valid only attribute can't be mandatory on create, use condition");
            break;

        case SAI_ATTR_FLAGS_CREATE_ONLY:

            /*
             * In general valid only attribute should be used only on
             * CREATE_AND_SET flags, since when attribute is CREATE_ONLY it has
             * default value and it can't be changed anyway, and entire purpose
             * of valid only attribute is to allow change during runtime.
             *
             * When attribute CREATE_ONLY is marked as valid only is more like
             * indication that this value will be used in that specific case
             * but you won't be able to change it anyway.
             */

            META_MD_LOG_WARN(md, "marked as valid only, on flags CREATE_ONLY, default value is present, should this be CREATE_AND_SET?");

            /* intentional fall through */

        case SAI_ATTR_FLAGS_CREATE_AND_SET:

            /* ok */

            break;

        case SAI_ATTR_FLAGS_READ_ONLY:

            META_MD_ASSERT_FAIL(md, "read only attribute can't be valid only");
            break;

        default:

            META_MD_ASSERT_FAIL(md, "marked as validonly, but invalid creation flags: 0x%x", md->flags);
    }

    if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_NONE)
    {
        /*
         * In struct defaultvalue member can be NULL for some other default
         * value types like empty list or internal etc. Default value is
         * provided for CONST only.
         */

        META_MD_ASSERT_FAIL(md, "expected default value on valid only attribute, but none provided");
    }

    /* condition must be the same object type as attribute we check */

    size_t index = 0;

    for (; index < md->validonlylength; ++index)
    {
        const sai_attr_condition_t* c = md->validonly[index];

        if (c->attrid == md->attrid)
        {
            META_MD_ASSERT_FAIL(md, "validonly attr id %d is the same as validonly attribute", c->attrid);
        }

        if (md->validonlytype == SAI_ATTR_CONDITION_TYPE_MIXED && c->attrid == SAI_INVALID_ATTRIBUTE_ID)
        {
            switch (c->type)
            {
                case SAI_ATTR_CONDITION_TYPE_OR:
                case SAI_ATTR_CONDITION_TYPE_AND:
                    break;

                default:

                    META_MD_ASSERT_FAIL(md, "validonly wrong sub condition type: %d (expected AND/OR)", c->type);
            }

            continue;
        }

        const sai_attr_metadata_t* cmd = sai_metadata_get_attr_metadata(md->objecttype, c->attrid);

        if (cmd == NULL)
        {
            META_MD_ASSERT_FAIL(md, "validonly attribute id %d was not defined yet in metadata", c->attrid);
        }

        switch (cmd->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_BOOL:

                META_LOG_DEBUG("attr id: %d cond.bool: %d", c->attrid, c->condition.booldata);

                break;

            case SAI_ATTR_VALUE_TYPE_INT32:

                /*
                 * Currently force conditional int32 attributes to be enum.
                 * This can be relaxed later when needed.
                 */

                if (!cmd->isenum)
                {
                    META_MD_ASSERT_FAIL(md, "validonly attribute %s is not enum type", cmd->attridname);
                }

                if (cmd->isenum)
                {
                    /* condition value can be a number or enum */

                    META_LOG_DEBUG("attr id: %d cond.s32: %d ", c->attrid, c->condition.s32);

                    /* check if condition enum is in condition attribute range */

                    if (sai_metadata_get_enum_value_name(cmd->enummetadata, c->condition.s32) == NULL)
                    {
                        META_MD_ASSERT_FAIL(md, "validonly enum %d not found on validonly attribute enum range", c->condition.s32);
                    }
                }

                break;

            case SAI_ATTR_VALUE_TYPE_INT8:
            case SAI_ATTR_VALUE_TYPE_INT16:
            case SAI_ATTR_VALUE_TYPE_INT64:
            case SAI_ATTR_VALUE_TYPE_UINT8:
            case SAI_ATTR_VALUE_TYPE_UINT16:
            case SAI_ATTR_VALUE_TYPE_UINT32:
            case SAI_ATTR_VALUE_TYPE_UINT64:
                break;

            default:

                META_MD_ASSERT_FAIL(md, "attr value type %d of validonly attribute is not supported yet", cmd->attrvaluetype);
        }

        /*
         * TODO can validonly attribute depend on condition attribute which is not provided?
         * TODO can validonly depend on other validonly?
         */

        if (cmd->validonlytype != SAI_ATTR_CONDITION_TYPE_NONE)
        {
            if (md->objecttype == SAI_OBJECT_TYPE_MIRROR_SESSION &&
                    (md->attrid == SAI_MIRROR_SESSION_ATTR_VLAN_TPID || md->attrid == SAI_MIRROR_SESSION_ATTR_VLAN_ID ||
                     md->attrid == SAI_MIRROR_SESSION_ATTR_VLAN_PRI || md->attrid == SAI_MIRROR_SESSION_ATTR_VLAN_CFI))
            {
                /*
                 * Vlan header attributes are depending on VLAN_HEADER_VALID which is
                 * also valid only for ERSPAN.
                 */
            }
            else if (md->objecttype == SAI_OBJECT_TYPE_NEXT_HOP &&
                    (md->attrid == SAI_NEXT_HOP_ATTR_OUTSEG_TTL_MODE || md->attrid == SAI_NEXT_HOP_ATTR_OUTSEG_TTL_VALUE ||
                     md->attrid == SAI_NEXT_HOP_ATTR_OUTSEG_EXP_MODE || md->attrid == SAI_NEXT_HOP_ATTR_OUTSEG_EXP_VALUE ||
                     md->attrid == SAI_NEXT_HOP_ATTR_QOS_TC_AND_COLOR_TO_MPLS_EXP_MAP))
            {
                /*
                 * MPLS out segment attributes are required for ingress node and valid only for MPLS next hop.
                 */
            }
            else
            {
                META_MD_ASSERT_FAIL(md, "validonly attribute is also validonly attribute, not allowed");
            }
        }

        if (cmd->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE)
        {
            META_MD_ASSERT_FAIL(md, "conditional attribute is also conditional, not allowed");
        }

        switch ((int)cmd->flags)
        {
            case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY | SAI_ATTR_FLAGS_KEY:
            case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY:
            case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_AND_SET:
            case SAI_ATTR_FLAGS_CREATE_ONLY:
            case SAI_ATTR_FLAGS_CREATE_AND_SET:

                /*
                 * Valid only attribute can be create_only or create_and_set
                 * conditional attribute can change during runtime and it may
                 * have impact on valid only attribute (it may or may not be
                 * used).
                 */

                break;

            default:

                META_MD_ASSERT_FAIL(cmd, "valid only condition attribute has invalid flags");
        }
    }

    if ((md->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE ) &&
            (md->validonlytype != SAI_ATTR_CONDITION_TYPE_NONE ))
    {
        META_MD_ASSERT_FAIL(md, "attribute is conditional and valid only, not supported");
    }
}

void check_attr_enum_list_condition(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->isenumlist)
    {
        if (md->attrvaluetype != SAI_ATTR_VALUE_TYPE_INT32_LIST)
        {
            META_MD_ASSERT_FAIL(md, "marked as enum list but wrong attr value type");
        }

        if (md->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE)
        {
            META_MD_ASSERT_FAIL(md, "conditional enum list not supported yet");
        }
    }
}

void check_attr_enum_list_validonly(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->isenumlist)
    {
        if (md->attrvaluetype != SAI_ATTR_VALUE_TYPE_INT32_LIST)
        {
            META_MD_ASSERT_FAIL(md, "marked as enum list but wrong attr value type");
        }
    }
}

void check_attr_allow_flags(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->allownullobjectid)
    {
        switch (md->attrvaluetype)
        {
            /* there may be other types in acl field/data that accept object id */

            case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID:
            case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:
            case SAI_ATTR_VALUE_TYPE_OBJECT_ID:

                break;

            default:

                META_MD_ASSERT_FAIL(md, "allow null object is set but attr value type is wrong");
        }

        /*
         * Object SAI_ATTR_VALUE_TYPE_POINTER should be allowed null pointer by
         * default pointers received from SAI should be only via query api.
         */

        if (md->allowedobjecttypeslength == 0)
        {
            META_MD_ASSERT_FAIL(md, "allow null object is set but allowed object types is empty");
        }
    }

    if (md->allowedobjecttypeslength != 0)
    {
        META_ASSERT_NOT_NULL(md->allowedobjecttypes);
    }

    size_t index = 0;

    for (; index < md->allowedobjecttypeslength; ++index)
    {
        sai_object_type_t ot = md->allowedobjecttypes[index];

        if (is_valid_object_type(ot))
        {
            continue;
        }

        META_MD_ASSERT_FAIL(md, "not allowed object type %d on list", ot);
    }

    /* allow empty list can point to any list, not only object id list */

    if (md->allowemptylist)
    {
        switch (md->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
                break;

            case SAI_ATTR_VALUE_TYPE_INT8_LIST:
            case SAI_ATTR_VALUE_TYPE_UINT8_LIST:
            case SAI_ATTR_VALUE_TYPE_UINT16_LIST:
            case SAI_ATTR_VALUE_TYPE_INT32_LIST:
            case SAI_ATTR_VALUE_TYPE_VLAN_LIST:
            case SAI_ATTR_VALUE_TYPE_UINT32_LIST:
            case SAI_ATTR_VALUE_TYPE_QOS_MAP_LIST:
            case SAI_ATTR_VALUE_TYPE_MAP_LIST:
            case SAI_ATTR_VALUE_TYPE_ACL_RESOURCE_LIST:
            case SAI_ATTR_VALUE_TYPE_TLV_LIST:
            case SAI_ATTR_VALUE_TYPE_SEGMENT_LIST:
            case SAI_ATTR_VALUE_TYPE_IP_ADDRESS_LIST:
            case SAI_ATTR_VALUE_TYPE_PORT_EYE_VALUES_LIST:
            case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST:
            case SAI_ATTR_VALUE_TYPE_SYSTEM_PORT_CONFIG_LIST:
            case SAI_ATTR_VALUE_TYPE_PORT_ERR_STATUS_LIST:
                break;

            default:

                META_MD_ASSERT_FAIL(md, "allow empty list is set but attr value type is not list");
        }
    }

    if (md->allowrepetitiononlist || md->allowmixedobjecttypes)
    {
        switch (md->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
                break;

            default:

                META_MD_ASSERT_FAIL(md, "allow null object is set but attr value type is wrong");
        }
    }
}

void check_attr_get_save(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->getsave)
    {
        switch (md->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_INT32:
                break;

            default:

                META_MD_ASSERT_FAIL(md, "get save not supported on %s", md->attridname);
        }
    }
}

void check_attr_key(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (SAI_HAS_FLAG_KEY(md->flags))
    {
        switch (md->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_UINT32_LIST:

                if (md->objecttype == SAI_OBJECT_TYPE_PORT && md->attrid == SAI_PORT_ATTR_HW_LANE_LIST)
                {
                    /*
                     * This is special case when HW_LANE_LIST is actual KEY for
                     * port, and it's more complicated because order don't
                     * matter and same lane can't be used on different port if
                     * some ports are splitted.
                     */

                    break;
                }

                META_MD_ASSERT_FAIL(md, "marked as key, but have invalid attr value type (list)");

            case SAI_ATTR_VALUE_TYPE_OBJECT_ID:

                if ((md->objecttype == SAI_OBJECT_TYPE_QUEUE && md->attrid == SAI_QUEUE_ATTR_PORT) ||
                    (md->objecttype == SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP && md->attrid == SAI_INGRESS_PRIORITY_GROUP_ATTR_PORT) ||
                    (md->objecttype == SAI_OBJECT_TYPE_PORT_CONNECTOR && md->attrid == SAI_PORT_CONNECTOR_ATTR_SYSTEM_SIDE_PORT_ID) ||
                    (md->objecttype == SAI_OBJECT_TYPE_PORT_CONNECTOR && md->attrid == SAI_PORT_CONNECTOR_ATTR_LINE_SIDE_PORT_ID))
                {
                    /*
                     * This is also special case, OBJECT_ID at should not be a
                     * KEY in any attribute, this is TODO action to get rid of
                     * this kind of dependency.
                     */

                    break;
                }

                META_MD_ASSERT_FAIL(md, "marked as key, but have invalid attr value type (object id)");

            case SAI_ATTR_VALUE_TYPE_INT32:
            case SAI_ATTR_VALUE_TYPE_UINT32:
            case SAI_ATTR_VALUE_TYPE_UINT8:
            case SAI_ATTR_VALUE_TYPE_UINT16:
                break;

            default:

                META_MD_ASSERT_FAIL(md, "marked as key, but have invalid attr value type");
        }
    }
}

void check_attr_acl_fields(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    switch (md->attrvaluetype)
    {
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT64:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST:

            if (md->objecttype == SAI_OBJECT_TYPE_ACL_ENTRY &&
                    md->attrid >= SAI_ACL_ENTRY_ATTR_FIELD_START &&
                    md->attrid  <= SAI_ACL_ENTRY_ATTR_FIELD_END)
            {
                break;

            }

            if (md->objecttype == SAI_OBJECT_TYPE_UDF_MATCH)
            {
                /*
                 * This is special case, object for UDF MATCH can use acl field
                 * attribute values since it's easier to maintain since this
                 * match also need a mask parameter. But restriction is that
                 * only primitive types can be used, no object id;
                 */

                switch (md->attrvaluetype)
                {
                    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8:
                    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8:
                    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16:
                    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16:
                    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32:
                    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32:
                    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT64:
                        break;

                    default:

                        META_MD_ASSERT_FAIL(md, "acl field data used on udf match can be only primitive type");
                }

                break;
            }

            if (md->objecttype == SAI_OBJECT_TYPE_DTEL &&
                    md->attrid == SAI_DTEL_ATTR_INT_L4_DSCP &&
                    md->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8)
            {
                break;
            }

            META_MD_ASSERT_FAIL(md, "acl field may only be set on acl field and udf match");

            break;

        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IP_ADDRESS:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:

            if (md->objecttype == SAI_OBJECT_TYPE_ACL_ENTRY && md->isextensionattr)
            {
                break;
            }

            if (md->objecttype != SAI_OBJECT_TYPE_ACL_ENTRY  ||
                    md->attrid < SAI_ACL_ENTRY_ATTR_ACTION_START  ||
                    md->attrid > SAI_ACL_ENTRY_ATTR_ACTION_END)
            {
                META_MD_ASSERT_FAIL(md, "acl action may only be set on acl action");
            }

            break;

        default:
            break;
    }

    if (md->objecttype == SAI_OBJECT_TYPE_ACL_ENTRY)
    {
        if (md->attrid >= SAI_ACL_ENTRY_ATTR_FIELD_START &&
                md->attrid <= SAI_ACL_ENTRY_ATTR_FIELD_END)
        {
            switch (md->attrvaluetype)
            {
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_BOOL:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT64:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST:
                    break;

                default:
                    META_MD_ASSERT_FAIL(md, "invalid attr value type for acl field");
            }
        }

        if (md->attrid >= SAI_ACL_ENTRY_ATTR_ACTION_START &&
                md->attrid <= SAI_ACL_ENTRY_ATTR_ACTION_END)
        {
            switch (md->attrvaluetype)
            {
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_BOOL:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IP_ADDRESS:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
                    break;

                default:
                    META_MD_ASSERT_FAIL(md, "invalid attr value type for acl action");
            }
        }
    }
}

void check_attr_vlan(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->isvlan)
    {
        if (md->attrvaluetype != SAI_ATTR_VALUE_TYPE_UINT16 &&
                md->attrvaluetype != SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16 &&
                md->attrvaluetype != SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16)
        {
            META_MD_ASSERT_FAIL(md, "marked as vlan, but has wrong attr value type");
        }
    }
}

void check_condition_in_range(
        _In_ const sai_attr_metadata_t* md,
        _In_ size_t length,
        _In_ const sai_attr_condition_t * const* const conditions,
        _In_ sai_attr_id_t start,
        _In_ sai_attr_id_t end)
{
    META_LOG_ENTER();

    size_t index = 0;

    for (; index < length; ++index)
    {
        const sai_attr_condition_t* c = conditions[index];

        if (c->attrid < start || c->attrid > end)
        {
            continue;
        }

        META_MD_ASSERT_FAIL(md, "has condition depending on acl field / action, not allowed");
    }
}

void check_attr_acl_conditions(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to find out if there is any condition in
     * attributes that depends on acl entry object field or actions since such
     * dependency has no sense.
     */

    if (md->objecttype == SAI_OBJECT_TYPE_ACL_TABLE)
    {
        check_condition_in_range(md, md->validonlylength, md->validonly,
                SAI_ACL_TABLE_ATTR_FIELD_START, SAI_ACL_TABLE_ATTR_FIELD_END);

        check_condition_in_range(md, md->conditionslength, md->conditions,
                SAI_ACL_TABLE_ATTR_FIELD_START, SAI_ACL_TABLE_ATTR_FIELD_END);

        if (md->attrid >= SAI_ACL_TABLE_ATTR_FIELD_START &&
                md->attrid >= SAI_ACL_TABLE_ATTR_FIELD_END)
        {
            if (md->conditionslength != 0 || md->validonlylength != 0)
            {
                META_MD_ASSERT_FAIL(md, "acl table field has conditions, not allowed");
            }
        }
    }

    if (md->objecttype == SAI_OBJECT_TYPE_ACL_ENTRY)
    {
        check_condition_in_range(md, md->validonlylength, md->validonly,
                SAI_ACL_ENTRY_ATTR_FIELD_START, SAI_ACL_ENTRY_ATTR_FIELD_END);

        check_condition_in_range(md, md->conditionslength, md->conditions,
                SAI_ACL_ENTRY_ATTR_FIELD_START, SAI_ACL_ENTRY_ATTR_FIELD_END);

        check_condition_in_range(md, md->validonlylength, md->validonly,
                SAI_ACL_ENTRY_ATTR_ACTION_START, SAI_ACL_ENTRY_ATTR_ACTION_END);

        check_condition_in_range(md, md->conditionslength, md->conditions,
                SAI_ACL_ENTRY_ATTR_ACTION_START, SAI_ACL_ENTRY_ATTR_ACTION_END);

        if (md->attrid >= SAI_ACL_ENTRY_ATTR_FIELD_START &&
                md->attrid >= SAI_ACL_ENTRY_ATTR_FIELD_END)
        {
            if (md->conditionslength != 0 || md->validonlylength != 0)
            {
                META_MD_ASSERT_FAIL(md, "acl entry field has conditions, not allowed");
            }
        }

        if (md->attrid >= SAI_ACL_ENTRY_ATTR_ACTION_START &&
                md->attrid >= SAI_ACL_ENTRY_ATTR_ACTION_END)
        {
            if (md->conditionslength != 0 || md->validonlylength != 0)
            {
                META_MD_ASSERT_FAIL(md, "acl entry action has conditions, not allowed");
            }
        }
    }
}

void check_attr_reverse_graph(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    /*
     * Purpose of this method is to check whether any defined attribute with is
     * object id type is defined in reverse graph correctly. Read only objects
     * are also included.
     */

    size_t index = 0;

    for (; index < md->allowedobjecttypeslength; index++)
    {
        /*
         * for each defined object id on that list we need to check
         * if its defined correctly in reverse graph
         */

        sai_object_type_t depobjecttype = md->allowedobjecttypes[index];

        const sai_object_type_info_t *oi = sai_metadata_all_object_type_infos[depobjecttype];

        META_ASSERT_NOT_NULL(oi->revgraphmembers);

        size_t revidx = 0;

        bool defined = false;

        for (; oi->revgraphmembers[revidx] != NULL; revidx++)
        {
            /*
             * now let's search for graph member which defines
             * this object and the same attribute value
             */

            const sai_rev_graph_member_t *rm = oi->revgraphmembers[revidx];

            META_ASSERT_TRUE(rm->objecttype == depobjecttype, "invalid objecttype definition");

            if (rm->depobjecttype != md->objecttype)
            {
                /*
                 * this is not the member we are looking for
                 */

                continue;
            }

            if (rm->attrmetadata == NULL)
            {
                META_ASSERT_NOT_NULL(rm->structmember);

                /*
                 * object is defined on non object id,
                 * this will require different method to check
                 */

                META_MD_ASSERT_FAIL(md, "This is attribute, it can't be defined in struct member");
            }
            else
            {
                /*
                 * object is attribute
                 */

                META_ASSERT_NOT_NULL(rm->attrmetadata);
                META_ASSERT_NULL(rm->structmember);

                size_t i = 0;

                for (; i < rm->attrmetadata->allowedobjecttypeslength; i++)
                {
                    /*
                     * Object type of graph member must match and also
                     * attribute id must match.
                     */

                    if (rm->attrmetadata->allowedobjecttypes[i] == depobjecttype &&
                            rm->attrmetadata->attrid == md->attrid)
                    {
                        META_LOG_DEBUG("dep %s ot %s attr %s\n",
                                sai_metadata_enum_sai_object_type_t.valuesnames[depobjecttype],
                                sai_metadata_enum_sai_object_type_t.valuesnames[md->objecttype],
                                md->attridname);

                        defined = true;
                        break;
                    }
                }

                if (defined)
                {
                    break;
                }
            }
        }

        META_ASSERT_TRUE(defined, "reverse graph object is not defined anywhere");
    }
}

void check_if_attr_was_already_defined(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    const defined_attr_t *p = defined_attributes;

    while (p)
    {
        if (p->metadata != NULL)
        {
            if (p->metadata->objecttype == md->objecttype &&
                    p->metadata->attrid == md->attrid)
            {
                META_MD_ASSERT_FAIL(md, "attribute was already declared");
            }
        }

        p = p->next;
    }
}

void check_attr_acl_capability(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->attrvaluetype != SAI_ATTR_VALUE_TYPE_ACL_CAPABILITY)
    {
        return;
    }

    if (md->flags != SAI_ATTR_FLAGS_READ_ONLY)
    {
        META_MD_ASSERT_FAIL(md, "attribute marked as acl capability should be READ_ONLY");
    }
}

void define_attr(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    defined_attr_t *p = (defined_attr_t*)malloc(sizeof(defined_attr_t));

    p->metadata = md;
    p->next = defined_attributes;

    defined_attributes = p;
}

void check_attr_acl_field_or_action(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    /*
     * Purpose of this test is to check if respective flags are set to true on
     * acl field or action.
     */

    if ((md->isaclfield || md->isaclaction) != sai_metadata_is_acl_field_or_action(md))
    {
        META_MD_ASSERT_FAIL(md, "isaclfield or isaclaction don't match utils method");
    }

    if (md->isaclfield)
    {
        META_ASSERT_FALSE(md->defaultvalue->aclfield.enable, "enable should be false");
    }

    if (md->isaclaction)
    {
        META_ASSERT_FALSE(md->defaultvalue->aclaction.enable, "enable should be false");
    }

    if (md->objecttype != SAI_OBJECT_TYPE_ACL_ENTRY)
    {
        META_ASSERT_FALSE(md->isaclfield, "field should be not marked as acl field");
        META_ASSERT_FALSE(md->isaclaction, "field should be not marked as acl action");

        return;
    }

    if (md->attrid >= SAI_ACL_ENTRY_ATTR_FIELD_START &&
            md->attrid <= SAI_ACL_ENTRY_ATTR_FIELD_END)
    {
        META_ASSERT_TRUE(md->isaclfield, "field should be marked as acl field");
        META_ASSERT_FALSE(md->isaclaction, "field should be not marked as acl action");
    }

    if (md->attrid >= SAI_ACL_ENTRY_ATTR_ACTION_START &&
            md->attrid <= SAI_ACL_ENTRY_ATTR_ACTION_END)
    {
        META_ASSERT_FALSE(md->isaclfield, "field should not be marked as acl field");
        META_ASSERT_TRUE(md->isaclaction, "field should be marked as acl action");
    }
}

void check_attr_existing_objects(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    /*
     * Purpose of this test it to find attributes on objects existing already
     * on the switch with attributes that are mandatory on create and create
     * and set.  Those attributes can be changed by user from previous value,
     * and this causes problem for comparison logic to bring those objects to
     * default value. We need to store those initial values of created objects
     * somewhere.
     *
     * Worth notice, that this is only helper, since metadata on attributes
     * where default value for oid attribute is SAI_NULL_OBJECT_ID, but maybe
     * on the switch vendor actually assigned some value, so default value will
     * not be NULL after creation.
     */

    if (sai_metadata_all_object_type_infos[md->objecttype]->isnonobjectid)
    {
        if (md->storedefaultvalue)
        {
            /*
             * Currently disabled since we need more complicated logic in parser
             * and we assume non object id's are not created at the switch by
             * internal components.
             *
             * META_MD_ASSERT_FAIL(md, "store default val should be not present on non object id");
             */
        }

        return;
    }

    if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_VENDOR_SPECIFIC ||
            md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE)
    {
        /*
         * For attr value we can make restriction that value also needs to be
         * CREATE_AND_SET, since some of those values are read only.
         */

        if (!md->storedefaultvalue)
        {
            META_MD_ASSERT_FAIL(md, "vendor/attrvalue specific values needs to be stored");
        }

        META_LOG_DEBUG("vendor/attrvalue specific values needs to be stored %s", md->attridname);

        return;
    }

    if (!SAI_HAS_FLAG_MANDATORY_ON_CREATE(md->flags) || !SAI_HAS_FLAG_CREATE_AND_SET(md->flags))
    {
        return;
    }

    if (!md->storedefaultvalue)
    {
        META_MD_ASSERT_FAIL(md, "default value needs to be stored");
    }

    META_LOG_DEBUG("MANDATORY_ON_CREATE|CREATE_AND_SET values needs to be stored %s", md->attridname);

    /*
     * If attribute is mandatory on create and create and set then there is no
     * default value on created object, and user can change it's value so in
     * comparison logic we will need to maintain this state somewhere as
     * default.
     *
     * Actually even if object is create only and is created on the switch we
     * need to keep it's value for future reference count in metadata db.
     */

    /*
     * Currently we are limiting value types on existing objects that are
     * mandatory on create to primitive values.
     */

    switch (md->attrvaluetype)
    {
        case SAI_ATTR_VALUE_TYPE_INT32:
        case SAI_ATTR_VALUE_TYPE_INT8:
        case SAI_ATTR_VALUE_TYPE_IP_ADDRESS:
        case SAI_ATTR_VALUE_TYPE_MAC:
        case SAI_ATTR_VALUE_TYPE_UINT16:
        case SAI_ATTR_VALUE_TYPE_UINT32:
        case SAI_ATTR_VALUE_TYPE_UINT64:
        case SAI_ATTR_VALUE_TYPE_UINT8:

            /*
             * Primitives we can skip for now, just left as was set by user
             * with warning in syslog.
             */

            break;

        case SAI_ATTR_VALUE_TYPE_OBJECT_ID:

            if (md->allownullobjectid)
            {
                /*
                 * If object allows NULL object id then we assume that this can
                 * be used as default value.
                 */

                return;
            }

            /*
             * When type is object id we need to store it's previous value
             * since we will not be able to bring it to default.
             */

            META_LOG_DEBUG("Default value (oid) needs to be stored %s", md->attridname);

            break;

        case SAI_ATTR_VALUE_TYPE_QOS_MAP_LIST:

            /*
             * Allow qos maps list to enable editing qos map values.
             * Since on switch initialization there are no qos map objects (all switch qos
             * maps attributes are null) this shouldn't be a problem.
             */

            break;

        case SAI_ATTR_VALUE_TYPE_UINT32_LIST:

            /*
             * Allow for TAM histogram bin boundary
             */

            break;

        case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:

            /*
             * Allow object list for selected objects (for now).
             */

            if (md->objecttype == SAI_OBJECT_TYPE_MIRROR_SESSION)
            {
                break;
            }

            META_MD_ASSERT_FAIL(md, "object list is not supported on this object type");

        case SAI_ATTR_VALUE_TYPE_POINTER:

            /*
             * Allow pointer for switch register read and write API's.
             */

            break;

        default:

            META_MD_ASSERT_FAIL(md, "not supported attr value type on existing object");
    }
}

void check_attr_sai_pointer(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->iscallback)
    {
        META_ASSERT_TRUE(md->attrvaluetype == SAI_ATTR_VALUE_TYPE_POINTER, "callback can be set only on pointer type");
        META_ASSERT_TRUE(md->notificationtype == -1, "callback can't be notification");
    }

    if (md->notificationtype != -1)
    {
        META_ASSERT_FALSE(md->iscallback, "notification can't be callback");
    }

    if (md->pointertype != -1)
    {
        META_ASSERT_TRUE(md->attrvaluetype == SAI_ATTR_VALUE_TYPE_POINTER, "pointer can be set only on pointer type");
    }

    /*
     * Purpose of this test is to check whether sai_pointer_t
     * is only used on SAI_OBJECT_TYPE_SWITCH.
     */

    if (md->objecttype == SAI_OBJECT_TYPE_SWITCH)
    {
        if (md->attrvaluetype == SAI_ATTR_VALUE_TYPE_POINTER)
        {
            /*
             * Make sure that all pointers are CREATE_AND_SET.
             */

            if (!SAI_HAS_FLAG_CREATE_AND_SET(md->flags))
            {
                META_MD_ASSERT_FAIL(md, "all pointers should be CREATE_AND_SET");
            }

            if (md->iscallback)
            {
                META_ASSERT_TRUE(md->notificationtype == -1, "notification type should be marked as callback");
            }
            else
            {
                META_ASSERT_TRUE(md->notificationtype >= 0, "notification type should be set to value on pointer");
            }

            if (md->pointertype < 0)
            {
                META_MD_ASSERT_FAIL(md, "pointer type should be set to value on pointer");
            }
        }
        else
        {
            META_ASSERT_TRUE(md->pointertype == -1, "pointer type should not be set to value on non pointer");
            META_ASSERT_TRUE(md->notificationtype == -1, "notification type should not be set to value on non pointer");
            META_ASSERT_TRUE(md->iscallback == false, "callback type should not be set to value on non pointer");
        }

        return;
    }

    if (md->attrvaluetype == SAI_ATTR_VALUE_TYPE_POINTER)
    {
        META_MD_ASSERT_FAIL(md, "attribute value pointer is only allowed on SAI_OBJECT_TYPE_SWITCH");
    }
}

void check_attr_brief_description(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to see if brief description extract from
     * header is present and not too long.
     */

    META_ASSERT_NOT_NULL(md->brief);

    if (strlen(md->brief) > 200)
    {
        META_MD_ASSERT_FAIL(md, "brief description is too long > 200");
    }
}

void check_attr_is_primitive(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to see if isprimitive flag is correct.
     */

    switch (md->attrvaluetype)
    {
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST:
        case SAI_ATTR_VALUE_TYPE_INT32_LIST:
        case SAI_ATTR_VALUE_TYPE_INT8_LIST:
        case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_QOS_MAP_LIST:
        case SAI_ATTR_VALUE_TYPE_MAP_LIST:
        case SAI_ATTR_VALUE_TYPE_UINT32_LIST:
        case SAI_ATTR_VALUE_TYPE_UINT8_LIST:
        case SAI_ATTR_VALUE_TYPE_UINT16_LIST:
        case SAI_ATTR_VALUE_TYPE_VLAN_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_CAPABILITY:
        case SAI_ATTR_VALUE_TYPE_ACL_RESOURCE_LIST:
        case SAI_ATTR_VALUE_TYPE_TLV_LIST:
        case SAI_ATTR_VALUE_TYPE_SEGMENT_LIST:
        case SAI_ATTR_VALUE_TYPE_IP_ADDRESS_LIST:
        case SAI_ATTR_VALUE_TYPE_PORT_EYE_VALUES_LIST:
        case SAI_ATTR_VALUE_TYPE_SYSTEM_PORT_CONFIG_LIST:
        case SAI_ATTR_VALUE_TYPE_PORT_ERR_STATUS_LIST:
        case SAI_ATTR_VALUE_TYPE_UINT16_RANGE_LIST:

            if (md->isprimitive)
            {
                META_MD_ASSERT_FAIL(md, "marked as primitive on list")
            }

            break;

        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_BOOL:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IP_ADDRESS:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_BOOL:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT64:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8:
        case SAI_ATTR_VALUE_TYPE_BOOL:
        case SAI_ATTR_VALUE_TYPE_CHARDATA:
        case SAI_ATTR_VALUE_TYPE_INT32:
        case SAI_ATTR_VALUE_TYPE_INT8:
        case SAI_ATTR_VALUE_TYPE_IP_ADDRESS:
        case SAI_ATTR_VALUE_TYPE_IP_PREFIX:
        case SAI_ATTR_VALUE_TYPE_PRBS_RX_STATE:
        case SAI_ATTR_VALUE_TYPE_MAC:
        case SAI_ATTR_VALUE_TYPE_OBJECT_ID:
        case SAI_ATTR_VALUE_TYPE_POINTER:
        case SAI_ATTR_VALUE_TYPE_UINT16:
        case SAI_ATTR_VALUE_TYPE_UINT32:
        case SAI_ATTR_VALUE_TYPE_UINT32_RANGE:
        case SAI_ATTR_VALUE_TYPE_UINT64:
        case SAI_ATTR_VALUE_TYPE_UINT8:
        case SAI_ATTR_VALUE_TYPE_TIMESPEC:
        case SAI_ATTR_VALUE_TYPE_IPV4:
        case SAI_ATTR_VALUE_TYPE_IPV6:
        case SAI_ATTR_VALUE_TYPE_ENCRYPT_KEY:
        case SAI_ATTR_VALUE_TYPE_AUTH_KEY:
        case SAI_ATTR_VALUE_TYPE_MACSEC_SAK:
        case SAI_ATTR_VALUE_TYPE_MACSEC_AUTH_KEY:
        case SAI_ATTR_VALUE_TYPE_MACSEC_SALT:
        case SAI_ATTR_VALUE_TYPE_SYSTEM_PORT_CONFIG:
        case SAI_ATTR_VALUE_TYPE_FABRIC_PORT_REACHABILITY:

            if (!md->isprimitive)
            {
                META_MD_ASSERT_FAIL(md, "not marked as primitive on primitive")
            }

            break;

        default:

            META_MD_ASSERT_FAIL(md, "attr value type not handled, FIXME");
    }
}

typedef struct _stack_item_t
{
    bool value;
    struct _stack_item_t* next;

} stack_item_t;

typedef struct _stack_t
{
    stack_item_t* top;

} stack_t;

void stack_init(
        _Inout_ stack_t* stack)
{
    META_LOG_ENTER();

    stack->top = NULL;
}

void stack_push(
        _Inout_ stack_t* stack,
        _In_ bool value)
{
    META_LOG_ENTER();

    stack_item_t *item = (stack_item_t*)calloc(1, sizeof(stack_item_t));

    item->value = value;
    item->next = stack->top;

    stack->top = item;
}

bool stack_pop(
        _Inout_ stack_t* stack)
{
    META_LOG_ENTER();

    if (stack->top == NULL)
    {
        META_ASSERT_FAIL("stack is empty");
    }

    bool value = stack->top->value;
    stack_item_t* top = stack->top;
    stack->top = top->next;

    free(top);

    return value;
}

bool check_mixed_condition_list(
        _In_ const sai_attr_metadata_t* md,
        _In_ const sai_attr_condition_t* const* list)
{
    META_LOG_ENTER();

    if (list[0] == NULL)
    {
        META_MD_ASSERT_FAIL(md, "hit end of condition list, RPN condition logic is BROKEN");
    }

    stack_t stack;

    stack_init(&stack);

    while (list[0] != NULL)
    {
        const sai_attr_condition_t* c = list[0];

        if (c->type == SAI_ATTR_CONDITION_TYPE_NONE)
        {
            stack_push(&stack, true);

            list++;
            continue;
        }

        if (c->type == SAI_ATTR_CONDITION_TYPE_AND)
        {
            bool value = stack_pop(&stack) & stack_pop(&stack);

            stack_push(&stack, value);

            list++;
            continue;
        }

        if (c->type == SAI_ATTR_CONDITION_TYPE_OR)
        {
            bool value = stack_pop(&stack) | stack_pop(&stack);

            stack_push(&stack, value);

            list++;
            continue;
        }

        META_MD_ASSERT_FAIL(md, "wrong condition type on list: %d", c->type);
    }

    bool value = stack_pop(&stack);

    if (stack.top != NULL)
    {
        META_MD_ASSERT_FAIL(md, "stack not empty after condition list check, RPN condition logic is BROKEN");
    }

    return value;
}

void check_attr_mixed_condition(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->conditiontype == SAI_ATTR_CONDITION_TYPE_MIXED)
    {
        META_ASSERT_TRUE(md->isconditional, "must be conditional");
        META_ASSERT_TRUE(md->conditions != NULL, "must be conditional");

        uint32_t index = 0;

        for (; index < md->conditionslength; ++index)
        {
            const sai_attr_condition_t* c = md->conditions[index];

            if (c->type == SAI_ATTR_CONDITION_TYPE_NONE)
            {
                META_ASSERT_TRUE(c->attrid != SAI_INVALID_ATTRIBUTE_ID, "attribute must be defined for condition");
            }
        }

        META_ASSERT_TRUE(md->conditions[0]->type == SAI_ATTR_CONDITION_TYPE_NONE, "first mixed condition entry must be type none");
        META_ASSERT_TRUE(md->conditions[md->conditionslength-1]->type != SAI_ATTR_CONDITION_TYPE_NONE, "last mixed condition entry cannot be none");

        bool value = check_mixed_condition_list(md, md->conditions);

        META_ASSERT_TRUE(value, "should evaluate to true");
    }
}

void check_attr_mixed_validonly(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->validonlytype == SAI_ATTR_CONDITION_TYPE_MIXED)
    {
        META_ASSERT_TRUE(md->isvalidonly, "must be validonly");
        META_ASSERT_TRUE(md->validonly != NULL, "must be validonly");

        uint32_t index = 0;

        for (; index < md->validonlylength; ++index)
        {
            const sai_attr_condition_t* c = md->validonly[index];

            if (c->type == SAI_ATTR_CONDITION_TYPE_NONE)
            {
                META_ASSERT_TRUE(c->attrid != SAI_INVALID_ATTRIBUTE_ID, "attribute must be defined for condition");
            }
        }

        META_ASSERT_TRUE(md->validonly[0]->type == SAI_ATTR_CONDITION_TYPE_NONE, "first mixed condition entry must be type none");
        META_ASSERT_TRUE(md->validonly[md->validonlylength-1]->type != SAI_ATTR_CONDITION_TYPE_NONE, "last mixed condition entry cannot be none");

        bool value = check_mixed_condition_list(md, md->validonly);

        META_ASSERT_TRUE(value, "should evaluate to true");
    }
}

void check_attr_condition_met(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    sai_attribute_t attr = { 0 };

    META_ASSERT_FALSE(sai_metadata_is_condition_met(NULL, 1, NULL), "condition check failed, %s", md->attridname);
    META_ASSERT_FALSE(sai_metadata_is_condition_met(NULL, 1, &attr), "condition check failed, %s", md->attridname);

    if (!md->isconditional)
    {
        META_ASSERT_FALSE(sai_metadata_is_condition_met(md, 1, &attr), "condition check failed");
        return;
    }

    META_ASSERT_TRUE(md->conditionslength <= SAI_METADATA_MAX_CONDITIONS_LEN, "length must not be exceeded");

    /* attr is conditional */

    /*
     * If there are multiple conditions, we need to provide fake values for all
     * others to force return false to test each one separately.
     */

    uint32_t count = (uint32_t)md->conditionslength;

    META_ASSERT_TRUE(count < 20, "too many conditions on %s", md->attridname);

    sai_attribute_t *attrs = (sai_attribute_t*)calloc(count, sizeof(sai_attribute_t));

    size_t idx = 0;

    for (idx = 0; idx < count; ++idx)
    {
        attrs[idx].id = md->conditions[idx]->attrid;
        attrs[idx].value = md->conditions[idx]->condition; /* copy */
    }

    META_ASSERT_TRUE(sai_metadata_is_condition_met(md, count, attrs), "condition should be met on %s", md->attridname);

    if (md->conditiontype == SAI_ATTR_CONDITION_TYPE_OR)
    {
        for (idx = 0; idx < count; ++idx)
        {
            attrs[idx].id ^= (uint32_t)(-1);
        }

        /*
         * Condition can actually be met here, since we are supplying unknown attributes
         * and condition by default attribute can be met
         * META_ASSERT_FALSE(sai_metadata_is_condition_met(md, count, attrs), "condition should not be met");
         */

        /* when condition is "or" then any of attribute should match */

        for (idx = 0; idx < count; ++idx)
        {
            /*
             * Since multiple attributes with the same ID are passed,
             * sai_metadata_is_condition_met is using sai_metadata_get_attr_by_id
             * and only first attribute will be selected.
             */

            attrs[idx].id ^= (uint32_t)(-1);

            META_ASSERT_TRUE(sai_metadata_is_condition_met(md, count, attrs), "condition should be met");

            attrs[idx].id ^= (uint32_t)(-1);
        }
    }
    else if (md->conditiontype == SAI_ATTR_CONDITION_TYPE_AND)
    {
        META_ASSERT_TRUE(sai_metadata_is_condition_met(md, count, attrs), "condition should not be met");

        /* when condition is "and" then any of wrong attribute should fail condition */

        for (idx = 0; idx < count; ++idx)
        {
            /*
             * NOTE: it may happen that missing attribute have default value
             * present, and then that default value will be used for condition
             * compare, eg: SAI_PORT_ATTR_1000X_SGMII_SLAVE_AUTODETECT and
             * SAI_PORT_ATTR_MEDIA_TYPE.
             */

            attrs[idx].id ^= (uint32_t)(-1);

            META_ASSERT_FALSE(sai_metadata_is_condition_met(md, count, attrs), "condition should be met");

            attrs[idx].id ^= (uint32_t)(-1);
        }
    }
    else if (md->conditiontype == SAI_ATTR_CONDITION_TYPE_MIXED)
    {
        /* OK */
    }
    else
    {
        META_MD_ASSERT_FAIL(md, "unsupported condition type");
    }

    free(attrs);
}

void check_attr_validonly_met(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    sai_attribute_t attr = { 0 };

    META_ASSERT_FALSE(sai_metadata_is_validonly_met(NULL, 1, NULL), "validonly check failed");
    META_ASSERT_FALSE(sai_metadata_is_validonly_met(NULL, 1, &attr), "validonly check failed");

    if (!md->isvalidonly)
    {
        META_ASSERT_FALSE(sai_metadata_is_validonly_met(md, 1, &attr), "validonly check failed");
        return;
    }

    META_ASSERT_TRUE(md->validonlylength <= SAI_METADATA_MAX_CONDITIONS_LEN, "length must not be exceeded");

    /* attr is validonly */

    /*
     * If there are multiple validonlys, we need to provide fake values for all
     * others to force return false to test each one separately.
     */

    uint32_t count = (uint32_t)md->validonlylength;

    META_ASSERT_TRUE(count < 20, "too many conditions on %s", md->attridname);

    sai_attribute_t *attrs = (sai_attribute_t*)calloc(count, sizeof(sai_attribute_t));

    size_t idx = 0;

    for (idx = 0; idx < count; ++idx)
    {
        attrs[idx].id = md->validonly[idx]->attrid;
        attrs[idx].value = md->validonly[idx]->condition; /* copy */
    }

    META_ASSERT_TRUE(sai_metadata_is_validonly_met(md, count, attrs), "validonly should be met on %s", md->attridname);

    if (md->validonlytype == SAI_ATTR_CONDITION_TYPE_OR)
    {
        for (idx = 0; idx < count; ++idx)
        {
            attrs[idx].id ^= (uint32_t)(-1);
        }

        /*
         * Condition can actually be met here, since we are supplying unknown attributes
         * and validonly by default attribute can be met
         * META_ASSERT_FALSE(sai_metadata_is_validonly_met(md, count, attrs), "validonly should not be met");
         */

        /* when validonly is "or" then any of attribute should match */

        for (idx = 0; idx < count; ++idx)
        {
            /*
             * Since multiple attributes with the same ID are passed,
             * sai_metadata_is_validonly_met is using sai_metadata_get_attr_by_id
             * and only first attribute will be selected.
             */

            attrs[idx].id ^= (uint32_t)(-1);

            META_ASSERT_TRUE(sai_metadata_is_validonly_met(md, count, attrs), "validonly should be met");

            attrs[idx].id ^= (uint32_t)(-1);
        }
    }
    else if (md->validonlytype == SAI_ATTR_CONDITION_TYPE_AND)
    {
        META_ASSERT_TRUE(sai_metadata_is_validonly_met(md, count, attrs), "validonly should not be met");

        /* when validonly is "and" then any of wrong attribute should fail validonly */

        for (idx = 0; idx < count; ++idx)
        {
            /*
             * NOTE: it may happen that missing attribute have default value
             * present, and then that default value will be used for condition
             * compare, eg: SAI_PORT_ATTR_1000X_SGMII_SLAVE_AUTODETECT and
             * SAI_PORT_ATTR_MEDIA_TYPE.
             */

            const sai_attr_metadata_t *a = sai_metadata_get_attr_metadata(md->objecttype, attrs[idx].id);

            if (a && a->defaultvalue)
            {
                /* alter passed value */

                attrs[idx].value.s32 ^= (int32_t)(-1);

                META_ASSERT_FALSE(sai_metadata_is_validonly_met(md, count, attrs), "validonly should be met, %s", md->attridname);

                attrs[idx].value.s32 ^= (int32_t)(-1);
            }
            else
            {
                /* simulate missing attribute */

                attrs[idx].id ^= (uint32_t)(-1);

                META_ASSERT_FALSE(sai_metadata_is_validonly_met(md, count, attrs), "validonly should be met, %s", md->attridname);

                attrs[idx].id ^= (uint32_t)(-1);
            }
        }
    }
    else if (md->validonlytype == SAI_ATTR_CONDITION_TYPE_MIXED)
    {
        /* OK */
    }
    else
    {
        META_MD_ASSERT_FAIL(md, "unsupported condition type");
    }

    free(attrs);
}

void check_attr_default_attrvalue(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    /*
     * When default value type is attrvalue, check if this attribute value is
     * switch, or if there is attribute on current object, with object
     * represented by default attrvalue. There can be only 1 attribute with
     * this object type, since when more, then we couldn't decide which one.
     */

    if (md->defaultvaluetype != SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE)
    {
        return;
    }

    if (md->defaultvalueobjecttype == SAI_OBJECT_TYPE_SWITCH)
    {
        /* switch is ok */
        return;
    }

    const sai_object_type_info_t* info =
        sai_metadata_all_object_type_infos[md->objecttype];

    /* search for attribute */

    size_t i = 0;

    int count = 0;

    for (; i < info->attrmetadatalength; ++i)
    {
        const sai_attr_metadata_t *cmd = info->attrmetadata[i];

        if (cmd->isreadonly)
        {
            /* skip read only attributes since we don't set them */
            continue;
        }

        if (cmd->attrvaluetype != SAI_ATTR_VALUE_TYPE_OBJECT_ID)
        {
            /* skip object lists */
            continue;
        }

        if (sai_metadata_is_allowed_object_type(cmd, md->defaultvalueobjecttype))
        {
            /* object type of default value is present on current object */
            count++;
        }
    }

    if (count == 1)
    {
        /* only 1 attribute with this object type is present */
        return;
    }

    if (count == 0)
    {
        META_MD_ASSERT_FAIL(md, "oid attribute with %s is not present in %s",
                sai_metadata_all_object_type_infos[md->defaultvalueobjecttype]->objecttypename,
                sai_metadata_all_object_type_infos[md->objecttype]->objecttypename);
    }

    META_MD_ASSERT_FAIL(md, "too many attributes with %s for default value attrvalue",
            sai_metadata_all_object_type_infos[md->defaultvalueobjecttype]->objecttypename);
}

void check_attr_fdb_flush(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->objecttype != SAI_OBJECT_TYPE_FDB_FLUSH)
    {
        return;
    }

    META_ASSERT_FALSE(md->isconditional, "flush attributes should not be conditional");
    META_ASSERT_FALSE(md->isvalidonly, "flush attributes should not be validonly");

    /*
     * Primitive check can be relaxed in the future.
     */
    META_ASSERT_TRUE(md->isprimitive, "flush attributes should be primitives");
    META_ASSERT_TRUE(md->flags == SAI_ATTR_FLAGS_CREATE_ONLY, "flush attributes should be create only");
}

void check_attr_hostif_packet(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->objecttype != SAI_OBJECT_TYPE_HOSTIF_PACKET)
    {
        return;
    }

    META_ASSERT_FALSE(md->isvalidonly, "hostif packet attributes should not be validonly");

    /*
     * Primitive check can be relaxed in the future.
     */
    META_ASSERT_TRUE(md->isprimitive, "hostif packet attributes should be primitives");

    bool flag = SAI_HAS_FLAG_READ_ONLY(md->flags) || SAI_HAS_FLAG_CREATE_ONLY(md->flags);

    META_ASSERT_TRUE(flag, "hostif packet attributes should be read only or create only");
}

void check_attr_capability(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->capability == NULL)
    {
        META_ASSERT_TRUE(md->capabilitylength == 0, "capability length should be zero when capability is not defined");
        return;
    }

    META_ASSERT_TRUE(md->capabilitylength != 0, "capability length should not be zero when capability is not defined");

    size_t i = 0;

    for (; i < md->capabilitylength; ++i)
    {
        const sai_attr_capability_metadata_t* cap = md->capability[i];

        if (md->isreadonly)
        {
            META_ASSERT_FALSE(cap->operationcapability.create_implemented,
                    "create must be false on readonly attribute, %s", md->attridname);

            META_ASSERT_FALSE(cap->operationcapability.set_implemented,
                    "set must be false on readonly attribute, %s", md->attridname);
        }

        if (md->iscreateonly)
        {
            META_ASSERT_FALSE(cap->operationcapability.set_implemented,
                    "set must be false on createonly attribute, %s", md->attridname);
        }

        if (md->ismandatoryoncreate)
        {
            META_ASSERT_TRUE(cap->operationcapability.create_implemented,
                    "create must be true on mandatoryoncreate attribute, %s", md->attridname);
        }

        if (!md->isenum)
        {
            META_ASSERT_NULL(cap->enumvalues);
            META_ASSERT_TRUE(cap->enumvaluescount == 0, "enum values can't be defined when attribute %s is not enum", md->attridname);
        }
    }

    META_ASSERT_NULL(md->capability[i]); /* guard */
}

void check_attr_extension_flag(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    const sai_object_type_info_t* oi = sai_metadata_get_object_type_info(md->objecttype);

    if (md->attrid >= oi->attridend && md->attrid < CUSTOM_ATTR_RANGE_START)
    {
        META_ASSERT_TRUE(md->isextensionattr, "attribute %s expected to be extension", md->attridname);
    }
    else
    {
        META_ASSERT_FALSE(md->isextensionattr, "attribute %s not expected to be extension", md->attridname);
    }
}

void check_single_attribute(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    META_LOG_DEBUG("performing metadata sanity check: object type %d, attr id: %d", md->objecttype, md->attrid);

    META_ASSERT_NOT_NULL(md->attridname);

    check_if_attr_was_already_defined(md);
    check_attr_object_type(md);
    check_attr_value_type_range(md);
    check_attr_flags(md);
    check_attr_object_type_provided(md);
    check_attr_allowed_object_types(md);
    check_attr_default_required(md);
    check_attr_enums(md);
    check_attr_default_value_type(md);
    check_attr_conditions(md);
    check_attr_enum_list_condition(md);
    check_attr_validonly(md);
    check_attr_enum_list_validonly(md);
    check_attr_allow_flags(md);
    check_attr_get_save(md);
    check_attr_key(md);
    check_attr_acl_fields(md);
    check_attr_vlan(md);
    check_attr_object_id_allownull(md);
    check_attr_acl_capability(md);
    check_attr_reverse_graph(md);
    check_attr_acl_conditions(md);
    check_attr_acl_field_or_action(md);
    check_attr_existing_objects(md);
    check_attr_sai_pointer(md);
    check_attr_brief_description(md);
    check_attr_is_primitive(md);
    check_attr_condition_met(md);
    check_attr_validonly_met(md);
    check_attr_default_attrvalue(md);
    check_attr_fdb_flush(md);
    check_attr_hostif_packet(md);
    check_attr_capability(md);
    check_attr_extension_flag(md);
    check_attr_mixed_condition(md);
    check_attr_mixed_validonly(md);

    define_attr(md);
}

void check_single_object_type_attributes(
        _In_ const sai_attr_metadata_t* const* const attributes)
{
    META_LOG_ENTER();

    size_t index = 0;

    for (; attributes[index] != NULL; ++index)
    {
        check_single_attribute(attributes[index]);
    }
}

void check_stat_enums()
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to find out if object types that have
     * statistics (like PORT, etc) have stat enum values populated.
     */

    size_t i = SAI_OBJECT_TYPE_NULL;

    int count = 0;

    for (; i <= SAI_OBJECT_TYPE_EXTENSIONS_MAX; ++i)
    {
        const sai_object_type_info_t* info = sai_metadata_all_object_type_infos[i];

        if (info == NULL)
        {
            continue;
        }

        if (info->statenum != NULL)
        {
            count++;
        }
    }

    META_ASSERT_TRUE(count > 10, "at least some sai_object_type_into_t->statenum must be populated");
}

void check_object_infos()
{
    META_LOG_ENTER();

    size_t i = SAI_OBJECT_TYPE_NULL;

    for (; i <= SAI_OBJECT_TYPE_EXTENSIONS_MAX; ++i)
    {
        const sai_object_type_info_t* info = sai_metadata_all_object_type_infos[i];

        if (i == SAI_OBJECT_TYPE_NULL || i == SAI_OBJECT_TYPE_EXTENSIONS_MAX)
        {
            META_ASSERT_NULL(info);
            continue;
        }

        META_ASSERT_NOT_NULL(info->enummetadata);

        META_ASSERT_TRUE(info->enummetadata->objecttype == i, "should be equal");

        META_ASSERT_TRUE(info->objecttype == i, "object type mismatch");

        META_ASSERT_NOT_NULL(info->objecttypename);

        META_LOG_DEBUG("processing object type: %s", sai_metadata_get_object_type_name((sai_object_type_t)i));

        META_ASSERT_TRUE(info->attridstart == 0, "attribute enum start should be zero");
        META_ASSERT_TRUE(info->attridend > 0, "attribute enum end must be > 0");

        const sai_attr_metadata_t* const* const meta = info->attrmetadata;

        META_ASSERT_NOT_NULL(meta);

        size_t index = 0;

        int last = -1;

        /* check all listed attributes under this object type */

        bool has_extensions_attrs = false;

        for (; meta[index] != NULL; ++index)
        {
            const sai_attr_metadata_t* am = meta[index];

            META_ASSERT_TRUE((int)am->attrid >= 0, "attribute must be non negative");
            META_ASSERT_TRUE(last < (int)am->attrid, "attributes are not increasing");

            if (last + 1 != (int)am->attrid)
            {
                if (is_flag_enum(info->enummetadata))
                {
                    /* flags, ok */
                }
                else
                {
                    META_MD_ASSERT_FAIL(am, "attr id is not increasing by 1: prev %d, curr %d", last, am->attrid);
                }
            }

            if (am->isextensionattr)
            {
                has_extensions_attrs = true;
            }

            last = (int)am->attrid;

            if (am->attrid >= info->attridstart &&
                    am->attrid < info->attridend)
            {
                continue;
            }

            if (am->attrid >= CUSTOM_ATTR_RANGE_START)
            {
                /*
                 * Attribute ID is in custom range, so it will not be in
                 * regular start .. end range.
                 */

                continue;
            }

            if (am->attrid >= info->attridend && am->isextensionattr)
            {
                /* extensions attribute id can be beyond attr id end range */
                continue;
            }

            META_MD_ASSERT_FAIL(am, "attr is is not in start .. end range");
        }

        META_ASSERT_NOT_NULL(info->enummetadata);

        if (index != info->attridend)
        {
            if (is_flag_enum(info->enummetadata))
            {
                /* ok, flags */
            }
            else if (has_extensions_attrs)
            {
                /* ok, extension attribute */
            }
            else
            {
                META_ENUM_ASSERT_FAIL(info->enummetadata, "end of attributes don't match attr count on %s",
                        sai_metadata_get_object_type_name((sai_object_type_t)i));
            }
        }
    }
}

void check_non_object_id_object_types()
{
    META_LOG_ENTER();

    size_t i = SAI_OBJECT_TYPE_NULL;

    for (; i <= SAI_OBJECT_TYPE_EXTENSIONS_MAX; ++i)
    {
        const sai_object_type_info_t* info = sai_metadata_all_object_type_infos[i];

        if (info == NULL)
        {
            continue;
        }

        if (!info->isnonobjectid)
        {
            if (info->structmemberscount != 0 ||
                    info->structmembers != NULL)
            {
                META_ASSERT_FAIL("object type %zu is non object id but struct members defined", i);
            }

            continue;
        }

        META_ASSERT_TRUE(info->structmemberscount != 0, "non object id should have members defined");
        META_ASSERT_NOT_NULL(info->structmembers);

        /* check each member of the struct */

        size_t j = 0;

        int member_supports_switch_id = 0;

        int lastoffset = -1;

        for (; j < info->structmemberscount; ++j)
        {
            META_ASSERT_NOT_NULL(info->structmembers[j]);

            const sai_struct_member_info_t *m = info->structmembers[j];

            META_ASSERT_NOT_NULL(m->membername);

            META_ASSERT_TRUE(m->size > 0, "struct member size must be greater than zero");
            META_ASSERT_TRUE((int)m->offset > lastoffset, "struct member offset must increase from member to member");

            lastoffset = (int)m->offset;

            switch (m->membervaluetype)
            {
                case SAI_ATTR_VALUE_TYPE_MAC:
                case SAI_ATTR_VALUE_TYPE_INT32:
                case SAI_ATTR_VALUE_TYPE_UINT32:
                case SAI_ATTR_VALUE_TYPE_UINT16:
                case SAI_ATTR_VALUE_TYPE_UINT8:
                case SAI_ATTR_VALUE_TYPE_IP_ADDRESS:
                case SAI_ATTR_VALUE_TYPE_IP_PREFIX:
                case SAI_ATTR_VALUE_TYPE_OBJECT_ID:
                case SAI_ATTR_VALUE_TYPE_NAT_ENTRY_DATA:
                case SAI_ATTR_VALUE_TYPE_ENCRYPT_KEY:
                case SAI_ATTR_VALUE_TYPE_AUTH_KEY:
                case SAI_ATTR_VALUE_TYPE_MACSEC_SAK:
                case SAI_ATTR_VALUE_TYPE_MACSEC_AUTH_KEY:
                case SAI_ATTR_VALUE_TYPE_MACSEC_SALT:
                case SAI_ATTR_VALUE_TYPE_IPV6:
                    break;

                default:

                    /*
                     * On struct members only primitive types should be
                     * supported so no other structs or lists.
                     */

                    META_ASSERT_FAIL("struct member %s have invalid value type %d", m->membername, m->membervaluetype);
            }

            if (m->isenum)
            {
                META_ASSERT_NOT_NULL(m->enummetadata);

                META_ASSERT_TRUE(m->membervaluetype == SAI_ATTR_VALUE_TYPE_INT32,
                        "when enum is defined in struct member non objectid its type must be INT32");
            }
            else
            {
                META_ASSERT_NULL(m->enummetadata);
            }

            if (m->isvlan)
            {
                META_ASSERT_TRUE(m->membervaluetype == SAI_ATTR_VALUE_TYPE_UINT16, "member marked as vlan, but wrong type specified");
            }

            if (m->membervaluetype == SAI_ATTR_VALUE_TYPE_OBJECT_ID)
            {
                META_ASSERT_NOT_NULL(m->getoid);
                META_ASSERT_NOT_NULL(m->setoid);
                META_ASSERT_NOT_NULL(m->allowedobjecttypes);
                META_ASSERT_TRUE(m->allowedobjecttypeslength > 0, "struct member object id, should specify some object types");

                size_t k = 0;

                for (; k < m->allowedobjecttypeslength; k++)
                {
                    sai_object_type_t ot = m->allowedobjecttypes[k];

                    if (ot == SAI_OBJECT_TYPE_FDB_FLUSH || ot == SAI_OBJECT_TYPE_HOSTIF_PACKET)
                    {
                        META_ASSERT_FAIL("fdb flush or hostif packet can't be used as object in nonobjectid struct");
                    }

                    if (is_valid_object_type(ot))
                    {
                        if (ot == SAI_OBJECT_TYPE_SWITCH)
                        {
                            /*
                             * to make struct object type complete, at least
                             * one struct member should be type of switch
                             */

                            member_supports_switch_id++;

                            if (strcmp("switch_id", m->membername) != 0)
                            {
                                META_ASSERT_FAIL("struct member %s supports object type SWITCH, should be named switch_id", m->membername);
                            }

                            META_ASSERT_TRUE(m->allowedobjecttypeslength == 1, "switch_id member should only support object type SWITCH");
                        }

                        /* non object id struct can't contain object id which is also non object id */

                        const sai_object_type_info_t* sinfo = sai_metadata_all_object_type_infos[ot];

                        META_ASSERT_NOT_NULL(sinfo);

                        if (sinfo->isnonobjectid)
                        {
                            META_ASSERT_FAIL("struct member %s of non object id type can't be used as object id in non object id struct: %s",
                                    m->membername,
                                    sai_metadata_get_object_type_name(ot));
                        }

                        continue;
                    }

                    META_ASSERT_FAIL("invalid object type specified on file %s: %d", m->membername, ot);
                }
            }
            else
            {
                META_ASSERT_NULL(m->getoid);
                META_ASSERT_NULL(m->setoid);
                META_ASSERT_NULL(m->allowedobjecttypes);
                META_ASSERT_TRUE(m->allowedobjecttypeslength == 0, "member is not object id, should not specify object types");
            }
        }

        META_ASSERT_TRUE(member_supports_switch_id == 1, "there should be only one struct member that support switch id object type");

        META_ASSERT_NULL(info->structmembers[j]);
    }
}

void check_non_object_id_object_attrs()
{
    META_LOG_ENTER();

    size_t i = SAI_OBJECT_TYPE_NULL;

    for (; i <= SAI_OBJECT_TYPE_EXTENSIONS_MAX; ++i)
    {
        const sai_object_type_info_t* info = sai_metadata_all_object_type_infos[i];

        if (info == NULL || !info->isnonobjectid)
        {
            continue;
        }

        const sai_attr_metadata_t* const* meta = info->attrmetadata;

        META_ASSERT_NOT_NULL(meta);

        size_t idx = 0;

        /* iterate all attributes on non object id type */

        for (; meta[idx] != NULL; ++idx)
        {
            const sai_attr_metadata_t* m = meta[idx];

            META_ASSERT_NOT_NULL(m);

            if (m->isresourcetype && (int)m->flags == SAI_ATTR_FLAGS_READ_ONLY)
            {
                continue;
            }

            switch ((int)m->flags)
            {
                case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_AND_SET:
                case SAI_ATTR_FLAGS_CREATE_AND_SET:
                    break;

                default:

                    META_MD_ASSERT_FAIL(m, "non object id attribute has invalid flags: 0x%x (should be CREATE_AND_SET)", m->flags);
            }
        }
    }
}

void check_attr_sorted_by_id_name()
{
    META_LOG_ENTER();

    size_t i = 0;

    const char *last = "AAA";

    META_ASSERT_TRUE(sai_metadata_attr_sorted_by_id_name_count > 800,
            "there should be at least 800 attributes in total");

    for (; i < sai_metadata_attr_sorted_by_id_name_count; ++i)
    {
        const sai_attr_metadata_t *am = sai_metadata_attr_sorted_by_id_name[i];

        META_ASSERT_NOT_NULL(am);

        const char *name = am->attridname;

        if (strcmp(last, name) >= 0)
        {
            META_MD_ASSERT_FAIL(am, "attribute id name in not sorted alphabetical");
        }

        META_ASSERT_TRUE(strncmp(name, "SAI_", 4) == 0, "all attributes should start with SAI_");

        last = name;
    }

    META_ASSERT_NULL(sai_metadata_attr_sorted_by_id_name[i]);

    /* check search */

    for (i = 0; i < sai_metadata_attr_sorted_by_id_name_count; ++i)
    {
        const sai_attr_metadata_t *am = sai_metadata_attr_sorted_by_id_name[i];

        META_LOG_DEBUG("search for %s", am->attridname);

        const sai_attr_metadata_t *found = sai_metadata_get_attr_metadata_by_attr_id_name(am->attridname);

        META_ASSERT_NOT_NULL(found);

        META_ASSERT_TRUE(strcmp(found->attridname, am->attridname) == 0, "search attr by id name failed to find");
    }

    META_ASSERT_NULL(sai_metadata_get_attr_metadata_by_attr_id_name(NULL));     /* null pointer */
    META_ASSERT_NULL(sai_metadata_get_attr_metadata_by_attr_id_name("AAA"));    /* before all attr names */
    META_ASSERT_NULL(sai_metadata_get_attr_metadata_by_attr_id_name("SAI_B"));  /* in the middle of attr names */
    META_ASSERT_NULL(sai_metadata_get_attr_metadata_by_attr_id_name("SAI_P"));  /* in the middle of attr names */
    META_ASSERT_NULL(sai_metadata_get_attr_metadata_by_attr_id_name("SAI_W"));  /* in the middle of attr names */
    META_ASSERT_NULL(sai_metadata_get_attr_metadata_by_attr_id_name("ZZZ"));    /* after all attr names */
}

void list_loop(
        _In_ const sai_object_type_info_t* info,
        _In_ const sai_object_type_t *visited,
        _In_ const uint32_t *attributes,
        _In_ int levelidx,
        _In_ int level)
{
    META_LOG_ENTER();

    META_LOG_WARN("LOOP DETECTED on object type: %s",
            sai_metadata_enum_sai_object_type_t.valuesnames[info->objecttype]);

    for (; levelidx < level; ++levelidx)
    {
        sai_object_type_t ot = visited[levelidx];

        const char* ot_name = sai_metadata_enum_sai_object_type_t.valuesnames[ot];

        const sai_attr_metadata_t* m = sai_metadata_get_attr_metadata(ot, attributes[levelidx]);

        META_LOG_WARN(" %s: %s", ot_name, m->attridname);
    }

    META_LOG_WARN(" -> %s", sai_metadata_enum_sai_object_type_t.valuesnames[info->objecttype]);

    if (level >= 0)
    {
        META_ASSERT_FAIL("LOOP is detected, we can't have loops in graph, please fix attributes");
    }
}

void check_objects_for_loops_recursive(
        _In_ const sai_object_type_info_t* info,
        _Inout_ sai_object_type_t *visited,
        _Inout_ uint32_t *attributes,
        _In_ int level)
{
    META_LOG_ENTER();

    visited[level] = info->objecttype;

    int levelidx = 0;

    for (; levelidx < level; ++levelidx)
    {
        if (visited[levelidx] == info->objecttype)
        {
            /* object type is already defined, so we have a loop */

            list_loop(info, visited, attributes, levelidx, level);

            return;
        }
    }

    const sai_attr_metadata_t* const* meta = info->attrmetadata;

    META_ASSERT_NOT_NULL(meta);

    size_t idx = 0;

    /* iterate all attributes on non object id type */

    for (; meta[idx] != NULL; ++idx)
    {
        const sai_attr_metadata_t* m = meta[idx];

        META_ASSERT_NOT_NULL(m);

        if (SAI_HAS_FLAG_READ_ONLY(m->flags))
        {
            /* skip read only attributes since with those we will have loops for sure */

            continue;
        }

        /* skip known loops */

        if (m->objecttype == SAI_OBJECT_TYPE_PORT)
        {
            if (m->attrid == SAI_PORT_ATTR_EGRESS_MIRROR_SESSION ||
                    m->attrid == SAI_PORT_ATTR_INGRESS_MIRROR_SESSION ||
                    m->attrid == SAI_PORT_ATTR_EGRESS_BLOCK_PORT_LIST ||
                    m->attrid == SAI_PORT_ATTR_INGRESS_SAMPLE_MIRROR_SESSION ||
                    m->attrid == SAI_PORT_ATTR_EGRESS_SAMPLE_MIRROR_SESSION)
            {
                continue;
            }
        }

        if (m->objecttype == SAI_OBJECT_TYPE_SCHEDULER_GROUP &&
                m->attrid == SAI_SCHEDULER_GROUP_ATTR_PARENT_NODE)
        {
            continue;
        }

        attributes[level] = m->attrid;

        size_t j = 0;

        for (; j < m->allowedobjecttypeslength; ++j)
        {
            const sai_object_type_info_t* next = sai_metadata_all_object_type_infos[ m->allowedobjecttypes[j] ];

            check_objects_for_loops_recursive(next, visited, attributes, level + 1);
        }
    }

    /* iterate for struct members on non object id object types */

    if (info->isnonobjectid)
    {
        size_t j = 0;

        for (; j < info->structmemberscount; ++j)
        {
            const sai_struct_member_info_t *m = info->structmembers[j];

            if (m->membervaluetype != SAI_ATTR_VALUE_TYPE_OBJECT_ID)
            {
                continue;
            }

            size_t k = 0;

            for (; k < m->allowedobjecttypeslength; k++)
            {
                const sai_object_type_info_t* next = sai_metadata_all_object_type_infos[ m->allowedobjecttypes[k] ];

                check_objects_for_loops_recursive(next, visited, attributes, level + 1);
            }
        }
    }

    /* clear level on exit */

    visited[level] = SAI_OBJECT_TYPE_NULL;
    attributes[level] = 0;
}

void check_objects_for_loops()
{
    META_LOG_ENTER();

    sai_object_type_t visited_objects[SAI_OBJECT_TYPE_EXTENSIONS_MAX];
    uint32_t visited_attributes[SAI_OBJECT_TYPE_EXTENSIONS_MAX];

    size_t i = SAI_OBJECT_TYPE_NULL;

    for (; i <= SAI_OBJECT_TYPE_EXTENSIONS_MAX; ++i)
    {
        const sai_object_type_info_t* info = sai_metadata_all_object_type_infos[i];

        if (info == NULL)
        {
            continue;
        }

        memset(visited_objects, 0, SAI_OBJECT_TYPE_EXTENSIONS_MAX * sizeof(sai_object_type_t));
        memset(visited_attributes, 0, SAI_OBJECT_TYPE_EXTENSIONS_MAX * sizeof(uint32_t));

        check_objects_for_loops_recursive(info, visited_objects, visited_attributes, 0);
    }
}

void check_null_object_id()
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to make sure that
     * SAI_NULL_OBJECT_ID is always ZERO.
     */

    META_ASSERT_TRUE(SAI_NULL_OBJECT_ID == 0, "SAI_NULL_OBJECT_ID must be zero");
}

void check_read_only_attributes()
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to find out if there is any
     * object that has only READ_ONLY attributes.
     *
     * If given object has only read only attributes
     * there should be no purpose of such object.
     * With only read only attributes there is no
     * way to compare 2 objects of the same type
     * sine we don't track read only attributes.
     *
     * As additional check we will also check if given
     * object type defines at least 1 attribute.
     */

    size_t i = SAI_OBJECT_TYPE_NULL;

    for (; i <= SAI_OBJECT_TYPE_EXTENSIONS_MAX; ++i)
    {
        const sai_object_type_info_t* info = sai_metadata_all_object_type_infos[i];

        if (info == NULL)
        {
            continue;
        }

        size_t index = 0;

        /* check all listed attributes under this object type */

        int non_read_only_count = 0;

        const sai_attr_metadata_t* const* const meta = info->attrmetadata;

        for (; meta[index] != NULL; ++index)
        {
            const sai_attr_metadata_t* m = meta[index];

            if (!SAI_HAS_FLAG_READ_ONLY(m->flags))
            {
                non_read_only_count++;
            }
        }

        if (index < 1)
        {
            META_ASSERT_FAIL("object %s must define at least 1 attribute",
                    sai_metadata_get_object_type_name((sai_object_type_t)i));
        }

        if (non_read_only_count == 0)
        {
            /*
             * currently we have some objects with only read only
             * attributes, we for now we just warn here until this
             * issue will be resolved.
             */

            META_LOG_WARN("object %s has only READ_ONLY attributes",
                    sai_metadata_enum_sai_object_type_t.valuesnames[i]);
        }
    }
}

void check_mixed_object_list_types()
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to find out if any of object id lists supports
     * multiple object types at the same time.  For now this ability will not
     * be supported.
     */

    META_ASSERT_TRUE(sai_metadata_attr_sorted_by_id_name_count > 800, "there should be at least 800 attributes in total");

    size_t idx = 0;

    for (; idx < sai_metadata_attr_sorted_by_id_name_count; ++idx)
    {
        const sai_attr_metadata_t* meta = sai_metadata_attr_sorted_by_id_name[idx];

        switch (meta->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:

                META_ASSERT_TRUE(meta->allowedobjecttypeslength > 0, "allowed object types on list can't be zero");

                if (meta->allowedobjecttypeslength == 1)
                {
                    continue;
                }

                if (meta->flags == SAI_ATTR_FLAGS_READ_ONLY)
                {
                    /*
                     * If attribute flag is READ_ONLY, then object can support
                     * mixed object types returned on list, for example
                     * SAI_SCHEDULER_GROUP_ATTR_CHILD_LIST when it returns
                     * schedulers and queues.
                     */
                }
                else
                {
                    if (meta->objecttype == SAI_OBJECT_TYPE_ACL_ENTRY)
                    {
                        /*
                         * Allow mixed object types on ACL entries since they can point
                         * to different object types like PORT or BRIDGE_PORT etc.
                         */

                        break;
                    }

                    if (meta->objecttype == SAI_OBJECT_TYPE_MIRROR_SESSION)
                    {
                        break;
                    }

                    /*
                     * For non read only attributes, there should be a good
                     * reason why object list should support mixed object
                     * types on that list. Then this restriction can be
                     * relaxed and description should be added why mixed
                     * object types should be possible.
                     */

                    META_MD_ASSERT_FAIL(meta, "allowed object types on object id list is more then 1, not supported yet");
                }

                break;

            default:

                META_ASSERT_FALSE(meta->allowmixedobjecttypes, "allow mixed object types should be false on non object id list");
                break;
        }
    }
}

/*
 * Below are defined all generic methods needed for api name check
 */

typedef sai_status_t (*generic_create_fn)(
        _Out_ sai_object_id_t *object_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

typedef sai_status_t (*generic_remove_fn)(
        _In_ sai_object_id_t object_id);

typedef sai_status_t (*generic_set_fn)(
        _In_ sai_object_id_t object_id,
        _In_ const sai_attribute_t *attr);

typedef sai_status_t (*generic_get_fn)(
        _In_ sai_object_id_t object_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef sai_status_t(*switch_create_fn)(
        _Out_ sai_object_id_t* switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

void check_single_non_object_id_for_rev_graph(
        _In_ const sai_struct_member_info_t *sm,
        _In_ sai_object_type_t objecttype,
        _In_ sai_object_type_t depobjecttype)
{
    META_LOG_ENTER();

    /*
     * This method checks single non object id struct
     * member.
     */

    const sai_object_type_info_t *oi = sai_metadata_all_object_type_infos[depobjecttype];

    META_ASSERT_NOT_NULL(oi->revgraphmembers);

    size_t revidx = 0;

    bool defined = false;

    for (; oi->revgraphmembers[revidx] != NULL; revidx++)
    {
        /*
         * now let's search for graph member which defines
         * this object and the same attribute value
         */

        const sai_rev_graph_member_t *rm = oi->revgraphmembers[revidx];

        META_ASSERT_TRUE(rm->objecttype == depobjecttype, "invalid objecttype definition");

        if (rm->depobjecttype != objecttype)
        {
            /*
             * this is not the member we are looking for
             */

            continue;
        }

        if (rm->attrmetadata == NULL)
        {
            META_ASSERT_NOT_NULL(rm->structmember);

            /*
             * This graph entry is struct member, maybe this i the
             * one we are looking for, since graph can have multiple
             * entries for the same object.
             */

            if (strcmp(rm->structmember->membername, sm->membername) != 0)
            {
                /* this is the member we are not looking for */

                continue;
            }

            /*
             * We found out member name, so our object must be on the object list
             */

            size_t i = 0;

            for (; i < rm->structmember->allowedobjecttypeslength; i++)
            {
                /*
                 * Object type of graph member must match and also
                 * attribute id must match.
                 */

                if (rm->structmember->allowedobjecttypes[i] == depobjecttype)
                {
                    META_LOG_DEBUG("dep %s ot %s attr %s\n",
                            sai_metadata_enum_sai_object_type_t.valuesnames[depobjecttype],
                            sai_metadata_enum_sai_object_type_t.valuesnames[objecttype],
                            sm->membername);

                    defined = true;
                    break;
                }
            }

            if (defined)
            {
                break;
            }
        }
        else
        {
            /*
             * object is attribute
             */

            META_ASSERT_NOT_NULL(rm->attrmetadata);
            META_ASSERT_NULL(rm->structmember);

            /*
             * we are not looking for attribute object
             * we are looking for struct member
             */

            continue;
        }
    }

    META_ASSERT_TRUE(defined, "reverse graph object is not defined anywhere");
}

void check_reverse_graph_for_non_object_id()
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to find out whether non object id structmembers
     * which are object id are well defined inside reverse graph. Attribute
     * values are checked during standard loop of attribute above.
     */

    size_t i = SAI_OBJECT_TYPE_NULL;

    for (; i <= SAI_OBJECT_TYPE_EXTENSIONS_MAX; ++i)
    {
        sai_object_type_t objecttype = (sai_object_type_t)i;

        const sai_object_type_info_t* info = sai_metadata_all_object_type_infos[i];

        if (info == NULL || !info->isnonobjectid)
        {
            continue;
        }

        /*
         * This is non object id and they can't have graph members
         * since non object id can't be used as any object id.
         */

        META_ASSERT_NULL(info->revgraphmembers);

        /*
         * Now for each struct member check if it's object id member
         * and process it.
         */

        size_t j = 0;

        for (; j < info->structmemberscount; ++j)
        {
            const sai_struct_member_info_t *m = info->structmembers[j];

            if (m->membervaluetype != SAI_ATTR_VALUE_TYPE_OBJECT_ID)
            {
                continue;
            }

            size_t k = 0;

            for (; k < m->allowedobjecttypeslength; k++)
            {
                /*
                 * For each object type check it's location in graph
                 */

                sai_object_type_t depobjecttype = m->allowedobjecttypes[k];

                check_single_non_object_id_for_rev_graph(m, objecttype, depobjecttype);
            }
        }
    }
}

void check_vlan_attributes()
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to make sure there in vlan object there is only
     * one attribute marked as a KEY and it's a VLAN_ID and it's value type is
     * UINT16. This will be helpful later on on comparison logic since we can
     * have so many vlan's then searching them in hash will be much faster than
     * iterating each time.
     */

    const sai_attr_metadata_t* const* const meta = sai_metadata_object_type_info_SAI_OBJECT_TYPE_VLAN.attrmetadata;

    size_t index = 0;

    int keys = 0;

    for (; meta[index] != NULL; index++)
    {
        const sai_attr_metadata_t *md = meta[index];

        if (SAI_HAS_FLAG_KEY(md->flags))
        {
            keys++;
        }

        if (md->attrid == SAI_VLAN_ATTR_VLAN_ID)
        {
            int expected_flags = (SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY | SAI_ATTR_FLAGS_KEY);

            if ((int)md->flags != expected_flags)
            {
                META_MD_ASSERT_FAIL(md, "vlan id should have flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY, but has: %d", md->flags);
            }

            META_ASSERT_TRUE(md->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT16, "VLAN_ID should be UINT16");
        }
    }

    META_ASSERT_TRUE(keys == 1, "vlan object type should have only 1 attribute marked as key which is vlan id");
}

void check_acl_table_fields_and_acl_entry_fields()
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to find out if acl table fields and acl entry
     * fields correspond to each other. We also make check if they have the
     * same attribute id which is not required but it is nice to have. We also
     * check if those attributes have right flags and right attribute values.
     */

    META_ASSERT_TRUE(SAI_ACL_ENTRY_ATTR_FIELD_START == 0x1000, "acl entry field start value should be 0x1000");
    META_ASSERT_TRUE(SAI_ACL_TABLE_ATTR_FIELD_START == 0x1000, "acl table field start value should be 0x1000");

    META_ASSERT_TRUE((int)SAI_ACL_ENTRY_ATTR_FIELD_START == (int)SAI_ACL_TABLE_ATTR_FIELD_START,
            "acl entry and table fields start should be the same");

    /*
     * We are using volatile here since if we use enum directly and values are
     * different, compiler will optimize this to true and throw error on
     * candidate for non return which in this case is confusing.
     */

    volatile int table_end = SAI_ACL_TABLE_ATTR_FIELD_END;
    volatile int entry_end = SAI_ACL_ENTRY_ATTR_FIELD_END;

    if (table_end != entry_end)
    {
        META_ASSERT_FAIL("SAI_ACL_TABLE_ATTR_FIELD_END 0x%x is not equal to SAI_ACL_ENTRY_ATTR_FIELD_END 0x%x",
                SAI_ACL_TABLE_ATTR_FIELD_END, SAI_ACL_ENTRY_ATTR_FIELD_END);
    }

    /*
     * find both attribute fields start for entry and table
     */

    const sai_attr_metadata_t* const* meta_acl_table = sai_metadata_object_type_info_SAI_OBJECT_TYPE_ACL_TABLE.attrmetadata;
    const sai_attr_metadata_t* const* meta_acl_entry = sai_metadata_object_type_info_SAI_OBJECT_TYPE_ACL_ENTRY.attrmetadata;

    int acl_table_field_index = 0;

    for (; meta_acl_table[acl_table_field_index] != NULL; acl_table_field_index++)
    {
        if (meta_acl_table[acl_table_field_index]->attrid == SAI_ACL_TABLE_ATTR_FIELD_START)
        {
            break;
        }
    }

    META_ASSERT_NOT_NULL(meta_acl_table[acl_table_field_index]);

    int acl_entry_field_index = 0;

    for (; meta_acl_entry[acl_entry_field_index] != NULL; acl_entry_field_index++)
    {
        if (meta_acl_entry[acl_entry_field_index]->attrid == SAI_ACL_ENTRY_ATTR_FIELD_START)
        {
            break;
        }
    }

    META_ASSERT_NOT_NULL(meta_acl_entry[acl_entry_field_index]);

    /*
     * we found our attribute indexes, now let's compare attributes
     */

    while (true)
    {
        const sai_attr_metadata_t *mtable = meta_acl_table[acl_table_field_index];
        const sai_attr_metadata_t *mentry = meta_acl_entry[acl_entry_field_index];

        if (mentry == NULL || mtable == NULL)
        {
            break;
        }

        if (mtable->attrid > SAI_ACL_TABLE_ATTR_FIELD_END ||
                mentry->attrid > SAI_ACL_ENTRY_ATTR_FIELD_END)
        {
            break;
        }

        META_LOG_DEBUG("processing acl fields: %s %s", mtable->attridname, mentry->attridname);

        /*
         * check acl table flags and attr value type
         */

        if ((mtable->attrid == SAI_ACL_TABLE_ATTR_FIELD_ACL_RANGE_TYPE) ||
        ((mtable->attrid >= SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MIN) &&
        (mtable->attrid <= SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MAX)))
        {
            /*
             * This field is exception, it's not bool, it's a list and it's
             * designed in this way to save resources on device to not support
             * all ranges on each acl table when it's not necessary.
             */
        }
        else
        {
            if (mtable->flags != SAI_ATTR_FLAGS_CREATE_ONLY)
            {
                META_MD_ASSERT_FAIL(mtable, "acl table field flags should be CREATE_ONLY");
            }

            if (mtable->attrvaluetype != SAI_ATTR_VALUE_TYPE_BOOL)
            {
                META_MD_ASSERT_FAIL(mtable, "acl table attr value type should be bool");
            }
        }

        /*
         * check acl entry flags
         */

        if (mentry->flags != SAI_ATTR_FLAGS_CREATE_AND_SET)
        {
            META_MD_ASSERT_FAIL(mentry, "acl entry field flags should be CREATE_AND_SET");
        }

        if (mentry->attrid != mtable->attrid)
        {
            META_MD_ASSERT_FAIL(mentry, "acl entry attr id %d is different than acl table field %d", mentry->attrid, mtable->attrid);
        }

        /*
         * check acl fields attribute if endings are the same
         */

        const char * attr_table_pos = strstr(mtable->attridname, "_ATTR_");

        META_ASSERT_NOT_NULL(attr_table_pos);

        const char * attr_entry_pos = strstr(mentry->attridname, "_ATTR_");

        META_ASSERT_NOT_NULL(attr_entry_pos);

        if (strcmp(attr_table_pos, attr_entry_pos) != 0)
        {
            META_ASSERT_FAIL("attr entry field name %s is not ending at the same name as acl table field %s",
                    mentry->attridname, mtable->attridname);
        }

        acl_table_field_index++;
        acl_entry_field_index++;
    }
}

void check_acl_entry_actions()
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to find out if acl entry actions correspond to
     * sai_acl_action_type_t enum type and contain all actions in the same
     * order.
     */

    META_ASSERT_TRUE(SAI_ACL_ENTRY_ATTR_ACTION_START == 0x2000, "acl entry action start value should be 0x2000");

    /*
     * find both attribute fields start for entry and table
     */

    const sai_attr_metadata_t *const * meta_acl_entry = sai_metadata_object_type_info_SAI_OBJECT_TYPE_ACL_ENTRY.attrmetadata;

    size_t index = 0;

    for (; meta_acl_entry[index] != NULL; index++)
    {
        if (meta_acl_entry[index]->attrid == SAI_ACL_ENTRY_ATTR_ACTION_START)
        {
            break;
        }
    }

    META_ASSERT_NOT_NULL(meta_acl_entry[index]);

    /*
     * lets compare all action attributes with enum names
     */

    size_t enum_index = 0;

    while (true)
    {
        const sai_attr_metadata_t *meta = meta_acl_entry[index];

        if (meta == NULL)
        {
            break;
        }

        if ((meta->isextensionattr == false) && (meta->attrid > SAI_ACL_ENTRY_ATTR_ACTION_END))
        {
            break;
        }

        if (meta->flags != SAI_ATTR_FLAGS_CREATE_AND_SET)
        {
            META_MD_ASSERT_FAIL(meta, "acl entry action flags should be CREATE_AND_SET");
        }

        const char* enum_name = sai_metadata_enum_sai_acl_action_type_t.valuesnames[enum_index];

        META_ASSERT_NOT_NULL(enum_name);

        META_LOG_DEBUG("processing acl action: %s %s", meta->attridname, enum_name);

        /*
         * check acl fields attribute if endings are the same
         */

        const char * enum_name_pos = strstr(enum_name, "_ACTION_TYPE_");

        META_ASSERT_NOT_NULL(enum_name_pos);

        const char * attr_entry_pos = strstr(meta->attridname, "_ATTR_ACTION_");

        META_ASSERT_NOT_NULL(attr_entry_pos);

        if (strcmp(enum_name_pos + strlen("_ACTION_TYPE_"), attr_entry_pos + strlen("_ATTR_ACTION_")) != 0)
        {
            META_ASSERT_FAIL("attr entry action name %s is not ending at the same enum name %s",
                    meta->attridname, enum_name);
        }

        index++;
        enum_index++;
    }

    META_ASSERT_TRUE(enum_index == sai_metadata_enum_sai_acl_action_type_t.valuescount,
            "number of acl entry action mismatch vs number of enums in sai_acl_action_type_t");
}

void check_switch_attributes()
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to find out whether switch object has some
     * conditional or validonly attributes. Currently we are making assumptions
     * that there are no such objects, so we are adding check for that, but if
     * there will be need for such in the future, this check can be removed.
     */

    const sai_attr_metadata_t* const* const meta = sai_metadata_object_type_info_SAI_OBJECT_TYPE_SWITCH.attrmetadata;

    size_t index = 0;

    for (; meta[index] != NULL; index++)
    {
        const sai_attr_metadata_t *md = meta[index];

        /*
         * Gearbox attributes can be marked as mandatory on create.
         */

        if (md->isoidattribute && md->ismandatoryoncreate)
        {
            META_MD_ASSERT_FAIL(md, "Mandatory on create can't be object id on SWITCH");
        }

        if (md->isoidattribute && md->iscreateonly)
        {
            META_MD_ASSERT_FAIL(md, "Create only can't be object id on SWITCH");
        }
    }
}

void check_switch_create_only_objects()
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to find out whether switch object has some
     * attributes that are object id type and are marked as CREATE_ONLY.  Such
     * attribute has no sense since, you need first switch_id to create any
     * other object so setting that object on create will be impossible.
     */

    const sai_attr_metadata_t* const* const meta = sai_metadata_object_type_info_SAI_OBJECT_TYPE_SWITCH.attrmetadata;

    size_t index = 0;

    for (; meta[index] != NULL; index++)
    {
        const sai_attr_metadata_t *md = meta[index];

        if (SAI_HAS_FLAG_CREATE_ONLY(md->flags) && md->isoidattribute)
        {
            META_MD_ASSERT_FAIL(md, "attribute is create_only and it's an object id, this is not allowed");
        }
    }
}

void check_quad_api_pointers(
        _In_ const sai_object_type_info_t *oi)
{
    META_LOG_ENTER();

    /*
     * Check if quad api pointers are not NULL, hostif packet and fdb flush are
     * special, dummy functions are generated.
     */

    META_ASSERT_NOT_NULL(oi->create);
    META_ASSERT_NOT_NULL(oi->remove);
    META_ASSERT_NOT_NULL(oi->set);
    META_ASSERT_NOT_NULL(oi->get);
}

void check_stats_api_pointers(
        _In_ const sai_object_type_info_t *oi)
{
    META_LOG_ENTER();

    /*
     * Check if stats api pointers are not NULL, for objects that don't support
     * stats dummy functions are generated.
     */

    META_ASSERT_NOT_NULL(oi->getstats);
    META_ASSERT_NOT_NULL(oi->getstatsext);
    META_ASSERT_NOT_NULL(oi->clearstats);
}

void check_object_id_non_object_id(
        _In_ const sai_object_type_info_t *oi)
{
    META_LOG_ENTER();

    /*
     * Purpose of this test is to check whether isobjectid and isnonobject id
     * have opposite values.
     */

    META_ASSERT_TRUE(oi->isnonobjectid == !oi->isobjectid, "non object id object id not match");
}

void check_enum_to_attr_map(
        _In_ const sai_object_type_info_t *oi)
{
    META_LOG_ENTER();

    /*
     * Check whether attribute enum declared has equal number of items as the
     * number of declared attributes. Item with @ignore flag should be
     * removed from enum and attribute should not be created.
     */

    META_LOG_DEBUG("checking %s", oi->objecttypename);

    uint32_t i = 0;

    META_ASSERT_TRUE(oi->enummetadata->valuescount == oi->attrmetadatalength, "attr length must be equal to enum length");

    for (; i < oi->enummetadata->valuescount ;i++)
    {
        META_LOG_DEBUG("checking enum %s", oi->enummetadata->valuesnames[i]);

        const sai_attr_metadata_t *m = oi->attrmetadata[i];

        META_ASSERT_NOT_NULL(m);

        META_ASSERT_TRUE(m->attrid == (uint32_t)oi->enummetadata->values[i], "attrid must be equal to enum");
    }

    META_ASSERT_NULL(oi->attrmetadata[i]);
}

void check_object_ro_list(
        _In_ const sai_object_type_info_t *oi)
{
    META_LOG_ENTER();

    /*
     * Purpose is to check if object is referenced in any other object as read
     * only attribute to know that we can get all objects of this type.
     * Example: VLAN and VLAN_MEMBER. All vlan members are listed on attribute:
     * SAI_VLAN_ATTR_MEMBER_LIST.
     *
     * Should we only check that for leaf objects?
     */

    if (oi->isnonobjectid)
    {
        return;
    }

    if (oi->objecttype == SAI_OBJECT_TYPE_FDB_FLUSH ||
            oi->objecttype == SAI_OBJECT_TYPE_HOSTIF_PACKET ||
            oi->objecttype == SAI_OBJECT_TYPE_SWITCH ||
            oi->objecttype == SAI_OBJECT_TYPE_BFD_SESSION ||
            oi->objecttype == SAI_OBJECT_TYPE_HOSTIF_TABLE_ENTRY ||
            oi->objecttype == SAI_OBJECT_TYPE_DTEL ||
            oi->objecttype == SAI_OBJECT_TYPE_DTEL_QUEUE_REPORT ||
            oi->objecttype == SAI_OBJECT_TYPE_DTEL_EVENT)
    {
        /*
         * We skip hostif table entry since there is no 1 object which can
         * identify all table entries. We would need to add one attribute for
         * each used object type port, lag, vlan etc.
         */

        return;
    }

    size_t idx = 0;

    for (; idx < sai_metadata_attr_sorted_by_id_name_count; ++idx)
    {
        const sai_attr_metadata_t *meta = sai_metadata_attr_sorted_by_id_name[idx];

        if (sai_metadata_is_allowed_object_type(meta, oi->objecttype))
        {
            if (oi->revgraphmembers != 0)
            {
                /* this object is not leaf, so it must be used as attribute */
                return;
            }
        }

        if (meta->attrvaluetype != SAI_ATTR_VALUE_TYPE_OBJECT_LIST)
        {
            continue;
        }

        if (!meta->isreadonly)
        {
            continue;
        }

        if (sai_metadata_is_allowed_object_type(meta, oi->objecttype))
        {
            return;
        }
    }

    if (oi->isexperimental)
    {
        META_LOG_WARN("experimental object %s not present on any object list (eg. VLAN_MEMBER is present on SAI_VLAN_ATTR_MEMBER_LIST)", oi->objecttypename);
        return;
    }

    if (SAI_OBJECT_TYPE_DEBUG_COUNTER == oi->objecttype)
    {
        META_LOG_WARN("debug counter object %s not present on any object list (eg. VLAN_MEMBER is present on SAI_VLAN_ATTR_MEMBER_LIST)", oi->objecttypename);
        return;
    }

    META_ASSERT_FAIL("%s not present on any object list (eg. VLAN_MEMBER is present on SAI_VLAN_ATTR_MEMBER_LIST)", oi->objecttypename);
}

void check_reverse_graph_count(
        _In_ const sai_object_type_info_t *oi)
{
    META_LOG_ENTER();

    size_t i = 0;

    if (oi->revgraphmemberscount == 0)
    {
        META_ASSERT_NULL(oi->revgraphmembers);

        return;
    }

    META_ASSERT_NOT_NULL(oi->revgraphmembers);

    for (; i < oi->revgraphmemberscount; ++i)
    {
        META_ASSERT_NOT_NULL(oi->revgraphmembers[i]);
    }

    META_ASSERT_NULL(oi->revgraphmembers[i]);
}

void check_experimental_flag(
        _In_ const sai_object_type_info_t *oi)
{
    META_LOG_ENTER();

    if (oi->objecttype >= SAI_OBJECT_TYPE_MAX)
    {
        META_ASSERT_TRUE(oi->isexperimental, "object %s is expected to be marked as experimental", oi->objecttypename);
    }
    else
    {
        META_ASSERT_FALSE(oi->isexperimental, "object %s is expected to not be marked as experimental", oi->objecttypename);
    }
}

void check_attr_end(
        _In_ const sai_object_type_info_t *oi)
{
    META_LOG_ENTER();

    /*
     * Check if all attributes are in start/end range.
     */

    const sai_attr_metadata_t* const* const meta = oi->attrmetadata;

    META_ASSERT_NOT_NULL(meta);

    size_t index = 0;

    for (; meta[index] != NULL; ++index)
    {
        if (meta[index]->attrid >= oi->attridstart)
            continue;

        if (meta[index]->attrid < oi->attridend)
            continue;

        if (meta[index]->isextensionattr)
            continue;

        META_MD_ASSERT_FAIL(meta[index], "attribute not in START .. END range");
    }
}

void check_single_object_info(
        _In_ const sai_object_type_info_t *oi)
{
    META_LOG_ENTER();

    check_quad_api_pointers(oi);
    check_stats_api_pointers(oi);
    check_object_id_non_object_id(oi);
    check_enum_to_attr_map(oi);
    check_object_ro_list(oi);
    check_reverse_graph_count(oi);
    check_experimental_flag(oi);
    check_attr_end(oi);
}

void check_api_max()
{
    META_LOG_ENTER();

    META_ASSERT_TRUE(SAI_API_MAX <= SAI_API_EXTENSIONS_MAX, "expected api MAX to be less equal than extensions MAX");

    META_ASSERT_TRUE(sai_metadata_enum_sai_api_t.valuescount == SAI_API_EXTENSIONS_MAX,
            "SAI_API_EXTENSIONS_MAX should be equal to number of SAI_API*");
}

void check_backward_comparibility_defines()
{
    META_LOG_ENTER();

    /* check assignments if type matches */

    sai_switch_attr_t sw = SAI_SWITCH_ATTR_SHUTDOWN_REQUEST_NOTIFY;
    sai_hostif_user_defined_trap_type_t trap = SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_NEIGH;
    sai_acl_bind_point_type_t bind = SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF;

    META_ASSERT_TRUE(sw == SAI_SWITCH_ATTR_SWITCH_SHUTDOWN_REQUEST_NOTIFY, "not equal");
    META_ASSERT_TRUE(trap == SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_NEIGHBOR, "not equal");
    META_ASSERT_TRUE(bind == SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE, "not equal");
}

void helper_check_graph_connected(
        _In_ sai_object_type_t ot,
        _Inout_ sai_object_type_t *visited)
{
    META_LOG_ENTER();

    if (visited[ot] == ot)
    {
        return;
    }

    visited[ot] = ot;

    const sai_object_type_info_t *oi = sai_metadata_all_object_type_infos[ot];

    size_t i = 0;

    /* check all attributes */

    for (; i < oi->attrmetadatalength; ++i)
    {
        const sai_attr_metadata_t *md = oi->attrmetadata[i];

        if (!md->isoidattribute)
        {
            continue;
        }

        if (!(md->iscreateonly || md->iscreateandset))
        {
            continue;
        }

        size_t j = 0;

        for (;j < md->allowedobjecttypeslength; ++j)
        {
            helper_check_graph_connected(md->allowedobjecttypes[j], visited);
        }
    }

    for (i = 0; i < oi->structmemberscount; ++i)
    {
        const sai_struct_member_info_t *sm = oi->structmembers[i];

        size_t j = 0;

        for (;j < sm->allowedobjecttypeslength; ++j)
        {
            helper_check_graph_connected(sm->allowedobjecttypes[j], visited);
        }
    }

    for (i = 0; i < oi->revgraphmemberscount; ++i)
    {
        const sai_rev_graph_member_t *rgm = oi->revgraphmembers[i];

        helper_check_graph_connected(rgm->depobjecttype, visited);
    }
}

void check_graph_connected()
{
    META_LOG_ENTER();

    /*
     * Check if all objects are used and are not "disconnected" from the graph.
     */

    sai_object_type_t visited[SAI_OBJECT_TYPE_EXTENSIONS_MAX];

    memset(visited, 0, SAI_OBJECT_TYPE_EXTENSIONS_MAX * sizeof(sai_object_type_t));

    helper_check_graph_connected(SAI_OBJECT_TYPE_PORT, visited);

    size_t i = 0;

    for (; i < SAI_OBJECT_TYPE_EXTENSIONS_MAX; ++i)
    {
        if (visited[i] == (sai_object_type_t)i)
        {
            continue;
        }

        if (sai_metadata_all_object_type_infos[i]->isexperimental)
        {
            /* allow experimental object types to be disconnected from main graph */

            META_LOG_WARN("experimental object %s is disconnected from graph",
                    sai_metadata_all_object_type_infos[i]->objecttypename);

            continue;
        }

        if (SAI_OBJECT_TYPE_DEBUG_COUNTER == i)
        {
            /*
             * Allow debug counters to be disconnected from main graph
             * as use case is by querying base object stats and not by direct reference
             */

            META_LOG_WARN("debug counter object %s is disconnected from graph",
                    sai_metadata_all_object_type_infos[i]->objecttypename);

            continue;
        }

        META_ASSERT_FAIL("object %s is disconnected from graph",
                sai_metadata_all_object_type_infos[i]->objecttypename);
    }
}

void check_get_attr_metadata()
{
    META_LOG_ENTER();

    int count = 0;

    size_t ot = 0;

    for (; ot < SAI_OBJECT_TYPE_EXTENSIONS_MAX; ++ot)
    {
        const sai_attr_metadata_t* const* mda = sai_metadata_attr_by_object_type[ot];

        int idx = 0;

        while (mda[idx])
        {
            const sai_attr_metadata_t* m = mda[idx++];

            const sai_attr_metadata_t* md = sai_metadata_get_attr_metadata(ot, m->attrid);

            META_ASSERT_NOT_NULL(md);
            META_ASSERT_TRUE(m == md, "different attribute found, fatal");

            count++;
        }
    }

    META_ASSERT_TRUE(count > 600, "expected at least 600 attributes");
}

void check_acl_user_defined_field()
{
    META_LOG_ENTER();

    META_ASSERT_TRUE(SAI_ACL_USER_DEFINED_FIELD_ATTR_ID_RANGE > 0, "should be positive");

    META_ASSERT_TRUE(SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MIN + SAI_ACL_USER_DEFINED_FIELD_ATTR_ID_RANGE  ==
            SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MAX, "expected true");

    META_ASSERT_TRUE(SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MIN + SAI_ACL_USER_DEFINED_FIELD_ATTR_ID_RANGE  ==
            SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MAX, "expected true");
}

void check_label_size()
{
    META_LOG_ENTER();

    META_ASSERT_TRUE(sizeof(sai_label_id_t) == sizeof(uint32_t), "label is expected to be 32 bit");
}

void check_switch_notify_list()
{
    META_LOG_ENTER();

    size_t i;

    for (i = 0; i < sai_metadata_switch_notify_attr_count; ++i)
    {
        META_ASSERT_NOT_NULL(sai_metadata_switch_notify_attr[i]);
    }

    /* check for NULL guard */

    META_ASSERT_NULL(sai_metadata_switch_notify_attr[i]);
}

void check_switch_pointers_list()
{
    META_LOG_ENTER();

    size_t i;

    for (i = 0; i < sai_metadata_switch_pointers_attr_count; ++i)
    {
        META_ASSERT_NOT_NULL(sai_metadata_switch_pointers_attr[i]);
    }

    /* check for NULL guard */

    META_ASSERT_NULL(sai_metadata_switch_pointers_attr[i]);
}

void check_defines()
{
    META_LOG_ENTER();

    /*
     * Check if defines are equal to their static values.
     */

    META_ASSERT_TRUE(SAI_METADATA_SWITCH_NOTIFY_ATTR_COUNT == sai_metadata_switch_notify_attr_count, "notify define must be equal");
    META_ASSERT_TRUE(SAI_METADATA_SWITCH_NOTIFY_ATTR_COUNT > 3, "there must be at least 3 notifications defined");
}

void check_object_type_attributes()
{
    META_LOG_ENTER();

    size_t i = 0;

    for (; i < sai_metadata_attr_by_object_type_count; ++i)
    {
        check_single_object_type_attributes(sai_metadata_attr_by_object_type[i]);
    }
}

void check_all_object_infos()
{
    META_LOG_ENTER();

    size_t i = SAI_OBJECT_TYPE_NULL + 1;

    for (; i < SAI_OBJECT_TYPE_EXTENSIONS_MAX; ++i)
    {
        check_single_object_info(sai_metadata_all_object_type_infos[i]);
    }

    META_ASSERT_TRUE((size_t)SAI_OBJECT_TYPE_EXTENSIONS_MAX == (size_t)SAI_OBJECT_TYPE_EXTENSIONS_RANGE_END, "must be equal");
}

void check_ignored_attributes()
{
    META_LOG_ENTER();

    META_ASSERT_NULL(sai_metadata_get_attr_metadata_by_attr_id_name("SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE"));

    const sai_attr_metadata_t* meta = sai_metadata_get_ignored_attr_metadata_by_attr_id_name("SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE");

    if (meta == NULL)
    {
        META_ASSERT_FAIL("Failed to find ignored attribute SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE");
    }

    META_ASSERT_TRUE(strcmp(meta->attridname, "SAI_BUFFER_PROFILE_ATTR_RESERVED_BUFFER_SIZE") == 0,
            "expected attribute was SAI_BUFFER_PROFILE_ATTR_RESERVED_BUFFER_SIZE");
}

#define RANGE_BASE 0x1000

#define SKIP_ENUM(x) if (strcmp(emd->name, #x) == 0) { return; }

void check_enum_object_type(
        _In_ const sai_enum_metadata_t* emd)
{
    META_LOG_ENTER();

    if (emd->objecttype == SAI_OBJECT_TYPE_NULL)
    {
        return;
    }

    const sai_object_type_info_t* oi = sai_metadata_get_object_type_info(emd->objecttype);

    META_ASSERT_NOT_NULL(oi);

    META_ASSERT_TRUE(emd == oi->enummetadata, "should be equal");
}

void check_enum_flags_type_strict(
        _In_ const sai_enum_metadata_t* emd)
{
    META_LOG_ENTER();

    if (emd->flagstype == SAI_ENUM_FLAGS_TYPE_STRICT)
    {
        META_ASSERT_TRUE(emd->containsflags, "must be marked as contains flags");

        int current = 1 << 0;

        size_t i = 0;

        for (; i < emd->valuescount; ++i)
        {
            int val = emd->values[i];

            if (val != current)
            {
                const char*name = emd->valuesnames[i];

                META_ASSERT_FAIL("enum %s value is 0x%x, but probably should be 0x%x to be a flag", name, val, current);
            }

            current = current << 1;
        }

        META_ASSERT_TRUE(emd->values[i] == -1, "missing guard at the end of enum");
    }
}

void check_enum_flags_type_ranges(
        _In_ const sai_enum_metadata_t* emd)
{
    META_LOG_ENTER();

    if (emd->flagstype == SAI_ENUM_FLAGS_TYPE_RANGES)
    {
        META_ASSERT_TRUE(emd->containsflags, "must be marked as contains flags");

        size_t i = 0;

        int32_t start = 0;

        int32_t prev = -1;

        for (; i < emd->valuescount; ++i)
        {
            int val = emd->values[i];

            const char*name = emd->valuesnames[i];

            /* this check can be relaxed, we allow now 16 types of ranges */

            META_ASSERT_TRUE((val < (16*RANGE_BASE)), "range value 0x%x is too high on %s", val, name);

            if ((val != prev + 1) && (val & 0xFFF) && ((val & ~0xFFF) == (prev & ~0xFFF)))
            {
                if ((emd->objecttype == SAI_OBJECT_TYPE_ACL_ENTRY &&
                            val == SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MAX) ||
                        (emd->objecttype == SAI_OBJECT_TYPE_ACL_TABLE &&
                         val == SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MAX))
                {
                    /* this is ACL explicit range which is auto generated by metadata */
                }
                else
                {
                    META_ASSERT_FAIL("value %s = 0x%x not increasing by 1, previous 0x%x", name, val, prev);
                }
            }

            prev = val;

            if (val < start)
            {
                continue;
            }

            while (val >= start)
            {
                start += RANGE_BASE;
            }

            start -= RANGE_BASE;

            if ((val & ~start) != 0)
            {
                META_ASSERT_FAIL("enum %s value is 0x%x, but probably should be 0x%x, missing = SAI_.._RANGE_BASE?", name, val, start);
            }

            start += RANGE_BASE;
        }

        META_ASSERT_TRUE(emd->values[i] == -1, "missing guard at the end of enum");
    }
}

void check_enum_flags_type_free(
        _In_ const sai_enum_metadata_t* emd)
{
    META_LOG_ENTER();

    if (emd->flagstype == SAI_ENUM_FLAGS_TYPE_FREE)
    {
        META_ASSERT_TRUE(emd->containsflags, "must be marked as contains flags");

        size_t i = 0;

        for (; i < emd->valuescount; ++i)
        {
            /* allow all */
        }

        META_ASSERT_TRUE(emd->values[i] == -1, "missing guard at the end of enum");
    }
}

void check_enum_flags_type_none(
        _In_ const sai_enum_metadata_t* emd)
{
    META_LOG_ENTER();

    if (emd->flagstype == SAI_ENUM_FLAGS_TYPE_NONE)
    {
        META_ASSERT_FALSE(emd->containsflags, "contains flags must be false");

        size_t j = 0;

        int last = -1;

        for (; j < emd->valuescount; ++j)
        {
            META_LOG_DEBUG("value: %s", emd->valuesnames[j]);

            int value = emd->values[j];

            META_ASSERT_FALSE(value < 0, "enum values are negative");

            META_ASSERT_TRUE(last < value, "enum values are not increasing");

            if (value != last + 1)
            {
                META_ENUM_ASSERT_FAIL(emd, "values are not increasing by 1: last: %d current: %d, should be marked as @flags?", last, value);
            }

            last = value;
        }

        META_ASSERT_TRUE(emd->values[j] == -1, "missing guard at the end of enum");
    }
}

void check_enum_flags_type(
        _In_ const sai_enum_metadata_t* emd)
{
    META_LOG_ENTER();

    if (emd->containsflags)
    {
        META_ASSERT_TRUE(emd->flagstype != SAI_ENUM_FLAGS_TYPE_NONE,
                "invalid combination of containsflags == true and flagstype == NONE on %s", emd->name);
    }

    if (emd->containsflags == false)
    {
        META_ASSERT_TRUE(emd->flagstype == SAI_ENUM_FLAGS_TYPE_NONE,
                "invalid combination of containsflags == false and flagstype != NONE on %s", emd->name);
    }

    if (emd->flagstype == SAI_ENUM_FLAGS_TYPE_NONE)
        return check_enum_flags_type_none(emd);

    if (emd->flagstype == SAI_ENUM_FLAGS_TYPE_STRICT)
        return check_enum_flags_type_strict(emd);

    if (emd->flagstype == SAI_ENUM_FLAGS_TYPE_RANGES)
        return check_enum_flags_type_ranges(emd);

    if (emd->flagstype == SAI_ENUM_FLAGS_TYPE_FREE)
        return check_enum_flags_type_free(emd);

    META_ASSERT_FAIL("enum %s flags type %d not supported yet, FIXME", emd->name, emd->flagstype);
}

void check_single_enum(
        _In_ const sai_enum_metadata_t* emd)
{
    META_LOG_ENTER();

    check_enum_flags_type(emd);
    check_enum_flags_type_none(emd);
    check_enum_flags_type_strict(emd);
    check_enum_flags_type_ranges(emd);
    check_enum_flags_type_free(emd);
    check_enum_object_type(emd);
}

void check_all_enums()
{
    META_LOG_ENTER();

    size_t i = 0;

    for (; i < sai_metadata_all_enums_count; ++i)
    {
        const sai_enum_metadata_t* emd = sai_metadata_all_enums[i];

        META_LOG_DEBUG("enum: %s", emd->name);

        check_single_enum(emd);
    }
}

void check_sai_version()
{
    META_LOG_ENTER();

    /* SAI_VERSION uses 100 base for each component, so each define must not exceed this value */

    /* Make sure sai version components are assignable to uint32_t */

    uint32_t major = SAI_MAJOR;
    uint32_t minor = SAI_MINOR;
    uint32_t revision = SAI_REVISION;

    META_ASSERT_TRUE((major) < 100, "invalid SAI_MAJOR version: %d", (SAI_MAJOR));
    META_ASSERT_TRUE((minor) < 100, "invalid SAI_MINOR version: %d", (SAI_MINOR));
    META_ASSERT_TRUE((revision) < 100, "invalid SAI_REVISION version: %d", (SAI_REVISION));
}

void check_max_conditions_len()
{
    META_LOG_ENTER();

    META_ASSERT_TRUE(SAI_METADATA_MAX_CONDITIONS_LEN > 0, "must be positive");
}

void check_object_type_extension_max_value()
{
    META_LOG_ENTER();

    /*
     * It can be handy for vendors to encode object type value on single byte
     * in every object it for easy object identification. We assume that we
     * will have no more than 255 objects types on SAI right now.
     */

    META_ASSERT_TRUE(SAI_OBJECT_TYPE_EXTENSIONS_MAX < 256, "max object type can be 255 to be encoded on single byte");
}

void check_global_apis()
{
    META_LOG_ENTER();

    sai_global_apis_t apis;

    apis.api_initialize = NULL;

    META_ASSERT_TRUE(sizeof(apis)/sizeof(void*) > 15, "there should be at least 15 global apis");

    sai_global_api_type_t type = SAI_GLOBAL_API_TYPE_API_INITIALIZE;

    META_ASSERT_TRUE(sizeof(type) >= sizeof(int32_t), "apis type should be at least int32");
}

int main(int argc, char **argv)
{
    debug = (argc > 1);

    SAI_META_LOG_ENTER();

    check_all_enums_name_pointers();
    check_all_enums_values();
    check_enums_ignore_values();
    check_sai_status();
    check_object_type();
    check_attr_by_object_type();
    check_object_type_attributes();
    check_object_infos();
    check_stat_enums();
    check_attr_sorted_by_id_name();
    check_non_object_id_object_types();
    check_non_object_id_object_attrs();
    check_objects_for_loops();
    check_null_object_id();
    check_read_only_attributes();
    check_mixed_object_list_types();
    check_vlan_attributes();
    check_switch_create_only_objects();
    check_switch_attributes();
    check_reverse_graph_for_non_object_id();
    check_acl_table_fields_and_acl_entry_fields();
    check_acl_entry_actions();
    check_api_max();
    check_backward_comparibility_defines();
    check_graph_connected();
    check_get_attr_metadata();
    check_acl_user_defined_field();
    check_label_size();
    check_switch_notify_list();
    check_switch_pointers_list();
    check_defines();
    check_all_object_infos();
    check_ignored_attributes();
    check_all_enums();
    check_sai_version();
    check_max_conditions_len();
    check_object_type_extension_max_value();
    check_global_apis();

    SAI_META_LOG_DEBUG("log test");

    printf("\n [ %s ]\n\n", sai_metadata_get_status_name(SAI_STATUS_SUCCESS));

    SAI_META_LOG_EXIT();

    return 0;
}
