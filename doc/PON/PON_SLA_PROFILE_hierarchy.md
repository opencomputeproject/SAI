# PON_SLA_PROFILE,  PON_DOWNSTREAM_QOS_MAP_MAP Config Hierarchy

## Graphical Form

```
┌────────────────────────────────────┐
│          PON_SLA_PROFILE           │
│  key: sla-profile-name             │
└─────────────────┬──────────────────┘
                  │
                  ▼
┌────────────────────────────────────┐
│      PON_SLA_PROFILE_CONTROLLER    │
│  key: name                         │
│  leafref: name -> PON_SLA_PROFILE/ │
│           sla-profile-name         │
└────────────────────────────────────┘

┌────────────────────────────────────┐
│      PON_DOWNSTREAM_QOS_MAP_MAP    │
│  keys: name, cos                   │
│  (no leafrefs; standalone table)   │
└────────────────────────────────────┘
```

## Table Form

| Table                      | Keys             | Leafrefs                                 |
| -------------------------- | ---------------- | ---------------------------------------- |
| PON_SLA_PROFILE            | sla-profile-name | none                                     |
| PON_SLA_PROFILE_CONTROLLER | name             | name -> PON_SLA_PROFILE/sla-profile-name |
| PON_DOWNSTREAM_QOS_MAP_MAP | name, cos        | none                                     |

## Non-Leafref References (by naming convention only)

| Referencing table / field                                                                | Points at (by convention)        |
| ---------------------------------------------------------------------------------------- | -------------------------------- |
| PON_OLT_INTF_NETWORK.flooding-sla-profile                                                | PON_SLA_PROFILE.sla-profile-name |
| PON_ONU_TEMPLATE_OLT_SERVICE.sla-profile / PON_ONU_OLT_SERVICE.sla-profile               | PON_SLA_PROFILE.sla-profile-name |
| PON_ONU_TEMPLATE_OLT_SERVICE.downstream-qos-map / PON_ONU_OLT_SERVICE.downstream-qos-map | PON_DOWNSTREAM_QOS_MAP_MAP.name  |
