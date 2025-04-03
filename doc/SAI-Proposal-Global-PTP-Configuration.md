# Global PTP Configuration in SAI Switch

Title       | Global PTP Configuration in SAI Switch
------------|----------------------------------------
Authors     | Open
Status      | Draft
Type        | Standards track
Created     | 2024-03-03
SAI-Version | 1.15

## Overview
Introducing a PTP (Precision Time Protocol) attribute to the SAI switch object, allowing for switch-wide PTP mode settings that affect all ports on the switch. 

A switch that does not set this attribute will observe no change in behavior.

## Motivation
Currently, PTP configuration in SAI is port-based, and many switches only support configuration of PTP on a global basis.
In addition, many existing switches have a configuration model which allows global configuration of PTP, and then override on a port level.  SAI should follow this flexible example.

## Technical Specification

### Updated description of existing PORT PTP type - the main comment is new
```c
/**
 * @brief PTP mode
 * 
 * These modes can be used at the port and switch level.
 * All ports use the value set at the switch level unless explicitly configured
 * at the port level to a value other than SAI_PORT_PTP_MODE_NONE.
 */
typedef enum _sai_port_ptp_mode_t
{
    /** No special processing for PTP packets */
    SAI_PORT_PTP_MODE_NONE,

    /** Single-step Timestamp mode for the PTP packets */
    SAI_PORT_PTP_MODE_SINGLE_STEP_TIMESTAMP,

    /** Two-step Timestamp mode for the PTP packets */
    SAI_PORT_PTP_MODE_TWO_STEP_TIMESTAMP,

} sai_port_ptp_mode_t;


### New Switch Attribute
```c
typedef enum _sai_switch_attr_t
{
    // ... existing attributes ...

    /**
     * @brief Global PTP mode configuration
     *
     * Global PTP mode configuration for the switch.
     * Applies to all ports unless overridden by port-specific settings.
     *
     * @type sai_port_ptp_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_PTP_MODE_NONE
     */
    SAI_SWITCH_ATTR_PORT_PTP_MODE,

    // ... existing attributes ...
} sai_switch_attr_t;
```

## Usage Example
```c
// Set global PTP configuration at switch level
sai_attribute_t attr;
attr.id = SAI_SWITCH_ATTR_PORT_PTP_MODE;
attr.value.s32 = SAI_PORT_PTP_MODE_SINGLE_STEP_TIMESTAMP;

sai_status_t status = sai_switch_api->set_switch_attribute(
    switch_id,
    &attr);

// Get global PTP configuration
attr.id = SAI_SWITCH_ATTR_PORT_PTP_MODE;
status = sai_switch_api->get_switch_attribute(
    switch_id,
    1,
    &attr);
```

## References
1. IEEE 1588-2008 Standard for a Precision Clock Synchronization Protocol for Networked Measurement and Control Systems
2. IEEE 1588-2019 Standard for a Precision Clock Synchronization Protocol for Networked Measurement and Control Systems 