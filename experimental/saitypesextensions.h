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

    SAI_OBJECT_TYPE_METER_BUCKET,

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

#endif /* __SAITYPESEXTENSIONS_H_ */

