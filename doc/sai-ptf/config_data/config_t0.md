# Sample T0 Configurations and data  <!-- omit in toc --> 
- [Overriew](#overriew)
- [IP and MAC naming convention](#ip-and-mac-naming-convention)
  - [MAC](#mac)
  - [IP v4](#ip-v4)
  - [IP v6](#ip-v6)
- [1. L2 Configurations](#1-l2-configurations)
  - [1.1 FDB Configuration](#11-fdb-configuration)
  - [1.2 VLAN configuration](#12-vlan-configuration)
- [2. L3 configuration](#2-l3-configuration)
  - [2.1 VLAN Interfaces](#21-vlan-interfaces)
  - [2.2 Route Interfaces](#22-route-interfaces)
  - [2.3 Route Configuration](#23-route-configuration)
  - [2.4 Neighbor Configuration](#24-neighbor-configuration)
  - [2.5 Default route entry and interface](#25-default-route-entry-and-interface)
- [3 LAG configuration](#3-lag-configuration)
  - [3.1 LAG Hash Rule](#31-lag-hash-rule)
- [4. Tunnel Configuration](#4-tunnel-configuration)
- [5. Tunnel QoS remapping (pcbb)](#5-tunnel-qos-remapping-pcbb)
  - [Port TC MAP](#port-tc-map)
  - [Tunnel TC MAP](#tunnel-tc-map)
  - [PCBB DSCP Config](#pcbb-dscp-config)
  - [PCBB IP_in_IP Tunnel Config](#pcbb-ip_in_ip-tunnel-config)
- [6. Buffer](#6-buffer)
- [7. QoS](#7-qos)
  - [Lossless Queue and Priority](#lossless-queue-and-priority)
# Overriew
This document describes the sample configuration data.

**Note: This configuration focused on T0 topology.**

# IP and MAC naming convention
In this configuration, we mapped the IP and MAC address into different parts of this configuration as below.

## MAC
For MAC addresses, we can use different sections in the MAC addresses to map different title numbers.
The pattern is
```
L1_NUM:L2_NUM:L3_NUM:ROLE:EXTRA:SEQ
ROLE: T1=1, Server=99
```

For example:
For the MAC address in ``1.1 FDB Configuration``.
```
#Server MAC
00:01:01:99:02:01~00:01:01:99:02:32
# 99: Server
# 02: EXTRA (Group ID)
```


## IP v4
For IP addresses, we will use different prefix for different role

Format: ROLE.NUM.GROUP_ID.SEQ

- ROLE_NUM
   ```
   T0: 10.0.0.0
   T0 ECMP: 10.0.50.0
   T1: 10.1.0.0
   T1 ECMP: 10.1.50.0
   Server: 192.168.0.0
   Server ECMP: 192.168.50.0
   Server Remote:192.168.10.0
   ```

For example
```
# IP in 
# 2.4.1 VLAN Neighbors
#Group0 (For Vlan10)
192.168.1.1~ 192.168.1.8
#Group1 (ForVlan20)
192.168.2.1~ 192.168.2.8
```

## IP v6
For IP addresses, we will use different prefix for different role

Format: ROLE.NUM.GROUP_ID.SEQ

- ROLE_NUM
   ```
   T0: fc00:0::
   T0 ECMP: fc00:0:50::
   T1: fc00:1::
   T1 ECMP: fc00:1:50::
   Server: fc02::
   Server ECMP: fc02:50::
   Server Remote:fc02:10::
   ```



# 1. L2 Configurations

## 1.1 FDB Configuration

The MAC Table for VLAN L2 forwarding as below
|Name|MAC|PORT|VLAN|HostIf|
|-|-|-|-|-|
|mac0|00:01:01:99:00:00|Port0||Ethernet0|
|mac1-8  |00:01:01:99:01:01 - 00:01:01:99:01:08|Port1-8|10|Ethernet4-Ethernet32|
|mac1-8  |00:01:01:99:01:91 - 00:01:01:99:01:98|Port1-8|10|Ethernet4-Ethernet32|
|mac9-16 |00:01:01:99:02:09 - 00:01:01:99:02:16|Port9-16|20|Ethernet36-Ethernet64|
|mac9-16 |00:01:01:99:02:91 - 00:01:01:99:02:98|Port9-16|20|Ethernet36-Ethernet64|

## 1.2 VLAN configuration

|HostIf|VLAN ID|Ports|Tag mode|
|-|-|-|-|
|Ethernet4-32|10|Port1-8|Untag|
|Ethernet36-72|20|Port9-16|Untag|


# 2. L3 configuration

Host interface IP
|Port|Interface IP| 
|-|-|
|port0|10.0.0.100|

## 2.1 VLAN Interfaces
|VLAN ID | VLAN Interface IP v4| VLAN Interface IP v6
|-|-|-|
|10|192.168.1.100|fc02::1:100|
|20|192.168.2.100|fc02::2:100|

## 2.2 Route Interfaces
|Port|Type|
|-|-|
|port0|port|
|port1-16|port|
|Lag1-4|Lag|
|VLAN10|VLAN|
|VLAN20|VLAN|



## 2.3 Route Configuration

|Dest IPv4|Dest IPv6| Next Hop/Group | Next hop IPv4 | Next hop IPv6 | next hop port|
|-|-|-|-|-|-|
|192.168.1.0/24|fc02::1::/112|Next Hop|192.168.1.0/24|fc02::1::|VLAN10|
|192.168.2.0/24|fc02::2::/112|Next Hop|192.168.2.0/24|fc02::2::|VLAN20|
|192.168.11.0/24|fc02::11:0/112|Next Hop|10.1.1.100|fc02::1:100|LAG1|
|192.168.12.0/24|fc02::12:0/112|Next Hop|10.1.2.100|fc02::2:100|LAG2|
|192.168.13.0/24|fc02::13:0/112|Next Hop|10.1.3.100|fc02::3:100|LAG3|
|192.168.14.0/24|fc02::14:0/112|Next Hop|10.1.4.100|fc02::4:100|LAG4|
|192.168.60.0/24|fc02::60:0/112|Next Hop Group|10.1.1.100; 10.1.2.100; 10.1.3.100; 10.1.4.100|fc00:1::1:100; fc00:1::2:100; fc00:1::3:100; fc00:1:4:100|LAG1-4|
|192.168.20.0/24|fc02::20:0/112|Next Hop|10.1.2.100|fc00:1::2:100|Tunnel|
|192.168.30.0/24|fc02::30:0/112|Next Hop|10.1.3.100|fc00:1::3:100|Tunnel|
|192.168.40.0/24|fc02::40:0/112|Next Hop|10.1.4.100|fc00:1::4:100|Tunnel|
|192.168.70.0/24|fc02::70:0/112|Next Hop Group|10.1.2.100;10.1.4.100|fc00:1::2:100; fc00:1::4:100|Tunnel|

## 2.4 Neighbor Configuration

|IPv4|IPv6|Port|No_host_route|dest_mac|
|-|-|-|-|-|
|192.168.1.255||SVI:VLAN10|No|ff:ff:ff:ff:ff:ff|
|192.168.2.255||SVI:VLAN20|No|ff:ff:ff:ff:ff:ff|
|10.1.1.100|fc00:1::1:100|LAG:lag1|No|00:01:01:01:01:a0|
|10.1.2.100|fc00:1::2:100|LAG:lag2|No|00:01:01:01:02:a0|
|10.1.3.100|fc00:1::3:100|LAG:lag3|Yes|00:01:01:01:03:a0|
|10.1.4.100|fc00:1::4:100|LAG:lag4|Yes|00:01:01:01:04:a0|
|192.168.1.1 ~ 192.168.1.8 |fc02::1:1 - fc02::1:8|Port1-8 | Yes|00:01:01:99:01:01 - 00:01:01:99:01:08|
|192.168.2.9 ~ 192.168.2.16| fc02::2:9 - fc02::2:16|Port9-16| Yes|00:01:01:99:02:09 - 00:01:01:99:02:16|
|192.168.1.91 ~ 192.168.1.98 |fc02::1:91 - fc02::1:98|VLAN10 | Yes|00:01:01:99:01:91 - 00:01:01:99:01:98|
|192.168.2.91 ~ 192.168.2.98 |fc02::2:91 - fc02::2:98|VLAN20 | Yes|00:01:01:99:02:91 - 00:01:01:99:02:98|

## 2.5 Default route entry and interface

Default Route Interface
|Virtual Router|interface type|
|-|-|
|default_virtual_router|LOOPBACK|

Default route
|Virtual Router|IPv4|IPv6|Action|
|-|-|-|-|
|default_virtual_router|0.0.0.0/0|::/0|Drop|


# 3 LAG configuration

|HostIf|LAG ID|Ports|
|-|-|-|
|Ethernet76-80|lag1|Port17-18|
|Ethernet84-88|lag2|Port19-20|
|Ethernet84-88|lag3|Port21-22|
|Ethernet92|lag4|Port23|
## 3.1 LAG Hash Rule
- Set hash algorithm as SAI_HASH_ALGORITHM_CRC
- Set switch hash attribute as below, which means switch computes hash using the five fields and seed(SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_SEED) as the hash configuration. 
```
SAI_NATIVE_HASH_FIELD_SRC_IP
SAI_NATIVE_HASH_FIELD_DST_IP
SAI_NATIVE_HASH_FIELD_IP_PROTOCOL
SAI_NATIVE_HASH_FIELD_L4_DST_PORT
SAI_NATIVE_HASH_FIELD_L4_SRC_PORT
```

# 4. Tunnel Configuration
- Config t0 loopback:
 
     |Name|IP|
     |-|-|
     |Router_lpb_ip1_v4| 10.10.10.1|
     |Router_lpb_ip2_v4| 10.10.10.2|
     |Router_lpb_ip1_v6| 4001:0E98:03EE::0D25|
     |Router_lpb_ip2_v6| 4001:0E98:03EE::0D26|


- IP IN IP Tunnel:
  1. Create ipinip tunnel with these attributes below,
     |Attribute Name|Value|
     |-|-|
     |encap_ttl_mode|SAI_TUNNEL_TTL_MODE_PIPE_MODEL| 
     |encap_ttl_val|50| 
     |decap_ttl_mode|SAI_TUNNEL_TTL_MODE_PIPE_MODEL| 
     |encap_dscp_mode|SAI_TUNNEL_DSCP_MODE_PIPE_MODEL| 
     |encap_dscp_vale|10| 
     |decap_dscp_mode|SAI_TUNNEL_DSCP_MODE_PIPE_MODEL| 

  2. Create tunnel type nexhop 
     |type|IP|MAC|
     |-|-|-|
     |SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP|10.1.2.100| 02:02:02:01:02:01|
     |SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP|10.1.4.100| 04:04:04:01:04:01|

  3. Create tunnel term table entry with attribute 
     |term type|dst_ip|src_ip|
     |-|-|-|
     |SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2MP|10.10.10.1| 10.1.2.100|


- Vxlan Tunnel:
  1. Create overlay loopback interface, underlay loopback interface, pass them when creating tunnel.
  2. Create tunnel with these attributes below,
     |property name|value|
     |-|-|
     |encap_ttl_mode|SAI_TUNNEL_TTL_MODE_PIPE_MODEL| 
     |encap_ttl_val|ttl_val| 
     |decap_ttl_mode|SAI_TUNNEL_TTL_MODE_PIPE_MODEL| 
     |encap_dscp_mode|SAI_TUNNEL_DSCP_MODE_PIPE_MODEL| 
     |encap_dscp_vale|tunnnel_dscp_val| 
     |decap_dscp_mode|SAI_TUNNEL_DSCP_MODE_PIPE_MODEL| 
  3. Create encap/decap mapper entry
     |type|vni|virtual router|
     |-|-|-|
     |SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID|1000| default vr id|
     |SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI|default vr id| 1000|

  4. Create tunnel term table entry with attribute 
     |term type|dst_ip|src_ip|
     |-|-|-|
     |SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2MP|10.10.10.2| 10.1.3.100|

  5. Create tunnel type nexhop
     |type|IP|MAC|
     |-|-|-|
     |SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP|10.1.3.100| 03:03:03:01:03:01|

# 5. Tunnel QoS remapping (pcbb)
## Port TC MAP 
|TC Value|DSCP Value|PRIORITY_GROUP Value|QUEUE Value|DSCP (Source)|
|-|-|-|-|-|
|0||0|0|8|
|1||0|1|0|
|2||2|2|2|
|3||3|3|3|
|4||4|4|4|
|5||0|5|46|
|6||6|6|6|
|7||7|7|48|
|8||0|1|33|

**p.s. For DSCP (Source), there should be a DSCP to TC map for them.**

## Tunnel TC MAP 
|TC Value|DSCP Value|PRIORITY_GROUP Value|QUEUE Value|DSCP (Source)|
|-|-|-|-|-|
|0|8|0|0|8|
|1|0|0|1|0|
|2|0|0|1|1|
|3|2|2|2|3|
|4|6|6|6|4|
|5|46|0|5|46|
|6|0|0|1||
|7|48|0|7|48|
|8|33|0|1|33|

**p.s. For DSCP (Source), there should be a DSCP to TC map for them.**

## PCBB DSCP Config
|Port|MAP GROUP|MAPs|
|-|-|-|
|Port1-8|PORT|DSCP_TO_TC_MAP; TC_TO_QUEUE; TC_TO_PRIORITY_GROUP|
|Tunnel_IP_IP|TUNNEL|DSCP_TO_TC_MAP; TC_TO_PRIORITY_GROUP_MAP; TC_TO_QUEUE_MAP; TC_TO_DSCP_MAP(TC_AND_COLOR_TO_DSCP_MAP)|

**For port map, please refer the table [Port TC MAP], for tunnel map, please refer the table [Tunnel TC MAP].**

## PCBB IP_in_IP Tunnel Config
|Tunnel|ECN MODE|DSCP MODE|
|-|-|-|
|IP_IN_IP|SAI_TUNNEL_ATTR_DECAP_ECN_MODE=SAI_TUNNEL_DECAP_ECN_MODE_COPY_FROM_OUTER; SAI_TUNNEL_ATTR_ENCAP_ECN_MODE=SAI_TUNNEL_ENCAP_ECN_MODE_STANDARD|SAI_TUNNEL_ATTR_DECAP_DSCP_MODE=SAI_TUNNEL_DSCP_MODE_PIPE_MODEL; SAI_TUNNEL_ATTR_ENCAP_DSCP_MODE=SAI_TUNNEL_DSCP_MODE_PIPE_MODEL;|

# 6. Buffer
**For the buffer configurations, they are different from different platform, please get the data from the config_db.json**

The SAI objects need to config includes:
- BUFFER_POOL: THRESHOLD_MODE, SIZE, TYPE
- BUFFER_PROFILE: POOL, RESERVED_BUFFER_SIZE, THRESHOLD_MODE, SHARED_DYNAMIC_TH, XOFF_TH, XON_TH, XON_OFFSET_TH
- QUEUE(BUFFER_QUEUE)
- INGRESS_PRIORITY_GROUP(BUFFER_PG)

# 7. QoS
## Lossless Queue and Priority
|Port|Queue Number| Priority Number| Attribute|
|-|-|-|-|
|0-32|3,4|3,4|PRIORITY_FLOW_CONTROL / SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL_TX|

**The property PRIORITY_FLOW_CONTROL or SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL_TX depend on port's property pfc_asym.**