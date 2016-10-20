/**
 * Copyright (c) 2014 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
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
 * @file    saiswitch.h
 *
 * @brief   This module defines SAI Switch interface
 */

#if !defined (__SAISWITCH_H_)
#define __SAISWITCH_H_

#include <saitypes.h>
#include <saiport.h>
#include <saifdb.h>
#include <saihostintf.h>

/**
 * @defgroup SAISWITCH SAI - Switch specific API definitions
 *
 * @{
 */

/**
 * @brief Maximum Hardware ID Lenght
 */
#define SAI_MAX_HARDWARE_ID_LEN                 255

/**
 * @brief Maximum Firmware Path Name Length
 */
#define SAI_MAX_FIRMWARE_PATH_NAME_LEN          PATH_MAX

/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_OPER_STATUS
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
 * @brief Attribute data for packet action
 */
typedef enum _sai_packet_action_t
{
    /* Basic Packet Actions */

    /*
     * These could be further classified based on the nature of action
     * - Data Plane Packet Actions
     * - CPU Path Packet Actions
     */

    /*
     * Data Plane Packet Actions
     * Following two packet actions only affect the packet action on the data plane.
     * Packet action on the CPU path remains unchanged.
     */

    /** Drop Packet in data plane */
    SAI_PACKET_ACTION_DROP,

    /** Forward Packet in data plane. */
    SAI_PACKET_ACTION_FORWARD,

    /*
     * CPU Path Packet Actions
     * Following two packet actions only affect the packet action on the CPU path.
     * Packet action on the data plane remains unchanged.
     */

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
 * @brief Attribute data for number of vlan tags present in a packet
 */
typedef enum _sai_packet_vlan_t
{
    /**
     * @brief Untagged
     *
     * Packet without vlan tags
     */
    SAI_PACKET_VLAN_UNTAG,

    /**
     * @brief Single Outer Tag
     *
     * Packet outer TPID matches to the ingress port outer TPID and
     * Packet inner TPID if present, does not matches the configured inner TPID
     */
    SAI_PACKET_VLAN_SINGLE_OUTER_TAG,

    /**
     * @brief Double Tag
     *
     * Packet outer TPID matches to the ingress port outer TPID and
     * Packet inner TPID matches to the configured inner TPID
     */
    SAI_PACKET_VLAN_DOUBLE_TAG

} sai_packet_vlan_t;

/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_SWITCHING_MODE
 */
typedef enum _sai_switch_switching_mode_t
{
    /** cut-through switching mode */
    SAI_SWITCH_SWITCHING_MODE_CUT_THROUGH,

    /** store-and-forward switching mode */
    SAI_SWITCH_SWITCHING_MODE_STORE_AND_FORWARD

} sai_switch_switching_mode_t;

/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM
 * and #SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_ALGORITHM
 */
typedef enum _sai_hash_algorithm_t
{
    /** CRC-based hash algorithm */
    SAI_HASH_ALGORITHM_CRC = 0,

    /** XOR-based hash algorithm */
    SAI_HASH_ALGORITHM_XOR = 1,

    /** Random-based hash algorithm */
    SAI_HASH_ALGORITHM_RANDOM = 2,

} sai_hash_algorithm_t;

/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_RESTART_TYPE
 */
typedef enum _sai_switch_restart_type_t
{
    /** NPU doesn't support warmboot */
    SAI_SWITCH_RESTART_TYPE_NONE = 0,

    /** Planned restart only */
    SAI_SWITCH_RESTART_TYPE_PLANNED = 1,

    /** Both planned and unplanned restart */
    SAI_SWITCH_RESTART_TYPE_ANY = 2,

} sai_switch_restart_type_t;

/**
 * @brief Attribute Id in sai_set_switch_attribute() and
 * sai_get_switch_attribute() calls
 */
typedef enum _sai_switch_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SWITCH_ATTR_START,

    /**
     * @brief The number of ports on the switch
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_PORT_NUMBER = SAI_SWITCH_ATTR_START,

    /**
     * @brief Get the port list
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_PORT
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_PORT_LIST,

    /**
     * @brief Get the Max MTU in bytes, supported by the switch
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_PORT_MAX_MTU,

    /**
     * @brief Get the CPU Port
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_PORT
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_CPU_PORT,

    /**
     * @brief Max number of virtual routers supported
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_VIRTUAL_ROUTERS,

    /**
     * @brief The size of the FDB Table in bytes
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_FDB_TABLE_SIZE,

    /**
     * @brief The L3 Host Table size
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_L3_NEIGHBOR_TABLE_SIZE,

    /**
     * @brief The L3 Route Table size
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_L3_ROUTE_TABLE_SIZE,

    /**
     * @brief Number of ports that can be part of a LAG
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_LAG_MEMBERS,

    /**
     * @brief Number of LAGs that can be created
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_LAGS,

    /**
     * @brief ECMP number of members per group
     *
     * Default is 64
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ECMP_MEMBERS,

    /**
     * @brief ECMP number of group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_ECMP_GROUPS,

    /**
     * @brief The number of Unicast Queues per port
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_UNICAST_QUEUES,

    /**
     * @brief The number of Multicast Queues per port
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_MULTICAST_QUEUES,

    /**
     * @brief The total number of Queues per port
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_QUEUES,

    /**
     * @brief The number of CPU Queues
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_CPU_QUEUES,

    /**
     * @brief Local subnet routing supported.
     * Routes with next hop set to "on-link".
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ON_LINK_ROUTE_SUPPORTED,

    /**
     * @brief Oper state
     *
     * @type sai_switch_oper_status_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_OPER_STATUS,

    /**
     * @brief The current value of the maximum temperature
     * retrieved from the switch sensors, in Celsius
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_TEMP,

    /**
     * @brief Minimum priority for ACL table
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY,

    /**
     * @brief Maximum priority for ACL table
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_TABLE_MAXIMUM_PRIORITY,

    /**
     * @brief Minimum priority for ACL entry
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY,

    /**
     * @brief Maximum priority for ACL entry
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_ENTRY_MAXIMUM_PRIORITY,

    /**
     * @brief Minimum priority for ACL table group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_TABLE_GROUP_MINIMUM_PRIORITY,

    /**
     * @brief Maximum priority for ACL table group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_TABLE_GROUP_MAXIMUM_PRIORITY,

    /**
     * @brief FDB DST user-based meta data range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_FDB_DST_USER_META_DATA_RANGE,

    /**
     * @brief Route DST Table user-based meta data range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ROUTE_DST_USER_META_DATA_RANGE,

    /**
     * @brief Neighbor DST Table user-based meta data range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NEIGHBOR_DST_USER_META_DATA_RANGE,

    /**
     * @brief Port user-based meta data range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_PORT_USER_META_DATA_RANGE,

    /**
     * @brief VLAN user-based meta data range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_VLAN_USER_META_DATA_RANGE,

    /**
     * @brief ACL user-based ACL meta data range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_USER_META_DATA_RANGE,

    /**
     * @brief ACL user-based trap id range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_USER_TRAP_ID_RANGE,

    /**
     * @brief Default SAI STP instance ID
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_STP
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID,

    /**
     * @brief Default SAI Virtual Router ID
     *
     * Must return #SAI_STATUS_OBJECT_IN_USE when try to delete this VR ID.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_DEFAULT_VIRTUAL_ROUTER_ID,

    /** 
     * @brief Switch/Global bind point for ingress ACL objects
     *
     * Bind (or unbind) an ingress acl tables/groups globally. Enable/Update 
     * ingress ACL table(s)/group(s) filtering by assigning the list of valid 
     * objects. Disable ingress filtering by assigning SAI_NULL_OBJECT_ID 
     * in the attribute value. 
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_ACL_TABLE and/or SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @flags CREATE_AND_SET
     * @default empty
     */
 
    SAI_SWITCH_ATTR_DEFAULT_INGRESS_ACL_LIST,

    /** 
     * @brief Switch/Global bind point for egress ACL objects
     *
     * Bind (or unbind) an egress acl tables/groups globally. Enable/Update 
     * egress ACL table(s)/group(s) filtering by assigning the list of valid 
     * objects. Disable egress filtering by assigning SAI_NULL_OBJECT_ID 
     * in the attribute value. 
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_ACL_TABLE and/or SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SWITCH_ATTR_DEFAULT_EGRESS_ACL_LIST,

    /** 
     * @brief ACL object support for non-conflicting action resolution
     *
     * Must return bool (true) if supported or (false) if not supported
     *
     * @type bool
     * @flags READ_ONLY
     *
     */
    SAI_ACL_NON_CONFLICTING_ACTION,

    /** Maximum traffic classes limit [sai_uint8_t] */
    /**
     * @brief Maximum traffic classes limit
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_TRAFFIC_CLASSES,

    /**
     * @brief HQOS - Maximum Number of Hierarchy scheduler
     * group levels(depth) supported
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUP_HIERARCHY_LEVELS,

    /**
     * @brief HQOS - Maximum number of scheduler groups supported on
     * each Hierarchy level
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUPS_PER_HIERARCHY_LEVEL,

    /**
     * @brief HQOS - Maximum number of childs supported per scheudler group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_CHILDS_PER_SCHEDULER_GROUP,

    /**
     * @brief Switch total buffer size in KB
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_TOTAL_BUFFER_SIZE,

    /**
     * Switch number of ingress buffer pool
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_INGRESS_BUFFER_POOL_NUM,

    /**
     * @brief Switch number of egress buffer pool
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_EGRESS_BUFFER_POOL_NUM,

    /**
     * @brief Default trap group
     *
     * Default value after switch initialization
     *
     * #SAI_HOSTIF_TRAP_GROUP_ATTR_ADMIN_STATE = true
     * SAI_HOSTIF_TRAP_GROUP_ATTR_PRIO = #SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY
     * #SAI_HOSTIF_TRAP_GROUP_ATTR_QUEUE = 0
     * #SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER = #SAI_NULL_OBJECT_ID
     *
     * The group handle is read only, while the group attributes, such as queue and policer,
     * may be modified
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP,

    /**
     * @brief The hash object for packets going through ECMP
     *
     * Default value after switch initialization
     *
     * #SAI_HASH_ATTR_NATIVE_FIELD_LIST = \[#SAI_NATIVE_HASH_FIELD_SRC_MAC,
     * #SAI_NATIVE_HASH_FIELD_DST_MAC, #SAI_NATIVE_HASH_FIELD_IN_PORT,
     * #SAI_NATIVE_HASH_FIELD_ETHERTYPE\]
     * #SAI_HASH_ATTR_UDF_GROUP_LIST empty list
     *
     * The object id is read only, while the object attributes can be modified
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_HASH
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ECMP_HASH,

    /**
     * @brief The hash object for packets going through LAG
     *
     * Default value after switch initialization
     *
     * #SAI_HASH_ATTR_NATIVE_FIELD_LIST = \[#SAI_NATIVE_HASH_FIELD_SRC_MAC,
     * #SAI_NATIVE_HASH_FIELD_DST_MAC, #SAI_NATIVE_HASH_FIELD_IN_PORT,
     * #SAI_NATIVE_HASH_FIELD_ETHERTYPE\]
     * #SAI_HASH_ATTR_UDF_GROUP_LIST empty list)
     *
     * The object id is read only, while the object attributes can be modified
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_HASH
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_LAG_HASH,

    /**
     * @brief Type of restart supported
     *
     * @type sai_switch_restart_type_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_RESTART_TYPE,

    /**
     * @brief Minimum interval of time required by SAI for planned restart in milliseconds.
     *
     * Will be 0 for #SAI_SWITCH_RESTART_TYPE_NONE. The Host Adapter will have to
     * wait for this minimum interval of time before it decides to bring down
     * SAI due to init failure.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MIN_PLANNED_RESTART_INTERVAL,

    /**
     * @brief Nonvolatile storage required by both SAI and NPU in KB
     *
     * Will be 0 for #SAI_SWITCH_RESTART_TYPE_NONE
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NV_STORAGE_SIZE,

    /**
     * @brief Count of the total number of actions supported by NPU
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_ACL_ACTION_COUNT,

    /**
     * @brief Acl capabilities supported by the NPU
     *
     * @type sai_acl_capability_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_CAPABILITY,

    /* READ-WRITE */

    /**
     * @brief Switching mode
     *
     * @type sai_switch_switching_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_SWITCH_SWITCHING_MODE_STORE_AND_FORWARD
     */
    SAI_SWITCH_ATTR_SWITCHING_MODE,

    /**
     * @brief L2 broadcast flood control to CPU port
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_BCAST_CPU_FLOOD_ENABLE,

    /**
     * @brief L2 multicast flood control to CPU port
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_MCAST_CPU_FLOOD_ENABLE,

    /**
     * @brief Default switch MAC Address
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default vendor
     */
    SAI_SWITCH_ATTR_SRC_MAC_ADDRESS,

    /**
     * @brief Maximum number of learned MAC addresses
     *
     * Zero means learning limit disable.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_MAX_LEARNED_ADDRESSES,

    /**
     * @brief Dynamic FDB entry aging time in seconds
     *
     * Zero means aging is disabled.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_FDB_AGING_TIME,

    /**
     * @brief Flood control for packets with unknown destination address.
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_SWITCH_ATTR_FDB_UNICAST_MISS_ACTION,

    /**
     * @brief Broadcast miss action
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_TRAP
     */
    SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_ACTION,

    /**
     * @brief Multicast miss action
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_TRAP
     */
    SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_ACTION,

    /**
     * @brief SAI ECMP default hash algorithm
     *
     * @type sai_hash_algorithm_t
     * @flags CREATE_AND_SET
     * @default SAI_HASH_ALGORITHM_CRC
     */
    SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM,

    /**
     * @brief SAI ECMP default hash seed
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_SEED,

    /**
     * @brief SAI ECMP default symmetric hash
     *
     * When set, the hash calculation will result in the same value as when the
     * source and destination addresses (L2 src/dst mac,L3 src/dst ip,L4
     * src/dst port) were swapped, ensuring the same conversation will result
     * in the same hash value.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_ECMP_DEFAULT_SYMMETRIC_HASH,

    /**
     * @brief The hash object for IPv4 packets going through ECMP
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_HASH
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ECMP_HASH_IPV4,

    /**
     * @brief The hash object for IPv4 in IPv4 packets going through ECMP
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_HASH
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ECMP_HASH_IPV4_IN_IPV4,

    /**
     * @brief The hash object for IPv6 packets going through ECMP
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_HASH
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ECMP_HASH_IPV6,

    /**
     * @brief SAI LAG default hash algorithm
     *
     * @type sai_hash_algorithm_t
     * @flags CREATE_AND_SET
     * @default SAI_HASH_ALGORITHM_CRC
     */
    SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_ALGORITHM,

    /**
     * @brief SAI LAG default hash seed
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_SEED,

    /**
     * @brief SAI LAG default symmetric hash
     *
     * When set, the hash calculation will result in the same value as when the source and
     * destination addresses (L2 src/dst mac,L3 src/dst ip,L4 src/dst port) were swapped,
     * ensuring the same conversation will result in the same hash value.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_LAG_DEFAULT_SYMMETRIC_HASH,

    /**
     * @brief The hash object for IPv4 packets going through LAG
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_HASH
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_LAG_HASH_IPV4,

    /** @brief The hash object for IPv4 in IPv4 packets going through LAG
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_HASH
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_LAG_HASH_IPV4_IN_IPV4,

    /**
     * @brief The hash object for IPv6 packets going through LAG
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_HASH
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_LAG_HASH_IPV6,

    /**
     * @brief Refresh interval
     *
     * @par The SDK can
     *
     * 1 - Read the counters directly from HW (or)
     * 2 - Cache the counters in SW. Caching is typically done if
     * retrieval of counters directly from HW for each counter
     * read is CPU intensive
     *
     * This setting can be used to
     *
     * 1 - Move from HW based to SW based or Vice versa
     * 2 - Configure the SW counter cache refresh rate
     *
     * Setting a value of 0 enables direct HW based counter read. A
     * non zero value enables the SW cache based and the counter
     * refresh rate.
     *
     * A NPU may support both or one of the option. It would return
     * error for unsupported options
     *
     * Default - 1 sec (SW counter cache)
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1
     */
    SAI_SWITCH_ATTR_COUNTER_REFRESH_INTERVAL,

    /**
     * @brief Default Traffic class value
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_QOS_DEFAULT_TC,

    /**
     * @brief Enable DOT1P -> TC MAP on switch.
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust Dot1p, Map ID should be add/remove on switch.
     * Default disabled
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     */
    SAI_SWITCH_ATTR_QOS_DOT1P_TO_TC_MAP,

    /**
     * @brief Enable DOT1P -> COLOR MAP on switch.
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust Dot1p, Map ID should be add/remove on switch.
     * Default disabled
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     */
    SAI_SWITCH_ATTR_QOS_DOT1P_TO_COLOR_MAP,

    /**
     * @brief Enable DSCP -> TC MAP on switch.
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust DSCP, Map ID should be add/remove on port.
     * Default no map
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     */
    SAI_SWITCH_ATTR_QOS_DSCP_TO_TC_MAP,

    /**
     * @brief Enable DSCP -> COLOR MAP on switch
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust DSCP, Map ID should be add/remove on switch.
     * Default no map
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     */
    SAI_SWITCH_ATTR_QOS_DSCP_TO_COLOR_MAP,

    /**
     * @brief Enable TC -> Queue MAP on switch
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map i.e All packets to queue 0.
     *
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     */
    SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP,

    /**
     * @brief Enable TC + COLOR -> DOT1P MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     */
    SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP,

    /**
     * @brief Enable TC + COLOR -> DSCP MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     */
    SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_DSCP_MAP,

    /**
     * @brief Enable vendor specific switch shell
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_SWITCH_SHELL_ENABLE,

    /**
    * @brief Enable SAI function call fast mode, which executes calls very quickly
    *
    * @type bool
    * @flags CREATE_AND_SET
    * @default false
    */
    SAI_SWITCH_ATTR_FAST_API_ENABLE,

    /**
     * @brief End of attributes
     */
    SAI_SWITCH_ATTR_END,

    /** Custom range base value */
    SAI_SWITCH_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SWITCH_ATTR_CUSTOM_RANGE_END

} sai_switch_attr_t;

/**
 * @def SAI_SWITCH_ATTR_MAX_KEY_STRING_LEN
 * Maximum length of switch attribute key string that can be set using key=value
 */
#define SAI_SWITCH_ATTR_MAX_KEY_STRING_LEN    64

/**
 * @def SAI_SWITCH_ATTR_MAX_KEY_COUNT
 * Maximum count of switch attribute keys
 *
 * @note This value needs to be incremented whenever a new switch attribute key
 * is added.
 */
#define SAI_SWITCH_ATTR_MAX_KEY_COUNT         16

/*
 * List of switch attributes keys that can be set using key=value
 */

/**
 * @def SAI_KEY_FDB_TABLE_SIZE
 */
#define SAI_KEY_FDB_TABLE_SIZE                    "SAI_FDB_TABLE_SIZE"

/**
 * @def SAI_KEY_L3_ROUTE_TABLE_SIZE
 */
#define SAI_KEY_L3_ROUTE_TABLE_SIZE               "SAI_L3_ROUTE_TABLE_SIZE"

/**
 * @def SAI_KEY_L3_NEIGHBOR_TABLE_SIZE
 */
#define SAI_KEY_L3_NEIGHBOR_TABLE_SIZE            "SAI_L3_NEIGHBOR_TABLE_SIZE"

/**
 * @def SAI_KEY_NUM_LAG_MEMBERS
 */
#define SAI_KEY_NUM_LAG_MEMBERS                   "SAI_NUM_LAG_MEMBERS"

/**
 * @def SAI_KEY_NUM_LAGS
 */
#define SAI_KEY_NUM_LAGS                          "SAI_NUM_LAGS"

/**
 * @def SAI_KEY_NUM_ECMP_MEMBERS
 */
#define SAI_KEY_NUM_ECMP_MEMBERS                  "SAI_NUM_ECMP_MEMBERS"

/**
 * @def SAI_KEY_NUM_ECMP_GROUPS
 */
#define SAI_KEY_NUM_ECMP_GROUPS                   "SAI_NUM_ECMP_GROUPS"

/**
 * @def SAI_KEY_NUM_UNICAST_QUEUES
 */
#define SAI_KEY_NUM_UNICAST_QUEUES                "SAI_NUM_UNICAST_QUEUES"

/**
 * @def SAI_KEY_NUM_MULTICAST_QUEUES
 */
#define SAI_KEY_NUM_MULTICAST_QUEUES              "SAI_NUM_MULTICAST_QUEUES"

/**
 * @def SAI_KEY_NUM_QUEUES
 */
#define SAI_KEY_NUM_QUEUES                        "SAI_NUM_QUEUES"

/**
 * @def SAI_KEY_NUM_CPU_QUEUES
 */
#define SAI_KEY_NUM_CPU_QUEUES                    "SAI_NUM_CPU_QUEUES"

/**
 * @def SAI_KEY_INIT_CONFIG_FILE
 */
#define SAI_KEY_INIT_CONFIG_FILE                  "SAI_INIT_CONFIG_FILE"

/**
 * @def SAI_KEY_BOOT_TYPE
 *
 * 0: cold boot. Initialize NPU and external phys.
 * 1: warm boot. Do not re-initialize NPU or external phys, reconstruct SAI/SDK state from stored state.
 * 2: fast boot. Only initilize NPU. SAI/SDK state should not be persisted except for those related
 *                to physical port attributes such as SPEED, AUTONEG mode, admin state, oper status.
 */
#define SAI_KEY_BOOT_TYPE                         "SAI_BOOT_TYPE"

/**
 * @def SAI_KEY_WARM_BOOT_READ_FILE
 * The file to recover SAI/NPU state from
 */
#define SAI_KEY_WARM_BOOT_READ_FILE               "SAI_WARM_BOOT_READ_FILE"

/**
 * @def SAI_KEY_WARM_BOOT_WRITE_FILE
 * The file to write SAI/NPU state to
 */
#define SAI_KEY_WARM_BOOT_WRITE_FILE              "SAI_WARM_BOOT_WRITE_FILE"

/**
 * @def SAI_KEY_HW_PORT_PROFILE_ID_CONFIG_FILE
 * Vendor specific Configuration file for Hardware Port Profile ID parameters.
 * HW port profile ID can be used to set vendor specific port attributes based on
 * the tranceiver type plugged in to the port
 */
#define SAI_KEY_HW_PORT_PROFILE_ID_CONFIG_FILE    "SAI_HW_PORT_PROFILE_ID_CONFIG_FILE"

/**
 * @brief Switch shutdown request callback.
 *
 * Adapter DLL may request a shutdown due to an unrecoverable failure
 * or a maintenance operation
 */
typedef void (*sai_switch_shutdown_request_fn)(void);

/**
 * @brief Switch oper state change notification
 *
 * @param[in] switch_oper_status New switch oper state
 */
typedef void (*sai_switch_state_change_notification_fn)(
        _In_ sai_switch_oper_status_t switch_oper_status);

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
 * @brief SDK initialization. After the call the capability attributes should
 * be ready for retrieval via sai_get_switch_attribute().
 *
 * @param[in] profile_id Handle for the switch profile.
 * @param[in] switch_hardware_id Switch hardware ID to open
 * @param[in] firmware_path_name Vendor specific path name of the firmware to load
 * @param[in] switch_notifications Switch notification table
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_initialize_switch_fn)(
        _In_ sai_switch_profile_id_t profile_id,
        _In_reads_z_(SAI_MAX_HARDWARE_ID_LEN) char *switch_hardware_id,
        _In_reads_opt_z_(SAI_MAX_FIRMWARE_PATH_NAME_LEN) char *firmware_path_name,
        _In_ sai_switch_notification_t *switch_notifications);

/**
 * @brief Release all resources associated with currently opened switch
 *
 * @param[in] warm_restart_hint Hint that indicates controlled warm restart.
 * Since warm restart can be caused by crash (therefore there are no guarantees
 * for this call), this hint is really a performance optimization.
 */
typedef void (*sai_shutdown_switch_fn)(
        _In_ bool warm_restart_hint);

/**
 * @brief SDK connect. This API connects library to the initialized SDK. After
 * the call the capability attributes should be ready for retrieval via
 * sai_get_switch_attribute().
 *
 * @param[in] profile_id Handle for the switch profile.
 * @param[in] switch_hardware_id Switch hardware ID to open
 * @param[in] switch_notifications Switch notification table
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_connect_switch_fn)(
        _In_ sai_switch_profile_id_t profile_id,
        _In_reads_z_(SAI_MAX_HARDWARE_ID_LEN) char *switch_hardware_id,
        _In_ sai_switch_notification_t *switch_notifications);

/**
 * @brief Disconnect this SAI library from the SDK.
 */
typedef void (*sai_disconnect_switch_fn)(void);

/**
 * @brief Set switch attribute value
 *
 * @param[in] attr Switch attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_switch_attribute_fn)(
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get switch attribute value
 *
 * @param[in] attr_count Number of switch attributes
 * @param[inout] attr_list Array of switch attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_switch_attribute_fn)(
        _In_ sai_uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

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
 * @}
 */
#endif /** __SAISWITCH_H_ */
