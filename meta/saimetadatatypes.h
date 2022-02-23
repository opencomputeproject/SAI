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
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
 *
 * @file    saimetadatatypes.h
 *
 * @brief   This module defines SAI Metadata Types
 */

#ifndef __SAIMETADATATYPES_H_
#define __SAIMETADATATYPES_H_

/**
 * @defgroup SAIMETADATATYPES SAI - Metadata Types Definitions
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
typedef struct _sai_object_meta_key_t
{
    /**
     * @brief Object type.
     */
    sai_object_type_t           objecttype;

    /**
     * @brief The key.
     *
     * @passparam objecttype
     */
    sai_object_key_t            objectkey;

} sai_object_meta_key_t;

/**
 * @brief Defines attribute value type.
 * Can be used when serializing attributes.
 */
typedef enum _sai_attr_value_type_t
{
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
     * @brief Attribute value is MAC address.
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
     * @brief Attribute value is IP prefix
     */
    SAI_ATTR_VALUE_TYPE_IP_PREFIX,

    /**
     * @brief Attribute value is PRBS RX state
     */
    SAI_ATTR_VALUE_TYPE_PRBS_RX_STATE,

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
     * @brief Attribute value is 16 bit unsigned integer range list.
     */
    SAI_ATTR_VALUE_TYPE_UINT16_RANGE_LIST,

    /**
     * @brief Attribute value is 32 bit signed integer range.
     */
    SAI_ATTR_VALUE_TYPE_INT32_RANGE,

    /**
     * @brief Attribute value is ACL field bool.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_BOOL,

    /**
     * @brief Attribute value is ACL field 8 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8,

    /**
     * @brief Attribute value is ACL field 8 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8,

    /**
     * @brief Attribute value is ACL field 16 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16,

    /**
     * @brief Attribute value is ACL field 16 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16,

    /**
     * @brief Attribute value is ACL field 32 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32,

    /**
     * @brief Attribute value is ACL field 32 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32,

    /**
     * @brief Attribute value is ACL field 64 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT64,

    /**
     * @brief Attribute value is ACL field MAC address.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC,

    /**
     * @brief Attribute value is ACL field IPv4.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4,

    /**
     * @brief Attribute value is ACL field IPv6.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6,

    /**
     * @brief Attribute value is MACsec rule match field SCI.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MACSEC_SCI,

    /**
     * @brief Attribute value is ACL field object id.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID,

    /**
     * @brief Attribute value is ACL field object list.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST,

    /**
     * @brief Attribute value is ACL field list of 8 bit unsigned integers.
     */
    SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST,

    /**
     * @brief Attribute value is ACL action bool.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_BOOL,

    /**
     * @brief Attribute value is ACL action 8 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8,

    /**
     * @brief Attribute value is ACL action 8 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8,

    /**
     * @brief Attribute value is ACL action 16 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16,

    /**
     * @brief Attribute value is ACL action 16 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16,

    /**
     * @brief Attribute value is ACL action 32 bit unsigned integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32,

    /**
     * @brief Attribute value is ACL action 32 bit signed integer.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32,

    /**
     * @brief Attribute value is ACL action MAC address.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC,

    /**
     * @brief Attribute value is ACL action IPv4.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4,

    /**
     * @brief Attribute value is ACL action IPv6.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6,

    /**
     * @brief Attribute value is ACL action IP address.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IP_ADDRESS,

    /**
     * @brief Attribute value is ACL action object id.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID,

    /**
     * @brief Attribute value is ACL action object list.
     */
    SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST,

    /**
     * @brief Attribute value is ACL capability.
     */
    SAI_ATTR_VALUE_TYPE_ACL_CAPABILITY,

    /**
     * @brief Attribute value is ACL resource.
     */
    SAI_ATTR_VALUE_TYPE_ACL_RESOURCE_LIST,

    /**
     * @brief Attribute value is generic map list.
     */
    SAI_ATTR_VALUE_TYPE_MAP_LIST,

    /**
     * @brief Attribute value is vlan list.
     */
    SAI_ATTR_VALUE_TYPE_VLAN_LIST,

    /**
     * @brief Attribute value is QOS map list.
     */
    SAI_ATTR_VALUE_TYPE_QOS_MAP_LIST,

    /**
     * @brief Attribute value is Segment Route Type Length Value list.
     */
    SAI_ATTR_VALUE_TYPE_TLV_LIST,

    /**
     * @brief Attribute value is Segment Route Segment list.
     */
    SAI_ATTR_VALUE_TYPE_SEGMENT_LIST,

    /**
     * @brief Attribute value is IP address list.
     */
    SAI_ATTR_VALUE_TYPE_IP_ADDRESS_LIST,

    /**
     * @brief Attribute value is port eye values list.
     */
    SAI_ATTR_VALUE_TYPE_PORT_EYE_VALUES_LIST,

    /**
     * @brief Attribute value is timespec.
     */
    SAI_ATTR_VALUE_TYPE_TIMESPEC,

    /**
     * @brief Attribute value is NAT data.
     */
    SAI_ATTR_VALUE_TYPE_NAT_ENTRY_DATA,

    /**
     * @brief Attribute value is MACsec SCI.
     */
    SAI_ATTR_VALUE_TYPE_MACSEC_SCI,

    /**
     * @brief Attribute value is MACsec SSCI.
     */
    SAI_ATTR_VALUE_TYPE_MACSEC_SSCI,

    /**
     * @brief Attribute value is MACsec SAK.
     */
    SAI_ATTR_VALUE_TYPE_MACSEC_SAK,

    /**
     * @brief Attribute value is MACsec Authentication Key.
     */
    SAI_ATTR_VALUE_TYPE_MACSEC_AUTH_KEY,

    /**
     * @brief Attribute value is MACsec SALT.
     */
    SAI_ATTR_VALUE_TYPE_MACSEC_SALT,

    /**
     * @brief Attribute value is System Port Configuration.
     */
    SAI_ATTR_VALUE_TYPE_SYSTEM_PORT_CONFIG,

    /**
     * @brief Attribute value is System Port Configuration list.
     */
    SAI_ATTR_VALUE_TYPE_SYSTEM_PORT_CONFIG_LIST,

    /**
     * @brief Attribute value is Fabric Port Reachability.
     */
    SAI_ATTR_VALUE_TYPE_FABRIC_PORT_REACHABILITY,

    /**
     * @brief Attribute value is fabric port error status.
     */
    SAI_ATTR_VALUE_TYPE_PORT_ERR_STATUS_LIST,

    /**
     * @brief Attribute value is encryption key.
     */
    SAI_ATTR_VALUE_TYPE_ENCRYPT_KEY,

    /**
     * @brief Attribute value is authentication Key.
     */
    SAI_ATTR_VALUE_TYPE_AUTH_KEY,

} sai_attr_value_type_t;

/**
 * @brief Attribute flags.
 *
 * @flags strict
 */
typedef enum _sai_attr_flags_t
{
    /**
     * @brief Mandatory on create flag.
     *
     * Attribute with this flag is mandatory when calling CREATE API, unless
     * this attribute is marked as conditional. Must be combined with
     * CREATE_ONLY or CREATE_AND_SET flag.
     */
    SAI_ATTR_FLAGS_MANDATORY_ON_CREATE = (1 << 0),

    /**
     * @brief Create only flag.
     *
     * Attribute with this flag can only be created and its value cannot be
     * changed by SET API. Can be combined with MANDATORY flag. If
     * attribute is not combined with MANDATORY flag then DEFAULT value must be
     * provided for this attribute.
     */
    SAI_ATTR_FLAGS_CREATE_ONLY         = (1 << 1),

    /**
     * @brief Create and set flag.
     *
     * Attribute with this flag can be created and after creation value may be
     * modified using SET API. Can be combined with MANDATORY flag. If
     * attribute is not combined with MANDATORY flag then DEFAULT value must be
     * provided for this attribute.
     */
    SAI_ATTR_FLAGS_CREATE_AND_SET      = (1 << 2),

    /**
     * @brief Read only flag.
     *
     * Attribute with this flag can only be read using GET API. Creation and
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
     * keys in order attribute ids are declared (internal details).
     */
    SAI_ATTR_FLAGS_KEY                 = (1 << 4),

    /**
     * @brief Dynamic flag.
     *
     * Attribute with this flag indicates that value of the attribute is
     * dynamic and can change in time (like an attribute counter value, or port
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
 * @def Defines helper to check if mandatory on create flag is set.
 */
#define SAI_HAS_FLAG_MANDATORY_ON_CREATE(x)   (((x) & SAI_ATTR_FLAGS_MANDATORY_ON_CREATE) == SAI_ATTR_FLAGS_MANDATORY_ON_CREATE)

/**
 * @def Defines helper to check if create only flag is set.
 */
#define SAI_HAS_FLAG_CREATE_ONLY(x)           (((x) & SAI_ATTR_FLAGS_CREATE_ONLY) == SAI_ATTR_FLAGS_CREATE_ONLY)

/**
 * @def Defines helper to check if create and set flag is set.
 */
#define SAI_HAS_FLAG_CREATE_AND_SET(x)        (((x) & SAI_ATTR_FLAGS_CREATE_AND_SET) == SAI_ATTR_FLAGS_CREATE_AND_SET)

/**
 * @def Defines helper to check if read only flag is set.
 */
#define SAI_HAS_FLAG_READ_ONLY(x)             (((x) & SAI_ATTR_FLAGS_READ_ONLY) == SAI_ATTR_FLAGS_READ_ONLY)

/**
 * @def Defines helper to check if key flag is set.
 */
#define SAI_HAS_FLAG_KEY(x)                   (((x) & SAI_ATTR_FLAGS_KEY) == SAI_ATTR_FLAGS_KEY)

/**
 * @def Defines helper to check if dynamic flag is set.
 */
#define SAI_HAS_FLAG_DYNAMIC(x)               (((x) & SAI_ATTR_FLAGS_DYNAMIC) == SAI_ATTR_FLAGS_DYNAMIC)

/**
 * @def Defines helper to check if special flag is set.
 */
#define SAI_HAS_FLAG_SPECIAL(x)               (((x) & SAI_ATTR_FLAGS_SPECIAL) == SAI_ATTR_FLAGS_SPECIAL)

/**
 * @brief Defines default value type.
 */
typedef enum _sai_default_value_type_t
{
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
     * Usually value is provided by switch object.
     * Range can be obtained by GET API.
     * Usually default value is minimum of range.
     */
    SAI_DEFAULT_VALUE_TYPE_ATTR_RANGE,

    /**
     * @brief Default value is equal to other attribute value.
     *
     * Usually value is provided by switch object.
     * Can be obtained using GET API.
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
     * from default objects that are created
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
     * @brief This attribute is not conditional attribute
     */
    SAI_ATTR_CONDITION_TYPE_NONE = 0,

    /**
     * @brief Any condition that will be true will make
     * this attribute mandatory.
     */
    SAI_ATTR_CONDITION_TYPE_OR,

    /**
     * @brief All conditions must meet for this attribute
     * to be mandatory on create.
     */
    SAI_ATTR_CONDITION_TYPE_AND,

    /**
     * @brief Mixed condition, can contain and/or operators as well
     * as grouping using brackets (). Conditions are stored in RPN.
     */
    SAI_ATTR_CONDITION_TYPE_MIXED,

} sai_attr_condition_type_t;

/**
 * @brief Condition operator (==,!=,<,>,<=.>=).
 */
typedef enum _sai_condition_operator_t
{
    SAI_CONDITION_OPERATOR_EQ = 0,

    SAI_CONDITION_OPERATOR_NE,

    SAI_CONDITION_OPERATOR_LT,

    SAI_CONDITION_OPERATOR_GT,

    SAI_CONDITION_OPERATOR_LE,

    SAI_CONDITION_OPERATOR_GE,

} sai_condition_operator_t;

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
    const sai_attribute_value_t         condition;

    /**
     * @brief Condition operator (==,!=,<,>,<=.>=).
     */
    sai_condition_operator_t            op;

    /**
     * @brief Condition type.
     *
     * If main condition type is MIXED, then condition list is written in RPN
     * (reverse polish notation) syntax notation. If this field is NONE, then
     * this is actual condition, otherwise it can be AND,OR type which is just
     * a operator indication that should be performed. For AND,OR case attrid
     * is equal to SAI_INVALID_ATTRIBUTE_ID.
     */
    sai_attr_condition_type_t           type;

} sai_attr_condition_t;

/**
 * @brief Defines enum flags type, if enum contains flags.
 *
 * Enum values repetitions are not allowed on all types, unless marked with
 * deprecated for backward compatibility or defined outside enum using
 * define directive.
 */
typedef enum _sai_enum_flags_type_t
{
    /**
     * @brief Enum has no flags, must start with 0 and have sequential values.
     *
     * This is default value for all enum, no need for explicit declaration.
     */
    SAI_ENUM_FLAGS_TYPE_NONE,

    /**
     * @brief Enum is strict flags starting from 1 and uses power of 2.
     *
     * Flags combinations enum definitions NOT allowed, like: C = A | B.
     *
     * User combined value can contain all flags set at once.
     */
    SAI_ENUM_FLAGS_TYPE_STRICT,

    /**
     * @brief Enum is mixed flags starting from 1 and uses power of 2.
     *
     * Flags combinations enum definitions ARE allowed, like: C = A | B.
     */
    SAI_ENUM_FLAGS_TYPE_MIXED,

    /**
     * @brief Enum contains ranges in base steps of 0x1000. Can start with
     * specific range. Inside ranges enum must have sequential values.
     */
    SAI_ENUM_FLAGS_TYPE_RANGES,

    /**
     * @brief Complete freedom of defining enum, everything is allowed here.
     */
    SAI_ENUM_FLAGS_TYPE_FREE,

    /* future types can be defined */

    /* TODO extension type? */

} sai_enum_flags_type_t;

/**
 * @brief Defines enum metadata information.
 */
typedef struct _sai_enum_metadata_t
{
    /**
     * @brief String representation of enum type definition.
     */
    const char* const               name;

    /**
     * @brief Values count in enum.
     */
    const size_t                    valuescount;

    /**
     * @brief Array of enum values.
     */
    const int* const                values;

    /**
     * @brief Array of enum values string names.
     */
    const char* const* const        valuesnames;

    /**
     * @brief Array of enum values string short names.
     */
    const char* const* const        valuesshortnames;

    /**
     * @brief Indicates whether enumeration contains flags.
     *
     * When set to true numbers of enumeration are not sequential.
     */
    bool                            containsflags;

    /**
     * @brief Defines enum flags type, if enum contains flags.
     *
     * If contains flags is false, then flag type must be
     * SAI_ENUM_FLAGS_TYPE_NONE.
     */
    sai_enum_flags_type_t           flagstype;

    /**
     * @brief Array of enum ignored values.
     */
    const int* const                ignorevalues;

    /**
     * @brief Array of enum ignored values string names.
     */
    const char* const* const        ignorevaluesnames;

    /**
     * @brief Object type to which this enum belongs.
     *
     * If enum don't belong to any object type then this field will be equal to
     * SAI_OBJECT_TYPE_NULL.
     */
    sai_object_type_t               objecttype;

} sai_enum_metadata_t;

/**
 * @brief Defines attribute capability metadata.
 */
typedef struct _sai_attr_capability_metadata_t
{
    /**
     * @brief Vendor ID.
     *
     * Used to distinguish different capabilities of
     * the same attribute for different ASIC instances.
     */
    uint64_t                    vendorid;

    /**
     * @brief Operation capability.
     *
     * Defines which operation is supported on specific attribute.
     */
    sai_attr_capability_t       operationcapability;

    /**
     * @brief Enum values count.
     *
     * When attribute is and enum, this list defines
     * enum values supported by vendor on that attribute.
     */
    const size_t                enumvaluescount;

    /**
     * @brief Enum values count.
     */
    const int* const            enumvalues;

} sai_attr_capability_metadata_t;

/**
 * @brief Defines attribute metadata.
 */
typedef struct _sai_attr_metadata_t
{
    /**
     * @brief Specifies valid SAI object type.
     */
    sai_object_type_t                           objecttype;

    /**
     * @brief Specifies valid attribute id for this object type.
     */
    sai_attr_id_t                               attrid;

    /**
     * @brief Specifies valid attribute id name for this object type.
     */
    const char* const                           attridname;

    /**
     * @brief Extracted brief description from Doxygen comment.
     */
    const char* const                           brief;

    /**
     * @brief Specifies attribute value type for this attribute.
     */
    sai_attr_value_type_t                       attrvaluetype;

    /**
     * @brief Specifies flags for this attribute.
     */
    sai_attr_flags_t                            flags;

    /**
     * @brief Specified allowed object types.
     *
     * If object attr value type is OBJECT_ID
     * this list specifies what object type can be used.
     */
    const sai_object_type_t* const              allowedobjecttypes;

    /**
     * @brief Length of allowed object types.
     */
    size_t                                      allowedobjecttypeslength;

    /**
     * @brief Allows repetitions on object list.
     *
     * Can be useful when using object id list.
     */
    bool                                        allowrepetitiononlist;

    /**
     * @brief Allows mixed object id types on list
     * like port and LAG.
     */
    bool                                        allowmixedobjecttypes;

    /**
     * @brief Allows empty list to be set on list value type.
     */
    bool                                        allowemptylist;

    /**
     * @brief Allows null object id to be passed.
     *
     * If object attr value type is OBJECT_ID
     * it tells whether SAI_NULL_OBJECT_ID can be used
     * as actual id.
     */
    bool                                        allownullobjectid;

    /**
     * @brief Determines whether attribute contains OIDs
     */
    bool                                        isoidattribute;

    /**
     * @brief Specifies default value type.
     *
     * Default value can be a const assigned by switch
     * (which is not known at compile), can be obtained
     * by GET API, or a min/max value in specific
     * range also assigned by switch at run time.
     *
     * Default value can be also an object id.
     */
    const sai_default_value_type_t              defaultvaluetype;

    /**
     * @brief Provides default value.
     *
     * If creation flag is CREATE_ONLY or CREATE_AND_SET
     * then default value must be provided for attribute.
     *
     * @note Default value may not apply for ACL field
     * or ACL entry, need special care.
     */
    const sai_attribute_value_t* const          defaultvalue;

    /**
     * @brief Default value object type.
     *
     * Required when default value type is pointing to
     * different object type.
     */
    sai_object_type_t                           defaultvalueobjecttype;

    /**
     * @brief Default value object id.
     *
     * Required when default value type is pointing to
     * different object attribute.
     */
    sai_attr_id_t                               defaultvalueattrid;

    /**
     * @brief Indicates whether default value needs to be saved.
     *
     * When switch is created some objects are created internally like vlan 1,
     * vlan members, bridge port, virtual router etc. Some of those objects
     * has attributes assigned by vendor like switch MAC address. When user
     * changes that value then there is no way to go back and set it's previous
     * value if user didn't query it first. This member will indicate whether
     * user needs to query it first (and store) before change, if he wants to
     * bring original attribute value later.
     *
     * Some of those attributes can be OID attributes with flags
     * MANDATORY_ON_CREATE and CREATE_AND_SET.
     */
    bool                                        storedefaultvalue;

    /**
     * @brief Indicates whether attribute is enum value.
     *
     * Attribute type must be set as INT32.
     *
     * @note Could be deduced from enum type string or
     * enum vector values and attr value type.
     */
    bool                                        isenum;

    /**
     * @brief Indicates whether attribute is enum list value.
     *
     * Attribute value must be set INT32 LIST.
     *
     * @note Could be deduced from enum type string or
     * enum vector values and attr value type.
     */
    bool                                        isenumlist;

    /**
     * @brief Provides enum metadata if attribute
     * is enum or enum list.
     */
    const sai_enum_metadata_t* const            enummetadata;

    /**
     * @brief Specifies condition type of attribute.
     *
     * @note Currently all conditions are "OR" conditions
     * so we can deduce if this is conditional type
     * if any conditions are defined.
     */
    sai_attr_condition_type_t                   conditiontype;

    /**
     * @brief Provide conditions for attribute under
     * which this attribute will be mandatory on create.
     */
    const sai_attr_condition_t* const* const    conditions;

    /**
     * @brief Length of the conditions.
     */
    size_t                                      conditionslength;

    /**
     * @brief Indicates whether attribute is conditional.
     */
    bool                                        isconditional;

    /**
     * @brief Specifies valid only type of attribute.
     *
     * @note Currently all valid only are "OR" conditions
     * so we can deduce if this is conditional type
     * if any conditions are defined.
     */
    sai_attr_condition_type_t                   validonlytype;

    /**
     * @brief Provides conditions when this attribute is valid.
     *
     * If conditions are specified (OR condition assumed)
     * then this attribute is only valid when different
     * attribute has condition value set. Valid only
     * attribute (against we check) can be dynamic so
     * this attribute can't be marked as MANDATORY on
     * create since default value will be required.
     *
     * @note There is only handful of attributes with
     * valid only mark. For now we will check that in
     * specific attribute logic.
     */
    const sai_attr_condition_t* const* const    validonly;

    /**
     * @brief Length of the valid only when conditions.
     */
    size_t                                      validonlylength;

    /**
     * @brief Indicates whether attribute is valid only.
     */
    bool                                        isvalidonly;

    /**
     * @brief When calling GET API result will be put
     * in local db for future use (extra logic).
     *
     * This flag must be taken with care, since when set
     * on dynamic attribute it may provide inconsistent data.
     *
     * Value should be updated after successful set or remove.
     */
    bool                                        getsave;

    /**
     * @brief Determines whether value is vlan.
     *
     * Can only be set on sai_uint16_t value type.
     */
    bool                                        isvlan;

    /**
     * @brief Determines whether attribute is ACL field
     *
     * This will become handy for fast determination whether
     * default value is present.
     */
    bool                                        isaclfield;

    /**
     * @brief Determines whether attribute is ACL action
     *
     * This will become handy for fast determination whether
     * default value is present.
     */
    bool                                        isaclaction;

    /**
     * @brief Determines whether attribute is mandatory on create
     */
    bool                                        ismandatoryoncreate;

    /**
     * @brief Determines whether attribute is create only
     */
    bool                                        iscreateonly;

    /**
     * @brief Determines whether attribute is create and set
     */
    bool                                        iscreateandset;

    /**
     * @brief Determines whether attribute is read only
     */
    bool                                        isreadonly;

    /**
     * @brief Determines whether attribute is key
     */
    bool                                        iskey;

    /**
     * @brief Determines whether attribute value is primitive.
     *
     * Primitive values will not contain any pointers so value can be
     * transferred by regular assignment operator.
     */
    bool                                        isprimitive;

    /**
     * @brief Notification type
     *
     * If attribute value type is POINTER then attribute
     * value is pointer to switch notification.
     * Enum sai_switch_notification_type_t is auto generated
     * so it can't be used here, int will be used instead.
     */
    int                                         notificationtype;

    /**
     * @brief Is callback function.
     *
     * Set to true if attribute is callback function but not notification.
     */
    bool                                        iscallback;

    /**
     * @brief Pointer type
     *
     * If attribute value type is POINTER then attribute
     * value is pointer to switch.
     * Enum sai_switch_pointer_type_t is auto generated
     * so it can't be used here, int will be used instead.
     */
    int                                         pointertype;

    /**
     * @brief Attribute capabilities.
     *
     * Represents attribute capability for each specific ASIC. Since each
     * vendor may support different capabilities for each attribute, this field
     * is optional. Also, since SAI API supports multiple switches (switch ids)
     * at the same time, then switches may support different capabilities on
     * different attributes. Vendor ID is provided inside capability struct for
     * difference.
     *
     * This data is designed for vendor internal usage.
     */
    const sai_attr_capability_metadata_t* const* const capability;

    /**
     * @brief Length of attribute capabilities.
     */
    size_t                                      capabilitylength;

    /**
     * @brief Indicates whether attribute is extension attribute.
     */
    bool                                        isextensionattr;

    /**
     * @brief Tells if attribute is a resource type.
     *
     * If true, attribute is used in getting object type availability
     * to distinguish between pools of resources.
     */
    bool                                        isresourcetype;

    /**
     * @brief Indicates whether attribute is deprecated.
     *
     * If true, attribute is deprecated and should not be used. Is up to vendor
     * to check this field and give run time warning about this attribute.
     */
    bool                                        isdeprecated;

} sai_attr_metadata_t;

/*
 * TODO since non object id members can have different type and can be located
 * at different object_key union position, we need to find a way to extract
 * those for automatic serialize/deserialize for example extracting value as
 * sai_attribute_value_t and pointing to right serialize/deserialize functions.
 * Also, an automatic generated functions for serialize/deserialize for those non
 * object id structures must be generated, we don't want to update them manually.
 */

/**
 * @brief Function definition for getting object id from non object
 * id structure member.
 *
 * @param[in] object_meta_key Object meta key
 *
 * @return Object id from struct member
 */
typedef sai_object_id_t (*sai_meta_get_struct_member_oid_fn)(
        _In_ const sai_object_meta_key_t *object_meta_key);

/**
 * @brief Function definition for setting object id from non object
 * id structure member.
 *
 * @param[inout] object_meta_key Object meta key
 * @param[in] object_id Object id to be set
 */
typedef void (*sai_meta_set_struct_member_oid_fn)(
        _Inout_ sai_object_meta_key_t *object_meta_key,
        _In_ sai_object_id_t object_id);

/**
 * @brief Defines struct member info for
 * non object id object type
 */
typedef struct _sai_struct_member_info_t
{
    /**
     * @brief Member value type
     */
    sai_attr_value_type_t                               membervaluetype;

    /**
     * @brief Member name
     */
    const char* const                                   membername;

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
     * @brief Indicates whether member is enum value.
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
     * will get its value.
     */
    const sai_meta_get_struct_member_oid_fn             getoid;

    /**
     * @brief If struct member is OID this function
     * will set its value.
     */
    const sai_meta_set_struct_member_oid_fn             setoid;

    /**
     * @brief Member offset from the struct beginning in bytes.
     *
     * Macro offsetof is used to calculate this field, and it value can be
     * different depending on compiler setting for struct packing.
     */
    size_t                                              offset;

    /**
     * @brief Member size using sizeof operator.
     */
    size_t                                              size;

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
     * This can be NULL if dependency object type
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
 * Generic QUAD API definitions. All APIs can be called using this quad generic
 * functions.
 *
 * When creating switch object or non object id switch_id parameter is ignored,
 * and can be NULL. Currently object type inside sai_object_meta_key_t is
 * ignored and can be skipped.
 *
 * This generic quad API will help us later to call any API, without doing any
 * switch cases for calling different signature functions including non object
 * id structures. Also, later we will generate automatic serialize and
 * deserialize methods for non object id which will deserialize data to object
 * union in sai_object_meta_key_t to right place.
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

typedef sai_status_t (*sai_meta_generic_get_stats_fn)(
        _In_ const sai_object_meta_key_t *meta_key,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

typedef sai_status_t (*sai_meta_generic_get_stats_ext_fn)(
        _In_ const sai_object_meta_key_t *meta_key,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

typedef sai_status_t (*sai_meta_generic_clear_stats_fn)(
        _In_ const sai_object_meta_key_t *meta_key,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

typedef sai_status_t (*sai_generic_create_fn)(
        _Out_ sai_object_id_t *object_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

typedef sai_status_t (*sai_generic_remove_fn)(
        _In_ sai_object_id_t object_id);

typedef sai_status_t (*sai_generic_set_fn)(
        _In_ sai_object_id_t object_id,
        _In_ const sai_attribute_t *attr);

typedef sai_status_t (*sai_generic_get_fn)(
        _In_ sai_object_id_t object_id,
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
    sai_object_type_t                               objecttype;

    /**
     * @brief Object Type name
     */
    const char* const                               objecttypename;

    /**
     * @brief Start of attributes *_START
     */
    sai_attr_id_t                                   attridstart;

    /**
     * @brief End of attributes *_END
     */
    sai_attr_id_t                                   attridend;

    /**
     * @brief Provides enum attr metadata related
     * to this object type.
     */
    const sai_enum_metadata_t* const                enummetadata;

    /**
     * @brief Attributes metadata
     */
    const sai_attr_metadata_t* const* const         attrmetadata;

    /**
     * @brief Attributes metadata length.
     */
    size_t                                          attrmetadatalength;

    /**
     * @brief Indicates if object is using struct
     * instead of actual object id
     */
    bool                                            isnonobjectid;

    /**
     * @brief Indicates if object is OID object
     */
    bool                                            isobjectid;

    /**
     * @brief Defines all struct members
     */
    const sai_struct_member_info_t* const* const    structmembers;

    /**
     * @brief Defines count of struct members
     */
    size_t                                          structmemberscount;

    /**
     * @brief Defines reverse dependency graph members
     */
    const sai_rev_graph_member_t* const* const      revgraphmembers;

    /**
     * @brief Defines reverse dependency graph members count.
     */
    size_t                                          revgraphmemberscount;

    /**
     * @brief Create function pointer.
     */
    const sai_meta_generic_create_fn                create;

    /**
     * @brief Remove function pointer.
     */
    const sai_meta_generic_remove_fn                remove;

    /**
     * @brief Set function pointer.
     */
    const sai_meta_generic_set_fn                   set;

    /**
     * @brief Get function pointer
     */
    const sai_meta_generic_get_fn                   get;

    /**
     * @brief Get stats function pointer.
     */
    const sai_meta_generic_get_stats_fn             getstats;

    /**
     * @brief Get stats extended function pointer.
     */
    const sai_meta_generic_get_stats_ext_fn         getstatsext;

    /**
     * @brief Clear stats function pointer
     */
    const sai_meta_generic_clear_stats_fn           clearstats;

    /**
     * @brief Indicates whether object type is experimental.
     */
    bool                                            isexperimental;

    /**
     * @brief Points to enum sai_OBJECT_TYPE_stat_t if object supports stats.
     */
    const sai_enum_metadata_t* const                statenum;

} sai_object_type_info_t;

/**
 * @}
 */
#endif /** __SAIMETADATATYPES_H_ */
