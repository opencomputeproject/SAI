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
 * @file    saiexperimentalponoltplug.h
 *
 * @brief   This module defines the experimental PON OLT plug API.
 */

#if !defined (__SAIEXPERIMENTALPONOLTPLUG_H_)
#define __SAIEXPERIMENTALPONOLTPLUG_H_

#include <saitypes.h>

#include <saiexperimentalponcommon.h>

/**
 * @defgroup SAIEXPERIMENTALPONOLTPLUG SAI - Experimental: PON specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for PON OLT plug FW bank file
 */
typedef enum _sai_pon_olt_plug_fw_bank_file_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_OLT_NAME = SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_START,

    /**
     * @brief Bank ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_BANK_ID,

    /**
     * @brief File
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_FILE,

    /**
     * @brief Version
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_VERSION,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_plug_fw_bank_file_attr_t;

/**
 * @brief Create PON OLT plug FW bank file
 *
 * @param[out] pon_olt_plug_fw_bank_file_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_olt_plug_fw_bank_file_fn)(
        _Out_ sai_object_id_t *pon_olt_plug_fw_bank_file_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON OLT plug FW bank file
 *
 * @param[in] pon_olt_plug_fw_bank_file_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_olt_plug_fw_bank_file_fn)(
        _In_ sai_object_id_t pon_olt_plug_fw_bank_file_id);

/**
 * @brief Set attribute for PON OLT plug FW bank file
 *
 * @param[in] pon_olt_plug_fw_bank_file_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_olt_plug_fw_bank_file_attribute_fn)(
        _In_ sai_object_id_t pon_olt_plug_fw_bank_file_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON OLT plug FW bank file
 *
 * @param[in] pon_olt_plug_fw_bank_file_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_plug_fw_bank_file_attribute_fn)(
        _In_ sai_object_id_t pon_olt_plug_fw_bank_file_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON OLT plug
 */
typedef enum _sai_pon_olt_plug_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_PLUG_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PLUG_ATTR_OLT_NAME = SAI_PON_OLT_PLUG_ATTR_START,

    /**
     * @brief Debug Log Level
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PLUG_ATTR_DEBUG_LOG_LEVEL,

    /**
     * @brief FW Bank Pointer
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PLUG_ATTR_FW_BANK_PTR,

    /**
     * @brief NNI Max Frame Size
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PLUG_ATTR_NNI_MAX_FRAME_SIZE,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_PLUG_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_PLUG_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_PLUG_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_plug_attr_t;

/**
 * @brief Create PON OLT plug
 *
 * @param[out] pon_olt_plug_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_olt_plug_fn)(
        _Out_ sai_object_id_t *pon_olt_plug_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON OLT plug
 *
 * @param[in] pon_olt_plug_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_olt_plug_fn)(
        _In_ sai_object_id_t pon_olt_plug_id);

/**
 * @brief Set attribute for PON OLT plug
 *
 * @param[in] pon_olt_plug_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_olt_plug_attribute_fn)(
        _In_ sai_object_id_t pon_olt_plug_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON OLT plug
 *
 * @param[in] pon_olt_plug_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_plug_attribute_fn)(
        _In_ sai_object_id_t pon_olt_plug_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON OLT plug FW upgrade status (STATE_DB)
 */
typedef enum _sai_pon_olt_plug_fw_upgrade_status_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_PLUG_FW_UPGRADE_STATUS_ATTR_START,

    /**
     * @brief Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_PLUG_FW_UPGRADE_STATUS_ATTR_NAME = SAI_PON_OLT_PLUG_FW_UPGRADE_STATUS_ATTR_START,

    /**
     * @brief Bank
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_FW_UPGRADE_STATUS_ATTR_BANK,

    /**
     * @brief Fx Code
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_FW_UPGRADE_STATUS_ATTR_FX_CODE,

    /**
     * @brief File
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_FW_UPGRADE_STATUS_ATTR_FILE,

    /**
     * @brief Status
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_FW_UPGRADE_STATUS_ATTR_STATUS,

    /**
     * @brief Upgrade Duration
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_FW_UPGRADE_STATUS_ATTR_UPGRADE_DURATION,

    /**
     * @brief Upgrade Time
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_FW_UPGRADE_STATUS_ATTR_UPGRADE_TIME,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_PLUG_FW_UPGRADE_STATUS_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_PLUG_FW_UPGRADE_STATUS_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_PLUG_FW_UPGRADE_STATUS_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_plug_fw_upgrade_status_attr_t;

/**
 * @brief Get attribute for PON OLT plug FW upgrade status (STATE_DB, read-only)
 *
 * @param[in] pon_olt_plug_fw_upgrade_status_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_plug_fw_upgrade_status_attribute_fn)(
        _In_ sai_object_id_t pon_olt_plug_fw_upgrade_status_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT plug FW upgrade status
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
typedef sai_status_t (*sai_bulk_get_pon_olt_plug_fw_upgrade_status_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT plug state (STATE_DB)
 */
typedef enum _sai_pon_olt_plug_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_START,

    /**
     * @brief Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_NAME = SAI_PON_OLT_PLUG_STATE_ATTR_START,

    /**
     * @brief FW Bank Pointer
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_FW_BANK_PTR,

    /**
     * @brief FW Version
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_FW_VERSION,

    /**
     * @brief Hardware Version
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_HARDWARE_VERSION,

    /**
     * @brief Manufacturer
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_MANUFACTURER,

    /**
     * @brief Manufacturer Model
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_MANUFACTURER_MODEL,

    /**
     * @brief Manufacturer Serial Number
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_MANUFACTURER_SERIAL_NUMBER,

    /**
     * @brief Model
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_MODEL,

    /**
     * @brief Online Time
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_ONLINE_TIME,

    /**
     * @brief Production Code
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_PRODUCTION_CODE,

    /**
     * @brief Serial Number
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_SERIAL_NUMBER,

    /**
     * @brief Uptime
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_UPTIME,

    /**
     * @brief Switch Chassis ID
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_SWITCH_CHASSIS_ID,

    /**
     * @brief Switch IPV4 Address
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_SWITCH_IPV4_ADDRESS,

    /**
     * @brief Switch IPV6 Address
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_SWITCH_IPV6_ADDRESS,

    /**
     * @brief Switch Port Description
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_SWITCH_PORT_DESCRIPTION,

    /**
     * @brief Switch Port ID
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_SWITCH_PORT_ID,

    /**
     * @brief Switch System Description
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_SWITCH_SYSTEM_DESCRIPTION,

    /**
     * @brief Switch System Name
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_SWITCH_SYSTEM_NAME,

    /**
     * @brief FW Upgrade Bank
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_FW_UPGRADE_BANK,

    /**
     * @brief FW Upgrade Fx Code
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_FW_UPGRADE_FX_CODE,

    /**
     * @brief FW Upgrade File
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_FW_UPGRADE_FILE,

    /**
     * @brief FW Upgrade Status
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_FW_UPGRADE_STATUS,

    /**
     * @brief FW Upgrade Duration
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_FW_UPGRADE_DURATION,

    /**
     * @brief FW Upgrade Time
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_FW_UPGRADE_TIME,

    /**
     * @brief HW Failure
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_HW_FAILURE,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_PLUG_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_PLUG_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_PLUG_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_plug_state_attr_t;

/**
 * @brief Get attribute for PON OLT plug state (STATE_DB, read-only)
 *
 * @param[in] pon_olt_plug_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_plug_state_attribute_fn)(
        _In_ sai_object_id_t pon_olt_plug_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT plug state
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
typedef sai_status_t (*sai_bulk_get_pon_olt_plug_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT plug FW bank version state (STATE_DB)
 */
typedef enum _sai_pon_olt_plug_fw_bank_version_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_PLUG_FW_BANK_VERSION_STATE_ATTR_START,

    /**
     * @brief Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_PLUG_FW_BANK_VERSION_STATE_ATTR_NAME = SAI_PON_OLT_PLUG_FW_BANK_VERSION_STATE_ATTR_START,

    /**
     * @brief Bank ID
     *
     * @type sai_uint8_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_PLUG_FW_BANK_VERSION_STATE_ATTR_BANK_ID,

    /**
     * @brief Version
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_FW_BANK_VERSION_STATE_ATTR_VERSION,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_PLUG_FW_BANK_VERSION_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_PLUG_FW_BANK_VERSION_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_PLUG_FW_BANK_VERSION_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_plug_fw_bank_version_state_attr_t;

/**
 * @brief Get attribute for PON OLT plug FW bank version state (STATE_DB, read-only)
 *
 * @param[in] pon_olt_plug_fw_bank_version_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_plug_fw_bank_version_state_attribute_fn)(
        _In_ sai_object_id_t pon_olt_plug_fw_bank_version_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT plug FW bank version state
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
typedef sai_status_t (*sai_bulk_get_pon_olt_plug_fw_bank_version_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT plug statistics binned env (COUNTERS_DB)
 */
typedef enum _sai_pon_olt_plug_statistics_binned_env_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_ENV_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_ENV_ATTR_OLT_NAME = SAI_PON_OLT_PLUG_STATISTICS_BINNED_ENV_ATTR_START,

    /**
     * @brief OLT Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_ENV_ATTR_OLT_STATS_ID,

    /**
     * @brief Current
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_ENV_ATTR_CURRENT,

    /**
     * @brief Transmit Bias
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_ENV_ATTR_TRANSMIT_BIAS,

    /**
     * @brief Voltage
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_ENV_ATTR_VOLTAGE,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_ENV_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_ENV_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_ENV_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_plug_statistics_binned_env_attr_t;

/**
 * @brief Get attribute for PON OLT plug statistics binned env (COUNTERS_DB, read-only)
 *
 * @param[in] pon_olt_plug_statistics_binned_env_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_plug_statistics_binned_env_attribute_fn)(
        _In_ sai_object_id_t pon_olt_plug_statistics_binned_env_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT plug statistics binned env
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
typedef sai_status_t (*sai_bulk_get_pon_olt_plug_statistics_binned_env_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT plug statistics binned NNI (COUNTERS_DB)
 */
typedef enum _sai_pon_olt_plug_statistics_binned_nni_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_OLT_NAME = SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_START,

    /**
     * @brief OLT Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_OLT_STATS_ID,

    /**
     * @brief Rx Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_BROADCAST_OCTETS,

    /**
     * @brief Rx Crc32 Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_CRC32_DROPS,

    /**
     * @brief Rx Encrypted Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_ENCRYPTED_FRAMES,

    /**
     * @brief Rx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_ENCRYPTED_OCTETS,

    /**
     * @brief Rx Filtered Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_FILTERED_FRAMES,

    /**
     * @brief Rx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_FRAMES_1024_1518,

    /**
     * @brief Rx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_FRAMES_128_255,

    /**
     * @brief Rx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_FRAMES_1519_PLUS,

    /**
     * @brief Rx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_FRAMES_256_511,

    /**
     * @brief Rx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_FRAMES_512_1023,

    /**
     * @brief Rx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_FRAMES_64,

    /**
     * @brief Rx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_FRAMES_65_127,

    /**
     * @brief Rx Frames Broadcast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_FRAMES_BROADCAST,

    /**
     * @brief Rx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_FRAMES_GREEN,

    /**
     * @brief Rx Frames Multicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_FRAMES_MULTICAST,

    /**
     * @brief Rx Frames Unicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_FRAMES_UNICAST,

    /**
     * @brief Rx Multicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_MULTICAST_OCTETS,

    /**
     * @brief Rx OAM
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_OAM,

    /**
     * @brief Rx OAM Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_OAM_OCTETS,

    /**
     * @brief Rx Other Cascading Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_OTHER_CASCADING_BYTES,

    /**
     * @brief Rx Other Cascading Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_OTHER_CASCADING_PACKETS,

    /**
     * @brief Rx Overflow Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_OVERFLOW_DROPS,

    /**
     * @brief Rx Overflow Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_OVERFLOW_OCTETS,

    /**
     * @brief Rx Plain Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_PLAIN_FRAMES,

    /**
     * @brief Rx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_PLAIN_OCTETS,

    /**
     * @brief Rx Too Long Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_TOO_LONG_DROPS,

    /**
     * @brief Rx Too Short Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_TOO_SHORT_DROPS,

    /**
     * @brief Rx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_RX_UNICAST_OCTETS,

    /**
     * @brief Tomi Requests
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TOMI_REQUESTS,

    /**
     * @brief Tomi Resp Time Avg
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TOMI_RESP_TIME_AVG,

    /**
     * @brief Tomi Resp Time Max
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TOMI_RESP_TIME_MAX,

    /**
     * @brief Tomi Resp Time Min
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TOMI_RESP_TIME_MIN,

    /**
     * @brief Tomi Responses
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TOMI_RESPONSES,

    /**
     * @brief Tomi Time To Send Avg
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TOMI_TIME_TO_SEND_AVG,

    /**
     * @brief Tomi Time To Send Max
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TOMI_TIME_TO_SEND_MAX,

    /**
     * @brief Tomi Time To Send Min
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TOMI_TIME_TO_SEND_MIN,

    /**
     * @brief Tomi Timeouts
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TOMI_TIMEOUTS,

    /**
     * @brief Tx Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_BROADCAST_OCTETS,

    /**
     * @brief Tx Cascading Bytes
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_CASCADING_BYTES,

    /**
     * @brief Tx Cascading Packets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_CASCADING_PACKETS,

    /**
     * @brief Tx Encrypted Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_ENCRYPTED_FRAMES,

    /**
     * @brief Tx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_FRAMES_1024_1518,

    /**
     * @brief Tx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_FRAMES_128_255,

    /**
     * @brief Tx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_FRAMES_1519_PLUS,

    /**
     * @brief Tx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_FRAMES_256_511,

    /**
     * @brief Tx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_FRAMES_512_1023,

    /**
     * @brief Tx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_FRAMES_64,

    /**
     * @brief Tx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_FRAMES_65_127,

    /**
     * @brief Tx Frames Broadcast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_FRAMES_BROADCAST,

    /**
     * @brief Tx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_FRAMES_GREEN,

    /**
     * @brief Tx Frames Multicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_FRAMES_MULTICAST,

    /**
     * @brief Tx Frames Unicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_FRAMES_UNICAST,

    /**
     * @brief Tx Multicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_MULTICAST_OCTETS,

    /**
     * @brief Tx Non Control Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_NON_CONTROL_OCTETS,

    /**
     * @brief Tx OAM
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_OAM,

    /**
     * @brief Tx OAM Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_OAM_OCTETS,

    /**
     * @brief Tx Plain Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_PLAIN_FRAMES,

    /**
     * @brief Tx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_TX_UNICAST_OCTETS,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_plug_statistics_binned_nni_attr_t;

/**
 * @brief Get attribute for PON OLT plug statistics binned NNI (COUNTERS_DB, read-only)
 *
 * @param[in] pon_olt_plug_statistics_binned_nni_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_plug_statistics_binned_nni_attribute_fn)(
        _In_ sai_object_id_t pon_olt_plug_statistics_binned_nni_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT plug statistics binned NNI
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
typedef sai_status_t (*sai_bulk_get_pon_olt_plug_statistics_binned_nni_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT plug statistics binned temp (COUNTERS_DB)
 */
typedef enum _sai_pon_olt_plug_statistics_binned_temp_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_TEMP_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_TEMP_ATTR_OLT_NAME = SAI_PON_OLT_PLUG_STATISTICS_BINNED_TEMP_ATTR_START,

    /**
     * @brief OLT Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_TEMP_ATTR_OLT_STATS_ID,

    /**
     * @brief ASIC
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_TEMP_ATTR_ASIC,

    /**
     * @brief Laser
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_TEMP_ATTR_LASER,

    /**
     * @brief Xcvr
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_TEMP_ATTR_XCVR,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_TEMP_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_TEMP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_TEMP_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_plug_statistics_binned_temp_attr_t;

/**
 * @brief Get attribute for PON OLT plug statistics binned temp (COUNTERS_DB, read-only)
 *
 * @param[in] pon_olt_plug_statistics_binned_temp_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_plug_statistics_binned_temp_attribute_fn)(
        _In_ sai_object_id_t pon_olt_plug_statistics_binned_temp_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT plug statistics binned temp
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
typedef sai_status_t (*sai_bulk_get_pon_olt_plug_statistics_binned_temp_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

typedef struct _sai_pon_olt_plug_api_t
{
    sai_create_pon_olt_plug_fw_bank_file_fn                       create_pon_olt_plug_fw_bank_file;
    sai_remove_pon_olt_plug_fw_bank_file_fn                       remove_pon_olt_plug_fw_bank_file;
    sai_set_pon_olt_plug_fw_bank_file_attribute_fn                set_pon_olt_plug_fw_bank_file_attribute;
    sai_get_pon_olt_plug_fw_bank_file_attribute_fn                get_pon_olt_plug_fw_bank_file_attribute;
    sai_create_pon_olt_plug_fn                                    create_pon_olt_plug;
    sai_remove_pon_olt_plug_fn                                    remove_pon_olt_plug;
    sai_set_pon_olt_plug_attribute_fn                             set_pon_olt_plug_attribute;
    sai_get_pon_olt_plug_attribute_fn                             get_pon_olt_plug_attribute;
    sai_get_pon_olt_plug_fw_upgrade_status_attribute_fn           get_pon_olt_plug_fw_upgrade_status_attribute;
    sai_get_pon_olt_plug_state_attribute_fn                       get_pon_olt_plug_state_attribute;
    sai_get_pon_olt_plug_fw_bank_version_state_attribute_fn       get_pon_olt_plug_fw_bank_version_state_attribute;
    sai_get_pon_olt_plug_statistics_binned_env_attribute_fn       get_pon_olt_plug_statistics_binned_env_attribute;
    sai_get_pon_olt_plug_statistics_binned_nni_attribute_fn       get_pon_olt_plug_statistics_binned_nni_attribute;
    sai_get_pon_olt_plug_statistics_binned_temp_attribute_fn      get_pon_olt_plug_statistics_binned_temp_attribute;
    sai_bulk_get_pon_olt_plug_fw_upgrade_status_attribute_fn      get_pon_olt_plug_fw_upgrade_statuss_attribute;
    sai_bulk_get_pon_olt_plug_state_attribute_fn                  get_pon_olt_plug_states_attribute;
    sai_bulk_get_pon_olt_plug_fw_bank_version_state_attribute_fn  get_pon_olt_plug_fw_bank_version_states_attribute;
    sai_bulk_get_pon_olt_plug_statistics_binned_env_attribute_fn  get_pon_olt_plug_statistics_binned_envs_attribute;
    sai_bulk_get_pon_olt_plug_statistics_binned_nni_attribute_fn  get_pon_olt_plug_statistics_binned_nnis_attribute;
    sai_bulk_get_pon_olt_plug_statistics_binned_temp_attribute_fn get_pon_olt_plug_statistics_binned_temps_attribute;
} sai_pon_olt_plug_api_t;

/**
 * @}
 */

#endif /* __SAIEXPERIMENTALPONOLTPLUG_H_ */
