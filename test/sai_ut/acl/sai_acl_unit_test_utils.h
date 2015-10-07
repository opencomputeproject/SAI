/************************************************************************
 * * Copyright (c) 2015 Dell Inc. 
 * *   
 * *    Licensed under the Apache License, Version 2.0 (the "License"); you may 
 * *    not use this file except in compliance with the License. You may obtain 
 * *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 * *
 * *    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR 
 * *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS 
 * *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 * *
 * *    See the Apache Version 2.0 License for specific language governing 
 * *    permissions and limitations under the License. 
 * *
 * *
 * * Module Name:
 * *
 * *    sai_acl_unit_test_utils.h
 * *     
 * * Abstract:
 * *
 * *    This contains the base class definition having the utility and helper 
 * *    functions for testing the SAI ACL functionalities.
 * *
 * *************************************************************************/

#ifndef __SAI_ACL_UNIT_TEST_H__
#define __SAI_ACL_UNIT_TEST_H__

#include "gtest/gtest.h"

extern "C" {
#include "saitypes.h"
#include "saistatus.h"
#include "saiswitch.h"
#include "saiacl.h"
#include "sailag.h"
#include "saimirror.h"
}

class saiACLTest : public ::testing::Test
{
    public:
        static void SetUpTestCase();

        /* Get SAI port id for acl test cases */
        static sai_object_id_t sai_acl_port_id_get (uint32_t port_index);
        static sai_object_id_t sai_acl_invalid_port_id_get ();
        static sai_object_id_t sai_test_acl_get_cpu_port ();

        /* Get SAI ACL range based on attributes */
        static bool sai_test_acl_table_field_range (sai_attr_id_t id);
        static bool sai_test_acl_rule_field_range (sai_attr_id_t id);
        static bool sai_test_acl_rule_action_range (sai_attr_id_t id);

        static sai_status_t sai_test_acl_table_priority_get (
                                        sai_attribute_t *p_attr_list,
                                        unsigned int attr_count, ...);

        /* SAI API - ACL Table functionality testing. */
        static sai_status_t sai_test_acl_table_create (
                            sai_object_id_t *acl_table_id,
                            unsigned int attr_count, ...);
        static sai_status_t sai_test_acl_table_remove (
                            sai_object_id_t acl_table_id);
        static sai_status_t sai_test_acl_table_set (
                            sai_object_id_t acl_table_id,
                            unsigned int attr_count, ...);
        static sai_status_t sai_test_acl_table_get (
                            sai_object_id_t acl_table_id,
                            unsigned int attr_count, ...);

        /* SAI API - ACL Rule functionality testing. */
        static sai_status_t sai_test_acl_rule_create (
                            sai_object_id_t *acl_rule_id,
                            unsigned int attr_count, ...);
        static sai_status_t sai_test_acl_rule_remove (
                            sai_object_id_t acl_rule_id);
        static sai_status_t sai_test_acl_rule_set (
                            sai_object_id_t acl_rule_id,
                            unsigned int attr_count, ...);
        static sai_status_t sai_test_acl_rule_get (
                            sai_object_id_t acl_rule_id,
                            sai_attribute_t *p_attr_list,
                            unsigned int attr_count, ...);

        /* SAI API - ACL Counter functionality testing. */
        static sai_status_t sai_test_acl_counter_create (
                            sai_object_id_t *acl_counter_id,
                            unsigned int attr_count, ...);
        static sai_status_t sai_test_acl_counter_remove (
                            sai_object_id_t acl_counter_id);
        static sai_status_t sai_test_acl_counter_set (
                            sai_object_id_t acl_counter_id,
                            unsigned int attr_count, ...);
        static sai_status_t sai_test_acl_counter_get (
                            sai_object_id_t acl_counter_id,
                            sai_attribute_t *p_attr_list,
                            unsigned int attr_count, ...);

        /* Util for converting to attribute index based status code */
        static inline sai_status_t sai_test_invalid_attr_status_code (
                                                       sai_status_t status,
                                                       unsigned int attr_index)
        {
            return (status + SAI_STATUS_CODE (attr_index));
        }

        /* SAI API - LAG functionality testing. */
        static sai_status_t sai_test_acl_rule_lag_create (
                            sai_object_id_t *lag_id, sai_attribute_t *attr);
        static sai_status_t sai_test_acl_rule_lag_delete (
                            sai_object_id_t lag_id);

        /* SAI API - Mirror functionality testing. */
        static sai_status_t sai_test_acl_rule_mirror_session_create (
                            sai_object_id_t *p_mirror_session_id,
                            unsigned int attr_count, sai_attribute_t *p_attr_list);
        static sai_status_t sai_test_acl_rule_mirror_session_destroy (
                            sai_object_id_t mirror_session_id);

        /* SAI API - Queue functionality testing. */
        static sai_status_t sai_test_acl_rule_get_max_queues (
                            sai_object_id_t port_id,
                            unsigned int *queue_count);
        static sai_status_t sai_test_acl_rule_get_queue_id_list (
                            sai_object_id_t port_id,
                            unsigned int queue_count,
                            sai_object_id_t *p_queue_id_list);

        static const unsigned int SAI_TEST_MAX_MIRROR_SESSIONS      = 4;

    private:
        static sai_switch_api_t             *p_sai_switch_api_tbl;
        static sai_acl_api_t                *p_sai_acl_api_tbl;
        static sai_lag_api_t                *p_sai_lag_api_tbl;
        static sai_mirror_api_t             *p_sai_mirror_api_tbl;
        static sai_port_api_t               *p_sai_port_api_tbl;
};

#endif  /* __SAI_ACL_UNIT_TEST_H__ */
