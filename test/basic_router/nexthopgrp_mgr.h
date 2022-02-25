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

#include <set>
#include <map>
#include <string>

extern "C"
{
#include <sainexthopgroup.h>
}

#include "log.h"
#include "ip.h"
#include "mac.h"
#include "basic_router.h"

class NeighborMgr;

struct NextHopGrpEntry
{
    IpAddresses nextHops;
    sai_object_id_t nhg_id;
};

class NextHopGrpMgr
{

    NeighborMgr* m_neighborMgr;

    std::map<IpAddresses, NextHopGrpEntry> m_ips2NextHGMap;

public:
    NextHopGrpMgr(NeighborMgr* neighborMgr);

    bool Add(IpAddresses nextHops);
    bool Del(IpAddresses nextHops);
    void Show();

    const NextHopGrpEntry* GetNextHopGrpEntry(const IpAddresses &) const;
};

