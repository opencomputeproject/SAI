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
 * @file    sairoute.h
 *
 * @brief   This module defines SAI Route Entry interface
 */

#if !defined (__SAIFLOW_H_)
#define __SAIFLOW_H_

#include <saitypes.h>

/**
 * @defgroup SAIROUTE SAI - Route specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute Id for SAI route object
 */
typedef enum _sai_route_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_ROUTE_ENTRY_ATTR_START,

    /* READ-WRITE */

    /**
     * @brief Packet action
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_ROUTE_ENTRY_ATTR_PACKET_ACTION = SAI_ROUTE_ENTRY_ATTR_START,

    /**
     * @brief Generate User Defined Trap ID for trap/log actions
     *
     * When it is SAI_NULL_OBJECT_ID, then packet will not be trapped.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HOSTIF_USER_DEFINED_TRAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ROUTE_ENTRY_ATTR_USER_TRAP_ID,

    /**
     * @brief Next hop or next hop group id for the packet, or a router interface
     * in case of directly reachable route, or the CPU port in case of IP2ME route
     *
     * The next hop id is only effective when the packet action is one of the
     * following: FORWARD, COPY, LOG, TRANSIT.
     *
     * The next hop id can be a generic next hop object, such as next hop, next
     * hop group. Directly reachable routes are the IP subnets that are
     * directly attached to the router. For such routes, fill the router
     * interface id to which the subnet is attached. IP2ME route adds a local
     * router IP address. For such routes, fill the CPU port
     * (#SAI_SWITCH_ATTR_CPU_PORT).
     * When pointing to a next hop group which is empty, the effective routing
     * action will be DROP.
     *
     * When it is SAI_NULL_OBJECT_ID, then packet will be dropped.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NEXT_HOP, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, SAI_OBJECT_TYPE_ROUTER_INTERFACE, SAI_OBJECT_TYPE_PORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ROUTE_ENTRY_ATTR_NEXT_HOP_ID,

    /**
     * @brief User based Meta Data
     *
     * Value Range #SAI_SWITCH_ATTR_ROUTE_DST_USER_META_DATA_RANGE
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ROUTE_ENTRY_ATTR_META_DATA,

    /** READ-ONLY */

    /**
     * @brief Route entry IP address family
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_ROUTE_ENTRY_ATTR_IP_ADDR_FAMILY,

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
    SAI_ROUTE_ENTRY_ATTR_COUNTER_ID,

    /**
     * @brief Route Prefix Aggregation ID
     *
     * Prefix aggregation ID is used for SRV6 VPN cases. When multiple prefixes
     * use the same set of remote SRV6 Endpoint(s) but with different VPN SID
     * values, prefix aggregation id allows SAI to derive different VPN SID while
     * keeping using single destination (NH or Next hop group)
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ROUTE_ENTRY_ATTR_PREFIX_AGG_ID,

    /**
     * @brief End of attributes
     */
    SAI_ROUTE_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_ROUTE_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_ROUTE_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_route_entry_attr_t;

/**
 * @brief Unicast route entry
 */
typedef struct _sai_route_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Virtual Router ID
     *
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     */
    sai_object_id_t vr_id;

    /**
     * @brief IP Prefix Destination
     */
    sai_ip_prefix_t destination;

} sai_route_entry_t;

/**
 * @brief Create Route
 *
 * Note: IP prefix/mask expected in Network Byte Order.
 *
 * @param[in] route_entry Route entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_route_entry_fn)(
        _In_ const sai_route_entry_t *route_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Route
 *
 * Note: IP prefix/mask expected in Network Byte Order.
 *
 * @param[in] route_entry Route entry
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_route_entry_fn)(
        _In_ const sai_route_entry_t *route_entry);

/**
 * @brief Set route attribute value
 *
 * @param[in] route_entry Route entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_route_entry_attribute_fn)(
        _In_ const sai_route_entry_t *route_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get route attribute value
 *
 * @param[in] route_entry Route entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_route_entry_attribute_fn)(
        _In_ const sai_route_entry_t *route_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create route entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] route_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_route_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_route_entry_t *route_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove route entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] route_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_route_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_route_entry_t *route_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk set attribute on route entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] route_entry List of objects to set attribute
 * @param[in] attr_list List of attributes to set on objects, one attribute per object
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_set_route_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_route_entry_t *route_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk get attribute on route entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] route_entry List of objects to set attribute
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get
 * @param[inout] attr_list List of attributes to set on objects, one attribute per object
 * @param[in] mode Bulk operation error handling mode
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_route_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_route_entry_t *route_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Router entry methods table retrieved with sai_api_query()
 */
typedef struct _sai_route_api_t
{
    sai_create_route_entry_fn                   create_route_entry;
    sai_remove_route_entry_fn                   remove_route_entry;
    sai_set_route_entry_attribute_fn            set_route_entry_attribute;
    sai_get_route_entry_attribute_fn            get_route_entry_attribute;

    sai_bulk_create_route_entry_fn              create_route_entries;
    sai_bulk_remove_route_entry_fn              remove_route_entries;
    sai_bulk_set_route_entry_attribute_fn       set_route_entries_attribute;
    sai_bulk_get_route_entry_attribute_fn       get_route_entries_attribute;

} sai_route_api_t;

/**
 * @}
 */
#endif /** __SAIFLOW_H_ */