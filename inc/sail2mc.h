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
 * @file    sail2mc.h
 *
 * @brief   This module defines SAI L2MC interface
 */

#if !defined (__SAIL2MC_H_)
#define __SAIL2MC_H_

#include <saitypes.h>

/**
 * @defgroup SAIL2MC SAI - L2MC specific API definitions
 *
 * @{
 */

/**
 * @brief L2MC entry type.
 */
typedef enum _sai_l2mc_entry_type_t
{
    /** L2MC entry with type (S,G) */
    SAI_L2MC_ENTRY_TYPE_SG,

    /** L2MC entry with type (*,G) */
    SAI_L2MC_ENTRY_TYPE_XG,

} sai_l2mc_entry_type_t;

/**
 * @brief L2MC entry key
 */
typedef struct _sai_l2mc_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /** Bridge type */
    sai_fdb_entry_bridge_type_t bridge_type;

    /** Vlan ID. Valid for .1Q */
    sai_vlan_id_t vlan_id;

    /**
     * Bridge ID. Valid for .1D
     *
     * @objects SAI_OBJECT_TYPE_BRIDGE
     */
    sai_object_id_t bridge_id;

    /** L2MC entry type */
    sai_l2mc_entry_type_t type;

    /** IP dest address */
    sai_ip_address_t destination;

    /** IP source address */
    sai_ip_address_t source;
} sai_l2mc_entry_t;

/**
 * @brief Attribute Id for l2mc entry
 */
typedef enum _sai_l2mc_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_L2MC_ENTRY_ATTR_START,

    /**
     * @brief L2MC entry type
     *
     * @type sai_packet_action_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_L2MC_ENTRY_ATTR_PACKET_ACTION = SAI_L2MC_ENTRY_ATTR_START,

    /**
     * @brief L2MC entry output group id
     *
     * This attribute only takes effect when ATTR_PACKET_ACTION is set to FORWARD.
     * If the group has no member, packets will be discarded.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_L2MC_ENTRY_ATTR_PACKET_ACTION == SAI_PACKET_ACTION_FORWARD
     */
    SAI_L2MC_ENTRY_ATTR_OUTPUT_GROUP_ID,

    /**
     * @brief End of attributes
     */
    SAI_L2MC_ENTRY_ATTR_END,

    /* Custom range base value */
    SAI_L2MC_ENTRY_ATTR_CUSTOM_RANGE_BASE  = 0x10000000,

    /* --*/
    SAI_L2MC_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_l2mc_entry_attr_t;

/**
 * @brief Create L2MC entry
 *
 * @param[in] l2mc_entry L2MC entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_l2mc_entry_fn)(
        _In_ const sai_l2mc_entry_t *l2mc_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove L2MC entry
 *
 * @param[in] l2mc_entry L2MC entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_l2mc_entry_fn)(
        _In_ const sai_l2mc_entry_t *l2mc_entry);

/**
 * @brief Set l2mc entry attribute value
 *
 * @param[in] l2mc_entry L2MC entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_l2mc_entry_attribute_fn)(
        _In_ const sai_l2mc_entry_t *l2mc_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get l2mc entry attribute value
 *
 * @param[in] l2mc_entry L2MC entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_l2mc_entry_attribute_fn)(
        _In_ const sai_l2mc_entry_t *l2mc_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief L2MC method table retrieved with sai_api_query()
 */
typedef struct _sai_l2mc_api_t
{
    sai_create_l2mc_entry_fn                     create_l2mc_entry;
    sai_remove_l2mc_entry_fn                     remove_l2mc_entry;
    sai_set_l2mc_entry_attribute_fn              set_l2mc_entry_attribute;
    sai_get_l2mc_entry_attribute_fn              get_l2mc_entry_attribute;

} sai_l2mc_api_t;

/**
 * @}
 */
#endif /** __SAIL2MC_H_ */
