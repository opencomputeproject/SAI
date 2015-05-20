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
    SAI_ACL_IP_FRAG_NON_FRAG,

    /* Non-Fragmented or First Fragment */
    SAI_ACL_IP_FRAG_NON_FRAG_OR_HEAD,

    /* First Fragment of Fragmented Packet */
    SAI_ACL_IP_FRAG_HEAD,

    /* Not the First Fragment */
    SAI_ACL_IP_FRAG_NON_HEAD

} sai_acl_ip_frag_t;

/*
*  Attribute Id for sai_acl_table
*/
typedef enum _sai_acl_table_attr_t
{
    /* READ-ONLY */

    /* READ-WRITE */

    /* ACL stage [sai_acl_stage_t]
     * (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_ACL_TABLE_ATTR_STAGE,

    /* Priority [sai_uint32_t]
     * (MANDATORY_ON_CREATE|CREATE_ONLY)
     * Value must be in the range defined in
     * [SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY,
     *  SAI_SWITCH_ATTR_ACL_TABLE_MAXIMUM_PRIORITY] */
    SAI_ACL_TABLE_ATTR_PRIORITY,

    /* Table size [sai_uint32_t], CREATE_ONLY
     * (default = 0) - Grow dynamically till MAX ACL TCAM Size
     * By default table can grow upto maximum ACL TCAM space.
     * Supported only during Table Create for now until NPU
     * supports Dynamic adjustment of Table size post Table creation
     *
     * The table size refers to the number of ACL entries. The number
     * of entries that get's allocated when we create a table with a
     * specific size would depend on the ACL CAM Arch of the NPU. Some
     * NPU supports different blocks, each may have same or different
     * size and what gets allocated can depend on the block size or other
     * factors. So internally what gets allocated when we do a table
     * create would be based on the NPU CAM Arch and size may be more
     * than what is requested. As an example the NPU may support blocks of
     * 128 entries. When a user creates a table of size 100, the actual
     * size that gets allocated is 128. Hence its recommended that the user
     * does a get_attribute(SAI_ACL_TABLE_ATTR_SIZE) to query the actual
     * table size on table create so the user knows the ACL CAM space
     * allocated and able to do ACL CAM Carving accurately */
    SAI_ACL_TABLE_ATTR_SIZE,

    /* Match fields [bool]
     * (MANDATORY_ON_CREATE, mandatory to pass at least one field during ACL Table Creation)
     * (CREATE_ONLY, match fields cannot be changed after the table is created) */

    /* Start of Table Match Field */
    SAI_ACL_TABLE_ATTR_FIELD_START = 0x00001000,

    /* Src IPv6 Address */
    SAI_ACL_TABLE_ATTR_FIELD_SRC_IPv6 = SAI_ACL_TABLE_ATTR_FIELD_START,

    /* Dst IPv6 Address */
    SAI_ACL_TABLE_ATTR_FIELD_DST_IPv6,

    /* Src MAC Address */
    SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC,

    /* Dst MAC Address */
    SAI_ACL_TABLE_ATTR_FIELD_DST_MAC,

    /* Src IPv4 Address */
    SAI_ACL_TABLE_ATTR_FIELD_SRC_IP,

    /* Dst IPv4 Address */
    SAI_ACL_TABLE_ATTR_FIELD_DST_IP,

    /* In-Ports */
    SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS,

    /* Out-Ports */
    SAI_ACL_TABLE_ATTR_FIELD_OUT_PORTS,

    /* In-Port */
    SAI_ACL_TABLE_ATTR_FIELD_IN_PORT,

    /* Out-Port */
    SAI_ACL_TABLE_ATTR_FIELD_OUT_PORT,

    /* Outer Vlan-Id */
    SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_ID,

    /* Outer Vlan-Priority */
    SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_PRI,

    /* Outer Vlan-CFI */
    SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_CFI,

    /* Inner Vlan-Id */
    SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_ID,

    /* Inner Vlan-Priority */
    SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_PRI,

    /* Inner Vlan-CFI */
    SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_CFI,

    /* L4 Src Port */
    SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT,

    /* L4 Dst Port */
    SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT,

    /* EtherType */
    SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE,

    /* IP Protocol */
    SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL,

    /* Ip Dscp */
    SAI_ACL_TABLE_ATTR_FIELD_DSCP,

    /* Ip Ecn */
    SAI_ACL_TABLE_ATTR_FIELD_ECN,

    /* Ip Ttl */
    SAI_ACL_TABLE_ATTR_FIELD_TTL,

    /* Ip Tos */
    SAI_ACL_TABLE_ATTR_FIELD_TOS,

    /* Ip Flags */
    SAI_ACL_TABLE_ATTR_FIELD_IP_FLAGS,

    /* Tcp Flags */
    SAI_ACL_TABLE_ATTR_FIELD_TCP_FLAGS,

    /* Ip Type */
    SAI_ACL_TABLE_ATTR_FIELD_IP_TYPE,

    /* Ip Frag */
    SAI_ACL_TABLE_ATTR_FIELD_IP_FRAG,

    /* IPv6 Flow Label */
    SAI_ACL_TABLE_ATTR_FIELD_IPv6_FLOW_LABEL,

    /* Class-of-Service (Traffic Class) */
    SAI_ACL_TABLE_ATTR_FIELD_TC,

    /* End of Table Match Field */
    SAI_ACL_TABLE_ATTR_FIELD_END = SAI_ACL_TABLE_ATTR_FIELD_TC,

    /* -- */

    /* Custom range base value */
    SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_acl_table_attr_t;

/*
*  Attribute Id for sai_acl_entry
*/
typedef enum _sai_acl_entry_attr_t
{
    /* READ-ONLY */

    /* SAI acl table object id [sai_object_id_t]
     * (MANDATORY_ON_CREATE) */
    SAI_ACL_ENTRY_ATTR_TABLE_ID,

    /* READ-WRITE */

    /* Priority [sai_uint32_t]
     * Value must be in the range defined in
     * [SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY,
     *  SAI_SWITCH_ATTR_ACL_ENTRY_MAXIMUM_PRIORITY]
     * (default = SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY) */
    SAI_ACL_ENTRY_ATTR_PRIORITY,

    /* Enabled / Disabled [bool]
     * (default = enabled) */
    SAI_ACL_ENTRY_ATTR_ADMIN_STATE,

    /* Match fields [sai_acl_field_data_t]
     * (MANDATORY_ON_CREATE, mandatory to pass at least one field during ACL Rule Creation)
     * - Unless noted specificially, both data and mask are required.
     * - When bitfield is used the comment, only those least significent bits
     *   are valid for matching.
     */

    /* Start of Rule Match Fields */
    SAI_ACL_ENTRY_ATTR_FIELD_START = 0x00001000,

    /* Src IPv6 Address [sai_ip6_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_SRC_IPv6 = SAI_ACL_ENTRY_ATTR_FIELD_START,

    /* Dst IPv6 Address [sai_ip6_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_DST_IPv6,

    /* Src MAC Address [sai_mac_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC,

    /* Dst MAC Address [sai_mac_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_DST_MAC,

    /* Src IPv4 Address [sai_ip4_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP,

    /* Dst IPv4 Address [sai_ip4_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_DST_IP,

    /* In-Ports [sai_object_list_t] (mask is not needed) */
    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS,

    /* Out-Ports [sai_object_list_t] (mask is not needed) */
    SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORTS,

    /* In-Port [sai_object_id_t] (mask is not needed) */
    SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT,

    /* Out-Port [sai_object_id_t] (mask is not needed) */
    SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT,

    /* Outer Vlan-Id [sai_uint16_t : 12] */
    SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_ID,

    /* Outer Vlan-Priority [sai_uint8_t : 3] */
    SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_PRI,

    /* Outer Vlan-CFI [sai_uint8_t : 1] */
    SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_CFI,

    /* Inner Vlan-Id [sai_uint16_t: 12] */
    SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_ID,

    /* Inner Vlan-Priority [sai_uint8_t : 3] */
    SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_PRI,

    /* Inner Vlan-CFI [sai_uint8_t : 1] */
    SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_CFI,

    /* L4 Src Port [sai_uint16_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT,

    /* L4 Dst Port [sai_uint16_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT,

    /* EtherType [sai_uint16_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE,

    /* IP Protocol [sai_uint8_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL,

    /* Ip Dscp [sai_uint8_t : 6] */
    SAI_ACL_ENTRY_ATTR_FIELD_DSCP,

    /* Ip Ecn [sai_uint8_t : 2] */
    SAI_ACL_ENTRY_ATTR_FIELD_ECN,

    /* Ip Ttl [sai_uint8_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_TTL,

    /* Ip Tos [sai_uint8_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_TOS,

    /* Ip Flags [sai_uint8_t : 3] */
    SAI_ACL_ENTRY_ATTR_FIELD_IP_FLAGS,

    /* Tcp Flags [sai_uint8_t : 6] */
    SAI_ACL_ENTRY_ATTR_FIELD_TCP_FLAGS,

    /* Ip Type [sai_acl_ip_type_t] (field mask is not needed) */
    SAI_ACL_ENTRY_ATTR_FIELD_IP_TYPE,

    /* Ip Frag [sai_acl_ip_frag_t] (field mask is not needed) */
    SAI_ACL_ENTRY_ATTR_FIELD_IP_FRAG,

    /* IPv6 Flow Label [sai_uint32_t : 20] */
    SAI_ACL_ENTRY_ATTR_FIELD_IPv6_FLOW_LABEL,

    /* Class-of-Service (Traffic Class) [sai_cos_t] */
    SAI_ACL_ENTRY_ATTR_FIELD_TC,

    /* End of Rule Match Fields */
    SAI_ACL_ENTRY_ATTR_FIELD_END = SAI_ACL_ENTRY_ATTR_FIELD_TC,

    /* Actions [sai_acl_action_data_t]
     * - To enable an action, parameter is needed unless noted specifically.
     * - To disable an action, parameter is not needed. */

    /* Start of Rule Actions */
    SAI_ACL_ENTRY_ATTR_ACTION_START = 0x00002000,

    /* Redirect Packet to a destination which can be a port,
     * lag, nexthop, nexthopgroup. [sai_object_id_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT = SAI_ACL_ENTRY_ATTR_ACTION_START,

    /* Drop Packet [sai_packet_action_t] */
    SAI_ACL_ENTRY_ATTR_PACKET_ACTION,

    /* Flood Packet on Vlan domain (parameter is not needed) */
    SAI_ACL_ENTRY_ATTR_ACTION_FLOOD,

    /* Attach/detach counter id to the entry [sai_object_id_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_COUNTER,

    /* Ingress Mirror (mirror session id) [sai_object_id_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS,

    /* Egress Mirror (mirror session id) [sai_object_id_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS,

    /* Assosiate with policer (policer id) [sai_object_id_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_POLICER,

    /* Decrement TTL (enable/disable) (parameter is not needed) */
    SAI_ACL_ENTRY_ATTR_ACTION_DECREMENT_TTL,

    /* Set Class-of-Service (Traffic Class) [sai_cos_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_TC,

    /* Set Packet Inner Vlan-Id [sai_uint16_t : 12] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_INNER_VLAN_ID,

    /* Set Packet Inner Vlan-Priority [sai_uint8_t : 3] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_INNER_VLAN_PRI,

    /* Set Packet Outer Vlan-Id [sai_uint16_t : 12] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_OUTER_VLAN_ID,

    /* Set Packet Outer Vlan-Priority [sai_uint8_t : 3] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_OUTER_VLAN_PRI,

    /* Set Packet Src MAC Address [sai_mac_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_MAC,

    /* Set Packet Dst MAC Address [sai_mac_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_MAC,

    /* Set Packet Src IPv4 Address [sai_ip4_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_IP,

    /* Set Packet Src IPv4 Address [sai_ip4_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_IP,

    /* Set Packet Src IPv6 Address [sai_ip6_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_IPv6,

    /* Set Packet Src IPv6 Address [sai_ip6_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_IPv6,

    /* Set Packet DSCP [sai_uint8_t : 6] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_DSCP,

    /* Set Packet L4 Src Port [sai_uint16_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_L4_SRC_PORT,

    /* Set Packet L4 Src Port [sai_uint16_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_L4_DST_PORT,

    /* Set ingress packet sampling (samplepacket session id) [sai_object_id_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_INGRESS_SAMPLEPACKET_ENABLE,

    /* Set egress packet sampling (samplepacket session id) [sai_object_id_t] */
    SAI_ACL_ENTRY_ATTR_ACTION_EGRESS_SAMPLEPACKET_ENABLE,

    /* End of Rule Actions */
    SAI_ACL_ENTRY_ATTR_ACTION_END = SAI_ACL_ENTRY_ATTR_ACTION_EGRESS_SAMPLEPACKET_ENABLE

} sai_acl_entry_attr_t;

/*
*  Attribute Id for sai_acl_counter
*/
typedef enum _sai_acl_counter_attr_t
{
    /* READ-ONLY */

    /* SAI acl table object id [sai_object_id_t]
     * (MANDATORY_ON_CREATE) */
    SAI_ACL_COUNTER_ATTR_TABLE_ID,

    /*
     * By default Byte Counter would be created and following
     * use of the below attributes would result in an error.
     *
     * - Both packet count and byte count set to disable
     * - Only Byte count used which is set to disable
     */

    /* enable/disable packet count [bool] */
    SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT,

    /* enable/disable byte count [bool] */
    SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT,

    /* get/set packet count [uint64_t] */
    SAI_ACL_COUNTER_ATTR_PACKETS,

    /* get/set byte count [uint64_t] */
    SAI_ACL_COUNTER_ATTR_BYTES

} sai_acl_counter_attr_t;

/*
* Routine Description:
*   Create an ACL table
*
* Arguments:
 *  [out] acl_table_id - the the acl table id
 *  [in] attr_count - number of attributes
 *  [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_acl_table_fn)(
    _Out_ sai_object_id_t* acl_table_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );
/*
* Routine Description:
*   Delete an ACL table
*
* Arguments:
*   [in] acl_table_id - the acl table id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_delete_acl_table_fn)(
    _In_ sai_object_id_t acl_table_id
    );

/*
* Routine Description:
*   Set ACL table attribute
*
* Arguments:
*    [in] acl_table_id - the acl table id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_acl_table_attribute_fn)(
    _In_ sai_object_id_t acl_table_id,
    _In_ const sai_attribute_t *attr
    );

/*
* Routine Description:
*   Get ACL table attribute
*
* Arguments:
*    [in] acl_table_id - acl table id
*    [in] attr_count - number of attributes
*    [Out] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_acl_table_attribute_fn)(
    _In_ sai_object_id_t acl_table_id,
    _In_ uint32_t attr_count,
    _Out_ sai_attribute_t *attr_list
    );

/*
* Routine Description:
*   Create an ACL entry
*
* Arguments:
*   [out] acl_entry_id - the acl entry id
*   [in] attr_count - number of attributes
*   [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_acl_entry_fn)(
    _Out_ sai_object_id_t *acl_entry_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/*
* Routine Description:
*   Delete an ACL entry
*
* Arguments:
 *  [in] acl_entry_id - the acl entry id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_delete_acl_entry_fn)(
    _In_ sai_object_id_t acl_entry_id
    );

/*
* Routine Description:
*   Set ACL entry attribute
*
* Arguments:
*    [in] acl_entry_id - the acl entry id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_acl_entry_attribute_fn)(
    _In_ sai_object_id_t acl_entry_id,
    _In_ const sai_attribute_t *attr
    );

/*
* Routine Description:
*   Get ACL entry attribute
*
* Arguments:
*    [in] acl_entry_id - acl entry id
*    [in] attr_count - number of attributes
*    [Out] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_acl_entry_attribute_fn)(
    _In_ sai_object_id_t acl_entry_id,
    _In_ uint32_t attr_count,
    _Out_ sai_attribute_t *attr_list
    );

/*
* Routine Description:
*   Create an ACL counter
*
* Arguments:
*   [out] acl_counter_id - the acl counter id
*   [in] attr_count - number of attributes
*   [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_acl_counter_fn)(
    _Out_ sai_object_id_t *acl_counter_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/*
* Routine Description:
*   Delete an ACL counter
*
* Arguments:
 *  [in] acl_counter_id - the acl counter id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_delete_acl_counter_fn)(
    _In_ sai_object_id_t acl_counter_id
    );

/*
* Routine Description:
*   Set ACL counter attribute
*
* Arguments:
*    [in] acl_counter_id - the acl counter id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_acl_counter_attribute_fn)(
    _In_ sai_object_id_t acl_counter_id,
    _In_ const sai_attribute_t *attr
    );

/*
* Routine Description:
*   Get ACL counter attribute
*
* Arguments:
*    [in] acl_counter_id - acl counter id
*    [in] attr_count - number of attributes
*    [Out] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_acl_counter_attribute_fn)(
    _In_ sai_object_id_t acl_counter_id,
    _In_ uint32_t attr_count,
    _Out_ sai_attribute_t *attr_list
    );

/*
* Port methods table retrieved with sai_api_query()
*/
typedef struct _sai_acl_api_t
{
    sai_create_acl_table_fn             create_acl_table;
    sai_delete_acl_table_fn             delete_acl_table;
    sai_set_acl_table_attribute_fn      set_acl_table_attribute;
    sai_get_acl_table_attribute_fn      get_acl_table_attribute;
    sai_create_acl_entry_fn             create_acl_entry;
    sai_delete_acl_entry_fn             delete_acl_entry;
    sai_set_acl_entry_attribute_fn      set_acl_entry_attribute;
    sai_get_acl_entry_attribute_fn      get_acl_entry_attribute;
    sai_create_acl_counter_fn           create_acl_counter;
    sai_delete_acl_counter_fn           delete_acl_counter;
    sai_set_acl_counter_attribute_fn    set_acl_counter_attribute;
    sai_get_acl_counter_attribute_fn    get_acl_counter_attribute;

} sai_acl_api_t;

#endif // __SAIACL_H_

