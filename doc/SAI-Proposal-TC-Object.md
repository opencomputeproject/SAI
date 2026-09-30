# [SAI] Generic Traffic Class (TC) Object
-------------------------------------------------------------------------------
 Title       | Generic Traffic Class (TC) Object
-------------|-----------------------------------------------------------------
 Authors     | Praneeth Chippada, Ravindranath C K (Marvell)
 Status      | In review
 Type        | Standards track
 Created     | 2025-11-22
 SAI-Version | 1.18
-------------------------------------------------------------------------------


## 1. Introduction

This proposal introduces a **Traffic Class (TC) object** in SAI as a configuration object that can be created and managed by applications to enable or disable features on a per-TC basis on demand. The TC object acts as an anchor for traffic-class specific functionality without altering existing QoS classification mechanisms, ensuring full backward compatibility with current SAI implementations.

The TC object is designed to provide extensibility for new capabilities at the traffic-class granularity. Initial use cases include:
- Per-TC BUM flood control
- TAM object binding for per-TC

For details on the per-traffic-class enhancements (BUM Flood Control and TAM Bind Point Support), refer to the companion document:
SAI-Proposal-Per-TC-Enhancements.md


## 2. Abbreviations and Information


|   **Term**   | **Definition**                               |
| ------------ | -------------------------------------------- |
| TC           | Traffic Class                                |
| TAM          | Telemetry, Analytics, and Monitoring         |
| BUM          | Broadcast, Unknown Unicast, Multicast        |


## 3. Motivation
- Provide a TC level object to attach different features and functionality.
- Provide extensibility for new functions/capability/actions per TC and remain backward compatible.


## 4. SAI Enhancement
### TC object and respective attributes
A new traffic class object is introduced to configure per TC actions at a switch level.This object uses the TC index to identify the traffic class. This object is created when the application needs to enable/disable features on a per TC granularity across the switch.
+ Note: TC object is a config object which can be created any time and is independent of the TC related QoS maps that are already supported in the SAI. In other words, the existing QoS maps continue to use the TC id and not this new TC object. This ensures backward compatibility and requires no changes in existing SAI implementations where these features (per TC flood control or per TC TAM objects) are not supported.

    ```c
    /* New object type */
    SAI_OBJECT_TYPE_TC
    ```

   ```c
   /**
    * @brief Enum defining TC attributes
    */
   typedef enum _sai_tc_attr_t
   {
      ...
      /**
       * @brief Start of attributes
       */
      SAI_TC_ATTR_START,
      /**
       * @brief Traffic Class index
       *
       * @type sai_uint8_t
       * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
       */
      SAI_TC_ATTR_INDEX = SAI_TC_ATTR_START,


      SAI_TC_ATTR_END,
      ...
   } sai_tc_attr_t;
   ```
### Switch Attributes:
New Attributes are introduced in switch to get the TC list.
   ```c
   /**
    * @brief Attribute Id in sai_set_switch_attribute() and
    * sai_get_switch_attribute() calls.
    */
   typedef enum _sai_switch_attr_t
   {
      ...
      /**
       * @brief List of TCs on the switch
       *
       * @type sai_object_list_t
       * @flags READ_ONLY
       * @objects SAI_OBJECT_TYPE_TC
       */
      SAI_SWITCH_ATTR_TC_LIST,

      ...
   } sai_switch_attr_t;
   ```


## 5. API Example

1. Create TC object with respective TC index.
2. Read the configured TCs from the Switch.
3. Remove the TC object.


### 5.1 Create TC object with respective TC index.

```c
/* Create TC object with index 4 and enable flood control */
attr_count = 0;
sai_attr_list[attr_count].id = SAI_TC_ATTR_INDEX;
sai_attr_list[attr_count++].value.u32 = 4;


sai_create_tc_fn(
   &tc_oid,
   switch_id,
   attr_count,
   sai_attr_list);
```

### 5.2 Read the configured TCs from the Switch.

```c
sai_attr_list.id = SAI_SWITCH_ATTR_TC_LIST;

status = sai_get_switch_attribute_fn(
            switch_id,
            attr_count,
            sai_attr_list);

if (status == SAI_STATUS_BUFFER_OVERFLOW) {
    /* sai_attr_list.value.objlist.count now holds required size */
    tc_oids = malloc(sizeof(sai_object_id_t) * sai_attr_list.value.objlist.count );
    sai_attr_list.value.objlist.list = tc_oids;
    sai_attr_list.value.objlist.count = sai_attr_list.value.objlist.count;
    status = sai_get_switch_attribute_fn(
                switch_id,
                attr_count,
                sai_attr_list);
}
if (status != SAI_STATUS_SUCCESS) {
    // handle error
}
```

### 5.3 Remove the configured TC object.

```c
/* Remove the TC object */
sai_remove_tc_fn(tc_oid);
```
