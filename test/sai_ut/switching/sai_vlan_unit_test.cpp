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
*
* Module Name:
*
*    sai_vlan_unit_test.cpp
*
* Abstract:
*
*    This file contains APIS for testing the SAI VLAN module
*
*************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include "inttypes.h"

extern "C" {
#include "sai.h"
#include "saivlan.h"
#include "saitypes.h"
#include "sai_l2_unit_test_defs.h"
}

#define SAI_MAX_PORTS  256

static uint32_t port_count = 0;
static sai_object_id_t port_list[SAI_MAX_PORTS] = {0};

static inline void sai_port_evt_callback (uint32_t count,
                                         sai_port_event_notification_t *data)
{
    uint32_t port_idx = 0;
    sai_object_id_t port_id = 0;
    sai_port_event_t port_event;

    for(port_idx = 0; port_idx < count; port_idx++) {
        port_id = data[port_idx].port_id;
        port_event = data[port_idx].port_event;

        if(port_event == SAI_PORT_EVENT_ADD) {
            if(port_count < SAI_MAX_PORTS) {
                port_list[port_count] = port_id;
                port_count++;
            }

            printf("PORT ADD EVENT FOR port %"PRIu64" and total ports count is %d \r\n",
                   port_id, port_count);
        } else if(port_event == SAI_PORT_EVENT_DELETE) {

            printf("PORT DELETE EVENT for  port %"PRIu64" and total ports count is %d \r\n",
                   port_id, port_count);
        } else {
            printf("Invalid PORT EVENT for port %"PRIu64" \r\n", port_id);
        }
    }
}

/*
 * API query is done while running the first test case and
 * the method table is stored in sai_vlan_api_table so
 * that its available for the rest of the test cases which
 * use the method table
 */
class vlanInit : public ::testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            sai_switch_api_t *p_sai_switch_api_tbl = NULL;
            sai_switch_notification_t notification;
            memset (&notification, 0, sizeof(sai_switch_notification_t));

            /*
             * Query and populate the SAI Switch API Table.
             */
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
                       (SAI_API_SWITCH, (static_cast<void**>
                                         (static_cast<void*>(&p_sai_switch_api_tbl)))));

            ASSERT_TRUE (p_sai_switch_api_tbl != NULL);

            /*
             * Switch Initialization.
             * Fill in notification callback routines with stubs.
             */
            notification.on_switch_state_change = sai_switch_operstate_callback;
            notification.on_fdb_event = sai_fdb_evt_callback;
            notification.on_port_state_change = sai_port_state_evt_callback;
            notification.on_switch_shutdown_request = sai_switch_shutdown_callback;
            notification.on_packet_event = sai_packet_event_callback;
            notification.on_port_event = sai_port_evt_callback;

            ASSERT_TRUE(p_sai_switch_api_tbl->initialize_switch != NULL);

            EXPECT_EQ (SAI_STATUS_SUCCESS,
                       (p_sai_switch_api_tbl->initialize_switch (0, NULL, NULL,
                                                                 &notification)));

            ASSERT_EQ(NULL,sai_api_query(SAI_API_VLAN,
                                         (static_cast<void**>(static_cast<void*>(&sai_vlan_api_table)))));

            ASSERT_TRUE(sai_vlan_api_table != NULL);

            EXPECT_TRUE(sai_vlan_api_table->create_vlan != NULL);
            EXPECT_TRUE(sai_vlan_api_table->remove_vlan != NULL);
            EXPECT_TRUE(sai_vlan_api_table->set_vlan_attribute != NULL);
            EXPECT_TRUE(sai_vlan_api_table->get_vlan_attribute != NULL);
            EXPECT_TRUE(sai_vlan_api_table->add_ports_to_vlan != NULL);
            EXPECT_TRUE(sai_vlan_api_table->remove_ports_from_vlan != NULL);
            EXPECT_TRUE(sai_vlan_api_table->remove_all_vlans != NULL);
            EXPECT_TRUE(sai_vlan_api_table->get_vlan_stats != NULL);

            port_id_1 = port_list[0];
            port_id_2 = port_list[port_count-1];
            port_id_invalid = port_id_2 + 1;
        }

        static sai_vlan_api_t* sai_vlan_api_table;
        static sai_object_id_t  port_id_1;
        static sai_object_id_t  port_id_2;
        static sai_object_id_t  port_id_invalid;
};

sai_vlan_api_t* vlanInit ::sai_vlan_api_table = NULL;
sai_object_id_t vlanInit ::port_id_1 = 0;
sai_object_id_t vlanInit ::port_id_2 = 0;
sai_object_id_t vlanInit ::port_id_invalid = 0;

/*
 * VLAN create and delete
 */
TEST_F(vlanInit, create_remove_vlan)
{

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->create_vlan(SAI_GTEST_VLAN));

    ASSERT_EQ(SAI_STATUS_ITEM_ALREADY_EXISTS,
              sai_vlan_api_table->create_vlan(SAI_GTEST_VLAN));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->remove_vlan(SAI_GTEST_VLAN));

    ASSERT_EQ(SAI_STATUS_ITEM_NOT_FOUND,
              sai_vlan_api_table->remove_vlan(SAI_GTEST_VLAN));

    ASSERT_EQ(SAI_STATUS_INVALID_VLAN_ID,
              sai_vlan_api_table->create_vlan(SAI_GTEST_INVALID_VLAN_ID));

    ASSERT_EQ(SAI_STATUS_INVALID_VLAN_ID,
              sai_vlan_api_table->remove_vlan(SAI_GTEST_INVALID_VLAN_ID));
}
TEST_F(vlanInit, remove_all_vlans)
{

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->create_vlan(SAI_GTEST_VLAN));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->remove_all_vlans());

    ASSERT_EQ(SAI_STATUS_ITEM_NOT_FOUND,
              sai_vlan_api_table->remove_vlan(SAI_GTEST_VLAN));
}
/*
 * VLAN add port and remove port
 */
TEST_F(vlanInit, vlan_port_test)
{
    sai_vlan_port_t vlan_port[2];
    sai_vlan_port_t invalid_vlan_port[2];

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->create_vlan(SAI_GTEST_VLAN));

    memset(vlan_port, 0, sizeof(vlan_port));

    vlan_port[0].port_id = port_id_1;
    vlan_port[0].tagging_mode = SAI_VLAN_PORT_UNTAGGED;
    vlan_port[1].port_id = port_id_2;
    vlan_port[1].tagging_mode = SAI_VLAN_PORT_TAGGED;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->add_ports_to_vlan(SAI_GTEST_VLAN,
                                                    2,
                                                    (const sai_vlan_port_t*)
                                                    vlan_port));
    invalid_vlan_port[0].port_id = port_id_1;
    invalid_vlan_port[0].tagging_mode = SAI_VLAN_PORT_TAGGED;
    invalid_vlan_port[1].port_id = port_id_2;
    invalid_vlan_port[1].tagging_mode = SAI_VLAN_PORT_TAGGED;

    ASSERT_EQ(SAI_STATUS_INVALID_PORT_MEMBER,
              sai_vlan_api_table->add_ports_to_vlan(SAI_GTEST_VLAN,
                                                    2,
                                                    (const sai_vlan_port_t*)
                                                    invalid_vlan_port));

    ASSERT_EQ(SAI_STATUS_INVALID_PORT_MEMBER,
              sai_vlan_api_table->remove_ports_from_vlan(SAI_GTEST_VLAN,
                                                    2,
                                                    (const sai_vlan_port_t*)
                                                    invalid_vlan_port));

    invalid_vlan_port[0].port_id = port_id_invalid;
    invalid_vlan_port[0].tagging_mode = SAI_VLAN_PORT_TAGGED;
    invalid_vlan_port[1].port_id = port_id_2;
    invalid_vlan_port[1].tagging_mode = SAI_VLAN_PORT_TAGGED;

    ASSERT_EQ(SAI_STATUS_INVALID_OBJECT_ID,
              sai_vlan_api_table->add_ports_to_vlan(SAI_GTEST_VLAN,
                                                    2,
                                                    (const sai_vlan_port_t*)
                                                    invalid_vlan_port));

    ASSERT_EQ(SAI_STATUS_INVALID_OBJECT_ID,
              sai_vlan_api_table->remove_ports_from_vlan(SAI_GTEST_VLAN,
                                                    2,
                                                    (const sai_vlan_port_t*)
                                                    invalid_vlan_port));
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->remove_ports_from_vlan(SAI_GTEST_VLAN,
                                                    2,
                                                    (const sai_vlan_port_t*)
                                                    vlan_port));

    ASSERT_EQ(SAI_STATUS_INVALID_PORT_MEMBER,
              sai_vlan_api_table->remove_ports_from_vlan(SAI_GTEST_VLAN,
                                                    2,
                                                    (const sai_vlan_port_t*)
                                                    vlan_port));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->remove_vlan(SAI_GTEST_VLAN));
}
/*
 * VLAN get port test
 */
TEST_F(vlanInit, vlan_get_port)
{
    sai_vlan_port_t vlan_port[2];
    sai_attribute_t attr;
    sai_vlan_port_list_t vplist;
    sai_vlan_port_t get_vlan_port[2];

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->create_vlan(SAI_GTEST_VLAN));

    memset(vlan_port, 0, sizeof(vlan_port));

    vlan_port[0].port_id = port_id_1;
    vlan_port[0].tagging_mode = SAI_VLAN_PORT_UNTAGGED;
    vlan_port[1].port_id = port_id_2;
    vlan_port[1].tagging_mode = SAI_VLAN_PORT_TAGGED;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->add_ports_to_vlan(SAI_GTEST_VLAN,
                                                    2,
                                                    (const sai_vlan_port_t*)
                                                    vlan_port));

    memset(&attr, 0, sizeof(attr));
    memset(&vplist, 0, sizeof(vplist));
    memset(get_vlan_port, 0, sizeof(get_vlan_port));
    vplist.count = 2;
    vplist.list = get_vlan_port;

    attr.id = SAI_VLAN_ATTR_PORT_LIST;
    attr.value.vlanportlist = vplist;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->get_vlan_attribute(SAI_GTEST_VLAN, 1, &attr));

    EXPECT_EQ(vlan_port[0].port_id, get_vlan_port[0].port_id);
    EXPECT_EQ(vlan_port[1].port_id, get_vlan_port[1].port_id);
    EXPECT_EQ(vlan_port[0].tagging_mode, get_vlan_port[0].tagging_mode);
    EXPECT_EQ(vlan_port[1].tagging_mode, get_vlan_port[1].tagging_mode);

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->remove_ports_from_vlan(SAI_GTEST_VLAN,
                                                    2,
                                                    (const sai_vlan_port_t*)
                                                    vlan_port));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->remove_vlan(SAI_GTEST_VLAN));
}
/*
 * VLAN port priority tag testing
 */
TEST_F(vlanInit, vlan_port_priority_tag_test)
{
    sai_vlan_port_t vlan_port;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->create_vlan(SAI_GTEST_VLAN));

    memset(&vlan_port, 0, sizeof(vlan_port));

    vlan_port.port_id = port_id_1;
    vlan_port.tagging_mode = SAI_VLAN_PORT_PRIORITY_TAGGED;

    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->add_ports_to_vlan(SAI_GTEST_VLAN,
                                                    1,
                                                    (const sai_vlan_port_t*)
                                                    &vlan_port));
    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->remove_ports_from_vlan(SAI_GTEST_VLAN,
                                                    1,
                                                    (const sai_vlan_port_t*)
                                                    &vlan_port));
    EXPECT_EQ(SAI_STATUS_INVALID_PORT_MEMBER,
              sai_vlan_api_table->remove_ports_from_vlan(SAI_GTEST_VLAN,
                                                    1,
                                                    (const sai_vlan_port_t*)
                                                    &vlan_port));
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->remove_vlan(SAI_GTEST_VLAN));
}
/*
 * VLAN test MAX learnt MAC address
 */
TEST_F(vlanInit, test_max_learnt_mac_address)
{
    sai_attribute_t set_attr;
    sai_attribute_t get_attr;


    memset(&set_attr, 0, sizeof(sai_attribute_t));
    memset(&get_attr, 0, sizeof(sai_attribute_t));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->create_vlan(SAI_GTEST_VLAN));

    set_attr.id = SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES;
    set_attr.value.u32 = 100;

    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->set_vlan_attribute(SAI_GTEST_VLAN,
                                                   (const sai_attribute_t*)&set_attr));
    get_attr.id = SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->get_vlan_attribute(SAI_GTEST_VLAN,1,
                                                           &get_attr));
    EXPECT_EQ(set_attr.value.u32,get_attr.value.u32);
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->remove_vlan(SAI_GTEST_VLAN));
}
/*
 * VLAN test learn disable
 */
TEST_F(vlanInit, test_vlan_learn_disable)
{
    sai_attribute_t set_attr;
    sai_attribute_t get_attr;


    memset(&set_attr, 0, sizeof(sai_attribute_t));
    memset(&get_attr, 0, sizeof(sai_attribute_t));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->create_vlan(SAI_GTEST_VLAN));

    set_attr.id = SAI_VLAN_ATTR_LEARN_DISABLE;
    set_attr.value.booldata = true;

    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->set_vlan_attribute(SAI_GTEST_VLAN,
                                                   (const sai_attribute_t*)&set_attr));
    get_attr.id = SAI_VLAN_ATTR_LEARN_DISABLE;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->get_vlan_attribute(SAI_GTEST_VLAN,1,
                                                           &get_attr));
    EXPECT_EQ(set_attr.value.booldata,get_attr.value.booldata);
    set_attr.value.booldata = false;

    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->set_vlan_attribute(SAI_GTEST_VLAN,
                                                   (const sai_attribute_t*)&set_attr));
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->remove_vlan(SAI_GTEST_VLAN));
}
/*
 * VLAN test get statistics
 */
TEST_F(vlanInit, vlan_get_stats)
{
    sai_vlan_stat_counter_t id = SAI_VLAN_STAT_IN_OCTETS;
    uint64_t counter_val = 0;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->create_vlan(SAI_GTEST_VLAN));

    EXPECT_EQ(SAI_STATUS_SUCCESS,
          sai_vlan_api_table->get_vlan_stats(SAI_GTEST_VLAN,
                                                (const sai_vlan_stat_counter_t*)&id,1,
                                                &counter_val));
    id = SAI_VLAN_STAT_IN_UCAST_PKTS;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
          sai_vlan_api_table->get_vlan_stats(SAI_GTEST_VLAN,
                                                (const sai_vlan_stat_counter_t*)&id,1,
                                                &counter_val));

    id = SAI_VLAN_STAT_IN_NON_UCAST_PKTS;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
          sai_vlan_api_table->get_vlan_stats(SAI_GTEST_VLAN,
                                                (const sai_vlan_stat_counter_t*)&id,1,
                                                &counter_val));

    id = SAI_VLAN_STAT_IN_DISCARDS;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
          sai_vlan_api_table->get_vlan_stats(SAI_GTEST_VLAN,
                                                (const sai_vlan_stat_counter_t*)&id,1,
                                                &counter_val));

    id = SAI_VLAN_STAT_IN_ERRORS;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
          sai_vlan_api_table->get_vlan_stats(SAI_GTEST_VLAN,
                                                (const sai_vlan_stat_counter_t*)&id,1,
                                                &counter_val));


    id = SAI_VLAN_STAT_IN_UNKNOWN_PROTOS;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
          sai_vlan_api_table->get_vlan_stats(SAI_GTEST_VLAN,
                                                (const sai_vlan_stat_counter_t*)&id,1,
                                                &counter_val));

    id = SAI_VLAN_STAT_OUT_OCTETS;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
          sai_vlan_api_table->get_vlan_stats(SAI_GTEST_VLAN,
                                                (const sai_vlan_stat_counter_t*)&id,1,
                                                &counter_val));

    id = SAI_VLAN_STAT_OUT_UCAST_PKTS;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
          sai_vlan_api_table->get_vlan_stats(SAI_GTEST_VLAN,
                                                (const sai_vlan_stat_counter_t*)&id,1,
                                                &counter_val));

    id = SAI_VLAN_STAT_OUT_NON_UCAST_PKTS;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
          sai_vlan_api_table->get_vlan_stats(SAI_GTEST_VLAN,
                                                (const sai_vlan_stat_counter_t*)&id,1,
                                                &counter_val));

    id = SAI_VLAN_STAT_OUT_DISCARDS;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
          sai_vlan_api_table->get_vlan_stats(SAI_GTEST_VLAN,
                                                (const sai_vlan_stat_counter_t*)&id,1,
                                                &counter_val));

    id = SAI_VLAN_STAT_OUT_ERRORS;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
          sai_vlan_api_table->get_vlan_stats(SAI_GTEST_VLAN,
                                                (const sai_vlan_stat_counter_t*)&id,1,
                                                &counter_val));

    id = SAI_VLAN_STAT_OUT_QLEN;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
          sai_vlan_api_table->get_vlan_stats(SAI_GTEST_VLAN,
                                                (const sai_vlan_stat_counter_t*)&id,1,
                                                &counter_val));
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_vlan_api_table->remove_vlan(SAI_GTEST_VLAN));
}
