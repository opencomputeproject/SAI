# SAI Tunnel Test plan
- [SAI Tunnel Test plan](#sai-tunnel-test-plan)
- [Overriew](#overriew)
- [Test Configuration](#test-configuration)
- [Test Execution](#test-execution)
  - [Test Group1: IP IN IP Tunnel Decap](#test-group1-ip-in-ip-tunnel-decap)
    - [Case1:  IpIp_Tunnel_Decap_Test_Ipv4inIpv4](#case1-ipip_tunnel_decap_test_ipv4inipv4)
    - [Case2:  IpIp_Tunnel_Decap_Test_Ipv6inIpv4](#case2-ipip_tunnel_decap_test_ipv6inipv4)
    - [Testing Data Packet](#testing-data-packet)
  - [Test Group2: IP IN IP Tunnel Encap](#test-group2-ip-in-ip-tunnel-encap)
    - [Case1:  IpIp_Tunnel_encap_Test_Ipv4inIpv4](#case1-ipip_tunnel_encap_test_ipv4inipv4)
    - [Case2:  IpIp_Tunnel_encap_Test_Ipv6inIpv](#case2-ipip_tunnel_encap_test_ipv6inipv)
    - [Testing Data Packet](#testing-data-packet-1)
  - [Test Group3: IP IN IP P2MP Tunnel Decap](#test-group3-ip-in-ip-p2mp-tunnel-decap)
    - [Case1:  IpIp_P2MP_Tunnel_Decap_Test_With_term_dstip_term_srcip](#case1-ipip_p2mp_tunnel_decap_test_with_term_dstip_term_srcip)
    - [Case2:  IpIp_P2MP_Tunnel_Decap_Test_With_term_dstip_diff_term_srcip](#case2-ipip_p2mp_tunnel_decap_test_with_term_dstip_diff_term_srcip)
    - [Case3:  IpIp_P2MP_Tunnel_Decap_Test_With_diff_dstip_diff_srcip](#case3-ipip_p2mp_tunnel_decap_test_with_diff_dstip_diff_srcip)
    - [Testing Data Packet](#testing-data-packet-2)
  - [Test Group4: IP IN IP Tunnel Decap +SVI](#test-group4-ip-in-ip-tunnel-decap-svi)
    - [Case1:  IpIp_Tunnel_Decap_SVI_Ipv4inIpv4](#case1-ipip_tunnel_decap_svi_ipv4inipv4)
    - [Case2:  IpIp_Tunnel_Decap_SVI_Ipv6inIpv4](#case2-ipip_tunnel_decap_svi_ipv6inipv4)
    - [Case3:  IpIp_Tunnel_Decap_SVI_Flood_Test](#case3-ipip_tunnel_decap_svi_flood_test)
    - [Testing Data Packet](#testing-data-packet-3)
  - [Test Group5: IP IN IP Tunnel Decap Loop Test](#test-group5-ip-in-ip-tunnel-decap-loop-test)
    - [Case1:  IpIp_Tunnel_Decap_With_Loop](#case1-ipip_tunnel_decap_with_loop)
    - [Case2:  IpIp_Tunnel_Decap_With_Loop_with_normal_route](#case2-ipip_tunnel_decap_with_loop_with_normal_route)
    - [Case2:  IpIp_Tunnel_Decap_Without_Loop](#case2-ipip_tunnel_decap_without_loop)
    - [Testing Data Packet](#testing-data-packet-4)
  - [Test Group6: IP IN IP TunneL Encap + LPM](#test-group6-ip-in-ip-tunnel-encap--lpm)
    - [Case1:  IpIp_Tunnel_encap_lpm_Ipv4inIpv4](#case1-ipip_tunnel_encap_lpm_ipv4inipv4)
    - [Case2:  IpIp_Tunnel_encap_lpm_Ipv6inIpv4](#case2-ipip_tunnel_encap_lpm_ipv6inipv4)
    - [Testing Data Packet](#testing-data-packet-5)
  - [Test Group7: IP IN IP Tunnel Decap + LPM](#test-group7-ip-in-ip-tunnel-decap--lpm)
    - [Case1:  IpIp_Tunnel_Decap_Lpm_Ipv4inIpv4](#case1-ipip_tunnel_decap_lpm_ipv4inipv4)
    - [Case2:  IpIp_Tunnel_Decap_Lpm_Ipv6inIpv4](#case2-ipip_tunnel_decap_lpm_ipv6inipv4)
    - [Testing Data Packet](#testing-data-packet-6)
      - [IPV4 IN IPV4 Packet](#ipv4-in-ipv4-packet)
  - [Test Group8: IP IN IP Tunnel Underly ECMP Encap](#test-group8-ip-in-ip-tunnel-underly-ecmp-encap)
    - [Case1:  IpIp_Tunnel_encap_ecmp_Ipv4inIpv4](#case1-ipip_tunnel_encap_ecmp_ipv4inipv4)
    - [Case2:  IpIp_Tunnel_encap_ecmp_Ipv6inIpv4](#case2-ipip_tunnel_encap_ecmp_ipv6inipv4)
    - [Testing Data Packet](#testing-data-packet-7)
  - [Test Group9: Vxlan Tunnel Decap](#test-group9-vxlan-tunnel-decap)
    - [case1:  Vxlan_Tunnel_Decap_Test_L2](#case1-vxlan_tunnel_decap_test_l2)
    - [Testing Data Packet](#testing-data-packet-8)
  - [Test Group11: IP IN IP ENCAP TTL](#test-group11-ip-in-ip-encap-ttl)
    - [Case1: encap_ttl_set_pipe_mode_v4](#case1-encap_ttl_set_pipe_mode_v4)
    - [Case2: encap_ttl_set_pipe_mode_v6](#case2-encap_ttl_set_pipe_mode_v6)
    - [Case3: encap_ttl_set_uniform_mode_v4](#case3-encap_ttl_set_uniform_mode_v4)
    - [Case4: encap_ttl_set_uniform_mode_v6](#case4-encap_ttl_set_uniform_mode_v6)
    - [Testing Data Packet](#testing-data-packet-9)
  - [Test Group12: IP In IP Decap TTL](#test-group12-ip-in-ip-decap-ttl)
    - [Case1: decap_ttl_set_pipe_mode_v4](#case1-decap_ttl_set_pipe_mode_v4)
    - [Case2: decap_ttl_set_pipe_mode_v6](#case2-decap_ttl_set_pipe_mode_v6)
    - [Case3: decap_ttl_set_uniform_mode_v4](#case3-decap_ttl_set_uniform_mode_v4)
    - [Case4: decap_ttl_set_uniform_mode_v6](#case4-decap_ttl_set_uniform_mode_v6)
    - [Testing Data Packet](#testing-data-packet-10)
  - [Test Group13: IP IN IP ENCAP DSCP](#test-group13-ip-in-ip-encap-dscp)
    - [Case1: encap_dscp_set_pipe_mode_v4](#case1-encap_dscp_set_pipe_mode_v4)
    - [Case2: encap_dscp_set_pipe_mode_v6](#case2-encap_dscp_set_pipe_mode_v6)
    - [Case3: encap_dscp_set_uniform_mode_v4](#case3-encap_dscp_set_uniform_mode_v4)
    - [Case4: encap_dscp_set_uniform_mode_v6](#case4-encap_dscp_set_uniform_mode_v6)
    - [Testing Data Packet](#testing-data-packet-11)
  - [Test Group14: IP In IP Decap DSCP](#test-group14-ip-in-ip-decap-dscp)
    - [Case1: decap_dscp_set_pipe_mode_v4](#case1-decap_dscp_set_pipe_mode_v4)
    - [Case2: decap_dscp_set_pipe_mode_v6](#case2-decap_dscp_set_pipe_mode_v6)
    - [Case3: decap_dscp_set_uniform_mode_v4](#case3-decap_dscp_set_uniform_mode_v4)
    - [Case4: decap_dscp_set_uniform_mode_v6](#case4-decap_dscp_set_uniform_mode_v6)
    - [Testing Data Packet](#testing-data-packet-12)
# Overriew
The purpose of this test plan is to test the Tunnel function from SAI.


# Test Configuration

For the test configuration, please refer to Tunnel configuration section of the file 
  - [Config_t0](./config_data/config_t0.md)
  
**Note. All the tests will be based on the configuration above, if any additional configuration is required, it will be specified in the Test case.**

# Test Execution

## Test Group1: IP IN IP Tunnel Decap 
	
### Case1:  IpIp_Tunnel_Decap_Test_Ipv4inIpv4
### Case2:  IpIp_Tunnel_Decap_Test_Ipv6inIpv4
### Testing Objective <!-- omit in toc --> 

    Tunnel Term Source is 10.1.2.100
    Tunnel Term Dest is 10.10.10.1

    -----------------------------------------------------------------
    Egress side[port1]           |          ingress side[lag2]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------

    We will send ipinip packet from lag2 and verify getting inner packet by matching tunnel term table entry, recieving the inner packet on port2 by  matching route entry (192.168.1.1/32,port1).

 ```                        
                           |  tunnel table      |                          |  route table      |
  ingress encap pkt->lag2->|term dst: 10.10.10.1|-> tunnel -> inner pkt -> |192.168.1.1/32,port1|
                           |term src: 10.1.2.100|   
  ->Port1   
        
```

### Testing Data Packet

#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- ingress ipinip packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1)/IP(src=192.168.20.1,dst=192.168.1.1)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=192.168.20.1,dst=192.168.1.1)/TCP()

#### IPV6 IN IPV4 Packet <!-- omit in toc --> 
- ingress ipinip packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1)/TCP()


### Test steps: <!-- omit in toc --> 
1. Generate ingress ipinip packet as decribed by Testing Data Packet
2. Send encap packet from lag2.
3. Generate expected decap packet as decribed by Testing Data Packet.
4. Recieve decap packet from port1, compare it with expected decap packet.


## Test Group2: IP IN IP Tunnel Encap 
### Case1:  IpIp_Tunnel_encap_Test_Ipv4inIpv4
### Case2:  IpIp_Tunnel_encap_Test_Ipv6inIpv

### Testing Objective <!-- omit in toc --> 

    -----------------------------------------------------------------
    Ingress side[port1]           |          Egress side[lag2]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------
 
    We will send normal packet from port1 and verify that packet goes into tunnel via matching route entry(192.168.20.0/24,tunnel), getting a ininip packet, recievinfg a encap packet on lag2 by matching route entry(10.1.2.100/32,lag2) .

 ```                        
                            |  route table         |                           |  route table      |
  ingress encap pkt->port1->|192.168.20.0/24,tunnel|-> tunnel -> ipinip pkt -> |10.1.2.100/32,lag2|
   
  ->lag2   
        
```
### Testing Data Packet
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(src=ROUTER_MAC,dst= 00:01:01:01:02:a0)/IP(dst=10.1.2.100,src=10.10.10.1)/IP(dst=192.168.20.1,src=192.168.1.1)/TCP()
- ingress normal packet = Ether(dst=ROUTER_MAC)/IP(dst=192.168.20.1,src=192.168.1.1)/TCP()


#### IPV6 IN IPV4 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(src=ROUTER_MAC,dst= 00:01:01:01:02:a0/IP(dst=10.1.2.100,src=10.10.10.1)/IP(dst=2001:0000:25DE::CADE,src=fc02::1:1)/TCP()
- ingres normal packet = Ether(dst=ROUTER_MAC)/IP(dst=2001:0000:25DE::CADE,src=fc02::1:1)/TCP()

### Test steps: <!-- omit in toc --> 
1. Generate ingress normal packet as decribed by Testing Data Packet
2. Send  normal packet from port1.
3. Generate expected encap packet as decribed by Testing Data Packet.
4. Recieve encap packet from lag2, compare it with expected encap packet.


## Test Group3: IP IN IP P2MP Tunnel Decap 
### Case1:  IpIp_P2MP_Tunnel_Decap_Test_With_term_dstip_term_srcip

### Case2:  IpIp_P2MP_Tunnel_Decap_Test_With_term_dstip_diff_term_srcip
### Case3:  IpIp_P2MP_Tunnel_Decap_Test_With_diff_dstip_diff_srcip

### Testing Objective <!-- omit in toc --> 

    -----------------------------------------------------------------
    Egress side[port1]           |          ingress side[lag2]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------

    tunnel term table:
    --------------------------------------------------------------
     src ip  |   dst ip |  term type | tunnel dscp mode
    ------------------------------------------------------------------
        10.1.2.100   |     dst=10.10.10.1    | p2p        | pipe
    ------------------------------------------------------------------
                     |     dst=10.10.10.1    | p2mp       | uniform
    -------------------------------------------------------------

     We sending ipinip packet with src ip=10.1.2.100 and dst=10.10.10.1 from lag2, expect getting a inner packet  via matching tunnel term  entry(src=10.1.2.100,dst=10.10.10.1,p2p) when both p2mp type term entry and p2p term entry exit in table, recieving the inner packet on port2 by matching route entry (192.168.1.1/32, port1).
     Then send another ipinip packet with src ip=10.1.4.100,dst=10.10.10.1, we expect it will match p2mp type term entry .
     Then send another ipinip packet with src ip=10.1.4.100,dst=10.10.10.100, we expect packet will drops.

### Testing Data Packet
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- ingress encap packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1,dscp=18)/IP(src=192.168.20.1,dst=192.168.1.1, dscp=10)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=192.168.20.1,dst=192.168.1.1,dscp=10)/TCP()

- ingress encap packet with different term src ip =Ether(dst=ROUTER_MAC)/IP(src=10.1.4.100,dst=10.10.10.1,dscp=18)/IP(src=192.168.40.1,dst=192.168.1.1,dscp=10)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=192.168.20.1,dst=192.168.1.1,dscp=18)/TCP()

- ingress encap packet with different term src ip =Ether(dst=ROUTER_MAC)/IP(src=10.1.4.100,dst=10.10.10.100)/IP(src=192.168.20.1,dst=192.168.1.1)/TCP()

### Test steps: <!-- omit in toc --> 
- IpIp_P2MP_Tunnel_Decap_Test_With_diff_dstip_diff_srcip
1. Generate ingress encap packet with outer dcsp as 18, inner dscp as 10, outer src ip as 10.1.2.10.
2. Send encap packets from lag2.
3. Generate expected decap packets with dscp field as 10.
4. Recieve decap packets from port1, compare it with expected decap packets.

- IpIp_P2MP_Tunnel_Decap_Test_With_term_dstip_diff_term_srcip
1. Generate ingress encap packet with outer dcsp as 18, inner dscp as 10, outer src ip as 10.1.4.100.
2. Send encap packet  from lag2.
3. Generate expected decap packets with dscp field as 18.
4. Recieve decap packets from port1, compare it with expected decap packets.

- IpIp_P2MP_Tunnel_Decap_Test_With_diff_dstip_diff_srcip
1. 1. Generate ingress encap packet with outer src ip as 10.1.4.100,dst as 10.10.10.100
2.  Send encap packet from lag2.
3.  Verify packet drop on port1.


## Test Group4: IP IN IP Tunnel Decap +SVI
	
### Case1:  IpIp_Tunnel_Decap_SVI_Ipv4inIpv4
### Case2:  IpIp_Tunnel_Decap_SVI_Ipv6inIpv4
### Case3:  IpIp_Tunnel_Decap_SVI_Flood_Test
### Testing Objective <!-- omit in toc --> 

    -----------------------------------------------------------------
    Egress side[port1]           |          ingress side[lag2]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------

    We will send ipinip packet from lag2 and verify getting inner packet by matching tunnel term table entry, recieving the inner packet on port2 by  matching svi route entry (192.168.1.0/24,SVI:VLAN10), querying neighbor table and fdb entry (00:01:01:99:01:a0, port1).
    Then remove fdb entry (00:01:01:99:01:a0, port1), we expect flood or drop on all ports of vlan 10.
 ```                        
                            |  tunnel table      |                  |  route table      |
  ingress ipinip pkt->lag2->|term dst: 10.10.10.1|->   inner pkt -> |192.168.1.0/24,SVI:VLAN10|
                            |term src: 10.1.2.100|   
  
  ->nb ->|  fdb table             |->Port1   
         |00:01:01:99:01:a0, port1|        
```

### Testing Data Packet

#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- ingress ipinip packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1)/IP(src=192.168.20.1,dst=192.168.1.253)/TCP()
- expected decap packet = Ether(dst=00:01:01:99:01:a0,src=ROUTER_MAC)/IP(src=192.168.20.1,dst=192.168.1.253)/TCP()


#### IPV6 IN IPV4 Packet <!-- omit in toc --> 
- ingress ip6inip4 packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1)/TCP()


### Test steps: <!-- omit in toc --> 
1. Create fdb entry (dstmac=00:01:01:99:01:a0, port=port1).
1. Generate ingress ipinip packet as decribed by Testing Data Packet
2. Send encap packet from lag2.
3. Generate expected decap packet as decribed by Testing Data Packet.
4. Recieve decap packet from port1, compare it with expected decap packet.
- IpIp_Tunnel_Decap_SVI_Flood_Test
1. Remove vlan10 fdb entry (dstmac=00:01:01:99:01:a0, port=port1);.
2. Generate ingress ipinip packet as decribed by Testing Data Packet
3. Send encap packet from lag2.
4. Check packet drops on port1.

## Test Group5: IP IN IP Tunnel Decap Loop Test
	
### Case1:  IpIp_Tunnel_Decap_With_Loop

### Testing Objective <!-- omit in toc --> 

  We will send ipinip packet from lag2 and verify getting inner packet by matching tunnel term table entry, drop the inner packet on port1 as a result of  matching route entry (192.168.1.253/32,tunnel).
 
 ```                          
                             |  tunnel table      |                |  route table      |
   ingress ipinip pkt->lag2->|term dst: 10.10.10.1| -> inner pkt ->|192.168.1.253/32,tunnel|
                             |term src: 10.1.2.100|                         
   -->Drop                                            
```
### Testing Data Packet

#### IPV4 IN IPV4 Packet <!-- omit in toc --> 

- ingress ipinip packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1)/IP(src=192.168.20.1,dst=192.168.253.253)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=192.168.20.1,dst=192.168.253.253)/TCP()

### Test steps: <!-- omit in toc --> 
- IpIp_Tunnel_Decap_With_Loop
1. Add route entry (192.168.1.253/32, next hop =tunnel), set tunnel SAI_TUNNEL_ATTR_LOOPBACK_PACKET_ACTION as drop
2. Generate ingress ipinip packet as decribed by Testing Data Packet.
3. Send encap packet from lag2.
4. Generate expected decap packet as decribed by Testing Data Packet.
5. Check packet drops on port1.


## Test Group6: IP IN IP TunneL Encap + LPM
### Case1:  IpIp_Tunnel_encap_lpm_Ipv4inIpv4
### Case2:  IpIp_Tunnel_encap_lpm_Ipv6inIpv4

### Testing Objective <!-- omit in toc --> 

    We will send normal packets from port1 and expect  encapsulated packets on lag2 and lag4 equally.
    -----------------------------------------------------------------
    Ingress side[port1]           |          Egress side[lag2] [lag4]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------


### Testing Data Packet
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(src=ROUTER_MAC,dst= 00:01:01:01:02:a0)/IP(dst=10.1.2.100,src=10.10.10.1)/IP(dst=192.168.20.1,src=192.168.1.1)/TCP()
- ingress decap packet = Ether(dst=ROUTER_MAC)/IP(dst=192.168.20.1,src=192.168.1.1)/TCP()

#### IPV6 IN IPV4 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(src=ROUTER_MAC,dst= 00:01:01:01:02:a0/IP(dst=10.1.2.100,src=10.10.10.1)/IP(dst=2001:0000:25DE::CADE,src=fc02::1:1)/TCP()
- ingres decap packet = Ether(dst=ROUTER_MAC)/IP(dst=2001:0000:25DE::CADE,src=fc02::1:1)/TCP()


### Test steps: <!-- omit in toc --> 
1. Generate ingress decap packets with dst ip as 192.168.20.1.
2. Send decap packets from port1.
3. Recieve encap packet from lag2, compare it with expected encap packet.
4. create route (dst ip = 192.168.20.1/32, next hop port=lag4).
5. Send decap packets from port1.
6. Recieve encap packet from lag4, compare it with expected encap packet.

## Test Group7: IP IN IP Tunnel Decap + LPM
	
### Case1:  IpIp_Tunnel_Decap_Lpm_Ipv4inIpv4
### Case2:  IpIp_Tunnel_Decap_Lpm_Ipv6inIpv4

### Testing Objective <!-- omit in toc --> 

  The test group mainly verify that sending ipinip packet from lag2 and getting a inner packet  by matching tunnel term table entry, recieving the inner packet on port2 when both (192.168.1.253/26, port1) and (192.168.1.253/32, port2) exist in route table, recieving the inner packet on port1 when only (192.168.1.253/26, port1)  exist in route table
 ```                          
             |  tunnel table      |                    |  Route table |
  pkt->lag2->|term dst: 10.10.10.1|-> - -> inner pkt ->|192.168.1.253/26, port1|
             |term src: 10.1.2.100|                    |192.168.1.253/32, port2|
```
### Testing Data Packet

#### IPV4 IN IPV4 Packet <!-- it in toc --> 
- ingress ip4inip4 packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1)/IP(src=192.168.20.1,dst=192.168.1.253)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=192.168.20.1,dst=192.168.1.253)/TCP()


#### IPV6 IN IPV4 Packet <!-- omit in toc --> 
- ingress ip6inip4 packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1)/TCP()


### Test steps: <!-- omit in toc --> 
1. Generate ingress encap packet as decribed by Testing Data Packet.
2. Create route (dst ip = 192.168.1.253/26, next hop port=port1).
3. Send encap packet from lag2.
4. Generate expected decap packet as decribed by Testing Data Packet.
5. Recieve decap packet from port1, compare it with expected decap packet.
6. Create route (dst ip = 192.168.1.253/32, next hop port=port2).
7. Send encap packet from lag2.
8. Recieve decap packet from port2, compare it with expected decap packet.
 
## Test Group8: IP IN IP Tunnel Underly ECMP Encap 
### Case1:  IpIp_Tunnel_encap_ecmp_Ipv4inIpv4
### Case2:  IpIp_Tunnel_encap_ecmp_Ipv6inIpv4

### Testing Objective <!-- omit in toc --> 


    -----------------------------------------------------------------
    Ingress side[port1]           |          Egress side[lag2] [lag4]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------
    We will send norml packets with differnnt dst ip,src tcp port and dport from port1 and expect  encapsulated packets on lag2 and lag4 equally.  packet goes into tunnel via matching route entry(192.168.70.0/24,tunnel), getting encapsulated ininip packets, recieving ipinip packets on lag2 and lag4 equally by matching ecmp nexthop group(10.1.60.0/24,nexhop group:lag2,lag4) .

 ```                        
                            |  route table         |                           |  route table       |
  ingress encap pkt->port1->|192.168.70.0/24,tunnel|-> tunnel -> ipinip pkt -> |dst ip: 10.1.60.0/24|
                                                                               |nexhop group:lag2, lag4|
  ->lag2 
    
  ```  

### Testing Data Packet
#### IPV4 inIPV4 Packet <!-- omit in toc --> 

- ingress normal packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(dst=192.168.70.1~100,src=192.168.1.1)/TCP(sport, dport)
- expected egress encap packet=Ether(src=ROUTER_MAC,dst= 00:01:01:01:(02|04):a0)/IP(dst=10.1.60.100,src=10.10.10.1)/IP(dst=192.168.70.1~100,src=192.168.1.1)/TCP()

#### IPV6  Packet <!-- omit in toc --> 
- ingres normal packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(dst=fc02::20:1-100,src=fc02::1:1)/TCP()
- expected egress encap packet=Ether(src=ROUTER_MAC,dst= 00:01:01:01:(02|04):a0)/IP(dst=10.1.60.100,src=10.10.10.1)/IP(dst=fc02::20:1-100,src=fc02::1:1)/TCP()

### Test steps: <!-- omit in toc --> 
1. Generate ingress normal packets with dst ip as 192.168.70.1-100, tcp sport as 16666-16765, dst tcp port 36666~36765.
2. Initalize hash seed as 444.
3. Send decap packets from port1.
4. Check if packets are received on lag2 and lag4 equally.


## Test Group9: Vxlan Tunnel Decap 
	

### case1:  Vxlan_Tunnel_Decap_Test_L2
### case2:  Vxlan_Tunnel_Decap_Test_L2_Diff_Vlan
### Testing Objective <!-- omit in toc --> 

    Tunnel Term Source is 10.1.3.100
    Tunnel Term Dest is 10.10.10.2
     
    -----------------------------------------------------------------
    Egress side[port2]           |          ingress side[lag3]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------

  The test group mainly verify that sending l2 vxlan packet from lag3and getting a decapsulated packet  by matching tunnel term table entry, recieving the inner packet on port2 by matching fdb entry (01:01:00:99:01:01, port2).
 ```                          
             |  tunnel table      |                    |  fdb table |
  pkt->lag3->|term dst: 10.10.10.2|-> - -> inner pkt ->|01:01:00:99:01:01, port2|
             |term src: 10.1.3.100|                   
```
### Testing Data Packet
```Python
 
           egress_l2_pkt = simple_udp_packet(eth_dst=01:01:00:99:01:01,
                                       dl_vlan_enable=True,
                                       vlan_vid=10,
                                       ip_dst=192.168.1.2,
                                       ip_src=192.168.30.101,
                                       ip_id=108,
                                       ip_ttl=64)
				       
           l2_inner_pkt = simple_udp_packet(eth_dst=01:01:00:99:01:01,
                                             ip_dst=192.168.1.2,
                                             ip_src=192.168.30.101,
                                             ip_id=108,
                                             ip_ttl=64)
	   l2_vxlan_pkt_vni1000 = simple_vxlan_packet(eth_dst=ROUTER_MAC,
                                               ip_dst=10.10.10.2,
                                               ip_src=10.1.3.100,
                                               ip_id=0,
                                               ip_ttl=64,
                                               ip_flags=0x2,
                                               udp_sport=11638,
                                               with_udp_chksum=False,
                                               vxlan_vni=1000,
                                               inner_frame=l2_inner_pkt)
	   egress_l2_pkt_vni2000 = simple_udp_packet(eth_dst=01:01:00:99:01:01,
                                       dl_vlan_enable=True,
                                       vlan_vid=20,
                                       ip_dst=192.168.1.2,
                                       ip_src=192.168.30.101,
                                       ip_id=108,
                                       ip_ttl=64)
	  l2_vxlan_pkt_vni2000 = simple_vxlan_packet(eth_dst=ROUTER_MAC,
                                               ip_dst=10.10.10.2,
                                               ip_src=10.1.3.100,
                                               ip_id=0,
                                               ip_ttl=64,
                                               ip_flags=0x2,
                                               udp_sport=11638,
                                               with_udp_chksum=False,
                                               vxlan_vni=2000,
                                               inner_frame=l2_inner_pkt)
```
### Test steps: <!-- omit in toc --> 

- Vxlan_Tunnel_Decap_Test_L2:
1. Generate l2 packet as decribed by Testing Data Packet
2. Send encap packet from lag3.
3. Recieve decap packet from port2, compare it with expected decap packet.

## Test Group11: IP IN IP ENCAP TTL 
	
### Case1: encap_ttl_set_pipe_mode_v4
### Case2: encap_ttl_set_pipe_mode_v6
### Case3: encap_ttl_set_uniform_mode_v4
### Case4: encap_ttl_set_uniform_mode_v6


### Testing Objective <!-- omit in toc --> 
This verifies if TTL field is user-defined for outer header on encapsulation and TTL field of inner header remains the same on decapsulation when using TTL pipe mode.
This verifies the TTL field is preserved end-to-end by copying into the outer header on encapsulation and copying from the outer header on decapsulation when using TTL unifrom mode.

    We will send decapsulated packet from port1 and expect a encapsulated packet on lag2
    -----------------------------------------------------------------
    Ingress side[port1]           |          Egress side[lag2]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------
    ipv6's falls in fc02::  |   ipv6's falls in fc00:1::
    ------------------------------------------------------------------

### Testing Data Packet
- PIPE MODE Packet:
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=10.1.2.100,src=10.10.10.1,ttl=50)/IP(dst=192.168.20.1,src=192.168.1.1, ttl=63)/TCP()
- ingress decap packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(dst=192.168.20.1,src=192.168.1.1, ttl=64)/TCP()

#### IPV6 IN IPV6 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=fc00:1::2:100,src=4001:0E98:03EE::0D25,ttl=50)/IP(fc02::20:1,src=fc02::1:1,ttl=63)/TCP()
- ingress decap packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(fc02::20:1,src=fc02::1:1,ttl=64)/TCP()

- Uniform MODE Packet:
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=10.1.2.100,src=10.10.10.1,ttl=63)/IP(dst=192.168.20.1,src=192.168.1.1, ttl=63)/TCP()
- ingress decap packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(dst=192.168.20.1,src=192.168.1.1, ttl=64)/TCP()

#### IPV6 IN IPV6 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=fc00:1::2:100,src=4001:0E98:03EE::0D25, TTL=63)/IP(fc02::20:1,src=fc02::1:1, TTL=63)/TCP()
- ingress decap packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(fc02::20:1,src=fc02::1:1, TTL=64)/TCP()
  
### Test steps: <!-- omit in toc --> 
- encap_ttl_set_pipe_mod
1. Make sure create tunel with encap_ttl_val attribute as 50, encap_ttl_mode attr with SAI_TUNNEL_TTL_MODE_PIPE_MODEL
2. Generate input packet with ip_ttl field as 64.
3. Send input packet from port1.
4. Create Expected ipinip packet with 50 field in outer ip header as ttl_val,inner ip_ttl as 63.
5. Recieve ipinip packet from lag2, compare it with expected ipinip packet.

- encap_ttl_set_uniform_mode
1. Set ipinip tunnel with encap_ttl_mode attr with SAI_TUNNEL_TTL_MODE_UNIFORM_MODEL
2. Generate input  packet with ip_ttl field as 64.
3. Send input packet from port1.
4. Create expected ipinip packet with ip_ttl field for outer ip header as 63,  inner ip_ttl as 63.
5. Recieve ipinip packet from lag2 port, compare it with expected ipinip packet.
   
## Test Group12: IP In IP Decap TTL 
	
### Case1: decap_ttl_set_pipe_mode_v4
### Case2: decap_ttl_set_pipe_mode_v6
### Case3: decap_ttl_set_uniform_mode_v4
### Case4: decap_ttl_set_uniform_mode_v6

### Testing Objective <!-- omit in toc --> 
This verifies if TTL field is user-defined for outer header on encapsulation and TTL field of inner header remains the same on decapsulation when using TTL pipe mode.
This verifies the TTL field is preserved end-to-end by copying into the outer header on encapsulation and copying from the outer header on decapsulation when using TTL unifrom mode.

    We will send encapsulated packet from lag2 and expect a decapsulated packet on port1
    -----------------------------------------------------------------
    Egress side[port1]           |          ingress side[lag2]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------
    ipv6's falls in fc02::   |   ipv6's falls in fc00:1::
    ------------------------------------------------------------------

### Testing Data Packet
- PIPE MODE PACKET:
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- ingress encap packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1, ttl=64)/IP(src=192.168.20.1,dst=192.168.1.1, ttl=51)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=192.168.20.1,dst=192.168.1.1,ttl=50)/TCP()

#### IPV6 IN IPV6 Packet <!-- omit in toc --> 
- ingress encap packet=Ether(dst=ROUTER_MAC)/IP(src=fc00:1::2:100,dst=4001:0E98:03EE::0D25, ttl=64)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1, ttl=51)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1,ttl=50)/TCP()

- UNIFORM MODE PACKET:
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- ingress encap packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1, ttl=64)/IP(src=192.168.20.1,dst=192.168.1.1, ttl=50)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=192.168.20.1,dst=192.168.1.1,ttl=63)/TCP()

#### IPV6 IN IPV6 Packet <!-- omit in toc --> 
- ingress encap packet=Ether(dst=ROUTER_MAC)/IP(src=fc00:1::2:100,dst=4001:0E98:03EE::0D25, ttl=64)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1, ttl=50)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1,ttl=63)/TCP()
### Test steps: <!-- omit in toc --> 
- decap_ttl_set_pipe_mod
1. Set ipinip tunnel encap_ttl_mode attr with SAI_TUNNEL_TTL_MODE_PIPE_MODEL
2. Generate input ipinip packet with ip_ttl field in outer ip header as 64 , one in inner ip header as 51, expected recieved packet with ip_ttl field as 50.
3. Send packet from lag2.
4. Recieve ipinip packet from port1, compare it with expected packet.

- decap_ttl_set_uniform_mode
1. Set ipinip tunnel encap_ttl_mode attr with with SAI_TUNNEL_TTL_MODE_UNIFORM_MODEL
2. Generate input ipinip packet with ip_ttl field in outer ip header as 64 , inner ip_ttl as 50.
3. Send packet from lag2 port.
4. Create expected recieved packet with ip_ttl field as 63.
5. Recieve ipinip packet from port1, compare it with expected packet.

## Test Group13: IP IN IP ENCAP DSCP 
	
### Case1: encap_dscp_set_pipe_mode_v4
### Case2: encap_dscp_set_pipe_mode_v6
### Case3: encap_dscp_set_uniform_mode_v4
### Case4: encap_dscp_set_uniform_mode_v6


### Testing Objective <!-- omit in toc --> 
This verifies if dscp field is user-defined for outer header on encapsulation and dscp field of inner header remains the same on decapsulation when using dscp pipe mode.
This verifies the dscp field is preserved end-to-end by copying into the outer header on encapsulation and copying from the outer header on decapsulation when using dscp unifrom mode.

    We will send decapsulated packet from port1 and expect a encapsulated packet on lag2
    -----------------------------------------------------------------
    Ingress side[port1]           |          Egress side[lag2]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------
    ipv6's falls in fc02::  |   ipv6's falls in fc00:1::
    ------------------------------------------------------------------

### Testing Data Packet

- PIPE MODE Packet:
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=10.1.2.100,src=10.10.10.1,dscp=10)/IP(dst=192.168.20.1,src=192.168.1.1, dscp=18)/TCP()
- ingress decap packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(dst=192.168.20.1,src=192.168.1.1, dscp=18)/TCP()

#### IPV6 IN IPV6 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=fc00:1::2:100,src=4001:0E98:03EE::0D25,dscp=10)/IP(fc02::20:1,src=fc02::1:1,dscp=18)/TCP()
- ingress decap packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(fc02::20:1,src=fc02::1:1,dscp=18)/TCP()

- Uniform MODE Packet:
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=10.1.2.100,src=10.10.10.1,dscp=18)/IP(dst=192.168.20.1,src=192.168.1.1, dscp=18)/TCP()
- ingress decap packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(dst=192.168.20.1,src=192.168.1.1,dscp=18)/TCP()

#### IPV6 IN IPV6 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=fc00:1::2:100,src=4001:0E98:03EE::0D25, dscp=18)/IP(fc02::20:1,src=fc02::1:1, dscp=18)/TCP()
- ingress decap packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(fc02::20:1,src=fc02::1:1, dscp=18)/TCP()

### Test steps: <!-- omit in toc --> 
- encap_dscp_in_pipe_mode:
1. Set tunnel  encap_dscp_mode attr as SAI_TUNNEL_DSCP_MODE_PIPE_MODEL, encap_dscp_val attribute as user defined ip_dscp=10
2. Generate input packet with dscp field as 18.
3. Send input packet from port1.
4. Create expected ipinip packet with dscp field in outer ip header as 10, inner dscp as 18.
5. Recieve ipinip packet from lag2 . Compare it with expected ipinip packet.

- encap_dscp_in_uniform_mode:
1. Set tunnel  encap_dscp_mode attr encap_dscp_mode attr as SAI_TUNNEL_DSCP_MODE_PIPE_MODEL
2. Generate input packet with dscp field as 18.
3. Send input packet from port1.
4. Create expected ipinip packet with dscp field in outer ip header as 18, inner dscp as 18.
5. Recieve ipinip packet from lag2 . Compare it with expected ipinip packet.
   
## Test Group14: IP In IP Decap DSCP
	
### Case1: decap_dscp_set_pipe_mode_v4
### Case2: decap_dscp_set_pipe_mode_v6
### Case3: decap_dscp_set_uniform_mode_v4
### Case4: decap_dscp_set_uniform_mode_v6

### Testing Objective <!-- omit in toc --> 
This verifies if dscp field is user-defined for outer header on encapsulation and dscp field of inner header remains the same on decapsulation when using dscp pipe mode.
This verifies thedscp field is preserved end-to-end by copying into the outer header on encapsulation and copying from the outer header on decapsulation when using dscp unifrom mode.

    We will send encapsulated packet from lag2 and expect a decapsulated packet on port1
    -----------------------------------------------------------------
    Egress side[port1]           |          ingress side[lag2]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------
    ipv6's falls in fc02::   |   ipv6's falls in fc00:1::
    ------------------------------------------------------------------

### Testing Data Packet
- PIPE MODE PACKET:
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- ingress encap packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1, dscp=10)/IP(src=192.168.20.1,dst=192.168.1.1, dscp=18)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=192.168.20.1,dst=192.168.1.1,dscp=18)/TCP()

#### IPV6 IN IPV6 Packet <!-- omit in toc --> 
- ingress encap packet=Ether(dst=ROUTER_MAC)/IP(src=fc00:1::2:100,dst=4001:0E98:03EE::0D25, dscp=10)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1, dscp=18)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1,dscp=18)/TCP()

- UNIFORM MODE PACKET:
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- ingress encap packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1, dscp=10)/IP(src=192.168.20.1,dst=192.168.1.1, dscp=18)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=192.168.20.1,dst=192.168.1.1,dscp=10)/TCP()

#### IPV6 IN IPV6 Packet <!-- omit in toc --> 
- ingress encap packet=Ether(dst=ROUTER_MAC)/IP(src=fc00:1::2:100,dst=4001:0E98:03EE::0D25, dscp=10)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1, dscp=18)/TCP()
- expected decap packet = Ether(dst=01:01:00:99:01:01,src=ROUTER_MAC)/IP(src=2001:0000:25DE::CADE,dst=fc02::1:1,dscp=10)/TCP()

### Test steps: <!-- omit in toc --> 
- decap_dscp_in_pipe_mode:
1. Set ipinip tunnel encap_dscp_mode attr as SAI_TUNNEL_DSCP_MODE_PIPE_MODEL.
2. Generate input ipinip packet with dscp field in outer ip header as 10, inner  dscp as 18. 
3. Send input packet from lag2.
4. Generate expect packet with dscp field as 18.
5. Recieve decap packet from port1. Compare it with expected ip packet.

- decap_dscp_in_unifrom_mode:
1. Set ipinip tunnel decap_dscp_mode attr as SAI_TUNNEL_DSCP_MODE_UNIFORM_MODEL
2. Generate input ipinip packet with dscp field in outer ip header as 10, one in inner ip header as 18. 
3. Send input packet from lag3 port.
4. Generate expect packet with dscp field as 10.
5. Recieve decap packet from port2. Compare it with expected ip packet.

