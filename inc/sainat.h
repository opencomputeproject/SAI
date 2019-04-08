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
 * @file    sainat.h
 *
 * @brief   This module defines SAI NAT (Network Address Translation) spec
 */

#if !defined (__SAINAT_H_)
#define __SAINAT_H_

#include <saitypes.h>

/**
 * @defgroup SAINAT SAI - Network Address Translation (NAT) specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for NAT Range Type
 */
typedef enum _sai_nat_field_range_type_t
{
    /** L4 Source Port Range */
    SAI_NAT_FIELD_RANGE_TYPE_L4_SRC_PORT_RANGE,

    /** L4 Destination Port Range */
    SAI_NAT_FIELD_RANGE_TYPE_L4_DST_PORT_RANGE,

    /** IP address Range */
    SAI_NAT_FIELD_RANGE_TYPE_NAT_IP_RANGE,

} sai_nat_field_range_type_t;

/**
 * @brief Attribute Id for NAT Range Object
 */
typedef enum _sai_nat_range_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_NAT_RANGE_ATTR_START,

    /**
     * @brief Range type
     *
     * Mandatory to pass only one of the range types defined in
     * sai_nat_field_range_type_t enum during NAT Range Creation.
     * Range Type cannot be changed after the range is created.
     *
     * @type sai_nat_field_range_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_NAT_RANGE_ATTR_NAT_FIELD_RANGE_TYPE = SAI_NAT_RANGE_ATTR_START,

    /**
     * @brief Start and End of NAT Range
     *
     * Range will include the start and end values.
     * Range Limit cannot be changed after the range is created.
     *
     * @type sai_u32_range_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_NAT_RANGE_ATTR_LIMIT,

    /**
     * @brief End of attributes
     */
    SAI_NAT_RANGE_ATTR_END,

    /** Custom range base value */
    SAI_NAT_RANGE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_NAT_RANGE_ATTR_CUSTOM_RANGE_END

} sai_nat_range_attr_t;

/**
 * @brief Create and return a NAT range object
 *
 * @param[out] nat_range_id NAT range object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_nat_range_fn)(
        _Out_ sai_object_id_t *nat_range_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified NAT range_object.
 *
 * Deleting a NAT range_object also deletes single specified NAT range.
 *
 * @param[in] nat_range_id NAT object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_nat_range_fn)(
        _In_ sai_object_id_t nat_range_id);

/**
 * @brief Set NAT range attribute value(s).
 *
 * @param[in] nat_range_id NAT range id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_nat_range_attribute_fn)(
        _In_ sai_object_id_t nat_range_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified NAT range attributes.
 *
 * @param[in] nat_range_id NAT range object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_nat_range_attribute_fn)(
        _In_ sai_object_id_t nat_range_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief NAT Attributes
 */
typedef enum _sai_nat_attr_t
{

    /**
     * @brief Start of Attributes
     */
    SAI_NAT_ATTR_START,

    /**
     * @brief Range Type defined in sai_nat_range_type_t
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NAT_RANGE
     * @default empty
     */
    SAI_NAT_ATTR_NAT_RANGE_OBJECT_LIST  = SAI_NAT_ATTR_START,

    /**
     * @brief NAT timeout in milliseconds
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 100
     */
    SAI_NAT_ATTR_TIMEOUT,

    /**
     * @brief NAT port translation
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_NAT_ATTR_PORT_TRANSLATION,

    /**
     * @brief End of Attributes
     */
    SAI_NAT_ATTR_END,

    /** Custom range base value */
    SAI_NAT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_NAT_ATTR_CUSTOM_RANGE_END

} sai_nat_attr_t;

/**
 * @brief Create and return a NAT object
 *
 * @param[out] nat_id NAT object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_nat_fn)(
        _Out_ sai_object_id_t *nat_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified NAT object.
 *
 * Deleting a NAT object also deletes single specified NAT.
 *
 * @param[in] nat_id NAT object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_nat_fn)(
        _In_ sai_object_id_t nat_id);

/**
 * @brief Set NAT attribute value(s).
 *
 * @param[in] nat_id NAT id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_nat_attribute_fn)(
        _In_ sai_object_id_t nat_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified NAT attributes.
 *
 * @param[in] nat_id NAT object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_nat_attribute_fn)(
        _In_ sai_object_id_t nat_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief SAI NAT API set
 */
typedef struct _sai_nat_api_t
{

    /**
     * @brief SAI NAT API set
     */
    sai_create_nat_fn                              create_nat;
    sai_remove_nat_fn                              remove_nat;
    sai_set_nat_attribute_fn                       set_nat_attribute;
    sai_get_nat_attribute_fn                       get_nat_attribute;

    sai_create_nat_range_fn                        create_nat_range;
    sai_remove_nat_range_fn                        remove_nat_range;
    sai_set_nat_range_attribute_fn                 set_nat_range_attribute;
    sai_get_nat_range_attribute_fn                 get_nat_range_attribute;
} sai_nat_api_t;

/**
 * @}
 */
#endif /** __SAINAT_H_ */
