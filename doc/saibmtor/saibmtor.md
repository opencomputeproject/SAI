# BMTOR Overview

![](https://github.com/marian-pritsak/SAI/blob/patch-2/doc/saibmtor/VXLAN%20pipe.PNG)

SAI BMTOR API defines a new kind of a routing table that allows a route entry to be in multiple VRF at the same time. In the standard router, exact match is done on VRF ID and lpm on prefix. In BMTOR tunnel table, every VRF ID is assigned a single bit in a bitvector instead of some integer number, which allows us to match on multiple VRF IDs at the same time without duplicating route entries for each VRF.

In standard router, VRF (VNET) classification is done by assigning router interface to virtual router. BMTOR API allows a user do classification based on either:
* packet VLAN id
* ingress port
* VxLAN packet VNI

Those fields can be used separately or together with ternary match on all of them. Table for VRF (VNET) classification is called VNET table.

VNET table is bound to all ports in the switch, and Tunnel Route table is bound to all RIFs

Tunnel Route table supports following actions (similar to standard router):
* Go to local RIF
* Go to nexthop
* Go to port
* Drop

Following APIs are used to populate entries:

```
typedef sai_status_t (*sai_create_table_vnet_entry_fn)(
        _Out_ sai_object_id_t *table_vnet_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_create_table_tunnel_route_entry_fn)(
        _Out_ sai_object_id_t *table_tunnel_route_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);
```
