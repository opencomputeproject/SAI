# [SAI] Mirror Session Guarantee Rate

------------------------------------------------------------
| Title       | Mirror session guarantee sample rate (pps) |
|-------------|--------------------------------------------|
| Autors      | Milos Moskovljevic, Cisco                  |
| Status      | In review                                  |
| Type        | Standards track                            |
| Created     | 2026-05-07                                 |
| SAI-Version | TBD                                        |
------------------------------------------------------------

## 1. Overview

This document describes `SAI_MIRROR_SESSION_ATTR_GUARANTEE_RATE`, a new mirror session attribute that works together with the existing `SAI_MIRROR_SESSION_ATTR_SAMPLE_RATE`.


| Attribute                                | Domain                  | Role                                                                                                                                              |
| ---------------------------------------- | ----------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------- |
| `SAI_MIRROR_SESSION_ATTR_SAMPLE_RATE`    | Statistical ratio (1:N) | Mirror 1 out of every `sample_rate` packets (0 = no sampling, 1 = every packet).                                                                  |
| `SAI_MIRROR_SESSION_ATTR_GUARANTEE_RATE` | Guaranteed rate (pps)   | When > 0, the first `guarantee_rate` packets per second are always sampled; traffic above that rate is statistically sampled using `SAMPLE_RATE`. |


When `GUARANTEE_RATE` is 0, the guarantee feature is disabled and only `SAMPLE_RATE` applies.

### Sampling formula

Let `G` = `GUARANTEE_RATE`, `N` = `SAMPLE_RATE`, and `R` = observed mirror session packet rate (pps).


| Condition           | Sampled packets per second                     |
| ------------------- | ---------------------------------------------- |
| `G = 0`             | Per `SAMPLE_RATE` only (see interaction table) |
| `G > 0` and `R ≤ G` | `R` (every packet — 100% sampling)             |
| `G > 0` and `R > G` | `G + (R − G) / N`                              |


The excess portion `(R − G)` is subject to statistical 1:N sampling, not the full rate `R`.

---

## 2. Motivation

For sFlow-style mirroring, `SAI_MIRROR_SESSION_ATTR_SAMPLE_RATE` provides statistical sampling (1 out of N packets). That model works well when traffic volume is high.

The requirement this attribute addresses is different: **we want a guaranteed number of samples per second at low rates**, without losing visibility on the first packets when traffic grows. Operations and security often need **every packet** sampled while the rate is low; once traffic exceeds a configured pps limit, only the **excess** traffic should be statistically thinned.

`GUARANTEE_RATE` defines how many packets per second are **always** sampled. `SAMPLE_RATE` applies only to packets **above** that guaranteed volume — not to the entire stream once the threshold is crossed.

---

## 3. Rate scope: mirror session level

The guarantee rate rule is evaluated **per mirror session**, not per port or per flow.

If the same mirror session is attached to multiple ports (or multiple mirror bindings share that session), **packet rates from all contributing sources are aggregated** when applying the formula.

Example: `GUARANTEE_RATE = 100`, `SAMPLE_RATE = 10`, session bound to three ports at 40, 35, and 30 pps (aggregate **105 pps**).

- Guaranteed portion: **100** packets/s (all sampled).
- Excess: **5** packets/s → statistically sampled at 1:10 → **5/10** packets/s.
- **Total sampled:** `100 + 5/10` packets/s.

---

## 4. Rate measurement (implementation-defined)

How the observed mirror session rate is measured — and how the transition between guaranteed sampling and statistical sampling on excess traffic is enforced — is **implementation-defined (vendor-specific)**. This document defines the **intended sampling outcome**, not the internal rate-measurement mechanism.

Implementations may use any suitable method (e.g. token bucket, hardware policers, or other vendor-specific logic) as long as the external behavior is consistent with the semantics described here.

### Illustrative example: token bucket

If an implementation chooses a **token bucket** for rate measurement, the behavior described in this proposal can be realized as follows:

- Bucket refill rate is `GUARANTEE_RATE` (tokens per second, 1 token per packet).
- While tokens are available, packets are sampled (guaranteed region).
- When tokens are depleted, excess packets are statistically sampled per `SAMPLE_RATE`.
- As tokens refill, newly arriving packets up to the guarantee budget are sampled again.

Rate is evaluated at packet arrival time based on token availability, with aggregate traffic at mirror-session scope.

This is **one possible implementation**, not a requirement of the API.

---

## 5. Interaction semantics


| `GUARANTEE_RATE` | `SAMPLE_RATE` | `R ≤ G`            | `R > G`                          |
| ---------------- | ------------- | ------------------ | -------------------------------- |
| 0                | 0             | No sampling        | No sampling                      |
| 0                | 1             | Every packet       | Every packet                     |
| 0                | N (≥ 2)       | `R / N`            | `R / N`                          |
| G (> 0)          | 0             | Every packet (`R`) | `G` (excess not sampled)         |
| G (> 0)          | 1             | Every packet (`R`) | `G + (R − G) / 1` = every packet |
| G (> 0)          | N (≥ 2)       | Every packet (`R`) | `G + (R − G) / N`                |


When `GUARANTEE_RATE > 0`, `SAMPLE_RATE` applies only to the **excess** `(R − G)`, not to the full rate `R`.

---

## 6. Worked examples

### Example 1 — below guarantee rate

**Configuration:** `GUARANTEE_RATE = 100`, `SAMPLE_RATE = 1000`
**Observed rate:** `R = 80` pps

`R ≤ G` → every packet is sampled.

**Result:** **80** sampled packets per second.

---

### Example 2 — above guarantee rate (large sample ratio)

**Configuration:** `GUARANTEE_RATE = 100`, `SAMPLE_RATE = 1000`
**Observed rate:** `R = 150` pps

- Guaranteed: **100** packets/s (all sampled).
- Excess: `150 − 100 = 50` packets/s → 1:1000 statistical sampling → **50/1000** packets/s.

**Result:** `100 + 50/1000` sampled packets per second.

---

### Example 3 — above guarantee rate (small sample ratio)

**Configuration:** `GUARANTEE_RATE = 100`, `SAMPLE_RATE = 10`
**Observed rate:** `R = 101` pps

- Guaranteed: **100** packets/s (all sampled).
- Excess: `101 − 100 = 1` packet/s → 1:10 statistical sampling → **1/10** packets/s.

**Result:** `100 + 1/10` sampled packets per second.

---

## 7. API reference

Defined in `saimirror.h`:

```c
/**
 * @brief Guarantee sample rate in packets per second (pps)
 *
 * While guarantee rate is not breached, all packets will be sampled (100% sampling).
 * Once breached, packets will be sampled based on the mirror statistics (fallback to
 * statistical sampling based on SAI_MIRROR_SESSION_ATTR_SAMPLE_RATE).
 *
 * This is expected to enable guaranteed sampling for very low pps flows.
 * A value of 0 disables the guarantee rate feature.
 *
 * @type sai_uint64_t
 * @flags CREATE_AND_SET
 * @default 0
 */
SAI_MIRROR_SESSION_ATTR_GUARANTEE_RATE,
```

Support for this attribute per mirror session type is **implementation-defined**.
