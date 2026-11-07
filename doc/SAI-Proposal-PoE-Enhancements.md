PoE Port SAI Enhancements


| Title       | PoE Port SAI Enhancements — Dynamic Allocation & Diagnostics |
| ----------- | ------------------------------------------------------------ |
| Authors     | Daniela Murin, Shira Ezra, Marvell                           |
| Status      | In review                                                    |
| Type        | Standards track                                              |
| Created     | 09/07/2026                                                   |
| SAI-Version | 1.18                                                         |


## Purpose

This document proposes extensions to the base [SAI-Proposal-PoE.md](SAI-Proposal-PoE.md) API in two parts:

- **Part A — Dynamic power allocation**
- **Part B — Diagnostics**

Together, Part A and Part B add the PoE port attributes needed for runtime power programming and operational visibility on top of the base PoE SAI.

**This PR is the proposal only.** Matching definitions belong in `inc/saipoe.h` / `inc/saitypes.h` (and SAI metadata) in a follow-up. Proposed C types are shown below so reviewers can comment on the API shape.

**Perpetual PoE** is **not** a new SAI attribute. Keeping PD power across a control-plane restart is a **warm-boot / NOS** behavior only (see below).

## Overview

The base PoE SAI defines device, PSE, and port objects with admin state, power limits, priority, consumption, and status. That is sufficient for basic enable/disable and static configuration, but not for two common deployment needs:

1. **Runtime power allocation** — The NOS learns what a PD wants (e.g. via LLDP), programs a requested allocation, and reads back what the PSE actually granted.
2. **Operational visibility** — Operators need per-path hardware status.

These extensions add **4 new PoE port attributes** in `saipoe.h` (and supporting types in `saitypes.h`) without changing the existing object model.

## Abbreviations


| Term     | Definition                              |
| -------- | --------------------------------------- |
| PSE      | Power Sourcing Equipment                |
| PD       | Powered Device                          |
| LLDP     | Link Layer Discovery Protocol           |
| MDI      | Media Dependent Interface               |
| AF/AT/BT | 802.3af, 802.3at, 802.3bt power classes |
| MPS      | Maintain Power Signature                |


## New attributes

All new attributes extend `sai_poe_port_attr_t` on `SAI_OBJECT_TYPE_POE_PORT`.

### Dynamic power allocation (Part A)

The NOS sets a dynamic power request in milliwatts and reads back the hardware-reported allocation, plus a configurable negotiation source. The mechanism is **not tied to any single protocol**—the same attributes apply whether the request comes from operator policy, a platform agent, or LLDP.


| Attribute                                    | Access           | Semantics |
| -------------------------------------------- | ---------------- | --------- |
| `SAI_POE_PORT_ATTR_DYNAMIC_POWER_REQUEST`    | `CREATE_AND_SET` | Requested allocation (mW). **Valid only when negotiation source is `DYNAMIC_REQUEST`.** `0` = no dynamic request. |
| `SAI_POE_PORT_ATTR_DYNAMIC_POWER_ALLOCATED`  | Read-only        | Hardware-granted active allocation (mW). Always readable: classification grant when source is `PHYSICAL_CLASSIFICATION` (LLDP publishes this), or what hardware granted after a dynamic request. |
| `SAI_POE_PORT_ATTR_POWER_NEGOTIATION_SOURCE` | `CREATE_AND_SET` | Which mechanism drives negotiation (`sai_poe_port_power_negotiation_source_t`). |


**Contract (dynamic allocation):** intent (request) vs reality (allocated).

#### Power negotiation source

Operator policy for `SAI_POE_PORT_ATTR_POWER_NEGOTIATION_SOURCE`. Selects which mechanism drives power negotiation on the port.

```
typedef enum _sai_poe_port_power_negotiation_source_t
{
    SAI_POE_PORT_POWER_NEGOTIATION_SOURCE_PHYSICAL_CLASSIFICATION,
    SAI_POE_PORT_POWER_NEGOTIATION_SOURCE_DYNAMIC_REQUEST,
} sai_poe_port_power_negotiation_source_t;
```


| Value                     | Meaning |
| ------------------------- | ------- |
| `PHYSICAL_CLASSIFICATION` | IEEE 802.3af/at/bt physical-layer classification determined the outcome |
| `DYNAMIC_REQUEST`         | Dynamic power request determined the outcome (e.g. LLDP, CDP, or operator-programmed allocation) |


### Diagnostics (Part B)

Portable diagnostics across **any** IEEE PoE port (802.3af / 802.3at / 802.3bt).


| Attribute                     | Access    | Semantics                                                             |
| ----------------------------- | --------- | --------------------------------------------------------------------- |
| `SAI_POE_PORT_ATTR_HW_STATUS` | Read-only | Per-path (Alt A / Alt B) hardware status (`sai_poe_port_hw_status_t`) |


#### Hardware status

`SAI_POE_PORT_ATTR_HW_STATUS` returns per-path (IEEE Alt A / Alt B) hardware status in a single read.

```
typedef enum _sai_poe_port_hw_status_code_t
{
    SAI_POE_PORT_HW_STATUS_UNKNOWN,

    SAI_POE_PORT_HW_STATUS_ON,

    SAI_POE_PORT_HW_STATUS_OFF_FAULT,
    SAI_POE_PORT_HW_STATUS_OFF_USER_SETTING,
    SAI_POE_PORT_HW_STATUS_OFF_DETECTION_IN_PROCESS,
    SAI_POE_PORT_HW_STATUS_OFF_NONSTANDARD_DEVICE,

    SAI_POE_PORT_HW_STATUS_OFF_UNDERLOAD,
    SAI_POE_PORT_HW_STATUS_OFF_OVERLOAD,
    SAI_POE_PORT_HW_STATUS_OFF_POWER_DENIED,

    SAI_POE_PORT_HW_STATUS_OFF_OUT_OF_BUDGET,
    SAI_POE_PORT_HW_STATUS_OFF_SHORT,
    SAI_POE_PORT_HW_STATUS_OFF_OVER_TEMP,
    SAI_POE_PORT_HW_STATUS_OFF_UNREACHABLE,

    SAI_POE_PORT_HW_STATUS_OFF_CLASSIFICATION_OVERCURRENT,
    SAI_POE_PORT_HW_STATUS_OFF_MPS_FAULT,

} sai_poe_port_hw_status_code_t;

typedef struct _sai_poe_port_hw_status_t
{
    sai_poe_port_hw_status_code_t hw_status_alt_a;
    sai_poe_port_hw_status_code_t hw_status_alt_b;
} sai_poe_port_hw_status_t;
```


| Code                             | Status | Meaning                                |
| -------------------------------- | ------ | -------------------------------------- |
| `ON`                             | On     | Port is delivering or detected on      |
| `OFF_FAULT`                      | Off    | Hardware fault                         |
| `OFF_USER_SETTING`               | Off    | User setting                           |
| `OFF_DETECTION_IN_PROCESS`       | Off    | Detection in process                   |
| `OFF_NONSTANDARD_DEVICE`         | Off    | Non-standard compliant device          |
| `OFF_UNDERLOAD`                  | Off    | Underload                              |
| `OFF_OVERLOAD`                   | Off    | Overload                               |
| `OFF_POWER_DENIED`               | Off    | Power denied                           |
| `OFF_OUT_OF_BUDGET`              | Off    | Out of power budget                    |
| `OFF_SHORT`                      | Off    | Short condition                        |
| `OFF_OVER_TEMP`                  | Off    | Over temperature at the port or device |
| `OFF_UNREACHABLE`                | Off    | Unreachable                            |
| `OFF_CLASSIFICATION_OVERCURRENT` | Off    | Classification overcurrent             |
| `OFF_MPS_FAULT`                  | Off    | Maintain Power Signature fault or loss |


On single-path ports, only `hw_status_alt_a` is meaningful; `hw_status_alt_b` is `UNKNOWN`.

### Perpetual PoE — warm boot only

Keeping power to connected PDs across a control-plane restart ("perpetual PoE") is supported only on **warm boot** at the platform/NOS layer. Marvell PoE implementations preserve port delivery when the host follows the documented warm-restart sequence.

A **regular reboot or shutdown** does **not** support perpetual PoE — all PoE ports are powered down. 

## End-to-end example: LLDP power upgrade

```
/* 0. After link-up: HW classified the PD. LLDP must advertise that grant. */
attr.id = SAI_POE_PORT_ATTR_POWER_NEGOTIATION_SOURCE;
sai_poe_api->get_poe_port_attribute(poe_port_id, 1, &attr);
/* PHYSICAL_CLASSIFICATION */

attr.id = SAI_POE_PORT_ATTR_DYNAMIC_POWER_ALLOCATED;
sai_poe_api->get_poe_port_attribute(poe_port_id, 1, &attr);
/* e.g. 15400 mW from class — NOS puts this in the LLDP TLV it sends */

/* 1. PD later asks for more via LLDP. Set the power request first
 *    (valid only after source is DYNAMIC_REQUEST). */
attr.id = SAI_POE_PORT_ATTR_DYNAMIC_POWER_REQUEST;
attr.value.u32 = 30000;
sai_poe_api->set_poe_port_attribute(poe_port_id, &attr);

/* 2. Switch negotiation source to dynamic request. */
attr.id = SAI_POE_PORT_ATTR_POWER_NEGOTIATION_SOURCE;
attr.value.s32 = SAI_POE_PORT_POWER_NEGOTIATION_SOURCE_DYNAMIC_REQUEST;
sai_poe_api->set_poe_port_attribute(poe_port_id, &attr);

/* 3. Read back what hardware actually granted. */
attr.id = SAI_POE_PORT_ATTR_DYNAMIC_POWER_ALLOCATED;
sai_poe_api->get_poe_port_attribute(poe_port_id, 1, &attr);
/* e.g. attr.value.u32 == 25000 if budget-limited */

/* 4. Diagnose (Part B). */
attr.id = SAI_POE_PORT_ATTR_HW_STATUS;
sai_poe_api->get_poe_port_attribute(poe_port_id, 1, &attr);
/* e.g. attr.value.hwstatus.hw_status_alt_a == SAI_POE_PORT_HW_STATUS_ON */
```
