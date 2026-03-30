# Performance Monitoring SAI Specification
-------------------------------------------------------------------------------
 Title       | SAI support for Performance Monitoring
:-------------|:-----------------------------------------------------------------
 Authors     | Jai Kumar, Broadcom Inc
 Status      | In review
 Type        | Standards track
 Created     | 03/18/2026: Initial Draft
 SAI-Version | 1.19
-------------------------------------------------------------------------------


## 1.0  Introduction
As network fabric scale increases and data centers require regional spine connectivity, the number of downlinks for cluster connectivity is growing. This leads to more LAGs, more prefixes, and larger ECMP. This is also true for large scale up and scale across fabrics for AI/ML.

This increasing scale mandates that SAI be scalable, reliable, and high-performance. This specification addresses the performance component of SAI by introducing a new set of metrics to accurately measure the performance of various components within the SAI layer and below, such as SDK and hardware updates.

Using these metrics, deployments can isolate components impacting performance and focus on their optimization.



## 2.0 Terms and Acronyms

| Term| Description | 
|:---|:---|
| perfmon | Performance Metrics  |

## 3.0 Overview
The SAI infrastructure exposes a set of APIs as a standard interface to the upper layer.

These APIs are synchronous and blocking, making the completion time of any given API a critical performance measure. Note that application-specific callbacks are not addressed by this specification.

```
/**
 * @brief SAI common API type
 */
typedef enum _sai_common_api_t
{
    SAI_COMMON_API_CREATE      = 0,
    SAI_COMMON_API_REMOVE      = 1,
    SAI_COMMON_API_SET         = 2,
    SAI_COMMON_API_GET         = 3,
    SAI_COMMON_API_BULK_CREATE = 4,
    SAI_COMMON_API_BULK_REMOVE = 5,
    SAI_COMMON_API_BULK_SET    = 6,
    SAI_COMMON_API_BULK_GET    = 7,
    SAI_COMMON_API_MAX         = 8,
} sai_common_api_t;

```

This specification proposes API performance measures for the following metrics
1. Average Latency
2. Instantaneous Latency
3. Maximum Latency

### 3.1 Average, Instantaneous, and Maximum Latency
API completion time consists of the time spent in the SAI adapter and the SDK, including hardware update or query time. Time measured is irrespetcive of the status of the API call i.e. if the API call completes with error status, adapter will still account the measured latency during the time interval of the metrics computation. NOS tracks the return status of API calls and can account for errors as needed. Discounting latency for specific error statuses would result in inconsistent measurements, requiring metric subscribers to implement manual workarounds for those cases.

These metrics can be used to: 
- Improve SAI adapter and SDK implementations
- Provide a baseline for comparing different hardware
- Instantaneous value: Provides [time, n], where n > 1 represents the number of objects in a bulk API, or n = 1 represents the last observed latency for a single object
- Maximum: The highest value observed across the last n invocations
- Average: The average value over the last n invocations.


## 4.0 SAI Specification 
New perfmon object is introduced. Each perfmon object specifies the object of interest, set of APIs and metrics to be measured for each API.


Each perfmon object created has a binding to the switch object.

### 4.2 Perfmon Object
New perfmon object is introduced specifying API and metrics of interest.

#### 4.3.1 Metrics
Each API can be measure for a specific performance metrics as specified in sai_perfmon_metrics_t

```
/**
 * @brief Performance Monitoring Metrics
 */
typedef enum _sai_perfmon_metrics_t
{
    /**
     * @brief None
     */
    SAI_PERFMON_METRICS_NONE,

    /**
     * @brief Maximum latency observed
     */
    SAI_PERFMON_METRICS_MAX_LATENCY,

    /**
     * @brief Average latency observed
     */
    SAI_PERFMON_METRICS_AVERAGE_LATENCY,

    /**
     * @brief Instantaneous latency observed
     */
    SAI_PERFMON_METRICS_INST_LATENCY,

} sai_perfmon_metrics_t;

```

#### 4.3.2 Perfmon Object Attributes
Type of API to be monitored for performance and its associated attributes are specified in the perfmon object attributes

```
/**
 * @brief Performance Monitoring Attributes
 */
typedef enum _sai_perfmon_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_PERFMON_ATTR_START,

    /**
     * @brief Object to be monitored
     *
     * @type sai_object_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_PERFMON_ATTR_OBJECT_TYPE = SAI_PERFMON_ATTR_START,

    /**
     * @brief API to be monitored
     *
     * @type sai_common_api_t
     * @flags CREATE_AND_SET
     */
    SAI_PERFMON_ATTR_COMMON_API,

    /**
     * @brief Performance metrics to be collected
     *
     * @type sai_perfmon_metrics_t
     * @flags CREATE_AND_SET
     * @default SAI_PERFMON_METRICS_NONE
     */
    SAI_PERFMON_ATTR_PERFMON_METRICS,

    /**
     * @brief Time interval in milliseconds for metrics computation
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1024
     */
    SAI_PERFMON_ATTR_METRICS_TIME_INTERVAL,

    /**
     * @brief Performance data as collected
     *
     * @type sai_perfdata_t
     * @flags READ_ONLY
     */
    SAI_PERFMON_ATTR_PERFDATA,

    /**
     * @brief End of Performance Monitoring attributes
     */
    SAI_PERFMON_ATTR_END,

    /**
     * @brief End of Performance Monitoring attributes
     */
    SAI_PERFMON_ATTR_END,

    /** Custom range base value */
    SAI_PERFMON_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PERFMON_ATTR_CUSTOM_RANGE_END

} sai_perfmon_attr_t;

```

#### 4.3.3 Perfmon Object Switch Binding
List of perfmon objects can be bound to the switch object. This binding can be done as a SET operation when perfmon object is created.

```
    /**
     * @brief Performance Monitoring enabled on the switch
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PERFMO$
     * @default empty
     */
    SAI_SWITCH_ATTR_PERFMON_LIST,
```

#### 4.3.4 New data type
New data type is introduced to return the meausre of latency. Latency can be returned for a bulk object and in that case number of objects handled for a given object type is returned in the data type. Latency is measured in milli seconds.

```
typedef struct _sai_perfdata_t
{
    /** Time when metrics is collected */
    sai_timespec_t timespec;

    /** Latency measure in milliseconds */
    sai_uint32_t latency;

    /** Number of objects handled in this measure */
    sai_uint16_t num_objects;
} sai_perfdata_t;
```

## 5.0 Sample Workflow

This section talks about enabling performance monitoring for a given API and a metrics.

### 5.1 Create perfmon object
- Each perfmon object supports a single API and a single set of metrics. To monitor additional metrics for the same API or to monitor a different API, a new perfmon object must be created.
- Monitoring in the SAI adapter will only begin once the perfmon object is bound to the switch object.

```
/*
 * Configure CSIG Compact Tag for ABW signal processing and time interval of 256 micro seconds
 */

// Specify the Object of intererst
sai_attr_list[0].id = ﻿﻿SAI_PERFMON_ATTR_OBJECT_TYPE;
sai_attr_list[0].value.s32 = ﻿SAI_OBJECT_TYPE_ROUTE_ENTRY;

// Specify the API of interest
sai_attr_list[1].id = ﻿SAI_PERFMON_ATTR_COMMON_API;
sai_attr_list[1].value.s32 = ﻿SAI_COMMON_API_BULK_SET;

// Configure metrics to be measured
sai_attr_list[2].id = ﻿SAI_PERFMON_ATTR_PERFMON_METRICS;
sai_attr_list[2].value.s32 = ﻿SAI_PERFMON_METRICS_AVERAGE_LATENCY;

// Configure Time Interval in msec
sai_attr_list[3].id = ﻿SAI_PERFMON_ATTR_METRICS_TIME_INTERVAL;
sai_attr_list[3].value.u32 = ﻿2048;


// Create perfmon object
attr_count = 4;
create_perfmon(
	&sai_perfmon_object,
	switch_id, 
	attr_count, 
	sai_attr_list);
```

### 5.2 Read perfmon Metrics

Read the perfmon attribute for getting the API related metrics.

```
// Specify the read attribute
sai_attr_list[1].id = ﻿SAI_PERFMON_ATTR_PERFDATA;

// Read perfmon metrics
attr_count = 1;
create_perfmon(
	sai_perfmon_object,
	attr_count, 
	sai_attr_list);
...

Performance metrics data is returned as sai_perfdata_t structure. Structure contains the timestamp when last calculation of metrics was done by the SAI adapter.
