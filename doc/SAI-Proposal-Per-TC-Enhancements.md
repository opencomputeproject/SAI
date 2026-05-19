# [SAI] Per-Traffic-Class Enhancements
-------------------------------------------------------------------------------
 Title       | Per-Traffic-Class Enhancements: BUM Flood Control and TAM Bind Point Support.
-------------|-----------------------------------------------------------------
 Authors     | Praneeth Chippada, Ravindranath C K (Marvell)
 Status      | In review
 Type        | Standards track
 Created     | 2025-11-22
 SAI-Version | 1.18
-------------------------------------------------------------------------------

## 1.0  Introduction


The HLD describes the design and implementation of Broadcast, Unknown Unicast, and Multicast (BUM) flood control per traffic class (TC) in SAI. The goal is to provide the ability to control flooding behavior for BUM traffic based on individual traffic classes. This feature helps optimize network resource usage and improve QoS for critical traffic.

Similarly, the new TC object allows binding TAM objects on a per TC granularity across the switch. TAM object bind point per TC object provides flexibitlity to user to attach telemetry to a traffic class.

## 2.0 Abbreviations and Information

|   **Term**   | **Definition**                               |
| ------------ | -------------------------------------------- |
| TC           | Traffic Class                                |
| BUM          | Broadcast, Unknown Unicast, Multicast        |


## 3.0  Behavior

### 3.1 BUM Flood Control per TC
#### Existing Behavior
1) BUM traffic is either allowed or dropped per-VLAN basis. BUM traffic can also be rate-limited on a per-port basis
2) No granularity exists for controlling BUM traffic based on TC and all TCs are treated the same for BUM flooding control.

#### New Behavior
1) Flood control settings are introduced per TC level for BUM traffic.
2) The hostif trap is used to derive the packet action for BUM flooded packets on TCs with flood control enabled. Hostif trap counter can also be used to count the packets hitting the hostif trap.

### 3.2 TAM Object Bind per TC
#### Existing Behavior
1) TAM already supports multiple bind points (Ports, queues, buffers etc.).
2) No provision exists to bind TAM object on a per-TC granularity.

#### New Behavior
1) TAM object bind on a per-TC granularity across switch is added.


## 4.0 SAI Enhancement

### Hostif trap type for BUM flood control per TC
A new hostif trap type is defined to apply the trap action to the flooded packets on the TCs which have flood control enabled.
   ```c
   /**
    * @brief Host interface trap type
    *
    * @flags ranges
    */
   typedef enum _sai_hostif_trap_type_t
   {
      ...
      /**
       * @brief Hostif trap to define flood control on broadcast,
       * unknown unicast and multicast packets per traffic class
       *
       * This applies to packets whose TC object has
       * SAI_TC_ATTR_FLOOD_CONTROL_ENABLE as true.
       *
       * (default packet action is drop)
       */
      SAI_HOSTIF_TRAP_TYPE_TC_FLOOD_CONTROL,
      ...
   } sai_hostif_trap_type_t;
   ```
### TC object attributes
New attributes are added in TC object for enabling flood control feature per traffic class and adding TAM object bind per traffic class.
   ```c
   /**
    * @brief Enum defining TC attributes
    */
   typedef enum _sai_tc_attr_t
   {
      ...

      /**
       * @brief Flood Control Enable
       * Enable flood control on traffic class for broadcast,
       * unknown unicast and multicast traffic
       *
       * Use SAI_HOSTIF_TRAP_TYPE_TC_FLOOD_CONTROL trap type
       * to apply flood control on the TC
       *
       * @type bool
       * @flags CREATE_AND_SET
       * @default false
       */
       SAI_TC_ATTR_FLOOD_CONTROL_ENABLE,
      /**
       * @brief TC Bind point for TAM object
       *
       * @type sai_object_list_t
       * @flags CREATE_AND_SET
       * @objects SAI_OBJECT_TYPE_TAM
       * @default empty
       */
       SAI_TC_ATTR_TAM_OBJECT,

      ...
   } sai_tc_attr_t;
   ```

## 5.0 API Example
1. Create a HostIf Trap with drop action.
2. Enable flood control on TC id 4.
3. Get Flood dropped packet count
4. Disable flood control on TC id 4.
5. Bind TAM objects to the TC object.

### 5.1 Create a HostIf Trap with drop action.
```c
/* Create HostIf Trap with drop action for BUM packets */
attr_count = 0;
sai_attr_list[attr_count].id = SAI_HOSTIF_TRAP_ATTR_TRAP_GROUP;
sai_attr_list[attr_count++].value.oid = test_trap_group;
sai_attr_list[attr_count].id = SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION;
sai_attr_list[attr_count++].value.s32 = SAI_PACKET_ACTION_DROP;
sai_attr_list[attr_count].id = SAI_HOSTIF_TRAP_ATTR_COUNTER_ID;
sai_attr_list[attr_count++].value.u32 = counter_id;
sai_attr_list[attr_count].id = SAI_HOSTIF_TRAP_ATTR_TRAP_TYPE;
sai_attr_list[attr_count++].value.s32 = SAI_HOSTIF_TRAP_TYPE_TC_FLOOD_CONTROL;
sai_create_hostif_trap_fn(
    &trap_oid,
    switch_id,
    attr_count,
    sai_attr_list);
```

### 5.2 Enable flood control on TC id 4


```c
/* Create TC object with index 4 and enable flood control */
attr_count = 0;
sai_attr_list[attr_count].id = SAI_TC_ATTR_INDEX;
sai_attr_list[attr_count++].value.u32 = 4;
sai_attr_list[attr_count].id = SAI_TC_ATTR_FLOOD_CONTROL_ENABLE;
sai_attr_list[attr_count++].value.booldata  = true;

sai_create_tc_fn(
   &tc_oid,
   switch_id,
   attr_count,
   sai_attr_list);
```
### 5.3 Get Flood-Dropped Packet Count
```c
/* Read the HostIf Trap counter for flood-dropped packets */
counter_attr.id = SAI_COUNTER_ATTR_PACKETS;
sai_get_counter_attribute_fn(
    counter_id,
    1,
    &counter_attr);
printf("Flood dropped packets = %" PRIu64 "\n",
       counter_attr.value.u64);
```

### 5.4 Disable Flood Control on TC ID 4

```c
/* Disable flood control on TC ID 4 */
attr_count = 0;
sai_attr_list[attr_count].id = SAI_TC_ATTR_FLOOD_CONTROL_ENABLE;
sai_attr_list[attr_count++].value.booldata = false;

sai_set_tc_attribute_fn(
    tc_oid,
    sai_attr_list);

```
### 5.5 Bind TAM Objects to the TC Object
Below API example shows the creation of TAM objects and their binding it on TC object to capture Queue Tail dropped packets to remote collector.
```c
/* Create a Transport Object */
count = 0;
sai_attr_list[count].id = SAI_TAM_TRANSPORT_ATTR_TRANSPORT_TYPE;
sai_attr_list[count].value.s32 = SAI_TAM_TRANSPORT_TYPE_GRE;
count++;
sai_create_tam_transport_fn(
    &tam_transport_id,
    switch_id,
    count,
    sai_attr_list);
/* Create a TAM Collector Object */
count = 0;
memset(attr_list, 0, sizeof(attr_list));
sai_attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_TRANSPORT;
sai_attr_list[count].value.oid = tam_transport_id;
count++;
sai_attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_SRC_IP;
sai_attr_list[count].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_attr_list[count].value.ipaddr.addr.ip4 = 0x0101010a;
count++;
sai_attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_DST_IP;
sai_attr_list[count].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_attr_list[count].value.ipaddr.addr.ip4 = 0x0101010b;
count++;
sai_attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_DESTINATION;
sai_attr_list[count].value.oid = port2_oid;
count++;
sai_attr_list[count].id = SAI_TAM_COLLECTOR_ATTR_DSCP_VALUE;
sai_attr_list[count].value.u8 = 8;
count++;
sai_create_tam_collector(
    &tam_collector_id,
    switch_id,
    count,
    sai_attr_list);
/* Create a TAM Report Object */
count = 0;
sai_attr_list[count].id = SAI_TAM_REPORT_ATTR_TYPE;
sai_attr_list[count].value.s32 = SAI_TAM_REPORT_TYPE_VENDOR_EXTN;
count++;
sai_attr_list[count].id = SAI_TAM_REPORT_ATTR_REPORT_MODE;
sai_attr_list[count].value.s32 = SAI_TAM_REPORT_MODE_ALL;
count++;
sai_create_tam_report_fn(
    &tam_report_id,
    switch_id,
    count,
    sai_attr_list);
/* Create a TAM Event Action Object */
count = 0;
sai_attr_list[count].id = SAI_TAM_EVENT_ACTION_ATTR_REPORT_TYPE;
sai_attr_list[count].value.oid = tam_report_id;
count++;
sai_create_tam_event_action_fn(
    &tam_event_action_id,
    switch_id,
    count,
    sai_attr_list);
/* Create a TAM Event Object */
count = 0;
sai_attr_list[count].id = SAI_TAM_EVENT_ATTR_TYPE;
sai_attr_list[count].value.s32 = SAI_TAM_EVENT_TYPE_QUEUE_TAIL_DROP;
count++;
sai_attr_list[count].id = SAI_TAM_EVENT_ATTR_ACTION_LIST;
sai_attr_list[count].value.objlist.count = 1;
sai_attr_list[count].value.objlist.list[0] = tam_event_action_id;
count++;
sai_attr_list[count].id = SAI_TAM_EVENT_ATTR_COLLECTOR_LIST;
sai_attr_list[count].value.objlist.count = 1;
sai_attr_list[count].value.objlist.list[0] = tam_collector_id;
count++;
sai_create_tam_event_fn(
    &tam_event_id,
    switch_id,
    count,
    sai_attr_list);
/* Create a TAM Object */
count = 0;
attr_list[count].id = SAI_TAM_ATTR_EVENT_OBJECTS_LIST;
attr_list[count].value.objlist.count = 1;
attr_list[count].value.objlist.list[0] = tam_event_id;
count++;
attr_list[count].id = SAI_TAM_ATTR_TAM_BIND_POINT_TYPE_LIST;
attr_list[count].value.objlist.count = 1;
attr_list[count].value.objlist.list[0] = SAI_TAM_BIND_POINT_TYPE_TC;
count++;
sai_create_tam_fn(
    &tam_oid,
    switch_id,
    count,
    attr_list);
/* Bind TAM object to the TC object */
attr_count = 0;
sai_attr_list[attr_count].id = SAI_TC_ATTR_TAM_OBJECT;
sai_attr_list[attr_count].value.objlist.count = 1;
sai_attr_list[attr_count].value.objlist.list[0] = tam_oid;
attr_count++;
sai_set_tc_attribute_fn(
    tc_oid,
    sai_attr_list);
```
