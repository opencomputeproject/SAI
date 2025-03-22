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
 * @file    saiexperimentaldashoutboundportmap.h
 *
 * @brief   This module defines SAI extensions for DASH outbound port map
 *
 * @warning This module is a SAI experimental module
 */

#if !defined (__SAIEXPERIMENTALDASHOUTBOUNDPORTMAP_H_)
#define __SAIEXPERIMENTALDASHOUTBOUNDPORTMAP_H_

#include <saitypesextensions.h>

/**
 * @defgroup SAIEXPERIMENTALDASHOUTBOUNDPORTMAP SAI - Experimental: DASH outbound port map specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_ACTION
 */
typedef enum _sai_outbound_port_map_port_range_entry_action_t
{
    SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ACTION_SKIP_MAPPING,

    SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ACTION_MAP_TO_PRIVATE_LINK_SERVICE,

} sai_outbound_port_map_port_range_entry_action_t;

/**
 * @brief Attribute ID for outbound port map
 */
typedef enum _sai_outbound_port_map_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_OUTBOUND_PORT_MAP_ATTR_START,

    /**
     * @brief Attach a counter. When it is empty, then packet hits won't be counted.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_OUTBOUND_PORT_MAP_ATTR_COUNTER_ID = SAI_OUTBOUND_PORT_MAP_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_OUTBOUND_PORT_MAP_ATTR_END,

    /** Custom range base value */
    SAI_OUTBOUND_PORT_MAP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_OUTBOUND_PORT_MAP_ATTR_CUSTOM_RANGE_END,

} sai_outbound_port_map_attr_t;

/**
 * @brief Entry for outbound_port_map_port_range_entry
 */
typedef struct _sai_outbound_port_map_port_range_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key outbound_port_map_id
     *
     * @objects SAI_OBJECT_TYPE_OUTBOUND_PORT_MAP
     */
    sai_object_id_t outbound_port_map_id;

    /**
     * @brief Range matched key dst_port_range
     */
    sai_u32_range_t dst_port_range;

} sai_outbound_port_map_port_range_entry_t;

/**
 * @brief Attribute ID for outbound port map port range entry
 */
typedef enum _sai_outbound_port_map_port_range_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_outbound_port_map_port_range_entry_action_t
     * @flags CREATE_AND_SET
     * @default SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ACTION_SKIP_MAPPING
     */
    SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_ACTION = SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_START,

    /**
     * @brief Action parameter back end IP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_ACTION == SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ACTION_MAP_TO_PRIVATE_LINK_SERVICE
     */
    SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_BACKEND_IP,

    /**
     * @brief Action parameter match port base
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     * @validonly SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_ACTION == SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ACTION_MAP_TO_PRIVATE_LINK_SERVICE
     */
    SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_MATCH_PORT_BASE,

    /**
     * @brief Action parameter back end port base
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     * @validonly SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_ACTION == SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ACTION_MAP_TO_PRIVATE_LINK_SERVICE
     */
    SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_BACKEND_PORT_BASE,

    /**
     * @brief Attach a counter. When it is empty, then packet hits won't be counted.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_COUNTER_ID,

    /**
     * @brief End of attributes
     */
    SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_outbound_port_map_port_range_entry_attr_t;

/**
 * @brief Create outbound port map
 *
 * @param[out] outbound_port_map_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_outbound_port_map_fn)(
        _Out_ sai_object_id_t *outbound_port_map_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove outbound port map
 *
 * @param[in] outbound_port_map_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_outbound_port_map_fn)(
        _In_ sai_object_id_t outbound_port_map_id);

/**
 * @brief Set attribute for outbound port map
 *
 * @param[in] outbound_port_map_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_outbound_port_map_attribute_fn)(
        _In_ sai_object_id_t outbound_port_map_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for outbound port map
 *
 * @param[in] outbound_port_map_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_outbound_port_map_attribute_fn)(
        _In_ sai_object_id_t outbound_port_map_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create outbound port map port range entry
 *
 * @param[in] outbound_port_map_port_range_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_outbound_port_map_port_range_entry_fn)(
        _In_ const sai_outbound_port_map_port_range_entry_t *outbound_port_map_port_range_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove outbound port map port range entry
 *
 * @param[in] outbound_port_map_port_range_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_outbound_port_map_port_range_entry_fn)(
        _In_ const sai_outbound_port_map_port_range_entry_t *outbound_port_map_port_range_entry);

/**
 * @brief Set attribute for outbound port map port range entry
 *
 * @param[in] outbound_port_map_port_range_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_outbound_port_map_port_range_entry_attribute_fn)(
        _In_ const sai_outbound_port_map_port_range_entry_t *outbound_port_map_port_range_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for outbound port map port range entry
 *
 * @param[in] outbound_port_map_port_range_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_outbound_port_map_port_range_entry_attribute_fn)(
        _In_ const sai_outbound_port_map_port_range_entry_t *outbound_port_map_port_range_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create outbound port map port range entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] outbound_port_map_port_range_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_outbound_port_map_port_range_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_port_map_port_range_entry_t *outbound_port_map_port_range_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove outbound port map port range entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] outbound_port_map_port_range_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_outbound_port_map_port_range_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_port_map_port_range_entry_t *outbound_port_map_port_range_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

typedef struct _sai_dash_outbound_port_map_api_t
{
    sai_create_outbound_port_map_fn                            create_outbound_port_map;
    sai_remove_outbound_port_map_fn                            remove_outbound_port_map;
    sai_set_outbound_port_map_attribute_fn                     set_outbound_port_map_attribute;
    sai_get_outbound_port_map_attribute_fn                     get_outbound_port_map_attribute;
    sai_bulk_object_create_fn                                  create_outbound_port_maps;
    sai_bulk_object_remove_fn                                  remove_outbound_port_maps;

    sai_create_outbound_port_map_port_range_entry_fn           create_outbound_port_map_port_range_entry;
    sai_remove_outbound_port_map_port_range_entry_fn           remove_outbound_port_map_port_range_entry;
    sai_set_outbound_port_map_port_range_entry_attribute_fn    set_outbound_port_map_port_range_entry_attribute;
    sai_get_outbound_port_map_port_range_entry_attribute_fn    get_outbound_port_map_port_range_entry_attribute;
    sai_bulk_create_outbound_port_map_port_range_entry_fn      create_outbound_port_map_port_range_entries;
    sai_bulk_remove_outbound_port_map_port_range_entry_fn      remove_outbound_port_map_port_range_entries;

} sai_dash_outbound_port_map_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHOUTBOUNDPORTMAP_H_ */
