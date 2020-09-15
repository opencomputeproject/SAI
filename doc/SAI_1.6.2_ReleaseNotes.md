
# SAI 1.6.2 Release Notes

The Switch Abstraction Interface defines the APIs to provide a vendor-independent way of controlling forwarding elements, such as a switching ASIC, an NPU or a software switch in a uniform manner.

This section describes the SAI changes done for new features, and various bug fixes mentioned below.

List of new features is as follows.
1) saimacsec API
2) saisystem port API

# 1. New Features  

### saimacsec.h

The switching hardware consists of network interfaces connected to a forwarding element, such as a switching ASIC.  Some switching hardware also include Phy ASIC(s) that interconnect network interfaces and forwarding element interfaces.  Each Phy ASIC supports one or more network interfaces.  SAI is used for each such Phy ASIC.  A sai_switch object is used to interface to either a forwarding element or a Phy. This SAI MACsec API provides a software interface for 802.1ae MACSec Entities (SecY) associated with some or all ports of a sai_switch object.

```
    sai_create_macsec_fn                create_macsec;
    sai_remove_macsec_fn                remove_macsec;
    sai_set_macsec_attribute_fn         set_macsec_attribute;
    sai_get_macsec_attribute_fn         get_macsec_attribute;
    sai_create_macsec_port_fn           create_macsec_port;
    sai_remove_macsec_port_fn           remove_macsec_port;
    sai_set_macsec_port_attribute_fn    set_macsec_port_attribute;
    sai_get_macsec_port_attribute_fn    get_macsec_port_attribute;
    sai_get_macsec_port_stats_fn        get_macsec_port_stats;
    sai_get_macsec_port_stats_ext_fn    get_macsec_port_stats_ext;
    sai_clear_macsec_port_stats_fn      clear_macsec_port_stats;
    sai_create_macsec_flow_fn           create_macsec_flow;
    sai_remove_macsec_flow_fn           remove_macsec_flow;
    sai_set_macsec_flow_attribute_fn    set_macsec_flow_attribute;
    sai_get_macsec_flow_attribute_fn    get_macsec_flow_attribute;
    sai_get_macsec_flow_stats_fn        get_macsec_flow_stats;
    sai_get_macsec_flow_stats_ext_fn    get_macsec_flow_stats_ext;
    sai_clear_macsec_flow_stats_fn      clear_macsec_flow_stats;
    sai_create_macsec_sc_fn             create_macsec_sc;
    sai_remove_macsec_sc_fn             remove_macsec_sc;
    sai_set_macsec_sc_attribute_fn      set_macsec_sc_attribute;
    sai_get_macsec_sc_attribute_fn      get_macsec_sc_attribute;
    sai_get_macsec_sc_stats_fn          get_macsec_sc_stats;
    sai_get_macsec_sc_stats_ext_fn      get_macsec_sc_stats_ext;
    sai_clear_macsec_sc_stats_fn        clear_macsec_sc_stats;
    sai_create_macsec_sa_fn             create_macsec_sa;
    sai_remove_macsec_sa_fn             remove_macsec_sa;
    sai_set_macsec_sa_attribute_fn      set_macsec_sa_attribute;
    sai_get_macsec_sa_attribute_fn      get_macsec_sa_attribute;
    sai_get_macsec_sa_stats_fn          get_macsec_sa_stats;
    sai_get_macsec_sa_stats_ext_fn      get_macsec_sa_stats_ext;
    sai_clear_macsec_sa_stats_fn        clear_macsec_sa_stats;

```

### saisystemport.h

System port creation can be accomplished by calling switch_create() with the list of system ports to be created using SAI_SWITCH_ATTR_SYSTEM_PORT_CONFIG_LIST or creating them individually using the create_system_port() function in the system port APIs. Although it is expected that all local ports be created in switch_create().

System port configuration can be retrieved by using the SAI_SYSTEM_PORT_ATTR_CONFIG_INFO attribute. There are also attributes to retrieve the mappings between system port objects and local port objects and retrieval of VoQs type object list associated with the system port. 

```
Create system port

typedef sai_status_t (*sai_create_system_port_fn)(
        _Out_ sai_object_id_t *system_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

Remove system port

typedef sai_status_t (*sai_remove_system_port_fn)(
        _In_ sai_object_id_t system_port_id);

Set system port attribute value.

typedef sai_status_t (*sai_set_system_port_attribute_fn)(
        _In_ sai_object_id_t system_port_id,
        _In_ const sai_attribute_t *attr);

Get system port attribute value.

typedef sai_status_t (*sai_get_system_port_attribute_fn)(
        _In_ sai_object_id_t system_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

Port methods table retrieved with sai_api_query()

typedef struct _sai_system_port_api_t
{
    sai_create_system_port_fn                create_system_port;
    sai_remove_system_port_fn                remove_system_port;
    sai_set_system_port_attribute_fn         set_system_port_attribute;
    sai_get_system_port_attribute_fn         get_system_port_attribute;

} sai_system_port_api_t;

```
# 2. Feature Enhancements

This section describes the SAI changes done for new features, enhancements on existing features/sub-features and various bug fixes mentioned below.

1.  saiacl
2.  saifdb
3.  saihash
4.  sailag
5.  saimpls
6.  saineighbor
7.  sainexthop
8.  sainexthopgroup
9.  saiport
10. saiqosmap
11. saiqueue
12. saiswitch
13. saitunnel
14. saitypes


### sah.h		

saimacsec and saisystem port API has bee added

### saiacl.h	

Provides generalized Access Control Lists management functions. The ACL contains three types of objects, ACL table, ACL entry and ACL counter. The ACL table contains a number of ACL entries. Each ACL table defines a set of unique matching fields for all its ACL entries. A packet can match rules in different ACL tables and take non-conflicting actions from all the matched rules. However, within an ACL table, if a packet matches multiple rules, only the actions from the rule of highest priority are executed. ACL counters can also be created and attached to an ACL entry in order to counter the number of packets or bytes that match the ACL entry.
In this release, the sai ACL Table attribute field(SAI_ACL_TABLE_ATTR_FIELD) has been added for vlan tags, macsec_sci,mpls label/Exp/TTL/BOS, and GRE key. sai acl entry action for macsec_flow, ecmp hash id.

```
SAI_ACL_TABLE_ATTR_FIELD_HAS_VLAN_TAG
SAI_ACL_TABLE_ATTR_FIELD_MACSEC_SCI
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL0_LABEL 
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL0_TTL
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL0_EXP 
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL0_BOS
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL1_LABEL
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL1_TTL
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL1_EXP
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL1_BOS
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL2_LABEL
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL2_TTL
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL2_EXP
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL2_BOS
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL3_LABEL
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL3_TTL
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL3_EXP
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL3_BOS
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL4_LABEL
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL4_TTL
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL4_EXP
SAI_ACL_TABLE_ATTR_FIELD_MPLS_LABEL4_BOS
SAI_ACL_TABLE_ATTR_FIELD_GRE_KEY
SAI_ACL_TABLE_ATTR_FIELD_TAM_INT_TYPE
	
```
			
### saifdb.h	
This API provides FDB entries manipulation as well as aging/learning notifications. Support for static FDB Entries to allow MAC Move -  When MAC_MOVE is explicitly disabled for a static MAC entry via this attribute, the trap introduced earlier would also not be generated.
FDB entries modifications for<br>
		1.)Bulk create FDB entry<br>
		2.)Bulk remove FDB entry<br>
		3.)Bulk set attribute on FDB entry<br>
		4.)Bulk get attribute on FDB entry

```

Bulk create FDB entry
 
typedef sai_status_t (*sai_bulk_create_fdb_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);


Bulk remove FDB entry
 
typedef sai_status_t (*sai_bulk_remove_fdb_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);


Bulk set attribute on FDB entry
 
typedef sai_status_t (*sai_bulk_set_fdb_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);


Bulk get attribute on FDB entry

typedef sai_status_t (*sai_bulk_get_fdb_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

```

### saihash.h	
Defintion type for sai_hash_api has been modified for create/remove/set/get for fine_grained_hash_field. Support for static FDB Entries to allow MAC Move such as Source/Destination IPv4 and IPv6 has been added

```

    sai_create_fine_grained_hash_field_fn          create_fine_grained_hash_field;
    sai_remove_fine_grained_hash_field_fn          remove_fine_grained_hash_field;
    sai_set_fine_grained_hash_field_attribute_fn   set_fine_grained_hash_field_attribute;
    sai_get_fine_grained_hash_field_attribute_fn   get_fine_grained_hash_field_attribute;
	
```	

### sailag.h	
SAI LAG provides APIs to create, remove and update Link Aggregation Group (LAG) object. The create call return a LAG id which can be later used to manage the port membership of the LAG. The remove call destroy the LAG object. The add_ports_to_lag() and remove_ports_from_lag() adds and removes SAI port to and from the LAG.
LAG is defined as a simple container for a group of the SAI ports. The LAG object only manages its port membership. All its member port attributes such as MTU, default VLAN are managed individually. The SAI applications need to be aware of the LAG membership and is responsible to keep a consistent view across all its member port attributes, for example all member ports should have same MTU and same default VLAN.
SAI, for a given set of member ports, should maintain the same flow-to-port mapping to the extent possible when the member ports gets deleted and added back given that the hash function for this LAG port is not changed.
In this release for API, LAG system port ID has been added. The application must manage the allocation of the system port aggregate IDs. associated with the LAG for consistency across all switches in a VOQ based system. The system port aggregate ID range is from 1 to SAI_SWITCH_ATTR_NUMBER_OF_LAGS. The default value of 0 means this field is not used and SAI will allocate the system port aggregate ID internally. Valid only when SAI_SWITCH_ATTR_TYPE	== SAI_SWITCH_TYPE_VOQ

```
    * @brief TPID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0x8100
     */
    SAI_LAG_ATTR_TPID,

    /**
     * @brief LAG system port ID
     *
     * The application must manage the allocation of the system port aggregate IDs
     * associated with the LAG for consistency across all switches in a VOQ based
     * system. The system port aggregate ID range is from 1 to SAI_SWITCH_ATTR_NUMBER_OF_LAGS.
     * The default value of 0 means this field is not used and SAI will allocate the system
     * port aggregate ID internally.
     * Valid only when SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_VOQ
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_LAG_ATTR_SYSTEM_PORT_AGGREGATE_ID,

    /**

```

### saimpls.h	
Provide TTL and QoS treatment during MPLS encap and decap. Associate TC by a label, QOS MAP and associate COLOR by a QOS MAP. Following attributes were modified part of this release.

```
SAI_INSEG_ENTRY_PSC_TYPE_ELSP
SAI_INSEG_ENTRY_PSC_TYPE_LLSP
SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM
SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE
SAI_INSEG_ENTRY_POP_QOS_MODE_UNIFORM
SAI_INSEG_ENTRY_POP_QOS_MODE_PIPE

```

### saineighbor.h  
Provides the IP neighbor table management functions. IP neighbor defines a L3 neighbor that is within the same IP subnet as one of the router interface on the router. The IP neighbor is associated with a specific router interface, is assigned to a specific IP address, and has a mac address. IP neighbor also has a packet action attribute to decide whether to forward, drop or trap the IP packet which comes from a different router interface with destination IP matching the neighbor IP. The IP neighbor does not define the outgoing port as this can be resolved using the FDB table.
In this release for API, Encapsulation index flag is added. This is a flag which states that the encap index was imposed. On create and set the SAI_NEIGHBOR_ENTRY_ATTR_ENCAP_INDEX must be present.

```
SAI_NEIGHBOR_ENTRY_ATTR_ENCAP_INDEX
SAI_NEIGHBOR_ENTRY_ATTR_ENCAP_IMPOSE_INDEX
SAI_NEIGHBOR_ENTRY_ATTR_IS_LOCAL

```

### sainexthop.h	
Provides next hop objects such as IP next hop and IP tunnel next hop. Current version only has IP next hop defined. IP next hop must also be an IP neighbor. It defines an IP neighbor that a packet can be routed to. The user needs to create an IP neighbor before it creates an IP next hop. The sai associates them using the router interface ID and the IP address. The IP next hop is identified by a next hop ID which can be used in sai_route as well as other matching rules to forward traffic to an IP prefix to it. Intuitively, IP neighbor represents a device without any packet forwarding capability so that only traffic destined to its IP are forwarded to it, whereas IP next hop represents a device that can also forward other traffic. IP next hop is associated with a specific router interface, and is identified with an IP address.
In this release, API for TTL and QoS treatment during MPLS encap and decap has been added.

```
SAI_NEXT_HOP_ATTR_DECREMENT_TTL
SAI_NEXT_HOP_ATTR_OUTSEG_TYPE
SAI_NEXT_HOP_ATTR_OUTSEG_TTL_MODE
SAI_NEXT_HOP_ATTR_OUTSEG_TTL_VALUE
SAI_NEXT_HOP_ATTR_OUTSEG_EXP_MODE
SAI_NEXT_HOP_ATTR_OUTSEG_EXP_VALUE
SAI_NEXT_HOP_ATTR_QOS_TC_AND_COLOR_TO_MPLS_EXP_MAP

```

### sainexthopgroup.h	
Provides next hop group functions. A router can have many next hops. In case of unicast, a packet can be spread among a group of next hops. Next hop group contains a group of next hops.
In this release, the below attributes are added in API along with enhancements. 

1)Object index in the fine grain ECMP table. Index specifying the strict member's order.<br>
2)Allowed value range for is from 0 to SAI_NEXT_HOP_GROUP_ATTR_REAL_SIZE - 1. <br>
3)Should only be used if the type of owning group is SAI_NEXT_HOP_GROUP_TYPE_FINE_GRAIN_ECMP. <br>
4)Object's sequence ID for enforcing the members' order. Loose index specifying the member's order. <br>
5)The index is not strict allowing for the missing IDs in a sequence. <br>
6)It's driver's job to translate the sequence IDs to the real indexes in the group. Should only be used if the type of owning group is SAI_NEXT_HOP_GROUP_TYPE_DYNAMIC_ORDERED_ECMP. <br>

```
SAI_NEXT_HOP_GROUP_ATTR_CONFIGURED_SIZE
SAI_NEXT_HOP_GROUP_ATTR_REAL_SIZE
   
```

### saiport.h	
SAI port provides following functions:

Provides physical port state manipulation as well as state (UP/DOWN) notifications.
Obtain physical lane information for a SAI port
Setup the lane mode for a SAI port

In this release, the below attributes are added in API along with enhancements. 

1)Attribute data for #SAI_PORT_ATTR_INTERFACE_TYPE. Used for selecting electrical interface with specific electrical pin and signal quality. <br>
2)Port bind point for ingress/egress ACL object. Bind (or unbind) an ingress/egress ACL table or ACL group on a port. Enable/Update ingress/egress ACL table or ACL group filtering by assigning the list of valid object id. Disable ingress/egress filtering by assigning SAI_NULL_OBJECT_ID in the attribute value. <br>
3)Port bind point for ingress MACsec ACL object. Bind (or unbind) an ingress MACsec ACL table on a port. Enable/Update ingress MACsec ACL table filtering by assigning the list of valid object id. Disable ingress filtering by assigning SAI_NULL_OBJECT_ID in the attribute value. <br>
4)Link training failure status and error codes port stat PRBS error counts and list of port connector attributes are added. <br>

```
SAI_PORT_ATTR_INGRESS_MACSEC_ACL
SAI_PORT_ATTR_EGRESS_MACSEC_ACL
SAI_PORT_ATTR_MACSEC_PORT_LIST

```
### saiqosmap.h	
QOS Map to set traffic class and color to EXP.

```
 /** QOS Map to set EXP to Traffic class */
    SAI_QOS_MAP_TYPE_MPLS_EXP_TO_TC = 0x0000000a,

    /** QOS Map to set EXP to color */
    SAI_QOS_MAP_TYPE_MPLS_EXP_TO_COLOR = 0x0000000b,

    /** QOS Map to set traffic class and color to EXP */
    SAI_QOS_MAP_TYPE_TC_AND_COLOR_TO_MPLS_EXP = 0x0000000c,
```

### saiqueue.h	
H/w Egress Unicast Queue and H/w Multicast Egress (Broadcast, Unknown unicast, Multicast) Queue has been added.

```
/** H/w Virtual Output Queue (VOQ). This queue is ingress unicast queue */
    SAI_QUEUE_TYPE_UNICAST_VOQ = 0x00000003,

    /** H/w Virtual Output Queue (VOQ). This queue is fabric multicast queue */
    SAI_QUEUE_TYPE_MULTICAST_VOQ = 0x00000004,

    /** H/w Fabric Queue. */
    SAI_QUEUE_TYPE_FABRIC_TX = 0x00000005,

```

### saiswitch.h	
This is a top-level object exposed from the Adapter. This switch object controls the switch level behavior such as cut-through or store-forward, ecmp hash configuration.
In this release, the below attributes are added in API along with enhancements. 

1)Attribute data for #SAI_SWITCH_ATTR_HARDWARE_ACCESS_BUS, SAI_SWITCH_ATTR_FIRMWARE_LOAD_METHOD, SAI_SWITCH_ATTR_FIRMWARE_LOAD_TYPE, SAI_SWITCH_ATTR_TYPE has been added. <br>
2)Switch hardware access bus MDIO/I2C/CPLD has been added. <br>
3)Platform context information provided by the host adapter to driver. This information is Host adapter specific, typically used for maintain synchronization and device information. <br>
4)Driver will give this context back to adapter as part of call back sai_switch_register_read/write_fn API. <br>
5)Platform adaption device write callback function passed to the adapter. This is mandatory function for driver when device access not supported by file system. <br>
6)Platform specific device register read access. This API provides platform adaption functionality to access device registers from driver. This is mandatory to pass as attribute to sai_create_switch when driver implementation does not support register access by device file system directly.<br>
7)Platform specific device register write access. This API provides platform adaption functionality to access device registers from driver. This is mandatory to pass as attribute to sai_create_switch when driver implementation does not support register access by device file system directly.<br>
8)Switch MDIO read API - Provides read access API for devices connected to MDIO from NPU SAI.<br>
9)Switch MDIO write API - Provides write access API for devices connected to MDIO from NPU SAI.<br>

```
SAI_SWITCH_HARDWARE_ACCESS_BUS_MDIO
SAI_SWITCH_HARDWARE_ACCESS_BUS_I2C
SAI_SWITCH_HARDWARE_ACCESS_BUS_CPLD
SAI_SWITCH_ATTR_FIRMWARE_LOAD_METHOD
SAI_SWITCH_ATTR_FIRMWARE_LOAD_TYPE
SAI_SWITCH_ATTR_TYPE
SAI_SWITCH_ATTR_HARDWARE_ACCESS_BUS
SAI_SWITCH_ATTR_PLATFROM_CONTEXT
SAI_SWITCH_ATTR_REGISTER_READ
SAI_SWITCH_ATTR_REGISTER_WRITE
SAI_SWITCH_ATTR_FIRMWARE_DOWNLOAD_BROADCAST
SAI_SWITCH_ATTR_FIRMWARE_LOAD_METHOD
SAI_SWITCH_ATTR_FIRMWARE_LOAD_TYPE
SAI_SWITCH_ATTR_FIRMWARE_DOWNLOAD_EXECUTE
SAI_SWITCH_ATTR_FIRMWARE_BROADCAST_STOP
SAI_SWITCH_ATTR_FIRMWARE_VERIFY_AND_INIT_SWITCH
SAI_SWITCH_ATTR_FIRMWARE_STATUS
SAI_SWITCH_ATTR_FIRMWARE_MAJOR_VERSION
SAI_SWITCH_ATTR_FIRMWARE_MINOR_VERSION
SAI_SWITCH_ATTR_PORT_CONNECTOR_LIST
SAI_SWITCH_ATTR_PROPOGATE_PORT_STATE_FROM_LINE_TO_SYSTEM_PORT_SUPPORT
SAI_SWITCH_ATTR_TYPE
SAI_SWITCH_ATTR_MACSEC_OBJECT_ID
SAI_SWITCH_ATTR_QOS_MPLS_EXP_TO_TC_MAP
SAI_SWITCH_ATTR_QOS_MPLS_EXP_TO_COLOR_MAP
SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_MPLS_EXP_MAP
SAI_SWITCH_ATTR_SWITCH_ID
SAI_SWITCH_ATTR_MAX_SYSTEM_CORES
SAI_SWITCH_ATTR_SYSTEM_PORT_CONFIG_LIST
SAI_SWITCH_ATTR_NUMBER_OF_SYSTEM_PORTS
SAI_SWITCH_ATTR_SYSTEM_PORT_LIST
SAI_SWITCH_ATTR_NUMBER_OF_FABRIC_PORTS
SAI_SWITCH_ATTR_FABRIC_PORT_LIST

```

### saitunnel.h	
Create and Set for Tunnel Attributes are added.

```
/**
 * @brief Defines tunnel peer mode
 */
typedef enum _sai_tunnel_peer_mode_t
{
    /**
     * @brief P2P Tunnel
     */
    SAI_TUNNEL_PEER_MODE_P2P,

    /**
     * @brief P2MP Tunnel
     */
    SAI_TUNNEL_PEER_MODE_P2MP,

} sai_tunnel_peer_mode_t;

/**
```

### saitypes.h	
				typedef UINT8   sai_macsec_sak_t[32];
				typedef UINT8   sai_macsec_auth_key_t[16];
				typedef UINT8   sai_macsec_salt_t[12];

				SAI_OBJECT_TYPE_PORT_CONNECTOR           = 86,
				SAI_OBJECT_TYPE_PORT_SERDES              = 87,
				SAI_OBJECT_TYPE_MACSEC                   = 88,
				SAI_OBJECT_TYPE_MACSEC_PORT              = 89,
				SAI_OBJECT_TYPE_MACSEC_FLOW              = 90,
				SAI_OBJECT_TYPE_MACSEC_SC                = 91,
				SAI_OBJECT_TYPE_MACSEC_SA                = 92,
				SAI_OBJECT_TYPE_SYSTEM_PORT              = 93,
				SAI_OBJECT_TYPE_MAX,  /* Must remain in last position */

The above type definetions and object types were added