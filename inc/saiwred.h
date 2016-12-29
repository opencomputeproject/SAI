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
 * @file    saiwred.h
 *
 * @brief   This module defines SAI QOS Wred interface
 */

#if !defined (__SAIWRED_H_)
#define __SAIWRED_H_

#include "saitypes.h"

/**
 * @defgroup SAIWRED SAI - Qos Wred specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_WRED_ATTR_ECN_MARK_MODE
 */
typedef enum _sai_ecn_mark_mode_t
{
    /** disable ECN marking for all colors */
    SAI_ECN_MARK_MODE_NONE,

    /** enable ECN marking for green color. Yellow and red are disabled */
    SAI_ECN_MARK_MODE_GREEN,

    /** enable ECN marking for yellow color. Green and red are disabled */
    SAI_ECN_MARK_MODE_YELLOW,

    /** enable ECN marking for red color. Green and yellow are disabled */
    SAI_ECN_MARK_MODE_RED,

    /** enable ECN marking for green and yellow colors. Red is disabled */
    SAI_ECN_MARK_MODE_GREEN_YELLOW,

    /** enable ECN marking for green and red colors. Yellow is disabled */
    SAI_ECN_MARK_MODE_GREEN_RED,

    /** enable ECN marking for yellow and red colors. Green is disabled */
    SAI_ECN_MARK_MODE_YELLOW_RED,

    /** enable ECN marking for all colors */
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
     * default to 0 i.e Maximum buffer size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @validonly SAI_WRED_ATTR_GREEN_ENABLE == true or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     * @default 0
     */
    SAI_WRED_ATTR_GREEN_MIN_THRESHOLD = 0x00000001,

    /**
     * @brief Green maximum threshold
     *
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @validonly SAI_WRED_ATTR_GREEN_ENABLE == true or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     * @default 0
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
     * @validonly SAI_WRED_ATTR_YELLOW_ENABLE == true or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     * @default 0
     */
    SAI_WRED_ATTR_YELLOW_MIN_THRESHOLD = 0x00000005,

    /**
     * @brief Yellow maximum threshold
     *
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     *
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @validonly SAI_WRED_ATTR_YELLOW_ENABLE == true or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_YELLOW or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     * @default 0
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
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @validonly SAI_WRED_ATTR_RED_ENABLE == true or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     * @default 0
     */
    SAI_WRED_ATTR_RED_MIN_THRESHOLD = 0x00000009,

    /**
     * @brief Red maximum threshold
     *
     * Range 1 - Max Buffer size.
     * default to 0 i.e Maximum buffer size
     *
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @validonly SAI_WRED_ATTR_RED_ENABLE == true or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_GREEN_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_YELLOW_RED or SAI_WRED_ATTR_ECN_MARK_MODE == SAI_ECN_MARK_MODE_ALL
     * @default 0
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
 * @param[out] wred_id Wred profile Id.
 * @param[in] switch_id Switch Id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_wred_fn)(
        _Out_ sai_object_id_t *wred_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove WRED Profile
 *
 * @param[in] wred_id Wred profile Id.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_wred_fn)(
        _In_ sai_object_id_t wred_id);

/**
 * @brief Set attributes to Wred profile.
 *
 * @param[out] wred_id Wred profile Id.
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_wred_attribute_fn)(
        _In_ sai_object_id_t wred_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Wred profile attribute
 *
 * @param[in] wred_id Wred Profile Id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
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
    sai_create_wred_fn          create_wred_profile;
    sai_remove_wred_fn          remove_wred_profile;
    sai_set_wred_attribute_fn   set_wred_attribute;
    sai_get_wred_attribute_fn   get_wred_attribute;

} sai_wred_api_t;

/**
 * @}
 */
#endif /** __SAIWRED_H_ */
