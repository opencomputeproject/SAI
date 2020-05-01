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
 * @file    saiserialize.c
 *
 * @brief   This module defines SAI Metadata Serialize
 */

#include <arpa/inet.h>
#include <byteswap.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sai.h>
#include "saimetadatautils.h"
#include "saimetadata.h"
#include "saiserialize.h"

#define PRIMITIVE_BUFFER_SIZE 128
#define MAX_CHARS_PRINT 25

bool sai_serialize_is_char_allowed(
        _In_ char c)
{
    /*
     * When we will perform deserialize, we allow buffer string to be
     * terminated not only by zero, but also with json characters like:
     *
     * - end of quote
     * - comma, next item in array
     * - end of array
     *
     * This will be handy when performing deserialize.
     */

    return c == 0 || c == '"' || c == ',' || c == ']' || c == '}';
}

int sai_serialize_bool(
        _Out_ char *buffer,
        _In_ bool flag)
{
    return sprintf(buffer, "%s", flag ? "true" : "false");
}

#define SAI_TRUE_LENGTH 4
#define SAI_FALSE_LENGTH 5

int sai_deserialize_bool(
        _In_ const char *buffer,
        _Out_ bool *flag)
{
    if (strncmp(buffer, "true", SAI_TRUE_LENGTH) == 0 &&
            sai_serialize_is_char_allowed(buffer[SAI_TRUE_LENGTH]))
    {
        *flag = true;
        return SAI_TRUE_LENGTH;
    }

    if (strncmp(buffer, "false", SAI_FALSE_LENGTH) == 0 &&
            sai_serialize_is_char_allowed(buffer[SAI_FALSE_LENGTH]))
    {
        *flag = false;
        return SAI_FALSE_LENGTH;
    }

    /*
     * Limit printf to maximum "false" length + 1 if there is invalid character
     * after "false" string.
     */

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as bool",
            SAI_FALSE_LENGTH + 1,
            buffer);

    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_chardata(
        _Out_ char *buffer,
        _In_ const char data[SAI_CHARDATA_LENGTH])
{
    int idx;

    for (idx = 0; idx < SAI_CHARDATA_LENGTH; ++idx)
    {
        char c = data[idx];

        if (c == 0)
        {
            break;
        }

        if (isprint(c) && c != '\\' && c != '"')
        {
            buffer[idx] = c;
            continue;
        }

        SAI_META_LOG_WARN("invalid character 0x%x in chardata", c);
        return SAI_SERIALIZE_ERROR;
    }

    buffer[idx] = 0;

    return idx;
}

int sai_deserialize_chardata(
        _In_ const char *buffer,
        _Out_ char data[SAI_CHARDATA_LENGTH])
{
    int idx;

    memset(data, 0, SAI_CHARDATA_LENGTH);

    for (idx = 0; idx < SAI_CHARDATA_LENGTH; ++idx)
    {
        char c = buffer[idx];

        if (isprint(c) && c != '\\' && c != '"')
        {
            data[idx] = c;
            continue;
        }

        if (c == 0)
        {
            break;
        }

        if (c == '"')
        {
            /*
             * We allow quote as last char since chardata will be serialized in
             * quotes.
             */

            break;
        }

        SAI_META_LOG_WARN("invalid character 0x%x in chardata", c);
        return SAI_SERIALIZE_ERROR;
    }

    if (sai_serialize_is_char_allowed(buffer[idx]))
    {
        return idx;
    }

    SAI_META_LOG_WARN("invalid character 0x%x in chardata", buffer[idx]);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_uint8(
        _Out_ char *buffer,
        _In_ uint8_t u8)
{
    return sprintf(buffer, "%u", u8);
}

int sai_deserialize_uint8(
        _In_ const char *buffer,
        _Out_ uint8_t *u8)
{
    uint64_t u64;

    int res = sai_deserialize_uint64(buffer, &u64);

    if (res > 0 && u64 <= UCHAR_MAX)
    {
        *u8 = (uint8_t)u64;
        return res;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as uint8", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_int8(
        _Out_ char *buffer,
        _In_ int8_t u8)
{
    return sprintf(buffer, "%d", u8);
}

int sai_deserialize_int8(
        _In_ const char *buffer,
        _Out_ int8_t *s8)
{
    int64_t s64;

    int res = sai_deserialize_int64(buffer, &s64);

    if (res > 0 && s64 >= CHAR_MIN && s64 <= CHAR_MAX)
    {
        *s8 = (int8_t)s64;
        return res;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as int8", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_uint16(
        _Out_ char *buffer,
        _In_ uint16_t u16)
{
    return sprintf(buffer, "%u", u16);
}

int sai_deserialize_uint16(
        _In_ const char *buffer,
        _Out_ uint16_t *u16)
{
    uint64_t u64;

    int res = sai_deserialize_uint64(buffer, &u64);

    if (res > 0 && u64 <= USHRT_MAX)
    {
        *u16 = (uint16_t)u64;
        return res;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as uint16", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_int16(
        _Out_ char *buffer,
        _In_ int16_t s16)
{
    return sprintf(buffer, "%d", s16);
}

int sai_deserialize_int16(
        _In_ const char *buffer,
        _Out_ int16_t *s16)
{
    int64_t s64;

    int res = sai_deserialize_int64(buffer, &s64);

    if (res > 0 && s64 >= SHRT_MIN && s64 <= SHRT_MAX)
    {
        *s16 = (int16_t)s64;
        return res;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as int16", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_uint32(
        _Out_ char *buffer,
        _In_ uint32_t u32)
{
    return sprintf(buffer, "%u", u32);
}

int sai_deserialize_uint32(
        _In_ const char *buffer,
        _Out_ uint32_t *u32)
{
    uint64_t u64;

    int res = sai_deserialize_uint64(buffer, &u64);

    if (res > 0 && u64 <= UINT_MAX)
    {
        *u32 = (uint32_t)u64;
        return res;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as uint32", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_int32(
        _Out_ char *buffer,
        _In_ int32_t s32)
{
    return sprintf(buffer, "%d", s32);
}

int sai_deserialize_int32(
        _In_ const char *buffer,
        _Out_ int32_t *s32)
{
    int64_t s64;

    int res = sai_deserialize_int64(buffer, &s64);

    if (res > 0 && s64 >= INT_MIN && s64 <= INT_MAX)
    {
        *s32 = (int32_t)s64;
        return res;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as int32", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_uint64(
        _Out_ char *buffer,
        _In_ uint64_t u64)
{
    return sprintf(buffer, "%"PRIu64, u64);
}

#define SAI_BASE_10 10

int sai_deserialize_uint64(
        _In_ const char *buffer,
        _Out_ uint64_t *u64)
{
    int idx = 0;
    uint64_t result = 0;

    while (isdigit(buffer[idx]))
    {
        char c = (char)(buffer[idx] - '0');

        /*
         * Base is 10 we can check, that if result is greater than (2^64-1)/10)
         * then next multiplication with 10 will cause overflow.
         */

        if (result > (ULONG_MAX/SAI_BASE_10) ||
            ((result == ULONG_MAX/SAI_BASE_10) && (c > (char)(ULONG_MAX % SAI_BASE_10))))
        {
            idx = 0;
            break;
        }

        result = result * 10 + (uint64_t)(c);

        idx++;
    }

    if (idx > 0 && sai_serialize_is_char_allowed(buffer[idx]))
    {
        *u64 = result;
        return idx;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s...' as uint64", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_int64(
        _Out_ char *buffer,
        _In_ int64_t s64)
{
    return sprintf(buffer, "%"PRId64, s64);
}

int sai_deserialize_int64(
        _In_ const char *buffer,
        _Out_ int64_t *s64)
{
    uint64_t result = 0;
    bool negative = 0;

    if (*buffer == '-')
    {
        buffer++;
        negative = true;
    }

    int res = sai_deserialize_uint64(buffer, &result);

    if (res > 0)
    {
        if (negative)
        {
            if (result <= (uint64_t)(LONG_MIN))
            {
                *s64 = -(int64_t)result;
                return res + 1;
            }
        }
        else
        {
            if (result <= LONG_MAX)
            {
                *s64 = (int64_t)result;
                return res;
            }
        }
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as int64", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_size(
        _Out_ char *buffer,
        _In_ sai_size_t size)
{
    return sprintf(buffer, "%zu", size);
}

int sai_deserialize_size(
        _In_ const char *buffer,
        _Out_ sai_size_t *size)
{
    uint64_t u64;

    int res = sai_deserialize_uint64(buffer, &u64);

    if (res > 0)
    {
        *size = (sai_size_t)u64;
        return res;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s...' as sai_size_t", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_object_id(
        _Out_ char *buffer,
        _In_ sai_object_id_t oid)
{
    return sprintf(buffer, "oid:0x%"PRIx64, oid);
}

int sai_deserialize_object_id(
        _In_ const char *buffer,
        _Out_ sai_object_id_t *oid)
{
    int read;

    int n = sscanf(buffer, "oid:0x%16"PRIx64"%n", oid, &read);

    if (n == 1 && sai_serialize_is_char_allowed(buffer[read]))
    {
        return read;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as oid", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_mac(
        _Out_ char *buffer,
        _In_ const sai_mac_t mac)
{
    return sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

#define SAI_MAC_ADDRESS_LENGTH 17

int sai_deserialize_mac(
        _In_ const char *buffer,
        _Out_ sai_mac_t mac)
{
    int arr[6];
    int read;

    int n = sscanf(buffer, "%2X:%2X:%2X:%2X:%2X:%2X%n",
            &arr[0], &arr[1], &arr[2], &arr[3], &arr[4], &arr[5], &read);

    if (n == 6 && read == SAI_MAC_ADDRESS_LENGTH && sai_serialize_is_char_allowed(buffer[read]))
    {
        for (n = 0; n < 6; n++)
        {
            mac[n] = (uint8_t)arr[n];
        }

        return SAI_MAC_ADDRESS_LENGTH;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as mac address", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_macsec_sak(
        _Out_ char *buffer,
        _In_ const sai_macsec_sak_t sak)
{
    return sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:\
%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:\
%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:\
%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
                   sak[0], sak[1], sak[2], sak[3], sak[4], sak[5],sak[6], sak[7],
                   sak[8], sak[9], sak[10], sak[11], sak[12], sak[13],sak[14], sak[15],
                   sak[16], sak[17], sak[18], sak[19], sak[20], sak[21],sak[22], sak[23],
                   sak[24], sak[25], sak[26], sak[27], sak[28], sak[29],sak[30], sak[31]);
}

int sai_deserialize_macsec_sak(
        _In_ const char *buffer,
        _Out_ sai_macsec_sak_t sak)
{
    int arr[32];
    int read;

    int n = sscanf(buffer, "%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:\
%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X%n",
                   &arr[0], &arr[1], &arr[2], &arr[3],
                   &arr[4], &arr[5], &arr[6], &arr[7],
                   &arr[8], &arr[9], &arr[10], &arr[11],
                   &arr[12], &arr[13], &arr[14], &arr[15],
                   &arr[16], &arr[17], &arr[18], &arr[19],
                   &arr[20], &arr[21], &arr[22], &arr[23],
                   &arr[24], &arr[25], &arr[26], &arr[27],
                   &arr[28], &arr[29], &arr[30], &arr[31], &read);

    if (n == 32 && read == (32*3-1))
    {
        for (n = 0; n < 32; n++)
        {
            sak[n] = (uint8_t)arr[n];
        }

        return read;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as macsec_sak", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_macsec_auth_key(
        _Out_ char *buffer,
        _In_ const sai_macsec_auth_key_t auth)
{
    return sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:\
%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
                   auth[0], auth[1], auth[2], auth[3], auth[4], auth[5],auth[6], auth[7],
                   auth[8], auth[9], auth[10], auth[11], auth[12], auth[13],auth[14], auth[15]);
}

int sai_deserialize_macsec_auth_key(
        _In_ const char *buffer,
        _Out_ sai_macsec_auth_key_t auth)
{
    int arr[16];
    int read;

    int n = sscanf(buffer, "%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X%n",
                   &arr[0], &arr[1], &arr[2], &arr[3],
                   &arr[4], &arr[5], &arr[6], &arr[7],
                   &arr[8], &arr[9], &arr[10], &arr[11],
                   &arr[12], &arr[13], &arr[14], &arr[15], &read);

    if (n == 16 && read == (16*3-1))
    {
       for (n = 0; n < 16; n++)
        {
            auth[n] = (uint8_t)arr[n];
        }

        return read;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as macsec_auth_key", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_macsec_salt(
        _Out_ char *buffer,
        _In_ const sai_macsec_salt_t salt)
{
    return sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
                   salt[0], salt[1], salt[2], salt[3], salt[4], salt[5],salt[6], salt[7],
                   salt[8], salt[9], salt[10], salt[11]);
}

int sai_deserialize_macsec_salt(
        _In_ const char *buffer,
        _Out_ sai_macsec_salt_t salt)
{
    int arr[32];
    int read;

    int n = sscanf(buffer, "%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:\
%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X%n",
                   &arr[0], &arr[1], &arr[2], &arr[3],
                   &arr[4], &arr[5], &arr[6], &arr[7],
                   &arr[8], &arr[9], &arr[10], &arr[11],
                   &arr[12], &arr[13], &arr[14], &arr[15],
                   &arr[16], &arr[17], &arr[18], &arr[19],
                   &arr[20], &arr[21], &arr[22], &arr[23],
                   &arr[24], &arr[25], &arr[26], &arr[27],
                   &arr[28], &arr[29], &arr[30], &arr[31], &read);

    if (n == 32 && read == (32*3-1))
    {
        for (n = 0; n < 32; n++)
        {
            salt[n] = (uint8_t)arr[n];
        }

        return read;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as macsec_salt", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_enum(
        _Out_ char *buffer,
        _In_ const sai_enum_metadata_t *meta,
        _In_ int32_t value)
{
    if (meta == NULL)
    {
        return sai_serialize_int32(buffer, value);
    }

    size_t i = 0;

    for (; i < meta->valuescount; ++i)
    {
        if (meta->values[i] == value)
        {
            return sprintf(buffer, "%s", meta->valuesnames[i]);
        }
    }

    SAI_META_LOG_WARN("enum value %d not found in enum %s", value, meta->name);

    return sai_serialize_int32(buffer, value);
}

int sai_deserialize_enum(
        _In_ const char *buffer,
        _In_ const sai_enum_metadata_t *meta,
        _Out_ int32_t *value)
{
    if (meta == NULL)
    {
        return sai_deserialize_int32(buffer, value);
    }

    size_t idx = 0;

    for (; idx < meta->valuescount; ++idx)
    {
        size_t len = strlen(meta->valuesnames[idx]);

        if (strncmp(meta->valuesnames[idx], buffer, len) == 0 &&
            sai_serialize_is_char_allowed(buffer[len]))
        {
            *value = meta->values[idx];
            return (int)len;
        }
    }

    SAI_META_LOG_WARN("enum value '%.*s' not found in enum %s", MAX_CHARS_PRINT, buffer, meta->name);

    return sai_deserialize_int32(buffer, value);
}

static int sai_deserialize_ip(
        _In_ const char *buffer,
        _In_ int inet,
        _Out_ uint8_t *ip)
{
    /*
     * Since we want relaxed version of deserialize, after ip address there
     * may be '"' (quote), but inet_pton expects '\0' at the end, so copy at
     * most INET6 characters to local buffer.
     */

    char local[INET6_ADDRSTRLEN + 1];

    int idx;

    for (idx = 0; idx < INET6_ADDRSTRLEN; idx++)
    {
        char c = buffer[idx];

        if (isxdigit(c) || c == ':' || c == '.')
        {
            local[idx] = c;
            continue;
        }

        break;
    }

    local[idx] = 0;

    if (inet_pton(inet, local, ip) != 1)
    {
        /*
         * We should not warn here, since we will use this method to
         * deserialize ip4 and ip6 and we will need to guess which one.
         */

        return SAI_SERIALIZE_ERROR;
    }

    if (sai_serialize_is_char_allowed(buffer[idx]) || buffer[idx] == '/')
    {
        return idx;
    }

    SAI_META_LOG_WARN("invalid char 0x%x at end of ip address", buffer[idx]);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_ip4(
        _Out_ char *buffer,
        _In_ sai_ip4_t ip4)
{
    if (inet_ntop(AF_INET, &ip4, buffer, INET_ADDRSTRLEN) == NULL)
    {
        SAI_META_LOG_WARN("failed to convert ipv4 address, errno: %s", strerror(errno));
        return SAI_SERIALIZE_ERROR;
    }

    return (int)strlen(buffer);
}

int sai_deserialize_ip4(
        _In_ const char *buffer,
        _Out_ sai_ip4_t *ip4)
{
    return sai_deserialize_ip(buffer, AF_INET, (uint8_t*)ip4);
}

int sai_serialize_ip6(
        _Out_ char *buffer,
        _In_ const sai_ip6_t ip6)
{
    if (inet_ntop(AF_INET6, ip6, buffer, INET6_ADDRSTRLEN) == NULL)
    {
        SAI_META_LOG_WARN("failed to convert ipv6 address, errno: %s", strerror(errno));
        return SAI_SERIALIZE_ERROR;
    }

    return (int)strlen(buffer);
}

int sai_deserialize_ip6(
        _In_ const char *buffer,
        _Out_ sai_ip6_t ip6)
{
    return sai_deserialize_ip(buffer, AF_INET6, ip6);
}

int sai_serialize_ip_address(
        _Out_ char *buffer,
        _In_ const sai_ip_address_t *ip_address)
{
    switch (ip_address->addr_family)
    {
        case SAI_IP_ADDR_FAMILY_IPV4:

            return sai_serialize_ip4(buffer, ip_address->addr.ip4);

        case SAI_IP_ADDR_FAMILY_IPV6:

            return sai_serialize_ip6(buffer, ip_address->addr.ip6);

        default:

            SAI_META_LOG_WARN("invalid ip address family: %d", ip_address->addr_family);
            return SAI_SERIALIZE_ERROR;
    }
}

int sai_deserialize_ip_address(
        _In_ const char *buffer,
        _Out_ sai_ip_address_t *ip_address)
{
    int res;

    /* try first deserialize ip4 then ip6 */

    res = sai_deserialize_ip(buffer, AF_INET, (uint8_t*)&ip_address->addr.ip4);

    if (res > 0)
    {
        ip_address->addr_family = SAI_IP_ADDR_FAMILY_IPV4;
        return res;
    }

    res = sai_deserialize_ip(buffer, AF_INET6, ip_address->addr.ip6);

    if (res > 0)
    {
        ip_address->addr_family = SAI_IP_ADDR_FAMILY_IPV6;
        return res;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as ip address",
            INET6_ADDRSTRLEN, buffer);

    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_ip_prefix(
        _Out_ char *buffer,
        _In_ const sai_ip_prefix_t *ip_prefix)
{
    int ret = 0;

    char addr[PRIMITIVE_BUFFER_SIZE];
    char mask[PRIMITIVE_BUFFER_SIZE];

    switch (ip_prefix->addr_family)
    {
        case SAI_IP_ADDR_FAMILY_IPV4:

            ret |= sai_serialize_ip4(addr, ip_prefix->addr.ip4);
            ret |= sai_serialize_ip4_mask(mask, ip_prefix->mask.ip4);

            if (ret < 0)
            {
                SAI_META_LOG_WARN("failed to serialize ipv4");
                return SAI_SERIALIZE_ERROR;
            }

            break;

        case SAI_IP_ADDR_FAMILY_IPV6:

            ret |= sai_serialize_ip6(addr, ip_prefix->addr.ip6);
            ret |= sai_serialize_ip6_mask(mask, ip_prefix->mask.ip6);

            if (ret < 0)
            {
                SAI_META_LOG_WARN("failed to serialize ipv6");
                return SAI_SERIALIZE_ERROR;
            }

            break;

        default:

            SAI_META_LOG_WARN("invalid ip address family: %d", ip_prefix->addr_family);
            return SAI_SERIALIZE_ERROR;
    }

    return sprintf(buffer, "%s/%s", addr, mask);
}

int sai_deserialize_ip_prefix(
    _In_ const char *buffer,
    _Out_ sai_ip_prefix_t *ip_prefix)
{
    /* try first deserialize ip4 then ip6 */

    int res, n;

    while (true)
    {
        res = sai_deserialize_ip(buffer, AF_INET, (uint8_t*)&ip_prefix->addr.ip4);

        if (res > 0)
        {
            ip_prefix->addr_family = SAI_IP_ADDR_FAMILY_IPV4;

            if (buffer[res++] != '/')
            {
                break;
            }

            n = sai_deserialize_ip4_mask(buffer + res, &ip_prefix->mask.ip4);

            if (n > 0)
            {
                return res + n;
            }

            break;
        }

        res = sai_deserialize_ip(buffer, AF_INET6, ip_prefix->addr.ip6);

        if (res > 0)
        {
            if (buffer[res++] != '/')
            {
                break;
            }

            ip_prefix->addr_family = SAI_IP_ADDR_FAMILY_IPV6;

            n = sai_deserialize_ip6_mask(buffer + res, (uint8_t*)&ip_prefix->mask.ip6);

            if (n > 0)
            {
                return res + n;
            }
        }

        break;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as ip prefix", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_ip4_mask(
        _Out_ char *buffer,
        _In_ sai_ip4_t mask)
{
    uint32_t n = 32;
    uint32_t tmp = 0xFFFFFFFF;

    mask = __builtin_bswap32(mask);

    for (; (tmp != mask) && tmp; tmp <<= 1, n--);

    if (tmp == mask)
    {
        return sai_serialize_uint32(buffer, n);
    }

    SAI_META_LOG_WARN("ipv4 mask 0x%X has holes", htonl(mask));
    return SAI_SERIALIZE_ERROR;
}

int sai_deserialize_ip4_mask(
        _In_ const char *buffer,
        _Out_ sai_ip4_t *mask)
{
    uint32_t value;

    int res = sai_deserialize_uint32(buffer, &value);

    if (res < 0 || value > 32)
    {
        SAI_META_LOG_WARN("failed to deserialize '%.*s' as ip4 mask", MAX_CHARS_PRINT, buffer);
        return SAI_SERIALIZE_ERROR;
    }

    if (value == 0)
    {
        /* mask is all zeros */
    }
    else if (value == 32)
    {
        value = 0xFFFFFFFF;
    }
    else
    {
        value = 0xFFFFFFFF << (32 - value);
    }

    *mask = __builtin_bswap32(value);

    return res;
}

int sai_serialize_ip6_mask(
        _Out_ char *buffer,
        _In_ const sai_ip6_t mask)
{
    uint32_t n = 64;
    uint64_t tmp = UINT64_C(0xFFFFFFFFFFFFFFFF);

    uint64_t high;
    uint64_t low;
    memcpy(&high, (const uint8_t*)mask, sizeof(uint64_t));
    memcpy(&low, ((const uint8_t*)mask + sizeof(uint64_t)), sizeof(uint64_t));

    high = __builtin_bswap64(high);
    low = __builtin_bswap64(low);

    if (high == tmp)
    {
        for (; (tmp != low) && tmp; tmp <<= 1, n--);

        if (tmp == low)
        {
            return sai_serialize_uint32(buffer, 64 + n);
        }
    }
    else if (low == 0)
    {
        for (; (tmp != high) && tmp; tmp <<= 1, n--);

        if (tmp == high)
        {
            return sai_serialize_uint32(buffer, n);
        }
    }

    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_serialize_ip6(buf, mask);

    SAI_META_LOG_WARN("ipv6 mask %s has holes", buf);
    return SAI_SERIALIZE_ERROR;
}

int sai_deserialize_ip6_mask(
        _In_ const char *buffer,
        _Out_ sai_ip6_t mask)
{
    uint64_t value;

    int res = sai_deserialize_uint64(buffer, &value);

    if (res < 0 || value > 128)
    {
        SAI_META_LOG_WARN("failed to deserialize '%.*s' as ip6 mask", MAX_CHARS_PRINT, buffer);
        return SAI_SERIALIZE_ERROR;
    }

    uint64_t high = UINT64_C(0xFFFFFFFFFFFFFFFF);
    uint64_t low  = UINT64_C(0xFFFFFFFFFFFFFFFF);
    uint64_t tmp;

    if (value == 128)
    {
        /* mask is all ones */
    }
    else if (value == 64)
    {
        low = 0;
    }
    else if (value == 0)
    {
        low = 0;
        high = 0;
    }
    else if (value > 64)
    {
        low = low << (128 - value);
    }
    else
    {
        high = high << (64 - value);
        low = 0;
    }

    tmp = __builtin_bswap64(high);
    memcpy((uint8_t*)mask, &tmp, sizeof(uint64_t));
    tmp = __builtin_bswap64(low);
    memcpy(((uint8_t*)mask + sizeof(uint64_t)), &tmp, sizeof(uint64_t));

    return res;
}

int sai_serialize_pointer(
        _Out_ char *buffer,
        _In_ const sai_pointer_t pointer)
{
    return sprintf(buffer, "ptr:%p", pointer);
}

int sai_deserialize_pointer(
        _In_ const char *buffer,
        _Out_ sai_pointer_t *pointer)
{
    int read;

    int n = sscanf(buffer, "ptr:%p%n", pointer, &read);

    if (n == 1 && sai_serialize_is_char_allowed(buffer[read]))
    {
        return read;
    }

    SAI_META_LOG_WARN("failed to deserialize '%.*s' as pointer", MAX_CHARS_PRINT, buffer);
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_enum_list(
        _Out_ char *buf,
        _In_ const sai_enum_metadata_t *meta,
        _In_ const sai_s32_list_t *list)
{
    if (meta == NULL)
    {
        return sai_serialize_s32_list(buf, list);
    }

    char *begin_buf = buf;
    int ret;

    buf += sprintf(buf, "{");

    buf += sprintf(buf, "\"count\":");

    if (list->list == NULL || list->count == 0)
    {
        buf += sprintf(buf, "null");
    }
    else
    {
        buf += sprintf(buf, "[");

        uint32_t idx;

        for (idx = 0; idx < list->count; idx++)
        {
            if (idx != 0)
            {
                buf += sprintf(buf, ",");
            }

            buf += sprintf(buf, "\"");

            ret = sai_serialize_enum(buf, meta, list->list[idx]);

            if (ret < 0)
            {
                SAI_META_LOG_WARN("failed to serialize enum_list");
                return SAI_SERIALIZE_ERROR;
            }

            buf += sprintf(buf, "\"");
        }

        buf += sprintf(buf, "]");
    }

    buf += sprintf(buf, "}");

    return (int)(buf - begin_buf);
}

int sai_deserialize_enum_list(
        _In_ const char *buffer,
        _In_ const sai_enum_metadata_t *meta,
        _Out_ sai_s32_list_t *list)
{
    SAI_META_LOG_WARN("not implemented");
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_attr_id(
        _Out_ char *buf,
        _In_ const sai_attr_metadata_t *meta,
        _In_ sai_attr_id_t attr_id)
{
    strcpy(buf, meta->attridname);

    return (int)strlen(buf);
}

int sai_deserialize_attr_id(
        _In_ const char *buffer,
        _Out_ sai_attr_id_t *attr_id)
{
    SAI_META_LOG_WARN("not implemented");
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_attribute(
        _Out_ char *buf,
        _In_ const sai_attr_metadata_t *meta,
        _In_ const sai_attribute_t *attribute)
{
    char *begin_buf = buf;
    int ret;

    /* can be auto generated */

    buf += sprintf(buf, "{");

    buf += sprintf(buf, "\"id\":");

    buf += sprintf(buf, "\"");

    ret = sai_serialize_attr_id(buf, meta, attribute->id);

    if (ret < 0)
    {
        SAI_META_LOG_WARN("failed to serialize attr id");
        return SAI_SERIALIZE_ERROR;
    }

    buf += ret;

    buf += sprintf(buf, "\",");

    buf += sprintf(buf, "\"value\":");

    ret = sai_serialize_attribute_value(buf, meta, &attribute->value);

    if (ret < 0)
    {
        SAI_META_LOG_WARN("failed to serialize attribute value");
        return SAI_SERIALIZE_ERROR;
    }

    buf += ret;

    buf += sprintf(buf, "}");

    return (int)(buf - begin_buf);
}

int sai_deserialize_attribute(
        _In_ const char *buffer,
        _Out_ sai_attribute_t *attribute)
{
    SAI_META_LOG_WARN("not implemented");
    return SAI_SERIALIZE_ERROR;
}
