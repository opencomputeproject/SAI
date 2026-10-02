# PON_SERVICE_CONFIG_PROFILE — IP / TRANSPORT Group

## Graphical Form

```
        ┌──────────────────────────────────────────────────────────────┐
        │  shared key: service-config-profile-name  (plain string)     │
        │  shared key: managed-entity-id            (omci-uint16)      │
        │  no leafrefs anywhere in this group — flat siblings          │
        └──────────────────────────────┬───────────────────────────────┘
                                       │
        ┌──────────────┬───────────────┼───────────────┬──────────────┐
        ▼              ▼               ▼               ▼              ▼
┌───────────────┐ ┌────────────┐ ┌────────────┐ ┌────────────┐ ┌────────────┐
│ IP_HOST_      │ │ IPV6_HOST_ │ │ TCP_UDP_   │ │ SSH_SERVER_│ │ SSH_SERVER_│
│ CONFIG_DATA   │ │ CONFIG_DATA│ │ CONFIG_DATA│ │ OPERATION  │ │ PORT_DATA  │
└───────────────┘ └────────────┘ └────────────┘ └────────────┘ └────────────┘
                                       │
                                       ▼
                    ┌────────────────────────────────────┐
                    │ RS232_RS485_PORT_OPER_CONFIG_DATA  │
                    └────────────────────────────────────┘
```

## Table Form

| Table (prefix `PON_SERVICE_CONFIG_PROFILE_`) | Keys                                           | Leafrefs |
| -------------------------------------------- | ---------------------------------------------- | -------- |
| IP_HOST_CONFIG_DATA                          | service-config-profile-name, managed-entity-id | (none)   |
| IPV6_HOST_CONFIG_DATA                        | service-config-profile-name, managed-entity-id | (none)   |
| TCP_UDP_CONFIG_DATA                          | service-config-profile-name, managed-entity-id | (none)   |
| SSH_SERVER_OPERATION                         | service-config-profile-name, managed-entity-id | (none)   |
| SSH_SERVER_PORT_DATA                         | service-config-profile-name, managed-entity-id | (none)   |
| RS232_RS485_PORT_OPER_CONFIG_DATA            | service-config-profile-name, managed-entity-id | (none)   |
