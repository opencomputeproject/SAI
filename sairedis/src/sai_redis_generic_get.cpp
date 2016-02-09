#include "sai_redis.h"

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

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

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

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

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

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

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

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

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

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

    REDIS_LOG_EXIT();

    return status;
}
