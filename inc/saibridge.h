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
 * @file    saibridge.h
 *
 * @brief   This module defines SAI Bridge
 */

#if !defined (__SAIBRIDGE_H_)
#define __SAIBRIDGE_H_

#include <saitypes.h>

/**
 * @defgroup SAIBRIDGE SAI - Bridge specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_BRIDGE_PORT_ATTR_FDB_LEARNING_MODE
 */
typedef enum _sai_bridge_port_fdb_learning_mode_t
{
    /** Drop packets with unknown source MAC. Do not learn. Do not forward */
    SAI_BRIDGE_PORT_FDB_LEARNING_MODE_DROP,

    /** Do not learn unknown source MAC. Forward based on destination MAC */
    SAI_BRIDGE_PORT_FDB_LEARNING_MODE_DISABLE,

    /** Hardware learning. Learn source MAC. Forward based on destination MAC */
    SAI_BRIDGE_PORT_FDB_LEARNING_MODE_HW,

    /** Trap packets with unknown source MAC to CPU. Do not learn. Do not forward */
    SAI_BRIDGE_PORT_FDB_LEARNING_MODE_CPU_TRAP,

    /** Trap packets with unknown source MAC to CPU. Do not learn. Forward based on destination MAC */
    SAI_BRIDGE_PORT_FDB_LEARNING_MODE_CPU_LOG,

    /**
     * @brief Notify unknown source MAC using FDB callback.
     *
     * Do not learn in hardware. Do not forward. When a packet from unknown
     * source MAC comes this mode will trigger a new learn notification via FDB
     * callback for the MAC address. This mode will generate only one
     * notification per unknown source MAC to FDB callback.
     */
    SAI_BRIDGE_PORT_FDB_LEARNING_MODE_FDB_NOTIFICATION,

} sai_bridge_port_fdb_learning_mode_t;

/**
 * @brief Attribute data for #SAI_BRIDGE_PORT_ATTR_TYPE
 */
typedef enum _sai_bridge_port_type_t
{
    /** Port or LAG or System Port */
    SAI_BRIDGE_PORT_TYPE_PORT,

    /** Port or LAG.vlan */
    SAI_BRIDGE_PORT_TYPE_SUB_PORT,

    /** Bridge router port */
    SAI_BRIDGE_PORT_TYPE_1Q_ROUTER,

    /** Bridge router port */
    SAI_BRIDGE_PORT_TYPE_1D_ROUTER,

    /** Bridge tunnel port */
    SAI_BRIDGE_PORT_TYPE_TUNNEL,

} sai_bridge_port_type_t;

/**
 * @brief Attribute data for #SAI_BRIDGE_PORT_ATTR_TAGGING_MODE
 */
typedef enum _sai_bridge_port_tagging_mode_t
{
    /** Untagged mode */
    SAI_BRIDGE_PORT_TAGGING_MODE_UNTAGGED,

    /** Tagged mode */
    SAI_BRIDGE_PORT_TAGGING_MODE_TAGGED,

} sai_bridge_port_tagging_mode_t;

/**
 * @brief SAI attributes for Bridge Port
 */
typedef enum _sai_bridge_port_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_BRIDGE_PORT_ATTR_START,

    /**
     * @brief Bridge port type
     *
     * @type sai_bridge_port_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_BRIDGE_PORT_ATTR_TYPE = SAI_BRIDGE_PORT_ATTR_START,

    /**
     * @brief Associated Port or LAG object id
     *
     * The CPU port is not a member of any bridge.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_SYSTEM_PORT
     * @condition SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_PORT or SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_SUB_PORT
     */
    SAI_BRIDGE_PORT_ATTR_PORT_ID,

    /**
     * @brief Tagging mode of the bridge port
     *
     * Specifies the tagging mode to be used during egress.
     *
     * @type sai_bridge_port_tagging_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_BRIDGE_PORT_TAGGING_MODE_TAGGED
     * @validonly SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_SUB_PORT
     */
    SAI_BRIDGE_PORT_ATTR_TAGGING_MODE,

    /**
     * @brief Associated Vlan
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan true
     * @condition SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_SUB_PORT
     */
    SAI_BRIDGE_PORT_ATTR_VLAN_ID,

    /**
     * @brief Associated router interface object id
     *
     * Please note that for SAI_BRIDGE_PORT_TYPE_1Q_ROUTER,
     * all vlan interfaces are auto bounded for the bridge port.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @condition SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_1D_ROUTER
     */
    SAI_BRIDGE_PORT_ATTR_RIF_ID,

    /**
     * @brief Associated tunnel id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_TUNNEL
     * @condition SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_TUNNEL
     */
    SAI_BRIDGE_PORT_ATTR_TUNNEL_ID,

    /**
     * @brief Associated bridge id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_BRIDGE
     * @condition SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_SUB_PORT or SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_1D_ROUTER or SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_TUNNEL
     */
    SAI_BRIDGE_PORT_ATTR_BRIDGE_ID,

    /**
     * @brief FDB Learning mode
     *
     * @type sai_bridge_port_fdb_learning_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_BRIDGE_PORT_FDB_LEARNING_MODE_HW
     */
    SAI_BRIDGE_PORT_ATTR_FDB_LEARNING_MODE,

    /**
     * @brief Maximum number of learned MAC addresses
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_BRIDGE_PORT_ATTR_MAX_LEARNED_ADDRESSES,

    /**
     * @brief Action for packets with unknown source MAC address
     * when FDB learning limit is reached.
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_DROP
     */
    SAI_BRIDGE_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION_PACKET_ACTION,

    /**
     * @brief Admin Mode.
     *
     * Before removing a bridge port, need to disable it by setting admin mode
     * to false, then flush the FDB entries, and then remove it.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_BRIDGE_PORT_ATTR_ADMIN_STATE,

    /**
     * @brief Ingress filtering (drop frames with unknown VLANs)
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_BRIDGE_PORT_ATTR_INGRESS_FILTERING,

    /**
     * @brief Egress filtering (drop frames with unknown VLANs at egress)
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_PORT
     */
    SAI_BRIDGE_PORT_ATTR_EGRESS_FILTERING,

    /**
     * @brief Isolation group id
     *
     * Packets ingressing on the bridge port should not be forwarded to the
     * members present in the isolation group.The isolation group type should
     * SAI_ISOLATION_GROUP_TYPE_BRIDGE_PORT.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ISOLATION_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_BRIDGE_PORT_ATTR_ISOLATION_GROUP,

    /**
     * @brief End of attributes
     */
    SAI_BRIDGE_PORT_ATTR_END,

    /** Custom range base value */
    SAI_BRIDGE_PORT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_BRIDGE_PORT_ATTR_CUSTOM_RANGE_END

} sai_bridge_port_attr_t;

/**
 * @brief Bridge port counter IDs in sai_get_bridge_port_stats() call
 */
typedef enum _sai_bridge_port_stat_t
{
    /** Ingress byte stat count */
    SAI_BRIDGE_PORT_STAT_IN_OCTETS,

    /** Ingress packet stat count */
    SAI_BRIDGE_PORT_STAT_IN_PACKETS,

    /** Egress byte stat count */
    SAI_BRIDGE_PORT_STAT_OUT_OCTETS,

    /** Egress packet stat count */
    SAI_BRIDGE_PORT_STAT_OUT_PACKETS

} sai_bridge_port_stat_t;

/**
 * @brief Create bridge port
 *
 * @param[out] bridge_port_id Bridge port ID
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_bridge_port_fn)(
        _Out_ sai_object_id_t *bridge_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove bridge port
 *
 * @param[in] bridge_port_id Bridge port ID
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_bridge_port_fn)(
        _In_ sai_object_id_t bridge_port_id);

/**
 * @brief Set attribute for bridge port
 *
 * @param[in] bridge_port_id Bridge port ID
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_bridge_port_attribute_fn)(
        _In_ sai_object_id_t bridge_port_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attributes of bridge port
 *
 * @param[in] bridge_port_id Bridge port ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_bridge_port_attribute_fn)(
        _In_ sai_object_id_t bridge_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get bridge port statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] bridge_port_id Bridge port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_bridge_port_stats_fn)(
        _In_ sai_object_id_t bridge_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get bridge port statistics counters extended.
 *
 * @param[in] bridge_port_id Bridge port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_bridge_port_stats_ext_fn)(
        _In_ sai_object_id_t bridge_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear bridge port statistics counters.
 *
 * @param[in] bridge_port_id Bridge port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_bridge_port_stats_fn)(
        _In_ sai_object_id_t bridge_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Attribute data for #SAI_BRIDGE_ATTR_TYPE
 */
typedef enum _sai_bridge_type_t
{
    /** Vlan aware bridge */
    SAI_BRIDGE_TYPE_1Q,

    /** Non vlan aware bridge */
    SAI_BRIDGE_TYPE_1D,

} sai_bridge_type_t;

/**
 * @brief Attribute data for unknown unicast, unknown multicast
 * and broadcast flood controls
 */
typedef enum _sai_bridge_flood_control_type_t
{
    /**
     * @brief Flood on all sub-ports
     *
     * When setting sub-ports to broadcast or unknown multicast flood, it also
     * includes flooding to the router. When setting sub-ports to unknown
     * unicast flood, it does not include flooding to the router
     */
    SAI_BRIDGE_FLOOD_CONTROL_TYPE_SUB_PORTS,

    /** Disable flooding */
    SAI_BRIDGE_FLOOD_CONTROL_TYPE_NONE,

    /** Flood on the L2MC group */
    SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP,

    /**
     * @brief Flood on all sub-ports and L2MC group
     *
     * Flood on all sub-ports, without the router
     * In addition, flood on the supplied L2MC group
     */
    SAI_BRIDGE_FLOOD_CONTROL_TYPE_COMBINED

} sai_bridge_flood_control_type_t;

/**
 * @brief SAI attributes for Bridge
 */
typedef enum _sai_bridge_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_BRIDGE_ATTR_START,

    /**
     * @brief Bridge type
     *
     * @type sai_bridge_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_BRIDGE_ATTR_TYPE = SAI_BRIDGE_ATTR_START,

    /**
     * @brief List of bridge ports associated to this bridge
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE_PORT
     */
    SAI_BRIDGE_ATTR_PORT_LIST,

    /**
     * @brief Maximum number of learned MAC addresses
     *
     * Zero means learning limit is disabled.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_BRIDGE_ATTR_MAX_LEARNED_ADDRESSES,

    /**
     * @brief To disable learning on a bridge
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_BRIDGE_ATTR_LEARN_DISABLE,

    /**
     * @brief Unknown unicast flood control type
     *
     * @type sai_bridge_flood_control_type_t
     * @flags CREATE_AND_SET
     * @default SAI_BRIDGE_FLOOD_CONTROL_TYPE_SUB_PORTS
     */
    SAI_BRIDGE_ATTR_UNKNOWN_UNICAST_FLOOD_CONTROL_TYPE,

    /**
     * @brief Unknown unicast flood group.
     *
     * Provides control on the set of bridge ports on which unknown unicast
     * packets need to be flooded. This attribute would be used only when
     * the SAI_BRIDGE_ATTR_UNKNOWN_UNICAST_FLOOD_CONTROL_TYPE is set as
     * SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP. When this attribute's value is
     * SAI_NULL_OBJECT_ID, then flooding would be disabled.
     * Valid for SAI_BRIDGE_TYPE_1D.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_BRIDGE_ATTR_UNKNOWN_UNICAST_FLOOD_CONTROL_TYPE == SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP or SAI_BRIDGE_ATTR_UNKNOWN_UNICAST_FLOOD_CONTROL_TYPE == SAI_BRIDGE_FLOOD_CONTROL_TYPE_COMBINED
     */
    SAI_BRIDGE_ATTR_UNKNOWN_UNICAST_FLOOD_GROUP,

    /**
     * @brief Unknown unicast flood control type
     *
     * @type sai_bridge_flood_control_type_t
     * @flags CREATE_AND_SET
     * @default SAI_BRIDGE_FLOOD_CONTROL_TYPE_SUB_PORTS
     */
    SAI_BRIDGE_ATTR_UNKNOWN_MULTICAST_FLOOD_CONTROL_TYPE,

    /**
     * @brief Unknown multicast flood group.
     *
     * Provides control on the set of bridge ports on which unknown multicast
     * packets need to be flooded. This attribute would be used only when
     * the SAI_BRIDGE_ATTR_UNKNOWN_MULTICAST_FLOOD_CONTROL_TYPE is set as
     * SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP.When this attribute's value is
     * SAI_NULL_OBJECT_ID, then flooding would be disabled.
     * Valid for SAI_BRIDGE_TYPE_1D.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_BRIDGE_ATTR_UNKNOWN_MULTICAST_FLOOD_CONTROL_TYPE == SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP or SAI_BRIDGE_ATTR_UNKNOWN_MULTICAST_FLOOD_CONTROL_TYPE == SAI_BRIDGE_FLOOD_CONTROL_TYPE_COMBINED
     */
    SAI_BRIDGE_ATTR_UNKNOWN_MULTICAST_FLOOD_GROUP,

    /**
     * @brief Broadcast flood control type
     *
     * @type sai_bridge_flood_control_type_t
     * @flags CREATE_AND_SET
     * @default SAI_BRIDGE_FLOOD_CONTROL_TYPE_SUB_PORTS
     */
    SAI_BRIDGE_ATTR_BROADCAST_FLOOD_CONTROL_TYPE,

    /**
     * @brief Broadcast flood group.
     *
     * Provides control on the set of bridge ports on which broadcast
     * packets need to be flooded. This attribute would be used only when
     * the SAI_BRIDGE_ATTR_BROADCAST_FLOOD_CONTROL_TYPE is set as
     * SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP.When this attribute's value is
     * SAI_NULL_OBJECT_ID, then flooding would be disabled.
     * Valid for SAI_BRIDGE_TYPE_1D.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_BRIDGE_ATTR_BROADCAST_FLOOD_CONTROL_TYPE == SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP or SAI_BRIDGE_ATTR_BROADCAST_FLOOD_CONTROL_TYPE == SAI_BRIDGE_FLOOD_CONTROL_TYPE_COMBINED
     */
    SAI_BRIDGE_ATTR_BROADCAST_FLOOD_GROUP,

    /**
     * @brief End of attributes
     */
    SAI_BRIDGE_ATTR_END,

    /** Custom range base value */
    SAI_BRIDGE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_BRIDGE_ATTR_CUSTOM_RANGE_END

} sai_bridge_attr_t;

/**
 * @brief Bridge counter IDs in sai_get_bridge_stats() call
 */
typedef enum _sai_bridge_stat_t
{
    /** Ingress byte stat count */
    SAI_BRIDGE_STAT_IN_OCTETS,

    /** Ingress packet stat count */
    SAI_BRIDGE_STAT_IN_PACKETS,

    /** Egress byte stat count */
    SAI_BRIDGE_STAT_OUT_OCTETS,

    /** Egress packet stat count */
    SAI_BRIDGE_STAT_OUT_PACKETS

} sai_bridge_stat_t;

/**
 * @brief Create bridge
 *
 * @param[out] bridge_id Bridge ID
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_bridge_fn)(
        _Out_ sai_object_id_t *bridge_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove bridge
 *
 * @param[in] bridge_id Bridge ID
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_bridge_fn)(
        _In_ sai_object_id_t bridge_id);

/**
 * @brief Set attribute for bridge
 *
 * @param[in] bridge_id Bridge ID
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_bridge_attribute_fn)(
        _In_ sai_object_id_t bridge_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attributes of bridge
 *
 * @param[in] bridge_id Bridge ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_bridge_attribute_fn)(
        _In_ sai_object_id_t bridge_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get bridge statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] bridge_id Bridge id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_bridge_stats_fn)(
        _In_ sai_object_id_t bridge_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get bridge statistics counters extended.
 *
 * @param[in] bridge_id Bridge id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_bridge_stats_ext_fn)(
        _In_ sai_object_id_t bridge_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear bridge statistics counters.
 *
 * @param[in] bridge_id Bridge id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_bridge_stats_fn)(
        _In_ sai_object_id_t bridge_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Bridge methods table retrieved with sai_api_query()
 */
typedef struct _sai_bridge_api_t
{
    sai_create_bridge_fn                create_bridge;
    sai_remove_bridge_fn                remove_bridge;
    sai_set_bridge_attribute_fn         set_bridge_attribute;
    sai_get_bridge_attribute_fn         get_bridge_attribute;
    sai_get_bridge_stats_fn             get_bridge_stats;
    sai_get_bridge_stats_ext_fn         get_bridge_stats_ext;
    sai_clear_bridge_stats_fn           clear_bridge_stats;
    sai_create_bridge_port_fn           create_bridge_port;
    sai_remove_bridge_port_fn           remove_bridge_port;
    sai_set_bridge_port_attribute_fn    set_bridge_port_attribute;
    sai_get_bridge_port_attribute_fn    get_bridge_port_attribute;
    sai_get_bridge_port_stats_fn        get_bridge_port_stats;
    sai_get_bridge_port_stats_ext_fn    get_bridge_port_stats_ext;
    sai_clear_bridge_port_stats_fn      clear_bridge_port_stats;
} sai_bridge_api_t;

/**
 * @}
 */
#endif /** __SAIBRIDGE_H_ */
