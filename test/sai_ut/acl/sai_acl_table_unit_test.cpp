/************************************************************************
 * * LEGALESE:   "Copyright (c) 2015, Dell Inc. All rights reserved."
 * *
 * * This source code is confidential, proprietary, and contains trade
 * * secrets that are the sole property of Dell Inc.
 * * Copy and/or distribution of this source code or disassembly or reverse
 * * engineering of the resultant object code are strictly forbidden without
 * * the written consent of Dell Inc.
 * *
 * ************************************************************************/
 /**
 * * @file sai_acl_table_unit_test.cpp
 * *
 * * @brief This file contains the google unit test cases to test the
 * *        SAI APIs for SAI ACL Table functionality
 * *
 * *************************************************************************/

#include <string.h>
#include <stdio.h>
#include "gtest/gtest.h"

#include "sai_acl_unit_test_utils.h"

extern "C" {
#include "saistatus.h"
#include "saitypes.h"
#include <inttypes.h>
}

class saiACLTableTest : public saiACLTest {
    public:
        static void SetUpTestCase (void);

        static sai_status_t sai_test_acl_table_create_attr_list(
                                            sai_attribute_t **p_attr_list,
                                            unsigned int attr_count);
        static void          sai_test_acl_table_free_attr_list(
                                            sai_attribute_t *p_attr_list);
};

void saiACLTableTest ::SetUpTestCase (void)
{
        /* Base SetUpTestCase for SAI initialization */
        saiACLTest ::SetUpTestCase ();
}

sai_status_t saiACLTableTest ::sai_test_acl_table_create_attr_list(
                                            sai_attribute_t **p_attr_list,
                                            unsigned int attr_count)
{
    sai_attribute_t *attribute_list = NULL;

    if (attr_count == 0) {
        return SAI_STATUS_INVALID_PARAMETER;
    }

    attribute_list = (sai_attribute_t *) calloc (attr_count, sizeof (sai_attribute_t));

    if (attribute_list == NULL) {
        printf ("Failed to allocate memory for attribute list.\r\n");
        return SAI_STATUS_NO_MEMORY;
    }

    *p_attr_list = (sai_attribute_t *)attribute_list;

    return SAI_STATUS_SUCCESS;
}

void saiACLTableTest ::sai_test_acl_table_free_attr_list(
                                            sai_attribute_t *p_attr_list)
{
    free (p_attr_list);
}

TEST_F(saiACLTableTest, get_acl_priority)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t      *p_attr_list_get = NULL;
    unsigned int          test_attr_count = 4;

    sai_rc = sai_test_acl_table_create_attr_list(&p_attr_list_get,
                                                 test_attr_count);

    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    /* Switch Attributes to fetch priority range for ACL Table and Rule */
    sai_rc = sai_test_acl_table_priority_get(p_attr_list_get,
                                test_attr_count,
                                SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY,
                                SAI_SWITCH_ATTR_ACL_TABLE_MAXIMUM_PRIORITY,
                                SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY,
                                SAI_SWITCH_ATTR_ACL_ENTRY_MAXIMUM_PRIORITY);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* NPU Specific attributes */
    printf ("Table Minimun Priority = %d\n", p_attr_list_get[0].value.u32);
    printf ("Table Maximum Priority = %d\n", p_attr_list_get[1].value.u32);
    printf ("Rule entry Minimun Priority = %d\n", p_attr_list_get[2].value.u32);
    printf ("Rule entry Maximum Priority = %d\n", p_attr_list_get[3].value.u32);

    sai_test_acl_table_free_attr_list(p_attr_list_get);
}

TEST_F(saiACLTableTest, table_create_with_null_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_table_id = 0;

    /* Create ACL Table */

    printf ("Table Creation with Null attributes\r\n");
    printf ("Expecting error - SAI_STATUS_FAILURE.\r\n");

    /* Attribute Count is 0 */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 0);
    EXPECT_EQ (SAI_STATUS_INVALID_PARAMETER, sai_rc);
}

TEST_F(saiACLTableTest, table_create_with_out_of_range_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_table_id = 0;

    printf ("Table Creation with Out of Range attributes\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_ATTRIBUTE.\r\n");

    /* ACL Table Attribute not in the range */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 1,
                            SAI_ACL_TABLE_ATTR_FIELD_END + 1);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);
}

TEST_F(saiACLTableTest, table_create_with_unknown_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_table_id = 0;
    unsigned int             invalid_attribute = SAI_ACL_TABLE_ATTR_PRIORITY + 1;

    printf ("Table Creation with Unknown attributes\r\n");
    printf ("Expecting error - SAI_STATUS_UNKNOWN_ATTRIBUTE_0.\r\n");

    /* ACL Table Attribute within the range but not defined */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 1,
                                        invalid_attribute);
    EXPECT_EQ (SAI_STATUS_UNKNOWN_ATTRIBUTE_0, sai_rc);
}

TEST_F(saiACLTableTest, table_create_without_mandatory_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_table_id = 0;

    printf ("Table Creation without Mandatory Attributes\r\n");
    printf ("Expecting error - SAI_MANDATORY_ATTRIBUTE_MISSING.\r\n");

    /* ACL Table Stage, Priority and Field are mandatory attributes */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 1,
                            SAI_ACL_TABLE_ATTR_STAGE, SAI_ACL_STAGE_INGRESS);
    EXPECT_EQ (SAI_MANDATORY_ATTRIBUTE_MISSING, sai_rc);

    sai_rc = sai_test_acl_table_create (&acl_table_id, 2,
                            SAI_ACL_TABLE_ATTR_STAGE, SAI_ACL_STAGE_INGRESS,
                            SAI_ACL_TABLE_ATTR_PRIORITY, 2);
    EXPECT_EQ (SAI_MANDATORY_ATTRIBUTE_MISSING, sai_rc);

    sai_rc = sai_test_acl_table_create (&acl_table_id, 2,
                            SAI_ACL_TABLE_ATTR_STAGE, SAI_ACL_STAGE_INGRESS,
                            SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT);
    EXPECT_EQ (SAI_MANDATORY_ATTRIBUTE_MISSING, sai_rc);
}

TEST_F(saiACLTableTest, table_create_with_invalid_stage_value)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_table_id = 0;

    printf ("Table Creation with Incorrect Table Stage attribute value\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_ATTR_VALUE. \r\n");

    sai_rc = sai_test_acl_table_create (&acl_table_id, 3,
                            SAI_ACL_TABLE_ATTR_STAGE,
                            SAI_ACL_STAGE_EGRESS + 1,
                            SAI_ACL_TABLE_ATTR_PRIORITY, 2,
                            SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTR_VALUE_0, sai_rc);
}

TEST_F(saiACLTableTest, table_create_with_dup_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_table_id = 0;

    printf ("Table Creation with Duplicate Attributes\r\n ");
    printf ("Expecting error - SAI_STATUS_INVALID_ATTRIBUTE.\r\n");

    /* Duplicate Attribute ACL Table Stage */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 4,
                            SAI_ACL_TABLE_ATTR_STAGE,
                            SAI_ACL_STAGE_EGRESS,
                            SAI_ACL_TABLE_ATTR_PRIORITY, 2,
                            SAI_ACL_TABLE_ATTR_STAGE,
                            SAI_ACL_STAGE_INGRESS,
                            SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT);
    EXPECT_EQ (sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTRIBUTE_0, 2), sai_rc);
}

TEST_F(saiACLTableTest, table_create_with_unsupported_field_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_table_id = 0;

    printf ("Table Creation with Unsupported Ingress Fields \r\n");
    printf ("Expecting error - SAI_STATUS_NOT_SUPPORTED.\r\n");

    /* Testing table create with unsupported ingress field. */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 4,
                            SAI_ACL_TABLE_ATTR_STAGE,
                            SAI_ACL_STAGE_INGRESS,
                            SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                            SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                            SAI_ACL_TABLE_ATTR_FIELD_OUT_PORT /* Unsupported */);

    /* Handle the case where all fields are supported in the NPU. */
    if (SAI_STATUS_SUCCESS == sai_rc) {
        sai_rc = sai_test_acl_table_remove (acl_table_id);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    } else {
        EXPECT_EQ (SAI_STATUS_NOT_SUPPORTED, sai_rc);
    }

    printf ("Table Creation with Unsupported Egress Fields \r\n");
    printf ("Expecting error - SAI_STATUS_NOT_SUPPORTED.\r\n");

    /* Testing table create with unsupported egress field. */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 4,
                            SAI_ACL_TABLE_ATTR_STAGE,
                            SAI_ACL_STAGE_EGRESS,
                            SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                            SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                            SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS /* Unsupported */);

    /* Handle the case where all fields are supported in the NPU. */
    if (SAI_STATUS_SUCCESS == sai_rc) {
        sai_rc = sai_test_acl_table_remove (acl_table_id);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    } else {
        EXPECT_EQ (SAI_STATUS_NOT_SUPPORTED, sai_rc);
    }
}

TEST_F(saiACLTableTest, table_create_and_remove)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_table_id = 0;

    printf ("Table Creation with Valid Attributes \r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS.\r\n");

    /* Table Create */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 8,
                             SAI_ACL_TABLE_ATTR_STAGE,
                             SAI_ACL_STAGE_INGRESS,
                             SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                             SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                             SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                             SAI_ACL_TABLE_ATTR_FIELD_DSCP,
                             SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                acl_table_id);
    }

    /* Table Remove */
    sai_rc = sai_test_acl_table_remove (acl_table_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("ACL Table Successfully removed for ID 0x%"PRIx64" \r\n",
                acl_table_id);
    }
}

TEST_F(saiACLTableTest, table_remove_with_invalid_id)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_table_id = 0;

    /* Table Create */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 8,
                             SAI_ACL_TABLE_ATTR_STAGE,
                             SAI_ACL_STAGE_INGRESS,
                             SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                             SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                             SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                             SAI_ACL_TABLE_ATTR_FIELD_DSCP,
                             SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    acl_table_id++;

    printf ("Table Deletion with Invalid Table Id \r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_OBJECT_ID. \r\n");

    sai_rc = sai_test_acl_table_remove (acl_table_id);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    acl_table_id--;
    sai_rc = sai_test_acl_table_remove (acl_table_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLTableTest, table_remove_with_rule_present)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_table_id = 0;
    sai_object_id_t       acl_rule_id = 0;

    printf ("Table Creation with Valid Attributes \r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS.\r\n");

    /* Table Create */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 8,
                             SAI_ACL_TABLE_ATTR_STAGE,
                             SAI_ACL_STAGE_INGRESS,
                             SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                             SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                             SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                             SAI_ACL_TABLE_ATTR_FIELD_DSCP,
                             SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                acl_table_id);
    }

    /* Rule Create */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 4,
                             SAI_ACL_ENTRY_ATTR_TABLE_ID, acl_table_id,
                             SAI_ACL_ENTRY_ATTR_PRIORITY, 5,
                             SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                             SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT,
                             1, 10, 255);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Table Deletion with rule present */
    printf ("Table Deletion with rules attached to Table Id \r\n");
    printf ("Expecting error - SAI_STATUS_FAILURE. \r\n");

    sai_rc = sai_test_acl_table_remove (acl_table_id);

    EXPECT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);

    /* Rule Deletion */
    sai_rc = sai_test_acl_rule_remove (acl_rule_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Table Deletion */
    printf ("Table Deletion with no rules attached to Table Id \r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS. \r\n");

    sai_rc = sai_test_acl_table_remove (acl_table_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLTableTest, table_create_with_same_priority)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_table_id = 0;
    sai_object_id_t       acl_table_id_dup = 0;

    printf ("Table Creation with Valid Attributes \r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS.\r\n");

    /* Table Create */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 8,
                             SAI_ACL_TABLE_ATTR_STAGE,
                             SAI_ACL_STAGE_INGRESS,
                             SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                             SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                             SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                             SAI_ACL_TABLE_ATTR_FIELD_DSCP,
                             SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                acl_table_id);
        printf ("Table Creation with Same Priority & Stage of "
                "Existing ACL Table \r\n");
        printf ("Expecting error - SAI_STATUS_INVALID_ATTR_VALUE. \r\n");

        sai_rc = sai_test_acl_table_create (&acl_table_id_dup, 5,
                            SAI_ACL_TABLE_ATTR_STAGE,
                            SAI_ACL_STAGE_INGRESS,
                            SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                            SAI_ACL_TABLE_ATTR_FIELD_SRC_IPv6,
                            SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,
                            SAI_ACL_TABLE_ATTR_FIELD_DST_IP);
        EXPECT_EQ (sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTR_VALUE_0, 1), sai_rc);

        printf ("Table Creation with Same Priority but with different"
                "stage should be allowed \r\n");
        printf ("Expecting error - SAI_STATUS_SUCCESS.\r\n");

        sai_rc = sai_test_acl_table_create (&acl_table_id_dup, 5,
                            SAI_ACL_TABLE_ATTR_STAGE,
                            SAI_ACL_STAGE_EGRESS,
                            SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                            SAI_ACL_TABLE_ATTR_FIELD_SRC_IPv6,
                            SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,
                            SAI_ACL_TABLE_ATTR_FIELD_DST_IP);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_rc = sai_test_acl_table_remove (acl_table_id_dup);

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    sai_rc = sai_test_acl_table_remove (acl_table_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLTableTest, table_set)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_table_id = 0;

    printf ("Table Creation with Valid Attributes \r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS.\r\n");

    /* Table Create */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 8,
                             SAI_ACL_TABLE_ATTR_STAGE,
                             SAI_ACL_STAGE_INGRESS,
                             SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                             SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                             SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                             SAI_ACL_TABLE_ATTR_FIELD_DSCP,
                             SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                acl_table_id);
    }

    printf ("Table Set for Table Id 0x%"PRIx64" \r\n", acl_table_id);
    printf ("Expecting error - SAI_STATUS_NOT_SUPPORTED.\r\n");

    /* Table Set */
    sai_rc = sai_test_acl_table_set (acl_table_id, 2,
                             SAI_ACL_TABLE_ATTR_PRIORITY, 5 /* New Value */,
                             SAI_ACL_TABLE_ATTR_FIELD_DST_IP /*New Field */);

    EXPECT_EQ (SAI_STATUS_NOT_SUPPORTED, sai_rc);

    sai_rc = sai_test_acl_table_remove (acl_table_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLTableTest, table_get)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_table_id = 0;

    printf ("Table Creation with Valid Attributes \r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS.\r\n");

    /* Table Create */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 8,
                             SAI_ACL_TABLE_ATTR_STAGE,
                             SAI_ACL_STAGE_INGRESS,
                             SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                             SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                             SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                             SAI_ACL_TABLE_ATTR_FIELD_DSCP,
                             SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                acl_table_id);
    }

    printf ("Table Get for Table Id 0x%"PRIx64" \r\n", acl_table_id);
    printf ("Expecting error - SAI_STATUS_SUCCESS.\r\n");

    /* Table Get */
    sai_rc = sai_test_acl_table_get (acl_table_id, 2,
                             SAI_ACL_TABLE_ATTR_STAGE,
                             SAI_ACL_TABLE_ATTR_PRIORITY);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Table Get with invalid Table Id */
    sai_rc = sai_test_acl_table_get ((acl_table_id + 1), 1,
                             SAI_ACL_TABLE_ATTR_STAGE);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Table Get with out of range Table Attribute */
    sai_rc = sai_test_acl_table_get (acl_table_id, 1,
                             (SAI_ACL_TABLE_ATTR_FIELD_END + 1));
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);

    /* Table Get with invalid Table Id */
    sai_rc = sai_test_acl_table_get (acl_table_id, 1,
                             (SAI_ACL_TABLE_ATTR_FIELD_START - 1));
    EXPECT_EQ (SAI_STATUS_UNKNOWN_ATTRIBUTE_0, sai_rc);

    sai_rc = sai_test_acl_table_remove (acl_table_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLTableTest, table_with_invalid_object_type)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_table_id = 0;
    sai_object_id_t          acl_table_invalid_id = 0;

    printf ("Table Creation with Valid Attributes \r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS.\r\n");

    /* Table Create */
    sai_rc = sai_test_acl_table_create (&acl_table_id, 8,
                             SAI_ACL_TABLE_ATTR_STAGE,
                             SAI_ACL_STAGE_INGRESS,
                             SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                             SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                             SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                             SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                             SAI_ACL_TABLE_ATTR_FIELD_DSCP,
                             SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                acl_table_id);
    }

    printf ("Table Get for Invalid Table Id 0x%"PRIx64" \r\n",
             acl_table_invalid_id);
    printf ("Expecting error - SAI_STATUS_INVALID_OBJECT_TYPE.\r\n");

    sai_rc = sai_test_acl_table_remove (acl_table_invalid_id);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, sai_rc);

    /* Table Get with invalid Table Id */
    sai_rc = sai_test_acl_table_get (acl_table_invalid_id, 1,
                             SAI_ACL_TABLE_ATTR_STAGE);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, sai_rc);

    sai_rc = sai_test_acl_table_remove (acl_table_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

