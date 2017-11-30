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
 *    Dell Products, L.P., Facebook, Inc
 *
 * @file    saidtel.h
 *
 * @brief       This module defines SAI data plane telemetry interface
 * @description Supported by: Barefoot Networks, Inc.
 * @warning     This module is a SAI experimental module.
 */

#if !defined (__SAIDTEL_H_)
#define __SAIDTEL_H_

#include <saitypes.h>
/**
 * @brief Queue report trigger attributes
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
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_QUEUE
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_QUEUE_ID = SAI_DTEL_QUEUE_REPORT_ATTR_START,

    /**
     * @brief Queue depth threshold in byte
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_DEPTH_THRESHOLD,

    /**
     * @brief Queue latency threshold in nanosecond
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_LATENCY_THRESHOLD,

    /**
     * @brief Maximum number of continuous reports after threshold breach
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1000
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_BREACH_QUOTA,

    /**
     * @brief Send report for packets dropped by the queue
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
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_INT_SESSION_ATTR_COLLECT_SWITCH_ID,

    /**
     * @brief Collect ingress and egress ports
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_INT_SESSION_ATTR_COLLECT_SWITCH_PORTS,

    /**
     * @brief Collect ingress timestamp
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_INT_SESSION_ATTR_COLLECT_INGRESS_TIMESTAMP,

    /**
     * @brief Collect egress timestamp
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DTEL_INT_SESSION_ATTR_COLLECT_EGRESS_TIMESTAMP,

    /**
     * @brief Collect queue information
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
 * @brief Telemetry report session attributes
 */
typedef enum _sai_dtel_report_session_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DTEL_REPORT_SESSION_ATTR_START,

    /**
     * @brief Telemetry report source IP address
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     */
    SAI_DTEL_REPORT_SESSION_ATTR_SRC_IP = SAI_DTEL_REPORT_SESSION_ATTR_START,

    /**
     * @brief Telemetry report destination IP addresses
     *
     * @type sai_ip_address_list_t
     * @flags CREATE_AND_SET
     */
    SAI_DTEL_REPORT_SESSION_ATTR_DST_IP_LIST,

    /**
     * @brief Telemetry report virtual router ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     */
    SAI_DTEL_REPORT_SESSION_ATTR_VIRTUAL_ROUTER_ID,

    /**
     * @brief Telemetry report truncate size
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_DTEL_REPORT_SESSION_ATTR_TRUNCATE_SIZE,

    /**
     * @brief Telemetry report UDP destination port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
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
 * @brief Enum defining DTel event types.
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
 * @brief DTel events attributes
 */
typedef enum _sai_dtel_event_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DTEL_EVENT_ATTR_START,

    /**
     * @brief DTel event type
     *
     * @type sai_dtel_event_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DTEL_EVENT_ATTR_TYPE = SAI_DTEL_EVENT_ATTR_START,

    /**
     * @brief DTel report session
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE
     * @objects SAI_OBJECT_TYPE_DTEL_REPORT_SESSION
     */
    SAI_DTEL_EVENT_ATTR_REPORT_SESSION,

    /**
     * @brief DTel report DSCP value
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE
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

typedef sai_status_t (*sai_create_dtel_queue_report_fn)(
        _Out_ sai_object_id_t *dtel_queue_report_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_dtel_queue_report_fn)(
        _In_ sai_object_id_t dtel_queue_report_id);

typedef sai_status_t (*sai_get_dtel_queue_report_attribute_fn)(
        _In_ sai_object_id_t dtel_queue_report_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_dtel_queue_report_attribute_fn)(
        _In_ sai_object_id_t dtel_queue_report_id,
        _In_ const sai_attribute_t *attr);

typedef sai_status_t (*sai_create_dtel_int_session_fn)(
        _Out_ sai_object_id_t *dtel_int_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_dtel_int_session_fn)(
        _In_ sai_object_id_t dtel_int_session_id);

typedef sai_status_t (*sai_get_dtel_int_session_attribute_fn)(
        _In_ sai_object_id_t dtel_int_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_dtel_int_session_attribute_fn)(
        _In_ sai_object_id_t dtel_int_session_id,
        _In_ const sai_attribute_t *attr);

typedef sai_status_t (*sai_create_dtel_report_session_fn)(
        _Out_ sai_object_id_t *dtel_report_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_dtel_report_session_fn)(
        _In_ sai_object_id_t dtel_report_session_id);

typedef sai_status_t (*sai_get_dtel_report_session_attribute_fn)(
        _In_ sai_object_id_t dtel_report_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_dtel_report_session_attribute_fn)(
        _In_ sai_object_id_t dtel_report_session_id,
        _In_ const sai_attribute_t *attr);

typedef sai_status_t (*sai_create_dtel_event_fn)(
        _Out_ sai_object_id_t *dtel_event_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_dtel_event_fn)(
        _In_ sai_object_id_t dtel_event_id);

typedef sai_status_t (*sai_get_dtel_event_attribute_fn)(
        _In_ sai_object_id_t dtel_event_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_dtel_event_attribute_fn)(
        _In_ sai_object_id_t dtel_event_id,
        _In_ const sai_attribute_t *attr);

typedef struct _sai_dtel_api_t
{
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
