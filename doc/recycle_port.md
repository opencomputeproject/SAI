RECYCLE PORT
-------------------------------------------------------------------------------
 Title       | SAI Recycle Port
-------------|-----------------------------------------------------------------
 Authors     | Sarath Gadagottu, Broadcom Inc.
 Status      | In review
 Type        | Standards track
 Created     | 03/17/2021
 SAI-Version | 1.7.2
 
 
-------------------------------------------------------------------------------

This spec talks about the Recycle port usecases.

It is helpful in certain scenarios for packets that have gone through a full 
switch pipeline to be re-injected into the beginning (Ingress) of the pipeline.
SONiC on a disaggregated VOQ chassis uses this capability. There are other examples.
Currently this can only be done via SAI by putting a port in loopback configuration.
But this takes a port away from regular network traffic. What is needed is a
dedicated Recycle port. Such a port must be treated like a regular network port
for all ASIC programming purposes (routing, bridging, ACL, queuing, tunnels, 
mirroring …). The only exception would be that it does not need to support some 
MAC layer functions.


A new generic port type for Recycle port is defined. The header mode is Ethernet
by default. Spec need to be updated for new header type for future applications.

```
/**
 * @brief Attribute data for #SAI_PORT_ATTR_TYPE
 */
typedef enum _sai_port_type_t
{
    /** Actual port. N.B. Different from the physical port. */
    SAI_PORT_TYPE_LOGICAL,

    /** CPU Port */
    SAI_PORT_TYPE_CPU,

    /** Fabric Port */
    SAI_PORT_TYPE_FABRIC,

    /** Recycle Port */
    SAI_PORT_TYPE_RECYCLE,

} sai_port_type_t;
```

SAI Adapter is expected to 
* Send Port oper status (UP) notification for the recycle port

### Usecase 1: Inband communication in VoQ System
Details are available at [VoQ HLD](https://github.com/Azure/SONiC/blob/master/doc/voq/voq_hld.md#251-inband-recycle-port-option)<br>
In this case L3 RIF, Neighbor are created on recycle port just like on SAI_PORT_TYPE_LOGICAL


### Usecase 2: Everflow - ERSPAN Monitor port is located on remote ASIC
In this case for Recycle port object id will be used for attribute SAI_MIRROR_SESSION_ATTR_MONITOR_PORT<br>
Before adding a ERSPAN session in local ASIC
* Add the necessary L3 RIF, Neighbor and Nexthop are created in the remote ASIC
* Add the corresponding RIF for remote system port, neighbor and nexthop in local ASIC
