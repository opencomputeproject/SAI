#include "sai_redis.h"

/**
 * @brief Create stp instance with default port state as forwarding.
 *
 * @param[out] stp_id stp instance id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t  redis_create_stp(
    _Out_ sai_object_id_t *stp_id,
    _In_  uint32_t attr_count, 
    _In_  const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create(
            SAI_OBJECT_TYPE_STP_INSTANCE,
            stp_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Remove stp instance.
 *
 * @param[in] stp_id stp instance id
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t  redis_remove_stp(
       _In_ sai_object_id_t stp_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove(
            SAI_OBJECT_TYPE_STP_INSTANCE,
            stp_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Update stp state of a port in specified stp instance.
 *
 * @param[in] stp_id stp instance id
 * @param[in] port_id port id
 * @param[in] stp_port_state stp state of the port
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t  redis_set_stp_port_state(
    _In_ sai_object_id_t stp_id,
    _In_ sai_object_id_t port_id, 
    _In_ sai_port_stp_port_state_t stp_port_state)
{
    REDIS_LOG_ENTER();

    // TODO function signature must be changed to match other types

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
 * @brief Retrieve stp state of a port in specified stp instance.
 *
 * @param[in] stp_id stp instance id
 * @param[in] port_id port id
 * @param[out] stp_port_state stp state of the port
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t  redis_get_stp_port_state(
    _In_ sai_object_id_t stp_id,
    _In_ sai_object_id_t port_id, 
    _Out_ sai_port_stp_port_state_t  *stp_port_state)
{
    REDIS_LOG_ENTER();

    // TODO function signature must be changed to match other types

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
 * @brief Set the attribute of STP instance.
 *
 * @param[in] stp_id stp instance id
 * @param[in] attr attribute value
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t  redis_set_stp_attribute(
    _In_ sai_object_id_t stp_id, 
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_STP_INSTANCE,
            stp_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Get the attribute of STP instance.
 *
 * @param[in] stp_id stp instance id
 * @param[in] attr_count number of the attribute
 * @param[in] attr_list attribute value
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t  redis_get_stp_attribute(
    _In_ sai_object_id_t stp_id,
    _In_ uint32_t attr_count, 
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_STP_INSTANCE,
            stp_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief STP method table retrieved with sai_api_query()
 */
const sai_stp_api_t redis_stp_api = {
    redis_create_stp,
    redis_remove_stp,
    redis_set_stp_attribute,
    redis_get_stp_attribute,
    redis_set_stp_port_state,
    redis_get_stp_port_state,
};
