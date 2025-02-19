# SAI TAM enhancements
-------------------------------------------------------------------------------
 Title       | SAI TAM enahncements
-------------|-----------------------------------------------------------------
 Authors     |<code> Rajkumar P R, Ganapathasaa K, Ravindranath C K (Marvell Technology Inc) </code>
 Status      | In review
 Type        | Standards track
 Created     | 02/19/2025

-------------------------------------------------------------------------------
## Overview

This proposal enhances SAI TAM 2.0 by adding new reporting mode (sampling), a new transport type (GRE), header encapsulation attributes, and collector destination options (port/lag).
Additionally, a new ACL action attribute is added to bind the TAM object to specific flow.

## Requirements
- Identify a flow using ACL and apply TAM action.
- Support GRE transport type.
- New TAM report modes and attributes to support the generation of report sampling.
- Rate limit reports.
- Support for missing collector attributes, such as source MAC, destination MAC (needed for GRE header encapsulation), and the destination interface to which reports are transmitted. 


## Report Format

Example report generated with transport type GRE and vendor extension header.
```
   ------------------
   |      L2        |
   ------------------
   |    IPv4/IPv6   |
   ------------------
   |      GRE       |
   ------------------
   |   Vendor hdr   |
   ------------------
   |  Original pkt  |
   |  (Truncated)   |
   ------------------
   ```

### ACL action - TAM Object
```c
+    /** Bind a TAM object */
+    SAI_ACL_ACTION_TYPE_TAM_OBJECT = 0x0000003c,
 } sai_acl_action_type_t;

```
```c
+    /**
+     * @brief ACL bind point for TAM object
+     *
+     * Bind (or unbind) a TAM object.
+     *
+     * @type sai_acl_action_data_t sai_object_id_t
+     * @flags CREATE_AND_SET
+     * @objects SAI_OBJECT_TYPE_TAM
+     * @allownull true
+     * @default disabled
+     */
+    SAI_ACL_ENTRY_ATTR_ACTION_TAM_OBJECT = SAI_ACL_ENTRY_ATTR_ACTION_START + 0x3c,
+
+    /**
+     * @brief End of Rule Actions
+     */
+    SAI_ACL_ENTRY_ATTR_ACTION_END = SAI_ACL_ENTRY_ATTR_ACTION_TAM_OBJECT,

```
### TAM report mode
#### Sampling
Sending all the report events can overwhelm the collector. Sampling reduces the load on the collector by only sending
 1 in every N events.

```c
+    /** Report in a sampling mode, one report is sent for every n reports */
+    SAI_TAM_REPORT_MODE_SAMPLING,
+
```

### TAM report attributes
```c

+    /**
+     * @brief Sampling rate (every 1/sample_rate)
+     *
+     * @type sai_uint32_t
+     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
+     * @condition SAI_TAM_REPORT_ATTR_REPORT_MODE == SAI_TAM_REPORT_MODE_SAMPLING
+     */
+    SAI_TAM_REPORT_ATTR_SAMPLE_RATE,
+
+    /**
+     * @brief Maximum report rate per second
+     *
+     * Value 0 to no limit.
+     *
+     * @type sai_uint64_t
+     * @flags CREATE_AND_SET
+     * @default 0
+     * @validonly SAI_TAM_REPORT_ATTR_REPORT_MODE == SAI_TAM_REPORT_MODE_ALL
+     */
+    SAI_TAM_REPORT_ATTR_MAX_REPORT_RATE,
+
+    /**
+     * @brief Maximum reports per burst
+     *
+     * @type sai_uint64_t
+     * @flags CREATE_AND_SET
+     * @default 0
+     * @validonly SAI_TAM_REPORT_ATTR_REPORT_MODE == SAI_TAM_REPORT_MODE_ALL
+     */
+    SAI_TAM_REPORT_ATTR_MAX_REPORT_BURST,

```
### TAM GRE transport type
```c
+     /**
+     * @brief Transport GRE
+     */
+    SAI_TAM_TRANSPORT_TYPE_GRE,

} sai_tam_transport_type_t;
```

### TAM transport attribute to set gre protocol
```c
+    /**
+     * @brief GRE protocol Id
+     *
+     * @type sai_uint16_t
+     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
+     * @isvlan false
+     * @condition SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE == SAI_TAM_TRANSPORT_TYPE_GRE
+     */
+    SAI_TAM_TRANSPORT_ATTR_GRE_PROTOCOL_TYPE,

```

### TAM collector attributes
TAM collector is reachable via Desitination interface.

```c
+    /**
+     * @brief Source MAC address
+     *
+     * Note: Applicable only when SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE != SAI_TAM_TRANSPORT_TYPE_NONE
+     *
+     * @type sai_mac_t
+     * @flags CREATE_AND_SET
+     * @default vendor
+     */
+    SAI_TAM_COLLECTOR_ATTR_SRC_MAC,
+
+    /**
+     * @brief Destination MAC address
+     *
+     * Note: Applicable only when SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE != SAI_TAM_TRANSPORT_TYPE_NONE
+     *
+     * @type sai_mac_t
+     * @flags CREATE_AND_SET
+     * @default vendor
+     */
+    SAI_TAM_COLLECTOR_ATTR_DST_MAC,
+
+    /**
+     * @brief Destination used to reach collector
+     * 
+     * @type sai_object_id_t
+     * @flags CREATE_AND_SET
+     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_SYSTEM_PORT
+     * @allownull true
+     * @default SAI_NULL_OBJECT_ID
+     * @validonly SAI_TAM_COLLECTOR_ATTR_LOCALHOST == false
+     */
+    SAI_TAM_COLLECTOR_ATTR_DESTINATION,

```

## Sample workflow
1. Create Transport object of type GRE

```c
   count = 0;
   attr_list[count].id = SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE;
   attr_list[count].value.s32 = SAI_TAM_TRANSPORT_TYPE_GRE;
   count++;
   attr_list[count].id = SAI_TAM_TRANSPORT_ATTR_GRE_PROTOCOL_TYPE;
   attr_list[count].value.u16 = 0x8818;
   count++;
   
   rc = tam_api_p->create_tam_transport(&tam_transport_id, switch_id, count, attr_list);
   ASSERT_EQ(SAI_STATUS_SUCCESS, rc);

```
2. Create a Collector object
```c
    count = 0;
    memset(attr_list, 0, sizeof(attr_list));
    attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_TRANSPORT;
    attr_list[count].value.oid = tam_transport_id;
    count++;
    attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_LOCALHOST;
    attr_list[count].value.oid = false;
    count++;
    attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_SRC_IP;
    attr_list[count].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    attr_list[count].value.ipaddr.addr.ip4 = 0x0101010a;
    count++;
    attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_SRC_MAC;
    memcpy(&attr_list[count].value.mac, &smac, sizeof(sai_mac_t));
    count++;

    attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_DST_IP;
    attr_list[count].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    attr_list[count].value.ipaddr.addr.ip4 = 0x0101010b;
    count++;
    attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_DST_MAC;
    memcpy(&attr_list[count].value.mac, &dmac, sizeof(sai_mac_t));
    count++;
    attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_DESTINATION;
    attr_list[count].value.oid = port2_oid;
    count++;
    attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_DSCP_VALUE;
    attr_list[count].value.u8 = 8;
    count++;

    rc = tam_api_p->create_tam_collector(&tam_collector_id, switch_id, count, attr_list);

```
3. Create a report object in sampling mode
```c
    count = 0;
    memset(attr_list, 0, sizeof(attr_list));

    attr_list[count].id = SAI_TAM_REPORT_ATTR_TYPE;
    attr_list[count].value.s32 = SAI_TAM_REPORT_TYPE_VENDOR_EXTN;
    count++;

    attr_list[count].id = SAI_TAM_REPORT_ATTR_REPORT_MODE;
    attr_list[count].value.s32 = SAI_TAM_REPORT_MODE_SAMPLING;
    count++;
    attr_list[count].id = SAI_TAM_REPORT_ATTR_SAMPLE_RATE;
    attr_list[count].value.u32 = 1000;
    count++;
    rc = tam_api_p->create_tam_report(&tam_report_id, switch_id, count, attr_list);
```
4. Create event threshold object
```c
    count = 0;
    memset(attr_list, 0, sizeof(attr_list));

    attr_list[count].id = SAI_TAM_EVENT_THRESHOLD_ATTR_LATENCY;
    attr_list[count].value.u32 = 8000; //nsec
    count++;

    rc = tam_api_p->create_tam_event_threshold(&tam_event_threshold_id, switch_id, count, attr_list);

```
5. Create an event action object
```c
    count = 0;
    memset(attr_list, 0, sizeof(attr_list));

    attr_list[count].id = SAI_TAM_EVENT_ACTION_ATTR_REPORT_TYPE;
    attr_list[count].value.oid = tam_report_id;
    count++;

    rc = tam_api_p->create_tam_event_action(&tam_event_action_id, switch_id, count, attr_list);

```
6. Create a tam event object
```c
    count = 0;
    memset(attr_list, 0, sizeof(attr_list));

    attr_list[count].id = SAI_TAM_EVENT_ATTR_TYPE;
    attr_list[count].value.s32 = SAI_TAM_EVENT_TYPE_QUEUE_THRESHOLD;
    count++;

    attr_list[count].id = SAI_TAM_EVENT_ATTR_ACTION_LIST;

    attr_list[count].value.objlist.count = 1;
    attr_list[count].value.objlist.list = (sai_object_id_t *)malloc(1*sizeof(sai_object_id_t));
    attr_list[count].value.objlist.list[0] = tam_event_action_id;
    count++;

    attr_list[count].id = SAI_TAM_EVENT_ATTR_COLLECTOR_LIST;
    attr_list[count].value.objlist.count = 1;
    attr_list[count].value.objlist.list = (sai_object_id_t *)malloc(1*sizeof(sai_object_id_t));
    attr_list[count].value.objlist.list[0] = tam_collector_id;
    count++;

    attr_list[count].id = SAI_TAM_EVENT_ATTR_THRESHOLD;
    attr_list[count].value.oid = tam_event_threshold_id;
    count++;

    rc = tam_api_p->create_tam_event(&tam_event_id, switch_id, count, attr_list);
```
7. Create a TAM object
```c
    count = 0;
    memset(attr_list, 0, sizeof(attr_list));

    attr_list[count].id = SAI_TAM_ATTR_EVENT_OBJECTS_LIST;
    attr_list[count].value.objlist.count = 1;
    attr_list[count].value.objlist.list = (sai_object_id_t *)malloc(1*sizeof(sai_object_id_t));
    attr_list[count].value.objlist.list[0] = tam_event_id;
    count++;

    attr_list[count].id = SAI_TAM_ATTR_TAM_BIND_POINT_TYPE_LIST;
    attr_list[count].value.objlist.count = 2;
    attr_list[count].value.objlist.list = (sai_object_id_t *)malloc(2*sizeof(sai_object_id_t));
    attr_list[count].value.objlist.list[0] = SAI_TAM_BIND_POINT_TYPE_SWITCH;
    count++;

    rc = tam_api_p->create_tam(&tam_id, switch_id, count, attr_list);
```

8. Apply a TAM policy to an ACL flow.

```c
    bindpoint_type_arr[arr_idx++]    = SAI_ACL_BIND_POINT_TYPE_SWITCH;
    sai_bindpoint_type_list.list     = bindpoint_type_arr;
    sai_bindpoint_type_list.count    = arr_idx;

    ...

    attr_list[count].value.s32list = sai_bindpoint_type_list;
    attr_list[count].id            = SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST;
    count++;

    attr_list[count].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_IP;
    attr_list[count].value.booldata = true;
    count++;

    attr_list[count].id = SAI_ACL_TABLE_ATTR_ACL_ACTION_TYPE_LIST;
    attr_list[count].value.s32list.list = action_list;
    action_list[action_count++] = SAI_ACL_ACTION_TYPE_TAM_OBJECT;
    attr_list[count].value.s32list.count = action_count;
    count++;

    rc = acl_api_p->create_acl_table(&acl_table_oid, switch_id, count, attr_list);

    count = 0;
    memset(attr_list, 0, sizeof(attr_list));

    attr_list[count].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
    attr_list[count].value.oid = acl_table_oid;
    count++;

    attr_list[count].id        = SAI_ACL_ENTRY_ATTR_PRIORITY;
    attr_list[count].value.u32 = 0xFFFFFFFF;
    count++;

    attr_list[count].id = SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP;
    attr_list[count].value.aclfield.enable = true;
    attr_list[count].value.aclfield.data.ip4 = 0x01010101;
    attr_list[count].value.aclfield.mask.ip4 = 0xFFFF0000;
    count++;

    attr_list[count].id = SAI_ACL_ENTRY_ATTR_ACTION_TAM_OBJECT;
    attr_list[count].value.aclaction.parameter.oid  = tam_id;
    attr_list[count].value.aclaction.enable= true;
    count++;

    rc = acl_api_p->create_acl_entry(&acl_entry_oid, switch_id, count, attr_list);
```

#### Rate limit tam reports
Example flow to create tam report object with rate limit configurations.
Replace above step (3) with below api sequence.
```c
    count = 0;
    memset(attr_list, 0, sizeof(attr_list));

    attr_list[count].id = SAI_TAM_REPORT_ATTR_TYPE;
    attr_list[count].value.s32 = SAI_TAM_REPORT_TYPE_VENDOR_EXTN;
    count++;

    attr_list[count].id = SAI_TAM_REPORT_ATTR_REPORT_MODE;
    attr_list[count].value.s32 = SAI_TAM_REPORT_MODE_ALL;
    count++;
    attr_list[count].id = SAI_TAM_REPORT_ATTR_MAX_REPORT_RATE;
    attr_list[count].value.u32 = 512;
    count++;

    attr_list[count].id = SAI_TAM_REPORT_ATTR_MAX_REPORT_BURST;
    attr_list[count].value.u32 = 1024;
    count++;
    rc = tam_api_p->create_tam_report(&tam_report_id, switch_id, count, attr_list);
```
