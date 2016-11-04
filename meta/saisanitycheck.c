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

            META_ASSERT_TRUE(value < 0x6000, "enum value is too big, range?");
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
                     * String  or Pointer may not provide default value, 
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

            if (md->defaultvaluetype != SAI_DEFAULT_VALUE_TYPE_NONE)
            {
                META_ASSERT_FAIL(md, "no default value expected, but type provided: %s",
                        sai_metadata_get_default_value_type_name(md->defaultvaluetype));
            }

            if (md->conditiontype != SAI_ATTR_CONDITION_TYPE_NONE)
            {
                META_ASSERT_FAIL(md, "read only can't be conditional");
            }

            break;

        default:

            META_ASSERT_FAIL(md, "invalid creation flags: 0x%u", md->flags);
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
        if (md->attrvaluetype == SAI_ATTR_VALUE_TYPE_OBJECT_ID)
        {
            /* object id may not have default value */
            return;
        }

        if (md->attrvaluetype == SAI_ATTR_VALUE_TYPE_POINTER)
        {
            /* Pointer or String may not have default value */
            return;
        }

        if (sai_metadata_is_acl_field_or_action(md))
        {
            return;
        }

        META_ASSERT_FAIL(md, "expected default value, but none provided");
    }

    switch (md->defaultvaluetype)
    {
        case SAI_DEFAULT_VALUE_TYPE_INHERIT:
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
        case SAI_DEFAULT_VALUE_TYPE_NONE:
        case SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST:

            if (md->defaultvalue != NULL)
            {
                META_ASSERT_FAIL(md, "default value type is provided, but default value pointer is NULL");
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

        case SAI_DEFAULT_VALUE_TYPE_INHERIT:

            if (md->objecttype == SAI_OBJECT_TYPE_BUFFER_PROFILE &&
                    md->attrid == SAI_BUFFER_PROFILE_ATTR_TH_MODE)
            {
                break;
            }

            META_ASSERT_FAIL(md, "inherit default value type not allowed");

        case SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE:

            {
                const sai_attr_metadata_t* def = sai_metadata_get_attr_metadata(md->defaultvalueobjecttype, md->defaultvalueattrid);

                if (def == NULL)
                {
                    META_ASSERT_FAIL(md, "attr value can't be found");
                }

                if (md->attrvaluetype != def->attrvaluetype)
                {
                    META_ASSERT_FAIL(md, "attr value attribute value value type is different");
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

            if (md->objecttype != SAI_OBJECT_TYPE_MIRROR_SESSION)
            {
                META_ASSERT_FAIL(md, "marked as conditional on non mirror session, but invalid creation flags: 0x%u", md->flags);
            }

            break;

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

void check_attr_allow_flags(
        _In_ const sai_attr_metadata_t* md)
{
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
    check_attr_allow_flags(md);
    check_attr_get_save(md);
    check_attr_key(md);
    check_attr_acl_fields(md);
    check_attr_vlan(md);

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

    printf("\n [ %s ]\n\n",  sai_metadata_get_status_name(SAI_STATUS_SUCCESS));

    return 0;
}
