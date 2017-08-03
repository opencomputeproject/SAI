/**
 * Copyright (c) 2017 Microsoft Open Technologies, Inc.
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
 * @file    saiuburst.h
 *
 * @brief   This module defines SAI TAM Microbursts Monitoring interface
 */

#if !defined (__SAIUBURST_H_)
#define __SAIUBURST_H_

#include <saitypes.h>

/**
 * @defgroup SAIUBURST SAI - TAM Microbursts Monitoring API
 *
 * @{
 */

/**
 * @brief Enum defining statistics for microburst.
 */
typedef enum _sai_tam_microburst_stat_t
{
    /** Get/set last uBurst duration in us [uint64_t] */
    SAI_TAM_MICROBURST_STAT_LAST_DURATION = 0x00000000,

    /** Get/set longest uBurst duration in us [uint64_t] */
    SAI_TAM_MICROBURST_STAT_LONGEST_DURATION = 0x00000001,

    /** Get/set shortest uBurst duration in us [uint64_t] */
    SAI_TAM_MICROBURST_STAT_SHORTEST_DURATION = 0x00000002,

    /** Get/set average uBurst duration in us [uint64_t] */
    SAI_TAM_MICROBURST_STAT_AVERAGE_DURATION = 0x00000003,

    /** Get/set number of uBursts [uint64_t] */
    SAI_TAM_MICROBURST_STAT_NUMBER = 0x00000004,

    /** Custom range base value */
    SAI_TAM_MICROBURST_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_tam_microburst_stat_t;

/**
 * @brief TAM Microburst Attributes.
 */
typedef enum _sai_tam_microburst_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_MICROBURST_ATTR_START,

    /**
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_MICROBURST_ATTR_TAM_ID = SAI_TAM_MICROBURST_ATTR_START,

    /**
     * @brief Statistic for this microburst
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_STAT
     */
    SAI_TAM_MICROBURST_ATTR_STATISTIC,

    /**
     * @brief Watermark Levels
     *
     * Breach high/low watermark level for this microburst statistic
     * in number of bytes.
     *
     * If not specified, the microburst is created without any
     * levels, which is effectively disabling the microburst
     * monitoring for the statistic.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_MICROBURST_ATTR_LEVEL_A,

    /**
     * @brief Watermark Levels
     *
     * Breach low/high watermark level for this microburst statistic
     * in number of bytes.
     *
     * If not specified, the microburst is created without any
     * levels, which is effectively disabling the microburst
     * monitoring for the statistic.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_MICROBURST_ATTR_LEVEL_B,

    /**
     * @brief Transporter Object
     *
     * Provides the transporter object for this microburst.
     *
     * In the absence of a transporter, the default transporter will
     * be used.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_TRANSPORTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_MICROBURST_ATTR_TRANSPORTER,

    /**
     * @brief Statistics for inclusion in the microburst
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_STAT
     * @default empty
     */
    SAI_TAM_MICROBURST_ATTR_STATS,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_MICROBURST_ATTR_END,

    /** Custom range base value */
    SAI_TAM_MICROBURST_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_MICROBURST_ATTR_CUSTOM_RANGE_END

} sai_tam_microburst_attr_t;

/**
 * @brief TAM Histogram Attributes.
 */
typedef enum _sai_tam_histogram_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_HISTOGRAM_ATTR_START,

    /**
     * @brief TAM Object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_HISTOGRAM_ATTR_TAM_ID = SAI_TAM_HISTOGRAM_ATTR_START,

    /**
     * @brief Buffers/Statistics for inclusion in histogram
     *
     * Specifies the Statistics/Types for a histogram.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_STAT
     * @default empty
     */
    SAI_TAM_HISTOGRAM_ATTR_STAT_TYPE,

    /**
     * @brief Histogram Bins Lower Boundaries
     *
     * List of lower boundary of each bin for this HISTOGRAM in
     * number referred object units. The upper boundary of a bin is
     * the lower boundary of next bin. The upper boundary of the
     * last bin is infinity.
     *
     * @type sai_u32_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_TAM_HISTOGRAM_ATTR_BIN_BOUNDARY,

    /**
     * @brief Histogram Resolution
     *
     * The resolution to read measure statistics for inclusion in histogram
     * May be every statistic object (as packet or microburst) or time interval
     * (as for buffer current usage level)
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_HISTOGRAM_ATTR_RESOLUTION,

    /**
     * @brief Histogram Clear-On-Read Mode
     *
     * If true the histogram bin values are clear on read/transport.
     * Otherwise values in bins are counting continuously.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_TAM_HISTOGRAM_ATTR_CLEAR_MODE,

    /**
     * @brief Transporter Object
     *
     * Provides the transporter object for this histogram. When the
     * data shot is made, this transporter will be used to 'copy'
     * the data to the 'transporter-desired' location. In the
     * absence of a transporter, the tracker's default transporter
     * will be used (DEFAULT).
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM_TRANSPORTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_HISTOGRAM_ATTR_TRANSPORTER,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_HISTOGRAM_ATTR_END,

    /** Custom range base value */
    SAI_TAM_HISTOGRAM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_HISTOGRAM_ATTR_CUSTOM_RANGE_END

} sai_tam_histogram_attr_t;

/**
 * @brief Create and return a microburst object
 *
 * @param[out] tam_microburst_id Microburst object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of sai_tam_microburst_attr_t attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_microburst_fn)(
        _Out_ sai_object_id_t *tam_microburst_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a specified microburst object
 *
 * @param[in] tam_microburst_id Microburst object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_microburst_fn)(
        _In_ sai_object_id_t tam_microburst_id);

/**
 * @brief Get values for specified microburst attributes.
 *
 * @param[in] tam_microburst_id Microburst object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_microburst_attribute_fn)(
        _In_ sai_object_id_t tam_microburst_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Set microburst attribute
 *
 * @param[in] tam_microburst_id Microburst object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_microburst_attribute_fn)(
        _In_ sai_object_id_t tam_microburst_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Create and return a histogram object
 *
 * This creates a histogram in the driver. Via the attributes,
 * caller may indicate a preference for histogram of a specific
 * set of bins.
 *
 * @param[out] tam_histogram_id Histogram object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_tam_histogram_fn)(
        _Out_ sai_object_id_t *tam_histogram_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a specified histogram object and free driver memory
 *
 * @param[in] tam_histogram_id Histogram object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_tam_histogram_fn)(
        _In_ sai_object_id_t tam_histogram_id);

/**
 * @brief Set histogram attribute value(s)
 *
 * @param[in] tam_histogram_id Histogram object id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_tam_histogram_attribute_fn)(
        _In_ sai_object_id_t tam_histogram_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified histogram attributes.
 *
 * @param[in] tam_histogram_id Histogram object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_histogram_attribute_fn)(
        _In_ sai_object_id_t tam_histogram_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Obtain the values for all bins from a histogram.
 *
 * Values array must supply sufficient memory for values of all
 * bins as specified for the histogram object.
 *
 * @param[in] tam_histogram_id Histogram object id
 * @param[inout] number_of_counters Number of bins (required/provided)
 * @param[out] counters Statistics values (allocated/provided)
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_tam_histogram_stats_fn)(
        _In_ sai_object_id_t tam_histogram_id,
        _Inout_ uint32_t *number_of_counters,
        _Out_ uint64_t *counters);

typedef struct _sai_uburst_api_t
{
    sai_create_tam_microburst_fn            create_tam_microburst;
    sai_remove_tam_microburst_fn            remove_tam_microburst;
    sai_set_tam_microburst_attribute_fn     set_tam_microburst_attribute;
    sai_get_tam_microburst_attribute_fn     get_tam_microburst_attribute;
    sai_create_tam_histogram_fn             create_tam_histogram;
    sai_remove_tam_histogram_fn             remove_tam_histogram;
    sai_set_tam_histogram_attribute_fn      set_tam_histogram_attribute;
    sai_get_tam_histogram_attribute_fn      get_tam_histogram_attribute;
    sai_get_tam_histogram_stats_fn          get_tam_histogram_stats;
} sai_uburst_api_t;

/**
 * @}
 */
#endif /** __SAIUBURST_H_ */
