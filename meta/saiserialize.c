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

int sai_serialize_bool(
        _Out_ char *buffer,
        _In_ bool flag)
{
    return sprintf(buffer, "%s", flag ? "true" : "false");
}

int sai_serialize_u8(
        _Out_ char *buffer,
        _In_ uint8_t u8)
{
    return sprintf(buffer, "%u", u8);
}

int sai_serialize_s8(
        _Out_ char *buffer,
        _In_ int8_t u8)
{
    return sprintf(buffer, "%d", u8);
}

int sai_serialize_u16(
        _Out_ char *buffer,
        _In_ uint16_t u16)
{
    return sprintf(buffer, "%u", u16);
}

int sai_serialize_s16(
        _Out_ char *buffer,
        _In_ int16_t s16)
{
    return sprintf(buffer, "%d", s16);
}

int sai_serialize_u32(
        _Out_ char *buffer,
        _In_ uint32_t u32)
{
    return sprintf(buffer, "%u", u32);
}

int sai_serialize_s32(
        _Out_ char *buffer,
        _In_ int32_t s32)
{
    return sprintf(buffer, "%d", s32);
}

int sai_serialize_u64(
        _Out_ char *buffer,
        _In_ uint64_t u64)
{
    return sprintf(buffer, "%lu", u64);
}

int sai_serialize_s64(
        _Out_ char *buffer,
        _In_ int64_t s64)
{
    return sprintf(buffer, "%ld", s64);
}

int sai_serialize_size(
        _Out_ char *buffer,
        _In_ sai_size_t size)
{
    return sprintf(buffer, "%zu", size);
}

int sai_serialize_object_id(
        _Out_ char *buffer,
        _In_ sai_object_id_t oid)
{
    return sprintf(buffer, "oid:0x%lx", oid);
}

int sai_serialize_mac(
        _Out_ char *buffer,
        _In_ const sai_mac_t mac)
{
    return sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

int sai_serialize_enum(
        _Out_ char *buffer,
        _In_ const sai_enum_metadata_t* meta,
        _In_ int32_t value)
{
    if (meta == NULL)
    {
        return sai_serialize_s32(buffer, value);
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

    return sai_serialize_s32(buffer, value);
}

int sai_serialize_ipv4(
        _Out_ char *buffer,
        _In_ sai_ip4_t ip)
{
    if (inet_ntop(AF_INET, &ip, buffer, INET_ADDRSTRLEN) == NULL)
    {
        SAI_META_LOG_WARN("failed to convert ipv4 address, errno: %s", strerror(errno));
        return SAI_SERIALIZE_ERROR;
    }

    return (int)strlen(buffer);
}

int sai_serialize_ipv6(
        _Out_ char *buffer,
        _In_ const sai_ip6_t ip)
{
    if (inet_ntop(AF_INET6, ip, buffer, INET6_ADDRSTRLEN) == NULL)
    {
        SAI_META_LOG_WARN("failed to convert ipv6 address, errno: %s", strerror(errno));
        return SAI_SERIALIZE_ERROR;
    }

    return (int)strlen(buffer);
}

int sai_serialize_ip_address(
        _Out_ char *buffer,
        _In_ const sai_ip_address_t *ip_address)
{
    switch (ip_address->addr_family)
    {
        case SAI_IP_ADDR_FAMILY_IPV4:

            return sai_serialize_ipv4(buffer, ip_address->addr.ip4);

        case SAI_IP_ADDR_FAMILY_IPV6:

            return sai_serialize_ipv6(buffer, ip_address->addr.ip6);

        default:

            SAI_META_LOG_WARN("invalid ip address family: %d", ip_address->addr_family);
            return SAI_SERIALIZE_ERROR;
    }
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

            ret |= sai_serialize_ipv4(addr, ip_prefix->addr.ip4);
            ret |= sai_serialize_ipv4_mask(mask, ip_prefix->mask.ip4);

            if (ret < 0)
            {
                SAI_META_LOG_WARN("failed to serialize ipv4");
                return SAI_SERIALIZE_ERROR;
            }

            break;

        case SAI_IP_ADDR_FAMILY_IPV6:

            ret |= sai_serialize_ipv6(addr, ip_prefix->addr.ip6);
            ret |= sai_serialize_ipv6_mask(mask, ip_prefix->mask.ip6);

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

int sai_serialize_ipv4_mask(
        _Out_ char *buffer,
        _In_ sai_ip4_t mask)
{
    uint32_t n = 32;
    uint32_t tmp = 0xFFFFFFFF;

    mask = __builtin_bswap32(mask);

    for (; (tmp != mask) && tmp; tmp <<= 1, n--);

    if (tmp == mask)
    {
        return sai_serialize_u32(buffer, n);
    }

    SAI_META_LOG_WARN("ipv4 mask 0x%X has holes", htonl(mask));
    return SAI_SERIALIZE_ERROR;
}

int sai_serialize_ipv6_mask(
        _Out_ char *buffer,
        _In_ const sai_ip6_t mask)
{
    uint32_t n = 64;
    uint64_t tmp = 0xFFFFFFFFFFFFFFFFUL;

    uint64_t high = *((const uint64_t*)mask);
    uint64_t low  = *((const uint64_t*)mask + 1);

    high = __builtin_bswap64(high);
    low = __builtin_bswap64(low);

    if (high == tmp)
    {
        for (; (tmp != low) && tmp; tmp <<= 1, n--);

        if (tmp == low)
        {
            return sai_serialize_u32(buffer, 64 + n);
        }
    }
    else if (low == 0)
    {
        for (; (tmp != high) && tmp; tmp <<= 1, n--);

        if (tmp == high)
        {
            return sai_serialize_u32(buffer, n);
        }
    }

    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_serialize_ipv6(buf, mask);

    SAI_META_LOG_WARN("ipv6 mask %s has holes", buf);
    return SAI_SERIALIZE_ERROR;
}
