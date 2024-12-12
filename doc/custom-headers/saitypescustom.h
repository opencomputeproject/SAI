/**
 * Copyright (c) 2018 Microsoft Open Technologies, Inc.
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
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
 *
 * @file    saitypescustom.h
 *
 * @brief   This module defines type custom of the Switch Abstraction Interface (SAI)
 */

#ifndef __SAITYPESCUSTOM_H_
#define __SAITYPESCUSTOM_H_

#include <saitypes.h>

/**
 * @brief SAI object type custom
 *
 * @flags free
 */
typedef enum _sai_object_type_custom_t
{
    SAI_OBJECT_TYPE_CUSTOM_RANGE_START = SAI_OBJECT_TYPE_CUSTOM_RANGE_BASE,

    SAI_OBJECT_TYPE_ONE = SAI_OBJECT_TYPE_CUSTOM_RANGE_START,

    /* Add new custom object types above this line */

    SAI_OBJECT_TYPE_CUSTOM_RANGE_END

} sai_object_type_custom_t;

typedef enum _sai_some_new_type_t
{
    SAI_SOME_NEW_TYPE_A,

    SAI_SOME_NEW_TYPE_B,

} sai_some_new_type_t;

#endif /* __SAITYPESCUSTOM_H_ */

