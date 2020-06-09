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
 * @file    saisystemport.h
 *
 * @brief   This module defines SAI System Port interface
 */

#if !defined (__SAISYSTEMPORT_H_)
#define __SAISYSTEMPORT_H_

#include <saitypes.h>

/**
 * @defgroup SAISYSTEMPORT SAI - System Port specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_SYSTEM_PORT_ATTR_TYPE
 */
typedef enum _sai_system_port_type_t
{
    /** Local to switch */
    SAI_SYSTEM_PORT_TYPE_LOCAL,

    /** Remote switch */
    SAI_SYSTEM_PORT_TYPE_REMOTE,

} sai_system_port_type_t;

/**
 * @brief Attribute Id in sai_set_system_port_attribute() and
 * sai_get_system_port_attribute() calls
 */
typedef enum _sai_system_port_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SYSTEM_PORT_ATTR_START,

    /* READ-ONLY */

    /**
     * @brief System Port Type
     *
     * @type sai_system_port_type_t
     * @flags READ_ONLY
     */
    SAI_SYSTEM_PORT_ATTR_TYPE = SAI_SYSTEM_PORT_ATTR_START,

    /**
     * @brief Number of Virtual output queues on port
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SYSTEM_PORT_ATTR_QOS_NUMBER_OF_VOQS,

    /**
     * @brief List of Virtual output Queues for the port
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_QUEUE
     */
    SAI_SYSTEM_PORT_ATTR_QOS_VOQ_LIST,

    /**
     * @brief Local port for the system port
     *
     * Only valid for system ports which are mapped to local ports.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_SYSTEM_PORT_ATTR_PORT,

    /* READ-WRITE */

    /**
     * @brief Admin Mode
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SYSTEM_PORT_ATTR_ADMIN_STATE,

    /**
     * @brief System Port Configuration Information
     *
     * @type sai_system_port_config_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_SYSTEM_PORT_ATTR_CONFIG_INFO,

    /**
     * @brief Enable TC -> VOQ MAP on system port
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on system port.
     * Default no map, i.e. all packets to VOQ 0.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SYSTEM_PORT_ATTR_QOS_TC_TO_QUEUE_MAP,

    /**
     * @brief End of attributes
     */
    SAI_SYSTEM_PORT_ATTR_END,

    /** Custom range base value */
    SAI_SYSTEM_PORT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SYSTEM_PORT_ATTR_CUSTOM_RANGE_END

} sai_system_port_attr_t;

/**
 * @brief Create system port
 *
 * @param[out] system_port_id System Port id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_system_port_fn)(
        _Out_ sai_object_id_t *system_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove system port
 *
 * @param[in] system_port_id System Port id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_system_port_fn)(
        _In_ sai_object_id_t system_port_id);

/**
 * @brief Set system port attribute value.
 *
 * @param[in] system_port_id System Port id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_system_port_attribute_fn)(
        _In_ sai_object_id_t system_port_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get system port attribute value.
 *
 * @param[in] system_port_id System Port id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_system_port_attribute_fn)(
        _In_ sai_object_id_t system_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Port methods table retrieved with sai_api_query()
 */
typedef struct _sai_system_port_api_t
{
    sai_create_system_port_fn                create_system_port;
    sai_remove_system_port_fn                remove_system_port;
    sai_set_system_port_attribute_fn         set_system_port_attribute;
    sai_get_system_port_attribute_fn         get_system_port_attribute;

} sai_system_port_api_t;

/**
 * @}
 */
#endif /** __SAISYSTEMPORT_H_ */
