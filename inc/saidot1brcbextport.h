/*
* Copyright (c) 2015 Dell Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License"); you may
* not use this file except in compliance with the License. You may obtain
* a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
* THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
* CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
* LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
* FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
*
* See the Apache Version 2.0 License for specific language governing
* permissions and limitations under the License.
*
* @file saidot1brcbextport.h
*
* @brief This module defines the instantiation of remote Port Extender (PE) ports,
*        as extended ports in the Controlling Bridge (CB).
*        This is applicable only to CB.
*************************************************************************************/

#if !defined (__SAIDOT1BRCBEXTPORT_H)
#define __SAIDOT1BRCBEXTPORT_H

#include "saitypes.h"
#include "saistatus.h"

/** \defgroup SAIDOT1BRCBEXTPORT SAI - 802.1BR Extension Port specific public APIs and datastructures in CB.
 *
 *  \{
 */

/**
 * @brief SAI attributes for SAI_OBJECT_TYPE_DOT1BR_CB_EXTENDED_PORT 
 */
typedef enum _sai_dot1br_cb_extended_port_attr_t
{
    /** READ-WRITE */

    /** Cascading Port in the Controlling Bridge [sai_object_id_t]
     * (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_DOT1BR_CB_EXTENDED_PORT_ATTR_CASCADING_PORT,

    /** E-Channel Id (ECID) of the Extended Port [sai_uint32_t]
     * (MANDATORY_ON_CREATE|CREATE_AND_SET */
    SAI_DOT1BR_CB_EXTENDED_PORT_ATTR_ECID,

    /* -- */

    /* Custom range base value */
    SAI_DOT1BR_CB_EXTENDED_PORT_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_dot1br_cb_extended_port_attr_t;

/**
 * @brief Create a 802.1BR extended port. This API is applicable only to Controlling Bridge.
 *
 * @param[out] extended_port_id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_create_cb_extended_port_fn)(
    _Out_ sai_object_id_t *extended_port_id,
    _In_  uint32_t attr_count,
    _In_  const sai_attribute_t *attr_list);

/**
 * @brief Remove extended port.
 *
 * @param[in] extended_port_id Extended Port object id.
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_remove_cb_extended_port_fn)(
    _In_ sai_object_id_t extended_port_id);

/**
 * @brief Set the attribute of the Extended Port.
 *
 * @param[in] extended_port_id Extended Port object id.
 * @param[in] attr attribute value
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_set_cb_extended_port_attribute_fn)(
    _In_ sai_object_id_t extended_port_id,
    _In_ const sai_attribute_t *attr);

/**
 * @brief Get the attribute of Extended Port.
 *
 * @param[in] extended_port_id Extended Port object id.
 * @param[in] attr_count number of the attributes
 * @param[inout] attr_list array of attributes
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_get_cb_extended_port_attribute_fn)(
    _In_ sai_object_id_t extended_port_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list);

/**
 * @brief SAI_OBJECT_TYPE_DOT1BR_CB_EXTENDED_PORT  method table retrieved with sai_api_query()
 */
typedef struct _sai_dot1br_extended_port_api_t {
    sai_create_cb_extended_port_fn        create_cb_extended_port;
    sai_remove_cb_extended_port_fn        remove_cb_extended_port;
    sai_set_cb_extended_port_attribute_fn set_cb_extended_port_attribute;
    sai_get_cb_extended_port_attribute_fn get_cb_extended_port_attribute;
} sai_dot1br_cb_extended_port_api_t;


/**
 * \}
 */
#endif // __SAIDOT1BRCBEXTPORT_H
