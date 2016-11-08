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
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc
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

    /** MPLS(NHLFE) next hop */
    SAI_NEXT_HOP_TYPE_MPLS,

    /** Tunnel next hop */
    SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP

} sai_next_hop_type_t;

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
     */
    SAI_NEXT_HOP_ATTR_TYPE = SAI_NEXT_HOP_ATTR_START,

    /**
     * @brief Next hop entry ipv4 address
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_IP
     */
    SAI_NEXT_HOP_ATTR_IP,

    /**
     * @brief Next hop entry router interface id
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,

    /**
     * @brief Next hop entry tunnel-id
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_TUNNEL
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_NEXT_HOP_ATTR_TYPE == SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP
     */
    SAI_NEXT_HOP_ATTR_TUNNEL_ID,

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
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
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
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_next_hop_fn)(
        _In_ sai_object_id_t next_hop_id);

/**
 * @brief Set Next Hop attribute
 *
 * @param[in] next_hop_id Next hop id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
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
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
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
