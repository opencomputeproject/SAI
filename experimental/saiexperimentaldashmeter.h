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
 * @file    saiexperimentaldashmeter.h
 *
 * @brief   This module defines SAI extensions for DASH meter
 *
 * @warning This module is a SAI experimental module
 */

#if !defined (__SAIEXPERIMENTALDASHMETER_H_)
#define __SAIEXPERIMENTALDASHMETER_H_

#include <saitypesextensions.h>

/**
 * @defgroup SAIEXPERIMENTALDASHMETER SAI - Experimental: DASH meter specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_METER_BUCKET_ENTRY_ATTR_ACTION
 */
typedef enum _sai_meter_bucket_entry_action_t
{
    SAI_METER_BUCKET_ENTRY_ACTION_UPDATE_METER_BUCKET,

} sai_meter_bucket_entry_action_t;

/**
 * @brief Entry for meter_bucket_entry
 */
typedef struct _sai_meter_bucket_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key eni_id
     *
     * @objects SAI_OBJECT_TYPE_ENI
     */
    sai_object_id_t eni_id;

    /**
     * @brief Exact matched key meter_class
     */
    sai_uint32_t meter_class;

} sai_meter_bucket_entry_t;

/**
 * @brief Attribute ID for meter bucket entry
 */
typedef enum _sai_meter_bucket_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_METER_BUCKET_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_meter_bucket_entry_action_t
     * @flags CREATE_AND_SET
     * @default SAI_METER_BUCKET_ENTRY_ACTION_UPDATE_METER_BUCKET
     */
    SAI_METER_BUCKET_ENTRY_ATTR_ACTION = SAI_METER_BUCKET_ENTRY_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_METER_BUCKET_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_METER_BUCKET_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_METER_BUCKET_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_meter_bucket_entry_attr_t;

/**
 * @brief Counter IDs for meter bucket entry
 */
typedef enum _sai_meter_bucket_entry_stat_t
{
    /** DASH METER_BUCKET_ENTRY OUTBOUND_BYTES stat count */
    SAI_METER_BUCKET_ENTRY_STAT_OUTBOUND_BYTES,

    /** DASH METER_BUCKET_ENTRY INBOUND_BYTES stat count */
    SAI_METER_BUCKET_ENTRY_STAT_INBOUND_BYTES,

} sai_meter_bucket_entry_stat_t;

/**
 * @brief Attribute ID for meter policy
 */
typedef enum _sai_meter_policy_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_METER_POLICY_ATTR_START,

    /**
     * @brief Action parameter IP address family
     *
     * @type sai_ip_addr_family_t
     * @flags CREATE_AND_SET
     * @default SAI_IP_ADDR_FAMILY_IPV4
     * @isresourcetype true
     */
    SAI_METER_POLICY_ATTR_IP_ADDR_FAMILY = SAI_METER_POLICY_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_METER_POLICY_ATTR_END,

    /** Custom range base value */
    SAI_METER_POLICY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_METER_POLICY_ATTR_CUSTOM_RANGE_END,

} sai_meter_policy_attr_t;

/**
 * @brief Attribute ID for meter rule
 */
typedef enum _sai_meter_rule_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_METER_RULE_ATTR_START,

    /**
     * @brief Exact matched key meter_policy_id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_METER_POLICY
     * @isresourcetype true
     */
    SAI_METER_RULE_ATTR_METER_POLICY_ID = SAI_METER_RULE_ATTR_START,

    /**
     * @brief Ternary matched key dip
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_METER_RULE_ATTR_DIP,

    /**
     * @brief Ternary matched key dip mask
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_METER_RULE_ATTR_DIP_MASK,

    /**
     * @brief Action parameter meter class
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_METER_RULE_ATTR_METER_CLASS,

    /**
     * @brief Rule priority in table
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_METER_RULE_ATTR_PRIORITY,

    /**
     * @brief IP address family for resource accounting
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_METER_RULE_ATTR_IP_ADDR_FAMILY,

    /**
     * @brief End of attributes
     */
    SAI_METER_RULE_ATTR_END,

    /** Custom range base value */
    SAI_METER_RULE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_METER_RULE_ATTR_CUSTOM_RANGE_END,

} sai_meter_rule_attr_t;

/**
 * @brief Create meter bucket entry
 *
 * @param[in] meter_bucket_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_meter_bucket_entry_fn)(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove meter bucket entry
 *
 * @param[in] meter_bucket_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_meter_bucket_entry_fn)(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry);

/**
 * @brief Set attribute for meter bucket entry
 *
 * @param[in] meter_bucket_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_meter_bucket_entry_attribute_fn)(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for meter bucket entry
 *
 * @param[in] meter_bucket_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_meter_bucket_entry_attribute_fn)(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get METER_BUCKET_ENTRY statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] meter_bucket_entry Entry
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_meter_bucket_entry_stats_fn)(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get METER_BUCKET_ENTRY statistics counters extended.
 *
 * @param[in] meter_bucket_entry Entry
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_meter_bucket_entry_stats_ext_fn)(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear METER_BUCKET_ENTRY statistics counters.
 *
 * @param[in] meter_bucket_entry Entry
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_meter_bucket_entry_stats_fn)(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Bulk create meter bucket entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] meter_bucket_entry List of object to create
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to create.
 * @param[in] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are created or
 * #SAI_STATUS_FAILURE when any of the objects fails to create. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_create_meter_bucket_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove meter bucket entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] meter_bucket_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_meter_bucket_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create meter policy
 *
 * @param[out] meter_policy_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_meter_policy_fn)(
        _Out_ sai_object_id_t *meter_policy_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove meter policy
 *
 * @param[in] meter_policy_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_meter_policy_fn)(
        _In_ sai_object_id_t meter_policy_id);

/**
 * @brief Set attribute for meter policy
 *
 * @param[in] meter_policy_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_meter_policy_attribute_fn)(
        _In_ sai_object_id_t meter_policy_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for meter policy
 *
 * @param[in] meter_policy_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_meter_policy_attribute_fn)(
        _In_ sai_object_id_t meter_policy_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create meter rule
 *
 * @param[out] meter_rule_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_meter_rule_fn)(
        _Out_ sai_object_id_t *meter_rule_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove meter rule
 *
 * @param[in] meter_rule_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_meter_rule_fn)(
        _In_ sai_object_id_t meter_rule_id);

/**
 * @brief Set attribute for meter rule
 *
 * @param[in] meter_rule_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_meter_rule_attribute_fn)(
        _In_ sai_object_id_t meter_rule_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for meter rule
 *
 * @param[in] meter_rule_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_meter_rule_attribute_fn)(
        _In_ sai_object_id_t meter_rule_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_dash_meter_api_t
{
    sai_create_meter_bucket_entry_fn           create_meter_bucket_entry;
    sai_remove_meter_bucket_entry_fn           remove_meter_bucket_entry;
    sai_set_meter_bucket_entry_attribute_fn    set_meter_bucket_entry_attribute;
    sai_get_meter_bucket_entry_attribute_fn    get_meter_bucket_entry_attribute;
    sai_get_meter_bucket_entry_stats_fn        get_meter_bucket_entry_stats;
    sai_get_meter_bucket_entry_stats_ext_fn    get_meter_bucket_entry_stats_ext;
    sai_clear_meter_bucket_entry_stats_fn      clear_meter_bucket_entry_stats;
    sai_bulk_create_meter_bucket_entry_fn      create_meter_bucket_entries;
    sai_bulk_remove_meter_bucket_entry_fn      remove_meter_bucket_entries;

    sai_create_meter_policy_fn                 create_meter_policy;
    sai_remove_meter_policy_fn                 remove_meter_policy;
    sai_set_meter_policy_attribute_fn          set_meter_policy_attribute;
    sai_get_meter_policy_attribute_fn          get_meter_policy_attribute;
    sai_bulk_object_create_fn                  create_meter_policys;
    sai_bulk_object_remove_fn                  remove_meter_policys;

    sai_create_meter_rule_fn                   create_meter_rule;
    sai_remove_meter_rule_fn                   remove_meter_rule;
    sai_set_meter_rule_attribute_fn            set_meter_rule_attribute;
    sai_get_meter_rule_attribute_fn            get_meter_rule_attribute;
    sai_bulk_object_create_fn                  create_meter_rules;
    sai_bulk_object_remove_fn                  remove_meter_rules;

} sai_dash_meter_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHMETER_H_ */
