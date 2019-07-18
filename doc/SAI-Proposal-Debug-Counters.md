# Generic SAI Debug Counters

Title       | Generic SAI Debug counters
------------|----------------
Authors     | Mellanox
Status      | In review
Type        | Standards track
Created     | 07/17/2019
SAI-Version | 1.5

## Overview
ASICs can provide a set of debug counters for certain object types.
Debug counters belong to certain families, each family is for cetain object type.
The content of a specific debug counter instance can be defined by the application.
Counting every statistics of every family might be too resource intensive, therefor the debug counters provide an efficient way to count and aggregate only the needed information.

## Debug counter object type
For the counter object, a sai_counter_stat_t enum is defined:
```
typedef enum _sai_debug_counter_type_t
{
    /** Port in drop reasons */
    SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,

    /** Port out drop reasons */
    SAI_DEBUG_COUNTER_TYPE_PORT_OUT_DROP_REASONS,

} sai_debug_counter_type_t;
```

## Debug counter port in drop reason family
```
typedef enum _sai_port_in_drop_reason_t
{
    /* L2 reasons */
	
    /** Source MAC is multicast */
    SAI_PORT_IN_DROP_REASON_SMAC_MULTICAST,

    /** Source MAC equals Destination MAC */
    SAI_PORT_IN_DROP_REASON_SMAC_EQUALS_DMAC,

    /** Destination MAC is Reserved (DMAC=01-80-C2-00-00-0x) */
    SAI_PORT_IN_DROP_REASON_DMAC_RESERVED,

    /** 
     * @brief VLAN tag not allowed
     *
     * Frame tagged when port is dropping tagged, 
     * or untagged when dropping untagged
     */
    SAI_PORT_IN_DROP_REASON_VLAN_TAG_NOT_ALLOWED,

    /** Ingress VLAN filter */
    SAI_PORT_IN_DROP_REASON_INGRESS_VLAN_FILTER,

    /** Ingress STP filter */
    SAI_PORT_IN_DROP_REASON_INGRESS_STP_FILTER,
	
    /** Unicast FDB table action discard */
    SAI_PORT_IN_DROP_REASON_FDB_UC_DISCARD,

    /** Multicast FDB table empty tx list */
    SAI_PORT_IN_DROP_REASON_FDB_MC_DISCARD,

    /** Port loopback filter */
    SAI_PORT_IN_DROP_REASON_LOOPBACK_FILTER,

    /* L3 reasons */	

} sai_port_in_drop_reason_t;
```

## Debug counter port out drop reason family
```
typedef enum _sai_port_out_drop_reason_t
{
    /** Egress VLAN filter */
    SAI_PORT_OUT_DROP_REASON_EGRESS_VLAN_FILTER,

} sai_port_out_drop_reason_t;
```

### Configuring a debug counter
```
/**
 * @brief Attribute Id in sai_set_counter_attribute() and
 * sai_get_counter_attribute() calls
 */
typedef enum _sai_debug_counter_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DEBUG_COUNTER_ATTR_START,

    /* READ-WRITE */

    /**
     * @brief Debug counter type
     *
     * @type sai_debug_counter_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isresourcetype true
     */
    SAI_DEBUG_COUNTER_ATTR_TYPE = SAI_DEBUG_COUNTER_ATTR_START,

    /**
     * @brief Object stat index
     * Index is added to base start
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DEBUG_COUNTER_ATTR_INDEX,

    /**
     * @brief List of port in drop reasons that will be counted
     *
     * @type sai_s32_list_t sai_port_in_drop_reason_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_DEBUG_COUNTER_ATTR_TYPE == 
     * SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS
     */
    SAI_DEBUG_COUNTER_ATTR_PORT_IN_DROP_REASON_LIST,

    /**
     * @brief List of port out drop reasons that will be counted
     *
     * @type sai_s32_list_t sai_port_out_drop_reason_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_DEBUG_COUNTER_ATTR_TYPE == 
     * SAI_DEBUG_COUNTER_TYPE_PORT_OUT_DROP_REASONS
     */
    SAI_DEBUG_COUNTER_ATTR_PORT_OUT_DROP_REASON_LIST,

    /**
     * @brief End of attributes
     */
    SAI_DEBUG_COUNTER_ATTR_END,

    /** Custom range base value */
    SAI_DEBUG_COUNTER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DEBUG_COUNTER_ATTR_CUSTOM_RANGE_END

} sai_debug_counter_attr_t;
```

### Reading a debug counter
```
    /** Port stat in drop reasons range start */
    SAI_PORT_STAT_IN_DROP_REASON_RANGE_BASE = 0x00010000,

    /** Port stat in drop reasons range end */
    SAI_PORT_STAT_IN_DROP_REASON_RANGE_END = 0x0001ffff,

    /** Port stat out drop reasons range start */
    SAI_PORT_STAT_OUT_DROP_REASON_RANGE_BASE = 0x00020000,

    /** Port stat out drop reasons range end */
    SAI_PORT_STAT_OUT_DROP_REASON_RANGE_END = 0x0002ffff,
```

### Checking debug counter capability
Application can query the ASIC support for counters of certain family by sai_query_attribute_enum_values_capability
Application can query the amount of ASIC available debug counters of certain family by generic CRM sai_object_type_get_availability, using SAI_DEBUG_COUNTER_ATTR_TYPE as an attribute if needed

## Usage example
```
sai_attribute_t debug_counter_attr[3];
debug_counter_attr[0].id = SAI_COUNTER_ATTR_TYPE;
debug_counter_attr[0].value.s32 = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS;
debug_counter_attr[1].id = SAI_COUNTER_ATTR_INDEX;
debug_counter_attr[1].value.u32 = 0;
debug_counter_attr[2].id = SAI_DEBUG_COUNTER_ATTR_PORT_IN_DROP_REASON_LIST;
debug_counter_attr[2].value.s32list.count = 3;
sai_port_in_drop_reason_t in_drop_reason1[] = {SAI_PORT_IN_DROP_REASON_SMAC_MULTICAST, SAI_PORT_IN_DROP_REASON_SMAC_EQUALS_DMAC, SAI_PORT_IN_DROP_REASON_DMAC_RESERVED};
debug_counter_attr[2].value.s32list.list = in_drop_reason1;
sai_object_id_t debug_counter_id1;
sai_status_t rc = sai_debug_counter_api->create_counter(&debug_counter_id1, g_switch_id, 3, &debug_counter_attr);

debug_counter_attr[0].id = SAI_COUNTER_ATTR_TYPE;
debug_counter_attr[0].value.s32 = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS;
debug_counter_attr[1].id = SAI_COUNTER_ATTR_INDEX;
debug_counter_attr[1].value.u32 = 1;
debug_counter_attr[2].id = SAI_DEBUG_COUNTER_ATTR_PORT_IN_DROP_REASON_LIST;
debug_counter_attr[2].value.s32list.count = 2;
sai_port_in_drop_reason_t in_drop_reason2[] = {SAI_PORT_IN_DROP_REASON_VLAN_TAG_NOT_ALLOWED, SAI_PORT_IN_DROP_REASON_INGRESS_STP_FILTER};
debug_counter_attr[2].value.s32list.list = in_drop_reason2;
sai_object_id_t debug_counter_id2;
sai_status_t rc = sai_debug_counter_api->create_counter(&debug_counter_id2, g_switch_id, 3, &debug_counter_attr);

sai_port_stat_t stat_ids[] = { SAI_PORT_STAT_IN_DROP_REASON_RANGE_BASE, SAI_PORT_STAT_IN_DROP_REASON_RANGE_BASE+1 };
uint64_t stats[2];
rc = sai_port_api->sai_get_counter_stats_ext(port_id, 2, stat_ids, stats);
printf("Port XXX In DROP_REASON_SMAC_MULTICAST + DROP_REASON_SMAC_EQUALS_DMAC + DROP_REASON_DMAC_RESERVED %lu\n", stats[0]);
printf("Port XXX In DROP_REASON_VLAN_TAG_NOT_ALLOWED + DROP_REASON_INGRESS_STP_FILTER %lu\n", stats[1]);
```
