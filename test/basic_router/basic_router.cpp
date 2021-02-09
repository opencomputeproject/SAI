/*
 * Copyright (c) 2014 Microsoft Open Technologies, Inc.
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
#include <limits.h>
#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>


#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include "log.h"
#include "mac.h"
#include "ip.h"
#include "neighbor_mgr.h"
#include "route_mgr.h"
#include "nexthopgrp_mgr.h"
#include "nexthop_mgr.h"
#include "fdb_mgr.h"
#include "basic_router.h"


using namespace std;
using ::testing::InitGoogleTest;

#define UNREFERENCED_PARAMETER(P)   (P)
#define PANEL_PORT_VLAN_START   1024
#define MAX_PORT                256
#define MAX_TEST                4

/*--------------------------------------------------------*/
//definition of the api tables
sai_switch_api_t* sai_switch_api;
sai_port_api_t* sai_port_api;
sai_vlan_api_t* sai_vlan_api;
sai_virtual_router_api_t* sai_vr_api;
sai_router_interface_api_t* sai_rif_api;
sai_hostif_api_t* sai_hif_api;
sai_neighbor_api_t* sai_neighbor_api;
sai_route_api_t* sai_route_api;
sai_next_hop_api_t* sai_next_hop_api;
sai_next_hop_group_api_t* sai_next_hop_group_api;
sai_fdb_api_t* sai_fdb_api;

sai_switch_notification_t plat_switch_notification_handlers =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

/*--------------------------------------------------------*/
//Profile Services

const char* test_profile_get_value(
    _In_ sai_switch_profile_id_t profile_id,
    _In_ const char* variable)
{
    UNREFERENCED_PARAMETER(profile_id);
    UNREFERENCED_PARAMETER(variable);

    return NULL;
}

int test_profile_get_next_value(
    _In_ sai_switch_profile_id_t profile_id,
    _Out_ const char** variable,
    _Out_ const char** value)
{
    UNREFERENCED_PARAMETER(profile_id);
    UNREFERENCED_PARAMETER(variable);
    UNREFERENCED_PARAMETER(value);

    return -1;
}

const service_method_table_t test_services =
{
    test_profile_get_value,
    test_profile_get_next_value
};

/*--------------------------------------------------------*/
// Global variables
MacAddress mac;

sai_object_id_t g_vr_id;
unsigned int g_testcount = MAX_TEST;

std::string g_intfAlias[MAX_PORT];
IpAddress   g_ipAddr[MAX_PORT];
IpAddress   g_ipMask[MAX_PORT];
MacAddress  g_macAddr[MAX_PORT];
sai_object_id_t g_rif_id[MAX_PORT];
MacAddress  g_dst_mac[MAX_PORT];

NextHopMgr* nexthop_mgr;
NextHopGrpMgr* nexthopgrp_mgr;
NeighborMgr* neighbor_mgr;
RouteMgr* route_mgr;
FdbMgr* fdb_mgr;

std::vector<sai_object_id_t> vlan_member_list;

/*--------------------------------------------------------*/
//L3 Interface Initialization
static bool setup_one_l3_interface(sai_vlan_id_t vlanid,
                                   int port_count,
                                   const sai_object_id_t *port_list,
                                   const MacAddress mac,
                                   const IpAddress ipaddr,
                                   const IpAddress ipmask,
                                   sai_object_id_t &rif_id)
{

    LOGG(TEST_INFO, SETL3, "sai_vlan_api->create_vlan, create vlan %hu.\n", vlanid);
    sai_status_t status = sai_vlan_api->create_vlan(vlanid);

    if (status != SAI_STATUS_SUCCESS && status != SAI_STATUS_ITEM_ALREADY_EXISTS)
    {
        LOGG(TEST_ERR, SETL3, "fail to create vlan %hu. status=0x%x\n", vlanid, -status);
        return false;
    }

    std::vector<sai_attribute_t> member_attrs;
    sai_attribute_t member_attr;
    sai_object_id_t vlan_member_id;
    
    for (int i = 0; i < port_count; ++i)
    {
        member_attr.id = SAI_VLAN_MEMBER_ATTR_VLAN_ID;
        member_attr.value.u16 = vlanid;
        member_attrs.push_back(member_attr);
        
        member_attr.id = SAI_VLAN_MEMBER_ATTR_PORT_ID;
        member_attr.value.oid =  port_list[i];
        member_attrs.push_back(member_attr);

        member_attr.id = SAI_VLAN_MEMBER_ATTR_TAGGING_MODE;
        member_attr.value.s32 = SAI_VLAN_PORT_UNTAGGED;
        member_attrs.push_back(member_attr);

        LOGG(TEST_INFO, SETL3, "sai_vlan_api->create_vlan_member, with vlan %d.\n", vlanid);
        status = sai_vlan_api->create_vlan_member(&vlan_member_id, member_attrs.size(), member_attrs.data());
        if (status != SAI_STATUS_SUCCESS)
        {
            LOGG(TEST_ERR, SETL3, "fail to create member vlan %hu. status=0x%x\n",  vlanid, -status);
            return false;
        }
        vlan_member_list.push_back(vlan_member_id);
    }

    sai_attribute_t attr;
    attr.id = SAI_PORT_ATTR_PORT_VLAN_ID;
    attr.value.u16 = vlanid;

    for (int i = 0; i < port_count; ++i)
    {
        LOGG(TEST_INFO, SETL3, "sai_port_api->set_port_attribute SAI_PORT_ATTR_PORT_VLAN_ID %hu to port 0x%lx\n",
             vlanid,  port_list[i]);
        status = sai_port_api->set_port_attribute(port_list[i], &attr);

        if (status != SAI_STATUS_SUCCESS)
        {
            LOGG(TEST_ERR, SETL3, "fail to set port %lu untagged vlan %hu. status=0x%x\n", port_list[i], vlanid, -status);
            return false;
        }
    }

    // create router interface
    std::vector<sai_attribute_t> rif_attrs;
    sai_attribute_t rif_attr;

    rif_attr.id = SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID;
    rif_attr.value.oid = g_vr_id;
    rif_attrs.push_back(rif_attr);

    rif_attr.id = SAI_ROUTER_INTERFACE_ATTR_TYPE;
    rif_attr.value.s32 = SAI_ROUTER_INTERFACE_TYPE_VLAN;
    rif_attrs.push_back(rif_attr);

    rif_attr.id = SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS;
    memcpy(rif_attr.value.mac, mac.to_bytes(), sizeof(sai_mac_t));
    rif_attrs.push_back(rif_attr);

    rif_attr.id = SAI_ROUTER_INTERFACE_ATTR_VLAN_ID;
    rif_attr.value.u16 = vlanid;
    rif_attrs.push_back(rif_attr);

    LOGG(TEST_INFO, SETL3, "sai_rif_api->create_router_interface\n");
    status = sai_rif_api->create_router_interface(&rif_id, rif_attrs.size(), rif_attrs.data());

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to create router interface. status=0x%x\n", -status);
        return false;
    }

    if (!SAI_OID_TYPE_CHECK(rif_id, SAI_OBJECT_TYPE_ROUTER_INTERFACE))
    {
        LOGG(TEST_ERR, SETL3, "router interface oid generated is not the right type\n");
        return false;
    }

    LOGG(TEST_DEBUG, SETL3, "router_interface created, rif_id 0x%lx\n", rif_id);

    // add interface ip to l3 host table
    LOGG(TEST_INFO, SETL3, "sai_route_api->create_route, SAI_ROUTE_ATTR_PACKET_ACTION, SAI_PACKET_ACTION_TRAP\n");
    sai_unicast_route_entry_t unicast_route_entry;
    unicast_route_entry.vr_id = g_vr_id;
    unicast_route_entry.destination.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    unicast_route_entry.destination.addr.ip4 = ipaddr.addr();
    unicast_route_entry.destination.mask.ip4 = 0xffffffff;
    sai_attribute_t route_attr;
    route_attr.id = SAI_ROUTE_ATTR_PACKET_ACTION;
    route_attr.value.s32 = SAI_PACKET_ACTION_TRAP;
    status = sai_route_api->create_route(&unicast_route_entry, 1, &route_attr);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to add route for l3 interface to cpu. status=0x%x\n", -status);
        return false;
    }

    // by default, drop all the traffic destined to the the ip subnet.
    // if we learn some of the neighbors, add them explicitly to the l3 host table.
    LOGG(TEST_INFO, SETL3, "sai_route_api->create_route, SAI_ROUTE_ATTR_PACKET_ACTION, SAI_PACKET_ACTION_DROP\n");
    unicast_route_entry.vr_id = g_vr_id;
    unicast_route_entry.destination.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    unicast_route_entry.destination.addr.ip4 = ipaddr.addr() & ipmask.addr();
    unicast_route_entry.destination.mask.ip4 = ipmask.addr();
    route_attr.id = SAI_ROUTE_ATTR_PACKET_ACTION;
    route_attr.value.s32 = SAI_PACKET_ACTION_DROP;
    status = sai_route_api->create_route(&unicast_route_entry, 1, &route_attr);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to add l3 intf subnet to blackhole. status=0x%x", -status);
        return false;
    }

    return true;
}


bool basic_router_setup()
{
    sai_status_t status;

    // setup saiport for each ethport
    LOGG(TEST_INFO, SETL3, "sai_switch_api->get_switch_attribute SAI_SWITCH_ATTR_PORT_NUMBER\n");

    sai_attribute_t attr;
    attr.id = SAI_SWITCH_ATTR_PORT_NUMBER;
    status = sai_switch_api->get_switch_attribute(1, &attr);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to get SAI_SWITCH_ATTR_PORT_NUMBER %d", -status);
        return false;
    }

    LOGG(TEST_DEBUG, SETL3, "SAI_SWITCH_ATTR_PORT_NUMBER %d\n", attr.value.u32);

    sai_uint32_t port_count = attr.value.u32;

    //We will cover all of the ports supoorted
    g_testcount = port_count;

    sai_object_id_t *port_list = new sai_object_id_t[port_count];

    LOGG(TEST_INFO, SETL3, "sai_switch_api->get_switch_attribute SAI_SWITCH_ATTR_PORT_LIST\n");

    attr.id = SAI_SWITCH_ATTR_PORT_LIST;
    attr.value.objlist.count = port_count;
    attr.value.objlist.list = port_list;
    status = sai_switch_api->get_switch_attribute(1, &attr);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to get SAI_SWITCH_ATTR_PORT_LIST %d", -status);
        return false;
    }

   
    unsigned int i = 0;
    sai_object_id_t vlan_member_id;
    
     while (!vlan_member_list.empty()) {
        vlan_member_id = vlan_member_list.back();
        if (!SAI_OID_TYPE_CHECK(vlan_member_id, SAI_OBJECT_TYPE_VLAN_MEMBER))
        {
            LOGG(TEST_ERR, SETL3, "vlan_member_id retrieved is not the right type%d", -status);
            return false;
        }
        
        LOGG(TEST_INFO, SETL3, "sai_vlan_api->remove_vlan_member\n");
        status = sai_vlan_api->remove_vlan_member(vlan_member_id);
        if (status != SAI_STATUS_SUCCESS )
        {
            LOGG(TEST_ERR, SETL3, "fail to remove member ports from vlan 1. status=0x%x\n",  -status);
            return false;
        }
        vlan_member_list.pop_back();
    }
    LOGG(TEST_INFO, SETL3, "sai_hif_api->set_trap_attribute SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION, TTL_ERROR\n");
    attr.id = SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION;
    attr.value.s32 = SAI_PACKET_ACTION_TRAP;
    status = sai_hif_api->set_trap_attribute(SAI_HOSTIF_TRAP_ID_TTL_ERROR, &attr);
    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to trap ttl=1 packets to cpu. status=0x%x\n", -status);
        return false;
    }
    attr.id = SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL;
    attr.value.s32 = SAI_HOSTIF_TRAP_CHANNEL_NETDEV;
    status = sai_hif_api->set_trap_attribute(SAI_HOSTIF_TRAP_ID_TTL_ERROR, &attr);
    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to set trap channel for SAI_HOSTIF_TRAP_ID_TTL_ERROR. status=0x%x\n", -status);
        return false;
    }
    LOGG(TEST_DEBUG, SETL3, "set SAI_HOSTIF_TRAP_ID_TTL_ERROR \n");


    LOGG(TEST_INFO, SETL3, "sai_hif_api->set_trap_attribute SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION, ARP_REQUEST\n");
    attr.id = SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION;
    attr.value.s32 = SAI_PACKET_ACTION_TRAP;
    status = sai_hif_api->set_trap_attribute(SAI_HOSTIF_TRAP_ID_ARP_REQUEST, &attr);
    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to trap arp request packets to cpu. status=0x%x\n", -status);
        return false;
    }
    attr.id = SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL;
    attr.value.s32 = SAI_HOSTIF_TRAP_CHANNEL_NETDEV;
    status = sai_hif_api->set_trap_attribute(SAI_HOSTIF_TRAP_ID_ARP_REQUEST, &attr);
    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to set trap channel for SAI_HOSTIF_TRAP_ID_ARP_REQUEST. status=0x%x\n", -status);
        return false;
    }
    LOGG(TEST_DEBUG, SETL3, "set SAI_HOSTIF_TRAP_ID_ARP_REQUEST \n");


    LOGG(TEST_INFO, SETL3, "sai_hif_api->set_trap_attribute SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION, ARP_RESPONSE\n");
    attr.id = SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION;
    attr.value.s32 = SAI_PACKET_ACTION_TRAP;
    status = sai_hif_api->set_trap_attribute(SAI_HOSTIF_TRAP_ID_ARP_RESPONSE, &attr);
    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to trap arp reply packets to cpu. status=0x%x\n", -status);
        return false;
    }
    attr.id = SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL;
    attr.value.s32 = SAI_HOSTIF_TRAP_CHANNEL_NETDEV;
    status = sai_hif_api->set_trap_attribute(SAI_HOSTIF_TRAP_ID_ARP_RESPONSE, &attr);
    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to set trap channel for SAI_HOSTIF_TRAP_ID_ARP_RESPONSE. status=0x%x\n", -status);
        return false;
    }
    LOGG(TEST_DEBUG, SETL3, "set SAI_HOSTIF_TRAP_ID_ARP_RESPONSE \n");


    LOGG(TEST_INFO, SETL3, "sai_hif_api->set_trap_attribute SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION, LLDP\n");
    attr.id = SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION;
    attr.value.s32 = SAI_PACKET_ACTION_TRAP;
    status = sai_hif_api->set_trap_attribute(SAI_HOSTIF_TRAP_ID_LLDP, &attr);
    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to trap lldp packets to cpu. status=0x%x\n", -status);
        return false;
    }
    attr.id = SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL;
    attr.value.s32 = SAI_HOSTIF_TRAP_CHANNEL_NETDEV;
    status = sai_hif_api->set_trap_attribute(SAI_HOSTIF_TRAP_ID_LLDP, &attr);
    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to set trap channel for SAI_HOSTIF_TRAP_ID_LLDP. status=0x%x\n", -status);
        return false;
    }
    LOGG(TEST_DEBUG, SETL3, "set SAI_HOSTIF_TRAP_ID_LLDP \n");


    LOGG(TEST_INFO, SETL3, "sai_vr_api->create_virtual_router\n");
    status = sai_vr_api->create_virtual_router(&g_vr_id, 0, NULL);

    if (status != SAI_STATUS_SUCCESS)
    {
        LOGG(TEST_ERR, SETL3, "fail to create virtual router. status=0x%x", -status);
        return false;
    }

    if (!SAI_OID_TYPE_CHECK(g_vr_id, SAI_OBJECT_TYPE_VIRTUAL_ROUTER))
    {
        LOGG(TEST_ERR, SETL3, "virtual router oid generated is not the right type\n");
        return false;
    }

    LOGG(TEST_DEBUG, SETL3, "virtual router id 0x%lx\n", g_vr_id);


    LOGG(TEST_INFO, SETL3, "for each port, sai_port_api->set_port_attribute SAI_PORT_ATTR_ADMIN_STATE true\n");
    LOGG(TEST_INFO, SETL3, "for each port, sai_port_api->set_port_attribute, SAI_PORT_ATTR_FDB_LEARNING, SAI_PORT_LEARN_MODE_HW\n");

    for (i = 0; i < port_count; i++)
    {
        attr.id = SAI_PORT_ATTR_ADMIN_STATE;
        attr.value.booldata = true;
        sai_status_t status = sai_port_api->set_port_attribute(port_list[i], &attr);

        if (status != SAI_STATUS_SUCCESS)
        {
            LOGG(TEST_ERR, SETL3, "fail to set port 0x%lx admin state to UP: %d\n", port_list[i], -status);
            return false;
        }

        attr.id = SAI_PORT_ATTR_FDB_LEARNING;
        attr.value.s32 = SAI_PORT_LEARN_MODE_HW;
        status = sai_port_api->set_port_attribute(port_list[i], &attr);

        if (status != SAI_STATUS_SUCCESS)
        {
            LOGG(TEST_ERR, SETL3, "fail to set port 0x%lx learning mode to hw: %d\n", port_list[i], -status);
            return false;
        }
    }

    //one interface for each port
    for (i = 0; i < g_testcount; i++)
    {
        g_intfAlias[i] = "et0_" + to_string(i + 1);
        g_ipAddr[i] = IpAddress("10.10." + to_string(140 + i + 1) + "." + to_string(130));
        g_ipMask[i] = IpAddress("255.255.255.252");
        g_macAddr[i] = MacAddress("00:11:11:11:11:" + to_string(i + 1));
    }

    for (i = 0; i < g_testcount; i++)
    {

        LOGG(TEST_DEBUG, SETL3, "--- interface %s %s/%s %s ---\n",
             g_intfAlias[i].c_str(),
             g_ipAddr[i].to_string().c_str(),
             g_ipMask[i].to_string().c_str(),
             g_macAddr[i].to_string().c_str()
            );
        std::vector<sai_object_id_t> port_objlist;
        long unsigned int vlanid;
        sai_attribute_t attr;
        std::vector<sai_attribute_t> attr_list;

        //assuming interface is of PANEL_INTF
        vlanid = PANEL_PORT_VLAN_START + i + 1;

        port_objlist.push_back(port_list[i]);

        if (!setup_one_l3_interface(vlanid, port_objlist.size(), port_objlist.data(),
                                    g_macAddr[i], g_ipAddr[i], g_ipMask[i], g_rif_id[i]))
        {
            LOGG(TEST_ERR, SETL3, "fail to setup l3 interface for %s\n", g_intfAlias[i].c_str());
            return false;
        }

        LOGG(TEST_DEBUG, SETL3, "setup_l3_interface for %s successfully\n",
             g_intfAlias[i].c_str());

        attr.id = SAI_HOSTIF_ATTR_TYPE;
        attr.value.s32 = SAI_HOSTIF_TYPE_NETDEV;
        attr_list.push_back(attr);

        attr.id = SAI_HOSTIF_ATTR_RIF_OR_PORT_ID;
        attr.value.oid = port_list[i];
        attr_list.push_back(attr);

        attr.id = SAI_HOSTIF_ATTR_NAME;
        strncpy((char *)&attr.value.chardata, g_intfAlias[i].c_str(), HOSTIF_NAME_SIZE);
        attr_list.push_back(attr);

        LOGG(TEST_INFO, SETL3, "sai_hif_api->create_hostif name %s\n", g_intfAlias[i].c_str());
        sai_object_id_t hif_id;
        status = sai_hif_api->create_hostif(&hif_id, attr_list.size(), attr_list.data());

        if (status != SAI_STATUS_SUCCESS)
        {
            LOGG(TEST_ERR, SETL3, "fail to create host interface name %s: %d\n",
                 g_intfAlias[i].c_str(), -status);
            return false;
        }

        if (!SAI_OID_TYPE_CHECK(hif_id, SAI_OBJECT_TYPE_HOST_INTERFACE))
        {
            LOGG(TEST_ERR, SETL3, "host interface oid generated is not the right type\n");
            return false;
        }

        LOGG(TEST_DEBUG, SETL3, "hif_id 0x%lx \n", hif_id);
    }

    LOGG(TEST_DEBUG, SETL3, "--- end of loop of interface ---\n");

    for (i = 0; i < g_testcount; i++)
    {
        g_dst_mac[i] = MacAddress("00:22:22:22:22:" + to_string(i + 1));
    }


    fdb_mgr->Show();

    for (i = 0; i < g_testcount; i++)
    {
        if (! fdb_mgr->Add(g_dst_mac[i], PANEL_PORT_VLAN_START + i + 1,
                           SAI_FDB_ENTRY_STATIC, port_list[i], SAI_PACKET_ACTION_FORWARD))
        {

            LOGG(TEST_ERR, SETL3, "fail to create sai_fdb_entry {mac %-15s vlan_id %hu}\n",
                 g_dst_mac[i].to_string().c_str(), PANEL_PORT_VLAN_START + i + 1);
            return false;
        }
    }

    fdb_mgr->Show();

    return true;
}

//TestCase defintion
class saiUnitTest : public ::testing::Test
{
public:

protected:
    static void SetUpTestCase(void)
    {
        mac = MacAddress("00:F2:F2:F2:F2:F2");

        LOGG(TEST_INFO, FRAMEWORK, "init mac %s\n", mac.to_string().c_str());

        LOGG(TEST_INFO, FRAMEWORK, "sai_api_initialize\n");
        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_api_initialize(0, (service_method_table_t *)&test_services));

        LOGG(TEST_INFO, FRAMEWORK, "sai_api_query SAI_API_SWITCH, SAI_API_PORT, ...\n");
        //query API methods of all types
        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_api_query(SAI_API_SWITCH, (void**)&sai_switch_api));
        ASSERT_TRUE(sai_switch_api != NULL);

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_api_query(SAI_API_PORT, (void**)&sai_port_api));
        ASSERT_TRUE(sai_port_api != NULL);

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_api_query(SAI_API_VLAN, (void**)&sai_vlan_api));
        ASSERT_TRUE(sai_vlan_api != NULL);

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_api_query(SAI_API_VIRTUAL_ROUTER, (void**)&sai_vr_api));
        ASSERT_TRUE(sai_vr_api != NULL);

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_api_query(SAI_API_ROUTER_INTERFACE, (void**)&sai_rif_api));
        ASSERT_TRUE(sai_rif_api != NULL);

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_api_query(SAI_API_HOST_INTERFACE, (void**)&sai_hif_api));
        ASSERT_TRUE(sai_hif_api != NULL);

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_api_query(SAI_API_NEIGHBOR, (void**)&sai_neighbor_api));
        ASSERT_TRUE(sai_neighbor_api != NULL);
        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_api_query(SAI_API_ROUTE, (void**)&sai_route_api));
        ASSERT_TRUE(sai_route_api != NULL);

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_api_query(SAI_API_NEXT_HOP, (void**)&sai_next_hop_api));
        ASSERT_TRUE(sai_next_hop_api != NULL);

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_api_query(SAI_API_NEXT_HOP_GROUP,
                                (void**)&sai_next_hop_group_api));
        ASSERT_TRUE(sai_next_hop_group_api != NULL);

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_api_query(SAI_API_FDB,
                                (void**)&sai_fdb_api));
        ASSERT_TRUE(sai_fdb_api != NULL);


        LOGG(TEST_INFO, FRAMEWORK, "sai_log_set SAI_API_SWITCH, SAI_API_PORT, ...\n");
        //set log
        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_log_set(SAI_API_SWITCH, SAI_LOG_DEBUG));

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_log_set(SAI_API_PORT, SAI_LOG_DEBUG));

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_log_set(SAI_API_VLAN, SAI_LOG_DEBUG));

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_log_set(SAI_API_VIRTUAL_ROUTER, SAI_LOG_DEBUG));

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_log_set(SAI_API_ROUTER_INTERFACE, SAI_LOG_DEBUG));


        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_log_set(SAI_API_HOST_INTERFACE, SAI_LOG_DEBUG));

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_log_set(SAI_API_NEIGHBOR, SAI_LOG_DEBUG));

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_log_set(SAI_API_ROUTE, SAI_LOG_DEBUG));

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_log_set(SAI_API_NEXT_HOP, SAI_LOG_DEBUG));

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_log_set(SAI_API_NEXT_HOP_GROUP, SAI_LOG_DEBUG));


        LOGG(TEST_INFO, FRAMEWORK, "sai_switch_api->initialize_switch \n");
        ASSERT_TRUE(sai_switch_api->initialize_switch);
        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_switch_api->initialize_switch(0, "0xb850", "",
                          &plat_switch_notification_handlers));


        sai_attribute_t     attr;

        attr.id = SAI_SWITCH_ATTR_SRC_MAC_ADDRESS;
        memcpy(attr.value.mac, mac.to_bytes(), 6);
        ASSERT_TRUE(sai_switch_api->set_switch_attribute != NULL);

        LOGG(TEST_INFO, FRAMEWORK, "sai_switch_api->set_switch_attribute SAI_SWITCH_ATTR_SRC_MAC_ADDRESS %s\n",
             mac.to_string().c_str());
        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_switch_api->set_switch_attribute(&attr));

        LOGG(TEST_INFO, FRAMEWORK, "Create neighbor_mgr, nexthopgrp_mgr and route_mgr\n");

        nexthop_mgr = new NextHopMgr();
        neighbor_mgr = new NeighborMgr(nexthop_mgr);
        nexthopgrp_mgr = new NextHopGrpMgr(neighbor_mgr);
        route_mgr = new RouteMgr(neighbor_mgr, nexthopgrp_mgr);
        fdb_mgr = new FdbMgr();


        LOGG(TEST_INFO, FRAMEWORK, "Set Up L3 Interfaces\n");
        ASSERT_EQ(true, basic_router_setup());
    }
};


static void neighbor_adding()
{

    neighbor_mgr->Show();

    IpAddress ipAddr;
    MacAddress macAddr;
    string intfAlias;
    sai_object_id_t rif_id;

    LOGG(TEST_INFO, TESTCASE, "--- add neighbor entry 192.168.1.1---\n");
    ipAddr = IpAddress("192.168.1.1");

    macAddr = g_dst_mac[0];
    intfAlias = g_intfAlias[0];
    rif_id = g_rif_id[0];

    ASSERT_TRUE(neighbor_mgr->Add(ipAddr, macAddr, intfAlias, rif_id));
    neighbor_mgr->Show();

    LOGG(TEST_INFO, TESTCASE, "--- remove neighbor entry 192.168.1.1---\n");
    ASSERT_TRUE(neighbor_mgr->Del(ipAddr));
    neighbor_mgr->Show();

    LOGG(TEST_INFO, TESTCASE, "--- add back neighbor entry 192.168.1.1---\n");
    ASSERT_TRUE(neighbor_mgr->Add(ipAddr, macAddr, intfAlias, rif_id));

    LOGG(TEST_INFO, TESTCASE, "--- add neighbor entry 172.16.20.22 ---\n");
    ipAddr = IpAddress("172.16.20.22");
    macAddr = g_dst_mac[1];
    intfAlias = g_intfAlias[1];
    rif_id = g_rif_id[1];
    ASSERT_TRUE(neighbor_mgr->Add(ipAddr, macAddr, intfAlias, rif_id));

    LOGG(TEST_INFO, TESTCASE, "--- add neighbor entry 192.168.2.1 ---\n");
    ipAddr = IpAddress("192.168.2.1");
    macAddr = g_dst_mac[2];
    intfAlias = g_intfAlias[2];
    rif_id = g_rif_id[2];
    ASSERT_TRUE(neighbor_mgr->Add(ipAddr, macAddr, intfAlias, rif_id));

    LOGG(TEST_INFO, TESTCASE, "--- add neighbor entry 192.168.3.1 ---\n");
    ipAddr = IpAddress("192.169.3.1");
    macAddr = g_dst_mac[3];
    intfAlias = g_intfAlias[3];
    rif_id = g_rif_id[3];
    ASSERT_TRUE(neighbor_mgr->Add(ipAddr, macAddr, intfAlias, rif_id));

    LOGG(TEST_INFO, TESTCASE, "--- add neighbor entry 24.58.202.118 ---\n");
    ipAddr = IpAddress("24.58.202.118");
    macAddr = g_dst_mac[0];
    intfAlias = g_intfAlias[0];
    rif_id = g_rif_id[0];
    ASSERT_TRUE(neighbor_mgr->Add(ipAddr, macAddr, intfAlias, rif_id));

    neighbor_mgr->Show();
}

static void neighbor_removing()
{

    neighbor_mgr->Show();
    LOGG(TEST_INFO, TESTCASE, "--- removing all neighbor entries ---\n");

    IpAddress ipAddr;

    ipAddr = IpAddress("192.168.1.1");
    ASSERT_TRUE(neighbor_mgr->Del(ipAddr));

    ipAddr = IpAddress("172.16.20.22");
    ASSERT_TRUE(neighbor_mgr->Del(ipAddr));

    ipAddr = IpAddress("192.168.2.1");
    ASSERT_TRUE(neighbor_mgr->Del(ipAddr));

    ipAddr = IpAddress("192.169.3.1");
    ASSERT_TRUE(neighbor_mgr->Del(ipAddr));

    ipAddr = IpAddress("24.58.202.118");
    ASSERT_TRUE(neighbor_mgr->Del(ipAddr));

    neighbor_mgr->Show();
}

TEST_F(saiUnitTest, neighbor_unittest)
{
    neighbor_adding();
    neighbor_removing();

    neighbor_adding();
    ASSERT_TRUE(neighbor_mgr->EraseAll());
    neighbor_mgr->Show();
}

static void nexthopgrp_test()
{
    neighbor_adding();

    IpAddresses nexthops;

    neighbor_mgr->Show();
    nexthopgrp_mgr->Show();

    nexthops.add("192.168.2.1");
    nexthops.add("192.169.3.1");

    LOGG(TEST_INFO, TESTCASE, "--- add nexthopgroup 192.168.2.1,192.169.3.1 ---\n");
    ASSERT_TRUE(nexthopgrp_mgr->Add(nexthops));
    nexthopgrp_mgr->Show();

    LOGG(TEST_INFO, TESTCASE, "--- remove nexthopgroup 192.168.2.1,192.169.3.1 ---\n");
    ASSERT_TRUE(nexthopgrp_mgr->Del(nexthops));
    ASSERT_TRUE(neighbor_mgr->EraseAll());
    nexthopgrp_mgr->Show();
}


TEST_F(saiUnitTest, nexthopgrp_test)
{
    nexthopgrp_test();
}


static void route_adding()
{
    neighbor_adding();

    route_mgr->Show();
    route_mgr->ShowECMP();

    IpPrefix prefix;
    IpAddresses nexthops;

    LOGG(TEST_INFO, TESTCASE, "--- add 1st route---\n");
    prefix = IpPrefix("192.168.1.0/24");
    nexthops.add("192.168.1.1");
    ASSERT_TRUE(route_mgr->Add(prefix, nexthops));

    LOGG(TEST_INFO, TESTCASE, "--- add 2nd route---\n");
    prefix = IpPrefix("172.16.2.0/24");
    nexthops = IpAddresses("172.16.20.22");
    ASSERT_TRUE(route_mgr->Add(prefix, nexthops));

    LOGG(TEST_INFO, TESTCASE, "--- add 3rd route, \"0.0.0.0\" as nexthop---\n");
    prefix = IpPrefix("182.30.31.199/24");
    nexthops = IpAddresses("0.0.0.0");
    ASSERT_TRUE(route_mgr->Add(prefix, nexthops));


    LOGG(TEST_INFO, TESTCASE, "--- add 4th route---\n");
    prefix = IpPrefix("192.168.2.0/24");
    nexthops = IpAddresses("192.168.2.1");
    ASSERT_TRUE(route_mgr->Add(prefix, nexthops));

    LOGG(TEST_INFO, TESTCASE, "--- add 5th route, ECMP group---\n");
    prefix = IpPrefix("192.168.0.0/16");
    nexthops.add("192.168.1.1");
    ASSERT_TRUE(route_mgr->Add(prefix, nexthops));

    LOGG(TEST_INFO, TESTCASE, "--- add 6th route, ECMP group---\n");
    prefix = IpPrefix("192.0.0.0/8");
    nexthops.add("192.169.3.1");
    ASSERT_TRUE(route_mgr->Add(prefix, nexthops));

    route_mgr->Show();
    route_mgr->ShowECMP();
}


static void route_removing()
{
    IpPrefix prefix;
    IpAddresses nexthops;
    IpAddress ipAddr;

    LOGG(TEST_INFO, TESTCASE, "--- remove route 192.168.1.0/24 ---\n");
    prefix = IpPrefix("192.168.1.0/24");
    ASSERT_TRUE(route_mgr->Del(prefix));

    LOGG(TEST_INFO, TESTCASE, "--- remove route 192.168.0.0/16 with ECMP group ---\n");
    prefix = IpPrefix("192.168.0.0/16");
    ASSERT_TRUE(route_mgr->Del(prefix));

    LOGG(TEST_INFO, TESTCASE, "--- remove route 182.30.31.199/24 with nexthop as \"0.0.0.0\" ---\n");
    prefix = IpPrefix("182.30.31.199/24");
    ASSERT_TRUE(route_mgr->Del(prefix));

    route_mgr->Show();
    route_mgr->ShowECMP();

    LOGG(TEST_INFO, TESTCASE, "--- remove the neighbor 192.168.1.1 when ECMP route 192.0.0.0/8 is still using it---\n");
    neighbor_mgr->Show();

    ipAddr = IpAddress("192.168.1.1");
    ASSERT_TRUE(!neighbor_mgr->Del(ipAddr));

    LOGG(TEST_INFO, TESTCASE, "*** expected failure to remove the neighbor 192.168.1.1 ***\n");

    LOGG(TEST_INFO, TESTCASE, "--- remove route 192.0.0.0/8 with ECMP group, the last route use neighbor 192.168.1.1 ---\n");
    prefix = IpPrefix("192.0.0.0/8");
    ASSERT_TRUE(route_mgr->Del(prefix));

    LOGG(TEST_INFO, TESTCASE, "--- remove the neighbor 192.168.1.1 in clean state---\n");
    ASSERT_TRUE(neighbor_mgr->Del(ipAddr));

    neighbor_mgr->Show();
}


TEST_F(saiUnitTest, route_unittest)
{
    route_adding();
    route_removing();

    ASSERT_TRUE(route_mgr->EraseAll());
    ASSERT_TRUE(neighbor_mgr->EraseAll());
    route_mgr->Show();
    neighbor_mgr->Show();
}

static void tearup_tests(void)
{

    int i = 1;
    fdb_mgr->Show();
    fdb_mgr->Del(g_dst_mac[i], PANEL_PORT_VLAN_START + i + 1);
    fdb_mgr->Show();

    fdb_mgr->EraseAll();
    fdb_mgr->Show();

    LOGG(TEST_INFO, FRAMEWORK, "sai_switch_api->shutdown_switch\n");
    ASSERT_TRUE(sai_switch_api->shutdown_switch);
    sai_switch_api->shutdown_switch(false);
}


int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);

    if (argc == 1 )
    {
        curr_log_level = TEST_INFO;
    }
    else if (argc == 2 )
    {
        if ((std::string(argv[1]) == "?") ||
                (std::string(argv[1]) == "-help") ||
                (std::string(argv[1]) == "-h") )
        {
            printf("Usage: %s -d <debug|info|notice|err>\n default debug level is <info>\n", argv[0]);
        }

        return 0;

    }
    else if (argc == 3)
    {
        if (std::string(argv[1]) == "-d")
        {
            if (std::string(argv[2]) == "debug")
            {
                curr_log_level = TEST_DEBUG;
            }
            else if (std::string(argv[2]) == "info")
            {
                curr_log_level = TEST_INFO;
            }
            else if (std::string(argv[2]) == "notice")
            {
                curr_log_level = TEST_NOTICE;
            }
            else if (std::string(argv[2]) == "err")
            {
                curr_log_level = TEST_ERR;
            }
            else
            {
                curr_log_level = TEST_INFO;
            }
        }

        printf("curr_log_level %d \n", curr_log_level);
    }
    else
    {
        printf("Usage: %s -d <debug|info|notice|err>\n", argv[0]);
        return 0;
    }

    int rt;
    rt = RUN_ALL_TESTS();
    tearup_tests();
    return rt;
}


