# Overview
Router Interface's Source MAC address is used in the ingress pipeline to match against a received packet's Destination MAC address (along with port and VLAN as applicable) to decide if the packet should be L3 processed or L2 processed.
 
# Proposal
This proposal provides for a dedicated way to program the MAC address used in the ingress pipeline.

A My-MAC entry is defined with port, VLAN and MAC address as match criteria.

# Usage

## Add My MAC entry
```
  sai_attribute_t attr;

  attr.id = SAI_MY_STATION_ATTR_MAC_ADDRESS;
  memcpy (attr.value.mac, my_mac, sizoef(sai_mac_t));

  status = sai_my_mac_api->create_my_mac(&my_mac_oid, switch_id,
                                         1, &attr);
```


