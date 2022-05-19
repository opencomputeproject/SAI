# SAI Lag Test plan
- [SAI Lag Test plan](#sai-lag-test-plan)
  - [Overriew](#overriew)
    - [Test Topology](#test-topology)
    - [Testbed](#testbed)
  - [Scope](#scope)
- [Basic SAI APIs and sample packets](#basic-sai-apis-and-sample-packets)
  - [APIs](#apis)
  - [Packets](#packets)
- [Test suites](#test-suites)
  - [Test suite #1: PortChannel Loadbalanceing](#test-suite-1-portchannel-loadbalanceing)
  - [Test suite #2: Ingress/Egreee disable](#test-suite-2-ingressegreee-disable)
## Overriew
The purpose of this test plan is to test the VLAN function from SAI.

### Test Topology
For SAI-PTF, it will use a non-topology network structure for the sai testing. 

### Testbed
Those tests will be run on the testbed structure, the components are:
* PTF - running in a server that can connect to the target DUT
* SAI server - running on a dut

*p.s. cause the SAI testing will not depend on any sonic components, then there will be no specific topology(T0 T1 T2) for testing.*

## Scope
The test will include two parts
1. Lag functionalities
   - Loadbalance
2. Lag SAI APIs
   - create/check/remove lag and lag member


# Basic SAI APIs and sample packets

## APIs

Create and lag member
```Python
sai_thrift_create_lag(self.client)
sai_thrift_create_lag_member(
            self.client, lag_id=lag3, port_id=self.port24)
```

Get lag members
```Python
sai_thrift_get_lag_attribute(
            self.client, lag3, port_list=portlist)
count = attr_list["SAI_LAG_ATTR_PORT_LIST"].count
```

Get port counter
```Python
counter_results = sai_thrift_get_port_stats(client, port)

counter_results["SAI_PORT_STAT_IF_IN_DISCARDS"],
counter_results["SAI_PORT_STAT_IF_IN_DISCARDS"],
counter_results["SAI_PORT_STAT_IF_IN_UCAST_PKTS"],
counter_results["SAI_PORT_STAT_IF_OUT_UCAST_PKTS"]))
```

Add fdb entry
**P.s.For fordwarding packet with vlan tag, fdb entry should enable the bv_id**
```python
sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac1, bv_id=self.vlan_oid)
sai_thrift_create_fdb_entry(
            self.client,
            fdb_entry,
            type=SAI_FDB_ENTRY_TYPE_STATIC,
            bridge_port_id=port_bp,
            packet_action=SAI_PACKET_ACTION_FORWARD)
```

Remove lag member
```Python
sai_thrift_remove_lag_member(self.client,  self.lag1_member6)
```

## Packets
Vlan tagged packet
```Python
simple_udp_packet(eth_dst=dst_mac,
                    eth_src=src_mac,
                    dl_vlan_enable=True,
                    vlan_vid=vlan_id,
                    pktlen=104)
```

# Test suites
## Test suite #1: PortChannel Loadbalanceing
For loadbalance, expecting the ports in a lag should received the packet equaly.

Even after remove and disable the port in a lag.

Sample APIS
Disbale egress
```Python
sai_thrift_set_lag_member_attribute(
                self.client,
                self.lag1_member4,
                ingress_disable=True,
                egress_disable=True)
```

| Steps/Cases | Goal | Expect  |
|-|-|-|
| Create lag, add lag members, port1 - 4. Add fdb entry for lag, map with a MAC. | Create lag and member| lag and member created|
| Send packet with.| Prepare to send from lag to vlan.| Vlan and member created.|
| Send packet on port0 to the lag by specify lag mac as dest mac. 4 times .| Packet forwards on port equally.| Loadbalance on lag members.|
| Every time, disable egress/ingress on one lag member, then send packet | Packet forwards on available ports equally.| Loadbalance on lag members.|
| Every time, enable egress/ingress on one lag member, then send packet | Packet forwards on available ports equally.| Loadbalance on lag members.|
| Every time, remove one lag member, then send packet | Packet forwards on available ports equally.| Loadbalance on lag members.|

## Test suite #2: Ingress/Egreee disable
Sample APIs

Ingress/Egreee disable
```python
    status = sai_thrift_set_lag_member_attribute(
        self.client,
        self.lag1_member4,
        ingress_disable=True,
        egress_disable=True)

```

lag port list
```Python
sai_thrift_get_lag_attribute(
                self.client, self.lag1, port_list=portlist)
```

| Steps/Cases | Goal | Expect  |
|-|-|-|
| Add FDB entry for port4 map to a MAC. Create lag and add port4 as member. | Create lag and member| lag and member created|
| Create vlan and add VLAN member with port0 and port1.| Prepare to send from lag to vlan.| Vlan and member created.|
| Send packet on port1 with target mac on port4. | Forwarding from port1 to port4.| Receive packet on port4.|
| Disable egress and ingress on lag member4. send packet | Packet dropped on port4| Packet drop.|
| Enable lag egress and ingress. Send packet with vlan tag on lag port4 with a new dest mac.|Packet flooding on vlan members, port0 and port1.|Packet received.|

