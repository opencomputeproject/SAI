# PON_OLT State (`ro`) Hierarchy

`PON_OLT_INTF_STATE` is the root of the OLT state tree — nearly every other `PON_OLT_*` `ro`
table leafrefs back to its `olt-intf-name`, directly or via `PON_OLT_PLUG_STATE`.

## Graphical Form — Interface / ONU

```
                        ┌──────────────────────────────────────┐
                        │        PON_OLT_INTF_STATE            │
                        │  key: olt-intf-name                  │
                        │  leafrefs: (none — root)             │
                        └──────────────────┬───────────────────┘
                                           │ olt-intf-name
        ┌──────────────────┬───────────────┼───────────────┬──────────────────┐
        ▼                  ▼               ▼               ▼                  ▼
┌────────────────┐ ┌──────────────┐ ┌─────────────┐ ┌──────────────┐ ┌──────────────────┐
│PON_OLT_INTF_   │ │PON_OLT_INTF_ │ │PON_OLT_INTF_│ │PON_OLT_INTF_ │ │ PON_OLT_PLUG_    │
│  ONU_STATE     │ │ NETWORK_STATE│ │ ONU_SERVICE_│ │ ONU_SERVICE_ │ │   STATE          │
│                │ │              │ │ TCONT_STATE │ │GEMPORT_STATE │ │                  │
│keys:           │ │keys:         │ │keys:        │ │keys:         │ │keys:             │
│ olt-intf-name  │ │ olt-intf-name│ │ olt-intf-   │ │ olt-intf-    │ │ name             │
│ onu-serial-    │ │ vlan-id      │ │  name       │ │  name        │ │                  │
│  number        │ │              │ │ onu-serial- │ │ onu-serial-  │ │leafref:          │
│                │ │              │ │  number     │ │  number      │ │ name ──► PON_OLT_│
│                │ │              │ │ service-    │ │ service-     │ │  INTF_STATE/     │
│                │ │              │ │  port-id    │ │  port-id     │ │  olt-intf-name   │
└───────┬────────┘ └──────┬───────┘ └──────┬──────┘ └──────┬───────┘ └────────┬─────────┘
        │                 │                │               │                  │
        │                 │                └───────────────┘                  │
        │                 │                 tcont-ref ──► ..._TCONT_STATE/     │
        │                 │                               service-port-id     │
        │                 │                                                   │
   ┌────┴─────┐           ▼                                    ┌──────────────┴──────────┐
   ▼          ▼   ┌──────────────────────────────┐             ▼                         ▼
┌────────┐ ┌──────┴────────┐ │PON_OLT_INTF_NNI_  │  ┌────────────────────┐ ┌──────────────────────┐
│ ..._ONU│ │ ..._ONU_      │ │ NETWORK_LEARNING_ │  │PON_OLT_PLUG_FW_    │ │PON_OLT_PLUG_FW_BANK_ │
│ _FW_   │ │ OPERATIONAL_  │ │ TABLE_STATE       │  │ UPGRADE_STATUS     │ │ VERSION_STATE        │
│UPGRADE_│ │ STATE         │ │keys:              │  │key: name           │ │keys: name, bank-id   │
│ STATE  │ │               │ │ olt-intf-name     │  │leafref: name ──►   │ │leafref: name ──►     │
│keys:   │ │keys:          │ │ network-id        │  │ PLUG_STATE/name    │ │ PLUG_STATE/name      │
│ olt-   │ │ olt-intf-name │ │ mac-address       │  └────────────────────┘ └──────────────────────┘
│ intf-  │ │ id            │ │leafrefs:          │
│ name   │ │               │ │ olt-intf-name ──► │
│ onu-   │ │leafref:       │ │  INTF_STATE       │
│ serial-│ │ olt-intf-name │ │ network-id ──►    │
│ number │ │  ──► ONU_STATE│ │  NETWORK_STATE/   │
│        │ │               │ │  network-id       │
│leafref:│ │               │ └───────────────────┘
│ olt-   │ │               │
│ intf-  │ │               │
│ name   │ │               │
│ ──►    │ │               │
│ ONU_   │ │               │
│ STATE  │ │               │
└────────┘ └───────────────┘
```

## Graphical Form — Statistics

```
        ┌──────────────────────────────────────┐
        │        PON_OLT_INTF_STATE            │
        │  key: olt-intf-name                  │
        └──────────────────┬───────────────────┘
                           │ olt-intf-name / olt-name
                           ▼
        ┌──────────────────────────────────────┐
        │  PON_OLT_INTF_STATISTICS_BINNED      │
        │  keys: olt-intf-name, olt-stats-id   │
        │  leafref: olt-intf-name ──►          │
        │           INTF_STATE/olt-intf-name   │
        │  (olt-stats-id is the bin timestamp) │
        └──────────────────┬───────────────────┘
                           │ olt-stats-id  (REAL leafref on all below)
   ┌───────────┬───────────┼───────────┬────────────────────────┐
   ▼           ▼           ▼           ▼                        ▼
┌────────┐ ┌────────┐ ┌────────┐ ┌──────────────────┐ ┌───────────────────────┐
│PLUG_   │ │PLUG_   │ │PLUG_   │ │STATISTICS_BINNED_│ │STATISTICS_BINNED_PON_ │
│STATIS- │ │STATIS- │ │STATIS- │ │  PON_FLOODING    │ │ FLOODING_NETWORK      │
│TICS_   │ │TICS_   │ │TICS_   │ │keys: olt-name,   │ │keys: olt-name,        │
│BINNED_ │ │BINNED_ │ │BINNED_ │ │ olt-stats-id,    │ │ olt-stats-id,         │
│ENV     │ │NNI     │ │TEMP    │ │ flood-id         │ │ flood-id              │
│        │ │        │ │        │ │                  │ │leafref: flood-id ──►  │
│        │ │        │ │        │ │                  │ │ PON_FLOODING/flood-id │
└────────┘ └────────┘ └────────┘ └──────────────────┘ └───────────────────────┘

Accumulating counterparts — keyed by olt-name only (no olt-stats-id bin):

┌──────────────────┐ ┌──────────────────┐ ┌──────────────────┐
│ ..._ACCUMULATING │ │ ..._ACCUMULATING_│ │ ..._ACCUMULATING_│
│                  │ │       ENV        │ │       TEMP       │
└──────────────────┘ └──────────────────┘ └──────────────────┘
┌────────────────────────────┐ ┌───────────────────────────────────────┐
│ ..._ACCUMULATING_PON_      │ │ ..._ACCUMULATING_PON_FLOODING_NNI_    │
│   FLOODING                 │ │   NETWORK                             │
│ keys: olt-name, olt-id     │ │ keys: olt-name, olt-id                │
│ (olt-id is a plain uint16) │ │ (olt-id is a plain uint16)            │
└────────────────────────────┘ └───────────────────────────────────────┘
```

## Table Form — Interface / ONU / Plug

| Table                                         | Keys                                              | Leafrefs                                                                                                                                                                            |
| --------------------------------------------- | ------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| PON_OLT_INTF_STATE                            | olt-intf-name                                     | (none — root)                                                                                                                                                                       |
| PON_OLT_INTF_ONU_STATE                        | olt-intf-name, onu-serial-number                  | olt-intf-name -> PON_OLT_INTF_STATE/olt-intf-name                                                                                                                                   |
| PON_OLT_INTF_ONU_FW_UPGRADE_STATE             | olt-intf-name, onu-serial-number                  | olt-intf-name -> PON_OLT_INTF_ONU_STATE/olt-intf-name                                                                                                                               |
| PON_OLT_INTF_ONU_OPERATIONAL_STATE            | olt-intf-name, id                                 | olt-intf-name -> PON_OLT_INTF_ONU_STATE/olt-intf-name                                                                                                                               |
| PON_OLT_INTF_NETWORK_STATE                    | olt-intf-name, vlan-id                            | olt-intf-name -> PON_OLT_INTF_STATE/olt-intf-name                                                                                                                                   |
| PON_OLT_INTF_NNI_NETWORK_LEARNING_TABLE_STATE | olt-intf-name, network-id, mac-address            | olt-intf-name -> PON_OLT_INTF_STATE/olt-intf-name; network-id -> PON_OLT_INTF_NETWORK_STATE/network-id                                                                              |
| PON_OLT_INTF_ONU_SERVICE_TCONT_STATE          | olt-intf-name, onu-serial-number, service-port-id | olt-intf-name -> PON_OLT_INTF_STATE/olt-intf-name; onu-serial-number -> PON_OLT_INTF_ONU_STATE/onu-serial-number                                                                    |
| PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE        | olt-intf-name, onu-serial-number, service-port-id | olt-intf-name -> PON_OLT_INTF_STATE/olt-intf-name; onu-serial-number -> PON_OLT_INTF_ONU_STATE/onu-serial-number; tcont-ref -> PON_OLT_INTF_ONU_SERVICE_TCONT_STATE/service-port-id |
| PON_OLT_PLUG_STATE                            | name                                              | name -> PON_OLT_INTF_STATE/olt-intf-name                                                                                                                                            |
| PON_OLT_PLUG_FW_UPGRADE_STATUS                | name                                              | name -> PON_OLT_PLUG_STATE/name                                                                                                                                                     |
| PON_OLT_PLUG_FW_BANK_VERSION_STATE            | name, bank-id                                     | name -> PON_OLT_PLUG_STATE/name                                                                                                                                                     |
| PON_OLT_PROTECTION_GROUP_STATE                | group-name                                        | (none)                                                                                                                                                                              |

## Table Form — Statistics

| Table                                                    | Keys                             | Leafrefs                                                                                                                                                               |
| -------------------------------------------------------- | -------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| PON_OLT_INTF_STATISTICS_BINNED                           | olt-intf-name, olt-stats-id      | olt-intf-name -> PON_OLT_INTF_STATE/olt-intf-name                                                                                                                      |
| PON_OLT_PLUG_STATISTICS_BINNED_ENV                       | olt-name, olt-stats-id           | olt-name -> PON_OLT_INTF_STATE/olt-intf-name; olt-stats-id -> PON_OLT_INTF_STATISTICS_BINNED/olt-stats-id                                                              |
| PON_OLT_PLUG_STATISTICS_BINNED_NNI                       | olt-name, olt-stats-id           | olt-name -> PON_OLT_INTF_STATE/olt-intf-name; olt-stats-id -> PON_OLT_INTF_STATISTICS_BINNED/olt-stats-id                                                              |
| PON_OLT_PLUG_STATISTICS_BINNED_TEMP                      | olt-name, olt-stats-id           | olt-name -> PON_OLT_INTF_STATE/olt-intf-name; olt-stats-id -> PON_OLT_INTF_STATISTICS_BINNED/olt-stats-id                                                              |
| PON_OLT_STATISTICS_BINNED_PON_FLOODING                   | olt-name, olt-stats-id, flood-id | olt-name -> PON_OLT_INTF_STATE/olt-intf-name; olt-stats-id -> PON_OLT_INTF_STATISTICS_BINNED/olt-stats-id                                                              |
| PON_OLT_STATISTICS_BINNED_PON_FLOODING_NETWORK           | olt-name, olt-stats-id, flood-id | olt-name -> PON_OLT_INTF_STATE/olt-intf-name; olt-stats-id -> PON_OLT_INTF_STATISTICS_BINNED/olt-stats-id; flood-id -> PON_OLT_STATISTICS_BINNED_PON_FLOODING/flood-id |
| PON_OLT_STATISTICS_ACCUMULATING                          | olt-name                         | olt-name -> PON_OLT_INTF_STATE/olt-intf-name                                                                                                                           |
| PON_OLT_STATISTICS_ACCUMULATING_ENV                      | olt-name                         | olt-name -> PON_OLT_INTF_STATE/olt-intf-name                                                                                                                           |
| PON_OLT_STATISTICS_ACCUMULATING_TEMP                     | olt-name                         | olt-name -> PON_OLT_INTF_STATE/olt-intf-name                                                                                                                           |
| PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING             | olt-name, olt-id                 | olt-name -> PON_OLT_INTF_STATE/olt-intf-name                                                                                                                           |
| PON_OLT_STATISTICS_ACCUMULATING_PON_FLOODING_NNI_NETWORK | olt-name, olt-id                 | olt-name -> PON_OLT_INTF_STATE/olt-intf-name                                                                                                                           |

## Config-to-State Key Mapping

| Config table                     | State counterpart                      | Key rename                            |
| -------------------------------- | -------------------------------------- | ------------------------------------- |
| PON_OLT_INTF                     | PON_OLT_INTF_STATE                     | none (`olt-intf-name`)                |
| PON_OLT_INTF_ONU                 | PON_OLT_INTF_ONU_STATE                 | none                                  |
| PON_OLT_INTF_NETWORK             | PON_OLT_INTF_NETWORK_STATE             | none                                  |
| PON_OLT_INTF_ONU_SERVICE_TCONT   | PON_OLT_INTF_ONU_SERVICE_TCONT_STATE   | `olt-service-id` -> `service-port-id` |
| PON_OLT_INTF_ONU_SERVICE_GEMPORT | PON_OLT_INTF_ONU_SERVICE_GEMPORT_STATE | `olt-service-id` -> `service-port-id` |
| PON_OLT_PLUG                     | PON_OLT_PLUG_STATE                     | `olt-name` -> `name`                  |
| PON_OLT_PROTECTION_GROUP         | PON_OLT_PROTECTION_GROUP_STATE         | none (`group-name`)                   |
| PON_OLT_PLUG_FW_BANK_FILE        | PON_OLT_PLUG_FW_BANK_VERSION_STATE     | `olt-name` -> `name`                  |
| PON_OLT_INTF_ONU_OLT_SERVICE     | _(no state counterpart)_               | —                                     |

Note: the statistics tables use `olt-name` while the interface state tables use
`olt-intf-name`; both leafref the same `PON_OLT_INTF_STATE/olt-intf-name`.
