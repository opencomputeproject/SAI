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
 * @file    sairouterintf.h
 *
 * @brief   This module defines SAI Router interface
 */

#if !defined (__SAIROUTERINTF_H_)
#define __SAIROUTERINTF_H_

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
    /** Port or Lag Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_PORT,

    /** VLAN Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_VLAN,

    /** Loopback Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,

    /** Sub port Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_SUB_PORT,

    /** .1D Bridge Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_BRIDGE

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
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID = SAI_ROUTER_INTERFACE_ATTR_START,

    /**
     * @brief Type
     *
     * @type sai_router_interface_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_ROUTER_INTERFACE_ATTR_TYPE,

    /**
     * @brief Assosiated Port or Lag object id
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_PORT or SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_SUB_PORT
     */
    SAI_ROUTER_INTERFACE_ATTR_PORT_ID,

    /**
     * @brief Assosiated Vlan
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_VLAN
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_VLAN or SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_SUB_PORT
     */
    SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,

    /* READ-WRITE */

    /**
     * @brief MAC Address
     *
     * Not valid when #SAI_ROUTER_INTERFACE_ATTR_TYPE == #SAI_ROUTER_INTERFACE_TYPE_LOOPBACK)
     * Default to #SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS if not set on create)
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
     * Bind (or unbind) an ingress acl table or acl group on a RIF. Enable/Update
     * ingress ACL table or ACL group filtering by assigning a valid object id.
     * Disable ingress filtering by assigning SAI_NULL_OBJECT_ID in the
     * attribute value.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ROUTER_INTERFACE_ATTR_INGRESS_ACL,

    /**
     * @brief RIF bind point for egress ACL object
     *
     * Bind (or unbind) an egress acl table or acl group on a RIF. Enable/Update
     * egress ACL table or ACL group filtering by assigning a valid object id.
     * Disable egress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_ROUTER_INTERFACE_ATTR_EGRESS_ACL,

    /** Packet action when neighbor table lookup miss for this router interface [sai_packet_action_t]
     * (CREATE_AND_SET) (default to SAI_PACKET_ACTION_TRAP) */
    /**
     * @brief Packet action when neighbor table lookup miss for this router interface
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_TRAP
     */
    SAI_ROUTER_INTERFACE_ATTR_NEIGHBOR_MISS_PACKET_ACTION,

    /**
     * @brief V4 Mcast enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ROUTER_INTERFACE_ATTR_V4_MCAST_ENABLE,

    /**
     * @brief V6 Mcast enable
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ROUTER_INTERFACE_ATTR_V6_MCAST_ENABLE,

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
 * @brief Create router interface.
 *
 * @param[out] rif_id Router interface id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_create_router_interface_fn)(
        _Out_ sai_object_id_t *rif_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove router interface
 *
 * @param[in] rif_id Router interface id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_remove_router_interface_fn)(
        _In_ sai_object_id_t rif_id);

/**
 * @brief Set router interface attribute
 *
 * @param[in] rif_id Router interface id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_router_interface_attribute_fn)(
        _In_ sai_object_id_t rif_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get router interface attribute
 *
 * @param[in] rif_id Router interface id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_router_interface_attribute_fn)(
        _In_ sai_object_id_t rif_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Routing interface methods table retrieved with sai_api_query()
 */
typedef struct _sai_router_interface_api_t
{
    sai_create_router_interface_fn          create_router_interface;
    sai_remove_router_interface_fn          remove_router_interface;
    sai_set_router_interface_attribute_fn   set_router_interface_attribute;
    sai_get_router_interface_attribute_fn   get_router_interface_attribute;

} sai_router_interface_api_t;

/**
 * @}
 */
#endif /** __SAIROUTERINTF_H_ */
