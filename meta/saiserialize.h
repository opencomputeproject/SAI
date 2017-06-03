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
 * @file    saiserialize.h
 *
 * @brief   This module defines SAI Serialize methods
 */

#ifndef __SAISERIALIZE_H_
#define __SAISERIALIZE_H_

/**
 * @defgroup SAISERIALIZE SAI - Serialize Definitions
 *
 * @{
 */

/**
 * @def SAI_SERIALIZE_ERROR
 *
 * Returned from serialize/deserialize methods on any error.
 * Meta log functions are used to produce specific error message.
 */
#define SAI_SERIALIZE_ERROR (-1)

/**
 * @brief Serialize bool value.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] flag Bool flag to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_bool(
        _Out_ char *buffer,
        _In_ bool flag);

/**
 * @brief Serialize 8 bit unsigned integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] u8 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_u8(
        _Out_ char *buffer,
        _In_ uint8_t u8);

/**
 * @brief Serialize 8 bit signed integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] u8 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_s8(
        _Out_ char *buffer,
        _In_ int8_t u8);

/**
 * @brief Serialize 16 bit unsigned integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] u16 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_u16(
        _Out_ char *buffer,
        _In_ uint16_t u16);

/**
 * @brief Serialize 16 bit signed integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] s16 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_s16(
        _Out_ char *buffer,
        _In_ int16_t s16);

/**
 * @brief Serialize 32 bit unsigned integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] u32 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_u32(
        _Out_ char *buffer,
        _In_ uint32_t u32);

/**
 * @brief Serialize 32 bit signed integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] s32 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_s32(
        _Out_ char *buffer,
        _In_ int32_t s32);

/**
 * @brief Serialize 64 bit unsigned integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] u64 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_u64(
        _Out_ char *buffer,
        _In_ uint64_t u64);

/**
 * @brief Serialize 64 bit signed integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] s64 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_s64(
        _Out_ char *buffer,
        _In_ int64_t s64);

/**
 * @brief Serialize object ID.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] object_id Object ID to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_object_id(
        _Out_ char *buffer,
        _In_ sai_object_id_t object_id);

/**
 * @brief Serialize MAC address.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] mac_address MAC address to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_mac(
        _Out_ char *buffer,
        _In_ const sai_mac_t mac_address);

/**
 * @brief Serialize enum value.
 *
 * Buffer will contain actual enum name of number if enum
 * value was not found in specified enum metadata.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] meta Enum metadata for serialization info.
 * @param[in] value Enum value to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_enum(
        _Out_ char *buffer,
        _In_ const sai_enum_metadata_t *meta,
        _In_ int32_t value);

/**
 * @brief Serialize IPv4 address.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] ip4 IP address to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_ipv4(
        _Out_ char *buffer,
        _In_ const sai_ip4_t ip4);

/**
 * @brief Serialize IPv6 address.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] ip6 IP address to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_ipv6(
        _Out_ char *buffer,
        _In_ const sai_ip6_t ip6);

/**
 * @brief Serialize IP address.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] ip_address IP address to be serialized
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_ip_address(
        _Out_ char *buffer,
        _In_ const sai_ip_address_t *ip_address);

/**
 * @brief Serialize IP prefix.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] ip_prefix IP prefix to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_ip_prefix(
        _Out_ char *buffer,
        _In_ const sai_ip_prefix_t *ip_prefix);

/**
 * @brief Serialize IPv4 mask.
 *
 * Mask will be serialized as single number like.
 * Holes in mask are not supported.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] ip4_mask IPv4 mask to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_ipv4_mask(
        _Out_ char *buffer,
        _In_ sai_ip4_t ip4_mask);

/**
 * @brief Serialize IPv6 mask.
 *
 * Mask will be serialized as single number like.
 * Holes in mask are not supported.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] ip6_mask IPv6 mask to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_ipv6_mask(
        _Out_ char *buffer,
        _In_ const sai_ip6_t ip6_mask);

/**
 * @}
 */
#endif /** __SAISERIALIZE_H_ */
