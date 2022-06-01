# Sample T0 Configurations and data for VLAN
- [Sample T0 Configurations and data for VLAN](#sample-t0-configurations-and-data-for-vlan)
  - [Overriew](#overriew)
  - [VLAN configuration](#vlan-configuration)
    - [VLAN and VLAN members](#vlan-and-vlan-members)
    - [VLAN Interfaces](#vlan-interfaces)
  - [APIs for VLAN configuration](#apis-for-vlan-configuration)
    - [Create vlan and its member](#create-vlan-and-its-member)
    - [Get VLAN, VLAN members, and attributes](#get-vlan-vlan-members-and-attributes)
    - [Update VLAN, VLAN members and attributes](#update-vlan-vlan-members-and-attributes)
    - [Delete VLAN and VLAN members](#delete-vlan-and-vlan-members)
  - [Sample data/packet](#sample-datapacket)
    - [Packet example](#packet-example)
  - [config data](#config-data)
## Overriew
This document describes the sample configuration data, sample test data/packet, and APIs to make the configuration that is used around VLAN testing.

**Note: This configuration focused on T0 topology.**

## VLAN configuration

### VLAN and VLAN members

|HostIf|VLAN ID|Ports|Tag mode|
|-|-|-|-|
|Ethernet4-32|1000|Port1-8|Untag|
|Ethernet36-72|2000|Port9-16|Untag|

### VLAN Interfaces
|VLAN ID | VLAN Interface IP | VLAN Interface MAC | 
|-|-|-|
|1000|192.168.10.1|10:00:01:11:11:11|
|2000|192.168.20.1|20:00:01:22:22:22|

## APIs for VLAN configuration
APIs relate to VLAN and VLAN attributes.

**P.S. There are just some sample APIs, for more attributes please refer to https://github.com/richardyu-ms/SAI/blob/support_ptf_sai_build/inc/saivlan.h**

### Create vlan and its member
- Create Vlan 1000
   ```Python
   self.vlan1000 = sai_thrift_create_vlan(self.client, vlan_id=1000)
   ```
- Create VLAN members with different modes
   Untag mode and access port
   ```python
    ----------------------------------
    |VLAN ID|Ports|Tag mode| HostIf  |
    |-------|-----|--------|---------|
    | 1000  |Port1| Untag  |Ethernet4|
    ----------------------------------

   self.vlan_member1 = sai_thrift_create_vlan_member(
                    self.client,
                    vlan_id=1000,
                    bridge_port_id=port_bridge_port1,
                    vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

   ```
### Get VLAN, VLAN members, and attributes
- Get VLAN member list
```Python
vlan_member_list = sai_thrift_object_list_t(count=1000)
mbr_list = sai_thrift_get_vlan_attribute(
            self.client, self.vlan10, member_list=vlan_member_list)
vlan_members = mbr_list['SAI_VLAN_ATTR_MEMBER_LIST'].idlist
#equals to vlan_members = mbr_list['member_list'].idlist
```
- Get VLAN 

```python
    vlan_attr = sai_thrift_get_vlan_attribute(
        self.client, vlan_oid=1000, learn_disable=True)
```
- Get counters
```Python
        stats = sai_thrift_get_vlan_stats(self.client, self.vlan10)
        in_bytes = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

```

### Update VLAN, VLAN members and attributes
- Set mac learning
  ```python
  sai_thrift_set_vlan_attribute(self.client, self.vlan20, learn_disable=True)
  ```
- Set Native vlan 1000 on a port
   ```python
   sai_thrift_set_port_attribute(self.client, port_id, port_vlan_id=1000)
   ```
- Clear counters
   ```Python
   sai_thrift_clear_vlan_stats(self.client, self.vlan10)
   ```

### Delete VLAN and VLAN members
- Remoev VLAN member
  ```python
   sai_thrift_remove_vlan_member(self.client, self.vlan_member1)
  ```
- Remoev VLAN
  ```python
   sai_thrift_remove_vlan(self.client, self.vlan1000)
  ```

## Sample data/packet

### Packet example
- Tagged packet with VLAN id 

   When a packet sent to a access port or send out from a access port, we will get a packet without the VLAN id.
   *In T0, we don't need to test tagged packet.*
   ```python
    tagged_packet(eth_dst='00:11:11:11:11:11',
                eth_src='00:22:22:22:22:22',
                vlan_vid=1000,
                ip_dst='172.16.0.1',
                ip_ttl=64)
   ```
- Untagged packet
  ```Python
    simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                      eth_src='00:22:22:22:22:22',
                      ip_dst='172.16.0.1',
                      ip_ttl=64)
  ```

## config data

Below is the sample config data in config_db.json

```JSON
   {
    "VLAN": {
        "Vlan1000": {            
            "members": [
                "Ethernet4",
                "Ethernet8",
                "Ethernet12",
                "Ethernet16",
                "Ethernet20",
                "Ethernet24",
                "Ethernet28",
                "Ethernet32"
            ],
            "vlanid": "1000"
        }
    },
    "VLAN_INTERFACE": {
        "Vlan1000": {},
        "Vlan1000|192.168.0.1/21": {},
        "Vlan1000|fc02:1000::1/64": {}
    },
    "VLAN_MEMBER": {
        "Vlan1000|Ethernet4": {
            "tagging_mode": "untagged"
        },
        "Vlan1000|Ethernet8": {
            "tagging_mode": "untagged"
        },
        "Vlan1000|Ethernet12": {
            "tagging_mode": "untagged"
        },
        "Vlan1000|Ethernet16": {
            "tagging_mode": "untagged"
        },
        "Vlan1000|Ethernet20": {
            "tagging_mode": "untagged"
        },
        "Vlan1000|Ethernet24": {
            "tagging_mode": "untagged"
        },
        "Vlan1000|Ethernet28": {
            "tagging_mode": "untagged"
        },
        "Vlan1000|Ethernet32": {
            "tagging_mode": "untagged"
        }
    }
   }
```