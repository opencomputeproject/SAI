#include "sai_redis.h"

/**
 *   Routine Description:
 *    @brief Generic create method
 * 
 *  Arguments:
 *  @param[out] acl_table_id - the the acl table id
 *  @param[in] attr_count - number of attributes
 *  @param[in] attr_list - array of attributes
 * 
 *  Return Values:
 *    @return  SAI_STATUS_SUCCESS on success
 *             Failure status code on error
 */
sai_status_t redis_generic_create(
        _In_ sai_object_type_t object_type,
        _Out_ sai_object_id_t* object_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    // this method will need to return generated virtual ID
    // since is't separated from syncd which whill make
    // actual calls to switch

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_create(
        _In_ sai_object_type_t object_type,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    // fdb entry is actual "key"
    // and attribute id is field:value (value is serialized attribute)
    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_create(
        _In_ sai_object_type_t object_type,
        _In_ const sai_neighbor_entry_t* neighbor_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    // need 
    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_create(
        _In_ sai_object_type_t object_type,
        _In_ const sai_unicast_route_entry_t* unicast_route_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_create_vlan(
        _In_ sai_object_type_t object_type,
        _In_ sai_vlan_id_t vlan_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

    REDIS_LOG_EXIT();

    return status;
}
