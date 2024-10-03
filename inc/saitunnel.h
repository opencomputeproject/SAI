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

    /** TUNNEL Map VNI to Virtual Router ID */
    SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID = 0x00000006,

    /** TUNNEL Map Virtual Router ID to VNI */
    SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI = 0x00000007,

    /** TUNNEL Map VSID to VLAN ID */
    SAI_TUNNEL_MAP_TYPE_VSID_TO_VLAN_ID = 0x00000008,

    /** TUNNEL Map VLAN ID to VSID */
    SAI_TUNNEL_MAP_TYPE_VLAN_ID_TO_VSID = 0x00000009,

    /** TUNNEL Map VSID to Bridge IF */
    SAI_TUNNEL_MAP_TYPE_VSID_TO_BRIDGE_IF = 0x0000000a,

    /** TUNNEL Map Bridge IF to VSID */
    SAI_TUNNEL_MAP_TYPE_BRIDGE_IF_TO_VSID = 0x0000000b,

    /** TUNNEL Map Virtual Router ID to SRV6 VPN SID */
    SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VPN_SID = 0x0000000c,

    /** TUNNEL Map Prefix Aggregation ID to SRV6 VPN SID */
    SAI_TUNNEL_MAP_TYPE_PREFIX_AGG_ID_TO_SRV6_VPN_SID = 0x0000000d,

    /** Custom range base value */
    SAI_TUNNEL_MAP_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_tunnel_map_type_t;

typedef enum _sai_tunnel_map_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_START = 0x00000000,

    /**
     * @brief Tunnel Map type
     *
     * @type sai_tunnel_map_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE = SAI_TUNNEL_MAP_ENTRY_ATTR_START,

    /**
     * @brief Tunnel map ex
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL_MAP
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP = 0x00000001,

    /**
     * @brief Inner ECN key
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_OECN_TO_UECN or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_UECN_OECN_TO_OECN
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_OECN_KEY = 0x00000002,

    /**
     * @brief Inner ECN value
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_UECN_OECN_TO_OECN
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_OECN_VALUE = 0x00000003,

    /**
     * @brief Outer ECN key
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_UECN_OECN_TO_OECN
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_UECN_KEY = 0x00000004,

    /**
     * @brief Outer ECN value
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_OECN_TO_UECN
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_UECN_VALUE = 0x00000005,

    /**
     * @brief Vlan ID key
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan true
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VLAN_ID_TO_VNI or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VLAN_ID_TO_VSID
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_VLAN_ID_KEY = 0x00000006,

    /**
     * @brief Vlan ID value
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan true
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VNI_TO_VLAN_ID or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VSID_TO_VLAN_ID
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_VLAN_ID_VALUE = 0x00000007,

    /**
     * @brief VNI ID key
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VNI_TO_VLAN_ID or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VNI_TO_BRIDGE_IF or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_KEY = 0x00000008,

    /**
     * @brief VNI ID value
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VLAN_ID_TO_VNI or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_BRIDGE_IF_TO_VNI or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_VALUE = 0x00000009,

    /**
     * @brief Bridge ID key
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_BRIDGE_IF_TO_VNI or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_BRIDGE_IF_TO_VSID
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_KEY = 0x0000000a,

    /**
     * @brief Bridge ID value
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VNI_TO_BRIDGE_IF or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VSID_TO_BRIDGE_IF
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_VALUE = 0x0000000b,

    /**
     * @brief Virtual Router ID key
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VPN_SID
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_VIRTUAL_ROUTER_ID_KEY = 0x0000000c,

    /**
     * @brief Virtual Router ID value
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_VIRTUAL_ROUTER_ID_VALUE = 0x0000000d,

    /**
     * @brief VSID ID key
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VSID_TO_VLAN_ID or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VSID_TO_BRIDGE_IF
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_VSID_ID_KEY = 0x0000000e,

    /**
     * @brief VSID ID value
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VLAN_ID_TO_VSID or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_BRIDGE_IF_TO_VSID
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_VSID_ID_VALUE = 0x0000000f,

    /**
     * @brief Prefix Aggregation ID key
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_PREFIX_AGG_ID_TO_SRV6_VPN_SID
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_PREFIX_AGG_ID_KEY = 0x00000010,

    /**
     * @brief SRV6 VPN SID
     *
     * @type sai_ip6_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_PREFIX_AGG_ID_TO_SRV6_VPN_SID or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VPN_SID
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_SRV6_VPN_SID_VALUE = 0x00000011,

    /**
     * @brief End of attributes
     */
    SAI_TUNNEL_MAP_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_TUNNEL_MAP_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TUNNEL_MAP_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_tunnel_map_entry_attr_t;

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
     * @brief Tunnel map entries associated with this map.
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL_MAP_ENTRY
     */
    SAI_TUNNEL_MAP_ATTR_ENTRY_LIST,

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
 * @param[out] tunnel_map_id Tunnel Map Id
 * @param[in] switch_id Switch Id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tunnel_map_fn)(
        _Out_ sai_object_id_t *tunnel_map_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove tunnel Map
 *
 * @param[in] tunnel_map_id Tunnel Map id to be removed
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tunnel_map_fn)(
        _In_ sai_object_id_t tunnel_map_id);

/**
 * @brief Set attributes for tunnel map
 *
 * @param[in] tunnel_map_id Tunnel Map Id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tunnel_map_attribute_fn)(
        _In_ sai_object_id_t tunnel_map_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attributes of tunnel map
 *
 * @param[in] tunnel_map_id Tunnel map id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tunnel_map_attribute_fn)(
        _In_ sai_object_id_t tunnel_map_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Defines tunnel TTL mode
 */
typedef enum _sai_tunnel_ttl_mode_t
{
    /**
     * @brief The uniform model
     *
     * Where the TTL field is preserved end-to-end by copying into the outer
     * header on encapsulation and copying from the outer header on
     * decapsulation. This is applicable for inner IP packets. If the inner
     * packet is a non-IP packet, then the value is undefined and implementation
     * can chose a valid/meaningful outer TTL value, say in the case of VXLAN encap.
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
 * @brief Defines tunnel DSCP mode
 */
typedef enum _sai_tunnel_dscp_mode_t
{
    /**
     * @brief The uniform model
     *
     * Where the DSCP field is preserved end-to-end by copying into the
     * outer header on encapsulation and copying from the outer header on
     * decapsulation. This is applicable for inner IP packets. If the inner
     * packet is a non-IP packet, then the value is undefined and implementation
     * can chose a valid/meaningful outer DSCP value, say in the case of VXLAN encap.
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
     * field of inner header remains the same on decapsulation.
     */
    SAI_TUNNEL_DSCP_MODE_PIPE_MODEL

} sai_tunnel_dscp_mode_t;

/**
 * @brief Defines tunnel peer mode
 */
typedef enum _sai_tunnel_peer_mode_t
{
    /**
     * @brief P2P Tunnel
     */
    SAI_TUNNEL_PEER_MODE_P2P,

    /**
     * @brief P2MP Tunnel
     */
    SAI_TUNNEL_PEER_MODE_P2MP,

} sai_tunnel_peer_mode_t;

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
     * @isresourcetype true
     */
    SAI_TUNNEL_ATTR_TYPE = SAI_TUNNEL_ATTR_START,

    /**
     * @brief Tunnel underlay interface
     *
     * Underlay interface is router interface when conditions are met.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @condition SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_GRE or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_VXLAN or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_SRV6 or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_NVGRE or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_ESP or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_UDP_ESP or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_VXLAN_UDP_ESP
     */
    SAI_TUNNEL_ATTR_UNDERLAY_INTERFACE,

    /**
     * @brief Tunnel overlay interface
     *
     * Overlay interface is router interface when conditions are met.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @condition SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_GRE or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_ESP or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_UDP_ESP
     */
    SAI_TUNNEL_ATTR_OVERLAY_INTERFACE,

    /* Tunnel encap attributes */

    /**
     * @brief Tunnel Peer Mode
     *
     * @type sai_tunnel_peer_mode_t
     * @flags CREATE_ONLY
     * @default SAI_TUNNEL_PEER_MODE_P2MP
     */
    SAI_TUNNEL_ATTR_PEER_MODE,

    /**
     * @brief Tunnel src IP
     *
     * @type sai_ip_address_t
     * @flags CREATE_ONLY
     * @default 0.0.0.0
     */
    SAI_TUNNEL_ATTR_ENCAP_SRC_IP,

    /**
     * @brief Tunnel Destination IP
     *
     * @type sai_ip_address_t
     * @flags CREATE_ONLY
     * @default 0.0.0.0
     * @validonly SAI_TUNNEL_ATTR_PEER_MODE == SAI_TUNNEL_PEER_MODE_P2P
     */
    SAI_TUNNEL_ATTR_ENCAP_DST_IP,

    /**
     * @brief Tunnel TTL mode (pipe or uniform model)
     *
     * @type sai_tunnel_ttl_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_TUNNEL_TTL_MODE_UNIFORM_MODEL
     */
    SAI_TUNNEL_ATTR_ENCAP_TTL_MODE,

    /**
     * @brief Tunnel TTL value
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 255
     * @validonly SAI_TUNNEL_ATTR_ENCAP_TTL_MODE == SAI_TUNNEL_TTL_MODE_PIPE_MODEL
     */
    SAI_TUNNEL_ATTR_ENCAP_TTL_VAL,

    /**
     * @brief Tunnel DSCP mode (pipe or uniform model)
     *
     * @type sai_tunnel_dscp_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_TUNNEL_DSCP_MODE_UNIFORM_MODEL
     */
    SAI_TUNNEL_ATTR_ENCAP_DSCP_MODE,

    /**
     * @brief Tunnel DSCP value (6 bits)
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_TUNNEL_ATTR_ENCAP_DSCP_MODE == SAI_TUNNEL_DSCP_MODE_PIPE_MODEL
     */
    SAI_TUNNEL_ATTR_ENCAP_DSCP_VAL,

    /**
     * @brief Tunnel GRE key valid
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_TUNNEL_ATTR_ENCAP_GRE_KEY_VALID,

    /**
     * @brief Tunnel GRE key
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_TUNNEL_ATTR_ENCAP_GRE_KEY_VALID == true
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
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL_MAP
     * @default empty
     */
    SAI_TUNNEL_ATTR_ENCAP_MAPPERS,

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
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL_MAP
     * @default empty
     */
    SAI_TUNNEL_ATTR_DECAP_MAPPERS,

    /**
     * @brief Tunnel TTL mode (pipe or uniform model)
     *
     * @type sai_tunnel_ttl_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_TUNNEL_TTL_MODE_UNIFORM_MODEL
     */
    SAI_TUNNEL_ATTR_DECAP_TTL_MODE,

    /**
     * @brief Tunnel DSCP mode (pipe or uniform model)
     *
     * @type sai_tunnel_dscp_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_TUNNEL_DSCP_MODE_UNIFORM_MODEL
     */
    SAI_TUNNEL_ATTR_DECAP_DSCP_MODE,

    /**
     * @brief Tunnel term table entries associated with this tunnel.
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY
     */
    SAI_TUNNEL_ATTR_TERM_TABLE_ENTRY_LIST,

    /**
     * @brief Packet action when a packet ingress and gets routed back to same tunnel
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     * @isresourcetype true
     */
    SAI_TUNNEL_ATTR_LOOPBACK_PACKET_ACTION,

    /**
     * @brief Tunnel VXLAN UDP source port mode
     *
     * @type sai_tunnel_vxlan_udp_sport_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_TUNNEL_VXLAN_UDP_SPORT_MODE_EPHEMERAL
     */
    SAI_TUNNEL_ATTR_VXLAN_UDP_SPORT_MODE,

    /**
     * @brief Tunnel UDP source port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     * @validonly SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_VXLAN and SAI_TUNNEL_ATTR_VXLAN_UDP_SPORT_MODE == SAI_TUNNEL_VXLAN_UDP_SPORT_MODE_USER_DEFINED
     */
    SAI_TUNNEL_ATTR_VXLAN_UDP_SPORT,

    /**
     * @brief Tunnel UDP source port mask
     *
     * Sport mask defining the number of least significant bits
     * reserved for the calculated hash value. 0 means a fixed value.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     * @validonly SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_VXLAN and SAI_TUNNEL_ATTR_VXLAN_UDP_SPORT_MODE == SAI_TUNNEL_VXLAN_UDP_SPORT_MODE_USER_DEFINED
     */
    SAI_TUNNEL_ATTR_VXLAN_UDP_SPORT_MASK,

    /**
     * @brief IPsec encryption SA index
     *
     * Index to bind an egress IPsec SA to a tunnel.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_ESP or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP_UDP_ESP or SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_VXLAN_UDP_ESP
     */
    SAI_TUNNEL_ATTR_SA_INDEX,

    /**
     * @brief List of ports that are programmed with SAs for this IPsec tunnel.
     * Useful only when IPsec is implemented in a PHY Chip (different sai_switch
     * object).
     *
     * For IPsec hardware in the Switch ASIC, the per-tunnel port list can be
     * derived from the union of SAI_IPSEC_SA_ATTR_IPSEC_PORT_LIST for all
     * sai_ipsec_sa objects for that tunnel.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT
     * @default empty
     */
    SAI_TUNNEL_ATTR_IPSEC_SA_PORT_LIST,

    /**
     * @brief Enable TC AND COLOR -> DSCP MAP on tunnel at encapsulation (access-to-network) node to remark the DSCP in tunnel header
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TUNNEL_ATTR_ENCAP_QOS_TC_AND_COLOR_TO_DSCP_MAP,

    /**
     * @brief Enable TC -> Queue MAP on tunnel encap
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on tunnel.
     * Default no map, i.e. packets are queued with static mapping.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TUNNEL_ATTR_ENCAP_QOS_TC_TO_QUEUE_MAP,

    /**
     * @brief Enable DSCP -> TC MAP on tunnel at termination (Network-to-access) node. This map if configured overrides the port MAP
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TUNNEL_ATTR_DECAP_QOS_DSCP_TO_TC_MAP,

    /**
     * @brief Enable TC -> Priority Group MAP. TC is derived from the tunnel MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TUNNEL_ATTR_DECAP_QOS_TC_TO_PRIORITY_GROUP_MAP,

    /**
     * @brief Drop tunnel packets with not allowed UDP source port
     *
     * Upon enabling this feature, if the tunnel packet ingresses with
     * UDP source port outside of range defined for this tunnel, it
     * will be dropped.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_VXLAN and SAI_TUNNEL_ATTR_VXLAN_UDP_SPORT_MODE == SAI_TUNNEL_VXLAN_UDP_SPORT_MODE_USER_DEFINED
     */
    SAI_TUNNEL_ATTR_VXLAN_UDP_SPORT_SECURITY,

    /**
     * @brief Set tunnel statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_TUNNEL_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_TUNNEL_ATTR_SELECTIVE_COUNTER_LIST,

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
 * @brief Tunnel counter IDs in sai_get_tunnel_stats() call
 */
typedef enum _sai_tunnel_stat_t
{
    /** Ingress byte stat count */
    SAI_TUNNEL_STAT_IN_OCTETS,

    /** Ingress packet stat count */
    SAI_TUNNEL_STAT_IN_PACKETS,

    /** Egress byte stat count */
    SAI_TUNNEL_STAT_OUT_OCTETS,

    /** Egress packet stat count */
    SAI_TUNNEL_STAT_OUT_PACKETS

} sai_tunnel_stat_t;

/**
 * @brief Create tunnel
 *
 * @param[out] tunnel_id Tunnel id
 * @param[in] switch_id Switch Id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
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
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tunnel_fn)(
        _In_ sai_object_id_t tunnel_id);

/**
 * @brief Set tunnel attribute
 *
 * @param[in] tunnel_id Tunnel id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tunnel_attribute_fn)(
        _In_ sai_object_id_t tunnel_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get tunnel attributes
 *
 * @param[in] tunnel_id Tunnel id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tunnel_attribute_fn)(
        _In_ sai_object_id_t tunnel_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get tunnel statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] tunnel_id Tunnel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tunnel_stats_fn)(
        _In_ sai_object_id_t tunnel_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get tunnel statistics counters extended.
 *
 * @param[in] tunnel_id Tunnel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tunnel_stats_ext_fn)(
        _In_ sai_object_id_t tunnel_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear tunnel statistics counters.
 *
 * @param[in] tunnel_id Tunnel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_tunnel_stats_fn)(
        _In_ sai_object_id_t tunnel_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Defines tunnel termination table entry type
 */
typedef enum _sai_tunnel_term_table_entry_type_t
{
    /** Tunnel termination table point to point entry match on dst & src IP & tunnel type */
    SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P,

    /** Tunnel termination table destination point to multi point source entry match on dst IP & src IP+mask & tunnel type */
    SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2MP,

    /** Tunnel termination table multi point destination to point source entry match on dst IP+mask & src IP & tunnel type */
    SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_MP2P,

    /** Tunnel termination table multi point to multi point entry match on dst IP+mask & src IP+mask & tunnel type */
    SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_MP2MP,

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
     * This attribute is deprecated and removed from
     * the termination entry key
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @deprecated true
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
     * @brief Tunnel termination IP address
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE == SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P or SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE == SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2MP
     * @relaxed true
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_DST_IP,

    /**
     * @brief Tunnel termination IP address mask
     *
     * @type sai_ip_address_t
     * @flags CREATE_ONLY
     * @default 0.0.0.0
     * @validonly SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE == SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_MP2P or SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE == SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_MP2MP
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_DST_IP_MASK,

    /**
     * @brief Tunnel source IP address
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE == SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P or SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE == SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_MP2P
     * @relaxed true
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_SRC_IP,

    /**
     * @brief Tunnel source IP address mask
     *
     * @type sai_ip_address_t
     * @flags CREATE_ONLY
     * @default 0.0.0.0
     * @validonly SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE == SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2MP or SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE == SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_MP2MP
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_SRC_IP_MASK,

    /**
     * @brief Tunnel type
     *
     * @type sai_tunnel_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TUNNEL_TYPE,

    /**
     * @brief Tunnel id to be use for decap
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_ACTION_TUNNEL_ID,

    /** READ-ONLY */

    /**
     * @brief Tunnel term table entry IP address family
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_IP_ADDR_FAMILY,

    /**
     * @brief IPsec packet verified by hardware Security Engine.  Valid only for
     * tunnel type SAI_TUNNEL_TYPE_IPINIP_ESP, SAI_TUNNEL_TYPE_IPINIP_UDP_ESP
     * or SAI_TUNNEL_TYPE_VXLAN_UDP_ESP
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_IPSEC_VERIFIED,

    /**
     * @brief Priority
     *
     * Value must be in the range defined in
     * \[#SAI_SWITCH_ATTR_TUNNEL_TERM_TABLE_ENTRY_MINIMUM_PRIORITY,
     * #SAI_SWITCH_ATTR_TUNNEL_TERM_TABLE_ENTRY_MAXIMUM_PRIORITY\]
     * (default = #SAI_SWITCH_ATTR_TUNNEL_TERM_TABLE_ENTRY_MINIMUM_PRIORITY)
     * higher value in priority equals higher priority
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_PRIORITY,

    /**
     * @brief End of attributes
     */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** Enc of custom range base */
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
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
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
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tunnel_term_table_entry_fn)(
        _In_ sai_object_id_t tunnel_term_table_entry_id);

/**
 * @brief Set tunnel termination table entry attribute
 *
 * @param[in] tunnel_term_table_entry_id Tunnel termination table entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
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
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tunnel_term_table_entry_attribute_fn)(
        _In_ sai_object_id_t tunnel_term_table_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create tunnel map item
 *
 * @param[out] tunnel_map_entry_id Tunnel map item id
 * @param[in] switch_id Switch Id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tunnel_map_entry_fn)(
        _Out_ sai_object_id_t *tunnel_map_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove tunnel map item
 *
 * @param[in] tunnel_map_entry_id Tunnel map item id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tunnel_map_entry_fn)(
        _In_ sai_object_id_t tunnel_map_entry_id);

/**
 * @brief Set tunnel map item attribute
 *
 * @param[in] tunnel_map_entry_id Tunnel map item id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tunnel_map_entry_attribute_fn)(
        _In_ sai_object_id_t tunnel_map_entry_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get tunnel map item attributes
 *
 * @param[in] tunnel_map_entry_id Tunnel map item id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tunnel_map_entry_attribute_fn)(
        _In_ sai_object_id_t tunnel_map_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Tunnel methods table retrieved with sai_api_query()
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
    sai_get_tunnel_stats_fn                      get_tunnel_stats;
    sai_get_tunnel_stats_ext_fn                  get_tunnel_stats_ext;
    sai_clear_tunnel_stats_fn                    clear_tunnel_stats;
    sai_create_tunnel_term_table_entry_fn        create_tunnel_term_table_entry;
    sai_remove_tunnel_term_table_entry_fn        remove_tunnel_term_table_entry;
    sai_set_tunnel_term_table_entry_attribute_fn set_tunnel_term_table_entry_attribute;
    sai_get_tunnel_term_table_entry_attribute_fn get_tunnel_term_table_entry_attribute;
    sai_create_tunnel_map_entry_fn               create_tunnel_map_entry;
    sai_remove_tunnel_map_entry_fn               remove_tunnel_map_entry;
    sai_set_tunnel_map_entry_attribute_fn        set_tunnel_map_entry_attribute;
    sai_get_tunnel_map_entry_attribute_fn        get_tunnel_map_entry_attribute;
    sai_bulk_object_create_fn                    create_tunnels;
    sai_bulk_object_remove_fn                    remove_tunnels;
    sai_bulk_object_set_attribute_fn             set_tunnels_attribute;
    sai_bulk_object_get_attribute_fn             get_tunnels_attribute;

} sai_tunnel_api_t;

/**
 * @}
 */
#endif /** __SAITUNNEL_H_ */
