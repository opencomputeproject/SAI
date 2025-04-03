# [SAI] Hashing Enhancements for Efficient RoCE Traffic Distribution
-------------------------------------------------------------------------------
 Title       | Hashing Enhancements for Efficient RoCE Traffic Distribution
-------------|-----------------------------------------------------------------
 Authors     | Satheesh Kumar Karra, Ravindranath C K (Marvell)
 Status      | In review
 Type        | Standards track
 Created     | 2025-02-27
 SAI-Version | 1.16
-------------------------------------------------------------------------------

## 1.0  Introduction


SAI (Switch Abstraction Interface) supports customization of hash field 
selection through the `saihash` object, allowing users to define hash fields 
based on network requirements. Configured `saihash` objects can be applied 
to different ECMP (Equal cost multi path) traffic flows using the following 
SAI switch attributes:  


1) SAI_SWITCH_ATTR_ECMP_HASH_IPV4 – Specifies the hash object for IPv4 packets in ECMP.
2) SAI_SWITCH_ATTR_ECMP_HASH_IPV4_IN_IPV4 – Specifies the hash object for IPv4-in-IPv4 encapsulated packets in ECMP.
3) SAI_SWITCH_ATTR_ECMP_HASH_IPV6 – Specifies the hash object for IPv6 packets in ECMP.    

These attributes allow fine-tuned ECMP hashing, optimizing traffic 
distribution based on application needs. Network administrators can create 
custom hash lists using SAI native hash fields and bind them to above switch 
attributes. SAI provided similar configurations even for LAG (Link 
Aggregation Groups), which ensures balanced traffic distribution across 
member links, reducing congestion, and enhancing overall network 
efficiency.  

In the current configuration, Remote Direct Memory Access over Converged 
Ethernet (RoCE) traffic utilizes the same ECMP and LAG hash objects as 
standard IP traffic. However, this can lead to traffic polarization, 
especially when multiple RoCE streams share the same IP endpoints.  


## 2.0 Motivation

The packet fields up to the L4 header for different RDMA streams between the 
same endpoints will be mostly identical leading to all these streams to hash 
to the same member. In order to improve the hash distribution for RDMA 
traffic, modern NPUs have native support for hashing on RDMA header fields. 

This proposal adds SAI native hash field support for the below fields in the 
RDMA Base Transport Header:

- Queue Pair (QP) Number
- RDMA opcode(Operation type)


## 3.0 SAI Enhancements

1) New Hash fields to support RoCE :
   ```c

   /**
    * @brief Attribute data for SAI native hash fields
    */
   typedef enum _sai_native_hash_field_t
   {
  
      ...
      /** Native hash field RDMA packet BTH(Base Transport Header) opcode */
      SAI_NATIVE_HASH_FIELD_RDMA_BTH_OPCODE,

      /** Native hash field RDMA packet BTH destination queue pair */
      SAI_NATIVE_HASH_FIELD_RDMA_BTH_DEST_QP,

   } sai_native_hash_field_t;
   ```
2) Switch Attributes to configure Hashing for RoCE Traffic:
   ```c
   /**
    * @brief Attribute Id in sai_set_switch_attribute() and
    * sai_get_switch_attribute() calls.
    */
   typedef enum _sai_switch_attr_t
   {
      ...
    /**
     * @brief The hash object for IPv4 RDMA packets going through ECMP
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HASH
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_ECMP_HASH_IPV4_RDMA,

    /**
     * @brief The hash object for IPv6 RDMA packets going through ECMP
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HASH
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_ECMP_HASH_IPV6_RDMA,

    /**
     * @brief The hash object for IPv4 RDMA packets going through LAG
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HASH
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_LAG_HASH_IPV4_RDMA,
     
    /**
     * @brief The hash object for IPv6 RDMA packets going through LAG
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HASH
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_LAG_HASH_IPV6_RDMA,
      ...
   } sai_switch_attr_t;
   ```


## 4.0 API Example

###  Create Hash Object

```c

hash_count = 0;
sai_attr_list[0].id = SAI_HASH_ATTR_NATIVE_HASH_FIELD_LIST;
...(Other hash fileds)
sai_attr_list[0].value.s32list.list[hash_count++] =
SAI_NATIVE_HASH_FIELD_RDMA_BTH_OPCODE;
sai_attr_list[0].value.s32list.list[hash_count++] = 
SAI_NATIVE_HASH_FIELD_RDMA_BTH_DEST_QP;
sai_attr_list[0].value.s32list.count = hash_count;
attr_count =1 

sai_create_hash_fn(
   &hash_rdma_v4_oid,
   switch_id,
   attr_count,
   sai_attr_list);
```

### Configure RDMA Hash on Switch

```c
attr_count = 0
sai_attr_list[attr_count].id = SAI_SWITCH_ATTR_ECMP_HASH_IPV4_RDMA;
sai_attr_list[attr_count].value.oid = hash_rdma_v4_oid;

sai_set_switch_attribute_fn(
   switch_id,
   sai_attr_list);
```
