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
 * @file    saifw.h
 *
 * @brief   This module defines SAI Firmware spec
 */

#if !defined (__SAIFW_H_)
#define __SAIFW_H_

#include <saitypes.h>

/**
 * @defgroup SAIFW SAI - Firmware specific API definitions
 *
 * @{
 */

/**
 * @brief SAI firmware operational state
 */
typedef enum _sai_fw_op_state_t
{
    /** Firmware operational state is firmware is loaded but not started */
    SAI_FW_OP_STATE_LOADED,

    /** Firmware operational state is firmware is not loaded */
    SAI_FW_OP_STATE_NOT_LOADED,

    /** Firmware operational state is running */
    SAI_FW_OP_STATE_RUNNING,

    /** Firmware operational state is stop */
    SAI_FW_OP_STATE_STOPPED,

    /** Firmware operational state is in error */
    SAI_FW_OP_STATE_ERROR,
} sai_fw_op_state_t;

/**
 * @brief Defines maximum length of firmware and log file name
 */
#define SAI_FW_NAME_SIZE 64

/**
 * @brief Firmware Attributes
 */
typedef enum _sai_fw_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_FW_ATTR_START,

    /**
     * @brief Firmware Log File and Path Name [char[SAI_FW_NAME_SIZE]]
     *
     * The maximum number of characters for the name is SAI_FW_NAME_SIZE - 1 since
     * it needs the terminating null byte ('\0') at the end.
     *
     * @type char
     * @flags CREATE_AND_SET
     * @default ""
     */
    SAI_FW_ATTR_LOG_FILE_AND_PATH_NAME = SAI_FW_ATTR_START,

    /**
     * @brief Firmware File and Path Name [char[SAI_FW_NAME_SIZE]]
     *
     * The maximum number of characters for the name is SAI_FW_NAME_SIZE - 1 since
     * it needs the terminating null byte ('\0') at the end.
     *
     * @type char
     * @flags CREATE_AND_SET
     * @default ""
     */
    SAI_FW_ATTR_FW_FILE_AND_PATH_NAME,

    /**
     * @brief Firmware to be loaded on the core
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FW_ATTR_CORE_ID,

    /**
     * @brief Firmware admin state
     *
     * @type sai_fw_admin_state_t
     * @flags CREATE_AND_SET
     * @default SAI_FW_ADMIN_STATE_AUTO
     */
    SAI_FW_ATTR_FW_ADMIN_STATE,

    /**
     * @brief Firmware operational state
     *
     * @type sai_fw_op_state_t
     * @flags READ_ONLY
     */
    SAI_FW_ATTR_FW_OP_STATE,

    /**
     * @brief Firmware major version
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_FW_ATTR_FW_MAJOR_VERSION,

    /**
     * @brief Firmware minor version
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_FW_ATTR_FW_MINOR_VERSION,

    /**
     * @brief End of Performance Monitoring attributes
     */
    SAI_FW_ATTR_END,

    /** Custom range base value */
    SAI_FW_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_FW_ATTR_CUSTOM_RANGE_END

} sai_fw_attr_t;

/**
 * @brief Create firmware object
 *
 * @param[out] fw_id Firmware id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_fw_fn)(
        _Out_ sai_object_id_t *fw_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove firmware object
 *
 * @param[in] fw_id Firmware id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_fw_fn)(
        _In_ sai_object_id_t fw_id);

/**
 * @brief Set firmware attribute
 *
 * @param[in] fw_id Firmware id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_fw_attribute_fn)(
        _In_ sai_object_id_t fw_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Firmware attribute
 *
 * @param[in] fw_id Firmware ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_fw_attribute_fn)(
        _In_ sai_object_id_t fw_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Firmware related API methods table retrieved with sai_api_query()
 */
typedef struct _sai_fw_api_t
{
    sai_create_fw_fn                   create_fw;
    sai_remove_fw_fn                   remove_fw;
    sai_set_fw_attribute_fn            set_fw_attribute;
    sai_get_fw_attribute_fn            get_fw_attribute;
} sai_fw_api_t;

/**
 * @}
 */
#endif /** __SAIFW_H_ */
