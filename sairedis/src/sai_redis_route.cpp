#include "sai_redis.h"

/**
 * Routine Description:
 *    @brief Create Route
 *
 * Arguments:
 *    @param[in] unicast_route_entry - route entry
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 *
 * Note: IP prefix/mask expected in Network Byte Order.
 *
 */
sai_status_t  redis_create_route(
    _In_ const sai_unicast_route_entry_t* unicast_route_entry,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create(
            SAI_OBJECT_TYPE_ROUTE,
            unicast_route_entry,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Remove Route
 *
 * Arguments:
 *    @param[in] unicast_route_entry - route entry
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 *
 * Note: IP prefix/mask expected in Network Byte Order.
 */
sai_status_t  redis_remove_route(
    _In_ const sai_unicast_route_entry_t* unicast_route_entry)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove(
            SAI_OBJECT_TYPE_ROUTE,
            unicast_route_entry);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Set route attribute value
 *
 * Arguments:
 *    @param[in] unicast_route_entry - route entry
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_set_route_attribute(
    _In_ const sai_unicast_route_entry_t* unicast_route_entry,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_ROUTE,
            unicast_route_entry,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Get route attribute value
 *
 * Arguments:
 *    @param[in] unicast_route_entry - route entry
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_get_route_attribute(
    _In_ const sai_unicast_route_entry_t* unicast_route_entry,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_ROUTE,
            unicast_route_entry,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}


/**
 *  @brief Router entry methods table retrieved with sai_api_query()
 */
const sai_route_api_t redis_route_api = {
    redis_create_route,
    redis_remove_route,
    redis_set_route_attribute,
    redis_get_route_attribute,
};
