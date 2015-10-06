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
 * * @file sai_acl_unit_test_utils.cpp
 * *
 * * @brief This file contains utility and helper function definitions for
 * *        testing the SAI ACL functionality
 * *
  *************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stdarg.h"
#include "gtest/gtest.h"
#include "inttypes.h"

#include "sai_acl_unit_test_utils.h"

extern "C" {
#include "sai.h"
#include "saitypes.h"
#include "saistatus.h"
#include "saiswitch.h"
#include <inttypes.h>
}

#define SAI_MAX_PORTS  256

typedef enum _sai_test_acl_rule_attr_type {
    SAI_TEST_ACL_ENTRY_ATTR_ONE_BYTE,
    SAI_TEST_ACL_ENTRY_ATTR_TWO_BYTES,
    SAI_TEST_ACL_ENTRY_ATTR_FOUR_BYTES,
    SAI_TEST_ACL_ENTRY_ATTR_ENUM,
    SAI_TEST_ACL_ENTRY_ATTR_MAC,
    SAI_TEST_ACL_ENTRY_ATTR_IPv4,
    SAI_TEST_ACL_ENTRY_ATTR_IPv6,
    SAI_TEST_ACL_ENTRY_ATTR_OBJECT_ID,
    SAI_TEST_ACL_ENTRY_ATTR_OBJECT_LIST,
    SAI_TEST_ACL_ENTRY_ATTR_INVALID
} sai_test_acl_rule_attr_type;

static uint32_t port_count = 0;
static sai_object_id_t port_list[SAI_MAX_PORTS] = {0};

sai_object_id_t saiACLTest ::sai_acl_port_id_get (uint32_t port_index)
{
    if(port_index >= port_count) {
        return 0;
    }

    return port_list [port_index];
}

sai_object_id_t saiACLTest ::sai_acl_invalid_port_id_get ()
{
    return (port_list[port_count-1] + 1);
}

bool saiACLTest ::sai_test_acl_table_field_range(sai_attr_id_t id)
{
    if ((id >= SAI_ACL_TABLE_ATTR_FIELD_START) &&
        (id <= SAI_ACL_TABLE_ATTR_FIELD_END)) {
        return true;
    }
    return false;
}

bool saiACLTest ::sai_test_acl_rule_field_range(sai_attr_id_t id)
{
    if ((id >= SAI_ACL_ENTRY_ATTR_FIELD_START) &&
        (id <= SAI_ACL_ENTRY_ATTR_FIELD_END)) {
        return true;
    }
    return false;
}

bool saiACLTest ::sai_test_acl_rule_action_range(sai_attr_id_t id)
{
    if ((id >= SAI_ACL_ENTRY_ATTR_ACTION_START) &&
        (id <= SAI_ACL_ENTRY_ATTR_ACTION_END)) {
        return true;
    }
    return false;
}

static inline void sai_port_state_evt_callback (uint32_t count,
                                                sai_port_oper_status_notification_t *data)
{
}

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

static inline void sai_fdb_evt_callback(uint32_t count, sai_fdb_event_notification_data_t *data)
{
}

static inline void sai_switch_operstate_callback (sai_switch_oper_status_t
                                                  switchstate)
{
}

/* Packet event callback
 */
static inline void sai_packet_event_callback (const void *buffer,
                                              sai_size_t buffer_size,
                                              uint32_t attr_count,
                                              const sai_attribute_t *attr_list)
{
}

static inline void  sai_switch_shutdown_callback (void)
{
}

/* SAI initialization */
void saiACLTest ::SetUpTestCase (void)
{
    sai_switch_notification_t notification;
    memset (&notification, 0, sizeof(sai_switch_notification_t));

    /*
     * Query and populate the SAI Switch API Method Table.
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

    /*
     * Query and populate the SAI ACL API Table
     */
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query (SAI_API_ACL,
              (static_cast<void**>(static_cast<void*>(&p_sai_acl_api_tbl)))));

    ASSERT_TRUE (p_sai_acl_api_tbl != NULL);

    /*
     * Query and populate the SAI LAG API Table.
     */
    ASSERT_EQ(NULL,sai_api_query(SAI_API_LAG,
             (static_cast<void**>(static_cast<void*>(&p_sai_lag_api_tbl)))));

    ASSERT_TRUE(p_sai_lag_api_tbl != NULL);

    /*
     * Query and populate the SAI Mirror API Table.
     */
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_MIRROR, (static_cast<void**>
                                         (static_cast<void*>
                                          (&p_sai_mirror_api_tbl)))));

    ASSERT_TRUE (p_sai_mirror_api_tbl != NULL);

    /*
     * Query and populate the SAI Port API Table.
     */
    ASSERT_EQ (NULL, sai_api_query(SAI_API_PORT,
                (static_cast<void**>(static_cast<void*>(&p_sai_port_api_tbl)))));

    ASSERT_TRUE (p_sai_port_api_tbl != NULL);

}

sai_switch_api_t* saiACLTest ::p_sai_switch_api_tbl = NULL;
sai_acl_api_t* saiACLTest ::p_sai_acl_api_tbl = NULL;
sai_lag_api_t* saiACLTest ::p_sai_lag_api_tbl = NULL;
sai_mirror_api_t* saiACLTest ::p_sai_mirror_api_tbl = NULL;
sai_port_api_t* saiACLTest ::p_sai_port_api_tbl = NULL;

sai_object_id_t saiACLTest ::sai_test_acl_get_cpu_port ()
{
    sai_attribute_t sai_attr_get;
    sai_status_t    sai_rc = SAI_STATUS_SUCCESS;

    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
    sai_attr_get.id = SAI_SWITCH_ATTR_CPU_PORT;

    sai_rc = p_sai_switch_api_tbl->get_switch_attribute(1, &sai_attr_get);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    return (sai_attr_get.value.oid);
}

static const char* sai_acl_test_table_attr_id_to_name_get (unsigned int attr_id) {
    if (SAI_ACL_TABLE_ATTR_STAGE == attr_id) {
        return "ACL Table Stage";
    } else if (SAI_ACL_TABLE_ATTR_PRIORITY == attr_id) {
        return "ACL Table Priority";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_SRC_IPv6 == attr_id) {
        return "SRC IPv6 Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_DST_IPv6 == attr_id) {
        return "DST IPv6 Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC == attr_id) {
        return "SRC Mac Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_DST_MAC == attr_id) {
        return "DST Mac Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_SRC_IP == attr_id) {
        return "SRC IP Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_DST_IP == attr_id) {
        return "DST IP Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS == attr_id) {
        return "In Ports Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_OUT_PORTS == attr_id) {
        return "Out Ports Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_IN_PORT == attr_id) {
        return "In Port Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_OUT_PORT == attr_id) {
        return "Out Port Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_ID == attr_id) {
        return "Outer Vlan ID Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_PRI == attr_id) {
        return "Outer Vlan Priority Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_CFI == attr_id) {
        return "Outer Vlan CFI Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_ID == attr_id) {
        return "Inner Vlan ID Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_PRI == attr_id) {
        return "Inner Vlan Priority Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_CFI == attr_id) {
        return "Inner Vlan CFI Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT == attr_id) {
        return "L4 SRC Port Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT == attr_id) {
        return "L4 DST Port Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE == attr_id) {
        return "Ethertype Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL == attr_id) {
        return "IP Protocol Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_DSCP == attr_id) {
        return "DSCP Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_ECN == attr_id) {
        return "ECN";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_TTL == attr_id) {
        return "TTL Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_TOS == attr_id) {
        return "TOS Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_IP_FLAGS == attr_id) {
        return "IP Flags Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_TCP_FLAGS == attr_id) {
        return "TCP Flags Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_IP_TYPE == attr_id) {
        return "IP Type Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_IP_FRAG == attr_id) {
        return "IP Frag Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_IPv6_FLOW_LABEL == attr_id) {
        return "IPv6 Flow Label";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_TC == attr_id) {
        return "TC Field";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_ICMP_TYPE == attr_id) {
        return "ICMPType";
    } else if (SAI_ACL_TABLE_ATTR_FIELD_ICMP_CODE == attr_id) {
        return "ICMPCode";
    } else {
        return "INVALID/UNKNOWN";
    }
}

sai_status_t saiACLTest ::sai_test_acl_table_priority_get (
                                        sai_attribute_t *p_attr_list,
                                        unsigned int attr_count, ...)
{
    va_list          ap;
    unsigned int     ap_idx = 0;
    unsigned int     attr_id = 0;

    va_start (ap, attr_count);

    if (!p_attr_list) {
        return SAI_STATUS_FAILURE;
    }

    printf ("Testing Switch Attrbiute to get ACL Table Priority\r\n");

    for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
        attr_id = va_arg (ap, unsigned int);
        p_attr_list [ap_idx].id = attr_id;
    }

    EXPECT_EQ(SAI_STATUS_SUCCESS, p_sai_switch_api_tbl->
                            get_switch_attribute(attr_count, p_attr_list));

    return SAI_STATUS_SUCCESS;
}

/*
 * acl_table_id  - [out] pointer to ACL Table ID generated by the SAI API.
 * attr_count  - [in]  number of ACL Table attributes passed.
 *               For each attribute, {id, value} is passed.
 *
 * For attr-count = 2,
 * sai_test_acl_table_create (acl_table_id, 2, id_0, val_0, id_1, val_1)
 */
sai_status_t saiACLTest ::sai_test_acl_table_create (
                                        sai_object_id_t *acl_table_id,
                                        unsigned int attr_count, ...)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    va_list          ap;
    unsigned int     ap_idx = 0;
    unsigned int     attr_id = 0;
    sai_attribute_t *p_attr_list = NULL;

    va_start (ap, attr_count);

    p_attr_list
     = (sai_attribute_t *) calloc (attr_count, sizeof (sai_attribute_t));

    printf ("Testing ACL Table Create API with attribute count: %d\r\n",
            attr_count);

    if (!p_attr_list) {
        printf ("Failed to allocate memory for attribute list.\r\n");

        return SAI_STATUS_FAILURE;
    }

    for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
        attr_id = va_arg (ap, unsigned int);
        p_attr_list [ap_idx].id = attr_id;

        printf ("Setting List index: %d with Attribute %s (ID = %d)\r\n",
                ap_idx, sai_acl_test_table_attr_id_to_name_get(attr_id),
                attr_id);

        switch (attr_id) {
            case SAI_ACL_TABLE_ATTR_STAGE:
                p_attr_list[ap_idx].value.s32 = va_arg (ap, unsigned int);
                printf ("ACL Table Stage Value: %d\r\n",
                        p_attr_list [ap_idx].value.s32);
                break;
            case SAI_ACL_TABLE_ATTR_PRIORITY:
                p_attr_list[ap_idx].value.u32 = va_arg (ap, unsigned int);
                printf ("ACL Table Priority Value: %d\r\n",
                        p_attr_list [ap_idx].value.u32);
                break;
            default:
                if (!(sai_test_acl_table_field_range(attr_id))) {
                    printf ("ACL Table Unknown attribute:\r\n");
                }
                break;
        }
    }

    sai_rc = p_sai_acl_api_tbl->create_acl_table (acl_table_id, attr_count,
                                                  p_attr_list);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("ACL Table Creation API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("ACL Table Creation API success, ACL Table ID: 0x%"PRIx64"\r\n",
                *acl_table_id);
    }

    if (p_attr_list) {
        free (p_attr_list);
    }

    return sai_rc;
}

/*
 * acl_table_id  - [in] ACL Table ID which needs to be removed.
 *
 * sai_test_acl_table_remove (acl_table_id)
 */
sai_status_t saiACLTest ::sai_test_acl_table_remove (
                                        sai_object_id_t acl_table_id)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;

    sai_rc = p_sai_acl_api_tbl->delete_acl_table (acl_table_id);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("ACL Table removal API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("ACL Table removal API success for ACL Table ID: 0x%"PRIx64"\r\n", acl_table_id);
    }

    return sai_rc;
}

/*
 * acl_table_id  - [in] ACL Table ID to modify the ACL Table
 * attr_count  - [in]  number of ACL Table attributes passed.
 *               For each attribute, {id, value} is passed.
 *
 * For attr-count = 2,
 * sai_test_acl_table_set (acl_table_id, 2, id_0, val_0, id_1, val_1)
 */
sai_status_t saiACLTest ::sai_test_acl_table_set (
                                        sai_object_id_t acl_table_id,
                                        unsigned int attr_count, ...)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    va_list          ap;
    unsigned int     ap_idx = 0;
    unsigned int     attr_id = 0;
    sai_attribute_t *p_attr_list = NULL;

    va_start (ap, attr_count);

    p_attr_list
     = (sai_attribute_t *) calloc (attr_count, sizeof (sai_attribute_t));

    printf ("Testing ACL Table Set API with attribute count: %d\r\n",
            attr_count);

    if (!p_attr_list) {
        printf ("Failed to allocate memory for attribute list.\r\n");

        return SAI_STATUS_FAILURE;
    }

    for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
        attr_id = va_arg (ap, unsigned int);
        p_attr_list [ap_idx].id = attr_id;

        printf ("Setting List index: %d with Attribute %s (ID = %d)\r\n",
                ap_idx, sai_acl_test_table_attr_id_to_name_get(attr_id),
                attr_id);

        switch (attr_id) {
            case SAI_ACL_TABLE_ATTR_STAGE:
                p_attr_list[ap_idx].value.s32 = va_arg (ap, unsigned int);
                printf ("ACL Table Stage Value: %d\r\n",
                        p_attr_list [ap_idx].value.s32);
                break;
            case SAI_ACL_TABLE_ATTR_PRIORITY:
                p_attr_list[ap_idx].value.u32 = va_arg (ap, unsigned int);
                printf ("ACL Table Priority Value: %d\r\n",
                        p_attr_list [ap_idx].value.u32);
                break;
            default:
                if (!(sai_test_acl_table_field_range(attr_id))) {
                    printf ("ACL Table Unknown attribute:\r\n");
                }
                break;
        }
    }

    sai_rc = p_sai_acl_api_tbl->set_acl_table_attribute (acl_table_id,
                                                         p_attr_list);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("ACL Table Set API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("ACL Table Set API success, ACL Table ID: 0x%"PRIx64"\r\n",
                acl_table_id);
    }

    if (p_attr_list) {
        free (p_attr_list);
    }

    return sai_rc;
}

/*
 * acl_table_id  - [in] ACL Table ID to fetch the ACL Table attr using Get
 * attr_count  - [in]  number of ACL Table attributes passed.
 *               For each attribute, {id, value} is passed.
 *
 * For attr-count = 2,
 * sai_test_acl_table_get (acl_table_id, 2, id_0, val_0, id_1, val_1)
 */
sai_status_t saiACLTest ::sai_test_acl_table_get (
                                        sai_object_id_t acl_table_id,
                                        unsigned int attr_count, ...)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    va_list          ap;
    unsigned int     ap_idx = 0;
    unsigned int     attr_id = 0;
    sai_attribute_t *p_attr_list = NULL;

    va_start (ap, attr_count);

    p_attr_list
     = (sai_attribute_t *) calloc (attr_count, sizeof (sai_attribute_t));

    printf ("Testing ACL Table Get API with attribute count: %d\r\n",
            attr_count);

    if (!p_attr_list) {
        printf ("Failed to allocate memory for attribute list.\r\n");

        return SAI_STATUS_FAILURE;
    }

    for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
        attr_id = va_arg (ap, unsigned int);
        p_attr_list [ap_idx].id = attr_id;

        printf ("Setting List index: %d with Attribute %s (ID = %d)\r\n",
                ap_idx, sai_acl_test_table_attr_id_to_name_get(attr_id),
                attr_id);

    }

    sai_rc = p_sai_acl_api_tbl->get_acl_table_attribute (acl_table_id, attr_count,
                                                         p_attr_list);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("ACL Table Get API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("ACL Table Get API success, ACL Table ID: 0x%"PRIx64"\r\n",
                acl_table_id);
        for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
            switch (p_attr_list [ap_idx].id) {
            case SAI_ACL_TABLE_ATTR_STAGE:
                printf ("ACL Table Get Stage Value: %d\r\n",
                        p_attr_list [ap_idx].value.s32);
                break;
            case SAI_ACL_TABLE_ATTR_PRIORITY:
                printf ("ACL Table Get Priority Value: %d\r\n",
                        p_attr_list [ap_idx].value.u32);
                break;
            default:
                printf ("ACL Table Nothing to Get \r\n");
                break;
            }
        }
    }

    if (p_attr_list) {
        free (p_attr_list);
    }

    return sai_rc;
}

static const char* sai_acl_test_rule_attr_id_to_name_get (unsigned int attr_id) {
    if (SAI_ACL_ENTRY_ATTR_TABLE_ID == attr_id) {
        return "ACL Rule Table ID";
    } else if (SAI_ACL_ENTRY_ATTR_PRIORITY == attr_id) {
        return "ACL Rule Priority";
    } else if (SAI_ACL_ENTRY_ATTR_ADMIN_STATE == attr_id) {
        return "ACL Rule Admin State";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_SRC_IPv6 == attr_id) {
        return "SRC IPv6 Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_DST_IPv6 == attr_id) {
        return "DST IPv6 Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC == attr_id) {
        return "SRC Mac Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_DST_MAC == attr_id) {
        return "DST Mac Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP == attr_id) {
        return "SRC IP Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_DST_IP == attr_id) {
        return "DST IP Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS == attr_id) {
        return "In Ports Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORTS == attr_id) {
        return "Out Ports Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT == attr_id) {
        return "In Port Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT == attr_id) {
        return "Out Port Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_ID == attr_id) {
        return "Outer Vlan ID Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_PRI == attr_id) {
        return "Outer Vlan Priority Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_CFI == attr_id) {
        return "Outer Vlan CFI Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_ID == attr_id) {
        return "Inner Vlan ID Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_PRI == attr_id) {
        return "Inner Vlan Priority Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_CFI == attr_id) {
        return "Inner Vlan CFI Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT == attr_id) {
        return "L4 SRC Port Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT == attr_id) {
        return "L4 DST Port Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE == attr_id) {
        return "Ethertype Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL == attr_id) {
        return "IP Protocol Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_DSCP == attr_id) {
        return "DSCP Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_ECN == attr_id) {
        return "ECN";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_TTL == attr_id) {
        return "TTL Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_TOS == attr_id) {
        return "TOS Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_IP_FLAGS == attr_id) {
        return "IP Flags Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_TCP_FLAGS == attr_id) {
        return "TCP Flags Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_IP_TYPE == attr_id) {
        return "IP Type Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_IP_FRAG == attr_id) {
        return "IP Frag Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_IPv6_FLOW_LABEL == attr_id) {
        return "IPv6 Flow Label";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_TC == attr_id) {
        return "TC Field";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE == attr_id) {
        return "ICMPType";
    } else if (SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE == attr_id) {
        return "ICMPCode";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT == attr_id) {
        return "Redirect Action";
    } else if (SAI_ACL_ENTRY_ATTR_PACKET_ACTION == attr_id) {
        return "Packet Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_FLOOD == attr_id) {
        return "Flood Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_COUNTER == attr_id) {
        return "Counter Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS == attr_id) {
        return "Mirror Ingress Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS == attr_id) {
        return "Mirror Egress Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_POLICER == attr_id) {
        return "Policer Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_DECREMENT_TTL == attr_id) {
        return "Decrement TTL Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_TC == attr_id) {
        return "Set TC Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_INNER_VLAN_ID == attr_id) {
        return "Set Inner Vlan ID Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_INNER_VLAN_PRI == attr_id) {
        return "Set Inner Vlan Pri Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_OUTER_VLAN_ID == attr_id) {
        return "Set Outer Vlan ID Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_OUTER_VLAN_PRI == attr_id) {
        return "Set Outer Vlan Pri Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_MAC == attr_id) {
        return "Set SRC MAC Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_MAC == attr_id) {
        return "Set DST MAC Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_IP == attr_id) {
        return "Set SRC IP Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_IP == attr_id) {
        return "Set DST IP Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_IPv6 == attr_id) {
        return "Set SRC IPv6 Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_IPv6 == attr_id) {
        return "Set DST IPv6 Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_DSCP == attr_id) {
        return "Set DSCP Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_L4_SRC_PORT == attr_id) {
        return "Set L4 SRC Port Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_L4_DST_PORT == attr_id) {
        return "Set L4 DST Port Action";
    } else if (SAI_ACL_ENTRY_ATTR_ACTION_SET_CPU_QUEUE == attr_id) {
        return "Set CPU Queue Action";
    } else {
        return "INVALID/UNKNOWN";
    }
}

static sai_test_acl_rule_attr_type sai_test_acl_rule_get_attr_type (unsigned int attr_id)
{
    switch (attr_id) {
        case SAI_ACL_ENTRY_ATTR_FIELD_SRC_IPv6:
        case SAI_ACL_ENTRY_ATTR_FIELD_DST_IPv6:
            return SAI_TEST_ACL_ENTRY_ATTR_IPv6;
        case SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP:
        case SAI_ACL_ENTRY_ATTR_FIELD_DST_IP:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_IP:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_IP:
            return SAI_TEST_ACL_ENTRY_ATTR_IPv4;
        case SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC:
        case SAI_ACL_ENTRY_ATTR_FIELD_DST_MAC:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_MAC:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_MAC:
            return SAI_TEST_ACL_ENTRY_ATTR_MAC;
        case SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_PRI:
        case SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_CFI:
        case SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_PRI:
        case SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_CFI:
        case SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL:
        case SAI_ACL_ENTRY_ATTR_FIELD_DSCP:
        case SAI_ACL_ENTRY_ATTR_FIELD_ECN:
        case SAI_ACL_ENTRY_ATTR_FIELD_TTL:
        case SAI_ACL_ENTRY_ATTR_FIELD_TOS:
        case SAI_ACL_ENTRY_ATTR_FIELD_IP_FLAGS:
        case SAI_ACL_ENTRY_ATTR_FIELD_TCP_FLAGS:
        case SAI_ACL_ENTRY_ATTR_FIELD_TC:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_TC:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_INNER_VLAN_PRI:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_OUTER_VLAN_PRI:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_DSCP:
        case SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE:
        case SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE:
            return SAI_TEST_ACL_ENTRY_ATTR_ONE_BYTE;
        case SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_ID:
        case SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_ID:
        case SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT:
        case SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT:
        case SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_INNER_VLAN_ID:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_OUTER_VLAN_ID:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_L4_SRC_PORT:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_L4_DST_PORT:
            return SAI_TEST_ACL_ENTRY_ATTR_TWO_BYTES;
        case SAI_ACL_ENTRY_ATTR_FIELD_IPv6_FLOW_LABEL:
            return SAI_TEST_ACL_ENTRY_ATTR_FOUR_BYTES;
        case SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT:
        case SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT:
        case SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT:
        case SAI_ACL_ENTRY_ATTR_ACTION_COUNTER:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_POLICER:
        case SAI_ACL_ENTRY_ATTR_ACTION_INGRESS_SAMPLEPACKET_ENABLE:
        case SAI_ACL_ENTRY_ATTR_ACTION_EGRESS_SAMPLEPACKET_ENABLE:
        case SAI_ACL_ENTRY_ATTR_ACTION_SET_CPU_QUEUE:
            return SAI_TEST_ACL_ENTRY_ATTR_OBJECT_ID;
        case SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS:
        case SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORTS:
        case SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS:
        case SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS:
            return SAI_TEST_ACL_ENTRY_ATTR_OBJECT_LIST;
        case SAI_ACL_ENTRY_ATTR_FIELD_IP_TYPE:
        case SAI_ACL_ENTRY_ATTR_FIELD_IP_FRAG:
        case SAI_ACL_ENTRY_ATTR_PACKET_ACTION:
            return SAI_TEST_ACL_ENTRY_ATTR_ENUM;
        default:
            return SAI_TEST_ACL_ENTRY_ATTR_INVALID;
    }
}

static void sai_test_acl_rule_attr_value_fill(sai_attribute_t *p_attr,
                                              unsigned long attr_val)
{
    switch (p_attr->id) {
        case SAI_ACL_ENTRY_ATTR_TABLE_ID:
            p_attr->value.oid = attr_val;
            printf ("ACL Rule Table ID Value: 0x%"PRIx64"\r\n", p_attr->value.oid);
            break;
        case SAI_ACL_ENTRY_ATTR_PRIORITY:
            p_attr->value.u32 = attr_val;
            printf ("ACL Rule Priority Value: %d\r\n", p_attr->value.u32);
            break;
        case SAI_ACL_ENTRY_ATTR_ADMIN_STATE:
            p_attr->value.booldata = attr_val;
            printf ("ACL Rule Admin State Value: %d\r\n", p_attr->value.booldata);
            break;
        default:
            break;
    }
}

sai_status_t saiACLTest ::sai_test_acl_rule_get_max_queues(
sai_object_id_t port_id, unsigned int *queue_count)
{
    sai_status_t sai_rc;
    sai_attribute_t    attr  = {0};

    attr.id = SAI_PORT_ATTR_QOS_NUMBER_OF_QUEUES;

    sai_rc = p_sai_port_api_tbl->get_port_attribute (port_id, 1, &attr);
    if(sai_rc != SAI_STATUS_SUCCESS) {
        printf("Failed to get queue count. Error code:%d\n", sai_rc);
    } else {
         *queue_count = attr.value.u32;
         printf("Max queues on port 0x%"PRIx64": %d. \n", port_id, *queue_count);
    }
    return sai_rc;
}

sai_status_t saiACLTest ::sai_test_acl_rule_get_queue_id_list(
sai_object_id_t port_id, unsigned int queue_count,
sai_object_id_t *p_queue_id_list)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    sai_attribute_t    attr  = {0};
    unsigned int       index = 0;

    attr.id = SAI_PORT_ATTR_QOS_QUEUE_LIST;

    attr.value.objlist.count = queue_count;
    attr.value.objlist.list = p_queue_id_list;

    if (attr.value.objlist.list == NULL) {

        printf ("%s(): Memory alloc failed for attribute list.\n", __FUNCTION__);

        return SAI_STATUS_NO_MEMORY;
    }

    sai_rc = p_sai_port_api_tbl->get_port_attribute (port_id, 1, &attr);

    if (sai_rc == SAI_STATUS_BUFFER_OVERFLOW) {
        printf("Requested queue count %d, Max queues %d on port :0x%"PRIx64".\n",
               queue_count, attr.value.objlist.count, port_id);
        return SAI_STATUS_FAILURE;
    }

    if (sai_rc != SAI_STATUS_SUCCESS) {

        printf ("SAI Port Get queue id list failed with error: %d.\n", sai_rc);

    } else {

        printf ("SAI port Get queue id list success for Port Id: 0x%"PRIx64".\n",
                port_id);

        printf ("SAI Port 0x%"PRIx64" Queue List.\n", port_id);

        for (index = 0; index < attr.value.objlist.count; ++index) {
            printf ("SAI Queue index %d QOID 0x%"PRIx64".\n",
                    index , p_queue_id_list[index]);
        }
    }

    return sai_rc;
}

sai_status_t saiACLTest ::sai_test_acl_rule_mirror_session_create(
sai_object_id_t *p_mirror_session_id, unsigned int attr_count,
sai_attribute_t *p_attr_list)
{
    sai_status_t        sai_rc = SAI_STATUS_SUCCESS;

    printf ("Testing Mirror Session Create API with attribute count: %d\r\n", attr_count);

    sai_rc = p_sai_mirror_api_tbl->create_mirror_session (p_mirror_session_id,attr_count,
                                                          p_attr_list);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("SAI Mirror session Creation API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("SAI Mirror session Creation API success, session ID: 0x%"PRIx64"\r\n", *p_mirror_session_id);
    }

    return sai_rc;
}

sai_status_t saiACLTest ::sai_test_acl_rule_mirror_session_destroy (
sai_object_id_t mirror_session_id)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;

    sai_rc = p_sai_mirror_api_tbl->remove_mirror_session (mirror_session_id);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("Mirror Session destroy API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("Mirror Session API success for session id: 0x%"PRIx64"\r\n", mirror_session_id);
    }

    return sai_rc;
}

sai_status_t saiACLTest ::sai_test_acl_rule_lag_create(
sai_object_id_t *lag_id, sai_attribute_t *attr)
{
    sai_status_t        sai_rc = SAI_STATUS_SUCCESS;

    sai_rc = p_sai_lag_api_tbl->create_lag(lag_id, 1, attr);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("LAG Creation API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("LAG Creation API success, LAG ID: 0x%"PRIx64"\r\n",
                *lag_id);
    }

    return sai_rc;
}

sai_status_t saiACLTest ::sai_test_acl_rule_lag_delete(
sai_object_id_t lag_id)
{
    sai_status_t        sai_rc = SAI_STATUS_SUCCESS;

    sai_rc = p_sai_lag_api_tbl->remove_lag(lag_id);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("LAG Deletion API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("LAG Deletion API success, LAG ID: 0x%"PRIx64"\r\n",
                lag_id);
    }

    return sai_rc;
}

/*
 * acl_rule_id    - [out] pointer to ACL Rule ID generated by the API.
 * attr_count  - [in] number of ACL Rule attributes passed.
 *               For each attribute, {id, value} is passed.
 *
 * For attr-count = 2,
 * sai_test_acl_rule_create (acl_rule_id, 2, id_0, val_0, id_1, val_1)
 */
sai_status_t saiACLTest ::sai_test_acl_rule_create (
sai_object_id_t *acl_rule_id, unsigned int attr_count, ...)
{
    sai_status_t        sai_rc = SAI_STATUS_SUCCESS;
    va_list             ap;
    unsigned int        ap_idx = 0;
    sai_attribute_t    *p_attr_list = NULL;
    unsigned int        attr_id = 0;
    unsigned long       val = 0;
    uint32_t            object_count = 0;
    sai_mac_t          *mac_data;
    sai_mac_t          *mac_mask;
    sai_ip4_t          *ip4_data;
    sai_ip4_t          *ip4_mask;
    sai_ip6_t          *ip6_data;
    sai_ip6_t          *ip6_mask;

    va_start (ap, attr_count);

    p_attr_list
        = (sai_attribute_t *) calloc (attr_count, sizeof (sai_attribute_t));

    printf ("Testing ACL Rule Create API with attribute count: %d\r\n", attr_count);

    if (!p_attr_list) {
        printf ("Failed to allocate memory for attribute list.\r\n");

        return SAI_STATUS_FAILURE;
    }

    for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
        attr_id = va_arg (ap, unsigned int);
        p_attr_list[ap_idx].id = attr_id;

        printf ("Setting List index: %d with Attribute %s (ID = %d)\r\n",
                ap_idx, sai_acl_test_rule_attr_id_to_name_get(attr_id),
                attr_id);

        if (sai_test_acl_rule_field_range(attr_id)) {
            switch (sai_test_acl_rule_get_attr_type(attr_id)) {
                case SAI_TEST_ACL_ENTRY_ATTR_ONE_BYTE:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.data.u8 =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.mask.u8 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_TWO_BYTES:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.data.u16 =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.mask.u16 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_FOUR_BYTES:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.data.u32 =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.mask.u32 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_ENUM:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.data.s32 =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.mask.s32 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_MAC:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    mac_data = va_arg (ap, sai_mac_t *);
                    mac_mask = va_arg (ap, sai_mac_t *);

                    memcpy((&p_attr_list[ap_idx].value.aclfield.data.mac),
                           mac_data, sizeof(sai_mac_t));
                    memcpy((&p_attr_list[ap_idx].value.aclfield.mask.mac),
                           mac_mask, sizeof(sai_mac_t));
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_IPv4:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    ip4_data = va_arg (ap, sai_ip4_t *);
                    ip4_mask = va_arg (ap, sai_ip4_t *);

                    memcpy((&p_attr_list[ap_idx].value.aclfield.data.ip4),
                           ip4_data, sizeof(sai_ip4_t));
                    memcpy((&p_attr_list[ap_idx].value.aclfield.mask.ip4),
                           ip4_mask, sizeof(sai_ip4_t));
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_IPv6:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    ip6_data = va_arg (ap, sai_ip6_t *);
                    ip6_mask = va_arg (ap, sai_ip6_t *);

                    memcpy((&p_attr_list[ap_idx].value.aclfield.data.ip6),
                           ip6_data, sizeof(sai_ip6_t));
                    memcpy((&p_attr_list[ap_idx].value.aclfield.mask.ip6),
                           ip6_mask, sizeof(sai_ip6_t));
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_OBJECT_ID:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.data.oid =
                                                va_arg (ap, sai_object_id_t);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_OBJECT_LIST:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    object_count = va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.data.objlist.list =
                                                        (sai_object_id_t *)calloc(
                                                        object_count, sizeof(sai_object_id_t));

                    if (!p_attr_list[ap_idx].value.aclfield.data.objlist.list) {
                        return SAI_STATUS_NO_MEMORY;
                    }
                    p_attr_list[ap_idx].value.aclfield.data.objlist.count = object_count;

                    for (object_count = 0; object_count <
                         p_attr_list[ap_idx].value.aclfield.data.objlist.count; object_count++) {
                         p_attr_list[ap_idx].value.aclfield.data.objlist.list[object_count] =
                                                            va_arg (ap, sai_object_id_t);
                    }
                    break;
                default :
                    break;
            }
        } else if (sai_test_acl_rule_action_range(attr_id)) {
            switch (sai_test_acl_rule_get_attr_type(attr_id)) {
                case SAI_TEST_ACL_ENTRY_ATTR_ONE_BYTE:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclaction.parameter.u8 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_TWO_BYTES:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclaction.parameter.u16 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_FOUR_BYTES:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclaction.parameter.u32 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_ENUM:
                    /*
                     * Packet Action case which uses enum sai_packet_action_t
                     */
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclaction.parameter.s32 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_MAC:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    mac_data = va_arg (ap, sai_mac_t *);

                    memcpy((&p_attr_list[ap_idx].value.aclaction.parameter.mac),
                           mac_data, sizeof(sai_mac_t));
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_IPv4:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    ip4_data = va_arg (ap, sai_ip4_t *);

                    memcpy((&p_attr_list[ap_idx].value.aclaction.parameter.ip4),
                           ip4_data, sizeof(sai_ip4_t));
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_IPv6:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    ip6_data = va_arg (ap, sai_ip6_t *);

                    memcpy((&p_attr_list[ap_idx].value.aclaction.parameter.ip6),
                           ip6_data, sizeof(sai_ip6_t));
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_OBJECT_ID:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclaction.parameter.oid =
                                                va_arg (ap, sai_object_id_t);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_OBJECT_LIST:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    object_count = va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclaction.parameter.objlist.list =
                                                                (sai_object_id_t *)calloc(
                                                                object_count, sizeof(sai_object_id_t));

                    if (!p_attr_list[ap_idx].value.aclaction.parameter.objlist.list) {
                        return SAI_STATUS_NO_MEMORY;
                    }
                    p_attr_list[ap_idx].value.aclaction.parameter.objlist.count = object_count;

                    for (object_count = 0; object_count <
                         p_attr_list[ap_idx].value.aclaction.parameter.objlist.count; object_count++) {
                         p_attr_list[ap_idx].value.aclaction.parameter.objlist.list[object_count] =
                                                            va_arg (ap, sai_object_id_t);
                    }
                    break;
                default :
                    break;
            }
        } else {
            val = va_arg (ap, unsigned long);
            sai_test_acl_rule_attr_value_fill (&p_attr_list[ap_idx], val);
        }
    }

    sai_rc = p_sai_acl_api_tbl->create_acl_entry (acl_rule_id, attr_count,
                                                  p_attr_list);
    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("ACL Rule Creation API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("ACL Rule Creation API success, ACL Rule ID: 0x%"PRIx64"\r\n",
                *acl_rule_id);
    }

    for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
         if (sai_test_acl_rule_get_attr_type(p_attr_list[ap_idx].id) ==
             SAI_TEST_ACL_ENTRY_ATTR_OBJECT_LIST) {
             if (sai_test_acl_rule_field_range(p_attr_list[ap_idx].id)) {
                 free (p_attr_list[ap_idx].value.aclfield.data.objlist.list);
             } else if (sai_test_acl_rule_action_range(p_attr_list[ap_idx].id)) {
                 free (p_attr_list[ap_idx].value.aclaction.parameter.objlist.list);
             }
         }
    }

    if (p_attr_list) {
        free (p_attr_list);
    }

    return sai_rc;
}

/*
 * acl_rule_id  - [in] ACL Rule ID which needs to be removed.
 *
 * sai_test_acl_rule_remove (acl_rule_id)
 */
sai_status_t saiACLTest ::sai_test_acl_rule_remove (
                                            sai_object_id_t acl_rule_id)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;

    sai_rc = p_sai_acl_api_tbl->delete_acl_entry (acl_rule_id);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("ACL Rule removal API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("ACL Rule removal API success for ACL Rule ID: 0x%"PRIx64"\r\n", acl_rule_id);
    }

    return sai_rc;
}

/*
 * acl_rule_id    - [in] acl rule id which needs to be modified using set api
 * attr_count  - [in] number of acl rule attributes passed.
 *               for each attribute, {id, value} is passed.
 *
 * for attr-count = 2,
 * sai_test_acl_rule_set (acl_rule_id, 2, id_0, val_0, id_1, val_1)
 */
sai_status_t saiACLTest ::sai_test_acl_rule_set (
sai_object_id_t acl_rule_id,
unsigned int attr_count, ...)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    va_list          ap;
    unsigned int     ap_idx = 0;
    sai_attribute_t *p_attr_list = NULL;
    unsigned int     attr_id = 0;
    unsigned long    val = 0;
    uint32_t         object_count = 0;
    sai_mac_t       *mac_data;
    sai_mac_t       *mac_mask;
    sai_ip4_t       *ip4_data;
    sai_ip4_t       *ip4_mask;
    sai_ip6_t       *ip6_data;
    sai_ip6_t       *ip6_mask;

    va_start (ap, attr_count);

    p_attr_list
        = (sai_attribute_t *) calloc (attr_count, sizeof (sai_attribute_t));

    printf ("Testing ACL Rule Set API with attribute count: %d\r\n", attr_count);

    if (!p_attr_list) {
        printf ("Failed to allocate memory for attribute list.\r\n");

        return SAI_STATUS_FAILURE;
    }

    for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
        attr_id = va_arg (ap, unsigned int);
        p_attr_list[ap_idx].id = attr_id;

        printf ("Setting List index: %d with Attribute %s (ID = %d)\r\n",
                ap_idx, sai_acl_test_rule_attr_id_to_name_get(attr_id),
                attr_id);

        if (sai_test_acl_rule_field_range(attr_id)) {
            switch (sai_test_acl_rule_get_attr_type(attr_id)) {
                case SAI_TEST_ACL_ENTRY_ATTR_ONE_BYTE:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.data.u8 =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.mask.u8 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_TWO_BYTES:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.data.u16 =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.mask.u16 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_FOUR_BYTES:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.data.u32 =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.mask.u32 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_ENUM:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.data.s32 =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.mask.s32 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_MAC:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    mac_data = va_arg (ap, sai_mac_t *);
                    mac_mask = va_arg (ap, sai_mac_t *);

                    memcpy((&p_attr_list[ap_idx].value.aclfield.data.mac),
                           mac_data, sizeof(sai_mac_t));
                    memcpy((&p_attr_list[ap_idx].value.aclfield.mask.mac),
                           mac_mask, sizeof(sai_mac_t));
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_IPv4:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    ip4_data = va_arg (ap, sai_ip4_t *);
                    ip4_mask = va_arg (ap, sai_ip4_t *);

                    memcpy((&p_attr_list[ap_idx].value.aclfield.data.ip4),
                           ip4_data, sizeof(sai_ip4_t));
                    memcpy((&p_attr_list[ap_idx].value.aclfield.mask.ip4),
                           ip4_mask, sizeof(sai_ip4_t));
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_IPv6:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    ip6_data = va_arg (ap, sai_ip6_t *);
                    ip6_mask = va_arg (ap, sai_ip6_t *);

                    memcpy((&p_attr_list[ap_idx].value.aclfield.data.ip6),
                           ip6_data, sizeof(sai_ip6_t));
                    memcpy((&p_attr_list[ap_idx].value.aclfield.mask.ip6),
                           ip6_mask, sizeof(sai_ip6_t));
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_OBJECT_ID:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.data.oid =
                                                va_arg (ap, sai_object_id_t);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_OBJECT_LIST:
                    p_attr_list[ap_idx].value.aclfield.enable =
                                                va_arg (ap, unsigned int);
                    object_count = va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclfield.data.objlist.list =
                                                        (sai_object_id_t *)calloc(
                                                        object_count, sizeof(sai_object_id_t));

                    if (!p_attr_list[ap_idx].value.aclfield.data.objlist.list) {
                        return SAI_STATUS_NO_MEMORY;
                    }
                    p_attr_list[ap_idx].value.aclfield.data.objlist.count = object_count;

                    for (object_count = 0; object_count <
                         p_attr_list[ap_idx].value.aclfield.data.objlist.count; object_count++) {
                         p_attr_list[ap_idx].value.aclfield.data.objlist.list[object_count] =
                                                            va_arg (ap, sai_object_id_t);
                    }
                    break;
                default :
                    break;
            }
        } else if (sai_test_acl_rule_action_range(attr_id)) {
            switch (sai_test_acl_rule_get_attr_type(attr_id)) {
                case SAI_TEST_ACL_ENTRY_ATTR_ONE_BYTE:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclaction.parameter.u8 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_TWO_BYTES:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclaction.parameter.u16 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_FOUR_BYTES:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclaction.parameter.u32 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_ENUM:
                    /*
                     * Packet Action case which uses enum sai_packet_action_t
                     */
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclaction.parameter.s32 =
                                                va_arg (ap, unsigned int);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_MAC:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    mac_data = va_arg (ap, sai_mac_t *);

                    memcpy((&p_attr_list[ap_idx].value.aclaction.parameter.mac),
                           mac_data, sizeof(sai_mac_t));
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_IPv4:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    ip4_data = va_arg (ap, sai_ip4_t *);

                    memcpy((&p_attr_list[ap_idx].value.aclaction.parameter.ip4),
                           ip4_data, sizeof(sai_ip4_t));
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_IPv6:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    ip6_data = va_arg (ap, sai_ip6_t *);

                    memcpy((&p_attr_list[ap_idx].value.aclaction.parameter.ip6),
                           ip6_data, sizeof(sai_ip6_t));
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_OBJECT_ID:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclaction.parameter.oid =
                                                va_arg (ap, sai_object_id_t);
                    break;
                case SAI_TEST_ACL_ENTRY_ATTR_OBJECT_LIST:
                    p_attr_list[ap_idx].value.aclaction.enable =
                                                va_arg (ap, unsigned int);
                    object_count = va_arg (ap, unsigned int);
                    p_attr_list[ap_idx].value.aclaction.parameter.objlist.list =
                                                                (sai_object_id_t *)calloc(
                                                                object_count, sizeof(sai_object_id_t));

                    if (!p_attr_list[ap_idx].value.aclaction.parameter.objlist.list) {
                        return SAI_STATUS_NO_MEMORY;
                    }
                    p_attr_list[ap_idx].value.aclaction.parameter.objlist.count = object_count;

                    for (object_count = 0; object_count <
                         p_attr_list[ap_idx].value.aclaction.parameter.objlist.count; object_count++) {
                         p_attr_list[ap_idx].value.aclaction.parameter.objlist.list[object_count] =
                                                            va_arg (ap, sai_object_id_t);
                    }
                    break;
                default :
                    break;
            }
        } else {
            val = va_arg (ap, unsigned int);
            sai_test_acl_rule_attr_value_fill (&p_attr_list[ap_idx], val);
        }
    }

    sai_rc = p_sai_acl_api_tbl->set_acl_entry_attribute (acl_rule_id,
                                                         p_attr_list);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("ACL Rule Set API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("ACL Rule Set API success, ACL Rule ID: 0x%"PRIx64"\r\n",
                acl_rule_id);
    }

    for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
         if (sai_test_acl_rule_get_attr_type(p_attr_list[ap_idx].id) ==
             SAI_TEST_ACL_ENTRY_ATTR_OBJECT_LIST) {
             if (sai_test_acl_rule_field_range(p_attr_list[ap_idx].id)) {
                 free (p_attr_list[ap_idx].value.aclfield.data.objlist.list);
             } else if (sai_test_acl_rule_action_range(p_attr_list[ap_idx].id)) {
                 free (p_attr_list[ap_idx].value.aclaction.parameter.objlist.list);
             }
         }
    }

    if (p_attr_list) {
        free (p_attr_list);
    }

    return sai_rc;
}

/*
 * acl_rule_id    - [in] ACL Rule ID which needs to be fetch using Get API
 * attr_count  - [in] number of ACL Rule attributes passed.
 *               For each attribute, {id, value} is passed.
 *
 * For attr-count = 2,
 * sai_test_acl_rule_get (acl_rule_id, 2, id_0, val_0, id_1, val_1)
 */
sai_status_t saiACLTest ::sai_test_acl_rule_get (
sai_object_id_t acl_rule_id,
sai_attribute_t *p_attr_list,
unsigned int attr_count, ...)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    va_list          ap;
    unsigned int     ap_idx = 0;
    unsigned int     attr_id = 0;

    va_start (ap, attr_count);

    printf ("Testing ACL Rule Get API with attribute count: %d\r\n", attr_count);

    if (!p_attr_list) {
        return SAI_STATUS_FAILURE;
    }

    for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
        attr_id = va_arg (ap, unsigned int);
        p_attr_list[ap_idx].id = attr_id;

        printf ("Setting List index: %d with Attribute %s (ID = %d)\r\n",
                ap_idx, sai_acl_test_rule_attr_id_to_name_get(attr_id),
                attr_id);
    }

    sai_rc = p_sai_acl_api_tbl->get_acl_entry_attribute (acl_rule_id, attr_count,
                                                         p_attr_list);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("ACL Rule Get API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("ACL Rule Get API success, ACL Rule ID: 0x%"PRIx64"\r\n",
                acl_rule_id);
    }

    return sai_rc;
}

static const char* sai_acl_test_counter_attr_id_to_name_get (unsigned int attr_id) {
    if (SAI_ACL_COUNTER_ATTR_TABLE_ID == attr_id) {
        return "ACL Counter Table ID";
    } else if (SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT == attr_id) {
        return "ACL Counter Packet Mode";
    } else if (SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT == attr_id) {
        return "ACL Counter Byte Mode";
    } else if (SAI_ACL_COUNTER_ATTR_PACKETS == attr_id) {
        return "ACL Counter Get/Set Packet Count";
    } else if (SAI_ACL_COUNTER_ATTR_BYTES == attr_id) {
        return "ACL Counter Get/Set Byte Count";
    } else {
        return "INVALID/UNKNOWN";
    }
}

/*
 * acl_counter_id   - [out] pointer to ACL Counter ID generated by the API.
 * attr_count  - [in] number of ACL Counter attributes passed.
 *               For each attribute, {id, value} is passed.
 *
 * For attr-count = 2,
 * sai_test_acl_counter_create (acl_counter_id, 2, id_0, val_0, id_1, val_1)
 */
sai_status_t saiACLTest ::sai_test_acl_counter_create (
sai_object_id_t *acl_counter_id, unsigned int attr_count, ...)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    va_list          ap;
    unsigned int     ap_idx = 0;
    sai_attribute_t *p_attr_list = NULL;
    unsigned int     attr_id = 0;

    va_start (ap, attr_count);

    p_attr_list
        = (sai_attribute_t *) calloc (attr_count, sizeof (sai_attribute_t));

    printf ("Testing ACL Counter Create API with attribute count: %d\r\n", attr_count);

    if (!p_attr_list) {
        printf ("Failed to allocate memory for attribute list.\r\n");

        return SAI_STATUS_FAILURE;
    }

    for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
        attr_id = va_arg (ap, unsigned int);
        p_attr_list[ap_idx].id = attr_id;

        printf ("Setting List index: %d with Attribute %s (ID = %d)\r\n",
                ap_idx, sai_acl_test_counter_attr_id_to_name_get(attr_id),
                attr_id);

        switch (attr_id) {
            case SAI_ACL_COUNTER_ATTR_TABLE_ID:
                p_attr_list[ap_idx].value.oid = va_arg (ap, unsigned long);
                printf ("ACL Counter Table ID Value: 0x%"PRIx64"\r\n",
                        p_attr_list [ap_idx].value.oid);
                break;
            case SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT:
            case SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT:
                p_attr_list[ap_idx].value.booldata = va_arg (ap, unsigned int);
                break;
            case SAI_ACL_COUNTER_ATTR_PACKETS:
            case SAI_ACL_COUNTER_ATTR_BYTES:
                p_attr_list[ap_idx].value.u64 = va_arg (ap, unsigned int);
                break;
            default:
                break;
        }
    }

    sai_rc = p_sai_acl_api_tbl->create_acl_counter (acl_counter_id, attr_count,
                                                    p_attr_list);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("ACL Counter Creation API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("ACL Counter Creation API success, ACL Counter ID: 0x%"PRIx64"\r\n",
                *acl_counter_id);
    }

    if (p_attr_list) {
        free (p_attr_list);
    }

    return sai_rc;
}

/*
 * acl_counter_id  - [in] ACL Counter ID which needs to be removed.
 *
 * sai_test_acl_counter_remove (acl_counter_id)
 */
sai_status_t saiACLTest ::sai_test_acl_counter_remove (
                                            sai_object_id_t acl_counter_id)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;

    sai_rc = p_sai_acl_api_tbl->delete_acl_counter (acl_counter_id);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("ACL Counter removal API failed with error: %d\r\n", sai_rc);
    } else {
        printf ("ACL Counter removal API success for ACL Counter ID: 0x%"PRIx64"\r\n",
                acl_counter_id);
    }

    return sai_rc;
}

/*
 * acl_counter_id   - [in] ACL Counter ID which needs to be set
 * attr_count  - [in] number of ACL Counter attributes passed.
 *               For each attribute, {id, value} is passed.
 *
 * For attr-count = 1,
 * sai_test_acl_counter_set (acl_counter_id, 1, id_0, val_0)
 */
sai_status_t saiACLTest ::sai_test_acl_counter_set (
sai_object_id_t acl_counter_id, unsigned int attr_count, ...)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    va_list          ap;
    unsigned int     ap_idx = 0;
    sai_attribute_t *p_attr_list = NULL;
    unsigned int     attr_id = 0;

    va_start (ap, attr_count);

    p_attr_list
        = (sai_attribute_t *) calloc (attr_count, sizeof (sai_attribute_t));

    printf ("Testing ACL Counter Set API with attribute count: %d\r\n", attr_count);

    if (!p_attr_list) {
        printf ("Failed to allocate memory for attribute list.\r\n");

        return SAI_STATUS_FAILURE;
    }

    for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
        attr_id = va_arg (ap, unsigned int);
        p_attr_list[ap_idx].id = attr_id;

        printf ("Setting List index: %d with Attribute %s (ID = %d)\r\n",
                ap_idx, sai_acl_test_counter_attr_id_to_name_get(attr_id),
                attr_id);

        switch (attr_id) {
            case SAI_ACL_COUNTER_ATTR_PACKETS:
            case SAI_ACL_COUNTER_ATTR_BYTES:
                p_attr_list[ap_idx].value.u64 = va_arg (ap, unsigned int);
                printf ("ACL Counter Count Value: 0x%"PRIx64"\r\n",
                        p_attr_list [ap_idx].value.u64);
                break;
            default:
                p_attr_list[ap_idx].value.u64 = va_arg (ap, unsigned int);
                break;
        }
    }

    sai_rc = p_sai_acl_api_tbl->set_acl_counter_attribute (acl_counter_id,
                                                           p_attr_list);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("ACL Counter Set API failed with error %d, ACL Counter ID: 0x%"PRIx64"\r\n",
                sai_rc, acl_counter_id);
    } else {
        printf ("ACL Counter Set API success, ACL Counter ID: 0x%"PRIx64"\r\n",
                acl_counter_id);
    }

    if (p_attr_list) {
        free (p_attr_list);
    }

    return sai_rc;
}

/*
 * acl_counter_id   - [in] ACL Counter ID for which count needs to be get
 * attr_count  - [in] number of ACL Counter attributes passed.
 *               For each attribute, {id, value} is passed.
 *
 * For attr-count = 1,
 * sai_test_acl_counter_get (acl_counter_id, 1, id_0, val_0)
 */
sai_status_t saiACLTest ::sai_test_acl_counter_get (
sai_object_id_t acl_counter_id, sai_attribute_t *p_attr_list,
unsigned int attr_count, ...)
{
    sai_status_t     sai_rc = SAI_STATUS_SUCCESS;
    va_list          ap;
    unsigned int     ap_idx = 0;
    unsigned int     attr_id = 0;

    va_start (ap, attr_count);

    printf ("Testing ACL Counter Get API with attribute count: %d\r\n", attr_count);

    if (!p_attr_list) {
        printf ("Failed to allocate memory for attribute list.\r\n");

        return SAI_STATUS_FAILURE;
    }

    for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
        attr_id = va_arg (ap, unsigned int);
        p_attr_list[ap_idx].id = attr_id;

        printf ("Setting List index: %d with Attribute %s (ID = %d)\r\n",
                ap_idx, sai_acl_test_counter_attr_id_to_name_get(attr_id),
                attr_id);
    }

    sai_rc = p_sai_acl_api_tbl->get_acl_counter_attribute (acl_counter_id, attr_count,
                                                           p_attr_list);

    if (sai_rc != SAI_STATUS_SUCCESS) {
        printf ("ACL Counter Get API failed with error %d, ACL Counter ID: 0x%"PRIx64"\r\n",
                sai_rc, acl_counter_id);
    } else {
        printf ("ACL Counter Get API success, ACL Counter ID: 0x%"PRIx64"\r\n",
                acl_counter_id);
        for (ap_idx = 0; ap_idx < attr_count; ap_idx++) {
            switch (attr_id) {
                 case SAI_ACL_COUNTER_ATTR_PACKETS:
                    printf ("ACL Counter Get Packet Count Value: 0x%"PRIx64"\r\n",
                            p_attr_list [ap_idx].value.u64);
                    break;
                 case SAI_ACL_COUNTER_ATTR_BYTES:
                    printf ("ACL Counter Get Byte Count Value: 0x%"PRIx64"\r\n",
                            p_attr_list [ap_idx].value.u64);
                    break;
                 case SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT:
                    printf ("ACL Counter Get Enable Packet Count: %d\r\n",
                            p_attr_list [ap_idx].value.booldata);
                    break;
                 case SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT:
                    printf ("ACL Counter Get Enable Byte Count: %d\r\n",
                            p_attr_list [ap_idx].value.booldata);
                    break;
                 case SAI_ACL_COUNTER_ATTR_TABLE_ID:
                    printf ("ACL Counter Get Table Id Value: 0x%"PRIx64"\r\n",
                            p_attr_list [ap_idx].value.oid);
                    break;
                 default:
                    break;
            }
        }
    }

    return sai_rc;
}

