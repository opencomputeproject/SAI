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
 * @file    saineighbor.h
 *
 * @brief   This module defines SAI Neighbor interface
 *
 * @par Abstract
 *
 *    This module defines SAI neighbor table API
 *    The table contains both IPv4 and IPv6 neighbors
 */

#if !defined (__SAINEIGHBOR_H_)
#define __SAINEIGHBOR_H_

#include <saitypes.h>

/**
 * @defgroup SAINEIGHBOR SAI - Neighbor specific API definitions.
 *
 * @{
 */

/**
 * @brief Attribute Id for SAI neighbor object
 */
typedef enum _sai_neighbor_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_NEIGHBOR_ENTRY_ATTR_START,

    /**
     * @brief Destination MAC address for the neighbor
     * Valid only when SAI_NEIGHBOR_ENTRY_ATTR_IS_LOCAL == true
     *
     * @type sai_mac_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_NEIGHBOR_ENTRY_ATTR_DST_MAC_ADDRESS = SAI_NEIGHBOR_ENTRY_ATTR_START,

    /**
     * @brief L3 forwarding action for this neighbor
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_NEIGHBOR_ENTRY_ATTR_PACKET_ACTION,

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
    SAI_NEIGHBOR_ENTRY_ATTR_USER_TRAP_ID,

    /**
     * @brief Neighbor not to be programmed as a host route entry in ASIC and
     * to be only used to setup next-hop purpose.
     *
     * Typical use-case is to set this true for neighbor with IPv6 link-local
     * addresses.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE,

    /**
     * @brief User based Meta Data
     *
     * Value Range #SAI_SWITCH_ATTR_NEIGHBOR_DST_USER_META_DATA_RANGE
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_NEIGHBOR_ENTRY_ATTR_META_DATA,

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
    SAI_NEIGHBOR_ENTRY_ATTR_COUNTER_ID,

    /**
     * @brief Encapsulation Index
     *
     * Defines the neighbor's encapsulation index
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default internal
     */
    SAI_NEIGHBOR_ENTRY_ATTR_ENCAP_INDEX,

    /**
     * @brief Encapsulation index is imposed. This is deprecated
     *
     * This attribute is deprecated
     * This is a flag which states that the encap index was imposed. On create and set
     * the SAI_NEIGHBOR_ENTRY_ATTR_ENCAP_INDEX must be present.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @deprecated true
     */
    SAI_NEIGHBOR_ENTRY_ATTR_ENCAP_IMPOSE_INDEX,

    /**
     * @brief Is Neighbor Local
     *
     * This is a flag which states that the neighbor being created is local. This can
     * be used to sanity check the impose index flag. For example, in some implementations
     * imposing an encap index when the RIF is port-based and the neighbor is local
     * may not be allowed.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_NEIGHBOR_ENTRY_ATTR_IS_LOCAL,

    /** READ-ONLY */

    /**
     * @brief Neighbor entry IP address family
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_NEIGHBOR_ENTRY_ATTR_IP_ADDR_FAMILY,

    /**
     * @brief End of attributes
     */
    SAI_NEIGHBOR_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_NEIGHBOR_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_NEIGHBOR_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_neighbor_entry_attr_t;

/**
 * @brief Neighbor entry
 */
typedef struct _sai_neighbor_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Router interface ID
     *
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     */
    sai_object_id_t rif_id;

    /**
     * @brief IP address
     */
    sai_ip_address_t ip_address;

} sai_neighbor_entry_t;

/**
 * @brief Create neighbor entry
 *
 * Note: IP address expected in Network Byte Order.
 *
 * @param[in] neighbor_entry Neighbor entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_neighbor_entry_fn)(
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove neighbor entry
 *
 * Note: IP address expected in Network Byte Order.
 *
 * @param[in] neighbor_entry Neighbor entry
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_neighbor_entry_fn)(
        _In_ const sai_neighbor_entry_t *neighbor_entry);

/**
 * @brief Set neighbor attribute value
 *
 * @param[in] neighbor_entry Neighbor entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_neighbor_entry_attribute_fn)(
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get neighbor attribute value
 *
 * @param[in] neighbor_entry Neighbor entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_neighbor_entry_attribute_fn)(
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Remove all neighbor entries
 *
 * @param[in] switch_id Switch id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_all_neighbor_entries_fn)(
        _In_ sai_object_id_t switch_id);

/**
 * @brief Bulk create Neighbor entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] neighbor_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_neighbor_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove Neighbor entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] neighbor_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_neighbor_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk set attribute on Neighbor entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] neighbor_entry List of objects to set attribute
 * @param[in] attr_list List of attributes to set on objects, one attribute per object
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are set or
 * #SAI_STATUS_FAILURE when any of the objects fails to set. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_set_neighbor_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk get attribute on Neighbor entry
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] neighbor_entry List of objects to get attribute
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get
 * @param[inout] attr_list List of attributes to get on objects, one attribute per object
 * @param[in] mode Bulk operation error handling mode
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are get or
 * #SAI_STATUS_FAILURE when any of the objects fails to get. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_neighbor_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);
/**
 * @brief Neighbor table methods, retrieved via sai_api_query()
 */
typedef struct _sai_neighbor_api_t
{
    sai_create_neighbor_entry_fn                create_neighbor_entry;
    sai_remove_neighbor_entry_fn                remove_neighbor_entry;
    sai_set_neighbor_entry_attribute_fn         set_neighbor_entry_attribute;
    sai_get_neighbor_entry_attribute_fn         get_neighbor_entry_attribute;
    sai_remove_all_neighbor_entries_fn          remove_all_neighbor_entries;

    sai_bulk_create_neighbor_entry_fn           create_neighbor_entries;
    sai_bulk_remove_neighbor_entry_fn           remove_neighbor_entries;
    sai_bulk_set_neighbor_entry_attribute_fn    set_neighbor_entries_attribute;
    sai_bulk_get_neighbor_entry_attribute_fn    get_neighbor_entries_attribute;

} sai_neighbor_api_t;

/**
 * @}
 */
#endif /** __SAINEIGHBOR_H_ */
