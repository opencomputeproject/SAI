/************************************************************************
* Copyright (c) 2015 Dell Inc. 
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
*
* Module Name:
*
*    sai_l3_route_unit_test.cpp
*     
* Abstract:
*
*    SAI ROUTE UNIT TEST :- Covers the test cases for validating all public
*    APIs in SAI ROUTE module.
*
*************************************************************************/

#include "gtest/gtest.h"

#include "sai_l3_unit_test_utils.h"

extern "C" {
#include "saistatus.h"
#include "saitypes.h"
#include "sairoute.h"
#include "saivlan.h"
#include "sailag.h"
#include "sai.h"
#include <stdio.h>
}

class saiL3RouteTest : public saiL3Test {
    public:
        static void SetUpTestCase (void);
        static void TearDownTestCase (void);

        static void sai_test_route_attr_verify (
                    sai_object_id_t vr_id, sai_ip_addr_family_t family,
                    const char *prefix_str, unsigned int prefix_len,
                    unsigned int nh_type, sai_object_id_t fwd_obj_id,
                    sai_packet_action_t pkt_action, unsigned int trap_prio);
        static void sai_test_route_remove_and_verify (
                    sai_object_id_t vr_id, sai_ip_addr_family_t family,
                    const char *prefix_str, unsigned int prefix_len);

        static const unsigned int test_port_1 = 0;
        static const unsigned int test_port_2 = 1;
        static const unsigned int test_ecmp_count_1 = 2;
        static const unsigned int test_ecmp_count_2 = 4;

        static const unsigned int SAI_TEST_ROUTE_DFLT_TRAP_PRIO  = 0;
        static const unsigned int SAI_TEST_ROUTE_NH_TYPE_NONE    = 0xFF;
        static const sai_packet_action_t SAI_TEST_ROUTE_DFLT_PKT_ACTION = \
                                                 SAI_PACKET_ACTION_FORWARD;

        static const char *ip_str_1;
        static const char *ip_str_2;
        static const char *ip6_str_1;

        static sai_object_id_t   vr_id;
        static sai_object_id_t   test_port_id_1, test_port_id_2;
        static sai_object_id_t   rif_id_1, rif_id_2;
        static sai_object_id_t   nh_id_1, nh_id_2;
        static sai_object_id_t   nh6_id_1;
        static sai_object_id_t   nh_grp_id_1, nh_grp_id_2;
        static sai_vlan_api_t*   sai_vlan_api_table;
};

const char * saiL3RouteTest ::ip_str_1 = "50.1.1.1";
const char * saiL3RouteTest ::ip_str_2 = "60.1.1.1";
const char * saiL3RouteTest ::ip6_str_1 = "1:2:3::1";

sai_object_id_t saiL3RouteTest ::vr_id = 0;
sai_object_id_t saiL3RouteTest ::rif_id_1 = 0;
sai_object_id_t saiL3RouteTest ::rif_id_2 = 0;
sai_object_id_t saiL3RouteTest ::nh_id_1 = 0;
sai_object_id_t saiL3RouteTest ::nh_id_2 = 0;
sai_object_id_t saiL3RouteTest ::nh6_id_1 = 0;
sai_object_id_t saiL3RouteTest ::nh_grp_id_1 = 0;
sai_object_id_t saiL3RouteTest ::nh_grp_id_2 = 0;
sai_object_id_t saiL3RouteTest ::test_port_id_1 = 0;
sai_object_id_t saiL3RouteTest ::test_port_id_2 = 0;
sai_vlan_api_t* saiL3RouteTest ::sai_vlan_api_table = NULL;

void saiL3RouteTest ::SetUpTestCase (void)
{
    sai_status_t      sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t   nh_list [4];
    unsigned int      id = 0;

    /* Base SetUpTestCase for SAI initialization */
    saiL3Test ::SetUpTestCase ();

    sai_rc = sai_test_router_mac_init (router_mac);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* VRF Creation */
    sai_rc = sai_test_vrf_create (&vr_id, 0);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    test_port_id_1 = sai_l3_port_id_get (test_port_1);
    test_port_id_2 = sai_l3_port_id_get (test_port_2);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_VLAN, (static_cast<void**>
                              (static_cast<void*> (&sai_vlan_api_table)))));

    /* RIF creation */
    sai_rc = sai_test_rif_create (&rif_id_1, default_rif_attr_count,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_rif_create (&rif_id_2, default_rif_attr_count,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id_2);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Neighbor creation */
    sai_rc = sai_test_neighbor_create (rif_id_1, SAI_IP_ADDR_FAMILY_IPV4,
                                       ip_str_1, default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       "00:22:22:22:22:22");

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_neighbor_create (rif_id_2, SAI_IP_ADDR_FAMILY_IPV4,
                                       ip_str_2, default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       "00:44:44:44:44:44");

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_neighbor_create (rif_id_1, SAI_IP_ADDR_FAMILY_IPV6,
                                       ip6_str_1, default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       "00:66:66:66:66:66");

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Next-hop Creation */
    sai_rc = sai_test_nexthop_create (&nh_id_1, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE, SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      SAI_IP_ADDR_FAMILY_IPV4, ip_str_1,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      rif_id_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_nexthop_create (&nh_id_2, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE, SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      SAI_IP_ADDR_FAMILY_IPV4, ip_str_2,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      rif_id_2);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_nexthop_create (&nh6_id_1, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE, SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      SAI_IP_ADDR_FAMILY_IPV6, ip6_str_1,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      rif_id_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Next-hop Group Creation */
    for (id = 0; id < test_ecmp_count_1; id++) {
        nh_list [id] = nh_id_1;
    }

    sai_rc = sai_test_nh_group_create (&nh_grp_id_1, nh_list,
                                       default_nh_group_attr_count,
                                       SAI_NEXT_HOP_GROUP_ATTR_TYPE,
                                       SAI_NEXT_HOP_GROUP_ECMP,
                                       SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST,
                                       test_ecmp_count_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    for (id = 0; id < test_ecmp_count_2; id++) {
        nh_list [id] = nh_id_2;
    }

    sai_rc = sai_test_nh_group_create (&nh_grp_id_2, nh_list,
                                       default_nh_group_attr_count,
                                       SAI_NEXT_HOP_GROUP_ATTR_TYPE,
                                       SAI_NEXT_HOP_GROUP_ECMP,
                                       SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST,
                                       test_ecmp_count_2);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

void saiL3RouteTest ::TearDownTestCase (void)
{
    sai_status_t      sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t   nh_list [4];
    unsigned int      id = 0;

    /* Remove all Neighbors */
    sai_rc = sai_test_neighbor_remove (rif_id_1, SAI_IP_ADDR_FAMILY_IPV4,
                                       ip_str_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_neighbor_remove (rif_id_2, SAI_IP_ADDR_FAMILY_IPV4,
                                       ip_str_2);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_neighbor_remove (rif_id_1, SAI_IP_ADDR_FAMILY_IPV6,
                                       ip6_str_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove Next-hops from Next-hop Groups */
    for (id = 0; id < test_ecmp_count_1; id++) {
        nh_list [id] = nh_id_1;
    }

    sai_rc =
        sai_test_remove_nh_from_group (nh_grp_id_1, test_ecmp_count_1, nh_list);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    for (id = 0; id < test_ecmp_count_2; id++) {
        nh_list [id] = nh_id_2;
    }

    sai_rc =
        sai_test_remove_nh_from_group (nh_grp_id_2, test_ecmp_count_2, nh_list);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove all Next-hops */
    sai_rc = sai_test_nexthop_remove (nh_id_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_nexthop_remove (nh_id_2);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_nexthop_remove (nh6_id_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove all Next-hop Groups */
    sai_rc = sai_test_nh_group_remove (nh_grp_id_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_nh_group_remove (nh_grp_id_2);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove all RIF */
    sai_rc = sai_test_rif_remove (rif_id_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_rif_remove (rif_id_2);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove VRF */
    sai_rc = sai_test_vrf_remove (vr_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Helper function to verify the Route Attributes.
 */
void saiL3RouteTest ::sai_test_route_attr_verify (
sai_object_id_t vr_id, sai_ip_addr_family_t family,
const char *prefix_str, unsigned int prefix_len, unsigned int nh_type,
sai_object_id_t fwd_obj_id, sai_packet_action_t pkt_action, 
unsigned int trap_prio)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    unsigned int    fwd_obj_attr_id;
    sai_attribute_t attr_list [3];
    unsigned int    attr_count = 3;
    bool            get_fwd_obj_attr = true;

    if (nh_type == SAI_ROUTE_ATTR_NEXT_HOP_ID) {
       fwd_obj_attr_id = SAI_ROUTE_ATTR_NEXT_HOP_ID;
    } else {
        attr_count--;
        get_fwd_obj_attr = false;
    }

    if (get_fwd_obj_attr) {
        sai_rc = sai_test_route_attr_get (vr_id, family, prefix_str, prefix_len,
                                          &attr_list [0], attr_count,
                                          SAI_ROUTE_ATTR_PACKET_ACTION,
                                          SAI_ROUTE_ATTR_TRAP_PRIORITY,
                                          fwd_obj_attr_id);
    } else {
        sai_rc = sai_test_route_attr_get (vr_id, family, prefix_str, prefix_len,
                                          &attr_list [0], attr_count,
                                          SAI_ROUTE_ATTR_PACKET_ACTION,
                                          SAI_ROUTE_ATTR_TRAP_PRIORITY);
    }

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    ASSERT_EQ (pkt_action, attr_list [0].value.s32);
    ASSERT_EQ (trap_prio, attr_list [1].value.u8);

    if (get_fwd_obj_attr) {
        ASSERT_EQ (fwd_obj_id, attr_list [2].value.oid);
    }
}

/*
 * Helper function to remove the route.
 */
void saiL3RouteTest ::sai_test_route_remove_and_verify (
sai_object_id_t vr_id, sai_ip_addr_family_t family,
const char *prefix_str, unsigned int prefix_len)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;

    sai_rc = sai_test_route_remove (vr_id, family, prefix_str, prefix_len);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Creates Route pointing to a next-hop and checks if duplicate create API call
 * returns appropriate status.
 * Updates the Route to point to another next-hop, ECMP/next-hop group.
 * Removes Route and checks if duplicate remove API call returns appropriate
 * status.
 */
TEST_F (saiL3RouteTest, route_fwd_with_nh_id_create_update_and_remove)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    const char           *prefix_str = "10.1.2.0";
    unsigned int          prefix_len = 24;
    sai_ip_addr_family_t  family = SAI_IP_ADDR_FAMILY_IPV4;

    /* Create Route to FWD packets on Next-hop nh_id_1 */
    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len,
                                    1, SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1,
                                SAI_TEST_ROUTE_DFLT_PKT_ACTION,
                                SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

    /* Duplicate Create will return SAI_STATUS_ITEM_ALREADY_EXISTS status */
    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len,
                                    1, SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_ITEM_ALREADY_EXISTS, sai_rc);

    /* Route is set to FWD on another Next-hop nh_id_2 */
    sai_rc =
        sai_test_route_attr_set (vr_id, family, prefix_str, prefix_len,
                                 SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_2);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_2,
                                SAI_TEST_ROUTE_DFLT_PKT_ACTION,
                                SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

    /* Route is set to FWD on ECMP group nh_grp_id_1 */
    sai_rc = sai_test_route_attr_set (vr_id, family, prefix_str, prefix_len,
                                      SAI_ROUTE_ATTR_NEXT_HOP_ID,
                                      nh_grp_id_1);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_grp_id_1,
                                SAI_TEST_ROUTE_DFLT_PKT_ACTION,
                                SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

    /* Route is set back to FWD on Next_hop nh_id_1 again */
    sai_rc = sai_test_route_attr_set (vr_id, family, prefix_str, prefix_len,
                                      SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1,
                                SAI_TEST_ROUTE_DFLT_PKT_ACTION,
                                SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

    /* Remove Route */
    sai_test_route_remove_and_verify (vr_id, family, prefix_str, prefix_len);

    /* Duplicate Remove will return SAI_STATUS_ITEM_NOT_FOUND status */
    sai_rc = sai_test_route_remove (vr_id, family, prefix_str, prefix_len);

    EXPECT_EQ (SAI_STATUS_ITEM_NOT_FOUND, sai_rc);
}

/*
 * Creates Route pointing to a next-hop group/ecmp and checks if duplicate 
 * create API call returns appropriate status.
 * Updates the Route to point to another ECMP/next-hop group, next-hop.
 * Removes Route and checks if duplicate remove API call returns appropriate
 * status.
 */
TEST_F (saiL3RouteTest, route_fwd_with_nh_grp_id_create_update_and_remove)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    const char           *prefix_str = "20.1.0.0";
    unsigned int          prefix_len = 16;
    sai_ip_addr_family_t  family = SAI_IP_ADDR_FAMILY_IPV4;

    /* Create Route to FWD packets on ECMP group nh_grp_id_1 */
    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len,
                                    1, SAI_ROUTE_ATTR_NEXT_HOP_ID,
                                    nh_grp_id_1);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_grp_id_1,
                                SAI_TEST_ROUTE_DFLT_PKT_ACTION,
                                SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

    /* Route is set to FWD on another ECMP group nh_grp_id_2 */
    sai_rc =
        sai_test_route_attr_set (vr_id, family, prefix_str, prefix_len,
                                 SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_grp_id_2);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_grp_id_2,
                                SAI_TEST_ROUTE_DFLT_PKT_ACTION,
                                SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

    /* Route is set to FWD on Next-hop ID nh_id_1 */
    sai_rc = sai_test_route_attr_set (vr_id, family, prefix_str, prefix_len,
                                      SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1,
                                SAI_TEST_ROUTE_DFLT_PKT_ACTION,
                                SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

    /* Route is set back to FWD on ECMP group nh_grp_id_1 again */
    sai_rc =
        sai_test_route_attr_set (vr_id, family, prefix_str, prefix_len,
                                 SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_grp_id_1);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_grp_id_1,
                                SAI_TEST_ROUTE_DFLT_PKT_ACTION,
                                SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

    /* Remove Route */
    sai_test_route_remove_and_verify (vr_id, family, prefix_str, prefix_len);
}

/*
 * Validates Route create, remove APIs for IPv6.
 */
TEST_F (saiL3RouteTest, route_ipv6_fwd_with_nh_id_create_remove)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    const char           *prefix_str = "1234:2222:3330::0";
    unsigned int          prefix_len = 48;
    sai_ip_addr_family_t  family = SAI_IP_ADDR_FAMILY_IPV6;

    /* Create Route to FWD packets on Next-hop nh6_id_1 */
    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len,
                                    1, SAI_ROUTE_ATTR_NEXT_HOP_ID, nh6_id_1);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh6_id_1,
                                SAI_TEST_ROUTE_DFLT_PKT_ACTION,
                                SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

    /* Duplicate Create will return SAI_STATUS_ITEM_ALREADY_EXISTS status */
    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len,
                                    1, SAI_ROUTE_ATTR_NEXT_HOP_ID, nh6_id_1);

    EXPECT_EQ (SAI_STATUS_ITEM_ALREADY_EXISTS, sai_rc);

    /* Remove Route */
    sai_test_route_remove_and_verify (vr_id, family, prefix_str, prefix_len);

    /* Duplicate Remove will return SAI_STATUS_ITEM_NOT_FOUND status */
    sai_rc = sai_test_route_remove (vr_id, family, prefix_str, prefix_len);

    EXPECT_EQ (SAI_STATUS_ITEM_NOT_FOUND, sai_rc);
}

/*
 * Validates the packet action attribute setting on a Route with FORWARD, TRAP,
 * DROP and LOG packet actions.
 */
TEST_F (saiL3RouteTest, route_set_with_pkt_action_attr)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    const char           *prefix_str = "30.0.0.0";
    unsigned int          prefix_len = 8;
    sai_ip_addr_family_t  family = SAI_IP_ADDR_FAMILY_IPV4;
    unsigned int          num_trials = 0;
    unsigned int          trial_id = 0;
    sai_packet_action_t   expected_pkt_action = SAI_TEST_ROUTE_DFLT_PKT_ACTION;
    sai_packet_action_t   packet_action [] =
    {SAI_PACKET_ACTION_FORWARD, SAI_PACKET_ACTION_DROP,
        SAI_PACKET_ACTION_FORWARD, SAI_PACKET_ACTION_TRAP,
        SAI_PACKET_ACTION_FORWARD, SAI_PACKET_ACTION_LOG,
        SAI_PACKET_ACTION_FORWARD};

    /* Create Route to FWD packets on Next-hop nh_id_1 */
    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len,
                                    1, SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1,
                                SAI_TEST_ROUTE_DFLT_PKT_ACTION,
                                SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

    /* Try setting Different packet actions attribute and verify */
    num_trials = sizeof (packet_action)/ sizeof (sai_packet_action_t);

    for (trial_id = 0; trial_id < num_trials; trial_id++) {
        printf ("Setting packet action attribute to %d\r\n",
                packet_action [trial_id]);

        sai_rc =
            sai_test_route_attr_set (vr_id, family, prefix_str, prefix_len,
                                     SAI_ROUTE_ATTR_PACKET_ACTION,
                                     packet_action [trial_id]);

        if (sai_rc == SAI_STATUS_SUCCESS) {

            expected_pkt_action = packet_action [trial_id];
        }

        sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                    SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1,
                                    expected_pkt_action,
                                    SAI_TEST_ROUTE_DFLT_TRAP_PRIO);
    }

    /* Remove Route */
    sai_test_route_remove_and_verify (vr_id, family, prefix_str, prefix_len);
}

/*
 * Validates Route attribute set API with trap priority.
 */
TEST_F (saiL3RouteTest, route_set_with_trap_priority_attr)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    const char           *prefix_str = "40.0.0.0";
    unsigned int          prefix_len = 8;
    sai_ip_addr_family_t  family = SAI_IP_ADDR_FAMILY_IPV4;
    sai_packet_action_t   packet_action = SAI_PACKET_ACTION_TRAP;
    uint8_t               trap_prio = 7;

    /* Create Route to TRAP packets with TRAP PRIORITY attribute set */
    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len, 2,
                                    SAI_ROUTE_ATTR_PACKET_ACTION, packet_action,
                                    SAI_ROUTE_ATTR_TRAP_PRIORITY, trap_prio);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_TEST_ROUTE_NH_TYPE_NONE, 0,
                                packet_action, trap_prio);

    /* Set Route with a different TRAP PRIORITY */
    trap_prio = 5;

    sai_rc = sai_test_route_attr_set (vr_id, family, prefix_str, prefix_len,
                                      SAI_ROUTE_ATTR_TRAP_PRIORITY, trap_prio);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_TEST_ROUTE_NH_TYPE_NONE, 0,
                                packet_action, trap_prio);

    /* Set Route with Next-hop id. */
    sai_rc = sai_test_route_attr_set (vr_id, family, prefix_str, prefix_len,
                                      SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1,
                                packet_action, trap_prio);

    /* Set Route to FWD packet action and verify that TRAP_PRIORIY is cached. */
    packet_action = SAI_PACKET_ACTION_FORWARD;

    sai_rc = sai_test_route_attr_set (vr_id, family, prefix_str, prefix_len,
                                      SAI_ROUTE_ATTR_PACKET_ACTION,
                                      packet_action);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1,
                                packet_action, trap_prio);

    /* Remove Route */
    sai_test_route_remove_and_verify (vr_id, family, prefix_str, prefix_len);
}

/*
 * Create IPv4 and IPv6 catch-all entries with TRAP action, update with
 * DROP action and finally remove them.
 */
TEST_F (saiL3RouteTest, route_catch_all_create_remove)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_packet_action_t   packet_action = SAI_PACKET_ACTION_TRAP;
    unsigned int          idx = 0;
    unsigned int          prefix_len = 0;
    const char           *prefix_str [2] = {"0.0.0.0", "0::0"};
    sai_ip_addr_family_t  family [2] = {
        SAI_IP_ADDR_FAMILY_IPV4, SAI_IP_ADDR_FAMILY_IPV6};

    /* Create catch-all Routes for IPv4 and IPv6 with TRAP packet action. */
    for (idx = 0; idx < 2; idx ++) {
        sai_rc = sai_test_route_create (vr_id, family [idx], prefix_str [idx],
                                        prefix_len, 1,
                                        SAI_ROUTE_ATTR_PACKET_ACTION,
                                        packet_action);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_test_route_attr_verify (vr_id, family [idx], prefix_str [idx],
                                    prefix_len, SAI_TEST_ROUTE_NH_TYPE_NONE, 0,
                                    packet_action,
                                    SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

        /* Duplicate Create will return SAI_STATUS_ITEM_ALREADY_EXISTS status */
        sai_rc = sai_test_route_create (vr_id, family [idx], prefix_str [idx],
                                        prefix_len, 1,
                                        SAI_ROUTE_ATTR_PACKET_ACTION,
                                        packet_action);

        EXPECT_EQ (SAI_STATUS_ITEM_ALREADY_EXISTS, sai_rc);
    }

    for (idx = 0; idx < 2; idx ++) {
        /* Set packet action to DROP */
        packet_action = SAI_PACKET_ACTION_DROP;

        sai_rc = sai_test_route_attr_set (vr_id, family [idx], prefix_str [idx],
                                          prefix_len,
                                          SAI_ROUTE_ATTR_PACKET_ACTION,
                                          packet_action);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_test_route_attr_verify (vr_id, family [idx], prefix_str [idx],
                                    prefix_len, SAI_TEST_ROUTE_NH_TYPE_NONE, 0,
                                    packet_action,
                                    SAI_TEST_ROUTE_DFLT_TRAP_PRIO);


        /* Remove catch-all Routes for IPv4 and IPv6 */
        sai_test_route_remove_and_verify (vr_id, family [idx], prefix_str [idx],
                                          prefix_len);
    }
}

/*
 * Check if the route creation with one of the mandatory attributes missing is 
 * returning appropriate error status.
 */
TEST_F (saiL3RouteTest, route_create_with_mandatory_attr_missing)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    const char           *prefix_str = "50.0.0.0";
    unsigned int          prefix_len = 8;
    sai_ip_addr_family_t  family = SAI_IP_ADDR_FAMILY_IPV4;
    uint8_t               trap_prio = 7;

    /*
     * Create Route with FWD packet action, but without any FWD object -
     * NEXT_HOP or NEXT_HOP_GROUP attribute.
     */
    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len, 1,
                                    SAI_ROUTE_ATTR_PACKET_ACTION,
                                    SAI_PACKET_ACTION_FORWARD);

    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_rc);

    /*
     * Create Route with LOG packet action, but without any FWD object -
     * NEXT_HOP or NEXT_HOP_GROUP attribute.
     */
    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len, 1,
                                    SAI_ROUTE_ATTR_PACKET_ACTION,
                                    SAI_PACKET_ACTION_LOG);

    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_rc);

    /*
     * Create Route with TRAP_PRIORITY attribute, but without any FWD object -
     * NEXT_HOP or NEXT_HOP_GROUP attribute.
     */
    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len, 1,
                                    SAI_ROUTE_ATTR_TRAP_PRIORITY, trap_prio);

    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_rc);
}

/*
 * Check if the Route attribute set on a non existing route is returning
 * appropriate error status.
 */
TEST_F (saiL3RouteTest, route_attr_set_on_non_existing_route)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    const char           *prefix_str = "200.100.100.0";
    unsigned int          prefix_len = 24;
    sai_ip_addr_family_t  family = SAI_IP_ADDR_FAMILY_IPV4;

    /* Next-hop attribute is set on a non-existing route */
    sai_rc =
        sai_test_route_attr_set (vr_id, family, prefix_str, prefix_len,
                                 SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_ITEM_NOT_FOUND, sai_rc);
}

/*
 * Check if the Route create, set and remove APIs called with non 
 * existing VRF returns appropriate error status.
 */
TEST_F (saiL3RouteTest, route_apis_invoke_with_non_existing_vr_id)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  non_existing_vrf = 100;
    const char              *prefix_str = "200.100.100.0";
    unsigned int             prefix_len = 24;
    sai_ip_addr_family_t     family = SAI_IP_ADDR_FAMILY_IPV4;

    /* Create Route with a non existing VRF */
    sai_rc = sai_test_route_create (non_existing_vrf,
                                    family, prefix_str, prefix_len, 1,
                                    SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Set Route Next-hop attribute with a non existing VRF */
    sai_rc = sai_test_route_attr_set (non_existing_vrf,
                                      family, prefix_str, prefix_len,
                                      SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Remove Route with a non existing VRF */
    sai_rc = sai_test_route_remove (non_existing_vrf,
                                    family, prefix_str, prefix_len);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
}

/*
 * Check if the Route create, set and remove APIs called with invalid VRF 
 * returns appropriate error status.
 */
TEST_F (saiL3RouteTest, route_apis_invoke_with_invalid_vr_id)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          invalid_vrf = 0;
    const char              *prefix_str = "200.100.100.0";
    unsigned int             prefix_len = 24;
    sai_ip_addr_family_t     family = SAI_IP_ADDR_FAMILY_IPV4;

    /* Create Route with a non existing VRF */
    sai_rc = sai_test_route_create (invalid_vrf,
                                    family, prefix_str, prefix_len, 1,
                                    SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Set Route Next-hop attribute with a non existing VRF */
    sai_rc = sai_test_route_attr_set (invalid_vrf,
                                      family, prefix_str, prefix_len,
                                      SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Remove Route with a non existing VRF */
    sai_rc = sai_test_route_remove (invalid_vrf,
                                    family, prefix_str, prefix_len);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
}

/*
 * Check if the Route create, set and remove APIs called with invalid IP family 
 * returns appropriate error status.
 */
TEST_F (saiL3RouteTest, route_apis_invoke_with_invalid_ip_family)
{
    sai_status_t  sai_rc = SAI_STATUS_SUCCESS;
    const char   *prefix_str = "50.0.0.0";
    unsigned int  prefix_len = 8;
    unsigned int  family = 0xff;

    /* Create Route with invalid IP family parameter */
    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len, 1,
                                    SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_INVALID_PARAMETER, sai_rc);

    /* Set Route Next-hop attribute with invalid IP family parameter */
    sai_rc = sai_test_route_attr_set (vr_id, family, prefix_str, prefix_len,
                                      SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id_1);

    EXPECT_EQ (SAI_STATUS_INVALID_PARAMETER, sai_rc);

    /* Remove Route with invalid IP family parameter */
    sai_rc = sai_test_route_remove (vr_id, family, prefix_str, prefix_len);

    EXPECT_EQ (SAI_STATUS_INVALID_PARAMETER, sai_rc);
}

/*
 * Check if the Route create API called with invalid Next Hop ID 
 * returns appropriate error status.
 */
TEST_F (saiL3RouteTest, route_create_with_invalid_nh_id)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    const char           *prefix_str = "50.0.0.0";
    unsigned int          prefix_len = 8;
    sai_ip_addr_family_t  family = SAI_IP_ADDR_FAMILY_IPV4;
    sai_object_id_t       invalid_nh_id = 0;

    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len, 1,
                                    SAI_ROUTE_ATTR_NEXT_HOP_ID, invalid_nh_id);

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

/*
 * Check if the Route create API called with invalid packet action 
 * returns appropriate error status.
 */
TEST_F (saiL3RouteTest, route_create_with_invalid_packet_action)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    const char           *prefix_str = "50.0.0.0";
    unsigned int          prefix_len = 8;
    sai_ip_addr_family_t  family = SAI_IP_ADDR_FAMILY_IPV4;
    unsigned int          invalid_pkt_action = 0xff;

    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len, 1,
                                    SAI_ROUTE_ATTR_PACKET_ACTION,
                                    invalid_pkt_action);

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

/*
 * Check if the Route create API called with invalid attribute ID 
 * returns appropriate error status.
 */
TEST_F (saiL3RouteTest, route_create_with_invalid_attr_id)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    const char           *prefix_str = "50.0.0.0";
    unsigned int          prefix_len = 8;
    sai_ip_addr_family_t  family = SAI_IP_ADDR_FAMILY_IPV4;
    unsigned int          invalid_attr_id = 0xFF;

    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len, 1,
                                    invalid_attr_id, 1);

    EXPECT_EQ (SAI_STATUS_UNKNOWN_ATTRIBUTE_0, sai_rc);
}

/*
 * Validates Route creation with a next-hop pointing to LAG object.
 */
TEST_F (saiL3RouteTest, route_next_hop_on_lag_id)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       lag_id;
    sai_object_id_t       rif_id;
    sai_object_id_t       nh_id;
    sai_lag_api_t*        sai_lag_api_table;
    sai_attribute_t       attr_list[2];
    const char           *ip_str = "21.1.1.1";
    const char           *prefix_str = "100.0.0.0";
    unsigned int          prefix_len = 8;
    sai_ip_addr_family_t  family = SAI_IP_ADDR_FAMILY_IPV4;
    const unsigned int    member_count = 2;
    sai_object_id_t       port_arr[member_count];
    sai_vlan_port_t       vlan_port[member_count];
    sai_object_id_t       member_arr[member_count];
    unsigned int          index;
    const unsigned int    attr_count = 2;

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_LAG, (static_cast<void**>
                              (static_cast<void*> (&sai_lag_api_table)))));

    ASSERT_TRUE(sai_lag_api_table != NULL);
    ASSERT_TRUE(sai_vlan_api_table != NULL);

    /* Remove the port members from default VLAN */
    port_arr[0] = sai_l3_port_id_get (2);
    port_arr[1] = sai_l3_port_id_get (3);
    vlan_port[0].port_id = port_arr[0];
    vlan_port[1].port_id = port_arr[1];
    vlan_port[0].tagging_mode = SAI_VLAN_PORT_UNTAGGED;
    vlan_port[1].tagging_mode = SAI_VLAN_PORT_UNTAGGED;

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_vlan_api_table->remove_ports_from_vlan(
               saiL3Test::SAI_TEST_DEFAULT_VLAN, 2,
               (const sai_vlan_port_t*)vlan_port));

    /* Create the LAG object with port members */
    sai_rc = sai_lag_api_table->create_lag (&lag_id, 0, attr_list);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    for (index = 0; index < member_count; index++) {
        attr_list [0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
        attr_list [0].value.oid = lag_id;

        attr_list [1].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
        attr_list [1].value.oid = port_arr [index];

        sai_rc = sai_lag_api_table->create_lag_member (&member_arr [index],
                                                       attr_count, attr_list);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    /* Create RIF with LAG object id */
    sai_rc = sai_test_rif_create (&rif_id, default_rif_attr_count,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  lag_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Neighbor creation */
    sai_rc = sai_test_neighbor_create (rif_id, SAI_IP_ADDR_FAMILY_IPV4,
                                       ip_str, default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       "00:22:22:22:22:22");

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create Next Hop object */
    sai_rc = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE, SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      SAI_IP_ADDR_FAMILY_IPV4, ip_str,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      rif_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create Route to FWD packets on Next-hop nh_id */
    sai_rc = sai_test_route_create (vr_id, family, prefix_str, prefix_len,
                                    1, SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, family, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id,
                                SAI_TEST_ROUTE_DFLT_PKT_ACTION,
                                SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

    /* Remove Route */
    sai_test_route_remove_and_verify (vr_id, family, prefix_str, prefix_len);

    /* Remove Next Hop object */
    sai_rc = sai_test_nexthop_remove (nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove Neighbor entry */
    sai_rc = sai_test_neighbor_remove (rif_id, SAI_IP_ADDR_FAMILY_IPV4,
                                       ip_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove RIF object */
    sai_rc = sai_test_rif_remove (rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the LAG members */
    for (index = 0; index < member_count; index++) {

        sai_rc = sai_lag_api_table->remove_lag_member (member_arr [index]);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    /* Remove the LAG */
    sai_rc = sai_lag_api_table->remove_lag (lag_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Add the port members back to default VLAN */
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_vlan_api_table->add_ports_to_vlan(
               saiL3Test::SAI_TEST_DEFAULT_VLAN, member_count,
               (const sai_vlan_port_t*)vlan_port));
}

/*
 * Validates Route creation with a next-hop that has the neighbor entry 
 * set with 'no_host_route' attribute.
 */
TEST_F (saiL3RouteTest, route_next_hop_with_no_host_route_attr)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    const char           *ip_str = "FE80::C001:1DFF:FEE0:0";
    sai_object_id_t       nh_id;
    const char           *prefix_str = "2001:DB8:c18:1::";
    unsigned int          prefix_len = 64;
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV6;

    /* Create IPv6 link-local address neighbor entry with no-host-route attr */
    sai_rc = sai_test_neighbor_create (rif_id_1, ip_af, ip_str,
                                       default_neighbor_attr_count + 1,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       "00:22:22:22:22:22",
                                       SAI_NEIGHBOR_ATTR_NO_HOST_ROUTE,
                                       true);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create the Next Hop object */
    sai_rc = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE, SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, ip_str,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      rif_id_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create Route to FWD packets on Next-hop nh_id */
    sai_rc = sai_test_route_create (vr_id, ip_af, prefix_str, prefix_len,
                                    1, SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_route_attr_verify (vr_id, ip_af, prefix_str, prefix_len,
                                SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id,
                                SAI_TEST_ROUTE_DFLT_PKT_ACTION,
                                SAI_TEST_ROUTE_DFLT_TRAP_PRIO);

    /* Remove Route */
    sai_test_route_remove_and_verify (vr_id, ip_af, prefix_str, prefix_len);

    /* Remove Next Hop object */
    sai_rc = sai_test_nexthop_remove (nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove Neighbor entry */
    sai_rc = sai_test_neighbor_remove (rif_id_1, ip_af, ip_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
