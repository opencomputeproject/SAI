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
#include "saihostintf.h"

/** \defgroup SAISWITCH SAI - Switch specific API definitions.
 *
 *  \{
 */

#define SAI_MAX_HARDWARE_ID_LEN         255
#define SAI_MAX_FIRMWARE_PATH_NAME_LEN  PATH_MAX

/**
 *  @brief Attribute data for SAI_SWITCH_ATTR_OPER_STATUS
 */
typedef enum _sai_switch_oper_status_t
{
    /** Unknown */
    SAI_SWITCH_OPER_STATUS_UNKNOWN,

    /** Up */
    SAI_SWITCH_OPER_STATUS_UP,

    /** Down */
    SAI_SWITCH_OPER_STATUS_DOWN,

    /** Switch encountered a fatal error */
    SAI_SWITCH_OPER_STATUS_FAILED,

} sai_switch_oper_status_t;

/**
*  @brief Attribute data for packet action
*/
typedef enum _sai_packet_action_t
{
    /** Basic Packet Actions */

    /** These could be further classified based on the nature of action
     * - Data Plane Packet Actions
     * - CPU Path Packet Actions */

    /** Data Plane Packet Actions
     * Following two packet actions only affect the packet action on the data plane.
     * Packet action on the CPU path remains unchanged. */

    /** Drop Packet in data plane */
    SAI_PACKET_ACTION_DROP,

    /** Forward Packet in data plane. */
    SAI_PACKET_ACTION_FORWARD,

    /** CPU Path Packet Actions
     * Following two packet actions only affect the packet action on the CPU path.
     * Packet action on the data plane remains unchanged. */

    /** Copy Packet to CPU. */
    SAI_PACKET_ACTION_COPY,

    /** Cancel copy the packet to CPU. */
    SAI_PACKET_ACTION_COPY_CANCEL,

    /** Combination of Packet Actions */

    /** This is a combination of sai packet action COPY and DROP. */
    SAI_PACKET_ACTION_TRAP,

    /** This is a combination of sai packet action COPY and FORWARD. */
    SAI_PACKET_ACTION_LOG,

    /** This is a combination of sai packet action COPY_CANCEL and DROP */
    SAI_PACKET_ACTION_DENY,

    /** This is a combination of sai packet action COPY_CANCEL and FORWARD */
    SAI_PACKET_ACTION_TRANSIT

} sai_packet_action_t;

/**
 * Attribute data for SAI_SWITCH_ ATTR_LAG_HASH_FIELDS
 * and SAI_SWITCH_ ATTR_ECMP_HASH_FIELDS
 */
typedef enum _sai_switch_hash_field_types_t
{
    SAI_HASH_SRC_IP = 0,
    SAI_HASH_DST_IP = 1,
    SAI_HASH_VLAN_ID = 2,
    SAI_HASH_IP_PROTOCOL = 3,
    SAI_HASH_ETHERTYPE = 4,
    SAI_HASH_L4_SOURCE_PORT = 5,
    SAI_HASH_L4_DEST_PORT = 6,
    SAI_HASH_SOURCE_MAC = 7,
    SAI_HASH_DEST_MAC = 8,
    SAI_HASH_IN_PORT = 9,
} sai_switch_hash_field_types_t;

/**
 * Attribute data for SAI_SWITCH_ATTR_LAG_HASH_ALGO
 * and SAI_SWITCH_ATTR_ECMP_HASH_ALGO
 */
typedef enum _sai_switch_hash_algo_t
{
    SAI_HASH_XOR = 1,
    SAI_HASH_CRC = 2,
    SAI_HASH_RANDOM = 3,
} sai_switch_hash_algo_t;

/**
* @brief Attribute data for SAI_SWITCH_SWITCHING_MODE
*/
typedef enum _sai_switch_switching_mode_t
{
    /** cut-through switching mode */
    SAI_SWITCHING_MODE_CUT_THROUGH,

    /** store-and-forward switching mode */
    SAI_SWITCHING_MODE_STORE_AND_FORWARD

} sai_switch_switching_mode_t;

/**
*  Attribute Id in sai_set_switch_attribute() and
*  sai_get_switch_attribute() calls
*/
typedef enum _sai_switch_attr_t
{
    /** READ-ONLY */

    /** The number of ports on the switch [uint32_t] */
    SAI_SWITCH_ATTR_PORT_NUMBER,

    /** Get the port list [sai_object_list_t] */
    SAI_SWITCH_ATTR_PORT_LIST,

    /** Get the Max MTU in bytes, Supported by the switch [uint32_t] */
    SAI_SWITCH_ATTR_PORT_MAX_MTU,

    /** Get the CPU Port [sai_object_id_t] */
    SAI_SWITCH_ATTR_CPU_PORT,

    /** Max number of virtual routers supported [uint32_t] */
    SAI_SWITCH_ATTR_MAX_VIRTUAL_ROUTERS,

    /** The size of the FDB Table in bytes [uint32_t] */
    SAI_SWITCH_ATTR_FDB_TABLE_SIZE,

    /**
    *   Local subnet routing supported [bool]
    *   Routes with next hop set to "on-link"
    */
    SAI_SWITCH_ATTR_ON_LINK_ROUTE_SUPPORTED,

    /** Oper state [sai_switch_oper_status_t] */
    SAI_SWITCH_ATTR_OPER_STATUS,

    /** The current value of the maximum temperature
     * retrieved from the switch sensors, in Celsius [int32_t] */
    SAI_SWITCH_ATTR_MAX_TEMP,

    /** minimum priority for ACL table [sai_uint32_t] */
    SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY,

    /** maximum priority for ACL table [sai_uint32_t] */
    SAI_SWITCH_ATTR_ACL_TABLE_MAXIMUM_PRIORITY,

    /** minimum priority for ACL entry [sai_uint32_t] */
    SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY,

    /** maximum priority for ACL entry [sai_uint32_t] */
    SAI_SWITCH_ATTR_ACL_ENTRY_MAXIMUM_PRIORITY,

    /** FDB DST user-based meta data range [sai_u32_range_t] */
    SAI_SWITCH_ATTR_FDB_DST_USER_META_DATA_RANGE,

    /** Route DST Table user-based meta data range [sai_u32_range_t] */
    SAI_SWITCH_ATTR_ROUTE_DST_USER_META_DATA_RANGE,

    /** Neighbor DST Table user-based meta data range [sai_u32_range_t] */
    SAI_SWITCH_ATTR_NEIGHBOR_DST_USER_META_DATA_RANGE,

    /** Port user-based meta data range [sai_u32_range_t] */
    SAI_SWITCH_ATTR_PORT_USER_META_DATA_RANGE,

    /** VLAN user-based meta data range [sai_u32_range_t] */
    SAI_SWITCH_ATTR_VLAN_USER_META_DATA_RANGE,

    /** ACL user-based ACL meta data range [sai_u32_range_t] */
    SAI_SWITCH_ATTR_ACL_USER_META_DATA_RANGE,

    /** Default SAI STP instance ID [sai_object_id_t] */
    SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID,

    /** Maximum traffic classes limit*/
    SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_TRAFFIC_CLASSES,

    /** HQOS - Maximum Number of Hierarchy scheduler
     *  group levels(depth) supported [sai_uint32_t]*/
    SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUP_HIERARCHY_LEVELS,

    /** HQOS - Maximum number of scheduler groups supported on
     * each Hierarchy level [sai_u32_list_t] */
    SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUPS_PER_HIERARCHY_LEVEL,

    /** Maximum number of ports that can be part of a LAG [uint32_t] */
    SAI_SWITCH_ATTR_MAX_LAG_MEMBERS,

    /** Maximum number of LAGs that can be created per switch [uint32_t] */
    SAI_SWITCH_ATTR_MAX_LAG_NUMBER,

    /** switch total buffer size in KB [sai_uint32_t] */
    SAI_SWITCH_ATTR_TOTAL_BUFFER_SIZE,

    /** switch number of ingress buffer pool [sai_uint32_t] */
    SAI_SWITCH_ATTR_INGRESS_BUFFER_POOL_NUM,

    /** switch number of egress buffer pool [sai_uint32_t] */
    SAI_SWITCH_ATTR_EGRESS_BUFFER_POOL_NUM,

    /** Default trap group [sai_object_id_t]
    * (default value after switch initialization
    *    SAI_HOSTIF_TRAP_GROUP_ATTR_ADMIN_STATE = true
    *    SAI_HOSTIF_TRAP_GROUP_ATTR_PRIO = SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY,
    *    SAI_HOSTIF_TRAP_GROUP_ATTR_QUEUE = 0,
    *    SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER = SAI_NULL_OBJECT_ID)
    * The group handle is read only, while the group attributes, such as queue and policer,
    * may be modified */
    SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP,

    /** READ-WRITE */

    /** Switching mode [sai_switch_switching_mode_t]
       (default to SAI_SWITCHING_MODE_STORE_AND_FORWARD) */
    SAI_SWITCH_ATTR_SWITCHING_MODE,

    /** L2 broadcast flood control to CPU port [bool] */
    SAI_SWITCH_ATTR_BCAST_CPU_FLOOD_ENABLE,

    /** L2 multicast flood control to CPU port [bool] */
    SAI_SWITCH_ATTR_MCAST_CPU_FLOOD_ENABLE,

    /** Default switch MAC Address [sai_mac_t] */
    SAI_SWITCH_ATTR_SRC_MAC_ADDRESS,

    /** Maximum number of learned MAC addresses [uint32_t]
     * zero means learning limit disable. (default to zero) */
    SAI_SWITCH_ATTR_MAX_LEARNED_ADDRESSES,

    /** Dynamic FDB entry aging time in seconds [uint32_t]
    *   Zero means aging is disabled.
    *  (default to zero)
    */
    SAI_SWITCH_ATTR_FDB_AGING_TIME,

    /** Flood control for packets with unknown destination address.
    *   [sai_packet_action_t] (default to SAI_PACKET_ACTION_FORWARD)
    */
    SAI_SWITCH_ATTR_FDB_UNICAST_MISS_ACTION,

    SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_ACTION,

    SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_ACTION,

    /** Hash algorithm for all LAG in the switch[sai_switch_hash_algo_t]
     * (default to SAI_HASH_CRC)
     */
    SAI_SWITCH_ATTR_LAG_HASH_ALGO,

    /** Hash seed for all LAG in the switch[sai_switch_hash_seed_t]*/
    SAI_SWITCH_ATTR_LAG_HASH_SEED,

    /** Hash fields for all LAG in the switch[sai_s32_list_t]
     * (default all fields in sai_switch_hash_field_types_t are enabled)
     */
    SAI_SWITCH_ATTR_LAG_HASH_FIELDS,

    /** Hash algorithm for all ECMP in the switch[sai_switch_hash_algo_t]
     * (default to SAI_HASH_CRC)
     */
    SAI_SWITCH_ATTR_ECMP_HASH_ALGO,

    /** Hash seed for all ECMP in the switch[sai_switch_hash_seed_t]*/
    SAI_SWITCH_ATTR_ECMP_HASH_SEED,

    /** Hash fields for all ECMP in the switch[sai_s32_list_t]
     * (default all fields in sai_switch_hash_field_types_t are enabled)
     */
    SAI_SWITCH_ATTR_ECMP_HASH_FIELDS,

    /** ECMP max number of paths per group [uint32_t]
       (default to 64) */
    SAI_SWITCH_ATTR_ECMP_MAX_PATHS,

    /** The SDK can
     * 1 - Read the counters directly from HW (or)
     * 2 - Cache the counters in SW. Caching is typically done if
     * retrieval of counters directly from HW for each counter
     * read is CPU intensive
     * This setting can be used to
     * 1 - Move from HW based to SW based or Vice versa
     * 2 - Configure the SW counter cache refresh rate
     * Setting a value of 0 enables direct HW based counter read. A
     * non zero value enables the SW cache based and the counter
     * refresh rate.
     * A NPU may support both or one of the option. It would return
     * error for unsupported options
     *
     * Default - 1 sec (SW counter cache)
     *
     * [uint32_t]
     */
    SAI_SWITCH_ATTR_COUNTER_REFRESH_INTERVAL,

    /** Default Traffic class value, Defalut TC = 0 */
    SAI_SWITCH_ATTR_QOS_DEFAULT_TC,

    /** Enable DOT1P -> TC MAP [sai_object_id_t] on switch.
     * MAP id = SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust Dot1p, Map ID should be add/remove on switch.
     * Default disabled */
    SAI_SWITCH_ATTR_QOS_DOT1P_TO_TC_MAP,

    /** Enable DOT1P -> COLOR MAP [sai_object_id_t] on switch.
     * MAP id = SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust Dot1p, Map ID should be add/remove on switch.
     * Default disabled */
    SAI_SWITCH_ATTR_QOS_DOT1P_TO_COLOR_MAP,

    /** Enable DOT1P -> TC AND COLOR MAP [sai_object_id_t] on switch.
     * MAP id = SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust Dot1p, Map ID should be add/remove on switch.
     * Default disabled */
    SAI_SWITCH_ATTR_QOS_DOT1P_TO_TC_AND_COLOR_MAP,

    /** Enable DSCP -> TC MAP [sai_object_id_t] on switch.
     * MAP id = SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust DSCP, Map ID should be add/remove on port.
     * Default no map */
    SAI_SWITCH_ATTR_QOS_DSCP_TO_TC_MAP,

    /** Enable DSCP -> COLOR MAP [sai_object_id_t] on switch
     * MAP id = SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust DSCP, Map ID should be add/remove on switch.
     * Default no map */
    SAI_SWITCH_ATTR_QOS_DSCP_TO_COLOR_MAP,

    /** Enable DSCP -> TC AND COLOR MAP [sai_object_id_t] on switch
     * MAP id = SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust DSCP, Map ID should be add/remove on switch.
     * Default no map */
    SAI_SWITCH_ATTR_QOS_DSCP_TO_TC_AND_COLOR_MAP,

    /** Enable TC -> Queue MAP [sai_object_id_t] on switch
     * Map id = SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map i.e All packets to queue 0.
     */
    SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP,

    /** Enable TC -> DOT1P MAP [sai_object_id_t]
       Map id = SAI_NULL_OBJECT_ID to disable map on switch.
       Default no map */
    SAI_SWITCH_ATTR_QOS_TC_TO_DOT1P_MAP,

    /** Enable TC + COLOR -> DOT1P MAP [sai_object_id_t]
       Map id = SAI_NULL_OBJECT_ID to disable map on switch.
       Default no map */
    SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP,

    /** Enable TC -> DSCP MAP [sai_object_id_t]
       Map id = SAI_NULL_OBJECT_ID to disable map on switch.
       Default no map */
    SAI_SWITCH_ATTR_QOS_TC_TO_DSCP_MAP,

    /** Enable TC + COLOR -> DSCP MAP [sai_object_id_t]
       Map id = SAI_NULL_OBJECT_ID to disable map on switch.
       Default no map */
    SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_DSCP_MAP,

    /** WRITE-ONLY */

    /** Port Breakout mode [sai_port_breakout_t] */
    SAI_SWITCH_ATTR_PORT_BREAKOUT,

    /* -- */

    /* Custom range base value */
    SAI_SWITCH_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_switch_attr_t;

/**
 * Routine Description:
 *   @brief Switch shutdown request callback.
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


/**
 * Routine Description:
 *   @brief Switch oper state change notification
 *
 * Arguments:
 *  @param[in] switch_oper_status - new switch oper state
 *
 * Return Values:
 *    None
 */
typedef void (*sai_switch_state_change_notification_fn)(
    _In_ sai_switch_oper_status_t switch_oper_status
    );


/**
 * @brief Switch notification table passed to the adapter via sai_initialize_switch()
 */
typedef struct _sai_switch_notification_t
{
    sai_switch_state_change_notification_fn on_switch_state_change;
    sai_fdb_event_notification_fn           on_fdb_event;
    sai_port_state_change_notification_fn   on_port_state_change;
    sai_port_event_notification_fn          on_port_event;
    sai_switch_shutdown_request_fn          on_switch_shutdown_request;
    sai_packet_event_notification_fn        on_packet_event;
} sai_switch_notification_t;

/**
* Routine Description:
*   SDK initialization. After the call the capability attributes should be
*   ready for retrieval via sai_get_switch_attribute().
*
* Arguments:
*   @param[in] profile_id - Handle for the switch profile.
*   @param[in] switch_hardware_id - Switch hardware ID to open
*   @param[in] firmware_path_name - Vendor specific path name of the firmware
*                                   to load
*   @param[in] switch_notifications - switch notification table
* Return Values:
*   @return SAI_STATUS_SUCCESS on success
*           Failure status code on error
*/
typedef sai_status_t (*sai_initialize_switch_fn)(
    _In_ sai_switch_profile_id_t profile_id,
    _In_reads_z_(SAI_MAX_HARDWARE_ID_LEN) char* switch_hardware_id,
    _In_reads_opt_z_(SAI_MAX_FIRMWARE_PATH_NAME_LEN) char* firmware_path_name,
    _In_ sai_switch_notification_t* switch_notifications
    );

/**
 * Routine Description:
 *   @brief Release all resources associated with currently opened switch
 *
 * Arguments:
 *   @param[in] warm_restart_hint - hint that indicates controlled warm restart.
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

/**
 * Routine Description:
 *   SDK connect. This API connects library to the initialized SDK.
 *   After the call the capability attributes should be ready for retrieval
 *   via sai_get_switch_attribute().
 *
 * Arguments:
 *   @param[in] profile_id - Handle for the switch profile.
 *   @param[in] switch_hardware_id - Switch hardware ID to open
 *   @param[in] switch_notifications - switch notification table
 * Return Values:
 *   @return SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t (*sai_connect_switch_fn)(
    _In_ sai_switch_profile_id_t profile_id,
    _In_reads_z_(SAI_MAX_HARDWARE_ID_LEN) char* switch_hardware_id,
    _In_ sai_switch_notification_t* switch_notifications
    );

/**
 * Routine Description:
 *   @brief Disconnect this SAI library from the SDK.
 *
 * Arguments:
 *   None
 * Return Values:
 *   None
 */
typedef void (*sai_disconnect_switch_fn)(
    void
    );


/**
 * Routine Description:
 *    @brief Set switch attribute value
 *
 * Arguments:
 *    @param[in] attr - switch attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_switch_attribute_fn)(
    _In_ const sai_attribute_t *attr
    );


/**
 * Routine Description:
 *    @brief Get switch attribute value
 *
 * Arguments:
 *    @param[in] attr_count - number of switch attributes
 *    @param[inout] attr_list - array of switch attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_switch_attribute_fn)(
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * @brief Switch method table retrieved with sai_api_query()
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

/**
 *\}
 */
#endif  // __SAISWITCH_H_

