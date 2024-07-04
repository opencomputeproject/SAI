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
 * @file    saiexperimentalbmtor.h
 *
 * @brief   This module defines SAI extensions needed for the BMTOR application
 *
 * @warning This module is a SAI experimental module.
 */

#if !defined (__SAIEXPERIMENTALBMTOR_H_)
#define __SAIEXPERIMENTALBMTOR_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALBMTOR SAI - Experimental: BMTOR specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ATTR_ACTION
 */
typedef enum _sai_table_bitmap_classification_entry_action_t
{
    SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ACTION_SET_METADATA,

    SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ACTION_NOACTION,

} sai_table_bitmap_classification_entry_action_t;

/**
 * @brief Attribute data for #SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_ACTION
 */
typedef enum _sai_table_bitmap_router_entry_action_t
{
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ACTION_TO_NEXTHOP,

    SAI_TABLE_BITMAP_ROUTER_ENTRY_ACTION_TO_LOCAL,

    SAI_TABLE_BITMAP_ROUTER_ENTRY_ACTION_TO_CPU,

    SAI_TABLE_BITMAP_ROUTER_ENTRY_ACTION_DROP,

    SAI_TABLE_BITMAP_ROUTER_ENTRY_ACTION_NOACTION,

} sai_table_bitmap_router_entry_action_t;

/**
 * @brief Attribute data for #SAI_TABLE_META_TUNNEL_ENTRY_ATTR_ACTION
 */
typedef enum _sai_table_meta_tunnel_entry_action_t
{
    SAI_TABLE_META_TUNNEL_ENTRY_ACTION_TUNNEL_ENCAP,

    SAI_TABLE_META_TUNNEL_ENTRY_ACTION_NOACTION,

} sai_table_meta_tunnel_entry_action_t;

/**
 * @brief Attribute ID for table_bitmap_classification
 */
typedef enum _sai_table_bitmap_classification_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_table_bitmap_classification_entry_action_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ATTR_ACTION = SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ATTR_START,

    /**
     * @brief Exact Matched key router_interface
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     */
    SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ATTR_ROUTER_INTERFACE_KEY,

    /**
     * @brief Is default entry
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ATTR_IS_DEFAULT,

    /**
     * @brief Action set_metadata parameter in_rif_metadata
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ATTR_ACTION == SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ACTION_SET_METADATA
     */
    SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ATTR_IN_RIF_METADATA,

    /**
     * @brief End of attributes
     */
    SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_table_bitmap_classification_entry_attr_t;

/**
 * @brief Attribute ID for table_bitmap_router
 */
typedef enum _sai_table_bitmap_router_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_table_bitmap_router_entry_action_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_ACTION = SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_START,

    /**
     * @brief Rule priority in table
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_PRIORITY,

    /**
     * @brief Ternary Matched key in_rif_metadata (key)
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_IN_RIF_METADATA_KEY,

    /**
     * @brief Ternary Matched mask in_rif_metadata (mask)
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_IN_RIF_METADATA_MASK,

    /**
     * @brief LPM Matched key dst_ip
     *
     * @type sai_ip_prefix_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_DST_IP_KEY,

    /**
     * @brief Action to_nexthop parameter tunnel_index
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan false
     * @condition SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_ACTION == SAI_TABLE_BITMAP_ROUTER_ENTRY_ACTION_TO_NEXTHOP
     */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_TUNNEL_INDEX,

    /**
     * @brief Action to_nexthop parameter next_hop
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_NEXT_HOP
     * @condition SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_ACTION == SAI_TABLE_BITMAP_ROUTER_ENTRY_ACTION_TO_NEXTHOP
     */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_NEXT_HOP,

    /**
     * @brief Action to_local parameter router_interface
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @condition SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_ACTION == SAI_TABLE_BITMAP_ROUTER_ENTRY_ACTION_TO_LOCAL
     */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_ROUTER_INTERFACE,

    /**
     * @brief Action to_cpu parameter trap_id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_HOSTIF_TRAP
     * @condition SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_ACTION == SAI_TABLE_BITMAP_ROUTER_ENTRY_ACTION_TO_CPU
     */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_TRAP_ID,

    /**
     * @brief End of attributes
     */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TABLE_BITMAP_ROUTER_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_table_bitmap_router_entry_attr_t;

/**
 * @brief Attribute ID for table_meta_tunnel
 */
typedef enum _sai_table_meta_tunnel_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_TABLE_META_TUNNEL_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_table_meta_tunnel_entry_action_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TABLE_META_TUNNEL_ENTRY_ATTR_ACTION = SAI_TABLE_META_TUNNEL_ENTRY_ATTR_START,

    /**
     * @brief Exact Matched key metadata
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan false
     */
    SAI_TABLE_META_TUNNEL_ENTRY_ATTR_METADATA_KEY,

    /**
     * @brief Is default entry
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_TABLE_META_TUNNEL_ENTRY_ATTR_IS_DEFAULT,

    /**
     * @brief Action tunnel_encap parameter tunnel_id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL
     * @condition SAI_TABLE_META_TUNNEL_ENTRY_ATTR_ACTION == SAI_TABLE_META_TUNNEL_ENTRY_ACTION_TUNNEL_ENCAP
     */
    SAI_TABLE_META_TUNNEL_ENTRY_ATTR_TUNNEL_ID,

    /**
     * @brief Action tunnel_encap parameter underlay_dip
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TABLE_META_TUNNEL_ENTRY_ATTR_ACTION == SAI_TABLE_META_TUNNEL_ENTRY_ACTION_TUNNEL_ENCAP
     */
    SAI_TABLE_META_TUNNEL_ENTRY_ATTR_UNDERLAY_DIP,

    /**
     * @brief End of attributes
     */
    SAI_TABLE_META_TUNNEL_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_TABLE_META_TUNNEL_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TABLE_META_TUNNEL_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_table_meta_tunnel_entry_attr_t;

/**
 * @brief Counter IDs in sai_get_table_bitmap_classification_entry_stats() call
 */
typedef enum _sai_table_bitmap_classification_entry_stat_t
{
    SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_STAT_HIT_PACKETS,
    SAI_TABLE_BITMAP_CLASSIFICATION_ENTRY_STAT_HIT_OCTETS,
} sai_table_bitmap_classification_entry_stat_t;

/**
 * @brief Counter IDs in sai_get_table_bitmap_router_entry_stats() call
 */
typedef enum _sai_table_bitmap_router_entry_stat_t
{
    SAI_TABLE_BITMAP_ROUTER_ENTRY_STAT_HIT_PACKETS,
    SAI_TABLE_BITMAP_ROUTER_ENTRY_STAT_HIT_OCTETS,
} sai_table_bitmap_router_entry_stat_t;

/**
 * @brief Counter IDs in sai_get_table_meta_tunnel_entry_stats() call
 */
typedef enum _sai_table_meta_tunnel_entry_stat_t
{
    SAI_TABLE_META_TUNNEL_ENTRY_STAT_HIT_PACKETS,
    SAI_TABLE_META_TUNNEL_ENTRY_STAT_HIT_OCTETS,
} sai_table_meta_tunnel_entry_stat_t;

/**
 * @brief Create table_bitmap_classification_entry
 *
 * @param[out] table_bitmap_classification_entry_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_table_bitmap_classification_entry_fn)(
        _Out_ sai_object_id_t *table_bitmap_classification_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove table_bitmap_classification_entry
 *
 * @param[in] table_bitmap_classification_entry_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_table_bitmap_classification_entry_fn)(
        _In_ sai_object_id_t table_bitmap_classification_entry_id);

/**
 * @brief Set attribute for table_bitmap_classification_entry
 *
 * @param[in] table_bitmap_classification_entry_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_table_bitmap_classification_entry_attribute_fn)(
        _In_ sai_object_id_t table_bitmap_classification_entry_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for table_bitmap_classification_entry
 *
 * @param[in] table_bitmap_classification_entry_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_table_bitmap_classification_entry_attribute_fn)(
        _In_ sai_object_id_t table_bitmap_classification_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get table_bitmap_classification statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] table_bitmap_classification_entry_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_table_bitmap_classification_entry_stats_fn)(
        _In_ sai_object_id_t table_bitmap_classification_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get table_bitmap_classification statistics counters extended.
 *
 * @param[in] table_bitmap_classification_entry_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_table_bitmap_classification_entry_stats_ext_fn)(
        _In_ sai_object_id_t table_bitmap_classification_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear statistics counters.
 *
 * @param[in] table_bitmap_classification_entry_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_clear_table_bitmap_classification_entry_stats_fn)(
        _In_ sai_object_id_t table_bitmap_classification_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Create table_bitmap_router_entry
 *
 * @param[out] table_bitmap_router_entry_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_table_bitmap_router_entry_fn)(
        _Out_ sai_object_id_t *table_bitmap_router_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove table_bitmap_router_entry
 *
 * @param[in] table_bitmap_router_entry_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_table_bitmap_router_entry_fn)(
        _In_ sai_object_id_t table_bitmap_router_entry_id);

/**
 * @brief Set attribute for table_bitmap_router_entry
 *
 * @param[in] table_bitmap_router_entry_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_table_bitmap_router_entry_attribute_fn)(
        _In_ sai_object_id_t table_bitmap_router_entry_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for table_bitmap_router_entry
 *
 * @param[in] table_bitmap_router_entry_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_table_bitmap_router_entry_attribute_fn)(
        _In_ sai_object_id_t table_bitmap_router_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get table_bitmap_router statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] table_bitmap_router_entry_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_table_bitmap_router_entry_stats_fn)(
        _In_ sai_object_id_t table_bitmap_router_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get table_bitmap_router statistics counters extended.
 *
 * @param[in] table_bitmap_router_entry_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_table_bitmap_router_entry_stats_ext_fn)(
        _In_ sai_object_id_t table_bitmap_router_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear statistics counters.
 *
 * @param[in] table_bitmap_router_entry_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_clear_table_bitmap_router_entry_stats_fn)(
        _In_ sai_object_id_t table_bitmap_router_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Create table_meta_tunnel_entry
 *
 * @param[out] table_meta_tunnel_entry_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_table_meta_tunnel_entry_fn)(
        _Out_ sai_object_id_t *table_meta_tunnel_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove table_meta_tunnel_entry
 *
 * @param[in] table_meta_tunnel_entry_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_table_meta_tunnel_entry_fn)(
        _In_ sai_object_id_t table_meta_tunnel_entry_id);

/**
 * @brief Set attribute for table_meta_tunnel_entry
 *
 * @param[in] table_meta_tunnel_entry_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_table_meta_tunnel_entry_attribute_fn)(
        _In_ sai_object_id_t table_meta_tunnel_entry_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for table_meta_tunnel_entry
 *
 * @param[in] table_meta_tunnel_entry_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_table_meta_tunnel_entry_attribute_fn)(
        _In_ sai_object_id_t table_meta_tunnel_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get table_meta_tunnel statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] table_meta_tunnel_entry_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_table_meta_tunnel_entry_stats_fn)(
        _In_ sai_object_id_t table_meta_tunnel_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get table_meta_tunnel statistics counters extended.
 *
 * @param[in] table_meta_tunnel_entry_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_table_meta_tunnel_entry_stats_ext_fn)(
        _In_ sai_object_id_t table_meta_tunnel_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear statistics counters.
 *
 * @param[in] table_meta_tunnel_entry_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_clear_table_meta_tunnel_entry_stats_fn)(
        _In_ sai_object_id_t table_meta_tunnel_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

typedef struct _sai_bmtor_api_t
{
    sai_create_table_bitmap_classification_entry_fn           create_table_bitmap_classification_entry;
    sai_remove_table_bitmap_classification_entry_fn           remove_table_bitmap_classification_entry;
    sai_set_table_bitmap_classification_entry_attribute_fn    set_table_bitmap_classification_entry_attribute;
    sai_get_table_bitmap_classification_entry_attribute_fn    get_table_bitmap_classification_entry_attribute;
    sai_get_table_bitmap_classification_entry_stats_fn        get_table_bitmap_classification_entry_stats;
    sai_get_table_bitmap_classification_entry_stats_ext_fn    get_table_bitmap_classification_entry_stats_ext;
    sai_clear_table_bitmap_classification_entry_stats_fn      clear_table_bitmap_classification_entry_stats;
    sai_create_table_bitmap_router_entry_fn                   create_table_bitmap_router_entry;
    sai_remove_table_bitmap_router_entry_fn                   remove_table_bitmap_router_entry;
    sai_set_table_bitmap_router_entry_attribute_fn            set_table_bitmap_router_entry_attribute;
    sai_get_table_bitmap_router_entry_attribute_fn            get_table_bitmap_router_entry_attribute;
    sai_get_table_bitmap_router_entry_stats_fn                get_table_bitmap_router_entry_stats;
    sai_get_table_bitmap_router_entry_stats_ext_fn            get_table_bitmap_router_entry_stats_ext;
    sai_clear_table_bitmap_router_entry_stats_fn              clear_table_bitmap_router_entry_stats;
    sai_create_table_meta_tunnel_entry_fn                     create_table_meta_tunnel_entry;
    sai_remove_table_meta_tunnel_entry_fn                     remove_table_meta_tunnel_entry;
    sai_set_table_meta_tunnel_entry_attribute_fn              set_table_meta_tunnel_entry_attribute;
    sai_get_table_meta_tunnel_entry_attribute_fn              get_table_meta_tunnel_entry_attribute;
    sai_get_table_meta_tunnel_entry_stats_fn                  get_table_meta_tunnel_entry_stats;
    sai_get_table_meta_tunnel_entry_stats_ext_fn              get_table_meta_tunnel_entry_stats_ext;
    sai_clear_table_meta_tunnel_entry_stats_fn                clear_table_meta_tunnel_entry_stats;
} sai_bmtor_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALBMTOR_H_ */
