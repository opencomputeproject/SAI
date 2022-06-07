# Sample T0 Configurations and data for VLAN
- [Sample T0 Configurations and data for VLAN](#sample-t0-configurations-and-data-for-vlan)
  - [Overriew](#overriew)
  - [VLAN configuration](#vlan-configuration)
    - [VLAN and VLAN members](#vlan-and-vlan-members)
    - [VLAN Interfaces](#vlan-interfaces)
    - [Json config data](#json-config-data)
  - [Route Configuration](#route-configuration)
    - [rotue entries](#rotue-entries)
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
|Ethernet4-32|1000|Port1-8|Untag|
|Ethernet36-72|2000|Port9-16|Untag|

### VLAN Interfaces
|VLAN ID | VLAN Interface IP | VLAN Interface MAC | 
|-|-|-|
|1000|192.168.10.1|10:00:01:11:11:11|
|2000|192.168.20.1|20:00:01:22:22:22|

### Json config data

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


## Route Configuration

### rotue entries

VLAN interfaces route entries
|VLAN ID | VLAN Member | NH IP | NH MAC|
|-|-|-|-|
|1000| Ethernet4-32 | 192.168.10.11 ~ 192.168.10.18 | 10:00:11:11:11:11 - 10:00:88:88:88:88 |
|2000| Ethernet36-72 | 192.168.20.21 ~ 192.168.20.28 | 20:00:11:11:11:11 - 20:00:88:88:88:88 |

VLAN Neighbors
|Name|IP|dest_mac|
|-|-|-|
|vlan1000_nb1-nb8| 192.168.10.11 ~ 192.168.10.18 | 10:00:11:11:11:11 - 10:00:88:88:88:88|
|vlan2000_nb1-nb8|192.168.20.21 ~ 192.168.20.28 |20:00:11:11:11:11 - 20:00:88:88:88:88 |


LAG Route entry

|DestIp|Next Hop |Next Hop ip|
|-|-|-|
|192.168.0.11-192.168.0.18|lag1:port17-18|192.168.0.11-192.168.0.18|
|192.168.0.19|lag2:port19-20|192.168.0.19|
|192.168.0.20|lag3:port21-22|192.168.0.20|

LAG Neighbors

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
|mac17-mac31 |01:11:11:11:11:11 - 03:11:11:11:11:11|Port17-31||Ethernet68-Ethernet124|
