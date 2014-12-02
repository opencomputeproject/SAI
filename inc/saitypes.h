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
typedef UINT64 uint64_t;

typedef  INT32  sai_status_t;  
typedef UINT32  sai_switch_profile_id_t;
typedef UINT32  sai_switch_id_t;
typedef UINT32  sai_port_id_t;
typedef UINT16  sai_vlan_id_t;
typedef UINT32  sai_vr_id_t;
typedef UINT32  sai_router_interface_id_t;
typedef UINT32  sai_route_id_t;
typedef UINT32  sai_next_hop_group_id_t;
typedef UINT64  sai_mac_t;
typedef UINT8   sai_cos_t;
typedef UINT32  sai_acl_id_t;

#include <ws2def.h>
#include <ws2ipdef.h>

#if !defined(__BOOL_DEFINED)

typedef enum {
  false,
  true
} _bool;

#define bool _bool

#endif  // __BOOL_DEFINED

#if !defined(_NETIOAPI_H_)

typedef struct _IP_ADDRESS_PREFIX {
  SOCKADDR_INET Prefix;
  UINT8         PrefixLength;
} IP_ADDRESS_PREFIX, *PIP_ADDRESS_PREFIX;


#endif // _NETIOAPI_H_

//
// N.B. Equal to 260 on Windows
//
#define PATH_MAX MAX_PATH



#else  // #if defined(_WIN32)


typedef sint32_t sai_status_t;  
typedef uint32_t sai_switch_profile_id_t;
typedef uint32_t sai_switch_id_t;
typedef uint32_t sai_port_id_t;
typedef uint16_t sai_vlan_id_t;
typedef uint32_t sai_vr_id_t;
typedef uint32_t sai_router_interface_id_t;
typedef uint32_t sai_route_id_t;
typedef uint32_t sai_next_hop_group_id_t;
typedef uint64_t sai_mac_t;
typedef uint8_t  sai_cos_t;
typedef uint32_t sai_acl_id_t;

#define _In_
#define _Out_
#define _In_reads_z_(_LEN_)
#define _In_reads_opt_z_(_LEN_)

#include <sys/socket.h>
#include <netinet/in.h> 

typedef union _SOCKADDR_INET {
    struct sockaddr_in  Ipv4;
    struct sockaddr_in6 Ipv6;
    sa_family_t         si_family;
} sockaddr_inet;

typedef struct _IP_ADDRESS_PREFIX {
  sockaddr_inet Prefix;
  uint8_t       PrefixLength;
} ip_address_prefix;


#endif // _WIN32

//
// New common definitions
//

typedef union _SOCKADDR_INET sockaddr_inet;
typedef struct _IP_ADDRESS_PREFIX ip_address_prefix;

#endif // __SAITYPES_H_
