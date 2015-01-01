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
*    saiswitch.h
*
* Abstract:
*
*    This module defines SAI Switch API
*
*/

#if !defined (__SAISWITCH_H_)
#define __SAISWITCH_H_

#include <saitypes.h>
#include "saiport.h"
#include "saifdb.h"


#define SAI_MAX_HARDWARE_ID_LEN         255
#define SAI_MAX_FIRMWARE_PATH_NAME_LEN  PATH_MAX

/*
*  Attribute data for SAI_SWITCH_ATTR_OPER_STATUS
*/
typedef enum _sai_switch_oper_status_t
{
    /* Unknown */
    SAI_SWITCH_OPER_STATUS_UNKNOWN,

    /* Up */
    SAI_SWITCH_OPER_STATUS_UP,

    /* Down */
    SAI_SWITCH_OPER_STATUS_DOWN,

    /* Switch encountered a fatal error */
    SAI_SWITCH_OPER_STATUS_FAILED,

} sai_switch_oper_status_t;


/*  
*  Attribute data for SAI_SWITCH_ATTR_FDB_MISS_ACTION
*/ 
typedef enum _sai_switch_fdb_miss_action_t
{
    SAI_SWITCH_FDB_MISS_DISCARD,

    /* Flood on VLAN, except to CPU port **/
    SAI_SWITCH_FDB_MISS_FORWARD,

    /* Trap to CPU port **/
    SAI_SWITCH_FDB_MISS_TRAP,

    /* Forward + Trap **/
    SAI_SWITCH_FDB_MISS_LOG

} sai_switch_fdb_miss_action_t;

/*
*  Attribute data for packet action
*/
typedef enum _sai_packet_action_t
{
    /* Drop Packet */
    SAI_PACKET_ACTION_DROP,

    /* Forward Packet */
    SAI_PACKET_ACTION_FORWARD,

    /* Trap Packet to CPU */
    SAI_PACKET_ACTION_TRAP,

    /* Log (Trap + Forward) Packet */
    SAI_PACKET_ACTION_LOG

} sai_packet_action_t;


/*
* Attribute data for SAI_SWITCH_ECMP_HASH_TYPE
*/
typedef enum _sai_switch_ecmp_hash_type_t
{
    SAI_SWITCH_ECMP_HASH_TYPE_XOR,

    SAI_SWITCH_ECMP_HASH_TYPE_CRC,
} sai_switch_ecmp_hash_type_t;

typedef enum _sai_switch_ecmp_hash_fields_t
{
    SAI_SWITCH_ECMP_HASH_SRC_IP         = (1 << 0),
    SAI_SWITCH_ECMP_HASH_DST_IP         = (1 << 1),
    SAI_SWITCH_ECMP_HASH_L4_SRC_PORT    = (1 << 2),
    SAI_SWITCH_ECMP_HASH_L4_DST_PORT    = (1 << 3),
    
} sai_switch_ecmp_hash_fields_t;

/*
* Attribute data for SAI_SWITCH_SWITCHING_MODE
*/
typedef enum _sai_switch_switching_mode_t 
{ 
    /* cut-through switching mode */
    SAI_SWITCHING_MODE_CUT_THROUGH,

    /* store-and-forward switching mode */
    SAI_SWITCHING_MODE_STORE_AND_FORWARD

} sai_switch_switching_mode_t;

/*
*  Attribute Id in sai_set_switch_attribute() and 
*  sai_get_switch_attribute() calls
*/
typedef enum _sai_switch_attr_t
{
    /* READ-ONLY */

    /* The number of ports on the switch [uint32_t] */
    SAI_SWITCH_ATTR_PORT_NUMBER,

    /* Max number of virtual routers supported [uint32_t] */
    SAI_SWITCH_ATTR_MAX_VIRTUAL_ROUTERS,

    /* The size of the FDB Table in bytes [uint32_t] */
    SAI_SWITCH_ATTR_FDB_TABLE_SIZE,

    /* 
    *   Local subnet routing supported [bool]
    *   Routes with next hop set to "on-link"
    */
    SAI_SWITCH_ATTR_ON_LINK_ROUTE_SUPPORTED,

    /* Oper state [sai_switch_oper_status_t] */
    SAI_SWITCH_ATTR_OPER_STATUS,

    /* The current value of the maximum temperature 
     * retrieved from the switch sensors, in Celsius [int32_t] */
    SAI_SWITCH_ATTR_MAX_TEMP,

    /* READ-WRITE */

    /* Switching mode [sai_switch_switching_mode_t] */
    SAI_SWITCH_ATTR_SWITCHING_MODE,

    /* L2 broadcast flood control to CPU port [bool] */
    SAI_SWITCH_ATTR_BCAST_CPU_FLOOD_ENABLE,

    /* L2 multicast flood control to CPU port [bool] */
    SAI_SWITCH_ATTR_MCAST_CPU_FLOOD_ENABLE,

    /* Action for Packets with TTL 0 or 1 [sai_packet_action_t] */
    SAI_SWITCH_ATTR_VIOLATION_TTL1_ACTION,

   /* Default VlanID for ports that are not members of any vlans [uint16] */
    SAI_SWITCH_ATTR_DEFAULT_PORT_VLAN_ID,

    /* Default switch MAC Address [sai_mac_t] */
    SAI_SWITCH_ATTR_SRC_MAC_ADDRESS,

    /* Maximum number of learned MAC addresses [uint32_t] */
    SAI_SWITCH_ATTR_MAX_LEARNED_ADDRESSES,

    /* Dynamic FDB entry aging time in seconds [uint32_t] 
    *   Zero means aging is disabled.
    */
    SAI_SWITCH_ATTR_FDB_AGING_TIME,

    /* Flood control for packets with unknown destination address.
    *   [sai_switch_fdb_miss_action_t]
    */
    SAI_SWITCH_ATTR_FDB_UNICAST_MISS_ACTION,

    SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_ACTION,

    SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_ACTION,

    /* ECMP hashing type  [uint32_t] */
    SAI_SWITCH_ATTR_ECMP_HASH_SEED,

    /* ECMP hashing type  [sai_switch_ecmp_hash_type_t] */
    SAI_SWITCH_ATTR_ECMP_HASH_TYPE,

    /* ECMP hashing fields [sai_switch_ecmp_hash_fields_t] */
    SAI_SWITCH_ATTR_ECMP_HASH_FIELDS,

    /* ECMP max number of paths per group [uint32_t] */
    SAI_SWITCH_ATTR_ECMP_MAX_PATHS,

    /* -- */
    
    /* Custom range base value */
    SAI_SWITCH_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_switch_attr_t;

/*
* Routine Description:
*   Switch shutdown request callback.
*   Adapter DLL may request a shutdown due to an unrecoverable failure
*   or a maintenance operation 
*
* Arguments:
*   None
*
* Return Values:
*   None
*/
typedef void (*sai_switch_shutdown_request_fn)(
    void
    );


/*
* Routine Description:
*   Switch oper state change notification
*
* Arguments:
*   [in] switch_oper_status - new switch oper state
*
* Return Values:
*    None
*/
typedef void (*sai_switch_state_change_notification_fn)(
    _In_ sai_switch_oper_status_t switch_oper_status
    );


/*
* Switch notification table passed to the adapter via sai_initialize_switch()
*/
typedef struct _sai_switch_notification_t
{
    sai_switch_state_change_notification_fn on_switch_state_change;
    sai_fdb_event_notification_fn           on_fdb_event;
    sai_port_state_change_notification_fn   on_port_state_change;
    sai_switch_shutdown_request_fn          on_switch_shutdown_request;
} sai_switch_notification_t;
  
/*
* Routine Description:
*   SDK initialization. After the call the capability attributes should be 
*   ready for retrieval via sai_get_switch_attribute().
*
* Arguments:
*   [in] profile_id - Handle for the switch profile.
*   [in] switch_hardware_id - Switch hardware ID to open
*   [in/opt] firmware_path_name - Vendor specific path name of the firmware
*                                     to load
*   [in] switch_notifications - switch notification table
* Return Values:
*   SAI_STATUS_SUCCESS on success
*   Failure status code on error
*/
typedef sai_status_t (*sai_initialize_switch_fn)(
    _In_ sai_switch_profile_id_t profile_id,
    _In_reads_z_(SAI_MAX_HARDWARE_ID_LEN) char* switch_hardware_id,
    _In_reads_opt_z_(SAI_MAX_FIRMWARE_PATH_NAME_LEN) char* firmware_path_name,
    _In_ sai_switch_notification_t* switch_notifications
    );

/*
* Routine Description:
*   Release all resources associated with currently opened switch   
*
* Arguments:
*   [in] warm_restart_hint - hint that indicates controlled warm restart.
*                            Since warm restart can be caused by crash
*                            (therefore there are no guarantees for this call),
*                            this hint is really a performance optimization.
*
* Return Values:
*   None
*/
typedef void (*sai_shutdown_switch_fn)(
    _In_ bool warm_restart_hint
    );

/*
* Routine Description:
*   SDK connect. This API connects library to the initialized SDK. 
*   After the call the capability attributes should be ready for retrieval 
*   via sai_get_switch_attribute().
*
* Arguments:
*   [in] profile_id - Handle for the switch profile.
*   [in] switch_hardware_id - Switch hardware ID to open
*   [in] switch_notifications - switch notification table
* Return Values:
*   SAI_STATUS_SUCCESS on success
*   Failure status code on error
*/
typedef sai_status_t (*sai_connect_switch_fn)(
    _In_ sai_switch_profile_id_t profile_id,
    _In_reads_z_(SAI_MAX_HARDWARE_ID_LEN) char* switch_hardware_id,
    _In_ sai_switch_notification_t* switch_notifications
    );

/*
* Routine Description:
*   Disconnect this SAI library from the SDK.
*
* Arguments:
*   None
* Return Values:
*   None
*/
typedef void (*sai_disconnect_switch_fn)(
    void
    );


/*
* Routine Description:
*    Set switch attribute value
*
* Arguments:
*    [in] attr - switch attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_switch_attribute_fn)(
    _In_ const sai_attribute_t *attr
    );


/*
* Routine Description:
*    Get switch attribute value
*
* Arguments:
*    [in] attr_count - number of switch attributes
*    [inout] attr_list - array of switch attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_switch_attribute_fn)(
    _In_ int attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/*
* Switch method table retrieved with sai_api_query() 
*/
typedef struct _sai_switch_api_t
{
    sai_initialize_switch_fn        initialize_switch;
    sai_shutdown_switch_fn          shutdown_switch;
    sai_connect_switch_fn           connect_switch;
    sai_disconnect_switch_fn        disconnect_switch;
    sai_set_switch_attribute_fn     set_switch_attribute;
    sai_get_switch_attribute_fn     get_switch_attribute;

} sai_switch_api_t;

#endif  // __SAISWITCH_H_
