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
*    saihostintf.h
*
* Abstract:
*
*    This module defines SAI Host Interface which is responsbile for
*    creating/deleting linux netdev corresponding to the host interface type.
*    All the management operations of the netdevs such as changing IP address
*    are outside the scope of SAI.
*
*/

#if !defined (__SAIHOSTINTF_H_)
#define __SAIHOSTINTF_H_

#include <saitypes.h>

#define HOSTIF_NAME_SIZE    16

typedef enum _sai_hostif_trap_group_attr_t
{
    /* Admin Mode [bool] (default to TRUE) */
    SAI_HOSTIF_TRAP_GROUP_ATTR_ADMIN_STATE,

    /* group priority [uint32_t] (MANDATORY_ON_CREATE|CREATE_ONLY).
    * This is equivalent to ACL table priority SAI_ACL_TABLE_ATTR_PRIORITY */
    SAI_HOSTIF_TRAP_GROUP_ATTR_PRIO,

    /* cpu egress queue [uint32_t] (CREATE_AND_SET)
     * (default to 0) */
    SAI_HOSTIF_TRAP_GROUP_ATTR_QUEUE,

    /* sai policer object id [sai_object_id_t] (CREATE_AND_SET) 
     * (default to SAI_NULL_OBJECT_ID) */
    SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER,

    SAI_HOSTIF_TRAP_GROUP_ATTR_CUSTOM_RANGE_BASE = 0x10000000
} sai_hostif_trap_group_attr_t;

/*
* Routine Description:
*    Create host interface trap group
*
* Arguments:
*  [out] hostif_trap_group_id  - host interface trap group id
*  [in] attr_count - number of attributes
*  [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_hostif_trap_group_fn)(
    _Out_ sai_object_id_t *hostif_trap_group_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/*
* Routine Description:
*    Remove host interface trap group
*
* Arguments:
*  [in] hostif_trap_group_id - host interface trap group id
*
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_remove_hostif_trap_group_fn)(
    _In_ sai_object_id_t hostif_trap_group_id
    );

/*
* Routine Description:
*   Set host interface trap group attribute value.
*
* Arguments:
*    [in] hostif_trap_group_id - host interface trap group id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_hostif_trap_group_attribute_fn)
(
    _In_ sai_object_id_t hostif_trap_group_id,
    _In_ const sai_attribute_t *attr
);

/*
* Routine Description:
*   get host interface trap group attribute value.
*
* Arguments:
*    [in] hostif_trap_group_id - host interface trap group id
*    [in] attr_count - number of attributes
*    [in,out] attr_list - array of attributes
*
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_get_hostif_trap_group_attribute_fn)(
    _In_ sai_object_id_t hostif_trap_group_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

typedef enum _sai_hostif_trap_id_t
{
    /* control plane protocol*/

    /* 
     * switch trap 
     */

    /* default action is drop */
    SAI_HOSTIF_TRAP_ID_STP = 0x00000001,

    /* default action is drop */
    SAI_HOSTIF_TRAP_ID_LACP = 0x00000002,

    /* default action is drop */
    SAI_HOSTIF_TRAP_ID_EAPOL = 0x00000003,

    /* default action is drop */
    SAI_HOSTIF_TRAP_ID_LLDP = 0x00000004,

    /* default action is drop */
    SAI_HOSTIF_TRAP_ID_PVRST = 0x00000005,

    /* default action is forward */
    SAI_HOSTIF_TRAP_ID_IGMP_TYPE_QUERY = 0x00000006,

    /* default action is forward */
    SAI_HOSTIF_TRAP_ID_IGMP_TYPE_LEAVE = 0x00000007,

    /* default action is forward */
    SAI_HOSTIF_TRAP_ID_IGMP_TYPE_V1_REPORT = 0x00000008,

    /* default action is forward */
    SAI_HOSTIF_TRAP_ID_IGMP_TYPE_V2_REPORT = 0x00000009,

    /* default action is forward */
    SAI_HOSTIF_TRAP_ID_IGMP_TYPE_V3_REPORT = 0x00000000a,

    /* default action is trap */
    SAI_HOSTIF_TRAP_ID_SAMPLEPACKET = 0x00000000b,

    SAI_HOSTIF_TRAP_ID_SWITCH_CUSTOM_RANGE_BASE = 0x00001000,

    /*
    * router trap
    */

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_ARP_REQUEST = 0x00002000,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_ARP_RESPONSE = 0x00002001,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_DHCP = 0x00002002,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_OSPF = 0x00002003,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_PIM = 0x00002004,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_VRRP = 0x00002005,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_BGP = 0x00002006,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_DHCPV6 = 0x00002007,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_OSPFV6 = 0x00002008,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_VRRPV6 = 0x00002009,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_BGPV6 = 0x0000200a,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_IPV6_NEIGHBOR_DISCOVERY = 0x0000200b,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_IPV6_MLD_V1_V2 = 0x0000200c,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_IPV6_MLD_V1_REPORT = 0x0000200d,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_IPV6_MLD_V1_DONE = 0x0000200e,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_MLD_V2_REPORT = 0x0000200f,

    /* default packet action is forward */
    SAI_HOSTIF_TRAP_ID_ROUTER_CUSTOM_RANGE_BASE = 0x0003000,

    /*
    * pipeline exceptions
    */

    /* packets size exceeds the router interface MTU size
     * (default packet action is trap) */
    SAI_HOSTIF_TRAP_ID_L3_MTU_ERROR = 0x00004000,

    /* packets with TTL 0 or 1
     * (default packet action is trap) */
    SAI_HOSTIF_TRAP_ID_TTL_ERROR = 0x00004001,

    SAI_HOSTIF_TRAP_ID_CUSTOM_EXCEPTION_RANGE_BASE = 0x00005000,

} sai_hostif_trap_id_t;

typedef enum _sai_hostif_trap_channel_t
{
    /* receive packets via file desriptor  */
    SAI_HOSTIF_TRAP_CHANNEL_FD,

    /* receive packets via callback */
    SAI_HOSTIF_TRAP_CHANNEL_CB,

    /* receive packets via OS net device */
    SAI_HOSTIF_TRAP_CHANNEL_NETDEV,

    SAI_HOSTIF_TRAP_CHANNEL_CUSTOM_RANGE_BASE = 0x10000000

} sai_hostif_trap_channel_t;

typedef enum _sai_hostif_trap_attr_t
{
    /* trap action [sai_packet_action_t] */
    SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION,

    /* Below attributes are only valid when
     * SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION == SAI_PACKET_ACTION_TRAP or 
     * SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION == SAI_PACKET_ACTION_LOG */

    /* trap priority [sai_uint32_t]
     * This is equivalent to ACL entry priority SAI_ACL_ENTRY_ATTR_PRIORITY
     * (default to SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY) */
    SAI_HOSTIF_TRAP_ATTR_TRAP_PRIORITY,

    /* trap channel to use [sai_hostif_trap_channel_t]
     * (default to SAI_SWITCH_ATTR_DEFAULT_TRAP_CHANNEL) */
    SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL,

    /* file descriptor [sai_object_id_t]
     * Valid only when SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL == SAI_HOSTIF_TRAP_CHANNEL_FD
     * Must be set before set SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL to SAI_HOSTIF_TRAP_CHANNEL_FD 
     * (default to SAI_SWITCH_ATTR_DEFAULT_TRAP_CHANNEL_FD) */
    SAI_HOSTIF_TRAP_ATTR_FD,

    /* enable trap for a list of SAI ports [sai_object_list_t]
     * (default to all SAI ports) */
    SAI_HOSTIF_TRAP_ATTR_PORT_LIST,

    /* trap-group ID for the trap [sai_object_id_t]
     * (default to SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP) */
    SAI_HOSTIF_TRAP_ATTR_TRAP_GROUP,

    SAI_HOSTIF_TRAP_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_hostif_trap_attr_t;

/*
* Routine Description:
*   Set trap attribute value.
*
* Arguments:
*    [in] hostif_trap_id - host interface trap id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_set_hostif_trap_attribute_fn)(
    _In_ sai_hostif_trap_id_t hostif_trapid,
    _In_ const sai_attribute_t *attr
    );

/*
* Routine Description:
*   Get trap attribute value.
*
* Arguments:
*    [in] hostif_trap_id - host interface trap id
*    [in] attr_count - number of attributes
*    [in,out] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_get_hostif_trap_attribute_fn)(
    _In_ sai_hostif_trap_id_t hostif_trapid,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

#define SAI_HOSTIF_USER_DEFINED_TRAP_ID_TABLE_RANGE 0x0

/*
 * user defined traps
 */
typedef enum _sai_hostif_user_defined_trap_id_t
{
    /* Samplepacket traps  */
    SAI_HOSTIF_USER_DEFINED_TRAP_ID_SAMPLEPACKET = 0x00000001,

    /* ACL traps */
    SAI_HOSTIF_USER_DEFINED_TRAP_ID_ACL_MIN = 0x00000002,

    SAI_HOSTIF_USER_DEFINED_TRAP_ID_ACL_MAX = SAI_HOSTIF_USER_DEFINED_TRAP_ID_ACL_MIN + SAI_HOSTIF_USER_DEFINED_TRAP_ID_TABLE_RANGE,

    /* router traps */
    SAI_HOSTIF_USER_DEFINED_TRAP_ID_ROUTER_MIN = SAI_HOSTIF_USER_DEFINED_TRAP_ID_ACL_MAX + 1,

    SAI_HOSTIF_USER_DEFINED_TRAP_ID_ROUTER_MAX = SAI_HOSTIF_USER_DEFINED_TRAP_ID_ROUTER_MIN + SAI_HOSTIF_USER_DEFINED_TRAP_ID_TABLE_RANGE,

    /* neighbor table traps */
    SAI_HOSTIF_USER_DEFINED_TRAP_ID_NEIGH_MIN = SAI_HOSTIF_USER_DEFINED_TRAP_ID_ROUTER_MAX+1,

    SAI_HOSTIF_USER_DEFINED_TRAP_ID_NEIGH_MAX = SAI_HOSTIF_USER_DEFINED_TRAP_ID_NEIGH_MIN + SAI_HOSTIF_USER_DEFINED_TRAP_ID_TABLE_RANGE,

    /* fdb traps */
    SAI_HOSTIF_USER_DEFINED_TRAP_ID_FDB_MIN = SAI_HOSTIF_USER_DEFINED_TRAP_ID_NEIGH_MAX + 1,

    SAI_HOSTIF_USER_DEFINED_TRAP_ID_FDB_MAX = SAI_HOSTIF_USER_DEFINED_TRAP_ID_FDB_MIN + SAI_HOSTIF_USER_DEFINED_TRAP_ID_TABLE_RANGE,

    SAI_HOSTIF_TRAP_ID_CUSTOM_RANGE_BASE = 0x10000000,

} sai_hostif_user_defined_trap_id_t;

typedef enum _sai_hostif_user_defined_trap_attr_t
{
    /* trap channel to use [sai_hostif_trap_channel_t]
     * (default to SAI_SWITCH_ATTR_DEFAULT_TRAP_CHANNEL) */
    SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TRAP_CHANNEL,

    /* file descriptor [sai_object_id_t]
     * Valid only when SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL == SAI_HOSTIF_TRAP_CHANNEL_FD
     * Must be set before set SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL to SAI_HOSTIF_TRAP_CHANNEL_FD 
     * (default to SAI_SWITCH_ATTR_DEFAULT_TRAP_CHANNEL_FD) */
    SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_FD,

} sai_hostif_user_defined_trap_attr_t;

/*
* Routine Description:
*   Set user defined trap attribute value.
*
* Arguments:
*    [in] hostif_user_defined_trap_id - host interface user defined trap id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_set_hostif_user_defined_trap_attribute_fn)(
    _In_ sai_hostif_user_defined_trap_id_t hostif_user_defined_trapid,
    _In_ const sai_attribute_t *attr
    );

/*
* Routine Description:
*   Get user defined trap attribute value.
*
* Arguments:
*    [in] hostif_user_defined_trap_id - host interface user defined trap id
*    [in] attr_count - number of attributes
*    [in,out] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_get_hostif_user_defined_trap_attribute_fn)(
    _In_ sai_hostif_user_defined_trap_id_t hostif_user_defined_trapid,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/*
*  Attribute data for SAI_HOSTIF_ATTR_TYPE
*/
typedef enum _sai_hostif_type_t
{
    /* Netdev-based Host Interface Type */
    SAI_HOSTIF_TYPE_NETDEV,

    /* file descriptor */
    SAI_HOSTIF_TYPE_FD

} sai_hostif_type_t;

/*
*  Host interface attribute IDs
*/
typedef enum _sai_hostif_attr_t
{
    /* READ-ONLY */

    /* READ-WRITE */

    /* Host interface type [sai_hostif_type_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_HOSTIF_ATTR_TYPE,

    /* Assosiated port or router interface [sai_object_id_t]
    * Valid only when SAI_HOSTIF_ATTR_TYPE == SAI_HOSTIF_TYPE_NETDEV
    *   (MANDATORY_ON_CREATE when SAI_HOSTIF_ATTR_TYPE == SAI_HOSTIF_TYPE_NETDEV | CREATE_ONLY) */
    SAI_HOSTIF_ATTR_RIF_OR_PORT_ID,

    /* Name [char[HOSTIF_NAME_SIZE]]
    * The maximum number of charactars for the name is HOSTIF_NAME_SIZE - 1 since
    * it needs the terminating null byte ('\0') at the end.
    * Valid only when SAI_HOSTIF_ATTR_TYPE == SAI_HOSTIF_TYPE_NETDEV
    *   (MANDATORY_ON_CREATE when SAI_HOSTIF_ATTR_TYPE == SAI_HOSTIF_TYPE_NETDEV) */
    SAI_HOSTIF_ATTR_NAME,

    /* Custom range base value */
    SAI_HOSTIF_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_hostif_attr_t;

/*
* Routine Description:
*    Create host interface
*
* Arguments:
*    [out] hif_id - host interface id
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_create_hostif_fn)(
    _Out_ sai_object_id_t * hif_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/*
* Routine Description:
*    Remove host interface
*
* Arguments:
*    [in] hif_id - host interface id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_remove_hostif_fn)(
    _In_ sai_object_id_t hif_id
    );

/*
* Routine Description:
*    Set host interface attribute
*
* Arguments:
*    [in] hif_id - host interface id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_set_hostif_attribute_fn)(
    _In_ sai_object_id_t hif_id,
    _In_ const sai_attribute_t *attr
    );

/*
* Routine Description:
*    Get host interface attribute
*
* Arguments:
*    [in] hif_id - host interface id
*    [in] attr_count - number of attributes
*    [inout] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_get_hostif_attribute_fn)(
    _In_ sai_object_id_t  hif_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

typedef enum _sai_hostif_tx_type
{
    /* bypass switch ASIC processing pipeline, 
     * tx packet goes to the specified output port directly */
    SAI_HOSTIF_TX_TYPE_PIPELINE_BYPASS,

    /* tx packet goes to the switch ASIC processing pipeline to decide the output port */
    SAI_HOSTIF_TX_TYPE_PIPELINE_LOOKUP,

    SAI_HOSTIF_TX_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_hostif_tx_type_t;

typedef enum _sai_hostif_packet_attr
{
    /* Trap ID [sai_hostif_trap_id_t] (for receive-only) */
    SAI_HOSTIF_PACKET_TRAP_ID,

    /* Ingress port [sai_object_id_t] (for receive-only) */
    SAI_HOSTIF_PACKET_INGRESS_PORT,

    /* Ingress LAG [sai_object_id_t] (for receive-only) */
    SAI_HOSTIF_PACKET_INGRESS_LAG,

    /* packet transmit type [sai_hostif_tx_type_t]. (MANDATORY_ON_SEND) */
    SAI_HOSTIF_PACKET_TX_TYPE,

    /* Egress port or LAG [sai_object_id_t] (for send-only).
     * (MANDATORY_ON_SEND when SAI_HOSTIF_PACKET_TX_TYPE == SAI_HOSTIF_TX_TYPE_PIPELINE_BYPASS) */
    SAI_HOSTIF_PACKET_EGRESS_PORT_OR_LAG,

} sai_hostif_packet_attr_t;

/*
* Routine Description:
*   hostif receive function
*
* Arguments:
*    [in]  hif_id  - host interface id
*    [out] buffer - packet buffer
*    [in,out] buffer_size - [in] allocated buffer size. [out] actual packet size in bytes
*    [in,out] attr_count - [in] allocated list size. [out] number of attributes
*    [out] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    SAI_STATUS_BUFFER_OVERFLOW if buffer_size is insufficient,
*    and buffer_size will be filled with required size. Or
*    if attr_count is insufficient, and attr_count
*    will be filled with required count.
*    Failure status code on error
*/
typedef sai_status_t(*sai_recv_hostif_packet_fn)(
    _In_ sai_object_id_t  hif_id,
    _Out_ void *buffer,
    _Inout_ sai_size_t *buffer_size,
    _Inout_ uint32_t *attr_count,
    _Out_ sai_attribute_t *attr_list
    );

/*
* Routine Description:
*   hostif send function
*
* Arguments:
*    [in] hif_id  - host interface id. only valid for send through FD channel. Use SAI_NULL_OBJECT_ID for send through CB channel.
*    [In] buffer - packet buffer
*    [in] buffer size - packet size in bytes
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t(*sai_send_hostif_packet_fn)(
    _In_ sai_object_id_t  hif_id,
    _In_ void *buffer,
    _In_ sai_size_t buffer_size,
    _In_ uint32_t attr_count,
    _In_ sai_attribute_t *attr_list
    );

/*
* Routine Description:
*   hostif receive callback
*
* Arguments:
*    [in] buffer - packet buffer
*    [in] buffer_size - actual packet size in bytes
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*/
typedef void(*sai_packet_event_notification_fn)(
    _In_ const void *buffer,
    _In_ sai_size_t buffer_size,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/*
* hostif methods table retrieved with sai_api_query()
*/
typedef struct _sai_hostif_api_t
{
    sai_create_hostif_fn                           create_hostif;
    sai_remove_hostif_fn                           remove_hostif;
    sai_set_hostif_attribute_fn                    set_hostif_attribute;
    sai_get_hostif_attribute_fn                    get_hostif_attribute;
    sai_create_hostif_trap_group_fn                create_hostif_trap_group;
    sai_remove_hostif_trap_group_fn                remove_hostif_trap_group;
    sai_set_hostif_trap_group_attribute_fn         set_trap_group_attribute;
    sai_get_hostif_trap_group_attribute_fn         get_trap_group_attribute;
    sai_set_hostif_trap_attribute_fn               set_trap_attribute;
    sai_get_hostif_trap_attribute_fn               get_trap_attribute;
    sai_set_hostif_user_defined_trap_attribute_fn  set_user_defined_trap_attribute;
    sai_get_hostif_user_defined_trap_attribute_fn  get_user_defined_trap_attribute;
    sai_recv_hostif_packet_fn                      recv_packet;
    sai_send_hostif_packet_fn                      send_packet;
} sai_hostif_api_t;


#endif // __SAIHOSTINTF_H_

