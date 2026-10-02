# PON_CONTROLLER Hierarchy

## Graphical Form

```
CONFIG (rw)
                    ┌───────────────────────────────────────┐
                    │            PON_CONTROLLER             │
                    │  key: controller-name                 │
                    │  leafrefs: (none)                     │
                    │  (no rw children)                     │
                    └───────────────────────────────────────┘

STATE (ro)
                    ┌───────────────────────────────────────┐
                    │         PON_CONTROLLER_STATE          │
                    │  key: name  (plain string)            │
                    │  leafrefs: (none)                     │
                    └───────────────────────────────────────┘
                                     ·
                       (no leafref back to PON_CONTROLLER
                        or PON_CONTROLLER_STATE — the ro
                        tables below repeat the key as a
                        plain string)
                                     ·
        ┌────────────────────────────┼────────────────────────────┐
        ▼                            ▼                            │
┌──────────────────────────┐ ┌──────────────────────────────┐     │
│ PON_CONTROLLER_ONU_FW_   │ │ PON_CONTROLLER_SYSTEM_       │     │
│   UPGRADE_STATE          │ │   STATUS_OLT                 │     │
│ keys:                    │ │ keys:                        │     │
│   name                   │ │   controller-name            │     │
│   onu-id                 │ │   mac-address                │     │
│ leafrefs: (none)         │ │ leafrefs: (none)             │     │
└──────────────────────────┘ └──────────────┬───────────────┘     │
                                            │ mac-address         │
                                            │ (REAL leafref)      │
                                            ▼                     │
                        ┌──────────────────────────────────────┐  │
                        │ PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU │◄─┘
                        │ keys:                                │
                        │   name                               │
                        │   mac-address                        │
                        │   onu-serial-number                  │
                        │ leafrefs:                            │
                        │   mac-address ──► PON_CONTROLLER_    │
                        │     SYSTEM_STATUS_OLT/mac-address    │
                        └──────────────────────────────────────┘
```

## Table Form — Config (rw)

| Table          | Keys            | Leafrefs |
| -------------- | --------------- | -------- |
| PON_CONTROLLER | controller-name | (none)   |

## Table Form — State (ro)

| Table                                | Keys                                 | Leafrefs                                                    |
| ------------------------------------ | ------------------------------------ | ----------------------------------------------------------- |
| PON_CONTROLLER_STATE                 | name                                 | (none)                                                      |
| PON_CONTROLLER_ONU_FW_UPGRADE_STATE  | name, onu-id                         | (none)                                                      |
| PON_CONTROLLER_SYSTEM_STATUS_OLT     | controller-name, mac-address         | (none)                                                      |
| PON_CONTROLLER_SYSTEM_STATUS_OLT_ONU | name, mac-address, onu-serial-number | mac-address -> PON_CONTROLLER_SYSTEM_STATUS_OLT/mac-address |

Note: `PON_SLA_PROFILE_CONTROLLER` is **not** a `PON_CONTROLLER` child despite the name — its
`name` key is a leafref to `PON_SLA_PROFILE/sla-profile-name`.
