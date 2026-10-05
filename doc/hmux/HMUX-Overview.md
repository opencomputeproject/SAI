# HMUX — Host Multiplexer: Overview, Architecture & Switchover

-------------------------------------------------------------------------------
 Title       | HMUX — Host Multiplexer: Overview, Architecture & Switchover
-------------|-----------------------------------------------------------------
 Authors     | Gokul CG (Marvell)
 Status      | Draft
 Type        | Informational (companion to warmboot checkpoint proposal)
 Created     | 2026-07-29
 Updated     | 2026-11-16
-------------------------------------------------------------------------------

## 1. What is HMUX?

HMUX (Host Multiplexer), also called **Hitless MUX**, is a hardware redundancy feature in external PHY devices that support **host/system-side multiplexing** (the host side of the PHY is also referred to as the **system side**). It allows two redundant control-plane nodes — typically **primary** and **secondary** supervisors or host CPUs — to share a single PHY while enabling near-zero-downtime data-plane switchover between them.

The term "hitless" reflects the core design goal: egress traffic continues to flow without interruption when the active host/system port changes. Ingress traffic from the line side is often replicated to both primary and secondary system-side ports so the standby can remain synchronized with live traffic.

---

## 2. Why is HMUX Needed?

### Carrier-Grade High Availability (HA)

Carrier-grade and enterprise platforms target very high availability. Hardware faults, software crashes, planned upgrades, and OIR (Online Insertion and Removal) should not cause visible traffic disruption. HMUX addresses this at the PHY layer so line-side link partners ideally avoid link flap and PCS errors when control-plane redundancy switches over.

| Property        | Cold standby                      | Hot standby (HMUX target)          |
| --------------- | --------------------------------- | ---------------------------------- |
| Backup state    | Inactive / powered down           | Running, synchronized              |
| Activation time | Boot + restore (minutes)          | Sub-second at PHY MUX              |
| Traffic impact  | Outage during switchover          | Near-zero loss (target)            |

HMUX enables **hot standby** at the PHY: the secondary node is attached to the same PHY; egress selection is an in-PHY MUX operation, decoupled from a single host CPU remaining up.

### Chassis architecture with redundant control plane

In a typical HA chassis:

- Line cards with PHY devices
- Two **supervisor** (or **route processor** / **control-plane**) cards — active and hot-standby — both reaching each line-card PHY on the system side
- The active card runs routing and control processes; the standby holds replicated state and can take over

Terminology varies by vendor (**supervisor**, **RP**, **host CPU**); this document uses **supervisor / host CPU** for the redundant cards that run the NOS and SAI.

Without HMUX, switchover often forces PHY re-init and a line-side flap. HMUX switches system-side attachment inside the PHY.

### Decoupling PHY from host CPU availability

Line-card PHYs are cut-through with limited buffering. HMUX reduces propagation of host/system-side transitions to the line partner during redundancy events.

---

### Where HMUX-class HA appears

- Data-centre and campus **core/spine** chassis with dual supervisors  
- **Service-provider edge** and carrier Ethernet with redundant control plane  
- **Industrial, financial, telecom, and public-safety** networks that cannot tolerate link flap on supervisor failover  

Underlying platform features (for example NSR/SSO, dual routing engines, virtual chassis) address **control-plane** protocol continuity; HMUX addresses **PHY/system-side** continuity on external PHY line cards.

---

## 3. HMUX architecture

### 3.1 Topology

```
  ┌────────────────────────────────────────────────┐
  │        Chassis (dual control-plane cards)      │
  │                                                │
  │  ┌──────────────────┐   ┌───────────────────┐  │
  │  │  Primary         │   │  Secondary        │  │
  │  │  (active)        │   │  (hot-standby)    │  │
  │  │  NOS + SAI       │   │  NOS + SAI        │  │
  │  └────────┬─────────┘   └────────┬──────────┘  │
  │           │  system / host side  │             │
  │           └──────────┬───────────┘             │
  │                      ▼                         │
  │           ┌──────────────────┐                 │
  │           │  External PHY    │                 │
  │           │  (HMUX)          │                 │
  │           └────────┬─────────┘                 │
  │                    │  line side                │
  └────────────────────┼───────────────────────────┘
                       ▼
                  Network partner
```

Egress between primary and secondary system-side ports is selected by PHY HMUX logic (vendor-specific). Operational modes and lane counts are PHY-vendor defined and are not specified here.

---

## 4. Use cases

| Scenario | Summary |
| -------- | ------- |
| Planned upgrade | Controlled switchover to standby; final state sync before MUX change. |
| OIR | Active supervisor removed; standby must serve traffic with pre-synced state. |
| Crash / failover | No orderly shutdown; continuous checkpoint pre-sync required. |
| Dual-supervisor chassis | Two independent OS instances; see Section 6–7. |

---

## 5. Graceful vs. ungraceful switchover

**Graceful** — Planned handoff: standby is verified ready, **final** SAI (and related) state is pushed to the standby, then HMUX moves egress to the standby system side. The **line-side link is protected**: the line partner should not see a link flap. The ideal outcome is **zero packet loss**; in practice, very few packets lost during the switchover window is acceptable.

**Ungraceful** — Active fails without warning: no final sync at failover time. The standby must already hold a **recent** replicated checkpoint.

| | Graceful | Ungraceful |
| --- | --- | --- |
| Planning | Yes | No |
| Sync before event | Final sync possible | Relies on continuous or periodic checkpoint sync |
| Standby recovery | Pre-sync + optional final delta | Last replicated checkpoint only |

**Recovery expectations (both):** line-side link remains up without flap; continue forwarding with zero or very few packets lost; judge success on **functional continuity** (operations, statistics, configuration, diagnostics), not register-identical state.

---

## Terminology: HA active and HA standby

These roles describe **which control-plane entity** performs an operation in dual-supervisor HA. They are unrelated to SAI attribute flags such as `READ_ONLY`.

| Term | Meaning |
| ---- | ------- |
| **HA active** | Entity that **persists** warmboot checkpoints (typically the active supervisor with a running switch). |
| **HA standby** | Entity that **imports** state at boot or failover (typically the standby supervisor). |

See [SAI-Proposal-Warmboot-Checkpoint.md](../SAI-Proposal-Warmboot-Checkpoint.md) for how `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS` is interpreted on a running switch vs after warm `sai_create_switch()`.

---

## 6. SAI layer considerations

HMUX adds a **second host CPU** (standby supervisor) that must run SAI after failover.

### Checkpoint on active (preferred)

1. Active `set(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT, N)` on configuration change (and before planned handoff); confirms `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS` ([warmboot proposal](../SAI-Proposal-Warmboot-Checkpoint.md)). `N` is a NOS-chosen unique save ID (`0` is a no-op).
2. Platform replicates checkpoint blob + `N` to the standby.
3. Standby `sai_create_switch()` with `SAI_SWITCH_ATTR_WARM_RECOVER=true` and the same `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT=N` (expected blob ID; `0` skips the check); on failure, cold boot per proposal.

---

## 7. Warmboot and HMUX: dual host CPUs

Classic SAI warmboot targets **one** control-plane entity restarting on **the same** CPU: export (often at `sai_remove_switch()`),
reboot, import with `SAI_SWITCH_ATTR_WARM_RECOVER`.

**HMUX introduces a new use case:** two redundant host CPUs attached to the same PHY. Recovery runs on the **standby**, not on the
failed active.

The standby cannot depend on the active completing `sai_remove_switch()` or `SAI_SWITCH_ATTR_RESTART_WARM` after an unplanned failure.
Continuous or triggered checkpoint plus sync closes that gap.

| Approach | When |
| -------- | ---- |
| **Checkpoint + ID sync** | Preferred for HMUX; HA active `set(CHECKPOINT, N)`, replicate blob + `N`, HA standby create with the same `CHECKPOINT=N` |

See [SAI-Proposal-Warmboot-Checkpoint.md](../SAI-Proposal-Warmboot-Checkpoint.md) for attributes, `cold` policy + latched `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS`, and atomic checkpoint commit (implementation-defined).
