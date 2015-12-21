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
 *    saihash.h
 *
 * Abstract:
 *
 *    This module defines SAI Hash API
 *
 */

#if !defined (__SAIHASH_H_)
#define __SAIHASH_H_

#include <saitypes.h>

/** \defgroup SAIHASH SAI - Hash specific API definitions.
 *
 *  \{
 */

/**
* brief@ Attribute data for sai native hash fields
*/
typedef enum _sai_native_hash_field
{
    /** Native hash field source IP.
     *  also refers to the outer source IP
     *  in case for encapsulated packets */
    SAI_NATIVE_HASH_FIELD_SRC_IP = 0,

    /** Native hash field destination IP
     *  also refers to the outer source IP
     *  in case for encapsulated packets */
    SAI_NATIVE_HASH_FIELD_DST_IP = 1,

    /** Native hash field inner source IP */
    SAI_NATIVE_HASH_FIELD_INNER_SRC_IP = 2,

    /** Native hash field inner destination IP */
    SAI_NATIVE_HASH_FIELD_INNER_DST_IP = 3,

    /** Native hash field vlan id */
    SAI_NATIVE_HASH_FIELD_VLAN_ID = 4,

    /** Native hash field IP protocol */
    SAI_NATIVE_HASH_FIELD_IP_PROTOCOL = 5,

    /** Native hash field ethernet type */
    SAI_NATIVE_HASH_FIELD_ETHERTYPE = 6,

    /** Native hash field L4 source port */
    SAI_NATIVE_HASH_FIELD_L4_SRC_PORT = 7,

    /** Native hash field L4 destination port */
    SAI_NATIVE_HASH_FIELD_L4_DST_PORT = 8,

    /** Native hash field source MAC */
    SAI_NATIVE_HASH_FIELD_SRC_MAC = 9,

    /** Native hash field destination MAC */
    SAI_NATIVE_HASH_FIELD_DST_MAC = 10,

    /** Native hash field source port */
    SAI_NATIVE_HASH_FIELD_IN_PORT = 11,

} sai_native_hash_field_t;

/**
 * @brief Hash attribute IDs
 */
typedef enum _sai_hash_attr_t
{
    /** READ-ONLY */

    /** READ-WRITE */

    /** Hash native fields [sai_s32_list_t(sai_native_hash_field)] (CREATE_AND_SET) (default to an empty list) */
    SAI_HASH_NATIVE_FIELD_LIST,

    /** Hash UDF group [sai_object_list_t(sai_udf_group_t)] (CREATE_AND_SET) (default to an empty list) */
    SAI_HASH_UDF_GROUP_LIST

} sai_hash_attr_t;

/**
 * Routine Description:
 *    @brief Create hash
 *
 * Arguments:
 *    @param[out] hash_id - hash id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 *
 */
typedef sai_status_t(*sai_create_hash_fn)(
    _Out_ sai_object_id_t* hash_id,
    _In_ uint32_t attr_count,
    _In_ sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Remove hash
 *
 * Arguments:
 *    @param[in] hash_id - hash id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t(*sai_remove_hash_fn)(
    _In_ sai_object_id_t hash_id
    );

/**
 * Routine Description:
 *    @brief Set hash attribute
 *
 * Arguments:
 *    @param[in] hash_id - hash id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_hash_attribute_fn)(
    _In_ sai_object_id_t hash_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * Routine Description:
 *    @brief Get hash attribute value
 *
 * Arguments:
 *    @param[in] hash_id - hash id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attrs - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_hash_attribute_fn)(
    _In_ sai_object_id_t hash_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 *  @brief hash methods, retrieved via sai_api_query()
 */
typedef struct _sai_hash_api_t
{
    sai_create_hash_fn          create_hash;
    sai_remove_hash_fn          remove_hash;
    sai_set_hash_attribute_fn   set_hash_attribute;
    sai_get_hash_attribute_fn   get_hash_attribute;

} sai_hash_api_t;

/**
 * \}
 */
#endif  // __SAIHASH_H_
