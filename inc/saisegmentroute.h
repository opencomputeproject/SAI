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
 * @brief Enum defining Head-end Behavior
 */
typedef enum _sai_segmentroute_sidlist_type_t
{
    /** Insertion of Segment Route Policy */
    SAI_SEGMENTROUTE_SIDLIST_TYPE_INSERT,

    /** Insertion of Segment Route Policy with Reduced SRH */
    SAI_SEGMENTROUTE_SIDLIST_TYPE_INSERT_RED,

    /** Encapsulation in a Segment Route Policy */
    SAI_SEGMENTROUTE_SIDLIST_TYPE_ENCAPS,

    /** Encapsulation in a Segment Route Policy with Reduced SRH */
    SAI_SEGMENTROUTE_SIDLIST_TYPE_ENCAPS_RED,

    /** Custom range base value */
    SAI_SEGMENTROUTE_SIDLIST_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_segmentroute_sidlist_type_t;

/**
 * @brief Enum defining Endpoint Action types
 */
typedef enum _sai_local_sid_entry_endpoint_type_t
{
    /** Basic Endpoint */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_E,

    /** End.X Endpoint with Layer-3 Cross-connect */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_X,

    /** End.T Endpoint with specific IPv6 Table */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_T,

    /** Endpoint with decapsulation and IPv6 Cross-connect */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DX6,

    /** Endpoint with decapsulation and IPv4 Cross-connect */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DX4,

    /** Endpoint with decapsulation and specific IPv6 table lookup */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DT6,

    /** Endpoint with decapsulation and specific IPv6 table lookup */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DT4,

    /** Endpoint with decapsulation and specific IP table lookup */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DT46,

    /** Endpoint Bound to a policy with Encapsulation */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_B6_ENCAPS,

    /** End.B6.Encaps function with a reduced SRH */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_B6_ENCAPS_RED,

    /** Endpoint Bound to a policy with Insertion */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_B6_INSERT,

    /** End.B6.Insert function with a reduced SRH */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_B6_INSERT_RED,

    /** Custom range base value */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_CUSTOM_RANGE_START = 0x10000000,

    /** End of Custom range base */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_CUSTOM_RANGE_END

} sai_local_sid_entry_endpoint_type_t;

/**
 * @brief Enum defining Endpoint Segment flavors for End, End.X and End.T functions
 */
typedef enum _sai_local_sid_entry_endpoint_flavor_t
{
    /** Penultimate segment pop */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_FLAVOR_PSP,

    /** Ultimate Segment pop */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_FLAVOR_USP,

    /** Ultimate Segment decapsulation */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_FLAVOR_USD,

    /** PSP and USP */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_FLAVOR_PSP_AND_USP,

    /** USD and USP */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_FLAVOR_USD_AND_USP,

    /** PSP and USD */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_FLAVOR_PSP_AND_USD,

    /** PSP, USP and USD */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_FLAVOR_PSP_AND_USP_AND_USD,

    /** None */
    SAI_LOCAL_SID_ENTRY_ENDPOINT_FLAVOR_NONE

} sai_local_sid_entry_endpoint_flavor_t;

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
 * @brief Attribute list for Local SID
 */
typedef enum _sai_local_sid_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_LOCAL_SID_ENTRY_ATTR_START,

    /**
     * @brief Endpoint Function
     *
     * @type sai_local_sid_entry_endpoint_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE = SAI_LOCAL_SID_ENTRY_ATTR_START,

    /**
     * @brief Flavor for End, End.X and End.T functions
     *
     * @type sai_local_sid_entry_endpoint_flavor_t
     * @flags CREATE_AND_SET
     * @default SAI_LOCAL_SID_ENTRY_ENDPOINT_FLAVOR_PSP_AND_USD
     * @validonly SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_E or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_X or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_T
     */
    SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_FLAVOR,

    /**
     * @brief Packet action
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_LOCAL_SID_ENTRY_ATTR_PACKET_ACTION,

    /**
     * @brief Packet priority for trap/log actions
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_LOCAL_SID_ENTRY_ATTR_TRAP_PRIORITY,

    /**
     * @brief Next hop for cross-connect functions
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NEXT_HOP, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_X or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DX4 or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DX6 or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_B6_ENCAPS or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_B6_ENCAPS_RED or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_B6_INSERT or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_B6_INSERT_RED
     */
    SAI_LOCAL_SID_ENTRY_ATTR_NEXT_HOP_ID,

    /**
     * @brief Tunnel id for decapsulation
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TUNNEL
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DT4 or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DT6 or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DT46
     */
    SAI_LOCAL_SID_ENTRY_ATTR_TUNNEL_ID,

    /**
     * @brief VRF for decapsulation and specific table lookup functions
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_T or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DT4 or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DT6 or SAI_LOCAL_SID_ENTRY_ATTR_ENDPOINT_TYPE == SAI_LOCAL_SID_ENTRY_ENDPOINT_TYPE_DT46
     */
    SAI_LOCAL_SID_ENTRY_ATTR_VRF,

    /**
     * @brief Attach a counter
     *
     * When it is empty, then packet hits won't be counted
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_LOCAL_SID_ENTRY_ATTR_COUNTER_ID,

    /**
     * @brief End of attributes
     */
    SAI_LOCAL_SID_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_LOCAL_SID_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_LOCAL_SID_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_local_sid_entry_attr_t;

/**
 * @brief Local SID Entry
 */
typedef struct _sai_local_sid_entry_t
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
     * @brief Length of the Locator Block part of the SID
     */
    sai_uint8_t locator_block_len;

    /**
     * @brief Length of the Locator Node part of the SID
     */
    sai_uint8_t locator_node_len;

    /**
     * @brief Length of the Function part of the SID
     */
    sai_uint8_t function_len;

    /**
     * @brief Length of the Args part of the SID
     */
    sai_uint8_t args_len;

    /**
     * @brief IPv6 prefix for Local SID
     */
    sai_ip6_t sid;

} sai_local_sid_entry_t;

/**
 * @brief Create Local SID entry
 *
 * @param[in] local_sid_entry Local SID entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_local_sid_entry_fn)(
        _In_ const sai_local_sid_entry_t *local_sid_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Local SID entry
 *
 * @param[in] local_sid_entry Local SID entry
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_local_sid_entry_fn)(
        _In_ const sai_local_sid_entry_t *local_sid_entry);

/**
 * @brief Set Local SID attribute value
 *
 * @param[in] local_sid_entry Local SID entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_local_sid_entry_attribute_fn)(
        _In_ const sai_local_sid_entry_t *local_sid_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Local SID attribute value
 *
 * @param[in] local_sid_entry Local SID entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_local_sid_entry_attribute_fn)(
        _In_ const sai_local_sid_entry_t *local_sid_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create Local SID entries
 *
 * @param[in] object_count Number of objects to create
 * @param[in] local_sid_entry List of object to create
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to create.
 * @param[in] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are created or
 * #SAI_STATUS_FAILURE when any of the objects fails to create. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_create_local_sid_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_local_sid_entry_t *local_sid_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove Local SID entries
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] local_sid_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_local_sid_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_local_sid_entry_t *local_sid_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk set attribute on Local SID entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] local_sid_entry List of objects to set attribute
 * @param[in] attr_list List of attributes to set on objects, one attribute per object
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_set_local_sid_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_local_sid_entry_t *local_sid_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk get attribute on Local SID entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] local_sid_entry List of objects to set attribute
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get
 * @param[inout] attr_list List of attributes to set on objects, one attribute per object
 * @param[in] mode Bulk operation error handling mode
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_local_sid_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_local_sid_entry_t *local_sid_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

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

    sai_create_local_sid_entry_fn             create_local_sid_entry;
    sai_remove_local_sid_entry_fn             remove_local_sid_entry;
    sai_set_local_sid_entry_attribute_fn      set_local_sid_entry_attribute;
    sai_get_local_sid_entry_attribute_fn      get_local_sid_entry_attribute;

    sai_bulk_create_local_sid_entry_fn        create_local_sid_entries;
    sai_bulk_remove_local_sid_entry_fn        remove_local_sid_entries;
    sai_bulk_set_local_sid_entry_attribute_fn set_local_sid_entries_attribute;
    sai_bulk_get_local_sid_entry_attribute_fn get_local_sid_entries_attribute;

} sai_segmentroute_api_t;

/**
 * @}
 */
#endif /** __SAISEGMENTROUTE_H_ */
