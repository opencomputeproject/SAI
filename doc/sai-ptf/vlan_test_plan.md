# SAI VLAN Test plan <!-- omit in toc --> 

- [Overriew](#overriew)
- [Test Environment](#test-environment)
  - [Testbed](#testbed)
  - [Test Configuration](#test-configuration)
  - [Variations](#variations)
- [Test Execution](#test-execution)
  - [Basic function test](#basic-function-test)
    - [Untagging and access](#untagging-and-access)
      - [Testing Objective](#testing-objective)
      - [Test Data/Packet](#test-datapacket)
      - [Test Cases](#test-cases)
    - [Frame Filtering](#frame-filtering)
      - [Testing Objective](#testing-objective-1)
      - [Test Data/Packet](#test-datapacket-1)
      - [Additional config:](#additional-config)
      - [Test Cases](#test-cases-1)
    - [MAC learning](#mac-learning)
      - [Testing Objective](#testing-objective-2)
      - [Test Data/Packet](#test-datapacket-2)
      - [Additional config:](#additional-config-1)
      - [Test Cases](#test-cases-2)
  - [SAI API test](#sai-api-test)
    - [Vlan member list](#vlan-member-list)
      - [Testing Objective](#testing-objective-3)
      - [Test Cases](#test-cases-3)
    - [VLAN related counters.](#vlan-related-counters)
      - [Testing Objective](#testing-objective-4)
      - [Test Data/Packet](#test-datapacket-3)
      - [Test Cases](#test-cases-4)
    - [Disable mac learning](#disable-mac-learning)
      - [Testing Objective](#testing-objective-5)
      - [Test Data/Packet](#test-datapacket-4)
      - [Additional config:](#additional-config-2)
      - [Test Cases](#test-cases-5)
  - [Composit scenario](#composit-scenario)
    - [L3 switching(Inter-VLAN)](#l3-switchinginter-vlan)
      - [Testing Objective](#testing-objective-6)
      - [Test Data/Packet](#test-datapacket-5)
      - [Test Cases](#test-cases-6)
    - [ARP Flooding and mac learning](#arp-flooding-and-mac-learning)
      - [Testing Objective](#testing-objective-7)
      - [Test Data/Packet](#test-datapacket-6)
      - [Additional config:](#additional-config-3)
      - [Test Cases](#test-cases-7)
# Overriew
The purpose of this test plan is to test the VLAN function from SAI.

The test will include three parts
- Vlan functionalities
- SAI APIs operations
- Composit scenario

# Test Environment
## Testbed
Those tests will be run on the testbed structure, the components are:
* PTF - running in a server that can connect to the target DUT
* SAI server - running on a dut
## Test Configuration

For the test configuration, please refer to the file 
  - [VLAN_config](./config_data/vlan_config_t0.md)
  - [FDB_config](./config_data/fdb_config_t0.md)
  - [Route_config](./config_data/route_config_t0.md)
  
**Note. All the tests will be based on the configuration above, if any additional configuration is required, it will be specified in the Test case.**

*p.s. Please refer the sample packet in [VLAN_config#Sample_Packet](./config_data/vlan_config_t0.md#sample-datapacket)*

## Variations
Cause the testbed might also encounter some issues like the host interface being down. 
Before running the actual test there will need some sanity test to check the DUT status and select the active ports for testing.

**All the ports in this test plan just to illustrate the test purpose, they are not exactly the same for the actual environment.**

# Test Execution
## Basic function test

### Untagging and access

#### Testing Objective
This test verifies the VLAN function around untag and access ports.

*p.s. This test will not check function with the native VLAN scenario (it will be in other tests). Please make sure the native VLAN will not impact the result.*



With an untagged packet, on the access port, when ingress and egress happen, the behavior as below
| Port mode | packet tag mode | Direction | Action                                   |
| :---------|--------------- | :-------- | :--------------------------------------- |
| Access|Untag| Ingress   | Accept the packet.       |
|       |Untag | Egress    | Untag the packet. |
| Access|Tag| Ingress   | Accept the packet.       |
|       |Tag | Egress    | Untag the packet. |


```
Test example:
Untagged:                                    
  pkt(Untag:DMAC=MAC2)  -> Port1:Access:VLAN1000-> Port2:Access:VLAN1000 -> pkt(Untag:DMAC=MAC2)
Tagged:
  pkt(Tag:VLAN2000:DMAC=MAC10)  -> Port9:Access:VLAN2000-> Port10:Access:VLAN2000 -> pkt(Untag:DMAC=MAC10)
```

#### Test Data/Packet
[Sample_Packet](./config_data/vlan_config_t0.md#sample-datapacket)
- Input: Packet(Untag) and Packet(Tag)
- Expected: Packet(Tag)

  *p.s. please refer the sample packet in [VLAN_config#Sample_Packet](./config_data/vlan_config_t0.md#sample-datapacket)*

#### Test Cases
| Goal| Cases | Expect  |
|-|-|-|
|``Untag``:Acess -> Acess port.| Send VLAN1000 ``Untagged`` packet on Port1 with mac2 as dest mac. |  ``Untag`` packet received on port2.|
|``Tag``:Acess -> Acess port.| Send VLAN2000 ``Untagged`` packet on Port9 with mac10 mac as dest mac. |  ``Untag`` packet received on port10.|

### Frame Filtering
#### Testing Objective

Drop packet when the destination port from MAC table search is the port which packet come into the switch.

```
Test example:                                
                                                                                  | MAC1 |
  pkt(Untag:DMAC=MAC1 or MACX) -> Port1:Access:VLAN1000 -> FDB(contains:MAC1,MACX)|      | -> X 
                                                                                  | MACX |
```

#### Test Data/Packet

[Sample_Packet](./config_data/vlan_config_t0.md#sample-datapacket)
- Input: Packet(Untag)
- Expected: Drop

#### Additional config:
- Add a non-existing ``MacX`` to port1

#### Test Cases
| Goal | Case |  Expect  |
|-|-|-|
| Filter frame. | Send VLAN1000 ``Untagged`` packet with dest ``MacX`` on ``port1``. |Packet dropped|
| Filter frame. | Send VLAN1000 ``Untagged`` packet with dest ``MAC1`` on ``port1``. |Packet dropped|



### MAC learning
#### Testing Objective
For mac learning in VLAN scenario, in contain those two cases
1. the packet sent to the VLAN port will flood to other ports in the same VLAN before learning the mac address from the packet
2. The packet is only sent to the target port when the MAC table contains the MAC address on the target port(Dest MAC).


```
1. Flooding                                 
                                                             | Port2|
 pkt(Untag:DMAC=MAC2) -> Port1:Access:VLAN1000 -> Flooding ->|  To  |-> pkt(Untag)
                                                             | Port8|

2. unicast 
  pkt(Untag:DMAC=MAC1)  -> Port2:Access:VLAN1000-> Port1:Access:VLAN1000 -> pkt(Untag:DMAC=MAC1)

```

#### Test Data/Packet

[Sample_Packet](./config_data/vlan_config_t0.md#sample-datapacket)
- Input: Packet(Untag)
- Expected: Packet(Untag)

#### Additional config:
- Do not config the MAC table

#### Test Cases
| Goal | Case | Expect  |
|-|-|-|
| Flooding in VLAN. | Send ``Untagged`` packet with dest ``mac2`` on ``port1``. |Receive ``Untagged`` packet from Port2 to Port8|
|MAC learning|check the FDB entries.|New mac entries in FDB.|
| Forwarding based on MAC table.| Send a ``Untagged`` packet with src:mac2, dest:mac1 on port2.| Receive ``Untagged`` packet on Port1|
|MAC learning|check the FDB entries.|New mac entries in FDB.|


## SAI API test

### Vlan member list
#### Testing Objective

Test VLAN and member list.

#### Test Cases
| Goal | Cases | Expect  |
|-|-|-|
| VLAN APIs get VLAN and its member. |Use the SAI API to check the VLAN and its member.|  VLAN and VLAN member list.|
| Remove VLAN and its member.|Remove the VLAN member from VLAN 1000 and remove VLAN. |  VLAN and its member removed.|
| Error when creating member to un-exist VLAN. |Use the SAI API to create a VLAN member on VLAN 1000 |  Error happened.|

### VLAN related counters.
#### Testing Objective

For VLAN-related counters, SAI should be able to get the counter and clear them.

#### Test Data/Packet
[Sample_Packet](./config_data/vlan_config_t0.md#sample-datapacket)
- Input: Packet(Untag)
- Expected: Packet(Untag)

#### Test Cases
| Goal |Cases |  Expect  |
|-|-|-|
| Packet Forwarding.|Send VLAN1000 ``Untagged`` packet on Port1 with port2 mac as dest mac. |  ``Untagged`` packet received on port3.|
| Counter Changed accordingly.|Use the SAI API to check the counters | Counter increased, bytes counter: OCTETS increased, other counters + 1|
|  Counter reset.|Use the SAI API to clear the counters.| Related counter is reset to zero. |


### Disable mac learning
#### Testing Objective
Test the function when disabling VLAN MAC learning.
When disabled, no new MAC will be learned in the MAC table.

#### Test Data/Packet

[Sample_Packet](./config_data/vlan_config_t0.md#sample-datapacket)
- Input: Packet(Untag)
- Expected: Packet(Untag)

#### Additional config:
- Do not config the MAC table

#### Test Cases
| Goal | Case | Expect  |
|-|-|-|
| Disable Vlan MAC learning. | Set the VLAN attribute to disable the VLAN MAC learning. |VLAN attribute set.|
| Flooding in VLAN. | Send ``Untagged`` packet with dest ``mac2`` on ``port1``. |Receive ``Untagged`` packet from Port2 to Port8|
| Flooding in VLAN. | Send ``Untagged`` packet with dest ``mac1`` on ``port2``. |Receive ``Untagged`` packet on ports except for port2.|
|MAC learning|check the FDB entries.|No new mac entries in FDB.|

## Composit scenario

### L3 switching(Inter-VLAN) 
#### Testing Objective

Testing L3 switching(Inter-VLAN).

In this case, it will cover the scenarios for Server to Server (east/west) in different VLAN(Inter-VLAN)

```
Server To Server
  PC1 -> pkt(Untag) -> |DUT|Port1:Access:VLAN1000 
                                              |L3| <-> |Virtual Switch| 
  PC2 <- pkt(Untag) <- |DUT|Port9:Access:VLAN2000
```
*All the layer 3 router tables are created as the basic config, not MAC and ARP learning in this test process.*
The process is as below:
1. PC1 sends a untag packet to port1, which is a VLAN1000 access port
2. Based on packet dest IP, derive the route to the dest IP, Dest MAC and Port derived from L3 table. Then SRC MAC change to Switch MAC, Dest MAC change to PORT MAC, forwarding to PORT9
3. Packet goes through VLAN2000 port9

#### Test Data/Packet
- Input Packet
  ```Python
  simple_tcp_packet(
            eth_dst=SVI_MAC,
            eth_src=SRC_MAC,
            ip_dst=DEST_IP,
            ip_src=SRC_IP)
  ```

- Expected Packet
  ```Python
  simple_tcp_packet(
            eth_dst=PORT_MAC, 
            eth_src=SVI_MAC,
            ip_dst=DEST_IP,
            ip_src=SRC_IP)
  ```
#### Test Cases
|  Goal | Case | Expect  |
|-|-|-|
| Inter-VLAN switching .| Send ``Untagged`` packet on port1 with dest ``SVI_MAC``. |  ``Untagged`` packet received on ``PORT9``.|




### ARP Flooding and mac learning
#### Testing Objective
In the ARP scenario, the mac learning process is:
1. Send an ARP request, with the source MAC, dest IP, and src IP, for broadcast, the DST MAC is ff:ff:ff:ff:ff:ff
2. After the target port gets the ARP packet MAC table fills with port SRC MAC.
3. Switch received that response, based on the MAC table, it makes a unicast here.

**p.s. The test will be ended here, will not test the send from the port in step 1 again, it is already covered in other tests.**

Testing ARP scenario
```
Test example:
1. ARP Request                                  
                                                                      | Port2|
  ARP Req pkt(Untag:DMAC=MAC2) -> Port1:Access:VLAN1000 -> Flooding ->| to   |-> pkt(Untag)
                                                                      | Port8|

2. ARP Response 
  ARP Resp pkt(Untag:DMAC=MAC1)  -> Port2:Access:VLAN1000-> Port1:Access:VLAN1000 -> pkt(Untag:DMAC=MAC1)

```
#### Test Data/Packet
**Both those two packets will be used in input and output.**
- ARP request
  ```Python
  simple_arp_packet(
            eth_dst='FF:FF:FF:FF:FF:FF', # boardcast
            eth_src=MAC1,
            arp_op=1,  # ARP request
            ip_tgt=IP2,
            ip_snd=IP1,
            hw_snd=MAC1,
            hw_tgt=MAC2)
  ```
- ARP response
  ```Python
  #Untagged:
  simple_arp_packet(
            eth_dst=MAC1, 
            eth_src=MAC2,
            arp_op=2,  # ARP response
            ip_tgt=IP1,
            ip_snd=IP2,
            hw_snd=MAC2,
            hw_tgt=MAC1)
  
  #Tagged(Just Sample, not used in this case):
  simple_arp_packet(
            eth_dst=MAC1, 
            eth_src=MAC2,
            arp_op=2,  # ARP response
            ip_tgt=IP1,
            ip_snd=IP2,
            vlan_vid=1000,
            hw_snd=MAC2,
            hw_tgt=MAC2)
  ```

#### Additional config:
- Do not config the MAC table

#### Test Cases
| Goal | Case | Expect  |
|-|-|-|
| ARP Request flooding. | Send ``Untagged`` ARP packet with dest ``mac2`` on ``port1``. |``Untagged`` Arp request from Port2 to Port8|
|MAC learning|check the FDB entries.|New mac entries in FDB.|
| ARP response Forwarding.| Send a ``Untagged`` ARP response packet with src:mac2, dest:mac1 on port2.|``Untagged`` ARP response from Port1|
|MAC learning|check the FDB entries.|New mac entries in FDB.|
