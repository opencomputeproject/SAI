# SAI IP Measurements (IPM) Feature Proposal
-------------------------------------------------------------------------------
 Title       | SAI IP Measurements
-------------|-----------------------------------------------------------------
 Authors     | Cisco
 Status      | Draft
 Type        | Standards track
 Created     | 2026-05-27 - Initial Draft
 SAI-Version | TBD
-------------------------------------------------------------------------------

## List of Changes

| Version | Changes | Name | Date |
| ------- | ------- | ---- | ---- |
| 0.1 | Initial proposal for introducing IP Measurements as a new SAI feature. Defines the IPM object model, switch controls, session configuration, histogram profiles, statistics, notifications, examples and validation guidance. | Cisco | 2026-05-27 |

## 1.0 Introduction

This proposal introduces IP Measurements (IPM) as a new SAI
feature. IPM enables a SAI application to configure active measurement sessions
that generate or process measurement packets and expose path health information
through SAI objects, attributes, session statistics and notifications.

The proposed API is intended to support:

- active delay measurement;
- packet count correlation for loss measurement;
- alternate-marking based measurement intervals;
- delay histogram reporting;
- reusable histogram profiles;
- IP/UDP endpoint configuration for measurement packets;
- optional encapsulation such as SRv6;
- SAI stats APIs for retrieving IPM counters;
- SAI switch attributes for global IPM controls, capabilities and notifications.

The design goal is to define a hardware-neutral SAI model. The API describes
what an application wants to measure and how it wants results reported. It does
not expose a particular ASIC pipeline, timestamp representation, counter layout
or vendor SDK configuration model.

## 2.0 Terminology

| Term | Meaning |
| ---- | ------- |
| IPM | IP Measurements. A SAI feature for path measurement using probe or measurement packets. |
| IPM session | A measurement session identifying endpoints, probe behavior, histogram profile and statistics. |
| IPM histogram profile | A reusable logical object that defines exact delay histogram bin boundaries in nanoseconds. |
| Histogram bin | A delay range used to count packets whose measured delay falls within that range. |
| Alternate marking | A packet coloring mechanism that alternates between color 0 and color 1 over time to correlate counts between measurement periods. |
| Probe packet | A packet generated or processed by the IPM feature for active performance measurement. |

## 3.0 Requirements

The SAI IPM API should satisfy the following requirements:

1. Provide a hardware-neutral API for configuring measurement sessions.
2. Allow applications to configure session endpoints and probe packet parameters.
3. Allow applications to define exact histogram bin boundaries in nanoseconds.
4. Allow multiple sessions to reference the same reusable histogram profile,
   and allow different sessions to select profiles with different nanosecond boundary vectors.
5. Allow applications to discover both the maximum supported bin count and the
   number of bins used by a profile or session.
6. Guarantee that successful profile and session creation does not round,
   remove or otherwise alter the requested boundaries.
7. Expose counters through the SAI stats model.
8. Expose session state and alternate-marking changes through notifications.
9. Allow implementations to reject unsupported configurations through standard SAI status codes.

## 4.0 Non-Goals

This proposal does not define:

- a packet wire format for all possible measurement protocols;
- a timestamp encapsulation format;
- collector or telemetry export formats;
- a requirement that all implementations support the same delay precision;
- a requirement that all implementations support the same number of histogram bins;
- a requirement for independently programmable histogram hardware resources per
  session;
- a requirement that all independently valid profiles can be active
  simultaneously;
- approximate or silently rounded histogram boundaries;
- a requirement that alternate marking be independently configurable per session.

The API is intended to provide a common SAI control surface for IPM objects and
stats while leaving implementation-specific packet processing and timestamp
mechanisms to the adapter and hardware.

## 5.0 IPM Feature Overview

An IPM deployment typically includes:

1. A switch-wide IPM receive/transmit configuration.
2. Optional switch-wide alternate marking interval.
3. One or more histogram profiles.
4. One or more IPM sessions optionally referencing profiles.
5. Application reads of IPM statistics through session stats APIs.
6. Optional notifications when session state or alternate marking state changes.

## 6.0 IPM Measurement Model

### 6.1 Delay Measurement

For delay measurement, the implementation observes a raw delay value. The raw
delay is expressed in nanoseconds and is classified using the exact boundaries
of the histogram profile referenced by the session.

Successful profile creation guarantees that the configured boundaries
are exactly representable in isolation.
Successful session creation guarantees that they are realized
exactly with the resources active at that time.

### 6.2 Delay Histogram

A histogram profile is a logical object that defines the bins used to classify
delay values. The profile uses nanosecond boundaries so the SAI application can
configure bins in operator-visible units. A profile does not imply a dedicated
hardware histogram table; implementations may share or deduplicate native
histogram resources among sessions.

Example:

```text
SAI_IPM_HISTOGRAM_PROFILE_ATTR_BIN_BOUNDARIES_NS = [32768, 65536, 131072]
```

This creates four bins:

```text
bin0 = [0 ns, 32768 ns)
bin1 = [32768 ns, 65536 ns)
bin2 = [65536 ns, 131072 ns)
bin3 = [131072 ns, infinity)
```

The number of bins is:

```text
number_of_bins = number_of_boundaries + 1
```

Every boundary must be greater than zero, the list must be strictly increasing,
and each boundary must be realized exactly. An adapter rejects a profile that is
not exactly representable when used in isolation. Boundaries are immutable for
the profile lifetime so that the meaning of accumulated counters cannot change.

A profile may be valid by itself but unable to coexist with profiles already
used by active sessions because an implementation has shared histogram
resources. In that case, creating a session that references the profile fails
with `SAI_STATUS_INSUFFICIENT_RESOURCES` and leaves existing sessions unchanged.

### 6.3 Alternate Marking

Alternate marking provides a color bit that alternates between 0 and 1 at a
configured interval. This lets an application compare transmitted and received
packet counts over the same marking period.

Example:

```text
alternate marking interval = 10 seconds

period A, color 0:
    TX = 100000 packets
    RX = 99995 packets
    loss = 5 packets

period B, color 1:
    TX = 120000 packets
    RX = 120000 packets
    loss = 0 packets
```

The marking cadence is switch-scoped because it is a global measurement timing
function. It is not part of the histogram profile because histogram profiles
describe delay binning, not marking period timing.

## 7.0 SAI Object Model

This proposal introduces two IPM object types:

```c
SAI_OBJECT_TYPE_IPM_SESSION
SAI_OBJECT_TYPE_IPM_HISTOGRAM_PROFILE
```

### 7.1 IPM Session Object

The IPM session object represents one measurement session.

Primary use cases:

- configure a measurement flow;
- identify local and remote endpoints;
- select the VRF;
- configure probe UDP ports;
- configure DSCP and IPv6 flow label;
- configure SRv6 encapsulation if needed;
- configure probe transmit interval;
- configure liveness timeout;
- bind the session to a histogram profile;
- read session statistics through session stats APIs.

### 7.2 IPM Histogram Profile Object

The IPM histogram profile object represents reusable delay bin configuration.

Primary use cases:

- define delay histogram bins in nanoseconds;
- share the same binning policy across multiple sessions;
- decouple delay bin definitions from session endpoint configuration.

## 8.0 Changes to SAI Headers

### 8.1 Changes to saitypes.h

The standard SAI object type enum should allocate object IDs for:

```c
SAI_OBJECT_TYPE_IPM_SESSION
SAI_OBJECT_TYPE_IPM_HISTOGRAM_PROFILE
```

The exact numeric values are left to upstream SAI allocation.

### 8.2 Changes to sai.h

The standard SAI API enum should allocate:

```c
SAI_API_IPM
```

The standard SAI header include list should include:

```c
#include <saiipm.h>
```

### 8.3 New Header saiipm.h

The new feature should be defined in a new standard header:

```c
#if !defined (__SAIIPM_H_)
#define __SAIIPM_H_

#include <saitypes.h>

/* IPM enums, notifications, attributes, function typedefs, and API table */

#endif /* __SAIIPM_H_ */
```

### 8.4 Changes to saiswitch.h

Switch-scoped IPM controls and notification callback registration belong in
`inc/saiswitch.h`, following the same pattern as TWAMP, BFD, and ICMP Echo:

- `SAI_SWITCH_ATTR_IPM_ALTERNATE_MARKING_INTERVAL`
- `SAI_SWITCH_ATTR_IPM_SESSION_LIST`
- `SAI_SWITCH_ATTR_IPM_MAX_HISTOGRAM_BINS`
- `SAI_SWITCH_ATTR_IPM_LOCAL_UDP_PORT_RANGE`
- `SAI_SWITCH_ATTR_IPM_SESSION_STATE_CHANGE_NOTIFY`
- `SAI_SWITCH_ATTR_IPM_SESSION_ALTERNATE_MARKING_CHANGE_NOTIFY`

## 9.0 IPM Enums And Notification Types

### 9.1 IPM Encapsulation Type

```c
typedef enum _sai_ipm_encapsulation_type_t
{
    /**
     * @brief No additional encapsulation.
     *
     * Packet format is expected to be:
     * L2 header | IP header | UDP header | measurement payload
     */
    SAI_IPM_ENCAPSULATION_TYPE_NONE = 0,

    /**
     * @brief SRv6 encapsulation.
     *
     * Packet format is expected to be:
     * L2 header | IPv6 header | SRH | inner IP header | UDP header | measurement payload
     */
    SAI_IPM_ENCAPSULATION_TYPE_SRV6
} sai_ipm_encapsulation_type_t;
```

Use case:

- `NONE` is used for direct IP/UDP measurement packets.
- `SRV6` is used when measurement packets must be carried over an SRv6 path.

### 9.2 IPM Session State

```c
typedef enum _sai_ipm_session_state_t
{
    /** IPM session is down */
    SAI_IPM_SESSION_STATE_DOWN,

    /** IPM session is up */
    SAI_IPM_SESSION_STATE_UP
} sai_ipm_session_state_t;
```

Use case:

- allows the adapter to notify the application whether a measurement session is
  operational.

### 9.3 IPM Session State Notification

```c
typedef struct _sai_ipm_session_state_notification_t
{
    /**
     * @brief IPM session object ID
     *
     * @objects SAI_OBJECT_TYPE_IPM_SESSION
     */
    sai_object_id_t ipm_session_id;

    /** IPM session state */
    sai_ipm_session_state_t session_state;
} sai_ipm_session_state_notification_t;
```

Use case:

- report session up/down transition to the application.

### 9.4 Alternate Marking State

```c
typedef enum _sai_ipm_session_alternate_marking_t
{
    /** Alternate marking color is zero */
    SAI_IPM_SESSION_ALTERNATE_MARKING_ZERO,

    /** Alternate marking color is one */
    SAI_IPM_SESSION_ALTERNATE_MARKING_ONE
} sai_ipm_session_alternate_marking_t;
```

Use case:

- identify the current packet marking color for a session or notification.

### 9.5 Alternate Marking Notification

```c
typedef struct _sai_ipm_session_alternate_marking_notification_t
{
    /**
     * @brief IPM session object ID
     *
     * @objects SAI_OBJECT_TYPE_IPM_SESSION
     */
    sai_object_id_t ipm_session_id;

    /** New alternate marking color */
    sai_ipm_session_alternate_marking_t new_alternate_marking;
} sai_ipm_session_alternate_marking_notification_t;
```

Use case:

- report a marking period change to the application.

## 10.0 IPM Session Statistics

```c
typedef enum _sai_ipm_session_stat_t
{
    /**
     * @brief Packets transmitted with alternate marking value 0
     */
    SAI_IPM_SESSION_STAT_TX_PKTS_ALTERNATE_MARKING_0 = 0,

    /**
     * @brief Packets transmitted with alternate marking value 1
     */
    SAI_IPM_SESSION_STAT_TX_PKTS_ALTERNATE_MARKING_1,

    /**
     * @brief Packets received with alternate marking value 0
     */
    SAI_IPM_SESSION_STAT_RX_PKTS_ALTERNATE_MARKING_0,

    /**
     * @brief Packets received with alternate marking value 1
     */
    SAI_IPM_SESSION_STAT_RX_PKTS_ALTERNATE_MARKING_1,

    /**
     * @brief First histogram bin stat ID.
     *
     * Bin N is read as:
     * SAI_IPM_SESSION_STAT_RX_PKTS_BIN_RANGE_BASE + N
     */
    SAI_IPM_SESSION_STAT_RX_PKTS_BIN_RANGE_BASE = 0x00001000,

    /**
     * @brief Last histogram bin stat ID
     */
    SAI_IPM_SESSION_STAT_RX_PKTS_BIN_RANGE_END = 0x00001fff,

    /** Custom range base */
    SAI_IPM_SESSION_STAT_CUSTOM_RANGE_BASE = 0x10000000
} sai_ipm_session_stat_t;
```

Use cases:

- `TX_PKTS_ALTERNATE_MARKING_0` and `TX_PKTS_ALTERNATE_MARKING_1` count
  transmitted packets for each alternate-marking value.
- `RX_PKTS_ALTERNATE_MARKING_0` and `RX_PKTS_ALTERNATE_MARKING_1` count
  received packets for each alternate-marking value.
- `RX_PKTS_BIN_RANGE_BASE + N` is used to read receive packet count for histogram
  bin N.

Example:

```text
SAI_IPM_SESSION_ATTR_HISTOGRAM_NUMBER_OF_BINS = 4

Valid histogram stat IDs:
    BASE + 0
    BASE + 1
    BASE + 2
    BASE + 3

Invalid histogram stat ID:
    BASE + 4
```

## 11.0 IPM Session Attributes

```c
typedef enum _sai_ipm_session_attr_t
{
    SAI_IPM_SESSION_ATTR_START,

    /**
     * @brief IPM session ID
     *
     * Identifies the measurement session. The value may be encoded in probe
     * packets or used internally by the implementation to match measurement
     * packets to a session.
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan false
     */
    SAI_IPM_SESSION_ATTR_SESSION_ID = SAI_IPM_SESSION_ATTR_START,

    /**
     * @brief Virtual router object
     *
     * Selects the VRF used by the session endpoints and probe packets.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     */
    SAI_IPM_SESSION_ATTR_VIRTUAL_ROUTER,

    /**
     * @brief Local IP address
     *
     * IP address of the local IPM endpoint.
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPM_SESSION_ATTR_LOCAL_IP_ADDRESS,

    /**
     * @brief Remote IP address
     *
     * IP address of the remote IPM endpoint.
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPM_SESSION_ATTR_REMOTE_IP_ADDRESS,

    /**
     * @brief Local UDP port
     *
     * UDP port of the local IPM endpoint, used to identify received packets.
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPM_SESSION_ATTR_LOCAL_UDP_PORT,

    /**
     * @brief Remote UDP port
     *
     * UDP port of the remote IPM endpoint, used for transmitted packets.
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPM_SESSION_ATTR_REMOTE_UDP_PORT,

    /**
     * @brief DSCP value in IP header
     *
     * Allows measurement packets to be marked for a specific traffic class.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_IPM_SESSION_ATTR_DSCP,

    /**
     * @brief IPv6 flow label
     *
     * Allows IPv6 measurement packets to carry a configured flow label.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_IPM_SESSION_ATTR_FLOW_LABEL,

    /**
     * @brief Encapsulation type
     *
     * Selects the encapsulation type used by the IPM session.
     *
     * @type sai_ipm_encapsulation_type_t
     * @flags CREATE_ONLY
     * @default SAI_IPM_ENCAPSULATION_TYPE_NONE
     */
    SAI_IPM_SESSION_ATTR_IPM_ENCAPSULATION_TYPE,

    /**
     * @brief SRv6 SID list
     *
     * Required when SRv6 encapsulation is selected.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_SRV6_SIDLIST
     * @condition SAI_IPM_SESSION_ATTR_IPM_ENCAPSULATION_TYPE == SAI_IPM_ENCAPSULATION_TYPE_SRV6
     */
    SAI_IPM_SESSION_ATTR_SRV6_SIDLIST_ID,

    /**
     * @brief Source IP of outer encapsulating header
     *
     * Required when SRv6 encapsulation is selected.
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_IPM_SESSION_ATTR_IPM_ENCAPSULATION_TYPE == SAI_IPM_ENCAPSULATION_TYPE_SRV6
     */
    SAI_IPM_SESSION_ATTR_TUNNEL_SRC_IP,

    /**
     * @brief IPM histogram profile
     *
     * Histogram profile defining the effective delay boundaries, in
     * nanoseconds, used to classify delay samples for this session.
     *
     * @type sai_object_id_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_IPM_HISTOGRAM_PROFILE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_IPM_SESSION_ATTR_HISTOGRAM_PROFILE_ID,

    /**
     * @brief Interval of transmitting probe packets in microseconds
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 1000
     */
    SAI_IPM_SESSION_ATTR_TX_INTERVAL,

    /**
     * @brief Liveness detection timeout in microseconds
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 3000
     */
    SAI_IPM_SESSION_ATTR_LIVENESS_TIMEOUT,

    /**
     * @brief Number of statistics exposed by this IPM session
     *
     * This equals fixed counter count plus histogram bin count.
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     */
    SAI_IPM_SESSION_ATTR_NUMBER_OF_COUNTERS,

    /**
     * @brief Number of histogram bins
     *
     * Zero when no histogram profile is configured. Otherwise, this equals
     * #SAI_IPM_HISTOGRAM_PROFILE_ATTR_NUMBER_OF_BINS on the configured
     * profile.
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     */
    SAI_IPM_SESSION_ATTR_HISTOGRAM_NUMBER_OF_BINS,

    SAI_IPM_SESSION_ATTR_END,

    SAI_IPM_SESSION_ATTR_CUSTOM_RANGE_START = 0x10000000,
    SAI_IPM_SESSION_ATTR_CUSTOM_RANGE_END
} sai_ipm_session_attr_t;
```

### 11.1 Session Attribute Use Cases

| Attribute | Use case |
| --------- | -------- |
| `SESSION_ID` | Identify a measurement session in hardware or measurement packet processing. |
| `VIRTUAL_ROUTER` | Select the routing domain used for the measurement path. |
| `LOCAL_IP_ADDRESS` | Configure the local IPM endpoint address. |
| `REMOTE_IP_ADDRESS` | Configure the remote IPM endpoint address. |
| `LOCAL_UDP_PORT` | Configure the UDP port of the local IPM endpoint. |
| `REMOTE_UDP_PORT` | Configure the UDP port of the remote IPM endpoint. |
| `DSCP` | Mark probe packets for a desired QoS class. |
| `FLOW_LABEL` | Configure IPv6 flow label for measurement packets. |
| `IPM_ENCAPSULATION_TYPE` | Select plain IP/UDP measurement or SRv6 encapsulated measurement. |
| `SRV6_SIDLIST_ID` | Select SRv6 segment list when SRv6 encapsulation is used. |
| `TUNNEL_SRC_IP` | Configure outer source IP for SRv6 encapsulation. |
| `HISTOGRAM_PROFILE_ID` | Select nanosecond boundaries for the session. |
| `TX_INTERVAL` | Configure active probe packet transmit period in microseconds. |
| `LIVENESS_TIMEOUT` | Configure session liveness detection timeout in microseconds. |
| `NUMBER_OF_COUNTERS` | Reports the total number of fixed and histogram stats available from this session. |
| `HISTOGRAM_NUMBER_OF_BINS` | Reports zero without a profile; otherwise reports the referenced profile's bin count. |

## 12.0 IPM Histogram Profile Attributes

```c
typedef enum _sai_ipm_histogram_profile_attr_t
{
    SAI_IPM_HISTOGRAM_PROFILE_ATTR_START,

    /**
     * @brief Delay histogram bin boundaries in nanoseconds
     *
     * The list must be nonempty. Every boundary must be greater than zero and
     * the list must be strictly increasing. The boundaries define half-open
     * bins [0, boundary[0]), [boundary[0], boundary[1]), and so on. The final
     * bin has no finite upper boundary.
     *
     * The adapter must realize every boundary exactly. If the boundary list
     * cannot be represented exactly, profile creation must fail with
     * #SAI_STATUS_INVALID_ATTR_VALUE_0 offset by this attribute's index in the
     * create attribute list.
     *
     * @type sai_u32_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPM_HISTOGRAM_PROFILE_ATTR_BIN_BOUNDARIES_NS =
        SAI_IPM_HISTOGRAM_PROFILE_ATTR_START,

    /**
     * @brief Number of histogram bins
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     */
    SAI_IPM_HISTOGRAM_PROFILE_ATTR_NUMBER_OF_BINS,

    SAI_IPM_HISTOGRAM_PROFILE_ATTR_END,

    SAI_IPM_HISTOGRAM_PROFILE_ATTR_CUSTOM_RANGE_START = 0x10000000,
    SAI_IPM_HISTOGRAM_PROFILE_ATTR_CUSTOM_RANGE_END
} sai_ipm_histogram_profile_attr_t;
```

### 12.1 Histogram Profile Attribute Use Cases

| Attribute | Use case |
| --------- | -------- |
| `BIN_BOUNDARIES_NS` | Defines exact, immutable delay ranges in operator-visible nanosecond units. |
| `NUMBER_OF_BINS` | Lets the application discover the number of valid histogram bin stat IDs. |

Example:

```text
BIN_BOUNDARIES_NS = [32768, 65536, 131072]

bin0 = [0 ns, 32768 ns)
bin1 = [32768 ns, 65536 ns)
bin2 = [65536 ns, 131072 ns)
bin3 = [131072 ns, infinity)

NUMBER_OF_BINS = 4
```

## 13.0 Switch Attributes

IPM requires a small number of switch-scoped controls, capabilities and
notification registrations.

### 13.1 Alternate Marking Interval

```c
/**
 * @brief Alternate marking interval time in seconds
 *
 * Zero disables alternate marking. Nonzero configures the global IPM
 * loss-measurement marking cadence in seconds.
 *
 * @type sai_uint32_t
 * @flags CREATE_AND_SET
 * @default 0
 */
SAI_SWITCH_ATTR_IPM_ALTERNATE_MARKING_INTERVAL
```

Use case:

- configure how often the alternate marking color changes.

Example:

```text
SAI_SWITCH_ATTR_IPM_ALTERNATE_MARKING_INTERVAL = 10
```

The marking color changes every 10 seconds.

### 13.2 IPM Session List

```c
/**
 * @brief IPM sessions instantiated on the switch
 *
 * @type sai_object_list_t
 * @flags READ_ONLY
 * @objects SAI_OBJECT_TYPE_IPM_SESSION
 */
SAI_SWITCH_ATTR_IPM_SESSION_LIST
```

Use case:

- allow applications to enumerate IPM sessions instantiated on the switch.

### 13.3 Maximum IPM Histogram Bins

```c
/**
 * @brief Maximum number of bins in a single IPM histogram profile
 *
 * @type sai_uint16_t
 * @flags READ_ONLY
 * @isvlan false
 */
SAI_SWITCH_ATTR_IPM_MAX_HISTOGRAM_BINS
```

Use case:

- allow applications to discover the maximum boundary-list count before
  creating a histogram profile. For a nonzero returned value, the maximum
  number of boundaries is one less than this value.

### 13.4 IPM Local UDP Port Range

```c
/**
 * @brief IPM local UDP port range
 *
 * Switch-wide inclusive range of local UDP ports used for IPM receive processing.
 * The default zero range leaves IPM receive ports unconfigured.
 *
 * @type sai_u16_range_t
 * @flags CREATE_AND_SET
 * @default 0
 */
SAI_SWITCH_ATTR_IPM_LOCAL_UDP_PORT_RANGE
```

Use case:

- configure the switch-wide local UDP port range used for IPM receive processing.

### 13.5 IPM Session State Notification

```c
/**
 * @brief IPM session state change notification
 *
 * @type sai_pointer_t sai_ipm_session_state_change_notification_fn
 * @flags CREATE_AND_SET
 * @default NULL
 */
SAI_SWITCH_ATTR_IPM_SESSION_STATE_CHANGE_NOTIFY
```

Use case:

- notify the application when an IPM session transitions between down and up.

### 13.6 IPM Alternate Marking Notification

```c
/**
 * @brief IPM session alternate marking change notification
 *
 * @type sai_pointer_t sai_ipm_session_alternate_marking_change_notification_fn
 * @flags CREATE_AND_SET
 * @default NULL
 */
SAI_SWITCH_ATTR_IPM_SESSION_ALTERNATE_MARKING_CHANGE_NOTIFY
```

Use case:

- notify the application when the alternate marking color changes.

## 14.0 IPM Function Typedefs And API Table

### 14.1 Histogram Profile Functions

```c
typedef sai_status_t (*sai_create_ipm_histogram_profile_fn)(
        _Out_ sai_object_id_t *ipm_histogram_profile_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_ipm_histogram_profile_fn)(
        _In_ sai_object_id_t ipm_histogram_profile_id);

typedef sai_status_t (*sai_set_ipm_histogram_profile_attribute_fn)(
        _In_ sai_object_id_t ipm_histogram_profile_id,
        _In_ const sai_attribute_t *attr);

typedef sai_status_t (*sai_get_ipm_histogram_profile_attribute_fn)(
        _In_ sai_object_id_t ipm_histogram_profile_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);
```

### 14.2 Session Functions

```c
typedef sai_status_t (*sai_create_ipm_session_fn)(
        _Out_ sai_object_id_t *ipm_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_ipm_session_fn)(
        _In_ sai_object_id_t ipm_session_id);

typedef sai_status_t (*sai_set_ipm_session_attribute_fn)(
        _In_ sai_object_id_t ipm_session_id,
        _In_ const sai_attribute_t *attr);

typedef sai_status_t (*sai_get_ipm_session_attribute_fn)(
        _In_ sai_object_id_t ipm_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef sai_status_t (*sai_get_ipm_session_stats_fn)(
        _In_ sai_object_id_t ipm_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

typedef sai_status_t (*sai_get_ipm_session_stats_ext_fn)(
        _In_ sai_object_id_t ipm_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

typedef sai_status_t (*sai_clear_ipm_session_stats_fn)(
        _In_ sai_object_id_t ipm_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);
```

### 14.3 Notification Function Types

```c
typedef void (*sai_ipm_session_state_change_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_ipm_session_state_notification_t *data);

typedef void (*sai_ipm_session_alternate_marking_change_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_ipm_session_alternate_marking_notification_t *data);
```

### 14.4 IPM API Table

```c
typedef struct _sai_ipm_api_t
{
    sai_create_ipm_session_fn                   create_ipm_session;
    sai_remove_ipm_session_fn                   remove_ipm_session;
    sai_set_ipm_session_attribute_fn            set_ipm_session_attribute;
    sai_get_ipm_session_attribute_fn            get_ipm_session_attribute;
    sai_get_ipm_session_stats_fn                get_ipm_session_stats;
    sai_get_ipm_session_stats_ext_fn            get_ipm_session_stats_ext;
    sai_clear_ipm_session_stats_fn              clear_ipm_session_stats;

    sai_create_ipm_histogram_profile_fn         create_ipm_histogram_profile;
    sai_remove_ipm_histogram_profile_fn         remove_ipm_histogram_profile;
    sai_set_ipm_histogram_profile_attribute_fn  set_ipm_histogram_profile_attribute;
    sai_get_ipm_histogram_profile_attribute_fn  get_ipm_histogram_profile_attribute;
} sai_ipm_api_t;
```

## 15.0 Configuration Examples

### 15.1 Get IPM API Table

```c
sai_ipm_api_t *ipm_api = NULL;
sai_status_t status;

status = sai_api_query(SAI_API_IPM, (void **)&ipm_api);
if (status != SAI_STATUS_SUCCESS) {
    /* IPM is not available */
}
```

### 15.2 Configure Alternate Marking

```c
sai_attribute_t attr;

attr.id = SAI_SWITCH_ATTR_IPM_ALTERNATE_MARKING_INTERVAL;
attr.value.u32 = 10; /* seconds */

status = sai_switch_api->set_switch_attribute(switch_id, &attr);
```

This configures alternate marking color changes every 10 seconds.

### 15.3 Query Histogram Capacity

```c
sai_attribute_t max_bins_attr;

max_bins_attr.id = SAI_SWITCH_ATTR_IPM_MAX_HISTOGRAM_BINS;
status = sai_switch_api->get_switch_attribute(
    switch_id,
    1,
    &max_bins_attr);
```

If `max_bins_attr.value.u16` is zero, the switch does not support IPM
histograms. Otherwise, the maximum accepted boundary-list count is one less
than the returned bin count.

### 15.4 Create Histogram Profile

```c
sai_object_id_t profile_id;
sai_uint32_t boundaries_ns[] = {32768, 65536, 131072};
sai_attribute_t attr;

attr.id = SAI_IPM_HISTOGRAM_PROFILE_ATTR_BIN_BOUNDARIES_NS;
attr.value.u32list.count = 3;
attr.value.u32list.list = boundaries_ns;

status = ipm_api->create_ipm_histogram_profile(
    &profile_id,
    switch_id,
    1,
    &attr);
```

Profile creation succeeds only when the adapter can preserve every configured
boundary exactly.

This creates:

```text
bin0 = [0 ns, 32768 ns)
bin1 = [32768 ns, 65536 ns)
bin2 = [65536 ns, 131072 ns)
bin3 = [131072 ns, infinity)
```

### 15.5 Create IPM Session

```c
sai_object_id_t session_id;
sai_attribute_t attrs[9];
uint32_t attr_count = 0;

attrs[attr_count].id = SAI_IPM_SESSION_ATTR_SESSION_ID;
attrs[attr_count].value.u16 = 100;
attr_count++;

attrs[attr_count].id = SAI_IPM_SESSION_ATTR_VIRTUAL_ROUTER;
attrs[attr_count].value.oid = vr_id;
attr_count++;

attrs[attr_count].id = SAI_IPM_SESSION_ATTR_LOCAL_IP_ADDRESS;
attrs[attr_count].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV6;
/* set attrs[attr_count].value.ipaddr.addr.ip6 */
attr_count++;

attrs[attr_count].id = SAI_IPM_SESSION_ATTR_REMOTE_IP_ADDRESS;
attrs[attr_count].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV6;
/* set attrs[attr_count].value.ipaddr.addr.ip6 */
attr_count++;

attrs[attr_count].id = SAI_IPM_SESSION_ATTR_LOCAL_UDP_PORT;
attrs[attr_count].value.u16 = 50000;
attr_count++;

attrs[attr_count].id = SAI_IPM_SESSION_ATTR_REMOTE_UDP_PORT;
attrs[attr_count].value.u16 = 862;
attr_count++;

attrs[attr_count].id = SAI_IPM_SESSION_ATTR_HISTOGRAM_PROFILE_ID;
attrs[attr_count].value.oid = profile_id;
attr_count++;

attrs[attr_count].id = SAI_IPM_SESSION_ATTR_TX_INTERVAL;
attrs[attr_count].value.u32 = 1000; /* microseconds */
attr_count++;

attrs[attr_count].id = SAI_IPM_SESSION_ATTR_LIVENESS_TIMEOUT;
attrs[attr_count].value.u32 = 3000; /* microseconds */
attr_count++;

status = ipm_api->create_ipm_session(
    &session_id,
    switch_id,
    attr_count,
    attrs);
```

### 15.6 Read Fixed Session Statistics

```c
sai_stat_id_t stat_ids[4];
uint64_t counters[4];

stat_ids[0] = SAI_IPM_SESSION_STAT_TX_PKTS_ALTERNATE_MARKING_0;
stat_ids[1] = SAI_IPM_SESSION_STAT_TX_PKTS_ALTERNATE_MARKING_1;
stat_ids[2] = SAI_IPM_SESSION_STAT_RX_PKTS_ALTERNATE_MARKING_0;
stat_ids[3] = SAI_IPM_SESSION_STAT_RX_PKTS_ALTERNATE_MARKING_1;

status = ipm_api->get_ipm_session_stats(
    session_id,
    4,
    stat_ids,
    counters);
```

### 15.7 Read Histogram Bin Statistics

```c
sai_attribute_t attr;
sai_uint16_t bin_count;

attr.id = SAI_IPM_SESSION_ATTR_HISTOGRAM_NUMBER_OF_BINS;
status = ipm_api->get_ipm_session_attribute(session_id, 1, &attr);
bin_count = attr.value.u16;

for (uint32_t i = 0; i < bin_count; i++) {
    stat_ids[i] = SAI_IPM_SESSION_STAT_RX_PKTS_BIN_RANGE_BASE + i;
}

status = ipm_api->get_ipm_session_stats_ext(
    session_id,
    bin_count,
    stat_ids,
    SAI_STATS_MODE_READ,
    counters);
```

## 16.0 Numeric And Resource Examples

### 16.1 Exact Histogram Bin Classification

For this profile:

```text
BIN_BOUNDARIES_NS = [32768, 65536, 131072]
```

the bins are:

```text
bin0 = [0 ns, 32768 ns)
bin1 = [32768 ns, 65536 ns)
bin2 = [65536 ns, 131072 ns)
bin3 = [131072 ns, infinity)
```

Classification at and around the boundaries is exact:

| Measured delay | Bin |
| -------------- | --- |
| `0 ns` | `bin0` |
| `32767 ns` | `bin0` |
| `32768 ns` | `bin1` |
| `100000 ns` | `bin2` |
| `131072 ns` | `bin3` |

A delay equal to a boundary belongs to the higher-numbered bin.

## 17.0 Implementation Guidance

SAI adapters may support different levels of IPM capability. The standard API
remains hardware-neutral by making effective nanosecond boundaries normative
and keeping native representations private.

Implementation guidance:

- validate all mandatory attributes on create;
- reject an empty boundary list, zero-valued boundaries and lists that are not
  strictly increasing;
- reject a boundary list when `count + 1` exceeds
  `SAI_SWITCH_ATTR_IPM_MAX_HISTOGRAM_BINS`;
- determine whether every requested boundary is exactly representable when the
  profile is created, without rounding, dropping or inserting boundaries;
- reject attempts to remove histogram profiles still referenced by sessions;
- translate SAI stat range IDs to backend counter indexes internally;
- report zero through `SAI_IPM_SESSION_ATTR_HISTOGRAM_NUMBER_OF_BINS` when no
  profile is attached; otherwise report the referenced profile's boundary
  count plus one;
- use SAI stats modes for read, read-and-clear, and clear semantics.

## 18.0 Error Handling

Recommended behavior:

| Condition | Recommended status |
| --------- | ------------------ |
| Missing mandatory session attribute | `SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING` |
| Empty boundary list, zero boundary or non-strict ordering | Indexed invalid attribute value status for `BIN_BOUNDARIES_NS` |
| Boundary count exceeds the advertised maximum | Indexed invalid attribute value status for `BIN_BOUNDARIES_NS` |
| Boundary list cannot be represented exactly | Indexed invalid attribute value status for `BIN_BOUNDARIES_NS` |
| Attempt to set create-only histogram boundaries | Indexed invalid attribute status for `BIN_BOUNDARIES_NS` |
| Histogram profile object type mismatch | Indexed invalid attribute value status for `HISTOGRAM_PROFILE_ID` |
| Histogram profile in use and removed | `SAI_STATUS_OBJECT_IN_USE` |
| Histogram stat bin index out of range | `SAI_STATUS_INVALID_PARAMETER` |

## 19.0 Test Plan

### 19.1 Object Operation Tests

Tests should cover:

- create, remove, and get histogram profiles;
- reject setting the create-only `BIN_BOUNDARIES_NS` attribute;
- create, remove, set, and get session;
- get switch IPM session list;
- get `SAI_SWITCH_ATTR_IPM_MAX_HISTOGRAM_BINS`;
- set and get switch alternate marking interval;
- set and get the switch IPM local UDP port range.

### 19.2 Attribute Validation Tests

Tests should cover:

- missing mandatory session attributes;
- invalid object type for `HISTOGRAM_PROFILE_ID`;
- empty boundary list;
- zero, duplicate and descending boundaries;
- boundary count above `SAI_SWITCH_ATTR_IPM_MAX_HISTOGRAM_BINS - 1`;
- attempted modification of create-only boundaries;
- profile boundary count resulting in `count + 1` bins;
- session bin count matching the referenced profile;
- zero session bins when no profile is referenced;
- remove profile while referenced by session.

### 19.3 Stats Tests

Tests should cover:

- reading fixed alternate-marking session statistics;
- reading histogram bin statistics using `RX_PKTS_BIN_RANGE_BASE + N`;
- rejecting out-of-range histogram stat IDs;
- read mode;
- read-and-clear mode;
- clear session stats API.

### 19.4 Functional Tests

Functional tests should verify end-to-end behavior:

- configure the IPM local UDP receive-port range;
- configure alternate marking interval;
- create histogram profile and session;
- share one profile across multiple sessions;
- transmit or emulate measurement packets;
- verify fixed TX/RX session statistics advance;
- verify packets are classified into expected histogram bins
- verify alternate marking period changes are reflected in statistics or
  notifications;
- verify session statistics clear correctly.

## 20.0 Summary

This proposal introduces IPM as a new SAI feature using a clean object model:

- `SAI_OBJECT_TYPE_IPM_SESSION` configures measurement sessions and exposes statistics;
- `SAI_OBJECT_TYPE_IPM_HISTOGRAM_PROFILE` defines reusable, immutable and exact
  delay histogram boundaries in nanoseconds;
- switch attributes configure global IPM controls and notifications and report
  maximum histogram bin capacity.

The API defines measurement semantics, attributes, session statistics,
and use cases while allowing each adapter to map the model to its hardware capabilities.
