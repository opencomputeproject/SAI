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
 * @file    saiwred.h
 *
 * @brief   This module defines SAI QOS WRED interface
 */

#if !defined (__SAIWRED_H_)
#define __SAIWRED_H_

#include "saitypes.h"

/**
 * @defgroup SAIWRED SAI - QOS WRED specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_WRED_ATTR_ECN_MARK_MODE
 */
typedef enum _sai_ecn_mark_mode_t
{
    /** Disable ECN marking for all colors */
    SAI_ECN_MARK_MODE_NONE,

    /** Enable ECN marking for green color. Yellow and red are disabled */
    SAI_ECN_MARK_MODE_GREEN,

    /** Enable ECN marking for yellow color. Green and red are disabled */
    SAI_ECN_MARK_MODE_YELLOW,

    /** Enable ECN marking for red color. Green and yellow are disabled */
    SAI_ECN_MARK_MODE_RED,

    /** Enable ECN marking for green and yellow colors. Red is disabled */
    SAI_ECN_MARK_MODE_GREEN_YELLOW,

    /** Enable ECN marking for green and red colors. Yellow is disabled */
    SAI_ECN_MARK_MODE_GREEN_RED,

    /** Enable ECN marking for yellow and red colors. Green is disabled */
    SAI_ECN_MARK_MODE_YELLOW_RED,

    /** Enable ECN marking for all colors */
    SAI_ECN_MARK_MODE_ALL,

} sai_ecn_mark_mode_t;

/**
 * @brief Enum defining WRED profile attributes
 */
typedef enum _sai_wred_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_WRED_ATTR_START = 0x00000000,

    /**
     * @brief Green enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_WRED_ATTR_GREEN_ENABLE = SAI_WRED_ATTR_START,

    /**
     * @brief Green minimum threshold bytes
     *
     * Range 1 - Max Buffer size.
     *
     * Default to 0 i.e. maximum buffer size.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_GREEN_ENABLE == true or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_GREEN_MIN_THRESHOLD = 0x00000001,

    /**
     * @brief Green maximum threshold
     *
     * Range 1 - Max Buffer size.
     * Default to 0 i.e. maximum buffer size.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_GREEN_ENABLE == true or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_GREEN_MAX_THRESHOLD = 0x00000002,

    /**
     * @brief Percentage 0 ~ 100
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 100
     */
    SAI_WRED_ATTR_GREEN_DROP_PROBABILITY = 0x00000003,

    /**
     * @brief Yellow enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_WRED_ATTR_YELLOW_ENABLE = 0x00000004,

    /**
     * @brief Yellow minimum threshold
     *
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_YELLOW_ENABLE == true or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_YELLOW_MIN_THRESHOLD = 0x00000005,

    /**
     * @brief Yellow maximum threshold
     *
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_YELLOW_ENABLE == true or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_YELLOW_MAX_THRESHOLD = 0x00000006,

    /**
     * @brief Percentage 0 ~ 100
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 100
     */
    SAI_WRED_ATTR_YELLOW_DROP_PROBABILITY = 0x00000007,

    /**
     * @brief Red enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_WRED_ATTR_RED_ENABLE = 0x00000008,

    /**
     * @brief Red minimum threshold
     *
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_RED_ENABLE == true or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_RED_MIN_THRESHOLD = 0x00000009,

    /**
     * @brief Red maximum threshold
     *
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_RED_ENABLE == true or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_RED_MAX_THRESHOLD = 0x0000000a,

    /**
     * @brief Percentage 0 ~ 100
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 100
     */
    SAI_WRED_ATTR_RED_DROP_PROBABILITY = 0x0000000b,

    /**
     * @brief Weight 0 ~ 15
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_WRED_ATTR_WEIGHT = 0x0000000c,

    /**
     * @brief ECN mark mode
     *
     * Enable/disable ECN marking
     *
     * @type sai_ecn_mark_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_ECN_MARK_MODE_NONE
     */
    SAI_WRED_ATTR_ECN_MARK_MODE = 0x0000000d,

    /**
     * @brief Green minimum threshold bytes for ECT traffic.
     *        In absence of this attribute, green ECT traffic
     *        would use SAI_WRED_ATTR_GREEN_MIN_THRESHOLD value as min threshold.
     *
     * Valid when SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE == true.
     * Range 1 - Max Buffer size.
     *
     * Default to 0 i.e. maximum buffer size.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_ECN_GREEN_MIN_THRESHOLD = 0x0000000e,

    /**
     * @brief Green maximum threshold for ECT traffic
     *        In absence of this attribute, green ECT traffic
     *        would use SAI_WRED_ATTR_GREEN_MAX_THRESHOLD value as max threshold.
     *
     * Valid when SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE == true.
     * Range 1 - Max Buffer size.
     * Default to 0 i.e. maximum buffer size.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_ECN_GREEN_MAX_THRESHOLD = 0x0000000f,

    /**
     * @brief Marking percentage 0 ~ 100 for green ECT traffic
     *        In absence of this attribute, green ECT traffic
     *        would use SAI_WRED_ATTR_GREEN_DROP_PROBABILITY value as marking probability.
     *
     * Valid when SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE == true.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 100
     * @validonly SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_ECN_GREEN_MARK_PROBABILITY = 0x00000010,

    /**
     * @brief Yellow minimum threshold for ECT traffic
     *        In absence of this attribute, yellow ECT traffic
     *        would use SAI_WRED_ATTR_YELLOW_MIN_THRESHOLD value as min threshold.
     *
     * Valid when SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE == true.
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_ECN_YELLOW_MIN_THRESHOLD = 0x00000011,

    /**
     * @brief Yellow maximum threshold for ECT traffic
     *        In absence of this attribute, yellow ECT traffic
     *        would use SAI_WRED_ATTR_YELLOW_MAX_THRESHOLD value as max threshold.
     *
     * Valid when SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE == true.
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_ECN_YELLOW_MAX_THRESHOLD = 0x00000012,

    /**
     * @brief Marking percentage 0 ~ 100 for yellow ECT traffic
     *        In absence of this attribute, yellow ECT traffic
     *        would use SAI_WRED_ATTR_YELLOW_DROP_PROBABILITY value as marking probability.
     *
     * Valid when SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE == true.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 100
     * @validonly SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_ECN_YELLOW_MARK_PROBABILITY = 0x00000013,

    /**
     * @brief Red minimum threshold for ECT traffic
     *        In absence of this attribute, red ECT traffic
     *        would use SAI_WRED_ATTR_RED_MIN_THRESHOLD value as min threshold.
     *
     * Valid when SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE == true.
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_ECN_RED_MIN_THRESHOLD = 0x00000014,

    /**
     * @brief Red maximum threshold for ECT traffic
     *        In absence of this attribute, red ECT traffic
     *        would use SAI_WRED_ATTR_RED_MAX_THRESHOLD value as max threshold.
     *
     * Valid when SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE == true.
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_ECN_RED_MAX_THRESHOLD = 0x00000015,

    /**
     * @brief Marking percentage 0 ~ 100 for red ECT traffic
     *        In absence of this attribute, red ECT traffic
     *        would use SAI_WRED_ATTR_RED_DROP_PROBABILITY value as marking probability.
     *
     * Valid when SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE == true.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 100
     * @validonly SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_ECN_RED_MARK_PROBABILITY = 0x00000016,

    /**
     * @brief Color unaware minimum threshold for ECT traffic.
     *
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     * Valid when SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE == true.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_ECN_COLOR_UNAWARE_MIN_THRESHOLD = 0x00000017,

    /**
     * @brief Color unaware maximum threshold for ECT traffic.
     *
     * Valid when SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE == true.
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_ECN_COLOR_UNAWARE_MAX_THRESHOLD = 0x00000018,

    /**
     * @brief Marking percentage 0 ~ 100 for color unaware ECT traffic.
     *
     * Valid when SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE == true.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 100
     * @validonly SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     */
    SAI_WRED_ATTR_ECN_COLOR_UNAWARE_MARK_PROBABILITY = 0x00000019,

    /**
     * @brief End of attributes
     */
    SAI_WRED_ATTR_END,

    /** Custom range base value */
    SAI_WRED_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_WRED_ATTR_CUSTOM_RANGE_END

} sai_wred_attr_t;

/**
 * @brief Create WRED Profile
 *
 * @param[out] wred_id WRED profile Id.
 * @param[in] switch_id Switch Id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_wred_fn)(
        _Out_ sai_object_id_t *wred_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove WRED Profile
 *
 * @param[in] wred_id WRED profile Id.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_wred_fn)(
        _In_ sai_object_id_t wred_id);

/**
 * @brief Set attributes to WRED profile.
 *
 * @param[in] wred_id WRED profile Id.
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_wred_attribute_fn)(
        _In_ sai_object_id_t wred_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get WRED profile attribute
 *
 * @param[in] wred_id WRED Profile Id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_wred_attribute_fn)(
        _In_ sai_object_id_t wred_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief WRED methods table retrieved with sai_api_query()
 */
typedef struct _sai_wred_api_t
{
    sai_create_wred_fn          create_wred;
    sai_remove_wred_fn          remove_wred;
    sai_set_wred_attribute_fn   set_wred_attribute;
    sai_get_wred_attribute_fn   get_wred_attribute;

} sai_wred_api_t;

/**
 * @}
 */
#endif /** __SAIWRED_H_ */
