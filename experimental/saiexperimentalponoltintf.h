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
 * @file    saiexperimentalponoltintf.h
 *
 * @brief   This module defines the experimental PON OLT interface API.
 */

#if !defined (__SAIEXPERIMENTALPONOLTINTF_H_)
#define __SAIEXPERIMENTALPONOLTINTF_H_

#include <saitypes.h>

#include <saiexperimentalponcommon.h>

/**
 * @defgroup SAIEXPERIMENTALPONOLTINTF SAI - Experimental: PON specific API definitions
 *
 * @{
 */

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
     */
    SAI_PON_OLT_INTF_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ATTR_START,

    /**
     * @brief Device ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ATTR_DEVICE_ID,

    /**
     * @brief Port ID
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ATTR_PORT_ID,

    /**
     * @brief PON Enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ATTR_PON_ENABLE,

    /**
     * @brief Discovery Period
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ATTR_DISCOVERY_PERIOD,

    /**
     * @brief Downstream FEC
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ATTR_DOWNSTREAM_FEC,

    /**
     * @brief Encryption
     *
     * @type sai_pon_olt_intf_encryption_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ATTR_ENCRYPTION,

    /**
     * @brief Encryption Key Time
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_OLT_INTF_ATTR_ENCRYPTION_KEY_TIME,

    /**
     * @brief Error Detection Maximum HEC Ratio
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ATTR_ERROR_DETECTION_MAXIMUM_HEC_RATIO,

    /**
     * @brief Error Detection Minimum HEC Sample
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_OLT_INTF_ATTR_ERROR_DETECTION_MINIMUM_HEC_SAMPLE,

    /**
     * @brief Error Detection Maximum Ratio
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ATTR_ERROR_DETECTION_MAXIMUM_RATIO,

    /**
     * @brief Error Detection Minimum Sample
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_OLT_INTF_ATTR_ERROR_DETECTION_MINIMUM_SAMPLE,

    /**
     * @brief Guard Time
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ATTR_GUARD_TIME,

    /**
     * @brief Max Frame Size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ATTR_MAX_FRAME_SIZE,

    /**
     * @brief PON ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ATTR_PON_ID,

    /**
     * @brief PON Tag
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ATTR_PON_TAG,

    /**
     * @brief Aging Time
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_OLT_INTF_ATTR_AGING_TIME,

    /**
     * @brief Protection Hold
     *
     * @type bool
     * @flags CREATE_AND_SET
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
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_NETWORK_ATTR_START,

    /**
     * @brief Network ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_NETWORK_ID,

    /**
     * @brief Learning Limit
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_LEARNING_LIMIT,

    /**
     * @brief Flooding GEMPORT ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_FLOODING_GEMPORT_ID,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     */
    SAI_PON_OLT_INTF_NETWORK_ATTR_VLAN_ID,

    /**
     * @brief Flooding SLA Profile
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
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
     */
    SAI_PON_OLT_INTF_ONU_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ONU_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief ONU ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_OLT_INTF_ONU_ATTR_ONU_ID,

    /**
     * @brief Disable
     *
     * @type bool
     * @flags CREATE_AND_SET
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
     */
    SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief Service Port ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ONU_OLT_SERVICE_ATTR_SERVICE_PORT_ID,

    /**
     * @brief Unicast ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
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
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_ATTR_OLT_SERVICE_ID,

    /**
     * @brief Allocation ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
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
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief OLT Service ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_OLT_SERVICE_ID,

    /**
     * @brief GEMPORT ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_ATTR_GEMPORT_ID,

    /**
     * @brief TCONT Ref
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
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
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_GROUP_NAME = SAI_PON_OLT_PROTECTION_GROUP_ATTR_START,

    /**
     * @brief Admin Status
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_ADMIN_STATUS,

    /**
     * @brief Local OLT Interface
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_LOCAL_OLT_INTERFACE,

    /**
     * @brief Peer OLT
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_PEER_OLT,

    /**
     * @brief Peer OLT Interface
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_PEER_OLT_INTERFACE,

    /**
     * @brief Coordination Mode
     *
     * @type sai_pon_olt_protection_group_coordination_mode_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_OLT_PROTECTION_GROUP_ATTR_COORDINATION_MODE,

    /**
     * @brief Revertive
     *
     * @type bool
     * @flags CREATE_AND_SET
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
     */
    SAI_PON_OLT_INTF_ONU_OPERATIONAL_STATE_ATTR_OLT_NAME = SAI_PON_OLT_INTF_ONU_OPERATIONAL_STATE_ATTR_START,

    /**
     * @brief ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
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
     */
    SAI_PON_OLT_INTF_NNI_NETWORK_LEARNING_TABLE_STATE_ATTR_NETWORK_ID,

    /**
     * @brief MAC Address
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
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
     */
    SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_NETWORK_STATE_ATTR_START,

    /**
     * @brief Vlan ID
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan true
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
     */
    SAI_PON_OLT_INTF_ONU_STATE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_STATE_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
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
     */
    SAI_PON_OLT_INTF_ONU_FW_UPGRADE_STATE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_FW_UPGRADE_STATE_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
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
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_TCONT_STATE_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief Service Port ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
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
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_OLT_INTF_NAME = SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_START,

    /**
     * @brief ONU Serial Number
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE_ATTR_ONU_SERIAL_NUMBER,

    /**
     * @brief Service Port ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
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
     */
    SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_OLT_NAME = SAI_PON_OLT_INTF_STATISTICS_BINNED_ATTR_START,

    /**
     * @brief OLT Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
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
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_OLT_NAME = SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_START,

    /**
     * @brief OLT Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_ATTR_OLT_STATS_ID,

    /**
     * @brief Flood ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
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
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_NETWORK_ATTR_OLT_NAME = SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_NETWORK_ATTR_START,

    /**
     * @brief OLT Stats ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     */
    SAI_PON_OLT_STATISTICS_BINNED_PON_FLOODING_NETWORK_ATTR_OLT_STATS_ID,

    /**
     * @brief Flood ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
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
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_OLT_NAME = SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_ATTR_START,

    /**
     * @brief OLT ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
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
     */
    SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_NNI_NETWORK_ATTR_OLT_NAME = SAI_PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_NNI_NETWORK_ATTR_START,

    /**
     * @brief OLT ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
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
* @brief Execute an OLT protection switch.
*
* @param[in] olt_intf_id SAI object ID of the local PON OLT interface.
* @param[in] target_role Requested OLT protection role.
*
* @return #SAI_STATUS_SUCCESS on success, failure status code on error.
*/
typedef sai_status_t (*sai_olt_protection_switch_fn)(
        _In_ sai_object_id_t olt_intf_id,
        _In_ sai_pon_olt_intf_state_protection_role_t target_role);

/**
* @brief Payload for a single OLT protection role change event.
*/
typedef struct _sai_olt_state_change_notification_data_t
{
    /** SAI object ID of the PON OLT interface whose role changed. */
    sai_object_id_t olt_intf_id;

    /** New OLT protection role. */
    sai_olt_role_t new_role;

    /**
    * Previous OLT protection role.
    *
    * SAI_OLT_ROLE_UNKNOWN is reported when the previous role is unavailable.
    */
    sai_olt_role_t prev_role;
} sai_olt_state_change_notification_data_t;

/**
* @brief OLT state change notification callback.
*
* The SDK owns the data array. The array is valid only for the duration of the
* callback and must not be retained by the callback recipient.
*
* It must transfer the notification to its normal PON notification processing
* path, which publishes the normalized event to PON_ASIC_DB:PON_NOTIFICATIONS.
*
* @param[in] count Number of events in this callback batch.
* @param[in] data Array of count OLT role change entries.
*/
typedef void (*sai_olt_state_change_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_olt_state_change_notification_data_t *data);


#endif /* __SAIPON_H_ */

typedef struct _sai_pon_olt_intf_api_t
{
    sai_create_pon_olt_intf_fn                                                         create_pon_olt_intf;
    sai_remove_pon_olt_intf_fn                                                         remove_pon_olt_intf;
    sai_set_pon_olt_intf_attribute_fn                                                  set_pon_olt_intf_attribute;
    sai_get_pon_olt_intf_attribute_fn                                                  get_pon_olt_intf_attribute;
    sai_create_pon_olt_intf_network_fn                                                 create_pon_olt_intf_network;
    sai_remove_pon_olt_intf_network_fn                                                 remove_pon_olt_intf_network;
    sai_set_pon_olt_intf_network_attribute_fn                                          set_pon_olt_intf_network_attribute;
    sai_get_pon_olt_intf_network_attribute_fn                                          get_pon_olt_intf_network_attribute;
    sai_create_pon_olt_intf_onu_fn                                                     create_pon_olt_intf_onu;
    sai_remove_pon_olt_intf_onu_fn                                                     remove_pon_olt_intf_onu;
    sai_set_pon_olt_intf_onu_attribute_fn                                              set_pon_olt_intf_onu_attribute;
    sai_get_pon_olt_intf_onu_attribute_fn                                              get_pon_olt_intf_onu_attribute;
    sai_create_pon_olt_intf_onu_olt_service_fn                                         create_pon_olt_intf_onu_olt_service;
    sai_remove_pon_olt_intf_onu_olt_service_fn                                         remove_pon_olt_intf_onu_olt_service;
    sai_set_pon_olt_intf_onu_olt_service_attribute_fn                                  set_pon_olt_intf_onu_olt_service_attribute;
    sai_get_pon_olt_intf_onu_olt_service_attribute_fn                                  get_pon_olt_intf_onu_olt_service_attribute;
    sai_create_pon_olt_intf_onu_service_tcont_fn                                       create_pon_olt_intf_onu_service_tcont;
    sai_remove_pon_olt_intf_onu_service_tcont_fn                                       remove_pon_olt_intf_onu_service_tcont;
    sai_set_pon_olt_intf_onu_service_tcont_attribute_fn                                set_pon_olt_intf_onu_service_tcont_attribute;
    sai_get_pon_olt_intf_onu_service_tcont_attribute_fn                                get_pon_olt_intf_onu_service_tcont_attribute;
    sai_create_pon_olt_intf_onu_service_gemport_fn                                     create_pon_olt_intf_onu_service_gemport;
    sai_remove_pon_olt_intf_onu_service_gemport_fn                                     remove_pon_olt_intf_onu_service_gemport;
    sai_set_pon_olt_intf_onu_service_gemport_attribute_fn                              set_pon_olt_intf_onu_service_gemport_attribute;
    sai_get_pon_olt_intf_onu_service_gemport_attribute_fn                              get_pon_olt_intf_onu_service_gemport_attribute;
    sai_create_pon_olt_protection_group_fn                                             create_pon_olt_protection_group;
    sai_remove_pon_olt_protection_group_fn                                             remove_pon_olt_protection_group;
    sai_set_pon_olt_protection_group_attribute_fn                                      set_pon_olt_protection_group_attribute;
    sai_get_pon_olt_protection_group_attribute_fn                                      get_pon_olt_protection_group_attribute;
    sai_get_pon_olt_intf_onu_operational_state_attribute_fn                            get_pon_olt_intf_onu_operational_state_attribute;
    sai_get_pon_olt_intf_nni_network_learning_table_state_attribute_fn                 get_pon_olt_intf_nni_network_learning_table_state_attribute;
    sai_get_pon_firmware_filename_state_attribute_fn                                   get_pon_firmware_filename_state_attribute;
    sai_get_pon_olt_intf_state_attribute_fn                                            get_pon_olt_intf_state_attribute;
    sai_get_pon_olt_intf_network_state_attribute_fn                                    get_pon_olt_intf_network_state_attribute;
    sai_get_pon_olt_intf_onu_state_attribute_fn                                        get_pon_olt_intf_onu_state_attribute;
    sai_get_pon_olt_intf_onu_fw_upgrade_state_attribute_fn                             get_pon_olt_intf_onu_fw_upgrade_state_attribute;
    sai_get_pon_olt_intf_onu_service_tcont_state_attribute_fn                          get_pon_olt_intf_onu_service_tcont_state_attribute;
    sai_get_pon_olt_intf_onu_service_gemport_state_attribute_fn                        get_pon_olt_intf_onu_service_gemport_state_attribute;
    sai_get_pon_olt_protection_group_state_attribute_fn                                get_pon_olt_protection_group_state_attribute;
    sai_get_pon_olt_intf_statistics_binned_attribute_fn                                get_pon_olt_intf_statistics_binned_attribute;
    sai_get_pon_olt_statistics_binned_pon_flooding_attribute_fn                        get_pon_olt_statistics_binned_pon_flooding_attribute;
    sai_get_pon_olt_statistics_binned_pon_flooding_network_attribute_fn                get_pon_olt_statistics_binned_pon_flooding_network_attribute;
    sai_get_pon_olt_statistics_accumulating_attribute_fn                               get_pon_olt_statistics_accumulating_attribute;
    sai_get_pon_olt_statistics_accumulating_env_attribute_fn                           get_pon_olt_statistics_accumulating_env_attribute;
    sai_get_pon_olt_statistics_accumulating_temp_attribute_fn                          get_pon_olt_statistics_accumulating_temp_attribute;
    sai_get_pon_olt_statistics_accumulating_pon_flooding_attribute_fn                  get_pon_olt_statistics_accumulating_pon_flooding_attribute;
    sai_get_pon_olt_statistics_accumulating_pon_flooding_nni_network_attribute_fn      get_pon_olt_statistics_accumulating_pon_flooding_nni_network_attribute;
    sai_bulk_get_pon_olt_intf_onu_operational_state_attribute_fn                       get_pon_olt_intf_onu_operational_states_attribute;
    sai_bulk_get_pon_olt_intf_nni_network_learning_table_state_attribute_fn            get_pon_olt_intf_nni_network_learning_table_states_attribute;
    sai_bulk_get_pon_firmware_filename_state_attribute_fn                              get_pon_firmware_filename_states_attribute;
    sai_bulk_get_pon_olt_intf_state_attribute_fn                                       get_pon_olt_intf_states_attribute;
    sai_bulk_get_pon_olt_intf_network_state_attribute_fn                               get_pon_olt_intf_network_states_attribute;
    sai_bulk_get_pon_olt_intf_onu_state_attribute_fn                                   get_pon_olt_intf_onu_states_attribute;
    sai_bulk_get_pon_olt_intf_onu_fw_upgrade_state_attribute_fn                        get_pon_olt_intf_onu_fw_upgrade_states_attribute;
    sai_bulk_get_pon_olt_intf_onu_service_tcont_state_attribute_fn                     get_pon_olt_intf_onu_service_tcont_states_attribute;
    sai_bulk_get_pon_olt_intf_onu_service_gemport_state_attribute_fn                   get_pon_olt_intf_onu_service_gemport_states_attribute;
    sai_bulk_get_pon_olt_intf_statistics_binned_attribute_fn                           get_pon_olt_intf_statistics_binneds_attribute;
    sai_bulk_get_pon_olt_statistics_binned_pon_flooding_attribute_fn                   get_pon_olt_statistics_binned_pon_floodings_attribute;
    sai_bulk_get_pon_olt_statistics_binned_pon_flooding_network_attribute_fn           get_pon_olt_statistics_binned_pon_flooding_networks_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_attribute_fn                          get_pon_olt_statistics_accumulatings_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_env_attribute_fn                      get_pon_olt_statistics_accumulating_envs_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_temp_attribute_fn                     get_pon_olt_statistics_accumulating_temps_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_pon_flooding_attribute_fn             get_pon_olt_statistics_accumulating_pon_floodings_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_pon_flooding_nni_network_attribute_fn get_pon_olt_statistics_accumulating_pon_flooding_nni_networks_attribute;
    sai_olt_protection_switch_fn                                                       olt_protection_switch;
    sai_register_olt_state_change_notification_fn                                      register_olt_state_change_notification;
} sai_pon_olt_intf_api_t;

/**
 * @}
 */

#endif /* __SAIEXPERIMENTALPONOLTINTF_H_ */
