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
 * @file    saidtel.h
 *
 * @brief   This module defines SAI data plane telemetry (DTEL) interface
 *
 * @warning This is a SAI experimental module
 */

#if !defined (__SAIDTEL_H_)
#define __SAIDTEL_H_

#include <saitypes.h>

/**
 * @defgroup SAIDTEL SAI - DTEL specific API definitions
 *
 * @{
 */

/**
 * @brief DTEL attributes for the switch
 *
 * @warning experimental
 *
 * @note Only one DTEL object per switch is allowed
 */
typedef enum _sai_dtel_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DTEL_ATTR_START,

    /**
     * @brief Enable DTEL INT endpoint
     *
     * @warning experimental
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_ATTR_INT_ENDPOINT_ENABLE = SAI_DTEL_ATTR_START,

    /**
     * @brief Enable DTEL INT transit
     *
     * @warning experimental
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_ATTR_INT_TRANSIT_ENABLE,

    /**
     * @brief Enable Packet postcard
     *
     * @warning experimental
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_ATTR_POSTCARD_ENABLE,

    /**
     * @brief Enable Drop Report
     *
     * @warning experimental
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_ATTR_DROP_REPORT_ENABLE,

    /**
     * @brief Enable Queue Report
     *
     * @warning experimental
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_ATTR_QUEUE_REPORT_ENABLE,

    /**
     * @brief Globally unique switch ID
     *
     * @warning experimental
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_DTEL_ATTR_SWITCH_ID,

    /**
     * @brief DTEL flow state clear cycle
     *
     * @warning experimental
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_DTEL_ATTR_FLOW_STATE_CLEAR_CYCLE,

    /**
     * @brief Latency sensitivity for flow state change detection
     *
     * @warning experimental
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_DTEL_ATTR_LATENCY_SENSITIVITY,

    /**
     * @brief DTEL sink ports
     *
     * @warning experimental
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT
     * @default empty
     */
    SAI_DTEL_ATTR_SINK_PORT_LIST,

    /**
     * @brief Reserved DSCP value for INT over L4
     *
     * @warning experimental
     *
     * @type sai_acl_field_data_t sai_uint8_t
     * @flags CREATE_AND_SET
     * @default disabled
     */
    SAI_DTEL_ATTR_INT_L4_DSCP,

    /**
     * @brief End of attributes
     */
    SAI_DTEL_ATTR_END,

    /**
     * @brief Custom range base value start
     */
    SAI_DTEL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /**
     * @brief End of Custom range base
     */
    SAI_DTEL_ATTR_CUSTOM_RANGE_END

} sai_dtel_attr_t;

/**
 * @brief Queue report trigger attributes
 *
 * @warning experimental
 */
typedef enum _sai_dtel_queue_report_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_START,

    /**
     * @brief Queue object ID
     *
     * @warning experimental
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_QUEUE
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_QUEUE_ID = SAI_DTEL_QUEUE_REPORT_ATTR_START,

    /**
     * @brief Queue depth threshold in byte
     *
     * @warning experimental
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_DEPTH_THRESHOLD,

    /**
     * @brief Queue latency threshold in nanosecond
     *
     * @warning experimental
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_LATENCY_THRESHOLD,

    /**
     * @brief Maximum number of continuous reports after threshold breach
     *
     * @warning experimental
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_BREACH_QUOTA,

    /**
     * @brief Send report for packets dropped by the queue
     *
     * @warning experimental
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_TAIL_DROP,

    /**
     * @brief End of attributes
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_END,

    /**
     * @brief Custom range base value start
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /**
     * @brief End of Custom range base
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_CUSTOM_RANGE_END

} sai_dtel_queue_report_attr_t;

/**
 * @brief INT session attributes
 *
 * @warning experimental
 */
typedef enum _sai_dtel_int_session_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DTEL_INT_SESSION_ATTR_START,

    /**
     * @brief INT max hop count
     *
     * @warning experimental
     *
     * The maximum number of hops that are allowed to
     * add their metadata to the packet
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 8
     */
    SAI_DTEL_INT_SESSION_ATTR_MAX_HOP_COUNT = SAI_DTEL_INT_SESSION_ATTR_START,

    /**
     * @brief Collect switch ID
     *
     * @warning experimental
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_INT_SESSION_ATTR_COLLECT_SWITCH_ID,

    /**
     * @brief Collect ingress and egress ports
     *
     * @warning experimental
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_INT_SESSION_ATTR_COLLECT_SWITCH_PORTS,

    /**
     * @brief Collect ingress timestamp
     *
     * @warning experimental
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_INT_SESSION_ATTR_COLLECT_INGRESS_TIMESTAMP,

    /**
     * @brief Collect egress timestamp
     *
     * @warning experimental
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_INT_SESSION_ATTR_COLLECT_EGRESS_TIMESTAMP,

    /**
     * @brief Collect queue information
     *
     * @warning experimental
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_INT_SESSION_ATTR_COLLECT_QUEUE_INFO,

    /**
     * @brief End of attributes
     */
    SAI_DTEL_INT_SESSION_ATTR_END,

    /**
     * @brief Custom range base value start
     */
    SAI_DTEL_INT_SESSION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /**
     * @brief End of Custom range base
     */
    SAI_DTEL_INT_SESSION_ATTR_CUSTOM_RANGE_END

} sai_dtel_int_session_attr_t;

/**
 * @brief DTEL report session attributes
 *
 * @warning experimental
 */
typedef enum _sai_dtel_report_session_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DTEL_REPORT_SESSION_ATTR_START,

    /**
     * @brief DTEL report source IP address
     *
     * @warning experimental
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_DTEL_REPORT_SESSION_ATTR_SRC_IP = SAI_DTEL_REPORT_SESSION_ATTR_START,

    /**
     * @brief DTEL report destination IP addresses
     *
     * @warning experimental
     *
     * @type sai_ip_address_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_DTEL_REPORT_SESSION_ATTR_DST_IP_LIST,

    /**
     * @brief DTEL report virtual router ID
     *
     * @warning experimental
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_DTEL_REPORT_SESSION_ATTR_VIRTUAL_ROUTER_ID,

    /**
     * @brief DTEL report truncate size
     *
     * @warning experimental
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_DTEL_REPORT_SESSION_ATTR_TRUNCATE_SIZE,

    /**
     * @brief DTEL report UDP destination port
     *
     * @warning experimental
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_DTEL_REPORT_SESSION_ATTR_UDP_DST_PORT,

    /**
     * @brief End of attributes
     */
    SAI_DTEL_REPORT_SESSION_ATTR_END,

    /**
     * @brief Custom range base value start
     */
    SAI_DTEL_REPORT_SESSION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /**
     * @brief End of Custom range base
     */
    SAI_DTEL_REPORT_SESSION_ATTR_CUSTOM_RANGE_END

} sai_dtel_report_session_attr_t;

/**
 * @brief Enum defining DTEL event types
 *
 * @warning experimental
 */
typedef enum _sai_dtel_event_type_t
{
    /** Report triggered by new flow or flow state (e.g., path, latency) change */
    SAI_DTEL_EVENT_TYPE_FLOW_STATE,

    /** Report triggered by REPORT_ALL_PACKETS in watchlist entry action */
    SAI_DTEL_EVENT_TYPE_FLOW_REPORT_ALL_PACKETS,

    /** Report triggered by TCP FLAGS */
    SAI_DTEL_EVENT_TYPE_FLOW_TCPFLAG,

    /** Report triggered by queue depth or latency threshold breach */
    SAI_DTEL_EVENT_TYPE_QUEUE_REPORT_THRESHOLD_BREACH,

    /** Report triggered by queue tail drop */
    SAI_DTEL_EVENT_TYPE_QUEUE_REPORT_TAIL_DROP,

    /** Report triggered by packet drop */
    SAI_DTEL_EVENT_TYPE_DROP_REPORT,

    SAI_DTEL_EVENT_TYPE_MAX

} sai_dtel_event_type_t;

/**
 * @brief DTEL events attributes
 *
 * @warning experimental
 */
typedef enum _sai_dtel_event_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DTEL_EVENT_ATTR_START,

    /**
     * @brief DTEL event type
     *
     * @warning experimental
     *
     * @type sai_dtel_event_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DTEL_EVENT_ATTR_TYPE = SAI_DTEL_EVENT_ATTR_START,

    /**
     * @brief DTEL report session
     *
     * @warning experimental
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_DTEL_REPORT_SESSION
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_DTEL_EVENT_ATTR_REPORT_SESSION,

    /**
     * @brief DTEL report DSCP value
     *
     * @warning experimental
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_DTEL_EVENT_ATTR_DSCP_VALUE,

    /**
     * @brief End of attributes
     */
    SAI_DTEL_EVENT_ATTR_END,

    /**
     * @brief Custom range base value start
     */
    SAI_DTEL_EVENT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /**
     * @brief End of Custom range base
     */
    SAI_DTEL_EVENT_ATTR_CUSTOM_RANGE_END

} sai_dtel_event_attr_t;

/**
 * @brief Create and return a DTEL object
 *
 * @warning experimental
 *
 * @param[out] dtel_id DTEL object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_dtel_fn)(
        _Out_ sai_object_id_t *dtel_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a DTEL object
 *
 * @warning experimental
 *
 * @param[in] dtel_id DTEL object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_dtel_fn)(
        _In_ sai_object_id_t dtel_id);

/**
 * @brief Set DTEL attribute
 *
 * @warning experimental
 *
 * @param[in] dtel_id DTEL object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_dtel_attribute_fn)(
        _In_ sai_object_id_t dtel_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get DTEL attribute
 *
 * @warning experimental
 *
 * @param[in] dtel_id DTEL object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_dtel_attribute_fn)(
        _In_ sai_object_id_t dtel_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create and return a DTEL queue report object
 *
 * @warning experimental
 *
 * @param[out] dtel_queue_report_id DTEL queue report object id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_dtel_queue_report_fn)(
        _Out_ sai_object_id_t *dtel_queue_report_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a DTEL queue report
 *
 * @warning experimental
 *
 * @param[in] dtel_queue_report_id DTEL queue report id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_dtel_queue_report_fn)(
        _In_ sai_object_id_t dtel_queue_report_id);

/**
 * @brief Set DTEL queue report attribute
 *
 * @warning experimental
 *
 * @param[in] dtel_queue_report_id DTEL queue report id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_dtel_queue_report_attribute_fn)(
        _In_ sai_object_id_t dtel_queue_report_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get DTEL queue report attribute
 *
 * @warning experimental
 *
 * @param[in] dtel_queue_report_id DTEL queue report id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_dtel_queue_report_attribute_fn)(
        _In_ sai_object_id_t dtel_queue_report_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create and return a DTEL INT session object
 *
 * @warning experimental
 *
 * @param[out] dtel_int_session_id DTEL INT session object id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_dtel_int_session_fn)(
        _Out_ sai_object_id_t *dtel_int_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a DTEL INT session
 *
 * @warning experimental
 *
 * @param[in] dtel_int_session_id DTEL INT session id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_dtel_int_session_fn)(
        _In_ sai_object_id_t dtel_int_session_id);

/**
 * @brief Set DTEL INT session attribute
 *
 * @warning experimental
 *
 * @param[in] dtel_int_session_id DTEL INT session id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_dtel_int_session_attribute_fn)(
        _In_ sai_object_id_t dtel_int_session_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get DTEL INT session attribute
 *
 * @warning experimental
 *
 * @param[in] dtel_int_session_id DTEL INT session id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_dtel_int_session_attribute_fn)(
        _In_ sai_object_id_t dtel_int_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create and return a DTEL report session object
 *
 * @warning experimental
 *
 * @param[out] dtel_report_session_id DTEL report session object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_dtel_report_session_fn)(
        _Out_ sai_object_id_t *dtel_report_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a DTEL report session
 *
 * @warning experimental
 *
 * @param[in] dtel_report_session_id DTEL report session id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_dtel_report_session_fn)(
        _In_ sai_object_id_t dtel_report_session_id);

/**
 * @brief Set DTEL report session attribute
 *
 * @warning experimental
 *
 * @param[in] dtel_report_session_id DTEL report session id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_dtel_report_session_attribute_fn)(
        _In_ sai_object_id_t dtel_report_session_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get DTEL report session attribute
 *
 * @warning experimental
 *
 * @param[in] dtel_report_session_id DTEL report session id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_dtel_report_session_attribute_fn)(
        _In_ sai_object_id_t dtel_report_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create and return a DTEL event object
 *
 * @warning experimental
 *
 * @param[out] dtel_event_id DTEL event object id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_dtel_event_fn)(
        _Out_ sai_object_id_t *dtel_event_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a DTEL event
 *
 * @warning experimental
 *
 * @param[in] dtel_event_id DTEL event id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_dtel_event_fn)(
        _In_ sai_object_id_t dtel_event_id);

/**
 * @brief Set DTEL event attribute
 *
 * @warning experimental
 *
 * @param[in] dtel_event_id DTEL event id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_dtel_event_attribute_fn)(
        _In_ sai_object_id_t dtel_event_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get DTEL event attribute
 *
 * @warning experimental
 *
 * @param[in] dtel_event_id DTEL event id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_dtel_event_attribute_fn)(
        _In_ sai_object_id_t dtel_event_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_dtel_api_t
{
    sai_create_dtel_fn                        create_dtel;
    sai_remove_dtel_fn                        remove_dtel;
    sai_set_dtel_attribute_fn                 set_dtel_attribute;
    sai_get_dtel_attribute_fn                 get_dtel_attribute;

    sai_create_dtel_queue_report_fn           create_dtel_queue_report;
    sai_remove_dtel_queue_report_fn           remove_dtel_queue_report;
    sai_set_dtel_queue_report_attribute_fn    set_dtel_queue_report_attribute;
    sai_get_dtel_queue_report_attribute_fn    get_dtel_queue_report_attribute;

    sai_create_dtel_int_session_fn            create_dtel_int_session;
    sai_remove_dtel_int_session_fn            remove_dtel_int_session;
    sai_set_dtel_int_session_attribute_fn     set_dtel_int_session_attribute;
    sai_get_dtel_int_session_attribute_fn     get_dtel_int_session_attribute;

    sai_create_dtel_report_session_fn         create_dtel_report_session;
    sai_remove_dtel_report_session_fn         remove_dtel_report_session;
    sai_set_dtel_report_session_attribute_fn  set_dtel_report_session_attribute;
    sai_get_dtel_report_session_attribute_fn  get_dtel_report_session_attribute;

    sai_create_dtel_event_fn                  create_dtel_event;
    sai_remove_dtel_event_fn                  remove_dtel_event;
    sai_set_dtel_event_attribute_fn           set_dtel_event_attribute;
    sai_get_dtel_event_attribute_fn           get_dtel_event_attribute;

} sai_dtel_api_t;

/**
 * @}
 */
#endif /** __SAIDTEL_H_ */
