# Host Interface Oper Status Update Mode
-------------------------------------------------------------------------------
 Title       | Host Interface Oper Status Update Mode
-------------|-----------------------------------------------------------------
 Authors     | Ashish Singh, Google LLC
 Status      | In review
 Type        | Standards track
 Created     | 02/13/2023

-------------------------------------------------------------------------------

This spec discusses about:
- Capability query of supported modes to update the hostif operational status.
- Setting the mode to update the hostif operational status.

## Overview

When a physical interface's operational status changes, operational status of corresponding host interface needs to be updated. There can be more than one possible paths that may update the hostif oper status. For example: hostif oper status maybe updated by application using the SAI API or internally by SAI adapter itself. This spec defines the modes based on layers controlling those paths such that when one particular mode is set, only corresponding layer is responsible for updating the hostif operational status.

This applies to VLAN hostif and LAG hostif too.

## Spec Enhancement

Following new switch attribute is introduced to support the modes:
```
typedef enum _sai_switch_attr_t
{
.
.
    /**
     * @brief Set hostif operational status update mode.
     *
     * This will set layer responsible for updating the operational status of
     * hostif.
     *
     * @type sai_switch_hostif_oper_status_update_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_SWITCH_HOSTIF_OPER_STATUS_UPDATE_MODE_APPLICATION
     */
    SAI_SWITCH_ATTR_HOSTIF_OPER_STATUS_UPDATE_MODE,

.
.
} sai_switch_attr_t;
```

where `sai_switch_hostif_oper_status_update_mode_t` is defined as:
```
/**
 * @brief Attribute data for SAI_SWITCH_ATTR_HOSTIF_OPER_STATUS_UPDATE_MODE.
 */
typedef enum _sai_switch_hostif_oper_status_update_mode_t
{
    /**
     * In this mode, operational status of hostif must be updated by application
     * using hostif API with SAI_HOSTIF_ATTR_OPER_STATUS attribute. SAI adapter
     * should not update the hostif operational status internally.
     * When a host interface is created, application must update the operational
     * status if required and should not rely on SAI adapter to update it.
     */
    SAI_SWITCH_HOSTIF_OPER_STATUS_UPDATE_MODE_APPLICATION = 0,

    /**
     * In this mode, operational status of hostif is updated internally by SAI
     * adapter. Update of hostif operational status by application using hostif
     * API with SAI_HOSTIF_ATTR_OPER_STATUS is ignored.
     */
    SAI_SWITCH_HOSTIF_OPER_STATUS_UPDATE_MODE_SAI_ADAPTER = 1,

} sai_switch_hostif_oper_status_update_mode_t;
```

### Set a mode

Using the switch object `set` API, mode to update the hostif operational status can be set.

Following are the possible scenarios for SAI adapter to handle:
- If SAI adapter doesn't support the attribute `SAI_SWITCH_ATTR_HOSTIF_OPER_STATUS_UPDATE_MODE`, it must return error `SAI_STATUS_NOT_SUPPORTED`.
- If SAI adapter doesn't support the mode that is requested to be set, it must return error `SAI_STATUS_NOT_SUPPORTED`.
- If SAI adapter is able to successfuly set the request mode, it must return `SAI_STATUS_SUCCESS`.

### Checking hostif oper status update mode capability

Application can query the support for oper status update modes using `sai_query_attribute_enum_values_capability` and SAI will return the list of implemented modes.

Example:

```
sai_s32_list_t        capabilities_list;
vector<int32_t>       capabilities(5);
capabilities_list.count = 5;
capabilities_list.list = capabilities.data();
if (sai_query_attribute_enum_values_capability(switch_id, SAI_OBJECT_TYPE_SWITCH,
                                               SAI_SWITCH_ATTR_HOSTIF_OPER_STATUS_UPDATE_MODE,
                                               &capabilities_list) == SAI_STATUS_SUCCESS) {

  for (uint32_t idx = 0; idx < capabilities_list.count; ++idx) {
    printf("Mode %d supported", capabilities_list.list[idx]);
  }
}
```
