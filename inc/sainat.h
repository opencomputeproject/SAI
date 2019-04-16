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
 * @brief NAT Entry Attributes
 */
typedef enum _sai_nat_entry_attr_t
{

    /**
     * @brief Start of Attributes
     */
    SAI_NAT_ENTRY_ATTR_START,

    /**
     * @brief NAT IP address translated from
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_NAT_ENTRY_ATTR_FROM_IP = SAI_NAT_ENTRY_ATTR_START,

    /**
     * @brief NAT IP address translated to
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_NAT_ENTRY_ATTR_TO_IP,

    /**
     * @brief NAT Port address translated from
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_NAT_ENTRY_ATTR_FROM_PORT,

    /**
     * @brief NAT Port address translated to
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_NAT_ENTRY_ATTR_TO_PORT,

    /**
     * @brief IP Protocol
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_NAT_ENTRY_ATTR_IP_PROTOCOL,

    /**
     * @brief VRF ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_NAT_ENTRY_ATTR_FIELD_VRF_ID,

    /**
     * @brief Enable/disable packet count
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_NAT_ENTRY_ATTR_ENABLE_PACKET_COUNT,

    /**
     * @brief Enable/disable byte count
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_NAT_ENTRY_ATTR_ENABLE_BYTE_COUNT,

    /**
     * @brief End of Attributes
     */
    SAI_NAT_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_NAT_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_NAT_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_nat_entry_attr_t;

/**
 * @brief Create and return a NAT object
 *
 * @param[out] nat_entry_id NAT pool object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_nat_entry_fn)(
        _Out_ sai_object_id_t *nat_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified NAT entry object.
 *
 * Deleting a NAT entry object does not delete reference to it.
 *
 * @param[in] nat_entry_id NAT object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_nat_entry_fn)(
        _In_ sai_object_id_t nat_entry_id);

/**
 * @brief Set NAT entry attribute value(s).
 *
 * @param[in] nat_entry_id NAT entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_nat_entry_attribute_fn)(
        _In_ sai_object_id_t nat_entry_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified NAT entry attributes.
 *
 * @param[in] nat_entry_id NAT entry object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_nat_entry_attribute_fn)(
        _In_ sai_object_id_t nat_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

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

} sai_nat_type_t;

/**
 * @brief NAT Counter Attributes
 */
typedef enum _sai_nat_counter_attr_t
{

    /**
     * @brief Start of Attributes
     */
    SAI_NAT_COUNTER_ATTR_START,

    /**
     * @brief NAT Type defined in sai_nat_type_t
     * @type sai_nat_type_t
     * @flags CREATE_AND_SET
     * @default SAI_NAT_TYPE_NONE
     */
    SAI_NAT_COUNTER_ATTR_NAT_TYPE  = SAI_NAT_COUNTER_ATTR_START,

    /**
     * @brief NAT Zone ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_NAT_COUNTER_ATTR_ZONE_ID,

    /**
     * @brief Enable/disable discard count
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_NAT_COUNTER_ATTR_ENABLE_DISCARD,

    /**
     * @brief Enable/disable translation needed count
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_NAT_COUNTER_ATTR_ENABLE_TRANSLATION_NEEDED,

    /**
     * @brief Enable/disable translations count
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_NAT_COUNTER_ATTR_ENABLE_TRANSLATIONS,

    /**
     * @brief End of Attributes
     */
    SAI_NAT_COUNTER_ATTR_END,

    /** Custom range base value */
    SAI_NAT_COUNTER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_NAT_COUNTER_ATTR_CUSTOM_RANGE_END

} sai_nat_counter_attr_t;

/**
 * @brief Create and return a NAT counter object
 *
 * @param[out] nat_counter_id NAT counter object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_nat_counter_fn)(
        _Out_ sai_object_id_t *nat_counter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified NAT counter object.
 *
 * Deleting a NAT counter object does not delete reference to it.
 *
 * @param[in] nat_counter_id NAT object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_nat_counter_fn)(
        _In_ sai_object_id_t nat_counter_id);

/**
 * @brief Set NAT counter attribute value(s).
 *
 * @param[in] nat_counter_id NAT counter id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_nat_counter_attribute_fn)(
        _In_ sai_object_id_t nat_counter_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified NAT counter attributes.
 *
 * @param[in] nat_counter_id NAT counter object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_nat_counter_attribute_fn)(
        _In_ sai_object_id_t nat_counter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief NAT Attributes
 */
typedef enum _sai_nat_attr_t
{

    /**
     * @brief Start of Attributes
     */
    SAI_NAT_ATTR_START,

    /**
     * @brief NAT Type defined in sai_nat_type_t
     * @type sai_nat_type_t
     * @flags CREATE_AND_SET
     * @default SAI_NAT_TYPE_NONE
     */
    SAI_NAT_ATTR_NAT_TYPE  = SAI_NAT_ATTR_START,

    /**
     * @brief NAT Inside Zone ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_NAT_ATTR_FROM_ZONE_ID,

    /**
     * @brief NAT Outside Zone ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_NAT_ATTR_TO_ZONE_ID,

    /**
     * @brief NAT rules in the group
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NAT_ENTRY
     * @default empty
     */
    SAI_NAT_ATTR_NAT_ENTRY_LIST,

    /**
     * @brief NAT counter in the group
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NAT_COUNTER
     * @default empty
     */
    SAI_NAT_ATTR_NAT_COUNTER_LIST,

    /**
     * @brief End of Attributes
     */
    SAI_NAT_ATTR_END,

    /** Custom range base value */
    SAI_NAT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_NAT_ATTR_CUSTOM_RANGE_END

} sai_nat_attr_t;

/**
 * @brief Create and return a NAT object
 *
 * @param[out] nat_id NAT object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_nat_fn)(
        _Out_ sai_object_id_t *nat_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified NAT object.
 *
 * Deleting a NAT object also deletes single specified NAT.
 *
 * @param[in] nat_id NAT object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_nat_fn)(
        _In_ sai_object_id_t nat_id);

/**
 * @brief Set NAT attribute value(s).
 *
 * @param[in] nat_id NAT id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_nat_attribute_fn)(
        _In_ sai_object_id_t nat_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified NAT attributes.
 *
 * @param[in] nat_id NAT object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_nat_attribute_fn)(
        _In_ sai_object_id_t nat_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief SAI NAT API set
 */
typedef struct _sai_nat_api_t
{

    /**
     * @brief SAI NAT API set
     */
    sai_create_nat_fn                              create_nat;
    sai_remove_nat_fn                              remove_nat;
    sai_set_nat_attribute_fn                       set_nat_attribute;
    sai_get_nat_attribute_fn                       get_nat_attribute;

    sai_create_nat_entry_fn                        create_nat_entry;
    sai_remove_nat_entry_fn                        remove_nat_entry;
    sai_set_nat_entry_attribute_fn                 set_nat_entry_attribute;
    sai_get_nat_entry_attribute_fn                 get_nat_entry_attribute;

    sai_create_nat_counter_fn                      create_nat_counter;
    sai_remove_nat_counter_fn                      remove_nat_counter;
    sai_set_nat_counter_attribute_fn               set_nat_counter_attribute;
    sai_get_nat_counter_attribute_fn               get_nat_counter_attribute;
} sai_nat_api_t;

/**
 * @}
 */
#endif /** __SAINAT_H_ */
