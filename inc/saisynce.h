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
 * @file    saisynce.h
 *
 * @brief   This module defines SAI Synchronous Ethernet interface
 */

#if !defined (__SAISYNCE_H_)
#define __SAISYNCE_H_

#include <saitypes.h>

/**
 * @defgroup SAISYNCE SAI - SYNCE specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_SYNCE_CLOCK_ATTR_DEBUG_FORCE_SYNCE_RECOVERED_CLOCK_STATE
 */
typedef enum _sai_synce_recovered_clock_state_t
{
    /** Valid signal set as per device fault status */
    SAI_SYNCE_RECOVERED_CLOCK_STATE_FORCE_VALID_DISABLE,

    /** Valid signal forced to valid */
    SAI_SYNCE_RECOVERED_CLOCK_STATE_FORCE_ENABLE_VALID,

    /** Valid signal forced to invalid */
    SAI_SYNCE_RECOVERED_CLOCK_STATE_FORCE_ENABLE_INVALID,

} sai_synce_recovered_clock_state_t;

/**
 * @brief Enum defining SYNCE attributes.
 */
typedef enum _sai_synce_clock_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SYNCE_CLOCK_ATTR_START = 0x00000000,

    /**
     * @brief Synchronous ethernet(SYNCE) clock source port
     *
     * Sets the Port to be the source for SYNCE
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SYNCE_CLOCK_ATTR_SRC_PORT = SAI_SYNCE_CLOCK_ATTR_START,

    /**
     * @brief Synchronous ethernet (SYNCE) clock status
     *
     * Gets recovered clock signal with respect to hardware
     * (valid only after attaching the clock to a clock source port)
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_SYNCE_CLOCK_ATTR_CLOCK_VALID,

    /**
     * @brief Hardware clock-id
     *
     * Returns the hardware clock-id associated
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_SYNCE_CLOCK_ATTR_CLOCK_HARDWARE_ID,

    /**
     * @brief Synchronous ethernet (SYNCE) clock status
     *
     * Force set the recovered clock signal state (for debugging)
     *
     * @type sai_synce_recovered_clock_state_t
     * @flags CREATE_AND_SET
     * @default SAI_SYNCE_RECOVERED_CLOCK_STATE_FORCE_VALID_DISABLE
     */
    SAI_SYNCE_CLOCK_ATTR_DEBUG_FORCE_SYNCE_RECOVERED_CLOCK_STATE,

    /**
     * @brief End of attributes
     */
    SAI_SYNCE_CLOCK_ATTR_END,

    /** Custom range base value */
    SAI_SYNCE_CLOCK_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SYNCE_CLOCK_ATTR_CUSTOM_RANGE_END

} sai_synce_clock_attr_t;

/**
 * @brief Create SYNCE clock
 *
 * @param[out] synce_clock_id SYNCE clock id
 * @param[in] switch_id The Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_synce_clock_fn)(
        _Out_ sai_object_id_t *synce_clock_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove SYNCE clock
 *
 * @param[in] synce_clock_id SYNCE clock id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_synce_clock_fn)(
        _In_ sai_object_id_t synce_clock_id);

/**
 * @brief Set SYNCE clock Attribute
 *
 * @param[in] synce_clock_id SYNCE clock id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_synce_clock_attribute_fn)(
        _In_ sai_object_id_t synce_clock_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get SYNCE clock attribute
 *
 * @param[in] synce_clock_id SYNCE clock id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_synce_clock_attribute_fn)(
        _In_ sai_object_id_t synce_clock_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief SYNCE methods table retrieved with sai_api_query()
 */
typedef struct _sai_synce_api_t
{
    sai_create_synce_clock_fn          create_synce_clock;
    sai_remove_synce_clock_fn          remove_synce_clock;
    sai_set_synce_clock_attribute_fn   set_synce_clock_attribute;
    sai_get_synce_clock_attribute_fn   get_synce_clock_attribute;

} sai_synce_api_t;

/**
 * @}
 */
#endif /** __SAISYNCE_H_ */
