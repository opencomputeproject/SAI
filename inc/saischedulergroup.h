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
 * @file    saischedulergroup.h
 *
 * @brief   This module defines SAI QOS Scheduler Group interface
 */

#if !defined (__SAISCHEDULER_GROUP_H_)
#define __SAISCHEDULER_GROUP_H_

#include <saitypes.h>

/**
 * @defgroup SAISCHEDULERGROUP SAI - Qos scheduler group specific API definitions
 *
 * @{
 */

/**
 * @brief Enum defining scheduler group attributes.
 */
typedef enum _sai_scheduler_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SCHEDULER_GROUP_ATTR_START = 0x00000000,

    /**
     * @brief Number of queues/groups childs added to
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SCHEDULER_GROUP_ATTR_CHILD_COUNT = SAI_SCHEDULER_GROUP_ATTR_START,

    /**
     * @brief Scheduler Group child obejct id list
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_SCHEDULER
     * @flags READ_ONLY
     */
    SAI_SCHEDULER_GROUP_ATTR_CHILD_LIST = 0x00000001,

    /**
     * @brief Scheduler group on port
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_PORT
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_SCHEDULER_GROUP_ATTR_PORT_ID = 0x00000002,

    /**
     * @brief Scheduler group level
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_SCHEDULER_GROUP_ATTR_LEVEL = 0x00000003,

    /**
     * @brief Maximum Number of childs on group
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_SCHEDULER_GROUP_ATTR_MAX_CHILDS = 0x00000004,

    /**
     * @brief Scheucler ID
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_SCHEDULER
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID = 0x00000005,

    /**
     * @brief Scheduler group parent node
     *
     * This is conditional when the level > 0, when level == 0, the parent is the port.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_SCHEDULER_GROUP
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_SCHEDULER_GROUP_ATTR_PARENT_NODE = 0x00000006,

    /**
     * @brief End of attributes
     */
    SAI_SCHEDULER_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_SCHEDULER_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SCHEDULER_GROUP_ATTR_CUSTOM_RANGE_END

} sai_scheduler_group_attr_t;

/**
 * @brief Create Scheduler group
 *
 * @param[out] scheduler_group_id Scheudler group id
 * @param[in] switch_id The Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_scheduler_group_fn)(
        _Out_ sai_object_id_t *scheduler_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Scheduler group
 *
 * @param[in] scheduler_group_id Scheudler group id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_scheduler_group_fn)(
        _In_ sai_object_id_t scheduler_group_id);

/**
 * @brief Set Scheduler group Attribute
 *
 * @param[in] scheduler_group_id Scheudler group id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_scheduler_group_attribute_fn)(
        _In_ sai_object_id_t scheduler_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Scheduler Group attribute
 *
 * @param[in] scheduler_group_id Scheduler group id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_scheduler_group_attribute_fn)(
        _In_ sai_object_id_t scheduler_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Scheduler Group methods table retrieved with sai_api_query()
 */
typedef struct _sai_scheduler_group_api_t
{
    sai_create_scheduler_group_fn          create_scheduler_group;
    sai_remove_scheduler_group_fn          remove_scheduler_group;
    sai_set_scheduler_group_attribute_fn   set_scheduler_group_attribute;
    sai_get_scheduler_group_attribute_fn   get_scheduler_group_attribute;

} sai_scheduler_group_api_t;

/**
 * @}
 */
#endif /** __SAISCHEDULER_GROUP_H_ */
