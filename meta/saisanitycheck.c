#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sai.h>
#include "saimetadatautils.h"
#include "saimetadata.h"
#include "saimetadatalogger.h"

typedef struct _defined_attr_t {

    const sai_attr_metadata_t* metadata;

    const struct _defined_attr_t* next;

} defined_attr_t;

bool debug = false;

defined_attr_t* defined_attributes = NULL;

#define META_LOG_INFO(format, ...)\
    if (debug) { printf(format "\n", ##__VA_ARGS__); }

#define META_LOG_WARN(emd, format, ...)\
    fprintf(stderr, "WARN: %s: " format "\n",emd->name, ##__VA_ARGS__);

#define META_WARN(md, format, ...)\
    fprintf(stderr, "WARN: %s: " format "\n",md->attridname, ##__VA_ARGS__);

#define META_WARN_LOG(format, ...)\
    fprintf(stderr, "WARN: " format "\n", ##__VA_ARGS__);

#define META_ASSERT_FAIL(md, format, ...)\
{\
    fprintf(stderr, \
            " ASSERT FAIL(on line %d) %s: " format "\n", \
            __LINE__, \
            md->attridname, \
##__VA_ARGS__); \
    exit(1);\
}

#define META_ENUM_ASSERT_FAIL(md, format, ...)\
{\
    fprintf(stderr, \
            " ASSERT FAIL(on line %d) %s: " format "\n", \
            __LINE__, \
            md->name, \
##__VA_ARGS__); \
    exit(1);\
}

#define META_ASSERT_NOT_NULL(x)\
    if ((x) == NULL) { fprintf(stderr, "assert null failed: '%s' on line %d\n", #x, __LINE__); exit(1); }

#define META_ASSERT_NULL(x)\
    if ((x) != NULL) { fprintf(stderr, "assert not null failed: '%s' on line %d\n", #x, __LINE__); exit(1); }

#define META_ASSERT_TRUE(x, msg)\
    if ((x) == false) { fprintf(stderr, "assert true failed: '%s' on line %d: %s\n", #x, __LINE__, msg); exit(1); }

#define META_ASSERT_FALSE(x, msg)\
    if ((x) == true) { fprintf(stderr, "assert false failed: '%s' on line %d: %s\n", #x, __LINE__, msg); exit(1); }

#define META_FAIL(format, ...)\
    { fprintf(stderr, "assert failed on line %d: " format "\n", __LINE__, ##__VA_ARGS__); exit(1);}

#define META_LOG_ENTER() \
    if (debug) { printf(":> %s\n", __FUNCTION__); }

void check_all_enums_name_pointers()
{
    META_LOG_ENTER();

    size_t i = 0;

    META_ASSERT_TRUE(metadata_all_enums_count > 100, "we need to have some enums");

    for (; i < metadata_all_enums_count; ++i)
    {
        const sai_enum_metadata_t* emd = metadata_all_enums[i];

        META_ASSERT_NOT_NULL(emd);

        META_LOG_INFO("enum: %s", emd->name);

        META_ASSERT_NOT_NULL(emd->name);
        META_ASSERT_NOT_NULL(emd->values);
        META_ASSERT_NOT_NULL(emd->valuesnames);
        META_ASSERT_NOT_NULL(emd->valuesshortnames);

        META_ASSERT_TRUE(emd->valuescount > 0, "enum must have some values");

        size_t j = 0;

        for (; j < emd->valuescount; ++j)
        {
            META_LOG_INFO(" value: %s", emd->valuesnames[j]);

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

    const char* flagenums[] = {
        "sai_acl_entry_attr_t",
        "sai_acl_table_attr_t",
        "sai_attr_flags_t",
        "sai_hostif_trap_type_t",
    };

    size_t i = 0;

    for (; i < sizeof(flagenums)/sizeof(const char*); ++i)
    {
        if (strcmp(emd->name, flagenums[i]) == 0)
        {
            return true;
        }
    }

    return false;
}

void check_all_enums_values()
{
    META_LOG_ENTER();

    size_t i = 0;

    for (; i < metadata_all_enums_count; ++i)
    {
        const sai_enum_metadata_t* emd = metadata_all_enums[i];

        META_LOG_INFO("enum: %s", emd->name);

        size_t j = 0;

        int last = -1;

        if (strcmp(emd->name, "sai_status_t") == 0)
        {
            /* status values are negative */
            continue;
        }

        for (; j < emd->valuescount; ++j)
        {
            META_LOG_INFO(" value: %s", emd->valuesnames[j]);

            int value = emd->values[j];

            META_ASSERT_FALSE(value < 0, "enum values are negative");

            META_ASSERT_TRUE(last < value, "enum values are not increasing");

            if (j == 0)
            {
                if (value != 0)
                {
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
                if (is_flag_enum(emd))
                {
                    /* flags, ok */
                }
                else
                {
                    META_ENUM_ASSERT_FAIL(emd, "values are not increasing by 1: last: %d current: %d", last, value);
                }
            }

            last = emd->values[j];

            META_ASSERT_TRUE(value < 0x10000, "enum value is too big, range?");
        }
    }
}

void check_sai_status()
{
    META_LOG_ENTER();

    META_ASSERT_TRUE(SAI_STATUS_SUCCESS == 0, "success must be zero");
    META_ASSERT_TRUE(metadata_enum_sai_status_t.valuescount > 1, "there must be error codes");

    size_t i = 0;

    int last = 1;

    for (; i < metadata_enum_sai_status_t.valuescount; ++i)
    {
        META_LOG_INFO("status: %s", metadata_enum_sai_status_t.valuesnames[i]);

        int value = metadata_enum_sai_status_t.values[i];

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
}

void check_object_type()
{
    META_LOG_ENTER();

    META_ASSERT_TRUE(SAI_OBJECT_TYPE_NULL == 0, "sai object type null mustbe zero");

    size_t i = 0;

    int last = -1; /* will enforce NULL be first */

    for (; i < metadata_enum_sai_object_type_t.valuescount; ++i)
    {
        META_LOG_INFO("object_type: %s", metadata_enum_sai_object_type_t.valuesnames[i]);

        int value = metadata_enum_sai_object_type_t.values[i];

        META_ASSERT_TRUE(value == last + 1, "object type values must be consecutive numbers");

        last = value;
    }
}

void check_attr_by_object_type()
{
    META_LOG_ENTER();

    META_ASSERT_TRUE(metadata_attr_by_object_type_count == SAI_OBJECT_TYPE_MAX, "invalid object type count in metadata");

    size_t i = 0;

    for (; i < metadata_attr_by_object_type_count; ++i)
    {
        META_LOG_INFO("processing %zu, %s", i, sai_metadata_get_object_type_name((sai_object_type_t)i));

        META_ASSERT_NOT_NULL(metadata_attr_by_object_type[i]);

        const sai_attr_metadata_t ** ot = metadata_attr_by_object_type[i];

        size_t index = 0;

        while (ot[index] != NULL)
        {
            sai_object_type_t current = ot[index]->objecttype;

            META_ASSERT_TRUE(current == i, "object type must be equal on object type list");
            META_ASSERT_TRUE(index < 200, "object defines > 200 attributes, metadata bug?");
            META_ASSERT_TRUE(current > SAI_OBJECT_TYPE_NULL, "object type must be > NULL");
            META_ASSERT_TRUE(current < SAI_OBJECT_TYPE_MAX, "object type must be < MAX");

            /* META_LOG_INFO("processing indexer %lu", index); */

            index++;
        }

        META_LOG_INFO("attr index %zu for %s", index, sai_metadata_get_object_type_name((sai_object_type_t)i));
    }

    META_ASSERT_NULL(metadata_attr_by_object_type[i]);
}

void check_attr_object_type(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if ((md->objecttype <= SAI_OBJECT_TYPE_NULL) ||
            (md->objecttype >= SAI_OBJECT_TYPE_MAX))
    {
        META_ASSERT_FAIL(md, "invalid object type value %d", md->objecttype);
    }
}

void check_attr_value_type_range(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    META_ASSERT_NOT_NULL(sai_metadata_get_enum_value_name(&metadata_enum_sai_attr_value_type_t, md->attrvaluetype));
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
                META_ASSERT_FAIL(md, "valid only attribute can't be mandatory on create, use condition");
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
                META_ASSERT_FAIL(md, "no default value expected, but type provided: %s",
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

                if (sai_metadata_is_acl_field_or_action(md))
                {
                    /*
                     * Default value for acl field or action is not provided
                     * since by default they are disabled, but as TODO we can
                     * add this support and provide default value inside
                     * metadata with disabled parameter and remove this check
                     * here.
                     */

                    break;
                }

                META_ASSERT_FAIL(md, "expected default value, but none provided");
            }

            break;

        case SAI_ATTR_FLAGS_READ_ONLY:

            if (md->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE)
            {
                META_ASSERT_FAIL(md, "read only attribute can't be conditional");
            }

            if (md->validonlytype != SAI_ATTR_CONDITION_TYPE_NONE)
            {
                META_ASSERT_FAIL(md, "read only attribute can't be valid only");
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
                META_ASSERT_FAIL(md, "no default value expected, but type provided: %s",
                        sai_metadata_get_default_value_type_name(md->defaultvaluetype));
            }

            break;

        default:

            META_ASSERT_FAIL(md, "invalid creation flags: 0x%u", md->flags);
    }
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

                        META_ASSERT_FAIL(md, "allow null object id should be set to true since default value is required");
                    }

                    break;

                case SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE:
                    /* default attr value from another attr may not support null */
                    break;

                default:
                    META_ASSERT_FAIL(md, "invalid default value type on object id when default is required");
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
                META_ASSERT_FAIL(md, "object types list is required but it's empty");
            }

            break;

        case SAI_ATTR_VALUE_TYPE_BOOL:
        case SAI_ATTR_VALUE_TYPE_INT8:
        case SAI_ATTR_VALUE_TYPE_INT32:
        case SAI_ATTR_VALUE_TYPE_INT8_LIST:
        case SAI_ATTR_VALUE_TYPE_UINT8_LIST:
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
        case SAI_ATTR_VALUE_TYPE_CHARDATA:
        case SAI_ATTR_VALUE_TYPE_UINT32_RANGE:
        case SAI_ATTR_VALUE_TYPE_UINT32_LIST:
        case SAI_ATTR_VALUE_TYPE_QOS_MAP_LIST:
        case SAI_ATTR_VALUE_TYPE_TUNNEL_MAP_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_CAPABILITY:

        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_BOOL:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST:

        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6:

            if (md->allowedobjecttypes != NULL)
            {
                META_ASSERT_FAIL(md, "allowed object types defined for non object type");
            }

            break;

        default:
            META_ASSERT_FAIL(md, "attr value type is not supported, FIXME");
    }
}

void check_attr_allowed_object_types(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (md->allowedobjecttypeslength != 0 && md->allowedobjecttypes == NULL)
    {
        META_ASSERT_FAIL(md, "allowed object type len is specified but pointer is NULL");
    }

    if (md->allowedobjecttypeslength == 0 && md->allowedobjecttypes != NULL)
    {
        META_ASSERT_FAIL(md, "allowed object type len zero, but but pointer to objects is specified");
    }

    if (md->allowedobjecttypes == NULL)
    {
        return;
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

            META_ASSERT_FAIL(md, "allowed object types should be empty on this attr value type");
    }

    /*
     * check if allowed object types are in range
     * they may repeat, but we can also check that
     */

    size_t i = 0;

    for (; i < md->allowedobjecttypeslength; ++i)
    {
        sai_object_type_t ot = md->allowedobjecttypes[i];

        if ((ot <= SAI_OBJECT_TYPE_NULL) ||
                (ot >= SAI_OBJECT_TYPE_MAX))
        {
            META_ASSERT_FAIL(md, "invalid allowed object type: %d", ot);
        }

        const sai_object_type_info_t* info = sai_all_object_type_infos[ot];

        META_ASSERT_NOT_NULL(info);

        if (info->isnonobjectid)
        {
            META_ASSERT_FAIL(md, "non object id can't be used as object id: %d", ot);
        }

        if (ot == SAI_OBJECT_TYPE_SWITCH)
        {
            /* switch object type is ment to be used only in non object id struct types */

            META_ASSERT_FAIL(md, "switch object type can't be used as object type in any attribute");
        }
    }
}

void check_attr_default_required(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    bool requiredefault = (!HAS_FLAG_MANDATORY_ON_CREATE(md->flags)) &&
        (HAS_FLAG_CREATE_ONLY(md->flags) || HAS_FLAG_CREATE_AND_SET(md->flags));

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

        if (sai_metadata_is_acl_field_or_action(md))
        {
            /*
             * By default we assume that default acl field or action is
             * disabled and default value is not provided.
             */

            return;
        }

        META_ASSERT_FAIL(md, "expected default value, but none provided");
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
                     * calse.
                     */

                    break;
                }
            }

            if (md->defaultvalue == NULL)
            {
                META_ASSERT_FAIL(md, "default value type is provided, but default value pointer is NULL");
            }

            break;

        case SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE:
        case SAI_DEFAULT_VALUE_TYPE_ATTR_RANGE:
        case SAI_DEFAULT_VALUE_TYPE_SWITCH_INTERNAL:
        case SAI_DEFAULT_VALUE_TYPE_VENDOR_SPECIFIC:
        case SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST:

            if (md->defaultvalue != NULL)
            {
                META_ASSERT_FAIL(md, "default value type is provided, but default value pointer is not NULL");
            }

            break;

        default:

            META_ASSERT_FAIL(md, "unknown default value type %d", md->defaultvaluetype);
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
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID:

        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:

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
            break;

        case SAI_ATTR_VALUE_TYPE_INT8_LIST:
        case SAI_ATTR_VALUE_TYPE_UINT32_LIST:
        case SAI_ATTR_VALUE_TYPE_INT32_LIST:
        case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
        case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST:

            if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST)
            {
                break;
            }

            META_ASSERT_FAIL(md, "default value list is needed on this attr value type but list is NULL");

            break;

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

            META_ASSERT_FAIL(md, "default value list is needed on this attr value type but list is NULL");

            break;

        case SAI_ATTR_VALUE_TYPE_POINTER:
            break;

        default:

            META_ASSERT_FAIL(md, "default value is required but this attr value type is not supported yet");
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
                META_ASSERT_FAIL(md, "attribute is marked as enum, but attr value type is not enum compatible");
        }
    }

    if (md->isenum && md->isenumlist)
    {
        META_ASSERT_FAIL(md, "attribute can't be marked as enum and enum list");
    }

    if ((md->isenum || md->isenumlist) && md->enummetadata == NULL)
    {
        META_ASSERT_FAIL(md, "is marked enum but missing enum metadata");
    }

    if (!(md->isenum || md->isenumlist) && md->enummetadata != NULL)
    {
        META_ASSERT_FAIL(md, "is not marked enum but has defined enum type string");
    }

    if ((md->isenum || md->isenumlist) && md->enummetadata->valuescount == 0)
    {
        META_ASSERT_FAIL(md, "is marked enum but missing enum allowed values");
    }

    bool requiredefault = (!HAS_FLAG_MANDATORY_ON_CREATE(md->flags)) &&
        (HAS_FLAG_CREATE_ONLY(md->flags) || HAS_FLAG_CREATE_AND_SET(md->flags));

    if (requiredefault && md->isenum)
    {
        if (md->defaultvalue == NULL)
        {
            if (sai_metadata_is_acl_field_or_action(md))
            {
                /*
                 * Default value on acl field or action is by default disabled
                 * so we just skip it here.
                 */

                return;
            }

            META_ASSERT_FAIL(md, "marked as enum, and require default, but not provided");
        }

        int32_t enumdefault = md->defaultvalue->s32;

        if (sai_metadata_get_enum_value_name(md->enummetadata, enumdefault) == NULL)
        {
            META_ASSERT_FAIL(md, "default enum value %d is not present on enum allowed values (%s)", enumdefault, md->enummetadata->name);
        }
    }

    if (requiredefault && md->isenumlist)
    {
        if (md->defaultvalue != NULL)
        {
            META_ASSERT_FAIL(md, "default values on enum list are not supported yet");
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

            /* check conditions/cretion flags? */
            break;

        case SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE:

            {
                const sai_attr_metadata_t* def = sai_metadata_get_attr_metadata(md->defaultvalueobjecttype, md->defaultvalueattrid);

                if (def == NULL)
                {
                    META_ASSERT_FAIL(md, "attr value can't be found");
                }

                if (md->attrvaluetype != def->attrvaluetype)
                {
                    META_ASSERT_FAIL(md, "default attr value type is different");
                }

                break;
            }

        case SAI_DEFAULT_VALUE_TYPE_ATTR_RANGE:

            {
                const sai_attr_metadata_t* def = sai_metadata_get_attr_metadata(md->defaultvalueobjecttype, md->defaultvalueattrid);

                if (def == NULL)
                {
                    META_ASSERT_FAIL(md, "attr range can't be found");
                }

                META_ASSERT_FAIL(md, "attr value attribute value range not supported yet");

                break;
            }

        case SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST:

            switch (md->attrvaluetype)
            {
                case SAI_ATTR_VALUE_TYPE_UINT32_LIST:
                case SAI_ATTR_VALUE_TYPE_INT32_LIST:
                case SAI_ATTR_VALUE_TYPE_UINT8_LIST:
                case SAI_ATTR_VALUE_TYPE_INT8_LIST:
                case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
                    break;

                default:

                    META_ASSERT_FAIL(md, "default empty list specified, but attribute is not list");
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

                    META_ASSERT_FAIL(md, "vendor specific not allowed on this type");
            }

            break;

        case SAI_DEFAULT_VALUE_TYPE_SWITCH_INTERNAL:

            if (md->flags != SAI_ATTR_FLAGS_READ_ONLY)
            {
                META_ASSERT_FAIL(md, "default internal currently can be set only on read only objects");
            }

            if (md->objecttype != SAI_OBJECT_TYPE_SWITCH)
            {
                /*
                 * This can be later relaxed to be set on PORTs since they have
                 * by default queues created.
                 */

                META_ASSERT_FAIL(md, "default internal can be only set on switch object type");
            }

            switch (md->attrvaluetype)
            {
                case SAI_ATTR_VALUE_TYPE_OBJECT_ID:
                case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
                    break;

                default:

                    META_ASSERT_FAIL(md, "invalid attribute value type specified: %d", md->attrvaluetype);
            }

            break;

        default:

            META_ASSERT_FAIL(md, "invalid default value type specified: %d", md->defaultvaluetype);
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
            break;

        default:

            META_ASSERT_FAIL(md, "invalid condition type specified: %d", md->conditiontype);
    }

    bool conditional = md->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE;

    if (!conditional && md->conditions != NULL)
    {
        META_ASSERT_FAIL(md, "not conditional but conditions specified");
    }

    if (!conditional)
    {
        return;
    }

    if (md->conditions == NULL)
    {
        META_ASSERT_FAIL(md, "marked as conditional but no conditions specified");
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

            META_ASSERT_FAIL(md, "marked as conditional, but invalid creation flags: 0x%u", md->flags);
    }

    /* condition must be the same object type as attribue we check */

    size_t index = 0;

    for (; index < md->conditionslength; ++index)
    {
        const sai_attr_condition_t* c = md->conditions[index];

        if (c->attrid == md->attrid)
        {
            META_ASSERT_FAIL(md, "conditional attr id %d is the same as condition attribute", c->attrid);
        }

        const sai_attr_metadata_t* cmd = sai_metadata_get_attr_metadata(md->objecttype, c->attrid);

        if (cmd == NULL)
        {
            META_ASSERT_FAIL(md, "conditional attribute id %d was not defined yet in metadata", c->attrid);
        }

        switch (cmd->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_BOOL:

                META_LOG_INFO("attr id: %d cond.bool: %d", c->attrid, c->condition.booldata);

                break;

            case SAI_ATTR_VALUE_TYPE_INT32:

                if (!cmd->isenum)
                {
                    META_ASSERT_FAIL(md, "conditional attribute %d is not enum type", cmd->attrid);
                }

                META_LOG_INFO("attr id: %d cond.s32: %d ", c->attrid, c->condition.s32);

                /* check if condition enum is in condition attribute range */

                if (sai_metadata_get_enum_value_name(cmd->enummetadata, c->condition.s32) == NULL)
                {
                    META_ASSERT_FAIL(md, "condition enum %d not found on condition attribute enum range", c->condition.s32);
                }

                break;

            default:

                META_ASSERT_FAIL(md, "attr value type %d of conditional attribute is not supported yet", cmd->attrvaluetype);

        }

        if (cmd->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE)
        {
            META_ASSERT_FAIL(md, "conditional attibute is also conditional, not allowed");
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

                META_ASSERT_FAIL(md, "conditional attribute must be create only");
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
            break;

        default:

            META_ASSERT_FAIL(md, "invalid validonly type specified: %d", md->validonlytype);
    }

    bool conditional = md->validonlytype != SAI_ATTR_CONDITION_TYPE_NONE;

    if (!conditional && md->validonly != NULL)
    {
        META_ASSERT_FAIL(md, "not validonly but validonly specified");
    }

    if (!conditional)
    {
        return;
    }

    if (md->validonly == NULL)
    {
        META_ASSERT_FAIL(md, "marked as validonly but no validonly specified");
    }

    switch ((int)md->flags)
    {
        case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY | SAI_ATTR_FLAGS_KEY:
        case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY:
        case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_AND_SET:

            META_ASSERT_FAIL(md, "valid only attribute can't be mandatory on create, use condition");
            break;

        case SAI_ATTR_FLAGS_CREATE_ONLY:

            /*
             * In generral valid only attribute should be used only on
             * CREATE_AND_SET flags, since when attribute is CREATE_ONLY it has
             * default value and it can't be changed anywa, and entire purpose
             * of valid only attribute is to allow change during runtime.
             *
             * Wthen attribute CREATE_ONLY is marked as valid only is more like
             * indication that this value will be used in that specific case
             * but you won't be able to change it anyway.
             */

            META_WARN(md, "marked as valid only, on flags CREATE_ONLY, default value is present, should this be CREATE_AND_SET?");

        case SAI_ATTR_FLAGS_CREATE_AND_SET:


            /* ok */

            break;

        case SAI_ATTR_FLAGS_READ_ONLY:

            META_ASSERT_FAIL(md, "read only attribute can't be valid only");
            break;

        default:

            META_ASSERT_FAIL(md, "marked as validonly, but invalid creation flags: 0x%u", md->flags);
    }

    if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_NONE)
    {
        /*
         * In struct defaultvalue member can be NULL for some other default
         * value types like empty list or internal etc. Default value is
         * provided for CONST only.
         */

        META_ASSERT_FAIL(md, "expected default value on vlaid only attribute, but none provided");
    }

    /* condition must be the same object type as attribue we check */

    size_t index = 0;

    for (; index < md->validonlylength; ++index)
    {
        const sai_attr_condition_t* c = md->validonly[index];

        if (c->attrid == md->attrid)
        {
            META_ASSERT_FAIL(md, "validonly attr id %d is the same as validonly attribute", c->attrid);
        }

        const sai_attr_metadata_t* cmd = sai_metadata_get_attr_metadata(md->objecttype, c->attrid);

        if (cmd == NULL)
        {
            META_ASSERT_FAIL(md, "validonly attribute id %d was not defined yet in metadata", c->attrid);
        }

        switch (cmd->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_BOOL:

                META_LOG_INFO("attr id: %d cond.bool: %d", c->attrid, c->condition.booldata);

                break;

            case SAI_ATTR_VALUE_TYPE_INT32:

                if (!cmd->isenum)
                {
                    META_ASSERT_FAIL(md, "validonly attribute %d is not enum type", cmd->attrid);
                }

                META_LOG_INFO("attr id: %d cond.s32: %d ", c->attrid, c->condition.s32);

                /* check if condition enum is in condition attribute range */

                if (sai_metadata_get_enum_value_name(cmd->enummetadata, c->condition.s32) == NULL)
                {
                    META_ASSERT_FAIL(md, "validonly enum %d not found on validonly attribute enum range", c->condition.s32);
                }

                break;

            default:

                META_ASSERT_FAIL(md, "attr value type %d of validonly attribute is not supported yet", cmd->attrvaluetype);
        }

        /*
         * TODO can validonly attribute depend on condition attribute which is not provided?
         * TODO can validonly depend on other validonly?
         */

        if (cmd->validonlytype != SAI_ATTR_CONDITION_TYPE_NONE)
        {
            if (md->objecttype == SAI_OBJECT_TYPE_TUNNEL && md->attrid == SAI_TUNNEL_ATTR_ENCAP_GRE_KEY)
            {
                /*
                 * For this case GRE_KEY is depending on GRE_KEY_VALID which is
                 * also valid only for other cases we don't allow valid only to
                 * be depending on valid only but maybe this is false
                 * assumption.
                 */
            }
            else
            {
                META_ASSERT_FAIL(md, "validonly attibute is also validonly attribute, not allowed");
            }
        }

        if (cmd->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE)
        {
            META_ASSERT_FAIL(md, "conditional attibute is also conditional, not allowed");
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

                META_ASSERT_FAIL(cmd, "valid only condition attribute has invalid flags");
        }
    }

    if ((md->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE ) &&
            (md->validonlytype != SAI_ATTR_CONDITION_TYPE_NONE ))
    {
        META_ASSERT_FAIL(md, "attribute is conditional and valid only, not supported");
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
            META_ASSERT_FAIL(md, "marked as enum list but wrong attr value type");
        }

        if (md->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE)
        {
            META_ASSERT_FAIL(md, "conditional enum list not supported yet");
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
            META_ASSERT_FAIL(md, "marked as enum list but wrong attr value type");
        }

        if (md->validonlytype != SAI_ATTR_CONDITION_TYPE_NONE)
        {
            /*
             * This restriction can be removed if necessary so far i don't see
             * any enum list that are marked as valid only.
             */

            META_ASSERT_FAIL(md, "validonly enum list not supported yet");
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

                META_ASSERT_FAIL(md, "allow null object is set but attr value type is wrong");
        }

        /*
         * Object SAI_ATTR_VALUE_TYPE_POINTER should be allowed null pointer by
         * default pointers received from SAI should be only via query api.
         */

        if (md->allowedobjecttypeslength == 0)
        {
            META_ASSERT_FAIL(md, "allow null object is set but allowed object types is empty");
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

        if (ot > SAI_OBJECT_TYPE_NULL &&
                ot < SAI_OBJECT_TYPE_MAX)
        {
            continue;
        }

        META_ASSERT_FAIL(md, "not allowed object type %d on list", ot);
    }

    if (md->allowrepetitiononlist || md->allowmixedobjecttypes || md->allowemptylist)
    {
        switch (md->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
                break;

            default:

                META_ASSERT_FAIL(md, "allow null object is set but attr value type is wrong");
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

                META_ASSERT_FAIL(md, "get save not supported on %s", md->attridname);
        }
    }
}

void check_attr_key(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    if (HAS_FLAG_KEY(md->flags))
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

                META_ASSERT_FAIL(md, "marked as key, but have invalid attr value type (list)");

            case SAI_ATTR_VALUE_TYPE_OBJECT_ID:

                if (md->objecttype == SAI_OBJECT_TYPE_QUEUE && md->attrid == SAI_QUEUE_ATTR_PORT)
                {
                    /*
                     * This is also special case, OBJECT_ID at should not be a
                     * KEY in any attribute, this is TODO action to get rid of
                     * this kind of dependency.
                     */

                    break;
                }

                META_ASSERT_FAIL(md, "marked as key, but have invalid attr value type (object id)");

            case SAI_ATTR_VALUE_TYPE_INT32:
            case SAI_ATTR_VALUE_TYPE_UINT32:
            case SAI_ATTR_VALUE_TYPE_UINT8:
            case SAI_ATTR_VALUE_TYPE_UINT16:
                break;

            default:

                META_ASSERT_FAIL(md, "marked as key, but have invalid attr value type");
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
                        break;

                    default:

                        META_ASSERT_FAIL(md, "acl field data used on udf match can be only primitive type");
                }

                break;
            }

            META_ASSERT_FAIL(md, "acl field may only be set on acl field and udf match");

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
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:
        case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:

            if (md->objecttype != SAI_OBJECT_TYPE_ACL_ENTRY ||
                    md->attrid < SAI_ACL_ENTRY_ATTR_ACTION_START ||
                    md->attrid > SAI_ACL_ENTRY_ATTR_ACTION_END)
            {
                META_ASSERT_FAIL(md, "acl action may only be set on acl action");
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
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
                case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST:
                    break;

                default:
                    META_ASSERT_FAIL(md, "invalid attr value type for acl field");
            }
        }

        if (md->attrid >= SAI_ACL_ENTRY_ATTR_ACTION_START &&
                md->attrid <= SAI_ACL_ENTRY_ATTR_ACTION_END)
        {
            switch (md->attrvaluetype)
            {
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:
                case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
                    break;

                default:
                    META_ASSERT_FAIL(md, "invalid attr value type for acl action");
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
        if (md->attrvaluetype != SAI_ATTR_VALUE_TYPE_UINT16)
        {
            META_ASSERT_FAIL(md, "marked as vlan, but fiels has wrong attr value type");
        }
    }
}

void check_condition_in_range(
        _In_ const sai_attr_metadata_t* md,
        _In_ size_t length,
        _In_ const sai_attr_condition_t **conditions,
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

        META_ASSERT_FAIL(md, "has condition depending on acl field / action, not allowed");
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
                META_ASSERT_FAIL(md, "acl table field has conditions, not allowed");
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
                META_ASSERT_FAIL(md, "acl entry field has conditions, not allowed");
            }
        }

        if (md->attrid >= SAI_ACL_ENTRY_ATTR_ACTION_START &&
            md->attrid >= SAI_ACL_ENTRY_ATTR_ACTION_END)
        {
            if (md->conditionslength != 0 || md->validonlylength != 0)
            {
                META_ASSERT_FAIL(md, "acl entry action has conditions, not allowed");
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

        const sai_object_type_info_t *oi = sai_all_object_type_infos[depobjecttype];

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

                META_ASSERT_FAIL(md, "This is attribute, it can't be defined in struct member");
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
                        META_LOG_INFO("dep %s ot %s attr %s\n",
                                metadata_enum_sai_object_type_t.valuesnames[depobjecttype],
                                metadata_enum_sai_object_type_t.valuesnames[md->objecttype],
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
                META_ASSERT_FAIL(md, "attribute was already declared");
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
        META_ASSERT_FAIL(md, "attribute marked as acl capability should be READ_ONLY");
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

    if ((md->isaclfield | md->isaclaction) != sai_metadata_is_acl_field_or_action(md))
    {
        META_ASSERT_FAIL(md, "isaclfield or isaclaction don't match utils method");
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
     * Purpose of this test it to find attributes on objects exisring already
     * on the switch with attributes that are mandatory on create and create
     * and set.  Those attributes can be changed by user fro previous value,
     * and this causes problem for comparison logic to bring those objects to
     * default value. We need to store those initial values of created objects
     * somewhere.
     */

    if (sai_all_object_type_infos[md->objecttype]->isnonobjectid)
    {
        return;
    }

    switch (md->objecttype)
    {
        /*
         * Those objects are not existing on the switch by default user needs
         * to crete them.
         */

        case SAI_OBJECT_TYPE_SAMPLEPACKET:
        case SAI_OBJECT_TYPE_HOSTIF_TRAP:
        case SAI_OBJECT_TYPE_MIRROR_SESSION:
            return;

            /*
             * Those objects are objects which exist already on the switch, to bring
             * back them to default state by comparison logic, we should not have any
             * MANDATORY_ON_CREATE attributes on them.
             */

        case SAI_OBJECT_TYPE_VLAN_MEMBER:
        case SAI_OBJECT_TYPE_VLAN:
        case SAI_OBJECT_TYPE_HASH:
        case SAI_OBJECT_TYPE_STP:
        case SAI_OBJECT_TYPE_VIRTUAL_ROUTER:
        case SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP:
        case SAI_OBJECT_TYPE_SWITCH:
        case SAI_OBJECT_TYPE_PORT:
        case SAI_OBJECT_TYPE_QUEUE:
        case SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP:
        case SAI_OBJECT_TYPE_SCHEDULER_GROUP:
        default:
            break;
    }

    if (!HAS_FLAG_MANDATORY_ON_CREATE(md->flags) || !HAS_FLAG_CREATE_AND_SET(md->flags))
    {
        return;
    }

    /*
     * If attribute is mandatory on create and create and set then there is no
     * default value on created object, and user can change it's value so in
     * comparison logic we will need to mantain this state somewhere as
     * default.
     */

    /*
     * Currently we are limiting value types on existing objects that are
     * mandatory on create to primitive values.
     */

    switch (md->attrvaluetype)
    {
        case SAI_ATTR_VALUE_TYPE_UINT32:
        case SAI_ATTR_VALUE_TYPE_INT32:
        case SAI_ATTR_VALUE_TYPE_INT8:

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
             * since we will not be albe to bring it to default.
             */

            META_WARN_LOG("Default value needs to be stored %s", md->attridname);

            break;

        default:

            META_ASSERT_FAIL(md, "not supported attr value type on existing object");
    }

    /* TODO there is default .1Q Bridge present */
}

void check_single_attribute(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    META_LOG_INFO("performing metadata sanity check: object type %d, attr id: %d", md->objecttype, md->attrid);

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

    define_attr(md);
}

void check_single_object_type_attributes(
        _In_ const sai_attr_metadata_t** attributes)
{
    META_LOG_ENTER();

    size_t index = 0;

    for (; attributes[index] != NULL; ++index)
    {
        check_single_attribute(attributes[index]);
    }
}

void check_object_infos()
{
    META_LOG_ENTER();

    size_t i = SAI_OBJECT_TYPE_NULL;

    for (; i <= SAI_OBJECT_TYPE_MAX; ++i)
    {
        const sai_object_type_info_t* info = sai_all_object_type_infos[i];

        if (info == NULL)
        {
            continue;
        }

        META_ASSERT_TRUE(info->objecttype == i, "object type mismatch");

        META_LOG_INFO("processing object type: %s", sai_metadata_get_object_type_name((sai_object_type_t)i));

        META_ASSERT_TRUE(info->attridstart == 0, "attribute enum start should be zero");
        META_ASSERT_TRUE(info->attridend > 0, "attribute enum end must be > 0");

        const sai_attr_metadata_t** const meta = info->attrmetadata;

        META_ASSERT_NOT_NULL(meta);

        size_t index = 0;

        int last = -1;

        /* check all listed attributes under this object type */

        for (; meta[index] != NULL; ++index)
        {
            const sai_attr_metadata_t* am = meta[index];

            META_ASSERT_TRUE((int)am->attrid >= 0, "attribute must be non negative");
            META_ASSERT_TRUE(last < (int)am->attrid, "attributes are not incresing");

            if (last + 1 != (int)am->attrid)
            {
                if (info->objecttype != SAI_OBJECT_TYPE_ACL_ENTRY &&
                        info->objecttype != SAI_OBJECT_TYPE_ACL_TABLE)
                {
                    META_ASSERT_FAIL(am, "attr id is not increasing by 1: prev %d, curr %d", last, am->attrid);
                }
            }

            last = (int)am->attrid;

            if (am->attrid >= info->attridstart &&
                    am->attrid < info->attridend)
            {
                continue;
            }

            META_ASSERT_FAIL(am, "attr is is not in start .. end range");
        }

        META_ASSERT_NOT_NULL(info->enummetadata);

        if (index != info->attridend)
        {
            if (is_flag_enum(info->enummetadata))
            {
                /* ok, flags */
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

    for (; i <= SAI_OBJECT_TYPE_MAX; ++i)
    {
        const sai_object_type_info_t* info = sai_all_object_type_infos[i];

        if (info == NULL)
        {
            continue;
        }

        if (!info->isnonobjectid)
        {
            if (info->structmemberscount != 0 ||
                    info->structmembers != NULL)
            {
                META_FAIL("object type %zu is non object id but struct members defined", i);
            }

            continue;
        }

        META_ASSERT_TRUE(info->structmemberscount != 0, "non object id should have members defined");
        META_ASSERT_NOT_NULL(info->structmembers);

        /* check each member of the struct */

        size_t j = 0;

        int member_supports_switch_id = 0;

        for (; j < info->structmemberscount; ++j)
        {
            META_ASSERT_NOT_NULL(info->structmembers[j]);

            const sai_struct_member_info_t *m = info->structmembers[j];

            META_ASSERT_NOT_NULL(m->membername);

            switch (m->membervaluetype)
            {
                case SAI_ATTR_VALUE_TYPE_MAC:
                case SAI_ATTR_VALUE_TYPE_INT32:
                case SAI_ATTR_VALUE_TYPE_UINT16:
                case SAI_ATTR_VALUE_TYPE_IP_ADDRESS:
                case SAI_ATTR_VALUE_TYPE_IP_PREFIX:
                case SAI_ATTR_VALUE_TYPE_OBJECT_ID:
                    break;

                default:

                    /*
                     * On struct members only primitive types should be
                     * supported so no other structs or lists.
                     */

                    META_FAIL("struct member %s have invalid value type %d", m->membername, m->membervaluetype);
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

                /*
                 * this check can be relaxed in the future, but currently
                 * supporting only one object type in non object id make sense
                 */

                META_ASSERT_TRUE(m->allowedobjecttypeslength == 1, "currently struct member object id, should specify only one object type");

                size_t k = 0;

                for (; k < m->allowedobjecttypeslength; k++)
                {
                    sai_object_type_t ot = m->allowedobjecttypes[k];

                    if (ot >= SAI_OBJECT_TYPE_NULL && ot <= SAI_OBJECT_TYPE_MAX)
                    {
                        if (ot == SAI_OBJECT_TYPE_SWITCH)
                        {
                            /*
                             * to make struct object type complete, at least
                             * one struct member should be type of switch
                             */

                            member_supports_switch_id++;
                        }

                        /* non object id struct can't contain object id which is also non object id */

                        const sai_object_type_info_t* sinfo = sai_all_object_type_infos[ot];

                        META_ASSERT_NOT_NULL(sinfo);

                        if (sinfo->isnonobjectid)
                        {
                            META_FAIL("struct member %s of non object id type can't be used as object id in non object id struct: %s",
                                    m->membername,
                                    sai_metadata_get_object_type_name(ot));
                        }

                        continue;
                    }

                    META_FAIL("invalid object type specified on file %s: %d", m->membername, ot);
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

    for (; i <= SAI_OBJECT_TYPE_MAX; ++i)
    {
        const sai_object_type_info_t* info = sai_all_object_type_infos[i];

        if (info == NULL || !info->isnonobjectid)
        {
            continue;
        }

        const sai_attr_metadata_t** meta = info->attrmetadata;

        META_ASSERT_NOT_NULL(meta);

        size_t idx = 0;

        /* iterate all attributes on non object id type */

        for (; meta[idx] != NULL; ++idx)
        {
            const sai_attr_metadata_t* m = meta[idx];

            META_ASSERT_NOT_NULL(m);

            switch ((int)m->flags)
            {
                case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_AND_SET:
                case SAI_ATTR_FLAGS_CREATE_AND_SET:
                    break;

                default:

                    META_ASSERT_FAIL(m, "non object id attribute has invalid flags: 0x%x (should be CREATE_AND_SET)", m->flags);
            }
        }
    }
}

void check_attr_sorted_by_id_name()
{
    META_LOG_ENTER();

    size_t i = 0;

    const char *last = "AAA";

    META_ASSERT_TRUE(metadata_attr_sorted_by_id_name_count > 500,
            "there should be at least 500 attributes in total");

    for (; i < metadata_attr_sorted_by_id_name_count; ++i)
    {
        const sai_attr_metadata_t *am = metadata_attr_sorted_by_id_name[i];

        META_ASSERT_NOT_NULL(am);

        const char *name = am->attridname;

        if (strcmp(last, name) >= 0)
        {
            META_ASSERT_FAIL(am, "attribute id name in not sorted alphabetical");
        }

        META_ASSERT_TRUE(strncmp(name, "SAI_", 4) == 0, "all attributes should start with SAI_");

        last = name;
    }

    META_ASSERT_NULL(metadata_attr_sorted_by_id_name[i]);

    /* check search */

    for (i = 0; i < metadata_attr_sorted_by_id_name_count; ++i)
    {
        const sai_attr_metadata_t *am = metadata_attr_sorted_by_id_name[i];

        META_LOG_INFO("search for %s", am->attridname);

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

    META_WARN_LOG("LOOP DETECTED on object type: %s",
            metadata_enum_sai_object_type_t.valuesnames[info->objecttype]);

    for (; levelidx < level; ++levelidx)
    {
        sai_object_type_t ot = visited[levelidx];

        const char* ot_name = metadata_enum_sai_object_type_t.valuesnames[ot];

        const sai_attr_metadata_t* m = sai_metadata_get_attr_metadata(ot, attributes[levelidx]);

        META_WARN_LOG(" %s: %s", ot_name, m->attridname);
    }

    META_WARN_LOG(" -> %s", metadata_enum_sai_object_type_t.valuesnames[info->objecttype]);

    if (level >= 0)
    {
        META_FAIL("LOOP is detected, we can't have loops in graph, please fix attributes");
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

    const sai_attr_metadata_t** meta = info->attrmetadata;

    META_ASSERT_NOT_NULL(meta);

    size_t idx = 0;

    /* iterate all attributes on non object id type */

    for (; meta[idx] != NULL; ++idx)
    {
        const sai_attr_metadata_t* m = meta[idx];

        META_ASSERT_NOT_NULL(m);

        if (HAS_FLAG_READ_ONLY(m->flags))
        {
            /* skip read only attributes since with those we will have loops for sure */

            continue;
        }

        /* skip known loops */

        if (m->objecttype == SAI_OBJECT_TYPE_PORT)
        {
            if (m->attrid == SAI_PORT_ATTR_EGRESS_MIRROR_SESSION ||
                m->attrid == SAI_PORT_ATTR_INGRESS_MIRROR_SESSION ||
                m->attrid == SAI_PORT_ATTR_EGRESS_BLOCK_PORT_LIST)
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

        switch (m->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID:
            case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:
            case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_OBJECT_ID:

                {
                    size_t j = 0;

                    for (; j < m->allowedobjecttypeslength; ++j)
                    {
                        const sai_object_type_info_t* next = sai_all_object_type_infos[ m->allowedobjecttypes[j] ];

                        check_objects_for_loops_recursive(next, visited, attributes, level + 1);
                    }
                }

                break;

            default:
                break;
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
                const sai_object_type_info_t* next = sai_all_object_type_infos[ m->allowedobjecttypes[k] ];

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

    sai_object_type_t visited_objects[SAI_OBJECT_TYPE_MAX];
    uint32_t visited_attributes[SAI_OBJECT_TYPE_MAX];

    size_t i = SAI_OBJECT_TYPE_NULL;

    for (; i <= SAI_OBJECT_TYPE_MAX; ++i)
    {
        const sai_object_type_info_t* info = sai_all_object_type_infos[i];

        if (info == NULL)
        {
            continue;
        }

        memset(visited_objects, 0, SAI_OBJECT_TYPE_MAX * sizeof(sai_object_type_t));
        memset(visited_attributes, 0, SAI_OBJECT_TYPE_MAX * sizeof(uint32_t));

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

    for (; i <= SAI_OBJECT_TYPE_MAX; ++i)
    {
        const sai_object_type_info_t* info = sai_all_object_type_infos[i];

        if (info == NULL)
        {
            continue;
        }

        size_t index = 0;

        /* check all listed attributes under this object type */

        int non_read_only_count = 0;

        const sai_attr_metadata_t** const meta = info->attrmetadata;

        for (; meta[index] != NULL; ++index)
        {
            const sai_attr_metadata_t* m = meta[index];

            if (!HAS_FLAG_READ_ONLY(m->flags))
            {
                non_read_only_count++;
            }
        }

        if (index < 1)
        {
            META_FAIL("object %s must define at least 1 attribute",
                    sai_metadata_get_object_type_name((sai_object_type_t)i));
        }

        if (non_read_only_count == 0)
        {
            /*
             * currently we have some objects with only read only
             * attributes, we for now we just warn here until this
             * issue will be resolved.
             */

            META_WARN_LOG("object %s has only READ_ONLY attributes",
                    metadata_enum_sai_object_type_t.valuesnames[i]);
        }
    }
}

void check_mixed_object_list_types()
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to find out if any of object id lists supports
     * multiple object types at the same time.  For now this abbility will not
     * be supported.
     */

    META_ASSERT_TRUE(metadata_attr_sorted_by_id_name_count > 500, "there should be at least 500 attributes in total");

    size_t idx = 0;

    for (; idx < metadata_attr_sorted_by_id_name_count; ++idx)
    {
        const sai_attr_metadata_t* meta = metadata_attr_sorted_by_id_name[idx];

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
                    /*
                     * For non read only attributes, there should be a good
                     * reason why object list should support mixed object
                     * types on that list. Then this restriction can be
                     * relaxed and description should be added why mixed
                     * object types should be possible.
                     */

                    META_ASSERT_FAIL(meta, "allowed object types on object id list is more then 1, not supported yet");
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

void check_api_names()
{
    META_LOG_ENTER();

    /*
     * Purpose of this check is to find out if all api names correspond to
     * actual object names and follow convention name and the same signature
     * except some special objects. Currently this test is performed here
     * manually, but it could be coverted to automatic generated test using
     * parse.pl script.
     *
     * NOTE: Currently all new objects needs to be added here manually.
     */

    sai_object_type_t checked[SAI_OBJECT_TYPE_MAX];

    memset(checked, 0, SAI_OBJECT_TYPE_MAX * sizeof(sai_object_type_t));

    void *dummy = NULL;

#define CHECK_API(apiname, short_object_type, object_type)\
    {\
        sai_ ## apiname ## _api_t apiname ## _api;\
        checked[(int)object_type] = object_type;\
        \
        generic_create_fn create = apiname ## _api.create_ ## short_object_type;\
        generic_remove_fn remove = apiname ## _api.remove_ ## short_object_type;\
        generic_set_fn set = apiname ## _api.set_ ## short_object_type ## _attribute;\
        generic_get_fn get =  apiname ## _api.get_ ## short_object_type ## _attribute;\
        sai_create_ ## short_object_type ## _fn cr = NULL;\
        sai_remove_ ## short_object_type ## _fn re = NULL;\
        sai_set_ ## short_object_type ## _attribute_fn se = NULL;\
        sai_get_ ## short_object_type ## _attribute_fn ge = NULL;\
        dummy = &create;\
        dummy = &remove;\
        dummy = &set;\
        dummy = &get;\
        dummy = &cr;\
        dummy = &re;\
        dummy = &se;\
        dummy = &ge;\
    }

    /*
     * Rule here is that SECOND parameter should be exact match for object name
     * but lower case, for example: CHECK_API(foo, xyz, SAI_OBJECT_TYPE_XYZ);
     */

    CHECK_API(port, port, SAI_OBJECT_TYPE_PORT);
    CHECK_API(lag, lag, SAI_OBJECT_TYPE_LAG);
    CHECK_API(virtual_router, virtual_router, SAI_OBJECT_TYPE_VIRTUAL_ROUTER);
    CHECK_API(next_hop, next_hop, SAI_OBJECT_TYPE_NEXT_HOP);
    CHECK_API(router_interface, router_interface, SAI_OBJECT_TYPE_ROUTER_INTERFACE);
    CHECK_API(acl, acl_table, SAI_OBJECT_TYPE_ACL_TABLE);
    CHECK_API(acl, acl_entry, SAI_OBJECT_TYPE_ACL_ENTRY);
    CHECK_API(acl, acl_counter, SAI_OBJECT_TYPE_ACL_COUNTER);
    CHECK_API(acl, acl_range, SAI_OBJECT_TYPE_ACL_RANGE);
    CHECK_API(acl, acl_table_group, SAI_OBJECT_TYPE_ACL_TABLE_GROUP);
    CHECK_API(acl, acl_table_group_member, SAI_OBJECT_TYPE_ACL_TABLE_GROUP_MEMBER);
    CHECK_API(hostif, hostif, SAI_OBJECT_TYPE_HOSTIF);
    CHECK_API(mirror, mirror_session, SAI_OBJECT_TYPE_MIRROR_SESSION);
    CHECK_API(samplepacket, samplepacket, SAI_OBJECT_TYPE_SAMPLEPACKET);
    CHECK_API(stp, stp, SAI_OBJECT_TYPE_STP);
    CHECK_API(hostif, hostif_trap_group, SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP);
    CHECK_API(policer, policer, SAI_OBJECT_TYPE_POLICER);
    CHECK_API(wred, wred, SAI_OBJECT_TYPE_WRED);
    CHECK_API(qos_map, qos_map, SAI_OBJECT_TYPE_QOS_MAP);
    CHECK_API(queue, queue, SAI_OBJECT_TYPE_QUEUE);
    CHECK_API(scheduler, scheduler, SAI_OBJECT_TYPE_SCHEDULER);
    CHECK_API(scheduler_group, scheduler_group, SAI_OBJECT_TYPE_SCHEDULER_GROUP);
    CHECK_API(buffer, buffer_pool, SAI_OBJECT_TYPE_BUFFER_POOL);
    CHECK_API(buffer, buffer_profile, SAI_OBJECT_TYPE_BUFFER_PROFILE);
    CHECK_API(buffer, ingress_priority_group, SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP);
    CHECK_API(lag, lag_member, SAI_OBJECT_TYPE_LAG_MEMBER);
    CHECK_API(hash, hash, SAI_OBJECT_TYPE_HASH);
    CHECK_API(udf, udf, SAI_OBJECT_TYPE_UDF);
    CHECK_API(udf, udf_match, SAI_OBJECT_TYPE_UDF_MATCH);
    CHECK_API(udf, udf_group, SAI_OBJECT_TYPE_UDF_GROUP);
    CHECK_API(hostif, hostif_trap, SAI_OBJECT_TYPE_HOSTIF_TRAP);
    CHECK_API(hostif, hostif_table_entry, SAI_OBJECT_TYPE_HOSTIF_TABLE_ENTRY);
    CHECK_API(vlan, vlan, SAI_OBJECT_TYPE_VLAN);
    CHECK_API(vlan, vlan_member, SAI_OBJECT_TYPE_VLAN_MEMBER);

    /*
     * hostif packet is special since its not a real object but represents
     * attruibutes received from host interface.
     */

    checked[(int)SAI_OBJECT_TYPE_HOSTIF_PACKET] = SAI_OBJECT_TYPE_HOSTIF_PACKET;

    CHECK_API(tunnel, tunnel_map, SAI_OBJECT_TYPE_TUNNEL_MAP);
    CHECK_API(tunnel, tunnel, SAI_OBJECT_TYPE_TUNNEL);
    CHECK_API(tunnel, tunnel_term_table_entry, SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY);

    /*
     * fdb flush is special since its not a real object but represents
     * attruibutes that are passed when flushng fdb entries
     */

    checked[(int)SAI_OBJECT_TYPE_FDB_FLUSH] = SAI_OBJECT_TYPE_FDB_FLUSH;

    CHECK_API(next_hop_group, next_hop_group, SAI_OBJECT_TYPE_NEXT_HOP_GROUP);
    CHECK_API(next_hop_group, next_hop_group_member, SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER);
    CHECK_API(stp, stp_port, SAI_OBJECT_TYPE_STP_PORT);
    CHECK_API(rpf_group, rpf_group, SAI_OBJECT_TYPE_RPF_GROUP);
    CHECK_API(rpf_group, rpf_group_member, SAI_OBJECT_TYPE_RPF_GROUP_MEMBER);
    CHECK_API(l2mc_group, l2mc_group, SAI_OBJECT_TYPE_L2MC_GROUP);
    CHECK_API(l2mc_group, l2mc_group_member, SAI_OBJECT_TYPE_L2MC_GROUP_MEMBER);
    CHECK_API(ipmc_group, ipmc_group, SAI_OBJECT_TYPE_IPMC_GROUP);
    CHECK_API(ipmc_group, ipmc_group_member, SAI_OBJECT_TYPE_IPMC_GROUP_MEMBER);
    CHECK_API(hostif, hostif_user_defined_trap, SAI_OBJECT_TYPE_HOSTIF_USER_DEFINED_TRAP);
    CHECK_API(bridge, bridge, SAI_OBJECT_TYPE_BRIDGE);
    CHECK_API(bridge, bridge_port, SAI_OBJECT_TYPE_BRIDGE_PORT);
    CHECK_API(tunnel, tunnel_map_entry, SAI_OBJECT_TYPE_TUNNEL_MAP_ENTRY);

#define CHECK_ENTRY_API(apiname, entry_name, object_type)\
    {\
        typedef sai_status_t (*entry_name ## _create_fn)(\
                _In_ const sai_ ## entry_name ## _t *entry_name,\
                _In_ uint32_t attr_count,\
                _In_ const sai_attribute_t *attr_list);\
        typedef sai_status_t (*entry_name ## _remove_fn)(\
                _In_ const sai_ ## entry_name ## _t *entry_name);\
        typedef sai_status_t (*entry_name ## _set_fn)(\
                _In_ const sai_ ## entry_name ## _t *entry_name,\
                _In_ const sai_attribute_t *attr);\
        typedef sai_status_t (*entry_name ## _get_fn)(\
                _In_ const sai_ ## entry_name ## _t *entry_name,\
                _In_ uint32_t attr_count,\
                _Inout_ sai_attribute_t *attr_list);\
        \
        sai_ ## apiname ## _api_t apiname ## _api;\
        checked[(int)object_type] = object_type;\
        \
        entry_name ## _create_fn create = apiname ## _api.create_ ## entry_name;\
        entry_name ## _remove_fn remove = apiname ## _api.remove_ ## entry_name;\
        entry_name ## _set_fn set = apiname ## _api.set_ ## entry_name ## _attribute;\
        entry_name ## _get_fn get =  apiname ## _api.get_ ## entry_name ## _attribute;\
        sai_create_ ## entry_name ## _fn cr = NULL;\
        sai_remove_ ## entry_name ## _fn re = NULL;\
        sai_set_ ## entry_name ## _attribute_fn se = NULL;\
        sai_get_ ## entry_name ## _attribute_fn ge = NULL;\
        dummy = &create;\
        dummy = &remove;\
        dummy = &set;\
        dummy = &get;\
        dummy = &cr;\
        dummy = &re;\
        dummy = &se;\
        dummy = &ge;\
    }

    /*
     * Those are objects with non object id, so we need to generate api
     * definitions on the fly.
     */

    CHECK_ENTRY_API(fdb, fdb_entry, SAI_OBJECT_TYPE_FDB_ENTRY);
    CHECK_ENTRY_API(neighbor, neighbor_entry, SAI_OBJECT_TYPE_NEIGHBOR_ENTRY);
    CHECK_ENTRY_API(route, route_entry, SAI_OBJECT_TYPE_ROUTE_ENTRY);
    CHECK_ENTRY_API(l2mc, l2mc_entry, SAI_OBJECT_TYPE_L2MC_ENTRY);
    CHECK_ENTRY_API(ipmc, ipmc_entry, SAI_OBJECT_TYPE_IPMC_ENTRY);
    CHECK_ENTRY_API(mcast_fdb, mcast_fdb_entry, SAI_OBJECT_TYPE_MCAST_FDB_ENTRY);

    {
        /*
         * Switch object is special since it create function
         * don't have switch_id as input parameter
         */

        checked[(int)SAI_OBJECT_TYPE_SWITCH] = SAI_OBJECT_TYPE_SWITCH;

        sai_switch_api_t switch_api;

        switch_create_fn create = switch_api.create_switch;
        generic_remove_fn remove = switch_api.remove_switch;
        generic_set_fn set = switch_api.set_switch_attribute;
        generic_get_fn get = switch_api.get_switch_attribute;
        sai_create_switch_fn cr = NULL;
        sai_remove_switch_fn re = NULL;
        sai_set_switch_attribute_fn se = NULL;
        sai_get_switch_attribute_fn ge = NULL;
        dummy = &create;
        dummy = &remove;
        dummy = &set;
        dummy = &get;
        dummy = &cr;
        dummy = &re;
        dummy = &se;
        dummy = &ge;
    }

    if (debug)
    {
        /*
         * to prevent warnings on not used variable
         */
        printf("dummy pointer: %p", dummy);
    }

    int index = SAI_OBJECT_TYPE_NULL;

    /*
     * check if all objects were processed
     */

    for (; index < SAI_OBJECT_TYPE_MAX; ++index)
    {
        if (checked[index] != (sai_object_type_t)index)
        {
            META_FAIL("object %s (%d) was not added to check",
                    metadata_enum_sai_object_type_t.valuesnames[index], index);
        }
    }
}

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

    const sai_object_type_info_t *oi = sai_all_object_type_infos[depobjecttype];

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
             * This graph entry is struct memner, maybe this i the
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
                    META_LOG_INFO("dep %s ot %s attr %s\n",
                            metadata_enum_sai_object_type_t.valuesnames[depobjecttype],
                            metadata_enum_sai_object_type_t.valuesnames[objecttype],
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

    for (; i <= SAI_OBJECT_TYPE_MAX; ++i)
    {
        sai_object_type_t objecttype = (sai_object_type_t)i;

        const sai_object_type_info_t* info = sai_all_object_type_infos[i];

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

    const sai_attr_metadata_t** const meta = sai_object_type_info_SAI_OBJECT_TYPE_VLAN.attrmetadata;

    size_t index = 0;

    int keys = 0;

    for (; meta[index] != NULL; index++)
    {
        const sai_attr_metadata_t *md = meta[index];

        if (HAS_FLAG_KEY(md->flags))
        {
            keys++;
        }

        if (md->attrid == SAI_VLAN_ATTR_VLAN_ID)
        {
            int expected_flags = (SAI_ATTR_FLAGS_MANDATORY_ON_CREATE|SAI_ATTR_FLAGS_CREATE_ONLY|SAI_ATTR_FLAGS_KEY);

            if ((int)md->flags != expected_flags)
            {
                META_ASSERT_FAIL(md, "vlan id should have flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY, but has: %d", md->flags);
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
        META_FAIL("SAI_ACL_TABLE_ATTR_FIELD_END 0x%x is not equal to SAI_ACL_ENTRY_ATTR_FIELD_END 0x%x",
                SAI_ACL_TABLE_ATTR_FIELD_END, SAI_ACL_ENTRY_ATTR_FIELD_END);
    }

    /*
     * find both attribute fields start for entry and table
     */

    const sai_attr_metadata_t **meta_acl_table = sai_object_type_info_SAI_OBJECT_TYPE_ACL_TABLE.attrmetadata;
    const sai_attr_metadata_t **meta_acl_entry = sai_object_type_info_SAI_OBJECT_TYPE_ACL_ENTRY.attrmetadata;

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

        META_LOG_INFO("processing acl fields: %s %s", mtable->attridname, mentry->attridname);

        /*
         * check acl table flags and attr value type
         */

        if (mtable->attrid == SAI_ACL_TABLE_ATTR_FIELD_ACL_RANGE_TYPE)
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
                META_ASSERT_FAIL(mtable, "acl table field flags should be CREATE_ONLY");
            }

            if (mtable->attrvaluetype != SAI_ATTR_VALUE_TYPE_BOOL)
            {
                META_ASSERT_FAIL(mtable, "acl table attr value type should be bool");
            }
        }

        /*
         * check acl entry flags
         */

        if (mentry->flags != SAI_ATTR_FLAGS_CREATE_AND_SET)
        {
            META_ASSERT_FAIL(mentry, "acl entry field flags should be CREATE_AND_SET");
        }

        if (mentry->attrid != mtable->attrid)
        {
            META_ASSERT_FAIL(mentry, "acl entry attr id %d is different than acl table field %d", mentry->attrid, mtable->attrid);
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
            META_FAIL("attr entry field name %s is not ending at the same name as acl table field %s",
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

    const sai_attr_metadata_t **meta_acl_entry = sai_object_type_info_SAI_OBJECT_TYPE_ACL_ENTRY.attrmetadata;

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

        if (meta->attrid > SAI_ACL_ENTRY_ATTR_ACTION_END)
        {
            break;
        }

        if (meta->flags != SAI_ATTR_FLAGS_CREATE_AND_SET)
        {
            META_ASSERT_FAIL(meta, "acl entry action flags should be CREATE_AND_SET");
        }

        const char* enum_name = metadata_enum_sai_acl_action_type_t.valuesnames[enum_index];

        META_ASSERT_NOT_NULL(enum_name);

        META_LOG_INFO("processing acl action: %s %s", meta->attridname, enum_name);

        /*
         * check acl fields attribute if endings are the same
         */

        const char * enum_name_pos = strstr(enum_name, "_ACTION_TYPE_");

        META_ASSERT_NOT_NULL(enum_name_pos);

        const char * attr_entry_pos = strstr(meta->attridname, "_ATTR_ACTION_");

        META_ASSERT_NOT_NULL(attr_entry_pos);

        if (strcmp(enum_name_pos + strlen("_ACTION_TYPE_"), attr_entry_pos + strlen("_ATTR_ACTION_")) != 0)
        {
            META_FAIL("attr entry action name %s is not ending at the same enum name %s",
                    meta->attridname, enum_name);
        }

        index++;
        enum_index++;
    }

    META_ASSERT_TRUE(enum_index == metadata_enum_sai_acl_action_type_t.valuescount,
            "number of acl entry action mismatch vs number of enums in sai_acl_action_type_t");
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

    const sai_attr_metadata_t** const meta = sai_object_type_info_SAI_OBJECT_TYPE_SWITCH.attrmetadata;

    size_t index = 0;

    for (; meta[index] != NULL; index++)
    {
        const sai_attr_metadata_t *md = meta[index];

        if (HAS_FLAG_CREATE_ONLY(md->flags) && md->allowedobjecttypeslength > 0)
        {
            META_ASSERT_FAIL(md, "attribute is create_only and it's an object id, this is not allowed");
        }
    }
}

void check_quad_api_pointers(
    _In_ const sai_object_type_info_t *oi)
{
    META_LOG_ENTER();

    /*
     * Check if quad api pointers are not NULL except hostif packet and fdb
     * flush which are special.
     */

    if (oi->objecttype == SAI_OBJECT_TYPE_HOSTIF_PACKET ||
        oi->objecttype == SAI_OBJECT_TYPE_FDB_FLUSH)
    {
        META_ASSERT_NULL(oi->create);
        META_ASSERT_NULL(oi->remove);
        META_ASSERT_NULL(oi->set);
        META_ASSERT_NULL(oi->get);
    }
    else
    {
        META_ASSERT_NOT_NULL(oi->create);
        META_ASSERT_NOT_NULL(oi->remove);
        META_ASSERT_NOT_NULL(oi->set);
        META_ASSERT_NOT_NULL(oi->get);
    }
}

void check_single_object_info(
    _In_ const sai_object_type_info_t *oi)
{
    META_LOG_ENTER();

    check_quad_api_pointers(oi);
}

int main(int argc, char **argv)
{
    debug = (argc > 1);

    SAI_META_LOG_ENTER();

    check_all_enums_name_pointers();
    check_all_enums_values();
    check_sai_status();
    check_object_type();
    check_attr_by_object_type();

    size_t i = 0;

    for (; i < metadata_attr_by_object_type_count; ++i)
    {
        check_single_object_type_attributes(metadata_attr_by_object_type[i]);
    }

    check_object_infos();
    check_attr_sorted_by_id_name();
    check_non_object_id_object_types();
    check_non_object_id_object_attrs();
    check_objects_for_loops();
    check_null_object_id();
    check_read_only_attributes();
    check_mixed_object_list_types();
    check_vlan_attributes();
    check_api_names();
    check_switch_create_only_objects();
    check_reverse_graph_for_non_object_id();
    check_acl_table_fields_and_acl_entry_fields();
    check_acl_entry_actions();

    i = SAI_OBJECT_TYPE_NULL + 1;

    for (; i < SAI_OBJECT_TYPE_MAX; ++i)
    {
        check_single_object_info(sai_all_object_type_infos[i]);
    }

    SAI_META_LOG_DEBUG("log test");

    printf("\n [ %s ]\n\n", sai_metadata_get_status_name(SAI_STATUS_SUCCESS));

    SAI_META_LOG_EXIT();

    return 0;
}
