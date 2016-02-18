#include "sai_attribute_list.h"

namespace ssw
{

SaiAttributeList::SaiAttributeList(
        _In_ const sai_object_type_t object_type,
        _In_ const std::vector<ssw::FieldValueTuple> &values,
        _In_ bool onlyCount)
{
    uint32_t attr_count = values.size();

    for (uint32_t i = 0; i < attr_count; ++i)
    {
        const std::string &str_attr_id = fvField(values[i]);
        const std::string &str_attr_value = fvValue(values[i]);

        std::cout << " <deserialize>  " << str_attr_id << " = " << str_attr_value << std::endl; // DEBUG

        sai_attribute_t attr;
        memset(&attr, 1, sizeof(sai_attribute_t));

        int index = 0;
        sai_deserialize_primitive(str_attr_id, index, attr.id);

        sai_attr_serialization_type_t serialization_type;
        sai_status_t status = sai_get_serialization_type(object_type, attr.id, serialization_type);

        if (status != SAI_STATUS_SUCCESS)
        {
            throw std::runtime_error("failed to get serialization type");
        }

        index = 0;
        status = sai_deserialize_attr_value(str_attr_value, index, serialization_type, attr, onlyCount);

        if (status != SAI_STATUS_SUCCESS)
        {
            throw std::runtime_error("failed to deserialize attribute value");
        }

        m_attr_list.push_back(attr);
        m_serialization_type_list.push_back(serialization_type);
    }
}

SaiAttributeList::~SaiAttributeList()
{
    uint32_t attr_count = m_attr_list.size();

    for (uint32_t i = 0; i < attr_count; ++i)
    {
        sai_attribute_t &attr = m_attr_list[i];

        sai_attr_serialization_type_t serialization_type = m_serialization_type_list[i];

        sai_status_t status = sai_deserialize_free_attribute_value(serialization_type, attr);

        if (status != SAI_STATUS_SUCCESS)
        {
            throw std::runtime_error("deserialize free failed");
        }
    }
}

std::vector<ssw::FieldValueTuple> SaiAttributeList::serialize_attr_list(
        _In_ sai_object_type_t object_type,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list,
        _In_ bool onlyCount)
{
    std::vector<ssw::FieldValueTuple> entry;

    for (uint32_t index = 0; index < attr_count; ++index)
    {
        const sai_attribute_t *attr = &attr_list[index];

        sai_attr_serialization_type_t serialization_type;

        sai_status_t status = sai_get_serialization_type(object_type, attr->id, serialization_type);

        if (status != SAI_STATUS_SUCCESS)
        {
            //LOG_ERR("Unable to find serialization type for object type: %u and attribute id: %u, status: %u",
            //        object_type,
            //        attr->id,
            //        status);

            throw std::runtime_error("unable to find serialization type");
        }

        std::string str_attr_id;
        sai_serialize_attr_id(*attr, str_attr_id);

        std::string str_attr_value;
        status = sai_serialize_attr_value(serialization_type, *attr, str_attr_value, onlyCount);

        if (status != SAI_STATUS_SUCCESS)
        {
            //LOG_ERR("Unable to serialize attribute for object type: %u and attribute id: %u, status: %u",
            //        object_type,
            //        attr->id,
            //        status);

            throw std::runtime_error("unable to serialize attribute value");
        }

        ssw::FieldValueTuple fvt(str_attr_id, str_attr_value);

        entry.push_back(fvt);
    }

    return std::move(entry);
}

sai_attribute_t* SaiAttributeList::get_attr_list()
{
    return m_attr_list.data();
}

uint32_t SaiAttributeList::get_attr_count()
{
    return m_attr_list.size();
}

} // namespace ssw
