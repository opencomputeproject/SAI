# Queue Pair Based Forwarding

Title       | Queue Pair Based Forwarding
------------|---------------------------------------------------------------
Authors     | Jason Bos, Cisco
Status      | Draft
Type        | Experimental
Created     | 2026-11-04
SAI-Version | Experimental extension, SAI 1.20

## 1. Overview

This proposal introduces an experimental SAI API for forwarding packets based on
the combination of a destination IP address and a destination Queue Pair number.
A matching entry selects a packet action and forwarding destination within a
switch and virtual router.

The API introduces a separate route entry keyed by switch, virtual router,
Queue Pair ID, and destination IP prefix. Each entry has forwarding attributes
similar to an ordinary IP route. The API is defined in
[`experimental/saiexperimentalqproute.h`](../experimental/saiexperimentalqproute.h).

## 2. Functional Behavior

### 2.1. Lookup key and prefix selection

A Queue Pair route lookup uses the destination Queue Pair number and destination
IP address within the applicable virtual router. For RoCEv2, the Destination
Queue Pair Number (DQPN) is the 24-bit field in the InfiniBand Base Transport
Header (BTH).

The Queue Pair number is matched exactly and the destination address uses longest-prefix matching.

### 2.2. Ordinary route fallback

When Queue Pair forwarding is enabled and a packet has a valid parsed RoCEv2
BTH, the Queue Pair route lookup precedes ordinary IP route lookup. On a Queue
Pair lookup hit, the selected entry supplies the forwarding result. A more
specific ordinary IP route does not override that result.

If no Queue Pair entry matches both the DQPN and destination prefix, forwarding
uses ordinary destination IP lookup in the packet's virtual router. Packets
without a valid parsed RoCEv2 BTH, and packets processed with Queue Pair
forwarding disabled, also use ordinary IP lookup.

Queue Pair entries and ordinary IP routes are programmed independently and can
coexist for the same destination prefix.

### 2.3. Actions and enablement

Queue Pair entry attributes are the same as regular IP routes and follow the same semantics.

## 3. API Specification

### 3.1. Experimental API and object type

The proposal adds the following experimental object type and API

| Identifier | Definition |
|------------|------------|
| `SAI_API_QP_ROUTE` | Queue Pair route API in `sai_api_extensions_t` |
| `SAI_OBJECT_TYPE_QP_ROUTE_ENTRY` | Entry type in `sai_object_type_extensions_t` |

```c
sai_qp_route_api_t *qp_route_api = NULL;

sai_status_t status = sai_api_query(
        (sai_api_t)SAI_API_QP_ROUTE,
        (void **)&qp_route_api);
```

### 3.2. Entry key

The key is the same format as the regular IP route key but with the addition of the `qp_id` identifier.
```c
typedef struct _sai_qp_route_entry_t
{
    /** @objects SAI_OBJECT_TYPE_SWITCH */
    sai_object_id_t switch_id;

    /** @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER */
    sai_object_id_t vr_id;

    sai_uint32_t qp_id;
    sai_ip_prefix_t destination;

} sai_qp_route_entry_t;
```

| Field | Meaning |
|-------|---------|
| `switch_id` | Switch containing the entry. |
| `vr_id` | Virtual router containing the entry. |
| `qp_id` | Destination Queue Pair number, in host byte order. Valid range: `0`–`0xFFFFFF`. |
| `destination` | IPv4 or IPv6 destination prefix. The address and mask use network byte order. |

The destination prefix supports longest-prefix matching, including `/0` and
host prefixes (`/32` for IPv4 or `/128` for IPv6).

### 3.3. Entry attributes

The experimental attribute enum is `sai_qp_route_entry_attr_t`. The attributes have the same properties and defaults as their counterparts in `sai_route_entry_attr_t`.

```c
typedef enum _sai_qp_route_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_QP_ROUTE_ENTRY_ATTR_START,

    /**
     * @brief Packet action
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_QP_ROUTE_ENTRY_ATTR_PACKET_ACTION = SAI_QP_ROUTE_ENTRY_ATTR_START,

    /**
     * @brief Generate User Defined Trap ID for trap/log actions
     *
     * When it is SAI_NULL_OBJECT_ID, then packet will not be trapped.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HOSTIF_USER_DEFINED_TRAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_QP_ROUTE_ENTRY_ATTR_USER_TRAP_ID,

    /**
     * @brief Next hop or next hop group id for the packet, or a router interface
     * in case of directly reachable route, or the CPU port in case of IP2ME route
     *
     * The next hop id is only effective when the packet action is one of the
     * following: FORWARD, COPY, LOG, TRANSIT.
     *
     * The next hop id can be a generic next hop object, such as next hop, next
     * hop group. Directly reachable routes are the IP subnets that are
     * directly attached to the router. For such routes, fill the router
     * interface id to which the subnet is attached. IP2ME route adds a local
     * router IP address. For such routes, fill the CPU port
     * (#SAI_SWITCH_ATTR_CPU_PORT).
     * When pointing to a next hop group which is empty, the effective routing
     * action will be DROP.
     *
     * When it is SAI_NULL_OBJECT_ID, then packet will be dropped.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NEXT_HOP, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, SAI_OBJECT_TYPE_ROUTER_INTERFACE, SAI_OBJECT_TYPE_PORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_QP_ROUTE_ENTRY_ATTR_NEXT_HOP_ID,

    /**
     * @brief User based Meta Data
     *
     * Value Range #SAI_SWITCH_ATTR_ROUTE_DST_USER_META_DATA_RANGE
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_QP_ROUTE_ENTRY_ATTR_META_DATA,

    /**
     * @brief Queue Pair route entry IP address family
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_QP_ROUTE_ENTRY_ATTR_IP_ADDR_FAMILY,

    /**
     * @brief Attach a counter
     *
     * When it is empty, then packet hits won't be counted
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_QP_ROUTE_ENTRY_ATTR_COUNTER_ID,

    /**
     * @brief Route Prefix Aggregation ID
     *
     * Prefix aggregation ID is used for SRV6 VPN cases. When multiple prefixes
     * use the same set of remote SRV6 Endpoint(s) but with different VPN SID
     * values, prefix aggregation id allows SAI to derive different VPN SID while
     * keeping using single destination (NH or Next hop group)
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_QP_ROUTE_ENTRY_ATTR_PREFIX_AGG_ID,

    /**
     * @brief End of attributes
     */
    SAI_QP_ROUTE_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_QP_ROUTE_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_QP_ROUTE_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_qp_route_entry_attr_t;
```

The separate enum lets metadata associate the attributes with the new entry
type. Its matching layout allows applications to share attribute-list construction
with ordinary routes. It does not make the two entry keys interchangeable.

`NEXT_HOP_ID` refers to an existing SAI forwarding object. Updating this attribute
changes the entry's forwarding destination without changing its key.

The attributes retain the corresponding ordinary-route semantics, including
drop behavior for a null next hop or an empty next hop group.

### 3.4. Methods

These methods parallel the corresponding IP routing API methods in
`sai_route_api_t`, using a `sai_qp_route_entry_t` key.

| Method | Operation |
|--------|-----------|
| `create_qp_route_entry` | Create an entry from its key and attributes. |
| `remove_qp_route_entry` | Remove the entry identified by its key. |
| `set_qp_route_entry_attribute` | Set one attribute on an existing entry. |
| `get_qp_route_entry_attribute` | Read attributes from an existing entry. |
| `create_qp_route_entries` | Create entries with per-entry attribute lists. |
| `remove_qp_route_entries` | Remove a list of entry keys. |
| `set_qp_route_entries_attribute` | Set one attribute per entry. |
| `get_qp_route_entries_attribute` | Read attribute lists for multiple entries. |

## 4. Example Call Sequence

The following example creates a host entry for destination `192.0.2.10` and
Queue Pair 51. The caller supplies an existing switch, virtual router, and next
hop.

```c
#include <arpa/inet.h>
#include <saiextensions.h>

sai_status_t create_queue_pair_route(
        sai_object_id_t switch_id,
        sai_object_id_t vr_id,
        sai_object_id_t next_hop_id)
{
    sai_qp_route_api_t *api = NULL;
    sai_status_t status = sai_api_query(
            (sai_api_t)SAI_API_QP_ROUTE,
            (void **)&api);

    if (status != SAI_STATUS_SUCCESS)
    {
        return status;
    }

    sai_qp_route_entry_t entry = {0};
    entry.switch_id = switch_id;
    entry.vr_id = vr_id;
    entry.qp_id = 51;
    entry.destination.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    entry.destination.addr.ip4 = htonl(0xc000020a);
    entry.destination.mask.ip4 = htonl(0xffffffff);

    sai_attribute_t attr = {0};
    attr.id = SAI_QP_ROUTE_ENTRY_ATTR_NEXT_HOP_ID;
    attr.value.oid = next_hop_id;

    return api->create_qp_route_entry(&entry, 1, &attr);
}
```

To program another Queue Pair for the same destination, create another entry
with a different `qp_id`. To change an entry's forwarding destination, set
`SAI_QP_ROUTE_ENTRY_ATTR_NEXT_HOP_ID` on its existing key. To delete the entry,
call `remove_qp_route_entry` with that key.

An ordinary IP route is programmed independently through
`SAI_API_ROUTE` with a `sai_route_entry_t` key.

## 5. Implementation and API Integration

### 5.1. Virtual router support

The SAI key includes `vr_id`, and Queue Pair routes in different virtual routers
have independent identities.

An implementation with a device-wide Queue Pair table may restrict the feature
to a default vrf configuration.

## 6. References

- [Experimental Queue Pair route API](../experimental/saiexperimentalqproute.h)
- [Ordinary route API](../inc/sairoute.h)
