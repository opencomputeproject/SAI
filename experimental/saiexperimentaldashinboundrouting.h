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
 * @file    saiexperimentaldashinboundrouting.h
 *
 * @brief   This module defines SAI P4 extension  interface
 */

#if !defined (__SAIEXPERIMENTALDASHINBOUNDROUTING_H_)
#define __SAIEXPERIMENTALDASHINBOUNDROUTING_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALDASH_INBOUND_ROUTING SAI - Extension specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_INBOUND_ROUTING_ENTRY_ATTR_ACTION
 */
typedef enum _sai_inbound_routing_entry_action_t
{
    SAI_INBOUND_ROUTING_ENTRY_ACTION_VXLAN_DECAP,

    SAI_INBOUND_ROUTING_ENTRY_ACTION_VXLAN_DECAP_PA_VALIDATE,

} sai_inbound_routing_entry_action_t;

/**
 * @brief Entry for inbound_routing_entry
 */
typedef struct _sai_inbound_routing_entry_t
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
     * @brief Exact matched key VNI
     */
    sai_uint32_t vni;

    /**
     * @brief Ternary matched key sip
     */
    sai_ip_address_t sip;

    /**
     * @brief Ternary key sip mask
     */
    sai_ip_address_t sip_mask;

    /**
     * @brief Rule priority in table
     */
    sai_uint32_t priority;

} sai_inbound_routing_entry_t;

/**
 * @brief Attribute ID for dash_inbound_routing_inbound_routing_entry
 */
typedef enum _sai_inbound_routing_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_INBOUND_ROUTING_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_inbound_routing_entry_action_t
     * @flags CREATE_AND_SET
     * @default SAI_INBOUND_ROUTING_ENTRY_ACTION_VXLAN_DECAP
     */
    SAI_INBOUND_ROUTING_ENTRY_ATTR_ACTION = SAI_INBOUND_ROUTING_ENTRY_ATTR_START,

    /**
     * @brief Action vxlan_decap_pa_validate parameter SRC_VNET_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VNET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_INBOUND_ROUTING_ENTRY_ATTR_ACTION == SAI_INBOUND_ROUTING_ENTRY_ACTION_VXLAN_DECAP_PA_VALIDATE
     */
    SAI_INBOUND_ROUTING_ENTRY_ATTR_SRC_VNET_ID,

    /**
     * @brief IP address family for resource accounting
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_INBOUND_ROUTING_ENTRY_ATTR_IP_ADDR_FAMILY,

    /**
     * @brief End of attributes
     */
    SAI_INBOUND_ROUTING_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_INBOUND_ROUTING_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_INBOUND_ROUTING_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_inbound_routing_entry_attr_t;

/**
 * @brief Create dash_inbound_routing_inbound_routing_entry
 *
 * @param[in] inbound_routing_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_inbound_routing_entry_fn)(
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_inbound_routing_inbound_routing_entry
 *
 * @param[in] inbound_routing_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_inbound_routing_entry_fn)(
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry);

/**
 * @brief Set attribute for dash_inbound_routing_inbound_routing_entry
 *
 * @param[in] inbound_routing_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_inbound_routing_entry_attribute_fn)(
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_inbound_routing_inbound_routing_entry
 *
 * @param[in] inbound_routing_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_inbound_routing_entry_attribute_fn)(
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create dash_inbound_routing_inbound_routing_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] inbound_routing_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_inbound_routing_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove dash_inbound_routing_inbound_routing_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] inbound_routing_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_inbound_routing_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

typedef struct _sai_dash_inbound_routing_api_t
{
    sai_create_inbound_routing_entry_fn           create_inbound_routing_entry;
    sai_remove_inbound_routing_entry_fn           remove_inbound_routing_entry;
    sai_set_inbound_routing_entry_attribute_fn    set_inbound_routing_entry_attribute;
    sai_get_inbound_routing_entry_attribute_fn    get_inbound_routing_entry_attribute;
    sai_bulk_create_inbound_routing_entry_fn      create_inbound_routing_entries;
    sai_bulk_remove_inbound_routing_entry_fn      remove_inbound_routing_entries;

} sai_dash_inbound_routing_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHINBOUNDROUTING_H_ */
