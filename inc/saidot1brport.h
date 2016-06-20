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
* @file saidot1brport.h
*
* @brief This module defines SAI API for IEEE 802.1BR Port attributes.
************************************************************************/

#if !defined (__SAIDOT1BRPORT_H)
#define __SAIDOT1BRPORT_H

#include "saitypes.h"
#include "saistatus.h"

/** \defgroup SAIDOT1BRPORT SAI - 802.1BR Port specific public APIs and datastructures.
 *
 *  \{
 */

/**
 * @brief Attribute data for SAI_DOT1BR_PORT_ATTR_TYPE
 */
typedef enum _sai_dot1br_port_type_t
{
    SAI_DOT1BR_PORT_TYPE_NONE,
    SAI_DOT1BR_PORT_TYPE_UPSTREAM,
    SAI_DOT1BR_PORT_TYPE_CASCADE,
    SAI_DOT1BR_PORT_TYPE_ACCESS,
} sai_dot1br_port_type_t;

/**
 * @brief SAI attributes for SAI_OBJECT_TYPE_DOT1BR_PORT */
typedef enum _sai_dot1br_port_attr_t
{
    /** READ-WRITE */

    /** The Port to which the 802.1BR Port is mapped to [sai_object_id_t]  (MANDATORY_ON_CREATE|CREATE_ONLY).
     * Applicable only to Physical ports. */
    SAI_DOT1BR_PORT_ATTR_PORT,

    /** 802.1BR Port Type [sai_dot1br_port_type_t]
     * (MANDATORY_ON_CREATE|CREATE_ONLY).
     * Applicable only to Physical ports. */
    SAI_DOT1BR_PORT_ATTR_TYPE,

    /** 802.1BR Port default ECID [sai_uint32_t] (MANDATORY_ON_CREATE|CREATE_AND_SET).
     * This attribute is valid only if the attribute SAI_DOT1BR_PORT_ATTR_TYPE
     * is set to SAI_DOT1BR_PORT_TYPE_ACCESS.
     * ECID to be added on receiving dot1br untagged frames.
     * Applicable only to Physical ports. */
    SAI_DOT1BR_PORT_ATTR_ECID,

    /** 802.1BR Port default PCP [sai_uint8_t] (CREATE_AND_SET) (default to 0).
     * This attribute is valid only if the attribute SAI_DOT1BR_PORT_ATTR_TYPE
     * is set to SAI_DOT1BR_PORT_TYPE_ACCESS.
     * PCP to be added on receiving dot1br untagged frames.
     * Applicable only to Physical ports. */
    SAI_DOT1BR_PORT_ATTR_PCP,

    /** 802.1BR Port default DEI [sai_uint8_t] (CREATE_AND_SET) (default to 0).
     * This attribute is valid only if the attribute SAI_DOT1BR_PORT_ATTR_TYPE
     * is set to SAI_DOT1BR_PORT_TYPE_ACCESS.
     * DEI to be added on receiving dot1br untagged frames.
     * Applicable only to Physical ports. */
    SAI_DOT1BR_PORT_ATTR_DEI,

    /* -- */

    /* Custom range base value */
    SAI_DOT1BR_PORT_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_dot1br_port_attr_t;

/**
 * @brief Create a 802.1BR port.
 *
 * @param[out] dot1br_port_id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_create_dot1br_port_fn)(
    _Out_ sai_object_id_t *dot1br_port_id,
    _In_  uint32_t attr_count,
    _In_  const sai_attribute_t *attr_list);

/**
 * @brief Remove dot1br port.
 *
 * @param[in] dot1br_port_id Dot1BR Port object id.
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_remove_dot1br_port_fn)(
    _In_ sai_object_id_t dot1br_port_id);

/**
 * @brief Set the attribute of the Dot1BR Port.
 *
 * @param[in] dot1br_port_id Dot1BR Port object id.
 * @param[in] attr attribute value
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_set_dot1br_port_attribute_fn)(
    _In_ sai_object_id_t dot1br_port_id,
    _In_ const sai_attribute_t *attr);

/**
 * @brief Get the attribute of Extended Port.
 *
 * @param[in] dot1br_port_id Dot1BR Port object id.
 * @param[in] attr_count number of the attributes
 * @param[inout] attr_list array of attributes
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_get_dot1br_port_attribute_fn)(
    _In_ sai_object_id_t dot1br_port_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list);

/**
 * @brief SAI_OBJECT_TYPE_DOT1BR_PORT method table retrieved with sai_api_query()
 */
typedef struct _sai_dot1br_port_api_t {
    sai_create_dot1br_port_fn        create_dot1br_port;
    sai_remove_dot1br_port_fn        remove_dot1br_port;
    sai_set_dot1br_port_attribute_fn set_dot1br_port_attribute;
    sai_get_dot1br_port_attribute_fn get_dot1br_port_attribute;
} sai_dot1br_port_api_t;

/**
 * \}
 */
#endif // __SAIDOT1BRPORT_H
