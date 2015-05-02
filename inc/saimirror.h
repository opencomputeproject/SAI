/*
* Copyright (c) 2015 Dell Inc.
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
*/
/**
* Module Name:
*
* saimirror.h
*
* Abstract:
*
* This module defines SAI Port mirror Interface
*
*/

#if !defined (__SAIMIRROR_H_)
#define __SAIMIRROR_H_

#include "saitypes.h"
#include "saistatus.h"

/** \defgroup SAIMIRROR SAI - Mirror specific public APIs and datastructures
 *
 *  \{
 */

/**
 * @brief SAI type of mirroring
 */
typedef enum _sai_mirror_type_t
{
    /** Local span */
    SAI_MIRROR_TYPE_LOCAL = 1,

    /** Remote span */
    SAI_MIRROR_TYPE_REMOTE,

    /** Enhanced Remote span */
    SAI_MIRROR_TYPE_ENHANCED_REMOTE,

} sai_mirror_type_t;

/**
 * @brief SAI type of encapsulation for RSPAN and ERSPAN
 */
typedef enum _sai_erspan_encapsulation_type_t
{
    /** L3 GRE Tunnel Encapsulation    
      | L2 Ethernet header | IP header | GRE header | Original mirrored packet |
     */
    SAI_MIRROR_L3_GRE_TUNNEL,

} sai_erspan_encapsulation_type_t;

/**
 * @brief SAI attributes for mirror session
 */
typedef enum _sai_mirror_session_attr_t
{
    /** READ_ONLY */

    /** READ_WRITE */

    /** MANDATORY_ON_CREATE|CREATE_ONLY */
    /** Mirror type SPAN/RSPAN/ERSPAN [sai_mirror_type_t]*/
    SAI_MIRROR_SESSION_ATTR_TYPE,

    /** MANDATORY_ON_CREATE|CREATE_AND_SET */
    /** Destination/Analyser/Monitor Port [sai_object_id_t]*/
    SAI_MIRROR_SESSION_ATTR_MONITOR_PORT,

    /** CREATE_AND_SET */
    /** Class-of-Service (Traffic Class) - [uint8_t],Default is 0*/
    SAI_MIRROR_SESSION_ATTR_TC,

    /** MANDATORY_ON_CREATE|CREATE_AND_SET */
    /** Valid for RSPAN and ERSPAN
     * L2 header TPID if vlanId is not zero - [uint16_t]*/
    SAI_MIRROR_SESSION_ATTR_VLAN_TPID,

    /** MANDATORY_ON_CREATE|CREATE_AND_SET */
    /** Valid for RSPAN and ERSPAN L2 header VlanId - [sai_vlan_id_t]*/
    SAI_MIRROR_SESSION_ATTR_VLAN_ID,

    /** MANDATORY_ON_CREATE|CREATE_AND_SET */
    /** Valid for RSPAN and ERSPAN packet priority - [uint8_t] */
    SAI_MIRROR_SESSION_ATTR_VLAN_PRI,

    /** All attributes below are Valid only for ERSPAN 
        [SAI_MIRROR_TYPE_ENHANCED_REMOTE]*/

    /** MANDATORY_ON_CREATE|CREATE_ONLY */
    /** Encapsulation type - sai_erspan_encapsulation_type_t */
    SAI_MIRROR_SESSION_ATTR_ENCAP_TYPE,

    /** MANDATORY_ON_CREATE|CREATE_AND_SET */
    /** tunnel IP header version - [uint8_t]*/
    SAI_MIRROR_SESSION_ATTR_IPHDR_VERSION,

    /** MANDATORY_ON_CREATE|CREATE_AND_SET */
    /** tunnel header TOS - [uint8_t]*/
    SAI_MIRROR_SESSION_ATTR_TOS,

    /** CREATE_AND_SET */
    /** tunnel header TTL - [uint8_t],default 255*/
    SAI_MIRROR_SESSION_ATTR_TTL,

    /** MANDATORY_ON_CREATE|CREATE_AND_SET */
    /** tunnel source IP - [sai_ip_address_t] */
    SAI_MIRROR_SESSION_ATTR_SRC_IP_ADDRESS,

    /** MANDATORY_ON_CREATE|CREATE_AND_SET */
    /** tunnel destination IP - [sai_ip_address_t] */
    SAI_MIRROR_SESSION_ATTR_DST_IP_ADDRESS,

    /** MANDATORY_ON_CREATE|CREATE_AND_SET */
    /** L2 source MAC address - [sai_mac_t] */
    SAI_MIRROR_SESSION_ATTR_SRC_MAC_ADDRESS,

    /** MANDATORY_ON_CREATE|CREATE_AND_SET */
    /** L2 destination MAC address - [sai_mac_t] */
    SAI_MIRROR_SESSION_ATTR_DST_MAC_ADDRESS,

    /** MANDATORY_ON_CREATE|CREATE_AND_SET */
    /** GRE protocol Id - [uint16_t] */
    SAI_MIRROR_SESSION_ATTR_GRE_PROTOCOL_TYPE,

} sai_mirror_session_attr_t;

/**
 * @brief Create mirror session.
 *
 * @param[out] session_id Port mirror session id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
typedef sai_status_t (*sai_create_mirror_session_fn)(
        _Out_ sai_object_id_t *session_id,
        _In_  uint32_t attr_count,
        _In_  const sai_attribute_t *attr_list);


/**
 * @brief Remove mirror session.
 *
 * @param[in] session_id Port mirror session id
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
typedef sai_status_t (*sai_remove_mirror_session_fn)(
        _In_ sai_object_id_t session_id);

/**
 * @brief Set mirror session attributes.
 *
 * @param[in] session_id Port mirror session id
 * @param[in] attr Value of attribute
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
typedef sai_status_t (*sai_set_mirror_session_attribute_fn)(
        _In_ sai_object_id_t session_id,
        _In_ const  sai_attribute_t *attr);

/**
 * @brief Get mirror session attributes.
 *
 * @param[in] session_id Port mirror session id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Value of attribute
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
typedef sai_status_t (*sai_get_mirror_session_attribute_fn)(
        _In_ sai_object_id_t session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);


/**
 * @brief MIRROR method table retrieved with sai_api_query()
 */
typedef struct _sai_mirror_api_t
{
    sai_create_mirror_session_fn create_mirror_session;
    sai_remove_mirror_session_fn remove_mirror_session;
    sai_set_mirror_session_attribute_fn set_mirror_session_attribute;
    sai_get_mirror_session_attribute_fn get_mirror_session_attribute;
} sai_mirror_api_t;

/**
 * \}
 */

#endif // __SAIMIRROR_H_

