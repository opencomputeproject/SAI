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
 * @file    saihostif.h
 *
 * @brief   This module defines SAI host interface
 *
 * @par Abstract
 *
 *    This module defines SAI Host Interface which is responsible for
 *    creating/deleting Linux netdev corresponding to the host interface type.
 *    All the management operations of the netdevs such as changing IP address
 *    are outside the scope of SAI.
 */

#if !defined (__SAIHOSTIF_H_)
#define __SAIHOSTIF_H_

#include <saitypes.h>

/**
 * @defgroup SAIHOSTINTF SAI - Host Interface specific API definitions
 *
 * @{
 */

/**
 * @brief Defines maximum host interface name
 */
#define SAI_HOSTIF_NAME_SIZE 16

/**
 * @brief Defines maximum length of generic netlink multicast group name
 */
#define SAI_HOSTIF_GENETLINK_MCGRP_NAME_SIZE 16

/**
 * @brief Host interface trap group attributes
 */
typedef enum _sai_hostif_trap_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_HOSTIF_TRAP_GROUP_ATTR_START,

    /**
     * @brief Admin Mode
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_HOSTIF_TRAP_GROUP_ATTR_ADMIN_STATE = SAI_HOSTIF_TRAP_GROUP_ATTR_START,

    /**
     * @brief CPU egress queue
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_HOSTIF_TRAP_GROUP_ATTR_QUEUE,

    /**
     * @brief SAI policer object id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_POLICER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER,

    /**
     * @brief End of attributes
     */
    SAI_HOSTIF_TRAP_GROUP_ATTR_END,

    /** Start of custom range base */
    SAI_HOSTIF_TRAP_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range */
    SAI_HOSTIF_TRAP_GROUP_ATTR_CUSTOM_RANGE_END

} sai_hostif_trap_group_attr_t;

/**
 * @brief Create host interface trap group
 *
 * @param[out] hostif_trap_group_id Host interface trap group id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_hostif_trap_group_fn)(
        _Out_ sai_object_id_t *hostif_trap_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove host interface trap group
 *
 * @param[in] hostif_trap_group_id Host interface trap group id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_hostif_trap_group_fn)(
        _In_ sai_object_id_t hostif_trap_group_id);

/**
 * @brief Set host interface trap group attribute value.
 *
 * @param[in] hostif_trap_group_id Host interface trap group id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_hostif_trap_group_attribute_fn)(
        _In_ sai_object_id_t hostif_trap_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get host interface trap group attribute value.
 *
 * @param[in] hostif_trap_group_id Host interface trap group id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_hostif_trap_group_attribute_fn)(
        _In_ sai_object_id_t hostif_trap_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Host interface trap type
 *
 * @flags Contains flags
 */
typedef enum _sai_hostif_trap_type_t
{
    /**
     * @brief Start of trap types
     */
    SAI_HOSTIF_TRAP_TYPE_START = 0x00000000,

    /* Control plane protocol */

    /* Switch trap */

    /** Default action is drop */
    SAI_HOSTIF_TRAP_TYPE_STP = SAI_HOSTIF_TRAP_TYPE_START,

    /** Default action is drop */
    SAI_HOSTIF_TRAP_TYPE_LACP = 0x00000001,

    /** Default action is drop */
    SAI_HOSTIF_TRAP_TYPE_EAPOL = 0x00000002,

    /** Default action is drop */
    SAI_HOSTIF_TRAP_TYPE_LLDP = 0x00000003,

    /** Default action is drop */
    SAI_HOSTIF_TRAP_TYPE_PVRST = 0x00000004,

    /** Default action is forward */
    SAI_HOSTIF_TRAP_TYPE_IGMP_TYPE_QUERY = 0x00000005,

    /** Default action is forward */
    SAI_HOSTIF_TRAP_TYPE_IGMP_TYPE_LEAVE = 0x00000006,

    /** Default action is forward */
    SAI_HOSTIF_TRAP_TYPE_IGMP_TYPE_V1_REPORT = 0x00000007,

    /** Default action is forward */
    SAI_HOSTIF_TRAP_TYPE_IGMP_TYPE_V2_REPORT = 0x00000008,

    /** Default action is forward */
    SAI_HOSTIF_TRAP_TYPE_IGMP_TYPE_V3_REPORT = 0x00000009,

    /** Default action is drop */
    SAI_HOSTIF_TRAP_TYPE_SAMPLEPACKET = 0x0000000a,

    /** Default action is drop */
    SAI_HOSTIF_TRAP_TYPE_UDLD = 0x0000000b,

    /** Default action is drop */
    SAI_HOSTIF_TRAP_TYPE_CDP = 0x0000000c,

    /** Default action is drop */
    SAI_HOSTIF_TRAP_TYPE_VTP = 0x0000000d,

    /** Default action is drop */
    SAI_HOSTIF_TRAP_TYPE_DTP = 0x0000000e,

    /** Default action is drop */
    SAI_HOSTIF_TRAP_TYPE_PAGP = 0x0000000f,

    /**
     * @brief PTP traffic (EtherType = 0x88F7 or UDP dst port == 319 or UDP dst port == 320)
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_PTP = 0x00000010,

    /**
     * @brief PTP packet sent from CPU with updated TX timestamp
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_PTP_TX_EVENT = 0x00000011,

    /** Switch traps custom range start */
    SAI_HOSTIF_TRAP_TYPE_SWITCH_CUSTOM_RANGE_BASE = 0x00001000,

    /* Router traps */

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_ARP_REQUEST = 0x00002000,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_ARP_RESPONSE = 0x00002001,

    /**
     * @brief DHCP traffic (UDP ports 67, 68), either L3 broadcast or unicast
     * to local router IP address (default packet action is forward)
     */
    SAI_HOSTIF_TRAP_TYPE_DHCP = 0x00002002,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_OSPF = 0x00002003,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_PIM = 0x00002004,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_VRRP = 0x00002005,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_DHCPV6 = 0x00002006,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_OSPFV6 = 0x00002007,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_VRRPV6 = 0x00002008,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_IPV6_NEIGHBOR_DISCOVERY = 0x00002009,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_IPV6_MLD_V1_V2 = 0x0000200a,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_IPV6_MLD_V1_REPORT = 0x0000200b,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_IPV6_MLD_V1_DONE = 0x0000200c,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_MLD_V2_REPORT = 0x0000200d,

    /**
     * @brief Unknown L3 multicast packets
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_UNKNOWN_L3_MULTICAST = 0x0000200e,

    /**
     * @brief Source NAT miss packets
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_SNAT_MISS = 0x0000200f,

    /**
     * @brief Destination NAT miss packets
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_DNAT_MISS = 0x00002010,

    /**
     * @brief NAT hairpin packets
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_NAT_HAIRPIN = 0x00002011,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_IPV6_NEIGHBOR_SOLICITATION = 0x00002012,

    /** Default packet action is forward */
    SAI_HOSTIF_TRAP_TYPE_IPV6_NEIGHBOR_ADVERTISEMENT = 0x00002013,

    /** Router traps custom range start */
    SAI_HOSTIF_TRAP_TYPE_ROUTER_CUSTOM_RANGE_BASE = 0x00003000,

    /* Local IP traps */

    /**
     * @brief IP packets to local router IP address (routes with
     * #SAI_ROUTE_ENTRY_ATTR_NEXT_HOP_ID = #SAI_SWITCH_ATTR_CPU_PORT)
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_IP2ME = 0x00004000,

    /**
     * @brief SSH traffic (TCP dst port == 22) to local router IP address
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_SSH = 0x00004001,

    /**
     * @brief SNMP traffic (UDP dst port == 161) to local router IP address
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_SNMP = 0x00004002,

    /**
     * @brief BGP traffic (TCP src port == 179 or TCP dst port == 179) to local
     * router IP address (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_BGP = 0x00004003,

    /**
     * @brief BGPv6 traffic (TCP src port == 179 or TCP dst port == 179) to
     * local router IP address (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_BGPV6 = 0x00004004,

    /**
     * @brief BFD traffic (UDP dst port == 3784 or UDP dst port == 4784) to local
     * router IP address (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_BFD = 0x00004005,

    /**
     * @brief BFDV6 traffic (UDP dst port == 3784 or UDP dst port == 4784) to
     * local router IP address (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_BFDV6 = 0x00004006,

    /** Local IP traps custom range start */
    SAI_HOSTIF_TRAP_TYPE_LOCAL_IP_CUSTOM_RANGE_BASE = 0x00005000,

    /* Pipeline exceptions */

    /**
     * @brief Packets size exceeds the router interface MTU size
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_L3_MTU_ERROR = 0x00006000,

    /**
     * @brief Packets with TTL 0 or 1
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_TTL_ERROR = 0x00006001,

    /**
     * @brief Packets trapped when station move is observed with static FDB entry
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_STATIC_FDB_MOVE = 0x00006002,

    /* Pipeline discards. For the following traps, packet action is either drop or trap */

    /**
     * @brief Packets discarded due to egress buffer full
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_PIPELINE_DISCARD_EGRESS_BUFFER = 0x00007000,

    /**
     * @brief Packets discarded by WRED
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_PIPELINE_DISCARD_WRED = 0x00007001,

    /**
     * @brief Packets discarded due to router causes, such as
     * header checksum, router interface is down,
     * matching a route with drop action (black holes), etc.
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_PIPELINE_DISCARD_ROUTER = 0x00007002,

    /**
     * @brief MPLS packets with expiring TTL value of 1
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_MPLS_TTL_ERROR = 0x00008000,

    /**
     * @brief MPLS packet with router alert label
     * (default packet action is forward)
     */
    SAI_HOSTIF_TRAP_TYPE_MPLS_ROUTER_ALERT_LABEL = 0x00008001,

    /** Exception traps custom range start */
    SAI_HOSTIF_TRAP_TYPE_CUSTOM_EXCEPTION_RANGE_BASE = 0x00009000,

    /**
     * @brief End of trap types
     */
    SAI_HOSTIF_TRAP_TYPE_END = 0x0000a000

} sai_hostif_trap_type_t;

/**
 * @brief Host interface trap attributes
 */
typedef enum _sai_hostif_trap_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_HOSTIF_TRAP_ATTR_START,

    /**
     * @brief Host interface trap type
     *
     * @type sai_hostif_trap_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     */
    SAI_HOSTIF_TRAP_ATTR_TRAP_TYPE = SAI_HOSTIF_TRAP_ATTR_START,

    /**
     * @brief Trap action
     *
     * @type sai_packet_action_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION,

    /**
     * @brief Trap priority.
     *
     * This is equivalent to ACL entry priority #SAI_ACL_ENTRY_ATTR_PRIORITY.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default attrvalue SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY
     * @validonly SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION == SAI_PACKET_ACTION_TRAP or SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION == SAI_PACKET_ACTION_COPY
     */
    SAI_HOSTIF_TRAP_ATTR_TRAP_PRIORITY,

    /**
     * @brief List of SAI ports to be excluded (disabled) from the trap generation
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT
     * @default empty
     * @validonly SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION == SAI_PACKET_ACTION_TRAP or SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION == SAI_PACKET_ACTION_COPY
     */
    SAI_HOSTIF_TRAP_ATTR_EXCLUDE_PORT_LIST,

    /**
     * @brief Trap group ID for the trap
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP
     * @default attrvalue SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP
     * @validonly SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION == SAI_PACKET_ACTION_TRAP or SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION == SAI_PACKET_ACTION_COPY
     */
    SAI_HOSTIF_TRAP_ATTR_TRAP_GROUP,

    /**
     * @brief Mirror session for the trap
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_MIRROR_SESSION
     * @default empty
     */
    SAI_HOSTIF_TRAP_ATTR_MIRROR_SESSION,

    /**
     * @brief Attach a counter
     *
     * When it is empty, then packet hits won't be counted
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_HOSTIF_TRAP_ATTR_COUNTER_ID,

    /**
     * @brief End of attributes
     */
    SAI_HOSTIF_TRAP_ATTR_END,

    /** Custom range start */
    SAI_HOSTIF_TRAP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** Custom range end */
    SAI_HOSTIF_TRAP_ATTR_CUSTOM_RANGE_END

} sai_hostif_trap_attr_t;

/**
 * @brief Create host interface trap
 *
 * @param[out] hostif_trap_id Host interface trap id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_hostif_trap_fn)(
        _Out_ sai_object_id_t *hostif_trap_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove host interface trap
 *
 * @param[in] hostif_trap_id Host interface trap id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_hostif_trap_fn)(
        _In_ sai_object_id_t hostif_trap_id);

/**
 * @brief Set trap attribute value.
 *
 * @param[in] hostif_trap_id Host interface trap id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_hostif_trap_attribute_fn)(
        _In_ sai_object_id_t hostif_trap_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get trap attribute value.
 *
 * @param[in] hostif_trap_id Host interface trap id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_hostif_trap_attribute_fn)(
        _In_ sai_object_id_t hostif_trap_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Host interface user defined trap type
 *
 * User defined traps action is controlled by the referencing object.
 * For example, ACL entry with packet action trap and user trap object ID
 */
typedef enum _sai_hostif_user_defined_trap_type_t
{
    /**
     * @brief Start of user defined trap types
     */
    SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_START = 0x00000000,

    /** Router traps */
    SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_ROUTER = SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_START,

    /**
     * @brief Neighbor table traps
     *
     * Generated by neighbor table entry hit with action trap/log, or by neighbor table miss
     */
    SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_NEIGHBOR,

    /** @ignore - for backward compatibility */
    SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_NEIGH = SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_NEIGHBOR,

    /** ACL traps */
    SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_ACL,

    /** FDB traps */
    SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_FDB,

    /** In Segment Entry traps */
    SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_INSEG_ENTRY,

    /** Custom range base */
    SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_CUSTOM_RANGE_BASE = 0x00001000,

    /**
     * @brief End of user defined trap types
     */
    SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_END,

} sai_hostif_user_defined_trap_type_t;

/**
 * @brief Host interface user defined trap attributes
 */
typedef enum _sai_hostif_user_defined_trap_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_START,

    /**
     * @brief Host interface user defined trap type
     *
     * It is valid to create multiple instances of the same user defined type
     *
     * @type sai_hostif_user_defined_trap_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TYPE = SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_START,

    /**
     * @brief Trap priority. This is equivalent to ACL entry priority
     * #SAI_ACL_ENTRY_ATTR_PRIORITY
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default attrvalue SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY
     */
    SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TRAP_PRIORITY,

    /**
     * @brief Trap group ID for the trap
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP
     * @default attrvalue SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP
     */
    SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TRAP_GROUP,

    /**
     * @brief End of attributes
     */
    SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_END,

    /** Custom range start */
    SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** Custom range end */
    SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_CUSTOM_RANGE_END

} sai_hostif_user_defined_trap_attr_t;

/**
 * @brief Create host interface user defined trap
 *
 * @param[out] hostif_user_defined_trap_id Host interface user defined trap id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_hostif_user_defined_trap_fn)(
        _Out_ sai_object_id_t *hostif_user_defined_trap_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove host interface user defined trap
 *
 * @param[in] hostif_user_defined_trap_id Host interface user defined trap id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_hostif_user_defined_trap_fn)(
        _In_ sai_object_id_t hostif_user_defined_trap_id);

/**
 * @brief Set user defined trap attribute value.
 *
 * @param[in] hostif_user_defined_trap_id Host interface user defined trap id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_hostif_user_defined_trap_attribute_fn)(
        _In_ sai_object_id_t hostif_user_defined_trap_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get user defined trap attribute value.
 *
 * @param[in] hostif_user_defined_trap_id Host interface user defined trap id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_hostif_user_defined_trap_attribute_fn)(
        _In_ sai_object_id_t hostif_user_defined_trap_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute data for SAI_HOSTIF_ATTR_TYPE
 */
typedef enum _sai_hostif_type_t
{
    /** Netdevice */
    SAI_HOSTIF_TYPE_NETDEV,

    /** File descriptor */
    SAI_HOSTIF_TYPE_FD,

    /** Generic netlink */
    SAI_HOSTIF_TYPE_GENETLINK

} sai_hostif_type_t;

/**
 * @brief Attribute data for SAI_HOSTIF_ATTR_VLAN_TAG
 */
typedef enum _sai_hostif_vlan_tag_t
{
    /**
     * @brief Strip vlan tag
     * Strip vlan tag from the incoming packet
     * when delivering the packet to host interface.
     */
    SAI_HOSTIF_VLAN_TAG_STRIP,

    /**
     * @brief Keep vlan tag.
     * When incoming packet is untagged, add PVID tag to the packet when delivering
     * the packet to host interface.
     */
    SAI_HOSTIF_VLAN_TAG_KEEP,

    /**
     * @brief Keep the packet same as the incoming packet
     *
     * The packet delivered to host interface is the same as the original packet.
     * When the host interface is PORT and LAG, the packet delivered to host interface is the
     * same as the original packet seen by the PORT and LAG.
     * When the host interface is VLAN, the packet delivered to host interface will not have tag.
     */
    SAI_HOSTIF_VLAN_TAG_ORIGINAL,

} sai_hostif_vlan_tag_t;

/**
 * @brief Host interface attribute IDs
 */
typedef enum _sai_hostif_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_HOSTIF_ATTR_START,

    /**
     * @brief Host interface type
     *
     * @type sai_hostif_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_HOSTIF_ATTR_TYPE = SAI_HOSTIF_ATTR_START,

    /**
     * @brief Host interface object ID
     *
     * Port netdev will be created when object type is SAI_OBJECT_TYPE_PORT
     * LAG netdev will be created when object type is SAI_OBJECT_TYPE_LAG
     * VLAN netdev will be created when object type is SAI_OBJECT_TYPE_VLAN
     * System Port netdev will be created when object type is SAI_OBJECT_TYPE_SYSTEM_PORT
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_VLAN, SAI_OBJECT_TYPE_SYSTEM_PORT
     * @condition SAI_HOSTIF_ATTR_TYPE == SAI_HOSTIF_TYPE_NETDEV
     */
    SAI_HOSTIF_ATTR_OBJ_ID,

    /**
     * @brief Name [char[SAI_HOSTIF_NAME_SIZE]]
     *
     * The maximum number of characters for the name is SAI_HOSTIF_NAME_SIZE - 1 since
     * it needs the terminating null byte ('\0') at the end.
     *
     * If Hostif is a generic netlink, this indicates the generic netlink family name.
     *
     * @type char
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_HOSTIF_ATTR_TYPE == SAI_HOSTIF_TYPE_NETDEV or SAI_HOSTIF_ATTR_TYPE == SAI_HOSTIF_TYPE_GENETLINK
     */
    SAI_HOSTIF_ATTR_NAME,

    /**
     * @brief Set the operational status for this host interface
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_HOSTIF_ATTR_OPER_STATUS,

    /**
     * @brief Set the queue index to be used for packets going out through this interface
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_HOSTIF_ATTR_QUEUE,

    /**
     * @brief Strip/keep vlan tag for received packet
     *
     * @type sai_hostif_vlan_tag_t
     * @flags CREATE_AND_SET
     * @default SAI_HOSTIF_VLAN_TAG_STRIP
     * @validonly SAI_HOSTIF_ATTR_TYPE == SAI_HOSTIF_TYPE_NETDEV
     */
    SAI_HOSTIF_ATTR_VLAN_TAG,

    /**
     * @brief Name [char[SAI_HOSTIF_GENETLINK_MCGRP_NAME_SIZE]]
     *
     * The maximum number of characters for the name is SAI_HOSTIF_GENETLINK_MCGRP_NAME_SIZE - 1
     * Set the Generic netlink multicast group name on which the packets/buffers
     * are received on this host interface
     *
     * @type char
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_HOSTIF_ATTR_TYPE == SAI_HOSTIF_TYPE_GENETLINK
     */
    SAI_HOSTIF_ATTR_GENETLINK_MCGRP_NAME,

    /**
     * @brief End of attributes
     */
    SAI_HOSTIF_ATTR_END,

    /** Custom range base value */
    SAI_HOSTIF_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_HOSTIF_ATTR_CUSTOM_RANGE_END

} sai_hostif_attr_t;

/**
 * @brief Create host interface
 *
 * @param[out] hostif_id Host interface id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_hostif_fn)(
        _Out_ sai_object_id_t *hostif_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove host interface
 *
 * @param[in] hostif_id Host interface id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_hostif_fn)(
        _In_ sai_object_id_t hostif_id);

/**
 * @brief Set host interface attribute
 *
 * @param[in] hostif_id Host interface id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_hostif_attribute_fn)(
        _In_ sai_object_id_t hostif_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get host interface attribute
 *
 * @param[in] hostif_id Host interface id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_hostif_attribute_fn)(
        _In_ sai_object_id_t hostif_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute data for SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE
 */
typedef enum _sai_hostif_table_entry_type_t
{
    /** Port-based Host Interface entry Type */
    SAI_HOSTIF_TABLE_ENTRY_TYPE_PORT,

    /** LAG based Host Interface entry Type */
    SAI_HOSTIF_TABLE_ENTRY_TYPE_LAG,

    /** Vlan based Host Interface entry Type */
    SAI_HOSTIF_TABLE_ENTRY_TYPE_VLAN,

    /** Wildcard Interface entry Type */
    SAI_HOSTIF_TABLE_ENTRY_TYPE_TRAP_ID,

    /** Wildcard Interface, wildcard trap id */
    SAI_HOSTIF_TABLE_ENTRY_TYPE_WILDCARD

} sai_hostif_table_entry_type_t;

/**
 * @brief Attribute data for SAI_HOSTIF_TABLE_ENTRY_ATTR_CHANNEL_TYPE
 */
typedef enum _sai_hostif_table_entry_channel_type_t
{
    /** Receive packets via callback */
    SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_CB,

    /** Receive packets via file descriptor */
    SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_FD,

    /** Receive packets via Linux netdev type port */
    SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_PHYSICAL_PORT,

    /** Receive packets via Linux netdev logical port (LAG or port) */
    SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_LOGICAL_PORT,

    /** Receive packets via Linux netdev L3 interface */
    SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_L3,

    /** Receive packets via Linux generic netlink interface */
    SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_GENETLINK

} sai_hostif_table_entry_channel_type_t;

/**
 * @brief Host interface table entry attribute IDs
 */
typedef enum _sai_hostif_table_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_HOSTIF_TABLE_ENTRY_ATTR_START,

    /**
     * @brief Host interface table entry type
     *
     * @type sai_hostif_table_entry_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE = SAI_HOSTIF_ATTR_START,

    /**
     * @brief Host interface table entry match field object-id
     *
     * Should be port object when type is SAI_HOSTIF_TABLE_ENTRY_TYPE_PORT.
     * Should be LAG object when type is SAI_HOSTIF_TABLE_ENTRY_TYPE_LAG.
     * Should be VLAN ID object when type is SAI_HOSTIF_TABLE_ENTRY_TYPE_VLAN.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @condition SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE == SAI_HOSTIF_TABLE_ENTRY_TYPE_PORT or SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE == SAI_HOSTIF_TABLE_ENTRY_TYPE_VLAN or SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE == SAI_HOSTIF_TABLE_ENTRY_TYPE_LAG
     */
    SAI_HOSTIF_TABLE_ENTRY_ATTR_OBJ_ID,

    /**
     * @brief Host interface table entry match field trap-id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_HOSTIF_TRAP, SAI_OBJECT_TYPE_HOSTIF_USER_DEFINED_TRAP
     * @condition SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE == SAI_HOSTIF_TABLE_ENTRY_TYPE_PORT or SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE == SAI_HOSTIF_TABLE_ENTRY_TYPE_VLAN or SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE == SAI_HOSTIF_TABLE_ENTRY_TYPE_LAG or SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE == SAI_HOSTIF_TABLE_ENTRY_TYPE_TRAP_ID
     */
    SAI_HOSTIF_TABLE_ENTRY_ATTR_TRAP_ID,

    /**
     * @brief Host interface table entry action channel
     *
     * @type sai_hostif_table_entry_channel_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_HOSTIF_TABLE_ENTRY_ATTR_CHANNEL_TYPE,

    /**
     * @brief Host interface table entry action target host interface object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_HOSTIF
     * @condition SAI_HOSTIF_TABLE_ENTRY_ATTR_CHANNEL_TYPE == SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_FD or SAI_HOSTIF_TABLE_ENTRY_ATTR_CHANNEL_TYPE == SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_GENETLINK
     */
    SAI_HOSTIF_TABLE_ENTRY_ATTR_HOST_IF,

    /**
     * @brief End of attributes
     */
    SAI_HOSTIF_TABLE_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_HOSTIF_TABLE_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_HOSTIF_TABLE_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_hostif_table_entry_attr_t;

/**
 * @brief Create host interface table entry
 *
 * @param[out] hostif_table_entry_id Host interface table entry
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_hostif_table_entry_fn)(
        _Out_ sai_object_id_t *hostif_table_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove host interface table entry
 *
 * @param[in] hostif_table_entry_id Host interface table entry
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_hostif_table_entry_fn)(
        _In_ sai_object_id_t hostif_table_entry_id);

/**
 * @brief Set host interface table entry attribute
 *
 * @param[in] hostif_table_entry_id Host interface table entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_hostif_table_entry_attribute_fn)(
        _In_ sai_object_id_t hostif_table_entry_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get host interface table entry attribute
 *
 * @param[in] hostif_table_entry_id Host interface table entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_hostif_table_entry_attribute_fn)(
        _In_ sai_object_id_t hostif_table_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Host interface TX type
 */
typedef enum _sai_hostif_tx_type_t
{
    /**
     * @brief Bypass switch ASIC processing pipeline,
     * tx packet goes to the specified output port directly
     */
    SAI_HOSTIF_TX_TYPE_PIPELINE_BYPASS,

    /** TX packet goes to the switch ASIC processing pipeline to decide the output port */
    SAI_HOSTIF_TX_TYPE_PIPELINE_LOOKUP,

    /** Custom range base */
    SAI_HOSTIF_TX_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_hostif_tx_type_t;

/**
 * @brief Host interface packet attributes
 */
typedef enum _sai_hostif_packet_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_HOSTIF_PACKET_ATTR_START,

    /**
     * @brief Trap ID (for receive-only)
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_HOSTIF_TRAP, SAI_OBJECT_TYPE_HOSTIF_USER_DEFINED_TRAP
     */
    SAI_HOSTIF_PACKET_ATTR_HOSTIF_TRAP_ID = SAI_HOSTIF_PACKET_ATTR_START,

    /**
     * @brief Ingress port (for receive-only)
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_HOSTIF_PACKET_ATTR_INGRESS_PORT,

    /**
     * @brief Ingress LAG (for receive-only)
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_LAG
     */
    SAI_HOSTIF_PACKET_ATTR_INGRESS_LAG,

    /**
     * @brief Packet transmit type. (MANDATORY_ON_SEND)
     *
     * @type sai_hostif_tx_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_HOSTIF_PACKET_ATTR_HOSTIF_TX_TYPE,

    /**
     * @brief Egress port
     *
     * For receive case, filled with the egress destination port for unicast packets.
     * Egress LAG member port id to be filled for the LAG destination case.
     * Applicable for use-case like samplepacket traps or PTP TX event
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     * @condition SAI_HOSTIF_PACKET_ATTR_HOSTIF_TX_TYPE == SAI_HOSTIF_TX_TYPE_PIPELINE_BYPASS
     */
    SAI_HOSTIF_PACKET_ATTR_EGRESS_PORT_OR_LAG,

    /**
     * @brief Bridge ID (for receive-only)
     *
     * The .1D or .1Q bridge on which the packet was received.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE
     */
    SAI_HOSTIF_PACKET_ATTR_BRIDGE_ID,

    /**
     * @brief Timestamp
     *
     * The timestamp on which the packet was received, or sent for PTP TX event.
     *
     * @type sai_timespec_t
     * @flags READ_ONLY
     */
    SAI_HOSTIF_PACKET_ATTR_TIMESTAMP,

    /**
     * @brief Egress queue index
     *
     * The egress queue id for egress port or LAG.
     *
     * @type sai_uint8_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_HOSTIF_PACKET_ATTR_EGRESS_QUEUE_INDEX,

    /**
     * @brief End of attributes
     */
    SAI_HOSTIF_PACKET_ATTR_END,

    /** Custom range base value */
    SAI_HOSTIF_PACKET_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_HOSTIF_PACKET_ATTR_CUSTOM_RANGE_END

} sai_hostif_packet_attr_t;

/**
 * @brief Hostif receive function
 *
 * @param[in] hostif_id Host interface id
 * @param[inout] buffer_size Allocated buffer size [in], Actual packet size in bytes [out]
 * @param[out] buffer Packet buffer
 * @param[inout] attr_count Allocated list size [in], Number of attributes [out]
 * @param[out] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success #SAI_STATUS_BUFFER_OVERFLOW if
 * buffer_size is insufficient, and buffer_size will be filled with required
 * size. Or if attr_count is insufficient, and attr_count will be filled with
 * required count. Failure status code on error
 */
typedef sai_status_t (*sai_recv_hostif_packet_fn)(
        _In_ sai_object_id_t hostif_id,
        _Inout_ sai_size_t *buffer_size,
        _Out_ void *buffer,
        _Inout_ uint32_t *attr_count,
        _Out_ sai_attribute_t *attr_list);

/**
 * @brief Hostif send function
 *
 * @param[in] hostif_id Host interface id.
 *    When sending through FD channel, fill SAI_OBJECT_TYPE_HOST_INTERFACE object, of type #SAI_HOSTIF_TYPE_FD.
 *    When sending through CB channel, fill Switch Object ID, SAI_OBJECT_TYPE_SWITCH.
 * @param[in] buffer_size Packet size in bytes
 * @param[in] buffer Packet buffer
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_send_hostif_packet_fn)(
        _In_ sai_object_id_t hostif_id,
        _In_ sai_size_t buffer_size,
        _In_ const void *buffer,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Hostif receive callback
 *
 * @count attr_list[attr_count]
 * @count buffer[buffer_size]
 * @objects attr_list SAI_OBJECT_TYPE_HOSTIF_PACKET
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 *
 * @param[in] switch_id Switch Object ID
 * @param[in] buffer_size Actual packet size in bytes
 * @param[in] buffer Packet buffer
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 */
typedef void (*sai_packet_event_notification_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ sai_size_t buffer_size,
        _In_ const void *buffer,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Hostif methods table retrieved with sai_api_query()
 */
typedef struct _sai_hostif_api_t
{
    sai_create_hostif_fn                           create_hostif;
    sai_remove_hostif_fn                           remove_hostif;
    sai_set_hostif_attribute_fn                    set_hostif_attribute;
    sai_get_hostif_attribute_fn                    get_hostif_attribute;
    sai_create_hostif_table_entry_fn               create_hostif_table_entry;
    sai_remove_hostif_table_entry_fn               remove_hostif_table_entry;
    sai_set_hostif_table_entry_attribute_fn        set_hostif_table_entry_attribute;
    sai_get_hostif_table_entry_attribute_fn        get_hostif_table_entry_attribute;
    sai_create_hostif_trap_group_fn                create_hostif_trap_group;
    sai_remove_hostif_trap_group_fn                remove_hostif_trap_group;
    sai_set_hostif_trap_group_attribute_fn         set_hostif_trap_group_attribute;
    sai_get_hostif_trap_group_attribute_fn         get_hostif_trap_group_attribute;
    sai_create_hostif_trap_fn                      create_hostif_trap;
    sai_remove_hostif_trap_fn                      remove_hostif_trap;
    sai_set_hostif_trap_attribute_fn               set_hostif_trap_attribute;
    sai_get_hostif_trap_attribute_fn               get_hostif_trap_attribute;
    sai_create_hostif_user_defined_trap_fn         create_hostif_user_defined_trap;
    sai_remove_hostif_user_defined_trap_fn         remove_hostif_user_defined_trap;
    sai_set_hostif_user_defined_trap_attribute_fn  set_hostif_user_defined_trap_attribute;
    sai_get_hostif_user_defined_trap_attribute_fn  get_hostif_user_defined_trap_attribute;
    sai_recv_hostif_packet_fn                      recv_hostif_packet;
    sai_send_hostif_packet_fn                      send_hostif_packet;
} sai_hostif_api_t;

/**
 * @}
 */
#endif /** __SAIHOSTIF_H_ */
