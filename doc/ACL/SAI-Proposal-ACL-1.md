SAI access control lists (ACL) enhancements for SAI 1.0.0
-------------------------------------------------------------------------------
 Title       | SAI ACL Model - Enhancements
-------------|-----------------------------------------------------------------
 Authors     | Cavium Inc.
 Status      | In review
 Type        | Standards track
 Created     | 08/09/2016
 Updated     | 11/17/2016
 SAI-Version | 1.0.0

-------------------------------------------------------------------------------

## Overview ##
SAI Access Control List (ACL) object implements ACL management functions. The SAI 1.0.0 specification in this document clarifies ambiguities and limitations that pre-SAI 1.0.0 ACL model had. This specification is also an attempt to formally document the behavioral model for SAI ACLs. These enhancements are relatively generic and simplifies the ACL model for operators. The primary goal of introducing SAI ACL model for 1.0.0 release is to provide a generic and abstract ACL interface for operators.

In SAI 0.9.1 through 0.9.5 versions, SAI ACL contained three types of objects, ACL table, ACL entry and ACL counter. SAI 1.0.0 introduces ACL group object to that list. An ACL table contains a number of ACL entries, with specific priorities. Each ACL table defines a set of unique match fields for all its ACL entries. Within an ACL table, if a packet matches multiple rules, only the actions from the rule of highest priority are executed. An ACL group handles the case for allowing prioritized resolution of ACL tables associated with the same group. Within an ACL group a packet can match rules in different ACL tables and take non-conflicting actions from all the matched rules. ACL counters can also be created and attached to an ACL entry in order to count the number of packets or bytes that match the ACL entry. In the SAI 1.0.0 (and as agreed in the community meeting 10/17/2016), ACL group behavior is currently restricted to parallel lookups with non-conflicting action being resolved. A later effort will clarify the per-ACL group behavior of enabling parallel or serial lookup which is beyond the scope of this proposal.

This specification proposes the following enhancements and changes:
1. Introduce the concept of binding points for ACLs.
2. Introduce ACL group object.
3. Introduce behavioral model for ACL tables and ACL groups.
4. Introduce the usage of metadata fields.
5. Introduce simplified ACL table stages (irrelevant by introducing binding points).
6. Introduce ACL (tables and groups) applied on Tunnels and Bridge Interfaces.
7. Introduce behavioral model when ACL (tables and groups) are applied to saiswitch object.
8. Address scaling concerns in absense of a well defined binding point.

## Binding Points
In SAI all physical and logical interfaces are represented by unique object ids, for eg. ports  - saiport.h, LAGs - sailag.h, RIFs - sairouterintf.h, tunnels - saitunnel.h, and bridge ports - saibridgeintf.h. These are well defined objects in SAI that identify a stage in the pipeline to clearly identify a flow, ingressing and egressing through a switch. The ability to filter, classify, or apply specific rules to the traffic that ingresses or egresses through these objects/interfaces allow applications and operators to focus on the functionality of what they want to achieve (**filtering traffic**) and avoid looking at the internals of the switch asics or relevant acl entry fields. At every binding point SAI APIs allow users to bind an ACL Table or ACL Group identified by their unique object id allocated on creation. The binding attribute provides a generic interface to configure or unconfigure an ACL table or group.

These physical and logical interfaces represented by unique object ids represent distinct bind points to apply ACL tables rules (and ACL groups). Following are the bind points (__already well defined SAI objects__) being used in this proposal:
1. Physical Ports and Lags (saiport.h and sailag.h)
2. VLANs (saivlan.h)
3. Router Interfaces (sairouterintf.h)
4. Tunnels (saitunnel.h)
5. Bridge Ports (saibridgeintf.h) - includes both .1q and .1d bridge ports
6. Sai Switch (saiswitch.h - globally applies to all traffic ingressing and egressing a switch).

### Binding to a physical port object(s)
When an ACL table (or group) is bound to a specific port all the traffic ingressing the port are subject to the match/filter/action rules specified by the INGRESS ACL (or group), similarly all the traffic egressing the port are subject to the match/filter/action rules specified by the EGRESS ACL (or group). Use the following attributes to bind an INGRESS ACL OBJECT or an EGRESS ACL OBJECT on a physical port:
1. SAI_PORT_ATTR_INGRESS_ACL
2. SAI_PORT_ATTR_EGRESS_ACL

### Binding to LAG object(s)
When an ACL table (or group) is bound to a specific LAG interface all the traffic ingressing the LAG are subject to the match/filter/action rules specified by the INGRESS ACL (or group), similarly all the traffic egressing the LAG are subject to the match/filter/action rules specified by the EGRESS ACL (or group). Use the following attributes to bind an INGRESS ACL OBJECT or an EGRESS ACL OBJECT on a physical LAG:
1. SAI_LAG_ATTR_INGRESS_ACL
2. SAI_LAG_ATTR_EGRESS_ACL

### Binding to VLAN object(s)
When an ACL table (or group) is bound to a specific VLAN all the traffic ingressing the switch associated with the VLAN are subject to the match/filter/action rules specified by the INGRESS ACL (or group), similarly all the traffic egressing the switch with the VLAN are subject to the match/filter/action rules specified by the EGRESS ACL (or group). Use the following attributes to bind an INGRESS ACL OBJECT or an EGRESS ACL OBJECT to a VLAN:
1. SAI_VLAN_ATTR_INGRESS_ACL
2. SAI_VLAN_ATTR_EGRESS_ACL

### Binding to RIF object(s)
When an ACL table (or group) is bound to a router interface all the traffic ingressing the switch which is qualified to be processed on the specific router interface, are subject to the match/filter/action rules specified by the INGRESS ACL (or group). Similarly all the traffic egressing the switch routed to a router interface, are subject to the match/filter/action rules specified by the EGRESS ACL (or group). Use the following attributes to bind an INGRESS ACL OBJECT or EGRESS ACL OBJECT on a router interface: 
1. SAI_ROUTER_INTERFACE_ATTR_INGRESS_ACL
2. SAI_ROUTER_INTERFACE_ATTR_EGRESS_ACL

### Binding to tunnel object(s)
When a tunnel is created using the SAI tunnel object (saitunnel.h) there are two unique OIDs provided as an argument to sai_create_tunnel_fn. These two unique OIDs are mandatory attributes while creating a tunnel: (a) SAI_TUNNEL_ATTR_UNDERLAY_INTERFACE (b) SAI_TUNNEL_ATTR_OVERLAY_INTERFACE. For an IPINIP tunnel both these attributes are allocated via the router interface objects (sairouterintf.h) and created as TUNNEL RIFs. For a VXLAN tunnel the underlay interface is identified as a bridge port and the overlay interface is a RIF object, identified as TUNNEL bridge interface and TUNNEL RIF respectively. To bind or apply an ACL table (or group) to a tunnel: implies that the user would like to filter traffic that ingresses or egresses the tunnel overlay interfaces or underlay interfaces. For eg. after an IPINIP tunnel is decap or enacp the packet will go through OVERLAY RIF or UNDERLAY RIF respectively. To simplify the binding points for tunnel, the ACL model does not introduce any new attributes in the saitunnel.h object, and reuse the router interface object's ACL bind point and bridge port's ACL bind point to bind ACL table (or group) to a tunnel object(s).

### Binding to switch object
When an ACL table (or group) is bound to a switch object (globally applied to all traffic), all the traffic ingressing a switch or egress a switch is subject to match/filter/action rules specified by the INGRESS or EGRESS ACL (or group). Primarily all the INGRESSED post-flow lookup traffic is filtered through the SWITCH BIND POINT. Binding an INGRESS ACL to a switch bind point, should result as the same behavior that was supported by SAI_ACL_STAGE_SUBSTAGE_INGRESS_POST_L3 in the pre-SAI 1.0.0 ACL model. To support SAI_ACL_STAGE_SUBSTAGE_INGRESS_PRE_L2 stage, one of the above L2/L3 interface specific bind points will work. This provides backward compatibility to pre-SAI 1.0.0 version of ACL stages and only to be used as a transitionary approach and the last resort. Use the following attributes to bind an INGRESS ACL OBJECT or EGRESS ACL OBJECT globally to a switch:
1. SAI_SWITCH_ATTR_DEFAULT_INGRESS_ACL
2. SAI_SWITCH_ATTR_DEFAULT_EGRESS_ACL

## ACL table bind/unbind model and match behavior
The usage of unique object id allocated for ACL table by the create_acl_table function should be uniformly applied to identify the bind point(s). These bind/unbind points described earlier are logical interfaces defined by: physical ports, LAGs, VLANs, RIFs, tunnels, bridge ports, and switch (global). At any given bind point there can be a set of valid ACl table bound to an interface to filter traffic. Various combinatorial use cases and their behavioral expectations are defined as follows:
1. One valid ACL table bound to a bind point: Higher priority ACL entry is selected , considering the flow matches that entry and its action set is executed.
2. ACL table bound to multiple bind points: For a specific flow, there can be multiple valid bind points configured with a one or more valid ACL table(s). These bind points can be cascaded in the pipeline. In such a scenario: any packet modification action to be executed (like SET VLAN, SET IP, etc.) will be executed at a specific stage of the bind point. Non-conflicting actions will be resolved across multiple ACLs derived from different bind points. However, any terminal action like DROP/TRAP will be executed right away, specifically to keep the health of counters, etc. sane.

Figure 1 shows the relationship between an ACL table and various bind points. Examples 1 and 2 in the subsequent sections show how to bind/unbind an ACL table.

![SAI acl design](figures/sai_aclobjs.png "Figure 1: Relationship between ACL table ID and various binding points.")
__Figure 1: Relationship between ACL table ID and various binding points.__

## ACL group management 
Grouping of ACL tables was supported in the previous version, but the model and managmenet of ACL group is different. This proposal introduces two new objects ACL Group and ACL Members. Both these new objects are managed using UOIDs allocated by their APIs. An ACL group is a group of ACL table(s) that is bound to the bind points previously discussed in this proposal. The following APIs are introduced for ACL Group and ACL Group Member management: 

    create_acl_table_group: creates an acl group (UOID managed: acl_group_id) , that can be bound to any of the bind points pretty much like an acl table.
    remove_acl_table_group: removes an allocated acl group (acl_group_id) for recycle of that id
    set_acl_table_group_attribute: add/udpate/delete acl group members to this acl group, or acl group type, or any other acl group attributes
    get_acl_table_group_attribute; retrieve acl group attributes previously configured
    create_acl_table_group_member: creates an acl table group member (UOID managed: acl_group_member_id) , that is associated with an ACL table and ACL group
    remove_acl_table_group_member: removes an acl table group member 
    set_acl_table_group_member_attribute: add/udpate/delete acl group member attribute - ACL table id associated or the ACL member priority
    get_acl_table_group_member_attribute: retrieve acl group attributes previously configured , and recycle the id

### ACL group type
There are two primary types of ACL groups introduced in this specification - sequential and parallel. The ACL group type configuration is per group object attribute and it defines the packet matching behavior across the ACL tables within a specific ACL group. 
1. Sequential - each ACL table is assigned with a unique priority within that group. With a packet matching multiple ACL entries within the ACL group, only one with the highest table priority within the group and highest entry priority within the acl table wins. 
2. Parallel - all ACL tables are matched and non-conflicting actions are executed.

## ACL group bind/unbind model and match behavior
The purpose of the group object is to group more than one ACL table and allow the group of ACL group be bound to a binding point (same as the ACL table bind points). Once an unique object id for an ACL group is created it can be bound to any of the previously described bind points. ACL group member objects are allocated and associated with a group independent of where the groups are bound to. This gives users flexibility and scalability to manage acl tables associated to acl group members. Existing or new ACL group members can be updated or created and be bound to this ACL groups. Here are the combinatorial use cases and their behavioral expectations: 
1. One valid ACL table group bound to a bind point: Every ACL group member has a priority. Within an ACL group all ACL tables are looked up in parallel or sequentially based on the acl group type attribute configured. 
2. ACL groups bound to multiple bind points: For a specific flow, there can be multiple valid bind points configured with one or more valid ACL groups and/or tables. These bind points can be cascaded in the pipeline. In such a scenario: any packet modification action to be executed (like SET VLAN, SET IP, etc.) will be executed at a specific stage of the bind point. Non-conflicting actions will be resolved across multiple ACLs derived from different bind points. Any terminal action like DROP/TRAP will be executed right away and no further cascading of ACL takes place. 

Figure 2 shows the relationship between ACL GROUPs and various bind points, it also provides a typical use case of allowing ACL TABLEs and ACL GROUPs to coexist and be bound to various bind points. Example 3 below shows how to create an ACL group and bind this ACL group to a port. Example 4 below shows how to bind the same ACL group to multiple bind points. Example 5 shows that an ACL table part of an ACL group can also be applied individually to any other bind point. The SAI implementation should handle such complex but intuitive scenarios and simplify application logic.

![SAI acl group](figures/sai_aclgroups.png "Figure 2: group ID and ACL ID's relation with several binding points. ")
__Figure 2: group ID and ACL IDs relation with several binding points.__

## ACL Stages 
On creation of any ACL table or ACL table group, it is mandatory to provide one MANDATORY_ON_CREATE attributes to "create_acl_table" and "create_acl_table_group":
    - SAI_ACL_TABLE_ATTR_ACL_STAGE
    - SAI_ACL_TABLE_GROUP_ATTR_ACL_STAGE
On the same lines to create an ACL table, host adapters can optionally use CREATE_ONLY attributes to validate and reserve (if valid) an ACL table to be applied for a set of binding points identified by the enum sai_acl_bind_point_type_t:
    - SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST
    - SAI_ACL_TABLE_GROUP_ATTR_ACL_BIND_POINT_TYPE_LIST

Based on attribute to validate ACL table or ACL table group, allows a SAI implementation to explicitly validate the scope of match fields and actions that can be supported at various bind points. Based on these explicit attributes the scope of ACL stages is restricted to INGRESS and EGRESS (removing the PRE_L2 and POST_L3). Hence, this proposal introduces well defined binding points along with specific stage(s) of the logical pipeline where any ACL table or ACL table group can be applied.

A new enumeration is added to handle the types of bind points that are currently supported in the ACL specification. This enum currently does not support bridge instance model.

    typedef enum _sai_acl_bind_point_type_t
    {
        /** Port Bind Point */
        SAI_ACL_BIND_POINT_TYPE_PORT,
    
        /** LAG Bind Point */
        SAI_ACL_BIND_POINT_TYPE_LAG,
    
        /** VLAN Bind Point */
        SAI_ACL_BIND_POINT_TYPE_VLAN,
    
        /** RIF Bind Point */
        SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF,
    
        /** Port Bind Point */
        SAI_ACL_BIND_POINT_TYPE_SWITCH
    } sai_acl_bind_point_type_t;

## Terminal and non-conflicting actions
This section clarifies the ACL actions that can be considered terminal and the ones that can be considered conflicting.

ACL actions that disrupts the normal behavior of a packet flowing through the pipeline like updating the packet command to DROP or TRAP - is termed as terminal.

    SAI_ACL_ACTION_TYPE_PACKET_ACTION : DROP and TRAP

ACL actions that modify the packet for subsequent ACL stages or pipeline stages : these actions are termed immediate and the non-conflicting action resolution happens right away. This means the action of following SET operations are available to the next stage(s):

    SAI_ACL_ACTION_TYPE_REDIRECT,               
    SAI_ACL_ACTION_TYPE_REDIRECT_LIST,      
    SAI_ACL_ACTION_TYPE_FLOOD,              
    SAI_ACL_ACTION_TYPE_COUNTER,            
    SAI_ACL_ACTION_TYPE_MIRROR_INGRESS,     
    SAI_ACL_ACTION_TYPE_MIRROR_EGRESS,      
    SAI_ACL_ACTION_TYPE_SET_POLICER,        
    SAI_ACL_ACTION_TYPE_DECREMENT_TTL,      
    SAI_ACL_ACTION_TYPE_SET_TC,             
    SAI_ACL_ACTION_TYPE_SET_COLOR,          
    SAI_ACL_ACTION_TYPE_SET_INNER_VLAN_ID,  
    SAI_ACL_ACTION_TYPE_SET_INNER_VLAN_PRI, 
    SAI_ACL_ACTION_TYPE_SET_OUTER_VLAN_ID,  

## Examples ##
### Example 1 - Binding an ACL to a port
The following example creates an ACL table and one ACL entry to denys a specific SMAC received on a port.

    // Create an ACL table
    sai_object_id_t acl_table_id1 = 0ULL;
    acl_attr_list[0].id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
    acl_attr_list[0].value.s32 = SAI_ACL_STAGE_INGRESS;

    acl_attr_list[1].id = SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST;
    acl_attr_list[1].value.objlist.count = 1;
    acl_attr_list[1].value.objlist.list[0] = SAI_ACL_BIND_POINT_TYPE_PORT;
 
    acl_attr_list[3].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC;
    acl_attr_list[3].value.booldata = True;

    saistatus = sai_acl_api->create_acl_table(&acl_table_id1, 3, acl_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

    // Create an ACL table entry to deny *src_Mac_to_suppress* mac entry
    acl_entry_attrs[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
    acl_entry_attrs[0].value.oid = acl_table_id1;
    acl_entry_attrs[1].id = SAI_ACL_ENTRY_ATTR_PRIORITY;
    acl_entry_attrs[1].value.u32 = 1;
    acl_entry_attrs[2].id = SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC;
    CONVERT_MAC_TO_SAI_MAC (acl_entry_attrs[2].value.aclfield.data.mac, src_mac_to_suppress);
    saistatus = sai_acl_api->create_acl_entry(&acl_entry, 3, acl_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

    // Bind this ACL table to port1s object id
    port_attr_list.count = 1;
    port_attr_list.list[0].id = SAI_PORT_ATTR_INGRESS_ACL;
    port_attr_list.list[0].value.oid = acl_table_id1;
    sai_port_api->set_port_attribute(port_id, port_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

### Example 2 - Binding an ACL to a router interface
The following example creates an Layer3 ACL and one ACL entry to deny SIP and SPORT received on a router interface.

    // Create an ACL table with IP keys configured 
    sai_object_id_t acl_table_id2 = 0ULL;
    acl_attr_list[0].id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
    acl_attr_list[0].value.s32 = SAI_ACL_STAGE_INGRESS;

    acl_attr_list[1].id = SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST;
    acl_attr_list[1].value.objlist.count = 1;
    acl_attr_list[1].value.objlist.list[0] = SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF;

    acl_attr_list[3].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_IP;
    acl_attr_list[3].value.booldata = True;

    acl_attr_list[4].id = SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT;
    acl_attr_list[4].value.booldata = True;

    saistatus = sai_acl_api->create_acl_table(&acl_table_id2, 4, acl_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    // Create an ACL table entry to deny *src_ip_to_suppress* and *src_l4_port_to_suppress*
    acl_entry_attrs[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
    acl_entry_attrs[0].value.oid = acl_table_id2;
    acl_entry_attrs[1].id = SAI_ACL_ENTRY_ATTR_PRIORITY;
    acl_entry_attrs[1].value.u32 = 1;
    acl_entry_attrs[2].id = SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP;
    CONVERT_STR_TO_IP(acl_entry_attrs[2].value.aclfield.data.ip4, "192.168.100.100");
    acl_entry_attrs[3].id = SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT; 
    acl_entry_attrs[3].value.aclfield.data.u16 = 1000;
    saistatus = sai_acl_api->create_acl_entry(&acl_entry, 4, acl_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    // Bind this ACL table to a router interface *rifid10* 
    rif_attr_list.count = 1;
    rif_attr_list.list[0].id = SAI_ROUTER_INTERFACE_ATTR_INGRESS_ACL;
    rif_attr_list.list[0].value.oid = acl_table_id2;
    saistatus = sai_router_interface_api->set_router_interface_attribute(rifid10, 1, rif_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    // Unbind any ACL from the router interface *rifid10* 
    rif_attr_list.count = 1;
    rif_attr_list.list[0].id = SAI_ROUTER_INTERFACE_ATTR_INGRESS_ACL;
    rif_attr_list.list[0].value.oid = SAI_NULL_OBJECT_ID;
    saistatus = sai_router_interface_api->set_router_interface_attribute(rifid10, 1, rif_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    // Remove the ACL table created earlier *acl_table_id2*
    saistatus = sai_acl_api->remove_acl_table(acl_table_id2);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

### Example 3 - Create an ACL group 
This example creates and ACL group with more than one ACL table and bind it to a port, the very same way an ACL table was bound to a port in Example 1. 

    // CREATE AN INGRESS ACL TABLE GROUP
    sai_object_id_t acl_grp_id1 = 0ULL;
    acl_grp_attr[0].id = SAI_ACL_TABLE_GROUP_ATTR_ACL_STAGE;
    acl_grp_attr[0].value.s32 = SAI_ACL_STAGE_INGRESS;

    acl_grp_attr[1].id = SAI_ACL_TABLE_GROUP_ATTR_ACL_BIND_POINT_TYPE_LIST;
    acl_grp_attr[1].value.objlist.count = 1;
    acl_grp_attr[1].value.objlist.list[0] = SAI_ACL_BIND_POINT_TYPE_PORT;

    acl_grp_attr[2].id = SAI_ACL_TABLE_GROUP_ATTR_TYPE;
    acl_grp_attr[2].value.s32 = SAI_ACL_TABLE_GROUP_SEQUENTIAL;

    saistatus = sai_acl_api->create_acl_table_group(&acl_grp_id1, 2, acl_grp_attr);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

    // Create an ACL table *acl_table_id3* , to be part of this group *acl_grp_id1*
    sai_object_id_t acl_table_id3 = 0ULL;
    acl_attr_list[0].id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
    acl_attr_list[0].value.s32 = SAI_ACL_STAGE_INGRESS;

    acl_attr_list[1].id = SAI_ACL_TABLE_GROUP_ATTR_ACL_BIND_POINT_TYPE_LIST;
    acl_attr_list[1].value.objlist.count = 1;
    acl_attr_list[1].value.objlist.list[0] = SAI_ACL_BIND_POINT_TYPE_PORT;

    acl_attr_list[2].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC;
    acl_attr_list[2].value.booldata = True;

    saistatus = sai_acl_api->create_acl_table(&acl_table_id3, 4, acl_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
   
    // Create an ACL table entry to deny *src_mac_to_suppress2*
    acl_entry_attrs[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
    acl_entry_attrs[0].value.oid = acl_table_id3;
    acl_entry_attrs[1].id = SAI_ACL_ENTRY_ATTR_PRIORITY;
    acl_entry_attrs[1].value.u32 = 1;
    acl_entry_attrs[2].id = SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC;
    CONVERT_MAC_TO_SAI_MAC (acl_entry_attrs[2].value.aclfield.data.mac, src_mac_to_suppress2);
    saistatus = sai_acl_api->create_acl_entry(&acl_entry, 3, acl_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
  
    // Create an acl group member with acl_table_id3 and acl_grp_id1
    sai_object_id_t acl_grp_mem1 = 0ULL;
    acl_mem_attr[0].id = SAI_ACL_TABLE_GROUP_MEMBER_ATTR_ACL_TABLE_GROUP_ID;
    acl_mem_attr[0].value.s32 = acl_grp_id1;

    acl_mem_attr[1].id = SAI_ACL_TABLE_GROUP_MEMBER_ATTR_ACL_TABLE_ID;
    acl_mem_attr[1].value.s32 = acl_table_id3;

    acl_mem_attr[1].id = SAI_ACL_TABLE_GROUP_MEMBER_ATTR_PRIORITY;
    acl_mem_attr[1].value.s32 = 100;

    saistatus = sai_acl_api->create_acl_table_group_member(&acl_grp_mem1, 2, acl_grp_attr);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    // Bind this ACL group to port1s OID (in the same way we bound ACL table in Example 1)
    port_attr_list.count = 1;
    port_attr_list.list[0].id = SAI_PORT_ATTR_INGRESS_ACL;
    port_attr_list.list[0].value.oid = acl_grp_id1;
    sai_port_api->set_port_attribute(port_id, port_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

### Example 4 - Binding an ACL group to a set of ports
This example creates an ACL group and binds it to multiple ports.

    // Bind this ACL group *acl_grp_id1* to port2, and port20s OID.
    port_attr_list.count = 1;
    port_attr_list.list[0].id = SAI_PORT_ATTR_INGRESS_ACL;
    port_attr_list.list[0].value.oid = acl_grp_id1;
    
    //port2
    sai_port_api->set_port_attribute(port2, port_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    //port20
    sai_port_api->set_port_attribute(port20, port_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

### Example 5 - Binding an ACL table individually to a specific bind point, even though the acl table was part of an ACL group 
This example shows how to bind an ACL table to a port, especially that ACL table is part of an ACL group.

    // ACL table *acl_table_id3*, which participates in acl group *acl_grp_id1* 
    // This example shows that SAI should allow them to be bound to any of the logical interfaces
    // or physical interfaces , as shown in this example it is being bound to port31.
    // at the same time the ACL group is bound to port2, port20 and port1.
    port_attr_list.count = 1;
    port_attr_list.list[0].id = SAI_PORT_ATTR_INGRESS_ACL;
    port_attr_list.list[0].value.oid = acl_table_id3;
    sai_port_api->set_port_attribute(port31, port_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

## References ##
1. SAI v0.9.1 specification.
