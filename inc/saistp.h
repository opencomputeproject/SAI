/**
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
* saistp.h
*
* Abstract:
*
* This module defines SAI STP API
*
*/

#if !defined (__SAISTP_H_)
#define __SAISTP_H_

#include "saitypes.h"
#include "saistatus.h"

/** \defgroup SAISTP SAI - STP specific public APIs and datastructures. 
 *
 *  \{
 */

/**  
 * Datastructure for stp port state 
 */  
typedef enum _sai_port_stp_port_state_t 
{  
    /** Port is in Learning mode*/  
    SAI_PORT_STP_STATE_LEARNING,  

    /** Port is in Forwarding mode */  
    SAI_PORT_STP_STATE_FORWARDING,  

    /** Port is in Blocking mode */  
    SAI_PORT_STP_STATE_BLOCKING,  

} sai_port_stp_port_state_t; 

/**
 * @brief SAI attributes for STP
 */
typedef enum _sai_stp_attr_t
{
    /** READ-ONLY */

    /** Vlans attached to STP instance [sai_vlan_list_t] */
    SAI_STP_ATTR_VLAN,

    /** READ-WRITE */

} sai_stp_attr_t;

/**
 * @brief Create stp instance with default port state as forwarding.
 *
 * @param[out] stp_id stp instance id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
typedef sai_status_t (*sai_create_stp_fn)(
    _Out_ sai_object_id_t *stp_id,
    _In_  uint32_t attr_count,
    _In_  const sai_attribute_t *attr_list);


/**
 * @brief Remove stp instance.
 *
 * @param[in] stp_id stp instance id
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
typedef sai_status_t (*sai_remove_stp_fn)(
    _In_ sai_object_id_t stp_id);

/**
 * @brief Update stp state of a port in specified stp instance.
 *
 * @param[in] stp_id stp instance id
 * @param[in] port_id port id
 * @param[in] stp_port_state stp state of the port
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
typedef sai_status_t (*sai_set_stp_port_state_fn)(
    _In_ sai_object_id_t stp_id,     
    _In_ sai_object_id_t port_id,   
    _In_ sai_port_stp_port_state_t stp_port_state);

/**
 * @brief Retrieve stp state of a port in specified stp instance.
 *
 * @param[in] stp_id stp instance id
 * @param[in] port_id port id
 * @param[out] stp_port_state stp state of the port
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
typedef sai_status_t (*sai_get_stp_port_state_fn)(
    _In_ sai_object_id_t stp_id,     
    _In_ sai_object_id_t port_id,   
    _Out_ sai_port_stp_port_state_t  *stp_port_state);

/**
 * @brief Set the attribute of STP instance.
 *
 * @param[in] stp_id stp instance id
 * @param[in] attr attribute value
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
typedef sai_status_t (*sai_set_stp_attribute_fn)(
    _In_ sai_object_id_t stp_id,
    _In_ const sai_attribute_t *attr);

/**
 * @brief Get the attribute of STP instance.
 *
 * @param[in] stp_id stp instance id
 * @param[in] attr_count number of the attribute
 * @param[in] attr_list attribute value
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
typedef sai_status_t (*sai_get_stp_attribute_fn)(
    _In_ sai_object_id_t stp_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list);

/**
 * @brief STP method table retrieved with sai_api_query()
 */
typedef struct _sai_stp_api_t {
    sai_create_stp_fn           create_stp;
    sai_remove_stp_fn           remove_stp;
    sai_set_stp_attribute_fn    set_stp_attribute;
    sai_get_stp_attribute_fn    get_stp_attribute;
    sai_set_stp_port_state_fn   set_stp_port_state;
    sai_get_stp_port_state_fn   get_stp_port_state;
} sai_stp_api_t;


/**
 * \}
 */
#endif // __SAISTP_H_
