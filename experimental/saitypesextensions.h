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

    /* Add new experimental object types above this line */

    SAI_OBJECT_TYPE_EXTENSIONS_RANGE_END

} sai_object_type_extensions_t;

/**
 * @extraparam sai_object_type_t object_type
 */
typedef union _sai_object_key_entry_extension_t
{
    /**
     * @brief Key is object ID.
     *
     * @validonly sai_metadata_is_object_type_oid(object_type) == true
     */
    sai_object_id_t           object_id;

    /* Add new experimental entries above this line */

} sai_object_key_entry_extension_t;

/**
 * @brief Structure for bulk retrieval of object ids, attribute and values for
 * each object-type. Key will be used in case of object-types not having
 * object-ids.
 *
 * @extraparam sai_object_type_t object_type
 */
typedef struct _sai_object_key_extension_t
{
    /** @passparam object_type */
    sai_object_key_entry_extension_t key;

} sai_object_key_extension_t;

#endif /* __SAITYPESEXTENSIONS_H_ */

