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
Counting every statistics of every family might be too resource intensive, therefore the debug counters provide an efficient way to count and aggregate only the needed information.

## Debug counter object type
For the counter object, a sai_counter_stat_t enum is defined:
```
typedef enum _sai_debug_counter_type_t
{
    /** Port in drop reasons. Base object : SAI_OBJECT_TYPE_PORT */
    SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS,

    /** Port out drop reasons. Base object : SAI_OBJECT_TYPE_PORT */
    SAI_DEBUG_COUNTER_TYPE_PORT_OUT_DROP_REASONS,

    /**
     * @brief Switch in drop reasons
     *
     * Base object: SAI_OBJECT_TYPE_SWITCH.
     * Values for all ports in the switch are summed up by switch counter
     */
    SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS,

    /**
     * @brief Switch out drop reasons
     *
     * Base object: SAI_OBJECT_TYPE_SWITCH.
     * Values for all ports in the switch are summed up by switch counter
     */
    SAI_DEBUG_COUNTER_TYPE_SWITCH_OUT_DROP_REASONS,

} sai_debug_counter_type_t;
```

## Debug counter in drop reason family
```
typedef enum _sai_in_drop_reason_t
{
    /* L2 reasons */

    /** Any L2 pipeline drop */
    SAI_IN_DROP_REASON_L2_ANY,

    /** Source MAC is multicast */
    SAI_IN_DROP_REASON_SMAC_MULTICAST,

    /** Source MAC equals destination MAC */
    SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC,

    /** Destination MAC is Reserved (Destination MAC=01-80-C2-00-00-0x) */
    SAI_IN_DROP_REASON_DMAC_RESERVED,

    /**
     * @brief VLAN tag not allowed
     *
     * Frame tagged when port is dropping tagged,
     * or untagged when dropping untagged
     */
    SAI_IN_DROP_REASON_VLAN_TAG_NOT_ALLOWED,

    /** Ingress VLAN filter */
    SAI_IN_DROP_REASON_INGRESS_VLAN_FILTER,

    /** Ingress STP filter */
    SAI_IN_DROP_REASON_INGRESS_STP_FILTER,

    /** Unicast FDB table action discard */
    SAI_IN_DROP_REASON_FDB_UC_DISCARD,

    /** Multicast FDB table empty tx list */
    SAI_IN_DROP_REASON_FDB_MC_DISCARD,

    /** Port L2 loopback filter (packet egressing on the same port+VLAN as ingressing) */
    SAI_IN_DROP_REASON_L2_LOOPBACK_FILTER,

    /** Packet size is larger than the L2 (Port) MTU */
    SAI_IN_DROP_REASON_EXCEEDS_L2_MTU,

    /* L3 reasons */

    /** Any L3 pipeline drop */
    SAI_IN_DROP_REASON_L3_ANY,

    /** Packet size is larger than the L3 (Router Interface) MTU */
    SAI_IN_DROP_REASON_EXCEEDS_L3_MTU,

    /** TTL expired */
    SAI_IN_DROP_REASON_TTL,

    /** RIF L3 loopback filter (packet egressing on the same RIF as ingressing) */
    SAI_IN_DROP_REASON_L3_LOOPBACK_FILTER,

    /**
     * @brief Non routable packet
     *
     * IGMP v1 v2 v3 membership query
     * IGMP v1 membership report
     * IGMP v2 membership report
     * IGMP v2 leave group
     * IGMP v3 membership report
     */
    SAI_IN_DROP_REASON_NON_ROUTABLE,

    /** Destination MAC is the router MAC, however packet is not routable (isn't IP or MPLS) */
    SAI_IN_DROP_REASON_NO_L3_HEADER,

    /**
     * @brief IP Header error
     *
     * Due to header checksum or bad IP version or IPv4 IHL too short
     */
    SAI_IN_DROP_REASON_IP_HEADER_ERROR,

    /** Unicast destination IP with non unicast (multicast or broadcast) destination MAC */
    SAI_IN_DROP_REASON_UC_DIP_MC_DMAC,

    /**
     * @brief Destination IP is loopback address
     *
     * for IPv4: Destination IP=127.0.0.0/8
     * for IPv6: Destination IP=::1/128 OR Destination IP=0:0:0:0:0:ffff:7f00:0/104
     */
    SAI_IN_DROP_REASON_DIP_LOOPBACK,

    /**
     * @brief Source IP is loopback address
     *
     * for IPv4: Source IP=127.0.0.0/8
     * for IPv6: Source IP=::1/128
     */
    SAI_IN_DROP_REASON_SIP_LOOPBACK,

    /**
     * @brief Source IP is multicast address
     *
     * for IPv4: Source IP=224.0.0.0/4
     * for IPv6: Source IP=FF00::/8
     */
    SAI_IN_DROP_REASON_SIP_MC,

    /**
     * @brief Source IP is in class E
     *
     * IPv4 AND Source IP=240.0.0.0/4 AND Source IP!=255.255.255.255
     */
    SAI_IN_DROP_REASON_SIP_CLASS_E,

    /**
     * @brief Source IP unspecified
     *
     * for IPv4: Source IP=0.0.0.0/32
     * for IPv6: Source IP=::0
     */
    SAI_IN_DROP_REASON_SIP_UNSPECIFIED,

    /**
     * @brief Destination IP is multicast but destination MAC isn't
     *
     * Destination IP is multicast AND
     * for IPv4: Destination MAC!={01-00-5E-0 (25 bits), dip[22:0]}
     * for IPv6: Destination MAC!={33-33, DIP[31:0]}
     */
    SAI_IN_DROP_REASON_MC_DMAC_MISMATCH,

    /** Source IP equals destination IP */
    SAI_IN_DROP_REASON_SIP_EQUALS_DIP,

    /** IPv4 source IP is limited broadcast (Source IP=255.255.255.255) */
    SAI_IN_DROP_REASON_SIP_BC,

    /** IPv4 destination IP is local network (Destination IP=0.0.0.0/8) */
    SAI_IN_DROP_REASON_DIP_LOCAL,

    /** IPv4 unicast destination IP is link local (Destination IP=169.254.0.0/16) */
    SAI_IN_DROP_REASON_DIP_LINK_LOCAL,

    /** IPv4 Source IP is link local (Source IP=169.254.0.0/16) */
    SAI_IN_DROP_REASON_SIP_LINK_LOCAL,

    /** IPv6 destination in multicast scope 0 reserved (Destination IP=ff:x0:/16) */
    SAI_IN_DROP_REASON_IPV6_MC_SCOPE0,

    /** IPv6 destination in multicast scope 1 interface-local (Destination IP=ff:x1:/16) */
    SAI_IN_DROP_REASON_IPV6_MC_SCOPE1,

    /** Ingress RIF is disabled */
    SAI_IN_DROP_REASON_IRIF_DISABLED,

    /** Egress RIF is disabled */
    SAI_IN_DROP_REASON_ERIF_DISABLED,

    /** IPv4 Routing table (LPM) unicast miss */
    SAI_IN_DROP_REASON_LPM4_MISS,

    /** IPv6 Routing table (LPM) unicast miss */
    SAI_IN_DROP_REASON_LPM6_MISS,

    /** Black hole route (discard by route entry) */
    SAI_IN_DROP_REASON_BLACKHOLE_ROUTE,

    /** Black hole ARP/Neighbor (discard by ARP or neighbor entries) */
    SAI_IN_DROP_REASON_BLACKHOLE_ARP,

    /** Unresolved next hop (missing ARP entry) */
    SAI_IN_DROP_REASON_UNRESOLVED_NEXT_HOP,

    /**
     * @brief Packet is destined for neighboring device but neighbor device link is down
     *
     * Counted on ingress link
     */
    SAI_IN_DROP_REASON_L3_EGRESS_LINK_DOWN,

    /* Tunnel reasons */

    /**
     * @brief Packet decapsulation failed
     *
     * e.g.: need to decap too many bytes, remaining packet is too short
     */
    SAI_IN_DROP_REASON_DECAP_ERROR,

    /* ACL reasons */

    /** Packet is dropped due to configured ACL rules, all stages/bind points combinations */
    SAI_IN_DROP_REASON_ACL_ANY,

    /** Packet is dropped due to configured ACL rules, ingress stage, port binding */
    SAI_IN_DROP_REASON_ACL_INGRESS_PORT,

    /** Packet is dropped due to configured ACL rules, ingress stage, LAG binding */
    SAI_IN_DROP_REASON_ACL_INGRESS_LAG,

    /** Packet is dropped due to configured ACL rules, ingress stage, VLAN binding */
    SAI_IN_DROP_REASON_ACL_INGRESS_VLAN,

    /** Packet is dropped due to configured ACL rules, ingress stage, RIF binding */
    SAI_IN_DROP_REASON_ACL_INGRESS_RIF,

    /** Packet is dropped due to configured ACL rules, ingress stage, switch binding */
    SAI_IN_DROP_REASON_ACL_INGRESS_SWITCH,

    /** Packet is dropped due to configured ACL rules, egress stage, port binding */
    SAI_IN_DROP_REASON_ACL_EGRESS_PORT,

    /** Packet is dropped due to configured ACL rules, egress stage, LAG binding */
    SAI_IN_DROP_REASON_ACL_EGRESS_LAG,

    /** Packet is dropped due to configured ACL rules, egress stage, VLAN binding */
    SAI_IN_DROP_REASON_ACL_EGRESS_VLAN,

    /** Packet is dropped due to configured ACL rules, egress stage, RIF binding */
    SAI_IN_DROP_REASON_ACL_EGRESS_RIF,

    /** Packet is dropped due to configured ACL rules, egress stage, switch binding */
    SAI_IN_DROP_REASON_ACL_EGRESS_SWITCH,

    /** Custom range base value */
    SAI_IN_DROP_REASON_CUSTOM_RANGE_BASE = 0x10000000

} sai_in_drop_reason_t;
```

## Debug counter out drop reason family
```
typedef enum _sai_out_drop_reason_t
{
    /* L2 reasons */

    /** Any L2 pipeline drop */
    SAI_OUT_DROP_REASON_L2_ANY,

    /** Egress VLAN filter */
    SAI_OUT_DROP_REASON_EGRESS_VLAN_FILTER,

    /* L3 reasons */

    /** Any L3 pipeline drop */
    SAI_OUT_DROP_REASON_L3_ANY,

    /**
     * @brief Packet is destined for neighboring device but neighbor device link is down
     *
     * Counted on egress link
     */
    SAI_OUT_DROP_REASON_L3_EGRESS_LINK_DOWN,

    /** Custom range base value */
    SAI_OUT_DROP_REASON_CUSTOM_RANGE_BASE = 0x10000000

} sai_out_drop_reason_t;
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

    /* READ-ONLY */

    /**
     * @brief Object stat index
     * Index is added to base start
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_DEBUG_COUNTER_ATTR_INDEX = SAI_DEBUG_COUNTER_ATTR_START,
	
    /* READ-WRITE */

    /**
     * @brief Debug counter type
     *
     * @type sai_debug_counter_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isresourcetype true
     */
    SAI_DEBUG_COUNTER_ATTR_TYPE,

    /**
     * @brief Bind method to base object
     *
     * @type sai_debug_counter_bind_method_t
     * @flags CREATE_ONLY
     * @default SAI_DEBUG_COUNTER_BIND_METHOD_AUTOMATIC
     */
    SAI_DEBUG_COUNTER_ATTR_BIND_METHOD,	

    /**
     * @brief List of in drop reasons that will be counted
     *
     * @type sai_s32_list_t sai_in_drop_reason_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_DEBUG_COUNTER_ATTR_TYPE == SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS or
     * SAI_DEBUG_COUNTER_ATTR_TYPE == SAI_DEBUG_COUNTER_TYPE_SWITCH_IN_DROP_REASONS
     */
    SAI_DEBUG_COUNTER_ATTR_IN_DROP_REASON_LIST,

    /**
     * @brief List of out drop reasons that will be counted
     *
     * @type sai_s32_list_t sai_out_drop_reason_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_DEBUG_COUNTER_ATTR_TYPE == SAI_DEBUG_COUNTER_TYPE_PORT_OUT_DROP_REASONS or
     * SAI_DEBUG_COUNTER_ATTR_TYPE == SAI_DEBUG_COUNTER_TYPE_SWITCH_OUT_DROP_REASONS
     */
    SAI_DEBUG_COUNTER_ATTR_OUT_DROP_REASON_LIST,

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

### Binding a debug counter
```
typedef enum _sai_debug_counter_bind_method_t
{
    /** Bind automatically to all instances of base object */
    SAI_DEBUG_COUNTER_BIND_METHOD_AUTOMATIC,

} sai_debug_counter_bind_method_t;
```

With automatic bind method, once configured, a debug counter is automatically attached to all object instances of the relevant object type.
Future extension may require binding only to certain object instances of the relevant object type. A possible extension will be to add SAI_DEBUG_COUNTER_BIND_METHOD_MANUAL.
In addition, adding object list of bounded counters in the relevant object type. For example, in case of port drop reasons, adding

```
/**
 * @brief Attach a list of debug counter
 *

 *
 * @type sai_object_list_t
 * @flags CREATE_AND_SET
 * @objects SAI_OBJECT_TYPE_DEBUG_COUNTER
 * @default empty
 */
SAI_PORT_ATTR_DEBUG_COUNTER_LIST,
```

### Reading a debug counter
```
    /** Port stat in drop reasons range start */
    SAI_PORT_STAT_IN_DROP_REASON_RANGE_BASE = 0x00001000,

    /** Port stat in drop reasons range end */
    SAI_PORT_STAT_IN_DROP_REASON_RANGE_END = 0x00001fff,

    /** Port stat out drop reasons range start */
    SAI_PORT_STAT_OUT_DROP_REASON_RANGE_BASE = 0x00002000,

    /** Port stat out drop reasons range end */
    SAI_PORT_STAT_OUT_DROP_REASON_RANGE_END = 0x00002fff,

    /** Switch stat in drop reasons range start */
    SAI_SWITCH_STAT_IN_DROP_REASON_RANGE_BASE = 0x00001000,

    /** Switch stat in drop reasons range end */
    SAI_SWITCH_STAT_IN_DROP_REASON_RANGE_END = 0x00001fff,

    /** Switch stat out drop reasons range start */
    SAI_SWITCH_STAT_OUT_DROP_REASON_RANGE_BASE = 0x00002000,

    /** Switch stat out drop reasons range end */
    SAI_SWITCH_STAT_OUT_DROP_REASON_RANGE_END = 0x00002fff,
```

### Checking debug counter capability
Application can query the ASIC support for counters of certain family by sai_query_attribute_enum_values_capability
SAI will return the list of implemented drop reasons, each reason is implemented independantly of others.
If there is a case of dependency between drop reasons, for example the ASIC can only track several reasons summed up together, it is suggested the ASIC vendor will add a new combined drop reason. 

Application can query the amount of ASIC available debug counters of certain family by generic CRM sai_object_type_get_availability, using SAI_DEBUG_COUNTER_ATTR_TYPE as an attribute if needed

### Counting packet which is dropped by multiple reasons
Per debug counter instance, a packet drop is counted once, even if a packet is dropped by multiple reasons at the same pipleine stage which the counter tracks.
For example, consider a packet which is dropped by reason 1 and 2, both at the same pipeline stage.
Debug counter A tracks both reason 1 and 2, counter B is tracking reason 1, counter C is tracking reason 2.
Counters A, B, C all will increase by 1

### Debug counter behavior when traffic checker is disabled
ASIC can be configured to ignore some of the checkers for drop conditions.
For example, ASIC can be configured with NOP for SIP=DIP traffic instead of dropping such traffic.
Or SAI allows configuring SAI_ROUTER_INTERFACE_ATTR_LOOPBACK_PACKET_ACTION=FORWARD for L3 loopback traffic.
If such a checker is configured to forward traffic, packet won't be dropped and counter measuring the relevant drop reason won't increase for traffic hitting the condition.
Drop counter is only increased when a packet is actually dropped.

### Usage example - creating and quering debug counters
```
sai_attribute_t debug_counter_attr[2];
debug_counter_attr[0].id = SAI_DEBUG_COUNTER_ATTR_TYPE;
debug_counter_attr[0].value.s32 = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS;
debug_counter_attr[1].id = SAI_DEBUG_COUNTER_ATTR_IN_DROP_REASON_LIST;
debug_counter_attr[1].value.s32list.count = 3;
sai_port_in_drop_reason_t in_drop_reason1[] = {SAI_IN_DROP_REASON_SMAC_MULTICAST, SAI_IN_DROP_REASON_SMAC_EQUALS_DMAC, SAI_IN_DROP_REASON_DMAC_RESERVED};
debug_counter_attr[1].value.s32list.list = in_drop_reason1;
sai_object_id_t debug_counter_id1;
sai_status_t rc = sai_debug_counter_api->create_debug_counter(&debug_counter_id1, g_switch_id, 2, debug_counter_attr);

debug_counter_attr[0].id = SAI_DEBUG_COUNTER_ATTR_INDEX;
sai_status_t rc = sai_debug_counter_api->get_debug_counter_attribute(debug_counter_id1, 1, debug_counter_attr);
unsigned int debug_counter_index1;
debug_counter_index1 = debug_counter_attr[0].value.u32;

debug_counter_attr[0].id = SAI_DEBUG_COUNTER_ATTR_TYPE;
debug_counter_attr[0].value.s32 = SAI_DEBUG_COUNTER_TYPE_PORT_IN_DROP_REASONS;
debug_counter_attr[1].id = SAI_DEBUG_COUNTER_ATTR_IN_DROP_REASON_LIST;
debug_counter_attr[1].value.s32list.count = 2;
sai_port_in_drop_reason_t in_drop_reason2[] = {SAI_IN_DROP_REASON_VLAN_TAG_NOT_ALLOWED, SAI_IN_DROP_REASON_INGRESS_STP_FILTER};
debug_counter_attr[1].value.s32list.list = in_drop_reason2;
sai_object_id_t debug_counter_id2;
sai_status_t rc = sai_debug_counter_api->create_debug_counter(&debug_counter_id2, g_switch_id, 2, debug_counter_attr);

debug_counter_attr[0].id = SAI_DEBUG_COUNTER_ATTR_INDEX;
sai_status_t rc = sai_debug_counter_api->get_debug_counter_attribute(debug_counter_id2, 1, debug_counter_attr);
unsigned int debug_counter_index2;
debug_counter_index2 = debug_counter_attr[0].value.u32;

sai_port_stat_t stat_ids[] = { SAI_PORT_STAT_IN_DROP_REASON_RANGE_BASE + debug_counter_index1, SAI_PORT_STAT_IN_DROP_REASON_RANGE_BASE + debug_counter_index2 };
uint64_t stats[2];
rc = sai_port_api->sai_get_counter_stats_ext(port_id, 2, stat_ids, stats);
printf("Port XXX In DROP_REASON_SMAC_MULTICAST + DROP_REASON_SMAC_EQUALS_DMAC + DROP_REASON_DMAC_RESERVED %lu\n", stats[0]);
printf("Port XXX In DROP_REASON_VLAN_TAG_NOT_ALLOWED + DROP_REASON_INGRESS_STP_FILTER %lu\n", stats[1]);
```

### Usage example - query capabilities
```
sai_s32_list_t        enum_caps_list;
int32_t               enums_caps[100];
enum_caps_list.count = 100;
enum_caps_list.list = enums_caps;
/* Get supported in drop reasons */
sai_query_attribute_enum_values_capability(switch_id1, SAI_OBJECT_TYPE_DEBUG_COUNTER, SAI_DEBUG_COUNTER_ATTR_IN_DROP_REASON_LIST, &enum_caps_list);
int ii;
for (ii=0; ii<enum_caps_list.count; ii++) {
  printf("reason ID %d supported", enum_caps_list.list[ii]);
}
```

## Usage example - query available debug counters
```
sai_attribute_t attr;
sai_status_t status;
uint64_t count;

attr.id = SAI_DEBUG_COUNTER_ATTR_TYPE;
attr.value.s32 = SAI_DEBUG_COUNTER_TYPE_PORT_OUT_DROP_REASONS;

/* Get available port out drop reasons debug counters */
status = sai_object_type_get_availability(switch_id1, SAI_OBJECT_TYPE_DEBUG_COUNTER, 1, &attr, &count);
```
