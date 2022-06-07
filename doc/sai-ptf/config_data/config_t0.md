# Sample T0 Configurations and data for VLAN
- [Sample T0 Configurations and data for VLAN](#sample-t0-configurations-and-data-for-vlan)
  - [Overriew](#overriew)
  - [VLAN configuration](#vlan-configuration)
    - [VLAN and VLAN members](#vlan-and-vlan-members)
    - [VLAN Interfaces](#vlan-interfaces)
  - [Route Configuration](#route-configuration)
    - [VLAN interfaces route entries](#vlan-interfaces-route-entries)
    - [VLAN Neighbors](#vlan-neighbors)
    - [LAG Route entry](#lag-route-entry)
    - [LAG Neighbors](#lag-neighbors)
  - [LAG configuration](#lag-configuration)
    - [LAG and LAG members](#lag-and-lag-members)
  - [FDB Configuration](#fdb-configuration)
    - [MAC Table](#mac-table)
## Overriew
This document describes the sample configuration data.

**Note: This configuration focused on T0 topology.**

## VLAN configuration

### VLAN and VLAN members

|HostIf|VLAN ID|Ports|Tag mode|
|-|-|-|-|
|Ethernet0||Port0||
|Ethernet4-32|1000|Port1-8|Untag|
|Ethernet36-72|2000|Port9-16|Untag|

### VLAN Interfaces
|VLAN ID | VLAN Interface IP | VLAN Interface MAC | 
|-|-|-|
|1000|192.168.10.1|10:00:01:11:11:11|
|2000|192.168.20.1|20:00:01:22:22:22|

## Route Configuration


### VLAN interfaces route entries
|VLAN ID | VLAN Member | NH IP | 
|-|-|-|
|1000| Ethernet4-32 | 192.168.10.11 ~ 192.168.10.18 | 
|2000| Ethernet36-72 | 192.168.20.21 ~ 192.168.20.28 | 

### VLAN Neighbors
|Name|IP|dest_mac|
|-|-|-|
|vlan1000_nb1-nb8| 192.168.10.11 ~ 192.168.10.18 | 10:00:11:11:11:11 - 10:00:88:88:88:88|
|vlan2000_nb1-nb8|192.168.20.21 ~ 192.168.20.28 |20:00:11:11:11:11 - 20:00:88:88:88:88 |


### LAG Route entry

|DestIp|Next Hop |Next Hop ip|
|-|-|-|
|192.168.0.11-192.168.0.18|lag1:port17-18|192.168.0.11-192.168.0.18|
|192.168.0.19|lag2:port19-20|192.168.0.19|
|192.168.0.20|lag3:port21-22|192.168.0.20|

### LAG Neighbors

|Name|IP|dest_mac|
|-|-|-|
|lag1_nb1-nb8| 192.168.0.11-192.168.0.18| 00:00:11:11:11:11-00:00:88:88:88:88|
|lag2_nb1|192.168.0.19|00:00:99:99:99:99|
|lag3_nb1|192.168.0.20|00:00:aa:aa:aa:aa|


## LAG configuration

### LAG and LAG members

|HostIf|LAG ID|Ports|
|-|-|-|
|Ethernet76-80|lag1|Port17-18|
|Ethernet84-88|lag2|Port19-20|
|Ethernet92-96|lag3|Port21-22|

## FDB Configuration
### MAC Table
The MAC Table for VLAN L2 forwarding as below
|Name|MAC|PORT|VLAN|HostIf|
|-|-|-|-|-|
|mac0|mac0-00:00:00:00:00:11|Port0||Ethernet0|
|mac1-8  |00:11:11:11:11:11 - 00:88:88:88:88:88|Port1-8|1000|Ethernet4-Ethernet32|
|mac9-16 |00:99:99:99:99:99 - 01:00:00:00:00:00|Port9-16|2000|Ethernet36-Ethernet64|
|mac17-mac31 |01:11:11:11:11:11 - 01:ff:ff:ff:ff:ff|Port17-31||Ethernet68-Ethernet124|
