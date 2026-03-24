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

#include <saitypesextensions.h>

/**
 * @defgroup SAIEXPERIMENTALDASHENI SAI - Experimental: DASH ENI specific API definitions
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
 * @brief Attribute ID for ENI ether address map entry
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
     * @brief Action parameter ENI id
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
 * @brief Attribute ID for ENI
 */
typedef enum _sai_eni_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_ENI_ATTR_START,

    /**
     * @brief Action parameter CPS
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ENI_ATTR_CPS = SAI_ENI_ATTR_START,

    /**
     * @brief Action parameter PPS
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ENI_ATTR_PPS,

    /**
     * @brief Action parameter flows
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ENI_ATTR_FLOWS,

    /**
     * @brief Action parameter admin state
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ENI_ATTR_ADMIN_STATE,

    /**
     * @brief Action parameter HA scope id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HA_SCOPE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_HA_SCOPE_ID,

    /**
     * @brief Action parameter  underlay dip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_ENI_ATTR_VM_UNDERLAY_DIP,

    /**
     * @brief Action parameter  VNI
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ENI_ATTR_VM_VNI,

    /**
     * @brief Action parameter VNET id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VNET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_VNET_ID,

    /**
     * @brief Action parameter PL sip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_ENI_ATTR_PL_SIP,

    /**
     * @brief Action parameter PL sip mask
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_ENI_ATTR_PL_SIP_MASK,

    /**
     * @brief Action parameter PL underlay sip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_ENI_ATTR_PL_UNDERLAY_SIP,

    /**
     * @brief Action parameter v4 meter policy id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_METER_POLICY
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_V4_METER_POLICY_ID,

    /**
     * @brief Action parameter v6 meter policy id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_METER_POLICY
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_V6_METER_POLICY_ID,

    /**
     * @brief Action parameter DASH tunnel DSCP mode
     *
     * @type sai_dash_tunnel_dscp_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_TUNNEL_DSCP_MODE_PRESERVE_MODEL
     */
    SAI_ENI_ATTR_DASH_TUNNEL_DSCP_MODE,

    /**
     * @brief Action parameter DSCP
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_ENI_ATTR_DASH_TUNNEL_DSCP_MODE == SAI_DASH_TUNNEL_DSCP_MODE_PIPE_MODEL
     */
    SAI_ENI_ATTR_DSCP,

    /**
     * @brief Action parameter inbound v4 stage1 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V4_STAGE1_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter inbound v4 stage2 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V4_STAGE2_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter inbound v4 stage3 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V4_STAGE3_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter inbound v4 stage4 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V4_STAGE4_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter inbound v4 stage5 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V4_STAGE5_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter inbound v6 stage1 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V6_STAGE1_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter inbound v6 stage2 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V6_STAGE2_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter inbound v6 stage3 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V6_STAGE3_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter inbound v6 stage4 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V6_STAGE4_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter inbound v6 stage5 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_INBOUND_V6_STAGE5_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter outbound v4 stage1 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V4_STAGE1_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter outbound v4 stage2 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V4_STAGE2_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter outbound v4 stage3 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V4_STAGE3_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter outbound v4 stage4 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V4_STAGE4_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter outbound v4 stage5 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V4_STAGE5_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter outbound v6 stage1 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V6_STAGE1_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter outbound v6 stage2 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V6_STAGE2_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter outbound v6 stage3 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V6_STAGE3_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter outbound v6 stage4 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V6_STAGE4_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter outbound v6 stage5 DASH ACL group id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DASH_ACL_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_OUTBOUND_V6_STAGE5_DASH_ACL_GROUP_ID,

    /**
     * @brief Action parameter disable fast path ICMP flow redirection
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ENI_ATTR_DISABLE_FAST_PATH_ICMP_FLOW_REDIRECTION,

    /**
     * @brief Action parameter full flow re-simulation requested
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ENI_ATTR_FULL_FLOW_RESIMULATION_REQUESTED,

    /**
     * @brief Action parameter max re-simulated flow per second
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
     * @brief Action parameter is HA flow owner
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ENI_ATTR_IS_HA_FLOW_OWNER,

    /**
     * @brief Action parameter enable reverse tunnel learning
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ENI_ATTR_ENABLE_REVERSE_TUNNEL_LEARNING,

    /**
     * @brief Action parameter reverse tunnel sip
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_ENI_ATTR_REVERSE_TUNNEL_SIP,

    /**
     * @brief Action parameter flow table id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_FLOW_TABLE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ENI_ATTR_FLOW_TABLE_ID,

    /**
     * @brief Action parameter DASH ENI mode
     *
     * @type sai_dash_eni_mode_t
     * @flags CREATE_ONLY
     * @default SAI_DASH_ENI_MODE_VM
     */
    SAI_ENI_ATTR_DASH_ENI_MODE,

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
 * @brief Counter IDs for ENI
 */
typedef enum _sai_eni_stat_t
{
    /** Number of bytes received by the ENI */
    SAI_ENI_STAT_RX_BYTES,

    /** Number of packets received by the ENI */
    SAI_ENI_STAT_RX_PACKETS,

    /** Number of bytes transmitted by the ENI */
    SAI_ENI_STAT_TX_BYTES,

    /** Number of packets transmitted by the ENI */
    SAI_ENI_STAT_TX_PACKETS,

    /** Number of bytes received for outbound sessions */
    SAI_ENI_STAT_OUTBOUND_RX_BYTES,

    /** Number of packets received for outbound sessions */
    SAI_ENI_STAT_OUTBOUND_RX_PACKETS,

    /** Number of bytes transmitted for outbound sessions */
    SAI_ENI_STAT_OUTBOUND_TX_BYTES,

    /** Number of packets transmitted for outbound sessions */
    SAI_ENI_STAT_OUTBOUND_TX_PACKETS,

    /** Number of bytes received for inbound sessions */
    SAI_ENI_STAT_INBOUND_RX_BYTES,

    /** Number of packets received for inbound sessions */
    SAI_ENI_STAT_INBOUND_RX_PACKETS,

    /** Number of bytes transmitted for inbound sessions */
    SAI_ENI_STAT_INBOUND_TX_BYTES,

    /** Number of packets transmitted for inbound sessions */
    SAI_ENI_STAT_INBOUND_TX_PACKETS,

    /** Number of bytes received for fast path ICMP flow redirect messages */
    SAI_ENI_STAT_LB_FAST_PATH_ICMP_IN_BYTES,

    /** Number of fast path ICMP flow redirect messages received */
    SAI_ENI_STAT_LB_FAST_PATH_ICMP_IN_PACKETS,

    /** Number of flows created */
    SAI_ENI_STAT_FLOW_CREATED,

    /** Number of failed flow-create */
    SAI_ENI_STAT_FLOW_CREATE_FAILED,

    /** Number of flows updated */
    SAI_ENI_STAT_FLOW_UPDATED,

    /** Number of failed flow-update */
    SAI_ENI_STAT_FLOW_UPDATE_FAILED,

    /** Number of flows updated by re-simulation */
    SAI_ENI_STAT_FLOW_UPDATED_BY_RESIMULATION,

    /** Number of failed flow-update by re-simulation */
    SAI_ENI_STAT_FLOW_UPDATE_BY_RESIMULATION_FAILED,

    /** Number of flows deleted */
    SAI_ENI_STAT_FLOW_DELETED,

    /** Number of failed flow-delete */
    SAI_ENI_STAT_FLOW_DELETE_FAILED,

    /** Number of flows idle-aged */
    SAI_ENI_STAT_FLOW_AGED,

    /** Number of bytes received for inline sync packets */
    SAI_ENI_STAT_INLINE_SYNC_PACKET_RX_BYTES,

    /** Number of inline sync packets received */
    SAI_ENI_STAT_INLINE_SYNC_PACKET_RX_PACKETS,

    /** Number of bytes transmitted for inline sync packets */
    SAI_ENI_STAT_INLINE_SYNC_PACKET_TX_BYTES,

    /** Number of inline sync packets transmitted */
    SAI_ENI_STAT_INLINE_SYNC_PACKET_TX_PACKETS,

    /** Number of bytes received for timed sync packets */
    SAI_ENI_STAT_TIMED_SYNC_PACKET_RX_BYTES,

    /** Number of timed sync packets received */
    SAI_ENI_STAT_TIMED_SYNC_PACKET_RX_PACKETS,

    /** Number of bytes transmitted for timed sync packets */
    SAI_ENI_STAT_TIMED_SYNC_PACKET_TX_BYTES,

    /** Number of timed sync packets transmitted */
    SAI_ENI_STAT_TIMED_SYNC_PACKET_TX_PACKETS,

    /** Number of inline flow-create requests sent */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_REQ_SENT,

    /** Number of inline flow-create requests received */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_REQ_RECV,

    /** Number of failed inline flow-create requests */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_REQ_FAILED,

    /** Number of ignored inline flow-create requests */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_REQ_IGNORED,

    /** Number of inline flow-create acknowledgements received */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_ACK_RECV,

    /** Number of failed inline flow-create acknowledgements */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_ACK_FAILED,

    /** Number of ignored inline flow-create acknowledgements */
    SAI_ENI_STAT_INLINE_FLOW_CREATE_ACK_IGNORED,

    /** Number of timed flow-create requests sent */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_REQ_SENT,

    /** Number of timed flow-create requests received */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_REQ_RECV,

    /** Number of failed timed flow-create requests */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_REQ_FAILED,

    /** Number of ignored timed flow-create requests */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_REQ_IGNORED,

    /** Number of timed flow-create acknowledgements received */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_ACK_RECV,

    /** Number of failed timed flow-create acknowledgements */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_ACK_FAILED,

    /** Number of ignored timed flow-create acknowledgements */
    SAI_ENI_STAT_TIMED_FLOW_CREATE_ACK_IGNORED,

    /** Number of inline flow-update requests sent */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_REQ_SENT,

    /** Number of inline flow-update requests received */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_REQ_RECV,

    /** Number of failed inline flow-update requests */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_REQ_FAILED,

    /** Number of ignored inline flow-update requests */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_REQ_IGNORED,

    /** Number of inline flow-update acknowledgements received */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_ACK_RECV,

    /** Number of failed inline flow-update acknowledgements */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_ACK_FAILED,

    /** Number of ignored inline flow-update acknowledgements */
    SAI_ENI_STAT_INLINE_FLOW_UPDATE_ACK_IGNORED,

    /** Number of timed flow-update requests sent */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_REQ_SENT,

    /** Number of timed flow-update requests received */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_REQ_RECV,

    /** Number of failed timed flow-update requests */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_REQ_FAILED,

    /** Number of ignored timed flow-update requests */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_REQ_IGNORED,

    /** Number of timed flow-update acknowledgements received */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_ACK_RECV,

    /** Number of failed timed flow-update acknowledgements */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_ACK_FAILED,

    /** Number of ignored timed flow-update acknowledgements */
    SAI_ENI_STAT_TIMED_FLOW_UPDATE_ACK_IGNORED,

    /** Number of inline flow-delete requests sent */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_REQ_SENT,

    /** Number of inline flow-delete requests received */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_REQ_RECV,

    /** Number of failed inline flow-delete requests */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_REQ_FAILED,

    /** Number of ignored inline flow-delete requests */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_REQ_IGNORED,

    /** Number of inline flow-delete acknowledgements received */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_ACK_RECV,

    /** Number of failed inline flow-delete acknowledgements */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_ACK_FAILED,

    /** Number of ignored inline flow-delete acknowledgements */
    SAI_ENI_STAT_INLINE_FLOW_DELETE_ACK_IGNORED,

    /** Number of timed flow-delete requests sent */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_REQ_SENT,

    /** Number of timed flow-delete requests received */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_REQ_RECV,

    /** Number of failed timed flow-delete requests */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_REQ_FAILED,

    /** Number of ignored timed flow-delete requests */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_REQ_IGNORED,

    /** Number of timed flow-delete acknowledgements received */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_ACK_RECV,

    /** Number of failed timed flow-delete acknowledgements */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_ACK_FAILED,

    /** Number of ignored timed flow-delete acknowledgements */
    SAI_ENI_STAT_TIMED_FLOW_DELETE_ACK_IGNORED,

    /** Number of packets dropped due to outbound routing entry miss */
    SAI_ENI_STAT_OUTBOUND_ROUTING_ENTRY_MISS_DROP_PACKETS,

    /** Number of packets dropped due to outbound CA-PA entry miss */
    SAI_ENI_STAT_OUTBOUND_CA_PA_ENTRY_MISS_DROP_PACKETS,

    /** Number of packets dropped due to inbound routing entry miss */
    SAI_ENI_STAT_INBOUND_ROUTING_ENTRY_MISS_DROP_PACKETS,

    /** Number of packets dropped due to outbound routing group miss */
    SAI_ENI_STAT_OUTBOUND_ROUTING_GROUP_MISS_DROP_PACKETS,

    /** Number of packets dropped due to outbound routing group disabled */
    SAI_ENI_STAT_OUTBOUND_ROUTING_GROUP_DISABLED_DROP_PACKETS,

    /** Number of packets dropped due to outbound port map miss */
    SAI_ENI_STAT_OUTBOUND_PORT_MAP_MISS_DROP_PACKETS,

    /** Number of packets dropped due to outbound port-map port-range entry miss */
    SAI_ENI_STAT_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY_MISS_DROP_PACKETS,

    /** Number of packets dropped due to ENI trusted VNI entry miss */
    SAI_ENI_STAT_ENI_TRUSTED_VNI_ENTRY_MISS_DROP_PACKETS,

    /** Number of TCP SYN packets received for inbound session */
    SAI_ENI_STAT_INBOUND_TCP_SYN_PACKETS,

    /** Number of TCP SYN+ACK packets received for inbound session */
    SAI_ENI_STAT_INBOUND_TCP_SYNACK_PACKETS,

    /** Number of TCP FIN packets received for inbound session */
    SAI_ENI_STAT_INBOUND_TCP_FIN_PACKETS,

    /** Number of TCP reset packets received for inbound session */
    SAI_ENI_STAT_INBOUND_TCP_RST_PACKETS,

    /** Number of TCP SYN packets transmitted for outbound session */
    SAI_ENI_STAT_OUTBOUND_TCP_SYN_PACKETS,

    /** Number of TCP SYN+ACK packets transmitted for outbound session */
    SAI_ENI_STAT_OUTBOUND_TCP_SYNACK_PACKETS,

    /** Number of TCP FIN packets transmitted for outbound session */
    SAI_ENI_STAT_OUTBOUND_TCP_FIN_PACKETS,

    /** Number of TCP reset packets transmitted for outbound session */
    SAI_ENI_STAT_OUTBOUND_TCP_RST_PACKETS,

    /** Maximum inbound CPS observed on ENI (potentially since last queried, if cleared) */
    SAI_ENI_STAT_MAX_RX_CPS,

    /** Maximum outbound CPS observed on ENI (potentially since last queried, if cleared) */
    SAI_ENI_STAT_MAX_TX_CPS,

    /** Number of TCP reset packets injected to force terminate TCP sessions on idle-timeout */
    SAI_ENI_STAT_TCP_RST_INJECT_PACKETS,

    /** Number of fast path ICMP flow redirect messages dropped */
    SAI_ENI_STAT_LB_FAST_PATH_ICMP_IN_DROP_PACKETS,

    /** Number of packets dropped on source PA (tunnel-endpoint) validation failure */
    SAI_ENI_STAT_PA_VALIDATION_FAIL_DROP_PACKETS,

    /** Number of packets dropped due to forwarding errors (mapping & route-lookup misses etc) */
    SAI_ENI_STAT_FORWARDING_DROP_PACKETS,

    /** Number of packets dropped as per ENI policy/ACL */
    SAI_ENI_STAT_POLICY_DROP_PACKETS,

    /** Number of TCP non-syn packet drops due to missing flow-entry */
    SAI_ENI_STAT_TCP_NON_SYN_FLOW_MISS_DROP_PACKETS,

    /** Number of (new session) packets dropped on reaching configured ENI session-limit */
    SAI_ENI_STAT_SESSION_LIMIT_EXCEEDED_DROP_PACKETS,

    /** Number of unsupported protocol (non-TCP/UDP/ICMP) packets received from tenant */
    SAI_ENI_STAT_UNSUPPORTED_PROTOCOL_DROP_PACKETS,

    /** Number of packets dropped on exceeding Control-Plane Policer limits */
    SAI_ENI_STAT_COPP_DROP_PACKETS,

    /** Number of packets dropped due to flow-entry inconsistency */
    SAI_ENI_STAT_INCONSISTENT_FLOW_ENTRY_DROP_PACKETS,

    /** Number of packets dropped on detecting packet-loop in pipeline */
    SAI_ENI_STAT_PIPELINE_PACKET_LOOP_DROP_PACKETS,

    /** Number of packets dropped due to other (internal) reason on ENI */
    SAI_ENI_STAT_OTHER_DROP_PACKETS,

    /** Dropped packets total per ENI */
    SAI_ENI_STAT_TOTAL_DROP_PACKETS,

    /** Number of inline sync requests sent to HA peer */
    SAI_ENI_STAT_INLINE_SYNC_REQ_TX,

    /** Number of inline sync acknowledgements sent to HA peer */
    SAI_ENI_STAT_INLINE_SYNC_ACK_TX,

    /** Number of inline sync redirect packets sent to ENI-owner HA peer */
    SAI_ENI_STAT_INLINE_SYNC_REDIRECT_PACKETS_TX,

    /** Number of inline sync requests received from HA peer */
    SAI_ENI_STAT_INLINE_SYNC_REQ_RX,

    /** Number of inline sync acknowledgements received from HA peer */
    SAI_ENI_STAT_INLINE_SYNC_ACK_RX,

    /** Number of inline sync redirect packets received by ENI-owner from HA peer */
    SAI_ENI_STAT_INLINE_SYNC_REDIRECT_PACKETS_RX,
} sai_eni_stat_t;

/**
 * @brief Create ENI ether address map entry
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
 * @brief Remove ENI ether address map entry
 *
 * @param[in] eni_ether_address_map_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_eni_ether_address_map_entry_fn)(
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry);

/**
 * @brief Set attribute for ENI ether address map entry
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
 * @brief Get attribute for ENI ether address map entry
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
 * @brief Bulk create ENI ether address map entry
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
 * @brief Bulk remove ENI ether address map entry
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
 * @brief Create ENI
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
 * @brief Remove ENI
 *
 * @param[in] eni_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_eni_fn)(
        _In_ sai_object_id_t eni_id);

/**
 * @brief Set attribute for ENI
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
 * @brief Get attribute for ENI
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
