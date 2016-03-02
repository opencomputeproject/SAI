#include "sai_redis.h"

sai_object_id_t translate_rid_to_vid(
        _In_ sai_object_type_t object_type,
        _In_ sai_object_id_t rid)
{
    // get from syncd should always return real id
    sai_object_id_t vid;

    std::string str_rid;
    std::string str_vid;

    sai_serialize_primitive(rid, str_rid);

    if (g_ridToVid->tryGet(str_rid, str_vid))
    {
        std::cout << "rid exists in db" << std::endl;

        // object exists

        int index = 0;
        sai_deserialize_primitive(str_vid, index, vid);

        return vid;
    }

    std::cout << "rid is missing from db" << std::endl;

    vid = redis_create_virtual_object_id(object_type);

    sai_serialize_primitive(vid, str_vid);

    g_ridToVid->set(str_rid, str_vid);
    g_vidToRid->set(str_vid, str_rid);

    return vid;
}

template <typename T>
void translate_list_rid_to_vid(
        _In_ sai_object_type_t object_type,
        _In_ T &element)
{
    for (uint32_t i = 0; i < element.count; i++)
    {
        element.list[i] = translate_rid_to_vid(object_type, element.list[i]); 
    }
}

void translate_rid_to_vid(
        _In_ sai_object_type_t object_type,
        _In_ uint32_t attr_count,
        _In_ sai_attribute_t *attr_list)
{
    // we receive real id's here, if they are new then create new id 
    // for them and put in db, if entry exists in db, use it

    for (uint32_t i = 0; i < attr_count; i++)
    {
        sai_attribute_t &attr = attr_list[i];

        sai_attr_serialization_type_t serialization_type;
        sai_status_t status = sai_get_serialization_type(object_type, attr.id, serialization_type);

        if (status != SAI_STATUS_SUCCESS)
        {
            throw std::runtime_error("unable to find serialization type");
        }

        switch (serialization_type)
        {
            case SAI_SERIALIZATION_TYPE_OBJECT_ID:
                attr.value.oid = translate_rid_to_vid(object_type, attr.value.oid);
                break;

            case SAI_SERIALIZATION_TYPE_OBJECT_LIST:
                translate_list_rid_to_vid(object_type, attr.value.objlist);
                break;

            case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_OBJECT_ID:
                attr.value.aclfield.data.oid = translate_rid_to_vid(object_type, attr.value.aclfield.data.oid);
                break;

            case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
                translate_list_rid_to_vid(object_type, attr.value.aclfield.data.objlist);
                break;

            case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_OBJECT_ID:
                attr.value.aclaction.parameter.oid = translate_rid_to_vid(object_type, attr.value.aclaction.parameter.oid);
                break;

            case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
                translate_list_rid_to_vid(object_type, attr.value.aclaction.parameter.objlist);
                break;

            case SAI_SERIALIZATION_TYPE_PORT_BREAKOUT:
                translate_list_rid_to_vid(object_type, attr.value.portbreakout.port_list);

            default:
                break;
        }
    }
}

sai_status_t internal_redis_get_process(
        _In_ sai_object_type_t object_type,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list,
        _In_ ssw::KeyOpFieldsValuesTuple &kco)
{
    REDIS_LOG_ENTER();

    // key is: object_type:object_id:sai_status

    const std::string &key = kfvKey(kco);
    const std::vector<ssw::FieldValueTuple> &values = kfvFieldsValues(kco);

    std::string str_sai_status = key; //.substr(key.find_last_of(":") + 1);

    sai_status_t status;

    int index = 0;
    sai_deserialize_primitive(str_sai_status, index, status);

    // we could deserialize directly to user data, but list is alocated by deserializer
    if (status == SAI_STATUS_SUCCESS)
    {
        ssw::SaiAttributeList list(object_type, values, false);

        transfer_attributes(object_type, attr_count, list.get_attr_list(), attr_list, false);

        translate_rid_to_vid(object_type, attr_count, attr_list);
    }
    else if (status == SAI_STATUS_BUFFER_OVERFLOW)
    {
        ssw::SaiAttributeList list(object_type, values, true);

        // no need for id fix since this is overflow
        transfer_attributes(object_type, attr_count, list.get_attr_list(), attr_list, true);
    }

    REDIS_LOG_EXIT();

    return status;
}

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
sai_status_t internal_redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ const std::string &serialized_object_id,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    std::vector<ssw::FieldValueTuple> entry = ssw::SaiAttributeList::serialize_attr_list(
            object_type, 
            attr_count,
            attr_list,
            false);

    std::string str_object_type;

    sai_serialize_primitive(object_type, str_object_type);

    std::string key = str_object_type + ":" + serialized_object_id;

    g_redisGetProducer->set(key, entry, "get");
    g_redisGetProducer->del(key, "delget");

    // wait for response
    
    ssw::Select s;

    s.addSelectable(g_redisGetConsumer);

    while (true)
    {
        ssw::Selectable *sel;

        int fd;

        int result = s.select(&sel, &fd, 2000);

        if (result == ssw::Select::OBJECT)
        {
            ssw::KeyOpFieldsValuesTuple kco;

            g_redisGetConsumer->pop(kco);

            const std::string &op = kfvOp(kco); 
            const std::string &key = kfvKey(kco);

            std::cout << "op = " << op << std::endl;
            std::cout << "key = " << key << std::endl;

            if (op != "getresponse") // ignore non response messages
                continue;

            sai_status_t sai_status = internal_redis_get_process(
                    object_type, 
                    attr_count, 
                    attr_list, 
                    kco);

            REDIS_LOG_EXIT();

            return sai_status;
        }

        REDIS_LOG_ERR("failed to get response for get status: %d", result);
        break;
    }

    REDIS_LOG_EXIT();

    return SAI_STATUS_FAILURE;
}

/**
 * Routine Description:
 *   @brief Generic get attribute
 *
 * Arguments:
 *    @param[in] object_type - the object type
 *    @param[in] object_id - the object id
 *    @param[in] attr_count - number of attributes
 *    @param[out] attr_list - array of attributes
 *
 * Return Values:
 *    @return  SAI_STATUS_SUCCESS on success
 *             Failure status code on error
 */
sai_status_t redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ sai_object_id_t object_id,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    std::string str_object_id;
    sai_serialize_primitive(object_id, str_object_id);

    sai_status_t status = internal_redis_generic_get(
            object_type,
            str_object_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    std::string str_fdb_entry;
    sai_serialize_primitive(*fdb_entry, str_fdb_entry);

    sai_status_t status = internal_redis_generic_get(
            object_type,
            str_fdb_entry,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ const sai_neighbor_entry_t* neighbor_entry,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    std::string str_neighbor_entry;
    sai_serialize_primitive(*neighbor_entry, str_neighbor_entry);

    sai_status_t status = internal_redis_generic_get(
            object_type,
            str_neighbor_entry,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ const sai_unicast_route_entry_t* unicast_route_entry,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    std::string str_route_entry;
    sai_serialize_primitive(*unicast_route_entry, str_route_entry);

    sai_status_t status = internal_redis_generic_get(
            object_type,
            str_route_entry,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_get_vlan(
        _In_ sai_object_type_t object_type,
        _In_ sai_vlan_id_t vlan_id,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    std::string str_vlan_id;
    sai_serialize_primitive(vlan_id, str_vlan_id);

    sai_status_t status = internal_redis_generic_get(
            object_type,
            str_vlan_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}
