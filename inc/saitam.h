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
 * @file    saitam.h
 *
 * @brief   This module defines SAI TAM (Telemetry And Monitoring) spec
 */

#if !defined (__SAITAM_H_)
#define __SAITAM_H_

#include <saitypes.h>

/**
 * @defgroup SAITAM SAI - Telemetry and monitoring specific API definitions
 *
 * @{
 */

/**
 * @brief TAM Attributes.
 */
typedef enum _sai_tam_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_ATTR_START,

    /**
     * @brief Tam telemetry objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_TELEMETRY
     * @default empty
     */
    SAI_TAM_ATTR_TELEMETRY_OBJECTS_LIST = SAI_TAM_ATTR_START,

    /**
     * @brief Tam event objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_EVENT
     * @default empty
     */
    SAI_TAM_ATTR_EVENT_OBJECTS_LIST,

    /**
     * @brief Tam INT objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_INT
     * @default empty
     */
    SAI_TAM_ATTR_INT_OBJECTS_LIST,

    /**
     * @brief List of TAM bind points where this object will be applied.
     *
     * TAM group bind point list - create only attribute required for TAM
     * object to let the user specify his intention to allow the
     * source to generate data.
     *
     * @type sai_s32_list_t sai_tam_bind_point_type_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_TAM_ATTR_TAM_BIND_POINT_TYPE_LIST,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_ATTR_END,

    /** Custom range base value */
    SAI_TAM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_ATTR_CUSTOM_RANGE_END

} sai_tam_attr_t;

/**
 * @brief Create and return a TAM object
 *
 * This creates a TAM object in the driver for tracking the buffer usage.
 * Via the attributes, caller may indicate a preference for tracking of a
 * specific set of statistics/groups.
 *
 * @param[out] tam_id TAM object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_fn)(
        _Out_ sai_object_id_t *tam_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified tam object.
 *
 * Deleting a TAM object also deletes all associated report and threshold objects.
 *
 * @param[in] tam_id TAM object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_fn)(
        _In_ sai_object_id_t tam_id);

/**
 * @brief Set TAM attribute value(s).
 *
 * @param[in] tam_id TAM id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_attribute_fn)(
        _In_ sai_object_id_t tam_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified TAM attributes.
 *
 * @param[in] tam_id TAM object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_attribute_fn)(
        _In_ sai_object_id_t tam_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief TAM Telemetry Math Function types
 */
typedef enum _sai_tam_tel_math_func_type_t
{
    /** None */
    SAI_TAM_TEL_MATH_FUNC_TYPE_NONE,

    /** Geometric mean */
    SAI_TAM_TEL_MATH_FUNC_TYPE_GEO_MEAN,

    /** Algebraic mean */
    SAI_TAM_TEL_MATH_FUNC_TYPE_ALGEBRAIC_MEAN,

    /** Average */
    SAI_TAM_TEL_MATH_FUNC_TYPE_AVERAGE,

    /** Statistical function Mode */
    SAI_TAM_TEL_MATH_FUNC_TYPE_MODE,

    /** Packet Rate computation */
    SAI_TAM_TEL_MATH_FUNC_TYPE_RATE,

} sai_tam_tel_math_func_type_t;

/**
 * @brief Attributes for Math function
 */
typedef enum _sai_tam_math_func_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_MATH_FUNC_ATTR_START,

    /**
     * @brief Type of math function
     *
     * @type sai_tam_tel_math_func_type_t
     * @flags CREATE_AND_SET
     * @default SAI_TAM_TEL_MATH_FUNC_TYPE_NONE
     */
    SAI_TAM_MATH_FUNC_ATTR_TAM_TEL_MATH_FUNC_TYPE = SAI_TAM_MATH_FUNC_ATTR_START,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_MATH_FUNC_ATTR_END,

    /** Custom range base value */
    SAI_TAM_MATH_FUNC_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_MATH_FUNC_ATTR_CUSTOM_RANGE_END

} sai_tam_math_func_attr_t;

/**
 * @brief Create and return a math function object
 *
 * @param[out] tam_math_func_id Object id for math function
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_math_func_fn)(
        _Out_ sai_object_id_t *tam_math_func_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified Match function object
 *
 * @param[in] tam_math_func_id Object id for math function
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_math_func_fn)(
        _In_ sai_object_id_t tam_math_func_id);

/**
 * @brief Get values for specified Math function attributes
 *
 * @param[in] tam_math_func_id Object id for math function
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_math_func_attribute_fn)(
        _In_ sai_object_id_t tam_math_func_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for specified Math function attribute
 *
 * @param[in] tam_math_func_id Object id for math function
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_math_func_attribute_fn)(
        _In_ sai_object_id_t tam_math_func_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief TAM event threshold unit
 */
typedef enum _sai_tam_event_threshold_unit_t
{
    /**
     * @brief Event threshold unit nanosecond
     */
    SAI_TAM_EVENT_THRESHOLD_UNIT_NANOSEC,

    /**
     * @brief Event threshold unit micro second
     */
    SAI_TAM_EVENT_THRESHOLD_UNIT_USEC,

    /**
     * @brief Event threshold unit millisecond
     */
    SAI_TAM_EVENT_THRESHOLD_UNIT_MSEC,

    /**
     * @brief Event threshold unit percent
     */
    SAI_TAM_EVENT_THRESHOLD_UNIT_PERCENT,

    /**
     * @brief Event threshold unit byte count
     */
    SAI_TAM_EVENT_THRESHOLD_UNIT_BYTES,

    /**
     * @brief Event threshold unit packet count
     */
    SAI_TAM_EVENT_THRESHOLD_UNIT_PACKETS,

    /**
     * @brief Event threshold unit cells
     */
    SAI_TAM_EVENT_THRESHOLD_UNIT_CELLS
} sai_tam_event_threshold_unit_t;

/**
 * @brief Event Threshold Attributes
 */
typedef enum _sai_tam_event_threshold_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_START,

    /**
     * @brief High water mark
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 90
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_HIGH_WATERMARK = SAI_TAM_EVENT_THRESHOLD_ATTR_START,

    /**
     * @brief Low Water Mark
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 10
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_LOW_WATERMARK,

    /**
     * @brief Latency in nanoseconds
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 10
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_LATENCY,

    /**
     * @brief Rate for specified event type
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_RATE,

    /**
     * @brief Abs Value for specified Event
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_ABS_VALUE,

    /**
     * @brief Tam event threshold unit
     *
     * @type sai_tam_event_threshold_unit_t
     * @flags CREATE_AND_SET
     * @default SAI_TAM_EVENT_THRESHOLD_UNIT_MSEC
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_UNIT,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_END,

    /** Custom range base value */
    SAI_TAM_EVENT_THRESHOLD_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_EVENT_THRESHOLD_ATTR_CUSTOM_RANGE_END

} sai_tam_event_threshold_attr_t;

/**
 * @brief Create and return a threshold object
 *
 * @param[out] tam_event_threshold_id Event Threshold object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_event_threshold_fn)(
        _Out_ sai_object_id_t *tam_event_threshold_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified threshold object
 *
 * @param[in] tam_event_threshold_id Event Threshold object
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_event_threshold_fn)(
        _In_ sai_object_id_t tam_event_threshold_id);

/**
 * @brief Get values for specified threshold object attributes
 *
 * @param[in] tam_event_threshold_id Event Threshold object
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_event_threshold_attribute_fn)(
        _In_ sai_object_id_t tam_event_threshold_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for a specified threshold object attribute
 *
 * @param[in] tam_event_threshold_id Event Threshold object
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_event_threshold_attribute_fn)(
        _In_ sai_object_id_t tam_event_threshold_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief TAM INT types
 */
typedef enum _sai_tam_int_type_t
{
    /**
     * @brief INT type IOAM
     */
    SAI_TAM_INT_TYPE_IOAM,

    /**
     * @brief INT type IFA1
     */
    SAI_TAM_INT_TYPE_IFA1,

    /**
     * @brief INT type IFA2
     */
    SAI_TAM_INT_TYPE_IFA2,

    /**
     * @brief INT type P4 INT v1
     */
    SAI_TAM_INT_TYPE_P4_INT_1,

    /**
     * @brief INT type P4 INT v2
     */
    SAI_TAM_INT_TYPE_P4_INT_2,

    /**
     * @brief Direct Export (aka postcard)
     */
    SAI_TAM_INT_TYPE_DIRECT_EXPORT,

    /**
     * @brief Telemetry data at the end of the packet
     */
    SAI_TAM_INT_TYPE_IFA1_TAILSTAMP,

    /**
     * @brief INT type Path Tracing
     */
    SAI_TAM_INT_TYPE_PATH_TRACING,

} sai_tam_int_type_t;

/**
 * @brief Type of indication of INT presence in a packet
 */
typedef enum _sai_tam_int_presence_type_t
{
    /**
     * @brief Indication of INT presence in a packet is undefined
     *
     * This type can be used when all indications of INT presence
     * in a packet are defined in well known specifications
     */
    SAI_TAM_INT_PRESENCE_TYPE_UNDEFINED,

    /**
     * @brief INT presence type probe marker
     */
    SAI_TAM_INT_PRESENCE_TYPE_PB,

    /**
     * @brief INT presence type L3 protocol
     */
    SAI_TAM_INT_PRESENCE_TYPE_L3_PROTOCOL,

    /**
     * @brief INT presence type DSCP
     */
    SAI_TAM_INT_PRESENCE_TYPE_DSCP

} sai_tam_int_presence_type_t;

/**
 * @brief Attributes for TAM INT
 */
typedef enum _sai_tam_int_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_INT_ATTR_START,

    /**
     * @brief Type of INT method
     *
     * @type sai_tam_int_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_INT_ATTR_TYPE = SAI_TAM_INT_ATTR_START,

    /**
     * @brief Device Identifier
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_INT_ATTR_DEVICE_ID,

    /**
     * @brief IOAM trace type
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_TAM_INT_ATTR_TYPE == SAI_TAM_INT_TYPE_IOAM
     */
    SAI_TAM_INT_ATTR_IOAM_TRACE_TYPE,

    /**
     * @brief Type of indication of INT presence in a packet
     *
     * @type sai_tam_int_presence_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_INT_ATTR_INT_PRESENCE_TYPE,

    /**
     * @brief First 4 octets of Probe Marker value that indicates INT presence
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TAM_INT_ATTR_INT_PRESENCE_TYPE == SAI_TAM_INT_PRESENCE_TYPE_PB
     */
    SAI_TAM_INT_ATTR_INT_PRESENCE_PB1,

    /**
     * @brief Second 4 octets of Probe Marker value that indicates INT presence
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TAM_INT_ATTR_INT_PRESENCE_TYPE == SAI_TAM_INT_PRESENCE_TYPE_PB
     */
    SAI_TAM_INT_ATTR_INT_PRESENCE_PB2,

    /**
     * @brief DSCP value that indicates presence of INT in a packet
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TAM_INT_ATTR_INT_PRESENCE_TYPE == SAI_TAM_INT_PRESENCE_TYPE_DSCP
     */
    SAI_TAM_INT_ATTR_INT_PRESENCE_DSCP_VALUE,

    /**
     * @brief Inline or Clone mode
     * Inline mode will insert header and metadata in live packet
     * Clone mode will insert header and metadata in cloned packet
     *
     * @type bool
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_INT_ATTR_INLINE,

    /**
     * @brief L3 protocol value that indicates presence of INT in a packet
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_TAM_INT_ATTR_INT_PRESENCE_TYPE == SAI_TAM_INT_PRESENCE_TYPE_L3_PROTOCOL
     */
    SAI_TAM_INT_ATTR_INT_PRESENCE_L3_PROTOCOL,

    /**
     * @brief Trace vector value
     * trace vector is used to specified the fields
     * of interest in metadata header
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     * @validonly SAI_TAM_INT_ATTR_TYPE == SAI_TAM_INT_TYPE_IFA1 or SAI_TAM_INT_ATTR_TYPE == SAI_TAM_INT_TYPE_IFA2
     */
    SAI_TAM_INT_ATTR_TRACE_VECTOR,

    /**
     * @brief Action vector value
     * action vector is used to specified the actions
     * of interest on metadata header
     * value of 0 means no actions of interest
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     * @validonly SAI_TAM_INT_ATTR_TYPE == SAI_TAM_INT_TYPE_IFA1 or SAI_TAM_INT_ATTR_TYPE == SAI_TAM_INT_TYPE_IFA2
     */
    SAI_TAM_INT_ATTR_ACTION_VECTOR,

    /**
     * @brief P4 INT instruction bitmap
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     * @validonly SAI_TAM_INT_ATTR_TYPE == SAI_TAM_INT_TYPE_P4_INT_1 or SAI_TAM_INT_ATTR_TYPE == SAI_TAM_INT_TYPE_P4_INT_2
     */
    SAI_TAM_INT_ATTR_P4_INT_INSTRUCTION_BITMAP,

    /**
     * @brief Enable metadata fragmentation
     *
     * When there is insufficient space in the packet to add INT
     * metadata for this hop (e.g. MTU would be exceeded), the device
     * may remove the metadata from the packet, send a report to the
     * collector, and insert its metadata before forwarding the packet.
     *
     * Note: Applicable only when SAI_TAM_INT_ATTR_TYPE != SAI_TAM_INT_TYPE_DIRECT_EXPORT
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_INT_ATTR_METADATA_FRAGMENT_ENABLE,

    /**
     * @brief Enable checksum
     *
     * Enable checksum for metadata stack
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_TAM_INT_ATTR_TYPE == SAI_TAM_INT_TYPE_IFA2
     */
    SAI_TAM_INT_ATTR_METADATA_CHECKSUM_ENABLE,

    /**
     * @brief TAM INT should report all packets without filtering
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_INT_ATTR_REPORT_ALL_PACKETS,

    /**
     * @brief TAM INT flow liveliness period in seconds
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_INT_ATTR_FLOW_LIVENESS_PERIOD,

    /**
     * @brief Latency sensitivity for flow state change detection
     * in units of 2^n nanoseconds
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 20
     */
    SAI_TAM_INT_ATTR_LATENCY_SENSITIVITY,

    /**
     * @brief INT bind point for ACL object
     *
     * Bind (or unbind) an ACL table or ACL group. Enable/Update
     * ACL table or ACL group filtering for INT insertion.
     * Disable ingress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_INT_ATTR_ACL_GROUP,

    /**
     * @brief Maximum number of hops allowed in the path
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_INT_ATTR_MAX_HOP_COUNT,

    /**
     * @brief Maximum length of metadata stack, in units of 4 octet words
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_INT_ATTR_MAX_LENGTH,

    /**
     * @brief Metadata name space ID
     * name space id defines the applicable format of metadata header
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_INT_ATTR_NAME_SPACE_ID,

    /**
     * @brief Metadata name space ID scope
     * name space id scope is global or local
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_INT_ATTR_NAME_SPACE_ID_GLOBAL,

    /**
     * @brief Enable/Disable Samplepacket session
     *
     * Enable ingress sampling by assigning samplepacket object id Disable
     * ingress sampling by assigning #SAI_NULL_OBJECT_ID as attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_SAMPLEPACKET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_INT_ATTR_INGRESS_SAMPLEPACKET_ENABLE,

    /**
     * @brief Collector object list
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_COLLECTOR
     * @default empty
     */
    SAI_TAM_INT_ATTR_COLLECTOR_LIST,

    /**
     * @brief Math function attached
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_MATH_FUNC
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_INT_ATTR_MATH_FUNC,

    /**
     * @brief Tam report type
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_REPORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_INT_ATTR_REPORT_ID,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_INT_ATTR_END,

    /** Custom range base value */
    SAI_TAM_INT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_INT_ATTR_CUSTOM_RANGE_END

} sai_tam_int_attr_t;

/**
 * @brief Create and return a INT type object
 *
 * @param[out] tam_int_id INT object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_int_fn)(
        _Out_ sai_object_id_t *tam_int_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified INT object
 *
 * @param[in] tam_int_id INT type object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_int_fn)(
        _In_ sai_object_id_t tam_int_id);

/**
 * @brief Get values for specified INT object attributes
 *
 * @param[in] tam_int_id INT object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_int_attribute_fn)(
        _In_ sai_object_id_t tam_int_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for a specified INT object attribute
 *
 * @param[in] tam_int_id INT object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_int_attribute_fn)(
        _In_ sai_object_id_t tam_int_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief TAM telemetry types supported
 */
typedef enum _sai_tam_telemetry_type_t
{
    /**
     * @brief Networking element TAM
     * All the data relevant to networking element
     * e.g. thermal, optics, switch interconnect
     */
    SAI_TAM_TELEMETRY_TYPE_NE,

    /**
     * @brief Switch silicon TAM
     * All the data relevant to switch
     * e.g. route, port, queue statistics
     */
    SAI_TAM_TELEMETRY_TYPE_SWITCH,

    /**
     * @brief Fabric TAM
     * All the data relevant to switch fabric
     */
    SAI_TAM_TELEMETRY_TYPE_FABRIC,

    /**
     * @brief Flow TAM
     * All the data relevant to a given flow
     */
    SAI_TAM_TELEMETRY_TYPE_FLOW,

    /**
     * @brief INT TAM
     * All the data relevant on a per packet basis
     */
    SAI_TAM_TELEMETRY_TYPE_INT,

    /**
     * @brief Data based on counter subscriptions
     *
     * Collect statistics for specific counters, using
     * SAI_TAM_COUNTER_SUBSCRIPTION objects
     */
    SAI_TAM_TELEMETRY_TYPE_COUNTER_SUBSCRIPTION,

} sai_tam_telemetry_type_t;

/**
 * @brief Telemetry type attributes
 */
typedef enum _sai_tam_tel_type_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_TEL_TYPE_ATTR_START,

    /**
     * @brief Telemetry type
     *
     * @type sai_tam_telemetry_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_TEL_TYPE_ATTR_TAM_TELEMETRY_TYPE = SAI_TAM_TEL_TYPE_ATTR_START,

    /**
     * @brief INT - Switch Identifier
     *
     * Switch Identifier can be an encoded number or an IP address
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_TEL_TYPE_ATTR_INT_SWITCH_IDENTIFIER,

    /**
     * @brief Switch - Collect Port stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_SWITCH_ENABLE_PORT_STATS,

    /**
     * @brief Switch - Collect Port stats ingress
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_SWITCH_ENABLE_PORT_STATS_INGRESS,

    /**
     * @brief Switch - Collect Port stats egress
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_SWITCH_ENABLE_PORT_STATS_EGRESS,

    /**
     * @brief Switch - Collect virtual queue stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_SWITCH_ENABLE_VIRTUAL_QUEUE_STATS,

    /**
     * @brief Switch - Collect output queue stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_SWITCH_ENABLE_OUTPUT_QUEUE_STATS,

    /**
     * @brief Switch - Collect MMU stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_SWITCH_ENABLE_MMU_STATS,

    /**
     * @brief Switch - Collect fabric stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_SWITCH_ENABLE_FABRIC_STATS,

    /**
     * @brief Switch - Collect filter stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_SWITCH_ENABLE_FILTER_STATS,

    /**
     * @brief Switch - Collect Resource utilization stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_SWITCH_ENABLE_RESOURCE_UTILIZATION_STATS,

    /**
     * @brief Fabric - Collect Queue information
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_FABRIC_Q,

    /**
     * @brief NE - Collect information of networking element
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_NE_ENABLE,

    /**
     * @brief DSCP value
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_TEL_TYPE_ATTR_DSCP_VALUE,

    /**
     * @brief Math function attached
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_MATH_FUNC
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_TEL_TYPE_ATTR_MATH_FUNC,

    /**
     * @brief Tam report type
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_REPORT
     */
    SAI_TAM_TEL_TYPE_ATTR_REPORT_ID,

    /**
     * @brief List of Tam counter subscription objects
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_COUNTER_SUBSCRIPTION
     */
    SAI_TAM_TEL_TYPE_ATTR_COUNTER_SUBSCRIPTION_LIST,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_TEL_TYPE_ATTR_END,

    /** Custom range base value */
    SAI_TAM_TEL_TYPE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_TEL_TYPE_ATTR_CUSTOM_RANGE_END
} sai_tam_tel_type_attr_t;

/**
 * @brief Create and return a telemetry type object
 *
 * @param[out] tam_tel_type_id Telemetry type object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_tel_type_fn)(
        _Out_ sai_object_id_t *tam_tel_type_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified telemetry type object
 *
 * @param[in] tam_tel_type_id Telemetry type object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_tel_type_fn)(
        _In_ sai_object_id_t tam_tel_type_id);

/**
 * @brief Get values for specified telemetry type object attributes
 *
 * @param[in] tam_tel_type_id Telemetry type object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_tel_type_attribute_fn)(
        _In_ sai_object_id_t tam_tel_type_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for a specified telemetry type object attribute
 *
 * @param[in] tam_tel_type_id Telemetry type object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_tel_type_attribute_fn)(
        _In_ sai_object_id_t tam_tel_type_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief TAM report types
 */
typedef enum _sai_tam_report_type_t
{
    /**
     * @brief Report using SFLOW
     */
    SAI_TAM_REPORT_TYPE_SFLOW,

    /**
     * @brief Report using IPFIX
     */
    SAI_TAM_REPORT_TYPE_IPFIX,

    /**
     * @brief Report using GPB
     */
    SAI_TAM_REPORT_TYPE_PROTO,

    /**
     * @brief Report using THRIFT
     */
    SAI_TAM_REPORT_TYPE_THRIFT,

    /**
     * @brief Report using JSON
     */
    SAI_TAM_REPORT_TYPE_JSON,

    /**
     * @brief Report using P4 format
     */
    SAI_TAM_REPORT_TYPE_P4_EXTN,

    /**
     * @brief Report using Histogram
     */
    SAI_TAM_REPORT_TYPE_HISTOGRAM,

    /**
     * @brief Report using vendor extensions
     */
    SAI_TAM_REPORT_TYPE_VENDOR_EXTN,
} sai_tam_report_type_t;

/**
 * @brief Enum defining reporting modes.
 */
typedef enum _sai_tam_report_mode_t
{
    /** Report all events */
    SAI_TAM_REPORT_MODE_ALL,

    /** Report in a bulk mode */
    SAI_TAM_REPORT_MODE_BULK,

} sai_tam_report_mode_t;

/**
 * @brief TAM report interval units
 */
typedef enum _sai_tam_report_interval_unit_t
{
    /**
     * @brief Report interval unit nanosecond
     */
    SAI_TAM_REPORT_INTERVAL_UNIT_NANOSEC,

    /**
     * @brief Report interval unit microsecond
     */
    SAI_TAM_REPORT_INTERVAL_UNIT_USEC,

    /**
     * @brief Report interval unit millisecond
     */
    SAI_TAM_REPORT_INTERVAL_UNIT_MSEC,

} sai_tam_report_interval_unit_t;

/**
 * @brief Attributes for TAM report
 */
typedef enum _sai_tam_report_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_REPORT_ATTR_START,

    /**
     * @brief Type of reporting method
     *
     * @type sai_tam_report_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_TAM_REPORT_ATTR_TYPE = SAI_TAM_REPORT_ATTR_START,

    /**
     * @brief Statistic for this histogram
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     * @validonly SAI_TAM_REPORT_ATTR_TYPE == SAI_TAM_REPORT_TYPE_HISTOGRAM
     */
    SAI_TAM_REPORT_ATTR_HISTOGRAM_NUMBER_OF_BINS,

    /**
     * @brief Histogram Bins Lower Boundaries
     *
     * List of lower boundary of each bin for this HISTOGRAM in
     * number referred object units. The upper boundary of a bin is
     * the lower boundary of next bin. The upper boundary of the
     * last bin is infinity.
     *
     * @type sai_u32_list_t
     * @flags CREATE_ONLY
     * @default empty
     * @validonly SAI_TAM_REPORT_ATTR_TYPE == SAI_TAM_REPORT_TYPE_HISTOGRAM
     */
    SAI_TAM_REPORT_ATTR_HISTOGRAM_BIN_BOUNDARY,

    /**
     * @brief Maximum number of reports to generate after an event
     *
     * Note: The value 0 indicates that there is no quota
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_REPORT_ATTR_QUOTA,

    /**
     * @brief Report Mode
     *
     * @type sai_tam_report_mode_t
     * @flags CREATE_ONLY
     * @default SAI_TAM_REPORT_MODE_ALL
     */
    SAI_TAM_REPORT_ATTR_REPORT_MODE,

    /**
     * @brief Report Interval
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1000
     * @validonly SAI_TAM_REPORT_ATTR_REPORT_MODE == SAI_TAM_REPORT_MODE_BULK
     */
    SAI_TAM_REPORT_ATTR_REPORT_INTERVAL,

    /**
     * @brief Enterprise number
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_TAM_REPORT_ATTR_TYPE == SAI_TAM_REPORT_TYPE_IPFIX
     */
    SAI_TAM_REPORT_ATTR_ENTERPRISE_NUMBER,

    /**
     * @brief Template report interval in minutes
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 15
     * @validonly SAI_TAM_REPORT_ATTR_TYPE == SAI_TAM_REPORT_TYPE_IPFIX
     */
    SAI_TAM_REPORT_ATTR_TEMPLATE_REPORT_INTERVAL,

    /**
     * @brief Report Interval Units
     *
     * @type sai_tam_report_interval_unit_t
     * @flags CREATE_AND_SET
     * @default SAI_TAM_REPORT_INTERVAL_UNIT_USEC
     * @validonly SAI_TAM_REPORT_ATTR_REPORT_MODE == SAI_TAM_REPORT_MODE_BULK
     */
    SAI_TAM_REPORT_ATTR_REPORT_INTERVAL_UNIT,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_REPORT_ATTR_END,

    /** Custom range base value */
    SAI_TAM_REPORT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_REPORT_ATTR_CUSTOM_RANGE_END

} sai_tam_report_attr_t;

/**
 * @brief Create and return a report object id
 *
 * @param[out] tam_report_id Report object Id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_report_fn)(
        _Out_ sai_object_id_t *tam_report_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified report object
 *
 * @param[in] tam_report_id Report object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_report_fn)(
        _In_ sai_object_id_t tam_report_id);

/**
 * @brief Get values for specified report object attributes
 *
 * @param[in] tam_report_id Report object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_report_attribute_fn)(
        _In_ sai_object_id_t tam_report_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for a specified report object attribute
 *
 * @param[in] tam_report_id Report object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_report_attribute_fn)(
        _In_ sai_object_id_t tam_report_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief TAM reporting unit
 */
typedef enum _sai_tam_reporting_unit_t
{
    /**
     * @brief Report Unit second
     */
    SAI_TAM_REPORTING_UNIT_SEC,

    /**
     * @brief Report unit minute
     */
    SAI_TAM_REPORTING_UNIT_MINUTE,

    /**
     * @brief Report unit hour
     */
    SAI_TAM_REPORTING_UNIT_HOUR,

    /**
     * @brief Report unit day
     */
    SAI_TAM_REPORTING_UNIT_DAY

} sai_tam_reporting_unit_t;

/**
 * @brief TAM telemetry attributes
 */
typedef enum _sai_tam_telemetry_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_TELEMETRY_ATTR_START,

    /**
     * @brief TAM tel type object list
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_TEL_TYPE
     * @default empty
     */
    SAI_TAM_TELEMETRY_ATTR_TAM_TYPE_LIST = SAI_TAM_TELEMETRY_ATTR_START,

    /**
     * @brief Collector object list
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_COLLECTOR
     * @default empty
     */
    SAI_TAM_TELEMETRY_ATTR_COLLECTOR_LIST,

    /**
     * @brief Tam telemetry reporting unit
     *
     * @type sai_tam_reporting_unit_t
     * @flags CREATE_AND_SET
     * @default SAI_TAM_REPORTING_UNIT_SEC
     */
    SAI_TAM_TELEMETRY_ATTR_TAM_REPORTING_UNIT,

    /**
     * @brief Tam event reporting interval
     *
     * defines the gap between two reports
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1
     */
    SAI_TAM_TELEMETRY_ATTR_REPORTING_INTERVAL,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_TELEMETRY_ATTR_END,

    /** Custom range base value */
    SAI_TAM_TELEMETRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_TELEMETRY_ATTR_CUSTOM_RANGE_END

} sai_tam_telemetry_attr_t;

/**
 * @brief Create and return a telemetry object
 *
 * @param[out] tam_telemetry_id Telemetry object id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_telemetry_fn)(
        _Out_ sai_object_id_t *tam_telemetry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified telemetry object
 *
 * @param[in] tam_telemetry_id Telemetry object
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_telemetry_fn)(
        _In_ sai_object_id_t tam_telemetry_id);

/**
 * @brief Get values for specified telemetry object attributes
 *
 * @param[in] tam_telemetry_id Telemetry object
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_telemetry_attribute_fn)(
        _In_ sai_object_id_t tam_telemetry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for a specified telemetry object attribute
 *
 * @param[in] tam_telemetry_id Telemetry object
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_telemetry_attribute_fn)(
        _In_ sai_object_id_t tam_telemetry_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Transport Types
 */
typedef enum _sai_tam_transport_type_t
{
    /**
     * @brief Transport None
     * This is usually used for local host
     */
    SAI_TAM_TRANSPORT_TYPE_NONE,

    /**
     * @brief Transport TCP
     */
    SAI_TAM_TRANSPORT_TYPE_TCP,

    /**
     * @brief Transport UDP
     */
    SAI_TAM_TRANSPORT_TYPE_UDP,

    /**
     * @brief Transport GRPC
     */
    SAI_TAM_TRANSPORT_TYPE_GRPC,

    /**
     * @brief Transport forwarding to port
     */
    SAI_TAM_TRANSPORT_TYPE_PORT,
} sai_tam_transport_type_t;

/**
 * @brief Transport Authentication Types
 */
typedef enum _sai_tam_transport_auth_type_t
{
    /**
     * @brief No Authentication
     */
    SAI_TAM_TRANSPORT_AUTH_TYPE_NONE,

    /**
     * @brief Authenticate using SSL
     */
    SAI_TAM_TRANSPORT_AUTH_TYPE_SSL,

    /**
     * @brief Authenticate using TLS
     */
    SAI_TAM_TRANSPORT_AUTH_TYPE_TLS

} sai_tam_transport_auth_type_t;

/**
 * @brief Transport object Attributes
 */
typedef enum _sai_tam_transport_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_TRANSPORT_ATTR_START,

    /**
     * @brief Transport type
     *
     * @type sai_tam_transport_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE = SAI_TAM_TRANSPORT_ATTR_START,

    /**
     * @brief Transport src port
     * Value of -1 can be used a hint to compute ephemeral
     * or entropy value
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 31337
     */
    SAI_TAM_TRANSPORT_ATTR_SRC_PORT,

    /**
     * @brief Transport dst port
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 31337
     */
    SAI_TAM_TRANSPORT_ATTR_DST_PORT,

    /**
     * @brief Transport authentication
     *
     * @type sai_tam_transport_auth_type_t
     * @flags CREATE_AND_SET
     * @default SAI_TAM_TRANSPORT_AUTH_TYPE_NONE
     */
    SAI_TAM_TRANSPORT_ATTR_TRANSPORT_AUTH_TYPE,

    /**
     * @brief Transport MTU size
     * Driver must ensure the size of packet do not exceed MTU size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1500
     */
    SAI_TAM_TRANSPORT_ATTR_MTU,

    /**
     * @brief L2 source MAC address
     *
     * @type sai_mac_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE == SAI_TAM_TRANSPORT_TYPE_PORT
     */
    SAI_TAM_TRANSPORT_ATTR_SRC_MAC_ADDRESS,

    /**
     * @brief L2 destination MAC address
     *
     * @type sai_mac_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE == SAI_TAM_TRANSPORT_TYPE_PORT
     */
    SAI_TAM_TRANSPORT_ATTR_DST_MAC_ADDRESS,

    /**
     * @brief L2 header TPID.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0x8100
     * @validonly SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE == SAI_TAM_TRANSPORT_TYPE_PORT
     */
    SAI_TAM_TRANSPORT_ATTR_VLAN_TPID,

    /**
     * @brief L2 header VLAN Id.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     * @validonly SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE == SAI_TAM_TRANSPORT_TYPE_PORT
     */
    SAI_TAM_TRANSPORT_ATTR_VLAN_ID,

    /**
     * @brief L2 header packet priority (3 bits).
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE == SAI_TAM_TRANSPORT_TYPE_PORT
     */
    SAI_TAM_TRANSPORT_ATTR_VLAN_PRI,

    /**
     * @brief L2 header Vlan CFI (1 bit).
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE == SAI_TAM_TRANSPORT_TYPE_PORT
     */
    SAI_TAM_TRANSPORT_ATTR_VLAN_CFI,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_TRANSPORT_ATTR_END,

    /** Custom range base value */
    SAI_TAM_TRANSPORT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_TRANSPORT_ATTR_CUSTOM_RANGE_END

} sai_tam_transport_attr_t;

/**
 * @brief Create and return a transport object id
 *
 * @param[out] tam_transport_id Transport object Id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_transport_fn)(
        _Out_ sai_object_id_t *tam_transport_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified transport object
 *
 * @param[in] tam_transport_id Transport object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_transport_fn)(
        _In_ sai_object_id_t tam_transport_id);

/**
 * @brief Get values for specified transport object attributes
 *
 * @param[in] tam_transport_id Transport object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_transport_attribute_fn)(
        _In_ sai_object_id_t tam_transport_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for a specified transport object attribute
 *
 * @param[in] tam_transport_id Transport object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_transport_attribute_fn)(
        _In_ sai_object_id_t tam_transport_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief TAM collector attributes
 */
typedef enum _sai_tam_collector_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_COLLECTOR_ATTR_START,

    /**
     * @brief Source IP address
     *
     * Note: Applicable only when SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE != SAI_TAM_TRANSPORT_TYPE_NONE
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_TAM_COLLECTOR_ATTR_SRC_IP = SAI_TAM_COLLECTOR_ATTR_START,

    /**
     * @brief Destination IP addresses
     *
     * Note: Applicable only when SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE != SAI_TAM_TRANSPORT_TYPE_NONE
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_TAM_COLLECTOR_ATTR_DST_IP,

    /**
     * @brief Destination local CPU
     *
     * Note: Applicable only when SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE == SAI_TAM_TRANSPORT_TYPE_NONE
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_TAM_COLLECTOR_ATTR_LOCALHOST,

    /**
     * @brief Virtual router ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_COLLECTOR_ATTR_VIRTUAL_ROUTER_ID,

    /**
     * @brief Telemetry report truncate size
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_COLLECTOR_ATTR_TRUNCATE_SIZE,

    /**
     * @brief Transport attributes object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_TRANSPORT
     */
    SAI_TAM_COLLECTOR_ATTR_TRANSPORT,

    /**
     * @brief DSCP value
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_TAM_COLLECTOR_ATTR_DSCP_VALUE,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_COLLECTOR_ATTR_END,

    /** Custom range base value */
    SAI_TAM_COLLECTOR_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_COLLECTOR_ATTR_CUSTOM_RANGE_END

} sai_tam_collector_attr_t;

/**
 * @brief Create and return a collector object id
 *
 * @param[out] tam_collector_id Collector object Id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_collector_fn)(
        _Out_ sai_object_id_t *tam_collector_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified collector object
 *
 * @param[in] tam_collector_id Collector object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_collector_fn)(
        _In_ sai_object_id_t tam_collector_id);

/**
 * @brief Get values for specified collector object attributes
 *
 * @param[in] tam_collector_id Collector object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_collector_attribute_fn)(
        _In_ sai_object_id_t tam_collector_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for a specified collector object attribute
 *
 * @param[in] tam_collector_id Collector object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_collector_attribute_fn)(
        _In_ sai_object_id_t tam_collector_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief TAM Packet Ingress Drop Types
 */
typedef enum _sai_packet_drop_type_ingress_t
{
    /** None */
    SAI_PACKET_DROP_TYPE_INGRESS_NONE,

    /** ALL */
    SAI_PACKET_DROP_TYPE_INGRESS_ALL,

    /** Flags drops */
    SAI_PACKET_DROP_TYPE_INGRESS_CML_FLAGS_DROP,

    /** Layer 2 source static move drops */
    SAI_PACKET_DROP_TYPE_INGRESS_L2_SRC_STATIC_MOVE,

    /** Layer 2 discard drops */
    SAI_PACKET_DROP_TYPE_INGRESS_L2_SRC_DISCARD,

    /** MAC multicast drops */
    SAI_PACKET_DROP_TYPE_INGRESS_MACSA_MULTICAST,

    /** TPID check fail drops */
    SAI_PACKET_DROP_TYPE_INGRESS_OUTER_TPID_CHECK_FAILED,

    /** Port vlan check fail drops */
    SAI_PACKET_DROP_TYPE_INGRESS_INCOMING_PVLAN_CHECK_FAILED,

    /** Packet integrity check fail drops */
    SAI_PACKET_DROP_TYPE_INGRESS_PKT_INTEGRITY_CHECK_FAILED,

    /** Unknown protocol drops */
    SAI_PACKET_DROP_TYPE_INGRESS_PROTOCOL_PKT_DROP,

    /** VLAN membership check fail drops */
    SAI_PACKET_DROP_TYPE_INGRESS_MEMBERSHIP_CHECK_FAILED,

    /** Spanning tree check fail drops */
    SAI_PACKET_DROP_TYPE_INGRESS_SPANNING_TREE_CHECK_FAILED,

    /** Layer 2 destination lookup miss drops */
    SAI_PACKET_DROP_TYPE_INGRESS_L2_DST_LOOKUP_MISS,

    /** Layer 2 destination discard drops */
    SAI_PACKET_DROP_TYPE_INGRESS_L2_DST_DISCARD,

    /** Layer 3 destination lookup miss drops */
    SAI_PACKET_DROP_TYPE_INGRESS_L3_DST_LOOKUP_MISS,

    /** Layer 3 destination discard drops */
    SAI_PACKET_DROP_TYPE_INGRESS_L3_DST_DISCARD,

    /** Layer 3 header error drops */
    SAI_PACKET_DROP_TYPE_INGRESS_L3_HDR_ERROR,

    /** Layer 3 IP TTL error drops */
    SAI_PACKET_DROP_TYPE_INGRESS_L3_TTL_ERROR,

    /** RPF check fail drops */
    SAI_PACKET_DROP_TYPE_INGRESS_IPMC_L3_IIF_OR_RPA_ID_CHECK_FAILED,

    /** Tunnel TTL check fail drops */
    SAI_PACKET_DROP_TYPE_INGRESS_TUNNEL_TTL_CHECK_FAILED,

    /** Invalid tunnel object drops */
    SAI_PACKET_DROP_TYPE_INGRESS_TUNNEL_OBJECT_VALIDATION_FAILED,

    /** Invalid tunnel adaptation drops */
    SAI_PACKET_DROP_TYPE_INGRESS_TUNNEL_ADAPT_DROP,

    /** Port VLAN drops */
    SAI_PACKET_DROP_TYPE_INGRESS_PVLAN_DROP,

    /** Filter drops */
    SAI_PACKET_DROP_TYPE_INGRESS_VFP,

    /** Filter drops */
    SAI_PACKET_DROP_TYPE_INGRESS_IFP,

    /** Filter based metering drops */
    SAI_PACKET_DROP_TYPE_INGRESS_IFP_METER,

    /** Destination filter drops */
    SAI_PACKET_DROP_TYPE_INGRESS_DST_FP,

    /** MPLS protection drops */
    SAI_PACKET_DROP_TYPE_INGRESS_MPLS_PROTECTION_DROP,

    /** Invalid MPLS label action drops */
    SAI_PACKET_DROP_TYPE_INGRESS_MPLS_LABEL_ACTION_INVALID,

    /** MPLS termination table drops */
    SAI_PACKET_DROP_TYPE_INGRESS_MPLS_TERM_POLICY_SELECT_TABLE_DROP,

    /** MPLS reserved label drops */
    SAI_PACKET_DROP_TYPE_INGRESS_MPLS_RESERVED_LABEL_EXPOSED,

    /** MPLS TTL error drops */
    SAI_PACKET_DROP_TYPE_INGRESS_MPLS_TTL_ERROR,

    /** MPLS ECN drops */
    SAI_PACKET_DROP_TYPE_INGRESS_MPLS_ECN_ERROR,

    /** Exact match table drops */
    SAI_PACKET_DROP_TYPE_INGRESS_EM_FT,

    /** VLAN translation table drops */
    SAI_PACKET_DROP_TYPE_INGRESS_IVXLT,

    /** Re-circulation mirror drops */
    SAI_PACKET_DROP_TYPE_INGRESS_EP_RECIRC_MIRROR_DROP,

    /** IPv4 gateway drops */
    SAI_PACKET_DROP_TYPE_INGRESS_MTOP_IPV4_GATEWAY,

    /** Reverse RPF check drops */
    SAI_PACKET_DROP_TYPE_INGRESS_URPF_CHECK_FAILED,

    /** Source port knockout drops */
    SAI_PACKET_DROP_TYPE_INGRESS_SRC_PORT_KNOCKOUT_DROP,

    /** LAG fail over port down drops */
    SAI_PACKET_DROP_TYPE_INGRESS_LAG_FAILOVER_PORT_DOWN,

    /** Split horizon check fail drops */
    SAI_PACKET_DROP_TYPE_INGRESS_SPLIT_HORIZON_CHECK_FAILED,

    /** Destination link down drops */
    SAI_PACKET_DROP_TYPE_INGRESS_DST_LINK_DOWN,

    /** Port block mask drops */
    SAI_PACKET_DROP_TYPE_INGRESS_BLOCK_MASK_DROP,

    /** Layer 3 MTU check fail drops */
    SAI_PACKET_DROP_TYPE_INGRESS_L3_MTU_CHECK_FAILED,

    /** Sequence number check fail drops */
    SAI_PACKET_DROP_TYPE_INGRESS_SEQ_NUM_CHECK_FAILED,

    /** Ingress interface same as egress interface drops */
    SAI_PACKET_DROP_TYPE_INGRESS_L3_IIF_EQ_L3_OIF,

    /** Storm control drops */
    SAI_PACKET_DROP_TYPE_INGRESS_STORM_CONTROL_DROP,

    /** Membership check fail drops */
    SAI_PACKET_DROP_TYPE_INGRESS_EGR_MEMBERSHIP_CHECK_FAILED,

    /** Spanning tree check fail drops */
    SAI_PACKET_DROP_TYPE_INGRESS_EGR_SPANNING_TREE_CHECK_FAILED,

    /** Port block mode drops */
    SAI_PACKET_DROP_TYPE_INGRESS_DST_PBM_ZERO,

    /** MPLS control packet drops */
    SAI_PACKET_DROP_TYPE_INGRESS_MPLS_CTRL_PKT_DROP,
} sai_packet_drop_type_ingress_t;

/**
 * @brief TAM Packet MMU Drop Types
 */
typedef enum _sai_packet_drop_type_mmu_t
{
    /** None */
    SAI_PACKET_DROP_TYPE_MMU_NONE,

    /** ALL */
    SAI_PACKET_DROP_TYPE_MMU_ALL,

    /** Port group limit */
    SAI_PACKET_DROP_TYPE_MMU_ING_PG_LIMIT,

    /** Ingress port service pool limit */
    SAI_PACKET_DROP_TYPE_MMU_ING_PORTSP_LIMIT,

    /** Headroom pool limit */
    SAI_PACKET_DROP_TYPE_MMU_ING_HEADROOM_POOL_LIMIT,

    /** Egress queue limit */
    SAI_PACKET_DROP_TYPE_MMU_EGR_QUEUE_LIMIT,

    /** Egress port service pool limit */
    SAI_PACKET_DROP_TYPE_MMU_EGR_PORTSP_LIMIT,

    /** WRED check */
    SAI_PACKET_DROP_TYPE_MMU_WRED_CHECKS,
} sai_packet_drop_type_mmu_t;

/**
 * @brief TAM Packet Egress Drop Types
 */
typedef enum _sai_packet_drop_type_egress_t
{
    /** None */
    SAI_PACKET_DROP_TYPE_EGRESS_NONE,

    /** ALL */
    SAI_PACKET_DROP_TYPE_EGRESS_ALL,

    /** Layer 2 output interface drops */
    SAI_PACKET_DROP_TYPE_EGRESS_L2_OIF,

    /** Membership drops */
    SAI_PACKET_DROP_TYPE_EGRESS_MEMBERSHIP,

    /** Membership drops */
    SAI_PACKET_DROP_TYPE_EGRESS_DVP_MEMBERSHIP,

    /** TTL check drops */
    SAI_PACKET_DROP_TYPE_EGRESS_TTL,

    /** Layer 3 same interface drops */
    SAI_PACKET_DROP_TYPE_EGRESS_L3_SAME_INTF,

    /** Layer 2 same interface drops */
    SAI_PACKET_DROP_TYPE_EGRESS_L2_SAME_PORT,

    /** Split horizon drops */
    SAI_PACKET_DROP_TYPE_EGRESS_SPLIT_HORIZON,

    /** Spanning tree disabled drops */
    SAI_PACKET_DROP_TYPE_EGRESS_STG_DISABLE,

    /** Spanning tree blocked drops */
    SAI_PACKET_DROP_TYPE_EGRESS_STG_BLOCK,

    /** Filter drops */
    SAI_PACKET_DROP_TYPE_EGRESS_EFP,

    /** Metering drops */
    SAI_PACKET_DROP_TYPE_EGRESS_EGR_METER,

    /** MTU check drops */
    SAI_PACKET_DROP_TYPE_EGRESS_EGR_MTU,

    /** VLAN translation table drops */
    SAI_PACKET_DROP_TYPE_EGRESS_EGR_VXLT,

    /** Cell size large drops */
    SAI_PACKET_DROP_TYPE_EGRESS_EGR_CELL_TOO_LARGE,

    /** Cell size small drops */
    SAI_PACKET_DROP_TYPE_EGRESS_EGR_CELL_TOO_SMALL,

    /** QOS remarking drops */
    SAI_PACKET_DROP_TYPE_EGRESS_QOS_REMARKING,

    /** Same membership drops */
    SAI_PACKET_DROP_TYPE_EGRESS_SVP_EQUAL_DVP,

    /** Invalid 1588 packet drops */
    SAI_PACKET_DROP_TYPE_EGRESS_INVALID_1588_PKT,

    /** Flex edit drops */
    SAI_PACKET_DROP_TYPE_EGRESS_FLEX_EDITOR,

    /** Egress re-circulation drops */
    SAI_PACKET_DROP_TYPE_EGRESS_EP_RECIRC,

    /** IFA metadata delete drops */
    SAI_PACKET_DROP_TYPE_EGRESS_IFA_MD_DELETE,
} sai_packet_drop_type_egress_t;

/**
 * @brief TAM Switch Event Types
 */
typedef enum _sai_switch_event_type_t
{
    /** None */
    SAI_SWITCH_EVENT_TYPE_NONE,

    /** ALL */
    SAI_SWITCH_EVENT_TYPE_ALL,

    /** Stable Full */
    SAI_SWITCH_EVENT_TYPE_STABLE_FULL,

    /** Stable Error */
    SAI_SWITCH_EVENT_TYPE_STABLE_ERROR,

    /** Uncontrolled Shutdown */
    SAI_SWITCH_EVENT_TYPE_UNCONTROLLED_SHUTDOWN,

    /** Downgrade during Warm Boot */
    SAI_SWITCH_EVENT_TYPE_WARM_BOOT_DOWNGRADE,

    /** Parity Error */
    SAI_SWITCH_EVENT_TYPE_PARITY_ERROR,
} sai_switch_event_type_t;

/**
 * @brief Enum defining event types.
 */
typedef enum _sai_tam_event_type_t
{
    /**
     * @brief New flow or flow state change event
     * This event is used to monitoring the state of flow
     * A flow can be learned, aged, or classified
     */
    SAI_TAM_EVENT_TYPE_FLOW_STATE,

    /**
     * @brief Watchlist event
     * Instead of a single flow, a group flows can be monitored
     */
    SAI_TAM_EVENT_TYPE_FLOW_WATCHLIST,

    /**
     * @brief Flow TCP FLAGS event
     * All TCP Flags are monitored for change
     */
    SAI_TAM_EVENT_TYPE_FLOW_TCPFLAG,

    /**
     * @brief Queue depth or latency threshold event
     * Queue occupancy threshold
     */
    SAI_TAM_EVENT_TYPE_QUEUE_THRESHOLD,

    /**
     * @brief Queue tail drop event
     * Number of packets dropped as tail drops because
     * the queue is full
     */
    SAI_TAM_EVENT_TYPE_QUEUE_TAIL_DROP,

    /**
     * @brief Type of packet drops
     * Simple drop monitoring of packets
     */
    SAI_TAM_EVENT_TYPE_PACKET_DROP,

    /**
     * @brief Packet drop event
     * State aware drop monitoring of packets
     */
    SAI_TAM_EVENT_TYPE_PACKET_DROP_STATEFUL,

    /**
     * @brief Switch resource utilization threshold event
     * Any resource utilization when exceeds a threshold
     * For example, route table if 90% full can generate an event
     */
    SAI_TAM_EVENT_TYPE_RESOURCE_UTILIZATION,

    /**
     * @brief Ingress priority group shared occupancy threshold event
     */
    SAI_TAM_EVENT_TYPE_IPG_SHARED,

    /**
     * @brief Ingress priority group XOFF room threshold event
     */
    SAI_TAM_EVENT_TYPE_IPG_XOFF_ROOM,

    /**
     * @brief Buffer service pool threshold event
     */
    SAI_TAM_EVENT_TYPE_BSP,

    /**
     * @brief Flow latency monitoring event
     */
    SAI_TAM_EVENT_TYPE_FLOW_LATENCY,

    /**
     * @brief Switch monitoring event
     */
    SAI_TAM_EVENT_TYPE_SWITCH,
} sai_tam_event_type_t;

/**
 * @brief Enum defining event types.
 */
typedef enum _sai_tam_event_action_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_EVENT_ACTION_ATTR_START,

    /**
     * @brief Report Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_REPORT
     */
    SAI_TAM_EVENT_ACTION_ATTR_REPORT_TYPE = SAI_TAM_EVENT_ACTION_ATTR_START,

    /**
     * @brief QOS action Type Object
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_EVENT_ACTION_ATTR_QOS_ACTION_TYPE,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_EVENT_ACTION_ATTR_END,

    /** Custom range base value */
    SAI_TAM_EVENT_ACTION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_EVENT_ACTION_ATTR_CUSTOM_RANGE_END

} sai_tam_event_action_attr_t;

/**
 * @brief Create and return a event action object id
 *
 * @param[out] tam_event_action_id Event object Id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_event_action_fn)(
        _Out_ sai_object_id_t *tam_event_action_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified event object
 *
 * @param[in] tam_event_action_id Event object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_event_action_fn)(
        _In_ sai_object_id_t tam_event_action_id);

/**
 * @brief Get values for specified event object attributes
 *
 * @param[in] tam_event_action_id Event object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_event_action_attribute_fn)(
        _In_ sai_object_id_t tam_event_action_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for a specified event object attribute
 *
 * @param[in] tam_event_action_id Event object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_event_action_attribute_fn)(
        _In_ sai_object_id_t tam_event_action_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Tam event attributes
 */
typedef enum _sai_tam_event_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_EVENT_ATTR_START,

    /**
     * @brief Tam event type
     *
     * @type sai_tam_event_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_EVENT_ATTR_TYPE = SAI_TAM_EVENT_ATTR_START,

    /**
     * @brief Type of ingress packet drops
     *
     * @type sai_s32_list_t sai_packet_drop_type_ingress_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP or SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP_STATEFUL
     */
    SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE_INGRESS,

    /**
     * @brief Type of MMU packet drops
     *
     * @type sai_s32_list_t sai_packet_drop_type_mmu_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP or SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP_STATEFUL
     */
    SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE_MMU,

    /**
     * @brief Type of egress packet drops
     *
     * @type sai_s32_list_t sai_packet_drop_type_egress_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP or SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP_STATEFUL
     */
    SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE_EGRESS,

    /**
     * @brief Type of switch event
     *
     * @type sai_s32_list_t sai_switch_event_type_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_SWITCH
     */
    SAI_TAM_EVENT_ATTR_SWITCH_EVENT_TYPE,

    /**
     * @brief Event action
     *
     * @type sai_object_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_EVENT_ACTION
     */
    SAI_TAM_EVENT_ATTR_ACTION_LIST,

    /**
     * @brief Collector object list
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_COLLECTOR
     * @default empty
     */
    SAI_TAM_EVENT_ATTR_COLLECTOR_LIST,

    /**
     * @brief Tam event threshold attr Object
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_EVENT_THRESHOLD
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_EVENT_ATTR_THRESHOLD,

    /**
     * @brief DSCP value
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_EVENT_ATTR_DSCP_VALUE,

    /**
     * @brief Aging interval (in milliseconds) for an event
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_EVENT_ATTR_AGING_INTERVAL,

    /**
     * @brief Enable/Disable Samplepacket session
     *
     * Enable ingress sampling by assigning samplepacket object id Disable
     * ingress sampling by assigning #SAI_NULL_OBJECT_ID as attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_SAMPLEPACKET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_EVENT_ATTR_INGRESS_SAMPLEPACKET_ENABLE,

    /**
     * @brief Device Identifier
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_EVENT_ATTR_DEVICE_ID,

    /**
     * @brief Event Identifier
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_EVENT_ATTR_EVENT_ID,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_EVENT_ATTR_END,

    /** Custom range base value */
    SAI_TAM_EVENT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_EVENT_ATTR_CUSTOM_RANGE_END

} sai_tam_event_attr_t;

/**
 * @brief Create and return a event object id
 *
 * @param[out] tam_event_id Event object Id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_event_fn)(
        _Out_ sai_object_id_t *tam_event_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified event object
 *
 * @param[in] tam_event_id Event object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_event_fn)(
        _In_ sai_object_id_t tam_event_id);

/**
 * @brief Get values for specified event object attributes
 *
 * @param[in] tam_event_id Event object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_event_attribute_fn)(
        _In_ sai_object_id_t tam_event_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for a specified event object attribute
 *
 * @param[in] tam_event_id Event object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_event_attribute_fn)(
        _In_ sai_object_id_t tam_event_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Counter Subscription attributes
 */
typedef enum _sai_tam_counter_subscription_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_START,

    /**
     * @brief TAM telemetry type object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_TEL_TYPE
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_TEL_TYPE = SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_START,

    /**
     * @brief Subscribed object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BUFFER_POOL, SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP, SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_QUEUE
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_OBJECT_ID,

    /**
     * @brief Subscribed stat enum
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_STAT_ID,

    /**
     * @brief Telemetry label
     *
     * Label to identify this counter in telemetry reports.
     *
     * @type sai_uint64_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_LABEL,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_END,

    /** Custom range base value */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_CUSTOM_RANGE_END

} sai_tam_counter_subscription_attr_t;

/**
 * @brief Create a counter subscription
 *
 * @param[out] tam_counter_subscription_id Counter subscription object Id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_counter_subscription_fn)(
        _Out_ sai_object_id_t *tam_counter_subscription_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a specified counter subscription
 *
 * @param[in] tam_counter_subscription_id Counter Subscription object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_counter_subscription_fn)(
        _In_ sai_object_id_t tam_counter_subscription_id);

/**
 * @brief Set value for a specified counter subscription object attribute
 *
 * @param[in] tam_counter_subscription_id Counter Subscription object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_counter_subscription_attribute_fn)(
        _In_ sai_object_id_t tam_counter_subscription_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified event object attributes
 *
 * @param[in] tam_counter_subscription_id Counter Subscription object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_counter_subscription_attribute_fn)(
        _In_ sai_object_id_t tam_counter_subscription_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief TAM event callback
 *
 * @count attr_list[attr_count]
 * @count buffer[buffer_size]
 * @objects attr_list SAI_OBJECT_TYPE_TAM_EVENT_ACTION
 * @objects tam_event_id SAI_OBJECT_TYPE_TAM_EVENT
 *
 * @param[in] tam_event_id Create Event Object ID
 * @param[in] buffer_size Actual buffer size in bytes
 * @param[in] buffer Data buffer
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 */
typedef void (*sai_tam_event_notification_fn)(
        _In_ sai_object_id_t tam_event_id,
        _In_ sai_size_t buffer_size,
        _In_ const void *buffer,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief TAM telemetry data get API
 *
 * @param[in] switch_id SAI Switch object id
 * @param[in] obj_list SAI Switch object list
 * @param[in] clear_on_read Flag to clear the read data
 * @param[inout] buffer_size Actual buffer size in bytes
 * @param[out] buffer Data buffer
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
sai_status_t sai_tam_telemetry_get_data(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_list_t obj_list,
        _In_ bool clear_on_read,
        _Inout_ sai_size_t *buffer_size,
        _Out_ void *buffer);

/**
 * @brief SAI TAM API set
 */
typedef struct _sai_tam_api_t
{
    /**
     * @brief SAI TAM v1 API set
     */
    sai_create_tam_fn                              create_tam;
    sai_remove_tam_fn                              remove_tam;
    sai_set_tam_attribute_fn                       set_tam_attribute;
    sai_get_tam_attribute_fn                       get_tam_attribute;

    sai_create_tam_math_func_fn                    create_tam_math_func;
    sai_remove_tam_math_func_fn                    remove_tam_math_func;
    sai_set_tam_math_func_attribute_fn             set_tam_math_func_attribute;
    sai_get_tam_math_func_attribute_fn             get_tam_math_func_attribute;

    sai_create_tam_report_fn                       create_tam_report;
    sai_remove_tam_report_fn                       remove_tam_report;
    sai_set_tam_report_attribute_fn                set_tam_report_attribute;
    sai_get_tam_report_attribute_fn                get_tam_report_attribute;

    sai_create_tam_event_threshold_fn              create_tam_event_threshold;
    sai_remove_tam_event_threshold_fn              remove_tam_event_threshold;
    sai_set_tam_event_threshold_attribute_fn       set_tam_event_threshold_attribute;
    sai_get_tam_event_threshold_attribute_fn       get_tam_event_threshold_attribute;

    sai_create_tam_int_fn                          create_tam_int;
    sai_remove_tam_int_fn                          remove_tam_int;
    sai_set_tam_int_attribute_fn                   set_tam_int_attribute;
    sai_get_tam_int_attribute_fn                   get_tam_int_attribute;

    sai_create_tam_tel_type_fn                     create_tam_tel_type;
    sai_remove_tam_tel_type_fn                     remove_tam_tel_type;
    sai_set_tam_tel_type_attribute_fn              set_tam_tel_type_attribute;
    sai_get_tam_tel_type_attribute_fn              get_tam_tel_type_attribute;

    sai_create_tam_transport_fn                    create_tam_transport;
    sai_remove_tam_transport_fn                    remove_tam_transport;
    sai_set_tam_transport_attribute_fn             set_tam_transport_attribute;
    sai_get_tam_transport_attribute_fn             get_tam_transport_attribute;

    sai_create_tam_telemetry_fn                    create_tam_telemetry;
    sai_remove_tam_telemetry_fn                    remove_tam_telemetry;
    sai_set_tam_telemetry_attribute_fn             set_tam_telemetry_attribute;
    sai_get_tam_telemetry_attribute_fn             get_tam_telemetry_attribute;

    sai_create_tam_collector_fn                    create_tam_collector;
    sai_remove_tam_collector_fn                    remove_tam_collector;
    sai_set_tam_collector_attribute_fn             set_tam_collector_attribute;
    sai_get_tam_collector_attribute_fn             get_tam_collector_attribute;

    sai_create_tam_event_action_fn                 create_tam_event_action;
    sai_remove_tam_event_action_fn                 remove_tam_event_action;
    sai_set_tam_event_action_attribute_fn          set_tam_event_action_attribute;
    sai_get_tam_event_action_attribute_fn          get_tam_event_action_attribute;

    sai_create_tam_event_fn                        create_tam_event;
    sai_remove_tam_event_fn                        remove_tam_event;
    sai_set_tam_event_attribute_fn                 set_tam_event_attribute;
    sai_get_tam_event_attribute_fn                 get_tam_event_attribute;

    sai_create_tam_counter_subscription_fn         create_tam_counter_subscription;
    sai_remove_tam_counter_subscription_fn         remove_tam_counter_subscription;
    sai_set_tam_counter_subscription_attribute_fn  set_tam_counter_subscription_attribute;
    sai_get_tam_counter_subscription_attribute_fn  get_tam_counter_subscription_attribute;
} sai_tam_api_t;

/**
 * @}
 */
#endif /** __SAITAM_H_ */
