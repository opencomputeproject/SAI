#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sai.h>
#include "saimetadatautils.h"
#include "saimetadata.h"

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

    META_ASSERT_TRUE(metadata_attr_by_object_type_count == SAI_OBJECT_TYPE_MAX + 1, "invalid object type count in metadata");

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

        META_LOG_INFO("attr index %3zu for %s", index, sai_metadata_get_object_type_name((sai_object_type_t)i));
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

            if (md->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT32 &&
                    md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_ATTR_RANGE)
            {
                break;
            }

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
                if (md->attrvaluetype == SAI_ATTR_VALUE_TYPE_OBJECT_ID)
                {
                    /*
                     * Object id's may not provide default value, which will mean it can
                     * be NULL or assigned by switch by default.
                     */
                    break;
                }

                if (md->attrvaluetype == SAI_ATTR_VALUE_TYPE_POINTER)
                {
                    /*
                     * Pointer may not provide default value,
                     * which will mean it can be NULL.
                     */
                    break;
                }

                if (sai_metadata_is_acl_field_or_action(md))
                {
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
                     * Read only object id/list can be marked as internal
                     * like default virtual router, cpu port id, etc.
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
        /* we don't care about ACL entry data/field */
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
            META_ASSERT_FAIL(md, "attr value type is not supported)");
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
        if (sai_metadata_is_acl_field_or_action(md))
        {
            return;
        }

        META_ASSERT_FAIL(md, "expected default value, but none provided");
    }

    switch (md->defaultvaluetype)
    {
        case SAI_DEFAULT_VALUE_TYPE_CONST:

            if (md->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT8_LIST)
            {
                /* const on list */
                break;
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

            if (md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_CONST)
            {
                break;
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
        case SAI_ATTR_FLAGS_CREATE_ONLY: /* will require default value, on some cases may be dynamic */
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
            /* this is provieded for SAI_TUNNEL_ATTR_ENCAP_GRE_KEY and needs to be converted to validonly */

            if (cmd->flags == SAI_ATTR_FLAGS_CREATE_ONLY &&
                    cmd->attrvaluetype == SAI_ATTR_VALUE_TYPE_BOOL)
            {
                /* ok, that means there is default value (it may be depending on switch intenal) */
            }
            else
            {
                META_ASSERT_FAIL(md, "conditional attibute is also conditional, not allowed");
            }
        }

        switch ((int)cmd->flags)
        {
            case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY | SAI_ATTR_FLAGS_KEY:
            case SAI_ATTR_FLAGS_MANDATORY_ON_CREATE | SAI_ATTR_FLAGS_CREATE_ONLY:
            case SAI_ATTR_FLAGS_CREATE_ONLY:
                /*
                 * condition attribute must be create only since
                 * if it could change then other object may be required to pass
                 * on creation time that was not passed
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
        case SAI_ATTR_FLAGS_CREATE_AND_SET:

            /* ok */

            break;

        case SAI_ATTR_FLAGS_READ_ONLY:

            META_ASSERT_FAIL(md, "read only attribute can't be valid only");
            break;

        default:

            META_ASSERT_FAIL(md, "marked as validonly, but invalid creation flags: 0x%u", md->flags);
    }

    if ((md->defaultvaluetype == SAI_DEFAULT_VALUE_TYPE_NONE) ||
            (md->defaultvalue == NULL))
    {
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
            META_ASSERT_FAIL(md, "validonly attibute is also validonly attribute, not allowed");
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
                 * valid only attribute can be create_only or create_and_set
                 * conditional attribute can change during runtime and it may
                 * have impact on valid only attribute (it may or may not be used)
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
            case SAI_ATTR_VALUE_TYPE_POINTER:
                break;

            default:

                META_ASSERT_FAIL(md, "allow null object is set but attr value type is wrong");
        }

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
                    break;
                }

                META_ASSERT_FAIL(md, "marked as key, but have invalid attr value type (list)");

            case SAI_ATTR_VALUE_TYPE_OBJECT_ID:

                if (md->objecttype == SAI_OBJECT_TYPE_QUEUE && md->attrid == SAI_QUEUE_ATTR_PORT)
                {
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

            if (md->objecttype != SAI_OBJECT_TYPE_ACL_ENTRY ||
                    md->attrid < SAI_ACL_ENTRY_ATTR_FIELD_START ||
                    md->attrid > SAI_ACL_ENTRY_ATTR_FIELD_END)
            {
                if (md->objecttype != SAI_OBJECT_TYPE_UDF_MATCH)
                {
                    META_ASSERT_FAIL(md, "acl field may only be set on acl field and udf match");
                }
            }

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

void define_attr(
        _In_ const sai_attr_metadata_t* md)
{
    META_LOG_ENTER();

    defined_attr_t *p = (defined_attr_t*)malloc(sizeof(defined_attr_t));

    p->metadata = md;
    p->next = defined_attributes;

    defined_attributes = p;
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

            if (info->objecttype == SAI_OBJECT_TYPE_ACL_ENTRY ||
                    info->objecttype == SAI_OBJECT_TYPE_ACL_TABLE)
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

        bool member_supports_switch_id = false;

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

                    META_FAIL("struct member %s have invalid value type %d", m->membername, m->membervaluetype);
            }

            if (m->isvlan)
            {
                META_ASSERT_TRUE(m->membervaluetype == SAI_ATTR_VALUE_TYPE_UINT16, "member marked as vlan, but wrong type specified");
            }

            if (m->membervaluetype == SAI_ATTR_VALUE_TYPE_OBJECT_ID)
            {
                META_ASSERT_NOT_NULL(m->allowedobjecttypes);
                META_ASSERT_TRUE(m->allowedobjecttypeslength > 0, "member is object id, should specify some object types");

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

                            member_supports_switch_id = true;
                        }

                        continue;
                    }

                    META_FAIL("invalid object type specified on file %s: %d", m->membername, ot);
                }
            }
            else
            {
                META_ASSERT_NULL(m->allowedobjecttypes);
                META_ASSERT_TRUE(m->allowedobjecttypeslength == 0, "member is not object id, should not specify object types");
            }
        }

        META_ASSERT_TRUE(member_supports_switch_id, "none of struct members support switch id object type");

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

                    META_WARN(m, "non object id attribute has invalid flags: 0x%x (should be CREATE_AND_SET)", m->flags);
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

    META_ASSERT_TRUE(SAI_NULL_OBJECT_ID == 0, "SAI_NULL_OBJECT_ID must be zero");
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

int main(int argc, char **argv)
{
    debug = (argc > 1);

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
    check_mixed_object_list_types();

    printf("\n [ %s ]\n\n", sai_metadata_get_status_name(SAI_STATUS_SUCCESS));

    return 0;
}
