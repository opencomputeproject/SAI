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
*    sai_lag_unit_test.h
*
* Abstract:
*
*    This file contains definition of APIs used by the SAI LAG unit test module
*
*************************************************************************/

#ifndef __SAI_LAG_UNIT_TEST_H__
#define __SAI_LAG_UNIT_TEST_H__

#include "gtest/gtest.h"

extern "C" {
#include "saiswitch.h"
#include "saistp.h"
#include "saitypes.h"

void sai_port_evt_callback (uint32_t count,
                            sai_port_event_notification_t *data);

sai_status_t sai_lag_ut_add_port_to_lag (sai_lag_api_t   *lag_api,
                                         sai_object_id_t *member_id,
                                         sai_object_id_t  lag_id,
                                         sai_object_id_t  port_id,
                                         bool is_ing_disable_present,
                                         bool ing_disable,
                                         bool is_egr_disable_present,
                                         bool egr_disable);

sai_status_t sai_lag_ut_remove_port_from_lag (sai_lag_api_t   *lag_api,
                                              sai_object_id_t  member_id);
sai_status_t sai_lag_ut_get_lag_attr (sai_lag_api_t   *lag_api,
                                      sai_object_id_t  member_id,
                                      bool            *ing_disable,
                                      bool            *egr_disable);

sai_status_t sai_lag_ut_set_lag_attr (sai_lag_api_t   *lag_api,
                                      sai_object_id_t member_id,
                                      bool is_ing_disable_present,
                                      bool ing_disable,
                                      bool is_egr_disable_present,
                                      bool egr_disable);
}

#define SAI_MAX_PORTS     256
#define SAI_LAG_UT_DEFAULT_VLAN 1

extern uint32_t port_count;
extern sai_object_id_t port_list[SAI_MAX_PORTS];

/*
 * API query is done while running the first test case and
 * the method table is stored in sai_lag_api_table so
 * that its available for the rest of the test cases which
 * use the method table
 */
class lagInit : public ::testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            sai_switch_api_t *p_sai_switch_api_tbl = NULL;
            sai_switch_notification_t notification;

            /*
             * Query and populate the SAI Switch API Table.
             */
            memset(&notification, 0, sizeof(notification));
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
            notification.on_packet_event = sai_packet_event_callback;
            notification.on_port_event = sai_port_evt_callback;

            ASSERT_TRUE(p_sai_switch_api_tbl->initialize_switch != NULL);

            EXPECT_EQ (SAI_STATUS_SUCCESS,
                       (p_sai_switch_api_tbl->initialize_switch (0, NULL, NULL,
                                                                 &notification)));

            ASSERT_EQ(NULL,sai_api_query(SAI_API_LAG,
                                         (static_cast<void**>(static_cast<void*>(&sai_lag_api_table)))));

            ASSERT_TRUE(sai_lag_api_table != NULL);

            EXPECT_TRUE(sai_lag_api_table->create_lag != NULL);
            EXPECT_TRUE(sai_lag_api_table->remove_lag != NULL);
            EXPECT_TRUE(sai_lag_api_table->set_lag_attribute != NULL);
            EXPECT_TRUE(sai_lag_api_table->get_lag_attribute != NULL);
            EXPECT_TRUE(sai_lag_api_table->create_lag_member != NULL);
            EXPECT_TRUE(sai_lag_api_table->remove_lag_member != NULL);
            EXPECT_TRUE(sai_lag_api_table->set_lag_member_attribute != NULL);
            EXPECT_TRUE(sai_lag_api_table->get_lag_member_attribute != NULL);

            ASSERT_TRUE(port_count!=0);
            port_id_1  = port_list[0];
            port_id_2  = port_list[port_count-1];
            port_id_l3 = port_list [1];
            port_id_invalid = port_id_2 + 1;
        }

        static sai_lag_api_t* sai_lag_api_table;
        static sai_object_id_t  port_id_1;
        static sai_object_id_t  port_id_2;
        static sai_object_id_t  port_id_l3;
        static sai_object_id_t  port_id_invalid;

    public:
        sai_lag_api_t *sai_lag_ut_get_lag_api () {return sai_lag_api_table;}
};
#endif /* __SAI_LAG_UNIT_TEST_H__ */
