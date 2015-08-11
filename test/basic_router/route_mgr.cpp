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
extern "C"
{
#define typeof(x) __typeof__(x)

#include <stdio.h>
#include "sai.h"
}

#include <vector>
#include <arpa/inet.h>

#include "ip.h"
#include "route_mgr.h"
#include "neighbor_mgr.h"
#include "nexthopgrp_mgr.h"

extern sai_neighbor_api_t* sai_neighbor_api;
extern sai_next_hop_api_t* sai_next_hop_api;
extern sai_next_hop_group_api_t* sai_next_hop_group_api;
extern sai_route_api_t* sai_route_api;

extern sai_object_id_t g_vr_id;

RouteMgr::RouteMgr(NeighborMgr* neighborMgr, NextHopGrpMgr* nhgMgr)
{
    m_neighborMgr = neighborMgr;
    m_nhgMgr = nhgMgr;
    // setup black hole
    IpAddresses ipaddrs("0.0.0.0");
    m_EcmpGroups[ipaddrs] = 0;
}


void RouteMgr::Show()
{
    LOGG(TEST_DEBUG, ROUTE, "\t--- --- --- --- --- --- Routes Synced --- --- --- --- --- --- ---\n");
    LOGG(TEST_DEBUG, ROUTE, "\t%-40s | %s\n", "route", "nexthops");

    for (RouteTable::const_iterator it = m_Routes.begin(); it != m_Routes.end(); it++)
    {
        IpPrefix prefix = it->first;
        LOGG(TEST_DEBUG, ROUTE, "\t%-40s | %s\n",
             prefix.to_string().c_str(),
             it->second.to_string().c_str());
    }

    LOGG(TEST_DEBUG, ROUTE, "\t--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -\n");
}

void RouteMgr::ShowECMP()
{
    LOGG(TEST_DEBUG, ROUTE, "\t--- --- --- --- --- --- ECMP Group Table --- --- --- --- --- --- \n");
    LOGG(TEST_DEBUG, ROUTE, "\t%-40s | %s\n", "nexthops", "next_hop_group_id");
    std::map<IpAddresses, sai_object_id_t>::iterator itnhg;

    for (itnhg = m_EcmpGroups.begin(); itnhg != m_EcmpGroups.end(); itnhg++)
    {
        IpAddresses nexthops = itnhg->first;
        LOGG(TEST_DEBUG, ROUTE, "\t%-40s | 0x%lx\n",
             nexthops.to_string().c_str(),
             itnhg->second);
    }

    LOGG(TEST_DEBUG, ROUTE, "\t--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -\n");
}

bool RouteMgr::Add(IpPrefix prefix, IpAddresses nexthops)
{
    sai_status_t status;
    sai_object_id_t nhg_id;
    std::vector<sai_object_id_t> nhids;

    std::set<IpAddress> addrset = nexthops.AddrSet();

    std::vector<sai_attribute_t> nhg_attrs;
    std::map<IpAddresses, sai_object_id_t>::iterator itnhg = m_EcmpGroups.find(nexthops);

    if (itnhg == m_EcmpGroups.end())
    {
        for (std::set<IpAddress>::const_iterator itnh = addrset.begin(); itnh != addrset.end(); itnh++)
        {
            const NeighborEntry *nbEntry = m_neighborMgr->GetNeighborEntry(*itnh);

            if (!nbEntry)
            {
                LOGG(TEST_ERR, ROUTE, "fail to find the NeiborEntry for nexthop %s\n", itnh->to_string().c_str());
                continue;
            }

            nhg_id = nbEntry->nhid;
            nhids.push_back(nbEntry->nhid);
        }

        if (nhids.size() == 0)
        {
            LOGG(TEST_DEBUG, ROUTE, "cannot find the any of nexthops %s in the neighbor table\n", nexthops.to_string().c_str());
            return false;
        }

        if (nhids.size() > 1)
        {
            if (!m_nhgMgr->Add(nexthops))
            {
                LOGG(TEST_ERR, ROUTE, "fail to add next hop group %s\n", nexthops.to_string().c_str());
                return false;
            }

            const NextHopGrpEntry *nhgEntry = m_nhgMgr->GetNextHopGrpEntry(nexthops);

            if (!nhgEntry)
            {
                LOGG(TEST_ERR, ROUTE, "fail to retrieve next hop group %s\n", nexthops.to_string().c_str());
                return false;
            }

            nhg_id = nhgEntry->nhg_id;
        }

        m_EcmpGroups[nexthops] = nhg_id;
    }
    else
    {
        nhg_id = m_EcmpGroups[nexthops];
    }


    sai_unicast_route_entry_t unicast_route_entry;
    unicast_route_entry.vr_id = g_vr_id;
    unicast_route_entry.destination.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    unicast_route_entry.destination.addr.ip4 = prefix.Addr().addr();
    unicast_route_entry.destination.mask.ip4 = prefix.Mask().addr();

    sai_attribute_t route_attr;

    if (nexthops.size() == 1 &&
            nexthops == IpAddresses("0.0.0.0"))
    {
        route_attr.id = SAI_ROUTE_ATTR_PACKET_ACTION;
        route_attr.value.s32 = SAI_PACKET_ACTION_DROP;
    }
    else
    {
        route_attr.id = SAI_ROUTE_ATTR_NEXT_HOP_ID;
        route_attr.value.oid = nhg_id;
    }

    if (m_Routes.find(prefix) == m_Routes.end())
    {
        LOGG(TEST_INFO, ROUTE, "sai_route_api->create_route %s | nexthops %s\n",
             prefix.to_string().c_str(), nexthops.to_string().c_str());

        status = sai_route_api->create_route(&unicast_route_entry, 1, &route_attr);

        if (status != SAI_STATUS_SUCCESS)
        {
            LOGG(TEST_ERR, ROUTE, "fail to create route for %s, nexthop(s) are %s rc=0x%x\n",
                 prefix.to_string().c_str(),
                 nexthops.to_string().c_str(), -status);
            return false;
        }
    }
    else
    {
        LOGG(TEST_INFO, ROUTE, "sai_route_api->set_route_attribute %s | nexthops %s\n",
             prefix.to_string().c_str(), nexthops.to_string().c_str());

        status = sai_route_api->set_route_attribute(&unicast_route_entry, &route_attr);

        if (status != SAI_STATUS_SUCCESS)
        {
            LOGG(TEST_ERR, ROUTE, "fail to set nexthop(s) %s for route %s, rc=0x%x",
                 nexthops.to_string().c_str(),
                 prefix.to_string().c_str(), -status);
            return false;
        }

        return true;
    }

    m_Routes[prefix] = nexthops;

    return true;
}

bool RouteMgr::Del(IpPrefix prefix)
{

    RouteTable::const_iterator it;
    it = m_Routes.find(prefix);
    IpAddresses nexthops;

    if (it == m_Routes.end())
    {
        LOGG(TEST_DEBUG, ROUTE, "cannot find route %s in the route table\n", prefix.to_string().c_str());
        return true;
    }


    LOGG(TEST_INFO, ROUTE, "sai_route_api->remove_route %s \n",
         prefix.to_string().c_str());

    sai_unicast_route_entry_t unicast_route_entry;
    unicast_route_entry.vr_id = g_vr_id;
    unicast_route_entry.destination.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    unicast_route_entry.destination.addr.ip4 = prefix.Addr().addr();
    unicast_route_entry.destination.mask.ip4 = prefix.Mask().addr();

    sai_status_t status = sai_route_api->remove_route(&unicast_route_entry);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, ROUTE, "failed to remove route for %s, rc=0x%x\n", prefix.to_string().c_str(), -status);
        return false;
    }


    sai_object_id_t nhg_id;
    nexthops = m_Routes[prefix];

    nhg_id = m_EcmpGroups[nexthops];

    //skip the entry for blackhole
    if (nhg_id == 0)
    {
        m_Routes.erase(prefix);
        return true;
    }

    //On this field, there could be next hop id and next hop group id.
    //the following handles only next hop group id
    if (SAI_OID_TYPE_CHECK(nhg_id, SAI_OBJECT_TYPE_NEXT_HOP_GROUP))
    {
        LOGG(TEST_INFO, ROUTE, "remove nexthopgrp id 0x%lx\n", nhg_id);

        if (!m_nhgMgr->Del(nexthops))
        {
            LOGG(TEST_ERR, ROUTE, "failed to remove nexthopgrp id 0x%lx\n", nhg_id);
            return false;
        }
    }

    m_EcmpGroups.erase(nexthops);
    m_Routes.erase(prefix);

    for (it = m_Routes.begin(); it != m_Routes.end(); ++it)
    {
        if (it->second == nexthops)
        {
            break;
        }
    }

    return true;
}


bool RouteMgr::EraseAll()
{
    RouteTable::const_iterator it;

    for (it = m_Routes.begin(); it != m_Routes.end(); ++it)
    {
        if (!RouteMgr::Del(it->first))
        {
            return false;
        }
    }

    return true;
}
