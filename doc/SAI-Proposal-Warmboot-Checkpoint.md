# SAI Proposal: On-Demand Warmboot Checkpoint and Readiness


| Field   | Value                                                          |
| ------- | -------------------------------------------------------------- |
| Title   | SAI Attributes for On-Demand Warmboot Checkpoint and Readiness |
| Authors | Gokul CG (Marvell)                                             |
| Status  | Draft                                                          |
| Type    | Standards Track                                                |
| Created | 2026-07-29                                                     |
| Updated | 2026-11-16                                                     |


## Problem statement

### Current SAI warmboot flow

The existing SAI warmboot mechanism is designed around a graceful, planned restart sequence:

```
1. Set SAI_SWITCH_ATTR_RESTART_WARM = TRUE   (signal warm restart intent)
2. Call sai_remove_switch()                  (triggers state export)
3. Reboot / restart the SAI process
4. Call sai_create_switch() with
   SAI_SWITCH_ATTR_WARM_RECOVER = TRUE       (triggers state import)
```

This flow works correctly for **planned, graceful restarts** where the control plane has foreknowledge of the restart event and can execute an orderly shutdown.

### Graceful-only state persistence

The state export is triggered exclusively inside `sai_remove_switch()`. **State is only saved if the control plane reaches that step in an orderly manner.**

In practice, control planes fail in ways that prevent orderly shutdown:

- **Process crash** — the SAI process terminates abnormally before `sai_remove_switch()` is called.
- **System crash or kernel panic** — the entire OS crashes without executing any cleanup.
- **Power loss or hardware failure** — the supervisor card loses power or is forcibly removed (OIR).
- **Watchdog-triggered reboot** — a hardware or software watchdog resets the CPU without invoking any software shutdown path.

In all of these cases, the SAI warmboot state file may never be written, leaving the system with no state to recover from.

### Unusable or missing checkpoints

Even when a checkpoint file exists, it may not be safe to recover from. A checkpoint may be truncated or corrupted, written by an incompatible SAI/SDK version, captured against a different hardware or pipeline profile, or simply absent.

The NOS needs the recovery attempt to **fail deterministically with a specific reason** rather than partially import bad state, so it can cleanly fall back to cold boot.

### Warmboot on a different control-plane entity (HA)

Modern high-availability platforms deploy redundant control-plane entities (dual supervisors, redundant route processors, or equivalent):

- A standby control-plane entity runs in parallel, ready to take over.
- On failure of the active entity, the standby assumes control.
- **Recovery (warmboot import) occurs on the standby**, a separate node with its own OS, file system, and memory.
- The standby has no direct access to the active entity's local file system or memory.

The current SAI warmboot design assumes the **same entity** exports and later imports the state. In dual-supervisor HA, the standby must import from a checkpoint replicated by the active supervisor.


## Overview

This document proposes extensions to `sai_switch_attr_t` that make SAI warmboot robust in real deployments, including high-availability (HA) platforms. The attributes are **motivated for external PHY** (for example [HMUX](hmux/HMUX-Overview.md) dual-supervisor switchover). Implementations that do not support them return `SAI_STATUS_NOT_SUPPORTED`.

1. **On-demand checkpoint** — `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT` is a NOS-chosen `sai_uint64_t` save ID. On a running switch, `set(N)` (`N != 0`) persists warmboot state without `sai_remove_switch()`. At warm `sai_create_switch()`, the same attribute is the **expected** blob ID (validate only; it does not take a new snapshot). `get` returns the last **committed** ID (`0` if none).
2. **Checkpoint status** — `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS` reports either persisted-checkpoint health on a **running** switch or the **latched outcome** of the last warm `sai_create_switch()` on this instance (see [Checkpoint status semantics](#checkpoint-status-semantics)); mandatory when profile policy `cold` is supported.

Well-run HA designs should **rarely** hit invalid or missing checkpoints (checkpoint on config change plus sync). Invalid checkpoint handling remains defined for robustness; normal failover paths use checkpoint import on the standby.

### Requirements

1. **Non-destructive save** — state save must be possible without calling `sai_remove_switch()`, leaving the switch in normal forwarding operation.
2. **On-demand save** — the NOS or orchestration layer must be able to initiate a save at any time (periodically, before maintenance, or on a config-change event), supplying a unique non-zero ID for that save.
3. **Recoverable state** — a subsequent `sai_create_switch()` with `SAI_SWITCH_ATTR_WARM_RECOVER=TRUE` must be able to restore from the checkpoint using existing import semantics.
4. **Checkpoint import scope** — import restores state as of the last committed checkpoint. Optionally, non-zero `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT` at warm create (HA standby) verifies the blob ID matches that value (the same ID the HA active used in `set`).
5. **No data-plane impact during checkpoint** — the save must not tear down the switch, reset the forwarding pipeline, or stop forwarding. The checkpoint is a point-in-time snapshot; configuration changes during an in-flight save appear only after the next successful commit.
6. **Deterministic recovery result** — a warm-recovery attempt against an invalid or missing checkpoint must fail (or self-heal per profile policy) with a specific, machine-readable reason, never a partial import.
7. **Non-disruptive warm boot** — `SAI_SWITCH_ATTR_WARM_RECOVER=TRUE` must not trigger a hardware reset or re-init of the forwarding pipeline.
8. **Discoverability** — optional attributes must be detected with `sai_query_attribute_capability()` after a switch object exists; they must not be inferred from operation success alone.
9. **HA roles** — HA **active** / **standby** terminology is defined in [HMUX Overview](hmux/HMUX-Overview.md#terminology-ha-active-and-ha-standby). Replicating the checkpoint blob and ID between supervisors is a platform/NOS responsibility.


## New SAI changes

This proposal adds the following to the SAI API:

| Change | Name |
| ------ | ---- |
| Switch attribute | `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT` |
| Switch attribute | `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS` |
| Enum type | `sai_warm_boot_checkpoint_status_t` |
| Status code | `SAI_STATUS_WARM_BOOT_CHECKPOINT_INVALID` |
| Profile key | `SAI_KEY_WARM_BOOT_ON_INVALID_CHECKPOINT` (`abort` default, or `cold`) |

Two new `sai_switch_attr_t` values are backed by one enum type, one typed `sai_create_switch()` status code, and one optional profile key so HA standby import results are deterministic.


## Explain the new SAI changes

### `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT`

```c
/**
 * @brief On-demand checkpoint: set ID to save, get last committed ID.
 *
 * @type sai_uint64_t
 * @flags CREATE_AND_SET
 * @default 0
 */
SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT,
```

The NOS chooses the save ID and passes it as this attribute value.

| Call | Meaning of `N` |
| ---- | -------------- |
| `set(N)` with `N != 0` on a running switch | Start a checkpoint **tagged** `N`. The ID is stored in the blob. |
| `set(0)` | No-op: does not start, cancel, or change a save; does not change `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS`. |
| `create_switch(..., SAI_SWITCH_ATTR_WARM_RECOVER=TRUE, N)` with `N != 0` | **Do not** take a snapshot. Verify the blob to import has ID `N`. |
| `create` with `N = 0` | Skip ID verification. |
| `get` | Last **committed** ID (`0` if no checkpoint has been committed). Do not return an in-flight `N` while `STATUS` is `IN_PROGRESS`. |

`set()` returns when the operation is **accepted**; poll `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS` until it is not `SAI_WARM_BOOT_CHECKPOINT_STATUS_IN_PROGRESS` to detect completion. Persistence may be synchronous or asynchronous (implementation-defined). There is **no** SAI notification for checkpoint completion. Failed writes must not corrupt the last committed snapshot.

**Uniqueness:** the NOS MUST pass a **new** non-zero `N` for every save. Repeating the current committed ID may be treated as a no-op (`set` of an unchanged `CREATE_AND_SET` value). Uniqueness across retries and HA peers is a NOS responsibility.

### `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS`

```c
/**
 * @brief Checkpoint persistence status or latched warm-create outcome.
 *
 * @type sai_warm_boot_checkpoint_status_t
 * @flags READ_ONLY
 */
SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS,
```

The NOS obtains this value only via `get_switch_attribute()` (poll). This proposal does not define a switch notification callback for checkpoint save or readiness.

`SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT` at default `0` (idle) does **not** imply a particular `STATUS`. `STATUS` is the health of the last committed blob (or latched warm-create outcome), independent of the last `set(0)`.

#### Checkpoint status semantics

`SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS` is one attribute with **two contexts** (not two stored states on one switch):


| Context                    | When                                                                                   | What `get(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS)` reports                                                                                                                                                                                                                                                                                                 |
| -------------------------- | -------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Checkpoint persistence** | Running switch (**HA active** taking checkpoints)                                      | Health of the **last committed** on-disk checkpoint (`SAI_WARM_BOOT_CHECKPOINT_STATUS_VALID`, `SAI_WARM_BOOT_CHECKPOINT_STATUS_IN_PROGRESS`, `SAI_WARM_BOOT_CHECKPOINT_STATUS_NOT_FOUND`, …).                                                                                                                                                                     |
| **Warm create outcome**    | After successful `sai_create_switch(..., SAI_SWITCH_ATTR_WARM_RECOVER=TRUE)` on **this** instance | **Latched** result of that create: `SAI_WARM_BOOT_CHECKPOINT_STATUS_VALID` if checkpoint import was used. With profile policy `cold`, any other enumerator means import was refused and this create used cold fallback; the value is the **cause**. Held until the next successful `set(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT)` with `N != 0` on this instance. With policy `abort`, create fails and there is no switch object (see below). |


**Example (ID mismatch):** The HA active may still see `SAI_WARM_BOOT_CHECKPOINT_STATUS_VALID` for its stored blob while the HA standby’s `sai_create_switch()` with a wrong `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT` returns `SAI_STATUS_WARM_BOOT_CHECKPOINT_INVALID` without creating a partial import.

`SAI_WARM_BOOT_CHECKPOINT_STATUS_IN_PROGRESS` applies to **checkpoint save** on the running switch, not to warm `create_switch()` completion.

#### Enum values by HA role

Enumerators are assigned **0–6 in this order**. With policy `cold`, `STATUS != SAI_WARM_BOOT_CHECKPOINT_STATUS_VALID` means fallback; the enumerator is the cause.


| Value | Group | Enumerator | Typical context |
| ----- | ----- | ---------- | ----------------- |
| 0 | HA active and HA standby | `SAI_WARM_BOOT_CHECKPOINT_STATUS_VALID` | Active: last committed checkpoint healthy. Standby: latched warm create used checkpoint import. |
| 1 | HA active and HA standby | `SAI_WARM_BOOT_CHECKPOINT_STATUS_NOT_FOUND` | Active: no checkpoint on storage. Standby after policy `cold`: no blob to import. |
| 2 | HA active only | `SAI_WARM_BOOT_CHECKPOINT_STATUS_INVALID` | Running switch: snapshot not trusted; take a new checkpoint. |
| 3 | HA active only | `SAI_WARM_BOOT_CHECKPOINT_STATUS_IN_PROGRESS` | Running switch: async checkpoint save. |
| 4 | HA standby (policy `cold`) | `SAI_WARM_BOOT_CHECKPOINT_STATUS_CHECKSUM_MISMATCH` | Import refused: integrity check failed. |
| 5 | HA standby (policy `cold`) | `SAI_WARM_BOOT_CHECKPOINT_STATUS_VERSION_INCOMPATIBLE` | Import refused: version mismatch. |
| 6 | HA standby (policy `cold`) | `SAI_WARM_BOOT_CHECKPOINT_STATUS_PROFILE_MISMATCH` | Import refused: HW/pipeline profile mismatch. |

**Policy `abort`:** `sai_create_switch()` returns `SAI_STATUS_WARM_BOOT_CHECKPOINT_INVALID` for every refuse reason (missing blob, checksum, version, profile, or ID mismatch). No switch object is created, so `get(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS)` is not available. The API error is **not** differentiated by cause. Fine-grained reason is **implementation log only**.

#### Checkpoint completion

- `set(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT, N)` with `N != 0` may complete persistence synchronously or asynchronously.
- SAI does not signal checkpoint save completion through an event or notification attribute. The NOS MUST poll `get(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS)`.
- NOS MUST NOT treat `SUCCESS` from `set` as “checkpoint committed.” Poll until the value is `SAI_WARM_BOOT_CHECKPOINT_STATUS_VALID`. Replicate blob + `N` only after `VALID`.

### `sai_warm_boot_checkpoint_status_t`

```c
/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS
 */
typedef enum _sai_warm_boot_checkpoint_status_t
{
    /** HA active and HA standby */
    SAI_WARM_BOOT_CHECKPOINT_STATUS_VALID = 0,
    SAI_WARM_BOOT_CHECKPOINT_STATUS_NOT_FOUND = 1,

    /** HA active only */
    SAI_WARM_BOOT_CHECKPOINT_STATUS_INVALID = 2,
    SAI_WARM_BOOT_CHECKPOINT_STATUS_IN_PROGRESS = 3,

    /** HA standby only (warm create / import, policy cold) */
    SAI_WARM_BOOT_CHECKPOINT_STATUS_CHECKSUM_MISMATCH = 4,
    SAI_WARM_BOOT_CHECKPOINT_STATUS_VERSION_INCOMPATIBLE = 5,
    SAI_WARM_BOOT_CHECKPOINT_STATUS_PROFILE_MISMATCH = 6,

} sai_warm_boot_checkpoint_status_t;
```

Only `SAI_WARM_BOOT_CHECKPOINT_STATUS_VALID` on the standby after a successful warm create indicates checkpoint-based import succeeded. Any other latched value after policy `cold` means this create used cold fallback and names the cause.

### Checkpoint ID behavior

- The NOS supplies `N` on `set`; SAI stores `N` inside the warmboot checkpoint blob with SAI state.
- **HA active:** `set(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT, N)`, poll until `SAI_WARM_BOOT_CHECKPOINT_STATUS_VALID`, replicate blob + `N` to the standby. `get` after `VALID` returns `N`.
- **HA standby:** `create_switch` with `SAI_SWITCH_ATTR_WARM_RECOVER=TRUE` and the same `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT=N` verifies the replicated blob. `N = 0` skips the check.
- **Mismatch:** non-zero create `N` that does not match the stored checkpoint → `SAI_STATUS_WARM_BOOT_CHECKPOINT_INVALID` (policy `abort`).
- After a successful import, `get(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT)` is the imported ID until this instance later `set`s a new non-zero ID.

### `SAI_STATUS_WARM_BOOT_CHECKPOINT_INVALID`

When `sai_create_switch(..., SAI_SWITCH_ATTR_WARM_RECOVER=TRUE)` is attempted against a checkpoint that is not usable, and profile policy is `abort` (default), the call returns this **single** status instead of importing partial state:

```c
#define SAI_STATUS_WARM_BOOT_CHECKPOINT_INVALID ...
```

The same status is used for missing blob, checksum mismatch, version incompatibility, profile mismatch, and ID mismatch. The create error is **not** split by reason. Implementation **logs** may include the fine-grained cause. No switch object is created, so the NOS cannot `get(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS)`. The NOS typically re-invokes `create_switch` for **cold boot**.

### `SAI_KEY_WARM_BOOT_ON_INVALID_CHECKPOINT`

Because `profile_get_value` is available **before** any switch object exists, the NOS may declare what SAI should do if warm recovery finds an invalid checkpoint:

```
SAI_KEY_BOOT_TYPE                         = 1        (warm)
SAI_KEY_WARM_BOOT_ON_INVALID_CHECKPOINT   = abort | cold   (default: abort)
```


| Policy            | On invalid checkpoint, `create_switch` …                           | Data plane | Returned to NOS                                                                                                                                  |
| ----------------- | ------------------------------------------------------------------ | ---------- | ------------------------------------------------------------------------------------------------------------------------------------------------ |
| `abort` (default) | does not create; returns `SAI_STATUS_WARM_BOOT_CHECKPOINT_INVALID` | untouched  | One error for all refuse reasons; cause is **log only**. NOS typically re-calls **cold** boot or a matching `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT`. |
| `cold`            | falls back to cold boot and reprograms hardware                    | disrupted  | `SAI_STATUS_SUCCESS`; NOS **MUST** `get(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS)`: `VALID` means import succeeded; **any other value** means cold fallback and is the cause. |


Implementations that support policy `cold` **MUST** implement `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS`. If the key is absent or unrecognized, SAI MUST default to `abort`.

### Invalid or missing checkpoint (exception path)

When warm import fails with policy `abort`, `sai_create_switch()` returns `SAI_STATUS_WARM_BOOT_CHECKPOINT_INVALID` (no object; reason only in logs). With policy `cold`, SAI completes cold boot in the same create call (`SAI_STATUS_SUCCESS`); the NOS must `get(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS)` — `VALID` means import, otherwise the latched cause.

### Feature discovery

```c
sai_attr_capability_t cap;
sai_status_t st;

st = sai_query_attribute_capability(switch_id, SAI_OBJECT_TYPE_SWITCH,
        SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT, &cap);
st = sai_query_attribute_capability(switch_id, SAI_OBJECT_TYPE_SWITCH,
        SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS, &cap);
```

There is **no** pre-init switch-attribute query for checkpoint contents; the HA standby relies on the typed `sai_create_switch()` result.


## Interaction with existing attributes

| Existing attribute             | Interaction                                                                                                      |
| ------------------------------ | ---------------------------------------------------------------------------------------------------------------- |
| `SAI_SWITCH_ATTR_RESTART_WARM` | Independent. `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT` does not imply a planned restart.                                          |
| `SAI_SWITCH_ATTR_WARM_RECOVER` | Unchanged for checkpoint recovery on the HA standby. At create, `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT` is expected blob ID only. |
| `SAI_KEY_BOOT_TYPE`            | Coexists. `SAI_KEY_WARM_BOOT_ON_INVALID_CHECKPOINT` refines warm-boot behavior when the checkpoint is unusable.  |
| `SAI_SWITCH_ATTR_RESTART_TYPE` | Unchanged. New checkpoint attributes do not alter restart-type semantics.                                        |

**Backward compatibility:** Existing warmboot (`SAI_SWITCH_ATTR_RESTART_WARM`, `SAI_SWITCH_ATTR_WARM_RECOVER`, `sai_remove_switch()` export) is unchanged. New attributes are optional extensions; callers that do not use them see no behavior change. Unsupported attributes return `SAI_STATUS_NOT_SUPPORTED`.


## Use cases

### Dual-supervisor HMUX (checkpoint sync)

1. HA active: `set(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT, N)` on configuration change (`N` unique, non-zero); poll `SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS` until `SAI_WARM_BOOT_CHECKPOINT_STATUS_VALID`.
2. Platform replicates checkpoint blob and `N` to the HA standby.
3. HA standby: `sai_create_switch(SAI_SWITCH_ATTR_WARM_RECOVER=TRUE, SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT=N)` imports the snapshot; on failure, cold boot per profile policy.

### Problem coverage


| Problem | How the new attributes address it |
| ------- | ----------------------------------- |
| State lost on ungraceful failure | Periodic or event-driven checkpoint; recover from last good snapshot. |
| State only saved at `sai_remove_switch()` | Checkpoint decoupled from switch removal. |
| HA active unsure checkpoint is healthy | `get(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS)` on the running switch. |
| Bad checkpoint on HA standby | Policy `abort`: `SAI_STATUS_WARM_BOOT_CHECKPOINT_INVALID` (reason in logs only). Policy `cold`: `SUCCESS` and `get(STATUS) != VALID` (value is the cause). No partial import. |
| Warmboot on a different supervisor | Replicate checkpoint from active; standby warm import. |
| HA standby without shared FS | Blob + NOS save ID `N` from `set(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT)`; same attribute at standby create. |


## NOS workflow

### Roles and mechanisms


| Role           | Mechanism                                                                                                           | Purpose                                     |
| -------------- | ------------------------------------------------------------------------------------------------------------------- | ------------------------------------------- |
| **HA active**  | `set(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT, N)`, `get(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS)` on config change | Keep checkpoint current; sync blob + `N`    |
| **HA standby** | `create_switch(SAI_SWITCH_ATTR_WARM_RECOVER=TRUE, SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT=N)` | Warm import; `abort` → one error (log has cause); `cold` → `get(STATUS) != VALID` is fallback + cause |


### Boot decision flow

```
HA active (running switch):
    on NOS configuration change:
        set(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT = N)   /* N unique, != 0 */
        poll SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS until not SAI_WARM_BOOT_CHECKPOINT_STATUS_IN_PROGRESS
        expect SAI_WARM_BOOT_CHECKPOINT_STATUS_VALID before handoff/sync
        replicate blob + N to standby

HA standby (warm import):
    rc = create_switch(SAI_SWITCH_ATTR_WARM_RECOVER=TRUE, SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT=N)
    if rc == SAI_STATUS_WARM_BOOT_CHECKPOINT_INVALID:
        /* abort: no switch object; API error is not split by reason;
           fine-grained cause is implementation log only */
        create_switch(... cold ...)
    else if rc == SUCCESS:
        get(SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS)
        /* VALID = checkpoint imported; any other value = policy cold fallback, value is the cause */
```

### Code examples

**Trigger a checkpoint**

```c
sai_attribute_t attr;
attr.id        = SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT;
attr.value.u64 = 1001;   /* NOS-chosen unique ID; 0 is a no-op */
sai_switch_api->set_switch_attribute(switch_id, &attr);
```

**Readiness on HA active**

```c
sai_attribute_t attr;
attr.id = SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS;
sai_switch_api->get_switch_attribute(switch_id, 1, &attr);

if (attr.value.s32 == SAI_WARM_BOOT_CHECKPOINT_STATUS_VALID) {
    /* proceed with warm restart / handoff; replicate blob + N */
}
```

**Standby warm import**

```c
/* Profile: SAI_KEY_BOOT_TYPE = "1"
 *         SAI_KEY_WARM_BOOT_ON_INVALID_CHECKPOINT = "abort" (default) or "cold"
 *
 * attrs_warm includes SAI_SWITCH_ATTR_WARM_RECOVER=true
 * and SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT=N (0 skips ID check)
 */
sai_status_t rc = sai_switch_api->create_switch(&switch_id, n_attrs, attrs_warm);

if (rc == SAI_STATUS_WARM_BOOT_CHECKPOINT_INVALID) {
    /* abort: no switch_id; reason is log only (not encoded in rc) */
    sai_switch_api->create_switch(&switch_id, n_attrs, attrs_cold);
} else if (rc == SAI_STATUS_SUCCESS) {
    sai_attribute_t st;
    st.id = SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT_STATUS;
    sai_switch_api->get_switch_attribute(switch_id, 1, &st);
    /* VALID = imported; else policy cold, st.value.s32 is the cause */
}
```

**Graceful maintenance (unchanged)**

```c
attr.id = SAI_SWITCH_ATTR_RESTART_WARM;
attr.value.booldata = true;
sai_switch_api->set_switch_attribute(switch_id, &attr);
sai_switch_api->remove_switch(switch_id);
/* reboot */
sai_switch_api->create_switch(&switch_id, n_attrs, attrs_with_warm_recover);
```

**HMUX checkpoint sync**

```c
sai_attribute_t ckpt;
ckpt.id        = SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT;
ckpt.value.u64 = 1001;
sai_switch_api->set_switch_attribute(active_switch_id, &ckpt);
/* poll STATUS until VALID; replicate blob + 1001 */

attrs_warm[k].id = SAI_SWITCH_ATTR_WARM_RECOVER;
attrs_warm[k].value.booldata = true;
attrs_warm[k+1].id = SAI_SWITCH_ATTR_WARM_BOOT_CHECKPOINT;
attrs_warm[k+1].value.u64 = 1001;   /* expected blob ID; not a new save */
sai_status_t rc = sai_switch_api->create_switch(&switch_id, n_attrs, attrs_warm);
```
