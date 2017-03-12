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
    if ((objecttype > SAI_OBJECT_TYPE_NULL) &&
            (objecttype < SAI_OBJECT_TYPE_MAX))
    {
        const sai_attr_metadata_t** md = metadata_attr_by_object_type[objecttype];

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
    ssize_t last = (ssize_t)(metadata_attr_sorted_by_id_name_count - 1);

    while (first <= last)
    {
        ssize_t middle = (first + last) / 2;

        int res = strcmp(attr_id_name, metadata_attr_sorted_by_id_name[middle]->attridname);

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

            return metadata_attr_sorted_by_id_name[middle];
        }
    }

    /* not found */

    return NULL;
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
    }

    return false;
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
