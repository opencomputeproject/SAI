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
#include <arpa/inet.h>
#include <string>
#include <stdexcept>

#include "ip.h"

IpAddress::IpAddress(const std::string &ipstr)
{
    if (inet_pton(AF_INET, ipstr.c_str(), &m_addr) != 1)
    {
        std::string errmsg = "cannot convert " + ipstr + " to ip address";
        throw std::invalid_argument(errmsg);
    }
}

const std::string IpAddress::to_string() const
{
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &m_addr, str, INET_ADDRSTRLEN);
    std::string addrstr(str);
    return addrstr;
}

IpAddresses::IpAddresses(const std::string &ipListStr)
{
    size_t pos = 0;
    size_t nextpos;
    std::string ipStr;

    while ((nextpos = ipListStr.find(',', pos)) != std::string::npos)
    {
        ipStr = ipListStr.substr(pos, nextpos - pos);

        if (!ipStr.empty())
        {
            IpAddress ip(ipStr);
            m_addrSet.insert(ip);
        }

        pos = nextpos + 1;
    }

    ipStr = ipListStr.substr(pos);

    if (!ipStr.empty())
    {
        IpAddress ip(ipStr);
        m_addrSet.insert(ip);
    }
}

void IpAddresses::add(const std::string &ipstr)
{
    IpAddress ip(ipstr);
    m_addrSet.insert(ip);
}

const std::string IpAddresses::to_string() const
{
    std::string addrList;

    for (std::set<IpAddress>::iterator it = m_addrSet.begin(); it != m_addrSet.end(); ++it)
    {
        if (it != m_addrSet.begin())
        {
            addrList += ",";
        }

        addrList += it->to_string();
    }

    return addrList;
}

bool IpAddresses::operator<(const IpAddresses &o) const
{
    return (m_addrSet < o.m_addrSet);
}

IpPrefix::IpPrefix(
    const std::string &prefix)
{
    size_t pos = prefix.find('/');
    std::string ipStr = prefix.substr(0, pos);

    if (ipStr.empty())
    {
        m_addr = 0;
    }
    else
    {
        m_addr = IpAddress(ipStr);
    }

    std::string maskStr = prefix.substr(pos + 1);
    m_maskLen = std::stoi(maskStr);

    if (m_maskLen < 0 || m_maskLen > 32)
    {
        std::string errmsg = "cannot convert " + ipStr + " to ip prefix";
        throw std::invalid_argument(errmsg);
    }

    m_mask = htonl(((uint64_t)0xFFFFFFFF << (32 - m_maskLen)) & 0xFFFFFFFF);
}

bool IpPrefix::operator<(const IpPrefix &o) const
{
    uint64_t addrmask = ((uint64_t)m_addr.addr()) << 32 | m_mask.addr();
    uint64_t o_addrmask = ((uint64_t)o.m_addr.addr()) << 32 | o.m_mask.addr();

    if (addrmask < o_addrmask)
        return true;

    return false;
}

const std::string IpPrefix::to_string() const
{
    return (m_addr.to_string() + "/" + m_mask.to_string());
}
