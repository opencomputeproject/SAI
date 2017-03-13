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
*    sai_infra_unit_test.cpp
*
* Abstract:
*
*    SAI INFRA UNIT TEST :- Covers the test cases for SAI API QUERY AND INIT
*
*************************************************************************/

#include "gtest/gtest.h"

extern "C" {
#include "sai.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
}

/*
 * Stubs for Callback functions to be passed from adapter host/application.
 */
#ifdef UNREFERENCED_PARAMETER
#elif defined(__GNUC__)
#define UNREFERENCED_PARAMETER(P)   (void)(P)
#else
#define UNREFERENCED_PARAMETER(P)   (P)
#endif

const char* profile_get_specific(sai_switch_profile_id_t profile_id,
                                 const char* variable)
{
    UNREFERENCED_PARAMETER(profile_id);
    UNREFERENCED_PARAMETER(variable);
    return NULL;
}

int profile_get_all(sai_switch_profile_id_t profile_id,
                    const char** variable,
                    const char** value)
{
    UNREFERENCED_PARAMETER(profile_id);
    UNREFERENCED_PARAMETER(variable);
    UNREFERENCED_PARAMETER(value);
    return 0;
}

/*
 * Pass the service method table and do API intialize.
 */
TEST(sai_unit_test, api_init)
{
    service_method_table_t  sai_service_method_table;

    sai_service_method_table.profile_get_value = profile_get_specific;
    sai_service_method_table.profile_get_next_value = profile_get_all;

    ASSERT_EQ(SAI_STATUS_SUCCESS,sai_api_initialize(0, &sai_service_method_table));
}

/*
 *Obtain the method table for the sai switch api.
 */
TEST(sai_unit_test, api_query)
{
    sai_switch_api_t *sai_switch_api_table = NULL;
    sai_switch_api_table = (sai_switch_api_t*)(malloc(sizeof(sai_switch_api_t)));

    ASSERT_TRUE(sai_switch_api_table != NULL);

    ASSERT_EQ(NULL,sai_api_query(SAI_API_SWITCH,
              (static_cast<void**>(static_cast<void*>(&sai_switch_api_table)))));

    EXPECT_TRUE(sai_switch_api_table->initialize_switch != NULL);
    EXPECT_TRUE(sai_switch_api_table->shutdown_switch != NULL);
    EXPECT_TRUE(sai_switch_api_table->connect_switch != NULL);
    EXPECT_TRUE(sai_switch_api_table->disconnect_switch != NULL);
    EXPECT_TRUE(sai_switch_api_table->set_switch_attribute != NULL);
    EXPECT_TRUE(sai_switch_api_table->get_switch_attribute != NULL);

}

/*
 *Verify if object_type_query returns OBJECT_TYPE_NULL for invalid object id.
 */
TEST(sai_unit_test, sai_object_type_query)
{
    sai_object_id_t  invalid_obj_id = 0;

    ASSERT_EQ(SAI_OBJECT_TYPE_NULL,sai_object_type_query(invalid_obj_id));
}

/*
 * Unintialize the SDK and free up the resources.
 */
TEST(sai_unit_test, api_uninit)
{
    ASSERT_EQ(SAI_STATUS_SUCCESS,sai_api_uninitialize());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

