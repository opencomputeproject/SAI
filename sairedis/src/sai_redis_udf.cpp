#include "sai_redis.h"

/**
 * Routine Description:
 *    @brief Create UDF
 *
 * Arguments:
 *    @param[out] udf_id - UDF id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 *
 */
sai_status_t  redis_create_udf(
    _Out_ sai_object_id_t* udf_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create(
            SAI_OBJECT_TYPE_UDF,
            udf_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Remove UDF
 *
 * Arguments:
 *    @param[in] udf_id - UDF id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t redis_remove_udf(
    _In_ sai_object_id_t udf_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove(
            SAI_OBJECT_TYPE_UDF,
            udf_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Set UDF attribute
 *
 * Arguments:
 *    @param[in] udf_id - UDF id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_set_udf_attribute(
    _In_ sai_object_id_t udf_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_UDF,
            udf_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Get UDF attribute value
 *
 * Arguments:
 *    @param[in] udf_id - UDF id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attrs - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_get_udf_attribute(
    _In_ sai_object_id_t udf_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_UDF,
            udf_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Create UDF match
 *
 * Arguments:
 *    @param[out] udf_match_id - UDF match id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 *
 */
sai_status_t  redis_create_udf_match(
    _Out_ sai_object_id_t* udf_match_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create(
            SAI_OBJECT_TYPE_UDF_MATCH,
            udf_match_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Remove UDF match
 *
 * Arguments:
 *    @param[in] udf_match_id - UDF match id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_remove_udf_match(
    _In_ sai_object_id_t udf_match_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove(
            SAI_OBJECT_TYPE_UDF_MATCH,
            udf_match_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Set UDF match attribute
 *
 * Arguments:
 *    @param[in] udf_match_id - UDF match id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_set_udf_match_attribute(
    _In_ sai_object_id_t udf_match_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_UDF_MATCH,
            udf_match_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Get UDF match attribute value
 *
 * Arguments:
 *    @param[in] udf_match_id - UDF match id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attrs - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_get_udf_match_attribute(
    _In_ sai_object_id_t udf_match_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_UDF_MATCH,
            udf_match_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Create UDF group
 *
 * Arguments:
 *    @param[out] udf_group_id - UDF group id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 *
 */
sai_status_t  redis_create_udf_group(
    _Out_ sai_object_id_t* udf_group_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create(
            SAI_OBJECT_TYPE_UDF_GROUP,
            udf_group_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Remove UDF group
 *
 * Arguments:
 *    @param[in] udf_group_id - UDF group id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_remove_udf_group(
    _In_ sai_object_id_t udf_group_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove(
            SAI_OBJECT_TYPE_UDF_GROUP,
            udf_group_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Set UDF group attribute
 *
 * Arguments:
 *    @param[in] udf_group_id - UDF group id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_set_udf_group_attribute(
    _In_ sai_object_id_t udf_group_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_UDF_GROUP,
            udf_group_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Get UDF group attribute value
 *
 * Arguments:
 *    @param[in] udf_group_id - UDF group id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attrs - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_get_udf_group_attribute(
    _In_ sai_object_id_t udf_group_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_UDF_GROUP,
            udf_group_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 *  @brief UDF methods, retrieved via sai_api_query()
 */
const sai_udf_api_t redis_udf_api = {
    redis_create_udf,
    redis_remove_udf,
    redis_set_udf_attribute,
    redis_get_udf_attribute,
    redis_create_udf_match,
    redis_remove_udf_match,
    redis_set_udf_match_attribute,
    redis_get_udf_match_attribute,
    redis_create_udf_group,
    redis_remove_udf_group,
    redis_set_udf_group_attribute,
    redis_get_udf_group_attribute,
};
