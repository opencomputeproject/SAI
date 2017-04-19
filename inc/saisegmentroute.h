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
 * @brief Enum defining Endpoint Action types
 */
typedef enum _sai_segmentroute_endpoint_entry_action_type_t
{
    /** Basic Endpoint */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ACTION_TYPE_E,

    /** End.X Endpoint with Layer-3 Cross-connect */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ACTION_TYPE_X,

    /** End.T Endpoint with specific IPv6 Table */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ACTION_TYPE_T,

    /** Endpoint with decapsulation and Layer 2 Cross-connect */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ACTION_TYPE_DX2,

    /** Endpoint with decapsulation and IPv6 Cross-connect */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ACTION_TYPE_DX6,

    /** Endpoint with decapsulation and IPv4 Cross-connect */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ACTION_TYPE_DX4,

    /** Endpoint with decapsulation and specific IPv6 */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ACTION_TYPE_DT6,

    /** Endpoint with decapsulation and specific IPv6 */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ACTION_TYPE_DT4,

    /** Custom range base value */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ACTION_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_segmentroute_endpoint_entry_action_type_t;

/**
 * @brief Enum defining Endpoint Segment Pop types for End, End.X and End.T
 */
typedef enum _sai_segmentroute_endpoint_entry_pop_type_t
{
    /** Penultimate segment pop */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_POP_TYPE_PSP,

    /** Ultimate Segment pop */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_POP_TYPE_USP,

} sai_segmentroute_endpoint_entry_pop_type_t;

/**
 * @brief Attribute Id for sai_segmentroute_endpoint_entry
 */
typedef enum _sai_segmentroute_endpoint_entry_attr_t
{
    /**
     * @brief Start of Segment Route Endpoint Entry attributes
     */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ATTR_START,

    /**
     * @brief SAI Segment Route Endpoint Action Type
     *
     * @type sai_segmentroute_endpoint_entry_action_type_t
     * @flags CREATE_AND_SET
     * @default SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ACTION_TYPE_E
     */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ATTR_ACTION_TYPE = SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ATTR_START,

    /**
     * @brief SAI Segment Route Endpoint Pop Type
     *
     * @type sai_segmentroute_endpoint_entry_pop_type_t
     * @flags CREATE_AND_SET
     * @default SAI_SEGMENTROUTE_ENDPOINT_ENTRY_POP_TYPE_USP
     */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ATTR_POP_TYPE,

    /**
     * @brief End of Endpoint Entry attributes
     */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_segmentroute_endpoint_entry_attr_t;

/**
 * @brief Enum defining Transit or Origination types
 */
typedef enum _sai_segmentroute_transit_type_t
{
    /** Insertion of Segment Route Policy */
    SAI_SEGMENTROUTE_TRANSIT_TYPE_INSERT,

    /** Encapsulation in a Segment Route Policy and Origination */
    SAI_SEGMENTROUTE_TRANSIT_TYPE_ENCAPS_ORIGINATION,

    /** Encapsulation in a Segment Route Policy and Ethernet */
    SAI_SEGMENTROUTE_TRANSIT_TYPE_ENCAPS_L2,

    /** Custom range base value */
    SAI_SEGMENTROUTE_TRANSIT_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_segmentroute_transit_type_t;

/**
 * @brief Attribute data for SAI Segment Route Transit / Origination Entry
 */
typedef enum _sai_segmentroute_transit_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SEGMENTROUTE_TRANSIT_ATTR_START = 0x00000000,

    /**
     * @brief Transit or Origination Type
     *
     * @type sai_segmentroute_transit_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_SEGMENTROUTE_TRANSIT_ATTR_TRANSIT_TYPE = SAI_SEGMENTROUTE_TRANSIT_ATTR_START,

    /**
     * @brief Number of Segments supported on device for origination
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_SEGMENTROUTE_TRANSIT_ATTR_SUPPORTED_NUM_SEGMENTS,

    /**
     * @brief List of TLV types supported on device for origination
     *
     * @type sai_s32_list_t sai_tlv_type_t
     * @flags READ_ONLY
     */
    SAI_SEGMENTROUTE_TRANSIT_ATTR_SUPPORTED_TLV_TYPE,

    /**
     * @brief List of TLVs for origination
     *
     * @type sai_tlv_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SEGMENTROUTE_TRANSIT_ATTR_TLV_LIST,

    /**
     * @brief List of Segments to Originate
     *
     * @type sai_segment_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SEGMENTROUTE_TRANSIT_ATTR_SEGMENT_LIST,

    /**
     * @brief End of attributes
     */
    SAI_SEGMENTROUTE_TRANSIT_ATTR_END,

    /** Custom range base value */
    SAI_SEGMENTROUTE_TRANSIT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SEGMENTROUTE_TRANSIT_ATTR_CUSTOM_RANGE_END
} sai_segmentroute_transit_attr_t;

/**
 * @brief Endpoint / Local Segment ID entry
 */
typedef struct _sai_segmentroute_endpoint_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Virtual Router ID
     *
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     */
    sai_object_id_t vr_id;

    /**
     * @brief IPv6 Segment ID
     */
    sai_ip_prefix_t segment_id;

} sai_segmentroute_endpoint_entry_t;

/**
 * @brief Create Origination / Transit Policy
 *
 * @param[out] policy_id Policy ID
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_segmentroute_transit_fn)(
        _Out_ sai_object_id_t *policy_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Origination / Transit Policy
 *
 * @param[in] policy_id Policy ID
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_segmentroute_transit_fn)(
        _In_ sai_object_id_t policy_id);

/**
 * @brief Set Origination / Transit Policy attribute value
 *
 * @param[in] policy_id Policy ID
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_segmentroute_transit_attribute_fn)(
        _In_ sai_object_id_t policy_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Origination / Transit Policy attribute value
 *
 * @param[in] policy_id Policy ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_segmentroute_transit_attribute_fn)(
        _In_ sai_object_id_t policy_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create Endpoint / Local Segment ID Entry
 *
 * @param[in] endpoint_entry Endpoint entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_segmentroute_endpoint_entry_fn)(
        _In_ const sai_segmentroute_endpoint_entry_t *endpoint_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Create Endpoint / Local Segment ID Entry
 *
 * @param[in] endpoint_entry Endpoint entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_segmentroute_endpoint_entry_fn)(
        _In_ const sai_segmentroute_endpoint_entry_t *endpoint_entry);

/**
 * @brief Set Endpoint / Local Segment ID attribute value
 *
 * @param[in] endpoint_entry Endpoint entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_segmentroute_endpoint_entry_attribute_fn)(
        _In_ const sai_segmentroute_endpoint_entry_t *endpoint_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Endpoint / Local Segment ID attribute value
 *
 * @param[in] endpoint_entry Endpoint entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_segmentroute_endpoint_entry_attribute_fn)(
        _In_ const sai_segmentroute_endpoint_entry_t *endpoint_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Segment Route methods table retrieved with sai_api_query()
 */
typedef struct _sai_segmentroute_api_t
{
    sai_create_segmentroute_transit_fn create_segmentroute_transit;
    sai_remove_segmentroute_transit_fn remove_segmentroute_transit;
    sai_set_segmentroute_transit_attribute_fn set_segmentroute_transit_attribute;
    sai_get_segmentroute_transit_attribute_fn get_segmentroute_transit_attribute;
    sai_create_segmentroute_endpoint_entry_fn create_segmentroute_endpoint_entry;
    sai_remove_segmentroute_endpoint_entry_fn remove_segmentroute_endpoint_entry;
    sai_set_segmentroute_endpoint_entry_attribute_fn set_segmentroute_endpoint_entry_attribute;
    sai_get_segmentroute_endpoint_entry_attribute_fn get_segmentroute_endpoint_entry_attribute;
} sai_segmentroute_api_t;

/**
 * @}
 */
#endif /** __SAISEGMENTROUTE_H_ */
