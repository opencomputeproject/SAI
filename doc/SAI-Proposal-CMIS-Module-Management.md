# CMIS Module Management
-------------------------------------------------------------------------------
 Title       | SAI support for CMIS Module Management
-------------|-----------------------------------------------------------------
 Authors     | Nvidia
 Status      | In review
 Type        | Standards track
 Created     | 07/25/2024
 SAI-Version | 1.13
-------------------------------------------------------------------------------

# 1. Introduction

Move to the software (SONIC)-based management of optical modules supporting the CMIS management on more vendor platforms requires some additions in SAI API

The current proposal introduces 2 additions:

- New Port SERDES attributes
- Enhancement of synchronization between ASIC port and module configuration

# 2. Addition of new Port SERDES attributes

Port SERDES attributes are used with SAI\_OBJECT\_TYPE\_PORT\_SERDES SAI object to set the Signal Integrity configuration. Few attributes are already defined and used today on some vendor platforms. Few more attributes shall be added to support additional vendor platforms

```
    /**
     * @brief Port serdes control TX PAM4 ratio
     *
     * Ratio between the central eye to the upper and lower eyes (for PAM4 only)
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_PAM4_RATIO,

    /**
     * @brief Port serdes control TX OUT common mode
     *
     * Output common mode
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_OUT_COMMON_MODE,

    /**
     * @brief Port serdes control TX PMOS common mode
     *
     * Output buffers input to Common mode PMOS side
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_PMOS_COMMON_MODE,

    /**
     * @brief Port serdes control TX NMOS common mode
     *
     * Output buffers input to Common mode NMOS side
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_NMOS_COMMON_MODE,

    /**
     * @brief Port serdes control TX PMOS voltage regulator
     *
     * Voltage regulator to pre output buffer PMOS side
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_PMOS_VLTG_REG,

    /**
     * @brief Port serdes control TX NMOS voltage regulator
     *
     * Voltage regulator to pre output buffer NMOS side
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_NMOS_VLTG_REG,

    /**
     * @brief Port serdes control TX pre-coding value
     *
     * TX pre-coding value (used for PAM4 links)
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_PRECODING,

    /**
     * @brief Port serdes control RX pre-coding value
     *
     * RX pre-coding value (used for PAM4 links)
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_RX_PRECODING,
```

# 3. Enhancement of synchronization between ASIC and module configuration
Configuration of the ASIC side (port SERDES) is handled by the SWSS docker (Ports Orch Agent) interacting with the vendor SAI via SAI calls whereas the modules supporting the CMIS protocol are configured by PMON docker (Xcvrd daemon). These initialization processes should be synchronized and the configuration of CMIS module should start only after the ASIC is initialized and per CMIS 5.2 it started sending the high-speed signal toward a module.

Currently, SONIC uses the *"host\_tx\_ready"*  flag in the PORT table in STATE DB for synchronization. This flag is set by Ports OA right after the SAI API for setting the Admin status to UP returns with OK/Success status. PMON registers for changes of this flag in Redis DB and starts the CMIS initialization  for a particular module when this flag is set

See the current flow below:

![The Current Flow](./figures/The-Current-Flow.png)

## 3.1. Problem statement
With move to SW-based management of CMIS modules when the ASIC side is configured by vendor SDK and module side - by SONIC, some problems have been identified with the current approach
### 3.1.1. Host TX Ready signal
As mentioned earlier, currently SONIC assumes that as soon as the Admin Status is set to UP and the corresponding SAI call returns with OK/SUCCESS status, the "host\_tx\_ready" flag can be set in the STATE DB to trigger the CMIS State Machine for specific port. But it is not always a truth as the process of ASIC port initialization takes some time and this time can increase with move to new transceiver technologies (e.g. BiDi). So, in some cases the module initialization can be triggered too early, before the high-speed signal started to be transmitted by ASIC to a module 
### 3.1.2. Unterminated transmission
With move to the SW-based module  management the module presence is handled by SONIC. The FW might be unaware of the module presence status. In this case, when the Admin status of a port is set to UP the FW can start transmitting the high-speed signal even without a plugged-in module. Such unterminated transmission can cause cross-talks to adjacent ports, high EMI, high BER and eventually shorten the transceiver lifetime so it is recommended that ASIC will not start send the high-speed signal before a module is plugged
## 3.2. New approach
To provide the response to the described problem statements SONIC should do the following:

1. Control transmitting of the high-speed signal based on module presence (allow this signal only when a module is plugged-in)
2. Trigger the module initialization (using the CMIS state machine) only when the high-speed signal is transmitted by ASIC towards a module

SWSS shall allow transmitting of the high-speed signal on receiving the INSERTION indication from PMON. Then on setting the Admin Status to UP the vendor SDK/FW shall start transmitting this signal to a module and shall report about that to SONIC.

ASIC FW shall start transmitting the high-speed TX signal only when **both** conditions are met:

1. It is allowed (by SWSS) to transmit this signal 
2. Port mapped to the module is set to Admin UP

See the high-level flow below (details will be provided in the following sub-sections):

![The High Level Flow](./figures/The-High-Level-Flow.png)

## 3.2. Changes in SAI AP

### 3.2.1. host_tx_signal

SONIC shall  inform the Vendor SDK of allowance to transmit the high-speed signal via setting of a new SAI attribute *SAI\_PORT\_ATTR\_HOST\_TX\_SIGNAL\_ENABLE.* 2 values shall be set – enabled (1 - on insertion event) and disabled (0 – on removal event)

```
    /**  
     * @brief Enable host_tx_signal (high-speed signal from ASIC to module) required
     * to start the CMIS module initialization
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_HOST_TX_SIGNAL_ENABLE,
```

This flow shall be used only on platforms supporting it. For that the capability query for new SAI attribute (*SAI\_PORT\_ATTR\_HOST\_TX\_SIGNAL\_ENABLE*) shall be done on SWSS (Ports Orch Agent) init.

The module's INSERTION/REMOVAL events shall trigger the calling of SAI API on a Port object with *SAI\_PORT\_ATTR\_HOST\_TX\_SIGNAL\_ENABLE* to enable or disable data signal from ASIC to module

![Host Tx Signal Enable Flow](./figures/Host-Tx-Signal-Enable-Flow.png)

It should be noted that the setting of *SAI\_PORT\_ATTR\_HOST\_TX\_SIGNAL\_ENABLE* to TRUE (when a module is plugged) is not sufficient to start the transmission of the high-speed signal towards a module. The SDK should wait until the Admin status of the port mapped to this module is set to UP to start transmitting the high-speed signal.

### 3.2.2.  host_tx_ready
When the ASIC starts transmitting the high-speed signal toward a plugged module the vendor SAI should notify the SONIC (SWSS) of that via a new notification

```
    /**  
     * @brief Host tx ready status
     *
     * It will be used for query and capability query of "host_tx_ready" signal
     *
     * @type sai_port_host_tx_ready_status_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_HOST_TX_READY_STATUS,
```

```
/**
 * @brief Port host tx ready notification
 *
 * Passed as a parameter into sai_initialize_switch()
 *
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 * @objects port_id SAI_OBJECT_TYPE_PORT
 *
 * @param[in] switch_id Switch Id
 * @param[in] port_id Port Id
 * @param[in] host_tx_ready_status New tx ready status
 */
typedef void (*sai_port_host_tx_ready_notification_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_id_t port_id,
        _In_ sai_port_host_tx_ready_status_t host_tx_ready_status);
```

```
    /**
     * @brief Port host tx ready notification callback
     * function passed to the adapter.
     *
     * Use sai_port_host_tx_ready_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_port_host_tx_ready_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_PORT_HOST_TX_READY_NOTIFY,
```
The SWSS shall use this notification to set "*host\_tx\_ready*" flag in STATE DB which will trigger the CMIS initialization of the module. It will ensure that the module initialization doesn't start before the high-speed signal is transmitted by ASIC to a module

The platform capabilities for supporting of this feature should be quired on Port OA init. The notification shall be expected and consumed only on platforms supporting it. On platforms not supporting this functionality the “*host\_tx\_ready*” flag shall be set in STATE DB upon return of Port Admin status UP with SUCCESS return code (backward-compatible behavior)

![Host Tx Ready Flow](./figures/Host-Tx-Ready-Flow.png)

