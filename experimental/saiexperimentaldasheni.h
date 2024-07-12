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
 * @file    saiexperimentaldasheni.h
 *
 * @brief   This module defines SAI extensions for DASH ENI
 *
 * @warning This module is a SAI experimental module
 */

#if !defined (__SAIEXPERIMENTALDASHENI_H_)
#define __SAIEXPERIMENTALDASHENI_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALDASH_ENI SAI - Experimental: DASH ENI specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_ENI_ETHER_ADDRESS_MAP_ENTRY_ATTR_ACTION
 */
typedef enum _sai_eni_ether_address_map_entry_action_t
{
    SAI_ENI_ETHER_ADDRESS_MAP_ENTRY_ACTION_SET_ENI,

} sai_eni_ether_address_map_entry_action_t;

/**
 * @brief Entry for eni_ether_address_map_entry
 */
typedef struct _sai_eni_ether_address_map_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key address
     */
    sai_mac_t address;

} sai_eni_ether_address_map_entry_t;

/**
 * @brief Attribute ID for dash_eni_eni_ether_address_map_entry
 */
typedef enum _sai_eni_ether_address_map_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_ENI_ETHER_ADDRESS_MAP_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_eni_ether_address_map_entry_action_t
     * @flags CREATE_AND_SET
     * @default SAI_ENI_ETHER_ADDRESS_MAP_ENTRY_ACTION_SET_ENI
     */
    SAI_ENI_ETHER_ADDRESS_MAP_ENTRY_ATTR_ACTION = SAI_ENI_ETHER_ADDRESS_MAP_ENTRY_ATTR_START,

    /**
     * @brief Action set_eni parameter ENI_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ENI
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ETHER_ADDRESS_MAP_ENTRY_ATTR_ENI_ID,

    /**
     * @brief End of attributes
     */
    SAI_ENI_ETHER_ADDRESS_MAP_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_ENI_ETHER_ADDRESS_MAP_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_ENI_ETHER_ADDRESS_MAP_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_eni_ether_address_map_entry_attr_t;

/**
 * @brief Attribute ID for dash_eni_eni
 */
typedef enum _sai_eni_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_ENI_ATTR_START,

    /**
     * @brief Action set_eni_attrs parameter CPS
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ENI_ATTR_CPS = SAI_ENI_ATTR_START,

    /**
     * @brief Action set_eni_attrs parameter PPS
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ENI_ATTR_PPS,

    /**
     * @brief Action set_eni_attrs parameter FLOWS
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ENI_ATTR_FLOWS,

    /**
     * @brief Action set_eni_attrs parameter ADMIN_STATE
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ENI_ATTR_ADMIN_STATE,

    /**
     * @brief Action set_eni_attrs parameter HA_SCOPE_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HA_SCOPE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_HA_SCOPE_ID,

    /**
     * @brief Action set_eni_attrs parameter VM_UNDERLAY_DIP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_ENI_ATTR_VM_UNDERLAY_DIP,

    /**
     * @brief Action set_eni_attrs parameter VM_VNI
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ENI_ATTR_VM_VNI,

    /**
     * @brief Action set_eni_attrs parameter VNET_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VNET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_VNET_ID,

    /**
     * @brief Action set_eni_attrs parameter PL_SIP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_ENI_ATTR_PL_SIP,

    /**
     * @brief Action set_eni_attrs parameter PL_SIP_MASK
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_ENI_ATTR_PL_SIP_MASK,

    /**
     * @brief Action set_eni_attrs parameter PL_UNDERLAY_SIP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_ENI_ATTR_PL_UNDERLAY_SIP,

    /**
     * @brief Action set_eni_attrs parameter V4_METER_POLICY_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_METER_POLICY
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_V4_METER_POLICY_ID,

    /**
     * @brief Action set_eni_attrs parameter V6_METER_POLICY_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_METER_POLICY
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_V6_METER_POLICY_ID,

    /**
     * @brief Action set_eni_attrs parameter DASH_TUNNEL_DSCP_MODE
     *
     * @type sai_dash_tunnel_dscp_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_TUNNEL_DSCP_MODE_PRESERVE_MODEL
     */
    SAI_ENI_ATTR_DASH_TUNNEL_DSCP_MODE,

    /**
     * @brief Action set_eni_attrs parameter DSCP
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_ENI_ATTR_DASH_TUNNEL_DSCP_MODE == SAI_DASH_TUNNEL_DSCP_MODE_PIPE_MODEL
     */
    SAI_ENI_ATTR_DSCP,

    /**
     * @brief Action set_eni_attrs parameter INBOUND_V4_STAGE1_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V4_STAGE1_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter INBOUND_V4_STAGE2_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V4_STAGE2_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter INBOUND_V4_STAGE3_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V4_STAGE3_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter INBOUND_V4_STAGE4_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V4_STAGE4_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter INBOUND_V4_STAGE5_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V4_STAGE5_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter INBOUND_V6_STAGE1_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V6_STAGE1_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter INBOUND_V6_STAGE2_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V6_STAGE2_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter INBOUND_V6_STAGE3_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V6_STAGE3_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter INBOUND_V6_STAGE4_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V6_STAGE4_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter INBOUND_V6_STAGE5_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V6_STAGE5_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter OUTBOUND_V4_STAGE1_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V4_STAGE1_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter OUTBOUND_V4_STAGE2_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V4_STAGE2_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter OUTBOUND_V4_STAGE3_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V4_STAGE3_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter OUTBOUND_V4_STAGE4_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V4_STAGE4_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter OUTBOUND_V4_STAGE5_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V4_STAGE5_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter OUTBOUND_V6_STAGE1_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V6_STAGE1_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter OUTBOUND_V6_STAGE2_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V6_STAGE2_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter OUTBOUND_V6_STAGE3_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V6_STAGE3_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter OUTBOUND_V6_STAGE4_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V6_STAGE4_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter OUTBOUND_V6_STAGE5_DASH_ACL_GROUP_ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V6_STAGE5_DASH_ACL_GROUP_ID,

    /**
     * @brief Action set_eni_attrs parameter DISABLE_FAST_PATH_ICMP_FLOW_REDIRECTION
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ENI_ATTR_DISABLE_FAST_PATH_ICMP_FLOW_REDIRECTION,

    /**
     * @brief Action set_eni_attrs parameter FULL_FLOW_RESIMULATION_REQUESTED
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ENI_ATTR_FULL_FLOW_RESIMULATION_REQUESTED,

    /**
     * @brief Action set_eni_attrs parameter MAX_RESIMULATED_FLOW_PER_SECOND
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ENI_ATTR_MAX_RESIMULATED_FLOW_PER_SECOND,

    /**
     * @brief Action parameter outbound routing group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_OUTBOUND_ROUTING_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_ROUTING_GROUP_ID,

    /**
     * @brief End of attributes
     */
    SAI_ENI_ATTR_END,

    /** Custom range base value */
    SAI_ENI_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_ENI_ATTR_CUSTOM_RANGE_END,

} sai_eni_attr_t;

/**
 * @brief Counter IDs for ENI in sai_get_eni_stats() call
 */
typedef enum _sai_eni_stat_t
{
    /** DASH ENI RX_BYTES stat count */
    SAI_ENI_STAT_RX_BYTES,

    /** DASH ENI RX_PACKETS stat count */
    SAI_ENI_STAT_RX_PACKETS,

    /** DASH ENI TX_BYTES stat count */
    SAI_ENI_STAT_TX_BYTES,

    /** DASH ENI TX_PACKETS stat count */
    SAI_ENI_STAT_TX_PACKETS,

    /** DASH ENI OUTBOUND_RX_BYTES stat count */
    SAI_ENI_STAT_OUTBOUND_RX_BYTES,

    /** DASH ENI OUTBOUND_RX_PACKETS stat count */
    SAI_ENI_STAT_OUTBOUND_RX_PACKETS,

    /** DASH ENI OUTBOUND_TX_BYTES stat count */
    SAI_ENI_STAT_OUTBOUND_TX_BYTES,

    /** DASH ENI OUTBOUND_TX_PACKETS stat count */
    SAI_ENI_STAT_OUTBOUND_TX_PACKETS,

    /** DASH ENI INBOUND_RX_BYTES stat count */
    SAI_ENI_STAT_INBOUND_RX_BYTES,

    /** DASH ENI INBOUND_RX_PACKETS stat count */
    SAI_ENI_STAT_INBOUND_RX_PACKETS,

    /** DASH ENI INBOUND_TX_BYTES stat count */
    SAI_ENI_STAT_INBOUND_TX_BYTES,

    /** DASH ENI INBOUND_TX_PACKETS stat count */
    SAI_ENI_STAT_INBOUND_TX_PACKETS,

    /** DASH ENI LB_FAST_PATH_ICMP_IN_BYTES stat count */
    SAI_ENI_STAT_LB_FAST_PATH_ICMP_IN_BYTES,

    /** DASH ENI LB_FAST_PATH_ICMP_IN_PACKETS stat count */
    SAI_ENI_STAT_LB_FAST_PATH_ICMP_IN_PACKETS,

    /** DASH ENI FLOW_CREATED stat count */
    SAI_ENI_STAT_FLOW_CREATED,

    /** DASH ENI FLOW_CREATE_FAILED stat count */
    SAI_ENI_STAT_FLOW_CREATE_FAILED,

    /** DASH ENI FLOW_UPDATED stat count */
    SAI_ENI_STAT_FLOW_UPDATED,

    /** DASH ENI FLOW_UPDATE_FAILED stat count */
    SAI_ENI_STAT_FLOW_UPDATE_FAILED,

    /** DASH ENI FLOW_UPDATED_BY_RESIMULATION stat count */
    SAI_ENI_STAT_FLOW_UPDATED_BY_RESIMULATION,

    /** DASH ENI FLOW_UPDATE_BY_RESIMULATION_FAILED stat count */
    SAI_ENI_STAT_FLOW_UPDATE_BY_RESIMULATION_FAILED,

    /** DASH ENI FLOW_DELETED stat count */
    SAI_ENI_STAT_FLOW_DELETED,

    /** DASH ENI FLOW_DELETE_FAILED stat count */
    SAI_ENI_STAT_FLOW_DELETE_FAILED,

    /** DASH ENI FLOW_AGED stat count */
    SAI_ENI_STAT_FLOW_AGED,

    /** DASH ENI INLINE_SYNC_PACKET_RX_BYTES stat count */
    SAI_ENI_STAT_INLINE_SYNC_PACKET_RX_BYTES,

    /** DASH ENI INLINE_SYNC_PACKET_RX_PACKETS stat count */
    SAI_ENI_STAT_INLINE_SYNC_PACKET_RX_PACKETS,

    /** DASH ENI INLINE_SYNC_PACKET_TX_BYTES stat count */
    SAI_ENI_STAT_INLINE_SYNC_PACKET_TX_BYTES,

    /** DASH ENI INLINE_SYNC_PACKET_TX_PACKETS stat count */
    SAI_ENI_STAT_INLINE_SYNC_PACKET_TX_PACKETS,

    /** DASH ENI TIMED_SYNC_PACKET_RX_BYTES stat count */
    SAI_ENI_STAT_TIMED_SYNC_PACKET_RX_BYTES,

    /** DASH ENI TIMED_SYNC_PACKET_RX_PACKETS stat count */
    SAI_ENI_STAT_TIMED_SYNC_PACKET_RX_PACKETS,

    /** DASH ENI TIMED_SYNC_PACKET_TX_BYTES stat count */
    SAI_ENI_STAT_TIMED_SYNC_PACKET_TX_BYTES,

    /** DASH ENI TIMED_SYNC_PACKET_TX_PACKETS stat count */
    SAI_ENI_STAT_TIMED_SYNC_PACKET_TX_PACKETS,

    /** DASH ENI INLINE_FLOW_CREATE_REQ_SENT stat count */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_REQ_SENT,

    /** DASH ENI INLINE_FLOW_CREATE_REQ_RECV stat count */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_REQ_RECV,

    /** DASH ENI INLINE_FLOW_CREATE_REQ_FAILED stat count */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_REQ_FAILED,

    /** DASH ENI INLINE_FLOW_CREATE_REQ_IGNORED stat count */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_REQ_IGNORED,

    /** DASH ENI INLINE_FLOW_CREATE_ACK_RECV stat count */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_ACK_RECV,

    /** DASH ENI INLINE_FLOW_CREATE_ACK_FAILED stat count */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_ACK_FAILED,

    /** DASH ENI INLINE_FLOW_CREATE_ACK_IGNORED stat count */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_ACK_IGNORED,

    /** DASH ENI TIMED_FLOW_CREATE_REQ_SENT stat count */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_REQ_SENT,

    /** DASH ENI TIMED_FLOW_CREATE_REQ_RECV stat count */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_REQ_RECV,

    /** DASH ENI TIMED_FLOW_CREATE_REQ_FAILED stat count */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_REQ_FAILED,

    /** DASH ENI TIMED_FLOW_CREATE_REQ_IGNORED stat count */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_REQ_IGNORED,

    /** DASH ENI TIMED_FLOW_CREATE_ACK_RECV stat count */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_ACK_RECV,

    /** DASH ENI TIMED_FLOW_CREATE_ACK_FAILED stat count */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_ACK_FAILED,

    /** DASH ENI TIMED_FLOW_CREATE_ACK_IGNORED stat count */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_ACK_IGNORED,

    /** DASH ENI INLINE_FLOW_UPDATE_REQ_SENT stat count */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_REQ_SENT,

    /** DASH ENI INLINE_FLOW_UPDATE_REQ_RECV stat count */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_REQ_RECV,

    /** DASH ENI INLINE_FLOW_UPDATE_REQ_FAILED stat count */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_REQ_FAILED,

    /** DASH ENI INLINE_FLOW_UPDATE_REQ_IGNORED stat count */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_REQ_IGNORED,

    /** DASH ENI INLINE_FLOW_UPDATE_ACK_RECV stat count */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_ACK_RECV,

    /** DASH ENI INLINE_FLOW_UPDATE_ACK_FAILED_RECV stat count */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_ACK_FAILED_RECV,

    /** DASH ENI INLINE_FLOW_UPDATE_ACK_IGNORED_RECV stat count */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_ACK_IGNORED_RECV,

    /** DASH ENI TIMED_FLOW_UPDATE_REQ_SENT stat count */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_REQ_SENT,

    /** DASH ENI TIMED_FLOW_UPDATE_REQ_RECV stat count */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_REQ_RECV,

    /** DASH ENI TIMED_FLOW_UPDATE_REQ_FAILED stat count */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_REQ_FAILED,

    /** DASH ENI TIMED_FLOW_UPDATE_REQ_IGNORED stat count */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_REQ_IGNORED,

    /** DASH ENI TIMED_FLOW_UPDATE_ACK_RECV stat count */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_ACK_RECV,

    /** DASH ENI TIMED_FLOW_UPDATE_ACK_FAILED_RECV stat count */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_ACK_FAILED_RECV,

    /** DASH ENI TIMED_FLOW_UPDATE_ACK_IGNORED_RECV stat count */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_ACK_IGNORED_RECV,

    /** DASH ENI INLINE_FLOW_DELETE_REQ_SENT stat count */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_REQ_SENT,

    /** DASH ENI INLINE_FLOW_DELETE_REQ_RECV stat count */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_REQ_RECV,

    /** DASH ENI INLINE_FLOW_DELETE_REQ_FAILED stat count */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_REQ_FAILED,

    /** DASH ENI INLINE_FLOW_DELETE_REQ_IGNORED stat count */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_REQ_IGNORED,

    /** DASH ENI INLINE_FLOW_DELETE_ACK_RECV stat count */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_ACK_RECV,

    /** DASH ENI INLINE_FLOW_DELETE_ACK_FAILED_RECV stat count */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_ACK_FAILED_RECV,

    /** DASH ENI INLINE_FLOW_DELETE_ACK_IGNORED_RECV stat count */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_ACK_IGNORED_RECV,

    /** DASH ENI TIMED_FLOW_DELETE_REQ_SENT stat count */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_REQ_SENT,

    /** DASH ENI TIMED_FLOW_DELETE_REQ_RECV stat count */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_REQ_RECV,

    /** DASH ENI TIMED_FLOW_DELETE_REQ_FAILED stat count */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_REQ_FAILED,

    /** DASH ENI TIMED_FLOW_DELETE_REQ_IGNORED stat count */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_REQ_IGNORED,

    /** DASH ENI TIMED_FLOW_DELETE_ACK_RECV stat count */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_ACK_RECV,

    /** DASH ENI TIMED_FLOW_DELETE_ACK_FAILED_RECV stat count */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_ACK_FAILED_RECV,

    /** DASH ENI TIMED_FLOW_DELETE_ACK_IGNORED stat count */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_ACK_IGNORED,

    /** DASH ENI OUTBOUND_ROUTING_ENTRY_MISS_DROP_PACKETS stat count */
    SAI_ENI_STAT_OUTBOUND_ROUTING_ENTRY_MISS_DROP_PACKETS,

    /** DASH ENI OUTBOUND_CA_PA_ENTRY_MISS_DROP_PACKETS stat count */
    SAI_ENI_STAT_OUTBOUND_CA_PA_ENTRY_MISS_DROP_PACKETS,

    /** DASH ENI INBOUND_ROUTING_ENTRY_MISS_DROP_PACKETS stat count */
    SAI_ENI_STAT_INBOUND_ROUTING_ENTRY_MISS_DROP_PACKETS,

    /** DASH ENI OUTBOUND_ROUTING_GROUP_MISS_DROP_PACKETS stat count */
    SAI_ENI_STAT_OUTBOUND_ROUTING_GROUP_MISS_DROP_PACKETS,

    /** DASH ENI OUTBOUND_ROUTING_GROUP_DISABLED_DROP_PACKETS stat count */
    SAI_ENI_STAT_OUTBOUND_ROUTING_GROUP_DISABLED_DROP_PACKETS,

} sai_eni_stat_t;

/**
 * @brief Create dash_eni_eni_ether_address_map_entry
 *
 * @param[in] eni_ether_address_map_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_eni_ether_address_map_entry_fn)(
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_eni_eni_ether_address_map_entry
 *
 * @param[in] eni_ether_address_map_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_eni_ether_address_map_entry_fn)(
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry);

/**
 * @brief Set attribute for dash_eni_eni_ether_address_map_entry
 *
 * @param[in] eni_ether_address_map_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_eni_ether_address_map_entry_attribute_fn)(
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_eni_eni_ether_address_map_entry
 *
 * @param[in] eni_ether_address_map_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_eni_ether_address_map_entry_attribute_fn)(
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create dash_eni_eni_ether_address_map_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] eni_ether_address_map_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_eni_ether_address_map_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove dash_eni_eni_ether_address_map_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] eni_ether_address_map_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_eni_ether_address_map_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create dash_eni_eni
 *
 * @param[out] eni_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_eni_fn)(
        _Out_ sai_object_id_t *eni_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_eni_eni
 *
 * @param[in] eni_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_eni_fn)(
        _In_ sai_object_id_t eni_id);

/**
 * @brief Set attribute for dash_eni_eni
 *
 * @param[in] eni_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_eni_attribute_fn)(
        _In_ sai_object_id_t eni_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_eni_eni
 *
 * @param[in] eni_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_eni_attribute_fn)(
        _In_ sai_object_id_t eni_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get ENI statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] eni_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_eni_stats_fn)(
        _In_ sai_object_id_t eni_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get ENI statistics counters extended.
 *
 * @param[in] eni_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_eni_stats_ext_fn)(
        _In_ sai_object_id_t eni_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear ENI statistics counters.
 *
 * @param[in] eni_id Entry id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_eni_stats_fn)(
        _In_ sai_object_id_t eni_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

typedef struct _sai_dash_eni_api_t
{
    sai_create_eni_ether_address_map_entry_fn           create_eni_ether_address_map_entry;
    sai_remove_eni_ether_address_map_entry_fn           remove_eni_ether_address_map_entry;
    sai_set_eni_ether_address_map_entry_attribute_fn    set_eni_ether_address_map_entry_attribute;
    sai_get_eni_ether_address_map_entry_attribute_fn    get_eni_ether_address_map_entry_attribute;
    sai_bulk_create_eni_ether_address_map_entry_fn      create_eni_ether_address_map_entries;
    sai_bulk_remove_eni_ether_address_map_entry_fn      remove_eni_ether_address_map_entries;

    sai_create_eni_fn                                   create_eni;
    sai_remove_eni_fn                                   remove_eni;
    sai_set_eni_attribute_fn                            set_eni_attribute;
    sai_get_eni_attribute_fn                            get_eni_attribute;
    sai_get_eni_stats_fn                                get_eni_stats;
    sai_get_eni_stats_ext_fn                            get_eni_stats_ext;
    sai_clear_eni_stats_fn                              clear_eni_stats;
    sai_bulk_object_create_fn                           create_enis;
    sai_bulk_object_remove_fn                           remove_enis;

} sai_dash_eni_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHENI_H_ */
