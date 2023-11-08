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
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
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
 * @return True if object is allowed on this attribute, false otherwise
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
 * @brief Gets attribute metadata based on attribute id name, supporting case of
 * attribute id name is in deserialized buffer and terminated by characters listed
 * in function sai_serialize_is_char_allowed.
 *
 * @param[in] attr_id_name Attribute id name
 *
 * @return Pointer to object metadata or NULL in case of failure
 */
extern const sai_attr_metadata_t* sai_metadata_get_attr_metadata_by_attr_id_name_ext(
        _In_ const char *attr_id_name);

/**
 * @brief Gets ignored attribute metadata based on attribute id name
 *
 * @param[in] attr_id_name Attribute id name
 *
 * @return Pointer to object metadata or NULL in case of failure
 */
extern const sai_attr_metadata_t* sai_metadata_get_ignored_attr_metadata_by_attr_id_name(
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
 * @brief Gets attribute from attribute list by attribute id.
 *
 * @param[in] id Attribute id to be found.
 * @param[in] attr_count Total number of attributes.
 * @param[in] attr_list List of attributes to search.
 *
 * @return Attribute pointer with requested ID or NULL if not found.
 * When multiple attributes with the same id are passed, only first
 * attribute is returned.
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
 * @brief Checks whether object type is OID object type.
 *
 * @param[in] object_type Object type to be checked.
 *
 * @return True if object type is OID type, false otherwise.
 */
extern bool sai_metadata_is_object_type_oid(
        _In_ sai_object_type_t object_type);

/**
 * @brief Check if condition met.
 *
 * List of attributes will be examined in terms of conditions. This is
 * convenient since user can pass list when calling create API. If
 * condition attribute is not on the list, then default value will be
 * examined.
 *
 * NOTE: When multiple attributes with the same ID are passed,
 * sai_metadata_get_attr_by_id will select only first one.
 * Function will not be able to handle duplicated attributes.
 *
 * @param[in] metadata Metadata of attribute that we need to check.
 * @param[in] attr_count Number of attributes.
 * @param[in] attr_list Attribute list to check. All attributes must
 * belong to the same object type as metadata parameter.
 *
 * @return True if condition is in force, false otherwise. False will be also
 * returned if any of input pointers is NULL or attribute is not conditional.
 */
extern bool sai_metadata_is_condition_met(
        _In_ const sai_attr_metadata_t *metadata,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Check if valid only condition is met.
 *
 * List of attributes will be examined in terms of valid only conditions. This
 * is convenient since user can pass list when calling create API. If valid
 * only condition attribute is not on the list, then default value will be
 * examined.
 *
 * NOTE: When multiple attributes with the same ID are passed,
 * sai_metadata_get_attr_by_id will select only first one. Function will not
 * be able to handle duplicated attributes.
 *
 * @param[in] metadata Metadata of attribute that we need to check.
 * @param[in] attr_count Number of attributes.
 * @param[in] attr_list Attribute list to check. All attributes must
 * belong to the same object type as metadata parameter.
 *
 * @return True if valid only condition is in force, false otherwise. False
 * will be also returned if any of input pointers is NULL or attribute is not
 * valid only conditional.
 */
extern bool sai_metadata_is_validonly_met(
        _In_ const sai_attr_metadata_t *metadata,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Metadata query API version.
 *
 * Will return SAI version which was used to generate metadata.
 */
extern sai_api_version_t sai_metadata_query_api_version(void);

/**
 * @}
 */
#endif /** __SAIMETADATAUTILS_H_ */
