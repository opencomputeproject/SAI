/**
 * Copyright (c) 2026 Microsoft Open Technologies, Inc.
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
 * @file    saiexperimentalponserviceconfigprofile.h
 *
 * @brief   This module defines the experimental PON service config profile API.
 */

#if !defined (__SAIEXPERIMENTALPONSERVICECONFIGPROFILE_H_)
#define __SAIEXPERIMENTALPONSERVICECONFIGPROFILE_H_

#include <saitypes.h>

#include <saiexperimentalponcommon.h>

/**
 * @defgroup SAIEXPERIMENTALPONSERVICECONFIGPROFILE SAI - Experimental: PON specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for PON service config profile header
 */
typedef enum _sai_pon_service_config_profile_header_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_ATTR_START,

    /**
     * @brief Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_ATTR_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_ATTR_START,

    /**
     * @brief Title
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_ATTR_TITLE,

    /**
     * @brief Version
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_ATTR_VERSION,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_header_attr_t;

/**
 * @brief Create PON service config profile header
 *
 * @param[out] pon_service_config_profile_header_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_header_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_header_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile header
 *
 * @param[in] pon_service_config_profile_header_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_header_fn)(
        _In_ sai_object_id_t pon_service_config_profile_header_id);

/**
 * @brief Set attribute for PON service config profile header
 *
 * @param[in] pon_service_config_profile_header_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_header_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_header_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile header
 *
 * @param[in] pon_service_config_profile_header_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_header_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_header_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile header compatibility vendor model
 */
typedef enum _sai_pon_service_config_profile_header_compatibility_vendor_model_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL_ATTR_START,

    /**
     * @brief Vendor ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL_ATTR_VENDOR_ID = SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL_ATTR_START,

    /**
     * @brief Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL_ATTR_NAME,

    /**
     * @brief Model
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL_ATTR_MODEL,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_header_compatibility_vendor_model_attr_t;

/**
 * @brief Create PON service config profile header compatibility vendor model
 *
 * @param[out] pon_service_config_profile_header_compatibility_vendor_model_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_header_compatibility_vendor_model_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_header_compatibility_vendor_model_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile header compatibility vendor model
 *
 * @param[in] pon_service_config_profile_header_compatibility_vendor_model_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_header_compatibility_vendor_model_fn)(
        _In_ sai_object_id_t pon_service_config_profile_header_compatibility_vendor_model_id);

/**
 * @brief Set attribute for PON service config profile header compatibility vendor model
 *
 * @param[in] pon_service_config_profile_header_compatibility_vendor_model_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_header_compatibility_vendor_model_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_header_compatibility_vendor_model_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile header compatibility vendor model
 *
 * @param[in] pon_service_config_profile_header_compatibility_vendor_model_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_header_compatibility_vendor_model_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_header_compatibility_vendor_model_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile header inputs ext input
 */
typedef enum _sai_pon_service_config_profile_header_inputs_ext_input_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT_ATTR_START,

    /**
     * @brief Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT_ATTR_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT_ATTR_START,

    /**
     * @brief Db Ref
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT_ATTR_DB_REF,

    /**
     * @brief Type
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT_ATTR_TYPE,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_header_inputs_ext_input_attr_t;

/**
 * @brief Create PON service config profile header inputs ext input
 *
 * @param[out] pon_service_config_profile_header_inputs_ext_input_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_header_inputs_ext_input_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_header_inputs_ext_input_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile header inputs ext input
 *
 * @param[in] pon_service_config_profile_header_inputs_ext_input_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_header_inputs_ext_input_fn)(
        _In_ sai_object_id_t pon_service_config_profile_header_inputs_ext_input_id);

/**
 * @brief Set attribute for PON service config profile header inputs ext input
 *
 * @param[in] pon_service_config_profile_header_inputs_ext_input_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_header_inputs_ext_input_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_header_inputs_ext_input_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile header inputs ext input
 *
 * @param[in] pon_service_config_profile_header_inputs_ext_input_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_header_inputs_ext_input_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_header_inputs_ext_input_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile ANI g
 */
typedef enum _sai_pon_service_config_profile_ani_g_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Gem Block Length
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_GEM_BLOCK_LENGTH,

    /**
     * @brief Sf Threshold
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_SF_THRESHOLD,

    /**
     * @brief Sd Threshold
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_SD_THRESHOLD,

    /**
     * @brief Arc
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_ARC,

    /**
     * @brief Arc Interval
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_ARC_INTERVAL,

    /**
     * @brief Lower Optical Threshold
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_LOWER_OPTICAL_THRESHOLD,

    /**
     * @brief Upper Optical Threshold
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_UPPER_OPTICAL_THRESHOLD,

    /**
     * @brief Lower Transmit Power Threshold
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_LOWER_TRANSMIT_POWER_THRESHOLD,

    /**
     * @brief Upper Transmit Power Threshold
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_UPPER_TRANSMIT_POWER_THRESHOLD,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ani_g_attr_t;

/**
 * @brief Create PON service config profile ANI g
 *
 * @param[out] pon_service_config_profile_ani_g_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ani_g_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ani_g_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile ANI g
 *
 * @param[in] pon_service_config_profile_ani_g_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ani_g_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ani_g_id);

/**
 * @brief Set attribute for PON service config profile ANI g
 *
 * @param[in] pon_service_config_profile_ani_g_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ani_g_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ani_g_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile ANI g
 *
 * @param[in] pon_service_config_profile_ani_g_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ani_g_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ani_g_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile cardholder
 */
typedef enum _sai_pon_service_config_profile_cardholder_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Expected Plugin Unit Type
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_EXPECTED_PLUGIN_UNIT_TYPE,

    /**
     * @brief Expected Port Count
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_EXPECTED_PORT_COUNT,

    /**
     * @brief Expected Equipment ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_EXPECTED_EQUIPMENT_ID,

    /**
     * @brief Invoke Protection Switch
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_INVOKE_PROTECTION_SWITCH,

    /**
     * @brief Arc
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_ARC,

    /**
     * @brief Arc Interval
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_ARC_INTERVAL,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_cardholder_attr_t;

/**
 * @brief Create PON service config profile cardholder
 *
 * @param[out] pon_service_config_profile_cardholder_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_cardholder_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_cardholder_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile cardholder
 *
 * @param[in] pon_service_config_profile_cardholder_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_cardholder_fn)(
        _In_ sai_object_id_t pon_service_config_profile_cardholder_id);

/**
 * @brief Set attribute for PON service config profile cardholder
 *
 * @param[in] pon_service_config_profile_cardholder_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_cardholder_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_cardholder_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile cardholder
 *
 * @param[in] pon_service_config_profile_cardholder_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_cardholder_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_cardholder_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile circuit pack
 */
typedef enum _sai_pon_service_config_profile_circuit_pack_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Administrative State
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_ADMINISTRATIVE_STATE,

    /**
     * @brief Bridged Or IP Ind
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_BRIDGED_OR_IP_IND,

    /**
     * @brief Card Configuration
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_CARD_CONFIGURATION,

    /**
     * @brief Power Sched Override
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_POWER_SCHED_OVERRIDE,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_circuit_pack_attr_t;

/**
 * @brief Create PON service config profile circuit pack
 *
 * @param[out] pon_service_config_profile_circuit_pack_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_circuit_pack_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_circuit_pack_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile circuit pack
 *
 * @param[in] pon_service_config_profile_circuit_pack_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_circuit_pack_fn)(
        _In_ sai_object_id_t pon_service_config_profile_circuit_pack_id);

/**
 * @brief Set attribute for PON service config profile circuit pack
 *
 * @param[in] pon_service_config_profile_circuit_pack_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_circuit_pack_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_circuit_pack_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile circuit pack
 *
 * @param[in] pon_service_config_profile_circuit_pack_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_circuit_pack_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_circuit_pack_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile enhanced FEC pm History data
 */
typedef enum _sai_pon_service_config_profile_enhanced_fec_pm_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_FEC_PM_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_FEC_PM_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_FEC_PM_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_FEC_PM_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 64 Bit ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_FEC_PM_HIST_DATA_ATTR_THRESHOLD_DATA_64_BIT_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_FEC_PM_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_FEC_PM_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_FEC_PM_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_enhanced_fec_pm_hist_data_attr_t;

/**
 * @brief Create PON service config profile enhanced FEC pm History data
 *
 * @param[out] pon_service_config_profile_enhanced_fec_pm_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_enhanced_fec_pm_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_enhanced_fec_pm_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile enhanced FEC pm History data
 *
 * @param[in] pon_service_config_profile_enhanced_fec_pm_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_enhanced_fec_pm_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_enhanced_fec_pm_hist_data_id);

/**
 * @brief Set attribute for PON service config profile enhanced FEC pm History data
 *
 * @param[in] pon_service_config_profile_enhanced_fec_pm_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_enhanced_fec_pm_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_enhanced_fec_pm_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile enhanced FEC pm History data
 *
 * @param[in] pon_service_config_profile_enhanced_fec_pm_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_enhanced_fec_pm_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_enhanced_fec_pm_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile enhanced TC Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_TC_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_TC_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_TC_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_TC_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 64 Bit ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_TC_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_64_BIT_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_TC_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_TC_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_TC_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile enhanced TC Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_enhanced_tc_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_enhanced_tc_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile enhanced TC Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_enhanced_tc_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_enhanced_tc_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile enhanced TC Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_enhanced_tc_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_enhanced_tc_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile enhanced TC Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_enhanced_tc_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_enhanced_tc_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile ethernet frame extended pm
 */
typedef enum _sai_pon_service_config_profile_ethernet_frame_extended_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_THRESHOLD_DATA_ID,

    /**
     * @brief Parent Me Class
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_PARENT_ME_CLASS,

    /**
     * @brief Parent Me Instance
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_PARENT_ME_INSTANCE,

    /**
     * @brief Accumulation Disable
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_ACCUMULATION_DISABLE,

    /**
     * @brief Tca Disable
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_TCA_DISABLE,

    /**
     * @brief Control Fields
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_CONTROL_FIELDS,

    /**
     * @brief Tci
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_TCI,

    /**
     * @brief Reserved
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_RESERVED,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ethernet_frame_extended_pm_attr_t;

/**
 * @brief Create PON service config profile ethernet frame extended pm
 *
 * @param[out] pon_service_config_profile_ethernet_frame_extended_pm_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ethernet_frame_extended_pm_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ethernet_frame_extended_pm_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile ethernet frame extended pm
 *
 * @param[in] pon_service_config_profile_ethernet_frame_extended_pm_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ethernet_frame_extended_pm_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_frame_extended_pm_id);

/**
 * @brief Set attribute for PON service config profile ethernet frame extended pm
 *
 * @param[in] pon_service_config_profile_ethernet_frame_extended_pm_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ethernet_frame_extended_pm_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_frame_extended_pm_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile ethernet frame extended pm
 *
 * @param[in] pon_service_config_profile_ethernet_frame_extended_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ethernet_frame_extended_pm_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_frame_extended_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile ethernet frame extended pm64 bit
 */
typedef enum _sai_pon_service_config_profile_ethernet_frame_extended_pm64_bit_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_THRESHOLD_DATA_ID,

    /**
     * @brief Parent Me Class
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_PARENT_ME_CLASS,

    /**
     * @brief Parent Me Instance
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_PARENT_ME_INSTANCE,

    /**
     * @brief Accumulation Disable
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_ACCUMULATION_DISABLE,

    /**
     * @brief Tca Disable
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_TCA_DISABLE,

    /**
     * @brief Control Fields
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_CONTROL_FIELDS,

    /**
     * @brief Tci
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_TCI,

    /**
     * @brief Reserved
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_RESERVED,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ethernet_frame_extended_pm64_bit_attr_t;

/**
 * @brief Create PON service config profile ethernet frame extended pm64 bit
 *
 * @param[out] pon_service_config_profile_ethernet_frame_extended_pm64_bit_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ethernet_frame_extended_pm64_bit_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ethernet_frame_extended_pm64_bit_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile ethernet frame extended pm64 bit
 *
 * @param[in] pon_service_config_profile_ethernet_frame_extended_pm64_bit_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ethernet_frame_extended_pm64_bit_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_frame_extended_pm64_bit_id);

/**
 * @brief Set attribute for PON service config profile ethernet frame extended pm64 bit
 *
 * @param[in] pon_service_config_profile_ethernet_frame_extended_pm64_bit_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ethernet_frame_extended_pm64_bit_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_frame_extended_pm64_bit_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile ethernet frame extended pm64 bit
 *
 * @param[in] pon_service_config_profile_ethernet_frame_extended_pm64_bit_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ethernet_frame_extended_pm64_bit_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_frame_extended_pm64_bit_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile ethernet frame Performance Monitoring History data downstream
 */
typedef enum _sai_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_DOWNSTREAM_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_DOWNSTREAM_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_DOWNSTREAM_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_DOWNSTREAM_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_DOWNSTREAM_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_DOWNSTREAM_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_DOWNSTREAM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_DOWNSTREAM_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_attr_t;

/**
 * @brief Create PON service config profile ethernet frame Performance Monitoring History data downstream
 *
 * @param[out] pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile ethernet frame Performance Monitoring History data downstream
 *
 * @param[in] pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_id);

/**
 * @brief Set attribute for PON service config profile ethernet frame Performance Monitoring History data downstream
 *
 * @param[in] pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile ethernet frame Performance Monitoring History data downstream
 *
 * @param[in] pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile ethernet frame Performance Monitoring History data upstream
 */
typedef enum _sai_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_UPSTREAM_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_UPSTREAM_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_UPSTREAM_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_UPSTREAM_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_UPSTREAM_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_UPSTREAM_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_UPSTREAM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_UPSTREAM_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_attr_t;

/**
 * @brief Create PON service config profile ethernet frame Performance Monitoring History data upstream
 *
 * @param[out] pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile ethernet frame Performance Monitoring History data upstream
 *
 * @param[in] pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_id);

/**
 * @brief Set attribute for PON service config profile ethernet frame Performance Monitoring History data upstream
 *
 * @param[in] pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile ethernet frame Performance Monitoring History data upstream
 *
 * @param[in] pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile ethernet Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_ethernet_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ethernet_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile ethernet Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_ethernet_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ethernet_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ethernet_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile ethernet Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_ethernet_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ethernet_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile ethernet Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_ethernet_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ethernet_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile ethernet Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_ethernet_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ethernet_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile ethernet Performance Monitoring History data3
 */
typedef enum _sai_pon_service_config_profile_ethernet_perf_mon_hist_data3_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA3_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA3_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA3_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA3_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA3_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA3_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA3_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA3_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ethernet_perf_mon_hist_data3_attr_t;

/**
 * @brief Create PON service config profile ethernet Performance Monitoring History data3
 *
 * @param[out] pon_service_config_profile_ethernet_perf_mon_hist_data3_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ethernet_perf_mon_hist_data3_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ethernet_perf_mon_hist_data3_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile ethernet Performance Monitoring History data3
 *
 * @param[in] pon_service_config_profile_ethernet_perf_mon_hist_data3_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ethernet_perf_mon_hist_data3_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_perf_mon_hist_data3_id);

/**
 * @brief Set attribute for PON service config profile ethernet Performance Monitoring History data3
 *
 * @param[in] pon_service_config_profile_ethernet_perf_mon_hist_data3_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ethernet_perf_mon_hist_data3_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_perf_mon_hist_data3_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile ethernet Performance Monitoring History data3
 *
 * @param[in] pon_service_config_profile_ethernet_perf_mon_hist_data3_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ethernet_perf_mon_hist_data3_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ethernet_perf_mon_hist_data3_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile extended vlan tagging operation config data
 */
typedef enum _sai_pon_service_config_profile_extended_vlan_tagging_operation_config_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Association Type
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_ASSOCIATION_TYPE,

    /**
     * @brief Input TPID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_INPUT_TPID,

    /**
     * @brief Output TPID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_OUTPUT_TPID,

    /**
     * @brief Downstream Mode
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_DOWNSTREAM_MODE,

    /**
     * @brief Associated Me Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_ASSOCIATED_ME_POINTER,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_extended_vlan_tagging_operation_config_data_attr_t;

/**
 * @brief Create PON service config profile extended vlan tagging operation config data
 *
 * @param[out] pon_service_config_profile_extended_vlan_tagging_operation_config_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_extended_vlan_tagging_operation_config_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile extended vlan tagging operation config data
 *
 * @param[in] pon_service_config_profile_extended_vlan_tagging_operation_config_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_extended_vlan_tagging_operation_config_data_id);

/**
 * @brief Set attribute for PON service config profile extended vlan tagging operation config data
 *
 * @param[in] pon_service_config_profile_extended_vlan_tagging_operation_config_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_extended_vlan_tagging_operation_config_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile extended vlan tagging operation config data
 *
 * @param[in] pon_service_config_profile_extended_vlan_tagging_operation_config_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_extended_vlan_tagging_operation_config_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile extended vlan tagging operation config data received frame vlan tagging operation
 */
typedef enum _sai_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_ID,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Received Frame Vlan Tagging Operation Table Entry ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_TABLE_ENTRY_ID,

    /**
     * @brief Filter Outer Priority
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_OUTER_PRIORITY,

    /**
     * @brief Filter Outer VID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_OUTER_VID,

    /**
     * @brief Filter Outer TPID De
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_OUTER_TPID_DE,

    /**
     * @brief Pad1
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_PAD1,

    /**
     * @brief Filter Inner Priority
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_INNER_PRIORITY,

    /**
     * @brief Filter Inner VID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_INNER_VID,

    /**
     * @brief Filter Inner TPID De
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_INNER_TPID_DE,

    /**
     * @brief Pad2
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_PAD2,

    /**
     * @brief Filter Ether Type
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_ETHER_TYPE,

    /**
     * @brief Treatment Tags To Remove
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_TAGS_TO_REMOVE,

    /**
     * @brief Pad3
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_PAD3,

    /**
     * @brief Treatment Outer Priority
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_OUTER_PRIORITY,

    /**
     * @brief Treatment Outer VID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_OUTER_VID,

    /**
     * @brief Treatment Outer TPID De
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_OUTER_TPID_DE,

    /**
     * @brief Pad4
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_PAD4,

    /**
     * @brief Treatment Inner Priority
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_INNER_PRIORITY,

    /**
     * @brief Treatment Inner VID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_INNER_VID,

    /**
     * @brief Treatment Inner TPID De
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_INNER_TPID_DE,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_attr_t;

/**
 * @brief Create PON service config profile extended vlan tagging operation config data received frame vlan tagging operation
 *
 * @param[out] pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile extended vlan tagging operation config data received frame vlan tagging operation
 *
 * @param[in] pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_fn)(
        _In_ sai_object_id_t pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_id);

/**
 * @brief Set attribute for PON service config profile extended vlan tagging operation config data received frame vlan tagging operation
 *
 * @param[in] pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile extended vlan tagging operation config data received frame vlan tagging operation
 *
 * @param[in] pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile extended vlan tagging operation config data DSCP to pbit mapping
 */
typedef enum _sai_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_ID,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief DSCP
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_DSCP,

    /**
     * @brief Priority
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_PRIORITY,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_attr_t;

/**
 * @brief Create PON service config profile extended vlan tagging operation config data DSCP to pbit mapping
 *
 * @param[out] pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile extended vlan tagging operation config data DSCP to pbit mapping
 *
 * @param[in] pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_fn)(
        _In_ sai_object_id_t pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_id);

/**
 * @brief Set attribute for PON service config profile extended vlan tagging operation config data DSCP to pbit mapping
 *
 * @param[in] pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile extended vlan tagging operation config data DSCP to pbit mapping
 *
 * @param[in] pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile FEC Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_fec_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_FEC_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_FEC_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_FEC_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_FEC_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_FEC_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_FEC_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_FEC_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_FEC_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_fec_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile FEC Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_fec_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_fec_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_fec_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile FEC Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_fec_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_fec_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_fec_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile FEC Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_fec_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_fec_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_fec_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile FEC Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_fec_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_fec_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_fec_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile gal ethernet Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile gal ethernet Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_gal_ethernet_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_gal_ethernet_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile gal ethernet Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_gal_ethernet_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gal_ethernet_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile gal ethernet Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_gal_ethernet_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gal_ethernet_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile gal ethernet Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_gal_ethernet_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gal_ethernet_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile gal ethernet profile
 */
typedef enum _sai_pon_service_config_profile_gal_ethernet_profile_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PROFILE_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PROFILE_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PROFILE_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PROFILE_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Max Gem Payload Size
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PROFILE_ATTR_MAX_GEM_PAYLOAD_SIZE,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PROFILE_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PROFILE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PROFILE_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_gal_ethernet_profile_attr_t;

/**
 * @brief Create PON service config profile gal ethernet profile
 *
 * @param[out] pon_service_config_profile_gal_ethernet_profile_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_gal_ethernet_profile_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_gal_ethernet_profile_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile gal ethernet profile
 *
 * @param[in] pon_service_config_profile_gal_ethernet_profile_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_gal_ethernet_profile_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gal_ethernet_profile_id);

/**
 * @brief Set attribute for PON service config profile gal ethernet profile
 *
 * @param[in] pon_service_config_profile_gal_ethernet_profile_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_gal_ethernet_profile_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gal_ethernet_profile_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile gal ethernet profile
 *
 * @param[in] pon_service_config_profile_gal_ethernet_profile_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_gal_ethernet_profile_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gal_ethernet_profile_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile gem interworking tp
 */
typedef enum _sai_pon_service_config_profile_gem_interworking_tp_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Gem Port Network CTP Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_GEM_PORT_NETWORK_CTP_POINTER,

    /**
     * @brief Interworking Option
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_INTERWORKING_OPTION,

    /**
     * @brief Service Profile Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_SERVICE_PROFILE_POINTER,

    /**
     * @brief Interworking Tp Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_INTERWORKING_TP_POINTER,

    /**
     * @brief Gal Profile Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_GAL_PROFILE_POINTER,

    /**
     * @brief Gal Loopback Configuration
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_GAL_LOOPBACK_CONFIGURATION,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_gem_interworking_tp_attr_t;

/**
 * @brief Create PON service config profile gem interworking tp
 *
 * @param[out] pon_service_config_profile_gem_interworking_tp_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_gem_interworking_tp_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_gem_interworking_tp_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile gem interworking tp
 *
 * @param[in] pon_service_config_profile_gem_interworking_tp_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_gem_interworking_tp_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gem_interworking_tp_id);

/**
 * @brief Set attribute for PON service config profile gem interworking tp
 *
 * @param[in] pon_service_config_profile_gem_interworking_tp_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_gem_interworking_tp_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gem_interworking_tp_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile gem interworking tp
 *
 * @param[in] pon_service_config_profile_gem_interworking_tp_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_gem_interworking_tp_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gem_interworking_tp_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile gem port network CTP
 */
typedef enum _sai_pon_service_config_profile_gem_port_network_ctp_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Port ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_PORT_ID,

    /**
     * @brief TCONT Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_TCONT_POINTER,

    /**
     * @brief Direction
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_DIRECTION,

    /**
     * @brief Traffic Management Pointer Upstream
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_TRAFFIC_MANAGEMENT_POINTER_UPSTREAM,

    /**
     * @brief Traffic Descriptor Profile Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_TRAFFIC_DESCRIPTOR_PROFILE_POINTER,

    /**
     * @brief Priority Queue Pointer Downstream
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_PRIORITY_QUEUE_POINTER_DOWNSTREAM,

    /**
     * @brief Traffic Desc Profile Pointer Downstream
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_TRAFFIC_DESC_PROFILE_POINTER_DOWNSTREAM,

    /**
     * @brief Encryption Key Ring
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_ENCRYPTION_KEY_RING,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_gem_port_network_ctp_attr_t;

/**
 * @brief Create PON service config profile gem port network CTP
 *
 * @param[out] pon_service_config_profile_gem_port_network_ctp_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_gem_port_network_ctp_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_gem_port_network_ctp_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile gem port network CTP
 *
 * @param[in] pon_service_config_profile_gem_port_network_ctp_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_gem_port_network_ctp_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gem_port_network_ctp_id);

/**
 * @brief Set attribute for PON service config profile gem port network CTP
 *
 * @param[in] pon_service_config_profile_gem_port_network_ctp_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_gem_port_network_ctp_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gem_port_network_ctp_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile gem port network CTP
 *
 * @param[in] pon_service_config_profile_gem_port_network_ctp_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_gem_port_network_ctp_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gem_port_network_ctp_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile gem port network CTP Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile gem port network CTP Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile gem port network CTP Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile gem port network CTP Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile gem port network CTP Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile ieee8021p mapper service profile
 */
typedef enum _sai_pon_service_config_profile_ieee8021p_mapper_service_profile_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Tp Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_TP_POINTER,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 0
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_0,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 1
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_1,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 2
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_2,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 3
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_3,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 4
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_4,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 5
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_5,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 6
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_6,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 7
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_7,

    /**
     * @brief Unmarked Frame Option
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_UNMARKED_FRAME_OPTION,

    /**
     * @brief Default P Bit Assumption
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_DEFAULT_P_BIT_ASSUMPTION,

    /**
     * @brief Tp Type
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_TP_TYPE,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ieee8021p_mapper_service_profile_attr_t;

/**
 * @brief Create PON service config profile ieee8021p mapper service profile
 *
 * @param[out] pon_service_config_profile_ieee8021p_mapper_service_profile_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ieee8021p_mapper_service_profile_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ieee8021p_mapper_service_profile_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile ieee8021p mapper service profile
 *
 * @param[in] pon_service_config_profile_ieee8021p_mapper_service_profile_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ieee8021p_mapper_service_profile_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ieee8021p_mapper_service_profile_id);

/**
 * @brief Set attribute for PON service config profile ieee8021p mapper service profile
 *
 * @param[in] pon_service_config_profile_ieee8021p_mapper_service_profile_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ieee8021p_mapper_service_profile_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ieee8021p_mapper_service_profile_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile ieee8021p mapper service profile
 *
 * @param[in] pon_service_config_profile_ieee8021p_mapper_service_profile_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ieee8021p_mapper_service_profile_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ieee8021p_mapper_service_profile_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile ieee8021p mapper service profile DSCP to p bit mapping
 */
typedef enum _sai_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_ID,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief DSCP
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_DSCP,

    /**
     * @brief Priority
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_PRIORITY,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_attr_t;

/**
 * @brief Create PON service config profile ieee8021p mapper service profile DSCP to p bit mapping
 *
 * @param[out] pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile ieee8021p mapper service profile DSCP to p bit mapping
 *
 * @param[in] pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_id);

/**
 * @brief Set attribute for PON service config profile ieee8021p mapper service profile DSCP to p bit mapping
 *
 * @param[in] pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile ieee8021p mapper service profile DSCP to p bit mapping
 *
 * @param[in] pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile IP host config data
 */
typedef enum _sai_pon_service_config_profile_ip_host_config_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief IP Options
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_IP_OPTIONS,

    /**
     * @brief ONU Identifier
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_ONU_IDENTIFIER,

    /**
     * @brief IP Address
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_IP_ADDRESS,

    /**
     * @brief Mask
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_MASK,

    /**
     * @brief Gateway
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_GATEWAY,

    /**
     * @brief Primary DNS
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_PRIMARY_DNS,

    /**
     * @brief Secondary DNS
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_SECONDARY_DNS,

    /**
     * @brief Relay Agent Options
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_RELAY_AGENT_OPTIONS,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ip_host_config_data_attr_t;

/**
 * @brief Create PON service config profile IP host config data
 *
 * @param[out] pon_service_config_profile_ip_host_config_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ip_host_config_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ip_host_config_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile IP host config data
 *
 * @param[in] pon_service_config_profile_ip_host_config_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ip_host_config_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ip_host_config_data_id);

/**
 * @brief Set attribute for PON service config profile IP host config data
 *
 * @param[in] pon_service_config_profile_ip_host_config_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ip_host_config_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ip_host_config_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile IP host config data
 *
 * @param[in] pon_service_config_profile_ip_host_config_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ip_host_config_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ip_host_config_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile IP host Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_ip_host_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ip_host_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile IP host Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_ip_host_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ip_host_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ip_host_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile IP host Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_ip_host_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ip_host_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ip_host_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile IP host Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_ip_host_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ip_host_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ip_host_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile IP host Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_ip_host_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ip_host_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ip_host_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile IPV6 host config data
 */
typedef enum _sai_pon_service_config_profile_ipv6_host_config_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief IP Options
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_IP_OPTIONS,

    /**
     * @brief ONU Identifier
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_ONU_IDENTIFIER,

    /**
     * @brief IPV6 Address
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_IPV6_ADDRESS,

    /**
     * @brief Default Router
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_DEFAULT_ROUTER,

    /**
     * @brief Primary DNS
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_PRIMARY_DNS,

    /**
     * @brief Secondary DNS
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_SECONDARY_DNS,

    /**
     * @brief On Link Prefix
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_ON_LINK_PREFIX,

    /**
     * @brief Relay Agent Options
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_RELAY_AGENT_OPTIONS,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ipv6_host_config_data_attr_t;

/**
 * @brief Create PON service config profile IPV6 host config data
 *
 * @param[out] pon_service_config_profile_ipv6_host_config_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ipv6_host_config_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ipv6_host_config_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile IPV6 host config data
 *
 * @param[in] pon_service_config_profile_ipv6_host_config_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ipv6_host_config_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ipv6_host_config_data_id);

/**
 * @brief Set attribute for PON service config profile IPV6 host config data
 *
 * @param[in] pon_service_config_profile_ipv6_host_config_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ipv6_host_config_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ipv6_host_config_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile IPV6 host config data
 *
 * @param[in] pon_service_config_profile_ipv6_host_config_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ipv6_host_config_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ipv6_host_config_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile large string
 */
typedef enum _sai_pon_service_config_profile_large_string_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Number Of Parts
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_NUMBER_OF_PARTS,

    /**
     * @brief Part 1
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_1,

    /**
     * @brief Part 2
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_2,

    /**
     * @brief Part 3
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_3,

    /**
     * @brief Part 4
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_4,

    /**
     * @brief Part 5
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_5,

    /**
     * @brief Part 6
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_6,

    /**
     * @brief Part 7
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_7,

    /**
     * @brief Part 8
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_8,

    /**
     * @brief Part 9
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_9,

    /**
     * @brief Part 10
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_10,

    /**
     * @brief Part 11
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_11,

    /**
     * @brief Part 12
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_12,

    /**
     * @brief Part 13
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_13,

    /**
     * @brief Part 14
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_14,

    /**
     * @brief Part 15
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_15,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_large_string_attr_t;

/**
 * @brief Create PON service config profile large string
 *
 * @param[out] pon_service_config_profile_large_string_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_large_string_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_large_string_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile large string
 *
 * @param[in] pon_service_config_profile_large_string_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_large_string_fn)(
        _In_ sai_object_id_t pon_service_config_profile_large_string_id);

/**
 * @brief Set attribute for PON service config profile large string
 *
 * @param[in] pon_service_config_profile_large_string_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_large_string_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_large_string_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile large string
 *
 * @param[in] pon_service_config_profile_large_string_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_large_string_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_large_string_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile MAC bridge Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_mac_bridge_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_mac_bridge_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile MAC bridge Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_mac_bridge_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_mac_bridge_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_mac_bridge_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile MAC bridge Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_mac_bridge_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_mac_bridge_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_mac_bridge_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile MAC bridge Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_mac_bridge_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_mac_bridge_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_mac_bridge_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile MAC bridge Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_mac_bridge_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_mac_bridge_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_mac_bridge_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile MAC bridge port config data
 */
typedef enum _sai_pon_service_config_profile_mac_bridge_port_config_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Bridge ID Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_BRIDGE_ID_POINTER,

    /**
     * @brief Port Num
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_PORT_NUM,

    /**
     * @brief Tp Type
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_TP_TYPE,

    /**
     * @brief Tp Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_TP_POINTER,

    /**
     * @brief Port Priority
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_PORT_PRIORITY,

    /**
     * @brief Port Path Cost
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_PORT_PATH_COST,

    /**
     * @brief Port Spanning Tree Ind
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_PORT_SPANNING_TREE_IND,

    /**
     * @brief Deprecated1
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_DEPRECATED1,

    /**
     * @brief Deprecated2
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_DEPRECATED2,

    /**
     * @brief Outbound Td Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_OUTBOUND_TD_POINTER,

    /**
     * @brief Inbound Td Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_INBOUND_TD_POINTER,

    /**
     * @brief MAC Learning Depth
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_MAC_LEARNING_DEPTH,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_mac_bridge_port_config_data_attr_t;

/**
 * @brief Create PON service config profile MAC bridge port config data
 *
 * @param[out] pon_service_config_profile_mac_bridge_port_config_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_mac_bridge_port_config_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_mac_bridge_port_config_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile MAC bridge port config data
 *
 * @param[in] pon_service_config_profile_mac_bridge_port_config_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_mac_bridge_port_config_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_mac_bridge_port_config_data_id);

/**
 * @brief Set attribute for PON service config profile MAC bridge port config data
 *
 * @param[in] pon_service_config_profile_mac_bridge_port_config_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_mac_bridge_port_config_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_mac_bridge_port_config_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile MAC bridge port config data
 *
 * @param[in] pon_service_config_profile_mac_bridge_port_config_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_mac_bridge_port_config_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_mac_bridge_port_config_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile MAC bridge port Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile MAC bridge port Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile MAC bridge port Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile MAC bridge port Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile MAC bridge port Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile MAC bridge service profile
 */
typedef enum _sai_pon_service_config_profile_mac_bridge_service_profile_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Spanning Tree Ind
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_SPANNING_TREE_IND,

    /**
     * @brief Learning Ind
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_LEARNING_IND,

    /**
     * @brief Port Bridging Ind
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_PORT_BRIDGING_IND,

    /**
     * @brief Priority
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_PRIORITY,

    /**
     * @brief Max Age
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_MAX_AGE,

    /**
     * @brief Hello Time
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_HELLO_TIME,

    /**
     * @brief Forward Delay
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_FORWARD_DELAY,

    /**
     * @brief Unknown MAC Address Discard
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_UNKNOWN_MAC_ADDRESS_DISCARD,

    /**
     * @brief MAC Learning Depth
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_MAC_LEARNING_DEPTH,

    /**
     * @brief Dynamic Filtering Ageing Time
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_DYNAMIC_FILTERING_AGEING_TIME,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_mac_bridge_service_profile_attr_t;

/**
 * @brief Create PON service config profile MAC bridge service profile
 *
 * @param[out] pon_service_config_profile_mac_bridge_service_profile_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_mac_bridge_service_profile_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_mac_bridge_service_profile_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile MAC bridge service profile
 *
 * @param[in] pon_service_config_profile_mac_bridge_service_profile_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_mac_bridge_service_profile_fn)(
        _In_ sai_object_id_t pon_service_config_profile_mac_bridge_service_profile_id);

/**
 * @brief Set attribute for PON service config profile MAC bridge service profile
 *
 * @param[in] pon_service_config_profile_mac_bridge_service_profile_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_mac_bridge_service_profile_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_mac_bridge_service_profile_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile MAC bridge service profile
 *
 * @param[in] pon_service_config_profile_mac_bridge_service_profile_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_mac_bridge_service_profile_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_mac_bridge_service_profile_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile multicast gem interworking tp
 */
typedef enum _sai_pon_service_config_profile_multicast_gem_interworking_tp_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Gem Port Network CTP Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_GEM_PORT_NETWORK_CTP_POINTER,

    /**
     * @brief Interworking Option
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_INTERWORKING_OPTION,

    /**
     * @brief Service Profile Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_SERVICE_PROFILE_POINTER,

    /**
     * @brief Interworking Tp Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_INTERWORKING_TP_POINTER,

    /**
     * @brief Gal Profile Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_GAL_PROFILE_POINTER,

    /**
     * @brief Gal Loopback Configuration
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_GAL_LOOPBACK_CONFIGURATION,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_multicast_gem_interworking_tp_attr_t;

/**
 * @brief Create PON service config profile multicast gem interworking tp
 *
 * @param[out] pon_service_config_profile_multicast_gem_interworking_tp_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_multicast_gem_interworking_tp_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_multicast_gem_interworking_tp_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile multicast gem interworking tp
 *
 * @param[in] pon_service_config_profile_multicast_gem_interworking_tp_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_multicast_gem_interworking_tp_fn)(
        _In_ sai_object_id_t pon_service_config_profile_multicast_gem_interworking_tp_id);

/**
 * @brief Set attribute for PON service config profile multicast gem interworking tp
 *
 * @param[in] pon_service_config_profile_multicast_gem_interworking_tp_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_multicast_gem_interworking_tp_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_multicast_gem_interworking_tp_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile multicast gem interworking tp
 *
 * @param[in] pon_service_config_profile_multicast_gem_interworking_tp_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_multicast_gem_interworking_tp_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_multicast_gem_interworking_tp_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile multicast gem interworking tp IPV4 multicast address
 */
typedef enum _sai_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_ID,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief IPV4 Multicast Address Table Entry ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_IPV4_MULTICAST_ADDRESS_TABLE_ENTRY_ID,

    /**
     * @brief Gem Port ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_GEM_PORT_ID,

    /**
     * @brief Secondary Key
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_SECONDARY_KEY,

    /**
     * @brief IP Multicast Da Range Start
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_IP_MULTICAST_DA_RANGE_BEGIN,

    /**
     * @brief IP Multicast Da Range Stop
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_IP_MULTICAST_DA_RANGE_STOP,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_attr_t;

/**
 * @brief Create PON service config profile multicast gem interworking tp IPV4 multicast address
 *
 * @param[out] pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile multicast gem interworking tp IPV4 multicast address
 *
 * @param[in] pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_fn)(
        _In_ sai_object_id_t pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_id);

/**
 * @brief Set attribute for PON service config profile multicast gem interworking tp IPV4 multicast address
 *
 * @param[in] pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile multicast gem interworking tp IPV4 multicast address
 *
 * @param[in] pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile multicast gem interworking tp IPV6 multicast address
 */
typedef enum _sai_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_ID,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief IPV6 Multicast Address Table Entry ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_IPV6_MULTICAST_ADDRESS_TABLE_ENTRY_ID,

    /**
     * @brief Gem Port ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_GEM_PORT_ID,

    /**
     * @brief Secondary Key
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_SECONDARY_KEY,

    /**
     * @brief Lsb IP Multicast Da Range Start
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_LSB_IP_MULTICAST_DA_RANGE_BEGIN,

    /**
     * @brief Lsb IP Multicast Da Range Stop
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_LSB_IP_MULTICAST_DA_RANGE_STOP,

    /**
     * @brief Msb IP Multicast Da
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_MSB_IP_MULTICAST_DA,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_attr_t;

/**
 * @brief Create PON service config profile multicast gem interworking tp IPV6 multicast address
 *
 * @param[out] pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile multicast gem interworking tp IPV6 multicast address
 *
 * @param[in] pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_fn)(
        _In_ sai_object_id_t pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_id);

/**
 * @brief Set attribute for PON service config profile multicast gem interworking tp IPV6 multicast address
 *
 * @param[in] pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile multicast gem interworking tp IPV6 multicast address
 *
 * @param[in] pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile OLT g
 */
typedef enum _sai_pon_service_config_profile_olt_g_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief OLT Vendor ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_OLT_VENDOR_ID,

    /**
     * @brief Equipment ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_EQUIPMENT_ID,

    /**
     * @brief Version
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_VERSION,

    /**
     * @brief Time Of Day
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_TIME_OF_DAY,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_olt_g_attr_t;

/**
 * @brief Create PON service config profile OLT g
 *
 * @param[out] pon_service_config_profile_olt_g_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_olt_g_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_olt_g_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile OLT g
 *
 * @param[in] pon_service_config_profile_olt_g_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_olt_g_fn)(
        _In_ sai_object_id_t pon_service_config_profile_olt_g_id);

/**
 * @brief Set attribute for PON service config profile OLT g
 *
 * @param[in] pon_service_config_profile_olt_g_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_olt_g_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_olt_g_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile OLT g
 *
 * @param[in] pon_service_config_profile_olt_g_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_olt_g_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_olt_g_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile onu2 g
 */
typedef enum _sai_pon_service_config_profile_onu2_g_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Security Mode
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_SECURITY_MODE,

    /**
     * @brief Current Connectivity Mode
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_CURRENT_CONNECTIVITY_MODE,

    /**
     * @brief Priority Queue Scale Factor
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_PRIORITY_QUEUE_SCALE_FACTOR,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_onu2_g_attr_t;

/**
 * @brief Create PON service config profile onu2 g
 *
 * @param[out] pon_service_config_profile_onu2_g_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_onu2_g_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_onu2_g_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile onu2 g
 *
 * @param[in] pon_service_config_profile_onu2_g_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_onu2_g_fn)(
        _In_ sai_object_id_t pon_service_config_profile_onu2_g_id);

/**
 * @brief Set attribute for PON service config profile onu2 g
 *
 * @param[in] pon_service_config_profile_onu2_g_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_onu2_g_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_onu2_g_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile onu2 g
 *
 * @param[in] pon_service_config_profile_onu2_g_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_onu2_g_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_onu2_g_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile ONU g
 */
typedef enum _sai_pon_service_config_profile_onu_g_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Battery Backup
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_BATTERY_BACKUP,

    /**
     * @brief Administrative State
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_ADMINISTRATIVE_STATE,

    /**
     * @brief Credentials Status
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_CREDENTIALS_STATUS,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_onu_g_attr_t;

/**
 * @brief Create PON service config profile ONU g
 *
 * @param[out] pon_service_config_profile_onu_g_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_onu_g_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_onu_g_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile ONU g
 *
 * @param[in] pon_service_config_profile_onu_g_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_onu_g_fn)(
        _In_ sai_object_id_t pon_service_config_profile_onu_g_id);

/**
 * @brief Set attribute for PON service config profile ONU g
 *
 * @param[in] pon_service_config_profile_onu_g_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_onu_g_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_onu_g_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile ONU g
 *
 * @param[in] pon_service_config_profile_onu_g_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_onu_g_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_onu_g_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile ONU operational Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_onu_operational_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_OPERATIONAL_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_OPERATIONAL_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ONU_OPERATIONAL_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_OPERATIONAL_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_OPERATIONAL_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_OPERATIONAL_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_OPERATIONAL_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_OPERATIONAL_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_onu_operational_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile ONU operational Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_onu_operational_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_onu_operational_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_onu_operational_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile ONU operational Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_onu_operational_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_onu_operational_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_onu_operational_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile ONU operational Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_onu_operational_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_onu_operational_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_onu_operational_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile ONU operational Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_onu_operational_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_onu_operational_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_onu_operational_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile pptp rs232 rs485 uni
 */
typedef enum _sai_pon_service_config_profile_pptp_rs232_rs485_uni_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Administrative State
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_ADMINISTRATIVE_STATE,

    /**
     * @brief Port Mode
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_PORT_MODE,

    /**
     * @brief Baud Rate
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_BAUD_RATE,

    /**
     * @brief Data Bits
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_DATA_BITS,

    /**
     * @brief Parity
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_PARITY,

    /**
     * @brief Stop Bits
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_STOP_BITS,

    /**
     * @brief Flow Control
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_FLOW_CONTROL,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_pptp_rs232_rs485_uni_attr_t;

/**
 * @brief Create PON service config profile pptp rs232 rs485 uni
 *
 * @param[out] pon_service_config_profile_pptp_rs232_rs485_uni_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_pptp_rs232_rs485_uni_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_pptp_rs232_rs485_uni_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile pptp rs232 rs485 uni
 *
 * @param[in] pon_service_config_profile_pptp_rs232_rs485_uni_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_pptp_rs232_rs485_uni_fn)(
        _In_ sai_object_id_t pon_service_config_profile_pptp_rs232_rs485_uni_id);

/**
 * @brief Set attribute for PON service config profile pptp rs232 rs485 uni
 *
 * @param[in] pon_service_config_profile_pptp_rs232_rs485_uni_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_pptp_rs232_rs485_uni_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_pptp_rs232_rs485_uni_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile pptp rs232 rs485 uni
 *
 * @param[in] pon_service_config_profile_pptp_rs232_rs485_uni_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_pptp_rs232_rs485_uni_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_pptp_rs232_rs485_uni_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile priority queue
 */
typedef enum _sai_pon_service_config_profile_priority_queue_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Allocated Queue Size
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_ALLOCATED_QUEUE_SIZE,

    /**
     * @brief Discard Block Counter Reset Interval
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_DISCARD_BLOCK_COUNTER_RESET_INTERVAL,

    /**
     * @brief Threshold Value For Discarded Blocks
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_THRESHOLD_VALUE_FOR_DISCARDED_BLOCKS,

    /**
     * @brief Related Port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_RELATED_PORT,

    /**
     * @brief Traffic Scheduler Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_TRAFFIC_SCHEDULER_POINTER,

    /**
     * @brief Weight
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_WEIGHT,

    /**
     * @brief Back Pressure Operation
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_BACK_PRESSURE_OPERATION,

    /**
     * @brief Back Pressure Time
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_BACK_PRESSURE_TIME,

    /**
     * @brief Back Pressure Occur Queue Threshold
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_BACK_PRESSURE_OCCUR_QUEUE_THRESHOLD,

    /**
     * @brief Back Pressure Clear Queue Threshold
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_BACK_PRESSURE_CLEAR_QUEUE_THRESHOLD,

    /**
     * @brief Packet Drop Max P
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_PACKET_DROP_MAX_P,

    /**
     * @brief Queue Drop W Q
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_QUEUE_DROP_W_Q,

    /**
     * @brief Drop Precedence Colour Marking
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_DROP_PRECEDENCE_COLOUR_MARKING,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_priority_queue_attr_t;

/**
 * @brief Create PON service config profile priority queue
 *
 * @param[out] pon_service_config_profile_priority_queue_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_priority_queue_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_priority_queue_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile priority queue
 *
 * @param[in] pon_service_config_profile_priority_queue_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_priority_queue_fn)(
        _In_ sai_object_id_t pon_service_config_profile_priority_queue_id);

/**
 * @brief Set attribute for PON service config profile priority queue
 *
 * @param[in] pon_service_config_profile_priority_queue_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_priority_queue_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_priority_queue_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile priority queue
 *
 * @param[in] pon_service_config_profile_priority_queue_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_priority_queue_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_priority_queue_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile priority queue packet drop queue threshold
 */
typedef enum _sai_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Min Green
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_MIN_GREEN,

    /**
     * @brief Max Green
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_MAX_GREEN,

    /**
     * @brief Min Yellow
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_MIN_YELLOW,

    /**
     * @brief Max Yellow
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_MAX_YELLOW,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_attr_t;

/**
 * @brief Create PON service config profile priority queue packet drop queue threshold
 *
 * @param[out] pon_service_config_profile_priority_queue_packet_drop_queue_threshold_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_priority_queue_packet_drop_queue_threshold_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile priority queue packet drop queue threshold
 *
 * @param[in] pon_service_config_profile_priority_queue_packet_drop_queue_threshold_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_fn)(
        _In_ sai_object_id_t pon_service_config_profile_priority_queue_packet_drop_queue_threshold_id);

/**
 * @brief Set attribute for PON service config profile priority queue packet drop queue threshold
 *
 * @param[in] pon_service_config_profile_priority_queue_packet_drop_queue_threshold_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_priority_queue_packet_drop_queue_threshold_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile priority queue packet drop queue threshold
 *
 * @param[in] pon_service_config_profile_priority_queue_packet_drop_queue_threshold_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_priority_queue_packet_drop_queue_threshold_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile rs232 rs485 Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile rs232 rs485 Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_rs232_rs485_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_rs232_rs485_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile rs232 rs485 Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_rs232_rs485_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_rs232_rs485_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile rs232 rs485 Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_rs232_rs485_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_rs232_rs485_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile rs232 rs485 Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_rs232_rs485_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_rs232_rs485_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile rs232 rs485 port Operational config data
 */
typedef enum _sai_pon_service_config_profile_rs232_rs485_port_oper_config_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief TCP UDP Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_TCP_UDP_PTR,

    /**
     * @brief Pptp Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_PPTP_PTR,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_rs232_rs485_port_oper_config_data_attr_t;

/**
 * @brief Create PON service config profile rs232 rs485 port Operational config data
 *
 * @param[out] pon_service_config_profile_rs232_rs485_port_oper_config_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_rs232_rs485_port_oper_config_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_rs232_rs485_port_oper_config_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile rs232 rs485 port Operational config data
 *
 * @param[in] pon_service_config_profile_rs232_rs485_port_oper_config_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_rs232_rs485_port_oper_config_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_rs232_rs485_port_oper_config_data_id);

/**
 * @brief Set attribute for PON service config profile rs232 rs485 port Operational config data
 *
 * @param[in] pon_service_config_profile_rs232_rs485_port_oper_config_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_rs232_rs485_port_oper_config_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_rs232_rs485_port_oper_config_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile rs232 rs485 port Operational config data
 *
 * @param[in] pon_service_config_profile_rs232_rs485_port_oper_config_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_rs232_rs485_port_oper_config_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_rs232_rs485_port_oper_config_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile SSH server operation
 */
typedef enum _sai_pon_service_config_profile_ssh_server_operation_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_OPERATION_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_OPERATION_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_OPERATION_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_OPERATION_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Server Action
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_OPERATION_ATTR_SERVER_ACTION,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_OPERATION_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_OPERATION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_OPERATION_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ssh_server_operation_attr_t;

/**
 * @brief Create PON service config profile SSH server operation
 *
 * @param[out] pon_service_config_profile_ssh_server_operation_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ssh_server_operation_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ssh_server_operation_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile SSH server operation
 *
 * @param[in] pon_service_config_profile_ssh_server_operation_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ssh_server_operation_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ssh_server_operation_id);

/**
 * @brief Set attribute for PON service config profile SSH server operation
 *
 * @param[in] pon_service_config_profile_ssh_server_operation_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ssh_server_operation_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ssh_server_operation_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile SSH server operation
 *
 * @param[in] pon_service_config_profile_ssh_server_operation_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ssh_server_operation_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ssh_server_operation_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile SSH server port data
 */
typedef enum _sai_pon_service_config_profile_ssh_server_port_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief TCP UDP Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_TCP_UDP_PTR,

    /**
     * @brief SSH Server Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_SSH_SERVER_PTR,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_ssh_server_port_data_attr_t;

/**
 * @brief Create PON service config profile SSH server port data
 *
 * @param[out] pon_service_config_profile_ssh_server_port_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_ssh_server_port_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_ssh_server_port_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile SSH server port data
 *
 * @param[in] pon_service_config_profile_ssh_server_port_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_ssh_server_port_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ssh_server_port_data_id);

/**
 * @brief Set attribute for PON service config profile SSH server port data
 *
 * @param[in] pon_service_config_profile_ssh_server_port_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_ssh_server_port_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ssh_server_port_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile SSH server port data
 *
 * @param[in] pon_service_config_profile_ssh_server_port_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_ssh_server_port_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_ssh_server_port_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile TCONT
 */
typedef enum _sai_pon_service_config_profile_tcont_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Allocation ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_ALLOC_ID,

    /**
     * @brief Policy
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_POLICY,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_tcont_attr_t;

/**
 * @brief Create PON service config profile TCONT
 *
 * @param[out] pon_service_config_profile_tcont_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_tcont_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_tcont_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile TCONT
 *
 * @param[in] pon_service_config_profile_tcont_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_tcont_fn)(
        _In_ sai_object_id_t pon_service_config_profile_tcont_id);

/**
 * @brief Set attribute for PON service config profile TCONT
 *
 * @param[in] pon_service_config_profile_tcont_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_tcont_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_tcont_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile TCONT
 *
 * @param[in] pon_service_config_profile_tcont_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_tcont_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_tcont_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile TCP UDP config data
 */
typedef enum _sai_pon_service_config_profile_tcp_udp_config_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Port ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_PORT_ID,

    /**
     * @brief Protocol
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_PROTOCOL,

    /**
     * @brief TOS Diffserv Field
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_TOS_DIFFSERV_FIELD,

    /**
     * @brief IP Host Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_IP_HOST_PTR,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_tcp_udp_config_data_attr_t;

/**
 * @brief Create PON service config profile TCP UDP config data
 *
 * @param[out] pon_service_config_profile_tcp_udp_config_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_tcp_udp_config_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_tcp_udp_config_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile TCP UDP config data
 *
 * @param[in] pon_service_config_profile_tcp_udp_config_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_tcp_udp_config_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_tcp_udp_config_data_id);

/**
 * @brief Set attribute for PON service config profile TCP UDP config data
 *
 * @param[in] pon_service_config_profile_tcp_udp_config_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_tcp_udp_config_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_tcp_udp_config_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile TCP UDP config data
 *
 * @param[in] pon_service_config_profile_tcp_udp_config_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_tcp_udp_config_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_tcp_udp_config_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile TCP UDP Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_tcp_udp_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_tcp_udp_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile TCP UDP Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_tcp_udp_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_tcp_udp_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_tcp_udp_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile TCP UDP Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_tcp_udp_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_tcp_udp_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_tcp_udp_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile TCP UDP Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_tcp_udp_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_tcp_udp_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_tcp_udp_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile TCP UDP Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_tcp_udp_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_tcp_udp_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_tcp_udp_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile threshold data64 bit
 */
typedef enum _sai_pon_service_config_profile_threshold_data64_bit_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Value 1
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_1,

    /**
     * @brief Threshold Value 2
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_2,

    /**
     * @brief Threshold Value 3
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_3,

    /**
     * @brief Threshold Value 4
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_4,

    /**
     * @brief Threshold Value 5
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_5,

    /**
     * @brief Threshold Value 6
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_6,

    /**
     * @brief Threshold Value 7
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_7,

    /**
     * @brief Threshold Value 8
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_8,

    /**
     * @brief Threshold Value 9
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_9,

    /**
     * @brief Threshold Value 10
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_10,

    /**
     * @brief Threshold Value 11
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_11,

    /**
     * @brief Threshold Value 12
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_12,

    /**
     * @brief Threshold Value 13
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_13,

    /**
     * @brief Threshold Value 14
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_14,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_threshold_data64_bit_attr_t;

/**
 * @brief Create PON service config profile threshold data64 bit
 *
 * @param[out] pon_service_config_profile_threshold_data64_bit_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_threshold_data64_bit_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_threshold_data64_bit_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile threshold data64 bit
 *
 * @param[in] pon_service_config_profile_threshold_data64_bit_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_threshold_data64_bit_fn)(
        _In_ sai_object_id_t pon_service_config_profile_threshold_data64_bit_id);

/**
 * @brief Set attribute for PON service config profile threshold data64 bit
 *
 * @param[in] pon_service_config_profile_threshold_data64_bit_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_threshold_data64_bit_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_threshold_data64_bit_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile threshold data64 bit
 *
 * @param[in] pon_service_config_profile_threshold_data64_bit_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_threshold_data64_bit_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_threshold_data64_bit_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile threshold data one
 */
typedef enum _sai_pon_service_config_profile_threshold_data_one_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Value 1
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_1,

    /**
     * @brief Threshold Value 2
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_2,

    /**
     * @brief Threshold Value 3
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_3,

    /**
     * @brief Threshold Value 4
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_4,

    /**
     * @brief Threshold Value 5
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_5,

    /**
     * @brief Threshold Value 6
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_6,

    /**
     * @brief Threshold Value 7
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_7,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_threshold_data_one_attr_t;

/**
 * @brief Create PON service config profile threshold data one
 *
 * @param[out] pon_service_config_profile_threshold_data_one_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_threshold_data_one_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_threshold_data_one_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile threshold data one
 *
 * @param[in] pon_service_config_profile_threshold_data_one_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_threshold_data_one_fn)(
        _In_ sai_object_id_t pon_service_config_profile_threshold_data_one_id);

/**
 * @brief Set attribute for PON service config profile threshold data one
 *
 * @param[in] pon_service_config_profile_threshold_data_one_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_threshold_data_one_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_threshold_data_one_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile threshold data one
 *
 * @param[in] pon_service_config_profile_threshold_data_one_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_threshold_data_one_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_threshold_data_one_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile threshold data two
 */
typedef enum _sai_pon_service_config_profile_threshold_data_two_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Value 8
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_8,

    /**
     * @brief Threshold Value 9
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_9,

    /**
     * @brief Threshold Value 10
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_10,

    /**
     * @brief Threshold Value 11
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_11,

    /**
     * @brief Threshold Value 12
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_12,

    /**
     * @brief Threshold Value 13
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_13,

    /**
     * @brief Threshold Value 14
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_14,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_threshold_data_two_attr_t;

/**
 * @brief Create PON service config profile threshold data two
 *
 * @param[out] pon_service_config_profile_threshold_data_two_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_threshold_data_two_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_threshold_data_two_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile threshold data two
 *
 * @param[in] pon_service_config_profile_threshold_data_two_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_threshold_data_two_fn)(
        _In_ sai_object_id_t pon_service_config_profile_threshold_data_two_id);

/**
 * @brief Set attribute for PON service config profile threshold data two
 *
 * @param[in] pon_service_config_profile_threshold_data_two_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_threshold_data_two_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_threshold_data_two_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile threshold data two
 *
 * @param[in] pon_service_config_profile_threshold_data_two_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_threshold_data_two_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_threshold_data_two_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile traffic descriptor
 */
typedef enum _sai_pon_service_config_profile_traffic_descriptor_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief CIR
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_CIR,

    /**
     * @brief PIR
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_PIR,

    /**
     * @brief CBS
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_CBS,

    /**
     * @brief PBS
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_PBS,

    /**
     * @brief Colour Mode
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_COLOUR_MODE,

    /**
     * @brief Ingress Colour Marking
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_INGRESS_COLOUR_MARKING,

    /**
     * @brief Egress Colour Marking
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_EGRESS_COLOUR_MARKING,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_traffic_descriptor_attr_t;

/**
 * @brief Create PON service config profile traffic descriptor
 *
 * @param[out] pon_service_config_profile_traffic_descriptor_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_traffic_descriptor_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_traffic_descriptor_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile traffic descriptor
 *
 * @param[in] pon_service_config_profile_traffic_descriptor_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_traffic_descriptor_fn)(
        _In_ sai_object_id_t pon_service_config_profile_traffic_descriptor_id);

/**
 * @brief Set attribute for PON service config profile traffic descriptor
 *
 * @param[in] pon_service_config_profile_traffic_descriptor_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_traffic_descriptor_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_traffic_descriptor_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile traffic descriptor
 *
 * @param[in] pon_service_config_profile_traffic_descriptor_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_traffic_descriptor_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_traffic_descriptor_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile traffic scheduler
 */
typedef enum _sai_pon_service_config_profile_traffic_scheduler_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief TCONT Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_TCONT_POINTER,

    /**
     * @brief Policy
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_POLICY,

    /**
     * @brief Priority Weight
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_PRIORITY_WEIGHT,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_traffic_scheduler_attr_t;

/**
 * @brief Create PON service config profile traffic scheduler
 *
 * @param[out] pon_service_config_profile_traffic_scheduler_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_traffic_scheduler_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_traffic_scheduler_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile traffic scheduler
 *
 * @param[in] pon_service_config_profile_traffic_scheduler_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_traffic_scheduler_fn)(
        _In_ sai_object_id_t pon_service_config_profile_traffic_scheduler_id);

/**
 * @brief Set attribute for PON service config profile traffic scheduler
 *
 * @param[in] pon_service_config_profile_traffic_scheduler_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_traffic_scheduler_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_traffic_scheduler_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile traffic scheduler
 *
 * @param[in] pon_service_config_profile_traffic_scheduler_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_traffic_scheduler_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_traffic_scheduler_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile virtual ethernet interface pt
 */
typedef enum _sai_pon_service_config_profile_virtual_ethernet_interface_pt_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Administrative State
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_ADMINISTRATIVE_STATE,

    /**
     * @brief Interdomain Service Config Profile Name
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_INTERDOMAIN_SERVICE_CONFIG_PROFILE_NAME,

    /**
     * @brief TCP UDP Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_TCP_UDP_POINTER,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_virtual_ethernet_interface_pt_attr_t;

/**
 * @brief Create PON service config profile virtual ethernet interface pt
 *
 * @param[out] pon_service_config_profile_virtual_ethernet_interface_pt_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_virtual_ethernet_interface_pt_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_virtual_ethernet_interface_pt_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile virtual ethernet interface pt
 *
 * @param[in] pon_service_config_profile_virtual_ethernet_interface_pt_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_virtual_ethernet_interface_pt_fn)(
        _In_ sai_object_id_t pon_service_config_profile_virtual_ethernet_interface_pt_id);

/**
 * @brief Set attribute for PON service config profile virtual ethernet interface pt
 *
 * @param[in] pon_service_config_profile_virtual_ethernet_interface_pt_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_virtual_ethernet_interface_pt_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_virtual_ethernet_interface_pt_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile virtual ethernet interface pt
 *
 * @param[in] pon_service_config_profile_virtual_ethernet_interface_pt_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_virtual_ethernet_interface_pt_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_virtual_ethernet_interface_pt_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile vlan tagging filter data
 */
typedef enum _sai_pon_service_config_profile_vlan_tagging_filter_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Forward Operation
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_FORWARD_OPERATION,

    /**
     * @brief Number Of Entries
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_NUMBER_OF_ENTRIES,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_vlan_tagging_filter_data_attr_t;

/**
 * @brief Create PON service config profile vlan tagging filter data
 *
 * @param[out] pon_service_config_profile_vlan_tagging_filter_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_vlan_tagging_filter_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_vlan_tagging_filter_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile vlan tagging filter data
 *
 * @param[in] pon_service_config_profile_vlan_tagging_filter_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_vlan_tagging_filter_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_vlan_tagging_filter_data_id);

/**
 * @brief Set attribute for PON service config profile vlan tagging filter data
 *
 * @param[in] pon_service_config_profile_vlan_tagging_filter_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_vlan_tagging_filter_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_vlan_tagging_filter_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile vlan tagging filter data
 *
 * @param[in] pon_service_config_profile_vlan_tagging_filter_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_vlan_tagging_filter_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_vlan_tagging_filter_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile vlan tagging filter data vlan filter
 */
typedef enum _sai_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_ID,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Vlan Filter Entry ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_VLAN_FILTER_ENTRY_ID,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_VLAN_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_attr_t;

/**
 * @brief Create PON service config profile vlan tagging filter data vlan filter
 *
 * @param[out] pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile vlan tagging filter data vlan filter
 *
 * @param[in] pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_fn)(
        _In_ sai_object_id_t pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_id);

/**
 * @brief Set attribute for PON service config profile vlan tagging filter data vlan filter
 *
 * @param[in] pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile vlan tagging filter data vlan filter
 *
 * @param[in] pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile XG PON downstream Management Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_DOWNSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_DOWNSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_DOWNSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_DOWNSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_DOWNSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_DOWNSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_DOWNSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_DOWNSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile XG PON downstream Management Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile XG PON downstream Management Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile XG PON downstream Management Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile XG PON downstream Management Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile XG PON TC Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_TC_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_TC_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_TC_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_TC_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_TC_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_TC_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_TC_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_TC_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile XG PON TC Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile XG PON TC Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile XG PON TC Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile XG PON TC Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON service config profile XG PON upstream Management Performance Monitoring History data
 */
typedef enum _sai_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_UPSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Service Config Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_UPSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_UPSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_UPSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_UPSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_1_2_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_UPSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_UPSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_UPSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_attr_t;

/**
 * @brief Create PON service config profile XG PON upstream Management Performance Monitoring History data
 *
 * @param[out] pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_fn)(
        _Out_ sai_object_id_t *pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON service config profile XG PON upstream Management Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_fn)(
        _In_ sai_object_id_t pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_id);

/**
 * @brief Set attribute for PON service config profile XG PON upstream Management Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON service config profile XG PON upstream Management Performance Monitoring History data
 *
 * @param[in] pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON SLA profile
 */
typedef enum _sai_pon_sla_profile_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SLA_PROFILE_ATTR_START,

    /**
     * @brief SLA Profile Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_ATTR_SLA_PROFILE_NAME = SAI_PON_SLA_PROFILE_ATTR_START,

    /**
     * @brief Downstream Guaranteed Rate
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_ATTR_DOWNSTREAM_GUARANTEED_RATE,

    /**
     * @brief Downstream Guaranteed Maximum Burst
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_ATTR_DOWNSTREAM_GUARANTEED_MAXIMUM_BURST,

    /**
     * @brief Downstream Best Effort Rate
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_ATTR_DOWNSTREAM_BEST_EFFORT_RATE,

    /**
     * @brief Downstream Best Effort Maximum Burst
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_ATTR_DOWNSTREAM_BEST_EFFORT_MAXIMUM_BURST,

    /**
     * @brief Upstream Fixed Rate
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_FIXED_RATE,

    /**
     * @brief Upstream Guaranteed Rate
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_GUARANTEED_RATE,

    /**
     * @brief Upstream Guaranteed Maximum Burst
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_GUARANTEED_MAXIMUM_BURST,

    /**
     * @brief Upstream Priority
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_PRIORITY,

    /**
     * @brief Upstream Best Effort Rate
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_BEST_EFFORT_RATE,

    /**
     * @brief Upstream Best Effort Maximum Burst
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_BEST_EFFORT_MAXIMUM_BURST,

    /**
     * @brief Upstream Best Effort Priority
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_BEST_EFFORT_PRIORITY,

    /**
     * @brief End of attributes
     */
    SAI_PON_SLA_PROFILE_ATTR_END,

    /** Custom range base value */
    SAI_PON_SLA_PROFILE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SLA_PROFILE_ATTR_CUSTOM_RANGE_END,

} sai_pon_sla_profile_attr_t;

/**
 * @brief Create PON SLA profile
 *
 * @param[out] pon_sla_profile_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_sla_profile_fn)(
        _Out_ sai_object_id_t *pon_sla_profile_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON SLA profile
 *
 * @param[in] pon_sla_profile_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_sla_profile_fn)(
        _In_ sai_object_id_t pon_sla_profile_id);

/**
 * @brief Set attribute for PON SLA profile
 *
 * @param[in] pon_sla_profile_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_sla_profile_attribute_fn)(
        _In_ sai_object_id_t pon_sla_profile_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON SLA profile
 *
 * @param[in] pon_sla_profile_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_sla_profile_attribute_fn)(
        _In_ sai_object_id_t pon_sla_profile_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON SLA profile controller
 */
typedef enum _sai_pon_sla_profile_controller_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_SLA_PROFILE_CONTROLLER_ATTR_START,

    /**
     * @brief Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_SLA_PROFILE_CONTROLLER_ATTR_NAME = SAI_PON_SLA_PROFILE_CONTROLLER_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_PON_SLA_PROFILE_CONTROLLER_ATTR_END,

    /** Custom range base value */
    SAI_PON_SLA_PROFILE_CONTROLLER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_SLA_PROFILE_CONTROLLER_ATTR_CUSTOM_RANGE_END,

} sai_pon_sla_profile_controller_attr_t;

/**
 * @brief Create PON SLA profile controller
 *
 * @param[out] pon_sla_profile_controller_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_sla_profile_controller_fn)(
        _Out_ sai_object_id_t *pon_sla_profile_controller_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON SLA profile controller
 *
 * @param[in] pon_sla_profile_controller_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_sla_profile_controller_fn)(
        _In_ sai_object_id_t pon_sla_profile_controller_id);

/**
 * @brief Set attribute for PON SLA profile controller
 *
 * @param[in] pon_sla_profile_controller_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_sla_profile_controller_attribute_fn)(
        _In_ sai_object_id_t pon_sla_profile_controller_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON SLA profile controller
 *
 * @param[in] pon_sla_profile_controller_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_sla_profile_controller_attribute_fn)(
        _In_ sai_object_id_t pon_sla_profile_controller_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_pon_service_config_profile_api_t
{
    sai_create_pon_service_config_profile_header_fn                                                                                   create_pon_service_config_profile_header;
    sai_remove_pon_service_config_profile_header_fn                                                                                   remove_pon_service_config_profile_header;
    sai_set_pon_service_config_profile_header_attribute_fn                                                                            set_pon_service_config_profile_header_attribute;
    sai_get_pon_service_config_profile_header_attribute_fn                                                                            get_pon_service_config_profile_header_attribute;
    sai_create_pon_service_config_profile_header_compatibility_vendor_model_fn                                                        create_pon_service_config_profile_header_compatibility_vendor_model;
    sai_remove_pon_service_config_profile_header_compatibility_vendor_model_fn                                                        remove_pon_service_config_profile_header_compatibility_vendor_model;
    sai_set_pon_service_config_profile_header_compatibility_vendor_model_attribute_fn                                                 set_pon_service_config_profile_header_compatibility_vendor_model_attribute;
    sai_get_pon_service_config_profile_header_compatibility_vendor_model_attribute_fn                                                 get_pon_service_config_profile_header_compatibility_vendor_model_attribute;
    sai_create_pon_service_config_profile_header_inputs_ext_input_fn                                                                  create_pon_service_config_profile_header_inputs_ext_input;
    sai_remove_pon_service_config_profile_header_inputs_ext_input_fn                                                                  remove_pon_service_config_profile_header_inputs_ext_input;
    sai_set_pon_service_config_profile_header_inputs_ext_input_attribute_fn                                                           set_pon_service_config_profile_header_inputs_ext_input_attribute;
    sai_get_pon_service_config_profile_header_inputs_ext_input_attribute_fn                                                           get_pon_service_config_profile_header_inputs_ext_input_attribute;
    sai_create_pon_service_config_profile_ani_g_fn                                                                                    create_pon_service_config_profile_ani_g;
    sai_remove_pon_service_config_profile_ani_g_fn                                                                                    remove_pon_service_config_profile_ani_g;
    sai_set_pon_service_config_profile_ani_g_attribute_fn                                                                             set_pon_service_config_profile_ani_g_attribute;
    sai_get_pon_service_config_profile_ani_g_attribute_fn                                                                             get_pon_service_config_profile_ani_g_attribute;
    sai_create_pon_service_config_profile_cardholder_fn                                                                               create_pon_service_config_profile_cardholder;
    sai_remove_pon_service_config_profile_cardholder_fn                                                                               remove_pon_service_config_profile_cardholder;
    sai_set_pon_service_config_profile_cardholder_attribute_fn                                                                        set_pon_service_config_profile_cardholder_attribute;
    sai_get_pon_service_config_profile_cardholder_attribute_fn                                                                        get_pon_service_config_profile_cardholder_attribute;
    sai_create_pon_service_config_profile_circuit_pack_fn                                                                             create_pon_service_config_profile_circuit_pack;
    sai_remove_pon_service_config_profile_circuit_pack_fn                                                                             remove_pon_service_config_profile_circuit_pack;
    sai_set_pon_service_config_profile_circuit_pack_attribute_fn                                                                      set_pon_service_config_profile_circuit_pack_attribute;
    sai_get_pon_service_config_profile_circuit_pack_attribute_fn                                                                      get_pon_service_config_profile_circuit_pack_attribute;
    sai_create_pon_service_config_profile_enhanced_fec_pm_hist_data_fn                                                                create_pon_service_config_profile_enhanced_fec_pm_hist_data;
    sai_remove_pon_service_config_profile_enhanced_fec_pm_hist_data_fn                                                                remove_pon_service_config_profile_enhanced_fec_pm_hist_data;
    sai_set_pon_service_config_profile_enhanced_fec_pm_hist_data_attribute_fn                                                         set_pon_service_config_profile_enhanced_fec_pm_hist_data_attribute;
    sai_get_pon_service_config_profile_enhanced_fec_pm_hist_data_attribute_fn                                                         get_pon_service_config_profile_enhanced_fec_pm_hist_data_attribute;
    sai_create_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_fn                                                           create_pon_service_config_profile_enhanced_tc_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_fn                                                           remove_pon_service_config_profile_enhanced_tc_perf_mon_hist_data;
    sai_set_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_attribute_fn                                                    set_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_attribute_fn                                                    get_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_ethernet_frame_extended_pm_fn                                                               create_pon_service_config_profile_ethernet_frame_extended_pm;
    sai_remove_pon_service_config_profile_ethernet_frame_extended_pm_fn                                                               remove_pon_service_config_profile_ethernet_frame_extended_pm;
    sai_set_pon_service_config_profile_ethernet_frame_extended_pm_attribute_fn                                                        set_pon_service_config_profile_ethernet_frame_extended_pm_attribute;
    sai_get_pon_service_config_profile_ethernet_frame_extended_pm_attribute_fn                                                        get_pon_service_config_profile_ethernet_frame_extended_pm_attribute;
    sai_create_pon_service_config_profile_ethernet_frame_extended_pm64_bit_fn                                                         create_pon_service_config_profile_ethernet_frame_extended_pm64_bit;
    sai_remove_pon_service_config_profile_ethernet_frame_extended_pm64_bit_fn                                                         remove_pon_service_config_profile_ethernet_frame_extended_pm64_bit;
    sai_set_pon_service_config_profile_ethernet_frame_extended_pm64_bit_attribute_fn                                                  set_pon_service_config_profile_ethernet_frame_extended_pm64_bit_attribute;
    sai_get_pon_service_config_profile_ethernet_frame_extended_pm64_bit_attribute_fn                                                  get_pon_service_config_profile_ethernet_frame_extended_pm64_bit_attribute;
    sai_create_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_fn                                             create_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream;
    sai_remove_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_fn                                             remove_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream;
    sai_set_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_attribute_fn                                      set_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_attribute;
    sai_get_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_attribute_fn                                      get_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_attribute;
    sai_create_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_fn                                               create_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream;
    sai_remove_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_fn                                               remove_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream;
    sai_set_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_attribute_fn                                        set_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_attribute;
    sai_get_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_attribute_fn                                        get_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_attribute;
    sai_create_pon_service_config_profile_ethernet_perf_mon_hist_data_fn                                                              create_pon_service_config_profile_ethernet_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_ethernet_perf_mon_hist_data_fn                                                              remove_pon_service_config_profile_ethernet_perf_mon_hist_data;
    sai_set_pon_service_config_profile_ethernet_perf_mon_hist_data_attribute_fn                                                       set_pon_service_config_profile_ethernet_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_ethernet_perf_mon_hist_data_attribute_fn                                                       get_pon_service_config_profile_ethernet_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_ethernet_perf_mon_hist_data3_fn                                                             create_pon_service_config_profile_ethernet_perf_mon_hist_data3;
    sai_remove_pon_service_config_profile_ethernet_perf_mon_hist_data3_fn                                                             remove_pon_service_config_profile_ethernet_perf_mon_hist_data3;
    sai_set_pon_service_config_profile_ethernet_perf_mon_hist_data3_attribute_fn                                                      set_pon_service_config_profile_ethernet_perf_mon_hist_data3_attribute;
    sai_get_pon_service_config_profile_ethernet_perf_mon_hist_data3_attribute_fn                                                      get_pon_service_config_profile_ethernet_perf_mon_hist_data3_attribute;
    sai_create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_fn                                              create_pon_service_config_profile_extended_vlan_tagging_operation_config_data;
    sai_remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_fn                                              remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data;
    sai_set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_attribute_fn                                       set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_attribute;
    sai_get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_attribute_fn                                       get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_attribute;
    sai_create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_fn        create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation;
    sai_remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_fn        remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation;
    sai_set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_attribute_fn set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_attribute;
    sai_get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_attribute_fn get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_attribute;
    sai_create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_fn                         create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping;
    sai_remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_fn                         remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping;
    sai_set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_attribute_fn                  set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_attribute;
    sai_get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_attribute_fn                  get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_attribute;
    sai_create_pon_service_config_profile_fec_perf_mon_hist_data_fn                                                                   create_pon_service_config_profile_fec_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_fec_perf_mon_hist_data_fn                                                                   remove_pon_service_config_profile_fec_perf_mon_hist_data;
    sai_set_pon_service_config_profile_fec_perf_mon_hist_data_attribute_fn                                                            set_pon_service_config_profile_fec_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_fec_perf_mon_hist_data_attribute_fn                                                            get_pon_service_config_profile_fec_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_fn                                                          create_pon_service_config_profile_gal_ethernet_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_fn                                                          remove_pon_service_config_profile_gal_ethernet_perf_mon_hist_data;
    sai_set_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_attribute_fn                                                   set_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_attribute_fn                                                   get_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_gal_ethernet_profile_fn                                                                     create_pon_service_config_profile_gal_ethernet_profile;
    sai_remove_pon_service_config_profile_gal_ethernet_profile_fn                                                                     remove_pon_service_config_profile_gal_ethernet_profile;
    sai_set_pon_service_config_profile_gal_ethernet_profile_attribute_fn                                                              set_pon_service_config_profile_gal_ethernet_profile_attribute;
    sai_get_pon_service_config_profile_gal_ethernet_profile_attribute_fn                                                              get_pon_service_config_profile_gal_ethernet_profile_attribute;
    sai_create_pon_service_config_profile_gem_interworking_tp_fn                                                                      create_pon_service_config_profile_gem_interworking_tp;
    sai_remove_pon_service_config_profile_gem_interworking_tp_fn                                                                      remove_pon_service_config_profile_gem_interworking_tp;
    sai_set_pon_service_config_profile_gem_interworking_tp_attribute_fn                                                               set_pon_service_config_profile_gem_interworking_tp_attribute;
    sai_get_pon_service_config_profile_gem_interworking_tp_attribute_fn                                                               get_pon_service_config_profile_gem_interworking_tp_attribute;
    sai_create_pon_service_config_profile_gem_port_network_ctp_fn                                                                     create_pon_service_config_profile_gem_port_network_ctp;
    sai_remove_pon_service_config_profile_gem_port_network_ctp_fn                                                                     remove_pon_service_config_profile_gem_port_network_ctp;
    sai_set_pon_service_config_profile_gem_port_network_ctp_attribute_fn                                                              set_pon_service_config_profile_gem_port_network_ctp_attribute;
    sai_get_pon_service_config_profile_gem_port_network_ctp_attribute_fn                                                              get_pon_service_config_profile_gem_port_network_ctp_attribute;
    sai_create_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_fn                                                  create_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_fn                                                  remove_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data;
    sai_set_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_attribute_fn                                           set_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_attribute_fn                                           get_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_ieee8021p_mapper_service_profile_fn                                                         create_pon_service_config_profile_ieee8021p_mapper_service_profile;
    sai_remove_pon_service_config_profile_ieee8021p_mapper_service_profile_fn                                                         remove_pon_service_config_profile_ieee8021p_mapper_service_profile;
    sai_set_pon_service_config_profile_ieee8021p_mapper_service_profile_attribute_fn                                                  set_pon_service_config_profile_ieee8021p_mapper_service_profile_attribute;
    sai_get_pon_service_config_profile_ieee8021p_mapper_service_profile_attribute_fn                                                  get_pon_service_config_profile_ieee8021p_mapper_service_profile_attribute;
    sai_create_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_fn                                   create_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping;
    sai_remove_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_fn                                   remove_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping;
    sai_set_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_attribute_fn                            set_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_attribute;
    sai_get_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_attribute_fn                            get_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_attribute;
    sai_create_pon_service_config_profile_ip_host_config_data_fn                                                                      create_pon_service_config_profile_ip_host_config_data;
    sai_remove_pon_service_config_profile_ip_host_config_data_fn                                                                      remove_pon_service_config_profile_ip_host_config_data;
    sai_set_pon_service_config_profile_ip_host_config_data_attribute_fn                                                               set_pon_service_config_profile_ip_host_config_data_attribute;
    sai_get_pon_service_config_profile_ip_host_config_data_attribute_fn                                                               get_pon_service_config_profile_ip_host_config_data_attribute;
    sai_create_pon_service_config_profile_ip_host_perf_mon_hist_data_fn                                                               create_pon_service_config_profile_ip_host_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_ip_host_perf_mon_hist_data_fn                                                               remove_pon_service_config_profile_ip_host_perf_mon_hist_data;
    sai_set_pon_service_config_profile_ip_host_perf_mon_hist_data_attribute_fn                                                        set_pon_service_config_profile_ip_host_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_ip_host_perf_mon_hist_data_attribute_fn                                                        get_pon_service_config_profile_ip_host_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_ipv6_host_config_data_fn                                                                    create_pon_service_config_profile_ipv6_host_config_data;
    sai_remove_pon_service_config_profile_ipv6_host_config_data_fn                                                                    remove_pon_service_config_profile_ipv6_host_config_data;
    sai_set_pon_service_config_profile_ipv6_host_config_data_attribute_fn                                                             set_pon_service_config_profile_ipv6_host_config_data_attribute;
    sai_get_pon_service_config_profile_ipv6_host_config_data_attribute_fn                                                             get_pon_service_config_profile_ipv6_host_config_data_attribute;
    sai_create_pon_service_config_profile_large_string_fn                                                                             create_pon_service_config_profile_large_string;
    sai_remove_pon_service_config_profile_large_string_fn                                                                             remove_pon_service_config_profile_large_string;
    sai_set_pon_service_config_profile_large_string_attribute_fn                                                                      set_pon_service_config_profile_large_string_attribute;
    sai_get_pon_service_config_profile_large_string_attribute_fn                                                                      get_pon_service_config_profile_large_string_attribute;
    sai_create_pon_service_config_profile_mac_bridge_perf_mon_hist_data_fn                                                            create_pon_service_config_profile_mac_bridge_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_mac_bridge_perf_mon_hist_data_fn                                                            remove_pon_service_config_profile_mac_bridge_perf_mon_hist_data;
    sai_set_pon_service_config_profile_mac_bridge_perf_mon_hist_data_attribute_fn                                                     set_pon_service_config_profile_mac_bridge_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_mac_bridge_perf_mon_hist_data_attribute_fn                                                     get_pon_service_config_profile_mac_bridge_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_mac_bridge_port_config_data_fn                                                              create_pon_service_config_profile_mac_bridge_port_config_data;
    sai_remove_pon_service_config_profile_mac_bridge_port_config_data_fn                                                              remove_pon_service_config_profile_mac_bridge_port_config_data;
    sai_set_pon_service_config_profile_mac_bridge_port_config_data_attribute_fn                                                       set_pon_service_config_profile_mac_bridge_port_config_data_attribute;
    sai_get_pon_service_config_profile_mac_bridge_port_config_data_attribute_fn                                                       get_pon_service_config_profile_mac_bridge_port_config_data_attribute;
    sai_create_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_fn                                                       create_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_fn                                                       remove_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data;
    sai_set_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_attribute_fn                                                set_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_attribute_fn                                                get_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_mac_bridge_service_profile_fn                                                               create_pon_service_config_profile_mac_bridge_service_profile;
    sai_remove_pon_service_config_profile_mac_bridge_service_profile_fn                                                               remove_pon_service_config_profile_mac_bridge_service_profile;
    sai_set_pon_service_config_profile_mac_bridge_service_profile_attribute_fn                                                        set_pon_service_config_profile_mac_bridge_service_profile_attribute;
    sai_get_pon_service_config_profile_mac_bridge_service_profile_attribute_fn                                                        get_pon_service_config_profile_mac_bridge_service_profile_attribute;
    sai_create_pon_service_config_profile_multicast_gem_interworking_tp_fn                                                            create_pon_service_config_profile_multicast_gem_interworking_tp;
    sai_remove_pon_service_config_profile_multicast_gem_interworking_tp_fn                                                            remove_pon_service_config_profile_multicast_gem_interworking_tp;
    sai_set_pon_service_config_profile_multicast_gem_interworking_tp_attribute_fn                                                     set_pon_service_config_profile_multicast_gem_interworking_tp_attribute;
    sai_get_pon_service_config_profile_multicast_gem_interworking_tp_attribute_fn                                                     get_pon_service_config_profile_multicast_gem_interworking_tp_attribute;
    sai_create_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_fn                                     create_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address;
    sai_remove_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_fn                                     remove_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address;
    sai_set_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_attribute_fn                              set_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_attribute;
    sai_get_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_attribute_fn                              get_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_attribute;
    sai_create_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_fn                                     create_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address;
    sai_remove_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_fn                                     remove_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address;
    sai_set_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_attribute_fn                              set_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_attribute;
    sai_get_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_attribute_fn                              get_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_attribute;
    sai_create_pon_service_config_profile_olt_g_fn                                                                                    create_pon_service_config_profile_olt_g;
    sai_remove_pon_service_config_profile_olt_g_fn                                                                                    remove_pon_service_config_profile_olt_g;
    sai_set_pon_service_config_profile_olt_g_attribute_fn                                                                             set_pon_service_config_profile_olt_g_attribute;
    sai_get_pon_service_config_profile_olt_g_attribute_fn                                                                             get_pon_service_config_profile_olt_g_attribute;
    sai_create_pon_service_config_profile_onu2_g_fn                                                                                   create_pon_service_config_profile_onu2_g;
    sai_remove_pon_service_config_profile_onu2_g_fn                                                                                   remove_pon_service_config_profile_onu2_g;
    sai_set_pon_service_config_profile_onu2_g_attribute_fn                                                                            set_pon_service_config_profile_onu2_g_attribute;
    sai_get_pon_service_config_profile_onu2_g_attribute_fn                                                                            get_pon_service_config_profile_onu2_g_attribute;
    sai_create_pon_service_config_profile_onu_g_fn                                                                                    create_pon_service_config_profile_onu_g;
    sai_remove_pon_service_config_profile_onu_g_fn                                                                                    remove_pon_service_config_profile_onu_g;
    sai_set_pon_service_config_profile_onu_g_attribute_fn                                                                             set_pon_service_config_profile_onu_g_attribute;
    sai_get_pon_service_config_profile_onu_g_attribute_fn                                                                             get_pon_service_config_profile_onu_g_attribute;
    sai_create_pon_service_config_profile_onu_operational_perf_mon_hist_data_fn                                                       create_pon_service_config_profile_onu_operational_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_onu_operational_perf_mon_hist_data_fn                                                       remove_pon_service_config_profile_onu_operational_perf_mon_hist_data;
    sai_set_pon_service_config_profile_onu_operational_perf_mon_hist_data_attribute_fn                                                set_pon_service_config_profile_onu_operational_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_onu_operational_perf_mon_hist_data_attribute_fn                                                get_pon_service_config_profile_onu_operational_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_pptp_rs232_rs485_uni_fn                                                                     create_pon_service_config_profile_pptp_rs232_rs485_uni;
    sai_remove_pon_service_config_profile_pptp_rs232_rs485_uni_fn                                                                     remove_pon_service_config_profile_pptp_rs232_rs485_uni;
    sai_set_pon_service_config_profile_pptp_rs232_rs485_uni_attribute_fn                                                              set_pon_service_config_profile_pptp_rs232_rs485_uni_attribute;
    sai_get_pon_service_config_profile_pptp_rs232_rs485_uni_attribute_fn                                                              get_pon_service_config_profile_pptp_rs232_rs485_uni_attribute;
    sai_create_pon_service_config_profile_priority_queue_fn                                                                           create_pon_service_config_profile_priority_queue;
    sai_remove_pon_service_config_profile_priority_queue_fn                                                                           remove_pon_service_config_profile_priority_queue;
    sai_set_pon_service_config_profile_priority_queue_attribute_fn                                                                    set_pon_service_config_profile_priority_queue_attribute;
    sai_get_pon_service_config_profile_priority_queue_attribute_fn                                                                    get_pon_service_config_profile_priority_queue_attribute;
    sai_create_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_fn                                               create_pon_service_config_profile_priority_queue_packet_drop_queue_threshold;
    sai_remove_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_fn                                               remove_pon_service_config_profile_priority_queue_packet_drop_queue_threshold;
    sai_set_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_attribute_fn                                        set_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_attribute;
    sai_get_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_attribute_fn                                        get_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_attribute;
    sai_create_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_fn                                                           create_pon_service_config_profile_rs232_rs485_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_fn                                                           remove_pon_service_config_profile_rs232_rs485_perf_mon_hist_data;
    sai_set_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_attribute_fn                                                    set_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_attribute_fn                                                    get_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_rs232_rs485_port_oper_config_data_fn                                                        create_pon_service_config_profile_rs232_rs485_port_oper_config_data;
    sai_remove_pon_service_config_profile_rs232_rs485_port_oper_config_data_fn                                                        remove_pon_service_config_profile_rs232_rs485_port_oper_config_data;
    sai_set_pon_service_config_profile_rs232_rs485_port_oper_config_data_attribute_fn                                                 set_pon_service_config_profile_rs232_rs485_port_oper_config_data_attribute;
    sai_get_pon_service_config_profile_rs232_rs485_port_oper_config_data_attribute_fn                                                 get_pon_service_config_profile_rs232_rs485_port_oper_config_data_attribute;
    sai_create_pon_service_config_profile_ssh_server_operation_fn                                                                     create_pon_service_config_profile_ssh_server_operation;
    sai_remove_pon_service_config_profile_ssh_server_operation_fn                                                                     remove_pon_service_config_profile_ssh_server_operation;
    sai_set_pon_service_config_profile_ssh_server_operation_attribute_fn                                                              set_pon_service_config_profile_ssh_server_operation_attribute;
    sai_get_pon_service_config_profile_ssh_server_operation_attribute_fn                                                              get_pon_service_config_profile_ssh_server_operation_attribute;
    sai_create_pon_service_config_profile_ssh_server_port_data_fn                                                                     create_pon_service_config_profile_ssh_server_port_data;
    sai_remove_pon_service_config_profile_ssh_server_port_data_fn                                                                     remove_pon_service_config_profile_ssh_server_port_data;
    sai_set_pon_service_config_profile_ssh_server_port_data_attribute_fn                                                              set_pon_service_config_profile_ssh_server_port_data_attribute;
    sai_get_pon_service_config_profile_ssh_server_port_data_attribute_fn                                                              get_pon_service_config_profile_ssh_server_port_data_attribute;
    sai_create_pon_service_config_profile_tcont_fn                                                                                    create_pon_service_config_profile_tcont;
    sai_remove_pon_service_config_profile_tcont_fn                                                                                    remove_pon_service_config_profile_tcont;
    sai_set_pon_service_config_profile_tcont_attribute_fn                                                                             set_pon_service_config_profile_tcont_attribute;
    sai_get_pon_service_config_profile_tcont_attribute_fn                                                                             get_pon_service_config_profile_tcont_attribute;
    sai_create_pon_service_config_profile_tcp_udp_config_data_fn                                                                      create_pon_service_config_profile_tcp_udp_config_data;
    sai_remove_pon_service_config_profile_tcp_udp_config_data_fn                                                                      remove_pon_service_config_profile_tcp_udp_config_data;
    sai_set_pon_service_config_profile_tcp_udp_config_data_attribute_fn                                                               set_pon_service_config_profile_tcp_udp_config_data_attribute;
    sai_get_pon_service_config_profile_tcp_udp_config_data_attribute_fn                                                               get_pon_service_config_profile_tcp_udp_config_data_attribute;
    sai_create_pon_service_config_profile_tcp_udp_perf_mon_hist_data_fn                                                               create_pon_service_config_profile_tcp_udp_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_tcp_udp_perf_mon_hist_data_fn                                                               remove_pon_service_config_profile_tcp_udp_perf_mon_hist_data;
    sai_set_pon_service_config_profile_tcp_udp_perf_mon_hist_data_attribute_fn                                                        set_pon_service_config_profile_tcp_udp_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_tcp_udp_perf_mon_hist_data_attribute_fn                                                        get_pon_service_config_profile_tcp_udp_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_threshold_data64_bit_fn                                                                     create_pon_service_config_profile_threshold_data64_bit;
    sai_remove_pon_service_config_profile_threshold_data64_bit_fn                                                                     remove_pon_service_config_profile_threshold_data64_bit;
    sai_set_pon_service_config_profile_threshold_data64_bit_attribute_fn                                                              set_pon_service_config_profile_threshold_data64_bit_attribute;
    sai_get_pon_service_config_profile_threshold_data64_bit_attribute_fn                                                              get_pon_service_config_profile_threshold_data64_bit_attribute;
    sai_create_pon_service_config_profile_threshold_data_one_fn                                                                       create_pon_service_config_profile_threshold_data_one;
    sai_remove_pon_service_config_profile_threshold_data_one_fn                                                                       remove_pon_service_config_profile_threshold_data_one;
    sai_set_pon_service_config_profile_threshold_data_one_attribute_fn                                                                set_pon_service_config_profile_threshold_data_one_attribute;
    sai_get_pon_service_config_profile_threshold_data_one_attribute_fn                                                                get_pon_service_config_profile_threshold_data_one_attribute;
    sai_create_pon_service_config_profile_threshold_data_two_fn                                                                       create_pon_service_config_profile_threshold_data_two;
    sai_remove_pon_service_config_profile_threshold_data_two_fn                                                                       remove_pon_service_config_profile_threshold_data_two;
    sai_set_pon_service_config_profile_threshold_data_two_attribute_fn                                                                set_pon_service_config_profile_threshold_data_two_attribute;
    sai_get_pon_service_config_profile_threshold_data_two_attribute_fn                                                                get_pon_service_config_profile_threshold_data_two_attribute;
    sai_create_pon_service_config_profile_traffic_descriptor_fn                                                                       create_pon_service_config_profile_traffic_descriptor;
    sai_remove_pon_service_config_profile_traffic_descriptor_fn                                                                       remove_pon_service_config_profile_traffic_descriptor;
    sai_set_pon_service_config_profile_traffic_descriptor_attribute_fn                                                                set_pon_service_config_profile_traffic_descriptor_attribute;
    sai_get_pon_service_config_profile_traffic_descriptor_attribute_fn                                                                get_pon_service_config_profile_traffic_descriptor_attribute;
    sai_create_pon_service_config_profile_traffic_scheduler_fn                                                                        create_pon_service_config_profile_traffic_scheduler;
    sai_remove_pon_service_config_profile_traffic_scheduler_fn                                                                        remove_pon_service_config_profile_traffic_scheduler;
    sai_set_pon_service_config_profile_traffic_scheduler_attribute_fn                                                                 set_pon_service_config_profile_traffic_scheduler_attribute;
    sai_get_pon_service_config_profile_traffic_scheduler_attribute_fn                                                                 get_pon_service_config_profile_traffic_scheduler_attribute;
    sai_create_pon_service_config_profile_virtual_ethernet_interface_pt_fn                                                            create_pon_service_config_profile_virtual_ethernet_interface_pt;
    sai_remove_pon_service_config_profile_virtual_ethernet_interface_pt_fn                                                            remove_pon_service_config_profile_virtual_ethernet_interface_pt;
    sai_set_pon_service_config_profile_virtual_ethernet_interface_pt_attribute_fn                                                     set_pon_service_config_profile_virtual_ethernet_interface_pt_attribute;
    sai_get_pon_service_config_profile_virtual_ethernet_interface_pt_attribute_fn                                                     get_pon_service_config_profile_virtual_ethernet_interface_pt_attribute;
    sai_create_pon_service_config_profile_vlan_tagging_filter_data_fn                                                                 create_pon_service_config_profile_vlan_tagging_filter_data;
    sai_remove_pon_service_config_profile_vlan_tagging_filter_data_fn                                                                 remove_pon_service_config_profile_vlan_tagging_filter_data;
    sai_set_pon_service_config_profile_vlan_tagging_filter_data_attribute_fn                                                          set_pon_service_config_profile_vlan_tagging_filter_data_attribute;
    sai_get_pon_service_config_profile_vlan_tagging_filter_data_attribute_fn                                                          get_pon_service_config_profile_vlan_tagging_filter_data_attribute;
    sai_create_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_fn                                                     create_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter;
    sai_remove_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_fn                                                     remove_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter;
    sai_set_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_attribute_fn                                              set_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_attribute;
    sai_get_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_attribute_fn                                              get_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_attribute;
    sai_create_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_fn                                                create_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_fn                                                remove_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data;
    sai_set_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_attribute_fn                                         set_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_attribute_fn                                         get_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_fn                                                             create_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_fn                                                             remove_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data;
    sai_set_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_attribute_fn                                                      set_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_attribute_fn                                                      get_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_fn                                                  create_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_fn                                                  remove_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data;
    sai_set_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_attribute_fn                                           set_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_attribute_fn                                           get_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_attribute;
    sai_create_pon_sla_profile_fn                                                                                                     create_pon_sla_profile;
    sai_remove_pon_sla_profile_fn                                                                                                     remove_pon_sla_profile;
    sai_set_pon_sla_profile_attribute_fn                                                                                              set_pon_sla_profile_attribute;
    sai_get_pon_sla_profile_attribute_fn                                                                                              get_pon_sla_profile_attribute;
    sai_create_pon_sla_profile_controller_fn                                                                                          create_pon_sla_profile_controller;
    sai_remove_pon_sla_profile_controller_fn                                                                                          remove_pon_sla_profile_controller;
    sai_set_pon_sla_profile_controller_attribute_fn                                                                                   set_pon_sla_profile_controller_attribute;
    sai_get_pon_sla_profile_controller_attribute_fn                                                                                   get_pon_sla_profile_controller_attribute;
} sai_pon_service_config_profile_api_t;

/**
 * @}
 */

#endif /* __SAIEXPERIMENTALPONSERVICECONFIGPROFILE_H_ */
