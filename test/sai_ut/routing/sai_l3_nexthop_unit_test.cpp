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
*    sai_l3_nexthop_unit_test.cpp
*     
* Abstract:
* 
*    SAI NEXT HOP UNIT TEST :- Covers the test cases for validating all 
*    public APIs in SAI NEXTHOP module.
*
*************************************************************************/

#include "gtest/gtest.h"

#include "sai_l3_unit_test_utils.h"

extern "C" {
#include "sai.h"
#include "saistatus.h"
#include "saitypes.h"

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <inttypes.h>
}

class saiL3NextHopTest : public saiL3Test
{
    public:
        static void SetUpTestCase (void);
        static void TearDownTestCase (void);

        static void sai_ip_nh_verify_after_creation (
                                              sai_object_id_t nh_id,
                                              sai_object_id_t rif_id,
                                              sai_ip_addr_family_t ip_af,
                                              const char *p_ip_addr_str);
        static void sai_ip_nh_verify_after_removal (sai_object_id_t nh_id);

        static void sai_ip_nh_verify_neighbor (sai_object_id_t rif_id,
                                               sai_ip_addr_family_t ip_af,
                                               const char *p_ip_addr_str);

        static void sai_ip_nh_create_neighbor (sai_object_id_t rif_id,
                                               sai_ip_addr_family_t ip_af,
                                               const char *p_ip_addr_str,
                                               unsigned int vlan_id);

        static void sai_ip_nh_remove_neighbor (sai_object_id_t rif_id,
                                               sai_ip_addr_family_t ip_af,
                                               const char *p_ip_addr_str,
                                               unsigned int vlan_id);

        static const unsigned int default_port = 0;
        static const unsigned int default_vlan = 100;
        static const char        *neighbor_mac_str;

        static sai_object_id_t  port_id;
        static sai_object_id_t  vrf_id;
        static sai_object_id_t  port_rif_id;
        static sai_object_id_t  vlan_rif_id;
};

sai_object_id_t saiL3NextHopTest::port_id = 0;
sai_object_id_t saiL3NextHopTest::vrf_id = 0;
sai_object_id_t saiL3NextHopTest::port_rif_id = 0;
sai_object_id_t saiL3NextHopTest::vlan_rif_id = 0;
const char*     saiL3NextHopTest::neighbor_mac_str = "00:a1:a2:a3:a4:a5";

void saiL3NextHopTest::SetUpTestCase (void)
{
    sai_status_t    status;

    /* Base SetUpTestCase for SAI initialization */
    saiL3Test::SetUpTestCase ();

    /* SAI Router default MAC address init */
    status = sai_test_router_mac_init (router_mac);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    port_id = sai_l3_port_id_get (default_port);

    /* Create Vlan */
    status = sai_test_vlan_create (default_vlan);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create a Virtual Router instance */
    status = sai_test_vrf_create (&vrf_id, 0);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create a Port RIF */
    status = sai_test_rif_create (&port_rif_id, default_rif_attr_count,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vrf_id,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  port_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create a VLAN RIF */
    status = sai_test_rif_create (&vlan_rif_id, default_rif_attr_count,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vrf_id,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  default_vlan);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);
}

void saiL3NextHopTest::TearDownTestCase (void)
{
    sai_status_t  status;

    /* Remove the VLAN RIF */
    status = sai_test_rif_remove (vlan_rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Remove the Port RIF */
    status = sai_test_rif_remove (port_rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Remove the VRF */
    status = sai_test_vrf_remove (vrf_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Remove Vlan */
    status = sai_test_vlan_remove (default_vlan);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);
}

/*
 * Helper function to verify the IP Nexthop attributes after creation.
 */
void saiL3NextHopTest::sai_ip_nh_verify_after_creation (
                                               sai_object_id_t nh_id,
                                               sai_object_id_t rif_id,
                                               sai_ip_addr_family_t ip_af,
                                               const char *p_ip_addr_str)
{
    char            ip_addr_buf [256];
    sai_status_t    status;
    sai_attribute_t attr_list [default_nh_attr_count];

    status = sai_test_nexthop_attr_get (nh_id, attr_list, default_nh_attr_count,
                                        SAI_NEXT_HOP_ATTR_TYPE,
                                        SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                        SAI_NEXT_HOP_ATTR_IP);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    EXPECT_EQ (SAI_NEXT_HOP_IP, attr_list[0].value.s32);
    EXPECT_EQ (rif_id, attr_list[1].value.oid);
    EXPECT_EQ (ip_af, attr_list[2].value.ipaddr.addr_family);

    sai_test_ip_addr_to_str (&attr_list[2].value.ipaddr, ip_addr_buf,
                             sizeof (ip_addr_buf));

    EXPECT_STREQ (p_ip_addr_str, ip_addr_buf);
}

/*
 * Helper function to verify IP Neighbor attribute get is successful after 
 * IP Nexthop is removed.
 */
void saiL3NextHopTest::sai_ip_nh_verify_neighbor (
                                               sai_object_id_t rif_id,
                                               sai_ip_addr_family_t ip_af,
                                               const char *p_ip_addr_str)
{
    sai_status_t    status;
    sai_attribute_t attr_list [default_neighbor_attr_count];

    status = sai_test_neighbor_attr_get (rif_id, ip_af, p_ip_addr_str,
                                         attr_list, default_neighbor_attr_count,
                                         SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);
}

/*
 * Helper function to verify IP Nexthop does not exist.
 */
void saiL3NextHopTest::sai_ip_nh_verify_after_removal (sai_object_id_t nh_id)
{
    sai_status_t  status;

    status = sai_test_nexthop_remove (nh_id);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, status);
}

/*
 * Helper function to create Neighbor entry first for the IP Nexthop.
 */

void saiL3NextHopTest::sai_ip_nh_create_neighbor (
                                               sai_object_id_t rif_id,
                                               sai_ip_addr_family_t ip_af,
                                               const char *p_ip_addr_str,
                                               unsigned int vlan_id)
{
    sai_status_t    status;

    if (vlan_id) {

        /* Create the L2 FDB entry for the neighbor dest mac */
        status = sai_test_neighbor_fdb_entry_create (neighbor_mac_str, 
                                                     vlan_id,
                                                     default_port);

        ASSERT_EQ (SAI_STATUS_SUCCESS, status);
    }

    status = sai_test_neighbor_create (rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       neighbor_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);
}

/*
 * Helper function to cleanup Neighbor entry for the IP Nexthop.
 */

void saiL3NextHopTest::sai_ip_nh_remove_neighbor (
                                               sai_object_id_t rif_id,
                                               sai_ip_addr_family_t ip_af,
                                               const char *p_ip_addr_str,
                                               unsigned int vlan_id)
{
    sai_status_t    status;

    /* Cleanup the neighbor entry */
    status = sai_test_neighbor_remove (rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    if (vlan_id) {

        /* Remove the L2 FDB entry for the neighbor dest mac */
        status = sai_test_neighbor_fdb_entry_remove (neighbor_mac_str, 
                                                     vlan_id);

        ASSERT_EQ (SAI_STATUS_SUCCESS, status);
    }
}

/*
 * Validate Nexthop creation and removal on a port based Router Interface.
 */
TEST_F (saiL3NextHopTest, create_and_remove_on_port_rif)
{
    sai_status_t          status;
    sai_object_id_t       nh_id = 0;
    const char           *p_nh_ip_addr = "11.0.0.1";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;

    /* Create the neighbor entry first */
    sai_ip_nh_create_neighbor (port_rif_id, ip_af, p_nh_ip_addr, 0);

    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      port_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    printf ("SAI Next Hop Object Id: 0x%"PRIx64".\n", nh_id);

    sai_ip_nh_verify_after_creation (nh_id, port_rif_id, ip_af, p_nh_ip_addr);

    status = sai_test_nexthop_remove (nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_ip_nh_verify_after_removal (nh_id);

    sai_ip_nh_remove_neighbor (port_rif_id, ip_af, p_nh_ip_addr, 0);
}

/*
 * Validate Nexthop creation and removal on a VLAN based Router Interface.
 */
TEST_F (saiL3NextHopTest, create_and_remove_on_vlan_rif)
{
    sai_status_t          status;
    sai_object_id_t       nh_id = 0;
    const char           *p_nh_ip_addr = "11.0.0.1";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;

    /* Create the neighbor entry first */
    sai_ip_nh_create_neighbor (vlan_rif_id, ip_af, p_nh_ip_addr, default_vlan);

    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      vlan_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    printf ("SAI Next Hop Object Id: 0x%"PRIx64".\n", nh_id);

    sai_ip_nh_verify_after_creation (nh_id, vlan_rif_id, ip_af, p_nh_ip_addr);

    status = sai_test_nexthop_remove (nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_ip_nh_verify_after_removal (nh_id);

    sai_ip_nh_remove_neighbor (vlan_rif_id, ip_af, p_nh_ip_addr, default_vlan);
}

/*
 * Validate Nexthop creation and removal for IPv6 address.
 */
TEST_F (saiL3NextHopTest, create_and_remove_with_ipv6_addr)
{
    sai_status_t         status;
    sai_object_id_t      nh_id = 0;
    const char          *p_nh_ip_addr = "2001:db8:85a3::8a2e:370:7334";
    sai_ip_addr_family_t ip_af = SAI_IP_ADDR_FAMILY_IPV6;

    /* Create the neighbor entry first */
    sai_ip_nh_create_neighbor (vlan_rif_id, ip_af, p_nh_ip_addr, default_vlan);

    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      vlan_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    printf ("%s(): SAI Next Hop Object Id: 0x%"PRIx64".\n", __FUNCTION__, nh_id);

    sai_ip_nh_verify_after_creation (nh_id, vlan_rif_id, ip_af, p_nh_ip_addr);

    status = sai_test_nexthop_remove (nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_ip_nh_verify_after_removal (nh_id);

    sai_ip_nh_remove_neighbor (vlan_rif_id, ip_af, p_nh_ip_addr, default_vlan);
}

/*
 * Validate Nexthop creation and removal when its neighbor entry exists.
 */
TEST_F (saiL3NextHopTest, create_and_remove_when_neighbor_exists)
{
    sai_status_t          status;
    sai_object_id_t       nh_id = 0;
    const char           *p_nh_ip_addr = "11.0.0.1";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;

    /* Create the neighbor entry first */
    status = sai_test_neighbor_create (port_rif_id, ip_af, p_nh_ip_addr,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       "00:a1:a2:a3:a4:a5");

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      port_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    printf ("SAI Next Hop Object Id: 0x%"PRIx64".\n", nh_id);

    sai_ip_nh_verify_after_creation (nh_id, port_rif_id, ip_af, p_nh_ip_addr);

    status = sai_test_nexthop_remove (nh_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    sai_ip_nh_verify_after_removal (nh_id);

    /* Verify the neighbor entry still exists */
    sai_ip_nh_verify_neighbor (port_rif_id, ip_af, p_nh_ip_addr);

    /* Cleanup the neighbor entry */
    status = sai_test_neighbor_remove (port_rif_id, ip_af, p_nh_ip_addr);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);
}

/*
 * Validate that Nexthop with same IP address can be created on 
 * router interfaces in different VRF.
 */
TEST_F (saiL3NextHopTest, duplicate_nh_ip_in_different_vrf)
{
    sai_status_t            status;
    sai_object_id_t         nh_id1 = 0;
    sai_object_id_t         nh_id2 = 0;
    const char             *p_nh_ip_addr = "2001:db8:85a3::8a2e:370:7334";
    sai_ip_addr_family_t    ip_af = SAI_IP_ADDR_FAMILY_IPV6;
    sai_object_id_t         new_vrf_id;
    sai_object_id_t         new_rif_id;
    const unsigned int      new_vlan = 200;

    /* Create Vlan */
    status = sai_test_vlan_create (new_vlan);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create a different Virtual Router instance */
    status = sai_test_vrf_create (&new_vrf_id, 0);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create a VLAN RIF on the new VRF */
    status = sai_test_rif_create (&new_rif_id, default_rif_attr_count,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  new_vrf_id,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  new_vlan);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create the neighbor entry first */
    sai_ip_nh_create_neighbor (vlan_rif_id, ip_af, p_nh_ip_addr, default_vlan);

    status = sai_test_nexthop_create (&nh_id1, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      vlan_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    printf ("SAI Next Hop Object Id for NH 1: 0x%"PRIx64".\n", nh_id1);

    /* Create another next hop with same address in the different VRF */
    sai_ip_nh_create_neighbor (new_rif_id, ip_af, p_nh_ip_addr, new_vlan);

    status = sai_test_nexthop_create (&nh_id2, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      new_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    printf ("SAI Next Hop Object Id for NH 2: 0x%"PRIx64".\n", nh_id2);

    sai_ip_nh_verify_after_creation (nh_id2, new_rif_id, ip_af, p_nh_ip_addr);

    status = sai_test_nexthop_remove (nh_id1);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_ip_nh_verify_after_removal (nh_id1);

    status = sai_test_nexthop_remove (nh_id2);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_ip_nh_verify_after_removal (nh_id2);

    /* Remove the neighbor entries */
    sai_ip_nh_remove_neighbor (vlan_rif_id, ip_af, p_nh_ip_addr, default_vlan);
    sai_ip_nh_remove_neighbor (new_rif_id, ip_af, p_nh_ip_addr, new_vlan);

    /* Remove the VLAN RIF */
    status = sai_test_rif_remove (new_rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Remove the VRF */
    status = sai_test_vrf_remove (new_vrf_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Remove Vlan */
    status = sai_test_vlan_remove (new_vlan);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);
}

/*
 * Check if Nexthop creation with same IP address on same VRF is rejected with 
 * appropriate error status.
 */
TEST_F (saiL3NextHopTest, duplicate_nh_in_same_vrf)
{
    sai_status_t          status;
    sai_object_id_t       nh_id = 0;
    const char           *p_nh_ip_addr = "11.0.0.1";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;

    /* Create the neighbor entry first */
    sai_ip_nh_create_neighbor (vlan_rif_id, ip_af, p_nh_ip_addr, default_vlan);

    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      vlan_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    printf ("SAI Next Hop Object Id: 0x%"PRIx64".\n", nh_id);

    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      vlan_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    EXPECT_EQ (SAI_STATUS_ITEM_ALREADY_EXISTS, status);

    status = sai_test_nexthop_remove (nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_ip_nh_verify_after_removal (nh_id);

    sai_ip_nh_remove_neighbor (vlan_rif_id, ip_af, p_nh_ip_addr, default_vlan);
}

/*
 * Check if Nexthop creation with invalid SAI_NEXT_HOP_ATTR_TYPE attribute 
 * value returns appropriate error status.
 */
TEST_F (saiL3NextHopTest, invalid_nh_type_attr_value)
{
    sai_status_t           status;
    sai_object_id_t        nh_id = 0;
    sai_ip_addr_family_t   ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char            *p_nh_ip_addr = "11.0.0.1";
    unsigned int           invalid_attr_index = 0;
    sai_int32_t            invalid_nh_type = -1;

    /* Pass invalid nh type attr value */
    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      invalid_nh_type,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      vlan_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    EXPECT_EQ ((sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTR_VALUE_0,
                invalid_attr_index)), status);
}

/*
 * Check if Nexthop creation with invalid SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID
 * attribute value returns appropriate error status.
 */
TEST_F (saiL3NextHopTest, invalid_rif_id_attr_value)
{
    sai_status_t             status;
    sai_object_id_t          nh_id = 0;
    sai_ip_addr_family_t     ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char              *p_nh_ip_addr = "11.0.0.1";
    unsigned int             invalid_attr_index = 1;
    sai_object_id_t          invalid_rif_id = 0;

    /* Pass RIF Id value that does not exist */
    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      invalid_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    EXPECT_EQ ((sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTR_VALUE_0,
                invalid_attr_index)), status);
}

/*
 * Check if Nexthop creation with invalid SAI_NEXT_HOP_ATTR_IP attribute 
 * value returns appropriate error status.
 */
TEST_F (saiL3NextHopTest, invalid_nh_ip_attr_value)
{
    sai_status_t        status;
    sai_object_id_t     nh_id = 0;
    unsigned int        invalid_attr_index = 2;
    sai_int32_t         invalid_ip_af = -1;

    /* Pass invalid IP address family value */
    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      vlan_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      invalid_ip_af, "11.0.0.1");

    EXPECT_EQ ((sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTR_VALUE_0,
                invalid_attr_index)), status);

    /* Pass zero IPv4 address */
    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      vlan_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      SAI_IP_ADDR_FAMILY_IPV4,
                                      "0.0.0.0");

    EXPECT_EQ ((sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTR_VALUE_0,
                invalid_attr_index)), status);

    /* Pass zero IPv6 address */
    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      vlan_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      SAI_IP_ADDR_FAMILY_IPV6,
                                      "::");

    EXPECT_EQ ((sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTR_VALUE_0,
                invalid_attr_index)), status);
}

/*
 * Check if Nexthop creation with mandatory attributes missing returns 
 * appropriate error status.
 */
TEST_F (saiL3NextHopTest, mandatory_attr_missing)
{
    sai_status_t         status;
    sai_object_id_t      nh_id = 0;
    const char          *p_nh_ip_addr = "11.0.0.1";
    sai_ip_addr_family_t ip_af = SAI_IP_ADDR_FAMILY_IPV4;

    /* Miss the NH type attribute */
    status = sai_test_nexthop_create (&nh_id,
                                      (default_nh_attr_count - 1),
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      vlan_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, status);

    /* Miss the NH RIF Id attribute */
    status = sai_test_nexthop_create (&nh_id,
                                      (default_nh_attr_count - 1),
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, status);

    /* Miss the NH IP Addr attribute */
    status = sai_test_nexthop_create (&nh_id,
                                      (default_nh_attr_count - 1),
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      vlan_rif_id);

    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, status);
}

/*
 * Check if Nexthop creation with invalid attribute ID returns appropriate 
 * error status.
 */
TEST_F (saiL3NextHopTest, invalid_attr_id)
{
    sai_status_t          status;
    sai_object_id_t       nh_id = 0;
    const char           *p_nh_ip_addr = "11.0.0.1";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const unsigned int    invalid_attr_pos = 3;
    const unsigned int    invalid_attr_id = 0xffff;

    /* Pass invalid attribute id in list */
    status = sai_test_nexthop_create (&nh_id,
                                      (default_nh_attr_count + 1),
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      vlan_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr,
                                      invalid_attr_id, 0);

    EXPECT_EQ ((sai_test_invalid_attr_status_code (SAI_STATUS_UNKNOWN_ATTRIBUTE_0,
                invalid_attr_pos)), status);
}

/*
 * Check if calling nexthop remove API for an invalid nexthop ID returns 
 * appropriate error status.
 */
TEST_F (saiL3NextHopTest, removal_for_invalid_nh_id)
{
    sai_status_t        status;
    sai_object_id_t     invalid_nh_id = 0;

    status = sai_test_nexthop_remove (invalid_nh_id);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, status);
}

/*
 * Validates whether trying to remove Nexthop when a route is associated to it 
 * is rejected with appropriate error status.
 */
TEST_F (saiL3NextHopTest, removal_when_route_exists)
{
    sai_status_t          status;
    sai_object_id_t       nh_id = 0;
    const char           *p_nh_ip_addr = "11.0.0.1";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char           *p_route_str = "100.1.1.0";
    const unsigned int    prefix_len = 24;

    /* Create the neighbor entry first */
    sai_ip_nh_create_neighbor (port_rif_id, ip_af, p_nh_ip_addr, 0);

    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      port_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    printf ("SAI Next Hop Object Id: 0x%"PRIx64".\n", nh_id);

    /* Create a route pointing to the next-hop */
    status = sai_test_route_create (vrf_id, ip_af, (char *)p_route_str, 
                                    prefix_len, 1,
                                    SAI_ROUTE_ATTR_NEXT_HOP_ID, nh_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Verify the next hop cannot be removed */
    status = sai_test_nexthop_remove (nh_id);

    EXPECT_NE (SAI_STATUS_SUCCESS, status);

    /* Clean-up the route */
    status = sai_test_route_remove (vrf_id, ip_af, (char*)p_route_str, 
                                    prefix_len);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Verify next-hop can be removed now */
    status = sai_test_nexthop_remove (nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_ip_nh_verify_after_removal (nh_id);

    sai_ip_nh_remove_neighbor (port_rif_id, ip_af, p_nh_ip_addr, 0);
}

/*
 * Validates whether trying to remove Nexthop when the nexthop is associated to
 * a nexthop group is rejected with appropriate error status.
 */
TEST_F (saiL3NextHopTest, removal_when_added_in_nh_group)
{
    sai_status_t           status;
    sai_object_id_t        nh_id = 0;
    const char            *p_nh_ip_addr = "11.0.0.1";
    sai_ip_addr_family_t   ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const unsigned int     group_nh_count = 1;
    sai_object_id_t        group_id = 0;

    /* Create the neighbor entry first */
    sai_ip_nh_create_neighbor (port_rif_id, ip_af, p_nh_ip_addr, 0);

    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      port_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    printf ("SAI Next Hop Object Id: 0x%"PRIx64".\n", nh_id);

    /* Create a Next Hop Group pointing to the next-hop */
    status = sai_test_nh_group_create (&group_id, &nh_id,
                                       default_nh_group_attr_count,
                                       SAI_NEXT_HOP_GROUP_ATTR_TYPE,
                                       SAI_NEXT_HOP_GROUP_ECMP,
                                       SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST,
                                       group_nh_count);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    printf ("SAI Next Hop Group Object Id: 0x%"PRIx64".\n", group_id);

    /* Verify the next hop cannot be removed */
    status = sai_test_nexthop_remove (nh_id);

    EXPECT_NE (SAI_STATUS_SUCCESS, status);

    /* Remove the Next Hop Group */
    status = sai_test_nh_group_remove (group_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Verify next-hop can be removed now */
    status = sai_test_nexthop_remove (nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_ip_nh_verify_after_removal (nh_id);

    sai_ip_nh_remove_neighbor (port_rif_id, ip_af, p_nh_ip_addr, 0);
}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest (&argc, argv);

    return RUN_ALL_TESTS ();
}
