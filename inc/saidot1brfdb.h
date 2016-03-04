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
* @file saidot1brfdb.h
*
* @brief This module defines SAI API for IEEE 802.1BR ECID based forwarding
         functionality. This is applicable only to Port Extenders (PE).
*************************************************************************************/

#if !defined (__SAIDOT1BRFDB_H)
#define __SAIDOT1BRFDB_H

#include "saitypes.h"
#include "saistatus.h"

/** \defgroup SAIDOT1BRFDBENTRY SAI - 802.1BR ECID based forwarding specific public APIs and datastructures in PE.
 *
 *  \{
 */

/**
 * @brief SAI attributes for SAI_OBJECT_TYPE_DOT1BR_FDB_ENTRY 
 */
typedef enum _sai_dot1br_fdb_entry_attr_t
{
    /** READ-WRITE */

    /** 802.1BR FDB entry ECID [sai_uint32_t] (MANDATORY_ON_CREATE)
     * The ECID for which the forwarding entry is to be created/set. Traffic
     * received on the Upstream Port in PE, containing this ECID, will be
     * forwarded to the the port/portlist specified by SAI_DOT1BR_FDB_ENTRY_ATTR_PORT_LIST. */
    
    SAI_DOT1BR_FDB_ENTRY_ECID,

    /** 802.1BR FDB entry port list [sai_object_list_t] (MANDATORY_ON_CREATE|CREATE_AND_SET)
     * The port id in the port list here can refer to a generic port object such as 
     * SAI port object id, SAI LAG object id but not SAI DOT1BR CB Extended Port.
     * If the ECID associated with the dot1br fdb entry is an unicast ECID, then the port list
     * MUST contain only one port.
     * If the ECID associated with the dot1br fdb entry is a multicast ECID, then the port 
     * list will overwrite the already present port list if any. */
     
    SAI_DOT1BR_FDB_ENTRY_ATTR_PORT_LIST,

    /* -- */

    /* Custom range base value */
    SAI_DOT1BR_FDB_ENTRY_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_dot1br_fdb_entry_attr_t;

/**
 * @brief Create a 802.1BR FDB entry.
 *
 * @param[out] dot1br_fdb_entry_id Dot1br FDB entry Object Id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_create_dot1br_fdb_entry_fn)(
    _Out_ sai_object_id_t *dot1br_fdb_entry_id,
    _In_  uint32_t attr_count,
    _In_  const sai_attribute_t *attr_list);

/**
 * @brief Remove 802.1BR FDB entry.
 *
 * @param[in] dot1br_fdb_entry_id Dot1br FDB entry Object Id
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_remove_dot1br_fdb_entry_fn)(
    _In_ sai_object_id_t dot1br_fdb_entry_id);

/**
 * @brief Set the attribute of the 802.1BR FDB entry.
 *
 * @param[in] dot1br_fdb_entry_id Dot1br FDB entry Object Id
 * @param[in] attr attribute value
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_set_dot1br_fdb_entry_attribute_fn)(
    _In_ sai_object_id_t dot1br_fdb_entry_id,
    _In_ const sai_attribute_t *attr);

/**
 * @brief Get the attribute of the 802.1BR FDB entry.
 *
 * @param[in] dot1br_fdb_entry_id Dot1br FDB entry Object Id
 * @param[in] attr_count number of the attributes
 * @param[inout] attr_list - array of attributes
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_get_dot1br_fdb_entry_attribute_fn)(
    _In_ sai_object_id_t dot1br_fdb_entry_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list);

/**
 * @brief SAI_OBJECT_TYPE_DOT1BR_FDB_ENTRY method table retrieved with sai_api_query().
 * This API is applicable only to PE.
 */
typedef struct _sai_dot1br_fdb_entry_api_t {
    sai_create_dot1br_fdb_entry_fn        create_dot1br_fdb_entry;
    sai_remove_dot1br_fdb_entry_fn        remove_dot1br_fdb_entry;
    sai_set_dot1br_fdb_entry_attribute_fn set_dot1br_fdb_entry_attribute;
    sai_get_dot1br_fdb_entry_attribute_fn get_dot1br_fdb_entry_attribute;
} sai_dot1br_fdb_entry_api_t;


/**
 * \}
 */
#endif // __SAIDOT1BRFDB_H

