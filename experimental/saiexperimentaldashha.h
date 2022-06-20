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
 * @file    saiexperimentaldashha.h
 *
 * @brief   This module defines SAI P4 extension  interface
 */

#if !defined (__SAIEXPERIMENTALDASHHA_H_)
#define __SAIEXPERIMENTALDASHHA_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALDASHHA SAI - Extension specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_DASH_HA_SESSION_ATTR_ROLE
 */
typedef enum _sai_dash_ha_session_role_t
{
    /**
     * @brief This device accepts new connections if session is down
     */
    SAI_DASH_HA_SESSION_ROLE_ACTIVE,

    /**
     * @brief This device doesn't accept new connections if session is down
     */
    SAI_DASH_HA_SESSION_ROLE_BACKUP,

} sai_dash_ha_session_role_t;

/**
 * @brief Attribute ID for DASH HA session
 */
typedef enum _sai_dash_ha_session_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DASH_HA_SESSION_ATTR_START,

    /**
     * @brief HA session administrative state
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_DASH_HA_SESSION_ATTR_ADMIN_STATE = SAI_DASH_HA_SESSION_ATTR_START,

    /**
     * @brief Peer's IP address
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_HA_SESSION_ATTR_PEER_IP,

    /**
     * @brief Device role
     *
     * @type sai_dash_ha_session_role_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_HA_SESSION_ROLE_ACTIVE
     */
    SAI_DASH_HA_SESSION_ATTR_ROLE,

    /**
     * @brief List of ENI to sync
     *
     * @type sai_object_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_ENI
     * @allownull false
     */
    SAI_DASH_HA_SESSION_ATTR_ENI_OBJECT_ID_LIST,

    /**
     * @brief Operational state of the session sync
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_DASH_HA_SESSION_ATTR_OPER_STATE,

    /**
     * @brief Sync message batch size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1
     */
    SAI_DASH_HA_SESSION_ATTR_BATCH_SIZE,

    /**
     * @brief Sync latency
     *
     * the greatest latency allowed before a newly
     * learned connection is sent to a peer if batch is not full.
     * Value is in microseconds.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_DASH_HA_SESSION_ATTR_BATCH_TIMEOUT,

    /**
     * @brief End of attributes
     */
    SAI_DASH_HA_SESSION_ATTR_END,

    /** Custom range base value */
    SAI_DASH_HA_SESSION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DASH_HA_SESSION_ATTR_CUSTOM_RANGE_END,

} sai_dash_ha_session_attr_t;

/**
 * @brief Create DASH HA session
 *
 * @param[out] dash_ha_session_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_dash_ha_session_fn)(
        _Out_ sai_object_id_t *dash_ha_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove DASH HA session
 *
 * @param[in] dash_ha_session_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_dash_ha_session_fn)(
        _In_ sai_object_id_t dash_ha_session_id);

/**
 * @brief Set attribute for DASH HA session
 *
 * @param[in] dash_ha_session_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_dash_ha_session_attribute_fn)(
        _In_ sai_object_id_t dash_ha_session_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for DASH HA session
 *
 * @param[in] dash_ha_session_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_dash_ha_session_attribute_fn)(
        _In_ sai_object_id_t dash_ha_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_dash_ha_api_t
{
    sai_create_dash_ha_session_fn           create_dash_ha_session;
    sai_remove_dash_ha_session_fn           remove_dash_ha_session;
    sai_set_dash_ha_session_attribute_fn    set_dash_ha_session_attribute;
    sai_get_dash_ha_session_attribute_fn    get_dash_ha_session_attribute;

} sai_dash_ha_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHHA_H_ */
