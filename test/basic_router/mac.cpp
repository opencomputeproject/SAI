/*
 * Copyright (c) 2015 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
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
 *
 */
#include "mac.h"

MacAddress::MacAddress(const uint8_t *mac)
{
    memcpy(m_mac, mac, 6);
}

MacAddress::MacAddress(const std::string& mac)
{
    MacAddress::TryParseBytes(mac, m_mac);
}

MacAddress::MacAddress()
{
    memset(m_mac, 0, 6);
}

void MacAddress::to_bytes(uint8_t *mac) const
{
    memcpy(mac, m_mac, 6);
}

std::string MacAddress::to_string()
{
    char macstr[32];
    sprintf(macstr, "%02x:%02x:%02x:%02x:%02x:%02x", m_mac[0], m_mac[1], m_mac[2], m_mac[3], m_mac[4], m_mac[5]);
    return std::string(macstr);
}

std::string MacAddress::to_string(const uint8_t* mac)
{
    uint8_t strMac[32];
    sprintf((char*)strMac, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return std::string((char*)strMac);
}

bool MacAddress::TryParseBytes(const std::string& macstr,
                               uint8_t* mac)
{
    if (mac == NULL)
    {
        return false;
    }

    uint32_t tempMac[6];

    if (sscanf(macstr.c_str(), "%02x:%02x:%02x:%02x:%02x:%02x",
               &tempMac[0], &tempMac[1], &tempMac[2], &tempMac[3], &tempMac[4], &tempMac[5]) != 6)
    {
        return false;
    }

    for (int i = 0; i < 6; i++)
    {
        mac[i] = (uint8_t)tempMac[i];
    }

    return true;
}
