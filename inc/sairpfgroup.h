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
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc
 *
 * @file    sairpfgroup.h
 *
 * @brief   This module defines SAI RFP Group interface
 */

#if !defined (__SAIRPFGROUP_H_)
#define __SAIRPFGROUP_H_

#include <saitypes.h>

/**
 * @defgroup SAIRPFGROUP SAI - RPF group specific API definitions
 *
 * @{
 */

/**
 * @brief Attributes for RPF interface group
 */
typedef enum _sai_rpf_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_RPF_GROUP_ATTR_START,

    /**
     * @brief Number of RPF interfaces in the group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_RPF_GROUP_ATTR_RPF_INTERFACE_COUNT = SAI_RPF_GROUP_ATTR_START,

    /**
     * @brief RPF member list
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_RPF_GROUP_MEMBER
     * @flags READ_ONLY
     */
    SAI_RPF_GROUP_ATTR_RPF_MEMBER_LIST,

    /**
     * @brief End of attributes
     */
    SAI_RPF_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_RPF_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_RPF_GROUP_ATTR_CUSTOM_RANGE_END

} sai_rpf_group_attr_t;

typedef enum _sai_rpf_group_member_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_RPF_GROUP_MEMBER_ATTR_START,

    /**
     * @brief RPF interface group id
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_RPF_GROUP
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_RPF_GROUP_MEMBER_ATTR_RPF_GROUP_ID = SAI_RPF_GROUP_MEMBER_ATTR_START,

    /**
     * @brief RPF interface id
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_RPF_GROUP_MEMBER_ATTR_RPF_INTERFACE_ID,

    /**
     * @brief End of attributes
     */
    SAI_RPF_GROUP_MEMBER_ATTR_END,

    /** Custom range base value */
    SAI_RPF_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START  = 0x10000000,

    /** End of custom range base */
    SAI_RPF_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END

} sai_rpf_group_member_attr_t;

/**
 * @brief Create RPF interface group
 *
 * @param[out] rpf_group_id RPF interface group id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_rpf_group_fn)(
        _Out_ sai_object_id_t *rpf_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove RPF interface group
 *
 * @param[in] rpf_group_id RPF interface group id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_rpf_group_fn)(
        _In_ sai_object_id_t rpf_group_id);

/**
 * @brief Set RPF interface Group attribute
 *
 * @param[in] sai_object_id_t RPF interface group id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_rpf_group_attribute_fn)(
        _In_ sai_object_id_t rpf_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get RPF interface Group attribute
 *
 * @param[in] sai_object_id_t RPF interface group id
 * @param[in] attr_count -Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_rpf_group_attribute_fn)(
        _In_ sai_object_id_t rpf_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create RPF interface group member
 *
 * @param[out] rpf_group_member_id - RPF interface group member id
 * @param[in] attr_count - number of attributes
 * @param[in] attr_list - array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_rpf_group_member_fn)(
    _Out_ sai_object_id_t* rpf_group_member_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * @brief Remove RPF interface group member
 *
 * @param[in] rpf_group_member_id - RPF interface group member id
 *
 * @return SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_rpf_group_member_fn)(
    _In_ sai_object_id_t rpf_group_member_id
    );

/**
 * @brief Set RPF interface Group attribute
 *
 * @param[in] sai_object_id_t - RPF interface group member id
 * @param[in] attr - attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_rpf_group_member_attribute_fn)(
    _In_ sai_object_id_t rpf_group_member_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief Get RPF interface Group attribute
 *
 * @param[in] sai_object_id_t - rpf_group_member_id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_rpf_group_member_attribute_fn)(
    _In_ sai_object_id_t rpf_group_member_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 *  @brief RPF group methods table retrieved with sai_api_query()
 */
typedef struct _sai_rpf_group_api_t
{
    sai_create_rpf_group_fn                    create_rpf_group;
    sai_remove_rpf_group_fn                    remove_rpf_group;
    sai_set_rpf_group_attribute_fn             set_rpf_group_attribute;
    sai_get_rpf_group_attribute_fn             get_rpf_group_attribute;
    sai_create_rpf_group_member_fn             create_rpf_group_member;
    sai_remove_rpf_group_member_fn             remove_rpf_group_member;
    sai_set_rpf_group_member_attribute_fn      set_rpf_group_member_attribute;
    sai_get_rpf_group_member_attribute_fn      get_rpf_group_member_attribute;

} sai_rpf_group_api_t;

/**
 * @}
 */
#endif /** __SAIRPFGROUP_H_ */
