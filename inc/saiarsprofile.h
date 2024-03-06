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
 * @file    saiarsprofile.h
 *
 * @brief   This module defines SAI QOS Maps interface
 */

#if !defined (__SAIARSPROFILE_H_)
#define __SAIARSPROFILE_H_

#include <saitypes.h>

/**
 * @defgroup SAIARSPROFILE SAI - ARS profile specific API definitions.
 *
 * @{
 */

/**
 * @brief Adaptive Routing and Switching quality measure algorithm
 */
typedef enum _sai_ars_profile_algo_t
{
    /** Exponentially weighted moving average */
    SAI_ARS_PROFILE_ALGO_EWMA,

} sai_ars_profile_algo_t;

/**
 * @brief Enum defining attributes for ARS profile.
 */
typedef enum _sai_ars_profile_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_ARS_PROFILE_ATTR_START,

    /**
     * @brief ARS algorithm used for quality computation
     *
     * @type sai_ars_profile_algo_t
     * @flags CREATE_AND_SET
     * @default SAI_ARS_PROFILE_ALGO_EWMA
     */
    SAI_ARS_PROFILE_ATTR_ALGO = SAI_ARS_PROFILE_ATTR_START,

    /**
     * @brief Sampling interval in microseconds of data for quality measure computation
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 16
     */
    SAI_ARS_PROFILE_ATTR_SAMPLING_INTERVAL,

    /**
     * @brief Random seed for adaptive routing and switching
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_ARS_RANDOM_SEED,

    /**
     * @brief Maximum number of ECMP ARS groups
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_ECMP_ARS_MAX_GROUPS,

    /**
     * @brief Maximum number of members per ECMP ARS group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_ECMP_ARS_MAX_MEMBERS_PER_GROUP,

    /**
     * @brief Maximum number of LAG ARS groups
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_LAG_ARS_MAX_GROUPS,

    /**
     * @brief Maximum number of members per LAG ARS group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_LAG_ARS_MAX_MEMBERS_PER_GROUP,

    /**
     * @brief Enable past port load as the quality parameter. This is the average egress bytes measured on a port
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     * @validonly SAI_ARS_PROFILE_ATTR_ALGO == SAI_ARS_PROFILE_ALGO_EWMA
     */
    SAI_ARS_PROFILE_ATTR_PORT_LOAD_PAST,

    /**
     * @brief Weight attribute is used in EWMA calculations.
     * Large weight lowers the significance of instantaneous measure on the overall average.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 16
     * @validonly SAI_ARS_PROFILE_ATTR_ALGO == SAI_ARS_PROFILE_ALGO_EWMA
     */
    SAI_ARS_PROFILE_ATTR_PORT_LOAD_PAST_WEIGHT,

    /**
     * @brief Enable future port load as the quality parameter. This is the average queued bytes measured on a port.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     * @validonly SAI_ARS_PROFILE_ATTR_ALGO == SAI_ARS_PROFILE_ALGO_EWMA
     */
    SAI_ARS_PROFILE_ATTR_PORT_LOAD_FUTURE,

    /**
     * @brief Weight attribute is used in EWMA calculations.
     * Large weight lowers the significance of instantaneous measure on the overall average.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 16
     * @validonly SAI_ARS_PROFILE_ATTR_ALGO == SAI_ARS_PROFILE_ALGO_EWMA
     */
    SAI_ARS_PROFILE_ATTR_PORT_LOAD_FUTURE_WEIGHT,

    /**
     * @brief Set port loading value to current sampled value when sampled value is less than the average.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_ARS_PROFILE_ATTR_ALGO == SAI_ARS_PROFILE_ALGO_EWMA
     */
    SAI_ARS_PROFILE_ATTR_PORT_LOAD_CURRENT,

    /**
     * @brief EWMA exponent used in port loading computation. Larger the exponent, larger is the weight to previously computed port loading value.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 2
     * @validonly SAI_ARS_PROFILE_ATTR_ALGO == SAI_ARS_PROFILE_ALGO_EWMA
     */
    SAI_ARS_PROFILE_ATTR_PORT_LOAD_EXPONENT,

    /**
     * @brief Number of quantization bands support for quality map.
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BANDS,

    /**
     * @brief Minimum threshold used for quantization process in Mbps for band 0.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_0_MIN_THRESHOLD,

    /**
     * @brief Maximum threshold used for quantization process in Mbps for band 0.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_0_MAX_THRESHOLD,

    /**
     * @brief Minimum threshold used for quantization process in Mbps for band 1.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_1_MIN_THRESHOLD,

    /**
     * @brief Maximum threshold used for quantization process in Mbps for band 1.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_1_MAX_THRESHOLD,

    /**
     * @brief Minimum threshold used for quantization process in Mbps for band 2.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_2_MIN_THRESHOLD,

    /**
     * @brief Maximum threshold used for quantization process in Mbps for band 2.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_2_MAX_THRESHOLD,

    /**
     * @brief Minimum threshold used for quantization process in Mbps for band 3.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_3_MIN_THRESHOLD,

    /**
     * @brief Maximum threshold used for quantization process in Mbps for band 3.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_3_MAX_THRESHOLD,

    /**
     * @brief Minimum threshold used for quantization process in Mbps for band 4.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_4_MIN_THRESHOLD,

    /**
     * @brief Maximum threshold used for quantization process in Mbps for band 4.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_4_MAX_THRESHOLD,

    /**
     * @brief Minimum threshold used for quantization process in Mbps for band 5.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_5_MIN_THRESHOLD,

    /**
     * @brief Maximum threshold used for quantization process in Mbps for band 5.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_5_MAX_THRESHOLD,

    /**
     * @brief Minimum threshold used for quantization process in Mbps for band 6.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_6_MIN_THRESHOLD,

    /**
     * @brief Maximum threshold used for quantization process in Mbps for band 6.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_6_MAX_THRESHOLD,

    /**
     * @brief Minimum threshold used for quantization process in Mbps for band 7.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_7_MIN_THRESHOLD,

    /**
     * @brief Maximum threshold used for quantization process in Mbps for band 7.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_7_MAX_THRESHOLD,

    /**
     * @brief Minimum past load threshold value for quantization process.
     * Used by hardware to allocate the quantization bands.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_LOAD_PAST_MIN_VAL,

    /**
     * @brief Maximum past load threshold value for quantization process.
     * Used by hardware to allocate the quantization bands.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_LOAD_PAST_MAX_VAL,

    /**
     * @brief Minimum threshold used for quantization bands for past load
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_MIN_THRESHOLD_LIST_LOAD_PAST,

    /**
     * @brief Maximum threshold used for quantization bands for past load
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_MAX_THRESHOLD_LIST_LOAD_PAST,

    /**
     * @brief Minimum future load threshold value for quantization process.
     * Used by hardware to allocate the quantization bands.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_LOAD_FUTURE_MIN_VAL,

    /**
     * @brief Maximum future load threshold value for quantization process.
     * Used by hardware to allocate the quantization bands.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_LOAD_FUTURE_MAX_VAL,

    /**
     * @brief Minimum threshold used for quantization bands for future load
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_MIN_THRESHOLD_LIST_LOAD_FUTURE,

    /**
     * @brief Maximum threshold used for quantization bands for future load
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_MAX_THRESHOLD_LIST_LOAD_FUTURE,

    /**
     * @brief Minimum current load threshold value for quantization process.
     * Used by hardware to allocate the quantization bands.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_LOAD_CURRENT_MIN_VAL,

    /**
     * @brief Maximum current load threshold value for quantization process.
     * Used by hardware to allocate the quantization bands.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_LOAD_CURRENT_MAX_VAL,

    /**
     * @brief Minimum threshold used for quantization bands for current load
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_MIN_THRESHOLD_LIST_LOAD_CURRENT,

    /**
     * @brief Maximum threshold used for quantization bands for current load
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_QUANT_BAND_MAX_THRESHOLD_LIST_LOAD_CURRENT,

    /**
     * @brief Maximum flows supported for ARS processing
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ARS_PROFILE_ATTR_MAX_FLOWS,

    /**
     * @brief End of attributes
     */
    SAI_ARS_PROFILE_ATTR_END,

    /** Custom range base value */
    SAI_ARS_PROFILE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_ARS_PROFILE_ATTR_CUSTOM_RANGE_END

} sai_ars_profile_attr_t;

/**
 * @brief Create ARS Profile
 *
 * @param[out] ars_profile_id ARS Profile Id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_ars_profile_fn)(
        _Out_ sai_object_id_t *ars_profile_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove ARS Profile
 *
 * @param[in] ars_profile_id ARS Profile id to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_ars_profile_fn)(
        _In_ sai_object_id_t ars_profile_id);

/**
 * @brief Set attributes for ARS profile
 *
 * @param[in] ars_profile_id ARS Profile Id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_ars_profile_attribute_fn)(
        _In_ sai_object_id_t ars_profile_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attributes of ARS profile
 *
 * @param[in] ars_profile_id ARS Profile id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ars_profile_attribute_fn)(
        _In_ sai_object_id_t ars_profile_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief ARS Profile methods table retrieved with sai_api_query()
 */
typedef struct _sai_ars_profile_api_t
{
    sai_create_ars_profile_fn         create_ars_profile;
    sai_remove_ars_profile_fn         remove_ars_profile;
    sai_set_ars_profile_attribute_fn  set_ars_profile_attribute;
    sai_get_ars_profile_attribute_fn  get_ars_profile_attribute;

} sai_ars_profile_api_t;

/**
 * @}
 */
#endif /** __SAIARSPROFILE_H_ */
