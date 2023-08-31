/**
 * Copyright (c) 2023 Microsoft Open Technologies, Inc.
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
 * @file    saitwamp.h
 *
 * @brief   This module defines SAI Two-Way Active Measurement Protocol interface
 */

#if !defined (__SAITWAMP_H_)
#define __SAITWAMP_H_

#include <saitypes.h>

/**
 * @defgroup SAITWAMP SAI - Two-Way Active Measurement Protocol specific public APIs and data structures
 *
 * @{
 */

/**
 * @brief SAI Two-Way Active Measurement Protocol session authentication mode,
 * there are three modes: unauthenticated, authenticated, and encrypted.
 */
typedef enum _sai_twamp_session_auth_mode_t
{
    /** Session session unauthenticated mode */
    SAI_TWAMP_SESSION_AUTH_MODE_UNAUTHENTICATED = 0,

    /** Session session authenticated mode */
    SAI_TWAMP_SESSION_AUTH_MODE_AUTHENTICATED,

    /** Session session encrypted mode */
    SAI_TWAMP_SESSION_AUTH_MODE_ENCRYPTED

} sai_twamp_session_auth_mode_t;

/**
 * @brief SAI Two-Way Active Measurement Protocol role
 */
typedef enum _sai_twamp_session_role_t
{
    /** Session-sender sends test request packets */
    SAI_TWAMP_SESSION_ROLE_SENDER = 0,

    /** Session-reflector reflects test response packets */
    SAI_TWAMP_SESSION_ROLE_REFLECTOR

} sai_twamp_session_role_t;

/**
 * @brief SAI Two-Way Active Measurement Protocol mode
 */
typedef enum _sai_twamp_mode_t
{
    /**
     * @brief Means TWAMP protocol when enabling Two-Way Active Measurement Protocol full mode
     */
    SAI_TWAMP_MODE_FULL = 0,

    /**
     * @brief Means TWAMP Light protocol when enabling Two-Way Active Measurement Protocol light mode
     */
    SAI_TWAMP_MODE_LIGHT

} sai_twamp_mode_t;

/**
 * @brief SAI Two-Way Active Measurement transmitting mode
 */
typedef enum _sai_twamp_pkt_tx_mode_t
{
    /** @brief Continue to send Two-Way Active Measurement Protocol test packet */
    SAI_TWAMP_PKT_TX_MODE_CONTINUOUS = 0,

    /** @brief Only send Two-Way Active Measurement Protocol test packets with assigned number */
    SAI_TWAMP_PKT_TX_MODE_PACKET_COUNT,

    /** @brief Send Two-Way Active Measurement Protocol test packets during period time */
    SAI_TWAMP_PKT_TX_MODE_PERIOD

} sai_twamp_pkt_tx_mode_t;

/**
 * @brief SAI Two-Way Active Measurement format of timestamp
 */
typedef enum _sai_twamp_timestamp_format_t
{
    /**
     * @brief Packet timestamp format is Network Time Protocol format, 32 bit second and 32 bit fractional part of second
     */
    SAI_TWAMP_TIMESTAMP_FORMAT_NTP = 0,

    /**
     * @brief Packet timestamp format is PTP format, 32 bit second and 32 bit nanosecond
     */
    SAI_TWAMP_TIMESTAMP_FORMAT_PTP

} sai_twamp_timestamp_format_t;

/**
 * @brief SAI Two-Way Active Measurement Protocol type of encapsulation
 */
typedef enum _sai_twamp_encapsulation_type_t
{
    /**
     * @brief IP Encapsulation, L2 header | IP(v4/v6) header | UDP header | Two-Way Active Measurement Protocol test packet
     */
    SAI_TWAMP_ENCAPSULATION_TYPE_IP = 0,

    /**
     * @brief L2 Virtual Private Network Encapsulation, L2 header | MPLS Label List | L2 header | IP(v4/v6) header | UDP header | Two-Way Active Measurement Protocol test packet
     */
    SAI_TWAMP_ENCAPSULATION_TYPE_MPLS_L2VPN,

    /**
     * @brief L3 Virtual Private Network Encapsulation, L2 header | MPLS Label List | IP(v4/v6) header | UDP header | Two-Way Active Measurement Protocol test packet
     */
    SAI_TWAMP_ENCAPSULATION_TYPE_MPLS_L3VPN,

    /**
     * @brief VXLAN Network Encapsulation, L2 header | IP(v4/v6) header | UDP header | VXLAN header | L2 header | IP(v4/v6) header | UDP header | Two-Way Active Measurement Protocol test packet
     */
    SAI_TWAMP_ENCAPSULATION_TYPE_VXLAN

} sai_twamp_encapsulation_type_t;

/**
 * @brief SAI attributes for Two-Way Active Measurement Protocol session
 */
typedef enum _sai_twamp_session_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_TWAMP_SESSION_ATTR_START,

    /**
     * @brief Two-Way Active Measurement Protocol mode: light mode and full mode
     *
     * @type sai_twamp_mode_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TWAMP_SESSION_ATTR_TWAMP_MODE = SAI_TWAMP_SESSION_ATTR_START,

    /**
     * @brief Two-Way Active Measurement Protocol session role of sender or reflector.
     *
     * @type sai_twamp_session_role_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TWAMP_SESSION_ATTR_SESSION_ROLE,

    /**
     * @brief Two-Way Active Measurement Protocol Session mode: unauthenticated, authenticated, and encrypted.
     *
     * @type sai_twamp_session_auth_mode_t
     * @flags CREATE_ONLY
     * @default SAI_TWAMP_SESSION_AUTH_MODE_UNAUTHENTICATED
     */
    SAI_TWAMP_SESSION_ATTR_AUTH_MODE,

    /**
     * @brief Hardware lookup valid
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default true
     */
    SAI_TWAMP_SESSION_ATTR_HW_LOOKUP_VALID,

    /**
     * @brief Virtual router object
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_TWAMP_SESSION_ATTR_HW_LOOKUP_VALID == true
     */
    SAI_TWAMP_SESSION_ATTR_VIRTUAL_ROUTER,

    /**
     * @brief L2 source MAC address
     *
     * @type sai_mac_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_TWAMP_SESSION_ATTR_HW_LOOKUP_VALID == false
     */
    SAI_TWAMP_SESSION_ATTR_SRC_MAC,

    /**
     * @brief L2 destination MAC address
     *
     * @type sai_mac_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_TWAMP_SESSION_ATTR_HW_LOOKUP_VALID == false
     */
    SAI_TWAMP_SESSION_ATTR_DST_MAC,

    /**
     * @brief L2 header VLAN Id.
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan true
     * @condition SAI_TWAMP_SESSION_ATTR_VLAN_HEADER_VALID == true
     */
    SAI_TWAMP_SESSION_ATTR_VLAN_ID,

    /**
     * @brief L2 header packet priority (3 bits).
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_TWAMP_SESSION_ATTR_VLAN_HEADER_VALID == true
     */
    SAI_TWAMP_SESSION_ATTR_VLAN_PRI,

    /**
     * @brief L2 header Vlan CFI (1 bit).
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_TWAMP_SESSION_ATTR_VLAN_HEADER_VALID == true
     */
    SAI_TWAMP_SESSION_ATTR_VLAN_CFI,

    /**
     * @brief Vlan header valid
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     * @validonly SAI_TWAMP_SESSION_ATTR_HW_LOOKUP_VALID == false
     */
    SAI_TWAMP_SESSION_ATTR_VLAN_HEADER_VALID,

    /**
     * @brief Local source IP address
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TWAMP_SESSION_ATTR_SRC_IP,

    /**
     * @brief Remote Destination IP address
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TWAMP_SESSION_ATTR_DST_IP,

    /**
     * @brief UDP Source port
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TWAMP_SESSION_ATTR_UDP_SRC_PORT,

    /**
     * @brief UDP Destination port
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TWAMP_SESSION_ATTR_UDP_DST_PORT,

    /**
     * @brief DSCP of IP header
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TWAMP_SESSION_ATTR_DSCP,

    /**
     * @brief TTL of IP header
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 255
     */
    SAI_TWAMP_SESSION_ATTR_TTL,

    /**
     * @brief MPLS L2 Virtual Private Network, MPLS L3 Virtual Private Network or VXLAN tunnel L2 header Src MAC Address, when hardware lookup is disable
     *
     * @type sai_mac_t
     * @flags CREATE_ONLY
     * @default vendor
     * @validonly SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_MPLS_L2VPN or SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_MPLS_L3VPN or SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_VXLAN
     */
    SAI_TWAMP_SESSION_ATTR_TUNNEL_SRC_MAC,

    /**
     * @brief MPLS L2 Virtual Private Network, MPLS L3 Virtual Private Network or VXLAN tunnel L2 header Dst MAC Address, when hardware lookup is disable
     *
     * @type sai_mac_t
     * @flags CREATE_ONLY
     * @default vendor
     * @validonly SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_MPLS_L2VPN or SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_MPLS_L3VPN or SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_VXLAN
     */
    SAI_TWAMP_SESSION_ATTR_TUNNEL_DST_MAC,

    /**
     * @brief MPLS L2 Virtual Private Network, MPLS L3 Virtual Private Network or VXLAN tunnel L2 header outer VLAN Id, when hardware lookup is disable
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan true
     * @default 0
     * @validonly SAI_TWAMP_SESSION_ATTR_TUNNEL_OUTER_VLAN_HEADER_VALID == true
     */
    SAI_TWAMP_SESSION_ATTR_TUNNEL_OUTER_VLAN_ID,

    /**
     * @brief MPLS L2 Virtual Private Network, MPLS L3 Virtual Private Network or VXLAN tunnel L2 header outer Vlan priority (3 bits), when hardware lookup is disable
     *
     * @type sai_uint8_t
     * @flags CREATE_ONLY
     * @default 0
     * @validonly SAI_TWAMP_SESSION_ATTR_TUNNEL_OUTER_VLAN_HEADER_VALID == true
     */
    SAI_TWAMP_SESSION_ATTR_TUNNEL_OUTER_VLAN_PRI,

    /**
     * @brief MPLS L2 Virtual Private Network, MPLS L3 Virtual Private Network or VXLAN tunnel L2 header outer Vlan CFI (1 bit), when hardware lookup is disable
     *
     * @type sai_uint8_t
     * @flags CREATE_ONLY
     * @default 0
     * @validonly SAI_TWAMP_SESSION_ATTR_TUNNEL_OUTER_VLAN_HEADER_VALID == true
     */
    SAI_TWAMP_SESSION_ATTR_TUNNEL_OUTER_VLAN_CFI,

    /**
     * @brief Tunnel outer vlan header valid
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     * @validonly SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_MPLS_L2VPN or SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_MPLS_L3VPN or SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_VXLAN
     */
    SAI_TWAMP_SESSION_ATTR_TUNNEL_OUTER_VLAN_HEADER_VALID,

    /**
     * @brief MPLS L2 Virtual Private Network, MPLS L3 Virtual Private Network tunnel push label, when hardware lookup is disable
     *
     * @type sai_u32_list_t
     * @flags CREATE_ONLY
     * @default empty
     * @validonly SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_MPLS_L2VPN or SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_MPLS_L3VPN
     */
    SAI_TWAMP_SESSION_ATTR_TUNNEL_LABELSTACK,

    /**
     * @brief VXLAN tunnel L3 header Src IPv4 Address, when hardware lookup is disable
     *
     * @type sai_ip_address_t
     * @flags CREATE_ONLY
     * @default 0.0.0.0
     * @validonly SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_VXLAN
     */
    SAI_TWAMP_SESSION_ATTR_TUNNEL_SRC_IP,

    /**
     * @brief VXLAN tunnel L3 header Dst IPv4 Address, when hardware lookup is disable
     *
     * @type sai_ip_address_t
     * @flags CREATE_ONLY
     * @default 0.0.0.0
     * @validonly SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_VXLAN
     */
    SAI_TWAMP_SESSION_ATTR_TUNNEL_DST_IP,

    /**
     * @brief VXLAN tunnel VNI, when hardware lookup is disable
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     * @validonly SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_VXLAN
     */
    SAI_TWAMP_SESSION_ATTR_TUNNEL_VNI,

    /**
     * @brief VXLAN tunnel L4 header UDP Source port, when hardware lookup is disable
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     * @validonly SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_VXLAN
     */
    SAI_TWAMP_SESSION_ATTR_TUNNEL_UDP_SRC_PORT,

    /**
     * @brief VXLAN tunnel L4 header UDP Destination port, when hardware lookup is disable
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     * @validonly SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE == SAI_TWAMP_ENCAPSULATION_TYPE_VXLAN
     */
    SAI_TWAMP_SESSION_ATTR_TUNNEL_UDP_DST_PORT,

    /**
     * @brief Encapsulation type
     *
     * @type sai_twamp_encapsulation_type_t
     * @flags CREATE_ONLY
     * @default SAI_TWAMP_ENCAPSULATION_TYPE_IP
     */
    SAI_TWAMP_SESSION_ATTR_TWAMP_ENCAPSULATION_TYPE,

    /**
     * @brief The format of timestamp in test packets.
     *
     * @type sai_twamp_timestamp_format_t
     * @flags CREATE_ONLY
     * @default SAI_TWAMP_TIMESTAMP_FORMAT_NTP
     */
    SAI_TWAMP_SESSION_ATTR_TWAMP_TIMESTAMP_FORMAT,

    /**
     * @brief To enable Two-Way Active Measurement Protocol session transmitting packets
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_TWAMP_SESSION_ATTR_SESSION_ROLE == SAI_TWAMP_SESSION_ROLE_SENDER
     */
    SAI_TWAMP_SESSION_ATTR_SESSION_ENABLE_TRANSMIT,

    /**
     * @brief Two-Way Active Measurement Protocol packet length
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 256
     */
    SAI_TWAMP_SESSION_ATTR_PACKET_LENGTH,

    /**
     * @brief Two-Way Active Measurement Protocol test port
     *
     * @type sai_object_id_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TWAMP_SESSION_ATTR_PORT,

    /**
     * @brief Two-Way Active Measurement Protocol egress port
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT
     * @allownull true
     * @condition SAI_TWAMP_SESSION_ATTR_HW_LOOKUP_VALID == false
     */
    SAI_TWAMP_SESSION_ATTR_TRANSMIT_PORT,

    /**
     * @brief Receiving port of Two-Way Active Measurement Protocol sender and reflector, enable ACL lookup on this port for match test packets to Two-Way Active Measurement Protocol engine.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT
     * @default empty
     */
    SAI_TWAMP_SESSION_ATTR_RECEIVE_PORT,

    /**
     * @brief Two-Way Active Measurement Protocol packets transmitting mode: CONTINUOUS, PACKET_COUNT, PERIOD
     *
     * @type sai_twamp_pkt_tx_mode_t
     * @flags CREATE_ONLY
     * @default SAI_TWAMP_PKT_TX_MODE_CONTINUOUS
     * @validonly SAI_TWAMP_SESSION_ATTR_SESSION_ROLE == SAI_TWAMP_SESSION_ROLE_SENDER
     */
    SAI_TWAMP_SESSION_ATTR_TWAMP_PKT_TX_MODE,

    /**
     * @brief Packet count of transmitting test packets
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     * @validonly SAI_TWAMP_SESSION_ATTR_SESSION_ROLE == SAI_TWAMP_SESSION_ROLE_SENDER and SAI_TWAMP_SESSION_ATTR_TWAMP_PKT_TX_MODE == SAI_TWAMP_PKT_TX_MODE_PACKET_COUNT
     */
    SAI_TWAMP_SESSION_ATTR_TX_PKT_CNT,

    /**
     * @brief Period duration of transmitting test packets
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     * @validonly SAI_TWAMP_SESSION_ATTR_SESSION_ROLE == SAI_TWAMP_SESSION_ROLE_SENDER and SAI_TWAMP_SESSION_ATTR_TWAMP_PKT_TX_MODE == SAI_TWAMP_PKT_TX_MODE_PERIOD
     */
    SAI_TWAMP_SESSION_ATTR_TX_PKT_PERIOD,

    /**
     * @brief Interval of transmitting test packets
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 1000
     * @validonly SAI_TWAMP_SESSION_ATTR_SESSION_ROLE == SAI_TWAMP_SESSION_ROLE_SENDER
     */
    SAI_TWAMP_SESSION_ATTR_TX_INTERVAL,

    /**
     * @brief Timeout of receiving test packets
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 3
     * @validonly SAI_TWAMP_SESSION_ATTR_SESSION_ROLE == SAI_TWAMP_SESSION_ROLE_SENDER
     */
    SAI_TWAMP_SESSION_ATTR_TIMEOUT,

    /**
     * @brief Interval of getting statistics and measurement data
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 10000
     * @validonly SAI_TWAMP_SESSION_ATTR_SESSION_ROLE == SAI_TWAMP_SESSION_ROLE_SENDER
     */
    SAI_TWAMP_SESSION_ATTR_STATISTICS_INTERVAL,

    /**
     * @brief End of attributes
     */
    SAI_TWAMP_SESSION_ATTR_END,

    /** Custom range base value */
    SAI_TWAMP_SESSION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TWAMP_SESSION_ATTR_CUSTOM_RANGE_END

} sai_twamp_session_attr_t;

/**
 * @brief Two-Way Active Measurement Protocol Session counter IDs in sai_get_twamp_session_stats() call
 */
typedef enum _sai_twamp_session_stat_t
{
    /** Rx packet stat */
    SAI_TWAMP_SESSION_STAT_RX_PACKETS,

    /** Rx byte stat */
    SAI_TWAMP_SESSION_STAT_RX_BYTE,

    /** Tx packet stat */
    SAI_TWAMP_SESSION_STAT_TX_PACKETS,

    /** Tx byte stat */
    SAI_TWAMP_SESSION_STAT_TX_BYTE,

    /** Drop packet stat */
    SAI_TWAMP_SESSION_STAT_DROP_PACKETS,

    /** Packet max latency */
    SAI_TWAMP_SESSION_STAT_MAX_LATENCY,

    /** Packet min latency */
    SAI_TWAMP_SESSION_STAT_MIN_LATENCY,

    /** Packet avg latency */
    SAI_TWAMP_SESSION_STAT_AVG_LATENCY,

    /** Packet max jitters */
    SAI_TWAMP_SESSION_STAT_MAX_JITTER,

    /** Packet min jitters */
    SAI_TWAMP_SESSION_STAT_MIN_JITTER,

    /** Packet avg jitters */
    SAI_TWAMP_SESSION_STAT_AVG_JITTER,

    /** Session first timestamp */
    SAI_TWAMP_SESSION_STAT_FIRST_TS,

    /** Session last timestamp */
    SAI_TWAMP_SESSION_STAT_LAST_TS,

    /** Session duration timestamp */
    SAI_TWAMP_SESSION_STAT_DURATION_TS

} sai_twamp_session_stat_t;

/**
 * @brief SAI TWAMP session state
 */
typedef enum _sai_twamp_session_state_t
{
    /** TWAMP Session is inactive */
    SAI_TWAMP_SESSION_STATE_INACTIVE,

    /** TWAMP Session is active */
    SAI_TWAMP_SESSION_STATE_ACTIVE

} sai_twamp_session_state_t;

/**
 * @brief Notification data format received from SAI TWAMP callback
 *
 * @count counters_ids[number_of_counters]
 * @count counters[number_of_counters]
 */
typedef struct _sai_twamp_session_stats_data_t
{
    /** TWAMP session statistics index */
    uint32_t index;

    /** Counters number */
    uint32_t number_of_counters;

    /** Specifies the TWAMP session statistics counters ids */
    sai_twamp_session_stat_t *counters_ids;

    /** Specifies the TWAMP session statistics counters */
    uint64_t *counters;

} sai_twamp_session_stats_data_t;

/**
 * @brief Defines the operational status of the TWAMP session
 */
typedef struct _sai_twamp_session_event_notification_data_t
{
    /**
     * @brief TWAMP session id
     *
     * @objects SAI_OBJECT_TYPE_TWAMP_SESSION
     */
    sai_object_id_t twamp_session_id;

    /** TWAMP session state */
    sai_twamp_session_state_t session_state;

    /** TWAMP session stats data */
    sai_twamp_session_stats_data_t session_stats;

} sai_twamp_session_event_notification_data_t;

/**
 * @brief Create Two-Way Active Measurement Protocol session.
 *
 * @param[out] twamp_session_id Two-Way Active Measurement Protocol session id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_create_twamp_session_fn)(
        _Out_ sai_object_id_t *twamp_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Two-Way Active Measurement Protocol session.
 *
 * @param[in] twamp_session_id Two-Way Active Measurement Protocol session id
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_remove_twamp_session_fn)(
        _In_ sai_object_id_t twamp_session_id);

/**
 * @brief Set Two-Way Active Measurement Protocol session attributes.
 *
 * @param[in] twamp_session_id Two-Way Active Measurement Protocol session id
 * @param[in] attr Value of attribute
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_set_twamp_session_attribute_fn)(
        _In_ sai_object_id_t twamp_session_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Two-Way Active Measurement Protocol session attributes.
 *
 * @param[in] twamp_session_id Two-Way Active Measurement Protocol session id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Value of attribute
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_get_twamp_session_attribute_fn)(
        _In_ sai_object_id_t twamp_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get Two-Way Active Measurement Protocol session statistics counters.
 *
 * @param[in] twamp_session_id Two-Way Active Measurement Protocol session id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_twamp_session_stats_fn)(
        _In_ sai_object_id_t twamp_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get Two-Way Active Measurement Protocol session statistics counters extended.
 *
 * @param[in] twamp_session_id Two-Way Active Measurement Protocol session id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_twamp_session_stats_ext_fn)(
        _In_ sai_object_id_t twamp_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear Two-Way Active Measurement Protocol session statistics counters.
 *
 * @param[in] twamp_session_id Two-Way Active Measurement Protocol session id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_twamp_session_stats_fn)(
        _In_ sai_object_id_t twamp_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief TWAMP session notification
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Pointer to TWAMP session notification data array
 */
typedef void (*sai_twamp_session_event_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_twamp_session_event_notification_data_t *data);

/**
 * @brief Two-Way Active Measurement Protocol method table retrieved with sai_api_query()
 */
typedef struct _sai_twamp_api_t
{
    sai_create_twamp_session_fn            create_twamp_session;
    sai_remove_twamp_session_fn            remove_twamp_session;
    sai_set_twamp_session_attribute_fn     set_twamp_session_attribute;
    sai_get_twamp_session_attribute_fn     get_twamp_session_attribute;
    sai_get_twamp_session_stats_fn         get_twamp_session_stats;
    sai_get_twamp_session_stats_ext_fn     get_twamp_session_stats_ext;
    sai_clear_twamp_session_stats_fn       clear_twamp_session_stats;

} sai_twamp_api_t;

/**
 * @}
 */
#endif /** __SAITWAMP_H_ */
