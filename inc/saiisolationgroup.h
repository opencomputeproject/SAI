/**
 * Copyright (c) 2018 Microsoft Open Technologies, Inc.
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
 * @file    saiisolationgroup.h
 *
 * @brief   This module defines SAI Isolation Group interface
 */

#if !defined (__SAIISOLATIONGROUP_H_)
#define __SAIISOLATIONGROUP_H_

#include <saitypes.h>

/**
 * @defgroup SAIISOLATIONGROUP SAI - Isolation group specific API definitions
 *
 * @{
 */

/**
 * @brief Isolation group type
 */
typedef enum _sai_isolation_group_type_t
{
    /** Isolation group consists of ports. */
    SAI_ISOLATION_GROUP_TYPE_PORT,

    /** Isolation group consists of bridge ports. */
    SAI_ISOLATION_GROUP_TYPE_BRIDGE_PORT,

} sai_isolation_group_type_t;

/**
 * @brief Attributes for isolation group
 */
typedef enum _sai_isolation_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_ISOLATION_GROUP_ATTR_START,

    /**
     * @brief Isolation group type
     *
     * @type sai_isolation_group_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_ISOLATION_GROUP_ATTR_TYPE = SAI_ISOLATION_GROUP_ATTR_START,

    /**
     * @brief Isolation group member list
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_ISOLATION_GROUP_MEMBER
     */
    SAI_ISOLATION_GROUP_ATTR_ISOLATION_MEMBER_LIST,

    /**
     * @brief End of attributes
     */
    SAI_ISOLATION_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_ISOLATION_GROUP_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_isolation_group_attr_t;

typedef enum _sai_isolation_group_member_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_ISOLATION_GROUP_MEMBER_ATTR_START,

    /**
     * @brief Isolation group id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_ISOLATION_GROUP
     */
    SAI_ISOLATION_GROUP_MEMBER_ATTR_ISOLATION_GROUP_ID = SAI_ISOLATION_GROUP_MEMBER_ATTR_START,

    /**
     * @brief Isolation group member object
     *
     * If the isolation group type is SAI_ISOLATION_GROUP_TYPE_PORT,
     * then the object should be of type SAI_OBJECT_TYPE_PORT. If the
     * isolation group type is SAI_ISOLATION_GROUP_TYPE_BRIDGE_PORT,
     * then the object should be of type SAI_OBJECT_TYPE_BRIDGE_PORT.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_BRIDGE_PORT
     */
    SAI_ISOLATION_GROUP_MEMBER_ATTR_ISOLATION_OBJECT,

    /**
     * @brief End of attributes
     */
    SAI_ISOLATION_GROUP_MEMBER_ATTR_END,

    /** Custom range base value */
    SAI_ISOLATION_GROUP_MEMBER_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_isolation_group_member_attr_t;

/**
 * @brief Create isolation group
 *
 * @param[out] isolation_group_id Isolation group id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_isolation_group_fn)(
        _Out_ sai_object_id_t *isolation_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove isolation group
 *
 * @param[in] isolation_group_id Isolation group id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_isolation_group_fn)(
        _In_ sai_object_id_t isolation_group_id);

/**
 * @brief Set isolation group attribute
 *
 * @param[in] isolation_group_id Isolation group id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_isolation_group_attribute_fn)(
        _In_ sai_object_id_t isolation_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get isolation group attribute
 *
 * @param[in] isolation_group_id Isolation group id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_isolation_group_attribute_fn)(
        _In_ sai_object_id_t isolation_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create isolation group member
 *
 * @param[out] isolation_group_member_id Isolation group member id
 * @param[in] switch_id Switch ID
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_isolation_group_member_fn)(
        _Out_ sai_object_id_t *isolation_group_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove isolation group member
 *
 * @param[in] isolation_group_member_id Isolation group member id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_isolation_group_member_fn)(
        _In_ sai_object_id_t isolation_group_member_id);

/**
 * @brief Set isolation group member attribute
 *
 * @param[in] isolation_group_member_id Isolation group member id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_isolation_group_member_attribute_fn)(
        _In_ sai_object_id_t isolation_group_member_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get isolation group member attribute
 *
 * @param[in] isolation_group_member_id Isolation group member id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_isolation_group_member_attribute_fn)(
        _In_ sai_object_id_t isolation_group_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Isolation group method table retrieved with sai_api_query()
 */
typedef struct _sai_isolation_group_api_t
{
    sai_create_isolation_group_fn                   create_isolation_group;
    sai_remove_isolation_group_fn                   remove_isolation_group;
    sai_set_isolation_group_attribute_fn            set_isolation_group_attribute;
    sai_get_isolation_group_attribute_fn            get_isolation_group_attribute;
    sai_create_isolation_group_member_fn            create_isolation_group_member;
    sai_remove_isolation_group_member_fn            remove_isolation_group_member;
    sai_set_isolation_group_member_attribute_fn     set_isolation_group_member_attribute;
    sai_get_isolation_group_member_attribute_fn     get_isolation_group_member_attribute;

} sai_isolation_group_api_t;

/**
 * @}
 */
#endif /** __SAIISOLATIONGROUP_H_ */
