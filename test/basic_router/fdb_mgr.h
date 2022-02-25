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
#include <vector>
#include <saitypes.h>
#include <saifdb.h>

#include "log.h"
#include "ip.h"
#include "mac.h"
#include "basic_router.h"

struct FdbEntry
{
    MacAddress macAddr;
    sai_uint32_t vlan_id;
    sai_int32_t type;
    sai_object_id_t port_id;
    sai_int32_t pkt_action;
};

class FdbMgr
{
    std::vector<FdbEntry> m_FdbVector;

public:
    bool Add(MacAddress macAddr,
             sai_uint32_t vlan_id,
             sai_int32_t type,
             sai_object_id_t port_id,
             sai_int32_t pkt_action);
    bool Del(MacAddress macAddr,
             sai_uint32_t vlan_id);
    bool EraseAll();
    void Show();

    const FdbEntry* GetFdbEntry(const MacAddress &, const sai_uint32_t &) const;
};
