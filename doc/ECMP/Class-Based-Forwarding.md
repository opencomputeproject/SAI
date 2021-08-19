# Class-based Forwarding

Title       | Class-based Forwarding
------------|----------------
Authors     | Cisco
Status      | In review
Type        | Standards track
Created     | 04/14/2021
SAI-Version | 1.8

Class-based forwarding provides a method to steer traffic among multiple paths through the network by policy rather than, or in combination with, traditional ECMP/UCMP flow-hashing.

A new type of next-hop group is introduced:

```
typedef enum _sai_next_hop_group_type_t
{
...
    /** Next hop group is class-based, with members selected by Forwarding class */
    SAI_NEXT_HOP_GROUP_TYPE_CLASS_BASED,
...
} sai_next_hop_group_type_t;
```

The behavior of SAI_NEXT_HOP_GROUP_TYPE_CLASS_BASED differs from the traditional SAI_NEXT_HOP_GROUP_TYPE_ECMP, in that each packet will have a Forwarding class that chooses next-hop group member index.

This is accomplished by directly mapping each forwarding class to the group member index, via map configured to the next-hop group object.

```
    /**
     * @brief Member selection map
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_NEXT_HOP_GROUP_ATTR_TYPE == SAI_NEXT_HOP_GROUP_TYPE_CLASS_BASED
     */
    SAI_NEXT_HOP_GROUP_ATTR_SELECTION_MAP,
```

If a packet arrives with a forwarding-class which is not present in the map, the chosen index shall be 0.

```
typedef  enum  _sai_next_hop_group_member_attr_t
{
...
    /**
     * @brief Object index in the next-hop group.
     *
     * Index specifying the strict member's order.
     * Allowed value range for is from 0 to SAI_NEXT_HOP_GROUP_ATTR_REAL_SIZE - 1.
     * Should only be used if the type of owning group is SAI_NEXT_HOP_GROUP_TYPE_FINE_GRAIN_ECMP
     * or SAI_NEXT_HOP_GROUP_TYPE_CLASS_BASED.
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_INDEX,
...
} sai_next_hop_group_member_attr_t;
```

If the map selects an index for which a member does not exist, the packet shall be treated as having a next-hop of SAI_NULL_OBJECT_ID, dropping the packet.

Members of type next-hop or next-hop groups of type ECMP shall be allowed. To allow this, next-hop group member type is extended to allow other next-hop groups:

```
   /**
     * @brief Next hop id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NEXT_HOP, <b>SAI_OBJECT_TYPE_NEXT_HOP_GROUP</b>
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID,
```

*Note: While this would also be a means to configure a hierarchical ECMP, hierarchical ECMP is outside the scope of this proposal.*

The forwarding-class for a packet may be selected via qos-map or ACL.

```
typedef  enum  _sai_qos_map_type_t
{
...
    /** QOS Map to set DSCP to Forwarding class */
    SAI_QOS_MAP_TYPE_DSCP_TO_FORWARDING_CLASS = 0x0000000d,

    /** QOS Map to set EXP to Forwarding class */
    SAI_QOS_MAP_TYPE_MPLS_EXP_TO_FORWARDING_CLASS = 0x0000000e,
...
} sai_qos_map_type_t;
```

```
typedef enum _sai_acl_entry_attr_t
...
    /**
     *  @brief  Set Forwarding Class
     *
     *  @type  sai_acl_action_data_t  sai_uint8_t
     *  @flags  CREATE_AND_SET
     *  @default  disabled
     */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_FORWARDING_CLASS,
...
}  sai_acl_entry_attr_t;
```

If the packet is not assigned a forwarding-class, then the forwarding-class of the packet shall be 0. For example, if no qos-map or ACL is configured.

*Resource monitoring considerations:*

The attribute SAI_SWITCH_ATTR_MAX_NUMBER_OF_FORWARDING_CLASSES may be used to identify the maximum forwarding-class allowed.

The SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MAP object is a resource. The sai_object_type_get_availability() API may be used to query the maximum number of permitted maps.

*Class-based forwarding group configuration example:*
```
/******************************************************
 * Create a forwarding-class -> index map.
 * In this example, map 8 forwarding-classes to 2 members.
 *   FC 0-5 -> index 0
 *   FC 6-7 -> index 1
 ******************************************************/
 const int num_forwarding_classes = 8;
 const int num_members = 2;

 sai_object_id_t nh_group_map;

 sai_map_t fc_map[num_forwarding_classes];
 for (int fc = 0; fc < num_forwarding_classes; ++fc) {
    fc_map[fc].key = fc;
    if (fc >= 6) {
       fc_map[fc].value = 1;
    } else {
       fc_map[fc].value = 0;
    }
 }

 sai_map_list_t fc_map_list;
 fc_map_list.key.count = num_forwarding_classes;
 fc_map_list.key.list = fc_map;

 attr.id = SAI_NEXT_HOP_GROUP_MAP_ATTR_TYPE;
 attr.value.u32 = SAI_NEXT_HOP_GROUP_MAP_TYPE_FORWARDING_CLASS_TO_INDEX;
 attrs.push_back(attr);

 attr.id = SAI_NEXT_HOP_GROUP_MAP_ATTR_MAP_TO_VALUE_LIST;
 attr.value.maplist = fc_map_list;
 attrs.push_back(attr);

 sai_next_hop_group_api->create_next_hop_group_map(
     &nh_group_map,
     g_switch_id,
     attrs.size(),
     attrs.data());

 /*****************************************************
  * Create a class-based forwarding group
  *****************************************************/
 attrs.clear();

 sai_object_id_t nh_group;

 attr.id = SAI_NEXT_HOP_GROUP_ATTR_TYPE;
 attr.value.u32 = SAI_NEXT_HOP_GROUP_TYPE_CLASS_BASED;
 attrs.push_back(attr);

 attr.id = SAI_NEXT_HOP_GROUP_ATTR_CONFIGURED_SIZE;
 attr.value.u32 = num_members;
 attrs.push_back(attr);

 attr.id = SAI_NEXT_HOP_GROUP_ATTR_SELECTION_MAP;
 attr.value.oid = nh_group_map;
 attrs.push_back(attr);

 sai_next_hop_group_api->create_next_hop_group(
     &nh_group,
     g_switch_id,
     attrs.size(),
     attrs.data());

 /*****************************************************
  * Create members
  *****************************************************/
 attrs.clear();

 for (index = 0; index < num_members; ++index) {
     attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID;
     attr.value.oid = nh_group;
     attrs.push_back(attr);

     attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID;
     attr.value.oid = destinations[index]; // Next-hop or ECMP group
     attrs.push_back(attr);

     attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_INDEX;
     attr.value.u32 = index;
     attrs.push_back(attr);

     sai_next_hop_group_api->create_next_hop_group_member(
         &members[member_index],
         g_switch_id,
         attrs.size(),
         attrs.data());
 }
```
