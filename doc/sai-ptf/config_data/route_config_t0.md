# Sample T0 Configurations and data for Route
- [Sample T0 Configurations and data for Route](#sample-t0-configurations-and-data-for-route)
  - [Overriew](#overriew)
  - [Route Configuration](#route-configuration)
    - [rotue entries](#rotue-entries)
  - [APIs for Route related configuration](#apis-for-route-related-configuration)
    - [Create VLAN Interface and Route entry](#create-vlan-interface-and-route-entry)
    - [Create LAG  Router interface and Route entry](#create-lag-router-interface-and-route-entry)
  - [Sample data/packet](#sample-datapacket)
    - [Packet example](#packet-example)
## Overriew
This document describes the sample configuration data, sample test data/packet, and APIs to make the configuration that is used around Route testing.

It includes the related data and config for a route, including
- Next Hop
- Neighbor
- Route entry

**Note: This configuration focused on T0 topology.**

## Route Configuration

### rotue entries

VLAN interfaces route entries
|VLAN ID | VLAN Member | NH IP | NH MAC|
|-|-|-|-|
|1000| Ethernet4-32 | 192.168.10.11 ~ 192.168.10.18 | 10:00:11:11:11:11 - 10:00:88:88:88:88 |
|2000| Ethernet36-72 | 192.168.20.21 ~ 192.168.20.28 | 20:00:11:11:11:11 - 20:00:88:88:88:88 |

*For vlan interface related configuration please refer to [VLAN_Config](./VLAN_config.md)**

## APIs for Route related configuration

### Create VLAN Interface and Route entry

Neighbors

|Name|IP|dest_mac|
|-|-|-|
|nb1-8| 192.168.10.11 ~ 192.168.10.18 | 10:00:11:11:11:11 - 10:00:88:88:88:88 |
|nb9-16| 192.168.20.11 ~ 192.168.20.18 | 20:00:11:11:11:11 - 20:00:88:88:88:88 |
|nb23-32| 192.168.0.17 ~ 192.168.0.26 | 00:00:77:77:77:77 - 00:00:ff:ff:ff:ff |
|nbvlan1000_gw| 192.168.10.255 | FF:FF:FF:FF:FF:FF |
|nbvlan2000_gw| 192.168.20.255 | FF:FF:FF:FF:FF:FF |

Route entry

|DestIp|Next Hop |Next Hop ip|
|-|-|-|
| 192.168.10.11 ~ 192.168.10.18 |port1 - 8 | 192.168.10.11 - 192.168.10.18 |
| 192.168.20.11 ~ 192.168.20.18 | port9 -16 | 192.168.20.11 - 192.168.20.18 |
| 192.168.0.17 ~ 192.168.0.26 | port23-32 | 192.168.0.17 - 192.168.0.26 |
|192.168.10.1|vlanInterface(VLAN1000)|CPU_PORT|
|192.168.20.1|vlanInterface(VLAN2000)|CPU_PORT|

- Create a Router for a VLAN interface
  ```Python
  
  self.vlan100_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan1000)
   self.rifnh = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress('192.168.10.1'),
            router_interface_id=rif, type=CPU_PORT)
   ```

- Create VLAN Interface GW
  ```python
  self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, 
            ip_address=sai_ipaddress('192.168.20.255'),
            dst_mac_address='FF:FF:FF:FF:FF:FF')
  ```

- Create Port Neighbor and next hop ( Port9 -> DestIP )
  ```Python
   rif = sai_thrift_create_router_interface(
                self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
                port_id=self.port9, virtual_router_id=self.vr)
   rifnh = sai_thrift_create_next_hop(
                self.client, ip=sai_ipaddress('192.168.20.2'),
                router_interface_id=rif, type=SAI_NEXT_HOP_TYPE_IP)
   self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.rif, 
            ip_address=sai_ipaddress('192.168.20.2'),
            dst_mac_address='20:00:11:11:11:11')
   self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, 
            destination=sai_ipprefix('192.168.20.2'+'/32')) # Packet dest IP
   sai_thrift_create_route_entry(
            self.client, self.route_entry1, next_hop_id=self.nhop1)
  ```
### Create LAG  Router interface and Route entry

Neighbors

|Name|IP|dest_mac|
|-|-|-|
|lag1_nb| 192.168.0.11 | 00:00:11:11:11:11 |
|lag2_nb| 1 192.168.0.12|00:00:22:22:22:22 |
|lag3_nb| 192.168.0.13| 00:00:33:33:33:33 |
Route entry

|DestIp|Next Hop |Next Hop ip|
|-|-|-|
| 192.168.0.11  |lag1:port17-18|192.168.0.11  |
|192.168.0.12 |lag2: port19 -20 | 192.168.0.12 |
| 192.168.0.13  | lag3:port21-22 | 192.168.0.13 |

```Python
        nhop = sai_thrift_create_next_hop(self.client,
                                          ip=sai_ipaddress('192.168.0.11'),
                                          router_interface_id=self.lag1_rif,
                                          type=SAI_NEXT_HOP_TYPE_IP)
        neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.lag1_rif, ip_address=sai_ipaddress('192.168.0.11'))
        sai_thrift_create_neighbor_entry(self.client, neighbor_entry,
                                         dst_mac_address='00:11:11:11:11:11')
        route1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('192.168.0.11/32'))
        sai_thrift_create_route_entry(self.client, route1, next_hop_id=nhop)


## Sample data/packet

### Packet example
When routing a packet, it will check the neighbor/adjacent table, and based on the Next-hop table, change the SRC MAC and Dest MAC.

- Input Packet
  ```Python
  simple_tcp_packet(
            eth_dst=SW_MAC,
            eth_src=SRC_MAC,
            ip_dst=DEST_IP,
            ip_src=SRC_IP)
  ```

- Output Packet
  ```Python
  simple_tcp_packet(
            eth_dst=PORT_MAC, 
            eth_src=SW_MAC,
            ip_dst=DEST_IP,
            ip_src=SRC_IP)
  ```
