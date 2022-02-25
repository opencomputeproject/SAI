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
#pragma once

#include <string.h>
#include <stdint.h>
#include <string>
#include "log.h"

class MacAddress
{
public:

    MacAddress(const uint8_t *mac);

    MacAddress(const std::string& mac);

    MacAddress();

    void to_bytes(uint8_t *mac) const;

    const uint8_t *to_bytes() const
    {
        return m_mac;
    }

    friend bool operator==(const MacAddress &mac1, const MacAddress &mac2)
    {
        return (!memcmp(mac1.m_mac, mac2.m_mac, 6));
    }

    inline bool operator!() const
    {
        return (!m_mac[0] && !m_mac[1] && !m_mac[2] &&
                !m_mac[3] && !m_mac[4] && !m_mac[5]);
    }

    inline operator bool() const
    {
        return !!(*this);
    }

    std::string to_string();

    static std::string to_string(const uint8_t* mac);

    static bool TryParseBytes(const std::string& strmac, uint8_t* mac);

private:
    uint8_t m_mac[6];
};

