# Sample T0 Configurations and data  <!-- omit in toc --> 
- [Overriew](#overriew)
- [IP and MAC naming convention](#ip-and-mac-naming-convention)
  - [MAC](#mac)
  - [IP](#ip)
- [1. L2 Configurations](#1-l2-configurations)
  - [1.1 FDB Configuration](#11-fdb-configuration)
  - [1.2 VLAN configuration](#12-vlan-configuration)
- [2. L3 configuration](#2-l3-configuration)
  - [2.1 VLAN Interfaces](#21-vlan-interfaces)
  - [2.2 LAG configuration](#22-lag-configuration)
    - [2.2.1 LAG Hash Rule](#221-lag-hash-rule)
  - [2.3 Route Configuration](#23-route-configuration)
    - [2.3.1 VLAN interfaces route entries](#231-vlan-interfaces-route-entries)
    - [2.3.2 LAG Route entry](#232-lag-route-entry)
  - [2.4 Neighbor Configuration](#24-neighbor-configuration)
    - [2.4.1 VLAN Neighbors](#241-vlan-neighbors)
    - [2.4.2 LAG Neighbors](#242-lag-neighbors)
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
01:01:00:99:02:01~01:01:00:99:02:32
# 99: Server
# 02: EXTRA (Group ID)
```


## IP
For IP addresses, we will use different prefix for different role

Format: ROLE.NUM.GROUP_ID.SEQ

- ROLE_NUM
T0: 10.0.0.0
T1: 10.0.0.0
Server: 192.168.0.0

For example
```
# IP in 
# 2.4.1 VLAN Neighbors
#Group0 (For Vlan10)
192.168.1.1~ 192.168.1.8
#Group1 (ForVlan20)
192.168.2.1~ 192.168.2.8
```




# 1. L2 Configurations

## 1.1 FDB Configuration

The MAC Table for VLAN L2 forwarding as below
|Name|MAC|PORT|VLAN|HostIf|
|-|-|-|-|-|
|mac0|01:01:00:99:00:00|Port0||Ethernet0|
|mac1-8  |01:01:00:99:01:01 - 01:01:00:99:01:08|Port1-8|10|Ethernet4-Ethernet32|
|mac9-16 |01:01:00:99:02:09 - 01:01:00:99:02:16|Port9-16|20|Ethernet36-Ethernet64|

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
|VLAN ID | VLAN Interface IP| 
|-|-|
|10|192.168.1.100|
|20|192.168.2.100|

## 2.2 LAG configuration

|HostIf|LAG ID|Ports|
|-|-|-|
|Ethernet76-80|lag1|Port17-18|
|Ethernet84-88|lag2|Port19-20|

### 2.2.1 LAG Hash Rule
- Set hash algorithm as SAI_HASH_ALGORITHM_CRC
- Set switch hash attribute as below, which means switch computes hash using the five fields and seed(SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_SEED) as the hash configuration. 
```
SAI_NATIVE_HASH_FIELD_SRC_IP
SAI_NATIVE_HASH_FIELD_DST_IP
SAI_NATIVE_HASH_FIELD_IP_PROTOCOL
SAI_NATIVE_HASH_FIELD_L4_DST_PORT
SAI_NATIVE_HASH_FIELD_L4_SRC_PORT
```

## 2.3 Route Configuration

### 2.3.1 VLAN interfaces route entries
|VLAN ID | route IP | Type |
|-|-| - |
|10| 192.168.1.100/24 | Direct Connect|
|20| 192.168.2.100/24 | Direct Connect|
### 2.3.2 LAG Route entry

|LAG ID | route IP | Type | VALUE|
|-|-| - |-|
|1| 10.0.1.100/31 | Direct Connect||
|2| 10.0.2.100/31 | Direct Connect||
|1| 192.168.10.1-192.168.10.100| NH|lag1_nb|
|2| 192.168.11.1-192.168.11.100| NH|lag2_nb|

## 2.4 Neighbor Configuration
### 2.4.1 VLAN Neighbors
|Name|Port|IP|dest_mac|
|-|-|-|-|
|vlan10_nb1-nb8|Port1-8 |192.168.1.1 ~ 192.168.1.8  |01:01:00:99:01:01 - 01:01:00:99:01:08|
|vlan20_nb1-nb8|Port9-16|192.168.2.9 ~ 192.168.2.16 |01:01:00:99:02:09 - 01:01:00:99:02:16 |


### 2.4.2 LAG Neighbors

|Name|Port|IP|dest_mac|
|-|-|-|-|
|lag1_nb|lag1| 10.0.1.101 | 02:04:02:01:01:01|
|lag2_nb|lag2| 10.0.2.101 | 02:04:02:01:02:01|
