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
 * @file    saiexperimentaldashtunnel.h
 *
 * @brief   This module defines SAI extensions for DASH tunnel
 *
 * @warning This module is a SAI experimental module
 */

#if !defined (__SAIEXPERIMENTALDASHTUNNEL_H_)
#define __SAIEXPERIMENTALDASHTUNNEL_H_

#include <saitypesextensions.h>

/**
 * @defgroup SAIEXPERIMENTALDASHTUNNEL SAI - Experimental: DASH tunnel specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for DASH tunnel
 */
typedef enum _sai_dash_tunnel_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DASH_TUNNEL_ATTR_START,

    /**
     * @brief Action parameter dip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_DASH_TUNNEL_ATTR_DIP = SAI_DASH_TUNNEL_ATTR_START,

    /**
     * @brief Action parameter DASH encapsulation
     *
     * @type sai_dash_encapsulation_t
     * @flags CREATE_ONLY
     * @default SAI_DASH_ENCAPSULATION_VXLAN
     */
    SAI_DASH_TUNNEL_ATTR_DASH_ENCAPSULATION,

    /**
     * @brief Action parameter tunnel key
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_DASH_TUNNEL_ATTR_TUNNEL_KEY,

    /**
     * @brief Action parameter max member size
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 1
     */
    SAI_DASH_TUNNEL_ATTR_MAX_MEMBER_SIZE,

    /**
     * @brief Action parameter sip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_DASH_TUNNEL_ATTR_SIP,

    /**
     * @brief End of attributes
     */
    SAI_DASH_TUNNEL_ATTR_END,

    /** Custom range base value */
    SAI_DASH_TUNNEL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DASH_TUNNEL_ATTR_CUSTOM_RANGE_END,

} sai_dash_tunnel_attr_t;

/**
 * @brief Attribute ID for DASH tunnel member
 */
typedef enum _sai_dash_tunnel_member_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DASH_TUNNEL_MEMBER_ATTR_START,

    /**
     * @brief Action parameter DASH tunnel id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_DASH_TUNNEL
     */
    SAI_DASH_TUNNEL_MEMBER_ATTR_DASH_TUNNEL_ID = SAI_DASH_TUNNEL_MEMBER_ATTR_START,

    /**
     * @brief Action parameter DASH tunnel next hop id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_TUNNEL_NEXT_HOP
     */
    SAI_DASH_TUNNEL_MEMBER_ATTR_DASH_TUNNEL_NEXT_HOP_ID,

    /**
     * @brief End of attributes
     */
    SAI_DASH_TUNNEL_MEMBER_ATTR_END,

    /** Custom range base value */
    SAI_DASH_TUNNEL_MEMBER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DASH_TUNNEL_MEMBER_ATTR_CUSTOM_RANGE_END,

} sai_dash_tunnel_member_attr_t;

/**
 * @brief Attribute ID for DASH tunnel next hop
 */
typedef enum _sai_dash_tunnel_next_hop_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DASH_TUNNEL_NEXT_HOP_ATTR_START,

    /**
     * @brief Action parameter dip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_DASH_TUNNEL_NEXT_HOP_ATTR_DIP = SAI_DASH_TUNNEL_NEXT_HOP_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_DASH_TUNNEL_NEXT_HOP_ATTR_END,

    /** Custom range base value */
    SAI_DASH_TUNNEL_NEXT_HOP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DASH_TUNNEL_NEXT_HOP_ATTR_CUSTOM_RANGE_END,

} sai_dash_tunnel_next_hop_attr_t;

/**
 * @brief Create DASH tunnel
 *
 * @param[out] dash_tunnel_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_dash_tunnel_fn)(
        _Out_ sai_object_id_t *dash_tunnel_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove DASH tunnel
 *
 * @param[in] dash_tunnel_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_dash_tunnel_fn)(
        _In_ sai_object_id_t dash_tunnel_id);

/**
 * @brief Set attribute for DASH tunnel
 *
 * @param[in] dash_tunnel_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_dash_tunnel_attribute_fn)(
        _In_ sai_object_id_t dash_tunnel_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for DASH tunnel
 *
 * @param[in] dash_tunnel_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_dash_tunnel_attribute_fn)(
        _In_ sai_object_id_t dash_tunnel_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create DASH tunnel member
 *
 * @param[out] dash_tunnel_member_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_dash_tunnel_member_fn)(
        _Out_ sai_object_id_t *dash_tunnel_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove DASH tunnel member
 *
 * @param[in] dash_tunnel_member_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_dash_tunnel_member_fn)(
        _In_ sai_object_id_t dash_tunnel_member_id);

/**
 * @brief Set attribute for DASH tunnel member
 *
 * @param[in] dash_tunnel_member_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_dash_tunnel_member_attribute_fn)(
        _In_ sai_object_id_t dash_tunnel_member_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for DASH tunnel member
 *
 * @param[in] dash_tunnel_member_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_dash_tunnel_member_attribute_fn)(
        _In_ sai_object_id_t dash_tunnel_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create DASH tunnel next hop
 *
 * @param[out] dash_tunnel_next_hop_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_dash_tunnel_next_hop_fn)(
        _Out_ sai_object_id_t *dash_tunnel_next_hop_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove DASH tunnel next hop
 *
 * @param[in] dash_tunnel_next_hop_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_dash_tunnel_next_hop_fn)(
        _In_ sai_object_id_t dash_tunnel_next_hop_id);

/**
 * @brief Set attribute for DASH tunnel next hop
 *
 * @param[in] dash_tunnel_next_hop_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_dash_tunnel_next_hop_attribute_fn)(
        _In_ sai_object_id_t dash_tunnel_next_hop_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for DASH tunnel next hop
 *
 * @param[in] dash_tunnel_next_hop_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_dash_tunnel_next_hop_attribute_fn)(
        _In_ sai_object_id_t dash_tunnel_next_hop_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_dash_tunnel_api_t
{
    sai_create_dash_tunnel_fn                    create_dash_tunnel;
    sai_remove_dash_tunnel_fn                    remove_dash_tunnel;
    sai_set_dash_tunnel_attribute_fn             set_dash_tunnel_attribute;
    sai_get_dash_tunnel_attribute_fn             get_dash_tunnel_attribute;
    sai_bulk_object_create_fn                    create_dash_tunnels;
    sai_bulk_object_remove_fn                    remove_dash_tunnels;

    sai_create_dash_tunnel_member_fn             create_dash_tunnel_member;
    sai_remove_dash_tunnel_member_fn             remove_dash_tunnel_member;
    sai_set_dash_tunnel_member_attribute_fn      set_dash_tunnel_member_attribute;
    sai_get_dash_tunnel_member_attribute_fn      get_dash_tunnel_member_attribute;
    sai_bulk_object_create_fn                    create_dash_tunnel_members;
    sai_bulk_object_remove_fn                    remove_dash_tunnel_members;

    sai_create_dash_tunnel_next_hop_fn           create_dash_tunnel_next_hop;
    sai_remove_dash_tunnel_next_hop_fn           remove_dash_tunnel_next_hop;
    sai_set_dash_tunnel_next_hop_attribute_fn    set_dash_tunnel_next_hop_attribute;
    sai_get_dash_tunnel_next_hop_attribute_fn    get_dash_tunnel_next_hop_attribute;
    sai_bulk_object_create_fn                    create_dash_tunnel_next_hops;
    sai_bulk_object_remove_fn                    remove_dash_tunnel_next_hops;

} sai_dash_tunnel_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHTUNNEL_H_ */
