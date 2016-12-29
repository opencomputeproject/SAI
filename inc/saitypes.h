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
 * @file    saitypes.h
 *
 * @brief   This module defines SAI portable types
 */

#if !defined (__SAITYPES_H_)
#define __SAITYPES_H_

/**
 * @defgroup SAITYPES SAI - Types definitions
 *
 * @{
 */

#if defined(_WIN32)

/*
 * *nix already has lower-case definitions for types.
 */

typedef UINT8  uint8_t;
typedef UINT16 uint16_t;
typedef UINT32 uint32_t;
typedef INT32  int32_t;
typedef INT64  int64_t;
typedef UINT64 uint64_t;

typedef  INT32  sai_status_t;
typedef UINT32  sai_switch_profile_id_t;
typedef UINT16  sai_vlan_id_t;
typedef UINT32  sai_attr_id_t;
typedef UINT8   sai_cos_t;
typedef UINT8   sai_queue_index_t;
typedef UINT8   sai_mac_t[6];
typedef UINT32  sai_ip4_t;
typedef UINT8   sai_ip6_t[16];
typedef UINT32  sai_switch_hash_seed_t;

#include <ws2def.h>
#include <ws2ipdef.h>

#if !defined(__BOOL_DEFINED)

typedef enum {
    false,
    true
} _bool;

#define bool _bool

#endif /* __BOOL_DEFINED */

/**
 * @def PATH_MAX
 * N.B. Equal to 260 on Windows
 */
#define PATH_MAX MAX_PATH

#else /* #if defined(_WIN32) */

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

typedef int32_t  sai_status_t;
typedef uint32_t sai_switch_profile_id_t;
typedef uint16_t sai_vlan_id_t;
typedef uint32_t sai_attr_id_t;
typedef uint8_t  sai_cos_t;
typedef uint8_t  sai_queue_index_t;
typedef uint8_t  sai_mac_t[6];
typedef uint32_t sai_ip4_t;
typedef uint8_t  sai_ip6_t[16];
typedef uint32_t sai_switch_hash_seed_t;

#define _In_
#define _Out_
#define _Inout_
#define _In_reads_z_(_LEN_)
#define _In_reads_opt_z_(_LEN_)

#endif /* _WIN32 */

/*
 * New common definitions
 */

typedef uint64_t sai_uint64_t;
typedef int64_t sai_int64_t;
typedef uint32_t sai_uint32_t;
typedef int32_t sai_int32_t;
typedef uint16_t sai_uint16_t;
typedef int16_t sai_int16_t;
typedef uint8_t sai_uint8_t;
typedef int8_t sai_int8_t;
typedef size_t sai_size_t;
typedef uint64_t sai_object_id_t;
typedef void *sai_pointer_t;
/**
 * @def SAI_NULL_OBJECT_ID
 * Sai NULL object ID
 */
#define SAI_NULL_OBJECT_ID 0L

/**
 * Defines a list of sai object ids used as sai attribute value.
 *
 * In set attribute function call, the count member defines the number of
 * objects.
 *
 * In get attribute function call, the function call returns a list of objects
 * to the caller in the list member. The caller is responsible for allocating the
 * buffer for the list member and set the count member to the size of allocated object
 * list. If the size is large enough to accomodate the list of object id, the
 * callee will then fill the list member and set the count member to the actual
 * number of objects. If the list size is not large enough, the callee will set the
 * count member to the actual number of object id and return
 * #SAI_STATUS_BUFFER_OVERFLOW. Once the caller gets such return code, it should
 * use the returned count member to re-allocate list and retry.
 */
typedef struct _sai_object_list_t {
    uint32_t count;
    sai_object_id_t *list;
} sai_object_list_t;

/**
 * @brief sai common api type
 */
typedef enum _sai_common_api_t {
    SAI_COMMON_API_CREATE = 0,
    SAI_COMMON_API_REMOVE = 1,
    SAI_COMMON_API_SET    = 2,
    SAI_COMMON_API_GET    = 3,
    SAI_COMMON_API_MAX    = 4,
} sai_common_api_t;

/**
 * @brief sai object type
 */
typedef enum _sai_object_type_t {
    SAI_OBJECT_TYPE_NULL                     =  0, /**< invalid object type */
    SAI_OBJECT_TYPE_PORT                     =  1,
    SAI_OBJECT_TYPE_LAG                      =  2,
    SAI_OBJECT_TYPE_VIRTUAL_ROUTER           =  3,
    SAI_OBJECT_TYPE_NEXT_HOP                 =  4,
    SAI_OBJECT_TYPE_NEXT_HOP_GROUP           =  5,
    SAI_OBJECT_TYPE_ROUTER_INTERFACE         =  6,
    SAI_OBJECT_TYPE_ACL_TABLE                =  7,
    SAI_OBJECT_TYPE_ACL_ENTRY                =  8,
    SAI_OBJECT_TYPE_ACL_COUNTER              =  9,
    SAI_OBJECT_TYPE_ACL_RANGE                = 10,
    SAI_OBJECT_TYPE_ACL_TABLE_GROUP          = 11,
    SAI_OBJECT_TYPE_ACL_TABLE_GROUP_MEMBER   = 12,
    SAI_OBJECT_TYPE_HOSTIF                   = 13,
    SAI_OBJECT_TYPE_MIRROR_SESSION           = 14,
    SAI_OBJECT_TYPE_SAMPLEPACKET             = 15,
    SAI_OBJECT_TYPE_STP                      = 16,
    SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP        = 17,
    SAI_OBJECT_TYPE_POLICER                  = 18,
    SAI_OBJECT_TYPE_WRED                     = 19,
    SAI_OBJECT_TYPE_QOS_MAP                  = 20,
    SAI_OBJECT_TYPE_QUEUE                    = 21,
    SAI_OBJECT_TYPE_SCHEDULER                = 22,
    SAI_OBJECT_TYPE_SCHEDULER_GROUP          = 23,
    SAI_OBJECT_TYPE_BUFFER_POOL              = 24,
    SAI_OBJECT_TYPE_BUFFER_PROFILE           = 25,
    SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP   = 26,
    SAI_OBJECT_TYPE_LAG_MEMBER               = 27,
    SAI_OBJECT_TYPE_HASH                     = 28,
    SAI_OBJECT_TYPE_UDF                      = 29,
    SAI_OBJECT_TYPE_UDF_MATCH                = 30,
    SAI_OBJECT_TYPE_UDF_GROUP                = 31,
    SAI_OBJECT_TYPE_FDB_ENTRY                = 32,
    SAI_OBJECT_TYPE_SWITCH                   = 33,
    SAI_OBJECT_TYPE_HOSTIF_TRAP              = 34,
    SAI_OBJECT_TYPE_HOSTIF_TABLE_ENTRY       = 35,
    SAI_OBJECT_TYPE_NEIGHBOR_ENTRY           = 36,
    SAI_OBJECT_TYPE_ROUTE_ENTRY              = 37,
    SAI_OBJECT_TYPE_VLAN                     = 38,
    SAI_OBJECT_TYPE_VLAN_MEMBER              = 39,
    SAI_OBJECT_TYPE_HOSTIF_PACKET            = 40,
    SAI_OBJECT_TYPE_TUNNEL_MAP               = 41,
    SAI_OBJECT_TYPE_TUNNEL                   = 42,
    SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY  = 43,
    SAI_OBJECT_TYPE_FDB_FLUSH                = 44,
    SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER    = 45,
    SAI_OBJECT_TYPE_STP_PORT                 = 46,
    SAI_OBJECT_TYPE_RPF_GROUP                = 47,
    SAI_OBJECT_TYPE_RPF_GROUP_MEMBER         = 48,
    SAI_OBJECT_TYPE_L2MC_GROUP               = 49,
    SAI_OBJECT_TYPE_L2MC_GROUP_MEMBER        = 50,
    SAI_OBJECT_TYPE_IPMC_GROUP               = 51,
    SAI_OBJECT_TYPE_IPMC_GROUP_MEMBER        = 52,
    SAI_OBJECT_TYPE_L2MC_ENTRY               = 53,
    SAI_OBJECT_TYPE_IPMC_ENTRY               = 54,
    SAI_OBJECT_TYPE_MCAST_FDB_ENTRY          = 55,
    SAI_OBJECT_TYPE_HOSTIF_USER_DEFINED_TRAP = 56,
    SAI_OBJECT_TYPE_BRIDGE                   = 57,
    SAI_OBJECT_TYPE_BRIDGE_PORT              = 58,
    SAI_OBJECT_TYPE_MAX                      = 59
} sai_object_type_t;

typedef struct _sai_u8_list_t {
    uint32_t count;
    uint8_t *list;
} sai_u8_list_t;

/**
 * @brief Defines a s8 list or string.
 * string should be null terminated and count should include '\0'
 */
typedef struct _sai_s8_list_t {
    uint32_t count;
    int8_t *list;
} sai_s8_list_t;

typedef struct _sai_u16_list_t {
    uint32_t count;
    uint16_t *list;
} sai_u16_list_t;

typedef struct _sai_s16_list_t {
    uint32_t count;
    int16_t *list;
} sai_s16_list_t;

typedef struct _sai_u32_list_t {
    uint32_t count;
    uint32_t *list;
} sai_u32_list_t;

typedef struct _sai_s32_list_t {
    uint32_t count;
    int32_t *list;
} sai_s32_list_t;

typedef struct _sai_u32_range_t {
    uint32_t min;
    uint32_t max;
} sai_u32_range_t;

typedef struct _sai_s32_range_t {
    int32_t min;
    int32_t max;
} sai_s32_range_t;

/**
 * @brief Defines a vlan list datastructure
 */
typedef struct _sai_vlan_list_t {

    /** Number of Vlans*/
    uint32_t count;

    /** List of Vlans*/
    sai_vlan_id_t *list;

} sai_vlan_list_t;

typedef enum _sai_ip_addr_family_t
{
    SAI_IP_ADDR_FAMILY_IPV4,

    SAI_IP_ADDR_FAMILY_IPV6

} sai_ip_addr_family_t;

typedef struct _sai_ip_address_t {
    sai_ip_addr_family_t addr_family;
    union {
        sai_ip4_t ip4;
        sai_ip6_t ip6;
    } addr;
} sai_ip_address_t;

typedef struct _sai_ip_prefix_t {
    sai_ip_addr_family_t addr_family;
    union {
        sai_ip4_t ip4;
        sai_ip6_t ip6;
    } addr;
    union {
        sai_ip4_t ip4;
        sai_ip6_t ip6;
    } mask;
} sai_ip_prefix_t;

/**
 * @brief Defines a single ACL filter
 *
 * @note IPv4 and IPv6 Address expected in Network Byte Order
 */
typedef struct _sai_acl_field_data_t
{
    /**
     * match enable/disable
     */
    bool enable;

    /**
     * @brief Field match mask
     */
    union {
        sai_uint8_t u8;
        sai_int8_t s8;
        sai_uint16_t u16;
        sai_int16_t s16;
        sai_uint32_t u32;
        sai_int32_t s32;
        sai_mac_t mac;
        sai_ip4_t ip4;
        sai_ip6_t ip6;
        sai_u8_list_t u8list;
    } mask;

    /**
     * @brief Expected AND result using match mask above with packet field value where applicable.
     */
    union {
        bool booldata;
        sai_uint8_t u8;
        sai_int8_t s8;
        sai_uint16_t u16;
        sai_int16_t s16;
        sai_uint32_t u32;
        sai_int32_t s32;
        sai_mac_t mac;
        sai_ip4_t ip4;
        sai_ip6_t ip6;
        sai_object_id_t oid;
        sai_object_list_t objlist;
        sai_u8_list_t u8list;
    } data;
} sai_acl_field_data_t;

/**
 * @brief Defines a single ACL action
 *
 * @note IPv4 and IPv6 Address expected in Network Byte Order
 */
typedef struct _sai_acl_action_data_t
{
    /**
     * @brief Action enable/disable
     */
    bool enable;

    /**
     * @brief Action parameter
     */
    union {
        sai_uint8_t u8;
        sai_int8_t s8;
        sai_uint16_t u16;
        sai_int16_t s16;
        sai_uint32_t u32;
        sai_int32_t s32;
        sai_mac_t mac;
        sai_ip4_t ip4;
        sai_ip6_t ip6;
        sai_object_id_t oid;
        sai_object_list_t objlist;
    } parameter;

} sai_acl_action_data_t;

/**
 * @brief Packet Color
 */
typedef enum _sai_packet_color_t
{
    /**
     * @brief Color Green
     */
    SAI_PACKET_COLOR_GREEN,

    /**
     * @brief Color Yellow
     */
    SAI_PACKET_COLOR_YELLOW,

    /**
     * @brief Color Red
     */
    SAI_PACKET_COLOR_RED,

} sai_packet_color_t;

/**
 * @brief Defines qos map types.
 *
 * @par Examples:
 *
 * dot1p/dscp --> TC
 * dot1p/dscp --> Color
 * dot1p/dscp --> TC + Color
 * Tc --> dot1p/Dscp.
 * Tc + color --> dot1p/Dscp.
 * Tc --> Egress Queue.
 */
typedef struct _sai_qos_map_params_t
{
    /** Traffic class */
    sai_cos_t tc;

    /** DSCP value */
    sai_uint8_t dscp;

    /** Dot1p value */
    sai_uint8_t dot1p;

    /** PFC priority value */
    sai_uint8_t prio;

    /** Priority group value */
    sai_uint8_t pg;

    /** Egress port queue UOID is not known at the time of map creation.
     * Using queue index for maps. */
    sai_queue_index_t queue_index;

    /** Color of the packet */
    sai_packet_color_t color;

} sai_qos_map_params_t;

typedef struct _sai_qos_map_t
{
    /** Input parameters to match */
    sai_qos_map_params_t key;

    /** Output map parameters */
    sai_qos_map_params_t value;

} sai_qos_map_t;

typedef struct _sai_qos_map_list_t
{
    /** Number of entries in the map */
    uint32_t count;
    /** Map list */
    sai_qos_map_t *list;

} sai_qos_map_list_t;

typedef struct _sai_tunnel_map_params_t
{
    /** inner ECN */
    sai_uint8_t oecn;

    /** outer ECN */
    sai_uint8_t uecn;

    /** vlan id */
    sai_vlan_id_t vlan_id;

    /** VNI id */
    sai_uint32_t vni_id;

    /** Bridge IF */
    sai_object_id_t bridge_if;

} sai_tunnel_map_params_t;

typedef struct _sai_tunnel_map_t
{
    /** Input parameters to match */
    sai_tunnel_map_params_t key;

    /** Output map parameters */
    sai_tunnel_map_params_t value;

} sai_tunnel_map_t;

typedef struct _sai_tunnel_map_list_t
{
    /** Number of entries in the map */
    uint32_t count;

    /** Map list */
    sai_tunnel_map_t * list;

} sai_tunnel_map_list_t;

/**
 * @brief Structure for acl attributes supported at each stage.
 * action_list alone is added now. Qualifier list can also be added
 * when needed.
 */
typedef struct _sai_acl_capability_t
{
    /**
     * @brief Type of acl stage. Input to get the action list
     * Failure to pass the stage as input will be treated as error
     */
    sai_int32_t stage;

    /**
     * @brief Output from get function.
     * boolean indicating whether action list is mandatory for table creation
     */
    bool is_action_list_mandatory;

    /**
     * @brief Output from get function.
     *
     * List of actions supported per stage from the sai_acl_table_action_list_t.
     * Max action list can be obtained using the #SAI_SWITCH_ATTR_MAX_ACL_ACTION_COUNT
     */
    sai_s32_list_t action_list;
}sai_acl_capability_t;

/**
 * @brief FDB entry type.
 */
typedef enum _sai_fdb_entry_bridge_type_t
{
    /** .1Q FDB Entry */
    SAI_FDB_ENTRY_BRIDGE_TYPE_1Q,

    /** .1D FDB Entry */
    SAI_FDB_ENTRY_BRIDGE_TYPE_1D,

} sai_fdb_entry_bridge_type_t;

/**
 * @brief Data Type to use enum's as attribute value is sai_int32_t s32
 */
typedef union {
    bool booldata;
    char chardata[32];
    sai_uint8_t u8;
    sai_int8_t s8;
    sai_uint16_t u16;
    sai_int16_t s16;
    sai_uint32_t u32;
    sai_int32_t s32;
    sai_uint64_t u64;
    sai_int64_t s64;
    sai_pointer_t ptr;
    sai_mac_t mac;
    sai_ip4_t ip4;
    sai_ip6_t ip6;
    sai_ip_address_t ipaddr;
    sai_object_id_t oid;
    sai_object_list_t objlist;
    sai_u8_list_t u8list;
    sai_s8_list_t s8list;
    sai_u16_list_t u16list;
    sai_s16_list_t s16list;
    sai_u32_list_t u32list;
    sai_s32_list_t s32list;
    sai_u32_range_t u32range;
    sai_s32_range_t s32range;
    sai_vlan_list_t vlanlist;
    sai_acl_field_data_t aclfield;
    sai_acl_action_data_t aclaction;
    sai_qos_map_list_t qosmap;
    sai_tunnel_map_list_t tunnelmap;
    sai_acl_capability_t aclcapability;

} sai_attribute_value_t;

typedef struct _sai_attribute_t {
    sai_attr_id_t id;
    sai_attribute_value_t value;
} sai_attribute_t;

/**
 * @}
 */
#endif /** __SAITYPES_H_ */
