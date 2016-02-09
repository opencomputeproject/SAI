#include "sai_redis.h"

/**
 * Routine Description:
 *    @brief Create a VLAN
 *
 * Arguments:
 *    @param[in] vlan_id - VLAN id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_create_vlan(
    _In_ sai_vlan_id_t vlan_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_create_vlan(
            SAI_OBJECT_TYPE_VLAN,
            vlan_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Remove a VLAN
 *
 * Arguments:
 *    @param[in] vlan_id - VLAN id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t redis_remove_vlan(
    _In_ sai_vlan_id_t vlan_id)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_remove_vlan(
            SAI_OBJECT_TYPE_VLAN,
            vlan_id);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Set VLAN attribute Value
 *
 * Arguments:
 *    @param[in] vlan_id - VLAN id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_set_vlan_attribute(
    _In_ sai_vlan_id_t vlan_id,
    _In_ const sai_attribute_t *attr)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_set_vlan(
            SAI_OBJECT_TYPE_VLAN,
            vlan_id,
            attr);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Get VLAN attribute Value
 *
 * Arguments:
 *    @param[in] vlan_id - VLAN id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_get_vlan_attribute(
    _In_ sai_vlan_id_t vlan_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    sai_status_t status = redis_generic_get_vlan(
            SAI_OBJECT_TYPE_VLAN,
            vlan_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

/**
 * Routine Description:
 *    @brief Remove VLAN configuration (remove all VLANs).
 *
 * Arguments:
 *    None
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_remove_all_vlans(void)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
 * Routine Description:
 *    @brief Add Port to VLAN
 *
 * Arguments:
 *    @param[in] vlan_id - VLAN id
 *    @param[in] port_count - number of ports
 *    @param[in] port_list - pointer to membership structures
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_add_ports_to_vlan(
    _In_ sai_vlan_id_t vlan_id,
    _In_ uint32_t port_count,
    _In_ const sai_vlan_port_t *port_list)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
 * Routine Description:
 *    @brief Remove Port from VLAN
 *
 * Arguments:
 *    @param[in] vlan_id - VLAN id
 *    @param[in] port_count - number of ports
 *    @param[in] port_list - pointer to membership structures
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_remove_ports_from_vlan(
    _In_ sai_vlan_id_t vlan_id,
    _In_ uint32_t port_count,
    _In_ const sai_vlan_port_t* port_list)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
 * Routine Description:
 *   @brief Get vlan statistics counters.
 *
 * Arguments:
 *    @param[in] vlan_id - VLAN id
 *    @param[in] counter_ids - specifies the array of counter ids
 *    @param[in] number_of_counters - number of counters in the array
 *    @param[out] counters - array of resulting counter values.
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_get_vlan_stats(
    _In_ sai_vlan_id_t vlan_id,
    _In_ const sai_vlan_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters,
    _Out_ uint64_t* counters)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
 * Routine Description:
 *   @brief Clear vlan statistics counters.
 *
 * Arguments:
 *    @param[in] vlan_id - vlan id
 *    @param[in] counter_ids - specifies the array of counter ids
 *    @param[in] number_of_counters - number of counters in the array
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
sai_status_t  redis_clear_vlan_stats(
    _In_ sai_vlan_id_t vlan_id,
    _In_ const sai_vlan_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters)
{
    REDIS_LOG_ENTER();

    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
}

/**
 * @brief VLAN methods table retrieved with sai_api_query()
 */
const sai_vlan_api_t redis_vlan_api = {
    redis_create_vlan,
    redis_remove_vlan,
    redis_set_vlan_attribute,
    redis_get_vlan_attribute,
    redis_add_ports_to_vlan,
    redis_remove_ports_from_vlan,
    redis_remove_all_vlans,
    redis_get_vlan_stats,
    redis_clear_vlan_stats,
};
