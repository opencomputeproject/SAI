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
 * @file    saiexperimentalpon.h
 *
 * @brief   This module defines the experimental PON API.
 */

#if !defined (__SAIEXPERIMENTALPON_H_)
#define __SAIEXPERIMENTALPON_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALPON SAI - Experimental: PON specific API definitions
 *
 * @{
 */

/**
 * @brief PON logging level
 */
typedef enum _sai_pon_logging_level_t
{
    SAI_PON_LOGGING_LEVEL_EMERGENCY,

    SAI_PON_LOGGING_LEVEL_ALERT,

    SAI_PON_LOGGING_LEVEL_CRITICAL,

    SAI_PON_LOGGING_LEVEL_ERROR,

    SAI_PON_LOGGING_LEVEL_WARNING,

    SAI_PON_LOGGING_LEVEL_NOTICE,

    SAI_PON_LOGGING_LEVEL_INFO,

    SAI_PON_LOGGING_LEVEL_DEBUG,

    SAI_PON_LOGGING_LEVEL_TRACE,

    SAI_PON_LOGGING_LEVEL_DISABLE,

} sai_pon_logging_level_t;

/**
 * @brief PON logging enable/disable
 */
typedef enum _sai_pon_logging_enable_t
{
    SAI_PON_LOGGING_ENABLE_ENABLE,

    SAI_PON_LOGGING_ENABLE_DISABLE,

} sai_pon_logging_enable_t;

/**
 * @brief PON OLT interface encryption mode
 */
typedef enum _sai_pon_olt_intf_encryption_t
{
    SAI_PON_OLT_INTF_ENCRYPTION_DISABLED,

    SAI_PON_OLT_INTF_ENCRYPTION_BIDIRECTIONAL,

    SAI_PON_OLT_INTF_ENCRYPTION_BROADCAST,

    SAI_PON_OLT_INTF_ENCRYPTION_DOWNSTREAM,

} sai_pon_olt_intf_encryption_t;

/**
 * @brief Firmware upgrade download format
 */
typedef enum _sai_fw_upgrade_download_format_t
{
    SAI_FW_UPGRADE_DOWNLOAD_FORMAT_BASELINE_OMCI,

    SAI_FW_UPGRADE_DOWNLOAD_FORMAT_EXTENDED_OMCI,

} sai_fw_upgrade_download_format_t;

/**
 * @brief Duplex mode
 */
typedef enum _sai_duplex_t
{
    SAI_DUPLEX_AUTO,

    SAI_DUPLEX_FULL,

    SAI_DUPLEX_HALF,

} sai_duplex_t;

/**
 * @brief Speed mode
 */
typedef enum _sai_speed_t
{
    SAI_SPEED_AUTO,

    SAI_SPEED_10,

    SAI_SPEED_100,

    SAI_SPEED_1000,

    SAI_SPEED_2500,

    SAI_SPEED_5000,

    SAI_SPEED_10000,

} sai_speed_t;

/**
 * @brief PON service config value type
 */
typedef enum _sai_pon_service_config_value_type_t
{
    SAI_PON_SERVICE_CONFIG_VALUE_TYPE_ARRAY,

    SAI_PON_SERVICE_CONFIG_VALUE_TYPE_AUTOMATIC,

    SAI_PON_SERVICE_CONFIG_VALUE_TYPE_BOOLEAN,

    SAI_PON_SERVICE_CONFIG_VALUE_TYPE_DOUBLE,

    SAI_PON_SERVICE_CONFIG_VALUE_TYPE_INT32,

    SAI_PON_SERVICE_CONFIG_VALUE_TYPE_INT64,

    SAI_PON_SERVICE_CONFIG_VALUE_TYPE_OBJECT,

    SAI_PON_SERVICE_CONFIG_VALUE_TYPE_NULL,

    SAI_PON_SERVICE_CONFIG_VALUE_TYPE_STRING,

} sai_pon_service_config_value_type_t;

/**
 * @brief PON OLT protection group coordination mode
 */
typedef enum _sai_pon_olt_protection_group_coordination_mode_t
{
    SAI_PON_OLT_PROTECTION_GROUP_COORDINATION_MODE_STATEFUL,

    SAI_PON_OLT_PROTECTION_GROUP_COORDINATION_MODE_STATELESS,

} sai_pon_olt_protection_group_coordination_mode_t;

/**
 * @brief PON OLT interface protection role
 */
typedef enum _sai_pon_olt_intf_state_protection_role_t
{
    SAI_PON_OLT_INTF_STATE_PROTECTION_ROLE_ACTIVE,

    SAI_PON_OLT_INTF_STATE_PROTECTION_ROLE_STANDBY,

    SAI_PON_OLT_INTF_STATE_PROTECTION_ROLE_UNKNOWN,

} sai_pon_olt_intf_state_protection_role_t;

/**
 * @brief PON OLT protection group state
 */
typedef enum _sai_pon_olt_protection_group_state_protection_state_t
{
    SAI_PON_OLT_PROTECTION_GROUP_STATE_PROTECTION_STATE_PROTECTING,

    SAI_PON_OLT_PROTECTION_GROUP_STATE_PROTECTION_STATE_DEGRADED,

} sai_pon_olt_protection_group_state_protection_state_t;

/**
 * @brief Notification name for generic PON state change events
 */
#define SAI_SWITCH_NOTIFICATION_NAME_PON_STATE_CHANGE "pon_state_change"

/**
 * @brief Generic PON state change notification payload
 *
 * @count attr_list[attr_count]
 */
typedef struct _sai_pon_state_change_event_data_t
{
    /** PON object type that generated this state change */
    sai_object_type_t object_type;

    /** Operation that triggered this state change payload */
    sai_common_api_t op_type;

    /**
     * @brief PON object id that generated this state change
     *
     * @objects SAI_OBJECT_TYPE_PON_CONTROLLER_STATE, SAI_OBJECT_TYPE_PON_CONTROLLER_SYSTEM_STATUS_OLT, SAI_OBJECT_TYPE_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU, SAI_OBJECT_TYPE_PON_OLT_INTF_STATE, SAI_OBJECT_TYPE_PON_ONU_STATE, SAI_OBJECT_TYPE_PON_CONTROLLER_ONU_FW_UPGRADE_STATE, SAI_OBJECT_TYPE_PON_FIRMWARE_FILENAME_STATE, SAI_OBJECT_TYPE_PON_OLT_INTF_NETWORK_STATE, SAI_OBJECT_TYPE_PON_OLT_INTF_ONU_STATE, SAI_OBJECT_TYPE_PON_OLT_INTF_ONU_FW_UPGRADE_STATE, SAI_OBJECT_TYPE_PON_OLT_INTF_ONU_OPERATIONAL_STATE, SAI_OBJECT_TYPE_PON_OLT_INTF_NNI_NETWORK_LEARNING_TABLE_STATE, SAI_OBJECT_TYPE_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE, SAI_OBJECT_TYPE_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE, SAI_OBJECT_TYPE_PON_OLT_PLUG_STATE, SAI_OBJECT_TYPE_PON_OLT_PLUG_FW_UPGRADE_STATUS, SAI_OBJECT_TYPE_PON_OLT_PLUG_FW_BANK_VERSION_STATE, SAI_OBJECT_TYPE_PON_ONU_FW_BANK_VERSION_STATE, SAI_OBJECT_TYPE_PON_ONU_OLT_SERVICE_STATE, SAI_OBJECT_TYPE_PON_ONU_OLT_SERVICE_NETWORK_STATE, SAI_OBJECT_TYPE_PON_ONU_UNI_STATE, SAI_OBJECT_TYPE_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE, SAI_OBJECT_TYPE_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE, SAI_OBJECT_TYPE_PON_ONU_UNI_LEARNED_ADDRESSES_STATE
     */
    sai_object_id_t object_id;

    /** Number of attributes carried in attr_list */
    uint32_t attr_count;

    /**
     * @brief Attribute list representing the state snapshot/change
     *
     * @objects SAI_OBJECT_TYPE_PON_CONTROLLER
     */
    sai_attribute_t *attr_list;

} sai_pon_state_change_event_data_t;

/**
 * @brief Generic PON state change notification callback
 *
 * @count data[count]
 *
 * @param[in] count Number of state change events
 * @param[in] data Array of state change events
 */
typedef void (*sai_pon_state_change_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_pon_state_change_event_data_t *data);

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
     * @default empty
     */
    SAI_PON_CONTROLLER_ATTR_CONTROLLER_NAME = SAI_PON_CONTROLLER_ATTR_START,

    /**
     * @brief Device ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_CONTROLLER_ATTR_DEVICE_ID,

    /**
     * @brief Allow Unprovisioned Onus
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_CONTROLLER_ATTR_ALLOW_UNPROVISIONED_ONUS,

    /**
     * @brief Create Date
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_CONTROLLER_ATTR_CREATE_DATE,

    /**
     * @brief OLT Timeout
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_CONTROLLER_ATTR_OLT_TIMEOUT,

    /**
     * @brief Statistics Sample
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_CONTROLLER_ATTR_STATISTICS_SAMPLE,

    /**
     * @brief Logging Controller Console
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_CONTROLLER_CONSOLE,

    /**
     * @brief Logging Controller File
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_CONTROLLER_FILE,

    /**
     * @brief Logging Controller Syslog
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_CONTROLLER_SYSLOG,

    /**
     * @brief Logging Controller Database
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_CONTROLLER_DATABASE,

    /**
     * @brief Logging OLT Console
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_OLT_CONSOLE,

    /**
     * @brief Logging OLT File
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_OLT_FILE,

    /**
     * @brief Logging OLT Syslog
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_CONTROLLER_ATTR_LOGGING_OLT_SYSLOG,

    /**
     * @brief OLT Management Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_CONTROLLER_ATTR_OLT_MANAGEMENT_INTERFACE_NAME,

    /**
     * @brief Unprovisioned Age
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_CONTROLLER_ATTR_UNPROVISIONED_AGE,

    /**
     * @brief Refresh State On OLT Change
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
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
     * @default empty
     */
    SAI_PON_DOWNSTREAM_QOS_MAP_MAP_ATTR_NAME = SAI_PON_DOWNSTREAM_QOS_MAP_MAP_ATTR_START,

    /**
     * @brief Cos
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_DOWNSTREAM_QOS_MAP_MAP_ATTR_COS,

    /**
     * @brief OLT Service Offset
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_CONTROLLER_NAME = SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ATTR_START,

    /**
     * @brief MAC Address
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
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
     * @default empty
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU_ATTR_NAME = SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU_ATTR_START,

    /**
     * @brief MAC Address
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU_ATTR_MAC_ADDRESS,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
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
     * @default empty
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
     * @default empty
     */
    SAI_PON_CONTROLLER_ONU_FW_UPGRADE_STATE_ATTR_NAME = SAI_PON_CONTROLLER_ONU_FW_UPGRADE_STATE_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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

/**
 * @brief Attribute ID for PON OLT Interface
 */
typedef enum _sai_pon_olt_intf_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ATTR_START,

    /**
     * @brief Device ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_ATTR_DEVICE_ID,

    /**
     * @brief Port ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_ATTR_PORT_ID,

    /**
     * @brief PON Enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_OLT_INTF_ATTR_PON_ENABLE,

    /**
     * @brief Discovery Period
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_OLT_INTF_ATTR_DISCOVERY_PERIOD,

    /**
     * @brief Downstream FEC
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_OLT_INTF_ATTR_DOWNSTREAM_FEC,

    /**
     * @brief Encryption
     *
     * @type sai_pon_olt_intf_encryption_t
     * @flags CREATE_AND_SET
     * @default SAI_PON_OLT_INTF_ENCRYPTION_DISABLED
     */
    SAI_PON_OLT_INTF_ATTR_ENCRYPTION,

    /**
     * @brief Encryption Key Time
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_OLT_INTF_ATTR_ENCRYPTION_KEY_TIME,

    /**
     * @brief Error Detection Maximum HEC Ratio
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_OLT_INTF_ATTR_ERROR_DETECTION_MAXIMUM_HEC_RATIO,

    /**
     * @brief Error Detection Minimum HEC Sample
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_OLT_INTF_ATTR_ERROR_DETECTION_MINIMUM_HEC_SAMPLE,

    /**
     * @brief Error Detection Maximum Ratio
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_OLT_INTF_ATTR_ERROR_DETECTION_MAXIMUM_RATIO,

    /**
     * @brief Error Detection Minimum Sample
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_OLT_INTF_ATTR_ERROR_DETECTION_MINIMUM_SAMPLE,

    /**
     * @brief Guard Time
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_OLT_INTF_ATTR_GUARD_TIME,

    /**
     * @brief Max Frame Size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_OLT_INTF_ATTR_MAX_FRAME_SIZE,

    /**
     * @brief PON ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_OLT_INTF_ATTR_PON_ID,

    /**
     * @brief PON Tag
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_ATTR_PON_TAG,

    /**
     * @brief Aging Time
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_OLT_INTF_ATTR_AGING_TIME,

    /**
     * @brief Protection Hold
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_OLT_INTF_ATTR_PROTECTION_HOLD,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_attr_t;

/**
 * @brief Create PON OLT Interface
 *
 * @param[out] pon_olt_intf_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_olt_intf_fn)(
        _Out_ sai_object_id_t *pon_olt_intf_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON OLT Interface
 *
 * @param[in] pon_olt_intf_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_olt_intf_fn)(
        _In_ sai_object_id_t pon_olt_intf_id);

/**
 * @brief Set attribute for PON OLT Interface
 *
 * @param[in] pon_olt_intf_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_olt_intf_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON OLT Interface
 *
 * @param[in] pon_olt_intf_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON OLT Interface network
 */
typedef enum _sai_pon_olt_intf_network_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_NETWORK_ATTR_START,

    /**
     * @brief Network ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_NETWORK_ID,

    /**
     * @brief Learning Limit
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_LEARNING_LIMIT,

    /**
     * @brief Flooding GEMPORT ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_FLOODING_GEMPORT_ID,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_VLAN_ID,

    /**
     * @brief Flooding SLA Profile
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_FLOODING_SLA_PROFILE,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_NETWORK_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_NETWORK_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_network_attr_t;

/**
 * @brief Create PON OLT Interface network
 *
 * @param[out] pon_olt_intf_network_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_olt_intf_network_fn)(
        _Out_ sai_object_id_t *pon_olt_intf_network_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON OLT Interface network
 *
 * @param[in] pon_olt_intf_network_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_olt_intf_network_fn)(
        _In_ sai_object_id_t pon_olt_intf_network_id);

/**
 * @brief Set attribute for PON OLT Interface network
 *
 * @param[in] pon_olt_intf_network_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_olt_intf_network_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_network_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON OLT Interface network
 *
 * @param[in] pon_olt_intf_network_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_network_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_network_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON OLT Interface ONU
 */
typedef enum _sai_pon_olt_intf_onu_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_ONU_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief ONU ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_OLT_INTF_ONU_ATTR_ONU_ID,

    /**
     * @brief Disable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_OLT_INTF_ONU_ATTR_DISABLE,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_ONU_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_ONU_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_ONU_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_onu_attr_t;

/**
 * @brief Create PON OLT Interface ONU
 *
 * @param[out] pon_olt_intf_onu_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_olt_intf_onu_fn)(
        _Out_ sai_object_id_t *pon_olt_intf_onu_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON OLT Interface ONU
 *
 * @param[in] pon_olt_intf_onu_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_olt_intf_onu_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_id);

/**
 * @brief Set attribute for PON OLT Interface ONU
 *
 * @param[in] pon_olt_intf_onu_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_olt_intf_onu_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON OLT Interface ONU
 *
 * @param[in] pon_olt_intf_onu_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_onu_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON OLT Interface ONU OLT service
 */
typedef enum _sai_pon_olt_intf_onu_olt_service_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief Service Port ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_SERVICE_PORT_ID,

    /**
     * @brief Unicast ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_UNICAST_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_onu_olt_service_attr_t;

/**
 * @brief Create PON OLT Interface ONU OLT service
 *
 * @param[out] pon_olt_intf_onu_olt_service_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_olt_intf_onu_olt_service_fn)(
        _Out_ sai_object_id_t *pon_olt_intf_onu_olt_service_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON OLT Interface ONU OLT service
 *
 * @param[in] pon_olt_intf_onu_olt_service_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_olt_intf_onu_olt_service_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_olt_service_id);

/**
 * @brief Set attribute for PON OLT Interface ONU OLT service
 *
 * @param[in] pon_olt_intf_onu_olt_service_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_olt_intf_onu_olt_service_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_olt_service_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON OLT Interface ONU OLT service
 *
 * @param[in] pon_olt_intf_onu_olt_service_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_onu_olt_service_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_olt_service_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON OLT Interface ONU service TCONT
 */
typedef enum _sai_pon_olt_intf_onu_service_tcont_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Allocation ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_ALLOC_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_onu_service_tcont_attr_t;

/**
 * @brief Create PON OLT Interface ONU service TCONT
 *
 * @param[out] pon_olt_intf_onu_service_tcont_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_olt_intf_onu_service_tcont_fn)(
        _Out_ sai_object_id_t *pon_olt_intf_onu_service_tcont_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON OLT Interface ONU service TCONT
 *
 * @param[in] pon_olt_intf_onu_service_tcont_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_olt_intf_onu_service_tcont_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_service_tcont_id);

/**
 * @brief Set attribute for PON OLT Interface ONU service TCONT
 *
 * @param[in] pon_olt_intf_onu_service_tcont_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_olt_intf_onu_service_tcont_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_service_tcont_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON OLT Interface ONU service TCONT
 *
 * @param[in] pon_olt_intf_onu_service_tcont_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_onu_service_tcont_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_service_tcont_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON OLT Interface ONU service GEMPORT
 */
typedef enum _sai_pon_olt_intf_onu_service_gemport_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_OLT_SERVICE_ID,

    /**
     * @brief GEMPORT ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_GEMPORT_ID,

    /**
     * @brief TCONT Ref
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_TCONT_REF,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_onu_service_gemport_attr_t;

/**
 * @brief Create PON OLT Interface ONU service GEMPORT
 *
 * @param[out] pon_olt_intf_onu_service_gemport_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_olt_intf_onu_service_gemport_fn)(
        _Out_ sai_object_id_t *pon_olt_intf_onu_service_gemport_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON OLT Interface ONU service GEMPORT
 *
 * @param[in] pon_olt_intf_onu_service_gemport_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_olt_intf_onu_service_gemport_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_service_gemport_id);

/**
 * @brief Set attribute for PON OLT Interface ONU service GEMPORT
 *
 * @param[in] pon_olt_intf_onu_service_gemport_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_olt_intf_onu_service_gemport_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_service_gemport_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON OLT Interface ONU service GEMPORT
 *
 * @param[in] pon_olt_intf_onu_service_gemport_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_onu_service_gemport_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_service_gemport_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON OLT protection group
 */
typedef enum _sai_pon_olt_protection_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_START,

    /**
     * @brief Group Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_GROUP_NAME = SAI_PON_OLT_PROTECTION_GROUP_ATTR_START,

    /**
     * @brief Admin Status
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_ADMIN_STATUS,

    /**
     * @brief Local OLT Interface
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_LOCAL_OLT_INTERFACE,

    /**
     * @brief Peer OLT
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_PEER_OLT,

    /**
     * @brief Peer OLT Interface
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_PEER_OLT_INTERFACE,

    /**
     * @brief Coordination Mode
     *
     * @type sai_pon_olt_protection_group_coordination_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_PON_OLT_PROTECTION_GROUP_COORDINATION_MODE_STATEFUL
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_COORDINATION_MODE,

    /**
     * @brief Revertive
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_REVERTIVE,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_protection_group_attr_t;

/**
 * @brief Create PON OLT protection group
 *
 * @param[out] pon_olt_protection_group_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pon_olt_protection_group_fn)(
        _Out_ sai_object_id_t *pon_olt_protection_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove PON OLT protection group
 *
 * @param[in] pon_olt_protection_group_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pon_olt_protection_group_fn)(
        _In_ sai_object_id_t pon_olt_protection_group_id);

/**
 * @brief Set attribute for PON OLT protection group
 *
 * @param[in] pon_olt_protection_group_id Entry id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pon_olt_protection_group_attribute_fn)(
        _In_ sai_object_id_t pon_olt_protection_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for PON OLT protection group
 *
 * @param[in] pon_olt_protection_group_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_protection_group_attribute_fn)(
        _In_ sai_object_id_t pon_olt_protection_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute ID for PON OLT Interface ONU operational state (STATE_DB)
 */
typedef enum _sai_pon_olt_intf_onu_operational_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_ONU_OPERATIONAL_STATE_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_OPERATIONAL_STATE_ATTR_OLT_NAME = SAI_PON_OLT_INTF_ONU_OPERATIONAL_STATE_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_OLT_INTF_ONU_OPERATIONAL_STATE_ATTR_ID,

    /**
     * @brief Operational State
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_ONU_OPERATIONAL_STATE_ATTR_OPERATIONAL_STATE,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_ONU_OPERATIONAL_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_ONU_OPERATIONAL_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_ONU_OPERATIONAL_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_onu_operational_state_attr_t;

/**
 * @brief Get attribute for PON OLT Interface ONU operational state (STATE_DB, read-only)
 *
 * @param[in] pon_olt_intf_onu_operational_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_onu_operational_state_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_operational_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT Interface ONU operational state
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
typedef sai_status_t (*sai_bulk_get_pon_olt_intf_onu_operational_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT Interface NNI network learning table state (STATE_DB)
 */
typedef enum _sai_pon_olt_intf_nni_network_learning_table_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_NNI_NETWORK_LEARNING_TABLE_STATE_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_NNI_NETWORK_LEARNING_TABLE_STATE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_NNI_NETWORK_LEARNING_TABLE_STATE_ATTR_START,

    /**
     * @brief Network ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
     */
    SAI_PON_OLT_INTF_NNI_NETWORK_LEARNING_TABLE_STATE_ATTR_NETWORK_ID,

    /**
     * @brief MAC Address
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_NNI_NETWORK_LEARNING_TABLE_STATE_ATTR_MAC_ADDRESS,

    /**
     * @brief Unicast ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_OLT_INTF_NNI_NETWORK_LEARNING_TABLE_STATE_ATTR_UNICAST_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_NNI_NETWORK_LEARNING_TABLE_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_NNI_NETWORK_LEARNING_TABLE_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_NNI_NETWORK_LEARNING_TABLE_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_nni_network_learning_table_state_attr_t;

/**
 * @brief Get attribute for PON OLT Interface NNI network learning table state (STATE_DB, read-only)
 *
 * @param[in] pon_olt_intf_nni_network_learning_table_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_nni_network_learning_table_state_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_nni_network_learning_table_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT Interface NNI network learning table state
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
typedef sai_status_t (*sai_bulk_get_pon_olt_intf_nni_network_learning_table_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON firmware filename state (STATE_DB)
 */
typedef enum _sai_pon_firmware_filename_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_FIRMWARE_FILENAME_STATE_ATTR_START,

    /**
     * @brief Filename
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_FIRMWARE_FILENAME_STATE_ATTR_FILENAME = SAI_PON_FIRMWARE_FILENAME_STATE_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_PON_FIRMWARE_FILENAME_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_FIRMWARE_FILENAME_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_FIRMWARE_FILENAME_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_firmware_filename_state_attr_t;

/**
 * @brief Get attribute for PON firmware filename state (STATE_DB, read-only)
 *
 * @param[in] pon_firmware_filename_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_firmware_filename_state_attribute_fn)(
        _In_ sai_object_id_t pon_firmware_filename_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON firmware filename state
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
typedef sai_status_t (*sai_bulk_get_pon_firmware_filename_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT Interface state (STATE_DB)
 */
typedef enum _sai_pon_olt_intf_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_STATE_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_STATE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_STATE_ATTR_START,

    /**
     * @brief Timestamp
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_TIMESTAMP,

    /**
     * @brief Protection Role
     *
     * @type sai_pon_olt_intf_state_protection_role_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_PROTECTION_ROLE,

    /**
     * @brief Fiber Reach
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_FIBER_REACH,

    /**
     * @brief Laser Shutdown
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_LASER_SHUTDOWN,

    /**
     * @brief Loss Of Signal
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_LOSS_OF_SIGNAL,

    /**
     * @brief PON Enable
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_PON_ENABLE,

    /**
     * @brief Discovery Period
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_DISCOVERY_PERIOD,

    /**
     * @brief Downstream FEC
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_DOWNSTREAM_FEC,

    /**
     * @brief Encryption
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_ENCRYPTION,

    /**
     * @brief Encryption Key Time
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_OLT_INTF_STATE_ATTR_ENCRYPTION_KEY_TIME,

    /**
     * @brief Error Detection Maximum HEC Ratio
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_ERROR_DETECTION_MAXIMUM_HEC_RATIO,

    /**
     * @brief Error Detection Minimum HEC Sample
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_OLT_INTF_STATE_ATTR_ERROR_DETECTION_MINIMUM_HEC_SAMPLE,

    /**
     * @brief Error Detection Maximum Ratio
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_ERROR_DETECTION_MAXIMUM_RATIO,

    /**
     * @brief Error Detection Minimum Sample
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_OLT_INTF_STATE_ATTR_ERROR_DETECTION_MINIMUM_SAMPLE,

    /**
     * @brief Guard Time
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_GUARD_TIME,

    /**
     * @brief MAC Address
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_MAC_ADDRESS,

    /**
     * @brief Max Frame Size
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_MAX_FRAME_SIZE,

    /**
     * @brief PON ID
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_PON_ID,

    /**
     * @brief PON Tag
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_PON_TAG,

    /**
     * @brief Port ID
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_PORT_ID,

    /**
     * @brief Device ID
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATE_ATTR_DEVICE_ID,

    /**
     * @brief Aging Time
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_OLT_INTF_STATE_ATTR_AGING_TIME,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_state_attr_t;

/**
 * @brief Get attribute for PON OLT Interface state (STATE_DB, read-only)
 *
 * @param[in] pon_olt_intf_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_state_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT Interface state
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
typedef sai_status_t (*sai_bulk_get_pon_olt_intf_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT Interface network state (STATE_DB)
 */
typedef enum _sai_pon_olt_intf_network_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_START,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan true
     * @default 0
     */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_VLAN_ID,

    /**
     * @brief Network ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_NETWORK_ID,

    /**
     * @brief Learning Limit
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_LEARNING_LIMIT,

    /**
     * @brief PON Flood ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_PON_FLOOD_ID,

    /**
     * @brief Flooding SLA Downstream Guaranteed Rate
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_FLOODING_SLA_DOWNSTREAM_GUARANTEED_RATE,

    /**
     * @brief Flooding SLA Downstream Guaranteed Maximum Burst
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_FLOODING_SLA_DOWNSTREAM_GUARANTEED_MAXIMUM_BURST,

    /**
     * @brief Flooding SLA Downstream Best Effort Rate
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_FLOODING_SLA_DOWNSTREAM_BEST_EFFORT_RATE,

    /**
     * @brief Flooding SLA Downstream Best Effort Maximum Burst
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_FLOODING_SLA_DOWNSTREAM_BEST_EFFORT_MAXIMUM_BURST,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_network_state_attr_t;

/**
 * @brief Get attribute for PON OLT Interface network state (STATE_DB, read-only)
 *
 * @param[in] pon_olt_intf_network_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_network_state_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_network_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT Interface network state
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
typedef sai_status_t (*sai_bulk_get_pon_olt_intf_network_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT Interface ONU state (STATE_DB)
 */
typedef enum _sai_pon_olt_intf_onu_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_ONU_STATE_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_STATE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_STATE_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_STATE_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief ONU ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_OLT_INTF_ONU_STATE_ATTR_ONU_ID,

    /**
     * @brief Disable
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_ONU_STATE_ATTR_DISABLE,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_ONU_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_ONU_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_ONU_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_onu_state_attr_t;

/**
 * @brief Get attribute for PON OLT Interface ONU state (STATE_DB, read-only)
 *
 * @param[in] pon_olt_intf_onu_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_onu_state_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT Interface ONU state
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
typedef sai_status_t (*sai_bulk_get_pon_olt_intf_onu_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT Interface ONU FW upgrade state (STATE_DB)
 */
typedef enum _sai_pon_olt_intf_onu_fw_upgrade_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_ONU_FW_UPGRADE_STATE_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_FW_UPGRADE_STATE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_FW_UPGRADE_STATE_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_FW_UPGRADE_STATE_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_ONU_FW_UPGRADE_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_ONU_FW_UPGRADE_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_ONU_FW_UPGRADE_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_onu_fw_upgrade_state_attr_t;

/**
 * @brief Get attribute for PON OLT Interface ONU FW upgrade state (STATE_DB, read-only)
 *
 * @param[in] pon_olt_intf_onu_fw_upgrade_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_onu_fw_upgrade_state_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_fw_upgrade_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT Interface ONU FW upgrade state
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
typedef sai_status_t (*sai_bulk_get_pon_olt_intf_onu_fw_upgrade_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT Interface ONU service TCONT state (STATE_DB)
 */
typedef enum _sai_pon_olt_intf_onu_service_tcont_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief Service Port ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE_ATTR_SERVICE_PORT_ID,

    /**
     * @brief Allocation ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE_ATTR_ALLOC_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_onu_service_tcont_state_attr_t;

/**
 * @brief Get attribute for PON OLT Interface ONU service TCONT state (STATE_DB, read-only)
 *
 * @param[in] pon_olt_intf_onu_service_tcont_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_onu_service_tcont_state_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_service_tcont_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT Interface ONU service TCONT state
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
typedef sai_status_t (*sai_bulk_get_pon_olt_intf_onu_service_tcont_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT Interface ONU service GEMPORT state (STATE_DB)
 */
typedef enum _sai_pon_olt_intf_onu_service_gemport_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_START,

    /**
     * @brief OLT Interface Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief Service Port ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_SERVICE_PORT_ID,

    /**
     * @brief GEMPORT ID
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_GEMPORT_ID,

    /**
     * @brief TCONT Ref
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_TCONT_REF,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_onu_service_gemport_state_attr_t;

/**
 * @brief Get attribute for PON OLT Interface ONU service GEMPORT state (STATE_DB, read-only)
 *
 * @param[in] pon_olt_intf_onu_service_gemport_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_onu_service_gemport_state_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_onu_service_gemport_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT Interface ONU service GEMPORT state
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
typedef sai_status_t (*sai_bulk_get_pon_olt_intf_onu_service_gemport_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT protection group state (STATE_DB)
 */
typedef enum _sai_pon_olt_protection_group_state_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_PROTECTION_GROUP_STATE_ATTR_START,

    /**
     * @brief Group Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_PROTECTION_GROUP_STATE_ATTR_GROUP_NAME = SAI_PON_OLT_PROTECTION_GROUP_STATE_ATTR_START,

    /**
     * @brief Protection State
     *
     * @type sai_pon_olt_protection_group_state_protection_state_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PROTECTION_GROUP_STATE_ATTR_PROTECTION_STATE,

    /**
     * @brief Last Failover Reason
     *
     * @type sai_s8_list_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PROTECTION_GROUP_STATE_ATTR_LAST_FAILOVER_REASON,

    /**
     * @brief Last Failover Time
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_PROTECTION_GROUP_STATE_ATTR_LAST_FAILOVER_TIME,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_PROTECTION_GROUP_STATE_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_PROTECTION_GROUP_STATE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_PROTECTION_GROUP_STATE_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_protection_group_state_attr_t;

/**
 * @brief Get attribute for PON OLT protection group state (STATE_DB, read-only)
 *
 * @param[in] pon_olt_protection_group_state_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_protection_group_state_attribute_fn)(
        _In_ sai_object_id_t pon_olt_protection_group_state_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT protection group state
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
typedef sai_status_t (*sai_bulk_get_pon_olt_protection_group_state_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT Interface statistics binned (COUNTERS_DB)
 */
typedef enum _sai_pon_olt_intf_statistics_binned_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_OLT_NAME = SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_START,

    /**
     * @brief OLT Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_OLT_STATS_ID,

    /**
     * @brief Timestamp
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TIMESTAMP,

    /**
     * @brief Offline Onus Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_OFFLINE_ONUS_COUNT,

    /**
     * @brief Online Onus Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_ONLINE_ONUS_COUNT,

    /**
     * @brief PON FEC Seconds
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_PON_FEC_SECONDS,

    /**
     * @brief Rx BW Ethernet Rate Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_BW_ETHERNET_RATE_BPS,

    /**
     * @brief Rx BW Overhead Burst Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_BW_OVERHEAD_BURST_BPS,

    /**
     * @brief Rx BW Overhead FEC Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_BW_OVERHEAD_FEC_BPS,

    /**
     * @brief Rx BW Overhead Total Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_BW_OVERHEAD_TOTAL_BPS,

    /**
     * @brief Rx BW Packet Used Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_BW_PACKET_USED_BPS,

    /**
     * @brief Rx BW Total Free Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_BW_TOTAL_FREE_BPS,

    /**
     * @brief Rx BW Total Used Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_BW_TOTAL_USED_BPS,

    /**
     * @brief Rx BW Total Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_BW_TOTAL_UTIL,

    /**
     * @brief Rx Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_BANDWIDTH_REQS,

    /**
     * @brief Rx Crc32 Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_CRC32_DROPS,

    /**
     * @brief Rx Crc8 Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_CRC8_ERRORS,

    /**
     * @brief Rx Empty Slots
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_EMPTY_SLOTS,

    /**
     * @brief Rx Encrypted Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_ENCRYPTED_FRAMES,

    /**
     * @brief Rx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_ENCRYPTED_OCTETS,

    /**
     * @brief Rx Encrypted Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_ENCRYPTED_SEGMENTS,

    /**
     * @brief Rx Errored Bip Bits
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_ERRORED_BIP_BITS,

    /**
     * @brief Rx Errored Bip Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_ERRORED_BIP_BLOCKS,

    /**
     * @brief Rx FEC Corrected Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FEC_CORRECTED_BLOCKS,

    /**
     * @brief Rx FEC Corrections
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FEC_CORRECTIONS,

    /**
     * @brief Rx FEC Good Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FEC_GOOD_BLOCKS,

    /**
     * @brief Rx FEC Uncorrectable Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FEC_UNCORRECTABLE_BLOCKS,

    /**
     * @brief Rx Filtered Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FILTERED_FRAMES,

    /**
     * @brief Rx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FRAMES_1024_1518,

    /**
     * @brief Rx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FRAMES_128_255,

    /**
     * @brief Rx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FRAMES_1519_PLUS,

    /**
     * @brief Rx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FRAMES_256_511,

    /**
     * @brief Rx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FRAMES_512_1023,

    /**
     * @brief Rx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FRAMES_64,

    /**
     * @brief Rx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FRAMES_65_127,

    /**
     * @brief Rx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_FRAMES_GREEN,

    /**
     * @brief Rx Good Bip Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_GOOD_BIP_BLOCKS,

    /**
     * @brief Rx HEC Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_HEC_ERRORS,

    /**
     * @brief Rx Idle Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_IDLE_OCTETS,

    /**
     * @brief Rx Mpcp Ploam
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_MPCP_PLOAM,

    /**
     * @brief Rx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Rx OMCI Mic Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_OMCI_MIC_ERRORS,

    /**
     * @brief Rx Optical Level Idle
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_OPTICAL_LEVEL_IDLE,

    /**
     * @brief Rx Overflow Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_OVERFLOW_DROPS,

    /**
     * @brief Rx Overflow Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_OVERFLOW_OCTETS,

    /**
     * @brief Rx Plain Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_PLAIN_FRAMES,

    /**
     * @brief Rx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_PLAIN_OCTETS,

    /**
     * @brief Rx Plain Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_PLAIN_SEGMENTS,

    /**
     * @brief Rx Ploam Mic Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_PLOAM_MIC_ERRORS,

    /**
     * @brief Rx Too Long Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_TOO_LONG_DROPS,

    /**
     * @brief Rx Too Short Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_TOO_SHORT_DROPS,

    /**
     * @brief Rx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_TOTAL_OCTETS,

    /**
     * @brief Rx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_UNICAST_OCTETS,

    /**
     * @brief Rx Unmatched Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_RX_UNMATCHED_DROPS,

    /**
     * @brief Total Onus Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TOTAL_ONUS_COUNT,

    /**
     * @brief Tx BW Ethernet Rate Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_BW_ETHERNET_RATE_BPS,

    /**
     * @brief Tx BW Overhead FEC Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_BW_OVERHEAD_FEC_BPS,

    /**
     * @brief Tx BW Overhead Framing Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_BW_OVERHEAD_FRAMING_BPS,

    /**
     * @brief Tx BW Overhead Total Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_BW_OVERHEAD_TOTAL_BPS,

    /**
     * @brief Tx BW Packet Used Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_BW_PACKET_USED_BPS,

    /**
     * @brief Tx BW Total Free Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_BW_TOTAL_FREE_BPS,

    /**
     * @brief Tx BW Total Used Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_BW_TOTAL_USED_BPS,

    /**
     * @brief Tx BW Total Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_BW_TOTAL_UTIL,

    /**
     * @brief Tx Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_BANDWIDTH_REQS,

    /**
     * @brief Tx Encrypted Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_ENCRYPTED_FRAMES,

    /**
     * @brief Tx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_ENCRYPTED_OCTETS,

    /**
     * @brief Tx Encrypted Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_ENCRYPTED_SEGMENTS,

    /**
     * @brief Tx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_FRAMES_1024_1518,

    /**
     * @brief Tx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_FRAMES_128_255,

    /**
     * @brief Tx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_FRAMES_1519_PLUS,

    /**
     * @brief Tx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_FRAMES_256_511,

    /**
     * @brief Tx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_FRAMES_512_1023,

    /**
     * @brief Tx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_FRAMES_64,

    /**
     * @brief Tx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_FRAMES_65_127,

    /**
     * @brief Tx Frames Broadcast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_FRAMES_BROADCAST,

    /**
     * @brief Tx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_FRAMES_GREEN,

    /**
     * @brief Tx Frames Multicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_FRAMES_MULTICAST,

    /**
     * @brief Tx Frames Unicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_FRAMES_UNICAST,

    /**
     * @brief Tx Idle Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_IDLE_OCTETS,

    /**
     * @brief Tx Mpcp Ploam
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_MPCP_PLOAM,

    /**
     * @brief Tx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Tx OAM
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_OAM,

    /**
     * @brief Tx Optical Level
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_OPTICAL_LEVEL,

    /**
     * @brief Tx Plain Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_PLAIN_FRAMES,

    /**
     * @brief Tx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_PLAIN_OCTETS,

    /**
     * @brief Tx Plain Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_PLAIN_SEGMENTS,

    /**
     * @brief Tx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_TOTAL_OCTETS,

    /**
     * @brief Tx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_TX_UNICAST_OCTETS,

    /**
     * @brief Uninventoried Onus Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_UNINVENTORIED_ONUS_COUNT,

    /**
     * @brief Unprovisioned Onus Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_UNPROVISIONED_ONUS_COUNT,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_intf_statistics_binned_attr_t;

/**
 * @brief Get attribute for PON OLT Interface statistics binned (COUNTERS_DB, read-only)
 *
 * @param[in] pon_olt_intf_statistics_binned_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_intf_statistics_binned_attribute_fn)(
        _In_ sai_object_id_t pon_olt_intf_statistics_binned_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT Interface statistics binned
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
typedef sai_status_t (*sai_bulk_get_pon_olt_intf_statistics_binned_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT statistics binned PON flooding (COUNTERS_DB)
 */
typedef enum _sai_pon_olt_statistics_binned_pon_flooding_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_OLT_NAME = SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_START,

    /**
     * @brief OLT Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_OLT_STATS_ID,

    /**
     * @brief Flood ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_FLOOD_ID,

    /**
     * @brief Tx BW Best Effort SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_BW_BEST_EFFORT_SLA_UTIL,

    /**
     * @brief Tx BW Best Effort SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_BW_BEST_EFFORT_SLA_BPS,

    /**
     * @brief Tx BW Guaranteed SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_BW_GUARANTEED_SLA_UTIL,

    /**
     * @brief Tx BW Guaranteed SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_BW_GUARANTEED_SLA_BPS,

    /**
     * @brief Tx BW Total SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_BW_TOTAL_SLA_UTIL,

    /**
     * @brief Tx BW Total SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_BW_TOTAL_SLA_BPS,

    /**
     * @brief Tx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_ENCRYPTED_OCTETS,

    /**
     * @brief Tx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_FRAMES_1024_1518,

    /**
     * @brief Tx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_FRAMES_128_255,

    /**
     * @brief Tx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_FRAMES_1519_PLUS,

    /**
     * @brief Tx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_FRAMES_256_511,

    /**
     * @brief Tx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_FRAMES_512_1023,

    /**
     * @brief Tx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_FRAMES_64,

    /**
     * @brief Tx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_FRAMES_65_127,

    /**
     * @brief Tx Frames Broadcast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_FRAMES_BROADCAST,

    /**
     * @brief Tx Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_FRAMES,

    /**
     * @brief Tx Frames Multicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_FRAMES_MULTICAST,

    /**
     * @brief Tx Frames Unicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_FRAMES_UNICAST,

    /**
     * @brief Tx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Tx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_PLAIN_OCTETS,

    /**
     * @brief Tx Rate Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_RATE_BPS,

    /**
     * @brief Tx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_TOTAL_OCTETS,

    /**
     * @brief Tx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_TX_UNICAST_OCTETS,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_statistics_binned_pon_flooding_attr_t;

/**
 * @brief Get attribute for PON OLT statistics binned PON flooding (COUNTERS_DB, read-only)
 *
 * @param[in] pon_olt_statistics_binned_pon_flooding_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_statistics_binned_pon_flooding_attribute_fn)(
        _In_ sai_object_id_t pon_olt_statistics_binned_pon_flooding_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT statistics binned PON flooding
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
typedef sai_status_t (*sai_bulk_get_pon_olt_statistics_binned_pon_flooding_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT statistics binned PON flooding network (COUNTERS_DB)
 */
typedef enum _sai_pon_olt_statistics_binned_pon_flooding_network_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_NETWORK_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_NETWORK_ATTR_OLT_NAME = SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_NETWORK_ATTR_START,

    /**
     * @brief OLT Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_NETWORK_ATTR_OLT_STATS_ID,

    /**
     * @brief Flood ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_NETWORK_ATTR_FLOOD_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_NETWORK_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_NETWORK_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_NETWORK_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_statistics_binned_pon_flooding_network_attr_t;

/**
 * @brief Get attribute for PON OLT statistics binned PON flooding network (COUNTERS_DB, read-only)
 *
 * @param[in] pon_olt_statistics_binned_pon_flooding_network_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_statistics_binned_pon_flooding_network_attribute_fn)(
        _In_ sai_object_id_t pon_olt_statistics_binned_pon_flooding_network_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT statistics binned PON flooding network
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
typedef sai_status_t (*sai_bulk_get_pon_olt_statistics_binned_pon_flooding_network_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT statistics accumulating (COUNTERS_DB)
 */
typedef enum _sai_pon_olt_statistics_accumulating_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_OLT_NAME = SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_START,

    /**
     * @brief Timestamp
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TIMESTAMP,

    /**
     * @brief Offline Onus Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_OFFLINE_ONUS_COUNT,

    /**
     * @brief Online Onus Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_ONLINE_ONUS_COUNT,

    /**
     * @brief PON FEC Seconds
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_PON_FEC_SECONDS,

    /**
     * @brief Rx BW Ethernet Rate Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_BW_ETHERNET_RATE_BPS,

    /**
     * @brief Rx BW Overhead Burst Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_BW_OVERHEAD_BURST_BPS,

    /**
     * @brief Rx BW Overhead FEC Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_BW_OVERHEAD_FEC_BPS,

    /**
     * @brief Rx BW Overhead Total Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_BW_OVERHEAD_TOTAL_BPS,

    /**
     * @brief Rx BW Packet Used Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_BW_PACKET_USED_BPS,

    /**
     * @brief Rx BW Total Free Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_BW_TOTAL_FREE_BPS,

    /**
     * @brief Rx BW Total Used Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_BW_TOTAL_USED_BPS,

    /**
     * @brief Rx BW Total Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_BW_TOTAL_UTIL,

    /**
     * @brief Rx Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_BANDWIDTH_REQS,

    /**
     * @brief Rx Crc32 Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_CRC32_DROPS,

    /**
     * @brief Rx Crc8 Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_CRC8_ERRORS,

    /**
     * @brief Rx Empty Slots
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_EMPTY_SLOTS,

    /**
     * @brief Rx Encrypted Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_ENCRYPTED_FRAMES,

    /**
     * @brief Rx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_ENCRYPTED_OCTETS,

    /**
     * @brief Rx Encrypted Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_ENCRYPTED_SEGMENTS,

    /**
     * @brief Rx Errored Bip Bits
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_ERRORED_BIP_BITS,

    /**
     * @brief Rx Errored Bip Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_ERRORED_BIP_BLOCKS,

    /**
     * @brief Rx FEC Corrected Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FEC_CORRECTED_BLOCKS,

    /**
     * @brief Rx FEC Corrections
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FEC_CORRECTIONS,

    /**
     * @brief Rx FEC Good Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FEC_GOOD_BLOCKS,

    /**
     * @brief Rx FEC Uncorrectable Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FEC_UNCORRECTABLE_BLOCKS,

    /**
     * @brief Rx Filtered Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FILTERED_FRAMES,

    /**
     * @brief Rx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FRAMES_1024_1518,

    /**
     * @brief Rx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FRAMES_128_255,

    /**
     * @brief Rx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FRAMES_1519_PLUS,

    /**
     * @brief Rx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FRAMES_256_511,

    /**
     * @brief Rx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FRAMES_512_1023,

    /**
     * @brief Rx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FRAMES_64,

    /**
     * @brief Rx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FRAMES_65_127,

    /**
     * @brief Rx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_FRAMES_GREEN,

    /**
     * @brief Rx Good Bip Blocks
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_GOOD_BIP_BLOCKS,

    /**
     * @brief Rx HEC Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_HEC_ERRORS,

    /**
     * @brief Rx Idle Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_IDLE_OCTETS,

    /**
     * @brief Rx Mpcp Ploam
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_MPCP_PLOAM,

    /**
     * @brief Rx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Rx OMCI Mic Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_OMCI_MIC_ERRORS,

    /**
     * @brief Rx Optical Level Idle
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_OPTICAL_LEVEL_IDLE,

    /**
     * @brief Rx Overflow Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_OVERFLOW_DROPS,

    /**
     * @brief Rx Overflow Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_OVERFLOW_OCTETS,

    /**
     * @brief Rx Plain Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_PLAIN_FRAMES,

    /**
     * @brief Rx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_PLAIN_OCTETS,

    /**
     * @brief Rx Plain Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_PLAIN_SEGMENTS,

    /**
     * @brief Rx Ploam Mic Errors
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_PLOAM_MIC_ERRORS,

    /**
     * @brief Rx Too Long Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_TOO_LONG_DROPS,

    /**
     * @brief Rx Too Short Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_TOO_SHORT_DROPS,

    /**
     * @brief Rx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_TOTAL_OCTETS,

    /**
     * @brief Rx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_UNICAST_OCTETS,

    /**
     * @brief Rx Unmatched Drops
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_RX_UNMATCHED_DROPS,

    /**
     * @brief Total Onus Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TOTAL_ONUS_COUNT,

    /**
     * @brief Tx BW Ethernet Rate Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_BW_ETHERNET_RATE_BPS,

    /**
     * @brief Tx BW Overhead FEC Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_BW_OVERHEAD_FEC_BPS,

    /**
     * @brief Tx BW Overhead Framing Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_BW_OVERHEAD_FRAMING_BPS,

    /**
     * @brief Tx BW Overhead Total Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_BW_OVERHEAD_TOTAL_BPS,

    /**
     * @brief Tx BW Packet Used Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_BW_PACKET_USED_BPS,

    /**
     * @brief Tx BW Total Free Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_BW_TOTAL_FREE_BPS,

    /**
     * @brief Tx BW Total Used Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_BW_TOTAL_USED_BPS,

    /**
     * @brief Tx BW Total Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_BW_TOTAL_UTIL,

    /**
     * @brief Tx Bandwidth Reqs
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_BANDWIDTH_REQS,

    /**
     * @brief Tx Encrypted Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_ENCRYPTED_FRAMES,

    /**
     * @brief Tx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_ENCRYPTED_OCTETS,

    /**
     * @brief Tx Encrypted Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_ENCRYPTED_SEGMENTS,

    /**
     * @brief Tx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_FRAMES_1024_1518,

    /**
     * @brief Tx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_FRAMES_128_255,

    /**
     * @brief Tx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_FRAMES_1519_PLUS,

    /**
     * @brief Tx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_FRAMES_256_511,

    /**
     * @brief Tx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_FRAMES_512_1023,

    /**
     * @brief Tx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_FRAMES_64,

    /**
     * @brief Tx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_FRAMES_65_127,

    /**
     * @brief Tx Frames Broadcast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_FRAMES_BROADCAST,

    /**
     * @brief Tx Frames Green
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_FRAMES_GREEN,

    /**
     * @brief Tx Frames Multicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_FRAMES_MULTICAST,

    /**
     * @brief Tx Frames Unicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_FRAMES_UNICAST,

    /**
     * @brief Tx Idle Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_IDLE_OCTETS,

    /**
     * @brief Tx Mpcp Ploam
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_MPCP_PLOAM,

    /**
     * @brief Tx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Tx OAM
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_OAM,

    /**
     * @brief Tx Optical Level
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_OPTICAL_LEVEL,

    /**
     * @brief Tx Plain Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_PLAIN_FRAMES,

    /**
     * @brief Tx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_PLAIN_OCTETS,

    /**
     * @brief Tx Plain Segments
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_PLAIN_SEGMENTS,

    /**
     * @brief Tx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_TOTAL_OCTETS,

    /**
     * @brief Tx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_TX_UNICAST_OCTETS,

    /**
     * @brief Uninventoried Onus Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_UNINVENTORIED_ONUS_COUNT,

    /**
     * @brief Unprovisioned Onus Count
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_UNPROVISIONED_ONUS_COUNT,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_statistics_accumulating_attr_t;

/**
 * @brief Get attribute for PON OLT statistics accumulating (COUNTERS_DB, read-only)
 *
 * @param[in] pon_olt_statistics_accumulating_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_statistics_accumulating_attribute_fn)(
        _In_ sai_object_id_t pon_olt_statistics_accumulating_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT statistics accumulating
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
typedef sai_status_t (*sai_bulk_get_pon_olt_statistics_accumulating_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT statistics accumulating env (COUNTERS_DB)
 */
typedef enum _sai_pon_olt_statistics_accumulating_env_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ENV_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ENV_ATTR_OLT_NAME = SAI_PON_OLT_STATISTICS_ACCUMULATING_ENV_ATTR_START,

    /**
     * @brief Current
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ENV_ATTR_CURRENT,

    /**
     * @brief Transmit Bias
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ENV_ATTR_TRANSMIT_BIAS,

    /**
     * @brief Voltage
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ENV_ATTR_VOLTAGE,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ENV_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ENV_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_ENV_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_statistics_accumulating_env_attr_t;

/**
 * @brief Get attribute for PON OLT statistics accumulating env (COUNTERS_DB, read-only)
 *
 * @param[in] pon_olt_statistics_accumulating_env_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_statistics_accumulating_env_attribute_fn)(
        _In_ sai_object_id_t pon_olt_statistics_accumulating_env_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT statistics accumulating env
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
typedef sai_status_t (*sai_bulk_get_pon_olt_statistics_accumulating_env_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT statistics accumulating temp (COUNTERS_DB)
 */
typedef enum _sai_pon_olt_statistics_accumulating_temp_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_TEMP_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_TEMP_ATTR_OLT_NAME = SAI_PON_OLT_STATISTICS_ACCUMULATING_TEMP_ATTR_START,

    /**
     * @brief ASIC
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_TEMP_ATTR_ASIC,

    /**
     * @brief Laser
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_TEMP_ATTR_LASER,

    /**
     * @brief Xcvr
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_TEMP_ATTR_XCVR,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_TEMP_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_TEMP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_TEMP_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_statistics_accumulating_temp_attr_t;

/**
 * @brief Get attribute for PON OLT statistics accumulating temp (COUNTERS_DB, read-only)
 *
 * @param[in] pon_olt_statistics_accumulating_temp_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_statistics_accumulating_temp_attribute_fn)(
        _In_ sai_object_id_t pon_olt_statistics_accumulating_temp_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT statistics accumulating temp
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
typedef sai_status_t (*sai_bulk_get_pon_olt_statistics_accumulating_temp_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT statistics accumulating PON flooding (COUNTERS_DB)
 */
typedef enum _sai_pon_olt_statistics_accumulating_pon_flooding_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_OLT_NAME = SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_START,

    /**
     * @brief OLT ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_OLT_ID,

    /**
     * @brief Tx BW Best Effort SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_BW_BEST_EFFORT_SLA_UTIL,

    /**
     * @brief Tx BW Best Effort SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_BW_BEST_EFFORT_SLA_BPS,

    /**
     * @brief Tx BW Guaranteed SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_BW_GUARANTEED_SLA_UTIL,

    /**
     * @brief Tx BW Guaranteed SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_BW_GUARANTEED_SLA_BPS,

    /**
     * @brief Tx BW Total SLA Util
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_BW_TOTAL_SLA_UTIL,

    /**
     * @brief Tx BW Total SLA Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_BW_TOTAL_SLA_BPS,

    /**
     * @brief Tx Encrypted Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_ENCRYPTED_OCTETS,

    /**
     * @brief Tx Frames 1024 1518
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_FRAMES_1024_1518,

    /**
     * @brief Tx Frames 128 255
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_FRAMES_128_255,

    /**
     * @brief Tx Frames 1519 Plus
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_FRAMES_1519_PLUS,

    /**
     * @brief Tx Frames 256 511
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_FRAMES_256_511,

    /**
     * @brief Tx Frames 512 1023
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_FRAMES_512_1023,

    /**
     * @brief Tx Frames 64
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_FRAMES_64,

    /**
     * @brief Tx Frames 65 127
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_FRAMES_65_127,

    /**
     * @brief Tx Frames Broadcast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_FRAMES_BROADCAST,

    /**
     * @brief Tx Frames
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_FRAMES,

    /**
     * @brief Tx Frames Multicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_FRAMES_MULTICAST,

    /**
     * @brief Tx Frames Unicast
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_FRAMES_UNICAST,

    /**
     * @brief Tx Multi Broadcast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_MULTI_BROADCAST_OCTETS,

    /**
     * @brief Tx Plain Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_PLAIN_OCTETS,

    /**
     * @brief Tx Rate Bps
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_RATE_BPS,

    /**
     * @brief Tx Total Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_TOTAL_OCTETS,

    /**
     * @brief Tx Unicast Octets
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_TX_UNICAST_OCTETS,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_statistics_accumulating_pon_flooding_attr_t;

/**
 * @brief Get attribute for PON OLT statistics accumulating PON flooding (COUNTERS_DB, read-only)
 *
 * @param[in] pon_olt_statistics_accumulating_pon_flooding_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_statistics_accumulating_pon_flooding_attribute_fn)(
        _In_ sai_object_id_t pon_olt_statistics_accumulating_pon_flooding_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT statistics accumulating PON flooding
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
typedef sai_status_t (*sai_bulk_get_pon_olt_statistics_accumulating_pon_flooding_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Attribute ID for PON OLT statistics accumulating PON flooding NNI network (COUNTERS_DB)
 */
typedef enum _sai_pon_olt_statistics_accumulating_pon_flooding_nni_network_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_NNI_NETWORK_ATTR_START,

    /**
     * @brief OLT Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_NNI_NETWORK_ATTR_OLT_NAME = SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_NNI_NETWORK_ATTR_START,

    /**
     * @brief OLT ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_NNI_NETWORK_ATTR_OLT_ID,

    /**
     * @brief End of attributes
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_NNI_NETWORK_ATTR_END,

    /** Custom range base value */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_NNI_NETWORK_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_NNI_NETWORK_ATTR_CUSTOM_RANGE_END,

} sai_pon_olt_statistics_accumulating_pon_flooding_nni_network_attr_t;

/**
 * @brief Get attribute for PON OLT statistics accumulating PON flooding NNI network (COUNTERS_DB, read-only)
 *
 * @param[in] pon_olt_statistics_accumulating_pon_flooding_nni_network_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pon_olt_statistics_accumulating_pon_flooding_nni_network_attribute_fn)(
        _In_ sai_object_id_t pon_olt_statistics_accumulating_pon_flooding_nni_network_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk get attribute for PON OLT statistics accumulating PON flooding NNI network
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
typedef sai_status_t (*sai_bulk_get_pon_olt_statistics_accumulating_pon_flooding_nni_network_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

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
     * @default empty
     */
    SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_OLT_NAME = SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_START,

    /**
     * @brief Bank ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_BANK_ID,

    /**
     * @brief File
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_PLUG_FW_BANK_FILE_ATTR_FILE,

    /**
     * @brief Version
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
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
     * @default empty
     */
    SAI_PON_OLT_PLUG_ATTR_OLT_NAME = SAI_PON_OLT_PLUG_ATTR_START,

    /**
     * @brief Debug Log Level
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_PLUG_ATTR_DEBUG_LOG_LEVEL,

    /**
     * @brief FW Bank Pointer
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_OLT_PLUG_ATTR_FW_BANK_PTR,

    /**
     * @brief NNI Max Frame Size
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
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
     * @default empty
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
     * @default empty
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
     * @default empty
     */
    SAI_PON_OLT_PLUG_FW_BANK_VERSION_STATE_ATTR_NAME = SAI_PON_OLT_PLUG_FW_BANK_VERSION_STATE_ATTR_START,

    /**
     * @brief Bank ID
     *
     * @type sai_uint8_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_ENV_ATTR_OLT_NAME = SAI_PON_OLT_PLUG_STATISTICS_BINNED_ENV_ATTR_START,

    /**
     * @brief OLT Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_OLT_NAME = SAI_PON_OLT_PLUG_STATISTICS_BINNED_NNI_ATTR_START,

    /**
     * @brief OLT Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_OLT_PLUG_STATISTICS_BINNED_TEMP_ATTR_OLT_NAME = SAI_PON_OLT_PLUG_STATISTICS_BINNED_TEMP_ATTR_START,

    /**
     * @brief OLT Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_ATTR_ONU_NAME = SAI_PON_ONU_ATTR_START,

    /**
     * @brief Device ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_ATTR_DEVICE_ID,

    /**
     * @brief Template Ref
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_ATTR_TEMPLATE_REF,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     */
    SAI_PON_ONU_ATTR_VLAN_ID,

    /**
     * @brief FW Bank Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_ONU_ATTR_FW_BANK_PTR,

    /**
     * @brief Real-time Stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ONU_ATTR_REALTIME_STATS,

    /**
     * @brief Service Config
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_ATTR_SERVICE_CONFIG,

    /**
     * @brief Service Config OMCI Stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ONU_ATTR_SERVICE_CONFIG_OMCI_STATS,

    /**
     * @brief FW Upgrade Backoff Delay
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_BACKOFF_DELAY,

    /**
     * @brief FW Upgrade Backoff Divisor
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_BACKOFF_DIVISOR,

    /**
     * @brief FW Upgrade Download Format
     *
     * @type sai_fw_upgrade_download_format_t
     * @flags CREATE_AND_SET
     * @default SAI_FW_UPGRADE_DOWNLOAD_FORMAT_BASELINE_OMCI
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_DOWNLOAD_FORMAT,

    /**
     * @brief FW Upgrade End Download Timeout
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_END_DOWNLOAD_TIMEOUT,

    /**
     * @brief FW Upgrade Maximum Retries
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_MAXIMUM_RETRIES,

    /**
     * @brief FW Upgrade Maximum Window Size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_ATTR_FW_UPGRADE_MAXIMUM_WINDOW_SIZE,

    /**
     * @brief FW Upgrade Response Timeout
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_ONU_NAME = SAI_PON_ONU_OLT_SERVICE_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Downstream QOS Map
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_DOWNSTREAM_QOS_MAP,

    /**
     * @brief Enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_ENABLE,

    /**
     * @brief Learning Limit
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_LEARNING_LIMIT,

    /**
     * @brief SLA Profile
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_OLT_SERVICE_ATTR_SLA_PROFILE,

    /**
     * @brief TCONT Service Ref
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
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
     * @default empty
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_ONU_NAME = SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Network ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_ATTR_NETWORK_ID,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_ONU_NAME = SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_START,

    /**
     * @brief Configuration Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_CFG_NAME,

    /**
     * @brief Value
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_SERVICE_CONFIG_VALUE_ATTR_VALUE,

    /**
     * @brief Value Type
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_UNI_ATTR_ONU_NAME = SAI_PON_ONU_UNI_ATTR_START,

    /**
     * @brief Port ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_UNI_ATTR_PORT_ID,

    /**
     * @brief Duplex
     *
     * @type sai_duplex_t
     * @flags CREATE_AND_SET
     * @default SAI_DUPLEX_AUTO
     */
    SAI_PON_ONU_UNI_ATTR_DUPLEX,

    /**
     * @brief Enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ONU_UNI_ATTR_ENABLE,

    /**
     * @brief Max Frame Size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_UNI_ATTR_MAX_FRAME_SIZE,

    /**
     * @brief POE
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ONU_UNI_ATTR_POE,

    /**
     * @brief Speed
     *
     * @type sai_speed_t
     * @flags CREATE_AND_SET
     * @default SAI_SPEED_AUTO
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
     * @default empty
     */
    SAI_PON_ONU_FW_BANK_FILE_ATTR_ONU_NAME = SAI_PON_ONU_FW_BANK_FILE_ATTR_START,

    /**
     * @brief Bank ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_FW_BANK_FILE_ATTR_BANK_ID,

    /**
     * @brief File
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_FW_BANK_FILE_ATTR_FILE,

    /**
     * @brief Version
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
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
     * @default empty
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE_ATTR_ONU_NAME = SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE_ATTR_ONU_NAME = SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Priority
     *
     * @type sai_uint8_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_UNI_LEARNED_ADDRESSES_STATE_ATTR_ONU_NAME = SAI_PON_ONU_UNI_LEARNED_ADDRESSES_STATE_ATTR_START,

    /**
     * @brief Port ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
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
     * @default empty
     */
    SAI_PON_ONU_FW_BANK_VERSION_STATE_ATTR_ONU_NAME = SAI_PON_ONU_FW_BANK_VERSION_STATE_ATTR_START,

    /**
     * @brief Bank ID
     *
     * @type sai_uint8_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_ONU_NAME = SAI_PON_ONU_OLT_SERVICE_STATE_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_STATE_ATTR_ONU_NAME = SAI_PON_ONU_OLT_SERVICE_NETWORK_STATE_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_OLT_SERVICE_NETWORK_STATE_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Network ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_UNI_STATE_ATTR_ONU_NAME = SAI_PON_ONU_UNI_STATE_ATTR_START,

    /**
     * @brief Port ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_OMCC_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_OLT_PON_SERVICE_ATTR_ONU_STATS_ID,

    /**
     * @brief Service Port ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ENHANCED_TC_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_ETHERNET_PM3_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_FEC_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GAL_ETHERNET_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_MAC_BRIDGE_PORT_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_OPERATIONAL_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_PON_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_PON_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_PON_ONU_STATISTICS_BINNED_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ONU_STATS_ID,

    /**
     * @brief Me ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan false
     * @default 0
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
     * @default empty
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
     * @default empty
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_OLT_PON_SERVICE_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ENHANCED_TC_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_ETHERNET_PM3_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_FEC_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GAL_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_IP_HOST_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_MAC_BRIDGE_PORT_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_OPERATIONAL_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_TCP_UDP_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_ACCUMULATING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_ENHANCED_TC_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_DOWNSTREAM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_FRAME_PM_UPSTREAM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_FEC_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_GAL_ETHERNET_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_GEM_PORT_NETWORK_CTP_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_RS232_RS485_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_DOWNSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_ONU_NAME = SAI_PON_ONU_STATISTICS_STREAMING_ONU_XG_PON_UPSTREAM_MGMT_PM_ATTR_START,

    /**
     * @brief ONU ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_ONU_TEMPLATE_NAME = SAI_PON_ONU_TEMPLATE_ONU_ATTR_START,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_VLAN_ID,

    /**
     * @brief FW Bank Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_BANK_PTR,

    /**
     * @brief Service Config
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_SERVICE_CONFIG,

    /**
     * @brief Service Config OMCI Stats
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_SERVICE_CONFIG_OMCI_STATS,

    /**
     * @brief FW Upgrade Backoff Delay
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_BACKOFF_DELAY,

    /**
     * @brief FW Upgrade Backoff Divisor
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_BACKOFF_DIVISOR,

    /**
     * @brief FW Upgrade Download Format
     *
     * @type sai_fw_upgrade_download_format_t
     * @flags CREATE_AND_SET
     * @default SAI_FW_UPGRADE_DOWNLOAD_FORMAT_BASELINE_OMCI
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_DOWNLOAD_FORMAT,

    /**
     * @brief FW Upgrade End Download Timeout
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_END_DOWNLOAD_TIMEOUT,

    /**
     * @brief FW Upgrade Maximum Retries
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_MAXIMUM_RETRIES,

    /**
     * @brief FW Upgrade Maximum Window Size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_ONU_ATTR_FW_UPGRADE_MAXIMUM_WINDOW_SIZE,

    /**
     * @brief FW Upgrade Response Timeout
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_ONU_TEMPLATE_NAME = SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_START,

    /**
     * @brief Bank ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_BANK_ID,

    /**
     * @brief File
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_ONU_FW_BANK_FILE_ATTR_FILE,

    /**
     * @brief Version
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
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
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_ONU_TEMPLATE_NAME = SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Downstream QOS Map
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_DOWNSTREAM_QOS_MAP,

    /**
     * @brief Enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_ENABLE,

    /**
     * @brief Learning Limit
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_LEARNING_LIMIT,

    /**
     * @brief SLA Profile
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_ATTR_SLA_PROFILE,

    /**
     * @brief TCONT Service Ref
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
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
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_ONU_TEMPLATE_NAME = SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_START,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Network ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK_ATTR_NETWORK_ID,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_ONU_TEMPLATE_NAME = SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_START,

    /**
     * @brief Configuration Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_CFG_NAME,

    /**
     * @brief Value
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE_ATTR_VALUE,

    /**
     * @brief Value Type
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_ONU_TEMPLATE_NAME = SAI_PON_ONU_TEMPLATE_UNI_ATTR_START,

    /**
     * @brief Port ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_PORT_ID,

    /**
     * @brief Duplex
     *
     * @type sai_duplex_t
     * @flags CREATE_AND_SET
     * @default SAI_DUPLEX_AUTO
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_DUPLEX,

    /**
     * @brief Enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_ENABLE,

    /**
     * @brief Max Frame Size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_MAX_FRAME_SIZE,

    /**
     * @brief POE
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ONU_TEMPLATE_UNI_ATTR_POE,

    /**
     * @brief Speed
     *
     * @type sai_speed_t
     * @flags CREATE_AND_SET
     * @default SAI_SPEED_AUTO
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_ATTR_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_ATTR_START,

    /**
     * @brief Title
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_ATTR_TITLE,

    /**
     * @brief Version
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL_ATTR_VENDOR_ID = SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL_ATTR_START,

    /**
     * @brief Name
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL_ATTR_NAME,

    /**
     * @brief Model
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT_ATTR_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT_ATTR_START,

    /**
     * @brief Db Ref
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT_ATTR_DB_REF,

    /**
     * @brief Type
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Gem Block Length
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_GEM_BLOCK_LENGTH,

    /**
     * @brief Sf Threshold
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_SF_THRESHOLD,

    /**
     * @brief Sd Threshold
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_SD_THRESHOLD,

    /**
     * @brief Arc
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_ARC,

    /**
     * @brief Arc Interval
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_ARC_INTERVAL,

    /**
     * @brief Lower Optical Threshold
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_LOWER_OPTICAL_THRESHOLD,

    /**
     * @brief Upper Optical Threshold
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_UPPER_OPTICAL_THRESHOLD,

    /**
     * @brief Lower Transmit Power Threshold
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ANI_G_ATTR_LOWER_TRANSMIT_POWER_THRESHOLD,

    /**
     * @brief Upper Transmit Power Threshold
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Expected Plugin Unit Type
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_EXPECTED_PLUGIN_UNIT_TYPE,

    /**
     * @brief Expected Port Count
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_EXPECTED_PORT_COUNT,

    /**
     * @brief Expected Equipment ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_EXPECTED_EQUIPMENT_ID,

    /**
     * @brief Invoke Protection Switch
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_INVOKE_PROTECTION_SWITCH,

    /**
     * @brief Arc
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CARDHOLDER_ATTR_ARC,

    /**
     * @brief Arc Interval
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Administrative State
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_ADMINISTRATIVE_STATE,

    /**
     * @brief Bridged Or IP Ind
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_BRIDGED_OR_IP_IND,

    /**
     * @brief Card Configuration
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_CIRCUIT_PACK_ATTR_CARD_CONFIGURATION,

    /**
     * @brief Power Sched Override
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_FEC_PM_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_FEC_PM_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_FEC_PM_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 64 Bit ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_TC_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_TC_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ENHANCED_TC_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 64 Bit ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_THRESHOLD_DATA_ID,

    /**
     * @brief Parent Me Class
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_PARENT_ME_CLASS,

    /**
     * @brief Parent Me Instance
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_PARENT_ME_INSTANCE,

    /**
     * @brief Accumulation Disable
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_ACCUMULATION_DISABLE,

    /**
     * @brief Tca Disable
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_TCA_DISABLE,

    /**
     * @brief Control Fields
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_CONTROL_FIELDS,

    /**
     * @brief Tci
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM_ATTR_TCI,

    /**
     * @brief Reserved
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_THRESHOLD_DATA_ID,

    /**
     * @brief Parent Me Class
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_PARENT_ME_CLASS,

    /**
     * @brief Parent Me Instance
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_PARENT_ME_INSTANCE,

    /**
     * @brief Accumulation Disable
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_ACCUMULATION_DISABLE,

    /**
     * @brief Tca Disable
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_TCA_DISABLE,

    /**
     * @brief Control Fields
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_CONTROL_FIELDS,

    /**
     * @brief Tci
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_EXTENDED_PM64_BIT_ATTR_TCI,

    /**
     * @brief Reserved
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_DOWNSTREAM_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_DOWNSTREAM_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_DOWNSTREAM_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_UPSTREAM_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_UPSTREAM_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_FRAME_PERF_MON_HIST_DATA_UPSTREAM_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA3_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA3_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ETHERNET_PERF_MON_HIST_DATA3_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Association Type
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_ASSOCIATION_TYPE,

    /**
     * @brief Input TPID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_INPUT_TPID,

    /**
     * @brief Output TPID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_OUTPUT_TPID,

    /**
     * @brief Downstream Mode
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_ATTR_DOWNSTREAM_MODE,

    /**
     * @brief Associated Me Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_ID,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Received Frame Vlan Tagging Operation Table Entry ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_TABLE_ENTRY_ID,

    /**
     * @brief Filter Outer Priority
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_OUTER_PRIORITY,

    /**
     * @brief Filter Outer VID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_OUTER_VID,

    /**
     * @brief Filter Outer TPID De
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_OUTER_TPID_DE,

    /**
     * @brief Pad1
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_PAD1,

    /**
     * @brief Filter Inner Priority
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_INNER_PRIORITY,

    /**
     * @brief Filter Inner VID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_INNER_VID,

    /**
     * @brief Filter Inner TPID De
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_INNER_TPID_DE,

    /**
     * @brief Pad2
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_PAD2,

    /**
     * @brief Filter Ether Type
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_FILTER_ETHER_TYPE,

    /**
     * @brief Treatment Tags To Remove
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_TAGS_TO_REMOVE,

    /**
     * @brief Pad3
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_PAD3,

    /**
     * @brief Treatment Outer Priority
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_OUTER_PRIORITY,

    /**
     * @brief Treatment Outer VID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_OUTER_VID,

    /**
     * @brief Treatment Outer TPID De
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_OUTER_TPID_DE,

    /**
     * @brief Pad4
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_PAD4,

    /**
     * @brief Treatment Inner Priority
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_INNER_PRIORITY,

    /**
     * @brief Treatment Inner VID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION_ATTR_TREATMENT_INNER_VID,

    /**
     * @brief Treatment Inner TPID De
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_ID,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief DSCP
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING_ATTR_DSCP,

    /**
     * @brief Priority
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_FEC_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_FEC_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_FEC_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PROFILE_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PROFILE_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GAL_ETHERNET_PROFILE_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Max Gem Payload Size
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Gem Port Network CTP Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_GEM_PORT_NETWORK_CTP_POINTER,

    /**
     * @brief Interworking Option
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_INTERWORKING_OPTION,

    /**
     * @brief Service Profile Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_SERVICE_PROFILE_POINTER,

    /**
     * @brief Interworking Tp Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_INTERWORKING_TP_POINTER,

    /**
     * @brief Gal Profile Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_INTERWORKING_TP_ATTR_GAL_PROFILE_POINTER,

    /**
     * @brief Gal Loopback Configuration
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Port ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_PORT_ID,

    /**
     * @brief TCONT Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_TCONT_POINTER,

    /**
     * @brief Direction
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_DIRECTION,

    /**
     * @brief Traffic Management Pointer Upstream
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_TRAFFIC_MANAGEMENT_POINTER_UPSTREAM,

    /**
     * @brief Traffic Descriptor Profile Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_TRAFFIC_DESCRIPTOR_PROFILE_POINTER,

    /**
     * @brief Priority Queue Pointer Downstream
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_PRIORITY_QUEUE_POINTER_DOWNSTREAM,

    /**
     * @brief Traffic Desc Profile Pointer Downstream
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_ATTR_TRAFFIC_DESC_PROFILE_POINTER_DOWNSTREAM,

    /**
     * @brief Encryption Key Ring
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_GEM_PORT_NETWORK_CTP_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Tp Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_TP_POINTER,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 0
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_0,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 1
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_1,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 2
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_2,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 3
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_3,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 4
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_4,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 5
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_5,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 6
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_6,

    /**
     * @brief Interwork Tp Pointer For P Bit Priority 7
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_INTERWORK_TP_POINTER_FOR_P_BIT_PRIORITY_7,

    /**
     * @brief Unmarked Frame Option
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_UNMARKED_FRAME_OPTION,

    /**
     * @brief Default P Bit Assumption
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_ATTR_DEFAULT_P_BIT_ASSUMPTION,

    /**
     * @brief Tp Type
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_ID,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief DSCP
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING_ATTR_DSCP,

    /**
     * @brief Priority
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief IP Options
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_IP_OPTIONS,

    /**
     * @brief ONU Identifier
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_ONU_IDENTIFIER,

    /**
     * @brief IP Address
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_IP_ADDRESS,

    /**
     * @brief Mask
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_MASK,

    /**
     * @brief Gateway
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_GATEWAY,

    /**
     * @brief Primary DNS
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_PRIMARY_DNS,

    /**
     * @brief Secondary DNS
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_CONFIG_DATA_ATTR_SECONDARY_DNS,

    /**
     * @brief Relay Agent Options
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IP_HOST_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief IP Options
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_IP_OPTIONS,

    /**
     * @brief ONU Identifier
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_ONU_IDENTIFIER,

    /**
     * @brief IPV6 Address
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_IPV6_ADDRESS,

    /**
     * @brief Default Router
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_DEFAULT_ROUTER,

    /**
     * @brief Primary DNS
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_PRIMARY_DNS,

    /**
     * @brief Secondary DNS
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_SECONDARY_DNS,

    /**
     * @brief On Link Prefix
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_IPV6_HOST_CONFIG_DATA_ATTR_ON_LINK_PREFIX,

    /**
     * @brief Relay Agent Options
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Number Of Parts
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_NUMBER_OF_PARTS,

    /**
     * @brief Part 1
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_1,

    /**
     * @brief Part 2
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_2,

    /**
     * @brief Part 3
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_3,

    /**
     * @brief Part 4
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_4,

    /**
     * @brief Part 5
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_5,

    /**
     * @brief Part 6
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_6,

    /**
     * @brief Part 7
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_7,

    /**
     * @brief Part 8
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_8,

    /**
     * @brief Part 9
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_9,

    /**
     * @brief Part 10
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_10,

    /**
     * @brief Part 11
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_11,

    /**
     * @brief Part 12
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_12,

    /**
     * @brief Part 13
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_13,

    /**
     * @brief Part 14
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_LARGE_STRING_ATTR_PART_14,

    /**
     * @brief Part 15
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Bridge ID Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_BRIDGE_ID_POINTER,

    /**
     * @brief Port Num
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_PORT_NUM,

    /**
     * @brief Tp Type
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_TP_TYPE,

    /**
     * @brief Tp Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_TP_POINTER,

    /**
     * @brief Port Priority
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_PORT_PRIORITY,

    /**
     * @brief Port Path Cost
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_PORT_PATH_COST,

    /**
     * @brief Port Spanning Tree Ind
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_PORT_SPANNING_TREE_IND,

    /**
     * @brief Deprecated1
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_DEPRECATED1,

    /**
     * @brief Deprecated2
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_DEPRECATED2,

    /**
     * @brief Outbound Td Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_OUTBOUND_TD_POINTER,

    /**
     * @brief Inbound Td Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_CONFIG_DATA_ATTR_INBOUND_TD_POINTER,

    /**
     * @brief MAC Learning Depth
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_PORT_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Spanning Tree Ind
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_SPANNING_TREE_IND,

    /**
     * @brief Learning Ind
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_LEARNING_IND,

    /**
     * @brief Port Bridging Ind
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_PORT_BRIDGING_IND,

    /**
     * @brief Priority
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_PRIORITY,

    /**
     * @brief Max Age
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_MAX_AGE,

    /**
     * @brief Hello Time
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_HELLO_TIME,

    /**
     * @brief Forward Delay
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_FORWARD_DELAY,

    /**
     * @brief Unknown MAC Address Discard
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_UNKNOWN_MAC_ADDRESS_DISCARD,

    /**
     * @brief MAC Learning Depth
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MAC_BRIDGE_SERVICE_PROFILE_ATTR_MAC_LEARNING_DEPTH,

    /**
     * @brief Dynamic Filtering Ageing Time
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Gem Port Network CTP Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_GEM_PORT_NETWORK_CTP_POINTER,

    /**
     * @brief Interworking Option
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_INTERWORKING_OPTION,

    /**
     * @brief Service Profile Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_SERVICE_PROFILE_POINTER,

    /**
     * @brief Interworking Tp Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_INTERWORKING_TP_POINTER,

    /**
     * @brief Gal Profile Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_ATTR_GAL_PROFILE_POINTER,

    /**
     * @brief Gal Loopback Configuration
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_ID,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief IPV4 Multicast Address Table Entry ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_IPV4_MULTICAST_ADDRESS_TABLE_ENTRY_ID,

    /**
     * @brief Gem Port ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_GEM_PORT_ID,

    /**
     * @brief Secondary Key
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_SECONDARY_KEY,

    /**
     * @brief IP Multicast Da Range Start
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS_ATTR_IP_MULTICAST_DA_RANGE_BEGIN,

    /**
     * @brief IP Multicast Da Range Stop
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_ID,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief IPV6 Multicast Address Table Entry ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_IPV6_MULTICAST_ADDRESS_TABLE_ENTRY_ID,

    /**
     * @brief Gem Port ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_GEM_PORT_ID,

    /**
     * @brief Secondary Key
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_SECONDARY_KEY,

    /**
     * @brief Lsb IP Multicast Da Range Start
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_LSB_IP_MULTICAST_DA_RANGE_BEGIN,

    /**
     * @brief Lsb IP Multicast Da Range Stop
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS_ATTR_LSB_IP_MULTICAST_DA_RANGE_STOP,

    /**
     * @brief Msb IP Multicast Da
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief OLT Vendor ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_OLT_VENDOR_ID,

    /**
     * @brief Equipment ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_EQUIPMENT_ID,

    /**
     * @brief Version
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_OLT_G_ATTR_VERSION,

    /**
     * @brief Time Of Day
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Security Mode
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_SECURITY_MODE,

    /**
     * @brief Current Connectivity Mode
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU2_G_ATTR_CURRENT_CONNECTIVITY_MODE,

    /**
     * @brief Priority Queue Scale Factor
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Battery Backup
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_BATTERY_BACKUP,

    /**
     * @brief Administrative State
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_G_ATTR_ADMINISTRATIVE_STATE,

    /**
     * @brief Credentials Status
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_OPERATIONAL_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_ONU_OPERATIONAL_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_ONU_OPERATIONAL_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Administrative State
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_ADMINISTRATIVE_STATE,

    /**
     * @brief Port Mode
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_PORT_MODE,

    /**
     * @brief Baud Rate
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_BAUD_RATE,

    /**
     * @brief Data Bits
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_DATA_BITS,

    /**
     * @brief Parity
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_PARITY,

    /**
     * @brief Stop Bits
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PPTP_RS232_RS485_UNI_ATTR_STOP_BITS,

    /**
     * @brief Flow Control
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Allocated Queue Size
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_ALLOCATED_QUEUE_SIZE,

    /**
     * @brief Discard Block Counter Reset Interval
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_DISCARD_BLOCK_COUNTER_RESET_INTERVAL,

    /**
     * @brief Threshold Value For Discarded Blocks
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_THRESHOLD_VALUE_FOR_DISCARDED_BLOCKS,

    /**
     * @brief Related Port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_RELATED_PORT,

    /**
     * @brief Traffic Scheduler Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_TRAFFIC_SCHEDULER_POINTER,

    /**
     * @brief Weight
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_WEIGHT,

    /**
     * @brief Back Pressure Operation
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_BACK_PRESSURE_OPERATION,

    /**
     * @brief Back Pressure Time
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_BACK_PRESSURE_TIME,

    /**
     * @brief Back Pressure Occur Queue Threshold
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_BACK_PRESSURE_OCCUR_QUEUE_THRESHOLD,

    /**
     * @brief Back Pressure Clear Queue Threshold
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_BACK_PRESSURE_CLEAR_QUEUE_THRESHOLD,

    /**
     * @brief Packet Drop Max P
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_PACKET_DROP_MAX_P,

    /**
     * @brief Queue Drop W Q
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_ATTR_QUEUE_DROP_W_Q,

    /**
     * @brief Drop Precedence Colour Marking
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Min Green
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_MIN_GREEN,

    /**
     * @brief Max Green
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_MAX_GREEN,

    /**
     * @brief Min Yellow
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD_ATTR_MIN_YELLOW,

    /**
     * @brief Max Yellow
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief TCP UDP Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_RS232_RS485_PORT_OPER_CONFIG_DATA_ATTR_TCP_UDP_PTR,

    /**
     * @brief Pptp Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_OPERATION_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_OPERATION_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_OPERATION_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Server Action
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief TCP UDP Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_SSH_SERVER_PORT_DATA_ATTR_TCP_UDP_PTR,

    /**
     * @brief SSH Server Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Allocation ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCONT_ATTR_ALLOC_ID,

    /**
     * @brief Policy
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Port ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_PORT_ID,

    /**
     * @brief Protocol
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_PROTOCOL,

    /**
     * @brief TOS Diffserv Field
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_CONFIG_DATA_ATTR_TOS_DIFFSERV_FIELD,

    /**
     * @brief IP Host Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TCP_UDP_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Value 1
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_1,

    /**
     * @brief Threshold Value 2
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_2,

    /**
     * @brief Threshold Value 3
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_3,

    /**
     * @brief Threshold Value 4
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_4,

    /**
     * @brief Threshold Value 5
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_5,

    /**
     * @brief Threshold Value 6
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_6,

    /**
     * @brief Threshold Value 7
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_7,

    /**
     * @brief Threshold Value 8
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_8,

    /**
     * @brief Threshold Value 9
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_9,

    /**
     * @brief Threshold Value 10
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_10,

    /**
     * @brief Threshold Value 11
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_11,

    /**
     * @brief Threshold Value 12
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_12,

    /**
     * @brief Threshold Value 13
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA64_BIT_ATTR_THRESHOLD_VALUE_13,

    /**
     * @brief Threshold Value 14
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Value 1
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_1,

    /**
     * @brief Threshold Value 2
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_2,

    /**
     * @brief Threshold Value 3
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_3,

    /**
     * @brief Threshold Value 4
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_4,

    /**
     * @brief Threshold Value 5
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_5,

    /**
     * @brief Threshold Value 6
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_ONE_ATTR_THRESHOLD_VALUE_6,

    /**
     * @brief Threshold Value 7
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Value 8
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_8,

    /**
     * @brief Threshold Value 9
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_9,

    /**
     * @brief Threshold Value 10
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_10,

    /**
     * @brief Threshold Value 11
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_11,

    /**
     * @brief Threshold Value 12
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_12,

    /**
     * @brief Threshold Value 13
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_THRESHOLD_DATA_TWO_ATTR_THRESHOLD_VALUE_13,

    /**
     * @brief Threshold Value 14
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief CIR
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_CIR,

    /**
     * @brief PIR
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_PIR,

    /**
     * @brief CBS
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_CBS,

    /**
     * @brief PBS
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_PBS,

    /**
     * @brief Colour Mode
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_COLOUR_MODE,

    /**
     * @brief Ingress Colour Marking
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_DESCRIPTOR_ATTR_INGRESS_COLOUR_MARKING,

    /**
     * @brief Egress Colour Marking
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief TCONT Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_TCONT_POINTER,

    /**
     * @brief Policy
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_TRAFFIC_SCHEDULER_ATTR_POLICY,

    /**
     * @brief Priority Weight
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Administrative State
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_ADMINISTRATIVE_STATE,

    /**
     * @brief Interdomain Service Config Profile Name
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VIRTUAL_ETHERNET_INTERFACE_PT_ATTR_INTERDOMAIN_SERVICE_CONFIG_PROFILE_NAME,

    /**
     * @brief TCP UDP Pointer
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Forward Operation
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_ATTR_FORWARD_OPERATION,

    /**
     * @brief Number Of Entries
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_ID,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Vlan Filter Entry ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_VLAN_TAGGING_FILTER_DATA_VLAN_FILTER_ATTR_VLAN_FILTER_ENTRY_ID,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_DOWNSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_DOWNSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_DOWNSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_TC_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_TC_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_TC_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_UPSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_SERVICE_CONFIG_PROFILE_NAME = SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_UPSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_START,

    /**
     * @brief Managed Entity ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_PON_SERVICE_CONFIG_PROFILE_XG_PON_UPSTREAM_MGMT_PERF_MON_HIST_DATA_ATTR_MANAGED_ENTITY_ID,

    /**
     * @brief Threshold Data 1 2 ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
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
     * @default empty
     */
    SAI_PON_SLA_PROFILE_ATTR_SLA_PROFILE_NAME = SAI_PON_SLA_PROFILE_ATTR_START,

    /**
     * @brief Downstream Guaranteed Rate
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SLA_PROFILE_ATTR_DOWNSTREAM_GUARANTEED_RATE,

    /**
     * @brief Downstream Guaranteed Maximum Burst
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SLA_PROFILE_ATTR_DOWNSTREAM_GUARANTEED_MAXIMUM_BURST,

    /**
     * @brief Downstream Best Effort Rate
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SLA_PROFILE_ATTR_DOWNSTREAM_BEST_EFFORT_RATE,

    /**
     * @brief Downstream Best Effort Maximum Burst
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SLA_PROFILE_ATTR_DOWNSTREAM_BEST_EFFORT_MAXIMUM_BURST,

    /**
     * @brief Upstream Fixed Rate
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_FIXED_RATE,

    /**
     * @brief Upstream Guaranteed Rate
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_GUARANTEED_RATE,

    /**
     * @brief Upstream Guaranteed Maximum Burst
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_GUARANTEED_MAXIMUM_BURST,

    /**
     * @brief Upstream Priority
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_PRIORITY,

    /**
     * @brief Upstream Best Effort Rate
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_BEST_EFFORT_RATE,

    /**
     * @brief Upstream Best Effort Maximum Burst
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_SLA_PROFILE_ATTR_UPSTREAM_BEST_EFFORT_MAXIMUM_BURST,

    /**
     * @brief Upstream Best Effort Priority
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
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
     * @default empty
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

/**
 * @brief PON action types for one-shot RPC operations
 */
typedef enum _sai_pon_action_type_t
{
    SAI_PON_ACTION_TYPE_OLT_RESET,

    SAI_PON_ACTION_TYPE_OLT_CLEAR,

    SAI_PON_ACTION_TYPE_OLT_DISABLE_ONU,

    SAI_PON_ACTION_TYPE_OLT_ALLOW_ONU_REGISTRATION,

    SAI_PON_ACTION_TYPE_OLT_PROTECTION_ARM,

    SAI_PON_ACTION_TYPE_OLT_PROTECTION_SWITCH,

    SAI_PON_ACTION_TYPE_ONU_RESET,

    SAI_PON_ACTION_TYPE_ONU_CLEAR,

    SAI_PON_ACTION_TYPE_ONU_CLEAR_ABORTED_UPGRADE,

    SAI_PON_ACTION_TYPE_CONTROLLER_CLEAR,

    SAI_PON_ACTION_TYPE_CONTROLLER_SET_STATUS,

    SAI_PON_ACTION_TYPE_DB_MANAGER_RESTART,

    SAI_PON_ACTION_TYPE_DB_MANAGER_SERVER_STATS_CLR,

    SAI_PON_ACTION_TYPE_DB_MANAGER_CLIENT_STATS_CLR,

} sai_pon_action_type_t;

/**
 * @brief Attribute IDs for PON action objects
 */
typedef enum _sai_pon_action_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ACTION_ATTR_START,

    /**
     * @brief Target name (OLT or ONU interface name)
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ACTION_ATTR_NAME = SAI_PON_ACTION_ATTR_START,

    /**
     * @brief OLT name for ONU-targeted actions
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ACTION_ATTR_OLT_NAME,

    /**
     * @brief ONU ID for ONU-specific actions
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ACTION_ATTR_ONU_ID,

    /**
     * @brief Device state for clear actions
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ACTION_ATTR_DEVICE_STATE,

    /**
     * @brief Disable flag for OLT-disable-ONU action
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ACTION_ATTR_DISABLE,

    /**
     * @brief Arm flag for OLT-protection-arm action
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ACTION_ATTR_ARM,

    /**
     * @brief Force switch flag for OLT-protection-arm action
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ACTION_ATTR_FORCE_SWITCH,

    /**
     * @brief Admin status for controller-set-status action
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PON_ACTION_ATTR_ADMIN_STATUS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ACTION_ATTR_END,

    /** Custom range base value */
    SAI_PON_ACTION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ACTION_ATTR_CUSTOM_RANGE_END

} sai_pon_action_attr_t;

/**
 * @brief Execute a PON action (one-shot RPC)
 *
 * @param[in] switch_id Switch ID
 * @param[in] action_type Type of PON action to execute
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_execute_pon_action_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ sai_pon_action_type_t action_type,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

typedef struct _sai_pon_api_t
{
    sai_create_pon_controller_fn                                                                                                      create_pon_controller;
    sai_remove_pon_controller_fn                                                                                                      remove_pon_controller;
    sai_set_pon_controller_attribute_fn                                                                                               set_pon_controller_attribute;
    sai_get_pon_controller_attribute_fn                                                                                               get_pon_controller_attribute;
    sai_create_pon_downstream_qos_map_map_fn                                                                                          create_pon_downstream_qos_map_map;
    sai_remove_pon_downstream_qos_map_map_fn                                                                                          remove_pon_downstream_qos_map_map;
    sai_set_pon_downstream_qos_map_map_attribute_fn                                                                                   set_pon_downstream_qos_map_map_attribute;
    sai_get_pon_downstream_qos_map_map_attribute_fn                                                                                   get_pon_downstream_qos_map_map_attribute;
    sai_get_pon_controller_system_status_olt_attribute_fn                                                                             get_pon_controller_system_status_olt_attribute;
    sai_get_pon_controller_system_status_olt_onu_attribute_fn                                                                         get_pon_controller_system_status_olt_onu_attribute;
    sai_get_pon_controller_state_attribute_fn                                                                                         get_pon_controller_state_attribute;
    sai_get_pon_controller_onu_fw_upgrade_state_attribute_fn                                                                          get_pon_controller_onu_fw_upgrade_state_attribute;
    sai_bulk_get_pon_controller_system_status_olt_attribute_fn                                                                        get_pon_controller_system_status_olts_attribute;
    sai_bulk_get_pon_controller_system_status_olt_onu_attribute_fn                                                                    get_pon_controller_system_status_olt_onus_attribute;
    sai_bulk_get_pon_controller_state_attribute_fn                                                                                    get_pon_controller_states_attribute;
    sai_bulk_get_pon_controller_onu_fw_upgrade_state_attribute_fn                                                                     get_pon_controller_onu_fw_upgrade_states_attribute;
    sai_create_pon_olt_intf_fn                                                                                                        create_pon_olt_intf;
    sai_remove_pon_olt_intf_fn                                                                                                        remove_pon_olt_intf;
    sai_set_pon_olt_intf_attribute_fn                                                                                                 set_pon_olt_intf_attribute;
    sai_get_pon_olt_intf_attribute_fn                                                                                                 get_pon_olt_intf_attribute;
    sai_create_pon_olt_intf_network_fn                                                                                                create_pon_olt_intf_network;
    sai_remove_pon_olt_intf_network_fn                                                                                                remove_pon_olt_intf_network;
    sai_set_pon_olt_intf_network_attribute_fn                                                                                         set_pon_olt_intf_network_attribute;
    sai_get_pon_olt_intf_network_attribute_fn                                                                                         get_pon_olt_intf_network_attribute;
    sai_create_pon_olt_intf_onu_fn                                                                                                    create_pon_olt_intf_onu;
    sai_remove_pon_olt_intf_onu_fn                                                                                                    remove_pon_olt_intf_onu;
    sai_set_pon_olt_intf_onu_attribute_fn                                                                                             set_pon_olt_intf_onu_attribute;
    sai_get_pon_olt_intf_onu_attribute_fn                                                                                             get_pon_olt_intf_onu_attribute;
    sai_create_pon_olt_intf_onu_olt_service_fn                                                                                        create_pon_olt_intf_onu_olt_service;
    sai_remove_pon_olt_intf_onu_olt_service_fn                                                                                        remove_pon_olt_intf_onu_olt_service;
    sai_set_pon_olt_intf_onu_olt_service_attribute_fn                                                                                 set_pon_olt_intf_onu_olt_service_attribute;
    sai_get_pon_olt_intf_onu_olt_service_attribute_fn                                                                                 get_pon_olt_intf_onu_olt_service_attribute;
    sai_create_pon_olt_intf_onu_service_tcont_fn                                                                                      create_pon_olt_intf_onu_service_tcont;
    sai_remove_pon_olt_intf_onu_service_tcont_fn                                                                                      remove_pon_olt_intf_onu_service_tcont;
    sai_set_pon_olt_intf_onu_service_tcont_attribute_fn                                                                               set_pon_olt_intf_onu_service_tcont_attribute;
    sai_get_pon_olt_intf_onu_service_tcont_attribute_fn                                                                               get_pon_olt_intf_onu_service_tcont_attribute;
    sai_create_pon_olt_intf_onu_service_gemport_fn                                                                                    create_pon_olt_intf_onu_service_gemport;
    sai_remove_pon_olt_intf_onu_service_gemport_fn                                                                                    remove_pon_olt_intf_onu_service_gemport;
    sai_set_pon_olt_intf_onu_service_gemport_attribute_fn                                                                             set_pon_olt_intf_onu_service_gemport_attribute;
    sai_get_pon_olt_intf_onu_service_gemport_attribute_fn                                                                             get_pon_olt_intf_onu_service_gemport_attribute;
    sai_create_pon_olt_protection_group_fn                                                                                            create_pon_olt_protection_group;
    sai_remove_pon_olt_protection_group_fn                                                                                            remove_pon_olt_protection_group;
    sai_set_pon_olt_protection_group_attribute_fn                                                                                     set_pon_olt_protection_group_attribute;
    sai_get_pon_olt_protection_group_attribute_fn                                                                                     get_pon_olt_protection_group_attribute;
    sai_get_pon_olt_intf_onu_operational_state_attribute_fn                                                                           get_pon_olt_intf_onu_operational_state_attribute;
    sai_get_pon_olt_intf_nni_network_learning_table_state_attribute_fn                                                                get_pon_olt_intf_nni_network_learning_table_state_attribute;
    sai_get_pon_firmware_filename_state_attribute_fn                                                                                  get_pon_firmware_filename_state_attribute;
    sai_get_pon_olt_intf_state_attribute_fn                                                                                           get_pon_olt_intf_state_attribute;
    sai_get_pon_olt_intf_network_state_attribute_fn                                                                                   get_pon_olt_intf_network_state_attribute;
    sai_get_pon_olt_intf_onu_state_attribute_fn                                                                                       get_pon_olt_intf_onu_state_attribute;
    sai_get_pon_olt_intf_onu_fw_upgrade_state_attribute_fn                                                                            get_pon_olt_intf_onu_fw_upgrade_state_attribute;
    sai_get_pon_olt_intf_onu_service_tcont_state_attribute_fn                                                                         get_pon_olt_intf_onu_service_tcont_state_attribute;
    sai_get_pon_olt_intf_onu_service_gemport_state_attribute_fn                                                                       get_pon_olt_intf_onu_service_gemport_state_attribute;
    sai_get_pon_olt_protection_group_state_attribute_fn                                                                               get_pon_olt_protection_group_state_attribute;
    sai_get_pon_olt_intf_statistics_binned_attribute_fn                                                                               get_pon_olt_intf_statistics_binned_attribute;
    sai_get_pon_olt_statistics_binned_pon_flooding_attribute_fn                                                                       get_pon_olt_statistics_binned_pon_flooding_attribute;
    sai_get_pon_olt_statistics_binned_pon_flooding_network_attribute_fn                                                               get_pon_olt_statistics_binned_pon_flooding_network_attribute;
    sai_get_pon_olt_statistics_accumulating_attribute_fn                                                                              get_pon_olt_statistics_accumulating_attribute;
    sai_get_pon_olt_statistics_accumulating_env_attribute_fn                                                                          get_pon_olt_statistics_accumulating_env_attribute;
    sai_get_pon_olt_statistics_accumulating_temp_attribute_fn                                                                         get_pon_olt_statistics_accumulating_temp_attribute;
    sai_get_pon_olt_statistics_accumulating_pon_flooding_attribute_fn                                                                 get_pon_olt_statistics_accumulating_pon_flooding_attribute;
    sai_get_pon_olt_statistics_accumulating_pon_flooding_nni_network_attribute_fn                                                     get_pon_olt_statistics_accumulating_pon_flooding_nni_network_attribute;
    sai_bulk_get_pon_olt_intf_onu_operational_state_attribute_fn                                                                      get_pon_olt_intf_onu_operational_states_attribute;
    sai_bulk_get_pon_olt_intf_nni_network_learning_table_state_attribute_fn                                                           get_pon_olt_intf_nni_network_learning_table_states_attribute;
    sai_bulk_get_pon_firmware_filename_state_attribute_fn                                                                             get_pon_firmware_filename_states_attribute;
    sai_bulk_get_pon_olt_intf_state_attribute_fn                                                                                      get_pon_olt_intf_states_attribute;
    sai_bulk_get_pon_olt_intf_network_state_attribute_fn                                                                              get_pon_olt_intf_network_states_attribute;
    sai_bulk_get_pon_olt_intf_onu_state_attribute_fn                                                                                  get_pon_olt_intf_onu_states_attribute;
    sai_bulk_get_pon_olt_intf_onu_fw_upgrade_state_attribute_fn                                                                       get_pon_olt_intf_onu_fw_upgrade_states_attribute;
    sai_bulk_get_pon_olt_intf_onu_service_tcont_state_attribute_fn                                                                    get_pon_olt_intf_onu_service_tcont_states_attribute;
    sai_bulk_get_pon_olt_intf_onu_service_gemport_state_attribute_fn                                                                  get_pon_olt_intf_onu_service_gemport_states_attribute;
    sai_bulk_get_pon_olt_protection_group_state_attribute_fn                                                                          get_pon_olt_protection_group_states_attribute;
    sai_bulk_get_pon_olt_intf_statistics_binned_attribute_fn                                                                          get_pon_olt_intf_statistics_binneds_attribute;
    sai_bulk_get_pon_olt_statistics_binned_pon_flooding_attribute_fn                                                                  get_pon_olt_statistics_binned_pon_floodings_attribute;
    sai_bulk_get_pon_olt_statistics_binned_pon_flooding_network_attribute_fn                                                          get_pon_olt_statistics_binned_pon_flooding_networks_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_attribute_fn                                                                         get_pon_olt_statistics_accumulatings_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_env_attribute_fn                                                                     get_pon_olt_statistics_accumulating_envs_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_temp_attribute_fn                                                                    get_pon_olt_statistics_accumulating_temps_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_pon_flooding_attribute_fn                                                            get_pon_olt_statistics_accumulating_pon_floodings_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_pon_flooding_nni_network_attribute_fn                                                get_pon_olt_statistics_accumulating_pon_flooding_nni_networks_attribute;
    sai_create_pon_olt_plug_fw_bank_file_fn                                                                                           create_pon_olt_plug_fw_bank_file;
    sai_remove_pon_olt_plug_fw_bank_file_fn                                                                                           remove_pon_olt_plug_fw_bank_file;
    sai_set_pon_olt_plug_fw_bank_file_attribute_fn                                                                                    set_pon_olt_plug_fw_bank_file_attribute;
    sai_get_pon_olt_plug_fw_bank_file_attribute_fn                                                                                    get_pon_olt_plug_fw_bank_file_attribute;
    sai_create_pon_olt_plug_fn                                                                                                        create_pon_olt_plug;
    sai_remove_pon_olt_plug_fn                                                                                                        remove_pon_olt_plug;
    sai_set_pon_olt_plug_attribute_fn                                                                                                 set_pon_olt_plug_attribute;
    sai_get_pon_olt_plug_attribute_fn                                                                                                 get_pon_olt_plug_attribute;
    sai_get_pon_olt_plug_fw_upgrade_status_attribute_fn                                                                               get_pon_olt_plug_fw_upgrade_status_attribute;
    sai_get_pon_olt_plug_state_attribute_fn                                                                                           get_pon_olt_plug_state_attribute;
    sai_get_pon_olt_plug_fw_bank_version_state_attribute_fn                                                                           get_pon_olt_plug_fw_bank_version_state_attribute;
    sai_get_pon_olt_plug_statistics_binned_env_attribute_fn                                                                           get_pon_olt_plug_statistics_binned_env_attribute;
    sai_get_pon_olt_plug_statistics_binned_nni_attribute_fn                                                                           get_pon_olt_plug_statistics_binned_nni_attribute;
    sai_get_pon_olt_plug_statistics_binned_temp_attribute_fn                                                                          get_pon_olt_plug_statistics_binned_temp_attribute;
    sai_bulk_get_pon_olt_plug_fw_upgrade_status_attribute_fn                                                                          get_pon_olt_plug_fw_upgrade_statuss_attribute;
    sai_bulk_get_pon_olt_plug_state_attribute_fn                                                                                      get_pon_olt_plug_states_attribute;
    sai_bulk_get_pon_olt_plug_fw_bank_version_state_attribute_fn                                                                      get_pon_olt_plug_fw_bank_version_states_attribute;
    sai_bulk_get_pon_olt_plug_statistics_binned_env_attribute_fn                                                                      get_pon_olt_plug_statistics_binned_envs_attribute;
    sai_bulk_get_pon_olt_plug_statistics_binned_nni_attribute_fn                                                                      get_pon_olt_plug_statistics_binned_nnis_attribute;
    sai_bulk_get_pon_olt_plug_statistics_binned_temp_attribute_fn                                                                     get_pon_olt_plug_statistics_binned_temps_attribute;
    sai_create_pon_onu_fn                                                                                                             create_pon_onu;
    sai_remove_pon_onu_fn                                                                                                             remove_pon_onu;
    sai_set_pon_onu_attribute_fn                                                                                                      set_pon_onu_attribute;
    sai_get_pon_onu_attribute_fn                                                                                                      get_pon_onu_attribute;
    sai_create_pon_onu_olt_service_fn                                                                                                 create_pon_onu_olt_service;
    sai_remove_pon_onu_olt_service_fn                                                                                                 remove_pon_onu_olt_service;
    sai_set_pon_onu_olt_service_attribute_fn                                                                                          set_pon_onu_olt_service_attribute;
    sai_get_pon_onu_olt_service_attribute_fn                                                                                          get_pon_onu_olt_service_attribute;
    sai_create_pon_onu_olt_service_network_fn                                                                                         create_pon_onu_olt_service_network;
    sai_remove_pon_onu_olt_service_network_fn                                                                                         remove_pon_onu_olt_service_network;
    sai_set_pon_onu_olt_service_network_attribute_fn                                                                                  set_pon_onu_olt_service_network_attribute;
    sai_get_pon_onu_olt_service_network_attribute_fn                                                                                  get_pon_onu_olt_service_network_attribute;
    sai_create_pon_onu_service_config_value_fn                                                                                        create_pon_onu_service_config_value;
    sai_remove_pon_onu_service_config_value_fn                                                                                        remove_pon_onu_service_config_value;
    sai_set_pon_onu_service_config_value_attribute_fn                                                                                 set_pon_onu_service_config_value_attribute;
    sai_get_pon_onu_service_config_value_attribute_fn                                                                                 get_pon_onu_service_config_value_attribute;
    sai_create_pon_onu_uni_fn                                                                                                         create_pon_onu_uni;
    sai_remove_pon_onu_uni_fn                                                                                                         remove_pon_onu_uni;
    sai_set_pon_onu_uni_attribute_fn                                                                                                  set_pon_onu_uni_attribute;
    sai_get_pon_onu_uni_attribute_fn                                                                                                  get_pon_onu_uni_attribute;
    sai_create_pon_onu_fw_bank_file_fn                                                                                                create_pon_onu_fw_bank_file;
    sai_remove_pon_onu_fw_bank_file_fn                                                                                                remove_pon_onu_fw_bank_file;
    sai_set_pon_onu_fw_bank_file_attribute_fn                                                                                         set_pon_onu_fw_bank_file_attribute;
    sai_get_pon_onu_fw_bank_file_attribute_fn                                                                                         get_pon_onu_fw_bank_file_attribute;
    sai_get_pon_onu_olt_service_downstream_qos_map_state_attribute_fn                                                                 get_pon_onu_olt_service_downstream_qos_map_state_attribute;
    sai_get_pon_onu_olt_service_downstream_qos_map_map_state_attribute_fn                                                             get_pon_onu_olt_service_downstream_qos_map_map_state_attribute;
    sai_get_pon_onu_uni_learned_addresses_state_attribute_fn                                                                          get_pon_onu_uni_learned_addresses_state_attribute;
    sai_get_pon_onu_state_attribute_fn                                                                                                get_pon_onu_state_attribute;
    sai_get_pon_onu_fw_bank_version_state_attribute_fn                                                                                get_pon_onu_fw_bank_version_state_attribute;
    sai_get_pon_onu_olt_service_state_attribute_fn                                                                                    get_pon_onu_olt_service_state_attribute;
    sai_get_pon_onu_olt_service_network_state_attribute_fn                                                                            get_pon_onu_olt_service_network_state_attribute;
    sai_get_pon_onu_uni_state_attribute_fn                                                                                            get_pon_onu_uni_state_attribute;
    sai_get_pon_onu_statistics_binned_attribute_fn                                                                                    get_pon_onu_statistics_binned_attribute;
    sai_get_pon_onu_statistics_binned_olt_pon_attribute_fn                                                                            get_pon_onu_statistics_binned_olt_pon_attribute;
    sai_get_pon_onu_statistics_binned_olt_pon_omcc_attribute_fn                                                                       get_pon_onu_statistics_binned_olt_pon_omcc_attribute;
    sai_get_pon_onu_statistics_binned_olt_pon_service_attribute_fn                                                                    get_pon_onu_statistics_binned_olt_pon_service_attribute;
    sai_get_pon_onu_statistics_binned_onu_enhanced_tc_pm_attribute_fn                                                                 get_pon_onu_statistics_binned_onu_enhanced_tc_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attribute_fn                                                     get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attribute_fn                                               get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attribute_fn                                                   get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attribute_fn                                                     get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_pm_attribute_fn                                                                    get_pon_onu_statistics_binned_onu_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_pm3_attribute_fn                                                                   get_pon_onu_statistics_binned_onu_ethernet_pm3_attribute;
    sai_get_pon_onu_statistics_binned_onu_fec_pm_attribute_fn                                                                         get_pon_onu_statistics_binned_onu_fec_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_gal_ethernet_pm_attribute_fn                                                                get_pon_onu_statistics_binned_onu_gal_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attribute_fn                                                        get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attribute_fn                                                     get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attribute_fn                                                             get_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_operational_pm_attribute_fn                                                                 get_pon_onu_statistics_binned_onu_operational_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_pon_attribute_fn                                                                            get_pon_onu_statistics_binned_onu_pon_attribute;
    sai_get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attribute_fn                                                 get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attribute_fn                                                     get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attribute_fn                                                      get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attribute_fn                                                        get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_olt_pon_attribute_fn                                                                      get_pon_onu_statistics_accumulating_olt_pon_attribute;
    sai_get_pon_onu_statistics_accumulating_olt_pon_omcc_attribute_fn                                                                 get_pon_onu_statistics_accumulating_olt_pon_omcc_attribute;
    sai_get_pon_onu_statistics_accumulating_olt_pon_service_attribute_fn                                                              get_pon_onu_statistics_accumulating_olt_pon_service_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attribute_fn                                                           get_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attribute_fn                                               get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attribute_fn                                         get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attribute_fn                                             get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attribute_fn                                               get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_pm_attribute_fn                                                              get_pon_onu_statistics_accumulating_onu_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_pm3_attribute_fn                                                             get_pon_onu_statistics_accumulating_onu_ethernet_pm3_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_fec_pm_attribute_fn                                                                   get_pon_onu_statistics_accumulating_onu_fec_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attribute_fn                                                          get_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attribute_fn                                                  get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attribute_fn                                               get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attribute_fn                                                       get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_operational_pm_attribute_fn                                                           get_pon_onu_statistics_accumulating_onu_operational_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_pon_attribute_fn                                                                      get_pon_onu_statistics_accumulating_onu_pon_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attribute_fn                                           get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attribute_fn                                               get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attribute_fn                                                get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attribute_fn                                                  get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attribute_fn                                                              get_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attribute_fn                                                get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attribute;
    sai_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attribute_fn                                                  get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attribute;
    sai_get_pon_onu_statistics_streaming_onu_ethernet_pm_attribute_fn                                                                 get_pon_onu_statistics_streaming_onu_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_fec_pm_attribute_fn                                                                      get_pon_onu_statistics_streaming_onu_fec_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attribute_fn                                                             get_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attribute_fn                                                     get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attribute_fn                                              get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attribute_fn                                                   get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attribute_fn                                                     get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attribute;
    sai_bulk_get_pon_onu_olt_service_downstream_qos_map_state_attribute_fn                                                            get_pon_onu_olt_service_downstream_qos_map_states_attribute;
    sai_bulk_get_pon_onu_olt_service_downstream_qos_map_map_state_attribute_fn                                                        get_pon_onu_olt_service_downstream_qos_map_map_states_attribute;
    sai_bulk_get_pon_onu_uni_learned_addresses_state_attribute_fn                                                                     get_pon_onu_uni_learned_addresses_states_attribute;
    sai_bulk_get_pon_onu_state_attribute_fn                                                                                           get_pon_onu_states_attribute;
    sai_bulk_get_pon_onu_fw_bank_version_state_attribute_fn                                                                           get_pon_onu_fw_bank_version_states_attribute;
    sai_bulk_get_pon_onu_olt_service_state_attribute_fn                                                                               get_pon_onu_olt_service_states_attribute;
    sai_bulk_get_pon_onu_olt_service_network_state_attribute_fn                                                                       get_pon_onu_olt_service_network_states_attribute;
    sai_bulk_get_pon_onu_uni_state_attribute_fn                                                                                       get_pon_onu_uni_states_attribute;
    sai_bulk_get_pon_onu_statistics_binned_attribute_fn                                                                               get_pon_onu_statistics_binneds_attribute;
    sai_bulk_get_pon_onu_statistics_binned_olt_pon_attribute_fn                                                                       get_pon_onu_statistics_binned_olt_pons_attribute;
    sai_bulk_get_pon_onu_statistics_binned_olt_pon_omcc_attribute_fn                                                                  get_pon_onu_statistics_binned_olt_pon_omccs_attribute;
    sai_bulk_get_pon_onu_statistics_binned_olt_pon_service_attribute_fn                                                               get_pon_onu_statistics_binned_olt_pon_services_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_enhanced_tc_pm_attribute_fn                                                            get_pon_onu_statistics_binned_onu_enhanced_tc_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attribute_fn                                                get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attribute_fn                                          get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bits_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attribute_fn                                              get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstreams_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attribute_fn                                                get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstreams_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_pm_attribute_fn                                                               get_pon_onu_statistics_binned_onu_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_pm3_attribute_fn                                                              get_pon_onu_statistics_binned_onu_ethernet_pm3s_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_fec_pm_attribute_fn                                                                    get_pon_onu_statistics_binned_onu_fec_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_gal_ethernet_pm_attribute_fn                                                           get_pon_onu_statistics_binned_onu_gal_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attribute_fn                                                   get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attribute_fn                                                get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attribute_fn                                                        get_pon_onu_statistics_binned_onu_mac_bridge_port_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_operational_pm_attribute_fn                                                            get_pon_onu_statistics_binned_onu_operational_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_pon_attribute_fn                                                                       get_pon_onu_statistics_binned_onu_pons_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attribute_fn                                            get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attribute_fn                                                get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attribute_fn                                                 get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attribute_fn                                                   get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_olt_pon_attribute_fn                                                                 get_pon_onu_statistics_accumulating_olt_pons_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_olt_pon_omcc_attribute_fn                                                            get_pon_onu_statistics_accumulating_olt_pon_omccs_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_olt_pon_service_attribute_fn                                                         get_pon_onu_statistics_accumulating_olt_pon_services_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attribute_fn                                                      get_pon_onu_statistics_accumulating_onu_enhanced_tc_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attribute_fn                                          get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attribute_fn                                    get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bits_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attribute_fn                                        get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstreams_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attribute_fn                                          get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstreams_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_pm_attribute_fn                                                         get_pon_onu_statistics_accumulating_onu_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_pm3_attribute_fn                                                        get_pon_onu_statistics_accumulating_onu_ethernet_pm3s_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_fec_pm_attribute_fn                                                              get_pon_onu_statistics_accumulating_onu_fec_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attribute_fn                                                     get_pon_onu_statistics_accumulating_onu_gal_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attribute_fn                                             get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attribute_fn                                          get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attribute_fn                                                  get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_operational_pm_attribute_fn                                                      get_pon_onu_statistics_accumulating_onu_operational_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_pon_attribute_fn                                                                 get_pon_onu_statistics_accumulating_onu_pons_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attribute_fn                                      get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attribute_fn                                          get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attribute_fn                                           get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attribute_fn                                             get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attribute_fn                                                         get_pon_onu_statistics_streaming_onu_enhanced_tc_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attribute_fn                                           get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstreams_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attribute_fn                                             get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstreams_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_ethernet_pm_attribute_fn                                                            get_pon_onu_statistics_streaming_onu_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_fec_pm_attribute_fn                                                                 get_pon_onu_statistics_streaming_onu_fec_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attribute_fn                                                        get_pon_onu_statistics_streaming_onu_gal_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attribute_fn                                                get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attribute_fn                                         get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attribute_fn                                              get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attribute_fn                                                get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pms_attribute;
    sai_create_pon_onu_template_onu_fn                                                                                                create_pon_onu_template_onu;
    sai_remove_pon_onu_template_onu_fn                                                                                                remove_pon_onu_template_onu;
    sai_set_pon_onu_template_onu_attribute_fn                                                                                         set_pon_onu_template_onu_attribute;
    sai_get_pon_onu_template_onu_attribute_fn                                                                                         get_pon_onu_template_onu_attribute;
    sai_create_pon_onu_template_onu_fw_bank_file_fn                                                                                   create_pon_onu_template_onu_fw_bank_file;
    sai_remove_pon_onu_template_onu_fw_bank_file_fn                                                                                   remove_pon_onu_template_onu_fw_bank_file;
    sai_set_pon_onu_template_onu_fw_bank_file_attribute_fn                                                                            set_pon_onu_template_onu_fw_bank_file_attribute;
    sai_get_pon_onu_template_onu_fw_bank_file_attribute_fn                                                                            get_pon_onu_template_onu_fw_bank_file_attribute;
    sai_create_pon_onu_template_olt_service_fn                                                                                        create_pon_onu_template_olt_service;
    sai_remove_pon_onu_template_olt_service_fn                                                                                        remove_pon_onu_template_olt_service;
    sai_set_pon_onu_template_olt_service_attribute_fn                                                                                 set_pon_onu_template_olt_service_attribute;
    sai_get_pon_onu_template_olt_service_attribute_fn                                                                                 get_pon_onu_template_olt_service_attribute;
    sai_create_pon_onu_template_olt_service_network_fn                                                                                create_pon_onu_template_olt_service_network;
    sai_remove_pon_onu_template_olt_service_network_fn                                                                                remove_pon_onu_template_olt_service_network;
    sai_set_pon_onu_template_olt_service_network_attribute_fn                                                                         set_pon_onu_template_olt_service_network_attribute;
    sai_get_pon_onu_template_olt_service_network_attribute_fn                                                                         get_pon_onu_template_olt_service_network_attribute;
    sai_create_pon_onu_template_service_config_value_fn                                                                               create_pon_onu_template_service_config_value;
    sai_remove_pon_onu_template_service_config_value_fn                                                                               remove_pon_onu_template_service_config_value;
    sai_set_pon_onu_template_service_config_value_attribute_fn                                                                        set_pon_onu_template_service_config_value_attribute;
    sai_get_pon_onu_template_service_config_value_attribute_fn                                                                        get_pon_onu_template_service_config_value_attribute;
    sai_create_pon_onu_template_uni_fn                                                                                                create_pon_onu_template_uni;
    sai_remove_pon_onu_template_uni_fn                                                                                                remove_pon_onu_template_uni;
    sai_set_pon_onu_template_uni_attribute_fn                                                                                         set_pon_onu_template_uni_attribute;
    sai_get_pon_onu_template_uni_attribute_fn                                                                                         get_pon_onu_template_uni_attribute;
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
    sai_execute_pon_action_fn                                                                                                         execute_pon_action;
} sai_pon_api_t;

/**
 * @}
 */

#endif /* __SAIEXPERIMENTALPON_H_ */
