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
 * @file    saiexperimentalpononutemplate.h
 *
 * @brief   This module defines the experimental PON ONU template API.
 */

#if !defined (__SAIEXPERIMENTALPONONUTEMPLATE_H_)
#define __SAIEXPERIMENTALPONONUTEMPLATE_H_

#include <saitypes.h>

#include <saiexperimentalponcommon.h>

/**
 * @defgroup SAIEXPERIMENTALPONONUTEMPLATE SAI - Experimental: PON specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for PON ONU template ONU
 */
typedef enum _sai_pon_onu_template_onu_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_START,

    /**
     * @brief ONU Template Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_ONU_TEMPLATE_NAME = SAI_PON_ONU_TEMPLATE_ONU_ATTR_START,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_VLAN_ID,

    /**
     * @brief FW Bank Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_BANK_PTR,

    /**
     * @brief Service Config
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_SERVICE_CONFIG,

    /**
     * @brief Service Config OMCI Stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_SERVICE_CONFIG_OMCI_STATS,

    /**
     * @brief FW Upgrade Backoff Delay
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_BACKOFF_DELAY,

    /**
     * @brief FW Upgrade Backoff Divisor
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_BACKOFF_DIVISOR,

    /**
     * @brief FW Upgrade Download Format
     *
     * @type sai_fw_upgrade_download_format_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_DOWNLOAD_FORMAT,

    /**
     * @brief FW Upgrade End Download Timeout
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_END_DOWNLOAD_TIMEOUT,

    /**
     * @brief FW Upgrade Maximum Retries
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_MAXIMUM_RETRIES,

    /**
     * @brief FW Upgrade Maximum Window Size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_MAXIMUM_WINDOW_SIZE,

    /**
     * @brief FW Upgrade Response Timeout
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_RESPONSE_TIMEOUT,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_template_onu_attr_t;

/**
 * @brief Create PON ONU template ONU
 *
 * @param[out] pon_onu_template_onu_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_onu_template_onu_fn)(
        _Out_ sai_object_id_t *pon_onu_template_onu_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON ONU template ONU
 *
 * @param[in] pon_onu_template_onu_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_onu_template_onu_fn)(
        _In_ sai_object_id_t pon_onu_template_onu_id);

/**
 * @brief Set attribute for PON ONU template ONU
 *
 * @param[in] pon_onu_template_onu_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_onu_template_onu_attribute_fn)(
        _In_ sai_object_id_t pon_onu_template_onu_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON ONU template ONU
 *
 * @param[in] pon_onu_template_onu_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_template_onu_attribute_fn)(
        _In_ sai_object_id_t pon_onu_template_onu_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON ONU template ONU FW bank file
 */
typedef enum _sai_pon_onu_template_onu_fw_bank_file_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_START,

    /**
     * @brief ONU Template Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_ONU_TEMPLATE_NAME = SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_START,

    /**
     * @brief Bank ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_BANK_ID,

    /**
     * @brief File
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_FILE,

    /**
     * @brief Version
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_VERSION,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_template_onu_fw_bank_file_attr_t;

/**
 * @brief Create PON ONU template ONU FW bank file
 *
 * @param[out] pon_onu_template_onu_fw_bank_file_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_onu_template_onu_fw_bank_file_fn)(
        _Out_ sai_object_id_t *pon_onu_template_onu_fw_bank_file_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON ONU template ONU FW bank file
 *
 * @param[in] pon_onu_template_onu_fw_bank_file_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_onu_template_onu_fw_bank_file_fn)(
        _In_ sai_object_id_t pon_onu_template_onu_fw_bank_file_id);

/**
 * @brief Set attribute for PON ONU template ONU FW bank file
 *
 * @param[in] pon_onu_template_onu_fw_bank_file_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_onu_template_onu_fw_bank_file_attribute_fn)(
        _In_ sai_object_id_t pon_onu_template_onu_fw_bank_file_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON ONU template ONU FW bank file
 *
 * @param[in] pon_onu_template_onu_fw_bank_file_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_template_onu_fw_bank_file_attribute_fn)(
        _In_ sai_object_id_t pon_onu_template_onu_fw_bank_file_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON ONU template OLT service
 */
typedef enum _sai_pon_onu_template_olt_service_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_START,

    /**
     * @brief ONU Template Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_ONU_TEMPLATE_NAME = SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Downstream QOS Map
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_DOWNSTREAM_QOS_MAP,

    /**
     * @brief Enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_ENABLE,

    /**
     * @brief Learning Limit
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_LEARNING_LIMIT,

    /**
     * @brief SLA Profile
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_SLA_PROFILE,

    /**
     * @brief TCONT Service Ref
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_TCONT_SERVICE_REF,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_template_olt_service_attr_t;

/**
 * @brief Create PON ONU template OLT service
 *
 * @param[out] pon_onu_template_olt_service_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_onu_template_olt_service_fn)(
        _Out_ sai_object_id_t *pon_onu_template_olt_service_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON ONU template OLT service
 *
 * @param[in] pon_onu_template_olt_service_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_onu_template_olt_service_fn)(
        _In_ sai_object_id_t pon_onu_template_olt_service_id);

/**
 * @brief Set attribute for PON ONU template OLT service
 *
 * @param[in] pon_onu_template_olt_service_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_onu_template_olt_service_attribute_fn)(
        _In_ sai_object_id_t pon_onu_template_olt_service_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON ONU template OLT service
 *
 * @param[in] pon_onu_template_olt_service_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_template_olt_service_attribute_fn)(
        _In_ sai_object_id_t pon_onu_template_olt_service_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON ONU template OLT service network
 */
typedef enum _sai_pon_onu_template_olt_service_network_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_START,

    /**
     * @brief ONU Template Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_ONU_TEMPLATE_NAME = SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Network ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_NETWORK_ID,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_VLAN_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_template_olt_service_network_attr_t;

/**
 * @brief Create PON ONU template OLT service network
 *
 * @param[out] pon_onu_template_olt_service_network_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_onu_template_olt_service_network_fn)(
        _Out_ sai_object_id_t *pon_onu_template_olt_service_network_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON ONU template OLT service network
 *
 * @param[in] pon_onu_template_olt_service_network_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_onu_template_olt_service_network_fn)(
        _In_ sai_object_id_t pon_onu_template_olt_service_network_id);

/**
 * @brief Set attribute for PON ONU template OLT service network
 *
 * @param[in] pon_onu_template_olt_service_network_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_onu_template_olt_service_network_attribute_fn)(
        _In_ sai_object_id_t pon_onu_template_olt_service_network_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON ONU template OLT service network
 *
 * @param[in] pon_onu_template_olt_service_network_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_template_olt_service_network_attribute_fn)(
        _In_ sai_object_id_t pon_onu_template_olt_service_network_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON ONU template service config value
 */
typedef enum _sai_pon_onu_template_service_config_value_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_START,

    /**
     * @brief ONU Template Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_ONU_TEMPLATE_NAME = SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_START,

    /**
     * @brief Configuration Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_CFG_NAME,

    /**
     * @brief Value
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_VALUE,

    /**
     * @brief Value Type
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_VALUE_TYPE,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_template_service_config_value_attr_t;

/**
 * @brief Create PON ONU template service config value
 *
 * @param[out] pon_onu_template_service_config_value_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_onu_template_service_config_value_fn)(
        _Out_ sai_object_id_t *pon_onu_template_service_config_value_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON ONU template service config value
 *
 * @param[in] pon_onu_template_service_config_value_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_onu_template_service_config_value_fn)(
        _In_ sai_object_id_t pon_onu_template_service_config_value_id);

/**
 * @brief Set attribute for PON ONU template service config value
 *
 * @param[in] pon_onu_template_service_config_value_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_onu_template_service_config_value_attribute_fn)(
        _In_ sai_object_id_t pon_onu_template_service_config_value_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON ONU template service config value
 *
 * @param[in] pon_onu_template_service_config_value_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_template_service_config_value_attribute_fn)(
        _In_ sai_object_id_t pon_onu_template_service_config_value_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON ONU template uni
 */
typedef enum _sai_pon_onu_template_uni_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_START,

    /**
     * @brief ONU Template Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_ONU_TEMPLATE_NAME = SAI_PON_ONU_TEMPLATE_UNI_ATTR_START,

    /**
     * @brief Port ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_PORT_ID,

    /**
     * @brief Duplex
     *
     * @type sai_duplex_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_DUPLEX,

    /**
     * @brief Enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_ENABLE,

    /**
     * @brief Max Frame Size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_MAX_FRAME_SIZE,

    /**
     * @brief POE
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_POE,

    /**
     * @brief Speed
     *
     * @type sai_speed_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_SPEED,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_template_uni_attr_t;

/**
 * @brief Create PON ONU template uni
 *
 * @param[out] pon_onu_template_uni_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_onu_template_uni_fn)(
        _Out_ sai_object_id_t *pon_onu_template_uni_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON ONU template uni
 *
 * @param[in] pon_onu_template_uni_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_onu_template_uni_fn)(
        _In_ sai_object_id_t pon_onu_template_uni_id);

/**
 * @brief Set attribute for PON ONU template uni
 *
 * @param[in] pon_onu_template_uni_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_onu_template_uni_attribute_fn)(
        _In_ sai_object_id_t pon_onu_template_uni_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON ONU template uni
 *
 * @param[in] pon_onu_template_uni_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_template_uni_attribute_fn)(
        _In_ sai_object_id_t pon_onu_template_uni_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_pon_onu_template_api_t
{
    sai_create_pon_onu_template_onu_fn                         create_pon_onu_template_onu;
    sai_remove_pon_onu_template_onu_fn                         remove_pon_onu_template_onu;
    sai_set_pon_onu_template_onu_attribute_fn                  set_pon_onu_template_onu_attribute;
    sai_get_pon_onu_template_onu_attribute_fn                  get_pon_onu_template_onu_attribute;
    sai_create_pon_onu_template_onu_fw_bank_file_fn            create_pon_onu_template_onu_fw_bank_file;
    sai_remove_pon_onu_template_onu_fw_bank_file_fn            remove_pon_onu_template_onu_fw_bank_file;
    sai_set_pon_onu_template_onu_fw_bank_file_attribute_fn     set_pon_onu_template_onu_fw_bank_file_attribute;
    sai_get_pon_onu_template_onu_fw_bank_file_attribute_fn     get_pon_onu_template_onu_fw_bank_file_attribute;
    sai_create_pon_onu_template_olt_service_fn                 create_pon_onu_template_olt_service;
    sai_remove_pon_onu_template_olt_service_fn                 remove_pon_onu_template_olt_service;
    sai_set_pon_onu_template_olt_service_attribute_fn          set_pon_onu_template_olt_service_attribute;
    sai_get_pon_onu_template_olt_service_attribute_fn          get_pon_onu_template_olt_service_attribute;
    sai_create_pon_onu_template_olt_service_network_fn         create_pon_onu_template_olt_service_network;
    sai_remove_pon_onu_template_olt_service_network_fn         remove_pon_onu_template_olt_service_network;
    sai_set_pon_onu_template_olt_service_network_attribute_fn  set_pon_onu_template_olt_service_network_attribute;
    sai_get_pon_onu_template_olt_service_network_attribute_fn  get_pon_onu_template_olt_service_network_attribute;
    sai_create_pon_onu_template_service_config_value_fn        create_pon_onu_template_service_config_value;
    sai_remove_pon_onu_template_service_config_value_fn        remove_pon_onu_template_service_config_value;
    sai_set_pon_onu_template_service_config_value_attribute_fn set_pon_onu_template_service_config_value_attribute;
    sai_get_pon_onu_template_service_config_value_attribute_fn get_pon_onu_template_service_config_value_attribute;
    sai_create_pon_onu_template_uni_fn                         create_pon_onu_template_uni;
    sai_remove_pon_onu_template_uni_fn                         remove_pon_onu_template_uni;
    sai_set_pon_onu_template_uni_attribute_fn                  set_pon_onu_template_uni_attribute;
    sai_get_pon_onu_template_uni_attribute_fn                  get_pon_onu_template_uni_attribute;
} sai_pon_onu_template_api_t;

/**
 * @}
 */

#endif /* __SAIEXPERIMENTALPONONUTEMPLATE_H_ */
