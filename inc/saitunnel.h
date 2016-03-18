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
*    saitunnel.h
*
* Abstract:
*
*    This module defines SAI Tunnel API
*
*/

#if !defined (__SAITUNNEL_H_)
#define __SAITUNNEL_H_

#include <saitypes.h>

/** \defgroup SAITUNNEL SAI - Next hop specific API definitions.
 *
 *  \{
 */

/**
 * @brief Enum defining tunnel map types.
 */
typedef enum _sai_tunnel_map_type_t
{
    /** TUNNEL Map overlay ECN to underlay ECN   */
    SAI_TUNNEL_MAP_OECN_TO_UECN = 0x00000001,

    /** TUNNEL Map underlay ECN to overlay ECN   */
    SAI_TUNNEL_MAP_UECN_TO_OECN = 0x00000002,

    /** TUNNEL Map VNI to VLAN ID  */
    SAI_TUNNEL_MAP_VNI_TO_VLAN_ID = 0x00000003,

    /** TUNNEL Map VLAN ID to VNI */
    SAI_TUNNEL_MAP_VLAN_ID_TO_VNI = 0x00000004,

    /* -- */
    /* Custom range base value */
    SAI_TUNNEL_MAP_CUSTOM_RANGE_BASE = 0x10000000

} sai_tunnel_map_type_t;

typedef enum _sai_tunnel_map_attr_t
{
    /** tunnel Map type [sai_tunnel_map_type_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_TUNNEL_MAP_ATTR_TYPE = 0x00000000,

    /** tunnel mapper  [sai_tunnel_map_list_t] */
    SAI_TUNNEL_MAP_ATTR_MAP_TO_VALUE_LIST = 0x00000001,

    /* -- */
    /* Custom range base value */
    SAI_TUNNEL_MAP_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_tunnel_map_attr_t;

/**
 * @brief Create tunnel Map
 *
 * @param[out] tunnel_map_id tunnel Map Id
 * @param[in] attr_count number of attributes
 * @param[in] attr_list array of attributes
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
typedef sai_status_t(*sai_create_tunnel_map_fn)(
    _Out_ sai_object_id_t* tunnel_map_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * @brief Remove tunnel Map
 *
 *  @param[in] tunnel_map_id tunnel Map id to be removed.
 *
 *  @return  SAI_STATUS_SUCCESS on success
 *           Failure status code on error
 */
typedef sai_status_t(*sai_remove_tunnel_map_fn) (
    _In_  sai_object_id_t   tunnel_map_id
    );

/**
 * @brief Set attributes for tunnel map
 *
 * @param[in] tunnel_map_id tunnel Map Id
 * @param[in] attr attribute to set
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
typedef sai_status_t(*sai_set_tunnel_map_attribute_fn)(
    _In_ sai_object_id_t  tunnel_map_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief  Get attrbutes of tunnel map
 *
 * @param[in] tunnel_map_id  tunnel map id
 * @param[in] attr_count  number of attributes
 * @param[inout] attr_list  array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success
 *        Failure status code on error
 */
typedef sai_status_t(*sai_get_tunnel_map_attribute_fn)(
    _In_ sai_object_id_t   tunnel_map_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

typedef enum _sai_tunnel_type_t
{
    SAI_TUNNEL_IPINIP,

    SAI_TUNNEL_IPINIP_GRE,

    SAI_TUNNEL_VXLAN,

    SAI_TUNNEL_MPLS,

} sai_tunnel_type_t;

typedef enum _sai_tunnel_ttl_mode_t
{
    SAI_TUNNEL_TTL_COPY_FROM_INNER,

    SAI_TUNNEL_TTL_USER_DEFINE

} sai_tunnel_ttl_mode_t;

typedef enum _sai_tunnel_dscp_mode_t
{
    SAI_TUNNEL_DSCP_COPY_FROM_INNER,

    SAI_TUNNEL_DSCP_USER_DEFINE

} sai_tunnel_dscp_mode_t;

typedef enum _sai_tunnel_ecn_mode_t
{
    SAI_TUNNEL_ECN_MODE_COPY_FROM_OUTER,

    SAI_TUNNEL_ECN_MODE_KEEP_INNER,

    SAI_TUNNEL_ECN_MODE_USER_DEFINED
} sai_tunnel_ecn_mode_t;

typedef enum _sai_tunnel_attr_t
{
    /** READ-WRITE */

    /** tunnel type [sai_tunnel_type_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_TUNNEL_ATTR_TYPE,

    /** tunnel underlay interface [sai_object_id_t] */
    SAI_TUNNEL_ATTR_UNDERLAY_INTERFACE,

    /** tunnel overlay interafce [sai_object_id_t] */
    SAI_TUNNEL_ATTR_OVERLAY_INTERFACE,

    /** tunnel encap attribute*/

    /** tunnel src ip [sai_ip_address_t] (CREATE_ONLY) */
    SAI_TUNNEL_ATTR_ENCAP_SRC_IP,

    /** tunnel TTL mode copy from inner or user define [sai_tunnel_ttl_mode_t]
     *  (CREATE_ONLY) */
    SAI_TUNNEL_ATTR_ENCAP_TTL_MODE,

    /** tunnel TTL value [sai_uint8_t]
     *  (MANDATORY_ON_CREATE when SAI_TUNNEL_ENCAP_TTL_MODE = SAI_TUNNEL_TTL_USER_DEFINE) */
    SAI_TUNNEL_ATTR_ENCAP_TTL_VAL,

    /** tunnel dscp mode (pipe or uniform model) [sai_tunnel_dscp_mode_t]
     *  (CREATE_ONLY) */
    SAI_TUNNEL_ATTR_ENCAP_DSCP_MODE,

    /** tunnel DSCP value [sai_uint8_t : 6]
     *  (MANDATORY_ON_CREATE when SAI_TUNNEL_ENCAP_DSCP_MODE = SAI_TUNNEL_DSCP_USER_DEFINE) */
    SAI_TUNNEL_ATTR_ENCAP_DSCP_VAL,

    /** tunnel GRE key valid [bool] (CREATE_ONLY) */
    SAI_TUNNEL_ATTR_ENCAP_GRE_KEY_VALID,

    /** tunnel GRE key [sai_uint32_t] (MANDATORY_ON_CREATE when
     *  SAI_TUNNEL_ATTR_ENCAP_GRE_KEY_VALID=true) (CREATE_ONLY) */
    SAI_TUNNEL_ATTR_ENCAP_GRE_KEY,

    /**  tunnel encap ECN mode [sai_tunnel_ecn_mode_t] */
    SAI_TUNNEL_ATTR_ENCAP_ECN_MODE,

    /** tunnel encap mappers [sai_object_list_t] */
    SAI_TUNNEL_ATTR_ENCAP_MAPPERS,

    /** tunnel decap attribute **/

    /**  tunnel decap ECN mode [sai_tunnel_ecn_mode_t] */
    SAI_TUNNEL_ATTR_DECAP_ECN_MODE,

    /**  tunnel decap mappers [sai_object_list_t] */
    SAI_TUNNEL_ATTR_DECAP_MAPPERS,

    /** tunnel TTL mode copy from inner or user define [sai_tunnel_ttl_mode_t]
    *  (MANDATORY_ON_CREATE when SAI_TUNNEL_ATTR_TYPE=SAI_TUNNEL_IPINIP,SAI_TUNNEL_IPINIP_GRE)
    *  (CREATE_ONLY) */
    SAI_TUNNEL_ATTR_DECAP_TTL_MODE,

    /** tunnel TTL value [sai_uint8_t]
    *  (MANDATORY_ON_CREATE when SAI_TUNNEL_DECAP_TTL_MODE = SAI_TUNNEL_TTL_USER_DEFINE) */
    SAI_TUNNEL_ATTR_DECAP_TTL_VAL,

    /** tunnel dscp mode (pipe or uniform model) [sai_tunnel_dscp_mode_t]
    *  (MANDATORY_ON_CREATE when SAI_TUNNEL_ATTR_TYPE=SAI_TUNNEL_IPINIP,SAI_TUNNEL_IPINIP_GRE)
    *  (CREATE_ONLY) */
    SAI_TUNNEL_ATTR_DECAP_DSCP_MODE,

    /** tunnel DSCP value [sai_uint8_t : 6]
    *  (MANDATORY_ON_CREATE when SAI_TUNNEL_DECAP_DSCP_MODE = SAI_TUNNEL_DSCP_USER_DEFINE) */
    SAI_TUNNEL_ATTR_DECAP_DSCP_VAL,

    /** Custom range base value */
    SAI_TUNNEL_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_tunnel_attr_t;

/**
 * Routine Description:
 *    @brief Create tunnel
 *
 * Arguments:
 *    @param[out] tunnel_id - tunnel id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 *
 */
typedef sai_status_t (*sai_create_tunnel_fn)(
    _Out_ sai_object_id_t* tunnel_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Remove tunnel
 *
 * Arguments:
 *    @param[in] tunnel_id – tunnel id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_remove_tunnel_fn)(
    _In_ sai_object_id_t tunnel_id
    );

/**
 * Routine Description:
 *    @brief Set tunnel attribute
 *
 * Arguments:
 *    @param[in] tunnel_id - tunnel id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_tunnel_attribute_fn)(
    _In_ sai_object_id_t tunnel_id,
    _In_ const sai_attribute_t *attr
    );


/**
 * Routine Description:
 *    @brief Get tunnel attributes
 *
 * Arguments:
 *    @param[in] tunnel _id - tunnel id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_tunnel_attribute_fn)(
    _In_ sai_object_id_t tunnel_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

typedef enum _sai_tunnel_term_table_entry_type_t
{
    /** tunnel termination table point to point entry match on dst & src ip & tunnel type  **/
    SAI_TUNNEL_TERM_TABLE_ENTRY_P2P,

    /** tunnel termination table point to multi point entry match on dst ip  & tunnel type  **/
    SAI_TUNNEL_TERM_TABLE_ENTRY_P2MP,

} sai_tunnel_term_table_entry_type_t;

typedef enum _sai_tunnel_term_table_entry_attr_t
{
    /** READ-ONLY */

    /** READ-WRITE */

    /** tunnel virtual router id [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY)*/
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_VR_ID,

    /** tunnel entry type [sai_tunnel_table_entry_type_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE,

    /** tunnel termination ip address [sai_ip_address_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_DST_IP,

    /** tunnel source ip address [sai_ip_address_t]
     *  (MANDATORY_ON_CREATE|CREATE_ONLY) valid only for SAI_TUNNEL_TABLE_P2P  */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_SRC_IP,

    /** tunnel type [sai_tunnel_type_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TUNNEL_TYPE,

    /** tunnel id to be use for decap [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) valid o */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ACTION_TUNNEL_ID,

    /** Custom range base value */
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_tunnel_term_table_entry_attr_t;

/**
 * Routine Description:
 *    @brief Create tunnel termination table entry
 *
 * Arguments:
 *    @param[out] tunnel_term_table_entry_id - tunnel termination table entry id
 *    @param[in] attr_count - number of attributes
 *    @param[in] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_create_tunnel_term_table_entry_fn)(
    _Out_ sai_object_id_t* tunnel_term_table_entry_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * Routine Description:
 *    @brief Remove tunnel termination table entry
 *
 * Arguments:
 *    @param[in] tunnel_term_table_entry_id - tunnel termination table entry id
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_remove_tunnel_term_table_entry_fn)(
    _In_ sai_object_id_t tunnel_term_table_entry_id
    );

/**
 * Routine Description:
 *    @brief Set tunnel termination table entry attribute
 *
 * Arguments:
 *    @param[in] tunnel_term_table_entry_id - tunnel termination table entry id
 *    @param[in] attr - attribute
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_set_tunnel_term_table_entry_attribute_fn)(
    _In_ sai_object_id_t tunnel_term_table_entry_id,
    _In_ const sai_attribute_t *attr
    );


/**
 * Routine Description:
 *    @brief Get tunnel termination table entry attributes
 *
 * Arguments:
 *    @param[in] tunnel_term_table_entry_id - tunnel termination table entry id
 *    @param[in] attr_count - number of attributes
 *    @param[inout] attr_list - array of attributes
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_get_tunnel_term_table_entry_attribute_fn)(
    _In_ sai_object_id_t tunnel_term_table_entry_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * @brief tunnel methods table retrieved with sai_api_query()
 */
typedef struct _sai_tunnel_api_t
{
    sai_create_tunnel_map_fn                     create_tunnel_map;
    sai_remove_tunnel_map_fn                     remove_tunnel_map;
    sai_set_tunnel_map_attribute_fn              set_tunnel_map_attribute;
    sai_get_tunnel_map_attribute_fn              get_tunnel_map_attribute;
    sai_create_tunnel_fn                         create_tunnel;
    sai_remove_tunnel_fn                         remove_tunnel;
    sai_set_tunnel_attribute_fn                  set_tunnel_attribute;
    sai_get_tunnel_attribute_fn                  get_tunnel_attribute;
    sai_create_tunnel_term_table_entry_fn        create_tunnel_term_table_entry;
    sai_remove_tunnel_term_table_entry_fn        remove_tunnel_term_table_entry;
    sai_set_tunnel_term_table_entry_attribute_fn set_tunnel_term_table_entry_attribute;
    sai_get_tunnel_term_table_entry_attribute_fn get_tunnel_term_table_entry_attribute;

} sai_tunnel_api_t;


/**
 * \}
 */
#endif // __SAITUNNEL_H_
