 VLAN MLD Snooping Support
-------------------------------------------------------------------------------
 Title       | VLAN MLD Snooping Support
:-------------|:-----------------------------------------------------------------
 Authors     | Anandhi Dhanabalan, Marvell Technology Inc 
 Status      | In review
 Type        | Standards track
 Created     | 08/04/2026: Initial Draft
 -------------------------------------------------------------------------------


## 1. Overview

This enhancement adds support for MLD (Multicast Listener Discovery)
snooping control at the VLAN level. MLD is the IPv6 equivalent of IGMP,
used for managing IPv6 multicast group memberships. As networks transition
to IPv6 and dual-stack deployments, the ability to control MLD snooping
independently is essential for efficient multicast traffic management.



## 2. Motivation

### Problem

- No independent IPv6 multicast snooping control.

### Current SAI State

- Existing SAI VLAN attribute provides per-VLAN IGMP snooping control.
- No corresponding per-VLAN MLD snooping control exists.
- IPv4 and IPv6 multicast snooping cannot be configured independently.

This proposal addresses these limitations by introducing independent MLD
snooping control.



# 3. Proposed SAI Attribute Changes

## 3.1 New VLAN Attribute

```c
    /**
     * @brief MLD Snooping enable or disable control for VLAN
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_VLAN_ATTR_MLD_SNOOPING_ENABLE,
```

# 4. Behavioral Model

## 4.1 MLD Snooping Operation

When VLAN MLD snooping is enabled, MLD protocol messages are delivered to the CPU according to
the configured MLD traps (see Section 4.1.1). The control plane learns multicast
listener membership and programs multicast forwarding entries in hardware.
The interaction between VLAN MLD snooping and global MLD HOSTIF traps is
described in Section 4.2.

### 4.1.1 MLD Traps

In this document, "MLD traps" refers to the following HOSTIF trap types:

- `SAI_HOSTIF_TRAP_TYPE_IPV6_MLD_V1_V2`
- `SAI_HOSTIF_TRAP_TYPE_IPV6_MLD_V1_REPORT`
- `SAI_HOSTIF_TRAP_TYPE_IPV6_MLD_V1_DONE`
- `SAI_HOSTIF_TRAP_TYPE_MLD_V2_REPORT`

### **When MLD Snooping is ENABLED (per VLAN):**
```
Control Traffic:
  MLD Protocol Messages (ICMPv6 Types 130, 131, 132, 143)
    └─ Delivered to the CPU via configured MLD traps
          → Control plane learns multicast listener membership

Data Traffic:
  IPv6 Multicast Data (FF00::/8)
    └─ Forwarding behavior is determined by multicast forwarding entries
       programmed by the control plane and the configured unknown multicast
       handling policy (see Section 4.4).
```

> **Note on Unregistered Group Handling:**
> The behavior for unregistered IPv6 multicast groups is not defined by `SAI_VLAN_ATTR_MLD_SNOOPING_ENABLE`. It follows the platform's existing unregistered multicast handling policy, which may vary across implementations. Refer to Section 4.4

### **When MLD Snooping is DISABLED (per VLAN):**
```
Control Traffic:
  MLD Protocol Messages (ICMPv6 Types 130, 131, 132, 143)
    └─ Not delivered to the CPU via configured MLD traps
         → No multicast listener membership learning

Data Traffic:
  IPv6 Multicast Data (FF00::/8)
    └─ Existing multicast forwarding entries, if any, continue to apply.
       No multicast forwarding entries are learned through MLD snooping.
       Unknown multicast traffic follows the configured unknown multicast
       handling policy (see Section 4.4).
```



## 4.2 Interaction with Global MLD HOSTIF Traps

The VLAN-level MLD snooping attribute only controls whether MLD protocol
messages on the VLAN participate in multicast listener learning. It does
not create MLD HOSTIF traps by itself.

Both of the following conditions must be satisfied:

- At least one global MLD HOSTIF trap is configured.
- VLAN MLD snooping is enabled.

| Global MLD HOSTIF Traps | VLAN MLD Snooping | Listener Learning |
|-------------------------|-------------------|-------------------|
| Yes | Enabled | Enabled |
| Yes | Disabled | Disabled |
| No | Enabled | Unavailable |
| No | Disabled | Unavailable |

> **Note on HOSTIF trap packet action:**
>
> The configured HOSTIF trap packet action is applied only when an MLD packet
> matches a configured MLD HOSTIF trap and VLAN MLD snooping is enabled for
> that VLAN. This proposal does not modify the behavior of other HOSTIF traps
> or packet classification mechanisms.
---

## 4.3 Interaction with IGMP Snooping
These attributes should be independent.

Valid configurations:
```
IGMP=true,  MLD=true   // Both IPv4 & IPv6
IGMP=true,  MLD=false  // IPv4 only
IGMP=false, MLD=true   // IPv6 only
IGMP=false, MLD=false  // No IGMP/MLD snooping
```


## 4.4 Interaction with Unknown Multicast Flood Attributes
The protocol-specific unknown multicast output group attributes
- `SAI_VLAN_ATTR_UNKNOWN_IPV4_MCAST_OUTPUT_GROUP_ID` and
- `SAI_VLAN_ATTR_UNKNOWN_IPV6_MCAST_OUTPUT_GROUP_ID`

take precedence over

- `SAI_VLAN_ATTR_UNKNOWN_MULTICAST_FLOOD_GROUP`.

```
                Unknown Multicast
                       │
            Is a protocol-specific
           OUTPUT_GROUP_ID configured?
                       │
          ┌────────────┴─────────────────┐
          │                              │
         Yes                             No
          │                              │
Use protocol-specific          Is UNKNOWN_MULTICAST_
 OUTPUT_GROUP_ID               FLOOD_GROUP configured?
          │                              │
          │                   ┌──────────┴──────────┐
          │                  Yes                    No
          │                   │                     │
          ▼                   ▼                     ▼
   Forward using        Use UNKNOWN_        Platform default
 protocol-specific       MULTICAST_         behavior
   output group          FLOOD_GROUP

```
> **Platforms without protocol-specific unknown multicast output group attributes**
>
> Platforms that support only
> `SAI_VLAN_ATTR_UNKNOWN_MULTICAST_FLOOD_GROUP`
> support independent IGMP and MLD snooping. Registered multicast traffic
> continues to be forwarded based on multicast listener learning.
> The only limitation is that unknown IPv4 and unknown IPv6 multicast
> traffic share the same fallback forwarding policy because separate
> protocol-specific output groups are unavailable.
