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
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc
 *
 * @file    saitam.h
 *
 * @brief   This module defines SAI Bridge
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
     */
	SAI_TAM_STAT_ATTR_PARENT_ID,

    /**
     * @brief Monitored object type
     * Eg.: SAI_OBJECT_TYPE_BUFFER_POOL, SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP
     * Not used for input. Optional for output.
     * 
     * @type sai_object_type_t
     * @flags READ_ONLY
     */
	SAI_TAM_STAT_ATTR_PARENT_TYPE,

    /**
     * @brief Counter
     * Eg.: SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES
     *
     * @type uint32_t
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
 * This creates a TAM stat id object.
 *
 * @param[out] tam_id TAM object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_stat_create_fn)(
	    _Out_ sai_object_id_t *stat_id,
	    _In_ sai_object_id_t switch_id,
	    _In_ uint32_t attr_count,
	    _In_ const sai_attribute_t *attr_list);

/**
 *
 * @brief Deletes a specified tam stat id object.
 *
 * @param[in] stat_id TAM object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_stat_remove_fn)(
        _In_ sai_object_id_t *stat_id);

/**
 * @brief Set TAM stat id object attribute value(s).
 *
 * @param[in] stat_id TAM stat id
 * @param[in] attr attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_stat_attribute_set_fn)(
        _In_ sai_object_id_t stat_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified TAM stat id attributes.
 *
 * @param[in] stat_id - tam stat id object id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_stat_attribute_get_fn)(
        _In_ sai_object_id_t stat_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief TAM Statistic
 *
 * Identifies a specific counter within the SAI object hierarchy
 * and provides the current value of the counter
 */
typedef struct _sai_tam_statistic_t 
{
    /**
     * @brief Statistic ID
     * @objects SAI_OBJECT_TYPE_TAM_STAT
     */
    sai_object_id_t statistic_id;

    /**
     * @brief Value
     */
    uint64_t value;

} sai_tam_statistic_t;

/**
 *  @brief TAM Tracking Options
 */
typedef enum _sai_tam_tracking_options_t 
{
    /** Peak value tracking mode */
    SAI_TAM_TRACKING_MODE_PEAK,
    /** Current value tracking mode */ 
    SAI_TAM_TRACKING_MODE_CURRENT,
    /** Average value tracking mode */
    SAI_TAM_TRACKING_MODE_AVERAGE,
    /** Minimum value tracking mode */
    SAI_TAM_TRACKING_MODE_MINIMUM
} sai_tam_tracking_options_t;

/**
 *  @brief TAM Reporting Options
 */
typedef enum _sai_tam_reporting_options_t 
{
    /** Report tracking data in terms of bytes */
    SAI_TAM_REPORTING_MODE_BYTES,
    /** Report tracking data in percentages */
    SAI_TAM_REPORTING_MODE_PERCENTAGE,
} sai_tam_reporting_options_t;

/**
 * @brief TAM Attributes.
 */
typedef enum _sai_tam_attr_t 
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_ATTR_START,

    /*
     * @brief Operational State for the Buffer Tracking
     *
     * @type boolean
     * @default true
     * @allownull true
     */
    SAI_TAM_ATTR_BUFFER_TRACKING_ADMIN_STATE,

    /*
     * @brief Statistics reporting mode.
     * When not specified, reports in number of bytes (DEFAULT)
     *
     * @type sai_tam_reporting_options_t
     * @default SAI_TAM_REPORTING_MODE_BYTES
     * @allownull true
     */
    SAI_TAM_ATTR_BUFFER_REPORTING_MODE,

    /**
     * @brief Buffer Tracker Mode
     *
     * Specifies whether the Chip should track the peak values of the
     * buffers or current usage values (DEFAULT)
     *
     * @type sai_tam_tracking_options_t
     * @default SAI_TAM_TRACKING_MODE_CURRENT
     * @allownull true
     */
    SAI_TAM_ATTR_BUFFER_TRACKING_MODE,

    /**
     * @brief Buffers/Statistics for tracking using this object
     *
     * Specifies the Statistics/Types for tracking. If not specified, all
     * supported buffers are included for tracking. (DEFAULT)
     *
     * A statistic can't be tracked by more then one TAM Objects
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_STAT
     * @allownull true
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
     * @allownull true
     */
    SAI_TAM_ATTR_TRANSPORTER,

    /*
     * @brief Clear all Thresholds
     * If this attribute is specified and set to true, then
     * the following actions take place for each of the created threshold objects
     * 1. The values specified via the SAI_TAM_THRESHOLD_ATTR_LEVEL attribute are
     *    removed
     * 2. Threshold monitoring is disabled
     *
     * @type Boolean
     * @flags WRITE_ONLY | CREATE_AND_SET
     */
    SAI_TAM_ATTR_CLEAR_ALL_THRESHOLDS,

    /*
     * @brief Total Number of counters supported
     *
     * @type uint32_t
     * @flags READ_ONLY
     */
    SAI_TAM_ATTR_TOTAL_NUM_STATISTICS,

    /*
     * @brief Latest Snapshot ID
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     */
    SAI_TAM_ATTR_LATEST_SNAPSHOT_ID,

    /*
     * @brief Maximum Number of snapshots that can be created.
     * If the number of currently created snapshots already reach this limit, any
     * attempt to create more snapshots return error.
     *
     * @type uint32_t
     * @flags READ_ONLY
     */
    SAI_TAM_ATTR_MAX_NUM_SNAPSHOTS,

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
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_create_fn)(
	    _Out_ sai_object_id_t *tam_id,
	    _In_ sai_object_id_t switch_id,
	    _In_ uint32_t attr_count,
	    _In_ const sai_attribute_t *attr_list);

/**
 *
 * @brief Deletes a specified tam object.
 *
 * Deleting a TAM object also deletes all associated snapshot and threshold objects.
 *
 * @param[in] tam_id TAM object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_remove_fn)(
        _In_ sai_object_id_t *tam_id);

/**
 * @brief Set TAM attribute value(s).
 *
 * @param[in] tam_id TAM id
 * @param[in] attr attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_attribute_set_fn)(
        _In_ sai_object_id_t tam_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified TAM attributes.
 *
 * @param[in] tam_id - tam object id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_attribute_get_fn)(
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
     */
    sai_object_id_t threshold_id;

    /**
     * @brief Snapshot Valid
     *
     * Indicates whether the snapshot_id field points to a valid object.
     * is_snapshot_valid is set to false when the attribute
     * SAI_TAM_THRESHOLD_ATTR_SNAPSHOT_ON_BREACH is either unspecified
     * or set to false.
     */
    bool is_snapshot_valid;

    /**
     * @brief Snapshot Id
     * This field is valid only when is_snapshot_valid is set to true
     *
     * @objects SAI_OBJECT_TYPE_TAM_SNAPSHOT
     */
    sai_object_id_t snapshot_id;

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
 * set to false,  then the event data passed to the callback function will 
 * have the field is_snapshot_valid set to false.
 *
 * @param[in] count Number of events
 * @param[in] data Pointer to TAM events data array
 */
typedef void(*sai_tam_event_notification_fn)(
        _In_ uint32_t count,
        _In_ sai_tam_threshold_breach_event_t *data);

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
     * @flags MANDATORY_ON_CREATE
     */
    SAI_TAM_THRESHOLD_ATTR_TAM_ID = SAI_TAM_THRESHOLD_ATTR_START,

    /**
     * @brief Statistic for this threshold
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_TAM_STAT
     * @flags MANDATORY_ON_CREATE
     */
    SAI_TAM_THRESHOLD_ATTR_STATISTIC,

    /**
     * @brief Threshold Level
     * Breach level for this threshold in number of bytes
     *
     * If specified, a threshold breach event will be recorded when the buffer
     * usage goes beyond the level.
     *
     * If not specified, then by default the threshold is created without 
     * any level, which is effectively disabling the threshold monitoring 
     * for the statistic
     *
     * @type uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_THRESHOLD_ATTR_LEVEL,

	/**
     * @brief Transporter Object
     *
     * Provides the snapshot transporter object for this threshold.
     * When the threshold is breached, this transporter will be used
     * to 'copy' the data to the 'transporter-desired' location.
     *
     * In the absence of a transporter, the tracker's default transporter
     * will be used.
     *
     * It may be noted that, Upon a breach, the 'snapshot' object is
     * automatically created (see below attribute), and it will not have a
     * separate transporter object. Instead this transporter (or the tracker's
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
     * Otherwise a snapshot is not created.
     *
     * @type boolean
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_TAM_THRESHOLD_ATTR_SNAPSHOT_ON_BREACH,

    /**
     * @brief Buffers/Statistics for inclusion in the snapshot
     * Specifies the Statistics/Types for the snapshot.
     * If not specified, all buffers tracked by
     * the associated TAM object are included in the snapshot.
     * When specified, the buffers requested for snapshot must be within the set
     * tracked by the associated TAM object.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_STAT
     * @allownull true
     */
    SAI_TAM_SNAPSHOT_ATTR_STATS,

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
 * @param[out] threshold_id  Threshold object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list Preferences for creating a threshold
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_threshold_create_fn)(
        _Out_ sai_object_id_t *threshold_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified threshold object.
 *
 * @param[in] threshold_id  - threshold object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_threshold_remove_fn)(
        _In_ sai_object_id_t *threshold_id);

/**
 * @brief Set threshold attribute value(s).
 *
 * @param[in] threshold_id - threshold object id
 * @param[in] attr attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_threshold_attribute_set_fn)(
        _In_ sai_object_id_t threshold_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified threshold attributes.
 *
 * @param[in] threshold_id Threshold object id
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_threshold_attribute_get_fn)(
        _In_ sai_object_id_t threshold_id,
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
     * @flags MANDATORY_ON_CREATE
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
     * @allownull true
     */
    SAI_TAM_SNAPSHOT_ATTR_STAT_TYPE,

    /**
     * @brief Transporter Object
     *
     * Provides the snapshot transporter object for this snapshot.
     * When the snapshot is made, this transporter will be used
     * to 'copy' the data to the 'transporter-desired' location.
     * In the absence of a transporter, the tracker's default transporter
     * will be used (DEFAULT)
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @allownull true
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
 * @param[out] snapshot_id Snapshot object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_snapshot_create_fn)(
        _Out_ sai_object_id_t *snapshot_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified snapshot object and free driver memory.
 *
 * @param[in] snapshot_id Snapshot object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_snapshot_remove_fn)(
        _In_ sai_object_id_t *snapshot_id);

/**
 * @brief Set Snapshot attribute value(s).
 *
 * @param[in] snapshot_id Snapshot object id
 * @param[in] attr attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_snapshot_attribute_set_fn)(
        _In_ sai_object_id_t snapshot_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified Snapshot attributes.
 *
 * @param[in] snapshot_id Snapshot object id
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_snapshot_attribute_get_fn)(
        _In_ sai_object_id_t snapshot_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Obtain the values for all statistics from a snapshot.
 *
 * Attribute list must supply sufficient memory for statistics
 * as specified for the snapshot object, which may be all statistics
 * supported by the associated tam object.
 *
 * @param[in] snapshot_id Snapshot object id
 * @param[inout] stat_count Number of statistics (required/provided)
 * @param[inout] statistics Statistics (allocated/provided)
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_snapshot_statistics_get_fn)(
        _In_ sai_object_id_t snapshot_id,
        _Inout_ uint32_t *stat_count,
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
typedef enum _sai_tam_transporter_attr_t {
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
     * @type uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_TRANSPORTER_ATTR_MAX_SNAPSHOT_SIZE,

    /**
     * @type Mirroring session object defining the remote transport capabilities.
     *
     * If this attribute is unspecified, Local CPU Transport is used (DEFAULT)
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_TRANSPORTER
     * @allownull true
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
 * to the desired location
 *
 * @param[out] transporter_id  Transporter object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_transporter_create_fn)(
        _Out_ sai_object_id_t *transporter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified Transporter object.
 *
 * @param[in] transporter_id Transporter object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_transporter_remove_fn)(
        _In_ sai_object_id_t *transporter_id);

/**
 * @brief Set TAM Transporter attribute value(s).
 *
 * @param[in] transporter_id - transporter object id
 * @param[in] attr attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_transporter_attribute_set_fn)(
        _In_ sai_object_id_t transporter_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified Transporter attributes.
 *
 * @param[in] transporter_id - Transporter object id
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_tam_transporter_attribute_get_fn)(
        _In_ sai_object_id_t transporter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_tam_api_t 
{
    sai_tam_create_fn                       tam_create;
    sai_tam_remove_fn                       tam_remove;
    sai_tam_attribute_set_fn                tam_attribute_set;
    sai_tam_attribute_get_fn                tam_attribute_get;
    sai_tam_stat_create_fn                  tam_stat_create;
    sai_tam_stat_remove_fn                  tam_stat_remove;
    sai_tam_stat_attribute_set_fn           tam_stat_attribute_set;
    sai_tam_stat_attribute_get_fn           tam_stat_attribute_get;
    sai_tam_threshold_create_fn             tam_threshold_create;
    sai_tam_threshold_remove_fn             tam_threshold_remove;
    sai_tam_threshold_attribute_set_fn      tam_threshold_attribute_set;
    sai_tam_threshold_attribute_get_fn      tam_threshold_attribute_get;
    sai_tam_snapshot_create_fn              tam_snapshot_create;
    sai_tam_snapshot_remove_fn              tam_snapshot_remove;
    sai_tam_snapshot_attribute_set_fn       tam_snapshot_attribute_set;
    sai_tam_snapshot_attribute_get_fn       tam_snapshot_attribute_get;
    sai_tam_snapshot_statistics_get_fn      tam_snapshot_statistics_get;
    sai_tam_transporter_create_fn           tam_transporter_create;
    sai_tam_transporter_remove_fn           tam_transporter_remove;
    sai_tam_transporter_attribute_set_fn    tam_transporter_attribute_set;
    sai_tam_transporter_attribute_get_fn    tam_transporter_attribute_get;
} sai_tam_api_t;

/**
 * @}
 */
#endif /** __SAITAM_H_ */
