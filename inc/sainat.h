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
 * @file    sainat.h
 *
 * @brief   This module defines SAI NAT (Network Address Translation) spec
 */

#if !defined (__SAINAT_H_)
#define __SAINAT_H_

#include <saitypes.h>

/**
 * @defgroup SAINAT SAI - Network Address Translation (NAT) specific API definitions
 *
 * @{
 */

/**
 * @brief NAT Type
 */
typedef enum _sai_nat_type_t
{
    /** No NAT */
    SAI_NAT_TYPE_NONE,

    /** Source NAT */
    SAI_NAT_TYPE_SOURCE_NAT,

    /** Destination NAT */
    SAI_NAT_TYPE_DESTINATION_NAT,

    /** Double NAT */
    SAI_NAT_TYPE_DOUBLE_NAT,

    /** Destination NAT Pool */
    SAI_NAT_TYPE_DESTINATION_NAT_POOL,

} sai_nat_type_t;

/**
 * @brief NAT Entry Attributes for Match
 */
typedef enum _sai_nat_entry_attr_t
{

    /**
     * @brief Start of Attributes
     */
    SAI_NAT_ENTRY_ATTR_START,

    /**
     * @brief NAT Type defined in sai_nat_type_t
     *
     * @type sai_nat_type_t
     * @flags CREATE_AND_SET
     * @default SAI_NAT_TYPE_NONE
     */
    SAI_NAT_ENTRY_ATTR_NAT_TYPE  = SAI_NAT_ENTRY_ATTR_START,

    /**
     * @brief Replace source IPv4 address in packet.
     * NAT actions will be
     *    (source/destination/both is identified by type of NAT)
     *    - replace IP address
     *    - replace layer 4 source port
     *    - replace layer 4 destination port
     *
     * @type sai_ip4_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_NAT_ENTRY_ATTR_NAT_TYPE == SAI_NAT_TYPE_SOURCE_NAT or SAI_NAT_ENTRY_ATTR_NAT_TYPE == SAI_NAT_TYPE_DOUBLE_NAT
     */
    SAI_NAT_ENTRY_ATTR_SRC_IP,

    /**
     * @brief Mask for source IPv4 address in packet.
     *
     * @type sai_ip4_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_NAT_ENTRY_ATTR_NAT_TYPE == SAI_NAT_TYPE_SOURCE_NAT or SAI_NAT_ENTRY_ATTR_NAT_TYPE == SAI_NAT_TYPE_DOUBLE_NAT
     */
    SAI_NAT_ENTRY_ATTR_SRC_IP_MASK,

    /**
     * @brief Replace virtual router id associate with DST_IP
     * NAT actions will be
     *    (source/destination/both is identified by type of NAT)
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_NAT_ENTRY_ATTR_NAT_TYPE == SAI_NAT_TYPE_DESTINATION_NAT or SAI_NAT_ENTRY_ATTR_NAT_TYPE == SAI_NAT_TYPE_DOUBLE_NAT
     */
    SAI_NAT_ENTRY_ATTR_VR_ID,

    /**
     * @brief Replace destination IPv4 address in packet.
     * NAT actions will be
     *    (source/destination/both is identified by type of NAT)
     *
     * @type sai_ip4_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_NAT_ENTRY_ATTR_NAT_TYPE == SAI_NAT_TYPE_DESTINATION_NAT or SAI_NAT_ENTRY_ATTR_NAT_TYPE == SAI_NAT_TYPE_DOUBLE_NAT
     */
    SAI_NAT_ENTRY_ATTR_DST_IP,

    /**
     * @brief Mask for destination IPv4 address in packet.
     *
     * @type sai_ip4_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_NAT_ENTRY_ATTR_NAT_TYPE == SAI_NAT_TYPE_DESTINATION_NAT or SAI_NAT_ENTRY_ATTR_NAT_TYPE == SAI_NAT_TYPE_DOUBLE_NAT
     */
    SAI_NAT_ENTRY_ATTR_DST_IP_MASK,

    /**
     * @brief Replace L4 source port in packet.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_NAT_ENTRY_ATTR_L4_SRC_PORT,

    /**
     * @brief Replace L4 destination port in packet.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_NAT_ENTRY_ATTR_L4_DST_PORT,

    /**
     * @brief Enable/disable packet count
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_NAT_ENTRY_ATTR_ENABLE_PACKET_COUNT,

    /**
     * @brief Per NAT entry packet count
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_NAT_ENTRY_ATTR_PACKET_COUNT,

    /**
     * @brief Enable/disable byte count
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_NAT_ENTRY_ATTR_ENABLE_BYTE_COUNT,

    /**
     * @brief Per NAT entry byte count
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_NAT_ENTRY_ATTR_BYTE_COUNT,

    /**
     * @brief NAT entry hit bit clear on read flag
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_NAT_ENTRY_ATTR_HIT_BIT_COR,

    /**
     * @brief Per NAT entry hit bit state
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_NAT_ENTRY_ATTR_HIT_BIT,

    /**
     * @brief End of NAT Entry attributes
     */
    SAI_NAT_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_NAT_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_NAT_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_nat_entry_attr_t;

/**
 * @brief NAT entry keys
 * API can be invoked with extra keys present.
 * Driver MUST pick the right set of keys for a
 * given NAT type.
 */
typedef struct _sai_nat_entry_key_t
{

    /**
     * @brief IPv4 source address
     */
    sai_ip4_t src_ip;

    /**
     * @brief IPv4 destination address
     */
    sai_ip4_t dst_ip;

    /**
     * @brief IP protocol value
     */
    sai_uint8_t proto;

    /**
     * @brief IP layer 4 source port
     */
    sai_uint16_t l4_src_port;

    /**
     * @brief IP layer 4 destination port
     */
    sai_uint16_t l4_dst_port;

} sai_nat_entry_key_t;

/**
 * @brief NAT entry key masks
 */
typedef struct _sai_nat_entry_mask_t
{

    /**
     * @brief IPv4 source address mask
     */
    sai_ip4_t src_ip;

    /**
     * @brief IPv4 destination address mask
     */
    sai_ip4_t dst_ip;

    /**
     * @brief IP protocol mask
     */
    sai_uint8_t proto;

    /**
     * @brief IP layer 4 source port mask
     */
    sai_uint16_t l4_src_port;

    /**
     * @brief IP layer 4 destination port mask
     */
    sai_uint16_t l4_dst_port;

} sai_nat_entry_mask_t;

typedef struct _sai_nat_entry_data_t
{

    /**
     * @brief NAT entry keys
     */
    sai_nat_entry_key_t key;

    /**
     * @brief NAT entry keys
     */
    sai_nat_entry_mask_t mask;

} sai_nat_entry_data_t;

/**
 * @brief NAT entry
 */
typedef struct _sai_nat_entry_t
{

    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Virtual Router
     *
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     */
    sai_object_id_t vr_id;

    /**
     * @brief NAT entry type
     */
    sai_nat_type_t nat_type;

    /**
     * @brief NAT entry data
     */
    sai_nat_entry_data_t data;

} sai_nat_entry_t;

/**
 * @brief Create and return a NAT object
 *
 * @param[in] nat_entry NAT entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_nat_entry_fn)(
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove NAT entry
 *
 * @param[in] nat_entry NAT entry to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_nat_entry_fn)(
        _In_ const sai_nat_entry_t *nat_entry);

/**
 * @brief Set NAT entry attribute value(s).
 *
 * @param[in] nat_entry NAT entry
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_nat_entry_attribute_fn)(
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified NAT entry attributes.
 *
 * @param[in] nat_entry NAT entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_nat_entry_attribute_fn)(
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create NAT entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] nat_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_nat_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove NAT entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] nat_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_nat_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk set attribute on NAT entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] nat_entry List of objects to set attribute
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
typedef sai_status_t (*sai_bulk_set_nat_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk get attribute on NAT entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] nat_entry List of objects to set attribute
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
typedef sai_status_t (*sai_bulk_get_nat_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief NAT zone counters for each NAT type
 */
typedef enum _sai_nat_zone_counter_attr_t
{

    /**
     * @brief Start of Attributes
     */
    SAI_NAT_ZONE_COUNTER_ATTR_START,

    /**
     * @brief NAT Type defined in sai_nat_type_t
     * @type sai_nat_type_t
     * @flags CREATE_AND_SET
     * @default SAI_NAT_TYPE_NONE
     */
    SAI_NAT_ZONE_COUNTER_ATTR_NAT_TYPE  = SAI_NAT_ZONE_COUNTER_ATTR_START,

    /**
     * @brief NAT Zone ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_NAT_ZONE_COUNTER_ATTR_ZONE_ID,

    /**
     * @brief Enable/disable discard count
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_NAT_ZONE_COUNTER_ATTR_ENABLE_DISCARD,

    /**
     * @brief Discard packet count
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_NAT_ZONE_COUNTER_ATTR_DISCARD_PACKET_COUNT,

    /**
     * @brief Enable/disable translation needed count
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_NAT_ZONE_COUNTER_ATTR_ENABLE_TRANSLATION_NEEDED,

    /**
     * @brief Translation needed packet count
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_NAT_ZONE_COUNTER_ATTR_TRANSLATION_NEEDED_PACKET_COUNT,

    /**
     * @brief Enable/disable translations count
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_NAT_ZONE_COUNTER_ATTR_ENABLE_TRANSLATIONS,

    /**
     * @brief Translations performed packet count
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_NAT_ZONE_COUNTER_ATTR_TRANSLATIONS_PACKET_COUNT,

    /**
     * @brief End of Attributes
     */
    SAI_NAT_ZONE_COUNTER_ATTR_END,

    /** Custom range base value */
    SAI_NAT_ZONE_COUNTER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_NAT_ZONE_COUNTER_ATTR_CUSTOM_RANGE_END

} sai_nat_zone_counter_attr_t;

/**
 * @brief Create and return a NAT zone counter object
 *
 * @param[out] nat_zone_counter_id NAT counter object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_nat_zone_counter_fn)(
        _Out_ sai_object_id_t *nat_zone_counter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified NAT zone_counter object.
 *
 * Deleting a NAT counter object does not delete reference to it.
 *
 * @param[in] nat_zone_counter_id NAT object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_nat_zone_counter_fn)(
        _In_ sai_object_id_t nat_zone_counter_id);

/**
 * @brief Set NAT zone counter attribute value(s).
 *
 * @param[in] nat_zone_counter_id NAT zone counter id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_nat_zone_counter_attribute_fn)(
        _In_ sai_object_id_t nat_zone_counter_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified NAT zone counter attributes.
 *
 * @param[in] nat_zone_counter_id NAT counter zone object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_nat_zone_counter_attribute_fn)(
        _In_ sai_object_id_t nat_zone_counter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief NAT API Router entry methods table retrieved with sai_api_query()
 */
typedef struct _sai_nat_api_t
{

    /**
     * @brief SAI NAT API set
     */
    sai_create_nat_entry_fn                   create_nat_entry;
    sai_remove_nat_entry_fn                   remove_nat_entry;
    sai_set_nat_entry_attribute_fn            set_nat_entry_attribute;
    sai_get_nat_entry_attribute_fn            get_nat_entry_attribute;

    sai_bulk_create_nat_entry_fn              create_nat_entries;
    sai_bulk_remove_nat_entry_fn              remove_nat_entries;
    sai_bulk_set_nat_entry_attribute_fn       set_nat_entries_attribute;
    sai_bulk_get_nat_entry_attribute_fn       get_nat_entries_attribute;

    sai_create_nat_zone_counter_fn            create_nat_zone_counter;
    sai_remove_nat_zone_counter_fn            remove_nat_zone_counter;
    sai_set_nat_zone_counter_attribute_fn     set_nat_zone_counter_attribute;
    sai_get_nat_zone_counter_attribute_fn     get_nat_zone_counter_attribute;
} sai_nat_api_t;

/**
 * @}
 */
#endif /** __SAINAT_H_ */
