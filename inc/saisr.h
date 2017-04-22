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
 * @file    saisr.h
 *
 * @brief   This module defines SAI Segment Route Entry interface
 */

#if !defined (__SAISR_H_)
#define __SAISR_H_

#include <saitypes.h>

/**
 * @defgroup SAISR SAI - Segment Route specific API definitions
 *
 * @{
 */

/**
 * @brief Enum defining Endpoint Action types
 */
typedef enum _sai_sr_pe_entry_action_type_t
{
    /** Basic Endpoint */
    SAI_SR_PE_ENTRY_ACTION_TYPE_E,

    /** End.X Endpoint with Layer-3 Cross-connect */
    SAI_SR_PE_ENTRY_ACTION_TYPE_X,

    /** End.T Endpoint with specific IPv6 Table */
    SAI_SR_PE_ENTRY_ACTION_TYPE_T,

    /** Endpoint with decapsulation and Layer 2 Cross-connect */
    SAI_SR_PE_ENTRY_ACTION_TYPE_DX2,

    /** Endpoint with decapsulation and IPv6 Cross-connect */
    SAI_SR_PE_ENTRY_ACTION_TYPE_DX6,

    /** Endpoint with decapsulation and IPv4 Cross-connect */
    SAI_SR_PE_ENTRY_ACTION_TYPE_DX4,

    /** Endpoint with decapsulation and specific IPv6 */
    SAI_SR_PE_ENTRY_ACTION_TYPE_DT6,

    /** Endpoint with decapsulation and specific IPv6 */
    SAI_SR_PE_ENTRY_ACTION_TYPE_DT4,

    /** Custom range base value */
    SAI_SR_PE_ENTRY_ACTION_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_sr_pe_entry_action_type_t;

/**
 * @brief Enum defining Endpoint Segment Pop types for End, End.X and End.T
 */
typedef enum _sai_sr_pe_entry_pop_type_t
{
    /** Penultimate segment pop */
    SAI_SR_PE_ENTRY_POP_TYPE_PSP,

    /** Ultimate Segment pop */
    SAI_SR_PE_ENTRY_POP_TYPE_USP,

} sai_sr_pe_entry_pop_type_t;

/**
 * @brief Attribute Id for sai_sr_pe_entry
 */
typedef enum _sai_sr_pe_entry_attr_t
{
    /**
     * @brief Start of Segment Route Endpoint Entry attributes
     */
    SAI_SR_PE_ENTRY_ATTR_START,

    /**
     * @brief SAI Segment Route Endpoint Action Type
     *
     * @type sai_sr_pe_entry_action_type_t
     * @flags CREATE_AND_SET
     * @default SAI_SR_PE_ENTRY_ACTION_TYPE_E
     */
    SAI_SR_PE_ENTRY_ATTR_ACTION_TYPE = SAI_SR_PE_ENTRY_ATTR_START,

    /**
     * @brief SAI Segment Route Endpoint Pop Type
     *
     * @type sai_sr_pe_entry_pop_type_t
     * @flags CREATE_AND_SET
     * @default SAI_SR_PE_ENTRY_POP_TYPE_USP
     */
    SAI_SR_PE_ENTRY_ATTR_POP_TYPE,

    /**
     * @brief Binding Segment ID for source or transit scenarios
     *
     * When it is SAI_NULL_OBJECT_ID, then packet will be processed as endpoint and
     * Binding Segment ID lookup will be skipped
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_SR_BSID,
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SR_PE_ENTRY_ATTR_BSID,

    /**
     * @brief End of Policy / Endpoint Entry attributes
     */
    SAI_SR_PE_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_SR_PE_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SR_PE_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_sr_pe_entry_attr_t;

/**
 * @brief Attribute id for Binding Segment ID
 */
typedef enum _sai_sr_bsid_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SR_BSID_ATTR_START,

    /**
     * @brief Number of Segment ID Lists in the group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SR_BSID_ATTR_SIDLIST_COUNT = SAI_SR_BSID_ATTR_START,

    /**
     * @brief Segment ID List member list
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_SR_SIDLIST
     */
    SAI_SR_BSID_ATTR_SR_SIDLIST_MEMBER_LIST,

    /**
     * @brief End of attributes
     */
    SAI_SR_BSID_ATTR_END,

    /** Custom range base value */
    SAI_SR_BSID_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SR_BSID_ATTR_CUSTOM_RANGE_END

} sai_sr_bsid_attr_t;

/**
 * @brief Enum defining Transit or Source types
 */
typedef enum _sai_sr_sidlist_type_t
{
    /** Insertion of Segment Route Policy */
    SAI_SR_SIDLIST_TYPE_INSERT,

    /** Encapsulation in a Segment Route Policy and Source */
    SAI_SR_SIDLIST_TYPE_ENCAPS,

    /** Encapsulation in a Segment Route Policy and Ethernet */
    SAI_SR_SIDLIST_TYPE_ENCAPS_L2,

    /** Custom range base value */
    SAI_SR_SIDLIST_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_sr_sidlist_type_t;

/**
 * @brief Attribute data for Segment Route Segment ID List Entry
 */
typedef enum _sai_sr_sidlist_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SR_SIDLIST_ATTR_START = 0x00000000,

    /**
     * @brief Transit or Source Type
     *
     * @type sai_sr_sidlist_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_SR_SIDLIST_ATTR_TYPE = SAI_SR_SIDLIST_ATTR_START,

    /**
     * @brief Binding Segment ID
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_SR_BSID
     */
    SAI_SR_SIDLIST_ATTR_SR_BSID,

    /**
     * @brief Member weights
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1
     */
    SAI_SR_SIDLIST_ATTR_WEIGHT,

    /**
     * @brief Number of Segments supported on device for source
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_SR_SIDLIST_ATTR_SUPPORTED_NUM_SEGMENTS,

    /**
     * @brief List of TLV types supported on device for source
     *
     * @type sai_s32_list_t sai_tlv_type_t
     * @flags READ_ONLY
     */
    SAI_SR_SIDLIST_ATTR_SUPPORTED_TLV_TYPE,

    /**
     * @brief List of TLVs for Source
     *
     * @type sai_tlv_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SR_SIDLIST_ATTR_TLV_LIST,

    /**
     * @brief List of Segments for Source / Transit
     *
     * @type sai_segment_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SR_SIDLIST_ATTR_SEGMENT_LIST,

    /**
     * @brief End of attributes
     */
    SAI_SR_SIDLIST_ATTR_END,

    /** Custom range base value */
    SAI_SR_SIDLIST_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SR_SIDLIST_ATTR_CUSTOM_RANGE_END
} sai_sr_sidlist_attr_t;

/**
 * @brief Policy / Endpoint entry
 */
typedef struct _sai_sr_pe_entry_t
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
     * @brief IPv6 Destination
     */
    sai_ip_address_t destination;

    /**
     * @brief Color to match on policy
     */
    sai_uint32_t policy_color;

} sai_sr_pe_entry_t;

/**
 * @brief Create Binding Segment ID
 *
 * @param[out] bsid_id Binding Segment ID
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_sr_bsid_fn)(
        _Out_ sai_object_id_t *bsid_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Binding Segment ID
 *
 * @param[in] bsid_id Binding Segment ID
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_sr_bsid_fn)(
        _In_ sai_object_id_t bsid_id);

/**
 * @brief Set Binding Segment ID attribute
 *
 * @param[in] bsid_id Binding Segment ID
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_sr_bsid_attribute_fn)(
        _In_ sai_object_id_t bsid_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Binding Segment ID attribute
 *
 * @param[in] bsid_id Binding Segment ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_sr_bsid_attribute_fn)(
        _In_ sai_object_id_t bsid_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create Segment ID List
 *
 * @param[out] sidlist_id Segment ID List ID
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_sr_sidlist_fn)(
        _Out_ sai_object_id_t *sidlist_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Segment ID List
 *
 * @param[in] sidlist_id Segment ID List ID
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_sr_sidlist_fn)(
        _In_ sai_object_id_t sidlist_id);

/**
 * @brief Set Segment ID List attribute value
 *
 * @param[in] sidlist_id Policy ID
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_sr_sidlist_attribute_fn)(
        _In_ sai_object_id_t sidlist_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Segment ID List attribute value
 *
 * @param[in] sidlist_id Segment ID List ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_sr_sidlist_attribute_fn)(
        _In_ sai_object_id_t sidlist_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create Policy / Endpoint Entry
 *
 * @param[in] pe_entry Policy / Endpoint entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_sr_pe_entry_fn)(
        _In_ const sai_sr_pe_entry_t *pe_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Create Policy / Endpoint Entry
 *
 * @param[in] pe_entry Policy / Endpoint entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_sr_pe_entry_fn)(
        _In_ const sai_sr_pe_entry_t *pe_entry);

/**
 * @brief Set Policy / Endpoint attribute value
 *
 * @param[in] pe_entry Policy / Endpoint entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_sr_pe_entry_attribute_fn)(
        _In_ const sai_sr_pe_entry_t *pe_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Policy / Endpoint attribute value
 *
 * @param[in] pe_entry Policy / Endpoint entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_sr_pe_entry_attribute_fn)(
        _In_ const sai_sr_pe_entry_t *pe_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Segment Route methods table retrieved with sai_api_query()
 */
typedef struct _sai_sr_api_t
{
    sai_create_sr_bsid_fn           create_sr_bsid;
    sai_remove_sr_bsid_fn           remove_sr_bsid;
    sai_set_sr_bsid_attribute_fn    set_sr_bsid_attribute;
    sai_get_sr_bsid_attribute_fn    get_sr_bsid_attribute;

    sai_create_sr_sidlist_fn        create_sr_sidlist;
    sai_remove_sr_sidlist_fn        remove_sr_sidlist;
    sai_set_sr_sidlist_attribute_fn set_sr_sidlist_attribute;
    sai_get_sr_sidlist_attribute_fn get_sr_sidlist_attribute;
    sai_bulk_object_create_fn       create_sr_sidlists;
    sai_bulk_object_remove_fn       remove_sr_sidlists;

    sai_create_sr_pe_entry_fn           create_sr_pe_entry;
    sai_remove_sr_pe_entry_fn           remove_sr_pe_entry;
    sai_set_sr_pe_entry_attribute_fn    set_sr_pe_entry_attribute;
    sai_get_sr_pe_entry_attribute_fn    get_sr_pe_entry_attribute;
} sai_sr_api_t;

/**
 * @}
 */
#endif /** __SAISR_H_ */
