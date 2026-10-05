# PON_SERVICE_CONFIG_PROFILE — BRIDGING / VLAN Group

## Graphical Form

Note: the `_TABLE_ENTRY` style children are _not_ leafrefs — they repeat
`service-config-profile-name` / `managed-entity-id` as plain keys plus an extra `id` key.
The arrows below show the naming/logical containment, not a YANG leafref.

```
┌────────────────────────────────────────────────────┐
│ EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA        │
│  keys: service-config-profile-name,                │
│        managed-entity-id                           │
└──────────────────────────┬─────────────────────────┘
                           │ (name containment, no leafref)
        ┌──────────────────┴──────────────────┐
        ▼                                     ▼
┌────────────────────────────────┐ ┌────────────────────────────────┐
│ ..._RECEIVED_FRAME_VLAN_       │ │ ..._DSCP_TO_PBIT_MAPPING       │
│    TAGGING_OPERATION           │ │                                │
│ keys: scp-name, id,            │ │ keys: scp-name, id,            │
│       managed-entity-id,       │ │       managed-entity-id,       │
│       received-frame-vlan-     │ │       dscp                     │
│       tagging-operation-       │ │                                │
│       table-entry-id           │ │                                │
└────────────────────────────────┘ └────────────────────────────────┘

┌────────────────────────────────────────────────────┐
│ VLAN_TAGGING_FILTER_DATA                           │
│  keys: service-config-profile-name,                │
│        managed-entity-id                           │
└──────────────────────────┬─────────────────────────┘
                           ▼
        ┌──────────────────────────────────────┐
        │ VLAN_TAGGING_FILTER_DATA_VLAN_FILTER │
        │ keys: scp-name, id,                  │
        │       managed-entity-id,             │
        │       vlan-filter-entry-id           │
        └──────────────────────────────────────┘

┌────────────────────────────────────────────────────┐
│ IEEE8021P_MAPPER_SERVICE_PROFILE                   │
│  keys: service-config-profile-name,                │
│        managed-entity-id                           │
└──────────────────────────┬─────────────────────────┘
                           ▼
        ┌──────────────────────────────────────────────┐
        │ IEEE8021P_MAPPER_SERVICE_PROFILE_            │
        │   DSCP_TO_P_BIT_MAPPING                      │
        │ keys: scp-name, id, managed-entity-id, dscp  │
        └──────────────────────────────────────────────┘

┌───────────────────────────────┐  ┌───────────────────────────────┐
│ MAC_BRIDGE_PORT_CONFIG_DATA   │  │ MAC_BRIDGE_SERVICE_PROFILE    │
│ keys: scp-name,               │  │ keys: scp-name,               │
│       managed-entity-id       │  │       managed-entity-id       │
└───────────────────────────────┘  └───────────────────────────────┘
```

## Table Form

| Table (prefix `PON_SERVICE_CONFIG_PROFILE_`)                                      | Keys                                                                                                     | Leafrefs |
| --------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------- | -------- |
| EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA                                       | service-config-profile-name, managed-entity-id                                                           | (none)   |
| EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_RECEIVED_FRAME_VLAN_TAGGING_OPERATION | service-config-profile-name, id, managed-entity-id, received-frame-vlan-tagging-operation-table-entry-id | (none)   |
| EXTENDED_VLAN_TAGGING_OPERATION_CONFIG_DATA_DSCP_TO_PBIT_MAPPING                  | service-config-profile-name, id, managed-entity-id, dscp                                                 | (none)   |
| VLAN_TAGGING_FILTER_DATA                                                          | service-config-profile-name, managed-entity-id                                                           | (none)   |
| VLAN_TAGGING_FILTER_DATA_VLAN_FILTER                                              | service-config-profile-name, id, managed-entity-id, vlan-filter-entry-id                                 | (none)   |
| MAC_BRIDGE_PORT_CONFIG_DATA                                                       | service-config-profile-name, managed-entity-id                                                           | (none)   |
| MAC_BRIDGE_SERVICE_PROFILE                                                        | service-config-profile-name, managed-entity-id                                                           | (none)   |
| IEEE8021P_MAPPER_SERVICE_PROFILE                                                  | service-config-profile-name, managed-entity-id                                                           | (none)   |
| IEEE8021P_MAPPER_SERVICE_PROFILE_DSCP_TO_P_BIT_MAPPING                            | service-config-profile-name, id, managed-entity-id, dscp                                                 | (none)   |
