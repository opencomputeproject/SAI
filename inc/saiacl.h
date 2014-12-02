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
*    saiacl.h
*
* Abstract:
*
*    This module defines SAI ACL API.
*
*/

#if !defined (__SAIACL_H_)
#define __SAIACL_H_

#include <saitypes.h>

typedef enum _sai_acl_stage_t 
{
    /* Any Stage */
    SAI_ACL_STAGE_ANY,

    /* Ingress Stage */
    SAI_ACL_STAGE_INGRESS,

    /* Egress Stage */
    SAI_ACL_STAGE_EGRESS,

} sai_acl_stage_t; 

typedef enum _sai_acl_ip_type_t 
{
    /* Don't care */
    SAI_ACL_IP_TYPE_ANY,

    /* IPv4 and IPv6 packets */
    SAI_ACL_IP_TYPE_IP,

    /* Non-Ip packet */
    SAI_ACL_IP_TYPE_NON_IP,

    /* Any IPv4 packet */
    SAI_ACL_IP_TYPE_IPv4ANY,

    /* Anything but IPv4 packets */
    SAI_ACL_IP_TYPE_NON_IPv4,

    /* IPv6 packet */
    SAI_ACL_IP_TYPE_IPv6ANY,

    /* Anything but IPv6 packets */
    SAI_ACL_IP_TYPE_NON_IPv6,

    /* ARP/RARP */
    SAI_ACL_IP_TYPE_ARP,

    /* ARP Request */
    SAI_ACL_IP_TYPE_ARP_REQUEST,

    /* ARP Reply */
    SAI_ACL_IP_TYPE_ARP_REPLY

} sai_acl_ip_type_t;

typedef enum _sai_acl_ip_frag_t 
{
    /* Any Fragment of Fragmented Packet */
    SAI_ACL_IP_FRAG_ANY,

    /* Non-Fragmented Packet */
    SAI_ACL_IP_FRAG_NOB_FRAG,

    /* Non-Fragmented or First Fragment */
    SAI_ACL_IP_FRAG_NON_FRAG_OR_HEAD,

    /* First Fragment of Fragmented Packet */
    SAI_ACL_IP_FRAG_HEAD,

    /* Not the First Fragment */
    SAI_ACL_IP_FRAG_NON_HEAD

} sai_acl_ip_frag_t;

typedef enum _sai_acl_field_t 
{
    /* Src IPv6 Address */
    SAI_ACL_FIELD_SRC_IPv6,

    /* Dst IPv6 Address */
    SAI_ACL_FIELD_DST_IPv6,

    /* Src MAC Address */
    SAI_ACL_FIELD_SRC_MAC,

    /* Dst MAC Address */
    SAI_ACL_FIELD_DST_MAC,

    /* Src IPv4 Address */
    SAI_ACL_FIELD_SRC_IP,

    /* Dst IPv4 Address */
    SAI_ACL_FIELD_DST_IP,

    /* In-Ports [bitmask of sai_port_id_t] */
    SAI_ACL_FIELD_IN_PORTS,

    /* Out-Ports [bitmask of sai_port_id_t] */
    SAI_ACL_FIELD_OUT_PORTS,

    /* IPv6 Flow */
    SAI_ACL_FIELD_IS_IPv6_FLOW,

    /* Outer Vlan-Id */
    SAI_ACL_FIELD_OUTER_VLAN_ID,

    /* Outer Vlan-Priority */
    SAI_ACL_FIELD_OUTER_VLAN_PRI,

    /* Outer Vlan-CFI */
    SAI_ACL_FIELD_OUTER_VLAN_CFI,

    /* Inner Vlan-Id */
    SAI_ACL_FIELD_INNER_VLAN_ID,

    /* Inner Vlan-Priority */
    SAI_ACL_FIELD_INNER_VLAN_PRI,

    /* Inner Vlan-CFI */
    SAI_ACL_FIELD_INNER_VLAN_CFI,

    /* L4 Src Port */
    SAI_ACL_FIELD_L4_SRC_PORT,

    /* L4 Dst Port */
    SAI_ACL_FIELD_L4_DST_PORT,

    /* EtherType */
    SAI_ACL_FIELD_ETHER_TYPE,

    /* IP Protocol */
    SAI_ACL_FIELD_IP_PROTOCOL,

    /* Ip Dscp */
    SAI_ACL_FIELD_DSCP,

    /* Ip Ttl */
    SAI_ACL_FIELD_TTL,

    /* Ip Tos */
    SAI_ACL_FIELD_TOS,

    /* Ip Flags */
    SAI_ACL_FIELD_IP_FLAGS,

    /* Tcp Flags */
    SAI_ACL_FIELD_TCP_FLAGS,

    /* Ip Type [sai_acl_ip_type_t] */
    SAI_ACL_FIELD_IP_TYPE,

    /* Ip Frag [sai_acl_ip_frag_t] */
    SAI_ACL_FIELD_IP_FRAG,

    /* Class-of-Service (Traffic Class) */
    SAI_ACL_FIELD_COS

} sai_acl_field_t;

typedef enum _sai_acl_match_mode_t 
{
    /* Field mode is determined by the field type */
    SAI_ACL_MATCH_AUTO,

    /* Field mode is AND with mask (<data> & <mask> == <field>) */
    SAI_ACL_MATCH_MASK

} sai_acl_match_mode_t;

typedef enum _sai_acl_action_type_t 
{
    /* Forward Normally */
    SAI_ACL_ACTION_FORWARD,

    /* Redirect Packet */
    SAI_ACL_ACTION_REDIRECT,

    /* Drop Packet */
    SAI_ACL_ACTION_DROP,

    /* Allow Packet (overwrites drop action) */
    SAI_ACL_ACTION_ALLOW,

    /* Flood Packet on Vlan domain */
    SAI_ACL_ACTION_FLOOD,

    /* Copy Packet to CPU */
    SAI_ACL_ACTION_COPY_TO_CPU,

    /* Trap Packet to CPU */
    SAI_ACL_ACTION_TRAP_TO_CPU,

    /* Ingress Mirror */
    SAI_ACL_ACTION_MIRROR_INGRESS,

    /* Egress Mirror */
    SAI_ACL_ACTION_MIRROR_EGRESS,

    /* Count */
    SAI_ACL_ACTION_COUNT,

    /* Assosiate with policer [sai_policer_id_t] */
    SAI_ACL_ACTION_SET_POLICER,

    /* Decrement TTL */
    SAI_ACL_ACTION_DECREMENT_TTL,

    /* Set Class-of-Service (Traffic Class) */
    SAI_ACL_ACTION_SET_COS,

    /* Set Packet Inner Vlan-Id */
    SAI_ACL_ACTION_SET_INNER_VLAN_ID,

    /* Set Packet Inner Vlan-Priority */
    SAI_ACL_ACTION_SET_INNER_VLAN_PRI,

    /* Set Packet Outer Vlan-Id */
    SAI_ACL_ACTION_SET_OUTER_VLAN_ID,

    /* Set Packet Outer Vlan-Priority */
    SAI_ACL_ACTION_SET_OUTER_VLAN_PRI,

    /* Set Packet Src MAC Address */
    SAI_ACL_ACTION_SET_SRC_MAC,

    /* Set Packet Dst MAC Address */
    SAI_ACL_ACTION_SET_DST_MAC,

    /* Set Packet Src IPv4 Address */
    SAI_ACL_ACTION_SET_SRC_IP,

    /* Set Packet Src IPv4 Address */
    SAI_ACL_ACTION_SET_DST_IP,

    /* Set Packet Src IPv6 Address */
    SAI_ACL_ACTION_SET_SRC_IPv6,

    /* Set Packet Src IPv6 Address */
    SAI_ACL_ACTION_SET_DST_IPv6,

    /* Set Packet DSCP */
    SAI_ACL_ACTION_SET_DSCP,

    /* Set Packet L4 Src Port */
    SAI_ACL_ACTION_SET_L4_SRC_PORT,

    /* Set Packet L4 Src Port */
    SAI_ACL_ACTION_SET_L4_DST_PORT

} sai_acl_action_type_t;

/*
*  Attribute Id in sai_set_acl_attribute() and 
*  sai_get_acl_attribute() calls
*/
typedef enum _sai_acl_attr_t 
{
    /* READ-ONLY */

    /* Priority [sai_acl_stage_t] */
    SAI_ACL_ATTR_STAGE,

    /* Priority [int] */
    SAI_ACL_ATTR_PRIORITY,

    /* READ-WRITE */

    /* Enabled / Disabled [bool] */
    SAI_ACL_ATTR_ADMIN_STATE,

    /* -- */

    /* Custom range base value */
    SAI_ACL_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_acl_attr_t;

/*
 * Defines a single ACL filter
 */
typedef struct _sai_acl_filter_t
{
    /*
     * Field specifier
     */
    sai_acl_field_t field;

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

} sai_acl_filter_t;

/*
 * Defines a single ACL action
 */
typedef struct _sai_acl_action_t
{
    /*
     * Action
     */
    sai_acl_action_type_t action;

    /*
     * Action parameter
     */
    uint64_t parameter[2];

} sai_acl_action_t;


/*
* Routine Description:
*   Create an ACL.
*
* Arguments:
 *  [in,out] acl_id - the the acl-id.
 *  [in] stage - the acl stage.
 *  [in] priority - the acl priority.
 *  [in] filter_count -  number of filter conditions.
 *  [in] filter_list - list of filters.
 *  [in] action_count - number of actions.
 *  [in] action_list - list of actions.
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_acl_fn)(
    _Inout_ sai_acl_id_t* acl_id,
    _In_ sai_acl_stage_t stage,
    _In_ uint32_t priority,
    _In_ uint32_t filter_count,
    _In_ sai_acl_filter_t* filter_list,
    _In_ uint32_t action_count,
    _In_ sai_acl_action_t* action_list
    );

/*
* Routine Description:
*   Modify an ACL.
*
* Arguments:
 *  [in] acl_id - the the acl-id.
 *  [in] action_count - number of actions.
 *  [in] action_list - list of actions.
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_modify_acl_fn)(
    _In_ sai_acl_id_t acl_id,
    _In_ uint32_t action_count,
    _In_ sai_acl_action_t* action_list
    );

/*
* Routine Description:
*   Delete an ACL.
*
* Arguments:
 *  [in] acl_id - the the acl-id.
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_delete_acl_fn)(
    _In_ sai_acl_id_t acl_id
    );

/*
* Routine Description:
*   Set ACL attribute 
*
* Arguments:
*    [in] acl_id - acl id.
*    [in] attribute - acl attribute.
*    [in] value - acl attribute value.
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_acl_attribute_fn)(
    _In_ sai_acl_id_t acl_id,
    _In_ sai_acl_attr_t attribute,
    _In_ uint64_t value
    );

/*
* Routine Description:
*   Get ACL attribute 
*
* Arguments:
*    [in] acl_id - acl id.
*    [in] attribute - acl attribute.
*    [out] value - acl attribute value.
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_acl_attribute_fn)(
    _In_ sai_acl_id_t acl_id,
    _In_ sai_acl_attr_t attribute,
    _Out_ uint64_t* value
    );

/*
* Port methods table retrieved with sai_api_query()
*/
typedef struct _sai_acl_api_t
{
    sai_create_acl_fn           create_acl;
    sai_delete_acl_fn           delete_acl;
    sai_modify_acl_fn           modify_acl;
    sai_set_acl_attribute_fn    set_attribute;
    sai_get_acl_attribute_fn    get_attribute;

} sai_acl_api_t;


#endif // __SAIACL_H_

