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
 * @file    saiexperimentalponcontroller.h
 *
 * @brief   This module defines the experimental PON controller API.
 */

#if !defined (__SAIEXPERIMENTALPONCONTROLLER_H_)
#define __SAIEXPERIMENTALPONCONTROLLER_H_

#include <saitypes.h>

#include <saiexperimentalponcommon.h>

/**
 * @defgroup SAIEXPERIMENTALPONCONTROLLER SAI - Experimental: PON specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for PON controller
 */
typedef enum _sai_pon_controller_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_CONTROLLER_ATTR_START,

    /**
     * @brief Controller Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_CONTROLLER_NAME = SAI_PON_CONTROLLER_ATTR_START,

    /**
     * @brief Device ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_DEVICE_ID,

    /**
     * @brief Allow Unprovisioned Onus
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_ALLOW_UNPROVISIONED_ONUS,

    /**
     * @brief Create Date
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_CREATE_DATE,

    /**
     * @brief OLT Timeout
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_OLT_TIMEOUT,

    /**
     * @brief Statistics Sample
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_STATISTICS_SAMPLE,

    /**
     * @brief Logging Controller Console
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_CONTROLLER_CONSOLE,

    /**
     * @brief Logging Controller File
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_CONTROLLER_FILE,

    /**
     * @brief Logging Controller Syslog
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_CONTROLLER_SYSLOG,

    /**
     * @brief Logging Controller Database
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_CONTROLLER_DATABASE,

    /**
     * @brief Logging OLT Console
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_OLT_CONSOLE,

    /**
     * @brief Logging OLT File
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_OLT_FILE,

    /**
     * @brief Logging OLT Syslog
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_OLT_SYSLOG,

    /**
     * @brief OLT Management Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_OLT_MANAGEMENT_INTERFACE_NAME,

    /**
     * @brief Unprovisioned Age
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_UNPROVISIONED_AGE,

    /**
     * @brief Refresh State On OLT Change
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_CONTROLLER_ATTR_REFRESH_STATE_ON_OLT_CHANGE,

    /**
     * @brief End of attributes
     */
    SAI_PON_CONTROLLER_ATTR_END,

    /** Custom range base value */
    SAI_PON_CONTROLLER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_CONTROLLER_ATTR_CUSTOM_RANGE_END,

} sai_pon_controller_attr_t;

/**
 * @brief Create PON controller
 *
 * @param[out] pon_controller_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_controller_fn)(
        _Out_ sai_object_id_t *pon_controller_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON controller
 *
 * @param[in] pon_controller_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_controller_fn)(
        _In_ sai_object_id_t pon_controller_id);

/**
 * @brief Set attribute for PON controller
 *
 * @param[in] pon_controller_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_controller_attribute_fn)(
        _In_ sai_object_id_t pon_controller_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON controller
 *
 * @param[in] pon_controller_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_controller_attribute_fn)(
        _In_ sai_object_id_t pon_controller_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON downstream QOS map map
 */
typedef enum _sai_pon_downstream_qos_map_map_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_DOWNSTREAM_QOS_MAP_MAP_ATTR_START,

    /**
     * @brief Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_DOWNSTREAM_QOS_MAP_MAP_ATTR_NAME = SAI_PON_DOWNSTREAM_QOS_MAP_MAP_ATTR_START,

    /**
     * @brief Cos
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_DOWNSTREAM_QOS_MAP_MAP_ATTR_COS,

    /**
     * @brief OLT Service Offset
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_DOWNSTREAM_QOS_MAP_MAP_ATTR_OLT_SERVICE_OFFSET,

    /**
     * @brief End of attributes
     */
    SAI_PON_DOWNSTREAM_QOS_MAP_MAP_ATTR_END,

    /** Custom range base value */
    SAI_PON_DOWNSTREAM_QOS_MAP_MAP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_DOWNSTREAM_QOS_MAP_MAP_ATTR_CUSTOM_RANGE_END,

} sai_pon_downstream_qos_map_map_attr_t;

/**
 * @brief Create PON downstream QOS map map
 *
 * @param[out] pon_downstream_qos_map_map_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_downstream_qos_map_map_fn)(
        _Out_ sai_object_id_t *pon_downstream_qos_map_map_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON downstream QOS map map
 *
 * @param[in] pon_downstream_qos_map_map_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_downstream_qos_map_map_fn)(
        _In_ sai_object_id_t pon_downstream_qos_map_map_id);

/**
 * @brief Set attribute for PON downstream QOS map map
 *
 * @param[in] pon_downstream_qos_map_map_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_downstream_qos_map_map_attribute_fn)(
        _In_ sai_object_id_t pon_downstream_qos_map_map_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON downstream QOS map map
 *
 * @param[in] pon_downstream_qos_map_map_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_downstream_qos_map_map_attribute_fn)(
        _In_ sai_object_id_t pon_downstream_qos_map_map_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON controller system status OLT (STATE_DB)
 */
typedef enum _sai_pon_controller_system_status_olt_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_START,

    /**
     * @brief Controller Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_CONTROLLER_NAME = SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_START,

    /**
     * @brief MAC Address
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_MAC_ADDRESS,

    /**
     * @brief OLT State
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_OLT_STATE,

    /**
     * @brief ONU Active Count
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_ONU_ACTIVE_COUNT,

    /**
     * @brief Switch Chassis ID
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_SWITCH_CHASSIS_ID,

    /**
     * @brief Switch IPV4 Address
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_SWITCH_IPV4_ADDRESS,

    /**
     * @brief Switch IPV6 Address
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_SWITCH_IPV6_ADDRESS,

    /**
     * @brief Switch Port Description
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_SWITCH_PORT_DESCRIPTION,

    /**
     * @brief Switch Port ID
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_SWITCH_PORT_ID,

    /**
     * @brief Switch System Description
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_SWITCH_SYSTEM_DESCRIPTION,

    /**
     * @brief Switch System Name
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_SWITCH_SYSTEM_NAME,

    /**
     * @brief End of attributes
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_END,

    /** Custom range base value */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_CUSTOM_RANGE_END,

} sai_pon_controller_system_status_olt_attr_t;

/**
 * @brief Get attribute for PON controller system status OLT (STATE_DB, read-only)
 *
 * @param[in] pon_controller_system_status_olt_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_controller_system_status_olt_attribute_fn)(
        _In_ sai_object_id_t pon_controller_system_status_olt_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON controller system status OLT
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
typedef sai_status_t (*sai_bulk_get_pon_controller_system_status_olt_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON controller system status OLT ONU (STATE_DB)
 */
typedef enum _sai_pon_controller_system_status_olt_onu_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU_ATTR_START,

    /**
     * @brief Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU_ATTR_NAME = SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU_ATTR_START,

    /**
     * @brief MAC Address
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU_ATTR_MAC_ADDRESS,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief ONU State
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU_ATTR_ONU_STATE,

    /**
     * @brief End of attributes
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU_ATTR_END,

    /** Custom range base value */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU_ATTR_CUSTOM_RANGE_END,

} sai_pon_controller_system_status_olt_onu_attr_t;

/**
 * @brief Get attribute for PON controller system status OLT ONU (STATE_DB, read-only)
 *
 * @param[in] pon_controller_system_status_olt_onu_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_controller_system_status_olt_onu_attribute_fn)(
        _In_ sai_object_id_t pon_controller_system_status_olt_onu_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON controller system status OLT ONU
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
typedef sai_status_t (*sai_bulk_get_pon_controller_system_status_olt_onu_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON controller state (STATE_DB)
 */
typedef enum _sai_pon_controller_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_CONTROLLER_STATE_ATTR_START,

    /**
     * @brief Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_CONTROLLER_STATE_ATTR_NAME = SAI_PON_CONTROLLER_STATE_ATTR_START,

    /**
     * @brief Timestamp
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_STATE_ATTR_TIMESTAMP,

    /**
     * @brief Allow Unprovisioned Onus
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_STATE_ATTR_ALLOW_UNPROVISIONED_ONUS,

    /**
     * @brief Config Read Failed
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_STATE_ATTR_CONFIG_READ_FAILED,

    /**
     * @brief Interface
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_STATE_ATTR_INTERFACE,

    /**
     * @brief OLT Timeout
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_STATE_ATTR_OLT_TIMEOUT,

    /**
     * @brief Statistics Sample
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_STATE_ATTR_STATISTICS_SAMPLE,

    /**
     * @brief Version
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_STATE_ATTR_VERSION,

    /**
     * @brief Unprovisioned Age
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_STATE_ATTR_UNPROVISIONED_AGE,

    /**
     * @brief Refresh State On OLT Change
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_CONTROLLER_STATE_ATTR_REFRESH_STATE_ON_OLT_CHANGE,

    /**
     * @brief End of attributes
     */
    SAI_PON_CONTROLLER_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_CONTROLLER_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_CONTROLLER_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_controller_state_attr_t;

/**
 * @brief Get attribute for PON controller state (STATE_DB, read-only)
 *
 * @param[in] pon_controller_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_controller_state_attribute_fn)(
        _In_ sai_object_id_t pon_controller_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON controller state
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
typedef sai_status_t (*sai_bulk_get_pon_controller_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON controller ONU FW upgrade state (STATE_DB)
 */
typedef enum _sai_pon_controller_onu_fw_upgrade_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_CONTROLLER_ONU_FW_UPGRADE_STATE_ATTR_START,

    /**
     * @brief Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_CONTROLLER_ONU_FW_UPGRADE_STATE_ATTR_NAME = SAI_PON_CONTROLLER_ONU_FW_UPGRADE_STATE_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     */
    SAI_PON_CONTROLLER_ONU_FW_UPGRADE_STATE_ATTR_ONU_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_CONTROLLER_ONU_FW_UPGRADE_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_CONTROLLER_ONU_FW_UPGRADE_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_CONTROLLER_ONU_FW_UPGRADE_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_controller_onu_fw_upgrade_state_attr_t;

/**
 * @brief Get attribute for PON controller ONU FW upgrade state (STATE_DB, read-only)
 *
 * @param[in] pon_controller_onu_fw_upgrade_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_controller_onu_fw_upgrade_state_attribute_fn)(
        _In_ sai_object_id_t pon_controller_onu_fw_upgrade_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON controller ONU FW upgrade state
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
typedef sai_status_t (*sai_bulk_get_pon_controller_onu_fw_upgrade_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

typedef struct _sai_pon_controller_api_t
{
    sai_create_pon_controller_fn                                   create_pon_controller;
    sai_remove_pon_controller_fn                                   remove_pon_controller;
    sai_set_pon_controller_attribute_fn                            set_pon_controller_attribute;
    sai_get_pon_controller_attribute_fn                            get_pon_controller_attribute;
    sai_create_pon_downstream_qos_map_map_fn                       create_pon_downstream_qos_map_map;
    sai_remove_pon_downstream_qos_map_map_fn                       remove_pon_downstream_qos_map_map;
    sai_set_pon_downstream_qos_map_map_attribute_fn                set_pon_downstream_qos_map_map_attribute;
    sai_get_pon_downstream_qos_map_map_attribute_fn                get_pon_downstream_qos_map_map_attribute;
    sai_get_pon_controller_system_status_olt_attribute_fn          get_pon_controller_system_status_olt_attribute;
    sai_get_pon_controller_system_status_olt_onu_attribute_fn      get_pon_controller_system_status_olt_onu_attribute;
    sai_get_pon_controller_state_attribute_fn                      get_pon_controller_state_attribute;
    sai_get_pon_controller_onu_fw_upgrade_state_attribute_fn       get_pon_controller_onu_fw_upgrade_state_attribute;
    sai_bulk_get_pon_controller_system_status_olt_attribute_fn     get_pon_controller_system_status_olts_attribute;
    sai_bulk_get_pon_controller_system_status_olt_onu_attribute_fn get_pon_controller_system_status_olt_onus_attribute;
    sai_bulk_get_pon_controller_state_attribute_fn                 get_pon_controller_states_attribute;
    sai_bulk_get_pon_controller_onu_fw_upgrade_state_attribute_fn  get_pon_controller_onu_fw_upgrade_states_attribute;
} sai_pon_controller_api_t;

/**
 * @}
 */

#endif /* __SAIEXPERIMENTALPONCONTROLLER_H_ */
