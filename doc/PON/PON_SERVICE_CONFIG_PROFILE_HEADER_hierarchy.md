# PON_SERVICE_CONFIG_PROFILE — HEADER Group

## Graphical Form

```
                    ┌────────────────────────────────────────────┐
                    │   PON_SERVICE_CONFIG_PROFILE_HEADER        │
                    │   key: name                                │
                    │   leafrefs: (none)                         │
                    └─────────────────────┬──────────────────────┘
                                          │
                ┌─────────────────────────┴─────────────────────────┐
                ▼                                                   ▼
┌───────────────────────────────────────┐  ┌───────────────────────────────────────┐
│ PON_SERVICE_CONFIG_PROFILE_HEADER_    │  │ PON_SERVICE_CONFIG_PROFILE_HEADER_    │
│   COMPATIBILITY_VENDOR_MODEL          │  │   INPUTS_EXT_INPUT                    │
│                                       │  │                                       │
│ keys:                                 │  │ keys:                                 │
│   vendor-id                           │  │   name                                │
│   name                                │  │   db-ref                              │
│                                       │  │                                       │
│ leafrefs:                             │  │ leafrefs:                             │
│   name ──► PON_SERVICE_CONFIG_        │  │   name ──► PON_SERVICE_CONFIG_        │
│            PROFILE_HEADER/name        │  │            PROFILE_HEADER/name        │
└───────────────────────────────────────┘  └───────────────────────────────────────┘
```

## Table Form

| Table                                                        | Keys            | Leafrefs                                       |
| ------------------------------------------------------------ | --------------- | ---------------------------------------------- |
| PON_SERVICE_CONFIG_PROFILE_HEADER                            | name            | (none)                                         |
| PON_SERVICE_CONFIG_PROFILE_HEADER_COMPATIBILITY_VENDOR_MODEL | vendor-id, name | name -> PON_SERVICE_CONFIG_PROFILE_HEADER/name |
| PON_SERVICE_CONFIG_PROFILE_HEADER_INPUTS_EXT_INPUT           | name, db-ref    | name -> PON_SERVICE_CONFIG_PROFILE_HEADER/name |
