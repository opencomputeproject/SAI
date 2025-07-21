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
 * @file    saiicmpecho.h
 *
 * @brief   This module defines SAI interface
 */

#if !defined (__SAIICMPECHO_H_)
#define __SAIICMPECHO_H_

#include <saitypes.h>

/**
 * @defgroup SAIICMPECHO SAI - ICMP_ECHO specific public APIs and data structures
 *
 * @{
 */

/**
 * @brief SAI ICMP_ECHO session state
 */
typedef enum _sai_icmp_echo_session_state_t
{
    /** ICMP_ECHO Session is in Down */
    SAI_ICMP_ECHO_SESSION_STATE_DOWN = 0,

    /** ICMP_ECHO Session is Up */
    SAI_ICMP_ECHO_SESSION_STATE_UP,

} sai_icmp_echo_session_state_t;

/**
 * @brief Defines the operational status of the ICMP_ECHO session
 */
typedef struct _sai_icmp_echo_session_state_notification_t
{
    /**
     * @brief ICMP_ECHO Session id
     *
     * @objects SAI_OBJECT_TYPE_ICMP_ECHO_SESSION
     */
    sai_object_id_t icmp_echo_session_id;

    /** ICMP_ECHO session state */
    sai_icmp_echo_session_state_t session_state;

} sai_icmp_echo_session_state_notification_t;

/**
 * @brief SAI attributes for ICMP_ECHO session
 */
typedef enum _sai_icmp_echo_session_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_ICMP_ECHO_SESSION_ATTR_START,

    /**
     * @brief Hardware lookup valid. TRUE routes packets via L3 forward lookup;
     * FALSE injects packets directly to specified port.
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default true
     */
    SAI_ICMP_ECHO_SESSION_ATTR_HW_LOOKUP_VALID = SAI_ICMP_ECHO_SESSION_ATTR_START,

    /**
     * @brief Virtual Router
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @condition SAI_ICMP_ECHO_SESSION_ATTR_HW_LOOKUP_VALID == true
     */
    SAI_ICMP_ECHO_SESSION_ATTR_VIRTUAL_ROUTER,

    /**
     * @brief Destination Port
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT
     * @condition SAI_ICMP_ECHO_SESSION_ATTR_HW_LOOKUP_VALID == false
     */
    SAI_ICMP_ECHO_SESSION_ATTR_PORT,

    /**
     * @brief Source Port
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ICMP_ECHO_SESSION_ATTR_RX_PORT,

    /**
     * @brief Session Global Unique Identifier
     *
     * @type sai_uint64_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_ICMP_ECHO_SESSION_ATTR_GUID,

    /**
     * @brief Session Cookie
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_ICMP_ECHO_SESSION_ATTR_COOKIE,

    /**
     * @brief IP header version
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_ICMP_ECHO_SESSION_ATTR_IPHDR_VERSION,

    /**
     * @brief IP header TOS
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ICMP_ECHO_SESSION_ATTR_TOS,

    /**
     * @brief IP header TTL
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 255
     */
    SAI_ICMP_ECHO_SESSION_ATTR_TTL,

    /**
     * @brief Source IP
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_ICMP_ECHO_SESSION_ATTR_SRC_IP_ADDRESS,

    /**
     * @brief Destination IP
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_ICMP_ECHO_SESSION_ATTR_DST_IP_ADDRESS,

    /**
     * @brief L2 source MAC address
     *
     * @type sai_mac_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_ICMP_ECHO_SESSION_ATTR_HW_LOOKUP_VALID == false
     */
    SAI_ICMP_ECHO_SESSION_ATTR_SRC_MAC_ADDRESS,

    /**
     * @brief L2 destination MAC address
     *
     * @type sai_mac_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_ICMP_ECHO_SESSION_ATTR_HW_LOOKUP_VALID == false
     */
    SAI_ICMP_ECHO_SESSION_ATTR_DST_MAC_ADDRESS,

    /**
     * @brief Transmit interval in microseconds
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_ICMP_ECHO_SESSION_ATTR_TX_INTERVAL,

    /**
     * @brief Receive interval in microseconds
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_ICMP_ECHO_SESSION_ATTR_RX_INTERVAL,

    /**
     * @brief To enable protection group switch over on session state change
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ICMP_ECHO_SESSION_ATTR_SET_NEXT_HOP_GROUP_SWITCHOVER,

    /**
     * @brief ICMP_ECHO Session state
     *
     * @type sai_icmp_echo_session_state_t
     * @flags READ_ONLY
     */
    SAI_ICMP_ECHO_SESSION_ATTR_STATE,

    /**
     * @brief Set ICMP echo session statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_ICMP_ECHO_SESSION_ATTR_STATS_COUNT_MODE,

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
    SAI_ICMP_ECHO_SESSION_ATTR_SELECTIVE_COUNTER_LIST,

    /**
     * @brief End of attributes
     */
    SAI_ICMP_ECHO_SESSION_ATTR_END,

    /** Custom range base value */
    SAI_ICMP_ECHO_SESSION_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_icmp_echo_session_attr_t;

/**
 * @brief ICMP_ECHO Session counter IDs in sai_get_icmp_echo_session_stats() call
 */
typedef enum _sai_icmp_echo_session_stat_t
{
    /** Ingress packet stat count */
    SAI_ICMP_ECHO_SESSION_STAT_IN_PACKETS,

    /** Egress packet stat count */
    SAI_ICMP_ECHO_SESSION_STAT_OUT_PACKETS,

} sai_icmp_echo_session_stat_t;

/**
 * @brief Create ICMP_ECHO session.
 *
 * @param[out] icmp_echo_session_id ICMP_ECHO session id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_create_icmp_echo_session_fn)(
        _Out_ sai_object_id_t *icmp_echo_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove ICMP_ECHO session.
 *
 * @param[in] icmp_echo_session_id ICMP_ECHO session id
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_remove_icmp_echo_session_fn)(
        _In_ sai_object_id_t icmp_echo_session_id);

/**
 * @brief Set ICMP_ECHO session attributes.
 *
 * @param[in] icmp_echo_session_id ICMP_ECHO session id
 * @param[in] attr Value of attribute
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_set_icmp_echo_session_attribute_fn)(
        _In_ sai_object_id_t icmp_echo_session_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get ICMP_ECHO session attributes.
 *
 * @param[in] icmp_echo_session_id ICMP_ECHO session id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Value of attribute
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_get_icmp_echo_session_attribute_fn)(
        _In_ sai_object_id_t icmp_echo_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get ICMP_ECHO session statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] icmp_echo_session_id ICMP_ECHO session id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_icmp_echo_session_stats_fn)(
        _In_ sai_object_id_t icmp_echo_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get ICMP_ECHO session statistics counters extended.
 *
 * @param[in] icmp_echo_session_id ICMP_ECHO session id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_icmp_echo_session_stats_ext_fn)(
        _In_ sai_object_id_t icmp_echo_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear ICMP_ECHO session statistics counters.
 *
 * @param[in] icmp_echo_session_id ICMP_ECHO session id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_icmp_echo_session_stats_fn)(
        _In_ sai_object_id_t icmp_echo_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief ICMP_ECHO session state change notification
 *
 * Passed as a parameter into sai_initialize_switch()
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Array of ICMP_ECHO session state
 */
typedef void (*sai_icmp_echo_session_state_change_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_icmp_echo_session_state_notification_t *data);

/**
 * @brief ICMP_ECHO method table retrieved with sai_api_query()
 */
typedef struct _sai_icmp_echo_api_t
{
    sai_create_icmp_echo_session_fn            create_icmp_echo_session;
    sai_remove_icmp_echo_session_fn            remove_icmp_echo_session;
    sai_set_icmp_echo_session_attribute_fn     set_icmp_echo_session_attribute;
    sai_get_icmp_echo_session_attribute_fn     get_icmp_echo_session_attribute;
    sai_get_icmp_echo_session_stats_fn         get_icmp_echo_session_stats;
    sai_get_icmp_echo_session_stats_ext_fn     get_icmp_echo_session_stats_ext;
    sai_clear_icmp_echo_session_stats_fn       clear_icmp_echo_session_stats;

} sai_icmp_echo_api_t;

/**
 * @}
 */
#endif /** __SAIICMPECHO_H_ */
