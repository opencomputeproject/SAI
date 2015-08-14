/*
* Copyright (c) 2015 Dell Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License"); you may
* not use this file except in compliance with the License. You may obtain
* a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
* THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
* CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
* LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
* FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
*
* See the Apache Version 2.0 License for specific language governing
* permissions and limitations under the License.
*
* @file saiwred.h
*
* @brief This file contains Qos Wred functionality.
************************************************************************/


#if !defined (__SAIWRED_H_)
#define __SAIWRED_H_

#include "saitypes.h"

/** \defgroup SAIWRED SAI - Qos Wred specific API definitions.
 *
 *  \{
 */


/**
 * @brief Enum defining WRED profile attributes
 */
typedef enum _sai_wred_attr_t
{
    /** [bool] enable/disable, Default FALSE*/
    SAI_WRED_ATTR_GREEN_ENABLE = 0x00000000,

    /**
     * bytes [ sai_uint32_t],  MANDATORY for SAI_WRED_ATTR_GREEN_ENABLE = TRUE
     * Range 1 - Max Buffer size.
     */
    SAI_WRED_ATTR_GREEN_MIN_THRESHOLD = 0x00000001,

    /**
     * bytes  [sai_uint32_t], MANDATORY for SAI_WRED_ATTR_GREEN_ENABLE = TRUE
     * Range 1 - Max Buffer size.
     */
    SAI_WRED_ATTR_GREEN_MAX_THRESHOLD = 0x00000002,

    /** Percentage 0 ~ 100 [sai_uint32_t], Default 100%*/
    SAI_WRED_ATTR_GREEN_DROP_PROBABILITY = 0x00000003,

    /** [bool] enable/disable , Default FALSE */
    SAI_WRED_ATTR_YELLOW_ENABLE = 0x00000004,

    /**
     * bytes [ sai_uint32_t], MANDATORY for SAI_WRED_ATTR_YELLOW_ENABLE = TRUE
     * Range 1 - Max Buffer size.
     */
    SAI_WRED_ATTR_YELLOW_MIN_THRESHOLD = 0x00000005,

    /**
     * bytes [sai_uint32_t], MANDATORY for SAI_WRED_ATTR_YELLOW_ENABLE = TRUE
     * Range 1 - Max Buffer size.
     */
    SAI_WRED_ATTR_YELLOW_MAX_THRESHOLD = 0x00000006,

    /** Percentage 0 ~ 100 [sai_uint32_t], Default 100% */
    SAI_WRED_ATTR_YELLOW_DROP_PROBABILITY = 0x00000007,

     /** [bool] enable/disable , Default FALSE*/
    SAI_WRED_ATTR_RED_ENABLE = 0x00000008,

    /**
     * bytes [ sai_uint32_t] , MANDATORY for SAI_WRED_ATTR_RED_ENABLE = TRUE
     * Range 1 - Max Buffer size.
     */
    SAI_WRED_ATTR_RED_MIN_THRESHOLD = 0x00000009,

    /**
     * bytes [ sai_uint32_t] , MANDATORY for SAI_WRED_ATTR_RED_ENABLE = TRUE
     * Range 1 - Max Buffer size.
     */
    SAI_WRED_ATTR_RED_MAX_THRESHOLD = 0x0000000a,

    /** Percentage 0 ~ 100 [sai_uint32_t], Default 100%*/
    SAI_WRED_ATTR_RED_DROP_PROBABILITY = 0x0000000b,

    /** 0 ~ 15 [sai_uint8_t], Default 0*/
    SAI_WRED_ATTR_WEIGHT = 0x0000000c,

    /** [bool] enable/disable ECN marking, Default is FALSE */
    SAI_WRED_ATTR_ECN_MARK_ENABLE = 0x0000000d,

    /** -- */
    /** Custom range base value */
    SAI_WRED_ATTR_CUSTOM_RANGE_BASE = 0x10000000
} sai_wred_attr_t;



/**
 * @brief Create WRED Profile
 *
 * @param[out] wred_id - Wred profile Id.
 * @param[in] attr_count - number of attributes
 * @param[in] attr_list - array of attributes
 *
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */

typedef sai_status_t (*sai_create_wred_fn)(
    _Out_ sai_object_id_t *wred_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * @brief Remove WRED Profile
 *
 * @param[in] wred_id Wred profile Id.
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_remove_wred_fn)(
    _In_ sai_object_id_t  wred_id
    );


/**
 * @brief Set attributes to Wred profile.
 *
 * @param[out] wred_id Wred profile Id.
 * @param[in] attr attribute
 *
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */

typedef sai_status_t (*sai_set_wred_attribute_fn)(
    _In_ sai_object_id_t wred_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief  Get Wred profile attribute
 *
 * @param[in] wred_id Wred Profile Id
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list  array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success
 *        Failure status code on error
 */
typedef sai_status_t (*sai_get_wred_attribute_fn)(
    _In_ sai_object_id_t wred_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
   );

/**
 * @brief WRED methods table retrieved with sai_api_query()
 */
typedef struct _sai_wred_api_t
{
    sai_create_wred_fn          create_wred_profile;
    sai_remove_wred_fn          remove_wred_profile;
    sai_set_wred_attribute_fn   set_wred_attribute;
    sai_get_wred_attribute_fn   get_wred_attribute;
} sai_wred_api_t;

/**
 *\}
 */

#endif

