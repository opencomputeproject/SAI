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

    /** Notify unknown source MAC using FDB callback. Do not learn in hardware. Do not forward.
    * When a packet from unknown source MAC comes this mode will trigger a new learn notification
    * via FDB callback for the MAC address. This mode will generate only one notification
    * per unknown source MAC to FDB callback.
    */
    SAI_BRIDGE_PORT_FDB_LEARNING_MODE_FDB_NOTIFICATION,

} sai_bridge_port_fdb_learning_mode_t;

/**
 * @brief Attribute data for #SAI_BRIDGE_PORT_ATTR_TYPE
 */
typedef enum _sai_bridge_port_type_t
{
    /** Port or Lag   */
    SAI_BRIDGE_PORT_TYPE_PORT,

    /** {Port or Lag.vlan}   */
    SAI_BRIDGE_PORT_TYPE_SUB_PORT,

    /**  bridge router port  */
    SAI_BRIDGE_PORT_TYPE_1Q_ROUTER,

    /**  bridge router port  */
    SAI_BRIDGE_PORT_TYPE_1D_ROUTER,

    /**  bridge tunnel  port  */
    SAI_BRIDGE_PORT_TYPE_TUNNEL,

} sai_bridge_port_type_t;

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
     * @brief Assosiated Port or Lag object id
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_PORT or SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_SUB_PORT
     */
    SAI_BRIDGE_PORT_ATTR_PORT_ID,

    /**
     * @brief Assosiated Vlan
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_SUB_PORT
     * @isvlan true
     */
    SAI_BRIDGE_PORT_ATTR_VLAN_ID,

    /**
     * @brief Assosiated rouer inerface object id
     * Please note that for SAI_BRIDGE_PORT_TYPE_1Q_ROUTER,
     * all vlan interfaces are auto bounded for the bridge port.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_1D_ROUTER
     */
    SAI_BRIDGE_PORT_ATTR_RIF_ID,

    /**
     * @brief Assosiated tunnel id
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_TUNNEL
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_BRIDGE_PORT_ATTR_TYPE == SAI_BRIDGE_PORT_TYPE_TUNNEL
     */
    SAI_BRIDGE_PORT_ATTR_TUNNEL_ID,

    /**
     * @brief Assosiated bridge id
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_BRIDGE
     * @flags CREATE_AND_SET
     * @default SAI_NULL_OBJECT_ID
     * @allownull true
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
     * @brief Action for packets with unknown source mac address
     * when FDB learning limit is reached.
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_DROP
     */
    SAI_BRIDGE_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION_PACKET_ACTION,

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
 * @brief Create bridge port
 *
 * @param[out] bridge_port_id Bridge port ID
 * @param[in] switch_id Switch object id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_create_bridge_port_fn)(
    _Out_ sai_object_id_t* bridge_port_id,
    _In_ sai_object_id_t switch_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove bridge port
 *
 * @param[in] bridge_port_id Bridge port ID
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_remove_bridge_port_fn) (
    _In_  sai_object_id_t bridge_port_id);

/**
 * @brief Set attribute for bridge port
 *
 * @param[in] bridge_port_id Bridge port ID
 * @param[in] attr attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_set_bridge_port_attribute_fn)(
    _In_ sai_object_id_t bridge_port_id,
    _In_ const sai_attribute_t *attr);

/**
 * @brief Get attributes of bridge port
 *
 * @param[in] bridge_port_id Bridge port ID
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_get_bridge_port_attribute_fn)(
    _In_ sai_object_id_t bridge_port_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute data for #SAI_BRIDGE_ATTR_TYPE
 */
typedef enum _sai_bridge_type_t
{
    /** vlan aware bridge */
    SAI_BRIDGE_TYPE_1Q,

    /** non vlan aware bridge */
    SAI_BRIDGE_TYPE_1D,

} sai_bridge_type_t;

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
     * @objects SAI_OBJECT_TYPE_BRIDGE_PORT
     * @flags READ_ONLY
     */
    SAI_BRIDGE_ATTR_PORT_LIST,

    /**
     * @brief Maximum number of learned MAC addresses
     *
     * Zero means learning limit disable
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
     * @brief End of attributes
     */
    SAI_BRIDGE_ATTR_END,

    /** Custom range base value */
    SAI_BRIDGE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_BRIDGE_ATTR_CUSTOM_RANGE_END

} sai_bridge_attr_t;

/**
 * @brief Create bridge
 *
 * @param[out] bridge_id Bridge ID
 * @param[in] switch_id Switch object id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_create_bridge_fn)(
    _Out_ sai_object_id_t* bridge_id,
    _In_ sai_object_id_t switch_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * @brief Remove bridge
 *
 * @param[in] bridge_id Bridge ID
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_remove_bridge_fn) (
    _In_  sai_object_id_t bridge_id
    );

/**
 * @brief Set attribute for bridge
 *
 * @param[in] bridge_id Bridge ID
 * @param[in] attr attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_set_bridge_attribute_fn)(
    _In_ sai_object_id_t  bridge_id,
    _In_ const sai_attribute_t *attr);

/**
 * @brief Get attributes of bridge
 *
 * @param[in] bridge_id Bridge ID
 * @param[in] attr_count number of attributes
 * @param[inout] attr_list array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_get_bridge_attribute_fn)(
    _In_ sai_object_id_t   bridge_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list);

/**
* @brief bridge methods table retrieved with sai_api_query()
*/
typedef struct _sai_bridge_api_t
{
    sai_create_bridge_fn         		create_bridge;
    sai_remove_bridge_fn 				remove_bridge;
    sai_set_bridge_attribute_fn  		set_bridge_attribute;
    sai_get_bridge_attribute_fn  		get_bridge_attribute;
    sai_create_bridge_port_fn         	create_bridge_port;
    sai_remove_bridge_port_fn 			remove_bridge_port;
    sai_set_bridge_port_attribute_fn  	set_bridge_port_attribute;
    sai_get_bridge_port_attribute_fn    get_bridge_port_attribute;
} sai_bridge_api_t;

/**
 * @}
 */
#endif /** __SAIBRIDGE_H_ */
