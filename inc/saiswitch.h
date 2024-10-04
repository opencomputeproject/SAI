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
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
 *
 * @file    saiswitch.h
 *
 * @brief   This module defines SAI Switch interface
 */

#if !defined (__SAISWITCH_H_)
#define __SAISWITCH_H_

#include <saitypes.h>

/**
 * @defgroup SAISWITCH SAI - Switch specific API definitions
 *
 * @{
 */

/**
 * @brief Maximum Hardware ID Length
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
     * Data Plane Packet Actions.
     *
     * Following two packet actions only affect the packet action on the data plane.
     * Packet action on the CPU path remains unchanged.
     */

    /** Drop Packet in data plane */
    SAI_PACKET_ACTION_DROP,

    /** Forward Packet in data plane. */
    SAI_PACKET_ACTION_FORWARD,

    /*
     * CPU Path Packet Actions.
     *
     * Following two packet actions only affect the packet action on the CPU path.
     * Packet action on the data plane remains unchanged.
     */

    /**
     * @brief Packet action copy
     *
     * Copy Packet to CPU without interfering the original packet action in the
     * pipeline.
     */
    SAI_PACKET_ACTION_COPY,

    /** Cancel copy the packet to CPU. */
    SAI_PACKET_ACTION_COPY_CANCEL,

    /** Combination of Packet Actions */

    /**
     * @brief Packet action trap
     *
     * This is a combination of SAI packet action COPY and DROP:
     * A copy of the original packet is sent to CPU port, the original
     * packet is forcefully dropped from the pipeline.
     */
    SAI_PACKET_ACTION_TRAP,

    /**
     * @brief Packet action log
     *
     * This is a combination of SAI packet action COPY and FORWARD:
     * A copy of the original packet is sent to CPU port, the original
     * packet, if it was to be dropped in the original pipeline,
     * change the pipeline action to forward (cancel drop).
     */
    SAI_PACKET_ACTION_LOG,

    /** This is a combination of SAI packet action COPY_CANCEL and DROP */
    SAI_PACKET_ACTION_DENY,

    /** This is a combination of SAI packet action COPY_CANCEL and FORWARD */
    SAI_PACKET_ACTION_TRANSIT,

    /** Do not drop the packet. */
    SAI_PACKET_ACTION_DONOTDROP

} sai_packet_action_t;

/**
 * @brief Attribute data for number of vlan tags present in a packet
 */
typedef enum _sai_packet_vlan_t
{
    /**
     * @brief Untagged
     *
     * Packet without vlan tags.
     */
    SAI_PACKET_VLAN_UNTAG,

    /**
     * @brief Single Outer Tag
     *
     * Packet outer TPID matches to the ingress port outer TPID and
     * packet inner TPID if present, does not match the configured inner TPID.
     */
    SAI_PACKET_VLAN_SINGLE_OUTER_TAG,

    /**
     * @brief Double Tag
     *
     * Packet outer TPID matches to the ingress port outer TPID and
     * packet inner TPID matches to the configured inner TPID.
     */
    SAI_PACKET_VLAN_DOUBLE_TAG

} sai_packet_vlan_t;

/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_SWITCHING_MODE
 */
typedef enum _sai_switch_switching_mode_t
{
    /** Cut-through switching mode */
    SAI_SWITCH_SWITCHING_MODE_CUT_THROUGH,

    /** Store-and-forward switching mode */
    SAI_SWITCH_SWITCHING_MODE_STORE_AND_FORWARD

} sai_switch_switching_mode_t;

/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM
 * and #SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_ALGORITHM
 */
typedef enum _sai_hash_algorithm_t
{
    /** CRC based hash algorithm */
    SAI_HASH_ALGORITHM_CRC = 0,

    /** XOR-based hash algorithm */
    SAI_HASH_ALGORITHM_XOR = 1,

    /** Random-based hash algorithm */
    SAI_HASH_ALGORITHM_RANDOM = 2,

    /** Lower 16-bits of CRC32 based hash algorithm */
    SAI_HASH_ALGORITHM_CRC_32LO = 3,

    /** Higher 16-bits of CRC32-based hash algorithm */
    SAI_HASH_ALGORITHM_CRC_32HI = 4,

    /** CRC using CCITT polynomial based hash algorithm */
    SAI_HASH_ALGORITHM_CRC_CCITT = 5,

    /** Combination of CRC and XOR based hash algorithm */
    SAI_HASH_ALGORITHM_CRC_XOR = 6,

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
 * @brief Attribute data for #SAI_SWITCH_ATTR_MCAST_SNOOPING_CAPABILITY
 */
typedef enum _sai_switch_mcast_snooping_capability_t
{
    /** NPU doesn't support IP based L2MC */
    SAI_SWITCH_MCAST_SNOOPING_CAPABILITY_NONE = 0,

    /** *G lookup only */
    SAI_SWITCH_MCAST_SNOOPING_CAPABILITY_XG = 1,

    /** SG lookup only */
    SAI_SWITCH_MCAST_SNOOPING_CAPABILITY_SG = 2,

    /** Both *G/SG lookup supported */
    SAI_SWITCH_MCAST_SNOOPING_CAPABILITY_XG_AND_SG = 3,

} sai_switch_mcast_snooping_capability_t;

/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_HARDWARE_ACCESS_BUS
 */
typedef enum _sai_switch_hardware_access_bus_t
{
    /** Hardware access bus is MDIO */
    SAI_SWITCH_HARDWARE_ACCESS_BUS_MDIO,

    /** Hardware access bus is I2C */
    SAI_SWITCH_HARDWARE_ACCESS_BUS_I2C,

    /** Hardware access bus is CPLD */
    SAI_SWITCH_HARDWARE_ACCESS_BUS_CPLD,

} sai_switch_hardware_access_bus_t;

/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_FIRMWARE_LOAD_METHOD
 */
typedef enum _sai_switch_firmware_load_method_t
{
    /** Do not download FW. Use already downloaded FW instead */
    SAI_SWITCH_FIRMWARE_LOAD_METHOD_NONE,

    /** Download FW internally via MDIO */
    SAI_SWITCH_FIRMWARE_LOAD_METHOD_INTERNAL,

    /** Load FW from EEPROM */
    SAI_SWITCH_FIRMWARE_LOAD_METHOD_EEPROM,

} sai_switch_firmware_load_method_t;

/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_FIRMWARE_LOAD_TYPE
 */
typedef enum _sai_switch_firmware_load_type_t
{
    /** Skip firmware download if firmware is already present */
    SAI_SWITCH_FIRMWARE_LOAD_TYPE_SKIP,

    /** Always download the firmware specified by firmware load method */
    SAI_SWITCH_FIRMWARE_LOAD_TYPE_FORCE,

    /** Check the firmware version. If it is different from current version download firmware */
    SAI_SWITCH_FIRMWARE_LOAD_TYPE_AUTO,

} sai_switch_firmware_load_type_t;

/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_TYPE
 */
typedef enum _sai_switch_type_t
{
    /** Switch type is Switching Network processing unit */
    SAI_SWITCH_TYPE_NPU,

    /** Switch type is PHY */
    SAI_SWITCH_TYPE_PHY,

    /** Switch type is VOQ based NPU */
    SAI_SWITCH_TYPE_VOQ,

    /** Switch type is Fabric switch device */
    SAI_SWITCH_TYPE_FABRIC,

    /** Switch type is POE (Power over Ethernet) */
    SAI_SWITCH_TYPE_POE,

} sai_switch_type_t;

/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_FAILOVER_CONFIG_MODE
 * Used for Failover configuration mode
 * In case of primary port failure, hitless enables the switch over to secondary
 * port without losing link. It allows uninterrupted data transmission.
 */
typedef enum _sai_switch_failover_config_mode_t
{
    /** Ports are configured but do not operate in hitless mode */
    SAI_SWITCH_FAILOVER_CONFIG_MODE_NO_HITLESS,

    /** Ports are configured and operate in hitless mode */
    SAI_SWITCH_FAILOVER_CONFIG_MODE_HITLESS

} sai_switch_failover_config_mode_t;

/**
 * @brief Defines tunnel type
 */
typedef enum _sai_tunnel_type_t
{
    SAI_TUNNEL_TYPE_IPINIP,

    SAI_TUNNEL_TYPE_IPINIP_GRE,

    SAI_TUNNEL_TYPE_VXLAN,

    SAI_TUNNEL_TYPE_MPLS,

    SAI_TUNNEL_TYPE_SRV6,

    SAI_TUNNEL_TYPE_NVGRE,

    SAI_TUNNEL_TYPE_IPINIP_ESP,

    SAI_TUNNEL_TYPE_IPINIP_UDP_ESP,

    SAI_TUNNEL_TYPE_VXLAN_UDP_ESP,

} sai_tunnel_type_t;

/**
 * @brief Defines VXLAN tunnel UDP source port mode
 */
typedef enum _sai_tunnel_vxlan_udp_sport_mode_t
{
    /**
     * @brief User define value
     */
    SAI_TUNNEL_VXLAN_UDP_SPORT_MODE_USER_DEFINED,

    /**
     * @brief RFC6335 Computed hash value in range 49152-65535
     */
    SAI_TUNNEL_VXLAN_UDP_SPORT_MODE_EPHEMERAL,
} sai_tunnel_vxlan_udp_sport_mode_t;

/**
 * @brief Defines tunnel encap ECN mode
 */
typedef enum _sai_tunnel_encap_ecn_mode_t
{
    /**
     * @brief Normal mode behavior defined in RFC 6040
     * section 4.1 copy from inner
     */
    SAI_TUNNEL_ENCAP_ECN_MODE_STANDARD,

    /**
     * @brief User defined behavior.
     */
    SAI_TUNNEL_ENCAP_ECN_MODE_USER_DEFINED

} sai_tunnel_encap_ecn_mode_t;

/**
 * @brief Defines tunnel decap ECN mode
 */
typedef enum _sai_tunnel_decap_ecn_mode_t
{
    /**
     * @brief Behavior defined in RFC 6040 section 4.2
     */
    SAI_TUNNEL_DECAP_ECN_MODE_STANDARD,

    /**
     * @brief Copy from outer ECN
     */
    SAI_TUNNEL_DECAP_ECN_MODE_COPY_FROM_OUTER,

    /**
     * @brief User defined behavior
     */
    SAI_TUNNEL_DECAP_ECN_MODE_USER_DEFINED

} sai_tunnel_decap_ecn_mode_t;

/**
 * @brief Defines tunnel attributes at switch level.
 * SAI_OBJECT_TYPE_SWITCH_TUNNEL object provides
 * per tunnel type global configuration.
 * SAI_OBJECT_TYPE_TUNNEL object configuration
 * overrides the switch scoped global configuration.
 */
typedef enum _sai_switch_tunnel_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SWITCH_TUNNEL_ATTR_START,

    /**
     * @brief Tunnel type key
     *
     * @type sai_tunnel_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     * @isresourcetype true
     */
    SAI_SWITCH_TUNNEL_ATTR_TUNNEL_TYPE = SAI_SWITCH_TUNNEL_ATTR_START,

    /**
     * @brief Packet action when a packet ingress and gets routed back to same tunnel
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_SWITCH_TUNNEL_ATTR_LOOPBACK_PACKET_ACTION,

    /* Tunnel encap attributes */

    /**
     * @brief Tunnel encap ECN mode
     *
     * @type sai_tunnel_encap_ecn_mode_t
     * @flags CREATE_ONLY
     * @default SAI_TUNNEL_ENCAP_ECN_MODE_STANDARD
     */
    SAI_SWITCH_TUNNEL_ATTR_TUNNEL_ENCAP_ECN_MODE,

    /**
     * @brief Tunnel encap ECN mappers only
     *
     * @type sai_object_list_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL_MAP
     * @default empty
     */
    SAI_SWITCH_TUNNEL_ATTR_ENCAP_MAPPERS,

    /* Tunnel decap attributes */

    /**
     * @brief Tunnel decap ECN mode
     *
     * @type sai_tunnel_decap_ecn_mode_t
     * @flags CREATE_ONLY
     * @default SAI_TUNNEL_DECAP_ECN_MODE_STANDARD
     */
    SAI_SWITCH_TUNNEL_ATTR_TUNNEL_DECAP_ECN_MODE,

    /**
     * @brief Tunnel decap ECN mappers only
     *
     * @type sai_object_list_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL_MAP
     * @default empty
     */
    SAI_SWITCH_TUNNEL_ATTR_DECAP_MAPPERS,

    /**
     * @brief Tunnel VXLAN UDP source port mode
     *
     * @type sai_tunnel_vxlan_udp_sport_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_TUNNEL_VXLAN_UDP_SPORT_MODE_EPHEMERAL
     */
    SAI_SWITCH_TUNNEL_ATTR_TUNNEL_VXLAN_UDP_SPORT_MODE,

    /**
     * @brief Tunnel UDP source port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     * @validonly SAI_SWITCH_TUNNEL_ATTR_TUNNEL_TYPE == SAI_TUNNEL_TYPE_VXLAN and SAI_SWITCH_TUNNEL_ATTR_TUNNEL_VXLAN_UDP_SPORT_MODE == SAI_TUNNEL_VXLAN_UDP_SPORT_MODE_USER_DEFINED
     */
    SAI_SWITCH_TUNNEL_ATTR_VXLAN_UDP_SPORT,

    /**
     * @brief Tunnel UDP source port mask
     *
     * Sport mask defining the number of least significant bits
     * reserved for the calculated hash value. 0 means a fixed value.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     * @validonly SAI_SWITCH_TUNNEL_ATTR_TUNNEL_TYPE == SAI_TUNNEL_TYPE_VXLAN and SAI_SWITCH_TUNNEL_ATTR_TUNNEL_VXLAN_UDP_SPORT_MODE == SAI_TUNNEL_VXLAN_UDP_SPORT_MODE_USER_DEFINED
     */
    SAI_SWITCH_TUNNEL_ATTR_VXLAN_UDP_SPORT_MASK,

    /**
     * @brief Enable TC AND COLOR -> DSCP MAP on tunnel at encapsulation (access-to-network) node to remark the DSCP in tunnel header
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_TUNNEL_ATTR_ENCAP_QOS_TC_AND_COLOR_TO_DSCP_MAP,

    /**
     * @brief Enable TC -> Queue MAP on tunnel encap
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on tunnel.
     * Default no map, i.e. packets are queued with static mapping.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_TUNNEL_ATTR_ENCAP_QOS_TC_TO_QUEUE_MAP,

    /**
     * @brief Enable DSCP -> TC MAP on tunnel at termination (Network-to-access) node.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_TUNNEL_ATTR_DECAP_QOS_DSCP_TO_TC_MAP,

    /**
     * @brief Enable TC -> Priority Group MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_TUNNEL_ATTR_DECAP_QOS_TC_TO_PRIORITY_GROUP_MAP,

    /**
     * @brief End of attributes
     */
    SAI_SWITCH_TUNNEL_ATTR_END,

    /** Custom range base value */
    SAI_SWITCH_TUNNEL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SWITCH_TUNNEL_ATTR_CUSTOM_RANGE_END

} sai_switch_tunnel_attr_t;

/**
 * @brief Attribute data for SAI_SWITCH_ATTR_HOSTIF_OPER_STATUS_UPDATE_MODE.
 */
typedef enum _sai_switch_hostif_oper_status_update_mode_t
{
    /**
     * @brief Application mode.
     *
     * In this mode, operational status of hostif must be updated by application
     * using hostif API with SAI_HOSTIF_ATTR_OPER_STATUS attribute. SAI adapter
     * should not update the hostif operational status internally.
     * When a host interface is created, application must update the operational
     * status if required and should not rely on SAI adapter to update it.
     */
    SAI_SWITCH_HOSTIF_OPER_STATUS_UPDATE_MODE_APPLICATION = 0,

    /**
     * @brief SAI adapter mode.
     *
     * In this mode, operational status of hostif is updated internally by SAI
     * adapter. Update of hostif operational status by application using hostif
     * API with SAI_HOSTIF_ATTR_OPER_STATUS is ignored.
     */
    SAI_SWITCH_HOSTIF_OPER_STATUS_UPDATE_MODE_SAI_ADAPTER = 1,

} sai_switch_hostif_oper_status_update_mode_t;

/**
 * @brief Attribute Id in sai_set_switch_attribute() and
 * sai_get_switch_attribute() calls.
 */
typedef enum _sai_switch_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SWITCH_ATTR_START,

    /**
     * @brief Number of active(created) ports on the switch
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_ACTIVE_PORTS = SAI_SWITCH_ATTR_START,

    /** @ignore - for backward compatibility */
    SAI_SWITCH_ATTR_PORT_NUMBER = SAI_SWITCH_ATTR_NUMBER_OF_ACTIVE_PORTS,

    /**
     * @brief Maximum number of supported ports on the switch
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_NUMBER_OF_SUPPORTED_PORTS,

    /**
     * @brief Get the port list
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     * @default internal
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
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     * @default internal
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
     * @brief The number of Unicast queues per port
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_UNICAST_QUEUES,

    /**
     * @brief The number of Multicast queues per port
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_MULTICAST_QUEUES,

    /**
     * @brief The total number of queues per port
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_QUEUES,

    /**
     * @brief The number of CPU queues
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_CPU_QUEUES,

    /**
     * @brief Local subnet routing supported.
     *
     * Routes with next hop set to "on-link".
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ON_LINK_ROUTE_SUPPORTED,

    /**
     * @brief Operational state
     *
     * @type sai_switch_oper_status_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_OPER_STATUS,

    /**
     * @brief Maximum number of temperature sensors available.
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_NUMBER_OF_TEMP_SENSORS,

    /**
     * @brief List of temperature readings from all sensors.
     *
     * Values in Celsius.
     *
     * @type sai_s32_list_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_TEMP_LIST,

    /**
     * @brief The current value of the maximum temperature
     * retrieved from the switch sensors
     *
     * Value in Celsius.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_TEMP,

    /**
     * @brief The average of temperature readings over all
     * sensors in the switch
     *
     * Value in Celsius.
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVERAGE_TEMP,

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
     * @brief Default SAI VLAN ID
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_VLAN
     * @default internal
     */
    SAI_SWITCH_ATTR_DEFAULT_VLAN_ID,

    /**
     * @brief Default SAI STP instance ID
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_STP
     * @default internal
     */
    SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID,

    /**
     * @brief Max number of STP instances that NPU supports
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_STP_INSTANCE,

    /**
     * @brief Default SAI Virtual Router ID
     *
     * Must return #SAI_STATUS_OBJECT_IN_USE when try to delete this VR ID.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @default internal
     */
    SAI_SWITCH_ATTR_DEFAULT_VIRTUAL_ROUTER_ID,

    /**
     * @brief Default SAI Override Virtual Router ID
     *
     * Must return #SAI_STATUS_OBJECT_IN_USE when try to delete this VR ID.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @default internal
     */
    SAI_SWITCH_ATTR_DEFAULT_OVERRIDE_VIRTUAL_ROUTER_ID,

    /**
     * @brief Default .1Q Bridge ID
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE
     * @default internal
     */
    SAI_SWITCH_ATTR_DEFAULT_1Q_BRIDGE_ID,

    /**
     * @brief Switch/Global bind point for ingress ACL object
     *
     * Bind (or unbind) an ingress ACL table or ACL group globally. Enable/Update
     * ingress ACL table or ACL group filtering by assigning a valid
     * object id. Disable ingress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_INGRESS_ACL,

    /**
     * @brief Switch/Global bind point for egress ACL object
     *
     * Bind (or unbind) an egress ACL tables or ACL group globally. Enable/Update
     * egress ACL table or ACL group filtering by assigning a valid
     * object id. Disable egress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_EGRESS_ACL,

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
     * @brief HQOS - Maximum number of childs supported per scheduler group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_CHILDS_PER_SCHEDULER_GROUP,

    /**
     * @brief Switch total buffer size in KB
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_TOTAL_BUFFER_SIZE,

    /**
     * @brief Switch number of ingress buffer pool
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
     * @brief Available IPv4 routes
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_IPV4_ROUTE_ENTRY,

    /**
     * @brief Available IPv6 routes
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_IPV6_ROUTE_ENTRY,

    /**
     * @brief Available IPv4 Nexthop entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_IPV4_NEXTHOP_ENTRY,

    /**
     * @brief Available IPv6 Nexthop entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_IPV6_NEXTHOP_ENTRY,

    /**
     * @brief Available IPv4 Neighbor entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_IPV4_NEIGHBOR_ENTRY,

    /**
     * @brief Available IPv6 Neighbor entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_IPV6_NEIGHBOR_ENTRY,

    /**
     * @brief Available Next hop group entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_ENTRY,

    /**
     * @brief Available Next hop group member entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_MEMBER_ENTRY,

    /**
     * @brief Available FDB entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_FDB_ENTRY,

    /**
     * @brief Available L2MC entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_L2MC_ENTRY,

    /**
     * @brief Available IPMC entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_IPMC_ENTRY,

    /**
     * @brief Available SNAT entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_SNAT_ENTRY,

    /**
     * @brief Available DNAT entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_DNAT_ENTRY,

    /**
     * @brief Available Double NAT entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_DOUBLE_NAT_ENTRY,

    /**
     * @brief Available ACL Tables
     *
     * @type sai_acl_resource_list_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_ACL_TABLE,

    /**
     * @brief Available ACL Table groups
     *
     * @type sai_acl_resource_list_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_ACL_TABLE_GROUP,

    /**
     * @brief Available My SID entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_MY_SID_ENTRY,

    /**
     * @brief Default trap group
     *
     * Default value after switch initialization:
     *
     * #SAI_HOSTIF_TRAP_GROUP_ATTR_ADMIN_STATE = true
     * SAI_HOSTIF_TRAP_GROUP_ATTR_PRIO = #SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY
     * #SAI_HOSTIF_TRAP_GROUP_ATTR_QUEUE = 0
     * #SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER = #SAI_NULL_OBJECT_ID
     *
     * The group handle is read only, while the group attributes, such as queue
     * and policer, may be modified.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP
     * @default internal
     */
    SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP,

    /**
     * @brief The hash object for packets going through ECMP
     *
     * Default value after switch initialization:
     *
     * #SAI_HASH_ATTR_NATIVE_HASH_FIELD_LIST = \[#SAI_NATIVE_HASH_FIELD_SRC_MAC,
     * #SAI_NATIVE_HASH_FIELD_DST_MAC, #SAI_NATIVE_HASH_FIELD_IN_PORT,
     * #SAI_NATIVE_HASH_FIELD_ETHERTYPE\]
     * #SAI_HASH_ATTR_UDF_GROUP_LIST empty list
     *
     * The object id is read only, while the object attributes can be modified.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_HASH
     * @default internal
     */
    SAI_SWITCH_ATTR_ECMP_HASH,

    /**
     * @brief The hash object for packets going through LAG
     *
     * Default value after switch initialization:
     *
     * #SAI_HASH_ATTR_NATIVE_HASH_FIELD_LIST = \[#SAI_NATIVE_HASH_FIELD_SRC_MAC,
     * #SAI_NATIVE_HASH_FIELD_DST_MAC, #SAI_NATIVE_HASH_FIELD_IN_PORT,
     * #SAI_NATIVE_HASH_FIELD_ETHERTYPE\]
     * #SAI_HASH_ATTR_UDF_GROUP_LIST empty list)
     *
     * The object id is read only, while the object attributes can be modified.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_HASH
     * @default internal
     */
    SAI_SWITCH_ATTR_LAG_HASH,

    /**
     * @brief Set Type of reboot WARM/COLD
     *
     * Indicates controlled warm restart.
     * Since warm restart can be caused by crash
     * (therefore there are no guarantees for this call),
     * this hint is really a performance optimization.
     * This hint is set as part of the shutdown sequence, before boot.
     * TRUE - Warm Reboot
     * FALSE - Cold Reboot
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_RESTART_WARM,

    /**
     * @brief Warm boot recovery
     *
     * Start warm boot recovery when set to true
     * This hint is set after boot.
     * In case of host adapter restart, host adapter can pass boot type in
     * #SAI_KEY_BOOT_TYPE. In case of host adapter recovery, host adapter can
     * pass a hint about the boot type and recovery, in this flag.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_WARM_RECOVER,

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
     * SAI due to initialize failure.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MIN_PLANNED_RESTART_INTERVAL,

    /**
     * @brief Nonvolatile storage required by both SAI and NPU in KB
     *
     * Will be 0 for #SAI_SWITCH_RESTART_TYPE_NONE.
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
     * @brief Count of the total number of ranges supported by NPU
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_ACL_RANGE_COUNT,

    /**
     * @brief ACL capabilities supported by the NPU
     *
     * @type sai_acl_capability_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_CAPABILITY,

    /**
     * @brief Multicast snooping capability supported by the NPU
     *
     * @type sai_switch_mcast_snooping_capability_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MCAST_SNOOPING_CAPABILITY,

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
     * Zero means learning limit is disabled.
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
    SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION,

    /**
     * @brief Broadcast miss action
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_PACKET_ACTION,

    /**
     * @brief Multicast miss action
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_PACKET_ACTION,

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
     * @brief SAI ECMP default hash offset
     *
     * When set, the output of the ECMP hash calculation will be rotated right
     * by the specified number of bits.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_OFFSET,

    /**
     * @brief SAI ECMP default symmetric hash
     *
     * When set, the hash calculation will result in the same value as when the
     * source and destination addresses (L2 src/dst MAC,L3 src/dst IP,L4
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
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HASH
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_ECMP_HASH_IPV4,

    /**
     * @brief The hash object for IPv4 in IPv4 packets going through ECMP
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HASH
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_ECMP_HASH_IPV4_IN_IPV4,

    /**
     * @brief The hash object for IPv6 packets going through ECMP
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HASH
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
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
     * @brief SAI LAG default hash offset
     *
     * When set, the output of the LAG hash calculation will be rotated right
     * by the specified number of bits.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_OFFSET,

    /**
     * @brief SAI LAG default symmetric hash
     *
     * When set, the hash calculation will result in the same value as when the source and
     * destination addresses (L2 src/dst MAC,L3 src/dst IP,L4 src/dst port) were swapped,
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
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HASH
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_LAG_HASH_IPV4,

    /**
     * @brief The hash object for IPv4 in IPv4 packets going through LAG
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HASH
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_LAG_HASH_IPV4_IN_IPV4,

    /**
     * @brief The hash object for IPv6 packets going through LAG
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HASH
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
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
     * To enable/disable trust Dot1p, Map ID should be added/removed on switch.
     * Default disabled.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_QOS_DOT1P_TO_TC_MAP,

    /**
     * @brief Enable DOT1P -> COLOR MAP on switch.
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust Dot1p, Map ID should be added/removed on switch.
     * Default disabled.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_QOS_DOT1P_TO_COLOR_MAP,

    /**
     * @brief Enable DSCP -> TC MAP on switch.
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust DSCP, Map ID should be added/removed on switch.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_QOS_DSCP_TO_TC_MAP,

    /**
     * @brief Enable DSCP -> COLOR MAP on switch
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * To enable/disable trust DSCP, Map ID should be added/removed on switch.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_QOS_DSCP_TO_COLOR_MAP,

    /**
     * @brief Enable TC -> Queue MAP on switch
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map i.e All packets to queue 0.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP,

    /**
     * @brief Enable TC + COLOR -> DOT1P MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP,

    /**
     * @brief Enable TC + COLOR -> DSCP MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
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
     * @brief Handle for switch profile id.
     *
     * Use this to retrieve the Key-Value pairs as part of switch
     * initialization.
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_SWITCH_ATTR_SWITCH_PROFILE_ID,

    /**
     * @brief Device Information for switch initialization.
     *
     * Hardware information format is based on SAI implementations by vendors.
     * String is NULL terminated. Format is vendor specific.
     * Example: Like PCI location, I2C address, MDIO address, MDIO bus SysFS information etc.
     * In case of NULL, First NPU attached to CPU will be initialized.
     * For the MDIO SysFS driver support, the interface name and phy_id should be
     * set and separated by "/", which should be formatted as {interface_name}/{phy_id}
     * Single NPU case this attribute is optional.
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_SWITCH_ATTR_SWITCH_HARDWARE_INFO,

    /**
     * @brief Vendor specific path name of the firmware to load.
     *
     * @type sai_s8_list_t
     * @flags CREATE_ONLY
     * @default empty
     */
    SAI_SWITCH_ATTR_FIRMWARE_PATH_NAME,

    /**
     * @brief Set to switch initialization or connect to NPU/SDK.
     *
     * TRUE - Initialize switch/SDK.
     * FALSE - Connect to SDK. This will connect library to the initialized SDK.
     * After this call the capability attributes should be ready for retrieval
     * via sai_get_switch_attribute()
     *
     * @type bool
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_SWITCH_ATTR_INIT_SWITCH,

    /**
     * @brief Operational status change notification callback
     * function passed to the adapter.
     *
     * Use sai_switch_state_change_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_switch_state_change_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_SWITCH_STATE_CHANGE_NOTIFY,

    /**
     * @brief Shutdown notification callback function passed to the adapter.
     *
     * Use sai_switch_shutdown_request_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_switch_shutdown_request_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_SWITCH_SHUTDOWN_REQUEST_NOTIFY,

    /** @ignore - for backward compatibility */
    SAI_SWITCH_ATTR_SHUTDOWN_REQUEST_NOTIFY = SAI_SWITCH_ATTR_SWITCH_SHUTDOWN_REQUEST_NOTIFY,

    /**
     * @brief FDB event notification callback function passed to the adapter.
     *
     * Use sai_fdb_event_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_fdb_event_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_FDB_EVENT_NOTIFY,

    /**
     * @brief Port state change notification callback function passed to the adapter.
     *
     * In case driver does not support this attribute, The Host adapter should poll
     * port status by SAI_PORT_ATTR_OPER_STATUS.
     *
     * Use sai_port_state_change_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_port_state_change_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_PORT_STATE_CHANGE_NOTIFY,

    /**
     * @brief Received packet event notification callback function passed to the adapter.
     *
     * Use sai_packet_event_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_packet_event_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_PACKET_EVENT_NOTIFY,

    /**
     * @brief Enable SAI function call fast mode, which executes calls very quickly
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_FAST_API_ENABLE,

    /**
     * @brief Set TC of mirrored packets
     *
     * This setting will apply to all mirror sessions.
     *
     * Default of 255 = disabled. When this attribute is disabled,
     * the TC of the mirrored frame will be derived from the packet
     * (the DOT1P priority in the VLAN Tag, for example).
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 255
     */
    SAI_SWITCH_ATTR_MIRROR_TC,

    /**
     * @brief Ingress ACL stage.
     *
     * @type sai_acl_capability_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_STAGE_INGRESS,

    /**
     * @brief Egress ACL stage.
     *
     * @type sai_acl_capability_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_STAGE_EGRESS,

    /**
     * @brief Max number of Segments supported in a single SRV6 SID List
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_SRV6_MAX_SID_DEPTH,

    /**
     * @brief List of Type Length Value types supported for source
     *
     * @type sai_s32_list_t sai_tlv_type_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_SRV6_TLV_TYPE,

    /**
     * @brief The number of lossless queues per port supported by the switch
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_QOS_NUM_LOSSLESS_QUEUES,

    /**
     * @brief Set Switch PFC deadlock event notification callback function passed to the adapter.
     *
     * Use sai_queue_pfc_deadlock_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_queue_pfc_deadlock_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_QUEUE_PFC_DEADLOCK_NOTIFY,

    /**
     * @brief Control for buffered and incoming packets on queue undergoing PFC Deadlock Recovery.
     *
     * This control applies to all packets on all applicable port/queues. If application wants finer packet
     * action control on per port per queue level then it is expected to set this control to packet forward
     * and install one or more ACL and enable/disable them in the DLD/DLR event callback
     * (SAI_SWITCH_ATTR_PFC_DEADLOCK_EVENT_NOTIFY) respectively.
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_DROP
     */
    SAI_SWITCH_ATTR_PFC_DLR_PACKET_ACTION,

    /**
     * @brief  PFC Deadlock Detection timer interval range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_PFC_TC_DLD_INTERVAL_RANGE,

    /**
     * @brief PFC Deadlock Detection timer interval in milliseconds.
     *
     * If the monitored queue is in XOFF state for more than this duration then
     * its considered to be in a PFC deadlock state and recovery process is kicked off.
     * Note: Use TC (Traffic Class) value as key and timer interval as value.
     *
     * @type sai_map_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SWITCH_ATTR_PFC_TC_DLD_INTERVAL,

    /**
     * @brief  PFC Deadlock Recovery timer interval range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_PFC_TC_DLR_INTERVAL_RANGE,

    /**
     * @brief PFC Deadlock Recovery timer interval in milliseconds.
     *
     * The PFC deadlock recovery process will run for this amount of time and then normal
     * state will resume. If the system remains in a deadlock state then the detection and
     * recovery will resume again after the configured detection timer interval.
     * Note: Use TC (Traffic Class) value as key and timer interval as value.
     *
     * @type sai_map_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SWITCH_ATTR_PFC_TC_DLR_INTERVAL,

    /**
     * @brief Get the list of supported protected object types.
     *        See comment for SAI_NEXT_HOP_GROUP_MEMBER_ATTR_MONITORED_OBJECT for more details.
     *
     * @type sai_s32_list_t sai_object_type_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_SUPPORTED_PROTECTED_OBJECT_TYPE,

    /**
     * @brief TPID for Outer vlan id
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0x88A8
     */
    SAI_SWITCH_ATTR_TPID_OUTER_VLAN,

    /**
     * @brief TPID for Inner vlan id
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0x8100
     */
    SAI_SWITCH_ATTR_TPID_INNER_VLAN,

    /**
     * @brief Perform CRC check
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_SWITCH_ATTR_CRC_CHECK_ENABLE,

    /**
     * @brief Perform CRC recalculation (overwriting CRC value on egress)
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_SWITCH_ATTR_CRC_RECALCULATION_ENABLE,

    /**
     * @brief Set Switch BFD session state change event notification callback function passed to the adapter.
     *
     * Use sai_bfd_session_state_change_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_bfd_session_state_change_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_BFD_SESSION_STATE_CHANGE_NOTIFY,

    /**
     * @brief Number of BFD session in the NPU
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_BFD_SESSION,

    /**
     * @brief Max number of BFD session NPU supports
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_BFD_SESSION,

    /**
     * @brief List of BFD session offloads that are supported for IPv4
     *
     * @type sai_s32_list_t sai_bfd_session_offload_type_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_SUPPORTED_IPV4_BFD_SESSION_OFFLOAD_TYPE,

    /**
     * @brief List of BFD session offloads that are supported for IPv6
     *
     * @type sai_s32_list_t sai_bfd_session_offload_type_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_SUPPORTED_IPV6_BFD_SESSION_OFFLOAD_TYPE,

    /**
     * @brief Minimum Receive interval NPU supports in microseconds
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MIN_BFD_RX,

    /**
     * @brief Minimum Transmit interval NPU supports in microseconds
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MIN_BFD_TX,

    /**
     * @brief Apply ECN thresholds for ECT traffic.
     *        Attribute controls whether ECT traffic needs to subjected to WRED
     *        thresholds or be subjected to ECN thresholds.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_ECN_ECT_THRESHOLD_ENABLE,

    /**
     * @brief Default VXLAN router MAC (inner destination MAC for VXLAN encapsulation)
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default vendor
     */
    SAI_SWITCH_ATTR_VXLAN_DEFAULT_ROUTER_MAC,

    /**
     * @brief Default VXLAN destination UDP port
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 4789
     */
    SAI_SWITCH_ATTR_VXLAN_DEFAULT_PORT,

    /**
     * @brief Max number of mirror session NPU supports
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_MIRROR_SESSION,

    /**
     * @brief Max number of sampled mirror session NPU supports
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_SAMPLED_MIRROR_SESSION,

    /**
     * @brief Get the list of supported get statistics extended modes
     *        Empty list should be returned if get statistics extended is not supported at all
     *
     * @type sai_s32_list_t sai_stats_mode_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_SUPPORTED_EXTENDED_STATS_MODE,

    /**
     * @brief Uninitialize data plane upon removal of switch object
     *
     * Typical use case for tear down of the host adapter, is to remove the switch ID,
     * which will stop all data and control plane, as leaving data plane open without
     * control can be a security risk.
     * However, on some scenarios, such as fast boot, host adapter would like to set
     * this value to false, call remove switch, and have the data plane still running.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_SWITCH_ATTR_UNINIT_DATA_PLANE_ON_REMOVAL,

    /**
     * @brief TAM bind point
     *
     * Bind (or unbind) the TAM object.
     * SAI_NULL_OBJECT_ID in the attribute value.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     * @default empty
     */
    SAI_SWITCH_ATTR_TAM_OBJECT_ID,

    /**
     * @brief Event notification callback
     * function passed to the adapter.
     *
     * Use sai_tam_event_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_tam_event_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_TAM_EVENT_NOTIFY,

    /**
     * @brief List of supported object types
     *
     * A list of object types (sai_object_type_t) that the SAI adapter can
     * support.
     *
     * @type sai_s32_list_t sai_object_type_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_SUPPORTED_OBJECT_TYPE_LIST,

    /**
     * @brief Instruct SAI to execute switch pre-shutdown
     *
     * Indicates controlled switch pre-shutdown as first step of warm shutdown.
     * This hint is optional, SAI application could skip this step and
     * go directly to warm shutdown.
     * This hint should be ignored, if at the time SAI receives this hint,
     * SAI_SWITCH_ATTR_RESTART_WARM is NOT already set to TRUE.
     * The scope of pre-shutdown is to backup SAI/SDK data, but leave CPU port
     * active for some final control plane traffic to go out.
     * TRUE - Execute switch pre-shutdown for warm shutdown
     * FALSE - No-op, does NOT mean cancelling already executed pre-shutdown
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_PRE_SHUTDOWN,

    /**
     * @brief NAT zone counter bind point
     *
     * Bind (or unbind) the NAT zone counter object.
     * SAI_NULL_OBJECT_ID in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NAT_ZONE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_NAT_ZONE_COUNTER_OBJECT_ID,

    /**
     * @brief Enable NAT function
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_NAT_ENABLE,

    /**
     * @brief Switch hardware access bus MDIO/I2C/CPLD
     *
     * @type sai_switch_hardware_access_bus_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_PHY
     */
    SAI_SWITCH_ATTR_HARDWARE_ACCESS_BUS,

    /**
     * @brief Platform context information
     *
     * Platform context information provided by the host adapter to driver.
     * This information is Host adapter specific, typically used for maintain
     * synchronization and device information. Driver will give this context back
     * to adapter as part of call back sai_switch_register_read/write_fn API.
     *
     * @type sai_uint64_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_PHY
     */
    SAI_SWITCH_ATTR_PLATFROM_CONTEXT,

    /**
     * @brief Platform adaption device read callback function passed to the adapter.
     * This is mandatory function for driver when device access not supported by file system.
     *
     * Use sai_switch_register_read_fn as read function.
     *
     * @type sai_pointer_t sai_switch_register_read_fn
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_PHY
     */
    SAI_SWITCH_ATTR_REGISTER_READ,

    /**
     * @brief Platform adaption device write callback function passed to the adapter.
     * This is mandatory function for driver when device access not supported by file system.
     *
     * Use sai_switch_register_write_fn as write function.
     *
     * @type sai_pointer_t sai_switch_register_write_fn
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_PHY
     */
    SAI_SWITCH_ATTR_REGISTER_WRITE,

    /**
     * @brief Enable/disable broadcast firmware download
     *
     * TRUE - Enable firmware download as broadcast.
     * FALSE - Enable firmware download as unicast.
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_SWITCH_ATTR_FIRMWARE_DOWNLOAD_BROADCAST,

    /**
     * @brief Firmware load method
     *
     * @type sai_switch_firmware_load_method_t
     * @flags CREATE_ONLY
     * @default SAI_SWITCH_FIRMWARE_LOAD_METHOD_INTERNAL
     */
    SAI_SWITCH_ATTR_FIRMWARE_LOAD_METHOD,

    /**
     * @brief Firmware load type auto/force/skip
     *
     * Check firmware version. If it is different from current version load firmware.
     * Otherwise always download the firmware specified by firmware load method.
     *
     * @type sai_switch_firmware_load_type_t
     * @flags CREATE_ONLY
     * @default SAI_SWITCH_FIRMWARE_LOAD_TYPE_AUTO
     */
    SAI_SWITCH_ATTR_FIRMWARE_LOAD_TYPE,

    /**
     * @brief Execute Firmware download
     *
     * In case of firmware download method broadcast, Set this attribute on
     * any one of device connected to same bus. As part of execute firmware will broadcast to
     * to all broadcast enabled devices on bus.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_SWITCH_ATTR_FIRMWARE_DOWNLOAD_BROADCAST == true
     */
    SAI_SWITCH_ATTR_FIRMWARE_DOWNLOAD_EXECUTE,

    /**
     * @brief End Broadcast
     *
     * Broadcast is enabled for BUS, All configurations will be broadcast.
     * End broadcast before initialize device.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_SWITCH_ATTR_FIRMWARE_DOWNLOAD_BROADCAST == true
     */
    SAI_SWITCH_ATTR_FIRMWARE_BROADCAST_STOP,

    /**
     * @brief Firmware status verify and complete initialize device.
     *
     * Host Adapter should mandatory to set attribute to true,
     * switch before doing any other configurations.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_SWITCH_ATTR_FIRMWARE_DOWNLOAD_BROADCAST == true
     */
    SAI_SWITCH_ATTR_FIRMWARE_VERIFY_AND_INIT_SWITCH,

    /**
     * @brief Firmware running status
     *
     * Indicates firmware download and running status.
     *
     * TRUE - Firmware running
     * FALSE - Firmware not running.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_FIRMWARE_STATUS,

    /**
     * @brief Firmware major version number
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_FIRMWARE_MAJOR_VERSION,

    /**
     * @brief Firmware minor version number
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_FIRMWARE_MINOR_VERSION,

    /**
     * @brief Get the port connector list
     *
     * validonly SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_PHY
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_PORT_CONNECTOR
     */
    SAI_SWITCH_ATTR_PORT_CONNECTOR_LIST,

    /**
     * @brief Propagate line side port state to system side port
     *
     * System side port state will reflect the ASIC port state.
     * Host adapter can depends on ASIC port state instead of port states from system side,
     * line side and ASIC port to determine interface operation status to application.
     *
     * TRUE - Device support for propagate line side port link status to system side port.
     * FALSE - Device does not support propagate port states.
     *
     * validonly SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_PHY
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_PROPOGATE_PORT_STATE_FROM_LINE_TO_SYSTEM_PORT_SUPPORT,

    /**
     * @brief Switch type NPU/PHY
     *
     * @type sai_switch_type_t
     * @flags CREATE_ONLY
     * @default SAI_SWITCH_TYPE_NPU
     */
    SAI_SWITCH_ATTR_TYPE,

    /**
     * @brief MACsec object list for this switch.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_MACSEC
     * @default empty
     */
    SAI_SWITCH_ATTR_MACSEC_OBJECT_LIST,

    /**
     * @brief Enable EXP -> TC MAP on switch.
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_QOS_MPLS_EXP_TO_TC_MAP,

    /**
     * @brief Enable EXP -> COLOR MAP on switch
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map in which case all exp values map to green color
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_QOS_MPLS_EXP_TO_COLOR_MAP,

    /**
     * @brief Enable TC + COLOR -> EXP MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_MPLS_EXP_MAP,

    /**
     * @brief Vendor specific switch ID. Identifies switch chip
     *
     * Mandatory in VOQ Switch
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_VOQ or SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_FABRIC
     */
    SAI_SWITCH_ATTR_SWITCH_ID,

    /**
     * @brief Maximum number of cores in the VOQ System (chassis)
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_VOQ
     */
    SAI_SWITCH_ATTR_MAX_SYSTEM_CORES,

    /**
     * @brief System port configuration list.
     *
     * @type sai_system_port_config_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_VOQ
     */
    SAI_SWITCH_ATTR_SYSTEM_PORT_CONFIG_LIST,

    /**
     * @brief Number of system ports
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_SYSTEM_PORTS,

    /**
     * @brief Get the system port list
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_SYSTEM_PORT
     * @default internal
     */
    SAI_SWITCH_ATTR_SYSTEM_PORT_LIST,

    /**
     * @brief Number of fabric ports on the switch
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_NUMBER_OF_FABRIC_PORTS,

    /**
     * @brief Get the fabric port list
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     * @default internal
     */
    SAI_SWITCH_ATTR_FABRIC_PORT_LIST,

    /**
     * @brief The size of the packet DMA pool memory in bytes
     * This can be used in conjunction with sai_allocate_hostif_packet_fn
     * to allocate a pool of memory upfront and manage it in the
     * application
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_PACKET_DMA_MEMORY_POOL_SIZE,

    /**
     * @brief Failover configuration modes
     *
     * @type sai_switch_failover_config_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_SWITCH_FAILOVER_CONFIG_MODE_NO_HITLESS
     */
    SAI_SWITCH_ATTR_FAILOVER_CONFIG_MODE,

    /**
     * @brief Query for Failover mode support
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_SUPPORTED_FAILOVER_MODE,

    /**
     * @brief Switch scoped Tunnel objects
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_SWITCH_TUNNEL
     * @default empty
     */
    SAI_SWITCH_ATTR_TUNNEL_OBJECTS_LIST,

    /**
     * @brief The size of the available packet DMA pool memory in bytes
     * This can be used in conjunction with total packet DMA pool
     * size to account/debug % of memory available.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_PACKET_AVAILABLE_DMA_MEMORY_POOL_SIZE,

    /**
     * @brief Switch/Global bind point for Pre-ingress ACL object
     *
     * Bind (or unbind) an Pre-ingress ACL table or ACL group globally. Enable/Update
     * Pre-ingress ACL table or ACL group filtering by assigning a valid
     * object id. Disable pre-ingress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_PRE_INGRESS_ACL,

    /**
     * @brief Available SNAPT entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_SNAPT_ENTRY,

    /**
     * @brief Available DNAPT entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_DNAPT_ENTRY,

    /**
     * @brief Available Double NAPT entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_DOUBLE_NAPT_ENTRY,

    /**
     * @brief Slave MDIO Address list
     *
     * Configure list of slave MDIO addresses for firmware download in Broadcast mode.
     * The sequence for firmware download in broadcast mode is as follows:
     * 1. For each MDIO master, call sai_create_switch() and pass the list of slave MDIO addresses.
     * In this step, gearbox will upgrade the firmware on all PHY devices including master and slave.
     *
     * 2. Call sai_create_switch() on all slave PHY devices with #SAI_SWITCH_ATTR_FIRMWARE_LOAD_TYPE = SAI_SWITCH_FIRMWARE_LOAD_TYPE_SKIP,
     * which will already have had their firmware upgraded.
     *
     * @type sai_u8_list_t
     * @flags CREATE_ONLY
     * @default empty
     * @validonly SAI_SWITCH_ATTR_FIRMWARE_DOWNLOAD_BROADCAST == true
     */
    SAI_SWITCH_ATTR_SLAVE_MDIO_ADDR_LIST,

    /**
     * @brief Minimum priority for My MAC
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MY_MAC_TABLE_MINIMUM_PRIORITY,

    /**
     * @brief Maximum priority for My MAC
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MY_MAC_TABLE_MAXIMUM_PRIORITY,

    /**
     * @brief My MAC entries installed on the switch
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_MY_MAC
     */
    SAI_SWITCH_ATTR_MY_MAC_LIST,

    /**
     * @brief Number of My MAC entries installed on the switch
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_INSTALLED_MY_MAC_ENTRIES,

    /**
     * @brief Number of available My MAC entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_MY_MAC_ENTRIES,

    /**
     * @brief Maximum forwarding classes limit
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_NUMBER_OF_FORWARDING_CLASSES,

    /**
     * @brief Enable DSCP -> Forwarding Class MAP on switch
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_QOS_DSCP_TO_FORWARDING_CLASS_MAP,

    /**
     * @brief Enable EXP -> Forwarding Class MAP on switch
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on switch.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_QOS_MPLS_EXP_TO_FORWARDING_CLASS_MAP,

    /**
     * @brief IPsec object for this switch.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_IPSEC
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_IPSEC_OBJECT_ID,

    /**
     * @brief TPID in IPsec SA-Tag.  This is used only between a Switch ASIC
     * and IPsec-enabled PHY chips and not packets on external network.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0xFFFE
     */
    SAI_SWITCH_ATTR_IPSEC_SA_TAG_TPID,

    /**
     * @brief IPsec SA status change notification callback function.
     *
     * In case driver does not support this attribute, The Host adapter should poll
     * IPsec SA status by SAI_IPSEC_SA_ATTR_OCTET_COUNT_STATUS and
     * SAI_IPSEC_SA_ATTR_EGRESS_ESN.
     *
     * Use sai_ipsec_sa_status_change_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_ipsec_sa_status_change_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_IPSEC_SA_STATUS_CHANGE_NOTIFY,

    /**
     * @brief NAT event notification callback function passed to the adapter.
     *
     * Use sai_nat_event_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_nat_event_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_NAT_EVENT_NOTIFY,

    /**
     * @brief Number of ECMP members supported across the all nexthop groups by switch
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_ECMP_MEMBER_COUNT,

    /**
     * @brief Number of ECMP Members configured. SAI_SWITCH_ATTR_ECMP_MEMBER_COUNT takes precedence over SAI_KEY_NUM_ECMP_GROUPS string. Default value is same as SAI_SWITCH_ATTR_ECMP_MEMBERS.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 64
     */
    SAI_SWITCH_ATTR_ECMP_MEMBER_COUNT,

    /**
     * @brief Adaptive routing and switching quality map
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ARS_PROFILE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_ARS_PROFILE,

    /**
     * @brief Post Ingress ACL stage.
     *
     * @type sai_acl_capability_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_STAGE_POST_INGRESS,

    /**
     * @brief Enable or disable credit watchdog
     *
     * Credit Watchdog can be enabled or disabled using this attribute for VOQ based system
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     * @validonly SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_VOQ
     */
    SAI_SWITCH_ATTR_CREDIT_WD,

    /**
     * @brief Credit watchdog threshold timer in milliseconds
     * Value must be within 10ms - 1000ms range
     * Queue is set to delete state and all packets in queue are deleted after the timer expiry
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 500
     * @validonly SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_VOQ
     */
    SAI_SWITCH_ATTR_CREDIT_WD_TIMER,

    /**
     * @brief Fabric chassis isolation setting.
     *
     * true: Trigger the switch isolate process
     * false: Undo the isolation operation.
     * This attribute is for Fabric or VOQ Chassis only
     * Setting this flag to true will stop data traffic from flowing
     * on local fabric ports. Fabric control traffic like credit grants
     * and control messages will still continue to flow. Setting this
     * flag to false will (re)enable data traffic on fabric ports.
     * If this attribute is set to true, it overrides port level isolation setting.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_FABRIC or SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_VOQ
     */
    SAI_SWITCH_ATTR_SWITCH_ISOLATE,

    /**
     * @brief Set hostif operational status update mode.
     *
     * This will set layer responsible for updating the operational status of
     * hostif.
     *
     * @type sai_switch_hostif_oper_status_update_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_SWITCH_HOSTIF_OPER_STATUS_UPDATE_MODE_APPLICATION
     */
    SAI_SWITCH_ATTR_HOSTIF_OPER_STATUS_UPDATE_MODE,

    /**
     * @brief Health notification callback function passed to the adapter.
     *
     * Use sai_switch_asic_sdk_health_event_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_switch_asic_sdk_health_event_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_SWITCH_ASIC_SDK_HEALTH_EVENT_NOTIFY,

    /**
     * @brief Registration for health fatal categories.
     *
     * For specifying categories of causes for severity fatal events
     *
     * @type sai_s32_list_t sai_switch_asic_sdk_health_category_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SWITCH_ATTR_REG_FATAL_SWITCH_ASIC_SDK_HEALTH_CATEGORY,

    /**
     * @brief Registration for health warning categories.
     *
     * For specifying categories of causes for severity warning events
     *
     * @type sai_s32_list_t sai_switch_asic_sdk_health_category_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SWITCH_ATTR_REG_WARNING_SWITCH_ASIC_SDK_HEALTH_CATEGORY,

    /**
     * @brief Registration for health notice categories.
     *
     * For specifying categories of causes for severity notice events
     *
     * @type sai_s32_list_t sai_switch_asic_sdk_health_category_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_SWITCH_ATTR_REG_NOTICE_SWITCH_ASIC_SDK_HEALTH_CATEGORY,

    /**
     * @brief ACL chain capabilities supported by the NPU
     *
     * @type sai_acl_chain_list_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_CHAIN_LIST,

    /**
     * @brief Port host tx ready notification callback
     * function passed to the adapter.
     *
     * Use sai_port_host_tx_ready_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_port_host_tx_ready_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_PORT_HOST_TX_READY_NOTIFY,

    /**
     * @brief Minimum priority for Tunnel Term table
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_TUNNEL_TERM_TABLE_ENTRY_MINIMUM_PRIORITY,

    /**
     * @brief Maximum priority for Tunnel Term table
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_TUNNEL_TERM_TABLE_ENTRY_MAXIMUM_PRIORITY,

    /**
     * @brief TWAMP session event notification callback function passed to the adapter.
     *
     * Use sai_twamp_session_event_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_twamp_session_event_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_TWAMP_SESSION_EVENT_NOTIFY,

    /**
     * @brief Number of Two-Way Active Measurement Protocol session
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_TWAMP_SESSION,

    /**
     * @brief Max number of Two-Way Active Measurement Protocol session supports
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_TWAMP_SESSION,

    /**
     * @brief Available IP Next hop group member entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_IP_NEXT_HOP_GROUP_MEMBER_ENTRY,

    /**
     * @brief Available VOQ including unicast and multicast VOQ
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_SYSTEM_VOQS,

    /**
     * @brief Pre-Ingress ACL stage.
     *
     * @type sai_acl_capability_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_ACL_STAGE_PRE_INGRESS,

    /**
     * @brief POE device list
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_POE_DEVICE
     * @default empty
     */
    SAI_SWITCH_ATTR_POE_DEVICE_LIST,

    /**
     * @brief Set Switch ICMP_ECHO session state change event notification callback function passed to the adapter.
     *
     * Use sai_icmp_echo_session_state_change_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_icmp_echo_session_state_change_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_ICMP_ECHO_SESSION_STATE_CHANGE_NOTIFY,

    /**
     * @brief Available ICMP ECHO sessions
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_ICMP_ECHO_SESSION,

    /**
     * @brief Max number of ICMP ECHO sessions supported
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_ICMP_ECHO_SESSION,

    /**
     * @brief Maximum size of the packet after trimming
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 128
     */
    SAI_SWITCH_ATTR_PACKET_TRIMMING_SIZE,

    /**
     * @brief New packet trimming DSCP value
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_PACKET_TRIMMING_DSCP_VALUE,

    /**
     * @brief New packet trimming queue index
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_PACKET_TRIMMING_QUEUE_INDEX,

    /**
     * @brief End of attributes
     */
    SAI_SWITCH_ATTR_END,

    /** Custom range base value */
    SAI_SWITCH_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_SWITCH_ATTR_CUSTOM_RANGE_END,

    /** Extensions range base */
    SAI_SWITCH_ATTR_EXTENSIONS_RANGE_BASE = 0x20000000

} sai_switch_attr_t;

/**
 * @brief Switch health event severity
 */
typedef enum _sai_switch_asic_sdk_health_severity_t
{
    /** Switch event severity fatal */
    SAI_SWITCH_ASIC_SDK_HEALTH_SEVERITY_FATAL,

    /** Switch event severity warning */
    SAI_SWITCH_ASIC_SDK_HEALTH_SEVERITY_WARNING,

    /** Switch event severity notice */
    SAI_SWITCH_ASIC_SDK_HEALTH_SEVERITY_NOTICE

} sai_switch_asic_sdk_health_severity_t;

/**
 * @brief Switch health categories
 */
typedef enum _sai_switch_asic_sdk_health_category_t
{
    /** Switch health software category */
    SAI_SWITCH_ASIC_SDK_HEALTH_CATEGORY_SW,

    /** Switch health firmware category */
    SAI_SWITCH_ASIC_SDK_HEALTH_CATEGORY_FW,

    /** Switch health cpu hardware category */
    SAI_SWITCH_ASIC_SDK_HEALTH_CATEGORY_CPU_HW,

    /** Switch health ASIC hardware category */
    SAI_SWITCH_ASIC_SDK_HEALTH_CATEGORY_ASIC_HW

} sai_switch_asic_sdk_health_category_t;

/**
 * @brief Switch counter IDs in sai_get_switch_stats() call
 *
 * @flags ranges
 */
typedef enum _sai_switch_stat_t
{
    /** Switch stat in drop reasons range start */
    SAI_SWITCH_STAT_IN_DROP_REASON_RANGE_BASE = 0x00001000,

    /** Get in switch packet drops configured by debug counter API at index 0 */
    SAI_SWITCH_STAT_IN_CONFIGURED_DROP_REASONS_0_DROPPED_PKTS = SAI_SWITCH_STAT_IN_DROP_REASON_RANGE_BASE,

    /** Get in switch packet drops configured by debug counter API at index 1 */
    SAI_SWITCH_STAT_IN_CONFIGURED_DROP_REASONS_1_DROPPED_PKTS,

    /** Get in switch packet drops configured by debug counter API at index 2 */
    SAI_SWITCH_STAT_IN_CONFIGURED_DROP_REASONS_2_DROPPED_PKTS,

    /** Get in switch packet drops configured by debug counter API at index 3 */
    SAI_SWITCH_STAT_IN_CONFIGURED_DROP_REASONS_3_DROPPED_PKTS,

    /** Get in switch packet drops configured by debug counter API at index 4 */
    SAI_SWITCH_STAT_IN_CONFIGURED_DROP_REASONS_4_DROPPED_PKTS,

    /** Get in switch packet drops configured by debug counter API at index 5 */
    SAI_SWITCH_STAT_IN_CONFIGURED_DROP_REASONS_5_DROPPED_PKTS,

    /** Get in switch packet drops configured by debug counter API at index 6 */
    SAI_SWITCH_STAT_IN_CONFIGURED_DROP_REASONS_6_DROPPED_PKTS,

    /** Get in switch packet drops configured by debug counter API at index 7 */
    SAI_SWITCH_STAT_IN_CONFIGURED_DROP_REASONS_7_DROPPED_PKTS,

    /** Switch stat in drop reasons range end */
    SAI_SWITCH_STAT_IN_DROP_REASON_RANGE_END = 0x00001fff,

    /** Switch stat out drop reasons range start */
    SAI_SWITCH_STAT_OUT_DROP_REASON_RANGE_BASE = 0x00002000,

    /** Get out switch packet drops configured by debug counter API at index 0 */
    SAI_SWITCH_STAT_OUT_CONFIGURED_DROP_REASONS_0_DROPPED_PKTS = SAI_SWITCH_STAT_OUT_DROP_REASON_RANGE_BASE,

    /** Get out switch packet drops configured by debug counter API at index 1 */
    SAI_SWITCH_STAT_OUT_CONFIGURED_DROP_REASONS_1_DROPPED_PKTS,

    /** Get out switch packet drops configured by debug counter API at index 2 */
    SAI_SWITCH_STAT_OUT_CONFIGURED_DROP_REASONS_2_DROPPED_PKTS,

    /** Get out switch packet drops configured by debug counter API at index 3 */
    SAI_SWITCH_STAT_OUT_CONFIGURED_DROP_REASONS_3_DROPPED_PKTS,

    /** Get out switch packet drops configured by debug counter API at index 4 */
    SAI_SWITCH_STAT_OUT_CONFIGURED_DROP_REASONS_4_DROPPED_PKTS,

    /** Get out switch packet drops configured by debug counter API at index 5 */
    SAI_SWITCH_STAT_OUT_CONFIGURED_DROP_REASONS_5_DROPPED_PKTS,

    /** Get out switch packet drops configured by debug counter API at index 6 */
    SAI_SWITCH_STAT_OUT_CONFIGURED_DROP_REASONS_6_DROPPED_PKTS,

    /** Get out switch packet drops configured by debug counter API at index 7 */
    SAI_SWITCH_STAT_OUT_CONFIGURED_DROP_REASONS_7_DROPPED_PKTS,

    /** Switch stat out drop reasons range end */
    SAI_SWITCH_STAT_OUT_DROP_REASON_RANGE_END = 0x00002fff,

    /** Switch stat fabric drop reasons range start */
    SAI_SWITCH_STAT_FABRIC_DROP_REASON_RANGE_BASE = 0x00003000,

    /** Get ECC discards [fabric] */
    SAI_SWITCH_STAT_ECC_DROP = SAI_SWITCH_STAT_FABRIC_DROP_REASON_RANGE_BASE,

    /** Get reach-ability discards [switch | fabric] */
    SAI_SWITCH_STAT_REACHABILITY_DROP,

    /** Congestion related high watermark [switch] */
    SAI_SWITCH_STAT_HIGHEST_QUEUE_CONGESTION_LEVEL,

    /** Discards not counted in other switch stat type [switch | fabric] */
    SAI_SWITCH_STAT_GLOBAL_DROP,

    /** Get integrity discards [fabric] */
    SAI_SWITCH_STAT_PACKET_INTEGRITY_DROP,

    /** Switch stat fabric drop reasons range end */
    SAI_SWITCH_STAT_FABRIC_DROP_REASON_RANGE_END = 0x00003fff,

} sai_switch_stat_t;

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
 * 2: fast boot. Only initialize NPU. SAI/SDK state should not be persisted except for those related
 *    to physical port attributes such as SPEED, AUTONEG mode, admin state, operational status.
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
 * the transceiver type plugged in to the port
 */
#define SAI_KEY_HW_PORT_PROFILE_ID_CONFIG_FILE    "SAI_HW_PORT_PROFILE_ID_CONFIG_FILE"

/**
 * @brief Switch health event callback
 *
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 *
 * @param[in] switch_id Switch Id
 * @param[in] severity Health event severity
 * @param[in] timestamp Time and date of receiving the SDK Health event
 * @param[in] category Category of cause
 * @param[in] data Data of switch health
 * @param[in] description JSON-encoded description string with information delivered from SDK event/trap
 * Example of a possible description:
 * {
 *    "switch_id": "0x00000000000000AB",
 *    "severity": "2",
 *    "timestamp": {
 *        "tv_sec": "22429",
 *        "tv_nsec": "3428724"
 *    },
 *    "category": "3",
 *    "data": {
 *        data_type: "0"
 *    },
 *    "additional_data": "Some additional information"
 * }
 */
typedef void (*sai_switch_asic_sdk_health_event_notification_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ sai_switch_asic_sdk_health_severity_t severity,
        _In_ sai_timespec_t timestamp,
        _In_ sai_switch_asic_sdk_health_category_t category,
        _In_ sai_switch_health_data_t data,
        _In_ const sai_u8_list_t description);

/**
 * @brief Switch shutdown request callback.
 *
 * Adapter DLL may request a shutdown due to an unrecoverable failure
 * or a maintenance operation
 *
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 *
 * @param[in] switch_id Switch Id
 */
typedef void (*sai_switch_shutdown_request_notification_fn)(
        _In_ sai_object_id_t switch_id);

/**
 * @brief Switch operational state change notification
 *
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 *
 * @param[in] switch_id Switch Id
 * @param[in] switch_oper_status New switch operational state
 */
typedef void (*sai_switch_state_change_notification_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ sai_switch_oper_status_t switch_oper_status);

/**
 * @brief Platform specific device register read access
 *
 * This API provides platform adaption functionality to access device
 * registers from driver. This is mandatory to pass as attribute to
 * sai_create_switch when driver implementation does not support register access
 * by device file system directly.
 *
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 *
 * @param[in] platform_context Platform context information.
 * @param[in] device_addr Device address(PHY/lane/port MDIO address)
 * @param[in] start_reg_addr Starting register address to read
 * @param[in] number_of_registers Number of consecutive registers to read
 * @param[out] reg_val Register read values
 */
typedef sai_status_t (*sai_switch_register_read_fn)(
        _In_ uint64_t platform_context,
        _In_ uint32_t device_addr,
        _In_ uint32_t start_reg_addr,
        _In_ uint32_t number_of_registers,
        _Out_ uint32_t *reg_val);

/**
 * @brief Platform specific device register write access
 *
 * This API provides platform adaption functionality to access device
 * registers from driver. This is mandatory to pass as attribute to
 * sai_create_switch when driver implementation does not support register access
 * by device file system directly.
 *
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 *
 * @param[in] platform_context Platform context information.
 * @param[in] device_addr Device address(PHY/lane/port MDIO address)
 * @param[in] start_reg_addr Starting register address to write
 * @param[in] number_of_registers Number of consecutive registers to write
 * @param[in] reg_val Register write values
 */
typedef sai_status_t (*sai_switch_register_write_fn)(
        _In_ uint64_t platform_context,
        _In_ uint32_t device_addr,
        _In_ uint32_t start_reg_addr,
        _In_ uint32_t number_of_registers,
        _In_ const uint32_t *reg_val);

/**
 * @brief Switch MDIO read API
 *
 * Provides read access API for devices connected to MDIO from NPU SAI.
 *
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 *
 * @param[in] switch_id Switch Id
 * @param[in] device_addr Device address(PHY/lane/port MDIO address)
 * @param[in] start_reg_addr Starting register address to read
 * @param[in] number_of_registers Number of consecutive registers to read
 * @param[out] reg_val Register read values
 */
typedef sai_status_t (*sai_switch_mdio_read_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t device_addr,
        _In_ uint32_t start_reg_addr,
        _In_ uint32_t number_of_registers,
        _Out_ uint32_t *reg_val);

/**
 * @brief Switch MDIO write API
 *
 * Provides write access API for devices connected to MDIO from NPU SAI.
 *
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 *
 * @param[in] switch_id Switch Id
 * @param[in] device_addr Device address(PHY/lane/port MDIO address)
 * @param[in] start_reg_addr Starting register address to write
 * @param[in] number_of_registers Number of consecutive registers to write
 * @param[in] reg_val Register write values
 */
typedef sai_status_t (*sai_switch_mdio_write_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t device_addr,
        _In_ uint32_t start_reg_addr,
        _In_ uint32_t number_of_registers,
        _In_ const uint32_t *reg_val);

/**
 * @brief Switch MDIO clause 22 read API
 *
 * Provides clause 22 read access API for devices connected to MDIO from NPU SAI.
 *
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 *
 * @param[in] switch_id Switch Id
 * @param[in] device_addr Device address(PHY/lane/port MDIO address)
 * @param[in] start_reg_addr Starting register address to read
 * @param[in] number_of_registers Number of consecutive registers to read
 * @param[out] reg_val Register read values
 */
typedef sai_status_t (*sai_switch_mdio_cl22_read_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t device_addr,
        _In_ uint32_t start_reg_addr,
        _In_ uint32_t number_of_registers,
        _Out_ uint32_t *reg_val);

/**
 * @brief Switch MDIO clause write API
 *
 * Provides clause 22 write access API for devices connected to MDIO from NPU SAI.
 *
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 *
 * @param[in] switch_id Switch Id
 * @param[in] device_addr Device address(PHY/lane/port MDIO address)
 * @param[in] start_reg_addr Starting register address to write
 * @param[in] number_of_registers Number of consecutive registers to write
 * @param[in] reg_val Register write values
 */
typedef sai_status_t (*sai_switch_mdio_cl22_write_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t device_addr,
        _In_ uint32_t start_reg_addr,
        _In_ uint32_t number_of_registers,
        _In_ const uint32_t *reg_val);

/**
 * @brief Create switch
 *
 * SDK initialization/connect to SDK. After the call the capability attributes should be
 * ready for retrieval via sai_get_switch_attribute(). Same Switch Object id should be
 * given for create/connect for each NPU.
 *
 * @param[out] switch_id The Switch Object ID
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_switch_fn)(
        _Out_ sai_object_id_t *switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove/disconnect Switch
 *
 * Release all resources associated with currently opened switch
 *
 * @param[in] switch_id The Switch id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_switch_fn)(
        _In_ sai_object_id_t switch_id);

/**
 * @brief Set switch attribute value
 *
 * @param[in] switch_id Switch id
 * @param[in] attr Switch attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_switch_attribute_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get switch attribute value
 *
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of switch attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_switch_attribute_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get switch statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] switch_id Switch id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_switch_stats_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get switch statistics counters extended.
 *
 * @param[in] switch_id Switch id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_switch_stats_ext_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear switch statistics counters.
 *
 * @param[in] switch_id Switch id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_switch_stats_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Create switch scoped tunnel
 *
 * @param[out] switch_tunnel_id The Switch Tunnel Object ID
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_switch_tunnel_fn)(
        _Out_ sai_object_id_t *switch_tunnel_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove/disconnect Switch scope tunnel
 *
 * Release all resources associated with currently opened switch
 *
 * @param[in] switch_tunnel_id The Switch Tunnel id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_switch_tunnel_fn)(
        _In_ sai_object_id_t switch_tunnel_id);

/**
 * @brief Set switch scoped tunnel attribute value
 *
 * @param[in] switch_tunnel_id Switch Tunnel id
 * @param[in] attr Switch tunnel attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_switch_tunnel_attribute_fn)(
        _In_ sai_object_id_t switch_tunnel_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get switch scoped tunnel attribute value
 *
 * @param[in] switch_tunnel_id Switch Tunnel id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of switch tunnel attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_switch_tunnel_attribute_fn)(
        _In_ sai_object_id_t switch_tunnel_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Switch method table retrieved with sai_api_query()
 */
typedef struct _sai_switch_api_t
{
    sai_create_switch_fn                   create_switch;
    sai_remove_switch_fn                   remove_switch;
    sai_set_switch_attribute_fn            set_switch_attribute;
    sai_get_switch_attribute_fn            get_switch_attribute;
    sai_get_switch_stats_fn                get_switch_stats;
    sai_get_switch_stats_ext_fn            get_switch_stats_ext;
    sai_clear_switch_stats_fn              clear_switch_stats;
    sai_switch_mdio_read_fn                switch_mdio_read;
    sai_switch_mdio_write_fn               switch_mdio_write;
    sai_create_switch_tunnel_fn            create_switch_tunnel;
    sai_remove_switch_tunnel_fn            remove_switch_tunnel;
    sai_set_switch_tunnel_attribute_fn     set_switch_tunnel_attribute;
    sai_get_switch_tunnel_attribute_fn     get_switch_tunnel_attribute;
    sai_switch_mdio_cl22_read_fn           switch_mdio_cl22_read;
    sai_switch_mdio_cl22_write_fn          switch_mdio_cl22_write;

} sai_switch_api_t;

/**
 * @}
 */
#endif /** __SAISWITCH_H_ */
