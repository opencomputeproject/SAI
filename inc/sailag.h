/*
* Copyright (c) 2015 Dell Inc.
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
*/
/**
* \file    sailag.h
*
* \brief   This module defines SAI LAG interface
*/

#if !defined (__SAILAG_H_)
#define __SAILAG_H_

#include <saitypes.h>

/** \defgroup SAILAG SAI - LAG specific API definitions.
 *
 *  \{
 */

/** Lag attribute: List of attributes for LAG object */
typedef enum _sai_lag_attr_t {

    /** READ-ONLY */

    /** SAI port list [sai_object_list_t] */
    SAI_LAG_ATTR_PORT_LIST,

    /** READ_WRITE */

    /** Custom range base value */
    SAI_LAG_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_lag_attr_t;

/*
    \brief Create LAG
    \param[out] lag_id LAG id
    \param[in] attr_count number of attributes
    \param[in] attr_list array of attributes
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/
typedef sai_status_t(*sai_create_lag_fn)(
    _Out_ sai_object_id_t* lag_id,
    _In_ uint32_t attr_count,
    _In_ sai_attribute_t *attr_list
    );

/*
    \brief Remove LAG
    \param[in] lag_id LAG id
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/
typedef sai_status_t(*sai_remove_lag_fn)(
    _In_ sai_object_id_t  lag_id
    );

/*
    \brief Set LAG Attribute
    \param[in] lag_id LAG id
    \param[in] attr Structure containing ID and value to be set
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/
typedef sai_status_t (*sai_set_lag_attribute_fn)(
    _In_ sai_object_id_t  lag_id,
    _In_ const sai_attribute_t *attr
    );

/*
    \brief Get LAG Attribute
    \param[in] lag_id LAG id
    \param[in] attr_count Number of attributes to be get
    \param[in,out] attr_list List of structures containing ID and value to be get
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/

typedef sai_status_t (*sai_get_lag_attribute_fn)(
    _In_ sai_object_id_t lag_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );


/**
 *  @brief List of LAG member attributes
 */
typedef enum _sai_lag_member_attr_t {

    /** READ_WRITE */

    /* LAG ID [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_LAG_MEMBER_ATTR_LAG_ID,

    /* logical port ID [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_LAG_MEMBER_ATTR_PORT_ID,

     /* Disable traffic distribution to this port as part of LAG. [bool] (CREATE_AND_SET) default to FALSE */
    SAI_LAG_MEMBER_ATTR_EGRESS_DISABLE,

     /* Disable traffic collection from this port as part of LAG. [bool] (CREATE_AND_SET) default to FALSE. */
    SAI_LAG_MEMBER_ATTR_INGRESS_DISABLE,

    /** Custom range base value */
    SAI_LAG_MEMBER_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_lag_member_attr_t;

/*
    \brief Create LAG Member
    \param[out] lag_member_id LAG Member id
    \param[in] attr_count number of attributes
    \param[in] attr_list array of attributes
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/
typedef sai_status_t(*sai_create_lag_member_fn)(
    _Out_ sai_object_id_t* lag_member_id,
    _In_ uint32_t attr_count,
    _In_ sai_attribute_t *attr_list
    );

/*
    \brief Remove LAG Member
    \param[in] lag_member_id LAG Member id
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/
typedef sai_status_t(*sai_remove_lag_member_fn)(
    _In_ sai_object_id_t  lag_member_id
    );


/*
    \brief Set LAG Member Attribute
    \param[in] lag_member_id LAG Member id
    \param[in] attr Structure containing ID and value to be set
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/
typedef sai_status_t (*sai_set_lag_member_attribute_fn)(
    _In_ sai_object_id_t  lag_member_id,
    _In_ const sai_attribute_t *attr
    );

/*
    \brief Get LAG Member Attribute
    \param[in] lag_member_id LAG Member id
    \param[in] attr_count Number of attributes to be get
    \param[in,out] attr_list List of structures containing ID and value to be get
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/

typedef sai_status_t (*sai_get_lag_member_attribute_fn)(
    _In_ sai_object_id_t lag_member_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * @brief LAG methods table retrieved with sai_api_query()
 */
typedef struct _sai_lag_api_t
{
   sai_create_lag_fn                create_lag;
   sai_remove_lag_fn                remove_lag;
   sai_set_lag_attribute_fn         set_lag_attribute;
   sai_get_lag_attribute_fn         get_lag_attribute;
   sai_create_lag_member_fn         create_lag_member;
   sai_remove_lag_member_fn         remove_lag_member;
   sai_set_lag_member_attribute_fn  set_lag_member_attribute;
   sai_get_lag_member_attribute_fn  get_lag_member_attribute;
} sai_lag_api_t;

/**
 * \}
 */
#endif
