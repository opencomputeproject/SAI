SAI Advanced Network Telemetry API Proposal
-------------------------------------------------------------------------------
 Title       | Advanced Network Telemetry API Proposal
-------------|-----------------------------------------------------------------
 Authors     | Barefoot Networks
 Status      | Draft
 Type        | Standards track
 Created     | 06/18/2017
 SAI-Version | XXX

-------------------------------------------------------------------------------

# Overview

This draft describes the API proposal to enable advanced telemetry capabilities in a network device. The main goal of this advanced telemetry proposal is to achieve per-packet network visibility with low overhead. The network device should be able to inspect and take telemetry actions on each individual data packet.

## Telemetry data plane workflow

![workflow](figures/workflow.png "Figure 1: Telemetry data plane workflow")

__Figure 1: Telemetry data plane workflow__

Figure 1 describes the telemetry data plane functionality. The telemetry module inspects every data packet without interfering with normal data forwarding pipeline. Different components may be located at different switches in the network. A telemetry watchlist table specifies the flows to monitor. It performs ternary match on the packet headers, and provides telemetry action parameters. Packets belonging to the specified flow spaces will be processed by the event detection logic. If a triggering event is detected, the switch will generate a report message to the monitor. The report message includes packet header and switch metadata associated with the packet (e.g., timestamp, ingress/egress ports, queue depth/latency).

Sections below introduce three different data plane telemetry capabilities: In-band Network Telemetry (INT), Packet Postcards, and Mirror on Drop.

## In-band Network Telemetry

![INT](figures/INT.png "Figure 2: In-band Network Telemetry")

__Figure 2: In-band Network Telemetry__

Figure 2 shows an example workflow of In-band Network Telemetry (INT). Switches along the route path add switch metadata into the packet header based on the *telemetry instructions* carried in the INT header.

Each switch may play the role of __endpoint__ or __transit__ for INT packets. INT endpoint acts both as __INT source__ and __INT sink__. As INT source it initiates INT operation by inserting INT header into a packet and thereby instructing other network devices along the routing path to add desired telemetry information into the packet. As INT sink it extracts the INT information from the packet and exports it using an ERSPAN mirror session to the monitor.

An __INT transit__ is a device along the path of a packet from source to sink. INT transit device adds its own INT information to the packet as requested by an INT source.


## Packet Postcards

![Postcard](figures/postcard.png "Figure 3: Postcard")

__Figure 3: Postcard Telemetry__

Figure 3 shows an example workflow of packet Postcards, an alternative telemetry approach. Each switch makes its own decision and reports packets info (aka “postcards”) to the monitor. Unlike INT, a Postcard switch never modifies the original data packets.

## Mirror on Drop

![MoD](figures/mod.png "Figure 4: Mirror on Drop")

__Figure 4: Mirror on Drop__

Figure 4 depicts the mirror on drop capability. Switches mirror packets dropped by the ingress pipe, egress pipe or queueing buffer to the monitor for network diagnosis. The report messages include packet header, switch metadata, and drop reason.

# Specification

This section describes the advanced telemetry API proposal.

## New Header `saitelemetry.h`

### Data Structures and Enumerations

#### Telemetry Attribute
~~~cpp
/**
 * @brief Telemetry feature types
 * INT_EP, INT_TRANSIT, POSTCARD are mutually exclusive
 * MOD can coexist with any one of the three above
 */
typedef enum _sai_telemetry_type_t {
    /** INT source and sink */
    SAI_TELEMETRY_TYPE_INT_EP,
    /** INT transit */
    SAI_TELEMETRY_TYPE_INT_TRANSIT,
    /** Postcard */
    SAI_TELEMETRY_TYPE_POSTCARD,
    /** Mirror on Drop */
    SAI_TELEMETRY_TYPE_MOD,
} sai_telemetry_type_t;

/**
 * @brief Telemetry attributes
 */
typedef enum _sai_telemetry_attr_t {
    /** List of telemetry features to enable */
    SAI_TELEMETRY_ATTR_TYPE_LIST,
    /** Globally unique switch ID */
    SAI_TELEMETRY_ATTR_SWITCH_ID,
    /** List of ERSPAN mirror sessions for sending reports */
    SAI_TELEMETRY_ATTR_MIRROR_LIST,

    /** INT sink downstream ports */
    SAI_TELEMETRY_ATTR_INT_SINK_PORT_LIST,
    /** Reserved DSCP value for INT over L4 */
    SAI_TELEMETRY_ATTR_INT_DSCP,
} sai_telemetry_attr_t;
~~~

#### Telemetry report triggering event detection
~~~cpp
/** Queue alert report trigger attributes */
typedef enum _sai_telemetry_queue_alert_attr_t {
    /** egress port */
    SAI_TELEMETRY_QUEUE_ALERT_ATTR_EGRESS_PORT,
    /** queue id */
    SAI_TELEMETRY_QUEUE_ALERT_ATTR_QUEUE_ID,
    /** queue depth threshold */
    SAI_TELEMETRY_QUEUE_ALERT_ATTR_QUEUE_DEPTH_THRESHOLD,
    /** queue latency threshold */
    SAI_TELEMETRY_QUEUE_ALERT_ATTR_QUEUE_LATENCY_THRESHOLD,
} sai_telemetry_queue_alert_attr_t;

/** Flow alert report trigger attributes */
typedef enum _sai_telemetry_flow_alert_attr_t {
    /** Flow state clear cycle */
    SAI_TELEMETRY_FLOW_ALERT_ATTR_FLOW_STATE_CLEAR_CYCLE,
    /** Latency sensitivity for flow state change detection */
    SAI_TELEMETRY_FLOW_ALERT_ATTR_LATENCY_SENSITIVITY,
} sai_telemetry_flow_alert_attr_t;
~~~

#### INT-specific attributes
~~~cpp
/** INT instructions */
typedef enum _sai_telemetry_int_instruction_t {
    /** Switch ID */
    SAI_TELEMETRY_INT_INST_SWITCH_ID,
    /** Ingress and egress ports */
    SAI_TELEMETRY_INT_INST_SWITCH_PORTS,
    /** Timestamp at ingress */
    SAI_TELEMETRY_INT_INST_INGRESS_TIMESTAMP,
    /** Timestamp at egress */
    SAI_TELEMETRY_INT_INST_EGRESS_TIMESTAMP,
    /** Queue ID and queue depth */
    SAI_TELEMETRY_INT_INST_QUEUE_INFO,
} sai_telemetry_int_instruction_t;

/** INT config session for endpoint switch */
typedef enum _sai_telemetry_int_session_attr_t {
    /** INT config session ID */
    SAI_TELEMETRY_INT_SESSION_ATTR_SESSION_ID
    /** INT max hop count */
    SAI_TELEMETRY_INT_SESSION_ATTR_MAX_HOP_COUNT,
    /** INT instruction list*/
    SAI_TELEMETRY_INT_SESSION_ATTR_INT_INST_LIST,
} sai_telemetry_int_session_attr_t;
~~~

### SAI API
~~~cpp
typedef sai_status_t (*sai_create_telemetry_fn)(
        _Out_ sai_object_id_t *telemetry_obj,
        _In_  uint32_t attr_count,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_telemetry_fn)(
        _In_ sai_object_id_t *telemetry_obj);

typedef sai_status_t (*sai_get_telemetry_attribute_fn)(
        _In_    sai_object_id_t telemetry_obj,
        _In_    uint32_t attr_count,
        _Inout_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_telemetry_attribute_fn)(
        _In_  sai_object_id_t telemetry_obj,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_create_telemetry_watchlist_entry_fn)(
        _Out_ sai_object_id_t *telemetry_watchlist_entry_id,
        _In_  uint32_t attr_count,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_telemetry_watchlist_entry_fn)(
        _In_ sai_object_id_t *telemetry_watchlist_entry_id);

typedef sai_status_t (*sai_get_telemetry_watchlist_entry_attribute_fn)(
        _In_    sai_object_id_t telemetry_watchlist_entry_id,
        _In_    uint32_t attr_count,
        _Inout_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_telemetry_watchlist_entry_attribute_fn)(
        _In_  sai_object_id_t telemetry_watchlist_entry_id,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_create_telemetry_queue_alert_fn)(
        _Out_ sai_object_id_t *telemetry_queue_alert_id,
        _In_  uint32_t attr_count,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_telemetry_queue_alert_fn)(
        _In_ sai_object_id_t * telemetry_queue_alert_id);

typedef sai_status_t (*sai_get_telemetry_queue_alert_attribute_fn)(
        _In_    sai_object_id_t telemetry_queue_alert_id,
        _In_    uint32_t attr_count,
        _Inout_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_telemetry_queue_alert_attribute_fn)(
        _In_  sai_object_id_t telemetry_queue_alert_id,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_create_telemetry_queue_alert_fn)(
        _Out_ sai_object_id_t *telemetry_queue_alert_id,
        _In_  uint32_t attr_count,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_telemetry_flow_alert_fn)(
        _In_ sai_object_id_t * telemetry_flow_alert_id);

typedef sai_status_t (*sai_get_telemetry_flow_alert_attribute_fn)(
        _In_    sai_object_id_t telemetry_flow_alert_id,
        _In_    uint32_t attr_count,
        _Inout_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_telemetry_flow_alert_attribute_fn)(
        _In_  sai_object_id_t telemetry_flow_alert_id,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_create_telemetry_int_session_fn)(
        _Out_ sai_object_id_t *telemetry_int_session_id,
        _In_  uint32_t attr_count,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_telemetry_int_session_fn)(
        _In_ sai_object_id_t *telemetry_int_session_id);

typedef sai_status_t (*sai_get_telemetry_int_session_attribute_fn)(
        _In_    sai_object_id_t telemetry_int_session_id,
        _In_    uint32_t attr_count,
        _Inout_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_telemetry_int_session_attribute_fn)(
        _In_  sai_object_id_t telemetry_int_session_id,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_create_telemetry_int_sink_port_fn)(
        _Out_ sai_object_id_t *telemetry_int_sink_port_id,
        _In_  uint32_t attr_count,
        _In_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_remove_telemetry_int_sink_port_fn)(
        _In_ sai_object_id_t *telemetry_int_sink_port_id);

typedef sai_status_t (*sai_get_telemetry_int_sink_port_attribute_fn)(
        _In_ sai_object_id_t telemetry_int_sink_port_id,
        _In_  uint32_t attr_count,
        _Out_  const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_set_telemetry_int_sink_port_attribute_fn)(
        _In_ sai_object_id_t telemetry_int_sink_port_id,
        _In_  const sai_attribute_t *attr_list);

typedef struct _sai_telemetry_api_t {
    sai_create_telemetry_fn                       create_telemetry;
    sai_remove_telemetry_fn                       remove_telemetry;
    sai_get_telemetry_attribute_fn                get_telemetry_attribute;
    sai_set_telemetry_attribute_fn                set_telemetry_attribute;

    sai_create_telemetry_queue_alert_fn           create_telemetry_queue_alert;
    sai_remove_telemetry_queue_alert_fn           remove_telemetry_queue_alert;
    sai_get_telemetry_queue_alert_attribute_fn    get_telemetry_queue_alert_attribute;
    sai_set_telemetry_queue_alert_attribute_fn    set_telemetry_queue_alert_attribute;

    sai_create_telemetry_flow_alert_fn            create_telemetry_flow_alert;
    sai_remove_telemetry_flow_alert_fn            remove_telemetry_flow_alert;
    sai_get_telemetry_flow_alert_attribute_fn     get_telemetry_flow_alert_attribute;
    sai_set_telemetry_flow_alert_attribute_fn     set_telemetry_flow_alert_attribute;

    sai_create_telemetry_int_session_fn           create_telemetry_int_session;
    sai_remove_telemetry_int_session_fn           remove_telemetry_int_session;
    sai_get_telemetry_int_session_attribute_fn    get_telemetry_int_session_attribute;
    sai_set_telemetry_int_session_attribute_fn    set_telemetry_int_session_attribute;

    sai_create_telemetry_int_sink_port_fn         create_telemetry_int_sink_port;
    sai_remove_telemetry_int_sink_port_fn         remove_telemetry_int_sink_port;
    sai_get_telemetry_int_sink_port_attribute_fn  get_telemetry_int_sink_port_attribute;
    sai_set_telemetry_int_sink_port_attribute_fn  set_telemetry_int_sink_port_attribute;
} sai_telemetry_api_t;
~~~

## Changes to `saiacl.h` for telemetry watchlist
~~~cpp
typedef enum _sai_acl_action_type_t
{
    ......

    /** Telemetry watch (default: true) */
    SAI_ACL_ACTION_TYPE_TELEMETRY_WATCH,
    /** Report every matched packet without event detection (default: false) */
    SAI_ACL_ACTION_TYPE_TELEMETRY_REPORT_ALL,
    /** Set INT config session ID */
    SAI_ACL_ACTION_TYPE_TELEMETRY_SET_INT_SESSION_ID,
} sai_acl_action_type_t;
~~~

~~~cpp
typedef enum _sai_acl_entry_attr_t
{
    ......

   /** Tunnel vni */
   SAI_ACL_ENTRY_ATTR_FIELD_TUNNEL_VNI,

   /** Telemetry watch (default: true) */
   SAI_ACL_ENTRY_ATTR_ACTION_TELEMETRY_WATCH,
   /** Report every matched packet without event detection (default: false) */
   SAI_ACL_ENTRY_ATTR_ACTION_TELEMETRY_REPORT_ALL,
   /** Set INT config session ID */
   SAI_ACL_ENTRY_ATTR_ACTION_TELEMETRY_SET_INT_SESSION_ID,
} sai_acl_entry_attr_t;
~~~

## Example
Example of configuring __INT Endpoint__ and __Mirror on Drop__ on a switch

#### Enable Telemetry Functionality
~~~cpp
sai_telemetry_query(SAI_API_TELEMETRY, &sai_telemetry_api);
sai_attribute_t telemetry_attr[5];
sai_object_id_t telemetry_obj;
sai_telemetry_type_t telemetry_type_list[2];

/** Enable INT and MoD */
telemetry_type_list[0] = SAI_TELEMETRY_TYPE_INT_EP;
telemetry_type_list[1] = SAI_TELEMETRY_TYPE_MOD;
telemetry_attr[0].id = SAI_TELEMETRY_ATTR_TYPE_LIST;
telemetry_attr[0].value.telemetry_type_list.count = 2;
telemetry_attr[0].value.telemetry_type_list.list = telemetry_type_list;

/** Configure globally unique switch ID */
telemetry_attr[1].id = SAI_TELEMETRY_ATTR_SWITCH_ID;
telemetry_attr[1].value.u32 = 0xfff222aa;

/** Add telemetry mirror session */
sai_telemetry_mirror_id_t telemetry_mirror_list[3];
telemetry_mirror_list[0] = 0;
telemetry_mirror_list[1] = 2;
telemetry_mirror_list[2] = 7;
telemetry_attr[2].id = SAI_TELEMETRY_ATTR_MIRROR_LIST;
telemetry_attr[2].value.u16list.count = 3;
telemetry_attr[2].value.u16list.list = telemetry_mirror_list;

/** Configure DSCP value for INT over L4 */
telemetry_attr[3].id = SAI_TELEMETRY_ATTR_INT_DSCP;
telemetry_attr[3].value.telemetry_int_dscp.value = 0x5c;
telemetry_attr[3].value.telemetry_int_dscp.mask = 0xfc;

/** Specify server-facing downstream ports for INT sink */
sai_telemetry_port_id_t telemetry_port_list[4];
telemetry_port_list[0] = 0;
telemetry_port_list[1] = 4;
telemetry_port_list[2] = 8;
telemetry_port_list[3] = 12;
telemetry_attr[4].id = SAI_TELEMETRY_ATTR_INT_SINK_PORT_LIST;
telemetry_attr[4].value.objlist.count = 4;
telemetry_attr[4].value.objlist.list = telemetry_port_list;

sai_telemetry_api->create_telemetry(&telemetry_obj, 5, telemetry_attr);
~~~

#### Configure Report Trigger
~~~cpp
/** Configure flow-based report trigger */
sai_attribute_t flow_alert_attr[2];
sai_object_id_t flow_alert_obj;
flow_alert_attr[0].id = SAI_TELEMETRY_FLOW_ALERT_ATTR_FLOW_STATE_CLEAR_CYCLE;
flow_alert_attr[0].value.u32 = 1;
flow_alert_attr[1].id = SAI_TELEMETRY_FLOW_ALERT_ATTR_LATENCY_SENSITIVITY;
flow_alert_attr[1].value.u16 = 15;
sai_telemetry_api->create_telemetry_flow_alert(&flow_alert_obj, 2, flow_alert_attr);

/** Create a queue threshold report trigger */
sai_attribute_t queue_alert_attr[4];
sai_object_id_t queue_alert_id;
queue_alert_attr[0].id = SAI_TELEMETRY_QUEUE_ALERT_ATTR_EGRESS_PORT;
queue_alert_attr[0].value.u16 = 2;
queue_alert_attr[1].id = SAI_TELEMETRY_QUEUE_ALERT_ATTR_QUEUE_ID;
queue_alert_attr[1].value.u16 = 0;
queue_alert_attr[2].id = SAI_TELEMETRY_QUEUE_ALERT_ATTR_QUEUE_DEPTH_THRESHOLD;
queue_alert_attr[2].value.u32 = 100;
queue_alert_attr[3].id = SAI_TELEMETRY_QUEUE_ALERT_ATTR_QUEUE_LATENCY_THRESHOLD;
queue_alert_attr[3].value.u32 = 1000;
sai_telemetry_api->create_telemetry_queue_alert(&queue_alert_id, 4, queue_alert_attr);
~~~

#### Create INT Config Session
~~~cpp
/** Create an INT config session */
sai_attribute_t int_session_attr[3];
sai_object_id_t int_session_id;
sai_telemetry_int_instruction_t instructions[5];
instructions[0] = SAI_TELEMETRY_INT_INST_SWITCH_ID;
instructions[1] = SAI_TELEMETRY_INT_INST_SWITCH_PORTS;
instructions[2] = SAI_TELEMETRY_INT_INST_INGRESS_TIMESTAMP;
instructions[3] = SAI_TELEMETRY_INT_INST_EGRESS_TIMESTAMP;
instructions[4] = SAI_TELEMETRY_INT_INST_QUEUE_INFO;
int_session_attr[0].id = SAI_TELEMETRY_INT_SESSION_ATTR_INT_INST_LIST;
int_session_attr[0].value.telemetry_int_inst_list.count = 5;
int_session_attr[0].value.telemetry_int_inst_list.list = instructions;
int_session_attr[1].id = SAI_TELEMETRY_INT_SESSION_ATTR_MAX_HOP_COUNT;
int_session_attr[1].value.u16 = 8;
int_session_attr[2].id = SAI_TELEMETRY_INT_SESSION_ATTR_SESSION_ID;
int_session_attr[2].value.u16 = 999;
sai_telemetry_api->create_telemetry_int_session(&int_session_id, 3, int_session_attr);
~~~

#### Add Telemetry Watchlist Entry
~~~cpp
/** Add INT watchlist entry */
sai_attribute_t acl_entry_attr[6];
sai_object_id_t int_watchlist_entry_id;
acl_entry_attr[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
acl_entry_attr[0].value.oid = telemetry_int_watchlist_obj; // previously created
acl_entry_attr[1].id = SAI_ACL_ENTRY_ATTR_PRIORITY;
acl_entry_attr[1].value.u32 = 100;
acl_entry_attr[2].id = SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL;
acl_entry_attr[2].value.aclfield.data.u8 = 6;
acl_entry_attr[2].value.aclfield.mask.u8 = 0xFF;
acl_entry_attr[3].id = SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT;
acl_entry_attr[3].value.aclfield.data.u16 = 80;
acl_entry_attr[3].value.aclfield.mask.u16 = 0xFFFF;
acl_entry_attr[4].id = SAI_ACL_ACTION_TYPE_TELEMETRY_WATCH;
acl_entry_attr[4].value.booldata = true;
acl_entry_attr[5].id = SAI_ACL_ACTION_TYPE_TELEMETRY_SET_INT_SESSION_ID;
acl_entry_attr[5].value.u16 = 999;
sai_acl_api->create_acl_entry(&int_watchlist_entry_id, switch_id, 6, acl_entry_attr);

/** Add Mirror on Drop watchlist entry */
sai_attribute_t acl_entry_attr[4];
sai_object_id_t mod_watchlist_entry_id;
acl_entry_attr[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
acl_entry_attr[0].value.oid = telemetry_mod_watchlist_obj; // previously created
acl_entry_attr[1].id = SAI_ACL_ENTRY_ATTR_PRIORITY;
acl_entry_attr[1].value.u16 = 100;
acl_entry_attr[2].id = SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL;
acl_entry_attr[2].value.aclfield.data.u8 = 6;
acl_entry_attr[2].value.aclfield.mask.u8 = 0xFF;
acl_entry_attr[3].id = SAI_ACL_ACTION_TYPE_TELEMETRY_WATCH;
acl_entry_attr[3].value.booldata = true;
sai_telemetry_api-> create_acl_entry(&mod_watchlist_entry_id, switch_id, 4, acl_entry_attr);
~~~
