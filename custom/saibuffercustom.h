/**
 * Copyright (c) 2025 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 * @file    saibuffercustom.h
 *
 * @brief   This module defines SAI Buffer custom interface
 */

#if !defined (__SAIBUFFER_CUSTOM_H_)
#define __SAIBUFFER_CUSTOM_H_

#include <saibuffer.h>

/**
 * @brief Custom buffer pool statistics
 *
 * @flags free
 */
typedef enum _sai_buffer_pool_stat_custom_t
{
    /**
     * @brief Custom range start of Buffer Pool statistics attributes
     */
    SAI_BUFFER_POOL_STAT_CUSTOM_RANGE_START = SAI_BUFFER_POOL_STAT_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of Buffer Pool statistics attributes
     */
    SAI_BUFFER_POOL_STAT_CUSTOM_RANGE_END

} sai_buffer_pool_stat_custom_t;

/**
 * @brief Custom ingress priority group attributes
 *
 * @flags free
 */
typedef enum _sai_ingress_priority_group_attr_custom_t
{
    /**
     * @brief Custom range start of Ingress Priority Group attributes
     */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_CUSTOM_RANGE_START = SAI_INGRESS_PRIORITY_GROUP_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of Ingress Priority Group attributes
     */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_CUSTOM_RANGE_END

} sai_ingress_priority_group_attr_custom_t;

/**
 * @brief Custom buffer pool attributes
 *
 * @flags free
 */
typedef enum _sai_buffer_pool_attr_custom_t
{
    /**
     * @brief Custom range start of Buffer Pool attributes
     */
    SAI_BUFFER_POOL_ATTR_CUSTOM_RANGE_START = SAI_BUFFER_POOL_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of Buffer Pool attributes
     */
    SAI_BUFFER_POOL_ATTR_CUSTOM_RANGE_END

} sai_buffer_pool_attr_custom_t;

/**
 * @brief Custom buffer profile attributes
 *
 * @flags free
 */
typedef enum _sai_buffer_profile_attr_custom_t
{
    /**
     * @brief Custom range start of Buffer Profile attributes
     */
    SAI_BUFFER_PROFILE_ATTR_CUSTOM_RANGE_START = SAI_BUFFER_PROFILE_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of Buffer Profile attributes
     */
    SAI_BUFFER_PROFILE_ATTR_CUSTOM_RANGE_END

} sai_buffer_profile_attr_custom_t;

#endif /** __SAIBUFFER_CUSTOM_H_ */