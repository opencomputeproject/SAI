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

This document covers IPv6-based Segment Routing as per IETF Draft: https://tools.ietf.org/html/draft-ietf-6man-segment-routing-header-06.  

This specification proposes the following points:
1. Introduce the concept of Segment Routing using IPv6.
2. Introduce behavioral model modifications to support SR origination, transit, and termination.
3. Introduce SAI APIs to define SR origination properties including segment and TLV definitions.

## Behavioral Model

In order to add IPv6 Segment Routing, it requires two mechanisms:
1. Way to specify which flows will be marked for SR origination
2. Way to add SR header (origination), remove SR header (termination), manipulate SR header to use next segment (transit) before normal IPv6 Route lookup or just do normal IPv6 Routing

For the first mechanism, ingress ACL is used to match on specific native IPv6 flows to originate a SR header due to n-tuple match flexbility.  From ACL match, it would derive a segment_id to pass into the Segment Route Origination Table.  One can also use segment_id as a compression mechanism for multiple flows to take the same segment path.

For the second mechanism, the Segment Route Tables would be used to manipulate the SR header information before IPv6 Route lookup.  This would require two mechanisms:
1. Way to specific what segment and/or TLV information would be added in the origination case.  This will be programmed via SAI APIs into a match/action within the Segment Route Table.
2. Way to identify if ingress packet has a SR header existing and SR DIP is my IP address for the transit and termination cases. The segment_exists metadata is set if SR header exists on ingress pakcet via a previous element such as the parser.

Within the Segment Route Origination Table, if a match on the segment_id is found, the resulting action would add the programmed SR header to the packet so the subsequent route lookup will be on the SR header segment DIP instead of the native DIP.  If segment_exists is set, then the logic will go to the Segment Route Transit/Termnation Table and match on whether the outer DIP matches the router IP.  If it does, the action will replace the outer IP with the next segment information or decapsulate the SR header if it is the last segment.  Ideally, this would be a direct access table.

With this model, only SR origination case requires user API interaction to configure.  Transit and Termination behavior, in current form, is implicit.  And only two new metadata values need to be added, segment_exists and segment_id.

Figure 1 shows the additional logic between the ACL and Router Table in the behavioral pipeline to support this.

![SAI v6SR bm](figures/sai_v6SR_bm.png "Figure 1: Behavioral Model Addition. ")
__Figure 1: Behavioral Model Addition.__

## API Modification

### ACL Table Modification
Adding an additional ACL action / value of SAI_ACL_ACTION_TYPE_SET_SEGMENT_ID to define the segment_id for native packets to be matched upon for SR origination in the next Segment Route Table lookup

### Segment Route Origination Table APIs
#### Vendor Support Advertisement

Included is also a way for vendors to advertise devcie support include the number of segments and TLV types that can be originated

    SAI_SEGMENTROUTE_ATTR_NUM_SEGMENTS_SUPPORTED
    SAI_SEGMENTROUTE_ATTR_TLV_TYPE_SUPPORTED

> Note: NSH Carrier and Padding TLVs were not included in this first draft

#### Match Parameter

The sole match parameter is the segment_id passed from the ACL lookup

    SAI_ACL_ACTION_TYPE_SET_SEGMENT_ID
   
#### Action Parameters

List of DIP segments to be added

    attribute enum: SAI_SEGMENTROUTE_ATTR_SEGMENT_LIST
    list of segments: sai_sr_segment_list_t
        
List of TLVs to be added

    attribute enum: SAI_SEGMENTROUTE_ATTR_TLV
    list of TLVs: sai_sr_tlv_list_t

#### APIs

To start with, the basic create/remove entry and set/get attributes APIs are included
 
    create_segmentroute
    remove_segmentroute
    set_segmentroute_attribute
    get_segmentroute_attribute

## Examples ##
### Example 1 - SR Origination
The following example creates an ACL entry to specify a specific flow to bind to segment_id = 1 as well as creating the corresponding entry in the Segment Routing Table to add 3 Segments and an Ingress Node TLV

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
    v6sr_entry_attrs[1].id = SAI_SEGMENTROUTE_ATTR_TLV
    v6sr_entry_attrs[1].value.objlist.count = 1
    v6sr_entry_attrs[1].value.objlist.list[0].tlv_type = SAI_TLV_TYPE_INGRESS;
    CONVERT_STR_TO_IPV6(v6sr_entry_attrs[1].value.objlist.list[0].ingress_node, "2001:db8:85a3::8a2e:370:9876");
    
    saistatus = sai_v6sr_api->create_segmentroute(&segment_id, switch_id, 2, v6sr_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

## References ##
1. Most Recent IPv6 Segment Routing IETF Draft: https://tools.ietf.org/html/draft-ietf-6man-segment-routing-header-06

