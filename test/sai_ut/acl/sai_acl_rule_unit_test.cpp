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
 * *    sai_acl_rule_unit_test.cpp
 * *     
 * * Abstract:
 * *
 * *    SAI ACL RULE TEST :- Covers the test cases for validating
 * *    all public APIs in SAI ACL RULE module.
 * *
 * *************************************************************************/
#include "gtest/gtest.h"

#include "sai_acl_unit_test_utils.h"
#include "sai_l3_unit_test_utils.h"

extern "C" {
#include "saiswitch.h"
#include "saistatus.h"
#include "saitypes.h"
#include "saiacl.h"
#include "sainexthop.h"
#include "saineighbor.h"
#include "sainexthopgroup.h"
#include <inttypes.h>
}

class saiACLRuleTest : public saiACLTest
{
    public:
        static void SetUpTestCase (void);
        static void TearDownTestCase (void);

        static sai_status_t sai_test_acl_rule_setup_nexthop(
                                    sai_object_id_t *group_id,
                                    sai_object_id_t *p_nh_id_list,
                                    sai_object_id_t port_rif_id);

        static sai_status_t sai_test_acl_rule_cleanup_nexthop(
                                        sai_object_id_t group_id,
                                        sai_object_id_t *p_nh_id_list,
                                        sai_object_id_t port_rif_id);

        enum sai_acl_table_type_t {SAI_ACL_TABLE_TYPE_MAC, SAI_ACL_TABLE_TYPE_IP,
                                   SAI_ACL_TABLE_TYPE_IPv6, SAI_ACL_TABLE_TYPE_IP_MAC};

        static sai_object_id_t     sai_test_acl_rule_entry_create (
                                                            sai_acl_table_type_t table_type);
        static void                sai_test_acl_rule_entry_remove (
                                                            sai_object_id_t acl_rule_id);
        static sai_status_t        sai_test_acl_rule_create_attr_list (
                                                            sai_attribute_t **p_attr_list,
                                                            unsigned int attr_count,
                                                            unsigned int list_count,
                                                            unsigned int list_idx,
                                                            bool is_field_list);
        static void                sai_test_acl_rule_free_attr_list (
                                                            sai_attribute_t *p_attr_list,
                                                            unsigned int attr_count);

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
        static sai_object_id_t port_id_2;
        static sai_object_id_t port_id_3;
        static sai_object_id_t port_id_4;
};

enum sai_acl_table_type_ {
    SAI_ACL_TABLE_TYPE_MAC,
    SAI_ACL_TABLE_TYPE_IP,
    SAI_ACL_TABLE_TYPE_IPv6,
    SAI_ACL_TABLE_TYPE_IP_MAC
} sai_acl_table_type_t;

const sai_mac_t         saiACLRuleTest ::src_mac_data = {0x01,0x02,0x03,0x04,0x05,0x06};
const sai_mac_t         saiACLRuleTest ::src_mac_mask = {0xff,0xff,0xff,0xff,0xff,0xff};
const sai_mac_t         saiACLRuleTest ::dst_mac_data = {0x06,0x05,0x04,0x03,0x02,0x01};
const sai_mac_t         saiACLRuleTest ::dst_mac_mask = {0xff,0xff,0xff,0xff,0xff,0xff};

const sai_ip4_t         saiACLRuleTest ::src_ip_data = 0x01020304;
const sai_ip4_t         saiACLRuleTest ::src_ip_mask = 0xffffffff;
const sai_ip4_t         saiACLRuleTest ::dst_ip_data = 0x04030201;
const sai_ip4_t         saiACLRuleTest ::dst_ip_mask = 0xffffffff;

const sai_ip6_t         saiACLRuleTest ::src_ipv6_data[] =
                                            {0x00,0x01,0x00,0x02,0x00,0x03,0x00,0x04,
                                             0x00,0x05,0x00,0x06,0x00,0x07,0x00,0x08};
const sai_ip6_t         saiACLRuleTest ::src_ipv6_mask[] =
                                            {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
                                             0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
const sai_ip6_t         saiACLRuleTest ::dst_ipv6_data[] =
                                            {0x00,0x08,0x00,0x07,0x00,0x06,0x00,0x05,
                                             0x00,0x04,0x00,0x03,0x00,0x02,0x00,0x01};
const sai_ip6_t         saiACLRuleTest ::dst_ipv6_mask[] =
                                            {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
                                             0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

const uint64_t        saiACLRuleTest ::standard_mask = 0xffffffffffffffff;

sai_object_id_t  saiACLRuleTest ::mac_table_id = 0;
sai_object_id_t  saiACLRuleTest ::mac_egress_table_id = 0;
sai_object_id_t  saiACLRuleTest ::ip_table_id = 0;
sai_object_id_t  saiACLRuleTest ::ip_egress_table_id = 0;
sai_object_id_t  saiACLRuleTest ::ipv6_table_id = 0;
sai_object_id_t  saiACLRuleTest ::ipv6_egress_table_id = 0;
sai_object_id_t  saiACLRuleTest ::ip_and_mac_table_id = 0;
sai_object_id_t  saiACLRuleTest ::port_id_1= 0;
sai_object_id_t  saiACLRuleTest ::port_id_2= 0;
sai_object_id_t  saiACLRuleTest ::port_id_3= 0;
sai_object_id_t  saiACLRuleTest ::port_id_4= 0;

void saiACLRuleTest ::SetUpTestCase (void)
{
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;

    /* Base SetUpTestCase for SAI initialization */
    saiACLTest ::SetUpTestCase ();

    saiL3Test ::SetUpL3ApiQuery ();

    /* Create MAC Based Ingress ACL Table */
    sai_rc = sai_test_acl_table_create (&mac_table_id, 15,
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
                                        SAI_ACL_TABLE_ATTR_FIELD_DSCP,
                                        SAI_ACL_TABLE_ATTR_FIELD_ECN,
                                        SAI_ACL_TABLE_ATTR_FIELD_IPv6_FLOW_LABEL,
                                        SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("Ingress MAC ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                 mac_table_id);
    }

    /* Create MAC Based Egress ACL Table */
    sai_rc = sai_test_acl_table_create (&mac_egress_table_id, 12,
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
                                        SAI_ACL_TABLE_ATTR_FIELD_ECN,
                                        SAI_ACL_TABLE_ATTR_FIELD_OUT_PORT);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("Egress MAC ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                 mac_egress_table_id);
    }

    /* Create IP Based Ingress ACL Table */
    sai_rc = sai_test_acl_table_create (&ip_table_id, 17,
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
                                        SAI_ACL_TABLE_ATTR_FIELD_ICMP_TYPE,
                                        SAI_ACL_TABLE_ATTR_FIELD_ICMP_CODE,
                                        SAI_ACL_TABLE_ATTR_FIELD_IPv6_FLOW_LABEL);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("Ingress IP ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                 ip_table_id);
    }

    /* Create IP Based Egress ACL Table */
    sai_rc = sai_test_acl_table_create (&ip_egress_table_id, 14,
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
                                        SAI_ACL_TABLE_ATTR_FIELD_ICMP_TYPE,
                                        SAI_ACL_TABLE_ATTR_FIELD_OUT_PORT);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("Egress IP ACL Table Successfully created with ID 0x%"PRIx64" \r\n",
                 ip_egress_table_id);
    }

    /* Create IPv6 Based Ingress ACL Table */
    sai_rc = sai_test_acl_table_create (&ipv6_table_id, 16,
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
                                        SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                                        SAI_ACL_TABLE_ATTR_FIELD_IN_PORT,
                                        SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS,
                                        SAI_ACL_TABLE_ATTR_FIELD_IP_FRAG,
                                        SAI_ACL_TABLE_ATTR_FIELD_ICMP_TYPE,
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
    port_id_2 = sai_acl_port_id_get (1);
    port_id_3 = sai_acl_port_id_get (2);
    port_id_4 = sai_acl_port_id_get (3);
}

void saiACLRuleTest ::TearDownTestCase (void)
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

sai_object_id_t saiACLRuleTest ::sai_test_acl_rule_entry_create (
                                                    sai_acl_table_type_t table_type)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_rule_id = 0;

    switch (table_type) {
        case SAI_ACL_TABLE_TYPE_MAC:
            /* Create MAC Type ACL Rule */
            sai_rc = sai_test_acl_rule_create (&acl_rule_id, 13,
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
                                       1, (unsigned long) SAI_ACL_IP_TYPE_ARP,
                                       standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_ID,
                                       1, 10, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS,
                                       1, 1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ECN,
                                       1, 1, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IPv6_FLOW_LABEL,
                                       1, 100, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP,
                                       SAI_ACL_ENTRY_ATTR_ACTION_SET_TC, true, 5);

            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
            break;
        case SAI_ACL_TABLE_TYPE_IP:
            /* Create MAC Type ACL Rule */
            sai_rc = sai_test_acl_rule_create (&acl_rule_id, 17,
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
                                       SAI_ACL_ENTRY_ATTR_FIELD_IPv6_FLOW_LABEL,
                                       1, 200, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE,
                                       1, 8, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE,
                                       1, 4, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP,
                                       SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_MAC,
                                       true, &src_mac_data);

            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
            break;
        case SAI_ACL_TABLE_TYPE_IPv6:
            /* Create MAC Type ACL Rule */
            sai_rc = sai_test_acl_rule_create (&acl_rule_id, 17,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ipv6_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 25,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_IPv6,
                                       1, &src_ipv6_data,
                                       &src_ipv6_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_IPv6,
                                       1, &dst_ipv6_data,
                                       &dst_ipv6_mask,
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
                                       SAI_ACL_ENTRY_ATTR_FIELD_IPv6_FLOW_LABEL,
                                       1, 300, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE,
                                       1, 0x86, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP,
                                       SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_MAC,
                                       true, &dst_mac_data);

            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
            break;
        case SAI_ACL_TABLE_TYPE_IP_MAC:
            /* Create MAC Type ACL Rule */
            sai_rc = sai_test_acl_rule_create (&acl_rule_id, 14,
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
                                       SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_ID,
                                       1, 100, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_PRI,
                                       1, 5, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_CFI,
                                       1, 1, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP);

            EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
            break;
        default:
            printf ("Unknown ACL Table Type passed \r\n");
            break;
    }

    return acl_rule_id;
}

void saiACLRuleTest ::sai_test_acl_rule_entry_remove(sai_object_id_t acl_rule_id)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;

    /* Delete the ACL Rule */
    sai_rc = sai_test_acl_rule_remove (acl_rule_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

sai_status_t saiACLRuleTest ::sai_test_acl_rule_create_attr_list(
                                            sai_attribute_t **p_attr_list,
                                            unsigned int attr_count,
                                            unsigned int list_count,
                                            unsigned int list_idx,
                                            bool is_field_list)
{
    sai_attribute_t *attribute_list = NULL;
    sai_object_id_t   *list = NULL;
    unsigned int     attr_cnt = 0;

    if (attr_count == 0) {
        return SAI_STATUS_INVALID_PARAMETER;
    }

    attribute_list = (sai_attribute_t *) calloc (attr_count, sizeof (sai_attribute_t));

    if (attribute_list == NULL) {
        printf ("Failed to allocate memory for attribute list.\r\n");
        return SAI_STATUS_NO_MEMORY;
    }

    if (list_count > 0) {
        if ((list_idx < 1) || (list_idx > attr_count)) {
            return SAI_STATUS_INVALID_PARAMETER;
        }
        list = (sai_object_id_t *)calloc(list_count, sizeof(sai_object_id_t));
        if (list == NULL) {
            return SAI_STATUS_NO_MEMORY;
        }

        if (is_field_list) {
            attribute_list[list_idx - 1].value.aclfield.data.objlist.list = list;
            attribute_list[list_idx - 1].value.aclfield.data.objlist.count = list_count;
        } else {
            attribute_list[list_idx - 1].value.aclaction.parameter.objlist.list = list;
            attribute_list[list_idx - 1].value.aclaction.parameter.objlist.count = list_count;
        }
    } else {
        for (attr_cnt = 0; attr_cnt < attr_count; attr_cnt++) {
            attribute_list[attr_cnt].value.aclfield.data.objlist.list = NULL;
            attribute_list[attr_cnt].value.aclfield.data.objlist.count = 0;
            attribute_list[attr_cnt].value.aclaction.parameter.objlist.list = NULL;
            attribute_list[attr_cnt].value.aclaction.parameter.objlist.count = 0;
        }
    }

    *p_attr_list = (sai_attribute_t *)attribute_list;

    return SAI_STATUS_SUCCESS;
}

void saiACLRuleTest ::sai_test_acl_rule_free_attr_list(
                                            sai_attribute_t *p_attr_list,
                                            unsigned int attr_count)
{
    unsigned int attr_cnt = 0;

    for (attr_cnt = 0; attr_cnt < attr_count; attr_cnt++) {
         if ((p_attr_list[attr_cnt].id == SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS) ||
             (p_attr_list[attr_cnt].id == SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORTS)) {
             if (p_attr_list[attr_cnt].value.aclfield.data.objlist.list &&
                 (p_attr_list[attr_cnt].value.aclfield.data.objlist.count != 0)) {
                 free (p_attr_list[attr_cnt].value.aclfield.data.objlist.list);
             }
         } else if ((p_attr_list[attr_cnt].id == SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS) ||
                    (p_attr_list[attr_cnt].id == SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS)) {
             if (p_attr_list[attr_cnt].value.aclaction.parameter.objlist.list &&
                 (p_attr_list[attr_cnt].value.aclaction.parameter.objlist.count != 0)) {
                 free (p_attr_list[attr_cnt].value.aclaction.parameter.objlist.list);
             }
         }
    }

    free (p_attr_list);
}

sai_status_t saiACLRuleTest ::sai_test_acl_rule_setup_nexthop(
                                    sai_object_id_t *group_id,
                                    sai_object_id_t *p_nh_id_list,
                                    sai_object_id_t port_rif_id)
{
    sai_status_t    status = SAI_STATUS_SUCCESS;
    unsigned int    max_paths = 0;
    const char     *p_neighbor_mac = "00:a1:a2:a3:a4:00";
    unsigned int    count = 0;
    unsigned int    addr_byte4 = 0;
    char            ip_addr_str [64];
    unsigned int    default_nh_attr_count = 3;
    unsigned int    default_neighbor_attr_count = 1;
    unsigned int    default_nh_group_attr_count = 2;
    sai_object_id_t nexthop_group_id = 0;

    EXPECT_NE ((sai_object_id_t *) NULL, group_id);
    EXPECT_NE ((sai_object_id_t *) NULL, p_nh_id_list);

    max_paths = 3;

    for (count = 0; count < max_paths; count++)
    {
        addr_byte4++;
        snprintf (ip_addr_str, sizeof (ip_addr_str), "10.0.0.%d", addr_byte4);

        status = saiL3Test::sai_test_nexthop_create (&p_nh_id_list [count],
                                                     default_nh_attr_count,
                                                     SAI_NEXT_HOP_ATTR_TYPE,
                                                     SAI_NEXT_HOP_IP,
                                                     SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                                     port_rif_id,
                                                     SAI_NEXT_HOP_ATTR_IP,
                                                     SAI_IP_ADDR_FAMILY_IPV4, ip_addr_str);

        EXPECT_EQ (SAI_STATUS_SUCCESS, status);

        status = saiL3Test::sai_test_neighbor_create (port_rif_id, SAI_IP_ADDR_FAMILY_IPV4,
                                                      ip_addr_str,
                                                      default_neighbor_attr_count,
                                                      SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS,
                                                      p_neighbor_mac);

        EXPECT_EQ (SAI_STATUS_SUCCESS, status);
    }

    status = saiL3Test::sai_test_nh_group_create (&nexthop_group_id,
                                                  p_nh_id_list,
                                                  default_nh_group_attr_count,
                                                  SAI_NEXT_HOP_GROUP_ATTR_TYPE,
                                                  SAI_NEXT_HOP_GROUP_ECMP,
                                                  SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST,
                                                  max_paths);
    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    printf ("SAI Next Hop Group Object Id created in ACL: 0x%"PRIx64".\n", nexthop_group_id);

    *group_id = nexthop_group_id;

    return SAI_STATUS_SUCCESS;
}

sai_status_t saiACLRuleTest ::sai_test_acl_rule_cleanup_nexthop(
                                        sai_object_id_t group_id,
                                        sai_object_id_t *p_nh_id_list,
                                        sai_object_id_t port_rif_id)
{
    unsigned int    count = 0;
    sai_status_t    status = SAI_STATUS_SUCCESS;
    unsigned int    addr_byte4 = 0;
    unsigned int    max_paths = 0;
    char            ip_addr_str [64];

    EXPECT_NE ((sai_object_id_t *) NULL, p_nh_id_list);

    max_paths = 3;

    status = saiL3Test::sai_test_remove_nh_from_group (group_id, 2,
                                                       p_nh_id_list);
    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    status = saiL3Test::sai_test_nh_group_remove (group_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    for (count = 0; count < max_paths; count++)
    {
        status = saiL3Test::sai_test_nexthop_remove (p_nh_id_list [count]);

        EXPECT_EQ (SAI_STATUS_SUCCESS, status);

        addr_byte4++;

        snprintf (ip_addr_str, sizeof (ip_addr_str), "10.0.0.%d", addr_byte4);

        status = saiL3Test::sai_test_neighbor_remove (port_rif_id, SAI_IP_ADDR_FAMILY_IPV4,
                                           ip_addr_str);

        EXPECT_EQ (SAI_STATUS_SUCCESS, status);
    }

    return SAI_STATUS_SUCCESS;
}

TEST_F(saiACLRuleTest, rule_create_with_null_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_rule_id = 0;

    /* Create ACL Rule with Null attr count */

    printf ("Rule Creation with Null attributes\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_PARAMETER.\r\n");

    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 0);
    EXPECT_EQ (SAI_STATUS_INVALID_PARAMETER, sai_rc);
}

TEST_F(saiACLRuleTest, rule_create_with_out_of_range_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_rule_id = 0;

    printf ("Rule Creation with Out of Range attributes\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_ATTRIBUTE.\r\n");

    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 1,
                            SAI_ACL_ENTRY_ATTR_TABLE_ID - 1);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);

    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 1,
                            SAI_ACL_ENTRY_ATTR_ACTION_END + 1);
    EXPECT_EQ (SAI_STATUS_INVALID_ATTRIBUTE_0, sai_rc);
}

TEST_F(saiACLRuleTest, rule_create_with_unknown_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_rule_id = 0;

    printf ("Rule Creation with Unknown attributes\r\n");
    printf ("Expecting error - SAI_STATUS_UNKNOWN_ATTRIBUTE_0.\r\n");

    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 2,
                            (SAI_ACL_ENTRY_ATTR_ADMIN_STATE + 1), true,
                            SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT, 1, 14, 255);
    EXPECT_EQ (SAI_STATUS_UNKNOWN_ATTRIBUTE_0, sai_rc);

    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 2,
                            (SAI_ACL_ENTRY_ATTR_FIELD_END + 1), 1,
                            SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT, 1, 14, 255);
    EXPECT_EQ (SAI_STATUS_UNKNOWN_ATTRIBUTE_0, sai_rc);
}

TEST_F(saiACLRuleTest, rule_create_with_dup_attr)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_rule_id = 0;

    printf ("Rule Creation with Duplicate Attributes\r\n");
    printf ("Expecting error - SAI_STATUS_INVALID_ATTRIBUTE.\r\n");

    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 4,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, 1,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 10,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 11 /* Duplicate */);
    EXPECT_EQ (sai_test_invalid_attr_status_code (SAI_STATUS_INVALID_ATTRIBUTE_0, 3), sai_rc);
}

TEST_F(saiACLRuleTest, rule_create_without_field_attr)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_rule_id = 0;

    printf ("Rule Creation without Field Attributes\r\n");
    printf ("Expecting error - SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING.\r\n");

    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 4,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, 1,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 10,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_LOG);
    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_rc);
}


TEST_F(saiACLRuleTest, rule_create_without_mandatory_attr)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_rule_id = 0;
    unsigned int          invalid_table_id = 0;

    printf ("Rule Creation without Mandatory Attributes\r\n");
    printf ("Expecting error - SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING.\r\n");

    /* Create ACL Rule without ACL Table ID */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 4,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 10,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT, 1, 14, 255,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_LOG);
    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_rc);

    /* Create ACL Rule with invalid ACL Table ID */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 5,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, invalid_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 10,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT, 1, 14, 255,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_LOG);
    EXPECT_EQ (SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING, sai_rc);

    /* Create ACL Rule with invalid ACL Counter Id */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 5,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 10,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT, 1, 14, 255,
                                       SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                       1, 2/* Invalid Counter ID */);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, sai_rc);

}

TEST_F(saiACLRuleTest, rule_create_with_field_not_in_table)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_rule_id = 0;

    printf ("Rule Creation with invalid field attribute\r\n");
    printf ("Expecting error - SAI_STATUS_ITEM_NOT_FOUND.\r\n");

    /* Create ACL Rule with valid attributes */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 5,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC, 1,
                                       src_mac_data, src_mac_mask, /* Not present in ACL Table */
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_LOG);
    EXPECT_EQ (SAI_STATUS_ITEM_NOT_FOUND, sai_rc);

}

TEST_F(saiACLRuleTest, rule_create_with_unsupported_fields)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_rule_id = 0;

    printf ("Rule Creation with unsupported fields based on stage\r\n");
    printf ("Expecting error - SAI_STATUS_NOT_SUPPORTED.\r\n");

    /* Create ACL Rule with unsupported field in Ingress Stage */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 5,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT, 1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_LOG);
    EXPECT_EQ (SAI_STATUS_NOT_SUPPORTED, sai_rc);

    /* Create ACL Rule with unsupported field in Egress Stage */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 5,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_egress_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS, 1, 1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_DROP);
    EXPECT_EQ (SAI_STATUS_NOT_SUPPORTED, sai_rc);

    /* Create ACL Rule with unsupported action in Ingress Stage */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 5,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS, 1, 1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_ACTION_DECREMENT_TTL /* Unsupported Ingress Action */,
                                       true, 0, 0);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        EXPECT_EQ (SAI_STATUS_NOT_SUPPORTED, sai_rc);
    } else {
        /* Delete the ACL Rule */
        sai_rc = sai_test_acl_rule_remove (acl_rule_id);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }

    /* Create ACL Rule with unsupported action in Egress Stage */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 5,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_egress_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT, 1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_LOG);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        EXPECT_EQ (SAI_STATUS_NOT_SUPPORTED, sai_rc);
    } else {
        /* Delete the ACL Rule */
        sai_rc = sai_test_acl_rule_remove (acl_rule_id);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }
}


TEST_F(saiACLRuleTest, rule_create_and_remove)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_rule_id_ingress = 0;
    sai_object_id_t       acl_rule_id_egress = 0;

    printf ("Rule Creation with valid attributes for Ingress Stage\r\n");
    printf ("Expecting error - SAI_STATUS_SUCCESS.\r\n");

    /* Create ACL Rule with valid attributes on Ingress */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id_ingress, 7,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, src_mac_mask,
                                       SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,
                                       1, &dst_mac_data, dst_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT,
                                       1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_LOG);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("Ingress ACL Rule Successfully created with ID 0x%"PRIx64" \r\n",
                 acl_rule_id_ingress);
    }

    /* Create ACL Rule with valid attributes on Egress */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id_egress, 7,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_egress_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, src_mac_mask,
                                       SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,
                                       1, &dst_mac_data, dst_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT,
                                       1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_DROP);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    if (sai_rc == SAI_STATUS_SUCCESS) {
        printf ("Egress ACL Rule Successfully created with ID 0x%"PRIx64" \r\n",
                 acl_rule_id_egress);
    }

    /* Delete the ACL Rule */
    sai_rc = sai_test_acl_rule_remove (acl_rule_id_ingress);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Delete the ACL Rule */
    sai_rc = sai_test_acl_rule_remove (acl_rule_id_egress);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLRuleTest, rule_create_with_cpu_port)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       acl_rule_id = 0;
    sai_object_id_t       cpu_port = 0;

    cpu_port = sai_test_acl_get_cpu_port ();

    printf("Ethernet CPU Port = 0x%"PRIx64" \r\n", cpu_port);

    /* Create ACL Rule with Ethernet CPU Port as In Port */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 4,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT,
                                       1, cpu_port);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Delete the ACL Rule */
    sai_rc = sai_test_acl_rule_remove (acl_rule_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

}

TEST_F(saiACLRuleTest, rule_create_with_counter)
{
    sai_status_t        sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t     acl_rule_id = 0;
    sai_object_id_t     acl_packet_counter_id = 0;
    sai_object_id_t     acl_byte_counter_id = 0;
    sai_object_id_t     acl_packet_byte_counter_id = 0;
    sai_attribute_t    *p_attr_list_get = NULL;
    unsigned int        test_attr_count = 1;

    /* Create Packet type Counter */
    sai_rc = sai_test_acl_counter_create (&acl_packet_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    printf ("ACL Packet Counter Successfully created with ID 0x%"PRIx64" \r\n",
             acl_packet_counter_id);

    /* Fetch the default values using the GET API */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    /* Create ACL Rule with Counter Action for NULL counter id */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 5,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT,
                                       1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                       1, 0);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, sai_rc);

    /* Create ACL Rule with Counter Action for invalid counter */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 5,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT,
                                       1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                       1, (acl_packet_counter_id + 1) /* Wrong Counter Id */);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Create ACL Rule with Counter Action for wrong table */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 5,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_table_id /* Wrong Table */,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT,
                                       1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                       1, acl_packet_counter_id);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Create ACL Rule with Counter Action for correct table */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 5,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT,
                                       1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                       1, acl_packet_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                    1, 0);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, sai_rc);

    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                    1, (acl_packet_counter_id + 1));
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Create Byte type Counter */
    sai_rc = sai_test_acl_counter_create (&acl_byte_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ipv6_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    printf ("ACL Byte Counter Successfully created with ID 0x%"PRIx64" \r\n",
             acl_byte_counter_id);

    /* Table mismatch */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                    1, acl_byte_counter_id);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Delete and create Byte type counter for the correct ACL Table */
    sai_rc = sai_test_acl_counter_remove (acl_byte_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_create (&acl_byte_counter_id, 2,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    printf ("ACL Byte Counter Successfully created with ID 0x%"PRIx64" \r\n",
             acl_byte_counter_id);

    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                    1, acl_byte_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                    1, acl_packet_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                    0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Delete the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_byte_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_counter_remove (acl_packet_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Delete the ACL Rule */
    sai_rc = sai_test_acl_rule_remove (acl_rule_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* ACL Counter as a new action in Set API */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 4,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT,
                                       1, port_id_1);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create Packet-Byte type Counter */
    sai_rc = sai_test_acl_counter_create (&acl_packet_byte_counter_id, 3,
                                          SAI_ACL_COUNTER_ATTR_TABLE_ID, ip_table_id,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT, true,
                                          SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT, true);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Attach the counter to the rule */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                    1, acl_packet_byte_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (acl_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_ACTION_COUNTER);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.oid,
                                            acl_packet_byte_counter_id);

    /* Detach the counter from the rule */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                    0, acl_packet_byte_counter_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (acl_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_ACTION_COUNTER);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, false);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.oid, 0);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);

    /* Delete the Counter */
    sai_rc = sai_test_acl_counter_remove (acl_packet_byte_counter_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Delete the ACL Rule */
    sai_rc = sai_test_acl_rule_remove (acl_rule_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLRuleTest, rule_create_with_defaults)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             test_attr_count = 3;

    /* Create ACL Rule with Default Attributes */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 3,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ipv6_table_id,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_IPv6,
                                       1, &src_ipv6_data,
                                       &src_ipv6_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_IPv6,
                                       1, &dst_ipv6_data,
                                       &dst_ipv6_mask);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the default values using the GET API */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (acl_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_TABLE_ID,
                                    SAI_ACL_ENTRY_ATTR_PRIORITY,
                                    SAI_ACL_ENTRY_ATTR_ADMIN_STATE);

    EXPECT_EQ (p_attr_list_get[0].value.oid, ipv6_table_id);

    /* Default ACL Rule attribute values */
    EXPECT_EQ (p_attr_list_get[1].value.u32, 0);
    EXPECT_EQ (p_attr_list_get[2].value.booldata, true);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);
    sai_test_acl_rule_entry_remove (acl_rule_id);
}

TEST_F(saiACLRuleTest, rule_create_with_port_list)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       rule_id = 0, port_rule_id = 0;
    sai_attribute_t      *p_attr_list_get = NULL;
    unsigned int          test_attr_count = 1;
    unsigned int          count = 2;

    /* Create ACL Rule with In Port List */
    sai_rc = sai_test_acl_rule_create (&port_rule_id, 3,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_table_id,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS,
                                       1, count, port_id_1, port_id_3,
                                       SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,
                                       1, &dst_mac_data, &dst_mac_mask);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create ACL Rule without In Port List */
    sai_rc = sai_test_acl_rule_create (&rule_id, 3,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ipv6_table_id,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_IPv6,
                                       1, &src_ipv6_data,
                                       &src_ipv6_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DST_IPv6,
                                       1, &dst_ipv6_data,
                                       &dst_ipv6_mask);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the In Port List using the GET API but with smaller port list*/
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get,
                    test_attr_count, 1 /* Lesser than actual port count */, 1, true);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (port_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS);

    EXPECT_EQ (SAI_STATUS_BUFFER_OVERFLOW, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.count, count);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);

    /* Add new IN Port list to the rule not containing port list */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS, 1,
                                    count, port_id_1, port_id_3);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get,
                        test_attr_count, 4, 1, true);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.count, count);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[0], port_id_1);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[1], port_id_3);

    /* Modify the port list in the rule with same port count */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS, 1,
                                    count, port_id_2, port_id_4);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.count, count);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[0], port_id_2);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[1], port_id_4);

    /* Modify the port list in the rule with different port count */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS, 1,
                                    4, port_id_1, port_id_2, port_id_3, port_id_4);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    p_attr_list_get[0].value.aclfield.data.objlist.count = 4;

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.count, 4);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[0], port_id_1);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[1], port_id_2);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[2], port_id_3);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[3], port_id_4);

    /* Remove the Port list and add it again */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS, 0, 1, port_id_4);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, false);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.count, 0);

    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS, 1,
                                    1, port_id_4);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    p_attr_list_get[0].value.aclfield.data.objlist.count = 1;

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.count, 1);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[0], port_id_4);

    /* Modify the port list in the rule which had port list during creation */
    p_attr_list_get[0].value.aclfield.data.objlist.count = 4;

    sai_rc = sai_test_acl_rule_set (port_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS, 1, 4,
                                    port_id_1, port_id_2, port_id_3, port_id_4);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (port_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.count, 4);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[0], port_id_1);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[1], port_id_2);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[2], port_id_3);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[3], port_id_4);

    /* Remove the Port list and add it again */
    sai_rc = sai_test_acl_rule_set (port_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS, 0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (port_rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, false);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.count, 0);

    sai_rc = sai_test_acl_rule_set (port_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS, 1,
                                    1, port_id_2);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    p_attr_list_get[0].value.aclfield.data.objlist.count = 1;

    sai_rc = sai_test_acl_rule_get (port_rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.count, 1);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.objlist.list[0], port_id_2);

    /* Clean up */
    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);
    sai_test_acl_rule_entry_remove (port_rule_id);
    sai_test_acl_rule_entry_remove (rule_id);
}

TEST_F(saiACLRuleTest, rule_remove)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_rule_id = 0;

    /* Create ACL Rule */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 6,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 5,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS,
                                       1, 1, port_id_1,
                                       SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,
                                       1, &dst_mac_data, &dst_mac_mask,
                                       SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                                       1, 0x8809, 0xffff);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Delete the rule for a different Rule ID */
    sai_rc = sai_test_acl_rule_remove (acl_rule_id + 1);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Delete the rule with the correct rule id */
    sai_rc = sai_test_acl_rule_remove (acl_rule_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Delete the rule again with the correct rule id */
    sai_rc = sai_test_acl_rule_remove (acl_rule_id);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

}

TEST_F(saiACLRuleTest, rule_set_with_invalid_attributes)
{
    sai_status_t          sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t       mac_rule_id = 0;
    sai_object_id_t       invalid_rule_id = 0;
    sai_object_id_t       invalid_port_id = sai_acl_invalid_port_id_get();
    sai_object_id_t       invalid_counter_id = 0;

    mac_rule_id = sai_test_acl_rule_entry_create (SAI_ACL_TABLE_TYPE_MAC);

    ASSERT_NE(0, mac_rule_id);

    /* Table Id is READ-ONLY and cannot be changed */
    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                  SAI_ACL_ENTRY_ATTR_TABLE_ID, 10);
    EXPECT_EQ (SAI_STATUS_ATTR_NOT_SUPPORTED_0, sai_rc);

    /* Provide wrong ACL Rule ID */
    sai_rc = sai_test_acl_rule_set (invalid_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, sai_rc);

    /* Invalid Port Numbers */
    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS,
                                    1, 2, port_id_1, invalid_port_id);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT,
                                    1, invalid_port_id);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Invalid Counter Id */
    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,
                                    1, invalid_counter_id);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, sai_rc);

    /* Field which is not present in ACL table */
    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP,
                                    true, &src_ip_data, &src_ip_mask);
    EXPECT_EQ (SAI_STATUS_ITEM_NOT_FOUND, sai_rc);

    /* Unknown Attributes */
    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                            (SAI_ACL_ENTRY_ATTR_ADMIN_STATE + 1), true);

    EXPECT_EQ (SAI_STATUS_UNKNOWN_ATTRIBUTE_0, sai_rc);

    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                            (SAI_ACL_ENTRY_ATTR_FIELD_END + 1), 1);
    EXPECT_EQ (SAI_STATUS_UNKNOWN_ATTRIBUTE_0, sai_rc);

    /* Clean up */
    sai_test_acl_rule_entry_remove (mac_rule_id);
}

TEST_F(saiACLRuleTest, rule_set_with_basic_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          mac_rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             new_priority = 0;
    unsigned int             test_attr_count = 2;

    mac_rule_id = sai_test_acl_rule_entry_create (SAI_ACL_TABLE_TYPE_MAC);
    ASSERT_NE(0, mac_rule_id);

    /* Priority Change */
    printf("ACL Set Rule Attribute, Priority Change\r\n");
    printf("Expected Result - SAI_STATUS_SUCCESS\r\n");

    new_priority = 16;
    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PRIORITY, new_priority);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    printf("ACL Set Rule Attribute, Admin State Change\r\n");
    printf("Expected Result - SAI_STATUS_SUCCESS\r\n");

    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ADMIN_STATE, false);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (mac_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_PRIORITY,
                                    SAI_ACL_ENTRY_ATTR_ADMIN_STATE);

    EXPECT_EQ (p_attr_list_get[0].value.u32, new_priority);
    EXPECT_EQ (p_attr_list_get[1].value.booldata, false);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);

    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_acl_rule_entry_remove (mac_rule_id);
}

TEST_F(saiACLRuleTest, rule_set_with_mac_fields_and_actions)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          mac_rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    const sai_mac_t          new_src_mac = {0x07,0x08,0x09,0x0a,0x0b,0x0c};
    const sai_mac_t          new_dst_mac = {0x0c,0x0b,0x0a,0x09,0x08,0x07};
    unsigned int             test_attr_count = 5;

    mac_rule_id = sai_test_acl_rule_entry_create (SAI_ACL_TABLE_TYPE_MAC);

    ASSERT_NE(0, mac_rule_id);

    /* ACL Rule Field Set */
    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC,
                                    1, &new_src_mac, &src_mac_mask);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_DST_MAC,
                                    1, &new_dst_mac, &dst_mac_mask);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE,
                                    0, 0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS,
                                    1, 2, port_id_2, port_id_3);

    /* ACL Rule Action Set */
    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, false,
                                    SAI_PACKET_ACTION_TRAP);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                    SAI_PACKET_ACTION_LOG);

    /* Fetch all the new attributes and verify */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 2, 4, true);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (mac_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC,
                                    SAI_ACL_ENTRY_ATTR_FIELD_DST_MAC,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE,
                                    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION);


    EXPECT_EQ (0, (memcmp(p_attr_list_get[0].value.aclfield.data.mac,
               new_src_mac, sizeof (sai_mac_t))));
    EXPECT_EQ (0, (memcmp(p_attr_list_get[1].value.aclfield.data.mac,
               new_dst_mac, sizeof (sai_mac_t))));
    EXPECT_EQ (p_attr_list_get[2].value.aclfield.enable, false);
    EXPECT_EQ (p_attr_list_get[3].value.aclfield.data.objlist.count, 2);
    EXPECT_EQ (p_attr_list_get[3].value.aclfield.data.objlist.list[0],
               port_id_2);
    EXPECT_EQ (p_attr_list_get[3].value.aclfield.data.objlist.list[1],
               port_id_3);
    EXPECT_EQ (p_attr_list_get[4].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[4].value.aclaction.parameter.s32,
                                                SAI_PACKET_ACTION_LOG);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);
    sai_test_acl_rule_entry_remove (mac_rule_id);
}

TEST_F(saiACLRuleTest, rule_set_with_ip_fields_and_actions)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          ip_rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    const sai_ip4_t          new_dst_ip = 0x08070605;
    unsigned int             test_attr_count = 8;

    ip_rule_id = sai_test_acl_rule_entry_create (SAI_ACL_TABLE_TYPE_IP);
    ASSERT_NE(0, ip_rule_id);

    /* ACL Rule Field Set */
    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_TOS,
                                    0, 0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_TOS,
                                    1, 18, 0xff);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                                    0, "", "");
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_TABLE_ATTR_FIELD_DST_IP,
                                    1, &new_dst_ip, &dst_ip_mask);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                                    1, 2048, standard_mask);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,
                                    1, 35, standard_mask);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT,
                                    0, 10 /* This should be ignored */, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL,
                                    1, 17, standard_mask);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* ACL Rule Action Set */
    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                    1, port_id_1);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_MAC,
                                    false, "", "");
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch all the new attributes and verify */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (ip_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,
                                    SAI_ACL_TABLE_ATTR_FIELD_DST_IP,
                                    SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,
                                    SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT,
                                    SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT,
                                    SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                    SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_MAC);


    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, false);
    EXPECT_EQ (p_attr_list_get[1].value.aclfield.data.ip4, new_dst_ip);
    EXPECT_EQ (p_attr_list_get[2].value.aclfield.data.u16, 2048);
    EXPECT_EQ (p_attr_list_get[3].value.aclfield.data.u16, 35);
    EXPECT_EQ (p_attr_list_get[4].value.aclfield.enable, false);
    EXPECT_EQ (p_attr_list_get[5].value.aclfield.data.u8, 17);
    EXPECT_EQ (p_attr_list_get[6].id, SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT);
    EXPECT_EQ (p_attr_list_get[6].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[6].value.aclaction.parameter.oid, port_id_1);
    EXPECT_EQ (p_attr_list_get[7].value.aclaction.enable, false);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);
    sai_test_acl_rule_entry_remove (ip_rule_id);
}

TEST_F(saiACLRuleTest, rule_set_with_ipv6_fields_and_actions)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          ipv6_rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    const sai_ip6_t          new_src_ipv6 =
                             {0x00,0x09,0x00,0x0a,0x00,0x0b,0x00,0x0c,
                              0x00,0x0d,0x00,0x0e,0x00,0x0f,0x00,0x10};
    const sai_ip6_t          new_dst_ipv6 =
                             {0x00,0x10,0x00,0x0f,0x00,0x0e,0x00,0x0d,
                              0x00,0x0c,0x00,0x0b,0x00,0x0a,0x00,0x09};
    unsigned int             test_attr_count = 2;

    ipv6_rule_id = sai_test_acl_rule_entry_create (SAI_ACL_TABLE_TYPE_IPv6);
    ASSERT_NE(0, ipv6_rule_id);

    /* ACL Rule Field Set */
    sai_rc = sai_test_acl_rule_set (ipv6_rule_id, 1,
                                    SAI_ACL_TABLE_ATTR_FIELD_SRC_IPv6,
                                    1, &new_src_ipv6,
                                    &src_ipv6_mask);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (ipv6_rule_id, 1,
                                    SAI_ACL_TABLE_ATTR_FIELD_DST_IPv6,
                                    1, &new_dst_ipv6,
                                    &dst_ipv6_mask);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch all the new attributes and verify */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (ipv6_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_TABLE_ATTR_FIELD_SRC_IPv6,
                                    SAI_ACL_TABLE_ATTR_FIELD_DST_IPv6);


    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (0, (memcmp(p_attr_list_get[0].value.aclfield.data.ip6,
               new_src_ipv6, sizeof(sai_ip6_t))));
    EXPECT_EQ (p_attr_list_get[1].value.aclfield.enable, true);
    EXPECT_EQ (0, (memcmp(p_attr_list_get[1].value.aclfield.data.ip6,
               new_dst_ipv6, sizeof(sai_ip6_t))));

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);
    sai_test_acl_rule_entry_remove (ipv6_rule_id);
}

TEST_F(saiACLRuleTest, rule_set_with_packet_actions)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          ip_rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             test_attr_count = 1;

    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    ip_rule_id = sai_test_acl_rule_entry_create (SAI_ACL_TABLE_TYPE_IP);

    ASSERT_NE(0, ip_rule_id);

    sai_rc = sai_test_acl_rule_get (ip_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.s32, SAI_PACKET_ACTION_TRAP);

    /* Rule created with Packet Action as Trap */
    /* Overwrite the Trap Packet Action with Forward Packet Action (DropCancel)*/
    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                    SAI_PACKET_ACTION_FORWARD);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (ip_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.s32, SAI_PACKET_ACTION_FORWARD);

    /* Overwrite the Forward Packet Action with Log (COPY+FORWARD) */
    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                    SAI_PACKET_ACTION_LOG);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (ip_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.s32, SAI_PACKET_ACTION_LOG);

    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, false, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (ip_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, false);

    /* Packet Action - Drop */
    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                    SAI_PACKET_ACTION_DROP);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (ip_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.s32, SAI_PACKET_ACTION_DROP);

    /* Overwrite the Drop Packet Action with Copy */
    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                    SAI_PACKET_ACTION_COPY);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (ip_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.s32, SAI_PACKET_ACTION_COPY);

    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, false, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Packet Action - COPYCANCEL */
    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                    SAI_PACKET_ACTION_COPY_CANCEL);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (ip_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.s32, SAI_PACKET_ACTION_COPY_CANCEL);

    /* Overwrite the CopyCancel Packet Action with Deny (COPYCANCEL+DROP) */
    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                    SAI_PACKET_ACTION_DENY);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (ip_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.s32, SAI_PACKET_ACTION_DENY);

    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, false, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Packet Action - TRANSIT (COPYCANCEL+FORWARD) */
    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                    SAI_PACKET_ACTION_TRANSIT);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (ip_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.s32, SAI_PACKET_ACTION_TRANSIT);

    sai_rc = sai_test_acl_rule_set (ip_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION, false, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (ip_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_PACKET_ACTION);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, false);

    sai_test_acl_rule_entry_remove (ip_rule_id);
}

TEST_F(saiACLRuleTest, rule_set_with_new_field_and_action_add)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          mac_rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             test_attr_count = 2;

    mac_rule_id = sai_test_acl_rule_entry_create (SAI_ACL_TABLE_TYPE_MAC);

    ASSERT_NE(0, mac_rule_id);

    /* Adding new field to existing ACL Rule */
    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_PRI,
                                    1, 7, 255);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Adding new action to existing ACL Rule */
    sai_rc = sai_test_acl_rule_set (mac_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                    1, port_id_1);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch all the new attributes and verify */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (mac_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_PRI,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.u8, 7);
    EXPECT_EQ (p_attr_list_get[1].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[1].value.aclaction.parameter.oid, port_id_1);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);
    sai_test_acl_rule_entry_remove (mac_rule_id);
}

TEST_F(saiACLRuleTest, rule_set_with_outer_vlan_attr)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          ip_mac_rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             test_attr_count = 3;

    ip_mac_rule_id = sai_test_acl_rule_entry_create (SAI_ACL_TABLE_TYPE_IP_MAC);

    ASSERT_NE(0, ip_mac_rule_id);

    /* Adding new field to existing ACL Rule */
    sai_rc = sai_test_acl_rule_set (ip_mac_rule_id, 1,
                            SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_ID,
                            1, 200, 255);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Adding new field to existing ACL Rule */
    sai_rc = sai_test_acl_rule_set (ip_mac_rule_id, 1,
                            SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_PRI,
                            0, 0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Adding new field to existing ACL Rule */
    sai_rc = sai_test_acl_rule_set (ip_mac_rule_id, 1,
                            SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_CFI,
                            1, 0, 255);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch all the new attributes and verify */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (ip_mac_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_ID,
                                    SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_PRI,
                                    SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_CFI);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.u16, 200);
    EXPECT_EQ (p_attr_list_get[1].value.aclfield.enable, false);
    EXPECT_EQ (p_attr_list_get[2].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[2].value.aclfield.data.u8, 0);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);
    sai_test_acl_rule_entry_remove (ip_mac_rule_id);
}

TEST_F(saiACLRuleTest, rule_with_invalid_object_type)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          invalid_rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             test_attr_count = 3;

    /* Adding new field to existing ACL Rule */
    sai_rc = sai_test_acl_rule_set (invalid_rule_id, 1,
                            SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_ID,
                            1, 200, 255);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, sai_rc);

    /* Fetch all the new attributes and verify */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get,
                                                 test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (invalid_rule_id, p_attr_list_get, test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_ID);

    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_TYPE, sai_rc);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);
}

TEST_F(saiACLRuleTest, rule_with_cpu_queue_action)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             test_attr_count = 1;
    unsigned int             max_queues = 0;
    sai_object_id_t          cpu_port = 0;

    cpu_port = sai_test_acl_get_cpu_port ();

    sai_rc = sai_test_acl_rule_get_max_queues(cpu_port, &max_queues);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_object_id_t queue_id_list[max_queues];

    sai_rc = sai_test_acl_rule_get_queue_id_list(cpu_port, max_queues,
                                                 &queue_id_list[0]);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create ACL Rule with CPU Queue Action in wrong ACL table stage */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 8,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_egress_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, src_mac_mask,
                                       SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,
                                       1, &dst_mac_data, dst_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT,
                                       1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_DROP,
                                       SAI_ACL_ENTRY_ATTR_ACTION_SET_CPU_QUEUE,
                                       1, queue_id_list[0]);
    EXPECT_EQ (SAI_STATUS_NOT_SUPPORTED, sai_rc);

    /* Create ACL Rule with CPU Queue Action */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 7,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, src_mac_mask,
                                       SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,
                                       1, &dst_mac_data, dst_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_DROP,
                                       SAI_ACL_ENTRY_ATTR_ACTION_SET_CPU_QUEUE,
                                       1, queue_id_list[0]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    ASSERT_NE(0, acl_rule_id);

    if (max_queues > 2)
    {
        /* Set the CPU Queue to valid CPU Queue value */
        sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                        SAI_ACL_ENTRY_ATTR_ACTION_SET_CPU_QUEUE,
                                        1, queue_id_list[1]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);


        /* Fetch the default values using the GET API */
        sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
        ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

        sai_rc = sai_test_acl_rule_get (acl_rule_id, p_attr_list_get,
                                        test_attr_count,
                                        SAI_ACL_ENTRY_ATTR_ACTION_SET_CPU_QUEUE);

        EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.oid, queue_id_list[1]);

        /* Remove CPU Queue Action using Set  */
        sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                        SAI_ACL_ENTRY_ATTR_ACTION_SET_CPU_QUEUE,
                                        0, 0);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                        SAI_ACL_ENTRY_ATTR_ACTION_SET_CPU_QUEUE,
                                        1, queue_id_list[2]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

        sai_rc = sai_test_acl_rule_get (acl_rule_id, p_attr_list_get,
                                        test_attr_count,
                                        SAI_ACL_ENTRY_ATTR_ACTION_SET_CPU_QUEUE);

        EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.oid, queue_id_list[2]);

        sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);
    }

    sai_test_acl_rule_entry_remove (acl_rule_id);
}

TEST_F(saiACLRuleTest, rule_with_tc_action)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             test_attr_count = 1;
    unsigned int             traf_class = 10;

    /* Create ACL Rule with CPU Queue Action in wrong ACL table stage */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 8,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_egress_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, src_mac_mask,
                                       SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,
                                       1, &dst_mac_data, dst_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT,
                                       1, port_id_1,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_DROP,
                                       SAI_ACL_ENTRY_ATTR_ACTION_SET_TC,
                                       1, traf_class);
    EXPECT_EQ (SAI_STATUS_NOT_SUPPORTED, sai_rc);

    /* Create ACL Rule with Set TC Action Queue 5 */
    acl_rule_id = sai_test_acl_rule_entry_create (SAI_ACL_TABLE_TYPE_MAC);
    ASSERT_NE(0, acl_rule_id);

    /* Set the TC to a different value */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_SET_TC,
                                    1, traf_class);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the default values using the GET API */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (acl_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_ACTION_SET_TC);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.u8, 10);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);

    /* Remove TC Action using Set  */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_SET_TC,
                                    0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_acl_rule_entry_remove (acl_rule_id);
}

TEST_F(saiACLRuleTest, rule_with_dscp_and_ecn_action)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             test_attr_count = 1;

    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 8,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, src_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DSCP,
                                       1, 8, 0x3f,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ECN,
                                       1, 1, 0x3,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_LOG,
                                       SAI_ACL_ENTRY_ATTR_ACTION_SET_DSCP,
                                       1, 45);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Set the DSCP to a different value */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_DSCP,
                                    1, 10, 0x3f);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Set the ECN to a different value */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ECN,
                                    1, 3, 0x3);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the ECN field */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ECN,
                                    0, 0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the DSCP field */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_DSCP,
                                    0, 0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Set the ECN to a different value */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ECN,
                                    1, 2, 0x3);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the default values using the GET API */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (acl_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_ACTION_SET_DSCP);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.u8, 45);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);

    /* Remove DSCP Action using Set  */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_SET_DSCP,
                                    0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_acl_rule_entry_remove (acl_rule_id);
}

TEST_F(saiACLRuleTest, rule_with_redirect_lag_action)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_rule_id = 0, lag_id = 0;
    sai_object_id_t          port_arr[2];
    sai_object_list_t        lag_port_list;
    sai_attribute_t          attr;
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             test_attr_count = 1;

    lag_port_list.count = 2;
    port_arr[0] = port_id_1;
    port_arr[1] = port_id_2;
    lag_port_list.list = port_arr;

    attr.id = SAI_LAG_ATTR_PORT_LIST;
    attr.value.objlist = lag_port_list;

    sai_rc = sai_test_acl_rule_lag_create(&lag_id, &attr);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 8,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, src_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DSCP,
                                       1, 16, 0x3f,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ECN,
                                       1, 2, 0x3,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_LOG,
                                       SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                       1, lag_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the Redirect Lag Action */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                    0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Add the Redirect Lag Action */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                    1, lag_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the default values using the GET API */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (acl_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.oid, lag_id);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);

    sai_test_acl_rule_entry_remove (acl_rule_id);

    sai_rc = sai_test_acl_rule_lag_delete(lag_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLRuleTest, rule_with_redirect_nexthop_action)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          acl_rule_id = 0, vrf_id = 0;
    sai_object_id_t          port_rif_id = 0;
    unsigned int             default_rif_attr_count = 3;
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             test_attr_count = 1;
    sai_object_id_t          nexthop_group_id = 0;
    sai_object_id_t          p_nh_id_list[3] = {0};

    /* Create a Virtual Router instance */
    sai_rc = saiL3Test::sai_test_vrf_create (&vrf_id, 1,
                                  SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                  "00:b0:b1:b2:b3:b4:b5");
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create a Port RIF */
    sai_rc = saiL3Test::sai_test_rif_create (&port_rif_id, default_rif_attr_count,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vrf_id,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_PORT,
                                  SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                  port_id_2);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Setup the Nexthop Group */
    sai_rc = sai_test_acl_rule_setup_nexthop(&nexthop_group_id,
                                             p_nh_id_list, port_rif_id);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create the ACL Rule with Redirect to Nexthop Action */
    sai_rc = sai_test_acl_rule_create (&acl_rule_id, 8,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, src_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DSCP,
                                       1, 16, 0x3f,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ECN,
                                       1, 2, 0x3,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_LOG,
                                       SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                       1, p_nh_id_list[0]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the Redirect to Nexthop Action in the existing ACL Rule */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                    0, p_nh_id_list[0]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Adding new nexthop action to existing ACL Rule */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                    1, p_nh_id_list[1]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the default values using the GET API */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (acl_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.oid, p_nh_id_list[1]);

    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                    0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (acl_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, false);

    /* Adding Redirect to Nexthop group Action in the existing ACL Rule */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                    1, nexthop_group_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (acl_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.oid, nexthop_group_id);

    /* Remove the Redirect to Nexthop group Action */
    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                    0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);

    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                    1, p_nh_id_list[2]+1);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    sai_rc = sai_test_acl_rule_set (acl_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT,
                                    1, nexthop_group_id+1);
    EXPECT_EQ (SAI_STATUS_INVALID_OBJECT_ID, sai_rc);

    /* Clean Up */
    sai_test_acl_rule_entry_remove (acl_rule_id);

    sai_rc = sai_test_acl_rule_cleanup_nexthop(nexthop_group_id,
                                               p_nh_id_list,
                                               port_rif_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the Port RIF */
    sai_rc = saiL3Test::sai_test_rif_remove (port_rif_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Remove the VRF */
    sai_rc = saiL3Test::sai_test_vrf_remove (vrf_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F(saiACLRuleTest, rule_with_icmp_field)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          rule_id = 0;
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             test_attr_count = 1;

    rule_id = sai_test_acl_rule_entry_create (SAI_ACL_TABLE_TYPE_IP);

    ASSERT_NE(0, rule_id);

    /* Fetch the default values using the GET API */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get, test_attr_count, 0, 0, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.u8, 0x08);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.u8, 4);

    /* Remove the ICMP Type */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE,
                                    0, 0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, false);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.u8, 4);

    /* Set the ICMP Type */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE,
                                    1, 0x0b, 0xff);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.u8, 0x0b);

    /* Remove the ICMP Code */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE,
                                    0, 0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, false);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.u8, 0x0b);

    /* Set the ICMP Code */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE,
                                    1, 0x11, 0xff);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.u8, 0x11);

    /* Remove both ICMP Type and Code */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE,
                                    0, 0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE,
                                    0, 0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Set them to a new value */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE,
                                    1, 0xaa, 0xff);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE,
                                    1, 0xcc, 0xff);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.u8, 0xaa);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE);

    EXPECT_EQ (p_attr_list_get[0].value.aclfield.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclfield.data.u8, 0xcc);

    sai_test_acl_rule_entry_remove (rule_id);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);
}

TEST_F(saiACLRuleTest, rule_with_multiple_mirror_action)
{
    sai_status_t             sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t          mirror_rule_id = 0, rule_id = 0;
    sai_object_id_t          mirror_id[SAI_TEST_MAX_MIRROR_SESSIONS] = {0};
    sai_attribute_t          attr[2] = {0};
    sai_attribute_t         *p_attr_list_get = NULL;
    unsigned int             test_attr_count = 1, mirror_session = 0;

    /* Create Multiple Mirror Sessions */
    attr[0].id =  SAI_MIRROR_SESSION_ATTR_MONITOR_PORT;
    attr[0].value.oid = port_id_1;
    attr[1].id =  SAI_MIRROR_SESSION_ATTR_TYPE;
    attr[1].value.s32 = SAI_MIRROR_TYPE_LOCAL;

    sai_rc = sai_test_acl_rule_mirror_session_create(&mirror_id[0], 2, attr);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr[0].id =  SAI_MIRROR_SESSION_ATTR_MONITOR_PORT;
    attr[0].value.oid = port_id_2;
    attr[1].id =  SAI_MIRROR_SESSION_ATTR_TYPE;
    attr[1].value.s32 = SAI_MIRROR_TYPE_LOCAL;

    sai_rc = sai_test_acl_rule_mirror_session_create(&mirror_id[1], 2, attr);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr[0].id =  SAI_MIRROR_SESSION_ATTR_MONITOR_PORT;
    attr[0].value.oid = port_id_3;
    attr[1].id =  SAI_MIRROR_SESSION_ATTR_TYPE;
    attr[1].value.s32 = SAI_MIRROR_TYPE_LOCAL;

    sai_rc = sai_test_acl_rule_mirror_session_create(&mirror_id[2], 2, attr);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    attr[0].id =  SAI_MIRROR_SESSION_ATTR_MONITOR_PORT;
    attr[0].value.oid = port_id_4;
    attr[1].id =  SAI_MIRROR_SESSION_ATTR_TYPE;
    attr[1].value.s32 = SAI_MIRROR_TYPE_LOCAL;

    sai_rc = sai_test_acl_rule_mirror_session_create(&mirror_id[3], 2, attr);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create ACL Rule with Mirror Session List */
    sai_rc = sai_test_acl_rule_create (&mirror_rule_id, 8,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, mac_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 1,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC,
                                       1, &src_mac_data, src_mac_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_DSCP,
                                       1, 16, 0x3f,
                                       SAI_ACL_ENTRY_ATTR_FIELD_ECN,
                                       1, 2, 0x3,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_LOG,
                                       SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS,
                                       true, 2, mirror_id[0], mirror_id[1]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Create ACL Rule without Mirror List */
    sai_rc = sai_test_acl_rule_create (&rule_id, 6,
                                       SAI_ACL_ENTRY_ATTR_TABLE_ID, ip_table_id,
                                       SAI_ACL_ENTRY_ATTR_PRIORITY, 5,
                                       SAI_ACL_ENTRY_ATTR_ADMIN_STATE, true,
                                       SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP,
                                       1, &src_ip_data, &src_ip_mask,
                                       SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT,
                                       1, 25, standard_mask,
                                       SAI_ACL_ENTRY_ATTR_PACKET_ACTION, true,
                                       SAI_PACKET_ACTION_TRAP);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /* Fetch the Mirror List using the GET API but with smaller mirror count */
    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get,
                                test_attr_count, 1 /* Lesser than actual mirror count */, 1, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (mirror_rule_id, p_attr_list_get,
                                    test_attr_count,
                                    SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS);
    EXPECT_EQ (SAI_STATUS_BUFFER_OVERFLOW, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.count, 2);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);

    /* Add new Mirror Sessions to the rule not containing mirror sessions */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS,
                                    true, 1, mirror_id[2]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_create_attr_list (&p_attr_list_get,
                                test_attr_count, 4, 1, false);
    ASSERT_TRUE (SAI_STATUS_SUCCESS == sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.count, 1);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.list[0], mirror_id[2]);

    /* Modify the mirror session list */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS,
                                    true, 2, mirror_id[2], mirror_id[3]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    p_attr_list_get[0].value.aclaction.parameter.objlist.count = 2;

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.count, 2);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.list[0], mirror_id[2]);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.list[1], mirror_id[3]);

    /* Modify the mirror session list with same count */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS,
                                    true, 2, mirror_id[0], mirror_id[1]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.count, 2);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.list[0], mirror_id[0]);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.list[1], mirror_id[1]);

    /* Remove the Mirror Session list and add it again */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS, 0, 0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, false);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.count, 0);

    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS, 1,
                                    1, mirror_id[3]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    p_attr_list_get[0].value.aclaction.parameter.objlist.count = 1;

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.count, 1);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.list[0], mirror_id[3]);

    /* Add the same session to Mirror Ingress */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS, 1,
                                    1, mirror_id[3]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    p_attr_list_get[0].value.aclaction.parameter.objlist.count = 1;

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.count, 1);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.list[0], mirror_id[3]);

    /* Remove the Mirror Session list and add it to different rule */
    sai_rc = sai_test_acl_rule_set (rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS, 0, 0, 0);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = sai_test_acl_rule_get (rule_id, p_attr_list_get,
                                     test_attr_count, SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, false);

    sai_rc = sai_test_acl_rule_set (mirror_rule_id, 1,
                                    SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS, 1, 4,
                                    mirror_id[0], mirror_id[1], mirror_id[2], mirror_id[3]);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    p_attr_list_get[0].value.aclaction.parameter.objlist.count = 4;

    sai_rc = sai_test_acl_rule_get (mirror_rule_id, p_attr_list_get,
                                    test_attr_count, SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    EXPECT_EQ (p_attr_list_get[0].value.aclaction.enable, true);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.count, 4);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.list[0], mirror_id[0]);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.list[1], mirror_id[1]);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.list[2], mirror_id[2]);
    EXPECT_EQ (p_attr_list_get[0].value.aclaction.parameter.objlist.list[3], mirror_id[3]);

    sai_test_acl_rule_free_attr_list (p_attr_list_get, test_attr_count);
    sai_test_acl_rule_entry_remove (mirror_rule_id);
    sai_test_acl_rule_entry_remove (rule_id);

    for (mirror_session = 0; mirror_session < SAI_TEST_MAX_MIRROR_SESSIONS; mirror_session++)
    {
        sai_test_acl_rule_mirror_session_destroy (mirror_id[mirror_session]);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    }
}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

