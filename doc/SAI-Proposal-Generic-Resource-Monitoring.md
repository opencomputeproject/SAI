Switch Abstraction Interface Change Proposal

Title       | Generic Resource Query
------------|----------------
Authors     | Mellanox
Status      | In review
Type        | Standards track
Created     | 04/15/2019
SAI-Version | 1.4
----------

## Overview
SAI manages ASIC resources. It is important for the user to query the current resources usage in the ASIC for different types of SAI objects.
It is also importaint to make as much resources availability exposed as possible.

## Spec

This proposal introduces an API for querying resource availability on per object basis.
```
/**
 * @brief Get SAI object type resource availability.
 *
 * @param[in] switch_id SAI Switch object id
 * @param[in] object_type SAI object type
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list List of attributes that to distinguish resource
 * @param[out] count Available objects left
 *
 * @return #SAI_STATUS_NOT_SUPPORTED if the given object type does not support resource accounting.
 * Otherwise, return #SAI_STATUS_SUCCESS.
 */
sai_status_t sai_object_type_get_availability(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list,
        _Out_ uint64_t *count);
```

To help distibguish between different resource pools for the same object type, attributes that are annotated with `@isresourcetype` must be passed to the function with corresponding values.
For example, different types of route entries are distinguished by the following attribute:
```
  /** READ-ONLY */

    /**
     * @brief Route entry IP address family
     *
     * @type sai_ip_addr_family_t
     * @flags READ_ONLY
     * @isresourcetype true
     */
    SAI_ROUTE_ENTRY_ATTR_IP_ADDR_FAMILY,
```

## Examples
```
sai_attribute_t attr;
sai_status_t status;
uint64_t count;

attr.id = SAI_ROUTE_ENTRY_ATTR_IP_ADDR_FAMILY;
attr.value.s32 = SAI_IP_ADDR_FAMILY_IPV4;

/* Get available v4 routes */
status = sai_object_type_get_availability(g_switch_id, SAI_OBJECT_TYPE_ROUTE, 1, &attr, &count);

attr.id = SAI_ROUTE_ENTRY_ATTR_IP_ADDR_FAMILY;
attr.value.s32 = SAI_IP_ADDR_FAMILY_IPV6;

/* Get available v6 routes */
status = sai_object_type_get_availability(g_switch_id, SAI_OBJECT_TYPE_ROUTE, 1, &attr, &count);

attr.id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
attr.value.oid = g_table_id;

/* Get available ACL entries for a given table id */
status = sai_object_type_get_availability(g_switch_id, SAI_OBJECT_TYPE_ACL_ENTRY, 1, &attr, &count);
```

## API behavior for the shared resource pools
If two objects share the same resource pool, e.g. IPv4 and IPv6 routes, then the object availability returned by the API is with an assumption that no other object type from that same pool is going to be created.
