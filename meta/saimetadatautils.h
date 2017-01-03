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
 * @file    saimetadatautils.h
 *
 * @brief   This module defines SAI Metadata Utils
 */

#ifndef __SAI_METADATA_UTILS_H__
#define __SAI_METADATA_UTILS_H__

#include "saimetadatatypes.h"

/**
 * @defgroup SAIMETADATAUTILS SAI Metadata Utils Definitions
 *
 * @{
 */

/**
 * @brief Is allowed object type
 *
 * @param[in] metadata Attribute metadata
 * @param[in] object_type Object type to be checked
 *
 * @return True if object is allowed on on this attribute, false otherwise
 */
extern bool sai_meta_is_allowed_object_type(
        _In_ const sai_attr_metadata_t* metadata,
        _In_ sai_object_type_t object_type);

/**
 * @brief Is allowed enum value
 *
 * @param[in] metadata Attribute metadata
 * @param[in] value Enum value to be checked
 *
 * @return True if enum value is allowed on this attribute, false otherwise
 */
extern bool sai_meta_is_allowed_enum_value(
        _In_ const sai_attr_metadata_t* metadata,
        _In_ int value);

/**
 * @brief Is attribute ACL field or action
 *
 * @param[in] metadata Attribute metadata
 *
 * @return True if is ACL field or action, false otherwise
 */
bool sai_metadata_is_acl_field_or_action(
        _In_ const sai_attr_metadata_t* metadata);

/**
 * @brief Gets attribute metadata based on object type and attribute id
 *
 * @param[in] objecttype Object type
 * @param[in] attrid Attribute Id
 *
 * @return Poionter to object metadata or NULL in case of failure
 */
extern const sai_attr_metadata_t* sai_metadata_get_attr_metadata(
        _In_ sai_object_type_t objecttype,
        _In_ sai_attr_id_t attrid);

/**
 * @brief Gets attribute metadata based on attribute id name
 *
 * @param[in] attr_id_name Attribute id name
 *
 * @return Poionter to object metadata or NULL in case of failure
 */
extern const sai_attr_metadata_t* sai_metadata_get_attr_metadata_by_attr_id_name(
        _In_ const char *attr_id_name);

/**
 * @brief Gets string representation of enum value
 *
 * @param[in] metadata Enum metadata
 * @param[in] value Enum value to bo converted to string
 *
 * @return String representation of enum value or NULL if value was not found
 */
extern const char* sai_metadata_get_enum_value_name(
        _In_ const sai_enum_metadata_t* metadata,
        _In_ int value);

/**
 * @}
 */
#endif /** __SAI_METADATA_UTILS_H_ */
