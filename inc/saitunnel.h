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
 * @file    saitunnel.h
 *
 * @brief   This module defines SAI Tunnel interface
 */

#if !defined (__SAITUNNEL_H_)
#define __SAITUNNEL_H_

#include <saitypes.h>

/**
 * @defgroup SAITUNNEL SAI - Next hop specific API definitions
 *
 * @{
 */

/**
 * @brief Enum defining tunnel map types.
 */
typedef enum _sai_tunnel_map_type_t
{
    /** TUNNEL Map overlay ECN to underlay ECN (only valid for encap) */
    SAI_TUNNEL_MAP_TYPE_OECN_TO_UECN = 0x00000000,

    /** TUNNEL Map underlay ECN and overlay ECN to overlay ECN (only valid for decap) */
    SAI_TUNNEL_MAP_TYPE_UECN_OECN_TO_OECN = 0x00000001,

    /** TUNNEL Map VNI to VLAN ID */
    SAI_TUNNEL_MAP_TYPE_VNI_TO_VLAN_ID = 0x00000002,

    /** TUNNEL Map VLAN ID to VNI */
    SAI_TUNNEL_MAP_TYPE_VLAN_ID_TO_VNI = 0x00000003,

    /** TUNNEL Map VNI to Bridge IF */
    SAI_TUNNEL_MAP_TYPE_VNI_TO_BRIDGE_IF = 0x00000004,

    /** TUNNEL Map Bridge IF to VNI */
    SAI_TUNNEL_MAP_TYPE_BRIDGE_IF_TO_VNI = 0x00000005,

    /** Custom range base value */
    SAI_TUNNEL_MAP_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_tunnel_map_type_t;

/**
 * @brief Defines tunnel map attributes
 */
typedef enum _sai_tunnel_map_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_TUNNEL_MAP_ATTR_START = 0x00000000,

    /**
     * @brief Tunnel Map type
     *
     * @type sai_tunnel_map_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TUNNEL_MAP_ATTR_TYPE = SAI_TUNNEL_MAP_ATTR_START,

    /**
     * @brief Tunnel mapper
     *
     * @type sai_tunnel_map_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TUNNEL_MAP_ATTR_MAP_TO_VALUE_LIST = 0x00000001,

    /**
     * @brief End of attributes
     */
    SAI_TUNNEL_MAP_ATTR_END,

    /** Custom range base value */
    SAI_TUNNEL_MAP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TUNNEL_MAP_ATTR_CUSTOM_RANGE_END

} sai_tunnel_map_attr_t;

/**
 * @brief Create tunnel Map
 *
 * @param[out] tunnel_map_id Runnel Map Id
 * @param[in] switch_id Switch Id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_create_tunnel_map_fn)(
        _Out_ sai_object_id_t *tunnel_map_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove tunnel Map
 *
 * @param[in] tunnel_map_id Tunnel Map id to be removed
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_remove_tunnel_map_fn) (
        _In_ sai_object_id_t tunnel_map_id);

/**
 * @brief Set attributes for tunnel map
 *
 * @param[in] tunnel_map_id Tunnel Map Id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_set_tunnel_map_attribute_fn)(
        _In_ sai_object_id_t tunnel_map_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attrbutes of tunnel map
 *
 * @param[in] tunnel_map_id Tunnel map id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_get_tunnel_map_attribute_fn)(
        _In_ sai_object_id_t tunnel_map_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Defines tunnel type
 */
typedef enum _sai_tunnel_type_t
{
    SAI_TUNNEL_TYPE_IPINIP,

    SAI_TUNNEL_TYPE_IPINIP_GRE,

    SAI_TUNNEL_TYPE_VXLAN,

    SAI_TUNNEL_TYPE_MPLS,

} sai_tunnel_type_t;

/**
 * @brief Defines tunnel ttl mode
 */
typedef enum _sai_tunnel_ttl_mode_t
{
    /**
     * @brief The uniform model
     *
     * Where the TTL field is preserved end-to-end by copying into the outer
     * header on encapsulation and copying from the outer header on
     * decapsulation.
     */
    SAI_TUNNEL_TTL_MODE_UNIFORM_MODEL,

    /**
     * @brief The pipe model
     *
     * Where the outer header is independent of that in the inner header so
     * it hides the TTL field of the inner header from any interaction
     * with nodes along the tunnel.
     *
     * TTL field is user-defined for outer header on encapsulation. TTL
     * field of inner header remains the same on decapsulation.
     */
    SAI_TUNNEL_TTL_MODE_PIPE_MODEL

} sai_tunnel_ttl_mode_t;

/**
 * @brief Defines tunnel dscp mode
 */
typedef enum _sai_tunnel_dscp_mode_t
{
    /**
     * @brief The uniform model
     *
     * Where the DSCP field is preserved end-to-end by copying into the
     * outer header on encapsulation and copying from the outer header on
     * decapsulation.
     */
    SAI_TUNNEL_DSCP_MODE_UNIFORM_MODEL,

    /**
     * @brief The pipe model
     *
     * Where the outer header is independent of that in the inner header so
     * it hides the DSCP field of the inner header from any interaction
     * with nodes along the tunnel.
     *
     * DSCP field is user-defined for outer header on encapsulation. DSCP
     * field of inner header remains the same on decapsulation. */
    SAI_TUNNEL_DSCP_MODE_PIPE_MODEL

} sai_tunnel_dscp_mode_t;

/**
 * @brief Defines tunnel encap ecn mode
 */
typedef enum _sai_tunnel_encap_ecn_mode_t
{
    /**
     * @brief Normal mode behavior defined in RFC 6040
     * section 4.1 copy from inner
     */
    SAI_TUNNEL_ENCAP_ECN_MODE_STANDARD,

    /**
     * @brief User defined behavior.
     *
     * Need to provide #SAI_TUNNEL_MAP_TYPE_OECN_TO_UECN in #SAI_TUNNEL_ATTR_ENCAP_MAPPERS
     */
    SAI_TUNNEL_ENCAP_ECN_MODE_USER_DEFINED

} sai_tunnel_encap_ecn_mode_t;

/**
 * @brief Defines tunnel decap ecn mode
 */
typedef enum _sai_tunnel_decap_ecn_mode_t
{
    /**
     * @brief Behavior defined in RFC 6040 section 4.2
     */
    SAI_TUNNEL_DECAP_ECN_MODE_STANDARD,

    /**
     * @brief Copy from outer ECN
     */
    SAI_TUNNEL_DECAP_ECN_MODE_COPY_FROM_OUTER,

    /**
     * @brief User defined behavior
     *
     * Need to provide #SAI_TUNNEL_MAP_TYPE_UECN_OECN_TO_OECN in #SAI_TUNNEL_ATTR_DECAP_MAPPERS
     */
    SAI_TUNNEL_DECAP_ECN_MODE_USER_DEFINED

} sai_tunnel_decap_ecn_mode_t;

/**
 * @brief Defines tunnel attributes
 */
typedef enum _sai_tunnel_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_TUNNEL_ATTR_START,

    /**
     * @brief Tunnel type
     *
     * @type sai_tunnel_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TUNNEL_ATTR_TYPE = SAI_TUNNEL_ATTR_START,

    /**
     * @brief tunnel underlay interface
     *
     * Underlay interface is router interface when conditions are met
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_GRE
     */
    SAI_TUNNEL_ATTR_UNDERLAY_INTERFACE,

    /**
     * @brief Tunnel overlay interafce
     *
     * Overlay interface is router interface when conditions are met
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_GRE
     */
    SAI_TUNNEL_ATTR_OVERLAY_INTERFACE,

    /* Tunnel encap attributes */

    /**
     * @brief Tunnel src ip
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TUNNEL_ATTR_ENCAP_SRC_IP,

    /**
     * @brief Tunnel TTL mode (pipe or uniform model)
     *
     * Default would be
     *
     * @type sai_tunnel_ttl_mode_t
     * @flags CREATE_ONLY
     * @default SAI_TUNNEL_TTL_MODE_UNIFORM_MODEL
     */
    SAI_TUNNEL_ATTR_ENCAP_TTL_MODE,

    /**
     * @brief Tunnel TTL value
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_ATTR_ENCAP_TTL_MODE == SAI_TUNNEL_TTL_MODE_PIPE_MODEL
     */
    SAI_TUNNEL_ATTR_ENCAP_TTL_VAL,

    /**
     * @brief Tunnel dscp mode (pipe or uniform model)
     *
     * @type sai_tunnel_dscp_mode_t
     * @flags CREATE_ONLY
     * @default SAI_TUNNEL_DSCP_MODE_UNIFORM_MODEL
     */
    SAI_TUNNEL_ATTR_ENCAP_DSCP_MODE,

    /**
     * @brief tunnel DSCP value (6 bits)
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_ATTR_ENCAP_DSCP_MODE == SAI_TUNNEL_DSCP_MODE_PIPE_MODEL
     */
    SAI_TUNNEL_ATTR_ENCAP_DSCP_VAL,

    /**
     * Tunnel GRE key valid
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     * @condition SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_GRE
     */
    SAI_TUNNEL_ATTR_ENCAP_GRE_KEY_VALID,

    /**
     * @brief Tunnel GRE key
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_ATTR_ENCAP_GRE_KEY_VALID == true
     */
    SAI_TUNNEL_ATTR_ENCAP_GRE_KEY,

    /**
     * @brief Tunnel encap ECN mode
     *
     * @type sai_tunnel_encap_ecn_mode_t
     * @flags CREATE_ONLY
     * @default SAI_TUNNEL_ENCAP_ECN_MODE_STANDARD
     */
    SAI_TUNNEL_ATTR_ENCAP_ECN_MODE,

    /**
     * @brief Tunnel encap mappers
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_TUNNEL_MAP
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_ATTR_ENCAP_ECN_MODE == SAI_TUNNEL_ENCAP_ECN_MODE_USER_DEFINED
     */
    SAI_TUNNEL_ATTR_ENCAP_MAPPERS,

    /** tunnel decap attribute **/
    /* Tunnel decap attributes */

    /**
     * @brief Tunnel decap ECN mode
     *
     * @type sai_tunnel_decap_ecn_mode_t
     * @flags CREATE_ONLY
     * @default SAI_TUNNEL_DECAP_ECN_MODE_STANDARD
     */
    SAI_TUNNEL_ATTR_DECAP_ECN_MODE,

    /**
     * @brief Tunnel decap mappers
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_TUNNEL_MAP
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_ATTR_DECAP_ECN_MODE == SAI_TUNNEL_DECAP_ECN_MODE_USER_DEFINED
     */
    SAI_TUNNEL_ATTR_DECAP_MAPPERS,

    /**
     * @brief Tunnel TTL mode (pipe or uniform model)
     *
     * Default SAI_TUNNEL_TTL_MODE_UNIFORM_MODEL
     *
     * @type sai_tunnel_ttl_mode_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_GRE
     */
    SAI_TUNNEL_ATTR_DECAP_TTL_MODE,

    /**
     * @brief Tunnel dscp mode (pipe or uniform model)
     *
     * Default SAI_TUNNEL_DSCP_MODE_UNIFORM_MODEL
     *
     * @type sai_tunnel_dscp_mode_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_GRE
     */
    SAI_TUNNEL_ATTR_DECAP_DSCP_MODE,

    /**
     * @brief End of attributes
     */
    SAI_TUNNEL_ATTR_END,

    /** Custom range base value */
    SAI_TUNNEL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TUNNEL_ATTR_CUSTOM_RANGE_END

} sai_tunnel_attr_t;

/**
 * @brief Create tunnel
 *
 * @param[out] tunnel_id Tunnel id
 * @param[in] switch_id Switch Id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_tunnel_fn)(
        _Out_ sai_object_id_t *tunnel_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove tunnel
 *
 * @param[in] tunnel_id Tunnel id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_tunnel_fn)(
        _In_ sai_object_id_t tunnel_id);

/**
 * @brief Set tunnel attribute
 *
 * @param[in] tunnel_id Tunnel id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_tunnel_attribute_fn)(
        _In_ sai_object_id_t tunnel_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get tunnel attributes
 *
 * @param[in] tunnel _id Tunnel id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Aarray of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_tunnel_attribute_fn)(
        _In_ sai_object_id_t tunnel_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Defines tunnel termination table entry type
 */
typedef enum _sai_tunnel_term_table_entry_type_t
{
    /** tunnel termination table point to point entry match on dst & src ip & tunnel type **/
    SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P,

    /** tunnel termination table point to multi point entry match on dst ip & tunnel type **/
    SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2MP,

} sai_tunnel_term_table_entry_type_t;

/**
 * @brief Defines tunnel termination table entry attributes
 */
typedef enum _sai_tunnel_term_table_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_START,

    /**
     * @brief Tunnel virtual router id
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_VR_ID = SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_START,

    /**
     * @brief Tunnel entry type
     *
     * @type sai_tunnel_term_table_entry_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE,

    /**
     * @brief Tunnel termination ip address [
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_DST_IP,

    /**
     * @brief Tunnel source ip address
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE == SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_SRC_IP,

    /**
     * @brief Tunnel type
     *
     * @type sai_tunnel_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE == SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TUNNEL_TYPE,

    /**
     * @brief Tunnel id to be use for decap
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_TUNNEL
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_ACTION_TUNNEL_ID,

    /**
     * @brief End of attributes
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** Enc od custom range base */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_tunnel_term_table_entry_attr_t;

/**
 * @brief Create tunnel termination table entry
 *
 * @param[out] tunnel_term_table_entry_id Tunnel termination table entry id
 * @param[in] switch_id Switch Id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_tunnel_term_table_entry_fn)(
        _Out_ sai_object_id_t *tunnel_term_table_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove tunnel termination table entry
 *
 * @param[in] tunnel_term_table_entry_id Tunnel termination table entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_tunnel_term_table_entry_fn)(
        _In_ sai_object_id_t tunnel_term_table_entry_id);

/**
 * @brief Set tunnel termination table entry attribute
 *
 * @param[in] tunnel_term_table_entry_id Tunnel termination table entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_tunnel_term_table_entry_attribute_fn)(
        _In_ sai_object_id_t tunnel_term_table_entry_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get tunnel termination table entry attributes
 *
 * @param[in] tunnel_term_table_entry_id Tunnel termination table entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_tunnel_term_table_entry_attribute_fn)(
        _In_ sai_object_id_t tunnel_term_table_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief tunnel methods table retrieved with sai_api_query()
 */
typedef struct _sai_tunnel_api_t
{
    sai_create_tunnel_map_fn                     create_tunnel_map;
    sai_remove_tunnel_map_fn                     remove_tunnel_map;
    sai_set_tunnel_map_attribute_fn              set_tunnel_map_attribute;
    sai_get_tunnel_map_attribute_fn              get_tunnel_map_attribute;
    sai_create_tunnel_fn                         create_tunnel;
    sai_remove_tunnel_fn                         remove_tunnel;
    sai_set_tunnel_attribute_fn                  set_tunnel_attribute;
    sai_get_tunnel_attribute_fn                  get_tunnel_attribute;
    sai_create_tunnel_term_table_entry_fn        create_tunnel_term_table_entry;
    sai_remove_tunnel_term_table_entry_fn        remove_tunnel_term_table_entry;
    sai_set_tunnel_term_table_entry_attribute_fn set_tunnel_term_table_entry_attribute;
    sai_get_tunnel_term_table_entry_attribute_fn get_tunnel_term_table_entry_attribute;

} sai_tunnel_api_t;

/**
 * @}
 */
#endif /** __SAITUNNEL_H_ */
