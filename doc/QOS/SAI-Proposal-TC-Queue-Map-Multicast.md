# [SAI] Separate TC-to-Queue Map for Multicast Flows

---

| Title       | Separate TC-to-Queue Map for Multicast Flows        |
|-------------|-----------------------------------------------------|
| Authors     | Komal Shah (Marvell)                                |
| Status      | In review                                           |
| Type        | Standards track                                     |
| Created     | 2026-07-16                                          |
| SAI-Version | 1.18                                                |

---

## 1.0 Overview

SAI provides a single Traffic Class (TC) to Queue mapping through `SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP` (switch level) and `SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP` (port level). This map is applied to all traffic on a port regardless of its forwarding type. In practice, a queue object is identified by the pair (`SAI_QUEUE_ATTR_TYPE`, `SAI_QUEUE_ATTR_INDEX`) and a UC queue and an MC queue may legally share the same index while remaining distinct objects.

This single-map model cannot express a different TC-to-Queue mapping for unicast and multicast traffic (Broadcast, Unknown Unicast, and Multicast)

Two configurations expose the limitation:

- **Unequal UC and MC queue counts.** A port has fewer MC queues than UC queues. With same queue index used for UC and MC queues, a single TC to Queue map cannot be used for mapping UC and MC traffic flows.

- **Unified queues (`SAI_QUEUE_TYPE_ALL`).** When each queue carries both UC and MC traffic under one OID, an operator may want UC traffic from TC0 and MC traffic from TC1 to land on the same Queue0. A single map cannot do this, because each TC resolves to exactly one queue index for all traffic types.

This proposal adds a dedicated TC-to-Queue map for MC flows at both the switch and port levels. When the new MC map attribute is `SAI_NULL_OBJECT_ID` (the default), MC flows reuse the existing UC map, so the change is fully backward compatible.

## 2.0 Specification

Two new attributes are proposed — one at the switch level and one at the port level — to carry a separate TC to Queue QoS map for Multicast flows.

### 2.1 Switch-level attribute (`inc/saiswitch.h`)

```c
    /**
     * @brief Enable TC -> Queue MAP on switch for Multicast(Broadcast, Unknown unicast, Multicast) flows
     *
     * Map id = #SAI_NULL_OBJECT_ID to have same map for Multicast as Unicast.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP_MULTICAST,
```

### 2.2 Port-level attribute (`inc/saiport.h`)

```c
    /**
     * @brief Enable TC -> Queue MAP on port for Multicast(Broadcast, Unknown unicast, Multicast) flows
     *
     * Map id = #SAI_NULL_OBJECT_ID to have same map for Multicast as Unicast.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP_MULTICAST,
```

### 2.3 Map resolution order

For a given port, the TC-to-Queue map used for MC flows is resolved in the following order, stopping at the first attribute that is not
`SAI_NULL_OBJECT_ID`:

1. `SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP_MULTICAST`
2. `SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP_MULTICAST`
3. `SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP` (existing UC map)
4. `SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP` (existing UC map)

UC flows continue to resolve using only the existing attributes (steps 3 then 4). Because both MC attributes default to `SAI_NULL_OBJECT_ID`, existing deployments that never set them are unaffected.

## 3.0 Use Case

### 3.1 Unequal UC and MC queue counts

A port has 4 MC queues and 8 UC queues. With same queue index used for UC and MC queues, single map for UC and MC flows becomes impossible

| TC | Queue index (UC map) | Queue index (MC map) |
|----|----------------------|----------------------|
| 0  | 0                    | 0                    |
| 1  | 1                    | 1                    |
| 2  | 2                    | 2                    |
| 3  | 3                    | 3                    |
| 4  | 4                    | 0                    |
| 5  | 5                    | 1                    |
| 6  | 6                    | 2                    |
| 7  | 7                    | 3                    |

### 3.2 Unified queues (`SAI_QUEUE_TYPE_ALL`) with different traffic map

Queue0 which is of type `SAI_QUEUE_TYPE_ALL` must carry UC traffic from TC0 and, at the same time, MC traffic from TC1. 

With a single map this is impossible, since TC0 and TC1 can each resolve to only one queue index.

| TC | Queue index (UC map) | Queue index (MC map) |
|----|----------------------|----------------------|
| 0  | 0                    | 0                    |
| 1  | 1                    | 0                    |
| 2  | 2                    | 0                    |
| 3  | 3                    | 0                    |
| 4  | 4                    | 0                    |
| 5  | 5                    | 1                    |
| 6  | 6                    | 2                    |
| 7  | 7                    | 3                    |

- The UC map (`SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP` / `SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP`) provides the 1:1 mapping on the left.
- The MC map (`SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP_MULTICAST` / `SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP_MULTICAST`) folds TC0–TC4 onto the   MC queues on the right.

With both maps applied, UC traffic from TC0 and MC traffic from TC1 both resolve to Queue0, satisfying the requirement.

## 4.0 API Example

The steps below create the two maps, apply them at switch and port level, and then demonstrate the fallback behavior when the MC override is removed.

### 4.1 Create the UC and MC TC-to-Queue maps

```c
/* UC map: TC N -> Queue N (1:1). */
sai_qos_map_t   uc_map_list[8];
sai_attribute_t uc_map_attrs[2];

for (uint32_t i = 0; i < 8; i++) {
    uc_map_list[i].key.tc            = i;
    uc_map_list[i].value.queue_index = i;
}

uc_map_attrs[0].id                 = SAI_QOS_MAP_ATTR_TYPE;
uc_map_attrs[0].value.u32          = SAI_QOS_MAP_TYPE_TC_TO_QUEUE;
uc_map_attrs[1].id                 = SAI_QOS_MAP_ATTR_MAP_TO_VALUE_LIST;
uc_map_attrs[1].value.qosmap.count = 8;
uc_map_attrs[1].value.qosmap.list  = uc_map_list;

sai_object_id_t uc_qos_map_id;
sai_create_qos_map_fn(switch_id, 2, uc_map_attrs, &uc_qos_map_id);

/* MC map: TC0..TC4 -> Queue0, then TC5->1, TC6->2, TC7->3. */
sai_qos_map_t   mc_map_list[8];
sai_attribute_t mc_map_attrs[2];
uint32_t        mc_queue[8] = {0, 0, 0, 0, 0, 1, 2, 3};

for (uint32_t i = 0; i < 8; i++) {
    mc_map_list[i].key.tc            = i;
    mc_map_list[i].value.queue_index = mc_queue[i];
}

mc_map_attrs[0].id                 = SAI_QOS_MAP_ATTR_TYPE;
mc_map_attrs[0].value.u32          = SAI_QOS_MAP_TYPE_TC_TO_QUEUE;
mc_map_attrs[1].id                 = SAI_QOS_MAP_ATTR_MAP_TO_VALUE_LIST;
mc_map_attrs[1].value.qosmap.count = 8;
mc_map_attrs[1].value.qosmap.list  = mc_map_list;

sai_object_id_t mc_qos_map_id;
sai_create_qos_map_fn(switch_id, 2, mc_map_attrs, &mc_qos_map_id);
```

### 4.2 Apply the UC and MC maps at switch level

```c
sai_attribute_t sw_attr;

sw_attr.id        = SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP;
sw_attr.value.oid = uc_qos_map_id;
sai_set_switch_attribute_fn(switch_id, &sw_attr);

sw_attr.id        = SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP_MULTICAST;
sw_attr.value.oid = mc_qos_map_id;
sai_set_switch_attribute_fn(switch_id, &sw_attr);
```

### 4.3 Apply the UC and MC maps at port level

Port-level maps override the switch-level maps for that port.

```c
sai_attribute_t port_attr;

port_attr.id        = SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP;
port_attr.value.oid = uc_qos_map_id;
sai_set_port_attribute_fn(port_id, &port_attr);

port_attr.id        = SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP_MULTICAST;
port_attr.value.oid = mc_qos_map_id;
sai_set_port_attribute_fn(port_id, &port_attr);
```

### 4.4 Remove the MC override (fall back to the UC map)

```c
/* Clear the port MC map: this port's MC flows now follow the switch-level
 * MC map if set, otherwise the UC map (see resolution order in 2.3). */
port_attr.id        = SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP_MULTICAST;
port_attr.value.oid = SAI_NULL_OBJECT_ID;
sai_set_port_attribute_fn(port_id, &port_attr);

/* Clear the switch MC map: every port without its own MC map now reuses
 * the UC map for MC flows. */
sw_attr.id        = SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP_MULTICAST;
sw_attr.value.oid = SAI_NULL_OBJECT_ID;
sai_set_switch_attribute_fn(switch_id, &sw_attr);
```
