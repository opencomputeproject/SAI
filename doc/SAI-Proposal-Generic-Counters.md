# Generic SAI Counters and Resources

Title       | Critical Resource Monitor
------------|----------------
Authors     | Mellanox
Status      | In review
Type        | Standards track
Created     | 04/10/2019
SAI-Version | 1.4

## Overview
ASICs usually provide a pool of generic counters that can be attached to dynamic entries for a better visibility and debuggability.
This document defines an API for the generic counter object type, that can be attached to one or more objects or entries for reading packet hits on them.

## Counter object type
For the counter object, a sai_counter_stat_t enum is defined:
```
typedef enum _sai_counter_stat_t
{
    /** Get tx packets count [uint64_t] */
    SAI_COUNTER_STAT_PACKETS = 0x00000000,

    /** Get tx bytes count [uint64_t] */
    SAI_COUNTER_STAT_BYTES = 0x00000001,

     /** Custom range base value */
    SAI_COUNTER_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_counter_stat_t
```

### Attaching counter to object
```
/**
 * @brief Attribute Id in sai_set_port_attribute() and
 * sai_get_port_attribute() calls
 */
typedef enum _sai_route_entry_attr_t
{
    …
         /**
     * @brief Attach a list of counters
     *
     * When it is empty, then packet hits won't be counted
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_ROUTE_ENTRY_ATTR_COUNTER_ID_LIST,

    /**
     * @brief End of attributes
     */
    SAI_ROUTE_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_ROUTE_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_ROUTE_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_route_entry_attr_t;
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
