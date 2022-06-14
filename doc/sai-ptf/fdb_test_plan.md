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
  - [Test Group2: MAC move](#test-group2-mac-move)
    - [Case9:  test_disable_move_drop](#case9--test_disable_move_drop)
    - [Case10: test_dynamic_mac_move](#case10-test_dynamic_mac_move)
    - [Case11: test_static_mac_move](#case11-test_static_mac_move)
  - [Test Group3: FDB age](#test-group3-fdb-age)
    - [Case12: test_port_age](#case12-test_port_age)
    - [Case13: test_aging_after_move](#case13-test_aging_after_move)
  - [Test Group4: FDB flush](#test-group4-fdb-flush)
    - [Case14: test_flush_vlan_static](#case14-test_flush_vlan_static)
    - [Case15: test_flush_vlan_dynamic](#case15-test_flush_vlan_dynamic)
    - [Case16: test_flush_port_static](#case16-test_flush_port_static)
    - [Case17: test_flush_port_dynamic](#case17-test_flush_port_dynamic)
    - [Case18: test_flush_all_static](#case18-test_flush_all_static)
    - [Case19: test_flush_all_dynamic](#case19-test_flush_all_dynamic)
    - [Case20: test_flush_all](#case20-test_flush_all)
  - [Test Group5: FDB miss](#test-group5-fdb-miss)
    - [Case21: test_unicast_action_copy](#case21-test_unicast_action_copy)
    - [Case22: test_unicast_action_trap](#case22-test_unicast_action_trap)
    - [Case23: test_unicast_action_drop](#case23-test_unicast_action_drop)
    - [Case24: test_multicast_action_copy](#case24-test_multicast_action_copy)
    - [Case25: test_multicast_action_trap](#case25-test_multicast_action_trap)
    - [Case26: test_multicast_action_drop](#case26-test_multicast_action_drop)
    - [Case27: test_broadcast_action_copy](#case27-test_broadcast_action_copy)
    - [Case28: test_broadcast_action_trap](#case28-test_broadcast_action_trap)
    - [Case29: test_broadcast_action_drop](#case29-test_broadcast_action_drop)

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
Verify if MAC addresses are not learned on the port when VLAN port or bridge port learning is disabled.
Verify if MAC addresses are not learned on the port when the port is not a bridge port.
Verify newly added VLAN members can learn.


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

- test_new_vlan_member_learn

1. Add Port24 to VLAN10
2. Create Packet with SMAC=``MacX`` DMAC=``Port1 MAC``
3. Send packet on port24
4. verify only receive a packet on port1
5. Create a packet with DMAC=``MacX``
6. Send packet on port1
7. Verify only receive a packet on port24

- test_remove_vlan_member_learn

1. Remove Port2 from VLAN10
2. Create a flood Packet with SMAC=``MacX`` and VLAN10 tag
3. Send packet on port2
4. Verify no packet was received on any port
5. Create a packet with DMAC=``MacX`` and VLAN10 tag
6. Send packet on port1
7. Verify flooding to VLAN10 ports, no packet on port2

- test_no_learn_invalidate_vlan
- test_no_learn_broadcast_src
- test_no_learn_multicast_src

1. Create a packet with vlan_id=``VLAN11`` SMAC=``MacX``/SMAC=``broadcast address``/SMAC=``multicast address``
2. Send packet on port2
3. Verify no packet was received on any port
4. Create a packet with vlan_id=``VLAN11`` DMAC=``MacX``/DMAC=``broadcast address``/DMAC=``multicast address``
5. Send packet on port1
6. Flooding on all vlan10 ports, except port1


## Test Group2: MAC move
### Case9:  test_disable_move_drop
### Case10: test_dynamic_mac_move
### Case11: test_static_mac_move
### Testing Objective <!-- omit in toc --> 
Verify if disable MAC move, drop packet with known SMAC, the SMAC was learned previously on other port.
Verify when enabling MAC move, if after receiving a packet with known SMAC, but from another port that on which, it was learned previously, next packets with such DMACs are forwarded to the new port.

### Test steps: <!-- omit in toc --> 
- test_disable_move_drop

1. Disable mac move for ``Port1 MAC`` on Port1
2. Create a packet with SMAC=``Port1 MAC`` DMAC=``Port2 MAC``
3. Send packet on port1
4. Verify packet received on port2
5. Send packet in step2 on port3
6. Verify the packet gets dropped

- test_dynamic_mac_move
- test_static_mac_move
  
1. Flush All MAC
2. Install dynamic/static ``Port1 MAC`` address for port1
3. Enable mac move for ``Port1 MAC`` on Port1
4. Create packet with SMAC=``Port1 MAC`` DMAC=``Port2 MAC``
5. Send packet on port1
6. Verify packet received on port2
7. Send packet in step2 on port3
8. Verify packet received on port2

## Test Group3: FDB age
### Case12: test_port_age
### Case13: test_aging_after_move
### Testing Objective <!-- omit in toc -->
Verifying if the dynamic FDB entry associated with the port is removed after the aging interval.
Verifying the aging time refreshed if dynamic FDB entry associated with one port and then moved to another port (not the initial learning time)


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
8. Wait for the ``aging`` time
9. Send packet on port1
10. Verify flooding packet to VLAN10 ports, except port1

## Test Group4: FDB flush
### Case14: test_flush_vlan_static
### Case15: test_flush_vlan_dynamic
### Case16: test_flush_port_static
### Case17: test_flush_port_dynamic
### Case18: test_flush_all_static
### Case19: test_flush_all_dynamic
### Case20: test_flush_all

### Testing Objective <!-- omit in toc -->
Verify flushing of static/dynamic entries on VLAN/Port/All.
### Test steps: <!-- omit in toc --> 

1. Flush with conditions for each case in sequence: ``Static`` on ``VLAN10``; ``Dynamic`` on ``VLAN20``; ``Static`` on ``Port1``;  ``Dynamic`` on ``Port9``; ``Static``; ``Dynamic``; ``All``
2. Send packets for each case in sequence: ``port1`` DMAC=``Port2 MAC``; ``Port9`` DMAC=``Port10 MAC``; ``Port2`` DMAC=``Port1 MAC``; ``Port10`` DMAC=``Port9 MAC``; ``port1`` DMAC=``Port2 MAC``; ``Port9`` DMAC=``Port10 MAC``
3. Verify flooding happened, packets received in related VLAN, except the ingress port.
4. Send packets for each case in sequence:  ``Port9`` DMAC=``Port10 MAC``; ``port1`` DMAC=``Port2 MAC``;``Port10`` DMAC=``Port9 MAC``; ``Port2`` DMAC=``Port1 MAC``;  ``Port9`` DMAC=``Port10 MAC``;``port1`` DMAC=``Port2 MAC``;
5. Verify unicast to the corresponding port.



## Test Group5: FDB miss
### Case21: test_unicast_action_copy
### Case22: test_unicast_action_trap
### Case23: test_unicast_action_drop
### Case24: test_multicast_action_copy
### Case25: test_multicast_action_trap
### Case26: test_multicast_action_drop
### Case27: test_broadcast_action_copy
### Case28: test_broadcast_action_trap
### Case29: test_broadcast_action_drop

### Testing Objective <!-- omit in toc --> 
Verify if unicast/multicast/broadcast packets are dropped or redirected/copy to the CPU after setting miss packet action to drop, trap or copy.

### Test steps: <!-- omit in toc --> 
- test_unicast_action_drop
- test_multicast_action_drop
- test_broadcast_action_drop

1. Set the FDB unicast/multicast/broadcast missing action to SAI_PACKET_ACTION_DROP 
2. Create a packet with DMAC as unknown ``MacX``, ``Multicase MAC`` or ``Broadcast MAC`` for each case
3. Send the packet to port1
4. Verify the packet gets dropped

- test_unicast_action_trap
- test_multicast_action_trap
- test_broadcast_action_trap

1. Set the FDB unicast/multicast/broadcast missing action to SAI_PACKET_ACTION_TRAP 
2. Create a packet with DMAC as unknown ``MacX``, ``Multicase MAC`` or ``Broadcast MAC`` for each case
3. Get the queue status SAI_QUEUE_STAT_PACKETS
4. Send the packet on port1
5. Verify the packet gets dropped
6. Check the SAI_QUEUE_STAT_PACKETS increased by 1

- test_unicast_action_copy
- test_multicast_action_copy
- test_broadcast_action_copy

1. Set the FDB unicast/multicast/broadcast missing action to SAI_PACKET_ACTION_TRAP 
2. Create a packet with DMAC as unknown ``MacX``, ``Multicase MAC`` or ``Broadcast MAC`` for each case
3. Get the queue status SAI_QUEUE_STAT_PACKETS
4. Send the packet on port1
5. Verify the packet flooding to vlan10 ports, except port1
6. Check the SAI_QUEUE_STAT_PACKETS increased by 1
