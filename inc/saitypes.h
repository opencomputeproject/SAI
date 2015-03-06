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
typedef UINT32  sai_switch_id_t;
typedef UINT32  sai_port_id_t;
typedef UINT16  sai_vlan_id_t;
typedef UINT32  sai_virtual_router_id_t;
typedef UINT32  sai_router_interface_id_t;
typedef UINT32  sai_host_interface_id_t;
typedef UINT32  sai_next_hop_id_t;
typedef UINT32  sai_next_hop_group_id_t;
typedef UINT32  sai_acl_table_id_t;
typedef UINT32  sai_acl_entry_id_t;
typedef UINT32  sai_acl_counter_id_t;
typedef UINT32  sai_attr_id_t;
typedef UINT8   sai_cos_t;
typedef UINT8   sai_mac_t[6];
typedef UINT32  sai_ip4_t;
typedef UINT8   sai_ip6_t[16];

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

typedef int32_t  sai_status_t;  
typedef uint32_t sai_switch_profile_id_t;
typedef uint32_t sai_switch_id_t;
typedef uint32_t sai_port_id_t;
typedef uint16_t sai_vlan_id_t;
typedef uint32_t sai_virtual_router_id_t;
typedef uint32_t sai_router_interface_id_t;
typedef uint32_t sai_host_interface_id_t;
typedef uint32_t sai_next_hop_id_t;
typedef uint32_t sai_next_hop_group_id_t;
typedef uint32_t sai_acl_table_id_t;
typedef uint32_t sai_acl_entry_id_t;
typedef uint32_t sai_acl_counter_id_t;
typedef uint32_t sai_attr_id_t;
typedef uint8_t  sai_cos_t;
typedef uint8_t  sai_mac_t[6];
typedef uint32_t sai_ip4_t;
typedef uint8_t  sai_ip6_t[16];

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

/* 
 * Defines a list of sai port ids used as sai attribute value.
 * 
 * - In set attribute function call, the port_count defines the number of
 * ports. 
 *
 * - In get attribute function call, the function call returns a list of ports
 * to the caller in port_list. The caller is responsible for allocating the
 * buffer for port_list and set the port_count to the size of allocated port
 * list. If the size is large enough to accomodate the list of port id, the
 * callee will then fill the port_list and set the port_count to the actual
 * number of ports.  If the list size is not large enough, the callee will set the
 * port_count to the actual number of port id and return
 * SAI_STATUS_BUFFER_OVERFLOW. Once the caller gets such return code, it should
 * use the returned port count to re-allocate list and retry.
 *
 * - The above behavior also applies to sai_next_hop_list_t.
 */
typedef struct _sai_port_list_t {
    uint32_t port_count;
    sai_port_id_t *port_list;
} sai_port_list_t;

typedef struct _sai_next_hop_list_t {
    uint32_t next_hop_count;
    sai_next_hop_id_t *next_hop_list;
} sai_next_hop_list_t;

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

typedef enum _sai_acl_match_mode_t 
{
    /* Field match is disbled. 
     * Used for disable a match field in an installed acl entry */
    SAI_ACL_MATCH_DISABLE, 

    /* Field mode is determined by the field type */
    SAI_ACL_MATCH_AUTO,

    /* Field mode is AND with mask (<data> & <mask> == <field>) */
    SAI_ACL_MATCH_MASK

} sai_acl_match_mode_t;

/*
 * Defines a single ACL filter
 */
typedef struct _sai_acl_field_data_t
{
    /*
     * Field match mode
     */
    sai_acl_match_mode_t mode;

    /*
     * Field match mask
     */
    uint64_t match_mask[2];

    /*
     * Expected AND result using match mask above with packet field value.
     */
    uint64_t match_data[2];

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
    uint64_t parameter[2];

} sai_acl_action_data_t;

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
    sai_port_list_t portlist;
    sai_next_hop_list_t nhlist;
    sai_acl_field_data_t aclfield;
    sai_acl_action_data_t acldata;
} sai_attribute_value_t;

typedef struct _sai_attribute_t {
    sai_attr_id_t id;
    sai_attribute_value_t value;
} sai_attribute_t;

#endif // __SAITYPES_H_
