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
*    saiipmc.h
*
* Abstract:
*
*    This module defines SAI IPMC API
*    IPMC: Layer3 Multicast
*
*/

#if !defined (__SAIIPMC_H_)
#define __SAIIPMC_H_

#include <saitypes.h>

/** \defgroup SAIIPMC SAI - IPMC specific API definitions.
 *
 *  \{
 */
 
/**
 * @brief Enum defining ipmc entry types.
 */
typedef enum _sai_ipmc_entry_type_t
{
    /** IPMC entry with type (S,G) */
    SAI_IPMC_ENTRY_TYPE_SG = 0x00000001,

    /** IPMC entry with type (*,G) */
    SAI_IPMC_ENTRY_TYPE_XG = 0x00000002,

} sai_ipmc_entry_type_t;

/*
*  IP multicast entry key
*/
typedef struct _sai_ipmc_entry_t
{
    sai_object_id_t vrf_id;
    sai_ipmc_entry_type_t type;
    sai_ip_address_t destination;
    sai_ip_address_t source;
} sai_ipmc_entry_t;

/*
*  Attribute Id for IP multicast entry
*/
typedef enum _sai_ipmc_entry_attr_t 
{
    SAI_IPMC_ENTRY_ATTR_START,
    /** READ-ONLY */

    /** READ-WRITE */

    /** Packet action [sai_packet_action_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_IPMC_ENTRY_ATTR_PACKET_ACTION = SAI_IPMC_ENTRY_ATTR_START,

    /** Packet priority for trap/log actions [uint8_t] (CREATE_AND_SET)
      * (default to 0) */
    SAI_IPMC_ENTRY_ATTR_TRAP_PRIORITY,

    /** Output group id for the packet [sai_object_id_t] (CREATE_AND_SET)
      * The generic group type should be SAI_GROUP_TYPE_IPMC.
      * If the group has no member, packets will be discarded.
      * (MANDATORY_ON_CREATE when SAI_IPMC_ATTR_PACKET_ACTION == SAI_PACKET_ACTION_FORWARD) */
    SAI_IPMC_ENTRY_ATTR_OUTPUT_GROUP_ID,

    /** RPF interface group id for the packet [sai_object_id_t] (CREATE_AND_SET)
      * The group type should be SAI_GROUP_TYPE_RPF.
      * If not set or the group has no member, RPF checking will be disabled */
    SAI_L2MC_ENTRY_ATTR_RPF_GROUP_ID,

    /* Custom range base value */
    SAI_IPMC_ENTRY_ATTR_CUSTOM_RANGE_BASE  = 0x10000000,

    /* --*/
    SAI_IPMC_ENTRY_ATTR_CUSTOM_RANGE_END
    
} sai_ipmc_entry_attr_t;

/*
* Routine Description:
*    Create IP multicast entry
*
* Arguments:
*    [in] ipmc_entry - IP multicast entry
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_ipmc_entry_fn)(
    _In_ const sai_ipmc_entry_t* ipmc_entry,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
);

/*
* Routine Description:
*    Remove IP multicast entry
*
* Arguments:
*    [in] ipmc_entry - IP multicast entry
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_remove_ipmc_entry_fn)(
    _In_ const sai_ipmc_entry_t* ipmc_entry
    );

/*
* Routine Description:
*    Set IP multicast entry attribute value
*
* Arguments:
*    [in] IP multicast - IP multicast entry
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_ipmc_entry_attribute_fn)(
    _In_ const sai_ipmc_entry_t* ipmc_entry,
    _In_ const sai_attribute_t *attr
    );

/*
* Routine Description:
*    Get IP multicast entry attribute value
*
* Arguments:
*    [in] ipmc_entry - IP multicast entry
*    [in] attr_count - number of attributes
*    [inout] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_ipmc_entry_attribute_fn)(
    _In_ const sai_ipmc_entry_t* ipmc_entry,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/*
* IP multicast method table retrieved with sai_api_query()
*/
typedef struct _sai_ipmc_api_t
{
    sai_create_ipmc_entry_fn                     create_ipmc_entry;
    sai_remove_ipmc_entry_fn                     remove_ipmc_entry;
    sai_set_ipmc_entry_attribute_fn              set_ipmc_entry_attribute;
    sai_get_ipmc_entry_attribute_fn              get_ipmc_entry_attribute;
} sai_ipmc_api_t;

/**
 * \}
 */
#endif /* __SAIIPMC_H_ */
