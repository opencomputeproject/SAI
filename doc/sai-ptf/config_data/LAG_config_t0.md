# Sample T0 Configurations and data for LAG
- [Sample T0 Configurations and data for LAG](#sample-t0-configurations-and-data-for-lag)
  - [Overriew](#overriew)
  - [LAG configuration](#lag-configuration)
    - [LAG and lag members](#lag-and-lag-members)
  - [APIs for LAG configuration](#apis-for-lag-configuration)
    - [Create lag and its member](#create-lag-and-its-member)
    - [Check LoadBalance](#check-loadBalance)
    - [Remove Lag member](#remove-lag-member)
    - [Ingress Egreee disable](#ingress-egreee-disable)
  - [Sample data/packet](#sample-datapacket)
    - [Packet example](#packet-example)
  - [config data](#config-data)
## Overriew
This document describes the sample configuration data, sample test data/packet, and APIs to make the configuration that is used around VLAN testing.

**Note: This configuration focused on T0 topology.**

## LAG configuration

### LAG and LAG members

|HostIf|VLAN ID|Ports|
|-|-|-|
|Ethernet76-80|lag1|Port17-18|
|Ethernet84-88|lag2|Port19-20|
|Ethernet92-96|lag3|Port21-22|

## APIs for LAG configuration
APIs relate to LAG and LAG attributes.

### Create lag and its member
```Python
    +------------+------------+-----------+-----------+
    |    lag1    |     --     |  port1   |     --    |
    |  lag1_rif  |            |  port2   |           |
    |            |            |           |           |
    +------------+------------+-----------+-----------+
    sai_thrift_create_lag(self.client)
    
    self.lag1_member1 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag1, port_id=self.port17)
     self.lag1_membe2 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag1, port_id=self.port18)
  
           
      self.lag1_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag1)
      
```
### Check LoadBalance

```python

 #How to check if each port of Lag receive an equal number of packets (if we have n members in a Lag)
 self.packet_numbers =100
 for i in range(0, n):
                self.assertTrue((count[i] >= ((self.packet_numbers / n) * 0.7)),
 

```
### Remove Lag member

```python
     print("Remove LAG2 member 2")
     status = sai_thrift_remove_lag_member(self.client, self.lag2_member2)
```

### Ingress Egreee disable
```python
    status = sai_thrift_set_lag_member_attribute(
        self.client,
        self.lag3_member2,
        ingress_disable=True,
        egress_disable=True)

```

## Sample data/packet

### Packet example
- Input packet 
```Python
   pkt = simple_tcp_packet(
                eth_dst=ROUTER_MAC,
                eth_src=srcmac,
                ip_dst=dstip,
                ip_src=srcip) 
                

```

- Output packet
  ```Python
   exp_pkt = simple_tcp_packet(
                eth_dst=dstmac,
                eth_src=ROUTER_MAC,
                ip_dst=dstip,
                ip_src=srcip)
  ```

## config data

Below is the sample config data in config_db.json

```JSON
    "PORTCHANNEL": {
    "PortChannel1": {
        "admin_status": "up",
        "members": [
            "Ethernet76",
            "Ethernet80"
        ],
        "min_links": "1",
        "mtu": "9100"
    },
    "PortChannel2": {
        "admin_status": "up",
        "members": [
             "Ethernet84",
             "Ethernet88"
        ],
        "min_links": "1",
        "mtu": "9100"
    },
    "PortChannel3": {
        "admin_status": "up",
        "members": [
             "Ethernet92",
             "Ethernet96"
        ],
        "min_links": "1",
        "mtu": "9100"
    },
},
"PORTCHANNEL_INTERFACE": {
    "PortChannel1": {},
    "PortChannel1|10.10.192.19/31": {},
    "PortChannel1|2603:10b0:31f:278::2a/126": {},
    "PortChannel2": {},
    "PortChannel2|10.10.193.19/31": {},
    "PortChannel2|2603:10b0:31f:279::2a/126": {},
    "PortChannel3": {},
    "PortChannel3|10.10.194.19/31": {},
    "PortChannel3|2603:10b0:31f:27a::2a/126": {},
},
"PORTCHANNEL_MEMBER": {
    "PortChannel1|Ethernet76": {},
    "PortChannel1|Ethernet80": {},
    "PortChannel2|Ethernet84": {},
    "PortChannel2|Ethernet88": {},
    "PortChannel3|Ethernet92": {},
    "PortChannel3|Ethernet96": {},
},
```
