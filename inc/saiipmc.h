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
 * @file    saiipmc.h
 *
 * @brief   This module defines SAI IPMC interface
 */

#if !defined (__SAIIPMC_H_)
#define __SAIIPMC_H_

#include <saitypes.h>

/**
 * @defgroup SAIIPMC SAI - IPMC specific API definitions
 *
 * @{
 */

/**
 * @brief IPMC entry type.
 */
typedef enum _sai_ipmc_entry_type_t
{
    /** IPMC entry with type (S,G) */
    SAI_IPMC_ENTRY_TYPE_SG,

    /** IPMC entry with type (*,G) */
    SAI_IPMC_ENTRY_TYPE_XG,

} sai_ipmc_entry_type_t;

/**
 * @brief IPMC entry key
 */
typedef struct _sai_ipmc_entry_t
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

    /** IPMC entry type */
    sai_ipmc_entry_type_t type;

    /** IP dest address */
    sai_ip_address_t destination;

    /** IP source address */
    sai_ip_address_t source;
} sai_ipmc_entry_t;

/**
 * @brief Attribute Id for ipmc entry
 */
typedef enum _sai_ipmc_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_IPMC_ENTRY_ATTR_START,

    /**
     * @brief IPMC entry type
     *
     * @type sai_packet_action_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_IPMC_ENTRY_ATTR_PACKET_ACTION = SAI_IPMC_ENTRY_ATTR_START,

    /**
     * @brief IPMC entry output group id
     *
     * This attribute only takes effect when ATTR_PACKET_ACTION is set to FORWARD
     * If the group has no member, packets will be discarded
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_IPMC_GROUP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_IPMC_ENTRY_ATTR_PACKET_ACTION == SAI_PACKET_ACTION_FORWARD
     */
    SAI_IPMC_ENTRY_ATTR_OUTPUT_GROUP_ID,

    /**
     * @brief IPMC entry RPF interface group id
     *
     * If not set or the group has no member, RPF checking will be disabled
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_RPF_GROUP
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_IPMC_ENTRY_ATTR_RPF_GROUP_ID,

    /**
     * @brief End of attributes
     */
    SAI_IPMC_ENTRY_ATTR_END,

    /* Custom range base value */
    SAI_IPMC_ENTRY_ATTR_CUSTOM_RANGE_BASE  = 0x10000000,

    /* --*/
    SAI_IPMC_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_ipmc_entry_attr_t;

/**
 * @brief Create IPMC entry
 *
 * @param[in] ipmc_entry IPMC entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_ipmc_entry_fn)(
        _In_ const sai_ipmc_entry_t *ipmc_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove IPMC entry
 *
 * @param[in] ipmc_entry IPMC entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_ipmc_entry_fn)(
        _In_ const sai_ipmc_entry_t *ipmc_entry);

/**
 * @brief Set ipmc entry attribute value
 *
 * @param[in] ipmc_entry IPMC entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_ipmc_entry_attribute_fn)(
        _In_ const sai_ipmc_entry_t *ipmc_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get ipmc entry attribute value
 *
 * @param[in] ipmc_entry IPMC entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_ipmc_entry_attribute_fn)(
        _In_ const sai_ipmc_entry_t *ipmc_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief IPMC method table retrieved with sai_api_query()
 */
typedef struct _sai_ipmc_api_t
{
    sai_create_ipmc_entry_fn                     create_ipmc_entry;
    sai_remove_ipmc_entry_fn                     remove_ipmc_entry;
    sai_set_ipmc_entry_attribute_fn              set_ipmc_entry_attribute;
    sai_get_ipmc_entry_attribute_fn              get_ipmc_entry_attribute;

} sai_ipmc_api_t;

/**
 * @}
 */
#endif /** __SAIIPMC_H_ */
