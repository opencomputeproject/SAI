# SAI NAT API
This document proposes set of API to configure NAT feature. API set is generic to configure various types of NAT. Besides configuration there is a need to read the NAT table for aging. Reading of data is achieved using the TAM GET API.

## NAT Types
In basic NAT, SNAT is performed for outbound (internal zone to public zone) traffic, and DNAT is performed for inbound (public zone to internal zone) traffic. XDuring outbound NAT processing, SNAT is performed and the SIP of the packet is replaced.

In the reverse direction, DNAT operation is performed in which the DIP of the packet is replaced.

- Subnet Based NAT
Subnet based NAT replaces only the subnet prefix of the IP address, leaving the rest of the IP address unchanged. Subnet based NAT is a simpler implementation of basic NAT. 

- NAPT
Network address port translation is very similar to basic NAT with addition of layer 4 source port translation. For a given source IP address, protocol in the packet, NAPT is performed by replacing SIP and source port for the outbound traffic. In the reverse direction, DIP is replaced along with the destination port.

- Double NAT
Double NAT is a NAT variation in which both source and destination IP addresses are modified as a packet crosses the zone boundary. Double NAT is typically used to fully interconnect subnets in two incompatible address zones. One common case involves merging enterprise networks using private overlapping address spaces.

## Configuring NAT zones
NAT zones are configured for translation. Only when packet crosses across NAT zones then NAT translation is done. SONiC will configure interfaces as a member of the zones. Zone ID is passed in the API setting up NAT rules.

A new uint8_t Zone ID attribute is added in sai_router_interface_attr_t.
> “SAI_ROUTER_INTERFACE_NAT_ZONE_ID”
> set_router_interface_attribute() API is used to set the NAT Zone ID.

## Enabling NAT in switch
There is no separate API to enable NAT on a switch.
SAI NAT driver should detect the presence of NAT object in switch object. If non NULL value is present in switch object for NAT then enable the HW global register with NAT.

Similarly detect the absence of NAT object in switch and if NULL then disable the HW global register for NAT.

## NAT Object Workflow
Following is a logical representation of SAI NAT objects

>Example 1: SNAT and DNAT
From Zone ID: 100
To Zone ID: 200
VRF: None
STATIC_NAT|65.55.42.1:1024 
InternalEndpoint: 10.0.0.1:6000
STATIC_NAT|65.55.42.1:1025
InternalEndpoint: 10.0.0.2:6000
STATIC_NAT|65.55.42.1:1026
InternalEndpoint: 10.0.0.3:6000

##### Step 1: Create a Source NAT Entry object:
```sh
# First Translation
sai_nat_entry_attr_list[0].id = SAI_NAT_ENTRY_ATTR_FROM_IP;
sai_nat_entry_attr_list[0].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_nat_entry_attr_list[0].value.ipaddr.ip4 = 10.0.0.1; 

>sai_nat_entry_attr_list[1].id = SAI_NAT_ENTRY_ATTR_TO_IP;
sai_nat_entry_attr_list[1].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_nat_entry_attr_list[1].value.ipaddr.ip4 = 65.55.42.1; 

>sai_nat_entry_attr_list[2].id = SAI_NAT_ENTRY_ATTR_FROM_PORT;
sai_nat_entry_attr_list[2].value.u16 = 6000;

sai_nat_entry_attr_list[3].id = SAI_NAT_ENTRY_ATTR_TO _PORT;
sai_nat_entry_attr_list[3].value.u16 = 1024;

sai_nat_entry_attr_list[4].id = SAI_NAT_ENTRY_ATTR_IP_PROTOCOL;
sai_nat_entry_attr_list[4].value.u8 = 17; 

# Second Translation
sai_nat_entry_attr_list[5].id = SAI_NAT_ENTRY_ATTR_FROM_IP;
sai_nat_entry_attr_list[5].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_nat_entry_attr_list[5].value.ipaddr.ip4 = 10.0.0.2; 

sai_nat_entry_attr_list[6].id = SAI_NAT_ENTRY_ATTR_TO_IP;
sai_nat_entry_attr_list[6].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_nat_entry_attr_list[6].value.ipaddr.ip4 = 65.55.42.1; 

sai_nat_entry_attr_list[7].id = SAI_NAT_ENTRY_ATTR_FROM_PORT;
sai_nat_entry_attr_list[7].value.u16 = 6000;

sai_nat_entry_attr_list[8].id = SAI_NAT_ENTRY_ATTR_TO _PORT;
sai_nat_entry_attr_list[8].value.u16 = 1025;

sai_nat_entry_attr_list[9].id = SAI_NAT_ENTRY_ATTR_IP_PROTOCOL;
sai_nat_entry_attr_list[9].value.u8 = 17; 

# Third Translation
sai_nat_entry_attr_list[10].id = SAI_NAT_ENTRY_ATTR_FROM_IP;
sai_nat_entry_attr_list[10].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_nat_entry_attr_list[10].value.ipaddr.ip4 = 10.0.0.3; 

sai_nat_entry_attr_list[11].id = SAI_NAT_ENTRY_ATTR_TO_IP;
sai_nat_entry_attr_list[11].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_nat_entry_attr_list[11].value.ipaddr.ip4 = 65.55.42.1; 

sai_nat_entry_attr_list[12].id = SAI_NAT_ENTRY_ATTR_FROM_PORT;
sai_nat_entry_attr_list[12].value.u16 = 6000;

sai_nat_entry_attr_list[13].id = SAI_NAT_ENTRY_ATTR_TO _PORT;
sai_nat_entry_attr_list[13].value.u16 = 1026;

sai_nat_entry_attr_list[14].id = SAI_NAT_ENTRY_ATTR_IP_PROTOCOL;
sai_nat_entry_attr_list[14].value.u8 = 17; 

nat_entry_attr_count = 15;
sai_create_nat_entry_fn(
    &sai_src_nat_entry_obj,
    switch_id,
    nat_entry_attr_count,
    sai_nat_entry_attr_list);
```

##### Step 2: Create a Destination NAT Entry object:
```sh
# First Translation
sai_nat_entry_attr_list[0].id = SAI_NAT_ENTRY_ATTR_FROM_IP;
sai_nat_entry_attr_list[0].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_nat_entry_attr_list[0].value.ipaddr.ip4 = 65.55.42.1; 

sai_nat_entry_attr_list[1].id = SAI_NAT_ENTRY_ATTR_TO_IP;
sai_nat_entry_attr_list[1].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_nat_entry_attr_list[1].value.ipaddr.ip4 = 10.0.0.1; 

sai_nat_entry_attr_list[2].id = SAI_NAT_ENTRY_ATTR_FROM_PORT;
sai_nat_entry_attr_list[2].value.u16 = 1024;

sai_nat_entry_attr_list[3].id = SAI_NAT_ENTRY_ATTR_TO _PORT;
sai_nat_entry_attr_list[3].value.u16 = 6000; 

sai_nat_entry_attr_list[4].id = SAI_NAT_ENTRY_ATTR_IP_PROTOCOL;
sai_nat_entry_attr_list[4].value.u8 = 17; 

# Second Translation
sai_nat_entry_attr_list[5].id = SAI_NAT_ENTRY_ATTR_FROM_IP;
sai_nat_entry_attr_list[5].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_nat_entry_attr_list[5].value.ipaddr.ip4 = 65.55.42.1; 

sai_nat_entry_attr_list[6].id = SAI_NAT_ENTRY_ATTR_TO_IP;
sai_nat_entry_attr_list[6].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_nat_entry_attr_list[6].value.ipaddr.ip4 = 10.0.0.2; 

sai_nat_entry_attr_list[7].id = SAI_NAT_ENTRY_ATTR_FROM_PORT;
sai_nat_entry_attr_list[7].value.u16 = 1025;

sai_nat_entry_attr_list[8].id = SAI_NAT_ENTRY_ATTR_TO _PORT;
sai_nat_entry_attr_list[8].value.u16 = 6000;

sai_nat_entry_attr_list[9].id = SAI_NAT_ENTRY_ATTR_IP_PROTOCOL;
sai_nat_entry_attr_list[9].value.u8 = 17; 

# Third Translation
sai_nat_entry_attr_list[10].id = SAI_NAT_ENTRY_ATTR_FROM_IP;
sai_nat_entry_attr_list[10].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_nat_entry_attr_list[10].value.ipaddr.ip4 = 65.55.42.1;  

sai_nat_entry_attr_list[11].id = SAI_NAT_ENTRY_ATTR_TO_IP;
sai_nat_entry_attr_list[11].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
sai_nat_entry_attr_list[11].value.ipaddr.ip4 = 10.0.0.3; 

sai_nat_entry_attr_list[12].id = SAI_NAT_ENTRY_ATTR_FROM_PORT;
sai_nat_entry_attr_list[12].value.u16 = 1026;

sai_nat_entry_attr_list[13].id = SAI_NAT_ENTRY_ATTR_TO _PORT;
sai_nat_entry_attr_list[13].value.u16 = 6000;

sai_nat_entry_attr_list[14].id = SAI_NAT_ENTRY_ATTR_IP_PROTOCOL;
sai_nat_entry_attr_list[14].value.u8 = 17; 

nat_entry_attr_count = 15;
sai_create_nat_entry_fn(
    &sai_dst_nat_entry_obj,
    switch_id,
    nat_entry_attr_count,
    sai_nat_entry_attr_list);
```

##### Step 3.1: Create a SNAT counter object:
```sh
sai_nat_counter_list[0].id = SAI_NAT_COUNTER_ATTR_NAT_TYPE;
sai_nat_counter_list[0].value.u32 = SAI_NAT_SOURCE_NAT;

sai_nat_counter_list[1].id = SAI_NAT_COUNTER_ATTRZONE_ID;
sai_nat_counter_list[1].value.u8 = 100; 

sai_nat_counter_list[2].id = SAI_NAT_COUNTER_ATTR_ENABLE_DISCARD;
sai_nat_counter_list[2].value = true;
sai_nat_counter_list[3].id = SAI_NAT_COUNTER_ATTR_ENABLE_TRANSLATION_NEEDED;
sai_nat_counter_list[3].value = true;

sai_nat_counter_list[4].id = SAI_NAT_COUNTER_ATTR_ENABLE_TRANSLATIONS;
sai_nat_counter_list[4].value = true;

nat_attr_count = 5;
sai_create_nat_counter_fn(
    &sai_src_nat_counter_obj,
    switch_id,
    nat_attr_count,
    sai_nat_attr_list);
```

##### Step 3.2: Create a SNAT object:
```sh
sai_nat_attr_list[0].id = SAI_NAT_ATTR_NAT_TYPE;
sai_nat_attr_list[0].value.u32 = SAI_NAT_SOURCE_NAT;

sai_nat_attr_list[1].id = SAI_NAT_ATTR_FROM_ZONE_ID;
sai_nat_attr_list[1].value.u8 = 100; 

sai_nat_attr_list[2].id = SAI_NAT_ATTR_TO_ZONE_ID;
sai_nat_attr_list[2].value.u8 = 200; 

sai_nat_attr_list[3].id = SAI_NAT_ATTR_NAT_ENTRY_LIST;
sai_nat_attr_list[3].value.objectlist.count = 1;
sai_nat_attr_list[3].value.objectlist.list[0] = sai_src_nat_entry_obj; 

sai_nat_attr_list[4].id = SAI_NAT_ATTR_NAT_COUNTER_LIST;
sai_nat_attr_list[4].value.objectlist.count = 1;
sai_nat_attr_list[4].value.objectlist.list[0] = sai_src_nat_counter_obj; 

nat_attr_count = 5;
sai_create_nat_fn(
    &sai_src_nat_obj,
    switch_id,
    nat_attr_count,
    sai_nat_attr_list);
```

##### Step 4: Create a DNAT object:
```sh
sai_nat_attr_list[0].id = SAI_NAT_ATTR_NAT_TYPE;
sai_nat_attr_list[0].value.u32 = SAI_NAT_TYPE_DESTINATION_NAT;

sai_nat_attr_list[1].id = SAI_NAT_ATTR_FROM_ZONE_ID;
sai_nat_attr_list[1].value.u8 = 200; 

sai_nat_attr_list[2].id = SAI_NAT_ATTR_TO_ZONE_ID;
sai_nat_attr_list[2].value.u8 = 100; 

sai_nat_attr_list[3].id = SAI_NAT_ATTR_NAT_ENTRY_LIST;
sai_nat_attr_list[3].value.objectlist.count = 1; 
sai_nat_attr_list[3].value.objectlist.list[0] = sai_dst_nat_entry_obj; 

nat_attr_count = 4;
    sai_create_nat_fn(
    &sai_dst_nat_obj,
    switch_id,
    nat_attr_count,
    sai_nat_attr_list);
```
## NAT Per flow and Zone Counters
Following per zone statistics is provided by the hardware for NAT feature.
- DNAT_DISACARDS/SNAT_DISCARDS – If Packet is not TCP/UDP and/or is a fragmentated IP packet. 
- DNAT_TRANSLATION_NEEDED/SNAT_TRANSLATION_NEEDED – If there is NAT table lookup miss for TCP/UDP packets, then this counter is incremented.
- DNAT_TRANSLATIONS/SNAT_TRANSLATIONS – If NAT table lookup is a hit, then this counter is incremented.

Counters can be attached to per NAT entry to gather per flow statistics. Per NAT entry stat is enabled in the entry attributes.

> Per zone counters are created using sai_create_nat_counter_fn() API.

NAT counters can be extracted using TAM streaming interface or by using TAM GET API.
This document proposes to use TAM GET API for time to market reasons. Same NAT counters and data format can be streamed as well using SAI TAM driver.

## GET API for reading NAT Table entries and hit bit
Besides NAT counters there is a HITBIT table as well which is used to age the NAT entries.

Dynamic NAT entries for a given flow need to be aged once the flow ceases to exist. In this proposal aging is performed by the NOS. NOS retrieves the hitbit table periodically at a configured time aging interface. If the hitbit is not SET for a given entry, the entry is removed from the NAT table using the *sai_remove_nat_entry_fn()* API.

HITBIT table data is retrieved using the TAM proposed generic GET API. 
Advantage of using TAM GET API is that normalized data for the hitibit data can also be streamed to local NOS, or to an external collector or to multi chip system.
Some hardware may not perform "clear on read" or NOS may want to control the operation "clear on read". For this reason a separate boolean attribute "CLEAR_ON_READ" is specified 

TAM object is bound to NAT object for stats and hitbit table.

Following is a simple GET API invocation for pulling NAT entry creates under source and destination nat entry objects. One or more objects can be specified in a single invocation. API is name 


> sai_tam_telemetry_get_data(switch_id, objlist, clear_on_read, buffer_size, buffer);


##### Invoke GET API
```sh
objlist.list[0].id = SAI_OBJECT_TYPE_NAT_ENTRY;
objlist.list[0].value = sai_src_nat_obj;
objlist.list[1].id = SAI_OBJECT_TYPE_NAT_ENTRY;
objlist.list[1].value = sai_dst_nat_obj;

objlist.count = 2;

buffer_size = 2048; /* Bytes */
buffer = malloc(buffer_size);
sai_tam_telemetry_pull_data(
    switch_id,
    attr_count,
    attr_list,
    buffer_size,
    buffer);
```




