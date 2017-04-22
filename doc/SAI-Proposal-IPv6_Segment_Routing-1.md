SAI IPv6 Segment Routing Proposal for SAI 1.2.0
-------------------------------------------------------------------------------
 Title       | SAI IPv6 Segment Routing
-------------|-----------------------------------------------------------------
 Authors     | Cavium Inc.
 Status      | In review
 Type        | Standards track
 Created     | 04/14/2017
 Updated     | 04/22/2017
 SAI-Version | 1.2.0

-------------------------------------------------------------------------------

## Overview ##

Segment Routing (SR) allows a node to steer a packet through a controlled set of instructions, called segments, by prepending an SR header to the packet.  A segment can represent any instruction, topological or service-based.  SR allows to enforce a flow through any path (topological, or application/service based) while maintaining per-flow state only at the ingress node to the SR domain.

This document covers IPv6-based Segment Routing as per IETF Drafts:
1. https://tools.ietf.org/html/draft-ietf-6man-segment-routing-header-06.  
2. https://tools.ietf.org/html/draft-filsfils-spring-srv6-network-programming-00
3. https://tools.ietf.org/html/draft-filsfils-spring-segment-routing-policy-00

This specification proposes the following points:
1. Introduce the concept of Segment Routing using IPv6.
2. Introduce behavioral model modifications to support SR source, transit, and endpoint.
3. Introduce SAI APIs to define SR properties including segment and TLV definitions.

## Behavioral Model

In order to add IPv6 Segment Routing, it requires three mechanisms:
1. Way to specify which flows will be marked for SR source or transit
2. Way to add SR header (source / transit) or modify / remove SR header (endpoint) before normal L2/L3 processing

For the first mechanism, ACL is used to match on specific flows to determine the policy "color".  The color and packet DIP is used to determine the Policy's Binding Segment ID (BSID).  The BSID is used similarly to a next-hop group table to group Segment ID Lists (SID Lists) and ECMP hash between the possible paths

For the second mechanism, the Segment Route Tables would be used to manipulate the SR header information before normal lookup routine.  This would require two mechanisms:
1. Way to specific what segment and/or TLV information would be added in the source / transit case.  This will be programmed via SAI APIs into a match/action within the SID List Table.
2. Way to identify if ingress packet has a SR header existing and SR DIP match for the endpoint cases. The segment_exists metadata is set if SR header exists on ingress packet via a previous element such as the parser.

Within the Segment Route SID List Table, if a match on the color and DIP is found in the previous Policy / Endpoint Table and BSID table is properly linked, the resulting action would add the programmed SR header to the packet so the subsequent route lookup will be on the SR header segment DIP instead of the native DIP.  In the Endpoint scenario, If a SID matches in the Policy / Endpoint table, the user can define various endpoint functionalities.

Only two new metadata values need to be added, segment_exists and sr_color.

Figure 1 shows the additional logic between the Egress ACL and Ingress Router Flow (similar to tunnel) in the behavioral pipeline to support this.

![SAI v6SR bm](figures/sai_v6SR_bm.png "Figure 1: Behavioral Model Addition. ")
__Figure 1: Behavioral Model Addition.__

## API Modification

### ACL Table Modification
Adding an additional ACL action / value of SAI_ACL_ACTION_TYPE_SET_POLICY_COLOR to define the policy color for packets to be matched upon for SR source or transit in the Policy / Endpoint Table lookup

### Policy / Endpoint Table APIs
#### Match Parameter

Match on VRF, IPv6 DIP, and color from ACL lookup
    sai_sr_pe_entry_t

#### Action Parameters

Set BSID for source or transit

    SAI_SR_PE_ENTRY_ATTR_BSID

Endpoint Actions to be taken

    SAI_SR_PE_ENTRY_ATTR_ACTION_TYPE
    SAI_SR_PE_ENTRY_ATTR_POP_TYPE

> Note: Not all endpoint actions included

### BSID Table APIs
#### Match Parameter

BSID Object

#### Action Parameters

To bind BSID object to corresponding SID List object, it is done in the SID List APIs.

    SAI_SR_SIDLIST_ATTR_SR_BSID

To Read the number of SID Lists bound to the BSID and the list of SID list objects

    SAI_SR_BSID_ATTR_SIDLIST_COUNT
    SAI_SR_BSID_ATTR_SR_SIDLIST_MEMBER_LIST

### SID List Table APIs
#### Vendor Support Advertisement

Included is also a way for vendors to advertise devcie support include the number of segments and TLV types that can be originated

    SAI_SR_SIDLIST_ATTR_NUM_SEGMENTS_SUPPORTED
    SAI_SR_SIDLIST_ATTR_TLV_TYPE_SUPPORTED

> Note: NSH Carrier and Padding TLVs were not included in this first draft

#### Match Parameter

SID List ID object and the configured BSID object
    
    SAI_SR_SIDLIST_ATTR_SR_BSID

#### Action Parameters

Transit or Source Action to be taken with policy

    SAI_SR_SIDLIST_ATTR_TYPE

List of DIP segments or TLVs to be added

    SAI_SR_SIDLIST_ATTR_SEGMENT_LIST
    SAI_SR_SIDLIST_ATTR_TLV

SID List weights for W-ECMP

    SAI_SR_SIDLIST_ATTR_WEIGHT

### Segment Route Counter Support

> Note: No counters included in first draft

### APIs

To start with, the basic create/remove entry and set/get attributes APIs are included

    create_sr_bsid;
    remove_sr_bsid;
    set_sr_bsid_attribute;
    get_sr_bsid_attribute;

    create_sr_sidlist;
    remove_sr_sidlist;
    set_sr_sidlist_attribute;
    get_sr_sidlist_attribute;
    create_sr_sidlists;
    remove_sr_sidlists;

    create_sr_pe_entry;
    remove_sr_pe_entry;
    set_sr_pe_entry_attribute;
    get_sr_pe_entry_attribute;

## Examples ##
### Example 1 - SR Source / Transit
The following example 
1. Creates an ACL entry to specify a specific flow to bind to policy_color = 1 
2. Creates a BSID ID
3. Create a Policy Match using vrf_id, DIP, and policy_color to match to BSID
4. Writes entry in the Segment Routing SID List Table to add 3 Segments and an Ingress Node TLV and bind to a BSID

    acl_entry_attrs[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
    acl_entry_attrs[0].value.oid = acl_table_id2;
    acl_entry_attrs[1].id = SAI_ACL_ENTRY_ATTR_PRIORITY;
    acl_entry_attrs[1].value.u32 = 1;
    acl_entry_attrs[2].id = SAI_ACL_ENTRY_ATTR_FIELD_SRC_IPV6;
    CONVERT_STR_TO_IPV6(acl_entry_attrs[2].value.aclfield.data.ip6, "2001:db8:85a3::8a2e:370:7334");
    acl_entry_attrs[3].id = SAI_ACL_ACTION_TYPE_SET_POLICY_COLOR; 
    acl_entry_attrs[3].value.aclfield.data.u32 = 1;
    saistatus = sai_acl_api->create_acl_entry(&acl_entry, 4, acl_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

    switch_id = 0;
    saistatus = sai_v6sr_api->create_sr_bsid(&bsid_id, switch_id, 0, v6sr_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    pe_entry.switch_id = 0;
    pe_entry.vr_id = 3;
    CONVERT_STR_TO_IPV6(pe_entry.destination, "2001:db8:85a3::8a2e:370:0000");
    pe_entry.policy_color = 1;
    v6sr_entry_attrs[0].id = SAI_SR_PE_ENTRY_ATTR_BSID;
    v6sr_entry_attrs[0].value = bsid_id;
    saistatus = sai_v6sr_api->create_sr_pe_entry(&pe_entry, 1, v6sr_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

    switch_id = 0;
    v6sr_entry_attrs[0].id = SAI_SR_SIDLIST_ATTR_SEGMENT_LIST
    v6sr_entry_attrs[0].value.objlist.count = 3;
    CONVERT_STR_TO_IPV6(v6sr_entry_attrs[0].value.objlist.list[0], "2001:db8:85a3::8a2e:370:1234");
    CONVERT_STR_TO_IPV6(v6sr_entry_attrs[0].value.objlist.list[1], "2001:db8:85a3::8a2e:370:2345");
    CONVERT_STR_TO_IPV6(v6sr_entry_attrs[0].value.objlist.list[2], "2001:db8:85a3::8a2e:370:3456");
    v6sr_entry_attrs[1].id = SAI_SR_SIDLIST_ATTR_TLV;
    v6sr_entry_attrs[1].value.objlist.count = 1;
    v6sr_entry_attrs[1].value.objlist.list[0].tlv_type = SAI_TLV_TYPE_INGRESS;
    CONVERT_STR_TO_IPV6(v6sr_entry_attrs[1].value.objlist.list[0].ingress_node, "2001:db8:85a3::8a2e:370:9876");
    v6sr_entry_attrs[2].id = SAI_SR_SIDLIST_ATTR_TYPE;
    v6sr_entry_attrs[2].value = SAI_SR_SIDLIST_TYPE_ENCAPS_ORIGINATION; 
    v6sr_entry_attrs[3].id = SAI_SR_SIDLIST_ATTR_SR_BSID;
    v6sr_entry_attrs[3].value = bsid_id;
    saistatus = sai_v6sr_api->create_sr_sidlist(&sidlist_id, switch_id, 4, v6sr_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

### Example 2 - SR Endpoint 
The following example creates an Endpoint entry to match on incoming DIP and do a basic endpoint behavior with PSP

    endpoint_entry.switch_id = 0;
    endpoint_entry.vr_id = 0;
    endpoint_entry.policy_color = 0;
    CONVERT_STR_TO_IPV6(endpoint_entry.destination, "2001:db8:85a3::8a2e:370:4567");
    v6sr_entry_attrs[0].id = SAI_SR_PE_ENTRY_ATTR_ACTION;
    v6sr_entry_attrs[0].value = SAI_SR_PE_ACTION_TYPE_E;
    v6sr_entry_attrs[1].id = SAI_SR_PE_ENTRY_ATTR_POP;
    v6sr_entry_attrs[1].value = SAI_SR_PE_POP_TYPE_PSP;

    saistatus = sai_v6sr_api->sai_create_sr_pe_entry(&endpoint_entry, 2, v6sr_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }

## References ##
1. https://tools.ietf.org/html/draft-ietf-6man-segment-routing-header-06
2. https://tools.ietf.org/html/draft-filsfils-spring-srv6-network-programming-00
3. https://tools.ietf.org/html/draft-filsfils-spring-segment-routing-policy-00
