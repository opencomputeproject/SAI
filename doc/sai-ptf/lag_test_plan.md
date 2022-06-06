# SAI Lag Test plan
- [SAI Lag Test plan](#sai-lag-test-plan)
- [Overriew](#overriew)
- [Test Configuration](#test-configuration)
- [Test Execution](#test-execution)
  - [Test Data/Packet](#test-datapacket)
  - [Test Group: L3 PortChannel Loadbalanceing](#test-group-l3-portchannel-loadbalanceing)
    - [Testing Objective](#testing-objective)
    - [Case2: Change destinstion_port](#case2-change-destinstion_port)
    - [Case3: Change source_ip](#case3-change-source_ip)
    - [Case4: Change destinstion_ip](#case4-change-destinstion_ip)
    - [Case5: Case5: Change protocol](#case4-change-protocol)
      
# Overriew
The purpose of this test plan is to test the LAG/PortChannel function from SAI.


# Test Configuration

For the test configuration, please refer to LAG configuration section ofthe file 
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
                             ip_dst=dst_ip_addr,
                             ip_src=src_ip_addr,
                             udp_sport=udp_sport,
                             udp_dport=udp_dport,
                             ip_id=106,
                             ip_ttl=63)
```
TCP Packet
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
                             ip_dst=dst_ip_addr,
                             ip_src=src_ip_addr,
                             tcp_sport=tcp_sport,
                             tcp_dport=tcp_dport,
                             ip_id=106,
                             ip_ttl=63)
```

## Test Group: L3 PortChannel Loadbalanceing
These cases will cover five scenarios: src/dst ip, src/dst port , protocol

### Case1: Change source_port
### Case2: Change destinstion_port
### Case3: Change source_ip
### Case4: Change destinstion_ip
### Case5: Change protocol


### Testing Objective
For load balancing, expecting the ports in a lag should receive the packet equally. Traffic direction: from server side to T1 side. 

### Test steps: <!-- omit in toc --> 
Test steps:
  - 1.Set switch hash attribute as (SAI_NATIVE_HASH_FIELD_SRC_IP,
                                SAI_NATIVE_HASH_FIELD_DST_IP,
                                SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                                SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                                SAI_NATIVE_HASH_FIELD_L4_SRC_PORT), which mean switch computes hash value  using the five fields of packet. 
  - 2.Gerate different packets by  updating source port/destination port/source ip/destination ip of packet.                                
  - 3.Send these packetes on port0 to the lag1. Check if packet forwards on ports of lag1 equally.


