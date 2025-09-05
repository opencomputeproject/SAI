/**
 * Copyright (c) 2018 Microsoft Open Technologies, Inc.
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
 * @file    saitypesextensions.h
 *
 * @brief   This module defines type extensions of the Switch Abstraction Interface (SAI)
 */

#ifndef __SAITYPESEXTENSIONS_H_
#define __SAITYPESEXTENSIONS_H_

#include <saitypes.h>

/**
 * @brief SAI object type extensions
 *
 * @flags free
 */
typedef enum _sai_object_type_extensions_t
{
    SAI_OBJECT_TYPE_EXTENSIONS_RANGE_START = SAI_OBJECT_TYPE_EXTENSIONS_RANGE_BASE,

    SAI_OBJECT_TYPE_TABLE_BITMAP_CLASSIFICATION_ENTRY = SAI_OBJECT_TYPE_EXTENSIONS_RANGE_START,

    SAI_OBJECT_TYPE_TABLE_BITMAP_ROUTER_ENTRY,

    SAI_OBJECT_TYPE_TABLE_META_TUNNEL_ENTRY,

    SAI_OBJECT_TYPE_DASH_ACL_GROUP,

    SAI_OBJECT_TYPE_DASH_ACL_RULE,

    SAI_OBJECT_TYPE_DIRECTION_LOOKUP_ENTRY,

    SAI_OBJECT_TYPE_ENI_ETHER_ADDRESS_MAP_ENTRY,

    SAI_OBJECT_TYPE_ENI,

    SAI_OBJECT_TYPE_INBOUND_ROUTING_ENTRY,

    SAI_OBJECT_TYPE_METER_BUCKET_ENTRY,

    SAI_OBJECT_TYPE_METER_POLICY,

    SAI_OBJECT_TYPE_METER_RULE,

    SAI_OBJECT_TYPE_OUTBOUND_CA_TO_PA_ENTRY,

    SAI_OBJECT_TYPE_OUTBOUND_ROUTING_ENTRY,

    SAI_OBJECT_TYPE_VNET,

    SAI_OBJECT_TYPE_PA_VALIDATION_ENTRY,

    SAI_OBJECT_TYPE_VIP_ENTRY,

    SAI_OBJECT_TYPE_HA_SET,

    SAI_OBJECT_TYPE_HA_SCOPE,

    SAI_OBJECT_TYPE_DASH_TUNNEL,

    SAI_OBJECT_TYPE_OUTBOUND_ROUTING_GROUP,

    SAI_OBJECT_TYPE_FLOW_TABLE,

    SAI_OBJECT_TYPE_FLOW_ENTRY,

    SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION_FILTER,

    SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION,

    SAI_OBJECT_TYPE_DASH_APPLIANCE,

    SAI_OBJECT_TYPE_DASH_TUNNEL_MEMBER,

    SAI_OBJECT_TYPE_DASH_TUNNEL_NEXT_HOP,

    SAI_OBJECT_TYPE_OUTBOUND_PORT_MAP,

    SAI_OBJECT_TYPE_OUTBOUND_PORT_MAP_PORT_RANGE_ENTRY,

    SAI_OBJECT_TYPE_GLOBAL_TRUSTED_VNI_ENTRY,

    SAI_OBJECT_TYPE_ENI_TRUSTED_VNI_ENTRY,

    /**
     * @brief OTN Extensions
     */
    SAI_OBJECT_TYPE_OTN_ATTENUATOR,

    SAI_OBJECT_TYPE_OTN_OA,

    /* Add new experimental object types above this line */

    SAI_OBJECT_TYPE_EXTENSIONS_RANGE_END

} sai_object_type_extensions_t;

typedef enum _sai_dash_direction_t
{
    SAI_DASH_DIRECTION_INVALID,

    SAI_DASH_DIRECTION_OUTBOUND,

    SAI_DASH_DIRECTION_INBOUND,

} sai_dash_direction_t;

typedef enum _sai_dash_encapsulation_t
{
    SAI_DASH_ENCAPSULATION_INVALID,

    SAI_DASH_ENCAPSULATION_VXLAN,

    SAI_DASH_ENCAPSULATION_NVGRE,

} sai_dash_encapsulation_t;

typedef enum _sai_dash_tunnel_dscp_mode_t
{
    SAI_DASH_TUNNEL_DSCP_MODE_PRESERVE_MODEL,

    SAI_DASH_TUNNEL_DSCP_MODE_PIPE_MODEL,

} sai_dash_tunnel_dscp_mode_t;

/**
 * @brief Defines a list of enums for dash_routing_actions
 *
 * @flags strict
 */
typedef enum _sai_dash_routing_actions_t
{
    SAI_DASH_ROUTING_ACTIONS_STATIC_ENCAP = 1,

    SAI_DASH_ROUTING_ACTIONS_NAT = 2,

    SAI_DASH_ROUTING_ACTIONS_NAT46 = 4,

    SAI_DASH_ROUTING_ACTIONS_NAT64 = 8,

    SAI_DASH_ROUTING_ACTIONS_NAT_PORT = 16,

} sai_dash_routing_actions_t;

typedef enum _sai_dash_ha_role_t
{
    SAI_DASH_HA_ROLE_DEAD,

    SAI_DASH_HA_ROLE_ACTIVE,

    SAI_DASH_HA_ROLE_STANDBY,

    SAI_DASH_HA_ROLE_STANDALONE,

    SAI_DASH_HA_ROLE_SWITCHING_TO_ACTIVE,

} sai_dash_ha_role_t;

/**
 * @brief Defines a list of enums for dash_flow_enabled_key
 *
 * @flags strict
 */
typedef enum _sai_dash_flow_enabled_key_t
{
    SAI_DASH_FLOW_ENABLED_KEY_ENI_MAC = 1,

    SAI_DASH_FLOW_ENABLED_KEY_VNI = 2,

    SAI_DASH_FLOW_ENABLED_KEY_PROTOCOL = 4,

    SAI_DASH_FLOW_ENABLED_KEY_SRC_IP = 8,

    SAI_DASH_FLOW_ENABLED_KEY_DST_IP = 16,

    SAI_DASH_FLOW_ENABLED_KEY_SRC_PORT = 32,

    SAI_DASH_FLOW_ENABLED_KEY_DST_PORT = 64,

} sai_dash_flow_enabled_key_t;

/**
 * @brief Defines a list of enums for dash_flow_action
 *
 * @flags strict
 */
typedef enum _sai_dash_flow_action_t
{
    SAI_DASH_FLOW_ACTION_NONE = 0,

    SAI_DASH_FLOW_ACTION_ENCAP_U0 = 1 << 0,

    SAI_DASH_FLOW_ACTION_ENCAP_U1 = 1 << 1,

    SAI_DASH_FLOW_ACTION_SET_SMAC = 1 << 2,

    SAI_DASH_FLOW_ACTION_SET_DMAC = 1 << 3,

    SAI_DASH_FLOW_ACTION_SNAT = 1 << 4,

    SAI_DASH_FLOW_ACTION_DNAT = 1 << 5,

    SAI_DASH_FLOW_ACTION_NAT46 = 1 << 6,

    SAI_DASH_FLOW_ACTION_NAT64 = 1 << 7,

    SAI_DASH_FLOW_ACTION_SNAT_PORT = 1 << 8,

    SAI_DASH_FLOW_ACTION_DNAT_PORT = 1 << 9,

} sai_dash_flow_action_t;

/**
 * @brief Defines a list of enums for dash_ha_state
 */
typedef enum _sai_dash_ha_state_t
{
    SAI_DASH_HA_STATE_DEAD,

    SAI_DASH_HA_STATE_CONNECTING,

    SAI_DASH_HA_STATE_CONNECTED,

    SAI_DASH_HA_STATE_INITIALIZING_TO_ACTIVE,

    SAI_DASH_HA_STATE_INITIALIZING_TO_STANDBY,

    SAI_DASH_HA_STATE_PENDING_STANDALONE_ACTIVATION,

    SAI_DASH_HA_STATE_PENDING_ACTIVE_ACTIVATION,

    SAI_DASH_HA_STATE_PENDING_STANDBY_ACTIVATION,

    SAI_DASH_HA_STATE_STANDALONE,

    SAI_DASH_HA_STATE_ACTIVE,

    SAI_DASH_HA_STATE_STANDBY,

    SAI_DASH_HA_STATE_DESTROYING,

    SAI_DASH_HA_STATE_SWITCHING_TO_STANDALONE,

} sai_dash_ha_state_t;

/**
 * @brief Defines a list of enums for dash_flow_entry_bulk_get_session_op_key
 */
typedef enum _sai_dash_flow_entry_bulk_get_session_op_key_t
{
    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_OP_KEY_FILTER_OP_INVALID,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_OP_KEY_FILTER_OP_EQUAL_TO,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_OP_KEY_FILTER_OP_GREATER_THAN,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_OP_KEY_FILTER_OP_GREATER_THAN_OR_EQUAL_TO,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_OP_KEY_FILTER_OP_LESS_THAN,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_OP_KEY_FILTER_OP_LESS_THAN_OR_EQUAL_TO,

} sai_dash_flow_entry_bulk_get_session_op_key_t;

/**
 * @brief Defines a list of enums for dash_flow_entry_bulk_get_session_mode
 */
typedef enum _sai_dash_flow_entry_bulk_get_session_mode_t
{
    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_MODE_SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_MODE_GRPC,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_MODE_SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_MODE_VENDOR,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_MODE_SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_MODE_EVENT,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_MODE_SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_MODE_EVENT_WITHOUT_FLOW_STATE,

} sai_dash_flow_entry_bulk_get_session_mode_t;

/**
 * @brief Defines a list of enums for dash_flow_entry_bulk_get_session_filter_key
 */
typedef enum _sai_dash_flow_entry_bulk_get_session_filter_key_t
{
    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_KEY_INVAILD,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_KEY_FLOW_TABLE_ID,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_KEY_ENI_ADDR,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_KEY_IP_PROTOCOL,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_KEY_SRC_IP_ADDR,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_KEY_DST_IP_ADDR,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_KEY_SRC_L4_PORT,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_KEY_DST_L4_PORT,

    SAI_DASH_FLOW_ENTRY_BULK_GET_SESSION_FILTER_KEY_KEY_VERSION,

} sai_dash_flow_entry_bulk_get_session_filter_key_t;

/**
 * @brief Defines a list of enums for dash_eni_mac_override_type
 */
typedef enum _sai_dash_eni_mac_override_type_t
{
    SAI_DASH_ENI_MAC_OVERRIDE_TYPE_NONE,

    SAI_DASH_ENI_MAC_OVERRIDE_TYPE_SRC_MAC,

    SAI_DASH_ENI_MAC_OVERRIDE_TYPE_DST_MAC,

} sai_dash_eni_mac_override_type_t;

/**
 * @brief Defines a list of enums for dash_flow_sync_state
 */
typedef enum _sai_dash_flow_sync_state_t
{
    SAI_DASH_FLOW_SYNC_STATE_FLOW_MISS,

    SAI_DASH_FLOW_SYNC_STATE_FLOW_CREATED,

    SAI_DASH_FLOW_SYNC_STATE_FLOW_SYNCED,

    SAI_DASH_FLOW_SYNC_STATE_FLOW_PENDING_DELETE,

    SAI_DASH_FLOW_SYNC_STATE_FLOW_PENDING_RESIMULATION,

} sai_dash_flow_sync_state_t;

/**
 * @brief Defines a list of enums for dash_eni_mode
 */
typedef enum _sai_dash_eni_mode_t
{
    SAI_DASH_ENI_MODE_VM,

    SAI_DASH_ENI_MODE_FNIC,

} sai_dash_eni_mode_t;

#endif /* __SAITYPESEXTENSIONS_H_ */

