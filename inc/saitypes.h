/*
* Copyright (c) 2014 Microsoft Open Technologies, Inc. 
*   
*    Licensed under the Apache License, Version 2.0 (the "License"); you may 
*    not use this file except in compliance with the License. You may obtain 
*    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
*    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR 
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
* Module Name:
*
*    saitypes.h
*
* Abstract:
*
*    This module contains SAI portable types.
*/

#if !defined (__SAITYPES_H_)
#define __SAITYPES_H_

#if defined(_WIN32)

//
// *nix already has lower-case definitions for types.
//
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

#endif  // __BOOL_DEFINED

//
// N.B. Equal to 260 on Windows
//
#define PATH_MAX MAX_PATH



#else  // #if defined(_WIN32)

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

typedef int32_t  sai_status_t;  
typedef uint32_t sai_switch_profile_id_t;
typedef uint16_t sai_vlan_id_t;
typedef uint32_t sai_attr_id_t;
typedef uint8_t  sai_cos_t;
typedef uint8_t  sai_mac_t[6];
typedef uint32_t sai_ip4_t;
typedef uint8_t  sai_ip6_t[16];
typedef uint32_t sai_switch_hash_seed_t;

#define _In_
#define _Out_
#define _Inout_
#define _In_reads_z_(_LEN_)
#define _In_reads_opt_z_(_LEN_)


#endif // _WIN32

//
// New common definitions
//
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

#define SAI_NULL_OBJECT_ID 0L

/* 
 * Defines a list of sai object ids used as sai attribute value.
 * 
 * - In set attribute function call, the count member defines the number of
 * objects.
 *
 * - In get attribute function call, the function call returns a list of objects
 * to the caller in the list member. The caller is responsible for allocating the
 * buffer for the list member and set the count member to the size of allocated object
 * list. If the size is large enough to accomodate the list of object id, the
 * callee will then fill the list member and set the count member to the actual
 * number of objects.  If the list size is not large enough, the callee will set the
 * count member to the actual number of object id and return
 * SAI_STATUS_BUFFER_OVERFLOW. Once the caller gets such return code, it should
 * use the returned count member to re-allocate list and retry.
 */
typedef struct _sai_object_list_t {
    uint32_t count;
    sai_object_id_t *list;
} sai_object_list_t;

/* 
 * sai object type
 */
typedef enum _sai_object_type_t {
    SAI_OBJECT_TYPE_NULL             =  0,
    SAI_OBJECT_TYPE_PORT             =  1, 
    SAI_OBJECT_TYPE_LAG              =  2, 
    SAI_OBJECT_TYPE_VIRTUAL_ROUTER   =  3,
    SAI_OBJECT_TYPE_NEXT_HOP         =  4,
    SAI_OBJECT_TYPE_NEXT_HOP_GROUP   =  5,
    SAI_OBJECT_TYPE_ROUTER_INTERFACE =  6,
    SAI_OBJECT_TYPE_ACL_TABLE        =  7,
    SAI_OBJECT_TYPE_ACL_ENTRY        =  8,
    SAI_OBJECT_TYPE_ACL_COUNTER      =  9,
    SAI_OBJECT_TYPE_HOST_INTERFACE   = 10,
    SAI_OBJECT_TYPE_MIRROR           = 11,
    SAI_OBJECT_TYPE_SAMPLEPACKET     = 12,
    SAI_OBJECT_TYPE_STP_INSTANCE     = 13,
    SAI_OBJECT_TYPE_TRAP_GROUP       = 14,
    SAI_OBJECT_TYPE_MAX              = 15
} sai_object_type_t;

typedef struct _sai_u32_list_t {
    uint32_t count;
    uint32_t *list;
} sai_u32_list_t;

typedef struct _sai_s32_list_t {
    uint32_t count;
    int32_t  *list;
} sai_s32_list_t;

/*
 * Defines a vlan list datastructure
 */
typedef struct _sai_vlan_list_t {

    /* Number of Vlans*/
    uint32_t vlan_count;

    /* List of Vlans*/
    sai_vlan_id_t *vlan_list;

} sai_vlan_list_t;

typedef enum _sai_ip_addr_family_t {
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

/*
 * Defines a single ACL filter
 */
typedef struct _sai_acl_field_data_t
{
    /*
     * match enable/disable
     */ 
    bool enable;

    /*
     * Field match mask
     */
    union {
        sai_uint8_t u8;
        sai_uint16_t u16;
        sai_uint32_t u32;
        sai_mac_t mac;
        sai_ip4_t ip4;
        sai_ip6_t ip6;
    } mask;

    /*
     * Expected AND result using match mask above with packet field value where applicable.
     */
    union {
        sai_uint8_t u8;
        sai_uint16_t u16;
        sai_uint32_t u32;
        sai_mac_t mac;
        sai_ip4_t ip4;
        sai_ip6_t ip6;
        sai_object_id_t oid;
        sai_object_list_t objlist;
    } data;
} sai_acl_field_data_t;

/*
 * Defines a single ACL action
 */
typedef struct _sai_acl_action_data_t
{
    /*
     * action enable/disable
     */ 
    bool enable;
    /*
     * Action parameter
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
    } parameter;
} sai_acl_action_data_t;

/*
 * Breakout Mode types based on number
 * of SerDes lanes used in a port
 */
typedef enum _sai_port_breakout_mode_type_t
{
    /* 1 lane breakout Mode */
    SAI_PORT_BREAKOUT_MODE_1_LANE = 1,

    /* 2 lanes breakout Mode */
    SAI_PORT_BREAKOUT_MODE_2_LANE = 2,

    /* 4 lanes breakout Mode */
    SAI_PORT_BREAKOUT_MODE_4_LANE = 4,

    /* Breakout mode max count */
    SAI_PORT_BREAKOUT_MODE_MAX
} sai_port_breakout_mode_type_t;

/*
 * Defines breakout mode on a switch port(s)
 */
typedef struct _sai_port_breakout_t
{
    /*
     * Breakout mode type
     */
    sai_port_breakout_mode_type_t breakout_mode;

    /*
     * List of ports to be breakout
     * Break out - typically 1 port; Break in - set of ports
     */
    sai_object_list_t  port_list;
} sai_port_breakout_t;

/* 
 * Data Type to use enum's as attribute value is sai_int32_t s32
 *
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
    sai_mac_t mac;
    sai_ip4_t ip4;
    sai_ip6_t ip6;
    sai_ip_address_t ipaddr;
    sai_object_id_t oid;
    sai_object_list_t objlist;
    sai_u32_list_t u32list;
    sai_s32_list_t s32list;
    sai_vlan_list_t vlanlist;
    sai_acl_field_data_t aclfield;
    sai_acl_action_data_t aclaction;
    sai_port_breakout_t portbreakout;
} sai_attribute_value_t;

typedef struct _sai_attribute_t {
    sai_attr_id_t id;
    sai_attribute_value_t value;
} sai_attribute_t;

#endif // __SAITYPES_H_
