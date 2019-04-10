# Generic SAI Counters and Resources

Title       | Critical Resource Monitor
------------|----------------
Authors     | Mellanox
Status      | In review
Type        | Standards track
Created     | 04/10/2019
SAI-Version | 1.4

## Overview
Defining counters explicitly per object type imposes restrictions to their usage.
* Objects that are capable of reading packet counters are predefined, e. g.:
```
typedef sai_status_t (*sai_get_port_stats_fn)(
        _In_ sai_object_id_t port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);
```
* Counter types per object are also predefined, e. g.:
```
{
    /** SAI port stat if in octets */
    SAI_PORT_STAT_IF_IN_OCTETS,

    /** SAI port stat if in ucast pkts */
    SAI_PORT_STAT_IF_IN_UCAST_PKTS,
…
```
* Types of resources for accounting are predefined, e. g.:
```
SAI_SWITCH_ATTR_AVAILABLE_FDB_ENTRY,
SAI_SWITCH_ATTR_AVAILABLE_ACL_TABLE,
…
```
This kind of a counter/resource definition results in a following implementation issues:
* Some implementations might not support some statistics types for a given object.
* Some implementations might not support gathering statistics at all for a given object.
* Some implementations might not support counting available resources for a given object.
* Some HW may support counters not defined by SAI.
* Some HW may support resource counting for objects that SAI doesn't.

So the counters model defined by SAI and counters model defined by HW look the following way:

```
+-----------------------------------------+
|                                         |
|                                         |
|                                         |
|       Counters that SAI defines         |
|                                         |
|                                         |
|                                         |
|                  +------------------------------------------+
|                  |                      |                   |
|                  |                      |                   |
|                  |                      |                   |
|                  |  What the users get  |                   |
|                  |                      |                   |
|                  |                      |                   |
|                  |                      |                   |
+-----------------------------------------+                   |
                   |                                          |
                   |                                          |
                   |                                          |
                   |         Counters that HW supports        |
                   |                                          |
                   |                                          |
                   |                                          |
                   +------------------------------------------+
```
## Generic mechanism for resource and packet counters
A new counters model is proposed:
* API to query which objects support which counters
* A counter object that can be dynamiccally attached to other objects.

## Counter object type
For the counter object, a sai_counter_stat_t enum is defined, which is a superset of all possible statistics types.
```
typedef enum _sai_counter_stat_t
{
    /** Get tx packets count [uint64_t] */
    SAI_COUNTER_STAT_PACKETS = 0x00000000,

    /** Get tx bytes count [uint64_t] */
    SAI_COUNTER_STAT_BYTES = 0x00000001,

    /** Get dropped packets count [uint64_t] */
    SAI_COUNTER_STAT_DROPPED_PACKETS = 0x00000002,

    /** Get dropped bytes count [uint64_t] */
    SAI_COUNTER_STAT_DROPPED_BYTES = 0x00000003,

    …

    /** Get current object occupancy in bytes [uint64_t] */
    SAI_COUNTER_STAT_CURR_OCCUPANCY_BYTES = 0x00000018,
  
    …

     /** Custom range base value */
    SAI_QUEUE_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_counter_stat_t
```

### Attaching counter to object case #1
```
/**
 * @brief Attribute Id in sai_get_counter_attribute() call
 */
typedef enum _sai_counter_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_COUNTER_ATTR_START,

    /**
     * @brief SAI Object for the counter to be attached to
     *
     * @type sai_object_key_t
     * @flags CREATE_ONLY
     */
    SAI_COUNTER_ATTR_OBJECT_ID = SAI_COUNTER_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_COUNTER_ATTR_END,

    /** Custom range base value */
    SAI_COUNTER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_COUNTER_ATTR_CUSTOM_RANGE_END

} sai_counter_attr_t;
```

### Attaching counter to object case #2
```
/**
 * @brief Attribute Id in sai_set_port_attribute() and
 * sai_get_port_attribute() calls
 */
typedef enum _sai_port_attr_t
{
    …
     /**
     * @brief Port counter object id
     *
     * SAI counter object id to be bound to the port
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_COUNTER_OBJECT_ID
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_COUNTER_ID,

    /**
     * @brief End of attributes
     */
    SAI_PORT_ATTR_END,

    /** Custom range base value */
    SAI_PORT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PORT_ATTR_CUSTOM_RANGE_END

} sai_port_attr_t;
```

API for getting stats values looks similar to what is defined today per object type.
```
/**
 * @brief Get counter statistics values.
 *
 * @param[in] counter_id Counter id
 * @param[in] number_of_stats Number of counter values in the array
 * @param[in] stat_ids Specifies the array of counter types
 * @param[in] stats_mode Counter read mode
 * @param[out] stats Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_counter_stats_fn)(
        _In_ sai_object_id_t counter_id,
        _In_ uint32_t number_of_stats,
        _In_ const sai_stat_id_t *stat_ids,
        _In_ sai_stats_mode_t stats_mode,
        _Out_ uint64_t *stats);
```

## Counter object behavior
* Counter can be attached/detached multiple times during it's lifetime.
* Detaching a counter from an object does not clear it's stats values.
* Attaching a counter to an object does not clear it's stats values.
* Counter can be attached to multiple objects at the same time.

## Generic resource accounting
Resources availability can be queried on a per object type basis.
```
/**
 * @brief Get SAI object type resource availability.
 *
 * @param[in] object_type SAI object type
 * @param[out] Available objects left
 *
 * @return #SAI_STATUS_NOT_SUPPORTED if the given object type does not support resource accounting.
 * Otherwise, return #SAI_STATUS_SUCCESS.
 */
sai_status_t sai_object_type_get_availability(
        _In_ sai_object_type_t object_type,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list,
        _Out_ uint64_t *count);
```

## Object query
```
/**
 * @brief Query SAI object counter capabilities.
 *
 * @param[in] object_id Object id
 * @param[inout] counter_type_list Supported counter type list
 *
 * @return #SAI_STATUS_BUFFER_OVERFLOW
 * if counter_type_list is of insufficient size.
 * Otherwise, return #SAI_STATUS_SUCCESS.
 */
sai_status_t sai_object_counter_types_query(
        _In_ sai_object_type_t object_type,
        _Inout_ sai_s32_list_t counter_type_list);
```
