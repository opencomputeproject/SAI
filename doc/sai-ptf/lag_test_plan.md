# SAI LAG Test plan
- [SAI LAG Test plan](#sai-lag-test-plan)
- [Overriew](#overriew)
- [Test Configuration](#test-configuration)
- [Test Execution](#test-execution)
  - [Test Data/Packet](#test-datapacket)
  - [Test Group1: L3 PortChannel Load balancing](#test-group1-l3-portchannel-load-balancing)
    - [Case1: test_loadbalance_on_source_port_v4](#case1-test_loadbalance_on_source_port_v4)
    - [Case2: test_loadbalance_on_destinstion_port_v4](#case2-test_loadbalance_on_destinstion_port_v4)
    - [Case3: test_loadbalance_on_source_ip_v4](#case3-test_loadbalance_on_source_ip_v4)
    - [Case4: test_loadbalance_on_destinstion_ip_v4](#case4-test_loadbalance_on_destinstion_ip_v4)
    - [Case5: test_loadbalance_on_protocol_v4](#case5-test_loadbalance_on_protocol_v4)
    - [Case6: test_loadbalance_on_source_port_v6](#case6-test_loadbalance_on_source_port_v6)
    - [Case7: test_loadbalance_on_destinstion_port_v6](#case7-test_loadbalance_on_destinstion_port_v6)
    - [Case8: test_loadbalance_on_source_ip_v6](#case8-test_loadbalance_on_source_ip_v6)
    - [Case9: test_loadbalance_on_destinstion_ip_v6](#case9-test_loadbalance_on_destinstion_ip_v6)
    - [Case10: test_loadbalance_on_protocol_v6](#case10-test_loadbalance_on_protocol_v6)
  - [Test Group2: Disable Egress/Ingress](#test-group2-disable-egressingress)
    - [Case1: test_disable_egress_v4](#case1-test_disable_egress_v4)
    - [Case2: test_disable_ingress_v4](#case2-test_disable_ingress_v4)
    - [Case3: test_disable_egress_v6](#case3-test_disable_egress_v6)
    - [Case4: test_disable_ingress_v6](#case4-test_disable_ingress_v6)
  - [Test Group3: Remove/Add LAG member](#test-group3-removeadd-lag-member)
    - [Case1: test_remove_lag_member_v4](#case1-test_remove_lag_member_v4)
    - [Case2: test_add_lag_member_v4](#case2-test_add_lag_member_v4)
    - [Case3: test_remove_lag_member_v6](#case3-test_remove_lag_member_v6)
    - [Case4: test_add_lag_member_v6](#case4-test_add_lag_member_v6)
  - [Test Group4: Indifference Ingress Port in Hash](#test-group4-indifference-ingress-port-in-hash)
    - [Case1: test_ingress_port_hash_indiff_v4](#case1-test_ingress_port_hash_indiff_v4)
    - [Case2: test_ingress_port_hash_indiff_v6](#case2-test_ingress_port_hash_indiff_v6)
# Overriew
The purpose of this test plan is to test the LAG/PortChannel function from SAI.


# Test Configuration

For the test configuration, please refer to LAG configuration section of the file 
  - [Config_t0](./config_data/config_t0.md)
  
**Note. All the tests will be based on the configuration above, if any additional configuration is required, it will be specified in the Test case.**

# Test Execution
## Test Data/Packet
```Python
pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                        eth_src=src_mac,
                        ip_dst=dst_ip_addr,
                        ip_src=src_ip_addr,
                        udp_sport=udp_sport,
                        udp_dport=udp_dport,
                        ip_id=106,
                        ip_ttl=64)

 exp_pkt = simple_udp_packet(eth_dst=dstmac,
                             eth_src=ROUTER_MAC,
                             ip_dst=dst_ip_addr,
                             ip_src=src_ip_addr,
                             udp_sport=udp_sport,
                             udp_dport=udp_dport,
                             ip_id=106,
                             ip_ttl=63)
```

```Python
pkt = simple_tcp_packet(eth_dst=ROUTER_MAC,
                        eth_src=src_mac,
                        ip_dst=dst_ip_addr,
                        ip_src=src_ip_addr,
                        tcp_sport=tcp_sport,
                        tcpp_dport=tcp_dport,
                        ip_id=106,
                        ip_ttl=64)

 exp_pkt = simple_udp_packet(eth_dst=dstmac,
                             eth_src=ROUTER_MAC,
                             ip_dst=dst_ip_addr,
                             ip_src=src_ip_addr,
                             tcp_sport=tcp_sport,
                             tcp_dport=tcp_dport,
                             ip_id=106,
                             ip_ttl=63)
```
## Test Group1: L3 PortChannel Load balancing
These cases will cover five scenarios: src/dst IP, src/dst port, and protocol. considering the mighty hash collision, please make sure the volume of the test data, and we can check the final result is in a range.


### Case1: test_loadbalance_on_source_port_v4
### Case2: test_loadbalance_on_destinstion_port_v4
### Case3: test_loadbalance_on_source_ip_v4
### Case4: test_loadbalance_on_destinstion_ip_v4
### Case5: test_loadbalance_on_protocol_v4
### Case6: test_loadbalance_on_source_port_v6
### Case7: test_loadbalance_on_destinstion_port_v6
### Case8: test_loadbalance_on_source_ip_v6
### Case9: test_loadbalance_on_destinstion_ip_v6
### Case10: test_loadbalance_on_protocol_v6


### Testing Objective <!-- omit in toc --> 
For load balancing, expecting the ports in a lag should receive the packet equally. Traffic direction: from server-side to T1 side. 

### Test steps: <!-- omit in toc --> 
1. Generate different packets by updating different factors for each case. The factor includes source port(L4), destination port(L4), source IP, and destination IP of the packet. Packets use lag1 neighbor IPs as destination IP, and lag1 MAC (Switch MAC) as destination MAC.
2. Send these packets with different protocols on port1(with different cases). 
3. Check if packets are received on ports of lag1 equally.

## Test Group2: Disable Egress/Ingress


### Case1: test_disable_egress_v4
### Case2: test_disable_ingress_v4
### Case3: test_disable_egress_v6
### Case4: test_disable_ingress_v6

### Testing Objective <!-- omit in toc --> 
These cases will cover two scenarios: disable egress and ingress.  We can disable ingress or egress on a lag member, then we expect traffic drop on the disabled lag member.

### Test steps: <!-- omit in toc -->
- test_disable_egress

1. Generate different packets by updating different factors for each case. The factor includes source port(L4), destination port(L4), source IP, and destination IP of the packet. Packets use remote Server IPs as destination IP, and lag1 MAC (Switch MAC) as destination MAC.
2. Send packet from port1 
3. Verify packets appear on different lag1 members(Baseline check).
4. Disable egress on lag1 member port18
5. Send packets in step1 from port1 again
6. Check if the Packet drop on port18

- test_disable_ingress

1. Generate packet using port1 neighbor IP as destination IP,  VLAN Interface MAC (Switch MAC) as destination MAC.
2. Send packet from lag1 port18 
3. Verify packets appear on Port1(Baseline check).
4. Disable ingress on lag1 member port18
5. Send packet in step1 again from lag1 port18
6. Check if Packet drop on port1

## Test Group3: Remove/Add LAG member
### Case1: test_remove_lag_member_v4
### Case2: test_add_lag_member_v4
### Case3: test_remove_lag_member_v6
### Case4: test_add_lag_member_v6

### Testing Objective <!-- omit in toc --> 
These cases will cover adding and removing the lag members.  We can remove or add a lag member, then expect traffic to drop/appear on the lag member.

### Test steps: <!-- omit in toc -->
- test_remove_lag_member

1. Generate different packets by updating different factors for each case. The factor includes source port(L4), destination port(L4), source IP, and destination IP of the packet. Packets use lag1 neighbor IPs as destination IP and lag1 MAC (Switch MAC) as destination MAC.
2. Send packet from port1
3. Verify packets appear on  lag1 port18(Baseline check).
4. Remove port18 from lag1
5. Send packets in step1 from port1 again
6. Check if the Packet drop on port18
   
- test_add_lag_member
  
1. Add port21 to lag1
2. Create packets with variations of the src_ip, dest_ip, src_port,dest_port.(dest_ip is lag1 neighbor IP and MAC is lag1 neighbor MAC)
3. Send packet from port1 
4. Verify packets appear on all the lag1 members, including port21.


## Test Group4: Indifference Ingress Port in Hash
### Case1: test_ingress_port_hash_indiff_v4
### Case2: test_ingress_port_hash_indiff_v6

### Test Objective <!-- omit in toc -->
This case will verify the ingress ports should not be as a Hash Factor in Lag loadbalance. 
When forwarding the packet from different ingress ports, if only the ingress port changed, then the loadbalance should not happen among lag members.

### Test steps: <!-- omit in toc -->
1. Generate packet using one lag1 neighbor IP as destination IP, lag1 MAC (Switch MAC) as destination MAC, and keep all packets from different ports have the same src_ip, dest_ip, src_port(L4),dest_port(L4), and protocol.
2. Send packet from Port1 ~ Port16 
3. Verify packets only appear on one LAG member port
