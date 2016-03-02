#ifndef __SAI_ATTRIBUTE_LIST__
#define __SAI_ATTRIBUTE_LIST__

#include <string>
#include <vector>

#include <hiredis/hiredis.h>
#include "common/dbconnector.h"
#include "common/table.h"
#include "sai.h"
#include "sai_serialize.h"
#include "string.h"

namespace ssw
{

class SaiAttributeList
{
    public:

        SaiAttributeList(
                _In_ const sai_object_type_t object_type,
                _In_ const std::vector<ssw::FieldValueTuple> &values,
                _In_ bool onlyCount);

        ~SaiAttributeList();

        sai_attribute_t* get_attr_list();

        uint32_t get_attr_count();

        static std::vector<ssw::FieldValueTuple> serialize_attr_list(
                _In_ sai_object_type_t object_type,
                _In_ uint32_t attr_count,
                _In_ const sai_attribute_t *attr_list,
                _In_ bool onlyCount);

    private:

        std::vector<sai_attribute_t> m_attr_list;
        std::vector<sai_attr_serialization_type_t> m_serialization_type_list;
};

} // namespace ssw

#endif // __SAI_ATTRIBUTE_LIST__
