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
*    sai_port_unit_test.cpp
*
* Abstract: 
*
*    SAI PORT UNIT TEST :- Covers the test cases for all Public API's in SAI
*    PORT module.
*
*/

#include "gtest/gtest.h"
#include "sai_port_breakout_test_utils.h"

extern "C" {
#include "sai.h"
#include "saiswitch.h"
#include "saiport.h"
#include "saitypes.h"

#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
}

#define SAI_MAX_PORTS  256

static uint32_t port_count = 0;
static sai_object_id_t port_list[SAI_MAX_PORTS] = {0};

/* global port id used for UT*/
static sai_object_id_t gport_id = 0;

#define SAI_PORT_CAP_SPEED_MAX 1

#define LOG_PRINT(msg, ...) \
    printf(msg, ##__VA_ARGS__)

/* Callback function stubs to be passed during SDK init. They callbacks will not be
 * validated as part of this UT.
 */

/*Port state change callback.
*/
static inline void sai_port_state_evt_callback(uint32_t count,
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
static inline void sai_port_evt_callback(uint32_t count,
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

            LOG_PRINT("PORT ADD EVENT FOR port 0x%"PRIx64" and total ports count is %d \r\n",
                      port_id, port_count);
        } else if(port_event == SAI_PORT_EVENT_DELETE) {
            port_count = 0; /* for testing breakout */
            LOG_PRINT("PORT DELETE EVENT for port 0x%"PRIx64" and total ports count is %d \r\n",
                      port_id, port_count);
        } else {
            LOG_PRINT("Invalid PORT EVENT for port 0x%"PRIx64" \r\n", port_id);
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
static inline void sai_fdb_evt_callback(uint32_t count, sai_fdb_event_notification_data_t *data)
{
    UNREFERENCED_PARAMETER(count);
    UNREFERENCED_PARAMETER(data);
}

/*Switch operstate callback.
*/
static inline void sai_switch_operstate_callback(sai_switch_oper_status_t switchstate)
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
 * API query is done while running the first test case and
 * the method table is stored in sai_port_api_table so
 * that its available for the rest of the test cases which
 * use the method table
 */
class portTest : public ::testing::Test
{
    public:
        void sai_port_supported_speed_check(sai_object_id_t port_id, uint32_t speed, bool *supported);
        bool sai_port_speed_set_get(sai_object_id_t port_id, uint32_t speed);
        bool sai_port_attr_set_get(sai_object_id_t port_id, sai_attribute_t *sai_attr);
        bool sai_internal_loopback_set_get(sai_object_id_t port_id,
                                           sai_port_internal_loopback_mode_t lb_mode);
        void sai_test_fdb_learn_mode(sai_port_fdb_learning_mode_t mode);

    protected:
        static void SetUpTestCase(void)
        {
            sai_switch_notification_t notification;
            memset (&notification, 0, sizeof(sai_switch_notification_t));

            /*
             * Query and populate the SAI Switch API Table.
             */
            EXPECT_EQ(SAI_STATUS_SUCCESS, sai_api_query
                      (SAI_API_SWITCH, (static_cast<void**>
                                        (static_cast<void*>(&sai_switch_api_table)))));

            ASSERT_TRUE(sai_switch_api_table != NULL);

            /*
             * Switch Initialization.
             * Fill in notification callback routines with stubs.
             */
            notification.on_switch_state_change = sai_switch_operstate_callback;
            notification.on_fdb_event = sai_fdb_evt_callback;
            notification.on_port_state_change = sai_port_state_evt_callback;
            notification.on_port_event = sai_port_evt_callback;
            notification.on_switch_shutdown_request = sai_switch_shutdown_callback;
            notification.on_packet_event = sai_packet_event_callback;

            ASSERT_TRUE(sai_switch_api_table->initialize_switch != NULL);
            EXPECT_TRUE(sai_switch_api_table->get_switch_attribute != NULL);

            ASSERT_EQ (SAI_STATUS_SUCCESS,
                       (sai_switch_api_table->initialize_switch (0, NULL, NULL,
                                                               &notification)));

            ASSERT_EQ(NULL,sai_api_query(SAI_API_PORT,
                                         (static_cast<void**>(static_cast<void*>(&sai_port_api_table)))));

            ASSERT_TRUE(sai_port_api_table != NULL);

            EXPECT_TRUE(sai_port_api_table->set_port_attribute != NULL);
            EXPECT_TRUE(sai_port_api_table->get_port_attribute != NULL);
            EXPECT_TRUE(sai_port_api_table->get_port_stats != NULL);

            gport_id = port_list[0];
        }

        static sai_switch_api_t *sai_switch_api_table;
        static sai_port_api_t* sai_port_api_table;
};

sai_switch_api_t* portTest ::sai_switch_api_table = NULL;
sai_port_api_t* portTest ::sai_port_api_table = NULL;

void portTest::sai_port_supported_speed_check(sai_object_id_t port_id, uint32_t speed, bool *supported)
{
    uint32_t count = 0;
    sai_status_t ret = SAI_STATUS_FAILURE;
    sai_attribute_t sai_attr_get;

    *supported = false;
    memset(&sai_attr_get, 0, sizeof(sai_attr_get));
    sai_attr_get.id = SAI_PORT_ATTR_SUPPORTED_SPEED;

    sai_attr_get.value.s32list.count = SAI_PORT_CAP_SPEED_MAX;
    sai_attr_get.value.s32list.list = (int32_t *)calloc (SAI_PORT_CAP_SPEED_MAX,
                                                         sizeof(int32_t));
    ASSERT_TRUE(sai_attr_get.value.s32list.list != NULL);

    do {
        ret = sai_port_api_table->get_port_attribute(port_id, 1, &sai_attr_get);

        /* Handles buffer overflow case, by reallocating required memory */
        if(ret == SAI_STATUS_BUFFER_OVERFLOW ) {
            free(sai_attr_get.value.s32list.list);
            LOG_PRINT("port 0x%"PRIx64" count of supported speeds is %d \r\n", port_id, sai_attr_get.value.s32list.count);
            sai_attr_get.value.s32list.list = (int32_t *)calloc (sai_attr_get.value.s32list.count,
                                                                 sizeof(int32_t));
            ASSERT_TRUE(sai_attr_get.value.s32list.list != NULL);
            ret = sai_port_api_table->get_port_attribute(port_id, 1, &sai_attr_get);
        }

        if(ret != SAI_STATUS_SUCCESS) {
            *supported = false;
            LOG_PRINT("port 0x%"PRIx64" supported speed get failed\n", port_id);
            break;
        }

        for (count = 0; count < sai_attr_get.value.s32list.count; count++) {
            if (speed == (uint32_t)sai_attr_get.value.s32list.list[count]) {
                *supported = true;
                break;
            }
        }

    } while(0);

    free(sai_attr_get.value.s32list.list);
}


/*
 * Validates Port type get for all the ports in the switch
 * UT PASS case: port should be logical
 */
TEST_F(portTest, logical_port_type_get)
{
    unsigned int port_idx = 0;

    for(port_idx = 0; port_idx < port_count; port_idx++) {
        /*@TODO Fix this later to not call the util API */
        EXPECT_TRUE(sai_port_type_logical(port_list[port_idx],
                                          sai_port_api_table));
    }
}

/*
 * Validates Port type get for CPU port in the switch
 * UT PASS case: port type should be CPU
 */
TEST_F(portTest, cpu_port_type_get)
{
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
    sai_attr_get.id = SAI_SWITCH_ATTR_CPU_PORT;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_switch_api_table->get_switch_attribute(1, &sai_attr_get));

    sai_object_id_t cpu_port = sai_attr_get.value.oid;

    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
    sai_attr_get.id = SAI_PORT_ATTR_TYPE;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(cpu_port, 1, &sai_attr_get));

    ASSERT_EQ(SAI_PORT_TYPE_CPU, sai_attr_get.value.s32);
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
    sai_status_t ret = SAI_STATUS_FAILURE;
    sai_attribute_t sai_attr_get;
    sai_attribute_t sai_attr_set;
    unsigned int port_idx = 0;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    /* Check port Oper state as UP by enabling the internal Loopback */
    for(port_idx = 0; port_idx < port_count; port_idx++) {
        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }
        /* Enable link admin state */
        sai_attr_set.id = SAI_PORT_ATTR_ADMIN_STATE;
        sai_attr_set.value.booldata = true;

        ret = sai_port_api_table->set_port_attribute(port_list[port_idx], &sai_attr_set);
        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        /* Loopback set to PHY */
        sai_attr_set.id = SAI_PORT_ATTR_INTERNAL_LOOPBACK;
        sai_attr_set.value.s32 = SAI_PORT_INTERNAL_LOOPBACK_PHY;

        ret = sai_port_api_table->set_port_attribute(port_list[port_idx], &sai_attr_set);
        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        sai_attr_get.id = SAI_PORT_ATTR_OPER_STATUS;
        ret = sai_port_api_table->get_port_attribute(port_list[port_idx], 1, &sai_attr_get);
        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }
        EXPECT_EQ(SAI_PORT_OPER_STATUS_UP, sai_attr_get.value.s32);
    }
}


/*
 * Set port attribute with given value and get the same attribute
 * @TODO: use this API in all applicable places
 */
bool portTest::sai_port_attr_set_get(sai_object_id_t port_id, sai_attribute_t *sai_attr)
{

    if(sai_port_api_table->set_port_attribute(port_id, sai_attr) != SAI_STATUS_SUCCESS) {
        return false;
    }

    memset(&sai_attr->value, 0, sizeof(sai_attribute_value_t));

    if(sai_port_api_table->get_port_attribute(port_id, 1, sai_attr) != SAI_STATUS_SUCCESS) {
        return false;
    }

    return true;
}

/*
 * Validates if the port is able to get/set a given port speed
 */
bool portTest::sai_port_speed_set_get(sai_object_id_t port_id, uint32_t speed)
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
    unsigned int port_idx = 0;
    uint32_t speed = 40000;
    bool supported = false;

    for(port_idx = 0; port_idx < port_count; port_idx++) {
        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }
        sai_port_supported_speed_check(port_list[port_idx], speed, &supported);
        if (supported == true) {
            EXPECT_TRUE(sai_port_speed_set_get(port_list[port_idx], speed));
        }
    }
}

/*
 * Validates 10G Port speed capability for all the valid ports in the switch
 * UT PASS case: port should be able to set get 10G speed
 */
TEST_F(portTest, speed_10g_set_get)
{
    unsigned int port_idx = 0;
    uint32_t speed = 10000;
    bool supported = false;

    for(port_idx = 0; port_idx < port_count; port_idx++) {
        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }

        sai_port_supported_speed_check(port_list[port_idx], speed, &supported);
        if (supported == true) {
            EXPECT_TRUE(sai_port_speed_set_get(port_list[port_idx], speed));
        }
    }
}

/*
 * Validates 1G Port speed capability for all the valid ports in the switch
 * UT PASS case: port should be able to set get 1G speed
 */
TEST_F(portTest, speed_1g_set_get)
{
    unsigned int port_idx = 0;
    uint32_t speed = 1000;
    bool supported = false;

    for(port_idx = 0; port_idx < port_count; port_idx++) {

        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }

        sai_port_supported_speed_check(port_list[port_idx], speed, &supported);
        if (supported == true) {
            EXPECT_TRUE(sai_port_speed_set_get(port_list[port_idx], speed));
        }
    }
}

/*
 * Validates if port admin state can be enabled for all the valid ports in the switch
 * UT PASS case: port admin state should get enabled
 */
TEST_F(portTest, admin_state_set_get)
{
    sai_status_t ret = SAI_STATUS_FAILURE;
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;
    unsigned int port_idx = 0;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    for(port_idx = 0; port_idx < port_count; port_idx++) {

        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }

        sai_attr_set.id = SAI_PORT_ATTR_ADMIN_STATE;
        sai_attr_set.value.booldata = true;

        ret = sai_port_api_table->set_port_attribute(port_list[port_idx], &sai_attr_set);
        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        sai_attr_get.id = SAI_PORT_ATTR_ADMIN_STATE;
        ret = sai_port_api_table->get_port_attribute(port_list[port_idx], 1, &sai_attr_get);
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

    sai_attr_set.id = SAI_PORT_ATTR_PORT_VLAN_ID;
    sai_attr_set.value.u16 = vlan_id;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_PORT_VLAN_ID;

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
bool portTest::sai_internal_loopback_set_get(sai_object_id_t port_id,
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
    unsigned int port_idx = 0;

    for(port_idx = 0; port_idx < port_count; port_idx++) {

        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }

        EXPECT_TRUE(sai_internal_loopback_set_get(port_list[port_idx], SAI_PORT_INTERNAL_LOOPBACK_PHY));
        EXPECT_TRUE(sai_internal_loopback_set_get(port_list[port_idx], SAI_PORT_INTERNAL_LOOPBACK_MAC));
        EXPECT_TRUE(sai_internal_loopback_set_get(port_list[port_idx], SAI_PORT_INTERNAL_LOOPBACK_NONE));
    }
}

/*
 * Validates if port autoneg can be enabled for all the valid ports
 * in the switch
 */
TEST_F(portTest, autoneg_state_set_get)
{
    sai_attribute_t sai_attr;
    unsigned int port_idx = 0;

    memset(&sai_attr, 0, sizeof(sai_attribute_t));

    for(port_idx = 0; port_idx < port_count; port_idx++) {

        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }

        EXPECT_TRUE(sai_internal_loopback_set_get(port_list[port_idx], SAI_PORT_INTERNAL_LOOPBACK_NONE));

        sai_attr.id = SAI_PORT_ATTR_AUTO_NEG_MODE;

        /* Enable auto-neg and check */
        sai_attr.value.booldata = true;
        EXPECT_TRUE(sai_port_attr_set_get(port_list[port_idx], &sai_attr));
        EXPECT_TRUE(sai_attr.value.booldata);

        /* Disable auto-neg and check */
        sai_attr.value.booldata = false;
        EXPECT_TRUE(sai_port_attr_set_get(port_list[port_idx], &sai_attr));
        EXPECT_FALSE(sai_attr.value.booldata);
    }
}

/*
 * Validates if port full duplex can be enabled for all the valid ports
 * in the switch
 */
TEST_F(portTest, fullduplex_state_set_get)
{
    sai_attribute_t sai_attr;
    unsigned int port_idx = 0;

    memset(&sai_attr, 0, sizeof(sai_attribute_t));

    for(port_idx = 0; port_idx < port_count; port_idx++) {

        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }

        sai_attr.id = SAI_PORT_ATTR_FULL_DUPLEX_MODE;

        /* Enable full duplex and check */
        sai_attr.value.booldata = true;
        EXPECT_TRUE(sai_port_attr_set_get(port_list[port_idx], &sai_attr));
        EXPECT_TRUE(sai_attr.value.booldata);

        /* Disable full duplex and check */
        sai_attr.value.booldata = false;
        EXPECT_TRUE(sai_port_attr_set_get(port_list[port_idx], &sai_attr));
        EXPECT_FALSE(sai_attr.value.booldata);
    }
}

/*
 * Validates and prints the HW lane list for all the valid logical ports in the switch
 * UT PASS case: hw lane list get API for all ports should succeed without error.
 */
TEST_F(portTest, hw_lane_list_get)
{
    unsigned int port_idx = 0, lane = 0;
    sai_status_t ret = SAI_STATUS_FAILURE;

    sai_attribute_t sai_attr_get;
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
    sai_attr_get.id = SAI_PORT_ATTR_HW_LANE_LIST;

    sai_attr_get.value.u32list.count = 1;
    sai_attr_get.value.u32list.list = (uint32_t *)calloc (1, sizeof(uint32_t *));

    ASSERT_TRUE(sai_attr_get.value.u32list.list != NULL);

    for(port_idx = 0; port_idx < port_count; port_idx++) {

        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }

        ret = sai_port_api_table->get_port_attribute(port_list[port_idx], 1, &sai_attr_get);

        /* Handles buffer overflow case, by reallocating required memory */
        if(ret == SAI_STATUS_BUFFER_OVERFLOW ) {
            free(sai_attr_get.value.u32list.list);
            sai_attr_get.value.u32list.list = (uint32_t *)calloc (sai_attr_get.value.u32list.count,
                                                                  sizeof(uint32_t *));
            ret = sai_port_api_table->get_port_attribute(port_list[port_idx], 1, &sai_attr_get);
        }

        if(ret != SAI_STATUS_SUCCESS) {
            free(sai_attr_get.value.u32list.list);
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        for(lane = 0; lane < sai_attr_get.value.u32list.count; lane++ ) {
            LOG_PRINT("port 0x%"PRIx64" lane %d is %d \r\n", port_list[port_idx], lane,
                      sai_attr_get.value.u32list.list[lane]);
        }
    }
    free(sai_attr_get.value.u32list.list);
}

/*
 * Validates and prints the supported breakout modes for all the valid logical ports in the switch
 * UT PASS case: supported breakout mode get API for all ports should succeed without error.
 */
TEST_F(portTest, supported_breakout_mode_get)
{
    unsigned int port_idx = 0, count = 0 ;
    sai_status_t ret = SAI_STATUS_FAILURE;

    sai_attribute_t sai_attr_get;
    memset(&sai_attr_get, 0, sizeof(sai_attr_get));
    sai_attr_get.id = SAI_PORT_ATTR_SUPPORTED_BREAKOUT_MODE;

    sai_attr_get.value.s32list.count = SAI_PORT_BREAKOUT_MODE_MAX;
    sai_attr_get.value.s32list.list = (int32_t *)calloc (SAI_PORT_BREAKOUT_MODE_MAX,
                                                         sizeof(int32_t *));
    ASSERT_TRUE(sai_attr_get.value.s32list.list != NULL);

    for(port_idx = 0; port_idx < port_count; port_idx++) {

        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }

        ret = sai_port_api_table->get_port_attribute(port_list[port_idx], 1, &sai_attr_get);

        /* Handles buffer overflow case, by reallocating required memory */
        if(ret == SAI_STATUS_BUFFER_OVERFLOW ) {
            free(sai_attr_get.value.s32list.list);
            sai_attr_get.value.s32list.list = (int32_t *)calloc (sai_attr_get.value.s32list.count,
                                                                 sizeof(int32_t *));
            ret = sai_port_api_table->get_port_attribute(port_list[port_idx], 1, &sai_attr_get);
        }

        if(ret != SAI_STATUS_SUCCESS) {
            free(sai_attr_get.value.s32list.list);
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        for(count = 0; count < sai_attr_get.value.s32list.count; count++ ) {
            LOG_PRINT("port 0x%"PRIx64" mode %d is %d \r\n", port_list[port_idx], count,
                      sai_attr_get.value.s32list.list[count]);
        }
    }

    free(sai_attr_get.value.u32list.list);
}

/*
 * Validates and prints the current breakout modes for all the valid logical ports in the switch
 * UT PASS case: current breakout mode get API for all ports should succeed without error.
 */
TEST_F(portTest, current_breakout_mode_get)
{
    unsigned int port_idx = 0;

    sai_attribute_t sai_attr_get;
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
    sai_attr_get.id = SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE;

    for(port_idx = 0; port_idx < port_count; port_idx++) {

        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_port_api_table->get_port_attribute(port_list[port_idx], 1, &sai_attr_get));

        LOG_PRINT("port 0x%"PRIx64" current breakout mode %d \r\n", port_list[port_idx], sai_attr_get.value.s32);
    }
}

void portTest::sai_test_fdb_learn_mode(sai_port_fdb_learning_mode_t mode)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;
    sai_status_t rc = SAI_STATUS_FAILURE;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_FDB_LEARNING;
    sai_attr_get.id = SAI_PORT_ATTR_FDB_LEARNING;

    /*Test set and get various possible port learn modes*/

    sai_attr_set.value.s32 = mode;
    rc = sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set);

    EXPECT_EQ(SAI_STATUS_SUCCESS, rc);

    if(rc == SAI_STATUS_SUCCESS) {
        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));
        ASSERT_EQ(mode, sai_attr_get.value.s32);
    }
}
/*
 * Validates if the port fdb learning mode can be set to Learning
 * UT PASS case: FDB learning mode should get enabled on the port
 */
TEST_F(portTest, fdb_learning_mode_set_get)
{
    sai_test_fdb_learn_mode(SAI_PORT_LEARN_MODE_DROP);
    sai_test_fdb_learn_mode(SAI_PORT_LEARN_MODE_DISABLE);
    sai_test_fdb_learn_mode(SAI_PORT_LEARN_MODE_CPU_TRAP);
    sai_test_fdb_learn_mode(SAI_PORT_LEARN_MODE_CPU_LOG);
    sai_test_fdb_learn_mode(SAI_PORT_LEARN_MODE_HW);
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

    sai_attr_set.id = SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID;

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

    sai_attr_set.id = SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID;

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

    sai_attr_set.id = SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID;
    sai_attr_set.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID;

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
    sai_status_t ret = SAI_STATUS_FAILURE;
    unsigned int port_idx = 0, mtu = 1000;

    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    for(port_idx = 0; port_idx < port_count; port_idx++) {

        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }

        sai_attr_set.id = SAI_PORT_ATTR_MTU;
        sai_attr_set.value.u32 = mtu;
        ret = sai_port_api_table->set_port_attribute(port_list[port_idx], &sai_attr_set);
        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        sai_attr_get.id = SAI_PORT_ATTR_MTU;
        ret = sai_port_api_table->get_port_attribute(port_list[port_idx], 1, &sai_attr_get);
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

TEST_F(portTest, fdb_learn_limit_violation_set_get)
{
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

    sai_attr_set.id = SAI_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION;
    sai_attr_set.value.s32 = SAI_PACKET_ACTION_DROP;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

    ASSERT_EQ(SAI_PACKET_ACTION_DROP, sai_attr_get.value.s32);
}

/*
 * Set all supported port media type one by one and verify configured value
 * is set by reading back the media type.
 * UT PASS case: Media type set to configured type
 */
TEST_F(portTest, media_type_set_get)
{
    sai_int32_t media_type = 0;
    sai_attribute_t sai_attr_set;
    sai_attribute_t sai_attr_get;

    for(media_type = SAI_PORT_MEDIA_TYPE_NOT_PRESENT;
        media_type <= SAI_PORT_MEDIA_TYPE_SFP_COPPER; media_type++) {
        memset(&sai_attr_set, 0, sizeof(sai_attribute_t));
        memset(&sai_attr_get, 0, sizeof(sai_attribute_t));

        sai_attr_set.id = SAI_PORT_ATTR_MEDIA_TYPE;
        sai_attr_set.value.s32 = media_type;

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set));

        sai_attr_get.id = SAI_PORT_ATTR_MEDIA_TYPE;

        ASSERT_EQ(SAI_STATUS_SUCCESS,
                  sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get));

        ASSERT_EQ(media_type, sai_attr_get.value.s32);
    }
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
        counter <= SAI_PORT_STAT_ETHER_OUT_PKTS_1024_TO_1518_OCTETS; counter++)
    {
        counter_ids[0] = (sai_port_stat_counter_t)counter;
        status = sai_port_api_table->get_port_stats(gport_id, counter_ids, 1, counters);

        if(status == SAI_STATUS_SUCCESS) {
            LOG_PRINT("port 0x%"PRIx64" stat id %d value is %ld \r\n", gport_id, counter_ids[0], counters[0]);
        } else if (status == (SAI_STATUS_ATTR_NOT_SUPPORTED_0 + counter)) {
            LOG_PRINT("port 0x%"PRIx64" stat id %d not implemented \r\n", gport_id, counter_ids[0]);
        }

        EXPECT_EQ(SAI_STATUS_SUCCESS, status);
    }
}

/*
 * Clear port's all statistic counters one by one
 */
TEST_F(portTest, port_stat_clear)
{
    int32_t counter = 0;
    sai_port_stat_counter_t counter_ids[1];
    sai_status_t status = SAI_STATUS_FAILURE;

    for(counter = SAI_PORT_STAT_IF_IN_OCTETS;
        counter <= SAI_PORT_STAT_ETHER_OUT_PKTS_1024_TO_1518_OCTETS; counter++)
    {
        counter_ids[0] = (sai_port_stat_counter_t)counter;
        status = sai_port_api_table->clear_port_stats(gport_id, counter_ids, 1);

        if(status == SAI_STATUS_SUCCESS) {
            LOG_PRINT("port 0x%"PRIx64" stat id %d \r\n", gport_id, counter_ids[0]);
        } else if (status == (SAI_STATUS_ATTR_NOT_SUPPORTED_0 + counter)) {
            LOG_PRINT("port 0x%"PRIx64" stat id %d not implemented \r\n", gport_id, counter_ids[0]);
        }

        EXPECT_EQ(SAI_STATUS_SUCCESS, status);
    }
}

/*
 * Clear port's all statistic counters at one shot
 */
TEST_F(portTest, port_all_stat_clear)
{
    sai_status_t status = SAI_STATUS_FAILURE;
    unsigned int port_idx = 0;

    for(port_idx = 0; port_idx < port_count; port_idx++) {
        if(sai_port_type_logical(port_list[port_idx], sai_port_api_table) != true) {
            continue;
        }

        status = sai_port_api_table->clear_port_all_stats(gport_id);
        if (status == SAI_STATUS_SUCCESS) {
            LOG_PRINT("port 0x%"PRIx64" stat cleared\r\n", gport_id);
        } else {
            LOG_PRINT("port 0x%"PRIx64" stat clear failed %d\r\n", gport_id, status);
        }
        EXPECT_EQ(SAI_STATUS_SUCCESS, status);
    }
}

/*
 * Set the breakout mode to 4 lanes for a port and read back the current breakout mode.
 * Revert back to breakout mode to 1 lane mode and read back the current breakout mode.
 * Note: 
 * 1. Only first port in port_list is used for testing breakout.
 * 2. During delete port event, port list is made empty for testing breakin to 
 * add only new breakout ports in port list. 
 */
TEST_F(portTest, attr_breakout_mode_set_get)
{
    sai_status_t ret = SAI_STATUS_FAILURE;

    /*set the breakout mode*/
    ret = sai_port_break_out_mode_set(sai_switch_api_table,
                                      sai_port_api_table,
                                      port_list[0],
                                      SAI_PORT_BREAKOUT_MODE_4_LANE);
    ASSERT_EQ(ret, SAI_STATUS_SUCCESS);

    /*get the mode and check*/
    ret = sai_port_break_out_mode_get(sai_port_api_table,
                                      port_list[0],
                                      SAI_PORT_BREAKOUT_MODE_4_LANE);
    ASSERT_EQ(ret, SAI_STATUS_SUCCESS);

    /*set the breakin mode*/
    ret = sai_port_break_in_mode_set(sai_switch_api_table,
                                     sai_port_api_table,
                                     4,
                                     port_list,
                                     SAI_PORT_BREAKOUT_MODE_1_LANE);
    ASSERT_EQ(ret, SAI_STATUS_SUCCESS);

    /*get the mode and check*/
    ret = sai_port_break_out_mode_get(sai_port_api_table,
                                      port_list[0],
                                      SAI_PORT_BREAKOUT_MODE_1_LANE);
    ASSERT_EQ(ret, SAI_STATUS_SUCCESS);
}

/* Test breakout mode rollback when breakout set API fails. NPU expects the port link
 * Admin state to be set to false before applying breakout mode, otherwise breakout mode
 * set fails. On Set failure, it should get restored to its earlier breakout mode */
TEST_F(portTest, attr_breakout_mode_rollback_set_get)
{
    sai_attribute_t set_attr;
    sai_attribute_t get_attr;
    sai_status_t ret = SAI_STATUS_FAILURE;
    sai_port_breakout_mode_type_t cur_breakmode = SAI_PORT_BREAKOUT_MODE_1_LANE;

    memset(&set_attr, 0, sizeof(set_attr));
    memset(&get_attr, 0, sizeof(get_attr));

    get_attr.id = SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
	sai_port_api_table->get_port_attribute(port_list[0],
                                           1, &get_attr));
    cur_breakmode = (sai_port_breakout_mode_type_t) get_attr.value.s32;

    /* Enable port link admin state */
    set_attr.id = SAI_PORT_ATTR_ADMIN_STATE;
    set_attr.value.booldata = true;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
	sai_port_api_table->set_port_attribute(port_list[0],
                                           &set_attr));
    set_attr.id = SAI_SWITCH_ATTR_PORT_BREAKOUT;
    set_attr.value.portbreakout.breakout_mode = SAI_PORT_BREAKOUT_MODE_4_LANE;

    set_attr.value.portbreakout.port_list.count = 1;
    set_attr.value.portbreakout.port_list.list =
        (sai_object_id_t *)calloc (1, sizeof(sai_object_id_t *));

    ASSERT_TRUE(set_attr.value.portbreakout.port_list.list != NULL);

    set_attr.value.portbreakout.port_list.list[0] = port_list[0];

    /* Switch set is expected to fail as port Admin state is UP */
    ret = sai_switch_api_table->set_switch_attribute((const sai_attribute_t*)&set_attr);
    if(ret == SAI_STATUS_SUCCESS) {
        free(set_attr.value.portbreakout.port_list.list);
        ASSERT_NE(SAI_STATUS_SUCCESS, ret);
    }

    get_attr.id = SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE;

    ret = sai_port_api_table->get_port_attribute(port_list[0], 1, &get_attr);
    if(ret != SAI_STATUS_SUCCESS) {
        free(set_attr.value.portbreakout.port_list.list);
        ASSERT_EQ(SAI_STATUS_SUCCESS, ret);
    }

    /* Though breakout mode set fails, it should get restored to its earlier mode */
    ASSERT_EQ(cur_breakmode, get_attr.value.s32);
}

