# PON_SERVICE_CONFIG_PROFILE Table Group Index

There is no single `PON_SERVICE_CONFIG_PROFILE` parent table in the tree. All tables are
siblings joined by the shared key `service-config-profile-name` (the `HEADER_*` tables use
`name` instead). They are split here by name substring.

## Grouping Pattern

| File                                                                                                           | Substring match                                                                                                                | Count |
| -------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------ | ----- |
| [PON_SERVICE_CONFIG_PROFILE_HEADER_hierarchy.md](PON_SERVICE_CONFIG_PROFILE_HEADER_hierarchy.md)               | `_HEADER*`                                                                                                                     | 3     |
| [PON_SERVICE_CONFIG_PROFILE_EQUIPMENT_hierarchy.md](PON_SERVICE_CONFIG_PROFILE_EQUIPMENT_hierarchy.md)         | `_ANI_G`, `_CARDHOLDER`, `_CIRCUIT_PACK`, `_OLT_G`, `_ONU_G`, `_ONU2_G`, `_LARGE_STRING`, `_PPTP_*_UNI`, `_VIRTUAL_ETHERNET_*` | 9     |
| [PON_SERVICE_CONFIG_PROFILE_BRIDGING_VLAN_hierarchy.md](PON_SERVICE_CONFIG_PROFILE_BRIDGING_VLAN_hierarchy.md) | `_VLAN_TAGGING*`, `_MAC_BRIDGE_*` (non-PM), `_IEEE8021P_*`                                                                     | 9     |
| [PON_SERVICE_CONFIG_PROFILE_GEM_TRAFFIC_hierarchy.md](PON_SERVICE_CONFIG_PROFILE_GEM_TRAFFIC_hierarchy.md)     | `_GEM_*`, `_GAL_ETHERNET_PROFILE`, `_TCONT`, `_TRAFFIC_*`, `_PRIORITY_QUEUE*`                                                  | 11    |
| [PON_SERVICE_CONFIG_PROFILE_IP_TRANSPORT_hierarchy.md](PON_SERVICE_CONFIG_PROFILE_IP_TRANSPORT_hierarchy.md)   | `_IP_HOST_CONFIG_DATA`, `_IPV6_*`, `_TCP_UDP_CONFIG_DATA`, `_SSH_SERVER_*`, `_RS232_RS485_PORT_*`                              | 6     |
| [PON_SERVICE_CONFIG_PROFILE_PM_hierarchy.md](PON_SERVICE_CONFIG_PROFILE_PM_hierarchy.md)                       | `*_PERF_MON_HIST_DATA*`, `*_PM*`                                                                                               | 20    |
| [PON_SERVICE_CONFIG_PROFILE_THRESHOLD_hierarchy.md](PON_SERVICE_CONFIG_PROFILE_THRESHOLD_hierarchy.md)         | `_THRESHOLD_DATA*`                                                                                                             | 3     |

## Shared Key Pattern

```
                        ┌──────────────────────────────────────────┐
                        │  service-config-profile-name  (string)   │
                        │  shared, non-leafref key on 58 tables    │
                        └──────────────────┬───────────────────────┘
                                           │
       ┌──────────────┬──────────────┬─────┴──────┬──────────────┬──────────────┐
       ▼              ▼              ▼            ▼              ▼              ▼
   EQUIPMENT    BRIDGING_VLAN   GEM_TRAFFIC  IP_TRANSPORT       PM         THRESHOLD

                        ┌──────────────────────────────────────────┐
                        │            name  (string)                │
                        │  shared key on the 3 HEADER_* tables     │
                        │  (the only real leafref parent/child)    │
                        └──────────────────────────────────────────┘
```

## Key Shapes

| Shape                        | Keys                                                                   | Applies to |
| ---------------------------- | ---------------------------------------------------------------------- | ---------- |
| 2-key (most common)          | `service-config-profile-name`, `managed-entity-id`                     | 52 tables  |
| 4-key (nested table entries) | `service-config-profile-name`, `id`, `managed-entity-id`, `<entry>-id` | 6 tables   |
| HEADER                       | `name` (+ `vendor-id` / `db-ref`)                                      | 3 tables   |

## All Leafrefs in the Group

Only three tables declare leafrefs; every other key is a plain `string` /
`omci-types:omci-uint16`.

| Table                                                                 | Leafref                                                                          |
| --------------------------------------------------------------------- | -------------------------------------------------------------------------------- |
| PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL          | name -> PON_SERVICE_CONFIG_PROFILE_HEADER/name                                   |
| PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT                    | name -> PON_SERVICE_CONFIG_PROFILE_HEADER/name                                   |
| PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE_PACKET_DROP_QUEUE_THRESHOLD | managed-entity-id -> PON_SERVICE_CONFIG_PROFILE_PRIORITY_QUEUE/managed-entity-id |
