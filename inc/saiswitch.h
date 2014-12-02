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
#define SAI_MAX_MICROCODE_NAME_LEN      PATH_MAX

#define SWITCH_COUNTER_SET_DEFAULT      0

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


typedef enum _sai_switch_ecmp_hash_fields_t
{
    SAI_SWITCH_ECMP_HASH_SRC_IP         = (1 << 0),
    SAI_SWITCH_ECMP_HASH_DST_IP         = (1 << 1),
    SAI_SWITCH_ECMP_HASH_L4_SRC_PORT    = (1 << 2),
    SAI_SWITCH_ECMP_HASH_L4_DST_PORT    = (1 << 3),
    
} sai_switch_ecmp_hash_fields_t;

/*
*  Switch counter IDs in sai_get_switch_stat_counters() call
*/
typedef enum _sai_switch_stat_counter_t
{
    SAI_SWITCH_STAT_GLOBAL_LOW_DROP_PKTS,
    SAI_SWITCH_STAT_GLOBAL_HIGH_DROP_PKTS,
    SAI_SWITCH_STAT_GLOBAL_PRIVILEGE_DROP_PKTS,
    SAI_SWITCH_STAT_DROP_COUNT_TX,
    SAI_SWITCH_STAT_DROP_COUNT_RX

} sai_switch_stat_counter_t;


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

    /* 
    *   Local subnet routing supported [bool]
    *   Routes with next hop set to "on-link"
    */
    SAI_SWITCH_ATTR_ON_LINK_ROUTE_SUPPORTED,

    /* Oper state [sai_switch_oper_status_t] */
    SAI_SWITCH_ATTR_OPER_STATUS,


    /* READ-WRITE */

    /* Hardware generation counter [uint64_t] */
    SAI_SWITCH_ATTR_HW_SEQUENCE_ID,

    /* Admin state [bool] */
    SAI_SWITCH_ATTR_ADMIN_STATE,

    /* L2 broadcast flood control to CPU port [bool] */
    SAI_SWITCH_ATTR_BCAST_CPU_FLOOD_ENABLE,

    /* L2 multicast flood control to CPU port [bool] */
    SAI_SWITCH_ATTR_MCAST_CPU_FLOOD_ENABLE,

    /* Default VlanID for ports that are not members of any vlans [uint16] */
    SAI_SWITCH_ATTR_DEFAULT_PORT_VLAN_ID,

    /* Maximum number of learned MAC addresses [uint32_t] */
    SAI_SWITCH_ATTR_MAX_LEARNED_ADDRESSES,

    /* Flood control for packets with unknown destination address.
    *   [sai_switch_fdb_miss_action_t]
    */
    SAI_SWITCH_ATTR_FDB_UNICAST_MISS_ACTION,

    SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_ACTION,

    SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_ACTION,

    /* ECMP hashing type  [sai_switch_ecmp_hash_type_t] */
    SAI_SWITCH_ATTR_ECMP_HASH_TYPE,

    /* ECMP hashing fields [sai_switch_ecmp_hash_fields_t] */
    SAI_SWITCH_ATTR_ECMP_HASH_FIELDS,

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
*   [in/opt] microcode_module_name - Vendor specific name of the microcode 
*                                     module to load
*   [in] switch_notifications - switch notification table
* Return Values:
*   SAI_STATUS_SUCCESS on success
*   Failure status code on error
*/
typedef sai_status_t (*sai_initialize_switch_fn)(
    _In_ sai_switch_profile_id_t profile_id,
    _In_reads_z_(SAI_MAX_HARDWARE_ID_LEN) char* switch_hardware_id,
    _In_reads_opt_z_(SAI_MAX_MICROCODE_NAME_LEN) char* microcode_module_name,
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
*    Set switch attribute value
*
* Arguments:
*    [in] attribute - switch attribute
*    [in] value - switch attribute value
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_switch_attribute_fn)(
    _In_ sai_switch_attr_t attribute, 
    _In_ uint64_t value
    );


/*
* Routine Description:
*    Get switch attribute value
*
* Arguments:
*    [in] attribute - switch attribute
*    [out] value - switch attribute value
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_switch_attribute_fn)(
    _In_ sai_switch_attr_t attribute, 
    _Out_ uint64_t* value
    );


/*
* Routine Description:
*   Enable/disable statistics counters for switch.
*
* Arguments:
*    [in] counter_set_id - specifies the counter set
*    [in] enable - TRUE to enable, FALSE to disable
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/ 
typedef sai_status_t (*sai_ctl_switch_stats_fn)(
    _In_ uint32_t counter_set_id,
    _In_ bool enable
    );

/*
* Routine Description:
*   Get switch statistics counters.
*
* Arguments:
*    [in] counter_set_id - specifies the counter set
*    [in] number_of_counters - number of counters in the array
*    [out] counters - array of resulting counter values.
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/ 
typedef sai_status_t (*sai_get_switch_stats_fn)(
    _In_ uint32_t counter_set_id,
    _In_ uint32_t number_of_counters,
    _Out_ uint64_t* counters
    );

/*
* Switch method table retrieved with sai_api_query() 
*/
typedef struct _sai_switch_api_t
{
    sai_initialize_switch_fn        initialize_switch;
    sai_shutdown_switch_fn          shutdown_switch;
    sai_set_switch_attribute_fn     set_attribute;
    sai_get_switch_attribute_fn     get_attribute;
    sai_ctl_switch_stats_fn         ctl_stats;
    sai_get_switch_stats_fn         get_stats;

} sai_switch_api_t;

#endif  // __SAISWITCH_H_

