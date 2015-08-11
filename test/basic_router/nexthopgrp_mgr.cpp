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

#include "nexthopgrp_mgr.h"
#include "neighbor_mgr.h"

extern sai_next_hop_group_api_t* sai_next_hop_group_api;

NextHopGrpMgr::NextHopGrpMgr(NeighborMgr* neighborMgr) : m_neighborMgr(neighborMgr)
{
}

void NextHopGrpMgr::Show()
{
    std::map<IpAddresses, NextHopGrpEntry>::const_iterator it;
    const NextHopGrpEntry* nhgEntry;

    LOGG(TEST_DEBUG, NXTHG, "\t--- --- --- --- --- --- NextHopGroup Entry Table --- --- --- --- --- --- \n");
    LOGG(TEST_DEBUG, NXTHG, "\t%-14s    %s\n", "next_hop_grp_id", "nexthops");

    for (it = m_ips2NextHGMap.begin(); it != m_ips2NextHGMap.end(); it++)
    {
        nhgEntry = &it->second;

        LOGG(TEST_DEBUG, NXTHG, "\t0x%-12lx     %s\n",
             nhgEntry->nhg_id,
             it->first.to_string().c_str());
    }

    LOGG(TEST_DEBUG, NXTHG, "\t--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---- \n");
}

bool NextHopGrpMgr::Add(IpAddresses nextHops)
{
    sai_status_t status;
    NextHopGrpEntry nhgEntry;
    nhgEntry.nextHops = nextHops;

    std::map<IpAddresses, NextHopGrpEntry>::iterator itnhg = m_ips2NextHGMap.find(nextHops);

    //create Next Hop Group
    sai_object_id_t nhg_id;
    std::vector<sai_object_id_t> nhids;

    std::set<IpAddress> addrset = nextHops.AddrSet();

    std::vector<sai_attribute_t> nhg_attrs;
    sai_attribute_t nhg_attr;

    //walkthrough the nexthops
    for (std::set<IpAddress>::const_iterator itnh = addrset.begin(); itnh != addrset.end(); itnh++)
    {
        const NeighborEntry *nbEntry = m_neighborMgr->GetNeighborEntry(*itnh);

        if (!nbEntry)
        {
            LOGG(TEST_ERR, NXTHG, "fail to find the NeiborEntry for nexthop %s\n", itnh->to_string().c_str());
            continue;
        }

        nhg_id = nbEntry->nhid;
        nhids.push_back(nbEntry->nhid);
    }

    //nexthops contain 0 neighbors
    if (nhids.size() == 0)
    {
        LOGG(TEST_DEBUG, NXTHG, "cannot find the any of nexthops %s in the neighbor table\n", nextHops.to_string().c_str());
        return false;
    }

    if (nhids.size() > 1)
    {
        nhg_attr.id = SAI_NEXT_HOP_GROUP_ATTR_TYPE;
        nhg_attr.value.s32 = SAI_NEXT_HOP_GROUP_ECMP;
        nhg_attrs.push_back(nhg_attr);

        nhg_attr.id = SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST;
        nhg_attr.value.objlist.count = nhids.size();
        nhg_attr.value.objlist.list = nhids.data();
        nhg_attrs.push_back(nhg_attr);

        LOGG(TEST_INFO, NXTHG, "sai_next_hop_group_api->create_next_hop_group %s\n",  nextHops.to_string().c_str());
        status = sai_next_hop_group_api->create_next_hop_group(&nhg_id, nhg_attrs.size(), nhg_attrs.data());

        if (status != SAI_STATUS_SUCCESS)
        {
            LOGG(TEST_ERR, NXTHG, "fail to create ECMP group for %s. status=0x%x\n", nextHops.to_string().c_str(), -status);
            return false;
        }

        if (!SAI_OID_TYPE_CHECK(nhg_id, SAI_OBJECT_TYPE_NEXT_HOP_GROUP))
        {
            LOGG(TEST_ERR, NXTHG, "next hop group oid generated is not the right type\n");
            return false;
        }

        nhgEntry.nhg_id = nhg_id;

        LOGG(TEST_DEBUG, NXTHG, "create ECMP groupnexthops %s nhg_id 0x%lx\n",
             nextHops.to_string().c_str(), nhg_id);
    }

    //insert this entry to the internal data structure
    m_ips2NextHGMap[nextHops] = nhgEntry;
    return true;
}

bool NextHopGrpMgr::Del(IpAddresses nextHops)
{
    sai_object_id_t nhg_id;
    sai_status_t status;

    const NextHopGrpEntry *nhgEntry = NextHopGrpMgr::GetNextHopGrpEntry(nextHops);

    if (!nhgEntry)
    {
        return false;
    }

    nhg_id = nhgEntry->nhg_id;

    LOGG(TEST_INFO, NXTHG, "sai_next_hop_group_api->sai_remove_next_hop_group nhg_id 0x%lx \n", nhg_id);

    status = sai_next_hop_group_api->remove_next_hop_group(nhg_id);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, ROUTE, "failed to remove nhg_id 0x%lx rc=0x%x\n", nhg_id, -status);
        return false;
    }

    m_ips2NextHGMap.erase(nextHops);

    return true;
}

const NextHopGrpEntry* NextHopGrpMgr::GetNextHopGrpEntry(const IpAddresses &ips) const
{
    std::map<IpAddresses, NextHopGrpEntry>::const_iterator it = m_ips2NextHGMap.find(ips);

    if (it != m_ips2NextHGMap.end())
    {
        return &it->second;
    }
    else
    {
        return NULL;
    }
}

