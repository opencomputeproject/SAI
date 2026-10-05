# PON_OLT_INTF Config Hierarchy

## Graphical Form

```
                          ┌───────────────────────────────────────────────┐
                          │                PON_OLT_INTF                   │
                          │  key: olt-intf-name                           │
                          │  leafref: port-id -> PORT/name                │
                          └───────────────────────┬───────────────────────┘
                                                  │
             ┌────────────────────────────────────┼────────────────────────────────────┐
             │                                    │                                    │
             ▼                                    ▼                                    │
┌────────────────────────────┐   ┌────────────────────────────────────┐                │
│   PON_OLT_INTF_NETWORK     │   │         PON_OLT_INTF_ONU           │                │
│  keys:                     │   │  keys:                             │                │
│    olt-intf-name           │   │    olt-intf-name                   │                │
│    vlan-id                 │   │    onu-serial-number               │                │
│  leafrefs:                 │   │  leafrefs:                         │                │
│    olt-intf-name ─────────►│   │    olt-intf-name ─────────────────►│                │
│      PON_OLT_INTF/         │   │      PON_OLT_INTF/                 │                │
│      olt-intf-name         │   │      olt-intf-name                 │                │
└────────────────────────────┘   └──────────────────┬─────────────────┘                │
                                                    │                                  │
                    ┌───────────────────────────────┼───────────────────────────┐      │
                    │                               │                           │      │
                    ▼                               ▼                           ▼      ▼
┌──────────────────────────────────┐ ┌──────────────────────────────┐ ┌──────────────────────────────────┐
│  PON_OLT_INTF_ONU_OLT_SERVICE    │ │ PON_OLT_INTF_ONU_SERVICE_    │ │ PON_OLT_INTF_ONU_SERVICE_        │
│                                  │ │            TCONT             │ │            GEMPORT               │
│  keys:                           │ │  keys:                       │ │  keys:                           │
│    olt-intf-name                 │ │    olt-intf-name             │ │    olt-intf-name                 │
│    onu-serial-number             │ │    onu-serial-number         │ │    onu-serial-number             │
│    service-port-id               │ │    olt-service-id            │ │    olt-service-id                │
│                                  │ │                              │ │                                  │
│  leafrefs:                       │ │  leafrefs:                   │ │  leafrefs:                       │
│    olt-intf-name ──► PON_OLT_    │ │    olt-intf-name ──► PON_OLT_│ │    olt-intf-name ──► PON_OLT_    │
│      INTF/olt-intf-name          │ │      INTF/olt-intf-name      │ │      INTF/olt-intf-name          │
│    onu-serial-number ──► PON_OLT_│ │    onu-serial-number ──►     │ │    onu-serial-number ──►         │
│      INTF_ONU/onu-serial-number  │ │      PON_OLT_INTF_ONU/       │ │      PON_OLT_INTF_ONU/           │
│                                  │ │      onu-serial-number       │ │      onu-serial-number           │
│                                  │ │                              │ │    tcont-ref ──► PON_OLT_INTF_   │
│                                  │ │                              │ │      ONU_SERVICE_TCONT/          │
│                                  │ │                              │ │      olt-service-id              │
└──────────────────────────────────┘ └──────────────┬───────────────┘ └──────────────────────────────────┘
                                                    │                                  ▲
                                                    └── tcont-ref ─────────────────────┘
                                                        (GEMPORT -> TCONT)
```

## Table Form

| Table                            | Keys                                              | Leafrefs                                                                                                                                                         |
| -------------------------------- | ------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| PON_OLT_INTF                     | olt-intf-name                                     | port-id -> /sonic-port/PORT/PORT_LIST/name                                                                                                                       |
| PON_OLT_INTF_NETWORK             | olt-intf-name, vlan-id                            | olt-intf-name -> PON_OLT_INTF/olt-intf-name                                                                                                                      |
| PON_OLT_INTF_ONU                 | olt-intf-name, onu-serial-number                  | olt-intf-name -> PON_OLT_INTF/olt-intf-name                                                                                                                      |
| PON_OLT_INTF_ONU_OLT_SERVICE     | olt-intf-name, onu-serial-number, service-port-id | olt-intf-name -> PON_OLT_INTF/olt-intf-name; onu-serial-number -> PON_OLT_INTF_ONU/onu-serial-number                                                             |
| PON_OLT_INTF_ONU_SERVICE_TCONT   | olt-intf-name, onu-serial-number, olt-service-id  | olt-intf-name -> PON_OLT_INTF/olt-intf-name; onu-serial-number -> PON_OLT_INTF_ONU/onu-serial-number                                                             |
| PON_OLT_INTF_ONU_SERVICE_GEMPORT | olt-intf-name, onu-serial-number, olt-service-id  | olt-intf-name -> PON_OLT_INTF/olt-intf-name; onu-serial-number -> PON_OLT_INTF_ONU/onu-serial-number; tcont-ref -> PON_OLT_INTF_ONU_SERVICE_TCONT/olt-service-id |
