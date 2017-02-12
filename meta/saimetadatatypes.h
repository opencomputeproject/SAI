/**
 * Copyright (c) 2014 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc
 *
 * @file    saimetadatatypes.h
 *
 * @brief   This module defines SAI Metadata Types
 */

#ifndef __SAI_METADATA_TYPES_H__
#define __SAI_METADATA_TYPES_H__

/**
 * @defgroup SAIMETADATATYPES SAI Metadata Types Definitions
 *
 * @{
 */

/**
 * @def SAI_INVALID_ATTRIBUTE_ID
 */
#define SAI_INVALID_ATTRIBUTE_ID ((sai_attr_id_t)-1)

/**
 * @brief Defines object metadata key.
 */
typedef struct _sai_object_meta_key_t {

    /**
     * @brief Object type.
     */
    sai_object_type_t           objecttype;

    /**
     * @brief The key.
     */
    sai_object_key_t            objectkey;

} sai_object_meta_key_t;

/**
 * @brief Defines attribute value type.
 * Can be used when serializing attributes.
 */
typedef enum _sai_attr_value_type_t {

    /**
     * @brief Attribute value is bool.
     */
    SAI_ATTR_VALUE_TYPE_BOOL,

    /**
     * @brief Attribute value is char data.
     */
    SAI_ATTR_VALUE_TYPE_CHARDATA,

    /**
     * @brief Attribute value is 8 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_UINT8,

    /**
     * @brief Attribute value is 8 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_INT8,

    /**
     * @brief Attribute value is 16 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_UINT16,

    /**
     * @brief Attribute value is 16 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_INT16,

    /**
     * @brief Attribute value is 32 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_UINT32,

    /**
     * @brief Attribute value is 32 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_INT32,

    /**
     * @brief Attribute value is 64 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_UINT64,

    /**
     * @brief Attribute value is 64 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_INT64,

    /**
     * @brief Attribute value is pointer address.
     */
    SAI_ATTR_VALUE_TYPE_POINTER,

    /**
     * @brief Attribute value is mac address.
     */
    SAI_ATTR_VALUE_TYPE_MAC,

    /**
     * @brief Attribute value is IPv4.
     */
    SAI_ATTR_VALUE_TYPE_IPV4,

    /**
     * @brief Attribute value is IPv6.
     */
    SAI_ATTR_VALUE_TYPE_IPV6,

    /**
     * @brief Attribute value is IP address.
     */
    SAI_ATTR_VALUE_TYPE_IP_ADDRESS,

    /**
     * @brief Attribute value is ip prefix
     */
    SAI_ATTR_VALUE_TYPE_IP_PREFIX,

    /**
     * @brief Attribute value is object id.
     */
    SAI_ATTR_VALUE_TYPE_OBJECT_ID,

    /**
     * @brief Attribute value is object list.
     */
    SAI_ATTR_VALUE_TYPE_OBJECT_LIST,

    /**
     * @brief Attribute value is list of 8 bit unsigned integers.
     */
    SAI_ATTR_VALUE_TYPE_UINT8_LIST,

    /**
     * @brief Attribute value is list of 8 bit signed integers.
     */
    SAI_ATTR_VALUE_TYPE_INT8_LIST,

    /**
     * @brief Attribute value is list of 16 bit unsigned integers.
     */
    SAI_ATTR_VALUE_TYPE_UINT16_LIST,

    /**
     * @brief Attribute value is list of 16 bit signed integers.
     */
    SAI_ATTR_VALUE_TYPE_INT16_LIST,

    /**
     * @brief Attribute value is list of 32 bit unsigned integers.
     */
    SAI_ATTR_VALUE_TYPE_UINT32_LIST,

    /**
     * @brief Attribute value is list of 32 bit signed integers.
     */
    SAI_ATTR_VALUE_TYPE_INT32_LIST,

    /**
     * @brief Attribute value is 32 bit unsigned integer range.
     */
    SAI_ATTR_VALUE_TYPE_UINT32_RANGE,

    /**
     * @brief Attribute value is 32 bit signed integer range.
     */
    SAI_ATTR_VALUE_TYPE_INT32_RANGE,

    /**
     * @brief Attribute value is vlan list.
     */
    SAI_ATTR_VALUE_TYPE_VLAN_LIST,

    /**
     * @brief Attribute value is acl field bool.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_BOOL,

    /**
     * @brief Attribute value is acl field 8 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8,

    /**
     * @brief Attribute value is acl field 8 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8,

    /**
     * @brief Attribute value is acl field 16 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16,

    /**
     * @brief Attribute value is acl field 16 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16,

    /**
     * @brief Attribute value is acl field 32 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32,

    /**
     * @brief Attribute value is acl field 32 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32,

    /**
     * @brief Attribute value is acl field mac address.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC,

    /**
     * @brief Attribute value is acl field IPv4.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4,

    /**
     * @brief Attribute value is acl field IPv6.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6,

    /**
     * @brief Attribute value is acl field object id.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID,

    /**
     * @brief Attribute value is acl field object list.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST,

    /**
     * @brief Attribute value is acl field list of 8 bit unsigned integers.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST,

    /**
     * @brief Attribute value is acl action 8 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8,

    /**
     * @brief Attribute value is acl action 8 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8,

    /**
     * @brief Attribute value is acl action 16 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16,

    /**
     * @brief Attribute value is acl action 16 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16,

    /**
     * @brief Attribute value is acl action 32 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32,

    /**
     * @brief Attribute value is acl action 32 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32,

    /**
     * @brief Attribute value is acl action mac address.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC,

    /**
     * @brief Attribute value is acl action IPv4.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4,

    /**
     * @brief Attribute value is acl action IPV6.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6,

    /**
     * @brief Attribute value is acl action object id.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID,

    /**
     * @brief Attribute value is acl action object list.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST,

    /**
     * @brief Attribute value is qos map list.
     */
    SAI_ATTR_VALUE_TYPE_QOS_MAP_LIST,

    /**
     * @brief Attribute value is tunnel map list.
     */
    SAI_ATTR_VALUE_TYPE_TUNNEL_MAP_LIST,

    /**
     * @brief Attribute value is acl capability.
     */
    SAI_ATTR_VALUE_TYPE_ACL_CAPABILITY,

} sai_attr_value_type_t;

/**
 * @brief Attribute flags.
 */
typedef enum _sai_attr_flags_t {

    /**
     * @brief Mandatory on create flag.
     *
     * Attribute with this flags is mandatory when calling CREATE api, unless
     * this attribute is marked as conditional. Must be combined with
     * CREATE_ONLY or CREATE_AND_SET flag.
     */
    SAI_ATTR_FLAGS_MANDATORY_ON_CREATE = (1 << 0),

    /**
     * @brief Create only flag.
     *
     * Attribute with this flag can only be created and it's value cannot be
     * changed by SET api. Can be combined with with MANDATORY flag. If
     * attribute is not combined with MANDATORY flag then DEFAULT value must be
     * provided for this attribute.
     */
    SAI_ATTR_FLAGS_CREATE_ONLY         = (1 << 1),

    /**
     * @brief Create and set flag.
     *
     * Attribute with this flag can be created and after creation value may be
     * modified using SET api. Can be bombined with MANDATORY flag. If
     * attribute is not combined with MANDATORY flag then DEFAULT value must be
     * provided for this attribute.
     */
    SAI_ATTR_FLAGS_CREATE_AND_SET      = (1 << 2),

    /**
     * @brief Read only flag.
     *
     * Attribute with this flag can only be read using GET api. Creation and
     * modification is not possible. Can be combined with DYNAMIC flag for
     * example counter attribute.
     */
    SAI_ATTR_FLAGS_READ_ONLY           = (1 << 3),

    /**
     * @brief Key flag.
     *
     * Attribute with this flag is treated as unique key (can only be combined
     * with MANDATORY and CREATE_ONLY flags. This flag will indicate that
     * creating new object with the same key will fail (for example VLAN).
     * There may be more than one key in attributes when creating object. Key
     * should be used only on primitive attribute values (like enum or int).
     * In some cases it may be supported on list (for port lanes) but then
     * extra logic is needed to compute and handle that key.
     *
     * If multiple keys are provided, meta key is created as combination of
     * keys in order attribute id's are declared (internal details).
     */
    SAI_ATTR_FLAGS_KEY                 = (1 << 4),

    /**
     * @brief Dynamic flag.
     *
     * Attribute with this flag indicates that value of the attribute is
     * dynamic and can change in time (like a attribute counter value, or port
     * operational status). Change may happen independently or when other
     * attribute was created or modified (creating vlan member will change vlan
     * member list). Can be combined with READ_ONLY flag.
     */
    SAI_ATTR_FLAGS_DYNAMIC             = (1 << 5),

    /**
     * @brief Special flag.
     *
     * Attribute with this flag will indicate that this attribute is special
     * and it needs extended logic to be handled. This flag can only be
     * standalone.
     */
    SAI_ATTR_FLAGS_SPECIAL             = (1 << 6),

} sai_attr_flags_t;

/**
 * @def Defines helper to chek if mandatory on create falg is set.
 */
#define HAS_FLAG_MANDATORY_ON_CREATE(x)   (((x) & SAI_ATTR_FLAGS_MANDATORY_ON_CREATE) == SAI_ATTR_FLAGS_MANDATORY_ON_CREATE)

/**
 * @def Defines helper to chek if create only falg is set.
 */
#define HAS_FLAG_CREATE_ONLY(x)           (((x) & SAI_ATTR_FLAGS_CREATE_ONLY) == SAI_ATTR_FLAGS_CREATE_ONLY)

/**
 * @def Defines helper to chek if create and set falg is set.
 */
#define HAS_FLAG_CREATE_AND_SET(x)        (((x) & SAI_ATTR_FLAGS_CREATE_AND_SET) == SAI_ATTR_FLAGS_CREATE_AND_SET)

/**
 * @def Defines helper to chek if read only falg is set.
 */
#define HAS_FLAG_READ_ONLY(x)             (((x) & SAI_ATTR_FLAGS_READ_ONLY) == SAI_ATTR_FLAGS_READ_ONLY)

/**
 * @def Defines helper to chek if key falg is set.
 */
#define HAS_FLAG_KEY(x)                   (((x) & SAI_ATTR_FLAGS_KEY) == SAI_ATTR_FLAGS_KEY)

/**
 * @def Defines helper to chek if dynamic falg is set.
 */
#define HAS_FLAG_DYNAMIC(x)               (((x) & SAI_ATTR_FLAGS_DYNAMIC) == SAI_ATTR_FLAGS_DYNAMIC)

/**
 * @def Defines helper to chek if special falg is set.
 */
#define HAS_FLAG_SPECIAL(x)               (((x) & SAI_ATTR_FLAGS_SPECIAL) == SAI_ATTR_FLAGS_SPECIAL)

/**
 * @brief Defines default value type.
 */
typedef enum _sai_default_value_type_t {

    /**
     * @brief There is no default value.
     *
     * This must be assigned on MANDATORY_ON_CREATE
     * attributes.
     */
    SAI_DEFAULT_VALUE_TYPE_NONE = 0,

    /**
     * @brief Default value is just a const value.
     */
    SAI_DEFAULT_VALUE_TYPE_CONST,

    /**
     * @brief Value must be in range provided by other attribute.
     *
     * Usually value is provieded by switch object.
     * Range can be obtined by GET api.
     * Usually default value is minimum of range.
     */
    SAI_DEFAULT_VALUE_TYPE_ATTR_RANGE,

    /**
     * @brief Default value is equal to other attribute value.
     *
     * Usually value is provided by switch object.
     * Can be obtained using GET api.
     */
    SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE,

    /**
     * @brief Default value is just empty list.
     */
    SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST,

    /**
     * @brief Default value is vendor specific.
     *
     * This value is assigned by switch vendor
     * like default switch MAC address.
     *
     * It can also be default created object
     * like default hash.
     *
     * Vendor specific should be different
     * than default objects that are created
     * by default.
     */
    SAI_DEFAULT_VALUE_TYPE_VENDOR_SPECIFIC,

    /**
     * @brief This object is created by default
     * inside switch (hidden object, like default hash or port).
     *
     * Should be used only on object id types.
     */
    SAI_DEFAULT_VALUE_TYPE_SWITCH_INTERNAL,

} sai_default_value_type_t;

/**
 * @brief Defines attribute condition type.
 */
typedef enum _sai_attr_condition_type_t
{
    /**
     * @brief This attribute is not conditional atttribute
     */
    SAI_ATTR_CONDITION_TYPE_NONE = 0,

    /**
     * @brief Any condition that will be true will make
     * this attribute mandatory.
     */
    SAI_ATTR_CONDITION_TYPE_OR,

    /**
     * @brief All condictions must meet for this attribute
     * to be mandatory on create.
     */
    SAI_ATTR_CONDITION_TYPE_AND,

} sai_attr_condition_type_t;

/**
 * @brief Defines attribute condition.
 */
typedef struct _sai_attr_condition_t
{
    /**
     * @brief Specifies valid attribute id for this object type.
     * Attribute is for the same object type.
     */
    sai_attr_id_t                       attrid;

    /**
     * @brief Condition value that attribute will be mandatory
     * then default value must be provided for attribute.
     */
    sai_attribute_value_t               condition;

} sai_attr_condition_t;

/**
 * @brief Defines enum metadata information.
 */
typedef struct _sai_enum_metadata_t {

    /**
     * @brief String representation of enum typedef.
     */
    const char*     name;

    /**
     * @brief Values count in enum.
     */
    const size_t    valuescount;

    /**
     * @brief Array of enum values.
     */
    const int*      values;

    /**
     * @brief Array of enum values string names.
     */
    const char**    valuesnames;

    /**
     * @brief Array of enum values string short names.
     */
    const char**    valuesshortnames;

} sai_enum_metadata_t;

/**
 * @brief Defines attribute metadata.
 */
typedef struct _sai_attr_metadata_t
{
    /**
     * @brief Specifies valid SAI object type.
     */
    sai_object_type_t                   objecttype;

    /**
     * @brief Specifies valid attribute id for this object type.
     */
    sai_attr_id_t                       attrid;

    /**
     * @brief Specifies valid attribute id name for this object type.
     */
    const char* const                   attridname;

    /**
     * @brief Specifies attribute value type for this attribute.
     */
    sai_attr_value_type_t               attrvaluetype;

    /**
     * @brief Specifies flags for this attribute.
     */
    sai_attr_flags_t                    flags;

    /**
     * @brief Specified allowed object types.
     *
     * If object attr value type is OBJECT_ID
     * this list specifies what object type can be used.
     */
    const sai_object_type_t* const      allowedobjecttypes;

    /**
     * @brief Length of allowed object types.
     */
    size_t                              allowedobjecttypeslength;

    /**
     * @brief Allows repetitions on object list.
     *
     * Can be useful when using object id list.
     */
    bool                                allowrepetitiononlist;

    /**
     * @brief Allows mixed object id types on list
     * like port and lag.
     */
    bool                                allowmixedobjecttypes;

    /**
     * @brief Allows empty list to be set on list value type.
     */
    bool                                allowemptylist;

    /**
     * @brief Allows null object id to be passed.
     *
     * If object attr value type is OBJECT_ID
     * it tells whether SAI_NULL_OBJECT_ID can be used
     * as actual id.
     */
    bool                                allownullobjectid;

    /**
     * @brief Specifies default value type.
     *
     * Default value can be a const assigned by switch
     * (which is not know at compile), can be obtained
     * by GET api, or a min/max value in specific
     * range also assigned by switch at runtime.
     *
     * Default value can be also an object id.
     */
    const sai_default_value_type_t      defaultvaluetype;

    /**
     * @brief Provides default value.
     *
     * If creation flag is CREATE_ONLY or CREATE_AND_SET
     * then default value must be provided for attribute.
     *
     * @note Default value may not apply for acl field
     * or acl entry, need special care.
     */
    const sai_attribute_value_t* const  defaultvalue;

    /**
     * @brief Default value object type.
     *
     * Required when default value type is pointing to
     * different object type.
     */
    sai_object_type_t                   defaultvalueobjecttype;

    /**
     * @brief Default value object id.
     *
     * Required when default value type is pointing to
     * different object attribute.
     */
    sai_attr_id_t                       defaultvalueattrid;

    /**
     * @brief Indicates wheter attribute is enum value.
     *
     * Attribute type must be set as INT32.
     *
     * @note Could be deduced from enum type string or
     * enum vector values and attr value type.
     */
    bool                                isenum;

    /**
     * @brief Indicates wheter attribute is enum list value.
     *
     * Attribute value must must be set INT32 LIST.
     *
     * @note Could be deduced from enum type string or
     * enum vector values and attr value type.
     */
    bool                                isenumlist;

    /**
     * @brief Provides enum metadata if attribute
     * is enum or enum list.
     */
    const sai_enum_metadata_t* const    enummetadata;

    /**
     * @brief Specifies condition type of attribute.
     *
     * @note Currently all conditions are "OR" conditions
     * so we can deduce if this is conditional type
     * if any conditions are defined.
     */
    sai_attr_condition_type_t           conditiontype;

    /**
     * @brief Provide conditions for attribute under
     * which this attribute will be mandatory on create.
     */
    const sai_attr_condition_t** const  conditions;

    /**
     * @brief Length of the conditions.
     */
    size_t                              conditionslength;

    /**
     * @brief Specifies valid only type of attribute.
     *
     * @note Currently all valid only are "OR" conditions
     * so we can deduce if this is conditional type
     * if any conditions are defined.
     */
    sai_attr_condition_type_t           validonlytype;

    /**
     * @brief Provides conditions when this attribute is valid.
     *
     * If conditions are specified (OR condition assumed)
     * then this attribute is only valid when different
     * atribute has condition value set. Valid only
     * attribute (against we check) can be dynamic so
     * this attribute can't be marked as MANDATORY on
     * create since default value will be required.
     *
     * @note There is only handful of attributes with
     * valid only mark. For now we will check that in
     * specific attribute logic.
     */
    const sai_attr_condition_t** const  validonly;

    /**
     * @brief Length of the valid only when conditions.
     */
    size_t                              validonlylength;

    /**
     * @brief When calling GET api result will be put
     * in local db for future use (extra logic).
     *
     * This flag must be taken with care, since when set
     * on dynamic attribute it may provide inconsistent data.
     *
     * Value should be updated after successfull set or remove.
     */
    bool                                getsave;

    /**
     * @brief Determines whether value is vlan.
     *
     * Can only be set on UINT16 value type.
     */
    bool                                isvlan;

    /**
     * @brief Determines whether attribute is ACL field
     *
     * This will become handy for fast detrmination whether
     * default value is present.
     */
    bool                                isaclfield;

    /*
     * @brief Determines whether attribute is ACL action
     *
     * This will become handy for fast detrmination whether
     * default value is present.
     */
    bool                                isaclaction;

} sai_attr_metadata_t;

/*
 * TODO since non object id members can have different type and can be localed
 * at different object_key union position, we need to find a way to extract
 * those for automatic serialize/deserialize for example extracting value as
 * sai_attribute_value_t and pointing to right serialize/deserialize functions.
 * Also a automatic generated functions for serialize/deserialize for those non
 * object id structs must be generated, we don't want to update them manually.
 */

/**
 * @brief Function definition for getting OID from non obeject
 * id struct member.
 */
typedef sai_object_id_t (*sai_meta_get_struct_member_oid_fn)(
        _In_ const sai_object_meta_key_t *object_meta_key);

/**
 * @brief Function definition for setting OID from non obeject
 * id struct member.
 */
typedef void (*sai_meta_set_struct_member_oid_fn)(
        _Inout_ sai_object_meta_key_t *object_meta_key,
        _In_ sai_object_id_t oid);

/**
 * @brief Defines struct member info for
 * non object id object type
 */
typedef struct _sai_struct_member_info_t
{
    /**
     * @brief Member vlaue type
     */
    sai_attr_value_type_t                               membervaluetype;

    /**
     * @brief Member name
     */
    const char*                                         membername;

    /**
     * @brief Indicates whether field is vlan
     */
    bool                                                isvlan;

    /**
     * @brief Specified allowed object types.
     *
     * If object attr value type is OBJECT_ID
     * this list specifies what object type can be used.
     */
    const sai_object_type_t* const                      allowedobjecttypes;

    /**
     * @brief Length of allowed object types.
     */
    size_t                                              allowedobjecttypeslength;

    /**
     * @brief Indicates wheter member is enum value.
     *
     * Type must be set as INT32.
     *
     * @note Could be deduced from enum type string or
     * enum vector values and attr value type.
     */
    bool                                                isenum;

    /**
     * @brief Provides enum metadata if member is enum
     */
    const sai_enum_metadata_t* const                    enummetadata;

    /**
     * @brief If struct member is OID this function
     * will get it's value.
     */
    const sai_meta_get_struct_member_oid_fn             getoid;

    /**
     * @brief If struct member is OID this function
     * will set it's value.
     */
    const sai_meta_set_struct_member_oid_fn             setoid;

} sai_struct_member_info_t;

/**
 * @brief SAI reverse graph member
 */
typedef struct _sai_rev_graph_member_t
{
    /**
     * @brief Defines main object type which is used
     * by dependency object type.
     */
    sai_object_type_t                       objecttype;

    /**
     * @brief Defines dependency object type on which
     * is object type defined above is used.
     */
    sai_object_type_t                       depobjecttype;

    /**
     * @brief Defines attribute metadata for object type
     *
     * This can be NULL if dependency objec type
     * is non object id type and dependency is on
     * defined struct.
     */
    const sai_attr_metadata_t* const        attrmetadata;

    /**
     * @brief Defines struct member for non object
     * id object type.
     *
     * This member can be NULL if dependency object type
     * is object attribute, and is not NULL id object
     * dependency is non object id struct member.
     */
    const sai_struct_member_info_t* const   structmember;

} sai_rev_graph_member_t;

/*
 * Generic QUAD api definitions. All apis can be called using this quad genric
 * functions.
 *
 * When creating switch object or non object id switch_id parameter is ignored,
 * and can be NULL. Currently objecttype inside sai_object_meta_key_t is
 * ignored and can be skipped.
 *
 * This generic quad api will help us later to call any api, without doind any
 * switch cases for calling differen signature functions including non object
 * id structs. Also later we will generate automatic serialize and deserialize
 * methods for non object id which will deserialize data to object union in
 * sai_object_meta_key_t to right place.
 *
 * TODO add medatada init function which will populate global api function
 * pointers which will be used when calling each api.
 */

typedef sai_status_t (*sai_meta_generic_create_fn)(
        _Inout_ sai_object_meta_key_t *meta_key,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_meta_generic_remove_fn)(
        _In_ const sai_object_meta_key_t *meta_key);

typedef sai_status_t (*sai_meta_generic_set_fn)(
        _In_ const sai_object_meta_key_t *meta_key,
        _In_ const sai_attribute_t *attr);

typedef sai_status_t (*sai_meta_generic_get_fn)(
        _In_ const sai_object_meta_key_t *meta_key,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief SAI object type information
 */
typedef struct _sai_object_type_info_t
{
    /**
     * @brief Object Type
     */
    sai_object_type_t                       objecttype;

    /**
     * @brief Start of attributes *_START
     */
    sai_attr_id_t                           attridstart;

    /**
     * @brief End of attributes *_END
     */
    sai_attr_id_t                           attridend;

    /**
     * @brief Provides enum metadata if attribute
     * is enum or enum list.
     */
    const sai_enum_metadata_t* const        enummetadata;

    /**
     * @brief Attributes metadata
     */
    const sai_attr_metadata_t** const       attrmetadata;

    /**
     * @brief Indicates if object is using struct
     * instead od actual object id
     */
    bool                                    isnonobjectid;

    /**
     * @brief Defines all struct members
     */
    const sai_struct_member_info_t** const  structmembers;

    /**
     * @brief Defines count of struct members
     */
    size_t                                  structmemberscount;

    /**
     * @brief Defines reverse dependency graph members
     */
    const sai_rev_graph_member_t** const    revgraphmembers;

    /**
     * @brief Create function pointer.
     */
    const sai_meta_generic_create_fn        create;

    /**
     * @brief Remove function pointer.
     */
    const sai_meta_generic_remove_fn        remove;

    /**
     * @brief Set function pointer.
     */
    const sai_meta_generic_set_fn           set;

    /**
     * @brief Get function pointer
     */
    const sai_meta_generic_get_fn           get;

} sai_object_type_info_t;

/**
 * @}
 */
#endif /** __SAI_METADATA_TYPES_H__ */
