# SAI FDB Test plan <!-- omit in toc --> 

- [Test Configuration](#test-configuration)
- [Test Execution](#test-execution)
- [Test Execution](#test-execution-1)
  - [Common Test Data/Packet](#common-test-datapacket)
  - [Test Group1: MAC learning](#test-group1-mac-learning)
    - [Case1: test_vlan_port_learn_disable](#case1-test_vlan_port_learn_disable)
    - [Case2: test_bg_port_learn_disable](#case2-test_bg_port_learn_disable)
    - [Case3: test_non_bgPort_no_learn](#case3-test_non_bgport_no_learn)
    - [Case4: test_new_vlan_member_learn](#case4-test_new_vlan_member_learn)
    - [Case5: test_remove_vlan_member_no_learn](#case5-test_remove_vlan_member_no_learn)
    - [Case6: test_no_learn_invalidate_vlan](#case6-test_no_learn_invalidate_vlan)
    - [Case7: test_no_learn_broadcast_src](#case7-test_no_learn_broadcast_src)
    - [Case8: test_no_learn_multicast_src](#case8-test_no_learn_multicast_src)
  - [Test Group2: FDB age](#test-group2-fdb-age)
    - [Case1: test_port_age](#case1-test_port_age)
    - [Case2: test_aging_after_move](#case2-test_aging_after_move)
    - [Case3: test_mac_moving_after_aging](#case3-test_mac_moving_after_aging)
  - [Test Group3: FDB flush](#test-group3-fdb-flush)
    - [Case1: test_flush_vlan_static](#case1-test_flush_vlan_static)
    - [Case2: test_flush_vlan_dynamic](#case2-test_flush_vlan_dynamic)
    - [Case3: test_flush_port_static](#case3-test_flush_port_static)
    - [Case4: test_flush_port_dynamic](#case4-test_flush_port_dynamic)
    - [Case5: test_flush_all_static](#case5-test_flush_all_static)
    - [Case6: test_flush_all_dynamic](#case6-test_flush_all_dynamic)
    - [Case7: test_flush_all](#case7-test_flush_all)
  - [Test Group4: MAC move](#test-group4-mac-move)
    - [Case1: test_disable_move_drop](#case1-test_disable_move_drop)
    - [Case2: test_dynamic_mac_move](#case2-test_dynamic_mac_move)
    - [Case3: test_static_mac_move](#case3-test_static_mac_move)

# Test Configuration

For the test configuration, please refer to the file 
  - [Config_t0](./config_data/config_t0.md)
  
**Note. All the tests will be based on the configuration above, if any additional configuration is required, it will be specified in the Test case.**

# Test Execution

# Test Execution

## Common Test Data/Packet
In this FDB test, the example packet structure is below.
- Simple L2 packet
   ```python
    simple_udp_packet(eth_dst=dmac,
                      eth_src=smac)
   ```
- VLAN
  ```Python
    simple_udp_packet(eth_dst=dmsc,
                      eth_src=smac,
                      vlan_vid=lvlan_id)
  ```

  **Note. If need other kinds of packets, they will be added to the test case/group respectively.**

## Test Group1: MAC learning 
### Case1: test_vlan_port_learn_disable
### Case2: test_bg_port_learn_disable
### Case3: test_non_bgPort_no_learn
### Case4: test_new_vlan_member_learn
### Case5: test_remove_vlan_member_no_learn
### Case6: test_no_learn_invalidate_vlan
### Case7: test_no_learn_broadcast_src
### Case8: test_no_learn_multicast_src

### Testing Objective <!-- omit in toc --> 
- test_vlan_port_learn_disable: Verify if MAC addresses are not learned on the port when VLAN port is disabled.
- test_bg_port_learn_disable: Verify if MAC addresses are not learned on the port whenbridge port learning is disabled.
- test_non_bgPort_no_learn: Verify if MAC addresses are not learned on the port when the port is not a bridge port.
- test_new_vlan_member_learn: Verify newly added VLAN members can learn.
- test_remove_vlan_member_no_learn: Verify no MAC addresses are learned on the removed vlan member.
- test_no_learn_invalidate_vlan: Verify no MAC addresses are learned on invalidate vlan ID.
- test_no_learn_broadcast_src: Verify broadcast mac address is learned.
- test_no_learn_multicast_src: Verify multicast mac address is learned.

### Test steps: <!-- omit in toc --> 

- test_vlan_port_learn_disable
- test_bg_port_learn_disable
- test_non_bgPort_no_learn

1. Flush all MAC
2. Disable MAC learn on VLAN10/Port1(Bridge Port1) and Removed Port1 from Bridge Port1 for each case
3. Create a packet with SMAC ``MacX``
4. send packet from port1
5. Verify the packet flood to other VLAN10 ports
6. Create a packet with DMAC ``MacX``
7. send the packet on port2
8. Verify the packet flood to other VLAN10 ports, including port1
9. check FDB entries, no new entry

- test_new_vlan_member_learn

1. Add Port24 to VLAN10
2. Create Packet with SMAC=``MacX`` DMAC=``Port1 MAC``
3. Send packet on port24
4. verify only receive a packet on port1
5. Create a packet with DMAC=``MacX``
6. Send packet on port1
7. Verify only receive a packet on port24
8. check FDB entries, new entry ``MacX`` on Port24 learned

- test_remove_vlan_member_learn

1. Remove Port2 from VLAN10
2. Create a flood Packet with SMAC=``MacX`` and VLAN10 tag
3. Send packet on port2
4. Verify no packet was received on any port
5. Create a packet with DMAC=``MacX`` and VLAN10 tag
6. Send packet on port1
7. Verify flooding to VLAN10 ports, no packet on port2
8. check FDB entries, no new entry

- test_no_learn_invalidate_vlan
- test_no_learn_broadcast_src
- test_no_learn_multicast_src

1. Create a packet with vlan_id=``VLAN11`` SMAC=``MacX``/SMAC=``broadcast address``/SMAC=``multicast address``
2. Send packet on port2
3. Verify no packet was received on any port
4. Create a packet with vlan_id=``VLAN11`` DMAC=``MacX``/DMAC=``broadcast address``/DMAC=``multicast address``
5. Send packet on port1
6. Dropped for ``VLAN11``, For broadcast and multicast address, flooding on all vlan10 ports, except port1
7. check FDB entries, no new entry


## Test Group2: FDB age
### Case1: test_port_age
### Case2: test_aging_after_move
### Case3: test_mac_moving_after_aging
### Testing Objective <!-- omit in toc -->
- test_port_age: Verifying if the dynamic FDB entry associated with the port is removed after the aging interval.
- test_aging_after_move: Verifying the aging time refreshed if dynamic FDB entry associated with one port and then moved to another port (not the initial learning time)
- test_mac_moving_after_aging: Verifying the mac can be learnt again after the mac aging reached.


### Test steps: <!-- omit in toc --> 
- test_port_age

1. Set FDB aging time=10
2. Create Packet with SMAC=``MacX`` DMAC=``Port1 MAC`` 
3. Send packet on port2
4. verify only receive a packet on port1
5. Create a packet with DMAC=``MacX``
6. Send packet on port1
7. Verify only receive a packet on port2
8. Wait for the ``aging`` time
9. Send packet on port1
10. Verify flooding packet to VLAN10 ports, except port1

- test_aging_after_move

1. Set FDB aging time=10
2. Create Packet with SMAC=``MacX`` DMAC=``Port1 MAC`` 
3. Send packet on port2
4. verify only receive a packet on port1
5. Create a packet with DMAC=``MacX``
6. Send packet on port1
7. Verify only receive a packet on port2
8. Send packet on port3
7. Verify only receive a packet on port2
9. Wait for the ``aging`` time
10. Send packet on port3
11. Verify flooding packet to VLAN10 ports, except port1

- test_mac_moving_after_aging

1. Set FDB aging time=10
2. Create Packet with SMAC=``MacX`` DMAC=``Port1 MAC`` 
3. Send packet on port2
4. verify only receive a packet on port1
5. Create a packet with DMAC=``MacX``
6. Send packet on port1
7. Verify only receive a packet on port2
8. Wait for the ``aging`` time
9. Send packet on port3
10. Verify only receive a packet on port2

## Test Group3: FDB flush
### Case1: test_flush_vlan_static
### Case2: test_flush_vlan_dynamic
### Case3: test_flush_port_static
### Case4: test_flush_port_dynamic
### Case5: test_flush_all_static
### Case6: test_flush_all_dynamic
### Case7: test_flush_all

### Testing Objective <!-- omit in toc -->
Verify flushing of static/dynamic entries on VLAN/Port/All.
### Test steps: <!-- omit in toc --> 
- test_flush_vlan_static
- test_flush_port_static
- test_flush_all_static

1. Flush with conditions for each case: ``Static`` flush on ``VLAN10``; ``Static`` flush on ``Port1``; flush for all ``Static`` 
2. Send packets for each case in sequence: ``port1`` DMAC=``Port2 MAC``; ``Port2`` DMAC=``Port1 MAC``; ``port1`` DMAC=``Port2 MAC``
3. Verify flooding happened, packets received in related VLAN, except the ingress port.
4. Send packets for each case in sequence:  ``Port9`` DMAC=``Port10 MAC``; ``Port10`` DMAC=``Port9 MAC``; `  ``Port9`` DMAC=``Port10 MAC``
5. Verify flush happens in a certain domain, unicast to the corresponding port.

- test_flush_vlan_dynamic
- test_flush_port_dynamic
- test_flush_all_dynamic
  
1. Flush with conditions for each case in sequence: ``Dynamic`` flush on ``VLAN20``;  ``Dynamic`` flush on ``Port9``; flush for all ``Dynamic`` 
2. Send packets for each case in sequence: ``Port9`` DMAC=``Port10 MAC``;  ``Port10`` DMAC=``Port9 MAC``; ``Port9`` DMAC=``Port10 MAC``
3. Verify flooding happened, packets received in related VLAN, except the ingress port.
4. Send packets for each case in sequence:  ``port1`` DMAC=``Port2 MAC``;``Port2`` DMAC=``Port1 MAC``; ``port1`` DMAC=``Port2 MAC``;
5.  Verify flush happens in a certain domain, unicast to the corresponding port.

- test_flush_all

1. Flush with conditions: flush for ``All``
2. Send packets : ``port1`` DMAC=``Port2 MAC``; ``Port9`` DMAC=``Port10 MAC``; 
3. Verify flooding happened, packets received in related VLAN, except the ingress port.


## Test Group4: MAC move
### Case1: test_disable_move_drop
### Case2: test_dynamic_mac_move
### Case3: test_static_mac_move
### Testing Objective <!-- omit in toc --> 
- test_disable_move_drop: Verify if disable MAC move, drop packet with known SMAC if the SMAC was already learnt on other port.
- test_dynamic_mac_move: Verify when enabling MAC move, previous learnt mac(SMAC) on a port can be learnt on other port
- test_static_mac_move: Verify when enabling MAC move, previous installed mac(static SMAC) on a port can be set to other port

### Test steps: <!-- omit in toc --> 
- test_disable_move_drop

1. Disable mac move for ``Port1 MAC`` on Port1
2. Create a packet with SMAC=``Port1 MAC`` DMAC=``Port2 MAC``
3. Send packet on port1
4. Verify packet received on port2
5. Send packet in step2 on port3
6. Verify the packet gets dropped

- test_dynamic_mac_move
  
1. Flush All MAC
2. Install static FDB entry for port2 with ``Port2 MAC``
3. Send Packet on Port1 with SMAC=``Port1 MAC`` DMAC=``Port2 MAC``
4. Create packet with SMAC=``Port1 MAC`` DMAC=``Port2 MAC``
5. Send packet on port1
6. Install static mac move for ``Port1 MAC`` on Port1 and enable mac move
7. Verify packet received on port2
8. Send packet in step2 on port3
9. Verify packet received on port2

- test_static_mac_move
  
1. Flush All MAC
2. Install static FDB entry for port2 with ``Port2 MAC``
3. Install static FDB entry for port1 with ``Port1 MAC``  
4. Enable mac move for ``Port1 MAC`` on Port1
5. Create packet with SMAC=``Port1 MAC`` DMAC=``Port2 MAC``
6. Send packet on port1
7. Verify packet received on port2
8. Install static FDB entry for port3 with ``Port1 MAC``  
9. Send packet in step2 on port3
10. Verify packet received on port2
