# SAI PCBB Test plan  <!-- omit in toc --> 
- [Test Group1: Encapsulation](#test-group1-encapsulation)
  - [Case1: encap_dscp_remap_v4_in_v4](#case1-encap_dscp_remap_v4_in_v4)
  - [Case2: encap_dscp_remap_v6_in_v4](#case2-encap_dscp_remap_v6_in_v4)
  - [Case3: encap_dscp_priority_v4_in_v4](#case3-encap_dscp_priority_v4_in_v4)
  - [Case4: encap_dscp_priority_v6_in_v4](#case4-encap_dscp_priority_v6_in_v4)
  - [Case5: encap_dscp_queue_v4_in_v4](#case5-encap_dscp_queue_v4_in_v4)
  - [Case6: encap_dscp_queue_v6_in_v4](#case6-encap_dscp_queue_v6_in_v4)
  - [Case7: encap_pfc_pause_v4_in_v4](#case7-encap_pfc_pause_v4_in_v4)
  - [Case8: encap_pfc_pause_v6_in_v4](#case8-encap_pfc_pause_v6_in_v4)
  - [Case9: encap_ecn_no_congestion_v4_in_v4](#case9-encap_ecn_no_congestion_v4_in_v4)
  - [Case10: encap_ecn_no_congestion_v6_in_v4](#case10-encap_ecn_no_congestion_v6_in_v4)
  - [Case11: encap_ecn_congestion_v4_in_v4](#case11-encap_ecn_congestion_v4_in_v4)
  - [Case12: encap_ecn_congestion_v6_in_v4](#case12-encap_ecn_congestion_v6_in_v4)
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

## Test Group1: Encapsulation

### Case1: encap_dscp_remap_v4_in_v4
### Case2: encap_dscp_remap_v6_in_v4
### Case3: encap_dscp_priority_v4_in_v4
### Case4: encap_dscp_priority_v6_in_v4
### Case5: encap_dscp_queue_v4_in_v4
### Case6: encap_dscp_queue_v6_in_v4
### Case7: encap_pfc_pause_v4_in_v4
### Case8: encap_pfc_pause_v6_in_v4
### Case9: encap_ecn_no_congestion_v4_in_v4
### Case10: encap_ecn_no_congestion_v6_in_v4
### Case11: encap_ecn_congestion_v4_in_v4
### Case12: encap_ecn_congestion_v6_in_v4

### Testing Objective <!-- omit in toc --> 
This verifies in tunnel dscp remap if DSCP field is remapped for the outer header on encapsulation and DSCP field of the inner header remains the same on decapsulation when using DSCP pipe mode.
This verifies the DSCP field is preserved end-to-end by copying into the outer header on encapsulation and copying from the outer header on decapsulation, combining with QoS map.

    We will send a decapsulated packet from port1 and expect an encapsulated packet on any lag1-4 member
    -----------------------------------------------------------------
    Ingress side[port1]           |          Egress side[lag1] [lag2] [lag3] [lag4]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.60.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------
    ipv6's falls in fc02::60:0  |   ipv6's falls in fc00:1::
    ------------------------------------------------------------------

### Testing Data Packet <!-- omit in toc --> 

This test should cover the data in the table below

DSCP_MAP_TABLE
|DSCP|TC to verify| Expected DSCP after encap(outer)|Outgoing Queue(Tunnel)|Priority(Port)|
| ---- | ---- | --- |-|-|
|8|0|8|0|0|
|0|1|0|1|0|
|33|8|33|1|0|
|3|3|2|2|3|
|4|4|6|6|4|
|46|5|46|5|0|
|48|7|48|7|7|

For ECN testing

ECN_NON_CGN_TABLE:
|DSCP| ECN Outer DSCP |Outer ECN |Inner DSCP |Inner ECN|
|-|-|-|-|-|
|33|3|33|3|33|3|
33|2|33|2|33|2|
33|1|33|1|33|1|
3|3|2|3|3|3|
3|2|2|2|3|2|
3|1|2|1|3|1|
4|3|6|3|4|3|
4|2|6|2|4|2|
4|1|6|1|4|1|

ECN_CGN_TABLE:
|DSCP|ECN|Outer DSCP|Outer ECN|Inner DSCP|Inner ECN|
|-|-|-|-|-|-|
|3|3|2|3|3|3|
|3|2|2|3|3|3|
|3|1|2|3|3|3|
|4|3|6|3|4|3|
|4|2|6|3|4|3|
|4|1|6|3|4|3|


- PIPE MODE Packet:
#### IPV4 IN IPV4 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=10.1.2.100,src=10.10.10.1,ip_dscp=``EXP_DSCP COL``, ip_ecn=``OUTER_ECN``)/IP(dst=192.168.60.1,src=192.168.1.1, ip_dscp=``DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()
- ingress packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(dst=192.168.60.1,src=192.168.1.1, ip_dscp=``DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()

#### IPV6 IN IPV4 Packet <!-- omit in toc --> 
- expected egress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=10.1.2.100,src=10.10.10.1,ip_dscp=``EXP_DSCP COL``, ip_ecn=``OUTER_ECN``)/IP(fc02::60:1,src=fc02::1:1,ip_dscp=``EXP_DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()
- ingress packet = Ether(dst=00:01:01:01:02:a0,src=ROUTER_MAC)/IP(dst=192.168.60.1,src=fc02::1:1,ip_dscp=``EXP_DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()


### Test steps: <!-- omit in toc --> 
- encap_dscp_remap:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)  
3. Generate 1000 packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
4. Send input packet from port1.
5. Create the expected ipinip packet with ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE``. 
6. Recieve ipinip packet from any lag1-4 member port. Compare it with the expected ipinip packet.

- encap_dscp_priority:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)  
3. According to DSCP_MAP_TABLE check the corrosponding priority_group packets stats on the possible ports(use sai_thrift_get_ingress_priority_group_stats with "SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS")
4. Generate 1000 packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
5. Send input packet from port1.
6. Create the expected ipinip packet with ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE``. 
7. Recieve ipinip packet from any lag1-4 member port. Compare it with the expected ipinip packet.
8. For each packet received port check the corresponding priority_group packets stats

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
3. Set all the dest ports with lossless buffer pool profile
4. According to DSCP_MAP_TABLE check the corrosponding priority_group packets stats on the possible ports(use sai_thrift_get_ingress_priority_group_stats with "SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS")
5. Check the corrosponding priority_group buffer state(SAI_INGRESS_PRIORITY_GROUP_STAT_SHARED_CURR_OCCUPANCY_BYTES, SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES, SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES)
6. Generate N packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
7. Disable ports (set port SAI_PORT_ATTR_PKT_TX_ENABLE)
8. Send N packets from port1 to fill the shared buffer 
9. verify PFC pause frames are generated on expected priority(``DST:01:80:c2:00:00:01``). 
10. After filling up the buffer, send packets to check the PG drop counters to verify the counter increased as expected (SAI_INGRESS_PRIORITY_GROUP_STAT_DROPPED_PACKETS)

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
5. Check the corrosponding priority_group buffer state(SAI_INGRESS_PRIORITY_GROUP_STAT_SHARED_CURR_OCCUPANCY_BYTES, SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES, SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES)
6. Generate N packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly, according to DSCP_MAP_TABLE set the ``ip_ecn``
7. Disable ports (set port SAI_PORT_ATTR_PKT_TX_ENABLE)
8. Send N packets from port1 to fill the shared buffer 
9. verify PFC pause frames are generated on expected priority(``DST:01:80:c2:00:00:01``)
10. Enable ports (set port SAI_PORT_ATTR_PKT_TX_ENABLE)
11. Recieve ipinip packet from any lag1-4 member port. Compare it with the expected ipinip packet.


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

### Testing Objective <!-- omit in toc --> 
This verifies if DSCP field is user-defined for the outer header on encapsulation and DSCP field of the inner header remains the same on decapsulation when using DSCP pipe mode.
This verifies the DSCP field is preserved end-to-end by copying into the outer header on encapsulation and copying from the outer header on decapsulation, combining with the QoS map.

    We will send a decapsulated packet from LAG1 and expect a decapsulated packet on port1
    -----------------------------------------------------------------
    Egress side[port1]           |          Ingress side[lag1]
    ------------------------------------------------------------------
    ipv4's falls in 192.168.1.0     |        ipv4's falls in 10.1.0.0
    ------------------------------------------------------------------
    ipv6's falls in fc02::1:0  |   ipv6's falls in fc00:1::
    ------------------------------------------------------------------

### Testing Data Packet <!-- omit in toc --> 

This test should cover the data in the table below

DSCP_MAP_TABLE
|DSCP(Base on Inner)|TC to verify| Outgoing Queue(Port)|Priority(Tunnel)|
| ---- | ---- | -|-|
|8|0|0|0|
|0|1|1|0|
|33|8|1|0|
|3|3|3|2|
|4|4|4|6|
|46|5|5|0|
|48|7|7|7|

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
- Ingress encap packet=Ether(dst=ROUTER_MAC)/IP(dst=10.10.10.1,src=10.1.2.100,ip_dscp=``EXP_DSCP COL``, ip_ecn=``OUTER_ECN``)/IP(src=192.168.60.1,dst=192.168.1.1, ip_dscp=``DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()


#### IPV6 IN IPV4 Packet <!-- omit in toc --> 
- Expected egress packet = Ether(dst=00:01:01:99:01:01,src=ROUTER_MAC)/IP(src=fc02::60:1,dst=fc02::1:1,ip_dscp=``EXP_DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()
- Ingress encap packet=Ether(dst=ROUTER_MAC)/IP(src=10.1.2.100,dst=10.10.10.1,ip_dscp=``EXP_DSCP COL``, ip_ecn=``OUTER_ECN``)/IP(src=fc02::60:1,dst=fc02::1:1,ip_dscp=``EXP_DSCP COL``, ip_ecn=``INNER_ECN``)/TCP()


### Test steps: <!-- omit in toc --> 
- decap_dscp_remap:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)  
3. Generate 1000 packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
4. Send input packet from lag1.
5. Create the expected ipinip packet with ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE``. 
6. Recieve ipinip packet from port1. Compare it with the expected ipinip packet.

- decap_dscp_priority:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)  
3. According to DSCP_MAP_TABLE check the corrosponding priority_group packets stats on the possible ports(use sai_thrift_get_ingress_priority_group_stats with "SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS")
4. Generate 1000 packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
5. Send input packet from lag1.
6. Create the expected ipinip packet with ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE``. 
7. Recieve ipinip packet from port1. Compare it with the expected ipinip packet.
8. For received port check the corresponding priority_group packets stats

- decap_dscp_queue:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)  
3. According to DSCP_MAP_TABLE check the corrosponding queue's packets stats on the possible ports(use sai_thrift_get_queue_stats with "SAI_QUEUE_STAT_PACKETS")
4. Generate 1000 packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
5. Send input packet from lag1.
6. Create the expected ipinip packet with ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE``. 
7. Recieve ipinip packet from port1. Compare it with the expected ipinip packet.
8. For received port check the corresponding queue packets stats

- decap_pfc_pause:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map and priority map as basic [config](./config_data/config_t0.md)
3. Set all the dest ports with lossless buffer pool profile
4. According to DSCP_MAP_TABLE check the corrosponding priority_group packets stats on the possible ports(use sai_thrift_get_ingress_priority_group_stats with "SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS")
5. Check the corrosponding priority_group buffer state(SAI_INGRESS_PRIORITY_GROUP_STAT_SHARED_CURR_OCCUPANCY_BYTES, SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES, SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES)
6. Generate N packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly
7. Disable ports (set port SAI_PORT_ATTR_PKT_TX_ENABLE)
8. Send N packets from lag1 to fill the shared buffer
9. verify PFC pause frames are generated on expected priority(``DST:01:80:c2:00:00:01``). 
10. After filling up the buffer, send packets to check the PG drop counters to verify the counter increased as expected (SAI_INGRESS_PRIORITY_GROUP_STAT_DROPPED_PACKETS)

- decap_ecn_no_congestion:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map and priority map as basic [config](./config_data/config_t0.md)
3. Make sure set PCBB ECN configurations
4. Generate 100 packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly, according to DSCP_MAP_TABLE set the ``ip_ecn``
5. Send input packet from lag1.
5. Create expected ipinip packet with ``Inner_ECN`` ``Outer_ECN`` ``Inner DSCP`` and ``Outer dscp`` according to ``DSCP_MAP_TABLE``. 
6. Recieve ipinip packet from port1. Compare it with the expected ipinip packet.

- decap_ecn_congestion:

1. Make sure LAGs and NextHop groups set as basic [config](./config_data/config_t0.md) 
2. Make sure tunnel DSCP in PIPE mode, port and tunnel binding to the DSCP map, queue map, and priority map as basic [config](./config_data/config_t0.md)
3. Make sure set PCBB ECN configurations
4. Set all the dest port with lossless buffer pool profile
5. According to DSCP_MAP_TABLE check the corrosponding priority_group packets stats on the possible ports(use sai_thrift_get_ingress_priority_group_stats with "SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS")
5. Check the corrosponding priority_group buffer state(SAI_INGRESS_PRIORITY_GROUP_STAT_SHARED_CURR_OCCUPANCY_BYTES, SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES, SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES)
6. Generate N packets, take one row from the ``DSCP_MAP_TABLE``, set the ``ip_dscp`` accordingly, according to DSCP_MAP_TABLE set the ``ip_ecn``
7. Disable ports (set port SAI_PORT_ATTR_PKT_TX_ENABLE)
8. Send N packets from lag1 to fill the shared buffer 
9. verify PFC pause frames are generated on expected priority(``DST:01:80:c2:00:00:01``)
10. Enable ports (set port SAI_PORT_ATTR_PKT_TX_ENABLE)
11. Recieve ipinip packet from port1. Compare it with the expected ipinip packet.


