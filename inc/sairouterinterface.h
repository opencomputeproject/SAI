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
 * @file    sairouterinterface.h
 *
 * @brief   This module defines SAI Router interface
 */

#if !defined (__SAIROUTERINTERFACE_H_)
#define __SAIROUTERINTERFACE_H_

#include <saitypes.h>

/**
 * @defgroup SAIROUTERINTF SAI - Router interface specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_ROUTER_INTERFACE_ATTR_TYPE
 */
typedef enum _sai_router_interface_type_t
{
    /** Port or LAG or System Port Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_PORT,

    /** VLAN Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_VLAN,

    /** Loopback Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,

    /** MPLS Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_MPLS_ROUTER,

    /** Sub port Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,

    /** .1D Bridge Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_BRIDGE,

    /** Q-in-Q Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_QINQ_PORT,

} sai_router_interface_type_t;

/**
 * @brief Routing interface attribute IDs
 */
typedef enum _sai_router_interface_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_ROUTER_INTERFACE_ATTR_START,

    /* READ-ONLY */

    /**
     * @brief Virtual router id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     */
    SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID = SAI_ROUTER_INTERFACE_ATTR_START,

    /**
     * @brief Router interface type
     *
     * @type sai_router_interface_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_ROUTER_INTERFACE_ATTR_TYPE,

    /**
     * @brief Associated Port, System Port or LAG object id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_SYSTEM_PORT
     * @condition SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_PORT or SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_SUB_PORT
     */
    SAI_ROUTER_INTERFACE_ATTR_PORT_ID,

    /**
     * @brief Associated Vlan
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_VLAN
     * @condition SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_VLAN
     */
    SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,

    /**
     * @brief Outer Vlan
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan true
     * @condition SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_QINQ_PORT or SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_SUB_PORT
     */
    SAI_ROUTER_INTERFACE_ATTR_OUTER_VLAN_ID,

    /**
     * @brief Inner Vlan
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isvlan true
     * @condition SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_QINQ_PORT
     */
    SAI_ROUTER_INTERFACE_ATTR_INNER_VLAN_ID,

    /**
     * @brief Associated 1D Bridge
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE
     * @condition SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_BRIDGE
     */
    SAI_ROUTER_INTERFACE_ATTR_BRIDGE_ID,

    /* READ-WRITE */

    /**
     * @brief MAC Address
     *
     * Not valid when SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_LOOPBACK
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default attrvalue SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS
     */
    SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,

    /**
     * @brief Admin V4 state
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE,

    /**
     * @brief Admin V6 state
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE,

    /**
     * @brief MTU
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1514
     */
    SAI_ROUTER_INTERFACE_ATTR_MTU,

    /**
     * @brief RIF bind point for ingress ACL object
     *
     * Bind (or unbind) an ingress ACL table or ACL group on a RIF.
     * Enable/Update ingress ACL table or ACL group filtering by assigning a
     * valid object id. Disable ingress filtering by assigning
     * SAI_NULL_OBJECT_ID in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ROUTER_INTERFACE_ATTR_INGRESS_ACL,

    /**
     * @brief RIF bind point for egress ACL object
     *
     * Bind (or unbind) an egress ACL table or ACL group on a RIF.
     * Enable/Update egress ACL table or ACL group filtering by assigning a
     * valid object id. Disable egress filtering by assigning
     * SAI_NULL_OBJECT_ID in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ROUTER_INTERFACE_ATTR_EGRESS_ACL,

    /**
     * @brief Packet action when neighbor table lookup miss for this router interface
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_TRAP
     */
    SAI_ROUTER_INTERFACE_ATTR_NEIGHBOR_MISS_PACKET_ACTION,

    /**
     * @brief V4 mcast enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ROUTER_INTERFACE_ATTR_V4_MCAST_ENABLE,

    /**
     * @brief V6 mcast enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ROUTER_INTERFACE_ATTR_V6_MCAST_ENABLE,

    /**
     * @brief Packet action when a packet ingress and gets routed on the same RIF
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_ROUTER_INTERFACE_ATTR_LOOPBACK_PACKET_ACTION,

    /**
     * @brief RIF creation is a virtual RIF.
     *
     * Create a Virtual RIF object, which only programs the ingress router MAC.
     * This simplifies the management of VRRP master router's configuration in
     * SAI adapter, as defined by RFC 5798 (or similar proprietary protocols).
     * Using a Virtual RIF allows SAI to optimize resources, so neighbor entries
     * cannot be learned on a Virtual RIF. On a virtual RIF following attributes
     * are invalid: ADMIN state, MTU size, packet action and multicast enable.
     * Alternatively VRRP can also be configured using native RIF objects without
     * using VIRTUAL attribute, with the expectation that SAI adapter will consume
     * resources that will not be used.
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ROUTER_INTERFACE_ATTR_IS_VIRTUAL,

    /**
     * @brief NAT Zone ID
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_ROUTER_INTERFACE_ATTR_NAT_ZONE_ID,

    /**
     * @brief To enable/disable Decrement TTL
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ROUTER_INTERFACE_ATTR_DISABLE_DECREMENT_TTL,

    /**
     * @brief Admin MPLS state
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ROUTER_INTERFACE_ATTR_ADMIN_MPLS_STATE,

    /**
     * @brief End of attributes
     */
    SAI_ROUTER_INTERFACE_ATTR_END,

    /** Custom range base value */
    SAI_ROUTER_INTERFACE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_ROUTER_INTERFACE_ATTR_CUSTOM_RANGE_END

} sai_router_interface_attr_t;

/**
 * @brief Router interface counter IDs in sai_get_router_interface_stats() call
 */
typedef enum _sai_router_interface_stat_t
{
    /** Ingress byte stat count */
    SAI_ROUTER_INTERFACE_STAT_IN_OCTETS,

    /** Ingress packet stat count */
    SAI_ROUTER_INTERFACE_STAT_IN_PACKETS,

    /** Egress byte stat count */
    SAI_ROUTER_INTERFACE_STAT_OUT_OCTETS,

    /** Egress packet stat count */
    SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS,

    /** Byte stat count for packets having errors on router ingress */
    SAI_ROUTER_INTERFACE_STAT_IN_ERROR_OCTETS,

    /** Packet stat count for packets having errors on router ingress */
    SAI_ROUTER_INTERFACE_STAT_IN_ERROR_PACKETS,

    /** Byte stat count for packets having errors on router egress */
    SAI_ROUTER_INTERFACE_STAT_OUT_ERROR_OCTETS,

    /** Packet stat count for packets having errors on router egress */
    SAI_ROUTER_INTERFACE_STAT_OUT_ERROR_PACKETS

} sai_router_interface_stat_t;

/**
 * @brief Create router interface.
 *
 * @param[out] router_interface_id Router interface id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_router_interface_fn)(
        _Out_ sai_object_id_t *router_interface_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove router interface
 *
 * @param[in] router_interface_id Router interface id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_router_interface_fn)(
        _In_ sai_object_id_t router_interface_id);

/**
 * @brief Set router interface attribute
 *
 * @param[in] router_interface_id Router interface id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_router_interface_attribute_fn)(
        _In_ sai_object_id_t router_interface_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get router interface attribute
 *
 * @param[in] router_interface_id Router interface id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_router_interface_attribute_fn)(
        _In_ sai_object_id_t router_interface_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get router interface statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] router_interface_id Router interface id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_router_interface_stats_fn)(
        _In_ sai_object_id_t router_interface_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get router interface statistics counters extended.
 *
 * @param[in] router_interface_id Router interface id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_router_interface_stats_ext_fn)(
        _In_ sai_object_id_t router_interface_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear router interface statistics counters.
 *
 * @param[in] router_interface_id Router interface id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_router_interface_stats_fn)(
        _In_ sai_object_id_t router_interface_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Routing interface methods table retrieved with sai_api_query()
 */
typedef struct _sai_router_interface_api_t
{
    sai_create_router_interface_fn          create_router_interface;
    sai_remove_router_interface_fn          remove_router_interface;
    sai_set_router_interface_attribute_fn   set_router_interface_attribute;
    sai_get_router_interface_attribute_fn   get_router_interface_attribute;
    sai_get_router_interface_stats_fn       get_router_interface_stats;
    sai_get_router_interface_stats_ext_fn   get_router_interface_stats_ext;
    sai_clear_router_interface_stats_fn     clear_router_interface_stats;

} sai_router_interface_api_t;

/**
 * @}
 */
#endif /** __SAIROUTERINTERFACE_H_ */
