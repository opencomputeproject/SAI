#  Telemetry Granular Counter Subscription
-------------------------------------------------------------------------------
 Title       | Telemetry Granular Counter Subscription
-------------|-----------------------------------------------------------------
 Authors     | Jason Bos, Cisco
 Status      | In review
 Type        | Standards track
 Created     | 2023-01-11 - Initial Draft
 SAI-Version | 1.12
-------------------------------------------------------------------------------


## 1.0  Introduction

This spec enhances the existing TAM (Telemetry and Monitoring) spec to add granular counter subscription.

The TAM API  allows for predefined telemetry groups, like SAI_TAM_TELEMETRY_TYPE_SWITCH or SAI_TAM_TELEMETRY_TYPE_PORT. This provides extensibility to the API only to define new fixed collections. When collecting counters, all supported counters for the relevant objects are reported.

However, the user may wish to enable telemetry for a more limited set of counters, selected at runtime. For example, a user may wish to receive only port byte counters, or queue watermarks. Limiting the scope of the telemetry collection permits a device to generate counters samples faster than possible when all counters are collected, and fit many more samples into each network packet, allowing a higher rate of delivery.

In this mode, the collector may require a meaningful identifier for a counter, which identifies both the object as well as the specific counter on the object.
Depending on the report type, this may be carried directly in the report itself. Or it may be delivered separately, for example as an enterprise-specific
information element ID in an IPFIX template set.

## 2.0 Configuration

The subscription is represented by an object of SAI_TAM_COUNTER_SUBSCRIPTION. The creation of this object will indicate that a specific counter should be monitored.

```c
typedef enum _sai_tam_counter_subscription_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_START,

    /**
     * @brief TAM telemetry type object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TAM
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_TEL_TYPE = SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_START,

    /**
     * @brief Subscribed object
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_OBJECT_ID,

    /**
     * @brief Subscribed stat enum
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_STAT_ID,

     /**
     * @brief Telemetry label
     *
     * Label to identify the subscribed counter in telemetry reports.
     *
     * @type sai_uint64_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_LABEL,
```

The attribute SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_LABEL configures the application counter ID.


## 3 Configuration example

```c

// Example: Create a bulk report object
sai_attr_list[0].id = SAI_TAM_REPORT_ATTR_TYPE;
sai_attr_list[0].value.s32 = SAI_TAM_REPORT_TYPE_PROTO;

sai_attr_list[1].id = SAI_TAM_REPORT_ATTR_REPORT_MODE;
sai_attr_list[1].value.s32 = SAI_TAM_REPORT_MODE_BULK;

attr_count = 2;

sai_create_tam_report_fn(
   &sai_tam_report_obj,
   switch_id,
   attr_count,
   sai_attr_list);

// Example: Create telemetry type object to collect object stats
sai_attr_list[0].id = SAI_TAM_TEL_TYPE_ATTR_TAM_TELEMETRY_TYPE;
sai_attr_list[0].value.u32 = SAI_TAM_TELEMETRY_TYPE_OBJECT_STAT;

sai_attr_list[1].id = SAI_TAM_TEL_TYPE_ATTR_REPORT_ID;
sai_attr_list[1].value.oid = sai_tam_report_obj;

attr_count = 2;
sai_create_tam_tel_type_fn(
    &sai_tam_tel_type_obj,
    switch_id,
    attr_count,
    sai_attr_list);

// Example: Create counter subscription(s) to collect queue length and watermark
sai_attr_list[0].id = SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_TEL_TYPE;
sai_attr_list[0].value.u32 = sai_tam_tel_type_obj;

sai_attr_list[1].id = SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_OBJECT_ID;
sai_attr_list[1].value.oid = sai_queue_obj;

sai_attr_list[2].id = SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_STAT_ID;
sai_attr_list[2].value.u32 = SAI_QUEUE_STAT_CURR_OCCUPANCY_BYTES;

sai_attr_list[3].id = SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_LABEL;
sai_attr_list[3].value.u64 = 1;

attr_count = 4;
sai_create_tam_counter_subscription_fn(
    &sai_tam_subscription_obj1,
    switch_id,
    attr_count,
    sai_attr_list);

sai_attr_list[0].id = SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_TEL_TYPE;
sai_attr_list[0].value.u32 = sai_tam_tel_type_obj;

sai_attr_list[1].id = SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_OBJECT_ID;
sai_attr_list[1].value.oid = sai_queue_obj;

sai_attr_list[2].id = SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_STAT_ID;
sai_attr_list[2].value.u32 = SAI_QUEUE_STAT_WATERMARK_BYTES;

sai_attr_list[3].id = SAI_TAM_COUNTER_SUBSCRIPTION_ATTR_LABEL;
sai_attr_list[3].value.u64 = 2;

attr_count = 4;
sai_create_tam_counter_subscription_fn(
    &sai_tam_subscription_obj2,
    switch_id,
    attr_count,
    sai_attr_list);

// Example: Create Telemetry object
sai_attr_list[0].id = SAI_TAM_TELEMETRY_ATTR_TAM_TYPE_LIST;
sai_attr_list[0].value.objlist.count = 1
sai_attr_list[0].value.objlist.list[0] = sai_tam_tel_type_obj;

sai_attr_list[1].id = SAI_TAM_TELEMETRY_ATTR_COLLECTOR_LIST
sai_attr_list[1].value.objlist.count = 1;
sai_attr_list[1].value.objlist.list[0] = collector_obj;

attr_count = 2;
sai_create_tam_telemetry_fn(
    &sai_tam_telemetry_obj,
    switch_id,
    attr_count,
    sai_attr_list);

// Example: Create TAM object and bind to monitored objects:

sai_attr_list[0].id = SAI_TAM_ATTR_TAM_TELEMETRY_OBJECTS_LIST;
sai_attr_list[0].value.objlist.count = 1;
sai_attr_list[0].value.objlist.list[0] = sai_tam_telemetry_obj;

sai_attr_list[1].id = SAI_TAM_ATTR_TAM_BIND_POINT_TYPE_LIST;
sai_attr_list[1].value.objlist.count = 1;
sai_attr_list[1].value.objlist.list[0] = SAI_TAM_BIND_POINT_TYPE_SWITCH;

attr_count = 2;
sai_create_tam_fn(
    &sai_tam_obj,
    switch_id,
    attr_count,
    sai_attr_list);

// Example: Attach the TAM to the switch

sai_attr.id = SAI_SWITCH_ATTR_TAM_OBJECT_ID;
sai_attr.value.oid = sai_tam_obj;

sai_set_queue_attribute_fn(
    sai_switch_obj,
    sai_attr);
```

## 4 Example data export format

Counter subscription is independent of a specific report format. The format of the report is determined by the report object. In case of protobuf report format, a potential data layout is below:

```
message CounterSample {
    required uint64 timestamp = 1 [(telemetry_options).is_timestamp = true];
    repeated uint64 counter_values = 2;
}

message CounterSampleList {
    // Counter labels in the order the values will appear in each sample
    repeated uint64 counter_labels = 1;
    repeated CounterSample samples = 2;
}
```

