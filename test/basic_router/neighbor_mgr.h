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

#include <string>
#include <map>
#include <sainexthop.h>

#include "log.h"
#include "ip.h"
#include "mac.h"
#include "basic_router.h"

class NextHopMgr;

struct NeighborEntry
{
    MacAddress macAddr;
    std::string intfAlias;
    sai_object_id_t nhid;
    sai_object_id_t rif_id;
};

class NeighborMgr
{
    std::map<IpAddress, NeighborEntry> m_ip2NbrMap;
    NextHopMgr* m_nhMgr;

public:
    NeighborMgr(NextHopMgr* nhMgr);

    bool Add(IpAddress ipAddr,
             MacAddress macAddr,
             std::string intfAlias,
             sai_object_id_t rif_id
            );
    bool Del(IpAddress ipAddr);
    bool EraseAll();
    void Show();

    const NeighborEntry* GetNeighborEntry(const IpAddress &) const;
};
