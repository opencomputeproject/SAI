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
 * @file    saiexperimentaldashoutboundcatopa.h
 *
 * @brief   This module defines SAI extensions for DASH outbound CA to PA
 *
 * @warning This module is a SAI experimental module
 */

#if !defined (__SAIEXPERIMENTALDASHOUTBOUNDCATOPA_H_)
#define __SAIEXPERIMENTALDASHOUTBOUNDCATOPA_H_

#include <saitypesextensions.h>

/**
 * @defgroup SAIEXPERIMENTALDASHOUTBOUNDCATOPA SAI - Experimental: DASH outbound CA to PA specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION
 */
typedef enum _sai_outbound_ca_to_pa_entry_action_t
{
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_TUNNEL_MAPPING,

    SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_PRIVATE_LINK_MAPPING,

} sai_outbound_ca_to_pa_entry_action_t;

/**
 * @brief Entry for outbound_ca_to_pa_entry
 */
typedef struct _sai_outbound_ca_to_pa_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key dst_vnet_id
     *
     * @objects SAI_OBJECT_TYPE_VNET
     */
    sai_object_id_t dst_vnet_id;

    /**
     * @brief Exact matched key dip
     */
    sai_ip_address_t dip;

} sai_outbound_ca_to_pa_entry_t;

/**
 * @brief Attribute ID for outbound CA to PA entry
 */
typedef enum _sai_outbound_ca_to_pa_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_outbound_ca_to_pa_entry_action_t
     * @flags CREATE_AND_SET
     * @default SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_TUNNEL_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION = SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_START,

    /**
     * @brief Action parameter underlay dip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_TUNNEL_MAPPING or SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_PRIVATE_LINK_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_UNDERLAY_DIP,

    /**
     * @brief Action parameter overlay destination MAC
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default vendor
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_TUNNEL_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_OVERLAY_DMAC,

    /**
     * @brief Action parameter use dst VNET VNI
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_TUNNEL_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_USE_DST_VNET_VNI,

    /**
     * @brief Action parameter meter class or
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_TUNNEL_MAPPING or SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_PRIVATE_LINK_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_METER_CLASS_OR,

    /**
     * @brief Action parameter DASH tunnel id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_TUNNEL
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_TUNNEL_MAPPING or SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_PRIVATE_LINK_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_DASH_TUNNEL_ID,

    /**
     * @brief Action parameter flow re-simulation requested
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_TUNNEL_MAPPING or SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_PRIVATE_LINK_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_FLOW_RESIMULATION_REQUESTED,

    /**
     * @brief Action parameter routing actions disabled in flow re-simulation
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_TUNNEL_MAPPING or SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_PRIVATE_LINK_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ROUTING_ACTIONS_DISABLED_IN_FLOW_RESIMULATION,

    /**
     * @brief Action parameter overlay sip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_PRIVATE_LINK_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_OVERLAY_SIP,

    /**
     * @brief Action parameter overlay sip mask
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_PRIVATE_LINK_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_OVERLAY_SIP_MASK,

    /**
     * @brief Action parameter overlay dip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_PRIVATE_LINK_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_OVERLAY_DIP,

    /**
     * @brief Action parameter overlay dip mask
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_PRIVATE_LINK_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_OVERLAY_DIP_MASK,

    /**
     * @brief Action parameter DASH encapsulation
     *
     * @type sai_dash_encapsulation_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_ENCAPSULATION_INVALID
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_PRIVATE_LINK_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_DASH_ENCAPSULATION,

    /**
     * @brief Action parameter tunnel key
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_ACTION == SAI_OUTBOUND_CA_TO_PA_ENTRY_ACTION_SET_PRIVATE_LINK_MAPPING
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_TUNNEL_KEY,

    /**
     * @brief Attach a counter. When it is empty, then packet hits won't be counted.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_COUNTER_ID,

    /**
     * @brief IP address family for resource accounting
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_IP_ADDR_FAMILY,

    /**
     * @brief End of attributes
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_outbound_ca_to_pa_entry_attr_t;

/**
 * @brief Create outbound CA to PA entry
 *
 * @param[in] outbound_ca_to_pa_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_outbound_ca_to_pa_entry_fn)(
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove outbound CA to PA entry
 *
 * @param[in] outbound_ca_to_pa_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_outbound_ca_to_pa_entry_fn)(
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry);

/**
 * @brief Set attribute for outbound CA to PA entry
 *
 * @param[in] outbound_ca_to_pa_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_outbound_ca_to_pa_entry_attribute_fn)(
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for outbound CA to PA entry
 *
 * @param[in] outbound_ca_to_pa_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_outbound_ca_to_pa_entry_attribute_fn)(
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create outbound CA to PA entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] outbound_ca_to_pa_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_outbound_ca_to_pa_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove outbound CA to PA entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] outbound_ca_to_pa_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_outbound_ca_to_pa_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

typedef struct _sai_dash_outbound_ca_to_pa_api_t
{
    sai_create_outbound_ca_to_pa_entry_fn           create_outbound_ca_to_pa_entry;
    sai_remove_outbound_ca_to_pa_entry_fn           remove_outbound_ca_to_pa_entry;
    sai_set_outbound_ca_to_pa_entry_attribute_fn    set_outbound_ca_to_pa_entry_attribute;
    sai_get_outbound_ca_to_pa_entry_attribute_fn    get_outbound_ca_to_pa_entry_attribute;
    sai_bulk_create_outbound_ca_to_pa_entry_fn      create_outbound_ca_to_pa_entries;
    sai_bulk_remove_outbound_ca_to_pa_entry_fn      remove_outbound_ca_to_pa_entries;

} sai_dash_outbound_ca_to_pa_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHOUTBOUNDCATOPA_H_ */
