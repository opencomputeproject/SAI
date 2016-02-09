#include "sai_redis.h"

/**
 *   Routine Description:
 *    @brief Internal set attribute
 *
 *  Arguments:
 *  @param[in] object_type - type of object
 *  @param[in] serialized_object_id - serialized object id
 *  @param[in] attr - attribute to serialize
 *
 *  Return Values:
 *    @return  SAI_STATUS_SUCCESS on success
 *             Failure status code on error
 */
sai_status_t internal_redis_generic_set(
        _In_ sai_object_type_t object_type,
        _In_ const std::string &serialized_object_id,
        _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    if (attr == NULL)
    {
        REDIS_LOG_EXIT();
        return SAI_STATUS_INVALID_PARAMETER;
    }

    sai_attr_serialization_type_t serialization_type;

    sai_status_t status = sai_get_serialization_type(object_type, attr->id, serialization_type);

    if (status != SAI_STATUS_SUCCESS)
    {
        REDIS_LOG_ERR("Unable to find serialization type for object type: %u and attribute id: %u, status: %u",
                object_type,
                attr->id,
                status);

        REDIS_LOG_EXIT();
        return status;
    }

    std::string str_object_type;
    sai_serialize_primitive(object_type, str_object_type);

    std::string str_common_api;
    sai_serialize_primitive(SAI_COMMON_API_SET, str_common_api);

    //std::string str_object_type;
    //status = sai_get_object_type_string(object_type, str_object_type);

    //if (status != SAI_STATUS_SUCCESS)
    //{
    //    REDIS_LOG_ERR("Unable to find object type string for %u, status: %u",
    //            object_type,
    //            status);

    //    REDIS_LOG_EXIT();
    //    return status;
    //}

    std::string str_attr_id;
    sai_serialize_attr_id(*attr, str_attr_id);
    
    std::string str_attr_value;
    status = sai_serialize_attr_value(serialization_type, *attr, str_attr_value);

    if (status != SAI_STATUS_SUCCESS)
    {
        REDIS_LOG_ERR("Unable to serialize attribute for object type: %u and attribute id: %u, status: %u",
                object_type,
                attr->id,
                status);

        REDIS_LOG_EXIT();
        return status;
    }

    ssw::FieldValueTuple fvt(str_attr_id, str_attr_value);

    std::vector<ssw::FieldValueTuple> entry = { fvt };

    std::string key = str_object_type + ":" + serialized_object_id;

    g_asicState->set(key, entry); // TODO set needs extra param op = str_common_api;

    REDIS_LOG_EXIT();

    return SAI_STATUS_SUCCESS;

}

sai_status_t redis_generic_set(
        _In_ sai_object_type_t object_type,
        _In_ sai_object_id_t object_id,
        _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    std::string str_object_id;
    sai_serialize_primitive(object_id, str_object_id);

    sai_status_t status = internal_redis_generic_set(
            object_type, 
            str_object_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_set(
        _In_ sai_object_type_t object_type,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    std::string str_fdb_entry;
    sai_serialize_primitive(*fdb_entry, str_fdb_entry);

    sai_status_t status = internal_redis_generic_set(
            object_type, 
            str_fdb_entry,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_set(
        _In_ sai_object_type_t object_type,
        _In_ const sai_neighbor_entry_t* neighbor_entry,
        _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    std::string str_neighbor_entry;
    sai_serialize_primitive(*neighbor_entry, str_neighbor_entry);

    sai_status_t status = internal_redis_generic_set(
            object_type, 
            str_neighbor_entry,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_set(
        _In_ sai_object_type_t object_type,
        _In_ const sai_unicast_route_entry_t* unicast_route_entry,
        _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    std::string str_route_entry;
    sai_serialize_primitive(*unicast_route_entry, str_route_entry);

    sai_status_t status = internal_redis_generic_set(
            object_type, 
            str_route_entry,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_set_vlan(
        _In_ sai_object_type_t object_type,
        _In_ sai_vlan_id_t vlan_id,
        _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    std::string str_vlan_id;
    sai_serialize_primitive(vlan_id, str_vlan_id);

    sai_status_t status = internal_redis_generic_set(
            object_type, 
            str_vlan_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}
