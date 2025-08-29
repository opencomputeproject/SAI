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
 * @file    saiipsec.h
 *
 * @brief   This module defines SAI IPsec interface
 */

#if !defined (__SAIIPSEC_H_)
#define __SAIIPSEC_H_

#include <saitypes.h>

/**
 * @defgroup SAIIPSEC SAI - IPsec specific API definitions
 *
 * @{
 */

/**
 * @brief IPsec direction types
 * For PHY ASIC Egress is system to line direction and ingress is the opposite.
 */
typedef enum _sai_ipsec_direction_t
{
    SAI_IPSEC_DIRECTION_EGRESS,
    SAI_IPSEC_DIRECTION_INGRESS,
} sai_ipsec_direction_t;

/**
 * @brief IPsec cipher suite types
 */
typedef enum _sai_ipsec_cipher_t
{
    SAI_IPSEC_CIPHER_AES128_GCM16,
    SAI_IPSEC_CIPHER_AES256_GCM16,
    SAI_IPSEC_CIPHER_AES128_GMAC,
    SAI_IPSEC_CIPHER_AES256_GMAC,
} sai_ipsec_cipher_t;

/**
 * @brief IPsec SA sequence number status type
 */
typedef enum _sai_ipsec_sa_octet_count_status_t
{
    /** SA byte count below lower of 2 watermarks */
    SAI_IPSEC_SA_OCTET_COUNT_STATUS_BELOW_LOW_WATERMARK,

    /** SA byte count below higher of 2 watermarks */
    SAI_IPSEC_SA_OCTET_COUNT_STATUS_BELOW_HIGH_WATERMARK,

    /** SA byte count above higher of 2 watermarks */
    SAI_IPSEC_SA_OCTET_COUNT_STATUS_ABOVE_HIGH_WATERMARK,

} sai_ipsec_sa_octet_count_status_t;

/**
 * @brief IPsec SA status for notification
 */
typedef struct _sai_ipsec_sa_status_notification_t
{
    /**
     * @brief IPsec SA object id
     *
     * @objects SAI_OBJECT_TYPE_IPSEC_SA
     */
    sai_object_id_t ipsec_sa_id;

    /**
     * @brief IPsec SA byte count status
     */
    sai_ipsec_sa_octet_count_status_t ipsec_sa_octet_count_status;

    /**
     * @brief IPsec egress SA sequence number at max limit
     */
    bool ipsec_egress_sn_at_max_limit;

} sai_ipsec_sa_status_notification_t;

/**
 * @brief Attribute data for #SAI_IPSEC_ATTR_POST_STATUS,
 */
typedef enum _sai_ipsec_post_status_t
{
    SAI_IPSEC_POST_STATUS_UNKNOWN,

    SAI_IPSEC_POST_STATUS_PASS,

    SAI_IPSEC_POST_STATUS_IN_PROGRESS,

    SAI_IPSEC_POST_STATUS_FAIL,
} sai_ipsec_post_status_t;

/**
 * @brief IPSEC post status notification
 *
 * @objects switch_id SAI_OBJECT_TYPE_IPSEC
 *
 * @param[in] ipsec_id IPSEC Id
 * @param[in] ipsec_post_status IPSEC post status
 */
typedef void (*sai_ipsec_post_status_notification_fn)(
        _In_ sai_object_id_t ipsec_id,
        _In_ sai_ipsec_post_status_t ipsec_post_status);

/**
 * @brief Attribute Id for sai_ipsec
 */
typedef enum _sai_ipsec_attr_t
{
    /**
     * @brief Start of IPsec attributes
     */
    SAI_IPSEC_ATTR_START,

    /**
     * @brief Security Engine supports matching source IP address for tunnel termination.
     *
     * If false, source IP address cannot be checked before decryption.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_IPSEC_ATTR_TERM_REMOTE_IP_MATCH_SUPPORTED = SAI_IPSEC_ATTR_START,

    /**
     * @brief SAI_SWITCH_SWITCHING_MODE_CUT_THROUGH supported
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_IPSEC_ATTR_SWITCHING_MODE_CUT_THROUGH_SUPPORTED,

    /**
     * @brief SAI_SWITCH_SWITCHING_MODE_STORE_AND_FORWARD supported
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_IPSEC_ATTR_SWITCHING_MODE_STORE_AND_FORWARD_SUPPORTED,

    /**
     * @brief SAI_STATS_MODE_READ supported
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_IPSEC_ATTR_STATS_MODE_READ_SUPPORTED,

    /**
     * @brief SAI_STATS_MODE_READ_CLEAR supported
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_IPSEC_ATTR_STATS_MODE_READ_CLEAR_SUPPORTED,

    /**
     * @brief Indicates if 32-bit Sequence Number (SN) is supported.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_IPSEC_ATTR_SN_32BIT_SUPPORTED,

    /**
     * @brief Indicates if 64-bit Extended Sequence Number (ESN) is supported.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_IPSEC_ATTR_ESN_64BIT_SUPPORTED,

    /**
     * @brief List of supported cipher suites
     *
     * @type sai_s32_list_t sai_ipsec_cipher_t
     * @flags READ_ONLY
     */
    SAI_IPSEC_ATTR_SUPPORTED_CIPHER_LIST,

    /**
     * @brief IPsec MTU capability on system side (not including IPsec overhead).
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_IPSEC_ATTR_SYSTEM_SIDE_MTU,

    /**
     * @brief Warm boot is supported for all saiipsec objects.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_IPSEC_ATTR_WARM_BOOT_SUPPORTED,

    /**
     * @brief If false, disables creation of saiipsec objects during warm-boot.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_IPSEC_ATTR_WARM_BOOT_ENABLE,

    /**
     * @brief If true, SA Index is assigned by NOS.
     * If false, SA Index is assigned by IPsec SAI driver.
     *
     * @type bool
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPSEC_ATTR_EXTERNAL_SA_INDEX_ENABLE,

    /**
     * @brief TPID value used to identify C-tag.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0x8100
     */
    SAI_IPSEC_ATTR_CTAG_TPID,

    /**
     * @brief TPID value used to identify S-tag.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0x88A8
     */
    SAI_IPSEC_ATTR_STAG_TPID,

    /**
     * @brief Maximum number of VLAN tags to parse.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_IPSEC_ATTR_MAX_VLAN_TAGS_PARSED,

    /**
     * @brief High watermark for byte count.
     *
     * The sai_ipsec_sa_status changes when a new packet is processed and the per
     * SA octet count crosses this watermark. This watermark is used even if only
     * 1 watermark is needed.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_IPSEC_ATTR_OCTET_COUNT_HIGH_WATERMARK,

    /**
     * @brief Low watermark for byte count
     *
     * The sai_ipsec_sa_status changes when a new packet is processed and the per
     * SA octet count crosses this watermark. This watermark is used only if 2
     * watermarks are needed.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_IPSEC_ATTR_OCTET_COUNT_LOW_WATERMARK,

    /**
     * @brief Global setting of read-clear or read-only for statistics read.
     * The mode parameter for get_ipsec_<foo>_stats_ext should match this.
     *
     * @type sai_stats_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_MODE_READ_AND_CLEAR
     */
    SAI_IPSEC_ATTR_STATS_MODE,

    /**
     * @brief Available IPsec Security Associations.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_IPSEC_ATTR_AVAILABLE_IPSEC_SA,

    /**
     * @brief IPsec SA list
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_IPSEC_SA
     */
    SAI_IPSEC_ATTR_SA_LIST,

    /**
     * @brief IPSEC POST status
     * Attribute to query the status of POST for an IPSEC engine
     *
     * @type sai_ipsec_post_status_t
     * @flags READ_ONLY
     */
    SAI_IPSEC_ATTR_POST_STATUS,

    /**
     * @brief Setting the value to true will start the post on all the ports serviced by this IPSEC engine
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_IPSEC_ATTR_ENABLE_POST,

    /**
     * @brief End of IPsec attributes
     */
    SAI_IPSEC_ATTR_END,

    /**
     * @brief Custom range base value
     */
    SAI_IPSEC_ATTR_CUSTOM_RANGE_BASE = 0x10000000
} sai_ipsec_attr_t;

/**
 * @brief Attribute data for #SAI_IPSEC_PORT_ATTR_POST_STATUS
 */
typedef enum _sai_ipsec_port_post_status_t
{
    SAI_IPSEC_PORT_POST_STATUS_UNKNOWN,

    SAI_IPSEC_PORT_POST_STATUS_PASS,

    SAI_IPSEC_PORT_POST_STATUS_FAIL,

} sai_ipsec_port_post_status_t;

/**
 * @brief Attribute Id for sai_ipsec_port
 */
typedef enum _sai_ipsec_port_attr_t
{
    /**
     * @brief Start of IPsec Port attributes
     */
    SAI_IPSEC_PORT_ATTR_START,

    /**
     * @brief Associated port id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_IPSEC_PORT_ATTR_PORT_ID = SAI_IPSEC_PORT_ATTR_START,

    /**
     * @brief Enable vlan tag parsing for C-tag TPID
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_IPSEC_PORT_ATTR_CTAG_ENABLE,

    /**
     * @brief Enable vlan tag parsing for S-tag TPID
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_IPSEC_PORT_ATTR_STAG_ENABLE,

    /**
     * @brief Port native Vlan Id used for Security Engine SA termination.
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan true
     */
    SAI_IPSEC_PORT_ATTR_NATIVE_VLAN_ID,

    /**
     * @brief Enable VRF identification from ingress parsed packet Vlan.
     *
     * False means only port native Vlan can be used for tunnel termination VRF.
     * True means packet Vlan tag is also used.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_IPSEC_PORT_ATTR_VRF_FROM_PACKET_VLAN_ENABLE,

    /**
     * @brief Switching mode for port. If configured as cut-through, the IPG
     * for Tx MAC in the switch ASIC has to be increased to accommodate the
     * IPsec packet size expansion.
     *
     * @type sai_switch_switching_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_SWITCH_SWITCHING_MODE_CUT_THROUGH
     */
    SAI_IPSEC_PORT_ATTR_SWITCH_SWITCHING_MODE,

    /**
     * @brief Set IPSEC port statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_IPSEC_PORT_ATTR_STATS_COUNT_MODE,

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
    SAI_IPSEC_PORT_ATTR_SELECTIVE_COUNTER_LIST,

    /**
     * @brief IPSEC Port POST completion status
     *
     * Attribute to query the status of POST for a IPSEC port
     *
     * @type sai_ipsec_port_post_status_t
     * @flags READ_ONLY
     */
    SAI_IPSEC_PORT_ATTR_POST_STATUS,

    /**
     * @brief End of IPsec Port attributes
     */
    SAI_IPSEC_PORT_ATTR_END,

    /**
     * @brief Custom range base value
     */
    SAI_IPSEC_PORT_ATTR_CUSTOM_RANGE_BASE = 0x10000000
} sai_ipsec_port_attr_t;

/**
 * @brief IPsec flow counter IDs in sai_get_ipsec_sa_stats() call.
 */
typedef enum _sai_ipsec_port_stat_t
{
    /**
     * @brief Packets dropped after receive MAC and before IPsec SA processing.
     * This could be due to malformed header, buffer overrun, etc
     */
    SAI_IPSEC_PORT_STAT_TX_ERROR_PKTS,

    /**
     * @brief Packets mapped to an SA for IPsec processing.
     */
    SAI_IPSEC_PORT_STAT_TX_IPSEC_PKTS,

    /**
     * @brief Non-IPsec packets that pass through this port.
     */
    SAI_IPSEC_PORT_STAT_TX_NON_IPSEC_PKTS,

    /**
     * @brief Packets dropped after receive MAC and before IPsec SA processing.
     * This could be due to malformed header, buffer overrun, etc
     */
    SAI_IPSEC_PORT_STAT_RX_ERROR_PKTS,

    /**
     * @brief Packets mapped to an SA for IPsec processing.
     */
    SAI_IPSEC_PORT_STAT_RX_IPSEC_PKTS,

    /**
     * @brief Non-IPsec packets that pass through this port.
     */
    SAI_IPSEC_PORT_STAT_RX_NON_IPSEC_PKTS,
} sai_ipsec_port_stat_t;

/**
 * @brief Attribute Id for sai_ipsec_sa
 */
typedef enum _sai_ipsec_sa_attr_t
{
    /**
     * @brief Start of IPsec Security Association attributes
     */
    SAI_IPSEC_SA_ATTR_START,

    /**
     * @brief IPsec direction
     *
     * @type sai_ipsec_direction_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPSEC_SA_ATTR_IPSEC_DIRECTION = SAI_IPSEC_SA_ATTR_START,

    /**
     * @brief IPsec object id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_IPSEC
     */
    SAI_IPSEC_SA_ATTR_IPSEC_ID,

    /**
     * @brief SA byte count status.
     *
     * @type sai_ipsec_sa_octet_count_status_t
     * @flags READ_ONLY
     */
    SAI_IPSEC_SA_ATTR_OCTET_COUNT_STATUS,

    /**
     * @brief Externally assigned SA Index value for this Security Association.
     * Used only when SAI_IPSEC_ATTR_EXTERNAL_SA_INDEX_ENABLE == true.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_IPSEC_SA_ATTR_EXTERNAL_SA_INDEX,

    /**
     * @brief SA Index value for this Security Association.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_IPSEC_SA_ATTR_SA_INDEX,

    /**
     * @brief List of IPsec ports for this SA.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_IPSEC_PORT
     * @default empty
     */
    SAI_IPSEC_SA_ATTR_IPSEC_PORT_LIST,

    /**
     * @brief SPI value for this Security Association, carried in ESP header.
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPSEC_SA_ATTR_IPSEC_SPI,

    /**
     * @brief Enable 64-bit ESN (vs 32-bit SN) for this Security Association
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default true
     */
    SAI_IPSEC_SA_ATTR_IPSEC_ESN_ENABLE,

    /**
     * @brief Cipher suite for this SA.
     *
     * @type sai_ipsec_cipher_t
     * @flags CREATE_ONLY
     * @default SAI_IPSEC_CIPHER_AES256_GCM16
     */
    SAI_IPSEC_SA_ATTR_IPSEC_CIPHER,

    /**
     * @brief IPsec Traffic Encryption Key used for encryption/decryption.
     * Network Byte order. AES128 uses only Bytes 16..31.
     *
     * @type sai_encrypt_key_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPSEC_SA_ATTR_ENCRYPT_KEY,

    /**
     * @brief IPsec Salt portion of IV
     * Network Byte order.
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPSEC_SA_ATTR_SALT,

    /**
     * @brief IPsec Authentication Key
     * Network Byte order.
     *
     * @type sai_auth_key_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPSEC_SA_ATTR_AUTH_KEY,

    /**
     * @brief Replay protection enable for this Security Association.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_IPSEC_SA_ATTR_IPSEC_DIRECTION == SAI_IPSEC_DIRECTION_INGRESS
     */
    SAI_IPSEC_SA_ATTR_IPSEC_REPLAY_PROTECTION_ENABLE,

    /**
     * @brief Replay protection window for this Security Association.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_IPSEC_SA_ATTR_IPSEC_DIRECTION == SAI_IPSEC_DIRECTION_INGRESS
     */
    SAI_IPSEC_SA_ATTR_IPSEC_REPLAY_PROTECTION_WINDOW,

    /**
     * @brief SA local IP address for tunnel termination.
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_IPSEC_SA_ATTR_IPSEC_DIRECTION == SAI_IPSEC_DIRECTION_INGRESS
     */
    SAI_IPSEC_SA_ATTR_TERM_DST_IP,

    /**
     * @brief Match Vlan Id for tunnel termination.
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     * @validonly SAI_IPSEC_SA_ATTR_IPSEC_DIRECTION == SAI_IPSEC_DIRECTION_INGRESS
     */
    SAI_IPSEC_SA_ATTR_TERM_VLAN_ID_ENABLE,

    /**
     * @brief Vlan Id for tunnel termination.
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan true
     * @condition SAI_IPSEC_SA_ATTR_IPSEC_DIRECTION == SAI_IPSEC_DIRECTION_INGRESS and SAI_IPSEC_SA_ATTR_TERM_VLAN_ID_ENABLE == true
     */
    SAI_IPSEC_SA_ATTR_TERM_VLAN_ID,

    /**
     * @brief Match remote IP address for tunnel termination.
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     * @validonly SAI_IPSEC_SA_ATTR_IPSEC_DIRECTION == SAI_IPSEC_DIRECTION_INGRESS
     */
    SAI_IPSEC_SA_ATTR_TERM_SRC_IP_ENABLE,

    /**
     * @brief Remote IP address for tunnel termination.
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_IPSEC_SA_ATTR_IPSEC_DIRECTION == SAI_IPSEC_DIRECTION_INGRESS and SAI_IPSEC_SA_ATTR_TERM_SRC_IP_ENABLE == true
     */
    SAI_IPSEC_SA_ATTR_TERM_SRC_IP,

    /**
     * @brief IPsec egress sequence number (SN). One less than the next SN.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_IPSEC_SA_ATTR_IPSEC_DIRECTION == SAI_IPSEC_DIRECTION_EGRESS
     */
    SAI_IPSEC_SA_ATTR_EGRESS_ESN,

    /**
     * @brief Minimum value of ingress IPsec sequence number (SN).
     * Can be Updated by value from IPsec peer for gross level delay prevention.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 1
     * @validonly SAI_IPSEC_SA_ATTR_IPSEC_DIRECTION == SAI_IPSEC_DIRECTION_INGRESS
     */
    SAI_IPSEC_SA_ATTR_MINIMUM_INGRESS_ESN,

    /**
     * @brief Set IPSEC SA statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_IPSEC_SA_ATTR_STATS_COUNT_MODE,

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
    SAI_IPSEC_SA_ATTR_SELECTIVE_COUNTER_LIST,

    /**
     * @brief End of IPsec Security Association attributes
     */
    SAI_IPSEC_SA_ATTR_END,

    /**
     * @brief Custom range base value
     */
    SAI_IPSEC_SA_ATTR_CUSTOM_RANGE_BASE = 0x10000000
} sai_ipsec_sa_attr_t;

/**
 * @brief IPsec flow counter IDs in sai_get_ipsec_sa_stats() call.
 */
typedef enum _sai_ipsec_sa_stat_t
{
    /**
     * @brief Total octets in all Ethernet frames processed by this SA.
     */
    SAI_IPSEC_SA_STAT_PROTECTED_OCTETS,

    /**
     * @brief Count of Ethernet frames processed by this SA. This should
     * normally be the sum of all the good and error packets for this SA.
     */
    SAI_IPSEC_SA_STAT_PROTECTED_PKTS,

    /**
     * @brief Count of validated error-free received (ingress) packets
     * for this SA.
     * Valid only for ingress, always returns 0 for egress.
     */
    SAI_IPSEC_SA_STAT_GOOD_PKTS,

    /**
     * @brief Count of packets with bad header for this SA. This could be due
     * the packet header being different from the format expected for this SA.
     * Valid only for ingress, always returns 0 for egress.
     */
    SAI_IPSEC_SA_STAT_BAD_HEADER_PKTS_IN,

    /**
     * @brief Count of replayed packets. This also includes late packets if
     * the hardware does not provide a separate counter for late packets.
     * Valid only for ingress, always returns 0 for egress.
     */
    SAI_IPSEC_SA_STAT_REPLAYED_PKTS_IN,

    /**
     * @brief Count of packets outside the replay window. Always 0 if the
     * hardware does not provide a separate counter for late packets.
     * Valid only for ingress, always returns 0 for egress.
     */
    SAI_IPSEC_SA_STAT_LATE_PKTS_IN,

    /**
     * @brief Count of packets with bad trailer. This could be due to
     * insufficient or invalid padding, etc. For cut-through switching, this
     * drop would normally be implemented as CRC corruption.
     * Valid only for ingress, always returns 0 for egress.
     */
    SAI_IPSEC_SA_STAT_BAD_TRAILER_PKTS_IN,

    /**
     * @brief Count of packets with authentication and integrity failure
     * For cut-through switching, this drop would normally be implemented as
     * CRC corruption.
     * Valid only for ingress, always returns 0 for egress.
     */
    SAI_IPSEC_SA_STAT_AUTH_FAIL_PKTS_IN,

    /**
     * @brief Count of dummy packets dropped by IPsec logic. These are packets
     * with 59 as the next header field value in IPsec trailer. For
     * cut-through switching, this drop would normally be implemented as CRC
     * corruption.
     * Valid only for ingress, always returns 0 for egress.
     */
    SAI_IPSEC_SA_STAT_DUMMY_DROPPED_PKTS_IN,

    /**
     * @brief Count of other packets dropped by IPsec logic. This could be due
     * to not programmed or incorrectly programmed SA, MTU violation, etc.
     */
    SAI_IPSEC_SA_STAT_OTHER_DROPPED_PKTS,
} sai_ipsec_sa_stat_t;

/**
 * @brief Create a IPsec object
 *
 * @param[out] ipsec_id The IPsec object id associated with this switch/PHY
 * @param[in] switch_id The switch/PHY Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_ipsec_fn)(
        _Out_ sai_object_id_t *ipsec_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete the IPsec object
 *
 * @param[in] ipsec_id The IPsec object id associated with this switch/PHY
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_ipsec_fn)(
        _In_ sai_object_id_t ipsec_id);

/**
 * @brief Set IPsec attribute
 *
 * @param[in] ipsec_id The IPsec object id associated with this switch/PHY
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_ipsec_attribute_fn)(
        _In_ sai_object_id_t ipsec_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get IPsec attribute
 *
 * @param[in] ipsec_id The IPsec object id associated with this switch/PHY
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ipsec_attribute_fn)(
        _In_ sai_object_id_t ipsec_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create a IPsec port
 *
 * @param[out] ipsec_port_id The IPsec port id
 * @param[in] switch_id The switch/PHY Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_ipsec_port_fn)(
        _Out_ sai_object_id_t *ipsec_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a IPsec port
 *
 * @param[in] ipsec_port_id The IPsec port id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_ipsec_port_fn)(
        _In_ sai_object_id_t ipsec_port_id);

/**
 * @brief Set IPsec port attribute
 *
 * @param[in] ipsec_port_id The IPsec port id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_ipsec_port_attribute_fn)(
        _In_ sai_object_id_t ipsec_port_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get IPsec port attribute
 *
 * @param[in] ipsec_port_id IPsec port id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ipsec_port_attribute_fn)(
        _In_ sai_object_id_t ipsec_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get IPsec port counters
 *
 * @param[in] ipsec_port_id IPsec port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ipsec_port_stats_fn)(
        _In_ sai_object_id_t ipsec_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get IPsec port counters extended
 *
 * @param[in] ipsec_port_id IPsec port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Should match SAI_IPSEC_ATTR_STATS_MODE
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ipsec_port_stats_ext_fn)(
        _In_ sai_object_id_t ipsec_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear IPsec port counters
 *
 * @param[in] ipsec_port_id IPsec port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_ipsec_port_stats_fn)(
        _In_ sai_object_id_t ipsec_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Create a IPsec Security Association
 *
 * @param[out] ipsec_sa_id The IPsec Security Association id
 * @param[in] switch_id The switch/PHY Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_ipsec_sa_fn)(
        _Out_ sai_object_id_t *ipsec_sa_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a IPsec Security Association
 *
 * @param[in] ipsec_sa_id The IPsec Security Association id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_ipsec_sa_fn)(
        _In_ sai_object_id_t ipsec_sa_id);

/**
 * @brief Set IPsec Security Association attribute
 *
 * @param[in] ipsec_sa_id The IPsec Security Association id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_ipsec_sa_attribute_fn)(
        _In_ sai_object_id_t ipsec_sa_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get IPsec Security Association attribute
 *
 * @param[in] ipsec_sa_id IPsec Security Association id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ipsec_sa_attribute_fn)(
        _In_ sai_object_id_t ipsec_sa_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get IPsec Security Association counters
 *
 * @param[in] ipsec_sa_id IPsec Security Association id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ipsec_sa_stats_fn)(
        _In_ sai_object_id_t ipsec_sa_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get IPsec Security Association counters extended
 *
 * @param[in] ipsec_sa_id IPsec Security Association id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Should match SAI_IPSEC_ATTR_STATS_MODE
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ipsec_sa_stats_ext_fn)(
        _In_ sai_object_id_t ipsec_sa_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear IPsec Security Association counters
 *
 * @param[in] ipsec_sa_id IPsec Security Association id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_ipsec_sa_stats_fn)(
        _In_ sai_object_id_t ipsec_sa_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief IPsec SA status change notification
 *
 * Passed as a parameter into sai_initialize_switch()
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Array of notifications
 */
typedef void (*sai_ipsec_sa_status_change_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_ipsec_sa_status_notification_t *data);

/**
 * @brief IPsec methods table retrieved with sai_api_query()
 */
typedef struct _sai_ipsec_api_t
{
    sai_create_ipsec_fn                create_ipsec;
    sai_remove_ipsec_fn                remove_ipsec;
    sai_set_ipsec_attribute_fn         set_ipsec_attribute;
    sai_get_ipsec_attribute_fn         get_ipsec_attribute;
    sai_create_ipsec_port_fn           create_ipsec_port;
    sai_remove_ipsec_port_fn           remove_ipsec_port;
    sai_set_ipsec_port_attribute_fn    set_ipsec_port_attribute;
    sai_get_ipsec_port_attribute_fn    get_ipsec_port_attribute;
    sai_get_ipsec_port_stats_fn        get_ipsec_port_stats;
    sai_get_ipsec_port_stats_ext_fn    get_ipsec_port_stats_ext;
    sai_clear_ipsec_port_stats_fn      clear_ipsec_port_stats;
    sai_create_ipsec_sa_fn             create_ipsec_sa;
    sai_remove_ipsec_sa_fn             remove_ipsec_sa;
    sai_set_ipsec_sa_attribute_fn      set_ipsec_sa_attribute;
    sai_get_ipsec_sa_attribute_fn      get_ipsec_sa_attribute;
    sai_get_ipsec_sa_stats_fn          get_ipsec_sa_stats;
    sai_get_ipsec_sa_stats_ext_fn      get_ipsec_sa_stats_ext;
    sai_clear_ipsec_sa_stats_fn        clear_ipsec_sa_stats;
} sai_ipsec_api_t;

/**
 * @}
 */
#endif /** __SAIIPSEC_H_ */
