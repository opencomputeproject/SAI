SAI access control lists (ACL) enhancements for SAI 1.0.0
-------------------------------------------------------------------------------
 Title       | SAI ACL Model - Enhancements
-------------|----------------------
 Authors     | Cavium Inc.
 Status      | In review
 Type        | Standards track
 Created     | 08/09/2016
 Updated     | 10/13/2016
 SAI-Version | 1.0.0

-------------------------------------------------------------------------------

## Overview ##
SAI Access Control List (ACL) object implements ACL management functions. The SAI 1.0.0 specification in this document clarifies ambiguities and limitations that pre-SAI 1.0.0 ACL model had. This specification is also an attempt to formally document the behavioral model for SAI ACLs. These enhancements are relatively generic and simplifies the ACL model for operators. The primary goal of introducing SAI ACL model for 1.0.0 release is to provide a generic and abstract ACL interface for operators.<br>

In SAI 0.9.1 through 0.9.5 versions, SAI ACL contained three types of objects, ACL table, ACL entry and ACL counter. SAI 1.0.0 introduces ACL group object to that list. An ACL table contains a number of ACL entries, with specific priorities. Each ACL table defines a set of unique match fields for all its ACL entries. Within an ACL table, if a packet matches multiple rules, only the actions from the rule of highest priority are executed. An ACL group handles the case for allowing prioritized resolution of ACL tables associated with the same group. Within an ACL group a packet can match rules in different ACL tables and take non-conflicting actions from all the matched rules. ACL counters can also be created and attached to an ACL entry in order to count the number of packets or bytes that match the ACL entry. 

This specification proposes the following enhancements and changes:<br>
1. Introduce the concept of binding points for ACLs.<br>
2. Introduce ACL group object.<br>
3. Introduce behavioral model for ACL tables and ACL groups.<br>
4. Introduce the usage of metadata fields.<br>
5. Introduce simplified ACL table stages (irrelevant by introducing binding points).<br>
6. Introduce ACL (tables and groups) applied on Tunnels and Bridge Interfaces.<br>
7. Introduce behavioral model when ACL (tables and groups) are applied to saiswitch object.<br>
8. Address scaling concerns in absense of a well defined binding point.<br>

## Binding Points
In SAI all physical and logical interfaces are represented by unique object ids, for eg. ports  - saiport.h, LAGs - sailag.h, RIFs - sairouterintf.h, tunnels - saitunnel.h, and bridge ports - saibridgeintf.h. These are well defined objects in SAI that identify a stage in the pipeline to clearly identify a flow, ingressing and egressing through a switch. The ability to filter, classify, or apply specific rules to the traffic that ingresses or egresses through these objects/interfaces allow applications and operators to focus on the functionality of what they want to achieve (**filtering traffic**) and avoid looking at the internals of the switch asics or relevant acl entry fields.<br>

These physical and logical interfaces represented by unique object ids represent distinct bind points to apply ACL tables rules (and ACL groups). Following are the bind points (__already well defined SAI objects__) being used in this proposal:<br>
1. Physical Ports and Lags (saiport.h and sailag.h)<br>
2. VLANs (saivlan.h)<br>
3. Router Interfaces (sairouterintf.h)<br>
4. Tunnels (saitunnel.h)<br>
5. Bridge Ports (saibridgeintf.h) - includes both .1q and .1d bridge ports<br>
6. Sai Switch (saiswitch.h - globally applies to all traffic ingressing and egressing a switch).<br>

### Binding to a physical port object(s)
When an ACL table (or group) is bound to a specific port all the traffic ingressing the port are subject to the match/filter/action rules specified by the INGRESS ACL (or group), similarly all the traffic egressing the port are subject to the match/filter/action rules specified by the EGRESS ACL (or group). Use the following attributes to bind an INGRESS ACL or an EGRESS ACL on a physical port:<br>
1. SAI_PORT_ATTR_INGRESS_ACL_LIST<br>
2. SAI_PORT_ATTR_EGRESS_ACL_LIST<br>

### Binding to LAG object(s)
When an ACL table (or group) is bound to a specific LAG interface all the traffic ingressing the LAG are subject to the match/filter/action rules specified by the INGRESS ACL (or group), similarly all the traffic egressing the LAG are subject to the match/filter/action rules specified by the EGRESS ACL (or group). Use the following attributes to bind an INGRESS ACL or an EGRESS ACL on a physical LAG:<br>
1. SAI_LAG_ATTR_INGRESS_ACL_LIST<br>
2. SAI_LAG_ATTR_EGRESS_ACL_LIST<br>

### Binding to VLAN object(s)
When an ACL table (or group) is bound to a specific VLAN all the traffic ingressing the switch associated with the VLAN are subject to the match/filter/action rules specified by the INGRESS ACL (or group), similarly all the traffic egressing the switch with the VLAN are subject to the match/filter/action rules specified by the EGRESS ACL (or group). Use the following attributes to bind an INGRESS ACL or an EGRESS ACL to a VLAN:<br>
1. SAI_VLAN_ATTR_INGRESS_ACL_LIST<br>
2. SAI_VLAN_ATTR_EGRESS_ACL_LIST<br>

### Binding to RIF object(s)
When an ACL table (or group) is bound to a router interface all the traffic ingressing the switch which is qualified to be processed on the specific router interface, are subject to the match/filter/action rules specified by the INGRESS ACL (or group). Similarly all the traffic egressing the switch routed to a router interface, are subject to the match/filter/action rules specified by the EGRESS ACL (or group). Use the following attributes to bind an INGRESS ACL or EGRESS ACL on a router interface: <br>
1. SAI_ROUTER_INTERFACE_ATTR_INGRESS_ACL_LIST<br>
2. SAI_ROUTER_INTERFACE_ATTR_EGRESS_ACL_LIST<br>

### Binding to tunnel object(s)
When an ACL table (or group) is bound to a tunneled interface (tunnel terminated flow) , all the traffic ingress the switch and matching the tunnel terminated flow are subject to match/filter/action rules specified by the INGRESS ACL (or group). Similarly all the traffic egressing the switch and qualified to egress a packet matching the tunnel originated flow (encapsulation)  are subject to match/filter/action rules specified by the EGRESS ACL (or group). Use the following attribute to bind an INGRESS ACL or EGRESS ACL to a TUNNEL: <br>
1. SAI_TUNNEL_ATTR_DECAP_INGRESS_ACL_LIST<br>
2. SAI_TUNNEL_ATTR_ENCAP_EGRESS_ACL_LIST<br>

### Binding to switch object
When an ACL table (or group) is bound to a switch object (globally applied to all traffic), all the traffic ingressing a switch or egress a switch is subject to match/filter/action rules specified by the INGRESS or EGRESS ACL (or group). Primarily all the INGRESSED post-flow lookup traffic is filtered through the SWITCH BIND POINT. Binding an INGRESS ACL to a switch bind point, should result as the same behavior that was supported by SAI_ACL_STAGE_SUBSTAGE_INGRESS_POST_L3 in the pre-SAI 1.0.0 ACL model. To support SAI_ACL_STAGE_SUBSTAGE_INGRESS_PRE_L2 stage, one of the above L2/L3 interface specific bind points will work. This provides backward compatibility to pre-SAI 1.0.0 version of ACL stages and only to be used as a transitionary approach and the last resort. Use the following attributes to bind an INGRESS ACL or EGRESS ACL globally to a switch:<br>
1. SAI_SWITCH_ATTR_DEFAULT_INGRESS_ACL_LIST<br>
2. SAI_SWITCH_ATTR_DEFAULT_EGRESS_ACL_LIST<br>

## ACL table bind/unbind model and match behavior
The usage of unique object id allocated for ACL table(s) by the create_acl_table function should be uniformly applied to identify the bind point(s). These bind/unbind points described earlier are logical interfaces defined by: physical ports, LAGs, VLANs, RIFs, tunnels, bridge ports, and switch (global). At any given bind point there can be a set of valid ACl table(s) bound to an interface to filter traffic. Various combinatorial use cases and their behavioral expectations are defined as follows:<br>
1. One valid ACL table bound to a bind point: Higher priority ACL entry is selected , considering the flow matches that entry and its action set is executed.<br>
2. More than one valid ACL tables bound to a bind point: Higher priority ACL entries are selected from each valid ACL table and each entry's action set is executed by resolving non-conflicting actions. Since there are multiple ACL tables their lookups are logically performed in **parallel** when bound to the same binding point.<br>
3. ACL tables bound to multiple bind points: For a specific flow, there can be multiple valid bind points configured with a one or more valid ACL table(s). These bind points can be cascaded in the pipeline. In such a scenario: any packet modification action to be executed (like SET VLAN, SET IP, etc.) will be executed at a specific stage of the bind point. Non-conflicting actions will be resolved across multiple ACLs derived from different bind points. However, any terminal action like DROP/TRAP will be executed right away, specifically to keep the health of counters, etc. sane.<br>

Figure 1 shows the relationship between an ACL table and various bind points. Examples 1 and 2 in the subsequent sections show how to bind/unbind an ACL table.<br>

![SAI acl design](figures/sai_aclobjs.png "Figure 1: Relationship between ACL table ID and various binding points.")
__Figure 1: Relationship between ACL table ID and various binding points.__<br>

## ACL group management 
Grouping of ACL tables was supported in the previous version. This specification enhances the ACL table grouping model and primarily the management of ACL groups - for scalability and flexibility. Two new APIs are introduced in saiacl.h object to manage group ID creation and removal. create_acl_table_group - allocates a unique object ID based ACL group table ID and remove_acl_table_group API removes the unique object ID for recycle. This proposal introduces a new ACL group object. ACL groupid is bound to all the previously discussed bind points that are valid for ACL tables.<br>

## ACL group bind/unbind model and match behavior
The purpose of the group object is to group more than one ACL table and allow the group of ACL group be bound to a binding point (same as the ACL table bind points). Once an unique object id for an ACL group is created, any existing or new ACL table can be updated or created and be bound to this ACL group. Here are the combinatorial use cases and their behavioral expectations: <br>
1. One valid ACL group bound to a bind point: Within one ACL group the highest priority ACL entry will be hit which is based on the ACL table prioirty as well as the ACL entry priority within the table. Every ACL table is associated to a priority which is mandatory on create. The order of the lookup within the ACL table group will be based on the individual priority. Actions from the ACL entry residing in the higher prioirty within the group will be perfoemed.<br>
2. More than one valid ACL groups bound to a bind point: Higher priority ACL entries are selected from each valid ACL group (and their highest priority ACL tables). Each entry's action set is executed by resolving non-conflicting actions. The lookups are logically performed in **parallel**. Hence, across the ACL groups, one hit ACL entry from each group will be selected and all the non-conflicting actions from them will be performed. In absense of grouping all non-conflicting actions from different ACL entry hits across the tables will be performed. In case there is a user configuration for the same priority of ACL table within a group, then the ACL entries behave as one ACL table.<br>
3. ACL groups bound to multiple bind points: For a specific flow, there can be multiple valid bind points configured with one or more valid ACL groups and/or tables. These bind points can be cascaded in the pipeline. In such a scenario: any packet modification action to be executed (like SET VLAN, SET IP, etc.) will be executed at a specific stage of the bind point. Non-conflicting actions will be resolved across multiple ACLs derived from different bind points. Any terminal action like DROP/TRAP will be executed right away and no further cascading of ACL takes place. <br>

Figure 2 shows the relationship between ACL GROUPs and various bind points, it also provides a typical use case of allowing ACL TABLEs and ACL GROUPs to coexist and be bound to various bind points. Example 3 below shows how to create an ACL group and bind this ACL group to a port. Example 4 below shows how to bind the same ACL group to multiple bind points. Example 5 shows that an ACL table part of an ACL group can also be applied individually to any other bind point. The SAI implementation should handle such complex but intuitive scenarios and simplify application logic.<br>

However, we introduce a READ_ONLY attribute "SAI_ACL_NON_CONFLICTING_ACTION" (boolen). If a switch asic and its SAI implementation supports ACL groups to support non-conflicting actions across the ACL tables in the group - it should be feasible.

![SAI acl group](figures/sai_aclgroups.png "Figure 2: group ID and ACL ID's relation with several binding points. ")
__Figure 2: group ID and ACL ID's relation with several binding points.__

## ACL Stages 
Based on various binding points, the scope of the ACL stages are restricted to primarily INGRESS and EGRESS. The ingress stage of the ACL table gets applied to various flows right after the determinition of the type of interface. For a bridge flow after the port or the bridge port determination, for the router flow right after the rif determination, and for a tunnelled flow it is after the tunnel decap and tunnel determination stage.<br>

For the lack of better mapping of stages relative to the binding points, there where PRE_L2 and POST_L3 substages defined in SAI. This proposal introduces well defined binding points which clarifies the stage of the pipeline where any ACL is being applied. Especially all the post-flow lookup traffic is filtered after all the L2/L3/Flow table lookups, and binding an ingress ACL to the switch object will achieve the same result. Similarly the pre_l2 or pre_flow lookup stage of an ACL is directly mapped by configuring an ingress ACL to any of the binding points (ports, lags, vlans, RIFs, etc.).<br>

## Metadata Usage Model
Metadata is a completely user defined field or an identifier that does not need to be allocated within the SAI implementtaions. The Metadata field(s) in the logical pipeline is to allow users to derive a *metadata* field from any SAI objects (ports, vlans, rifs, bridge ports, Etc.), as well as flow tables (like unicast/multicast FDBs, Neighbor table, acl table entries, route entries). Currently the Metadata field derived at various stages of the pipeline are appended to each other and a specific META_DATA is being used for lookup in the ACL entry. 

## Terminal and non-conflicting actions
This section clarifies the ACL actions that can be considered terminal and the ones that can be considered conflicting.<br>

ACL actions that disrupts the normal behavior of a packet flowing through the pipeline like updating the packet command to DROP or TRAP - is termed as terminal.<br>

    SAI_ACL_ACTION_TYPE_PACKET_ACTION : DROP and TRAP

ACL actions that modify the packet for subsequent ACL stages or pipeline stages : these actions are termed immediate and the non-conflicting action resolution happens right away. This means the action of following SET operations are available to the next stage(s):<br>

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
The following example creates an ACL table and one ACL entry to denys a specific SMAC received on a port.<br>

    // Create an ACL table
    sai_object_id_t acl_table_id1 = 0ULL;
    acl_attr_list[0].id = SAI_ACL_TABLE_ATTR_STAGE;
    acl_attr_list[0].value.s32 = SAI_ACL_STAGE_INGRESS;
    acl_attr_list[1].id = SAI_ACL_TABLE_ATTR_PRIORITY;
    acl_attr_list[1].value.s32 = 100;
    acl_attr_list[2].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC;
    acl_attr_list[2].value.booldata = True;
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

    // Bind this ACL table to port1's object id
    port_attr_list.count = 1;
    port_attr_list.list[0].id = SAI_PORT_ATTR_INGRESS_ACL_LIST;
    port_attr_list.list[0].value.oid = acl_table_id1;
    sai_port_api->set_port_attribute(port_id, port_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

### Example 2 - Binding an ACL to a router interface
The following example creates an Layer3 ACL and one ACL entry to deny SIP and SPORT received on a router interface.<br>

    // Create an ACL table with IP keys configured 
    sai_object_id_t acl_table_id2 = 0ULL;
    acl_attr_list[0].id = SAI_ACL_TABLE_ATTR_STAGE;
    acl_attr_list[0].value.s32 = SAI_ACL_STAGE_INGRESS;
    acl_attr_list[1].id = SAI_ACL_TABLE_ATTR_PRIORITY;
    acl_attr_list[1].value.s32 = 100;
    acl_attr_list[2].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_IP;
    acl_attr_list[2].value.booldata = True;
    acl_attr_list[3].id = SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT;
    acl_attr_list[3].value.booldata = True;
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
    rif_attr_list.list[0].id = SAI_ROUTER_INTERFACE_ATTR_INGRESS_ACL_LIST;
    rif_attr_list.list[0].value.oid = acl_table_id2;
    saistatus = sai_router_interface_api->set_router_interface_attribute(rifid10, 1, rif_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    // Unbind any ACL from the router interface *rifid10* 
    rif_attr_list.count = 1;
    rif_attr_list.list[0].id = SAI_ROUTER_INTERFACE_ATTR_INGRESS_ACL_LIST;
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

    // Create an ingress acl table group 
    sai_object_id_t acl_grp_id1 = 0ULL;
    acl_grp_attr[0].id = SAI_ACL_TABLE_GROUP_STAGE;
    acl_grp_attr[0].value.s32 = SAI_ACL_STAGE_INGRESS;
    acl_grp_attr[1].id = SAI_ACL_TABLE_GROUP_ATTR_PRIORITY;
    acl_grp_attr[1].value.s32 = 100;
    saistatus = sai_acl_api->create_acl_table_group(&acl_grp_id1, 2, acl_grp_attr);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    // Update the ACL table created in Example 1, to be part of this group
    acl_attr_list[0].id = SAI_ACL_TABLE_ATTR_GROUP_ID;
    acl_attr_list[0].value.oid = acl_grp_id1;
    saistatus = sai_acl_api->set_acl_table_attribute(acl_table_id1, acl_attr_list[0]);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    // Create an aCL table *acl_table_id3* , to be part of this group *acl_grp_id1*
    sai_object_id_t acl_table_id3 = 0ULL;
    acl_attr_list[0].id = SAI_ACL_TABLE_ATTR_STAGE;
    acl_attr_list[0].value.s32 = SAI_ACL_STAGE_INGRESS;
    acl_attr_list[1].id = SAI_ACL_TABLE_ATTR_PRIORITY;
    acl_attr_list[1].value.s32 = 101;
    acl_attr_list[2].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC;
    acl_attr_list[2].value.booldata = True;
    
    acl_attr_list[3].id = SAI_ACL_TABLE_ATTR_GROUP_ID;
    acl_attr_list[3].value.oid = acl_grp_id1;
    
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
    
    // Bind this ACL group to port1's OID (in the same way we bound ACL table in Example 1)
    port_attr_list.count = 1;
    port_attr_list.list[0].id = SAI_PORT_ATTR_INGRESS_ACL_LIST;
    port_attr_list.list[0].value.oid = acl_grp_id1;
    sai_port_api->set_port_attribute(port_id, port_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

### Example 4 - Binding an ACL group to a set of ports
This example creates an ACL group and binds it to multiple ports.
    // Bind this ACL group *acl_grp_id1* to port2, and port20's OID.
    port_attr_list.count = 1;
    port_attr_list.list[0].id = SAI_PORT_ATTR_INGRESS_ACL_LIST;
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

### Example 5 - Binding an ACL table part of the ACL table group to a specific bind point.
This example shows how to bind an ACL table to a port, especially that ACL table is part of an ACL group.<br>

    // ACL table *acl_table_id3*, which participates in acl group *acl_grp_id1* 
    // This example shows that SAI should allow them to be bound to any of the logical interfaces
    // or physical interfaces , as shown in this example it is being bound to port31.
    // at the same time the ACL group is bound to port2, port20 and port1.
    port_attr_list.count = 1;
    port_attr_list.list[0].id = SAI_PORT_ATTR_INGRESS_ACL_LIST;
    port_attr_list.list[0].value.oid = acl_table_id3;
    sai_port_api->set_port_attribute(port31, port_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

## FAQs 
1. Clarify the usage of binding point acl_id with the acl_table_id that is allocated from the saiacl.h object?
    - On creating an ACL table via create_acl_table API generates a UOID for a unique ACL table (or unique ACL group table using create_acl_group api). This UOID is used to bind this ACL table or the ACL group to a binding point(s) that are idnetified by their UOIDs representing ports, vlans, lags, rifs, bridge-ifs, etc. The ACL table or the ACL group will be a hit if the object type its bound to is hit  ,and the ACL UOID is derived from the binding point (ports, vlan, rif, lag, etc..). 
    - The purpose of having bind points is to allow duplication of rules in the ACL table when ports, vlans, rifs, lags, are MATCH KEYs in the ACL entry. However, this proposal does not restrict any of those less efficient behaviors, provided underlying switch ASIC has to use them.
2. ACL table ids and group-ids are not one of the key fields in the ACL table entries, but they only point to the table(s) to be used? 
    - Yes. Binding point UOIDs (ports, vlans, etc..) are not match keys in the ACL table which gets bound. Binding points (port, lag, vlans, etc.. UOIDs) are configured/bound by an acl_table_id , so they derive an “acl_table_id” which is the table being used to filter all the traffic.
3. Multiple bind points will come in picture as, say a packet in Rx on port a (bind point table id is x), and the incoming vlan is b (bind point is table id y) and finally the port a, Vlan b is rif c (bind point is table id z). So now this packet will give me 3 bind points and that means it will look-up table x, y and z and how does the packet gets processed ? 
    - Multiple bind points is a valid scenario, where the table priority should take precedence since there are multiple tables being looked up. In case of several tables having the same priority in that case the table created earlier should have higher priority in terms of resolution. This is simply to make the model and usage behavior deterministic and avoid implementation specific differences.
4. Now for a port a’ suppose one wants to hit table id’s x’, y’ and z’ then a group G needs to be formed between x’, y’ and z', and use G as group ID. Can we clarify this behavior?
    - Yes. For tables x’, y’ and z’ -> use group id G. That is associated or bound to port a’. IN that case flows hitting port a' will derive group ID G, and lookup ACL tables x', y', and z' in the prioritized order. The idea is to use the ACL table as a normal ACL table, but allow one level of grouping between ACLs. Again, this is grouping between ACL tables and not ACL entries. To group ACL entries metadata should be used.
5. Meta data is one of the key fields so it will not interfere with the ACL ids/group ids. The match/resolution are not conflicting between ACL/group-id and meta-data. Meta data will interfere with the bind-point index, because if one uses meta data from the interface tables, he would want to match all the entries with the same meta data. Meta from flow entries is still ok. Because same port traffic will hit multiple of them.
    - Metadata is designed to provide more granularity within the ACL Entry rules to be matched. Metadata might interfere with the bind-point, but the idea is to use group_id there and not metadata. Metadata is used to have more granularities within the ACL table entry rules, ie. grouping ACL entry rules as opposed to grouping ACL tables. Especially metadata can be derived from flow entries. 
6. We have port, vlan, rif in the egress and ingress both. While using for the binding point, how we get the direction.
    - When an ACL table is created the direction is specified, so UOID carries the ING or EGR direction of any ACL table. When an ingress ACL UOID is bound to a binding point, there is already a direction known, same for the egress. 

## References ##
1. SAI v0.9.1 specification.

## Next Steps
1. Initial community review - 09/01/2016. 
2. Second review with feedback and updates - 10/13/2016.
