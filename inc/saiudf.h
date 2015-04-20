/*
 * Copyright (c) 2014 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
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
 * Module Name:
 *
 *    saiudf.h
 *
 * Abstract:
 *
 *    This module defines SAI UDF (User Defined Field)
 *
 */

#if !defined (__SAIUDF_H_)
#define __SAIUDF_H_

#include <saitypes.h>

/** \defgroup SAIUDF SAI - User Defined Field specific API definitions.
 *
 *  \{
 */
 
/**
 * @brief UDF base enum
 */
typedef enum _sai_udf_base_t
{
    /** UDF offset base L2 header */
    SAI_UDF_BASE_L2,

    /** UDF offset base L3 header */
    SAI_UDF_BASE_L3,

    /** UDF offset base L4 header */
    SAI_UDF_BASE_L4,

} sai_udf_base_t;

/**
 * @breif Attribute id for UDF
 */
typedef enum _sai_udf_attr_t
{
    /** READ-ONLY */

    /** READ-WRITE */

    /** UDF match ID [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_UDF_ATTR_MATCH_ID,

    /** UDF group id [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_UDF_ATTR_GROUP_ID,

    /** UDF base [sai_udf_base_t] (CREATE_AND_SET) (default to SAI_UDF_BASE_L2) */
    SAI_UDF_ATTR_BASE,

    /** UDF byte offset [uint16_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_UDF_ATTR_OFFSET,

    /** UDF Mask [sai_u8_list_t](CREATE_AND_SET) (default to 2 bytes, value 0xFF, 0xFF)
     * The count in the list must be equal to the UDF byte length.
     * The mask only applies to extracted UDF when it is used for hash,
     * it does not apply to the extracted UDF when it is used for ACL.  */
    SAI_UDF_ATTR_HASH_MASK

} sai_udf_attr_t;

/**
 * @brief Attribute id for UDF match
 */
typedef enum _sai_udf_match_attr_t
{
    /** READ-ONLY */

    /** READ-WRITE */

    /** UDF L2 match rule [sai_acl_field_data_t(uint16_t)] (CREATE_ONLY) (default to None) */
    SAI_UDF_MATCH_ATTR_L2_TYPE,

    /** UDF L3 match rule [sai_acl_field_data_t(uint8_t)] (CREATE_ONLY) (default to None) */
    SAI_UDF_MATCH_ATTR_L3_TYPE,

    /** UDF GRE match rule [sai_acl_field_data_t(uint16_t)] (CREATE_ONLY) (default to None) */
    SAI_UDF_MATCH_ATTR_GRE_TYPE,

    /** UDF match priority [uint8_t] (CREATE_ONLY) (default to 0) */
    SAI_UDF_MATCH_ATTR_PRIORITY

} sai_udf_match_attr_t;

/**
 * @brief UDF group type.
 */
typedef enum _sai_udf_group_type_t
{
    /** Generic UDF group */
    SAI_UDF_GROUP_GENERIC,

    /** UDF group for hash */
    SAI_UDF_GROUP_HASH,

} sai_udf_group_type_t;

/**
 * @brief Attribute id for UDF group
 */
typedef enum _sai_udf_group_attr_t
{
    /** READ-ONLY */

    /** UDF list [sai_object_list_t] */
    SAI_UDF_GROUP_ATTR_UDF_LIST,

    /** READ-WRITE */

    /** UDF group type [sai_udf_group_type_t] (CREATE_ONLY) (default to SAI_UDF_GENERIC) */
    SAI_UDF_GROUP_ATTR_TYPE,

    /** UDF byte length [uint16_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_UDF_ATTR_LENGTH,

} sai_udf_group_attr_t;

/**
 * Routine Description:
 *    @brief Create UDF
 *
 * Arguments:
 *    @param[out] udf_id - UDF id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 *
 */
typedef sai_status_t (*sai_create_udf_fn)(
    _Out_ sai_object_id_t* udf_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Remove UDF
 *
 * Arguments:
 *    @param[in] udf_id - UDF id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_remove_udf_fn)(
    _In_ sai_object_id_t udf_id
    );

/**
 * Routine Description:
 *    @brief Set UDF attribute
 *
 * Arguments:
 *    @param[in] udf_id - UDF id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_udf_attribute_fn)(
    _In_ sai_object_id_t udf_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * Routine Description:
 *    @brief Get UDF attribute value
 *
 * Arguments:
 *    @param[in] udf_id - UDF id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attrs - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_udf_attribute_fn)(
    _In_ sai_object_id_t udf_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Create UDF match
 *
 * Arguments:
 *    @param[out] udf_match_id - UDF match id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 *
 */
typedef sai_status_t (*sai_create_udf_match_fn)(
    _Out_ sai_object_id_t* udf_match_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Remove UDF match
 *
 * Arguments:
 *    @param[in] udf_match_id - UDF match id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_remove_udf_match_fn)(
    _In_ sai_object_id_t udf_match_id
    );

/**
 * Routine Description:
 *    @brief Set UDF match attribute
 *
 * Arguments:
 *    @param[in] udf_match_id - UDF match id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_udf_match_attribute_fn)(
    _In_ sai_object_id_t udf_match_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * Routine Description:
 *    @brief Get UDF match attribute value
 *
 * Arguments:
 *    @param[in] udf_match_id - UDF match id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attrs - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_udf_match_attribute_fn)(
    _In_ sai_object_id_t udf_match_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Create UDF group
 *
 * Arguments:
 *    @param[out] udf_group_id - UDF group id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 *
 */
typedef sai_status_t (*sai_create_udf_group_fn)(
    _Out_ sai_object_id_t* udf_group_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Remove UDF group
 *
 * Arguments:
 *    @param[in] udf_group_id - UDF group id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_remove_udf_group_fn)(
    _In_ sai_object_id_t udf_group_id
    );

/**
 * Routine Description:
 *    @brief Set UDF group attribute
 *
 * Arguments:
 *    @param[in] udf_group_id - UDF group id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_udf_group_attribute_fn)(
    _In_ sai_object_id_t udf_group_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * Routine Description:
 *    @brief Get UDF group attribute value
 *
 * Arguments:
 *    @param[in] udf_group_id - UDF group id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attrs - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_udf_group_attribute_fn)(
    _In_ sai_object_id_t udf_group_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 *  @brief UDF methods, retrieved via sai_api_query()
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
 * \}
 */
#endif // __SAIUDF_H_
