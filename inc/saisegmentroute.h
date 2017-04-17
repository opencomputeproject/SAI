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
 * @file    saisegmentroute.h
 *
 * @brief   This module defines SAI Segment Route Entry interface
 */

#if !defined (__SAISEGMENTROUTE_H_)
#define __SAISEGMENTROUTE_H_

#include <saitypes.h>

/**
 * @defgroup SAISEGMENTROUTE SAI - Segment Route specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for SAI Segment Route Entry
 */
typedef enum _sai_segmentroute_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SEGMENTROUTE_ATTR_START = 0x00000000,

    /**
     * @brief Number of Segments supported on device for origination
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_SEGMENTROUTE_ATTR_SUPPORTED_NUM_SEGMENTS = SAI_SEGMENTROUTE_ATTR_START,

    /**
     * @brief List of TLV types supported on device for origination
     *
     * @type sai_s32_list_t sai_tlv_type_t
     * @flags READ_ONLY
     */
    SAI_SEGMENTROUTE_ATTR_SUPPORTED_TLV_TYPE,

    /**
     * @brief List of TLVs for origination
     *
     * @type sai_tlv_list_t
     * @flags CREATE_AND_SET
     */
    SAI_SEGMENTROUTE_ATTR_TLV_LIST,

    /**
     * @brief List of Segments to Originate
     *
     * @type sai_segment_list_t
     * @flags CREATE_AND_SET
     */
    SAI_SEGMENTROUTE_ATTR_SEGMENT_LIST,

    /**
     * @brief End of attributes
     */
    SAI_SEGMENTROUTE_ATTR_END,

    /** Custom range base value */
    SAI_SEGMENTROUTE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SEGMENTROUTE_ATTR_CUSTOM_RANGE_END
} sai_segmentroute_attr_t;

/**
 * @brief Create Route
 *
 * @param[out] segment_id Segment ID
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_segmentroute_fn)(
        _Out_ sai_object_id_t *segment_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Segment Route
 *
 * @param[in] segment_id Segment ID
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_segmentroute_fn)(
        _In_ sai_object_id_t segment_id);

/**
 * @brief Set segment route attribute value
 *
 * @param[in] segment_id Segment ID
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_segmentroute_attribute_fn)(
        _In_ sai_object_id_t segment_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get route attribute value
 *
 * @param[in] segment_id Segment ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_segmentroute_attribute_fn)(
        _In_ sai_object_id_t segment_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Segment Route methods table retrieved with sai_api_query()
 */
typedef struct _sai_segmentroute_api_t
{
    sai_create_segmentroute_fn create_segmentroute;
    sai_remove_segmentroute_fn remove_segmentroute;
    sai_set_segmentroute_attribute_fn set_segmentroute_attribute;
    sai_get_segmentroute_attribute_fn get_segmentroute_attribute;
} sai_segmentroute_api_t;

/**
 * @}
 */
#endif /** __SAISEGMENTROUTE_H_ */
