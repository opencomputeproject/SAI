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
*    sai_stp_unit_test.cpp
*
* Abstract:
*
*    This file contains APIS for testing the SAI STP module
*
*************************************************************************/

#include "gtest/gtest.h"
#include "sai_stp_unit_test.h"

extern "C" {
#include "sai.h"
#include "saistatus.h"
#include "saitypes.h"
#include "saiswitch.h"
#include "saistp.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "stdarg.h"
}

#define SAI_STP_NO_OF_ATTRIB 1
#define SAI_STP_DEF_VLAN 1
#define SAI_STP_VLAN_2 2
#define SAI_STP_VLAN_3 3
#define SAI_STP_VLAN_10 10
#define SAI_MAX_PORTS 106

static uint32_t port_count = 0;

static sai_object_id_t port_list[SAI_MAX_PORTS] = {0};

sai_object_id_t stpTest ::sai_stp_port_id_get (uint32_t port_index)
{
    if(port_index >= port_count) {
        return 0;
    }

    return port_list [port_index];
}

sai_object_id_t stpTest ::sai_stp_invalid_port_id_get ()
{
    return (port_list[port_count-1] + 1);
}

/*
 * Stubs for Callback functions to be passed from adaptor host/application
 * upon switch initialization API call.
 */
static inline void sai_port_evt_callback (uint32_t count,
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

            printf("PORT ADD EVENT FOR port %"PRIu64" and total ports count is %d \r\n",
                   port_id, port_count);
        } else if(port_event == SAI_PORT_EVENT_DELETE) {

            printf("PORT DELETE EVENT for  port %"PRIu64" and total ports count is %d \r\n",
                   port_id, port_count);
        } else {
            printf("Invalid PORT EVENT for port %"PRIu64" \r\n", port_id);
        }
    }
}

static inline void sai_port_state_evt_callback (uint32_t count,
                                                sai_port_oper_status_notification_t *data)
{
}

static inline void sai_fdb_evt_callback (uint32_t count, sai_fdb_event_notification_data_t *data)
{
}

static inline void sai_switch_operstate_callback (sai_switch_oper_status_t
                                                  switchstate) {
}

/* Packet event callback
 */
static inline void sai_packet_event_callback (const void *buffer,
                                              sai_size_t buffer_size,
                                              uint32_t attr_count,
                                              const sai_attribute_t *attr_list)
{
}

static inline void  sai_switch_shutdown_callback (void) {
}


void stpTest ::SetUpTestCase (void) {
    sai_switch_notification_t notification;
    memset (&notification, 0, sizeof(sai_switch_notification_t));

    memset(&notification, 0, sizeof(sai_switch_notification_t));
    /*
     * Query and populate the SAI Switch API Table.
     */
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_SWITCH, (static_cast<void**>
                                 (static_cast<void*>(&p_sai_switch_api_tbl)))));

    ASSERT_TRUE (p_sai_switch_api_tbl != NULL);

    /*
     * Switch Initialization.
     */
    ASSERT_TRUE(p_sai_switch_api_tbl->initialize_switch != NULL);

    /*
     * Fill in notification callback routines with stubs.
     */
    notification.on_switch_state_change = sai_switch_operstate_callback;
    notification.on_fdb_event = sai_fdb_evt_callback;
    notification.on_port_state_change = sai_port_state_evt_callback;
    notification.on_switch_shutdown_request = sai_switch_shutdown_callback;
    notification.on_packet_event = sai_packet_event_callback;
    notification.on_port_event = sai_port_evt_callback;

    EXPECT_EQ (SAI_STATUS_SUCCESS,
               (p_sai_switch_api_tbl->initialize_switch (0, NULL, NULL,
                                                         &notification)));
    /*
     * Query and populate the SAI Virtual Router API Table.
     */
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_VLAN, (static_cast<void**>
                                         (static_cast<void*>
                                          (&p_sai_vlan_api_tbl)))));

    ASSERT_TRUE (p_sai_vlan_api_tbl != NULL);

    /*
     * Query and populate the SAI Virtual Router API Table.
     */
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_STP, (static_cast<void**>
                                         (static_cast<void*>
                                          (&p_sai_stp_api_tbl)))));

    ASSERT_TRUE (p_sai_stp_api_tbl != NULL);

    sai_port_id_1 = sai_stp_port_id_get (0);
}

sai_switch_api_t* stpTest ::p_sai_switch_api_tbl = NULL;
sai_vlan_api_t* stpTest ::p_sai_vlan_api_tbl = NULL;
sai_stp_api_t* stpTest ::p_sai_stp_api_tbl = NULL;
sai_object_id_t stpTest ::sai_port_id_1 = 0;



TEST_F(stpTest, def_stp_get)
{
    sai_attribute_t attr[SAI_STP_NO_OF_ATTRIB] = {0};
    sai_object_id_t def_stp_id = 0;
    sai_vlan_id_t   vlan_id = SAI_STP_VLAN_10;

    attr[0].id =  SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID;
    attr[0].value.u16 = 0;

    EXPECT_EQ(SAI_STATUS_INVALID_ATTRIBUTE_0,p_sai_switch_api_tbl->
                  set_switch_attribute(attr));

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_switch_api_tbl->
                  get_switch_attribute(SAI_STP_NO_OF_ATTRIB, attr));

    def_stp_id = attr[0].value.oid;

    attr[0].id = SAI_STP_ATTR_VLAN_LIST;
    attr[0].value.vlanlist.vlan_count = 1;
    attr[0].value.vlanlist.vlan_list = &vlan_id;

    EXPECT_EQ(SAI_STATUS_INVALID_ATTRIBUTE_0,p_sai_stp_api_tbl->
                  set_stp_attribute(def_stp_id, attr));

    attr[0].value.vlanlist.vlan_count = 1;
    attr[0].value.vlanlist.vlan_list = (sai_vlan_id_t *) calloc (attr[0].value.vlanlist.vlan_count,
                                                             sizeof(sai_vlan_id_t));

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_stp_api_tbl->
                  get_stp_attribute(def_stp_id, SAI_STP_NO_OF_ATTRIB, attr));

    EXPECT_EQ (attr[0].value.objlist.count, 1);
    EXPECT_EQ (attr[0].value.objlist.list[0], SAI_STP_DEF_VLAN);

    free (attr[0].value.vlanlist.vlan_list);
}

TEST_F(stpTest, create_stp_group)
{
    sai_attribute_t attr[SAI_STP_NO_OF_ATTRIB] = {0};
    sai_object_id_t stp_id = 0;
    sai_object_id_t def_inst_id = 0;

    attr[SAI_STP_NO_OF_ATTRIB - 1].id =  SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID;

    EXPECT_EQ(SAI_STATUS_INVALID_ATTRIBUTE_0,p_sai_switch_api_tbl->
                  set_switch_attribute(attr));

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_switch_api_tbl->
                  get_switch_attribute(SAI_STP_NO_OF_ATTRIB, attr));

    def_inst_id = attr[0].value.oid;

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_stp_api_tbl->
            create_stp(&stp_id,0,attr));

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_vlan_api_tbl->
            create_vlan(SAI_STP_VLAN_2));

    attr[0].id = SAI_VLAN_ATTR_STP_INSTANCE;
    attr[0].value.oid = stp_id;

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_vlan_api_tbl->
            set_vlan_attribute(SAI_STP_VLAN_2,attr));

    EXPECT_EQ(SAI_STATUS_OBJECT_IN_USE, p_sai_stp_api_tbl->
            remove_stp(stp_id));

    attr[0].id = SAI_VLAN_ATTR_STP_INSTANCE;
    attr[0].value.oid = def_inst_id;

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_vlan_api_tbl->
            set_vlan_attribute(SAI_STP_VLAN_2,attr));

    EXPECT_EQ(SAI_STATUS_SUCCESS, p_sai_stp_api_tbl->
            remove_stp(stp_id));

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_vlan_api_tbl->
            remove_vlan(SAI_STP_VLAN_2));
}

TEST_F(stpTest, stp_port_state)
{
    sai_attribute_t attr[SAI_STP_NO_OF_ATTRIB] = {0};
    sai_object_id_t stp_id = 0;
    sai_port_stp_port_state_t port_state;
    sai_object_id_t port_id = sai_port_id_1;
    sai_object_id_t invalid_port_id = sai_stp_invalid_port_id_get();

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_stp_api_tbl->
            create_stp(&stp_id,0,attr));

    EXPECT_EQ(SAI_STATUS_INVALID_OBJECT_ID, p_sai_stp_api_tbl->
            set_stp_port_state (stp_id, invalid_port_id, SAI_PORT_STP_STATE_FORWARDING));

    EXPECT_EQ(SAI_STATUS_INVALID_OBJECT_TYPE, p_sai_stp_api_tbl->
            set_stp_port_state (10, port_id, SAI_PORT_STP_STATE_FORWARDING));

    EXPECT_EQ(SAI_STATUS_INVALID_OBJECT_TYPE, p_sai_stp_api_tbl->
            get_stp_port_state (10, port_id, &port_state));

    /* Set to some random number */
    port_state = SAI_PORT_STP_STATE_LEARNING;
    EXPECT_EQ(SAI_STATUS_INVALID_OBJECT_ID, p_sai_stp_api_tbl->
            get_stp_port_state (stp_id, invalid_port_id, &port_state));

    EXPECT_EQ(SAI_STATUS_SUCCESS, p_sai_stp_api_tbl->
            set_stp_port_state (stp_id, port_id, SAI_PORT_STP_STATE_FORWARDING));

    /* Set to some random number */
    port_state = SAI_PORT_STP_STATE_LEARNING;
    EXPECT_EQ(SAI_STATUS_SUCCESS, p_sai_stp_api_tbl->
            get_stp_port_state (stp_id, port_id, &port_state));

    EXPECT_EQ(SAI_PORT_STP_STATE_FORWARDING, port_state);

    EXPECT_EQ(SAI_STATUS_SUCCESS, p_sai_stp_api_tbl->
            set_stp_port_state (stp_id, port_id, SAI_PORT_STP_STATE_LEARNING));

    /* Set to some random number */
    port_state = SAI_PORT_STP_STATE_BLOCKING;
    EXPECT_EQ(SAI_STATUS_SUCCESS, p_sai_stp_api_tbl->
            get_stp_port_state (stp_id, port_id, &port_state));

    EXPECT_EQ(SAI_PORT_STP_STATE_LEARNING, port_state);

    EXPECT_EQ(SAI_STATUS_SUCCESS, p_sai_stp_api_tbl->
            set_stp_port_state (stp_id, port_id, SAI_PORT_STP_STATE_BLOCKING));

    /* Set to some random number */
    port_state = SAI_PORT_STP_STATE_LEARNING;
    EXPECT_EQ(SAI_STATUS_SUCCESS, p_sai_stp_api_tbl->
            get_stp_port_state (stp_id, port_id, &port_state));

    EXPECT_EQ(SAI_PORT_STP_STATE_BLOCKING, port_state);

    EXPECT_EQ(SAI_STATUS_SUCCESS, p_sai_stp_api_tbl->
            remove_stp(stp_id));
}

TEST_F(stpTest, delet_vlan)
{
    sai_attribute_t attr[SAI_STP_NO_OF_ATTRIB] = {0};
    sai_object_id_t stp_id = 0;

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_stp_api_tbl->
            create_stp(&stp_id,0,attr));

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_vlan_api_tbl->
            create_vlan(SAI_STP_VLAN_2));

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_vlan_api_tbl->
            create_vlan(SAI_STP_VLAN_3));

    attr[0].id = SAI_VLAN_ATTR_STP_INSTANCE;
    attr[0].value.oid = stp_id;

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_vlan_api_tbl->
            set_vlan_attribute(SAI_STP_VLAN_3,attr));

    attr[0].id = SAI_VLAN_ATTR_STP_INSTANCE;
    attr[0].value.oid = 10;

    EXPECT_EQ(SAI_STATUS_INVALID_OBJECT_ID, p_sai_vlan_api_tbl->
            set_vlan_attribute(SAI_STP_VLAN_2, attr));

    attr[0].id = SAI_VLAN_ATTR_STP_INSTANCE;
    attr[0].value.oid = stp_id;

    EXPECT_EQ(SAI_STATUS_SUCCESS, p_sai_vlan_api_tbl->
            set_vlan_attribute(SAI_STP_VLAN_2, attr));

    memset (attr, 0, sizeof(attr));
    attr[0].id = SAI_VLAN_ATTR_STP_INSTANCE;

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_vlan_api_tbl->
            get_vlan_attribute(SAI_STP_VLAN_3, SAI_STP_NO_OF_ATTRIB, attr));

    EXPECT_EQ (attr->value.oid, stp_id);

    memset (attr, 0, sizeof(attr));
    attr[0].id = SAI_VLAN_ATTR_STP_INSTANCE;

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_vlan_api_tbl->
            get_vlan_attribute(SAI_STP_VLAN_2, SAI_STP_NO_OF_ATTRIB, attr));

    EXPECT_EQ (attr->value.oid, stp_id);

    memset (attr, 0, sizeof(attr));
    attr[0].id = SAI_STP_ATTR_VLAN_LIST;
    attr[0].value.vlanlist.vlan_count = 1;
    attr[0].value.vlanlist.vlan_list = (sai_vlan_id_t *) calloc (attr[0].value.vlanlist.vlan_count,
                                       sizeof(sai_vlan_id_t));

    EXPECT_EQ(SAI_STATUS_BUFFER_OVERFLOW, p_sai_stp_api_tbl->
            get_stp_attribute(stp_id, SAI_STP_NO_OF_ATTRIB, attr));

    EXPECT_EQ (attr->value.vlanlist.vlan_count, SAI_STP_VLAN_2);

    attr[0].id = SAI_STP_ATTR_VLAN_LIST;
    attr[0].value.vlanlist.vlan_list = (sai_vlan_id_t *) realloc (attr[0].value.vlanlist.vlan_list,
                                       sizeof(sai_vlan_id_t));

    EXPECT_EQ(SAI_STATUS_SUCCESS, p_sai_stp_api_tbl->
            get_stp_attribute(stp_id, SAI_STP_NO_OF_ATTRIB, attr));

    EXPECT_EQ (attr->value.vlanlist.vlan_count, 2);
    EXPECT_EQ (attr->value.vlanlist.vlan_list[0], SAI_STP_VLAN_2);
    EXPECT_EQ (attr->value.vlanlist.vlan_list[1], SAI_STP_VLAN_3);

    free (attr[0].value.vlanlist.vlan_list);

    EXPECT_EQ(SAI_STATUS_OBJECT_IN_USE, p_sai_stp_api_tbl->
            remove_stp(stp_id));

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_vlan_api_tbl->
            remove_vlan(SAI_STP_VLAN_3));

    EXPECT_EQ(SAI_STATUS_SUCCESS,p_sai_vlan_api_tbl->
            remove_vlan(SAI_STP_VLAN_2));

    EXPECT_EQ(SAI_STATUS_SUCCESS, p_sai_stp_api_tbl->
            remove_stp(stp_id));

    EXPECT_EQ(SAI_STATUS_INVALID_OBJECT_ID, p_sai_stp_api_tbl->
            remove_stp(stp_id));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
