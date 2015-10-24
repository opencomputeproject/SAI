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
*    sai_qos_unit_test_utils.h
*     
* Abstract:
*
*    This contains the base class definition having the utility and helper 
*    functions for testing the SAI QOS functionalities.
*
*************************************************************************/


#ifndef __SAI_QOS_UNIT_TEST_H__
#define __SAI_QOS_UNIT_TEST_H__

extern "C" {
#include "sai.h"
#include "saitypes.h"
#include "saistatus.h"
#include "saiswitch.h"
#include "saiport.h"
#include "saischedulergroup.h"
#include "saiqueue.h"
#include <inttypes.h>
}

void SetUpTestCase();

/* Methods for retrieving SAI port id for routing test cases */
sai_object_id_t sai_qos_port_id_get (uint32_t port_index);
sai_object_id_t sai_qos_invalid_port_id_get ();
sai_object_id_t sai_qos_max_ports_get ();

sai_status_t sai_test_switch_max_number_hierarchy_levels_get (unsigned int *hierarchy_levels);
sai_status_t sai_test_port_max_number_queues_get (sai_object_id_t  port_id,
                                                  unsigned int *queue_count);
sai_status_t sai_test_port_queue_id_list_get (sai_object_id_t  port_id,
                                              unsigned int queue_count,
                                              sai_object_id_t *p_queue_id_list);

sai_status_t sai_test_port_sched_group_id_count_get (sai_object_id_t port_id,
                                                     unsigned int *sg_count);

sai_status_t sai_test_port_sched_group_id_list_get (sai_object_id_t port_id,
                                                    unsigned int sg_count,
                                                    sai_object_id_t *p_sg_id_list);

static inline sai_status_t sai_test_invalid_attr_status_code (
                                          sai_status_t status,
                                          unsigned int attr_index)
{
    return (status + SAI_STATUS_CODE (attr_index));
}
sai_status_t sai_test_scheduler_create (sai_object_id_t *p_sched_id,
                                        unsigned int attr_count, ...);
sai_status_t sai_test_scheduler_remove (sai_object_id_t sched_id);

sai_status_t sai_test_cpu_port_id_get (sai_object_id_t *port_id);

extern sai_switch_api_t            *p_sai_switch_api_table;
extern sai_scheduler_group_api_t   *p_sai_qos_sg_api_table;
extern sai_queue_api_t             *p_sai_qos_queue_api_table;
extern sai_port_api_t              *p_sai_port_api_table;
extern sai_scheduler_api_t         *p_sai_scheduler_api_table;

extern void sai_port_state_evt_callback (uint32_t count, sai_port_oper_status_notification_t *data);
extern void sai_port_evt_callback (uint32_t count, sai_port_event_notification_t *data);
extern void sai_fdb_evt_callback(uint32_t count, sai_fdb_event_notification_data_t *data);
extern void sai_switch_operstate_callback (sai_switch_oper_status_t switchstate);
extern sai_status_t sai_packet_rx_callback (void * buffer, uint32_t buffer_size,
                                            uint32_t attr_count, sai_attribute_t *attr_list);
extern void  sai_switch_shutdown_callback (void);
extern void sai_packet_event_callback (const void *buffer,
                                sai_size_t buffer_size,
                                uint32_t attr_count,
                                const sai_attribute_t *attr_list);

extern sai_status_t sai_test_port_queue_count_and_id_list_get(sai_object_id_t port_id,
                                                    unsigned int *max_queues,
                                                    sai_object_id_t *queue_id_list);


#endif /* __SAI_QOS_UNIT_TEST_H__ */
