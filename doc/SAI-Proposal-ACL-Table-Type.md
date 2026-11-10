# ACL Table Packet Type List Attribute

## Revision History

| Version | Date       | Author             | Description                                                                                       |
| ------- | ---------- | ------------------ | ------------------------------------------------------------------------------------------------- |
| 0.1     | 2026-09-1  | Anandhi Dhanabalan | Initial draft                                                                                     |
| 0.2     | 2026-10-27 | Anandhi Dhanabalan, Gai Nachum | Replace single-value IP type with a list of packet types. ARP is an explicit packet type. Remove `ANY`. |

---

# 1. Overview

SAI currently defines the packet type of an ACL entry through `SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE`, but does not provide an ACL-table-level attribute to declare the intended packet domain of the table.

NOSes such as SONiC create separate ACL tables for different packet domains, for example IPv4 and IPv6. Some tables combine domains, for example IPv4 together with non-IP.

Since an ACL table is created before its entries, the intended packet domain is not always available to the underlying implementation from the ACL entry match fields.

This proposal introduces `SAI_ACL_TABLE_ATTR_ACL_PACKET_TYPE_LIST` to allow the NOS to explicitly declare the list of packet types that the entries of an ACL table may match.

The attribute enables:

1. Explicit creation of protocol-specific ACL tables by the NOS.
2. ACL resource optimization by the underlying implementation where supported.

The attribute applies to all ACL stages and defines the intended packet domain at table creation time.  
It does not perform packet matching.

## 1.1 SONiC Reference

SONiC models packet-specific ACL tables through predefined ACL table types.  
The SWSS schema identifies predefined types including:

* `L3` → IPv4 ACL table
* `L3V6` → IPv6 ACL table
* `MIRROR` → IPv4 mirror ACL table
* `MIRRORV6` → IPv6 mirror ACL table

In the separated mirror model, the `MIRROR` table carries the IPv4 match fields and also the non-IP match fields (for example `ETHERTYPE`), while `MIRRORV6` carries the IPv6 match fields. A single table can therefore need more than one packet type, for example IPv4 and non-IP, but not IPv6.

This demonstrates that the NOS already expresses packet-domain intent at ACL table creation time.
The proposed SAI attribute makes this intent explicit at the SAI ACL table level rather than encoding it in implementation-specific table type names or deriving it from the table match fields.

A list of packet types is used because a single value cannot express combined domains such as IPv4 and non-IP. This is the same pattern as `SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST`.

---

# 3. Proposed SAI Changes

## 3.1 New Enumeration

```c
/**
 * @brief ACL table packet types
 *
 * Packet types that the entries of an ACL table may match.
 */
typedef enum _sai_acl_packet_type_t
{
    /**
     * @brief IPv4 packets (excluding ARP).
     */
    SAI_ACL_PACKET_TYPE_IPV4,

    /**
     * @brief IPv6 packets.
     */
    SAI_ACL_PACKET_TYPE_IPV6,

    /**
     * @brief Non-IP packets (excluding ARP).
     */
    SAI_ACL_PACKET_TYPE_NON_IP,

    /**
     * @brief ARP packets.
     *
     * Includes ARP request and ARP reply.
     */
    SAI_ACL_PACKET_TYPE_ARP

} sai_acl_packet_type_t;
```

There is no `ANY` value. An empty list means the table is unrestricted (see 3.2 and 6).

## 3.2 New ACL Table Attribute

```c
/**
 * @brief ACL Table Packet Type list
 *
 * Packet types that the entries of this table may match, for example
 * [IPV4, NON_IP, ARP].
 *
 * Empty list: the attribute is not configured. The table is unrestricted
 * and behaves as if all packet types were listed. No packet-type
 * validation is applied to entries, and the implementation selects the
 * table resources based on match qualifiers configured.
 *
 * @type sai_s32_list_t sai_acl_packet_type_t
 * @flags CREATE_ONLY
 * @default empty
 */
SAI_ACL_TABLE_ATTR_ACL_PACKET_TYPE_LIST,
```

---

# 4. Relationship with existing attributes

SAI today defines two related but distinct mechanisms involving IP type:

1. **ACL Table Field Enablement**
   `SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_TYPE`
   - Declares whether the ACL table supports matching on IP type.
   - Used during ACL table creation to enable the ACL_IP_TYPE match field.
   - Does not restrict the packet domain of the table.
   - If enabled, ACL entries may use `SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE`.

2. **ACL Entry Match**
   `SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE`
   - Optional match field at the ACL entry level.
   - Provides finer-grained packet-type matching (e.g., ARP, ARP_REQUEST).
   - Requires `SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_TYPE` to be enabled in the table.

3. **ACL Table Packet Types (NEW)**
   `SAI_ACL_TABLE_ATTR_ACL_PACKET_TYPE_LIST`
   - Declares the packet types of the ACL table (e.g., IPv4, IPv6, Non-IP, ARP).
   - Constrains which packets can be processed by the table.
   - Constrains which ACL entries and match fields are valid in the table.
   - Independent of whether ACL_IP_TYPE matching is enabled.

| Attribute                                          | Level | Purpose                          | Restricts Packet Domain |
|----------------------------------------------------|-------|----------------------------------|-------------------------|
| `SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_TYPE`             | Table | Enables IP-type match field      | No                      |
| `SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE`             | Entry | Matches specific packet types    | Yes (entry-level)       |
| `SAI_ACL_TABLE_ATTR_ACL_PACKET_TYPE_LIST` **(NEW)** | Table | Declares packet types of the table | Yes (when not empty)    |

For example:

```text
ACL Table:  ACL_PACKET_TYPE_LIST = [IPV4, NON_IP]
            ACL_IP_TYPE = TRUE
ACL Entry:  ACL_IP_TYPE = SAI_ACL_IP_TYPE_IPV4ANY
```

is valid because `SAI_ACL_IP_TYPE_IPV4ANY` is covered by `IPV4` in the table's list.

In contrast:

```text
ACL Table:  ACL_PACKET_TYPE_LIST = [IPV4]
            ACL_IP_TYPE = TRUE
ACL Entry:  ACL_IP_TYPE = SAI_ACL_IP_TYPE_IPV6ANY
```

is invalid because the entry requests an IPv6 packet type that is not in the table's list.

Similarly:

```text
ACL Table:  ACL_PACKET_TYPE_LIST = [IPV4]
            ACL_IP_TYPE = TRUE
ACL Entry:  ACL_IP_TYPE = SAI_ACL_IP_TYPE_ARP
```

is invalid because `ARP` is not listed. `IPV4` does not imply `ARP`. The table must be created with `[IPV4, ARP]`.

Similarly:

```text
ACL Table:  ACL_PACKET_TYPE_LIST = [IPV4]
            ACL_IP_TYPE = FALSE
ACL Entry:  ACL_IP_TYPE = SAI_ACL_IP_TYPE_IPV4ANY
```

is invalid because `SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_TYPE` is not enabled in the table. Entries cannot use ACL_IP_TYPE as a match field.

Finally:

```text
ACL Table:  ACL_PACKET_TYPE_LIST = [] (not configured)
            ACL_IP_TYPE = TRUE
ACL Entry:  ACL_IP_TYPE = SAI_ACL_IP_TYPE_IPV6ANY
```

is valid because an empty list means the table is unrestricted and no packet-type validation is applied.

## 4.1 Compatibility

When the list is not empty, an ACL entry `SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE` is valid only if every packet type in the **Required list entries** column is present in the table's list:

| Entry IP type (`sai_acl_ip_type_t`)                                   | Required list entries                |
| --------------------------------------------------------------------- | ------------------------------------ |
| `SAI_ACL_IP_TYPE_ANY`                                                 | `IPV4`, `IPV6`, `NON_IP`, `ARP`      |
| `SAI_ACL_IP_TYPE_IP`                                                  | `IPV4`, `IPV6`                       |
| `SAI_ACL_IP_TYPE_IPV4ANY`                                             | `IPV4`                               |
| `SAI_ACL_IP_TYPE_IPV6ANY`                                             | `IPV6`                               |
| `SAI_ACL_IP_TYPE_NON_IP`                                              | `NON_IP`                             |
| `SAI_ACL_IP_TYPE_ARP`, `SAI_ACL_IP_TYPE_ARP_REQUEST`, `SAI_ACL_IP_TYPE_ARP_REPLY` | `ARP`                |
| `SAI_ACL_IP_TYPE_NON_IPV4`                                            | `IPV6`, `NON_IP`                     |
| `SAI_ACL_IP_TYPE_NON_IPV6`                                            | `IPV4`, `NON_IP`                     |

An ACL entry specifying a packet type that is not covered by the table's list shall be rejected with `SAI_STATUS_INVALID_PARAMETER`.

Similarly, IP-specific match fields shall be consistent with the table's list:

* IPv6-specific match fields require `IPV6` in the list.
* IPv4-specific match fields require `IPV4` in the list.
* IPv4/IPv6 header match fields shall not be permitted in a table whose list has neither `IPV4` nor `IPV6` (for example `[NON_IP]` or `[ARP]`).

The table packet type list defines the permitted packet domain; it does not itself perform packet matching.
Packet matching continues to be determined by the ACL entry match fields.

## 4.2 ACL Entry Validation Workflow

When an ACL entry is created, the implementation shall:

1. Obtain the `SAI_ACL_TABLE_ATTR_ACL_PACKET_TYPE_LIST` of the associated ACL table.
2. If the list is empty, skip the packet-type validation in steps 3 to 5.
3. If `SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE` is specified, verify that the packet types required by the entry (table in 4.1) are all present in the table's list.
4. Verify that IP-specific ACL match fields are compatible with the table's list.
5. If the entry packet type or match fields are incompatible with the table's list, return `SAI_STATUS_INVALID_PARAMETER`.

---

# 5. Non-IP Packet Handling

`SAI_ACL_PACKET_TYPE_NON_IP` represents the non-IP packet domain, excluding ARP.

For example the following packet types are within the `NON_IP` domain: LLDP, STP, LACP, EAPOL and MPLS.

These packets may be matched in ACL tables whose list contains `NON_IP`, or whose list is empty.

## 5.1 ARP Handling

ARP is a separate packet type, `SAI_ACL_PACKET_TYPE_ARP`. It is not part of `IPV4` or `NON_IP`.

Hardware classification of ARP differs between implementations. Some ASICs classify ARP together with IPv4 packets, others together with non-IP packets. The SAI specification does not fix ARP to either domain. Instead the NOS declares ARP explicitly, and the implementation maps the listed packet types to its own hardware classification.

Therefore:

* ARP request, reply and `SAI_ACL_IP_TYPE_ARP*` entries may be matched only in tables whose list contains `ARP`, or whose list is empty.
* `IPV4` and `NON_IP` do not imply `ARP`.
* The table needs resources for ARP according to the ASIC's classification. For example, `[IPV6, ARP]` may require IPv4 resources on one ASIC and non-IP resources on another. This is an implementation detail and is not visible to the NOS.
* If the implementation cannot support a combination of packet types, `create_acl_table` shall return `SAI_STATUS_NOT_SUPPORTED`.

Note that IPv6 does not use ARP. IPv6 address resolution uses Neighbor Discovery over ICMPv6, which is part of the `IPV6` packet type.

## 5.2 Example Lists

| Table purpose                           | List                          |
| --------------------------------------- | ----------------------------- |
| IPv4 table                              | `[IPV4]`                      |
| IPv4 table that also matches ARP        | `[IPV4, ARP]`                 |
| IPv6 table                              | `[IPV6]`                      |
| Non-IP table                            | `[NON_IP]`                    |
| SONiC `MIRROR` (separated model)        | `[IPV4, NON_IP]`              |
| SONiC `MIRRORV6` (separated model)      | `[IPV6]`                      |
| Combined table with all packet types    | `[]` or `[IPV4, IPV6, NON_IP, ARP]` |

---

# 6. Backward Compatibility

The new attribute is optional and defaults to an empty list.

An empty list means the attribute is not configured. The table is unrestricted: it behaves as if all packet types were listed, no packet-type validation is applied to entries, and the implementation selects the table resources.

Existing applications that do not specify the attribute retain their existing behavior.

Duplicate values in the list are ignored.

Implementations that do not support this attribute shall indicate this through capability queries.
`sai_query_attribute_enum_values_capability()` reports the packet types supported by the implementation.
A combination of packet types that cannot be realized by the implementation shall be rejected at table creation with `SAI_STATUS_NOT_SUPPORTED`.

---

# 7. NOS Workflow

```c
/* enum_list_contains(): helper to search sai_s32_list_t for a value */
/* create_acl_table_using_existing_behavior(): fallback, implementation-defined */

/*
 * Discover SAI_ACL_TABLE_ATTR_ACL_PACKET_TYPE_LIST capability, select
 * supported packet types, and create the ACL table with them.
 * This example creates a SONiC MIRROR table: IPv4 and non-IP packets.
 */

sai_status_t status;
sai_attr_capability_t capability;
sai_object_id_t acl_table_id;

/* Check attribute support */
status = sai_query_attribute_capability(
             switch_id,
             SAI_OBJECT_TYPE_ACL_TABLE,
             SAI_ACL_TABLE_ATTR_ACL_PACKET_TYPE_LIST,
             &capability);

if (status == SAI_STATUS_SUCCESS &&
    capability.create_implemented)
{
    sai_s32_list_t supported_packet_types;

    /* Query supported enum values */
    status = sai_query_attribute_enum_values_capability(
                 switch_id,
                 SAI_OBJECT_TYPE_ACL_TABLE,
                 SAI_ACL_TABLE_ATTR_ACL_PACKET_TYPE_LIST,
                 &supported_packet_types);

    /*
     * If SAI_ACL_PACKET_TYPE_IPV4 and SAI_ACL_PACKET_TYPE_NON_IP are
     * both supported, the NOS can explicitly create the table.
     */
    if (status == SAI_STATUS_SUCCESS &&
        enum_list_contains(&supported_packet_types, SAI_ACL_PACKET_TYPE_IPV4) &&
        enum_list_contains(&supported_packet_types, SAI_ACL_PACKET_TYPE_NON_IP))
    {
        sai_attribute_t acl_table_attr[5];
        sai_int32_t bind_point = SAI_ACL_BIND_POINT_TYPE_PORT;
        sai_int32_t packet_types[2] = {
            SAI_ACL_PACKET_TYPE_IPV4,
            SAI_ACL_PACKET_TYPE_NON_IP };

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
        acl_table_attr[3].id = SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_TYPE;
        acl_table_attr[3].value.booldata = true;

        /*
         * Explicitly declare the packet types of the table: IPv4 and non-IP.
         */
        acl_table_attr[4].id = SAI_ACL_TABLE_ATTR_ACL_PACKET_TYPE_LIST;
        acl_table_attr[4].value.s32list.count = 2;
        acl_table_attr[4].value.s32list.list = packet_types;

        status = sai_acl_api->create_acl_table(
                     &acl_table_id,
                     switch_id,
                     5,
                     acl_table_attr);

        if (status == SAI_STATUS_NOT_SUPPORTED)
        {
            /* Combination not supported. Use the existing behavior. */
            create_acl_table_using_existing_behavior();
        }
    }
    else
    {
        /*
         * New attribute or requested packet types are unavailable.
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
