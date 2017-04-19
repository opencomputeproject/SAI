SAI IPv6 Segment Routing Proposal for SAI 1.2.0
-------------------------------------------------------------------------------
 Title       | SAI IPv6 Segment Routing
-------------|-----------------------------------------------------------------
 Authors     | Cavium Inc.
 Status      | In review
 Type        | Standards track
 Created     | 04/14/2017
 Updated     | 04/14/2017
 SAI-Version | 1.2.0

-------------------------------------------------------------------------------

## Overview ##

Segment Routing (SR) allows a node to steer a packet through a controlled set of instructions, called segments, by prepending an SR header to the packet.  A segment can represent any instruction, topological or service-based.  SR allows to enforce a flow through any path (topological, or application/service based) while maintaining per-flow state only at the ingress node to the SR domain.

This document covers IPv6-based Segment Routing as per IETF Drafts:
1. https://tools.ietf.org/html/draft-ietf-6man-segment-routing-header-06.  
2. https://tools.ietf.org/html/draft-filsfils-spring-srv6-network-programming-00

This specification proposes the following points:
1. Introduce the concept of Segment Routing using IPv6.
2. Introduce behavioral model modifications to support SR source, transit, and endpoint.
3. Introduce SAI APIs to define SR source / transit properties including segment and TLV definitions.

## Behavioral Model

In order to add IPv6 Segment Routing, it requires two mechanisms:
1. Way to specify which flows will be marked for SR source or transit
2. Way to add SR header (source / transit) or modify / remove SR header (endpoint) before normal processing

For the first mechanism, ingress ACL is used to match on specific IPv6 flows to originate or transit a SR header due to n-tuple match flexbility.  From ACL match, it would derive a policy_id to pass into the Segment Route Source / Transit Table.  One can also use policy_id as a compression mechanism for multiple flows to take the same segment path.

For the second mechanism, the Segment Route Tables would be used to manipulate the SR header information before normal lookup routine.  This would require two mechanisms:
1. Way to specific what segment and/or TLV information would be added in the source / transit case.  This will be programmed via SAI APIs into a match/action within the Source / Transit Table.
2. Way to identify if ingress packet has a SR header existing and SR DIP matches local Segment ID (SID) for the endpoint cases. The segment_exists metadata is set if SR header exists on ingress packet via a previous element such as the parser.

Within the Segment Route Source / Transit Table, if a match on the policy_id is found, the resulting action would add the programmed SR header to the packet so the subsequent route lookup will be on the SR header segment DIP instead of the native DIP.  In the Endpoint table, if a SID matches, the user can define various endpoint functionalities.

Only two new metadata values need to be added, segment_exists and policy_id.

Figure 1 shows the additional logic between the ACL and Router Table in the behavioral pipeline to support this.

![SAI v6SR bm](figures/sai_v6SR_bm.png "Figure 1: Behavioral Model Addition. ")
__Figure 1: Behavioral Model Addition.__

## API Modification

### ACL Table Modification
Adding an additional ACL action / value of SAI_ACL_ACTION_TYPE_SET_POLICY_ID to define the policy_id for native packets to be matched upon for SR source or transit in the Segment Route Source / Transit Table lookup

### Segment Route Source / Transit Table APIs
#### Vendor Support Advertisement

Included is also a way for vendors to advertise devcie support include the number of segments and TLV types that can be originated

    SAI_SEGMENTROUTE_ATTR_NUM_SEGMENTS_SUPPORTED
    SAI_SEGMENTROUTE_ATTR_TLV_TYPE_SUPPORTED

> Note: NSH Carrier and Padding TLVs were not included in this first draft

#### Match Parameter

The sole match parameter is the policy_id passed from the ACL lookup

    SAI_ACL_ACTION_TYPE_SET_POLICY_ID
   
#### Action Parameters

Transit or Source Action to be taken with policy

    SAI_SEGMENTROUTE_ATTR_ST_TYPE

List of DIP segments to be added

    attribute enum: SAI_SEGMENTROUTE_ATTR_SEGMENT_LIST
        
List of TLVs to be added

    attribute enum: SAI_SEGMENTROUTE_ATTR_TLV

### Segment Route Endpoint Table APIs
#### Match Parameter

Match on VRF and IPv6 DIP to local segment ID / Endpoint Table

    sai_segmentroute_endpoint_entry_t

#### Action Parameters

Endpoint Actions to be taken

    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ATTR_ACTION
    SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ATTR_POP

> Note: Not all endpoint actions included

### Segment Route Counter Support

> Note: No counters included in first draft

### APIs

To start with, the basic create/remove entry and set/get attributes APIs are included
 
    create_segmentroute_transit
    remove_segmentroute_transit
    set_segmentroute_st_attribute
    get_segmentroute_st_attribute

    create_segmentroute_endpoint_entry
    remove_segmentroute_endpoint_entry
    set_segmentroute_endpoint_entry_attribute
    get_segmentroute_endpoint_entry_attribute

    create_segmentroute_counter
    remove_segmentroute_counter
    set_segmentroute_counter_attribute
    get_segmentroute_counter_attribute

## Examples ##
### Example 1 - SR Source / Transit
The following example creates an ACL entry to specify a specific flow to bind to policy_id = 1 as well as creating the corresponding entry in the Segment Routing Transit / Source Table to add 3 Segments and an Ingress Node TLV

    acl_entry_attrs[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
    acl_entry_attrs[0].value.oid = acl_table_id2;
    acl_entry_attrs[1].id = SAI_ACL_ENTRY_ATTR_PRIORITY;
    acl_entry_attrs[1].value.u32 = 1;
    acl_entry_attrs[2].id = SAI_ACL_ENTRY_ATTR_FIELD_SRC_IPV6;
    CONVERT_STR_TO_IPV6(acl_entry_attrs[2].value.aclfield.data.ip6, "2001:db8:85a3::8a2e:370:7334");
    acl_entry_attrs[3].id = SAI_ACL_ACTION_TYPE_SET_SEGMENTROUTE; 
    acl_entry_attrs[3].value.aclfield.data.u32 = 1;
    saistatus = sai_acl_api->create_acl_entry(&acl_entry, 4, acl_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

    switch_id = 0;
    v6sr_entry_attrs[0].id = SAI_SEGMENTROUTE_ATTR_SEGMENT_LIST
    v6sr_entry_attrs[0].value.objlist.count = 3;
    CONVERT_STR_TO_IPV6(v6sr_entry_attrs[0].value.objlist.list[0], "2001:db8:85a3::8a2e:370:1234");
    CONVERT_STR_TO_IPV6(v6sr_entry_attrs[0].value.objlist.list[1], "2001:db8:85a3::8a2e:370:2345");
    CONVERT_STR_TO_IPV6(v6sr_entry_attrs[0].value.objlist.list[2], "2001:db8:85a3::8a2e:370:3456");
    v6sr_entry_attrs[1].id = SAI_SEGMENTROUTE_ATTR_TLV;
    v6sr_entry_attrs[1].value.objlist.count = 1;
    v6sr_entry_attrs[1].value.objlist.list[0].tlv_type = SAI_TLV_TYPE_INGRESS;
    CONVERT_STR_TO_IPV6(v6sr_entry_attrs[1].value.objlist.list[0].ingress_node, "2001:db8:85a3::8a2e:370:9876");
    v6sr_entry_attrs[2].id = SAI_SEGMENTROUTE_ATTR_ST_TYPE;
    v6sr_entry_attrs[2].value = SAI_SEGMENTROUTE_ST_TYPE_ENCAPS_ORIGINATION; 

    saistatus = sai_v6sr_api->create_segmentroute_transit(&policy_id, switch_id, 3, v6sr_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

### Example 2 - SR Endpoint 
The following example creates an local SID / Endpoint entry to match on incoming DIP and do a basic endpoint behavior with PSP

    endpoint_entry.switch_id = 0;
    endpoint_entry.vr_id = 0;
    CONVERT_STR_TO_IPV6(endpoint_entry.segment_id, "2001:db8:85a3::8a2e:370:4567");
    v6sr_entry_attrs[0].id = SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ATTR_ACTION;
    v6sr_entry_attrs[0].value = SAI_SEGMENTROUTE_ENDPOINT_ACTION_TYPE_E;
    v6sr_entry_attrs[1].id = SAI_SEGMENTROUTE_ENDPOINT_ENTRY_ATTR_POP;
    v6sr_entry_attrs[1].value = SAI_SEGMENTROUTE_ENDPOINT_POP_TYPE_PSP;

    saistatus = sai_v6sr_api->sai_create_segmentroute_endpoint_entry(&endpoint_entry, 2, v6sr_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

## References ##
1. https://tools.ietf.org/html/draft-ietf-6man-segment-routing-header-06
2. https://tools.ietf.org/html/draft-filsfils-spring-srv6-network-programming-00
