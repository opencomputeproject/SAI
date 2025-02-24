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
 * @file    sail2mcgroup.h
 *
 * @brief   This module defines SAI L2MC Group interface
 */

#if !defined (__SAIL2MCGROUP_H_)
#define __SAIL2MCGROUP_H_

#include <saitypes.h>

/**
 * @defgroup SAIL2MCGROUP SAI - L2MC group specific API definitions
 *
 * @{
 */

/**
 * @brief Attributes for L2MC group
 */
typedef enum _sai_l2mc_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_L2MC_GROUP_ATTR_START,

    /**
     * @brief Number of L2MC output in the group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_L2MC_GROUP_ATTR_L2MC_OUTPUT_COUNT = SAI_L2MC_GROUP_ATTR_START,

    /**
     * @brief L2MC member list
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP_MEMBER
     */
    SAI_L2MC_GROUP_ATTR_L2MC_MEMBER_LIST,

    /**
     * @brief L2MC group id
     *
     * This attribute only takes effect when the value(type is SAI_OBJECT_TYPE_L2MC_GROUP) is not equal to SAI_NULL_OBJECT_ID
     * @type sai_uint64_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_L2MC_GROUP_ATTR_L2MC_GROUP_ID,

    /**
     * @brief End of attributes
     */
    SAI_L2MC_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_L2MC_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_L2MC_GROUP_ATTR_CUSTOM_RANGE_END

} sai_l2mc_group_attr_t;

typedef enum _sai_l2mc_group_member_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_L2MC_GROUP_MEMBER_ATTR_START,

    /**
     * @brief L2MC group id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     */
    SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_GROUP_ID = SAI_L2MC_GROUP_MEMBER_ATTR_START,

    /**
     * @brief L2MC output id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE_PORT
     */
    SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_OUTPUT_ID,

    /**
     * @brief L2MC Tunnel Endpoint IP.Valid and mandatory if bridge port
     * is of type SAI_BRIDGE_PORT_TYPE_TUNNEL.
     *
     * @type sai_ip_address_t
     * @flags CREATE_ONLY
     * @default 0.0.0.0
     */
    SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_ENDPOINT_IP,

    /**
     * @brief End of attributes
     */
    SAI_L2MC_GROUP_MEMBER_ATTR_END,

    /** Custom range base value */
    SAI_L2MC_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_L2MC_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END

} sai_l2mc_group_member_attr_t;

/**
 * @brief Create L2MC group
 *
 * There is no problem with sharing member group mechanism in L2MC
 * entry,but we suggest to use 1:1 mechanism because of the
 * convenience of the L2MC member to be updated
 *
 * If there is no SAI_L2MC_GROUP_ATTR_L2MC_GROUP_ID in attribute list,
 * or the value of SAI_L2MC_GROUP_ATTR_L2MC_GROUP_ID attribute is zero,
 * It means that the upper layer user(usually SONIC) wants to create
 * a new L2MC group object id. If there is SAI_L2MC_GROUP_ATTR_L2MC_GROUP_ID
 * in attribute list, the expected behavior is that the sai_create_l2mc_group_fn
 * reuse the L2MC group object id
 *
 * @param[out] l2mc_group_id L2MC group id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_l2mc_group_fn)(
        _Out_ sai_object_id_t *l2mc_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove L2MC group
 *
 * @param[in] l2mc_group_id L2MC group id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_l2mc_group_fn)(
        _In_ sai_object_id_t l2mc_group_id);

/**
 * @brief Set L2MC Group attribute
 *
 * @param[in] l2mc_group_id L2MC group id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_l2mc_group_attribute_fn)(
        _In_ sai_object_id_t l2mc_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get L2MC Group attribute
 *
 * @param[in] l2mc_group_id L2MC group id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_l2mc_group_attribute_fn)(
        _In_ sai_object_id_t l2mc_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create L2MC group member
 *
 * @param[out] l2mc_group_member_id L2MC group member id
 * @param[in] switch_id Switch ID
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_l2mc_group_member_fn)(
        _Out_ sai_object_id_t *l2mc_group_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove L2MC group member
 *
 * @param[in] l2mc_group_member_id L2MC group member id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_l2mc_group_member_fn)(
        _In_ sai_object_id_t l2mc_group_member_id);

/**
 * @brief Set L2MC Group attribute
 *
 * @param[in] l2mc_group_member_id L2MC group member id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_l2mc_group_member_attribute_fn)(
        _In_ sai_object_id_t l2mc_group_member_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get L2MC Group attribute
 *
 * @param[in] l2mc_group_member_id L2MC group member id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_l2mc_group_member_attribute_fn)(
        _In_ sai_object_id_t l2mc_group_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief L2MC group methods table retrieved with sai_api_query()
 */
typedef struct _sai_l2mc_group_api_t
{
    sai_create_l2mc_group_fn                   create_l2mc_group;
    sai_remove_l2mc_group_fn                   remove_l2mc_group;
    sai_set_l2mc_group_attribute_fn            set_l2mc_group_attribute;
    sai_get_l2mc_group_attribute_fn            get_l2mc_group_attribute;
    sai_create_l2mc_group_member_fn            create_l2mc_group_member;
    sai_remove_l2mc_group_member_fn            remove_l2mc_group_member;
    sai_set_l2mc_group_member_attribute_fn     set_l2mc_group_member_attribute;
    sai_get_l2mc_group_member_attribute_fn     get_l2mc_group_member_attribute;

} sai_l2mc_group_api_t;

/**
 * @}
 */
#endif /** __SAIL2MCGROUP_H_ */
