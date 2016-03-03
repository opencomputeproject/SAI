#include "sai_redis.h"

/**
 * @brief Set ingress priority group attribute
 * @param[in] ingress_pg_id ingress priority group id
 * @param[in] attr attribute to set
 *
 * @return  SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
sai_status_t redis_set_ingress_priority_group_attr(
    _In_ sai_object_id_t ingress_pg_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_PRIORITY_GROUP,
            ingress_pg_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Get ingress priority group attributes
 * @param[in] ingress_pg_id ingress priority group id
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list array of attributes
 *
 * @return  SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
sai_status_t redis_get_ingress_priority_group_attr(
    _In_ sai_object_id_t ingress_pg_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_PRIORITY_GROUP,
            ingress_pg_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
* @brief   Get ingress priority group statistics counters.
*
* @param[in] ingress_pg_id ingress priority group id
* @param[in] counter_ids specifies the array of counter ids
* @param[in] number_of_counters number of counters in the array
* @param[out] counters array of resulting counter values.
*
* @return SAI_STATUS_SUCCESS on success
*         Failure status code on error
*/
sai_status_t redis_get_ingress_priority_group_stats(
    _In_ sai_object_id_t ingress_pg_id,
    _In_ const sai_ingress_priority_group_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters,
    _Out_ uint64_t* counters)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
* @brief   Clear ingress priority group statistics counters.
*
* @param[in] ingress_pg_id ingress priority group id
* @param[in] counter_ids specifies the array of counter ids
* @param[in] number_of_counters number of counters in the array
*
* @return SAI_STATUS_SUCCESS on success
*         Failure status code on error
*/
sai_status_t redis_clear_ingress_priority_group_stats(
    _In_ sai_object_id_t ingress_pg_id,
    _In_ const sai_ingress_priority_group_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
 * @brief Create buffer pool
 * @param[out] pool_id buffer pool id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
sai_status_t redis_create_buffer_pool(
    _Out_ sai_object_id_t* pool_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create(
        SAI_OBJECT_TYPE_BUFFER_POOL,
        pool_id,
        attr_count,
        attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Remove buffer pool
 * @param[in] pool_id buffer pool id
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
sai_status_t redis_remove_buffer_pool(
    _In_ sai_object_id_t pool_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove(
            SAI_OBJECT_TYPE_BUFFER_POOL,
            pool_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Set buffer pool attribute
 * @param[in] pool_id buffer pool id
 * @param[in] attr attribute
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
sai_status_t redis_set_buffer_pool_attr(
    _In_ sai_object_id_t pool_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_BUFFER_POOL,
            pool_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Get buffer pool attributes
 * @param[in] pool_id buffer pool id
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list array of attributes
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
sai_status_t redis_get_buffer_pool_attr(
    _In_ sai_object_id_t pool_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_BUFFER_POOL,
            pool_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
* @brief   Get buffer pool statistics counters.
*
* @param[in] pool_id buffer pool id
* @param[in] counter_ids specifies the array of counter ids
* @param[in] number_of_counters number of counters in the array
* @param[out] counters array of resulting counter values.
*
* @return SAI_STATUS_SUCCESS on success
*         Failure status code on error
*/
sai_status_t redis_get_buffer_pool_stats(
    _In_ sai_object_id_t pool_id,
    _In_ const sai_buffer_pool_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters,
    _Out_ uint64_t* counters)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
 * @brief Create buffer profile
 * @param[out] buffer_profile_id buffer profile id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
sai_status_t redis_create_buffer_profile(
    _Out_ sai_object_id_t* buffer_profile_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create(
            SAI_OBJECT_TYPE_BUFFER_PROFILE,
            buffer_profile_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Remove buffer profile
 * @param[in] buffer_profile_id buffer profile id
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
sai_status_t redis_remove_buffer_profile(
    _In_ sai_object_id_t buffer_profile_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove(
            SAI_OBJECT_TYPE_BUFFER_PROFILE,
            buffer_profile_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Set buffer profile attribute
 * @param[in] buffer_profile_id buffer profile id
 * @param[in] attr attribute
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
sai_status_t redis_set_buffer_profile_attr(
    _In_ sai_object_id_t buffer_profile_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_BUFFER_PROFILE,
            buffer_profile_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Get buffer profile attributes
 * @param[in] buffer_profile_id buffer profile id
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list array of attributes
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
sai_status_t redis_get_buffer_profile_attr(
    _In_ sai_object_id_t buffer_profile_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_BUFFER_PROFILE,
            buffer_profile_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 *  @brief buffer methods table retrieved with sai_api_query()
 */
const sai_buffer_api_t redis_buffer_api = {
    redis_create_buffer_pool,
    redis_remove_buffer_pool,
    redis_set_buffer_pool_attr,
    redis_get_buffer_pool_attr,
    redis_get_buffer_pool_stats,

    redis_set_ingress_priority_group_attr,
    redis_get_ingress_priority_group_attr,
    redis_get_ingress_priority_group_stats,
    redis_clear_ingress_priority_group_stats,

    redis_create_buffer_profile,
    redis_remove_buffer_profile,
    redis_set_buffer_profile_attr,
    redis_get_buffer_profile_attr,
};
