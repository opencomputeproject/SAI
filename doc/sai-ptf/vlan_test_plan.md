# SAI VLAN Test plan <!-- omit in toc --> 

- [Test Configuration](#test-configuration)
- [Test Execution](#test-execution)
  - [Common Test Data/Packet](#common-test-datapacket)
  - [Test Group: Tagging and access](#test-group-tagging-and-access)
    - [Case1: test_untag_access_to_access](#case1-test_untag_access_to_access)
    - [Case2: test_tag_access_to_access](#case2-test_tag_access_to_access)
  - [Test Group: Frame Filtering](#test-group-frame-filtering)
    - [Case3: test_untagged_frame_filtering](#case3-test_untagged_frame_filtering)
    - [Case4: test_tagged_frame_filtering](#case4-test_tagged_frame_filtering)
  - [Test Group: VLAN flooding](#test-group-vlan-flooding)
    - [Case5: test_tagged_vlan_flooding](#case5-test_tagged_vlan_flooding)
    - [Case6: test_untagged_vlan_flooding](#case6-test_untagged_vlan_flooding)
  - [Test Group: MAC learning](#test-group-mac-learning)
    - [Case7: test_untagged_mac_learning](#case7-test_untagged_mac_learning)
    - [Case8: test_tagged_mac_learning](#case8-test_tagged_mac_learning)
  - [Test Case9: Vlan member API](#test-case9-vlan-member-api)
  - [Test Case10: Failed to add member to invalidate VLAN](#test-case10-failed-to-add-member-to-invalidate-vlan)
  - [Test Group: VLAN Counters/Status](#test-group-vlan-countersstatus)
    - [Case11: test_tagged_vlan_status](#case11-test_tagged_vlan_status)
    - [Case12: test_untagged_vlan_status](#case12-test_untagged_vlan_status)
  - [Test Group7: Disable mac learning](#test-group7-disable-mac-learning)
    - [Case13: test_disable_mac_learning_tagged](#case13-test_disable_mac_learning_tagged)
    - [Case14: test_disable_mac_learning_untagged](#case14-test_disable_mac_learning_untagged)
  - [Test Case15: L3 switching(Inter-VLAN)](#test-case15-l3-switchinginter-vlan)
  - [Test Group: ARP Flooding and mac learning](#test-group-arp-flooding-and-mac-learning)
    - [Case16: test_arp_request_flooding](#case16-test_arp_request_flooding)
    - [Case17: test_arp_response_learning](#case17-test_arp_response_learning)
# Test Configuration

For the test configuration, please refer to the file 
  - [Config_t0](./config_data/config_t0.md)
  
**Note. All the tests will be based on the configuration above, if any additional configuration is required, it will be specified in the Test case.**

# Test Execution

## Common Test Data/Packet
In this VLAN test, most of test case will be tested with Tagged or Untagged packet, the example packet structure as below.
- Tagged packet with VLAN id 

   ```python
    tagged_packet(eth_dst='00:11:11:11:11:11',
                eth_src='00:22:22:22:22:22',
                vlan_vid=1000,
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
**Note. If need other kinds of packet, it will be added in the test case/group respectively.**

## Test Group: Tagging and access 
### Case1: test_untag_access_to_access
### Case2: test_tag_access_to_access

### Testing Objective <!-- omit in toc --> 
This test verifies the VLAN function around untag and access ports.

*p.s. This test will not check function with the native VLAN scenario. Please make sure the native VLAN will not impact the result.*

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

### Test steps: <!-- omit in toc --> 
Test Steps:
  - Create ``Untagged``/``Tagged VLAN 1000`` packet with ``mac2`` as dest mac.
  - Send packet on Port1.
  - Verify ``Untagged`` packet received port2.

## Test Group: Frame Filtering
### Case3: test_untagged_frame_filtering
### Case4: test_tagged_frame_filtering
### Testing Objective <!-- omit in toc --> 

Drop packet when the destination port from MAC table search is the port which packet come into the switch.


```
Test example(Untag):                                
                                                                                  | MAC1 |
  pkt(Untag:DMAC=MAC1 or MACX) -> Port1:Access:VLAN1000 -> FDB(contains:MAC1,MACX)|      | -> X 
                                                                                  | MACX |
```

### Test steps <!-- omit in toc --> 
  - Add another ``MacX`` as Port1 mac address into MAC table
  - Create ``Untagged``/``Tagged VLAN1000`` packet, ``mac1`` as src mac, ``MacX`` as dest mac
  - Send packet on VLAN Port.
  - Verify no packet received on any port.

## Test Group: VLAN flooding
### Case5: test_tagged_vlan_flooding
### Case6: test_untagged_vlan_flooding
### Testing Objective <!-- omit in toc --> 
For mac flooding in VLAN scenario, before learning the mac address from the packet, the packet sends to the VLAN port will flood to other ports, the egress ports will be in the same VLAN as ingress port.
```
Flooding                                 
                                                             | Port2|
 pkt(Untag:DMAC=MAC2) -> Port1:Access:VLAN1000 -> Flooding ->|  To  |-> pkt(Untag)
                                                             | Port8|
```


### Test Steps: <!-- omit in toc --> 
  - Create ``Untagged``/``Tagged VLAN1000`` packet, with ``mac1`` as source MAC and a un-existing ``MacX`` as dest MAC
  - Send packet on Port1.
  - Verify received packet on all port expect Port1.

## Test Group: MAC learning
### Case7: test_untagged_mac_learning
### Case8: test_tagged_mac_learning
### Testing Objective <!-- omit in toc --> 
For mac learning in VLAN scenario, after learning the mac address from the packet, the packet sends to the VLAN port will only sends to the port whose MAC address matching the MAC table entry.

```
 unicast 
  pkt(Untag:DMAC=MAC1)  -> Port2:Access:VLAN1000-> Port1:Access:VLAN1000 -> pkt(Untag:DMAC=MAC1)

```
### Test Steps: <!-- omit in toc --> 
  - Create ``Untagged``/``Tagged VLAN1000`` packet, with a un-existing ``MacX`` as src MAC and ``mac2`` as dest MAC
  - Send packet on a VLAN source port1.
  - Verify packet received on port2.
  - Verify MAC table get a new entry for ``MacX`` on port1, 


## Test Case9: Vlan member API
  - test_vlan_member_api

### Testing Objective <!-- omit in toc --> 

Test VLAN and member APIs.

### Test steps: <!-- omit in toc --> 
Test steps:
  - Use VLAN API to list all of the VLAN member in VLAN
  - Verify the VLAN member and the account of the VLAN member are same as config in [config](./config_data/config_t0.md)
  - Use VLAN API add a new Member, into VLAN
  - Verify the VLAN member and the account of the VLAN member are increased by 1.

## Test Case10: Failed to add member to invalidate VLAN
  - test_add_vlan_member_failed
### Testing Objective <!-- omit in toc --> 

When add a VLAN member to a non-exist VLAN, it will fail.

### Test Steps: <!-- omit in toc --> 
  - Use VLAN API add a new Member to a non-exist VLAN, ``VLAN1010``
  - Verify the VLAN member added failed.

## Test Group: VLAN Counters/Status
### Case11: test_tagged_vlan_status
### Case12: test_untagged_vlan_status
### Testing Objective <!-- omit in toc --> 

For VLAN-related counters, SAI should be able to get the counter and clear them.

### Test Steps: <!-- omit in toc --> 
Steps:
  - Use SAI API to get the VLAN Status ``_sai_vlan_stat_t`` from [VLAN_HEADER](https://github.com/opencomputeproject/SAI/blob/master/inc/saivlan.h) 
  - Create ``Untagged``/``Tagged VLAN1000`` packet, with ``mac1`` as src MAC and ``mac2`` as dest MAC
  - Send packet from the source port1
  - Verify packet received on the target port2
  - Verify counters increased, bytes counter: OCTETS increased, other counters + 1
  - Use VLAN clear status API
  - Verify counters have been reset

## Test Group7: Disable mac learning
### Case13: test_disable_mac_learning_tagged
### Case14: test_disable_mac_learning_untagged
### Testing Objective <!-- omit in toc --> 
Test the function when disabling VLAN MAC learning.
When disabled, no new MAC will be learned in the MAC table.

### Additional config: <!-- omit in toc --> 
- Do not config the MAC table

### Test steps: <!-- omit in toc --> 
  - Create ``Untagged``/``Tagged VLAN1000`` packet, with ``mac1`` as src MAC and ``mac2`` as dest MAC
  - Send packet from the source port1.
  - Verify packets received on all ports except the source port1.
  - Verify MAC table, no new entry added in MAC table

## Test Case15: L3 switching(Inter-VLAN) 
### Testing Objective <!-- omit in toc --> 

Testing L3 switching(Inter-VLAN).

In this case, it will cover the scenarios for Server to Server (east/west) in different VLAN(Inter-VLAN)

```
Server To Server
  PC1 -> pkt(Untag) -> |DUT|Port1:Access:VLAN1000 
                                              |L3| <-> |Virtual Switch| 
  PC2 <- pkt(Untag) <- |DUT|Port9:Access:VLAN2000
```

The process is as below:
1. PC1 sends a untag packet to port1, which is a VLAN1000 access port
2. Based on packet dest IP, derive the route to the dest IP, Dest MAC and Port derived from L3 table. Then SRC MAC change to Switch MAC, Dest MAC change to PORT MAC, forwarding to PORT9
3. Packet goes through VLAN2000 port9

### Test Data/Packet <!-- omit in toc --> 
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
### Test Steps: <!-- omit in toc --> 
  - Create Packet, with with dest ``SVI_MAC`` as dest mac, and dest IP ``192.168.20.21``(config as [Config_t0](./config_data/config_t0.md))
  - Send ``Untagged`` packet on ``port1``
  - Verify ``Untagged`` packet received on ``PORT9``

## Test Group: ARP Flooding and mac learning
### Case16: test_arp_request_flooding
### Case17: test_arp_response_learning
### Testing Objective <!-- omit in toc --> 
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
### Test Data/Packet <!-- omit in toc --> 
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

### Additional config: <!-- omit in toc --> 
- Do not config the MAC table

### Test Steps: <!-- omit in toc --> 
- test_arp_request_flooding
  - Create ARP packet with dest ``mac2``
  - Send ``Untagged`` ARP packet on ``port1``
  - Verify ``Untagged`` Arp request received from Port2 to Port8
- test_arp_response_learning
  - Create ARP response packet with src:mac2, dest:mac1
  - Send ARP response packet on port2
  - Verify ``Untagged`` ARP response from Port1
