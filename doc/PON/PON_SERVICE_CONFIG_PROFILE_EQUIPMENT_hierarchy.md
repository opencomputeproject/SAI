# PON_SERVICE_CONFIG_PROFILE — EQUIPMENT / DEVICE ME Group

## Graphical Form

```
        ┌──────────────────────────────────────────────────────────────┐
        │  shared key: service-config-profile-name  (plain string)     │
        │  shared key: managed-entity-id            (omci-uint16)      │
        │  no leafrefs anywhere in this group — flat siblings          │
        └──────────────────────────────┬───────────────────────────────┘
                                       │
   ┌──────────┬──────────┬─────────────┼─────────────┬──────────┬──────────┐
   ▼          ▼          ▼             ▼             ▼          ▼          ▼
┌───────┐ ┌─────────┐ ┌──────────┐ ┌───────┐ ┌────────┐ ┌────────┐ ┌──────────────┐
│ ANI_G │ │CARDHOLD-│ │ CIRCUIT_ │ │ OLT_G │ │ ONU_G  │ │ ONU2_G │ │ LARGE_STRING │
│       │ │   ER    │ │   PACK   │ │       │ │        │ │        │ │              │
└───────┘ └─────────┘ └──────────┘ └───────┘ └────────┘ └────────┘ └──────────────┘
                                       │
                    ┌──────────────────┴──────────────────┐
                    ▼                                     ▼
    ┌───────────────────────────────┐    ┌────────────────────────────────────┐
    │  PPTP_RS232_RS485_UNI         │    │  VIRTUAL_ETHERNET_INTERFACE_PT     │
    └───────────────────────────────┘    └────────────────────────────────────┘
```

## Table Form

| Table (prefix `PON_SERVICE_CONFIG_PROFILE_`) | Keys                                           | Leafrefs |
| -------------------------------------------- | ---------------------------------------------- | -------- |
| ANI_G                                        | service-config-profile-name, managed-entity-id | (none)   |
| CARDHOLDER                                   | service-config-profile-name, managed-entity-id | (none)   |
| CIRCUIT_PACK                                 | service-config-profile-name, managed-entity-id | (none)   |
| OLT_G                                        | service-config-profile-name, managed-entity-id | (none)   |
| ONU_G                                        | service-config-profile-name, managed-entity-id | (none)   |
| ONU2_G                                       | service-config-profile-name, managed-entity-id | (none)   |
| LARGE_STRING                                 | service-config-profile-name, managed-entity-id | (none)   |
| PPTP_RS232_RS485_UNI                         | service-config-profile-name, managed-entity-id | (none)   |
| VIRTUAL_ETHERNET_INTERFACE_PT                | service-config-profile-name, managed-entity-id | (none)   |
