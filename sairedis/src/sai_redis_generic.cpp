#include "sai_redis.h"

/**
 *   Routine Description:
 *    @brief Generic set attribute
 *
 *  Arguments:
 *  @param[in] api_function - caller
 *  @param[in] object_type - type of object
 *  @param[in] object_id - id of object
 *  @param[in] attr - attribute to serialize
 *
 *  Return Values:
 *    @return  SAI_STATUS_SUCCESS on success
 *             Failure status code on error
 */
sai_status_t redis_generic_set(
    _In_ sai_object_type_t objet_type,
    _In_ sai_object_id_t object_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = SAI_STATUS_NOT_IMPLEMENTED;

    // TODO base on object_type and attr.id we should know what
    // type to serialize, object_id will be key

    REDIS_LOG_EXIT();

    return status;
}

