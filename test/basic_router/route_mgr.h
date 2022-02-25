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
#include <sairoute.h>
#include <sainexthop.h>
#include <sainexthopgroup.h>
}

#include "log.h"
#include "ip.h"
#include "basic_router.h"


class NeighborMgr;
class NextHopGrpMgr;

typedef std::map<IpPrefix, IpAddresses> RouteTable;

class RouteMgr
{
    NeighborMgr* m_neighborMgr;
    NextHopGrpMgr* m_nhgMgr;

    RouteTable m_Routes;

    std::map<IpAddresses, sai_object_id_t> m_EcmpGroups;

public:
    RouteMgr(NeighborMgr* neighborMgr, NextHopGrpMgr* nhgMgr);

    bool Add(IpPrefix prefix, IpAddresses nexthops);
    bool Del(IpPrefix prefix);
    bool EraseAll();
    void Show();
    void ShowECMP();
};
