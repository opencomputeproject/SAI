# SAI 1.9.1 Release Notes

The Switch Abstraction Interface(SAI) defines the APIs to provide a vendor-independent way of controlling forwarding elements, such as a switching ASIC, an NPU or a software switch in a uniform manner. This release document covers the SAI API changes from SAI tag 1.8.1 to SAI tag 1.9.1. The previous release notes corresponding to SAI tag 1.8.1 is available at [SAI 1.8.1 release notes](https://github.com/opencomputeproject/SAI/blob/master/doc/SAI_1.8.1_ReleaseNotes.md) 

This document explains the new SAI features as well as the enhancements and the bug fixes on existing features. 


### Class-Based Forwarding 

This PR defines class-based forwarding. It contains two aspects:

	* Assignment of a Forwarding Class to a packet, via QOS map or ACL
	* New next-hop group type: SAI_NEXT_HOP_GROUP_TYPE_CLASS_BASED

Where member selection is based on the forwarding-class of the packet. saiacl is updated with new type def SAI_ACL_ENTRY_ATTR_ACTION_SET_FORWARDING_CLASS and sainexthopgroup.h with SAI_NEXT_HOP_GROUP_TYPE_CLASS_BASED


````````````````

sainexthopgroup.h


typedef enum _sai_next_hop_group_map_type_t
{
    /** Next hop group map forwarding-class to index */
    SAI_NEXT_HOP_GROUP_MAP_TYPE_FORWARDING_CLASS_TO_INDEX

} sai_next_hop_group_map_type_t;

typedef enum _sai_next_hop_group_map_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_NEXT_HOP_GROUP_MAP_ATTR_START,

    /**
     * @brief Next hop group map type
     *
     * @type sai_next_hop_group_map_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */

    SAI_NEXT_HOP_GROUP_MAP_ATTR_TYPE = SAI_NEXT_HOP_GROUP_MAP_ATTR_START,

    /**
     * @brief Next hop group entries associated with this map.
     *
     * @type sai_map_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_NEXT_HOP_GROUP_MAP_ATTR_MAP_TO_VALUE_LIST,

    /**
     * @brief End of attributes
     */
    SAI_NEXT_HOP_GROUP_MAP_ATTR_END,

    /** Custom range base value */
    SAI_NEXT_HOP_GROUP_MAP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_NEXT_HOP_GROUP_MAP_ATTR_CUSTOM_RANGE_END

} sai_next_hop_group_map_attr_t;
	
	
	 @brief Create next hop group map
 *
 * @param[out] next_hop_group_map_id Next hop group map id
 * @param[in] switch_id Switch ID
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_next_hop_group_map_fn)(
        _Out_ sai_object_id_t *next_hop_group_map_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove next hop group map
 *
 * @param[in] next_hop_group_map_id Next hop group map ID
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_next_hop_group_map_fn)(
        _In_ sai_object_id_t next_hop_group_map_id);

/**
 * @brief Set Next Hop Group map attribute
 *
 * @param[in] next_hop_group_map_id Next hop group map ID
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_next_hop_group_map_attribute_fn)(
        _In_ sai_object_id_t next_hop_group_map_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get next hop group map attribute
 *
 * @param[in] next_hop_group_map_id Next hop group map ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_next_hop_group_map_attribute_fn)(
        _In_ sai_object_id_t next_hop_group_map_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);
		
	sai_create_next_hop_group_map_fn           create_next_hop_group_map;
    sai_remove_next_hop_group_map_fn           remove_next_hop_group_map;
    sai_set_next_hop_group_map_attribute_fn    set_next_hop_group_map_attribute;
    sai_get_next_hop_group_map_attribute_fn    get_next_hop_group_map_attribute;
	
````````````````	

````````````````	
saiport.h


 /**
     * @brief Enable DSCP -> Forwarding Class MAP on port
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_DSCP_TO_FORWARDING_CLASS_MAP,

    /**
     * @brief Enable EXP -> Forwarding Class MAP on port
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_MPLS_EXP_TO_FORWARDING_CLASS_MAP,
	
````````````````	

The PR related to this feature is available at PR#[1193](https://github.com/opencomputeproject/SAI/pull/1193)


### Adding IPsec support

A new IPsec API saiipsec.h has been added part of this current release. For more detail, please refer to [SAI_IPsec_API](https://github.com/opencomputeproject/SAI/doc/IPsec/SAI_IPsec_API_proposal.md)

The PR related to this feature is available at PR#[1206](https://github.com/opencomputeproject/SAI/pull/1206)


### Updates to SRv6 programming model and related objects/attributes

This implementation will bring SRv6 APIs and attributes in-line with the latest RFCs and SAI MPLS/tunnel pipeline model Also, this implementation doesn't explicitly address the functionality related to Traffic Class, Hop-Limit, ECN marking and load-balancing hash calculation. It's assumed that existing attributes for other IPv6 tunnels will apply to the SRv6 as well.

There are few changes in following API's sainexthop.h, saiobject.h, saisegmentroute.h,  saisrv6.h, saiswitch.h, saitunnel.h and saitypes.h for this implementation

The PR related to this feature is available at PR#[1231](https://github.com/opencomputeproject/SAI/pull/1231)


### New attribute to start fw broadcast process 

This additional attribute is to support 5-step FW broadcast process. This attribute is only applicable to PHY and its valid when broadcast is enabled. This is mainly used when the design has multiple PHYs and user would like to take advantage of broadcast FW download to save MDIO cycles and time.

````````````````

saiswitch.h 

 /**
     * @brief Slave MDIO Address list
     *
     * Configure list of slave MDIO addresses for firmware download in Broadcast mode.
     * The sequence for firmware download in broadcast mode is as follows:
     * 1. For each MDIO master, call sai_create_switch() and pass the list of slave MDIO addresses.
     * In this step, gearbox will upgrade the firmware on all PHY devices including master and slave.
     *
     * 2. Call sai_create_switch() on all slave PHY devices with #SAI_SWITCH_ATTR_FIRMWARE_LOAD_TYPE = SAI_SWITCH_FIRMWARE_LOAD_TYPE_SKIP,
     * which will already have had their firmware upgraded.
     *
     * @type sai_u8_list_t
     * @flags CREATE_ONLY
     * @default empty
     * @validonly SAI_SWITCH_ATTR_FIRMWARE_DOWNLOAD_BROADCAST == true
     */
    SAI_SWITCH_ATTR_SLAVE_MDIO_ADDR_LIST,


````````````````
The PR related to this feature is available at PR#[1232](https://github.com/opencomputeproject/SAI/pull/1232)


### ISIS trap support for SAI

This feature extends SAI-v1.8 capabilities to support new ISIS header SAI_HOSTIF_TRAP_TYPE_ISIS.

````````````````
saihostif.h

    /**
     * @brief Intermediate System-to-Intermediate System (IS-IS) protocol
     *
     * Traffic:
     * L1 IS: 01:80:c2:00:00:14, All Level 1 Intermediate Systems Address
     * L2 IS: 01:80:c2:00:00:15, All Level 2 Intermediate Systems Address
     * All IS: 09:00:2b:00:00:05, All Intermediate System Network Entities address
     *
     * Default packet action is forward
     */
    SAI_HOSTIF_TRAP_TYPE_ISIS = 0x00002014,
	
	
````````````````
The PR related to this feature is available at PR#[1238](https://github.com/opencomputeproject/SAI/pull/1238)


### Add support for programming My MAC table separately from RIF

RIF Source MAC is used to set the Source MAC address in transmitted packet. It can also be used to match the Destination MAC address in received packet. This proposal provides the capability to program the MAC address used in the receive path.

````````````````

saimymac.h

/**
 * Copyright (c) 2014 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
 *
 * @file    saimymac.h
 *
 * @brief   This module defines SAI My MAC
 */

#if !defined (__SAIMYMAC_H_)
#define __SAIMYMAC_H_

#include <saitypes.h>

/**
 * @brief My MAC entry attribute IDs
 */
typedef enum _sai_my_mac_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_MY_MAC_ATTR_START,

    /**
     * @brief Priority
     *
     * Value must be in the range defined in
     * \[#SAI_SWITCH_ATTR_MY_MAC_TABLE_MINIMUM_PRIORITY,
     * #SAI_SWITCH_ATTR_MY_MAC_TABLE_MAXIMUM_PRIORITY\]
     * (default = #SAI_SWITCH_ATTR_MY_MAC_TABLE_MINIMUM_PRIORITY)
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_MY_MAC_ATTR_PRIORITY = SAI_MY_MAC_ATTR_START,

    /**
     * @brief Associated Port, LAG object id,
     * if not specified any port will match
     *
     * @type sai_object_id_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_MY_MAC_ATTR_PORT_ID,

    /**
     * @brief Associated Vlan Id,
     * if not specified any vlan id will match
     *
     * @type sai_uint16_t
     * @flags CREATE_ONLY
     * @isvlan true
     * @default 0
     */
    SAI_MY_MAC_ATTR_VLAN_ID,

    /**
     * @brief MAC Address
     *
     * @type sai_mac_t
     * @flags CREATE_ONLY
     * @default vendor
     */
    SAI_MY_MAC_ATTR_MAC_ADDRESS,

    /**
     * @brief MAC Address Mask
     *
     * @type sai_mac_t
     * @flags CREATE_ONLY
     * @default vendor
     */
    SAI_MY_MAC_ATTR_MAC_ADDRESS_MASK,

    /**
     * @brief End of attributes
     */
    SAI_MY_MAC_ATTR_END,

    /** Custom range base value */
    SAI_MY_MAC_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_MY_MAC_ATTR_CUSTOM_RANGE_END

} sai_my_mac_attr_t;

/**
 * @brief Create My MAC entry.
 *
 * @param[out] my_mac_id My MAC id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_my_mac_fn)(
        _Out_ sai_object_id_t *my_mac_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove My MAC entry
 *
 * @param[in] my_mac_id My MAC Id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_my_mac_fn)(
        _In_ sai_object_id_t my_mac_id);

/**
 * @brief Set My MAC entry attribute
 *
 * @param[in] my_mac_id My MAC id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_my_mac_attribute_fn)(
        _In_ sai_object_id_t my_mac_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get My MAC entry attribute
 *
 * @param[in] my_mac_id My MAC id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_my_mac_attribute_fn)(
        _In_ sai_object_id_t my_mac_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief My MAC methods table retrieved with sai_api_query()
 */
typedef struct _sai_my_mac_api_t
{
    sai_create_my_mac_fn                create_my_mac;
    sai_remove_my_mac_fn                remove_my_mac;
    sai_set_my_mac_attribute_fn         set_my_mac_attribute;
    sai_get_my_mac_attribute_fn         get_my_mac_attribute;

} sai_my_mac_api_t;

#endif /** __SAIMYMAC_H_ */

````````````````

````````````````

    /**
     * @brief Minimum priority for My MAC
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MY_MAC_TABLE_MINIMUM_PRIORITY,

    /**
     * @brief Maximum priority for My MAC
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MY_MAC_TABLE_MAXIMUM_PRIORITY,

    /**
     * @brief My MAC entries installed on the switch
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_MY_MAC
     */
    SAI_SWITCH_ATTR_MY_MAC_LIST,

    /**
     * @brief Number of My MAC entries installed on the switch
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_INSTALLED_MY_MAC_ENTRIES,

    /**
     * @brief Number of available My MAC entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_MY_MAC_ENTRIES,


````````````````
The PR related to this feature is available at PR#[1243](https://github.com/opencomputeproject/SAI/pull/1243)


### Allow modification of nexthop member sequence id.

This implementation allows application to modify the sequence ID of the nexthop group member. This allows the application layer to use stateless incrementing indices to specify the ordering while allowing for group modification. The below modification is done in sainexthopgroup.h 


````````````````
sainexthopgroup.h 

SAI_NEXT_HOP_GROUP_TYPE_DYNAMIC_ORDERED_ECMP.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET     <<<<< changed from CREATE_ONLY
     * @default 0
     */

````````````````

The PR related to this feature is available at PR#[1247](https://github.com/opencomputeproject/SAI/pull/1247)


### Add a trap type for mpls lookup miss

This enhancement is for adding a new trap type for packets discarded due to mpls label lookup miss. One use case for this new trap is to mirror the trapped packets to a host for debug purpose. The below code has been added under saihostif.h

````````````````
saihostif.h

    /**
     * @brief MPLS packets discarded due to label lookup miss
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_MPLS_LABEL_LOOKUP_MISS = 0x00008002,
	
````````````````

The PR related to this feature is available at PR#[1252](https://github.com/opencomputeproject/SAI/pull/1252)


### Trap type changes

Few changes in trap has been in saihostif.h file. Added UDP port 6784 for BFD and BFDv6 trap types and added new LDP trap type

````````````````
saihostif.h 

    /**
     * @brief Micro BFD traffic (UDP dst port == 6784) to local
     * router IP address (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_BFD_MICRO = 0x00004007,

    /**
     * @brief Micro BFDV6 traffic (UDP dst port == 6784) to local
     * router IP address (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_BFDV6_MICRO = 0x00004008,

    /**
     * @brief LDP traffic (TCP src port == 646 or TCP dst port == 646) to local
     * router IP address or, (UDP dst port == 646) to the 'all routers on this
     * subnet' group multicast address (224.0.0.2) (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_LDP = 0x00004009,

````````````````

The PR related to this feature is available at PR#[1258](https://github.com/opencomputeproject/SAI/pull/1258)


### Assign values to enum fields

This implementation  ensures every enum field will have an explicit integer value assigned to it. Insertions can be "in between" (maintains readability) but the next available unused integer value would be used for the new insertions. Thus, once an integer is assigned to an enum field, it does not change with spec revision thereby not breaking warmboot. 

The PR related to this feature is available at PR#[1259](https://github.com/opencomputeproject/SAI/pull/1259)


### Add enable/disable option for forwarding pause frame to SAI port 

This enhancement is for the flow control APIs including enable/disable forwarding flow control frame. The below code has been added in saiport.h file.

````````````````

/**
     * @brief Forward or terminate the global flow control(802.3X) frame
     *
     * If true, flow control frames are switched between ports.
     * If false, flow control frames are terminated by the switch.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL_FORWARD,

    /**
     * @brief Forward or terminate the PFC(802.1Qbb) frame
     *
     * If true, flow control frames are switched between ports.
     * If false, flow control frames are terminated by the switch.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL_FORWARD,


````````````````

The PR related to this feature is available at PR#[1262](https://github.com/opencomputeproject/SAI/pull/1262)


### Fix counter SAI_SWITCH_STAT_ECC_DROP value

This implements the bundle of few fixes for SAI_SWITCH_STAT_ECC_DROP attribute in saiswitch.h. 

The PR related to this feature is available at PR#[1264](https://github.com/opencomputeproject/SAI/pull/1264)


### Update SAI_ACL_TABLE_ATTR_ENTRY_LIST value

This enhancement is for setting up of value to 0x2000 which will allow future expansion of acl table fields without changing attr values after SAI_ACL_TABLE_ATTR_FIELD_END. This is upadated in saiacl.h as below.

````````````````
saiacl.h

SAI_ACL_TABLE_ATTR_ENTRY_LIST = 0x00002000,  <<< value added

````````````````

### Add NVGRE tunnel

This implements the addition of NVGRE tunnel and its attributes in saitunnel.h file

The PR related to this feature is available at PR#[1269](https://github.com/opencomputeproject/SAI/pull/1269)


### Update bridge attributes @validonly tag using mixed condition

This is an enhancement on SAI_BRIDGE_ATTR_TYPE in saibridge.h file for @validonly tag using mixed condition to support SAI. The below changes are made as below.

````````````````
saibridge.h

@validonly SAI_BRIDGE_ATTR_TYPE == SAI_BRIDGE_TYPE_1D and (SAI_BRIDGE_ATTR_UNKNOWN_UNICAST_FLOOD_CONTROL_TYPE == SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP or SAI_BRIDGE_ATTR_UNKNOWN_UNICAST_FLOOD_CONTROL_TYPE == SAI_BRIDGE_FLOOD_CONTROL_TYPE_COMBINED)

@validonly SAI_BRIDGE_ATTR_TYPE == SAI_BRIDGE_TYPE_1D and (SAI_BRIDGE_ATTR_UNKNOWN_MULTICAST_FLOOD_CONTROL_TYPE == SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP or SAI_BRIDGE_ATTR_UNKNOWN_MULTICAST_FLOOD_CONTROL_TYPE == SAI_BRIDGE_FLOOD_CONTROL_TYPE_COMBINED)

@validonly SAI_BRIDGE_ATTR_TYPE == SAI_BRIDGE_TYPE_1D and (SAI_BRIDGE_ATTR_BROADCAST_FLOOD_CONTROL_TYPE == SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP or SAI_BRIDGE_ATTR_BROADCAST_FLOOD_CONTROL_TYPE == SAI_BRIDGE_FLOOD_CONTROL_TYPE_COMBINED)

````````````````

The PR related to this feature is available at PR#[1271](https://github.com/opencomputeproject/SAI/pull/1271)

### Fix description for ACL bind points

This is an update to enable/update ingress ACL table or ACL group filtering by assigning a valid object id. Disable ingress filtering by assigning egress ACL table or ACL group filtering by assigninga valid object id. 

The PR related to this feature is available at PR#[1296](https://github.com/opencomputeproject/SAI/pull/1296)


### SRv6 drop reason 

This change simply adds a new drop reason for drops related to IPv6 Segment Routing. It will be useful for debug counters implementation for such drops. The below update have been done in saidebugcounter.h file.


````````````````
saidebugcounter.h

    /**
     * @brief SRV6 local SID drop
     *
     * Packet is dropped due to local SID configuration or incorrect value in SRV6 packet header
     * e.g.: Local SID packet action is set to SAI_PACKET_ACTION_DROP
     * Next Header is SRH and Segments Left value is 0 for End, End.X, End.T, End.B* endpoint types
     * Next Header is not SRH while local SID is configured for packet DA
     * Segments Left value is not 0 when received packet is destined to S and S is a local SID of type End.D*
     */
    SAI_IN_DROP_REASON_SRV6_LOCAL_SID_DROP,


````````````````

The PR related to this feature is available at PR#[1306](https://github.com/opencomputeproject/SAI/pull/1306)


