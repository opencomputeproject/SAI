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
 * @file    saiudf.h
 *
 * @brief   This module defines SAI UDF (User Defined Field) interface
 */

#if !defined (__SAIUDF_H_)
#define __SAIUDF_H_

#include <saitypes.h>

/**
 * @defgroup SAIUDF SAI - User Defined Field specific API definitions
 *
 * @{
 */

/**
 * @brief UDF base enum
 */
typedef enum _sai_udf_base_t
{
    /** UDF offset base from the start of L2 header */
    SAI_UDF_BASE_L2,

    /** UDF offset base from the start of L3 header */
    SAI_UDF_BASE_L3,

    /** UDF offset base from the start of L4 header */
    SAI_UDF_BASE_L4,

} sai_udf_base_t;

/**
 * @brief Attribute id for UDF
 */
typedef enum _sai_udf_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_UDF_ATTR_START,

    /**
     * @brief UDF match ID
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_UDF_MATCH
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_UDF_ATTR_MATCH_ID = SAI_UDF_ATTR_START,

    /**
     * @brief UDF group id
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_UDF_GROUP
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_UDF_ATTR_GROUP_ID,

    /**
     * @brief UDF base
     *
     * @type sai_udf_base_t
     * @flags CREATE_AND_SET
     * @default SAI_UDF_BASE_L2
     */
    SAI_UDF_ATTR_BASE,

    /**
     * @brief UDF byte offset
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_UDF_ATTR_OFFSET,

    /**
     * @brief UDF Mask
     *
     * Default to 2 bytes, value 0xFF, 0xFF
     *
     * The count in the list must be equal to the UDF byte length.
     * The mask only applies to extracted UDF when it is used for hash,
     * it does not apply to the extracted UDF when it is used for ACL.
     *
     * @type sai_u8_list_t
     * @flags CREATE_AND_SET
     * @default const
     */
    SAI_UDF_ATTR_HASH_MASK,

    /**
     * @brief End of attributes
     */
    SAI_UDF_ATTR_END,

} sai_udf_attr_t;

/**
 * @brief Attribute id for UDF match
 */
typedef enum _sai_udf_match_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_UDF_MATCH_ATTR_START,

    /**
     * @brief UDF L2 match rule
     *
     * Default to None
     *
     * @type sai_acl_field_data_t sai_uint16_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_UDF_MATCH_ATTR_L2_TYPE = SAI_UDF_MATCH_ATTR_START,

    /**
     * @brief UDF L3 match rule
     *
     * Default to None
     *
     * @type sai_acl_field_data_t sai_uint8_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_UDF_MATCH_ATTR_L3_TYPE,

    /**
     * @brief UDF GRE match rule
     *
     * Default to None
     *
     * @type sai_acl_field_data_t sai_uint16_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_UDF_MATCH_ATTR_GRE_TYPE,

    /**
     * @brief UDF match priority
     *
     * @type sai_uint8_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_UDF_MATCH_ATTR_PRIORITY,

    /**
     * @brief End of attributes
     */
    SAI_UDF_MATCH_ATTR_END,

} sai_udf_match_attr_t;

/**
 * @brief UDF group type.
 */
typedef enum _sai_udf_group_type_t
{
    /** Start of group type */
    SAI_UDF_GROUP_TYPE_START,

    /** Generic UDF group */
    SAI_UDF_GROUP_TYPE_GENERIC = SAI_UDF_GROUP_TYPE_START,

    /** UDF group for hash */
    SAI_UDF_GROUP_TYPE_HASH,

    /** End of group type */
    SAI_UDF_GROUP_TYPE_END

} sai_udf_group_type_t;

/**
 * @brief Attribute id for UDF group
 */
typedef enum _sai_udf_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_UDF_GROUP_ATTR_START,

    /**
     * @brief UDF list
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_UDF
     * @flags READ_ONLY
     */
    SAI_UDF_GROUP_ATTR_UDF_LIST = SAI_UDF_GROUP_ATTR_START,

    /**
     * @brief UDF group type
     *
     * @type sai_udf_group_type_t
     * @flags CREATE_ONLY
     * @default SAI_UDF_GROUP_TYPE_GENERIC
     */
    SAI_UDF_GROUP_ATTR_TYPE,

    /**
     * @brief UDF byte length
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_UDF_GROUP_ATTR_LENGTH,

    /**
     * @brief End of attributes
     */
    SAI_UDF_GROUP_ATTR_END,

} sai_udf_group_attr_t;

/**
 * @brief Create UDF
 *
 * @param[out] udf_id UDF id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Aarray of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_udf_fn)(
        _Out_ sai_object_id_t *udf_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove UDF
 *
 * @param[in] udf_id UDF id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_udf_fn)(
        _In_ sai_object_id_t udf_id);

/**
 * @brief Set UDF attribute
 *
 * @param[in] udf_id UDF id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_udf_attribute_fn)(
        _In_ sai_object_id_t udf_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get UDF attribute value
 *
 * @param[in] udf_id UDF id
 * @param[in] attr_count number of attributes
 * @param[inout] attrs -rray of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_udf_attribute_fn)(
        _In_ sai_object_id_t udf_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create UDF match
 *
 * @param[out] udf_match_id UDF match id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_udf_match_fn)(
        _Out_ sai_object_id_t *udf_match_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove UDF match
 *
 * @param[in] udf_match_id UDF match id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_udf_match_fn)(
        _In_ sai_object_id_t udf_match_id);

/**
 * @brief Set UDF match attribute
 *
 * @param[in] udf_match_id UDF match id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_udf_match_attribute_fn)(
        _In_ sai_object_id_t udf_match_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get UDF match attribute value
 *
 * @param[in] udf_match_id UDF match id
 * @param[in] attr_count Number of attributes
 * @param[inout] attrs Aarray of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_udf_match_attribute_fn)(
        _In_ sai_object_id_t udf_match_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create UDF group
 *
 * @param[out] udf_group_id UDF group id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_udf_group_fn)(
        _Out_ sai_object_id_t *udf_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove UDF group
 *
 * @param[in] udf_group_id UDF group id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_udf_group_fn)(
        _In_ sai_object_id_t udf_group_id);

/**
 * @brief Set UDF group attribute
 *
 * @param[in] udf_group_id UDF group id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_udf_group_attribute_fn)(
        _In_ sai_object_id_t udf_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get UDF group attribute value
 *
 * @param[in] udf_group_id UDF group id
 * @param[in] attr_count Number of attributes
 * @param[inout] attrs Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_udf_group_attribute_fn)(
        _In_ sai_object_id_t udf_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief UDF methods, retrieved via sai_api_query()
 */
typedef struct _sai_udf_api_t
{
    sai_create_udf_fn               create_udf;
    sai_remove_udf_fn               remove_udf;
    sai_set_udf_attribute_fn        set_udf_attribute;
    sai_get_udf_attribute_fn        get_udf_attribute;
    sai_create_udf_match_fn         create_udf_match;
    sai_remove_udf_match_fn         remove_udf_match;
    sai_set_udf_match_attribute_fn  set_udf_match_attribute;
    sai_get_udf_match_attribute_fn  get_udf_match_attribute;
    sai_create_udf_group_fn         create_udf_group;
    sai_remove_udf_group_fn         remove_udf_group;
    sai_set_udf_group_attribute_fn  set_udf_group_attribute;
    sai_get_udf_group_attribute_fn  get_udf_group_attribute;

} sai_udf_api_t;

/**
 * @}
 */
#endif /** __SAIUDF_H_ */
