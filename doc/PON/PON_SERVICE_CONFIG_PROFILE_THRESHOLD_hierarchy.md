# PON_SERVICE_CONFIG_PROFILE — THRESHOLD Group

## Graphical Form

`THRESHOLD_DATA_ONE` and `THRESHOLD_DATA_TWO` are a split pair — `ONE` carries
`threshold-value-1..7`, `TWO` carries `threshold-value-8..14`. They are keyed identically
but are _not_ joined by a leafref. `THRESHOLD_DATA64_BIT` carries all 14 in one table.

```
        ┌──────────────────────────────────────────────────────────────┐
        │  shared key: service-config-profile-name  (plain string)     │
        │  shared key: managed-entity-id            (omci-uint16)      │
        │  no leafrefs anywhere in this group                          │
        └──────────────────────────────┬───────────────────────────────┘
                                       │
              ┌────────────────────────┼────────────────────────┐
              ▼                        ▼                        ▼
┌──────────────────────┐  ┌──────────────────────┐  ┌──────────────────────┐
│ THRESHOLD_DATA_ONE   │  │ THRESHOLD_DATA_TWO   │  │ THRESHOLD_DATA64_BIT │
│  threshold-value-1..7│  │ threshold-value-8..14│  │ threshold-value-1..14│
│  (uint32)            │  │ (uint32)             │  │ (uint64)             │
└──────────┬───────────┘  └──────────┬───────────┘  └──────────────────────┘
           │                         │
           └──── split pair, same key shape, no leafref between them
```

## Table Form

| Table (prefix `PON_SERVICE_CONFIG_PROFILE_`) | Keys                                           | Leafrefs |
| -------------------------------------------- | ---------------------------------------------- | -------- |
| THRESHOLD_DATA_ONE                           | service-config-profile-name, managed-entity-id | (none)   |
| THRESHOLD_DATA_TWO                           | service-config-profile-name, managed-entity-id | (none)   |
| THRESHOLD_DATA64_BIT                         | service-config-profile-name, managed-entity-id | (none)   |
