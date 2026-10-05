# PON_SERVICE_CONFIG_PROFILE — PERFORMANCE MONITORING (PM) Group

## Graphical Form

Every table in this group is a flat sibling with the identical 2-key shape and no leafrefs.
They differ only in the payload leaf they point at via `threshold-data-*-id`, which is a
plain `omci-uint16`, not a leafref.

```
        ┌──────────────────────────────────────────────────────────────┐
        │  shared key: service-config-profile-name  (plain string)     │
        │  shared key: managed-entity-id            (omci-uint16)      │
        │  no leafrefs anywhere in this group — 20 flat siblings       │
        └──────────────────────────────┬───────────────────────────────┘
                                       │
    ┌────────────────┬─────────────────┼─────────────────┬────────────────┐
    ▼                ▼                 ▼                 ▼                ▼
┌──────────┐  ┌──────────────┐  ┌──────────────┐  ┌─────────────┐ ┌─────────────┐
│ FEC / TC │  │  ETHERNET    │  │ ETH FRAME    │  │  GEM / GAL  │ │  MAC BRIDGE │
├──────────┤  ├──────────────┤  ├──────────────┤  ├─────────────┤ ├─────────────┤
│ENHANCED_ │  │ETHERNET_     │  │ETHERNET_     │  │GAL_ETHERNET_│ │MAC_BRIDGE_  │
│FEC_PM_   │  │PERF_MON_     │  │FRAME_        │  │PERF_MON_    │ │PERF_MON_    │
│HIST_DATA │  │HIST_DATA     │  │EXTENDED_PM   │  │HIST_DATA    │ │HIST_DATA    │
│          │  │              │  │              │  │             │ │             │
│ENHANCED_ │  │ETHERNET_     │  │ETHERNET_     │  │GEM_PORT_    │ │MAC_BRIDGE_  │
│TC_PERF_  │  │PERF_MON_     │  │FRAME_        │  │NETWORK_CTP_ │ │PORT_PERF_   │
│MON_HIST_ │  │HIST_DATA3    │  │EXTENDED_     │  │PERF_MON_    │ │MON_HIST_    │
│DATA      │  │              │  │PM64_BIT      │  │HIST_DATA    │ │DATA         │
│          │  │              │  │              │  │             │ │             │
│FEC_PERF_ │  │              │  │ETHERNET_     │  │             │ │             │
│MON_HIST_ │  │              │  │FRAME_PERF_   │  │             │ │             │
│DATA      │  │              │  │MON_HIST_     │  │             │ │             │
│          │  │              │  │DATA_UPSTREAM │  │             │ │             │
│          │  │              │  │              │  │             │ │             │
│          │  │              │  │ETHERNET_     │  │             │ │             │
│          │  │              │  │FRAME_PERF_   │  │             │ │             │
│          │  │              │  │MON_HIST_DATA_│  │             │ │             │
│          │  │              │  │DOWNSTREAM    │  │             │ │             │
└──────────┘  └──────────────┘  └──────────────┘  └─────────────┘ └─────────────┘

    ┌────────────────┬─────────────────┬─────────────────┐
    ▼                ▼                 ▼                 ▼
┌──────────────┐ ┌──────────────┐ ┌──────────────┐ ┌────────────────────┐
│   HOST / IP  │ │   SERIAL     │ │  ONU / OPER  │ │      XG-PON        │
├──────────────┤ ├──────────────┤ ├──────────────┤ ├────────────────────┤
│IP_HOST_PERF_ │ │RS232_RS485_  │ │ONU_          │ │XG_PON_DOWNSTREAM_  │
│MON_HIST_DATA │ │PERF_MON_     │ │OPERATIONAL_  │ │MGMT_PERF_MON_      │
│              │ │HIST_DATA     │ │PERF_MON_     │ │HIST_DATA           │
│TCP_UDP_PERF_ │ │              │ │HIST_DATA     │ │                    │
│MON_HIST_DATA │ │              │ │              │ │XG_PON_TC_PERF_MON_ │
│              │ │              │ │              │ │HIST_DATA           │
│              │ │              │ │              │ │                    │
│              │ │              │ │              │ │XG_PON_UPSTREAM_    │
│              │ │              │ │              │ │MGMT_PERF_MON_      │
│              │ │              │ │              │ │HIST_DATA           │
└──────────────┘ └──────────────┘ └──────────────┘ └────────────────────┘
```

## Table Form

| Table (prefix `PON_SERVICE_CONFIG_PROFILE_`) | Keys                                           | Leafrefs |
| -------------------------------------------- | ---------------------------------------------- | -------- |
| ENHANCED_FEC_PM_HIST_DATA                    | service-config-profile-name, managed-entity-id | (none)   |
| ENHANCED_TC_PERF_MON_HIST_DATA               | service-config-profile-name, managed-entity-id | (none)   |
| ETHERNET_FRAME_EXTENDED_PM                   | service-config-profile-name, managed-entity-id | (none)   |
| ETHERNET_FRAME_EXTENDED_PM64_BIT             | service-config-profile-name, managed-entity-id | (none)   |
| ETHERNET_FRAME_PERF_MON_HIST_DATA_DOWNSTREAM | service-config-profile-name, managed-entity-id | (none)   |
| ETHERNET_FRAME_PERF_MON_HIST_DATA_UPSTREAM   | service-config-profile-name, managed-entity-id | (none)   |
| ETHERNET_PERF_MON_HIST_DATA                  | service-config-profile-name, managed-entity-id | (none)   |
| ETHERNET_PERF_MON_HIST_DATA3                 | service-config-profile-name, managed-entity-id | (none)   |
| FEC_PERF_MON_HIST_DATA                       | service-config-profile-name, managed-entity-id | (none)   |
| GAL_ETHERNET_PERF_MON_HIST_DATA              | service-config-profile-name, managed-entity-id | (none)   |
| GEM_PORT_NETWORK_CTP_PERF_MON_HIST_DATA      | service-config-profile-name, managed-entity-id | (none)   |
| IP_HOST_PERF_MON_HIST_DATA                   | service-config-profile-name, managed-entity-id | (none)   |
| MAC_BRIDGE_PERF_MON_HIST_DATA                | service-config-profile-name, managed-entity-id | (none)   |
| MAC_BRIDGE_PORT_PERF_MON_HIST_DATA           | service-config-profile-name, managed-entity-id | (none)   |
| ONU_OPERATIONAL_PERF_MON_HIST_DATA           | service-config-profile-name, managed-entity-id | (none)   |
| RS232_RS485_PERF_MON_HIST_DATA               | service-config-profile-name, managed-entity-id | (none)   |
| TCP_UDP_PERF_MON_HIST_DATA                   | service-config-profile-name, managed-entity-id | (none)   |
| XG_PON_DOWNSTREAM_MGMT_PERF_MON_HIST_DATA    | service-config-profile-name, managed-entity-id | (none)   |
| XG_PON_TC_PERF_MON_HIST_DATA                 | service-config-profile-name, managed-entity-id | (none)   |
| XG_PON_UPSTREAM_MGMT_PERF_MON_HIST_DATA      | service-config-profile-name, managed-entity-id | (none)   |
