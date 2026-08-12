# Mirror on Drop SAI Specification
-------------------------------------------------------------------------------
 Title       | SAI support for Packet Drop Monitoring
:-------------|:-----------------------------------------------------------------
 Authors     | Jai Kumar, Broadcom Inc
 Status      | In review
 Type        | Standards track
 Created     | 03/18/20206: Initial Draft
 SAI-Version | 1.18
-------------------------------------------------------------------------------


## 1.0  Introduction
Packet drops in networks, whether intentional due to policy or unintentional due to hardware errors and misconfigurations, adversely impact application performance and SLAs. Common causes include hardware errors (CRC, SER), policy-based drops (ACLs), forwarding decisions (invalid routes, TTL expiry), and network congestion.

Monitoring these drops can place a significant load on the collector subsystem. Depending on the requirements, a collector may need to monitor all drops, unique drops, or specific types of drops for certain flows.

TAM provides a common framework for monitoring these occurrences. This document outlines the SAI API steps required to configure packet drop monitoring for a flow or flow group based on specific drop reasons.


## 2.0 Terms and Acronyms

| Term| Description | 
|:---|:---|
| Drop Report | Typically IPFIX format packet carrying drop information along with other metadata |
| TAM | Telemetry And Monitoring. A SAI object for anything telemetry |




## 3.0 Overview
Packet drops in the switch are critical events. Reporting these drops, along with specific drop reasons and metadata, is essential for the fabric controller to analyze the cause and take corrective action.

Drop reports can be generated directly by the switch hardware directred twowards an external collector or local CPU or can be sent as notifications to the local CPU. 

Drop Report is the packetized information about the drop send to the collector or to the host typically encapsulated in IPFIX format. TAM framework supports different types of encapsulation and can be supported for drop report based on the underlying hardware support.
Minimaly drop report Should contain

- Trimmed Packet
- One or more drop reasons
- Timestamp
- Ingress port

## 4.0 Goals

Following principles are maintained regarding the drop monitoring infrastructure:

1. Hardware Independence: The APIs do not make assumptions about the hardware's drop monitoring capabilities. Silicon vendors may provide various capabilities and support for vendor specific drop reasons.

2. TAM Hierarchy: The NOS must use the TAM hierarchy  (including collectors, reports, events, and drop types) for configuring drop monitoring.

3. Flow Definitions: Flow definitions must use ACLs or equivalent infrastructure within the NOS, as these are basic match-and-action constructs.

4. Drop Types and Drop Reasons: SAI API attributes speicify a broad catageory of drop type like ingeress drops, egress drops and buffer drops. Each category of drops can further be queried for a detailed list of drop reasons supported by the underlying hardware.

5. Statefull Drop Monitoring: When flow state along with the drop experienced is maintained in the hardware it is referred to as stateful drop monitoring. Mainly goal of confiuring statefull drop monitoring is de-duplication and not flodding the collector or local CPU with the flood of drop reports or notifications. Hardware learning if enabled using the the hw learning atribute indicates that this is a stateful drop monitoring configuration. For stateless drop monitoring no hw learning is needed.

6. Learn Notification: If NOS is interested in maintaining the flow data based for drops experienced, this can be achieved using the learn notifications. NOS is notified using the learn notification callback. Callback includes enough content to be able to provide details on the learn event and drop reasons.

7. Flow Aging: Stateful drop monitoring maintains a flow state along with the drop reason in the hardware. These flows need to be aged out and are not in scope of this specification. Expectation is that aging of the learned flows experiencing drop is done by NOS.


## 5.0 SAI Specification Enhancements

The Packet Drop Monitoring specification is designed to utilize the TAM infrastructure, which provides the necessary objects and hierarchy to support packet drops. The TAM infrastructure has been extended to support specific drop types and reasons, as well as learn notifications.

### 5.1 Switch object enhancements.

New attributes are added that allow for querying supported drop types for specific hardware. This includes Ingress and Egress drop types, which leverage existing debug counter objects and have been extended to include buffer drops.

Additionally, a learn notification has been introduced. This allows for the registration of a callback for flows experiencing drops that are learned in the hardware, enabling stateful packet drop monitoring.


- Debug Counter Object Enhancement
```
/**
 * @brief Attribute data for buffer drop reasons
 */
typedef enum _sai_buffer_drop_reason_t
{
    /** Start of buffer drop reasons */
    SAI_BUFFER_DROP_REASON_START,

    /** Any buffer drop */
    SAI_BUFFER_DROP_REASON_ANY = SAI_BUFFER_DROP_REASON_START,

    /** IPG packet drops */
    SAI_BUFFER_DROP_REASON_IPG,

    /** End of buffer drop reasons */
    SAI_BUFFER_DROP_REASON_END,

    /** Custom range base value */
    SAI_BUFFER_DROP_REASON_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range */
    SAI_BUFFER_DROP_REASON_CUSTOM_RANGE_END

} sai_buffer_drop_reason_t;
```

- Switch Object Enhancement

```
    /**
     * @brief List of supported in drop reasons
     *
     * @type sai_s32_list_t sai_in_drop_reason_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_IN_DROP_REASON_LIST,

    /**
     * @brief List of supported out drop reasons
     *
     * @type sai_s32_list_t sai_out_drop_reason_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_OUT_DROP_REASON_LIST,

    /**
     * @brief List of supported buffer drop reasons
     *
     * @type sai_s32_list_t sai_buffer_drop_reason_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_BUFFER_DROP_REASON_LIST,

    /**
     * @brief Event learn notification callback
     * function passed to the adapter.
     *
     * Use sai_tam_event_learn_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_tam_event_learn_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_TAM_EVENT_LEARN_NOTIFY,
```

### 5.2 TAM Event Object Enhancements

New packet drop type enum is introduced to specify a specific category of drops for monitoiring.

```
/**
 * @brief Attribute data for buffer drop reasons
 */
typedef enum _sai_buffer_drop_reason_t
{
    /** Start of buffer drop reasons */
    SAI_BUFFER_DROP_REASON_START,

    /** Any buffer drop */
    SAI_BUFFER_DROP_REASON_ANY = SAI_BUFFER_DROP_REASON_START,

    /** IPG packet drops */
    SAI_BUFFER_DROP_REASON_IPG,

    /** End of buffer drop reasons */
    SAI_BUFFER_DROP_REASON_END,

    /** Custom range base value */
    SAI_BUFFER_DROP_REASON_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range */
    SAI_BUFFER_DROP_REASON_CUSTOM_RANGE_END

} sai_buffer_drop_reason_t;
```

TAM Event object is enhanced to enable drop monitoring for a specific category of drop and/or specific drop reasons. SAI_TAM_EVENT_ATTR_HW_LEARN enables hardware based learning to support stateful packet drop monitoring.

The SAI_TAM_EVENT_ATTR_TABLE_ID attribute indicates which hardware ACL table is being used to learn the dropped flows.

```
   /**
     * @brief Enable hardware based learning of events
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP
     */
    SAI_TAM_EVENT_ATTR_HW_LEARN,

    /**
     * @brief Enable packet drop type
     *
     * @type sai_packet_drop_type_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_DROP_TYPE_NONE
     * @validonly SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP
     */
    SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE,

    /**
     * @brief Enable ingress packet drop reason for monitoring
     *
     * @type sai_s32_list_t sai_in_drop_reason_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE == SAI_PACKET_DROP_TYPE_INGRESS
     */
    SAI_TAM_EVENT_ATTR_IN_DROP_REASON,

    /**
     * @brief Enable egress packet drop reason for monitoring
     *
     * @type sai_s32_list_t sai_out_drop_reason_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE == SAI_PACKET_DROP_TYPE_EGRESS
     */
    SAI_TAM_EVENT_ATTR_OUT_DROP_REASON,

    /**
     * @brief Enable buffer packet drop reason for monitoring
     *
     * @type sai_s32_list_t sai_buffer_drop_reason_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE == SAI_PACKET_DROP_TYPE_BUFFER
     */
    SAI_TAM_EVENT_ATTR_BUFFER_DROP_REASON,

    /**
     * @brief SAI ACL table object id. Indicates the field used to install the learned entry
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_TAM_EVENT_ATTR_HW_LEARN == true
     */
    SAI_TAM_EVENT_ATTR_TABLE_ID,
```


### 5.3 TAM Event Learn Entry Object

New TAM event learn entry object, SAI_OBJECT_TYPE_TAM_EVENT_LEARN_ENTRY is introduced, which represents a single entry learned in the hardware.

For every entry learned, the SAI adapter creates and populates this object as part of a callback notification. Multiple entries can be included in a single callback to facilitate bulk notification. These objects capture the specific flow dropped as well as the associated drop reason.

The SAI_TAM_EVENT_LEARN_ENTRY_ATTR_COUNTER_ID is allocated by the SAI adapter and provides the packet drop count. This should be read by the NOS periodically to determine if a flow is still experiencing drops, which is essential for the NOS to manage flow aging.

The SAI_TAM_EVENT_LEARN_ENTRY_ATTR_COUNTER_ID is read using standard stats api.

SAI_TAM_EVENT_LEARN_ENTRY_ATTR_ACL_ENTRY object is allocated by the SAI adapter for the learned entry and is used by NOS to age the entry if needed.

```
/**
 * @brief Enum defining learn event.
 */
typedef enum _sai_tam_event_learn_entry_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_START,

    /**
     * @brief Src IPv4 Address
     *
     * @type sai_ip4_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_FIELD_SRC_IP = SAI_TAM_EVENT_LEARN_ENTRY_ATTR_START,

    /**
     * @brief Dst IPv4 Address
     *
     * @type sai_ip4_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_FIELD_DST_IP,

    /**
     * @brief In-Port (mask is not needed)
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_FIELD_IN_PORT,

    /**
     * @brief Out-Port (mask is not needed)
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_FIELD_OUT_PORT,

    /**
     * @brief L4 Src Port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_FIELD_L4_SRC_PORT,

    /**
     * @brief L4 Dst Port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_FIELD_L4_DST_PORT,

    /**
     * @brief Inner L4 Src Port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_FIELD_INNER_L4_SRC_PORT,

    /**
     * @brief Inner L4 Dst Port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_FIELD_INNER_L4_DST_PORT,

    /**
     * @brief EtherType
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_FIELD_ETHER_TYPE,

    /**
     * @brief Inner EtherType
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_FIELD_INNER_ETHER_TYPE,

    /**
     * @brief IP Protocol
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_FIELD_IP_PROTOCOL,

    /**
     * @brief Inner IP Protocol
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_FIELD_INNER_IP_PROTOCOL,

    /**
     * @brief In drop reason
     *
     * @type sai_in_drop_reason_t
     * @flags CREATE_AND_SET
     * @default SAI_IN_DROP_REASON_L2_ANY
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_IN_DROP_REASON,

    /**
     * @brief Out drop reason
     *
     * @type sai_out_drop_reason_t
     * @flags CREATE_AND_SET
     * @default SAI_OUT_DROP_REASON_L2_ANY
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_OUT_DROP_REASON,

    /**
     * @brief Attach a counter
     *
     * SAI_COUNTER_STAT_PACKETS reflects the total number of packets dropped.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_COUNTER
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_COUNTER_ID,

    /**
     * @brief ACL table entry associated with this learn entry.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_ACL_ENTRY
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_ACL_ENTRY,

    /**
     * @brief End of Attributes
     */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_TAM_EVENT_LEARN_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_tam_event_learn_entry_attr_t;
```

### 5.3 Learn Entry Callback Notification

NOS can register to receive a callback notification for each lerned entry.
New data type is created to capture flow information as part of the bacllback.


```
//**
 * @brief Notification data format received from SAI TAM event learn callback
 *
 * @count attr[attr_count]
 */
typedef struct _sai_tam_event_learn_notification_data_t
{
    /**
     * @brief TAM Learn entry ID
     *
     * @objects SAI_OBJECT_TYPE_TAM_EVENT_LEARN_ENTRY
     */
    sai_object_id_t le_id;

    /** Attributes count */
    uint32_t attr_count;

    /**
     * @brief Attributes
     *
     * @objects SAI_OBJECT_TYPE_TAM_EVENT_LEARN_ENTRY
     */
    sai_attribute_t *attr;


} sai_tam_event_learn_notification_data_t;
```

Instance of this data structure is part of the callback.

```
/**
 * @brief TAM Event learn notifications
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Pointer to TAM event learn notification data array
 */
typedef void (*sai_tam_event_learn_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_tam_event_learn_notification_data_t *data);
```


### 5.3 ACL Action 

ACL tables already support TAM object as the ACL rule action and can be used to specify a flow tuple for drop monitoring.

No new fields or actions are added to ACL.

## 6.0 Sample Workflow

This section describes a sample workflow for enabling drop monitoring.


### 6.1 Capability Query
NOS can read SAI_SWITCH_ATTR_IN_DROP_REASON_LIST, SAI_SWITCH_ATTR_OUT_DROP_REASON_LIST, and SAI_SWITCH_ATTR_BUFFER_DROP_REASON_LIST for the list of supported drop reasons. This list may contain well known drop reasons as specified by the SAI specification along with vendor extensions.

NOS can subsequently use these set of drop reaons for enabling drop monitoring as well as understanding the drop reason code in the learn notification SAI_TAM_EVENT_LEARN_ENTRY_ATTR_IN_DROP_REASON, SAI_TAM_EVENT_LEARN_ENTRY_ATTR_OUT_DROP_REASON, and SAI_TAM_EVENT_LEARN_ENTRY_ATTR_BUFFER_DROP_REASON.

NOS can query SAI_TAM_EVENT_ATTR_HW_LEARN attribute to determine if the hardware supports stateful drop monitoring or not.

### 6.2 Enable Ingress Drop Reason Moinitoring for a 5-tuple flow

This section provides an example workflow for monitoriung a flow specified using 5-tuple for a drop reason.
This workflow assumes that ACL table acl_learn_table_id object is already allocated for learning of flows.


```
/*
 * Configure CSIG Compact Tag for ABW signal processing and time interval of 256 micro seconds
 */

// Specify the TAM event type for packet drops
sai_attr_list[0].id = ﻿﻿SAI_TAM_EVENT_ATTR_TYPE;
sai_attr_list[0].value.s32 = ﻿SAI_TAM_EVENT_TYPE_PACKET_DROP;

// Specify the TAM event type for drop monitoring
sai_attr_list[1].id = ﻿SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE;
sai_attr_list[1].value.s32 = ﻿SAI_PACKET_DROP_TYPE_INGRESS;

// Specify the drop reason
sai_attr_list[2].id = ﻿SAI_TAM_EVENT_ATTR_IN_DROP_REASON;
sai_attr_list[2].value.s32 = SAI_IN_DROP_REASON_TTL;

// Specify the ACL table
sai_attr_list[3].id = ﻿SAI_TAM_EVENT_ATTR_TABLE_ID;
sai_attr_list[3].value.oid = ﻿acl_learn_table_id;

// Enable HW learning
sai_attr_list[4].id = ﻿SAI_TAM_EVENT_ATTR_HW_LEARN;
sai_attr_list[4].booldata = ﻿true;

// Create tam event object
attr_count = 5;
create_tam_event(
	&sai_tam_event_packet_drop_obj,
	switch_id, 
	attr_count, 
	sai_attr_list);

// Create tam object
// This is helpful to support multiple tam event objects
sai_attr_list[0].id = SAI_TAM_ATTR_EVENT_OBJECTS_LIST
sai_attr_list[0].list[0].count = 1;
sai_attr_list[0].list[0].value.oid = sai_tam_event_packet_drop_obj;


attr_count = 1;
create_tam(
    &sai_tam_obj,
	switch_id, 
	attr_count, 
	sai_attr_list);
   
```

### 6.3 Create ACL table for learning 

ACL table is created with fields enabled that will be used to install the flow entry when the leraning happens. In this example flow is learned for the qset[ip proto, src ip, dst ip, sport, dport, ingress port].

This table can later be queried by NOS for learned entries using the GET API. Learned entries can be aged out using the SAI_TAM_EVENT_LEARN_ENTRY_ATTR_ACL_ENTRY object.

NOS can also age out the entries using sai_tam_event_learn_entry_attr_t/SAI_OBJECT_TYPE_TAM_EVENT_LEARN_ENTRY object. In such case SAI adapter will dereference the hardware index of the learned entry for aging.

Conceptually there is no SET on the ACL table entries as they are created by SAI adapter.
SAI adapter should fail any SET operation if done by the NOS.

New SAI_ACL_BIND_POINT_TYPE_TAM bind point is introduced for ACL table. This is to provide hint to the SAI adapter that this ACL table is mainly used for TAM purposes. 


```
    sai_object_id_t acl_table_id = 0ULL;
    acl_table_attrs[0].id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
    acl_table_attrs[0].value.s32 = SAI_ACL_STAGE_INGRESS;

    acl_table_attrs[2].id = SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL;
    acl_table_attrs[2].value.booldata = true;

    acl_table_attrs[2].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_IP;
    acl_table_attrs[2].value.booldata = true;

    acl_table_attrs[2].id = SAI_ACL_TABLE_ATTR_FIELD_DST_IP;
    acl_table_attrs[2].value.booldata = true;

    acl_table_attrs[2].id = SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS;
    acl_table_attrs[2].value.booldata = true;

    acl_table_attrs[2].id = SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT;
    acl_table_attrs[2].value.booldata = true;

    acl_table_attrs[2].id = SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT;
    acl_table_attrs[2].value.booldata = true;

    status = sai_acl_api->create_acl_table(&acl_table_id, 3, acl_table_attrs);
    if (status != SAI_STATUS_SUCCESS) {
        return status;
    }
```

### 6.4 Bind the TAM object to ACL rule specofying the flow of interest
Once the TAM event object is created and is bind to the tam object, final binding of tam object is done to the ACL rule as an action.
This ACL rule specify all IPv4 packets being monitored for drop.

```
    /* ACL Table created for the rules */
    sai_object_id_t acl_entry_id = 0ULL;
    acl_entry_attrs[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
    acl_entry_attrs[0].value.oid = acl_table_id;

    acl_entry_attrs[1].id = SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL;
    acl_entry_attrs[1].value.aclfield.data.u8 = 17;
    acl_entry_attrs[1].value.aclfield.mask.u8 = 255;

    acl_entry_attrs[2].id = SAI_ACL_ENTRY_ATTR_ACTION_TAM_OBJECT;
    acl_entry_attrs[2].value.aclaction.enable = true;
    acl_entry_attrs[2].value.aclaction.parameter.oid = sai_tam_obj;

    status = sai_acl_api->create_acl_entry(&acl_entry_id, 3, acl_entry_attrs);
    if (status != SAI_STATUS_SUCCESS) {
        return status;
```

### 6.5 How to configure stateless drop monitoring

Setting the SAI_TAM_EVENT_ATTR_HW_LEARN attribute to false disables hardware learning for dropped packets. Consequently, there are no learn notifications when this feature is disabled.

A TAM object can then be bound to an ACL entry for fine-grained packet drop monitoring or attached to a switch object to monitor any drops as specified in the TAM event object.

Please note that the NOS must configure the collector—either an external entity or a local CPU—to receive drop reports. The workflow for creating collectors and attaching them to TAM event or INT objects is standard and documented across several other documents.




