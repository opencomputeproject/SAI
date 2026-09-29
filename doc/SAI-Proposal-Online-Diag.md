# SAI Online Diagnostics
-------------------------------------------------------------------------------
 Title       | SAI Online Diagnostics
-------------|-----------------------------------------------------------------
 Authors     | Cisco
 Status      | Draft
 Type        | Standards track
 Created     | 2026-06-30 - Initial Draft
 SAI-Version | TBD
-------------------------------------------------------------------------------

## Overview

Online Diagnostics is used to validate switch datapath health while the switch
remains in service. The diagnostic function periodically injects
implementation-controlled packets, validates that expected packets are received,
and reports aggregate diagnostic results to the SAI application.

Online Diagnostics is modeled as a switch-scoped capability because the
diagnostic function validates device-wide internal connectivity and datapath
health rather than a customer data flow or a user-created forwarding object.

The design is hardware-neutral. SAI describes how an application enables
diagnostics and retrieves results. It does not standardize vendor-specific
packet injection mechanisms, packet trap programming, diagnostic resource
allocation, internal path selection, or ASIC counter implementation.

This proposal adds switch attributes for configuring Online Diagnostics and
switch statistics for reporting aggregate diagnostic results. Diagnostic events
are reported through the existing switch health event callback by extending
`sai_health_data_type_t` and `sai_health_data_t`. It does not add a new callback,
SAI object type, API table, or custom range.

Consistent with SAI conventions, the implementation reports diagnostic events to
the application and does not autonomously modify datapath state. The application
is responsible for any corrective action, such as raising an alarm, isolating or
draining the device, or triggering failover.

## Motivation

Existing SAI APIs expose many port, queue, buffer, and switch counters, but they
do not provide a common active health check that periodically injects known test
packets and validates that they return correctly. This leaves NOS applications
with platform-specific mechanisms for detecting internal packet loss, detecting
packet corruption on internal datapath paths, applying consistent diagnostic
intervals and thresholds, and collecting diagnostic counters.

Standardizing Online Diagnostics allows SAI applications to enable comparable
health monitoring across vendors while preserving implementation flexibility.

## Technical Specification

### New SAI switch attributes

New attributes are added into `sai_switch_attr_t`:

```C
    /**
     * @brief Enable or disable Online Diagnostics
     *
     * TRUE - Enable Online Diagnostics.
     * FALSE - Disable Online Diagnostics.
     *
     * When enabled, the SAI implementation starts supported internal diagnostic
     * paths and periodically injects diagnostic packets. When disabled, the
     * implementation stops diagnostic paths and releases resources that are not
     * required while diagnostics are disabled.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_ONLINE_DIAG_ENABLE,

    /**
     * @brief Online Diagnostics packet injection interval in milliseconds
     *
     * Specifies the interval between diagnostic probe cycles. A value of zero is
     * invalid.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 5000
     */
    SAI_SWITCH_ATTR_ONLINE_DIAG_INTERVAL_MS,

    /**
     * @brief Online Diagnostics consecutive loss threshold
     *
     * Number of consecutive packet loss observations before the implementation
     * records a diagnostic loss condition.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 1
     */
    SAI_SWITCH_ATTR_ONLINE_DIAG_LOSS_THRESHOLD,

    /**
     * @brief Online Diagnostics payload patterns
     *
     * List of 32-bit payload patterns used to fill diagnostic packets. The
     * implementation may cycle through the configured patterns during probing.
     * An empty list selects implementation-defined defaults.
     *
     * @type sai_u32_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SWITCH_ATTR_ONLINE_DIAG_PAYLOAD_PATTERNS,

    /**
     * @brief Online Diagnostics packet lengths
     *
     * List of diagnostic packet lengths in bytes. The implementation may cycle
     * through the configured lengths during probing. An empty list selects
     * implementation-defined defaults.
     *
     * The recommended valid range is 64 to 9000 bytes. Implementations may
     * reject unsupported values with #SAI_STATUS_INVALID_PARAMETER.
     *
     * @type sai_u32_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SWITCH_ATTR_ONLINE_DIAG_PACKET_LENGTHS,
```

### New SAI switch statistics

New statistics are added into `sai_switch_stat_t`:

```C
    /** Get Online Diagnostics transmitted packets count */
    SAI_SWITCH_STAT_ONLINE_DIAG_TX_PACKETS,

    /** Get Online Diagnostics received packets count */
    SAI_SWITCH_STAT_ONLINE_DIAG_RX_PACKETS,

    /** Get Online Diagnostics packet loss count */
    SAI_SWITCH_STAT_ONLINE_DIAG_PACKET_LOSS,

    /** Get Online Diagnostics packet corruption count */
    SAI_SWITCH_STAT_ONLINE_DIAG_PACKET_CORRUPTIONS,
```

### Existing switch health event extension

Online Diagnostics reuses
`SAI_SWITCH_ATTR_SWITCH_ASIC_SDK_HEALTH_EVENT_NOTIFY` and
`sai_switch_asic_sdk_health_event_notification_fn` for asynchronous event
reporting. A new health data type and typed union member are added in
`saitypes.h`. No new callback or callback attribute is introduced.

```C
/**
 * @brief Online Diagnostics event type
 */
typedef enum _sai_switch_online_diag_event_type_t
{
    /** Diagnostic loss condition detected */
    SAI_SWITCH_ONLINE_DIAG_EVENT_TYPE_LOSS_DETECTED,

    /** Diagnostic packet corruption detected */
    SAI_SWITCH_ONLINE_DIAG_EVENT_TYPE_CORRUPTION_DETECTED,

    /** Diagnostics returned to healthy state */
    SAI_SWITCH_ONLINE_DIAG_EVENT_TYPE_RECOVERED

} sai_switch_online_diag_event_type_t;

/**
 * @brief Online Diagnostics switch health event data
 */
typedef struct _sai_online_diag_health_data_t
{
    /** Online Diagnostics event type */
    sai_switch_online_diag_event_type_t event_type;

} sai_online_diag_health_data_t;

typedef enum _sai_health_data_type_t
{
    /* Existing values are omitted. */

    /** Online Diagnostics health data type */
    SAI_HEALTH_DATA_TYPE_ONLINE_DIAG
} sai_health_data_type_t;

typedef union _sai_health_data_t
{
    /* Existing members are omitted. */

    /** @validonly data_type == SAI_HEALTH_DATA_TYPE_ONLINE_DIAG */
    sai_online_diag_health_data_t online_diag;
} sai_health_data_t;
```

The new `sai_online_diag_health_data_t` member is smaller than the existing SER
health data member. Adding it therefore does not increase the size of
`sai_health_data_t` or change the calling convention of the existing switch
health callback.

### API behavior

`SAI_SWITCH_ATTR_ONLINE_DIAG_ENABLE` starts supported diagnostic paths when set
to `true` and stops diagnostic paths when set to `false`.

`SAI_SWITCH_ATTR_ONLINE_DIAG_INTERVAL_MS` controls the interval between
diagnostic probe cycles. The default value is 5000 milliseconds. A value of zero
is invalid.

`SAI_SWITCH_ATTR_ONLINE_DIAG_LOSS_THRESHOLD` controls how many consecutive loss
observations are required before the implementation records a diagnostic loss
condition.

`SAI_SWITCH_ATTR_ONLINE_DIAG_PAYLOAD_PATTERNS` provides a list of 32-bit
patterns used to fill diagnostic probe payloads. The implementation may cycle
through the configured patterns during probing. An empty list selects
implementation-defined defaults. On get, this attribute follows the standard
`sai_u32_list_t` convention. If the application-provided list is too small, the
implementation returns `SAI_STATUS_BUFFER_OVERFLOW` and updates the list count
with the required number of entries.

`SAI_SWITCH_ATTR_ONLINE_DIAG_PACKET_LENGTHS` provides a list of diagnostic
packet lengths in bytes. The implementation may cycle through the configured
lengths during probing. An empty list selects implementation-defined defaults.
The recommended valid range is 64 to 9000 bytes. Implementations may further
restrict this range based on the minimum diagnostic header size or maximum
supported packet size. On get, this attribute follows the standard
`sai_u32_list_t` convention. If the application-provided list is too small, the
implementation returns `SAI_STATUS_BUFFER_OVERFLOW` and updates the list count
with the required number of entries.

Online Diagnostics events use the existing
`sai_switch_asic_sdk_health_event_notification_fn`. For these events,
`data.data_type` is `SAI_HEALTH_DATA_TYPE_ONLINE_DIAG` and
`data.data.online_diag.event_type` identifies loss, corruption, or recovery. The
callback's existing `switch_id`, `timestamp`, `severity`, and `category`
parameters provide the event context without duplicating those fields in the
Online Diagnostics health data.

Online Diagnostics datapath failures use
`SAI_SWITCH_ASIC_SDK_HEALTH_CATEGORY_ASIC_HW`. Loss and corruption events are
reported with warning or fatal severity according to their impact. Recovery is
reported with notice severity. Applications that require these events register
the existing switch health callback and include the ASIC hardware category in
the corresponding warning, fatal, and notice health-category registration
attributes.

When the existing switch health callback is `NULL`, or the application has not
registered the ASIC hardware category for the event severity, the implementation
does not report the event asynchronously. Diagnostic results remain available
through the switch statistics. Consistent with SAI conventions, the
implementation only reports the event; the application is responsible for any
corrective action.

Counter reads use the existing SAI switch statistics mechanisms and follow
standard SAI read and read-and-clear behavior where supported.

Implementations should use standard SAI status codes:

- `SAI_STATUS_SUCCESS` when the operation succeeds.
- `SAI_STATUS_NOT_SUPPORTED` when Online Diagnostics or a diagnostic path is not
  supported.
- `SAI_STATUS_INVALID_PARAMETER` when an interval, list size, or packet length is
  invalid.
- `SAI_STATUS_INSUFFICIENT_RESOURCES` when implementation resources are not
  available.
- `SAI_STATUS_FAILURE` for implementation-specific failures that do not map to a
  more specific status code.

Applications should use existing SAI capability discovery mechanisms to
determine whether Online Diagnostics attributes and counters are supported.

### Diagnostic coverage

This proposal models Online Diagnostics as a single switch-scoped health
monitoring service, not as a generic diagnostic test runner. Diagnostic probes
provide two standard result classes:

- Packet delivery: a diagnostic packet injected into an
  implementation-selected internal path is expected to reach its observation
  point. A missing packet contributes to the packet-loss statistic and may
  generate a loss event.
- Packet integrity: a received diagnostic packet is validated against its known
  contents. A mismatch contributes to the packet-corruption statistic and may
  generate a corruption event.

The same probe can perform both checks, so an implementation is not required to
represent them as separate tests. Setting
`SAI_SWITCH_ATTR_ONLINE_DIAG_ENABLE` to `true` requests the implementation to
run its complete supported diagnostic suite over its supported internal paths.
The API does not expose a per-test or per-path list or selection mask, and the
SAI application cannot independently enable or disable an internal test or
path. This gives the enabled state and aggregate statistics consistent meaning
without exposing hardware-specific test organization or internal path topology.

## API flexibility

The proposed API is intentionally limited to switch-level configuration and
aggregate telemetry:

- The diagnostic packet format, injection mechanism, and internal path selection
  are implementation specific.
- Implementations must keep diagnostic packet generation and validation isolated
  from normal user packet forwarding behavior. Diagnostic packets must not
  escape onto front-panel networks.
- SAI applications should configure diagnostic intervals conservatively. Very
  short intervals may increase CPU, DMA, counter, or implementation-specific
  diagnostic resource load.
- Empty payload pattern and packet length lists select implementation-defined
  defaults.
- Implementations that do not support Online Diagnostics can advertise lack of
  support through normal SAI capability discovery and return
  `SAI_STATUS_NOT_SUPPORTED` for the new attributes or counters.
- Online Diagnostics events reuse the switch health event channel. Applications
  can distinguish them from other switch health events using
  `SAI_HEALTH_DATA_TYPE_ONLINE_DIAG`.
- Vendors may expose richer debug or per-path status through vendor extensions,
  but such mechanisms are outside this standard proposal.

## Usage Example

### Register for Online Diagnostics switch health events

```C
void switch_health_event_cb(
        sai_object_id_t switch_id,
        sai_switch_asic_sdk_health_severity_t severity,
        sai_timespec_t timestamp,
        sai_switch_asic_sdk_health_category_t category,
        sai_switch_health_data_t data,
        const sai_u8_list_t description)
{
    if (data.data_type == SAI_HEALTH_DATA_TYPE_ONLINE_DIAG)
    {
        sai_switch_online_diag_event_type_t event_type =
            data.data.online_diag.event_type;

        /* Application decides on corrective action based on event_type. */
    }
}

sai_attribute_t attr;
sai_status_t status;
sai_int32_t asic_hw_category =
    SAI_SWITCH_ASIC_SDK_HEALTH_CATEGORY_ASIC_HW;

attr.id = SAI_SWITCH_ATTR_SWITCH_ASIC_SDK_HEALTH_EVENT_NOTIFY;
attr.value.ptr = (void *)switch_health_event_cb;
status = sai_switch_api->set_switch_attribute(switch_id, &attr);

attr.value.s32list.count = 1;
attr.value.s32list.list = &asic_hw_category;

attr.id = SAI_SWITCH_ATTR_REG_FATAL_SWITCH_ASIC_SDK_HEALTH_CATEGORY;
status = sai_switch_api->set_switch_attribute(switch_id, &attr);

attr.id = SAI_SWITCH_ATTR_REG_WARNING_SWITCH_ASIC_SDK_HEALTH_CATEGORY;
status = sai_switch_api->set_switch_attribute(switch_id, &attr);

attr.id = SAI_SWITCH_ATTR_REG_NOTICE_SWITCH_ASIC_SDK_HEALTH_CATEGORY;
status = sai_switch_api->set_switch_attribute(switch_id, &attr);
```

### Enable Online Diagnostics

```C
sai_attribute_t attr;
sai_status_t status;

attr.id = SAI_SWITCH_ATTR_ONLINE_DIAG_INTERVAL_MS;
attr.value.u64 = 5000;
status = sai_switch_api->set_switch_attribute(switch_id, &attr);

attr.id = SAI_SWITCH_ATTR_ONLINE_DIAG_LOSS_THRESHOLD;
attr.value.u64 = 3;
status = sai_switch_api->set_switch_attribute(switch_id, &attr);

attr.id = SAI_SWITCH_ATTR_ONLINE_DIAG_ENABLE;
attr.value.booldata = true;
status = sai_switch_api->set_switch_attribute(switch_id, &attr);
```

### Read Online Diagnostics counters

```C
sai_stat_id_t counter_ids[] = {
    SAI_SWITCH_STAT_ONLINE_DIAG_TX_PACKETS,
    SAI_SWITCH_STAT_ONLINE_DIAG_RX_PACKETS,
    SAI_SWITCH_STAT_ONLINE_DIAG_PACKET_LOSS,
    SAI_SWITCH_STAT_ONLINE_DIAG_PACKET_CORRUPTIONS,
};
uint64_t counters[sizeof(counter_ids) / sizeof(counter_ids[0])];
sai_status_t status;

status = sai_switch_api->get_switch_stats_ext(
        switch_id,
        sizeof(counter_ids) / sizeof(counter_ids[0]),
        counter_ids,
        SAI_STATS_MODE_READ,
        counters);
```

## Backward Compatibility

This proposal is backward compatible with existing SAI applications.

- No existing attribute, statistic, enum, object type, or API method is removed.
- The existing switch health event callback is reused without changing its
  signature or the size of `sai_switch_health_data_t`.
- Online Diagnostics is disabled by default.
- Applications that do not set the new attributes observe no change in switch
  behavior.
- Existing switch create, set, get, stats, and clear-stats semantics are
  preserved.

## Non-goals

This proposal does not define:

- a required diagnostic packet wire format;
- a required source or destination MAC address for diagnostic packets;
- required use of a CPU path, recycle path, or any specific hardware block;
- a per-port, per-slice, or per-internal-path object model;
- portable enumeration, description, or independent selection of
  implementation-specific diagnostic tests or paths;
- autonomous in-SAI remediation actions, such as automatically disabling ports
  or taking the switch out of service in response to a diagnostic event;
- test-only error injection controls;
- a standard CLI or RPC interface for diagnostics.
