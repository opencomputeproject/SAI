/**
 * Copyright (c) 2014 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
 *
 * @file    saicounter.h
 *
 * @brief   This module defines SAI Counter interface
 *
 * @par Abstract
 *
 *    This module defines SAI Counter API.
 */

#if !defined (__SAICOUNTER_H_)
#define __SAICOUNTER_H_

#include <saitypes.h>

/**
 * @defgroup SAICOUNTER SAI - Counter specific API definitions
 *
 * @{
 */

/**
 * @brief Counter type
 */
typedef enum _sai_counter_type_t
{
    /** Regular */
    SAI_COUNTER_TYPE_REGULAR,

} sai_counter_type_t;

/**
 * @brief Attribute Id in sai_set_counter_attribute() and
 * sai_get_counter_attribute() calls
 */
typedef enum _sai_counter_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_COUNTER_ATTR_START,

    /* READ-WRITE */

    /**
     * @brief Counter
     *
     * @type sai_counter_type_t
     * @flags CREATE_ONLY
     * @default SAI_COUNTER_TYPE_REGULAR
     */
    SAI_COUNTER_ATTR_TYPE = SAI_COUNTER_ATTR_START,

    /**
     * @brief Label attribute used to unique identify counter.
     *
     * @type char
     * @flags CREATE_AND_SET
     * @default ""
     */
    SAI_COUNTER_ATTR_LABEL,

    /**
     * @brief End of attributes
     */
    SAI_COUNTER_ATTR_END,

    /** Custom range base value */
    SAI_COUNTER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_COUNTER_ATTR_CUSTOM_RANGE_END

} sai_counter_attr_t;

/**
 * @brief Create counter
 *
 * @param[out] counter_id Counter id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success
 */
typedef sai_status_t (*sai_create_counter_fn)(
        _Out_ sai_object_id_t *counter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove counter
 *
 * @param[in] counter_id Counter id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_counter_fn)(
        _In_ sai_object_id_t counter_id);

/**
 * @brief Set counter attribute Value
 *
 * @param[in] counter_id Counter id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_counter_attribute_fn)(
        _In_ sai_object_id_t counter_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get counter attribute Value
 *
 * @param[in] counter_id Counter id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_counter_attribute_fn)(
        _In_ sai_object_id_t counter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Enum defining statistics for Counter.
 */
typedef enum _sai_counter_stat_t
{
    /** Get/set hit packets count [uint64_t] */
    SAI_COUNTER_STAT_PACKETS = 0x00000000,

    /** Get/set hit bytes count [uint64_t] */
    SAI_COUNTER_STAT_BYTES = 0x00000001,

    /** Custom range base value */
    SAI_COUNTER_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_counter_stat_t;

/**
 * @brief Get counter statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] counter_id Counter id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_counter_stats_fn)(
        _In_ sai_object_id_t counter_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get counter statistics counters extended.
 *
 * @param[in] counter_id Counter id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_counter_stats_ext_fn)(
        _In_ sai_object_id_t counter_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear counter statistics counters.
 *
 * @param[in] counter_id Counter id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_counter_stats_fn)(
        _In_ sai_object_id_t counter_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Counter methods table retrieved with sai_api_query()
 */
typedef struct _sai_counter_api_t
{
    sai_create_counter_fn        create_counter;
    sai_remove_counter_fn        remove_counter;
    sai_set_counter_attribute_fn set_counter_attribute;
    sai_get_counter_attribute_fn get_counter_attribute;
    sai_get_counter_stats_fn     get_counter_stats;
    sai_get_counter_stats_ext_fn get_counter_stats_ext;
    sai_clear_counter_stats_fn   clear_counter_stats;

} sai_counter_api_t;

/**
 * @}
 */
#endif /** __SAICOUNTER_H_ */
