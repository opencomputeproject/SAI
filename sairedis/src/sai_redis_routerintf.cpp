#include "sai_redis.h"

/**
 * Routine Description:
 *    @brief Create router interface. 
 *
 * Arguments:
 *    @param[out] rif_id - router interface id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t redis_create_router_interface(
    _Out_ sai_object_id_t* rif_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create(
            SAI_OBJECT_TYPE_ROUTER_INTERFACE,
            rif_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Remove router interface
 *
 * Arguments:
 *    @param[in] rif_id - router interface id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t redis_remove_router_interface(
    _In_ sai_object_id_t rif_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove(
            SAI_OBJECT_TYPE_ROUTER_INTERFACE,
            rif_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Set router interface attribute
 *
 * Arguments:
 *    @param[in] rif_id - router interface id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_set_router_interface_attribute(
    _In_ sai_object_id_t rif_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_ROUTER_INTERFACE,
            rif_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Get router interface attribute
 *
 * Arguments:
 *    @param[in] rif_id - router interface id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_get_router_interface_attribute(
    _In_ sai_object_id_t rif_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_ROUTER_INTERFACE,
            rif_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 *  @brief Routing interface methods table retrieved with sai_api_query()
 */
const sai_router_interface_api_t redis_router_interface_api = {
    redis_create_router_interface,
    redis_remove_router_interface,
    redis_set_router_interface_attribute,
    redis_get_router_interface_attribute,
};
