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
*    sai_l3_vr_unit_test.cpp
*     
* Abstract:
*
*    SAI VR UNIT TEST :- Covers the test cases for validating all public
*    APIs in SAI VIRTUAL ROUTER module.
*
*************************************************************************/

#include "gtest/gtest.h"

#include "sai_l3_unit_test_utils.h"

extern "C" {
#include "saistatus.h"
#include "saitypes.h"
#include "sairouter.h"
#include "sairouterintf.h"
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
}

class saiL3VrfTest : public saiL3Test
{
    public:
        static void SetUpTestCase ();
        static void TearDownTestCase (void);

        static void sai_test_vrf_attr_verify (sai_object_id_t vr_id,
                                              sai_mac_t mac,
                                              bool v4_state, bool v6_state,
                                              sai_packet_action_t ip_opt_action,
                                              sai_packet_action_t ttl_action);
        static sai_status_t sai_test_vrf_switch_mac_set_and_verify (void);

        static sai_object_id_t test_port_id;
        static sai_object_id_t test_vr_id;
        static sai_object_id_t test_vlan_rif_id;
        static sai_object_id_t test_port_rif_id;

        static const unsigned int test_port = 0;
        static const unsigned int test_vlan_id = 100;

        static const sai_packet_action_t SAI_TEST_IP_OPT_PKT_ACTION_DFLT \
                                               = SAI_PACKET_ACTION_TRAP;
        static const sai_packet_action_t SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT \
                                               = SAI_PACKET_ACTION_TRAP;
};

sai_object_id_t saiL3VrfTest ::test_port_id = 0;
sai_object_id_t saiL3VrfTest ::test_vr_id = 0;
sai_object_id_t saiL3VrfTest ::test_vlan_rif_id = 0;
sai_object_id_t saiL3VrfTest ::test_port_rif_id = 0;

void saiL3VrfTest ::SetUpTestCase (void)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  vr_id_0 = 0;

    /* Base SetUpTestCase for SAI initialization */
    saiL3Test ::SetUpTestCase ();

    /* Switch MAC configuration and related tests, validations */
    sai_rc = sai_test_vrf_switch_mac_set_and_verify ();

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create a VRF ID (non-zero ID) to use for testing set attribute APIs */
    sai_rc = sai_test_vrf_create (&vr_id_0, 0);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_vrf_create (&test_vr_id, 0);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_vrf_remove (vr_id_0);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create a vlan RIF on test_vr_id */
    sai_rc = sai_test_vlan_create (test_vlan_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_rif_create (&test_vlan_rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  test_vr_id,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    test_port_id = sai_l3_port_id_get (test_port);

    /* Create a port RIF on test_vr_id */
    sai_rc = sai_test_rif_create (&test_port_rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  test_vr_id,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

void saiL3VrfTest ::TearDownTestCase (void)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;

    /* Remove vlan RIF */
    sai_rc = sai_test_rif_remove (test_vlan_rif_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove Vlan */
    sai_rc = sai_test_vlan_remove (test_vlan_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove port RIF */
    sai_rc = sai_test_rif_remove (test_port_rif_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove VRF */
    sai_rc = sai_test_vrf_remove (test_vr_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

void saiL3VrfTest ::sai_test_vrf_attr_verify (
sai_object_id_t vr_id, sai_mac_t mac, bool v4_state, bool v6_state,
sai_packet_action_t ip_opt_action, sai_packet_action_t ttl_action)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t attr_list [4];
    unsigned int    attr_count = 4;

    sai_rc =
        sai_test_vrf_attr_get (vr_id, &attr_list [0], attr_count,
                               SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                               SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE,
                               SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE,
                               SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (0, (memcmp (mac, attr_list [0].value.mac, sizeof (sai_mac_t))));
    EXPECT_EQ (v4_state, attr_list [1].value.booldata);
    EXPECT_EQ (v6_state, attr_list [2].value.booldata);
    EXPECT_EQ (ip_opt_action, attr_list [3].value.s32);

    attr_count = 1;

    sai_rc =
        sai_test_vrf_attr_get (vr_id, &attr_list [0], attr_count,
                               SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION);

    if (sai_rc != SAI_STATUS_ATTR_NOT_SUPPORTED_0) {
        EXPECT_EQ (ttl_action, attr_list [0].value.s32);
    }
}

sai_status_t saiL3VrfTest ::sai_test_vrf_switch_mac_set_and_verify (void)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  vr_id = 0;
    const char      *mac_str = "00:01:02:03:04:05";

    printf ("Creating VRF without MAC, when switch MAC is not set.\r\n");
    printf ("Expecting error - SAI_STATUS_ADDR_NOT_FOUND.\r\n");

    sai_rc = sai_test_vrf_create (&vr_id, 0);

    EXPECT_EQ (SAI_STATUS_ADDR_NOT_FOUND, sai_rc);

    printf ("Creating VRF with SRC_MAC %s\r\n", mac_str);
    printf ("Expecting SAI_STATUS_SUCCESS.\r\n");

    sai_rc = sai_test_vrf_create (&vr_id, 1,
                                  SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                  mac_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    printf ("Setting switch MAC to %s\r\n", mac_str);
    printf ("Expecting SAI_STATUS_FAILURE.\r\n");

    sai_rc = sai_test_router_mac_set (mac_str);

    EXPECT_NE (SAI_STATUS_SUCCESS, sai_rc);

    printf ("Removing VRF and then set Switch MAC to %s\r\n", mac_str);
    printf ("Expecting SAI_STATUS_SUCCESS.\r\n");

    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_router_mac_set (mac_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    printf ("Replacing switch MAC to %s\r\n", router_mac);
    printf ("Expecting SAI_STATUS_SUCCESS.\r\n");

    sai_rc = sai_test_router_mac_set (router_mac);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    return sai_rc;
}

/*
 * Validates VRF create and remove.
 */
TEST_F(saiL3VrfTest, vrf_create_and_remove)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t vr_id = 0;
    sai_mac_t       switch_mac;
    sai_attribute_t attr;

    /* Create VRF */
    sai_rc = sai_test_vrf_create (&vr_id, 0);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("VRF created with ID 0x%"PRIx64"\r\n", vr_id);
    }

    sai_rc = sai_test_router_mac_get (&switch_mac);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vrf_attr_verify (vr_id, switch_mac, 
                              saiL3Test::SAI_TEST_V4_ADMIN_STATE_DFLT,
                              saiL3Test::SAI_TEST_V6_ADMIN_STATE_DFLT,
                              SAI_TEST_IP_OPT_PKT_ACTION_DFLT,
                              SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT);

    /* Remove VRF */
    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("VRF ID 0x%"PRIx64" removed.\r\n", vr_id);
    }

    /* Getting VRF attributes must fail */
    sai_rc = sai_test_vrf_attr_get (vr_id, &attr, 1 /* attr_count */,
                                    SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
}

/*
 * Validates creation and removal of multiple VRF.
 */
TEST_F(saiL3VrfTest, vrf_multiple_create_and_remove)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t vr_id [5];
    unsigned int    num_vr = 0;
    sai_attribute_t attr;

    /* Create 5 virtual Routers */
    for (num_vr = 0; num_vr < 5; num_vr++) {
        sai_rc = sai_test_vrf_create (&vr_id [num_vr], 0);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        if (sai_rc == SAI_STATUS_SUCCESS) {
            printf ("VRF created with ID 0x%"PRIx64"\r\n", vr_id [num_vr]);
        }

        /* Getting VRF attributes must be successful */
        sai_rc = sai_test_vrf_attr_get (vr_id [num_vr], &attr, 1 /* count */,
                                        SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    /* Remove the Virtual Routers */
    for (num_vr = 0; num_vr < 5; num_vr++) {
        sai_rc = sai_test_vrf_remove (vr_id [num_vr]);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        if (sai_rc == SAI_STATUS_SUCCESS) {
            printf ("VRF ID 0x%"PRIx64" removed.\r\n", vr_id [num_vr]);
        }

        /* Getting VRF attributes must fail */
        sai_rc = sai_test_vrf_attr_get (vr_id [num_vr], &attr, 1 /* count */,
                                        SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE);

        EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
    }
}

/*
 * Checks if removing an invalid VRF ID returns appropriate error status.
 */
TEST_F(saiL3VrfTest, vrf_remove_invalid_vr_id)
{
    sai_status_t  sai_rc = SAI_STATUS_SUCCESS;
    const int     SAI_TEST_INVALID_VR_ID  = 10000;

    sai_rc = sai_test_vrf_remove (SAI_TEST_INVALID_VR_ID);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, sai_rc);
}

/*
 * Checks if creating VRF with MAC set to all zeros returns appropriate error 
 * status.
 */
TEST_F(saiL3VrfTest, vrf_create_with_mac_as_zero)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t vr_id = 0;

    sai_rc = sai_test_vrf_create (&vr_id, 1,
                                  SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                  "00:00:00:00:00:00");

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

/*
 * checks if creating VRF with an invalid attribute ID returns appropriate 
 * error status.
 */
TEST_F(saiL3VrfTest, vrf_create_with_invalid_attr_id)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t vr_id = 0;
    const unsigned int  invalid_attr_id = 0xff;

    sai_rc = sai_test_vrf_create (&vr_id, 1, invalid_attr_id, 1);

    EXPECT_EQ (SAI_STATUS_UNKNOWN_ATTRIBUTE_0, sai_rc);
}

/*
 * Validates VRF creation with SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION
 * attribute.
 */
TEST_F(saiL3VrfTest, vrf_create_with_attr_ttl1_pkt_action)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t vr_id = 0;
    unsigned int    invalid_pkt_action = 65;

    /* Creating VRF with invalid TTL violation packet action */
    sai_rc = sai_test_vrf_create (&vr_id, 1,
                                  SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION,
                                  invalid_pkt_action);

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);

    sai_rc = sai_test_vrf_create (&vr_id, 1,
                                  SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION,
                                  SAI_PACKET_ACTION_DROP);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}
/*
 * Validates VRF creation with SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS
 * attribute.
 */
TEST_F(saiL3VrfTest, vrf_create_with_attr_ip_options_pkt_action)
{
    sai_status_t            sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t         vr_id = 0;
    sai_mac_t               switch_mac;
    sai_packet_action_t     pkt_action = SAI_PACKET_ACTION_DROP;

    sai_rc = sai_test_vrf_create (&vr_id, 1,
                                  SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS,
                                  pkt_action);

    sai_rc = sai_test_router_mac_get (&switch_mac);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vrf_attr_verify (vr_id, switch_mac, 
                              saiL3Test::SAI_TEST_V4_ADMIN_STATE_DFLT,
                              saiL3Test::SAI_TEST_V6_ADMIN_STATE_DFLT, 
                              pkt_action, 
                              SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT);

    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validates if appropriate error status is returned when attempting to remove 
 * a VRF that has a router interface associated to it.
 */
TEST_F(saiL3VrfTest, vrf_remove_when_rif_exists)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t vr_id = 0;
    sai_object_id_t rif_id = 0;
    unsigned int    port_num = 5;
    sai_object_id_t test_port_id = 0;

    sai_rc = sai_test_vrf_create (&vr_id, 0);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    test_port_id = sai_l3_port_id_get (port_num);

    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* VRF can not be deleted when it has RIF associated. */
    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);

    /* Removing RIF and then removing VRF must be successful. */
    sai_rc = sai_test_rif_remove (rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Checks if calling VRF attribute_set API for a non existing VRF ID returns 
 * appropriate error status.
 */
TEST_F (saiL3VrfTest, vrf_attr_set_on_non_existing_vrf_id)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  vr_id;

    sai_rc = sai_test_vrf_create (&vr_id, 0);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove VR */
    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Verify set attribute API for the VR object id */
    sai_rc =
        sai_test_vrf_attr_set (vr_id, SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE,
                               false, 0);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
}

/*
 * Checks if calling VRF attribute_get API for a non existing VRF ID returns 
 * appropriate error status.
 */
TEST_F (saiL3VrfTest, vrf_attr_get_on_non_existing_vrf_id)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  vr_id;
    sai_attribute_t  attr;

    sai_rc = sai_test_vrf_create (&vr_id, 0);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove VR */
    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Verify get attribute API for the VR object id */
    sai_rc = sai_test_vrf_attr_get (vr_id, &attr, 1 /* attr_count */,
                                    SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
}

/*
 * Validate VRF creation with SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE attribute.
 */
TEST_F(saiL3VrfTest, vrf_create_with_attr_admin_v4)
{
    sai_status_t      sai_rc = SAI_STATUS_SUCCESS;
    sai_mac_t         switch_mac;
    sai_object_id_t   vr_id = 0;
    unsigned int      v4_admin_state = 0;

    sai_rc = sai_test_vrf_create (&vr_id, 1,
                                  SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE,
                                  v4_admin_state);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_router_mac_get (&switch_mac);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vrf_attr_verify (vr_id, switch_mac, v4_admin_state,
                              saiL3Test::SAI_TEST_V6_ADMIN_STATE_DFLT,
                              SAI_TEST_IP_OPT_PKT_ACTION_DFLT,
                              SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT);

    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate VRF creation with SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE attribute.
 */
TEST_F(saiL3VrfTest, vrf_create_with_attr_admin_v6)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_mac_t       switch_mac;
    sai_object_id_t vr_id = 0;
    unsigned int    v6_admin_state = 0;

    sai_rc = sai_test_vrf_create (&vr_id, 1,
                                  SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE,
                                  v6_admin_state);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_router_mac_get (&switch_mac);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vrf_attr_verify (vr_id, switch_mac, 
                              saiL3Test::SAI_TEST_V4_ADMIN_STATE_DFLT,
                              v6_admin_state, SAI_TEST_IP_OPT_PKT_ACTION_DFLT,
                              SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT);

    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate VRF creation with SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE 
 * and SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE attribute.
 */
TEST_F(saiL3VrfTest, vrf_create_with_attr_admin_v4_and_v6)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_mac_t       switch_mac;
    sai_object_id_t vr_id = 0;
    unsigned int    v4_admin_state = 0;
    unsigned int    v6_admin_state = 0;

    sai_rc = sai_test_vrf_create (&vr_id, 2,
                                  SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE,
                                  v4_admin_state,
                                  SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE,
                                  v6_admin_state);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("VRF created with ID 0x%"PRIx64".\r\n", vr_id);
    }

    sai_rc = sai_test_router_mac_get (&switch_mac);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vrf_attr_verify (vr_id, switch_mac, v4_admin_state,
                              v6_admin_state, SAI_TEST_IP_OPT_PKT_ACTION_DFLT,
                              SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT);

    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate VRF creation with SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE and 
 * SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS attribute.
 */
TEST_F(saiL3VrfTest, vrf_create_with_attr_admin_v4_and_mac)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  vr_id = 0;
    unsigned int     v4_admin_state = 0;
    uint8_t          sai_mac [6];
    const char      *mac_str = "00:cc:cc:cc:cc:cc";

    sai_rc = sai_test_vrf_create (&vr_id, 2,
                                  SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE,
                                  v4_admin_state,
                                  SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                  mac_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_router_mac_str_to_bytes_get (mac_str, sai_mac);

    sai_test_vrf_attr_verify (vr_id, sai_mac, v4_admin_state,
                              saiL3Test::SAI_TEST_V6_ADMIN_STATE_DFLT,
                              SAI_TEST_IP_OPT_PKT_ACTION_DFLT,
                              SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT);

    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate VRF creation with SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE and 
 * SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS attribute.
 */
TEST_F(saiL3VrfTest, vrf_create_with_attr_admin_v6_and_mac)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  vr_id = 0;
    unsigned int     v6_admin_state = 0;
    uint8_t          sai_mac [6];
    const char      *mac_str = "00:a0:a1:a2:a3:a4";

    sai_rc = sai_test_vrf_create (&vr_id, 2,
                                  SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE,
                                  v6_admin_state,
                                  SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                  mac_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("VRF created with ID 0x%"PRIx64".\r\n", vr_id);
    }

    sai_test_router_mac_str_to_bytes_get (mac_str, sai_mac);

    sai_test_vrf_attr_verify (vr_id, sai_mac, 
                              saiL3Test::SAI_TEST_V4_ADMIN_STATE_DFLT,
                              v6_admin_state, SAI_TEST_IP_OPT_PKT_ACTION_DFLT,
                              SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT);

    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate VRF creation with SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE, 
 * SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE and 
 * SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS attribute.
 */
TEST_F(saiL3VrfTest, vrf_create_with_attr_admin_v4_v6_and_mac)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  vr_id = 0;
    unsigned int     v4_admin_state = 0;
    unsigned int     v6_admin_state = 0;
    uint8_t          sai_mac [6];
    const char      *mac_str = "00:aa:bb:cc:dd:ee";

    sai_rc = sai_test_vrf_create (&vr_id, 3,
                                  SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE,
                                  v4_admin_state,
                                  SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE,
                                  v6_admin_state,
                                  SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                  mac_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_router_mac_str_to_bytes_get (mac_str, sai_mac);

    sai_test_vrf_attr_verify (vr_id, sai_mac, v4_admin_state, v6_admin_state,
                              SAI_TEST_IP_OPT_PKT_ACTION_DFLT,
                              SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT);

    sai_rc = sai_test_vrf_remove (vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate VRF attribute set with SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE 
 * attribute.
 */
TEST_F(saiL3VrfTest, vrf_attr_set_v4_admin_state)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    sai_mac_t    switch_mac;
    unsigned int v4_admin_state [2] = {0, 1};
    unsigned int trial_id = 0;

    sai_rc = sai_test_router_mac_get (&switch_mac);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    for (trial_id = 0; trial_id < 2; trial_id++) {
    sai_rc = sai_test_vrf_attr_set (test_vr_id,
                                    SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE,
                                    v4_admin_state [trial_id], NULL /* MAC */);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vrf_attr_verify (test_vr_id, switch_mac, v4_admin_state [trial_id],
                              saiL3Test::SAI_TEST_V6_ADMIN_STATE_DFLT,
                              SAI_TEST_IP_OPT_PKT_ACTION_DFLT,
                              SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT);
    }
}

/*
 * Validate VRF attribute set with SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE 
 * attribute.
 */
TEST_F(saiL3VrfTest, vrf_attr_set_v6_admin_state)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    sai_mac_t    switch_mac;
    unsigned int v6_admin_state [2] = {0, 1};
    unsigned int trial_id = 0;

    sai_rc = sai_test_router_mac_get (&switch_mac);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    for (trial_id = 0; trial_id < 2; trial_id++) {
    sai_rc = sai_test_vrf_attr_set (test_vr_id,
                                    SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE,
                                    v6_admin_state [trial_id], NULL /* MAC */);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vrf_attr_verify (test_vr_id, switch_mac,
                              saiL3Test::SAI_TEST_V4_ADMIN_STATE_DFLT,
                              v6_admin_state [trial_id],
                              SAI_TEST_IP_OPT_PKT_ACTION_DFLT,
                              SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT);
    }
}

/*
 * Validate VRF attribute set with SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS 
 * attribute.
 */
TEST_F(saiL3VrfTest, vrf_attr_set_src_mac)
{
    sai_status_t  sai_rc = SAI_STATUS_SUCCESS;
    unsigned int  trial_id = 0;
    uint8_t       sai_mac [6];
    const char   *test_mac_str [3] = {"00:10:11:12:13:14",
        "00:20:21:22:23:24", router_mac};

    for (trial_id = 0; trial_id < 3; trial_id++) {
        sai_rc = sai_test_vrf_attr_set (test_vr_id,
                                        SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                        0, test_mac_str [trial_id]);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_test_router_mac_str_to_bytes_get (test_mac_str [trial_id], sai_mac);

        sai_test_vrf_attr_verify (test_vr_id, sai_mac,
                                  saiL3Test::SAI_TEST_V4_ADMIN_STATE_DFLT,
                                  saiL3Test::SAI_TEST_V6_ADMIN_STATE_DFLT,
                                  SAI_TEST_IP_OPT_PKT_ACTION_DFLT,
                                  SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT);
    }
}

/*
 * Validate VRF attribute set with SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS 
 * attribute.
 */
TEST_F(saiL3VrfTest, vrf_attr_set_ip_options_pkt_action)
{
    sai_status_t        sai_rc = SAI_STATUS_SUCCESS;
    sai_mac_t           switch_mac;
    unsigned int        num_trials = 0;
    unsigned int        trial_id = 0;
    unsigned int        invalid_pkt_action = 65;
    sai_packet_action_t pkt_action [] = {SAI_TEST_IP_OPT_PKT_ACTION_DFLT,
        SAI_PACKET_ACTION_DROP, SAI_PACKET_ACTION_FORWARD,
        SAI_PACKET_ACTION_TRAP, SAI_PACKET_ACTION_LOG,
        SAI_TEST_IP_OPT_PKT_ACTION_DFLT};

    sai_rc = sai_test_router_mac_get (&switch_mac);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Try setting Different IP Options pkt actions attribute and verify */
    num_trials = sizeof (pkt_action)/ sizeof (sai_packet_action_t);

    for (trial_id = 0; trial_id < num_trials; trial_id++) {
        sai_rc =
            sai_test_vrf_attr_set (test_vr_id,
                                   SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS,
                                   pkt_action [trial_id], NULL /* MAC */);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vrf_attr_verify (test_vr_id, switch_mac,
                              saiL3Test::SAI_TEST_V4_ADMIN_STATE_DFLT,
                              saiL3Test::SAI_TEST_V6_ADMIN_STATE_DFLT,
                              pkt_action [trial_id],
                              SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT);
    }

    /* Set an invalid packet action attribute value */
    sai_rc =
        sai_test_vrf_attr_set (test_vr_id,
                               SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS,
                               invalid_pkt_action, NULL /* MAC */);

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

/*
 * Validate VRF attribute set with SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION
 * attribute.
 */
TEST_F(saiL3VrfTest, vrf_attr_set_ttl_violation_pkt_action)
{
    sai_status_t        sai_rc = SAI_STATUS_SUCCESS;
    sai_mac_t           switch_mac;
    unsigned int        num_trials = 0;
    unsigned int        trial_id = 0;
    unsigned int        invalid_pkt_action = 65;
    sai_packet_action_t pkt_action [] = {SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT,
        SAI_PACKET_ACTION_DROP, SAI_PACKET_ACTION_FORWARD,
        SAI_PACKET_ACTION_TRAP, SAI_PACKET_ACTION_LOG,
        SAI_TEST_TTL_VIOLATION_PKT_ACTION_DFLT};

    sai_rc = sai_test_router_mac_get (&switch_mac);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Try setting Different TTL violation pkt actions attribute and verify */
    num_trials = sizeof (pkt_action)/ sizeof (sai_packet_action_t);

    for (trial_id = 0; trial_id < num_trials; trial_id++) {
        sai_rc = sai_test_vrf_attr_set (test_vr_id,
                                 SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION,
                                 pkt_action [trial_id], NULL /* MAC */);

        if (sai_rc != SAI_STATUS_ATTR_NOT_SUPPORTED_0) {
            sai_test_vrf_attr_verify (test_vr_id, switch_mac,
                                      saiL3Test::SAI_TEST_V4_ADMIN_STATE_DFLT,
                                      saiL3Test::SAI_TEST_V6_ADMIN_STATE_DFLT,
                                      SAI_TEST_IP_OPT_PKT_ACTION_DFLT,
                                      pkt_action [trial_id]);
        }
    }

    /* Set an invalid packet action attribute value */
    sai_rc =
        sai_test_vrf_attr_set (test_vr_id,
                               SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_ACTION,
                               invalid_pkt_action, NULL /* MAC */);

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

/*
 * Validate switch attribute set and get APIs for 
 * SAI_SWITCH_ATTR_ECMP_MAX_PATHS attribute.
 */
TEST_F(saiL3VrfTest, max_ecmp_paths_switch_attr_set)
{
    sai_status_t        status;
    sai_attribute_t     attr;
    const unsigned int  max_paths = 32;

    memset (&attr, 0, sizeof (sai_attribute_t));

    /* Set the Max ECMP Paths value */
    attr.id        = SAI_SWITCH_ATTR_ECMP_MEMBERS;
    attr.value.u32 = max_paths;

    status  = saiL3Test::switch_api_tbl_get()->set_switch_attribute (
                                               (const sai_attribute_t *)&attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Verify the Max ECMP Paths attribute */
    memset (&attr, 0, sizeof (sai_attribute_t));

    status  = saiL3Test::switch_api_tbl_get()->get_switch_attribute (1, &attr);

    ASSERT_NE (attr.value.u32, max_paths);
}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
