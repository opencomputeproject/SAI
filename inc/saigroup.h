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
*    saigroup.h
*
* Abstract:
*
*    This module defines generic SAI object Group API
*
*/

#if !defined (__SAIGROUP_H_)
#define __SAIGROUP_H_

#include <saitypes.h>

/** \defgroup SAIGROUP SAI - Generic object group specific API definitions.
 *
 *  \{
 */
 
/**
 *  @brief Generic object group type
 */
typedef enum _sai_group_type_t
{
    /** Group members are ECMP nexthops */
    SAI_GROUP_TYPE_ECMP,

    /** Group members are outputs of a L2 multicast group */
    SAI_GROUP_TYPE_L2MC,

    /** Group members are outputs of a L3 multicast group */
    SAI_GROUP_TYPE_IPMC,

    /** Group members are RPF interfaces of a L3 multicast group */
    SAI_GROUP_TYPE_RPF,

    /** Other types of group to be defined in the future, e.g., WCMP */

} sai_group_type_t;

/**
 *  @brief Attribute id for generic group
 */
typedef enum _sai_group_attr_t
{
    SAI_GROUP_ATTR_START,

    /** READ-ONLY */

    /** Number of members in the group [uint32_t] */
    SAI_GROUP_ATTR_MEMBER_COUNT = SAI_GROUP_ATTR_START,

    /** Member list [sai_object_list_t] */
    SAI_GROUP_ATTR_MEMBER_LIST,

    /** READ-WRITE */

    /** Generic group type [sai_group_type_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_GROUP_ATTR_TYPE,

    SAI_GROUP_ATTR_END,

    /* Custom range base value */
    SAI_GROUP_ATTR_CUSTOM_RANGE_BASE  = 0x10000000,

    /* --*/
    SAI_GROUP_ATTR_CUSTOM_RANGE_END

} sai_group_attr_t;

typedef enum _sai_group_member_attr_t 
{
    SAI_GROUP_MEMBER_ATTR_START,

    /** READ_WRITE */
    /** Generic Group ID [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_GROUP_MEMBER_ATTR_GROUP_ID = SAI_GROUP_MEMBER_ATTR_START,

    /** Group Member Object ID [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) 
      * When group type is ECMP, the member should be a nexthop object 
      * When group type is L2MC, the member should be a port/LAG/... object */
    SAI_GROUP_MEMBER_ATTR_MEMBER_ID,

    /** Member Weights [sai_uint32_t] (CREATE_AND_SET)
     *  Applicable when group type is ECMP (default to 1) */
    SAI_GROUP_MEMBER_ATTR_WEIGHT,

    SAI_GROUP_MEMBER_ATTR_END,

    /** custom range base value */
    SAI_GROUP_MEMBER_ATTR_CUSTOM_RANGE_BASE  = 0x10000000,

    /* --*/
    SAI_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END

} sai_group_member_attr_t;

/**
 * Routine Description:
 *    @brief Create generic group
 *
 * Arguments:
 *    @param[out] group_id - generic group id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_create_group_fn)(
    _Out_ sai_object_id_t* group_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Remove generic group 
 *
 * Arguments:
 *    @param[in] group_id - generic group id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_remove_group_fn)(
    _In_ sai_object_id_t group_id
    );

/**
 * Routine Description:
 *    @brief Set Generic Group attribute
 *
 * Arguments:
 *    @param[in] sai_object_id_t - generic group id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_group_attribute_fn)(
    _In_ sai_object_id_t group_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * Routine Description:
 *    @brief Get Generic Group attribute
 *
 * Arguments:
 *    @param[in] sai_object_id_t - group_id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_group_attribute_fn)(
    _In_ sai_object_id_t group_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Create generic group member
 *
 * Arguments:
 *    @param[out] group_member_id - generic group member id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_create_group_member_fn)(
    _Out_ sai_object_id_t* group_member_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Remove generic group member
 *
 * Arguments:
 *    @param[in] group_member_id - generic group member id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_remove_group_member_fn)(
    _In_ sai_object_id_t group_member_id
    );

/**
 * Routine Description:
 *    @brief Set Generic Group Member attribute
 *
 * Arguments:
 *    @param[in] sai_object_id_t - group_member_id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_group_member_attribute_fn)(
    _In_ sai_object_id_t group_member_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * Routine Description:
 *    @brief Get Generic Group Member attribute
 *
 * Arguments:
 *    @param[in] sai_object_id_t - group_member_id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_group_member_attribute_fn)(
    _In_ sai_object_id_t group_member_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 *  @brief Generic group methods table retrieved with sai_api_query()
 */
typedef struct _sai_group_api_t
{
    sai_create_group_fn               create_group;
    sai_remove_group_fn               remove_group;
    sai_set_group_attribute_fn        set_group_attribute;
    sai_get_group_attribute_fn        get_group_attribute;
    sai_create_group_member_fn        create_group_member;
    sai_remove_group_member_fn        remove_group_member;
    sai_set_group_member_attribute_fn set_next_member_attribute;
    sai_get_group_member_attribute_fn get_next_member_attribute;

} sai_group_api_t;

/**
 * \}
 */
#endif // __SAIGROUP_H_
