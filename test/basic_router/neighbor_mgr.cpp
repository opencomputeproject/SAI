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
#include "neighbor_mgr.h"
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

extern sai_neighbor_api_t* sai_neighbor_api;
extern sai_next_hop_api_t* sai_next_hop_api;

NeighborMgr::NeighborMgr(NextHopMgr* nhMgr) : m_nhMgr(nhMgr)
{
}

void NeighborMgr::Show()
{
    std::map<IpAddress, NeighborEntry>::const_iterator it;
    const NeighborEntry* nbentry;
    MacAddress mac;


    LOGG(TEST_DEBUG, NEIGHBOR, "\t--- --- --- --- --- --- Neighbor Entry Table --- --- --- --- --- --- \n");
    LOGG(TEST_DEBUG, NEIGHBOR, "\t%-15s %-20s  via %-10s %-14s %-14s\n", "station", "mac_addr", "intf", "rif_id", "next_hop_id");

    for (it = m_ip2NbrMap.begin(); it != m_ip2NbrMap.end(); it++)
    {
        nbentry = &it->second;
        mac =  nbentry->macAddr;
        LOGG(TEST_DEBUG, NEIGHBOR, "\t%-15s %-20s  via %-10s 0x%-12lx 0x%-12lx\n",
             it->first.to_string().c_str(),
             mac.to_string().c_str(),
             nbentry->intfAlias.c_str(),
             nbentry->rif_id,
             nbentry->nhid);
    }

    LOGG(TEST_DEBUG, NEIGHBOR, "\t--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---- \n");

}

bool NeighborMgr::Add(IpAddress ipAddr,
                      MacAddress macAddr,
                      std::string intfAlias,
                      sai_object_id_t rif_id)
{
    sai_status_t status;

    //Check the current Neighbor Record
    sai_neighbor_entry_t sainb;
    NeighborEntry nbEntry;
    nbEntry.macAddr = macAddr;
    nbEntry.intfAlias = intfAlias;
    std::map<IpAddress, NeighborEntry>::iterator itnb = m_ip2NbrMap.find(ipAddr);

    //Write to the ASIC
    // add new neighbor
    sainb.rif_id = rif_id;
    sainb.ip_address.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    sainb.ip_address.addr.ip4 = ipAddr.addr();

    sai_attribute_t rif_attr;
    rif_attr.id = SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS;
    memcpy(rif_attr.value.mac, macAddr.to_bytes(), 6);

    LOGG(TEST_INFO, NEIGHBOR, "sai_neighbor_api->create_neighbor_entry IPaddr[%s] MACaddr[%s] Interface[%s] rif_id[0x%lx]\n",
         ipAddr.to_string().c_str(),  macAddr.to_string().c_str(), intfAlias.c_str(), rif_id);

    status = sai_neighbor_api->create_neighbor_entry(&sainb, 1, &rif_attr);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, NEIGHBOR, "fail to create neighbor {%s %s} %s, rc=0x%x\n",
             ipAddr.to_string().c_str(), macAddr.to_string().c_str(), intfAlias.c_str(), -status);
        return false;
    }

    LOGG(TEST_INFO, NEIGHBOR, "sai_next_hop_api->create_next_hop IPaddr[%s] MACaddr[%s] Interface[%s] rif_id[0x%lx]\n",
         ipAddr.to_string().c_str(),  macAddr.to_string().c_str(), intfAlias.c_str(), rif_id);

    sai_object_id_t nhid;

    if (!m_nhMgr->Add(ipAddr, macAddr, intfAlias, rif_id))
    {
        LOGG(TEST_ERR, NEIGHBOR, "fail to add next_hop_id\n");
        return false;
    }

    const NextHopEntry *nhEntry = m_nhMgr->GetNextHopEntry(ipAddr);

    if (!nhEntry)
    {
        LOGG(TEST_ERR, NEIGHBOR, "fail to retrieve next_hop_id\n");
        return false;
    }

    nhid = nhEntry->nhid;
    LOGG(TEST_DEBUG, NEIGHBOR, "next_hop_id 0x%lx\n", nhid);

    nbEntry.rif_id = rif_id;
    nbEntry.nhid = nhid;
    m_ip2NbrMap[ipAddr] = nbEntry;

    return true;
}

bool NeighborMgr::Del(IpAddress ipAddr)
{
    sai_status_t status;
    sai_neighbor_entry_t sainb;

    const NeighborEntry *nbEntry = NeighborMgr::GetNeighborEntry(ipAddr);

    if (!nbEntry)
    {
        LOGG(TEST_DEBUG, NEIGHBOR, "cannot find %s in the Neighbor Table\n", ipAddr.to_string().c_str());
        return true;
    }

    if (!m_nhMgr->Del(ipAddr))
    {
        LOGG(TEST_INFO, NEIGHBOR, "fail to remove nexthop\n");
        return false;
    }


    sainb.rif_id = nbEntry->rif_id;
    sainb.ip_address.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    sainb.ip_address.addr.ip4 = ipAddr.addr();

    LOGG(TEST_INFO, NEIGHBOR, "sai_neighbor_api->remove_neighbor_entry ip %s rif_id 0x%lx \n",
         ipAddr.to_string().c_str(), nbEntry->rif_id);

    status = sai_neighbor_api->remove_neighbor_entry(&sainb);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, NEIGHBOR, "fail to remove neighbor ip %s, rc=0x%x\n", ipAddr.to_string().c_str(), -status);
        return false;
    }

    m_ip2NbrMap.erase(ipAddr);
    return true;
}

bool NeighborMgr::EraseAll()
{
    std::map<IpAddress, NeighborEntry>::iterator itnb;

    for (itnb = m_ip2NbrMap.begin(); itnb != m_ip2NbrMap.end(); ++itnb)
    {
        if (!NeighborMgr::Del(itnb->first))
        {
            return false;
        }
    }

    return true;
}

const NeighborEntry* NeighborMgr::GetNeighborEntry(const IpAddress &ip) const
{
    std::map<IpAddress, NeighborEntry>::const_iterator it = m_ip2NbrMap.find(ip);

    if (it != m_ip2NbrMap.end())
    {
        return &it->second;
    }
    else
    {
        return NULL;
    }
}
