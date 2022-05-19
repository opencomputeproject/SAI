# SAI Vlan Test plan
- [SAI Vlan Test plan](#sai-vlan-test-plan)
  - [Overriew](#overriew)
  - [Scope](#scope)
  - [Functionalities of VLAN](#functionalities-of-vlan)
    - [Forwarding](#forwarding)
    - [Sample API for forwarding attributes](#sample-api-for-forwarding-attributes)
    - [Lag](#lag)
      - [Flooding](#flooding)
      - [Ingress/Egress Unicast/Multicast/Broadcast](#ingressegress-unicastmulticastbroadcast)
  - [SAI APIs operations](#sai-apis-operations)
    - [Vlan Counters](#vlan-counters)
    - [Vlan and member list operations](#vlan-and-member-list-operations)
## Overriew
The purpose of this test plan is to test the VLAN function from SAI.
## Scope
The test will include two parts
1. Basic Vlan functionalities on tagged and untagged ports and LAGS
   - forwarding
   - Lag - flooding
   - Ingress/Egress Unicast/Multicast/Broadcast statistics for VLAN
2. SAI APIs operations
   - Vlan Counters
   - Vlan and member list operations

## Functionalities of VLAN 
### Forwarding
From SAI perspective a VLAN port will include four attributes:
* Tag Mode/Trunk: Tag the packet with a VLAN tag, for ingress, the switch will accept a tagged packet, and for egress, the switch will send out a tagged packet.
* Untag Mode/Access: Untag the packet, for ingress, the switch will accept an untagged packet, for egress, the switch will send out an untagged packet.
* Vlan id: Vlan id to a VLAN member.
* Native VLAN: Set VLAN member as a port attribute, for a trunk port, when ingress an untagged packet, the packet will be treated as tagged with native VLAN id, native VLAN only accept matched VLAN tag.


The detailed actions of VLAN ports forwarding:


| Port Tagged or Untagged | Direction | Action                                   |
| :------------------------ | :-------- | :--------------------------------------- |
| Untagged/access                  | Ingress   | Accept the packet.       |
|                           | Egress    | Untag the packet. |
| Tagged/Trunk                    | Ingress   | Accept the packet with matched Tag. |
|                           | Egress    | Tag the packet. |

Native VLAN (on tagged/trunk port):
| Packet Tag state | native VLAN matched | Action                                   |
| :------------------------ | :-------- | :--------------------------------------- |
| Untag                    | false   | Accept |
| Tag     | True    | Accept |
| Tag     | False    | Accept |

### Sample API for forwarding attributes 
Add tagged Vlan to a port
```python
    self.vlan10_member1 = sai_thrift_create_vlan_member(
        self.client,
        vlan_id=self.vlan10,
        bridge_port_id=self.bridge_port0,
        vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        
```
Add untagged vlan to a port
```python
self.vlan10_member2 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan10,
            bridge_port_id=self.bridge_port1,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
```
Set native vlan
```python
sai_thrift_set_port_attribute(
            self.client, self.port1, port_vlan_id=self.vlan10)
```

### Lag
#### Flooding
From SAI perspective, related to VLAN flooding, it will include three attributes:
disable mac learning: Disable the mac learning for a VLAN, it will enable the flooding for that VLAN
Lag Vlan member: Add the VLAN member to a lag
Lag on VLAN: Set the lag to a VLAN directly

| Ingress port type | Egreee port type | Action                                   |
| :------------------------ | :-------- | :--------------------------------------- |
| untag                  | some VLAN member in lag   | flooding to all VLAN including lag ports.       |
| Tag                    | lag with VLAN member   | flooding to other VLAN members, not in the tag. |
| unmatch tag                | VLAN    | drop. |

Sample SAI APIs
Disable mac learning
```python
sai_thrift_set_vlan_attribute(
                self.client, self.vlan20, learn_disable=False)
```
Lag Vlan member
```Python
self.vlan10_member = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan20,
            bridge_port_id=self.lag_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
```
Lag on vlan
```python
self.vlan20_member2 = sai_thrift_create_vlan_member(
            self.client,
            vlan_id=self.vlan20,
            bridge_port_id=self.lag2_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
```

#### Ingress/Egress Unicast/Multicast/Broadcast
Related attributes
flood control type: Disable the certain type of flooding includes:
- unknown_unicast_flood_control_type
- unknown_multicast_flood_control_type
- broadcast_flood_control_type

sample code
```python
    sai_thrift_set_vlan_attribute(
        self.client,
        vlan100,
        broadcast_flood_control_type=flood_control_type_none)
```
## SAI APIs operations
### Vlan Counters
Sample APIs and attributes
Clear counters
```Python
sai_thrift_clear_vlan_stats(self.client, self.vlan10)
```
Check counters
```Python
        in_bytes = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

```
### Vlan and member list operations
Check the process for creating VLAN
Check the error code for add invalidate vlan member, or get the attribute from invalidate vlan
Sample APIs
nagtive test
```python
    vlan_attr = sai_thrift_get_vlan_attribute(
        self.client, vlan_oid=11, learn_disable=True)

    incorrect_member = sai_thrift_create_vlan_member(
        self.client,
        vlan_id=11,
        bridge_port_id=self.port27_bp,
        vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
```
