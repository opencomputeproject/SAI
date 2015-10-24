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
*    sai_qos_unit_test_utils.cpp
*     
* Abstract:

*    This contains utility and helper function definitions for testing the SAI 
*    QOS functionalities.
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
#include "saitypes.h"
#include "saistatus.h"
#include "saiswitch.h"
#include "saiport.h"
#include "saischedulergroup.h"
#include "saiqueue.h"
#include <inttypes.h>
}

#define SAI_TEST_MAX_PORTS                     (256)
static uint32_t port_count = 0;
static sai_object_id_t port_list[SAI_TEST_MAX_PORTS] = {0};

sai_switch_api_t* p_sai_switch_api_table = NULL;
sai_scheduler_group_api_t* p_sai_qos_sg_api_table = NULL;
sai_queue_api_t* p_sai_qos_queue_api_table = NULL;
sai_port_api_t* p_sai_port_api_table = NULL;
sai_scheduler_api_t *p_sai_scheduler_api_table = NULL;

sai_object_id_t sai_qos_port_id_get (uint32_t port_index)
{
    if(port_index >= port_count) {
        return 0;
    }

    return port_list [port_index];
}

sai_object_id_t sai_qos_invalid_port_id_get ()
{
    return (port_list[port_count-1] + 1);
}

sai_object_id_t sai_qos_max_ports_get ()
{
    return (port_count);
}

/*
 * Stubs for Callback functions to be passed from adaptor host/application
 * upon switch initialization API call.
 */

/*
 * Port state change callback.
 */
void sai_port_state_evt_callback (uint32_t count, sai_port_oper_status_notification_t *data)
{
}

/*
 * Port ADD/DELETE event callback.
 */
void sai_port_evt_callback (uint32_t count, sai_port_event_notification_t *data)
{
    uint32_t port_idx = 0;
    sai_object_id_t port_id = 0;
    sai_port_event_t port_event;

    for(port_idx = 0; port_idx < count; port_idx++) {
        port_id = data[port_idx].port_id;
        port_event = data[port_idx].port_event;

        if(port_event == SAI_PORT_EVENT_ADD) {
            if(port_count < SAI_TEST_MAX_PORTS) {
                port_list[port_count] = port_id;
                port_count++;
            }

            printf("PORT ADD EVENT FOR port 0x%"PRIx64" and total ports count is %d \r\n",
                   port_id, port_count);
        } else if(port_event == SAI_PORT_EVENT_DELETE) {

            printf("PORT DELETE EVENT for  port 0x%"PRIx64" and total ports count is %d \r\n",
                   port_id, port_count);
        } else {
            printf("Invalid PORT EVENT for port 0x%"PRIx64" \r\n", port_id);
        }
    }
}

/*
 * FDB event callback.
 */
void sai_fdb_evt_callback(uint32_t count, sai_fdb_event_notification_data_t *data)
{
}

/*
 * Switch operstate callback.
 */

void sai_switch_operstate_callback (sai_switch_oper_status_t switchstate)
{
}

/*
 * Packet event callback
 */
sai_status_t sai_packet_rx_callback (void * buffer, uint32_t buffer_size,
                                     uint32_t attr_count, sai_attribute_t *attr_list)
{
    return SAI_STATUS_SUCCESS;
}

/*
 * Switch shutdown callback.
 */
void  sai_switch_shutdown_callback (void)
{
}
/*
 * Packet event callback
 */
void sai_packet_event_callback (const void *buffer,
                                              sai_size_t buffer_size,
                                              uint32_t attr_count,
                                              const sai_attribute_t *attr_list)
{
}

sai_status_t sai_test_switch_max_number_hierarchy_levels_get (
                                                        unsigned int *hierarchy_levels)
{
    sai_status_t sai_rc;
    sai_attribute_t    attr  = {0};

    attr.id = SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUP_HIERARCHY_LEVELS;

    sai_rc = p_sai_switch_api_table->get_switch_attribute (1, &attr);

    if (sai_rc != SAI_STATUS_SUCCESS) {

        printf ("SAI Switch Get max hierarchy levels failed with error: %d.\n", sai_rc);

    } else {
        *hierarchy_levels = attr.value.u32;
        printf ("SAI Switch Get max hierarchy levels success. Count : %d\n",
                *hierarchy_levels);
    }


    return sai_rc;
}

sai_status_t sai_test_port_max_number_queues_get (sai_object_id_t  port_id,
                                                  unsigned int *queue_count)
{
    sai_status_t sai_rc;
    sai_attribute_t    attr  = {0};

    attr.id = SAI_PORT_ATTR_QOS_NUMBER_OF_QUEUES;

    sai_rc = p_sai_port_api_table->get_port_attribute (port_id, 1, &attr);
    if(sai_rc != SAI_STATUS_SUCCESS) {
        printf("Failed to get queue count. Error code:%d\n", sai_rc);
    } else {
         *queue_count = attr.value.u32;
         printf("Max queues on port 0x%"PRIx64": %d. \n", port_id, *queue_count);
    }
    return sai_rc;
}

sai_status_t sai_test_port_queue_id_list_get (sai_object_id_t port_id,
                                              unsigned int queue_count,
                                              sai_object_id_t *p_queue_id_list)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t    attr  = {0};
    unsigned int       index = 0;

    attr.id = SAI_PORT_ATTR_QOS_QUEUE_LIST;

    attr.value.objlist.count = queue_count;
    attr.value.objlist.list = p_queue_id_list;

    if (attr.value.objlist.list == NULL) {

        printf ("%s(): Memory alloc failed for attribute list.\n", __FUNCTION__);

        return SAI_STATUS_NO_MEMORY;
    }

    sai_rc = p_sai_port_api_table->get_port_attribute (port_id, 1, &attr);

    if (sai_rc == SAI_STATUS_BUFFER_OVERFLOW) {
        printf("Requested queue count %d, Max queues %d on port :0x%"PRIx64".\n",
               queue_count, attr.value.objlist.count, port_id);
        return SAI_STATUS_FAILURE;
    }

    if (sai_rc != SAI_STATUS_SUCCESS) {

        printf ("SAI Port Get queue id list failed with error: %d.\n", sai_rc);

    } else {

        printf ("SAI port Get queue id list success for Port Id: 0x%"PRIx64".\n",
                port_id);

        printf ("SAI Port 0x%"PRIx64" Queue List.\n", port_id);

        for (index = 0; index < attr.value.objlist.count; ++index) {
            printf ("SAI Queue index %d QOID 0x%"PRIx64".\n",
                    index , p_queue_id_list[index]);
        }
    }

    return sai_rc;
}

sai_status_t sai_test_port_queue_count_and_id_list_get(sai_object_id_t port_id,
                                             unsigned int *max_queues,
                                             sai_object_id_t *queue_id_list)
{
    sai_status_t  sai_rc;

    sai_rc = sai_test_port_max_number_queues_get (port_id, max_queues);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_port_queue_id_list_get (port_id, *max_queues, queue_id_list);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    return sai_rc;
}

sai_status_t sai_test_port_sched_group_id_count_get (sai_object_id_t port_id,
                                                     unsigned int *sg_count)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t    attr  = {0};

    attr.id = SAI_PORT_ATTR_QOS_NUMBER_OF_SCHEDULER_GROUPS;


    sai_rc = p_sai_port_api_table->get_port_attribute (port_id, 1, &attr);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("SAI Port Get SG id count failed with error: %d.\n", sai_rc);
    } else {
        *sg_count = attr.value.objlist.count;
        printf ("Scheduler group count for port 0x%"PRIx64" is %d. \n",port_id, *sg_count);
    }

    return sai_rc;
}

sai_status_t sai_test_port_sched_group_id_list_get (sai_object_id_t port_id,
                                              unsigned int sg_count,
                                              sai_object_id_t *p_sg_id_list)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t    attr  = {0};
    unsigned int       index = 0;

    attr.id = SAI_PORT_ATTR_QOS_SCHEDULER_GROUP_LIST;

    attr.value.objlist.count = sg_count;
    attr.value.objlist.list = p_sg_id_list;

    if (attr.value.objlist.list == NULL) {

        printf ("%s(): Memory alloc failed for attribute list.\n", __FUNCTION__);

        return SAI_STATUS_NO_MEMORY;
    }

    sai_rc = p_sai_port_api_table->get_port_attribute (port_id, 1, &attr);

    if (sai_rc == SAI_STATUS_BUFFER_OVERFLOW) {
        printf("Requested sg count %d, Max sg nodes %d on port :0x%"PRIx64".\n",
               sg_count, attr.value.objlist.count, port_id);
        return SAI_STATUS_FAILURE;
    }

    if (sai_rc != SAI_STATUS_SUCCESS) {

        printf ("SAI Port Get SG id list failed with error: %d.\n", sai_rc);

    } else {

        printf ("SAI port Get SG id list success for Port Id: 0x%"PRIx64".\n",
                port_id);

        printf ("SAI Port 0x%"PRIx64" SG List.\n", port_id);

        for (index = 0; index < attr.value.objlist.count; ++index) {
            printf ("SAI SG index %d SG ID 0x%"PRIx64".\n",
                    index , p_sg_id_list[index]);
        }
    }

    return sai_rc;
}

sai_status_t sai_test_scheduler_create (sai_object_id_t *p_sched_id,
                                        unsigned int attr_count, ...)
{
    sai_status_t       sai_rc;
    sai_attribute_t    attr_list[attr_count];
    va_list            varg_list;
    unsigned int       index;

    va_start (varg_list, attr_count);
    for (index = 0; index < attr_count; ++index) {

        attr_list[index].id = va_arg (varg_list, unsigned int);

        switch (attr_list[index].id) {

            case SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM:
                attr_list[index].value.s32 = va_arg (varg_list, int32_t);

                printf ("Attr Index: %d, Set Scheduler algorithem %d.\n",
                        index, attr_list[index].value.s32);
                break;

            case SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT:
                attr_list[index].value.u8 = va_arg (varg_list, int);

                printf ("Attr Index: %d, Set Scheduler weigth %d.\n",
                        index, attr_list[index].value.u8);
                break;

            case SAI_SCHEDULER_ATTR_SHAPER_TYPE:
                attr_list[index].value.s32 = va_arg (varg_list, int32_t);

                printf ("Attr Index: %d, Set Scheduler shaper tyep %d.\n",
                        index, attr_list[index].value.s32);
                break;

            case SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE:
                attr_list[index].value.u64 = va_arg (varg_list, uint32_t);

                printf ("Attr Index: %d, Set Scheduler min bw rate  %"PRIu64".\n",
                        index, attr_list[index].value.u64);
                break;

            case SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE:
                attr_list[index].value.u64 = va_arg (varg_list, uint32_t);

                printf ("Attr Index: %d, Set Scheduler min bw burst %"PRIu64".\n",
                        index, attr_list[index].value.u64);
                break;

            case SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE:
                attr_list[index].value.u64 = va_arg (varg_list, uint32_t);

                printf ("Attr Index: %d, Set Scheduler max bw rate  %"PRIu64".\n",
                        index, attr_list[index].value.u64);
                break;

            case SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE:
                attr_list[index].value.u64 = va_arg (varg_list, uint32_t);

                printf ("Attr Index: %d, Set Scheduler max bw burst %"PRIu64".\n",
                        index, attr_list[index].value.u64);
                break;

            default:
                attr_list[index].value.u64 = va_arg (varg_list, unsigned int);
                printf ("Attr Index: %d, Set unknown Attr Id: %d to value: %ld.\n",
                        index, attr_list[index].id, attr_list[index].value.u64);
                break;
        }
    }

    va_end (varg_list);

    sai_rc = p_sai_scheduler_api_table->create_scheduler_profile (p_sched_id,
                                          attr_count,(attr_count ? &attr_list[0] : NULL));

    if (sai_rc != SAI_STATUS_SUCCESS) {

        printf ("SAI Scheduler Creation failed with error: %d.\n", sai_rc);

    } else {

        printf ("SAI Scheduler Creation success, Group Id: 0x%"PRIx64".\n",
                (*p_sched_id));
    }

    return sai_rc;
}

sai_status_t sai_test_scheduler_remove (sai_object_id_t sched_id)
{
    sai_status_t sai_rc;

    sai_rc = p_sai_scheduler_api_table->remove_scheduler_profile (sched_id);
    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("SAI Scheduler Remove failed with error: %d.\n", sai_rc);
    } else {
        printf ("SAI Scheduler Remove success for Group Id: 0x%"PRIx64".\n",
                sched_id);
    }

    return sai_rc;
}

sai_status_t sai_test_cpu_port_id_get (sai_object_id_t *port_id)
{
    sai_status_t      sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t   attr  = {0};

    attr.id = SAI_SWITCH_ATTR_CPU_PORT;

    sai_rc = p_sai_switch_api_table->get_switch_attribute(1, &attr);

    *port_id = attr.value.oid;

    return sai_rc;
}

