# [SAI] Port Storm Control Enhancement
-------------------------------------------------------------------------------
 Title       | Port storm control enhancement
-------------|-----------------------------------------------------------------
 Authors     | Rajesh Perumal (Marvell)
 Status      | Reviewed on 26-Feb-2026
 Type        | Standards track
 Created     | 24-Feb-2026
 SAI-Version | 1.18
-------------------------------------------------------------------------------

## Introduction


There is a long‑standing mismatch between the SAI storm‑control attribute definitions and how these attributes are interpreted within SONiC. The existing SAI attributes for flood and multicast storm control do not clearly differentiate among unknown unicast, unknown multicast, and known multicast traffic. As a result, vendors and SONiC implementations interpret these attributes differently, leading to inconsistent behavior across platforms.
To improve clarity and alignment, two approaches can be followed:

To improve clarity and alignment, new, explicit attributes are introduced for each traffic class and the ambiguous ones are deprecated. This resolves the ambiguity present in the current specification and enables consistent implementation across vendors and NOSes.
The following section details the selected enhancement approach and the required SAI header updates.

## Motivation

The table below highlights the gap between the current SAI attribute descriptions and how these attributes are interpreted within SONiC:

| Existing SAI Attribute                          | SAI Description                                | SONiC usage                  |
|-------------------------------------------------|------------------------------------------------|------------------------------|
| SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID    | Unknown unicast/unknown multicast flood control| Unknown unicast flood control|
| SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID| Multicast storm control policer on port        | Unknown multicast traffic    |

This mismatch creates ambiguity, leads to inconsistent vendor implementations, and results in unclear handling of traffic classes (unknown unicast, unknown multicast, and known multicast). To address this, either the existing SAI attribute definitions must be refined, or new traffic‑specific attributes should be introduced. However, redefining current SAI attributes may introduce backward‑compatibility issues.

By defining explicit attributes, SAI can present storm‑control behavior more clearly across all network operating systems (including SONiC), reduce implementation confusion, and support future enhancements in storm‑control processing.


## SAI Enhancement

This enhancement proposes to deprecate the existing storm control attributes SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID and SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID, and replace it with dedicated, traffic‑specific attributes for unknown unicast, known unicast, unknown multicastand known multicast storm control. This provides clear separation of behavior and removes the ambiguity present in the current definition.

### Deprecated Attributes
```
--- Old Code
+++ New Code
 /**
  * @brief Enable flood (unknown unicast or unknown multicast)
  * storm control policer on port.
+ * Deprecated. Use SAI_PORT_ATTR_UNKNOWN_UNICAST_STORM_CONTROL_POLICER_ID and SAI_PORT_ATTR_UNKNOWN_MULTICAST_STORM_CONTROL_POLICER_ID
  *
  * Set policer id = #SAI_NULL_OBJECT_ID to disable policer on port.
  *
  * @type sai_object_id_t
  * @flags CREATE_AND_SET
  * @objects SAI_OBJECT_TYPE_POLICER
  * @allownull true
  * @default SAI_NULL_OBJECT_ID
+ * @deprecated true
  */
 SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID,
```
```
--- Old Code
+++ New Code
 /**
  * @brief Enable multicast storm control policer on port.
+ * Deprecated. Use SAI_PORT_ATTR_KNOWN_MULTICAST_STORM_CONTROL_POLICER_ID and SAI_PORT_ATTR_UNKNOWN_MULTICAST_STORM_CONTROL_POLICER_ID
  *
  * Set policer id = #SAI_NULL_OBJECT_ID to disable policer on port.
  *
  * @type sai_object_id_t
  * @flags CREATE_AND_SET
  * @objects SAI_OBJECT_TYPE_POLICER
  * @allownull true
  * @default SAI_NULL_OBJECT_ID
+ * @deprecated true
  */
 SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID,
```

### New port storm control attributes
```
    /**
     * @brief Enable unknown unicast storm control policer on port.
     *
     * Set policer id = #SAI_NULL_OBJECT_ID to disable policer on port.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_POLICER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_UNKNOWN_UNICAST_STORM_CONTROL_POLICER_ID,

    /**
     * @brief Enable known unicast storm control policer on port.
     *
     * Set policer id = #SAI_NULL_OBJECT_ID to disable policer on port.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_POLICER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_KNOWN_UNICAST_STORM_CONTROL_POLICER_ID,

    /**
     * @brief Enable unknown multicast storm control policer on port.
     *
     * Set Policer id = #SAI_NULL_OBJECT_ID to disable policer on port.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_POLICER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_UNKNOWN_MULTICAST_STORM_CONTROL_POLICER_ID,

    /**
     * @brief Enable known multicast storm control policer on port.
     *
     * Set Policer id = #SAI_NULL_OBJECT_ID to disable policer on port.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_POLICER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_KNOWN_MULTICAST_STORM_CONTROL_POLICER_ID,

```

This approach provides more granular storm‑control handling per traffic type.
