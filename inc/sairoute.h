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
*    sairoute.h
*
* Abstract:
*
*    This module defines SAI Route Entry API
*
*/

#if !defined (__SAIROUTE_H_)
#define __SAIROUTE_H_

#include <saitypes.h>

/** \defgroup SAIROUTE SAI - Route specific API definitions.
 *
 *  \{
 */
 
/**
 *  @brief Attribute Id for sai route object
 */
typedef enum _sai_route_attr_t
{
    /** READ-WRITE */

    /** Packet action [sai_packet_action_t]
       (default to SAI_PACKET_ACTION_FORWARD) */
    SAI_ROUTE_ATTR_PACKET_ACTION,

    /** Packet priority for trap/log actions [uint8_t]
       (default to 0) */
    SAI_ROUTE_ATTR_TRAP_PRIORITY,

    /** Next hop or next hop group id for the packet [sai_object_id_t]
     * The next hop id can be a generic next hop object, such as next hop,
     * next hop group. */
    SAI_ROUTE_ATTR_NEXT_HOP_ID,

    /** Is directly reachable route [bool] (CREATE_AND_SET)
     * Directly reachable routes are the IP subnets that are directly attached to the router.
       (default to false) */
    SAI_ROUTE_ATTR_DIRECTLY_REACHABLE_ROUTE,

    /** User based Meta Data
     * [sai_uint32_t] (CREATE_AND_SET)
     * Value Range SAI_SWITCH_ATTR_ROUTE_USER_META_DATA_RANGE */
    SAI_ROUTE_ATTR_META_DATA,

    /** Custom range base value */
    SAI_ROUTE_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_route_attr_t;


/**
 *  @brief Unicast route entry
 */
typedef struct _sai_unicast_route_entry_t
{
    sai_object_id_t vr_id;
    sai_ip_prefix_t destination;

} sai_unicast_route_entry_t;

/**
 * Routine Description:
 *    @brief Create Route
 *
 * Arguments:
 *    @param[in] unicast_route_entry - route entry
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 *
 * Note: IP prefix/mask expected in Network Byte Order.
 *
 */
typedef sai_status_t (*sai_create_route_fn)(
    _In_ const sai_unicast_route_entry_t* unicast_route_entry,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Remove Route
 *
 * Arguments:
 *    @param[in] unicast_route_entry - route entry
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 *
 * Note: IP prefix/mask expected in Network Byte Order.
 */
typedef sai_status_t (*sai_remove_route_fn)(
    _In_ const sai_unicast_route_entry_t* unicast_route_entry
    );

/**
 * Routine Description:
 *    @brief Set route attribute value
 *
 * Arguments:
 *    @param[in] unicast_route_entry - route entry
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_route_attribute_fn)(
    _In_ const sai_unicast_route_entry_t* unicast_route_entry,
    _In_ const sai_attribute_t *attr
    );

/**
 * Routine Description:
 *    @brief Get route attribute value
 *
 * Arguments:
 *    @param[in] unicast_route_entry - route entry
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_route_attribute_fn)(
    _In_ const sai_unicast_route_entry_t* unicast_route_entry,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );


/**
 *  @brief Router entry methods table retrieved with sai_api_query()
 */
typedef struct _sai_route_api_t
{
    sai_create_route_fn         create_route;
    sai_remove_route_fn         remove_route;
    sai_set_route_attribute_fn  set_route_attribute;
    sai_get_route_attribute_fn  get_route_attribute;

} sai_route_api_t;

/**
 * \}
 */
#endif // __SAIROUTE_H_
