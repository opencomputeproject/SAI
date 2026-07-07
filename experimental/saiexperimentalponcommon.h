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
 * @file    saiexperimentalponcommon.h
 *
 * @brief   This module defines common experimental PON enum types.
 */

#if !defined (__SAIEXPERIMENTALPONCOMMON_H_)
#define __SAIEXPERIMENTALPONCOMMON_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALPONCOMMON SAI - Experimental: PON specific API definitions
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
 * @}
 */

#endif /* __SAIEXPERIMENTALPONCOMMON_H_ */
