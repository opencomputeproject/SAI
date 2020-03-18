# SAI Isolation Groups
Isolation Groups are used to prevent traffic from being forwarded  to the   
isolation group members

## Isolation Group Type

There are two types of Isolation Groups:
1. Port Isolation Groups
2. Bridge-Port Isolation Groups

### Port Isolation Group
Port Isolation Groups are used to prevent traffic coming  on a physical port from going out of set of physical ports. They would prevent both bridged and routed packets from going out via the physical port. The members of Port Isolation Groups would be port objects. Port Isolation Group can be applied only on a port object.

### Bridge-Port Isolation Group
Bridge-Port Isolation Groups are used to prevent traffic coming  on a Bridge-Port from going out of set of Bridge-Ports. They would prevent only Bridged (L2 Switched) packets from going out of other bridged ports. The members of Bridge-Port Isolation Groups would be Bridge-Port objects. Bridge-Port Isolation Group can be applied only on a Bridge-Port object.

*Note: If port isolation group is applied on underlying physical port of bridge port and at the same time bridge port isolation group is applied on the bridge port, then switched traffic would be prevented from being forwarded to the members of bridge port isolation group.
Both switched and routed traffic would be prevented from being forwarded to the members of the port isolation group.*

### Example configuration

#### Port Isolation Group - Example configuration

```
/*Assume the requirement is to prevent all packets(switched and routed)
  coming on port1 from going out via port2 and port3 */

sai_object_id_t isolation_group_oid;  
sai_object_id_t mem_id1;  
sai_object_id_t mem_id2;  
sai_attribute_t attr, mem_attr[2];  
sai_status_t sai_rc;

/*Create port isolation group */

attr.id = SAI_ISOLATION_GROUP_ATTR_TYPE  
attr.value.s32 = SAI_ISOLATION_GROUP_TYPE_PORT;

sai_rc = sai_create_isolation_group(&isolation_group_oid, switch_id, 1, &attr);  

/*Create the first port isolation group member object*/

mem_attr[0].id = SAI_ISOLATION_GROUP_MEMBER_ATTR_ISOLATION_GROUP_ID;  
mem_attr[0].value.oid = isolation_group_oid;
mem_attr[1].id = SAI_ISOLATION_GROUP_MEMBER_ATTR_ISOLATION_OBJECT;
mem_attr[1].value.oid = port2_oid;

sai_rc = sai_create_isolation_group_member(&mem_id1, switch_id, 2, &mem_attr);

/*Create the second port isolation group member object*/

mem_attr[1].value.oid = port3_oid;  

sai_rc = sai_create_isolation_group_member(&mem_id2, switch_id, 2, &mem_attr);

/*Set the port isolation group on port 1 to prevent packets getting
  forwarded to members of the port isolation group*/

attr.id = SAI_PORT_ATTR_ISOLATION_GROUP;
attr.value.oid = isolation_group_oid;

sai_rc = sai_set_port_attribute(port1_oid,&attr);
```

#### Bridge-Port Isolation Group - Example configuration

```
/*Assume the requirement is to prevent switched packets
  coming on bridge-port1 from going out via bridge-port2 and
  bridge-port3 */

sai_object_id_t isolation_group_oid;  
sai_object_id_t mem_id1;  
sai_object_id_t mem_id2;  
sai_attribute_t attr, mem_attr[2];  
sai_status_t sai_rc;

/*Create bridge port isolation group */

attr.id = SAI_ISOLATION_GROUP_ATTR_TYPE  
attr.value.s32 = SAI_ISOLATION_GROUP_TYPE_BRIDGE_PORT;

sai_rc = sai_create_isolation_group(&isolation_group_oid, switch_id, 1, &attr);  

/*Create the first port isolation group member object*/

mem_attr[0].id = SAI_ISOLATION_GROUP_MEMBER_ATTR_ISOLATION_GROUP_ID;  
mem_attr[0].value.oid = isolation_group_oid;
mem_attr[1].id = SAI_ISOLATION_GROUP_MEMBER_ATTR_ISOLATION_OBJECT;
mem_attr[1].value.oid = bridge_port2_oid;

sai_rc = sai_create_isolation_group_member(&mem_id1, switch_id, 2, &mem_attr);

/*Create the second port isolation group member object*/

mem_attr[1].value.oid = bridge_port3_oid;  

sai_rc = sai_create_isolation_group_member(&mem_id2, switch_id, 2, &mem_attr);

/* Set the bridge port isolation group on bridge port 1 to prevent packets getting
   forwarded to members of the bridge port isolation group */

attr.id = SAI_BRIDGE_PORT_ATTR_ISOLATION_GROUP;
attr.value.oid = isolation_group_oid;

sai_rc = sai_set_bridge_port_attribute(bridge_port1_oid,&attr);
```
