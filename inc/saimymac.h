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
 * @file    saimymac.h
 *
 * @brief   This module defines SAI My MAC
 */

#if !defined (__SAIMYMAC_H_)
#define __SAIMYMAC_H_

#include <saitypes.h>

/**
 * @brief My MAC entry attribute IDs
 */
typedef enum _sai_my_mac_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_MY_MAC_ATTR_START,

    /**
     * @brief Priority
     *
     * Value must be in the range defined in
     * \[#SAI_SWITCH_ATTR_MY_MAC_TABLE_MINIMUM_PRIORITY,
     * #SAI_SWITCH_ATTR_MY_MAC_TABLE_MAXIMUM_PRIORITY\]
     * (default = #SAI_SWITCH_ATTR_MY_MAC_TABLE_MINIMUM_PRIORITY)
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_MY_MAC_ATTR_PRIORITY = SAI_MY_MAC_ATTR_START,

    /**
     * @brief Associated Port, LAG object id,
     * if not specified any port will match
     *
     * @type sai_object_id_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_MY_MAC_ATTR_PORT_ID,

    /**
     * @brief Associated Vlan Id,
     * if not specified any vlan id will match
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan true
     * @default 0
     */
    SAI_MY_MAC_ATTR_VLAN_ID,

    /**
     * @brief MAC Address
     *
     * @type sai_mac_t
     * @flags CREATE_ONLY
     * @default vendor
     */
    SAI_MY_MAC_ATTR_MAC_ADDRESS,

    /**
     * @brief MAC Address Mask
     *
     * @type sai_mac_t
     * @flags CREATE_ONLY
     * @default vendor
     */
    SAI_MY_MAC_ATTR_MAC_ADDRESS_MASK,

    /**
     * @brief End of attributes
     */
    SAI_MY_MAC_ATTR_END,

    /** Custom range base value */
    SAI_MY_MAC_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_my_mac_attr_t;

/**
 * @brief Create My MAC entry.
 *
 * @param[out] my_mac_id My MAC id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_my_mac_fn)(
        _Out_ sai_object_id_t *my_mac_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove My MAC entry
 *
 * @param[in] my_mac_id My MAC Id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_my_mac_fn)(
        _In_ sai_object_id_t my_mac_id);

/**
 * @brief Set My MAC entry attribute
 *
 * @param[in] my_mac_id My MAC id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_my_mac_attribute_fn)(
        _In_ sai_object_id_t my_mac_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get My MAC entry attribute
 *
 * @param[in] my_mac_id My MAC id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_my_mac_attribute_fn)(
        _In_ sai_object_id_t my_mac_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief My MAC methods table retrieved with sai_api_query()
 */
typedef struct _sai_my_mac_api_t
{
    sai_create_my_mac_fn                create_my_mac;
    sai_remove_my_mac_fn                remove_my_mac;
    sai_set_my_mac_attribute_fn         set_my_mac_attribute;
    sai_get_my_mac_attribute_fn         get_my_mac_attribute;

} sai_my_mac_api_t;

#endif /** __SAIMYMAC_H_ */
