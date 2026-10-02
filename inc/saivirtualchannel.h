/**
 * Copyright (c) 2025 Microsoft Open Technologies, Inc.
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
 * @file    saivirtualchannel.h
 *
 * @brief   This module defines SAI Virtual Channel interface
 */

#if !defined (__SAIVIRTUALCHANNEL_H_)
#define __SAIVIRTUALCHANNEL_H_

#include <saitypes.h>

/**
 * @defgroup SAIVIRTUALCHANNEL SAI - Virtual Channel specific API definitions
 *
 * @{
 */

/**
 * @brief CBFC Credit Profile threshold mode.
 */
typedef enum _sai_cbfc_credit_profile_threshold_mode_t
{
    /** No access to shared credits */
    SAI_CBFC_CREDIT_PROFILE_THRESHOLD_MODE_NONE,

    /** Static maximum */
    SAI_CBFC_CREDIT_PROFILE_THRESHOLD_MODE_STATIC,

    /** Dynamic maximum (relative) */
    SAI_CBFC_CREDIT_PROFILE_THRESHOLD_MODE_DYNAMIC,

} sai_cbfc_credit_profile_threshold_mode_t;

/**
 * @brief Enum defining attributes for Virtual Channel
 */
typedef enum _sai_virtual_channel_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_VIRTUAL_CHANNEL_ATTR_START,

    /**
     * @brief Port Id of the port to which the VC belongs
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_VIRTUAL_CHANNEL_ATTR_PORT = SAI_VIRTUAL_CHANNEL_ATTR_START,

    /**
     * @brief Virtual Channel index Range 0-31
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     */
    SAI_VIRTUAL_CHANNEL_ATTR_INDEX,

    /**
     * @brief Get receiver credit limit for this VC.
     * If 0, receiver sets port credit limit.
     * Upto 2^20-1.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_VIRTUAL_CHANNEL_ATTR_CBFC_RECEIVER_NATIVE_CREDIT_LIMIT,

    /**
     * @brief CBFC Credit profile pointer.
     * Default no profile, i.e., best effort VC.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_CBFC_CREDIT_PROFILE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_VIRTUAL_CHANNEL_ATTR_CBFC_SENDER_CREDIT_PROFILE,

    /**
     * @brief Enable Lossless CBFC Receiver on this VC.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_VIRTUAL_CHANNEL_ATTR_CBFC_RECEIVER_ENABLE,

    /**
     * @brief Enable Lossless CBFC Sender on this VC.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_VIRTUAL_CHANNEL_ATTR_CBFC_SENDER_ENABLE,

    /**
     * @brief End of attributes
     */
    SAI_VIRTUAL_CHANNEL_ATTR_END,

    /** Custom range base value */
    SAI_VIRTUAL_CHANNEL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_VIRTUAL_CHANNEL_ATTR_CUSTOM_RANGE_END

} sai_virtual_channel_attr_t;

/**
 * @brief Enum defining statistics for Virtual Channel.
 */
typedef enum _sai_virtual_channel_stat_t
{
    /** Get sender credits used */
    SAI_VIRTUAL_CHANNEL_STAT_SENDER_CREDITS_USED,

    /** Get sender credits consumed */
    SAI_VIRTUAL_CHANNEL_STAT_SENDER_CREDITS_CONSUMED,

    /** Get sender credits freed */
    SAI_VIRTUAL_CHANNEL_STAT_SENDER_CREDITS_FREED,

    /** Get sender credits used watermark */
    SAI_VIRTUAL_CHANNEL_STAT_SENDER_CREDITS_USED_WATERMARK,

    /** Get receiver credits consumed */
    SAI_VIRTUAL_CHANNEL_STAT_RECEIVER_CREDITS_CONSUMED,

    /** Get receiver credits freed */
    SAI_VIRTUAL_CHANNEL_STAT_RECEIVER_CREDITS_FREED,

    /** Custom range base value */
    SAI_VIRTUAL_CHANNEL_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_virtual_channel_stat_t;

/**
 * @brief Create Virtual Channel
 *
 * @param[out] virtual_channel_id Virtual Channel id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_virtual_channel_fn)(
        _Out_ sai_object_id_t *virtual_channel_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Virtual Channel
 *
 * @param[in] virtual_channel_id Virtual Channel id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_virtual_channel_fn)(
        _In_ sai_object_id_t virtual_channel_id);

/**
 * @brief Set Virtual Channel attribute
 *
 * @param[in] virtual_channel_id Virtual Channel id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_virtual_channel_attribute_fn)(
        _In_ sai_object_id_t virtual_channel_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Virtual Channel attributes
 *
 * @param[in] virtual_channel_id Virtual Channel id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_virtual_channel_attribute_fn)(
        _In_ sai_object_id_t virtual_channel_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get Virtual Channel statistics counters.
 *
 * @param[in] virtual_channel_id Virtual Channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_virtual_channel_stats_fn)(
        _In_ sai_object_id_t virtual_channel_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get Virtual Channel statistics counters extended.
 *
 * @param[in] virtual_channel_id Virtual Channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_virtual_channel_stats_ext_fn)(
        _In_ sai_object_id_t virtual_channel_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear Virtual Channel statistics counters.
 *
 * @param[in] virtual_channel_id Virtual Channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_virtual_channel_stats_fn)(
        _In_ sai_object_id_t virtual_channel_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Enum defining attributes for CBFC Credit Pool
 */
typedef enum _sai_cbfc_credit_pool_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_CBFC_CREDIT_POOL_ATTR_START,

    /**
     * @brief Port Id of the port to which the pool belongs.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_CBFC_CREDIT_POOL_ATTR_PORT = SAI_CBFC_CREDIT_POOL_ATTR_START,

    /**
     * @brief CBFC credit pool size in credits.
     *
     * @type sai_uint64_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_CBFC_CREDIT_POOL_ATTR_SIZE,

    /**
     * @brief Shared CBFC credit pool size in credits.
     * This is derived from subtracting all reserved credits
     * of VCs associated with this credit pool from the total
     * pool size
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_CBFC_CREDIT_POOL_ATTR_SHARED_SIZE,

    /**
     * @brief End of attributes
     */
    SAI_CBFC_CREDIT_POOL_ATTR_END,

    /** Custom range base value */
    SAI_CBFC_CREDIT_POOL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_CBFC_CREDIT_POOL_ATTR_CUSTOM_RANGE_END

} sai_cbfc_credit_pool_attr_t;

/**
 * @brief Create CBFC Credit Pool
 *
 * @param[out] cbfc_credit_pool_id CBFC Credit Pool id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_cbfc_credit_pool_fn)(
        _Out_ sai_object_id_t *cbfc_credit_pool_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove CBFC Credit Pool
 *
 * @param[in] cbfc_credit_pool_id CBFC Credit Pool id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_cbfc_credit_pool_fn)(
        _In_ sai_object_id_t cbfc_credit_pool_id);

/**
 * @brief Set CBFC Credit Pool Attribute
 *
 * @param[in] cbfc_credit_pool_id CBFC Credit Pool id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_cbfc_credit_pool_attribute_fn)(
        _In_ sai_object_id_t cbfc_credit_pool_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get CBFC Credit Pool attribute
 *
 * @param[in] cbfc_credit_pool_id CBFC Credit Pool id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_cbfc_credit_pool_attribute_fn)(
        _In_ sai_object_id_t cbfc_credit_pool_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Enum defining attributes for CBFC Credit Profile
 */
typedef enum _sai_cbfc_credit_profile_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_CBFC_CREDIT_PROFILE_ATTR_START,

    /**
     * @brief Pointer to credit pool object ID.
     * Can be NULL when credit pools are not used (e.g., per VC credit limit).
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_CBFC_CREDIT_POOL
     * @allownull true
     */
    SAI_CBFC_CREDIT_PROFILE_ATTR_POOL_ID = SAI_CBFC_CREDIT_PROFILE_ATTR_START,

    /**
     * @brief Reserved credits for the VC. Represents dedicated buffer allocation.
     *
     * @type sai_uint64_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_CBFC_CREDIT_PROFILE_ATTR_RESERVED_CREDIT_SIZE,

    /**
     * @brief Shared threshold mode for the credit profile. Determines how shared buffer usage is limited.
     *
     * @type sai_cbfc_credit_profile_threshold_mode_t
     * @flags CREATE_ONLY
     * @default SAI_CBFC_CREDIT_PROFILE_THRESHOLD_MODE_NONE
     */
    SAI_CBFC_CREDIT_PROFILE_ATTR_THRESHOLD_MODE,

    /**
     * @brief Dynamic threshold for the shared usage.
     * The threshold is set to the 2^n of available credit of the pool.
     * Valid only if THRESHOLD_MODE=DYNAMIC.
     *
     * @type sai_int8_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_CBFC_CREDIT_PROFILE_ATTR_THRESHOLD_MODE == SAI_CBFC_CREDIT_PROFILE_THRESHOLD_MODE_DYNAMIC
     */
    SAI_CBFC_CREDIT_PROFILE_ATTR_SHARED_DYNAMIC_TH,

    /**
     * @brief Static threshold for shared usage in credits.
     * Zero means no limit. Valid only if THRESHOLD_MODE=STATIC.
     *
     * @type sai_uint64_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_CBFC_CREDIT_PROFILE_ATTR_THRESHOLD_MODE == SAI_CBFC_CREDIT_PROFILE_THRESHOLD_MODE_STATIC
     */
    SAI_CBFC_CREDIT_PROFILE_ATTR_SHARED_STATIC_TH,

    /**
     * @brief End of attributes
     */
    SAI_CBFC_CREDIT_PROFILE_ATTR_END,

    /** Custom range base value */
    SAI_CBFC_CREDIT_PROFILE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_CBFC_CREDIT_PROFILE_ATTR_CUSTOM_RANGE_END

} sai_cbfc_credit_profile_attr_t;

/**
 * @brief Create CBFC Credit Profile
 *
 * @param[out] cbfc_credit_profile_id CBFC Credit Profile id
 * @param[in] switch_id The Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_cbfc_credit_profile_fn)(
        _Out_ sai_object_id_t *cbfc_credit_profile_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove CBFC Credit Profile
 *
 * @param[in] cbfc_credit_profile_id CBFC Credit Profile id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_cbfc_credit_profile_fn)(
        _In_ sai_object_id_t cbfc_credit_profile_id);

/**
 * @brief Set CBFC Credit Profile Attribute
 *
 * @param[in] cbfc_credit_profile_id CBFC Credit Profile id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_cbfc_credit_profile_attribute_fn)(
        _In_ sai_object_id_t cbfc_credit_profile_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get CBFC Credit Profile attribute
 *
 * @param[in] cbfc_credit_profile_id CBFC Credit Profile id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_cbfc_credit_profile_attribute_fn)(
        _In_ sai_object_id_t cbfc_credit_profile_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Virtual Channel methods table retrieved with sai_api_query()
 */
typedef struct _sai_virtual_channel_api_t
{
    sai_create_virtual_channel_fn               create_virtual_channel;
    sai_remove_virtual_channel_fn               remove_virtual_channel;
    sai_set_virtual_channel_attribute_fn        set_virtual_channel_attribute;
    sai_get_virtual_channel_attribute_fn        get_virtual_channel_attribute;
    sai_get_virtual_channel_stats_fn            get_virtual_channel_stats;
    sai_get_virtual_channel_stats_ext_fn        get_virtual_channel_stats_ext;
    sai_clear_virtual_channel_stats_fn          clear_virtual_channel_stats;
    sai_create_cbfc_credit_pool_fn              create_cbfc_credit_pool;
    sai_remove_cbfc_credit_pool_fn              remove_cbfc_credit_pool;
    sai_set_cbfc_credit_pool_attribute_fn       set_cbfc_credit_pool_attribute;
    sai_get_cbfc_credit_pool_attribute_fn       get_cbfc_credit_pool_attribute;
    sai_create_cbfc_credit_profile_fn           create_cbfc_credit_profile;
    sai_remove_cbfc_credit_profile_fn           remove_cbfc_credit_profile;
    sai_set_cbfc_credit_profile_attribute_fn    set_cbfc_credit_profile_attribute;
    sai_get_cbfc_credit_profile_attribute_fn    get_cbfc_credit_profile_attribute;
} sai_virtual_channel_api_t;

/**
 * @}
 */
#endif /* __SAIVIRTUALCHANNEL_H_ */