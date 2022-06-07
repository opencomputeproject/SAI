# SAI Lag Test plan
- [SAI Lag Test plan](#sai-lag-test-plan)
- [Overriew](#overriew)
- [Test Configuration](#test-configuration)
- [Test Execution](#test-execution)
  - [Test Data/Packet](#test-datapacket)
  - [Test Group: L3 PortChannel Loadbalanceing](#test-group-l3-portchannel-loadbalanceing)
    - [Case1: test_loadbalance_on_source_port](#case1-test_loadbalance_on_source_port)
    - [Case2: test_loadbalance_on_destinstion_port](#case2-test_loadbalance_on_destinstion_port)
    - [Case3: test_loadbalance_on_source_ip](#case3-test_loadbalance_on_source_ip)
    - [Case4: test_loadbalance_on_destinstion_ip](#case4-test_loadbalance_on_destinstion_ip)
    - [Case5: test_loadbalance_on_protocol](#case5-test_loadbalance_on_protocol)
  - [Test Group: Disable Egress/Ingress](#test-group-disable-egressingress)
    - [Case6: test_disable_egress](#case6-test_disable_egress)
    - [Case7: test_disable_ingress](#case7-test_disable_ingress)
  - [Test Case8: Remove lag member](#test-case8-remove-lag-member)
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

## Test Group: L3 PortChannel Loadbalanceing
These cases will cover five scenarios: src/dst ip, src/dst port , protocol. considering the mighty hash collision, please make sure the volume of the test data, and we can check the final result is in a range.


### Case1: test_loadbalance_on_source_port
### Case2: test_loadbalance_on_destinstion_port
### Case3: test_loadbalance_on_source_ip
### Case4: test_loadbalance_on_destinstion_ip
### Case5: test_loadbalance_on_protocol


### Testing Objective <!-- omit in toc --> 
For load balancing, expecting the ports in a lag should receive the packet equally. Traffic direction: from server side to T1 side. 

### Additional config: <!-- omit in toc --> 
- Set hash alogrithm as SAI_HASH_ALGORITHM_CRC
- Set switch hash attribute as below, which mean switch computes hash value  using the five fields of packet. 
```
SAI_NATIVE_HASH_FIELD_SRC_IP
SAI_NATIVE_HASH_FIELD_DST_IP
SAI_NATIVE_HASH_FIELD_IP_PROTOCOL
SAI_NATIVE_HASH_FIELD_L4_DST_PORT
SAI_NATIVE_HASH_FIELD_L4_SRC_PORT
```
### Test steps: <!-- omit in toc --> 
- Generate different packets by updating source port, destination port, source ip, and destination ip of the packet. Packets use lag1 neighbor IPs as destination ip, lag1 neighbor MAC as destination MAC.
- Send these packetes with different protocols on port1. 
- Check if packets are recieved on ports of lag1 equally.

## Test Group: Disable Egress/Ingress


### Case6: test_disable_egress
### Case7: test_disable_ingress

### Testing Objective <!-- omit in toc --> 
These cases will cover two scenarios: disable egress and ingress.  We can disable ingress or egress on a lag member, then we expect traffic drop on the disabled lag member.

### Test steps: <!-- omit in toc -->
- Create packets with variations of the src_ip, dest_ip is lag2 neighbor IP and MAC is lag2 neighbor MAC.
- Send packet from port1 
- Verify packets appear on differnt lag2 members.
- Disable egress/ingress on lag2 member port20
- Create packets with variations of the src_ip, dest ip is lag2 neighbor IP and MAC is lag2 neighbor MAC.
- Send packet from port1
- Check if Packet drop on port20

## Test Case8: Remove lag member
- test_remove_lag_member
### Testing Objective <!-- omit in toc --> 
These cases will cover lag memeber removement.  We can remove a lag member, then expect traffic drop on the lag member.

### Test steps: <!-- omit in toc -->
- Create packets with variations of the src_ip, dest_ip is lag3 neighbor IP and MAC is lag3 neighbor MAC.
- Send packet from port0 
- Verify packets appear on differnt lag3 members.
- Remove port22 from lag3
- Create packets with variations of the src_ip, dest ip is lag3 neighbor IP and MAC is lag3 neighbor MAC.
- Send packet from port0 
- Check if Packet drop on port22




