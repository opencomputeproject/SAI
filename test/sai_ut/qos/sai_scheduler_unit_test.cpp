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
*    sai_scheduler_unit_test.cpp
*     
* Abstract:
* 
*    SAI SCHEDULER UNIT TEST :- Covers the test cases for validating all 
*    public APIs in SAI SCHEDULER module.
*
*************************************************************************/

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
}

#define SAI_MAX_QUEUES_PER_PORT  40
static const unsigned int default_port = 31;
static sai_object_id_t  default_port_id  = 0;
static sai_object_id_t  cpu_port_id = 0;
static sai_object_id_t  queue_id_list[SAI_MAX_QUEUES_PER_PORT];
unsigned int            max_queues = 0;

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

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_api_query(SAI_API_SCHEDULER,
                    (static_cast<void**>(static_cast<void*>(&p_sai_scheduler_api_table)))));

    ASSERT_TRUE (p_sai_scheduler_api_table != NULL);

    ASSERT_TRUE (p_sai_scheduler_api_table->create_scheduler_profile != NULL);
    ASSERT_TRUE (p_sai_scheduler_api_table->remove_scheduler_profile != NULL);
    ASSERT_TRUE (p_sai_scheduler_api_table->set_scheduler_attribute != NULL);
    ASSERT_TRUE (p_sai_scheduler_api_table->get_scheduler_attribute != NULL);
}

static void sai_test_scheduler_attr_list_print (unsigned int attr_count,
                                                  sai_attribute_t *p_attr_list)
{
    unsigned int     attr_index;
    sai_attribute_t *p_attr = NULL;

    printf ("Printing SAI Scheduler attribute list..\n");
    for (attr_index = 0, p_attr = p_attr_list;
         attr_index < attr_count; ++attr_index, ++p_attr) {

        switch (p_attr->id) {

            case SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM:
                printf ("Index: %d, Scheduler algorithem %d.\n",
                        attr_index, p_attr->value.s32);
                break;

            case SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT:
                printf ("Index: %d, Scheduler weight %d.\n",
                        attr_index, p_attr->value.u8);
                break;

            case SAI_SCHEDULER_ATTR_SHAPER_TYPE:
                printf ("Index: %d, Scheduler shape type %d.\n",
                        attr_index, p_attr->value.u8);
                break;

            case SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE:
                printf ("Index: %d, Scheduler min bw rate %"PRIu64".\n",
                        attr_index, p_attr->value.u64);
                break;

            case SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE:
                printf ("Index: %d, Scheduler min bw burst %"PRIu64".\n",
                        attr_index, p_attr->value.u64);
                break;

            case SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE:
                printf ("Index: %d, Scheduler max bw rate %"PRIu64".\n",
                        attr_index, p_attr->value.u64);
                break;

            case SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE:
                printf ("Index: %d, Scheduler max bw burst %"PRIu64".\n",
                        attr_index, p_attr->value.u64);
                break;

            default:
                printf ("Index: %d, Scheduler unknown Attr Id: %d, Value: %ld.\n",
                        attr_index, p_attr->id, p_attr->value.u64);
                break;
        }
    }
}

static sai_status_t sai_test_scheduler_attr_get (sai_object_id_t sched_id,
                                            sai_attribute_t *p_attr_list,
                                            unsigned int attr_count, ...)
{
    sai_status_t         sai_rc;
    va_list              varg_list;
    unsigned int         index;

    va_start (varg_list, attr_count);

    for (index = 0; index < attr_count; index++) {

        p_attr_list [index].id = va_arg (varg_list, unsigned int);
    }

    va_end (varg_list);

    sai_rc = p_sai_scheduler_api_table->get_scheduler_attribute (sched_id,
                                                                 attr_count,
                                                                 p_attr_list);

    if (sai_rc != SAI_STATUS_SUCCESS) {

        printf ("SAI Scheduler Get attribute failed with error: %d.\n", sai_rc);

    } else {

        printf ("SAI Scheduler Get attribute success.\n");

        sai_test_scheduler_attr_list_print (attr_count, p_attr_list);
    }

    return sai_rc;
}

static void sai_scheduler_verify_after_removal (sai_object_id_t sched_id)
{
    sai_status_t  sai_rc;

    sai_rc =  sai_test_scheduler_remove (sched_id);

    ASSERT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
}

/*
 * Validate scheduler create, remove and duplicate delete.
 */
TEST (saiQosSchedulerTest, scheduler_create_and_remove)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t sched_id = SAI_NULL_OBJECT_ID;

    sai_rc = sai_test_scheduler_create (&sched_id, 0);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_scheduler_verify_after_removal (sched_id);

    sai_rc = sai_test_scheduler_create (&sched_id, 2,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_DWRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_scheduler_create (&sched_id, 3,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_scheduler_create (&sched_id, 3,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_scheduler_verify_after_removal (sched_id);
}

/*
 * Validate scheduler get attributes.
 */
TEST (saiQosSchedulerTest, schededuler_attribute_get)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;
    unsigned int     attr_count = 7;
    sai_attribute_t  attr_list[attr_count];

    sai_rc = sai_test_scheduler_create (&sched_id, 0);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_scheduler_attr_get (sched_id, &attr_list[0], attr_count,
                                            SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM,
                                            SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT,
                                            SAI_SCHEDULER_ATTR_SHAPER_TYPE,
                                            SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE,
                                            SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE,
                                            SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE,
                                            SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE);

    ASSERT_EQ (SAI_SCHEDULING_WRR, attr_list[0].value.s32);
    ASSERT_EQ (1, attr_list[1].value.u8);
    ASSERT_EQ (SAI_METER_TYPE_BYTES, attr_list[2].value.s32);
    ASSERT_EQ (0, attr_list[3].value.u64);
    ASSERT_EQ (0, attr_list[4].value.u64);
    ASSERT_EQ (0, attr_list[5].value.u64);
    ASSERT_EQ (0, attr_list[6].value.u64);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_scheduler_create (&sched_id, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_DWRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_scheduler_attr_get (sched_id, &attr_list[0], attr_count,
                                            SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM,
                                            SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT,
                                            SAI_SCHEDULER_ATTR_SHAPER_TYPE,
                                            SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE,
                                            SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE,
                                            SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE,
                                            SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE);


    ASSERT_EQ (SAI_SCHEDULING_DWRR, attr_list[0].value.s32);
    ASSERT_EQ (10, attr_list[1].value.u8);
    ASSERT_EQ (SAI_METER_TYPE_BYTES, attr_list[2].value.s32);
    ASSERT_EQ (2048, attr_list[3].value.u64);
    ASSERT_EQ (1024, attr_list[4].value.u64);
    ASSERT_EQ (20480, attr_list[5].value.u64);
    ASSERT_EQ (10240, attr_list[6].value.u64);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate scheduler set attributes.
 */
TEST (saiQosSchedulerTest, scheduler_attribute_set)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t  attr;
    unsigned int     attr_count = 7;
    sai_attribute_t  attr_list[attr_count];
    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;

    sai_rc = sai_test_scheduler_create (&sched_id, 0);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM;
    attr.value.s32 = SAI_SCHEDULING_DWRR;

    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT;
    attr.value.u8 = 10;

    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_SHAPER_TYPE;
    attr.value.s32 = SAI_METER_TYPE_BYTES;

    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE;
    attr.value.u64 = 2048;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE;
    attr.value.u64 = 1024;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE;
    attr.value.u64 = 20480;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE;
    attr.value.u64 = 10240;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_scheduler_attr_get (sched_id, &attr_list[0], attr_count,
                                            SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM,
                                            SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT,
                                            SAI_SCHEDULER_ATTR_SHAPER_TYPE,
                                            SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE,
                                            SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE,
                                            SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE,
                                            SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE);

    ASSERT_EQ (SAI_SCHEDULING_DWRR, attr_list[0].value.s32);
    ASSERT_EQ (10, attr_list[1].value.u8);
    ASSERT_EQ (SAI_METER_TYPE_BYTES, attr_list[2].value.s32);
    ASSERT_EQ (2048, attr_list[3].value.u64);
    ASSERT_EQ (1024, attr_list[4].value.u64);
    ASSERT_EQ (20480, attr_list[5].value.u64);
    ASSERT_EQ (10240, attr_list[6].value.u64);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate scheduler apply on queue.
 */
TEST (saiQosSchedulerTest, scheduler_on_queue)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t  attr;
    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;
    unsigned int     queue_index = 0;
    sai_object_id_t  queue_id = SAI_NULL_OBJECT_ID;

    sai_rc = sai_test_port_max_number_queues_get (default_port_id,
                                                  &max_queues);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Scheduler DWRR + SHAPE */
    sai_rc = sai_test_scheduler_create (&sched_id, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_DWRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 10000000,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 1000000);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id;

    /* Apply Scheduler on all queues */
    for (queue_index = 0; queue_index < max_queues; queue_index++) {
        queue_id = queue_id_list[queue_index];
        sai_rc = p_sai_qos_queue_api_table->set_queue_attribute (queue_id, &attr);
        ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);

    /* Delete Scheduler from all queues */
    attr.id = SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;

    for (queue_index = 0; queue_index < max_queues; queue_index++) {
        queue_id = queue_id_list[queue_index];
        sai_rc = p_sai_qos_queue_api_table->set_queue_attribute (queue_id, &attr);
        ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate Strict priority + Shaping scheduler on queue.
 */
TEST (saiQosSchedulerTest, scheduler_strict_priority_on_queue)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t  attr;
    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t  queue_id = SAI_NULL_OBJECT_ID;

    sai_rc = sai_test_port_max_number_queues_get (default_port_id,
                                                  &max_queues);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Test Strict priority + shape scheduler on queue */
    sai_rc = sai_test_scheduler_create (&sched_id, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_STRICT,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    queue_id = queue_id_list[0];

    attr.id = SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id;
    sai_rc = p_sai_qos_queue_api_table->set_queue_attribute (queue_id, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* SP + NO MAX Rate & Burst */
    attr.id = SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE;
    attr.value.u64 = 0;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE;
    attr.value.u64 = 0;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* SP + NO SHAPE */
    attr.id = SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE;
    attr.value.u64 = 0;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE;
    attr.value.u64 = 0;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;

    sai_rc = p_sai_qos_queue_api_table->set_queue_attribute (queue_id, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Scheduler replace on queue.
 */
TEST (saiQosSchedulerTest, scheduler_replace_on_queue)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t  attr;
    sai_object_id_t  queue_id = SAI_NULL_OBJECT_ID;

    sai_rc = sai_test_port_max_number_queues_get (default_port_id,
                                                  &max_queues);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Modify Scheduler on queue */
    sai_object_id_t  sched_id_1 = SAI_NULL_OBJECT_ID;
    sai_object_id_t  sched_id_2 = SAI_NULL_OBJECT_ID;

    sai_rc = sai_test_scheduler_create (&sched_id_1, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_WRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_scheduler_create (&sched_id_2, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_DWRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 100,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 4098,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 409800,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 20480);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    queue_id = queue_id_list[0];
    attr.id = SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID;

    attr.value.oid = sched_id_1;
    sai_rc = p_sai_qos_queue_api_table->set_queue_attribute (queue_id, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.value.oid = sched_id_2;
    sai_rc = p_sai_qos_queue_api_table->set_queue_attribute (queue_id, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id_1);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id_2);
    ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);

    attr.id = SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;

    sai_rc = p_sai_qos_queue_api_table->set_queue_attribute (queue_id, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id_2);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate scheduler appply on port.
 */

TEST (saiQosSchedulerTest, scheduler_on_port)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;
    sai_attribute_t  attr;

    sai_rc = sai_test_scheduler_create (&sched_id, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_DWRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id;

    sai_rc = p_sai_port_api_table->set_port_attribute (default_port_id,
                                                       &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);

    attr.id = SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;

    sai_rc = p_sai_port_api_table->set_port_attribute (default_port_id,
                                                       &attr);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate scheduler replace on port.
 */

TEST (saiQosSchedulerTest, scheduler_replace_on_port)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t  attr;

    sai_object_id_t  sched_id_1 = SAI_NULL_OBJECT_ID;
    sai_object_id_t  sched_id_2 = SAI_NULL_OBJECT_ID;

    sai_rc = sai_test_scheduler_create (&sched_id_1, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_WRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_scheduler_create (&sched_id_2, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_DWRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 100,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 4098,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 409800,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 20480);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id_1;
    sai_rc = p_sai_port_api_table->set_port_attribute (default_port_id,
                                                       &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id_2;
    sai_rc = p_sai_port_api_table->set_port_attribute (default_port_id,
                                                       &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id_1);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id_2);
    ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);

    attr.id = SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;
    sai_rc = p_sai_port_api_table->set_port_attribute (default_port_id,
                                                       &attr);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id_2);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate Modifying scheduler parmas after applyinig on queue and port.
 */

TEST (saiQosSchedulerTest, scheduler_modify)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t  attr;
    unsigned int     queue_index = 0;
    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;

    sai_rc = sai_test_port_max_number_queues_get (default_port_id,
                                                  &max_queues);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_scheduler_create (&sched_id, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_WRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id;

    sai_object_id_t queue_id = queue_id_list[queue_index];
    sai_rc = p_sai_qos_queue_api_table->set_queue_attribute (queue_id, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id;

    sai_rc = p_sai_port_api_table->set_port_attribute (default_port_id,
                                                       &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Modify */
    attr.id = SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM;
    attr.value.s32 = SAI_SCHEDULING_STRICT;

    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT;
    attr.value.u8 = 50;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_SHAPER_TYPE;
    attr.value.s32 = SAI_METER_TYPE_PACKETS;

    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove */
    attr.id = SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;

    sai_rc = p_sai_qos_queue_api_table->set_queue_attribute (queue_id, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;
    sai_rc = p_sai_port_api_table->set_port_attribute (default_port_id,
                                                       &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate scheduler appply on CPU port queues.
 */
TEST (saiQosSchedulerTest, scheduler_on_cpu_port_queue)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t  attr;
    unsigned int     max_queues = 0;
    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;
    unsigned int     queue_index = 0;
    sai_object_id_t  queue_id = SAI_NULL_OBJECT_ID;

    sai_rc = sai_test_port_max_number_queues_get (cpu_port_id,
                                                  &max_queues);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_object_id_t queue_id_list[max_queues];

    sai_rc = sai_test_port_queue_id_list_get (cpu_port_id, max_queues,
                                              &queue_id_list[0]);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Scheduler DWRR + SHAPE */
    sai_rc = sai_test_scheduler_create (&sched_id, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_DWRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id;

    /* Apply Scheduler on all queues */
    for (queue_index = 0; queue_index < max_queues; queue_index++) {
        queue_id = queue_id_list[queue_index];
        sai_rc = p_sai_qos_queue_api_table->set_queue_attribute (queue_id, &attr);
        ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    /* Test Packet mode */
    attr.id = SAI_SCHEDULER_ATTR_SHAPER_TYPE;
    attr.value.s32 = SAI_METER_TYPE_PACKETS;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE;
    attr.value.u64 = 4000;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE;
    attr.value.u64 = 200;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE;
    attr.value.u64 = 500;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE;
    attr.value.u64 = 200;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);

    attr.id = SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;

    for (queue_index = 0; queue_index < max_queues; queue_index++) {
        queue_id = queue_id_list[queue_index];
        sai_rc = p_sai_qos_queue_api_table->set_queue_attribute (queue_id, &attr);
        ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}


/*
 * Validate scheduler apply on CPU port.
 */

TEST (saiQosSchedulerTest, scheduler_on_cpu_port)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;
    sai_attribute_t  attr;

    sai_rc = sai_test_scheduler_create (&sched_id, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_DWRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id;

    sai_rc = p_sai_port_api_table->set_port_attribute (cpu_port_id, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;

    sai_rc = p_sai_port_api_table->set_port_attribute (cpu_port_id, &attr);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}


int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    SetUpTestCase ();
    default_port_id = sai_qos_port_id_get (default_port);
    sai_test_cpu_port_id_get(&cpu_port_id);
    sai_test_port_queue_count_and_id_list_get(default_port_id, &max_queues,
                                              &queue_id_list[0]);

    return RUN_ALL_TESTS();
}

