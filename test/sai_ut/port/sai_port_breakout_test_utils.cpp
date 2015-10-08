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
*    This contains utility and helper function definitions for SAI PORT BREAKOUT
*    functionality 
*
*************************************************************************/

#include "sai_port_breakout_test_utils.h"

extern "C" {
#include "saiswitch.h"
#include "saitypes.h"
#include "saiport.h"
#include "saistatus.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
}

#define LOG_PRINT(msg, ...) \
    printf(msg, ##__VA_ARGS__)
/*
 *Checks whether the given port is a logical port
 */

bool sai_port_type_logical(sai_object_id_t port_id, sai_port_api_t
                                                    *sai_port_api_table)
{
    sai_attribute_t sai_attr_get;

    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
    sai_attr_get.id = SAI_PORT_ATTR_TYPE;

    if(sai_port_api_table->get_port_attribute(port_id, 1, &sai_attr_get) != SAI_STATUS_SUCCESS) {
        return false;
    }

    if(SAI_PORT_TYPE_LOGICAL != sai_attr_get.value.s32) {
        return false;
    }

    return true;
}

/*
 * Checks whether the given breakout mode is supported in the given port
 */

static sai_status_t sai_check_supported_breakout_mode(sai_port_api_t *sai_port_api_table,
                                                      sai_object_id_t port,
                                                      sai_port_breakout_mode_type_t
                                                      breakout_mode,
                                                      bool *is_supported)
{
    unsigned int mode_index = 0;
    sai_status_t ret = SAI_STATUS_FAILURE;
    sai_attribute_t sai_attr_get;
    *is_supported = false;

    if(sai_port_type_logical(port, sai_port_api_table) != true) {
        LOG_PRINT("Port 0x%"PRIx64" is not a logical port", port);
        return ret;
    }

    memset(&sai_attr_get, 0, sizeof(sai_attr_get));
    sai_attr_get.id = SAI_PORT_ATTR_SUPPORTED_BREAKOUT_MODE;
    sai_attr_get.value.s32list.count = SAI_PORT_BREAKOUT_MODE_MAX;
    sai_attr_get.value.s32list.list = (int32_t *)calloc (SAI_PORT_BREAKOUT_MODE_MAX,
                                                         sizeof(int32_t));
    if(sai_attr_get.value.s32list.list == NULL) {
        LOG_PRINT("Could not allocate memory for breakout modes list");
        return SAI_STATUS_NO_MEMORY;
    }

    ret = sai_port_api_table->get_port_attribute(port, 1, &sai_attr_get);
    if(ret == SAI_STATUS_BUFFER_OVERFLOW ) {
        free(sai_attr_get.value.u32list.list);
        sai_attr_get.value.s32list.list = (int32_t *)calloc (sai_attr_get.value.s32list.count,
                                                             sizeof(int32_t));
        if(sai_attr_get.value.s32list.list == NULL) {
            LOG_PRINT("Could not allocate memory for breakout modes list");
            return SAI_STATUS_NO_MEMORY;
        }
        ret = sai_port_api_table->get_port_attribute(port, 1, &sai_attr_get);
    }
    if(ret != SAI_STATUS_SUCCESS) {
        free(sai_attr_get.value.s32list.list);
        return ret;
    }

    for(mode_index = 0; mode_index < sai_attr_get.value.s32list.count;
    mode_index++) {
        if(sai_attr_get.value.s32list.list[mode_index] == breakout_mode) {
            LOG_PRINT("port 0x%"PRIx64" supports mode %d", port, breakout_mode);
            *is_supported = true;
            break;
        }
    }

    free(sai_attr_get.value.u32list.list);
    return ret;
}

/*
 * Sets the breakout mode on the given port
 */

sai_status_t sai_port_break_out_mode_set(sai_switch_api_t *sai_switch_api_table,
                                         sai_port_api_t *sai_port_api_table,
                                         sai_object_id_t port,
                                         sai_port_breakout_mode_type_t breakout_mode)
{
    sai_status_t ret = SAI_STATUS_FAILURE;
    sai_attribute_t set_attr;
    bool is_supported = false;

    ret = sai_check_supported_breakout_mode(sai_port_api_table,
                                            port,
                                            breakout_mode,
                                            &is_supported);
    if(ret != SAI_STATUS_SUCCESS) {
        LOG_PRINT("check for supported beeakout mode %d for port 0x%"PRIx64" failed",
                   breakout_mode, port);
        return ret;
    }
    if(is_supported != true) {
        LOG_PRINT("port 0x%"PRIx64" does not support breakout mode %d", port,
                   breakout_mode);
        return SAI_STATUS_FAILURE;
    }

    memset(&set_attr, 0, sizeof(set_attr));
    /* Disable port link admin state */
    set_attr.id = SAI_PORT_ATTR_ADMIN_STATE;
    set_attr.value.booldata = false;

    ret = sai_port_api_table->set_port_attribute(port, &set_attr);
    if (ret != SAI_STATUS_SUCCESS) {
        LOG_PRINT("Port 0x%"PRIx64" Link Admin state disable failed\n", port);
        return ret;
    }

    memset(&set_attr, 0, sizeof(set_attr));
    /* Set port breakout */
    set_attr.id = SAI_SWITCH_ATTR_PORT_BREAKOUT;
    set_attr.value.portbreakout.breakout_mode = breakout_mode;
    set_attr.value.portbreakout.port_list.count = 1;
    set_attr.value.portbreakout.port_list.list =
    (sai_object_id_t *) calloc(1, sizeof(sai_object_id_t));
    if(set_attr.value.portbreakout.port_list.list == NULL) {
        LOG_PRINT("Could not allocate memory for breakout modes list");
        return SAI_STATUS_NO_MEMORY;
    }

    set_attr.value.portbreakout.port_list.list[0] = port;

    ret = sai_switch_api_table->set_switch_attribute((const
    sai_attribute_t*)&set_attr);
    if(ret != SAI_STATUS_SUCCESS) {
        LOG_PRINT("Port 0x%"PRIx64" Breakout %d mode failed\n", port,
                   breakout_mode);
        free(set_attr.value.portbreakout.port_list.list);
        return ret; /* rollback ? */
    }

    free(set_attr.value.portbreakout.port_list.list);
    return ret;
}

/*
 * Sets breakin mode on the given list of ports
 */

sai_status_t sai_port_break_in_mode_set(sai_switch_api_t *sai_switch_api_table,
                                        sai_port_api_t *sai_port_api_table,
                                        uint32_t port_count,
                                        sai_object_id_t *port_list,
                                        sai_port_breakout_mode_type_t
                                        breakout_mode)
{
    sai_status_t ret = SAI_STATUS_FAILURE;
    sai_attribute_t set_attr;
    sai_object_id_t port;
    uint32_t port_index = 0;
    bool is_supported = false;

    for (port_index=0; port_index < port_count; port_index++) {
        port = port_list[port_index];
        ret = sai_check_supported_breakout_mode(sai_port_api_table,
                                                port,
                                                breakout_mode,
                                                &is_supported);
        if(ret != SAI_STATUS_SUCCESS) {
            LOG_PRINT("check for supported beeakout mode %d for port 0x%"PRIx64" failed",
                      breakout_mode, port);
            return ret;
        }
        if(is_supported != true) {
            LOG_PRINT("port 0x%"PRIx64" does not support breakout mode %d", port,
                       breakout_mode);
            return SAI_STATUS_FAILURE;
        }
    }

    memset(&set_attr, 0, sizeof(set_attr));
    set_attr.id = SAI_SWITCH_ATTR_PORT_BREAKOUT;
    set_attr.value.portbreakout.breakout_mode = breakout_mode;
    set_attr.value.portbreakout.port_list.count = port_count;
    set_attr.value.portbreakout.port_list.list =
    (sai_object_id_t *) calloc(port_count, sizeof(sai_object_id_t));
    if (set_attr.value.portbreakout.port_list.list == NULL) {
        LOG_PRINT("Could not allocate memory for breakout modes list");
        return SAI_STATUS_NO_MEMORY;
    }

    for (port_index = 0; port_index < port_count; port_index++) {
        set_attr.value.portbreakout.port_list.list[port_index] = port_list[port_index];
    }

    ret = sai_switch_api_table->set_switch_attribute((const
    sai_attribute_t*)&set_attr);
    if(ret != SAI_STATUS_SUCCESS) {
        free(set_attr.value.portbreakout.port_list.list);
        LOG_PRINT("Port list break-in mode set failed\n");
        return ret;
    }

    free(set_attr.value.portbreakout.port_list.list);
    return ret;
}

/*
 * Gets the current breakout mode of the given port
 */

sai_status_t sai_port_break_out_mode_get(sai_port_api_t *sai_port_api_table,
                                         sai_object_id_t port,
                                         sai_port_breakout_mode_type_t breakout_mode)
{
    sai_status_t ret = SAI_STATUS_FAILURE;
    sai_attribute_t get_attr;
    get_attr.id = SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE;

    ret = sai_port_api_table->get_port_attribute(port, 1, &get_attr);
    if(ret != SAI_STATUS_SUCCESS) {
        LOG_PRINT("Port 0x%"PRIx64" get current breakout mode %d failed\n",
                   port, breakout_mode);
        return ret;
    }

    if (get_attr.value.s32 != breakout_mode) {
        LOG_PRINT("Port 0x%"PRIx64" breakout mode %d configuration failed \n",
                   port, breakout_mode);
        return SAI_STATUS_FAILURE;
    }

    LOG_PRINT("Port 0x%"PRIx64" breakout configuration %d success\n", port,
               breakout_mode);
    return ret;
}
