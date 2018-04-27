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
 * @brief TAM Statistic ID
 *
 * Identifies a specific counter within the SAI object hierarchy
 */
typedef enum _sai_tam_stat_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_STAT_ATTR_START,

    /**
     * @brief Monitored object id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_BUFFER_POOL, SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP, SAI_OBJECT_TYPE_QUEUE
     */
    SAI_TAM_STAT_ATTR_PARENT_ID = SAI_TAM_STAT_ATTR_START,

    /**
     * @brief Counter
     *
     * Example: SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES.
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_TAM_STAT_ATTR_COUNTER_ID,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_STAT_ATTR_END,

    /** Custom range base value */
    SAI_TAM_STAT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_STAT_ATTR_CUSTOM_RANGE_END

} sai_tam_stat_attr_t;

/**
 * @brief Create and return a TAM stat id object
 *
 * @param[out] tam_stat_id TAM stat id object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_stat_fn)(
        _Out_ sai_object_id_t *tam_stat_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified tam stat id object.
 *
 * @param[in] tam_stat_id TAM object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_stat_fn)(
        _In_ sai_object_id_t tam_stat_id);

/**
 * @brief Set TAM stat id object attribute value(s).
 *
 * @param[in] tam_stat_id TAM stat id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_stat_attribute_fn)(
        _In_ sai_object_id_t tam_stat_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified TAM stat id attributes.
 *
 * @param[in] tam_stat_id TAM stat id object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_stat_attribute_fn)(
        _In_ sai_object_id_t tam_stat_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief TAM Statistic
 *
 * Identifies a specific counter within the SAI object hierarchy
 * and provides the current value of the counter.
 */
typedef struct _sai_tam_statistic_t
{
    /**
     * @brief Statistic ID
     *
     * @objects SAI_OBJECT_TYPE_TAM_STAT
     */
    sai_object_id_t statistic_id;

    /**
     * @brief Value
     */
    uint64_t value;

} sai_tam_statistic_t;

/**
 * @brief TAM Tracking Options
 */
typedef enum _sai_tam_tracking_mode_t
{
    /** Peak value tracking mode */
    SAI_TAM_TRACKING_MODE_PEAK,

    /** Current value tracking mode */
    SAI_TAM_TRACKING_MODE_CURRENT,

    /** Average value tracking mode */
    SAI_TAM_TRACKING_MODE_AVERAGE,

    /** Minimum value tracking mode */
    SAI_TAM_TRACKING_MODE_MINIMUM
} sai_tam_tracking_mode_t;

/**
 * @brief TAM Reporting Options
 */
typedef enum _sai_tam_reporting_mode_t
{
    /** Report tracking data in terms of bytes */
    SAI_TAM_REPORTING_MODE_BYTES,

    /** Report tracking data in percentages */
    SAI_TAM_REPORTING_MODE_PERCENTAGE,
} sai_tam_reporting_mode_t;

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
     * @brief Operational State for the Buffer Tracking
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_TAM_ATTR_BUFFER_TRACKING_ADMIN_STATE = SAI_TAM_ATTR_START,

    /**
     * @brief Statistics reporting mode.
     *
     * When not specified, reports in number of bytes (DEFAULT)
     *
     * @type sai_tam_reporting_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_TAM_REPORTING_MODE_BYTES
     */
    SAI_TAM_ATTR_BUFFER_REPORTING_MODE,

    /**
     * @brief Buffer Tracker Mode
     *
     * Specifies whether the Chip should track the peak values of the
     * buffers or current usage values (DEFAULT).
     *
     * @type sai_tam_tracking_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_TAM_TRACKING_MODE_CURRENT
     */
    SAI_TAM_ATTR_BUFFER_TRACKING_MODE,

    /**
     * @brief Buffers/Statistics for tracking using this object
     *
     * Specifies the Statistics/Types for tracking. If not specified, all
     * supported buffers are included for tracking (DEFAULT).
     *
     * A statistic can't be tracked by more than one TAM Objects.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_STAT
     * @default empty
     */
    SAI_TAM_ATTR_TRACKING_OPTIONS,

    /**
     * @brief Default Transporter
     *
     * Provides a default snapshot transporter object for the Tracker.
     * When a snapshot is made, this transporter will be used
     * to 'copy' the data to the 'transporter-desired' location.
     *
     * In the absence of a transporter, the tracker will copy the
     * data to the local CPU transporter.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_TRANSPORTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_ATTR_TRANSPORTER,

    /**
     * @brief Clear all Thresholds
     *
     * If this attribute is specified and set to true, then
     * the following actions take place for each of the created threshold objects
     * 1. The values specified via the SAI_TAM_THRESHOLD_ATTR_LEVEL attribute are
     *    removed
     * 2. Threshold monitoring is disabled
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_ATTR_CLEAR_ALL_THRESHOLDS,

    /**
     * @brief Total Number of counters supported
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_TAM_ATTR_TOTAL_NUM_STATISTICS,

    /**
     * @brief The latest Snapshot ID
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_SNAPSHOT
     */
    SAI_TAM_ATTR_LATEST_SNAPSHOT_ID,

    /**
     * @brief Maximum Number of snapshots that can be created.
     *
     * If the number of currently created snapshots already reach this limit, any
     * attempt to create more snapshots return error.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_TAM_ATTR_MAX_NUM_SNAPSHOTS,

    /**
     * @brief Tam thresholds associated with this tam.
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_THRESHOLD
     */
    SAI_TAM_ATTR_THRESHOLD_LIST,

    /**
     * @brief Tam IFA HDR objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_IFA_HDR
     */
    SAI_TAM_ATTR_IFA_HDR_LIST,

    /**
     * @brief Tam IFA probe objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_IFA_PROBE
     */
    SAI_TAM_ATTR_IFA_PROBE_LIST,

    /**
     * @brief Tam Probes associated with this tam
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_PROBE
     */
    SAI_TAM_ATTR_PROBE_LIST,

    /**
     * @brief Tam Math functions associated with this tam
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_MATH_FUNC
     */
    SAI_TAM_ATTR_MATH_FUNC_LIST,

    /**
     * @brief Tam report objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_REPORT
     */
    SAI_TAM_ATTR_MATH_REPORT_LIST,

    /**
     * @brief Tam Telemetry type objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_TEL_TYPE
     */
    SAI_TAM_ATTR_TELEMETRY_TYPE_LIST,

    /**
     * @brief Tam telemetry objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_TELEMETRY
     */
    SAI_TAM_ATTR_TELEMETRY_LIST,

    /**
     * @brief Tam transport objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_TRANSPORT
     */
    SAI_TAM_ATTR_TRANSPORT_LIST,

    /**
     * @brief Tam event threshold objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_EVENT_THRESHOLD
     */
    SAI_TAM_ATTR_EVENT_THRESHOLD_LIST,

    /**
     * @brief Tam collector objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_COLLECTOR
     */
    SAI_TAM_ATTR_COLLECTOR_LIST,

    /**
     * @brief Tam event action objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_EVENT_ACTION
     */
    SAI_TAM_ATTR_EVENT_ACTION_LIST,

    /**
     * @brief Tam event objects associated with this tam
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_EVENT
     */
    SAI_TAM_ATTR_EVENT_LIST,

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
 * Deleting a TAM object also deletes all associated snapshot and threshold objects.
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
 * @brief TAM Threshold Breach Event notification
 */
typedef struct _sai_tam_threshold_breach_event_t
{
    /**
     * @brief Threshold ID
     *
     * @objects SAI_OBJECT_TYPE_TAM_THRESHOLD
     */
    sai_object_id_t threshold_id;

    /**
     * @brief Snapshot Valid
     *
     * Indicates whether the tam_snapshot_id field points to a valid object.
     * is_snapshot_valid is set to false when the attribute
     * SAI_TAM_THRESHOLD_ATTR_SNAPSHOT_ON_BREACH is either unspecified or set
     * to false.
     */
    bool is_snapshot_valid;

    /**
     * @brief Snapshot Id
     *
     * This field is valid only when is_snapshot_valid is set to true.
     *
     * @objects SAI_OBJECT_TYPE_TAM_SNAPSHOT
     * @validonly is_snapshot_valid == true
     */
    sai_object_id_t tam_snapshot_id;

    /**
     * @brief Threshold / Statistic value for the breach event
     */
    uint64_t value;

} sai_tam_threshold_breach_event_t;

/**
 * @brief TAM event notification function
 *
 * Provides the callback function to be invoked upon a threshold breach.
 * In the absence of a callback function, the event will be ignored (DEFAULT)
 * If neither of callback nor transporter is provided, no snapshot is made.
 * If callback is required but SAI_TAM_THRESHOLD_ATTR_SNAPSHOT_ON_BREACH is
 * set to false, then the event data passed to the callback function will
 * have the field is_snapshot_valid set to false.
 *
 * @count data[count]
 *
 * @param[in] count Number of events
 * @param[in] data Pointer to TAM events data array
 */
typedef void (*sai_tam_event_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_tam_threshold_breach_event_t *data);

/**
 * @brief TAM Threshold Attributes.
 */
typedef enum _sai_tam_threshold_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_THRESHOLD_ATTR_START,

    /**
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_THRESHOLD_ATTR_TAM_ID = SAI_TAM_THRESHOLD_ATTR_START,

    /**
     * @brief Statistic for this threshold
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_STAT
     */
    SAI_TAM_THRESHOLD_ATTR_STATISTIC,

    /**
     * @brief Threshold Level
     *
     * Breach level for this threshold in number of bytes If specified, a
     * threshold breach event will be recorded when the buffer usage goes
     * beyond the level.
     *
     * If not specified, then by default the threshold is created without any
     * level, which is effectively disabling the threshold monitoring for the
     * statistic.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_THRESHOLD_ATTR_LEVEL,

    /**
     * @brief Transporter Object
     *
     * Provides the snapshot transporter object for this threshold. When the
     * threshold is breached, this transporter will be used to 'copy' the data
     * to the 'transporter-desired' location.
     *
     * In the absence of a transporter, the tracker's default transporter will
     * be used.
     *
     * It may be noted that, Upon a breach, the 'snapshot' object is
     * automatically created (see below attribute), and it will not have a
     * separate transporter object. Instead, this transporter (or the tracker's
     * default transporter) is used.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_TRANSPORTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_THRESHOLD_ATTR_TRANSPORTER,

    /**
     * @brief Snapshot on breach
     *
     * Take a snapshot upon a threshold breach.
     * When this attribute is specified and set to true, Snapshots are
     * automatically created upon the threshold breach.
     * Otherwise, a snapshot is not created.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_THRESHOLD_ATTR_SNAPSHOT_ON_BREACH,

    /**
     * @brief Buffers/Statistics for inclusion in the snapshot
     *
     * Specifies the Statistics/Types for the snapshot.
     * If not specified, all buffers tracked by
     * the associated TAM object are included in the snapshot.
     * When specified, the buffers requested for snapshot must be within the set
     * tracked by the associated TAM object.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_STAT
     * @default empty
     */
    SAI_TAM_THRESHOLD_ATTR_SNAPSHOT_STATS,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_THRESHOLD_ATTR_END,

    /** Custom range base value */
    SAI_TAM_THRESHOLD_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_THRESHOLD_ATTR_CUSTOM_RANGE_END

} sai_tam_threshold_attr_t;

/**
 * @brief Create and return a threshold object
 *
 * This creates a threshold in the hardware with the associated statistic
 * passed via the attributes.
 *
 * @param[out] tam_threshold_id Threshold object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Preferences for creating a threshold
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_threshold_fn)(
        _Out_ sai_object_id_t *tam_threshold_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified threshold object.
 *
 * @param[in] tam_threshold_id Threshold object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_threshold_fn)(
        _In_ sai_object_id_t tam_threshold_id);

/**
 * @brief Set threshold attribute value(s).
 *
 * @param[in] tam_threshold_id Threshold object id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_threshold_attribute_fn)(
        _In_ sai_object_id_t tam_threshold_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified threshold attributes.
 *
 * @param[in] tam_threshold_id Threshold object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_threshold_attribute_fn)(
        _In_ sai_object_id_t tam_threshold_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief TAM Snapshot Attributes.
 */
typedef enum _sai_tam_snapshot_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_SNAPSHOT_ATTR_START,

    /**
     * @brief TAM Object for this snapshot
     *
     * Specifies the TAM object for this snapshot.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_SNAPSHOT_ATTR_TAM_ID = SAI_TAM_SNAPSHOT_ATTR_START,

    /**
     * @brief Buffers/Statistics for inclusion in snapshot
     *
     * Specifies the Statistics/Types for a snapshot.
     * If not specified, all buffers tracked by
     * the associated TAM object are included in the snapshot. (DEFAULT)
     * When specified, the buffers requested for snapshot must be within the set
     * tracked by the associated TAM object.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_STAT
     * @default empty
     */
    SAI_TAM_SNAPSHOT_ATTR_STAT_TYPE,

    /**
     * @brief Transporter Object
     *
     * Provides the snapshot transporter object for this snapshot.
     * When the snapshot is made, this transporter will be used
     * to 'copy' the data to the 'transporter-desired' location.
     * In the absence of a transporter, the tracker's default transporter
     * will be used (DEFAULT).
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_TRANSPORTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_SNAPSHOT_ATTR_TRANSPORTER,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_SNAPSHOT_ATTR_END,

    /** Custom range base value */
    SAI_TAM_SNAPSHOT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_SNAPSHOT_ATTR_CUSTOM_RANGE_END

} sai_tam_snapshot_attr_t;

/**
 * @brief Create and return a snapshot object
 *
 * This creates a snapshot in the hardware and copies the snapshot data
 * into the driver. Via the attributes, caller may indicate a preference
 * for snapshot of a specific set of statistics/groups.
 *
 * @param[out] tam_snapshot_id Snapshot object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_snapshot_fn)(
        _Out_ sai_object_id_t *tam_snapshot_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified snapshot object and free driver memory.
 *
 * @param[in] tam_snapshot_id Snapshot object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_snapshot_fn)(
        _In_ sai_object_id_t tam_snapshot_id);

/**
 * @brief Set Snapshot attribute value(s).
 *
 * @param[in] tam_snapshot_id Snapshot object id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_snapshot_attribute_fn)(
        _In_ sai_object_id_t tam_snapshot_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified Snapshot attributes.
 *
 * @param[in] tam_snapshot_id Snapshot object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_snapshot_attribute_fn)(
        _In_ sai_object_id_t tam_snapshot_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Obtain the values for all statistics from a snapshot.
 *
 * Attribute list must supply sufficient memory for statistics
 * as specified for the snapshot object, which may be all statistics
 * supported by the associated tam object.
 *
 * @param[in] tam_snapshot_id Snapshot object id
 * @param[inout] number_of_counters Number of statistics (required/provided)
 * @param[inout] statistics Statistics (allocated/provided)
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_snapshot_stats_fn)(
        _In_ sai_object_id_t tam_snapshot_id,
        _Inout_ uint32_t *number_of_counters,
        _Inout_ sai_tam_statistic_t *statistics);

/**
 * @brief TAM type of snapshot transport
 */
typedef enum _sai_tam_transporter_type_t
{
    /** Transport local, to the CPU */
    SAI_TAM_TRANSPORTER_TYPE_LOCAL,

    /** Transport remote, to a remote monitoring client */
    SAI_TAM_TRANSPORTER_TYPE_REMOTE,

} sai_tam_transporter_type_t;

/**
 * @brief TAM Snapshot Transporter Attributes
 */
typedef enum _sai_tam_transporter_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_TRANSPORTER_ATTR_START,

    /**
     * @brief Transporter Type
     *
     * If this attribute value is unspecified the local transporter is used.
     *
     * @type sai_tam_transporter_type_t
     * @flags CREATE_AND_SET
     * @default SAI_TAM_TRANSPORTER_TYPE_LOCAL
     */
    SAI_TAM_TRANSPORTER_ATTR_TYPE = SAI_TAM_TRANSPORTER_ATTR_START,

    /**
     * @brief Maximum size beyond which it will be truncated.
     *
     * If this attribute value is zero or unspecified, snapshots are not
     * truncated while transporting. (DEFAULT)
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_TRANSPORTER_ATTR_MAX_SNAPSHOT_SIZE,

    /**
     * @brief Mirroring session object defining the remote transport capabilities.
     *
     * If this attribute is unspecified, Local CPU Transport is used (DEFAULT).
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_MIRROR_SESSION
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_TRANSPORTER_ATTR_MONITOR_ID,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_TRANSPORTER_ATTR_END,

    /** Custom range base value */
    SAI_TAM_TRANSPORTER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_TRANSPORTER_ATTR_CUSTOM_RANGE_END

} sai_tam_transporter_attr_t;

/**
 * @brief Create and return a Transporter object
 *
 * This creates a transport object for copying the snapshot data
 * to the desired location.
 *
 * @param[out] tam_transporter_id Transporter object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_transporter_fn)(
        _Out_ sai_object_id_t *tam_transporter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified Transporter object.
 *
 * @param[in] tam_transporter_id Transporter object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_transporter_fn)(
        _In_ sai_object_id_t tam_transporter_id);

/**
 * @brief Set TAM Transporter attribute value(s).
 *
 * @param[in] tam_transporter_id Transporter object id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_transporter_attribute_fn)(
        _In_ sai_object_id_t tam_transporter_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified Transporter attributes.
 *
 * @param[in] tam_transporter_id Transporter object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_transporter_attribute_fn)(
        _In_ sai_object_id_t tam_transporter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief IFA(Inband Flow Analyzer) header Object Attributes
 */
typedef enum _sai_tam_ifa_hdr_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_IFA_HDR_ATTR_START,

    /**
     * @brief IFA Header Marker 1
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_TAM_IFA_HDR_ATTR_MARKER_1 = SAI_TAM_IFA_HDR_ATTR_START,

    /**
     * @brief IFA Header Marker 2
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_TAM_IFA_HDR_ATTR_MARKER_2,

    /**
     * @brief IFA Header Version Bits
     * Bit field: 4bits
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_TAM_IFA_HDR_ATTR_VER,

    /**
     * @brief IFA Header Replication Bits
     * Bit field: 2bits
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_IFA_HDR_ATTR_REP_BITS,

    /**
     * @brief IFA Header Copy Bit
     * Bit field: 1bit
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_IFA_HDR_ATTR_C_BIT,

    /**
     * @brief IFA Header Max Hop Count Exceed Bit
     * Bit field: 1bit
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_IFA_HDR_ATTR_E_BIT,

    /**
     * @brief IFA Header Control Bits
     * Bit field: 5bits
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_IFA_HDR_ATTR_CNTRL_BITS,

    /**
     * @brief IFA Header Message Type Bits
     * Bit field: 3bits
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_IFA_HDR_ATTR_MTYPE_BITS,

    /**
     * @brief IFA Header Flag
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_IFA_HDR_ATTR_FLAG,

    /**
     * @brief IFA Header Request Vector
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_IFA_HDR_ATTR_REQUEST_VECTOR,

    /**
     * @brief IFA Header Action Vector
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_IFA_HDR_ATTR_ACTION_VECTOR,

    /**
     * @brief IFA Header Hop Limit and Count
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_IFA_HDR_ATTR_HOP_LIMIT_AND_COUNT,

    /**
     * @brief IFA Header Reserved
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_IFA_HDR_ATTR_RSVD,

    /**
     * @brief IFA Header Max Length
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_IFA_HDR_ATTR_MAX_LEN,

    /**
     * @brief IFA Header Current Length
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_TAM_IFA_HDR_ATTR_CUR_LENGTH,

    /**
     * @brief IFA Header Senders Handle
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_IFA_HDR_ATTR_SENDERS_HANDLE,

    /**
     * @brief IFA Header Sequence Number
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_TAM_IFA_HDR_ATTR_SEQ_NUMBER,

    /**
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_IFA_HDR_ATTR_TAM_ID,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_IFA_HDR_ATTR_END,

    /** Custom range base value */
    SAI_TAM_IFA_HDR_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_IFA_HDR_ATTR_CUSTOM_RANGE_END

} sai_tam_ifa_hdr_attr_t;

/**
 * @brief Create and return a IFA header object
 *
 * @param[out] tam_ifa_hdr_id IFA header object id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_ifa_hdr_fn)(
        _Out_ sai_object_id_t *tam_ifa_hdr_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified IFA header object
 *
 * @param[in] tam_ifa_hdr_id IFA header object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_ifa_hdr_fn)(
        _In_ sai_object_id_t tam_ifa_hdr_id);

/**
 * @brief Get values for specified IFA Hdr attributes
 *
 * @param[in] tam_ifa_hdr_id IFA header object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_ifa_hdr_attribute_fn)(
        _In_ sai_object_id_t tam_ifa_hdr_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for a specified IFA Hdr attribute
 *
 * @param[in] tam_ifa_hdr_id IFA header object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_ifa_hdr_attribute_fn)(
        _In_ sai_object_id_t tam_ifa_hdr_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief IFA Probe Object
 */
typedef enum _sai_tam_ifa_probe_attr_t
{

    /**
     * @brief Start of Attributes
     */
    SAI_TAM_IFA_PROBE_ATTR_START,

    /**
     * @brief Probe IP header version
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_IFA_PROBE_ATTR_HDR_IPHDR_VERSION = SAI_TAM_IFA_PROBE_ATTR_START,

    /**
     * @brief Probe header TOS
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_IFA_PROBE_ATTR_HDR_TOS,

    /**
     * @brief Probe header TTL
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 255
     */
    SAI_TAM_IFA_PROBE_ATTR_HDR_TTL,

    /**
     * @brief Probe source IP
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_IFA_PROBE_ATTR_SRC_IP_ADDRESS,

    /**
     * @brief Probe destination IP
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_TAM_IFA_PROBE_ATTR_DST_IP_ADDRESS,

    /**
     * @brief Probe protocol id
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan false
     */
    SAI_TAM_IFA_PROBE_ATTR_PROTOCOL_TYPE,

    /**
     * @brief Probe UDP header source port
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @isvlan false
     */
    SAI_TAM_IFA_PROBE_ATTR_PROTO_SPORT,

    /**
     * @brief Probe UDP header destination port
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @isvlan false
     */
    SAI_TAM_IFA_PROBE_ATTR_PROTO_DPORT,

    /**
     * @brief Probe UDP header length
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @isvlan false
     */
    SAI_TAM_IFA_PROBE_ATTR_LEN,

    /**
     * @brief IFA HDR Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_IFA_HDR
     */
    SAI_TAM_IFA_PROBE_ATTR_IFA_HDR,

    /**
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_IFA_PROBE_ATTR_TAM_ID,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_IFA_PROBE_ATTR_END,

    /** Custom range base value */
    SAI_TAM_IFA_PROBE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_IFA_PROBE_ATTR_CUSTOM_RANGE_END

} sai_tam_ifa_probe_attr_t;

/**
 * @brief Create and return a IFA probe object
 *
 * @param[out] tam_ifa_probe_id IFA probe object id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_ifa_probe_fn)(
        _Out_ sai_object_id_t *tam_ifa_probe_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified IFA probe object
 *
 * @param[in] tam_ifa_probe_id IFA probe object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_ifa_probe_fn)(
        _In_ sai_object_id_t tam_ifa_probe_id);

/**
 * @brief Get values for specified IFA probe attributes
 *
 * @param[in] tam_ifa_probe_id IFA probe object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_ifa_probe_attribute_fn)(
        _In_ sai_object_id_t tam_ifa_probe_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for a specified IFA probe attribute
 *
 * @param[in] tam_ifa_probe_id IFA probe object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_ifa_probe_attribute_fn)(
        _In_ sai_object_id_t tam_ifa_probe_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief TAM probe types
 */
typedef enum _sai_tam_probe_type_t
{
    /**
     * @brief SYNTHETIC PROBE
     */
    SAI_TAM_PROBE_TYPE_SYNTHETIC,

    /**
     * @brief SAMPLED PROBE
     */
    SAI_TAM_PROBE_TYPE_SAMPLED,

} sai_tam_probe_type_t;

/**
 * @brief TAM probe Attributes
 */
typedef enum _sai_tam_probe_attr_t
{

    /**
     * @brief Start of Attributes
     */
    SAI_TAM_PROBE_ATTR_START,

    /**
     * @brief Probe Type
     *
     * @type sai_tam_probe_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_PROBE_ATTR_TYPE = SAI_TAM_PROBE_ATTR_START,

    /**
     * @brief Probe header object ID
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_IFA_HDR, SAI_OBJECT_TYPE_TAM_IFA_PROBE
     */
    SAI_TAM_PROBE_ATTR_HDR,

    /**
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_PROBE_ATTR_HDR_TAM_ID,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_PROBE_ATTR_END,

    /** Custom range base value */
    SAI_TAM_PROBE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_PROBE_ATTR_CUSTOM_RANGE_END

} sai_tam_probe_attr_t;

/**
 * @brief Create and return a TAM probe object
 *
 * @param[out] tam_probe_id TAM probe object id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_probe_fn)(
        _Out_ sai_object_id_t *tam_probe_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified TAM probe object
 *
 * @param[in] tam_probe_id TAM probe object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_probe_fn)(
        _In_ sai_object_id_t tam_probe_id);

/**
 * @brief Get values for specified TAM probe attributes
 *
 * @param[in] tam_probe_id TAM probe object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_probe_attribute_fn)(
        _In_ sai_object_id_t tam_probe_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set value for a specified TAM probe attribute
 *
 * @param[in] tam_probe_id TAM probe object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_probe_attribute_fn)(
        _In_ sai_object_id_t tam_probe_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief TAM Telemetry Math Func types
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
     * @type sai_tam_tel_math_func_type_t
     * @flags CREATE_AND_SET
     * @default SAI_TAM_TEL_MATH_FUNC_TYPE_NONE
     */
    SAI_TAM_MATH_FUNC_ATTR_TAM_TEL_MATH_FUNC_TYPE = SAI_TAM_MATH_FUNC_ATTR_START,

    /**
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_MATH_FUNC_ATTR_TAM_ID,

    /**
     * @brief Add Math func attributes below
     */

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
 * @brief Event Threshold Attributes
 */
typedef enum _sai_tam_event_threshold_attr_t
{

    /**
     * @brief Start of Attributes
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_START,

    /**
     * @brief High water mark %
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 90
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_HIGH_WATERMARK = SAI_TAM_EVENT_THRESHOLD_ATTR_START,

    /**
     * @brief Low Water Mark %
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 10
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_LOW_WATERMARK,

    /**
     * @brief Latency in Nano second
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
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_EVENT_THRESHOLD_ATTR_TAM_ID,

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
 * @brief TAM telemetry types supported
 */
typedef enum _sai_tam_telemetry_type_t
{
    /**
     * @brief Networking element TAM
     */
    SAI_TAM_TELEMETRY_TYPE_NE,

    /**
     * @brief Switch silicon TAM
     */
    SAI_TAM_TELEMETRY_TYPE_SWITCH,

    /**
     * @brief Fabric TAM
     */
    SAI_TAM_TELEMETRY_TYPE_FABRIC,

    /**
     * @brief Flow TAM
     */
    SAI_TAM_TELEMETRY_TYPE_FLOW,

    /**
     * @brief Networking element TAM
     */
    SAI_TAM_TELEMETRY_TYPE_INT

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
     * @brief INT - Template Id
     *
     * Template id indicates the supported metadata header.
     * Device can support one or more template IDs
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_TEL_TYPE_ATTR_INT_TEMPLATE_ID,

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
     * @brief Flow - Flow ID
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_TAM_TEL_TYPE_ATTR_TAM_TELEMETRY_TYPE == SAI_TAM_TELEMETRY_TYPE_FLOW
     */
    SAI_TAM_TEL_TYPE_ATTR_FLOW_ID,

    /**
     * @brief Flow - Elephant flows: Elephant/Mice classification
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_FLOW_CLASSIFICATION_ENABLE,

    /**
     * @brief Flow - Duration in Milliseconds
     *
     * @type sai_int32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_TEL_TYPE_ATTR_FLOW_DURATION,

    /**
     * @brief Flow - Size in Kilo Bytes
     *
     * @type sai_int32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_TEL_TYPE_ATTR_FLOW_SIZE,

    /**
     * @brief Flow - Action (report object)
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_REPORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_TEL_TYPE_ATTR_FLOW_ACTION,

    /**
     * @brief Switch - Collect Port stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_SWITCH_ENABLE_PORT_STATS,

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
     * @brief NE - Collect information
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_TEL_TYPE_ATTR_NE_ENABLE,

    /**
     * @brief Math func attached
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_MATH_FUNC
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_TEL_TYPE_ATTR_MATH_FUNC,

    /**
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_TEL_TYPE_ATTR_TAM_ID,

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
     * @brief Report using PROTO
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
     * @brief Report using INT
     */
    SAI_TAM_REPORT_TYPE_INT,

    /**
     * @brief Report using vendor extensions
     */
    SAI_TAM_REPORT_TYPE_VENDOR_EXTN,
} sai_tam_report_type_t;

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
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_REPORT_ATTR_TAM_ID,

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
    SAI_TAM_REPORTING_UNIT_SEC = 0,

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
     * @type sai_object_id_t
     *
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_TEL_TYPE
     */
    SAI_TAM_TELEMETRY_ATTR_TAM_TYPE_LIST = SAI_TAM_TELEMETRY_ATTR_START,

    /**
     * @brief Collector object list
     * @type sai_object_list_t
     *
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_COLLECTOR
     */
    SAI_TAM_TELEMETRY_ATTR_COLLECTOR_LIST,

    /**
     * @brief Tam report type
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_REPORT
     */
    SAI_TAM_TELEMETRY_ATTR_REPORT_ID,

    /**
     * @brief Tam event reporting unit
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
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_TELEMETRY_ATTR_TAM_ID,

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
     * @brief Transport TCP
     */
    SAI_TAM_TRANSPORT_TYPE_TCP,

    /**
     * @brief Transport UDP
     */
    SAI_TAM_TRANSPORT_TYPE_UDP,

    /**
     * @brief Transport INT
     */
    SAI_TAM_TRANSPORT_TYPE_INT,

    /**
     * @brief Transport GRPC
     */
    SAI_TAM_TRANSPORT_TYPE_GRPC,

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
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_TRANSPORT_ATTR_TAM_ID,

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
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_TAM_COLLECTOR_ATTR_SRC_IP = SAI_TAM_COLLECTOR_ATTR_START,

    /**
     * @brief Destination IP addresses
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_TAM_COLLECTOR_ATTR_DST_IP,

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
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_COLLECTOR_ATTR_TAM_ID,

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
 * @brief Enum defining event types.
 */
typedef enum _sai_tam_event_type_t
{

    /**
     * @brief New flow or flow state change event
     */
    SAI_TAM_EVENT_TYPE_FLOW_STATE,

    /**
     * @brief Watchlist event
     */
    SAI_TAM_EVENT_TYPE_FLOW_WATCHLIST,

    /**
     * @brief Flow TCP FLAGS event
     */
    SAI_TAM_EVENT_TYPE_FLOW_TCPFLAG,

    /**
     * @brief Queue depth or latency threshold event
     */
    SAI_TAM_EVENT_TYPE_QUEUE_THRESHOLD,

    /**
     * @brief Queue tail drop event
     */
    SAI_TAM_EVENT_TYPE_QUEUE_TAIL_DROP,

    /**
     * @brief Packet drop event
     */
    SAI_TAM_EVENT_TYPE_PACKET_DROP,

    /**
     * @brief Switch resource utilization threshold event
     */
    SAI_TAM_EVENT_TYPE_RESOURCE_UTILIZATION,
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
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_REPORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_EVENT_ACTION_ATTR_REPORT_TYPE = SAI_TAM_EVENT_ACTION_ATTR_START,

    /**
     * @brief QOS action Type Object
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_EVENT_ACTION_ATTR_QOS_ACTION_TYPE,

    /**
     * @brief Action type Next Hop
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NEXT_HOP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_EVENT_ACTION_ATTR_NEXTHOP_TYPE,

    /**
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_EVENT_ACTION_ATTR_TAM_ID,

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
     * @brief Event action
     * @type sai_object_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_EVENT_ACTION
     */
    SAI_TAM_EVENT_ATTR_ACTION_LIST,

    /**
     * @brief Collector object list
     * @type sai_object_list_t
     *
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_COLLECTOR
     */
    SAI_TAM_EVENT_ATTR_COLLECTOR_LIST,

    /**
     * @brief Tam event threshold attr Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TAM_EVENT_THRESHOLD
     */
    SAI_TAM_EVENT_ATTR_THRESHOLD,

    /**
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_EVENT_ATTR_TAM_ID,

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
 * @brief SAI TAM API set
 */
typedef struct _sai_tam_api_t
{

    /**
     * @brief SAI TAM v1 API set
     */
    sai_create_tam_fn                         create_tam;
    sai_remove_tam_fn                         remove_tam;
    sai_set_tam_attribute_fn                  set_tam_attribute;
    sai_get_tam_attribute_fn                  get_tam_attribute;
    sai_create_tam_stat_fn                    create_tam_stat;
    sai_remove_tam_stat_fn                    remove_tam_stat;
    sai_set_tam_stat_attribute_fn             set_tam_stat_attribute;
    sai_get_tam_stat_attribute_fn             get_tam_stat_attribute;
    sai_create_tam_threshold_fn               create_tam_threshold;
    sai_remove_tam_threshold_fn               remove_tam_threshold;
    sai_set_tam_threshold_attribute_fn        set_tam_threshold_attribute;
    sai_get_tam_threshold_attribute_fn        get_tam_threshold_attribute;
    sai_create_tam_snapshot_fn                create_tam_snapshot;
    sai_remove_tam_snapshot_fn                remove_tam_snapshot;
    sai_set_tam_snapshot_attribute_fn         set_tam_snapshot_attribute;
    sai_get_tam_snapshot_attribute_fn         get_tam_snapshot_attribute;
    sai_get_tam_snapshot_stats_fn             get_tam_snapshot_stats;
    sai_create_tam_transporter_fn             create_tam_transporter;
    sai_remove_tam_transporter_fn             remove_tam_transporter;
    sai_set_tam_transporter_attribute_fn      set_tam_transporter_attribute;
    sai_get_tam_transporter_attribute_fn      get_tam_transporter_attribute;

    sai_create_tam_math_func_fn               create_tam_math_func;
    sai_remove_tam_math_func_fn               remove_tam_math_func;
    sai_set_tam_math_func_attribute_fn        set_tam_math_func_attribute;
    sai_get_tam_math_func_attribute_fn        get_tam_math_func_attribute;

    sai_create_tam_report_fn                  create_tam_report;
    sai_remove_tam_report_fn                  remove_tam_report;
    sai_set_tam_report_attribute_fn           set_tam_report_attribute;
    sai_get_tam_report_attribute_fn           get_tam_report_attribute;

    sai_create_tam_event_threshold_fn         create_tam_event_threshold;
    sai_remove_tam_event_threshold_fn         remove_tam_event_threshold;
    sai_set_tam_event_threshold_attribute_fn  set_tam_event_threshold_attribute;
    sai_get_tam_event_threshold_attribute_fn  get_tam_event_threshold_attribute;

    sai_create_tam_tel_type_fn                create_tam_tel_type;
    sai_remove_tam_tel_type_fn                remove_tam_tel_type;
    sai_set_tam_tel_type_attribute_fn         set_tam_tel_type_attribute;
    sai_get_tam_tel_type_attribute_fn         get_tam_tel_type_attribute;

    sai_create_tam_transport_fn               create_tam_transport;
    sai_remove_tam_transport_fn               remove_tam_transport;
    sai_set_tam_transport_attribute_fn        set_tam_transport_attribute;
    sai_get_tam_transport_attribute_fn        get_tam_transport_attribute;

    sai_create_tam_ifa_hdr_fn                 create_tam_ifa_hdr;
    sai_remove_tam_ifa_hdr_fn                 remove_tam_ifa_hdr;
    sai_set_tam_ifa_hdr_attribute_fn          set_tam_ifa_hdr_attribute;
    sai_get_tam_ifa_hdr_attribute_fn          get_tam_ifa_hdr_attribute;

    sai_create_tam_ifa_probe_fn               create_tam_ifa_probe;
    sai_remove_tam_ifa_probe_fn               remove_tam_ifa_probe;
    sai_set_tam_ifa_probe_attribute_fn        set_tam_ifa_probe_attribute;
    sai_get_tam_ifa_probe_attribute_fn        get_tam_ifa_probe_attribute;

    sai_create_tam_probe_fn                   create_tam_probe;
    sai_remove_tam_probe_fn                   remove_tam_probe;
    sai_set_tam_probe_attribute_fn            set_tam_probe_attribute;
    sai_get_tam_probe_attribute_fn            get_tam_probe_attribute;

    sai_create_tam_telemetry_fn               create_tam_telemetry;
    sai_remove_tam_telemetry_fn               remove_tam_telemetry;
    sai_set_tam_telemetry_attribute_fn        set_tam_telemetry_attribute;
    sai_get_tam_telemetry_attribute_fn        get_tam_telemetry_attribute;

    sai_create_tam_collector_fn               create_tam_collector;
    sai_remove_tam_collector_fn               remove_tam_collector;
    sai_set_tam_collector_attribute_fn        set_tam_collector_attribute;
    sai_get_tam_collector_attribute_fn        get_tam_collector_attribute;

    sai_create_tam_event_action_fn            create_tam_event_action;
    sai_remove_tam_event_action_fn            remove_tam_event_action;
    sai_set_tam_event_action_attribute_fn     set_tam_event_action_attribute;
    sai_get_tam_event_action_attribute_fn     get_tam_event_action_attribute;

    sai_create_tam_event_fn                   create_tam_event;
    sai_remove_tam_event_fn                   remove_tam_event;
    sai_set_tam_event_attribute_fn            set_tam_event_attribute;
    sai_get_tam_event_attribute_fn            get_tam_event_attribute;
} sai_tam_api_t;

/**
 * @}
 */
#endif /** __SAITAM_H_ */
