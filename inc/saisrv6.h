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
 * @file    saisrv6.h
 *
 * @brief   This module defines SAI SRV6 Entry interface
 */

#if !defined (__SAISRV6_H_)
#define __SAISRV6_H_

#include <saitypes.h>

/**
 * @defgroup SAISRV6 SAI - SRV6 specific API definitions
 *
 * @{
 */

/**
 * @brief Enum defining Head-end Behavior
 */
typedef enum _sai_srv6_sidlist_type_t
{
    /** Insertion of SRV6 Policy */
    SAI_SRV6_SIDLIST_TYPE_INSERT,

    /** Insertion of SRV6 Policy with Reduced SRH */
    SAI_SRV6_SIDLIST_TYPE_INSERT_RED,

    /** Encapsulation in a SRV6 Policy */
    SAI_SRV6_SIDLIST_TYPE_ENCAPS,

    /** Encapsulation in a SRV6 Policy with Reduced SRH */
    SAI_SRV6_SIDLIST_TYPE_ENCAPS_RED,

    /** Custom range base value */
    SAI_SRV6_SIDLIST_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_srv6_sidlist_type_t;

/**
 * @brief Enum defining Endpoint Behavior
 */
typedef enum _sai_my_sid_entry_endpoint_behavior_t
{
    /** Basic Endpoint */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_E,

    /** End.X Endpoint with Layer-3 Cross-connect */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_X,

    /** End.T Endpoint with specific IPv6 Table */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_T,

    /** Endpoint with decapsulation and IPv6 Cross-connect */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DX6,

    /** Endpoint with decapsulation and IPv4 Cross-connect */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DX4,

    /** Endpoint with decapsulation and specific IPv6 table lookup */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT6,

    /** Endpoint with decapsulation and specific IPv6 table lookup */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT4,

    /** Endpoint with decapsulation and specific IP table lookup */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT46,

    /** Endpoint Bound to a policy with Encapsulation */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_B6_ENCAPS,

    /** End.B6.Encaps function with a reduced SRH */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_B6_ENCAPS_RED,

    /** Endpoint Bound to a policy with Insertion */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_B6_INSERT,

    /** End.B6.Insert function with a reduced SRH */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_B6_INSERT_RED,

    /** End.uN function for shift-and-lookup behavior */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_UN,

    /** End.uA function for shift-and-xconnect behavior */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_UA,

    /** Custom range base value */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_CUSTOM_RANGE_START = 0x10000000,

    /** End of Custom range base */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_CUSTOM_RANGE_END

} sai_my_sid_entry_endpoint_behavior_t;

/**
 * @brief Enum defining Endpoint Behavior flavors for End, End.X and End.T functions
 */
typedef enum _sai_my_sid_entry_endpoint_behavior_flavor_t
{
    /** None */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_NONE,

    /** Penultimate segment pop */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP,

    /** Ultimate Segment pop */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_USP,

    /** Ultimate Segment decapsulation */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_USD,

    /** PSP and USP */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP_AND_USP,

    /** USD and USP */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_USD_AND_USP,

    /** PSP and USD */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP_AND_USD,

    /** PSP, USP and USD */
    SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_PSP_AND_USP_AND_USD

} sai_my_sid_entry_endpoint_behavior_flavor_t;

/**
 * @brief Attribute data for SRV6 SID List Entry
 */
typedef enum _sai_srv6_sidlist_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SRV6_SIDLIST_ATTR_START = 0x00000000,

    /**
     * @brief Transit or Source Type
     *
     * @type sai_srv6_sidlist_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_SRV6_SIDLIST_ATTR_TYPE = SAI_SRV6_SIDLIST_ATTR_START,

    /**
     * @brief List of Type Length Values for Source
     *
     * @type sai_tlv_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SRV6_SIDLIST_ATTR_TLV_LIST,

    /**
     * @brief List of Segments for Source / Transit
     *
     * @type sai_segment_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SRV6_SIDLIST_ATTR_SEGMENT_LIST,

    /**
     * @brief Underlay Next hop to forward packets to this SID List
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NEXT_HOP 
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SRV6_SIDLIST_ATTR_NEXT_HOP_ID,

    /**
     * @brief End of attributes
     */
    SAI_SRV6_SIDLIST_ATTR_END,

    /** Custom range base value */
    SAI_SRV6_SIDLIST_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SRV6_SIDLIST_ATTR_CUSTOM_RANGE_END
} sai_srv6_sidlist_attr_t;

/**
 * @brief SRV6 SID List counter IDs
 */
typedef enum _sai_srv6_sidlist_stat_t
{
    /** Egress packet stat count */
    SAI_SRV6_SIDLIST_STAT_OUT_PACKETS,

    /** Egress byte stat count */
    SAI_SRV6_SIDLIST_STAT_OUT_OCTETS,
} sai_srv6_sidlist_stat_t;

/**
 * @brief Create Segment ID List
 *
 * @param[out] srv6_sidlist_id Segment ID List ID
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_srv6_sidlist_fn)(
        _Out_ sai_object_id_t *srv6_sidlist_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Segment ID List
 *
 * @param[in] srv6_sidlist_id Segment ID List ID
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_srv6_sidlist_fn)(
        _In_ sai_object_id_t srv6_sidlist_id);

/**
 * @brief Set Segment ID List attribute value
 *
 * @param[in] srv6_sidlist_id Segment ID List ID
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_srv6_sidlist_attribute_fn)(
        _In_ sai_object_id_t srv6_sidlist_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Segment ID List attribute value
 *
 * @param[in] srv6_sidlist_id Segment ID List ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_srv6_sidlist_attribute_fn)(
        _In_ sai_object_id_t srv6_sidlist_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get SRV6 SID List statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] srv6_sidlist_id SRV6 SID List id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_srv6_sidlist_stats_fn)(
        _In_ sai_object_id_t srv6_sidlist_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get SRV6 SID List statistics counters extended.
 *
 * @param[in] srv6_sidlist_id SRV6 SID List id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_srv6_sidlist_stats_ext_fn)(
        _In_ sai_object_id_t srv6_sidlist_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear SRV6 SID List statistics counters.
 *
 * @param[in] srv6_sidlist_id SRV6 SID List id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_srv6_sidlist_stats_fn)(
        _In_ sai_object_id_t srv6_sidlist_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Attribute list for My SID
 */
typedef enum _sai_my_sid_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_MY_SID_ENTRY_ATTR_START,

    /**
     * @brief Endpoint Function
     *
     * @type sai_my_sid_entry_endpoint_behavior_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR = SAI_MY_SID_ENTRY_ATTR_START,

    /**
     * @brief Flavor for End, End.X and End.T functions
     *
     * @type sai_my_sid_entry_endpoint_behavior_flavor_t
     * @flags CREATE_AND_SET
     * @default SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_FLAVOR_NONE
     * @validonly SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_E or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_X or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_T
     */
    SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR_FLAVOR,

    /**
     * @brief Packet action
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_MY_SID_ENTRY_ATTR_PACKET_ACTION,

    /**
     * @brief Packet priority for trap/log actions
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_MY_SID_ENTRY_ATTR_TRAP_PRIORITY,

    /**
     * @brief Next hop for cross-connect functions
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NEXT_HOP, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_X or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DX4 or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DX6 or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_B6_ENCAPS or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_B6_ENCAPS_RED or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_B6_INSERT or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_B6_INSERT_RED
     */
    SAI_MY_SID_ENTRY_ATTR_NEXT_HOP_ID,

    /**
     * @brief Tunnel id for decapsulation
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TUNNEL
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT4 or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT6 or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT46
     */
    SAI_MY_SID_ENTRY_ATTR_TUNNEL_ID,

    /**
     * @brief VRF for decapsulation and specific table lookup functions
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_T or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT4 or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT6 or SAI_MY_SID_ENTRY_ATTR_ENDPOINT_BEHAVIOR == SAI_MY_SID_ENTRY_ENDPOINT_BEHAVIOR_DT46
     */
    SAI_MY_SID_ENTRY_ATTR_VRF,

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
    SAI_MY_SID_ENTRY_ATTR_COUNTER_ID,

    /**
     * @brief End of attributes
     */
    SAI_MY_SID_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_MY_SID_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_MY_SID_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_my_sid_entry_attr_t;

/**
 * @brief My SID Entry
 */
typedef struct _sai_my_sid_entry_t
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
     * @brief IPv6 Address for My SID
     */
    sai_ip6_t sid;

} sai_my_sid_entry_t;

/**
 * @brief Create My SID entry
 *
 * @param[in] my_sid_entry My SID entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_my_sid_entry_fn)(
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove My SID entry
 *
 * @param[in] my_sid_entry My SID entry
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_my_sid_entry_fn)(
        _In_ const sai_my_sid_entry_t *my_sid_entry);

/**
 * @brief Set My SID attribute value
 *
 * @param[in] my_sid_entry My SID entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_my_sid_entry_attribute_fn)(
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief My SID attribute value
 *
 * @param[in] my_sid_entry My SID entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_my_sid_entry_attribute_fn)(
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create My SID entries
 *
 * @param[in] object_count Number of objects to create
 * @param[in] my_sid_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_my_sid_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove My SID entries
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] my_sid_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_my_sid_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk set attribute on My SID entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] my_sid_entry List of objects to set attribute
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
typedef sai_status_t (*sai_bulk_set_my_sid_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk get attribute on My SID entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] my_sid_entry List of objects to set attribute
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
typedef sai_status_t (*sai_bulk_get_my_sid_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief SRV6 methods table retrieved with sai_api_query()
 */
typedef struct _sai_srv6_api_t
{
    sai_create_srv6_sidlist_fn             create_srv6_sidlist;
    sai_remove_srv6_sidlist_fn             remove_srv6_sidlist;
    sai_set_srv6_sidlist_attribute_fn      set_srv6_sidlist_attribute;
    sai_get_srv6_sidlist_attribute_fn      get_srv6_sidlist_attribute;
    sai_bulk_object_create_fn              create_srv6_sidlists;
    sai_bulk_object_remove_fn              remove_srv6_sidlists;

    sai_get_srv6_sidlist_stats_fn          get_srv6_sidlist_stats;
    sai_get_srv6_sidlist_stats_ext_fn      get_srv6_sidlist_stats_ext;
    sai_clear_srv6_sidlist_stats_fn        clear_srv6_sidlist_stats;

    sai_create_my_sid_entry_fn             create_my_sid_entry;
    sai_remove_my_sid_entry_fn             remove_my_sid_entry;
    sai_set_my_sid_entry_attribute_fn      set_my_sid_entry_attribute;
    sai_get_my_sid_entry_attribute_fn      get_my_sid_entry_attribute;

    sai_bulk_create_my_sid_entry_fn        create_my_sid_entries;
    sai_bulk_remove_my_sid_entry_fn        remove_my_sid_entries;
    sai_bulk_set_my_sid_entry_attribute_fn set_my_sid_entries_attribute;
    sai_bulk_get_my_sid_entry_attribute_fn get_my_sid_entries_attribute;

} sai_srv6_api_t;

/**
 * @}
 */
#endif /** __SAISRV6_H_ */
