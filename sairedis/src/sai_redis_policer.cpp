#include "sai_redis.h"



/**
 * @brief Create Policer
 *
 * @param[out] policer_id - the policer id
 * @param[in] attr_count - number of attributes
 * @param[in] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
sai_status_t  redis_create_policer(
    _Out_ sai_object_id_t *policer_id,
    _In_ uint32_t attr_count, 
    _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create(
            SAI_OBJECT_TYPE_POLICER,
            policer_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief Delete policer
 *
 * @param[in] policer_id - Policer id
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
sai_status_t  redis_remove_policer(
        _In_ sai_object_id_t policer_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove(
            SAI_OBJECT_TYPE_POLICER,
            policer_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief  Set Policer attribute
 *
 * @param[in] policer_id - Policer id
 * @param[in] attr - attribute
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
sai_status_t  redis_set_policer_attribute(
    _In_ sai_object_id_t policer_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_POLICER,
            policer_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief  Get Policer attribute
 *
 * @param[in] policer_id - policer id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
sai_status_t  redis_get_policer_attribute(
    _In_ sai_object_id_t policer_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_POLICER,
            policer_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief  Get Policer Statistics
 *
 * @param[in] policer_id - policer id
 * @param[in] counter_ids - array of counter ids
 * @param[in] number_of_counters - number of counters in the array
 * @param[out] counters - array of resulting counter values.
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
sai_status_t  redis_get_policer_stats(
    _In_ sai_object_id_t policer_id,
    _In_ const sai_policer_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters,
    _Out_ uint64_t* counters)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
 * @brief Policer methods table retrieved with sai_api_query()
 */
const sai_policer_api_t redis_policer_api = {
    redis_create_policer,
    redis_remove_policer,
    redis_set_policer_attribute,
    redis_get_policer_attribute,
    redis_get_policer_stats,
};
