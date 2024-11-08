#  Prefix Compression #
-------------------------------------------------------------------------------
 Title       | Prefix Compression
-------------|-----------------------------------------------------------------
 Authors     | Nader Shinouda, Cisco
 Status      | In review
 Type        | Standards track
 Created     | 2024-07-1 - Initial Draft
 SAI-Version | 1.16
-------------------------------------------------------------------------------


## 1.0  Introduction ##

This specification introduces Prefix Compression, which enables the mapping of an IP prefix/mask (longest prefix) to a meta-data value. These prefix/meta-data mappings can be grouped to form a prefix compression table. These tables can include both IPv4 and IPv6 entries.

Prefix compression tables can be utilized in features such as ACL to match specific meta fields, thereby enhancing the functionality of ACLs or any other feature that can benefit from these groupings.

## 1.1.0 Function Requirement of Prefix Compression
- Enable the creation of a prefix compresison table as a SAI object
- Enable adding IPV4/IPV6 Prefix mapping to a META value

## 2.0 Specification ##

Specficicaton covers prefix compression and ACL use of Prefix Compression

## 2.1 Prefix Compression

A prefix compression table is represented by an object of type SAI_PREFIX_COMPRESSION_TABLE. Creating this object allocates an empty table to which specific entries can be added later. The table creation process requires associating a stage with the table and specifying the type of table being created: source addresses, destination addresses, or both.

### sai.h ###
New type SAI_API_PREFIX_COMPRESSION is added into sai_api_t

### saiobject.h ###
New entry sai_prefix_compression_entry_t prefix_compression_entry.

### saitypes.h ###
Two new types: SAI_OBJECT_TYPE_PREFIX_COMPRESSION_TABLE and SAI_OBJECT_TYPE_PREFIX_COMPRESSION_ENTRY

### New Header saiprefixcompression.h ###

#### sai_prefix_compression_table_attr_t ####
This defines the prefix compression attributes table

Tables are composed of prefix compression entries. These entries map a specific IP prefix to a meta data value. Both IPV4 and IPV6 entries can be added to the same table. During table creation a stage must be set to the table (Ingress/Egress)

```c
typedef enum _sai_prefix_compression_table_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PREFIX_COMPRESSION_TABLE_ATTR_START,

    /**
     * @brief Label attribute used to unique identify Table.
     *
     * @type char
     * @flags CREATE_AND_SET
     * @default ""
     */
    SAI_PREFIX_COMPRESSION_TABLE_ATTR_LABEL = SAI_PREFIX_COMPRESSION_TABLE_ATTR_START,

    /**
     * @brief Prefix Compression table stage
     *
     * @type sai_prefix_compression_stage_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_PREFIX_COMPRESSION_TABLE_ATTR_PREFIX_COMPRESSION_STAGE,

    /**
     * @brief Prefix Compression table type
     *
     * @type sai_prefix_compression_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isresourcetype true
     */
    SAI_PREFIX_COMPRESSION_TABLE_ATTR_PREFIX_COMPRESSION_TYPE,

```

#### sai_prefix_compression_entry_attr_t ####

Attributes structure for prefix compression entries

```c
    /**
     * @brief Attribute Id for SAI prefix compression object
     */
    typedef enum _sai_prefix_compression_entry_attr_t
    {
        /**
         * @brief Start of attributes
         */
        SAI_PREFIX_COMPRESSION_ENTRY_ATTR_START,

        /**
         * @brief Prefix Compression entry META data
         *
         * @type sai_uint32_t
         * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
         */
        SAI_PREFIX_COMPRESSION_ENTRY_ATTR_META = SAI_PREFIX_COMPRESSION_ENTRY_ATTR_START,
```

#### sai_prefix_compression_entry_t ####

This structure defines a prefix compression entry. A prefix compression entry is composed of switch ID, a table ID that the entry will be added to and a IP prefix

```c
typedef struct _sai_prefix_compression_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Prefix Compression Table ID
     *
     * @objects SAI_OBJECT_TYPE_PREFIX_COMPRESSION_TABLE
     */
    sai_object_id_t prefix_table_id;

    /**
     * @brief IP Prefix Destination
     */
    sai_ip_prefix_t prefix;
```

#### sai_create_prefix_compression_table_fn ####
This defines the interface to create prefix compression table

- prefix_compression_table_id (Out): Prefix Compression table object ID
- switch_id (In): The ID of the switch on which the ICMP ECHO session is to be created.
- attr_count (In): The number of attributes provided in the attr_list.
- attr_list (In): An array of sai_attribute_t structures containing the attribute key-value pairs to configure the ICMP ECHO session.

#### sai_remove_prefix_compression_table_fn ####
The function takes a Prefix Compression object ID as a parameter. The function returns SAI_STATUS_SUCCESS if the operation is successful; otherwise, it returns a different error code indicating the nature of the failure.

#### sai_set_prefix_compression_table_attribute_fn ####
This defines the interfaces to update prefix compression table. It requires the unique identifier prefix_compression_table_id,  and the attr parameter represents the attribute to be set along with its value. The function returns SAI_STATUS_SUCCESS if the operation is successful; otherwise, it returns an error code, indicating the nature of the failure.

#### sai_get_prefix_compression_table_attribute_fn ####
It takes the unique identifier prefix_compression_table_id to specify the Prefix Compression table for which attributes are to be retrieved. The attr_count parameter indicates the number of attributes in the attr_list, and the attr_list itself holds the values of the requested attributes.

#### sai_create_prefix_compression_entry_fn ####
This defines the interface to create prefix compression entry

- prefix_compression_entry (In): Prefix Compression table object ID
- attr_count (In): The number of attributes provided in the attr_list.
- attr_list (In): An array of sai_attribute_t structures containing the attribute key-value pairs to configure the ICMP ECHO session.

#### sai_set_prefix_compression_entry_attribute_fn ####
This defines the interfaces to update prefix compression entry. It takes the entry to be modified, and the attr parameter represents the attribute to be set along with its value. The function returns SAI_STATUS_SUCCESS if the operation is successful; otherwise, it returns an error code, indicating the nature of the failure.

#### sai_get_prefix_compression_entry_attribute_fn ####
It takes a specific entry that will be used to reterive information about the entry. The attr_count parameter indicates the number of attributes in the attr_list, and the attr_list itself holds the values of the requested attributes.

#### sai_bulk_create_prefix_compression_entry_fn ####
This defines a bulk entry create API that is used to create multiple entries at once. This API takes:
- object_count (In): Number of objects to create
- prefix_compression_entry (In): List of object to create
- attr_count (In): List of attr_count. Caller passes the number of attribute for each object to create.
- attr_list (In): List of attributes for every object.
- mode (In): Bulk operation error handling mode.
- object_statuses (Out): List of status for every object. Caller needs to allocate the buffer

#### sai_bulk_remove_prefix_compression_entry_fn ####
This defines a bulk remove of entries in a prefix compression table:
- object_count (In): Number of objects to remove
- prefix_compression_entry (In):  List of objects to remove
- mode (In): Bulk operation error handling mode.
- object_statuses (Out): List of status for every object. Caller needs to allocate the buffer

#### sai_prefix_compression_api_t ####
```c
typedef struct _sai_prefix_compression_api_t
{
    sai_create_prefix_compression_table_fn              create_prefix_compression_table;
    sai_remove_prefix_compression_table_fn              remove_prefix_compression_table;
    sai_set_prefix_compression_table_attribute_fn       set_prefix_compression_table_attribute;
    sai_get_prefix_compression_table_attribute_fn       get_prefix_compression_table_attribute;
    sai_create_prefix_compression_entry_fn              create_prefix_compression_entry;
    sai_remove_prefix_compression_entry_fn              remove_prefix_compression_entry;
    sai_set_prefix_compression_entry_attribute_fn       set_prefix_compression_entry_attribute;
    sai_get_prefix_compression_entry_attribute_fn       get_prefix_compression_entry_attribute;
    sai_bulk_create_prefix_compression_entry_fn         create_prefix_compression_entries;
    sai_bulk_remove_prefix_compression_entry_fn         remove_prefix_compression_entries;
} sai_prefix_compression_api_t;
```

## 2.2 ACL with Prefix Compression

Enhanced support for meta-data field matching in ACL. New table attributes enable the configuration of source and destination prefix compression tables during the creation of an ACL table. Additionally, new field entry attributes allow matching on specific meta-data from either the source or destination prefix tables.


```c
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

New field entry attributes allow for lookups based on a meta-data value.

```c
   /**
     * @brief SRC meta-data
     *
     * @type sai_acl_field_data_t sai_uint32_t
     * @flags CREATE_AND_SET
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_FIELD_SRC_PREFIX_META = SAI_ACL_ENTRY_ATTR_FIELD_START + 0x15b,

    /**
     * @brief DST meta-data
     *
     * @type sai_acl_field_data_t sai_uint32_t
     * @flags CREATE_AND_SET
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_FIELD_DST_PREFIX_META = SAI_ACL_ENTRY_ATTR_FIELD_START + 0x15c,
```

## 2.2.1 Prefix Compression packet pipeline

New table attributes allow for the configuration of both source and destination prefix compression tables, which are used in field matching. The prefix compression SAI object ID is stored within the ACL table. An ACL table can be configured to use different prefix compression tables for the source and destination or the same table for both. This flexibility extends to different ACL tables as well, permitting the reuse of the same prefix compression table across multiple ACL tables or the creation of unique tables for each specific ACL table.

During meta-data entry lookup in an ACL, the packet derives the associated prefix compression table from the ACL table configuration, either using SAI_ACL_TABLE_ATTR_SRC_PREFIX_COMPRESSION_TABLE or SAI_ACL_TABLE_ATTR_DST_PREFIX_COMPRESSION_TABLE. There is no match criteria to derive the source or destination compression tables; this is not an action. Subsequently, a lookup is performed on either the source or destination compression table, resulting in a "metadata" value. This "metadata" is then used as match criteria for the ACL entry, determining a hit or miss. Standard ACL actions for a hit or miss will then take effect.

In hardware, there is no such derivation from the ACL tables; instead, there is an implicit pipeline behavior such that the prefix compression table lookup always occurs before the ACL table lookup. This ensures that the metadata from the compression table is available for matching in the ACL table.

Example: Two prefix compression tables, Table_1 and Table_2, are created. ACL Table_1 is then configured with the attributes SAI_ACL_TABLE_ATTR_SRC_PREFIX_COMPRESSION_TABLE set to Prefix Compression Table_1 and SAI_ACL_TABLE_ATTR_DST_PREFIX_COMPRESSION_TABLE set to Prefix Compression Table_2. Under ACL Table_1, two entries are created. Entry_1 references metadata from Prefix Compression Table_1, and Entry_2 references metadata from Prefix Compression Table_2. During the pipeline process for Entry_1, the ID of Prefix Compression Table_1 is dereferenced from ACL Table_1. there is not a result of an action. A lookup is performed on Prefix Compression Table_1, which is a separate table in the pipeline. The result of this lookup determines whether Entry_1 hits or misses in the ACL. The same process applies to Entry_2, with the lookup performed in Prefix Compression Table_2.

## 3 Examples

## 3.0.1 Create Prefix Compression Table with entries

```c
// Create Prefix Compression Table 1
sai_attr_table_list[];
sai_attr_table_list[0].id = SAI_ACL_TABLE_ATTR_PREFIX_COMPRESSION_STAGE;
sai_attr_table_list[0].value.s32 = SAI_PREFIX_COMPRESSION_STAGE_INGRESS;
sai_attr_table_list[0].id = SAI_PREFIX_COMPRESSION_TABLE_ATTR_PREFIX_COMPRESSION_TYPE;
sai_attr_table_list[0].value.s32 = SAI_PREFIX_COMPRESSION_TYPE_SRC;

attr_table_count = 0;
sai_create_prefix_compression_table_fn(
    &src_prefix_compression_table_id,
    switch_id,
    attr_table_count,
    sai_attr_table_list);

// Create Prefix Compression Table 2
sai_attr_table_list[];
sai_attr_table_list[0].id = SAI_ACL_TABLE_ATTR_PREFIX_COMPRESSION_STAGE;
sai_attr_table_list[0].value.s32 = SAI_PREFIX_COMPRESSION_STAGE_INGRESS;
sai_attr_table_list[0].id = SAI_PREFIX_COMPRESSION_TABLE_ATTR_PREFIX_COMPRESSION_TYPE;
sai_attr_table_list[0].value.s32 = SAI_PREFIX_COMPRESSION_TYPE_DST;

attr_table_count = 0;
sai_create_prefix_compression_table_fn(
    &dst_prefix_compression_table_id,
    switch_id,
    attr_table_count,
    sai_attr_table_list);

// Example: Create Prefix Compression Entries in SRC table
// IPV4 First Entry
sai_prefix_compression_entry_t entry_v4_1;
entry_v4_1.switch_id = switch_id;
entry_v4_1.prefix_table_id = src_prefix_compression_table_id;
entry_v4_1.prefix.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
entry_v4_1.prefix.addr.ipv4 = "1.1.1.1";
entry_v4_1.prefix_mask.ipv4 = "255.255.255.0";
sai_entry_list[0].id = SAI_PREFIX_COMPRESSION_ENTRY_ATTR_META;
sai_entry_list[0].value.u32 = 2;

// IPV4 Second Entry
sai_prefix_compression_entry_t entry_v4_2;
entry_v4_2.switch_id = switch_id;
entry_v4_2.prefix_table_id = src_prefix_compression_table_id;
entry_v4_2.prefix.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
entry_v4_2.prefix.addr.ipv4 = "2.2.2.1";
entry_v4_2.prefix_mask.ipv4 = "255.255.255.0";
sai_entries_attribute_list[0].id = SAI_PREFIX_COMPRESSION_ENTRY_ATTR_META;
sai_entries_attribute_list[0].value.u32 = 800;

// IPV6 Entry
sai_prefix_compression_entry_t entry_v6_1;
entry_v6_1.switch_id = switch_id;
entry_v6_1.prefix_table_id = src_prefix_compression_table_id;
entry_v6_1.prefix.addr_family = SAI_IP_ADDR_FAMILY_IPV6;
entry_v6_1.prefix.addr.ipv4 ="2001:1::4";
entry_v6_1.prefix_mask = "ffff:ffff:ffff:ffff:ffff:ffff:ffff:fffc";
sai_entries_attribute_list[1].id = SAI_PREFIX_COMPRESSION_ENTRY_ATTR_META;
sai_entries_attribute_list[1].value.u32 = 4;


// Example: Create Prefix Compression Entries in DST table
// IPV4 Entry
sai_prefix_compression_entry_t entry_v4_3;
entry_v4_3.switch_id = switch_id;
entry_v4_3.prefix_table_id = dst_prefix_compression_table_id;
entry_v4_3.prefix.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
entry_v4_3.prefix.addr.ipv4 = "12.12.1.1";
entry_v4_3.prefix_mask.ipv4 = "255.255.0.0";
sai_entry_list[0].id = SAI_PREFIX_COMPRESSION_ENTRY_ATTR_META;
sai_entry_list[0].value.u32 = 200;

entries_list = [entry_v4_2, entry_v6_1, entry_v4_3];
entries_list_size = 2;
sai_status_t bulk_status;

// Adding a single entry to a table
sai_attr_list_count = 1;
sai_create_prefix_compression_entry_fn(
    entry_v4_1,
    sai_attr_list,
    sai_attr_list_count);

// Adding using bulk add
attr_entries_count = 2;
sai_bulk_create_prefix_compression_entry_fn(
    entries_list_size,
    entries_list,
    attr_entries_count,
    sai_entries_attribute_list,
    SAI_BULK_OP_ERROR_MODE_STOP_ON_ERROR,
    bulk_status);

// Example of using Prefix Compression with ACL
// Create ACL table
sai_table_attr_list[0].id = SAI_ACL_TABLE_ATTR_SRC_PREFIX_COMPRESSION_TABLE;
sai_table_attr_list[0].value.oid = src_prefix_compression_table_id;
sai_table_attr_list[0].id = SAI_ACL_TABLE_ATTR_DST_PREFIX_COMPRESSION_TABLE;
sai_table_attr_list[0].value.oid = dst_prefix_compression_table_id;
sai_table_attr_count = 1;

sai_create_acl_table_fn(
    &acl_table_id,
    switch_id,
    sai_table_attr_count,
    sai_table_attr_list);

// Entry 1
sai_entry_attr_list[0].id = SAI_ACL_ENTRY_ATTR_FIELD_SRC_PREFIX_META;
sai_attr_list[1].value.aclfield.enable = true;
sai_attr_list[1].value.aclfield.mask = 0xFFFFFF;
sai_attr_list[1].value.aclfield.data = 800;
sai_entry_attr_count = 1;

// Create an Entry to match on META data
sai_create_acl_entry_fn(
    &acl_entry_id_1,
    switch_id,
    sai_entry_attr_count,
    sai_entry_attr_list);

// Entry 2
sai_entry_attr_list[0].id = SAI_ACL_ENTRY_ATTR_FIELD_DST_PREFIX_META;
sai_attr_list[1].value.aclfield.enable = true;
sai_attr_list[1].value.aclfield.mask = 0xFFFFFF;
sai_attr_list[1].value.aclfield.data = 200;
sai_entry_attr_count = 1;

// Create an Entry to match on META data
sai_create_acl_entry_fn(
    &acl_entry_id_2,
    switch_id,
    sai_entry_attr_count,
    sai_entry_attr_list);

// Remove ACL entry
sai_remove_acl_entry_fn(
    acl_entry_id_1);

// Remove ACL Table
sai_remove_acl_entry_fn(
    acl_table_id_2);

// Remove Single Entry
sai_remove_prefix_compression_entry_fn(
    entry_v4_1);

// Remove Bulk Entries
sai_bulk_remove_prefix_compression_entry_fn(
    entries_list_size,
    entries_list,
    SAI_BULK_OP_ERROR_MODE_STOP_ON_ERROR,
    bulk_status);

// Remove Prefix Compression Table
sai_remove_prefix_compression_table_fn(
    src_prefix_compression_table_id);

```