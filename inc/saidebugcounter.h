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
 * @file    saidebugcounter.h
 *
 * @brief   This module defines SAI Debug Counter interface
 *
 * @par Abstract
 *
 *    This module defines SAI Debug Counter API.
 */

#if !defined (__SAIDEBUGCOUNTER_H_)
#define __SAIDEBUGCOUNTER_H_

#include <saitypes.h>

/**
 * @defgroup SAIDEBUGCOUNTER SAI - Debug counter specific API definitions
 *
 * @{
 */

/**
 * @brief Debug counter type
 */
typedef enum _sai_debug_counter_type_t
{
    /** Port in drop reasons. Base object: SAI_OBJECT_TYPE_PORT */
    SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,

    /** Port out drop reasons. Base object: SAI_OBJECT_TYPE_PORT */
    SAI_DEBUG_COUNTER_TYPE_PORT_OUT_DROP_REASONS,

    /**
     * @brief Switch in drop reasons
     *
     * Base object: SAI_OBJECT_TYPE_SWITCH.
     * Values for all ports in the switch are summed up by switch counter
     */
    SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS,

    /**
     * @brief Switch out drop reasons
     *
     * Base object: SAI_OBJECT_TYPE_SWITCH.
     * Values for all ports in the switch are summed up by switch counter
     */
    SAI_DEBUG_COUNTER_TYPE_SWITCH_OUT_DROP_REASONS,

} sai_debug_counter_type_t;

/**
 * @brief Debug counter bind method
 */
typedef enum _sai_debug_counter_bind_method_t
{
    /** Bind automatically to all instances of base object */
    SAI_DEBUG_COUNTER_BIND_METHOD_AUTOMATIC,

} sai_debug_counter_bind_method_t;

/**
 * @brief Attribute data for in drop reasons
 */
typedef enum _sai_in_drop_reason_t
{
    /** Start of in drop reasons */
    SAI_IN_DROP_REASON_START,

    /* L2 reasons */

    /** Any L2 pipeline drop */
    SAI_IN_DROP_REASON_L2_ANY = SAI_IN_DROP_REASON_START,

    /** Source MAC is multicast */
    SAI_IN_DROP_REASON_SMAC_MULTICAST,

    /** Source MAC equals destination MAC */
    SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC,

    /** Destination MAC is Reserved (Destination MAC=01-80-C2-00-00-0x) */
    SAI_IN_DROP_REASON_DMAC_RESERVED,

    /**
     * @brief VLAN tag not allowed
     *
     * Frame tagged when port is dropping tagged,
     * or untagged when dropping untagged
     */
    SAI_IN_DROP_REASON_VLAN_TAG_NOT_ALLOWED,

    /** Ingress VLAN filter */
    SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER,

    /** Ingress STP filter */
    SAI_IN_DROP_REASON_INGRESS_STP_FILTER,

    /** Unicast FDB table action discard */
    SAI_IN_DROP_REASON_FDB_UC_DISCARD,

    /** Multicast FDB table empty tx list */
    SAI_IN_DROP_REASON_FDB_MC_DISCARD,

    /** Port L2 loopback filter (packet egressing on the same port+VLAN as ingressing) */
    SAI_IN_DROP_REASON_L2_LOOPBACK_FILTER,

    /** Packet size is larger than the L2 (Port) MTU */
    SAI_IN_DROP_REASON_EXCEEDS_L2_MTU,

    /* L3 reasons */

    /** Any L3 pipeline drop */
    SAI_IN_DROP_REASON_L3_ANY,

    /** Packet size is larger than the L3 (Router Interface) MTU */
    SAI_IN_DROP_REASON_EXCEEDS_L3_MTU,

    /** TTL expired */
    SAI_IN_DROP_REASON_TTL,

    /** RIF L3 loopback filter (packet egressing on the same RIF as ingressing) */
    SAI_IN_DROP_REASON_L3_LOOPBACK_FILTER,

    /**
     * @brief Non routable packet
     *
     * IGMP v1 v2 v3 membership query
     * IGMP v1 membership report
     * IGMP v2 membership report
     * IGMP v2 leave group
     * IGMP v3 membership report
     */
    SAI_IN_DROP_REASON_NON_ROUTABLE,

    /** Destination MAC is the router MAC, however packet is not routable (isn't IP or MPLS) */
    SAI_IN_DROP_REASON_NO_L3_HEADER,

    /**
     * @brief IP Header error
     *
     * Due to header checksum or bad IP version or IPv4 IHL too short
     */
    SAI_IN_DROP_REASON_IP_HEADER_ERROR,

    /** Unicast destination IP with non unicast (multicast or broadcast) destination MAC */
    SAI_IN_DROP_REASON_UC_DIP_MC_DMAC,

    /**
     * @brief Destination IP is loopback address
     *
     * for IPv4: Destination IP=127.0.0.0/8
     * for IPv6: Destination IP=::1/128 OR Destination IP=0:0:0:0:0:ffff:7f00:0/104
     */
    SAI_IN_DROP_REASON_DIP_LOOPBACK,

    /**
     * @brief Source IP is loopback address
     *
     * for IPv4: Source IP=127.0.0.0/8
     * for IPv6: Source IP=::1/128
     */
    SAI_IN_DROP_REASON_SIP_LOOPBACK,

    /**
     * @brief Source IP is multicast address
     *
     * for IPv4: Source IP=224.0.0.0/4
     * for IPv6: Source IP=FF00::/8
     */
    SAI_IN_DROP_REASON_SIP_MC,

    /**
     * @brief Source IP is in class E
     *
     * IPv4 AND Source IP=240.0.0.0/4 AND Source IP!=255.255.255.255
     */
    SAI_IN_DROP_REASON_SIP_CLASS_E,

    /**
     * @brief Source IP unspecified
     *
     * for IPv4: Source IP=0.0.0.0/32
     * for IPv6: Source IP=::0
     */
    SAI_IN_DROP_REASON_SIP_UNSPECIFIED,

    /**
     * @brief Destination IP is multicast but destination MAC isn't
     *
     * Destination IP is multicast AND
     * for IPv4: Destination MAC!={01-00-5E-0 (25 bits), dip[22:0]}
     * for IPv6: Destination MAC!={33-33, DIP[31:0]}
     */
    SAI_IN_DROP_REASON_MC_DMAC_MISMATCH,

    /** Source IP equals destination IP */
    SAI_IN_DROP_REASON_SIP_EQUALS_DIP,

    /** IPv4 source IP is limited broadcast (Source IP=255.255.255.255) */
    SAI_IN_DROP_REASON_SIP_BC,

    /** IPv4 destination IP is local network (Destination IP=0.0.0.0/8) */
    SAI_IN_DROP_REASON_DIP_LOCAL,

    /** IPv4 unicast destination IP is link local (Destination IP=169.254.0.0/16) */
    SAI_IN_DROP_REASON_DIP_LINK_LOCAL,

    /** IPv4 Source IP is link local (Source IP=169.254.0.0/16) */
    SAI_IN_DROP_REASON_SIP_LINK_LOCAL,

    /** IPv6 destination in multicast scope 0 reserved (Destination IP=ff:x0:/16) */
    SAI_IN_DROP_REASON_IPV6_MC_SCOPE0,

    /** IPv6 destination in multicast scope 1 interface-local (Destination IP=ff:x1:/16) */
    SAI_IN_DROP_REASON_IPV6_MC_SCOPE1,

    /** Ingress RIF is disabled */
    SAI_IN_DROP_REASON_IRIF_DISABLED,

    /** Egress RIF is disabled */
    SAI_IN_DROP_REASON_ERIF_DISABLED,

    /** IPv4 Routing table (LPM) unicast miss */
    SAI_IN_DROP_REASON_LPM4_MISS,

    /** IPv6 Routing table (LPM) unicast miss */
    SAI_IN_DROP_REASON_LPM6_MISS,

    /** Black hole route (discard by route entry) */
    SAI_IN_DROP_REASON_BLACKHOLE_ROUTE,

    /** Black hole ARP/Neighbor (discard by ARP or neighbor entries) */
    SAI_IN_DROP_REASON_BLACKHOLE_ARP,

    /** Unresolved next hop (missing ARP entry) */
    SAI_IN_DROP_REASON_UNRESOLVED_NEXT_HOP,

    /**
     * @brief Packet is destined for neighboring device but neighbor device link is down
     *
     * Counted on ingress link
     */
    SAI_IN_DROP_REASON_L3_EGRESS_LINK_DOWN,

    /* Tunnel reasons */

    /**
     * @brief Packet decapsulation failed
     *
     * e.g.: need to decap too many bytes, remaining packet is too short, UDP port out of defined range
     */
    SAI_IN_DROP_REASON_DECAP_ERROR,

    /* ACL reasons */

    /** Packet is dropped due to configured ACL rules, all stages/bind points combinations */
    SAI_IN_DROP_REASON_ACL_ANY,

    /** Packet is dropped due to configured ACL rules, ingress stage, port binding */
    SAI_IN_DROP_REASON_ACL_INGRESS_PORT,

    /** Packet is dropped due to configured ACL rules, ingress stage, LAG binding */
    SAI_IN_DROP_REASON_ACL_INGRESS_LAG,

    /** Packet is dropped due to configured ACL rules, ingress stage, VLAN binding */
    SAI_IN_DROP_REASON_ACL_INGRESS_VLAN,

    /** Packet is dropped due to configured ACL rules, ingress stage, RIF binding */
    SAI_IN_DROP_REASON_ACL_INGRESS_RIF,

    /** Packet is dropped due to configured ACL rules, ingress stage, switch binding */
    SAI_IN_DROP_REASON_ACL_INGRESS_SWITCH,

    /** Packet is dropped due to configured ACL rules, egress stage, port binding */
    SAI_IN_DROP_REASON_ACL_EGRESS_PORT,

    /** Packet is dropped due to configured ACL rules, egress stage, LAG binding */
    SAI_IN_DROP_REASON_ACL_EGRESS_LAG,

    /** Packet is dropped due to configured ACL rules, egress stage, VLAN binding */
    SAI_IN_DROP_REASON_ACL_EGRESS_VLAN,

    /** Packet is dropped due to configured ACL rules, egress stage, RIF binding */
    SAI_IN_DROP_REASON_ACL_EGRESS_RIF,

    /** Packet is dropped due to configured ACL rules, egress stage, switch binding */
    SAI_IN_DROP_REASON_ACL_EGRESS_SWITCH,

    /* Reasons added in 1.6 */

    /** Packet is dropped due to FDB action discards or route discards (black hole route) */
    SAI_IN_DROP_REASON_FDB_AND_BLACKHOLE_DISCARDS,

    /** MPLS Routing table lookup miss */
    SAI_IN_DROP_REASON_MPLS_MISS,

    /**
     * @brief SRV6 local SID drop
     *
     * Packet is dropped due to local SID configuration or incorrect value in SRV6 packet header
     * e.g.: Local SID packet action is set to SAI_PACKET_ACTION_DROP
     * Next Header is SRH and Segments Left value is 0 for End, End.X, End.T, End.B* endpoint types
     * Next Header is not SRH while local SID is configured for packet DA
     * Segments Left value is not 0 when received packet is destined to S and S is a local SID of type End.D*
     */
    SAI_IN_DROP_REASON_SRV6_LOCAL_SID_DROP,

    /** Source MAC address is zero **/ 
    SAI_IN_DROP_REASON_SMAC_ZERO,
    
    /** Destination MAC address is zero **/ 
    SAI_IN_DROP_REASON_DMAC_ZERO,

    /** IPv4 or IPv6 Routing table (LPM) unicast miss */
    SAI_IN_DROP_REASON_LPM_MISS,
    
    /** End of in drop reasons */
    SAI_IN_DROP_REASON_END,

    /** Custom range base value */
    SAI_IN_DROP_REASON_CUSTOM_RANGE_BASE = 0x10000000,

    /** End of custom range */
    SAI_IN_DROP_REASON_CUSTOM_RANGE_END

} sai_in_drop_reason_t;

/**
 * @brief Attribute data for out drop reasons
 */
typedef enum _sai_out_drop_reason_t
{
    /** Start of out drop reasons */
    SAI_OUT_DROP_REASON_START,

    /* L2 reasons */

    /** Any L2 pipeline drop */
    SAI_OUT_DROP_REASON_L2_ANY = SAI_OUT_DROP_REASON_START,

    /** Egress VLAN filter */
    SAI_OUT_DROP_REASON_EGRESS_VLAN_FILTER,

    /* L3 reasons */

    /** Any L3 pipeline drop */
    SAI_OUT_DROP_REASON_L3_ANY,

    /**
     * @brief Packet is destined for neighboring device but neighbor device link is down
     *
     * Counted on egress link
     */
    SAI_OUT_DROP_REASON_L3_EGRESS_LINK_DOWN,

    /* Tunnel reasons */

    /**
     * @brief Tunnel packets dropped if going back to the incoming tunnel
     */
    SAI_OUT_DROP_REASON_TUNNEL_LOOPBACK_PACKET_DROP,

    /** End of out drop reasons */
    SAI_OUT_DROP_REASON_END,

    /** Custom range base value */
    SAI_OUT_DROP_REASON_CUSTOM_RANGE_BASE = 0x10000000,

    /** End of custom range */
    SAI_OUT_DROP_REASON_CUSTOM_RANGE_END

} sai_out_drop_reason_t;

/**
 * @brief Attribute Id in sai_set_counter_attribute() and
 * sai_get_counter_attribute() calls
 */
typedef enum _sai_debug_counter_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DEBUG_COUNTER_ATTR_START,

    /* READ-ONLY */

    /**
     * @brief Object stat index
     * Index is added to base start
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_DEBUG_COUNTER_ATTR_INDEX = SAI_DEBUG_COUNTER_ATTR_START,

    /* READ-WRITE */

    /**
     * @brief Debug counter type
     *
     * @type sai_debug_counter_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isresourcetype true
     */
    SAI_DEBUG_COUNTER_ATTR_TYPE,

    /**
     * @brief Bind method to base object
     *
     * @type sai_debug_counter_bind_method_t
     * @flags CREATE_ONLY
     * @default SAI_DEBUG_COUNTER_BIND_METHOD_AUTOMATIC
     */
    SAI_DEBUG_COUNTER_ATTR_BIND_METHOD,

    /**
     * @brief List of in drop reasons that will be counted
     *
     * @type sai_s32_list_t sai_in_drop_reason_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_DEBUG_COUNTER_ATTR_TYPE == SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS or SAI_DEBUG_COUNTER_ATTR_TYPE == SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS
     */
    SAI_DEBUG_COUNTER_ATTR_IN_DROP_REASON_LIST,

    /**
     * @brief List of out drop reasons that will be counted
     *
     * @type sai_s32_list_t sai_out_drop_reason_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_DEBUG_COUNTER_ATTR_TYPE == SAI_DEBUG_COUNTER_TYPE_PORT_OUT_DROP_REASONS or SAI_DEBUG_COUNTER_ATTR_TYPE == SAI_DEBUG_COUNTER_TYPE_SWITCH_OUT_DROP_REASONS
     */
    SAI_DEBUG_COUNTER_ATTR_OUT_DROP_REASON_LIST,

    /**
     * @brief End of attributes
     */
    SAI_DEBUG_COUNTER_ATTR_END,

    /** Custom range base value */
    SAI_DEBUG_COUNTER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DEBUG_COUNTER_ATTR_CUSTOM_RANGE_END

} sai_debug_counter_attr_t;

/**
 * @brief Create debug counter
 *
 * @param[out] debug_counter_id Debug counter id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success
 */
typedef sai_status_t (*sai_create_debug_counter_fn)(
        _Out_ sai_object_id_t *debug_counter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove debug counter
 *
 * @param[in] debug_counter_id Debug counter id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_debug_counter_fn)(
        _In_ sai_object_id_t debug_counter_id);

/**
 * @brief Set debug counter attribute Value
 *
 * @param[in] debug_counter_id Debug counter id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_debug_counter_attribute_fn)(
        _In_ sai_object_id_t debug_counter_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get debug counter attribute Value
 *
 * @param[in] debug_counter_id Debug counter id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_debug_counter_attribute_fn)(
        _In_ sai_object_id_t debug_counter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Counter methods table retrieved with sai_api_query()
 */
typedef struct _sai_debug_counter_api_t
{
    sai_create_debug_counter_fn        create_debug_counter;
    sai_remove_debug_counter_fn        remove_debug_counter;
    sai_set_debug_counter_attribute_fn set_debug_counter_attribute;
    sai_get_debug_counter_attribute_fn get_debug_counter_attribute;

} sai_debug_counter_api_t;

/**
 * @}
 */
#endif /** __SAIDEBUGCOUNTER_H_ */
