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
*    sai_scheduler_group_unit_test.cpp
*     
* Abstract:
* 
*    SAI SCHEDULER GROUP UNIT TEST :- Covers the test cases for validating all 
*    public APIs in SAI SCHEDULER GROUP module.
*
*************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stdarg.h"
#include "gtest/gtest.h"

extern "C" {
#include "sai_qos_unit_test_utils.h"
#include "sai.h"
#include "saistatus.h"
#include <inttypes.h>
}
#define SAI_MAX_GROUPS_PER_LEVEL 2
#define SAI_MAX_HIERARCHY_LEVELS 10
#define SAI_MAX_QUEUES_PER_PORT  40

#define SAI_MAX_QUEUES_PER_TEST  2

static const unsigned int default_port = 0;
static sai_object_id_t    default_port_id  = 0;
static sai_object_id_t    sg_id_list[SAI_MAX_HIERARCHY_LEVELS][SAI_MAX_GROUPS_PER_LEVEL];
static sai_object_id_t    queue_id_list[SAI_MAX_QUEUES_PER_PORT];
static unsigned int max_queues = 0;
static unsigned int hqos_levels = 0;
static unsigned int leaf_level = 0;

/* SAI initialization */
void SetUpTestCase (void)
{
    sai_switch_notification_t notification;
    memset (&notification, 0, sizeof(sai_switch_notification_t));

    /*
     * Query and populate the SAI Switch API Table.
     */
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_SWITCH, (static_cast<void**>
                                 (static_cast<void*>(&p_sai_switch_api_table)))));

    ASSERT_TRUE (p_sai_switch_api_table != NULL);

    ASSERT_TRUE (p_sai_switch_api_table->initialize_switch != NULL);
    ASSERT_TRUE (p_sai_switch_api_table->shutdown_switch != NULL);
    ASSERT_TRUE (p_sai_switch_api_table->connect_switch != NULL);
    ASSERT_TRUE (p_sai_switch_api_table->disconnect_switch != NULL);
    ASSERT_TRUE (p_sai_switch_api_table->set_switch_attribute != NULL);
    ASSERT_TRUE (p_sai_switch_api_table->get_switch_attribute != NULL);

    /*
     * Switch Initialization.
     * Fill in notification callback routines with stubs.
     */
    notification.on_switch_state_change = sai_switch_operstate_callback;
    notification.on_fdb_event = sai_fdb_evt_callback;
    notification.on_port_state_change = sai_port_state_evt_callback;
    notification.on_switch_shutdown_request = sai_switch_shutdown_callback;
    notification.on_port_event = sai_port_evt_callback;

    ASSERT_TRUE(p_sai_switch_api_table->initialize_switch != NULL);

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               (p_sai_switch_api_table->initialize_switch (0, NULL, NULL,
                                                         &notification)));

    printf("Switch Init success \r\n");
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_api_query(SAI_API_SCHEDULER_GROUP,
                   (static_cast<void**>(static_cast<void*>(&p_sai_qos_sg_api_table)))));

    ASSERT_TRUE (p_sai_qos_sg_api_table != NULL);

    ASSERT_TRUE (p_sai_qos_sg_api_table->create_scheduler_group != NULL);
    ASSERT_TRUE (p_sai_qos_sg_api_table->remove_scheduler_group != NULL);
    ASSERT_TRUE (p_sai_qos_sg_api_table->set_scheduler_group_attribute != NULL);
    ASSERT_TRUE (p_sai_qos_sg_api_table->get_scheduler_group_attribute != NULL);
    ASSERT_TRUE (p_sai_qos_sg_api_table->add_child_object_to_group != NULL);
    ASSERT_TRUE (p_sai_qos_sg_api_table->remove_child_object_from_group != NULL);

    ASSERT_EQ (NULL, sai_api_query(SAI_API_PORT,
              (static_cast<void**>(static_cast<void*>(&p_sai_port_api_table)))));

    ASSERT_TRUE (p_sai_port_api_table != NULL);

    ASSERT_TRUE (p_sai_port_api_table->set_port_attribute != NULL);
    ASSERT_TRUE (p_sai_port_api_table->get_port_attribute != NULL);
    ASSERT_TRUE (p_sai_port_api_table->get_port_stats != NULL);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_api_query(SAI_API_SCHEDULER,
                    (static_cast<void**>(static_cast<void*>(&p_sai_scheduler_api_table)))));

    ASSERT_TRUE (p_sai_scheduler_api_table != NULL);

    ASSERT_TRUE (p_sai_scheduler_api_table->create_scheduler_profile != NULL);
    ASSERT_TRUE (p_sai_scheduler_api_table->remove_scheduler_profile != NULL);
    ASSERT_TRUE (p_sai_scheduler_api_table->set_scheduler_attribute != NULL);
    ASSERT_TRUE (p_sai_scheduler_api_table->get_scheduler_attribute != NULL);
}

static sai_status_t sai_test_sched_group_create (sai_object_id_t *p_group_id,
                                          unsigned int attr_count, ...)
{
    sai_status_t       sai_rc;
    sai_attribute_t   *p_attr_list = NULL;
    sai_attribute_t   *p_attr = NULL;
    va_list            varg_list;
    unsigned int       index;

    p_attr_list = (sai_attribute_t *) calloc (attr_count,
                                              sizeof (sai_attribute_t));
    if (p_attr_list == NULL) {

        printf ("%s(): Memory alloc failed for attribute list.\n", __FUNCTION__);

        return SAI_STATUS_NO_MEMORY;
    }

    va_start (varg_list, attr_count);

    for (index = 0, p_attr = p_attr_list;
         index < attr_count; ++index, ++p_attr) {

        p_attr->id = va_arg (varg_list, unsigned int);

        switch (p_attr->id) {

            case SAI_SCHEDULER_GROUP_ATTR_PORT_ID:
                p_attr->value.oid = va_arg (varg_list, uint64_t);

                printf ("Attr Index: %d, Set Scheduler group port oid: 0x%"PRIx64".\n",
                        index, p_attr->value.oid);
                break;

            case SAI_SCHEDULER_GROUP_ATTR_LEVEL:
                p_attr->value.u32 = va_arg (varg_list, unsigned int);

                printf ("Attr Index: %d, Set Scheduler group level to value: %d.\n",
                        index, p_attr->value.s32);

                break;

            case SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID:
                p_attr->value.oid = va_arg (varg_list, uint64_t);

                printf ("Attr Index: %d, Set Scheduler group scheduler oid: 0x%"PRIx64".\n",
                        index, p_attr->value.oid);
                break;

            default:
                p_attr->value.u64 = va_arg (varg_list, unsigned int);
                printf ("Attr Index: %d, Set unknown Attr Id: %d to value: %ld.\n",
                        index, p_attr->id, p_attr->value.u64);
                break;
        }
    }

    va_end (varg_list);

    sai_rc = p_sai_qos_sg_api_table->create_scheduler_group (p_group_id, attr_count,
                                                          p_attr_list);

    if (sai_rc != SAI_STATUS_SUCCESS) {

        printf ("SAI Scheduler Group Creation failed with error: %d.\n", sai_rc);

    } else {

        printf ("SAI Scheduler Group Creation success, Group Id: 0x%"PRIx64".\n",
                (*p_group_id));
    }

    free (p_attr_list);

    return sai_rc;
}

static sai_status_t sai_test_sched_group_remove (sai_object_id_t sg_id)
{
    sai_status_t sai_rc;

    sai_rc = p_sai_qos_sg_api_table->remove_scheduler_group (sg_id);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("SAI Scheduler Group Remove failed with error: %d.\n", sai_rc);
    } else {
        printf ("SAI Scheduler Group Remove success for Group Id: 0x%"PRIx64".\n",
                sg_id);
    }

    return sai_rc;
}

static void sai_test_sched_group_attr_list_print (unsigned int attr_count,
                                                  sai_attribute_t *p_attr_list)
{
    unsigned int     attr_index;
    sai_attribute_t *p_attr = NULL;
    unsigned int     idx;

    printf ("Printing SAI Scheduler Group attribute list..\n");
    for (attr_index = 0, p_attr = p_attr_list;
         attr_index < attr_count; ++attr_index, ++p_attr) {

        switch (p_attr->id) {

            case SAI_SCHEDULER_GROUP_ATTR_MAX_SUPPORTED_CHILDS:
                printf ("Index: %d, Max supported Group child Count: %d.\n",
                        attr_index, p_attr->value.u32);
                break;

            case SAI_SCHEDULER_GROUP_ATTR_CHILD_COUNT:
                printf ("Index: %d, Active Group child Count: %d.\n",
                        attr_index, p_attr->value.u32);
                break;


            case SAI_SCHEDULER_GROUP_ATTR_CHILD_LIST:
                printf ("Index: %d, Scheduler Group child List count: %d, "
                        "Child list Ids: \n", attr_index,
                        p_attr->value.objlist.count);

                for (idx = 0; idx < p_attr->value.objlist.count; idx++)
                {
                    printf ("0x%"PRIx64" ", p_attr->value.objlist.list[idx]);
                }
                printf ("\n");
                break;

            case SAI_SCHEDULER_GROUP_ATTR_PORT_ID:
                printf ("Index: %d, Scheduler Group port: 0x%"PRIx64".\n", attr_index,
                        p_attr->value.oid);

                break;

            case SAI_SCHEDULER_GROUP_ATTR_LEVEL:
                printf ("Index: %d, Scheduler Group Hierarchy level: %d.\n",
                        attr_index,
                        p_attr->value.u32);
                break;

            case SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID:
                printf ("Index: %d, Scheduler Group scheduler profile: 0x%"PRIx64".\n",
                        attr_index,
                        p_attr->value.oid);
                break;

            default:
                printf ("Index: %d, Scheduler group unknown Attr Id: %d, Value: %ld.\n",
                        attr_index, p_attr->id, p_attr->value.u64);
                break;
        }
    }
}

/* This API is not for SAI_SCHEDULER_GROUP_ATTR_CHILD_LIST */
static sai_status_t sai_test_sched_group_attr_get (sai_object_id_t sg_id,
                                            sai_attribute_t *p_attr_list,
                                            unsigned int attr_count, ...)
{
    sai_status_t         sai_rc;
    va_list              varg_list;
    unsigned int         index;

    va_start (varg_list, attr_count);

    for (index = 0; index < attr_count; index++) {

        p_attr_list [index].id = va_arg (varg_list, unsigned int);
    }

    va_end (varg_list);

    sai_rc = p_sai_qos_sg_api_table->get_scheduler_group_attribute (sg_id,
                                                                    attr_count,
                                                                    p_attr_list);

    if (sai_rc != SAI_STATUS_SUCCESS) {

        printf ("SAI Scheduler Group Get attribute failed with error: %d.\n", sai_rc);

    } else {

        printf ("SAI Scheduler group Get attribute success.\n");

        sai_test_sched_group_attr_list_print (attr_count, p_attr_list);
    }

    return sai_rc;
}

static sai_status_t sai_test_add_child_object_to_group (
                                                  sai_object_id_t  sg_id,
                                                  unsigned int child_count,...)
{
    sai_status_t sai_rc;
    sai_object_id_t    child_list [child_count];
    va_list            varg_list;
    unsigned int       index;

    va_start (varg_list, child_count);

    for (index = 0; index < child_count; ++index) {

        child_list[index] = va_arg (varg_list, uint64_t);

        printf ("SAI Add Child 0x%"PRIx64" to parent SG Id: 0x%"PRIx64".\n",
                child_list[index], sg_id);
    }
    va_end (varg_list);

    sai_rc = p_sai_qos_sg_api_table->add_child_object_to_group (sg_id, child_count,
                                                          &child_list[0]);

    if (sai_rc != SAI_STATUS_SUCCESS) {

        printf ("SAI Add Childs to parent failed with error: %d.\n", sai_rc);

    } else {

        printf ("SAI Add Childs to parent SG success for Parent Id: 0x%"PRIx64".\n",
                sg_id);
    }

    return sai_rc;
}

static sai_status_t sai_test_remove_child_object_from_group (
                                              sai_object_id_t  sg_id,
                                              unsigned int child_count,...)
{
    sai_status_t       sai_rc;
    sai_object_id_t    child_list[child_count];
    va_list            varg_list;
    unsigned int       index;

    va_start (varg_list, child_count);

    for (index = 0; index < child_count; ++index) {

        child_list[index] = va_arg (varg_list, uint64_t);

        printf ("SAI Remove Child 0x%"PRIx64" to parent SG Id: 0x%"PRIx64".\n",
                child_list[index], sg_id);
    }
    va_end (varg_list);

    sai_rc = p_sai_qos_sg_api_table->remove_child_object_from_group (sg_id,
                                                                     child_count,
                                                                     &child_list[0]);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("SAI Remove Childs from parent failed with error: %d.\n", sai_rc);
    } else {
        printf ("SAI Remove Childs from parent success for Parent SG Id: 0x%"PRIx64".\n",
                sg_id);
    }

    return sai_rc;
}

static sai_status_t sai_test_sched_group_child_id_list_get (
                                                     sai_object_id_t  sg_id,
                                                     unsigned int child_count,
                                                     sai_object_id_t *p_child_id_list)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t    attr  = {0};
    unsigned int       attr_count = 1;

    attr.value.objlist.count = child_count;
    attr.value.objlist.list = p_child_id_list;

    if (attr.value.objlist.list == NULL) {

        printf ("%s(): Memory alloc failed for attribute list.\n", __FUNCTION__);

        return SAI_STATUS_NO_MEMORY;
    }

    sai_rc = sai_test_sched_group_attr_get (sg_id, &attr, attr_count,
                                            SAI_SCHEDULER_GROUP_ATTR_CHILD_LIST);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf("Failed to get child list on sg :0x%"PRIx64". \n", sg_id);
        return SAI_STATUS_FAILURE;
    }

    sai_test_sched_group_attr_list_print (attr_count, &attr);
    return sai_rc;
}

static void sai_sched_group_verify_after_removal (sai_object_id_t sg_id)
{
    sai_status_t  sai_rc;

    sai_rc =  sai_test_sched_group_remove (sg_id);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
}

static sai_status_t sai_sched_group_max_hierarchy_level_get(void)
{
    sai_status_t  sai_rc;

    sai_rc = sai_test_switch_max_number_hierarchy_levels_get (&hqos_levels);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (1, (hqos_levels > 0));

    leaf_level = hqos_levels  - 1;
    return sai_rc;
}

static sai_status_t sai_sched_group_create_test_hierarchy (void)
{
    sai_object_id_t  parent_sg_id = SAI_NULL_OBJECT_ID;
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    unsigned int     level = 0;
    unsigned int     group = 0;
    unsigned int     queue = 0;

    printf("Create Scheduler groups at all levels \r\n");
    /* Root/Level 0 */
    sai_rc = sai_test_sched_group_create (&sg_id_list[level][group], 2,
                                          SAI_SCHEDULER_GROUP_ATTR_PORT_ID,
                                          default_port_id,
                                          SAI_SCHEDULER_GROUP_ATTR_LEVEL, level);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create SAI_MAX_GROUPS_PER_LEVEL at each level */
    for (level = 1; level < hqos_levels; level++) {
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {
            sai_rc = sai_test_sched_group_create (&sg_id_list[level][group], 2,
                                                  SAI_SCHEDULER_GROUP_ATTR_PORT_ID,
                                                  default_port_id,
                                                  SAI_SCHEDULER_GROUP_ATTR_LEVEL, level);
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
        }
    }

    /* Add childs SG's at level N+1 to parent level N */
    parent_sg_id = sg_id_list[0][0];

    for (level = 1; level < hqos_levels; level++) {
        sai_rc = sai_test_add_child_object_to_group (parent_sg_id, 2,
                                                      sg_id_list[level][0],
                                                      sg_id_list[level][1]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        parent_sg_id = sg_id_list[level][0];
    }

    /* Add queues to leaf level scheuler group */
    for (queue = 0; queue < SAI_MAX_QUEUES_PER_TEST; queue++) {
        sai_rc = sai_test_add_child_object_to_group (parent_sg_id, 1, queue_id_list[queue]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    return SAI_STATUS_SUCCESS;
}

static sai_status_t sai_sched_group_remove_test_hierarchy (void)
{
    sai_object_id_t  parent_sg_id = SAI_NULL_OBJECT_ID;
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    unsigned int     level = 0;
    unsigned int     group = 0;
    unsigned int     queue = 0;

    /* Remove queues as child */
    printf("Remove Queues from parent SG's at leaf level\r\n");
    parent_sg_id = sg_id_list[leaf_level][0];
    for (queue = 0; queue < SAI_MAX_QUEUES_PER_TEST; queue++) {
        sai_rc = sai_test_remove_child_object_from_group (parent_sg_id, 1,
                                                          queue_id_list[queue]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    printf("Remove Child SG's from Parent SG's\r\n");
    /* Remove SG from leaf to root */
    for (level = leaf_level; level > 0; level--) {
        parent_sg_id = sg_id_list[level - 1][0];
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {
            sai_rc = sai_test_remove_child_object_from_group (parent_sg_id, 1,
                                                              sg_id_list[level][group]);
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
        }
    }

    printf("Remove SG's created at all levels\r\n");
    /* Remove Scheduler groups created */
    sai_rc = sai_test_sched_group_remove(sg_id_list[0][0]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    for (level = 1; level < hqos_levels; level++) {
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {
            sai_rc = sai_test_sched_group_remove(sg_id_list[level][group]);
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
        }
    }
    return sai_rc;
}
/*
 * Validate get port scheduler group id list get.
 */
TEST (saiQosQueueTest, port_sched_group_id_list_get)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    unsigned int max_ports = 0;
    unsigned int max_sg_count = 0;
    unsigned int port_index = 0;
    sai_object_id_t  port_id = SAI_NULL_OBJECT_ID;

    max_ports = sai_qos_max_ports_get();

    for (port_index = 0; port_index < max_ports; port_index++)
    {
        port_id = sai_qos_port_id_get (port_index);

        sai_rc = sai_test_port_sched_group_id_count_get(port_id,
                                                      &max_sg_count);
        ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_object_id_t sg_id_list[max_sg_count];

        sai_rc = sai_test_port_sched_group_id_list_get (port_id, max_sg_count,
                                                        &sg_id_list[0]);
        ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }
}

/*
 * Validate scheduler group create, remove and duplicate delete cases in
 * all hierarchy levels.
 */
TEST (saiQosSchedulerGroupTest, sched_group_create_and_remove)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    unsigned int level = 0;

    for (level = 0; level < hqos_levels; level++) {
        sai_rc = sai_test_sched_group_create (&sg_id_list[level][0], 2,
                                              SAI_SCHEDULER_GROUP_ATTR_PORT_ID,
                                              default_port_id,
                                              SAI_SCHEDULER_GROUP_ATTR_LEVEL, level);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    for (level = 0; level < hqos_levels; level++) {
        sai_rc =  sai_test_sched_group_remove (sg_id_list[level][0]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    for (level = 0; level < hqos_levels; level++) {
        sai_sched_group_verify_after_removal (sg_id_list[level][0]);
    }
}

/*
 * Validate scheduler group create mandatory attribuite test.
 */
TEST (saiQosSchedulerGroupTest, sched_group_create_and_mandatory_attribute)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;

    /* Create Level 1 node */
    sai_rc = sai_test_sched_group_create (&sg_id_list[1][0], 1,
                                          SAI_SCHEDULER_GROUP_ATTR_PORT_ID,
                                          default_port_id);
    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_rc);

    sai_rc = sai_test_sched_group_create (&sg_id_list[1][0], 1,
                                          SAI_SCHEDULER_GROUP_ATTR_LEVEL, 1);
    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_rc);
}

/*
 * Validate add/remove childs to parent SG.
 * - Create Root/Level 0 scheduler group.
 * - Create 2 scheduler group at level 1 ... N-1.
 * - Add all scheduler groups at level 1 to parent scheduler groups at level 0.
 * - Remove child scheduler groups at level 1 from parent scheduler group at level 0.
 */
TEST (saiQosSchedulerGroupTest, add_and_remove_childs_to_group)
{
    sai_object_id_t  parent_sg_id = SAI_NULL_OBJECT_ID;
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    unsigned int     level = 0;
    unsigned int     group = 0;
    unsigned int     queue = 0;

    printf("Create Scheduler groups at all levels \r\n");
    /* Root/Level 0 */
    sai_rc = sai_test_sched_group_create (&sg_id_list[level][group], 2,
                                          SAI_SCHEDULER_GROUP_ATTR_PORT_ID,
                                          default_port_id,
                                          SAI_SCHEDULER_GROUP_ATTR_LEVEL, level);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create SAI_MAX_GROUPS_PER_LEVEL at each level */
    for (level = 1; level < hqos_levels; level++) {
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {
            sai_rc = sai_test_sched_group_create (&sg_id_list[level][group], 2,
                                                  SAI_SCHEDULER_GROUP_ATTR_PORT_ID,
                                                  default_port_id,
                                                  SAI_SCHEDULER_GROUP_ATTR_LEVEL, level);
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
        }
    }

    /* Add childs SG's at level N+1 to parent level N */
    parent_sg_id = sg_id_list[0][0];

    for (level = 1; level < hqos_levels; level++) {
        sai_rc = sai_test_add_child_object_to_group (parent_sg_id, 2,
                                                      sg_id_list[level][0],
                                                      sg_id_list[level][1]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        parent_sg_id = sg_id_list[level][0];
    }

    /* Add queues to leaf level scheuler group */
    for (queue = 0; queue < SAI_MAX_QUEUES_PER_TEST; queue++) {
        sai_rc = sai_test_add_child_object_to_group (parent_sg_id, 1, queue_id_list[queue]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    /* Remove SG from Leval 1 to ROOT Leval 0 */
    if (hqos_levels >= 1) {
        printf("Remove L1 -> L0 \r\n");
        parent_sg_id = sg_id_list[0][0];
        sai_rc = sai_test_remove_child_object_from_group (parent_sg_id, 1, sg_id_list[1][0]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    /* Remove SG from Leval 2 to Leval 1 */
    if (hqos_levels >= 2) {
        printf("Remove L2 -> L1 \r\n");
        parent_sg_id = sg_id_list[1][0];
        sai_rc = sai_test_remove_child_object_from_group (parent_sg_id, 1, sg_id_list[2][0]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    /* Add SG from Leval 1 to ROOT Leval 0 */
    if (hqos_levels >= 1) {
        printf("Add L1 -> L0 \r\n");
        parent_sg_id = sg_id_list[0][0];
        sai_rc = sai_test_add_child_object_to_group (parent_sg_id, 1, sg_id_list[1][0]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    /* Add SG from Leval 2 to Leval 1 */
    if (hqos_levels >= 2) {
        printf("Add L2 -> L1 \r\n");
        parent_sg_id = sg_id_list[1][0];
        sai_rc = sai_test_add_child_object_to_group (parent_sg_id, 1, sg_id_list[2][0]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    /* Remove queues as child */
    printf("Remove Queues from parent SG's at leaf level\r\n");
    parent_sg_id = sg_id_list[leaf_level][0];
    for (queue = 0; queue < SAI_MAX_QUEUES_PER_TEST; queue++) {
        sai_rc = sai_test_remove_child_object_from_group (parent_sg_id, 1, queue_id_list[queue]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    printf("Remove Child SG's from Parent SG's\r\n");
    /* Remove SG from leaf to root */
    for (level = leaf_level; level > 0; level--) {
        parent_sg_id = sg_id_list[level - 1][0];
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {
            sai_rc = sai_test_remove_child_object_from_group (parent_sg_id, 1, sg_id_list[level][group]);
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
        }
    }

    printf("Remove SG's created at all levels\r\n");
    /* Remove Scheduler groups created */
    sai_rc = sai_test_sched_group_remove(sg_id_list[0][0]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    for (level = 1; level < hqos_levels; level++) {
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {
            sai_rc = sai_test_sched_group_remove(sg_id_list[level][group]);
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
        }
    }
}

/*
 * Validate scheduler group get attributes.
 */
TEST (saiQosSchedulerGroupTest, sched_group_attribute_get)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  sg_id;
    unsigned int     attr_count = 4;
    sai_attribute_t  attr_list[attr_count];

    sai_rc = sai_test_sched_group_create (&sg_id, 2,
                                          SAI_SCHEDULER_GROUP_ATTR_PORT_ID,
                                          default_port_id,
                                          SAI_SCHEDULER_GROUP_ATTR_LEVEL, 1);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_sched_group_attr_get (sg_id, &attr_list[0], attr_count,
                                            SAI_SCHEDULER_GROUP_ATTR_CHILD_COUNT,
                                            SAI_SCHEDULER_GROUP_ATTR_PORT_ID,
                                            SAI_SCHEDULER_GROUP_ATTR_LEVEL,
                                            SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID);

    EXPECT_EQ (0, attr_list[0].value.u32);
    EXPECT_EQ (default_port_id, attr_list[1].value.oid);
    EXPECT_EQ (1, attr_list[2].value.u32);
    EXPECT_EQ (0, attr_list[3].value.oid);

    sai_rc =  sai_test_sched_group_remove (sg_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate scheduler group child list get attribute.
 */
TEST (saiQosSchedulerGroupTest, sched_group_child_list_attribute_get)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    unsigned int     queue = 0;
    unsigned int     child = 0;
    unsigned int     level = 0;
    unsigned int     next_level = 0;
    sai_object_id_t  child_list[SAI_MAX_GROUPS_PER_LEVEL];
    sai_object_id_t  queue_list[max_queues];

    sai_rc = sai_sched_group_create_test_hierarchy();
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Get Child List and Validate */
    for (level = 0; level < leaf_level; level++) {

        next_level = level + 1;
        sai_rc = sai_test_sched_group_child_id_list_get (sg_id_list[level][0],
                                                         SAI_MAX_GROUPS_PER_LEVEL,
                                                         &child_list[0]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        for (child = 0; child < SAI_MAX_GROUPS_PER_LEVEL; child++) {
            EXPECT_EQ (sg_id_list[next_level][child], child_list[child]);
        }
    }


    sai_rc = sai_test_sched_group_child_id_list_get (sg_id_list[leaf_level][0],
                                                     max_queues,
                                                     &queue_list[0]);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    for (queue = 0; queue < SAI_MAX_QUEUES_PER_TEST; queue++) {
        EXPECT_EQ (queue_id_list[queue], queue_list[queue]);
    }
    sai_rc = sai_sched_group_remove_test_hierarchy();
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate scheduler group set attributes.
 */
TEST (saiQosSchedulerGroupTest, sched_group_attribute_set)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  sg_id;
    sai_attribute_t  attr;
    sai_object_id_t  child_id;

    sai_rc = sai_test_sched_group_create (&sg_id, 2,
                                          SAI_SCHEDULER_GROUP_ATTR_PORT_ID,
                                          default_port_id,
                                          SAI_SCHEDULER_GROUP_ATTR_LEVEL, 1);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Validate CREATE_ONLY Attributes's */
    attr.id = SAI_SCHEDULER_GROUP_ATTR_PORT_ID;
    attr.value.oid = default_port_id;

    sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id,
                                                                    &attr);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);

    attr.id = SAI_SCHEDULER_GROUP_ATTR_LEVEL;
    attr.value.u32 = 1;
    sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id,
                                                                    &attr);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);

    /* Validate READ ONLY Attribute */
    attr.id = SAI_SCHEDULER_GROUP_ATTR_CHILD_COUNT;
    attr.value.objlist.count = 1;
    attr.value.objlist.list = &child_id;

    sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id,
                                                                    &attr);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);

    sai_rc =  sai_test_sched_group_remove (sg_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate scheduler profile on scheduler group.
 * - Create Hiearchy
 * - Set Scheduler profile
 * - Replace with new profile.
 * - Delete the profile.
 * - Delete Hierarchy.
 */

TEST (saiQosSchedulerGroupTest, sched_group_scheduler_profile_attribute_set)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    unsigned int     level = 0;
    unsigned int     group = 0;
    sai_attribute_t  attr;

    sai_rc = sai_sched_group_create_test_hierarchy();
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;
    /* Create Schduler Profile */
    sai_rc = sai_test_scheduler_create (&sched_id, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_DWRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Set scheduler in SG's for all levels, except ROOT */
    attr.id = SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id;

    for (level = 1; level < hqos_levels; level++) {
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {

            sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id_list[level][group],
                                                                            &attr);
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
            if (sai_rc == SAI_STATUS_SUCCESS)
                printf ("Set Schduler profile id 0x%"PRIx64" for SG  0x%"PRIx64".\n",
                        attr.value.oid, sg_id_list[level][group]);
        }
    }

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);

    sai_object_id_t  sched_id_2 = SAI_NULL_OBJECT_ID;
    /* Create New Schduler Group */
    sai_rc = sai_test_scheduler_create (&sched_id_2, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_DWRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 100,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 4098,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 409800,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 20480);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Set new scheduler in SG's for all levels, except ROOT */
    attr.id = SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id_2;

    for (level = 1; level < hqos_levels; level++) {
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {

            sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id_list[level][group],
                                                                            &attr);
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
            if (sai_rc == SAI_STATUS_SUCCESS)
                printf ("Set Schduler profile id 0x%"PRIx64" for SG  0x%"PRIx64".\n",
                        attr.value.oid, sg_id_list[level][group]);
        }
    }

    sai_rc =  sai_test_scheduler_remove (sched_id_2);
    ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);

    /* Remove Scheduler ID from ALL SG's */
    attr.id = SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;

    for (level = 1; level < hqos_levels; level++) {
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {
            sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id_list[level][group],
                                                                            &attr);
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

            if (sai_rc == SAI_STATUS_SUCCESS)
                printf ("ReSet Schduler profile id 0x%"PRIx64" for SG  0x%"PRIx64".\n",
                        attr.value.oid, sg_id_list[level][group]);
        }
    }

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id_2);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_sched_group_remove_test_hierarchy();
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate Modifying scheduler parmas after applyinig on
 * scheduler group.
 */

TEST (saiQosSchedulerGroupTest, sched_group_modify_scheduler_attributes)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t  attr;
    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;

    sai_rc = sai_test_scheduler_create (&sched_id, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_WRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_sched_group_create_test_hierarchy();
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id;
    /* Apply scheduler on Level 1 SG */
    sai_object_id_t  sg_id = sg_id_list[1][0];
    sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id,
                                                                    &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Modify */
    attr.id = SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM;
    attr.value.s32 = SAI_SCHEDULING_STRICT;

    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT;
    attr.value.u8 = 50;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_SHAPER_TYPE;
    attr.value.s32 = SAI_METER_TYPE_PACKETS;

    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove */
    attr.id = SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;

    sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id,
                                                                    &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_sched_group_remove_test_hierarchy();
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate strict priority scheduler apply on scheduler group.
 */
TEST (saiQosSchedulerGroupTest, scheduler_strict_priority_on_scheduler_group)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;
    sai_attribute_t  attr;

    sai_rc = sai_test_scheduler_create (&sched_id, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_STRICT,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_sched_group_create_test_hierarchy();
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id;

    /* Apply Scheduler to Level 1 node */
    sai_object_id_t  sg_id = sg_id_list[1][0];
    sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Apply Scheduler to Level 2.0 node */
    sg_id = sg_id_list[2][0];
    sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* SP + NO MAX Rate & Burst */
    attr.id = SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE;
    attr.value.u64 = 0;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE;
    attr.value.u64 = 0;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* SP + NO SHAPE */
    attr.id = SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE;
    attr.value.u64 = 0;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE;
    attr.value.u64 = 0;
    sai_rc = p_sai_scheduler_api_table->set_scheduler_attribute (sched_id,
                                                                 &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr.id = SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;

    sg_id = sg_id_list[2][0];
    sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sg_id = sg_id_list[1][0];
    sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_sched_group_remove_test_hierarchy();
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate scheduler group set attributes along with create.
 */
TEST (saiQosSchedulerGroupTest, sched_group_attribute_create_set)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t  sg_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;

    sai_rc = sai_test_scheduler_create (&sched_id, 0);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_sched_group_create (&sg_id, 3,
                                          SAI_SCHEDULER_GROUP_ATTR_PORT_ID,
                                          default_port_id,
                                          SAI_SCHEDULER_GROUP_ATTR_LEVEL, 1,
                                          SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID,
                                          sched_id);
    EXPECT_EQ (sai_test_invalid_attr_status_code(SAI_STATUS_INVALID_ATTRIBUTE_0, 2), sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

/*
 * Validate scheduler group inuse test case.
 */
TEST (saiQosSchedulerGroupTest, sched_group_inuse)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    unsigned int     level = 0;
    unsigned int     group = 0;
    sai_attribute_t  attr;

    sai_rc = sai_sched_group_create_test_hierarchy();
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_object_id_t  sched_id = SAI_NULL_OBJECT_ID;

    for (level = 1; level < hqos_levels; level++) {
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {

            sai_rc =  sai_test_sched_group_remove (sg_id_list[level][group]);
            ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);
        }
    }

    /* Create Schduler Profile */
    sai_rc = sai_test_scheduler_create (&sched_id, 7,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_ALGORITHM, SAI_SCHEDULING_DWRR,
                                        SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT, 10,
                                        SAI_SCHEDULER_ATTR_SHAPER_TYPE, SAI_METER_TYPE_BYTES,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE, 2048,
                                        SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE, 1024,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE, 20480,
                                        SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE, 10240);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Set scheduler in SG's for all levels, except ROOT */
    attr.id = SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = sched_id;

    for (level = 1; level < hqos_levels; level++) {
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {

            sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id_list[level][group],
                                                                            &attr);
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
            if (sai_rc == SAI_STATUS_SUCCESS)
                printf ("Set Schduler profile id 0x%"PRIx64" for SG  0x%"PRIx64".\n",
                        attr.value.oid, sg_id_list[level][group]);
        }
    }

    for (level = 1; level < hqos_levels; level++) {
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {
            sai_rc =  sai_test_sched_group_remove (sg_id_list[level][group]);
            ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);
        }
    }

    /* Remove Scheduler ID from ALL SG's */
    attr.id = SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID;
    attr.value.oid = SAI_NULL_OBJECT_ID;

    for (level = 1; level < hqos_levels; level++) {
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {
            sai_rc = p_sai_qos_sg_api_table->set_scheduler_group_attribute (sg_id_list[level][group],
                                                                            &attr);
            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

            if (sai_rc == SAI_STATUS_SUCCESS)
                printf ("ReSet Schduler profile id 0x%"PRIx64" for SG  0x%"PRIx64".\n",
                        attr.value.oid, sg_id_list[level][group]);
        }
    }

    for (level = 1; level < hqos_levels; level++) {
        for (group = 0; group < SAI_MAX_GROUPS_PER_LEVEL; group++) {
            sai_rc =  sai_test_sched_group_remove (sg_id_list[level][group]);
            ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);
        }
    }

    sai_rc = sai_sched_group_remove_test_hierarchy();
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc =  sai_test_scheduler_remove (sched_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    SetUpTestCase ();
    default_port_id = sai_qos_port_id_get (default_port);
    sai_test_port_queue_count_and_id_list_get(default_port_id, &max_queues,
                                              &queue_id_list[0]);
    sai_sched_group_max_hierarchy_level_get();
    return RUN_ALL_TESTS();
}

