/**
 * Copyright (c) 2024 Microsoft Open Technologies, Inc.
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
 * @file    saipoe.h
 *
 * @brief   This module defines SAI POE interface
 */

#if !defined (__SAIPOE_H_)
#define __SAIPOE_H_

#include <saitypes.h>

/**
 * @brief POE device power limit mode
 *
 * port limit - max power limit per port is configured by the user
 * class limit - max power is set automatically according the class of the connected device
 */
typedef enum _sai_poe_device_limit_mode_t
{
    SAI_POE_DEVICE_LIMIT_MODE_PORT  = 0,
    SAI_POE_DEVICE_LIMIT_MODE_CLASS  = 1
} sai_poe_device_limit_mode_t;

/**
 * @brief POE device PSE (power sourcing equipment) status
 */
typedef enum _sai_poe_pse_status_t
{
    SAI_POE_PSE_STATUS_TYPE_ACTIVE,
    SAI_POE_PSE_STATUS_TYPE_FAIL,
    SAI_POE_PSE_STATUS_TYPE_NOT_PRESENT,
} sai_poe_pse_status_t;

/**
 * @brief POE port (IEEE) standard
 */
typedef enum _sai_poe_port_standard_t
{
    SAI_POE_PORT_STANDARD_TYPE_AF,
    SAI_POE_PORT_STANDARD_TYPE_AT,
    SAI_POE_PORT_STANDARD_TYPE_60W,
    SAI_POE_PORT_STANDARD_TYPE_BT_TYPE3,
    SAI_POE_PORT_STANDARD_TYPE_BT_TYPE4,
} sai_poe_port_standard_t;

/**
 * @brief POE port power priority
 */
typedef enum _sai_poe_port_power_priority_t
{
    SAI_POE_PORT_POWER_PRIORITY_TYPE_LOW,
    SAI_POE_PORT_POWER_PRIORITY_TYPE_HIGH,
    SAI_POE_PORT_POWER_PRIORITY_TYPE_CRITICAL,
} sai_poe_port_power_priority_t;

/**
 * @brief POE port status
 */
typedef enum _sai_poe_port_status_t
{
    SAI_POE_PORT_STATUS_TYPE_OFF,
    SAI_POE_PORT_STATUS_TYPE_SEARCHING,
    SAI_POE_PORT_STATUS_TYPE_DELIVERING_POWER,
    SAI_POE_PORT_STATUS_TYPE_FAULT,
} sai_poe_port_status_t;

/**
 * @brief POE device attributes
 */
typedef enum _sai_poe_device_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_POE_DEVICE_ATTR_START,

    /**
     * @brief Device Information for POE initialization.
     *
     * Hardware information format is based on POE SAI implementations by vendors.
     * Format is vendor specific.
     * Example: Like PCI location, I2C address, UART etc.
     *
     * @type char
     * @flags CREATE_ONLY
     * @default ""
     */
    SAI_POE_DEVICE_ATTR_HARDWARE_INFO = SAI_POE_DEVICE_ATTR_START,

    /**
     * @brief A list of all the PSE devices
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_POE_PSE
     */
    SAI_POE_DEVICE_ATTR_POE_PSE_LIST,

    /**
     * @brief A list of all the POE ports
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_POE_PORT
     */
    SAI_POE_DEVICE_ATTR_POE_PORT_LIST,

    /**
     * @brief The total power in the device, in watts
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_POE_DEVICE_ATTR_TOTAL_POWER,

    /**
     * @brief Total power consumption, in MILLI watts
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_POE_DEVICE_ATTR_POWER_CONSUMPTION,

    /**
     * @brief POE device version and information (POE firmware version)
     *
     * @type char
     * @flags READ_ONLY
     */
    SAI_POE_DEVICE_ATTR_VERSION,

    /**
     * @brief Power limit mode
     *
     * @type sai_poe_device_limit_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_POE_DEVICE_LIMIT_MODE_CLASS
     */
    SAI_POE_DEVICE_ATTR_POWER_LIMIT_MODE,

    /**
     * @brief End of attributes
     */
    SAI_POE_DEVICE_ATTR_END,

    /** Custom range base value */
    SAI_POE_DEVICE_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_poe_device_attr_t;

/**
 * @brief POE PSE attributes
 */
typedef enum _sai_poe_pse_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_POE_PSE_ATTR_START,

    /**
     * @brief POE PSE ID (index)
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_POE_PSE_ATTR_ID = SAI_POE_PSE_ATTR_START,

    /**
     * @brief POE device ID
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_POE_DEVICE
     */
    SAI_POE_PSE_ATTR_DEVICE_ID,

    /**
     * @brief POE PSE software version
     *
     * @type char
     * @flags READ_ONLY
     */
    SAI_POE_PSE_ATTR_SOFTWARE_VERSION,

    /**
     * @brief POE PSE hardware version
     *
     * @type char
     * @flags READ_ONLY
     */
    SAI_POE_PSE_ATTR_HARDWARE_VERSION,

    /**
     * @brief Temperature (in Celsius) of the PSE
     *
     * @type sai_int16_t
     * @flags READ_ONLY
     */
    SAI_POE_PSE_ATTR_TEMPERATURE,

    /**
     * @brief Status of the PSE
     *
     * @type sai_poe_pse_status_t
     * @flags READ_ONLY
     */
    SAI_POE_PSE_ATTR_STATUS,

    /**
     * @brief End of attributes
     */
    SAI_POE_PSE_ATTR_END,

    /** Custom range base value */
    SAI_POE_PSE_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_poe_pse_attr_t;

/**
 * @brief POE port attributes
 */
typedef enum _sai_poe_port_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_POE_PORT_ATTR_START,

    /**
     * @brief POE port front panel ID
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_POE_PORT_ATTR_FRONT_PANEL_ID = SAI_POE_PORT_ATTR_START,

    /**
     * @brief POE device ID
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_POE_DEVICE
     */
    SAI_POE_PORT_ATTR_DEVICE_ID,

    /**
     * @brief POE port standard
     *
     * @type sai_poe_port_standard_t
     * @flags READ_ONLY
     */
    SAI_POE_PORT_ATTR_STANDARD,

    /**
     * @brief POE port enabled/disabled state, as set by the user
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_POE_PORT_ATTR_ADMIN_ENABLED_STATE,

    /**
     * @brief POE port power limit mode
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_POE_PORT_ATTR_POWER_LIMIT,

    /**
     * @brief POE port priority
     *
     * @type sai_poe_port_power_priority_t
     * @flags CREATE_AND_SET
     * @default SAI_POE_PORT_POWER_PRIORITY_TYPE_HIGH
     */
    SAI_POE_PORT_ATTR_POWER_PRIORITY,

    /**
     * @brief POE port consumption information
     *
     * @type sai_poe_port_power_consumption_t
     * @flags READ_ONLY
     */
    SAI_POE_PORT_ATTR_CONSUMPTION,

    /**
     * @brief POE port status
     *
     * @type sai_poe_port_status_t
     * @flags READ_ONLY
     */
    SAI_POE_PORT_ATTR_STATUS,

    /**
     * @brief End of attributes
     */
    SAI_POE_PORT_ATTR_END,

    /** Custom range base value */
    SAI_POE_PORT_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_poe_port_attr_t;

/**
 * @brief Create a POE device instance
 *
 * @param[out] poe_device_id POE device ID
 * @param[in] switch_id Switch ID
 * @param[in] attr_count Count
 * @param[in] attr_list Attribute list values
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_create_poe_device_fn)(
        _Out_ sai_object_id_t *poe_device_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove POE device instance.
 *
 * @param[in] poe_device_id POE device ID
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_remove_poe_device_fn)(
        _In_ sai_object_id_t poe_device_id);

/**
 * @brief Set the attribute of POE instance.
 *
 * @param[in] poe_device_id POE device ID
 * @param[in] attr Attribute value
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_set_poe_device_attribute_fn)(
        _In_ sai_object_id_t poe_device_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get the attribute of POE instance.
 *
 * @param[in] poe_device_id POE device ID
 * @param[in] attr_count Number of the attribute
 * @param[inout] attr_list Attribute value
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_get_poe_device_attribute_fn)(
        _In_ sai_object_id_t poe_device_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create a POE PSE instance
 *
 * @param[out] poe_pse_id POE PSE ID
 * @param[in] switch_id Switch ID
 * @param[in] attr_count Count
 * @param[in] attr_list Attribute list values
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_create_poe_pse_fn)(
        _Out_ sai_object_id_t *poe_pse_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove POE device PSE.
 *
 * @param[in] poe_pse_id POE PSE ID
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_remove_poe_pse_fn)(
        _In_ sai_object_id_t poe_pse_id);

/**
 * @brief Set the attribute of POE PSE.
 *
 * @param[in] poe_pse_id POE PSE ID
 * @param[in] attr Attribute value
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_set_poe_pse_attribute_fn)(
        _In_ sai_object_id_t poe_pse_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get the attribute of POE PSE.
 *
 * @param[in] poe_pse_id POE PSE ID
 * @param[in] attr_count Number of the attribute
 * @param[inout] attr_list Attribute value
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_get_poe_pse_attribute_fn)(
        _In_ sai_object_id_t poe_pse_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create POE port object
 *
 * @param[out] poe_port_id POE port id
 * @param[in] switch_id Switch ID
 * @param[in] attr_count Number of the attribute
 * @param[in] attr_list Value of attributes
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_create_poe_port_fn)(
        _Out_ sai_object_id_t *poe_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove POE port
 *
 * @param[in] poe_port_id POE port id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_poe_port_fn)(
        _In_ sai_object_id_t poe_port_id);

/**
 * @brief Set the attribute of POE port.
 *
 * @param[in] poe_port_id POE port id
 * @param[in] attr Attribute value
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_set_poe_port_attribute_fn)(
        _In_ sai_object_id_t poe_port_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get the attribute of POE port.
 *
 * @param[in] poe_port_id POE port id
 * @param[in] attr_count Attribute count
 * @param[inout] attr_list Attribute value
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_get_poe_port_attribute_fn)(
        _In_ sai_object_id_t poe_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief POE device method table retrieved with poe_api_query()
 */
typedef struct _sai_poe_api_t
{
    sai_create_poe_device_fn               create_poe_device;
    sai_remove_poe_device_fn               remove_poe_device;
    sai_set_poe_device_attribute_fn        set_poe_device_attribute;
    sai_get_poe_device_attribute_fn        get_poe_device_attribute;
    sai_create_poe_pse_fn                  create_poe_pse;
    sai_remove_poe_pse_fn                  remove_poe_pse;
    sai_set_poe_pse_attribute_fn           set_poe_pse_attribute;
    sai_get_poe_pse_attribute_fn           get_poe_pse_attribute;
    sai_create_poe_port_fn                 create_poe_port;
    sai_remove_poe_port_fn                 remove_poe_port;
    sai_set_poe_port_attribute_fn          set_poe_port_attribute;
    sai_get_poe_port_attribute_fn          get_poe_port_attribute;
} sai_poe_api_t;

#endif /** __SAIPOE_H_ */

