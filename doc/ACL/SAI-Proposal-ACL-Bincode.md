#  ACL META Field Matching
-------------------------------------------------------------------------------
 Title       | ACL META Field Matching
-------------|-----------------------------------------------------------------
 Authors     | Nader Shinouda, Cisco
 Status      | In review
 Type        | Standards track
 Created     | 2024-07-01 - Initial Draft
 SAI-Version | 1.14
-------------------------------------------------------------------------------


## 1.0  Introduction ##

This spec enhances the existing ACL spec to add support for meta field matching. Meta data is part of prefix compression entry, where an IP prefix is mapped to a meta value.

New table attributes allow setting a source and destination prefix compression tables on the creation of an ACL table. New field entry attributes allow for matching on a specific meta-data from either the source or destionation prefix tables.

## 2.0 Specification ##

New table attributes allow for setting both the source and destination prefix compression tables that will be used in field matching
```c
    /**
     * @brief SRC META data
     *
     * This key is dedicated to matching on a SRC META data
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_SRC_PREFIX_META

    /**
     * @brief DST META data
     *
     * This key is dedicated to matching on a DST META data
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_DST_PREFIX_META

    /**
     * @brief SRC prefix Table Object ID
     *
     * An object pointer to a prefix table used for
     * source prefix lookups
     *
     * @type sai_object_id_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PREFIX_COMPRESSION_TABLE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ACL_TABLE_ATTR_SRC_PREFIX_COMPRESSION_TABLE,

    /**
     * @brief DST prefix Table Object ID
     *
     * An object pointer to a prefix table used for
     * destination prefix lookups
     *
     * @type sai_object_id_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PREFIX_COMPRESSION_TABLE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ACL_TABLE_ATTR_DST_PREFIX_COMPRESSION_TABLE,
```

New field entry attributes allow for lookups based on a meta value.

```c
   /**
     * @brief SRC META data
     *
     * @type sai_acl_field_data_t sai_uint32_t
     * @flags CREATE_AND_SET
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_FIELD_SRC_PREFIX_META,

    /**
     * @brief DST META data
     *
     * @type sai_acl_field_data_t sai_uint32_t
     * @flags CREATE_AND_SET
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_FIELD_DST_PREFIX_META,
```

## 3.0 Examples ##

```c
// Example: Create Prefix Compression Table
sai_attr_table_list[0].id = SAI_PREFIX_COMPRESSION_TABLE_ATTR_DEFAULT_ENTRY_META;
sai_attr_table_list[0].value.u32 = 9000;
attr_table_count = 1;

// Create SRC Prefix Table
sai_create_prefix_compression_table_fn(
    &src_prefix_compression_table_id,
    switch_id,
    attr_table_count,
    sai_attr_table_list);

// Example: Create Prefix Compression Entries
// IPV4 First Entry source prefix table
sai_prefix_compression_entry_t entry_v4_src_1;
entry_v4_src_1.switch_id = switch_id;
entry_v4_src_1.prefix_table_id = src_prefix_compression_table_id;
entry_v4_src_1.prefix.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
entry_v4_src_1.prefix.addr.ipv4 = "1.1.1.1";
entry_v4_src_1.prefix_mask.ipv4 = "255.255.255.0";
sai_attr_list_src[1].id = SAI_PREFIX_COMPRESSION_ENTRY_ATTR_META;
sai_attr_list_src[1].value.u32 = 500;

entry_list_src = [entry_v4_src_1];
entry_list_size = 1;
attr_count_src = 1;
sai_status_t bulk_status;

// Adding using bulk add
sai_bulk_create_prefix_compression_entry_fn(
    entry_list_size,
    entry_list_src,
    attr_count_src,
    sai_attr_list_src,
    SAI_BULK_OP_ERROR_MODE_STOP_ON_ERROR,
    bulk_status);

// Create ACL table
sai_table_attr_list[0].id = SAI_ACL_TABLE_ATTR_SRC_PREFIX_COMPRESSION_TABLE;
sai_table_attr_list[0].value.oid = src_prefix_compression_table_id;
sai_table_attr_count = 1;

sai_create_acl_table_fn(
    &acl_table_id,
    switch_id,
    sai_table_attr_count,
    sai_table_attr_list);

sai_entry_attr_list[0].id = SAI_ACL_ENTRY_ATTR_FIELD_SRC_PREFIX_META;
sai_attr_list[1].value.aclfield.enable = true;
sai_attr_list[1].value.aclfield.mask = 0xFFFFFF;
sai_attr_list[1].value.aclfield.data = 500;
sai_entry_attr_count = 1;

// Create an Entry to match on META data of value 1
sai_create_acl_entry_fn(
    &acl_entry_id,
    switch_id,
    sai_entry_attr_count,
    sai_entry_attr_list);
```