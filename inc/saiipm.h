/**
 * Copyright (c) 2026 Microsoft Open Technologies, Inc.
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
 * @file    saiipm.h
 *
 * @brief   This module defines SAI IP Measurements (IPM) interface
 */

#if !defined (__SAIIPM_H_)
#define __SAIIPM_H_

#include <saitypes.h>

/**
 * @defgroup SAIIPM SAI - IP Measurements specific public APIs and data structures
 *
 * @{
 */

/**
 * @brief SAI IPM type of encapsulation
 */
typedef enum _sai_ipm_encapsulation_type_t
{
    /**
     * @brief No additional encapsulation
     *
     * Packet format is expected to be:
     * L2 header | IP header | UDP header | measurement payload
     */
    SAI_IPM_ENCAPSULATION_TYPE_NONE = 0,

    /**
     * @brief SRV6 encapsulation
     *
     * Packet format is expected to be:
     * L2 header | IPv6 header | SRH | inner IP header | UDP header | measurement payload
     */
    SAI_IPM_ENCAPSULATION_TYPE_SRV6

} sai_ipm_encapsulation_type_t;

/**
 * @brief IPM session counter IDs in sai_get_ipm_session_stats() call
 */
typedef enum _sai_ipm_session_stat_t
{
    /**
     * @brief Packets transmitted with alternate marking value 0
     */
    SAI_IPM_SESSION_STAT_TX_PKTS_ALTERNATE_MARKING_0 = 0,

    /**
     * @brief Packets transmitted with alternate marking value 1
     */
    SAI_IPM_SESSION_STAT_TX_PKTS_ALTERNATE_MARKING_1,

    /**
     * @brief Packets received with alternate marking value 0
     */
    SAI_IPM_SESSION_STAT_RX_PKTS_ALTERNATE_MARKING_0,

    /**
     * @brief Packets received with alternate marking value 1
     */
    SAI_IPM_SESSION_STAT_RX_PKTS_ALTERNATE_MARKING_1,

    /**
     * @brief First histogram bin stat ID
     *
     * Bin N is read as:
     * SAI_IPM_SESSION_STAT_RX_PKTS_BIN_RANGE_BASE + N
     */
    SAI_IPM_SESSION_STAT_RX_PKTS_BIN_RANGE_BASE = 0x00001000,

    /**
     * @brief Last histogram bin stat ID
     */
    SAI_IPM_SESSION_STAT_RX_PKTS_BIN_RANGE_END = 0x00001fff,

    /** Custom range base value */
    SAI_IPM_SESSION_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_ipm_session_stat_t;

/**
 * @brief SAI IPM session state
 */
typedef enum _sai_ipm_session_state_t
{
    /** IPM session is down */
    SAI_IPM_SESSION_STATE_DOWN = 0,

    /** IPM session is up */
    SAI_IPM_SESSION_STATE_UP

} sai_ipm_session_state_t;

/**
 * @brief Defines the operational status of the IPM session
 */
typedef struct _sai_ipm_session_state_notification_t
{
    /**
     * @brief IPM session object ID
     *
     * @objects SAI_OBJECT_TYPE_IPM_SESSION
     */
    sai_object_id_t ipm_session_id;

    /** IPM session state */
    sai_ipm_session_state_t session_state;

} sai_ipm_session_state_notification_t;

/**
 * @brief SAI IPM session alternate marking
 */
typedef enum _sai_ipm_session_alternate_marking_t
{
    /** Alternate marking color is zero */
    SAI_IPM_SESSION_ALTERNATE_MARKING_ZERO = 0,

    /** Alternate marking color is one */
    SAI_IPM_SESSION_ALTERNATE_MARKING_ONE

} sai_ipm_session_alternate_marking_t;

/**
 * @brief Defines the alternate marking of the IPM session
 */
typedef struct _sai_ipm_session_alternate_marking_notification_t
{
    /**
     * @brief IPM session object ID
     *
     * @objects SAI_OBJECT_TYPE_IPM_SESSION
     */
    sai_object_id_t ipm_session_id;

    /** New alternate marking color */
    sai_ipm_session_alternate_marking_t new_alternate_marking;

} sai_ipm_session_alternate_marking_notification_t;

/**
 * @brief SAI attributes for IPM session
 */
typedef enum _sai_ipm_session_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_IPM_SESSION_ATTR_START,

    /**
     * @brief IPM session ID
     *
     * Identifies the measurement session. The value may be encoded in probe
     * packets or used internally by the implementation to match measurement
     * packets to a session.
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan false
     */
    SAI_IPM_SESSION_ATTR_SESSION_ID = SAI_IPM_SESSION_ATTR_START,

    /**
     * @brief Virtual router object
     *
     * Selects the VRF used by the session endpoints and probe packets.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     */
    SAI_IPM_SESSION_ATTR_VIRTUAL_ROUTER,

    /**
     * @brief Local IP address
     *
     * IP address of the local IPM endpoint.
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPM_SESSION_ATTR_LOCAL_IP_ADDRESS,

    /**
     * @brief Remote IP address
     *
     * IP address of the remote IPM endpoint.
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPM_SESSION_ATTR_REMOTE_IP_ADDRESS,

    /**
     * @brief Local UDP port
     *
     * UDP port of the local IPM endpoint, used to identify received packets.
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan false
     */
    SAI_IPM_SESSION_ATTR_LOCAL_UDP_PORT,

    /**
     * @brief Remote UDP port
     *
     * UDP port of the remote IPM endpoint, used for transmitted packets.
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan false
     */
    SAI_IPM_SESSION_ATTR_REMOTE_UDP_PORT,

    /**
     * @brief DSCP value in IP header
     *
     * Allows measurement packets to be marked for a specific traffic class.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_IPM_SESSION_ATTR_DSCP,

    /**
     * @brief IPv6 flow label
     *
     * Allows IPv6 measurement packets to carry a configured flow label.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_IPM_SESSION_ATTR_FLOW_LABEL,

    /**
     * @brief Encapsulation type
     *
     * Selects the encapsulation type used by the IPM session.
     *
     * @type sai_ipm_encapsulation_type_t
     * @flags CREATE_ONLY
     * @default SAI_IPM_ENCAPSULATION_TYPE_NONE
     */
    SAI_IPM_SESSION_ATTR_IPM_ENCAPSULATION_TYPE,

    /**
     * @brief SRV6 SID list
     *
     * Required when SRV6 encapsulation is selected.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_SRV6_SIDLIST
     * @condition SAI_IPM_SESSION_ATTR_IPM_ENCAPSULATION_TYPE == SAI_IPM_ENCAPSULATION_TYPE_SRV6
     */
    SAI_IPM_SESSION_ATTR_SRV6_SIDLIST_ID,

    /**
     * @brief Source IP of outer encapsulating header
     *
     * Required when SRV6 encapsulation is selected.
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_IPM_SESSION_ATTR_IPM_ENCAPSULATION_TYPE == SAI_IPM_ENCAPSULATION_TYPE_SRV6
     */
    SAI_IPM_SESSION_ATTR_TUNNEL_SRC_IP,

    /**
     * @brief IPM histogram profile
     *
     * Histogram profile defining the effective delay boundaries, in
     * nanoseconds, used to classify delay samples for this session.
     *
     * @type sai_object_id_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_IPM_HISTOGRAM_PROFILE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_IPM_SESSION_ATTR_HISTOGRAM_PROFILE_ID,

    /**
     * @brief Interval of transmitting probe packets in microseconds
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 1000
     */
    SAI_IPM_SESSION_ATTR_TX_INTERVAL,

    /**
     * @brief Liveness detection timeout in microseconds
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 3000
     */
    SAI_IPM_SESSION_ATTR_LIVENESS_TIMEOUT,

    /**
     * @brief Number of statistics exposed by this IPM session
     *
     * This equals fixed counter count plus histogram bin count.
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_IPM_SESSION_ATTR_NUMBER_OF_COUNTERS,

    /**
     * @brief Number of histogram bins
     *
     * Zero when no histogram profile is configured. Otherwise, this equals
     * #SAI_IPM_HISTOGRAM_PROFILE_ATTR_NUMBER_OF_BINS on the configured
     * profile.
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_IPM_SESSION_ATTR_HISTOGRAM_NUMBER_OF_BINS,

    /**
     * @brief End of attributes
     */
    SAI_IPM_SESSION_ATTR_END,

    /** Custom range base value */
    SAI_IPM_SESSION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_IPM_SESSION_ATTR_CUSTOM_RANGE_END

} sai_ipm_session_attr_t;

/**
 * @brief Attribute Id for sai_ipm_histogram_profile
 */
typedef enum _sai_ipm_histogram_profile_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_IPM_HISTOGRAM_PROFILE_ATTR_START,

    /**
     * @brief Exact delay histogram bin boundaries in nanoseconds
     *
     * The list must be nonempty. Every boundary must be greater than zero and
     * the list must be strictly increasing. The boundaries define half-open
     * bins [0, boundary[0]), [boundary[0], boundary[1]), and so on. The final
     * bin has no finite upper boundary.
     *
     * The adapter must realize every boundary exactly. If the boundary list
     * cannot be represented exactly, profile creation must fail with
     * #SAI_STATUS_INVALID_ATTR_VALUE_0 offset by this attribute's index in the
     * create attribute list.
     *
     * @type sai_u32_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPM_HISTOGRAM_PROFILE_ATTR_BIN_BOUNDARIES_NS = SAI_IPM_HISTOGRAM_PROFILE_ATTR_START,

    /**
     * @brief Number of histogram bins
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_IPM_HISTOGRAM_PROFILE_ATTR_NUMBER_OF_BINS,

    /**
     * @brief End of attributes
     */
    SAI_IPM_HISTOGRAM_PROFILE_ATTR_END,

    /** Custom range base value */
    SAI_IPM_HISTOGRAM_PROFILE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_IPM_HISTOGRAM_PROFILE_ATTR_CUSTOM_RANGE_END

} sai_ipm_histogram_profile_attr_t;

/**
 * @brief Create an IPM histogram profile
 *
 * @param[out] ipm_histogram_profile_id The IPM histogram profile id
 * @param[in] switch_id The switch Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_ipm_histogram_profile_fn)(
        _Out_ sai_object_id_t *ipm_histogram_profile_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete an IPM histogram profile
 *
 * @param[in] ipm_histogram_profile_id The IPM histogram profile id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_ipm_histogram_profile_fn)(
        _In_ sai_object_id_t ipm_histogram_profile_id);

/**
 * @brief Set IPM histogram profile attribute
 *
 * @param[in] ipm_histogram_profile_id The IPM histogram profile id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_ipm_histogram_profile_attribute_fn)(
        _In_ sai_object_id_t ipm_histogram_profile_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get IPM histogram profile attribute
 *
 * @param[in] ipm_histogram_profile_id IPM histogram profile id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ipm_histogram_profile_attribute_fn)(
        _In_ sai_object_id_t ipm_histogram_profile_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create IPM session.
 *
 * @param[out] ipm_session_id IPM session id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_create_ipm_session_fn)(
        _Out_ sai_object_id_t *ipm_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove IPM session.
 *
 * @param[in] ipm_session_id IPM session id
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_remove_ipm_session_fn)(
        _In_ sai_object_id_t ipm_session_id);

/**
 * @brief Set IPM session attributes.
 *
 * @param[in] ipm_session_id IPM session id
 * @param[in] attr Value of attribute
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_set_ipm_session_attribute_fn)(
        _In_ sai_object_id_t ipm_session_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get IPM session attributes.
 *
 * @param[in] ipm_session_id IPM session id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Value of attribute
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_get_ipm_session_attribute_fn)(
        _In_ sai_object_id_t ipm_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get IPM session statistics counters.
 *
 * @param[in] ipm_session_id IPM session id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ipm_session_stats_fn)(
        _In_ sai_object_id_t ipm_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get IPM session statistics counters extended.
 *
 * @param[in] ipm_session_id IPM session id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ipm_session_stats_ext_fn)(
        _In_ sai_object_id_t ipm_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear IPM session statistics counters.
 *
 * @param[in] ipm_session_id IPM session id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_ipm_session_stats_fn)(
        _In_ sai_object_id_t ipm_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief IPM session state change notification
 *
 * Register on the switch using #SAI_SWITCH_ATTR_IPM_SESSION_STATE_CHANGE_NOTIFY.
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Array of IPM session state
 */
typedef void (*sai_ipm_session_state_change_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_ipm_session_state_notification_t *data);

/**
 * @brief IPM session alternate marking change notification
 *
 * Register on the switch using
 * #SAI_SWITCH_ATTR_IPM_SESSION_ALTERNATE_MARKING_CHANGE_NOTIFY.
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Array of IPM session alternate marking
 */
typedef void (*sai_ipm_session_alternate_marking_change_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_ipm_session_alternate_marking_notification_t *data);

/**
 * @brief IPM methods table retrieved with sai_api_query()
 */
typedef struct _sai_ipm_api_t
{
    sai_create_ipm_session_fn                   create_ipm_session;
    sai_remove_ipm_session_fn                   remove_ipm_session;
    sai_set_ipm_session_attribute_fn            set_ipm_session_attribute;
    sai_get_ipm_session_attribute_fn            get_ipm_session_attribute;
    sai_get_ipm_session_stats_fn                get_ipm_session_stats;
    sai_get_ipm_session_stats_ext_fn            get_ipm_session_stats_ext;
    sai_clear_ipm_session_stats_fn              clear_ipm_session_stats;

    sai_create_ipm_histogram_profile_fn         create_ipm_histogram_profile;
    sai_remove_ipm_histogram_profile_fn         remove_ipm_histogram_profile;
    sai_set_ipm_histogram_profile_attribute_fn  set_ipm_histogram_profile_attribute;
    sai_get_ipm_histogram_profile_attribute_fn  get_ipm_histogram_profile_attribute;

} sai_ipm_api_t;

/**
 * @}
 */
#endif /** __SAIIPM_H_ */
