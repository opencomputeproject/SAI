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

#include "nexthop_mgr.h"
#include "ip.h"
#include <saistatus.h>
#include <saineighbor.h>
#include <sainexthop.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <arpa/inet.h>
#include <string.h>

extern sai_next_hop_api_t* sai_next_hop_api;

void NextHopMgr::Show()
{
    std::map<IpAddress, NextHopEntry>::const_iterator it;
    const NextHopEntry* nhEntry;
    MacAddress mac;


    LOGG(TEST_DEBUG, NEXTHOP, "\t--- --- --- --- --- --- NextHop Entry Table --- --- --- --- --- --- \n");
    LOGG(TEST_DEBUG, NEXTHOP, "\t%-15s %-20s  via %-10s %-14s %-14s\n", "station", "mac_addr", "intf", "rif_id", "next_hop_id");

    for (it = m_ip2NextHopMap.begin(); it != m_ip2NextHopMap.end(); it++)
    {
        nhEntry = &it->second;
        mac =  nhEntry->macAddr;
        LOGG(TEST_DEBUG, NEXTHOP, "\t%-15s %-20s  via %-10s 0x%-12lx 0x%-12lx\n",
             it->first.to_string().c_str(),
             mac.to_string().c_str(),
             nhEntry->intfAlias.c_str(),
             nhEntry->rif_id,
             nhEntry->nhid);
    }

    LOGG(TEST_DEBUG, NEXTHOP, "\t--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---- \n");

}

bool NextHopMgr::Add(IpAddress ipAddr,
                     MacAddress macAddr,
                     std::string intfAlias,
                     sai_object_id_t rif_id)
{
    sai_status_t status;

    if (!SAI_OID_TYPE_CHECK(rif_id, SAI_OBJECT_TYPE_ROUTER_INTERFACE))
    {
        LOGG(TEST_ERR, NEXTHOP, "router interface id is not the right type\n");
        return false;
    }

    //Check the current NextHop Record
    NextHopEntry nhEntry;
    nhEntry.macAddr = macAddr;
    nhEntry.intfAlias = intfAlias;
    std::map<IpAddress, NextHopEntry>::iterator itnh = m_ip2NextHopMap.find(ipAddr);

    if (itnh != m_ip2NextHopMap.end())
    {
        // do nothing when same neighbor already exists, otherwise remove existing neighbor
        if (itnh->second.macAddr == nhEntry.macAddr &&
                itnh->second.intfAlias == nhEntry.intfAlias)
        {
            LOGG(TEST_DEBUG, NEXTHOP, "the same nexthop entry already exists \n");
            return true;
        }
    }

    sai_object_id_t nhid;

    sai_attribute_t nhattrs[3];
    nhattrs[0].id = SAI_NEXT_HOP_ATTR_TYPE;
    nhattrs[0].value.u64 = SAI_NEXT_HOP_IP;
    nhattrs[1].id = SAI_NEXT_HOP_ATTR_IP;
    nhattrs[1].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    nhattrs[1].value.ipaddr.addr.ip4 = ipAddr.addr();
    nhattrs[2].id = SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID;
    nhattrs[2].value.oid = rif_id;
    status = sai_next_hop_api->create_next_hop(&nhid, 3, nhattrs);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, NEXTHOP, "fail to create next hop %-20s %-20s %s, rc=0x%x\n",
             ipAddr.to_string().c_str(), macAddr.to_string().c_str(), intfAlias.c_str(), -status);
        return false;
    }

    if (!SAI_OID_TYPE_CHECK(nhid, SAI_OBJECT_TYPE_NEXT_HOP))
    {
        LOGG(TEST_ERR, NEXTHOP, "next hop oid generated is not the right type\n");
        return false;
    }

    LOGG(TEST_DEBUG, NEXTHOP, "next_hop_id 0x%lx\n", nhid);

    nhEntry.rif_id = rif_id;
    nhEntry.nhid = nhid;
    m_ip2NextHopMap[ipAddr] = nhEntry;

    return true;
}

bool NextHopMgr::Del(IpAddress ipAddr)
{
    sai_status_t status;
    sai_object_id_t nhid;

    const NextHopEntry *nhEntry = NextHopMgr::GetNextHopEntry(ipAddr);

    if (!nhEntry)
    {
        LOGG(TEST_DEBUG, NEXTHOP, "cannot find %s in the NextHop Table\n", ipAddr.to_string().c_str());
        return true;
    }

    nhid = nhEntry->nhid;

    LOGG(TEST_INFO, NEXTHOP, "sai_next_hop_api->remove_next_hop nhid 0x%lx \n", nhid);

    status = sai_next_hop_api->remove_next_hop(nhid);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, NEXTHOP, "fail to remove nexthop 0x%lx, rc=0x%x\n", nhid, -status);
        return false;
    }

    m_ip2NextHopMap.erase(ipAddr);
    return true;
}

bool NextHopMgr::EraseAll()
{
    std::map<IpAddress, NextHopEntry>::iterator itnh;

    for (itnh = m_ip2NextHopMap.begin(); itnh != m_ip2NextHopMap.end(); ++itnh)
    {
        if (!NextHopMgr::Del(itnh->first))
        {
            return false;
        }
    }

    return true;
}

const NextHopEntry* NextHopMgr::GetNextHopEntry(const IpAddress &ip) const
{
    std::map<IpAddress, NextHopEntry>::const_iterator it = m_ip2NextHopMap.find(ip);

    if (it != m_ip2NextHopMap.end())
    {
        return &it->second;
    }
    else
    {
        return NULL;
    }
}
