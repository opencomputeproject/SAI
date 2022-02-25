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
*    sai_l3_neighbor_unit_test.cpp
*     
* Abstract:
*
*    SAI NEIGHBOR UNIT TEST :- Covers the test cases for validating all 
*    public APIs in SAI NEIGHBOR module.
*
*************************************************************************/
#include "gtest/gtest.h"

#include "sai_l3_unit_test_utils.h"

extern "C" {
#include "sai.h"
#include "saistatus.h"
#include "saitypes.h"
#include "saifdb.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <inttypes.h>
}

class saiL3NeighborTest : public saiL3Test
{
    public:
        static void SetUpTestCase (void);
        static void TearDownTestCase (void);

        static void sai_neighbor_verify_after_creation (
                                            sai_object_id_t rif_id,
                                            sai_ip_addr_family_t ip_af,
                                            const char *p_ip_addr_str,
                                            const char *p_dest_mac_str,
                                            sai_packet_action_t pkt_action);
        static void sai_neighbor_verify_after_removal (
                                            sai_object_id_t rif_id,
                                            sai_ip_addr_family_t ip_af,
                                            const char *p_ip_addr_str);

        static void sai_verify_ip_nh_after_neighbor_removal (
                                            sai_object_id_t nh_id,
                                            sai_object_id_t rif_id,
                                            sai_ip_addr_family_t ip_af,
                                            const char *p_ip_addr_str);

        static const unsigned int default_port = 0;
        static const unsigned int default_vlan = 42;

        static const unsigned int max_neighbor_attr_count = 2;

        static const sai_packet_action_t default_pkt_action = \
                                                     SAI_PACKET_ACTION_FORWARD;

        static sai_object_id_t  default_port_id;
        static sai_object_id_t  vrf_id;
        static sai_object_id_t  port_rif_id;
        static sai_object_id_t  vlan_rif_id;
};

sai_object_id_t saiL3NeighborTest::default_port_id = 0;
sai_object_id_t saiL3NeighborTest::vrf_id = 0;
sai_object_id_t saiL3NeighborTest::port_rif_id = 0;
sai_object_id_t saiL3NeighborTest::vlan_rif_id = 0;

void saiL3NeighborTest::SetUpTestCase (void)
{
    sai_status_t    status;

    /* Base SetUpTestCase for SAI initialization */
    saiL3Test::SetUpTestCase ();

    /* SAI Router default MAC address init */
    status = sai_test_router_mac_init (router_mac);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create Vlan */
    status = sai_test_vlan_create (default_vlan);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create a Virtual Router instance */
    status = sai_test_vrf_create (&vrf_id, 0);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    default_port_id = sai_l3_port_id_get (default_port);

    /* Create a Port RIF */
    status = sai_test_rif_create (&port_rif_id, default_rif_attr_count,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vrf_id,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  default_port_id);

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

void saiL3NeighborTest::TearDownTestCase (void)
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
 * Helper function to verify the neighbor attributes after Neighbor is created.
 */
void saiL3NeighborTest::sai_neighbor_verify_after_creation (
                                            sai_object_id_t rif_id,
                                            sai_ip_addr_family_t ip_af,
                                            const char *p_ip_addr_str,
                                            const char *p_mac_str,
                                            sai_packet_action_t pkt_action)
{
    unsigned int    cmp_result = 0;
    uint8_t         dest_mac [6];
    sai_status_t    status;
    sai_attribute_t attr_list [max_neighbor_attr_count];

    sai_test_router_mac_str_to_bytes_get (p_mac_str, dest_mac);

    status = sai_test_neighbor_attr_get (rif_id, ip_af, p_ip_addr_str,
                                         attr_list, max_neighbor_attr_count,
                                         SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                         SAI_NEIGHBOR_ATTR_PACKET_ACTION);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    cmp_result = memcmp (dest_mac, attr_list[0].value.mac, sizeof (sai_mac_t));

    EXPECT_EQ (cmp_result, 0);
    EXPECT_EQ (pkt_action, attr_list[1].value.s32);
}

/*
 * Helper function to verify Neighbor removal is successful.
 */
void saiL3NeighborTest::sai_neighbor_verify_after_removal (
                                            sai_object_id_t rif_id,
                                            sai_ip_addr_family_t ip_af,
                                            const char *p_ip_addr_str)
{
    sai_status_t  status;

    status = sai_test_neighbor_remove (rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_ITEM_NOT_FOUND, status);
}

/*
 * Helper function to verify IP Nexthop attributes after neighbor is removed.
 */
void saiL3NeighborTest::sai_verify_ip_nh_after_neighbor_removal (
                                            sai_object_id_t nh_id,
                                            sai_object_id_t rif_id,
                                            sai_ip_addr_family_t ip_af,
                                            const char *p_ip_addr_str)
{
    sai_status_t    status;
    sai_attribute_t attr_list [default_nh_attr_count];
    char            ip_addr_buf [256];

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
 * Validates Neighbor creation and removal for IPv4 address family on a 
 * Port router interface.
 */
TEST_F (saiL3NeighborTest, create_and_remove_on_port_rif)
{
    sai_status_t          status;
    const char           *p_ip_addr_str = "11.0.0.1";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char           *p_mac_str = "00:a1:a2:a3:a4:a5";

    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_creation (port_rif_id, ip_af, p_ip_addr_str,
                                        p_mac_str, default_pkt_action);

    status = sai_test_neighbor_remove (port_rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_removal (port_rif_id, ip_af, p_ip_addr_str);
}

/*
 * Validates Neighbor creation and removal for IPv6 address family on a Port 
 * router interface.
 */
TEST_F (saiL3NeighborTest, create_and_remove_with_ipv6_addr)
{
    sai_status_t          status;
    const char           *p_ip_addr_str = "2001:db8:85a3::8a2e:370:7334";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV6;
    const char           *p_mac_str = "00:a1:a2:a3:a4:a5";

    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_creation (port_rif_id, ip_af, p_ip_addr_str,
                                        p_mac_str, default_pkt_action);

    status = sai_test_neighbor_remove (port_rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_removal (port_rif_id, ip_af, p_ip_addr_str);
}

/*
 * Validates Neighbor creation and removal with SAI_NEIGHBOR_ATTR_PACKET_ACTION
 * attribute set.
 */
TEST_F (saiL3NeighborTest, create_and_remove_with_pkt_action)
{
    sai_status_t          status;
    const char           *p_ip_addr_str = "2001:db8:85a3::8a2e:370:7334";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV6;
    const char           *p_mac_str = "00:a1:a2:a3:a4:a5";
    sai_packet_action_t   pkt_action = SAI_PACKET_ACTION_LOG;
    const unsigned int    attr_count = default_neighbor_attr_count + 1;

    /* Create with a non-default packet action */
    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str,
                                       SAI_NEIGHBOR_ATTR_PACKET_ACTION,
                                       pkt_action);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_creation (port_rif_id, ip_af, p_ip_addr_str,
                                        p_mac_str, pkt_action);

    status = sai_test_neighbor_remove (port_rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_removal (port_rif_id, ip_af, p_ip_addr_str);
}

/*
 * Validates Neighbor creation and removal for IPv4 address family on a 
 * VLAN router interface.
 */
TEST_F (saiL3NeighborTest, create_and_remove_on_vlan_rif)
{
    sai_status_t          status;
    const char           *p_ip_addr_str = "11.0.0.1";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char           *p_mac_str = "00:c1:c2:c3:c4:c5";

    /* Create the L2 FDB entry for the neighbor dest mac */
    status = sai_test_neighbor_fdb_entry_create (p_mac_str, default_vlan,
                                                 default_port);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    status = sai_test_neighbor_create (vlan_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_creation (vlan_rif_id, ip_af, p_ip_addr_str,
                                        p_mac_str, default_pkt_action);

    status = sai_test_neighbor_remove (vlan_rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_removal (port_rif_id, ip_af, p_ip_addr_str);

    /* Remove the L2 FDB entry for the neighbor dest mac */
    status = sai_test_neighbor_fdb_entry_remove (p_mac_str, default_vlan);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);
}

/*
 * Validate setting SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS for an existing 
 * neighbor.
 */
TEST_F (saiL3NeighborTest, set_dest_mac_attr)
{
    sai_status_t          status;
    const char           *p_ip_addr_str = "11.0.0.1";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char           *p_mac_str = "00:a1:a2:a3:a4:a5";
    const char           *p_new_mac_str = "00:b1:b2:b3:b4:b5";

    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_creation (port_rif_id, ip_af, p_ip_addr_str,
                                        p_mac_str, default_pkt_action);

    /* Set the new mac address value to neighbor dest_mac attr */
    status = sai_test_neighbor_attr_set (port_rif_id, ip_af, p_ip_addr_str,
                                         SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                         0, p_new_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_creation (port_rif_id, ip_af, p_ip_addr_str,
                                        p_new_mac_str, default_pkt_action);

    status = sai_test_neighbor_remove (port_rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_removal (port_rif_id, ip_af, p_ip_addr_str);
}

/*
 * Validate setting SAI_NEIGHBOR_ATTR_PACKET_ACTION for an existing neighbor.
 */
TEST_F (saiL3NeighborTest, set_pkt_action_attr)
{
    sai_status_t          status;
    const char           *p_ip_addr_str = "11.0.0.1";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char           *p_mac_str = "00:a1:a2:a3:a4:a5";
    sai_packet_action_t   new_pkt_action = SAI_PACKET_ACTION_DROP;

    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_creation (port_rif_id, ip_af, p_ip_addr_str,
                                        p_mac_str, default_pkt_action);

    /* Set the new packet action to neighbor attr */
    status = sai_test_neighbor_attr_set (port_rif_id, ip_af, p_ip_addr_str,
                                         SAI_NEIGHBOR_ATTR_PACKET_ACTION,
                                         new_pkt_action, NULL);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_creation (port_rif_id, ip_af, p_ip_addr_str,
                                        p_mac_str, new_pkt_action);

    status = sai_test_neighbor_remove (port_rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_removal (port_rif_id, ip_af, p_ip_addr_str);
}

/*
 * Validate that a same Neighbor IP can be created in different VRFs.
 */
TEST_F (saiL3NeighborTest, duplicate_neighbor_ip_in_different_vrf)
{
    sai_status_t          status;
    const char           *p_ip_addr_str = "2001:db8:85a3::8a2e:370:7334";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV6;
    const char           *p_mac_str = "00:a1:a2:a3:a4:a5";
    sai_object_id_t     new_vrf_id;
    sai_object_id_t     new_rif_id;
    const unsigned int  new_port = 2;
    sai_object_id_t     new_port_id = 0;

    /* Create a different Virtual Router instance */
    status = sai_test_vrf_create (&new_vrf_id, 0);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    new_port_id = sai_l3_port_id_get(new_port);

    /* Create a Port RIF on the new VRF */
    status = sai_test_rif_create (&new_rif_id, default_rif_attr_count,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  new_vrf_id,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  new_port_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create another neighbor with same address in the different VRF */
    status = sai_test_neighbor_create (new_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_creation (port_rif_id, ip_af, p_ip_addr_str,
                                        p_mac_str, default_pkt_action);

    status = sai_test_neighbor_remove (port_rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_removal (port_rif_id, ip_af, p_ip_addr_str);

    status = sai_test_neighbor_remove (new_rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_removal (new_rif_id, ip_af, p_ip_addr_str);

    /* Remove the VLAN RIF */
    status = sai_test_rif_remove (new_rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Remove the VRF */
    status = sai_test_vrf_remove (new_vrf_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);
}

/*
 * Create Neighbor for a VLAN Router interface and Destination MAC attribute 
 * set to a MAC address that does not have an entry in FDB table. Validate that
 * Neighbor creation fails with appropriate error status.
 */
TEST_F (saiL3NeighborTest, create_on_vlan_rif_without_fdb_entry)
{
    sai_status_t          status;
    const char           *p_ip_addr_str = "11.0.0.1";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char           *p_mac_str = "00:c1:c2:c3:c4:c5";

    status = sai_test_neighbor_create (vlan_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    EXPECT_EQ (SAI_STATUS_ADDR_NOT_FOUND, status);
}

/*
 * Validate that creating neighbors with same IP address on Router interfaces
 * of same VRF is rejected with appropriate error status.
 */
TEST_F (saiL3NeighborTest, duplicate_neighbor_in_same_vrf)
{
    sai_status_t          status;
    const char           *p_ip_addr_str = "11.0.0.1";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char           *p_mac_str = "00:a1:a2:a3:a4:a5";

    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    EXPECT_EQ (SAI_STATUS_ITEM_ALREADY_EXISTS, status);

    status = sai_test_neighbor_remove (port_rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_removal (port_rif_id, ip_af, p_ip_addr_str);
}

/*
 * Check if neighbor creation fails when router interface field is invalid and 
 * appropriate error status is returned.
 */
TEST_F (saiL3NeighborTest, invalid_rif_id_key_value)
{
    sai_status_t               status;
    sai_ip_addr_family_t       ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char                *p_ip_addr_str = "11.0.0.1";
    sai_object_id_t  invalid_rif_id = 0;
    const char                *p_mac_str = "00:a1:a2:a3:a4:a5";

    status = sai_test_neighbor_create (invalid_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, status);
}

/*
 * Check if neighbor creation fails when ip address family field is invalid and 
 * appropriate error status is returned.
 */
TEST_F (saiL3NeighborTest, invalid_ip_addr_key_value)
{
    sai_status_t         status;
    sai_int32_t          invalid_ip_af = -1;
    const char          *p_mac_str = "00:a1:a2:a3:a4:a5";

    /* Pass invalid IP address family value */
    status = sai_test_neighbor_create (port_rif_id,
                                       (sai_ip_addr_family_t) invalid_ip_af,
                                       "1.0.0.1",
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    EXPECT_NE (SAI_STATUS_SUCCESS, status);

    /* Pass zero IPv4 address */
    status = sai_test_neighbor_create (port_rif_id, SAI_IP_ADDR_FAMILY_IPV4,
                                       "0.0.0.0",
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    EXPECT_NE (SAI_STATUS_SUCCESS, status);

    /* Pass zero IPv6 address */
    status = sai_test_neighbor_create (port_rif_id, SAI_IP_ADDR_FAMILY_IPV6,
                                       "::",
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str);

    EXPECT_NE (SAI_STATUS_SUCCESS, status);
}

/*
 * Check if neighbor creation with invalid MAC address attribute value returns
 * appropriate error status.
 */
TEST_F (saiL3NeighborTest, invalid_mac_addr_attr_value)
{
    sai_status_t          status;
    unsigned int          invalid_attr_index = 0;
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char           *p_ip_addr_str = "11.0.0.1";

    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       "0:0:0:0:0:0");

    EXPECT_EQ ((sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTR_VALUE_0,
                invalid_attr_index)), status);
}

/*
 * Check if neighbor creation with an invalid value set for 
 * SAI_NEIGHBOR_ATTR_PACKET_ACTION attribute returns appropriate error status.
 */
TEST_F (saiL3NeighborTest, invalid_pkt_action_attr_value)
{
    sai_status_t          status;
    unsigned int          invalid_attr_index = 1;
    sai_int32_t           invalid_pkt_action = -1;
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char           *p_ip_addr_str = "11.0.0.1";
    const char           *p_mac_str = "00:a1:a2:a3:a4:a5";

    /* Pass invalid Packet action attr value */
    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       (default_neighbor_attr_count + 1),
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str,
                                       SAI_NEIGHBOR_ATTR_PACKET_ACTION,
                                       invalid_pkt_action);

    EXPECT_EQ ((sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTR_VALUE_0,
                invalid_attr_index)), status);
}

/*
 * Check if neighbor creation with one of the mandatory attributes missing 
 * returns appropriate error status.
 */
TEST_F (saiL3NeighborTest, mandatory_attr_missing)
{
    sai_status_t          status;
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char           *p_ip_addr_str = "11.0.0.1";

    /* Miss the Dest Mac attribute */
    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       default_neighbor_attr_count,
                                       SAI_NEIGHBOR_ATTR_PACKET_ACTION,
                                       SAI_PACKET_ACTION_LOG);

    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, status);
}

/*
 * Check if the neighbor creation with invalid attribute ID returns appropriate
 * error status.
 */
TEST_F (saiL3NeighborTest, invalid_attr_id)
{
    sai_status_t          status;
    const unsigned int    invalid_attr_pos = 1;
    const unsigned int    invalid_attr_id = 0xffff;
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV4;
    const char           *p_ip_addr_str = "11.0.0.1";
    const char           *p_mac_str = "00:a1:a2:a3:a4:a5";

    /* Pass invalid attribute id in list */
    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       (default_neighbor_attr_count + 1),
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str,
                                       invalid_attr_id, 0);

    EXPECT_EQ ((sai_test_invalid_attr_status_code (SAI_STATUS_UNKNOWN_ATTRIBUTE_0,
                invalid_attr_pos)), status);
}

/*
 * Validate getting SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS is successful, even 
 * when the SAI_NEIGHBOR_ATTR_PACKET_ACTION is set to TRAP or DROP.
 */
TEST_F (saiL3NeighborTest, dst_mac_attr_get_for_trap_drop_action)
{
    sai_status_t          status;
    const char           *p_ip_addr_str = "2001:db8:85a3::8a2e:370:7334";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV6;
    const char           *p_mac_str = "00:a1:a2:a3:a4:a5";
    sai_packet_action_t   pkt_action = SAI_PACKET_ACTION_TRAP;
    const unsigned int    attr_count = default_neighbor_attr_count + 1;

    /* Create with trap packet action */
    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str,
                                       SAI_NEIGHBOR_ATTR_PACKET_ACTION,
                                       pkt_action);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Verify dest mac attr value is still returned */
    sai_neighbor_verify_after_creation (port_rif_id, ip_af, p_ip_addr_str,
                                        p_mac_str, pkt_action);

    /* Set the packet action to drop */
    status = sai_test_neighbor_attr_set (port_rif_id, ip_af, p_ip_addr_str,
                                         SAI_NEIGHBOR_ATTR_PACKET_ACTION,
                                         SAI_PACKET_ACTION_DROP, NULL);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Verify dest mac attr value is still returned */
    sai_neighbor_verify_after_creation (port_rif_id, ip_af, p_ip_addr_str,
                                        p_mac_str, SAI_PACKET_ACTION_DROP);

    status = sai_test_neighbor_remove (port_rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_removal (port_rif_id, ip_af, p_ip_addr_str);
}

/*
 * Validate creating neighbor entry with 'no_host_route' attribute set.
 */
TEST_F (saiL3NeighborTest, create_and_remove_with_no_host_route)
{
    sai_status_t          status;
    const char           *p_ip_addr_str = "FE80::C001:1DFF:FEE0:0";
    sai_ip_addr_family_t  ip_af = SAI_IP_ADDR_FAMILY_IPV6;
    const char           *p_mac_str = "00:a1:a2:a3:a4:a5";
    const unsigned int    attr_count = default_neighbor_attr_count + 1;
    sai_attribute_t       attr_list [max_neighbor_attr_count];
    sai_object_id_t       nh_id;

    /* Create IPv6 link-local address neighbor entry with no-host-route attr */
    status = sai_test_neighbor_create (port_rif_id, ip_af, p_ip_addr_str,
                                       attr_count,
                                       SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                       p_mac_str,
                                       SAI_NEIGHBOR_ATTR_NO_HOST_ROUTE,
                                       true);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_creation (port_rif_id, ip_af, p_ip_addr_str,
                                        p_mac_str, default_pkt_action);

    status = sai_test_neighbor_attr_get (port_rif_id, ip_af, p_ip_addr_str,
                                         attr_list, 1,
                                         SAI_NEIGHBOR_ATTR_NO_HOST_ROUTE);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    EXPECT_EQ (attr_list[0].value.booldata, true);

    /* Verify that next hop creation is successful for the IPv6 address */
    status = sai_test_nexthop_create (&nh_id, default_nh_attr_count,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      port_rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_ip_addr_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    printf ("SAI Next Hop Object Id: 0x%"PRIx64".\n", nh_id);

    /* Cleanup the next hop object */
    status = saiL3Test::sai_test_nexthop_remove (nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Remove the neighbor entry */
    status = sai_test_neighbor_remove (port_rif_id, ip_af, p_ip_addr_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_neighbor_verify_after_removal (port_rif_id, ip_af, p_ip_addr_str);
}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest (&argc, argv);

    return RUN_ALL_TESTS ();
}
