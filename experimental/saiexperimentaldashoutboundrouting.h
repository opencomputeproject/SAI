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
 * @file    saiexperimentaldashoutboundrouting.h
 *
 * @brief   This module defines SAI P4 extension  interface
 */

#if !defined (__SAIEXPERIMENTALDASHOUTBOUNDROUTING_H_)
#define __SAIEXPERIMENTALDASHOUTBOUNDROUTING_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALDASH_OUTBOUND_ROUTING SAI - Extension specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION
 */
typedef enum _sai_outbound_routing_entry_action_t
{
    SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_VNET,

    SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_VNET_DIRECT,

    SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_DIRECT,

    SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_SERVICE_TUNNEL,

    SAI_OUTBOUND_ROUTING_ENTRY_ACTION_DROP,

} sai_outbound_routing_entry_action_t;

/**
 * @brief Entry for outbound_routing_entry
 */
typedef struct _sai_outbound_routing_entry_t
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
     * @brief LPM matched key destination
     */
    sai_ip_prefix_t destination;

} sai_outbound_routing_entry_t;

/**
 * @brief Attribute ID for dash_outbound_routing_outbound_routing_entry
 */
typedef enum _sai_outbound_routing_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_outbound_routing_entry_action_t
     * @flags CREATE_AND_SET
     * @default SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_VNET
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION = SAI_OUTBOUND_ROUTING_ENTRY_ATTR_START,

    /**
     * @brief Action route_vnet, route_vnet_direct parameter DST_VNET_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VNET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION == SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_VNET or SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION == SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_VNET_DIRECT
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_DST_VNET_ID,

    /**
     * @brief Action route_vnet_direct parameter OVERLAY_IP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION == SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_VNET_DIRECT
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_OVERLAY_IP,

    /**
     * @brief Action route_service_tunnel parameter OVERLAY_DIP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION == SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_SERVICE_TUNNEL
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_OVERLAY_DIP,

    /**
     * @brief Action route_service_tunnel parameter OVERLAY_DIP_MASK
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION == SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_SERVICE_TUNNEL
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_OVERLAY_DIP_MASK,

    /**
     * @brief Action route_service_tunnel parameter OVERLAY_SIP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION == SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_SERVICE_TUNNEL
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_OVERLAY_SIP,

    /**
     * @brief Action route_service_tunnel parameter OVERLAY_SIP_MASK
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION == SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_SERVICE_TUNNEL
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_OVERLAY_SIP_MASK,

    /**
     * @brief Action route_service_tunnel parameter UNDERLAY_DIP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION == SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_SERVICE_TUNNEL
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_UNDERLAY_DIP,

    /**
     * @brief Action route_service_tunnel parameter UNDERLAY_SIP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION == SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_SERVICE_TUNNEL
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_UNDERLAY_SIP,

    /**
     * @brief Action route_service_tunnel parameter ENCAP_TYPE
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     * @validonly SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION == SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_SERVICE_TUNNEL
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ENCAP_TYPE,

    /**
     * @brief Action route_service_tunnel parameter TUNNEL_ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OUTBOUND_ROUTING_ENTRY_ATTR_ACTION == SAI_OUTBOUND_ROUTING_ENTRY_ACTION_ROUTE_SERVICE_TUNNEL
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_TUNNEL_ID,

    /**
     * @brief Attach a counter
     *
     * When it is empty, then packet hits won't be counted
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_COUNTER_ID,

    /**
     * @brief End of attributes
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_outbound_routing_entry_attr_t;

/**
 * @brief Create dash_outbound_routing_outbound_routing_entry
 *
 * @param[in] outbound_routing_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_outbound_routing_entry_fn)(
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_outbound_routing_outbound_routing_entry
 *
 * @param[in] outbound_routing_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_outbound_routing_entry_fn)(
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry);

/**
 * @brief Set attribute for dash_outbound_routing_outbound_routing_entry
 *
 * @param[in] outbound_routing_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_outbound_routing_entry_attribute_fn)(
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_outbound_routing_outbound_routing_entry
 *
 * @param[in] outbound_routing_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_outbound_routing_entry_attribute_fn)(
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create dash_outbound_routing_outbound_routing_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] outbound_routing_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_outbound_routing_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove dash_outbound_routing_outbound_routing_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] outbound_routing_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_outbound_routing_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

typedef struct _sai_dash_outbound_routing_api_t
{
    sai_create_outbound_routing_entry_fn           create_outbound_routing_entry;
    sai_remove_outbound_routing_entry_fn           remove_outbound_routing_entry;
    sai_set_outbound_routing_entry_attribute_fn    set_outbound_routing_entry_attribute;
    sai_get_outbound_routing_entry_attribute_fn    get_outbound_routing_entry_attribute;
    sai_bulk_create_outbound_routing_entry_fn      create_outbound_routing_entries;
    sai_bulk_remove_outbound_routing_entry_fn      remove_outbound_routing_entries;

} sai_dash_outbound_routing_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHOUTBOUNDROUTING_H_ */
