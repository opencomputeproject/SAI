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
*    sai_port_breakout_test_utils.cpp
*
* Abstract:
*
*    This file contains prototype declarations for utility functions used to 
*    set and get breakout modes for ports. 
*
*************************************************************************/

#ifndef SAI_PORT_BREAKOUT_UTILS_H
#define SAI_PORT_BREAKOUT_UTILS_H

extern "C" {
#include "saiswitch.h"
#include "saitypes.h"
#include "saiport.h"
#include "saistatus.h"
}

/**************************************************************************
 *                     Function Prototypes
 **************************************************************************/

bool sai_port_type_logical(sai_object_id_t port,
                           sai_port_api_t*
                           sai_port_api_table);

sai_status_t sai_port_break_out_mode_set(sai_switch_api_t *sai_switch_api_table,
                                        sai_port_api_t *sai_port_api_table,
                                        sai_object_id_t port,
                                        sai_port_breakout_mode_type_t
                                        breakout_mode);

sai_status_t sai_port_break_in_mode_set(sai_switch_api_t *sai_switch_api_table,
                                        sai_port_api_t *sai_port_api_table,
                                        uint32_t port_count,
                                        sai_object_id_t *port_list,
                                        sai_port_breakout_mode_type_t
                                        breakout_mode);

sai_status_t sai_port_break_out_mode_get(sai_port_api_t *sai_port_api_table,
                                         sai_object_id_t port,
                                         sai_port_breakout_mode_type_t
                                         breakout_mode);
#endif
