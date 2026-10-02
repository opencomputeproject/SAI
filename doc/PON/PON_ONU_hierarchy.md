# PON_ONU Config Hierarchy

## Graphical Form

```
                                  ┌───────────────────────────────┐
                                  │            PON_ONU            │
                                  │  key: onu-name                │
                                  │  leafrefs: (none)             │
                                  └───────────────┬───────────────┘
                                                  │
            ┌───────────────────────┬─────────────┴─────────────┬───────────────────────┐
            │                       │                           │                       │
            ▼                       ▼                           ▼                       ▼
┌──────────────────────┐ ┌──────────────────────┐ ┌──────────────────────┐ ┌──────────────────────┐
│ PON_ONU_OLT_SERVICE  │ │ PON_ONU_SERVICE_     │ │     PON_ONU_UNI      │ │ PON_ONU_FW_BANK_FILE │
│                      │ │     CONFIG_VALUE     │ │                      │ │                      │
│ keys:                │ │ keys:                │ │ keys:                │ │ keys:                │
│   onu-name           │ │   onu-name           │ │   onu-name           │ │   onu-name           │
│   olt-service-id     │ │   cfg-name           │ │   port-id            │ │   bank-id            │
│                      │ │                      │ │                      │ │                      │
│ leafrefs:            │ │ leafrefs:            │ │ leafrefs:            │ │ leafrefs:            │
│   onu-name ──►       │ │   onu-name ──►       │ │   onu-name ──►       │ │   onu-name ──►       │
│     PON_ONU/onu-name │ │     PON_ONU/onu-name │ │     PON_ONU/onu-name │ │     PON_ONU/onu-name │
└──────────┬───────────┘ └──────────────────────┘ └──────────────────────┘ └──────────────────────┘
           │
           ▼
┌───────────────────────────────────────────────┐
│        PON_ONU_OLT_SERVICE_NETWORK            │
│  keys:                                        │
│    onu-name                                   │
│    olt-service-id                             │
│    vlan-id                                    │
│  leafrefs:                                    │
│    onu-name       ──► PON_ONU/onu-name        │
│    olt-service-id ──► PON_ONU_OLT_SERVICE/    │
│                       olt-service-id          │
└───────────────────────────────────────────────┘
```

## Table Form

| Table                        | Keys                              | Leafrefs                                                                           |
| ---------------------------- | --------------------------------- | ---------------------------------------------------------------------------------- |
| PON_ONU                      | onu-name                          | (none)                                                                             |
| PON_ONU_OLT_SERVICE          | onu-name, olt-service-id          | onu-name -> PON_ONU/onu-name                                                       |
| PON_ONU_OLT_SERVICE_NETWORK  | onu-name, olt-service-id, vlan-id | onu-name -> PON_ONU/onu-name; olt-service-id -> PON_ONU_OLT_SERVICE/olt-service-id |
| PON_ONU_SERVICE_CONFIG_VALUE | onu-name, cfg-name                | onu-name -> PON_ONU/onu-name                                                       |
| PON_ONU_UNI                  | onu-name, port-id                 | onu-name -> PON_ONU/onu-name                                                       |
| PON_ONU_FW_BANK_FILE         | onu-name, bank-id                 | onu-name -> PON_ONU/onu-name                                                       |
