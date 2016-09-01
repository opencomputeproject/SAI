SAI access control lists (ACL) enhancements for SAI 1.0 release
-------------------------------------------------------------------------------

 Title       | SAI ACL Model - Enhancements
-------------|----------------------
 Authors     | Cavium Inc.
 Status      | In review
 Type        | Standards track
 Created     | 08/09/2016
 SAI-Version | 1.0.0

-------------------------------------------------------------------------------

## Overview ##
SAI Access Control Lists (ACL) object implements ACL management functions. In SAI 0.9.1 through 0.9.5 versions, SAI ACL contained three types of objects, ACL table, ACL entry and ACL counter. The ACL table contains a number of ACL entries. Each ACL table defines a set of unique matching fields for all its ACL entries. A packet can match rules in different ACL tables and take non-conflicting actions from all the matched rules. However, within an ACL table, if a packet matches multiple rules, only the actions from the rule of highest priority are executed. ACL counters can also be created and attached to an ACL entry in order to counter the number of packets or bytes that match the ACL entry. The initial version of ACL table object has several ambiguities and limitations. We propose the following enhancements to address some of those and propose a generic and simple ACL model for operators:

1. Well defined binding point for an ACL table
2. Well defined behavior and usage of ACL group ID and metadata fields
3. ACL table stages that were relevant in absense of a binding point
4. Scaling issues in absense of a well defined binding point
5. Missing behavioral model for ACL stage(s)

In this proposal, we provide a model for binding an ACL table, clarify the usage of group IDs and metadata with UOID based ACL Table ID, mapping of ACL stages relevant to binding points, and tunnel specific ACL behavior. We also introduce the behavioral model for ACLs.

### Binding Points
In SAI all physical and logical interfaces are represented by a UOID (for eg. ports  - saiport.h, LAGs - sailag.h, RIFs - sairouterintf.h, tunnels, bridge ports, etc). These are well defined objects in SAI that allows a packet or a flow to ingress or egress through a switch. The ability to filter, classify, or apply specific rules to the traffic that ingresses or egresses through these objects/interfaces allows applications and operators to focus on the functionality of what they want to achieve, and avoid looking at the internals of the switch asic. 

The physical and logical interfaces represented by UOIDs, should be well defined and serves as clear binding points to apply ACL tables rules. 

### ACL TABLE ID Bind/Unbind Model 
We propose the usage of UOID based ACL Table ID allocated by the create_acl_table function should be uniformly applied to identify the binding point. This bind/unbind point is typically identified by various physical and or logical interfaces identified by various objects: 
1. Physical Ports and Lag (saiport.h and sailag.h)
2. VLANs (saivlan.h)
3. Router Interfaces (sairouterintf.h)
4. Bridge Ports (saibridgeintf.h) - includes both .1q and .1d bridge ports
5. Globally apply to all packets (saiswitch.h). Currently the limitation is to be able to bind only one ACL ID per switch. This is to support backward compatibility to pre- SAI 1.0 ACL model.

Considering there are various ACL IDs derived from several binding points, there are use cases where more than one valid ACL ID can be associated for a particular flow. The behavioral expectation is to apply all the valid ACL IDs derived from different binding points, but apply the ACL tables in the order of their priority.

![SAI acl design](figures/sai_aclobjs.png "Figure 1: Relationship between ACL Table ID and various binding points.")
__Figure 1: Relationship between ACL Table ID and various binding points.__

### Group ID Binding Model
Group ID is an object ID based identifier that is a typically a software only object allocated via saiacl.h apis "create_acl_group". The purpose of the group object is to group several ACL tables logically, and then allow the group ID to be bound to a specific binding point. This proposal introduces the group ID management APIs to saiacl.h. Figure 2 conceptually shows the use case of allowing group IDs be configured to various binding points. This also introduces the group ID configuration attribute at various binding points. Naturally group ID attribute once configured should superseed the acl ID configured on any binding point. 

![SAI acl group](figures/sai_aclgroups.png "Figure 2: Group ID and ACL ID's relation with several binding points. ")
__Figure 2: Group ID and ACL ID's relation with several binding points.__

### Group ID Management
Two new APIs are introduced in saiacl.h object to manage group ID creation and removal. create_acl_group API allocates a UOID based group id and remove_acl_group API removes/frees the UOID for recycle.

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
