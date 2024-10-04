# HW Based FRR
-------------------------------------------------------------------------------
 Title       | HW Based Fast Re-Route
-------------|-----------------------------------------------------------------
 Authors     | Jai Kumar (Broadcom Inc.)
 Status      | In review
 Type        | Standards track
 Created     | 2024-09-19
 SAI-Version | 1.15
-------------------------------------------------------------------------------

## 1.0  Introduction

SAI supports SW based FRR where the decision to switch over to the secondary path is triggered by the SW.

Following is the current SAI workflow for SW based FRR.
- Create a protection NH
nhg_entry_attrs[0].id = SAI_NEXT_HOP_GROUP_ATTR_TYPE;
nhg_entry_attrs[0].value.u32 = SAI_NEXT_HOP_GROUP_TYPE_PROTECTION;

- Create primary and secondary members (Note members can be NHG as well)
nhgm_entry_attrs[2].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CONFIGURED_ROLE;
nhgm_entry_attrs[2].value.u32 = SAI_NEXT_HOP_GROUP_MEMBER_CONFIGURED_ROLE_PRIMARY;
nhgm_entry_attrs[2].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CONFIGURED_ROLE;
nhgm_entry_attrs[2].value.u32 = SAI_NEXT_HOP_GROUP_MEMBER_CONFIGURED_ROLE_PRIMARY;

- Based on the monitoring object, SW sets the following boolean to trigger switchover
nhg_entry_attrs[1].id = SAI_NEXT_HOP_GROUP_ATTR_SET_SWITCHOVER;
nhg_entry_attrs[1].value.u32 = true;
saistatus = sai_set_next_hop_group_attribute_fn(nhg_id, nhg_entry_attrs);

## 2.0 HW Based Trigger
Main change in this proposal is the trigger. Now SW is not responsible for monitoring an object and triggering the switchover.
HW monitors the configured object and triggers the switch to secondary path based on the state of the monitored object.
For example if a port is being monitored and port goes down then all the NH resolving via this port will be switched over to the secondary path.

## 3.0 SAI Enhancements
Hardware needs to know ahead of time which NHG/NH are part of the secondary group so as to mark them as backup from the configured primary group. For this reason a hint is needed to identify such NHG/NH.

This hint is provided using a new NHG type
```c
    /** Next hop hardware protection group. This is the group backing up the primary in the protection group type and is managed by hardware */
    SAI_NEXT_HOP_GROUP_TYPE_HW_PROTECTION,
```

Additionally port counters are introduced to capture 
- How many times port has participated in the failover
- Drops observed during failover

```c
    /** SAI port stat if HW protection switchover events */
    SAI_PORT_STAT_IF_IN_HW_PROTECTION_SWITCHOVER_EVENTS,

    /** SAI port stat if HW protection switchover related packet drops */
    SAI_PORT_STAT_IF_IN_HW_PROTECTION_SWITCHOVER_DROP_PKTS,
```

## 4.0 Example Workflow


### Topology Example
There are two uplinks from a switch and both are part of the primary and secondary group.
For such case we will
- Create a NHG nhg1 of type PROTECTION and configure NH1/port1 and NH2/port2 NH as primary members
- Create a NHG nhg2 of type HW_PROTECTION with members as NH1/port1 and NH2/port2
- Set NHG nhg2 as a secondary member of NHG nhg1

PROTECTION[nhg1] --> PRIMARY[NH1, NH2], SECONDARY[nhg2]
HW_PROTECTION[nhg2] --> [NH1, NH2]




```c
nh_1_interface_id = 1
nh_2_interface_id = 2
switch_id = 0;

nhg_entry_attrs[0].id = SAI_NEXT_HOP_GROUP_ATTR_TYPE;
nhg_entry_attrs[0].value.u32 = SAI_NEXT_HOP_GROUP_TYPE_PROTECTION;
saistatus = sai_frr_api->create_next_hop_group(&nhg1, switch_id, 1, nhg_entry_attrs);
if (saistatus != SAI_STATUS_SUCCESS) {
    return saistatus;
}

nhg_entry_attrs[0].id = SAI_NEXT_HOP_GROUP_ATTR_TYPE;
nhg_entry_attrs[0].value.u32 = SAI_NEXT_HOP_GROUP_TYPE_HW_PROTECTION;
saistatus = sai_frr_api->create_next_hop_group(&nhg2, switch_id, 1, nhg_entry_attrs);
if (saistatus != SAI_STATUS_SUCCESS) {
    return saistatus;
}

nh_entry_attrs[0].id = SAI_NEXT_HOP_ATTR_TYPE;
nh_entry_attrs[0].value.u32 = SAI_NEXT_HOP_TYPE_IP;
nh_entry_attrs[1].id = SAI_NEXT_HOP_ATTR_IP;
CONVERT_STRING_TO_SAI_IPV4(nh_entry_attrs[1].value, "10.1.1.1");
nh_entry_attrs[2].id = SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID;
nh_entry_attrs[2].value.u64 = nh_1_interface_id;
saistatus = sai_frr_api->create_next_hop(&nh_1_id, switch_id, 2, nh_entry_attrs);
if (saistatus != SAI_STATUS_SUCCESS) {
    return saistatus;
}

nh_entry_attrs[0].id = SAI_NEXT_HOP_ATTR_TYPE;
nh_entry_attrs[0].value.u32 = SAI_NEXT_HOP_TYPE_IP;
nh_entry_attrs[1].id = SAI_NEXT_HOP_ATTR_IP;
CONVERT_STRING_TO_SAI_IPV4(nh_entry_attrs[1].value, "10.1.2.1");
nh_entry_attrs[2].id = SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID;
nh_entry_attrs[2].value.u64 = nh_2_interface_id;
saistatus = sai_frr_api->create_next_hop(&nh_2_id, switch_id, 2, nh_entry_attrs);
if (saistatus != SAI_STATUS_SUCCESS) {
    return saistatus;
}

// Program the primary NH Group member.
nhgm_entry_attrs[0].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID;
nhgm_entry_attrs[0].value.oid = nhg1;
nhgm_entry_attrs[1].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID;
nhgm_entry_attrs[1].value.oid = nh_1_id;
nhgm_entry_attrs[2].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CONFIGURED_ROLE;
nhgm_entry_attrs[2].value.u32 = SAI_NEXT_HOP_GROUP_MEMBER_CONFIGURED_ROLE_PRIMARY;
saistatus = sai_frr_api->create_next_hop_group_member(&nhgm_1_id, switch_id, 2, nhgm_entry_attrs);
if (saistatus != SAI_STATUS_SUCCESS) {
    return saistatus;
}

nhgm_entry_attrs[0].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID;
nhgm_entry_attrs[0].value.oid = nhg1;
nhgm_entry_attrs[1].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID;
nhgm_entry_attrs[1].value.oid = nh_2_id;
nhgm_entry_attrs[2].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CONFIGURED_ROLE;
nhgm_entry_attrs[2].value.u32 = SAI_NEXT_HOP_GROUP_MEMBER_CONFIGURED_ROLE_PRIMARY;
saistatus = sai_frr_api->create_next_hop_group_member(&nhgm_1_id, switch_id, 2, nhgm_entry_attrs);
if (saistatus != SAI_STATUS_SUCCESS) {
    return saistatus;
}

// Program the secondary NH Group member.
nhgm_entry_attrs[0].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID;
nhgm_entry_attrs[0].value.oid = nhg1;
nhgm_entry_attrs[1].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID;
nhgm_entry_attrs[1].value.oid = nhg2;
nhgm_entry_attrs[2].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CONFIGURED_ROLE;
nhgm_entry_attrs[2].value.u32 = SAI_NEXT_HOP_GROUP_MEMBER_CONFIGURED_ROLE_PRIMARY;
saistatus = sai_frr_api->create_next_hop_group_member(&nhgm_2_id, switch_id, 2, nhgm_entry_attrs);
if (saistatus != SAI_STATUS_SUCCESS) {
    return saistatus;
}

nhgm_entry_attrs[0].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID;
nhgm_entry_attrs[0].value.oid = nhg_id;
nhgm_entry_attrs[1].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID;
nhgm_entry_attrs[1].value.oid = nh_2_id;
nhgm_entry_attrs[2].id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CONFIGURED_ROLE;
nhgm_entry_attrs[2].value.u32 = SAI_NEXT_HOP_GROUP_MEMBER_CONFIGURED_ROLE_PRIMARY;
saistatus = sai_frr_api->create_next_hop_group_member(&nhgm_3_id, switch_id, 2, nhgm_entry_attrs);
if (saistatus != SAI_STATUS_SUCCESS) {
    return saistatus;
}
```
