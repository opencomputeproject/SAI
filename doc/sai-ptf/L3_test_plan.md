# SAI L3 Test plan <!-- omit in toc --> 

- [Test Configuration](#test-configuration)
- [Test Execution](#test-execution)
  - [Test Group1: Route](#test-group1-route)
    - [Case1: test_route_rif_v4](#case1-test_route_rif_v4)
    - [Case2: test_drop_route_v4](#case2-test_drop_route_v4)
    - [Case3: test_route_update_v4](#case3-test_route_update_v4)
    - [Case4: test_lag_multiple_route_v4](#case4-test_lag_multiple_route_v4)
    - [Case5: test_svi_route_l3_v4](#case5-test_svi_route_l3_v4)
    - [Case6: test_svi_learning_v4](#case6-test_svi_learning_v4)
    - [Case7: test_svi_flooding_v4](#case7-test_svi_flooding_v4)
    - [Case8: test_svi_mac_move_v4](#case8-test_svi_mac_move_v4)
    - [Case9: test_svi_mac_move_stress_v4](#case9-test_svi_mac_move_stress_v4)
    - [Case10: test_svi_mac_age_v4](#case10-test_svi_mac_age_v4)
    - [Case11: test_svi_mac_age_after_move_v4](#case11-test_svi_mac_age_after_move_v4)
    - [Case12: test_svi_dierctbroadcast_v4](#case12-test_svi_dierctbroadcast_v4)
    - [Case13: test_route_rif_v6](#case13-test_route_rif_v6)
    - [Case14: test_drop_route_v6](#case14-test_drop_route_v6)
    - [Case15: test_route_update_v6](#case15-test_route_update_v6)
    - [Case16: test_lag_multiple_route_v6](#case16-test_lag_multiple_route_v6)
    - [Case17: test_svi_route_l3_v6](#case17-test_svi_route_l3_v6)
    - [Case18: test_svi_learning_v6](#case18-test_svi_learning_v6)
    - [Case19: test_svi_flooding_v6](#case19-test_svi_flooding_v6)
    - [Case20: test_svi_mac_move_v6](#case20-test_svi_mac_move_v6)
    - [Case21: test_svi_mac_age_v6](#case21-test_svi_mac_age_v6)
    - [Case22: test_svi_mac_age_after_move_v6](#case22-test_svi_mac_age_after_move_v6)
    - [Case23: test_svi_mac_move_stress_v6](#case23-test_svi_mac_move_stress_v6)
    - [Case24: test_default_route_v4](#case24-test_default_route_v4)
    - [Case25: test_default_route_v6](#case25-test_default_route_v6)
    - [Case26: test_svi_mac_learn_after_age_v4](#case26-test_svi_mac_learn_after_age_v4)
    - [Case27: test_svi_mac_learn_after_age_v6](#case27-test_svi_mac_learn_after_age_v6)
    - [Case28: test_route_diff_prefix_add_then_delete_longer_v4](#case28-test_route_diff_prefix_add_then_delete_longer_v4)
    - [Case29: test_route_diff_prefix_add_then_delete_longer_v6](#case29-test_route_diff_prefix_add_then_delete_longer_v6)
    - [Case30: test_route_diff_prefix_add_then_delete_shorter_v4](#case30-test_route_diff_prefix_add_then_delete_shorter_v4)
    - [Case31: test_route_diff_prefix_add_then_delete_shorter_v6](#case31-test_route_diff_prefix_add_then_delete_shorter_v6)
    - [Case32: test_route_diff_prefix_add_then_delete_longer_v4](#case32-test_route_diff_prefix_add_then_delete_longer_v4)
    - [Case33: test_route_diff_prefix_add_then_delete_longer_v6](#case33-test_route_diff_prefix_add_then_delete_longer_v6)
    - [Case34: test_route_diff_prefix_add_then_delete_shorter_v4](#case34-test_route_diff_prefix_add_then_delete_shorter_v4)
    - [Case35: test_route_diff_prefix_add_then_delete_shorter_v6](#case35-test_route_diff_prefix_add_then_delete_shorter_v6)
    - [Case36: test_route_same_sip_dip_v4](#case36-test_route_same_sip_dip_v4)
    - [Case37: test_route_same_sip_dip_v6](#case37-test_route_same_sip_dip_v6)
    - [Case38: test_route_lpm_route_nexthop_v4](#case38-test_route_lpm_route_nexthop_v4)
    - [Case39: test_route_lpm_route_rif_v4](#case39-test_route_lpm_route_rif_v4)
  - [Test Group2: Neighbor](#test-group2-neighbor)
    - [Case1: test_host_route_v4](#case1-test_host_route_v4)
    - [Case2: test_host_route_v6](#case2-test_host_route_v6)
    - [Case3: test_no_host_route_v4](#case3-test_no_host_route_v4)
    - [Case4: test_no_host_route_v6](#case4-test_no_host_route_v6)
    - [Case5: test_remove_add_neighbor_v4](#case5-test_remove_add_neighbor_v4)
    - [Case6: test_remove_add_neighbor_v6](#case6-test_remove_add_neighbor_v6)
    - [Case7: test_nexthop_diff_prefix_add_then_remove_longer_v4](#case7-test_nexthop_diff_prefix_add_then_remove_longer_v4)
    - [Case8: test_nexthop_diff_prefix_add_then_remove_longer_v6](#case8-test_nexthop_diff_prefix_add_then_remove_longer_v6)
    - [Case9: test_nexthop_diff_prefix_add_then_remove_shorter_v4](#case9-test_nexthop_diff_prefix_add_then_remove_shorter_v4)
    - [Case10: test_nexthop_diff_prefix_add_then_remove_shorter_v6](#case10-test_nexthop_diff_prefix_add_then_remove_shorter_v6)
  - [Test Group3: Next Hop group](#test-group3-next-hop-group)
    - [Case1: test_ecmp_lags](#case1-test_ecmp_lags)
    - [Case2: test_ecmp_hash_field](#case2-test_ecmp_hash_field)
    - [Case3: test_ecmp_hash_seed](#case3-test_ecmp_hash_seed)
    - [Case4: test_ingress_no_diff](#case4-test_ingress_no_diff)
    - [Case5: test_lag_ecmp](#case5-test_lag_ecmp)
    - [Case6: test_lag_ecmp_add](#case6-test_lag_ecmp_add)
    - [Case7: test_lag_ecmp_disable](#case7-test_lag_ecmp_disable)
    - [Case8: test_lag_ecmp_remove](#case8-test_lag_ecmp_remove)
    - [Case9: test_ecmp_lags_v6](#case9-test_ecmp_lags_v6)
    - [Case10: test_ecmp_hash_field_v6](#case10-test_ecmp_hash_field_v6)
    - [Case11: test_ecmp_hash_seed_v6](#case11-test_ecmp_hash_seed_v6)
    - [Case12: test_ingress_no_diff_v6](#case12-test_ingress_no_diff_v6)
    - [Case13: test_lag_ecmp_v6](#case13-test_lag_ecmp_v6)
    - [Case14: test_lag_ecmp_add_v6](#case14-test_lag_ecmp_add_v6)
    - [Case15: test_lag_ecmp_disable_v6](#case15-test_lag_ecmp_disable_v6)
    - [Case16: test_lag_ecmp_remove_v6](#case16-test_lag_ecmp_remove_v6)
    - [Case17: test_next_hop_group_and_ecmp_entry_api](#case17-test_next_hop_group_and_ecmp_entry_api)
    - [Case18: test_nexthop_group_replace_v4](#case18-test_nexthop_group_replace_v4)
    - [Case19: test_nexthop_group_replace_v6](#case19-test_nexthop_group_replace_v6)
    - [Case20: test_ecmp_two_layer_with_diff_hash_offset_v4](#case20-test_ecmp_two_layer_with_diff_hash_offset_v4)
    - [Case21: test_ecmp_two_layer_with_diff_hash_offset_v6](#case21-test_ecmp_two_layer_with_diff_hash_offset_v6)
    - [Case22: test_lag_two_layer_with_diff_hash_offset_v4](#case22-test_lag_two_layer_with_diff_hash_offset_v4)
    - [Case23: test_lag_two_layer_with_diff_hash_offset_v6](#case23-test_lag_two_layer_with_diff_hash_offset_v6)
    - [Case24: test_ecmp_lag_two_layer_with_diff_hash_offset_v4](#case24-test_ecmp_lag_two_layer_with_diff_hash_offset_v4)
    - [Case25: test_ecmp_lag_two_layer_with_diff_hash_offset_v6](#case25-test_ecmp_lag_two_layer_with_diff_hash_offset_v6)
  - [Test Group4: Route interface](#test-group4-route-interface)
    - [Case1: test_ingress_disable](#case1-test_ingress_disable)
    - [Case2: test_ingress_mac_update](#case2-test_ingress_mac_update)
    - [Case3: test_ingress_mtu](#case3-test_ingress_mtu)
    - [Case4: test_ingress_disable_v6](#case4-test_ingress_disable_v6)
    - [Case5: test_ingress_mac_update_v6](#case5-test_ingress_mac_update_v6)
    - [Case6: test_ingress_mtu_v6](#case6-test_ingress_mtu_v6)
    - [Case7: test_sub_port](#case7-test_sub_port)
# Test Configuration

For the test configuration, please refer to the file 
  - [Config_t0](./config_data/config_t0.md)
  
**Note. All the tests will be based on the configuration above, if any additional configuration is required, it will be specified in the Test case.**

# Test Execution

## Test Group1: Route
### Case1: test_route_rif_v4
### Case2: test_drop_route_v4
### Case3: test_route_update_v4
### Case4: test_lag_multiple_route_v4
### Case5: test_svi_route_l3_v4
### Case6: test_svi_learning_v4
### Case7: test_svi_flooding_v4
### Case8: test_svi_mac_move_v4
### Case9: test_svi_mac_move_stress_v4
### Case10: test_svi_mac_age_v4
### Case11: test_svi_mac_age_after_move_v4
### Case12: test_svi_dierctbroadcast_v4
### Case13: test_route_rif_v6
### Case14: test_drop_route_v6
### Case15: test_route_update_v6
### Case16: test_lag_multiple_route_v6
### Case17: test_svi_route_l3_v6
### Case18: test_svi_learning_v6
### Case19: test_svi_flooding_v6
### Case20: test_svi_mac_move_v6
### Case21: test_svi_mac_age_v6
### Case22: test_svi_mac_age_after_move_v6
### Case23: test_svi_mac_move_stress_v6
### Case24: test_default_route_v4

### Case25: test_default_route_v6
### Case26: test_svi_mac_learn_after_age_v4
### Case27: test_svi_mac_learn_after_age_v6
### Case28: test_route_diff_prefix_add_then_delete_longer_v4
### Case29: test_route_diff_prefix_add_then_delete_longer_v6
### Case30: test_route_diff_prefix_add_then_delete_shorter_v4
### Case31: test_route_diff_prefix_add_then_delete_shorter_v6
### Case32: test_route_diff_prefix_add_then_delete_longer_v4
### Case33: test_route_diff_prefix_add_then_delete_longer_v6
### Case34: test_route_diff_prefix_add_then_delete_shorter_v4
### Case35: test_route_diff_prefix_add_then_delete_shorter_v6
### Case36: test_route_same_sip_dip_v4
### Case37: test_route_same_sip_dip_v6
### Case38: test_route_lpm_route_nexthop_v4
### Case39: test_route_lpm_route_rif_v4

### Testing Objective <!-- omit in toc --> 
Verify the basic route functions:
- test_route_rif:           Verify route with RIF directly (next hop is RIF)
- test_lag_multiple_route:  Verify multi-route to the same nhop
- test_drop_route:          Verify drop the packet when SAI_PACKET_ACTION_DROP 
- test_route_update:        Verify route action gets updated when set attribute from SAI_PACKET_ACTION_DROP to SAI_PACKET_ACTION_FORWARD
- test_route_lpm_route_nexthop:   Verify lpm route path (with next-hop), route path will be alter to the more accurate one. 
- test_route_lpm_route_rif:       Verify lpm route path (with rif as next hop), route path will be alter to the more accurate one. 
- test_svi_route_l3:        Verify route to svi route(through next hop and neighbor)
- test_svi_mac_learning:  Verify route to svi and mac learning
- test_svi_mac_flooding:  Verify route to svi and flooding
- test_svi_mac_move:  Verify route to svi and mac move
- test_svi_mac_move_stress: Verify route to svi and mac move (change simultaneously with 100 times in differernt threads)
- test_default_route: Verify default router drop action
- test_svi_mac_age:   Verify route to svi and mac age
- test_svi_mac_age_after_move:  Verify route to svi and mac age after mac move
- test_svi_dierctbroadcast:       Verify svi direct broadcast
- test_svi_mac_learn_after_age: Verify route mac learn after age
- test_route_diff_prefix_add_then_delete: Verify route add and delete with different ipaddress prefix (add and delete the prefix shorter or longer than the first one)
- test_route_same_sip_dip: Verify route packet with same dip and sip also can be forwarded.
  
### Test steps <!-- omit in toc --> 

- test_route_rif

1. Create route interface for LAG1:``rifx``
2. Create a route for ``DIP:10.1.1.10/32`` through the new ``rifx``
3. Send packet for ``DIP:10.1.1.10`` ``SIP 192.168.0.1`` ``DMAC: SWITCH_MAC``  on port5
4. verify packet received with ``SMAC: SWITCH_MAC`` ``SIP 192.168.0.1`` ``DIP:10.1.1.10`` on one of LAG1 member


- test_lag_multiple_route

1. create new route. ``Dest_IP 10.1.1.10``, through an already existing next-hop: ``IP 10.1.1.100`` ``LAG1``
2. Send packet with ``SIP:192.168.0.1`` ``DIP:10.1.1.10`` ``DMAC:SWITCH_MAC`` on port5
3. verify packet received with ``SMAC: SWITCH_MAC`` ``SIP 192.168.0.1`` ``DIP:10.1.1.10`` on one of LAG1 member
4. Send packet with ``SIP:192.168.0.1`` ``DIP:192.168.11.1`` ``DMAC:SWITCH_MAC`` on port5
5. verify packet received with ``SMAC: SWITCH_MAC`` ``SIP 192.168.0.1`` ``DIP:192.168.11.1`` on one of LAG1 member

- test_drop_route

1. create new route. ``Dest_IP 10.1.1.10`` with ``SAI_PACKET_ACTION_DROP``, through an already existing next-hop: ``IP 10.1.1.100`` ``LAG1``
2. Send packet with ``SIP:192.168.0.1`` ``DIP:10.1.1.10`` ``DMAC:SWITCH_MAC`` on port5
3. verify no packet on any of the LAG1 members
4. Check the packet drop counter

- test_route_update

1. Set Route on DEST IP ``192.168.11.0/24`` with packet action as SAI_PACKET_ACTION_DROP
3. Send packet with ``SIP 192.168.0.1`` ``DIP:192.168.11.1`` ``DMAC: SWITCH_MAC``  on port5
4. verify no packet on any of the LAG1 members
5. Set Route packet action as SAI_PACKET_ACTION_FORWARD
6. Send packet with ``SIP 192.168.0.1`` ``DIP:192.168.11.1`` ``DMAC: SWITCH_MAC``  on port5
7. verify packet received with ``SMAC: SWITCH_MAC`` ``SIP 192.168.0.1`` ``DIP:192.168.11.1`` on one of LAG1 member

- test_route_lpm_route_nexthop

1. Make sure the route for ``DIP:192.168.1.0/24``is already configured (the route through the next hop)
2. Add a neighbor with ``IP:192.168.1.200`` on Port2 with ``DMAC1``
3. Send packet with ``DMAC: SWITCH_MAC`` and ``DIP: IP:192.168.1.200`` on port5
4. Received packet with the ``DMAC1``, ``SMAC: SWITCH_MAC`` and ``DIP: IP:192.168.1.200`` on port2
5. Add a new route for ``DIP: IP:192.168.1.200/32`` with a next-hop on port2
6. Add a neighbor with ``IP:192.168.1.200`` on Port1 with ``DMAC2``
7. Send packet with ``DMAC: SWITCH_MAC`` and ``DIP: IP:192.168.1.200`` on port5
8. Received packet with the ``DMAC2``, ``SMAC: SWITCH_MAC`` and ``DIP: IP:192.168.1.200`` on port1
  
- test_route_lpm_route_rif

1. Make sure the route for ``DIP:192.168.1.0/24``is already configured (the route through next hop)
2. Add a neighbor with ``IP:192.168.1.200`` on Port2 with ``DMAC1``
3. Send packet with ``DMAC: SWITCH_MAC`` and ``DIP: IP:192.168.1.200`` on port5
4. Received packet with the ``DMAC1``, ``SMAC: SWITCH_MAC`` and ``DIP: IP:192.168.1.200`` on port2
5. Add a new route for ``DIP: IP:192.168.1.200/32`` and bind to port2 ``rif2`` directly
6. Add a neighbor with ``IP:192.168.1.200`` on Port1 with ``DMAC2``
7. Send packet with ``DMAC: SWITCH_MAC`` and ``DIP: IP:192.168.1.200`` on port5
8. Received packet with the ``DMAC2``, ``SMAC: SWITCH_MAC`` and ``DIP: IP:192.168.1.200`` on port1

- test_svi_mac_flooding

1. Check config for mac for port1~8 already exist
2. Check already created route and neighbor as, route subnet ``192.168.1.0/24`` next hop to ``VLAN10`` SVI, neighbor for Port2-4 with IP ``DIP:192.168.1.92-94`` bind to ``VLAN10`` SVI
3. Create neighbor for ``DIP:192.168.1.94`` with new ``DMAC:Mac4`` on port4 route interface 
4. Send packet with ``DMAC: SWITCH_MAC``, ``SMAC:00:01:01:99:01:92`` and ``DIP: IP:192.168.1.94`` on port10
5. No packet or flooding on VLAN10 member port

- test_svi_mac_learning

1. Run step 1-5 in test_svi_route_flooding
2. No packet or flooding on VLAN10 member port(expect port2)
3. For mac learning, send packet with ``SMAC:ServerX_MAC`` ``DMAC: SWITCH_MAC`` and ``DIP: Port10 Server_IP`` on port4 (learn ``Server_MAC`` on port4)
4. Received packet with the ``DMAC:Port10 Server_MAC``, ``SMAC: SWITCH_MAC`` and ``DIP: Port10 Server_IP`` on port10
5. For check mac learning from L2, send packet with ``DMAC: ServerX_MAC``, ``SMAC:Port2 Server_MAC``  on port2
6. Received packet with the ``DMAC: ServerX_MAC``, ``SMAC:Port2 Server_MAC`` on port4

- test_svi_mac_move

1. Run step 1-6 in test_svi_route_learning
2. Send packet with ``DMAC: SWITCH_MAC``, ``SMAC:ServerX_MAC`` and ``DIP: Port10 Server_IP`` on port3 (mac ``Server_MAC`` move to port3)
3. Received packet with the ``DMAC:Port10 Server_MAC``, ``SMAC: SWITCH_MAC`` and ``DIP: Port10 Server_IP`` on port10
4. For check mac learning from L2, send packet with ``DMAC: ServerX_MAC``, ``SMAC:Port2 Server_MAC``  on port2
5. Received packet with the ``DMAC: ServerX_MAC``, ``SMAC:Port2 Server_MAC`` on port3

- test_svi_mac_move_stress

1. Run step 1-6 in test_svi_route_learning
2. Send packet with ``DMAC: SWITCH_MAC``, ``SMAC:ServerX_MAC`` and ``DIP: Port10 Server_IP`` on port3 (mac ``Server_MAC`` move to port3)
3. Received packet with the ``DMAC:Port10 Server_MAC``, ``SMAC: SWITCH_MAC`` and ``DIP: Port10 Server_IP`` on port10
4. For check mac learning from L2, send packet with ``DMAC: ServerX_MAC``, ``SMAC:Port2 Server_MAC``  on port2
5. Received packet with the ``DMAC: ServerX_MAC``, ``SMAC:Port2 Server_MAC`` on port3
6. repeat step2 and step4 for 10000 times (in 100 threads, 100 for each)
7. check the fdb available entries, should be no change from step 2

- test_svi_mac_age

1. Set FDB aging time=10
2. Run step 1-6 in test_svi_route_learning
3. Wait for the FDB aging time
4. Send packet with ``DMAC: SWITCH_MAC``, ``SMAC:00:01:01:99:01:92`` and ``DIP: IP:192.168.1.94`` on port2
5.  No packet or flooding on vlan10 member

- test_svi_mac_age_after_move

1. Set FDB aging time=10
2. Run step 1-6 in test_svi_mac_move
3. wait for FDB aging time
4. Send packet with ``DMAC: SWITCH_MAC``, ``SMAC:00:01:01:99:01:92`` and ``DIP: IP:192.168.1.94`` on port2
5. No packet or flooding on VLAN10 member port(expect port2)
6. repeat test_svi_mac_move to check the mac learn and mac move is funcational after age

- test_svi_mac_learn_after_age

1. Set FDB aging time=10
2. Run step 1-6 in test_svi_route_learning
3. wait for half of the FDB aging time
4. Run step 2-3 in test_svi_mac_move
5. wait for the FDB aging time
6. Send packet with ``DMAC: SWITCH_MAC``, ``SMAC:00:01:01:99:01:92`` and ``DIP: IP:192.168.1.94`` on port2
7. No packet or flooding on vlan10 member

- test_svi_route_l3

1. Make Sure route interface, neighbor, and route already exist in the config for VLAN20 SVI and its members
2. send packets with ``DMAC: SWITCH_MAC`` and different IPs in the range ``DIP: IP:192.168.2.9-11`` on port5
3. Verify the packet received on port9 to Port11 when sending a packet with different DIPs

- test_svi_dierctbroadcast

1. Make sure route interface, neighbor, and route already exist in the config for VLAN20 SVI.
2. Make sure Broadcast neighbor already exists in config within VLAN20 subnet (broadcast IP and DMAC is broadcast address)
3. send packet with ``DMAC: SWITCH_MAC`` ``DIP: IP:192.168.2.255`` on port5
4. Verify packet received on port9-16

- test_default_route

1. Make sure default route and route interface are created as config spec.
2. Send packet with a DIP which is not exist in the config spec for any route or host neighbor, ``IPX``
3. No packet received

- test_route_diff_prefix_add_then_delete_longer

1. Make sure the route for ``DIP:192.168.1.0/24``is already configured (the route through the next hop on port2)
2. Add a neighbor with ``IP:192.168.1.200`` on Port2 with ``DMAC1``
3. Send packet with ``DMAC: SWITCH_MAC`` and ``DIP: IP:192.168.1.200`` on port5
4. Received packet with the ``DMAC1``, ``SMAC: SWITCH_MAC`` and ``DIP: IP:192.168.1.200`` on port2
5. Add a neighbor with ``IP:192.168.1.200`` on Port1 with ``DMAC2``
6. Add a new route for ``DIP: IP:192.168.1.200/32`` with next-hop on port1
7. Send packet with ``DMAC: SWITCH_MAC`` and ``DIP: IP:192.168.1.200`` on port5
8. Received packet with the ``DMAC2``, ``SMAC: SWITCH_MAC`` and ``DIP: IP:192.168.1.200`` on port1
9. Delete neighbor with ``IP:192.168.1.100`` on Port1 with ``DMAC2``
10. Delete route for ``DIP: IP:192.168.1.200/32`` with next-hop on port1
11. Delete neighbor with ``IP:192.168.1.200`` on Port2 with ``DMAC1``
12. Delete route for ``DIP:192.168.1.0/24`` and its related next hop

- test_route_diff_prefix_add_then_delete_shorter
  
1. Check neighbor created as common config
2. Add a new route for ``DIP: IP:192.168.1.0/24`` with next-hop on port5
4. Add route for ``DIP:192.168.1.0/12`` with next-hop on port5
5. Delete route for ``DIP:192.168.1.0/12`` with next-hop on port5
7. Delete route for ``DIP: IP:192.168.1.200/24`` with next-hop on port5

- test_route_same_sip_dip

1. Create route interface for LAG1:``rifx``
2. Create a route for ``DIP:10.1.1.10/32`` through the new ``rifx``
3. Send packet for ``DIP:10.1.1.10`` ``SIP:10.1.1.10`` ``DMAC: SWITCH_MAC``  on port5
4. verify packet received with ``SMAC: SWITCH_MAC`` ``SIP 10.1.1.10`` ``DIP:10.1.1.10`` on one of LAG1 member

## Test Group2: Neighbor
### Case1: test_host_route_v4
### Case2: test_host_route_v6
### Case3: test_no_host_route_v4
### Case4: test_no_host_route_v6
### Case5: test_remove_add_neighbor_v4
### Case6: test_remove_add_neighbor_v6
### Case7: test_nexthop_diff_prefix_add_then_remove_longer_v4
### Case8: test_nexthop_diff_prefix_add_then_remove_longer_v6
### Case9: test_nexthop_diff_prefix_add_then_remove_shorter_v4
### Case10: test_nexthop_diff_prefix_add_then_remove_shorter_v6


### Testing Objective <!-- omit in toc --> 
- test_host_route: Verify if no_host is false, neighbor can be used directly for forwarding.
- test_no_host_route: Verify if set the no_host as true, it must bind to a route explicit, otherwise, the packet will be dropped.
- test_remove_add_neighbor: Verify trap to cpu if remove neighbor for a rif directly binded route, and add back will recover the functionality
- test_nexthop_diff_prefix_add_then_remove:  Verify nexthop add and delete with different ipaddress prefix (add and delete the prefix shorter or longer than the first one)

### Test steps <!-- omit in toc --> 
- test_host_route

1. Add a neighbor for IP ``10.1.1.10`` on the LAG1 Route interface and a new ``MACX``
2. Send packet on port5 with ``DMAC: SWITCH_MAC`` ``DIP:10.1.1.10``
3. verify packet received on one of LAG1 member

- test_no_host_route

1. Add a neighbor for IP ``10.1.1.10`` on the LAG1 Route interface, set ``NO_HOST_ROUTE=True``
2. Send packet with on port5 with ``DMAC: SWITCH_MAC`` ``DIP:10.1.1.10``
3. Verify no packet was received on any port

- test_remove_add_neighbor

1. Check the config, make sure the CPU queue0 is created and neighbor(``NO_HOST_ROUTE=True``) for LAGs already created
2. Create route interface for LAG1:``rifx``
2. Create a route for ``DIP:10.1.1.10/32`` through the new ``rifx``
3. Send packet for ``DIP:10.1.1.10`` ``DMAC: SWITCH_MAC``  on port5
4. verify packet received with ``SMAC: SWITCH_MAC`` ``SIP 192.168.0.1`` ``DIP:10.1.1.10`` on one of LAG1 member
5. Delete the neighbor for ``IP:10.1.1.10``
6. Send packet for ``DIP:10.1.1.10`` ``DMAC: SWITCH_MAC``  on port5
7. Verify no packet on any port
8. Verify the CPU queue0 get one more item
9. Add the neighbor for ``IP:10.1.1.10`` on LAG1 again
10. Send packet for ``DIP:10.1.1.10`` ``DMAC: SWITCH_MAC``  on port5
10. verify packet received with ``SMAC: SWITCH_MAC`` ``SIP 192.168.0.1`` ``DIP:10.1.1.10`` on one of LAG1 member

- test_nexthop_diff_prefix_add_then_remove_longer

1. Add nhop with ipprefix length ``12`` which contains the IPaddress for LAG1 Neighbor (exist one in common config)
2. Add nhop with ipprefix length ``24`` which contains the IPaddress for LAG1 Neighbor (exist one in common config)
3. Delete new created nhop with ipprefix length ``24``
3. Delete new created nhop with ipprefix length ``12``

- test_nexthop_diff_prefix_add_then_remove_shorter

1. Add nhop with ipprefix length ``24`` which contains the IPaddress for LAG1 Neighbor (exist one in common config)
2. Add nhop with ipprefix length ``12`` which contains the IPaddress for LAG1 Neighbor (exist one in common config)
3. Delete new created nhop with ipprefix length ``12``
3. Delete new created nhop with ipprefix length ``24``

## Test Group3: Next Hop group
### Case1: test_ecmp_lags
### Case2: test_ecmp_hash_field
### Case3: test_ecmp_hash_seed
### Case4: test_ingress_no_diff
### Case5: test_lag_ecmp
### Case6: test_lag_ecmp_add
### Case7: test_lag_ecmp_disable
### Case8: test_lag_ecmp_remove
### Case9: test_ecmp_lags_v6
### Case10: test_ecmp_hash_field_v6
### Case11: test_ecmp_hash_seed_v6
### Case12: test_ingress_no_diff_v6
### Case13: test_lag_ecmp_v6
### Case14: test_lag_ecmp_add_v6
### Case15: test_lag_ecmp_disable_v6
### Case16: test_lag_ecmp_remove_v6
### Case17: test_next_hop_group_and_ecmp_entry_api
### Case18: test_nexthop_group_replace_v4
### Case19: test_nexthop_group_replace_v6
### Case20: test_ecmp_two_layer_with_diff_hash_offset_v4
### Case21: test_ecmp_two_layer_with_diff_hash_offset_v6
### Case22: test_lag_two_layer_with_diff_hash_offset_v4
### Case23: test_lag_two_layer_with_diff_hash_offset_v6
### Case24: test_ecmp_lag_two_layer_with_diff_hash_offset_v4
### Case25: test_ecmp_lag_two_layer_with_diff_hash_offset_v6

### Testing Objective <!-- omit in toc --> 
Verify the basic Next Hop group functions: 
- test_next_hop_group_and_ecmp_entry: Verify loadbalance on ECMP members
- test_ecmp_hash_field: Verify ECMP loadbalance with different hash field
- test_ecmp_hash_seed:  Verify if the hash seed changes will impact the loadbalance
- test_ingress_no_diff: Verify if different ingress ports will not impact the loadbalance(not change to other egress ports)
- test_ingress_disable: Verify ingress RIF disable
- test_lag_ecmp:    verify LPM with ecmp on LAG port, with add, remove and disable ECMP members 
- test_next_hop_group_and_ecmp_entry_api: Checks SAI switch ECMP attributes and validates get and set attributes
- test_nexthop_group_replace: Verify if nextgroup replacement will take effect
- test_ecmp_two_layer_with_diff_hash_offset: Verify with in two layers (leaf and spine) if different hash offset in ECMP can take effect for the loadbalncing
- test_lag_two_layers_with_diff_hash_offset: Verify with in two layers (leaf and spine) if different hash offset in LAG can take effect for the loadbalncing
- test_ecmp_lag_two_layers_with_diff_hash_offset: Verify with in two layers (leaf and spine) if different hash offset in LAG and ECMP can take effect for the loadbalncing


### Precondition <!-- omit in toc -->
- Make sure hash field already configured for LAG and ECMP(V6 and V4)
  SAI_NATIVE_HASH_FIELD_SRC_IP
  SAI_NATIVE_HASH_FIELD_DST_IP
  SAI_NATIVE_HASH_FIELD_IP_PROTOCOL
  SAI_NATIVE_HASH_FIELD_L4_DST_PORT
  SAI_NATIVE_HASH_FIELD_L4_SRC_PORT

### Test steps <!-- omit in toc --> 
- test_ecmp_lags

1. Generate Packets, with different source IP in range ``SIP:192.168.0.1-192.168.0.10`` 
2. Change other elements in the packets as well, including ``DIP:192.168.60.1-192.168.60.10`` and ``L4_port``
3. send packets with different protocols
4. Verify packets received on different lags and their members

- test_ecmp_hash_field

1. Generate Packets, with different source IPs as ``SIP:192.168.0.1-192.168.0.10`` 
3. For each case, set only one of the four ECMP hash fields(exclude SAI_NATIVE_HASH_FIELD_DST_IP)
4. For each field just change the corresponding field, for example, for SAI_NATIVE_HASH_FIELD_IP_PROTOCOL, use UDP or TCP
5. Verify packets received on different lags and their members (check loadbalanced in LAG and ECMP)
6. Change other un-related fields in the packet
7. Verify packet received on a certain Lag member, not changed 

- test_ecmp_hash_seed

1. Send Packets, with ``SIP:192.168.0.1`` ``DIP:192.168.60.1`` 
2. Verify the packet received on one of the member ports in LAG1-4 (check loadbalanced in LAG and ECMP)
3. Check and change ecmp_default_hash_seed
4. Send the same packet as the Step2
5. Verify the packet received on one of the member ports in LAG1-4 but different from step2 (check loadbalanced in LAG and ECMP)

- test_ingress_no_diff

1. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:192.168.60.1``
2. Send packets from Port5 - Port8
3. Verify packet received on a certain LAG's member, with corresponding SIP, DIP, and ``SMAC: SWITCH_MAC``
4. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:192.168.60.2``
5. Send packets from Port5 - Port8
6. Verify packet received on a certain LAG's member but different from step4

- test_ingress_disable

1. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:192.168.60.1``
2. Disable the ingress for Port5 - Port8 related RIF
3. Send packest from Port5 - Port8
4. Verify no packet was received on any LAG member


- test_lag_ecmp

1. Generate Packets, with different source IPs as ``SIP:192.168.0.1-192.168.0.10`` to match the exiting config
2. Change other elements in the packets, including ``DIP:192.168.60.1`` ``L4_port``
3. Verify packet received on different lags and their members, with corresponding SIP, DIP, and SMAC: SWITCH_MAC (check loadbalanced in LAG and ECMP)
4. Create a next-hop group and add two next hops on IP ``10.1.1.100`` ``10.1.2.100`` on LAG1 and LAG2
5. Set the Routes on ``DIP:192.168.60.1`` as ``SWITCH_MAC_2``  and bind to the new next-hop group
6. Generate Packets, ``SIP:192.168.0.1-192.168.0.10`` 
8. Sent packets with ``DIP:192.168.60.1`` and change the ``L4_port`` number in different packets
9. Verify Packets only get received on LAG1 and LAG2 and with ``SMAC: SWITCH_MAC_2``

- test_lag_ecmp_add

1. run steps 1-10 in test_ecmp
2. add already existing next hop on IP ``10.1.2.100`` for ``LAG3`` to the next-hop group in test_ecmp
4. Generate Packets, with different source IP ``SIP:192.168.0.1-192.168.0.10`` 
5. Change other elements in the packets, including ``DIP:192.168.60.1`` ``L4_port``
6. Verify Packets can be received on LAG1 LAG2 and LAG3, with ``SMAC: SWITCH_MAC_2`` (check loadbalanced in LAG and ECMP)

- test_lag_ecmp_disable

1. run steps 1-8 in test_ecmp_add
2. Disable LAG3 members(member attribute)
3. Generate Packets, with different source IPs as ``SIP:192.168.0.1-192.168.0.10`` 
4. Change other elements in the packets, including ``DIP:192.168.60.1`` ``L4_port``
5. Verify Packets no packet lost and only can be received on LAG1 and LAG2, with ``SMAC: SWITCH_MAC_2`` (check loadbalanced in LAG and ECMP)

- test_lag_ecmp_remove

1. run steps 1-8 in test_ecmp_add
2. remove the next hop from next-hop group in test_ecmp: next-hop with IP ``DIP:10.1.3.100`` on LAG3 
3. Generate Packets, with different source IPs as ``SIP:192.168.0.1-192.168.0.10`` 
4. Change other elements in the packets, including ``DIP:192.168.60.1`` ``L4_port``
5. Verify Packets only can be received on LAG1 and LAG2, with ``SMAC: SWITCH_MAC_2`` (check loadbalanced in LAG and ECMP)

- test_nexthop_group_replace

1. Generate Packets, with different source IPs as ``SIP:192.168.0.1-192.168.0.10`` to match the exiting config
2. Change other elements in the packets, including ``DIP:192.168.60.1`` ``L4_port``
3. Verify packet received on different lags and their members, with corresponding SIP, DIP, and SMAC: SWITCH_MAC
4. Create a next-hop group and add two next hops on IP ``10.1.1.100`` ``10.1.2.100`` on LAG1 and LAG2
5. Set the Routes on ``DIP:192.168.60.1`` as ``SWITCH_MAC_2``  and bind to the new next-hop group
6. Generate Packets, ``SIP:192.168.0.1-192.168.0.10`` 
8. Sent packets with ``DIP:192.168.60.1`` and change the ``L4_port`` number in different packets
9. Verify Packets only get received on LAG1 and LAG2 and with ``SMAC: SWITCH_MAC_2``
10. Create another next-hop group and add one next hop on IP ``10.1.3.100`` on LAG3
11. Set the Routes on ``DIP:192.168.60.1`` as ``SWITCH_MAC_2``  and bind to the new next-hop group
12. Generate Packets, ``SIP:192.168.0.1-192.168.0.10`` 
8. Sent packets with ``DIP:192.168.60.1`` and change the ``L4_port`` number in different packets
9. Verify Packets only get received on LAG3 member and with ``SMAC: SWITCH_MAC_2``

- test_next_hop_group_and_ecmp_entry_api

1. Get those attributes [number_of_ecmp_groups, ecmp_members, available_next_hop_group_entry, available_next_hop_group_member_entry]
2. remove ecmp member and next group member
3. Get attributes again and check the value

- test_ecmp_two_layers_with_diff_hash_offset

1. Check the basic config, make sure there are four lags
2. Make each lag just contains one member(remove extra member from lag)
3. Send 4K packets with different SIP, DIP(192.168.60.0/24), DEST_L4_port,SRC_L4_PORT, PROTO
4. Save the four ports receive sequence for all those 4K packet
5. Send those 4K packet again
6. Received 4K packets with the same sequence in step4
7. Change the ecmp hash offset
8. Send those 4K packet
9. Received 4K packets with the different sequence (range for hash collision)

- test_ecmp_lag_only_two_layers_with_diff_hash_offset

1. Check the basic config, make sure there are four lags
2. Make each lag contains two members(add extra member with available ports)
3. Send 4K packets with different SIP, DIP(192.168.60.0/24), DEST_L4_port,SRC_L4_PORT, PROTO
4. Save the 8 ports receive sequence for all those 4K packet
5. Send those 4K packet again
6. Received 4K packets with the same sequence in step4
7. Change the lag hash offset
8. Send those 4K packet
9. Received 4K packets with the different sequence and those change only happened amoung lag ports within the same lag

**p.s. LAG number should be the same, LAG memeber should be different**

**p.s. use a range for hash collision**

- test_ecmp_lag_two_layers_with_diff_hash_offset

1. Check the basic config, make sure there are four lags
2. Make each lag contains two members(add extra member with available ports)
3. Send 4K packets with different SIP, DIP(192.168.60.0/24), DEST_L4_port,SRC_L4_PORT, PROTO
4. Save the 8 ports receive sequence for all those 4K packet
5. Send those 4K packet again
6. Received 4K packets with the same sequence in step4
7. Change the lag hash and ecmp hash offset
8. Send those 4K packet
9. Received 4K packets with the different sequence on different lags members(use a range for hash collision)

## Test Group4: Route interface
### Case1: test_ingress_disable
### Case2: test_ingress_mac_update
### Case3: test_ingress_mtu
### Case4: test_ingress_disable_v6
### Case5: test_ingress_mac_update_v6
### Case6: test_ingress_mtu_v6
### Case7: test_sub_port

### Testing Objective <!-- omit in toc --> 
- test_ingress_disable:   Verify turn off the admin state for RIF (v4 or v6)
- test_ingress_mac_update: Verify the packet will be dropped if the packet dest mac does not match the mac in the route interface 
- test_ingress_mtu:       Verify the packet will be dropped if the packet length exceeds the MTU value

### Test steps <!-- omit in toc --> 

- test_ingress_disable

1. Set the admin state to False on route interface binds to Port5
2. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:10.1.1.100`` ``DMAC:SWITCH_MAC``
3. Send packet on Port5
4. Verify no packet was received on one of the LAG1's member

- test_ingress_mac_update

1. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:10.1.1.100`` ``DMAC:SWITCH_MAC``
2. Send packet on Port5
3. Verify packet received on one of the LAG1's member
4. Set RIF mac to ``MacX``, the RIF related to Port5
5. Send packet on Port5 
6. Verify no packet was received on any LAG1 member

- test_ingress_mtu

1. Generate Packets, with ``SIP:192.168.0.1`` ``DIP:10.1.1.100`` ``DMAC:SWITCH_MAC``
2. Send packet on Port5
3. Verify packet received on one of the LAG1's member
4. Set RIF MTU to ``200``, the RIF related to Port5
5. Send packet on Port5 with length (200 + 14) ( extra 14 for IPv4, 14 + 40 for IPv6. Bytes from the floor Ethernet layer, It contains the source and destination MAC Address, And the type of agreement)
6. Verify packet received on one of the LAG1's member
7. Send packet on Port5 with length (201 + 14)
8. Verify no packet was received on any LAG1 member
