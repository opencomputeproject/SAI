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
 * @file    saihash.h
 *
 * @brief   This module defines SAI Hash interface
 */

#if !defined (__SAIHASH_H_)
#define __SAIHASH_H_

#include <saitypes.h>

/**
 * @defgroup SAIHASH SAI - Hash specific API definitions.
 *
 * @{
 */

/**
 * @brief Attribute data for SAI native hash fields
 */
typedef enum _sai_native_hash_field_t
{
    /**
     * @brief Native hash field source IP.
     *
     * Also, refers to the outer source IP
     * in case for encapsulated packets.
     * Used for both IPv4 and IPv6
     */
    SAI_NATIVE_HASH_FIELD_SRC_IP,

    /**
     * @brief Native hash field destination IP.
     *
     * Also, refers to the outer source IP
     * in case for encapsulated packets.
     * Used for both IPv4 and IPv6
     */
    SAI_NATIVE_HASH_FIELD_DST_IP,

    /** Native hash field inner source IP */
    SAI_NATIVE_HASH_FIELD_INNER_SRC_IP,

    /** Native hash field inner destination IP */
    SAI_NATIVE_HASH_FIELD_INNER_DST_IP,

    /**
     * @brief Native hash field source IPv4.
     *
     * Also, refers to the outer source IPv4
     * in case for encapsulated packets
     */
    SAI_NATIVE_HASH_FIELD_SRC_IPV4,

    /**
     * @brief Native hash field destination IPv4
     *
     * Also, refers to the outer source IPv4
     * in case for encapsulated packets
     */
    SAI_NATIVE_HASH_FIELD_DST_IPV4,

    /**
     * @brief Native hash field source IPv6.
     *
     * Also, refers to the outer source IPv6
     * in case for encapsulated packets
     */
    SAI_NATIVE_HASH_FIELD_SRC_IPV6,

    /**
     * @brief Native hash field destination IPv6
     *
     * Also, refers to the outer source IPv6
     * in case for encapsulated packets
     */
    SAI_NATIVE_HASH_FIELD_DST_IPV6,

    /** Native hash field inner source IPv4 */
    SAI_NATIVE_HASH_FIELD_INNER_SRC_IPV4,

    /** Native hash field inner destination IPv4 */
    SAI_NATIVE_HASH_FIELD_INNER_DST_IPV4,

    /** Native hash field inner source IPv6 */
    SAI_NATIVE_HASH_FIELD_INNER_SRC_IPV6,

    /** Native hash field inner destination IPv6 */
    SAI_NATIVE_HASH_FIELD_INNER_DST_IPV6,

    /** Native hash field vlan id */
    SAI_NATIVE_HASH_FIELD_VLAN_ID,

    /** Native hash field IP protocol */
    SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,

    /** Native hash field Ethernet type */
    SAI_NATIVE_HASH_FIELD_ETHERTYPE,

    /** Native hash field L4 source port */
    SAI_NATIVE_HASH_FIELD_L4_SRC_PORT,

    /** Native hash field L4 destination port */
    SAI_NATIVE_HASH_FIELD_L4_DST_PORT,

    /** Native hash field source MAC */
    SAI_NATIVE_HASH_FIELD_SRC_MAC,

    /** Native hash field destination MAC */
    SAI_NATIVE_HASH_FIELD_DST_MAC,

    /** Native hash field source port */
    SAI_NATIVE_HASH_FIELD_IN_PORT,

    /** Native hash field inner IP protocol */
    SAI_NATIVE_HASH_FIELD_INNER_IP_PROTOCOL,

    /** Native hash field inner Ethernet type */
    SAI_NATIVE_HASH_FIELD_INNER_ETHERTYPE,

    /** Native hash field inner L4 source port */
    SAI_NATIVE_HASH_FIELD_INNER_L4_SRC_PORT,

    /** Native hash field inner L4 destination port */
    SAI_NATIVE_HASH_FIELD_INNER_L4_DST_PORT,

    /** Native hash field inner source MAC */
    SAI_NATIVE_HASH_FIELD_INNER_SRC_MAC,

    /** Native hash field inner destination MAC */
    SAI_NATIVE_HASH_FIELD_INNER_DST_MAC,

    /** Native hash field entire MPLS label stack */
    SAI_NATIVE_HASH_FIELD_MPLS_LABEL_ALL,

    /** Native hash field the top MPLS label */
    SAI_NATIVE_HASH_FIELD_MPLS_LABEL_0,

    /** Native hash field second MPLS label from the top */
    SAI_NATIVE_HASH_FIELD_MPLS_LABEL_1,

    /** Native hash field third MPLS label from the top */
    SAI_NATIVE_HASH_FIELD_MPLS_LABEL_2,

    /** Native hash field fourth MPLS label from the top */
    SAI_NATIVE_HASH_FIELD_MPLS_LABEL_3,

    /** Native hash field fifth MPLS label from the top */
    SAI_NATIVE_HASH_FIELD_MPLS_LABEL_4,

    /** No field - for compatibility, must be last */
    SAI_NATIVE_HASH_FIELD_NONE,

} sai_native_hash_field_t;

/**
 * @brief Fine-grained hash field attribute IDs
 */
typedef enum _sai_fine_grained_hash_field_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FINE_GRAINED_HASH_FIELD_ATTR_START,

    /**
     * @brief Hash native field ID.
     *
     * @type sai_native_hash_field_t
     * @flags CREATE_ONLY
     * @default SAI_NATIVE_HASH_FIELD_NONE
     */
    SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD = SAI_FINE_GRAINED_HASH_FIELD_ATTR_START,

    /**
     * @brief Mask for a IPv4 address.
     *
     * @type sai_ip4_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD == SAI_NATIVE_HASH_FIELD_SRC_IPV4 or SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD == SAI_NATIVE_HASH_FIELD_DST_IPV4 or SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD == SAI_NATIVE_HASH_FIELD_INNER_SRC_IPV4 or SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD == SAI_NATIVE_HASH_FIELD_INNER_DST_IPV4
     */
    SAI_FINE_GRAINED_HASH_FIELD_ATTR_IPV4_MASK,

    /**
     * @brief Mask for a IPv6 address.
     *
     * @type sai_ip6_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD == SAI_NATIVE_HASH_FIELD_SRC_IPV6 or SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD == SAI_NATIVE_HASH_FIELD_DST_IPV6 or SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD == SAI_NATIVE_HASH_FIELD_INNER_SRC_IPV6 or SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD == SAI_NATIVE_HASH_FIELD_INNER_DST_IPV6
     */
    SAI_FINE_GRAINED_HASH_FIELD_ATTR_IPV6_MASK,

    /**
     * @brief Optional field ordering.
     *
     * Specifies in which order the fields are hashed,
     * and defines in which fields should be associative for CRC with the same sequence ID.
     * If not provided, it's up to SAI driver to choose.
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_FINE_GRAINED_HASH_FIELD_ATTR_SEQUENCE_ID,

    /**
     * @brief End of attributes
     */
    SAI_FINE_GRAINED_HASH_FIELD_ATTR_END,

    /** Custom range base value */
    SAI_FINE_GRAINED_HASH_FIELD_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_FINE_GRAINED_HASH_FIELD_ATTR_CUSTOM_RANGE_END

} sai_fine_grained_hash_field_attr_t;

/**
 * @brief Hash attribute IDs
 */
typedef enum _sai_hash_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_HASH_ATTR_START,

    /**
     * @brief Hash native fields
     *
     * @type sai_s32_list_t sai_native_hash_field_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_HASH_ATTR_NATIVE_HASH_FIELD_LIST = SAI_HASH_ATTR_START,

    /**
     * @brief Hash UDF group
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_UDF_GROUP
     * @default empty
     */
    SAI_HASH_ATTR_UDF_GROUP_LIST,

    /**
     * @brief Hash fine-grained field list
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_FINE_GRAINED_HASH_FIELD
     * @default empty
     */
    SAI_HASH_ATTR_FINE_GRAINED_HASH_FIELD_LIST,

    /**
     * @brief End of attributes
     */
    SAI_HASH_ATTR_END,

    /** Custom range base value */
    SAI_HASH_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_HASH_ATTR_CUSTOM_RANGE_END

} sai_hash_attr_t;

/**
 * @brief Create fine-grained hash field
 *
 * @param[out] fine_grained_hash_field_id Fine-grained hash field id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_fine_grained_hash_field_fn)(
        _Out_ sai_object_id_t *fine_grained_hash_field_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove fine-grained hash field
 *
 * @param[in] fine_grained_hash_field_id Fine-grained hash field id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_fine_grained_hash_field_fn)(
        _In_ sai_object_id_t fine_grained_hash_field_id);

/**
 * @brief Set fine-grained hash field attribute
 *
 * @param[in] fine_grained_hash_field_id Fine-grained hash field id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_fine_grained_hash_field_attribute_fn)(
        _In_ sai_object_id_t fine_grained_hash_field_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get fine-grained hash field attribute value
 *
 * @param[in] fine_grained_hash_field_id Fine-grained hash field id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_fine_grained_hash_field_attribute_fn)(
        _In_ sai_object_id_t fine_grained_hash_field_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create hash
 *
 * @param[out] hash_id Hash id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_hash_fn)(
        _Out_ sai_object_id_t *hash_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove hash
 *
 * @param[in] hash_id Hash id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_hash_fn)(
        _In_ sai_object_id_t hash_id);

/**
 * @brief Set hash attribute
 *
 * @param[in] hash_id Hash id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_hash_attribute_fn)(
        _In_ sai_object_id_t hash_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get hash attribute value
 *
 * @param[in] hash_id Hash id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_hash_attribute_fn)(
        _In_ sai_object_id_t hash_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Hash methods, retrieved via sai_api_query()
 */
typedef struct _sai_hash_api_t
{
    sai_create_hash_fn                             create_hash;
    sai_remove_hash_fn                             remove_hash;
    sai_set_hash_attribute_fn                      set_hash_attribute;
    sai_get_hash_attribute_fn                      get_hash_attribute;
    sai_create_fine_grained_hash_field_fn          create_fine_grained_hash_field;
    sai_remove_fine_grained_hash_field_fn          remove_fine_grained_hash_field;
    sai_set_fine_grained_hash_field_attribute_fn   set_fine_grained_hash_field_attribute;
    sai_get_fine_grained_hash_field_attribute_fn   get_fine_grained_hash_field_attribute;

} sai_hash_api_t;

/**
 * @}
 */
#endif /** __SAIHASH_H_ */
