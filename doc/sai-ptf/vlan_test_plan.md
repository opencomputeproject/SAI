# SAI VLAN Test plan <!-- omit in toc --> 

- [Test Configuration](#test-configuration)
- [Test Execution](#test-execution)
  - [Common Test Data/Packet](#common-test-datapacket)
  - [Test Group1: Tagging and access](#test-group1-tagging-and-access)
    - [Case1: test_untag_access_to_access](#case1-test_untag_access_to_access)
    - [Case2: test_unmatch_drop](#case2-test_unmatch_drop)
  - [Test Group2: Frame Filtering](#test-group2-frame-filtering)
    - [Case3: test_untagged_frame_filtering](#case3-test_untagged_frame_filtering)
    - [Case4: test_tagged_frame_filtering](#case4-test_tagged_frame_filtering)
  - [Test Group3: VLAN flooding](#test-group3-vlan-flooding)
    - [Case5: test_tagged_vlan_flooding](#case5-test_tagged_vlan_flooding)
    - [Case6: test_untagged_vlan_flooding](#case6-test_untagged_vlan_flooding)
  - [Test Group4: VLAN broadcast](#test-group4-vlan-broadcast)
    - [Case7: test_vlan_broadcast](#case7-test_vlan_broadcast)
  - [Test Group5: MAC learning](#test-group5-mac-learning)
    - [Case8: test_untagged_mac_learning](#case8-test_untagged_mac_learning)
    - [Case9: test_tagged_mac_learning](#case9-test_tagged_mac_learning)
  - [Test Group6: Vlan member API](#test-group6-vlan-member-api)
    - [Case10:test_vlan_member_api](#case10test_vlan_member_api)
  - [Test Group7: Add member to invalidate VLAN](#test-group7-add-member-to-invalidate-vlan)
    - [Case11:test_add_vlan_member_failed](#case11test_add_vlan_member_failed)
  - [Test Group8: Disable mac learning](#test-group8-disable-mac-learning)
    - [Case12: test_disable_mac_learning_tagged](#case12-test_disable_mac_learning_tagged)
    - [Case13: test_disable_mac_learning_untagged](#case13-test_disable_mac_learning_untagged)
  - [Test Group9: ARP Flooding and mac learning](#test-group9-arp-flooding-and-mac-learning)
    - [Case14: test_arp_request_flooding](#case14-test_arp_request_flooding)
    - [Case15: test_arp_response_learning](#case15-test_arp_response_learning)
  - [Test Group10: VLAN Counters/Status](#test-group10-vlan-countersstatus)
    - [Case16: test_tagged_vlan_status](#case16-test_tagged_vlan_status)
    - [Case17: test_untagged_vlan_status](#case17-test_untagged_vlan_status)
# Test Configuration

For the test configuration, please refer to the file 
  - [Config_t0](./config_data/config_t0.md)
  
**Note. All the tests will be based on the configuration above, if any additional configuration is required, it will be specified in the Test case.**

# Test Execution

## Common Test Data/Packet
In this VLAN test, most of the test cases will be tested with a Tagged or Untagged packet, the example packet structure as below.
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
**Note. If need other kinds of packets, they will be added to the test case/group respectively.**

## Test Group1: Tagging and access 
### Case1: test_untag_access_to_access
### Case2: test_unmatch_drop

### Testing Objective <!-- omit in toc --> 
This test verifies the VLAN function around untag and access ports.

*p.s. This test will not check the function with the native VLAN scenario. Please make sure the native VLAN will not impact the result.*

With an untagged packet, on the access port, when ingress and egress happen, the behavior as below
| Port mode | packet tag mode | Direction | Action                                   |
| :---------|--------------- | :-------- | :--------------------------------------- |
| Access|Untag| Ingress   | Accept the packet.       |
|       |Untag | Egress    | Untag the packet. |


```
Test example:
Untagged:                                    
  pkt(Untag:DMAC=MAC2)  -> Port1:Access:VLAN1000-> Port2:Access:VLAN1000 -> pkt(Untag:DMAC=MAC2)
```

### Test steps: <!-- omit in toc --> 
- test_untag_access_to_access

1. Create ``Untagged`` packet with ``mac2`` as dest mac.
2. Send packet on Port1.
3. Verify ``Untagged`` packet received port2.

- test_unmatch_drop

1. Create ``Tagged VLAN 1010`` packet with ``mac2`` as dest mac.
2. Send packet on Port1.
3. Verify packet is dropped.

## Test Group2: Frame Filtering
### Case3: test_untagged_frame_filtering
### Case4: test_tagged_frame_filtering
### Testing Objective <!-- omit in toc --> 

Drop packet when the destination port from MAC table search is the port which packet comes into the switch.


```
Test example(Untag):                                
                                                                                  | MAC1 |
  pkt(Untag:DMAC=MAC1 or MACX) -> Port1:Access:VLAN1000 -> FDB(contains:MAC1,MACX)|      | -> X 
                                                                                  | MACX |
```

### Test steps <!-- omit in toc --> 
  1. Add another ``MacX`` as Port1 mac address into the MAC table
  1. Create ``Untagged``/``Tagged VLAN1000`` packet, ``mac1`` as src mac, ``MacX`` as dest mac
  1. Send packet on VLAN Port.
  1. Verify no packet was received on any port.

## Test Group3: VLAN flooding
### Case5: test_tagged_vlan_flooding
### Case6: test_untagged_vlan_flooding
### Testing Objective <!-- omit in toc --> 
For mac flooding in the VLAN scenario, before learning the mac address from the packet, the packet sent to the VLAN port will flood to other ports, and the egress ports will be in the same VLAN as the ingress port.
```
Flooding                                 
                                                             | Port2|
 pkt(Untag:DMAC=MAC2) -> Port1:Access:VLAN1000 -> Flooding ->|  To  |-> pkt(Untag)
                                                             | Port8|
```


### Test Steps: <!-- omit in toc --> 
  1. Create ``Untagged``/``Tagged VLAN1000`` packet, with ``mac1`` as source MAC and an un-existing ``MacX`` as dest MAC
  1. Send packet on Port1.
  1. Verify received packet on all VLAN 1000 ports expect Port1.


## Test Group4: VLAN broadcast
### Case7: test_vlan_broadcast
### Testing Objective <!-- omit in toc --> 
A VLAN is a logical broadcast domain that can span multiple physical LAN segments. 


### Test Steps: <!-- omit in toc --> 
  1. Create ``Untagged``/``Tagged VLAN1000`` packet, with ``broadcast`` MAC as dest MAC
  2. Send packet on Port1.
  3. Verify received packet on all VLAN 1000 ports expect Port1.

## Test Group5: MAC learning
### Case8: test_untagged_mac_learning
### Case9: test_tagged_mac_learning
### Testing Objective <!-- omit in toc --> 
For mac learning in the VLAN scenario, after learning the mac address from the packet, the packet sent to the VLAN port will only send to the port whose MAC address matches the MAC table entry.

```
 unicast 
  pkt(Untag:DMAC=MAC1)  -> Port2:Access:VLAN1000-> Port1:Access:VLAN1000 -> pkt(Untag:DMAC=MAC1)

```
### Test Steps: <!-- omit in toc --> 
  1. Create ``Untagged``/``Tagged VLAN1000`` packet, with an un-existing ``MacX`` as src MAC and ``mac2`` as dest MAC
  1. Send packet on a VLAN source port1.
  1. Verify packet received on port2.
  1. Verify MAC table get a new entry for ``MacX`` on port1, 

## Test Group6: Vlan member API
### Case10:test_vlan_member_api

### Testing Objective <!-- omit in toc --> 

Test VLAN and member APIs.

### Test steps: <!-- omit in toc --> 
  1. Use VLAN API to list all of the VLAN members in VLAN
  1. Verify the VLAN member and the account of the VLAN member are the same as config in [config](./config_data/config_t0.md)
  1. Use VLAN API to add a new Member to VLAN
  1. Verify the VLAN member and the account of the VLAN member is increased by 1.

## Test Group7: Add member to invalidate VLAN
### Case11:test_add_vlan_member_failed
### Testing Objective <!-- omit in toc --> 

When adding a VLAN member to a non-exist VLAN, it will fail.

### Test Steps: <!-- omit in toc --> 
  1. Use VLAN API to add a new Member to a non-exist VLAN, ``VLAN1010``
  1. Verify the VLAN member added failed.


## Test Group8: Disable mac learning
### Case12: test_disable_mac_learning_tagged
### Case13: test_disable_mac_learning_untagged
### Testing Objective <!-- omit in toc --> 
Test the function when disabling VLAN MAC learning.
When disabled, no new MAC will be learned in the MAC table.

### Additional config: <!-- omit in toc --> 
- Do not config the MAC table

### Test steps: <!-- omit in toc --> 
  1. Create ``Untagged``/``Tagged VLAN1000`` packet, with ``mac1`` as src MAC and ``mac2`` as dest MAC
  1. Send packet from the source port1.
  1. Verify packets received on all ports except the source port1.
  1. Verify MAC table, no new entry added in MAC table


## Test Group9: ARP Flooding and mac learning
### Case14: test_arp_request_flooding
### Case15: test_arp_response_learning
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


1. Create ARP packet with dest ``mac2``
2. Send ``Untagged`` ARP packet on ``port1``
3. Verify ``Untagged`` Arp request received from Port2 to Port8

- test_arp_response_learning

1. Create ARP response packet with src:mac2, dest:mac1
2. Send ARP response packet on port2
3. Verify ``Untagged`` ARP response from Port1


## Test Group10: VLAN Counters/Status
### Case16: test_tagged_vlan_status
### Case17: test_untagged_vlan_status
### Testing Objective <!-- omit in toc --> 

For VLAN-related counters, SAI should be able to get the counter and clear them.

### Test Steps: <!-- omit in toc --> 
  1. Use SAI API to get the VLAN Status ``_sai_vlan_stat_t`` (defined in [saivlan.h](https://github.com/opencomputeproject/SAI/blob/master/inc/saivlan.h) )
  2. Create ``Untagged``/``Tagged VLAN1000`` packet, with ``mac1`` as src MAC and ``mac2`` as dest MAC
  3. Send packet from the source port1
  4. Verify the packet received on the target port2
  5. Verify counters increased, bytes counter: OCTETS increased, other counters + 1
  6. Use VLAN clear status API
  7. Verify counters have been reset