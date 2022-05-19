# SAI Vlan Test plan
- [SAI Vlan Test plan](#sai-vlan-test-plan)
  - [Overriew](#overriew)
    - [Test Topology](#test-topology)
    - [Testbed](#testbed)
  - [Scope](#scope)
  - [Basic Test data and SAI APIs](#basic-test-data-and-sai-apis)
    - [Create VLAN and VLAN member](#create-vlan-and-vlan-member)
    - [Packet example](#packet-example)
  - [Basic Configurations](#basic-configurations)
    - [Basic VLAN configuration](#basic-vlan-configuration)
    - [Basic Forwarding Table](#basic-forwarding-table)
  - [Tests](#tests)
    - [Test suite: Tagging and trunk/access](#test-suite-tagging-and-trunkaccess)
    - [Test suite: Flooding and learning](#test-suite-flooding-and-learning)
      - [Trunk VLAN](#trunk-vlan)
      - [Access VLAN](#access-vlan)
    - [Composit scenario: ARP Flooding and learn](#composit-scenario-arp-flooding-and-learn)
    - [Test case: Test Frame Filtering](#test-case-test-frame-filtering)
    - [Test case: Test native vlan](#test-case-test-native-vlan)
  - [SAI APIs operations](#sai-apis-operations)
    - [Test case: Test VLAN related counters.](#test-case-test-vlan-related-counters)
    - [Test case: Vlan member list.](#test-case-vlan-member-list)
  - [Test Case: VLAN interface (RIF/SVI)](#test-case-vlan-interface-rifsvi)
## Overriew
The purpose of this test plan is to test the VLAN function from SAI.

### Test Topology
For SAI-PTF, it will use a non-topology network structure for the sai testing. 

### Testbed
Those tests will be run on the testbed structure as below, the components are:
* PTF - running in a server that can connect to the target DUT
* SAI server - running on a dut
  

*p.s. cause the SAI testing will not depend on any sonic components, then there will be no specific topology(T0 T1 T2) for testing.*
## Scope
The test will include three parts
1. Vlan functionalities
   - Flooding
   - Forwarding
   - Trunk/Access
   - Tagging/Untagging(802.1Q)
   - ARP 
2. SAI APIs operations
   - Vlan Counters
   - Vlan and member list operations
3. Composit scenario
   - VLAN Interface (RIF/SVI)

## Basic Test data and SAI APIs
During testing, we need to use SAI APIs for testing. By using the SAI-PTF structure, we can invoke the SAI with RPC APIs remotely, the sample code is below.
### Create VLAN and VLAN member
- Create Vlan 10
   ```Python
   sai_thrift_create_vlan(self.client, vlan_id=10)
   ```
- Create Native vlan
   ```python
   sai_thrift_set_port_attribute(self.client, port_id, port_vlan_id=10)
   ```
- Create Vlan member and VLAN member with different mode
   Untag mode and access port
   ```python
   sai_thrift_create_vlan_member(
                    self.client,
                    vlan_id=10,
                    bridge_port_id=port_bridge_port,
                    vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
   ```
   Tag mode and trunk port
   ```python
   sai_thrift_create_vlan_member(
                    self.client,
                    vlan_id=10,
                    bridge_port_id=port_bridge_port,
                    vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
   ```
- Add static FDB entry in the FDB table
   
   In the code below, we can create FDB entry, and bind the SMAC with a port with a static dfb entry.
   ```python
   sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac1, bv_id=self.vlan_oid)
   sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=port_bp,
                packet_action=SAI_PACKET_ACTION_FORWARD)
   ```
- FDB operations
  Get FDB entries
  ```python
  sai_thrift_get_switch_attribute(
            self.client,
            available_fdb_entry=True)
  ```
- Clear FDB entries
  Clear the learned FDB entries with the VLAN id object id
  ```python
  sai_thrift_flush_fdb_entries(
                    self.client,
                    bv_id=self.vlan_oid,
                    entry_type=SAI_FDB_ENTRY_TYPE_DYNAMIC)
  ```

### Packet example
- Tagged packet with VLAN id 

   When a packet sent to a access port or send out from a access port, we will get a packet with the VLAN id.
   ```python
    tagged_packet(eth_dst='00:11:11:11:11:11',
                eth_src='00:22:22:22:22:22',
                vlan_vid=10,
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
## Basic Configurations
### Basic VLAN configuration
|VLAN ID|Ports|Tag mode|
|-|-|-|
|10|Port1-4|Tag|
||Port5-8|Untag|
|100|port9-12|Tag|
||Port13-16|Untag|

### Basic Forwarding Table
For testing, we can use basic FDB APIs to Add the FDB entry into CAM. The Rule as below
|Name|MAC|PORT|VLAN|
|-|-|-|-|
|fdb1-4  |mac1:00:11:11:11:11:11 -  mac4:00:44:44:44:44:44|Port1-4|10|
|fdb5-8  |mac5:00:55:55:55:55:55 -  mac8:00:88:88:88:88:88|Port5-8|10|
|fdb9-12 |mac9:00:99:99:99:99:99 -  mac12:01:22:22:22:22:22|Port9-12|100|
|fdb13-16|mac13:01:33:33:33:33:33 - mac16:01:66:66:66:66:66|Port13-16|100|
|fdb17-32|mac17:01:77:77:77:77:77 - mac32:03:22:22:22:22:22|Port17-32||

## Tests

### Test suite: Tagging and trunk/access

This is a basic set of testing. This test suite will cover the basic VLAN function around tag/untag and trunk/access port.

*p.s. Do not take native VLAN in this scenario (it will be in other tests). Please make sure the native VLAN will not impact the result.*

**Testing Objective**

With a Tagged packet or untagged packet, on the trunk and access port, when ingress and egress happen, the behavior as below
| Port mode | packet tag mode | Direction | Action                                   |
| :---------|--------------- | :-------- | :--------------------------------------- |
| Access|Untag| Ingress   | Accept the packet.       |
|       |Untag | Egress    | Untag the packet. |
|       |Tag| Ingress   | Drop if the native VLAN doesn't match the tag. Accept if the native VLAN matches the tag.|
|       |Tag | Egress    | Untag the packet. |
| Trunk |Untag| Ingress   | Drop if native VLAN doesn't match the tag. Accept if the native VLAN matches the tag.  |
|       |Untag| Egress    | Tag the packet. |
|       |Tag| Ingress   | Drop, if both VLAN and native VLAN don't match. |
|       |Tag| Egress    | Tag the packet. |

**Testing Description**
```
Test example:
                                    DMAC-> |(Trunk:10)->pkt(Tag:10)
1. pkt(tagged：10) ->(trunk:10)|DUT|
                                    DMAC-> |(Acess:10)->pkt(Untag)

                                    DMAC-> |(Trunk:10)->pkt(Tag:10)
2. pkt(tagged:10)  -> (access:10)|DUT|
                                    DMAC-> |(Acess:10)->pkt(Untag)                              

3. pkt(Tagged:20)  -> (Trunk:10)|DUT| -> X
```

Below is the test for checking this.
Precondition/Setup:
- Create VLAN as the basic configuration.
- Create an FDB table as a basic configuration.

Cases:
| Goal| Cases | Expect  |
|-|-|-|
|``Tag``:Trunk -> Trunk port.| Send VLAN10 ``tagged`` packet with dest mac4 on port1. |  VLAN10 ``tagged`` packet received on port4.|
|``Tag``:Trunk -> Acess port.| Send VLAN10 ``tagged`` packet with dest mac5 on port1. |  ``Untag`` packet received on port5.|
|``Tag``:Acess -> Trunk port.| Send VLAN10 ``tagged`` packet with dest mac4 on port6. |  VLAN10 ``tagged`` packet received on port4.|
|``Tag``:Acess -> Acess port.| Send VLAN10 ``tagged`` packet with dest mac5 on port6. |  ``Untag`` packet received on port4.|
|Drop: Unmatched ``Tag`` on the trunk.| Send VLAN10 ``tagged`` packet with dest mac4 on port1. |  Drop.|

### Test suite: Flooding and learning

#### Trunk VLAN 
Those tests will verify flooding and learning for VLAN. 

VLANs divide the broadcast domains into multicast domains. When flooding a VLAN packet, the broadcast should only happen within a target VLAN.

After saving the mac address to the forwarding table, only unicast will happen with the target mac.

ARP requests and responses will also be covered in this test suite.

**Testing Description**
For flooding on the VLAN packet, it should only flood to VLAN members.
```
Test example:
Without DMAC in CAM                                   
                                                                | pkt(Untag):VLAN10:access
  pkt(tagged:10:DMAC)   ->  (trunk:10)|DUT| -> Flooding -> VLAN:10 
                                                                | pkt(Tag):VLAN10:Trunk
With MAC in CAM
  pkt(tagged:10:DMAC)   ->  (trunk:10)|DUT| -> Forward(FDB:MAC=DMAC) -> VLAN:10:PORT
```
**Precondition**
- Create VLAN as the basic configuration.

*Need to loop on all the trunk ports.*

Cases:
| Goal |Cases |  Expect  |
|-|-|-|
| Flooding to VLAN members. | Send ``VLAN10`` ``tagged`` packet with dest ``mac4`` on ``port1~3``. |``Tagged`` Packets from ``Trunk``, ``Untag`` Packet from ``Access``| 
|Unicast on VLAN port after learning.| Send ``VLAN10`` ``tagged`` packet with dest ``mac1~3`` on ``port4``.|  Received ``VLAN10`` ``tagged`` packet on mac matched port.|
|No flooding on VLAN port after learning.| Send ``VLAN10`` ``tagged`` packet with dest ``mac1~3`` on ``port4``.| No other packet than mac matched port.|
|MAC learning.| Use FDB SAI API to check FDB entries.| ``4`` entries should be added, and marked with VLAN id.|

#### Access VLAN 
**Testing Description**
For flooding on the VLAN packet, it should only flood to VLAN members.
```
Test example:
Without DMAC in CAM                                   
                                                                | pkt(Untag):VLAN10:Access
  pkt(tagged:10:DMAC)   ->  (access:10)|DUT| -> Flooding -> VLAN:10 
                                                                | pkt(Tag):VLAN10:Trunk
With MAC in CAM
  pkt(tagged:10:DMAC)   ->  (access:10)|DUT| -> Forward(FDB:MAC=DMAC) -> VLAN:10:PORT
```
Precondition/Setup:
- Create VLAN as the basic configuration.

*Need to loop on all the trunk ports.*

| Goal | Steps |  Expect  |
|-|-|-|
| Flooding to VLAN members. | Send ``Untagged`` packet with dest ``mac5`` on ``port6~8``. |``Tagged`` Packets from ``Trunk``, ``Untagged`` Packet from ``Access``| 
|Unicast on VLAN port after learning.| Send ``Untagged`` packet with dest ``mac6~8`` on ``port5``.| Received ``Untagged`` packet on mac matched port.|
|No flooding on VLAN port after learning.| Send ``Untagged`` packet with dest ``mac6~8`` on ``port5``.| No other packet than mac matched port.|
|MAC learning.| Use FDB SAI API to check FDB entries.| | 4 entries should be added, and marked with VLAN id.|

### Composit scenario: ARP Flooding and learn
- ARP request
  ```Python
  simple_arp_packet(
            eth_dst='FF:FF:FF:FF:FF:FF', # boardcast
            eth_src='00:11:11:11:11:11',
            arp_op=1,  # ARP request
            ip_tgt='10.10.10.2',
            ip_snd='10.10.10.1',
            hw_snd='00:11:11:11:11:11',
            hw_tgt="00:00:00:00:00:00")
  ```
- ARP response
  ```Python
  simple_arp_packet(
            eth_dst='00:11:11:11:11:11', 
            eth_src='00:22:22:22:22:22',
            arp_op=2,  # ARP response
            ip_tgt='10.10.10.1',
            ip_snd='10.10.10.2',
            hw_snd='00:22:22:22:22:22',
            hw_tgt="00:11:11:11:11:11")
  ```
In the ARP scenario, the mac learning process is:
1. Send an ARP request, with the source MAC, dest IP, and src IP, for broadcast, the DST MAC is ff:ff:ff:ff:ff:ff
2. After the encountered device get the ARP packet and sends out a response, the packet will be filled with a source mac, this is the mac we want from step 1.
3. Switch received that response, cause the switch learned a MAC on step 1 on the sending port, then it can make a unicast here.

**p.s. The test will be ended here, will not test the send from the port in step 1 again, it is already covered in other tests.**

**Testing Description**

Testing ARP scernairo
```
Test example:
1. ARP Request                                  
                                                                | pkt(Untag):VLAN10:Access
  ARP Req pkt(Untag:DMAC) ->  (access:10)|DUT| -> Flooding -> VLAN:10 
                                                                | pkt(Tag):VLAN10:Trunk
2. ARP REsponse 
  pkt(access:10)|DUT| <- Forward(FDB:MAC=DMAC) <- VLAN:10:PORT <- RP Resp pkt(tagged:10:DMAC)

```
Precondition/Setup:
- Create VLAN as the basic configuration.

| Goal | Steps | Expect  |
|-|-|-|
| ARP Request flooding to all ports. | Send ``Untagged`` ARP packet with dest ``mac4`` on ``port1``. |``Tagged`` Packets from ``Trunk``, ``Untagged`` Packet from ``Access``|
| Mac learned from ARP request and ARP response.| Send a ``tagged`` ARP response packet with src:mac4, dest:mac1 to port4 and check the FDB entries.|Unicast happened.|
| FDB entries added.| Call API to check the FDB entries.|New mac entries in CAM.|


### Test case: Test Frame Filtering
**Testing Objective**

The switch will not forward packet source MAC equals to the port's mac in FDB.

**Testing Description**

The switch will drop the packet when the packet sends to a dest MAC, but the same dest mac-address already exists in the FDB and is mapping to that port. 
```
Test example:                                
                                                                | SRCPORT:MAC1 = DMAC|
  pkt(Untag:DMAC):vlan:10 -> SRCPORT(Trunk:10)  -> |DUT| -> |FDB|                    | -> X 
                                                                | MAC2               |

```
Precondition/Setup:
- Create VLAN as the basic configuration.
- Create an FDB table as a basic configuration.

| Goal |Steps |  Expect  |
|-|-|-|
| FDB added.| Add a non-existing ``MacX`` to port1. Check FDB |FDB added.|
| Filter frame. | Send VLAN10 ``tagged`` packet with dest ``MacX`` on ``port1``. |Packet dropped|
| Filter frame. | Send VLAN10 ``tagged`` packet with dest ``MAC1`` on ``port1``. |Packet dropped|

### Test case: Test native vlan

**Testing Objective**

For Native VLAN, only consider the ingress direction, the entry condition for VLAN, and the behavior below
| Port mode | packet tag mode |  Action                                   |
| ---------|--------------- | --------------------------------------- |
| Access|Tag| Drop, if the native id does not match.       |
| Trunk |Untag| Drop, if native VLAN id does not match..  |
|       |Tag|  Drop, if the VLAN id does not match and does not meet the native VLAN case. |

**Testing Description**

Test for native VLAN
*Only Consider the ingress direction*
```
Test example:  
                                           | Y Native VLAN -> Forward(FDB:MAC=DMAC) -> VLAN:10:PORT
  pkt(tagged:10:DMAC)   ->  (Access:10)|DUT|
                                           | N Native VLAN -> Drop

  pkt(untagged:10:DMAC) ->   (Trunk:10)|DUT| Native VLAN -> Forward(FDB:MAC=DMAC) -> VLAN:10:PORT

                                           | Y (VLAN||Native VLAN) -> Forward(FDB:MAC=DMAC) -> VLAN:10:PORT
  pkt(tagged:10:DMAC)   ->   (Trunk:10)|DUT|
                                           | N (VLAN&Native VLAN) -> Drop
```
**Precondition**
- Create VLAN as the basic configuration.
- Create an FDB table as a basic configuration.

*Need to loop on all the access and trunk ports.*

Cases:
| Goal |Cases |  Expect  |
|-|-|-|
| Forwarding, ``VLAN10`` ``Tagged`` packet on ``Native`` from a ``VLAN10`` ``Access``.|Send ``tagged`` packet with dest ``mac3`` on ``port5``. |  ``tagged`` packet received on port3.|
| Drop, ``VLAN40`` ``Tagged`` packet on ``Native`` from a ``Access``.|Send ``tagged`` packet with dest ``mac3`` on ``port5``. |  Packet dropped.|
| Forwarding, ``Untagged`` packet on ``Native`` from a ``Trunk``.|Send ``Untagged`` packet with dest ``mac3`` on ``port1``. |  ``tagged`` packet received on port3.| ``Untagged`` packet with dest mac3 on port1. |  Packet dropped.|
| Forwarding, ``VLAN10`` ``Tagged`` packet on ``Native`` from a ``VLAN10`` ``Trunk``.|Send ``tagged`` packet with dest ``mac3`` on ``port1``. |  ``tagged`` packet received on port3.|
| Drop, ``VLAN40`` ``Tagged`` packet on ``Native`` from a ``Trunk``.|Send ``tagged`` packet with dest ``mac3`` on ``port1``. |  Packet dropped.|


## SAI APIs operations

### Test case: Test VLAN related counters.
**Testing Objective**
For VLAN-related counters, SAI should be able to get the counter and clear them.

Below is the sample API to operate those data

Check counters
```Python
        stats = sai_thrift_get_vlan_stats(self.client, self.vlan10)
        in_bytes = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

```
Clear counters
```Python
sai_thrift_clear_vlan_stats(self.client, self.vlan10)
```

Precondition/Setup:
- Create VLAN as the basic configuration.
- Create an FDB table as a basic configuration.

| Goal |Steps/Cases |  Expect  |
|-|-|-|
| Forwarding, ``VLAN10`` ``Tagged`` packet on ``Native`` from a ``VLAN10`` ``Access``.|Send ``tagged`` packet with dest ``mac3`` on ``port5``. |  ``tagged`` packet received on port3.|
| Counter Changed accordingly.|Use the SAI API to check the counters | Counter increased, bytes counter: OCTETS increased, other counters + 1|
|  Counter reset.|Use the SAI API to clear the counters.| Related counter is reset to zero. |


### Test case: Vlan member list.
**Testing Objective**
Test VLAN and member list.

Sample APIs

Get VLAN member list
```Python
 vlan_member_list = sai_thrift_object_list_t(count=100)
        mbr_list = sai_thrift_get_vlan_attribute(
            self.client, self.vlan10, member_list=vlan_member_list)
```

nagtive test
```python
    vlan_attr = sai_thrift_get_vlan_attribute(
        self.client, vlan_oid=11, learn_disable=True)

    incorrect_member = sai_thrift_create_vlan_member(
        self.client,
        vlan_id=11,
        bridge_port_id=self.port27_bp,
        vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
```

Precondition/Setup:
- Create VLAN as the basic configuration.
- Create an FDB table as a basic configuration.


| Steps/Cases | Goal | Expect  |
|-|-|-|
| VLAN member list and member count are right. |Use the SAI API to check the VLAN member |  Vlan member list.|
| VLAN and its member removed.|Remove the VLAN member from VLAN 10 and remove VLAN. |  VLAN and its member removed.|
| Error when creating member to un-exist VLAN. |Use the SAI API to create a VLAN member on VLAN 10 |  Vlan attribute is 0.|



## Test Case: VLAN interface (RIF/SVI) 
**Testing Objective**
Test the configuration as below

```xml
 <DeviceL3Info Hostname="AMS08-0101-0522-08T0">
    <VlanInterface Name="s1h" AttachTo="Ethernet6/1;Ethernet7/1;Ethernet8/1" vlanid="608" tag="608" Subnets="10.95.88.208/28" Type="AzureCompute" />
    <IPInterface AttachTo="s1h" Prefix="10.95.88.209/28" />
    <IPInterface AttachTo="s1h" Prefix="2603:10a0:31c:8144::1/64" />
  </DeviceL3Info>
```
Convert to config_db.json
```json
    "VLAN": {
        "Vlan608": {
            "members": [
                "Ethernet6/1",
                "Ethernet7/1",
                "Ethernet8/1",
				],
            "vlanid": "608"
        }
    },
    "VLAN_INTERFACE": {
        "Vlan608": {},
        "Vlan608|10.95.88.209/28": {},
        "Vlan608|2603:10a0:31c:8144::1/64": {}
    },
    "VLAN_MEMBER": {
        "Vlan608|Ethernet6/1": {
            "tagging_mode": "untagged"
        },
        "Vlan608|Ethernet7/1": {
            "tagging_mode": "untagged"
        },

        "Vlan608|Ethernet8/1": {
            "tagging_mode": "untagged"
        },
    }
```

Need the APIs as below

- Create Router Interface for a VLAN interface
  ```Python
  sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan10)
  ```
- Create next hop to a IP address '10.10.0.1' and mac
  ```python
  sai_thrift_create_next_hop(
        self.client,
        ip=sai_ipaddress('10.10.0.1'),
        router_interface_id=self.vlan100_rif,
        type=SAI_NEXT_HOP_TYPE_IP)
  sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, dst_mac_address=self.dmac1)
  sai_thrift_route_entry_t(
            vr_id=self.default_vrf, destination=sai_ipprefix('10.10.10.1/32'))
  ```
**Testing Description**
When a switch needs to forward in layer3, it needs a vlan interface in layer3(SVI). By this vlan interface, swtich can forward the packet to other VLAN, which can be on other ports in the same switch or to other server port, that dest port then can accept packet from different ports.
```
Test example:

pkt(Tagged:10)  -> (Trunk:10)|DUT| -> RIF(VLAN_IF->MAC5:IP1)
                          PORT  <- FDB(PORT:MAC)   <-|
                                                  OR | -> Server(NHop)
```

**Precondition/Setup:**
- Create VLAN as the basic configuration.
- Create an FDB table as a basic configuration.

Below is the test for checking this.
|  Goal |Steps/Cases | Expect  |
|-|-|-|
|  Create a VLAN interface. |Create route entry and next hop for VLAN 10, with [IP1] and [mac5].| Vlan interface created.|
| Add FDB entries for RIF. MAC is [mac5], and map it to Port5 in the FDB entry. |Simulate the mac learning.| Set up forwarding table|
| Forwarding from trunk to a trunk port on native VLAN.| Send ``tagged`` packet with dest mac3 on port1. |  ``tagged`` packet received on ``port5``.|
