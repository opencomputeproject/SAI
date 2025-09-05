
SAI - Linear Pluggable Optics (LPO) support
============================================

| Title       | SAI LPO attributes         |
|-------------|----------------------------|
| Authors     | Pulla Rao, Broadcom Inc    |
| Status      | In Review                  |
| Type        | Standards Track            |
| Created     | 06/09/2025                 |
| SAI-Version | 1.16                       |

### 1. Overview
---------------
LPO optics are designed specifically to meet the data center requirements for low power consumption, low cost, low latency, compact form factor, reach up to 500meters. 

This document defines the port serdes attributes used to tune the Linear Pluggable Optics (LPO). At a port level, the attributes define the non-linear compensation percentage values, Error Correcting Decoder (ECD) state and Tx/Rx polarity settings. 

### 2. Specification
--------------------

#### 2.1 Changes to saiport.h
-----------------------------
    /**
     * @brief Serdes Rx Error Correcting Decoder/Maximum Likelihood
     * Sequence Estimation control state
     */
    typedef enum _sai_port_serdes_rx_ecd_mlse_state_t
    {
        /** Disable */
	SAI_PORT_SERDES_RX_ECD_MLSE_STATE_DISABLE,
        /** Enable */
        SAI_PORT_SERDES_RX_ECD_MLSE_STATE_ENABLE
	
    } sai_port_serdes_rx_ecd_mlse_state_t;

    /**
     * @brief Serdes polarity setting value
     */
    typedef enum _sai_port_serdes_polarity_t
    {
        /** Normal polarity */
        SAI_PORT_SERDES_POLARITY_NORMAL,
        /** Inverted polarity */
        SAI_PORT_SERDES_POLARITY_INVERTED
	
    } sai_port_serdes_polarity_t;
		

The following attributes are added to `sai_port_serdes_attr_t`:

    /**
     * @brief Port serdes Tx upper eye non linear compensation percentage value
     *
     * List of port serdes Tx upper eye non linear compensation percentage value
     * The values are of type sai_u32_list_t where the count is number of lanes
     * in a port and the list specifies list of values to be applied to each
     * lane.
     *
     * @type sai_u32_list_t
     * @flags CREATE_AND_SET
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_NLC_PERCENTAGE,

    /**
     * @brief Port serdes Tx lower eye non linear compensation percentage value
     *
     * List of port serdes Tx lower eye non linear compensation percentage value
     * The values are of type sai_u32_list_t where the count is number of lanes
     * in a port and the list specifies list of values to be applied to each
     * lane.
     *
     * @type sai_u32_list_t
     * @flags CREATE_AND_SET
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_NLC_LOWER_EYE_PERCENTAGE,

    /**
     * @brief Port serdes Rx Error Correcting Decoder/Maximum Likelihood
     * Sequence Estimation control
     *
     * To enable/disable Rx ECD for a port with back plane media type.
     *
     * @type sai_s32_list_t sai_port_serdes_rx_ecd_mlse_state_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_SERDES_ATTR_RX_ECD_MLSE_STATE,

     /**
     * @brief Port serdes control for inverted TX polarity setting
     *
     * TX polarity setting value
     * The values are of type sai_s32_list_t where the count is number of lanes in
     * a port and the list specifies list of values to be applied to each lane.
     * This extension is added to support both create and set operations.
     *
     * @type sai_s32_list_t sai_port_serdes_polarity_t
     * @flags CREATE_AND_SET
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_POLARITY,

    /**
     * @brief Port serdes control for inverted RX polarity setting
     *
     * RX polarity setting value
     * The values are of type sai_s32_list_t where the count is number of lanes in
     * a port and the list specifies list of values to be applied to each lane.
     * This extension is added to support both create and set operations.
     *
     * @type sai_s32_list_t sai_port_serdes_polarity_t
     * @flags CREATE_AND_SET
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_RX_POLARITY,

