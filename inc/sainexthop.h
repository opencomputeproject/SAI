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
 * @file    sainexthop.h
 *
 * @brief   This module defines SAI Next Hop interface
 */

#if !defined (__SAINEXTHOP_H_)
#define __SAINEXTHOP_H_

#include <saitypes.h>

/**
 * @defgroup SAINEXTHOP SAI - Next hop specific API definitions.
 *
 * @{
 */

/**
 * @brief Next hop type
 */
typedef enum _sai_next_hop_type_t
{
    /** IP next hop */
    SAI_NEXT_HOP_TYPE_IP,

    /** MPLS(outsegment) next hop */
    SAI_NEXT_HOP_TYPE_MPLS,

    /** Tunnel next hop */
    SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP,

    /** IPv6 Segment Route SID List */
    SAI_NEXT_HOP_TYPE_SEGMENTROUTE_SIDLIST,

    /** IPv6 Segment Route Endpoint Function */
    SAI_NEXT_HOP_TYPE_SEGMENTROUTE_ENDPOINT,

} sai_next_hop_type_t;

/**
 * @brief Enum defining Endpoint Action types
 */
typedef enum _sai_next_hop_endpoint_type_t
{
    /** Basic Endpoint */
    SAI_NEXT_HOP_ENDPOINT_TYPE_E,

    /** End.X Endpoint with Layer-3 Cross-connect */
    SAI_NEXT_HOP_ENDPOINT_TYPE_X,

    /** End.T Endpoint with specific IPv6 Table */
    SAI_NEXT_HOP_ENDPOINT_TYPE_T,

    /** Endpoint with decapsulation and Layer 2 Cross-connect */
    SAI_NEXT_HOP_ENDPOINT_TYPE_DX2,

    /** Endpoint with decapsulation and IPv6 Cross-connect */
    SAI_NEXT_HOP_ENDPOINT_TYPE_DX6,

    /** Endpoint with decapsulation and IPv4 Cross-connect */
    SAI_NEXT_HOP_ENDPOINT_TYPE_DX4,

    /** Endpoint with decapsulation and specific IPv6 */
    SAI_NEXT_HOP_ENDPOINT_TYPE_DT6,

    /** Endpoint with decapsulation and specific IPv6 */
    SAI_NEXT_HOP_ENDPOINT_TYPE_DT4,

    /** Custom range base value */
    SAI_NEXT_HOP_ENDPOINT_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_next_hop_endpoint_type_t;

/**
 * @brief Enum defining Endpoint Segment Pop types for End, End.X and End.T
 */
typedef enum _sai_next_hop_endpoint_pop_type_t
{
    /** Penultimate segment pop */
    SAI_NEXT_HOP_ENDPOINT_POP_TYPE_PSP,

    /** Ultimate Segment pop */
    SAI_NEXT_HOP_ENDPOINT_POP_TYPE_USP,

} sai_next_hop_endpoint_pop_type_t;

/**
 * @brief Attribute id for next hop
 */
typedef enum _sai_next_hop_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_NEXT_HOP_ATTR_START,

    /**
     * @brief Next hop entry type
     *
     * @type sai_next_hop_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isresourcetype true
     */
    SAI_NEXT_HOP_ATTR_TYPE = SAI_NEXT_HOP_ATTR_START,

    /**
     * @brief Next hop entry IPv4 address
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_IP or SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_MPLS or SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP
     */
    SAI_NEXT_HOP_ATTR_IP,

    /**
     * @brief Next hop entry router interface id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @condition SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_IP or SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_MPLS
     */
    SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,

    /**
     * @brief Next hop entry tunnel-id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL
     * @condition SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP
     */
    SAI_NEXT_HOP_ATTR_TUNNEL_ID,

    /**
     * @brief Next hop entry VNI (override tunnel mapper)
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP
     */
    SAI_NEXT_HOP_ATTR_TUNNEL_VNI,

    /**
     * @brief Inner destination MAC address
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default attrvalue SAI_SWITCH_ATTR_VXLAN_DEFAULT_ROUTER_MAC
     * @validonly SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP
     */
    SAI_NEXT_HOP_ATTR_TUNNEL_MAC,

    /**
     * @brief Next hop entry Segment Route SID List
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_SEGMENTROUTE_SIDLIST
     * @condition SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_SEGMENTROUTE_SIDLIST
     */
    SAI_NEXT_HOP_ATTR_SEGMENTROUTE_SIDLIST_ID,

    /**
     * @brief Next hop entry Segment Route Endpoint Function
     *
     * @type sai_next_hop_endpoint_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_SEGMENTROUTE_ENDPOINT
     */
    SAI_NEXT_HOP_ATTR_SEGMENTROUTE_ENDPOINT_TYPE,

    /**
     * @brief Next hop entry Segment Route Endpoint Pop Option
     *
     * @type sai_next_hop_endpoint_pop_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_SEGMENTROUTE_ENDPOINT
     */
    SAI_NEXT_HOP_ATTR_SEGMENTROUTE_ENDPOINT_POP_TYPE,

    /**
     * @brief Push label
     *
     * @type sai_u32_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_MPLS
     */
    SAI_NEXT_HOP_ATTR_LABELSTACK,

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
    SAI_NEXT_HOP_ATTR_COUNTER_ID,

    /**
     * @brief To enable/disable Decrement TTL
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_NEXT_HOP_ATTR_DECREMENT_TTL,

    /**
     * @brief MPLS Outsegment type
     *
     * @type sai_outseg_type_t
     * @flags CREATE_AND_SET
     * @default SAI_OUTSEG_TYPE_SWAP
     * @validonly SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_MPLS
     */
    SAI_NEXT_HOP_ATTR_OUTSEG_TYPE,

    /**
     * @brief MPLS Outsegment TTL mode
     *
     * @type sai_outseg_ttl_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_OUTSEG_TTL_MODE_UNIFORM
     * @validonly SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_MPLS and SAI_NEXT_HOP_ATTR_OUTSEG_TYPE == SAI_OUTSEG_TYPE_PUSH
     */
    SAI_NEXT_HOP_ATTR_OUTSEG_TTL_MODE,

    /**
     * @brief MPLS Outsegment TTL value for pipe mode
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 255
     * @validonly SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_MPLS and SAI_NEXT_HOP_ATTR_OUTSEG_TYPE == SAI_OUTSEG_TYPE_PUSH and SAI_NEXT_HOP_ATTR_OUTSEG_TTL_MODE == SAI_OUTSEG_TTL_MODE_PIPE
     */
    SAI_NEXT_HOP_ATTR_OUTSEG_TTL_VALUE,

    /**
     * @brief MPLS Outsegment MPLS EXP mode
     *
     * @type sai_outseg_exp_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_OUTSEG_EXP_MODE_UNIFORM
     * @validonly SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_MPLS and SAI_NEXT_HOP_ATTR_OUTSEG_TYPE == SAI_OUTSEG_TYPE_PUSH
     */
    SAI_NEXT_HOP_ATTR_OUTSEG_EXP_MODE,

    /**
     * @brief MPLS Outsegment EXP value for pipe mode
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_MPLS and SAI_NEXT_HOP_ATTR_OUTSEG_TYPE == SAI_OUTSEG_TYPE_PUSH and SAI_NEXT_HOP_ATTR_OUTSEG_TTL_MODE == SAI_OUTSEG_TTL_MODE_PIPE
     */
    SAI_NEXT_HOP_ATTR_OUTSEG_EXP_VALUE,

    /**
     * @brief TC AND COLOR -> MPLS EXP MAP for Uniform Mode
     *
     * If present overrides SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_MPLS_EXP_MAP and SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_MPLS_EXP_MAP
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_MPLS and SAI_NEXT_HOP_ATTR_OUTSEG_TYPE == SAI_OUTSEG_TYPE_PUSH and SAI_NEXT_HOP_ATTR_OUTSEG_TTL_MODE == SAI_OUTSEG_TTL_MODE_UNIFORM
     */
    SAI_NEXT_HOP_ATTR_QOS_TC_AND_COLOR_TO_MPLS_EXP_MAP,

    /**
     * @brief End of attributes
     */
    SAI_NEXT_HOP_ATTR_END,

    /** Custom range base value */
    SAI_NEXT_HOP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_NEXT_HOP_ATTR_CUSTOM_RANGE_END

} sai_next_hop_attr_t;

/**
 * @brief Create next hop
 *
 * Note: IP address expected in Network Byte Order.
 *
 * @param[out] next_hop_id Next hop id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_next_hop_fn)(
        _Out_ sai_object_id_t *next_hop_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove next hop
 *
 * @param[in] next_hop_id Next hop id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_next_hop_fn)(
        _In_ sai_object_id_t next_hop_id);

/**
 * @brief Set Next Hop attribute
 *
 * @param[in] next_hop_id Next hop id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_next_hop_attribute_fn)(
        _In_ sai_object_id_t next_hop_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Next Hop attribute
 *
 * @param[in] next_hop_id Next hop id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_next_hop_attribute_fn)(
        _In_ sai_object_id_t next_hop_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Next Hop methods table retrieved with sai_api_query()
 */
typedef struct _sai_next_hop_api_t
{
    sai_create_next_hop_fn        create_next_hop;
    sai_remove_next_hop_fn        remove_next_hop;
    sai_set_next_hop_attribute_fn set_next_hop_attribute;
    sai_get_next_hop_attribute_fn get_next_hop_attribute;

} sai_next_hop_api_t;

/**
 * @}
 */
#endif /** __SAINEXTHOP_H_ */
