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
 * @file    saistp.h
 *
 * @brief   This module defines SAI STP interface
 */

#if !defined (__SAISTP_H_)
#define __SAISTP_H_

#include <saitypes.h>

/**
 * @defgroup SAISTP SAI - STP specific public APIs and data structures
 *
 * @{
 */

/**
 * @brief Data structure for STP port state
 */
typedef enum _sai_stp_port_state_t
{
    /** Port is in Learning mode */
    SAI_STP_PORT_STATE_LEARNING,

    /** Port is in Forwarding mode */
    SAI_STP_PORT_STATE_FORWARDING,

    /** Port is in Blocking mode */
    SAI_STP_PORT_STATE_BLOCKING,

} sai_stp_port_state_t;

/**
 * @brief SAI attributes for STP
 */
typedef enum _sai_stp_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_STP_ATTR_START,

    /**
     * @brief VLANs attached to STP instance
     *
     * @type sai_vlan_list_t
     * @flags READ_ONLY
     */
    SAI_STP_ATTR_VLAN_LIST = SAI_STP_ATTR_START,

    /**
     * @brief Bridge attached to STP instance
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE
     */
    SAI_STP_ATTR_BRIDGE_ID,

    /**
     * @brief Port member list
     *
     * When a STP is created, this list is empty, all ports state as blocking.
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_STP_PORT
     */
    SAI_STP_ATTR_PORT_LIST,

    /**
     * @brief End of attributes
     */
    SAI_STP_ATTR_END,

    /** Custom range base value */
    SAI_STP_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_stp_attr_t;

/**
 * @brief SAI attributes for STP
 */
typedef enum _sai_stp_port_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_STP_PORT_ATTR_START,

    /**
     * @brief STP id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_STP
     */
    SAI_STP_PORT_ATTR_STP = SAI_STP_PORT_ATTR_START,

    /**
     * @brief Bridge Port id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE_PORT
     */
    SAI_STP_PORT_ATTR_BRIDGE_PORT,

    /**
     * @brief STP port state
     *
     * @type sai_stp_port_state_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_STP_PORT_ATTR_STATE,

    /**
     * @brief End of attributes
     */
    SAI_STP_PORT_ATTR_END,

    /** Custom range base value */
    SAI_STP_PORT_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_stp_port_attr_t;

/**
 * @brief Create STP instance with default port state as blocking.
 *
 * @param[out] stp_id STP instance id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_create_stp_fn)(
        _Out_ sai_object_id_t *stp_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove STP instance.
 *
 * @param[in] stp_id STP instance id
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_remove_stp_fn)(
        _In_ sai_object_id_t stp_id);

/**
 * @brief Set the attribute of STP instance.
 *
 * @param[in] stp_id STP instance id
 * @param[in] attr Attribute value
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_set_stp_attribute_fn)(
        _In_ sai_object_id_t stp_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get the attribute of STP instance.
 *
 * @param[in] stp_id STP instance id
 * @param[in] attr_count Number of the attribute
 * @param[inout] attr_list Attribute value
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_get_stp_attribute_fn)(
        _In_ sai_object_id_t stp_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create STP port object
 *
 * @param[out] stp_port_id STP port id
 * @param[in] switch_id Switch ID
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_create_stp_port_fn)(
        _Out_ sai_object_id_t *stp_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove STP port object.
 *
 * @param[in] stp_port_id STP object id
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_remove_stp_port_fn)(
        _In_ sai_object_id_t stp_port_id);

/**
 * @brief Set the attribute of STP port.
 *
 * @param[in] stp_port_id STP port id
 * @param[in] attr Attribute value
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_set_stp_port_attribute_fn)(
        _In_ sai_object_id_t stp_port_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get the attribute of STP port.
 *
 * @param[in] stp_port_id STP port id
 * @param[in] attr_count Number of the attribute
 * @param[inout] attr_list Attribute value
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *    error code is returned.
 */
typedef sai_status_t (*sai_get_stp_port_attribute_fn)(
        _In_ sai_object_id_t stp_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief STP method table retrieved with sai_api_query()
 */
typedef struct _sai_stp_api_t
{
    sai_create_stp_fn               create_stp;
    sai_remove_stp_fn               remove_stp;
    sai_set_stp_attribute_fn        set_stp_attribute;
    sai_get_stp_attribute_fn        get_stp_attribute;
    sai_create_stp_port_fn          create_stp_port;
    sai_remove_stp_port_fn          remove_stp_port;
    sai_set_stp_port_attribute_fn   set_stp_port_attribute;
    sai_get_stp_port_attribute_fn   get_stp_port_attribute;
    sai_bulk_object_create_fn       create_stp_ports;
    sai_bulk_object_remove_fn       remove_stp_ports;
} sai_stp_api_t;

/**
 * @}
 */
#endif /** __SAISTP_H_ */
