# SAI APSU / ILT Attributes

## Overview

This document maps the SAI APSU/ILT port attributes to their normative definitions in **IEEE P802.3dj Annex 178B** (*Autonomous PHY Start-Up (APSU) and In-band Link Training (ILT)*).

APSU defines a procedure by which two connected PHY interfaces autonomously bring up a link — including in-band link training (ILT) and a Ready-to-Send (RTS) handshake — without requiring out-of-band coordination. The procedure applies to both the host interface (C2M AUI) and the media interface (PMD to medium).

**Reference specification:** IEEE P802.3dj/D3.1, Annex 178B (4 June 2026 draft)

---

## Architecture Summary

```
Per-LANE variables (independent per lane):
  local_tf_lock       — local receiver has identified training frame boundaries
  remote_tf_lock      — peer receiver has identified training frame boundaries (from status field)
  local_rx_ready      — local receiver fully adapted, ready for data
  remote_rx_ready     — peer receiver fully adapted (from status field)
  training_failure    — training failed to complete on this lane
  lane_training_status — aggregate per-lane training state (IN_PROGRESS/TRAINED/OK/FAIL)

Per-INTERFACE variables (aggregated across all lanes):
  training_status     — FAIL/IN_PROGRESS/READY/OK
  isl_ready           — local_rx_ready AND remote_rx_ready true for ALL lanes
  local_rts           — this interface is ready to send data
  remote_rts          — peer interface is ready to send (all peer lanes have NRTS=0)
  rts_status          — FAIL/IN_PROGRESS/READY/OK

Management variables (per interface):
  mr_training_enable  — enables/disables ILT training on the interface
  mr_restart          — triggers a restart of the RTS and ILT functions
```

---

## Enum Definitions

### `sai_port_ilt_training_status_t`

Maps to the IEEE `training_status` per-interface variable (§178B.8.2.1, MDIO 1.1476.11:10).

| SAI Enum Value | IEEE Value | Description |
|---|---|---|
| `SAI_PORT_ILT_TRAINING_STATUS_RESERVED` | `—` | Unknown / not applicable |
| `SAI_PORT_ILT_TRAINING_STATUS_FAIL` | `FAIL` | At least one lane has `training_failure = true` |
| `SAI_PORT_ILT_TRAINING_STATUS_IN_PROGRESS` | `IN_PROGRESS` | At least one lane is training; no lanes have failed |
| `SAI_PORT_ILT_TRAINING_STATUS_READY` | `READY` | All lanes are TRAINED or OK with at least one TRAINED |
| `SAI_PORT_ILT_TRAINING_STATUS_OK` | `OK` | All lanes are OK (fully trained and stable) |

**Aggregation logic (§178B.8.2.1):**
- `FAIL` if ANY lane has `lane_training_status = FAIL`
- `IN_PROGRESS` if ≥1 lane is `IN_PROGRESS` and no lane is `FAIL`
- `READY` if ALL lanes are `TRAINED` or `OK` and ≥1 lane is `TRAINED`
- `OK` if ALL lanes are `OK`

### `sai_port_ilt_rts_status_t`

Maps to the IEEE `rts_status` per-interface variable (§178B.8.2.1, MDIO 1.1476.14:13).

| SAI Enum Value | IEEE Value | Description |
|---|---|---|
| `SAI_PORT_ILT_RTS_STATUS_RESERVED` | `—` | Unknown / not applicable |
| `SAI_PORT_ILT_RTS_STATUS_FAIL` | `FAIL` | `training_status = FAIL`; RTS cannot proceed |
| `SAI_PORT_ILT_RTS_STATUS_IN_PROGRESS` | `IN_PROGRESS` | RTS handshake in progress |
| `SAI_PORT_ILT_RTS_STATUS_READY` | `READY` | Local RTS set; awaiting remote RTS |
| `SAI_PORT_ILT_RTS_STATUS_OK` | `OK` | Both local and remote RTS established; ISL in DATA mode |

### `sai_port_ilt_lane_training_status_t`

Maps to the IEEE `lane_training_status` per-lane variable (§178B.8.3.1, MDIO 1.1477.1:0).

| SAI Enum Value | IEEE Value | Description |
|---|---|---|
| `SAI_PORT_ILT_LANE_TRAINING_STATUS_RESERVED` | `—` | Unknown / not applicable |
| `SAI_PORT_ILT_LANE_TRAINING_STATUS_FAIL` | `FAIL` | Training failed on this lane |
| `SAI_PORT_ILT_LANE_TRAINING_STATUS_IN_PROGRESS` | `IN_PROGRESS` | Lane is actively training |
| `SAI_PORT_ILT_LANE_TRAINING_STATUS_TRAINED` | `TRAINED` | Lane completed training; both sides RX ready |
| `SAI_PORT_ILT_LANE_TRAINING_STATUS_OK` | `OK` | Lane is in DATA mode and operational |

---

## SAI Port Attribute to IEEE 802.3dj Variable Mapping

### Control Attributes

| SAI Attribute | IEEE Variable | IEEE Section | MDIO Register (PMD) | MDIO Ref | Granularity | SAI Type |
|---|---|---|---|---|---|---|
| `SAI_PORT_ATTR_APSU_MR_TRAINING_ENABLE` | `mr_training_enable` | §178B.8.2.1 | `1.150.1` | 45.2.1.95 | Per interface | `bool` (default `true`) |
| `SAI_PORT_ATTR_APSU_MR_RESTART` | `mr_restart` | §178B.8.2.1 | `1.150.0` | 45.2.1.95 | Per interface | `bool` (self-clearing, default `false`) |

**`mr_training_enable`** (§178B.8.2.1): When `true`, ILT training is enabled on the interface. Must be set to the same value at both ends of each ISL. Disabling training on ILT-capable ISLs may compromise APSU and link performance.

**`mr_restart`** (§178B.8.2.1): When set to `true`, triggers a restart of both the RTS and ILT state machines on the interface. The attribute is self-clearing — after the restart is initiated, hardware clears the bit, allowing subsequent restarts to be triggered by writing `true` again.

---

### Per-Interface Status Attributes

| SAI Attribute | IEEE Variable | IEEE Section | MDIO Register (PMD) | MDIO Ref | SAI Type |
|---|---|---|---|---|---|
| `SAI_PORT_ATTR_ILT_TRAINING_STATUS` | `training_status` | §178B.8.2.1 | `1.1476.11:10` | 45.2.1.168c | `sai_port_ilt_training_status_t` |
| `SAI_PORT_ATTR_ILT_RTS_STATUS` | `rts_status` | §178B.8.2.1 | `1.1476.14:13` | 45.2.1.168c | `sai_port_ilt_rts_status_t` |
| `SAI_PORT_ATTR_APSU_LOCAL_RTS_STATUS` | `local_rts` | §178B.8.2.1 | `1.1320.10` | 45.2.1.165 | `sai_latch_status_t` |
| `SAI_PORT_ATTR_APSU_REMOTE_RTS_STATUS` | `remote_rts` | §178B.8.2.1 | `1.1120.10` | 45.2.1.161 | `sai_latch_status_t` |
| `SAI_PORT_ATTR_APSU_ISL_READY` | `isl_ready` | §178B.8.2.1 | `1.1476.8` | 45.2.1.168c | `sai_latch_status_t` |

**`training_status`**: Per-interface aggregation of all per-lane `lane_training_status` values. See enum table above.

**`rts_status`**: Tracks the RTS handshake state machine. Transitions from `IN_PROGRESS` → `READY` (local RTS set) → `OK` (remote RTS received, ISL enters DATA mode). Set to `FAIL` if `training_status = FAIL`.

**`local_rts`**: Single boolean per interface. Set to `true` after all lanes reach `TRAINED`/`OK` and the clock swap completes. Its logical-NOT is encoded as the **Not Ready to Send (NRTS)** bit in the control field of every transmitted training frame (§178B.7.4.2).

**`remote_rts`**: Set to `true` when received training frames on **all lanes** of the interface have NRTS bit = 0. Represents `local_rts` of the peer interface as observed locally.

**`isl_ready`**: Set to `true` when `local_rx_ready` AND `remote_rx_ready` are `true` for **all lanes** of the interface (§178B.8.2.1). Triggers the RTS state machine. This is the primary observable indicator that bidirectional training has converged on all lanes.

---

### Per-Lane Status Attributes

The following attributes return a list of per-lane values (`sai_port_lane_latch_status_list_t`), with one entry per lane of the interface.

| SAI Attribute | IEEE Variable | IEEE Section | MDIO Register (PMD, lane 0) | MDIO Ref | SAI Type |
|---|---|---|---|---|---|
| `SAI_PORT_ATTR_APSU_LOCAL_TF_LOCK_STATUS` | `local_tf_lock` | §178B.8.3.1 | `1.151.1` (lane 0–3); see 45.2.1.96 for lanes 4–7 | 45.2.1.96 | `sai_port_lane_latch_status_list_t` |
| `SAI_PORT_ATTR_APSU_REMOTE_TF_LOCK_STATUS` | `remote_tf_lock` | §178B.8.3.1 | `1.1420.9` (lane 0; lanes 1–7 offset in same reg) | 45.2.1.167 | `sai_port_lane_latch_status_list_t` |
| `SAI_PORT_ATTR_APSU_LOCAL_RX_READY_STATUS` | `local_rx_ready` | §178B.8.3.1 | `1.1420.15` (lane 0; lanes 1–7 offset in same reg) | 45.2.1.167 | `sai_port_lane_latch_status_list_t` |
| `SAI_PORT_ATTR_APSU_REMOTE_RX_READY_STATUS` | `remote_rx_ready` | §178B.8.3.1 | `1.1220.15` (lane 0; lanes 1–7 offset in same reg) | 45.2.1.163 | `sai_port_lane_latch_status_list_t` |
| `SAI_PORT_ATTR_APSU_LOCAL_TRAINING_FAILURE_STATUS` | `training_failure` | §178B.8.3.1 | `1.151.3` (lane 0–3); see 45.2.1.101 for lanes 4–7 | 45.2.1.96 | `sai_port_lane_latch_status_list_t` |

> **Note on MDIO lane addressing:** Register bit references above are for lane 0 on a PMD. Lanes 1–7 are accessed at defined offsets within the same register (footnote c in Table 178B–7). Mappings for upper/lower AUI components are defined in §176C.8 and §176D.9 respectively.

**`local_tf_lock`** (§178B.8.3.1): `true` when the training frame marker position has been identified on a lane. Set by the training frame lock state diagram (Figure 178B–11). Reflected in the **Receiver frame lock** bit (bit 9) of the training frame status field (§178B.7.5.4), making it observable by the peer as `remote_tf_lock`.

**`remote_tf_lock`** (§178B.8.3.1): Reflects `local_tf_lock` of the peer on this lane. Derived from the Receiver frame lock bit (bit 9) of received training frames (Table 178B–4, Table 178B–5).

**`local_rx_ready`** (§178B.8.3.1): `true` when the receiver on a lane has completed equalization and is ready for data. Set by the training control state diagram (Figure 178B–10a). Encoded as the **Receiver ready** bit (bit 15) of the training frame status field (§178B.7.5.1).

**`remote_rx_ready`** (§178B.8.3.1): Reflects `local_rx_ready` of the peer on this lane. Derived from the Receiver ready bit (bit 15) of received training frames. `isl_ready` is `true` when this is `true` on all lanes.

**`training_failure`** (§178B.8.3.1): `true` when training fails to complete on a lane, typically due to `max_wait_timer` expiry. Set by the training control state diagram (Figure 178B–10a). When `true` on any lane, it causes `training_status = FAIL` and `rts_status = FAIL` at the interface level.

> **Observability note:** There is no `remote_training_failure` variable in IEEE 802.3dj. A remote-side training failure is only indirectly observable: it causes `remote_rx_ready` to remain `false`, which prevents `isl_ready` from being set, which causes the local `max_wait_timer` to expire and sets the local `training_failure = true`. Software cannot distinguish between a local-only failure and a remote-triggered timeout from these attributes alone.

---

## Variable Interaction and Bring-Up Sequence

```
For each lane i:
  1. Training frames exchanged (tx_mode = TRAINING)
  2. local_tf_lock[i]  → true  (frame boundaries found)
  3. remote_tf_lock[i] → true  (peer reports frame lock in status field)
  4. Coefficient update / equalization runs
  5. local_rx_ready[i]  → true  (local adaptation complete)
  6. remote_rx_ready[i] → true  (peer reports ready in status field)
     OR training_failure[i] → true  (max_wait_timer expired)

Across all lanes (interface level):
  7. isl_ready  → true   (local_rx_ready AND remote_rx_ready for ALL lanes)
  8. local_rts  → true   (clock swap completes, NRTS bit set to 0 in all TX frames)
  9. remote_rts → true   (all peer lanes have NRTS=0)
  10. rts_status → OK    (tx_mode = DATA; live traffic)
```

---

## MDIO Register Summary (PMD, Annex 178B Tables 178B–6 and 178B–7)

| IEEE Variable | MDIO Register (lane 0) | MDIO Ref | Control/Status | Granularity |
|---|---|---|---|---|
| `mr_restart` | `1.150.0` | 45.2.1.95 | Control | Per interface |
| `mr_training_enable` | `1.150.1` | 45.2.1.95 | Control | Per interface |
| `max_wait_timer_duration` | `1.1459.15:0` | 45.2.1.168a | Control | Per interface |
| `isl_ready` | `1.1476.8` | 45.2.1.168c | Status | Per interface |
| `rts_status` | `1.1476.14:13` | 45.2.1.168c | Status | Per interface |
| `training_status` | `1.1476.11:10` | 45.2.1.168c | Status | Per interface |
| `local_rts` | `1.1320.10` | 45.2.1.165 | Status | Per interface |
| `remote_rts` | `1.1120.10` | 45.2.1.161 | Status | Per interface |
| `lane_training_status` | `1.1477.1:0` | 45.2.1.168d | Status | Per lane |
| `local_tf_lock` | `1.151.1` (lanes 0–3) | 45.2.1.96 | Status | Per lane |
| `remote_tf_lock` | `1.1420.9` | 45.2.1.167 | Status | Per lane |
| `local_rx_ready` | `1.1420.15` | 45.2.1.167 | Status | Per lane |
| `remote_rx_ready` | `1.1220.15` | 45.2.1.163 | Status | Per lane |
| `training_failure` | `1.151.3` (lanes 0–3) | 45.2.1.96 | Status | Per lane |
| `polarity_correction` | `1.1476.0` | 45.2.1.168c | Status | Per lane |
