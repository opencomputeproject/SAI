# Overview
Router Interface's Source MAC address is used in the ingress pipeline to match against a received packet's Destination MAC address (along with port and VLAN as applicable) to decide if the packet should be L3 processed or L2 processed.
 
# Proposal
This proposal provides for a dedicated way to program the MAC address used in the ingress pipeline.

A My-MAC entry is defined with port, VLAN and MAC address as match criteria. A My-MAC table is defined as a container for these entries. On most architectures there is only one such instance of the table and it is essentially a read only attribute.

# Usage

## Fetch My MAC table OID
```
    sai_attribute_t attr;
    attr.id = SAI_SWITCH_ATTR_MY_STATION_TABLE_ID;
    status = sai_switch_api->get_switch_attribute(1, &attr);
    table_oid = attr.value.oid;
```
## Add My MAC entry
```
  sai_attribute_t attr[3];
  attr[0].id = SAI_MY_STATION_ATTR_TABLE_ID;
  attr[0].value.oid = table_oid;

  attr[1].id = SAI_MY_STATION_ATTR_MAC_ADDRESS;
  memcpy (attr[1].value.mac, my_mac, sizoef(sai_mac_t));

  attr[2].id = SAI_MY_MAC_ATTR_FLAG_TYPE;
  attr[2].value.s32 = SAI_MY_MAC_FLAG_TYPE_ALL;

  status = sai_my_mac_api->create_my_mac(&my_mac_oid, switch_id,
                                         3, attr);
```


