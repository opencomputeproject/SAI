SAI UDF IFP
-------------------------------------------------------------------------------
 Title       | SAI UDF based ACL
-------------|-----------------------------------------------------------------
 Authors     | Jai Kumar, Broadcom Inc.
 Status      | In review
 Type        | Standards track
 Created     | 05/04/2020: Initial Draft
 SAI-Version | 1.?
-------------------------------------------------------------------------------

This spec talks about using UDF extracted fields as qualifiers in ACL.

# SAI UDF Overview
---

#### UDF Group
SAI UDF group object is used to extract a single field from a packet. Since incoming packet can be of different type, e.g., ipv4, ipv4-in-ipv4, SAI UDF group contains a list of UDFs, each of which is used to extract a field for a certain type of packet. When a packet goes through a UDF group object, one UDF is selected based on its UDF match object.
A UDF group that will be used in ACL should be set SAI_UDF_GROUP_GENERIC as the group type.

Length of the field extracted is controlled by SAI_UDF_GROUP_ATTR_LENGTH UDF group attribute.

#### UDF Group, UDF and UDF Match Objects
Following the relationship between all UDF objects.
> UDF_Group1--->UDF1--->UDF_Match1
> UDF_Group1--->UDF2--->UDF_Match1
> UDF_GRoup1--->UDF3--->UDF_Match2
   
UDF group may contain multiple UDF objects. Multiple UDF objects may point to same or different UDF Match objects. As mentioned earlier each UDF group object is used to extract a single field from a packet.

#### Number of UDF Groups
Maximum number of UDF groups supported for a given device are exposed in capability infra.

#### UDF Group in ACL
User should be able to configure a mask in the ACL entry for a given UDF extracted field by the UDF group.

> Following attributes already exists in ACL headers but provide only UDF group OID information.

```
    /**
     * @brief Attribute Id for sai_acl_table
     *
     * @flags Contains flags
     */
    typedef enum _sai_acl_table_attr_t
    {
        ...
        /**
         * @brief User Defined Field Groups
         *
         * @type bool
         * @flags CREATE_ONLY
         * @default false
         * @range SAI_ACL_USER_DEFINED_FIELD_ATTR_ID_RANGE
         */
        SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MIN,

        /**
         * @brief User Defined Field Groups end
         *
         * @type bool
         * @flags CREATE_ONLY
         * @default false
         */
        SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MAX = SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MIN + SAI_ACL_USER_DEFINED_FIELD_ATTR_ID_RANGE,
        ...
    } sai_acl_table_attr_t;        

    /**
     * @brief Attribute Id for sai_acl_entry
     *
     * @flags Contains flags
     */
    typedef enum _sai_acl_entry_attr_t
    {
        ...
        /**
         * @brief User Defined Field data for the UDF Groups in ACL Table
         *
         * @type sai_acl_field_data_t sai_object_id_t
         * @flags CREATE_AND_SET
         * @objects SAI_OBJECT_TYPE_UDF_GROUP
         * @default disabled
         * @range SAI_ACL_USER_DEFINED_FIELD_ATTR_ID_RANGE
         */
        SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MIN,
    
        /**
         * @brief User Defined Field data max
         *
         * @type sai_acl_field_data_t sai_object_id_t
         * @flags CREATE_AND_SET
         * @objects SAI_OBJECT_TYPE_UDF_GROUP
         * @default disabled
         */
        SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MAX = SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MIN + SAI_ACL_USER_DEFINED_FIELD_ATTR_ID_RANGE,
        ...
        } sai_acl_entry_attr_t;
```
#### saiudf.h Updates
UDF group attribute is updated with a read only index. This index is allocated by the SAI adapter and is used by NOS to set the corresponding (SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MIN + index) in ACL table as true. 
This will help derive the UDF group when a ACL table update is recieved by the SAI adapater. This will also help in determining the UDF group length and correct ACL table width can be set in HW.

Following ranges should be in sync

> #define SAI_UDF_GROUP_ATTR_ID_RANGE 0xFF
> #define SAI_ACL_USER_DEFINED_FIELD_ATTR_ID_RANGE 0xFF

```sh
    /**
     * @brief UDF group index
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     * @range SAI_UDF_GROUP_ATTR_ID_RANGE
     */
    SAI_UDF_GROUP_ATTR_INDEX,
```

#### saiacl.h Updates

> SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MIN and MAX is change to u8_list_t data type. Given that UDF qualifier in ACL is incomplete and not used currently. This change will not impact backward compatibility. 

```sh
    
    /**
     * @brief Attribute Id for sai_acl_entry
     *
     * @flags Contains flags
     */
    typedef enum _sai_acl_entry_attr_t
    {
        ...
        /**
         * @brief User Defined Field object for the UDF Groups in ACL Table
         *
         * @type sai_acl_field_data_t sai_u8_list_t
         * @flags CREATE_AND_SET
         * @default disabled
         * @range SAI_ACL_USER_DEFINED_FIELD_ATTR_ID_RANGE
         */
        SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MIN,
    
        /**
         * @brief User Defined Field data max
         *
         * @type sai_acl_field_data_t sai_u8_list_t
         * @flags CREATE_AND_SET
         * @default disabled
         */
        SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MAX = SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MIN + SAI_ACL_USER_DEFINED_FIELD_ATTR_ID_RANGE,
        ...
        } sai_acl_entry_attr_t;

```


# WorkFlow Example
---
#### Example 1
Following example is from UDF spec and shows how to define UDF extraction fields for all IPv4 packets. It first defines one UDF match object to match GRE packet with specific GRE protocol type. It then creates two UDF group objects, and creates one UDF object for each UDF group. Finally, it uses these two UDF groups as the ACL fields for all IPv4 packets.

>UDF_Group1-->UDF1-->UDF_Match1: Extracted field: GRE Packet Inner Source IP
>UDF_Group2-->UDF2-->UDF_Match1: Extracted field: GRE Packet Inner Dest IP

```
    // Create UDF_Match 1 to match the GRE packet
    sai_object_id_t udf_match1_id;
    sai_attribute_t udf_match1_attrs[3];
    udf_match1_attrs[0].id = (sai_attr_id_t)SAI_UDF_ATTR_MATCH_L2_TYPE;
    udf_match1_attrs[0].value.u16 = 0x0800;
    udf_match1_attrs[1].id = (sai_attr_id_t)SAI_UDF_ATTR_MATCH_L3_TYPE;
    udf_match1_attrs[1].value.u8 = 0x2f;
    udf_match1_attrs[2].id = (sai_attr_id_t)SAI_UDF_MATCH_ATTR_GRE_TYPE;
    udf_match1_attrs[2].value.u16 = 0x88be;
    sai_udf_match_api->create_udf_match(&udf_match1_id, 3, udf_match1_attrs);
    
    // Create two UDF groups, UDF_Group1 and UDF_Group2
    // SAI Adapter will allocate index 0 for UDF_Group1 and index 1 for UDF_Group2
    sai_object_id_t udf_group_ids[2];
    sai_attribute_t udf_group_attr;
    udf_group_attr.id = (sai_attr_id_t)SAI_UDF_GROUP_ATTR_TYPE;
    udf_group_attr.value.s32 = SAI_UDF_GROUP_TYPE_GENERIC;
    udf_group_attr.id = (sai_attr_id_t)SAI_UDF_GROUP_ATTR_LENGTH;
    udf2_attrs[4].value.u16 = 2;
    sai_udf_group_api->create_udf_group(&udf_group_ids[0], 1, udf_group_attr);
    
    udf_group_attr.id = (sai_attr_id_t)SAI_UDF_GROUP_ATTR_TYPE;
    udf_group_attr.value.s32 = SAI_UDF_GROUP_TYPE_GENERIC;
    sai_udf_group_api->create_udf_group(&udf_group_ids[1], 1, udf_group_attr);
    
    // Create UDF1 to match the inner src IP
    sai_object_id_t udf1_id;
    sai_attribute_t udf1_attrs[5];
    udf1_attrs[0].id = (sai_attr_id_t)SAI_UDF_ATTR_MATCH_ID;
    udf1_attrs[0].value.oid = udf_match1_id;
    udf1_attrs[1].id = (sai_attr_id_t)SAI_UDF_ATTR_GROUP_ID;
    udf1_attrs[1].value.oid = udf_group_ids[0];
    udf1_attrs[2].id = (sai_attr_id_t)SAI_UDF_ATTR_BASE;
    udf1_attrs[2].value.s32 = SAI_UDF_BASE_L2;
    udf1_attrs[3].id = (sai_attr_id_t)SAI_UDF_ATTR_OFFSET;
    udf1_attrs[3].value.u16 = 56;
    sai_udf_api->create_udf(&udf1_id, 5, udf1_attrs);
    
    // Create UDF2 to match the inner dest IP
    sai_object_id_t udf2_id;
    sai_attribute_t udf2_attrs[5];
    udf2_attrs[0].id = (sai_attr_id_t)SAI_UDF_ATTR_MATCH_ID;
    udf2_attrs[0].value.oid = udf_match1_id;
    udf2_attrs[1].id = (sai_attr_id_t)SAI_UDF_ATTR_GROUP_ID;
    udf1_attrs[1].value.oid = udf_group_ids[1];
    udf2_attrs[2].id = (sai_attr_id_t)SAI_UDF_ATTR_BASE;
    udf2_attrs[2].value.s32 = SAI_UDF_BASE_L2;
    udf2_attrs[3].id = (sai_attr_id_t)SAI_UDF_ATTR_OFFSET;
    udf2_attrs[3].value.u16 = 60;
    sai_udf_api->create_udf(&udf2_id, 5, udf2_attrs);
```

Following workflow shows how to stitch UDF group and its associated extracted field data in ACL table/entry. Note that each UDF group has associated field data to be used as ACL field qualifier.

> ACL_Table1--->UDF_Group1
> ACL_Table1--->UDF_Group2
> ACL_Entry1--->UDF_Group1_OID--->Field_data1_mask1
> ACL_Entry2--->UDF_Group2_OID--->Field_data2_mask2

```sh
    sai_object_id_t acl_table_id = 0ULL;
    acl_attr_list[0].id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
    acl_attr_list[0].value.s32 = SAI_ACL_STAGE_INGRESS;
    
    acl_attr_list[1].id = SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST;
    acl_attr_list[1].value.objlist.count = 1;
    acl_attr_list[1].value.objlist.list[0] = SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF;
    
    // Set group corresponding to index 0
    // This will map to UDF_Group1
    acl_attr_list[2].id = SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MIN;
    acl_attr_list[2].value.booldata = True;
    
    // Set group corresponding to index 1
    // This will map to UDF_Group2  
    acl_attr_list[3].id = SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MIN+1;
    acl_attr_list[3].value.booldata = True;
    
    saistatus = sai_acl_api->create_acl_table(&acl_table_id2, 6, acl_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    // Create an ACL table entry mask for UDF group 1 and 2
    acl_entry_attrs[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
    acl_entry_attrs[0].value.oid = acl_table_id;
    acl_entry_attrs[1].id = SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MIN;
    acl_entry_attrs[1].value.aclfield.data.count = 4;
    acl_entry_attrs[1].value.aclfield.data.list[0] = 0x10;
    acl_entry_attrs[1].value.aclfield.data.list[1] = 0x10;
    acl_entry_attrs[1].value.aclfield.data.list[2] = 0x10;
    acl_entry_attrs[1].value.aclfield.data.list[3] = 0x00;
    acl_entry_attrs[1].value.aclfield.mask.count = 4;
    acl_entry_attrs[1].value.aclfield.mask.list[0] = 0xff;
    acl_entry_attrs[1].value.aclfield.mask.list[1] = 0xff;
    acl_entry_attrs[1].value.aclfield.mask.list[2] = 0xff;
    acl_entry_attrs[1].value.aclfield.mask.list[3] = 0x00;
    
    acl_entry_attrs[2].id = SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MIN+1;
    acl_entry_attrs[2].value.aclfield.data.count = 4;
    acl_entry_attrs[2].value.aclfield.data.list[0] = 0x20;
    acl_entry_attrs[2].value.aclfield.data.list[1] = 0x20;
    acl_entry_attrs[2].value.aclfield.data.list[2] = 0x20;
    acl_entry_attrs[2].value.aclfield.data.list[3] = 0x00;
    acl_entry_attrs[2].value.aclfield.mask.count = 4;
    acl_entry_attrs[2].value.aclfield.mask.list[0] = 0xff;
    acl_entry_attrs[2].value.aclfield.mask.list[1] = 0xff;
    acl_entry_attrs[2].value.aclfield.mask.list[2] = 0xff;
    acl_entry_attrs[2].value.aclfield.mask.list[3] = 0x00;

    saistatus = sai_acl_api->create_acl_entry(&acl_entry, 5, acl_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
```

#### Example 2
Following example creates single UDF group. Two match objects are created for IPv4 and IPv6 UDP packets. Single UDF object is created to extract UDP destination port.

> UDF_Group1-->UDF1-->UDF_Match1: Extracted field: IPv4 UDP destination port
> UDF_Group1-->UDF2-->UDF_Match2: Extracted field: IPv6 UDP destination port

```
    // Create UDF_Match 1 to match the IPv4 UDP packet
    sai_object_id_t udf_match1_id;
    sai_attribute_t udf_match1_attrs[2];
    udf_match1_attrs[0].id = (sai_attr_id_t)SAI_UDF_ATTR_MATCH_L2_TYPE;
    udf_match1_attrs[0].value.u16 = 0x0800;
    udf_match1_attrs[1].id = (sai_attr_id_t)SAI_UDF_ATTR_MATCH_L3_TYPE;
    udf_match1_attrs[1].value.u8 = 0x11;
    sai_udf_match_api->create_udf_match(&udf_match1_id, 2, udf_match1_attrs);
    
    // Create UDF_Match 2 to match the IPv6 UDP packet
    sai_object_id_t udf_match2_id;
    sai_attribute_t udf_match2_attrs[2];
    udf_match2_attrs[0].id = (sai_attr_id_t)SAI_UDF_ATTR_MATCH_L2_TYPE;
    udf_match2_attrs[0].value.u16 = 0x86dd;
    udf_match2_attrs[1].id = (sai_attr_id_t)SAI_UDF_ATTR_MATCH_L3_TYPE;
    udf_match2_attrs[1].value.u8 = 0x11;
    sai_udf_match_api->create_udf_match(&udf_match2_id, 2, udf_match2_attrs);
    
    // Create UDF_Group1
    sai_object_id_t udf_group_ids;
    sai_attribute_t udf_group_attr;
    udf_group_attr.id = (sai_attr_id_t)SAI_UDF_GROUP_ATTR_TYPE;
    udf_group_attr.value.s32 = SAI_UDF_GROUP_TYPE_GENERIC;
    udf_group_attr.id = (sai_attr_id_t)SAI_UDF_GROUP_ATTR_LENGTH;
    udf2_attrs[4].value.u16 = 2;
    sai_udf_group_api->create_udf_group(&udf_group_ids, 1, udf_group_attr);
    
    // Create UDF1 to match the IPv4 UDP dest port
    sai_object_id_t udf1_id;
    sai_attribute_t udf1_attrs[5];
    udf1_attrs[0].id = (sai_attr_id_t)SAI_UDF_ATTR_MATCH_ID;
    udf1_attrs[0].value.oid = udf_match1_id;
    udf1_attrs[1].id = (sai_attr_id_t)SAI_UDF_ATTR_GROUP_ID;
    udf1_attrs[1].value.oid = udf_group_ids[0];
    udf1_attrs[2].id = (sai_attr_id_t)SAI_UDF_ATTR_BASE;
    udf1_attrs[2].value.s32 = SAI_UDF_BASE_L4;
    udf1_attrs[3].id = (sai_attr_id_t)SAI_UDF_ATTR_OFFSET;
    udf1_attrs[3].value.u16 = 2;
    sai_udf_api->create_udf(&udf1_id, 5, udf1_attrs);
    
    // Create UDF2 to match the IPv6 UDP dest port
    sai_object_id_t udf2_id;
    sai_attribute_t udf2_attrs[5];
    udf2_attrs[0].id = (sai_attr_id_t)SAI_UDF_ATTR_MATCH_ID;
    udf2_attrs[0].value.oid = udf_match2_id;
    udf2_attrs[1].id = (sai_attr_id_t)SAI_UDF_ATTR_GROUP_ID;
    udf1_attrs[1].value.oid = udf_group_ids;
    udf2_attrs[2].id = (sai_attr_id_t)SAI_UDF_ATTR_BASE;
    udf2_attrs[2].value.s32 = SAI_UDF_BASE_L4;
    udf2_attrs[3].id = (sai_attr_id_t)SAI_UDF_ATTR_OFFSET;
    udf2_attrs[3].value.u16 = 2;
    sai_udf_api->create_udf(&udf2_id, 5, udf2_attrs);
```
Following workflow shows how to stitch UDF group and its associated extracted field data in ACL table/entry. Note that each UDF group has associated field data to be used as ACL field qualifier.

> ACL_Table1--->UDF_Group1
> ACL_Entry1--->UDF_Group1_OID--->Field_data1_mask1


```sh
    sai_object_id_t acl_table_id = 0ULL;
    acl_attr_list[0].id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
    acl_attr_list[0].value.s32 = SAI_ACL_STAGE_INGRESS;
    
    acl_attr_list[1].id = SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST;
    acl_attr_list[1].value.objlist.count = 1;
    acl_attr_list[1].value.objlist.list[0] = SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF;
    
    acl_attr_list[2].id = SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MIN;
    acl_attr_list[2].value.booldata = True;
    
    saistatus = sai_acl_api->create_acl_table(&acl_table_id2, 4, acl_attr_list);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
    
    // Create an ACL table entry mask for UDF group 1
    acl_entry_attrs[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
    acl_entry_attrs[0].value.oid = acl_table_id;
    acl_entry_attrs[1].id = SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_GROUP_MIN;
    acl_entry_attrs[1].value.oid = udf_group_ids;
    acl_entry_attrs[2].id = SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_DATA_MIN;
    acl_entry_attrs[2].value.aclfield.data.count = 2;
    acl_entry_attrs[2].value.aclfield.data.list[0] = 0x00;
    acl_entry_attrs[2].value.aclfield.data.list[1] = 0x11;
    acl_entry_attrs[2].value.aclfield.mask.count = 2;
    acl_entry_attrs[2].value.aclfield.mask.list[0] = 0xff;
    acl_entry_attrs[2].value.aclfield.mask.list[1] = 0xff;

    saistatus = sai_acl_api->create_acl_entry(&acl_entry, 3, acl_entry_attrs);
    if (saistatus != SAI_STATUS_SUCCESS) {
        return saistatus;
    }
```

