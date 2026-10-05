# PON_ONU_TEMPLATE Config Hierarchy

## Graphical Form

```
                                  ┌───────────────────────────────────────┐
                                  │         PON_ONU_TEMPLATE_ONU          │
                                  │  key: onu-template-name               │
                                  │  leafrefs: (none)                     │
                                  └───────────────────┬───────────────────┘
                                                      │
            ┌───────────────────────┬─────────────────┴─────┬───────────────────────┐
            │                       │                       │                       │
            ▼                       ▼                       ▼                       ▼
┌──────────────────────┐ ┌──────────────────────┐ ┌──────────────────────┐ ┌──────────────────────┐
│ PON_ONU_TEMPLATE_ONU_│ │ PON_ONU_TEMPLATE_    │ │ PON_ONU_TEMPLATE_    │ │ PON_ONU_TEMPLATE_UNI │
│     FW_BANK_FILE     │ │     OLT_SERVICE      │ │ SERVICE_CONFIG_VALUE │ │                      │
│                      │ │                      │ │                      │ │                      │
│ keys:                │ │ keys:                │ │ keys:                │ │ keys:                │
│   onu-template-name  │ │   onu-template-name  │ │   onu-template-name  │ │   onu-template-name  │
│   bank-id            │ │   olt-service-id     │ │   cfg-name           │ │   port-id            │
│                      │ │                      │ │                      │ │                      │
│ leafrefs:            │ │ leafrefs:            │ │ leafrefs:            │ │ leafrefs:            │
│   onu-template-name  │ │   (none - plain      │ │   (none - plain      │ │   (none - plain      │
│     ──► PON_ONU_     │ │    string key)       │ │    string key)       │ │    string key)       │
│     TEMPLATE_ONU/    │ │                      │ │                      │ │                      │
│     onu-template-name│ │                      │ │                      │ │                      │
└──────────────────────┘ └──────────┬───────────┘ └──────────────────────┘ └──────────────────────┘
                                    │
                                    ▼
                    ┌───────────────────────────────────────────────┐
                    │   PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK        │
                    │  keys:                                        │
                    │    onu-template-name                          │
                    │    olt-service-id                             │
                    │    vlan-id                                    │
                    │  leafrefs:                                    │
                    │    olt-service-id ──► PON_ONU_TEMPLATE_       │
                    │                       OLT_SERVICE/            │
                    │                       olt-service-id          │
                    └───────────────────────────────────────────────┘
```

## Table Form

| Table                                 | Keys                                       | Leafrefs                                                      |
| ------------------------------------- | ------------------------------------------ | ------------------------------------------------------------- |
| PON_ONU_TEMPLATE_ONU                  | onu-template-name                          | (none)                                                        |
| PON_ONU_TEMPLATE_ONU_FW_BANK_FILE     | onu-template-name, bank-id                 | onu-template-name -> PON_ONU_TEMPLATE_ONU/onu-template-name   |
| PON_ONU_TEMPLATE_OLT_SERVICE          | onu-template-name, olt-service-id          | (none)                                                        |
| PON_ONU_TEMPLATE_OLT_SERVICE_NETWORK  | onu-template-name, olt-service-id, vlan-id | olt-service-id -> PON_ONU_TEMPLATE_OLT_SERVICE/olt-service-id |
| PON_ONU_TEMPLATE_SERVICE_CONFIG_VALUE | onu-template-name, cfg-name                | (none)                                                        |
| PON_ONU_TEMPLATE_UNI                  | onu-template-name, port-id                 | (none)                                                        |

Note: only `PON_ONU_TEMPLATE_ONU_FW_BANK_FILE` declares `onu-template-name` as a leafref; the
other child tables use a plain `string` key.
