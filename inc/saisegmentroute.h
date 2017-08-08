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
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
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
 * @brief Enum defining Transit or Source types
 */
typedef enum _sai_segmentroute_sidlist_type_t
{
    /** Insertion of Segment Route Policy */
    SAI_SEGMENTROUTE_SIDLIST_TYPE_INSERT,

    /** Encapsulation in a Segment Route Policy */
    SAI_SEGMENTROUTE_SIDLIST_TYPE_ENCAPS,

    /** Custom range base value */
    SAI_SEGMENTROUTE_SIDLIST_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_segmentroute_sidlist_type_t;

/**
 * @brief Attribute data for Segment Route Segment ID List Entry
 */
typedef enum _sai_segmentroute_sidlist_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SEGMENTROUTE_SIDLIST_ATTR_START = 0x00000000,

    /**
     * @brief Transit or Source Type
     *
     * @type sai_segmentroute_sidlist_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_SEGMENTROUTE_SIDLIST_ATTR_TYPE = SAI_SEGMENTROUTE_SIDLIST_ATTR_START,

    /**
     * @brief List of Type Length Values for Source
     *
     * @type sai_tlv_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SEGMENTROUTE_SIDLIST_ATTR_TLV_LIST,

    /**
     * @brief List of Segments for Source / Transit
     *
     * @type sai_segment_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SEGMENTROUTE_SIDLIST_ATTR_SEGMENT_LIST,

    /**
     * @brief End of attributes
     */
    SAI_SEGMENTROUTE_SIDLIST_ATTR_END,

    /** Custom range base value */
    SAI_SEGMENTROUTE_SIDLIST_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SEGMENTROUTE_SIDLIST_ATTR_CUSTOM_RANGE_END
} sai_segmentroute_sidlist_attr_t;

/**
 * @brief Create Segment ID List
 *
 * @param[out] segmentroute_sidlist_id Segment ID List ID
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_segmentroute_sidlist_fn)(
        _Out_ sai_object_id_t *segmentroute_sidlist_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Segment ID List
 *
 * @param[in] segmentroute_sidlist_id Segment ID List ID
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_segmentroute_sidlist_fn)(
        _In_ sai_object_id_t segmentroute_sidlist_id);

/**
 * @brief Set Segment ID List attribute value
 *
 * @param[in] segmentroute_sidlist_id Segment ID List ID
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_segmentroute_sidlist_attribute_fn)(
        _In_ sai_object_id_t segmentroute_sidlist_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Segment ID List attribute value
 *
 * @param[in] segmentroute_sidlist_id Segment ID List ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_segmentroute_sidlist_attribute_fn)(
        _In_ sai_object_id_t segmentroute_sidlist_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Segment Route methods table retrieved with sai_api_query()
 */
typedef struct _sai_segmentroute_api_t
{
    sai_create_segmentroute_sidlist_fn        create_segmentroute_sidlist;
    sai_remove_segmentroute_sidlist_fn        remove_segmentroute_sidlist;
    sai_set_segmentroute_sidlist_attribute_fn set_segmentroute_sidlist_attribute;
    sai_get_segmentroute_sidlist_attribute_fn get_segmentroute_sidlist_attribute;
    sai_bulk_object_create_fn                 create_segmentroute_sidlists;
    sai_bulk_object_remove_fn                 remove_segmentroute_sidlists;
} sai_segmentroute_api_t;

/**
 * @}
 */
#endif /** __SAISEGMENTROUTE_H_ */
