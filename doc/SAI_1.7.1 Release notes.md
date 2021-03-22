# SAI 1.7.1 Release Notes

The Switch Abstraction Interface(SAI) defines the APIs to provide a vendor-independent way of controlling forwarding elements, such as a switching ASIC, an NPU or a software switch in a uniform manner. This release document covers the SAI API changes from SAI tag 1.6.3 to SAI tag 1.7.1. The previous release notes corresponding to SAI tag 1.6.3 is available at [SAI 1.6.3 release notes](https://github.com/opencomputeproject/SAI/blob/master/doc/SAI_1.6.3_ReleaseNotes.md) 

This document explains the new SAI features as well as the enhancements and the bug fixes on existing features. 

### Updates for Linux MACsec driver and 802.1AE-2018    	

This commit contains following non-backward compatible changes

       Replace attribute SAI_MACSEC_SC_ATTR_MACSEC_SSCI with SAI_MACSEC_SA_ATTR_MACSEC_SSCI.
Reason:
IEEE standards 802.1AE-2018 and 802.1X-2020 both state that SSCI is a property of the SA not the SC. SSCI can only be assigned once the full Live Peer List has been exchanged via EAPOL between peers, which will occur after the KaY has already created the transmit SC for the actor.

`````
 * @brief SSCI value for this Secure Association
 *
 * Valid when SAI_MACSEC_SC_ATTR_MACSEC_CIPHER_SUITE == SAI_MACSEC_CIPHER_SUITE_GCM_AES_XPN_128 or SAI_MACSEC_SC_ATTR_MACSEC_CIPHER_SUITE == SAI_MACSEC_CIPHER_SUITE_GCM_AES_XPN_256.
 *
 * @type sai_uint32_t
 * @flags MANDATORY_ON_CREATE | CREATE_ONLY
`````
SAI_MACSEC_SA_ATTR_MACSEC_SSCI,
       Replace attribute SAI_MACSEC_SA_ATTR_ENCRYPTION_ENABLE with SAI_MACSEC_SC_ATTR_ENCRYPTION_ENABLE
Reason:
As seen from both the open source KaY (wpa-supplicant) and SecY (Linux MACsec driver), the encryption attribute is typically an attribute of the SC, not the SA. This alignment can be retained by shifting the SAI MACsec definition of the encryption attribute from the SA to the SC. This should allow for a cleaner, more direct translation between the SAI MACsec layer and the underlying platform driver.

`````
 * @brief True means encryption is enabled.  False means encryption is disabled.
 *
 * @type bool
 * @flags CREATE_AND_SET
 * @default true
`````
SAI_MACSEC_SC_ATTR_ENCRYPTION_ENABLE,
       Replace attribute SAI_MACSEC_SC_ATTR_MACSEC_XPN64_ENABLE and SAI_MACSEC_SA_ATTR_SAK_256_BITS with SAI_MACSEC_SC_ATTR_MACSEC_CIPHER_SUITE
Reason:
As seen from both the open source KaY (wpa-supplicant) and SecY (Linux MACsec driver), the MACsec cipher suite is typically a single attribute of the SC. This alignment can be retained by combining the two properties of the MACsec cipher suite (SAK length and XPN enable) of the SAI MACsec definition into a single attribute of the SC. This should allow for a cleaner, more direct translation between the SAI MACsec layer and the underlying platform driver.

`````
 * @brief Cipher suite for this Secure Channel.
 *
 * @type sai_macsec_cipher_suite_t
 * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
`````
SAI_MACSEC_SC_ATTR_MACSEC_CIPHER_SUITE,

The PR related to this feature is available at [PR#1156](https://github.com/opencomputeproject/SAI/pull/1156) ;[PR#1172](https://github.com/opencomputeproject/SAI/pull/1172)

### Tunnel loopback packet action as resource	
SAI_TUNNEL_ATTR_LOOPBACK_PACKET_ACTION is marked as resource and can be queried using the sai_object_type_get_availability API.
`````
Example
sai_attribute_t *attr;
sai_status_t status;
uint64_t count;

attr[0].id = SAI_TUNNEL_ATTR_TYPE;
attr[0].value.s32 = SAI_TUNNEL_TYPE_VXLAN;

attr[1].id = SAI_TUNNEL_ATTR_LOOPBACK_PACKET_ACTION;
attr[1].value.s32 = SAI_PACKET_ACTION_DROP;

status = sai_object_type_get_availability(g_switch_id, SAI_OBJECT_TYPE_TUNNEL, 2, &attr, &count);
`````

This API will return number of vxlan tunnels which can forward with packet action as drop.
SONiC when invokes this API with out any tunnels configured
SAI will return 4
SONiC will configure 1 tunnel with packet action as drop and call the query API.
SAI will return 3 and so forth

The PR related to this feature is available at [PR#1163](https://github.com/opencomputeproject/SAI/pull/1163)

### PFC pause duration in microseconds 
Pause duration has been added as below

     * PFC pause duration for RX and TX per PFC priority in micro seconds 
     * RX pause duration for certain priority is a the duration in micro seconds converted
     * from quanta in ingress pause frame for that priority (a pause frame received by the
     * switch).
     * While TX pause duration for certain priority is the duration in micro seconds converted
     * from quanta in egress pause frame for that priority (a pause frame sent by the switch).

This is acheieved by adding attribute SAI_PORT_STAT_PFC_7_TX_PAUSE_DURATION for enum sai_port_stat_t in saiport.h 

The PR related to this feature is available at [PR#1143](https://github.com/opencomputeproject/SAI/pull/1143); [PR#1144](https://github.com/opencomputeproject/SAI/pull/1144); [PR#1145](https://github.com/opencomputeproject/SAI/pull/1145)

### Query statistics capability   
Query statistics capability adds the ability to query statistics values capability per object type - meaning which statistics enum values are implemented per object type
Currently, these is not exposed, and in order to check capabilities, the user has to retry each value, one by one. This is achieved by adding new function sai_query_stats_capability for saiobject.h also by adding new function sai_stat_capability_t in saitypes.h 

The PR related to this feature is available at [PR#1148](https://github.com/opencomputeproject/SAI/pull/1148)

### Separate XPN configuration attribute from read-only attribute  
Added a new read-only current XPN (available for both ingress and egress) and separate from the egress attribute for setting initial XPN value.
Renamed ingress set-and-clear attribute MINIMUM_XPN to MINIMUM_INGRESS_XPN.

The PR related to this feature is available at	[PR#1169](https://github.com/opencomputeproject/SAI/pull/1169)

### Add packet allocate, free function to allow for 0 copy packet TX  
This PR enables efficient packet TX, by leveraging zero copy packet memory allocation when available.<br>
<br>
Sending a packet requires allocating it first. Typically, this requires allocating from a special pool of DMAed memory. For this a ASIC vendor SDK may support custom allocate and free functions. So mirror those in the SAI spec. 
<br>
Without such a ability we are left to allocate heap memory, pass it to send_hostif_packet_fn, which then allocates from the aforementioned DMA memory, copies the contents of heap memory and sends the packet along. This causes us to have an extra allocation and copy. This can induce a substantial slowdown -~30% in our measurements.
<br>
This is achieved by
<br>
- Adding SAI_HOSTIF_PACKET_ATTR_ZERO_COPY_TX in sai_hostif_packet_attr_t
- Adding functions sai_allocate_hostif_packet_fn, sai_free_hostif_packet_fn
 in saihostif.h
<br>
Furthermore total DMA pool size can be queried via SAI_SWITCH_ATTR_PACKET_DMA_MEMORY_POOL_SIZE in sai_switch_attr_t on saiswitch.h

The PR related to this feature is available at [PR#1137](https://github.com/opencomputeproject/SAI/pull/1137)

### Add label attribute for LAG and virtual router 
Since LAG and virtual router can be created without any unique mandatory attributes, anchor attribute can be used to uniquely identify specified objects. This will be very useful in SONiC warm boot scenario, for example to identify 2 empty LAGs which are present on the device after reboot.
This attribute doesn't correspond to any internal SAI vendor device resources, and at this point it don't need to be implemented by any vendor, it implementation at create function internals CAN be skipped without posting error. This attribute is considered as user data attached to specific object.

The PR related to this feature is available at [PR#1158](https://github.com/opencomputeproject/SAI/pull/1158)

### Add neighbor and tunnel term ip addr family for CRM 
Generic resource monitoring, done by API sai_object_type_get_availability, introduced the ability to query avaialble resources per object type, and provide additional attributes as query params. This introduces 2 such params - neighbor entry IP address family, to be able to query avaialble IPv4 and IPv6 neigbor entries, and tunnel termination table entry IP address family, to be able to query available IPv4 and IPv6 tunnel termination table entries. Introduced attributes are read only, marked as resourcetype, and used only for this query by adding attribute SAI_NEIGHBOR_ENTRY_ATTR_IP_ADDR_FAMILY in enum sai_neighbor_entry_attr_t for saineighbor.h and attribute SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_IP_ADDR_FAMILY in enum sai_tunnel_term_table_entry_attr_t for saitunnel.h

The PR related to this feature is available at [PR#1151](https://github.com/opencomputeproject/SAI/pull/1151)

### UDF Field Qualifier in ACL 	
This feature brings in change to be able to specify field extracted by UDF group as a qualifier in ACL Table for typedef enum _sai_acl_table_attr_t. A UDF group that will be used in ACL must be set to SAI_UDF_GROUP_GENERIC as the group type. 

The PR related to this feature is available at [PR#1122](https://github.com/opencomputeproject/SAI/pull/1122)

### Packet loopback action for tunnels 
Added loopback action as switch attribute: This will apply to all the tunnels configured and as a tunnel specific action. Added SAI_OUT_DROP_REASON_TUNNEL_LOOPBACK_PACKET_DROP under typedef enum _sai_out_drop_reason_t for saidebugcounter.h and  SAI_SWITCH_ATTR_TUNNEL_LOOPBACK_PACKET_ACTION, under typedef enum _sai_switch_attr_t for saiswitch.h and SAI_TUNNEL_ATTR_LOOPBACK_PACKET_ACTION under typedef enum _sai_tunnel_attr_t for saitunnel.h 

The PR related to this feature is available at [PR#1152](https://github.com/opencomputeproject/SAI/pull/1152)

### Allow for multiple interface types  
This allows for provisioning and advertising a list of multiple IEEE ethernet protocols on a port, by adding SAI_PORT_ATTR_ADVERTISED_INTERFACE_TYPE in enum sai_port_attr_t for saiport.h

The PR related to this feature is available at [PR#1135](https://github.com/opencomputeproject/SAI/pull/1135)

### New Interface type based on transceivers variants  
New interface types as been added for saiport.h under enum sai_port_interface_type_t 

The PR related to this feature is available at [PR#1139](https://github.com/opencomputeproject/SAI/pull/1139)

### Add next hop group member counter	    
This features adds the ability to attach a SAI counter to next hop group member, to count traffic routed to it by adding attribute SAI_NEXT_HOP_GROUP_MEMBER_ATTR_COUNTER_ID in enum sai_next_hop_group_member_attr_t for sainexthopgroup.h

The PR related to this feature is available at [PR#1150](https://github.com/opencomputeproject/SAI/pull/1150)

### Masked hash with optional ordering 
This feature has been enabled for binary backward compatibility and removed the enum assigned values numerical value indicating the order in which the field is fed to the hash engine

The PR related to this feature is available at [PR#1075](https://github.com/opencomputeproject/SAI/pull/1075)

### Additional Port Interface Types
This feature provides more options for port interface types to allow more granular programmability of port settings. These interface types are part of IEEE 802.3 standard. This is achieved by adding attribute SAI_PORT_INTERFACE_TYPE for enum _sai_port_interface_type_t in saiport.h

GMII - Gigabit
SFI - 10 Gigabit
XLAUI - 40 Gigabit
CAUI - 100 Gigabit

Each of these have different electrical characteristics and different from available port interface types.

The PR related to this feature is available at [PR#1098](https://github.com/opencomputeproject/SAI/pull/1098)

### Policy based hash

Sai defines two metadata values for ECMP and LAG hash. The fields that are used for their calculation are defined at the switch level with attributes `SAI_SWITCH_ATTR_ECMP_HASH` and `SAI_SWITCH_ATTR_LAG_HASH`. There are also hash objects that can be assigned for some basic packet types.
This is achieved by adding the attribute SAI_ACL_ACTION_TYPE_SET_LAG_HASH_ID and SAI_ACL_ACTION_TYPE_SET_ECMP_HASH_ID in enum sai_acl_action_type_t for saiacl.h 

The PR related to this feature is available at [PR#1074](https://github.com/opencomputeproject/SAI/pull/1074)


