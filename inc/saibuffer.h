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
*    saibuffer.h
*
* Abstract:
*
*    This module defines SAI Buffer API
*
*/

#if !defined (__SAIBUFFER_H_)
#define __SAIBUFFER_H_

#include <saitypes.h>

/** \defgroup SAIBUFFER SAI - Buffer specific API definitions.
 *
 *  \{
 */

/**
 * @brief Enum defining ingress priority group attributes.
 */
typedef enum _sai_ingress_priority_group_attr_t
{
    /** buffer profile pointer [sai_object_id_t] */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE,
} sai_ingress_priority_group_attr_t;

/**
 * @brief Set ingress priority group attribute
 * @param[in] ingress_pg_id ingress priority group id
 * @param[in] attr attribute to set
 *
 * @return  SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t(*sai_set_ingress_priority_group_attr_fn)(
    _In_ sai_object_id_t ingress_pg_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief Get ingress priority group attributes
 * @param[in] ingress_pg_id ingress priority group id
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list array of attributes
 *
 * @return  SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t(*sai_get_ingress_priority_group_attr_fn)(
    _In_ sai_object_id_t ingress_pg_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * @brief Enum defining buffer pool types.
 */
typedef enum _sai_buffer_pool_type_t
{
    /** Ingress buffer pool */
    SAI_BUFFER_POOL_INGRESS,

    /** Egress buffer pool */
    SAI_BUFFER_POOL_EGRESS,

} sai_buffer_pool_type_t;

/**
 * @brief Enum defining buffer threshold modes
 */
typedef enum _sai_buffer_threshold_mode_t
{
    /** static maximum */
    SAI_BUFFER_THRESHOLD_MODE_STATIC,

    /** dynamic maximum (relative) */
    SAI_BUFFER_THRESHOLD_MODE_DYNAMIC,

} sai_buffer_threshold_mode_t;

/**
 * @brief Enum defining buffer pool attributes.
 */
typedef enum _sai_buffer_pool_attr_t
{
    /** READ-ONLY */

    /** shared buffer size in bytes [sai_uint32_t].
     * This is derived from substracting all reversed buffers of queue/port
     * from the total pool size. */
    SAI_BUFFER_POOL_ATTR_SHARED_SIZE,

    /** READ-WRITE */

    /** buffer pool type [sai_buffer_pool_type_t]  (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_BUFFER_POOL_ATTR_TYPE,

    /** buffer pool size in bytes [sai_uint32_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_BUFFER_POOL_ATTR_SIZE,

    /** shared threshold mode for the buffer pool [sai_buffer_threadhold_mode_t] (CREATE_AND_SET)
     * (default to SAI_BUFFER_POOL_DYNAMIC_TH) */
    SAI_BUFFER_POOL_ATTR_TH_MODE,

} sai_buffer_pool_attr_t;

/**
 * @brief Create buffer pool
 * @param[out] pool_id buffer pool id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t(*sai_create_buffer_pool_fn)(
    _Out_ sai_object_id_t* pool_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * @brief Remove buffer pool
 * @param[in] pool_id buffer pool id
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t(*sai_remove_buffer_pool_fn)(
    _In_ sai_object_id_t pool_id
    );

/**
 * @brief Set buffer pool attribute
 * @param[in] pool_id buffer pool id
 * @param[in] attr attribute
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t(*sai_set_buffer_pool_attr_fn)(
    _In_ sai_object_id_t pool_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief Get buffer pool attributes
 * @param[in] pool_id buffer pool id
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list array of attributes
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t(*sai_get_buffer_pool_attr_fn)(
    _In_ sai_object_id_t pool_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * @brief Enum defining buffer profile attributes.
 */
typedef enum _sai_buffer_profile_attr_t
{
    /** READ-WRITE */

    /** pointer to buffer pool object id [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_AND_SET)  */
    SAI_BUFFER_PROFILE_ATTR_POOL_ID,

    /** reserved buffer size in bytes [sai_uint32_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE,

    /** dynamic threshold for the shared usage [sai_int8_t]
     * The threshold is set to the 2^n of available buffer of the pool.
     * Mandatory when SAI_BUFFER_POOL_TH_MODE = SAI_BUFFER_THRESHOLD_MODE_DYNAMIC
     * (CREATE_AND_SET). */
    SAI_BUFFER_PROFILE_ATTR_SHARED_DYNAMIC_TH,

    /** static threshold for the shared usage in bytes [sai_uint32_t]
     * Mandatory when SAI_BUFFER_POOL_TH_MODE = SAI_BUFFER_THRESHOLD_MODE_STATIC
     * (CREATE_AND_SET) */
    SAI_BUFFER_PROFILE_ATTR_SHARED_STATIC_TH,

    /** set the buffer profile XOFF threshold in bytes [sai_uint32_t]
     * Valid only for ingress PG (CREATE_AND_SET).
     * Generate XOFF when available buffer in the PG buffer
     * is less than this threshold.
     * default to 0. */
    SAI_BUFFER_PROFILE_ATTR_XOFF_TH,

    /** set the buffer profile XON threshold in byte [sai_uint32_t]
     * Valid only for ingress PG (CREATE_AND_SET)
     * Generate XON when the total buffer usage of this PG
     * is less this threshold and available buffer in the PG buffer
     * is larger than the XOFF threahold.
     * default to 0. */
    SAI_BUFFER_PROFILE_ATTR_XON_TH,

} sai_buffer_profile_attr_t;

/**
 * @brief Create buffer profile
 * @param[out] buffer_profile_id buffer profile id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t(*sai_create_buffer_profile_fn)(
    _Out_ sai_object_id_t* buffer_profile_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * @brief Remove buffer profile
 * @param[in] buffer_profile_id buffer profile id
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t(*sai_remove_buffer_profile_fn)(
    _In_ sai_object_id_t buffer_profile_id
    );

/**
 * @brief Set buffer profile attribute
 * @param[in] buffer_profile_id buffer profile id
 * @param[in] attr attribute
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t(*sai_set_buffer_profile_attr_fn)(
    _In_ sai_object_id_t buffer_profile_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief Get buffer profile attributes
 * @param[in] buffer_profile_id buffer profile id
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list array of attributes
 * @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t(*sai_get_buffer_profile_attr_fn)(
    _In_ sai_object_id_t buffer_profile_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 *  @brief buffer methods table retrieved with sai_api_query()
 */
typedef struct _sai_buffer_api_t
{
    sai_create_buffer_pool_fn              create_buffer_pool;
    sai_remove_buffer_pool_fn              remove_buffer_pool;
    sai_set_buffer_pool_attr_fn            set_buffer_pool_attr;
    sai_get_buffer_pool_attr_fn            get_buffer_pool_attr;
    sai_set_ingress_priority_group_attr_fn set_ingress_priority_group_attr;
    sai_get_ingress_priority_group_attr_fn get_ingress_priority_group_attr;
    sai_create_buffer_profile_fn           create_buffer_profile;
    sai_remove_buffer_profile_fn           remove_buffer_profile;
    sai_set_buffer_profile_attr_fn         set_buffer_profile_attr;
    sai_get_buffer_profile_attr_fn         get_buffer_profile_attr;
} sai_buffer_api_t;

/**
 *\}
 */

#endif // __SAIBUFFER_H_
