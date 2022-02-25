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
*    sai_lag_unit_test.cpp
*
* Abstract:
*
*    This file contains APIS for testing the SAI LAG module
*
*************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <inttypes.h>

extern "C" {
#include "sai.h"
#include "sailag.h"
#include "saitypes.h"
#include "sai_l2_unit_test_defs.h"
}

#define SAI_MAX_PORTS     256
#define SAI_LAG_UT_DEFAULT_VLAN 1

uint32_t port_count = 0;
sai_object_id_t port_list[SAI_MAX_PORTS] = {0};

#include "sai_lag_unit_test.h"

sai_lag_api_t* lagInit ::sai_lag_api_table = NULL;
sai_object_id_t lagInit ::port_id_1 = 0;
sai_object_id_t lagInit ::port_id_2 = 0;
sai_object_id_t lagInit ::port_id_l3 = 0;
sai_object_id_t lagInit ::port_id_invalid = 0;

void sai_port_evt_callback (uint32_t count, sai_port_event_notification_t *data)
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

sai_status_t sai_lag_ut_add_port_to_lag (sai_lag_api_t   *lag_api,
                                         sai_object_id_t *member_id,
                                         sai_object_id_t  lag_id,
                                         sai_object_id_t  port_id,
                                         bool is_ing_disable_present,
                                         bool ing_disable,
                                         bool is_egr_disable_present,
                                         bool egr_disable)
{
    sai_attribute_t attr_list [4];
    uint32_t        count = 0;

    memset (attr_list, 0, sizeof (attr_list));

    attr_list [count].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attr_list [count].value.oid = lag_id;
    count++;

    attr_list [count].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attr_list [count].value.oid = port_id;
    count++;

    if (is_ing_disable_present) {
        attr_list [count].id = SAI_LAG_MEMBER_ATTR_INGRESS_DISABLE;
        attr_list [count].value.booldata = ing_disable;
        count++;
    }

    if (is_egr_disable_present) {
        attr_list [count].id = SAI_LAG_MEMBER_ATTR_EGRESS_DISABLE;
        attr_list [count].value.booldata = egr_disable;
        count++;
    }

    return lag_api->create_lag_member (member_id, count, attr_list);
}

sai_status_t sai_lag_ut_get_lag_attr (sai_lag_api_t   *lag_api,
                                      sai_object_id_t  member_id,
                                      bool            *ing_disable,
                                      bool            *egr_disable)
{
    sai_status_t    rc;
    sai_attribute_t attr_list [4];
    uint32_t        count = 0;

    memset (attr_list, 0, sizeof (attr_list));

    attr_list [count].id = SAI_LAG_MEMBER_ATTR_INGRESS_DISABLE;
    attr_list [count].value.booldata = ing_disable;
    count++;

    attr_list [count].id = SAI_LAG_MEMBER_ATTR_EGRESS_DISABLE;
    attr_list [count].value.booldata = egr_disable;
    count++;

    rc = lag_api->get_lag_member_attribute (member_id, count, attr_list);

    *ing_disable = attr_list [0].value.booldata;
    *egr_disable = attr_list [1].value.booldata;

    return rc;
}

sai_status_t sai_lag_ut_set_lag_attr (sai_lag_api_t   *lag_api,
                                      sai_object_id_t member_id,
                                      bool is_ing_disable_present,
                                      bool ing_disable,
                                      bool is_egr_disable_present,
                                      bool egr_disable)
{
    sai_status_t    rc;
    sai_attribute_t attr;

    memset (&attr, 0, sizeof (attr));

    if (is_ing_disable_present) {
        attr.id = SAI_LAG_MEMBER_ATTR_INGRESS_DISABLE;
        attr.value.booldata = ing_disable;

        rc = lag_api->set_lag_member_attribute (member_id, &attr);

        if (rc != SAI_STATUS_SUCCESS) {
            return rc;
        }
    }

    if (is_egr_disable_present) {
        attr.id = SAI_LAG_MEMBER_ATTR_EGRESS_DISABLE;
        attr.value.booldata = egr_disable;

        rc = lag_api->set_lag_member_attribute (member_id, &attr);

        if (rc != SAI_STATUS_SUCCESS) {
            return rc;
        }
    }

    return SAI_STATUS_SUCCESS;
}

sai_status_t sai_lag_ut_remove_port_from_lag (sai_lag_api_t   *lag_api,
                                              sai_object_id_t  member_id)
{
    return lag_api->remove_lag_member (member_id);
}

/*
 * LAG create and delete
 */
TEST_F(lagInit, create_remove_lag)
{
    sai_object_id_t lag_id = 0;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_api_table->create_lag(&lag_id, 0, NULL));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_api_table->remove_lag(lag_id));

    ASSERT_EQ(SAI_STATUS_ITEM_NOT_FOUND,
              sai_lag_api_table->remove_lag(lag_id));

}


/*
 * LAG port priority tag testing
 */
TEST_F(lagInit, lag_attribute_test)
{
    sai_object_id_t lag_id = 0;
    sai_object_id_t port_arr[2];
    sai_object_id_t get_port_arr[2];
    sai_object_list_t lag_port_list;
    sai_object_list_t get_lag_port_list;
    sai_object_id_t member_id_1 = 0;
    sai_object_id_t member_id_2 = 0;
    sai_attribute_t attr;
    sai_attribute_t get_attr;

    lag_port_list.count = 2;
    port_arr[0] = port_id_1;
    port_arr[1] = port_id_2;
    lag_port_list.list = port_arr;

    memset(&attr, 0, sizeof(attr));
    attr.id = SAI_LAG_ATTR_PORT_LIST;
    attr.value.objlist = lag_port_list;


    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_api_table->create_lag(&lag_id, 0, NULL));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_ut_add_port_to_lag (sai_lag_api_table, &member_id_1,
                                          lag_id, port_id_1,
                                          false, false, false, false));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_ut_add_port_to_lag (sai_lag_api_table, &member_id_2,
                                          lag_id, port_id_2,
                                          false, false, false, false));

    memset(&get_attr, 0, sizeof(get_attr));
    memset(get_port_arr, 0, sizeof(get_port_arr));
    memset(&get_lag_port_list, 0, sizeof(get_lag_port_list));
    get_lag_port_list.count = 2;
    get_lag_port_list.list = get_port_arr;
    get_attr.id = SAI_LAG_ATTR_PORT_LIST;
    get_attr.value.objlist = get_lag_port_list;

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_api_table->get_lag_attribute(lag_id, 1, &get_attr));
    EXPECT_EQ(port_arr[0], get_port_arr[0]);
    EXPECT_EQ(port_arr[1], get_port_arr[1]);

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_api_table->remove_lag(lag_id));
}

TEST_F(lagInit, lag_member_new_api_test)
{
    sai_object_id_t lag_id_1 = 0;
    sai_object_id_t lag_id_2 = 0;
    sai_object_id_t member_id_1 = 0;
    sai_object_id_t member_id_2 = 0;
    sai_object_id_t tmp_member_id = 0;
    sai_attribute_t attr_list [4];
    uint32_t        count;

    /* Create Lag lag_id_1 and add port_id_1 and port_id_2 as members */
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_api_table->create_lag(&lag_id_1, 0, NULL));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_ut_add_port_to_lag (sai_lag_api_table, &member_id_1,
                                          lag_id_1, port_id_1,
                                          false, false, false, false));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_ut_add_port_to_lag (sai_lag_api_table, &member_id_2,
                                          lag_id_1, port_id_2,
                                          false, false, true, true));

    /* Check if same port cannot be added to two lags */
    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_api_table->create_lag (&lag_id_2, 0, NULL));

    ASSERT_EQ(SAI_STATUS_INVALID_PORT_MEMBER,
              sai_lag_ut_add_port_to_lag (sai_lag_api_table, &tmp_member_id,
                                          lag_id_2, port_id_1,
                                          false, false, false, false));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_api_table->remove_lag(lag_id_2));

    /* Check if the addition of a lag member fails if it is already a member */
    ASSERT_EQ(SAI_STATUS_ITEM_ALREADY_EXISTS,
              sai_lag_ut_add_port_to_lag (sai_lag_api_table, &tmp_member_id,
                                          lag_id_1, port_id_1,
                                          false, false, false, false));

    /* Check if invalid port is not allowed to be added to the lag */
    ASSERT_EQ(SAI_STATUS_INVALID_PARAMETER,
              sai_lag_ut_add_port_to_lag (sai_lag_api_table, &tmp_member_id,
                                          lag_id_1, port_id_invalid,
                                          false, false, false, false));

    /* Check if member removal with an invalid member_id fails */
    ASSERT_EQ(SAI_STATUS_ITEM_NOT_FOUND,
              sai_lag_ut_remove_port_from_lag (sai_lag_api_table, 0));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_ut_remove_port_from_lag (sai_lag_api_table, member_id_1));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_ut_remove_port_from_lag (sai_lag_api_table, member_id_2));

    /* Check if member removal fails, if it is not a member of the lag */
    ASSERT_EQ(SAI_STATUS_ITEM_NOT_FOUND,
              sai_lag_ut_remove_port_from_lag (sai_lag_api_table, member_id_1));

    ASSERT_EQ(SAI_STATUS_ITEM_NOT_FOUND,
              sai_lag_ut_remove_port_from_lag (sai_lag_api_table, member_id_2));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_api_table->remove_lag(lag_id_1));

    /* Check if member addition fails if the lag is not created. */
    ASSERT_EQ(SAI_STATUS_ITEM_NOT_FOUND,
              sai_lag_ut_add_port_to_lag (sai_lag_api_table, &tmp_member_id,
                                          lag_id_1, port_id_1,
                                          false, false, false, false));

    ASSERT_EQ(SAI_STATUS_ITEM_NOT_FOUND,
              sai_lag_ut_add_port_to_lag (sai_lag_api_table, &tmp_member_id,
                                          lag_id_1, port_id_2,
                                          false, false, false, false));

    ASSERT_EQ(SAI_STATUS_SUCCESS,
              sai_lag_api_table->create_lag (&lag_id_1, 0, NULL));

    /* Check if member addition fails, if mandatory arguments are not present */
    memset (attr_list, 0, sizeof (attr_list));
    count = 0;

    attr_list [count].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attr_list [count].value.oid = lag_id_1;
    count++;

    ASSERT_EQ (SAI_MANDATORY_ATTRIBUTE_MISSING,
               sai_lag_api_table->create_lag_member (&tmp_member_id,
                                                     count, attr_list));

    count = 0;
    attr_list [count].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attr_list [count].value.oid = port_id_1;
    count++;

    ASSERT_EQ (SAI_MANDATORY_ATTRIBUTE_MISSING,
               sai_lag_api_table->create_lag_member (&tmp_member_id,
                                                     count, attr_list));

    count = 0;
    attr_list [count].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attr_list [count].value.oid = lag_id_1;
    count++;

    attr_list [count].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attr_list [count].value.oid = port_id_1;
    count++;

    attr_list [count].id = SAI_LAG_MEMBER_ATTR_CUSTOM_RANGE_BASE + 1000;
    attr_list [count].value.oid = 0;
    count++;

    /* Check if member addition fails if unknown attribute is present. */
    ASSERT_EQ (SAI_STATUS_UNKNOWN_ATTRIBUTE_0,
               sai_lag_api_table->create_lag_member (&tmp_member_id,
                                                     count, attr_list));

}

TEST_F(lagInit, lag_member_attribute_new_api_test)
{
    sai_object_id_t lag_id_1 = 0;
    sai_object_id_t member_id_1 = 0;
    sai_object_id_t member_id_2 = 0;
    bool            ing_disable;
    bool            egr_disable;

    /*
     * Create a lag with members assigned default ingress/egress disable
     * attribute values.
     */
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_api_table->create_lag (&lag_id_1, 0, NULL));

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_ut_add_port_to_lag (sai_lag_api_table, &member_id_1,
                                           lag_id_1, port_id_1,
                                           false, false, false, false));

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_ut_add_port_to_lag (sai_lag_api_table, &member_id_2,
                                           lag_id_1, port_id_2,
                                           false, false, false, false));

    /*
     * Check if the get attribute returns correct values for ingress/egress
     * disable attributes.
     */
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_ut_get_lag_attr (sai_lag_api_table, member_id_1,
                                        &ing_disable, &egr_disable));
    ASSERT_FALSE (ing_disable);
    ASSERT_FALSE (egr_disable);

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_ut_get_lag_attr (sai_lag_api_table, member_id_2,
                                        &ing_disable, &egr_disable));
    ASSERT_FALSE (ing_disable);
    ASSERT_FALSE (egr_disable);

    /*
     * Modify the attributes and check if the get attribute returns
     * correct values for ingress/egress disable attributes.
     */
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_ut_set_lag_attr (sai_lag_api_table, member_id_1,
                                        true, true, true, false));
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_ut_get_lag_attr (sai_lag_api_table, member_id_1,
                                        &ing_disable, &egr_disable));
    ASSERT_TRUE (ing_disable);
    ASSERT_FALSE (egr_disable);

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_ut_set_lag_attr (sai_lag_api_table, member_id_2,
                                        false, false, true, true));
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_ut_get_lag_attr (sai_lag_api_table, member_id_2,
                                        &ing_disable, &egr_disable));
    ASSERT_FALSE (ing_disable);
    ASSERT_TRUE (egr_disable);

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_ut_set_lag_attr (sai_lag_api_table, member_id_2,
                                        false, false, true, true));
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_ut_get_lag_attr (sai_lag_api_table, member_id_2,
                                        &ing_disable, &egr_disable));
    ASSERT_FALSE (ing_disable);
    ASSERT_TRUE (egr_disable);

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_ut_remove_port_from_lag(sai_lag_api_table, member_id_1));
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_ut_remove_port_from_lag(sai_lag_api_table, member_id_2));

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_lag_api_table->remove_lag (lag_id_1));
}
