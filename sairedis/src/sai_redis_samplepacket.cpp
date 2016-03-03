#include "sai_redis.h"

/**
 * @brief Create samplepacket session.
 *
 * @param[out] session_id samplepacket session id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t redis_create_samplepacket_session(
        _Out_ sai_object_id_t *session_id,
        _In_ uint32_t attr_count, 
        _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create(
            SAI_OBJECT_TYPE_SAMPLEPACKET,
            session_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Remove samplepacket session.
 *
 * @param[in] session_id samplepacket session id
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t  redis_remove_samplepacket_session(
       _In_ sai_object_id_t session_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove(
            SAI_OBJECT_TYPE_SAMPLEPACKET,
            session_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Set samplepacket session attributes.
 *
 * @param[in] session_id samplepacket session id
 * @param[in] attr Value of attribute
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t  redis_set_samplepacket_attribute(
        _In_ sai_object_id_t session_id, 
        _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_SAMPLEPACKET,
            session_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Get samplepacket session attributes.
 *
 * @param[in] session_id samplepacket session id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Value of attribute
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t  redis_get_samplepacket_attribute(
        _In_ sai_object_id_t session_id,
        _In_ uint32_t attr_count, 
        _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_SAMPLEPACKET,
            session_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief samplepacket method table retrieved with sai_api_query()
 */
const sai_samplepacket_api_t redis_samplepacket_api = {
    redis_create_samplepacket_session,
    redis_remove_samplepacket_session,
    redis_set_samplepacket_attribute,
    redis_get_samplepacket_attribute,
};
