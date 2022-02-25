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
*    sai_fdb_unit_test.cpp
*
* Abstract:
*
*    This file contains APIS for testing the SAI FDB module
*
*************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include "inttypes.h"

extern "C" {
#include "sai.h"
#include "saifdb.h"
#include "saitypes.h"
#include "saiswitch.h"
#include "sai_l2_unit_test_defs.h"
}

#define SAI_MAX_PORTS  256
#define SAI_MAX_FDB_ATTRIBUTES 3

static uint32_t port_count = 0;
static sai_object_id_t port_list[SAI_MAX_PORTS] = {0};

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

static inline void sai_set_test_fdb_entry(sai_fdb_entry_t* fdb_entry)
{
    memset(fdb_entry,0, sizeof(sai_fdb_entry_t));
    fdb_entry->mac_address[5] = 0xa;
    fdb_entry->vlan_id = SAI_GTEST_VLAN;
}

/*
 * API query is done while running the first test case and
 * the method table is stored in sai_fdb_api_table so
 * that its available for the rest of the test cases which
 * use the method table
 */
class fdbInit : public ::testing::Test
{
    public:
        void sai_fdb_entry_create(void);
    protected:
        static void SetUpTestCase()
        {
            sai_switch_api_t *p_sai_switch_api_tbl = NULL;
            sai_switch_notification_t notification;
            memset (&notification, 0, sizeof(sai_switch_notification_t));

            /*
             * Query and populate the SAI Switch API Table.
             */
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
                       (SAI_API_SWITCH, (static_cast<void**>
                                         (static_cast<void*>(&p_sai_switch_api_tbl)))));

            ASSERT_TRUE (p_sai_switch_api_tbl != NULL);

            /*
             * Switch Initialization.
             * Fill in notification callback routines with stubs.
             */
            notification.on_switch_state_change = sai_switch_operstate_callback;
            notification.on_fdb_event = sai_fdb_evt_callback;
            notification.on_port_state_change = sai_port_state_evt_callback;
            notification.on_switch_shutdown_request = sai_switch_shutdown_callback;
            notification.on_port_event = sai_port_evt_callback;
            notification.on_packet_event = sai_packet_event_callback;

            ASSERT_TRUE(p_sai_switch_api_tbl->initialize_switch != NULL);

            EXPECT_EQ (SAI_STATUS_SUCCESS,
                       (p_sai_switch_api_tbl->initialize_switch (0, NULL, NULL,
                                                                 &notification)));

            ASSERT_EQ(NULL,sai_api_query(SAI_API_FDB,
                                         (static_cast<void**>(static_cast<void*>(&sai_fdb_api_table)))));

            ASSERT_TRUE(sai_fdb_api_table != NULL);

            EXPECT_TRUE(sai_fdb_api_table->create_fdb_entry != NULL);
            EXPECT_TRUE(sai_fdb_api_table->remove_fdb_entry != NULL);
            EXPECT_TRUE(sai_fdb_api_table->get_fdb_entry_attribute != NULL);
            EXPECT_TRUE(sai_fdb_api_table->set_fdb_entry_attribute != NULL);
            EXPECT_TRUE(sai_fdb_api_table->flush_fdb_entries != NULL);

            port_id_1 = port_list[0];
            port_id_2 = port_list[port_count-1];
        }
        static sai_fdb_api_t* sai_fdb_api_table;
        static sai_object_id_t  port_id_1;
        static sai_object_id_t  port_id_2;
};

sai_fdb_api_t* fdbInit ::sai_fdb_api_table = NULL;
sai_object_id_t fdbInit ::port_id_1 = 0;
sai_object_id_t fdbInit ::port_id_2 = 0;

void fdbInit ::sai_fdb_entry_create(void)
{

    sai_fdb_entry_t fdb_entry;
    sai_attribute_t attr_list[SAI_MAX_FDB_ATTRIBUTES];

    sai_set_test_fdb_entry(&fdb_entry);

    memset(attr_list,0, sizeof(attr_list));
    attr_list[0].id = SAI_FDB_ENTRY_ATTR_TYPE;
    attr_list[0].value.s32 = SAI_FDB_ENTRY_DYNAMIC;

    attr_list[1].id = SAI_FDB_ENTRY_ATTR_PORT_ID;
    attr_list[1].value.oid = port_id_1;

    attr_list[2].id = SAI_FDB_ENTRY_ATTR_PACKET_ACTION;
    attr_list[2].value.s32 = SAI_PACKET_ACTION_FORWARD;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->create_fdb_entry(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 SAI_MAX_FDB_ATTRIBUTES,
                                 (const sai_attribute_t*)attr_list));
}

/*
 * FDB Create and Get FDB Entry
 */
TEST_F(fdbInit, create_fdb_entry)
{
    sai_fdb_entry_t fdb_entry;
    sai_attribute_t get_attr_list[SAI_MAX_FDB_ATTRIBUTES];

    sai_fdb_entry_create();
    sai_set_test_fdb_entry(&fdb_entry);
    memset(get_attr_list,0, sizeof(get_attr_list));
    get_attr_list[0].id = SAI_FDB_ENTRY_ATTR_TYPE;
    get_attr_list[1].id = SAI_FDB_ENTRY_ATTR_PORT_ID;
    get_attr_list[2].id = SAI_FDB_ENTRY_ATTR_PACKET_ACTION;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->get_fdb_entry_attribute(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 SAI_MAX_FDB_ATTRIBUTES,
                                 get_attr_list));

    ASSERT_EQ(SAI_FDB_ENTRY_DYNAMIC, get_attr_list[0].value.s32);
    ASSERT_EQ(port_id_1, get_attr_list[1].value.oid);
    ASSERT_EQ(SAI_PACKET_ACTION_FORWARD, get_attr_list[2].value.s32);

}

/*
 * FDB Create and Get FDB Entry
 */
TEST_F(fdbInit, remove_fdb_entry)
{
    sai_fdb_entry_t fdb_entry;
    sai_attribute_t get_attr_list[SAI_MAX_FDB_ATTRIBUTES];

    sai_set_test_fdb_entry(&fdb_entry);
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->remove_fdb_entry(
                                 (const sai_fdb_entry_t*)&fdb_entry));
    memset(get_attr_list,0, sizeof(get_attr_list));
    get_attr_list[0].id = SAI_FDB_ENTRY_ATTR_TYPE;
    get_attr_list[1].id = SAI_FDB_ENTRY_ATTR_PORT_ID;
    get_attr_list[2].id = SAI_FDB_ENTRY_ATTR_PACKET_ACTION;

    ASSERT_EQ(SAI_STATUS_ADDR_NOT_FOUND,
              sai_fdb_api_table->get_fdb_entry_attribute(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 SAI_MAX_FDB_ATTRIBUTES,
                                 get_attr_list));

}
TEST_F(fdbInit, flush_all_fdb_entries_by_port)
{

    sai_fdb_entry_t fdb_entry;
    sai_attribute_t get_attr_list[SAI_MAX_FDB_ATTRIBUTES];
    sai_attribute_t flush_attr[2];

    sai_fdb_entry_create();
    sai_set_test_fdb_entry(&fdb_entry);
    memset(get_attr_list,0, sizeof(get_attr_list));
    memset(flush_attr,0, sizeof(flush_attr));

    flush_attr[0].id = SAI_FDB_FLUSH_ATTR_PORT_ID;
    flush_attr[0].value.oid = port_id_1;

    flush_attr[1].id = SAI_FDB_FLUSH_ATTR_ENTRY_TYPE;
    flush_attr[1].value.s32 = SAI_FDB_FLUSH_ENTRY_DYNAMIC;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->flush_fdb_entries(2,
                                 (const sai_attribute_t*)flush_attr));

    get_attr_list[0].id = SAI_FDB_ENTRY_ATTR_TYPE;
    get_attr_list[1].id = SAI_FDB_ENTRY_ATTR_PORT_ID;
    get_attr_list[2].id = SAI_FDB_ENTRY_ATTR_PACKET_ACTION;

    ASSERT_EQ(SAI_STATUS_ADDR_NOT_FOUND,
              sai_fdb_api_table->get_fdb_entry_attribute(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 SAI_MAX_FDB_ATTRIBUTES,
                                 get_attr_list));
}

TEST_F(fdbInit, flush_all_fdb_entries_by_vlan)
{
    sai_fdb_entry_t fdb_entry;
    sai_attribute_t get_attr_list[SAI_MAX_FDB_ATTRIBUTES];
    sai_attribute_t flush_attr[2];

    sai_fdb_entry_create();
    sai_set_test_fdb_entry(&fdb_entry);
    memset(get_attr_list,0, sizeof(get_attr_list));
    memset(flush_attr,0, sizeof(flush_attr));

    flush_attr[0].id = SAI_FDB_FLUSH_ATTR_VLAN_ID;
    flush_attr[0].value.u16 =  SAI_GTEST_VLAN;

    flush_attr[1].id = SAI_FDB_FLUSH_ATTR_ENTRY_TYPE;
    flush_attr[1].value.s32 = SAI_FDB_FLUSH_ENTRY_DYNAMIC;


    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->flush_fdb_entries(2,
                                 (const sai_attribute_t*)flush_attr));
    get_attr_list[0].id = SAI_FDB_ENTRY_ATTR_TYPE;
    get_attr_list[1].id = SAI_FDB_ENTRY_ATTR_PORT_ID;
    get_attr_list[2].id = SAI_FDB_ENTRY_ATTR_PACKET_ACTION;

    ASSERT_EQ(SAI_STATUS_ADDR_NOT_FOUND,
              sai_fdb_api_table->get_fdb_entry_attribute(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 SAI_MAX_FDB_ATTRIBUTES,
                                 get_attr_list));
}
TEST_F(fdbInit, flush_all_fdb_entries_by_port_vlan)
{
    sai_fdb_entry_t fdb_entry;
    sai_attribute_t get_attr_list[SAI_MAX_FDB_ATTRIBUTES];
    sai_attribute_t flush_attr[3];

    sai_fdb_entry_create();
    sai_set_test_fdb_entry(&fdb_entry);
    memset(get_attr_list,0, sizeof(get_attr_list));
    memset(flush_attr,0, sizeof(flush_attr));

    flush_attr[0].id = SAI_FDB_FLUSH_ATTR_VLAN_ID;
    flush_attr[0].value.u16 =  SAI_GTEST_VLAN;

    flush_attr[1].id = SAI_FDB_FLUSH_ATTR_PORT_ID;
    flush_attr[1].value.oid =  port_id_1;

    flush_attr[2].id = SAI_FDB_FLUSH_ATTR_ENTRY_TYPE;
    flush_attr[2].value.s32 = SAI_FDB_FLUSH_ENTRY_DYNAMIC;


    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->flush_fdb_entries(3,
                                 (const sai_attribute_t*)flush_attr));

    get_attr_list[0].id = SAI_FDB_ENTRY_ATTR_TYPE;
    get_attr_list[1].id = SAI_FDB_ENTRY_ATTR_PORT_ID;
    get_attr_list[2].id = SAI_FDB_ENTRY_ATTR_PACKET_ACTION;

    ASSERT_EQ(SAI_STATUS_ADDR_NOT_FOUND,
              sai_fdb_api_table->get_fdb_entry_attribute(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 SAI_MAX_FDB_ATTRIBUTES,
                                 get_attr_list));
}
TEST_F(fdbInit, flush_all_fdb_entries)
{
    sai_fdb_entry_t fdb_entry;
    sai_attribute_t get_attr_list[SAI_MAX_FDB_ATTRIBUTES];
    sai_attribute_t flush_attr[1];

    sai_fdb_entry_create();
    sai_set_test_fdb_entry(&fdb_entry);
    memset(get_attr_list,0, sizeof(get_attr_list));
    memset(flush_attr,0, sizeof(flush_attr));

    flush_attr[0].id = SAI_FDB_FLUSH_ATTR_ENTRY_TYPE;
    flush_attr[0].value.s32 = SAI_FDB_FLUSH_ENTRY_DYNAMIC;


    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->flush_fdb_entries(1,
                                 (const sai_attribute_t*)flush_attr));

    get_attr_list[0].id = SAI_FDB_ENTRY_ATTR_TYPE;
    get_attr_list[1].id = SAI_FDB_ENTRY_ATTR_PORT_ID;
    get_attr_list[2].id = SAI_FDB_ENTRY_ATTR_PACKET_ACTION;

    ASSERT_EQ(SAI_STATUS_ADDR_NOT_FOUND,
              sai_fdb_api_table->get_fdb_entry_attribute(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 SAI_MAX_FDB_ATTRIBUTES,
                                 get_attr_list));
}
/*
 * FDB Create and Get FDB Entry
 */
TEST_F(fdbInit, set_fdb_entry)
{
    sai_fdb_entry_t fdb_entry;
    sai_attribute_t get_attr;
    sai_attribute_t set_attr;

    sai_fdb_entry_create();
    sai_set_test_fdb_entry(&fdb_entry);

    /*Changing type*/
    memset(&set_attr,0, sizeof(set_attr));
    set_attr.id = SAI_FDB_ENTRY_ATTR_TYPE;
    set_attr.value.s32 = SAI_FDB_ENTRY_STATIC;

    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->set_fdb_entry_attribute(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 (const sai_attribute_t*)&set_attr));

    memset(&get_attr,0, sizeof(get_attr));
    get_attr.id = SAI_FDB_ENTRY_ATTR_TYPE;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->get_fdb_entry_attribute(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 1, &get_attr));
    EXPECT_EQ(SAI_FDB_ENTRY_STATIC, get_attr.value.s32);

    /*Changing action*/
    memset(&set_attr,0, sizeof(set_attr));
    set_attr.id = SAI_FDB_ENTRY_ATTR_PACKET_ACTION;
    set_attr.value.s32 = SAI_PACKET_ACTION_DROP;

    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->set_fdb_entry_attribute(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 (const sai_attribute_t*)&set_attr));

    memset(&get_attr,0, sizeof(get_attr));
    get_attr.id = SAI_FDB_ENTRY_ATTR_PACKET_ACTION;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->get_fdb_entry_attribute(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 1, &get_attr));
    EXPECT_EQ(SAI_PACKET_ACTION_DROP, get_attr.value.s32);

    /*Changing port*/
    memset(&set_attr,0, sizeof(set_attr));
    set_attr.id = SAI_FDB_ENTRY_ATTR_PORT_ID;
    set_attr.value.oid = port_id_2;

    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->set_fdb_entry_attribute(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 (const sai_attribute_t*)&set_attr));

    memset(&get_attr,0, sizeof(get_attr));
    get_attr.id = SAI_FDB_ENTRY_ATTR_PORT_ID;
    EXPECT_EQ(SAI_STATUS_SUCCESS,
              sai_fdb_api_table->get_fdb_entry_attribute(
                                 (const sai_fdb_entry_t*)&fdb_entry,
                                 1, &get_attr));
    EXPECT_EQ(port_id_2, get_attr.value.oid);

}
