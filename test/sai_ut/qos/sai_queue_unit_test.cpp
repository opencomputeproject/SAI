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
*    sai_queue_unit_test.cpp
*     
* Abstract:
* 
*    SAI QUEUE UNIT TEST :- Covers the test cases for validating all 
*    public APIs in SAI QUEUE module.
*
*************************************************************************/


#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stdarg.h"
#include "gtest/gtest.h"

extern "C" {
#include "sai_qos_unit_test_utils.h"
#include "sai.h"
#include "saistatus.h"
#include <inttypes.h>
#include <string.h>
}

static const unsigned int default_port = 0;
static sai_object_id_t  default_port_id = 0;
static sai_object_id_t  cpu_port_id = 0;

/* SAI initialization */
void SetUpTestCase (void)
{
    sai_switch_notification_t notification;
    memset (&notification, 0, sizeof(sai_switch_notification_t));

    /*
     * Query and populate the SAI Switch API Table.
     */
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_SWITCH, (static_cast<void**>
                                 (static_cast<void*>(&p_sai_switch_api_table)))));

    ASSERT_TRUE (p_sai_switch_api_table != NULL);

    ASSERT_TRUE (p_sai_switch_api_table->initialize_switch != NULL);
    ASSERT_TRUE (p_sai_switch_api_table->shutdown_switch != NULL);
    ASSERT_TRUE (p_sai_switch_api_table->connect_switch != NULL);
    ASSERT_TRUE (p_sai_switch_api_table->disconnect_switch != NULL);
    ASSERT_TRUE (p_sai_switch_api_table->set_switch_attribute != NULL);
    ASSERT_TRUE (p_sai_switch_api_table->get_switch_attribute != NULL);

    /*
     * Switch Initialization.
     * Fill in notification callback routines with stubs.
     */
    notification.on_switch_state_change = sai_switch_operstate_callback;
    notification.on_fdb_event = sai_fdb_evt_callback;
    notification.on_port_state_change = sai_port_state_evt_callback;
    notification.on_switch_shutdown_request = sai_switch_shutdown_callback;
    notification.on_port_event = sai_port_evt_callback;

    ASSERT_TRUE(p_sai_switch_api_table->initialize_switch != NULL);

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               (p_sai_switch_api_table->initialize_switch (0, NULL, NULL,
                                                         &notification)));

    printf("Switch Init success \r\n");

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_api_query(SAI_API_QUEUE,
                    (static_cast<void**>(static_cast<void*>(&p_sai_qos_queue_api_table)))));

    ASSERT_TRUE (p_sai_qos_queue_api_table != NULL);

    ASSERT_TRUE (p_sai_qos_queue_api_table->set_queue_attribute != NULL);
    ASSERT_TRUE (p_sai_qos_queue_api_table->get_queue_attribute != NULL);
    ASSERT_TRUE (p_sai_qos_queue_api_table->get_queue_stats != NULL);

    ASSERT_EQ (NULL, sai_api_query(SAI_API_PORT,
              (static_cast<void**>(static_cast<void*>(&p_sai_port_api_table)))));

    ASSERT_TRUE (p_sai_port_api_table != NULL);

    ASSERT_TRUE (p_sai_port_api_table->set_port_attribute != NULL);
    ASSERT_TRUE (p_sai_port_api_table->get_port_attribute != NULL);
    ASSERT_TRUE (p_sai_port_api_table->get_port_stats != NULL);
}

/*
 * Validate get port queue id list get.
 */
TEST (saiQosQueueTest, port_queue_id_list_get)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    unsigned int max_ports = 0;
    unsigned int max_queues = 0;
    unsigned int port_index = 0;
    sai_object_id_t  port_id = SAI_NULL_OBJECT_ID;

    max_ports = sai_qos_max_ports_get();

    for (port_index = 0; port_index < max_ports; port_index++)
    {
        port_id = sai_qos_port_id_get (port_index);

        sai_rc = sai_test_port_max_number_queues_get (port_id,
                                                      &max_queues);
        ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_object_id_t queue_id_list[max_queues];

        sai_rc = sai_test_port_queue_id_list_get (port_id, max_queues,
                                                  &queue_id_list[0]);
        ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }
}

/*
 * Validate get cpu port queue id list get.
 */
TEST (saiQosQueueTest, cpu_port_queue_id_list_get)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    unsigned int max_queues = 0;

    sai_rc = sai_test_port_max_number_queues_get (cpu_port_id,
                                                  &max_queues);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_object_id_t queue_id_list[max_queues];

    sai_rc = sai_test_port_queue_id_list_get (cpu_port_id, max_queues,
                                              &queue_id_list[0]);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}


/*
 * Validate queue set attributes.
 */
TEST (saiQosQueueTest, queue_attribute_set)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t  attr;
    unsigned int     max_queues = 0;

    sai_rc = sai_test_port_max_number_queues_get (default_port_id,
                                                  &max_queues);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_object_id_t queue_id_list[max_queues];

    sai_rc = sai_test_port_queue_id_list_get (default_port_id, max_queues,
                                              &queue_id_list[0]);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_object_id_t  queue_id = queue_id_list[0];
    attr.id = SAI_QUEUE_ATTR_TYPE;
    attr.value.s32 = SAI_QUEUE_TYPE_MULTICAST;

    sai_rc = p_sai_qos_queue_api_table->set_queue_attribute (queue_id,
                                                             &attr);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);
}

/*
 * Validate Queue get attributes.
 */
TEST (saiQosQueueTest, queue_attribute_get)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t  attr;
    unsigned int     max_queues = 0;

    sai_rc = sai_test_port_max_number_queues_get (default_port_id,
                                                  &max_queues);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_object_id_t queue_id_list[max_queues];

    sai_rc = sai_test_port_queue_id_list_get (default_port_id, max_queues,
                                              &queue_id_list[0]);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_object_id_t  queue_id = queue_id_list[0];

    attr.id = SAI_QUEUE_ATTR_TYPE;
    sai_rc = p_sai_qos_queue_api_table->get_queue_attribute (queue_id, 1,
                                                             &attr);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    EXPECT_EQ (SAI_QUEUE_TYPE_UNICAST, attr.value.s32);

}

static sai_queue_stat_counter_t counter_id_get(unsigned int index)
{
    switch(index)
    {
        case 0:
            return SAI_QUEUE_STAT_PACKETS;
        case 1:
            return SAI_QUEUE_STAT_BYTES;
        case 2:
            return SAI_QUEUE_STAT_DROPPED_PACKETS;
        case 3:
            return SAI_QUEUE_STAT_DROPPED_BYTES;
        case 4:
            return SAI_QUEUE_STAT_GREEN_PACKETS;
        case 5:
            return SAI_QUEUE_STAT_GREEN_BYTES;
        case 6:
            return SAI_QUEUE_STAT_GREEN_DROPPED_PACKETS;
        case 7:
            return SAI_QUEUE_STAT_GREEN_DROPPED_BYTES;
        case 8:
            return SAI_QUEUE_STAT_YELLOW_PACKETS;
        case 9:
            return SAI_QUEUE_STAT_YELLOW_BYTES;
        case 10:
            return SAI_QUEUE_STAT_YELLOW_DROPPED_PACKETS;
        case 11:
            return SAI_QUEUE_STAT_YELLOW_DROPPED_BYTES;
        case 12:
            return SAI_QUEUE_STAT_RED_PACKETS;
        case 13:
            return SAI_QUEUE_STAT_RED_BYTES;
        case 14:
            return SAI_QUEUE_STAT_RED_DROPPED_PACKETS;
        case 15:
            return SAI_QUEUE_STAT_RED_DROPPED_BYTES;
        case 16:
            return SAI_QUEUE_STAT_GREEN_DISCARD_DROPPED_PACKETS;
        case 17:
            return SAI_QUEUE_STAT_GREEN_DISCARD_DROPPED_BYTES;
        case 18:
            return SAI_QUEUE_STAT_YELLOW_DISCARD_DROPPED_PACKETS;
        case 19:
            return SAI_QUEUE_STAT_YELLOW_DISCARD_DROPPED_BYTES;
        case 20:
            return SAI_QUEUE_STAT_RED_DISCARD_DROPPED_PACKETS;
        case 21:
            return SAI_QUEUE_STAT_RED_DISCARD_DROPPED_BYTES;
        case 22:
            return SAI_QUEUE_STAT_DISCARD_DROPPED_PACKETS;
        case 23:
            return SAI_QUEUE_STAT_DISCARD_DROPPED_BYTES;
    }
    return SAI_QUEUE_STAT_CUSTOM_RANGE_BASE;
}
/*
 * Validate queue statistics
 */
TEST (saiQosQueueTest, queue_stats_get)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    unsigned int     max_queues = 0;
    sai_queue_stat_counter_t counter_id;
    uint64_t counter_val;
    unsigned int idx = 0;

    sai_rc = sai_test_port_max_number_queues_get (default_port_id,
                                                  &max_queues);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_object_id_t queue_id_list[max_queues];

    sai_rc = sai_test_port_queue_id_list_get (default_port_id, max_queues,
                                              &queue_id_list[0]);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_object_id_t  queue_id = queue_id_list[0];
    while(1)
    {
        counter_id = counter_id_get(idx);
        if(counter_id == SAI_QUEUE_STAT_CUSTOM_RANGE_BASE) {
            break;
        }
        sai_rc = p_sai_qos_queue_api_table->get_queue_stats(queue_id, &counter_id,
                                                            1, &counter_val);
        if(sai_rc == SAI_STATUS_SUCCESS) {
             printf("Counter ID %d is supported. Val:0x%"PRIx64"\r\n",counter_id,counter_val);
        } else if( sai_rc == SAI_STATUS_NOT_SUPPORTED) {
             printf("Counter ID %d is not supported.\r\n",counter_id);
        } else {
             printf("Counter ID %d get returned error %d\r\n",counter_id, sai_rc);
        }
        idx++;
    }

}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    SetUpTestCase ();
    default_port_id = sai_qos_port_id_get (default_port);
    sai_test_cpu_port_id_get(&cpu_port_id);

    return RUN_ALL_TESTS();
}
