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
 * @file    saistatefultable.h
 *
 * @brief   This module defines SAI stateful table interface
 */

#if !defined (__SAISTATEFULTABLE_H_)
#define __SAISTATEFULTABLE_H_

#include <saitypes.h>

/**
 * @defgroup SAISTATEFULTABLE SAI - stateful table specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_STATEFUL_TABLE_ATTR_EVICTION_POLICY
 */
typedef enum _sai_stateful_table_eviction_policy_t
{
    /** Ignore a new flow */
    SAI_STATEFUL_TABLE_EVICTION_POLICY_IGNORE,

    /** Least recently used flow is evicted */
    SAI_STATEFUL_TABLE_EVICTION_POLICY_LRU,

} sai_stateful_table_eviction_policy_t;

typedef enum _sai_flow_key_field_t
{
    SAI_FLOW_KEY_FIELD_SRC_IPV6,

    SAI_FLOW_KEY_FIELD_DST_IPV6,

    SAI_FLOW_KEY_FIELD_INNER_SRC_IPV6,

    SAI_FLOW_KEY_FIELD_INNER_DST_IPV6,

    SAI_FLOW_KEY_FIELD_SRC_MAC,

    SAI_FLOW_KEY_FIELD_DST_MAC,

    SAI_FLOW_KEY_FIELD_SRC_IP,

    SAI_FLOW_KEY_FIELD_DST_IP,

    SAI_FLOW_KEY_FIELD_INNER_SRC_IP,

    SAI_FLOW_KEY_FIELD_INNER_DST_IP,

    SAI_FLOW_KEY_FIELD_L4_SRC_PORT,

    SAI_FLOW_KEY_FIELD_L4_DST_PORT,

    SAI_FLOW_KEY_FIELD_INNER_L4_SRC_PORT,

    SAI_FLOW_KEY_FIELD_INNER_L4_DST_PORT,

    SAI_FLOW_KEY_FIELD_IP_PROTOCOL,

    SAI_FLOW_KEY_FIELD_INNER_IP_PROTOCOL,

} sai_flow_key_field_t;

/**
 * @brief Attribute Id for sai_stateful_table
 */
typedef enum _sai_stateful_table_attr_t
{
    /**
     * @brief Table attributes start
     */
    SAI_STATEFUL_TABLE_ATTR_START,

    /**
     * @brief Maximum number of entries in the table
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_STATEFUL_TABLE_ATTR_SIZE = SAI_STATEFUL_TABLE_ATTR_START,

    /**
     * @brief Flow key #0
     *
     * @type sai_s32_list_t sai_flow_key_field_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_STATEFUL_TABLE_ATTR_KEY_0,

    /**
     * @brief Flow key #1
     *
     * @type sai_s32_list_t sai_flow_key_field_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_STATEFUL_TABLE_ATTR_KEY_1,

    /**
     * @brief Eviction policy
     *
     * Defines the eviction policy for the entries in a stateful table
     * in case when a table is full and a new flow learn event happens
     *
     * @type sai_stateful_table_eviction_policy_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @default SAI_STATEFUL_TABLE_EVICTION_POLICY_IGNORE
     */
    SAI_STATEFUL_TABLE_ATTR_EVICTION_POLICY,

    /**
     * @brief Flow context
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @default 0
     */
    SAI_STATEFUL_TABLE_ATTR_FLOW_CONTEXT_SIZE,

    /**
     * @brief Global context
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @default 0
     */
    SAI_STATEFUL_TABLE_ATTR_GLOBAL_CONTEXT_SIZE,

    /**
     * @brief State functions implementation
     *
     * The implementation is provided in the format of
     * restircted C code, as described by the accompanying documentation
     *
     * @type sai_u8_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_STATEFUL_TABLE_ATTR_STATE_GRAPH,

    /**
     * @brief End of stateful table attributes
     */
    SAI_STATEFUL_TABLE_ATTR_END,

    /**
     * @brief Custom range base value start
     */
    SAI_STATEFUL_TABLE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /**
     * @brief End of Custom range base
     */
    SAI_STATEFUL_TABLE_ATTR_CUSTOM_RANGE_END

} sai_stateful_table_attr_t;

/**
 * @brief Create stateful table
 *
 * @param[out] stateful_table_id The stateful table id
 * @param[in] switch_id Switch Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_stateful_table_fn)(
        _Out_ sai_object_id_t *stateful_table_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete the stateful table
 *
 * @param[in] stateful_table_id The stateful table id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_stateful_table_fn)(
        _In_ sai_object_id_t stateful_table_id);

/**
 * @brief Set stateful table attribute
 *
 * @param[in] stateful_table_id The stateful table id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_stateful_table_attribute_fn)(
        _In_ sai_object_id_t stateful_table_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get stateful table attribute
 *
 * @param[in] stateful_table_id stateful table id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_stateful_table_attribute_fn)(
        _In_ sai_object_id_t stateful_table_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Stateful table methods table retrieved with sai_api_query()
 */
typedef struct _sai_stateful_table_api_t
{
    sai_create_stateful_table_table_fn         create_stateful_table_table;
    sai_remove_stateful_table_table_fn         remove_stateful_table_table;
    sai_set_stateful_table_table_attribute_fn  set_stateful_table_table_attribute;
    sai_get_stateful_table_table_attribute_fn  get_stateful_table_table_attribute;
} sai_stateful_table_api_t;

/**
 * @}
 */
#endif /** __SAISTATEFULTABLE_H_ */
