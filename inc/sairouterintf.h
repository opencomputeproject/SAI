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
*    sairouterintf.h
*
* Abstract:
*
*    This module defines SAI Router Interface
*
*/

#if !defined (__SAIROUTERINTF_H_)
#define __SAIROUTERINTF_H_

#include <saitypes.h>

/** \defgroup SAIROUTERINTF SAI - Router interface specific API definitions.
 *
 *  \{
 */
 
/**
 *  @brief Attribute data for SAI_ROUTER_INTERFACE_ATTR_TYPE
 */
typedef enum _sai_router_interface_type_t 
{
    /** Port or Lag Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_PORT,

    /** VLAN Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_VLAN,

    /** Loopback Router Interface Type */
    SAI_ROUTER_INTERFACE_TYPE_LOOPBACK

} sai_router_interface_type_t;


/**
 *  @brief Routing interface attribute IDs 
 */
typedef enum _sai_router_interface_attr_t
{
    /** READ-ONLY */

    /** Virtual router id [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,

    /** Type [sai_router_interface_type_t]  (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_ROUTER_INTERFACE_ATTR_TYPE,

    /** Assosiated Port or Lag object id [sai_object_id_t] 
    *  (MANDATORY_ON_CREATE when SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_PORT | CREATE_ONLY) 
    */
    SAI_ROUTER_INTERFACE_ATTR_PORT_ID,

    /** Assosiated Vlan [sai_vlan_id_t] 
    *  (MANDATORY_ON_CREATE when SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_VLAN | CREATE_ONLY)
    */
    SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,

    /** READ-WRITE */

    /** MAC Address [sai_mac_t] (CREATE_AND_SET)
     *  (not valid when SAI_ROUTER_INTERFACE_ATTR_TYPE == SAI_ROUTER_INTERFACE_TYPE_LOOPBACK)
     *  (default to SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS if not set on create) */
    SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,

    /** Admin V4 state [bool] (CREATE_AND_SET) (default to TRUE) */
    SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE,

    /** Admin V6 state [bool] (CREATE_AND_SET) (default to TRUE) */
    SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE,

    /** MTU [uint32_t] (CREATE_AND_SET) (default to 1514 bytes) */
    SAI_ROUTER_INTERFACE_ATTR_MTU,

    /** Packet action when neighbor table lookup miss for this router interface [sai_packet_action_t]
     * (CREATE_AND_SET) (default to SAI_PACKET_ACTION_TRAP) */
    SAI_ROUTER_INTERFACE_ATTR_NEIGHBOR_MISS_PACKET_ACTION,

    /* -- */

    /* Custom range base value */
    SAI_ROUTER_INTERFACE_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_router_interface_attr_t;

/**
 * Routine Description:
 *    @brief Create router interface. 
 *
 * Arguments:
 *    @param[out] rif_id - router interface id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t(*sai_create_router_interface_fn)(
    _Out_ sai_object_id_t* rif_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Remove router interface
 *
 * Arguments:
 *    @param[in] rif_id - router interface id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t(*sai_remove_router_interface_fn)(
    _In_ sai_object_id_t rif_id
    );

/**
 * Routine Description:
 *    @brief Set router interface attribute
 *
 * Arguments:
 *    @param[in] rif_id - router interface id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_router_interface_attribute_fn)(
    _In_ sai_object_id_t rif_id,
    _In_ const sai_attribute_t *attr
    );


/**
 * Routine Description:
 *    @brief Get router interface attribute
 *
 * Arguments:
 *    @param[in] rif_id - router interface id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_router_interface_attribute_fn)(
    _In_ sai_object_id_t rif_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 *  @brief Routing interface methods table retrieved with sai_api_query()
 */
typedef struct _sai_router_interface_api_t
{
    sai_create_router_interface_fn          create_router_interface;
    sai_remove_router_interface_fn          remove_router_interface;
    sai_set_router_interface_attribute_fn   set_router_interface_attribute;
    sai_get_router_interface_attribute_fn   get_router_interface_attribute;

} sai_router_interface_api_t;

/**
 * \}
 */
#endif // __SAIROUTERINTF_H_

