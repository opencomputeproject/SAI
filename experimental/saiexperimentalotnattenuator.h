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
 * @file    saiexperimentalotnattenuator.h
 *
 * @brief   This module defines SAI extensions for optical attenuator.
 * It is derived from openconfig-optical-attenuator.yang, revision 2024-07-10.
 *
 * @warning This module is a SAI experimental module
 */

#if !defined (__SAIEXPERIMENTALOTNATTENUATOR_H_)
#define __SAIEXPERIMENTALOTNATTENUATOR_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALOTNATTENUATOR SAI - Experimental: Attenuator specific API definitions
 *
 * @{
 */

/**
 * @brief Type definition for different types of optical attenuator operating modes
 */
typedef enum _sai_otn_attenuator_mode_t
{
    SAI_OTN_ATTENUATOR_MODE_CONSTANT_POWER,
    SAI_OTN_ATTENUATOR_MODE_CONSTANT_ATTENUATION,
    SAI_OTN_ATTENUATOR_MODE_SYSTEM_CONTROLLED,
} sai_otn_attenuator_mode_t;

/**
 * @brief Attenuator attribute IDs
 */
typedef enum _sai_otn_attenuator_attr_t
{
    /**
     * @brief Start of attributes.
     */
    SAI_OTN_ATTENUATOR_ATTR_START,

    /**
     * @brief User-defined name assigned to identify a specific attenuator in the device.
     *
     * @type char
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_OTN_ATTENUATOR_ATTR_NAME = SAI_OTN_ATTENUATOR_ATTR_START,

    /**
     * @brief The operating mode of the attenuator.
     *
     * @type sai_otn_attenuator_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_OTN_ATTENUATOR_MODE_CONSTANT_ATTENUATION
     */
    SAI_OTN_ATTENUATOR_ATTR_ATTENUATION_MODE,

    /**
     * @brief Power level set on the output of attenuator in units of 0.01dBm.
     * This leaf is only relevant when in CONSTANT_POWER mode.
     *
     * @type sai_int32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @precision 2
     */
    SAI_OTN_ATTENUATOR_ATTR_TARGET_OUTPUT_POWER,

    /**
     * @brief Attenuation applied by the attenuator in units of 0.01dB.
     * This leaf is only relevant when in CONSTANT_ATTENUATION mode.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 500
     * @precision 2
     */
    SAI_OTN_ATTENUATOR_ATTR_ATTENUATION,

    /**
     * @brief When true, attenuator is set to specified attenuation or varies to
     * maintain constant output power. When false, the attenuator is set
     * max attenuation or blocked.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_OTN_ATTENUATOR_ATTR_ENABLED,

    /**
     * @brief The max power level allowed on the output of attenuator in units of 0.01dBm.
     * This leaf is optional when in SYSTEM_CONTROLLED mode.
     *
     * @type sai_int32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @precision 2
     */
    SAI_OTN_ATTENUATOR_ATTR_MAX_OUTPUT_POWER,

    /**
     * @brief A device alarm will be raised within /system/alarms when
     * system-derived-target-output-power >= max-output-power + max-output-power-threshold.
     * This leaf is only relevant when in SYSTEM_CONTROLLED mode.
     *
     * @type sai_int32_t
     * @flags CREATE_AND_SET
     * @default 300
     * @precision 2
     */
    SAI_OTN_ATTENUATOR_ATTR_MAX_OUTPUT_POWER_THRESHOLD,

    /**
     * @brief Reference to system-supplied name of the attenuator ingress port.
     * This leaf is only valid for ports of type INGRESS.
     *
     * @type char
     * @flags READ_ONLY
     */
    SAI_OTN_ATTENUATOR_ATTR_INGRESS_PORT,

    /**
     * @brief Reference to system-supplied name of the attenuator egress port.
     * This leaf is only valid for ports of type EGRESS.
     *
     * @type char
     * @flags READ_ONLY
     */
    SAI_OTN_ATTENUATOR_ATTR_EGRESS_PORT,

    /**
     * @brief The target output power as determined by the device in units of 0.01dBm.
     * This leaf is only relevant when in SYSTEM_CONTROLLED mode.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_ATTENUATOR_ATTR_SYSTEM_DERIVED_TARGET_OUTPUT_POWER,

    /**
     * @brief The actual attenuation applied by the attenuator in units of 0.01dB.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_ATTENUATOR_ATTR_ACTUAL_ATTENUATION,

    /**
     * @brief The total output optical power of this port in units of 0.01dBm.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_ATTENUATOR_ATTR_OUTPUT_POWER_TOTAL,

    /**
     * @brief The optical return loss (ORL) is the ratio of the light
     * reflected back into the port to the light launched out of the port.
     * ORL is in units of 0.01dB.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_ATTENUATOR_ATTR_OPTICAL_RETURN_LOSS,

    /**
     * @brief End of attributes
     */
    SAI_OTN_ATTENUATOR_ATTR_END,

    /** Custom range base value */
    SAI_OTN_ATTENUATOR_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_OTN_ATTENUATOR_ATTR_CUSTOM_RANGE_END

} sai_otn_attenuator_attr_t;

/**
 * @brief Create attenuator.
 *
 * Allocates and initializes an attenuator.
 *
 * @param[out] otn_attenuator_id Attenuator id
 * @param[in] switch_id Switch id on which the attenuator exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_otn_attenuator_fn)(
        _Out_ sai_object_id_t *otn_attenuator_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove attenuator
 *
 * @param[in] otn_attenuator_id Attenuator id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_otn_attenuator_fn)(
        _In_ sai_object_id_t otn_attenuator_id);

/**
 * @brief Set attenuator attribute
 *
 * @param[in] otn_attenuator_id Attenuator id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_otn_attenuator_attribute_fn)(
        _In_ sai_object_id_t otn_attenuator_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attenuator attribute
 *
 * @param[in] otn_attenuator_id Attenuator id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_otn_attenuator_attribute_fn)(
        _In_ sai_object_id_t otn_attenuator_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Routing interface methods table retrieved with sai_api_query()
 */
typedef struct _sai_otn_attenuator_api_t
{
    sai_create_otn_attenuator_fn                create_otn_attenuator;
    sai_remove_otn_attenuator_fn                remove_otn_attenuator;
    sai_set_otn_attenuator_attribute_fn         set_otn_attenuator_attribute;
    sai_get_otn_attenuator_attribute_fn         get_otn_attenuator_attribute;
} sai_otn_attenuator_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALOTNATTENUATOR_H_ */
