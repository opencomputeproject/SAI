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

#include <stdint.h>
#include <string>
#include <set>
#include "log.h"

class IpAddress
{
public:
    IpAddress() : m_addr(0) {}
    IpAddress(uint32_t addr)
    {
        m_addr = addr;
    }
    IpAddress(const std::string &ipstr);

    // the address is in network order
    uint32_t addr() const
    {
        return m_addr;
    }

    bool operator<(const IpAddress &o) const
    {
        return (m_addr < o.m_addr);
    }

    bool operator==(const IpAddress &o) const
    {
        return (m_addr == o.m_addr);
    }

    const std::string to_string() const;

private:
    uint32_t m_addr;
};

class IpAddresses
{
public:
    IpAddresses() {}

    // ipStrList is a list IPs separated by ","
    IpAddresses(const std::string &ipstrList);

    void add(const std::string &ipstr);

    bool operator<(const IpAddresses &o) const;

    bool operator==(const IpAddresses &o) const
    {
        return m_addrSet == o.m_addrSet;
    }

    bool operator!=(const IpAddresses &o) const
    {
        return m_addrSet != o.m_addrSet;
    }

    size_t size() const
    {
        return m_addrSet.size();
    }

    const std::string to_string() const;

    const std::set<IpAddress> &AddrSet() const
    {
        return m_addrSet;
    }

private:
    std::set<IpAddress> m_addrSet;
};

class IpPrefix
{
public:
    IpPrefix() : m_addr(0), m_mask(0), m_maskLen(0) {}

    IpPrefix(const std::string &);

    const std::string to_string() const;

    IpAddress Addr() const
    {
        return m_addr;
    }

    IpAddress Mask() const
    {
        return m_mask;
    }

    int MaskLen() const
    {
        return m_maskLen;
    }

    uint32_t SubnetSize() const
    {
        uint32_t i = 1;

        for (int j = 0; j < 32 - m_maskLen; ++j)
        {
            i *= 2;
        }

        return i;
    }

    bool operator<(const IpPrefix &o) const;

private:
    IpAddress m_addr;
    IpAddress m_mask;
    int m_maskLen;
};

