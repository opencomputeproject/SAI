# SAI Enhancement: Query value constraints for numerical attributes
-------------------------------------------------------------------------------
 Title       | API to query value constraints of numerical SAI attributes
-------------|-----------------------------------------------------------------
 Authors     | Ravindranath C K (Marvell)
 Status      | In review
 Type        | Standards track
 Created     | 2026-05-27 - Initial Draft
 SAI-Version | 1.17
-------------------------------------------------------------------------------

## 1.0  Introduction

SAI models a device configuration as a set of `sai_attribute_t` (`id`, `value`) pairs. Many of these attributes are numerical — `sai_uint8_t` ... `sai_uint64_t`, `sai_int8_t` ... `sai_int64_t` — and the value range that a given SAI implementation actually supports is, in practice, a strict subset of the native type's full range. The supported subset can be expressed as a minimum, a maximum, a step (granularity), or a discrete set of allowed values.

Today the SAI user has no consistent way to discover these constraints at run time. For some capabilities, there are read-only attributes such as `SAI_SWITCH_ATTR_PFC_TC_DLD_INTERVAL_RANGE` and `SAI_PORT_ATTR_SUPPORTED_SPEED`. These causes problems like: 1) the attributes are missing for several capabilities, and 2) there would be an explosion in the number of read-only attributes.

As more platforms adopt SAI and their capabilities diverge by use case — datacenter (scale-up vs. scale-out vs. scale-across) vs. enterprise — there is a growing need for the NOS and the user to be able to discover these underlying capabilities in a uniform manner.

This proposal introduces a single, generic SAI API — `sai_query_attribute_value_constraints()` — that returns the implementation-enforced value constraints for any numerical attribute on a given object type or object instance. The detailed type design and API signature are covered in §4 below; this section focuses on the motivation.

## 2.0  Use case

In a NOS such as SONiC, constraint discovery serves two concrete purposes. First, listing the valid range or allowed values for a configuration knob lets operators and automation tools know what the hardware actually accepts before they attempt to apply a change. Second, if those constraints are queried from the SAI implementation at start-up and stored in the `STATE_DB`, the northbound stack — CLI, gNMI, or REST — can validate user input against the hardware-reported limits before the configuration ever reaches `orchagent` or `syncd`. This early rejection avoids the expensive rollback that would otherwise be required when `syncd` returns an error after the object has already been partially committed to other layers of the NOS pipeline.

The constraints to be discovered fall into a few common categories of use-cases, described below.

### 2.1  Constraints that depend only on the platform

The simplest case is an attribute whose legal range is fixed by the underlying silicon, SDK build, or SAI profile, and does not depend on any other attribute of the object being configured.

Example: **`SAI_SWITCH_ATTR_LINK_UP_DEBOUNCE_TIMEOUT`** ([inc/saiswitch.h](../inc/saiswitch.h)).

```c
/**
 * @brief Set Link Up debounce time in micro seconds
 * @type sai_uint32_t
 * @flags CREATE_AND_SET
 * @default 0
 */
SAI_SWITCH_ATTR_LINK_UP_DEBOUNCE_TIMEOUT,
```

The attribute is declared as `sai_uint32_t` (giving a nominal range of 0..2^32-1 microseconds), but the actual minimum, maximum and timer granularity supported by an implementation are bounded by the silicon's debounce-timer subsystem. The values typically have:

- a non-zero minimum (a debounce timer below the HW tick period is meaningless),
- a maximum well below 2^32-1 microseconds, and
- a step that matches the HW tick period.

These three numbers are constant for the lifetime of the switch instance and do not depend on any other attribute. The NOS discovers them with a single type-level call to `sai_query_attribute_value_constraints()` — see §5.1.

### 2.2  Constraints that vary per object instance

The second case is an attribute whose allowed values differ between individual instances of the same object type. The constraint is tied to the specific physical resource underlying each instance; a type-level query cannot return a meaningful answer.

Example: **`SAI_PORT_ATTR_SPEED`** and **`SAI_PORT_ATTR_HALF_DUPLEX_SPEED`** ([inc/saiport.h](../inc/saiport.h)).

The speeds a port supports depend on its physical medium (copper vs. optical), the transceiver inserted, and the SerDes lane complex wired to that port. A 400G optical port may accept `{ 100000, 200000, 400000 }` Mbps for `SAI_PORT_ATTR_SPEED`, while a copper port on a different SerDes block accepts only `{ 10, 100, 1000 }` Mbps. Both are instances of `SAI_OBJECT_TYPE_PORT`, yet their valid value sets for the same attribute are entirely disjoint.

SAI today addresses this pair by introducing dedicated read-only siblings: `SAI_PORT_ATTR_SUPPORTED_SPEED` (`sai_u32_list_t`) for full-duplex speeds and `SAI_PORT_ATTR_SUPPORTED_HALF_DUPLEX_SPEED` (`sai_u32_list_t`) for half-duplex speeds — exactly the companion-attribute pattern discussed in §2.5. The generic API handles this case uniformly: the caller passes the port OID via `object_key` with no sibling-attribute context, and the implementation returns the per-instance allowed value set as a `SAI_ATTRIBUTE_CONSTRAINT_TYPE_ALLOWED_VALUES` entry.

### 2.3  Constraints that depend on other attributes of the same object

The third, more involved case is an attribute whose legal range is itself a function of the values chosen for sibling attributes of the same object. The constraint cannot be expressed as a single static range.

Example: **`SAI_POLICER_ATTR_CIR`** ([inc/saipolicer.h](../inc/saipolicer.h)).

```c
/**
 * @brief Committed information rate BPS/PPS based on
 * #SAI_POLICER_ATTR_METER_TYPE
 *
 * @type sai_uint64_t
 * @flags CREATE_AND_SET
 * @default 0
 */
SAI_POLICER_ATTR_CIR,
```

`CIR` is a `sai_uint64_t` representing the committed information rate of a policer. Its **units**, **minimum**, **maximum**, and on some silicons its **granularity (step)** all depend on the value chosen for the sibling `CREATE_ONLY` attribute `SAI_POLICER_ATTR_METER_TYPE` (`PACKETS` vs `BYTES`):

- For `METER_TYPE == BYTES` the value is bits per second, the minimum is typically the line-rate / counter-resolution of the policer block, and the step is the smallest rate increment the rate compiler can produce (e.g. 8 Kbps).
- For `METER_TYPE == PACKETS` the value is packets per second, both range and granularity are typically very different (e.g. step = 1 pps, much smaller max).

Across SAI implementations the actual min, max and step vary further with the policer block in use, the policer mode (`SAI_POLICER_ATTR_MODE`: `sr_TCM` vs `tr_TCM`), and the meter algorithm. In some deployments the legal CIR values are not even continuous but are quantised to a discrete stepped set.

Answering "what values of CIR will this implementation accept?" therefore cannot be done with a single fixed range — the query has to be conditioned on the sibling attribute values that the NOS is about to use. The API accepts a partial attribute list as a _context_; the adapter computes the constraint as if the object were being configured with those sibling attribute values. This works both for an existing object (pass it via `object_key`) and — crucially — for an object that has not been created yet (pass `object_key = NULL` and let the context attribute list alone describe the proposed configuration). See §5.2 for the API call.

### 2.4  Constraints that depend on the pipeline stage

Some constraints depend on the HW pipeline stage at which the object is applied, but the object **itself does not carry an attribute that conveys that stage**. The stage is established later, by some other binding step. In this case, neither `attr_list` (the sibling-attribute context) nor the existing object's attribute values are sufficient to disambiguate the query.

Example: **`SAI_MIRROR_SESSION_ATTR_TRUNCATE_SIZE`** ([inc/saimirror.h](../inc/saimirror.h)).

```c
/**
 * @brief Truncate size. Truncate mirrored packets to this size to reduce SPAN traffic bandwidth
 *
 * Value 0 to no truncation
 *
 * @type sai_uint16_t
 * @flags CREATE_AND_SET
 * @isvlan false
 * @default 0
 */
SAI_MIRROR_SESSION_ATTR_TRUNCATE_SIZE,
```

`TRUNCATE_SIZE` is a `sai_uint16_t`. The legal range commonly differs between ingress and egress mirror sessions because the truncation is performed by different pipeline blocks (typically the ingress parser/replicator vs the egress packet editor), which have different minimum / maximum / step constraints on the truncation length.

The mirror session object has no attribute that tells the adapter "this session will be used as ingress" or "this session will be used as egress" — the direction is set later, when the session OID is bound to a port via `SAI_PORT_ATTR_INGRESS_MIRROR_SESSION` vs `SAI_PORT_ATTR_EGRESS_MIRROR_SESSION`. Other examples of objects with the same pattern include sFlow sessions, samplepacket objects, and packet-trim profiles.

To support this case, the API takes an optional `sai_object_stage_t stage` parameter. The caller passes `SAI_OBJECT_STAGE_INGRESS` or `SAI_OBJECT_STAGE_EGRESS` to query the constraints for that pipeline stage. `SAI_OBJECT_STAGE_BOTH` (zero) means "stage-unspecified" — the adapter returns stage-agnostic constraints (typically the intersection of the per-stage constraints, i.e. the most restrictive bounds that are safe at every stage). See §5.4 for the API call.

### 2.5  Why not just add more read-only sibling attributes?

SAI today partially addresses constraint discovery by adding, for some attributes, a read-only sibling whose value advertises the legal range or set. Existing examples include:

- `SAI_SWITCH_ATTR_PFC_TC_DLD_INTERVAL_RANGE` ([inc/saiswitch.h](../inc/saiswitch.h)) — companion to `SAI_SWITCH_ATTR_PFC_TC_DLD_INTERVAL`.
- `SAI_PORT_ATTR_SUPPORTED_SPEED` ([inc/saiport.h](../inc/saiport.h)) — companion to `SAI_PORT_ATTR_SPEED`.
- `SAI_PORT_ATTR_PFC_TC_DLD_INTERVAL_RANGE` ([inc/saiport.h](../inc/saiport.h)) — per-port companion to `SAI_PORT_ATTR_PFC_TC_DLD_INTERVAL`.


This bloats the number of attributes of an object type, and there are frequent cases of the read-only attributes being missed in the first place.
A single generic API — registered in the SAI metadata system, supporting type-level queries, instance-specific queries, and conditional constraints driven by a caller-supplied sibling-attribute context — is therefore preferable to extending the existing companion-attribute approach. 
We should introduce read-only sibling attributes only when the supported values cannot be determined by the generic APIs. For example, let us say the supported policer CIR values are a series of ranges instead of single range.

## 3.0  Missing API

SAI today exposes attribute capability through two APIs in [inc/saiobject.h](../inc/saiobject.h):

- `sai_query_attribute_capability()` returns the `{create, set, get}` implementation flags for an attribute.
- `sai_query_attribute_enum_values_capability()` returns the list of enum values supported by the implementation for an `enum` (or `enum list`) attribute.

There is no equivalent for **numerical** attributes. The application has no portable way to ask "what numerical values of this attribute does this implementation actually support?", and there is no way at all to ask the question conditioned on the values that the application intends to use for sibling attributes. This proposal adds the missing API.

## 4.0  SAI Enhancements

The primary goal is to reduce the proliferation of read-only capability attributes for numerical types. Some constraints are inherently too complex or too implementation-specific to fit a generic model — those will still warrant dedicated read-only attributes. This proposal deliberately limits scope to the common cases, keeping the API straightforward to implement and use.

The following constructs are added:


### 4.1  New enum `sai_attribute_constraint_type_t`

Encodes the kind of value constraint an implementation enforces on a numerical attribute. Defined as a freestanding enum so the same set of constraint kinds applies uniformly across all object types and all numerical attribute value types. This allows us to extend this to new types of constraints like unsupported values, powers of 2 etc.

```c
typedef enum _sai_attribute_constraint_type_t
{
    /** Minimum supported value (inclusive). */
    SAI_ATTRIBUTE_CONSTRAINT_TYPE_MIN,

    /** Maximum supported value (inclusive). */
    SAI_ATTRIBUTE_CONSTRAINT_TYPE_MAX,

    /** Granularity (step). Legal values: { min + k * step } intersected with [min, max]. */
    SAI_ATTRIBUTE_CONSTRAINT_TYPE_STEP,

    /** Implementation-specific default (when it differs from the attribute metadata default). */
    SAI_ATTRIBUTE_CONSTRAINT_TYPE_DEFAULT,

    /** Discrete set of allowed values. The value carrier is a list. */
    SAI_ATTRIBUTE_CONSTRAINT_TYPE_ALLOWED_VALUES,

    SAI_ATTRIBUTE_CONSTRAINT_TYPE_END

} sai_attribute_constraint_type_t;
```

### 4.2  New tuple `sai_attribute_constraint_t`

A single constraint entry is a `(kind, value)` tuple. The value reuses `sai_attribute_value_t` so that any future numerical type added to the union is automatically usable here without changes to the constraint structure. Using sai_attribute_value_t to return the actual constraint value allows us to keep the API backward compatible even when new numerical types (e.g. float) are added to `sai_attribute_value_t`.

```c
/**
 * @extraparam const sai_attr_metadata_t *meta
 */
typedef struct _sai_attribute_constraint_t
{
    sai_attribute_constraint_type_t type;

    /** @passparam meta */
    sai_attribute_value_t value;

} sai_attribute_constraint_t;
```

The `value` field is interpreted using the **queried** attribute's `attrvaluetype` (obtained via `sai_metadata_get_attr_metadata()` on the original `object_type` + `attr_id` passed to `sai_query_attribute_value_constraints()`), not the metadata for the constraint-kind enum.

### 4.3  New list `sai_attribute_constraint_list_t`

The constraint entries returned for one attribute query are carried in a list with standard SAI two-pass sizing semantics:

```c
typedef struct _sai_attribute_constraint_list_t
{
    /** In: buffer capacity. Out: actual / required count. */
    uint32_t count;

    sai_attribute_constraint_t *list;

} sai_attribute_constraint_list_t;
```

### 4.4  New API `sai_query_attribute_value_constraints()`

Added to [inc/saiobject.h](../inc/saiobject.h):

```c
sai_status_t sai_query_attribute_value_constraints(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _In_ const sai_object_key_t *object_key,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_object_stage_t stage,
        _In_ sai_attr_id_t attr_id,
        _Inout_ sai_status_t *attr_status,
        _Inout_ sai_attribute_constraint_list_t *constraints);
```

Parameter summary:

| Parameter | Optional? | Set by caller | Returned by callee | Caller-owned memory | Semantics |
| --------- | --------- | ------------- | ------------------ | ------------------- | --------- |
| `switch_id`    | No  | SAI switch OID | — | — | — |
| `object_type`  | No  | SAI object type of the queried attribute | — | — | — |
| `object_key`   | Yes (may be `NULL`) | Pointer to an existing object instance, or `NULL` for object-type-level / not-yet-created queries | — | Caller allocates and frees; not retained by callee | See `object_key` / `attr_list` composition table below |
| `attr_count`   | Yes (may be `0`) | Number of entries in `attr_list` | — | — | See composition table below |
| `attr_list`    | Yes (may be `NULL` when `attr_count == 0`) | Sibling-attribute context array. Entries whose `attr_id` equals the queried `attr_id` are ignored | — | Caller allocates and frees; not retained by callee | See composition table below |
| `stage`        | Yes (pass `SAI_OBJECT_STAGE_BOTH` when not stage-dependent) | Pipeline-stage context | — | — | See `stage` semantics table below |
| `attr_id`      | No  | Numerical attribute id whose constraints are being queried | — | — | MUST refer to an attribute whose `attrvaluetype` is a scalar integer (verify with `sai_metadata_is_numerical_attr_value_type()`, §4.5) |
| `attr_status`  | No (pointer must be non-NULL) | — | The status of the requested `(attr_id, attr_list, stage)` combination, encoded per [inc/saistatus.h](../inc/saistatus.h) | — | Carries the *semantic* result of the query, separate from the API-level return value. See "Return value and `attr_status`" below for the encoding and the consumption rules |
| `constraints`  | No  | `constraints->count` = caller buffer capacity (use `0` for the sizing pass); `constraints->list` = caller-allocated array (or `NULL` for the sizing pass) | `constraints->count` = actual (or required, on `SAI_STATUS_BUFFER_OVERFLOW`) number of entries; `constraints->list[0..count-1]` filled with constraint tuples | Caller allocates and frees `constraints->list`. For `ALLOWED_VALUES` entries, the embedded value-list buffer is also caller-allocated via the same two-pass protocol | Two-pass sizing: first call with `count = 0` / `list = NULL` to learn the required size, allocate, then re-issue. Constraint values are interpreted using the queried attribute's `attrvaluetype` (see §4.2). Only meaningful when `*attr_status == SAI_STATUS_SUCCESS` |

The API is context-aware by design. `object_key`, the sibling-attribute context (`attr_count` / `attr_list`), and `stage` are all optional inputs that together describe the configuration under which the constraints are evaluated.

`object_key` and `attr_list` compose as follows:

| `object_key` | `attr_list` | Semantics |
| ------------ | ----------- | --------- |
| non-NULL | non-empty | Existing object; sibling values from `attr_list` override the corresponding current attribute values for the purpose of this query. Useful when NOS wants to understand whether a given transition (set of multiple attributes) will be supported |
| non-NULL | empty | Existing object; sibling values are taken from the object's current attribute values. |
| NULL | non-empty | Hypothetical not-yet-created object of `object_type` configured with the sibling values from `attr_list`. |
| NULL | empty | Object-type-level query using attribute metadata defaults. Implementations MAY return `SAI_STATUS_NOT_SUPPORTED` when the answer depends on a sibling that has no default. |

`stage` carries an additional pipeline-stage context that cannot be expressed as a sibling attribute on the object itself (see §2.4). It is independent of `object_key` / `attr_list`:

| `stage` | Semantics |
| ------- | --------- |
| `SAI_OBJECT_STAGE_BOTH` (zero) | Stage-unspecified. Implementation returns the stage-agnostic answer (typically the intersection of the per-stage constraints, i.e. the most restrictive bounds that are safe at every stage). This is the value to use when the queried attribute is not stage-dependent. |
| `SAI_OBJECT_STAGE_INGRESS` | Constraints evaluated for the ingress pipeline. |
| `SAI_OBJECT_STAGE_EGRESS` | Constraints evaluated for the egress pipeline. |

Implementations MUST accept `SAI_OBJECT_STAGE_BOTH`. `INGRESS` / `EGRESS` selectors are honoured only when the queried attribute is actually stage-dependent (and otherwise behave identically to `SAI_OBJECT_STAGE_BOTH`).


#### Return value and `attr_status`

The function **return value** is the single gate for whether the call ran and whether `constraints` may be read: anything other than `SAI_STATUS_SUCCESS` or `SAI_STATUS_BUFFER_OVERFLOW` means **do not** consume `constraints` (invalid combination and malformed-input cases are reported on the return value as well, so a caller need not read `attr_status` first to stay safe).

The **`attr_status`** out-parameter duplicates the combination verdict when the combination is invalid (same code as the return value, including `..._0 + i` offsets into `attr_list`), and is the place to read **combination validity alone** when the return value is `SAI_STATUS_BUFFER_OVERFLOW` (then `*attr_status == SAI_STATUS_SUCCESS` because overflow reflects buffer size, not an invalid `(attr_id, attr_list, stage)`). For invalid combinations the implementation sets `constraints->count = 0`.

| Return value | `attr_status` | Consume `constraints`? | Meaning |
| ------------ | ------------- | ------------------------ | ------- |
| `SAI_STATUS_NOT_IMPLEMENTED` | untouched | No | Adapter does not implement this API. |
| `SAI_STATUS_INVALID_PARAMETER` | untouched | No | Malformed fixed inputs (e.g. null `attr_status` / `constraints`, unknown `object_type`, or `attr_id` not numerical per §4.5). |
| `SAI_STATUS_NOT_SUPPORTED` | same as return | No | No queryable constraint answer for this `attr_id` in the caller's context (e.g. phased rollout: query shape not implemented yet; or no platform-specific constraint to return for that shape). |
| `SAI_STATUS_ATTR_NOT_SUPPORTED_0 + i` | same as return | No | Sibling at `attr_list[i]` makes this combination non-queryable; decode with `SAI_STATUS_IS_ATTR_NOT_SUPPORTED()`. |
| `SAI_STATUS_INVALID_ATTRIBUTE_0 + i` / `SAI_STATUS_INVALID_ATTR_VALUE_0 + i` | same as return | No | Sibling at `attr_list[i]` is invalid; decode with `SAI_STATUS_IS_INVALID_ATTRIBUTE()` / `SAI_STATUS_IS_INVALID_ATTR_VALUE()`. |
| `SAI_STATUS_BUFFER_OVERFLOW` | `SAI_STATUS_SUCCESS` | No (yet) | Valid combination; sizing pass—`constraints->count` holds required entry count; re-issue with a large enough buffer. |
| `SAI_STATUS_SUCCESS` | `SAI_STATUS_SUCCESS` | Yes | Data pass complete; `constraints` is authoritative (may be empty: no extra restriction beyond the native type range). |

**Consumption:** If the return value is not `SAI_STATUS_SUCCESS` and not `SAI_STATUS_BUFFER_OVERFLOW`, stop. Otherwise require `*attr_status == SAI_STATUS_SUCCESS`, then run two-pass sizing on `constraints`. Use `attr_status` when you need the combination-valid signal without conflating it with `SAI_STATUS_BUFFER_OVERFLOW`.

#### NOS discovery (no separate capability API)

A NOS discovers what it can query by combining **metadata** with **return codes**: use `sai_metadata_get_attr_metadata()` and `sai_metadata_is_numerical_attr_value_type()` (§4.5) to filter candidate attributes; use `SAI_STATUS_NOT_IMPLEMENTED` / `SAI_STATUS_NOT_SUPPORTED` and the table above to learn whether the adapter or a given attribute supports constraint queries on this platform. There is **no** separate bitmap for which constraint *kinds* (`MIN`, `MAX`, `STEP`, etc.) exist—those appear only as `constraints->list[i].type` on a successful response (possibly after `SAI_STATUS_BUFFER_OVERFLOW` sizing).

#### Support for Phased implementation

A SAI implementation is likely to roll out this API **incrementally**: for example, honour only object-type-level queries first (`object_key == NULL`, `attr_count == 0`, `stage == SAI_OBJECT_STAGE_BOTH`), then add support for existing-object keys, hypothetical create (`attr_list` non-empty with `object_key == NULL`), richer sibling overrides, and ingress/egress stage selectors for certain attributes as and when needed. For an invocation with parameters that are **not yet implemented** (or intentionally not offered) for an otherwise valid `attr_id`, the implementation returns an appropriate **`sai_status_t` on the function return value**—typically `SAI_STATUS_NOT_SUPPORTED` / `SAI_STATUS_NOT_IMPLEMENTED`.

An alternative could be to propose multiple variants of `sai_query_attribute_value_constraints()`, like `sai_query_attribute_value_constraints_object_type()`, `sai_query_attribute_value_constraints_object_key()` etc.
The availability of the attr_status allows to achieve the same without having to create multiple combinations of the SAI API.

### 4.5  New metadata helper `sai_metadata_is_numerical_attr_value_type()`

Added to [meta/saimetadatautils.h](../meta/saimetadatautils.h) and [meta/saimetadatautils.c](../meta/saimetadatautils.c). Returns `true` for `SAI_ATTR_VALUE_TYPE_UINT8` ... `UINT64` and `INT8` ... `INT64`, false otherwise. The helper lets a caller decide whether `sai_query_attribute_value_constraints()` is applicable to a given attribute (by first calling `sai_metadata_get_attr_metadata()` and then this helper on the resulting `attrvaluetype`), and is also used by the metadata sanity checks.

## 5.0  API Example

The examples below exercise the use-case shapes from §2 (platform-only §2.1, sibling-conditioned §2.3, stage-dependent §2.4) plus a discrete **`ALLOWED_VALUES`** list (§5.5), using the standard SAI two-pass sizing protocol (first call with `count = 0` / `list = NULL` to learn the required size, allocate, then re-call to fetch the data). Nested lists use the same pattern twice: once for `constraints->list`, and again for each embedded `u32list` inside an `ALLOWED_VALUES` entry. Every call also passes the `attr_status` channel and checks it before consuming `constraints`, as required by §4.4.

### 5.1  Platform-only constraint, no sibling context

Discover the legal range of `SAI_SWITCH_ATTR_LINK_UP_DEBOUNCE_TIMEOUT` (§2.1):

```c
sai_attribute_constraint_list_t constraints = { .count = 0, .list = NULL };
sai_status_t status;
sai_status_t attr_status;

/* First pass: ask the adapter how many constraint entries it has. */
status = sai_query_attribute_value_constraints(
        switch_id,
        SAI_OBJECT_TYPE_SWITCH,
        NULL,                                       /* object_key  - not needed                  */
        0,                                          /* attr_count  - no sibling context          */
        NULL,                                       /* attr_list   - no sibling context          */
        SAI_OBJECT_STAGE_BOTH,                      /* stage       - not stage-dependent         */
        SAI_SWITCH_ATTR_LINK_UP_DEBOUNCE_TIMEOUT,
        &attr_status,                               /* combination status (see §4.4)             */
        &constraints);

/* A single check on the return value covers API-level errors AND an invalid
   combination (the latter is mirrored onto the return value, see §4.4).
   attr_status carries the identical verdict if finer handling is wanted. */
if (status != SAI_STATUS_SUCCESS && status != SAI_STATUS_BUFFER_OVERFLOW)
    return status;                                 /* NOT_IMPLEMENTED / NOT_SUPPORTED / ...      */

/* status is SAI_STATUS_BUFFER_OVERFLOW and constraints.count now holds the
   required number of entries. */

/* Allocate and re-issue. */
constraints.list = calloc(constraints.count, sizeof(*constraints.list));

status = sai_query_attribute_value_constraints(
        switch_id,
        SAI_OBJECT_TYPE_SWITCH,
        NULL,
        0,
        NULL,
        SAI_OBJECT_STAGE_BOTH,
        SAI_SWITCH_ATTR_LINK_UP_DEBOUNCE_TIMEOUT,
        &attr_status,
        &constraints);

/* Consume. SAI_SWITCH_ATTR_LINK_UP_DEBOUNCE_TIMEOUT is sai_uint32_t,
   so read constraints.list[i].value.u32. */
uint32_t min_us = 0, max_us = 0, step_us = 1;
for (uint32_t i = 0; i < constraints.count; i++)
{
    switch (constraints.list[i].type)
    {
        case SAI_ATTRIBUTE_CONSTRAINT_TYPE_MIN:
            min_us  = constraints.list[i].value.u32;
            break;
        case SAI_ATTRIBUTE_CONSTRAINT_TYPE_MAX:
            max_us  = constraints.list[i].value.u32;
            break;
        case SAI_ATTRIBUTE_CONSTRAINT_TYPE_STEP:
            step_us = constraints.list[i].value.u32;
            break;
        default:
            /* DEFAULT / ALLOWED_VALUES not relevant here; ignore. */
            break;
    }
}

free(constraints.list);
```

### 5.2  Constraint conditioned on a sibling attribute (object not yet created)

Discover the legal range of `SAI_POLICER_ATTR_CIR` for a policer that the NOS is about to instantiate with `METER_TYPE = BYTES` (§2.3). The policer does not yet exist, so `object_key = NULL`; the proposed `METER_TYPE` is supplied as the context. Because the answer is conditioned on `attr_list`, this is the case where `attr_status` may report that the specific sibling combination is not constrainable:

```c
sai_attribute_t context[1];
context[0].id        = SAI_POLICER_ATTR_METER_TYPE;
context[0].value.s32 = SAI_METER_TYPE_BYTES;

sai_attribute_constraint_list_t constraints = { .count = 0, .list = NULL };
sai_status_t status;
sai_status_t attr_status;

/* First pass. */
status = sai_query_attribute_value_constraints(
        switch_id,
        SAI_OBJECT_TYPE_POLICER,
        NULL,                                       /* object_key = NULL  (object not yet created)        */
        1,                                          /* attr_count = 1                                     */
        context,                                    /* attr_list  - the proposed sibling values           */
        SAI_OBJECT_STAGE_BOTH,                      /* stage      - policer CIR is not stage-dependent    */
        SAI_POLICER_ATTR_CIR,
        &attr_status,                               /* combination status (see §4.4)                      */
        &constraints);

/* The return value reflects an invalid combination too. The offset, when
   present, indexes attr_list; attr_status holds the identical code. */
if (status != SAI_STATUS_SUCCESS && status != SAI_STATUS_BUFFER_OVERFLOW)
{
    if (SAI_STATUS_IS_ATTR_NOT_SUPPORTED(status))
    {
        uint32_t idx = (uint32_t)(status - SAI_STATUS_ATTR_NOT_SUPPORTED_0);
        /* CIR is not constrainable for the meter type in context[idx];
           the NOS can fall back to accepting the full native range. */
    }
    return status;
}

/* Allocate and re-issue. */
constraints.list = calloc(constraints.count, sizeof(*constraints.list));

sai_query_attribute_value_constraints(
        switch_id,
        SAI_OBJECT_TYPE_POLICER,
        NULL,
        1,
        context,
        SAI_OBJECT_STAGE_BOTH,
        SAI_POLICER_ATTR_CIR,
        &attr_status,
        &constraints);

/* Consume. SAI_POLICER_ATTR_CIR is sai_uint64_t, so read .value.u64. */
uint64_t cir_min = 0, cir_max = 0, cir_step = 1;
for (uint32_t i = 0; i < constraints.count; i++)
{
    switch (constraints.list[i].type)
    {
        case SAI_ATTRIBUTE_CONSTRAINT_TYPE_MIN:  cir_min  = constraints.list[i].value.u64; break;
        case SAI_ATTRIBUTE_CONSTRAINT_TYPE_MAX:  cir_max  = constraints.list[i].value.u64; break;
        case SAI_ATTRIBUTE_CONSTRAINT_TYPE_STEP: cir_step = constraints.list[i].value.u64; break;
        default: break;
    }
}

free(constraints.list);
```

### 5.3  Same query, but the policer already exists

When the policer already exists (e.g. the NOS is reconfiguring `CIR` on a live policer in response to an intent update), the same query is issued with `object_key` pointing at the existing instance and `attr_count = 0`, `attr_list = NULL`. The implementation then uses the object's current attribute values as the context:

```c
sai_object_key_t object_key;
object_key.key.object_id = existing_policer_oid;

sai_query_attribute_value_constraints(
        switch_id,
        SAI_OBJECT_TYPE_POLICER,
        &object_key,
        0,
        NULL,
        SAI_OBJECT_STAGE_BOTH,
        SAI_POLICER_ATTR_CIR,
        &attr_status,                               /* check before consuming constraints (see §4.4) */
        &constraints);
```

(Note the local-variable name `object_key` — not `key` — as required by §4.4 so the field access reads `object_key.key.object_id`.)

### 5.4  Stage-dependent constraint (mirror session truncate size, egress)

Discover the legal range of `SAI_MIRROR_SESSION_ATTR_TRUNCATE_SIZE` for a mirror session that the NOS is about to instantiate and bind as an **egress** mirror (§2.4). The mirror session has no direction attribute, so the stage is conveyed by the new `stage` parameter; `object_key` is `NULL` because the session does not exist yet, and no sibling-attribute context is needed:

```c
sai_attribute_constraint_list_t constraints = { .count = 0, .list = NULL };
sai_status_t status;
sai_status_t attr_status;

/* First pass. */
status = sai_query_attribute_value_constraints(
        switch_id,
        SAI_OBJECT_TYPE_MIRROR_SESSION,
        NULL,                                       /* object_key = NULL  (session not yet created)       */
        0,                                          /* attr_count = 0     (no sibling context)            */
        NULL,                                       /* attr_list  = NULL                                  */
        SAI_OBJECT_STAGE_EGRESS,                    /* stage      - asking specifically for egress side   */
        SAI_MIRROR_SESSION_ATTR_TRUNCATE_SIZE,
        &attr_status,                               /* combination status (see §4.4)                      */
        &constraints);

/* Single return-value check covers API errors and an invalid combination
   (e.g. egress truncation not constrainable here); attr_status mirrors it. */
if (status != SAI_STATUS_SUCCESS && status != SAI_STATUS_BUFFER_OVERFLOW)
    return status;

/* Allocate and re-issue. */
constraints.list = calloc(constraints.count, sizeof(*constraints.list));

sai_query_attribute_value_constraints(
        switch_id,
        SAI_OBJECT_TYPE_MIRROR_SESSION,
        NULL,
        0,
        NULL,
        SAI_OBJECT_STAGE_EGRESS,
        SAI_MIRROR_SESSION_ATTR_TRUNCATE_SIZE,
        &attr_status,
        &constraints);

/* Consume. SAI_MIRROR_SESSION_ATTR_TRUNCATE_SIZE is sai_uint16_t,
   so read constraints.list[i].value.u16. */
uint16_t trunc_min = 0, trunc_max = 0, trunc_step = 1;
for (uint32_t i = 0; i < constraints.count; i++)
{
    switch (constraints.list[i].type)
    {
        case SAI_ATTRIBUTE_CONSTRAINT_TYPE_MIN:  trunc_min  = constraints.list[i].value.u16; break;
        case SAI_ATTRIBUTE_CONSTRAINT_TYPE_MAX:  trunc_max  = constraints.list[i].value.u16; break;
        case SAI_ATTRIBUTE_CONSTRAINT_TYPE_STEP: trunc_step = constraints.list[i].value.u16; break;
        default: break;
    }
}

free(constraints.list);
```

### 5.5  Discrete allowed values (`ALLOWED_VALUES`)

Let us see how to achieve the functionality of `SAI_PORT_ATTR_SUPPORTED_SPEED` using the proposed APIs. `SAI_PORT_ATTR_SPEED` is a `sai_uint32_t` (Mbps). When the hardware exposes a **finite set** of legal speeds rather than only `MIN` / `MAX` / `STEP`, the adapter may return a constraint entry with `type == SAI_ATTRIBUTE_CONSTRAINT_TYPE_ALLOWED_VALUES`; the speeds are then in `constraints.list[i].value.u32list` (see §4.2). The outer list uses a sizing call; embedded `ALLOWED_VALUES` lists are sized in a **second** call that publishes all required inner lengths at once, then a **third** call fills them.

The snippet below queries an **existing** port (`object_key` set). It allocates an inner `u32list.list` for **every** entry whose `type` is `ALLOWED_VALUES`; the read loop at the end only prints the first such entry as an illustration.

**Sizing (no grow loops):** (1) Call once with `constraints.count == 0` and `constraints.list == NULL` to learn the required outer length (`constraints.count` on `SAI_STATUS_BUFFER_OVERFLOW` or success). (2) `calloc` the outer array, zero it, and call again with inner pointers `NULL` and inner counts `0`. Implementations **SHOULD**, in that second call, publish the required `value.u32list.count` for **every** `ALLOWED_VALUES` element (not only up to the first short buffer) so the NOS can `calloc` each inner buffer in one shot—still with `SAI_STATUS_BUFFER_OVERFLOW` if any inner buffer is missing—then (3) call a third time to fetch data.

```c
sai_object_key_t object_key;
object_key.key.object_id = port_oid;

sai_attribute_constraint_list_t constraints = { .count = 0, .list = NULL };
sai_status_t status;
sai_status_t attr_status;

/* Pass A — required number of sai_attribute_constraint_t entries. */
status = sai_query_attribute_value_constraints(
        switch_id,
        SAI_OBJECT_TYPE_PORT,
        &object_key,
        0,
        NULL,
        SAI_OBJECT_STAGE_BOTH,
        SAI_PORT_ATTR_SPEED,
        &attr_status,
        &constraints);

if (status != SAI_STATUS_SUCCESS && status != SAI_STATUS_BUFFER_OVERFLOW)
    return status;
if (attr_status != SAI_STATUS_SUCCESS)
    return status;

if (constraints.count == 0)
    return status;

constraints.list = calloc(constraints.count, sizeof(*constraints.list));
if (constraints.list == NULL)
    return SAI_STATUS_NO_MEMORY;

memset(constraints.list, 0, constraints.count * sizeof(*constraints.list));

/* Pass B — outer list valid; inner list pointers NULL, inner counts 0.
   Adapter fills each .type and publishes every required u32list.count for
   ALLOWED_VALUES (then typically BUFFER_OVERFLOW until inner lists exist). */
status = sai_query_attribute_value_constraints(
        switch_id,
        SAI_OBJECT_TYPE_PORT,
        &object_key,
        0,
        NULL,
        SAI_OBJECT_STAGE_BOTH,
        SAI_PORT_ATTR_SPEED,
        &attr_status,
        &constraints);

if (status != SAI_STATUS_SUCCESS && status != SAI_STATUS_BUFFER_OVERFLOW)
{
    free(constraints.list);
    return status;
}
if (attr_status != SAI_STATUS_SUCCESS)
{
    free(constraints.list);
    return attr_status;
}

/* Allocate each inner list from the counts published in pass B. */
for (uint32_t i = 0; i < constraints.count; i++)
{
    if (constraints.list[i].type != SAI_ATTRIBUTE_CONSTRAINT_TYPE_ALLOWED_VALUES)
        continue;

    uint32_t n = constraints.list[i].value.u32list.count;

    if (n == 0)
        continue;

    constraints.list[i].value.u32list.list = calloc(n, sizeof(uint32_t));
    if (constraints.list[i].value.u32list.list == NULL)
    {
        for (uint32_t k = 0; k < i; k++)
        {
            if (constraints.list[k].type == SAI_ATTRIBUTE_CONSTRAINT_TYPE_ALLOWED_VALUES)
                free(constraints.list[k].value.u32list.list);
        }
        free(constraints.list);
        return SAI_STATUS_NO_MEMORY;
    }
}

/* Pass C — fetch scalar constraints and fill every ALLOWED_VALUES list. */
status = sai_query_attribute_value_constraints(
        switch_id,
        SAI_OBJECT_TYPE_PORT,
        &object_key,
        0,
        NULL,
        SAI_OBJECT_STAGE_BOTH,
        SAI_PORT_ATTR_SPEED,
        &attr_status,
        &constraints);

if (status != SAI_STATUS_SUCCESS || attr_status != SAI_STATUS_SUCCESS)
{
    for (uint32_t i = 0; i < constraints.count; i++)
    {
        if (constraints.list[i].type == SAI_ATTRIBUTE_CONSTRAINT_TYPE_ALLOWED_VALUES)
            free(constraints.list[i].value.u32list.list);
    }
    free(constraints.list);
    return status;
}

/* Example: use the first ALLOWED_VALUES entry (if any). */
for (uint32_t i = 0; i < constraints.count; i++)
{
    if (constraints.list[i].type != SAI_ATTRIBUTE_CONSTRAINT_TYPE_ALLOWED_VALUES)
        continue;

    for (uint32_t j = 0; j < constraints.list[i].value.u32list.count; j++)
    {
        uint32_t speed_mbps = constraints.list[i].value.u32list.list[j];
        (void)speed_mbps; /* e.g. validate or display */
    }
    break;
}

for (uint32_t i = 0; i < constraints.count; i++)
{
    if (constraints.list[i].type == SAI_ATTRIBUTE_CONSTRAINT_TYPE_ALLOWED_VALUES)
        free(constraints.list[i].value.u32list.list);
}
free(constraints.list);
```

