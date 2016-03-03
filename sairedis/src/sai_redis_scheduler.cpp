#include "sai_redis.h"

/**
 * @brief  Create Scheduler Profile
 *
 * @param[out] scheduler_id Scheduler id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
sai_status_t redis_create_scheduler_profile(
    _Out_ sai_object_id_t  *scheduler_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create(
            SAI_OBJECT_TYPE_SCHEDULER,
            scheduler_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief  Remove Scheduler profile
 *
 * @param[in] scheduler_id Scheduler id
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
sai_status_t redis_remove_scheduler_profile(
    _In_ sai_object_id_t scheduler_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove(
            SAI_OBJECT_TYPE_SCHEDULER,
            scheduler_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief  Set Scheduler Attribute
 *
 * @param[in] scheduler_id Scheduler id
 * @param[in] attr attribute to set
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
sai_status_t redis_set_scheduler_attribute(
    _In_ sai_object_id_t scheduler_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_SCHEDULER,
            scheduler_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief  Get Scheduler attribute
 *
 * @param[in] scheduler_id - scheduler id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success
 *        Failure status code on error
 */

sai_status_t redis_get_scheduler_attribute(
    _In_ sai_object_id_t scheduler_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_SCHEDULER,
            scheduler_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief  Scheduler methods table retrieved with sai_api_query()
 */
const sai_scheduler_api_t redis_scheduler_api = {
    redis_create_scheduler_profile,
    redis_remove_scheduler_profile,
    redis_set_scheduler_attribute,
    redis_get_scheduler_attribute,
};
