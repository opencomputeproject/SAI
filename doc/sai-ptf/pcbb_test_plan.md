# SAI PCBB Test plan  <!-- omit in toc --> 
- [Test Group1: Encapsulation](#test-group1-encapsulation)
  - [Case1: encap_dscp_remap_v4_in_v4](#case1-encap_dscp_remap_v4_in_v4)
  - [Case2: encap_dscp_remap_v6_in_v4](#case2-encap_dscp_remap_v6_in_v4)
  - [Case3: encap_dscp_queue_v4_in_v4](#case3-encap_dscp_queue_v4_in_v4)
  - [Case4: encap_dscp_queue_v6_in_v4](#case4-encap_dscp_queue_v6_in_v4)
  - [Case5: encap_pfc_pause_v4_in_v4](#case5-encap_pfc_pause_v4_in_v4)
  - [Case6: encap_pfc_pause_v6_in_v4](#case6-encap_pfc_pause_v6_in_v4)
  - [Case7: encap_ecn_no_congestion_v4_in_v4](#case7-encap_ecn_no_congestion_v4_in_v4)
  - [Case9: encap_ecn_no_congestion_v6_in_v4](#case9-encap_ecn_no_congestion_v6_in_v4)
  - [Case10: encap_ecn_congestion_v4_in_v4](#case10-encap_ecn_congestion_v4_in_v4)
  - [Case11: encap_ecn_congestion_v6_in_v4](#case11-encap_ecn_congestion_v6_in_v4)
  - [Case12: encap_global_dscp_remap_v4_in_v4](#case12-encap_global_dscp_remap_v4_in_v4)
  - [Case13: encap_global_dscp_remap_v6_in_v4](#case13-encap_global_dscp_remap_v6_in_v4)
- [Test Group2: Decapsulation](#test-group2-decapsulation)
  - [Case1: decap_dscp_remap_v4_in_v4](#case1-decap_dscp_remap_v4_in_v4)
  - [Case2: decap_dscp_remap_v6_in_v4](#case2-decap_dscp_remap_v6_in_v4)
  - [Case3: decap_dscp_priority_v4_in_v4](#case3-decap_dscp_priority_v4_in_v4)
  - [Case4: decap_dscp_priority_v6_in_v4](#case4-decap_dscp_priority_v6_in_v4)
  - [Case5: decap_dscp_queue_v4_in_v4](#case5-decap_dscp_queue_v4_in_v4)
  - [Case6: decap_dscp_queue_v6_in_v4](#case6-decap_dscp_queue_v6_in_v4)
  - [Case7: decap_pfc_pause_v4_in_v4](#case7-decap_pfc_pause_v4_in_v4)
  - [Case8: decap_pfc_pause_v6_in_v4](#case8-decap_pfc_pause_v6_in_v4)
  - [Case9: decap_ecn_no_congestion_v4_in_v4](#case9-decap_ecn_no_congestion_v4_in_v4)
  - [Case10: decap_ecn_no_congestion_v6_in_v4](#case10-decap_ecn_no_congestion_v6_in_v4)
  - [Case11: decap_ecn_congestion_v4_in_v4](#case11-decap_ecn_congestion_v4_in_v4)
  - [Case12: decap_ecn_congestion_v6_in_v4](#case12-decap_ecn_congestion_v6_in_v4)
  - [Case13: decap_global_dscp_to_tc_map_v4_in_v4](#case13-decap_global_dscp_to_tc_map_v4_in_v4)
  - [Case14: decap_global_dscp_to_tc_map_v6_in_v4](#case14-decap_global_dscp_to_tc_map_v6_in_v4)

## Test Group1: Encapsulation

### Case1: encap_dscp_remap_v4_in_v4
### Case2: encap_dscp_remap_v6_in_v4
### Case3: encap_dscp_queue_v4_in_v4
### Case4: encap_dscp_queue_v6_in_v4
### Case5: encap_pfc_pause_v4_in_v4
### Case6: encap_pfc_pause_v6_in_v4
### Case7: encap_ecn_no_congestion_v4_in_v4
### Case9: encap_ecn_no_congestion_v6_in_v4
### Case10: encap_ecn_congestion_v4_in_v4
### Case11: encap_ecn_congestion_v6_in_v4
### Case12: encap_global_dscp_remap_v4_in_v4
### Case13: encap_global_dscp_remap_v6_in_v4

### Testing Objective <!-- omit in toc --> 
- encap_dscp_remap: This verifies on encapsulation, the DSCP field is preserved end-to-end in inner header and the outer header is mapped to the expect encap value base on the DSCP map.
- encap_dscp_queue: This verifies on encapsulation, the DSCP field is preserved end-to-end in inner header and the outer header is mapped to the expect encap value base on the queue map ``DSCP_MAP_TABLE``.
- encap_pfc_pause: This verifies if the buffer is filled up, the pfc frame generated as expected in encap.
- encap_ecn_no_congestion: This verifies the ecn generated as expected in encap when no congestion happens, like ``ECN_NON_CGN_TABLE``.
- encap_ecn_congestion: This verifies the ecn generated as expected in encap when congestion happens, as the ``ECN_CGN_TABLE``
- encap_global_dscp_remap: This verifies on encapsulation, the DSCP field is preserved end-to-end in inner header and the outer header is mapped to the expect encap value base on the switch global DSCP map.


    We will send a decapsulated packet from port1 and expect an encapsulated packet on any lag1-4 member


  | Ingress side[port1]      |     Egress side[lag1] [lag2] [lag3] [lag4] |
  |--------------------------|----------------------------------------
  | ipv4's falls in 192.168.60.0     |     ipv4's falls in 10.1.0.0 |
  | ipv6's falls in fc02::60:0  |   ipv6's falls in fc00:1:: |

### Testing Data Packet <!-- omit in toc --> 

This test should cover the data in the table below

DSCP_MAP_TABLE
|DSCP|TC to verify| Expected DSCP after encap(outer)|Outgoing Queue(Tunnel)|Priority(Port)|
| ---- | ---- | --- |-|-|
|8|0|8|0|0|
|0-1|1|0|1|0|
|3|3|2|2|3|
|4|4|6|6|4|
|5-32|1|0|1|0|
|33|8|33|1|0|
|34-45|1|0|1|0|
|47|1|0|1|0|
|46|5|46|5|0|
|48|7|48|7|7|
|49-63|1|0|1|0|


For ECN testing

ECN_NON_CGN_TABLE:
|DSCP| ECN Outer DSCP |Outer ECN |Inner DSCP |Inner ECN|
|-|-|-|-|-|
|33|3|33|3|33|3|
|33|2|33|2|33|2|
|33|1|33|1|33|1|
|3|3|2|3|3|3|
|3|2|2|2|3|2|
|3|1|2|1|3|1|
|4|3|6|3|4|3|
|4|2|6|2|4|2|
|4|1|6|1|4|1|

ECN_CGN_TABLE:
|DSCP|ECN|Outer DSCP|Outer ECN|Inner DSCP|Inner ECN|
|-|-|-|-|-|-|
|3|3|2|3|3|3|
|3|2|2|3|3|3|
|3|1|2|3|3|3|
|4|3|6|3|4|3|
|4|2|6|3|4|3|
|4|1|6|3|4|3|


#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=``ACTIVE_TOR_IP``,src=`STAND_BY_TOR_IP``,ip_dscp=``EXP_DSCP COL``, ip_ecn=``OUTER_ECN``)/IP(dst=192.168.60.1,src=192.168.1.1, ip_dscp=``DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()
- ingress packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(dst=192.168.60.1,src=192.168.1.1, ip_dscp=``DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()

#### IPV6 IN IPV4 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=``ACTIVE_TOR_IP``,src=`STAND_BY_TOR_IP``,ip_dscp=``EXP_DSCP COL``, ip_ecn=``OUTER_ECN``)/IP(fc02::60:1,src=fc02::1:1,ip_dscp=``EXP_DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()
- ingress packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(dst=``REMOTE_SERVER_IPv6``,src=fc02::1:1,ip_dscp=``EXP_DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()


### Test steps: <!-- omit in toc --> 
- encap_dscp_remap:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)  
3. Generate packet, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
4. Send input packet from port1.
5. Create the expected ipinip packet with ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE``. 
6. Recieve ipinip packet from any lag1-4 member port. Compare it with the expected ipinip packet.

- encap_dscp_queue:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)  
3. According to DSCP_MAP_TABLE check the corrosponding queue's packets stats on the possible ports(use sai_thrift_get_queue_stats with "SAI_QUEUE_STAT_PACKETS")
4. Generate 1000 packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
5. Send input packet from port1.
6. Create the expected ipinip packet with ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE``. 
7. Recieve ipinip packet from any lag1-4 member port. Compare it with the expected ipinip packet.
8. For each packet received port check the corresponding queue packets stats

- encap_pfc_pause:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map and priority map as basic [config](./config_data/config_t0.md)
3. Set the dest ports with lossless buffer pool profile
4. According to DSCP_MAP_TABLE check the corrosponding priority_group packets stats on the possible ports(use sai_thrift_get_ingress_priority_group_stats with "SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS")
5. Get the packet number for different meanings, pkts_num_egr_mem for filling up memory, pkts_num_leak_out for leakout packets, pkts_num_trig_pfc for triggering PFC and margin for a tolerance ( reference [config](https://github.com/Azure/sonic-mgmt/blob/master//ansible/vars/qos.yml) which use in case [PFCTest](https://github.com/Azure/sonic-mgmt/blob/master/tests/saitests/sai_qos_tests.py#665))
6. Disable ports (set port SAI_PORT_ATTR_PKT_TX_ENABLE)
7. Send N packets from lag1 to fill up the shared buffer (pkts_num_egr_mem + pkts_num_leak_out + pkts_num_trig_pfc - 1 - margin)
8. Recording the counters on the receiving and transmit ports
9. Send the packets again
10. Verify PFC drop happened(Receive port counters are larger than transmit port counters).

- encap_ecn_no_congestion:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)
3. Make sure set PCBB ECN configurations
4. Generate 100 packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly, according to DSCP_MAP_TABLE set the ``ip_ecn``
5. Send input packet from port1.
6. Create expected ipinip packet with ``Inner_ECN`` ``Outer_ECN`` ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE``. 
7. Recieve ipinip packet from any lag1-4 member port. Compare it with the expected ipinip packet.

- encap_ecn_congestion:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)
3. Make sure set PCBB ECN configurations
4. Set all the dest ports with lossless buffer pool profile
5. According to DSCP_MAP_TABLE check the corrosponding priority_group packets stats on the possible ports(use sai_thrift_get_ingress_priority_group_stats with "SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS")
6. Disable ports (set port SAI_PORT_ATTR_PKT_TX_ENABLE)
7. Check the corrosponding priority_group buffer state(SAI_INGRESS_PRIORITY_GROUP_STAT_SHARED_CURR_OCCUPANCY_BYTES, SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES, SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES)
8. Generate N packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly, according to DSCP_MAP_TABLE set the ``ip_ecn``
9. Use the similar approach in decap_pfc_pause test to fill up the buffer
10. Enable ports (set port SAI_PORT_ATTR_PKT_TX_ENABLE)
11. Recieve decap packet from port1. Compare it with the expected packet for the ECN and DSCP values.

- encap_global_dscp_to_tc_map:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, config port with ``Port DSCP MAP`` and switch_dscp_to_tc with dscp map in ``Tunnel TC MAP`` as basic [config](./config_data/config_t0.md)  
3. Generate packet, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
4. Send input packet from port1.
5. Create the expected ipinip packet with ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE``. 
6. Recieve ipinip packet from any lag1-4 member port. Compare it with the expected ipinip packet.


## Test Group2: Decapsulation

### Case1: decap_dscp_remap_v4_in_v4
### Case2: decap_dscp_remap_v6_in_v4
### Case3: decap_dscp_priority_v4_in_v4
### Case4: decap_dscp_priority_v6_in_v4
### Case5: decap_dscp_queue_v4_in_v4
### Case6: decap_dscp_queue_v6_in_v4
### Case7: decap_pfc_pause_v4_in_v4
### Case8: decap_pfc_pause_v6_in_v4
### Case9: decap_ecn_no_congestion_v4_in_v4
### Case10: decap_ecn_no_congestion_v6_in_v4
### Case11: decap_ecn_congestion_v4_in_v4
### Case12: decap_ecn_congestion_v6_in_v4
### Case13: decap_global_dscp_to_tc_map_v4_in_v4
### Case14: decap_global_dscp_to_tc_map_v6_in_v4


### Testing Objective <!-- omit in toc --> 
- decap_dscp_remap: This verifies on decapsulation, the DSCP inner field is preserved end-to-end and mapping to the expect dscp value base on the DSCP map.
- decap_dscp_priority:  This verifies on decapsulation, the DSCP inner field is preserved end-to-end and mapping to the expect priority value base on the DSCP map.
- decap_dscp_queue:  This verifies on decapsulation, the DSCP inner field is preserved end-to-end and mapping to the expect queue value base on the DSCP map.
- decap_pfc_pause: This verifies the pfc frame generated as expected in decap.
- decap_ecn_no_congestion: This verifies the ecn generated as expected in decap when no congestion happens.
- decap_ecn_congestion: This verifies the ecn generated as expected in decap when congestion happens.
- decap_global_dscp_to_tc_map: This verifies on decapsulation, the DSCP inner field is preserved end-to-end and mapping to the expect dscp value base on the DSCP map.


    We will send a decapsulated packet from LAG1 and expect a decapsulated packet on port1

  Egress side[port1]           |          Ingress side[lag1]
  -----------------------------|-------------------------------------
  ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
  ipv6's falls in fc02::1:0  |   ipv6's falls in fc00:1::
### Testing Data Packet <!-- omit in toc --> 

This test should cover the data in the table below

DSCP_MAP_TABLE
|DSCP(Base on Inner)|TC to verify| Outgoing Queue(Port)|Priority(Tunnel)|
| ---- | ---- | -|-|
|0-2|1|1|0|
|3|3|3|2|
|4|4|4|6|
|5-7|1|1|0|
|8|0|0|0|
|9-32|1|1|0|
|33|8|1|0|
|34-45|1|1|0|
|46|5|5|0|
|48|7|7|7|
|49-63|1|1|0|

For ECN testing
ECN_NON_CGN_TABLE:
|Outer_DSCP|Outer_ECN|Inner_DSCP|Inner_ECN|Decap_DSCP|Decap_ECN|
|-|-|-|-|-|-|
|2|3|3|3|3|3|
|2|2|3|2|3|2|
|2|1|3|1|3|1|
|2|3|3|2|3|3|
|2|2|3|3|3|2|
|6|3|4|3|4|3|
|6|2|4|2|4|2|
|6|1|4|1|4|1|


ECN_CGN_TABLE:
|Outer_DSCP|Outer_ECN|Inner_DSCP|Inner_ECN|Decap_DSCP|Decap_ECN|
|-|-|-|-|-|-|
|2|2|3|2|3|3|
|6|2|4|2|4|3|


- PIPE MODE Packet:
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- expected egress packet = Ether(dst=00:01:01:99:01:01,src=ROUTER_MAC)/IP(dst=192.168.1.1,src=192.168.60.1, ip_dscp=``DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()
- Ingress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=`STAND_BY_TOR_IP``,src=``ACTIVE_TOR_IP``,ip_dscp=``EXP_DSCP COL``, ip_ecn=``OUTER_ECN``)/IP(src=192.168.60.1,dst=192.168.1.1, ip_dscp=``DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()


#### IPV6 IN IPV4 Packet <!-- omit in toc --> 
- Expected egress packet = Ether(dst=00:01:01:99:01:01,src=ROUTER_MAC)/IP(src=fc02::60:1,dst=fc02::1:1,ip_dscp=``EXP_DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()
- Ingress encap packet=Ether(dst=ROUTER_MAC)/IP(src=``ACTIVE_TOR_IP``,dst=`STAND_BY_TOR_IP``,ip_dscp=``EXP_DSCP COL``, ip_ecn=``OUTER_ECN``)/IP(src=fc02::60:1,dst=fc02::1:1,ip_dscp=``EXP_DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()


### Test steps: <!-- omit in toc --> 
- decap_dscp_remap:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)  
3. Generate packet, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
4. Send input ipinip packet from lag1 with ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE`` (only inner take effect).
5. Create the expected decap packet with ``Inner dscp`` according to ``DSCP_MAP_TABLE``. 
6. Recieve decapped packet from port1. Compare it with the expected decap packet.

- decap_dscp_priority:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)  
3. According to DSCP_MAP_TABLE check the corrosponding priority_group packets stats on the possible ports(use sai_thrift_get_ingress_priority_group_stats with "SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS")
4. Generate 1000 packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
5. Send input ipinip packet from lag1 with ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE`` (only inner take effect).
6. Create the expected decap packet with ``Inner dscp`` according to ``DSCP_MAP_TABLE``. 
7. Recieve decapped packet from port1. Compare it with the expected decap packet.
8. For received port check the corresponding priority_group packets stats (SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES)

- decap_dscp_queue:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)  
3. According to DSCP_MAP_TABLE check the corrosponding queue's packets stats on the possible ports(use sai_thrift_get_queue_stats with "SAI_QUEUE_STAT_PACKETS")
4. Generate 1000 packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
5. Send input ipinip packet from lag1 with ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE`` (only inner take effect).
6. Create the expected decap packet with ``Inner dscp`` according to ``DSCP_MAP_TABLE``. 
7. Recieve decapped packet from port1. Compare it with the expected decap packet.
8. For received port check the corresponding queue packets stats

- decap_pfc_pause:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map and priority map as basic [config](./config_data/config_t0.md)
3. Set dest ports with lossless buffer pool profile
4. According to DSCP_MAP_TABLE check the corrosponding priority_group packets stats on the possible ports(use sai_thrift_get_ingress_priority_group_stats with "SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS")
5. Get the packet number for different meanings, pkts_num_egr_mem for filling up memory, pkts_num_leak_out for leakout packets, pkts_num_trig_pfc for triggering PFC and margin for a tolerance ( reference [config](https://github.com/Azure/sonic-mgmt/blob/master//ansible/vars/qos.yml) which use in case [PFCTest](https://github.com/Azure/sonic-mgmt/blob/master/tests/saitests/sai_qos_tests.py#665))
6. Disable ports (set port SAI_PORT_ATTR_PKT_TX_ENABLE)
7. Send N packets from lag1 to fill up the shared buffer (pkts_num_egr_mem + pkts_num_leak_out + pkts_num_trig_pfc - 1 - margin)
8. Recording the counters on the receiving and transmit ports
9. Send the packets again
10. Verify PFC drop happened(Receive port counters are larger than transmit port counters).

- decap_ecn_no_congestion:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map and priority map as basic [config](./config_data/config_t0.md)
3. Make sure set PCBB ECN configurations
4. Generate 100 packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly, according to DSCP_MAP_TABLE set the ``ip_ecn``
5. Send input ipinip packet from lag1 with ``Inner_ECN`` ``Outer_ECN`` ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE`` (only inner take effect).
5. Create expected decap packet with ``ECN`` and ``Inner DSCP`` according to ``DSCP_MAP_TABLE`` and ``ECN_TABLE``
6. Recieve decapped packet from port1. Compare it with the expected decap packet.

- decap_ecn_congestion:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)
3. Make sure set PCBB ECN configurations
4. Set all the dest port with lossless buffer pool profile
5. Disable ports (set port SAI_PORT_ATTR_PKT_TX_ENABLE)
6. According to DSCP_MAP_TABLE check the corrosponding priority_group packets stats on the possible ports(use sai_thrift_get_ingress_priority_group_stats with "SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS")
7. Generate N packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly, according to DSCP_MAP_TABLE set the ``ip_ecn``
8. Use the similar approach in decap_pfc_pause test to fill up the buffer
8.  Enable ports (set port SAI_PORT_ATTR_PKT_TX_ENABLE)
9.  Recieve decap packet from port1. Compare it with the expected packet for the ECN and DSCP values.

- decap_global_dscp_to_tc_map:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, config port with ``Port DSCP MAP`` and switch_dscp_to_tc with dscp map in ``Tunnel TC MAP`` as basic [config](./config_data/config_t0.md)  
3. Generate packet, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
4. Send input ipinip packet from port1.
5. Create the expected decap packet with ``Inner dscp`` according to ``DSCP_MAP_TABLE``. 
6. Recieve ipinip packet from any lag1-4 member port. Compare it with the expected decap packet.
