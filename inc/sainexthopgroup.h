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
*    sainexthopgroup.h
*
* Abstract:
*
*    This module defines SAI Next Hop Group API
*
*/

#if !defined (__SAINEXTHOPGROUP_H_)
#define __SAINEXTHOPGROUP_H_

#include <saitypes.h>

/** \defgroup SAINEXTHOPGROUP SAI - Next hop group specific API definitions.
 *
 *  \{
 */
 
/**
 *  @brief Next hop group type
 */
typedef enum _sai_next_hop_group_type_t
{
    /** Next hop group is ECMP */
    SAI_NEXT_HOP_GROUP_ECMP,

    /** Other types of next hop group to be defined in the future, e.g., WCMP */

} sai_next_hop_group_type_t;


/**
 *  @brief Attribute id for next hop
 */
typedef enum _sai_next_hop_group_attr_t
{
    /** READ-ONLY */
    /** Number of next hops in the group [uint32_t] */
    SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_COUNT,

    /** READ-WRITE */

    /** Next hop group type [sai_next_hop_group_type_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_NEXT_HOP_GROUP_ATTR_TYPE,

    /** Next hop list [sai_object_list_t] (MAXDATORY_ON_CREATE) 
     * The next hop group must have at least one next hop member at the creation time */
    SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST,

    /* -- */

    /* Custom range base value */
    SAI_NEXT_HOP_GROUP_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_next_hop_group_attr_t;

/**
 * Routine Description:
 *    @brief Create next hop group
 *
 * Arguments:
 *    @param[out] next_hop_group_id - next hop group id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_create_next_hop_group_fn)(
    _Out_ sai_object_id_t* next_hop_group_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Remove next hop group 
 *
 * Arguments:
 *    @param[in] next_hop_group_id - next hop group id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_remove_next_hop_group_fn)(
    _In_ sai_object_id_t next_hop_group_id
    );

/**
 * Routine Description:
 *    @brief Set Next Hop Group attribute
 *
 * Arguments:
 *    @param[in] sai_object_id_t - next_hop_group_id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_next_hop_group_attribute_fn)(
    _In_ sai_object_id_t next_hop_group_id,
    _In_ const sai_attribute_t *attr
    );


/**
 * Routine Description:
 *    @brief Get Next Hop Group attribute
 *
 * Arguments:
 *    @param[in] sai_object_id_t - next_hop_group_id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_next_hop_group_attribute_fn)(
    _In_ sai_object_id_t next_hop_group_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Add next hop to a group 
 *
 * Arguments:
 *    @param[in] next_hop_group_id - next hop group id
 *    @param[in] next_hop_count - number of next hops
 *    @param[in] nexthops - array of next hops
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_add_next_hop_to_group_fn)(
    _In_ sai_object_id_t next_hop_group_id,
    _In_ uint32_t next_hop_count,
    _In_ const sai_object_id_t* nexthops
    );


/**
 * Routine Description:
 *    @brief Remove next hop from a group 
 *
 * Arguments:
 *    @param[in] next_hop_group_id - next hop group id
 *    @param[in] next_hop_count - number of next hops
 *    @param[in] nexthops - array of next hops
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_remove_next_hop_from_group_fn)(
    _In_ sai_object_id_t next_hop_group_id,
    _In_ uint32_t next_hop_count,
    _In_ const sai_object_id_t* nexthops
    );


/**
 *  @brief Next Hop methods table retrieved with sai_api_query()
 */
typedef struct _sai_next_hop_group_api_t
{
    sai_create_next_hop_group_fn        create_next_hop_group;
    sai_remove_next_hop_group_fn        remove_next_hop_group;
    sai_set_next_hop_group_attribute_fn set_next_hop_group_attribute;
    sai_get_next_hop_group_attribute_fn get_next_hop_group_attribute;
    sai_add_next_hop_to_group_fn        add_next_hop_to_group;
    sai_remove_next_hop_from_group_fn   remove_next_hop_from_group;

} sai_next_hop_group_api_t;

/**
 * \}
 */
#endif // __SAINEXTHOPGROUP_H_

