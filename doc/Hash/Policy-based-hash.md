# Policy-based Hash

Title       | Policy-based Hash
------------|----------------
Authors     | Nvidia
Status      | In review
Type        | Standards track
Created     | 04/29/2020
SAI-Version | 1.6

Sai defines two metadata values for ECMP and LAG hash.
The fields that are used for their calculation are defined at the switch level with attributes `SAI_SWITCH_ATTR_ECMP_HASH` and `SAI_SWITCH_ATTR_LAG_HASH`.
There are also hash objects that can be assigned for some basic packet types.

But if we would like to cover some use cases that are not defined in SAI, we need to be more flexible in a way that won't require changing the API.
These use cases will be covered by a concept introduced here called policy-based hashing.

```
    /** Set custom LAG hash object ID */
    SAI_ACL_ACTION_TYPE_SET_LAG_HASH_ID,

    /** Set custom ECMP hash object ID */
    SAI_ACL_ACTION_TYPE_SET_ECMP_HASH_ID,

} sai_acl_action_type_t;
```

The two action types are not conflicting between one another, but the same action type used more than once for the parralel execution is.

It utilizes the ACL infrastructure with the new actions to set custom hash object for a special ECMP or LAG treatment.

Also, the hash object itself is extended to be able to specify the portions of the fields that we are interested in, like, for example, only some bytes from IPv6 address.

```
    /**
     * @brief Hash fine-grained field list
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_FINE_GRAINED_HASH_FIELD
     * @default empty
     */
    SAI_HASH_ATTR_FINE_GRAINED_HASH_FIELD_LIST,
```

Optional ordering of the fields is available as well.
In the case where the traffic is represented by two different packet types for inbound and outbound direction, setting the hash order
is neccessary for non-associative hash algorithms like CRC, so that the resulting hash value will be the same.
For example, for the bare metal machine sending outbound IP packet to the ToR, the hash field array could look like this: {SRC_IP:0, DST_IP:1, SRC_PORT:2, DST_PORT:3, IP_TYPE:4}, and for the inbound, getting VxLAN packet from the T1 to the same ToR, the hash field array will look like this: {INNER_DST_IP:0, INNER_SRC_IP:1, INNER_DST_PORT:2, INNER_SRC_PORT:3, INNER_IP_TYPE:4}. Here the hash associativity is achieved by swapping source and destination sequence IDs. The other way to achieve this would be to assign the same shared sequence ID value for IP addresses, and another for L4 ports.

```
    /**
     * @brief Optional field ordering.
     *
     * Specifies in which order the fields are hashed,
     * and defines in which fields should be associative for CRC with the same sequence ID.
     * If not provided, it's up to SAI driver to choose.
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_FINE_GRAINED_HASH_FIELD_ATTR_SEQUENCE_ID,
```

The example below provides a complete view of the capabilities of the policy-based hashing:

```
/*****************************************************
 * Create fine grained fields for custom hash
 *****************************************************/
 
 sai_object_id_t inner_v6_hash_oid;

 attr.id = SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD;
 attr.value.s32 = SAI_NATIVE_HASH_FIELD_INNER_IP_PROTOCOL;
 attrs.push_back(attr);
 
 attr.id = SAI_FINE_GRAINED_HASH_FIELD_ATTR_SEQUENCE_ID;
 attr.value.u32 = 0;
 attrs.push_back(attr);
 
 status = sai_hash_api->create_fine_grained_hash_field(
	&ip_proto_oid,
	gSwitchId,
	(uint32_t)attrs.size(),
	attrs.data());
 attrs.clear()
  
 attr.id = SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD;
 attr.value.s32 = SAI_NATIVE_HASH_FIELD_INNER_L4_SRC_PORT;
 attrs.push_back(attr);
 
 attr.id = SAI_FINE_GRAINED_HASH_FIELD_ATTR_SEQUENCE_ID;
 attr.value.u32 = 1;
 attrs.push_back(attr);
 
 status = sai_hash_api->create_fine_grained_hash_field(
	&src_port_oid,
	gSwitchId,
	(uint32_t)attrs.size(),
	attrs.data());
 attrs.clear()
  
 attr.id = SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD;
 attr.value.s32 = SAI_NATIVE_HASH_FIELD_INNER_L4_DST_PORT;
 attrs.push_back(attr);
 
 attr.id = SAI_FINE_GRAINED_HASH_FIELD_ATTR_SEQUENCE_ID;
 attr.value.u32 = 1;
 attrs.push_back(attr);
 
 status = sai_hash_api->create_fine_grained_hash_field(
	&dst_port_oid,
	gSwitchId,
	(uint32_t)attrs.size(),
	attrs.data());
 attrs.clear()
  
 attr.id = SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD;
 attr.value.s32 = SAI_NATIVE_HASH_FIELD_INNER_SRC_IPV6;
 attrs.push_back(attr);
 
 attr.id = SAI_FINE_GRAINED_HASH_FIELD_ATTR_IPV6_MASK;
 attr.value.ipv6 = 0xffffffff;
 attrs.push_back(attr);
 
 attr.id = SAI_FINE_GRAINED_HASH_FIELD_ATTR_SEQUENCE_ID;
 attr.value.u32 = 2;
 attrs.push_back(attr);
 
 status = sai_hash_api->create_fine_grained_hash_field(
	&src_ip_oid,
	gSwitchId,
	(uint32_t)attrs.size(),
	attrs.data());
 attrs.clear()
  
 attr.id = SAI_FINE_GRAINED_HASH_FIELD_ATTR_NATIVE_HASH_FIELD;
 attr.value.s32 = SAI_NATIVE_HASH_FIELD_INNER_DST_IPV6;
 attrs.push_back(attr);
 
 attr.id = SAI_FINE_GRAINED_HASH_FIELD_ATTR_IPV6_MASK;
 attr.value.ipv6 = 0xffffffff;
 attrs.push_back(attr);
 
 attr.id = SAI_FINE_GRAINED_HASH_FIELD_ATTR_SEQUENCE_ID;
 attr.value.u32 = 2;
 attrs.push_back(attr);
 
 status = sai_hash_api->create_fine_grained_hash_field(
	&dst_ip_oid,
	gSwitchId,
	(uint32_t)attrs.size(),
	attrs.data());
 attrs.clear()

/*****************************************************
 * Create a hash object for the inner IPv6
 *****************************************************/

 attr.id = SAI_HASH_ATTR_FINE_GRAINED_FIELD_LIST;
 attr.value.objlist.count = 5;
 attr.value.objlist.list = [ ip_proto_oid, src_port_oid, dst_port_oid, src_ip_oid, dst_ip_oid ];
 attrs.push_back(attr);
 
 status = sai_hash_api->create_hash(
	&inner_hash_oid,
	gSwitchId,
	(uint32_t)attrs.size(),
	attrs.data());
 attrs.clear()
	
/*****************************************************
 * Define ACL table
 *****************************************************/
 sai_object_id_t table_oid;
 
 attr.id = SAI_ACL_TABLE_ATTR_FIELD_VNI;
 attr.value.booldata = true;
 attrs.push_back(attr);
 
 attr.id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
 attr.value.s32 = SAI_ACL_STAGE_INGRESS;
 table_attrs.push_back(attr);

 status = sai_acl_api->create_acl_table(
	&table_oid,
	gSwitchId,
	(uint32_t)attrs.size(),
	attrs.data());
 attrs.clear()
	
 /*****************************************************
 * Define ACL rule
 *****************************************************/	
 sai_object_id_t rule_oid;
 
 attr.id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
 attr.value.oid = table_oid;
 attrs.push_back(attr);
 
 attr.id = SAI_ACL_ENTRY_ATTR_FIELD_VNI;
 attr.value.u32 = 3030;
 attr.value.mask = 0xffffffff;
 rule_attrs.push_back(attr);
 
 attr.id = SAI_ACL_ENTRY_ATTR_ACTION_SET_ECMP_HASH_ID;
 attr.value.oid = inner_hash_oid;
 
 status = sai_acl_api->create_acl_rule(
	&rule_oid,
	gSwitchId,
	(uint32_t)attrs.size(),
	attrs.data());
 attrs.clear()

```
