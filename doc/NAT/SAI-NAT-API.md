SAI NAT Proposal
-------------------------------------------------------------------------------
 Title       | SAI Network Address Translation
-------------|-----------------------------------------------------------------
 Authors     | Jai Kumar, Broadcom Inc.
             | Rita Hui, Microsoft Inc.
             | Matty Kadosh, Mellanox Inc.
 Status      | In review
 Type        | Standards track
 Created     | 04/15/2019
 Updated     | 05/10/2019
 SAI-Version | TBD

-------------------------------------------------------------------------------


# SAI NAT API
This document proposes set of API to configure NAT feature. API set is generic to configure various types of NAT. Besides configuration there is a need to read the NAT table for aging. Reading of data is achieved using the TAM GET API.

## 1.0 NAT Types
In basic NAT, SNAT is performed for outbound (internal zone to public zone) traffic, and DNAT is performed for inbound (public zone to internal zone) traffic. XDuring outbound NAT processing, SNAT is performed and the SIP of the packet is replaced.

In the reverse direction, DNAT operation is performed in which the DIP of the packet is replaced.

- Subnet Based NAT
Subnet based NAT replaces only the subnet prefix of the IP address, leaving the rest of the IP address unchanged. Subnet based NAT is a simpler implementation of basic NAT. 

- NAPT
Network address port translation is very similar to basic NAT with addition of layer 4 source port translation. For a given source IP address, protocol in the packet, NAPT is performed by replacing SIP and source port for the outbound traffic. In the reverse direction, DIP is replaced along with the destination port.

- Double NAT
Double NAT is a NAT variation in which both source and destination IP addresses are modified as a packet crosses the zone boundary. Double NAT is typically used to fully interconnect subnets in two incompatible address zones. One common case involves merging enterprise networks using private overlapping address spaces.

## 2.0 Configuring NAT zones
NAT zones are configured for translation. Only when packet crosses across NAT zones then NAT translation is done. SONiC will configure interfaces as a member of the zones. Zone ID is passed in the API setting up NAT rules.

A new uint8_t Zone ID attribute is added in sai_router_interface_attr_t.
> “SAI_ROUTER_INTERFACE_NAT_ZONE_ID”
> set_router_interface_attribute() API is used to set the NAT Zone ID.

## 3.0 Enabling NAT in switch
NAT feature is enabled at the switch level. SAI driver must handle this attribute and set corresponding HW register. Some of the settings can be
-	Global config register
-	Control settings for NAT miss packets for dynamic NAT

A new boolean field is added to sai_switch_attr in saiswitch.h
> “SAI_SWITCH_ATTR_NAT_ENABLE”
> set_switch_attribute() API is used to enable/disable NAT feature.

## 4.0 Enable Traps for SNAT and DNAT Miss Packets
Following two traps are added to hostif. Hostif driver MUST enable these traps for SNAT and DNAT miss in hw for receiving the packets. 
SNAT/DNAT miss packets are received on a regular netdev channel to the application.

- “SAI_HOSTIF_TRAP_TYPE_SNAT_MISS”
- “SAI_HOSTIF_TRAP_TYPE_DNAT_MISS”

## 5.0 NAT Object Workflow
 NAT or Network Address translation is used to avoid exposing the Internal or private IP addresses when the Host systems in an enterprise network or Datacenter communicate with external internet servers . The main advantage of using NAT is to reduce the usage of the Public IP addresses allocated for an enterprise or Data center.NAT feature allows the capability to change the Src IP address , Destination IP address , Src TCP/UDP port number or Destination TCP/UDP port number for an IP packet undergoing L3  routing in the switch.

### 5.1 Symmetric SNAT and DNAT
For directional symmetric flows there is always a corresponding DNAT entry for each SNAT entry. SAI APIs do not allow implicit installation of reverse direction NAT entry. SAI API MUST be invoked for both SNAT and DNAT entry to be installed.

> From Zone ID: 100
To Zone ID: 200
VRF: None
Packet Count: Enable
Byte Count: Enable
ExternalEndpoint: 65.55.42.1:1024 
InternalEndpoint: 10.0.0.1:6000

##### Step 1: Create a Source and Destination NAT Entry object:
```sh
sai_attribute_t nat_entry_attr[10];
nat_entry_t snat_entry;

nat_entry_attr[0].id = SAI_NAT_ENTRY_ATTR_NAT_TYPE;
nat_entry_attr[0].value = SAI_NAT_TYPE_SOURCE_NAT;

nat_entry_attr[1].id = SAI_NAT_ENTRY_ATTR_SRC_IP;
nat_entry_attr[1].value.u32 = 65.55.42.1; //example string

nat_entry_attr[2].id = SAI_NAT_ENTRY_ATTR_L4_SRC_PORT;
nat_entry_attr[2].value.u16 = 1024;

nat_entry_attr[3].id = SAI_NAT_ENTRY_ATTR_TO_ZONE;
nat_entry_attr[3].value.u32 = 200;

nat_entry_attr[4].id = SAI_NAT_ENTRY_ATTR_FROM_ZONE;
nat_entry_attr[4].value.u32 = 100;

nat_entry_attr[5].id = SAI_NAT_ENTRY_ATTR_ENABLE_PACKET_COUNT;
nat_entry_attr[5].value.bool = true;

nat_entry_attr[6].id = SAI_NAT_ENTRY_ATTR_ENABLE_BYTE_COUNT;
nat_entry_attr[6].value.bool = true;

attr_count = 7;

memset(&snat_etnry, 0, sizeof(nat_etnry));

snat_entry.data.key.src_ip = 10.0.0.1;
snat_entry.data.mask.src_ip = 0xffffffff;
snat_entry.data.key.l4_src_port = 6000;
snat_entry.data.mask.l4_src_port = 0xffff;
snat_entry.data.key.proto = 17;
snat_entry.data.mask.proto = 0xff;


create_nat_entry(&snat_entry, attr_count, nat_entry_attr);

```

##### Step 2: Create a Destination NAT Entry object:
```sh

sai_attribute_t nat_entry_attr[10];
nat_entry_t dnat_entry;

nat_entry_attr[0].id = SAI_NAT_ENTRY_ATTR_NAT_TYPE;
nat_entry_attr[0].value = SAI_NAT_TYPE_DESTINATION_NAT;

nat_entry_attr[1].id = SAI_NAT_ENTRY_ATTR_DST_IP;
nat_entry_attr[1].value.u32 = 10.0.0.1; //example string

nat_entry_attr[2].id = SAI_NAT_ENTRY_ATTR_L4_DST_PORT;
nat_entry_attr[2].value.u16 = 6000;

nat_entry_attr[3].id = SAI_NAT_ENTRY_ATTR_TO_ZONE;
nat_entry_attr[3].value.u32 = 100;

nat_entry_attr[4].id = SAI_NAT_ENTRY_ATTR_FROM_ZONE;
nat_entry_attr[4].value.u32 =200;

nat_entry_attr[5].id = SAI_NAT_ENTRY_ATTR_ENABLE_PACKET_COUNT;
nat_entry_attr[5].value.bool = true;

nat_entry_attr[6].id = SAI_NAT_ENTRY_ATTR_ENABLE_BYTE_COUNT;
nat_entry_attr[6].value.bool = true;

attr_count = 7;

memset(&dnat_etnry, 0, sizeof(nat_etnry));

dnat_entry.data.key.dst_ip = 65.55.42.1;
dnat_entry.data.mask.dst_ip = 0xffffffff;
dnat_entry.data.key.l4_dst_port = 1024;
dnat_entry.data.mask.l4_dst_port = 0xffff;
dnat_entry.data.key.proto = 17;
dnat_entry.data.mask.proto = 0xff;

create_nat_entry(&dnat_entry, attr_count, nat_entry_attr);

```

### 5.2 Double NAT
Double NAT is a nat variant where both the Src and Dest IP gets modified when a packet crosses the address zone. Generally used when conflicting addresses are used in the private networks which conflicts with the public IP addresses. Double NAT overrides SNAT or DNAT actions in the SAI pipeline.

> From Zone ID: 100
To Zone ID: 200
VRF: None
Packet Count: Enable
Byte Count: Enable
SNAT Entry
ExternalEndpoint: 138.76.28.1 
InternalEndpoint: 200.200.200.1
DNAT Entry
ExternalEndpoint: 200.200.200.100
InternalEndpoint: 172.16.1.100

##### Step 1: Create a Double NAT Entry:
```sh
sai_attribute_t nat_entry_attr[10];
nat_entry_t dbl_nat_entry;

nat_entry_attr[0].id = SAI_NAT_ENTRY_ATTR_NAT_TYPE;
nat_entry_attr[0].value = SAI_NAT_TYPE_DOUBLE_NAT;

nat_entry_attr[1].id = SAI_NAT_ENTRY_ATTR_SRC_IP;
nat_entry_attr[1].value.u32 = 138.76.28.1; //example string

nat_entry_attr[2].id = SAI_NAT_ENTRY_ATTR_DST_IP;
nat_entry_attr[2].value.u32 = 200.200.200.100; //example string

nat_entry_attr[3].id = SAI_NAT_ENTRY_ATTR_TO_ZONE;
nat_entry_attr[3].value.u32 = 200;

nat_entry_attr[4].id = SAI_NAT_ENTRY_ATTR_FROM_ZONE;
nat_entry_attr[4].value.u32 =100;

nat_entry_attr[5].id = SAI_NAT_ENTRY_ATTR_ENABLE_PACKET_COUNT;
nat_entry_attr[5].value.bool = true;

nat_entry_attr[6].id = SAI_NAT_ENTRY_ATTR_ENABLE_BYTE_COUNT;
nat_entry_attr[6].value.bool = true;

attr_count = 7;

memset(&dbl_nat_etnry, 0, sizeof(nat_etnry));

dbl_nat_entry.data.key.src_ip = 200.200.200.1;
dbl_nat_entry.data.mask.src_ip = 0xffffffff;
dbl_nat_entry.data.key.dst_ip = 172.16.1.100;
dbl_nat_entry.data.mask.dst_ip = 0xffffffff;

create_nat_entry(&dbl_nat_entry, attr_count, nat_entry_attr);

```
### 5.3 Subnet NAT
Subnet-based NAT replaces only the subnet prefix in the IP address, leaving the rest of the IP address unchanged. Subnet-based NAT is a simple implementation of basic NAT. For example, a private network is interconnected with a public network through a switch enabling subnet-based basic NAT. The public address for the private network (128.17.18.0/24 ) is 200.0.0.0/24. In the inbound direction, for a packet with DIP = 200.0.0.1, the subnet-based Basic NAT router will replace the subnet prefix 200.0.0/24 with new subnet prefix 128.17.18/24, and keep the host address .1/8. The translated SIP is 128.17.18.1.
In the outbound direction, the SIP is translated in same fashion.
> From Zone ID: 100
To Zone ID: 200
VRF: None
Packet Count: Enable
Byte Count: Enable
DNAT Entry
ExternalEndpoint: 200.0.0.0/24
InternalEndpoint: 128.17.18.0/24

##### Step 1: Create a Destination NAT Entry object:
```sh

sai_attribute_t nat_entry_attr[10];
nat_entry_t subnet_nat_entry;

nat_entry_attr[0].id = SAI_NAT_ENTRY_ATTR_NAT_TYPE;
nat_entry_attr[0].value = SAI_NAT_TYPE_DESTINATION_NAT;

nat_entry_attr[1].id = SAI_NAT_ENTRY_ATTR_DST_IP;
nat_entry_attr[1].value.u32 = 128.17.18.0; //example string

nat_entry_attr[2].id = SAI_NAT_ENTRY_ATTR_DST_IP_MASK;
nat_entry_attr[2].value.u32 = 0xffffff00;

nat_entry_attr[3].id = SAI_NAT_ENTRY_ATTR_TO_ZONE;
nat_entry_attr[3].value.u32 = 200;
nat_entry_attr[4].id = SAI_NAT_ENTRY_ATTR_FROM_ZONE;
nat_entry_attr[4].value.u32 =100;

nat_entry_attr[5].id = SAI_NAT_ENTRY_ATTR_ENABLE_PACKET_COUNT;
nat_entry_attr[5].value.bool = true;

nat_entry_attr[6].id = SAI_NAT_ENTRY_ATTR_ENABLE_BYTE_COUNT;
nat_entry_attr[6].value.bool = true;

attr_count = 7;

memset(&subnet_nat_etnry, 0, sizeof(nat_etnry));

subnet_nat_entry.data.key.dst_ip = 200.0.0.0;
subnet_nat_entry.data.mask.dst_ip = 0xffffff00;

create_nat_entry(&subnet_nat_entry, attr_count, nat_entry_attr);
```

## 6.0 NAT Exceptions
NAT exceptions can be defined using the NAT entry. In that case they will be placed in the SNAT/DNAT table. Operator MUST consider defining the exceptions so as not to collide with other NAT entries.
An override NAT exception can be created using the ACL rule. A new ACL action is defined in saiacl.h

> SAI_ACL_ENTRY_ATTR_ACTION_NO_NAT

In SAI pipeline if there is a ACL match with action NO_NAT and there is also a match in SNAT/DNAT table then ACL match result always takes precedence.

### 6.1 NAT exception using NAT Entry
> From Zone ID: 100
To Zone ID: 200
VRF: None
SNAT Exclusion
&nbsp;&nbsp;&nbsp;&nbsp;if (InternalEndpoint == 200.200.200.1:1023) AND (From_Zone == 100) AND (To_ZONE == 200)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;NO_NAT
##### Step 1: Create a SNAT Exception Entry:
```sh
sai_attribute_t nat_entry_attr[10];
nat_entry_t no_nat_entry;

nat_entry_attr[0].id = SAI_NAT_ENTRY_ATTR_NAT_TYPE;
nat_entry_attr[0].value = SAI_NAT_TYPE_NO_NAT;

nat_entry_attr[1].id = SAI_NAT_ENTRY_ATTR_TO_ZONE;
nat_entry_attr[1].value.u32 = 200;

nat_entry_attr[2].id = SAI_NAT_ENTRY_ATTR_FROM_ZONE;
nat_entry_attr[2].value.u32 =100;

attr_count = 3;

memset(&no_nat_etnry, 0, sizeof(nat_etnry));
no_nat_entry.data.key.src_ip = 200.200.200.1;
no_nat_entry.data.mask.src_ip = 0xffffffff;
no_nat_entry.data.key.l4_src_port = 1023;
no_nat_entry.data.mask.l4_src_port = 0xffff;

create_nat_entry(&no_nat_entry, attr_count, nat_entry_attr);
```


## NAT Per Zone Counters
Following per zone statistics is provided by the hardware for NAT feature.
- DNAT_DISACARDS/SNAT_DISCARDS – If Packet is not TCP/UDP and/or is a fragmentated IP packet. 
- DNAT_TRANSLATION_NEEDED/SNAT_TRANSLATION_NEEDED – If there is NAT table lookup miss for TCP/UDP packets, then this counter is incremented.
- DNAT_TRANSLATIONS/SNAT_TRANSLATIONS – If NAT table lookup is a hit, then this counter is incremented.

##### Step 1: Create a NAT Zone Counter Object for Source NAT Zone 100:
```sh
sai_attribute_t nat_zone_counter_attr[10];

nat_zone_counter_attr[0].id = SAI_NAT_ZONE_COUNTER_ATTR_NAT_TYPE;
nat_zone_counter _attr[0].value = SAI_NAT_TYPE_SOURCE_NAT;

nat_zone_counter_attr[1].id = SAI_NAT_ZONE_COUNTER_ATTR_ZONE_ID;
nat_zone_counter_attr[1].value.u32 = 100;

nat_zone_counter_attr[2].id = SAI_NAT_ZONE_COUNTER_ATTR_ENABLE_TRANSLATION_NEEDED;
nat_zone_counter_attr[2].value.bool = true;

nat_zone_counter_attr[3].id = SAI_NAT_ZONE_COUNTER_ATTR_ENABLE_DISCARD;
nat_zone_counter_attr[3].value.bool = true;

nat_zone_counter_attr[4].id = SAI_NAT_ZONE_COUNTER_ATTR_ENABLE_TRANSLATIONS;
nat_zone_counter_attr[4].value.bool = true;

attr_count = 5;

nat_zone100_counter_id = 
create_nat_zone_counter(switch_id, attr_count, nat_zone_counter_attr);
```


## Get Hit Bit, NAT Zone and Per NAT Entry Stats
Besides per NAT entry and per Zone counters there is hit bit value as. Hit bit is used to age the dynamic entries installed in HW.
In this proposal aging is performed by the NOS. NOS retrieves the hitbit table periodically at a configured time aging interface. If the hitbit is not SET for a given entry, the entry is removed from the NAT table using the sai_remove_entry_fn() API.

Per NAT entry stats and hit bit is retrieved by “get_nat_entry_attributes()”.
##### Step 1: Read SNAT entry hit bit and packet/byte count:
```sh
sai_attribute_t nat_entry_attr[10];

nat_entry_attr[0].id = SAI_NAT_ENTRY_ATTR_HIT_BIT;

nat_entry_attr[1].id = SAI_NAT_ENTRY_ATTR_BYTE_COUNT;

nat_entry_attr[2].id = SAI_NAT_ENTRY_ATTR_PACKET_COUNT;

attr_count = 3;

get_nat_entry_attributes(snat_entry, attr_count, nat_entry_attr);
```

##### Step 2: Read SNAT Zone 100 Translations Done Packet Count:
```sh
sai_attribute_t nat_zone_counter_attr[10];

nat_zone_counter_attr[0].id = SAI_NAT_ZONE_COUNTER_ATTR_NAT_TYPE;
nat_zone_counter _attr[0].value = SAI_NAT_TYPE_SOURCE_NAT;

nat_zone_counter_attr[1].id = SAI_NAT_ZONE_COUNTER_ATTR_ZONE_ID;
nat_zone_counter_attr[1].value.u32 = 100;

nat_zone_counter_attr[2].id = SAI_NAT_ZONE_COUNTER_ATTR_TRANSLATIONS_PACKET_COUNT;

get_nat_zone_counter_attributes(nat_zone100_counter_id, attr_count, nat_zone_counter_attr);
```



