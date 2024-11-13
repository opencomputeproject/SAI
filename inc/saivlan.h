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
 * @file    saivlan.h
 *
 * @brief   This module defines SAI VLAN interface
 */

#if !defined (__SAIVLAN_H_)
#define __SAIVLAN_H_

#include <saitypes.h>

/**
 * @defgroup SAIVLAN SAI - VLAN specific API definitions
 *
 * @{
 */

/**
 * @def SAI_VLAN_COUNTER_SET_DEFAULT
 */
#define SAI_VLAN_COUNTER_SET_DEFAULT 0

/**
 * @brief Attribute data for tagging_mode parameter
 */
typedef enum _sai_vlan_tagging_mode_t
{
    SAI_VLAN_TAGGING_MODE_UNTAGGED,

    SAI_VLAN_TAGGING_MODE_TAGGED,

    SAI_VLAN_TAGGING_MODE_PRIORITY_TAGGED

} sai_vlan_tagging_mode_t;

/**
 * @brief Attribute data for multicast_lookup_key_type parameter
 */
typedef enum _sai_vlan_mcast_lookup_key_type_t
{
    SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_MAC_DA,

    SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_XG,

    SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_SG,

    SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_XG_AND_SG

} sai_vlan_mcast_lookup_key_type_t;

/**
 * @brief Attribute data for unknown unicast, unknown multicast
 * and broadcast flood controls
 */
typedef enum _sai_vlan_flood_control_type_t
{
    /**
     * @brief Flood on all vlan members
     *
     * When setting all to broadcast or unknown multicast flood, it also includes
     * flooding to the router. When setting all to unknown unicast flood, it does
     * not include flooding to the router
     */
    SAI_VLAN_FLOOD_CONTROL_TYPE_ALL,

    /** Disable flooding */
    SAI_VLAN_FLOOD_CONTROL_TYPE_NONE,

    /** Flood on the L2MC group */
    SAI_VLAN_FLOOD_CONTROL_TYPE_L2MC_GROUP,

    /**
     * @brief Flood on all vlan members and L2MC group
     *
     * Flood on all vlan members, without the router
     * In addition, flood on the supplied L2MC group
     */
    SAI_VLAN_FLOOD_CONTROL_TYPE_COMBINED

} sai_vlan_flood_control_type_t;

/**
 * @brief Attribute Id in sai_set_vlan_attribute() and
 * sai_get_vlan_attribute() calls
 */
typedef enum _sai_vlan_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_VLAN_ATTR_START,

    /**
     * @brief Vlan Id
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     * @isvlan true
     */
    SAI_VLAN_ATTR_VLAN_ID = SAI_VLAN_ATTR_START,

    /**
     * @brief List of vlan members in a VLAN
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_VLAN_MEMBER
     */
    SAI_VLAN_ATTR_MEMBER_LIST,

    /**
     * @brief Maximum number of learned MAC addresses
     *
     * Zero means learning limit is disabled.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES,

    /**
     * @brief STP Instance that the VLAN is associated to
     *
     * Default to default STP instance id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_STP
     * @default attrvalue SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID
     */
    SAI_VLAN_ATTR_STP_INSTANCE,

    /**
     * @brief To disable learning on a VLAN
     *
     * This should override port learn settings. If this is set to true on a
     * vlan, then the source MAC learning is disabled for this vlan on a member
     * port even if learn is enable on the port (based on port learn attribute).
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_VLAN_ATTR_LEARN_DISABLE,

    /**
     * @brief To set IPv4 multicast lookup key on a VLAN
     *
     * @type sai_vlan_mcast_lookup_key_type_t
     * @flags CREATE_AND_SET
     * @default SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_MAC_DA
     */
    SAI_VLAN_ATTR_IPV4_MCAST_LOOKUP_KEY_TYPE,

    /**
     * @brief To set IPv6 multicast lookup key on a VLAN
     *
     * @type sai_vlan_mcast_lookup_key_type_t
     * @flags CREATE_AND_SET
     * @default SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_MAC_DA
     */
    SAI_VLAN_ATTR_IPV6_MCAST_LOOKUP_KEY_TYPE,

    /**
     * @brief L2MC Group ID that unknown non-IP mcast packets forwarded to
     *
     * Indicating the output ports/LAGs for unknown non-IP multicast packets.
     * This attribute only takes effect when one of the following conditions is met:
     * (1) non-IP multicast packet
     * (2) IPv4 multicast packet && not linklocal && IPv4 mcast snooping disabled for vlan
     * (3) IPv6 multicast packet && not linklocal && IPv6 mcast snooping disabled for vlan
     * In case of SAI_NULL_OBJECT_ID, unknown multicast packets will be discarded.
     * If the group has no member, unknown multicast packets will be discarded.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_VLAN_ATTR_UNKNOWN_NON_IP_MCAST_OUTPUT_GROUP_ID,

    /**
     * @brief L2MC Group ID that unknown IPv4 mcast packets forwarded to
     *
     * Indicating the output ports/LAGs for unknown IPv4 multicast packets.
     * This attribute only takes effect when the following condition is met:
     * (1) IPv4 multicast packet && not linklocal && IPv4 mcast snooping enabled for vlan
     * In case of SAI_NULL_OBJECT_ID, unknown multicast packets will be discarded.
     * If the group has no member, unknown multicast packets will be discarded.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_VLAN_ATTR_UNKNOWN_IPV4_MCAST_OUTPUT_GROUP_ID,

    /**
     * @brief L2MC Group ID that unknown IPv6 mcast packets forwarded to
     *
     * Indicating the output ports/LAGs for unknown IPv6 multicast packets.
     * This attribute only takes effect when the following condition is met:
     * (1) IPv6 multicast packet && not linklocal && IPv6 mcast snooping enabled for vlan
     * In case of SAI_NULL_OBJECT_ID, unknown multicast packets will be discarded.
     * If the group has no member, unknown multicast packets will be discarded.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_VLAN_ATTR_UNKNOWN_IPV6_MCAST_OUTPUT_GROUP_ID,

    /**
     * @brief L2MC Group ID that unknown linklocal mcast packets forwarded to
     *
     * Indicating the output ports/LAGs for unknown linklocal multicast packets.
     * This attribute only takes effect when the following condition is met:
     * (1) IPv4 multicast packet && linklocal address && IPv4 mcast snooping enabled for vlan
     * (2) IPv6 multicast packet && linklocal address && IPv6 mcast snooping enabled for vlan
     * In case of SAI_NULL_OBJECT_ID, unknown multicast packets will be discarded.
     * If the group has no member, unknown multicast packets will be discarded.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_VLAN_ATTR_UNKNOWN_LINKLOCAL_MCAST_OUTPUT_GROUP_ID,

    /**
     * @brief VLAN bind point for ingress ACL object
     *
     * Bind (or unbind) an ingress ACL table or ACL group on a VLAN. Enable/Update
     * ingress ACL table or ACL group filtering by assigning the list of valid
     * object id. Disable ingress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_VLAN_ATTR_INGRESS_ACL,

    /**
     * @brief VLAN bind point for egress ACL object
     *
     * Bind (or unbind) an egress ACL table or ACL group on a VLAN. Enable/Update
     * egress ACL table or ACL group filtering by assigning the list of valid
     * object id. Disable egress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_VLAN_ATTR_EGRESS_ACL,

    /**
     * @brief User based Meta Data
     *
     * Value Range #SAI_SWITCH_ATTR_VLAN_USER_META_DATA_RANGE
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_VLAN_ATTR_META_DATA,

    /**
     * @brief Unknown unicast flood control type
     *
     * @type sai_vlan_flood_control_type_t
     * @flags CREATE_AND_SET
     * @default SAI_VLAN_FLOOD_CONTROL_TYPE_ALL
     */
    SAI_VLAN_ATTR_UNKNOWN_UNICAST_FLOOD_CONTROL_TYPE,

    /**
     * @brief Unknown unicast flood group.
     *
     * Provides control on the set of vlan members on which unknown unicast
     * packets need to be flooded. This attribute would be used when the
     * SAI_VLAN_ATTR_UNKNOWN_UNICAST_FLOOD_CONTROL_TYPE is set as
     * SAI_VLAN_FLOOD_CONTROL_TYPE_L2MC_GROUP or
     * SAI_VLAN_FLOOD_CONTROL_TYPE_COMBINED. When this attribute's value is
     * SAI_NULL_OBJECT_ID, then flooding would be disabled.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_VLAN_ATTR_UNKNOWN_UNICAST_FLOOD_CONTROL_TYPE == SAI_VLAN_FLOOD_CONTROL_TYPE_L2MC_GROUP or SAI_VLAN_ATTR_UNKNOWN_UNICAST_FLOOD_CONTROL_TYPE == SAI_VLAN_FLOOD_CONTROL_TYPE_COMBINED
     */
    SAI_VLAN_ATTR_UNKNOWN_UNICAST_FLOOD_GROUP,

    /**
     * @brief Unknown unicast flood control type
     *
     * @type sai_vlan_flood_control_type_t
     * @flags CREATE_AND_SET
     * @default SAI_VLAN_FLOOD_CONTROL_TYPE_ALL
     */
    SAI_VLAN_ATTR_UNKNOWN_MULTICAST_FLOOD_CONTROL_TYPE,

    /**
     * @brief Unknown multicast flood group.
     *
     * Provides control on the set of vlan members on which unknown multicast
     * packets need to be flooded. This attribute would be used when the
     * SAI_VLAN_ATTR_UNKNOWN_MULTICAST_FLOOD_CONTROL_TYPE is set as
     * SAI_VLAN_FLOOD_CONTROL_TYPE_L2MC_GROUP or
     * SAI_VLAN_FLOOD_CONTROL_TYPE_COMBINED. When this attribute's value is
     * SAI_NULL_OBJECT_ID, then flooding would be disabled.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_VLAN_ATTR_UNKNOWN_MULTICAST_FLOOD_CONTROL_TYPE == SAI_VLAN_FLOOD_CONTROL_TYPE_L2MC_GROUP or SAI_VLAN_ATTR_UNKNOWN_MULTICAST_FLOOD_CONTROL_TYPE == SAI_VLAN_FLOOD_CONTROL_TYPE_COMBINED
     */
    SAI_VLAN_ATTR_UNKNOWN_MULTICAST_FLOOD_GROUP,

    /**
     * @brief Broadcast flood control type
     *
     * @type sai_vlan_flood_control_type_t
     * @flags CREATE_AND_SET
     * @default SAI_VLAN_FLOOD_CONTROL_TYPE_ALL
     */
    SAI_VLAN_ATTR_BROADCAST_FLOOD_CONTROL_TYPE,

    /**
     * @brief Broadcast flood group.
     *
     * Provides control on the set of vlan members on which broadcast
     * packets need to be flooded. This attribute would be used when the
     * SAI_VLAN_ATTR_BROADCAST_FLOOD_CONTROL_TYPE is set as
     * SAI_VLAN_FLOOD_CONTROL_TYPE_L2MC_GROUP or
     * SAI_VLAN_FLOOD_CONTROL_TYPE_COMBINED. When this attribute's value is
     * SAI_NULL_OBJECT_ID, then flooding would be disabled.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_VLAN_ATTR_BROADCAST_FLOOD_CONTROL_TYPE == SAI_VLAN_FLOOD_CONTROL_TYPE_L2MC_GROUP or SAI_VLAN_ATTR_BROADCAST_FLOOD_CONTROL_TYPE == SAI_VLAN_FLOOD_CONTROL_TYPE_COMBINED
     */
    SAI_VLAN_ATTR_BROADCAST_FLOOD_GROUP,

    /**
     * @brief IGMP Snooping enable or disable control for VLAN
     *
     * IGMP Snooping enable control for VLAN. Default is
     * disabled
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_VLAN_ATTR_CUSTOM_IGMP_SNOOPING_ENABLE,

    /**
     * @brief Vlan bind point for TAM object
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     * @default empty
     */
    SAI_VLAN_ATTR_TAM_OBJECT,

    /**
     * @brief Set vlan statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_VLAN_ATTR_STATS_COUNT_MODE,

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
    SAI_VLAN_ATTR_SELECTIVE_COUNTER_LIST,

    /**
     * @brief End of attributes
     */
    SAI_VLAN_ATTR_END,

    /** Custom range base value */
    SAI_VLAN_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_VLAN_ATTR_CUSTOM_RANGE_END

} sai_vlan_attr_t;

/**
 * @brief List of VLAN Member Attributes
 */
typedef enum _sai_vlan_member_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_VLAN_MEMBER_ATTR_START,

    /**
     * @brief VLAN ID
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_VLAN
     */
    SAI_VLAN_MEMBER_ATTR_VLAN_ID = SAI_VLAN_MEMBER_ATTR_START,

    /**
     * @brief Bridge port ID.
     *
     * Valid only for .1Q bridge ports.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE_PORT
     */
    SAI_VLAN_MEMBER_ATTR_BRIDGE_PORT_ID,

    /**
     * @brief VLAN tagging mode
     *
     * @type sai_vlan_tagging_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_VLAN_TAGGING_MODE_UNTAGGED
     */
    SAI_VLAN_MEMBER_ATTR_VLAN_TAGGING_MODE,

    /**
     * @brief Indicates if the bridge port is set to drop the Tunnel Terminated broadcast, unknown unicast and multicast traffic
     *
     * When set to true, egress BUM traffic will be dropped
     * Valid only when the SAI_VLAN_MEMBER_ATTR_BRIDGE_PORT_ID is of type SAI_BRIDGE_PORT_TYPE_PORT.
     * Valid only for .1Q bridge ports.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_VLAN_MEMBER_ATTR_TUNNEL_TERM_BUM_TX_DROP,

    /**
     * @brief Indicates if the vlan member is set to drop the ingress traffic
     *
     * When set to true, ingress traffic will be dropped
     *
     * Valid only when the SAI_VLAN_MEMBER_ATTR_BRIDGE_PORT_ID is of type SAI_BRIDGE_PORT_TYPE_PORT.
     * Valid only for .1Q bridge ports.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_VLAN_MEMBER_ATTR_RX_DROP,

    /**
     * @brief Indicates if the vlan member is set to drop the egress traffic
     *
     * When set to true, egress traffic will be dropped
     *
     * Valid only when the SAI_VLAN_MEMBER_ATTR_BRIDGE_PORT_ID is of type SAI_BRIDGE_PORT_TYPE_PORT.
     * Valid only for .1Q bridge ports.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_VLAN_MEMBER_ATTR_TX_DROP,

    /**
     * @brief End of attributes
     */
    SAI_VLAN_MEMBER_ATTR_END,

    /** Custom range base value */
    SAI_VLAN_MEMBER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_VLAN_MEMBER_ATTR_CUSTOM_RANGE_END

} sai_vlan_member_attr_t;

/**
 * @brief VLAN counter IDs in sai_get_vlan_stats() call
 */
typedef enum _sai_vlan_stat_t
{
    SAI_VLAN_STAT_IN_OCTETS,
    SAI_VLAN_STAT_IN_PACKETS,
    SAI_VLAN_STAT_IN_UCAST_PKTS,
    SAI_VLAN_STAT_IN_NON_UCAST_PKTS,
    SAI_VLAN_STAT_IN_DISCARDS,
    SAI_VLAN_STAT_IN_ERRORS,
    SAI_VLAN_STAT_IN_UNKNOWN_PROTOS,
    SAI_VLAN_STAT_OUT_OCTETS,
    SAI_VLAN_STAT_OUT_PACKETS,
    SAI_VLAN_STAT_OUT_UCAST_PKTS,
    SAI_VLAN_STAT_OUT_NON_UCAST_PKTS,
    SAI_VLAN_STAT_OUT_DISCARDS,
    SAI_VLAN_STAT_OUT_ERRORS,
    SAI_VLAN_STAT_OUT_QLEN

} sai_vlan_stat_t;

/**
 * @brief Create a VLAN
 *
 * @param[out] vlan_id VLAN ID
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_vlan_fn)(
        _Out_ sai_object_id_t *vlan_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove VLAN
 *
 * @param[in] vlan_id VLAN member ID
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_vlan_fn)(
        _In_ sai_object_id_t vlan_id);

/**
 * @brief Set VLAN Attribute
 *
 * @param[in] vlan_id VLAN ID
 * @param[in] attr Attribute structure containing ID and value
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_vlan_attribute_fn)(
        _In_ sai_object_id_t vlan_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get VLAN Attribute
 *
 * @param[in] vlan_id VLAN ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list List of attribute structures containing ID and value
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_vlan_attribute_fn)(
        _In_ sai_object_id_t vlan_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create VLAN Member
 *
 * @param[out] vlan_member_id VLAN member ID
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_vlan_member_fn)(
        _Out_ sai_object_id_t *vlan_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove VLAN Member
 *
 * @param[in] vlan_member_id VLAN member ID
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_vlan_member_fn)(
        _In_ sai_object_id_t vlan_member_id);

/**
 * @brief Set VLAN Member Attribute
 *
 * @param[in] vlan_member_id VLAN member ID
 * @param[in] attr Attribute structure containing ID and value
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_vlan_member_attribute_fn)(
        _In_ sai_object_id_t vlan_member_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get VLAN Member Attribute
 *
 * @param[in] vlan_member_id VLAN member ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list List of attribute structures containing ID and value
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_vlan_member_attribute_fn)(
        _In_ sai_object_id_t vlan_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get vlan statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] vlan_id VLAN id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_vlan_stats_fn)(
        _In_ sai_object_id_t vlan_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get vlan statistics counters extended.
 *
 * @param[in] vlan_id VLAN id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_vlan_stats_ext_fn)(
        _In_ sai_object_id_t vlan_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear vlan statistics counters.
 *
 * @param[in] vlan_id Vlan id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_vlan_stats_fn)(
        _In_ sai_object_id_t vlan_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief VLAN methods table retrieved with sai_api_query()
 */
typedef struct _sai_vlan_api_t
{
    sai_create_vlan_fn                  create_vlan;
    sai_remove_vlan_fn                  remove_vlan;
    sai_set_vlan_attribute_fn           set_vlan_attribute;
    sai_get_vlan_attribute_fn           get_vlan_attribute;
    sai_create_vlan_member_fn           create_vlan_member;
    sai_remove_vlan_member_fn           remove_vlan_member;
    sai_set_vlan_member_attribute_fn    set_vlan_member_attribute;
    sai_get_vlan_member_attribute_fn    get_vlan_member_attribute;
    sai_bulk_object_create_fn           create_vlan_members;
    sai_bulk_object_remove_fn           remove_vlan_members;
    sai_get_vlan_stats_fn               get_vlan_stats;
    sai_get_vlan_stats_ext_fn           get_vlan_stats_ext;
    sai_clear_vlan_stats_fn             clear_vlan_stats;

} sai_vlan_api_t;

/**
 * @}
 */
#endif /** __SAIVLAN_H_ */
