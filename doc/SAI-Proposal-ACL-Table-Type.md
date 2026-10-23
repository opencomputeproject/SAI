# ACL Table IP Type Attribute

## Revision History

| Version | Date       | Author             | Description   |
| ------- | ---------- | ------------------ | ------------- |
| 0.1     | 2026-09-1  | Anandhi Dhanabalan | Initial draft |

---

# 1. Overview

SAI currently defines the packet type of an ACL entry through `SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE`, but does not provide an ACL-table-level attribute to declare the intended packet domain of the table.

NOSes such as SONiC create separate ACL tables for different packet domains, for example IPv4 and IPv6. Since an ACL table is created before its entries, the intended packet domain is not always available to the underlying implementation from the ACL entry match fields.

This proposal introduces `SAI_ACL_TABLE_ATTR_IP_TYPE` to allow the NOS to explicitly declare the packet domain of an ACL table.

The attribute enables:

1. Explicit creation of protocol-specific ACL tables by the NOS.
2. ACL resource optimization by the underlying implementation where supported.

The attribute applies to all ACL stages and defines the intended packet domain at table creation time. It does not perform packet matching.

## 1.1. SONiC Reference

SONiC models packet-specific ACL tables through predefined ACL table types. The SWSS schema identifies predefined types including:

* `L3` → IPv4 ACL table
* `L3V6` → IPv6 ACL table
* `MIRROR` → IPv4 mirror ACL table
* `MIRRORV6` → IPv6 mirror ACL table

This demonstrates that the NOS already expresses packet-domain intent at ACL table creation time. The proposed SAI attribute makes this intent explicit at the SAI ACL table level rather than encoding it in implementation-specific table type names or deriving it from the table match fields.

---

# 3. Proposed SAI Changes

## 3.1 New Enumeration

```c
typedef enum _sai_acl_table_ip_type_t
{
    /**
     * @brief Packet type is not restricted.
     *
     * The table may contain ACL entries matching IPv4, IPv6,
     * and non-IP packets.
     */
    SAI_ACL_TABLE_IP_TYPE_ANY,

    /**
     * @brief IPv4 and IPv6 packets.
     */
    SAI_ACL_TABLE_IP_TYPE_IP,

    /**
     * @brief IPv4 packets (including ARP).
     */
    SAI_ACL_TABLE_IP_TYPE_IPV4,

    /**
     * @brief IPv6 packets.
     */
    SAI_ACL_TABLE_IP_TYPE_IPV6,

    /**
     * @brief Non-IP packets.
     */
    SAI_ACL_TABLE_IP_TYPE_NON_IP

} sai_acl_table_ip_type_t;
```

## 3.2 New ACL Table Attribute

```c
/**
 * @brief ACL Table IP Type
 *
 * Defines the packet domain supported by the ACL table.
 *
 * @type sai_acl_table_ip_type_t
 * @flags CREATE_ONLY
 * @default SAI_ACL_TABLE_IP_TYPE_ANY
 */
SAI_ACL_TABLE_ATTR_IP_TYPE,
```

---

# 4. Relationship with existing attributes

SAI today defines two related but distinct mechanisms involving IP type:

1. **ACL Table Field Enablement**

    `SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_TYPE`

    - Declares whether the ACL table supports matching on IP type.
    - Used during ACL table creation to enable the ACL_IP_TYPE match field.
    - Does not restrict the packet domain of the table.
    - If enabled, ACL entries may use SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE.

2. **ACL Entry Match**

    `SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE`

    - Optional match field at the ACL entry level.
    - Provides finer-grained packet-type matching (e.g., ARP, ARP_REQUEST).
    - Requires SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_TYPE to be enabled in the table.

3. **ACL Table Packet Domain (NEW)**

    `SAI_ACL_TABLE_ATTR_IP_TYPE`

    - Declares the packet domain of the ACL table (e.g., IPv4, IPv6, Non-IP).
    - Constrains which packets can be processed by the table.
    - Constrains which ACL entries and match fields are valid in the table.
    - Independent of whether ACL_IP_TYPE matching is enabled.
  
| Attribute                                   | Level | Purpose                        | Restricts Packet Domain |
|---------------------------------------------|-------|--------------------------------|-------------------------|
| `SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_TYPE`      | Table | Enables IP-type match field    | No                      |
| `SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE`      | Entry | Matches specific packet types  | Yes (entry-level)       |
| `SAI_ACL_TABLE_ATTR_IP_TYPE` **(NEW)**      | Table | Declares packet domain         | Yes                     |


For example:

```text
ACL Table:  IP_TYPE = ANY
            ACL_IP_TYPE = TRUE
ACL Entry:  ACL_IP_TYPE = SAI_ACL_IP_TYPE_IPV4ANY
```

is valid because `SAI_ACL_IP_TYPE_IPV4ANY` is within the ANY packet domain.

In contrast:

```text
ACL Table:  IP_TYPE = IPV4
            ACL_IP_TYPE = TRUE
ACL Entry:  ACL_IP_TYPE = SAI_ACL_IP_TYPE_IPV6ANY
```
is invalid because the entry requests an IPv6 packet domain that is outside the table's declared domain.

Similarly:
```text
ACL Table:  IP_TYPE = ANY
            ACL_IP_TYPE = FALSE
ACL Entry:  ACL_IP_TYPE = SAI_ACL_IP_TYPE_IPV4ANY
```
is invalid because `SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_TYPE` is not enabled in the table — entries cannot use ACL_IP_TYPE as a match field.


## 4.1 Compatibility

The following combinations illustrate the required relationship between the table packet domain and the existing ACL entry IP type:

| Table IP Type | Permitted entry IP type (`sai_acl_ip_type_t`) |
| ------------- | --------------------------------------------- |
| `ANY`         | All supported `sai_acl_ip_type_t` values |
| `IP`          | `SAI_ACL_IP_TYPE_IP`, `SAI_ACL_IP_TYPE_IPV4ANY`, `SAI_ACL_IP_TYPE_IPV6ANY`, `SAI_ACL_IP_TYPE_ARP`, `SAI_ACL_IP_TYPE_ARP_REQUEST`, `SAI_ACL_IP_TYPE_ARP_REPLY`  |
| `IPV4`        | `SAI_ACL_IP_TYPE_IPV4ANY`, `SAI_ACL_IP_TYPE_ARP`, `SAI_ACL_IP_TYPE_ARP_REQUEST`, `SAI_ACL_IP_TYPE_ARP_REPLY` <br> _(ARP included — classified within IPv4 domain per §5.1)_ |
| `IPV6`        | `SAI_ACL_IP_TYPE_IPV6ANY` |
| `NON_IP`      | `SAI_ACL_IP_TYPE_NON_IP` |

An ACL entry specifying a packet domain broader than the table packet domain shall be rejected with `SAI_STATUS_INVALID_PARAMETER`.

Similarly, IP-specific match fields shall be consistent with the table packet domain:
- IPv6-specific match fields shall not be permitted in an IPV4 table.
- IPv4-specific match fields shall not be permitted in an IPV6 table.
- IPv4/IPv6 header match fields shall not be permitted in a NON_IP table.

The table IP type defines the permitted packet domain; it does not itself perform packet matching. Packet matching continues to be determined by the ACL entry match fields.

## 4.2 ACL Entry Validation Workflow

When an ACL entry is created, the implementation shall:

1. Obtain the `SAI_ACL_TABLE_ATTR_IP_TYPE` of the associated ACL table.
2. If `SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE` is specified, verify that the entry packet domain is compatible with the table packet domain.
3. Verify that IP-specific ACL match fields are compatible with the table packet domain.
4. If the entry packet domain or match fields are incompatible with the table packet domain, return `SAI_STATUS_INVALID_PARAMETER`.


> **Note:-**  
> `SAI_ACL_IP_TYPE_NON_IPV4` and `SAI_ACL_IP_TYPE_NON_IPV6` are not
> permitted in any restricted table domain (IP/IPV4/IPV6/NON_IP)
> as their packet domains are broader than any single table domain.
> They are only permitted in `SAI_ACL_TABLE_IP_TYPE_ANY` tables
---

# 5. Non-IP Packet Handling

`SAI_ACL_TABLE_IP_TYPE_NON_IP` represents the non-IP packet domain. 
For example the following packet types are within the `NON_IP` domain: LLDP, STP, LACP, EAPOL and MPLS.

These packets may be matched in ACL tables configured with either `SAI_ACL_TABLE_IP_TYPE_NON_IP` or `SAI_ACL_TABLE_IP_TYPE_ANY`.

## 5.1 ARP Classification

ARP is classified as part of the `IPV4` domain, not `NON_IP`. This is consistent with the existing SAI spec — `sai_acl_ip_type_t` defines `SAI_ACL_IP_TYPE_ARP`, `SAI_ACL_IP_TYPE_ARP_REQUEST`, and `SAI_ACL_IP_TYPE_ARP_REPLY` as distinct values separate from `SAI_ACL_IP_TYPE_NON_IP`. Furthermore, per RFC 826, ARP is exclusively tied to IPv4 address resolution and carries IPv4 addresses in its payload — it is not a general-purpose L2 control protocol.

Therefore:

* ARP may be matched in tables configured with `SAI_ACL_TABLE_IP_TYPE_IPV4` or `SAI_ACL_TABLE_IP_TYPE_ANY`
* ARP shall not be matched in tables configured with `SAI_ACL_TABLE_IP_TYPE_NON_IP`

---

# 6. Backward Compatibility

The new attribute is optional and defaults to `SAI_ACL_TABLE_IP_TYPE_ANY`.

Existing applications that do not specify the attribute retain their existing behavior.

Implementations that do not support this attribute shall indicate this through capability queries.

---

# 7. NOS Workflow

```c
/* enum_list_contains(): helper to search sai_s32_list_t for a value */
/* create_acl_table_using_existing_behavior(): fallback, implementation-defined */

/*
 * Discover SAI_ACL_TABLE_ATTR_IP_TYPE capability, select a supported
 * IP type, and create the ACL table with the selected packet domain.
 */

sai_status_t status;
sai_attr_capability_t capability;
sai_object_id_t acl_table_id;

/* Check attribute support */
status = sai_query_attribute_capability(
    switch_id,
    SAI_OBJECT_TYPE_ACL_TABLE,
    SAI_ACL_TABLE_ATTR_IP_TYPE,
    &capability);

if (status == SAI_STATUS_SUCCESS &&
    capability.create_implemented)
{
    sai_s32_list_t supported_ip_types;

    /* Query supported enum values */
    status = sai_query_attribute_enum_values_capability(
        switch_id,
        SAI_OBJECT_TYPE_ACL_TABLE,
        SAI_ACL_TABLE_ATTR_IP_TYPE,
        &supported_ip_types);

    /*
     *  If SAI_ACL_TABLE_IP_TYPE_IPV4 is supported, the NOS can
     *  explicitly create an IPv4 ACL table.
     */
    if (status == SAI_STATUS_SUCCESS &&
        enum_list_contains(
            &supported_ip_types,
            SAI_ACL_TABLE_IP_TYPE_IPV4))
    {
        sai_attribute_t acl_table_attr[5];
        sai_int32_t bind_point = SAI_ACL_BIND_POINT_TYPE_PORT;

        acl_table_attr[0].id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
        acl_table_attr[0].value.s32 = SAI_ACL_STAGE_INGRESS;

        acl_table_attr[1].id = SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST;
        acl_table_attr[1].value.s32list.count = 1;
        acl_table_attr[1].value.s32list.list = &bind_point;

        acl_table_attr[2].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_IP;
        acl_table_attr[2].value.booldata = true;


        /*
        * Enable ACL_IP_TYPE as a match field in entries.
        * Required if entries will use SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE.
        */
        acl_table_attr[3].id    = SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_TYPE;
        acl_table_attr[3].value.booldata = true;

        /*
         * Explicitly declare the table as IPv4.
         */
        acl_table_attr[4].id = SAI_ACL_TABLE_ATTR_IP_TYPE;
        acl_table_attr[4].value.s32 = SAI_ACL_TABLE_IP_TYPE_IPV4;

        status = sai_acl_api->create_acl_table(
            &acl_table_id,
            switch_id,
            5,
            acl_table_attr);
    }
    else
    {
        /*
         * New attribute or requested IP type is unavailable.
         * Use the existing ACL table creation behavior.
         */
        create_acl_table_using_existing_behavior();
    }
}
else
{
    /*
     * New attribute is unavailable.
     * Use the existing ACL table creation behavior.
     */
    create_acl_table_using_existing_behavior();
}
```

