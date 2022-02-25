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
*    sai_l2_unit_test_defs.h
*
* Abstract:
*
*    This file contains declarations to be used by all L2 testing utilities
*
*************************************************************************/

#include "saitypes.h"
#include "saifdb.h"

#define SAI_GTEST_VLAN 10
#define SAI_GTEST_INVALID_VLAN_ID 4096

/*
 * Stubs for Callback functions to be passed from adaptor host/application
 * upon switch initialization API call.
 */
inline void sai_port_state_evt_callback (uint32_t count,
                                         sai_port_oper_status_notification_t *data)
{
}

inline void sai_fdb_evt_callback(uint32_t count, sai_fdb_event_notification_data_t *data)
{
}

inline void sai_switch_operstate_callback (sai_switch_oper_status_t
                                                  switchstate)
{
}

/* Packet event callback
 */
static inline void sai_packet_event_callback (const void *buffer,
                                              sai_size_t buffer_size,
                                              uint32_t attr_count,
                                              const sai_attribute_t *attr_list)
{
}

inline void  sai_switch_shutdown_callback (void)
{
}
