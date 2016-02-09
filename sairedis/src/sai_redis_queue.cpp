#include "sai_redis.h"

/**
 * @brief Set attribute to Queue
 * @param[in] queue_id queue id to set the attribute
 * @param[in] attr attribute to set
 *
 * @return  SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
sai_status_t  redis_set_queue_attribute(
    _In_ sai_object_id_t queue_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_QUEUE,
            queue_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Get attribute to Queue
 * @param[in] queue_id queue id to set the attribute
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return  SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
sai_status_t  redis_get_queue_attribute(
    _In_ sai_object_id_t queue_id,
    _In_ uint32_t        attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_QUEUE,
            queue_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief   Get queue statistics counters.
 *
 * @param[in] queue_id Queue id
 * @param[in] counter_ids specifies the array of counter ids
 * @param[in] number_of_counters number of counters in the array
 * @param[out] counters array of resulting counter values.
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
sai_status_t  redis_get_queue_stats(
    _In_ sai_object_id_t queue_id,
    _In_ const sai_queue_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters,
    _Out_ uint64_t* counters)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
 * @brief   Clear queue statistics counters.
 *
 * @param[in] queue_id Queue id
 * @param[in] counter_ids specifies the array of counter ids
 * @param[in] number_of_counters number of counters in the array
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
sai_status_t  redis_clear_queue_stats(
    _In_ sai_object_id_t queue_id,
    _In_ const sai_queue_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}


/**
 *  @brief Qos methods table retrieved with sai_api_query()
 */
const sai_queue_api_t redis_queue_api  = {
    redis_set_queue_attribute,
    redis_get_queue_attribute,
    redis_get_queue_stats,
    redis_clear_queue_stats,
};
