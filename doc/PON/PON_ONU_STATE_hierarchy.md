# PON_ONU State (`ro`) Hierarchy

`PON_ONU_STATE` is the root of the ONU state tree — every other `PON_ONU_*` `ro` table
leafrefs back to its `onu-name`.

## Graphical Form — Core State

```
                        ┌──────────────────────────────────────┐
                        │           PON_ONU_STATE              │
                        │  key: onu-name                       │
                        │  leafrefs: (none — root)             │
                        └──────────────────┬───────────────────┘
                                           │ onu-name
   ┌───────────────┬───────────────┬───────┴───────┬───────────────┬───────────────┐
   ▼               ▼               ▼               ▼               ▼               ▼
┌──────────┐ ┌───────────────┐ ┌──────────┐ ┌──────────────┐ ┌──────────────────────┐
│PON_ONU_  │ │PON_ONU_OLT_   │ │PON_ONU_  │ │PON_ONU_FW_   │ │PON_ONU_STATISTICS_   │
│UNI_STATE │ │ SERVICE_STATE │ │OLT_      │ │BANK_VERSION_ │ │ BINNED               │
│          │ │               │ │SERVICE_  │ │STATE         │ │                      │
│keys:     │ │keys:          │ │NETWORK_  │ │keys:         │ │keys:                 │
│ onu-name │ │ onu-name      │ │STATE     │ │ onu-name     │ │ onu-name             │
│ port-id  │ │ olt-service-id│ │keys:     │ │ bank-id      │ │ onu-stats-id         │
│          │ │               │ │ onu-name │ │              │ │  (bin timestamp)     │
│          │ │               │ │ olt-     │ │              │ │                      │
│          │ │               │ │ service- │ │              │ │  ── root of all      │
│          │ │               │ │ id       │ │              │ │     BINNED tables    │
│          │ │               │ │ vlan-id  │ │              │ │                      │
└────┬─────┘ └───────┬───────┘ └──────────┘ └──────────────┘ └──────────────────────┘
     │               │              ▲
     │               │              └── olt-service-id ──► PON_ONU_OLT_SERVICE_STATE/
     │               │                                     olt-service-id
     ▼               ▼
┌──────────────────┐ ┌──────────────────────────────────────┐
│PON_ONU_UNI_      │ │PON_ONU_OLT_SERVICE_DOWNSTREAM_       │
│ LEARNED_         │ │  QOS_MAP_STATE                       │
│ ADDRESSES_STATE  │ │keys: onu-name, olt-service-id        │
│keys:             │ │leafrefs:                             │
│ onu-name         │ │ onu-name ──► PON_ONU_STATE/onu-name  │
│ port-id          │ │ olt-service-id ──► OLT_SERVICE_STATE/│
│leafref:          │ │                    olt-service-id    │
│ port-id ──►      │ └──────────────────┬───────────────────┘
│  UNI_STATE/      │                    │ onu-name AND olt-service-id
│  port-id         │                    ▼
└──────────────────┘  ┌───────────────────────────────────────────┐
                      │PON_ONU_OLT_SERVICE_DOWNSTREAM_            │
                      │  QOS_MAP_MAP_STATE                        │
                      │keys: onu-name, olt-service-id, priority   │
                      │leafrefs: BOTH keys point at               │
                      │  ..._QOS_MAP_STATE (not at PON_ONU_STATE) │
                      └───────────────────────────────────────────┘
```

## Graphical Form — Statistics Families

```
┌──────────────────────────────────────────────────────────────────────────────┐
│ BINNED — keys: onu-name, onu-stats-id [, me-id | service-port-id]            │
│   leafrefs: onu-name     ──► PON_ONU_STATE/onu-name                          │
│             onu-stats-id ──► PON_ONU_STATISTICS_BINNED/onu-stats-id          │
│   21 tables                                                                  │
├──────────────────────────────────────────────────────────────────────────────┤
│ ACCUMULATING — keys: onu-name [, onu-id]                                     │
│   leafrefs: onu-name ──► PON_ONU_STATE/onu-name   (onu-id is a plain uint)   │
│   21 tables                                                                  │
├──────────────────────────────────────────────────────────────────────────────┤
│ STREAMING — keys: onu-name, onu-id                                           │
│   leafrefs: onu-name ──► PON_ONU_STATE/onu-name   (onu-id is a plain uint16) │
│   10 tables                                                                  │
└──────────────────────────────────────────────────────────────────────────────┘

                    ┌──────────────────────────────┐
                    │  PON_ONU_STATISTICS_BINNED   │
                    │  keys: onu-name, onu-stats-id│
                    └───────────────┬──────────────┘
                                    │ onu-stats-id (REAL leafref)
        ┌────────────┬──────────────┼──────────────┬──────────────┐
        ▼            ▼              ▼              ▼              ▼
   ┌─────────┐ ┌──────────┐ ┌────────────┐ ┌─────────────┐ ┌──────────────┐
   │OLT_PON  │ │OLT_PON_  │ │ ONU_PON    │ │OLT_PON_     │ │ 17 × ONU_*_PM│
   │         │ │  OMCC    │ │            │ │ SERVICE     │ │  (+me-id key)│
   └─────────┘ └──────────┘ └────────────┘ └─────────────┘ └──────────────┘
                                            (+service-port-id)
```

## Table Form — Core State

| Table                                            | Keys                               | Leafrefs                                                                                                                                         |
| ------------------------------------------------ | ---------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------ |
| PON_ONU_STATE                                    | onu-name                           | (none — root)                                                                                                                                    |
| PON_ONU_OLT_SERVICE_STATE                        | onu-name, olt-service-id           | onu-name -> PON_ONU_STATE/onu-name                                                                                                               |
| PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE     | onu-name, olt-service-id           | onu-name -> PON_ONU_STATE/onu-name; olt-service-id -> PON_ONU_OLT_SERVICE_STATE/olt-service-id                                                   |
| PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_MAP_STATE | onu-name, olt-service-id, priority | onu-name -> PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE/onu-name; olt-service-id -> PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP_STATE/olt-service-id |
| PON_ONU_OLT_SERVICE_NETWORK_STATE                | onu-name, olt-service-id, vlan-id  | onu-name -> PON_ONU_STATE/onu-name; olt-service-id -> PON_ONU_OLT_SERVICE_STATE/olt-service-id                                                   |
| PON_ONU_UNI_STATE                                | onu-name, port-id                  | onu-name -> PON_ONU_STATE/onu-name                                                                                                               |
| PON_ONU_UNI_LEARNED_ADDRESSES_STATE              | onu-name, port-id                  | onu-name -> PON_ONU_STATE/onu-name; port-id -> PON_ONU_UNI_STATE/port-id                                                                         |
| PON_ONU_FW_BANK_VERSION_STATE                    | onu-name, bank-id                  | onu-name -> PON_ONU_STATE/onu-name                                                                                                               |

## Table Form — BINNED Statistics

Shared leafrefs on every row below: `onu-name` -> `PON_ONU_STATE/onu-name`, and (except on
the root) `onu-stats-id` -> `PON_ONU_STATISTICS_BINNED/onu-stats-id`.

| Table (prefix `PON_ONU_STATISTICS_BINNED`) | Extra key       |
| ------------------------------------------ | --------------- |
| _(root)_                                   | —               |
| \_OLT_PON                                  | —               |
| \_OLT_PON_OMCC                             | —               |
| \_ONU_PON                                  | —               |
| \_OLT_PON_SERVICE                          | service-port-id |
| \_ONU_FEC_PM                               | me-id           |
| \_ONU_ENHANCED_TC_PM                       | me-id           |
| \_ONU_ETHERNET_PM                          | me-id           |
| \_ONU_ETHERNET_PM3                         | me-id           |
| \_ONU_ETHERNET_FRAME_EXTENDED_PM           | me-id           |
| \_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT     | me-id           |
| \_ONU_ETHERNET_FRAME_PM_DOWNSTREAM         | me-id           |
| \_ONU_ETHERNET_FRAME_PM_UPSTREAM           | me-id           |
| \_ONU_GAL_ETHERNET_PM                      | me-id           |
| \_ONU_GEM_PORT_NETWORK_CTP_PM              | me-id           |
| \_ONU_IP_HOST_PERF_MON_HIST_DATA           | me-id           |
| \_ONU_MAC_BRIDGE_PORT_PM                   | me-id           |
| \_ONU_OPERATIONAL_PM                       | me-id           |
| \_ONU_RS232_RS485_PERF_MON_HIST_DATA       | me-id           |
| \_ONU_TCP_UDP_PERF_MON_HIST_DATA           | me-id           |
| \_ONU_XG_PON_DOWNSTREAM_MGMT_PM            | me-id           |
| \_ONU_XG_PON_UPSTREAM_MGMT_PM              | me-id           |

## Table Form — ACCUMULATING Statistics

Shared leafref on every row: `onu-name` -> `PON_ONU_STATE/onu-name`. `onu-id` is a plain
integer, **not** a leafref.

| Table (prefix `PON_ONU_STATISTICS_ACCUMULATING`) | Extra key      |
| ------------------------------------------------ | -------------- |
| \_OLT_PON                                        | —              |
| \_OLT_PON_OMCC                                   | —              |
| \_ONU_PON                                        | —              |
| \_OLT_PON_SERVICE                                | onu-id (uint8) |
| \_ONU_FEC_PM                                     | onu-id         |
| \_ONU_ENHANCED_TC_PM                             | onu-id         |
| \_ONU_ETHERNET_PM                                | onu-id         |
| \_ONU_ETHERNET_PM3                               | onu-id         |
| \_ONU_ETHERNET_FRAME_EXTENDED_PM                 | onu-id         |
| \_ONU_ETHERNET_FRAME_EXTENDED_PM_64BIT           | onu-id         |
| \_ONU_ETHERNET_FRAME_PM_DOWNSTREAM               | onu-id         |
| \_ONU_ETHERNET_FRAME_PM_UPSTREAM                 | onu-id         |
| \_ONU_GAL_ETHERNET_PM                            | onu-id         |
| \_ONU_GEM_PORT_NETWORK_CTP_PM                    | onu-id         |
| \_ONU_IP_HOST_PERF_MON_HIST_DATA                 | onu-id         |
| \_ONU_MAC_BRIDGE_PORT_PM                         | onu-id         |
| \_ONU_OPERATIONAL_PM                             | onu-id         |
| \_ONU_RS232_RS485_PERF_MON_HIST_DATA             | onu-id         |
| \_ONU_TCP_UDP_PERF_MON_HIST_DATA                 | onu-id         |
| \_ONU_XG_PON_DOWNSTREAM_MGMT_PM                  | onu-id         |
| \_ONU_XG_PON_UPSTREAM_MGMT_PM                    | onu-id         |

## Table Form — STREAMING Statistics

Shared leafref on every row: `onu-name` -> `PON_ONU_STATE/onu-name`. All are keyed
`onu-name, onu-id` where `onu-id` is a plain `uint16`.

| Table (prefix `PON_ONU_STATISTICS_STREAMING`) |
| --------------------------------------------- |
| \_ONU_FEC_PM                                  |
| \_ONU_ENHANCED_TC_PM                          |
| \_ONU_ETHERNET_PM                             |
| \_ONU_ETHERNET_FRAME_PM_DOWNSTREAM            |
| \_ONU_ETHERNET_FRAME_PM_UPSTREAM              |
| \_ONU_GAL_ETHERNET_PM                         |
| \_ONU_GEM_PORT_NETWORK_CTP_PM                 |
| \_ONU_RS232_RS485_PERF_MON_HIST_DATA          |
| \_ONU_XG_PON_DOWNSTREAM_MGMT_PM               |
| \_ONU_XG_PON_UPSTREAM_MGMT_PM                 |

## Config-to-State Key Mapping

| Config table                 | State counterpart                               | Key rename                   |
| ---------------------------- | ----------------------------------------------- | ---------------------------- |
| PON_ONU                      | PON_ONU_STATE                                   | none (`onu-name`)            |
| PON_ONU_OLT_SERVICE          | PON_ONU_OLT_SERVICE_STATE                       | none                         |
| PON_ONU_OLT_SERVICE_NETWORK  | PON_ONU_OLT_SERVICE_NETWORK_STATE               | none                         |
| PON_ONU_UNI                  | PON_ONU_UNI_STATE                               | none                         |
| PON_ONU_FW_BANK_FILE         | PON_ONU_FW_BANK_VERSION_STATE                   | none (`onu-name`, `bank-id`) |
| PON_ONU_SERVICE_CONFIG_VALUE | _(no state counterpart)_                        | —                            |
| _(no config counterpart)_    | PON_ONU_UNI_LEARNED_ADDRESSES_STATE             | —                            |
| _(no config counterpart)_    | PON_ONU_OLT_SERVICE_DOWNSTREAM_QOS_MAP\*\_STATE | —                            |

Note: the same logical PM counter set is keyed on `me-id` in the BINNED family but on
`onu-id` in the ACCUMULATING and STREAMING families.
