# Ordered and Fine-Grained ECMP

Title       | Ordered and Fine-Grained ECMP
------------|----------------
Authors     | Mellanox
Status      | In review
Type        | Standards track
Created     | 04/08/2020
SAI-Version | 1.5

SAI supports a simple ECMP definition `SAI_NEXT_HOP_GROUP_TYPE_ECMP`, which can grow or shrink in size, and the members do not assume any order.
SAI implementation is responsible for managing the oreder of the members and the overall group size.

A new ECMP type is proposed that allows a user to control an order of members:

```
/**
 * @brief Next hop group type
 */
typedef enum _sai_next_hop_group_type_t
{
    ...
    /** Next hop group is ECMP, with a dynamic number of members, sorted by priority */
    SAI_NEXT_HOP_GROUP_TYPE_DYNAMIC_ORDERED_ECMP,
    ...
} sai_next_hop_group_type_t;
```

The order is not strict and controlled by specifying a priority:

```
    /**
     * @brief Object priority for enforcing the members' order.
     *
     * Index specifying the member's order. The index is not strict exhibiting the behavior of
     * the priority. Members don't have to have the sequential priorities and it's driver's job
     * to translate the priorities to the real indices in the group.
     * Should only be used if the type of owning group is SAI_NEXT_HOP_GROUP_TYPE_DYNAMIC_ORDERED_ECMP.
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_PRIORITY,
```

The rest of the behavior is not differrent from the traditional `SAI_NEXT_HOP_GROUP_TYPE_ECMP`, which now is also aliased to `SAI_NEXT_HOP_GROUP_TYPE_DYNAMIC_UNORDERED_ECMP`.

There could be a more specific use case, where not only order of the ECMP members matters, but also the traffic distribution needs to be controled precisely.
Consider an example of two ToR switches connected to the set of VMs load balancing the traffic to them.
Usually, the desired behavior is to distribute traffic equeally to all of them.

![](https://github.com/marian-pritsak/SAI/blob/nhgm-order/doc/ECMP/ECMP.jpg)

But in the case one of the nodes fails, the flow redistribution needs to be handled in the following way - all the flows that belong to healthy nodes, experience no change in distribution,
and the flows to the failed node should be equally redistributed to the healthy nodes.

![](https://github.com/marian-pritsak/SAI/blob/nhgm-order/doc/ECMP/ECMP2.jpg)

The mechanism that allows for such control is introduced with the new fine-grained ECMP type:

```
/**
 * @brief Next hop group type
 */
typedef enum _sai_next_hop_group_type_t
{
    ...

    /** Next hop group is ECMP, with a fixed, usually large, number of members, sorted by index */
    SAI_NEXT_HOP_GROUP_TYPE_FINE_GRAIN_ECMP,
    ...
} sai_next_hop_group_type_t;
```

This group type represents a HW table with a fixed (usually large) size,

```
    /**
     * @brief Configured group size
     *
     * Maximum desired number of members. The real size should
     * be queried from SAI_NEXT_HOP_GROUP_ATTR_REAL_SIZE
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     * @validonly SAI_NEXT_HOP_GROUP_ATTR_TYPE == SAI_NEXT_HOP_GROUP_TYPE_FINE_GRAIN_ECMP
     * @isresourcetype true
     */
    SAI_NEXT_HOP_GROUP_ATTR_CONFIGURED_SIZE,

    /**
     * @brief Real group size
     *
     * Can be different (greater or equal) from the configured
     * size. Application must use this value to know the exact size
     * of the group.
     * Should be used with SAI_NEXT_HOP_GROUP_TYPE_FINE_GRAIN_ECMP.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_NEXT_HOP_GROUP_ATTR_REAL_SIZE,
```

where all the next hops are assigned multiple positions in the table by the application.

```
    /**
     * @brief Object index in the fine grain ECMP table.
     *
     * Index specifying the strict member's order.
     * Should only be used if the type of owning group is SAI_NEXT_HOP_GROUP_TYPE_FINE_GRAIN_ECMP.
     * Allowed value range for is from 0 to SAI_NEXT_HOP_GROUP_ATTR_REAL_SIZE - 1.
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_INDEX,
```

By remapping the entries from the failed next hop to the rest of the healthy ones, the flows will be redistributed with the least traffic disruption.

```
/*****************************************************
 * Create an ordered ECMP group for 1.1.1.1:1.1.1.6
 *****************************************************/
 
 sai_object_id_t ecmp_group;
 
 attr.id = SAI_NEXT_HOP_GROUP_ATTR_TYPE;
 attr.value.oid = SAI_NEXT_HOP_GROUP_TYPE_FINE_GRAIN_ECMP;
 
 attr.id = SAI_NEXT_HOP_GROUP_ATTR_CONFIGURED_SIZE;
 attr.value.oid = 100;
 
 sai_next_hop_group_api->create_next_hop_group_member(
	&ecmp_group,
	g_switch_id,
	attrs.size(),
	attrs.data());
	
 attr.id = SAI_NEXT_HOP_GROUP_ATTR_REAL_SIZE;
	
 sai_next_hop_group_api->get_next_hop_group_attribute(ecmp_group,&attr);
 
 uint32_t real_size = attr.value.u32;

/*****************************************************
 * Create ECMP members for 1.1.1.1:1.1.1.6
 *****************************************************/
 
sai_object_id_t members[real_size];

for (i = 0; i < (real_size - real_size % 6); i ++) {
  attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID
  attr.value.oid = nh_group;
  attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID
  attr.value.oid = 1.1.1.1 + i % 6; // NH object with this IP actually passed
  attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_INDEX;
  attr.value.oid = i;

  sai_next_hop_group_api->create_next_hop_group_member(
	&members[i],
	g_switch_id,
	attrs.size(),
	attrs.data());
}

// fill the remaining entries

for (i = 0; i < real_size % 6; i ++) {
  attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID
  attr.value.oid = nh_group;
  attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID
  attr.value.oid = 1.1.1.1 + i % 6; // NH object with this IP actually passed
  attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_INDEX;
  attr.value.oid = i;

  sai_next_hop_group_api->create_next_hop_group_member(
	&members[i],
	g_switch_id,
	attrs.size(),
	attrs.data());
}

/*****************************************************
 * Remap the 1.1.1.2 next hop to the rest
 *****************************************************/
 
 for (i = 0; i < real_size; i ++) {
	sai_object_id_t fallback_nhs[5] = { 1.1.1.1, 1.1.1.3, 1.1.1.4, 1.1.1.5, 1.1.1.6}; // NH object with this IP actually passed
	int j = 0;
	
	attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID;  // NH object with this IP actually passed
	sai_next_hop_group_api->get_next_hop_group_member_attribute(members[i],&attr);
	
	if (attr.value.oid == 1.1.1.2) {
		attr.value.oid = fallback_nhs[j % 5];
		sai_next_hop_group_api->set_next_hop_group_member_attribute(members[i],&attr);
		j++;
	}
 }
```
