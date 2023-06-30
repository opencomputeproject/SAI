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
 * @brief   This module defines SAI P4 extension  interface
 */

#if !defined (__SAIEXPERIMENTALDASHMETER_H_)
#define __SAIEXPERIMENTALDASHMETER_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALDASH_METER SAI - Extension specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for dash_meter_meter_bucket
 */
typedef enum _sai_meter_bucket_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_METER_BUCKET_ATTR_START,

    /**
     * @brief Exact matched key eni_id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_ENI
     */
    SAI_METER_BUCKET_ATTR_ENI_ID = SAI_METER_BUCKET_ATTR_START,

    /**
     * @brief Exact matched key meter_class
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan false
     */
    SAI_METER_BUCKET_ATTR_METER_CLASS,

    /**
     * @brief Action meter_bucket_action parameter OUTBOUND_BYTES_COUNTER
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_METER_BUCKET_ATTR_OUTBOUND_BYTES_COUNTER,

    /**
     * @brief Action meter_bucket_action parameter INBOUND_BYTES_COUNTER
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_METER_BUCKET_ATTR_INBOUND_BYTES_COUNTER,

    /**
     * @brief End of attributes
     */
    SAI_METER_BUCKET_ATTR_END,

    /** Custom range base value */
    SAI_METER_BUCKET_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_METER_BUCKET_ATTR_CUSTOM_RANGE_END,

} sai_meter_bucket_attr_t;

/**
 * @brief Attribute ID for dash_meter_meter_policy
 */
typedef enum _sai_meter_policy_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_METER_POLICY_ATTR_START,

    /**
     * @brief Action check_ip_addr_family parameter IP_ADDR_FAMILY
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
 * @brief Attribute ID for dash_meter_meter_rule
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
     * @brief Ternary matched mask dip
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_METER_RULE_ATTR_DIP_MASK,

    /**
     * @brief Action set_policy_meter_class parameter METER_CLASS
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
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
 * @brief Create dash_meter_meter_bucket
 *
 * @param[out] meter_bucket_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_meter_bucket_fn)(
        _Out_ sai_object_id_t *meter_bucket_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_meter_meter_bucket
 *
 * @param[in] meter_bucket_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_meter_bucket_fn)(
        _In_ sai_object_id_t meter_bucket_id);

/**
 * @brief Set attribute for dash_meter_meter_bucket
 *
 * @param[in] meter_bucket_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_meter_bucket_attribute_fn)(
        _In_ sai_object_id_t meter_bucket_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_meter_meter_bucket
 *
 * @param[in] meter_bucket_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_meter_bucket_attribute_fn)(
        _In_ sai_object_id_t meter_bucket_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create dash_meter_meter_policy
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
 * @brief Remove dash_meter_meter_policy
 *
 * @param[in] meter_policy_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_meter_policy_fn)(
        _In_ sai_object_id_t meter_policy_id);

/**
 * @brief Set attribute for dash_meter_meter_policy
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
 * @brief Get attribute for dash_meter_meter_policy
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
 * @brief Create dash_meter_meter_rule
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
 * @brief Remove dash_meter_meter_rule
 *
 * @param[in] meter_rule_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_meter_rule_fn)(
        _In_ sai_object_id_t meter_rule_id);

/**
 * @brief Set attribute for dash_meter_meter_rule
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
 * @brief Get attribute for dash_meter_meter_rule
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
    sai_create_meter_bucket_fn           create_meter_bucket;
    sai_remove_meter_bucket_fn           remove_meter_bucket;
    sai_set_meter_bucket_attribute_fn    set_meter_bucket_attribute;
    sai_get_meter_bucket_attribute_fn    get_meter_bucket_attribute;
    sai_bulk_object_create_fn            create_meter_buckets;
    sai_bulk_object_remove_fn            remove_meter_buckets;

    sai_create_meter_policy_fn           create_meter_policy;
    sai_remove_meter_policy_fn           remove_meter_policy;
    sai_set_meter_policy_attribute_fn    set_meter_policy_attribute;
    sai_get_meter_policy_attribute_fn    get_meter_policy_attribute;
    sai_bulk_object_create_fn            create_meter_policys;
    sai_bulk_object_remove_fn            remove_meter_policys;

    sai_create_meter_rule_fn             create_meter_rule;
    sai_remove_meter_rule_fn             remove_meter_rule;
    sai_set_meter_rule_attribute_fn      set_meter_rule_attribute;
    sai_get_meter_rule_attribute_fn      get_meter_rule_attribute;
    sai_bulk_object_create_fn            create_meter_rules;
    sai_bulk_object_remove_fn            remove_meter_rules;

} sai_dash_meter_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHMETER_H_ */
