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
*    saivlan.h
*
* Abstract:
*
*    This module defines SAI VLAN API
*
*/

#if !defined (__SAIVLAN_H_)
#define __SAIVLAN_H_

#include <saitypes.h>

/** \defgroup SAIVLAN SAI - VLAN specific API definitions.
 *
 *  \{
 */
 
#define VLAN_COUNTER_SET_DEFAULT    0

/**
 *  @brief Attribute data for tagging_mode parameter
 */
typedef enum _sai_vlan_tagging_mode_t
{
    SAI_VLAN_PORT_UNTAGGED,

    SAI_VLAN_PORT_TAGGED,

    SAI_VLAN_PORT_PRIORITY_TAGGED

} sai_vlan_tagging_mode_t;

/**
 *  @brief Attribute Id in sai_set_vlan_attribute() and
 *  sai_get_vlan_attribute() calls
 */
typedef enum _sai_vlan_attr_t
{

    SAI_VLAN_ATTR_START,    

    /** READ-ONLY */

    /** Switch Object ID [sai_object_id_t] (CREATE_ONLY),
     * Default SAI_NULL_OBJECT_ID */
    SAI_VLAN_ATTR_SWITCH_ID,

    /** vlan id [sai_vlan_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_VLAN_ATTR_VLAN_ID,

    /** List of vlan members in a VLAN [sai_object_list_t]*/
    SAI_VLAN_ATTR_MEMBER_LIST = SAI_VLAN_ATTR_START,

    /** READ-WRITE */

    /** STP Instance that the VLAN is associated to [sai_object_id_t]
      * (default to default stp instance id)*/
    SAI_VLAN_ATTR_STP_INSTANCE,


    /** Maximum number of learned MAC addresses [uint32_t]
      * zero means learning limit disable. (default to zero).
      */
    SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES,

    /** STP Instance that the VLAN is associated to [sai_object_id_t]
      * (default to default stp instance id)*/
    SAI_VLAN_ATTR_STP_INSTANCE,

    /** To disable learning on a VLAN. [bool] (CREATE_AND_SET)
      * (default set to false)
      * This should override port learn settings. If this is set to true on a vlan,
      * then the source mac learning is disabled for this vlan on a member port even
      * if learn is enable on the port(based on port learn attribute)
      */
    SAI_VLAN_ATTR_LEARN_DISABLE,

    /** User based Meta Data
      * [sai_uint32_t] (CREATE_AND_SET)
      * Value Range SAI_SWITCH_ATTR_VLAN_USER_META_DATA_RANGE */
    SAI_VLAN_ATTR_META_DATA,

    SAI_VLAN_ATTR_END,

    /** Custom range base value */
    SAI_VLAN_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /* --*/
    SAI_VLAN_ATTR_CUSTOM_RANGE_END


} sai_vlan_attr_t;


/*
    \brief List of VLAN Member Attributes
*/
typedef enum _sai_vlan_member_attr_t {

    SAI_VLAN_MEMBER_ATTR_START,

    /** READ_WRITE */

    /** VLAN ID [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_VLAN_MEMBER_ATTR_VLAN_ID = SAI_VLAN_MEMBER_ATTR_START,

    /** logical port ID [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_VLAN_MEMBER_ATTR_PORT_ID,

    /** VLAN tagging mode [sai_vlan_tagging_mode_t] (CREATE_AND_SET)
     * (default to SAI_VLAN_PORT_UNTAGGED) */
    SAI_VLAN_MEMBER_ATTR_TAGGING_MODE,

    SAI_VLAN_MEMBER_ATTR_END,

    /** custom range base value */
    SAI_VLAN_MEMBER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /* --*/
    SAI_VLAN_MEMBER_ATTR_CUSTOM_RANGE_END


} sai_vlan_member_attr_t;



/**
 *  @brief VLAN counter IDs in sai_get_vlan_stat_counters() call
 */
typedef enum _sai_vlan_stat_counter_t
{
    SAI_VLAN_STAT_IN_OCTETS,
    SAI_VLAN_STAT_IN_PACKETS,
    SAI_VLAN_STAT_IN_UCAST_PKTS,
    SAI_VLAN_STAT_IN_NON_UCAST_PKTS,
    SAI_VLAN_STAT_IN_DISCARDS,
    SAI_VLAN_STAT_IN_ERRORS,
    SAI_VLAN_STAT_IN_UNKNOWN_PROTOS,
    SAI_VLAN_STAT_OUT_OCTETS,
    SAI_VLAN_STAT_OUT_PACKETS,
    SAI_VLAN_STAT_OUT_UCAST_PKTS,
    SAI_VLAN_STAT_OUT_NON_UCAST_PKTS,
    SAI_VLAN_STAT_OUT_DISCARDS,
    SAI_VLAN_STAT_OUT_ERRORS,
    SAI_VLAN_STAT_OUT_QLEN

} sai_vlan_stat_counter_t;

/**
 * Routine Description:
 *    @brief Create a VLAN Object
 *
 * Arguments:
 *    @param[out] vlan_obj_id - Vlan object id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_create_vlan_fn)(
     _Out_ sai_object_id_t* vlan_obj_id,
     _In_ uint32_t attr_count,
     _In_ const sai_attribute_t *attr_list
   );

/**
 * Routine Description:
 *    @brief Remove a VLAN
 *
 * Arguments:
 *    @param[in] vlan_obj_id - VLAN id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_remove_vlan_fn)(
    _In_ sai_object_id_t vlan_obj_id
    );

/**
 * Routine Description:
 *    @brief Set VLAN attribute Value
 *
 * Arguments:
 *    @param[in] vlan_id - VLAN id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_vlan_attribute_fn)(
    _In_ sai_object_id_t vlan_id,
    _In_ const sai_attribute_t  *attr
    );

/**
 * Routine Description:
 *    @brief Get VLAN attribute Value
 *
 * Arguments:
 *    @param[in] vlan_id - VLAN id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_vlan_attribute_fn)(
    _In_ sai_object_id_t vlan_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/*
    \brief Create VLAN Member
    \param[out] vlan_member_id VLAN member ID
    \param[in] attr_count number of attributes
    \param[in] attr_list array of attributes
    \return Success: SAI_STATUS_SUCCESS
            Failure: failure status code on error
*/
typedef sai_status_t (*sai_create_vlan_member_fn)(
    _Out_ sai_object_id_t* vlan_member_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/*
    \brief Remove VLAN Member
    \param[in] vlan_member_id VLAN member ID
    \return Success: SAI_STATUS_SUCCESS
            Failure: failure status code on error
*/
typedef sai_status_t (*sai_remove_vlan_member_fn)(
    _In_ sai_object_id_t vlan_member_id
    );

/*
    \brief Set VLAN Member Attribute
    \param[in] vlan_member_id VLAN member ID
    \param[in] attr attribute structure containing ID and value
    \return Success: SAI_STATUS_SUCCESS
            Failure: failure status code on error
*/
typedef sai_status_t (*sai_set_vlan_member_attribute_fn)(
    _In_ sai_object_id_t vlan_member_id,
    _In_ const sai_attribute_t *attr
    );

/*
    \brief Get VLAN Member Attribute
    \param[in] vlan_member_id VLAN member ID
    \param[in] attr_count number of attributes
    \param[in,out] attr_list list of attribute structures containing ID and value
    \return Success: SAI_STATUS_SUCCESS
            Failure: failure status code on error
*/
typedef sai_status_t (*sai_get_vlan_member_attribute_fn)(
    _In_ sai_object_id_t vlan_member_id,
    _In_ const uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *   @brief Get vlan statistics counters.
 *
 * Arguments:
 *    @param[in] vlan_id - VLAN id
 *    @param[in] counter_ids - specifies the array of counter ids
 *    @param[in] number_of_counters - number of counters in the array
 *    @param[out] counters - array of resulting counter values.
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_vlan_stats_fn)(
    _In_ sai_object_id_t vlan_id,
    _In_ const sai_vlan_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters,
    _Out_ uint64_t* counters
    );

/**
 * Routine Description:
 *   @brief Clear vlan statistics counters.
 *
 * Arguments:
 *    @param[in] vlan_id - vlan id
 *    @param[in] counter_ids - specifies the array of counter ids
 *    @param[in] number_of_counters - number of counters in the array
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_clear_vlan_stats_fn)(
    _In_ sai_object_id_t vlan_id,
    _In_ const sai_vlan_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters
    );

/**
 * @brief VLAN methods table retrieved with sai_api_query()
 */
typedef struct _sai_vlan_api_t
{
    sai_create_vlan_fn                  create_vlan;
    sai_remove_vlan_fn                  remove_vlan;
    sai_set_vlan_attribute_fn           set_vlan_attribute;
    sai_get_vlan_attribute_fn           get_vlan_attribute;
    sai_create_vlan_member_fn           create_vlan_member;
    sai_remove_vlan_member_fn           remove_vlan_member;
    sai_set_vlan_member_attribute_fn    set_vlan_member_attribute;
    sai_get_vlan_member_attribute_fn    get_vlan_member_attribute;
    sai_get_vlan_stats_fn               get_vlan_stats;
    sai_clear_vlan_stats_fn             clear_vlan_stats;

} sai_vlan_api_t;

/**
 *\}
 */
#endif // __SAIVLAN_H_
