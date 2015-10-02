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
*    sai_l3_rif_unit_test.cpp
*     
* Abstract:
*
*    SAI ROUTER INTERFACE UNIT TEST :- Covers the test cases for validating 
*    all public APIs in SAI ROUTER INTERFACE module.
*
*************************************************************************/

#include "gtest/gtest.h"

#include "sai_l3_unit_test_utils.h"

extern "C" {
#include "saistatus.h"
#include "saitypes.h"
#include "sairouter.h"
#include "sairouterintf.h"
#include "sailag.h"
#include "sai.h"
#include <string.h>
}

class saiL3RifTest : public saiL3Test {
    public:
        static void SetUpTestCase (void);
        static void TearDownTestCase (void);

        static void sai_test_rif_get_vrf_attributes (
                    sai_object_id_t vrf, sai_mac_t src_mac,
                    bool *p_v4_admin_state, bool *p_v6_admin_state);
        static void sai_test_rif_mandatory_attr_verify (
                    sai_object_id_t rif, sai_object_id_t vrf,
                    unsigned int rif_type, uint64_t rif_attach_id);
        static void sai_test_rif_optional_attr_verify (
                    sai_object_id_t rif, sai_mac_t src_mac,
                    bool v4_admin_state, bool v6_admin_state);
        static void sai_test_rif_mtu_attr_verify (sai_object_id_t rif,
                                                  uint32_t mtu);

        static const unsigned int test_port = 0;
        static const unsigned int test_vlan_id = 1000;
        static const unsigned int test_port_2 = 1;
        static const unsigned int test_vlan_id_2 = 2000;

        static const unsigned int SAI_TEST_MTU_DFLT = 1514;

        static sai_object_id_t test_port_id;
        static sai_object_id_t test_port_id_2;
        static sai_object_id_t vr_id_dflt;
        static sai_object_id_t vr_id_with_mac_attr;
        static sai_object_id_t vr_id_with_all_attr;
        static sai_object_id_t test_port_rif;
        static sai_object_id_t test_vlan_rif;
        static sai_lag_api_t*  sai_lag_api_table;
        static sai_vlan_api_t* sai_vlan_api_table;
};

sai_object_id_t saiL3RifTest ::test_port_id = 0;
sai_object_id_t saiL3RifTest ::test_port_id_2 = 0;
sai_object_id_t   saiL3RifTest ::vr_id_dflt = 0;
sai_object_id_t   saiL3RifTest ::vr_id_with_mac_attr = 0;
sai_object_id_t   saiL3RifTest ::vr_id_with_all_attr = 0;
sai_object_id_t saiL3RifTest ::test_port_rif = 0;
sai_object_id_t saiL3RifTest ::test_vlan_rif = 0;
sai_lag_api_t* saiL3RifTest ::sai_lag_api_table = NULL;
sai_vlan_api_t* saiL3RifTest ::sai_vlan_api_table = NULL;

void saiL3RifTest ::SetUpTestCase (void)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;

    /* Base SetUpTestCase for SAI initialization */
    saiL3Test ::SetUpTestCase ();

    test_port_id   = sai_l3_port_id_get (test_port);
    test_port_id_2 = sai_l3_port_id_get (test_port_2);
    /*Remove port from default VLAN before testing rif*/

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_VLAN, (static_cast<void**>
                              (static_cast<void*> (&sai_vlan_api_table)))));

    sai_rc = sai_test_router_mac_init (router_mac);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create Vlan */
    sai_rc = sai_test_vlan_create (test_vlan_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create VRF with no optional attribute settings. */
    sai_rc = sai_test_vrf_create (&vr_id_dflt, 0);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create VRF with SRC_MAC attribute set. */
    sai_rc = sai_test_vrf_create (&vr_id_with_mac_attr, 1,
                                  SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                  "00:b0:b1:b2:b3:b4:b5");

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create VRF with SRC_MAC, V4 and V6 admin state attributes set. */
    sai_rc = sai_test_vrf_create (&vr_id_with_all_attr, 3,
                                  SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE, 0,
                                  SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE, 0,
                                  SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                  "00:c0:c1:c2:c3:c4:c5");

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create a vlan RIF on vr_id_dflt to be used in SET attribute APIs */
    sai_rc = sai_test_vlan_create (test_vlan_id_2);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_rif_create (&test_vlan_rif, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id_2);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create a port RIF on vr_id_dflt to be used in SET attribute APIs */
    sai_rc = sai_test_rif_create (&test_port_rif, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id_2);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_LAG, (static_cast<void**>
                              (static_cast<void*> (&sai_lag_api_table)))));

    EXPECT_TRUE (sai_lag_api_table != NULL);
}

void saiL3RifTest ::TearDownTestCase (void)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;

    /* Remove all the RIF IDs created on the test case setup */
    sai_rc = sai_test_rif_remove (test_vlan_rif);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_rif_remove (test_port_rif);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove all the VRF IDs created on the test case setup */
    sai_rc = sai_test_vrf_remove (vr_id_dflt);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_vrf_remove (vr_id_with_mac_attr);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_vrf_remove (vr_id_with_all_attr);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove all the VLAN IDs created on the test case setup */
    sai_rc = sai_test_vlan_remove (test_vlan_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_vlan_remove (test_vlan_id_2);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Helper function to get the VRF attributes.
 */
void saiL3RifTest ::sai_test_rif_get_vrf_attributes (
sai_object_id_t vrf, sai_mac_t mac, bool *p_v4_state, bool *p_v6_state)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t attr_list [3];

    sai_rc = sai_test_vrf_attr_get (vrf, &attr_list [0], 3 /* attr_count */,
                                    SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                    SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE,
                                    SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    memcpy (mac, attr_list [0].value.mac, sizeof (sai_mac_t));

    *p_v4_state = attr_list [1].value.booldata;
    *p_v6_state = attr_list [2].value.booldata;
}

/*
 * Helper function to verify the Mandatory RIF attributes.
 */
void saiL3RifTest ::sai_test_rif_mandatory_attr_verify (
sai_object_id_t rif_id, sai_object_id_t vr_id,
unsigned int rif_type, uint64_t attach_id)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    unsigned int    attach_attr_id =  0;
    sai_attribute_t attr_list [3];
    unsigned int    attr_count = 3;

    if (rif_type == SAI_ROUTER_INTERFACE_TYPE_PORT) {
        attach_attr_id = SAI_ROUTER_INTERFACE_ATTR_PORT_ID;
    } else if (rif_type == SAI_ROUTER_INTERFACE_TYPE_VLAN) {
        attach_attr_id = SAI_ROUTER_INTERFACE_ATTR_VLAN_ID;
    }

    sai_rc = sai_test_rif_attr_get (rif_id, &attr_list [0], attr_count,
                                    SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                    SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                    attach_attr_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (vr_id, attr_list [0].value.oid);
    EXPECT_EQ (rif_type, attr_list [1].value.s32);

    if (rif_type == SAI_ROUTER_INTERFACE_TYPE_PORT) {
        EXPECT_EQ (attach_id, attr_list [2].value.oid);
    } else if (rif_type == SAI_ROUTER_INTERFACE_TYPE_VLAN) {
        EXPECT_EQ (attach_id, attr_list [2].value.u16);
    }
}

/*
 * Helper function to verify the optional RIF attributes.
 */
void saiL3RifTest ::sai_test_rif_optional_attr_verify (
sai_object_id_t rif_id, sai_mac_t mac, bool v4_state, bool v6_state)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t attr_list [3];
    unsigned int    attr_count = 3;

    sai_rc = sai_test_rif_attr_get (rif_id, &attr_list [0], attr_count,
                                    SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                                    SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE,
                                    SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (0, (memcmp (mac, attr_list [0].value.mac, sizeof (sai_mac_t))));
    EXPECT_EQ (v4_state, attr_list [1].value.booldata);
    EXPECT_EQ (v6_state, attr_list [2].value.booldata);
}

/*
 * Helper function to verify RIF MTU attribute.
 */
void saiL3RifTest ::sai_test_rif_mtu_attr_verify (sai_object_id_t rif_id,
                                                  uint32_t mtu)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t mtu_attr;
    unsigned int    attr_count = 1;

    sai_rc = sai_test_rif_attr_get (rif_id, &mtu_attr, attr_count,
                                    SAI_ROUTER_INTERFACE_ATTR_MTU);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    EXPECT_EQ (mtu, mtu_attr.value.u32);
}

/*
 * Validate Router Interface creation and removal for a port interface.
 * Port RIF created without any optional attributes.
 * All optional attributes will be inherited by RIF from VRF.
 */
TEST_F (saiL3RifTest, rif_port_create_and_remove)
{
    sai_status_t              sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t           rif_id = 0;
    sai_mac_t                 vrf_mac;
    bool                      vrf_v4_state = false;
    bool                      vrf_v6_state = false;
    sai_attribute_t           attr;

    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_mac_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Get the VRF attributes */
    sai_test_rif_get_vrf_attributes (vr_id_with_mac_attr, vrf_mac,
                                     &vrf_v4_state, &vrf_v6_state);

    sai_test_rif_mandatory_attr_verify (rif_id, vr_id_with_mac_attr,
                                        SAI_ROUTER_INTERFACE_TYPE_PORT,
                                        test_port_id);

    sai_test_rif_optional_attr_verify (rif_id, vrf_mac, vrf_v4_state,
                                       vrf_v6_state);

    /*
     * Duplicate RIF create API call will return
     * SAI_STATUS_ITEM_ALREADY_EXISTS.
     */
    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_mac_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id);

    EXPECT_EQ (SAI_STATUS_ITEM_ALREADY_EXISTS, sai_rc);

    /* Remove the port RIF. */
    sai_rc = sai_test_rif_remove (rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Getting any attribute for the removed port RIF must fail now */
    sai_rc = sai_test_rif_attr_get (rif_id, &attr, 1 /* attr_count */,
                                    SAI_ROUTER_INTERFACE_ATTR_TYPE);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Duplicate RIF remove API call must return SAI_STATUS_INVALID_OBJECT_ID */
    sai_rc = sai_test_rif_remove (rif_id);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
}

/*
 * Validate Router Interface creation and removal for a VLAN interface.
 * VLAN RIF created without any optional attributes.
 * All optional attributes will be inherited by RIF from VRF.
 */
TEST_F (saiL3RifTest, rif_vlan_create_and_remove)
{
    sai_status_t              sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t           rif_id = 0;
    sai_mac_t                 vrf_mac;
    bool                      vrf_v4_state = false;
    bool                      vrf_v6_state = false;
    sai_attribute_t           attr;

    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_mac_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Get the VRF attributes */
    sai_test_rif_get_vrf_attributes (vr_id_with_mac_attr, vrf_mac,
                                     &vrf_v4_state, &vrf_v6_state);

    sai_test_rif_mandatory_attr_verify (rif_id, vr_id_with_mac_attr,
                                        SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                        test_vlan_id);

    sai_test_rif_optional_attr_verify (rif_id, vrf_mac, vrf_v4_state,
                                       vrf_v6_state);

    /*
     * Duplicate RIF create API call will return
     * SAI_STATUS_ITEM_ALREADY_EXISTS.
     */
    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_mac_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id);

    EXPECT_EQ (SAI_STATUS_ITEM_ALREADY_EXISTS, sai_rc);

    /* Remove the VLAN RIF. */
    sai_rc = sai_test_rif_remove (rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Getting any attribute for the removed vlan RIF must fail now */
    sai_rc = sai_test_rif_attr_get (rif_id, &attr, 1 /* attr_count */,
                                    SAI_ROUTER_INTERFACE_ATTR_TYPE);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Duplicate RIF remove API call must return SAI_STATUS_INVALID_OBJECT_ID */
    sai_rc = sai_test_rif_remove (rif_id);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
}

/*
 * PORT RIF, VLAN RIF created with MAC attribute on a VRF with SRC MAC set.
 * RIF V4, V6 admin state values from VRF.
 * SRC MAC attribute value from RIF attribute input.
 */
TEST_F (saiL3RifTest, rif_create_with_mac_attr_on_vrf_with_mac_attr)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    unsigned int     idx = 0;
    uint64_t         intf_id [2] = {test_port_id, test_vlan_id};
    unsigned int     intf_type [2] = {SAI_ROUTER_INTERFACE_TYPE_PORT,
                                      SAI_ROUTER_INTERFACE_TYPE_VLAN};
    sai_object_id_t  rif_id [2];
    sai_mac_t        vrf_mac;
    bool             vrf_v4_state = false;
    bool             vrf_v6_state = false;
    uint8_t          sai_mac [6];
    const char       *mac_str = "00:50:51:52:53:54";

    /* Create a port RIF */
    sai_rc = sai_test_rif_create (&rif_id [0], 4,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_mac_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id,
                                  SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                                  mac_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create a VLAN RIF */
    sai_rc = sai_test_rif_create (&rif_id [1], 4,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_mac_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id,
                                  SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                                  mac_str);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Get the VRF attributes */
    sai_test_rif_get_vrf_attributes (vr_id_with_mac_attr, vrf_mac,
                                     &vrf_v4_state, &vrf_v6_state);

    for (idx = 0; idx < 2; idx++) {
        /* Verify RIF attributes */
        sai_test_rif_mandatory_attr_verify (rif_id [idx],
                                            vr_id_with_mac_attr,
                                            intf_type [idx], intf_id [idx]);

        sai_test_router_mac_str_to_bytes_get (mac_str, sai_mac);

        sai_test_rif_optional_attr_verify (rif_id [idx], sai_mac, vrf_v4_state,
                                           vrf_v6_state);

        /* Remove the VLAN RIF. */
        sai_rc = sai_test_rif_remove (rif_id [idx]);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }
}

/*
 * PORT RIF, VLAN RIF created without any optional paramters on VRF with
 * SRC MAC, V4 and V6 admin state set.
 *
 * SRC MAC, V4 and V6 admin state attributes inherited by these RIF's from VRF.
 */
TEST_F (saiL3RifTest, create_rif_without_attr_on_vrf_with_all_attr)
{
    sai_status_t              sai_rc = SAI_STATUS_SUCCESS;
    unsigned int              idx = 0;
    uint64_t                  intf_id [2] = {test_port_id, test_vlan_id};
    unsigned int              intf_type [2] = {SAI_ROUTER_INTERFACE_TYPE_PORT,
                                               SAI_ROUTER_INTERFACE_TYPE_VLAN};
    sai_object_id_t           rif_id [2];
    sai_mac_t                 vrf_mac;
    bool                      vrf_v4_state = false;
    bool                      vrf_v6_state = false;

    /* Create a port RIF */
    sai_rc = sai_test_rif_create (&rif_id [0], 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_all_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create a VLAN RIF */
    sai_rc = sai_test_rif_create (&rif_id [1], 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_all_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Get the VRF attributes */
    sai_test_rif_get_vrf_attributes (vr_id_with_all_attr, vrf_mac,
                                     &vrf_v4_state, &vrf_v6_state);

    for (idx = 0; idx < 2; idx++) {
        /* Verify RIF attributes */
        sai_test_rif_mandatory_attr_verify (rif_id [idx],
                                            vr_id_with_all_attr,
                                            intf_type [idx], intf_id [idx]);

        sai_test_rif_optional_attr_verify (rif_id [idx], vrf_mac, vrf_v4_state,
                                           vrf_v6_state);

        /* Remove the VLAN RIF. */
        sai_rc = sai_test_rif_remove (rif_id [idx]);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }
}

/*
 * PORT RIF, VLAN RIF with all optional attributes set, are created on
 * VRF with SRC MAC, V4 and V6 admin state set.
 *
 * SRC MAC, V4 and V6 admin state attributes set on these RIF's create API call
 * will be taking effect.
 */
TEST_F (saiL3RifTest, create_rif_with_all_attr_on_vrf_with_all_attr)
{
    sai_status_t               sai_rc = SAI_STATUS_SUCCESS;
    unsigned int               idx = 0;
    sai_object_id_t            rif_id [2];
    uint64_t                   intf_id [2] = {test_port_id, test_vlan_id};
    unsigned int               intf_type [2] = {SAI_ROUTER_INTERFACE_TYPE_PORT,
                                                SAI_ROUTER_INTERFACE_TYPE_VLAN};
    sai_mac_t                  vrf_mac;
    bool                       vrf_v4_state = false;
    bool                       vrf_v6_state = false;
    uint8_t                    sai_mac [6];
    unsigned int               v4_admin_state = 1;
    unsigned int               v6_admin_state = 1;
    const char                *mac_str = "00:60:61:62:63:64";

    /* Create a port RIF */
    sai_rc = sai_test_rif_create (&rif_id [0], 6,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_all_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id,
                                  SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                                  mac_str,
                                  SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE,
                                  v4_admin_state,
                                  SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE,
                                  v6_admin_state);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create a VLAN RIF */
    sai_rc = sai_test_rif_create (&rif_id [1], 6,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_all_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id,
                                  SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                                  mac_str,
                                  SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE,
                                  v4_admin_state,
                                  SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE,
                                  v6_admin_state);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Get the VRF attributes */
    sai_test_rif_get_vrf_attributes (vr_id_with_all_attr, vrf_mac,
                                     &vrf_v4_state, &vrf_v6_state);

    for (idx = 0; idx < 2; idx++) {
        sai_test_rif_mandatory_attr_verify (rif_id [idx],
                                            vr_id_with_all_attr,
                                            intf_type [idx], intf_id [idx]);

        sai_test_router_mac_str_to_bytes_get (mac_str, sai_mac);

        sai_test_rif_optional_attr_verify (rif_id [idx], sai_mac,
                                           v4_admin_state, v6_admin_state);

        /* Remove the VLAN RIF. */
        sai_rc = sai_test_rif_remove (rif_id [idx]);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }
}

/*
 * Validate attribute_set API for all attributes that are valid for SET, on a 
 * port ROUTER INTERFACE.
 */
TEST_F (saiL3RifTest, port_rif_optional_attr_set)
{
    sai_status_t  sai_rc = SAI_STATUS_SUCCESS;
    sai_mac_t     sai_mac;
    unsigned int  expected_mtu = SAI_TEST_MTU_DFLT;
    unsigned int  idx = 0;
    bool          v4_state [3] = {false, true, saiL3Test::SAI_TEST_V4_ADMIN_STATE_DFLT};
    bool          v6_state [3] = {false, true, saiL3Test::SAI_TEST_V4_ADMIN_STATE_DFLT};
    unsigned int  mtu [3] = {5000, 18000, SAI_TEST_MTU_DFLT};
    const char   *mac [3] = {"00:30:31:32:33:34", "00:40:41:42:43:44",
                             router_mac};

    /* Modify RIF attributes using SET API and verify using GET API */
    for (idx = 0; idx < 3; idx++) {
        sai_rc =
            sai_test_rif_attr_set (test_port_rif,
                                   SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE,
                                   v4_state [idx], 0);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_rc =
            sai_test_rif_attr_set (test_port_rif,
                                   SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE,
                                   v6_state [idx], 0);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_rc =
            sai_test_rif_attr_set (test_port_rif,
                                   SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                                   0, mac [idx]);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_test_router_mac_str_to_bytes_get (mac [idx], sai_mac);

        sai_test_rif_optional_attr_verify (test_port_rif, sai_mac,
                                           v4_state [idx], v6_state [idx]);

        sai_rc =
            sai_test_rif_attr_set (test_port_rif, SAI_ROUTER_INTERFACE_ATTR_MTU,
                                   mtu [idx], 0);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        if (sai_rc == SAI_STATUS_SUCCESS) {
            expected_mtu = mtu [idx];

            sai_test_rif_mtu_attr_verify (test_port_rif, expected_mtu);
        }
    }
}

/*
 * Validate attribute_set API for all attributes that are valid for SET, on a 
 * VLAN ROUTER INTERFACE.
 */
TEST_F (saiL3RifTest, vlan_rif_optional_attr_set)
{
    sai_status_t  sai_rc = SAI_STATUS_SUCCESS;
    sai_mac_t     sai_mac;
    unsigned int  expected_mtu = SAI_TEST_MTU_DFLT;
    unsigned int  idx = 0;
    bool          v4_state [3] = {false, true, saiL3Test::SAI_TEST_V4_ADMIN_STATE_DFLT};
    bool          v6_state [3] = {false, true, saiL3Test::SAI_TEST_V4_ADMIN_STATE_DFLT};
    unsigned int  mtu [3] = {5000, 9000, SAI_TEST_MTU_DFLT};
    const char   *mac [3] = {"00:30:31:32:33:34", "00:40:41:42:43:44",
                             router_mac};

    /* Modify RIF attributes using SET API and verify using GET API */
    for (idx = 0; idx < 3; idx++) {
        sai_rc =
            sai_test_rif_attr_set (test_vlan_rif,
                                   SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE,
                                   v4_state [idx], 0);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_rc =
            sai_test_rif_attr_set (test_vlan_rif,
                                   SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE,
                                   v6_state [idx], 0);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_rc =
            sai_test_rif_attr_set (test_vlan_rif,
                                   SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                                   0, mac [idx]);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_test_router_mac_str_to_bytes_get (mac [idx], sai_mac);

        sai_test_rif_optional_attr_verify (test_vlan_rif, sai_mac,
                                           v4_state [idx], v6_state [idx]);

        sai_rc =
            sai_test_rif_attr_set (test_vlan_rif, SAI_ROUTER_INTERFACE_ATTR_MTU,
                                   mtu [idx], 0);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        if (sai_rc == SAI_STATUS_SUCCESS) {
            expected_mtu = mtu [idx];

            sai_test_rif_mtu_attr_verify (test_vlan_rif, expected_mtu);
        }
    }
}

/*
 * Check if Creating Router interface on a non existing VRF returns appropriate
 * error status.
 */
TEST_F (saiL3RifTest, rif_create_with_non_existing_vr_id)
{
    sai_status_t      sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t   rif_id = 0;

    /* VRF 100 is in valid range but not created yet. */
    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  100,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id);

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

/*
 * Check if Creating Router interface on an invalid VRF returns appropriate
 * error status.
 */
TEST_F (saiL3RifTest, rif_create_with_invalid_vr_id)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t rif_id = 0;
    sai_object_id_t vr_id = 0;

    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id, SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id);

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

/*
 * Checks if creating Router Interface with MAC as all zeros returns 
 * appropriate error status.
 */
TEST_F (saiL3RifTest, rif_create_with_zero_mac)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t rif_id = 0;

    sai_rc = sai_test_rif_create (&rif_id, 4,
                                  SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                                  "00:00:00:00:00:00",
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id);

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

/*
 * Checks if Router interface creation with invalid port ID returns 
 * appropriate error status.
 */
TEST_F (saiL3RifTest, rif_create_with_invalid_port_id)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  rif_id = 0;
    unsigned int     invalid_port = 0;

    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  invalid_port,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt, SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT);

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

/*
 * Checks if Router interface creation with invalid VLAN ID returns 
 * appropriate error status.
 */
TEST_F (saiL3RifTest, rif_create_with_invalid_vlan_id)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t rif_id = 0;
    unsigned int    invalid_vlan = 5000;

    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  invalid_vlan,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt, SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN);

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

/*
 * Checks if Router interface creation with non-existing VLAN ID returns 
 * appropriate error status.
 */
TEST_F (saiL3RifTest, rif_create_with_non_existing_vlan_id)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t rif_id = 0;
    unsigned int    vlan_id = 500;

    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID, vlan_id,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt, SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN);

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

/*
 * Checks if Router Interface creation with Invalid value for
 * SAI_ROUTER_INTERFACE_ATTR_TYPE attribute returns appropriate error status.
 */
TEST_F (saiL3RifTest, rif_create_with_invalid_type_attr)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t rif_id = 0;
    unsigned int    invalid_type = 0xff;

    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE, invalid_type,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id);

    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

/*
 * Checks if the Router Interface creation with an Invalid attribute ID 
 * returns appropriate error status.
 */
TEST_F (saiL3RifTest, rif_create_with_invalid_attr_id)
{
    sai_status_t              sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t           rif_id = 0;
    unsigned int              invalid_attr = 0xff;
    unsigned int              invalid_attr_list_idx = 3;

    sai_rc = sai_test_rif_create (&rif_id, 4,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id, invalid_attr, 1);

    EXPECT_EQ ((sai_test_invalid_attr_status_code
                (SAI_STATUS_UNKNOWN_ATTRIBUTE_0, invalid_attr_list_idx)),
               sai_rc);
}

/*
 * Checks if Router Interface creation with one of the mandatory attributes 
 * missing returns appropriate error status.
 */
TEST_F (saiL3RifTest, rif_create_with_mandatory_attr_missing)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t rif_id = 0;

    /* VRF attribute missing */
    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id,
                                  SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE, 1);

    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_rc);

    /* TYPE attribute missing */
    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id,
                                  SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE, 1);

    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_rc);

    /* VLAN_ID attribute missing */
    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE, 1);

    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_rc);

    /* PORT_ID attribute missing */
    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE, 1);

    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_rc);
}

/*
 * Checks if the Router Interface creation with TYPE attribute set to VLAN, 
 * but PORT_ID attribute is set instead of VLAN_ID attribute or vice-versa, 
 * returns appropriate error status.
 */
TEST_F (saiL3RifTest, rif_create_with_type_mismatch)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t rif_id = 0;

    /*
     * Create RIF with TYPE attribute set to VLAN,
     * but PORT_ID attribute is set instead of VLAN_ID attribute.
     */
    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id);

    EXPECT_EQ ((sai_test_invalid_attr_status_code
                (SAI_STATUS_INVALID_ATTRIBUTE_0, 2)), sai_rc);

    /*
     * Create RIF with TYPE attribute set to PORT,
     * but VLAN_ID attribute is set instead of PORT_ID attribute.
     */
    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_dflt,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                  test_vlan_id);

    EXPECT_EQ ((sai_test_invalid_attr_status_code
                (SAI_STATUS_INVALID_ATTRIBUTE_0, 2)), sai_rc);
}

/*
 * Checks if calling Route Interface attribute_set API to set the 
 * CREATE_ONLY attributes returns appropriate error status.
 */
TEST_F (saiL3RifTest, rif_mandatory_attr_set)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    unsigned int idx = 0;
    unsigned int num_idx = 0;
    unsigned int attr_id_list [] = {
                 SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                 SAI_ROUTER_INTERFACE_ATTR_TYPE,
                 SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                 SAI_ROUTER_INTERFACE_ATTR_VLAN_ID};
    unsigned int attr_val_list [] = {1, 1, 1, 1};

    num_idx = sizeof (attr_id_list)/sizeof (sai_attr_id_t);

    for (idx = 0; idx < num_idx; idx++) {
        sai_rc = sai_test_rif_attr_set (test_vlan_rif, attr_id_list [idx],
                                        attr_val_list [idx], 0 /* MAC */);

        EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);
    }
}

/*
 * Checks if Router interface attribute_set API called with non existing RIF
 * ID returns appropriate error status.
 */
TEST_F (saiL3RifTest, rif_attr_set_on_non_existing_rif_id)
{
    sai_status_t      sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t   rif_id;

    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_mac_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the RIF. */
    sai_rc = sai_test_rif_remove (rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Verify set attribute API for the RIF object id */
    sai_rc =
        sai_test_rif_attr_set (rif_id, SAI_ROUTER_INTERFACE_ATTR_MTU, 4096, 0);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
}

/*
 * Checks if getting SAI_ROUTER_INTERFACE_ATTR_PORT_ID for a VLAN router 
 * interface returns appropriate error status.
 */
TEST_F (saiL3RifTest, vlan_rif_port_id_attr_get)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t port_id_attr;

    sai_rc = sai_test_rif_attr_get (test_vlan_rif, &port_id_attr, 1 /* count */,
                                    SAI_ROUTER_INTERFACE_ATTR_PORT_ID);

    ASSERT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);
}

/*
 * Checks if getting SAI_ROUTER_INTERFACE_ATTR_VLAN_ID for a PORT router 
 * interface returns appropriate error status.
 */
TEST_F (saiL3RifTest, port_rif_vlan_id_attr_get)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t vlan_id_attr;

    sai_rc = sai_test_rif_attr_get (test_port_rif, &vlan_id_attr, 1 /* count */,
                                    SAI_ROUTER_INTERFACE_ATTR_VLAN_ID);

    ASSERT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);
}

/*
 * Checks if Router Interface attribute_get API called with a non existing
 * router interface ID returns appropriate error status.
 */
TEST_F (saiL3RifTest, rif_attr_get_on_non_existing_rif_id)
{
    sai_status_t      sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t   attr;
    sai_object_id_t   rif_id;

    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_mac_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  test_port_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the RIF. */
    sai_rc = sai_test_rif_remove (rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Verify get attribute API for the RIF object id */
    sai_rc = sai_test_rif_attr_get (rif_id, &attr, 1 /* attr_count */,
                                    SAI_ROUTER_INTERFACE_ATTR_TYPE);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
}

/*
 * Validate RIF creation on a LAG object having zero port members.
 */
TEST_F (saiL3RifTest, create_rif_on_zero_member_lag)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t rif_id;
    sai_mac_t       vrf_mac;
    bool            vrf_v4_state = false;
    bool            vrf_v6_state = false;
    sai_object_id_t lag_id;

    ASSERT_TRUE (sai_lag_api_table != NULL);

    /* Create the LAG object */
    sai_rc = sai_lag_api_table->create_lag(&lag_id, 0, NULL);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create RIF with LAG object id */
    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_mac_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  lag_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Get the VRF attributes */
    sai_test_rif_get_vrf_attributes (vr_id_with_mac_attr, vrf_mac,
                                     &vrf_v4_state, &vrf_v6_state);

    sai_test_rif_mandatory_attr_verify (rif_id, vr_id_with_mac_attr,
                                        SAI_ROUTER_INTERFACE_TYPE_PORT,
                                        lag_id);

    sai_test_rif_optional_attr_verify (rif_id, vrf_mac, vrf_v4_state,
                                       vrf_v6_state);

    /* Remove the port RIF. */
    sai_rc = sai_test_rif_remove (rif_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the LAG */
    sai_rc = sai_lag_api_table->remove_lag (lag_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate RIF creation on a LAG object having valid port members.
 */
TEST_F (saiL3RifTest, create_rif_on_active_lag)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t rif_id;
    sai_mac_t       vrf_mac;
    bool            vrf_v4_state = false;
    bool            vrf_v6_state = false;
    const unsigned int member_count = 2;
    sai_object_id_t lag_id;
    sai_object_id_t port_arr[member_count];
    sai_object_list_t lag_port_list;
    sai_attribute_t attr;
    sai_vlan_port_t  vlan_port[member_count];

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
               saiL3Test::SAI_TEST_DEFAULT_VLAN, member_count, 
               (const sai_vlan_port_t*)vlan_port));

    /* Create the LAG object with port members */
    lag_port_list.count = member_count;
    lag_port_list.list = port_arr;

    attr.id = SAI_LAG_ATTR_PORT_LIST;
    attr.value.objlist = lag_port_list;

    sai_rc = sai_lag_api_table->create_lag (&lag_id, 1, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create RIF with LAG object id */
    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_mac_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  lag_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Get the VRF attributes */
    sai_test_rif_get_vrf_attributes (vr_id_with_mac_attr, vrf_mac,
                                     &vrf_v4_state, &vrf_v6_state);

    sai_test_rif_mandatory_attr_verify (rif_id, vr_id_with_mac_attr,
                                        SAI_ROUTER_INTERFACE_TYPE_PORT,
                                        lag_id);

    sai_test_rif_optional_attr_verify (rif_id, vrf_mac, vrf_v4_state,
                                       vrf_v6_state);

    /* Remove the port RIF. */
    sai_rc = sai_test_rif_remove (rif_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the LAG */
    sai_rc = sai_lag_api_table->remove_lag (lag_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Add the port members back to default VLAN */
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_vlan_api_table->add_ports_to_vlan(
               saiL3Test::SAI_TEST_DEFAULT_VLAN, member_count,
               (const sai_vlan_port_t*)vlan_port));
}

/*
 * Validate LAG member add or remove for a LAG object that is attached 
 * to a RIF object.
 */
TEST_F (saiL3RifTest, rif_lag_member_update)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t rif_id;
    sai_object_id_t lag_id;
    sai_object_id_t port_arr[2];
    sai_object_list_t lag_port_list;
    sai_attribute_t attr;
    const unsigned int member_count = 4;
    sai_vlan_port_t  vlan_port[member_count];

    vlan_port[0].port_id = sai_l3_port_id_get (2);
    vlan_port[1].port_id = sai_l3_port_id_get (3);
    vlan_port[2].port_id = sai_l3_port_id_get (4);
    vlan_port[3].port_id = sai_l3_port_id_get (5);
    vlan_port[0].tagging_mode = SAI_VLAN_PORT_UNTAGGED;
    vlan_port[1].tagging_mode = SAI_VLAN_PORT_UNTAGGED;
    vlan_port[2].tagging_mode = SAI_VLAN_PORT_UNTAGGED;
    vlan_port[3].tagging_mode = SAI_VLAN_PORT_UNTAGGED;

    ASSERT_TRUE(sai_vlan_api_table != NULL);

    /* Remove the port members from default VLAN */
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_vlan_api_table->remove_ports_from_vlan(
               saiL3Test::SAI_TEST_DEFAULT_VLAN, member_count,
               (const sai_vlan_port_t*)vlan_port));

    /* Create the LAG object with first two port members */
    port_arr[0] = sai_l3_port_id_get (2);
    port_arr[1] = sai_l3_port_id_get (3);
    lag_port_list.count = 2;
    lag_port_list.list = port_arr;

    attr.id = SAI_LAG_ATTR_PORT_LIST;
    attr.value.objlist = lag_port_list;

    sai_rc = sai_lag_api_table->create_lag (&lag_id, 1, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create RIF with LAG object id */
    sai_rc = sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_mac_attr,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  lag_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Add two new members to the LAG */
    lag_port_list.count = 2;
    port_arr[0] = sai_l3_port_id_get (4);
    port_arr[1] = sai_l3_port_id_get (5);

    lag_port_list.list = port_arr;
    sai_rc = sai_lag_api_table->add_ports_to_lag (lag_id, &lag_port_list);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove two members from the LAG */
    lag_port_list.count = 2;
    port_arr[0] = sai_l3_port_id_get (3);
    port_arr[1] = sai_l3_port_id_get (5);

    lag_port_list.list = port_arr;
    sai_rc = sai_lag_api_table->remove_ports_from_lag (lag_id, &lag_port_list);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the port RIF. */
    sai_rc = sai_test_rif_remove (rif_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the LAG */
    sai_rc = sai_lag_api_table->remove_lag (lag_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Add the port members back to default VLAN */
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_vlan_api_table->add_ports_to_vlan(
               saiL3Test::SAI_TEST_DEFAULT_VLAN, member_count,
               (const sai_vlan_port_t*)vlan_port));
}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
