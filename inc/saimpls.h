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
 * @file    saimpls.h
 *
 * @brief   This module defines SAI MPLS interface
 */

#if !defined (__SAIMPLS_H_)
#define __SAIMPLS_H_

#include <saitypes.h>

/**
 * @defgroup SAIMPLS SAI - MPLS specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute Id for SAI in segment
 */
typedef enum _sai_inseg_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_INSEG_ENTRY_ATTR_START,

    /**
     * @brief Number of pops
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_INSEG_ENTRY_ATTR_NUM_OF_POP = SAI_INSEG_ENTRY_ATTR_START,

    /**
     * @brief Packet action
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_INSEG_ENTRY_ATTR_PACKET_ACTION,

    /**
     * @brief Packet priority for trap/log actions
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_INSEG_ENTRY_ATTR_TRAP_PRIORITY,

    /**
     * @brief The next hop id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NEXT_HOP, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_INSEG_ENTRY_ATTR_NEXT_HOP_ID,

    /**
     * @brief End of attributes
     */
    SAI_INSEG_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_INSEG_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_INSEG_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_inseg_entry_attr_t;

/**
 * @brief In segment entry
 */
typedef struct _sai_inseg_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief MPLS label
     */
    sai_label_id_t label;

} sai_inseg_entry_t;

/**
 * @brief Create In Segment entry
 *
 * @param[in] inseg_entry InSegment entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_inseg_entry_fn)(
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove In Segment entry
 *
 * @param[in] inseg_entry InSegment entry
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_inseg_entry_fn)(
        _In_ const sai_inseg_entry_t *inseg_entry);

/**
 * @brief Set In Segment attribute value
 *
 * @param[in] inseg_entry InSegment entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_inseg_entry_attribute_fn)(
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get In Segment attribute value
 *
 * @param[in] inseg_entry InSegment entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_inseg_entry_attribute_fn)(
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief MPLS methods table retrieved with sai_api_query()
 */
typedef struct _sai_mpls_api_t
{
    sai_create_inseg_entry_fn                      create_inseg_entry;
    sai_remove_inseg_entry_fn                      remove_inseg_entry;
    sai_set_inseg_entry_attribute_fn               set_inseg_entry_attribute;
    sai_get_inseg_entry_attribute_fn               get_inseg_entry_attribute;

} sai_mpls_api_t;

/**
 * @}
 */
#endif /** __SAIMPLS_H_ */
