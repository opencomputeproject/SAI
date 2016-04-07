#include "sai_redis.h"

/**
 *  Routine Description:
 *    @brief Removes specified object
 *
 *  Arguments:
 *    @param[in] object_type - the object type
 *    @param[in] object_id - the object id
 *
 *  Return Values:
 *    @return  SAI_STATUS_SUCCESS on success
 *             Failure status code on error
 */
sai_status_t redis_generic_remove(
        _In_ sai_object_type_t object_type,
        _In_ sai_object_id_t object_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_remove(
        _In_ sai_object_type_t object_type,
        _In_ const sai_fdb_entry_t* fdb_entry)
{
    REDIS_LOG_ENTER();

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_remove(
        _In_ sai_object_type_t object_type,
        _In_ const sai_neighbor_entry_t* neighbor_entry)
{
    REDIS_LOG_ENTER();

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_remove(
        _In_ sai_object_type_t object_type,
        _In_ const sai_unicast_route_entry_t* unicast_route_entry)
{
    REDIS_LOG_ENTER();

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_remove_vlan(
        _In_ sai_object_type_t object_type,
        _In_ sai_vlan_id_t vlan_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

    REDIS_LOG_EXIT();

    return status;
}


