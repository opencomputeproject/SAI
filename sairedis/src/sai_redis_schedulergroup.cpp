#include "sai_redis.h"

/**
 * @brief  Create Scheduler group
 *
 * @param[out] scheduler_group_id Scheudler group id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
sai_status_t redis_create_scheduler_group(
    _Out_ sai_object_id_t  *scheduler_group_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create(
            SAI_OBJECT_TYPE_SCHEDULER_GROUP,
            scheduler_group_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief  Remove Scheduler group
 *
 * @param[in] scheduler_group_id Scheudler group id
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
sai_status_t redis_remove_scheduler_group(
    _In_ sai_object_id_t scheduler_group_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove(
            SAI_OBJECT_TYPE_SCHEDULER_GROUP,
            scheduler_group_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief  Set Scheduler group Attribute
 *
 * @param[in] scheduler_group_id Scheudler group id
 * @param[in] attr attribute to set
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
sai_status_t redis_set_scheduler_group_attribute(
    _In_ sai_object_id_t scheduler_group_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set(
            SAI_OBJECT_TYPE_SCHEDULER_GROUP,
            scheduler_group_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief  Get Scheduler Group attribute
 *
 * @param[in] scheduler_group_id - scheduler group id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success
 *        Failure status code on error
 */

sai_status_t  redis_get_scheduler_group_attribute(
    _In_ sai_object_id_t scheduler_group_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get(
            SAI_OBJECT_TYPE_SCHEDULER_GROUP,
            scheduler_group_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * @brief   Add Child queue/group objects to scheduler group
 *
 * @param[in] scheduler_group_id Scheduler group id.
 * @param[in] child_count number of child count
 * @param[in] child_objects array of child objects
 *
 * @return SAI_STATUS_SUCCESS on success
 *        Failure status code on error
 */
sai_status_t  redis_add_child_object_to_group(
    _In_ sai_object_id_t scheduler_group_id,
    _In_ uint32_t        child_count,
    _In_ const sai_object_id_t* child_objects)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}


/**
 * @brief   Remove Child queue/group objects from scheduler group
 *
 * @param[in] scheduler_group_id Scheduler group id.
 * @param[in] child_count number of child count
 * @param[in] child_objects array of child objects
 *
 * @return SAI_STATUS_SUCCESS on success
 *        Failure status code on error
 */
sai_status_t  redis_remove_child_object_from_group(
    _In_ sai_object_id_t scheduler_group_id,
    _In_ uint32_t        child_count,
    _In_ const sai_object_id_t* child_objects)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
 * @brief  Scheduler Group methods table retrieved with sai_api_query()
 */
const sai_scheduler_group_api_t redis_scheduler_group_api = {
    redis_create_scheduler_group,
    redis_remove_scheduler_group,
    redis_set_scheduler_group_attribute,
    redis_get_scheduler_group_attribute,
    redis_add_child_object_to_group,
    redis_remove_child_object_from_group,
};
