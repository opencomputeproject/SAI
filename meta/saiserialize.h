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
 * @def SAI_CHARDATA_LENGTH
 *
 * Defines size of char data inside sai_attribute_value_t union.
 */
#define SAI_CHARDATA_LENGTH 32

/**
 * @brief Is char allowed.
 *
 * Function checks if given char is one of the following:
 * - '\0', '"', ',', ']', '}'
 *
 * Since serialization is done to json format, after each value
 * there may be some characters specific to json format, like:
 *
 * * quote, if value was in quotes (string)
 * * comma, if value was without quotes but an item in array (number, bool)
 * * square bracket, if item was last item in array (number, bool)
 * * curly bracket, if item was last item in object (number, bool)
 *
 * This means that deserialize is "relaxed", so each item don't need to end
 * as zero '\0' but it can end on any of those characters. This allows us to
 * deserialize json string reading it directly without creating json object
 * tree and without any extra string copy. For example if we have item:
 * {"foo":true}, we can just read value "true}" and ignore last character and
 * still value will be deserialized correctly.
 *
 * This is not ideal solution, but in this case it will work just fine.
 *
 * NOTE: All auto generated methods will enforce to check extra characters at
 * the end of each value.
 */
bool sai_serialize_is_char_allowed(
        _In_ char c);

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
 * @brief Deserialize bool value.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] flag Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_bool(
        _In_ const char *buffer,
        _Out_ bool *flag);

/**
 * @brief Serialize char data value.
 *
 * All printable characters (isprint) are allowed except '\' and '"'.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] data Data to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_chardata(
        _Out_ char *buffer,
        _In_ const char data[SAI_CHARDATA_LENGTH]);

/**
 * @brief Deserialize char data value.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] data Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_chardata(
        _In_ const char *buffer,
        _Out_ char data[SAI_CHARDATA_LENGTH]);

/**
 * @brief Serialize 8 bit unsigned integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] u8 Deserialized value.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_uint8(
        _Out_ char *buffer,
        _In_ uint8_t u8);

/**
 * @brief Deserialize 8 bit unsigned integer.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] u8 Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_uint8(
        _In_ const char *buffer,
        _Out_ uint8_t *u8);

/**
 * @brief Serialize 8 bit signed integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] u8 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_int8(
        _Out_ char *buffer,
        _In_ int8_t u8);

/**
 * @brief Deserialize 8 bit signed integer.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] s8 Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_int8(
        _In_ const char *buffer,
        _Out_ int8_t *s8);

/**
 * @brief Serialize 16 bit unsigned integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] u16 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_uint16(
        _Out_ char *buffer,
        _In_ uint16_t u16);

/**
 * @brief Deserialize 16 bit unsigned integer.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] u16 Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_uint16(
        _In_ const char *buffer,
        _Out_ uint16_t *u16);

/**
 * @brief Serialize 16 bit signed integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] s16 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_int16(
        _Out_ char *buffer,
        _In_ int16_t s16);

/**
 * @brief Deserialize 16 bit signed integer.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] s16 Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_int16(
        _In_ const char *buffer,
        _Out_ int16_t *s16);

/**
 * @brief Serialize 32 bit unsigned integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] u32 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_uint32(
        _Out_ char *buffer,
        _In_ uint32_t u32);

/**
 * @brief Deserialize 32 bit unsigned integer.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] u32 Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_uint32(
        _In_ const char *buffer,
        _Out_ uint32_t *u32);

/**
 * @brief Serialize 32 bit signed integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] s32 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_int32(
        _Out_ char *buffer,
        _In_ int32_t s32);

/**
 * @brief Deserialize 32 bit signed integer.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] s32 Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_int32(
        _In_ const char *buffer,
        _Out_ int32_t *s32);

/**
 * @brief Serialize 64 bit unsigned integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] u64 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_uint64(
        _Out_ char *buffer,
        _In_ uint64_t u64);

/**
 * @brief Deserialize 64 bit unsigned integer.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] u64 Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_uint64(
        _In_ const char *buffer,
        _Out_ uint64_t *u64);

/**
 * @brief Serialize 64 bit signed integer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] s64 Integer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_int64(
        _Out_ char *buffer,
        _In_ int64_t s64);

/**
 * @brief Deserialize 64 bit signed integer.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] s64 Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_int64(
        _In_ const char *buffer,
        _Out_ int64_t *s64);

/**
 * @brief Serialize sai_size_t.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] size Size to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_size(
        _Out_ char *buffer,
        _In_ sai_size_t size);

/**
 * @brief Deserialize sai_size_t.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] size Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_size(
        _In_ const char *buffer,
        _Out_ sai_size_t *size);

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
 * @brief Deserialize object Id.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] object_id Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_object_id(
        _In_ const char *buffer,
        _Out_ sai_object_id_t *object_id);

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
 * @brief Deserialize MAC address.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] mac Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_mac(
        _In_ const char *buffer,
        _Out_ sai_mac_t mac);

/**
 * @brief Serialize encrypt_key.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] key The encrypt_key to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_encrypt_key(
        _Out_ char *buffer,
        _In_ const sai_encrypt_key_t key);

/**
 * @brief Deserialize encrypt_key.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] key The encrypt_key deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_encrypt_key(
        _In_ const char *buffer,
        _Out_ sai_encrypt_key_t key);

/**
 * @brief Serialize auth_key.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] auth The auth_key to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_auth_key(
        _Out_ char *buffer,
        _In_ const sai_auth_key_t auth);

/**
 * @brief Deserialize auth_key.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] auth The auth_key deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_auth_key(
        _In_ const char *buffer,
        _Out_ sai_auth_key_t auth);

/**
 * @brief Serialize macsec_sak.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] sak The macsec_sak to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_macsec_sak(
        _Out_ char *buffer,
        _In_ const sai_macsec_sak_t sak);

/**
 * @brief Deserialize macsec_sak.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] sak The macsec_sak deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_macsec_sak(
        _In_ const char *buffer,
        _Out_ sai_macsec_sak_t sak);

/**
 * @brief Serialize macsec_auth_key.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] auth The macsec_auth_key to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_macsec_auth_key(
        _Out_ char *buffer,
        _In_ const sai_macsec_auth_key_t auth);

/**
 * @brief Deserialize macsec_auth_key.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] auth The macsec_auth_key deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_macsec_auth_key(
        _In_ const char *buffer,
        _Out_ sai_macsec_auth_key_t auth);

/**
 * @brief Serialize macsec_salt.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] salt The macsec_salt to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_macsec_salt(
        _Out_ char *buffer,
        _In_ const sai_macsec_salt_t salt);

/**
 * @brief Deserialize macsec_salt.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] salt The macsec_salt Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_macsec_salt(
        _In_ const char *buffer,
        _Out_ sai_macsec_salt_t salt);

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
 * @brief Deserialize enum value.
 *
 * If buffer will not contain valid enum name, function will attempt to
 * deserialize value as signed 32 bit integer.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[in] meta Enum metadata.
 * @param[out] value Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_enum(
        _In_ const char *buffer,
        _In_ const sai_enum_metadata_t *meta,
        _Out_ int32_t *value);

/**
 * @brief Serialize IPv4 address.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] ip4 IP address to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_ip4(
        _Out_ char *buffer,
        _In_ const sai_ip4_t ip4);

/**
 * @brief Deserialize IPv4 address.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] ip4 Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_ip4(
        _In_ const char *buffer,
        _Out_ sai_ip4_t *ip4);

/**
 * @brief Serialize IPv6 address.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] ip6 IP address to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_ip6(
        _Out_ char *buffer,
        _In_ const sai_ip6_t ip6);

/**
 * @brief Deserialize IPv6 address.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] ip6 Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_ip6(
        _In_ const char *buffer,
        _Out_ sai_ip6_t ip6);

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
 * @brief Deserialize IP address.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] ip_address Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_ip_address(
        _In_ const char *buffer,
        _Out_ sai_ip_address_t *ip_address);

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
 * @brief Deserialize IP prefix.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] ip_prefix Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_ip_prefix(
        _In_ const char *buffer,
        _Out_ sai_ip_prefix_t *ip_prefix);

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
int sai_serialize_ip4_mask(
        _Out_ char *buffer,
        _In_ sai_ip4_t ip4_mask);

/**
 * @brief Deserialize IPv4 mask.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] ip4_mask Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_ip4_mask(
        _In_ const char *buffer,
        _Out_ sai_ip4_t *ip4_mask);

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
int sai_serialize_ip6_mask(
        _Out_ char *buffer,
        _In_ const sai_ip6_t ip6_mask);

/**
 * @brief Deserialize IPv6 mask.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] ip6_mask Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_ip6_mask(
        _In_ const char *buffer,
        _Out_ sai_ip6_t ip6_mask);

/**
 * @brief Serialize pointer.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] pointer Pointer to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_pointer(
        _Out_ char *buffer,
        _In_ const sai_pointer_t pointer);

/**
 * @brief Deserialize pointer.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] pointer Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_pointer(
        _In_ const char *buffer,
        _Out_ sai_pointer_t *pointer);

/**
 * @brief Serialize enum list.
 *
 * If enum metadata is null, then list is serialized using
 * sai_serialize_s32_list and it will not contain quotes.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] meta Enum metadata used to serialize.
 * @param[in] s32_list List of enum values to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_enum_list(
        _Out_ char *buffer,
        _In_ const sai_enum_metadata_t *meta,
        _In_ const sai_s32_list_t *s32_list);

/**
 * @brief Deserialize enum list.
 *
 * If enum metadata is null, then list is deserialized using
 * sai_deserialize_s32_list and it will not contain quotes.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[in] meta Enum metadata.
 * @param[out] s32_list Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_enum_list(
        _In_ const char *buffer,
        _In_ const sai_enum_metadata_t *meta,
        _Out_ sai_s32_list_t *s32_list);

/**
 * @brief Serialize attribute id.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] meta Attribute metadata.
 * @param[in] attr_id Attribute id to be serialized
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_attr_id(
        _Out_ char *buffer,
        _In_ const sai_attr_metadata_t *meta,
        _In_ sai_attr_id_t attr_id);

/**
 * @brief Deserialize attribute id.
 *
 * Metadata is not needed since attribute ID is serialized as string, and it
 * can point to unique attribute metadata.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] attr_id Deserialized attribute id.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_attr_id(
        _In_ const char *buffer,
        _Out_ sai_attr_id_t *attr_id);

/**
 * @brief Serialize SAI attribute.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] meta Attribute metadata.
 * @param[in] attribute Attribute to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_attribute(
        _Out_ char *buffer,
        _In_ const sai_attr_metadata_t *meta,
        _In_ const sai_attribute_t *attribute);

/**
 * @brief Deserialize SAI attribute.
 *
 * Metadata is not needed since attribute ID is serialized as string, and it
 * can point to unique attribute metadata.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] attribute Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_attribute(
        _In_ const char *buffer,
        _Out_ sai_attribute_t *attribute);

/**
 * @brief Free SAI attribute.
 *
 * @param[in] meta Attribute metadata.
 * @param[in] attribute Attribute to be freed.
 */
void sai_free_attribute(
        _In_ const sai_attr_metadata_t *meta,
        _In_ const sai_attribute_t *attribute);

/**
 * @brief Serialize decimal p2.
 *
 * @param[out] buffer Output buffer for serialized value.
 * @param[in] decimal Decimal_p2 to be serialized.
 *
 * @return Number of characters written to buffer excluding '\0',
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_serialize_decimal_p2(
        _Out_ char *buffer,
        _In_ sai_decimal_p2_t decimal);

/**
 * @brief Deserialize decimal p2.
 *
 * @param[in] buffer Input buffer to be examined.
 * @param[out] decimal Deserialized value.
 *
 * @return Number of characters consumed from the buffer,
 * or #SAI_SERIALIZE_ERROR on error.
 */
int sai_deserialize_decimal_p2(
        _In_ const char *buffer,
        _Out_ sai_decimal_p2_t *decimal);

/**
 * @}
 */
#endif /** __SAISERIALIZE_H_ */
