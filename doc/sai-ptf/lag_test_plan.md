# SAI Lag Test plan
- [SAI Lag Test plan](#sai-lag-test-plan)
  - [Overriew](#overriew)
    - [Test Topology](#test-topology)
    - [Testbed](#testbed)
  - [Scope](#scope)
- [Basic Configurations, SAI API and sample packets](#basic-configurations-sai-api-and-sample-packets)
  - [Basic Configurations And SAI API](#basic-configurations-and-sai-api)
    - [Basic Portchannel configuration](#basic-portchannel-configuration)
      - [Create lag and lag member Using SAI API](#create-lag-and-lag-member-using-sai-api)
    - [Basic Route Entry](#basic-route-entry)
      - [Create Router Entry Using SAI API](#create-router-entry-using-sai-api)
  - [Packets](#packets)
- [Test suites](#test-suites)
  - [Test suite #1: PortChannel Loadbalanceing](#test-suite-1-portchannel-loadbalanceing)
  - [Test suite #2: Ingress/Egreee disable](#test-suite-2-ingressegreee-disable)
  - [Test suite #3: Remove Lag member](#test-suite-3-remove-lag-member)
## Overriew
The purpose of this test plan is to test the LAG/PortChannel function from SAI.

### Test Topology
For SAI-PTF, it will use a non-topology network structure for the sai testing. 

### Testbed
Those tests will be run on the testbed structure, the components are:
* PTF - running in a server that can connect to the target DUT
* SAI server - running on a dut

*P.S. All the tests are target on T0 scenario.*

## Scope
The test will include two parts
1. Lag functionalities
   - Load balancing
2. Lag SAI APIs
   - create/check/remove lag and lag member


# Basic Configurations, SAI API and sample packets

## Basic Configurations And SAI API
### Basic Portchannel configuration
|PortChannel Name|Ports|
|-|-|
| lag3  |Port14-16|
#### Create lag and lag member Using SAI API
```Python
    +------------+------------+-----------+-----------+
    |    lag3    |     --     |  port14   |     --    |
    |  lag3_rif  |            |  port15   |           |
    |            |            |  port16   |           |
    +------------+------------+-----------+-----------+
    sai_thrift_create_lag(self.client)
    
    self.lag3_member14 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag3, port_id=self.port14)
     self.lag3_member15 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag3, port_id=self.port15)
     self.lag3_member16 = sai_thrift_create_lag_member(
            self.client, lag_id=self.lag3, port_id=self.port16)
           
      self.lag3_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag3)
      
```

### Basic Route Entry

|DestIp|Next Hop |Next Hop ip|Next Hop Mac|
|-|-|-|-|
|10.10.10.2|lag3|10.10.10.10|00:99:99:99:99:99|

#### Create Router Entry Using SAI API
```Python
        nhop = sai_thrift_create_next_hop(self.client,
                                          ip=sai_ipaddress('10.10.10.10'),
                                          router_interface_id=self.lag3_rif,
                                          type=SAI_NEXT_HOP_TYPE_IP)
        neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.lag3_rif, ip_address=sai_ipaddress('10.10.10.10'))
        sai_thrift_create_neighbor_entry(self.client, neighbor_entry,
                                         dst_mac_address='00:99:99:99:99:99')
        route1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.2/32'))
        sai_thrift_create_route_entry(self.client, route1, next_hop_id=nhop)

```
## Packets
```Python
   pkt = simple_tcp_packet(
                eth_dst=dev_port11_MAC,
                eth_src='00:22:22:22:22:22',
                ip_dst='10.10.10.2',
                ip_src=srcip) 
                
   exp_pkt = simple_tcp_packet(
                eth_dst='00:99:99:99:99:99',
                eth_src=dev_port11_MAC,
                ip_dst='10.10.10.2',
                ip_src=srcip)
```

# Test suites
## Test suite #1: PortChannel Loadbalanceing
For load balancing, expecting the ports in a lag should receive the packet equally.

Even after removing and disabling the port in a lag.

Sample APIS
Disbale egress
```Python
sai_thrift_set_lag_member_attribute(
                self.client,
                self.lag3_member14,
                ingress_disable=True,
                egress_disable=True)
```

|  Goal| Steps/Cases  | Expect  |
|-|-|-|
| Prepare to send from dev_port11 to Lag3.| Send packet with.| Lag3 and members have been created.|
| Packet forwards on port equally.| Send packet on dev_port11 to the lag3  4 times .| Loadbalance on lag members.|
| Packet forwards on available ports equally.| Every time, disable egress/ingress on one lag member, then send packet | Loadbalance on lag members.|
| Packet forwards on available ports equally.| Every time, enable egress/ingress on one lag member, then send packet | Loadbalance on lag members.|
| Packet forwards on available ports equally.| Every time, remove one lag member, then send packet | Loadbalance on lag members.|

## Test suite #2: Ingress/Egreee disable
For lag, we can disable it from ingress or egress direction, after we disable the member of a lag, we expect traffic can be loadbalanced to other lag members.

Sample APIs

Ingress/Egreee disable
```python
    status = sai_thrift_set_lag_member_attribute(
        self.client,
        self.lag3_member14,
        ingress_disable=True,
        egress_disable=True)

```

lag port list
```Python
sai_thrift_get_lag_attribute(
                self.client, self.lag3, port_list=portlist)
```

| Goal | Steps/Cases | Expect  |
|-|-|-|
|Packet dropped on port14| Disable egress and ingress on lag member14. send packet | Packet drop.|

## Test suite #3: Remove Lag member 
Test verifies the LAG load balancing for scenario when LAG members are removed.

Sample APIs

Remove Lag member
```python
     print("Remove LAG member 16")
     status = sai_thrift_remove_lag_member(self.client, self.lag3_member16)

```
How to check if each port of Lag receive an equal number of packets (if we have n members in a Lag), 
```python
 self.max_itrs =100
 for i in range(0, n):
                self.assertTrue((count[i] >= ((self.max_itrs / n) * 0.7)),

```
| Goal | Steps/Cases | Expect  |
|-|-|-|
|Remove port16 and forwarding packet from port1 to port14,15|Remove port16 form Lag3 and Send packet on dev_port11 to lag3 100 times| Port14 and port15 will receive an equal number of packets.|

