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

    std::vector<ssw::FieldValueTuple> entry = ssw::SaiAttributeList::serialize_attr_list(
            object_type, 
            1,
            attr,
            false);

    std::string str_object_type;

    sai_serialize_primitive(object_type, str_object_type);

    std::string key = str_object_type + ":" + serialized_object_id;

    g_asicState->set(key, entry, "set");

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
