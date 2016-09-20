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
*    sail2mc.h
*
* Abstract:
*
*    This module defines SAI L2MC API
*    L2MC: Layer2 Multicast (IP based)
*
*/

#if !defined (__SAIL2MC_H_)
#define __SAIL2MC_H_

#include <saitypes.h>

/** \defgroup SAIL2MC SAI - L2MC specific API definitions.
 *
 *  \{
 */
 
/**
 * @brief Enum defining l2mc entry types.
 */
typedef enum _sai_l2mc_entry_type_t
{
    /** L2MC entry with type (S,G) */
    SAI_L2MC_TYPE_SG = 0x00000001,

    /** L2MC entry with type (*,G) */
    SAI_L2MC_TYPE_XG = 0x00000002,

} sai_l2mc_entry_type_t;

/*
*  L2 multicast entry key
*/
typedef struct _sai_l2mc_entry_t
{
    sai_vlan_id_t vlan_id;
    sai_l2mc_entry_type_t type;
    sai_ip_address_t destination;
    sai_ip_address_t source;
} sai_l2mc_entry_t;

/*
*  Attribute Id for L2 multicast entry
*/
typedef enum _sai_l2mc_entry_attr_t 
{
    SAI_L2MC_ENTRY_ATTR_START,
    /** READ-ONLY */

    /** READ-WRITE */
    /** Packet action [sai_packet_action_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_L2MC_ENTRY_ATTR_PACKET_ACTION = SAI_L2MC_ENTRY_ATTR_START,

    /** Output group id for the packet [sai_object_id_t] (CREATE_AND_SET)
      * This attribute only takes effect when ATTR_PACKET_ACTION is set to FORWARD.
      * The group type should be SAI_GROUP_TYPE_L2MC. If the group has no member, packets will be discarded.
      * (MANDATORY_ON_CREATE when SAI_L2MC_ATTR_PACKET_ACTION == SAI_PACKET_ACTION_FORWARD) */
    SAI_L2MC_ENTRY_ATTR_OUTPUT_GROUP_ID,

    /* Custom range base value */
    SAI_L2MC_ENTRY_ATTR_CUSTOM_RANGE_BASE  = 0x10000000,

    /* --*/
    SAI_L2MC_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_l2mc_entry_attr_t;

/*
* Routine DescrL2tion:
*    Create L2 multicast entry
*
* Arguments:
*    [in] l2mc_entry - L2 multicast entry
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_create_l2mc_entry_fn)(
    _In_ const sai_l2mc_entry_t* l2mc_entry,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
);

/*
* Routine DescrL2tion:
*    Remove L2 multicast entry
*
* Arguments:
*    [in] l2mc_entry - L2 multicast entry
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_remove_l2mc_entry_fn)(
    _In_ const sai_l2mc_entry_t* l2mc_entry
    );

/*
* Routine DescrL2tion:
*    Set L2 multicast entry attribute value
*
* Arguments:
*    [in] L2 multicast - L2 multicast entry
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_l2mc_entry_attribute_fn)(
    _In_ const sai_l2mc_entry_t* l2mc_entry,
    _In_ const sai_attribute_t *attr
    );

/*
* Routine DescrL2tion:
*    Get L2 multicast entry attribute value
*
* Arguments:
*    [in] l2mc_entry - L2 multicast entry
*    [in] attr_count - number of attributes
*    [inout] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_l2mc_entry_attribute_fn)(
    _In_ const sai_l2mc_entry_t* l2mc_entry,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/*
* L2 multicast method table retrieved with sai_api_query()
*/
typedef struct _sai_l2mc_api_t
{
    sai_create_l2mc_entry_fn                     create_l2mc_entry;
    sai_remove_l2mc_entry_fn                     remove_l2mc_entry;
    sai_set_l2mc_entry_attribute_fn              set_l2mc_entry_attribute;
    sai_get_l2mc_entry_attribute_fn              get_l2mc_entry_attribute;
} sai_l2mc_api_t;

/**
 * \}
 */
#endif /* __SAIL2MC_H_ */
