# PON_SERVICE_CONFIG_PROFILE — GEM / TCONT / TRAFFIC Group

## Graphical Form

```
┌────────────────────────────────────────────────────┐
│ PRIORITY_QUEUE                                     │
│  keys: service-config-profile-name,                │
│        managed-entity-id                           │
└──────────────────────────┬─────────────────────────┘
                           │ managed-entity-id  (REAL leafref)
                           ▼
        ┌────────────────────────────────────────────────┐
        │ PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD     │
        │ keys: service-config-profile-name,             │
        │       managed-entity-id                        │
        │ leafref: managed-entity-id ──► PRIORITY_QUEUE/ │
        │                                managed-entity-id│
        └────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────┐
│ MULTICAST_GEM_INTERWORKING_TP                      │
│  keys: service-config-profile-name,                │
│        managed-entity-id                           │
└──────────────────────────┬─────────────────────────┘
                           │ (name containment, no leafref)
        ┌──────────────────┴──────────────────┐
        ▼                                     ▼
┌────────────────────────────────┐ ┌────────────────────────────────┐
│ ..._IPV4_MULTICAST_ADDRESS     │ │ ..._IPV6_MULTICAST_ADDRESS     │
│ keys: scp-name, id,            │ │ keys: scp-name, id,            │
│       managed-entity-id,       │ │       managed-entity-id,       │
│       ipv4-multicast-address-  │ │       ipv6-multicast-address-  │
│       table-entry-id           │ │       table-entry-id           │
└────────────────────────────────┘ └────────────────────────────────┘

Flat siblings (keys: service-config-profile-name, managed-entity-id; no leafrefs):

┌──────────────────────┐ ┌──────────────────────┐ ┌──────────────────────┐
│ GAL_ETHERNET_PROFILE │ │ GEM_INTERWORKING_TP  │ │ GEM_PORT_NETWORK_CTP │
└──────────────────────┘ └──────────────────────┘ └──────────────────────┘
┌──────────────────────┐ ┌──────────────────────┐ ┌──────────────────────┐
│ TCONT                │ │ TRAFFIC_DESCRIPTOR   │ │ TRAFFIC_SCHEDULER    │
└──────────────────────┘ └──────────────────────┘ └──────────────────────┘
```

## Table Form

| Table (prefix `PON_SERVICE_CONFIG_PROFILE_`)         | Keys                                                                                      | Leafrefs                                                                         |
| ---------------------------------------------------- | ----------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------- |
| GAL_ETHERNET_PROFILE                                 | service-config-profile-name, managed-entity-id                                            | (none)                                                                           |
| GEM_INTERWORKING_TP                                  | service-config-profile-name, managed-entity-id                                            | (none)                                                                           |
| GEM_PORT_NETWORK_CTP                                 | service-config-profile-name, managed-entity-id                                            | (none)                                                                           |
| MULTICAST_GEM_INTERWORKING_TP                        | service-config-profile-name, managed-entity-id                                            | (none)                                                                           |
| MULTICAST_GEM_INTERWORKING_TP_IPV4_MULTICAST_ADDRESS | service-config-profile-name, id, managed-entity-id, ipv4-multicast-address-table-entry-id | (none)                                                                           |
| MULTICAST_GEM_INTERWORKING_TP_IPV6_MULTICAST_ADDRESS | service-config-profile-name, id, managed-entity-id, ipv6-multicast-address-table-entry-id | (none)                                                                           |
| TCONT                                                | service-config-profile-name, managed-entity-id                                            | (none)                                                                           |
| TRAFFIC_DESCRIPTOR                                   | service-config-profile-name, managed-entity-id                                            | (none)                                                                           |
| TRAFFIC_SCHEDULER                                    | service-config-profile-name, managed-entity-id                                            | (none)                                                                           |
| PRIORITY_QUEUE                                       | service-config-profile-name, managed-entity-id                                            | (none)                                                                           |
| PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD           | service-config-profile-name, managed-entity-id                                            | managed-entity-id -> PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE/managed-entity-id |
