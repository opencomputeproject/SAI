
Add support for QinQ router interface and routing-based VxLAN
-------------------------------------------------------------------------------

 Title       | QinQ router interface and routing-based VxLAN
-------------|-----------------------------------------------------------------
 Authors     | Microsoft
 Status      | In review
 Type        | Standards track
 Created     | 05/04/2017
 Updated     | 08/03/2018
 SAI-Version | 1.2

-------------------------------------------------------------------------------

## Overview ##

The SAI representation of the QinQ-VxLAN-translator router is below:

![qinq-to-vxlan](figures/qinq-to-vxlan.png "Figure 2: QinQ to VxLAN usage scenario")
__Figure 1: QinQ to VxLAN usage scenario__
 
The packet processing pipeline is below:
![qinq-to-vxlan-pipeline](figures/qinq-to-vxlan-pipeline.png "Figure 2: QinQ to VxLAN SAI Pipeline")
__Figure 2: QinQ to VxLAN SAI Pipeline__
 
## Packet Format ##

- QinQ -> vxlan: For the vxlan packet , the inner L2 header is a dummy header without vlan header.

- Vxlan -> QinQ: For the vxlan packet, the inner L2 header is also a dummy header with no vlan header.

## SAI Header changes

- TPID for Outer and Inner VLAN

```
   typedef enum _sai_switch_attr_t
   {
       ...
       
       /**
        * @brief TPID for Outer vlan id
        *
        * @type sai_uint16_t
        * @flags CREATE_AND_SET
        * @default 0x88a8
        */
       SAI_SWITCH_ATTR_TPID_OUTER_VLAN;
       
       /**
        * @brief TPID for Inner vlan id
        *
        * @type sai_uint16_t
        * @flags CREATE_AND_SET
        * @default 0x8100
        */
       SAI_SWITCH_ATTR_TPID_INNER_VLAN;
       
   } sai_switch_attr_t;
```

- QinQ router interface type

```
    typedef enum _sai_router_interface_type_t
    {
        ...
        
        /** QinQ Router Interface Type */
        SAI_ROUTER_INTERFACE_TYPE_QINQ_PORT;
    } sai_router_interface_type_t;
```

- New router interface attribute:

```
    typedef enum _sai_router_interface_attr_t
    {
        /**
         * @brief Outer Vlan
         *
         * @type sai_uint16_t
         * @flags MANDATORY_ON_CREATE | CREATE_ONLY
         * @condition SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_QINQ_PORT;
         */
        SAI_ROUTER_INTERFACE_ATTR_OUTER_VLAN_ID

        /**
         * @brief Inner Vlan
         *
         * @type sai_uint16_t
         * @flags MANDATORY_ON_CREATE | CREATE_ONLY
         * @condition SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_QINQ_PORT;
         */
        SAI_ROUTER_INTERFACE_ATTR_INNER_VLAN_ID
    } sai_router_interface_attr_t;
```

- New tunnel map type between VNI to Virtual Router ID

```
    typedef enum _sai_tunnel_map_type_t
    {
        ...
        
        /** TUNNEL Map VNI to Virtual Router ID */
        SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID = 0x00000006,

        /** TUNNEL Map Virtual Router ID to VNI */
        SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI = 0x00000007,
    } sai_tunnel_map_type_t;
```

- New tunnel map entry for virtual router id

```
    typedef enum _sai_tunnel_map_entry_attr_t
    {
        /**
         * @brief Virtual Router ID key
         *
         * @type sai_object_id_t
         * @flags MANDATORY_ON_CREATE | CREATE_ONLY
         * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
         * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI
         */
        SAI_TUNNEL_MAP_ENTRY_ATTR_VIRTUAL_ROUTER_ID_KEY = 0x0000000a,

        /**
         * @brief Virtual Router ID value
         *
         * @type sai_object_id_t
         * @flags MANDATORY_ON_CREATE | CREATE_ONLY
         * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
         * @condition SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID or SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE == SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI
         */
        SAI_TUNNEL_MAP_ENTRY_ATTR_VIRTUAL_ROUTER_ID_VALUE = 0x0000000b,
    } sai_tunnel_map_entry_attr_t;
```
## Examples

### QinQ to Vxlan example

We have one customer server (100.100.3.1/24) with outer vlan 100 and inner vlan tag 1 want to 
talk to VM1 (100.100.1.1/32) and VM2 (100.100.2.1/32). VM1 is in host
100.100.1.1 and VM2 is in host 100.100.2.1. VM1 is in VXLAN id 2000 and 
VM2 is in VXLAN id 2001.

When we use riot VXLAN tunnel, the default inner destination mac in the 
tunnel encap is the ```SAI_SWITCH_ATTR_VXLAN_DEFAULT_ROUTER_MAC```.

To reach VM1 from the customer, the switch needs to encap the packet with 
outer IP 100.100.1.1 and VXLAN id 2000. To reach VM2, the switch needs to encap the packet with 
outer IP 100.100.2.1 with VXLAN id 2001. For VM2, it we also need to specify
the inner destination mac "00:12:34:56:78:9a".

To reach the customer from VM1 and VM2, the customer will send VXLAN packet
with VXLAN id 2000. The switch will decap the packet, using the VXLAN id 2000
to map to the currect virtual router and then lookup the inner destination IP
(100.100.3.1/24).


```
# Main program

sai_attribute_t switch_attr;
switch_attr.id = SAI_SWITCH_ATTR_VXLAN_DEFAULT_ROUTER_MAC;
switch_attr.value.mac = "00:11:11:11:11:11";
sai_switch_api->set_switch_attribute(switch_id, &switch_attr);

sai_attribute_t switch_attr;
switch_attr.id = SAI_SWITCH_ATTR_VXLAN_DEFAULT_PORT;
switch_attr.value.u16 = 12345;
sai_switch_api->set_switch_attribute(switch_id, &switch_attr);

create_virtual_router(&virtual_router_id);

# create qinq router interface
status = create_router_interface_qinq(virtual_router_id, 100, 1, &rif);

# create one tunnel for the virtual router
status = create_tunnel(virtual_router_id, 2000, &tunnel_id_1);

# create two tunnel nexthops on the virtual router for the two VMs
status = create_nexthop_tunnel(virtual_router_id, "10.10.10.1", 0, NULL, tunnel_id_1, &nexthop_id_1);
status = create_nexthop_tunnel(virtual_router_id, "20.20.20.1", 2001, "00:12:34:56:78:9a", tunnel_id_2, &nexthop_id_2);

# create two routes to the two VMs
status = create_route("100.100.1.1/32", virtual_router_id, nexthop_id_1);
status = create_route("100.100.2.1/32", virtual_router_id, nexthop_id_2);

# creat the route to the customer server
status = create_route("100.100.3.0/24", virtual_router_id, nexthop_id_3);

# create tunnel decap for VM to customer server
status = create_tunnel_termination(tunnel_id_1, "10.10.10.10", &term_table_id);

```

Create QinQ router interface

```
sai_status_t create_router_interface_qinq(
    sai_object_id_t overlay_router_id, 
    uint32_t out_vlan, 
    uint32_t in_vlan, 
    sai_object_id_t *router_intf)
{
    sai_status_t status;
    sai_attribute_t intf_attrs[4];

    intf_attrs[0].id = SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID;
    intf_attrs[0].value.oid = overlay_router_id;

    intf_attrs[1].id = SAI_ROUTER_INTERFACE_ATTR_TYPE;
    intf_attrs[1].value.s32 = SAI_ROUTER_INTERFACE_TYPE_QINQ_PORT;

    intf_attrs[2].id = SAI_ROUTER_INTERFACE_ATTR_OUTER_VLAN_ID;
    intf_attrs[2].value.u16 = outer_vlan;

    intf_attrs[3].id = SAI_ROUTER_INTERFACE_ATTR_INNER_VLAN_ID;
    intf_attrs[3].value.u16 = inner_vlan;

    status = sai_router_intfs_api->create_router_interface(router_intf, switch_id, 4, intf_attrs);

    return status;
}
```

Create encap/decap mapper

```
sai_object_id_t create_encap_tunnel_map()
{
    sai_status_t status;
    sai_attribute_t attr;
    std::vector<sai_attribute_t> tunnel_map_attrs;

    attr.id = SAI_TUNNEL_MAP_ATTR_TYPE;
    attr.value.s32 = SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI;
    tunnel_map_attrs.push_back(attr);

    sai_tunnel_api->create_tunnel_map(tunnel_encap_map_id, 1, tunnel_map_attrs);

    return tunnel_map_id;
}

sai_object_id_t create_encap_tunnel_map_entry(
    sai_object_id_t tunnel_map_id,
    sai_object_id_t router_id, 
    sai_uint32_t vni)
{
    sai_status_t status;
    sai_attribute_t attr;
    std::vector<sai_attribute_t> tunnel_map_entry_attrs;

    attr.id = SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE;
    attr.value.s32 = SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI;
    tunnel_map_entry_attrs.push_back(attr);

    attr.id = SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP;
    attr.value.oid = tunnel_map_id;
    tunnel_map_entry_attrs.push_back(attr);

    attr.id = SAI_TUNNEL_MAP_ENTRY_ATTR_VIRTUAL_ROUTER_ID_KEY;
    attr.value.oid = router_id;
    tunnel_map_entry_attrs.push_back(attr);

    attr.id = SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_VALUE;
    attr.value.oid = vni;
    tunnel_map_entry_attrs.push_back(attr);

    sai_tunnel_api->create_tunnel_map(tunnel_map_id, tunnel_map_entry_attrs.size(), tunnel_map_entry_attrs);

    return tunnel_map_entry_id;
}

sai_object_id_t create_decap_tunnel_map()
{
    sai_status_t status;
    sai_attribute_t attr;
    std::vector<sai_attribute_t> tunnel_map_attrs;

    attr.id = SAI_TUNNEL_MAP_ATTR_TYPE;
    attr.value.s32 = SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID;
    tunnel_map_attrs.push_back(attr);

    sai_tunnel_api->create_tunnel_map(tunnel_decap_map_id, 1, tunnel_map_attrs);

    return tunnel_map_id;
}
    
sai_object_id_t create_decap_tunnel_map_entry(
    sai_object_id_t tunnel_map_id,
    sai_object_id_t router_id, 
    sai_uint32_t vni)
{
    sai_status_t status;
    sai_attribute_t attr;
    std::vector<sai_attribute_t> tunnel_map_entry_attrs;

    attr.id = SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE;
    attr.value.s32 = SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID;
    tunnel_map_entry_attrs.push_back(attr);

    attr.id = SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP;
    attr.value.oid = tunnel_map_id;
    tunnel_map_entry_attrs.push_back(attr);

    attr.id = SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_KEY;
    attr.value.oid = router_id;
    tunnel_map_entry_attrs.push_back(attr);

    attr.id = SAI_TUNNEL_MAP_ENTRY_ATTR_VIRTUAL_ROUTER_VALUE;
    attr.value.oid = vni;
    tunnel_map_entry_attrs.push_back(attr);

    sai_tunnel_api->create_tunnel_map(tunnel_map_id, tunnel_map_entry_attrs.size(), tunnel_map_entry_attrs);

    return tunnel_map_entry_id;
}
```

Create tunnel

```
sai_status_t create_tunnel(
    sai_object_id_t overlay_id,
    sai_uint32_t vni, 
    sai_object_id_t *tunnel_id)
{
  sai_status_t status;
  sai_attribute_t attr;
  std::vector<sai_attribute_t> tunnel_attrs;

  attr.id = SAI_TUNNEL_ATTR_TYPE;
  attr.value.s32 = SAI_TUNNEL_TYPE_VXLAN;
  tunnel_attrs.push_back(attr);

  sai_tunnel_map_list_t encap_list;
  sai_tunnel_map_list_t decap_list;
  sai_tunnel_map_t encap_map;
  sai_tunnel_map_t decap_map;

  tunnel_encap_map_id = create_encap_tunnel_map();
  tunnel_decap_map_id = create_decap_tunnel_map();

  create_encap_tunnel_map_entry(tunnel_encap_map_id, router_id, vni);
  create_decap_tunnel_map_entry(tunnel_decap_map_id, vni, router_id);

  // encap ecn mode (copy from outer/standard)
  attr.id = SAI_TUNNEL_ATTR_ENCAP_ECN_MODE;
  attr.value.s32 = SAI_TUNNEL_ENCAP_ECN_MODE_USER_DEFINED;
  tunnel_attrs.push_back(attr);

  attr.id = SAI_TUNNEL_ATTR_ENCAP_MAPPERS;
  attr.value.tunnelmap = { tunnel_encap_map_id };
  tunnel_attrs.push_back(attr);

  attr.id = SAI_TUNNEL_ATTR_DECAP_ECN_MODE;
  attr.value.s32 = SAI_TUNNEL_DECAP_ECN_MODE_USER_DEFINED; //SAI_TUNNEL_DECAP_ECN_MODE_COPY_FROM_OUTER;
  tunnel_attrs.push_back(attr);
  attr.id = SAI_TUNNEL_ATTR_DECAP_MAPPERS;
  attr.value.tunnelmap = { tunnel_decap_map_id };
  tunnel_attrs.push_back(attr);

  // ttl mode (uniform/pipe)
  attr.id = SAI_TUNNEL_ATTR_DECAP_TTL_MODE;
  attr.value.s32 = SAI_TUNNEL_TTL_MODE_UNIFORM_MODEL;
  tunnel_attrs.push_back(attr);

  // dscp mode (uniform/pipe)
  attr.id = SAI_TUNNEL_ATTR_DECAP_DSCP_MODE;
  attr.value.s32 = SAI_TUNNEL_DSCP_MODE_UNIFORM_MODEL;
  tunnel_attrs.push_back(attr);

  status = sai_tunnel_api->create_tunnel(tunnel_id, switch_id, tunnel_attrs.size(), tunnel_attrs.data());

  return status;
}
```

Create nexthop for the tunnel interface

```
sai_status_t create_nexthop_tunnel(
    sai_object_id_t router_id, 
    sai_ip4_t host_ip, 
    sai_uint32_t vni, // optional vni
    sai_mac_t mac, // inner destination mac
    sai_object_id_t tunnel_id, 
    sai_object_id_t *next_hop_id)
{
  vector<sai_attribute_t> next_hop_attrs;

  sai_attribute_t next_hop_attr;

  next_hop_attr.id = SAI_NEXT_HOP_ATTR_TYPE;
  next_hop_attr.value.s32 = SAI_NEXT_HOP_TYPE_ENCAP;
  next_hop_attrs.push_back(next_hop_attr);

  next_hop_attr.id = SAI_NEXT_HOP_ATTR_IP;
  next_hop_attr.value.ip4 = host_ip;
  next_hop_attrs.push_back(next_hop_attr);

  next_hop_attr.id = SAI_NEXT_HOP_ATTR_TUNNEL_ID;
  next_hop_attr.value.oid = tunnel_id;
  next_hop_attrs.push_back(next_hop_attr);
  if (vni != 0)
  {
     next_hop_attr.id = SAI_NEXT_HOP_ATTR_TUNNEL_VNI;
     next_hop_attr.value.u32 = vni;
     next_hop_attrs.push_back(next_hop_attr);
  }

  if (mac != null)
  {
     next_hop_attr.id = SAI_NEXT_HOP_ATTR_TUNNEL_MAC;
     next_hop_attr.value.mac = mac;
     next_hop_attrs.push_back(next_hop_attr);
  }


  sai_status_t status = sai_next_hop_api->create_next_hop(next_hop_id, router_id, next_hop_attrs.size(), next_hop_attrs.data());
  return status;
}
```

Create route

```
sai_status_t create_route(
    sai_ip4_t ip, 
    sai_ip4_t mask, 
    sai_object_id_t vrf_id, 
    sai_object_id_t nexthop_id)
{
  sai_status_t status;
  sai_route_entry_t route_entry;
  route_entry.switch_id = switch_id;
  route_entry.vr_id = vrf_id;

  sai_ip_prefix_t destination;
  destination.addr.ip4 = ip;
  destination.mask.ip4 = mask;
  route_entry.destination = destination;

  sai_attribute_t attr;
  attr.id = SAI_ROUTE_ENTRY_ATTR_NEXT_HOP_ID;
  attr.value.oid = nexthop_id;

  status = sai_route_api->create_route(&route_entry, 1, &attr);
  return status;
}
```

### Vxlan to QinQ


Create tunnel termination

```
sai_status_t create_tunnel_termination(
    sai_object_id_t oid,  // tunnel oid
    sai_ip4_t dstip,      // tunnel dstip ip
    sai_object_id_t *term_table_id)
{
  sai_attribute_t attr;
  std::vector<sai_attribute_t> tunnel_attrs;

  attr.id = SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY;
  attr.value.oid = SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2MP;
  tunnel_attrs.push_back(attr);

  attr.id = SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_DST_IP;
  attr.value.ip4 = dstip;
  tunnel_attrs.push_back(attr);

  attr.id = SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TUNNEL_TYPE;
  attr.value.oid = SAI_TUNNEL_TYPE_VXLAN;
  tunnel_attrs.push_back(attr);

  attr.id = SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_ACTION_TUNNEL_ID;
  attr.value.oid = oid;
  tunnel_attrs.push_back(attr);

  sai_status_t status;
  status = sai_tunnel_api->create_tunnel_term_table_entry(term_table_id, switch_id, tunnel_attrs.size(), tunnel_attrs.data());

  return status;
}
```
