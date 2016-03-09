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

#define SAI_MAX_HARDWARE_ID_LEN                 255
#define SAI_MAX_FIRMWARE_PATH_NAME_LEN          PATH_MAX

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
 *  @brief Attribute data for number of vlan tags present in a packet
 */
typedef enum _sai_packet_vlan_t
{
    /** Untagged
     *  Packet without vlan tags */
    SAI_PACKET_VLAN_UNTAG,

    /** Single Outer Tag
     *  Packet outer TPID matches to the ingress port outer TPID and
     *  Packet inner TPID if present, does not matches the configured inner TPID */
    SAI_PACKET_VLAN_SINGLE_OUTER_TAG,

    /** Double Tag
     *  Packet outer TPID matches to the ingress port outer TPID and
     *  Packet inner TPID matches to the configured inner TPID */
    SAI_PACKET_VLAN_DOUBLE_TAG

} sai_packet_vlan_t;

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
* @brief Attribute data for SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM
* and SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_ALGORITHM
*/
typedef enum _sai_hash_algorithm_t
{
    /** CRC-based hash algorithm */
    SAI_HASH_ALGORITHM_CRC = 1,

    /** XOR-based hash algorithm */
    SAI_HASH_ALGORITHM_XOR = 2,

    /** Random-based hash algorithm */
    SAI_HASH_ALGORITHM_RANDOM = 3,

} sai_hash_algorithm_t;

/**
 * @brief Attribute data for SAI_SWITCH_ATTR_RESTART_TYPE
 */
typedef enum _sai_switch_restart_type_t
{
    /** NPU doesn't support warmboot */
    SAI_RESTART_TYPE_NONE = 0,

    /** Planned restart only */
    SAI_RESTART_TYPE_PLANNED = 1,

    /** Both planned and unplanned restart */
    SAI_RESTART_TYPE_ANY = 2,


} sai_switch_restart_type_t;

/**
*  Attribute Id in sai_set_switch_attribute() and
*  sai_get_switch_attribute() calls
*/
typedef enum _sai_switch_attr_t
{
    /** READ-ONLY */

    /** The number of ports on the switch [sai_uint32_t] */
    SAI_SWITCH_ATTR_PORT_NUMBER,

    /** Get the port list [sai_object_list_t] */
    SAI_SWITCH_ATTR_PORT_LIST,

    /** Get the Max MTU in bytes, Supported by the switch [sai_uint32_t] */
    SAI_SWITCH_ATTR_PORT_MAX_MTU,

    /** Get the CPU Port [sai_object_id_t] */
    SAI_SWITCH_ATTR_CPU_PORT,

    /** Max number of virtual routers supported [sai_uint32_t] */
    SAI_SWITCH_ATTR_MAX_VIRTUAL_ROUTERS,

    /** The size of the FDB Table in bytes [sai_uint32_t] */
    SAI_SWITCH_ATTR_FDB_TABLE_SIZE,

    /** The L3 Host Table size [sai_uint32_t] */
    SAI_SWITCH_ATTR_L3_NEIGHBOR_TABLE_SIZE,

    /** The L3 Route Table size [sai_uint32_t] */
    SAI_SWITCH_ATTR_L3_ROUTE_TABLE_SIZE,

    /** Number of ports that can be part of a LAG [sai_uint32_t] */
    SAI_SWITCH_ATTR_LAG_MEMBERS,

    /** Number of LAGs that can be created [sai_uint32_t] */
    SAI_SWITCH_ATTR_NUMBER_OF_LAGS,

    /** ECMP number of members per group [sai_uint32_t] (default is 64) */
    SAI_SWITCH_ATTR_ECMP_MEMBERS,

    /** ECMP number of group [sai_uint32_t] */
    SAI_SWITCH_ATTR_NUMBER_OF_ECMP_GROUPS,

    /** The number of Unicast Queues per port [sai_uint32_t] */
    SAI_SWITCH_ATTR_NUMBER_OF_UNICAST_QUEUES,

    /** The number of Multicast Queues per port [sai_uint32_t] */
    SAI_SWITCH_ATTR_NUMBER_OF_MULTICAST_QUEUES,

    /** The total number of Queues per port [sai_uint32_t] */
    SAI_SWITCH_ATTR_NUMBER_OF_QUEUES,

    /** The number of CPU Queues [sai_uint32_t] */
    SAI_SWITCH_ATTR_NUMBER_OF_CPU_QUEUES,

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

    /** ACL user-based trap id range [sai_u32_range_t] */
    SAI_SWITCH_ATTR_ACL_USER_TRAP_ID_RANGE,

    /** Default SAI STP instance ID [sai_object_id_t] */
    SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID,

    /* Default SAI Virtual Router ID [sai_object_id_t]
     * Must return SAI_STATUS_OBJECT_IN_USE when try to delete this VR ID. */
    SAI_SWITCH_ATTR_DEFAULT_VIRTUAL_ROUTER_ID,

    /** Maximum traffic classes limit [sai_uint8_t] */
    SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_TRAFFIC_CLASSES,

    /** HQOS - Maximum Number of Hierarchy scheduler
     *  group levels(depth) supported [sai_uint32_t]*/
    SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUP_HIERARCHY_LEVELS,

    /** HQOS - Maximum number of scheduler groups supported on
     * each Hierarchy level [sai_u32_list_t] */
    SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUPS_PER_HIERARCHY_LEVEL,

    /** HQOS - Maximum number of childs supported per scheudler group [sai_uint32_t]*/
    SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_CHILDS_PER_SCHEDULER_GROUP,

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

    /** The hash object for packets going through ECMP [sai_object_id_t]
     * (default value after switch initialization
     *   SAI_HASH_NATIVE_FIELD_LIST = [SAI_NATIVE_HASH_FIELD_SRC_MAC,
     *   SAI_NATIVE_HASH_FIELD_DST_MAC, SAI_NATIVE_HASH_FIELD_IN_PORT,
     *   SAI_NATIVE_HASH_FIELD_ETHERTYPE]
     *   SAI_HASH_UDF_GROUP_LIST empty list)
     * The object id is read only, while the object attributes can be modified */
    SAI_SWITCH_ATTR_ECMP_HASH,

    /** The hash object for packets going through LAG [sai_object_id_t]
     * (default value after switch initialization
     *   SAI_HASH_NATIVE_FIELD_LIST = [SAI_NATIVE_HASH_FIELD_SRC_MAC,
     *   SAI_NATIVE_HASH_FIELD_DST_MAC, SAI_NATIVE_HASH_FIELD_IN_PORT,
     *   SAI_NATIVE_HASH_FIELD_ETHERTYPE]
     *   SAI_HASH_UDF_GROUP_LIST empty list)
     * The object id is read only, while the object attributes can be modified */
    SAI_SWITCH_ATTR_LAG_HASH,

    /** Type of restart supported [sai_switch_restart_type_t] */
    SAI_SWITCH_ATTR_RESTART_TYPE,

    /** Minimum interval of time required by SAI for planned restart [sai_uint32_t]
     *  in milliseconds. Will be 0 for SAI_RESTART_TYPE_NONE.
     *  The Host Adapter will have to wait for this minimum interval of time before it decides
     *  to bring down SAI due to init failure. */
    SAI_SWITCH_ATTR_MIN_PLANNED_RESTART_INTERVAL,

    /** Nonvolatile storage required by both SAI and NPU in KB [sai_uint64_t]
     * Will be 0 for SAI_RESTART_TYPE_NONE */
    SAI_SWITCH_ATTR_NV_STORAGE_SIZE,

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

    /** Maximum number of learned MAC addresses [sai_uint32_t]
     * zero means learning limit disable. (default to zero) */
    SAI_SWITCH_ATTR_MAX_LEARNED_ADDRESSES,

    /** Dynamic FDB entry aging time in seconds [sai_uint32_t]
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

    /** SAI ECMP default hash algorithm [sai_hash_algorithm] (default to SAI_HASH_ALGORITHM_CRC) */
    SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM,

    /** SAI ECMP default hash seed [sai_uint32_t] (default to 0) */
    SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_SEED,

    /** SAI ECMP default symmetric hash [bool] (default to false)
    *   When set, the hash calculation will result in the same value as when the source and
    *   destination addresses (L2 src/dst mac,L3 src/dst ip,L4 src/dst port) were swapped,
    *   ensuring the same conversation will result in the same hash value.
    */
    SAI_SWITCH_ATTR_ECMP_DEFAULT_SYMMETRIC_HASH,

    /** The hash object for IPv4 packets going through ECMP [sai_object_id_t] */
    SAI_SWITCH_ATTR_ECMP_HASH_IPV4,

    /** The hash object for IPv4 in IPv4 packets going through ECMP [sai_object_id_t] */
    SAI_SWITCH_ATTR_ECMP_HASH_IPV4_IN_IPV4,

    /** SAI LAG default hash algorithm [sai_hash_algorithm] (default to SAI_HASH_ALGORITHM_CRC) */
    SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_ALGORITHM,

    /** SAI LAG default hash seed [sai_uint32_t] (default to 0) */
    SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_SEED,

    /** SAI LAG default symmetric hash [bool] (default to false)
    *   When set, the hash calculation will result in the same value as when the source and
    *   destination addresses (L2 src/dst mac,L3 src/dst ip,L4 src/dst port) were swapped,
    *   ensuring the same conversation will result in the same hash value.
    */
    SAI_SWITCH_ATTR_LAG_DEFAULT_SYMMETRIC_HASH,

    /** The hash object for IPv4 packets going through LAG [sai_object_id_t] */
    SAI_SWITCH_ATTR_LAG_HASH_IPV4,

    /** The hash object for IPv4 in IPv4 packets going through LAG [sai_object_id_t] */
    SAI_SWITCH_ATTR_LAG_HASH_IPV4_IN_IPV4,

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
     * [sai_uint32_t]
     */
    SAI_SWITCH_ATTR_COUNTER_REFRESH_INTERVAL,

    /** Default Traffic class value, Default TC = 0 */
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

    /** Enable TC -> Queue MAP [sai_object_id_t] on switch
     * Map id = SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map i.e All packets to queue 0.
     */
    SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP,

    /** Enable TC + COLOR -> DOT1P MAP [sai_object_id_t]
       Map id = SAI_NULL_OBJECT_ID to disable map on switch.
       Default no map */
    SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP,

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
 * @def SAI_SWITCH_ATTR_MAX_KEY_STRING_LEN
 * Maximum length of switch attribute key string that can be set using key=value
 */
#define SAI_SWITCH_ATTR_MAX_KEY_STRING_LEN    64

/**
 * @def SAI_SWITCH_ATTR_MAX_KEY_COUNT
 * Maximum count of switch attribute keys
 * @note This value needs to be incremented whenever a new switch attribute key
 * is added.
 */
#define SAI_SWITCH_ATTR_MAX_KEY_COUNT         15

/**
 * List of switch attributes keys that can be set using key=value
 */
#define SAI_KEY_FDB_TABLE_SIZE                "SAI_FDB_TABLE_SIZE"
#define SAI_KEY_L3_ROUTE_TABLE_SIZE           "SAI_L3_ROUTE_TABLE_SIZE"
#define SAI_KEY_L3_NEIGHBOR_TABLE_SIZE        "SAI_L3_NEIGHBOR_TABLE_SIZE"
#define SAI_KEY_NUM_LAG_MEMBERS               "SAI_NUM_LAG_MEMBERS"
#define SAI_KEY_NUM_LAGS                      "SAI_NUM_LAGS"
#define SAI_KEY_NUM_ECMP_MEMBERS              "SAI_NUM_ECMP_MEMBERS"
#define SAI_KEY_NUM_ECMP_GROUPS               "SAI_NUM_ECMP_GROUPS"
#define SAI_KEY_NUM_UNICAST_QUEUES            "SAI_NUM_UNICAST_QUEUES"
#define SAI_KEY_NUM_MULTICAST_QUEUES          "SAI_NUM_MULTICAST_QUEUES"
#define SAI_KEY_NUM_QUEUES                    "SAI_NUM_QUEUES"
#define SAI_KEY_NUM_CPU_QUEUES                "SAI_NUM_CPU_QUEUES"
#define SAI_KEY_INIT_CONFIG_FILE              "SAI_INIT_CONFIG_FILE"
/** 0: means cold boot, and 1: means warm boot */
#define SAI_KEY_WARM_BOOT                     "SAI_WARM_BOOT"
/** The file to recover SAI/NPU state from */
#define SAI_KEY_WARM_BOOT_READ_FILE           "SAI_WARM_BOOT_READ_FILE"
/** The file to write SAI/NPU state to */
#define SAI_KEY_WARM_BOOT_WRITE_FILE          "SAI_WARM_BOOT_WRITE_FILE"


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
    _In_ sai_uint32_t attr_count,
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

