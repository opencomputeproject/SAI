/**
 * Copyright (c) 2021 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
 *
 * @file    sai_rpc_server_notify.cpp
 *
 * @brief   This module contains RPC server handler and helper functions
 */
#include <arpa/inet.h>

#include <iostream>
#include <cstring>
#include "sai_rpc.h"

extern "C" {
#include "sai.h"
#include "saitypes.h"
#include "saimetadatatypes.h"
#include "saimetadatautils.h"
#include "saimetadata.h"
}

using namespace ::sai;

extern "C" {

void on_switch_state_change(_In_ sai_object_id_t switch_id,
                            _In_ sai_switch_oper_status_t switch_oper_status)//
{
}

void on_fdb_event(_In_ uint32_t count,
                  _In_ sai_fdb_event_notification_data_t *data)
{
}

void on_port_state_change(_In_ uint32_t count,
                          _In_ sai_port_oper_status_notification_t *data)
{
}

void on_shutdown_request(_In_ sai_object_id_t switch_id)//
{
}

void on_packet_event(_In_ sai_object_id_t switch_id,
                     _In_ const void *buffer,
                     _In_ sai_size_t buffer_size,
                     _In_ uint32_t attr_count,
                     _In_ const sai_attribute_t *attr_list)
{
}

void test()
{
  printf("Test");
}

}