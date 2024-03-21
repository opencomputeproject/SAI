SAI Advanced Telemetry
-------------------------------------------------------------------------------
 Title       | SAI Advanced Telemetry using TAM Event Framework
-------------|-----------------------------------------------------------------
 Authors     | Jai Kumar, Broadcom Inc.
 Status      | In review
 Type        | Standards track
 Created     | 07/29/2020: Initial Draft
 SAI-Version | 1.6.3
-------------------------------------------------------------------------------

# Introduction
---
This spec talks about set of advanved telemetry features in SAI. These features use TAM as a generic framework. Following features are covered in this spec.

1. **Drop Monitoring**
a. Packet and Switch level drop monitoring with metadata
b. Intelligent drop monitoring using flow and flow's drop state
2. **Flow Monitoring**
3. **Latency Monitoring**
a. Static Thresholding
b. High/Low Watermark Thresholding

Objective of these features are not only to report an event but also provide ability to report event based on history/condition, additional metadata like timestamp/ingress/egress interface etc and portion of packet if applicable.

# Packet Drop Monitoring
---
Packets are dropped in networks for various reasons and impacts application performance and SLA adversely. Drops may happen intentionaly as a policy configuration or unintentionaly as a misconfiguration or hw error like SER.

Some of the common reasons for packet drops are
- HW Errors like CRC, SER. This may be because of faulty cables/optics or uncorrectable parity errors.
- Policy based for eg ACLs. This may be intentional where certain class of traffic is dropped in the switch pipeline or unintentional because of misconfiguration
- Forwarding decisions. Some examples are invalid route, switch not able to handle certain kind of packets, ttl expiry.
- Network congestion. This may happen because of queue congestion causing queue tail drops.

Monitoring all these packets drops is a big load on colletor subsystem. Collector subsystem MAY be interested in
> All the drops all the time
Unique drops all the time
Only certain kind of drops
Only certain kind of drops for certain flows
Monitoring a flow or a flow group for drops

TAM provides a common framework for monitoring such drops.
This document provides SAI API steps for configuring packet drop monitoring on flow, flow group or switch basis.

# SAI Packet Drop Reports
---
Report is the pacekt drop information send to collector.
Minimally report will contain
- packet itself
- drop reason
- timestamp
- ingress port

Additional information can be provide on based on switch capability like
- flow information
- drop state: first drop seen on the flow, new drop see on the flow or drop stopped on the flow

Packet drop reports can be generated in switch using a mirror session with a given encap.
Packet drop reports can also be generated in software with more detailed information and fancy data decoration like JSON/GPB/Thrift etc.

Content of drop report is out of scope from SAI API definition and is switch dependent information.

Broadly speaking there are 3 types of drop configuration possible
- **Stateless Drop Monitoring:** This is where switch is enabled for monitoring one or more or all drops. This configuration is at a switch level. Per drop reason counters MAY be provided by the switch pipeline.
- **Stateless Per Flow Drop Monitoring:** This is where switch is enabled for monitoring one or more or all drops on a per flow basis. This configuration needs a flow definition. Per flow per drop reason counters MAY be provided by switch pipeline.
- **Statefull Drop Monitoring:** This is where switch is enabled for monitoring one or more or all drops on a per flow basis and per flow drop state is maintained by the switch. This configuration needs a flow definition. Besides per flow per drop reason counters, switch pipeline MAY provide ability to detect the first drop seen and the last drop seen events. In this case if flow do not observe any drops for a certain duration also referred to flow aging interval, flow will be removed from the monitored state.

# SAI API Architecture Goals
---
1. No assumption about the drop monitoring infrastrcture in hardware. Silicon vendors provide different capability in terms of monitoring drops such as using ACLs, using trace event framework and so forth. APIs do not make any assumption about the methods used in the hardware to enable drop monitoring.
2. NOS MUST use TAM hierarchy or equivalent abstraction for configuring drop monitoring. Collector, report, event, drop types etc in this hierarchy.
3. Flow definitions MUST use ACL or equivalent infrastructure in NOS as they are basic match and action kind of constructs.
4. SAI APIs DO NOT provide explicit drop monitoring type e.g. stateless, statefull etc instead such functionality is achieved by combination of configuration semantics. For eg if there is no flow specified for a given drop monitoring event object then it is stateless drop monitoring, if there is a flow specified but event action is only reports then it is per flow stateless drop monitoring and lastly if event action is specified as monitoring "event state" along with flows then statefull drop monitoring is enabled for the flows in event object.
5. Individual event object can be created to achieve various kinds of drop monitoring. System MAY be configured for either one or any combination of such drop monitoring schemes.

# TAM Spec Update
---

Following new atributes will be added to TAM spec for supporting all flavors of drop monitoring.

[x] New binding of TAM object to ACL entry action is introduced to monitor TAM events.
For flow based and stateful drop monitoring where flows are monitored for drops, a new TAM object binding is introduced in the ACL entry.

```
    /**
     * @brief ACL Action Type
     */
    typedef enum _sai_acl_action_type_t
    {
        ...
        /** Bind a TAM object */
        SAI_ACL_ACTION_TYPE_TAM_OBJECT,

        ...
    } sai_acl_action_type_t;
    
    /**
     * @brief Attribute Id for sai_acl_entry
     *
     * @flags Contains flags
     */
    typedef enum _sai_acl_entry_attr_t
    {
        ...
        /**
         * @brief ACL bind point for TAM EVENT object
         *
         * Bind (or unbind) a TAM object.
         *
         * @type sai_acl_action_data_t sai_object_id_t
         * @flags CREATE_AND_SET
         * @objects SAI_OBJECT_TYPE_TAM
         * @allownull true
         * @default disabled
         */
        SAI_ACL_ENTRY_ATTR_ACTION_TAM_OBJECT,

        ...
    } sai_acl_entry_attr_t;
```
[x] TAM event aging interval for specifying aging of any tam event.
Any event may create a local state which need to be aged out. For example if a flow is monitored for drops then it may need to be aged out if no more drops are seen for  some time interval. This interval can be configured using the newly introduced event object attribute.
```sh
    /**
     * @brief TAM Attributes.
     */
    typedef enum _sai_tam_attr_t
    {
        ...
        /**
         * @brief Aging interval for an event
         *
         * @type sai_uint32_t
         * @flags CREATE_AND_SET
         * @default 0
         */
        SAI_TAM_EVENT_ATTR_AGING_INTERVAL,
        ...
    } sai_tam_event_attr_t;
```
[x] Extended TAM event types.
Packet drop type of interest can be specified using following attribute. Drop monitoring can be enabled for stateful monitoring using SAI_TAM_EVENT_TYPE_PACKET_DROP_STATEFUL event type.
Switch events can be specified using sai_switch_event_type_t.

```sh
    /**
     * @brief TAM Packet Ingress Drop Types
     */
    typedef enum _sai_packet_drop_type_ingress_t
    {
        /** None */
        SAI_PACKET_DROP_TYPE_INGRESS_NONE,
    
        /** ALL */
        SAI_PACKET_DROP_TYPE_INGRESS_ALL,
    
        /** Flags drops */
        SAI_PACKET_DROP_TYPE_INGRESS_CML_FLAGS_DROP,
    
        /** Layer 2 source static move drops */
        SAI_PACKET_DROP_TYPE_INGRESS_L2_SRC_STATIC_MOVE,
    
        /** Layer 2 discard drops */
        SAI_PACKET_DROP_TYPE_INGRESS_L2_SRC_DISCARD,
        ...
        ...
    } sai_packet_drop_type_ingress_t;

    /**
     * @brief TAM Packet MMU Drop Types
     */
    typedef enum _sai_packet_drop_type_mmu_t
    {
        /** None */
        SAI_PACKET_DROP_TYPE_MMU_NONE,
    
        /** ALL */
        SAI_PACKET_DROP_TYPE_MMU_ALL,
    
        /** Port group limit */
        SAI_PACKET_DROP_TYPE_MMU_ING_PG_LIMIT,
    
        /** Ingress port service pool limit */
        SAI_PACKET_DROP_TYPE_MMU_ING_PORTSP_LIMIT,
    
        /** Headroom pool limit */
        SAI_PACKET_DROP_TYPE_MMU_ING_HEADROOM_POOL_LIMIT,
    
        /** Egress queue limit */
        SAI_PACKET_DROP_TYPE_MMU_EGR_QUEUE_LIMIT,
    
        /** Egress port service pool limit */
        SAI_PACKET_DROP_TYPE_MMU_EGR_PORTSP_LIMIT,
    
        /** WRED check */
        SAI_PACKET_DROP_TYPE_MMU_WRED_CHECKS,
    } sai_packet_drop_type_mmu_t;

    /**
     * @brief TAM Packet Egress Drop Types
     */
    typedef enum _sai_packet_drop_type_egress_t
    {
        /** None */
        SAI_PACKET_DROP_TYPE_EGRESS_NONE,
    
        /** ALL */
        SAI_PACKET_DROP_TYPE_EGRESS_ALL,
    
        /** Layer 2 output interface drops */
        SAI_PACKET_DROP_TYPE_EGRESS_L2_OIF,
    
        /** Membership drops */
        SAI_PACKET_DROP_TYPE_EGRESS_MEMBERSHIP,
    
        /** Membership drops */
        SAI_PACKET_DROP_TYPE_EGRESS_DVP_MEMBERSHIP,
    
        /** TTL check drops */
        SAI_PACKET_DROP_TYPE_EGRESS_TTL,
    } sai_packet_drop_type_egress_t;


    /**
     * @brief TAM Switch Event Types
     */
    typedef enum _sai_switch_event_type_t
    {
        /** None */
        SAI_SWITCH_EVENT_TYPE_NONE,
    
        /** ALL */
        SAI_SWITCH_EVENT_TYPE_ALL,
    
        /** Stable Full */
        SAI_SWITCH_EVENT_TYPE_STABLE_FULL,
    
        /** Stable Error */
        SAI_SWITCH_EVENT_TYPE_STABLE_ERROR,
    
        /** Uncontrolled Shutdown */
        SAI_SWITCH_EVENT_TYPE_UNCONTROLLED_SHUTDOWN,
    
        /** Downgrade during Warm Boot */
        SAI_SWITCH_EVENT_TYPE_WARM_BOOT_DOWNGRADE,
    
        /** Parity Error */
        SAI_SWITCH_EVENT_TYPE_PARITY_ERROR,
    } sai_switch_event_type_t;

    /**
     * @brief Enum defining event types.
     */
    typedef enum _sai_tam_event_type_t
    {
        ...
        /**
         * @brief Packet drop event
         * Simple drop monitoring of packets
         */
        SAI_TAM_EVENT_TYPE_PACKET_DROP,

        /**
         * @brief Packet drop event
         * State aware drop monitoring of packets
         */
        SAI_TAM_EVENT_TYPE_PACKET_DROP_STATEFUL,
        
        /**
         * @brief Switch monitoring event
         */
        SAI_TAM_EVENT_TYPE_SWITCH,
        ...
        ...
    } sai_tam_event_type_t;

    /**
     * @brief Tam event attributes
     */
    typedef enum _sai_tam_event_attr_t
    {
        ...
        /**
         * @brief Type of ingress packet drops
         *
         * @type sai_packet_drop_type_ingress_t
         * @flags CREATE_AND_SET
         * @default SAI_PACKET_DROP_TYPE_INGRESS_NONE
         * @validonly SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP or SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP_STATEFUL
         */
        SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE_INGRESS,
    
        /**
         * @brief Type of MMU packet drops
         *
         * @type sai_packet_drop_type_mmu_t
         * @flags CREATE_AND_SET
         * @default SAI_PACKET_DROP_TYPE_MMU_NONE
         * @validonly SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP or SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP_STATEFUL
         */
        SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE_MMU,
    
        /**
         * @brief Type of egress packet drops
         *
         * @type sai_packet_drop_type_egress_t
         * @flags CREATE_AND_SET
         * @default SAI_PACKET_DROP_TYPE_EGRESS_NONE
         * @validonly SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP or SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_PACKET_DROP_STATEFUL
         */
        SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE_EGRESS,
     
        /**
         * @brief Type of switch event
         *
         * @type sai_switch_event_type_t
         * @flags CREATE_AND_SET
         * @default SAI_SWITCH_EVENT_TYPE_NONE
         * @validonly SAI_TAM_EVENT_ATTR_TYPE == SAI_TAM_EVENT_TYPE_SWITCH
         */
        SAI_TAM_EVENT_ATTR_SWITCH_EVENT_TYPE,
    
        /**
         * @brief Enable/Disable Samplepacket session
         *
         * Enable ingress sampling by assigning samplepacket object id Disable
         * ingress sampling by assigning #SAI_NULL_OBJECT_ID as attribute value.
         *
         * @type sai_object_id_t
         * @flags CREATE_AND_SET
         * @objects SAI_OBJECT_TYPE_SAMPLEPACKET
         * @allownull true
         * @default SAI_NULL_OBJECT_ID
         */
        SAI_TAM_EVENT_ATTR_INGRESS_SAMPLEPACKET_ENABLE,
    
        /**
         * @brief Device Identifier
         *
         * @type sai_uint32_t
         * @flags CREATE_AND_SET
         * @default 0
         */
        SAI_TAM_EVENT_ATTR_DEVICE_ID,
    
        /**
         * @brief Event Identifier
         *
         * @type sai_uint32_t
         * @flags CREATE_AND_SET
         * @default 0
         */
        SAI_TAM_EVENT_ATTR_EVENT_ID,

        ...
    } sai_tam_event_attr_t;
```

# Monitoring Any Packet Drop aka Stateless Drop Monitoring
---
This section talks about configuring switch for monitoring ANY packet drop using TAM APIs.
SAI API do not make any assumption about the hardware implementation for capturing the dropped packets. Drop packet may be captured using ACL infrastructure or more customer trace infrastructure in hardware. API provide a generic TAM object binding to switch for monitoring any kind of packet drops. Hardware specific SAI driver may implement the packet drop as ACL, TRACE or other kind of switch pipeline infrastructure.

##### Step 1: Create Report Object 
In this example report format is IPFIX
```sh
sai_attr_list[0].id = SAI_TAM_REPORT_ATTR_TYPE;
sai_attr_list[0].value.s32 = SAI_TAM_REPORT_TYPE_IPFIX;

sai_attr_list[1].id = SAI_TAM_REPORT_ATTR_ENTERPRISE_NUMBER;
sai_attr_list[1].value.s32 = 1234;

attr_count = 2;
sai_create_tam_report_fn(
    &sai_tam_report_obj,
    switch_id,
    attr_count,
    sai_attr_list);
```
##### Step 2: Create Event Action Object
Event action is to send packet drop report
```sh
sai_attr_list[0].id = SAI_TAM_EVENT_ACTION_ATTR_REPORT_TYPE;
sai_attr_list[0].value.oid = sai_tam_report_obj;

attr_count = 1;

sai_create_tam_event_action_fn(
    &sai_tam_event_action_obj, 
    switch_id,
    attr_count,
    sai_attr_list);
```
##### Step 3: Create a Transport Object
```sh
sai_attr_list[0].id = SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE; 
sai_attr_list[0].value.s32 = SAI_TAM_TRANSPORT_TYPE_UDP;

sai_attr_list[1].id = SAI_TAM_TRANSPORT_ATTR_MTU
sai_attr_list[1].value.u32 = 1500;
  
attr_count = 2;
 
sai_create_tam_transport_fn(
    &sai_tam_transport_obj,
    switch_id,
    attr_count,
    sai_attr_list);
    
/*
 * Transport type can be specified as port, to directly send 
 * reports out on a management interface
 */

sai_attr_list[0].id = SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE; 
sai_attr_list[0].value.s32 = SAI_TAM_TRANSPORT_TYPE_PORT;

sai_attr_list[1].id = SAI_TAM_TRANSPORT_ATTR_MTU;
sai_attr_list[1].value.u32 = 1500;

sai_attr_list[2].id = SAI_TAM_TRANSPORT_ATTR_SRC_MAC_ADDRESS;
sai_attr_list[2].value.u32 = 1500;

sai_attr_list[3].id = SAI_TAM_TRANSPORT_ATTR_DST_MAC_ADDRESS;
sai_attr_list[3].value.u32 = 1500;

attr_count = 4;
 
sai_create_tam_transport_fn(
    &sai_tam_transport_obj,
    switch_id,
    attr_count,
    sai_attr_list);
 
```
##### Step 4: Create a Collector Object

```sh
sai_attr_list[0].id = SAI_TAM_COLLECTOR_ATTR_SRC_IP;
sai_attr_list[0].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_attr_list[0].value.ipaddr.ip4 = 0x0101010a; 
 
sai_attr_list[1].id = SAI_TAM_COLLECTOR_ATTR_DST_IP;
sai_attr_list[1].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_attr_list[1].value.ipaddr.ip4 = 0x0101010b;
 
sai_attr_list[2].id = SAI_TAM_COLLECTOR_ATTR_TRANSPORT; 
sai_attr_list[2].value.oid = sai_tam_transport_obj;
 
attr_count = 3;
    sai_create_tam_collector_fn(
    &sai_tam_collector_obj,
    switch_id,
    attr_count,
    sai_attr_list);
```
##### Step 5: Create an Event  Object
```sh
sai_attr_list[0].id = SAI_TAM_EVENT_ATTR_TYPE;
sai_attr_list[0].value.s32 = SAI_TAM_EVENT_TYPE_PACKET_DROP;

sai_attr_list[1].id = SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE;
sai_attr_list[1].value.s32 = SAI_PACKET_DROP_TYPE_INGRESS;

sai_attr_list[2].id = SAI_TAM_EVENT_ATTR_ACTION_LIST;
sai_attr_list[2].value.objlist.count = 1;
sai_attr_list[2].value.objlist.list[0] = sai_tam_event_action_obj;

sai_attr_list[3].id = SAI_TAM_EVENT_ATTR_COLLECTOR_LIST;
sai_attr_list[3].value.objlist.count = 1;
sai_attr_list[3].value.objlist.list[0] = sai_tam_collector_obj;

attr_count = 4;

sai_create_tam_event_fn(
    &sai_tam_event_packet_drop_obj,
    switch_id,
    attr_count,
    sai_attr_list);
```
##### Step 7: Create TAM Object
```sh
sai_attr_list[0].id = SAI_TAM_ATTR_EVENT_OBJECTS_LIST;
sai_attr_list[0].value.objlist.count = 1;
sai_attr_list[0].value.objlist.list[0] = sai_tam_event_packet_drop_obj;
sai_attr_list[1].id = SAI_TAM_ATTR_TAM_BIND_POINT_TYPE_LIST;
sai_attr_list[1].value.objlist.count = 2;
sai_attr_list[1].value.objlist.list[0] = SAI_TAM_BIND_POINT_TYPE_PORT;
sai_attr_list[1].value.objlist.list[1] = SAI_TAM_BIND_POINT_TYPE_QUEUE;

attr_count = 2;

sai_create_tam_fn(
    &sai_tam_obj,
    switch_id,
    attr_count,
    sai_attr_list);
```
##### Step 7.1 : Event forwarding to Port
Binding TAM object to port object for all events will result in forwarding for reports to the port if transport type is specified as TYPE_PORT

```sh
sai_attr_list[x].id = SAI_PORT_ATTR_TAM_OBJECT; 
sai_attr_list[x].value.oid = sai_tam_obj;

sai_create_port_fn(..);
```

##### Step 8: Register a callback for host processing (Optional)
Host can register a SAI callback handler for given SAI TAM event object or object list.
Handler will be called with object id and data in the buffer. Buffer is treated as an opaque object. Producer and consumer of the data MUST have a pre-agreed format of the buffer. SAI API do not impose any restricton of the formating of the data in callbacks.

In this example callback handler is registered for packet drop event objects in the TAM object. Individual event object can be binded separately for the callback as well.

```sh
/**
 * @brief TAM event callback
 *
 * @count attr_list[attr_count]
 * @count buffer[buffer_size]
 * @objects attr_list SAI_OBJECT_TYPE_TAM_EVENT_ACTION
 * @objects tam_event_id SAI_OBJECT_TYPE_TAM_EVENT
 *
 * @param[in] tam_event_id Create Event Object ID
 * @param[in] buffer_size Actual buffer size in bytes
 * @param[in] buffer Data buffer
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 */
typedef void (*sai_tam_event_notification_fn)(
        _In_ sai_object_id_t tam_event_id,
        _In_ sai_size_t buffer_size,
        _In_ const void *buffer,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);
        
    // Event callback example 
    void *buffer; 
    sai_size_t buffer_size,  
    sai_attribute_t sai_tam_event_attr[3];
    uint32_t attr_count = 3;

    // alloocate buffer 
    buffer_alloc(&buffer, TAM_EVENT_BUF_SIZE);

    while(TRUE)
    { 
        buffer_size = TAM_EVENT_BUF_SIZE;
        sai_tam_event_notification_fn(sai_tam_obj, buffer, &buffer_size, &attr_count, sai_tam_event_attr);
        handle_packet(buffer, buffer_size, attr_count, sai_tam_event_attr);
    );
```

# Policy Based Monitoring Of Packet Drops
---
There may be an interest on  specific kind of packet drops for eg
- stage in a switch pipeline
-- ingress pipeline drops
-- egress pipeline drops
-- mmu drops

- packet drops for a flow or flow group
-- only flows ingress on a port
-- only vxlan flows
-- and so forth

Policy based drop monitoring creates an interesting profile of flow observation points.
Flows can be monitored for drops.

# Flow Based Stateless Drop Monitoring
---
Drop monitoring can be enabled for a given flow or flow group. There is no learning of flows in this method. If there is matching flow and drops are observed on the flow, drop report is generated. There are no additional flow state specific information is generated in drop reports like "drop start" or "drop stopped".
All the steps remain same as in the previous section with an additional step of configuring the flow. Flows are configured using the ACL tables in SAI.

This example configures a flow for monitoring ingress pipeline drops.
##### Step 1: Create Report Object 
##### Step 2: Create Event Action Object
##### Step 3: Create a Transport Object
##### Step 4: Create a Collector Object
##### Step 5: Create an Event  Object
##### Step 7: Create TAM Object and Event Bind Point if needed
Above steps are same as in previous section

##### Step 6: Create ACL entry for monitoring a flow match for ingress drops
Monitor a flow for ingress pipeline drops.

```sh
// Create an ACL table with IP keys configured 
sai_object_id_t acl_table_id2 = 0ULL;
acl_attr_list[0].id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
acl_attr_list[0].value.s32 = SAI_ACL_STAGE_INGRESS;

acl_attr_list[1].id = SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST;
acl_attr_list[1].value.objlist.count = 1;
acl_attr_list[1].value.objlist.list[0] = SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF;

acl_attr_list[3].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_IP;
acl_attr_list[3].value.booldata = True;

acl_attr_list[4].id = SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT;
acl_attr_list[4].value.booldata = True;

saistatus = sai_acl_api->create_acl_table(&acl_table_id2, 4, acl_attr_list);
if (saistatus != SAI_STATUS_SUCCESS) {
    return saistatus;
}

// Create an ACL table entry to deny *src_ip_to_suppress* and *src_l4_port_to_suppress*
acl_entry_attrs[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
acl_entry_attrs[0].value.oid = acl_table_id2;
acl_entry_attrs[1].id = SAI_ACL_ENTRY_ATTR_PRIORITY;
acl_entry_attrs[1].value.u32 = 1;
acl_entry_attrs[2].id = SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP;
CONVERT_STR_TO_IP(acl_entry_attrs[2].value.aclfield.data.ip4, "192.168.100.100");
acl_entry_attrs[3].id = SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT; 
acl_entry_attrs[3].value.aclfield.data.u16 = 1000;

/*
 **** Bind the TAM event object to the ACL entry ****
 */
acl_entry_attrs[3].id = SAI_ACL_ENTRY_ATTR_ACTION_TAM_EVENT_OBJECT;
acl_entry_attrs[3].value.oid = sai_tam_event_packet_drop_obj;
saistatus = sai_acl_api->create_acl_entry(&acl_entry, 4, acl_entry_attrs);
if (saistatus != SAI_STATUS_SUCCESS) {
    return saistatus;
}
```

# Stateful Drop Monitoring
---
Drop monitoring can be enabled on a per flow basis for a given drop type along with drop event state like "drop start" or "drop observed" or drop stop". Drop state is generated by the switch pipeline is vendor specific.
Enable stateful packet drop type event attribute. Optionally aging time can also be configured for flows not observing any drops and need to be aged out.

##### Step 1: Create Report Object 
##### Step 2: Create Event Action Object
##### Step 3: Create a Transport Object
##### Step 4: Create a Collector Object
Above steps are same as in previous section
##### Step 5: Create an Event  Object
```sh
    /* 
     * Stateful packet drops
     */
    sai_attr_list[0].id = SAI_TAM_EVENT_ATTR_TYPE;
    sai_attr_list[0].value.s32 = SAI_TAM_EVENT_TYPE_PACKET_DROP_STATEFUL;

    sai_attr_list[1].id = SAI_TAM_EVENT_ATTR_PACKET_DROP_TYPE;
    sai_attr_list[1].value.s32 = SAI_PACKET_DROP_TYPE_INGRESS;

    sai_attr_list[2].id = SAI_TAM_EVENT_ATTR_AGING_INTERVAL;
    sai_attr_list[2].value.s32 = 1000;

    sai_attr_list[3].id = SAI_TAM_EVENT_ATTR_ACTION_LIST;
    sai_attr_list[3].value.objlist.count = 1;
    sai_attr_list[3].value.objlist.list[0] = sai_tam_event_action_obj;
    
    sai_attr_list[4].id = SAI_TAM_EVENT_ATTR_COLLECTOR_LIST;
    sai_attr_list[4].value.objlist.count = 1;
    sai_attr_list[4].value.objlist.list[0] = sai_tam_collector_obj;
    
    attr_count = 5;
    
    sai_create_tam_event_fn(
        &sai_tam_event_packet_drop_obj,
        switch_id,
        attr_count,
        sai_attr_list);
```
##### Step 6: Create ACL entry for monitoring a flow match for ingress drops
##### Step 7: Create TAM Object and Event Bind Point if needed
Above steps are same as in previous section

# Flow Monitoring Using TAM Event Framework
---
Flow monitoring can be eanbled by choosing the flow watchlist event type. ACLs are used to define a flow and TAM event object is binded to the ACL entry or ACL table.
Following worflow creates an IPFIX report send using a mirror session in HW for the monitored flows. Report can be send to the collector as specified in the collector attribute using transport like UDP/TCP/gRPC/localhost etc. Flow is defined as an IP tuple using ACL framework.
##### Step 1: Create Report Object 
##### Step 2: Create Event Action Object
##### Step 3: Create a Transport Object
##### Step 4: Create a Collector Object
Above steps are same as in previous section
##### Step 5: Create an Event  Object
```sh
    /* 
     * Create a flow watchlist event type
     */
    sai_attr_list[0].id = SAI_TAM_EVENT_ATTR_TYPE;
    sai_attr_list[0].value.s32 = SAI_TAM_EVENT_TYPE_FLOW_WATCHLIST;

    sai_attr_list[2].id = SAI_TAM_EVENT_ATTR_AGING_INTERVAL;
    sai_attr_list[2].value.s32 = 1000;

    sai_attr_list[3].id = SAI_TAM_EVENT_ATTR_ACTION_LIST;
    sai_attr_list[3].value.objlist.count = 1;
    sai_attr_list[3].value.objlist.list[0] = sai_tam_event_action_obj;
    
    sai_attr_list[4].id = SAI_TAM_EVENT_ATTR_COLLECTOR_LIST;
    sai_attr_list[4].value.objlist.count = 1;
    sai_attr_list[4].value.objlist.list[0] = sai_tam_collector_obj;
    
    attr_count = 5;
    
    sai_create_tam_event_fn(
        &sai_tam_event_flow_monitoring_obj,
        switch_id,
        attr_count,
        sai_attr_list);
```
##### Step 6: Create ACL entry for monitoring a flow
##### Step 7: Create TAM Object and Event Bind Point if needed
Above steps are same as in previous section


# Flow Latency Monitoring
---
Flows are monitored using various techniques. Some of the common techniques are
- Sflow
- IPFIX
- Mirroring

These techniques focus on creating a sample profile of a flow distribution and export this with various measurements to an external consumer. Analysis of the measurements are done offline by the consumer. These techniques work very well for small scale, low throughput traffic. As the data center scale increases and new tbps throughput chips come out, the amount of measurements to be analyzed by the consumer hereby called as “analytics engine” (AE) grows exponentially.  Investment on AE infrastructure becomes prohibitively expensive.
 
There is a clear need to reduce the overhead on AE infrastructure and perform hierarchical analysis of data. First level of hierarchy is the switch pipeline itself. Second level of hierarchy is embedded controllers and then host cpu and so on. As the traffic bubbles up at each hierarchy there are set of operations performed and concise reports are generated to reduce the traffic rate at the next level of hierarchy. Assumption is that this disaggregated hierarchical model of analytics will scale better for mega scale data centers.

### Problem Definition
---
In a data center, packets in a flow spend certain time within a switch. This time spend in the switch pipeline is referred to as residence time or latency. Residence time is comprised of mainly 3 components in a typical pipeline
- Ingress pipeline
- MMU/Fabric
- Egress pipeline

Time spent in IP and EP is fixed. MMU/Fabric based on queue congestion experienced will impact the residence time directly. This means that customers can use residence time as a good indicator of congestion experienced or chances of experiencing drop, on a per packet per flow basis. Queue latency measure is not good enough as it is an aggregate measure for the flows enqueued in a given queue.
 
Given the high rate of traffic and number of flows, exporting residence time for collector or AE to analyze is prohibitively expensive as discussed earlier. Better approach can be if a switch pipeline can generate an event only when residence time is greater than certain value hereby referred to as a high watermark.
 
Just checking the high watermark may not be good enough as multiple such breaches may just indicate a transient condition.  Event for the first breach would have been enough.  
 
To prevent unnecessary such events, a low watermark value is proposed to create a hysteresis cycle. Events are generated based on history of the packet in flow whenever high or low watermark is breached.
 
Based on these events, analytics engine can use the latency experienced by a flow in the hysteresis window to do following
- Generate an audit ticket for impacted applications
- Monitor flows belonging to different SLA class for billing
- Run ML algorithm to do some kind of predictions and provisioning (or load distribution) when congestion is experienced

Collector workflow will look something like this
> Step 1:
Collector configures latency profile map with [high, low] watermark values
 
> Step 2:
Collector configures flow group for latency monitoring and assigns a latency profile.
 
> Step 3:
Collector receives high latency breach event from switch and triggers monitoring of flow i.e. starts reading flow latency from HW
 
> Step 4:
Collector MAY run some algorithm to tune the threshold parameters, generate SLA/audit tickets etc
 
> Step 5:
Collector receives a low latency breach event from switch and stops monitoring the flow. It MAY also instruct host cpu to stop monitoring flow without any low breach event.

TAM provides a common framework for monitoring such latency events.
This document provides SAI API steps for configuring latency monitoring for a given flow or flow group.

# TAM Spec Update
---
Following new event type will be added to TAM spec for supporting flow latency monitoring.
```sh
    /**
     * @brief Enum defining event types.
     */
    typedef enum _sai_tam_event_type_t
    {
        ...
        /**
         * @brief Flow latency monitoring event
         */
        SAI_TAM_EVENT_TYPE_FLOW_LATENCY,
        ...
    } sai_tam_event_type_t;
```
    
# SAI Flow latency monitoring with an absolute watermark value
---
This section talks about configuring an absolute value as a latency threshold for a flow group. This is also referred to as stateless latency monitoring. Flows are defined using ACL table/entry.

##### Step 1: Create Report Object 
In this example report format is IPFIX
```sh
    sai_attr_list[0].id = SAI_TAM_REPORT_ATTR_TYPE;
    sai_attr_list[0].value.s32 = SAI_TAM_REPORT_TYPE_IPFIX;
    
    sai_attr_list[1].id = SAI_TAM_REPORT_ATTR_ENTERPRISE_NUMBER;
    sai_attr_list[1].value.s32 = 1234;
    
    attr_count = 2;
    sai_create_tam_report_fn(
        &sai_tam_report_obj,
        switch_id,
        attr_count,
        sai_attr_list);
```
##### Step 2: Create Event Action Object
Event action to send report
```sh
    sai_attr_list[0].id = SAI_TAM_EVENT_ACTION_ATTR_REPORT_TYPE;
    sai_attr_list[0].value.oid = sai_tam_report_obj;
    
    attr_count = 1;
    
    sai_create_tam_event_action_fn(
        &sai_tam_event_action_obj, 
        switch_id,
        attr_count,
        sai_attr_list);
```
##### Step 3: Create a Transport Object
Transport layer to use for reports.
```sh
    sai_attr_list[0].id = SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE; 
    sai_attr_list[0].value.s32 = SAI_TAM_TRANSPORT_TYPE_UDP;
    
    /*
     * Transport type can be specified as mirror as well to send 
     * reports using mirror session in hw.
     *
     * sai_attr_list[0].id = SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE; 
     * sai_attr_list[0].value.s32 = SAI_TAM_TRANSPORT_TYPE_MIRROR;
     */
    
    sai_attr_list[1].id = SAI_TAM_TRANSPORT_ATTR_MTU
    sai_attr_list[1].value.u32 = 1500;
      
    attr_count = 2;
     
    sai_create_tam_transport_fn(
        &sai_tam_transport_obj,
        switch_id,
        attr_count,
        sai_attr_list);
```
##### Step 4: Create a Collector Object
Collector related information.
```sh
    sai_attr_list[0].id = SAI_TAM_COLLECTOR_ATTR_SRC_IP;
    sai_attr_list[0].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    sai_attr_list[0].value.ipaddr.ip4 = 0x0101010a; 
     
    sai_attr_list[1].id = SAI_TAM_COLLECTOR_ATTR_DST_IP;
    sai_attr_list[1].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    sai_attr_list[1].value.ipaddr.ip4 = 0x0101010b;
     
    sai_attr_list[2].id = SAI_TAM_COLLECTOR_ATTR_TRANSPORT; 
    sai_attr_list[2].value.oid = sai_tam_transport_obj;
     
    attr_count = 3;
        sai_create_tam_collector_fn(
        &sai_tam_collector_obj,
        switch_id,
        attr_count,
        sai_attr_list);
```
##### Step 5: Create a Threshold Object
Create threshold object for event.
```sh
    sai_attr_list[0].id = SAI_TAM_EVENT_THRESHOLD_ATTR_ABS_VALUE;
    sai_attr_list[0].value.u32 = 1234;
    
    sai_attr_list[1].id = SAI_TAM_EVENT_THRESHOLD_ATTR_UNIT;
    sai_attr_list[1].value.s32 = SAI_TAM_EVENT_THRESHOLD_UNIT_NANOSEC;
    
    attr_count = 2;
    
    sai_create_tam_event_threshold_fn(
        &sai_tam_event_threshold_obj,
        switch_id,
        attr_count,
        sai_attr_list);
```
##### Step 6: Create an Event Object
Bind event related objects together in event object.
```sh
    sai_attr_list[0].id = SAI_TAM_EVENT_ATTR_TYPE;
    sai_attr_list[0].value.s32 = SAI_TAM_EVENT_TYPE_FLOW_LATENCY;

    sai_attr_list[1].id = SAI_TAM_EVENT_ATTR_ACTION_LIST;
    sai_attr_list[1].value.objlist.count = 1;
    sai_attr_list[1].value.objlist.list[0] = sai_tam_event_action_obj;
    
    sai_attr_list[2].id = SAI_TAM_EVENT_ATTR_COLLECTOR_LIST;
    sai_attr_list[2].value.objlist.count = 1;
    sai_attr_list[2].value.objlist.list[0] = sai_tam_collector_obj;
    
    sai_attr_list[3].id = SAI_TAM_EVENT_ATTR_THRESHOLD;
    sai_attr_list[3].value.objlist.count = 1;
    sai_attr_list[3].value.objlist.list[0] = sai_tam_event_threshold_obj;
    
    attr_count = 4;
    
    sai_create_tam_event_fn(
        &sai_tam_event_flow_lat_obj,
        switch_id,
        attr_count,
        sai_attr_list);
```
##### Step 7: Create TAM Object
Binding of TAM object is at switch level since any packet drop is being monitored.
```sh
    sai_attr_list[0].id = SAI_TAM_ATTR_EVENT_OBJECTS_LIST;
    sai_attr_list[0].value.objlist.count = 1;
    sai_attr_list[0].value.objlist.list[0] = sai_tam_event_obj;
     
    sai_attr_list[1].id = SAI_TAM_ATTR_TAM_BIND_POINT_TYPE_LIST;
    sai_attr_list[1].value.objlist.count = 2;
    sai_attr_list[1].value.objlist.list[0] = SAI_TAM_BIND_POINT_TYPE_PORT; 
    sai_attr_list[1].value.objlist.list[1] = SAI_TAM_BIND_POINT_TYPE_QUEUE; 
     
    attr_count = 2;
        sai_create_tam_fn(
        &sai_tam_obj,
        switch_id,
        attr_count,
        sai_attr_list);
```
##### Step 8: Create ACL entry for flow latency monitoring
Monitor a flow for latency breach.

```sh
    // Create an ACL table with IP keys configured 
    sai_object_id_t acl_table_id2 = 0ULL;
    acl_attr_list[0].id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
    acl_attr_list[0].value.s32 = SAI_ACL_STAGE_EGRESS;
    
    acl_attr_list[1].id = SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST;
    acl_attr_list[1].value.objlist.count = 1;
    acl_attr_list[1].value.objlist.list[0] = SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF;
    
    acl_attr_list[3].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_IP;
    acl_attr_list[3].value.booldata = True;
    
    acl_attr_list[4].id = SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT;
    acl_attr_list[4].value.booldata = True;
    
    saistatus = sai_acl_api->create_acl_table(&acl_table_id2, 4, acl_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    // Create an ACL table entry
    acl_entry_attrs[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
    acl_entry_attrs[0].value.oid = acl_table_id2;
    acl_entry_attrs[1].id = SAI_ACL_ENTRY_ATTR_PRIORITY;
    acl_entry_attrs[1].value.u32 = 1;
    acl_entry_attrs[2].id = SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP;
    CONVERT_STR_TO_IP(acl_entry_attrs[2].value.aclfield.data.ip4, "192.168.100.100");
    acl_entry_attrs[3].id = SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT; 
    acl_entry_attrs[3].value.aclfield.data.u16 = 1000;
    
    /*
     **** Bind the TAM  object to the ACL entry ****
     */
    acl_entry_attrs[3].id = SAI_ACL_ENTRY_ATTR_ACTION_TAM_OBJECT;
    acl_entry_attrs[3].value.oid = sai_tam_event_flow_lat_obj;
    saistatus = sai_acl_api->create_acl_entry(&acl_entry, 4, acl_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
```

# SAI Flow latency monitoring with high/low watermark value
---
This is stateful latency monitoring where high and low watermark breach is managed via hysteresis cycle. 
##### Step 1: Create Report Object 
##### Step 2: Create Event Action Object
##### Step 3: Create a Transport Object
##### Step 4: Create a Collector Object
Above steps are same as in previous section

##### Step 5: Create a Threshold Object
Create a threshold object.
```sh
    sai_attr_list[0].id = SAI_TAM_EVENT_THRESHOLD_ATTR_LOW_WATERMARK;
    sai_attr_list[0].value.u32 = 1234;
    
    sai_attr_list[1].id = SAI_TAM_EVENT_THRESHOLD_ATTR_HIGH_WATERMARK;
    sai_attr_list[1].value.u32 = 6789;
    
    sai_attr_list[2].id = SAI_TAM_EVENT_THRESHOLD_ATTR_UNIT;
    sai_attr_list[2].value.s32 = SAI_TAM_EVENT_THRESHOLD_UNIT_NANOSEC;
    
    attr_count = 3;
    
    sai_create_tam_event_threshold_fn(
        &sai_tam_event_threshold_obj,
        switch_id,
        attr_count,
        sai_attr_list);
```
##### Step 6: Create an Event Object
##### Step 7: Create TAM Object
##### Step 8: Create ACL entry for flow latency monitoring
Steps 7 and 8 are same as in section 2.0
