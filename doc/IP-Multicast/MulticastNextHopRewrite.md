# SAI attributes for disabling L3 rewrites

# for IP Multicast forwarding

```
Author:mholankar@
```
## Overview

This document discusses requirements and the SAI spec proposal for disabling rewriting fields
(SourceMAC,VLAN) as part of IPMC routing.

## Background

#### SAI pipeline for IPMC forwarding. After Multicast Replication, the egress
#### pipeline is very similar to Unicast Forwarding. The reference pipeline is
#### fromUnicastForwarding.

- Nexthop sets the egress RIF and NextHopIP
- Neighbor table lookup on NextHopIP to set packet’s destination MAC address
- Egress RIF lookup to set packet’s source MAC address, VLAN and port.

## Requirements

We require knobs for disabling rewrites to following fields as part of Multicast forwarding flows

```
● Src MAC disable
● Vlan rewrite disable
```

We have scenarios where we need knobs for disabling header field rewrites, for Multicast 
Replication.

Case1:For certain flows, the switch does MulticastReplication for a VLAN tagged packet. However,
we do not want the VLAN tag to be over-written during replication, we want the replication to retain
the VLAN tag. For such a case we would like to disable L3 VLAN rewrite.

Case2: For some scenarios, we may want to rewrite the SRC MAC. Having NextHop based knobs
for such rewrites would be very helpful.

In summary, with SDN based forwarding, Controller treats the L2 fields like any other header field
which can be controlled and requires them to be configured flexibility as part of MulticastReplication.

## Proposal

Since we require capability to disable the rewrites for certain Multicast flows and not for all flows
via/to neighbor, the best option is to have these as part of NextHop object:

- SAI_NEXT_HOP_ATTR_DISABLE_SRC_MAC_REWRITE
- SAI_NEXT_HOP_ATTR_DISABLE_DST_MAC_REWRITE
- SAI_NEXT_HOP_ATTR_DISABLE_VLAN_REWRITE

### Example SAI object creation for an IPMCGroup:

- Create a **SAI_OBJECT_TYPE_IPMC_GROUP** for the multicast group with the following
    attributes.
- Create a SAI_OBJECT_TYPE_ROUTER_INTERFACE with following attributes:
    ● SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID
    ● SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS
    ● SAI_ROUTER_INTERFACE_ATTR_TYPE=SAI_ROUTER_INTERFACE_TYPE_
       PORT
    ● SAI_ROUTER_INTERFACE_ATTR_PORT_ID
- Create a SAI_OBJECT_TYPE_NEIGHBOR_ENTRY for each neighbor with:
    ● “ip”=Link local address
    ● “rif”
    ● “switch id”
    ● SAI_NEIGHBOR_ENTRY_ATTR_DST_MAC_ADDRESS(optional)
    ● SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE=true
- Create SAI_OBJECT_TYPE_NEXT_HOP with:
    ● SAI_NEXT_HOP_ATTR_TYPE=SAI_NEXT_HOP_TYPE_IPMC
    ● SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID
    ● SAI_NEXT_HOP_ATTR_IP=“ip”of neighbor


```
● SAI_NEXT_HOP_ATTR_DISABLE_SRC_MAC_REWRITE=true
● SAI_NEXT_HOP_ATTR_DISABLE_VLAN_REWRITE=true
```
- Create a **SAI_OBJECT_TYPE_IPMC_GROUP_MEMBER** with following attributes:
    ● SAI_IPMC_GROUP_MEMBER_ATTR_IPMC_GROUP_ID with ipmc_group_oid
    ● SAI_IPMC_GROUP_MEMBER_ATTR_IPMC_OUTPUT_ID with rif_oid
    ● SAI_IPMC_GROUP_MEMBER_ATTR_IPMC_NEXT_HOP with next_hop_oid



