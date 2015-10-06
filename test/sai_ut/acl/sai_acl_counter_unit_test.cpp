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
 * * @file sai_acl_counter_unit_test.cpp
 * *
 * * @brief This file contains the google unit test cases to test the
 * *        SAI APIs for SAI ACL Counter functionality
 * *
 * *************************************************************************/

#include "gtest/gtest.h"

#include "sai_acl_unit_test_utils.h"

extern "C" {
#include "saistatus.h"
#include "saitypes.h"
#include <inttypes.h>
}

class saiACLCounterTest : public saiACLTest
{
    public:
        static void SetUpTestCase (void);
        static void TearDownTestCase (void);

        enum sai_acl_table_type_t {SAI_ACL_TABLE_TYPE_MAC, SAI_ACL_TABLE_TYPE_IP,
                                   SAI_ACL_TABLE_TYPE_IPv6, SAI_ACL_TABLE_TYPE_IP_MAC};

        static sai_object_id_t  sai_test_acl_rule_entry_create_with_counter (
                                                        sai_acl_table_type_t table_type,
                                                        sai_object_id_t acl_counter_id);
        static void                sai_test_acl_rule_entry_with_counter_remove (
                                                        sai_object_id_t acl_rule_id);
        static sai_status_t        sai_test_acl_counter_create_attr_list (
                                                            sai_attribute_t **p_attr_list,
                                                            unsigned int attr_count);
        static void                sai_test_acl_counter_free_attr_list (
                                                            sai_attribute_t *p_attr_list);

        static const sai_mac_t src_mac_data;
        static const sai_mac_t src_mac_mask;
        static const sai_mac_t dst_mac_data;
        static const sai_mac_t dst_mac_mask;

        static const sai_ip4_t src_ip_data;
        static const sai_ip4_t src_ip_mask;
        static const sai_ip4_t dst_ip_data;
        static const sai_ip4_t dst_ip_mask;

        static const sai_ip6_t src_ipv6_data[];
        static const sai_ip6_t src_ipv6_mask[];
        static const sai_ip6_t dst_ipv6_data[];
        static const sai_ip6_t dst_ipv6_mask[];

        static const uint64_t standard_mask;

        static sai_object_id_t mac_table_id;
        static sai_object_id_t mac_egress_table_id;
        static sai_object_id_t ip_table_id;
        static sai_object_id_t ip_egress_table_id;
        static sai_object_id_t ipv6_table_id;
        static sai_object_id_t ipv6_egress_table_id;
        static sai_object_id_t ip_and_mac_table_id;
        static sai_object_id_t port_id_1;
};

enum sai_acl_table_type_ {
    SAI_ACL_TABLE_TYPE_MAC,
    SAI_ACL_TABLE_TYPE_IP,
    SAI_ACL_TABLE_TYPE_IPv6,
    SAI_ACL_TABLE_TYPE_IP_MAC
} sai_acl_table_type_t;


const sai_mac_t         saiACLCounterTest ::src_mac_data = {0x01,0x02,0x03,0x04,0x05,0x06};
const sai_mac_t         saiACLCounterTest ::src_mac_mask = {0xff,0xff,0xff,0xff,0xff,0xff};
const sai_mac_t         saiACLCounterTest ::dst_mac_data = {0x06,0x05,0x04,0x03,0x02,0x01};
const sai_mac_t         saiACLCounterTest ::dst_mac_mask = {0xff,0xff,0xff,0xff,0xff,0xff};

const sai_ip4_t         saiACLCounterTest ::src_ip_data = 0x01020304;
const sai_ip4_t         saiACLCounterTest ::src_ip_mask = 0xffffffff;
const sai_ip4_t         saiACLCounterTest ::dst_ip_data = 0x04030201;
const sai_ip4_t         saiACLCounterTest ::dst_ip_mask = 0xffffffff;

const sai_ip6_t         saiACLCounterTest ::src_ipv6_data[] =
                                            {0x00,0x01,0x00,0x02,0x00,0x03,0x00,0x04,
                                             0x00,0x05,0x00,0x06,0x00,0x07,0x00,0x08};
const sai_ip6_t         saiACLCounterTest ::src_ipv6_mask[] =
                                            {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
                                             0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
const sai_ip6_t         saiACLCounterTest ::dst_ipv6_data[] =
                                            {0x00,0x08,0x00,0x07,0x00,0x06,0x00,0x05,
                                             0x00,0x04,0x00,0x03,0x00,0x02,0x00,0x01};
const sai_ip6_t         saiACLCounterTest ::dst_ipv6_mask[] =
                                            {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
                                             0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

const uint64_t        saiACLCounterTest ::standard_mask = 0xffffffffffffffff;

sai_object_id_t  saiACLCounterTest ::mac_table_id = 0;
sai_object_id_t  saiACLCounterTest ::mac_egress_table_id = 0;
sai_object_id_t  saiACLCounterTest ::ip_table_id = 0;
sai_object_id_t  saiACLCounterTest ::ip_egress_table_id = 0;
sai_object_id_t  saiACLCounterTest ::ipv6_table_id = 0;
sai_object_id_t  saiACLCounterTest ::ipv6_egress_table_id = 0;
sai_object_id_t  saiACLCounterTest ::ip_and_mac_table_id = 0;
sai_object_id_t  saiACLCounterTest ::port_id_1= 0;

void saiACLCounterTest ::SetUpTestCase (void)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;

    /* Base SetUpTestCase for SAI initialization */
    saiACLTest ::SetUpTestCase ();

    /* Create MAC Based Ingress ACL Table */
    sai_rc = sai_test_acl_table_create (&mac_table_id, 14,
                                        SAI_ACL_TABLE_ATTR_STAGE,
                                        SAI_ACL_STAGE_INGRESS,
                                        SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                                        SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                                        SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,
                                        SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_TYPE,
                                        SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_ID,
                                        SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_PRI,
                                        SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_CFI,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL,
                                        SAI_ACL_TABLE_ATTR_FIELD_IN_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_ECN,
                                        SAI_ACL_TABLE_ATTR_FIELD_IPv6_FLOW_LABEL,
                                        SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("Ingress MAC ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                 mac_table_id);
    }

    /* Create MAC Based Egress ACL Table */
    sai_rc = sai_test_acl_table_create (&mac_egress_table_id, 11,
                                        SAI_ACL_TABLE_ATTR_STAGE,
                                        SAI_ACL_STAGE_EGRESS,
                                        SAI_ACL_TABLE_ATTR_PRIORITY, 1,
                                        SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                                        SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,
                                        SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_TYPE,
                                        SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_ID,
                                        SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_PRI,
                                        SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_CFI,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL,
                                        SAI_ACL_TABLE_ATTR_FIELD_OUT_PORT);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("Egress MAC ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                 mac_egress_table_id);
    }

    /* Create IP Based Ingress ACL Table */
    sai_rc = sai_test_acl_table_create (&ip_table_id, 15,
                                        SAI_ACL_TABLE_ATTR_STAGE,
                                        SAI_ACL_STAGE_INGRESS,
                                        SAI_ACL_TABLE_ATTR_PRIORITY, 2,
                                        SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                                        SAI_ACL_TABLE_ATTR_FIELD_DST_IP,
                                        SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_TTL,
                                        SAI_ACL_TABLE_ATTR_FIELD_TCP_FLAGS,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL,
                                        SAI_ACL_TABLE_ATTR_FIELD_TOS,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_FRAG,
                                        SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                                        SAI_ACL_TABLE_ATTR_FIELD_IN_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS,
                                        SAI_ACL_TABLE_ATTR_FIELD_IPv6_FLOW_LABEL);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("Ingress IP ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                 ip_table_id);
    }

    /* Create IP Based Egress ACL Table */
    sai_rc = sai_test_acl_table_create (&ip_egress_table_id, 12,
                                        SAI_ACL_TABLE_ATTR_STAGE,
                                        SAI_ACL_STAGE_EGRESS,
                                        SAI_ACL_TABLE_ATTR_PRIORITY, 2,
                                        SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                                        SAI_ACL_TABLE_ATTR_FIELD_DST_IP,
                                        SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_TTL,
                                        SAI_ACL_TABLE_ATTR_FIELD_TCP_FLAGS,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL,
                                        SAI_ACL_TABLE_ATTR_FIELD_TOS,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_FRAG,
                                        SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                                        SAI_ACL_TABLE_ATTR_FIELD_OUT_PORT);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("Egress IP ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                 ip_egress_table_id);
    }

    /* Create IPv6 Based Ingress ACL Table */
    sai_rc = sai_test_acl_table_create (&ipv6_table_id, 15,
                                        SAI_ACL_TABLE_ATTR_STAGE,
                                        SAI_ACL_STAGE_INGRESS,
                                        SAI_ACL_TABLE_ATTR_PRIORITY, 3,
                                        SAI_ACL_TABLE_ATTR_FIELD_SRC_IPv6,
                                        SAI_ACL_TABLE_ATTR_FIELD_DST_IPv6,
                                        SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_TTL,
                                        SAI_ACL_TABLE_ATTR_FIELD_TCP_FLAGS,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL,
                                        SAI_ACL_TABLE_ATTR_FIELD_TOS,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_FRAG,
                                        SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                                        SAI_ACL_TABLE_ATTR_FIELD_IN_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS,
                                        SAI_ACL_TABLE_ATTR_FIELD_IPv6_FLOW_LABEL);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("Ingress IPv6 ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                 ipv6_table_id);
    }

    /* Create IPv6 Based Egress ACL Table */
    sai_rc = sai_test_acl_table_create (&ipv6_egress_table_id, 12,
                                        SAI_ACL_TABLE_ATTR_STAGE,
                                        SAI_ACL_STAGE_EGRESS,
                                        SAI_ACL_TABLE_ATTR_PRIORITY, 3,
                                        SAI_ACL_TABLE_ATTR_FIELD_SRC_IPv6,
                                        SAI_ACL_TABLE_ATTR_FIELD_DST_IPv6,
                                        SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_TTL,
                                        SAI_ACL_TABLE_ATTR_FIELD_TCP_FLAGS,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL,
                                        SAI_ACL_TABLE_ATTR_FIELD_TOS,
                                        SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                                        SAI_ACL_TABLE_ATTR_FIELD_OUT_PORT);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("Egress IPv6 ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                 ipv6_egress_table_id);
    }

    /* Create IP + MAC Based ACL Table */
    sai_rc = sai_test_acl_table_create (&ip_and_mac_table_id, 20,
                                        SAI_ACL_TABLE_ATTR_STAGE,
                                        SAI_ACL_STAGE_INGRESS,
                                        SAI_ACL_TABLE_ATTR_PRIORITY, 4,
                                        SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                                        SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,
                                        SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                                        SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_ID,
                                        SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_PRI,
                                        SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_ID,
                                        SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_PRI,
                                        SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_CFI,
                                        SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                                        SAI_ACL_TABLE_ATTR_FIELD_DST_IP,
                                        SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_TTL,
                                        SAI_ACL_TABLE_ATTR_FIELD_TCP_FLAGS,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL,
                                        SAI_ACL_TABLE_ATTR_FIELD_TOS,
                                        SAI_ACL_TABLE_ATTR_FIELD_IN_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("IP + MAC ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                 ip_and_mac_table_id);
    }

    port_id_1 = sai_acl_port_id_get (0);
}

void saiACLCounterTest ::TearDownTestCase (void)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;

    /* Remove all the ACL Tables created on the test case SetUp */
    sai_rc = sai_test_acl_table_remove (mac_table_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_table_remove (mac_egress_table_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_table_remove (ip_table_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_table_remove (ip_egress_table_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_table_remove (ipv6_table_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_table_remove (ipv6_egress_table_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_table_remove (ip_and_mac_table_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

sai_object_id_t saiACLCounterTest ::sai_test_acl_rule_entry_create_with_counter (
                                                    sai_acl_table_type_t table_type,
                                                    sai_object_id_t acl_counter_id)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_rule_id = 0;

    if (acl_counter_id) {
        switch (table_type) {
            case SAI_ACL_TABLE_TYPE_MAC:
                /* Create MAC Type ACL Rule */
                sai_rc = sai_test_acl_rule_create (&acl_rule_id, 11,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 15,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, &src_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_MAC,
                                       1, &dst_mac_data, &dst_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE,
                                       1, 34825, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IP_TYPE,
                                       1, (unsigned long) SAI_ACL_IP_TYPE_ANY,
                                       standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_ID,
                                       1, 10, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS,
                                       1, 1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP,
                                       SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                       1, acl_counter_id);

                EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
                break;
            case SAI_ACL_TABLE_TYPE_IP:
                /* Create MAC Type ACL Rule */
                sai_rc = sai_test_acl_rule_create (&acl_rule_id, 15,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 20,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP,
                                       1, &src_ip_data, &src_ip_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_IP,
                                       1, &dst_ip_data, &dst_ip_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE,
                                       1, 34825, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT,
                                       1, 25, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT,
                                       1, 45, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_TCP_FLAGS,
                                       1, 2 /* SYN */, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL,
                                       1, 6 /* TCP */, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_TOS,
                                       1, 32 , standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IP_FRAG,
                                       1, (unsigned long) SAI_ACL_IP_FRAG_NON_FRAG,
                                       standard_mask,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP,
                                       SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_MAC,
                                       true, src_mac_data,
                                       SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                       1, acl_counter_id);

                EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

                break;
            case SAI_ACL_TABLE_TYPE_IPv6:
                /* Create MAC Type ACL Rule */
                sai_rc = sai_test_acl_rule_create (&acl_rule_id, 16,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ipv6_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 25,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_IPv6,
                                       1, &src_ipv6_data, &src_ipv6_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_IPv6,
                                       1, &dst_ipv6_data, &dst_ipv6_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE,
                                       1, 34825, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT,
                                       1, 20, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT,
                                       1, 40, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_TCP_FLAGS,
                                       1, 2 /* SYN */, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL,
                                       1, 6 /* TCP */, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_TOS,
                                       1, 32 , standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IP_FRAG,
                                       1, (unsigned long) SAI_ACL_IP_FRAG_NON_FRAG,
                                       standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS,
                                       1, 1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP,
                                       SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_MAC,
                                       true, dst_mac_data,
                                       SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                       1, acl_counter_id);

                EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

                break;
            case SAI_ACL_TABLE_TYPE_IP_MAC:
                /* Create MAC Type ACL Rule */
                sai_rc = sai_test_acl_rule_create (&acl_rule_id, 12,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_and_mac_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 35,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, &src_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_MAC,
                                       1, &dst_mac_data, &dst_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP,
                                       1, &src_ip_data, &src_ip_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_IP,
                                       1, &dst_ip_data, &dst_ip_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE,
                                       1, 34825, 65535,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT,
                                       1, 5, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT,
                                       1, 6, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP,
                                       SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                       1, acl_counter_id);

                EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

                break;
            default:
                printf ("Unknown ACL Table Type passed \r\n");
                break;
        }
    } else {
        switch (table_type) {
        case SAI_ACL_TABLE_TYPE_MAC:
            /* Create MAC Type ACL Rule */
            sai_rc = sai_test_acl_rule_create (&acl_rule_id, 10,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 15,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, &src_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_MAC,
                                       1, &dst_mac_data, &dst_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE,
                                       1, 34825, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IP_TYPE,
                                       1, (unsigned long) SAI_ACL_IP_TYPE_ANY,
                                       standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_ID,
                                       1, 10, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS,
                                       1, 1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP);

            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
            break;
        case SAI_ACL_TABLE_TYPE_IP:
            /* Create MAC Type ACL Rule */
            sai_rc = sai_test_acl_rule_create (&acl_rule_id, 14,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 20,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP,
                                       1, &src_ip_data, &src_ip_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_IP,
                                       1, &dst_ip_data, &dst_ip_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE,
                                       1, 34825, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT,
                                       1, 25, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT,
                                       1, 45, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_TCP_FLAGS,
                                       1, 2 /* SYN */, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL,
                                       1, 6 /* TCP */, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_TOS,
                                       1, 32 , standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IP_FRAG,
                                       1, (unsigned long) SAI_ACL_IP_FRAG_NON_FRAG,
                                       standard_mask,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP,
                                       SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_MAC,
                                       true, src_mac_data);

            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

            break;
        case SAI_ACL_TABLE_TYPE_IPv6:
            /* Create MAC Type ACL Rule */
            sai_rc = sai_test_acl_rule_create (&acl_rule_id, 15,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ipv6_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 25,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_IPv6,
                                       1, &src_ipv6_data, &src_ipv6_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_IPv6,
                                       1, &dst_ipv6_data, &dst_ipv6_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE,
                                       1, 34825, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT,
                                       1, 20, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT,
                                       1, 40, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_TCP_FLAGS,
                                       1, 2 /* SYN */, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL,
                                       1, 6 /* TCP */, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_TOS,
                                       1, 32 , standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IP_FRAG,
                                       1, (unsigned long) SAI_ACL_IP_FRAG_NON_FRAG,
                                       standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS,
                                       1, 1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP,
                                       SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_MAC,
                                       true, dst_mac_data);

            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

            break;
        case SAI_ACL_TABLE_TYPE_IP_MAC:
            /* Create MAC Type ACL Rule */
            sai_rc = sai_test_acl_rule_create (&acl_rule_id, 11,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_and_mac_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 35,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, &src_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_MAC,
                                       1, &dst_mac_data, &dst_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP,
                                       1, &src_ip_data, &src_ip_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_IP,
                                       1, &dst_ip_data, &dst_ip_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE,
                                       1, 34825, 65535,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT,
                                       1, 5, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT,
                                       1, 6, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP);

            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

            break;
        default:
            printf ("Unknown ACL Table Type passed \r\n");
            break;
        }
    }

    return acl_rule_id;
}

void saiACLCounterTest ::sai_test_acl_rule_entry_with_counter_remove(
                                                sai_object_id_t acl_rule_id)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;

    /* Delete the ACL Rule */
    sai_rc = sai_test_acl_rule_remove (acl_rule_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

sai_status_t saiACLCounterTest ::sai_test_acl_counter_create_attr_list(
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

void saiACLCounterTest ::sai_test_acl_counter_free_attr_list(
                                            sai_attribute_t *p_attr_list)
{
    free (p_attr_list);
    p_attr_list = NULL;
}

TEST_F(saiACLCounterTest, counter_create_with_null_attr)
{
    sai_status_t        sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t     acl_counter_id = 0;

    /* Create ACL Counter with Null attr count */

    printf ("Counter Creation with Null attributes\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_PARAMETER.\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 0);
    EXPECT_EQ (SAI_STATUS_INVALID_PARAMETER, sai_rc);
}

TEST_F(saiACLCounterTest, counter_create_with_out_of_range_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;
    unsigned int             out_of_range_attr_id = SAI_ACL_COUNTER_ATTR_BYTES + 1;

    printf ("Counter Creation with Out of Range attributes\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_ATTRIBUTE.\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 1,
                                          out_of_range_attr_id, true);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);
}

TEST_F(saiACLCounterTest, counter_create_with_invalid_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;
    unsigned int             invalid_attr_id_byte = SAI_ACL_COUNTER_ATTR_BYTES;
    unsigned int             invalid_attr_id_packet = SAI_ACL_COUNTER_ATTR_PACKETS;

    printf ("Counter Creation with Invalid attributes\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_ATTRIBUTE.\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 1,
                                          invalid_attr_id_byte, 100);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 1,
                                          invalid_attr_id_packet, 100);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          invalid_attr_id_byte, 100);
    EXPECT_EQ (sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTRIBUTE_0, 1), sai_rc);

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 3,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true,
                                          invalid_attr_id_byte, 100);
    EXPECT_EQ (sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTRIBUTE_0, 2), sai_rc);

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 3,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          invalid_attr_id_byte, 100,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTRIBUTE_0, 1), sai_rc);
}

TEST_F(saiACLCounterTest, counter_create_with_dup_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;

    /* Create ACL Counter with Null attr count */

    printf ("Counter Creation with Duplicate attributes\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_ATTRIBUTE.\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 3,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id);
    EXPECT_EQ (sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTRIBUTE_0, 2), sai_rc);

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 4,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTRIBUTE_0, 3), sai_rc);
}

TEST_F(saiACLCounterTest, counter_create_with_default)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;

    /* Create ACL Counter with Null attr count */

    printf ("Counter Creation with Default attributes\r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS.\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 1,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Type Created */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 1);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       1, SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT /* Default */);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    /* Clean up the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Next case where default counter will be created */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, false,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Type Created */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 1);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       1, SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT /* Default */);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    /* Clean up the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /*
     * Test Case 1:
     * SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT = false
     * Behavior :
     * Returns SAI_STATUS_INVALID_ATTR_VALUE
     *
     * Test Case 2:
     * SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT = false &&
     * SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT = false
     * Behavior :
     * Returns SAI_STATUS_INVALID_ATTR_VALUE
     */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, false);
    EXPECT_EQ (sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTR_VALUE_0, 1), sai_rc);

    /* Next case where default counter will be created */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 3,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, false,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, false);
    EXPECT_EQ (sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTR_VALUE_0, 1), sai_rc);

    /* Next case where default counter will be created */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 3,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, false);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Type Created */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 1);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       1, SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    /* Clean up the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Next case where default counter will be created */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 3,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, false,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Type Created */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 1);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       1, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    /* Clean up the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Next case where default counter will be created */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 3,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Type Created */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 2);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       2, SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[1].value.booldata, true);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    /* Clean up the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

}

TEST_F(saiACLCounterTest, counter_create_without_mandatory_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;

    /* Create ACL Counter without Mandatory attr */

    printf ("Counter Creation without Mandatory attributes\r\n");
    printf ("Expecting error - SAI_MANDATORY_ATTRIBUTE_MISSING.\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 1,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_MANDATORY_ATTRIBUTE_MISSING, sai_rc);

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true);
    EXPECT_EQ (SAI_MANDATORY_ATTRIBUTE_MISSING, sai_rc);
}

TEST_F(saiACLCounterTest, counter_create_with_invalid_table_id)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;

    /* Create ACL Counter with Invalid ACL Table Id attr */

    printf ("Counter Creation with Invalid Table ID attribute\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_OBJECT_ID.\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, (ip_and_mac_table_id + 1),
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, (ip_and_mac_table_id + 1),
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);
}

TEST_F(saiACLCounterTest, counter_create_with_no_rule_in_table)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;

    printf ("Counter Creation with no rules in table\r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS.\r\n");

    /* Packet Type Counter Creation */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Type Created */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 3);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       3, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);   /* Packet Type */
    EXPECT_EQ (p_attr_list_get[1].value.booldata, false);  /* Byte Type */
    EXPECT_EQ (p_attr_list_get[2].value.oid, mac_table_id);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    /* Clean up the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Byte Type Counter Creation */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Type Created */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 3);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       3, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, false);
    EXPECT_EQ (p_attr_list_get[1].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[2].value.oid, ip_table_id);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    /* Clean up the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Both Packet and Byte Type Counter Creation */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 3,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ipv6_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Type Created */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 3);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       3, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[1].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[2].value.oid, ipv6_table_id);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    /* Clean up the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLCounterTest, counter_create_with_rules_in_table)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_counter_id = 0;
    sai_object_id_t       ip_rule_id = 0;
    sai_object_id_t       mac_rule_id = 0;
    sai_object_id_t       ipv6_rule_id = 0;
    sai_attribute_t      *p_attr_list_get = NULL;

    printf ("Counter Creation with  rules in table\r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS.\r\n");

    /* ACL Rule Creation */
    mac_rule_id = sai_test_acl_rule_entry_create_with_counter (SAI_ACL_TABLE_TYPE_MAC, 0);

    ASSERT_NE(0, mac_rule_id);

    ip_rule_id = sai_test_acl_rule_entry_create_with_counter (SAI_ACL_TABLE_TYPE_IP, 0);

    ASSERT_NE(0, ip_rule_id);

    ipv6_rule_id = sai_test_acl_rule_entry_create_with_counter (SAI_ACL_TABLE_TYPE_IPv6, 0);

    ASSERT_NE(0, ipv6_rule_id);

    /* Packet Type Counter Creation */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Type Created */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 3);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       3, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);   /* Packet Type */
    EXPECT_EQ (p_attr_list_get[1].value.booldata, false);  /* Byte Type */
    EXPECT_EQ (p_attr_list_get[2].value.oid, mac_table_id);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    /* Clean up the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Byte Type Counter Creation */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Type Created */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 3);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       3, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, false);
    EXPECT_EQ (p_attr_list_get[1].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[2].value.oid, ip_table_id);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    /* Clean up the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Both Packet and Byte Type Counter Creation */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 3,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ipv6_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Type Created */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 3);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       3, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[1].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[2].value.oid, ipv6_table_id);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    /* Clean up the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_acl_rule_entry_with_counter_remove (mac_rule_id);
    sai_test_acl_rule_entry_with_counter_remove (ip_rule_id);
    sai_test_acl_rule_entry_with_counter_remove (ipv6_rule_id);
}

TEST_F(saiACLCounterTest, counter_remove_with_invalid_counter_id)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;

    /* Create ACL Counter without Mandatory attr */

    printf ("Counter Deletion with Invalid Counter ID\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_OBJECT_ID\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id + 1);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLCounterTest, counter_remove_with_rule_attached)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_counter_id = 0;
    sai_object_id_t       ip_rule_id = 0;

    /* Create ACL Counter without Mandatory attr */

    printf ("Counter Deletion with Rules Attached \r\n");
    printf ("Expecting error - SAI_STATUS_FAILURE\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create Rules and attach them to the Counter */
    ip_rule_id = sai_test_acl_rule_entry_create_with_counter (
                                          SAI_ACL_TABLE_TYPE_IP, acl_counter_id);
    ASSERT_NE(0, ip_rule_id);

    /* Try deleting the counter with rule attached to it */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_OBJECT_IN_USE, sai_rc);

    /* Delete the ACL Rule */
    sai_test_acl_rule_entry_with_counter_remove (ip_rule_id);

    /* Counter should be able to be deleted */
    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLCounterTest, counter_set_with_invalid_counter_id)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;

    /* Create ACL Counter without Mandatory attr */

    printf ("Counter Set with Invalid Counter ID\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_OBJECT_ID\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_set ((acl_counter_id + 1), 1,
                                       SAI_ACL_COUNTER_ATTR_BYTES, 100);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLCounterTest, counter_set_with_out_of_range_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;
    unsigned int             out_of_range_attr_id = SAI_ACL_COUNTER_ATTR_BYTES + 1;

    /* Create ACL Counter without Mandatory attr */

    printf ("Counter Set with Out of Range Counter Attribute \r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_ATTRIBUTE\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       out_of_range_attr_id, 100);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLCounterTest, counter_set_with_mismatch_counter_type)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;

    printf ("Counter Set with Mismatch Counter Type\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_ATTRIBUTE\r\n");

    /* Create Packet type Counter */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Try setting byte count in packet type counter */
    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_BYTES, 100);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create Byte type Counter */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Try setting packet count in byte type counter */
    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_PACKETS, 100);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLCounterTest, counter_set_without_mandatory_attr)
{
    sai_status_t            sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t         acl_counter_id = 0;

    /* Create ACL Counter without Mandatory attr */

    printf ("Counter Set without Mandatory Attr\r\n");
    printf ("Expecting error - SAI_MANDATORY_ATTRIBUTE_MISSING\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, 10);
    EXPECT_EQ (SAI_MANDATORY_ATTRIBUTE_MISSING, sai_rc);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLCounterTest, counter_set_valid)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    sai_object_id_t          mac_rule_id = 0;

    /* Create ACL Counter without Mandatory attr */

    printf ("Counter Set with valid attr\r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS\r\n");

    /* Create Default Counter Type (Byte) */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 1,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Broadcom behavior, where set is not supported if no rules are attached */
    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_BYTES, 100000);
    EXPECT_EQ (SAI_STATUS_NOT_SUPPORTED, sai_rc);

    /* Create Rules and attach them to the Counter */
    mac_rule_id = sai_test_acl_rule_entry_create_with_counter (
                                          SAI_ACL_TABLE_TYPE_MAC, acl_counter_id);
    ASSERT_NE(0, mac_rule_id);

    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_BYTES, 100000);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Count Set */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 4);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       4, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID,
                                       SAI_ACL_COUNTER_ATTR_BYTES);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, false);
    EXPECT_EQ (p_attr_list_get[1].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[2].value.oid, mac_table_id);
    EXPECT_EQ (p_attr_list_get[3].value.u64, 100000);

    sai_test_acl_rule_entry_with_counter_remove (mac_rule_id);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Same Test for Packet Type Counter */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create Rules and attach them to the Counter */
    mac_rule_id = sai_test_acl_rule_entry_create_with_counter (
                                          SAI_ACL_TABLE_TYPE_MAC, acl_counter_id);
    ASSERT_NE(0, mac_rule_id);

    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_PACKETS, 100000);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Count Set */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 4);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       4, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID,
                                       SAI_ACL_COUNTER_ATTR_PACKETS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[1].value.booldata, false);
    EXPECT_EQ (p_attr_list_get[2].value.oid, mac_table_id);
    EXPECT_EQ (p_attr_list_get[3].value.u64, 100000);

    sai_test_acl_rule_entry_with_counter_remove (mac_rule_id);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Same Test for both Packet and Byte Type Counter */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 3,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create Rules and attach them to the Counter */
    mac_rule_id = sai_test_acl_rule_entry_create_with_counter (
                                          SAI_ACL_TABLE_TYPE_MAC, acl_counter_id);
    ASSERT_NE(0, mac_rule_id);

    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_PACKETS, 100000);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_BYTES, 200000);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Count Set */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 5);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       5, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID,
                                       SAI_ACL_COUNTER_ATTR_PACKETS,
                                       SAI_ACL_COUNTER_ATTR_BYTES);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[1].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[2].value.oid, mac_table_id);
    EXPECT_EQ (p_attr_list_get[3].value.u64, 200000);
    EXPECT_EQ (p_attr_list_get[4].value.u64, 200000);

    sai_test_acl_rule_entry_with_counter_remove (mac_rule_id);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLCounterTest, counter_get_with_invalid_counter_id)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;

    /* Get with Invalid Counter ID */

    printf ("Counter Get with Invalid Counter ID attribute\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_OBJECT_ID.\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 2);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get ((acl_counter_id + 1), p_attr_list_get,
                                       2, SAI_ACL_COUNTER_ATTR_TABLE_ID,
                                       SAI_ACL_COUNTER_ATTR_PACKETS);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLCounterTest, counter_get_with_out_of_range_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;
    unsigned int             out_of_range_attr = SAI_ACL_COUNTER_ATTR_BYTES + 1;
    sai_attribute_t         *p_attr_list_get = NULL;

    /* Get with Invalid Counter ID */

    printf ("Counter Get with Invalid Counter ID attribute\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_PARAMETER.\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 2);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       2, SAI_ACL_COUNTER_ATTR_TABLE_ID,
                                       out_of_range_attr);
    EXPECT_EQ (sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTRIBUTE_0, 1), sai_rc);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLCounterTest, counter_get_with_mismatch_counter_type)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;

    /* Create ACL Counter without Mandatory attr */

    printf ("Counter Get with Mismatch Counter Type\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_ATTRIBUTE\r\n");

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 1);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id,
                                       p_attr_list_get, 1,
                                       SAI_ACL_COUNTER_ATTR_BYTES);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, mac_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 1);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get, 1,
                                       SAI_ACL_COUNTER_ATTR_PACKETS);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLCounterTest, counter_get_valid)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_counter_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    sai_object_id_t          ip_rule_id = 0;

    /* Create ACL Counter without Mandatory attr */

    printf ("Counter Get with valid attr\r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS\r\n");

    /* Create Default Counter Type (Byte) */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 1,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    ip_rule_id = sai_test_acl_rule_entry_create_with_counter (
                                          SAI_ACL_TABLE_TYPE_IP, acl_counter_id);
    ASSERT_NE(0, ip_rule_id);

    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_BYTES, 100000);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Count Set */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 4);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       4, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID,
                                       SAI_ACL_COUNTER_ATTR_BYTES);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, false);
    EXPECT_EQ (p_attr_list_get[1].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[2].value.oid, ip_table_id);
    EXPECT_EQ (p_attr_list_get[3].value.u64, 100000);

    sai_test_acl_rule_entry_with_counter_remove (ip_rule_id);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Same Test for Packet Type Counter */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    ip_rule_id = sai_test_acl_rule_entry_create_with_counter (
                                          SAI_ACL_TABLE_TYPE_IP, acl_counter_id);
    ASSERT_NE(0, ip_rule_id);

    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_PACKETS, 100000);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Count Set */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 4);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       4, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID,
                                       SAI_ACL_COUNTER_ATTR_PACKETS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[1].value.booldata, false);
    EXPECT_EQ (p_attr_list_get[2].value.oid, ip_table_id);
    EXPECT_EQ (p_attr_list_get[3].value.u64, 100000);

    sai_test_acl_rule_entry_with_counter_remove (ip_rule_id);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Same Test for both Packet and Byte Type Counter */
    sai_rc = sai_test_acl_counter_create (&acl_counter_id, 3,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    ip_rule_id = sai_test_acl_rule_entry_create_with_counter (
                                          SAI_ACL_TABLE_TYPE_IP, acl_counter_id);
    ASSERT_NE(0, ip_rule_id);

    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_PACKETS, 100000);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_set (acl_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_BYTES, 200000);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Counter Count Set */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 5);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (acl_counter_id, p_attr_list_get,
                                       5, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID,
                                       SAI_ACL_COUNTER_ATTR_PACKETS,
                                       SAI_ACL_COUNTER_ATTR_BYTES);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[1].value.booldata, true);
    EXPECT_EQ (p_attr_list_get[2].value.oid, ip_table_id);
    EXPECT_EQ (p_attr_list_get[3].value.u64, 200000);
    EXPECT_EQ (p_attr_list_get[4].value.u64, 200000);

    sai_test_acl_rule_entry_with_counter_remove (ip_rule_id);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);

    sai_rc = sai_test_acl_counter_remove (acl_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLCounterTest, counter_with_invalid_object_type)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          invalid_counter_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;

    sai_rc = sai_test_acl_counter_set (invalid_counter_id, 1,
                                       SAI_ACL_COUNTER_ATTR_BYTES, 100000);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, sai_rc);

    /* Fetch the Counter Count Set */
    sai_rc = sai_test_acl_counter_create_attr_list(&p_attr_list_get, 4);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_counter_get (invalid_counter_id, p_attr_list_get,
                                       4, SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,
                                       SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,
                                       SAI_ACL_COUNTER_ATTR_TABLE_ID,
                                       SAI_ACL_COUNTER_ATTR_BYTES);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, sai_rc);

    sai_test_acl_counter_free_attr_list (p_attr_list_get);
}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
