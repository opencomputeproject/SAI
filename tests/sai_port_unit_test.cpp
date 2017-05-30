/*
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
*    sai_port_unit_test.cpp
*     
* Abstract:
*
*    SAI PORT UNIT TEST :- Covers the test cases for all Public API's in SAI PORT module.
*    It begins with default switch Init and covers all the physical switch ports,
*    Port Attribute Get/Set and port statistics counter get APIs.
*
*    For port oper state get to succeed, internal loopback mode will be used,
*
*/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gtest/gtest.h"

extern "C" {
#include "sai.h"
#include "saiswitch.h"
#include "saiport.h"
#include "saitypes.h"
}

/* global port id used for UT*/
sai_port_id_t gport_id = 1;

#define LOG_PRINT(msg, ...) \
    printf(msg, ##__VA_ARGS__)

/* Callback function stubs to be passed during SDK init. They callbacks will not be
 * validated as part of this UT.
 */

/*Port state change callback.
*/
void sai_port_evt_callback(sai_port_id_t port_id,
                           sai_port_oper_status_t opstate)
{
    LOG_PRINT("Port State callback: Port %d link state is %d \r\n", port_id, opstate);
}

/*FDB event callback.
*/
void sai_fdb_evt_callback(sai_fdb_event_t evt_type,
                          sai_fdb_entry_t* fdb_entry,
                          uint32_t attr_count,
                          sai_attribute_t *attr)
{
}
/*Switch operstate callback.
*/
void sai_switch_operstate_callback(sai_switch_oper_status_t switchstate)
{
}

/*
 * Switch shutdown callback.
 */
void  sai_switch_shutdown_callback()
{
}

/*
 * API query is done while running the first test case and
 * the method table is stored in sai_port_api_table so
 * that its available for the rest of the test cases which
 * use the method table
 */
class portTest : public ::testing::Test
{
    public:
        bool sai_switch_max_port_get(uint32_t *max_port);
        bool sai_port_type_logical(sai_port_id_t port_id);
        bool sai_port_speed_set_get(sai_port_id_t port_id, uint32_t speed);
        bool sai_internal_loopback_set_get(sai_port_id_t port_id,
                                           sai_port_internal_loopback_mode_t lb_mode);

    protected:
        static void SetUpTestCase(void)
        {
            sai_switch_notification_t notification;

            /*
             * Query and populate the SAI Switch API Table.
             */
            EXPECT_EQ(SAI_STATUS_SUCCESS, sai_api_query
                      (SAI_API_SWITCH, (static_cast<void**>
                                        (static_cast<void*>(&sai_switch_api_tbl)))));

            ASSERT_TRUE(sai_switch_api_tbl != NULL);

            /*
             * Switch Initialization.
             * Fill in notification callback routines with stubs.
             */
            notification.on_switch_state_change = sai_switch_operstate_callback;
            notification.on_fdb_event = sai_fdb_evt_callback;
            notification.on_port_state_change = sai_port_evt_callback;
            notification.on_switch_shutdown_request = sai_switch_shutdown_callback;

            ASSERT_TRUE(sai_switch_api_tbl->initialize_switch != NULL);
            EXPECT_TRUE(sai_switch_api_tbl->get_switch_attribute != NULL);

            EXPECT_EQ (SAI_STATUS_SUCCESS,
                       (sai_switch_api_tbl->initialize_switch (0, NULL, NULL,
                                                               &notification)));

            ASSERT_EQ(NULL,sai_api_query(SAI_API_PORT,
                                         (static_cast<void**>(static_cast<void*>(&sai_port_api_table)))));

            ASSERT_TRUE(sai_port_api_table != NULL);

            EXPECT_TRUE(sai_port_api_table->set_port_attribute != NULL);
            EXPECT_TRUE(sai_port_api_table->get_port_attribute != NULL);
            EXPECT_TRUE(sai_port_api_table->get_port_stats != NULL);
        }

        static sai_switch_api_t *sai_switch_api_tbl;
        static sai_port_api_t* sai_port_api_table;
};

sai_switch_api_t* portTest ::sai_switch_api_tbl = NULL;
sai_port_api_t* portTest ::sai_port_api_table = NULL;

/* Switch Max port get - returns the maximum ports in the switch */
bool portTest::sai_switch_max_port_get(uint32_t *max_port)
{
    sai_attribute_t sai_get_attr;

    memset(&sai_get_attr, 0, sizeof(sai_attribute_t));
    sai_get_attr.id = SAI_SWITCH_ATTR_PORT_NUMBER;

    if(sai_switch_api_tbl->get_switch_attribute(1, &sai_get_attr) != SAI_STATUS_SUCCESS) {
        return false;
    }

    *max_port = sai_get_attr.value.u32;

    return true;
}

/* For a given SAI port checks for port type - LOGICAL */
bool portTest::sai_port_type_logical(sai_port_id_t port_id)
{
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
    sai_attr_get.id = SAI_PORT_ATTR_TYPE;

    if(sai_port_api_table->get_port_attribute(port_id, 1, &sai_attr_get) != SAI_STATUS_SUCCESS) {
        return false;
    }

    if(SAI_PORT_TYPE_LOGICAL != sai_attr_get.value.s32) {
        return false;
    }

    return true;
}

/*
 * Validates Port type get for all the ports in the switch
 * UT PASS case: port should be logical
 */
TEST_F(portTest, type_get)
{
    uint32_t max_port = 0;
    sai_port_id_t port_id = 0;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
    sai_attr_get.id = SAI_PORT_ATTR_TYPE;
    ASSERT_TRUE(sai_switch_max_port_get(&max_port));

    for(port_id = 0; port_id <= max_port; port_id++) {
        EXPECT_TRUE(sai_port_type_logical(port_id));
    }
}

/*
 * Validates Port Operational Status get for all the valid ports in the switch
 * UT PASS case: port should be Link UP
 *
 * For port oper state get to succeed, internal loopback mode will be used,
 * as time taken for a link to come UP after Admin state set might vary depending on the NPU,
 * and would need adding some sleep in between these calls. Need to re-visit about adding a
 * sleep to validate the actual port Link status instead of using Internal Loopback.
 */
TEST_F(portTest, oper_status_get)
{
    uint32_t max_port = 0;
    sai_port_id_t port_id = 0;
    sai_status_t ret = SAI_STATUS_FAILURE;
    sai_attribute_t sai_attr_get;
    sai_attribute_t sai_attr_set;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
    ASSERT_TRUE(sai_switch_max_port_get(&max_port));

    /* Check port Oper state as UP by enabling the internal Loopback */
    for(port_id = 0; port_id <= max_port; port_id++) {
        if(sai_port_type_logical(port_id) != true) {
            continue;
        }
        /* Enable link admin state */
        sai_attr_set.id = SAI_PORT_ATTR_ADMIN_STATE;
        sai_attr_set.value.booldata = true;

        ret = sai_port_api_table->set_port_attribute(port_id, &sai_attr_set);
        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        /* Loopback set to PHY */
        sai_attr_set.id = SAI_PORT_ATTR_INTERNAL_LOOPBACK;
        sai_attr_set.value.s32 = SAI_PORT_INTERNAL_LOOPBACK_PHY;

        ret = sai_port_api_table->set_port_attribute(port_id, &sai_attr_set);
        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        sai_attr_get.id = SAI_PORT_ATTR_OPER_STATUS;
        ret = sai_port_api_table->get_port_attribute(port_id, 1, &sai_attr_get);
        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }
        EXPECT_EQ(SAI_PORT_OPER_STATUS_UP, sai_attr_get.value.s32);
    }
}


/*
 * Validates if the port is able to get/set a given port speed
 */
bool portTest::sai_port_speed_set_get(sai_port_id_t port_id, uint32_t speed)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_SPEED;
    sai_attr_set.value.u32 = speed;

    if(sai_port_api_table->set_port_attribute(port_id, &sai_attr_set) != SAI_STATUS_SUCCESS) {
        return false;
    }

    sai_attr_get.id = SAI_PORT_ATTR_SPEED;

    if(sai_port_api_table->get_port_attribute(port_id, 1, &sai_attr_get) != SAI_STATUS_SUCCESS) {
        return false;
    }

    if(speed != sai_attr_get.value.u32) {
        return false;
    }

    return true;
}

/*
 * Validates 40G Port speed capability for all the valid ports in the switch
 * UT PASS case: port should be able to set get 40G speed
 */
TEST_F(portTest, speed_40g_set_get)
{
    uint32_t max_port = 0;
    sai_port_id_t port_id = 0;

    ASSERT_TRUE(sai_switch_max_port_get(&max_port));
    for(port_id = 0; port_id <= max_port; port_id++) {
        if(sai_port_type_logical(port_id) != true) {
            continue;
        }
        EXPECT_TRUE(sai_port_speed_set_get(port_id, 40000));
    }
}

/*
 * Validates 10G Port speed capability for all the valid ports in the switch
 * UT PASS case: port should be able to set get 10G speed
 */
TEST_F(portTest, speed_10g_set_get)
{
    uint32_t max_port = 0;
    sai_port_id_t port_id = 0;

    ASSERT_TRUE(sai_switch_max_port_get(&max_port));
    for(port_id = 0; port_id <= max_port; port_id++) {
        if(sai_port_type_logical(port_id) != true) {
            continue;
        }

        EXPECT_TRUE(sai_port_speed_set_get(port_id, 10000));
    }
}

/*
 * Validates 1G Port speed capability for all the valid ports in the switch
 * UT PASS case: port should be able to set get 1G speed
 */
TEST_F(portTest, speed_1g_set_get)
{
    uint32_t max_port = 0;
    sai_port_id_t port_id = 0;

    ASSERT_TRUE(sai_switch_max_port_get(&max_port));
    for(port_id = 0; port_id <= max_port; port_id++) {
        if(sai_port_type_logical(port_id) != true) {
            continue;
        }

        EXPECT_TRUE(sai_port_speed_set_get(port_id, 1000));
    }
}

/*
 * Validates if port admin state can be enabled for all the valid ports in the switch
 * UT PASS case: port admin state should get enabled
 */
TEST_F(portTest, admin_state_set_get)
{
    uint32_t max_port = 0;
    sai_port_id_t port_id = 0;
    sai_status_t ret = SAI_STATUS_FAILURE;
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
    ASSERT_TRUE(sai_switch_max_port_get(&max_port));

    for(port_id = 0; port_id <= max_port; port_id++) {
        if(sai_port_type_logical(port_id) != true) {
            continue;
        }

        sai_attr_set.id = SAI_PORT_ATTR_ADMIN_STATE;
        sai_attr_set.value.booldata = true;

        ret = sai_port_api_table->set_port_attribute(port_id, &sai_attr_set);
        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        sai_attr_get.id = SAI_PORT_ATTR_ADMIN_STATE;
        ret = sai_port_api_table->get_port_attribute(port_id, 1, &sai_attr_get);
        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        EXPECT_TRUE(sai_attr_get.value.booldata);
    }
}

/*
 * Validates if the port default VLAN can be set to 100.
 * UT PASS case: port should be set with default VLAN as 100
 */
TEST_F(portTest, default_vlan_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;
    sai_vlan_id_t vlan_id = 100;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_DEFAULT_VLAN;
    sai_attr_set.value.u16 = vlan_id;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_DEFAULT_VLAN;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_EQ(vlan_id, sai_attr_get.value.u16);
}

/*
 * Validates if the port default VLAN priority can be set to 1.
 * UT PASS case: port should be set with default VLAN priority as 1.
 */
TEST_F(portTest, default_vlan_prio_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY;
    sai_attr_set.value.u8 = 1;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_EQ(1, sai_attr_get.value.u8);
}

/*
 * Validates if the port ingress filter can be enabled
 * UT PASS case: port ingress filter should get enabled
 */
TEST_F(portTest, ingress_filter_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_EQ(true, sai_attr_get.value.booldata);
}

/*
 * Validates if the port drop untagged capability can be enabled
 * UT PASS case: port drop untagged capability should get enabled
 */
TEST_F(portTest, drop_untagged_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_DROP_UNTAGGED;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_DROP_UNTAGGED;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_TRUE(sai_attr_get.value.booldata);
}

/*
 * Validates if the port drop tagged capability can be enabled
 * UT PASS case: port drop tagged capability should get enabled
 */
TEST_F(portTest, drop_tagged_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_DROP_TAGGED;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_DROP_TAGGED;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_TRUE(sai_attr_get.value.booldata);
}

/*
 * Validates if a given looback mode gets configured correctly on a given port
 */
bool portTest::sai_internal_loopback_set_get(sai_port_id_t port_id,
                                             sai_port_internal_loopback_mode_t lb_mode)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_INTERNAL_LOOPBACK;
    sai_attr_set.value.s32 = lb_mode;

    if(sai_port_api_table->set_port_attribute(port_id, &sai_attr_set) != SAI_STATUS_SUCCESS) {
        return false;
    }

    sai_attr_get.id = SAI_PORT_ATTR_INTERNAL_LOOPBACK;

    if(sai_port_api_table->get_port_attribute(port_id, 1, &sai_attr_get) != SAI_STATUS_SUCCESS) {
        return false;
    }
    if(lb_mode != sai_attr_get.value.s32) {
        return false;
    }

    return true;
}

/*
 * Validates internal loopback mode set/get for all the valid ports in the switch
 * UT PASS case: port internal loopback mode should get set to PHY/MAC/NONE
 */
TEST_F(portTest, internal_loopback_set_get)
{
    uint32_t max_port = 0;
    sai_port_id_t port_id = 0;

    ASSERT_TRUE(sai_switch_max_port_get(&max_port));
    for(port_id = 0; port_id <= max_port; port_id++) {
        if(sai_port_type_logical(port_id) != true) {
            continue;
        }

        EXPECT_TRUE(sai_internal_loopback_set_get(port_id, SAI_PORT_INTERNAL_LOOPBACK_PHY));
        EXPECT_TRUE(sai_internal_loopback_set_get(port_id, SAI_PORT_INTERNAL_LOOPBACK_MAC));
        EXPECT_TRUE(sai_internal_loopback_set_get(port_id, SAI_PORT_INTERNAL_LOOPBACK_NONE));
    }
}

/*
 * Validates if the port fdb learning mode can be set to Learning
 * UT PASS case: FDB learning mode should get enabled on the port
 */
TEST_F(portTest, fdb_learning_mode_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_FDB_LEARNING;
    sai_attr_set.value.s32 = SAI_PORT_LEARN_MODE_DISABLE;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_FDB_LEARNING;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_EQ(SAI_PORT_LEARN_MODE_DISABLE, sai_attr_get.value.s32);
}

/*
 * Validates if the port STP state can be set to Learning
 * UT PASS case: STP learning mode should get enabled on the port
 */
TEST_F(portTest, stp_state_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_STP_STATE;
    sai_attr_set.value.s32 = SAI_PORT_STP_STATE_LEARNING;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_STP_STATE;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_EQ(SAI_PORT_STP_STATE_LEARNING, sai_attr_get.value.s32);
}

/*
 * Validates if the port update DSCP can be enabled on the port
 * UT PASS case: Update DSCP should get enabled on the port
 */
TEST_F(portTest, update_dscp_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_UPDATE_DSCP;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_UPDATE_DSCP;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_TRUE(sai_attr_get.value.booldata);
}

/*
 * Validates if the flood storm control can be enabled on the port
 * UT PASS case: flood storm control should get enabled on the port
 */
TEST_F(portTest, flood_storm_control_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_FLOOD_STORM_CONTROL;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_FLOOD_STORM_CONTROL;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_TRUE(sai_attr_get.value.booldata);
}

/*
 * Validates if the broadcast storm control can be enabled on the port
 * UT PASS case: broadcast storm control should get enabled on the port
 */
TEST_F(portTest, broadcast_storm_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_BROADCAST_STORM_CONTROL;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_BROADCAST_STORM_CONTROL;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_TRUE(sai_attr_get.value.booldata);
}

/*
 * Validates if the multicast storm control can be enabled on the port
 * UT PASS case: multicast storm control should get enabled on the port
 */
TEST_F(portTest, multicast_storm_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_MULTICAST_STORM_CONTROL;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_MULTICAST_STORM_CONTROL;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_TRUE(sai_attr_get.value.booldata);
}

/*
 * Validates if the port flow control can be set to Tx only
 * UT PASS case: Flow control should get enabled for Tx only
 */
TEST_F(portTest, flow_control_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL;
    sai_attr_set.value.s32 = SAI_PORT_FLOW_CONTROL_TX_ONLY;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_EQ(SAI_PORT_FLOW_CONTROL_TX_ONLY, sai_attr_get.value.s32);
}

/*
 * Validates if the port MTU can be set to 1000 for all valid ports in the switch
 * UT PASS case: MTU should get set to 1000 on all valid ports
 */
TEST_F(portTest, mtu_set_get)
{
    uint32_t mtu = 1000, max_port = 0;
    sai_port_id_t port_id = 0;
    sai_status_t ret = SAI_STATUS_FAILURE;

    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
    ASSERT_TRUE(sai_switch_max_port_get(&max_port));

    for(port_id = 0; port_id <= max_port; port_id++) {
        if(sai_port_type_logical(port_id) != true) {
            continue;
        }

        sai_attr_set.id = SAI_PORT_ATTR_MTU;
        sai_attr_set.value.u32 = mtu;
        ret = sai_port_api_table->set_port_attribute(port_id, &sai_attr_set);
        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        sai_attr_get.id = SAI_PORT_ATTR_MTU;
        ret = sai_port_api_table->get_port_attribute(port_id, 1, &sai_attr_get);
        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        EXPECT_EQ(mtu, sai_attr_get.value.u32);
    }
}

/*
 * Validates if the port Max Learned Address can be set to 1000
 * UT PASS case: Max learned Address should get set to 1000
 */
TEST_F(portTest, max_learned_addr_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;
    uint32_t learn_limit = 1000;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_MAX_LEARNED_ADDRESSES;
    sai_attr_set.value.u32 = learn_limit;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_MAX_LEARNED_ADDRESSES;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_EQ(learn_limit, sai_attr_get.value.u32);
}

/*
 * Validates if the port FDB learn limit violation to be set to Action Forward
 * UT PASS case: FDB learn limit violation should get to Action Forward
 */
TEST_F(portTest, fdb_learn_limit_violation_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION;
    sai_attr_set.value.s32 = SAI_PACKET_ACTION_FORWARD;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_EQ(SAI_PACKET_ACTION_FORWARD, sai_attr_get.value.s32);
}


/*
 * Port All Statistics Get: Tests only the support of statistics counters;
 * not the stats collection functionality
 */
TEST_F(portTest, all_stats_get)
{
    uint64_t counters[1] = {0};
    int32_t counter = 0;
    sai_port_stat_counter_t counter_ids[1];
    sai_status_t status = SAI_STATUS_FAILURE;

    for(counter = SAI_PORT_STAT_IF_IN_OCTETS;
        counter <= SAI_PORT_STAT_IPV6_OUT_DISCARDS; counter++)
    {
        counter_ids[0] = (sai_port_stat_counter_t)counter;
        status = sai_port_api_table->get_port_stats(gport_id, counter_ids, 1, counters);

        if(status == SAI_STATUS_SUCCESS) {
            LOG_PRINT("Port %d stat id %d value is %ld \r\n", gport_id, counter_ids[0], counters[0]);
        } else if (status == (SAI_STATUS_ATTR_NOT_IMPLEMENTED_0 + counter)) {
            LOG_PRINT("Port %d stat id %d not implemented \r\n", gport_id, counter_ids[0]);
        }

        EXPECT_EQ(SAI_STATUS_SUCCESS, status);
    }
}

