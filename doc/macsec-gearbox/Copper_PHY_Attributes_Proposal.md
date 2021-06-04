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

/\*\*

\* @brief Attribute data for #SAI\_PORT\_ATTR\_MDIX\_MODE\_STATUS

\* Used for MDIX mode status

\*/

typedef enum \_sai\_port\_mdix\_mode\_status\_t

{

/\*\* MDIX mode status straight \*/

SAI\_PORT\_MDIX\_MODE\_STATUS\_STRAIGHT,

/\*\* MDIX mode status cross over \*/

SAI\_PORT\_MDIX\_MODE\_STATUS\_CROSSOVER

} sai\_port\_mdix\_mode\_status\_t;

/\*\*

\* @brief Attribute data for #SAI\_PORT\_ATTR\_MDIX\_MODE\_CONFIG

\* Used for MDIX mode configuration

\*/

typedef enum \_sai\_port\_mdix\_mode\_config\_t

{

/\*\* MDIX mode status auto \*/

SAI\_PORT\_MDIX\_MODE\_CONFIG\_AUTO,

/\*\* MDIX mode status straight \*/

SAI\_PORT\_MDIX\_MODE\_CONFIG\_STRAIGHT,

/\*\* MDIX mode status cross over \*/

SAI\_PORT\_MDIX\_MODE\_CONFIG\_CROSSOVER

} sai\_port\_mdix\_mode\_config\_t;

/\*\*

\* @brief MDIX mode status for the port

\*

\* @type sai\_port\_mdix\_mode\_status\_t

\* @flags READ\_ONLY

\*/

SAI\_PORT\_ATTR\_MDIX\_MODE\_STATUS,

/\*\*

\* @brief MDIX mode configuration for the port

\*

\* @type sai\_port\_mdix\_mode\_config\_t

\* @flags CREATE\_AND\_SET

\* @default SAI\_PORT\_MDIX\_MODE\_CONFIG\_AUTO

\*/

SAI\_PORT\_ATTR\_MDIX\_MODE\_CONFIG,

## Master/Slave Auto-negotiation Configuration

In 1000BASE-T mode, one end of the link must be configured as the timing master and the other end as the slave. Master/slave configuration is performed by the auto-negotiation function. The auto-negotiation function first looks at the manual master/slave configuration modes advertised by the local PHY and the link partner. If both ends of the link attempt to force the same manual configuration (both master or both slave), PHY will try to link up and restart auto-negotiation if needed. SAI\_PORT\_ATTR\_AUTO\_NEG\_CONFIG\_MODE can be used to set the PHY either in Master, Slave,

Auto mode (defer to hardware) or disable auto-negotiation mode.

/\*\*

\* @brief Attribute data for #SAI\_PORT\_ATTR\_AUTO\_NEG\_CONFIG\_MODE

\* Used for auto negotiation mode to configure master or slave mode

\*/

typedef enum \_sai\_port\_auto\_neg\_config\_mode\_t

{

/\*\* Auto neg mode auto \*/

SAI\_PORT\_AUTO\_NEG\_CONFIG\_MODE\_AUTO,

/\*\* Auto neg mode slave \*/

SAI\_PORT\_AUTO\_NEG\_CONFIG\_MODE\_SLAVE,

/\*\* Auto neg mode master \*/

SAI\_PORT\_AUTO\_NEG\_CONFIG\_MODE\_MASTER

} sai\_port\_auto\_neg\_config\_mode\_t;

/\*\*

\* @brief Configure auto negotiation configuration mode for the port

\*

\* @type sai\_port\_auto\_neg\_config\_mode\_t

\* @flags CREATE\_AND\_SET

\* @default SAI\_PORT\_MDIX\_MODE\_CONFIG\_AUTO

\* @validonly SAI\_PORT\_ATTR\_AUTO\_NEG\_MODE == true

\*/

SAI\_PORT\_ATTR\_AUTO\_NEG\_CONFIG\_MODE,

## 1000BaseX and SGMII Slave Autodetect

When set to true, PHY device can automatically switch between 1000BaseX and SGMII-Slave mode and 1000Base-X or SGMII-Slave module can get a link up without modifying PHY configurations. This attribute is only valid for Fiber interfaces and when the speeds are 1G or 100M.

/\*\*

\* @brief Enable auto detection between 1000X and SGMII slave mode

\*

\* @type bool

\* @flags CREATE\_AND\_SET

\* @default false

\* @validonly SAI\_PORT\_ATTR\_MEDIA\_TYPE == SAI\_PORT\_MEDIA\_TYPE\_FIBER and SAI\_PORT\_ATTR\_SPEED == 1000

\*/

SAI\_PORT\_ATTR\_1000X\_SGMII\_SLAVE\_AUTODETECT,

## EEE Latency Mode

EEE Latency mode allows user to configure a fixed or variable latency when EEE is enabled.

Variable latency can be configured using SAI\_PORT\_ATTR\_EEE\_WAKE\_TIME

/\*\*

\* @brief Configure EEE latency mode

\*

\* False: Fixed latency

\* True: Variable latency

\*

\* @type bool

\* @flags CREATE\_AND\_SET

\* @default false

\*/

SAI\_PORT\_ATTR\_EEE\_LATENCY\_MODE,

## Fiber Module Type

SAI\_PORT\_ATTR\_MODULE\_TYPE allows user to force various module/transceivers types when fiber interface is active. Supported fiber interfaces are 1000BASE-X, SGMII-SLAVE, and 100FX.

/\*\*

\* @brief Attribute data for #SAI\_PORT\_ATTR\_MODULE\_TYPE

\* Used for configuring Fiber module type

\*/

typedef enum \_sai\_port\_module\_type\_t

{

/\*\* Module Type Fiber \*/

SAI\_PORT\_MODULE\_TYPE\_1000BASE\_X,

/\*\* Module Type 100FX \*/

SAI\_PORT\_MODULE\_TYPE\_100FX,

/\*\* Module Type SGMII-Slave \*/

SAI\_PORT\_MODULE\_TYPE\_SGMII\_SLAVE,

} sai\_port\_module\_type\_t;

/\*\*

\* @brief Configure Fiber module type

\*

\* @type sai\_port\_module\_type\_t

\* @flags CREATE\_AND\_SET

\* @default SAI\_PORT\_MODULE\_TYPE\_1000BASE\_X

\* @validonly SAI\_PORT\_ATTR\_MEDIA\_TYPE == SAI\_PORT\_MEDIA\_TYPE\_FIBER and SAI\_PORT\_ATTR\_SPEED == 1000

\*/

SAI\_PORT\_ATTR\_MODULE\_TYPE,

## Dual Media Support

Dual media support allows user to configure Copper and Fiber supported External PHYs. When a setup has both Copper and Fiber media types but only one media type can be active at any given time. SAI\_PORT\_ATTR\_DUAL\_MEDIA attribute can be used to either use Copper only, Fiber only, Copper preferred, or Fiber preferred ports. SAI\_PORT\_DUAL\_MEDIA\_NONE will be used if the External PHY doesn&#39;t support dual media configuration.

/\*\*

\* @brief Attribute data for #SAI\_PORT\_ATTR\_DUAL\_MEDIA

\* Used to configure media type for dual media supported PHY

\*/

typedef enum \_sai\_port\_dual\_media\_t

{

/\*\* Dual media not supported \*/

SAI\_PORT\_DUAL\_MEDIA\_NONE,

/\*\* Force Copper mode, Fiber is inactive/disabled \*/

SAI\_PORT\_DUAL\_MEDIA\_COPPER\_ONLY,

/\*\* Force Fiber mode, Copper is inactive/disabled \*/

SAI\_PORT\_DUAL\_MEDIA\_FIBER\_ONLY,

/\*\* Both Copper and Fiber supported, but Copper preferred \*/

SAI\_PORT\_DUAL\_MEDIA\_COPPER\_PREFERRED,

/\*\* Both Copper and Fiber supported, but Fiber preferred \*/

SAI\_PORT\_DUAL\_MEDIA\_FIBER\_PREFERRED

} sai\_port\_dual\_media\_t;

/\*\*

\* @brief Configure media types for dual media supported PHY

\*

\* @type sai\_port\_dual\_media\_t

\* @flags CREATE\_AND\_SET

\* @default SAI\_PORT\_DUAL\_MEDIA\_NONE

\*/

SAI\_PORT\_ATTR\_DUAL\_MEDIA,
