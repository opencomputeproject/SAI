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
 * @file    saitypesextensions.h
 *
 * @brief   This module defines type extensions of the Switch Abstraction Interface (SAI)
 */

#ifndef __SAITYPESEXTENSIONS_H_
#define __SAITYPESEXTENSIONS_H_

#include <saitypes.h>

/**
 * @brief SAI object type extensions
 *
 * @flags free
 */
typedef enum _sai_object_type_extensions_t
{
    SAI_OBJECT_TYPE_EXTENSIONS_RANGE_START = SAI_OBJECT_TYPE_MAX,

    SAI_OBJECT_TYPE_TABLE_BITMAP_CLASSIFICATION_ENTRY = SAI_OBJECT_TYPE_EXTENSIONS_RANGE_START,

    SAI_OBJECT_TYPE_TABLE_BITMAP_ROUTER_ENTRY,

    SAI_OBJECT_TYPE_TABLE_META_TUNNEL_ENTRY,

    SAI_OBJECT_TYPE_DIRECTION_LOOKUP_ENTRY,

    SAI_OBJECT_TYPE_ENI_LOOKUP_TO_VM_ENTRY,

    SAI_OBJECT_TYPE_INBOUND_ROUTING_ENTRY,

    SAI_OBJECT_TYPE_PA_VALIDATION_ENTRY,

    SAI_OBJECT_TYPE_OUTBOUND_ENI_LOOKUP_FROM_VM_ENTRY,

    SAI_OBJECT_TYPE_OUTBOUND_ENI_TO_VNI_ENTRY,

    SAI_OBJECT_TYPE_DASH_ACL,

    SAI_OBJECT_TYPE_OUTBOUND_ROUTING_ENTRY,

    SAI_OBJECT_TYPE_OUTBOUND_CA_TO_PA_ENTRY,

    SAI_OBJECT_TYPE_INBOUND_ENI_TO_VM_ENTRY,

    SAI_OBJECT_TYPE_INBOUND_VM,

    /* Add new experimental object types above this line */

    SAI_OBJECT_TYPE_EXTENSIONS_RANGE_END

} sai_object_type_extensions_t;

#endif /* __SAITYPESEXTENSIONS_H_ */

