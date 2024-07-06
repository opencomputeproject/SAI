/**
 * Copyright (c) 20XX Microsoft Open Technologies, Inc.
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
 * @file    sail2ecmpgroup.h
 *
 * @brief   This module defines SAI L2 ECMP GROUP interface
 */

#if !defined (__SAIL2ECMPGROUP_H_)
#define __SAIL2ECMPGROUP_H_

#include <saitypes.h>

/**
 * @defgroup SAIL2ECMPGROUP SAI - L2 ECMP GROUP specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute id for L2 ECMP GROUP
 */
typedef enum _sai_l2_ecmp_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_L2_ECMP_GROUP_ATTR_START,

    /**
     * @brief Number of L2 ECMP GROUP members in the group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_L2_ECMP_GROUP_ATTR_MEMBER_COUNT = SAI_L2_ECMP_GROUP_ATTR_START,

    /**
     * @brief L2 ECMP GROUP member list
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_L2_ECMP_GROUP_MEMBER
     */
    SAI_L2_ECMP_GROUP_ATTR_MEMBER_LIST,

    /**
     * @brief Attach a counter
     *
     * When it is empty, then packet hits won't be counted
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_L2_ECMP_GROUP_ATTR_COUNTER_ID,

    /**
     * @brief End of attributes
     */
    SAI_L2_ECMP_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_L2_ECMP_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_L2_ECMP_GROUP_ATTR_CUSTOM_RANGE_END

} sai_l2_ecmp_group_attr_t;

typedef enum _sai_l2_ecmp_group_member_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_L2_ECMP_GROUP_MEMBER_ATTR_START,

    /**
     * @brief L2 ECMP GROUP id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_L2_ECMP_GROUP
     */
    SAI_L2_ECMP_GROUP_MEMBER_ATTR_L2_ECMP_GROUP_ID = SAI_L2_ECMP_GROUP_MEMBER_ATTR_START,

    /**
     * @brief P2P Tunnel oid
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL
     */
    SAI_L2_ECMP_GROUP_MEMBER_ATTR_TUNNEL_ID,

    /**
     * @brief End of attributes
     */
    SAI_L2_ECMP_GROUP_MEMBER_ATTR_END,

    /** Custom range base value */
    SAI_L2_ECMP_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_L2_ECMP_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END

} sai_l2_ecmp_group_member_attr_t;

/**
 * @brief Create L2 ECMP group
 *
 * @param[out] l2_ecmp_group_id L2 ECMP group id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_l2_ecmp_group_fn)(
        _Out_ sai_object_id_t *l2_ecmp_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove L2 ECMP group
 *
 * @param[in] l2_ecmp_group_id L2 ECMP group id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_l2_ecmp_group_fn)(
        _In_ sai_object_id_t l2_ecmp_group_id);

/**
 * @brief Set L2 ECMP Group attribute
 *
 * @param[in] l2_ecmp_group_id L2 ECMP group id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_l2_ecmp_group_attribute_fn)(
        _In_ sai_object_id_t l2_ecmp_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get L2 ECMP Group attribute
 *
 * @param[in] l2_ecmp_group_id L2 ECMP group ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_l2_ecmp_group_attribute_fn)(
        _In_ sai_object_id_t l2_ecmp_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create L2 ECMP group member
 *
 * @param[out] l2_ecmp_group_member_id L2 ECMP group member id
 * @param[in] switch_id Switch ID
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_l2_ecmp_group_member_fn)(
        _Out_ sai_object_id_t *l2_ecmp_group_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove L2 ECMP group member
 *
 * @param[in] l2_ecmp_group_member_id L2 ECMP group member ID
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_l2_ecmp_group_member_fn)(
        _In_ sai_object_id_t l2_ecmp_group_member_id);

/**
 * @brief Set L2 ECMP Group member attribute
 *
 * @param[in] l2_ecmp_group_member_id L2 ECMP group member ID
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_l2_ecmp_group_member_attribute_fn)(
        _In_ sai_object_id_t l2_ecmp_group_member_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get L2 ECMP Group member attribute
 *
 * @param[in] l2_ecmp_group_member_id L2 ECMP group member ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_l2_ecmp_group_member_attribute_fn)(
        _In_ sai_object_id_t l2_ecmp_group_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_l2_ecmp_group_api_t
{
    sai_create_l2_ecmp_group_fn               create_l2_ecmp_group;
    sai_remove_l2_ecmp_group_fn               remove_l2_ecmp_group;
    sai_set_l2_ecmp_group_attribute_fn        set_l2_ecmp_group_attribute;
    sai_get_l2_ecmp_group_attribute_fn        get_l2_ecmp_group_attribute;
    sai_create_l2_ecmp_group_member_fn        create_l2_ecmp_group_member;
    sai_remove_l2_ecmp_group_member_fn        remove_l2_ecmp_group_member;
    sai_set_l2_ecmp_group_member_attribute_fn set_l2_ecmp_group_member_attribute;
    sai_get_l2_ecmp_group_member_attribute_fn get_l2_ecmp_group_member_attribute;
} sai_l2_ecmp_group_api_t;

/**
 * @}
 */
#endif /** __SAIL2ECMPGROUP_H_ */
