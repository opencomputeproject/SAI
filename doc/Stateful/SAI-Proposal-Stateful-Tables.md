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
