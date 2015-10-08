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
*    sai_switch_unit_test.cpp
*
* Abstract: 
*
*    SAI SWITCH UNIT TEST :- Covers the test cases for validating switch public
*    APIs in SAI SWITCH Module
*
*************************************************************************/

#include "gtest/gtest.h"


extern "C" {
#include "sai.h"

#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
}

#define SAI_MAX_PORTS 256
#define SAI_SWITCH_MAX_HASH_FIELDS    (10)

static uint32_t port_count = 0;
static sai_object_id_t port_list[SAI_MAX_PORTS] = {0};
static sai_object_id_t cpu_port = 0;

#define LOG_PRINT(msg, ...) \
    printf(msg, ##__VA_ARGS__)

/* Callback functions to be passed during SDK init.
*/

/*Port state change callback.
*/
void sai_port_state_evt_callback(uint32_t count,
                                 sai_port_oper_status_notification_t *data)
{
    uint32_t port_idx = 0;
    for(port_idx = 0; port_idx < count; port_idx++) {
        LOG_PRINT("port %"PRIx64" State callback: Link state is %d\r\n",
                  data[port_idx].port_id, data[port_idx].port_state);
    }
}

/*Port ADD/DELETE event callback.
*/
void sai_port_evt_callback(uint32_t count,
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

            LOG_PRINT("PORT ADD EVENT FOR port %"PRIu64" and total ports count is %d \r\n",
                      port_id, port_count);
        } else if(port_event == SAI_PORT_EVENT_DELETE) {
            port_count = 0;

            LOG_PRINT("PORT DELETE EVENT for port %"PRIu64" and total ports count is %d \r\n",
                      port_id, port_count);
        } else {
            LOG_PRINT("Invalid PORT EVENT for port %"PRIu64" \r\n", port_id);
        }
    }
}

/*
 * Stubs for Callback functions to be passed from adapter host/application.
 */
#ifdef UNREFERENCED_PARAMETER
#elif defined(__GNUC__)
#define UNREFERENCED_PARAMETER(P)   (void)(P)
#else
#define UNREFERENCED_PARAMETER(P)   (P)
#endif
/*FDB event callback.
*/
void sai_fdb_evt_callback(uint32_t count, sai_fdb_event_notification_data_t *data)
{
    UNREFERENCED_PARAMETER(count);
    UNREFERENCED_PARAMETER(data);
}
/*Switch operstate callback.
*/
void sai_switch_operstate_callback(sai_switch_oper_status_t switchstate)
{
    LOG_PRINT("Switch Operation state is %d\r\n", switchstate);
}

/* Packet event callback
 */
static inline void sai_packet_event_callback (const void *buffer,
                                              sai_size_t buffer_size,
                                              uint32_t attr_count,
                                              const sai_attribute_t *attr_list)
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(buffer_size);
    UNREFERENCED_PARAMETER(attr_count);
    UNREFERENCED_PARAMETER(attr_list);
}

/*
 * Switch shutdown callback.
 */
void  sai_switch_shutdown_callback()
{
}

/*
 * NPU SDK init sequence.
 */
void sdkinit()
{
    sai_switch_api_t* sai_switch_api = NULL;
    sai_switch_notification_t notification;

    memset (&notification, 0, sizeof(sai_switch_notification_t));

    notification.on_switch_state_change = sai_switch_operstate_callback;
    notification.on_fdb_event = sai_fdb_evt_callback;
    notification.on_port_state_change = sai_port_state_evt_callback;
    notification.on_switch_shutdown_request = sai_switch_shutdown_callback;
    notification.on_port_event = sai_port_evt_callback;
    notification.on_packet_event = sai_packet_event_callback;

    sai_api_query(SAI_API_SWITCH,
                  (static_cast<void**>
                   (static_cast<void*>(&sai_switch_api))));

    if(sai_switch_api->initialize_switch)
    {
        sai_switch_api->initialize_switch(0,NULL,NULL,&notification);
    }
}


/*
 * API query is done while running the first test case and
 * the method table is stored in sai_switch_api_table so
 * that its available for the rest of the test cases which
 * use the method table
 */
class switchInit : public ::testing::Test
{
    public:
        bool sai_miss_action_test(sai_switch_attr_t attr, sai_packet_action_t action);
        bool sai_switch_max_port_get(uint32_t *max_port);

    protected:
        static void SetUpTestCase()
        {
            ASSERT_EQ(NULL,sai_api_query(SAI_API_SWITCH,
                                         (static_cast<void**>(static_cast<void*>(&sai_switch_api_table)))));

            ASSERT_TRUE(sai_switch_api_table != NULL);

            EXPECT_TRUE(sai_switch_api_table->initialize_switch != NULL);
            EXPECT_TRUE(sai_switch_api_table->shutdown_switch != NULL);
            EXPECT_TRUE(sai_switch_api_table->connect_switch != NULL);
            EXPECT_TRUE(sai_switch_api_table->disconnect_switch != NULL);
            EXPECT_TRUE(sai_switch_api_table->set_switch_attribute != NULL);
            EXPECT_TRUE(sai_switch_api_table->get_switch_attribute != NULL);

            sdkinit();

            ASSERT_EQ(NULL,sai_api_query(SAI_API_PORT,
                                         (static_cast<void**>(static_cast<void*>(&sai_port_api_table)))));

            ASSERT_TRUE(sai_port_api_table != NULL);

            EXPECT_TRUE(sai_port_api_table->set_port_attribute != NULL);
            EXPECT_TRUE(sai_port_api_table->get_port_attribute != NULL);
            EXPECT_TRUE(sai_port_api_table->get_port_stats != NULL);

            /* Get and update the CPU port */
            sai_attribute_t get_attr;
            memset(&get_attr, 0, sizeof(get_attr));
            get_attr.id = SAI_SWITCH_ATTR_CPU_PORT;

            EXPECT_EQ(SAI_STATUS_SUCCESS,
                      sai_switch_api_table->get_switch_attribute(1, &get_attr));

            cpu_port = get_attr.value.oid;
        }

        static sai_switch_api_t* sai_switch_api_table;
        static sai_port_api_t* sai_port_api_table;
};

sai_switch_api_t* switchInit ::sai_switch_api_table = NULL;
sai_port_api_t* switchInit ::sai_port_api_table = NULL;

/*
 * Get the values for the list of attribute passed.
 */
TEST_F(switchInit, attr_get)
{
    sai_attribute_t sai_attr[3];

    sai_attribute_t sai_attr_set;

    sai_attr_set.id = SAI_SWITCH_ATTR_SWITCHING_MODE;
    sai_attr_set.value.s32 = SAI_SWITCHING_MODE_CUT_THROUGH;

    ASSERT_EQ(SAI_STATUS_SUCCESS, 
              sai_switch_api_table->set_switch_attribute(&sai_attr_set));

    sai_attr[0].id = SAI_SWITCH_ATTR_SWITCHING_MODE;

    ASSERT_EQ(SAI_STATUS_SUCCESS,sai_switch_api_table->
              get_switch_attribute(1,&sai_attr[0]));

    ASSERT_EQ(SAI_SWITCHING_MODE_CUT_THROUGH,sai_attr[0].value.s32);
}

/*
 * Get the switch oper state
 */
TEST_F(switchInit, oper_status_get)
{
    sai_attribute_t get_attr;

    get_attr.id = SAI_SWITCH_ATTR_OPER_STATUS;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1, &get_attr));

    ASSERT_EQ(SAI_SWITCH_OPER_STATUS_UP, get_attr.value.s32);
}

/*
 * Get the current switch maximum temperature
 */
TEST_F(switchInit, temp_get)
{
    sai_attribute_t get_attr;

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_MAX_TEMP;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1, &get_attr));

    LOG_PRINT("Switch current max temperature is %d \r\n", get_attr.value.s32);
}

/*
 * Get the maximum ports count
 */
TEST_F(switchInit, max_port_get)
{
    sai_attribute_t get_attr;

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_PORT_NUMBER;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1, &get_attr));

    LOG_PRINT("Maximum port number in the switch is %d \r\n", get_attr.value.u32);
}

/*
 * Get the CPU port
 */
TEST_F(switchInit, cpu_port_get)
{
    sai_attribute_t get_attr;

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_CPU_PORT;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1, &get_attr));

    cpu_port = get_attr.value.oid;
    LOG_PRINT("CPU port obj id is %"PRIu64" \r\n", get_attr.value.oid);
}

/*
 * Validates if the CPU port default VLAN can be set to 100.
 * UT PASS case: port should be set with default VLAN as 100
 */
TEST_F(switchInit, cpu_default_vlan_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;
    sai_vlan_id_t vlan_id = 100;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_PORT_VLAN_ID;
    sai_attr_set.value.u16 = vlan_id;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(cpu_port, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_PORT_VLAN_ID;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(cpu_port, 1, &sai_attr_get));

    ASSERT_EQ(vlan_id, sai_attr_get.value.u16);
}

/*
 * Validates if the CPU port default VLAN priority can be set to 1.
 * UT PASS case: port should be set with default VLAN priority as 1.
 */
TEST_F(switchInit, cpu_default_vlan_prio_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY;
    sai_attr_set.value.u8 = 1;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(cpu_port, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(cpu_port, 1, &sai_attr_get));

    ASSERT_EQ(1, sai_attr_get.value.u8);
}

/*
 * Validates if the CPU port ingress filter can be enabled
 * UT PASS case: port ingress filter should get enabled
 */
TEST_F(switchInit, cpu_ingress_filter_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(cpu_port, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(cpu_port, 1, &sai_attr_get));

    ASSERT_EQ(true, sai_attr_get.value.booldata);
}

/*
 * Validates if the CPU port drop untagged capability can be enabled
 * UT PASS case: port drop untagged capability should get enabled
 */
TEST_F(switchInit, cpu_drop_untagged_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_DROP_UNTAGGED;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(cpu_port, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_DROP_UNTAGGED;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(cpu_port, 1, &sai_attr_get));

    ASSERT_TRUE(sai_attr_get.value.booldata);
}

/*
 * Validates if the CPU port drop tagged capability can be enabled
 * UT PASS case: port drop tagged capability should get enabled
 */
TEST_F(switchInit, cpu_drop_tagged_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_DROP_TAGGED;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(cpu_port, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_DROP_TAGGED;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(cpu_port, 1, &sai_attr_get));

    ASSERT_TRUE(sai_attr_get.value.booldata);
}

/*
 * Validates if the CPU port fdb learning mode can be set to Learning
 * UT PASS case: FDB learning mode should get enabled on the port
 */
TEST_F(switchInit, cpu_fdb_learning_mode_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_FDB_LEARNING;
    sai_attr_set.value.s32 = SAI_PORT_LEARN_MODE_DISABLE;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(cpu_port, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_FDB_LEARNING;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(cpu_port, 1, &sai_attr_get));

    ASSERT_EQ(SAI_PORT_LEARN_MODE_DISABLE, sai_attr_get.value.s32);
}

/* Switch Max port get - returns the maximum ports in the switch */
bool switchInit::sai_switch_max_port_get(uint32_t *max_port)
{
    sai_attribute_t sai_get_attr;

    memset(&sai_get_attr, 0, sizeof(sai_attribute_t));
    sai_get_attr.id = SAI_SWITCH_ATTR_PORT_NUMBER;

    if(sai_switch_api_table->get_switch_attribute(1, &sai_get_attr) != SAI_STATUS_SUCCESS) {
        return false;
    }

    *max_port = sai_get_attr.value.u32;

    return true;
}

/*
 * Get and print the list of valid logical ports in the switch
 */
TEST_F(switchInit, attr_port_list_get)
{
    sai_attribute_t get_attr;
    uint32_t count = 0;
    sai_status_t ret = SAI_STATUS_FAILURE;

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_PORT_LIST;

    get_attr.value.objlist.count = 1;
    get_attr.value.objlist.list = (sai_object_id_t *) calloc(1,
                                                             sizeof(sai_object_id_t));
    ASSERT_TRUE(get_attr.value.objlist.list != NULL);

    ret = sai_switch_api_table->get_switch_attribute(1, &get_attr);

    /* Handles buffer overflow case, by reallocating required memory */
    if(ret == SAI_STATUS_BUFFER_OVERFLOW ) {
        free(get_attr.value.objlist.list);
        get_attr.value.objlist.list = (sai_object_id_t *)calloc (get_attr.value.objlist.count,
                                                                 sizeof(sai_object_id_t *));
        ret = sai_switch_api_table->get_switch_attribute(1, &get_attr);
    }

    if(ret != SAI_STATUS_SUCCESS) {
        free(get_attr.value.objlist.list);
        ASSERT_EQ(SAI_STATUS_SUCCESS, ret);
    }

    LOG_PRINT("Port list count is %d \r\n", get_attr.value.objlist.count);

    for(count = 0; count < get_attr.value.objlist.count; count++) {
        LOG_PRINT("Port list id %d is %lu \r\n", count, get_attr.value.objlist.list[count]);
    }

    free(get_attr.value.objlist.list);
}

/*
 * Test attribute counter thread refresh interval
 */
TEST_F(switchInit, attr_counter_refresh_interval_get)
{
    sai_attribute_t set_attr;
    sai_attribute_t get_attr;

    memset(&set_attr, 0, sizeof(set_attr));
    set_attr.id = SAI_SWITCH_ATTR_COUNTER_REFRESH_INTERVAL;
    set_attr.value.u32 = 100;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->set_switch_attribute((const sai_attribute_t*)&set_attr));

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_COUNTER_REFRESH_INTERVAL;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1,&get_attr));

    ASSERT_EQ(get_attr.value.u32, set_attr.value.u32);

    /* HW based counter statistics read */
    set_attr.value.u32 = 0;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->set_switch_attribute((const sai_attribute_t*)&set_attr));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1,&get_attr));

    ASSERT_EQ(get_attr.value.u32, set_attr.value.u32);
}

/*
 * Test attribute broadcast flood enable
 */
TEST_F(switchInit, attr_bcast_flood_enable)
{
    sai_attribute_t set_attr;
    sai_attribute_t get_attr;

    memset(&set_attr, 0, sizeof(set_attr));
    set_attr.id = SAI_SWITCH_ATTR_BCAST_CPU_FLOOD_ENABLE;
    set_attr.value.booldata = true;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->set_switch_attribute((const sai_attribute_t*)&set_attr));

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_BCAST_CPU_FLOOD_ENABLE;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1,&get_attr));

    ASSERT_EQ(get_attr.value.booldata,set_attr.value.booldata);
}

/*
 * Test attribute multicast flood enable
 */
TEST_F(switchInit, attr_mcast_flood_enable)
{
    sai_attribute_t set_attr;
    sai_attribute_t get_attr;

    memset(&set_attr, 0, sizeof(set_attr));
    set_attr.id = SAI_SWITCH_ATTR_MCAST_CPU_FLOOD_ENABLE;
    set_attr.value.booldata = true;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->set_switch_attribute((const sai_attribute_t*)&set_attr));

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_MCAST_CPU_FLOOD_ENABLE;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1,&get_attr));

    ASSERT_EQ(get_attr.value.booldata,set_attr.value.booldata);
}

/*
 * Test attribute max learned address
 */
TEST_F(switchInit, attr_max_learned_addresses)
{
    sai_attribute_t set_attr;
    sai_attribute_t get_attr;

    memset(&set_attr, 0, sizeof(set_attr));
    set_attr.id = SAI_SWITCH_ATTR_MAX_LEARNED_ADDRESSES;
    set_attr.value.u32 = 100;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->set_switch_attribute((const sai_attribute_t*)&set_attr));

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_MAX_LEARNED_ADDRESSES;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1,&get_attr));

    ASSERT_EQ(get_attr.value.u32, set_attr.value.u32);
}

/*
 * Test attribute FDB Aging time
 */
TEST_F(switchInit, attr_fdb_aging_time)
{
    sai_attribute_t set_attr;
    sai_attribute_t get_attr;

    memset(&set_attr, 0, sizeof(set_attr));
    set_attr.id = SAI_SWITCH_ATTR_FDB_AGING_TIME;
    set_attr.value.u32 = 10;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->set_switch_attribute((const sai_attribute_t*)&set_attr));

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_FDB_AGING_TIME;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1,&get_attr));

    ASSERT_EQ(get_attr.value.u32, set_attr.value.u32);
}

bool switchInit::sai_miss_action_test(sai_switch_attr_t attr, sai_packet_action_t action)
{
    sai_attribute_t set_attr;
    sai_attribute_t get_attr;

    memset(&set_attr, 0, sizeof(set_attr));
    set_attr.id = attr;
    set_attr.value.s32 = action;
    if(sai_switch_api_table->set_switch_attribute(
                                                  (const sai_attribute_t*)&set_attr) != SAI_STATUS_SUCCESS) {
        return false;
    }

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = attr;

    if(sai_switch_api_table->get_switch_attribute(1,&get_attr)!= SAI_STATUS_SUCCESS) {
        return false;
    }

    if(get_attr.value.s32 != set_attr.value.s32) {
        return false;
    }
    return true;
}
/*
 * Test attributes miss actions
 */
TEST_F(switchInit, attr_miss_action)
{
    EXPECT_TRUE(sai_miss_action_test(SAI_SWITCH_ATTR_FDB_UNICAST_MISS_ACTION,SAI_PACKET_ACTION_FORWARD));
    EXPECT_TRUE(sai_miss_action_test(SAI_SWITCH_ATTR_FDB_UNICAST_MISS_ACTION,SAI_PACKET_ACTION_TRAP));
    EXPECT_TRUE(sai_miss_action_test(SAI_SWITCH_ATTR_FDB_UNICAST_MISS_ACTION,SAI_PACKET_ACTION_LOG));
    EXPECT_TRUE(sai_miss_action_test(SAI_SWITCH_ATTR_FDB_UNICAST_MISS_ACTION,SAI_PACKET_ACTION_DROP));

    EXPECT_TRUE(sai_miss_action_test(SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_ACTION,SAI_PACKET_ACTION_FORWARD));
    EXPECT_TRUE(sai_miss_action_test(SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_ACTION,SAI_PACKET_ACTION_TRAP));
    EXPECT_TRUE(sai_miss_action_test(SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_ACTION,SAI_PACKET_ACTION_LOG));
    EXPECT_TRUE(sai_miss_action_test(SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_ACTION,SAI_PACKET_ACTION_DROP));

    EXPECT_TRUE(sai_miss_action_test(SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_ACTION,SAI_PACKET_ACTION_FORWARD));
    EXPECT_TRUE(sai_miss_action_test(SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_ACTION,SAI_PACKET_ACTION_TRAP));
    EXPECT_TRUE(sai_miss_action_test(SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_ACTION,SAI_PACKET_ACTION_LOG));
    EXPECT_TRUE(sai_miss_action_test(SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_ACTION,SAI_PACKET_ACTION_DROP));
}
/*
 * Connects different processes to the SAI process.
 */
TEST_F(switchInit, connect_switch)
{
    sai_switch_notification_t notification;
    ASSERT_EQ(SAI_STATUS_SUCCESS,sai_switch_api_table->
              connect_switch(0,NULL,&notification));
}

TEST_F(switchInit, default_hash_values_get)
{
    sai_attribute_t get_attr;

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_ECMP_HASH_ALGO;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->get_switch_attribute(1,&get_attr));
    ASSERT_EQ(get_attr.value.u32, SAI_HASH_CRC);

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_LAG_HASH_ALGO;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->get_switch_attribute(1,&get_attr));
    ASSERT_EQ(get_attr.value.u32, SAI_HASH_CRC);

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_ECMP_HASH_SEED;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->get_switch_attribute(1,&get_attr));

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_LAG_HASH_SEED;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->get_switch_attribute(1,&get_attr));

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_ECMP_HASH_FIELDS;
    get_attr.value.s32list.count = SAI_SWITCH_MAX_HASH_FIELDS;
    get_attr.value.s32list.list =
                        (int32_t *) calloc (get_attr.value.s32list.count,
                                            sizeof(int32_t));

    ASSERT_TRUE (get_attr.value.s32list.list != NULL);
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->get_switch_attribute(1,&get_attr));
    ASSERT_EQ(get_attr.value.s32list.count, SAI_SWITCH_MAX_HASH_FIELDS);
    free(get_attr.value.s32list.list);

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_LAG_HASH_FIELDS;
    get_attr.value.s32list.count = SAI_SWITCH_MAX_HASH_FIELDS;
    get_attr.value.s32list.list =
                            (int32_t *) calloc (get_attr.value.s32list.count,
                                                sizeof(int32_t));

    ASSERT_TRUE (get_attr.value.s32list.list != NULL);
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->get_switch_attribute(1,&get_attr));
    ASSERT_EQ(get_attr.value.s32list.count, SAI_SWITCH_MAX_HASH_FIELDS);
    free(get_attr.value.s32list.list);
}


TEST_F(switchInit, hash_values_set_get)
{
    sai_attribute_t set_attr;
    sai_attribute_t get_attr;

/************************** ECMP/LAG HASH ALGO SET AND GET *************/
    memset(&get_attr, 0, sizeof(get_attr));
    memset(&set_attr, 0, sizeof(set_attr));
    set_attr.id = SAI_SWITCH_ATTR_LAG_HASH_ALGO;
    set_attr.value.u32 = SAI_HASH_CRC;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->set_switch_attribute (
                                    (const sai_attribute_t*)&set_attr));


    get_attr.id = SAI_SWITCH_ATTR_LAG_HASH_ALGO;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->get_switch_attribute(1,&get_attr));

    ASSERT_EQ(get_attr.value.u32, SAI_HASH_CRC);

    memset(&get_attr, 0, sizeof(get_attr));
    memset(&set_attr, 0, sizeof(set_attr));
    set_attr.id = SAI_SWITCH_ATTR_ECMP_HASH_ALGO;
    set_attr.value.u32 = SAI_HASH_XOR;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->set_switch_attribute
                                    ((const sai_attribute_t*)&set_attr));


    get_attr.id = SAI_SWITCH_ATTR_ECMP_HASH_ALGO;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->get_switch_attribute(1,&get_attr));
    ASSERT_EQ(get_attr.value.u32, (SAI_HASH_XOR));

/************************** ECMP/LAG HASH SEED SET AND GET *************/

    memset(&get_attr, 0, sizeof(get_attr));
    memset(&set_attr, 0, sizeof(set_attr));
    set_attr.id = SAI_SWITCH_ATTR_ECMP_HASH_SEED;
    set_attr.value.u32 = 12435;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->set_switch_attribute
                                    ((const sai_attribute_t*)&set_attr));


    get_attr.id = SAI_SWITCH_ATTR_ECMP_HASH_SEED;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->get_switch_attribute(1,&get_attr));
    ASSERT_EQ(get_attr.value.u32, 12435);


    memset(&get_attr, 0, sizeof(get_attr));
    memset(&set_attr, 0, sizeof(set_attr));
    set_attr.id = SAI_SWITCH_ATTR_LAG_HASH_SEED;
    set_attr.value.u32 = 56432;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->set_switch_attribute
                                    ((const sai_attribute_t*)&set_attr));

    get_attr.id = SAI_SWITCH_ATTR_LAG_HASH_SEED;
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->get_switch_attribute (1,&get_attr));
    ASSERT_EQ(get_attr.value.u32, 56432);

/************************** ECMP/LAG HASH FIELD SET AND GET *************/

    memset(&get_attr, 0, sizeof(get_attr));
    memset(&set_attr, 0, sizeof(set_attr));

    set_attr.id = SAI_SWITCH_ATTR_LAG_HASH_FIELDS;
    set_attr.value.s32list.count = 2;
    set_attr.value.s32list.list =
                            (int32_t *) calloc (set_attr.value.s32list.count,
                                                sizeof(int32_t));

    ASSERT_TRUE(set_attr.value.s32list.list != NULL);

    set_attr.value.s32list.list[0] = SAI_HASH_SOURCE_MAC;
    set_attr.value.s32list.list[1] = SAI_HASH_SRC_IP;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
                sai_switch_api_table->set_switch_attribute
                                    ((const sai_attribute_t*)&set_attr));
    free(set_attr.value.s32list.list);


    get_attr.id = SAI_SWITCH_ATTR_LAG_HASH_FIELDS;
    get_attr.value.s32list.count = 1;
    get_attr.value.s32list.list =
                            (int32_t *) calloc (get_attr.value.s32list.count,
                                                sizeof(int32_t));

    ASSERT_TRUE (get_attr.value.s32list.list != NULL);
    /** Negative test case to validate the overflow check */
    ASSERT_EQ(SAI_STATUS_BUFFER_OVERFLOW,
            sai_switch_api_table->get_switch_attribute(1,&get_attr));
    free(get_attr.value.s32list.list);

    get_attr.value.s32list.count = 2;
    get_attr.value.s32list.list =
        (int32_t *) calloc (get_attr.value.s32list.count,
                sizeof(int32_t));

    ASSERT_TRUE (get_attr.value.s32list.list != NULL);
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->get_switch_attribute(1,&get_attr));

    ASSERT_EQ(get_attr.value.s32list.count, 2);
    free(get_attr.value.s32list.list);

/************************** ECMP/LAG ALL HASH FIELDS SET AND GET *************/

    memset(&get_attr, 0, sizeof(get_attr));
    memset(&set_attr, 0, sizeof(set_attr));

    set_attr.id = SAI_SWITCH_ATTR_ECMP_HASH_FIELDS;
    set_attr.value.s32list.count = SAI_SWITCH_MAX_HASH_FIELDS;
    set_attr.value.s32list.list =
                            (int32_t *) calloc (set_attr.value.s32list.count,
                                                sizeof(int32_t));

    ASSERT_TRUE(set_attr.value.s32list.list != NULL);

    set_attr.value.s32list.list[0] = SAI_HASH_SOURCE_MAC;
    set_attr.value.s32list.list[1] = SAI_HASH_SRC_IP;
    set_attr.value.s32list.list[2] = SAI_HASH_IP_PROTOCOL;
    set_attr.value.s32list.list[3] = SAI_HASH_ETHERTYPE;
    set_attr.value.s32list.list[4] = SAI_HASH_L4_SOURCE_PORT;
    set_attr.value.s32list.list[5] = SAI_HASH_DEST_MAC;
    set_attr.value.s32list.list[6] = SAI_HASH_VLAN_ID;
    set_attr.value.s32list.list[7] = SAI_HASH_DST_IP;
    set_attr.value.s32list.list[8] = SAI_HASH_L4_DEST_PORT;
    set_attr.value.s32list.list[9] = SAI_HASH_IN_PORT;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->set_switch_attribute
                                        ((const sai_attribute_t*)&set_attr));
    free(set_attr.value.s32list.list);


    get_attr.id = SAI_SWITCH_ATTR_ECMP_HASH_FIELDS;
    get_attr.value.s32list.count = SAI_SWITCH_MAX_HASH_FIELDS;
    get_attr.value.s32list.list =
                            (int32_t *) calloc (get_attr.value.s32list.count,
                                                sizeof(int32_t));


    ASSERT_TRUE (get_attr.value.s32list.list != NULL);
    ASSERT_EQ(SAI_STATUS_SUCCESS,
            sai_switch_api_table->get_switch_attribute(1,&get_attr));
    ASSERT_EQ(get_attr.value.s32list.count, SAI_SWITCH_MAX_HASH_FIELDS);

    free(get_attr.value.s32list.list);

}

/*
 * Get the maximum LAG number and max LAG members
 */
TEST_F(switchInit, max_lag_attr)
{
    sai_attribute_t get_attr;

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_MAX_LAG_NUMBER;

    ASSERT_EQ(SAI_STATUS_INVALID_ATTRIBUTE_0,
              sai_switch_api_table->set_switch_attribute((const sai_attribute_t*)&get_attr));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1, &get_attr));

    EXPECT_NE(get_attr.value.u32, 0);

    LOG_PRINT("Maximum lag number in the switch is %d \r\n", get_attr.value.u32);

    memset(&get_attr, 0, sizeof(get_attr));
    get_attr.id = SAI_SWITCH_ATTR_MAX_LAG_MEMBERS;

    ASSERT_EQ(SAI_STATUS_INVALID_ATTRIBUTE_0,
              sai_switch_api_table->set_switch_attribute((const sai_attribute_t*)&get_attr));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1, &get_attr));

    EXPECT_NE(get_attr.value.u32, 0);

    LOG_PRINT("Maximum number of ports per LAG in the switch is %d \r\n", get_attr.value.u32);
}

TEST_F(switchInit, default_tc_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;
    uint32_t        tc = 7;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_SWITCH_ATTR_QOS_DEFAULT_TC;
    sai_attr_set.value.u32 = 7;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->set_switch_attribute(&sai_attr_set));

    sai_attr_get.id = SAI_SWITCH_ATTR_QOS_DEFAULT_TC;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1, &sai_attr_get));

    ASSERT_EQ(tc, sai_attr_get.value.u32);
}

TEST_F(switchInit, max_port_mtu_get)
{
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_get.id = SAI_SWITCH_ATTR_PORT_MAX_MTU;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1, &sai_attr_get));

    EXPECT_NE(sai_attr_get.value.u32, 0);

    LOG_PRINT("Maximum PORT mtu is %d \r\n", sai_attr_get.value.u32);
}

