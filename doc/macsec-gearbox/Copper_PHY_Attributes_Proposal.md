# External PHY Copper Attributes

# Proposal

| **Title** | **External PHY Abstraction Interface** |
| --- | --- |
| **Authors** | **Sidharaj Ukidve, Broadcom** |
| **Status** | **Draft** |
| **Type** |  |
| **Created** | **6/3/2021** |
| **SAI-Version** | **1.8.1** |

# Overview

The purpose of this document is to describe External PHY attributes needed to support Copper PHYs

## MDIX Mode and Status

During copper forced-speed mode, one end of the link must perform an MDI crossover so that each transceiver&#39;s transmitter is connected to the other receiver. PHY can perform an automatic media-dependent interface crossover, eliminating the need for crossover cables or cross-wired ports. SAI\_PORT\_ATTR\_MDIX\_MODE\_CONFIG attribute can be used to set auto, crossover, or straight mode and SAI\_PORT\_ATTR\_MDIX\_MODE\_STATUS can be used to read the current status of the MDIX mode.

Following are the enums and attributes defined to support MDIX mode and status:
```
/**
 * @brief Attribute data for #SAI_PORT_ATTR_MDIX_MODE_STATUS
 * Used for MDIX mode status
 */
typedef enum _sai_port_mdix_mode_status_t
{
    /** MDIX mode status straight */
    SAI_PORT_MDIX_MODE_STATUS_STRAIGHT,

    /**  MDIX mode status cross over */
    SAI_PORT_MDIX_MODE_STATUS_CROSSOVER
} sai_port_mdix_mode_status_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_MDIX_MODE_CONFIG
 * Used for MDIX mode configuration
 */
typedef enum _sai_port_mdix_mode_config_t
{
    /** MDIX mode status auto */
    SAI_PORT_MDIX_MODE_CONFIG_AUTO,

    /** MDIX mode status straight */
    SAI_PORT_MDIX_MODE_CONFIG_STRAIGHT,

    /**  MDIX mode status cross over */
    SAI_PORT_MDIX_MODE_CONFIG_CROSSOVER
} sai_port_mdix_mode_config_t;

/**
 * @brief MDIX mode status for the port
 *
 * @type sai_port_mdix_mode_status_t
 * @flags READ_ONLY
 */
SAI_PORT_ATTR_MDIX_MODE_STATUS,

/**
 * @brief MDIX mode configuration for the port
 *
 * @type sai_port_mdix_mode_config_t
 * @flags CREATE_AND_SET
 * @default SAI_PORT_MDIX_MODE_CONFIG_AUTO
 */
SAI_PORT_ATTR_MDIX_MODE_CONFIG,

```
## Master/Slave Auto-negotiation Configuration

In 1000BASE-T mode, one end of the link must be configured as the timing master and the other end as the slave. Master/slave configuration is performed by the auto-negotiation function. The auto-negotiation function first looks at the manual master/slave configuration modes advertised by the local PHY and the link partner. If both ends of the link attempt to force the same manual configuration (both master or both slave), PHY will try to link up and restart auto-negotiation if needed. SAI\_PORT\_ATTR\_AUTO\_NEG\_CONFIG\_MODE can be used to set the PHY either in Master, Slave, Auto mode (defer to hardware) or disable auto-negotiation mode.

```
/**
 * @brief Attribute data for #SAI_PORT_ATTR_AUTO_NEG_CONFIG_MODE
 * Used for auto negotiation mode to configure master or slave mode
 */
typedef enum _sai_port_auto_neg_config_mode_t
{
    /** Auto neg configuration mode disabled */
    SAI_PORT_AUTO_NEG_CONFIG_MODE_DISABLED,

    /** Auto neg mode auto */
    SAI_PORT_AUTO_NEG_CONFIG_MODE_AUTO,

    /** Auto neg mode slave */
    SAI_PORT_AUTO_NEG_CONFIG_MODE_SLAVE,

    /** Auto neg mode master */
    SAI_PORT_AUTO_NEG_CONFIG_MODE_MASTER
} sai_port_auto_neg_config_mode_t;

/**
 * @brief Configure auto negotiation configuration mode for the port
 *
 * @type sai_port_auto_neg_config_mode_t
 * @flags CREATE_AND_SET
 * @default SAI_PORT_AUTO_NEG_CONFIG_MODE_DISABLED
 * @validonly SAI_PORT_ATTR_AUTO_NEG_MODE == true
 */
SAI_PORT_ATTR_AUTO_NEG_CONFIG_MODE,
```
## 1000BaseX and SGMII Slave Autodetect

When set to true, PHY device can automatically switch between 1000BaseX and SGMII-Slave mode and 1000Base-X or SGMII-Slave module can get a link up without modifying PHY configurations. This attribute is only valid for Fiber interfaces and when the speeds are 1G or 100M.

```
/**
 * @brief Enable auto detection between 1000X and SGMII slave mode
 *
 * @type bool
 * @flags CREATE_AND_SET
 * @default false
 * @validonly SAI_PORT_ATTR_MEDIA_TYPE == SAI_PORT_MEDIA_TYPE_FIBER and SAI_PORT_ATTR_SPEED == 1000
 */
SAI_PORT_ATTR_1000X_SGMII_SLAVE_AUTODETECT,

```

## Fiber Module Type

SAI_PORT_ATTR_MODULE_TYPE allows user to force various module/transceivers types when fiber interface is active. Supported fiber interfaces are 1000BASE-X, SGMII-SLAVE, and 100FX.

```
/**
 * @brief Attribute data for #SAI_PORT_ATTR_MODULE_TYPE
 * Used for configuring Fiber module type
 */
typedef enum _sai_port_module_type_t
{
    /** Module Type Fiber */
    SAI_PORT_MODULE_TYPE_1000BASE_X,

    /** Module Type 100FX */
    SAI_PORT_MODULE_TYPE_100FX,

    /** Module Type SGMII-Slave */
    SAI_PORT_MODULE_TYPE_SGMII_SLAVE,
} sai_port_module_type_t;

/**
 * @brief Configure Fiber module type
 *
 * @type sai_port_module_type_t
 * @flags CREATE_AND_SET
 * @default SAI_PORT_MODULE_TYPE_1000BASE_X
 * @validonly SAI_PORT_ATTR_MEDIA_TYPE == SAI_PORT_MEDIA_TYPE_FIBER and SAI_PORT_ATTR_SPEED == 1000
 */
SAI_PORT_ATTR_MODULE_TYPE,

```

## Dual Media Support

Dual media support allows user to configure Copper and Fiber supported External PHYs. When a setup has both Copper and Fiber media types but only one media type can be active at any given time. SAI_PORT_ATTR_DUAL_MEDIA attribute can be used to either use Copper only, Fiber only, Copper preferred, or Fiber preferred ports. SAI_PORT_DUAL_MEDIA_NONE will be used if the External PHY does not support dual media configuration.

```
/**
 * @brief Attribute data for #SAI_PORT_ATTR_DUAL_MEDIA
 * Used to configure media type for dual media supported PHY
 */
typedef enum _sai_port_dual_media_t
{
    /**  Dual media not supported */
    SAI_PORT_DUAL_MEDIA_NONE,

    /**  Force Copper mode, Fiber is inactive/disabled */
    SAI_PORT_DUAL_MEDIA_COPPER_ONLY,

    /**  Force Fiber mode, Copper is inactive/disabled */
    SAI_PORT_DUAL_MEDIA_FIBER_ONLY,

    /**  Both Copper and Fiber supported, but Copper preferred */
    SAI_PORT_DUAL_MEDIA_COPPER_PREFERRED,

    /**  Both Copper and Fiber supported, but Fiber preferred */
    SAI_PORT_DUAL_MEDIA_FIBER_PREFERRED
} sai_port_dual_media_t;

/**
 * @brief Configure media types for dual media supported PHY
 *
 * @type sai_port_dual_media_t
 * @flags CREATE_AND_SET
 * @default SAI_PORT_DUAL_MEDIA_NONE
 */
SAI_PORT_ATTR_DUAL_MEDIA,

```
