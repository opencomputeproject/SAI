SAI Infra for distributed HW resource management
-------------------------------------------------------------------------------
 Title       | SAI distributed HW resource management
-------------|-----------------------------------------------------------------
 Authors     | Jai Kumar, Broadcom Inc.
 Status      | In review
 Type        | Standards track
 Created     | 07/08/2022: Initial Draft
 SAI-Version | 1.9
-------------------------------------------------------------------------------

Modern chips have distributed architecture where HW resources are split between ingress and egress stages of the pipleine for scaling and performance reasons.

PR #1482 introduces a method to
1. Specify the stage of a resource
2. Query an object's attribute stage for a given HW using capability API
3. Error handling for incorrect resource stage specification

## Common Data Type
-----------------
New data type sai_object_stage_t is introduced. This data type specifies if a resource is of type

- SAI_OBJECT_STAGE_BOTH:
  Resource is a common shared resource between ingress and egress HW pipeline
- SAI_OBJECT_STAGE_INGRESS:
  Resource belongs to ingress stage of HW pipeline
- SAI_OBJECT_STAGE_EGRESS:
  Resource belongs to egress stage of HW pipeline

## Capability Query
----------------
New  query API is introduced for an attribute's stage. NOS can query the stage of an object's attribute for a given HW support. One of the following stage is returned as part the API
- SAI_ATTR_STAGE_NA: Stage is not applicable
- SAI_ATTR_STAGE_BOTH: Attribute is common to ingress and egress stage
- SAI_ATTR_STAGE_INGRESS: Attribute is applicable only to ingress stage
- SAI_ATTR_STAGE_EGRESS: Attribute is applicabe only to egress stage

Length of the returned array "stage" is attr_count. Caller must provide the buffer for array "stage".
 
```
sai_status_t sai_query_object_stage(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list,
        _Out_ sai_object_stage_t *stage);
```

## HOSTIF Trap Group
-----------------
A new CREATE_ONLY trap group attribute is introduced. NOS can set this attribute to specify the stage of the trap group.
- SAI_HOTIF_TRAP_GROUP_ATTR_OBJECT_STAGE: 
CREATE_ONLY attribute to configure a trap group of type stage ingress/egress/both

## Policer Pool
------------
A new CREATE_ONLY policer attribute is introduced. NOS can set this attribute
to specify the stage of the pool.
- SAI_POLICER_ATTR_STAGE:
  CREATE_ONLY attribute that specifies the stage of the policer pool. Default is kept as SAI_STAGE_BOTH common pool for backward compatibility.

## Example
-------
Hostif trap's are implemented as an ingress or egress pipeline stage based on a HW architecture. In this example workflow, a hostif trap capability is queried. Based on returned value, appropriate trap group and policer pool is assigned to it. For e.g. a trap of type ingress will be assigned a trap group of type ingress using an ingress policer pool.

## Sample Existing Workflow
------------------------
There is no concept of stages in this workflow.
By default this trap is created in SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP trap group.

```
1. create_hostif_trap_group()
2. create_hostif_trap(): 
    By default this trap is created in 
    SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP trap group.
or
2. create_hostif_trap():
    For trap group created in step 1.
3. create_policer()
4. Attach policer to the trap using set of 
    SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER attribute.
```

### Sample New Workflow
-------------------
New workflow will identify if a given trap is applicable to ingress/egress/both. This will be done based on the enhanced capability query API.
```
1. Query the HW capability for a given trap
    sai_status_t sai_query_attribute_capability(
        switch_id,
        SAI_OBJECT_TYPE_HOSTIF_TRAP,
        SAI_HOSTIF_TRAP_TYPE_DNAT_MISS,
        attr_capability);

    SAI adapter will fill attr_capability->attr_stage with appropriate HW pipeline stage where this trap is handled. Lets say give HW supports this attribute in ingress pipeline and will return SAI_ATTR_STAGE_INGRESS as the attr_stage value.

2. Create a policer pool for ingress stage
    sai_object_id_t ingress_policer_id;
    sai_attribute_t ingress_policer_attrs[2];

    ingress_policer_attrs[0].id =
        (sai_attr_id_t)SAI_POLICER_ATTR_METER_TYPE;
    ingress_policer_attrs[0].value =
        SAI_METER_TYPE_PACKETS;

    ingress_policer_attrs[1].id =
        (sai_attr_id_t)SAI_POLICER_ATTR_OBJECT_STAGE;
    ingress_policer_attrs[1].value =
        SAI_OBJECT_STAGE_INGRESS;

    sai_hostif_trap_api->create_policer(
        &ingress_policer_id,
        2,
        ingress_policer_attrs);

3. Create a hostif trap group for ingress stage
    sai_object_id_t trap_group_id;
    sai_attribute_t trap_group_attrs[3];

    trap_group_attrs[0].id =
        (sai_attr_id_t)SAI_HOSTIF_TRAP_GROUP_ATTR_QUEUE;
    trap_group_attrs[0].value.u32 =
        0x10;

    trap_group_attrs[1].id =
        (sai_attr_id_t)SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER,
    trap_group_attrs[1].value.oid =
        ingress_policer_id;

    trap_group_attrs[2].id =
        (sai_attr_id_t)SAI_HOSTIF_TRAP_GROUP_ATTR_OBJECT_STAGE,
    trap_group_attrs[2].value =
        SAI_OBJECT_STAGE_INGRESS;

    sai_hostif_trap_group_api->create_hostif_trap_group(
        &trap_group_id,
        3,
        trap_group_attrs);

4. Create hostif trap for ingress stage
    sai_object_id_t trap_id;
    sai_attribute_t trap_attrs[2];

    trap_attrs[0].id =
        (sai_attr_id_t) SAI_HOSTIF_TRAP_ATTR_TRAP_TYPE;
    trap_attrs[0].value =
        SAI_HOSTIF_TRAP_TYPE_DNAT_MISS;

    trap_attrs[1].id =
        (sai_attr_id_t)SAI_HOSTIF_TRAP_ATTR_TRAP_GROUP;
    trap_attrs[1].value.oid =
        trap_group_id;

    sai_hostif_trap_api->create_hostif_trap(
        &trap_id,
        2,
        trap_attrs);
```

### Error Handling
--------------
New error codepoint SAI_STATUS_MISMATCH is introduced. SAI adapter must 
return this error status if there is a mismatch between trap group, trap 
and policer pool.
- If trap group and trap type is of different stage e.g. trap group is of 
  SAI_OBJECT_STAGE_INGRESS and trap capability is SAI_ATTR_STAGE_EGRESS
- If trap group and policer pool is of different stage