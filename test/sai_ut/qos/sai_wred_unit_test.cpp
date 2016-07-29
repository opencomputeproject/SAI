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
*    sai_wred_unit_test.cpp
*     
* Abstract:
* 
*    SAI WRED UNIT TEST :- Covers the test cases for validating all 
*    public APIs in WRED module.
*
*************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gtest/gtest.h"

extern "C" {
#include "sai_qos_unit_test_utils.h"
#include "sai.h"
#include "saitypes.h"
#include "saistatus.h"
#include "saiswitch.h"
#include <inttypes.h>
}
#define SAI_MAX_QUEUES 40
static const unsigned int default_port = 0;
static sai_object_id_t queue_list[SAI_MAX_QUEUES] = {0};
static unsigned int max_queues = 0;

#define LOG_PRINT(msg, ...) \
    printf(msg, ##__VA_ARGS__)


/*
 * API query is done while running the first test case and
 * the method table is stored in sai_switch_api_table so
 * that its available for the rest of the test cases which
 * use the method table
 */
class wred : public ::testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            ASSERT_EQ(SAI_STATUS_SUCCESS,sai_api_query(SAI_API_SWITCH,
                                         (static_cast<void**>(static_cast<void*>(&sai_switch_api_table)))));

            ASSERT_TRUE(sai_switch_api_table != NULL);

            EXPECT_TRUE(sai_switch_api_table->initialize_switch != NULL);
            EXPECT_TRUE(sai_switch_api_table->shutdown_switch != NULL);
            EXPECT_TRUE(sai_switch_api_table->connect_switch != NULL);
            EXPECT_TRUE(sai_switch_api_table->disconnect_switch != NULL);
            EXPECT_TRUE(sai_switch_api_table->set_switch_attribute != NULL);
            EXPECT_TRUE(sai_switch_api_table->get_switch_attribute != NULL);

            sai_switch_notification_t notification;

            notification.on_switch_state_change = sai_switch_operstate_callback;
            notification.on_fdb_event = sai_fdb_evt_callback;
            notification.on_port_state_change = sai_port_state_evt_callback;
            notification.on_switch_shutdown_request = sai_switch_shutdown_callback;
            notification.on_port_event = sai_port_evt_callback;
            notification.on_packet_event = sai_packet_event_callback;

            if(sai_switch_api_table->initialize_switch) {
                sai_switch_api_table->initialize_switch(0,NULL,NULL,&notification);
            }

            ASSERT_EQ(SAI_STATUS_SUCCESS,sai_api_query(SAI_API_WRED,
                                         (static_cast<void**>(static_cast<void*>(&sai_wred_api_table)))));

            ASSERT_TRUE(sai_wred_api_table != NULL);

            EXPECT_TRUE(sai_wred_api_table->create_wred_profile != NULL);
            EXPECT_TRUE(sai_wred_api_table->remove_wred_profile  != NULL);
            EXPECT_TRUE(sai_wred_api_table->set_wred_attribute != NULL);
            EXPECT_TRUE(sai_wred_api_table->get_wred_attribute != NULL);

            ASSERT_EQ(NULL,sai_api_query(SAI_API_QUEUE,
                                         (static_cast<void**>(static_cast<void*>(&sai_queue_api_table)))));

            ASSERT_TRUE(sai_queue_api_table != NULL);

            EXPECT_TRUE(sai_queue_api_table->set_queue_attribute != NULL);
            EXPECT_TRUE(sai_queue_api_table->get_queue_attribute != NULL);
            EXPECT_TRUE(sai_queue_api_table->get_queue_stats != NULL);

            ASSERT_EQ(NULL,sai_api_query(SAI_API_PORT,
                                         (static_cast<void**>(static_cast<void*>(&sai_port_api_table)))));

            ASSERT_TRUE(sai_port_api_table != NULL);

            EXPECT_TRUE(sai_port_api_table->set_port_attribute != NULL);
            EXPECT_TRUE(sai_port_api_table->get_port_attribute != NULL);
            EXPECT_TRUE(sai_port_api_table->get_port_stats != NULL);

            ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_list_get());

        }

        static sai_status_t sai_queue_list_get();
        static sai_switch_api_t* sai_switch_api_table;
        static sai_wred_api_t* sai_wred_api_table;
        static sai_queue_api_t* sai_queue_api_table;
        static sai_port_api_t* sai_port_api_table;
        static const unsigned int test_port_id = 30;
};

sai_switch_api_t* wred ::sai_switch_api_table = NULL;
sai_wred_api_t* wred ::sai_wred_api_table = NULL;
sai_queue_api_t* wred ::sai_queue_api_table = NULL;
sai_port_api_t* wred ::sai_port_api_table = NULL;

sai_status_t wred::sai_queue_list_get()
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t    attr  = {0};
    unsigned int       index = 0;

    attr.id = SAI_PORT_ATTR_QOS_NUMBER_OF_QUEUES;

    sai_rc = sai_port_api_table->get_port_attribute (sai_qos_port_id_get(default_port), 1, &attr);
    if(sai_rc != SAI_STATUS_SUCCESS) {
        printf("Failed to get queue count. Error code:%d\n", sai_rc);
        return sai_rc;
    } else {
         max_queues = attr.value.u32;
         printf("Max queues on port 0x%"PRIx64": %d. \n", sai_qos_port_id_get(default_port), max_queues);
    }
    attr.id = SAI_PORT_ATTR_QOS_QUEUE_LIST;

    attr.value.objlist.count = max_queues;
    attr.value.objlist.list = &queue_list[0];

    if (attr.value.objlist.list == NULL) {

        printf ("%s(): Memory alloc failed for attribute list.\n", __FUNCTION__);

        return SAI_STATUS_NO_MEMORY;
    }

    sai_rc = sai_port_api_table->get_port_attribute (sai_qos_port_id_get(default_port), 1, &attr);

    if (sai_rc == SAI_STATUS_BUFFER_OVERFLOW) {
        printf("Requested queue count %d, Max queues %d on port :0x%"PRIx64".\n",
               max_queues, attr.value.objlist.count, sai_qos_port_id_get(default_port));
        return SAI_STATUS_FAILURE;
    }

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("SAI Port Get queue id list failed with error: %d.\n", sai_rc);
        return sai_rc;
    } else {

        printf ("SAI port Get queue id list success for Port Id: 0x%"PRIx64".\n",
                sai_qos_port_id_get(default_port));

        printf ("SAI Port 0x%"PRIx64" Queue List.\n", sai_qos_port_id_get(default_port));

        for (index = 0; index < attr.value.objlist.count; ++index) {
            printf ("SAI Queue index %d QOID 0x%"PRIx64".\n",
                    index , queue_list[index]);
        }
    }

    return sai_rc;
}

/*
 * Create,set a wred green profile.
 * Checks for mandatory attribute missing case.
 */
TEST_F(wred, wred_create_set)
{
    sai_attribute_t new_attr_list[5];
    sai_attribute_t set_attr;
    sai_object_id_t wred_id1 = SAI_NULL_OBJECT_ID;
    sai_object_id_t queue_id = queue_list[0];
    unsigned int attr_count = 0;

    attr_count = 0;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_GREEN_ENABLE;
    new_attr_list[attr_count].value.booldata = true;

    attr_count ++;
    ASSERT_EQ(SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_wred_api_table->create_wred_profile
              (&wred_id1, attr_count, (const sai_attribute_t *)new_attr_list));

    attr_count = 0;
    new_attr_list[attr_count].id = SAI_WRED_ATTR_GREEN_ENABLE;
    new_attr_list[attr_count].value.booldata = true;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_GREEN_MIN_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 400;

    attr_count ++;

    ASSERT_EQ(SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_wred_api_table->create_wred_profile
              (&wred_id1, attr_count, (const sai_attribute_t *)new_attr_list));

    new_attr_list[attr_count].id = SAI_WRED_ATTR_GREEN_ENABLE;
    new_attr_list[attr_count].value.booldata = true;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_GREEN_MIN_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 400;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_GREEN_MAX_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 800;

    attr_count ++;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->create_wred_profile
              (&wred_id1, attr_count, (const sai_attribute_t *)new_attr_list));

    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = wred_id1;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_WRED_ATTR_GREEN_DROP_PROBABILITY;
    set_attr.value.u32 = 56;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->set_wred_attribute
              (wred_id1,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_WRED_ATTR_WEIGHT;
    set_attr.value.u32 = 13;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->set_wred_attribute
              (wred_id1,(const sai_attribute_t *)&set_attr));

    ASSERT_EQ(SAI_STATUS_OBJECT_IN_USE, sai_wred_api_table->remove_wred_profile
              (wred_id1));

    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = SAI_NULL_OBJECT_ID;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id,(const sai_attribute_t *)&set_attr));

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->remove_wred_profile
              (wred_id1));
}

/*
 * Create,get,set a yellow wred profile.
 */
TEST_F(wred, yellow_profile_create)
{
    sai_attribute_t new_attr_list[5];
    sai_attribute_t set_attr;
    sai_object_id_t wred_id1 = SAI_NULL_OBJECT_ID;
    sai_object_id_t queue_id1 = queue_list[0];
    unsigned int attr_count = 0;

    attr_count = 0;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_YELLOW_ENABLE;
    new_attr_list[attr_count].value.booldata = true;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_YELLOW_MIN_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 400;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_YELLOW_MAX_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 800;

    attr_count ++;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->create_wred_profile
              (&wred_id1, attr_count, (const sai_attribute_t *)new_attr_list));

    set_attr.id = SAI_WRED_ATTR_YELLOW_DROP_PROBABILITY;
    set_attr.value.u32 = 56;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->set_wred_attribute
              (wred_id1,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_WRED_ATTR_WEIGHT;
    set_attr.value.u32 = 10;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->set_wred_attribute
              (wred_id1,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = wred_id1;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id1,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_WRED_ATTR_ECN_MARK_ENABLE;
    set_attr.value.booldata = true;
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->set_wred_attribute
              (wred_id1,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_WRED_ATTR_ECN_MARK_ENABLE;
    set_attr.value.booldata = false;
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->set_wred_attribute
              (wred_id1,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = SAI_NULL_OBJECT_ID;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id1,(const sai_attribute_t *)&set_attr));

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->remove_wred_profile
              (wred_id1));
}

/*
 * Create,get,set red wred profile.
 */
TEST_F(wred, red_profile_create)
{
    sai_attribute_t new_attr_list[5];
    sai_attribute_t set_attr;
    sai_object_id_t wred_id1 = SAI_NULL_OBJECT_ID;
    sai_object_id_t queue_id = queue_list[0];
    unsigned int attr_count = 0;

    attr_count = 0;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_RED_ENABLE;
    new_attr_list[attr_count].value.booldata = true;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_RED_MIN_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 500;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_RED_MAX_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 800;

    attr_count ++;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->create_wred_profile
              (&wred_id1, attr_count, (const sai_attribute_t *)new_attr_list));

    set_attr.id = SAI_WRED_ATTR_RED_DROP_PROBABILITY;
    set_attr.value.u32 = 34;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->set_wred_attribute
              (wred_id1,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_WRED_ATTR_WEIGHT;
    set_attr.value.u32 = 10;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->set_wred_attribute
              (wred_id1,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_WRED_ATTR_RED_ENABLE;
    set_attr.value.booldata = false;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->set_wred_attribute
              (wred_id1,(const sai_attribute_t *)&set_attr));


    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = wred_id1;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = SAI_NULL_OBJECT_ID;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id,(const sai_attribute_t *)&set_attr));

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->remove_wred_profile
              (wred_id1));
}

/*
 * Create 2 profiles and apply one profile.
 * Replace with another profile.
 * Remove from queue and remove the profiles also.
 */
TEST_F(wred, two_profiles)
{
    sai_attribute_t new_attr_list[5];
    sai_attribute_t set_attr;
    sai_object_id_t wred_id1 = SAI_NULL_OBJECT_ID;
    sai_object_id_t wred_id2 = SAI_NULL_OBJECT_ID;
    sai_object_id_t queue_id = queue_list[0];
    sai_object_id_t queue_id1 = queue_list[1];
    unsigned int attr_count = 0;

    attr_count = 0;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_RED_ENABLE;
    new_attr_list[attr_count].value.booldata = true;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_RED_MIN_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 500;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_RED_MAX_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 800;

    attr_count ++;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->create_wred_profile
              (&wred_id1, attr_count, (const sai_attribute_t *)new_attr_list));

    attr_count = 0;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_RED_ENABLE;
    new_attr_list[attr_count].value.booldata = true;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_RED_MIN_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 45;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_RED_MAX_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 200;

    attr_count ++;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->create_wred_profile
              (&wred_id2, attr_count, (const sai_attribute_t *)new_attr_list));

    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = wred_id1;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id,(const sai_attribute_t *)&set_attr));


    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = wred_id2;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id,(const sai_attribute_t *)&set_attr));


    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = wred_id1;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id1,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = SAI_NULL_OBJECT_ID;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id,(const sai_attribute_t *)&set_attr));

    ASSERT_EQ(SAI_STATUS_OBJECT_IN_USE, sai_wred_api_table->remove_wred_profile
              (wred_id1));

    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = SAI_NULL_OBJECT_ID;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id1,(const sai_attribute_t *)&set_attr));

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->remove_wred_profile
              (wred_id1));
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->remove_wred_profile
              (wred_id2));
}

/*
 * Create,get,set a wred profile.
 */
TEST_F(wred, create_set_get)
{
    sai_attribute_t new_attr_list[12];
    sai_attribute_t set_attr;
    sai_attribute_t get_attr[5];
    sai_object_id_t wred_id1 = SAI_NULL_OBJECT_ID;
    sai_object_id_t queue_id = queue_list[0];
    sai_object_id_t queue_id1 = queue_list[1];
    unsigned int attr_count = 0;

    attr_count = 0;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_RED_ENABLE;
    new_attr_list[attr_count].value.booldata = true;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_RED_MIN_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 500;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_RED_MAX_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 800;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_YELLOW_ENABLE;
    new_attr_list[attr_count].value.booldata = true;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_YELLOW_MIN_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 1500;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_YELLOW_MAX_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 2000;

    attr_count ++;
    new_attr_list[attr_count].id = SAI_WRED_ATTR_GREEN_ENABLE;
    new_attr_list[attr_count].value.booldata = true;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_GREEN_MIN_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 5000;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_GREEN_MAX_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 22000;
    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_ECN_MARK_ENABLE;
    new_attr_list[attr_count].value.booldata = true;
    attr_count ++;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->create_wred_profile
              (&wred_id1, attr_count, (const sai_attribute_t *)new_attr_list));


    attr_count = 0;
    get_attr[attr_count].id = SAI_WRED_ATTR_GREEN_MAX_THRESHOLD;
    attr_count ++;

    get_attr[attr_count].id = SAI_WRED_ATTR_GREEN_MIN_THRESHOLD;
    attr_count ++;

    get_attr[attr_count].id = SAI_WRED_ATTR_WEIGHT;
    attr_count ++;

    ASSERT_EQ(SAI_STATUS_SUCCESS,sai_wred_api_table->
              get_wred_attribute(wred_id1, attr_count,
                                    get_attr));

    LOG_PRINT("Green max is %x green min %x weight %x\r\n",get_attr[0].value.u32,
              get_attr[1].value.u32, get_attr[2].value.u32);


    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = wred_id1;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = wred_id1;


    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id1,(const sai_attribute_t *)&set_attr));



    set_attr.id = SAI_WRED_ATTR_WEIGHT;
    set_attr.value.u32 = 10;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->set_wred_attribute
              (wred_id1,(const sai_attribute_t *)&set_attr));


    attr_count = 0;
    get_attr[attr_count].id = SAI_WRED_ATTR_RED_MAX_THRESHOLD;
    attr_count ++;

    get_attr[attr_count].id = SAI_WRED_ATTR_YELLOW_DROP_PROBABILITY;
    attr_count ++;

    get_attr[attr_count].id = SAI_WRED_ATTR_WEIGHT;
    attr_count ++;

    ASSERT_EQ(SAI_STATUS_SUCCESS,sai_wred_api_table->
              get_wred_attribute(wred_id1, attr_count,
                                    get_attr));
    LOG_PRINT("Red max is %x yellowdrop  %x weight %x\r\n",get_attr[0].value.u32,
                      get_attr[1].value.u32, get_attr[2].value.u32);

    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = SAI_NULL_OBJECT_ID;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id,(const sai_attribute_t *)&set_attr));

    set_attr.id = SAI_QUEUE_ATTR_WRED_PROFILE_ID;
    set_attr.value.oid = SAI_NULL_OBJECT_ID;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
              (queue_id1,(const sai_attribute_t *)&set_attr));

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->remove_wred_profile
              (wred_id1));
}

/*
 * Apply a wred profile on port.
 * For now its not supported.
 */
TEST_F(wred, wred_on_port)
{
    sai_attribute_t new_attr_list[5];
    sai_attribute_t set_attr;
    sai_object_id_t wred_id1 = SAI_NULL_OBJECT_ID;
    unsigned int attr_count = 0;

    attr_count = 0;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_YELLOW_ENABLE;
    new_attr_list[attr_count].value.booldata = true;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_YELLOW_MIN_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 400;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_YELLOW_MAX_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 800;

    attr_count ++;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_wred_api_table->create_wred_profile
              (&wred_id1, attr_count, (const sai_attribute_t *)new_attr_list));

    set_attr.id = SAI_PORT_ATTR_QOS_WRED_PROFILE_ID;
    set_attr.value.oid = wred_id1;

    ASSERT_EQ(SAI_STATUS_NOT_SUPPORTED,sai_port_api_table->set_port_attribute
              (sai_qos_port_id_get(test_port_id), (const sai_attribute_t *)&set_attr));
}

/*
 * Test case for checking invalid number of attributes for porfile create.
 */
TEST_F(wred, invalid_create_params)
{
    sai_attribute_t new_attr_list[5];
    sai_object_id_t wred_id1 = SAI_NULL_OBJECT_ID;
    unsigned int attr_count = 0;

    attr_count = 0;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_YELLOW_ENABLE;
    new_attr_list[attr_count].value.booldata = true;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_YELLOW_MIN_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 400;

    attr_count ++;

    new_attr_list[attr_count].id = SAI_WRED_ATTR_YELLOW_MAX_THRESHOLD;
    new_attr_list[attr_count].value.u32 = 800;

    attr_count ++;

    ASSERT_EQ(SAI_STATUS_INVALID_PARAMETER, sai_wred_api_table->create_wred_profile
              (&wred_id1, 0, (const sai_attribute_t *)new_attr_list));

    ASSERT_EQ(SAI_STATUS_INVALID_PARAMETER, sai_wred_api_table->create_wred_profile
              (&wred_id1, 15, (const sai_attribute_t *)new_attr_list));
}


int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

