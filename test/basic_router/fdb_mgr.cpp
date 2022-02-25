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
#include "fdb_mgr.h"
#include "ip.h"
#include <sai.h>
#include <saitypes.h>
#include <saistatus.h>
#include <saifdb.h>

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

extern sai_fdb_api_t* sai_fdb_api;

void FdbMgr::Show()
{
    const FdbEntry* fdbEntry;
    MacAddress mac;
    std::vector<FdbEntry>::iterator it;

    LOGG(TEST_DEBUG, FDB, "\t--- --- --- --- --- --- Fdb Entry Table --- --- --- --- --- --- \n");
    LOGG(TEST_DEBUG, FDB, "\t{%-20s %-10s} {%-10s %-14s %-10s}\n", "mac", "valn_id", "type", "port id", "pkt act");

    for (it = m_FdbVector.begin(); it != m_FdbVector.end(); ++it)
    {
        fdbEntry = &(*it);
        mac = fdbEntry->macAddr;
        LOGG(TEST_DEBUG, FDB, "\t{%-20s %-10hu} {%-10s 0x%-12lx %-10s}\n",
             mac.to_string().c_str(),
             fdbEntry->vlan_id,
             (fdbEntry->type == SAI_FDB_ENTRY_STATIC) ? "STATIC" : "DYNAMIC",
             fdbEntry->port_id,
             (fdbEntry->pkt_action == SAI_PACKET_ACTION_FORWARD) ? "FORWARD" :
             (fdbEntry->pkt_action == SAI_PACKET_ACTION_DROP) ? "DROP" :
             (fdbEntry->pkt_action == SAI_PACKET_ACTION_TRAP) ? "TRAP" : "LOG");
    }

    LOGG(TEST_DEBUG, FDB, "\t--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- \n");
}

bool FdbMgr::Add( MacAddress macAddr,
                  sai_uint32_t vlan_id,
                  sai_int32_t type,
                  sai_object_id_t port_id,
                  sai_int32_t pkt_action)
{
    FdbEntry fdbEntry;

    fdbEntry.macAddr = macAddr;
    fdbEntry.vlan_id = vlan_id;
    fdbEntry.type = type;
    fdbEntry.port_id = port_id;
    fdbEntry.pkt_action = pkt_action;

    LOGG(TEST_INFO, FDB, "lookup fdb_entry {mac %-15s vlan_id %hu} \n",
         macAddr.to_string().c_str(), vlan_id);

    for (std::vector<FdbEntry>::iterator it = m_FdbVector.begin();
            it != m_FdbVector.end(); ++it)
    {
        if (it->macAddr == macAddr && it->vlan_id == vlan_id )
        {
            LOGG(TEST_DEBUG, FDB, "fdb_entry {mac %-15s vlan_id %hu} already exists\n",
                 macAddr.to_string().c_str(), vlan_id);
            return true;
        }
    }

    sai_status_t status;

    sai_fdb_entry_t saifdbent;
    sai_attribute_t fdbattrs[3];

    fdbattrs[0].id = SAI_FDB_ENTRY_ATTR_TYPE;
    fdbattrs[0].value.s32 = type;
    fdbattrs[1].id = SAI_FDB_ENTRY_ATTR_PORT_ID;
    fdbattrs[1].value.oid = port_id;
    fdbattrs[2].id = SAI_FDB_ENTRY_ATTR_PACKET_ACTION;
    fdbattrs[2].value.s32 = pkt_action;

    memcpy(saifdbent.mac_address, macAddr.to_bytes(), sizeof(sai_mac_t));
    saifdbent.vlan_id = vlan_id;

    LOGG(TEST_INFO, FDB, "create sai_fdb_entry {mac %-15s vlan_id %hu}\n",
         macAddr.to_string().c_str(), saifdbent.vlan_id);
    status = sai_fdb_api->create_fdb_entry(&saifdbent, 3, fdbattrs);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, FDB, "fail to create sai_fdb_entry {mac %-15s vlan_id %hu}\n",
             macAddr.to_string().c_str(), saifdbent.vlan_id);
        return false;
    }

    m_FdbVector.push_back(fdbEntry);

    return true;
}

bool FdbMgr::Del(MacAddress macAddr,
                 sai_uint32_t vlan_id)
{
    sai_status_t status;
    sai_fdb_entry_t saifdbent;


    std::vector<FdbEntry>::iterator it;

    for (it = m_FdbVector.begin(); it != m_FdbVector.end(); ++it)
    {
        if ((it->macAddr == macAddr) && (it->vlan_id == vlan_id))
        {
            break;
        }
    }

    if (it == m_FdbVector.end() )
    {
        LOGG(TEST_DEBUG, FDB, "fdb_entry {mac %-15s vlan_id %hu} does not exist\n",
             macAddr.to_string().c_str(), vlan_id);

        return true;
    }

    memcpy(saifdbent.mac_address, macAddr.to_bytes(), sizeof(sai_mac_t));
    saifdbent.vlan_id = it->vlan_id;

    LOGG(TEST_INFO, FDB, "remove sai_fdb_entry {mac %-15s vlan_id %hu}\n",
         macAddr.to_string().c_str(), saifdbent.vlan_id);

    status = sai_fdb_api->remove_fdb_entry(&saifdbent);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, FDB, "fail to remove sai_fdb_entry {mac %-15s vlan_id %hu}\n",
             macAddr.to_string().c_str(), saifdbent.vlan_id);

        return false;
    }

    m_FdbVector.erase(it);

    return true;
}

bool FdbMgr::EraseAll()
{
    sai_status_t status;
    sai_fdb_entry_t saifdbent;
    MacAddress macAddr;

    std::vector<FdbEntry>::const_iterator it;

    for (it = m_FdbVector.begin(); it != m_FdbVector.end(); it++)
    {
        macAddr = it->macAddr;
        memcpy(saifdbent.mac_address, macAddr.to_bytes(), sizeof(sai_mac_t));
        saifdbent.vlan_id = it->vlan_id;

        LOGG(TEST_INFO, FDB, "remove sai_fdb_entry {mac %-15s vlan_id %hu}\n",
             macAddr.to_string().c_str(), saifdbent.vlan_id);

        status = sai_fdb_api->remove_fdb_entry(&saifdbent);

        if (status != SAI_STATUS_SUCCESS)
        {
            LOGG(TEST_ERR, FDB, "fail to remove sai_fdb_entry {mac %-15s vlan_id %hu}\n",
                 macAddr.to_string().c_str(), saifdbent.vlan_id);
            return false;
        }
    }

    m_FdbVector.clear();
    return true;
}

const FdbEntry* FdbMgr::GetFdbEntry(const MacAddress &mac, const sai_uint32_t &vlan_id) const
{
    std::vector<FdbEntry>::const_iterator it;

    for (it = m_FdbVector.begin(); it != m_FdbVector.end(); ++it)
    {
        if ((it->macAddr == mac) && (it->vlan_id == vlan_id))
        {
            break;
        }
    }

    if (it != m_FdbVector.end())
    {
        return &(*it);
    }
    else
    {
        return NULL;
    }
}
