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
 * @brief   This module defines SAI MACsec interface
 */

#if !defined (__SAIMACSEC_H_)
#define __SAIMACSEC_H_

#include <saitypes.h>

/**
 * @defgroup SAIMACSEC SAI - MACsec specific API definitions
 *
 * @{
 */

/**
 * @brief MACsec direction types
 * For PHY ASIC Egress is system to line direction and ingress is the opposite.
 */
typedef enum _sai_macsec_direction_t
{
    SAI_MACSEC_DIRECTION_EGRESS,
    SAI_MACSEC_DIRECTION_INGRESS,
} sai_macsec_direction_t;

/**
 * @brief Attribute Id for sai_macsec
 */
typedef enum _sai_macsec_attr_t
{
    /**
     * @brief Start of MACsec attributes
     */
    SAI_MACSEC_ATTR_START,

    /**
     * @brief MACsec direction
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
     * @brief Indicates if ingress can use SCI only as an ACL field.
     *
     * False means one 1 flow can be associated with multiple ACL entries
     * and multiple SC. True means SCI can only be used as ACL field i.e.
     * 1 ingress flow can be associated with only 1 ACL entry and 1 SC.
     * Valid only for ingress.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_SCI_IN_INGRESS_MACSEC_ACL,

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
     * @brief MACsec MTU capability on system side (not including MACsec overhead).
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
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
     * @brief When false disables creation of saimacsec objects during warm-boot.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_MACSEC_ATTR_WARM_BOOT_ENABLE,

    /**
     * @brief TPID value used to identify packet C-tag.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0x8100
     */
    SAI_MACSEC_ATTR_CTAG_TPID,

    /**
     * @brief TPID value used to identify packet S-tag.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0x88A8
     */
    SAI_MACSEC_ATTR_STAG_TPID,

    /**
     * @brief Maximum number of VLAN tags to parse.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_MACSEC_ATTR_MAX_VLAN_TAGS_PARSED,

    /**
     * @brief Global setting of read-clear or read-only for statistics read.
     * The mode parameter for get_macsec_<foo>_stats_ext should match this.
     *
     * @type sai_stats_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_MODE_READ_AND_CLEAR
     */
    SAI_MACSEC_ATTR_STATS_MODE,

    /**
     * @brief Enables physical bypass of MACsec module. Packets can physically
     * bypass MACsec module only when there is no macsec_port object
     * associated with and MACsec-capable port.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_MACSEC_ATTR_PHYSICAL_BYPASS_ENABLE,

    /**
     * @brief List of ports that can support MACsec
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_MACSEC_ATTR_SUPPORTED_PORT_LIST,

    /**
     * @brief Available MACsec flows.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_AVAILABLE_MACSEC_FLOW,

    /**
     * @brief List of MACsec flow associated with this MACsec object.
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_FLOW
     */
    SAI_MACSEC_ATTR_FLOW_LIST,

    /**
     * @brief Available MACsec Secure Channels.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_AVAILABLE_MACSEC_SC,

    /**
     * @brief Available MACsec Secure Associations.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_AVAILABLE_MACSEC_SA,

    /**
     * @brief End of MACsec attributes
     */
    SAI_MACSEC_ATTR_END,

    /**
     * @brief Custom range base value
     */
    SAI_MACSEC_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /**
     * @brief End of custom range base
     */
    SAI_MACSEC_ATTR_CUSTOM_RANGE_END
} sai_macsec_attr_t;

/**
 * @brief Attribute Id for sai_macsec_port
 */
typedef enum _sai_macsec_port_attr_t
{
    /**
     * @brief Start of MACsec Port attributes
     */
    SAI_MACSEC_PORT_ATTR_START,

    /**
     * @brief MACsec direction
     *
     * @type sai_macsec_direction_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_PORT_ATTR_MACSEC_DIRECTION = SAI_MACSEC_PORT_ATTR_START,

    /**
     * @brief Associated port id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_MACSEC_PORT_ATTR_PORT_ID,

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

    /**
     * @brief Custom range base value
     */
    SAI_MACSEC_PORT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /**
     * @brief End of custom range base
     */
    SAI_MACSEC_PORT_ATTR_CUSTOM_RANGE_END
} sai_macsec_port_attr_t;

/**
 * @brief MACsec port counter IDs in sai_get_macsec_stats() call
 */
typedef enum _sai_macsec_port_stat_t
{
    /**
     * @brief Malformed packets dropped before MACsec processing, not in 802.1ae MIB
     */
    SAI_MACSEC_PORT_STAT_PRE_MACSEC_DROP_PKTS,

    /**
     * @brief Packets classified as control packets for MACsec processing, not in 802.1ae MIB
     */
    SAI_MACSEC_PORT_STAT_CONTROL_PKTS,

    /**
     * @brief Packets classified as data packets for MACsec processing, not in 802.1ae MIB
     */
    SAI_MACSEC_PORT_STAT_DATA_PKTS,
} sai_macsec_port_stat_t;

/**
 * @brief Attribute Id for sai_macsec_flow
 */
typedef enum _sai_macsec_flow_attr_t
{
    /**
     * @brief Start of MACsec Flow attributes
     */
    SAI_MACSEC_FLOW_ATTR_START,

    /**
     * @brief MACsec direction
     *
     * @type sai_macsec_direction_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_FLOW_ATTR_MACSEC_DIRECTION = SAI_MACSEC_FLOW_ATTR_START,

    /**
     * @brief List of MACsec ACL entries associated with this flow.
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_ACL_ENTRY
     */
    SAI_MACSEC_FLOW_ATTR_ACL_ENTRY_LIST,

    /**
     * @brief List of MACsec Secure Channels associated with this flow.
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_SC
     */
    SAI_MACSEC_FLOW_ATTR_SC_LIST,

    /**
     * @brief End of MACsec Flow attributes
     */
    SAI_MACSEC_FLOW_ATTR_END,

    /**
     * @brief Custom range base value
     */
    SAI_MACSEC_FLOW_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /**
     * @brief End of custom range base
     */
    SAI_MACSEC_FLOW_ATTR_CUSTOM_RANGE_END
} sai_macsec_flow_attr_t;

/**
 * @brief MACsec flow counter IDs in sai_get_macsec_stats() call
 */
typedef enum _sai_macsec_flow_stat_t
{
    /**
     * @brief Packets dropped/error for In or Out interface before MACsec processing for controlled and uncontrolled port, not in 802.1ae MIB
     */
    SAI_MACSEC_FLOW_STAT_OTHER_ERR,

    /**
     * @brief IEEE 802.1ae defined ifOutOctets or ifInOctets for MACSEC uncontrolled port
     */
    SAI_MACSEC_FLOW_STAT_OCTETS_UNCONTROLLED,

    /**
     * @brief IEEE 802.1ae defined ifOutOctets or ifInOctets for MACSEC controlled port
     */
    SAI_MACSEC_FLOW_STAT_OCTETS_CONTROLLED,

    /**
     * @brief IEEE 802.1ae defined ifOutOctets for MACSEC common port
     */
    SAI_MACSEC_FLOW_STAT_OUT_OCTETS_COMMON,

    /**
     * @brief IEEE 802.1ae defined ifOutUcastPkts or ifInUcastPkts for MACSEC uncontrolled port
     */
    SAI_MACSEC_FLOW_STAT_UCAST_PKTS_UNCONTROLLED,

    /**
     * @brief IEEE 802.1ae defined ifOutUcastPkts or ifInUcastPkts for MACSEC controlled port
     */
    SAI_MACSEC_FLOW_STAT_UCAST_PKTS_CONTROLLED,

    /**
     * @brief IEEE 802.1ae defined ifOutMulticastPkts or ifInMulticastPkts for MACSEC uncontrolled port
     */
    SAI_MACSEC_FLOW_STAT_MULTICAST_PKTS_UNCONTROLLED,

    /**
     * @brief IEEE 802.1ae defined ifOutMulticastPkts or ifInMulticastPkts for MACSEC controlled port
     */
    SAI_MACSEC_FLOW_STAT_MULTICAST_PKTS_CONTROLLED,

    /**
     * @brief IEEE 802.1ae defined ifOutBroadcastPkts or ifInBroadcastPkts for MACSEC uncontrolled port
     */
    SAI_MACSEC_FLOW_STAT_BROADCAST_PKTS_UNCONTROLLED,

    /**
     * @brief IEEE 802.1ae defined ifOutBroadcastPkts or ifInBroadcastPkts for MACSEC controlled port
     */
    SAI_MACSEC_FLOW_STAT_BROADCAST_PKTS_CONTROLLED,

    /**
     * @brief Control packets which are not secured (using MACsec uncontrolled port) -
     * not in 802.1ae MIB
     */
    SAI_MACSEC_FLOW_STAT_CONTROL_PKTS,

    /**
     * @brief IEEE 802.1ae defined OutPktsUntagged or InPktsUntagged
     */
    SAI_MACSEC_FLOW_STAT_PKTS_UNTAGGED,

    /**
     * @brief Control packets with SecTAG which are not secured (using MACsec
     * uncontrolled port) - not in 802.1ae MIB
     */
    SAI_MACSEC_FLOW_STAT_IN_TAGGED_CONTROL_PKTS,

    /**
     * @brief IEEE 802.1ae defined OutPktsTooLong.
     * Valid for egress, always returns 0 for ingress.
     */
    SAI_MACSEC_FLOW_STAT_OUT_PKTS_TOO_LONG,

    /**
     * @brief IEEE 802.1ae defined InPktsNoTag.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_FLOW_STAT_IN_PKTS_NO_TAG,

    /**
     * @brief IEEE 802.1ae defined InPktsBadTag.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_FLOW_STAT_IN_PKTS_BAD_TAG,

    /**
     * @brief IEEE 802.1ae defined InPktsNoSci.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_FLOW_STAT_IN_PKTS_NO_SCI,

    /**
     * @brief IEEE 802.1ae defined InPktsUnknownSci.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_FLOW_STAT_IN_PKTS_UNKNOWN_SCI,

    /**
     * @brief IEEE 802.1ae defined InPktsOverrun.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_FLOW_STAT_IN_PKTS_OVERRUN,
} sai_macsec_flow_stat_t;

/**
 * @brief Attribute Id for sai_macsec_sc
 */
typedef enum _sai_macsec_sc_attr_t
{
    /**
     * @brief Start of MACsec Secure Channel attributes
     */
    SAI_MACSEC_SC_ATTR_START,

    /**
     * @brief MACsec direction
     *
     * @type sai_macsec_direction_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SC_ATTR_MACSEC_DIRECTION = SAI_MACSEC_SC_ATTR_START,

    /**
     * @brief MACsec flow object id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_FLOW
     */
    SAI_MACSEC_SC_ATTR_FLOW_ID,

    /**
     * @brief SCI value for this Secure Channel, carried in MACsec packet SecTAG.
     *
     * @type sai_uint64_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SC_ATTR_MACSEC_SCI,

    /**
     * @brief SSCI value for this Secure Channel
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_MACSEC_SC_ATTR_MACSEC_XPN64_ENABLE == true
     */
    SAI_MACSEC_SC_ATTR_MACSEC_SSCI,

    /**
     * @brief Enable 64-bit XPN (vs 32-bit PN) for this Secure Channel
     *
     * @type bool
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SC_ATTR_MACSEC_XPN64_ENABLE,

    /**
     * @brief Explicit SCI enable for this Secure Channel.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_MACSEC_SC_ATTR_MACSEC_DIRECTION == SAI_MACSEC_DIRECTION_EGRESS
     */
    SAI_MACSEC_SC_ATTR_MACSEC_EXPLICIT_SCI_ENABLE,

    /**
     * @brief SecTAG offset for this Secure Channel with respect to 802.1ae
     * standard SecTAG location
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_MACSEC_SC_ATTR_MACSEC_SECTAG_OFFSET,

    /**
     * @brief Active MACsec SA identifier.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_SA
     */
    SAI_MACSEC_SC_ATTR_ACTIVE_EGRESS_SA_ID,

    /**
     * @brief Replay protection enable for this Secure Channel.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     * @validonly SAI_MACSEC_SC_ATTR_MACSEC_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
     */
    SAI_MACSEC_SC_ATTR_MACSEC_REPLAY_PROTECTION_ENABLE,

    /**
     * @brief Replay protection window for this Secure Channel.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_MACSEC_SC_ATTR_MACSEC_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
     */
    SAI_MACSEC_SC_ATTR_MACSEC_REPLAY_PROTECTION_WINDOW,

    /**
     * @brief MACsec SA list
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_SA
     */
    SAI_MACSEC_SC_ATTR_SA_LIST,

    /**
     * @brief End of MACsec Secure Channel attributes
     */
    SAI_MACSEC_SC_ATTR_END,

    /**
     * @brief Custom range base value
     */
    SAI_MACSEC_SC_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /**
     * @brief End of custom range base
     */
    SAI_MACSEC_SC_ATTR_CUSTOM_RANGE_END
} sai_macsec_sc_attr_t;

/**
 * @brief MACsec Secure Channel counter IDs in sai_get_macsec_sc_stats() call
 */
typedef enum _sai_macsec_sc_stat_t
{
    /**
     * @brief Packets that have valid SCI, but the AN value does not have associated SA -
     * not in 802.1ae MIB.
     * Provides the sum of InPktsNotUsingSA for all invalid SAs for a SC
     */
    SAI_MACSEC_SC_STAT_SA_NOT_IN_USE,
} sai_macsec_sc_stat_t;

/**
 * @brief Attribute Id for sai_macsec_sa
 */
typedef enum _sai_macsec_sa_attr_t
{
    /**
     * @brief Start of MACsec Secure Association attributes
     */
    SAI_MACSEC_SA_ATTR_START,

    /**
     * @brief MACsec direction
     *
     * @type sai_macsec_direction_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SA_ATTR_MACSEC_DIRECTION = SAI_MACSEC_SA_ATTR_START,

    /**
     * @brief MACSEC Secure Channel object id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_SC
     */
    SAI_MACSEC_SA_ATTR_SC_ID,

    /**
     * @brief AN value (2-bit) for this Secure Channel, carried in MACsec packet SecTAG.
     * The value must be distinct from other Secure Associations for the same Secure Channel.
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SA_ATTR_AN,

    /**
     * @brief True means encryption is enabled.  False means encryption is disabled.
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default true
     */
    SAI_MACSEC_SA_ATTR_ENCRYPTION_ENABLE,

    /**
     * @brief True means 256-bit SAK (encryption key).  False means 128-bit key.
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default true
     */
    SAI_MACSEC_SA_ATTR_SAK_256_BITS,

    /**
     * @brief MACsec SAK (Secure Association Key) used for encryption/decryption.
     * Network Byte order. 128-bit SAK uses only Bytes 8..15.
     *
     * @type sai_macsec_sak_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SA_ATTR_SAK,

    /**
     * @brief MACsec Salt used for encryption/decryption.
     * Network Byte order.
     *
     * Valid when SAI_MACSEC_SC_ATTR_MACSEC_XPN64_ENABLE == true.
     *
     * @type sai_macsec_salt_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SA_ATTR_SALT,

    /**
     * @brief MACsec Authentication Key
     * Network Byte order.
     *
     * @type sai_macsec_auth_key_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MACSEC_SA_ATTR_AUTH_KEY,

    /**
     * @brief MACsec egress packet number (PN/XPN).  At most 1 less than the next PN/XPN.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_MACSEC_SA_ATTR_MACSEC_DIRECTION == SAI_MACSEC_DIRECTION_EGRESS
     */
    SAI_MACSEC_SA_ATTR_XPN,

    /**
     * @brief Minimum value of ingress MACsec packet number (PN/XPN).
     * Updated by value from MACsec peer by Key Agreement protocol.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 1
     * @validonly SAI_MACSEC_SA_ATTR_MACSEC_DIRECTION == SAI_MACSEC_DIRECTION_INGRESS
     */
    SAI_MACSEC_SA_ATTR_MINIMUM_XPN,

    /**
     * @brief End of MACsec Secure Association attributes
     */
    SAI_MACSEC_SA_ATTR_END,

    /**
     * @brief Custom range base value
     */
    SAI_MACSEC_SA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /**
     * @brief End of custom range base
     */
    SAI_MACSEC_SA_ATTR_CUSTOM_RANGE_END
} sai_macsec_sa_attr_t;

/**
 * @brief MACsec flow counter IDs in sai_get_macsec_sa_stats() call.
 * Some of these counters appear as per Secure Channel counters in 802.1ae MIB.
 * The application (NOS) has to add these per Secure Association counters to
 * get the per Secure Channel value.
 */
typedef enum _sai_macsec_sa_stat_t
{
    /**
     * @brief The sum of this count over all Secure Associations of a Secure
     * Channel gives 802.1ae statistics outOctetsEncrypted for egress and
     * inOctetsDecrypted for ingress
     */
    SAI_MACSEC_SA_STAT_OCTETS_ENCRYPTED,

    /**
     * @brief The sum of this count over Secure Associations gives 802.1ae
     * statistics outOctetsProtected for egress and inOctetsValidated for ingress
     */
    SAI_MACSEC_SA_STAT_OCTETS_PROTECTED,

    /**
     * @brief The sum of this count over Secure Associations gives 802.1ae
     * statistics OutPktsEncrypted.
     * Valid for egress, always returns 0 for ingress.
     */
    SAI_MACSEC_SA_STAT_OUT_PKTS_ENCRYPTED,

    /**
     * @brief The sum of this count over Secure Associations gives 802.1ae
     * statistics OutPktsProtected.
     * Valid for egress, always returns 0 for ingress.
     */
    SAI_MACSEC_SA_STAT_OUT_PKTS_PROTECTED,

    /**
     * @brief The sum of this count over Secure Associations gives 802.1ae
     * statistics InPktsUnchecked.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_SA_STAT_IN_PKTS_UNCHECKED,

    /**
     * @brief The sum of this count over Secure Associations gives 802.1ae
     * statistics InPktsDelayed.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_SA_STAT_IN_PKTS_DELAYED,

    /**
     * @brief The sum of this count over Secure Associations gives 802.1ae
     * statistics InPktsLate.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_SA_STAT_IN_PKTS_LATE,

    /**
     * @brief IEEE 802.1ae defined InPktsInvalid.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_SA_STAT_IN_PKTS_INVALID,

    /**
     * @brief IEEE 802.1ae defined InPktsNotValid.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_SA_STAT_IN_PKTS_NOT_VALID,

    /**
     * @brief IEEE 802.1ae defined InPktsNotUsingSA.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_SA_STAT_IN_PKTS_NOT_USING_SA,

    /**
     * @brief IEEE 802.1ae defined InPktsUnusedSA.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_SA_STAT_IN_PKTS_UNUSED_SA,

    /**
     * @brief IEEE 802.1ae defined InPktsOk.
     * Valid for ingress, always returns 0 for egress.
     */
    SAI_MACSEC_SA_STAT_IN_PKTS_OK,
} sai_macsec_sa_stat_t;

/**
 * @brief Create a MACsec object
 *
 * @param[out] macsec_id The MACsec object id associated with this switch/PHY
 * @param[in] switch_id The switch/PHY Object id
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
 * @param[in] macsec_id The MACsec object id associated with this switch/PHY
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_macsec_fn)(
        _In_ sai_object_id_t macsec_id);

/**
 * @brief Set MACsec attribute
 *
 * @param[in] macsec_id The MACsec object id associated with this switch/PHY
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
 * @param[in] macsec_id The MACsec object id associated with this switch/PHY
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_attribute_fn)(
        _In_ sai_object_id_t macsec_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create a MACsec port
 *
 * @param[out] macsec_port_id The MACsec port id
 * @param[in] switch_id The switch/PHY Object id
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
 * @brief Delete a MACsec port
 *
 * @param[in] macsec_port_id The MACsec port id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_macsec_port_fn)(
        _In_ sai_object_id_t macsec_port_id);

/**
 * @brief Set MACsec port attribute
 *
 * @param[in] macsec_port_id The MACsec port id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_macsec_port_attribute_fn)(
        _In_ sai_object_id_t macsec_port_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get MACsec port attribute
 *
 * @param[in] macsec_port_id MACsec port id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_port_attribute_fn)(
        _In_ sai_object_id_t macsec_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get MACsec port counters
 *
 * @param[in] macsec_port_id MACsec port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_port_stats_fn)(
        _In_ sai_object_id_t macsec_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get MACsec port counters extended
 *
 * @param[in] macsec_port_id MACsec port id
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
 * @brief Clear MACsec port counters
 *
 * @param[in] macsec_port_id MACsec port id
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
 * @brief Create a MACsec flow
 *
 * @param[out] macsec_flow_id The MACsec flow id
 * @param[in] switch_id The switch/PHY Object id
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
 * @brief Delete a MACsec flow
 *
 * @param[in] macsec_flow_id The MACsec flow id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_macsec_flow_fn)(
        _In_ sai_object_id_t macsec_flow_id);

/**
 * @brief Set MACsec flow attribute
 *
 * @param[in] macsec_flow_id The MACsec flow id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_macsec_flow_attribute_fn)(
        _In_ sai_object_id_t macsec_flow_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get MACsec flow attribute
 *
 * @param[in] macsec_flow_id MACsec flow id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_flow_attribute_fn)(
        _In_ sai_object_id_t macsec_flow_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get MACsec flow counters
 *
 * @param[in] macsec_flow_id MACsec flow id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_flow_stats_fn)(
        _In_ sai_object_id_t macsec_flow_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get MACsec flow counters extended
 *
 * @param[in] macsec_flow_id MACsec flow id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Should match SAI_MACSEC_ATTR_STATS_MODE
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_flow_stats_ext_fn)(
        _In_ sai_object_id_t macsec_flow_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear MACsec flow counters
 *
 * @param[in] macsec_flow_id MACsec flow id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_macsec_flow_stats_fn)(
        _In_ sai_object_id_t macsec_flow_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Create a MACsec Secure Channel
 *
 * @param[out] macsec_sc_id The MACsec Secure Channel id
 * @param[in] switch_id The switch/PHY Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_macsec_sc_fn)(
        _Out_ sai_object_id_t *macsec_sc_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a MACsec Secure Channel
 *
 * @param[in] macsec_sc_id The MACsec Secure Channel id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_macsec_sc_fn)(
        _In_ sai_object_id_t macsec_sc_id);

/**
 * @brief Set MACsec Secure Channel attribute
 *
 * @param[in] macsec_sc_id The MACsec Secure Channel id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_macsec_sc_attribute_fn)(
        _In_ sai_object_id_t macsec_sc_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get MACsec Secure Channel attribute
 *
 * @param[in] macsec_sc_id MACsec Secure Channel id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_sc_attribute_fn)(
        _In_ sai_object_id_t macsec_sc_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get MACsec Secure Channel counters
 *
 * @param[in] macsec_sc_id MACsec Secure Channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_sc_stats_fn)(
        _In_ sai_object_id_t macsec_sc_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get MACsec Secure Channel counters extended
 *
 * @param[in] macsec_sc_id MACsec Secure Channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Should match SAI_MACSEC_ATTR_STATS_MODE
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_sc_stats_ext_fn)(
        _In_ sai_object_id_t macsec_sc_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear MACsec Secure Channel counters
 *
 * @param[in] macsec_sc_id MACsec Secure Channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_macsec_sc_stats_fn)(
        _In_ sai_object_id_t macsec_sc_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Create a MACsec Secure Association
 *
 * @param[out] macsec_sa_id The MACsec Secure Association id
 * @param[in] switch_id The switch/PHY Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_macsec_sa_fn)(
        _Out_ sai_object_id_t *macsec_sa_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a MACsec Secure Association
 *
 * @param[in] macsec_sa_id The MACsec Secure Association id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_macsec_sa_fn)(
        _In_ sai_object_id_t macsec_sa_id);

/**
 * @brief Set MACsec Secure Association attribute
 *
 * @param[in] macsec_sa_id The MACsec Secure Association id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_macsec_sa_attribute_fn)(
        _In_ sai_object_id_t macsec_sa_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get MACsec Secure Association attribute
 *
 * @param[in] macsec_sa_id MACsec Secure Association id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_sa_attribute_fn)(
        _In_ sai_object_id_t macsec_sa_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get MACsec Secure Association counters
 *
 * @param[in] macsec_sa_id MACsec Secure Association id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_sa_stats_fn)(
        _In_ sai_object_id_t macsec_sa_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get MACsec Secure Association counters extended
 *
 * @param[in] macsec_sa_id MACsec Secure Association id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Should match SAI_MACSEC_ATTR_STATS_MODE
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_macsec_sa_stats_ext_fn)(
        _In_ sai_object_id_t macsec_sa_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear MACsec Secure Association counters
 *
 * @param[in] macsec_sa_id MACsec Secure Association id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_macsec_sa_stats_fn)(
        _In_ sai_object_id_t macsec_sa_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief MACsec methods table retrieved with sai_api_query()
 */
typedef struct _sai_macsec_api_t
{
    sai_create_macsec_fn                create_macsec;
    sai_remove_macsec_fn                remove_macsec;
    sai_set_macsec_attribute_fn         set_macsec_attribute;
    sai_get_macsec_attribute_fn         get_macsec_attribute;
    sai_create_macsec_port_fn           create_macsec_port;
    sai_remove_macsec_port_fn           remove_macsec_port;
    sai_set_macsec_port_attribute_fn    set_macsec_port_attribute;
    sai_get_macsec_port_attribute_fn    get_macsec_port_attribute;
    sai_get_macsec_port_stats_fn        get_macsec_port_stats;
    sai_get_macsec_port_stats_ext_fn    get_macsec_port_stats_ext;
    sai_clear_macsec_port_stats_fn      clear_macsec_port_stats;
    sai_create_macsec_flow_fn           create_macsec_flow;
    sai_remove_macsec_flow_fn           remove_macsec_flow;
    sai_set_macsec_flow_attribute_fn    set_macsec_flow_attribute;
    sai_get_macsec_flow_attribute_fn    get_macsec_flow_attribute;
    sai_get_macsec_flow_stats_fn        get_macsec_flow_stats;
    sai_get_macsec_flow_stats_ext_fn    get_macsec_flow_stats_ext;
    sai_clear_macsec_flow_stats_fn      clear_macsec_flow_stats;
    sai_create_macsec_sc_fn             create_macsec_sc;
    sai_remove_macsec_sc_fn             remove_macsec_sc;
    sai_set_macsec_sc_attribute_fn      set_macsec_sc_attribute;
    sai_get_macsec_sc_attribute_fn      get_macsec_sc_attribute;
    sai_get_macsec_sc_stats_fn          get_macsec_sc_stats;
    sai_get_macsec_sc_stats_ext_fn      get_macsec_sc_stats_ext;
    sai_clear_macsec_sc_stats_fn        clear_macsec_sc_stats;
    sai_create_macsec_sa_fn             create_macsec_sa;
    sai_remove_macsec_sa_fn             remove_macsec_sa;
    sai_set_macsec_sa_attribute_fn      set_macsec_sa_attribute;
    sai_get_macsec_sa_attribute_fn      get_macsec_sa_attribute;
    sai_get_macsec_sa_stats_fn          get_macsec_sa_stats;
    sai_get_macsec_sa_stats_ext_fn      get_macsec_sa_stats_ext;
    sai_clear_macsec_sa_stats_fn        clear_macsec_sa_stats;
} sai_macsec_api_t;

/**
 * @}
 */
#endif /** __SAIMACSEC_H_ */
