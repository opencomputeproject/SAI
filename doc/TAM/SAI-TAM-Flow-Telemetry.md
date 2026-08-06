# SAI TAM per-flow telemetry

---

| Title   | SAI TAM per-flow telemetry (TELEMETRY_TYPE_FLOW)                                    |
| ------- | ----------------------------------------------------------------------------------- |
| Authors | `Aakash Kumar S, Garvith V`                                                         |
| Status  | In review                                                                           |
| Type    | Standards track                                                                     |
| Created | `06/07/2026`                                                                        |

---

## Overview

This proposal defines the attribute block for `SAI_TAM_TELEMETRY_TYPE_FLOW` and, separately, adds a set of switch-level attributes that configure hardware autonomous flow learning. The FLOW telemetry-type enum value already exists in `sai_tam_telemetry_type_t` with the header comment "All the data relevant to a given flow", but `sai_tam_tel_type_attr_t` currently defines no attributes for it — it is a declared category with no operational specification.

Per-flow telemetry is a distinct capability from object-counter telemetry (`SAI_TAM_TELEMETRY_TYPE_COUNTER_SUBSCRIPTION`): the unit is a flow (identified by a classification key), not a named object (identified by OID). This proposal fills in the FLOW attribute block so that SAI TAM can carry per-flow telemetry. Flows can be populated from two independent sources: operator-configured (an ACL entry with an action referencing the enclosing TAM object) and hardware auto-learned (the ASIC autonomously identifies flows from live traffic, when enabled at the switch level). Both sources can be active simultaneously. `COUNTER_SUBSCRIPTION` is not modified.

## Requirements

- Enable per-flow packet/byte streaming through the existing TAM report/collector chain, using IPFIX records.
- Support operator-configured per-flow monitoring via the existing `SAI_ACL_ENTRY_ATTR_ACTION_TAM_OBJECT` mechanism (no new ACL attribute required).
- Support hardware autonomous flow learning as an independent, switch-level capability that operators can enable or disable at runtime.
- Express operator policy at the intent layer (aging time) — not at the implementation-mechanism layer.
- Leave the internal mechanism by which the implementation learns, ages, or reclaims flows unspecified.
- Do not modify `SAI_TAM_TELEMETRY_TYPE_COUNTER_SUBSCRIPTION` or the existing event-side flow telemetry (`SAI_TAM_EVENT_TYPE_FLOW_STATE / _WATCHLIST / _TCPFLAG`).

## Design summary

The proposal decouples two concerns that were previously entangled:

- **Per-flow telemetry configuration** lives on the FLOW tel_type. It says "produce per-flow IPFIX records and wire them through this report chain." The emit cadence is not a FLOW-specific attribute; it reuses the existing `SAI_TAM_REPORT` interval (see Reporting cadence).
- **Hardware autonomous flow learning** lives on the switch object. It is a HW-plane capability, independent of any specific telemetry configuration. When enabled, the implementation autonomously identifies flows from live traffic.

Flows populated into a FLOW tel_type can come from either source, or both, or neither:

- **Operator-configured flows** appear when the operator creates ACL entries with `SAI_ACL_ENTRY_ATTR_ACTION_TAM_OBJECT` pointing at the TAM object that references the FLOW tel_type. This is the existing SAI mechanism; no new ACL attribute is added.
- **HW-learned flows** appear when `SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_ENABLE` is true at the switch level. The switch attributes govern operator policy (aging) for autonomous learning.

The two sources are independent. Neither depends on the other.

## FLOW tel_type attributes

No new tel_type attributes are added. A FLOW tel_type is configured by setting `SAI_TAM_TEL_TYPE_ATTR_TAM_TELEMETRY_TYPE = SAI_TAM_TELEMETRY_TYPE_FLOW` and attaching a report chain, exactly as the existing counter-subscription tel_type does.

## Reporting cadence

Flow telemetry reuses the existing `SAI_TAM_REPORT` object for its emit cadence rather than adding a FLOW-specific interval. The FLOW tel_type's report is created with `SAI_TAM_REPORT_ATTR_TYPE = SAI_TAM_REPORT_TYPE_IPFIX` and `SAI_TAM_REPORT_ATTR_REPORT_MODE = SAI_TAM_REPORT_MODE_BULK`; `SAI_TAM_REPORT_ATTR_REPORT_INTERVAL` (with `SAI_TAM_REPORT_ATTR_REPORT_INTERVAL_UNIT`) then sets the interval at which each active flow's record is emitted. This is the same mechanism the SONiC high-frequency-telemetry path already uses (`poll_interval`, unit USEC), so no new SAI attribute or new orchestration code is required.

## Switch attributes for HW autonomous flow learning

The following attributes are added to `sai_switch_attr_t` in `saiswitch.h`. They govern hardware autonomous flow learning independently of any specific tel_type or TAM object.

```
+    /**
+     * @brief Enable hardware autonomous flow learning
+     *
+     * When true, the implementation autonomously identifies new flows
+     * from live traffic and populates them into any FLOW tel_type that
+     * is configured on the switch. When false, only flows explicitly
+     * bound via ACL entries with SAI_ACL_ENTRY_ATTR_ACTION_TAM_OBJECT
+     * populate FLOW tel_types; the implementation does not
+     * autonomously identify new flows.
+     *
+     * The handling of flows that were already being tracked at the
+     * moment this attribute transitions from true to false is
+     * implementation-defined.
+     *
+     * Default is false. Autonomous learning must be explicitly enabled.
+     *
+     * @type bool
+     * @flags CREATE_AND_SET
+     * @default false
+     */
+    SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_ENABLE,
+
+    /**
+     * @brief Aging time for HW-learned flows (seconds)
+     *
+     * Operator-intent aging: a HW-learned flow idle for more than this
+     * long is eligible for reclaim by the implementation. The reclaim
+     * mechanism (autonomous HW sweep, HW-assisted SW loop, or
+     * otherwise) is deliberately implementation-defined.
+     *
+     * Applies only to flows populated by HW auto-learn. Operator-
+     * configured flows (installed via ACL entries) do not age; the
+     * operator owns their lifetime by adding or removing ACL entries.
+     *
+     * A value of 0 means the implementation chooses its own aging time.
+     *
+     * @type sai_uint32_t
+     * @flags CREATE_AND_SET
+     * @default 0
+     * @validonly SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_ENABLE == true
+     */
+    SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_AGING_TIME_S,
+
+    /**
+     * @brief HW auto-learn failure count
+     *
+     * Count of new-flow learn attempts that were rejected because the
+     * implementation could not admit the flow (capacity exhausted).
+     * Without this counter, operators would have no way to distinguish
+     * "table running fine" from "table chronically saturated, missing
+     * new flows".
+     *
+     * Valid only when SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_ENABLE is true;
+     * otherwise the returned value is 0.
+     *
+     * @type sai_uint64_t
+     * @flags READ_ONLY
+     */
+    SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_FAILURES,
+
+    /**
+     * @brief Overflow policy for HW-learned flows
+     *
+     * Operator intent for how an implementation behaves when it cannot
+     * admit a newly seen flow because per-flow auto-learn capacity is
+     * exhausted. The cause of overflow is implementation-specific; the
+     * policy response is expressed as an observable outcome.
+     *
+     * SAI_TAM_FLOW_OVERFLOW_POLICY_REJECT_NEW is the default and is the
+     * baseline behavior every implementation supports. An implementation
+     * that cannot honor a requested policy returns
+     * SAI_STATUS_ATTR_NOT_SUPPORTED_0 on set.
+     *
+     * @type sai_tam_flow_overflow_policy_t
+     * @flags CREATE_AND_SET
+     * @default SAI_TAM_FLOW_OVERFLOW_POLICY_REJECT_NEW
+     * @validonly SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_ENABLE == true
+     */
+    SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_OVERFLOW_POLICY,
```

## Overflow policy type

The following enum is added to `saitypes.h`. Policies are expressed as observable outcomes; the selection of which existing flow (if any) is displaced, and the internal capacity model, are implementation-defined — keeping the contract vendor-neutral.

```
+/**
+ * @brief TAM per-flow auto-learn overflow policy
+ */
+typedef enum _sai_tam_flow_overflow_policy_t
+{
+    /**
+     * @brief Preserve existing flows; reject the new flow
+     *
+     * Flows already being tracked continue unchanged. Visibility of the
+     * new flow is lost until aging or eviction frees capacity, and
+     * SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_FAILURES is incremented.
+     */
+    SAI_TAM_FLOW_OVERFLOW_POLICY_REJECT_NEW,
+
+    /**
+     * @brief Admit the new flow; displace an existing flow
+     *
+     * The new flow is admitted by evicting one existing tracked flow.
+     * Which flow is chosen (e.g. oldest, least-recently-active) is
+     * implementation-defined. The evicted flow's final record carries
+     * flowEndReason = "lack of resources" (RFC 7011 / RFC 5102).
+     */
+    SAI_TAM_FLOW_OVERFLOW_POLICY_EVICT_EXISTING,
+
+    /*
+     * This enum can be extended with additional values in future
+     * proposals as vendor capabilities converge.
+     */
+} sai_tam_flow_overflow_policy_t;
```

## IPFIX record schema

The FLOW tel_type's IPFIX template is generated by the implementation and retrievable via the existing `SAI_TAM_TEL_TYPE_ATTR_IPFIX_TEMPLATES` attribute. Each record carries, per active flow, per interval:

- Flow key values (one Information Element per key field). The specific fields are determined by the implementation and advertised in the template. For operator-configured flows, the keys correspond to the referencing ACL entry's match fields; for HW-learned flows, the keys correspond to whatever the implementation keys on autonomously.
- Statistics (packet count and byte count at minimum; additional per-flow counters if the implementation supports them and includes them in the advertised template).
- Snapshot timestamp (`observationTimeNanoseconds` — existing RFC 7011 IE).
- Flow-end reason (`flowEndReason` — existing RFC 7011 IE) on the final record for a flow: idle timeout (HW auto-learn aging), operator-initiated end (ACL entry removed), or lack of resources (overflow eviction under `EVICT_EXISTING`).
- IPFIX sequence number (existing RFC 7011 field) so consumers can detect gaps in the stream.

On flow termination, the implementation SHOULD emit one final record with a non-zero `flowEndReason` before releasing the flow's HW state, so that consumers capture the last counter increment between the previous interval snapshot and termination. This is SHOULD rather than MUST because some implementations may not guarantee it under memory-pressure eviction.

## Cleanup semantics

Stated as a semantic contract, not a prescribed mechanism:

- **HW-learned flows — idle reclaim.** A HW-learned flow idle for longer than `SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_AGING_TIME_S` must eventually be reclaimed. The final record's `flowEndReason` indicates idle timeout. The detection interval and mechanism are implementation-defined.
- **HW-learned flows — capacity exhaustion.** When the implementation cannot admit a new flow because per-flow capacity is exhausted, behavior follows `SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_OVERFLOW_POLICY`. Under `REJECT_NEW` (the default), the new flow is not learned, existing flows continue unchanged, and `SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_FAILURES` is incremented so operators can observe the rejection rate. Under `EVICT_EXISTING`, the new flow is admitted by displacing one existing flow (selection implementation-defined); the evicted flow emits a final record with `flowEndReason` = "lack of resources", and `_FAILURES` does not increment.
- **HW-learned flows — auto-learn disable.** When `SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_ENABLE` transitions from true to false, the implementation stops autonomously identifying new flows. The handling of HW-learned flows that were already being tracked at the moment of the transition is implementation-defined.
- **Operator-configured flows — operator-driven.** Flows exist as long as their defining ACL entries exist. Removal of an ACL entry (or clearing its `ACTION_TAM_OBJECT` action) causes the corresponding flow to be terminated and a final record emitted. Operator-configured flows do not age; the operator owns their lifetime.
- **Tel_type destruction.** When the application removes the FLOW tel_type object, all state the implementation was maintaining for that tel_type must be released. Whether a final-record burst precedes destruction is implementation-defined; consumers must not depend on it.

## Sample workflow

The following sequences show HW auto-learn mode and operator-configured mode. In practice, both can be active simultaneously — the two mechanisms are independent and either or both can populate flows into the FLOW tel_type.

### Common setup — TAM report and tel_type

1. Create Transport and Collector objects (existing TAM machinery, not shown).

2. Create a report object (for example, of type `SAI_TAM_REPORT_TYPE_IPFIX`).

```
count = 0;
attr_list[count].id = SAI_TAM_REPORT_ATTR_TYPE;
attr_list[count].value.s32 = SAI_TAM_REPORT_TYPE_IPFIX;
count++;
attr_list[count].id = SAI_TAM_REPORT_ATTR_REPORT_MODE;
attr_list[count].value.s32 = SAI_TAM_REPORT_MODE_BULK;
count++;
attr_list[count].id = SAI_TAM_REPORT_ATTR_REPORT_INTERVAL;
attr_list[count].value.u32 = 500000;    /* emit cadence */
count++;
attr_list[count].id = SAI_TAM_REPORT_ATTR_REPORT_INTERVAL_UNIT;
attr_list[count].value.s32 = SAI_TAM_REPORT_INTERVAL_UNIT_USEC;    /* 500 ms */
count++;

rc = tam_api_p->create_tam_report(&tam_report_id, switch_id, count, attr_list);
```

3. Create a FLOW tel_type. The tel_type is the same regardless of how flows will be populated.

```
count = 0;
attr_list[count].id = SAI_TAM_TEL_TYPE_ATTR_TAM_TELEMETRY_TYPE;
attr_list[count].value.s32 = SAI_TAM_TELEMETRY_TYPE_FLOW;
count++;
attr_list[count].id = SAI_TAM_TEL_TYPE_ATTR_REPORT_ID;
attr_list[count].value.oid = tam_report_id;
count++;

rc = tam_api_p->create_tam_tel_type(&tam_tel_type_id, switch_id, count, attr_list);
```

4. Create the TAM object referencing the tel_type and bind it at switch scope.

```
count = 0;
attr_list[count].id = SAI_TAM_ATTR_TELEMETRY_OBJECTS_LIST;
attr_list[count].value.objlist.count = 1;
attr_list[count].value.objlist.list = (sai_object_id_t *)malloc(1*sizeof(sai_object_id_t));
attr_list[count].value.objlist.list[0] = tam_tel_type_id;
count++;

bind_point_types[0] = SAI_TAM_BIND_POINT_TYPE_SWITCH;
attr_list[count].id = SAI_TAM_ATTR_TAM_BIND_POINT_TYPE_LIST;
attr_list[count].value.s32list.count = 1;
attr_list[count].value.s32list.list = bind_point_types;
count++;

rc = tam_api_p->create_tam(&tam_id, switch_id, count, attr_list);
```

5. Query the IPFIX template the implementation will use. The existing `SAI_TAM_TEL_TYPE_ATTR_IPFIX_TEMPLATES` attribute returns the FLOW-mode record template once the tel_type is sufficiently configured (`REPORT_ID` set).

```
attr.id = SAI_TAM_TEL_TYPE_ATTR_IPFIX_TEMPLATES;
attr.value.u8list.list = template_buf;
attr.value.u8list.count = sizeof(template_buf);

rc = tam_api_p->get_tam_tel_type_attribute(tam_tel_type_id, 1, &attr);
```

### Enable HW autonomous flow learning

6. Enable HW auto-learn at the switch level. This is independent of any FLOW tel_type — it is a switch-wide capability that, when enabled, causes the implementation to autonomously identify flows.

```
count = 0;
attr_list[count].id = SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_ENABLE;
attr_list[count].value.booldata = true;
count++;
attr_list[count].id = SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_AGING_TIME_S;
attr_list[count].value.u32 = 60;        /* 60 s idle timeout */
count++;

for (int i = 0; i < count; i++) {
    rc = switch_api_p->set_switch_attribute(switch_id, &attr_list[i]);
}
```

7. Periodically read the switch-level learn-failure count to observe rejection rate.

```
attr.id = SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_FAILURES;
rc = switch_api_p->get_switch_attribute(switch_id, 1, &attr);
/* attr.value.u64 = cumulative count of rejected learn attempts */
```

8. To disable HW auto-learn later, set the enable attribute to false. The handling of currently-tracked HW-learned flows at that moment is implementation-defined.

```
attr.id = SAI_SWITCH_ATTR_TAM_FLOW_AUTO_LEARN_ENABLE;
attr.value.booldata = false;
rc = switch_api_p->set_switch_attribute(switch_id, &attr);
```

### Operator-configured flows

To monitor specific flows the operator defines (independently of, or in addition to, HW auto-learn):

9. Create an ACL table with `SAI_ACL_ACTION_TYPE_TAM_OBJECT` in its allowed action list.

```
count = 0;
action_list[0] = SAI_ACL_ACTION_TYPE_TAM_OBJECT;
attr_list[count].id = SAI_ACL_TABLE_ATTR_ACL_ACTION_TYPE_LIST;
attr_list[count].value.s32list.list = action_list;
attr_list[count].value.s32list.count = 1;
count++;
/* ... other standard ACL table attributes (stage, bind points, match fields) ... */

rc = acl_api_p->create_acl_table(&acl_table_oid, switch_id, count, attr_list);
```

10. Add ACL entries, each with the `TAM_OBJECT` action pointing at the TAM object. Each such entry defines one monitored flow.

```
count = 0;
attr_list[count].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
attr_list[count].value.oid = acl_table_oid;
count++;
attr_list[count].id = SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP;
attr_list[count].value.aclfield.enable = true;
attr_list[count].value.aclfield.data.ip4 = 0x0a000001;
attr_list[count].value.aclfield.mask.ip4 = 0xffffffff;
count++;
/* ... other match fields defining this flow ... */

attr_list[count].id = SAI_ACL_ENTRY_ATTR_ACTION_TAM_OBJECT;
attr_list[count].value.aclaction.parameter.oid = tam_id;
attr_list[count].value.aclaction.enable = true;
count++;

rc = acl_api_p->create_acl_entry(&acl_entry_oid, switch_id, count, attr_list);
```

The ACL entry's action tells the implementation "monitor this flow under the referenced TAM object's policy." Removing the entry (or clearing its action) ends that flow's monitoring; a final IPFIX record is emitted.

## Composition with existing TAM machinery

- The FLOW tel_type binds through existing TAM machinery (`SAI_TAM_ATTR_TELEMETRY_OBJECTS_LIST` on the parent TAM object, bound at `SAI_TAM_BIND_POINT_TYPE_SWITCH` by default). No new bind-point type is introduced.
- Operator-configured flows use `SAI_ACL_ENTRY_ATTR_ACTION_TAM_OBJECT`, which already exists in `saiacl.h`. No new ACL attribute or action type is introduced by this proposal.
- The FLOW tel_type coexists with the existing event-side flow telemetry (`SAI_TAM_EVENT_TYPE_FLOW_STATE / _WATCHLIST / _TCPFLAG`). The two paths are independent and composable.
- `SAI_TAM_TELEMETRY_TYPE_COUNTER_SUBSCRIPTION` is unchanged; it continues to serve object-counter telemetry (PORT / QUEUE / BUFFER_POOL / INGRESS_PRIORITY_GROUP).