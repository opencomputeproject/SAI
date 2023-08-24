Switchport Mode Hybrid Proposal
=====================

Title    | Switchport Mode Hybrid Proposal
-------- | ---
Authors  | Rida Hanif , Hafiz Mati Ur Rehman
Company  | xFlow Research Inc
Status   | In review
Created  | 08/24/2023
----------

**Background:**
============
This document provides an enhancement in an existing [HLD#912](https://github.com/sonic-net/SONiC/pull/912)


**Introduction**
============


A hybrid switchport is a versatile port that can handle both tagged and untagged traffic in both ingress and egress directions. For incoming traffic, it can receive packets with VLAN tags (tagged) or without VLAN tags (untagged) from various VLANs. For outgoing traffic, it can forward packets with the appropriate VLAN tags based on the VLAN tag information in the packet or add VLAN tags to untagged packets before sending them out to the appropriate VLANs.


***Functional Requirement***
-------------------------


1. The hybrid switchport sets a Port VLAN ID (PVID) for any untagged vlan members. This PVID assignment determines the VLAN to which untagged packets belong.

2. The hybrid switchport must inspect the VLAN ID of every incoming tagged packet.If the VLAN ID matches one of the VLANs permitted on the port, the interface should accept the packet for further processing and forwarding.

3. If the VLAN ID does not match any of the allowed VLANs, the interface should drop the packet to prevent unauthorized traffic.

4. When forwarding VLAN packets, the hybrid switchport should examine the VLAN ID carried within the packet.

5. For Untagged packets, the switchport forward the packet to the appropriate destination based on the PVID setting.

6. For packets with a tagged VLAN ID, the switchport must retain the VLAN tag and forward the packet with the tag intact to ensure proper delivery to the destination VLAN.

7. The hybrid switchport should support configuration options for allowing or denying specific VLANs on the port.It should enable administrators to configure the PVID for untagged packets and define which tagged VLANs are permitted to pass through the port.



***Sample Topology***
-------------------------

![Hybrid-port](https://github.com/ridahanif96/SAI/assets/61490193/9ed501fc-9604-4974-ba62-11021b2758fe)



***SAI Changes***
-------------------------

****Option 1**** 


We can add switchport mode hybrid in SONiC using PVID Concept.The PVID of a port is the VLAN id that will be assigned to any untagged frames entering the switch on that port. A Port can have only one PVID. Currently, untagged vlan is considered as PVID. 

Now we are proposing that behavior of multiple untagged vlan members works same as  of multiple tagged vlan members current behavior in SONIC.

This can be done by adding/updating same functionality used in PortOrch::doVlanMemberTask(). A port handles multiple untagged members in same way as it handles for tagged members.

For this, do we need any change in SAI? Need guidance from SAI Members for this. 
Ideally this is the most apporperiate one. 



****Option 2****


We can add Hybrid port mode by introducing concept of PVID in SAI. Currently, a port can have one PVID which is Vlan Id of untagged member port.  When there are multiple untagged members, we might need to add multiple PVID configuration as well. Currently there is no concepts of PVID in SAI and ASIC SDK as far as our understanding. Need guidance from SAI members for this. 



****Option 3****

We propose a new attribute in saibridge.h to support hybrid port as follows: 

```
/**
 * @brief Attribute data for #SAI_BRIDGE_PORT_ATTR_TAGGING_MODE
 */
typedef enum _sai_bridge_port_tagging_mode_t
{
    /** Untagged mode */
    SAI_BRIDGE_PORT_TAGGING_MODE_UNTAGGED,

    /** Tagged mode */
    SAI_BRIDGE_PORT_TAGGING_MODE_TAGGED,

    /** Hybrid mode */
    SAI_BRIDGE_PORT_TAGGING_MODE_HYBRID,

} sai_bridge_port_tagging_mode_t;

```
But as per IEEE 802.1Q Standard a bridge has only two modes (Tagged, Untagged) to handle Vlan traffic.




***Proposed CLI Commands***
-------------------------
1. config switchport mode hybrid <member_portname>/<member_portchannel>
2. config switchport hybrid <untagged|tagged>  <vid>
3. config switchport hybrid pvid vlan set/add
4. config vlan member add -u -m <vid> <member_portname>
5. config switchport hybrid tagged allowed vlan <vlan-range>

