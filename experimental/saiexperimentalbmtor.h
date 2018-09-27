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
 * @brief   This module defines SAI || P4 extension interface
 */

#if !defined (__SAIEXPERIMENTALBMTOR_H_)
#define __SAIEXPERIMENTALBMTOR_H_

#include <saitypes.h>

sai_status_t sai_ext_api_initialize(sai_object_list_t in_port_if_list);
sai_status_t sai_ext_api_uninitialize(sai_object_list_t in_port_if_list);

/**
 * @defgroup SAIEXPERIMENTALBMTOR SAI - Extension specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_TABLE_PEERING_ENTRY_ATTR_ACTION
 */
typedef enum _sai_table_peering_entry_action_t
{
    SAI_TABLE_PEERING_ENTRY_ACTION_SET_VNET_BITMAP,

    SAI_TABLE_PEERING_ENTRY_ACTION_NOACTION,

} sai_table_peering_entry_action_t;

/**
 * @brief Attribute data for #SAI_TABLE_VHOST_ENTRY_ATTR_ACTION
 */
typedef enum _sai_table_vhost_entry_action_t
{
    SAI_TABLE_VHOST_ENTRY_ACTION_TO_TUNNEL,

    SAI_TABLE_VHOST_ENTRY_ACTION_TO_ROUTER,

    SAI_TABLE_VHOST_ENTRY_ACTION_TO_PORT,

    SAI_TABLE_VHOST_ENTRY_ACTION_NOACTION,

} sai_table_vhost_entry_action_t;

/**
 * @brief Attribute ID for table_peering
 */
typedef enum _sai_table_peering_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_TABLE_PEERING_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_table_peering_entry_action_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TABLE_PEERING_ENTRY_ATTR_ACTION = SAI_TABLE_PEERING_ENTRY_ATTR_START,

    /**
     * @brief Matched key src_port
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_TABLE_PEERING_ENTRY_ATTR_SRC_PORT,

    /**
     * @brief Is default entry
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_TABLE_PEERING_ENTRY_ATTR_IS_DEFAULT,

    /**
     * @brief Action set_vnet_bitmap parameter meta_reg
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan false
     * @condition SAI_TABLE_PEERING_ENTRY_ATTR_ACTION == SAI_TABLE_PEERING_ENTRY_ACTION_SET_VNET_BITMAP
     */
    SAI_TABLE_PEERING_ENTRY_ATTR_META_REG,

    /**
     * @brief End of attributes
     */
    SAI_TABLE_PEERING_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_TABLE_PEERING_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TABLE_PEERING_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_table_peering_entry_attr_t;

/**
 * @brief Attribute ID for table_vhost
 */
typedef enum _sai_table_vhost_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_table_vhost_entry_action_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_ACTION = SAI_TABLE_VHOST_ENTRY_ATTR_START,

    /**
     * @brief Rule priority in table
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_PRIORITY,

    /**
     * @brief Matched key meta_reg (key)
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan false
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_META_REG_KEY,

    /**
     * @brief Matched key meta_reg (mask)
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan false
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_META_REG_MASK,

    /**
     * @brief Matched key dst_ip
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_DST_IP,

    /**
     * @brief Is default entry
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_IS_DEFAULT,

    /**
     * @brief Action to_tunnel parameter bridge_id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE
     * @condition SAI_TABLE_VHOST_ENTRY_ATTR_ACTION == SAI_TABLE_VHOST_ENTRY_ACTION_TO_TUNNEL
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_BRIDGE_ID,

    /**
     * @brief Action to_tunnel parameter tunnel_id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL
     * @condition SAI_TABLE_VHOST_ENTRY_ATTR_ACTION == SAI_TABLE_VHOST_ENTRY_ACTION_TO_TUNNEL
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_TUNNEL_ID,

    /**
     * @brief Action to_tunnel parameter underlay_dip
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TABLE_VHOST_ENTRY_ATTR_ACTION == SAI_TABLE_VHOST_ENTRY_ACTION_TO_TUNNEL
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_UNDERLAY_DIP,

    /**
     * @brief Action to_router parameter vr_id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @condition SAI_TABLE_VHOST_ENTRY_ATTR_ACTION == SAI_TABLE_VHOST_ENTRY_ACTION_TO_ROUTER
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_VR_ID,

    /**
     * @brief Action to_port parameter port_id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     * @condition SAI_TABLE_VHOST_ENTRY_ATTR_ACTION == SAI_TABLE_VHOST_ENTRY_ACTION_TO_PORT
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_PORT_ID,

    /**
     * @brief End of attributes
     */
    SAI_TABLE_VHOST_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_TABLE_VHOST_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TABLE_VHOST_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_table_vhost_entry_attr_t;

/**
 * @brief Counter IDs in sai_get_table_vhost_entry_stats() call
 */
typedef enum _sai_table_vhost_entry_stat_t
{
    SAI_TABLE_VHOST_ENTRY_STAT_HIT_PACKETS,
    SAI_TABLE_VHOST_ENTRY_STAT_HIT_OCTETS,
} sai_table_vhost_entry_stat_t;

/**
 * @brief Create table_peering_entry
 *
 * @param[out] table_peering_entry_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_table_peering_entry_fn)(
        _Out_ sai_object_id_t *table_peering_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove table_peering_entry
 *
 * @param[in] table_peering_entry_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_table_peering_entry_fn)(
        _In_ sai_object_id_t table_peering_entry_id);

/**
 * @brief Set attribute for table_peering_entry
 *
 * @param[in] table_peering_entry_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_table_peering_entry_attribute_fn)(
        _In_ sai_object_id_t table_peering_entry_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for table_peering_entry
 *
 * @param[in] table_peering_entry_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_table_peering_entry_attribute_fn)(
        _In_ sai_object_id_t table_peering_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create table_vhost_entry
 *
 * @param[out] table_vhost_entry_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_table_vhost_entry_fn)(
        _Out_ sai_object_id_t *table_vhost_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove table_vhost_entry
 *
 * @param[in] table_vhost_entry_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_table_vhost_entry_fn)(
        _In_ sai_object_id_t table_vhost_entry_id);

/**
 * @brief Set attribute for table_vhost_entry
 *
 * @param[in] table_vhost_entry_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_table_vhost_entry_attribute_fn)(
        _In_ sai_object_id_t table_vhost_entry_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for table_vhost_entry
 *
 * @param[in] table_vhost_entry_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_table_vhost_entry_attribute_fn)(
        _In_ sai_object_id_t table_vhost_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get statistics counters.
 *
 * @param[in] table_vhost_entry_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_table_vhost_entry_stats_fn)(
        _In_ sai_object_id_t table_vhost_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_table_vhost_entry_stat_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Clear statistics counters.
 *
 * @param[in] entry_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_clear_table_vhost_entry_stats_fn)(
        _In_ sai_object_id_t entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_table_vhost_entry_stat_t *counter_ids);

typedef struct _sai_bmtor_api_t
{
    sai_create_table_peering_entry_fn        create_table_peering_entry;
    sai_remove_table_peering_entry_fn        remove_table_peering_entry;
    sai_set_table_peering_entry_attribute_fn set_table_peering_entry_attribute;
    sai_get_table_peering_entry_attribute_fn get_table_peering_entry_attribute;
    sai_create_table_vhost_entry_fn          create_table_vhost_entry;
    sai_remove_table_vhost_entry_fn          remove_table_vhost_entry;
    sai_set_table_vhost_entry_attribute_fn   set_table_vhost_entry_attribute;
    sai_get_table_vhost_entry_attribute_fn   get_table_vhost_entry_attribute;
    sai_get_table_vhost_entry_stats_fn       get_table_vhost_entry_stats;
    sai_clear_table_vhost_entry_stats_fn     clear_table_vhost_entry_stats;
} sai_bmtor_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALBMTOR_H_ */
