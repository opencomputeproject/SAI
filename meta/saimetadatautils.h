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
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
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
 * @brief   This module defines SAI Metadata Utilities
 */

#ifndef __SAIMETADATAUTILS_H_
#define __SAIMETADATAUTILS_H_

#include "saimetadatatypes.h"

/**
 * @defgroup SAIMETADATAUTILS SAI - Metadata Utilities Definitions
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
extern bool sai_metadata_is_allowed_object_type(
        _In_ const sai_attr_metadata_t *metadata,
        _In_ sai_object_type_t object_type);

/**
 * @brief Is allowed enum value
 *
 * @param[in] metadata Attribute metadata
 * @param[in] value Enum value to be checked
 *
 * @return True if enum value is allowed on this attribute, false otherwise
 */
extern bool sai_metadata_is_allowed_enum_value(
        _In_ const sai_attr_metadata_t *metadata,
        _In_ int value);

/**
 * @brief Gets attribute metadata based on object type and attribute id
 *
 * @param[in] object_type Object type
 * @param[in] attr_id Attribute Id
 *
 * @return Pointer to object metadata or NULL in case of failure
 */
extern const sai_attr_metadata_t* sai_metadata_get_attr_metadata(
        _In_ sai_object_type_t object_type,
        _In_ sai_attr_id_t attr_id);

/**
 * @brief Gets attribute metadata based on attribute id name
 *
 * @param[in] attr_id_name Attribute id name
 *
 * @return Pointer to object metadata or NULL in case of failure
 */
extern const sai_attr_metadata_t* sai_metadata_get_attr_metadata_by_attr_id_name(
        _In_ const char *attr_id_name);

/**
 * @brief Gets string representation of enum value
 *
 * @param[in] metadata Enum metadata
 * @param[in] value Enum value to be converted to string
 *
 * @return String representation of enum value or NULL if value was not found
 */
extern const char* sai_metadata_get_enum_value_name(
        _In_ const sai_enum_metadata_t *metadata,
        _In_ int value);

/**
 * @brief Gets attribute from attribute list by attribute id
 *
 * @param[in] id Attribute id to be found
 * @param[in] attr_count Total number of attributes
 * @param[in] attr_list List of attributes to search
 *
 * @return Attribute pointer with requested ID or NULL if not found
 */
extern const sai_attribute_t* sai_metadata_get_attr_by_id(
        _In_ sai_attr_id_t id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Gets object type info
 *
 * @param[in] object_type Object type
 *
 * @return Object type info structure or NULL if not found
 */
extern const sai_object_type_info_t* sai_metadata_get_object_type_info(
        _In_ sai_object_type_t object_type);

/**
 * @brief Checks if object type is valid
 *
 * @param[in] object_type Object type
 *
 * @return True if object type is valid, false otherwise
 */
extern bool sai_metadata_is_object_type_valid(
        _In_ sai_object_type_t object_type);
/**
 * @}
 */
#endif /** __SAIMETADATAUTILS_H_ */
