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
 * @file    saiexperimentalpononu.h
 *
 * @brief   This module defines the experimental PON ONU API.
 */

#if !defined (__SAIEXPERIMENTALPONONU_H_)
#define __SAIEXPERIMENTALPONONU_H_

#include <saitypes.h>

#include <saiexperimentalponcommon.h>

/**
 * @defgroup SAIEXPERIMENTALPONONU SAI - Experimental: PON specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for PON ONU
 */
typedef enum _sai_pon_onu_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_ONU_NAME = SAI_PON_ONU_ATTR_START,

    /**
     * @brief Device ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_DEVICE_ID,

    /**
     * @brief Template Ref
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_TEMPLATE_REF,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     */
    SAI_PON_ONU_ATTR_VLAN_ID,

    /**
     * @brief FW Bank Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_ONU_ATTR_FW_BANK_PTR,

    /**
     * @brief Real-time Stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_REALTIME_STATS,

    /**
     * @brief Service Config
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_SERVICE_CONFIG,

    /**
     * @brief Service Config OMCI Stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_SERVICE_CONFIG_OMCI_STATS,

    /**
     * @brief FW Upgrade Backoff Delay
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_BACKOFF_DELAY,

    /**
     * @brief FW Upgrade Backoff Divisor
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_BACKOFF_DIVISOR,

    /**
     * @brief FW Upgrade Download Format
     *
     * @type sai_fw_upgrade_download_format_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_DOWNLOAD_FORMAT,

    /**
     * @brief FW Upgrade End Download Timeout
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_END_DOWNLOAD_TIMEOUT,

    /**
     * @brief FW Upgrade Maximum Retries
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_MAXIMUM_RETRIES,

    /**
     * @brief FW Upgrade Maximum Window Size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_MAXIMUM_WINDOW_SIZE,

    /**
     * @brief FW Upgrade Response Timeout
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_RESPONSE_TIMEOUT,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_attr_t;

/**
 * @brief Create PON ONU
 *
 * @param[out] pon_onu_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_onu_fn)(
        _Out_ sai_object_id_t *pon_onu_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON ONU
 *
 * @param[in] pon_onu_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_onu_fn)(
        _In_ sai_object_id_t pon_onu_id);

/**
 * @brief Set attribute for PON ONU
 *
 * @param[in] pon_onu_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_onu_attribute_fn)(
        _In_ sai_object_id_t pon_onu_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON ONU
 *
 * @param[in] pon_onu_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_attribute_fn)(
        _In_ sai_object_id_t pon_onu_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON ONU OLT service
 */
typedef enum _sai_pon_onu_olt_service_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_ONU_NAME = SAI_PON_ONU_OLT_SERVICE_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Downstream QOS Map
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_DOWNSTREAM_QOS_MAP,

    /**
     * @brief Enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_ENABLE,

    /**
     * @brief Learning Limit
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_LEARNING_LIMIT,

    /**
     * @brief SLA Profile
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_SLA_PROFILE,

    /**
     * @brief TCONT Service Ref
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_TCONT_SERVICE_REF,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_OLT_SERVICE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_OLT_SERVICE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_olt_service_attr_t;

/**
 * @brief Create PON ONU OLT service
 *
 * @param[out] pon_onu_olt_service_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_onu_olt_service_fn)(
        _Out_ sai_object_id_t *pon_onu_olt_service_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON ONU OLT service
 *
 * @param[in] pon_onu_olt_service_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_onu_olt_service_fn)(
        _In_ sai_object_id_t pon_onu_olt_service_id);

/**
 * @brief Set attribute for PON ONU OLT service
 *
 * @param[in] pon_onu_olt_service_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_onu_olt_service_attribute_fn)(
        _In_ sai_object_id_t pon_onu_olt_service_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON ONU OLT service
 *
 * @param[in] pon_onu_olt_service_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_olt_service_attribute_fn)(
        _In_ sai_object_id_t pon_onu_olt_service_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON ONU OLT service network
 */
typedef enum _sai_pon_onu_olt_service_network_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_ONU_NAME = SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Network ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_NETWORK_ID,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_VLAN_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_olt_service_network_attr_t;

/**
 * @brief Create PON ONU OLT service network
 *
 * @param[out] pon_onu_olt_service_network_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_onu_olt_service_network_fn)(
        _Out_ sai_object_id_t *pon_onu_olt_service_network_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON ONU OLT service network
 *
 * @param[in] pon_onu_olt_service_network_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_onu_olt_service_network_fn)(
        _In_ sai_object_id_t pon_onu_olt_service_network_id);

/**
 * @brief Set attribute for PON ONU OLT service network
 *
 * @param[in] pon_onu_olt_service_network_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_onu_olt_service_network_attribute_fn)(
        _In_ sai_object_id_t pon_onu_olt_service_network_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON ONU OLT service network
 *
 * @param[in] pon_onu_olt_service_network_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_olt_service_network_attribute_fn)(
        _In_ sai_object_id_t pon_onu_olt_service_network_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON ONU service config value
 */
typedef enum _sai_pon_onu_service_config_value_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_ONU_NAME = SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_START,

    /**
     * @brief Configuration Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_CFG_NAME,

    /**
     * @brief Value
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_VALUE,

    /**
     * @brief Value Type
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_VALUE_TYPE,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_service_config_value_attr_t;

/**
 * @brief Create PON ONU service config value
 *
 * @param[out] pon_onu_service_config_value_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_onu_service_config_value_fn)(
        _Out_ sai_object_id_t *pon_onu_service_config_value_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON ONU service config value
 *
 * @param[in] pon_onu_service_config_value_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_onu_service_config_value_fn)(
        _In_ sai_object_id_t pon_onu_service_config_value_id);

/**
 * @brief Set attribute for PON ONU service config value
 *
 * @param[in] pon_onu_service_config_value_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_onu_service_config_value_attribute_fn)(
        _In_ sai_object_id_t pon_onu_service_config_value_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON ONU service config value
 *
 * @param[in] pon_onu_service_config_value_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_service_config_value_attribute_fn)(
        _In_ sai_object_id_t pon_onu_service_config_value_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON ONU uni
 */
typedef enum _sai_pon_onu_uni_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_UNI_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_UNI_ATTR_ONU_NAME = SAI_PON_ONU_UNI_ATTR_START,

    /**
     * @brief Port ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_UNI_ATTR_PORT_ID,

    /**
     * @brief Duplex
     *
     * @type sai_duplex_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_UNI_ATTR_DUPLEX,

    /**
     * @brief Enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_UNI_ATTR_ENABLE,

    /**
     * @brief Max Frame Size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_UNI_ATTR_MAX_FRAME_SIZE,

    /**
     * @brief POE
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_UNI_ATTR_POE,

    /**
     * @brief Speed
     *
     * @type sai_speed_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_UNI_ATTR_SPEED,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_UNI_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_UNI_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_UNI_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_uni_attr_t;

/**
 * @brief Create PON ONU uni
 *
 * @param[out] pon_onu_uni_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_onu_uni_fn)(
        _Out_ sai_object_id_t *pon_onu_uni_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON ONU uni
 *
 * @param[in] pon_onu_uni_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_onu_uni_fn)(
        _In_ sai_object_id_t pon_onu_uni_id);

/**
 * @brief Set attribute for PON ONU uni
 *
 * @param[in] pon_onu_uni_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_onu_uni_attribute_fn)(
        _In_ sai_object_id_t pon_onu_uni_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON ONU uni
 *
 * @param[in] pon_onu_uni_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_uni_attribute_fn)(
        _In_ sai_object_id_t pon_onu_uni_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON ONU FW bank file
 */
typedef enum _sai_pon_onu_fw_bank_file_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_FW_BANK_FILE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_FW_BANK_FILE_ATTR_ONU_NAME = SAI_PON_ONU_FW_BANK_FILE_ATTR_START,

    /**
     * @brief Bank ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_FW_BANK_FILE_ATTR_BANK_ID,

    /**
     * @brief File
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_FW_BANK_FILE_ATTR_FILE,

    /**
     * @brief Version
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ONU_FW_BANK_FILE_ATTR_VERSION,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_FW_BANK_FILE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_FW_BANK_FILE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_FW_BANK_FILE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_fw_bank_file_attr_t;

/**
 * @brief Create PON ONU FW bank file
 *
 * @param[out] pon_onu_fw_bank_file_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_onu_fw_bank_file_fn)(
        _Out_ sai_object_id_t *pon_onu_fw_bank_file_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON ONU FW bank file
 *
 * @param[in] pon_onu_fw_bank_file_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_onu_fw_bank_file_fn)(
        _In_ sai_object_id_t pon_onu_fw_bank_file_id);

/**
 * @brief Set attribute for PON ONU FW bank file
 *
 * @param[in] pon_onu_fw_bank_file_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_onu_fw_bank_file_attribute_fn)(
        _In_ sai_object_id_t pon_onu_fw_bank_file_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON ONU FW bank file
 *
 * @param[in] pon_onu_fw_bank_file_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_fw_bank_file_attribute_fn)(
        _In_ sai_object_id_t pon_onu_fw_bank_file_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON ONU OLT service downstream QOS map state (STATE_DB)
 */
typedef enum _sai_pon_onu_olt_service_downstream_qos_map_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE_ATTR_ONU_NAME = SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Type
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE_ATTR_TYPE,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_olt_service_downstream_qos_map_state_attr_t;

/**
 * @brief Get attribute for PON ONU OLT service downstream QOS map state (STATE_DB, read-only)
 *
 * @param[in] pon_onu_olt_service_downstream_qos_map_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_olt_service_downstream_qos_map_state_attribute_fn)(
        _In_ sai_object_id_t pon_onu_olt_service_downstream_qos_map_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU OLT service downstream QOS map state
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_olt_service_downstream_qos_map_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU OLT service downstream QOS map map state (STATE_DB)
 */
typedef enum _sai_pon_onu_olt_service_downstream_qos_map_map_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE_ATTR_ONU_NAME = SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Priority
     *
     * @type sai_uint8_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE_ATTR_PRIORITY,

    /**
     * @brief OLT Service Offset
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE_ATTR_OLT_SERVICE_OFFSET,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_olt_service_downstream_qos_map_map_state_attr_t;

/**
 * @brief Get attribute for PON ONU OLT service downstream QOS map map state (STATE_DB, read-only)
 *
 * @param[in] pon_onu_olt_service_downstream_qos_map_map_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_olt_service_downstream_qos_map_map_state_attribute_fn)(
        _In_ sai_object_id_t pon_onu_olt_service_downstream_qos_map_map_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU OLT service downstream QOS map map state
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_olt_service_downstream_qos_map_map_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU uni learned addresses state (STATE_DB)
 */
typedef enum _sai_pon_onu_uni_learned_addresses_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_UNI_LEARNED_ADDRESSES_STATE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_UNI_LEARNED_ADDRESSES_STATE_ATTR_ONU_NAME = SAI_PON_ONU_UNI_LEARNED_ADDRESSES_STATE_ATTR_START,

    /**
     * @brief Port ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_UNI_LEARNED_ADDRESSES_STATE_ATTR_PORT_ID,

    /**
     * @brief Learned Address
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_UNI_LEARNED_ADDRESSES_STATE_ATTR_LEARNED_ADDRESS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_UNI_LEARNED_ADDRESSES_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_UNI_LEARNED_ADDRESSES_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_UNI_LEARNED_ADDRESSES_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_uni_learned_addresses_state_attr_t;

/**
 * @brief Get attribute for PON ONU uni learned addresses state (STATE_DB, read-only)
 *
 * @param[in] pon_onu_uni_learned_addresses_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_uni_learned_addresses_state_attribute_fn)(
        _In_ sai_object_id_t pon_onu_uni_learned_addresses_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU uni learned addresses state
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_uni_learned_addresses_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU state (STATE_DB)
 */
typedef enum _sai_pon_onu_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_ONU_NAME = SAI_PON_ONU_STATE_ATTR_START,

    /**
     * @brief Timestamp
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_TIMESTAMP,

    /**
     * @brief ONU ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATE_ATTR_ONU_ID,

    /**
     * @brief CVID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan true
     */
    SAI_PON_ONU_STATE_ATTR_CVID,

    /**
     * @brief Equipment ID
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_EQUIPMENT_ID,

    /**
     * @brief FW Bank Pointer
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATE_ATTR_FW_BANK_PTR,

    /**
     * @brief FW Version
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_FW_VERSION,

    /**
     * @brief Hardware Version
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_HARDWARE_VERSION,

    /**
     * @brief Last Provisioning Time
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_LAST_PROVISIONING_TIME,

    /**
     * @brief MAC Address
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_MAC_ADDRESS,

    /**
     * @brief Manufacturer
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_MANUFACTURER,

    /**
     * @brief Model
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_MODEL,

    /**
     * @brief Online Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_ONLINE_TIME,

    /**
     * @brief Real-time Stats
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_REALTIME_STATS,

    /**
     * @brief Registration ID
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_REGISTRATION_ID,

    /**
     * @brief Allocation ID Omcc
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATE_ATTR_ALLOC_ID_OMCC,

    /**
     * @brief Host MAC Address
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_HOST_MAC_ADDRESS,

    /**
     * @brief Laser Bias Current
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_LASER_BIAS_CURRENT,

    /**
     * @brief Logical ID
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_LOGICAL_ID,

    /**
     * @brief Logical Password
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_LOGICAL_PASSWORD,

    /**
     * @brief OMCI Txn Correlation ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATE_ATTR_OMCI_TXN_CORRELATION_ID,

    /**
     * @brief Omcc Version
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_OMCC_VERSION,

    /**
     * @brief Temperature
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_TEMPERATURE,

    /**
     * @brief Uptime
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_UPTIME,

    /**
     * @brief Voltage
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_VOLTAGE,

    /**
     * @brief Registration ID Hex
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_REGISTRATION_ID_HEX,

    /**
     * @brief Serial Number
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_SERIAL_NUMBER,

    /**
     * @brief Service Config
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_SERVICE_CONFIG,

    /**
     * @brief Vendor
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_VENDOR,

    /**
     * @brief FW Upgrade Backoff Delay
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_FW_UPGRADE_BACKOFF_DELAY,

    /**
     * @brief FW Upgrade Backoff Divisor
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_FW_UPGRADE_BACKOFF_DIVISOR,

    /**
     * @brief FW Upgrade Download Format
     *
     * @type sai_fw_upgrade_download_format_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_FW_UPGRADE_DOWNLOAD_FORMAT,

    /**
     * @brief FW Upgrade End Download Timeout
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_FW_UPGRADE_END_DOWNLOAD_TIMEOUT,

    /**
     * @brief FW Upgrade Maximum Retries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_FW_UPGRADE_MAXIMUM_RETRIES,

    /**
     * @brief FW Upgrade Maximum Window Size
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_FW_UPGRADE_MAXIMUM_WINDOW_SIZE,

    /**
     * @brief FW Upgrade Response Timeout
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_FW_UPGRADE_RESPONSE_TIMEOUT,

    /**
     * @brief Server State
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_SERVER_STATE,

    /**
     * @brief Registration Disallowed
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_REGISTRATION_DISALLOWED,

    /**
     * @brief Bank
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_BANK,

    /**
     * @brief Current Window
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_CURRENT_WINDOW,

    /**
     * @brief Fx Code
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_FX_CODE,

    /**
     * @brief File
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_FILE,

    /**
     * @brief Negotiated Window
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_NEGOTIATED_WINDOW,

    /**
     * @brief Progress
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_PROGRESS,

    /**
     * @brief Retries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_RETRIES,

    /**
     * @brief Sent Blocks
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_SENT_BLOCKS,

    /**
     * @brief Status
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_STATUS,

    /**
     * @brief Total Blocks
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_TOTAL_BLOCKS,

    /**
     * @brief Upgrade Duration
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_UPGRADE_DURATION,

    /**
     * @brief Upgrade Time
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_UPGRADE_TIME,

    /**
     * @brief Aborted
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_ABORTED,

    /**
     * @brief Failures
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_FAILURES,

    /**
     * @brief Version Mismatches
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATE_ATTR_VERSION_MISMATCHES,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_state_attr_t;

/**
 * @brief Get attribute for PON ONU state (STATE_DB, read-only)
 *
 * @param[in] pon_onu_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_state_attribute_fn)(
        _In_ sai_object_id_t pon_onu_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU state
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU FW bank version state (STATE_DB)
 */
typedef enum _sai_pon_onu_fw_bank_version_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_FW_BANK_VERSION_STATE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_FW_BANK_VERSION_STATE_ATTR_ONU_NAME = SAI_PON_ONU_FW_BANK_VERSION_STATE_ATTR_START,

    /**
     * @brief Bank ID
     *
     * @type sai_uint8_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_FW_BANK_VERSION_STATE_ATTR_BANK_ID,

    /**
     * @brief Version
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_FW_BANK_VERSION_STATE_ATTR_VERSION,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_FW_BANK_VERSION_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_FW_BANK_VERSION_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_FW_BANK_VERSION_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_fw_bank_version_state_attr_t;

/**
 * @brief Get attribute for PON ONU FW bank version state (STATE_DB, read-only)
 *
 * @param[in] pon_onu_fw_bank_version_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_fw_bank_version_state_attribute_fn)(
        _In_ sai_object_id_t pon_onu_fw_bank_version_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU FW bank version state
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_fw_bank_version_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU OLT service state (STATE_DB)
 */
typedef enum _sai_pon_onu_olt_service_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_ONU_NAME = SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Enable
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_ENABLE,

    /**
     * @brief Learning Limit
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_LEARNING_LIMIT,

    /**
     * @brief TCONT Service Ref
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_TCONT_SERVICE_REF,

    /**
     * @brief Unicast ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_UNICAST_ID,

    /**
     * @brief Upstream Priority Treatment
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_UPSTREAM_PRIORITY_TREATMENT,

    /**
     * @brief Upstream Priority Value
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_UPSTREAM_PRIORITY_VALUE,

    /**
     * @brief SLA Downstream Guaranteed Rate
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_SLA_DOWNSTREAM_GUARANTEED_RATE,

    /**
     * @brief SLA Downstream Guaranteed Maximum Burst
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_SLA_DOWNSTREAM_GUARANTEED_MAXIMUM_BURST,

    /**
     * @brief SLA Downstream Best Effort Rate
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_SLA_DOWNSTREAM_BEST_EFFORT_RATE,

    /**
     * @brief SLA Downstream Best Effort Maximum Burst
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_SLA_DOWNSTREAM_BEST_EFFORT_MAXIMUM_BURST,

    /**
     * @brief SLA Upstream Fixed Rate
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_SLA_UPSTREAM_FIXED_RATE,

    /**
     * @brief SLA Upstream Guaranteed Rate
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_SLA_UPSTREAM_GUARANTEED_RATE,

    /**
     * @brief SLA Upstream Guaranteed Maximum Burst
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_SLA_UPSTREAM_GUARANTEED_MAXIMUM_BURST,

    /**
     * @brief SLA Upstream Priority
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_SLA_UPSTREAM_PRIORITY,

    /**
     * @brief SLA Upstream Best Effort Rate
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_SLA_UPSTREAM_BEST_EFFORT_RATE,

    /**
     * @brief SLA Upstream Best Effort Maximum Burst
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_SLA_UPSTREAM_BEST_EFFORT_MAXIMUM_BURST,

    /**
     * @brief SLA Upstream Best Effort Priority
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_SLA_UPSTREAM_BEST_EFFORT_PRIORITY,

    /**
     * @brief Downstream QOS Map
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_DOWNSTREAM_QOS_MAP,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_olt_service_state_attr_t;

/**
 * @brief Get attribute for PON ONU OLT service state (STATE_DB, read-only)
 *
 * @param[in] pon_onu_olt_service_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_olt_service_state_attribute_fn)(
        _In_ sai_object_id_t pon_onu_olt_service_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU OLT service state
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_olt_service_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU OLT service network state (STATE_DB)
 */
typedef enum _sai_pon_onu_olt_service_network_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_STATE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_STATE_ATTR_ONU_NAME = SAI_PON_ONU_OLT_SERVICE_NETWORK_STATE_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_STATE_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Network ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_STATE_ATTR_NETWORK_ID,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan true
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_STATE_ATTR_VLAN_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_olt_service_network_state_attr_t;

/**
 * @brief Get attribute for PON ONU OLT service network state (STATE_DB, read-only)
 *
 * @param[in] pon_onu_olt_service_network_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_olt_service_network_state_attribute_fn)(
        _In_ sai_object_id_t pon_onu_olt_service_network_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU OLT service network state
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_olt_service_network_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU uni state (STATE_DB)
 */
typedef enum _sai_pon_onu_uni_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_UNI_STATE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_UNI_STATE_ATTR_ONU_NAME = SAI_PON_ONU_UNI_STATE_ATTR_START,

    /**
     * @brief Port ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_UNI_STATE_ATTR_PORT_ID,

    /**
     * @brief Duplex
     *
     * @type sai_duplex_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_UNI_STATE_ATTR_DUPLEX,

    /**
     * @brief Enable
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_ONU_UNI_STATE_ATTR_ENABLE,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_UNI_STATE_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Max Frame Size
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_UNI_STATE_ATTR_MAX_FRAME_SIZE,

    /**
     * @brief POE
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_ONU_UNI_STATE_ATTR_POE,

    /**
     * @brief Speed
     *
     * @type sai_speed_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_UNI_STATE_ATTR_SPEED,

    /**
     * @brief State
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_UNI_STATE_ATTR_STATE,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_UNI_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_UNI_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_UNI_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_uni_state_attr_t;

/**
 * @brief Get attribute for PON ONU uni state (STATE_DB, read-only)
 *
 * @param[in] pon_onu_uni_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_uni_state_attribute_fn)(
        _In_ sai_object_id_t pon_onu_uni_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU uni state
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_uni_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_ONU_STATS_ID,

    /**
     * @brief OLT PON Rx Optical Level
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_RX_OPTICAL_LEVEL,

    /**
     * @brief OLT PON Tx Optical Level
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_TX_OPTICAL_LEVEL,

    /**
     * @brief OLT PON Rx Deregistrations
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_RX_DEREGISTRATIONS,

    /**
     * @brief OLT PON Rx Registrations
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_RX_REGISTRATIONS,

    /**
     * @brief OLT PON Fiber Distance
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_FIBER_DISTANCE,

    /**
     * @brief OLT PON Equalization Delay
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_EQUALIZATION_DELAY,

    /**
     * @brief OLT PON Round Trip Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_ROUND_TRIP_TIME,

    /**
     * @brief OLT PON One Way Delay
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_ONE_WAY_DELAY,

    /**
     * @brief OLT PON Rx Acc FEC Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_RX_ACC_FEC_BYTES,

    /**
     * @brief OLT PON Rx Acc FEC Correct Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_RX_ACC_FEC_CORRECT_BYTES,

    /**
     * @brief OLT PON Rx Acc FEC Error Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_RX_ACC_FEC_ERROR_BYTES,

    /**
     * @brief OLT PON Rx Pre FEC BER
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_RX_PRE_FEC_BER,

    /**
     * @brief OLT PON Rx Post FEC BER
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_OLT_PON_RX_POST_FEC_BER,

    /**
     * @brief Timestamp
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_TIMESTAMP,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned OLT PON (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_olt_pon_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_ONU_STATS_ID,

    /**
     * @brief Rx Optical Level
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_RX_OPTICAL_LEVEL,

    /**
     * @brief Tx Optical Level
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_TX_OPTICAL_LEVEL,

    /**
     * @brief Rx Deregistrations
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_RX_DEREGISTRATIONS,

    /**
     * @brief Rx Registrations
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_RX_REGISTRATIONS,

    /**
     * @brief Fiber Distance
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_FIBER_DISTANCE,

    /**
     * @brief Equalization Delay
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_EQUALIZATION_DELAY,

    /**
     * @brief Round Trip Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_ROUND_TRIP_TIME,

    /**
     * @brief One Way Delay
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_ONE_WAY_DELAY,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_olt_pon_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned OLT PON (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_olt_pon_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_olt_pon_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_olt_pon_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned OLT PON
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_olt_pon_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned OLT PON omcc (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_olt_pon_omcc_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_ONU_STATS_ID,

    /**
     * @brief Bad Key Exchanges
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_BAD_KEY_EXCHANGES,

    /**
     * @brief Good Key Exchanges
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_GOOD_KEY_EXCHANGES,

    /**
     * @brief OMCI OAM Requests
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_OMCI_OAM_REQUESTS,

    /**
     * @brief OMCI OAM Resp Time Avg
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_OMCI_OAM_RESP_TIME_AVG,

    /**
     * @brief OMCI OAM Resp Time Max
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_OMCI_OAM_RESP_TIME_MAX,

    /**
     * @brief OMCI OAM Resp Time Min
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_OMCI_OAM_RESP_TIME_MIN,

    /**
     * @brief OMCI OAM Responses
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_OMCI_OAM_RESPONSES,

    /**
     * @brief OMCI OAM Time To Send Avg
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_OMCI_OAM_TIME_TO_SEND_AVG,

    /**
     * @brief OMCI OAM Time To Send Max
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_OMCI_OAM_TIME_TO_SEND_MAX,

    /**
     * @brief OMCI OAM Time To Send Min
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_OMCI_OAM_TIME_TO_SEND_MIN,

    /**
     * @brief OMCI OAM Timeouts
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_OMCI_OAM_TIMEOUTS,

    /**
     * @brief Ploam Timeouts
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_PLOAM_TIMEOUTS,

    /**
     * @brief Rx All Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_ALL_BANDWIDTH_REQS,

    /**
     * @brief Rx Bad Icv Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_BAD_ICV_DROPS,

    /**
     * @brief Rx Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_BANDWIDTH_REQS,

    /**
     * @brief Rx Crc32 Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_CRC32_DROPS,

    /**
     * @brief Rx Crc8 Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_CRC8_ERRORS,

    /**
     * @brief Rx Control Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_CONTROL_OCTETS,

    /**
     * @brief Rx Empty Slots
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_EMPTY_SLOTS,

    /**
     * @brief Rx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_ENCRYPTED_OCTETS,

    /**
     * @brief Rx Encrypted Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_ENCRYPTED_SEGMENTS,

    /**
     * @brief Rx Errored Bip Bits
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_ERRORED_BIP_BITS,

    /**
     * @brief Rx Errored Bip Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_ERRORED_BIP_BLOCKS,

    /**
     * @brief Rx FEC Corrected Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FEC_CORRECTED_BLOCKS,

    /**
     * @brief Rx FEC Corrections
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FEC_CORRECTIONS,

    /**
     * @brief Rx FEC Good Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FEC_GOOD_BLOCKS,

    /**
     * @brief Rx FEC Uncorrectable Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FEC_UNCORRECTABLE_BLOCKS,

    /**
     * @brief Rx Filtered Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FILTERED_FRAMES,

    /**
     * @brief Rx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FRAMES_1024_1518,

    /**
     * @brief Rx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FRAMES_128_255,

    /**
     * @brief Rx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FRAMES_1519_PLUS,

    /**
     * @brief Rx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FRAMES_256_511,

    /**
     * @brief Rx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FRAMES_512_1023,

    /**
     * @brief Rx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FRAMES_64,

    /**
     * @brief Rx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FRAMES_65_127,

    /**
     * @brief Rx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_FRAMES_GREEN,

    /**
     * @brief Rx Good Bip Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_GOOD_BIP_BLOCKS,

    /**
     * @brief Rx HEC Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_HEC_ERRORS,

    /**
     * @brief Rx Idle Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_IDLE_OCTETS,

    /**
     * @brief Rx Key Mismatch Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_KEY_MISMATCH_OCTETS,

    /**
     * @brief Rx Mpcp Ploam
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_MPCP_PLOAM,

    /**
     * @brief Rx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Rx OAM
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_OAM,

    /**
     * @brief Rx Overflow Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_OVERFLOW_DROPS,

    /**
     * @brief Rx Overflow Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_OVERFLOW_OCTETS,

    /**
     * @brief Rx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_PLAIN_OCTETS,

    /**
     * @brief Rx Plain Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_PLAIN_SEGMENTS,

    /**
     * @brief Rx Ploam Mic Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_PLOAM_MIC_ERRORS,

    /**
     * @brief Rx Rate Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_RATE_BPS,

    /**
     * @brief Rx Security Drop Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_SECURITY_DROP_OCTETS,

    /**
     * @brief Rx Too Long Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_TOO_LONG_DROPS,

    /**
     * @brief Rx Too Short Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_TOO_SHORT_DROPS,

    /**
     * @brief Rx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_TOTAL_OCTETS,

    /**
     * @brief Rx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_RX_UNICAST_OCTETS,

    /**
     * @brief Tx Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_BANDWIDTH_REQS,

    /**
     * @brief Tx Control Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_CONTROL_OCTETS,

    /**
     * @brief Tx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_ENCRYPTED_OCTETS,

    /**
     * @brief Tx Encrypted Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_ENCRYPTED_SEGMENTS,

    /**
     * @brief Tx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_FRAMES_1024_1518,

    /**
     * @brief Tx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_FRAMES_128_255,

    /**
     * @brief Tx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_FRAMES_1519_PLUS,

    /**
     * @brief Tx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_FRAMES_256_511,

    /**
     * @brief Tx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_FRAMES_512_1023,

    /**
     * @brief Tx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_FRAMES_64,

    /**
     * @brief Tx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_FRAMES_65_127,

    /**
     * @brief Tx Frames Broadcast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_FRAMES_BROADCAST,

    /**
     * @brief Tx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_FRAMES_GREEN,

    /**
     * @brief Tx Frames Multicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_FRAMES_MULTICAST,

    /**
     * @brief Tx Frames Unicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_FRAMES_UNICAST,

    /**
     * @brief Tx Grant Ups Tq
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_GRANT_UPS_TQ,

    /**
     * @brief Tx Mpcp Ploam
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_MPCP_PLOAM,

    /**
     * @brief Tx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Tx OAM
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_OAM,

    /**
     * @brief Tx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_PLAIN_OCTETS,

    /**
     * @brief Tx Plain Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_PLAIN_SEGMENTS,

    /**
     * @brief Tx Ploam Ds Ranging Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_PLOAM_DS_RANGING_TIME,

    /**
     * @brief Tx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_TOTAL_OCTETS,

    /**
     * @brief Tx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_UNICAST_OCTETS,

    /**
     * @brief Tx Upstream Slots
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_TX_UPSTREAM_SLOTS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_olt_pon_omcc_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned OLT PON omcc (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_olt_pon_omcc_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_olt_pon_omcc_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_olt_pon_omcc_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned OLT PON omcc
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_olt_pon_omcc_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned OLT PON service (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_olt_pon_service_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_ONU_STATS_ID,

    /**
     * @brief Service Port ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_SERVICE_PORT_ID,

    /**
     * @brief Bad Key Exchanges
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_BAD_KEY_EXCHANGES,

    /**
     * @brief Enable Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_ENABLE_COUNT,

    /**
     * @brief Good Key Exchanges
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_GOOD_KEY_EXCHANGES,

    /**
     * @brief OMCI OAM Requests
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_OMCI_OAM_REQUESTS,

    /**
     * @brief OMCI OAM Resp Time Avg
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_OMCI_OAM_RESP_TIME_AVG,

    /**
     * @brief OMCI OAM Resp Time Max
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_OMCI_OAM_RESP_TIME_MAX,

    /**
     * @brief OMCI OAM Resp Time Min
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_OMCI_OAM_RESP_TIME_MIN,

    /**
     * @brief OMCI OAM Responses
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_OMCI_OAM_RESPONSES,

    /**
     * @brief OMCI OAM Time To Send Avg
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_OMCI_OAM_TIME_TO_SEND_AVG,

    /**
     * @brief OMCI OAM Time To Send Max
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_OMCI_OAM_TIME_TO_SEND_MAX,

    /**
     * @brief OMCI OAM Time To Send Min
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_OMCI_OAM_TIME_TO_SEND_MIN,

    /**
     * @brief OMCI OAM Timeouts
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_OMCI_OAM_TIMEOUTS,

    /**
     * @brief Ploam Timeouts
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_PLOAM_TIMEOUTS,

    /**
     * @brief Rx All Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_ALL_BANDWIDTH_REQS,

    /**
     * @brief Rx BW Best Effort SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_BW_BEST_EFFORT_SLA_UTIL,

    /**
     * @brief Rx BW Best Effort SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_BW_BEST_EFFORT_SLA_BPS,

    /**
     * @brief Rx BW Fixed SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_BW_FIXED_SLA_UTIL,

    /**
     * @brief Rx BW Fixed SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_BW_FIXED_SLA_BPS,

    /**
     * @brief Rx BW Guaranteed SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_BW_GUARANTEED_SLA_UTIL,

    /**
     * @brief Rx BW Guaranteed SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_BW_GUARANTEED_SLA_BPS,

    /**
     * @brief Rx BW Total SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_BW_TOTAL_SLA_UTIL,

    /**
     * @brief Rx BW Total SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_BW_TOTAL_SLA_BPS,

    /**
     * @brief Rx Bad Icv Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_BAD_ICV_DROPS,

    /**
     * @brief Rx Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_BANDWIDTH_REQS,

    /**
     * @brief Rx Crc32 Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_CRC32_DROPS,

    /**
     * @brief Rx Crc8 Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_CRC8_ERRORS,

    /**
     * @brief Rx Control Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_CONTROL_OCTETS,

    /**
     * @brief Rx Empty Slots
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_EMPTY_SLOTS,

    /**
     * @brief Rx Encrypted Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_ENCRYPTED_FRAMES,

    /**
     * @brief Rx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_ENCRYPTED_OCTETS,

    /**
     * @brief Rx Encrypted Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_ENCRYPTED_SEGMENTS,

    /**
     * @brief Rx Errored Bip Bits
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_ERRORED_BIP_BITS,

    /**
     * @brief Rx Errored Bip Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_ERRORED_BIP_BLOCKS,

    /**
     * @brief Rx FEC Corrected Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FEC_CORRECTED_BLOCKS,

    /**
     * @brief Rx FEC Corrections
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FEC_CORRECTIONS,

    /**
     * @brief Rx FEC Good Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FEC_GOOD_BLOCKS,

    /**
     * @brief Rx FEC Uncorrectable Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FEC_UNCORRECTABLE_BLOCKS,

    /**
     * @brief Rx Filtered Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FILTERED_FRAMES,

    /**
     * @brief Rx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FRAMES_1024_1518,

    /**
     * @brief Rx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FRAMES_128_255,

    /**
     * @brief Rx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FRAMES_1519_PLUS,

    /**
     * @brief Rx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FRAMES_256_511,

    /**
     * @brief Rx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FRAMES_512_1023,

    /**
     * @brief Rx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FRAMES_64,

    /**
     * @brief Rx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FRAMES_65_127,

    /**
     * @brief Rx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_FRAMES_GREEN,

    /**
     * @brief Rx Good Bip Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_GOOD_BIP_BLOCKS,

    /**
     * @brief Rx HEC Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_HEC_ERRORS,

    /**
     * @brief Rx Idle Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_IDLE_OCTETS,

    /**
     * @brief Rx Key Mismatch Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_KEY_MISMATCH_OCTETS,

    /**
     * @brief Rx Mpcp Ploam
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_MPCP_PLOAM,

    /**
     * @brief Rx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Rx Overflow Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_OVERFLOW_DROPS,

    /**
     * @brief Rx Overflow Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_OVERFLOW_OCTETS,

    /**
     * @brief Rx Plain Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_PLAIN_FRAMES,

    /**
     * @brief Rx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_PLAIN_OCTETS,

    /**
     * @brief Rx Plain Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_PLAIN_SEGMENTS,

    /**
     * @brief Rx Ploam Mic Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_PLOAM_MIC_ERRORS,

    /**
     * @brief Rx Rate Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_RATE_BPS,

    /**
     * @brief Rx Security Drop Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_SECURITY_DROP_OCTETS,

    /**
     * @brief Rx Too Long Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_TOO_LONG_DROPS,

    /**
     * @brief Rx Too Short Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_TOO_SHORT_DROPS,

    /**
     * @brief Rx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_TOTAL_OCTETS,

    /**
     * @brief Rx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_RX_UNICAST_OCTETS,

    /**
     * @brief Tx BW Best Effort SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_BW_BEST_EFFORT_SLA_UTIL,

    /**
     * @brief Tx BW Best Effort SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_BW_BEST_EFFORT_SLA_BPS,

    /**
     * @brief Tx BW Guaranteed SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_BW_GUARANTEED_SLA_UTIL,

    /**
     * @brief Tx BW Guaranteed SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_BW_GUARANTEED_SLA_BPS,

    /**
     * @brief Tx BW Total SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_BW_TOTAL_SLA_UTIL,

    /**
     * @brief Tx BW Total SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_BW_TOTAL_SLA_BPS,

    /**
     * @brief Tx Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_BANDWIDTH_REQS,

    /**
     * @brief Tx Control Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_CONTROL_OCTETS,

    /**
     * @brief Tx Encrypted Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_ENCRYPTED_FRAMES,

    /**
     * @brief Tx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_ENCRYPTED_OCTETS,

    /**
     * @brief Tx Encrypted Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_ENCRYPTED_SEGMENTS,

    /**
     * @brief Tx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_FRAMES_1024_1518,

    /**
     * @brief Tx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_FRAMES_128_255,

    /**
     * @brief Tx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_FRAMES_1519_PLUS,

    /**
     * @brief Tx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_FRAMES_256_511,

    /**
     * @brief Tx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_FRAMES_512_1023,

    /**
     * @brief Tx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_FRAMES_64,

    /**
     * @brief Tx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_FRAMES_65_127,

    /**
     * @brief Tx Frames Broadcast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_FRAMES_BROADCAST,

    /**
     * @brief Tx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_FRAMES_GREEN,

    /**
     * @brief Tx Frames Multicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_FRAMES_MULTICAST,

    /**
     * @brief Tx Frames Unicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_FRAMES_UNICAST,

    /**
     * @brief Tx Mpcp Ploam
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_MPCP_PLOAM,

    /**
     * @brief Tx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Tx Plain Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_PLAIN_FRAMES,

    /**
     * @brief Tx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_PLAIN_OCTETS,

    /**
     * @brief Tx Plain Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_PLAIN_SEGMENTS,

    /**
     * @brief Tx Ploam Ds Ranging Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_PLOAM_DS_RANGING_TIME,

    /**
     * @brief Tx Rate Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_RATE_BPS,

    /**
     * @brief Tx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_TOTAL_OCTETS,

    /**
     * @brief Tx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_UNICAST_OCTETS,

    /**
     * @brief Tx Upstream Slots
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_TX_UPSTREAM_SLOTS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_olt_pon_service_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned OLT PON service (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_olt_pon_service_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_olt_pon_service_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_olt_pon_service_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned OLT PON service
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_olt_pon_service_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU enhanced TC pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_enhanced_tc_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_ME_ID,

    /**
     * @brief Lods Event Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_LODS_EVENT_COUNT,

    /**
     * @brief Lods Event Restored Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_LODS_EVENT_RESTORED_COUNT,

    /**
     * @brief Fragment Xgem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_FRAGMENT_XGEM_FRAMES,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief ONU Reactivation By Lods Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_ONU_REACTIVATION_BY_LODS_EVENTS,

    /**
     * @brief Psbd HEC Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_PSBD_HEC_ERROR_COUNT,

    /**
     * @brief Received Bytes In Nonidle Xgem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_RECEIVED_BYTES_IN_NONIDLE_XGEM_FRAMES,

    /**
     * @brief Transmitted Bytes In Nonidle Xgem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_TRANSMITTED_BYTES_IN_NONIDLE_XGEM_FRAMES,

    /**
     * @brief Transmitted Xgem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_TRANSMITTED_XGEM_FRAMES,

    /**
     * @brief Unknown Profile Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_UNKNOWN_PROFILE_COUNT,

    /**
     * @brief Xgem HEC Lost Words Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_XGEM_HEC_LOST_WORDS_COUNT,

    /**
     * @brief Xgem Key Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_XGEM_KEY_ERRORS,

    /**
     * @brief Xgem HEC Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_XGEM_HEC_ERROR_COUNT,

    /**
     * @brief Xgtc HEC Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_XGTC_HEC_ERROR_COUNT,

    /**
     * @brief Threshold Data 64 Bit ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_THRESHOLD_DATA_64_BIT_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_enhanced_tc_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU enhanced TC pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_enhanced_tc_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_enhanced_tc_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_enhanced_tc_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU enhanced TC pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_enhanced_tc_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU ethernet frame extended pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_ME_ID,

    /**
     * @brief Direction
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_DIRECTION,

    /**
     * @brief Broadcast Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_BROADCAST_FRAMES,

    /**
     * @brief CRC Errored Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_CRC_ERRORED_FRAMES,

    /**
     * @brief Drop Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_DROP_EVENTS,

    /**
     * @brief Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES,

    /**
     * @brief Frames 1024 To 1518 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES_1024_TO_1518_OCTETS,

    /**
     * @brief Frames 128 To 255 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES_128_TO_255_OCTETS,

    /**
     * @brief Frames 256 To 511 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES_256_TO_511_OCTETS,

    /**
     * @brief Frames 512 To 1023 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES_512_TO_1023_OCTETS,

    /**
     * @brief Frames 64 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES_64_OCTETS,

    /**
     * @brief Frames 65 To 127 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES_65_TO_127_OCTETS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Multicast Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_MULTICAST_FRAMES,

    /**
     * @brief Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_OCTETS,

    /**
     * @brief Oversize Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_OVERSIZE_FRAMES,

    /**
     * @brief Undersize Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_UNDERSIZE_FRAMES,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU ethernet frame extended pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU ethernet frame extended pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU ethernet frame extended pm 64bit (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_ME_ID,

    /**
     * @brief Direction
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_DIRECTION,

    /**
     * @brief Broadcast Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_BROADCAST_FRAMES,

    /**
     * @brief CRC Errored Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_CRC_ERRORED_FRAMES,

    /**
     * @brief Drop Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_DROP_EVENTS,

    /**
     * @brief Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES,

    /**
     * @brief Frames 1024 To 1518 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES_1024_TO_1518_OCTETS,

    /**
     * @brief Frames 128 To 255 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES_128_TO_255_OCTETS,

    /**
     * @brief Frames 256 To 511 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES_256_TO_511_OCTETS,

    /**
     * @brief Frames 512 To 1023 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES_512_TO_1023_OCTETS,

    /**
     * @brief Frames 64 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES_64_OCTETS,

    /**
     * @brief Frames 65 To 127 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES_65_TO_127_OCTETS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Multicast Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_MULTICAST_FRAMES,

    /**
     * @brief Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_OCTETS,

    /**
     * @brief Oversize Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_OVERSIZE_FRAMES,

    /**
     * @brief Undersize Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_UNDERSIZE_FRAMES,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU ethernet frame extended pm 64bit (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU ethernet frame extended pm 64bit
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU ethernet frame pm downstream (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_ME_ID,

    /**
     * @brief Broadcast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_BROADCAST_PACKETS,

    /**
     * @brief CRC Errored Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_CRC_ERRORED_PACKETS,

    /**
     * @brief Drop Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_DROP_EVENTS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Multicast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_MULTICAST_PACKETS,

    /**
     * @brief Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_OCTETS,

    /**
     * @brief Oversize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_OVERSIZE_PACKETS,

    /**
     * @brief Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS,

    /**
     * @brief Packets 1024 To 1518 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_1024_TO_1518_OCTETS,

    /**
     * @brief Packets 128 To 255 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_128_TO_255_OCTETS,

    /**
     * @brief Packets 256 To 511 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_256_TO_511_OCTETS,

    /**
     * @brief Packets 512 To 1023 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_512_TO_1023_OCTETS,

    /**
     * @brief Packets 64 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_64_OCTETS,

    /**
     * @brief Packets 65 To 127 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_65_TO_127_OCTETS,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Undersize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_UNDERSIZE_PACKETS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU ethernet frame pm downstream (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU ethernet frame pm downstream
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU ethernet frame pm upstream (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_ME_ID,

    /**
     * @brief Broadcast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_BROADCAST_PACKETS,

    /**
     * @brief CRC Errored Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_CRC_ERRORED_PACKETS,

    /**
     * @brief Drop Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_DROP_EVENTS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Multicast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_MULTICAST_PACKETS,

    /**
     * @brief Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_OCTETS,

    /**
     * @brief Oversize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_OVERSIZE_PACKETS,

    /**
     * @brief Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS,

    /**
     * @brief Packets 1024 To 1518 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_1024_TO_1518_OCTETS,

    /**
     * @brief Packets 128 To 255 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_128_TO_255_OCTETS,

    /**
     * @brief Packets 256 To 511 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_256_TO_511_OCTETS,

    /**
     * @brief Packets 512 To 1023 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_512_TO_1023_OCTETS,

    /**
     * @brief Packets 64 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_64_OCTETS,

    /**
     * @brief Packets 65 To 127 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_65_TO_127_OCTETS,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Undersize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_UNDERSIZE_PACKETS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU ethernet frame pm upstream (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU ethernet frame pm upstream
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU ethernet pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_ethernet_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_ME_ID,

    /**
     * @brief Alignment Error Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_ALIGNMENT_ERROR_COUNTER,

    /**
     * @brief Buffer Overflows On Receive
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_BUFFER_OVERFLOWS_ON_RECEIVE,

    /**
     * @brief Buffer Overflows On Transmit
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_BUFFER_OVERFLOWS_ON_TRANSMIT,

    /**
     * @brief Carrier Sense Error Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_CARRIER_SENSE_ERROR_COUNTER,

    /**
     * @brief Deferred Transmission Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_DEFERRED_TRANSMISSION_COUNTER,

    /**
     * @brief Excessive Collision Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_EXCESSIVE_COLLISION_COUNTER,

    /**
     * @brief FCS Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_FCS_ERRORS,

    /**
     * @brief Frames Too Long
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_FRAMES_TOO_LONG,

    /**
     * @brief Internal MAC Receive Error Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_INTERNAL_MAC_RECEIVE_ERROR_COUNTER,

    /**
     * @brief Internal MAC Transmit Error Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_INTERNAL_MAC_TRANSMIT_ERROR_COUNTER,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Late Collision Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_LATE_COLLISION_COUNTER,

    /**
     * @brief Multiple Collisions Frame Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_MULTIPLE_COLLISIONS_FRAME_COUNTER,

    /**
     * @brief Single Collision Frame Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_SINGLE_COLLISION_FRAME_COUNTER,

    /**
     * @brief SQE Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_SQE_COUNTER,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_ethernet_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU ethernet pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_ethernet_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_ethernet_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_ethernet_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU ethernet pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU ethernet pm3 (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_ethernet_pm3_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_ME_ID,

    /**
     * @brief Broadcast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_BROADCAST_PACKETS,

    /**
     * @brief Drop Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_DROP_EVENTS,

    /**
     * @brief Fragments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_FRAGMENTS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Jabbers
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_JABBERS,

    /**
     * @brief Multicast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_MULTICAST_PACKETS,

    /**
     * @brief Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_OCTETS,

    /**
     * @brief Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_PACKETS,

    /**
     * @brief Packets 64 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_PACKETS_64_OCTETS,

    /**
     * @brief Packets 65 To 127 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_PACKETS_65_TO_127_OCTETS,

    /**
     * @brief Packets 128 To 255 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_PACKETS_128_TO_255_OCTETS,

    /**
     * @brief Packets 256 To 511 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_PACKETS_256_TO_511_OCTETS,

    /**
     * @brief Packets 512 To 1023 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_PACKETS_512_TO_1023_OCTETS,

    /**
     * @brief Packets 1024 To 1518 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_PACKETS_1024_TO_1518_OCTETS,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Undersize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_UNDERSIZE_PACKETS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_ethernet_pm3_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU ethernet pm3 (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_ethernet_pm3_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_ethernet_pm3_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_ethernet_pm3_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU ethernet pm3
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_pm3_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU FEC pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_fec_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_ME_ID,

    /**
     * @brief Corrected Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_CORRECTED_BYTES,

    /**
     * @brief Corrected Code Words
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_CORRECTED_CODE_WORDS,

    /**
     * @brief FEC Seconds
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_FEC_SECONDS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Total Code Words
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_TOTAL_CODE_WORDS,

    /**
     * @brief Uncorrectable Code Words
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_UNCORRECTABLE_CODE_WORDS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_fec_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU FEC pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_fec_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_fec_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_fec_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU FEC pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_fec_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU gal ethernet pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_gal_ethernet_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_ME_ID,

    /**
     * @brief Discarded Downstream Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_DISCARDED_DOWNSTREAM_FRAMES,

    /**
     * @brief Discarded Upstream Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_DISCARDED_UPSTREAM_FRAMES,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_gal_ethernet_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU gal ethernet pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_gal_ethernet_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_gal_ethernet_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_gal_ethernet_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU gal ethernet pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_gal_ethernet_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU gem port network CTP pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ME_ID,

    /**
     * @brief Encryption Key Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ENCRYPTION_KEY_ERRORS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Received Gem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_RECEIVED_GEM_FRAMES,

    /**
     * @brief Received Payload Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_RECEIVED_PAYLOAD_BYTES,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Transmitted Gem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_TRANSMITTED_GEM_FRAMES,

    /**
     * @brief Transmitted Payload Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_TRANSMITTED_PAYLOAD_BYTES,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU gem port network CTP pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU gem port network CTP pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU IP host Performance Monitoring History data (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_ME_ID,

    /**
     * @brief DNS Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_DNS_ERRORS,

    /**
     * @brief DHCP Timeouts
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_DHCP_TIMEOUTS,

    /**
     * @brief ICMP Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_ICMP_ERRORS,

    /**
     * @brief Internal Error
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_INTERNAL_ERROR,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_INTERVAL_END_TIME,

    /**
     * @brief IP Address Conflict
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_IP_ADDRESS_CONFLICT,

    /**
     * @brief Out Of Memory
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_OUT_OF_MEMORY,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU IP host Performance Monitoring History data (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU IP host Performance Monitoring History data
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU MAC bridge port pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_ME_ID,

    /**
     * @brief Delay Exceeded Discard Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_DELAY_EXCEEDED_DISCARD_COUNTER,

    /**
     * @brief Forwarded Frame Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_FORWARDED_FRAME_COUNTER,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief MTU Exceeded Discard Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_MTU_EXCEEDED_DISCARD_COUNTER,

    /**
     * @brief Received And Discarded Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_RECEIVED_AND_DISCARDED_COUNTER,

    /**
     * @brief Received Frame Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_RECEIVED_FRAME_COUNTER,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU MAC bridge port pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_mac_bridge_port_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_mac_bridge_port_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU MAC bridge port pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU operational pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_operational_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_ME_ID,

    /**
     * @brief CPU Percent Utilization
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_CPU_PERCENT_UTILIZATION,

    /**
     * @brief Errors In Operations
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_ERRORS_IN_OPERATIONS,

    /**
     * @brief Flash Size Available
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_FLASH_SIZE_AVAILABLE,

    /**
     * @brief Flash Utilization
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_FLASH_UTILIZATION,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Ram Size Available
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_RAM_SIZE_AVAILABLE,

    /**
     * @brief Ram Utilization
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_RAM_UTILIZATION,

    /**
     * @brief Software Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_SOFTWARE_ERRORS,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Temperature Sensor Description
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_TEMPERATURE_SENSOR_DESCRIPTION,

    /**
     * @brief Temperature Sensor Value
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_TEMPERATURE_SENSOR_VALUE,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_operational_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU operational pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_operational_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_operational_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_operational_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU operational pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_operational_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU PON (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_pon_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_PON_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_PON_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_PON_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_PON_ATTR_ONU_STATS_ID,

    /**
     * @brief Rx Optical Level
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_PON_ATTR_RX_OPTICAL_LEVEL,

    /**
     * @brief Tx Optical Level
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_PON_ATTR_TX_OPTICAL_LEVEL,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_PON_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_PON_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_PON_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_pon_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU PON (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_pon_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_pon_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_pon_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU PON
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_pon_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU rs232 rs485 Performance Monitoring History data (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_ME_ID,

    /**
     * @brief Incoming Bytes From Chip
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_INCOMING_BYTES_FROM_CHIP,

    /**
     * @brief Incoming Bytes From PON
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_INCOMING_BYTES_FROM_PON,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Outgoing Bytes From Chip
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_OUTGOING_BYTES_FROM_CHIP,

    /**
     * @brief Outgoing Bytes From PON
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_OUTGOING_BYTES_FROM_PON,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU rs232 rs485 Performance Monitoring History data (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU rs232 rs485 Performance Monitoring History data
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU TCP UDP Performance Monitoring History data (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_ME_ID,

    /**
     * @brief Accept Failed
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_ACCEPT_FAILED,

    /**
     * @brief Bind Failed
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_BIND_FAILED,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Listen Failed
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_LISTEN_FAILED,

    /**
     * @brief Select Failed
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_SELECT_FAILED,

    /**
     * @brief Socket Failed
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_SOCKET_FAILED,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU TCP UDP Performance Monitoring History data (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU TCP UDP Performance Monitoring History data
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU XG PON downstream Management pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ME_ID,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Ploam Mic Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_PLOAM_MIC_ERROR_COUNT,

    /**
     * @brief Downstream Ploam Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_DOWNSTREAM_PLOAM_MESSAGE_COUNT,

    /**
     * @brief Profile Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_PROFILE_MESSAGES_RECEIVED,

    /**
     * @brief Ranging Time Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_RANGING_TIME_MESSAGES_RECEIVED,

    /**
     * @brief Deactivate ONU ID Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_DEACTIVATE_ONU_ID_MESSAGES_RECEIVED,

    /**
     * @brief Disable Serial Number Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_DISABLE_SERIAL_NUMBER_MESSAGES_RECEIVED,

    /**
     * @brief Request Registration Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_REQUEST_REGISTRATION_MESSAGES_RECEIVED,

    /**
     * @brief Assign Allocation ID Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ASSIGN_ALLOC_ID_MESSAGES_RECEIVED,

    /**
     * @brief Key Control Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_KEY_CONTROL_MESSAGES_RECEIVED,

    /**
     * @brief Sleep Allow Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_SLEEP_ALLOW_MESSAGES_RECEIVED,

    /**
     * @brief Baseline OMCI Messages Received Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_BASELINE_OMCI_MESSAGES_RECEIVED_COUNT,

    /**
     * @brief Extended OMCI Messages Received Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_EXTENDED_OMCI_MESSAGES_RECEIVED_COUNT,

    /**
     * @brief Assign ONU ID OMCI Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ASSIGN_ONU_ID_OMCI_MESSAGES_RECEIVED,

    /**
     * @brief OMCI Mic Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_OMCI_MIC_ERROR_COUNT,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU XG PON downstream Management pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU XG PON downstream Management pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics binned ONU XG PON upstream Management pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ME_ID,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Upstream Ploam Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_UPSTREAM_PLOAM_MESSAGE_COUNT,

    /**
     * @brief Serial Number Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_SERIAL_NUMBER_MESSAGE_COUNT,

    /**
     * @brief Registration Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_REGISTRATION_MESSAGE_COUNT,

    /**
     * @brief Key Report Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_KEY_REPORT_MESSAGE_COUNT,

    /**
     * @brief Acknowledge Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ACKNOWLEDGE_MESSAGE_COUNT,

    /**
     * @brief Sleep Request Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_SLEEP_REQUEST_MESSAGE_COUNT,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics binned ONU XG PON upstream Management pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics binned ONU XG PON upstream Management pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating OLT PON (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_olt_pon_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_START,

    /**
     * @brief Rx Optical Level
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_RX_OPTICAL_LEVEL,

    /**
     * @brief Tx Optical Level
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_TX_OPTICAL_LEVEL,

    /**
     * @brief Rx Deregistrations
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_RX_DEREGISTRATIONS,

    /**
     * @brief Rx Registrations
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_RX_REGISTRATIONS,

    /**
     * @brief Fiber Distance
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_FIBER_DISTANCE,

    /**
     * @brief Equalization Delay
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_EQUALIZATION_DELAY,

    /**
     * @brief Round Trip Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_ROUND_TRIP_TIME,

    /**
     * @brief One Way Delay
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_ONE_WAY_DELAY,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_olt_pon_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating OLT PON (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_olt_pon_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_olt_pon_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_olt_pon_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating OLT PON
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_olt_pon_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating OLT PON omcc (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_olt_pon_omcc_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_START,

    /**
     * @brief Bad Key Exchanges
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_BAD_KEY_EXCHANGES,

    /**
     * @brief Good Key Exchanges
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_GOOD_KEY_EXCHANGES,

    /**
     * @brief OMCI OAM Requests
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_OMCI_OAM_REQUESTS,

    /**
     * @brief OMCI OAM Resp Time Avg
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_OMCI_OAM_RESP_TIME_AVG,

    /**
     * @brief OMCI OAM Resp Time Max
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_OMCI_OAM_RESP_TIME_MAX,

    /**
     * @brief OMCI OAM Resp Time Min
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_OMCI_OAM_RESP_TIME_MIN,

    /**
     * @brief OMCI OAM Responses
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_OMCI_OAM_RESPONSES,

    /**
     * @brief OMCI OAM Time To Send Avg
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_OMCI_OAM_TIME_TO_SEND_AVG,

    /**
     * @brief OMCI OAM Time To Send Max
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_OMCI_OAM_TIME_TO_SEND_MAX,

    /**
     * @brief OMCI OAM Time To Send Min
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_OMCI_OAM_TIME_TO_SEND_MIN,

    /**
     * @brief OMCI OAM Timeouts
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_OMCI_OAM_TIMEOUTS,

    /**
     * @brief Ploam Timeouts
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_PLOAM_TIMEOUTS,

    /**
     * @brief Rx All Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_ALL_BANDWIDTH_REQS,

    /**
     * @brief Rx Bad Icv Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_BAD_ICV_DROPS,

    /**
     * @brief Rx Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_BANDWIDTH_REQS,

    /**
     * @brief Rx Crc32 Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_CRC32_DROPS,

    /**
     * @brief Rx Crc8 Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_CRC8_ERRORS,

    /**
     * @brief Rx Control Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_CONTROL_OCTETS,

    /**
     * @brief Rx Empty Slots
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_EMPTY_SLOTS,

    /**
     * @brief Rx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_ENCRYPTED_OCTETS,

    /**
     * @brief Rx Encrypted Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_ENCRYPTED_SEGMENTS,

    /**
     * @brief Rx Errored Bip Bits
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_ERRORED_BIP_BITS,

    /**
     * @brief Rx Errored Bip Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_ERRORED_BIP_BLOCKS,

    /**
     * @brief Rx FEC Corrected Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FEC_CORRECTED_BLOCKS,

    /**
     * @brief Rx FEC Corrections
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FEC_CORRECTIONS,

    /**
     * @brief Rx FEC Good Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FEC_GOOD_BLOCKS,

    /**
     * @brief Rx FEC Uncorrectable Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FEC_UNCORRECTABLE_BLOCKS,

    /**
     * @brief Rx Filtered Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FILTERED_FRAMES,

    /**
     * @brief Rx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FRAMES_1024_1518,

    /**
     * @brief Rx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FRAMES_128_255,

    /**
     * @brief Rx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FRAMES_1519_PLUS,

    /**
     * @brief Rx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FRAMES_256_511,

    /**
     * @brief Rx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FRAMES_512_1023,

    /**
     * @brief Rx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FRAMES_64,

    /**
     * @brief Rx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FRAMES_65_127,

    /**
     * @brief Rx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_FRAMES_GREEN,

    /**
     * @brief Rx Good Bip Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_GOOD_BIP_BLOCKS,

    /**
     * @brief Rx HEC Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_HEC_ERRORS,

    /**
     * @brief Rx Idle Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_IDLE_OCTETS,

    /**
     * @brief Rx Key Mismatch Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_KEY_MISMATCH_OCTETS,

    /**
     * @brief Rx Mpcp Ploam
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_MPCP_PLOAM,

    /**
     * @brief Rx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Rx OAM
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_OAM,

    /**
     * @brief Rx Overflow Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_OVERFLOW_DROPS,

    /**
     * @brief Rx Overflow Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_OVERFLOW_OCTETS,

    /**
     * @brief Rx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_PLAIN_OCTETS,

    /**
     * @brief Rx Plain Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_PLAIN_SEGMENTS,

    /**
     * @brief Rx Ploam Mic Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_PLOAM_MIC_ERRORS,

    /**
     * @brief Rx Rate Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_RATE_BPS,

    /**
     * @brief Rx Security Drop Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_SECURITY_DROP_OCTETS,

    /**
     * @brief Rx Too Long Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_TOO_LONG_DROPS,

    /**
     * @brief Rx Too Short Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_TOO_SHORT_DROPS,

    /**
     * @brief Rx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_TOTAL_OCTETS,

    /**
     * @brief Rx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_RX_UNICAST_OCTETS,

    /**
     * @brief Tx Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_BANDWIDTH_REQS,

    /**
     * @brief Tx Control Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_CONTROL_OCTETS,

    /**
     * @brief Tx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_ENCRYPTED_OCTETS,

    /**
     * @brief Tx Encrypted Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_ENCRYPTED_SEGMENTS,

    /**
     * @brief Tx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_FRAMES_1024_1518,

    /**
     * @brief Tx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_FRAMES_128_255,

    /**
     * @brief Tx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_FRAMES_1519_PLUS,

    /**
     * @brief Tx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_FRAMES_256_511,

    /**
     * @brief Tx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_FRAMES_512_1023,

    /**
     * @brief Tx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_FRAMES_64,

    /**
     * @brief Tx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_FRAMES_65_127,

    /**
     * @brief Tx Frames Broadcast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_FRAMES_BROADCAST,

    /**
     * @brief Tx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_FRAMES_GREEN,

    /**
     * @brief Tx Frames Multicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_FRAMES_MULTICAST,

    /**
     * @brief Tx Frames Unicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_FRAMES_UNICAST,

    /**
     * @brief Tx Grant Ups Tq
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_GRANT_UPS_TQ,

    /**
     * @brief Tx Mpcp Ploam
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_MPCP_PLOAM,

    /**
     * @brief Tx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Tx OAM
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_OAM,

    /**
     * @brief Tx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_PLAIN_OCTETS,

    /**
     * @brief Tx Plain Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_PLAIN_SEGMENTS,

    /**
     * @brief Tx Ploam Ds Ranging Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_PLOAM_DS_RANGING_TIME,

    /**
     * @brief Tx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_TOTAL_OCTETS,

    /**
     * @brief Tx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_UNICAST_OCTETS,

    /**
     * @brief Tx Upstream Slots
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_TX_UPSTREAM_SLOTS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_OMCC_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_olt_pon_omcc_attr_t;


/**
 * @brief Get attribute for PON ONU statistics accumulating OLT PON omcc (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_olt_pon_omcc_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_olt_pon_omcc_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_olt_pon_omcc_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating OLT PON omcc
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_olt_pon_omcc_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating OLT PON service (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_olt_pon_service_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_ONU_ID,

    /**
     * @brief Bad Key Exchanges
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_BAD_KEY_EXCHANGES,

    /**
     * @brief Enable Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_ENABLE_COUNT,

    /**
     * @brief Good Key Exchanges
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_GOOD_KEY_EXCHANGES,

    /**
     * @brief OMCI OAM Requests
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_OMCI_OAM_REQUESTS,

    /**
     * @brief OMCI OAM Resp Time Avg
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_OMCI_OAM_RESP_TIME_AVG,

    /**
     * @brief OMCI OAM Resp Time Max
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_OMCI_OAM_RESP_TIME_MAX,

    /**
     * @brief OMCI OAM Resp Time Min
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_OMCI_OAM_RESP_TIME_MIN,

    /**
     * @brief OMCI OAM Responses
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_OMCI_OAM_RESPONSES,

    /**
     * @brief OMCI OAM Time To Send Avg
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_OMCI_OAM_TIME_TO_SEND_AVG,

    /**
     * @brief OMCI OAM Time To Send Max
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_OMCI_OAM_TIME_TO_SEND_MAX,

    /**
     * @brief OMCI OAM Time To Send Min
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_OMCI_OAM_TIME_TO_SEND_MIN,

    /**
     * @brief OMCI OAM Timeouts
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_OMCI_OAM_TIMEOUTS,

    /**
     * @brief Ploam Timeouts
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_PLOAM_TIMEOUTS,

    /**
     * @brief Rx All Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_ALL_BANDWIDTH_REQS,

    /**
     * @brief Rx BW Best Effort SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_BW_BEST_EFFORT_SLA_UTIL,

    /**
     * @brief Rx BW Best Effort SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_BW_BEST_EFFORT_SLA_BPS,

    /**
     * @brief Rx BW Fixed SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_BW_FIXED_SLA_UTIL,

    /**
     * @brief Rx BW Fixed SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_BW_FIXED_SLA_BPS,

    /**
     * @brief Rx BW Guaranteed SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_BW_GUARANTEED_SLA_UTIL,

    /**
     * @brief Rx BW Guaranteed SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_BW_GUARANTEED_SLA_BPS,

    /**
     * @brief Rx BW Total SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_BW_TOTAL_SLA_UTIL,

    /**
     * @brief Rx BW Total SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_BW_TOTAL_SLA_BPS,

    /**
     * @brief Rx Bad Icv Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_BAD_ICV_DROPS,

    /**
     * @brief Rx Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_BANDWIDTH_REQS,

    /**
     * @brief Rx Crc32 Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_CRC32_DROPS,

    /**
     * @brief Rx Crc8 Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_CRC8_ERRORS,

    /**
     * @brief Rx Control Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_CONTROL_OCTETS,

    /**
     * @brief Rx Empty Slots
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_EMPTY_SLOTS,

    /**
     * @brief Rx Encrypted Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_ENCRYPTED_FRAMES,

    /**
     * @brief Rx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_ENCRYPTED_OCTETS,

    /**
     * @brief Rx Encrypted Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_ENCRYPTED_SEGMENTS,

    /**
     * @brief Rx Errored Bip Bits
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_ERRORED_BIP_BITS,

    /**
     * @brief Rx Errored Bip Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_ERRORED_BIP_BLOCKS,

    /**
     * @brief Rx FEC Corrected Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FEC_CORRECTED_BLOCKS,

    /**
     * @brief Rx FEC Corrections
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FEC_CORRECTIONS,

    /**
     * @brief Rx FEC Good Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FEC_GOOD_BLOCKS,

    /**
     * @brief Rx FEC Uncorrectable Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FEC_UNCORRECTABLE_BLOCKS,

    /**
     * @brief Rx Filtered Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FILTERED_FRAMES,

    /**
     * @brief Rx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FRAMES_1024_1518,

    /**
     * @brief Rx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FRAMES_128_255,

    /**
     * @brief Rx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FRAMES_1519_PLUS,

    /**
     * @brief Rx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FRAMES_256_511,

    /**
     * @brief Rx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FRAMES_512_1023,

    /**
     * @brief Rx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FRAMES_64,

    /**
     * @brief Rx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FRAMES_65_127,

    /**
     * @brief Rx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_FRAMES_GREEN,

    /**
     * @brief Rx Good Bip Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_GOOD_BIP_BLOCKS,

    /**
     * @brief Rx HEC Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_HEC_ERRORS,

    /**
     * @brief Rx Idle Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_IDLE_OCTETS,

    /**
     * @brief Rx Key Mismatch Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_KEY_MISMATCH_OCTETS,

    /**
     * @brief Rx Mpcp Ploam
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_MPCP_PLOAM,

    /**
     * @brief Rx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Rx Overflow Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_OVERFLOW_DROPS,

    /**
     * @brief Rx Overflow Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_OVERFLOW_OCTETS,

    /**
     * @brief Rx Plain Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_PLAIN_FRAMES,

    /**
     * @brief Rx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_PLAIN_OCTETS,

    /**
     * @brief Rx Plain Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_PLAIN_SEGMENTS,

    /**
     * @brief Rx Ploam Mic Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_PLOAM_MIC_ERRORS,

    /**
     * @brief Rx Rate Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_RATE_BPS,

    /**
     * @brief Rx Security Drop Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_SECURITY_DROP_OCTETS,

    /**
     * @brief Rx Too Long Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_TOO_LONG_DROPS,

    /**
     * @brief Rx Too Short Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_TOO_SHORT_DROPS,

    /**
     * @brief Rx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_TOTAL_OCTETS,

    /**
     * @brief Rx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_RX_UNICAST_OCTETS,

    /**
     * @brief Tx BW Best Effort SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_BW_BEST_EFFORT_SLA_UTIL,

    /**
     * @brief Tx BW Best Effort SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_BW_BEST_EFFORT_SLA_BPS,

    /**
     * @brief Tx BW Guaranteed SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_BW_GUARANTEED_SLA_UTIL,

    /**
     * @brief Tx BW Guaranteed SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_BW_GUARANTEED_SLA_BPS,

    /**
     * @brief Tx BW Total SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_BW_TOTAL_SLA_UTIL,

    /**
     * @brief Tx BW Total SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_BW_TOTAL_SLA_BPS,

    /**
     * @brief Tx Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_BANDWIDTH_REQS,

    /**
     * @brief Tx Control Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_CONTROL_OCTETS,

    /**
     * @brief Tx Encrypted Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_ENCRYPTED_FRAMES,

    /**
     * @brief Tx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_ENCRYPTED_OCTETS,

    /**
     * @brief Tx Encrypted Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_ENCRYPTED_SEGMENTS,

    /**
     * @brief Tx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_FRAMES_1024_1518,

    /**
     * @brief Tx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_FRAMES_128_255,

    /**
     * @brief Tx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_FRAMES_1519_PLUS,

    /**
     * @brief Tx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_FRAMES_256_511,

    /**
     * @brief Tx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_FRAMES_512_1023,

    /**
     * @brief Tx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_FRAMES_64,

    /**
     * @brief Tx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_FRAMES_65_127,

    /**
     * @brief Tx Frames Broadcast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_FRAMES_BROADCAST,

    /**
     * @brief Tx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_FRAMES_GREEN,

    /**
     * @brief Tx Frames Multicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_FRAMES_MULTICAST,

    /**
     * @brief Tx Frames Unicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_FRAMES_UNICAST,

    /**
     * @brief Tx Mpcp Ploam
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_MPCP_PLOAM,

    /**
     * @brief Tx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Tx Plain Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_PLAIN_FRAMES,

    /**
     * @brief Tx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_PLAIN_OCTETS,

    /**
     * @brief Tx Plain Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_PLAIN_SEGMENTS,

    /**
     * @brief Tx Ploam Ds Ranging Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_PLOAM_DS_RANGING_TIME,

    /**
     * @brief Tx Rate Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_RATE_BPS,

    /**
     * @brief Tx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_TOTAL_OCTETS,

    /**
     * @brief Tx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_UNICAST_OCTETS,

    /**
     * @brief Tx Upstream Slots
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_TX_UPSTREAM_SLOTS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_olt_pon_service_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating OLT PON service (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_olt_pon_service_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_olt_pon_service_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_olt_pon_service_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating OLT PON service
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_olt_pon_service_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU enhanced TC pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_ONU_ID,

    /**
     * @brief Lods Event Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_LODS_EVENT_COUNT,

    /**
     * @brief Lods Event Restored Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_LODS_EVENT_RESTORED_COUNT,

    /**
     * @brief Fragment Xgem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_FRAGMENT_XGEM_FRAMES,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief ONU Reactivation By Lods Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_ONU_REACTIVATION_BY_LODS_EVENTS,

    /**
     * @brief Psbd HEC Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_PSBD_HEC_ERROR_COUNT,

    /**
     * @brief Received Bytes In Nonidle Xgem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_RECEIVED_BYTES_IN_NONIDLE_XGEM_FRAMES,

    /**
     * @brief Transmitted Bytes In Nonidle Xgem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_TRANSMITTED_BYTES_IN_NONIDLE_XGEM_FRAMES,

    /**
     * @brief Transmitted Xgem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_TRANSMITTED_XGEM_FRAMES,

    /**
     * @brief Unknown Profile Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_UNKNOWN_PROFILE_COUNT,

    /**
     * @brief Xgem HEC Lost Words Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_XGEM_HEC_LOST_WORDS_COUNT,

    /**
     * @brief Xgem Key Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_XGEM_KEY_ERRORS,

    /**
     * @brief Xgem HEC Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_XGEM_HEC_ERROR_COUNT,

    /**
     * @brief Xgtc HEC Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_XGTC_HEC_ERROR_COUNT,

    /**
     * @brief Threshold Data 64 Bit ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_THRESHOLD_DATA_64_BIT_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU enhanced TC pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_enhanced_tc_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_enhanced_tc_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU enhanced TC pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU ethernet frame extended pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_ONU_ID,

    /**
     * @brief Direction
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_DIRECTION,

    /**
     * @brief Broadcast Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_BROADCAST_FRAMES,

    /**
     * @brief CRC Errored Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_CRC_ERRORED_FRAMES,

    /**
     * @brief Drop Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_DROP_EVENTS,

    /**
     * @brief Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES,

    /**
     * @brief Frames 1024 To 1518 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES_1024_TO_1518_OCTETS,

    /**
     * @brief Frames 128 To 255 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES_128_TO_255_OCTETS,

    /**
     * @brief Frames 256 To 511 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES_256_TO_511_OCTETS,

    /**
     * @brief Frames 512 To 1023 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES_512_TO_1023_OCTETS,

    /**
     * @brief Frames 64 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES_64_OCTETS,

    /**
     * @brief Frames 65 To 127 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_FRAMES_65_TO_127_OCTETS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Multicast Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_MULTICAST_FRAMES,

    /**
     * @brief Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_OCTETS,

    /**
     * @brief Oversize Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_OVERSIZE_FRAMES,

    /**
     * @brief Undersize Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_UNDERSIZE_FRAMES,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU ethernet frame extended pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU ethernet frame extended pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU ethernet frame extended pm 64bit (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_ONU_ID,

    /**
     * @brief Direction
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_DIRECTION,

    /**
     * @brief Broadcast Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_BROADCAST_FRAMES,

    /**
     * @brief CRC Errored Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_CRC_ERRORED_FRAMES,

    /**
     * @brief Drop Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_DROP_EVENTS,

    /**
     * @brief Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES,

    /**
     * @brief Frames 1024 To 1518 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES_1024_TO_1518_OCTETS,

    /**
     * @brief Frames 128 To 255 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES_128_TO_255_OCTETS,

    /**
     * @brief Frames 256 To 511 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES_256_TO_511_OCTETS,

    /**
     * @brief Frames 512 To 1023 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES_512_TO_1023_OCTETS,

    /**
     * @brief Frames 64 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES_64_OCTETS,

    /**
     * @brief Frames 65 To 127 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_FRAMES_65_TO_127_OCTETS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Multicast Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_MULTICAST_FRAMES,

    /**
     * @brief Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_OCTETS,

    /**
     * @brief Oversize Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_OVERSIZE_FRAMES,

    /**
     * @brief Undersize Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_UNDERSIZE_FRAMES,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU ethernet frame extended pm 64bit (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU ethernet frame extended pm 64bit
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU ethernet frame pm downstream (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_ONU_ID,

    /**
     * @brief Broadcast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_BROADCAST_PACKETS,

    /**
     * @brief CRC Errored Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_CRC_ERRORED_PACKETS,

    /**
     * @brief Drop Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_DROP_EVENTS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Multicast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_MULTICAST_PACKETS,

    /**
     * @brief Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_OCTETS,

    /**
     * @brief Oversize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_OVERSIZE_PACKETS,

    /**
     * @brief Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS,

    /**
     * @brief Packets 1024 To 1518 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_1024_TO_1518_OCTETS,

    /**
     * @brief Packets 128 To 255 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_128_TO_255_OCTETS,

    /**
     * @brief Packets 256 To 511 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_256_TO_511_OCTETS,

    /**
     * @brief Packets 512 To 1023 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_512_TO_1023_OCTETS,

    /**
     * @brief Packets 64 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_64_OCTETS,

    /**
     * @brief Packets 65 To 127 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_65_TO_127_OCTETS,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Undersize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_UNDERSIZE_PACKETS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU ethernet frame pm downstream (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU ethernet frame pm downstream
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU ethernet frame pm upstream (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_ONU_ID,

    /**
     * @brief Broadcast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_BROADCAST_PACKETS,

    /**
     * @brief CRC Errored Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_CRC_ERRORED_PACKETS,

    /**
     * @brief Drop Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_DROP_EVENTS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Multicast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_MULTICAST_PACKETS,

    /**
     * @brief Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_OCTETS,

    /**
     * @brief Oversize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_OVERSIZE_PACKETS,

    /**
     * @brief Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS,

    /**
     * @brief Packets 1024 To 1518 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_1024_TO_1518_OCTETS,

    /**
     * @brief Packets 128 To 255 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_128_TO_255_OCTETS,

    /**
     * @brief Packets 256 To 511 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_256_TO_511_OCTETS,

    /**
     * @brief Packets 512 To 1023 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_512_TO_1023_OCTETS,

    /**
     * @brief Packets 64 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_64_OCTETS,

    /**
     * @brief Packets 65 To 127 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_65_TO_127_OCTETS,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Undersize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_UNDERSIZE_PACKETS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU ethernet frame pm upstream (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU ethernet frame pm upstream
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU ethernet pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_ethernet_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_ONU_ID,

    /**
     * @brief Alignment Error Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_ALIGNMENT_ERROR_COUNTER,

    /**
     * @brief Buffer Overflows On Receive
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_BUFFER_OVERFLOWS_ON_RECEIVE,

    /**
     * @brief Buffer Overflows On Transmit
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_BUFFER_OVERFLOWS_ON_TRANSMIT,

    /**
     * @brief Carrier Sense Error Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_CARRIER_SENSE_ERROR_COUNTER,

    /**
     * @brief Deferred Transmission Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_DEFERRED_TRANSMISSION_COUNTER,

    /**
     * @brief Excessive Collision Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_EXCESSIVE_COLLISION_COUNTER,

    /**
     * @brief FCS Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_FCS_ERRORS,

    /**
     * @brief Frames Too Long
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_FRAMES_TOO_LONG,

    /**
     * @brief Internal MAC Receive Error Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_INTERNAL_MAC_RECEIVE_ERROR_COUNTER,

    /**
     * @brief Internal MAC Transmit Error Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_INTERNAL_MAC_TRANSMIT_ERROR_COUNTER,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Late Collision Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_LATE_COLLISION_COUNTER,

    /**
     * @brief Multiple Collisions Frame Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_MULTIPLE_COLLISIONS_FRAME_COUNTER,

    /**
     * @brief Single Collision Frame Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_SINGLE_COLLISION_FRAME_COUNTER,

    /**
     * @brief SQE Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_SQE_COUNTER,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_ethernet_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU ethernet pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_ethernet_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_ethernet_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_ethernet_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU ethernet pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU ethernet pm3 (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_ethernet_pm3_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_ONU_ID,

    /**
     * @brief Broadcast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_BROADCAST_PACKETS,

    /**
     * @brief Drop Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_DROP_EVENTS,

    /**
     * @brief Fragments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_FRAGMENTS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Jabbers
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_JABBERS,

    /**
     * @brief Multicast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_MULTICAST_PACKETS,

    /**
     * @brief Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_OCTETS,

    /**
     * @brief Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_PACKETS,

    /**
     * @brief Packets 64 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_PACKETS_64_OCTETS,

    /**
     * @brief Packets 65 To 127 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_PACKETS_65_TO_127_OCTETS,

    /**
     * @brief Packets 128 To 255 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_PACKETS_128_TO_255_OCTETS,

    /**
     * @brief Packets 256 To 511 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_PACKETS_256_TO_511_OCTETS,

    /**
     * @brief Packets 512 To 1023 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_PACKETS_512_TO_1023_OCTETS,

    /**
     * @brief Packets 1024 To 1518 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_PACKETS_1024_TO_1518_OCTETS,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Undersize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_UNDERSIZE_PACKETS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_ethernet_pm3_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU ethernet pm3 (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_ethernet_pm3_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_ethernet_pm3_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_ethernet_pm3_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU ethernet pm3
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_pm3_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU FEC pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_fec_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_ONU_ID,

    /**
     * @brief Corrected Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_CORRECTED_BYTES,

    /**
     * @brief Corrected Code Words
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_CORRECTED_CODE_WORDS,

    /**
     * @brief FEC Seconds
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_FEC_SECONDS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Total Code Words
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_TOTAL_CODE_WORDS,

    /**
     * @brief Uncorrectable Code Words
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_UNCORRECTABLE_CODE_WORDS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_fec_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU FEC pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_fec_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_fec_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_fec_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU FEC pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_fec_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU gal ethernet pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_ONU_ID,

    /**
     * @brief Discarded Downstream Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_DISCARDED_DOWNSTREAM_FRAMES,

    /**
     * @brief Discarded Upstream Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_DISCARDED_UPSTREAM_FRAMES,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU gal ethernet pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_gal_ethernet_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_gal_ethernet_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU gal ethernet pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU gem port network CTP pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ONU_ID,

    /**
     * @brief Encryption Key Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ENCRYPTION_KEY_ERRORS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Received Gem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_RECEIVED_GEM_FRAMES,

    /**
     * @brief Received Payload Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_RECEIVED_PAYLOAD_BYTES,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Transmitted Gem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_TRANSMITTED_GEM_FRAMES,

    /**
     * @brief Transmitted Payload Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_TRANSMITTED_PAYLOAD_BYTES,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU gem port network CTP pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU gem port network CTP pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU IP host Performance Monitoring History data (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_ONU_ID,

    /**
     * @brief DNS Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_DNS_ERRORS,

    /**
     * @brief DHCP Timeouts
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_DHCP_TIMEOUTS,

    /**
     * @brief ICMP Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_ICMP_ERRORS,

    /**
     * @brief Internal Error
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_INTERNAL_ERROR,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_INTERVAL_END_TIME,

    /**
     * @brief IP Address Conflict
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_IP_ADDRESS_CONFLICT,

    /**
     * @brief Out Of Memory
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_OUT_OF_MEMORY,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU IP host Performance Monitoring History data (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU IP host Performance Monitoring History data
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU MAC bridge port pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_ONU_ID,

    /**
     * @brief Delay Exceeded Discard Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_DELAY_EXCEEDED_DISCARD_COUNTER,

    /**
     * @brief Forwarded Frame Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_FORWARDED_FRAME_COUNTER,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief MTU Exceeded Discard Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_MTU_EXCEEDED_DISCARD_COUNTER,

    /**
     * @brief Received And Discarded Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_RECEIVED_AND_DISCARDED_COUNTER,

    /**
     * @brief Received Frame Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_RECEIVED_FRAME_COUNTER,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU MAC bridge port pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU MAC bridge port pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU operational pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_operational_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_ONU_ID,

    /**
     * @brief CPU Percent Utilization
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_CPU_PERCENT_UTILIZATION,

    /**
     * @brief Errors In Operations
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_ERRORS_IN_OPERATIONS,

    /**
     * @brief Flash Size Available
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_FLASH_SIZE_AVAILABLE,

    /**
     * @brief Flash Utilization
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_FLASH_UTILIZATION,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Ram Size Available
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_RAM_SIZE_AVAILABLE,

    /**
     * @brief Ram Utilization
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_RAM_UTILIZATION,

    /**
     * @brief Software Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_SOFTWARE_ERRORS,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Temperature Sensor Description
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_TEMPERATURE_SENSOR_DESCRIPTION,

    /**
     * @brief Temperature Sensor Value
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_TEMPERATURE_SENSOR_VALUE,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_operational_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU operational pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_operational_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_operational_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_operational_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU operational pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_operational_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU PON (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_pon_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_PON_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_PON_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_PON_ATTR_START,

    /**
     * @brief Rx Optical Level
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_PON_ATTR_RX_OPTICAL_LEVEL,

    /**
     * @brief Tx Optical Level
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_PON_ATTR_TX_OPTICAL_LEVEL,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_PON_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_PON_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_PON_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_pon_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU PON (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_pon_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_pon_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_pon_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU PON
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_pon_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU rs232 rs485 Performance Monitoring History data (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_ONU_ID,

    /**
     * @brief Incoming Bytes From Chip
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_INCOMING_BYTES_FROM_CHIP,

    /**
     * @brief Incoming Bytes From PON
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_INCOMING_BYTES_FROM_PON,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Outgoing Bytes From Chip
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_OUTGOING_BYTES_FROM_CHIP,

    /**
     * @brief Outgoing Bytes From PON
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_OUTGOING_BYTES_FROM_PON,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU rs232 rs485 Performance Monitoring History data (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU rs232 rs485 Performance Monitoring History data
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU TCP UDP Performance Monitoring History data (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_ONU_ID,

    /**
     * @brief Accept Failed
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_ACCEPT_FAILED,

    /**
     * @brief Bind Failed
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_BIND_FAILED,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Listen Failed
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_LISTEN_FAILED,

    /**
     * @brief Select Failed
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_SELECT_FAILED,

    /**
     * @brief Socket Failed
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_SOCKET_FAILED,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU TCP UDP Performance Monitoring History data (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU TCP UDP Performance Monitoring History data
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU XG PON downstream Management pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ONU_ID,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Ploam Mic Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_PLOAM_MIC_ERROR_COUNT,

    /**
     * @brief Downstream Ploam Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_DOWNSTREAM_PLOAM_MESSAGE_COUNT,

    /**
     * @brief Profile Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_PROFILE_MESSAGES_RECEIVED,

    /**
     * @brief Ranging Time Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_RANGING_TIME_MESSAGES_RECEIVED,

    /**
     * @brief Deactivate ONU ID Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_DEACTIVATE_ONU_ID_MESSAGES_RECEIVED,

    /**
     * @brief Disable Serial Number Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_DISABLE_SERIAL_NUMBER_MESSAGES_RECEIVED,

    /**
     * @brief Request Registration Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_REQUEST_REGISTRATION_MESSAGES_RECEIVED,

    /**
     * @brief Assign Allocation ID Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ASSIGN_ALLOC_ID_MESSAGES_RECEIVED,

    /**
     * @brief Key Control Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_KEY_CONTROL_MESSAGES_RECEIVED,

    /**
     * @brief Sleep Allow Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_SLEEP_ALLOW_MESSAGES_RECEIVED,

    /**
     * @brief Baseline OMCI Messages Received Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_BASELINE_OMCI_MESSAGES_RECEIVED_COUNT,

    /**
     * @brief Extended OMCI Messages Received Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_EXTENDED_OMCI_MESSAGES_RECEIVED_COUNT,

    /**
     * @brief Assign ONU ID OMCI Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ASSIGN_ONU_ID_OMCI_MESSAGES_RECEIVED,

    /**
     * @brief OMCI Mic Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_OMCI_MIC_ERROR_COUNT,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU XG PON downstream Management pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU XG PON downstream Management pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics accumulating ONU XG PON upstream Management pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ONU_ID,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Upstream Ploam Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_UPSTREAM_PLOAM_MESSAGE_COUNT,

    /**
     * @brief Serial Number Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_SERIAL_NUMBER_MESSAGE_COUNT,

    /**
     * @brief Registration Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_REGISTRATION_MESSAGE_COUNT,

    /**
     * @brief Key Report Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_KEY_REPORT_MESSAGE_COUNT,

    /**
     * @brief Acknowledge Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ACKNOWLEDGE_MESSAGE_COUNT,

    /**
     * @brief Sleep Request Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_SLEEP_REQUEST_MESSAGE_COUNT,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics accumulating ONU XG PON upstream Management pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics accumulating ONU XG PON upstream Management pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics streaming ONU enhanced TC pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_ONU_ID,

    /**
     * @brief Lods Event Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_LODS_EVENT_COUNT,

    /**
     * @brief Lods Event Restored Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_LODS_EVENT_RESTORED_COUNT,

    /**
     * @brief Fragment Xgem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_FRAGMENT_XGEM_FRAMES,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief ONU Reactivation By Lods Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_ONU_REACTIVATION_BY_LODS_EVENTS,

    /**
     * @brief Psbd HEC Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_PSBD_HEC_ERROR_COUNT,

    /**
     * @brief Received Bytes In Nonidle Xgem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_RECEIVED_BYTES_IN_NONIDLE_XGEM_FRAMES,

    /**
     * @brief Transmitted Bytes In Nonidle Xgem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_TRANSMITTED_BYTES_IN_NONIDLE_XGEM_FRAMES,

    /**
     * @brief Transmitted Xgem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_TRANSMITTED_XGEM_FRAMES,

    /**
     * @brief Unknown Profile Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_UNKNOWN_PROFILE_COUNT,

    /**
     * @brief Xgem HEC Lost Words Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_XGEM_HEC_LOST_WORDS_COUNT,

    /**
     * @brief Xgem Key Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_XGEM_KEY_ERRORS,

    /**
     * @brief Xgem HEC Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_XGEM_HEC_ERROR_COUNT,

    /**
     * @brief Xgtc HEC Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_XGTC_HEC_ERROR_COUNT,

    /**
     * @brief Threshold Data 64 Bit ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_THRESHOLD_DATA_64_BIT_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics streaming ONU enhanced TC pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_streaming_onu_enhanced_tc_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_streaming_onu_enhanced_tc_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics streaming ONU enhanced TC pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics streaming ONU ethernet frame pm downstream (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_ONU_ID,

    /**
     * @brief Broadcast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_BROADCAST_PACKETS,

    /**
     * @brief CRC Errored Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_CRC_ERRORED_PACKETS,

    /**
     * @brief Drop Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_DROP_EVENTS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Multicast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_MULTICAST_PACKETS,

    /**
     * @brief Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_OCTETS,

    /**
     * @brief Oversize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_OVERSIZE_PACKETS,

    /**
     * @brief Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS,

    /**
     * @brief Packets 1024 To 1518 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_1024_TO_1518_OCTETS,

    /**
     * @brief Packets 128 To 255 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_128_TO_255_OCTETS,

    /**
     * @brief Packets 256 To 511 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_256_TO_511_OCTETS,

    /**
     * @brief Packets 512 To 1023 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_512_TO_1023_OCTETS,

    /**
     * @brief Packets 64 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_64_OCTETS,

    /**
     * @brief Packets 65 To 127 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_PACKETS_65_TO_127_OCTETS,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Undersize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_UNDERSIZE_PACKETS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attr_t;

/**
 * @brief Get attribute for PON ONU statistics streaming ONU ethernet frame pm downstream (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics streaming ONU ethernet frame pm downstream
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics streaming ONU ethernet frame pm upstream (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_ONU_ID,

    /**
     * @brief Broadcast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_BROADCAST_PACKETS,

    /**
     * @brief CRC Errored Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_CRC_ERRORED_PACKETS,

    /**
     * @brief Drop Events
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_DROP_EVENTS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Multicast Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_MULTICAST_PACKETS,

    /**
     * @brief Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_OCTETS,

    /**
     * @brief Oversize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_OVERSIZE_PACKETS,

    /**
     * @brief Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS,

    /**
     * @brief Packets 1024 To 1518 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_1024_TO_1518_OCTETS,

    /**
     * @brief Packets 128 To 255 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_128_TO_255_OCTETS,

    /**
     * @brief Packets 256 To 511 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_256_TO_511_OCTETS,

    /**
     * @brief Packets 512 To 1023 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_512_TO_1023_OCTETS,

    /**
     * @brief Packets 64 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_64_OCTETS,

    /**
     * @brief Packets 65 To 127 Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_PACKETS_65_TO_127_OCTETS,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Undersize Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_UNDERSIZE_PACKETS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attr_t;

/**
 * @brief Get attribute for PON ONU statistics streaming ONU ethernet frame pm upstream (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics streaming ONU ethernet frame pm upstream
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics streaming ONU ethernet pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_streaming_onu_ethernet_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_ONU_ID,

    /**
     * @brief Alignment Error Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_ALIGNMENT_ERROR_COUNTER,

    /**
     * @brief Buffer Overflows On Receive
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_BUFFER_OVERFLOWS_ON_RECEIVE,

    /**
     * @brief Buffer Overflows On Transmit
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_BUFFER_OVERFLOWS_ON_TRANSMIT,

    /**
     * @brief Carrier Sense Error Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_CARRIER_SENSE_ERROR_COUNTER,

    /**
     * @brief Deferred Transmission Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_DEFERRED_TRANSMISSION_COUNTER,

    /**
     * @brief Excessive Collision Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_EXCESSIVE_COLLISION_COUNTER,

    /**
     * @brief FCS Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_FCS_ERRORS,

    /**
     * @brief Frames Too Long
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_FRAMES_TOO_LONG,

    /**
     * @brief Internal MAC Receive Error Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_INTERNAL_MAC_RECEIVE_ERROR_COUNTER,

    /**
     * @brief Internal MAC Transmit Error Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_INTERNAL_MAC_TRANSMIT_ERROR_COUNTER,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Late Collision Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_LATE_COLLISION_COUNTER,

    /**
     * @brief Multiple Collisions Frame Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_MULTIPLE_COLLISIONS_FRAME_COUNTER,

    /**
     * @brief Single Collision Frame Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_SINGLE_COLLISION_FRAME_COUNTER,

    /**
     * @brief SQE Counter
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_SQE_COUNTER,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_streaming_onu_ethernet_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics streaming ONU ethernet pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_streaming_onu_ethernet_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_streaming_onu_ethernet_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_streaming_onu_ethernet_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics streaming ONU ethernet pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_streaming_onu_ethernet_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics streaming ONU FEC pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_streaming_onu_fec_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_ONU_ID,

    /**
     * @brief Corrected Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_CORRECTED_BYTES,

    /**
     * @brief Corrected Code Words
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_CORRECTED_CODE_WORDS,

    /**
     * @brief FEC Seconds
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_FEC_SECONDS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Total Code Words
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_TOTAL_CODE_WORDS,

    /**
     * @brief Uncorrectable Code Words
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_UNCORRECTABLE_CODE_WORDS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_streaming_onu_fec_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics streaming ONU FEC pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_streaming_onu_fec_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_streaming_onu_fec_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_streaming_onu_fec_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics streaming ONU FEC pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_streaming_onu_fec_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics streaming ONU gal ethernet pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_ONU_ID,

    /**
     * @brief Discarded Downstream Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_DISCARDED_DOWNSTREAM_FRAMES,

    /**
     * @brief Discarded Upstream Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_DISCARDED_UPSTREAM_FRAMES,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics streaming ONU gal ethernet pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_streaming_onu_gal_ethernet_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_streaming_onu_gal_ethernet_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics streaming ONU gal ethernet pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics streaming ONU gem port network CTP pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ONU_ID,

    /**
     * @brief Encryption Key Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ENCRYPTION_KEY_ERRORS,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Received Gem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_RECEIVED_GEM_FRAMES,

    /**
     * @brief Received Payload Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_RECEIVED_PAYLOAD_BYTES,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Transmitted Gem Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_TRANSMITTED_GEM_FRAMES,

    /**
     * @brief Transmitted Payload Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_TRANSMITTED_PAYLOAD_BYTES,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics streaming ONU gem port network CTP pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics streaming ONU gem port network CTP pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics streaming ONU rs232 rs485 Performance Monitoring History data (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_ONU_ID,

    /**
     * @brief Incoming Bytes From Chip
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_INCOMING_BYTES_FROM_CHIP,

    /**
     * @brief Incoming Bytes From PON
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_INCOMING_BYTES_FROM_PON,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Outgoing Bytes From Chip
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_OUTGOING_BYTES_FROM_CHIP,

    /**
     * @brief Outgoing Bytes From PON
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_OUTGOING_BYTES_FROM_PON,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attr_t;

/**
 * @brief Get attribute for PON ONU statistics streaming ONU rs232 rs485 Performance Monitoring History data (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics streaming ONU rs232 rs485 Performance Monitoring History data
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics streaming ONU XG PON downstream Management pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ONU_ID,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Ploam Mic Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_PLOAM_MIC_ERROR_COUNT,

    /**
     * @brief Downstream Ploam Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_DOWNSTREAM_PLOAM_MESSAGE_COUNT,

    /**
     * @brief Profile Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_PROFILE_MESSAGES_RECEIVED,

    /**
     * @brief Ranging Time Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_RANGING_TIME_MESSAGES_RECEIVED,

    /**
     * @brief Deactivate ONU ID Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_DEACTIVATE_ONU_ID_MESSAGES_RECEIVED,

    /**
     * @brief Disable Serial Number Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_DISABLE_SERIAL_NUMBER_MESSAGES_RECEIVED,

    /**
     * @brief Request Registration Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_REQUEST_REGISTRATION_MESSAGES_RECEIVED,

    /**
     * @brief Assign Allocation ID Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ASSIGN_ALLOC_ID_MESSAGES_RECEIVED,

    /**
     * @brief Key Control Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_KEY_CONTROL_MESSAGES_RECEIVED,

    /**
     * @brief Sleep Allow Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_SLEEP_ALLOW_MESSAGES_RECEIVED,

    /**
     * @brief Baseline OMCI Messages Received Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_BASELINE_OMCI_MESSAGES_RECEIVED_COUNT,

    /**
     * @brief Extended OMCI Messages Received Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_EXTENDED_OMCI_MESSAGES_RECEIVED_COUNT,

    /**
     * @brief Assign ONU ID OMCI Messages Received
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ASSIGN_ONU_ID_OMCI_MESSAGES_RECEIVED,

    /**
     * @brief OMCI Mic Error Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_OMCI_MIC_ERROR_COUNT,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics streaming ONU XG PON downstream Management pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics streaming ONU XG PON downstream Management pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON ONU statistics streaming ONU XG PON upstream Management pm (COUNTERS_DB)
 */
typedef enum _sai_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ONU_ID,

    /**
     * @brief Interval End Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_INTERVAL_END_TIME,

    /**
     * @brief Threshold Data Half ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_THRESHOLD_DATA_HALF_ID,

    /**
     * @brief Upstream Ploam Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_UPSTREAM_PLOAM_MESSAGE_COUNT,

    /**
     * @brief Serial Number Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_SERIAL_NUMBER_MESSAGE_COUNT,

    /**
     * @brief Registration Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_REGISTRATION_MESSAGE_COUNT,

    /**
     * @brief Key Report Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_KEY_REPORT_MESSAGE_COUNT,

    /**
     * @brief Acknowledge Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ACKNOWLEDGE_MESSAGE_COUNT,

    /**
     * @brief Sleep Request Message Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_SLEEP_REQUEST_MESSAGE_COUNT,

    /**
     * @brief End of attributes
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_END,

    /** Custom range base value */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_CUSTOM_RANGE_END,

} sai_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attr_t;

/**
 * @brief Get attribute for PON ONU statistics streaming ONU XG PON upstream Management pm (COUNTERS_DB, read-only)
 *
 * @param[in] pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attribute_fn)(
        _In_ sai_object_id_t pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON ONU statistics streaming ONU XG PON upstream Management pm
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attribute on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

typedef struct _sai_pon_onu_api_t
{
    sai_create_pon_onu_fn                                                                          create_pon_onu;
    sai_remove_pon_onu_fn                                                                          remove_pon_onu;
    sai_set_pon_onu_attribute_fn                                                                   set_pon_onu_attribute;
    sai_get_pon_onu_attribute_fn                                                                   get_pon_onu_attribute;
    sai_create_pon_onu_olt_service_fn                                                              create_pon_onu_olt_service;
    sai_remove_pon_onu_olt_service_fn                                                              remove_pon_onu_olt_service;
    sai_set_pon_onu_olt_service_attribute_fn                                                       set_pon_onu_olt_service_attribute;
    sai_get_pon_onu_olt_service_attribute_fn                                                       get_pon_onu_olt_service_attribute;
    sai_create_pon_onu_olt_service_network_fn                                                      create_pon_onu_olt_service_network;
    sai_remove_pon_onu_olt_service_network_fn                                                      remove_pon_onu_olt_service_network;
    sai_set_pon_onu_olt_service_network_attribute_fn                                               set_pon_onu_olt_service_network_attribute;
    sai_get_pon_onu_olt_service_network_attribute_fn                                               get_pon_onu_olt_service_network_attribute;
    sai_create_pon_onu_service_config_value_fn                                                     create_pon_onu_service_config_value;
    sai_remove_pon_onu_service_config_value_fn                                                     remove_pon_onu_service_config_value;
    sai_set_pon_onu_service_config_value_attribute_fn                                              set_pon_onu_service_config_value_attribute;
    sai_get_pon_onu_service_config_value_attribute_fn                                              get_pon_onu_service_config_value_attribute;
    sai_create_pon_onu_uni_fn                                                                      create_pon_onu_uni;
    sai_remove_pon_onu_uni_fn                                                                      remove_pon_onu_uni;
    sai_set_pon_onu_uni_attribute_fn                                                               set_pon_onu_uni_attribute;
    sai_get_pon_onu_uni_attribute_fn                                                               get_pon_onu_uni_attribute;
    sai_create_pon_onu_fw_bank_file_fn                                                             create_pon_onu_fw_bank_file;
    sai_remove_pon_onu_fw_bank_file_fn                                                             remove_pon_onu_fw_bank_file;
    sai_set_pon_onu_fw_bank_file_attribute_fn                                                      set_pon_onu_fw_bank_file_attribute;
    sai_get_pon_onu_fw_bank_file_attribute_fn                                                      get_pon_onu_fw_bank_file_attribute;
    sai_get_pon_onu_olt_service_downstream_qos_map_state_attribute_fn                              get_pon_onu_olt_service_downstream_qos_map_state_attribute;
    sai_get_pon_onu_olt_service_downstream_qos_map_map_state_attribute_fn                          get_pon_onu_olt_service_downstream_qos_map_map_state_attribute;
    sai_get_pon_onu_uni_learned_addresses_state_attribute_fn                                       get_pon_onu_uni_learned_addresses_state_attribute;
    sai_get_pon_onu_state_attribute_fn                                                             get_pon_onu_state_attribute;
    sai_get_pon_onu_fw_bank_version_state_attribute_fn                                             get_pon_onu_fw_bank_version_state_attribute;
    sai_get_pon_onu_olt_service_state_attribute_fn                                                 get_pon_onu_olt_service_state_attribute;
    sai_get_pon_onu_olt_service_network_state_attribute_fn                                         get_pon_onu_olt_service_network_state_attribute;
    sai_get_pon_onu_uni_state_attribute_fn                                                         get_pon_onu_uni_state_attribute;
    sai_get_pon_onu_statistics_binned_attribute_fn                                                 get_pon_onu_statistics_binned_attribute;
    sai_get_pon_onu_statistics_binned_olt_pon_attribute_fn                                         get_pon_onu_statistics_binned_olt_pon_attribute;
    sai_get_pon_onu_statistics_binned_olt_pon_omcc_attribute_fn                                    get_pon_onu_statistics_binned_olt_pon_omcc_attribute;
    sai_get_pon_onu_statistics_binned_olt_pon_service_attribute_fn                                 get_pon_onu_statistics_binned_olt_pon_service_attribute;
    sai_get_pon_onu_statistics_binned_onu_enhanced_tc_pm_attribute_fn                              get_pon_onu_statistics_binned_onu_enhanced_tc_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attribute_fn                  get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attribute_fn            get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attribute_fn                get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attribute_fn                  get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_pm_attribute_fn                                 get_pon_onu_statistics_binned_onu_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_pm3_attribute_fn                                get_pon_onu_statistics_binned_onu_ethernet_pm3_attribute;
    sai_get_pon_onu_statistics_binned_onu_fec_pm_attribute_fn                                      get_pon_onu_statistics_binned_onu_fec_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_gal_ethernet_pm_attribute_fn                             get_pon_onu_statistics_binned_onu_gal_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attribute_fn                     get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attribute_fn                  get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attribute_fn                          get_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_operational_pm_attribute_fn                              get_pon_onu_statistics_binned_onu_operational_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_pon_attribute_fn                                         get_pon_onu_statistics_binned_onu_pon_attribute;
    sai_get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attribute_fn              get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attribute_fn                  get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attribute_fn                   get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attribute_fn                     get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_olt_pon_attribute_fn                                   get_pon_onu_statistics_accumulating_olt_pon_attribute;
    sai_get_pon_onu_statistics_accumulating_olt_pon_omcc_attribute_fn                              get_pon_onu_statistics_accumulating_olt_pon_omcc_attribute;
    sai_get_pon_onu_statistics_accumulating_olt_pon_service_attribute_fn                           get_pon_onu_statistics_accumulating_olt_pon_service_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attribute_fn                        get_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attribute_fn            get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attribute_fn      get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attribute_fn          get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attribute_fn            get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_pm_attribute_fn                           get_pon_onu_statistics_accumulating_onu_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_pm3_attribute_fn                          get_pon_onu_statistics_accumulating_onu_ethernet_pm3_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_fec_pm_attribute_fn                                get_pon_onu_statistics_accumulating_onu_fec_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attribute_fn                       get_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attribute_fn               get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attribute_fn            get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attribute_fn                    get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_operational_pm_attribute_fn                        get_pon_onu_statistics_accumulating_onu_operational_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_pon_attribute_fn                                   get_pon_onu_statistics_accumulating_onu_pon_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attribute_fn        get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attribute_fn            get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attribute_fn             get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attribute_fn               get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attribute_fn                           get_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attribute_fn             get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attribute;
    sai_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attribute_fn               get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attribute;
    sai_get_pon_onu_statistics_streaming_onu_ethernet_pm_attribute_fn                              get_pon_onu_statistics_streaming_onu_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_fec_pm_attribute_fn                                   get_pon_onu_statistics_streaming_onu_fec_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attribute_fn                          get_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attribute_fn                  get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attribute_fn           get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attribute_fn                get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attribute_fn                  get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attribute;
    sai_bulk_get_pon_onu_olt_service_downstream_qos_map_state_attribute_fn                         get_pon_onu_olt_service_downstream_qos_map_states_attribute;
    sai_bulk_get_pon_onu_olt_service_downstream_qos_map_map_state_attribute_fn                     get_pon_onu_olt_service_downstream_qos_map_map_states_attribute;
    sai_bulk_get_pon_onu_uni_learned_addresses_state_attribute_fn                                  get_pon_onu_uni_learned_addresses_states_attribute;
    sai_bulk_get_pon_onu_state_attribute_fn                                                        get_pon_onu_states_attribute;
    sai_bulk_get_pon_onu_fw_bank_version_state_attribute_fn                                        get_pon_onu_fw_bank_version_states_attribute;
    sai_bulk_get_pon_onu_olt_service_state_attribute_fn                                            get_pon_onu_olt_service_states_attribute;
    sai_bulk_get_pon_onu_olt_service_network_state_attribute_fn                                    get_pon_onu_olt_service_network_states_attribute;
    sai_bulk_get_pon_onu_uni_state_attribute_fn                                                    get_pon_onu_uni_states_attribute;
    sai_bulk_get_pon_onu_statistics_binned_attribute_fn                                            get_pon_onu_statistics_binneds_attribute;
    sai_bulk_get_pon_onu_statistics_binned_olt_pon_attribute_fn                                    get_pon_onu_statistics_binned_olt_pons_attribute;
    sai_bulk_get_pon_onu_statistics_binned_olt_pon_omcc_attribute_fn                               get_pon_onu_statistics_binned_olt_pon_omccs_attribute;
    sai_bulk_get_pon_onu_statistics_binned_olt_pon_service_attribute_fn                            get_pon_onu_statistics_binned_olt_pon_services_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_enhanced_tc_pm_attribute_fn                         get_pon_onu_statistics_binned_onu_enhanced_tc_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attribute_fn             get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attribute_fn       get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bits_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attribute_fn           get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstreams_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attribute_fn             get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstreams_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_pm_attribute_fn                            get_pon_onu_statistics_binned_onu_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_pm3_attribute_fn                           get_pon_onu_statistics_binned_onu_ethernet_pm3s_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_fec_pm_attribute_fn                                 get_pon_onu_statistics_binned_onu_fec_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_gal_ethernet_pm_attribute_fn                        get_pon_onu_statistics_binned_onu_gal_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attribute_fn                get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attribute_fn             get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attribute_fn                     get_pon_onu_statistics_binned_onu_mac_bridge_port_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_operational_pm_attribute_fn                         get_pon_onu_statistics_binned_onu_operational_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_pon_attribute_fn                                    get_pon_onu_statistics_binned_onu_pons_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attribute_fn         get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attribute_fn             get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attribute_fn              get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attribute_fn                get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_olt_pon_attribute_fn                              get_pon_onu_statistics_accumulating_olt_pons_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_olt_pon_omcc_attribute_fn                         get_pon_onu_statistics_accumulating_olt_pon_omccs_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_olt_pon_service_attribute_fn                      get_pon_onu_statistics_accumulating_olt_pon_services_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attribute_fn                   get_pon_onu_statistics_accumulating_onu_enhanced_tc_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attribute_fn       get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attribute_fn get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bits_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attribute_fn     get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstreams_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attribute_fn       get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstreams_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_pm_attribute_fn                      get_pon_onu_statistics_accumulating_onu_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_pm3_attribute_fn                     get_pon_onu_statistics_accumulating_onu_ethernet_pm3s_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_fec_pm_attribute_fn                           get_pon_onu_statistics_accumulating_onu_fec_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attribute_fn                  get_pon_onu_statistics_accumulating_onu_gal_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attribute_fn          get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attribute_fn       get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attribute_fn               get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_operational_pm_attribute_fn                   get_pon_onu_statistics_accumulating_onu_operational_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_pon_attribute_fn                              get_pon_onu_statistics_accumulating_onu_pons_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attribute_fn   get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attribute_fn       get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attribute_fn        get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attribute_fn          get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attribute_fn                      get_pon_onu_statistics_streaming_onu_enhanced_tc_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attribute_fn        get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstreams_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attribute_fn          get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstreams_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_ethernet_pm_attribute_fn                         get_pon_onu_statistics_streaming_onu_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_fec_pm_attribute_fn                              get_pon_onu_statistics_streaming_onu_fec_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attribute_fn                     get_pon_onu_statistics_streaming_onu_gal_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attribute_fn             get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attribute_fn      get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attribute_fn           get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attribute_fn             get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pms_attribute;
} sai_pon_onu_api_t;

/**
 * @}
 */

#endif /* __SAIEXPERIMENTALPONONU_H_ */
