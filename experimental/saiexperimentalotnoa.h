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
 * @file    saiexperimentalotnoa.h
 *
 * @brief   This module defines SAI extensions for optical amplifier (OA).
 * It is derived from openconfig-optical-amplifier.yang, revision 2019-12-06.
 *
 * @warning This module is a SAI experimental module
 */

#if !defined (__SAIEXPERIMENTALOTNOA_H_)
#define __SAIEXPERIMENTALOTNOA_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALOTNOA SAI - Experimental: OA specific API definitions
 *
 * @{
 */

/** @brief OA type */
typedef enum _sai_otn_oa_type_t
{
    SAI_OTN_OA_TYPE_EDFA,
    SAI_OTN_OA_TYPE_FORWARD_RAMAN,
    SAI_OTN_OA_TYPE_BACKWARD_RAMAN,
    SAI_OTN_OA_TYPE_HYBRID,
} sai_otn_oa_type_t;

/** @brief OA gain range */
typedef enum _sai_otn_oa_gain_range_t
{
    SAI_OTN_OA_GAIN_RANGE_LOW_GAIN_RANGE,
    SAI_OTN_OA_GAIN_RANGE_MID_GAIN_RANGE,
    SAI_OTN_OA_GAIN_RANGE_HIGH_GAIN_RANGE,
    SAI_OTN_OA_GAIN_RANGE_FIXED_GAIN_RANGE,
} sai_otn_oa_gain_range_t;

/** @brief OA amp mode */
typedef enum _sai_otn_oa_amp_mode_t
{
    SAI_OTN_OA_AMP_MODE_CONSTANT_POWER,
    SAI_OTN_OA_AMP_MODE_CONSTANT_GAIN,
    SAI_OTN_OA_AMP_MODE_DYNAMIC_GAIN,
} sai_otn_oa_amp_mode_t;

/** @brief OA fiber type profile */
typedef enum _sai_otn_oa_fiber_type_profile_t
{
    SAI_OTN_OA_FIBER_TYPE_PROFILE_DSF,
    SAI_OTN_OA_FIBER_TYPE_PROFILE_LEAF,
    SAI_OTN_OA_FIBER_TYPE_PROFILE_SSMF,
    SAI_OTN_OA_FIBER_TYPE_PROFILE_TWC,
    SAI_OTN_OA_FIBER_TYPE_PROFILE_TWRS,
} sai_otn_oa_fiber_type_profile_t;

/**
 * @brief OA attribute IDs
 */
typedef enum _sai_otn_oa_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_OTN_OA_ATTR_START,

    /**
     * @brief User-defined name assigned to identify a specific amplifier in the device.
     *
     * @type char
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_OTN_OA_ATTR_NAME = SAI_OTN_OA_ATTR_START,

    /**
     * @brief Type of the amplifier.
     *
     * @type sai_otn_oa_type_t
     * @flags CREATE_AND_SET
     * @default SAI_OTN_OA_TYPE_EDFA
     */
    SAI_OTN_OA_ATTR_TYPE,

    /**
     * @brief Positive gain applied by the amplifier in units of 0.01dB.
     * This is used when the amp-mode is in CONSTANT_GAIN or DYNAMIC_GAIN
     * mode to set the target gain that the amplifier should achieve.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 2000
     * @precision 2
     */
    SAI_OTN_OA_ATTR_TARGET_GAIN,

    /**
     * @brief The minimum allowed gain of the amplifier in units of 0.01dB.
     * This is used when the amp-mode is in CONSTANT_POWER or DYNAMIC_GAIN mode
     * to prevent the gain from dropping below a desired threshold.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 500
     * @precision 2
     */
    SAI_OTN_OA_ATTR_MIN_GAIN,

    /**
     * @brief The maximum allowed gain of the amplifier in units of 0.01dB.
     * This is used when the amp-mode is in CONSTANT_POWER or DYNAMIC_GAIN mode
     * to prevent the gain from exceeding a desired threshold.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 3500
     * @precision 2
     */
    SAI_OTN_OA_ATTR_MAX_GAIN,

    /**
     * @brief Gain tilt control in units of 0.01dB.
     *
     * @type sai_int32_t
     * @flags CREATE_AND_SET
     * @default 0
     * @precision 2
     */
    SAI_OTN_OA_ATTR_TARGET_GAIN_TILT,

    /**
     * @brief Selected gain range. The gain range is a platform-defined
     * value indicating the switched gain amplifier setting.
     *
     * @type sai_otn_oa_gain_range_t
     * @flags CREATE_AND_SET
     * @default SAI_OTN_OA_GAIN_RANGE_LOW_GAIN_RANGE
     */
    SAI_OTN_OA_ATTR_GAIN_RANGE,

    /**
     * @brief The operating mode of the amplifier.
     *
     * @type sai_otn_oa_amp_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_OTN_OA_AMP_MODE_CONSTANT_GAIN
     */
    SAI_OTN_OA_ATTR_AMP_MODE,

    /**
     * @brief Output optical power of the amplifier in units of 0.01dBm.
     *
     * @type sai_int32_t
     * @flags CREATE_AND_SET
     * @default 800
     * @precision 2
     */
    SAI_OTN_OA_ATTR_TARGET_OUTPUT_POWER,

    /**
     * @brief The maximum optical output power of the amplifier in units of 0.01dBm.
     * This may be used to prevent the output power from exceeding a desired threshold.
     *
     * @type sai_int32_t
     * @flags CREATE_AND_SET
     * @default 2500
     * @precision 2
     */
    SAI_OTN_OA_ATTR_MAX_OUTPUT_POWER,

    /**
     * @brief Turns power on / off to the amplifiers gain module.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_OTN_OA_ATTR_ENABLED,

    /**
     * @brief The fiber type profile specifies details about the
     * fiber type which are needed to accurately determine
     * the gain and perform efficient amplification. This is
     * only needed for Raman type amplifiers.
     *
     * @type sai_otn_oa_fiber_type_profile_t
     * @flags CREATE_AND_SET
     * @default SAI_OTN_OA_FIBER_TYPE_PROFILE_SSMF
     */
    SAI_OTN_OA_ATTR_FIBER_TYPE_PROFILE,

    /**
     * @brief Reference to system-supplied name of the amplifier ingress port.
     * This leaf is only valid for ports of type INGRESS.
     *
     * @type char
     * @flags READ_ONLY
     */
    SAI_OTN_OA_ATTR_INGRESS_PORT,

    /**
     * @brief Reference to system-supplied name of the amplifier egress port.
     * This leaf is only valid for ports of type EGRESS.
     *
     * @type char
     * @flags READ_ONLY
     */
    SAI_OTN_OA_ATTR_EGRESS_PORT,

    /**
     * @brief The actual gain applied by the amplifier in units of 0.01dB.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_OA_ATTR_ACTUAL_GAIN,

    /**
     * @brief The actual tilt applied by the amplifier in units of 0.01dB.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_OA_ATTR_ACTUAL_GAIN_TILT,

    /**
     * @brief The total input optical power of this port in units of 0.01dBm.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_OA_ATTR_INPUT_POWER_TOTAL,

    /**
     * @brief The C band (consisting of approximately 191 to 195 THz or
     * 1530nm to 1565 nm) input optical power of this port in units of 0.01dBm.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_OA_ATTR_INPUT_POWER_C_BAND,

    /**
     * @brief The L band (consisting of approximately 184 to 191 THz or
     * 1565 to 1625 nm) input optical power of this port in units of 0.01dBm.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_OA_ATTR_INPUT_POWER_L_BAND,

    /**
     * @brief The total output optical power of this port in units of 0.01dBm.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_OA_ATTR_OUTPUT_POWER_TOTAL,

    /**
     * @brief The C band (consisting of approximately 191 to 195 THz or
     * 1530nm to 1565 nm)output optical power of this port in units of 0.01dBm.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_OA_ATTR_OUTPUT_POWER_C_BAND,

    /**
     * @brief The L band (consisting of approximately 184 to 191 THz or
     * 1565 to 1625 nm)output optical power of this port in units of 0.01dBm.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_OA_ATTR_OUTPUT_POWER_L_BAND,

    /**
     * @brief The current applied by the system to the transmit laser to
     * achieve the output power. The current is expressed in mA
     * with up to two decimal precision.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_OA_ATTR_LASER_BIAS_CURRENT,

    /**
     * @brief The optical return loss (ORL) is the ratio of the light
     * reflected back into the port to the light launched out of the port.
     * ORL is in units of 0.01dB.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     * @precision 2
     */
    SAI_OTN_OA_ATTR_OPTICAL_RETURN_LOSS,

    /**
     * @brief End of attributes
     */
    SAI_OTN_OA_ATTR_END,

    /** Custom range base value */
    SAI_OTN_OA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_OTN_OA_ATTR_CUSTOM_RANGE_END

} sai_otn_oa_attr_t;

/**
 * @brief Create OA.
 *
 * Allocates and initializes an OA.
 *
 * @param[out] otn_oa_id OA id
 * @param[in] switch_id Switch id on which the OA exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_otn_oa_fn)(
        _Out_ sai_object_id_t *otn_oa_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove OA
 *
 * @param[in] otn_oa_id OA id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_otn_oa_fn)(
        _In_ sai_object_id_t otn_oa_id);

/**
 * @brief Set OA attribute
 *
 * @param[in] otn_oa_id OA id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_otn_oa_attribute_fn)(
        _In_ sai_object_id_t otn_oa_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get OA attribute
 *
 * @param[in] otn_oa_id OA id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_otn_oa_attribute_fn)(
        _In_ sai_object_id_t otn_oa_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Routing interface methods table retrieved with sai_api_query()
 */
typedef struct _sai_otn_oa_api_t
{
    sai_create_otn_oa_fn                create_otn_oa;
    sai_remove_otn_oa_fn                remove_otn_oa;
    sai_set_otn_oa_attribute_fn         set_otn_oa_attribute;
    sai_get_otn_oa_attribute_fn         get_otn_oa_attribute;
} sai_otn_oa_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALOTNOA_H_ */
