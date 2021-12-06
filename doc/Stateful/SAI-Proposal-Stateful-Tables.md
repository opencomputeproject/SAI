# SAI Stateful Tables

Title       | Stateful Tables
------------|----------------
Authors     | Nvidia
Status      | In review
Type        | Standards track
Created     | 10/13/2021
SAI-Version | 1.10

## Use Cases
* __Stateful Firewall VNF__: this use case requires implementing stateful connection tracking mechanisms  both on programmable NICs and switches.
* __Telemetry__: this use case requires to implement per flow bandwidth estimation and exporting both in programmable NICs and switches.
* __In-network computation__: this use case investigates the use of programmable switches to offload allreduce operations.
* __Flow cache – Smart switch__: this use case implements a scalable tunneling mechanisms based on the differentiation of top talker flows (to be handled by the switch) and the reminders that are handled by the NIC.
* __Programmable Congestion Control__: this use case requires to offload a subset of transport protocol primitives directly to the programmable data plane (both switches and NICs).

## Requirements
### Programmable flow key extraction
The programmable node MUST be able to extract from the packet header a set of fields defined using a parser state machine.
The fields will be used to build the lookup key to retrieve the flow context.
### Flow registers
The programmable node MUST allow a programmer to define the structure of the flow context associating to each flow a configurable number of registers.
These registers can be accessed only by the corresponding flow.
For each register the size (number of bits) SHOULD be configurable.
Flow registers MUST be available to the pipeline as additional matching fields and inputs to the ALU.
### Flow counters
The programmable node MUST allow a programmer to instantiate counters. The flow counters increment by the number of pkt, bytes of each table entry that is matched by a lookup. Flow counters MUST be available to the pipeline as additional matching fields and inputs to the ALU.
### Flow policer
The programmable node MUST allow a programmer to instantiate flow policers. The policer is bound to the matching of a flow key and uses a three color code following the srDCM and trTCM policies as defined in RFC 2697.
Policers MUST be available to the pipeline as additional matching fields and inputs to the ALU.
### Value/mask rule insertion
The programmable node MUST be able to insert in the matching table a rule (composed by a key and a mask) extracted from packet header fields.
This can be useful in use case 4 to learn a whole subnet coming from the OTR.
Depending on the use case details the matching SHOULD support an LPM policy.
### Flow context storage in the data plane
The programmable node MUST support the storage of the flow context.
The programmable node MUST support allocation of the flow context for any “new” flow with programmable default values and insertion in a map that associates the flow key to the flow context.
Flow context entries SHOULD also be deleted.
It is important to highlight that flow creation time is a critical aspect, since the latency in flow insertion can create data races.
Specific countermeasures SHOULD be taken into consideration to avoid this issue.
To guarantee consistency, flow context is locally defined (i.e. it is not possible to read/modify entries in other tables).
If we need to pass information among different stages, they can be inserted as metadata.
### Flow context retrieve from the data plane
The programmable node MUST be able to retrieve the flow context and make it accessible from the reminder of the pipeline as packet metadata.
The flow context metadata MUST be available at the match/action stage of the pipeline.
The flow context fields MUST be treated as the other “matchable fields” already supported by the programmable node: packet fields, input/output ports, standard metadata, etc.
### Verification of arithmetic conditions
The programmable node MUST be able to match the results of a set of boolean expressions consisting of arithmetic conditions (greater than, equal to, less than, etc.) that take the flow context metadata and/or the header fields as input operands.
### Flow context update from the data plane
The action blocks of the programmable pipeline MUST support the modification of the flow context fields with:
1. Results of arithmetic/logic operations. At least AND,OR,NOT,XOR, ADD,SUB, logical/arithmetic shift and rotate,MUL,DIV.
2. Timestamp.
3. Header field values.
4. Other metadata.

Note that the programmable node MUST be able to handle race conditions due to concurrent state read/update.
A flow context scheduler SHOULD be designed to guarantee consistency by locking the pipeline only when two packets need to access the same flow context.
Some complex arithmetic operations are required only for specific use cases.
### Configurable flow aging mechanism
Flow context entries that are not accessed (read) in a configurable amount of time MUST be removed in order to prevent the flow context map to grow indefinitely.
### Configurable flow eviction policy
The programmable node MUST support the configuration of a flow eviction policy.
In other words, a programmer MUST be able to define what to do if a new flow has to be inserted in a “full” flow context map.
At least one of the following strategies MUST be supported:
1. the new flow is not inserted and a signal is generated;
2. the new flow is inserted and a flow already in the flow context map is deleted. Least Frequently Used, Least Recently Used, Random eviction policies SHOULD be supported. This requires to add specific flags in the flow context that are updated during the insertion/lookup operations;
3. the new flow is forwarded to the control plane.
### Bidirectional flow key extraction
The programmable node MUST support the extraction from a packet of a key that is the same for both directions of a flow (e.g.: from client to server and vice versa).
### Global registers
The programmable node MUST support the storage (read/write) of data variables that may be shared among all flows. These variables MUST be available in the match/action stage of the pipeline.
### Multiple stage operations
The programmable node MUST be able to execute a sequence of ALU operations. Such feature in conjunction with Req-12 permits the execution of multi-operand operations like for example: ```(a*b+c)/d```
### Flexible packet encapsulation/decapsulation
The programmable node MUST be able to encapsulate (and decapsulate) packet within a custom header defined by the programmers.
### Scalable programmable timers
The programmable node MUST support the scalable scheduling of timers.
A timer MUST be scheduled by an action.
The expiration of a timer MUST be treated as an event catchable by the match/action table.
A timer SHOULD carry a reference to the context of the flow that has scheduled it.
For the timer instantiation we have two options:
1. when a timer is set it is identified with an ID, which can be used to delete/update the timer
2. when a timer is set, it cannot be modified (no need for a timer ID) and it will always be fired. The application should manage that timer expiration may not have effects (e.g. if the timer delete an entry that is already deleted).
### Flow context and global storage read/update/insert/delete from control plane
The control plane must be able to read and modify the value stored in the flow context storage.
The control plane should be also able to insert and delete entries.

## API

```c
#if !defined (__SAIEBPF_H_)
#define __SAIEBPF_H_

/**
 * @brief State callback signature
 */
typedef void (* state_handler)(void *flow_ctx, void *global_ctx);
/**
 * @brief Set the next state
 *
 * Set the state callback to be executed when the stateful
 * table is applied to the next packet classified to
 * this flow.
 *
 * @param[in] state_handler state handler callback
 */
void sai_set_state(state_handler);

/**
 * @brief Defines a packet field ID
 */
typedef enum _sai_packet_field_t
{
    SAI_PACKET_FIELD_SRC_IPV6,

    SAI_PACKET_FIELD_DST_IPV6,

    SAI_PACKET_FIELD_INNER_SRC_IPV6,

    SAI_PACKET_FIELD_INNER_DST_IPV6,

    SAI_PACKET_FIELD_SRC_MAC,

    SAI_PACKET_FIELD_DST_MAC,

    SAI_PACKET_FIELD_SRC_IP,

    SAI_PACKET_FIELD_DST_IP,

    SAI_PACKET_FIELD_INNER_SRC_IP,

    SAI_PACKET_FIELD_INNER_DST_IP,

    SAI_PACKET_FIELD_L4_SRC_PORT,

    SAI_PACKET_FIELD_L4_DST_PORT,

    SAI_PACKET_FIELD_INNER_L4_SRC_PORT,

    SAI_PACKET_FIELD_INNER_L4_DST_PORT,

    SAI_PACKET_FIELD_IP_PROTOCOL,

    SAI_PACKET_FIELD_INNER_IP_PROTOCOL,

    SAI_PACKET_FIELD_TCP_FLAGS,

    SAI_PACKET_FIELD_INNER_TCP_FLAGS,

} sai_packet_field_t;

/**
 * @brief Get byte packet field
 *
 * @param[in] field_id field ID
 *
 * @return value of that field in the packet
 */
unsigned char sai_load_packet_field_u8(sai_packet_field_t field_id);

/**
 * @brief Get 2 byte packet field
 *
 * @param[in] field_id field ID
 *
 * @return value of that field in the packet
 */
unsigned short sai_load_packet_field_u16(sai_packet_field_t field_id);

/**
 * @brief Get 4 byte packet field
 *
 * @param[in] field_id field ID
 *
 * @return value of that field in the packet
 */
unsigned int sai_load_packet_field_u32(sai_packet_field_t field_id);

#endif /** __SAIEBPF_H_ */
```

## Example

```
               ┌────────────────────────────────────────┐
               │  ┌─────────────┐         ┌───────────┐ │
               │  │             │         │           │ │
   ┌──────┐    │  │             │         │           │ │
   │ NP   ├────┼──►             │         │           │ │
   │      │    │  │             │         │           │ │
   └──────┘    │  │ Conntrack   │         │  FW       │ │
               │  │   ACL       ├─┐    ┌──►  ACL      │ │
   ┌──────┐    │  │             │ │    │  │           │ │
   │ CP   │    │  │             │ │    │  │           │ │
   │      ├────┼──►             │ │    │  │           │ │
   └──────┘    │  │             │ │    │  │           │ │
               │  └─────────────┘ │    │  └───────────┘ │
               └──────────────────┼────┼────────────────┘
                                  │    │
                    ┌─────────────▼────┴────────────┐
                    │         Conntrack             │
                    │      State Table              │
                    └───────────────────────────────┘
```

```c
/**
 * Create a flow context for a conntrack table, will be referenced by ACL
 */

sai_object_id_t conntrack_flow_ctx_oid;
 
attr.id = SAI_STATEFUL_METADATA_ATTR_SIZE;
attr.value.u32 = 2;

/**
 * Create a conntrack stateful table
 */

sai_object_id_t conntrack_table_oid;
sai_attribute_t attr;

attr.id = SAI_STATEFUL_TABLE_ATTR_SIZE;
attr.value.u32 = 1000000;

attr.id = SAI_STATEFUL_TABLE_ATTR_KEY_0;
attr.value.s32list.count = 1;
attr.value.s32list.list = [
    SAI_FLOW_KEY_FIELD_SRC_IP,
	SAI_FLOW_KEY_FIELD_DST_IP,
	SAI_FLOW_KEY_FIELD_L4_SRC_PORT,
	SAI_FLOW_KEY_FIELD_L4_DST_PORT,
	SAI_FLOW_KEY_FIELD_IP_PROTOCOL];

attr.id = SAI_STATEFUL_TABLE_ATTR_KEY_1;
attr.value.s32list.count = 1;
attr.value.s32list.list = [
	SAI_FLOW_KEY_FIELD_DST_IP,
	SAI_FLOW_KEY_FIELD_SRC_IP,
	SAI_FLOW_KEY_FIELD_L4_DST_PORT,
	SAI_FLOW_KEY_FIELD_L4_SRC_PORT,
	SAI_FLOW_KEY_FIELD_IP_PROTOCOL];

attr.id = SAI_STATEFUL_TABLE_ATTR_EVICTION_POLICY;
attr.value.s32 = SAI_STATEFUL_TABLE_EVICTION_POLICY_LRU;

attr.id = SAI_STATEFUL_TABLE_ATTR_FLOW_CONTEXT;
attr.value.s32 = conntrack_flow_ctx_oid;

/**
 * Create table to drop TCP traffic from network ports by default
 */

sai_object_id_t port_table_oid;

attr.id = SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST;
attr.value.s32list.count = 1;
attr.value.s32list.list = SAI_ACL_BIND_POINT_TYPE_PORT;

attr.id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
attr.value.s32 = SAI_ACL_STAGE_INGRESS;

attr.id = SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL;
attr.value.bool = true;

attr.id = SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS;
attr.value.bool = true;

/**
 * Create rule to drop TCP traffic from network ports by default
 */

sai_object_id_t port_rule_id;

attr.id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
attr.value.oid = port_table_oid;

attr.id = SAI_ACL_ENTRY_ATTR_PRIORITY;
attr.value.u32 = 0;

attr.id = SAI_ACL_ENTRY_ATTR_ADMIN_STATE;
attr.value.booldata = true;

attr.id = SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS;
attr.value.objlist = network_ports;

attr.id = SAI_ACL_ENTRY_ATTR_ACTION_PACKET_ACTION;
attr.value.packet_action = SAI_PACKET_ACTION_DISCARD;

/**
 * Create table to apply connection tracking to TCP packets
 */

sai_object_id_t conntrack_table_oid;

attr.id = SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST;
attr.value.s32list.count = 1;
attr.value.s32list.list = SAI_ACL_BIND_POINT_TYPE_PORT;

attr.id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
attr.value.s32 = SAI_ACL_STAGE_INGRESS;

attr.id = SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL;
attr.value.bool = true;

/**
 * ...
 * These two tables are members of a sequential table group, port table being applied first
 * ...
 */
```

```c
#include "inc/saiebpf.h"

typedef enum _connection_state_t {
    CONNECTION_STATE_CLOSED = 0,
    CONNECTION_STATE_SYN_SENT = 1,
    CONNECTION_STATE_OPEN = 2,
} connection_state_t;

typedef struct _flow_ctx_t {
    unsigned char connection_state;
} flow_ctx_t;

void wait_synack(void *flow_ctx, void *global_ctx);
void allow(void *flow_ctx, void *global_ctx);
void wait_finack(void *flow_ctx, void *global_ctx);
void wait_ack(void *flow_ctx, void *global_ctx);

void start(void *flow_ctx, void *global_ctx)
{
    flow_ctx_t *fc = (flow_ctx_t *)flow_ctx;
    unsigned short tcp_flags = sai_load_packet_field_u16(14);

    if (tcp_flags != 0x02 /* TCP SYN */)
    {
        return;
    }

    fc->connection_state = CONNECTION_STATE_SYN_SENT;

    sai_set_state(allow);
}

void wait_synack(void *flow_ctx, void *global_ctx)
{
    flow_ctx_t *fc = (flow_ctx_t *)flow_ctx;
    unsigned short tcp_flags = sai_load_packet_field_u16(14);

    if (tcp_flags & 0x12 /* SYNACK */)
    {
        fc->connection_state = CONNECTION_STATE_OPEN;
        sai_set_state(allow);
    }
}

void allow(void *flow_ctx, void *global_ctx)
{
    flow_ctx_t *fc = (flow_ctx_t *)flow_ctx;
    unsigned short tcp_flags = sai_load_packet_field_u16(14);

    if (tcp_flags & 0x4 /* RST */)
    {
        fc->connection_state = CONNECTION_STATE_CLOSED;
        sai_set_state(start);
    }

    if (tcp_flags & 0x1 /* FIN */)
    {
        sai_set_state(wait_finack);
    }
}

void wait_finack(void *flow_ctx, void *global_ctx)
{
    unsigned short tcp_flags = sai_load_packet_field_u16(14);

    if (tcp_flags & 0x11 /* FINACK */)
    {
        sai_set_state(wait_ack);
    }
}

void wait_ack(void *flow_ctx, void *global_ctx)
{
    flow_ctx_t *fc = (flow_ctx_t *)flow_ctx;
    unsigned short tcp_flags = sai_load_packet_field_u16(14);

    if (tcp_flags & 0x10 /* ACK */)
    {
        fc->connection_state = CONNECTION_STATE_CLOSED;
        sai_set_state(start);
    }
}
```

```
start:
       0:       bf 16 00 00 00 00 00 00         r6 = r1
       1:       b7 01 00 00 10 00 00 00         r1 = 16                            // set param 1 to TCP_FLAGS
       2:       85 10 00 00 ff ff ff ff         call -1                            // call sai_load_packet_field_u16, result saved in r0
                0000000000000010:  R_BPF_64_32  sai_load_packet_field_u16
       3:       55 00 05 00 02 00 00 00         if r0 != 2 goto +5 <LBB0_2>        // if tcp_flags is not SYN, return
       4:       b7 01 00 00 01 00 00 00         r1 = 1
       5:       73 16 00 00 00 00 00 00         *(u8 *)(r6 + 0) = r1               // fc->connection_state = CONNECTION_STATE_SYN_SENT;
       6:       18 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00         r1 = 0 ll
                0000000000000030:  R_BPF_64_64  allow
       8:       85 10 00 00 ff ff ff ff         call -1                            // set state to allow
                0000000000000040:  R_BPF_64_32  sai_set_state
LBB0_2:
       9:       95 00 00 00 00 00 00 00         exit
```
