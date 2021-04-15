# Class-based Forwarding

Title       | Class-based Forwarding
------------|----------------
Authors     | Cisco
Status      | In review
Type        | Standards track
Created     | 04/14/2021
SAI-Version | 1.8

Class-based forwarding provides a method to steer traffic among multiple paths through the network by policy rather than, or in combination with, traditional ECMP/UCMP flow-hashing.

A new type of next-hop group is proposed:

```
typedef enum _sai_next_hop_group_type_t
{
...
    /** Next hop group is class-based, with members selected by Forwarding class */
    SAI_NEXT_HOP_GROUP_TYPE_CLASS_BASED,
...
} sai_next_hop_group_type_t;
```

The behavior of SAI_NEXT_HOP_GROUP_TYPE_CLASS_BASED differs from the traditional SAI_NEXT_HOP_GROUP_TYPE_ECMP, in that each packet will have a Forwarding class that directly chooses next-hop group member index.

This is accomplished by mapping each forwarding-class to the group member index.

```
    /**
     * @brief Forwarding-class to index map
     *
     * @type sai_map_list_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_NEXT_HOP_GROUP_ATTR_TYPE == SAI_NEXT_HOP_GROUP_TYPE_CLASS_BASED
     */
    SAI_NEXT_HOP_GROUP_ATTR_FORWARDING_CLASS_TO_INDEX_MAP,
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

<pre><code>    /**
     * @brief Next hop id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NEXT_HOP, <b>SAI_OBJECT_TYPE_NEXT_HOP_GROUP</b>
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID,
</code></pre>

*Note: While this would also be a means to configure hierarchical ECMP, hierarchical ECMP is outside the scope of this proposal.*

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
     *  @brief  Set  Forwarding  Class
     *
     *  @type  sai_acl_action_data_t  sai_uint8_t
     *  @flags  CREATE_AND_SET
     *  @default  disabled
     */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_FORWARDING_CLASS,
...
}  sai_acl_entry_attr_t;
```

In the event a packet is not assigned a forwarding-class, for example, a qos-map or ACL is not configured, then the forwarding-class of the packet shall be 0.

*Class-based forwarding group configuration example:*
```
/******************************************************
 * Create a forwarding-class -> index map.
 * In this example, a simple 1:1 mapping is configured.
 ******************************************************/
 const int num_forwarding_classes = 8;

 sai_map_t fc_map[num_forwarding_classes];
 for (int i = 0; i < num_forwarding_classes; ++i) {
     fc_map[i].key = i;
     fc_map[i].value = i;
 }

 sai_map_list_t fc_map_list;
 fc_map_list.key.count = num_forwarding_classes;
 fc_map_list.key.list = fc_map;

 /*****************************************************
  * Create a class-based forwarding group
  *****************************************************/
 sai_object_id_t nh_group;

 attr.id = SAI_NEXT_HOP_GROUP_ATTR_TYPE;
 attr.value.oid = SAI_NEXT_HOP_GROUP_TYPE_CLASS_BASED;
 attrs.push_back(attr);

 attr.id = SAI_NEXT_HOP_GROUP_ATTR_CONFIGURED_SIZE;
 attr.value.oid = num_forwarding_classes;
 attrs.push_back(attr);

 attr.id = SAI_NEXT_HOP_GROUP_ATTR_FORWARDING_CLASS_TO_INDEX_MAP;
 attr.value.maplist = fc_map_list;
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

 for (i = 0; i < size; ++i) {
     attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID;
     attr.value.oid = nh_group;
     attrs.push_back(attr);

     attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID;
     attr.value.oid = destinations[i]; // Next-hop or ECMP object allowed
     attrs.push_back(attr);

     attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_INDEX;
     attr.value.oid = i;
     attrs.push_back(attr);

     sai_next_hop_group_api->create_next_hop_group_member(
         &members[i],
         g_switch_id,
         attrs.size(),
         attrs.data());
 }
 ```
