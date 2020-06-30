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
typedef UINT32  sai_label_id_t;
typedef UINT32  sai_stat_id_t;
typedef UINT8   sai_macsec_sak_t[32];
typedef UINT8   sai_macsec_auth_key_t[16];
typedef UINT8   sai_macsec_salt_t[12];

#include <ws2def.h>
#include <ws2ipdef.h>

#if !defined(__BOOL_DEFINED)

typedef enum
{
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
typedef uint32_t sai_label_id_t;
typedef uint32_t sai_stat_id_t;
typedef uint8_t sai_macsec_sak_t[32];
typedef uint8_t sai_macsec_auth_key_t[16];
typedef uint8_t sai_macsec_salt_t[12];

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

typedef struct _sai_timespec_t
{
    uint64_t tv_sec;
    uint32_t tv_nsec;
} sai_timespec_t;

/**
 * @def SAI_NULL_OBJECT_ID
 * SAI NULL object ID
 */
#define SAI_NULL_OBJECT_ID 0L

/**
 * @brief Defines a list of SAI object ids used as SAI attribute value.
 *
 * In set attribute function call, the count member defines the number of
 * objects.
 *
 * In get attribute function call, the function call returns a list of objects
 * to the caller in the list member. The caller is responsible for allocating the
 * buffer for the list member and set the count member to the size of allocated object
 * list. If the size is large enough to accommodate the list of object id, the
 * callee will then fill the list member and set the count member to the actual
 * number of objects. If the list size is not large enough, the callee will set the
 * count member to the actual number of object id and return
 * #SAI_STATUS_BUFFER_OVERFLOW. Once the caller gets such return code, it should
 * use the returned count member to re-allocate list and retry.
 */
typedef struct _sai_object_list_t
{
    uint32_t count;
    sai_object_id_t *list;
} sai_object_list_t;

/**
 * @brief SAI common API type
 */
typedef enum _sai_common_api_t
{
    SAI_COMMON_API_CREATE      = 0,
    SAI_COMMON_API_REMOVE      = 1,
    SAI_COMMON_API_SET         = 2,
    SAI_COMMON_API_GET         = 3,
    SAI_COMMON_API_BULK_CREATE = 4,
    SAI_COMMON_API_BULK_REMOVE = 5,
    SAI_COMMON_API_BULK_SET    = 6,
    SAI_COMMON_API_BULK_GET    = 7,
    SAI_COMMON_API_MAX         = 8,
} sai_common_api_t;

/**
 * @brief SAI object type
 */
typedef enum _sai_object_type_t
{
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
    SAI_OBJECT_TYPE_TUNNEL_MAP_ENTRY         = 59,
    SAI_OBJECT_TYPE_TAM                      = 60,
    SAI_OBJECT_TYPE_SEGMENTROUTE_SIDLIST     = 61,
    SAI_OBJECT_TYPE_PORT_POOL                = 62,
    SAI_OBJECT_TYPE_INSEG_ENTRY              = 63,
    SAI_OBJECT_TYPE_DTEL                     = 64, /**< experimental */
    SAI_OBJECT_TYPE_DTEL_QUEUE_REPORT        = 65, /**< experimental */
    SAI_OBJECT_TYPE_DTEL_INT_SESSION         = 66, /**< experimental */
    SAI_OBJECT_TYPE_DTEL_REPORT_SESSION      = 67, /**< experimental */
    SAI_OBJECT_TYPE_DTEL_EVENT               = 68, /**< experimental */
    SAI_OBJECT_TYPE_BFD_SESSION              = 69,
    SAI_OBJECT_TYPE_ISOLATION_GROUP          = 70,
    SAI_OBJECT_TYPE_ISOLATION_GROUP_MEMBER   = 71,
    SAI_OBJECT_TYPE_TAM_MATH_FUNC            = 72,
    SAI_OBJECT_TYPE_TAM_REPORT               = 73,
    SAI_OBJECT_TYPE_TAM_EVENT_THRESHOLD      = 74,
    SAI_OBJECT_TYPE_TAM_TEL_TYPE             = 75,
    SAI_OBJECT_TYPE_TAM_TRANSPORT            = 76,
    SAI_OBJECT_TYPE_TAM_TELEMETRY            = 77,
    SAI_OBJECT_TYPE_TAM_COLLECTOR            = 78,
    SAI_OBJECT_TYPE_TAM_EVENT_ACTION         = 79,
    SAI_OBJECT_TYPE_TAM_EVENT                = 80,
    SAI_OBJECT_TYPE_NAT_ZONE_COUNTER         = 81,
    SAI_OBJECT_TYPE_NAT_ENTRY                = 82,
    SAI_OBJECT_TYPE_TAM_INT                  = 83,
    SAI_OBJECT_TYPE_COUNTER                  = 84,
    SAI_OBJECT_TYPE_DEBUG_COUNTER            = 85,
    SAI_OBJECT_TYPE_PORT_CONNECTOR           = 86,
    SAI_OBJECT_TYPE_PORT_SERDES              = 87,
    SAI_OBJECT_TYPE_MACSEC                   = 88,
    SAI_OBJECT_TYPE_MACSEC_PORT              = 89,
    SAI_OBJECT_TYPE_MACSEC_FLOW              = 90,
    SAI_OBJECT_TYPE_MACSEC_SC                = 91,
    SAI_OBJECT_TYPE_MACSEC_SA                = 92,
    SAI_OBJECT_TYPE_SYSTEM_PORT              = 93,
    SAI_OBJECT_TYPE_FINE_GRAINED_HASH_FIELD  = 94,
    SAI_OBJECT_TYPE_MAX,  /* Must remain in last position */
} sai_object_type_t;

typedef struct _sai_u8_list_t
{
    uint32_t count;
    uint8_t *list;
} sai_u8_list_t;

/**
 * @brief Defines a s8 list or string
 *
 * String should be null terminated and count should include '\0'.
 */
typedef struct _sai_s8_list_t
{
    uint32_t count;
    int8_t *list;
} sai_s8_list_t;

typedef struct _sai_u16_list_t
{
    uint32_t count;
    uint16_t *list;
} sai_u16_list_t;

typedef struct _sai_s16_list_t
{
    uint32_t count;
    int16_t *list;
} sai_s16_list_t;

typedef struct _sai_u32_list_t
{
    uint32_t count;
    uint32_t *list;
} sai_u32_list_t;

typedef struct _sai_s32_list_t
{
    uint32_t count;
    int32_t *list;
} sai_s32_list_t;

typedef struct _sai_u32_range_t
{
    uint32_t min;
    uint32_t max;
} sai_u32_range_t;

typedef struct _sai_s32_range_t
{
    int32_t min;
    int32_t max;
} sai_s32_range_t;

/**
 * @brief Defines a vlan list data structure
 */
typedef struct _sai_vlan_list_t
{
    /** Number of VLANs */
    uint32_t count;

    /** List of VLANs */
    sai_vlan_id_t *list;

} sai_vlan_list_t;

typedef enum _sai_ip_addr_family_t
{
    SAI_IP_ADDR_FAMILY_IPV4,

    SAI_IP_ADDR_FAMILY_IPV6

} sai_ip_addr_family_t;

/**
 * @extraparam sai_ip_addr_family_t addr_family
 */
typedef union _sai_ip_addr_t
{
    /** @validonly addr_family == SAI_IP_ADDR_FAMILY_IPV4 */
    sai_ip4_t ip4;

    /** @validonly addr_family == SAI_IP_ADDR_FAMILY_IPV6 */
    sai_ip6_t ip6;
} sai_ip_addr_t;

typedef struct _sai_ip_address_t
{
    sai_ip_addr_family_t addr_family;

    /** @passparam addr_family */
    sai_ip_addr_t addr;
} sai_ip_address_t;

typedef struct _sai_ip_address_list_t
{
    uint32_t count;
    sai_ip_address_t *list;
} sai_ip_address_list_t;

typedef struct _sai_ip_prefix_t
{
    sai_ip_addr_family_t addr_family;

    /** @passparam addr_family */
    sai_ip_addr_t addr;

    /** @passparam addr_family */
    sai_ip_addr_t mask;
} sai_ip_prefix_t;

/**
 * @brief Field match mask
 *
 * @extraparam const sai_attr_metadata_t *meta
 */
typedef union _sai_acl_field_data_mask_t
{
    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8 */
    sai_uint8_t u8;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8 */
    sai_int8_t s8;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16 */
    sai_uint16_t u16;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16 */
    sai_int16_t s16;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32 */
    sai_uint32_t u32;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32 */
    sai_int32_t s32;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT64 */
    sai_uint64_t u64;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC */
    sai_mac_t mac;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4 */
    sai_ip4_t ip4;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6 */
    sai_ip6_t ip6;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST */
    sai_u8_list_t u8list;
} sai_acl_field_data_mask_t;

/**
 * @brief ACL field data union.
 *
 * @extraparam const sai_attr_metadata_t *meta
 */
typedef union _sai_acl_field_data_data_t
{
    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_BOOL */
    bool booldata;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8 */
    sai_uint8_t u8;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8 */
    sai_int8_t s8;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16 */
    sai_uint16_t u16;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16 */
    sai_int16_t s16;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32 */
    sai_uint32_t u32;

    /**
     * @suffix enum
     * @passparam meta->enummetadata
     * @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32
     */
    sai_int32_t s32;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT64 */
    sai_uint64_t u64;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC */
    sai_mac_t mac;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4 */
    sai_ip4_t ip4;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6 */
    sai_ip6_t ip6;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID */
    sai_object_id_t oid;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST */
    sai_object_list_t objlist;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST */
    sai_u8_list_t u8list;
} sai_acl_field_data_data_t;

/**
 * @brief Defines a single ACL filter
 *
 * @note IPv4 and IPv6 Address expected in Network Byte Order
 *
 * @extraparam const sai_attr_metadata_t *meta
 */
typedef struct _sai_acl_field_data_t
{
    /**
     * @brief Match enable/disable
     */
    bool enable;

    /**
     * @brief Field match mask
     *
     * @note Nothing can be serialized if mask is not needed for data items
     * like object id.
     *
     * @passparam meta
     * @validonly enable == true
     */
    sai_acl_field_data_mask_t mask;

    /**
     * @brief Expected AND result using match mask above with packet field
     * value where applicable.
     *
     * @passparam meta
     * @validonly enable == true
     */
    sai_acl_field_data_data_t data;
} sai_acl_field_data_t;

/**
 * @extraparam const sai_attr_metadata_t *meta
 */
typedef union _sai_acl_action_parameter_t
{
    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_BOOL */
    bool booldata;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8 */
    sai_uint8_t u8;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8 */
    sai_int8_t s8;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16 */
    sai_uint16_t u16;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16 */
    sai_int16_t s16;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32 */
    sai_uint32_t u32;

    /**
     * @suffix enum
     * @passparam meta->enummetadata
     * @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32
     */
    sai_int32_t s32;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC */
    sai_mac_t mac;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4 */
    sai_ip4_t ip4;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6 */
    sai_ip6_t ip6;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID */
    sai_object_id_t oid;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST */
    sai_object_list_t objlist;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IP_ADDRESS */
    sai_ip_address_t ipaddr;
} sai_acl_action_parameter_t;

/**
 * @brief Defines a single ACL action
 *
 * @note IPv4 and IPv6 Address expected in Network Byte Order
 *
 * @extraparam const sai_attr_metadata_t *meta
 */
typedef struct _sai_acl_action_data_t
{
    /**
     * @brief Action enable/disable
     */
    bool enable;

    /**
     * @brief Action parameter
     *
     * @passparam meta
     * @validonly enable == true
     */
    sai_acl_action_parameter_t parameter;

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
 * @brief Defines QOS map types.
 *
 * @par Examples:
 *
 * dot1p/DSCP/MPLS_EXP --> TC
 * dot1p/DSCP/MPLS_EXP --> Color
 * dot1p/DSCP/MPLS_EXP --> TC + Color
 * TC --> dot1p/DSCP/MPLS_EXP.
 * TC + color --> dot1p/DSCP/MPLS_EXP.
 * TC --> Egress Queue.
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

    /**
     * @brief Egress port queue OID is not known at the time of map creation.
     * Using queue index for maps.
     */
    sai_queue_index_t queue_index;

    /** Color of the packet */
    sai_packet_color_t color;

    /** MPLS exp value */
    sai_uint8_t mpls_exp;

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

typedef struct _sai_map_t
{
    /** Input key value */
    sai_uint32_t key;

    /** Input data value for the key */
    sai_int32_t value;

} sai_map_t;

typedef struct _sai_map_list_t
{
    /** Number of entries in the map */
    uint32_t count;

    /** Map list */
    sai_map_t *list;

} sai_map_list_t;

/**
 * @brief Structure for ACL attributes supported at each stage.
 * action_list alone is added now. Qualifier list can also be added
 * when needed.
 */
typedef struct _sai_acl_capability_t
{
    /**
     * @brief Output from get function.
     *
     * Flag indicating whether action list is mandatory for table creation.
     */
    bool is_action_list_mandatory;

    /**
     * @brief Output from get function.
     *
     * List of actions supported per stage from the sai_acl_table_action_list_t.
     * Max action list can be obtained using the #SAI_SWITCH_ATTR_MAX_ACL_ACTION_COUNT.
     *
     * @suffix enum_list
     * @passparam &sai_metadata_enum_sai_acl_action_type_t
     */
    sai_s32_list_t action_list;
} sai_acl_capability_t;

/**
 * @brief Attribute data for SAI_ACL_TABLE_ATTR_STAGE
 */
typedef enum _sai_acl_stage_t
{
    /** Ingress Stage */
    SAI_ACL_STAGE_INGRESS,

    /** Egress Stage */
    SAI_ACL_STAGE_EGRESS,

    /** Ingress Stage */
    SAI_ACL_STAGE_INGRESS_MACSEC,

    /** Egress Stage */
    SAI_ACL_STAGE_EGRESS_MACSEC,

} sai_acl_stage_t;

/**
 * @brief Attribute data for SAI_ACL_TABLE_ATTR_BIND_POINT
 */
typedef enum _sai_acl_bind_point_type_t
{
    /** Bind Point Type Port */
    SAI_ACL_BIND_POINT_TYPE_PORT,

    /** Bind Point Type LAG */
    SAI_ACL_BIND_POINT_TYPE_LAG,

    /** Bind Point Type VLAN */
    SAI_ACL_BIND_POINT_TYPE_VLAN,

    /** Bind Point Type RIF */
    SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE,

    /** @ignore - for backward compatibility */
    SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF = SAI_ACL_BIND_POINT_TYPE_ROUTER_INTERFACE,

    /** Bind Point Type Switch */
    SAI_ACL_BIND_POINT_TYPE_SWITCH

} sai_acl_bind_point_type_t;

/**
 * @brief Attribute data for SAI_TAM_TABLE_ATTR_BIND_POINT
 */
typedef enum _sai_tam_bind_point_type_t
{
    /** Bind Point Type Queue */
    SAI_TAM_BIND_POINT_TYPE_QUEUE,

    /** Bind Point Type Port */
    SAI_TAM_BIND_POINT_TYPE_PORT,

    /** Bind Point Type LAG */
    SAI_TAM_BIND_POINT_TYPE_LAG,

    /** Bind Point Type VLAN */
    SAI_TAM_BIND_POINT_TYPE_VLAN,

    /** Bind Point Type Switch */
    SAI_TAM_BIND_POINT_TYPE_SWITCH,

    /** Bind Point Type Ingress Priority Group */
    SAI_TAM_BIND_POINT_TYPE_IPG,

    /** Bind Point Type Buffer Service Pool */
    SAI_TAM_BIND_POINT_TYPE_BSP,

} sai_tam_bind_point_type_t;

/**
 * @brief Structure for ACL Resource Count
 */
typedef struct _sai_acl_resource_t
{
    /** ACL stage */
    sai_acl_stage_t stage;

    /** ACL Bind point */
    sai_acl_bind_point_type_t bind_point;

    /** Available number of entries */
    sai_uint32_t avail_num;

} sai_acl_resource_t;

/**
 * @brief List of available ACL resources at each stage and
 * each binding point. This shall be returned when queried for
 * SAI_SWITCH_ATTR_AVAILABLE_ACL_TABLE or
 * SAI_SWITCH_ATTR_AVAILABLE_ACL_TABLE_GROUP
 */
typedef struct _sai_acl_resource_list_t
{
    /** Number of entries */
    uint32_t count;

    /** Resource list */
    sai_acl_resource_t *list;

} sai_acl_resource_list_t;

/**
 * @brief Segment Routing Tag Length Value Types
 */
typedef enum _sai_tlv_type_t
{
    /** Ingress Tag Length Value */
    SAI_TLV_TYPE_INGRESS,

    /** Egress Tag Length Value */
    SAI_TLV_TYPE_EGRESS,

    /** Opaque Tag Length Value */
    SAI_TLV_TYPE_OPAQUE,

    /** Hash-based Message Authentication Code Tag Length Value */
    SAI_TLV_TYPE_HMAC
} sai_tlv_type_t;

/**
 * @brief Segment Routing Hash-based Message Authentication Code Tag Length Value Format
 */
typedef struct _sai_hmac_t
{
    sai_uint32_t key_id;

    sai_uint32_t hmac[8];
} sai_hmac_t;

/**
 * @extraparam sai_tlv_type_t tlv_type
 */
typedef union _sai_tlv_entry_t
{
    /** @validonly tlv_type == SAI_TLV_TYPE_INGRESS */
    sai_ip6_t ingress_node;

    /** @validonly tlv_type == SAI_TLV_TYPE_EGRESS */
    sai_ip6_t egress_node;

    /** @validonly tlv_type == SAI_TLV_TYPE_OPAQUE */
    sai_uint32_t opaque_container[4];

    /** @validonly tlv_type == SAI_TLV_TYPE_HMAC */
    sai_hmac_t hmac;
} sai_tlv_entry_t;

/**
 * @brief Segment Routing Tag Length Value entry
 */
typedef struct _sai_tlv_t
{
    sai_tlv_type_t tlv_type;

    /** @passparam tlv_type */
    sai_tlv_entry_t entry;
} sai_tlv_t;

/**
 * @brief List of Segment Routing Tag Length Value entries
 */
typedef struct _sai_tlv_list_t
{
    /** Number of Tag Length Value entries */
    uint32_t count;

    /** Tag Length Value list */
    sai_tlv_t *list;
} sai_tlv_list_t;

/**
 * @brief List of Segment Routing segment entries
 */
typedef struct _sai_segment_list_t
{
    /** Number of IPv6 Segment Route entries */
    uint32_t count;

    /** Segment list */
    sai_ip6_t *list;
} sai_segment_list_t;

/**
 * @brief Defines a lane with its eye values with the up and down values
 * being in mV and left and right being in mUI.
 */
typedef struct _sai_port_lane_eye_values_t
{
    uint32_t lane;
    int32_t left;
    int32_t right;
    int32_t up;
    int32_t down;
} sai_port_lane_eye_values_t;

/**
 * @brief Defines a port's lanes eye values list
 *
 * In get_port_attribute function call, the count member defines the number
 * of objects which will be returned to the caller in the list member. The
 * caller must allocate the buffer for the list member and set the count
 * member to the size of the allocated objects in the list member.
 *
 * If the size is large enough to accommodate the list of objects, the
 * callee must fill the list member and set the count member to the actual
 * number of objects filled. If the size is not large enough, the callee
 * must set the count member to the actual number of objects filled in the
 * list member and return #SAI_STATUS_BUFFER_OVERFLOW. Once the caller
 * gets such a return code, it may use the returned count member to
 * re-allocate the list and retry.
 */
typedef struct _sai_port_eye_values_list_t
{
    uint32_t count;
    sai_port_lane_eye_values_t *list;
} sai_port_eye_values_list_t;

/**
 * @brief Enum defining MPLS out segment type
 */
typedef enum _sai_outseg_type_t
{
    /** Out segment of ingress node, label stack depth is at least one */
    SAI_OUTSEG_TYPE_PUSH,

    /** Out segment of intermediate node, label stack depth is one */
    SAI_OUTSEG_TYPE_SWAP,

} sai_outseg_type_t;

/**
 * @brief Enum defining TTL mode for MPLS out segment
 */
typedef enum _sai_outseg_ttl_mode_t
{
    SAI_OUTSEG_TTL_MODE_UNIFORM,

    SAI_OUTSEG_TTL_MODE_PIPE,

} sai_outseg_ttl_mode_t;

/**
 * @brief Enum defining MPLS EXP mode for MPLS out segment
 */
typedef enum _sai_outseg_exp_mode_t
{
    SAI_OUTSEG_EXP_MODE_UNIFORM,

    SAI_OUTSEG_EXP_MODE_PIPE,

} sai_outseg_exp_mode_t;

/**
 * @brief System port configuration attributes
 *
 * Speed parameter should be the same value as SAI_PORT_ATTR_SPEED.
 * This is used for VOQ scheduling.
 *
 * All elements are mandatory
 */
typedef struct _sai_system_port_config_t
{
    /** System Port ID */
    uint32_t port_id;

    /** Switch ID of where the system port exists */
    uint32_t attached_switch_id;

    /** Core associated with the system port */
    uint32_t attached_core_index;

    /** Port Index within the core associated with the system port */
    uint32_t attached_core_port_index;

    /** Speed of the system port */
    uint32_t speed;

    /** Number of Virtual Output Queues associated with the system port */
    uint32_t num_voq;
} sai_system_port_config_t;

/**
 * @brief System port configuration list
 */
typedef struct _sai_system_port_config_list_t
{
    /** Number of entries in the list */
    uint32_t count;

    /** System port configuration list */
    sai_system_port_config_t *list;
} sai_system_port_config_list_t;

/**
 * @brief Fabric port reachability
 */
typedef struct _sai_fabric_port_reachability_t
{
    /** Remote switch ID (SAI_SWITCH_TYPE_NPU) */
    uint32_t switch_id;

    /** Remote switch ID is reachable through the fabric port */
    bool reachable;
} sai_fabric_port_reachability_t;

/**
 * @brief Port error status
 */
typedef enum _sai_port_err_status_t
{
    /** Data Unit CRC Error */
    SAI_PORT_ERR_STATUS_DATA_UNIT_CRC_ERROR,

    /** Data Unit Size Error */
    SAI_PORT_ERR_STATUS_DATA_UNIT_SIZE,

    /** Data Unit Misalignment Error */
    SAI_PORT_ERR_STATUS_DATA_UNIT_MISALIGNMENT_ERROR,

    /** Uncorrectable RS-FEC code word error */
    SAI_PORT_ERR_STATUS_CODE_GROUP_ERROR,

    /** SerDes Signal is out of sync */
    SAI_PORT_ERR_STATUS_SIGNAL_LOCAL_ERROR,

    /** Port is not accepting reachability data units */
    SAI_PORT_ERR_STATUS_NO_RX_REACHABILITY,

    /** Rate of data units with CRC errors passed its threshold */
    SAI_PORT_ERR_STATUS_CRC_RATE,

    /** Error remote fault indication */
    SAI_PORT_ERR_STATUS_REMOTE_FAULT_STATUS,

    /** Error status max */
    SAI_PORT_ERR_STATUS_MAX,
} sai_port_err_status_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_ERR_STATUS_LIST
 */
typedef struct _sai_port_err_status_list_t
{
    /** Number of entries in the list */
    uint32_t count;

    /** Port error list */
    sai_port_err_status_t *list;
} sai_port_err_status_list_t;

/**
 * @brief Data Type
 *
 * To use enum values as attribute value is sai_int32_t s32
 *
 * @extraparam const sai_attr_metadata_t *meta
 */
typedef union _sai_attribute_value_t
{
    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_BOOL */
    bool booldata;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_CHARDATA */
    char chardata[32];

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT8 */
    sai_uint8_t u8;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_INT8 */
    sai_int8_t s8;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT16 */
    sai_uint16_t u16;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_INT16 */
    sai_int16_t s16;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT32 */
    sai_uint32_t u32;

    /**
     * @suffix enum
     * @passparam meta->enummetadata
     * @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_INT32
     */
    sai_int32_t s32;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT64 */
    sai_uint64_t u64;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_INT64 */
    sai_int64_t s64;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_POINTER */
    sai_pointer_t ptr;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_MAC */
    sai_mac_t mac;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_IPV4 */
    sai_ip4_t ip4;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_IPV6 */
    sai_ip6_t ip6;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_IP_ADDRESS */
    sai_ip_address_t ipaddr;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_IP_PREFIX */
    sai_ip_prefix_t ipprefix;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_OBJECT_ID */
    sai_object_id_t oid;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_OBJECT_LIST */
    sai_object_list_t objlist;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT8_LIST */
    sai_u8_list_t u8list;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_INT8_LIST */
    sai_s8_list_t s8list;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT16_LIST */
    sai_u16_list_t u16list;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_INT16_LIST */
    sai_s16_list_t s16list;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT32_LIST */
    sai_u32_list_t u32list;

    /**
     * @suffix enum_list
     * @passparam meta->enummetadata
     * @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_INT32_LIST
     */
    sai_s32_list_t s32list;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_UINT32_RANGE */
    sai_u32_range_t u32range;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_INT32_RANGE */
    sai_s32_range_t s32range;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_VLAN_LIST */
    sai_vlan_list_t vlanlist;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_QOS_MAP_LIST */
    sai_qos_map_list_t qosmap;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_MAP_LIST */
    sai_map_list_t maplist;

    /* TODO UDF also we need flag for UDF */

    /**
     * @passparam meta
     * @validonly meta->isaclfield == true
     */
    sai_acl_field_data_t aclfield;

    /**
     * @passparam meta
     * @validonly meta->isaclaction == true
     */
    sai_acl_action_data_t aclaction;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_CAPABILITY */
    sai_acl_capability_t aclcapability;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_RESOURCE_LIST */
    sai_acl_resource_list_t aclresource;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_TLV_LIST */
    sai_tlv_list_t tlvlist;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_SEGMENT_LIST */
    sai_segment_list_t segmentlist;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_IP_ADDRESS_LIST */
    sai_ip_address_list_t ipaddrlist;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_PORT_EYE_VALUES_LIST */
    sai_port_eye_values_list_t porteyevalues;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_TIMESPEC */
    sai_timespec_t timespec;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_MACSEC_SAK */
    sai_macsec_sak_t macsecsak;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_MACSEC_AUTH_KEY */
    sai_macsec_auth_key_t macsecauthkey;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_MACSEC_SALT */
    sai_macsec_salt_t macsecsalt;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_SYSTEM_PORT_CONFIG */
    sai_system_port_config_t sysportconfig;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_SYSTEM_PORT_CONFIG_LIST */
    sai_system_port_config_list_t sysportconfiglist;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_FABRIC_PORT_REACHABILITY */
    sai_fabric_port_reachability_t reachability;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_PORT_ERR_STATUS_LIST */
    sai_port_err_status_list_t porterror;
} sai_attribute_value_t;

/**
 * @extraparam const sai_attr_metadata_t *meta
 */
typedef struct _sai_attribute_t
{
    /** @passparam meta */
    sai_attr_id_t id;

    /** @passparam meta */
    sai_attribute_value_t value;
} sai_attribute_t;

typedef enum _sai_bulk_op_error_mode_t
{
    /**
     * @brief Bulk operation error handling mode where operation stops on the first failed creation
     *
     * Rest of objects will use SAI_STATUS_NON_EXECUTED return status value.
     */
    SAI_BULK_OP_ERROR_MODE_STOP_ON_ERROR,

    /**
     * @brief Bulk operation error handling mode where operation ignores the failures and continues to create other objects
     */
    SAI_BULK_OP_ERROR_MODE_IGNORE_ERROR,
} sai_bulk_op_error_mode_t;

/**
 * @brief Bulk objects creation.
 *
 * @param[in] switch_id SAI Switch object id
 * @param[in] object_count Number of objects to create
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to create.
 * @param[in] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 *
 * @param[out] object_id List of object ids returned
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are created or #SAI_STATUS_FAILURE when
 * any of the objects fails to create. When there is failure, Caller is expected to go through the
 * list of returned statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_object_create_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk objects removal.
 *
 * @param[in] object_count Number of objects to create
 * @param[in] object_id List of object ids
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or #SAI_STATUS_FAILURE when
 * any of the objects fails to remove. When there is failure, Caller is expected to go through the
 * list of returned statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_object_remove_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk objects set attributes.
 *
 * @param[in] object_count Number of objects to set on attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_list List of attributes for every object, one per object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when set attributes on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to set attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_object_set_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk objects get attributes.
 *
 * @param[in] object_count Number of objects to get on attribute
 * @param[in] object_id List of object ids
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get.
 * @param[inout] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 *
 * @return #SAI_STATUS_SUCCESS when get attributes on all objects succeeded or
 * #SAI_STATUS_FAILURE when any of the objects fails to get attribute. When
 * there is failure, Caller is expected to go through the list of returned
 * statuses to find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_object_get_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

typedef enum _sai_stats_mode_t
{
    /**
     * @brief Read statistics
     */
    SAI_STATS_MODE_READ,

    /**
     * @brief Read and clear after reading
     */
    SAI_STATS_MODE_READ_AND_CLEAR,
} sai_stats_mode_t;

/**
 * @}
 */
#endif /** __SAITYPES_H_ */
