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
 * @file    saimacsec.h
 *
 * @brief   This module defines SAI MACSEC interface
 *          Version 1.0
 */

#if !defined (__SAIMACSEC_H_)
#define __SAIMACSEC_H_

#include <saitypes.h>

/**
 * @defgroup SAIMACSEC SAI - MACSEC specific API definitions
 *
 * @{
 */

/**
 * @brief MACSEC direction types
 */
typedef enum _sai_macsec_direction_t
{
    /*
     * MACSEC directions.
     *
     */

    /* 
     * @brief System to line direction
     */
    SAI_MACSEC_DIRECTION_EGRESS,

    /*
     * @brief Line to system direction
     */
    SAI_MACSEC_DIRECTION_INGRESS,
} sai_macsec_direction_t;


/**
 * @brief Attribute Id for sai_macsec
 *
 * @flags Contains flags
 */
typedef enum _sai_macsec_attr_t
{
    /**
     * @brief Start of MACSEC attributes
     */
    SAI_MACSEC_ATTR_START,

    /**
     * @brief MACSEC direction
     *
     * @type sai_macsec_direction_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_ATTR_DIRECTION = SAI_MACSEC_ATTR_START,

    /**
     * @brief SAI_SWITCH_SWITCHING_MODE_CUT_THROUGH supported
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_SWITCHING_MODE_CUT_THROUGH_SUPPORTED,

    /**
     * @brief SAI_SWITCH_SWITCHING_MODE_STORE_AND_FORWARD supported
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_SWITCHING_MODE_STORE_AND_FORWARD_SUPPORTED,

    /**
     * @brief SAI_STATS_MODE_READ supported
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_STATS_MODE_READ_SUPPORTED,

    /**
     * @brief SAI_STATS_MODE_READ_CLEAR supported
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_STATS_MODE_READ_CLEAR_SUPPORTED,

    /**
     * @brief Indicates if ingress can use SCI only as a rule match field.
     * True indicates SCI can only be used as rule match field. In that case,
     * 1 ingress flow can be associated with only 1 rule and 1 Secure Channel.
     * false indicates one 1 flow can be associated with multiple rules and
     * multiple Secure Channels.
     *
     * @type bool
     * @flags READ_ONLY
     * @condition SAI_MACSEC_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
     */
    SAI_MACSEC_ATTR_SCI_IN_INGRESS_RULES,

    /**
     * @brief Range of MACSEC rule priority values
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_RULE_PRIORITY_RANGE,

    /**
     * @brief Indicates if 32-bit Packer Number (PN) is supported.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_PN_32BIT_SUPPORTED,

    /**
     * @brief Indicates if 64-bit Extended Packer Number (PN) is supported.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_XPN_64BIT_SUPPORTED,

    /**
     * @brief Indicates if GCM-AES128 cipher-suite is supported.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_GCM_AES128_SUPPORTED,

    /**
     * @brief Indicates if GCM-AES256 cipher-suite is supported.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_GCM_AES256_SUPPORTED,

    /**
     * @brief List of supported SecTAG offset values for both ingress parsing
     * and for egress.
     *
     * @type sai_u8_list_t
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_SECTAG_OFFSETS_SUPPORTED,

    /**
     * @brief Macsec MTU capability on system side (not including Macsec overhead).
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_SYSTEM_SIDE_MTU,

    /**
     * @brief Warm boot is supported for all saimacsec objects.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_WARM_BOOT_SUPPORTED,

    /**
     * @brief When false disables creation of saimacsec objects during Phy warm-boot.
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_MACSEC_ATTR_WARM_BOOT_ENABLE,

    /**
     * @brief TPID value used to identify packet C-tag.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @default 0x8100
     */
    SAI_MACSEC_ATTR_CTAG_TPID,

    /**
     * @brief TPID value used to identify packet S-tag.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @default 0x88A8
     */
    SAI_MACSEC_ATTR_STAG_TPID,

    /**
     * @brief Global setting of read-clear or read-only for statistics read.
     * The mode parameter for get_macsec_<foo>_stats_ext should match this.
     *
     * @type sai_stats_mode_t
     * @flags CREATE_AND_SET
     */
    SAI_MACSEC_ATTR_STATS_MODE,

    /**
     * @brief Enables physical bypass of MACsec module. Packets can physically
     * bypass MACsec module only when there is no macsec_port object
     * associated with Phy line-side port(s).
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_MACSEC_ATTR_PHYSICAL_BYPASS_ENABLE,

    /**
     * @brief List of MACsec ports
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_MACSEC_ATTR_PORT_LIST,

    /**
     * @brief List of all MACsec flow objects
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_FLOW
     */
    SAI_MACSEC_ATTR_FLOW_LIST,

    /**
     * @brief End of MACsec attributes
     */
    SAI_MACSEC_ATTR_END,
} sai_macsec_attr_t;


/**
 * @brief Attribute Id for sai_macsec_port
 *
 * @flags Contains flags
 */
typedef enum _sai_macsec_port_attr_t
{
    /**
     * @brief Start of MACsec Port attributes
     */
    SAI_MACSEC_PORT_ATTR_START,

    /**
     * @brief MACSEC direction
     *
     * @type sai_macsec_direction_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_PORT_ATTR_MACSEC_DIRECTION = SAI_MACSEC_PORT_ATTR_START,

    /**
     * @brief Phy line-side Port Id associated with this MACsec port
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_MACSEC_PORT_ATTR_PHY_LINE_PORT_ID,

    /**
     * @brief Enable vlan tag parsing for C-tag TPID
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_MACSEC_PORT_ATTR_CTAG_ENABLE,

    /**
     * @brief Enable vlan tag parsing for S-tag TPID
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_MACSEC_PORT_ATTR_STAG_ENABLE,

    /**
     * @brief Switching mode for port.  If configured as cut-through, the IPG
     * for Tx MAC in the switch ASIC has to be increased to accommodate the
     * MACsec packet size expansion.
     *
     * @type sai_switch_switching_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_SWITCH_SWITCHING_MODE_CUT_THROUGH
     */
    SAI_MACSEC_PORT_ATTR_SWITCH_SWITCHING_MODE,

    /**
     * @brief End of MACsec Port attributes
     */
    SAI_MACSEC_PORT_ATTR_END,
} sai_macsec_port_attr_t;


/**
 * @brief MACSEC port counter IDs in sai_get_macsec_stats_ext() call
 */
typedef enum _sai_macsec_port_stat_t
{
     /**
      *Malformed packets dropped before MACsec processing, not in 802.1ae MIB
      */
     SAI_MACSEC_STAT_PRE_MACSEC_DROP_PACKETS,

     /**
      *Packets classified as control packets for MACsec processing, not in 802.1ae MIB
      */
     SAI_MACSEC_STAT_CONTROL_PACKETS,

     /**
      *Packets classified as data packets for MACsec processing, not in 802.1ae MIB
      */
     SAI_MACSEC_STAT_DATA_PACKETS,

     /**
      * 802.1ae defined ifOutOctets or ifInOctets for MACSEC uncontrolled port
      */
     SAI_MACSEC_STAT_OCTETS_UNCONTROLLED,

     /**
      *802.1ae defined ifOutOctets or ifInOctets for MACSEC controlled port
      */
     SAI_MACSEC_STAT_OCTETS_CONTROLLED,

     /**
      *802.1ae defined ifOutUcastPkts or ifInUcastPkts for MACSEC uncontrolled port
      */
     SAI_MACSEC_STAT_UCAST_PACKETS_UNCONTROLLED,

     /**
      *802.1ae defined ifOutUcastPkts or ifInUcastPkts for MACSEC controlled port
      */
     SAI_MACSEC_STAT_UCAST_PACKETS_CONTROLLED,

     /**
      *802.1ae defined ifOutMulticastPkts or ifInMulticastPkts for MACSEC uncontrolled port
      */
     SAI_MACSEC_STAT_MULTICAST_PACKETS_UNCONTROLLED,

   /**
    *802.1ae defined ifOutMulticastPkts or ifInMulticastPkts for MACSEC controlled port
    */
   SAI_MACSEC_STAT_MULTICAST_PACKETS_CONTROLLED,

   /**
    *802.1ae defined ifOutBroadcastPkts or ifInBroadcastPkts for MACSEC uncontrolled port
    */
   SAI_MACSEC_STAT_BROADCAST_PACKETS_UNCONTROLLED,

   /**
    *802.1ae defined ifOutBroadcastPkts or ifInBroadcastPkts for MACSEC controlled port
    */
   SAI_MACSEC_STAT_BROADCAST_PACKETS_CONTROLLED,
} sai_macsec_port_stat_t;


/**
 * @brief MACSEC flow action types
 */
typedef enum _sai_macsec_flow_action_t
{
    /*
     * MACSEC flow actions.
     *
     */

    /** Drop Packet */
    SAI_MACSEC_FLOW_ACTION_DROP,

    /** Permit Packet in the clear - no encryption/decryption/authentication */
    SAI_MACSEC_FLOW_ACTION_PERMIT,

    /** Permit Packet in the clear and count as control packets */
    SAI_MACSEC_FLOW_ACTION_CONTROL_PACKET,

    /** Secure Packet - encrypt on egress, decrypt and authenticate on ingress */
    SAI_MACSEC_FLOW_ACTION_SECURE,
} sai_macsec_flow_action_t;


/**
 * @brief Attribute Id for sai_macsec_flow
 *
 * @flags Contains flags
 */
typedef enum _sai_macsec_flow_attr_t
{
    /**
     * @brief Start of MACSEC Flow attributes
     */
    SAI_MACSEC_FLOW_ATTR_START,

    /**
     * @brief MACSEC direction
     *
     * @type sai_macsec_direction_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_FLOW_ATTR_MACSEC_DIRECTION = SAI_MACSEC_FLOW_ATTR_START,

    /**
     * @brief MACSEC flow action
     *
     * @type sai_macsec_flow_action_t
     * @flags CREATE_AND_SET
     * @default SAI_MACSEC_FLOW_ACTION_DROP
     */
    SAI_MACSEC_FLOW_ATTR_FLOW_ACTION,

    /**
     * @brief List of MACSEC rules associated with this flow.
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_RULE
     */
    SAI_MACSEC_FLOW_ATTR_RULE_LIST,

    /**
     * @brief List of MACSEC Secure Channels associated with this flow.
     * This list non-empty only for SAI_MACSEC_FLOW_ACTION_SECURE.
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_SECURE_CHANNEL
     */
    SAI_MACSEC_FLOW_ATTR_SECURE_CHANNEL_LIST,

    /**
     * @brief End of MACSEC Flow attributes
     */
    SAI_MACSEC_FLOW_ATTR_END,
} sai_macsec_flow_attr_t;


/**
 * @brief Attribute Id for sai_macsec_rule
 *
 * @flags Contains flags
 */
typedef enum _sai_macsec_rule_attr_t
{
    /**
     * @brief Start of MACSEC Rule attributes
     */
    SAI_MACSEC_RULE_ATTR_START,

    /**
     * @brief MACSEC direction
     *
     * @type sai_macsec_direction_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_RULE_ATTR_MACSEC_DIRECTION = SAI_MACSEC_RULE_ATTR_START,

    /**
     * @brief MACsec phy port object id
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_PORT
     */
    SAI_MACSEC_RULE_ATTR_PORT_ID,

    /**
     * @brief Priority value used to resolve conflict in case a packet matches
     * multiple rules. For a given SAI_MACSEC_RULE_ATTR_PORT_ID, the rules
     * must have unique priority values. A higher priority value has higher
     * precedence.
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_RULE_ATTR_PRIORITY,

    /*
     * Match fields [sai_macsec_match_field_t]
     * - For enabled field only, mask and data are needed.
     * - When a bit field is used, only the least significant bit(s) are valid
     * for matching.
     */

    /**
     * @brief Start of Rule Match Fields
     */
    SAI_MACSEC_RULE_ATTR_FIELD_START = 0x00001000,

    /**
     * @brief Dst MAC Address
     *
     * @type sai_macsec_rule_match_field_data_t sai_mac_t
     * @flags CREATE_ONLY
     * @default disabled
     */
    SAI_MACSEC_RULE_ATTR_FIELD_DST_MAC = SAI_MACSEC_RULE_ATTR_FIELD_START,

    /**
     * @brief Match on packet with no vlan tag
     *
     * @type sai_macsec_rule_match_field_data_t bool
     * @flags CREATE_ONLY
     * @default disabled
     */
    SAI_MACSEC_RULE_ATTR_FIELD_NO_VLAN_TAG,

    /**
     * @brief Match on packet with only 1 vlan tag
     *
     * @type sai_macsec_rule_match_field_data_t bool
     * @flags CREATE_ONLY
     * @default disabled
     */
    SAI_MACSEC_RULE_ATTR_FIELD_1_VLAN_TAG,

    /**
     * @brief Match on packet with 2 or more vlan tags
     *
     * @type sai_macsec_rule_match_field_data_t bool
     * @flags CREATE_ONLY
     * @default disabled
     */
    SAI_MACSEC_RULE_ATTR_FIELD_2_OR_MORE_VLAN_TAGS,

    /**
     * @brief Outermost vlan_id (12-bits)
     *
     * @type sai_macsec_rule_match_field_data_t sai_vlan_id_t
     * @flags CREATE_ONLY
     * @default disabled
     */
    SAI_MACSEC_RULE_ATTR_FIELD_OUTERMOST_VLAN_ID,

    /**
     * @brief Ethertype of packet after last parsed VLAN tag
     *
     * @type sai_macsec_rule_match_field_data_t sai_uint16_t
     * @flags CREATE_ONLY
     * @default disabled
     */
    SAI_MACSEC_RULE_ATTR_FIELD_ETHERTYPE,

    /**
     * @brief SCI value in MACSEC packet SecTAG.
     *
     * @type sai_macsec_rule_match_field_data_t sai_macsec_sci_t
     * @flags CREATE_ONLY
     * @default disabled
     * @condition SAI_MACSEC_RULE_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
     */
    SAI_MACSEC_RULE_ATTR_FIELD_MACSEC_SCI_ID,

    /**
     * @brief End of Rule Match Fields
     */
    SAI_MACSEC_RULE_ATTR_FIELD_END = SAI_MACSEC_RULE_ATTR_FIELD_MACSEC_SCI_ID,

    /**
     * @brief Enable/disable control of the MACsec rule.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_MACSEC_RULE_ATTR_RULE_ENABLE,

    /**
     * @brief MACSEC flow object id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_FLOW
     */
    SAI_MACSEC_RULE_SECURE_CHANNEL_ATTR_FLOW_ID,

    /**
     * @brief End of MACSEC Rule attributes
     */
    SAI_MACSEC_RULE_ATTR_END,
} sai_macsec_rule_attr_t;

/**
 * @brief MACSEC rule counter ID in sai_get_macsec_rule_stats_ext() call
 */
typedef enum _sai_macsec_rule_stat_t
{
   /**
    *@brief Count of packets that hit this rule
    */
    SAI_MACSEC_RULE_STAT,

} sai_macsec_rule_stat_t;

/**
 * @brief Attribute Id for sai_macsec_secure_channel
 *
 * @flags Contains flags
 */
typedef enum _sai_macsec_secure_channel_attr_t
{
    /**
     * @brief Start of MACSEC Secure Channel attributes
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_START,

    /**
     * @brief MACSEC direction
     *
     * @type sai_macsec_direction_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_DIRECTION = SAI_MACSEC_SECURE_CHANNEL_ATTR_START,

    /**
     * @brief MACSEC flow object id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_FLOW
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_FLOW_ID,

    /**
     * @brief SCI value for this Secure Channel, carried in MACSEC packet SecTAG.
     *
     * @type sai_macsec_sci_t
     * @flags CREATE_AND_SET
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_SCI,

    /**
     * @brief SSCI value for this Secure Channel
     *
     * @type sai_macsec_ssci_t
     * @flags CREATE_AND_SET
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_SSCI,

    /**
     * @brief XPN enable for this Secure Channel
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_XPN_ENABLE,

    /**
     * @brief Explicit SCI enable for this Secure Channel.
     * @type bool
     * @flags CREATE_AND_SET
     * @condition SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_DIRECTION == SAI_MACSEC_DIRECTION_EGRESS
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_EXPLICIT_SCI_ENABLE,

    /**
     * @brief SecTAG offset for this Secure Channel.
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @condition SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_DIRECTION == SAI_MACSEC_DIRECTION_EGRESS
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_SECTAG_OFFSET,

    /**
     * @brief Active MACSEC secure_association
     * @type sai_object_id_t
     * @flags SET_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_SECURE_ASSOCIATION
     * @condition SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_DIRECTION == SAI_MACSEC_DIRECTION_EGRESS
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_ACTIVE_SECURE_ASSOCIATION,

    /**
     * @brief Replay protection enable for this Secure Channel.
     * @type bool
     * @flags CREATE_AND_SET
     * @condition SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_REPLAY_PROTECTION_ENABLE,

    /**
     * @brief Replay protection window for this Secure Channel.
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @condition SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_REPLAY_PROTECTION_WINDOW,

    /**
     * @brief MACSEC secure_association list
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_SECURE_ASSOCIATION
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_SECURE_ASSOCIATION_LIST,

    /**
     * @brief End of MACSEC Secure Channel attributes
     */
    SAI_MACSEC_SECURE_CHANNEL_ATTR_END,
} sai_macsec_secure_channel_attr_t;

/**
 * @brief MACSEC Secure Channel counter IDs in sai_get_macsec_secure_channel_stats_ext() call
 */
typedef enum _sai_macsec_secure_channel_stat_t
{
     /**
      * @brief Control packets which are not secured (using MACsec uncontrolled port) -
      * not in 802.1ae MIB
      */
     SAI_MACSEC_SECURE_CHANNEL_STAT_CONTROL_PACKETS,

     /**
      *@brief 802.1ae defined OutPktsUntagged or InPktsUntagged
      */
     SAI_MACSEC_SECURE_CHANNEL_STAT_PACKETS_UNTAGGED,

     /**
      *@brief Packets that have valid SCI, but the AN value does not have associated SA -
      * not in 802.1ae MIB.
      * Provides the aggregate of InPktsNotUsingSA for all invalid SAs of a SC
      */
     SAI_MACSEC_SECURE_CHANNEL_STAT_SA_NOT_IN_USE,

     /**
      *@brief Control packets with SecTAG which are not secured (using MACsec
      * uncontrolled port) - not in 802.1ae MIB
      */
     SAI_MACSEC_SECURE_CHANNEL_STAT_TAGGED_CONTROL_PACKETS,

     /**
      *@brief 802.1ae defined OutPktsTooLong
      * @condition SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_DIRECTION = SAI_MACSEC_DIRECTION_EGRESS
      */
     SAI_MACSEC_SECURE_CHANNEL_STAT_OUT_PACKETS_TOO_LONG,

     /**
      *@brief 802.1ae defined InPktsNoTag
      * @condition SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_DIRECTION = SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_CHANNEL_STAT_IN_PACKETS_NO_TAG,

     /**
      *@brief 802.1ae defined InPktsBadTag
      * @condition SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_DIRECTION = SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_CHANNEL_STAT_IN_PACKETS_BAD_TAG,

     /**
      *@brief 802.1ae defined InPktsNoSci
      * @condition SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_DIRECTION = SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_CHANNEL_STAT_IN_PACKETS_NO_SCI,

     /**
      *@brief 802.1ae defined InPktsUnknownSci
      * @condition SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_DIRECTION = SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_CHANNEL_STAT_IN_PACKETS_UNKNOWN_SCI,

     /**
      *@brief 802.1ae defined InPktsOverrun
      * @condition SAI_MACSEC_SECURE_CHANNEL_ATTR_MACSEC_DIRECTION = SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_CHANNEL_STAT_IN_PACKETS_OVERRUN,
} sai_macsec_secure_channel_stat_t;


/**
 * @brief Attribute Id for sai_macsec_secure_association
 *
 * @flags Contains flags
 */
typedef enum _sai_macsec_secure_association_attr_t
{
    /**
     * @brief Start of MACSEC Secure Association attributes
     */
    SAI_MACSEC_SECURE_ASSOCIATION_ATTR_START,

    /**
     * @brief MACSEC direction
     *
     * @type sai_macsec_direction_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SECURE_ASSOCIATION_ATTR_MACSEC_DIRECTION = SAI_MACSEC_SECURE_ASSOCIATION_ATTR_START,

    /**
     * @brief MACSEC Secure Channel object id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_SECURE_CHANNEL
     */
    SAI_MACSEC_SECURE_ASSOCIATION_ATTR_SECURE_CHANNEL_ID,

    /**
     * @brief AN value (2-bit) for this Secure Channel, carried in MACSEC packet SecTAG.
     * The value must be distinct from other Secure Associations for the same Secure Channel.
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SECURE_ASSOCIATION_ATTR_AN,

    /**
     * @brief True means encryption is enabled.  False means encryption is disabled.
     *
     * @type bool
     * @flags MANDATORY_ON_CREATE
     */
    SAI_MACSEC_SECURE_ASSOCIATION_ENCRYPTION_ENABLE,

    /**
     * @brief True means 256-bit SAK (encryption key).  False means 128-bit key.
     *
     * @type bool
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SECURE_ASSOCIATION_ATTR_SAK_256_BITS,

    /**
     * @brief MACSEC SAK (Secure Association Key) used for encryption/decryption.
     *
     * @type sai_macsec_sak_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SECURE_ASSOCIATION_ATTR_SAK,

    /**
     * @brief MACSEC Salt used for encryption/decryption.
     *
     * @type sai_macsec_salt_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SECURE_ASSOCIATION_ATTR_SALT,

    /**
     * @brief MACSEC Authentication Key
     *
     * @type sai_macsec_auth_key_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SECURE_ASSOCIATION_ATTR_AUTH_KEY,

    /**
     * @brief MACSEC egress packet number (PN/XPN).  At most 1 less than the next PN/XPN.
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @condition SAI_MACSEC_SECURE_ASSOCIATION_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_EGRESS
     */
    SAI_MACSEC_SECURE_ASSOCIATION_ATTR_XPN,

    /**
     * @brief Minimum value of ingress MACSEC packet number (PN/XPN).
     * Updated by value from MACSEC peer by Key Agreement protocol.
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @condition SAI_MACSEC_SECURE_ASSOCIATION_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
     */
    SAI_MACSEC_SECURE_ASSOCIATION_ATTR_MINIMUM_XPN,

    /**
     * @brief End of MACSEC Secure Association attributes
     */
    SAI_MACSEC_SECURE_ASSOCIATION_ATTR_END,
} sai_macsec_secure_association_attr_t;


/**
 * @brief MACSEC flow counter IDs in sai_get_macsec_secure_association_stats_ext() call.
 * Some of these counters appear as per Secure Channel counters in 802.1ae MIB.
 * The application (NOS) has to add these per Secure Association counters to
 * get the per Secure Channel value.
 */
typedef enum _sai_macsec_secure_association_stat_t
{
     /**
      *@brief The sum of this count over all Secure Associations of a Secure
      * Channel gives 802.1ae statistics outOctetsEncrypted for egress and
      * inOctetsDecrypted for ingress
      */
     SAI_MACSEC_SECURE_ASSOCIATION_STAT_OCTETS_ENCRYPTED,

     /**
      *@brief The sum of this count over Secure Associations gives 802.1ae
      * statistics outOctetsProtected for egress and inOctetsValidated for ingress
      */
     SAI_MACSEC_SECURE_ASSOCIATION_STAT_OCTETS_PROTECTED,

     /**
      *@brief The sum of this count over Secure Associations gives 802.1ae statistics
      * outPacketsEncrypted.
      * @condition
      * SAI_MACSEC_SECURE_ASSOCIATION_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_EGRESS
      */
     SAI_MACSEC_SECURE_ASSOCIATION_STAT_OUT_PACKETS_ENCRYPTED,

     /**
      *@brief The sum of this count over Secure Associations gives 802.1ae statistics
      * outPacketsProtected.
      * @condition
      * SAI_MACSEC_SECURE_ASSOCIATION_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_EGRESS
      */
     SAI_MACSEC_SECURE_ASSOCIATION_STAT_OUT_PACKETS_PROTECTED,

     /**
      *@brief The sum of this count over Secure Associations gives 802.1ae statistics
      * inPacketsUnchecked.
      * @condition
      * SAI_MACSEC_SECURE_ASSOCIATION_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_ASSOCIATION_STAT_IN_PACKETS_UNCHECKED,

     /**
      *@brief The sum of this count over Secure Associations gives 802.1ae statistics
      * inPacketsDelayed.
      * @condition
      * SAI_MACSEC_SECURE_ASSOCIATION_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_ASSOCIATION_STAT_IN_PACKETS_DELAYED,

     /**
      *@brief The sum of this count over Secure Associations gives 802.1ae statistics
      * inPacketsLate.
      * @condition
      * SAI_MACSEC_SECURE_ASSOCIATION_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_ASSOCIATION_STAT_IN_PACKETS_LATE,

     /**
      *@brief 802.1ae defined inPacketsInvalid.
      * @condition
      * SAI_MACSEC_SECURE_ASSOCIATION_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_ASSOCIATION_STAT_IN_PACKETS_INVALID,

     /**
      *@brief 802.1ae defined inPacketsNotValid.
      * @condition
      * SAI_MACSEC_SECURE_ASSOCIATION_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_ASSOCIATION_STAT_IN_PACKETS_NOT_VALID,

     /**
      *@brief 802.1ae defined inPacketsNotUsingSA.
      * @condition
      * SAI_MACSEC_SECURE_ASSOCIATION_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_ASSOCIATION_STAT_IN_PACKETS_NOT_USING_SA,

     /**
      *@brief 802.1ae defined inPacketsUnusedSA.
      * @condition
      * SAI_MACSEC_SECURE_ASSOCIATION_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_ASSOCIATION_STAT_IN_PACKETS_UNUSED_SA,

     /**
      *@brief 802.1ae defined inPacketsOk.
      * @condition
      * SAI_MACSEC_SECURE_ASSOCIATION_ATTR_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
      */
     SAI_MACSEC_SECURE_ASSOCIATION_STAT_IN_PACKETS_OK,
} sai_macsec_secure_association_stat_t;


/**
 * @brief Create a MACsec object
 *
 * @param[out] macsec_id The MACsec object id associated with this Phy
 * @param[in] switch_id The Phy Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_macsec_fn)(
        _Out_ sai_object_id_t *macsec_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete the MACsec object
 *
 * @param[in] macsec_id The MACsec object id associated with this Phy
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_macsec_fn)(
        _In_ sai_object_id_t macsec_id);

/**
 * @brief Set MACsec attribute
 *
 * @param[in] macsec_id The MACsec object id associated with this Phy
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_macsec_attribute_fn)(
        _In_ sai_object_id_t macsec_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get MACsec attribute
 *
 * @param[in] macsec_id The MACsec object id associated with this Phy
 * @param[in] attr_count Number of attributes
 * @param[out] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_attribute_fn)(
        _In_ sai_object_id_t macsec_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create a MACsec phy port
 *
 * @param[out] macsec_port_id The MACsec phy port id
 * @param[in] switch_id The Phy Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_macsec_port_fn)(
        _Out_ sai_object_id_t *macsec_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a MACsec phy port
 *
 * @param[in] macsec_port_id The MACsec phy port id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_macsec_port_fn)(
        _In_ sai_object_id_t macsec_port_id);

/**
 * @brief Set MACsec phy port attribute
 *
 * @param[in] macsec_port_id The MACsec phy port id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_macsec_port_attribute_fn)(
        _In_ sai_object_id_t macsec_port_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get MACsec phy port attribute
 *
 * @param[in] macsec_port_id MACsec phy port id
 * @param[in] attr_count Number of attributes
 * @param[out] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_port_attribute_fn)(
        _In_ sai_object_id_t macsec_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get MACSEC port counters extended
 *
 * @param[in] macsec_port_id MACSEC port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Should match SAI_MACSEC_ATTR_STATS_MODE
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_port_stats_ext_fn)(
        _In_ sai_object_id_t macsec_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear MACSEC port counters
 *
 * @param[in] macsec_port_id MACSEC port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_macsec_port_stats_fn)(
        _In_ sai_object_id_t macsec_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Create a MACSEC flow
 *
 * @param[out] macsec_flow_id The MACSEC flow id
 * @param[in] switch_id The Phy Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_macsec_flow_fn)(
        _Out_ sai_object_id_t *macsec_flow_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete an MACSEC flow
 *
 * @param[in] macsec_flow_id The MACSEC flow id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_macsec_flow_fn)(
        _In_ sai_object_id_t macsec_flow_id);

/**
 * @brief Set MACSEC flow attribute
 *
 * @param[in] macsec_flow_id The MACSEC flow id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_macsec_flow_attribute_fn)(
        _In_ sai_object_id_t macsec_flow_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get MACSEC flow attribute
 *
 * @param[in] macsec_flow_id MACSEC flow id
 * @param[in] attr_count Number of attributes
 * @param[out] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_flow_attribute_fn)(
        _In_ sai_object_id_t macsec_flow_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create a MACSEC rule
 *
 * @param[out] macsec_rule_id The MACSEC rule id
 * @param[in] switch_id The Phy Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_macsec_rule_fn)(
        _Out_ sai_object_id_t *macsec_rule_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a MACSEC rule
 *
 * @param[in] macsec_rule_id The MACSEC rule id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_macsec_rule_fn)(
        _In_ sai_object_id_t macsec_rule_id);

/**
 * @brief Set MACSEC rule attribute
 *
 * @param[in] macsec_rule_id The MACSEC rule id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_macsec_rule_attribute_fn)(
        _In_ sai_object_id_t macsec_rule_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get MACSEC rule attribute
 *
 * @param[in] macsec_rule_id MACSEC rule id
 * @param[in] attr_count Number of attributes
 * @param[out] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_rule_attribute_fn)(
        _In_ sai_object_id_t macsec_rule_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get MACSEC rule counters extended
 *
 * @param[in] macsec_rule_id MACSEC rule id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Should match SAI_MACSEC_ATTR_STATS_MODE
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_rule_stats_ext_fn)(
        _In_ sai_object_id_t macsec_rule_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear MACSEC rule counters
 *
 * @param[in] macsec_rule_id MACSEC rule id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_macsec_rule_stats_fn)(
        _In_ sai_object_id_t macsec_rule_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Create a MACSEC Secure Channel
 *
 * @param[out] macsec_secure_channel_id The MACSEC secure_channel id
 * @param[in] switch_id The Phy Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_macsec_secure_channel_fn)(
        _Out_ sai_object_id_t *macsec_secure_channel_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete an MACSEC Secure Channel
 *
 * @param[in] macsec_secure_channel_id The MACSEC secure_channel id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_macsec_secure_channel_fn)(
        _In_ sai_object_id_t macsec_secure_channel_id);

/**
 * @brief Set MACSEC Secure Channel attribute
 *
 * @param[in] macsec_secure_channel_id The MACSEC secure_channel id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_macsec_secure_channel_attribute_fn)(
        _In_ sai_object_id_t macsec_secure_channel_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get MACSEC Secure Channel attribute
 *
 * @param[in] macsec_secure_channel_id MACSEC secure_channel id
 * @param[in] attr_count Number of attributes
 * @param[out] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_secure_channel_attribute_fn)(
        _In_ sai_object_id_t macsec_secure_channel_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get MACSEC Secure Channel counters extended
 *
 * @param[in] macsec_secure_channel_id MACSEC Secure Channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Should match SAI_MACSEC_ATTR_STATS_MODE
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_secure_channel_stats_ext_fn)(
        _In_ sai_object_id_t macsec_secure_channel_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear MACSEC Secure Channel counters
 *
 * @param[in] macsec_secure_channel_id MACSEC Secure Channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_macsec_secure_channel_stats_fn)(
        _In_ sai_object_id_t macsec_secure_channel_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Create a MACSEC Secure Association
 *
 * @param[out] macsec_secure_association_id The MACSEC Secure Association id
 * @param[in] switch_id The Phy Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_macsec_secure_association_fn)(
        _Out_ sai_object_id_t *macsec_secure_association_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete an MACSEC Secure Association
 *
 * @param[in] macsec_secure_association_id The MACSEC Secure Association id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_macsec_secure_association_fn)(
        _In_ sai_object_id_t macsec_secure_association_id);

/**
 * @brief Set MACSEC Secure Association attribute
 *
 * @param[in] macsec_secure_association_id The MACSEC Secure Association id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_macsec_secure_association_attribute_fn)(
        _In_ sai_object_id_t macsec_secure_association_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get MACSEC Secure Association attribute
 *
 * @param[in] macsec_secure_association_id MACSEC Secure Association id
 * @param[in] attr_count Number of attributes
 * @param[out] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_secure_association_attribute_fn)(
        _In_ sai_object_id_t macsec_secure_association_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get MACSEC Secure Association counters extended
 *
 * @param[in] macsec_secure_association_id MACSEC Secure Association id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Should match SAI_MACSEC_ATTR_STATS_MODE
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_secure_association_stats_ext_fn)(
        _In_ sai_object_id_t macsec_secure_association_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear MACSEC Secure Association counters
 *
 * @param[in] macsec_secure_association_id MACSEC Secure Association id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_macsec_secure_association_stats_fn)(
        _In_ sai_object_id_t macsec_secure_association_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief MACSEC methods table retrieved with sai_api_query()
 */
typedef struct _sai_macsec_api_t
{
    sai_create_macsec_fn                            create_macsec;
    sai_remove_macsec_fn                            remove_macsec;
    sai_set_macsec_attribute_fn                     set_macsec_attribute;
    sai_get_macsec_attribute_fn                     get_macsec_attribute;
    sai_create_macsec_port_fn                       create_macsec_port;
    sai_remove_macsec_port_fn                       remove_macsec_port;
    sai_set_macsec_port_attribute_fn                set_macsec_port_attribute;
    sai_get_macsec_port_attribute_fn                get_macsec_port_attribute;
    sai_get_macsec_port_stats_ext_fn                get_macsec_port_stats_ext;
    sai_clear_macsec_port_stats_fn                  clear_macsec_port_stats;
    sai_create_macsec_flow_fn                       create_macsec_flow;
    sai_remove_macsec_flow_fn                       remove_macsec_flow;
    sai_set_macsec_flow_attribute_fn                set_macsec_flow_attribute;
    sai_get_macsec_flow_attribute_fn                get_macsec_flow_attribute;
    sai_create_macsec_rule_fn                       create_macsec_rule;
    sai_remove_macsec_rule_fn                       remove_macsec_rule;
    sai_set_macsec_rule_attribute_fn                set_macsec_rule_attribute;
    sai_get_macsec_rule_attribute_fn                get_macsec_rule_attribute;
    sai_get_macsec_rule_stats_ext_fn                get_macsec_rule_stats_ext;
    sai_clear_macsec_rule_stats_fn                  clear_macsec_rule_stats;
    sai_create_macsec_secure_channel_fn             create_macsec_secure_channel;
    sai_remove_macsec_secure_channel_fn             remove_macsec_secure_channel;
    sai_set_macsec_secure_channel_attribute_fn      set_macsec_secure_channel_attribute;
    sai_get_macsec_secure_channel_attribute_fn      get_macsec_secure_channel_attribute;
    sai_get_macsec_secure_channel_stats_ext_fn      get_macsec_secure_channel_stats_ext;
    sai_clear_macsec_secure_channel_stats_fn        clear_macsec_secure_channel_stats;
    sai_create_macsec_secure_association_fn         create_macsec_secure_association;
    sai_remove_macsec_secure_association_fn         remove_macsec_secure_association;
    sai_set_macsec_secure_association_attribute_fn  set_macsec_secure_association_attribute;
    sai_get_macsec_secure_association_attribute_fn  get_macsec_secure_association_attribute;
    sai_get_macsec_secure_association_stats_ext_fn  get_macsec_secure_association_stats_ext;
    sai_clear_macsec_secure_association_stats_fn    clear_macsec_secure_association_stats;
} sai_macsec_api_t;

/**
 * @}
 */
#endif /** __SAIMACSEC_H_ */
