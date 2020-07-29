SAI Flow Latency Monitoring
-------------------------------------------------------------------------------
 Title       | SAI Flow Latency Monitoring using TAM Event Framework
-------------|-----------------------------------------------------------------
 Authors     | Jai Kumar, Broadcom Inc.
 Status      | In review
 Type        | Standards track
 Created     | 07/29/2020
 SAI-Version | 1.6.3
-------------------------------------------------------------------------------


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
