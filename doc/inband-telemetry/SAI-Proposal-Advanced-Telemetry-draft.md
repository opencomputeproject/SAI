SAI Advanced Network Telemetry API Proposal
-------------------------------------------------------------------------------
 Title       | Advanced Network Telemetry
-------------|-----------------------------------------------------------------
 Authors     | Barefoot Networks
 Status      | In review
 Type        | Standards track
 Created     | 06/18/2017
 Updated     | 09/20/2017
 SAI-Version | 1.x

-------------------------------------------------------------------------------

# Overview

This draft describes the API proposal to enable advanced telemetry capabilities in a network device. The main goal of this advanced telemetry proposal is to achieve per-packet network visibility with low overhead. The network device should be able to inspect and take telemetry actions on each individual data packet.


## Telemetry data plane workflow

![workflow](figures/workflow.png "Figure 1: Telemetry data plane workflow")

__Figure 1: Telemetry data plane workflow__

Figure 1 describes the telemetry data plane functionality at a high level. The telemetry module inspects every data packet without interfering with normal data forwarding pipeline. Different components may be located at different switches in the network. A telemetry watchlist table specifies the flows to monitor. It performs ternary match on the packet headers and switch ports, and provides telemetry action parameters. Watchlist is realized through SAI ACL, with a few new field and action attributes added to ACL table and entry objects. Packets belonging to the specified flow spaces will be processed by the event detection logic. If a triggering event is detected, the switch will generate a report message to the monitor. The report message includes packet header and switch metadata associated with the packet (e.g., timestamp, ingress/egress ports, queue depth/latency).

Sections below introduce four different data plane telemetry capabilities: In-band Network Telemetry (INT), Packet Postcards, and Mirror on Drop, and Queue Congestion Report.

## In-band Network Telemetry

![INT](figures/INT.png "Figure 2: In-band Network Telemetry")

__Figure 2: In-band Network Telemetry__

Figure 2 shows an example workflow of In-band Network Telemetry (INT). Switches along the route path add switch metadata into the packet header based on the *telemetry instructions* carried in the INT header.

Each switch may play the role of __endpoint__ or __transit__ for INT packets. INT endpoint acts both as __INT source__ and __INT sink__. As INT source it initiates INT operation by inserting INT header into a packet and thereby instructing other network devices along the routing path to add desired telemetry information into the packet. As INT sink it extracts the INT information from the packets and send telemetry reports to the monitor.

An __INT transit__ is a device along the path of a packet from source to sink. INT transit device adds its own INT information to the packet as requested by an INT source.


## Packet Postcards

![Postcard](figures/postcard.png "Figure 3: Postcard")

__Figure 3: Packet Postcards__

Figure 3 shows an example workflow of packet Postcards. Each switch makes its own decision and reports packets info (aka “postcards”) to the monitor individually. Unlike INT, a Postcard switch never modifies the original data packets. INT and Postcard achieve the same flow monitoring capabilities with different data plane approaches. 

## Mirror on Drop

![MoD](figures/mod.png "Figure 4: Mirror on Drop")

__Figure 4: Mirror on Drop__

Figure 4 depicts the mirror on drop capability. Switches mirror packets that are dropped by the ingress pipe, egress pipe or queueing buffer to the monitor for network diagnosis. The report messages include original packet header, switch metadata, and drop reason.

## Queue reports
![Queue](figures/queue.png "Figure 5: Queue report")

__Figure 5: Queue Congestion Reports__

Figure 5 shows the queue congestion reports functionality. Switches can be configured to report all packets experience congestion at specified queues. Congestion is detected by queue depth and latency thresholds, which can be configured separately for each individual queue. Queue congestion report is turned off for each queue by default, and is only enabled when depth and latency thresholds are configured.


# Specification

This section describes the advanced telemetry API proposal.

## Changes to `saiswitch.h` for switch telemetry attributes
~~~cpp
typedef enum _sai_switch_attr_t
{
    ......
    /**
     * @brief INT endpoint
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_SWITCH_ATTR_TELEMETRY_INT_ENDPOINT_ENABLE,

    /**
     * @brief INT transit
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_SWITCH_ATTR_TELEMETRY_INT_TRANSIT_ENABLE,

    /**
     * @brief Packet postcard
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_SWITCH_ATTR_TELEMETRY_POSTCARD_ENABLE,

    /**
     * @brief Mirror on Drop
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_SWITCH_ATTR_TELEMETRY_MIRROR_ON_DROP_ENABLE,

    /**
     * @brief Globally unique switch ID
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_SWITCH_ATTR_TELEMETRY_SWITCH_ID,

    /**
     * @brief Telemetry report source IP address
     *
     * @type sai_ip4_t
     * @flags CREATE_AND_SET
     */
    SAI_SWITCH_ATTR_TELEMETRY_REPORT_SRC_IP,

    /**
     * @brief Telemetry report destination IP addresses
     *
     * @type sai_ip4_list_t
     * @flags CREATE_AND_SET
     */
    SAI_SWITCH_ATTR_TELEMETRY_REPORT_DST_IP_LIST,

    /**
     * @brief Telemetry report virtual router ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     */
    SAI_SWITCH_ATTR_TELEMETRY_REPORT_VIRTUAL_ROUTER_ID,

    /**
     * @brief Telemetry report truncate size
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     */
    SAI_SWITCH_ATTR_TELEMETRY_REPORT_TRUNCATE_SIZE,

    /**
     * @brief Telemetry report UDP destination port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     */
    SAI_SWITCH_ATTR_TELEMETRY_REPORT_UDP_DST_PORT,

    /**
     * @brief Telemetry flow state clear cycle
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_TELEMETRY_FLOW_STATE_CLEAR_CYCLE,

    /**
     * @brief Latency sensitivity for flow state change detection
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_SWITCH_ATTR_TELEMETRY_LATENCY_SENSITIVITY,

    /**
     * @brief INT sink downstream ports
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     */
    SAI_SWITCH_ATTR_TELEMETRY_INT_SINK_PORT_LIST,

    /**
     * @brief Reserved DSCP value for INT over L4
     *
     * @type sai_ternary_field_t
     * @flags CREATE_AND_SET
     */
    SAI_SWITCH_ATTR_TELEMETRY_INT_L4_DSCP,

} sai_switch_attr_t;
~~~

## Changes to `saiacl.h` for INT, Postcard and MoD watchlist
~~~cpp
typedef enum _sai_acl_action_type_t
{
    ......
    /** Enable mirror on drop */
    SAI_ACL_ACTION_TYPE_TELEMETRY_MOD_ENABLE,

    /** Enable in-band network telemetry */
    SAI_ACL_ACTION_TYPE_TELEMETRY_INT_ENABLE,

    /** INT session ID */
    SAI_ACL_ACTION_TYPE_TELEMETRY_INT_SESSION,

    /** Enable packet postcard */
    SAI_ACL_ACTION_TYPE_TELEMETRY_POSTCARD_ENABLE,

    /** Telemetry flow sample percent within matched flow space */
    SAI_ACL_ACTION_TYPE_TELEMETRY_FLOW_SAMPLE_PERCENT,

    /** Report every packet for the matched flow */
    SAI_ACL_ACTION_TYPE_TELEMETRY_REPORT_ALL,

} sai_acl_action_type_t;

typedef enum _sai_acl_table_attr_t
{
    ......
    /**
     * @brief Tunnel VNI
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_TUNNEL_VNI,

    /**
     * @brief Inner EtherType
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_INNER_ETHER_TYPE,

    /**
     * @brief Inner IP Protocol
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_INNER_IP_PROTOCOL,

    /**
     * @brief Inner L4 Src Port
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_INNER_L4_SRC_PORT,

    /**
     * @brief Inner L4 Dst Port
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_FIELD_INNER_L4_DST_PORT,

} sai_acl_table_attr_t;

typedef enum _sai_acl_entry_attr_t
{
    ......
    /**
     * @brief Tunnel VNI
     *
     * @type sai_acl_field_data_t sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_ACL_ENTRY_ATTR_FIELD_TUNNEL_VNI,

    /**
     * @brief Inner EtherType
     *
     * @type sai_acl_field_data_t sai_uint16_t
     * @flags CREATE_AND_SET
     */
    SAI_ACL_ENTRY_ATTR_FIELD_INNER_ETHER_TYPE,

    /**
     * @brief Inner IP Protocol
     *
     * @type sai_acl_field_data_t sai_uint8_t
     * @flags CREATE_AND_SET
     */
    SAI_ACL_ENTRY_ATTR_FIELD_INNER_IP_PROTOCOL,

    /**
     * @brief Inner L4 Src Port
     *
     * @type sai_acl_field_data_t sai_uint16_t
     * @flags CREATE_AND_SET
     */
    SAI_ACL_ENTRY_ATTR_FIELD_INNER_L4_SRC_PORT,

    /**
     * @brief Inner L4 Dst Port
     *
     * @type sai_acl_field_data_t sai_uint16_t
     * @flags CREATE_AND_SET
     */
    SAI_ACL_ENTRY_ATTR_FIELD_INNER_L4_DST_PORT,

    /**
     * @brief Enable mirror on drop
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_ACL_ENTRY_ATTR_ACTION_TELEMETRY_MOD_ENABLE,

    /**
     * @brief Enable in-band network telemetry
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_ACL_ENTRY_ATTR_ACTION_TELEMETRY_INT_ENABLE,

    /**
     * @brief INT session ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     */
    SAI_ACL_ENTRY_ATTR_ACTION_TELEMETRY_INT_SESSION,

    /**
     * @brief Enable packet postcard
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_ACL_ENTRY_ATTR_ACTION_TELEMETRY_POSTCARD_ENABLE,

    /**
     * @brief Telemetry flow sample percent within matched flow space
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 100
     */
    SAI_ACL_ENTRY_ATTR_ACTION_TELEMETRY_FLOW_SAMPLE_PERCENT,

    /**
     * @brief Report every packet for the matched flow
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_ACL_ENTRY_ATTR_ACTION_TELEMETRY_REPORT_ALL,

} sai_acl_entry_attr_t;
~~~

## New Header `saitelemetry.h`

### Data Structures and Enumerations

~~~cpp
/**
 * @brief Queue alert report trigger attributes
 */
typedef enum _sai_telemetry_queue_alert_attr_t 
{
    /**
     * @brief queue object ID
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TELEMETRY_QUEUE_ALERT_ATTR_QUEUE_ID,

    /**
     * @brief queue depth threshold
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default UINT32_MAX
     */
    SAI_TELEMETRY_QUEUE_ALERT_ATTR_QUEUE_DEPTH_THRESHOLD,

    /**
     * @brief queue latency threshold
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default UINT32_MAX
     */
    SAI_TELEMETRY_QUEUE_ALERT_ATTR_QUEUE_LATENCY_THRESHOLD

} sai_telemetry_queue_alert_attr_t;

/**
 * @brief INT config session attributes
 */
typedef enum _sai_telemetry_int_session_attr_t 
{
    /**
     * @brief INT max hop count
     *
     * The maximum number of hops that are allowed to
     * add their metadata to the packet
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 8
     */
    SAI_TELEMETRY_INT_SESSION_ATTR_MAX_HOP_COUNT,

    /**
     * @brief add switch ID in INT instruction
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_TELEMETRY_INT_SESSION_ATTR_INST_SWITCH_ID,

    /**
     * @brief add ingress and egress ports in INT instruction
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_TELEMETRY_INT_SESSION_ATTR_INST_SWITCH_PORTS,

    /**
     * @brief add ingress Timestamp in INT instruction
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_TELEMETRY_INT_SESSION_ATTR_INST_INGRESS_TIMESTAMP,

    /**
     * @brief add egress Timestamp in INT instruction
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_TELEMETRY_INT_SESSION_ATTR_INST_EGRESS_TIMESTAMP,

    /**
     * @brief add queue information in INT instruction
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default False
     */
    SAI_TELEMETRY_INT_SESSION_ATTR_INST_QUEUE_INFO

} sai_telemetry_int_session_attr_t;

/**
 * @brief Enum defining telemetry event types.
 */
typedef enum _sai_telemetry_event_type_t 
{
    /** Report triggered by flow state (e.g., path, latency) change */
    SAI_TELEMETRY_EVENT_TYPE_FLOW_STATE_CHANGE,
    
    /** Report triggered by REPORT_ALL in watchlist entry action */
    SAI_TELEMETRY_EVENT_TYPE_FLOW_REPORT_ALL,
    
    /** Report triggered by TCP FLAGS */
    SAI_TELEMETRY_EVENT_TYPE_FLOW_TCPFLAG,    
    
    /** Report triggered by queue alert, with queue status change */
    SAI_TELEMETRY_EVENT_TYPE_QUEUE_ALERT_CHANGE,   
      
    /** Report triggered by queue alert, without queue status change */
    SAI_TELEMETRY_EVENT_TYPE_QUEUE_ALERT_STABLE,         
    
    /** Report triggered by packet drop */
    SAI_TELEMETRY_EVENT_TYPE_PACKET_DROP, 

} sai_telemetry_event_type_t;

/**
 * @brief Telemetry event DSCP values
 */
typedef enum _sai_telemetry_event_dscp_attr_t 
{
    /**
     * @brief Telemetry event type
     *
     * @type sai_telemetry_event_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_TELEMETRY_EVENT_DSCP_ATTR_TYPE,
	
    /**
     * @brief Telemetry event dscp value
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE
     */	
    SAI_TELEMETRY_EVENT_DSCP_ATTR_VALUE,

} sai_telemetry_event_dscp_attr_t;
~~~

### SAI API
~~~cpp
typedef sai_status_t (*sai_create_telemetry_queue_alert_fn)(
        _Out_ sai_object_id_t *telemetry_queue_alert_id,
        _In_  uint32_t attr_count,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_telemetry_queue_alert_fn)(
        _In_ sai_object_id_t telemetry_queue_alert_id);

typedef sai_status_t (*sai_get_telemetry_queue_alert_attribute_fn)(
        _In_    sai_object_id_t telemetry_queue_alert_id,
        _In_    uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_telemetry_queue_alert_attribute_fn)(
        _In_  sai_object_id_t telemetry_queue_alert_id,
        _In_  const sai_attribute_t *attr);

typedef sai_status_t (*sai_create_telemetry_int_session_fn)(
        _Out_ sai_object_id_t *telemetry_int_session_id,
        _In_  uint32_t attr_count,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_telemetry_int_session_fn)(
        _In_ sai_object_id_t telemetry_int_session_id);

typedef sai_status_t (*sai_get_telemetry_int_session_attribute_fn)(
        _In_    sai_object_id_t telemetry_int_session_id,
        _In_    uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_telemetry_int_session_attribute_fn)(
        _In_  sai_object_id_t telemetry_int_session_id,
        _In_  const sai_attribute_t *attr);

typedef sai_status_t (*sai_create_telemetry_event_dscp_fn)(
        _Out_ sai_object_id_t *telemetry_event_dscp_id,
        _In_  uint32_t attr_count,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_telemetry_event_dscp_fn)(
        _In_ sai_object_id_t telemetry_queue_alert_id);

typedef sai_status_t (*sai_get_telemetry_event_dscp_attribute_fn)(
        _In_    sai_object_id_t telemetry_event_dscp_id,
        _In_    uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_telemetry_event_dscp_attribute_fn)(
        _In_  sai_object_id_t telemetry_event_dscp_id,
        _In_  const sai_attribute_t *attr);

typedef struct _sai_telemetry_api_t 
{
    sai_create_telemetry_queue_alert_fn          create_telemetry_queue_alert;
    sai_remove_telemetry_queue_alert_fn          remove_telemetry_queue_alert;
    sai_get_telemetry_queue_alert_attribute_fn   get_telemetry_queue_alert_attribute;
    sai_set_telemetry_queue_alert_attribute_fn   set_telemetry_queue_alert_attribute;

    sai_create_telemetry_int_session_fn          create_telemetry_int_session;
    sai_remove_telemetry_int_session_fn          remove_telemetry_int_session;
    sai_get_telemetry_int_session_attribute_fn   get_telemetry_int_session_attribute;
    sai_set_telemetry_int_session_attribute_fn   set_telemetry_int_session_attribute;
    
    sai_create_telemetry_event_dscp_fn           create_telemetry_event_dscp;
    sai_remove_telemetry_event_dscp_fn           remove_telemetry_event_dscp;
    sai_get_telemetry_event_dscp_attribute_fn    get_telemetry_event_dscp_attribute;
    sai_set_telemetry_event_dscp_attribute_fn    set_telemetry_event_dscp_attribute;

} sai_telemetry_api_t;
~~~

## Example
Example of configuring __INT Endpoint__ on a switch

~~~cpp
sai_attribute_t attr
// Set globally unique switch ID
attr.id = SAI_SWITCH_ATTR_TELEMETRY_SWITCH_ID;
attr.value.u32 = 0xfff222aa;
sai_switch_api-> set_switch_attribute(0, &attr);

// Set telemetry report source IPv4 address
attr.id = SAI_SWITCH_ATTR_TELEMETRY_REPORT_SRC_IP;
attr.value.ip4 = 0xc0a8640b; // 192.168.100.11
sai_switch_api-> set_switch_attribute(0, &attr);

// Set telemetry report destination IPv4 addresses
attr.id = SAI_SWITCH_ATTR_TELEMETRY_REPORT_DST_IP_LIST;
sai_ip4_t telemetry_dst_ip_list[3]
telemetry_dst_ip_list[0] = 0xc0a80c65; // 192.168.12.101
telemetry_dst_ip_list[0] = 0xc0a80c66; // 192.168.12.102
telemetry_dst_ip_list[0] = 0xc0a80c67; // 192.168.12.103
attr.value.ip4list.count = 3;
attr.value.ip4list.list = telemetry_dst_ip_list;
sai_switch_api-> set_switch_attribute(0, &attr);

// Set telemetry report virtual router ID
attr.id = SAI_SWITCH_ATTR_TELEMETRY_REPORT_VIRTUAL_ROUTER_ID;
attr.value.oid = default_vr_id;
sai_switch_api-> set_switch_attribute(0, &attr);

// Set telemetry report truncate size
attr.id = SAI_SWITCH_ATTR_TELEMETRY_REPORT_TRUNCATE_SIZE;
attr.value.u16 = 256;
sai_switch_api-> set_switch_attribute(0, &attr);

// Set telemetry report UDP destination port
attr.id = SAI_SWITCH_ATTR_TELEMETRY_REPORT_UDP_DST_PORT;
attr.value.u16 = 8890;
sai_switch_api-> set_switch_attribute(0, &attr);

// Enable INT endpoint
attr.id = SAI_SWITCH_ATTR_TELEMETRY_INT_ENDPOINT_ENABLE;
attr.value.booldata = true;
sai_switch_api-> set_switch_attribute(0, &attr);

// Set DSCP value for INT over L4
attr.id = SAI_SWITCH_ATTR_TELEMETRY_INT_L4_DSCP;
attr.value.ternaryfield.value = 0x17;
attr.value.ternaryfield.mask = 0x3f;
sai_switch_api-> set_switch_attribute(0, &attr);

// Set server-facing INT sink ports 
attr.id = = SAI_SWITCH_ATTR_TELEMETRY_INT_SINK_PORT_LIST;
sai_object_id_t telemetry_port_list[4];
telemetry_port_list[0] = port1_oid;
telemetry_port_list[1] = port2_oid;
telemetry_port_list[2] = port3_oid;
telemetry_port_list[3] = port4_oid;
attr.value.objlist.count = 4;
attr.value.objlist.list = telemetry_port_list;
sai_switch_api-> set_switch_attribute(0, &attr);

// Set flow-based report trigger - sensitivity to latency change
attr.id = SAI_SWITCH_ATTR_TELEMETRY_LATENCY_SENSITIVITY;
attr.value.u16 = 15;
sai_switch_api-> set_switch_attribute(0, &attr);

// Set flow-based report trigger - flow state clear cycle
attr.id = SAI_SWITCH_ATTR_TELEMETRY_FLOW_STATE_CLEAR_CYCLE;
attr.value.u16 = 1;
sai_switch_api-> set_switch_attribute(0, &attr);

// Create a queue threshold report trigger
sai_attribute_t queue_alert_attr[3];
sai_object_id_t queue_alert_id;
queue_alert_attr[0].id = SAI_TELEMETRY_QUEUE_ALERT_ATTR_QUEUE_ID;
queue_alert_attr[0].value.oid = queue_oid;
queue_alert_attr[1].id = SAI_TELEMETRY_QUEUE_ALERT_ATTR_QUEUE_DEPTH_THRESHOLD;
queue_alert_attr[1].value.u32 = 100;
queue_alert_attr[2].id = SAI_TELEMETRY_QUEUE_ALERT_ATTR_QUEUE_LATENCY_THRESHOLD;
queue_alert_attr[2].value.u32 = 1000;
sai_telemetry_api->create_telemetry_queue_alert(&queue_alert_id, 3, queue_alert_attr);

// Create an INT config session
sai_attribute_t int_session_attr[6];
sai_object_id_t int_session_id;
int_session_attr[0].id = SAI_TELEMETRY_INT_SESSION_ATTR_MAX_HOP_COUNT;
int_session_attr[0].value.u16 = 8;
int_session_attr[1].id = SAI_TELEMETRY_INT_SESSION_ATTR_INST_SWITCH_ID;
int_session_attr[1].value.booldata = true;
int_session_attr[2].id = SAI_TELEMETRY_INT_SESSION_ATTR_INST_SWITCH_PORTS;
int_session_attr[2].value.booldata = true;
int_session_attr[3].id = SAI_TELEMETRY_INT_SESSION_ATTR_INST_INGRESS_TIMESTAMP;
int_session_attr[3].value.booldata = true;
int_session_attr[4].id = SAI_TELEMETRY_INT_SESSION_ATTR_INST_EGRESS_TIMESTAMP;
int_session_attr[4].value.booldata = true;
int_session_attr[5].id = SAI_TELEMETRY_INT_SESSION_ATTR_INST_QUEUE_INFO;
int_session_attr[5].value.booldata = true;
sai_telemetry_api->create_telemetry_int_session(&int_session_id, 6, int_session_attr);

// Create an INT watchlist table
sai_attribute_t acl_table_attr[12];
sai_object_id_t int_watchlist_id;
acl_table_attr[0].id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
acl_table_attr[0].value.s32 = SAI_ACL_STAGE_INGRESS;
acl_table_attr[1].id = SAI_ACL_TABLE_ATTR_ACL_ACTION_TYPE_LIST;
int32_t acl_action_list[4];
acl_action_list[0] = SAI_ACL_ACTION_TYPE_TELEMETRY_INT_ENABLE;
acl_action_list[1] = SAI_ACL_ACTION_TYPE_TELEMETRY_INT_SESSION;
acl_action_list[2] = SAI_ACL_ACTION_TYPE_TELEMETRY_REPORT_ALL;
acl_action_list[3] = SAI_ACL_ACTION_TYPE_TELEMETRY_FLOW_SAMPLE_PERCENT;
acl_table_attr[1].value.s32list.count = 4;
acl_table_attr[1].value.s32list.list = acl_action_list;
acl_table_attr[2].id = SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE;
acl_table_attr[2].value.booldata = true;
acl_table_attr[3].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_IP;
acl_table_attr[3].value.booldata = true;
acl_table_attr[4].id = SAI_ACL_TABLE_ATTR_FIELD_DST_IP;
acl_table_attr[4].value.booldata = true;
acl_table_attr[5].id = SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL;
acl_table_attr[5].value.booldata = true;
acl_table_attr[6].id = SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT;
acl_table_attr[6].value.booldata = true;
acl_table_attr[7].id = SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT;
acl_table_attr[7].value.booldata = true;
acl_table_attr[8].id = SAI_ACL_TABLE_ATTR_FIELD_TUNNEL_VNI;
acl_table_attr[8].value.booldata = true;
acl_table_attr[9].id = SAI_ACL_TABLE_ATTR_FIELD_INNER_ETHER_TYPE;
acl_table_attr[9].value.booldata = true;
acl_table_attr[10].id = SAI_ACL_TABLE_ATTR_FIELD_INNER_SRC_IP;
acl_table_attr[10].value.booldata = true;
acl_table_attr[11].id = SAI_ACL_TABLE_ATTR_FIELD_INNER_DST_IP;
acl_table_attr[11].value.booldata = true;
sai_acl_api-> create_acl_table(&int_watchlist_id, 0, 12, acl_table_attr);

// Add an INT watchlist entry
sai_attribute_t acl_entry_attr[6];
sai_object_id_t int_watchlist_entry_id;
acl_entry_attr[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
acl_entry_attr[0].value.oid = int_watchlist_id;
acl_entry_attr[1].id = SAI_ACL_ENTRY_ATTR_PRIORITY;
acl_entry_attr[1].value.u32 = 100;
acl_entry_attr[2].id = SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL;
acl_entry_attr[2].value.aclfield.data.u8 = 6;
acl_entry_attr[2].value.aclfield.mask.u8 = 0xFF;
acl_entry_attr[3].id = SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT;
acl_entry_attr[3].value.aclfield.data.u16 = 80;
acl_entry_attr[3].value.aclfield.mask.u16 = 0xFFFF;
acl_entry_attr[4].id = SAI_ACL_ENTRY_ATTR_ACTION_TELEMETRY_INT_ENABLE;
acl_entry_attr[4].value.aclaction.enable = true;
acl_entry_attr[5].id = SAI_ACL_ENTRY_ATTR_ACTION_TELEMETRY_INT_SESSION;
acl_entry_attr[5].value.aclaction.parameter.oid = int_session_id;
sai_acl_api->create_acl_entry(&int_watchlist_entry_id, 0, 6, acl_entry_attr);

// Create a Mirror on Drop watchlist table
sai_attribute_t acl_table_attr[8];
sai_object_id_t mod_watchlist_id;
acl_table_attr[0].id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
acl_table_attr[0].value.s32 = SAI_ACL_STAGE_INGRESS;
acl_table_attr[1].id = SAI_ACL_TABLE_ATTR_ACL_ACTION_TYPE_LIST;
int32_t acl_action_list[1];
acl_action_list[0] = SAI_ACL_ACTION_TYPE_TELEMETRY_MOD_ENABLE;
acl_table_attr[1].value.s32list.count = 1;
acl_table_attr[1].value.s32list.list = acl_action_list;
acl_table_attr[2].id = SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE;
acl_table_attr[2].value.booldata = true;
acl_table_attr[3].id = SAI_ACL_TABLE_ATTR_FIELD_SRC_IP;
acl_table_attr[3].value.booldata = true;
acl_table_attr[4].id = SAI_ACL_TABLE_ATTR_FIELD_DST_IP;
acl_table_attr[4].value.booldata = true;
acl_table_attr[5].id = SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL;
acl_table_attr[5].value.booldata = true;
acl_table_attr[6].id = SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT;
acl_table_attr[6].value.booldata = true;
acl_table_attr[7].id = SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT;
acl_table_attr[7].value.booldata = true;
sai_acl_api-> create_acl_table(&mod_watchlist_id, 0, 8, acl_table_attr);

// Add a Mirror on Drop watchlist entry
sai_attribute_t acl_entry_attr[4];
sai_object_id_t mod_watchlist_entry_id;
acl_entry_attr[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
acl_entry_attr[0].value.oid = mod_watchlist_id;
acl_entry_attr[1].id = SAI_ACL_ENTRY_ATTR_PRIORITY;
acl_entry_attr[1].value.u32 = 100;
acl_entry_attr[2].id = SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL;
acl_entry_attr[2].value.aclfield.data.u8 = 6;
acl_entry_attr[2].value.aclfield.mask.u8 = 0xFF;
acl_entry_attr[3].id = SAI_ACL_ACTION_TYPE_TELEMETRY_MOD_ENABLE;
acl_entry_attr[3].value.aclaction.enable = true;
sai_acl_api->create_acl_entry(&mod_watchlist_entry_id, 0, 4, acl_entry_attr);
~~~
