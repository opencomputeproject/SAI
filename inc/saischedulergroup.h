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
* @file saischedulergroup.h
*
* @brief This file contains Qos Scheduler functionality.
************************************************************************/

#if !defined (__SAISCHEDULER_GROUP_H_)
#define __SAISCHEDULER_GROUP_H_

#include "saitypes.h"

/** \defgroup SAISCHEDULERGROUP SAI - Qos scheduler group specific API definitions.
 *
 *  \{
 */

/**
 * @brief Enum defining scheduler group attributes.
 */
typedef enum _sai_scheduler_group_attr_t
{
    /** READ-ONLY */

    /** Maximum Number of childs on group [uint32_t] */
    SAI_SCHEDULER_GROUP_ATTR_MAX_SUPPORTED_CHILDS = 0x00000000,

    /** Number of queues/groups childs added to
     * scheduler group [uint32_t] */
    SAI_SCHEDULER_GROUP_ATTR_CHILD_COUNT = 0x00000001,

    /** Scheduler Group child obejct id List [sai_object_list_t] */
    SAI_SCHEDULER_GROUP_ATTR_CHILD_LIST = 0x00000002,

    /** READ-WRITE */

    /** Scheduler group on port [sai_object_id_t]
       MANDATORY_ON_CREATE,  CREATE_ONLY */
    SAI_SCHEDULER_GROUP_ATTR_PORT_ID = 0x00000003,

    /** Scheduler group level [sai_uint8_t]
       MANDATORY_ON_CREATE,  CREATE_ONLY */
    SAI_SCHEDULER_GROUP_ATTR_LEVEL = 0x00000004,

    /** Scheucler ID [sai_object_id_t]
     * SET_ONLY */
    SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID = 0x00000005,

    /* -- */
    /* Custom range base value */
    SAI_SCHEDULER_GROUP_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_scheduler_group_attr_t;


/**
 * @brief  Create Scheduler group
 *
 * @param[out] scheduler_group_id Scheudler group id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
typedef sai_status_t (*sai_create_scheduler_group_fn)(
    _Out_ sai_object_id_t  *scheduler_group_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * @brief  Remove Scheduler group
 *
 * @param[in] scheduler_group_id Scheudler group id
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
typedef sai_status_t (*sai_remove_scheduler_group_fn)(
    _In_ sai_object_id_t scheduler_group_id
    );


/**
 * @brief  Set Scheduler group Attribute
 *
 * @param[in] scheduler_group_id Scheudler group id
 * @param[in] attr attribute to set
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
typedef sai_status_t (*sai_set_scheduler_group_attribute_fn)(
    _In_ sai_object_id_t scheduler_group_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief  Get Scheduler Group attribute
 *
 * @param[in] scheduler_group_id - scheduler group id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success
 *        Failure status code on error
 */

typedef sai_status_t (*sai_get_scheduler_group_attribute_fn)(
    _In_ sai_object_id_t scheduler_group_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * @brief   Add Child queue/group objects to scheduler group
 *
 * @param[in] scheduler_group_id Scheduler group id.
 * @param[in] child_count number of child count
 * @param[in] child_objects array of child objects
 *
 * @return SAI_STATUS_SUCCESS on success
 *        Failure status code on error
 */
typedef sai_status_t (*sai_add_child_object_to_group_fn)(
    _In_ sai_object_id_t scheduler_group_id,
    _In_ uint32_t        child_count,
    _In_ const sai_object_id_t* child_objects
    );


/**
 * @brief   Remove Child queue/group objects from scheduler group
 *
 * @param[in] scheduler_group_id Scheduler group id.
 * @param[in] child_count number of child count
 * @param[in] child_objects array of child objects
 *
 * @return SAI_STATUS_SUCCESS on success
 *        Failure status code on error
 */
typedef sai_status_t (*sai_remove_child_object_from_group_fn)(
    _In_ sai_object_id_t scheduler_group_id,
    _In_ uint32_t        child_count,
    _In_ const sai_object_id_t* child_objects
    );

/**
 * @brief  Scheduler Group methods table retrieved with sai_api_query()
 */
typedef struct _sai_scheduler_group_api_t
{
    sai_create_scheduler_group_fn          create_scheduler_group;
    sai_remove_scheduler_group_fn          remove_scheduler_group;
    sai_set_scheduler_group_attribute_fn   set_scheduler_group_attribute;
    sai_get_scheduler_group_attribute_fn   get_scheduler_group_attribute;
    sai_add_child_object_to_group_fn       add_child_object_to_group;
    sai_remove_child_object_from_group_fn  remove_child_object_from_group;

} sai_scheduler_group_api_t;


/**
 * \}
 */

#endif
