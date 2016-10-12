SAI access control lists (ACL) enhancements for SAI 1.0 release
-------------------------------------------------------------------------------

 Title       | SAI ACL Model - Enhancements
-------------|----------------------
 Authors     | Cavium Inc.
 Status      | In review
 Type        | Standards track
 Created     | 08/09/2016
 Updated     | 10/10/2016
 SAI-Version | 1.0.0

-------------------------------------------------------------------------------

## Overview ##
SAI Access Control List (ACL) object implements ACL management functions. In SAI 0.9.1 through 0.9.5 versions, SAI ACL contained three types of objects, ACL table, ACL entry and ACL counter. The ACL table contains a number of ACL entries. Each ACL table defines a set of unique matching fields for all its ACL entries. A packet can match rules in different ACL tables and take non-conflicting actions from all the matched rules. However, within an ACL table, if a packet matches multiple rules, only the actions from the rule of highest priority are executed. ACL counters can also be created and attached to an ACL entry in order to counter the number of packets or bytes that match the ACL entry. The initial version of ACL table object has several ambiguities and limitations.

This proposal introduced for SAI 1.0.0, proposes the following enhancements:
1. Introduce well defined binding point for an ACL table (or ACL Group) to be applied
2. Clarify the behavior and usage of ACL group ID and metadata fields
3. Simplify ACL table stages that were relevant in absense of a binding point
4. Address scaling concerns in absense of a well defined binding point
5. Introduce behavioral model for ACLs
6. Adding tunnel and bridge interface specific ACL behavior

These enhancements are relatively generic and simplifies the ACL model for operators.

### Binding Points
In SAI all physical and logical interfaces are represented by a UOID (for eg. ports  - saiport.h, LAGs - sailag.h, RIFs - sairouterintf.h, Tunnels - saitunnel.h, Bridge Ports - saibridgeintf.h, etc). These are well defined objects in SAI that identify a flow ingressing and egressing through a switch. The ability to filter, classify, or apply specific rules to the traffic that ingresses or egresses through these objects/interfaces allow applications and operators to focus on the functionality of what they want to achieve (filtering traffic), and avoid looking at the internals of the switch asics.

These physical and logical interfaces represented by UOIDs are well defined binding points to apply ACL tables rules (and ACL groups). Following are the binding points introduced in SAI 1.0.0:
1. Physical Ports and Lags (saiport.h and sailag.h)
2. VLANs (saivlan.h)
3. Router Interfaces (sairouterintf.h)
4. Tunnels (saitunnel.h)
5. Bridge Ports (saibridgeintf.h) - includes both .1q and .1d bridge ports
6. Sai Switch (saiswitch.h - globally applies to all traffic ingressing and egressing a switch).

Binding an ACL using SAI_SWITCH_ATTR_DEFAULT_INGRESS_ACL_ID / SAI_SWITCH_ATTR_DEFAULT_EGRESS_ACL_ID to a saiswitch object, allows an operator to define ACL rules to globally apply a filter to all traffic flowing through the switch. This provides backward compatibility to pre-SAI 1.0.0 version of ACLs and only to be used as a transitionary approach. 

### ACL TABLE ID Bind/Unbind Model 
The usage of UOID based ACL table ID allocated by the create_acl_table function should be uniformly applied to identify the binding point. This bind/unbind point is typically identified by various physical and or logical interfaces identified by these objects: Physical Ports, LAGs, VLANs, RIFs, Tunnels, Bridge Ports, and SaiSwitch.

Considering that binding an ACL to several logical interfaces can lead to use cases where more than one ACL becomes valid for a specific flow. The behavioral expectation is to apply all the valid ACL IDs derived from various binding points - in the order of their table priorities and within a table use an ACL entry's priority to resolve such conflicts. In addition the ACL model today does not support nor expects non-conflicting action resolution to take place. Example 1 and 2 below shows how to bind and unbind an ACL table. Figure 1 shows the relationship between an ACL TABLE and various bind points.

![SAI acl design](figures/sai_aclobjs.png "Figure 1: Relationship between ACL Table ID and various binding points.")
__Figure 1: Relationship between ACL Table ID and various binding points.__

#### Example 1 - Binding an ACL to a port
// create ACL 
// update ACL table entry
// bind an Acl to a port 
// update ACL table entry
// unbind an acl from a port 

#### Example 2 - Binding an ACL to a router interface
// Use the ACL created in example 1
// bind an ACL to a router interface
// unbind an acl from a router interface

### Group ID Management
Two new APIs are introduced in saiacl.h object to manage group ID creation and removal. create_acl_group API allocates a UOID based group id and remove_acl_group API removes/frees the UOID for recycle.

### ACL GROUP ID Bind/Unbind Model 
ACL GROUP ID is an UOID based identifier allocated via saiacl.h apis "create_acl_group". The purpose of the group object is to group more than one ACL tables and allow the group of ACL tables be bound to any bind points. This proposal introduces the ACL GROUP ID management APIs within saiacl.h. Figure 2 shows the relationship between ACL GROUPs and various bind points, it also provides a typical use case of allowing ACL TABLEs and ACL GROUPs to coexist and be bound to various bind points. Example 3 below shows how to create an ACL group and Example 4 below shows how to bind an unbind an ACL group. Example 5 shows the configuration example used in Figure 2.

![SAI acl group](figures/sai_aclgroups.png "Figure 2: Group ID and ACL ID's relation with several binding points. ")
__Figure 2: Group ID and ACL ID's relation with several binding points.__

#### Example 3 - Create an ACL group 
// create an ACL table
// create an ACL table 2 with priority
// create an acl group using ACL group (new) apis

#### Example 4 - Binding na ACL group to a set of ports
// use the same acl group used in Example 3 
// bind acl group to port 2, 3, 4. 
// unbind acl group from port 3

#### Example 5 - 
// configuration example used in Figure 2. 

### ACL Table and ACL Group Match Behavior
Within one ACL GROUP TABLE only one ACL entry will be hit which is based on the table priority as well as ACL entry priority within the table. Since only on ACL Group (or ACL Table) can be associated with a bind point, it is clear that only one entry (if hit) has corresponding actions to be taken. SAI 1.0.0 does not support resolution of non-conflicting actions across various ACL Groups. However, considering the behavioral pipeline (model) for SAI 1.0.0, multiple bind points can be valid for a specific flow. There are use cases when multiple ACL Tables and/or ACL Groups are assigned at different bind points, but the very first bind point with a valid ACL entry (if hit - irrespective of an ACL Table or ACL Group) takes precedence over rest of the entries. This avoids resolving non-conflicting actions across various ACL Groups.

### Metadata Usage Model
Metadata is a completely user defined field or an identifier that does not need to be allocated within the SAI implementtaions. The Metadata field(s) in the logical pipeline is to allow users to derive a *Metadata* field from any SAI objects (ports, vlans, rifs, bridge ports, Etc.), as well as flow tables (like unicast/multicast FDBs, Neighbor table, acl table entries, route entries). Currently the Metadata field derived at various stages of the pipeline are appended to each other and a specific META_DATA is being used for lookup in the ACL entry. 

### ACLs on Tunnels 
Tunnel interfaces are defined by saitunnel.h. The following tunnel attributes can be configured on the Decap flow as well as Encap flows: 
* SAI_TUNNEL_ATTR_DECAP_INGRESS_ACL_ID [Ingress ACL table bound to inner packets ]
* SAI_TUNNEL_ATTR_DECAP_INGRESS_ACL_GROUP_ID [Ingress ACL group bound to inner packets]
* SAI_TUNNEL_ATTR_ENCAP_EGRESS_ACL_ID [EGRESS ACL bound on encap or originated tunnels]

### ACL Stages 
Based on various binding points, the scope of the ACL stages are restricted to primarily INGRESS and EGRESS. The ingress stage of the ACL table gets applied to various flows right after the determinition of the type of interface. For a bridge flow after the port or the bridge port determination, for the router flow right after the rif determination, and for a tunnelled flow it is after the tunnel decap and tunnel determination stage. Please refer to the ACL changes to the behavioral pipeline model for various ACL stages.

## FAQs 
1. Clarify the usage of binding point acl_id with the acl_table_id that is allocated from the saiacl.h object?
    - On creating an ACL table via create_acl_table API generates a UOID for a unique ACL table (or unique ACL group table using create_acl_group api). This UOID is used to bind this ACL table or the ACL Group to a binding point(s) that are idnetified by their UOIDs representing ports, vlans, lags, rifs, bridge-ifs, etc. The ACL table or the ACL group will be a hit if the object type its bound to is hit  ,and the ACL UOID is derived from the binding point (ports, vlan, rif, lag, etc..). 
    - The purpose of having bind points is to allow duplication of rules in the ACL table when ports, vlans, rifs, lags, are MATCH KEYs in the ACL entry. However, this proposal does not restrict any of those less efficient behaviors, provided underlying switch ASIC has to use them.
2. ACL table ids and group-ids are not one of the key fields in the ACL table entries, but they only point to the table(s) to be used? 
    - Yes. Binding point UOIDs (ports, vlans, etc..) are not match keys in the ACL table which gets bound. Binding points (port, lag, vlans, etc.. UOIDs) are configured/bound by an acl_table_id , so they derive an “acl_table_id” which is the table being used to filter all the traffic.
3. Multiple bind points will come in picture as, say a packet in Rx on port a (bind point table id is x), and the incoming vlan is b (bind point is table id y) and finally the port a, Vlan b is rif c (bind point is table id z). So now this packet will give me 3 bind points and that means it will look-up table x, y and z and how does the packet gets processed ? 
    - Multiple bind points is a valid scenario, where the table priority should take precedence since there are multiple tables being looked up. In case of several tables having the same priority in that case the table created earlier should have higher priority in terms of resolution. This is simply to make the model and usage behavior deterministic and avoid implementation specific differences.
4. Now for a port a’ suppose one wants to hit table id’s x’, y’ and z’ then a group G needs to be formed between x’, y’ and z', and use G as group ID. Can we clarify this behavior?
    - Yes. For tables x’, y’ and z’ -> use group id G. That is associated or bound to port a’. IN that case flows hitting port a' will derive group ID G, and lookup ACL tables x', y', and z' in the prioritized order. The idea is to use the ACL table as a normal ACL table, but allow one level of grouping between ACLs. Again, this is grouping between ACL tables and not ACL entries. To group ACL entries metadata should be used.
5. Meta data is one of the key fields so it will not interfere with the ACL ids/Group ids. The match/resolution are not conflicting between ACL/Group-id and meta-data. Meta data will interfere with the bind-point index, because if one uses meta data from the interface tables, he would want to match all the entries with the same meta data. Meta from flow entries is still ok. Because same port traffic will hit multiple of them.
    - Metadata is designed to provide more granularity within the ACL Entry rules to be matched. Metadata might interfere with the bind-point, but the idea is to use group_id there and not metadata. Metadata is used to have more granularities within the ACL table entry rules, ie. grouping ACL entry rules as opposed to grouping ACL tables. Especially metadata can be derived from flow entries. 
6. We have port, vlan, rif in the egress and ingress both. While using for the binding point, how we get the direction.
    - When an ACL table is created the direction is specified, so UOID carries the ING or EGR direction of any ACL table. When an ingress ACL UOID is bound to a binding point, there is already a direction known, same for the egress. 

## Example ##
1. TBD: Examples show how to define ACLs, Group IDs, Metadata, Etc.
2. TBD: Update pipeline model after review and incorporate feedbacks. 

## References ##
1. SAI v0.9.1 specification.

## Next Steps
1. Community Review - 09/01/16
2. Update examples
3. Update pipeline models
