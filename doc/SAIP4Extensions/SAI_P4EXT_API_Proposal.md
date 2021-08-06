SAI P4EXT API Proposal
=====================

Title       | SAI P4 Extensions Proposal
------------|----------------------
Authors     | Intel.
Status      | Draft
Type        | Standards track
Created     | 03/18/2021
SAI-Version | ?
-------------------------------

# Scope #

This document defines the technical specifications for the API used to support P4 Extensions in Open Compute Project Switch Abstraction Interface (SAI). P4 Extensions can be seen as an abstraction over niche/device specific features. It facilitates programming of these new features via the familiar SAI API model using a single P4 Extension object type. The exact feature enabled via the P4 Extensions itself is outside the scope of this document and SAI P4 Extensions API.

# Overview #

SAI P4 extension introduces a single new SAI object SAI_OBJECT_TYPE_P4EXT_ENTRY of object_type_oid. This object along with its attributes provides an abstraction of target specific configurable features otherwise not described in SAI via the P4 match action paradigm in a feature agnostic manner. In contrast to other SAI objects which are mapped 1:1 to an entity in the SAI Pipeline Networking Object Model, for e.g., VLAN, Neighbor, Mirror etc.; SAI P4 Extensions introduces 1:N mapping between the  SAI_OBJECT_TYPE_P4EXT_ENTRY object and niche features Feature1, Feature2, etc. This new paradigm enables the
1. Ability to add exclusive features
2. Expose device specific capabilities
3. Rapid application prototyping
4. Ability to introduce new features potentially without Recompiling SAI

## Object Dependencies ##

In the current SAI P4 Extension Proposal the newly introduced SAI_OBJECT_TYPE_P4EXT_ENTRY shall remain disconnected from the SAI Pipeline Object Graph. In other words, there shall be no dependency from/to other SAI object to/from SAI_OBJECT_TYPE_P4EXT_ENTRY. Also, the proposal places no restriction on the relative placement of the new features/table in the SAI Pipeline. The implementation and interaction of the new feature with the SAI pipeline is left to each individual vendor.

# API Specification #
This section describes SAI P4 Extension API Proposal

## New header file saip4ext.h ##

The new header file defines interfaces for a single object of type P4EXT_ENTRY. This new object mimics a P4 table entity. The attributes of the object define the various P4 table constructs such as table name, match key, action name and action parameters. Each of these attributes is defined as string type. For SAI implementations that are P4 compatible, these strings can simply be set to corresponding P4 table construct. For SAI implementations that do not express their pipeline in P4, a mapping library can be implemented to map the P4 Extension SAI API calls via some translation logic to corresponding driver APIs. This translation could be static, in which case the SAI recompilation would become necessary to implement a new feature or it could be dynamic.

### SAI P4EXT Entry Attributes ###
*sai_p4ext_entry_attr_t* defines the SAI P4 Extension Attributes. Each of the attributes is of type string. As seen below the attributes mimic P4 table attributes such as table name, match fields (key:value pairs), action field (key:value pairs), action parameters (key:value pairs). The format for each of these string attribute values will be described later in the API usage section.

```cpp
/**
 * @brief Attribute Id for P4 ext */
typedef enum _sai_p4ext_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_P4EXT_ENTRY_ATTR_START,

    /**
     * @brief SAI P4 EXT table id
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_P4EXT_ENTRY_ATTR_TABLE_ID = SAI_P4EXT_ENTRY_ATTR_START,

    /**
     * @brief SAI P4 EXT Match field
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_P4EXT_ENTRY_ATTR_MATCH_FIELD_ID,

    /**
     * @brief SAI P4 EXT Action id
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_P4EXT_ENTRY_ATTR_ACTION_ID,

    /**
     * @brief SAI P4 EXT Action parameters
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_P4EXT_ENTRY_ATTR_PARAMETER_ID,

    /**
     * @brief End of attributes
     */
    SAI_P4EXT_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_P4EXT_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_P4EXT_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_p4ext_entry_attr_t;
```


### SAI P4 Extension APIs ###

```cpp
/**
 * @brief Create an P4 table entry
 *
 * @param[out] p4ext_entry_id The P4 table id
 * @param[in] switch_id The Switch Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_p4ext_entry_fn)(
        _Out_ sai_object_id_t *p4ext_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete an P4 entry
 *
 * @param[in] p4ext_entry_id The P4 table id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_p4ext_entry_fn)(
        _In_ sai_object_id_t p4ext_entry_id);

/**
 * @brief Set P4 Table entry attribute
 *
 * @param[in] p4ext_entry_id The P4 table id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_p4ext_entry_attribute_fn)(
        _In_ sai_object_id_t p4ext_entry_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get P4 entry attribute
 *
 * @param[in] p4ext_entry_id P4 table id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_p4ext_entry_attribute_fn)(
        _In_ sai_object_id_t p4ext_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);
```

### SAI P4 Extension API Table ###

```cpp
/**
 * @brief P4Ext  methods table retrieved with sai_api_query()
 **/
typedef struct _sai_p4ext_api_t
{
    sai_create_p4ext_entry_fn                     create_p4ext_entry;
    sai_remove_p4ext_entry_fn                     remove_p4ext_entry;
    sai_set_p4ext_entry_attribute_fn              set_p4ext_entry_attribute;
    sai_get_p4ext_entry_attribute_fn              get_p4ext_entry_attribute;
} sai_p4ext_api_t;

```

# API Usage #

Consider a scenario where a Vendor A wishes to extend the SAI pipeline functionality with a custom feature which can be modeled with a P4 match-action table but is not yet supported in SAI. Let say this new feature classifies incoming packets based on packet field tuples SIP,DIP and sets the traffic class tc. As mentioned earlier the proposal does not deal with the implementation details of data pipeline by each individual vendor, thus the relative position of this table in the SAI pipeline is not part of the specification and is vendor dependent. It's up to the vendor to decide whether this table will come after the SAI QoS tables or before and resolve any conflict that would arise from the actions of the QoS tables and the new P4 Extension Table. This simple feature can be expressed in P4 as below.

```
table flow_classification {
    action set_tc(switch_tc_t tc) {
        ig_md.qos.etrap_tc = tc;
    }

    key = {
        hdr.ipv4.src_addr : ternary @name("src_addr");
        hdr.ipv4.dst_addr : ternary @name("dst_addr");
    }
    actions = {
        set_tc;
    }
}
```

The above P4 table - *flow_classification*, has two key/ match fields.
1. *src_addr* - This is the incoming packet IPv4 Source Address
2. *dst_addr* - This is the incoming packet IPv4 Destination Address
The type of match for both these fields is ternary. In general, the match_kind is expected to be one of the match_kinds
as defined by the P4 core library. More information please refer to the [P4 16
Specification](https://p4.org/p4-spec/docs/P4-16-v1.0.0-spec.html#sec-match-kind-type)
In addition, the above table describes just one possible action - *set_tc*. An incoming packet that matches an entry in
the above table can be programmed to execute this action. The action *set_tc* accepts only one parameter *tc* which is used
to set etrap_tc metadata in the P4 data pipeline

## Creating a P4 Extension Table Entry ##

```cpp
#define P4_TABLE_NAME "flow_classification"
#define P4_TABLE_MATCH_FIELDS "{\"priority\":100,\"src_addr\":\"10.1.1.0&255.255.255.0\"}"
#define P4_TABLE_ACTION "set_tc"
#define P4_TABLE_ACTION_PARAMETERS "{\"tc\":\"4\"}"

sai_object_id_t p4ext_entry_id = {};
uint8_t num_attrs = 4;
sai_attribute_id_t p4ext_entry_attrs[num_attrs] = {};

p4ext_entry_attrs[0].id = (sai_attr_id_t) SAI_P4EXT_ENTRY_ATTR_TABLE_ID;
p4ext_entry_attrs[0].value.s8list.count = strlen(P4_TABLE_NAME);
/** P4 Table Name */
p4ext_entry_attrs[0].value.s8list.value = (sai_s8_list_t.list)P4_TABLE_NAME;

p4ext_entry_attrs[1].id = (sai_attr_id_t) SAI_P4EXT_ENTRY_ATTR_MATCH_FIELD_ID;
p4ext_entry_attrs[1].value.s8list.count = strlen(P4_TABLE_MATCH_FIELDS);
/**
 * Match field keys and values represented as a string.
 * Ternary fields values are represented as value&mask.
 * If the table has one or more ternary match fields, then a
 * a key value pair with entry priority should be included
 */
p4ext_entry_attrs[1].value.s8list.value = P4_TABLE_MATCH_FIELDS;

p4ext_entry_attrs[2].id = (sai_attr_id_t) SAI_P4EXT_ENTRY_ATTR_ACTION_ID;
p4ext_entry_attrs[2].value.s8list.count = strlen(P4_TABLE_ACTION);
/** P4 Action Name */
p4ext_entry_attrs[2].value.s8list.value = P4_TABLE_ACTION;

p4ext_entry_attrs[3].id = (sai_attr_id_t) SAI_P4EXT_ENTRY_ATTR_PARAMETER_ID;
p4ext_entry_attrs[3].value.s8list.count = strlen(P4_TABLE_ACTION_PARAMETERS);
/**  Map of action param name and values represented as a string. */
p4ext_entry_attrs[3].value.s8list.value = P4_TABLE_ACTION_PARAMETERS;

if (sai_p4ext_api->create_p4ext_entry(&p4ext_entry_id, num_attrs, p4ext_entry_attrs) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
}
else
{
    // Failed...
}
```

## Removing a P4 Extension Table Entry ##

```cpp
if (sai_p4ext_api->remove_p4ext_entry(p4ext_entry_id) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
}
else
{
    // Failed...
}
```

## Set a P4 Extension Table Entry Attribute##

A user can update either the action or the action parameter associated with a P4 table entry. Both the action parameters and match fields are represented as key value pairs string. So, in order to update to a single key value pair, the entire string should be passed again with the updated action parameter or match field value.

```cpp
const char updated_action_param[] = "{\"tc\":\"5\"}"
sai_attribute_id_t p4ext_entry_attr = {};
p4ext_entry_attr.id = (sai_attr_id_t) SAI_P4EXT_ENTRY_ATTR_PARAMETER_ID;
p4ext_entry_attr.value.s8list.count = strlen(updated_action_para```ext_entry_attr.value.s8list.value = (sai_s8_list_t.list)updated_action_param;

if (sai_p4ext_api->set_p4ext_entry(&p4ext_entry_id, &p4ext_entry_attr) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
}
else
{
    // Failed...
}
```

## Get a P4 Extension Table Entry Attribute ##

```cpp
sai_attribute_t sai_p4ext_entry_attrs[2] = {};
sai_p4ext_entry_attrs[0].id = (sai_attr_id_t)SAI_P4EXT_ENTRY_ATTR_MATCH_FIELD_ID;
sai_p4ext_entry_attrs[1].id = (sai_attr_id_t)SAI_P4EXT_ENTRY_ATTR_PARAMETER_ID;

if (sai_p4ext_api->get_p4ext_entry__attribute(&p4ext_entry_id, 2, p4ext_entry_attrs) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
    // Free up memory for the strings
    if(sai_p4ext_entry_attrs[0].value.s8list.count) free(sai_p4ext_entry_attrs[0].value.s8list.value)
    if(sai_p4ext_entry_attrs[1].value.s8list.count) free(sai_p4ext_entry_attrs[1].value.s8list.value)
}
else
{
    // Failed...
}
```
