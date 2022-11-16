Hierarchical ECMP
-------------------------------------------------------------------------------
 Title       | SAI support for hierarchical ecmp
-------------|-----------------------------------------------------------------
 Authors     | Jai Kumar, Broadcom Inc.
 Status      | In review
 Type        | Standards track
 Created     | 07/08/2022: Initial Draft
 SAI-Version | 
-------------------------------------------------------------------------------

Hierarchical ECMP provides two level route resolution for equal cost multipaths. First level of route resolution will happen via next hop group containing tunnel and ip nexthops. Tunnel or IP nexthop may resolve via another nexthop group table containing only IP nexthops. Its also possibe that there is only single level of resolution using either one of the nexthop group tables. These two level of nexthop group resolution is provided in HW as two separete tables for performance and fast convergence reasons.

### Usecase
Typical workflow is to first create a nexthop group and then add members to the nexthop group. Today there is no hint present in the nexthop group creation to indicate if it is carrying nexthops that are resolving through another nexthop group. SAI can do the deferred processing of nexthop group and wait for nexthop members to be added. But even then there is no easy way (short of keeping the entire route cache in SAI adapter layer) for SAI to determine the type of nexthops eventually a nexthop group may contain.
This PR provides a hint to be set by NOS to indicate if nexthop group is containing mix of tunnel or IP nexthops or just single hierarchy of ip next hops.

![Hierarchical ECMP](../figures/H-ECMP.png "Figure 1: Hierarchical ECMP")

__Figure 1: NH resolution in Hierarchical Next Hop Group__


Following new attribute is introduced in next hop group object.
```
    /**
     * @brief Hierarchical next hop group level.
     * false: Nexthop group consists of tunnel and IP nexthop
     * true : Nexthop group consists of IP nexthop only
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_NEXT_HOP_GROUP_ATTR_TYPE == SAI_NEXT_HOP_GROUP_TYPE_DYNAMIC_UNORDERED_ECMP
     */
    SAI_NEXT_HOP_GROUP_ATTR_HIERARCHICAL_NEXTHOP,
```


### Sample Workflow
##### Sequence of Programming:
Orchagent gets an aggregated information of the following information (from the higher layer). Nexthop Group Creation along with all the individual nexthop members (as part of the group which needs to be created)

Step 1
Orchagent will create the nexthop group (However, it will iterate through all the nexthop members, which will be part of this group). Based on the condition (either if there is atleast 1 tunnel nexthop or it is pointing to another nexthop group), it needs to create nexthop group with SAI_NEXT_HOP_GROUP_ATTR_IP_NEXTHOP_ONLY set to false.

Step 2
Once the nexthop group is created, it will subsequently add all the nexthop members in the group.

In today's sonic (and FRR) implementation, the nexthop group is never updated. It is always created (with initial set of nexthop members). Routes are pointed to the nexthop group. If any modification needs to be made to the nexthop group (say addition/deletion of nexthop group members), a new nexthop group is created
All the nexthop members are added (which has the most updated list of the nexthop group members)
All the routes are modified to point to the new nexthop group.
The old nexthop members and the nexthop group is deleted
Hence, of there is a transition of a nexthop containing only ip nexthops to ip nexthop plus tunnel nexthop then a always a new nexthop group is created with the SAI_NEXT_HOP_GROUP_ATTR_IP_NEXTHOP_ONLY set to flase (earlier it would be true) and vice versa.

Existing implementation will have a the default value of SAI_NEXT_HOP_GROUP_ATTR_HIERARCHICAL_NEXTHOP as false meaning the current behavior is maintained for backward compataiblity reasons.

Capability query can be used to find of out if a given HW supports hierarchical ECMP supporting SAI_NEXT_HOP_GROUP_ATTR_HIERARCHICAL_NEXTHOP flag.
