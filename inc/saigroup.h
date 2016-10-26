/**
 * Copyright (c) 2014 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
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
 * @file    sainexthopgroup.h
 *
 * @brief   This module defines SAI Next Hop Group interface
 */

#if !defined (__SAINEXTHOPGROUP_H_)
#define __SAINEXTHOPGROUP_H_

#include <saitypes.h>

/**
 * @defgroup SAINEXTHOPGROUP SAI - Next hop group specific API definitions
 *
 * @{
 */

/**
 * @brief Next hop group type
 */
typedef enum _sai_next_hop_group_type_t
{
    /** Next hop group is ECMP */
    SAI_NEXT_HOP_GROUP_TYPE_ECMP,

    /* Other types of next hop group to be defined in the future, e.g., WCMP */

} sai_next_hop_group_type_t;

/**
 * @brief Attribute id for next hop
 */
typedef enum _sai_next_hop_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_NEXT_HOP_GROUP_ATTR_START,

    /**
     * @brief Number of next hops in the group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_COUNT = SAI_NEXT_HOP_GROUP_ATTR_START,

    /**
     * @brief Next hop member list
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER
     * @flags READ_ONLY
     */
    SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_MEMBER_LIST,

    /**
     * @brief Next hop group type
     *
     * @type sai_next_hop_group_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_NEXT_HOP_GROUP_ATTR_TYPE,

    /**
     * @brief End of attributes
     */
    SAI_NEXT_HOP_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_NEXT_HOP_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_NEXT_HOP_GROUP_ATTR_CUSTOM_RANGE_END

} sai_next_hop_group_attr_t;

typedef enum _sai_next_hop_group_member_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_START,

    /**
     * @brief Next hop group id
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_NEXT_HOP_GROUP
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_START,

    /**
     * @brief Next hop id
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_NEXT_HOP
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID,

    /**
     * @brief Member weights
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_WEIGHT,

    /**
     * @brief End of attributes
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_END,

    /** Custom range base value */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START  = 0x10000000,

    /** End of custom range base */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END

} sai_next_hop_group_member_attr_t;

/**
 * @brief Attributes for RPF interface group
 */
typedef enum _sai_rpf_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_RPF_GROUP_ATTR_START,

    /**
     * @brief Number of RPF interfaces in the group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_RPF_GROUP_ATTR_RPF_INTERFACE_COUNT = SAI_RPF_GROUP_ATTR_START,

    /**
     * @brief RPF member list
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_RPF_GROUP_MEMBER
     * @flags READ_ONLY
     */
    SAI_RPF_GROUP_ATTR_RPF_MEMBER_LIST,

    /**
     * @brief End of attributes
     */
    SAI_RPF_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_RPF_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_RPF_GROUP_ATTR_CUSTOM_RANGE_END

} sai_rpf_group_attr_t;

typedef enum _sai_rpf_group_member_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_RPF_GROUP_MEMBER_ATTR_START,

    /**
     * @brief RPF interface group id
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_RPF_GROUP
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_RPF_GROUP_MEMBER_ATTR_RPF_GROUP_ID = SAI_RPF_GROUP_MEMBER_ATTR_START,

    /**
     * @brief RPF interface id
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_RPF_GROUP_MEMBER_ATTR_RPF_INTERFACE_ID,

    /**
     * @brief End of attributes
     */
    SAI_RPF_GROUP_MEMBER_ATTR_END,

    /** Custom range base value */
    SAI_RPF_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START  = 0x10000000,

    /** End of custom range base */
    SAI_RPF_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END

} sai_rpf_group_member_attr_t;

/**
 * @brief Attributes for L2MC group
 */
typedef enum _sai_l2mc_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_L2MC_GROUP_ATTR_START,

    /**
     * @brief Number of L2MC output in the group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_L2MC_GROUP_ATTR_L2MC_OUTPUT_COUNT = SAI_L2MC_GROUP_ATTR_START,

    /**
     * @brief L2MC member list
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP_MEMBER
     * @flags READ_ONLY
     */
    SAI_L2MC_GROUP_ATTR_L2MC_MEMBER_LIST,

    /**
     * @brief End of attributes
     */
    SAI_L2MC_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_L2MC_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_L2MC_GROUP_ATTR_CUSTOM_RANGE_END

} sai_l2mc_group_attr_t;

typedef enum _sai_l2mc_group_member_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_L2MC_GROUP_MEMBER_ATTR_START,

    /**
     * @brief L2MC group id
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_GROUP_ID = SAI_L2MC_GROUP_MEMBER_ATTR_START,

    /**
     * @brief L2MC output id
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_TUNNEL
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_OUTPUT_ID,

    /**
     * @brief End of attributes
     */
    SAI_L2MC_GROUP_MEMBER_ATTR_END,

    /** Custom range base value */
    SAI_L2MC_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START  = 0x10000000,

    /** End of custom range base */
    SAI_L2MC_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END

} sai_l2mc_group_member_attr_t;

/**
 * @brief Attributes for IPMC group
 */
typedef enum _sai_ipmc_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_IPMC_GROUP_ATTR_START,

    /**
     * @brief Number of IPMC interfaces in the group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_IPMC_GROUP_ATTR_IPMC_OUTPUT_COUNT = SAI_IPMC_GROUP_ATTR_START,

    /**
     * @brief IPMC member list
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_IPMC_GROUP_MEMBER
     * @flags READ_ONLY
     */
    SAI_IPMC_GROUP_ATTR_IPMC_MEMBER_LIST,

    /**
     * @brief End of attributes
     */
    SAI_IPMC_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_IPMC_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_IPMC_GROUP_ATTR_CUSTOM_RANGE_END

} sai_ipmc_group_attr_t;

typedef enum _sai_ipmc_group_member_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_IPMC_GROUP_MEMBER_ATTR_START,

    /**
     * @brief IPMC group id
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_IPMC_GROUP
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPMC_GROUP_MEMBER_ATTR_IPMC_GROUP_ID = SAI_IPMC_GROUP_MEMBER_ATTR_START,

    /**
     * @brief IPMC output id
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE, SAI_OBJECT_TYPE_TUNNEL
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_IPMC_GROUP_MEMBER_ATTR_IPMC_OUTPUT_ID,

    /**
     * @brief End of attributes
     */
    SAI_IPMC_GROUP_MEMBER_ATTR_END,

    /** Custom range base value */
    SAI_IPMC_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START  = 0x10000000,

    /** End of custom range base */
    SAI_IPMC_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END

} sai_ipmc_group_member_attr_t;

/**
 * @brief Create next hop group
 *
 * @param[out] next_hop_group_id Next hop group id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_next_hop_group_fn)(
        _Out_ sai_object_id_t *next_hop_group_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove next hop group
 *
 * @param[in] next_hop_group_id Next hop group id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_next_hop_group_fn)(
        _In_ sai_object_id_t next_hop_group_id);

/**
 * @brief Set Next Hop Group attribute
 *
 * @param[in] sai_object_id_t Next hop group id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_next_hop_group_attribute_fn)(
        _In_ sai_object_id_t next_hop_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Next Hop Group attribute
 *
 * @param[in] sai_object_id_t Next_hop_group_id
 * @param[in] attr_count -Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_next_hop_group_attribute_fn)(
        _In_ sai_object_id_t next_hop_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create next hop group member
 *
 * @param[out] next_hop_group_member_id - next hop group member id
 * @param[in] attr_count - number of attributes
 * @param[in] attr_list - array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_next_hop_group_member_fn)(
    _Out_ sai_object_id_t* next_hop_group_member_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * @brief Remove next hop group member
 *
 * @param[in] next_hop_group_member_id - next hop group member id
 *
 * @return SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_next_hop_group_member_fn)(
    _In_ sai_object_id_t next_hop_group_member_id
    );

/**
 * @brief Set Next Hop Group attribute
 *
 * @param[in] sai_object_id_t - next_hop_group_member_id
 * @param[in] attr - attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_next_hop_group_member_attribute_fn)(
    _In_ sai_object_id_t next_hop_group_member_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief Get Next Hop Group attribute
 *
 * @param[in] sai_object_id_t - next_hop_group_member_id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_next_hop_group_member_attribute_fn)(
    _In_ sai_object_id_t next_hop_group_member_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * @brief Create RPF interface group
 *
 * @param[out] rpf_group_id RPF interface group id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_rpf_group_fn)(
        _Out_ sai_object_id_t *rpf_group_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove RPF interface group
 *
 * @param[in] rpf_group_id RPF interface group id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_rpf_group_fn)(
        _In_ sai_object_id_t rpf_group_id);

/**
 * @brief Set RPF interface Group attribute
 *
 * @param[in] sai_object_id_t RPF interface group id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_rpf_group_attribute_fn)(
        _In_ sai_object_id_t rpf_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get RPF interface Group attribute
 *
 * @param[in] sai_object_id_t RPF interface group id
 * @param[in] attr_count -Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_rpf_group_attribute_fn)(
        _In_ sai_object_id_t rpf_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create RPF interface group member
 *
 * @param[out] rpf_group_member_id - RPF interface group member id
 * @param[in] attr_count - number of attributes
 * @param[in] attr_list - array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_rpf_group_member_fn)(
    _Out_ sai_object_id_t* rpf_group_member_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * @brief Remove RPF interface group member
 *
 * @param[in] rpf_group_member_id - RPF interface group member id
 *
 * @return SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_rpf_group_member_fn)(
    _In_ sai_object_id_t rpf_group_member_id
    );

/**
 * @brief Set RPF interface Group attribute
 *
 * @param[in] sai_object_id_t - RPF interface group member id
 * @param[in] attr - attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_rpf_group_member_attribute_fn)(
    _In_ sai_object_id_t rpf_group_member_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief Get RPF interface Group attribute
 *
 * @param[in] sai_object_id_t - rpf_group_member_id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_rpf_group_member_attribute_fn)(
    _In_ sai_object_id_t rpf_group_member_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * @brief Create L2MC group
 *
 * @param[out] l2mc_group_id L2MC group id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_l2mc_group_fn)(
        _Out_ sai_object_id_t *l2mc_group_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove L2MC group
 *
 * @param[in] l2mc_group_id L2MC group id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_l2mc_group_fn)(
        _In_ sai_object_id_t l2mc_group_id);

/**
 * @brief Set L2MC Group attribute
 *
 * @param[in] sai_object_id_t L2MC group id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_l2mc_group_attribute_fn)(
        _In_ sai_object_id_t l2mc_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get L2MC Group attribute
 *
 * @param[in] sai_object_id_t L2MC group id
 * @param[in] attr_count -Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_l2mc_group_attribute_fn)(
        _In_ sai_object_id_t l2mc_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create L2MC group member
 *
 * @param[out] l2mc_group_member_id - L2MC group member id
 * @param[in] attr_count - number of attributes
 * @param[in] attr_list - array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_l2mc_group_member_fn)(
    _Out_ sai_object_id_t* l2mc_group_member_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * @brief Remove L2MC group member
 *
 * @param[in] l2mc_group_member_id - L2MC group member id
 *
 * @return SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_l2mc_group_member_fn)(
    _In_ sai_object_id_t l2mc_group_member_id
    );

/**
 * @brief Set L2MC Group attribute
 *
 * @param[in] sai_object_id_t - L2MC group member id
 * @param[in] attr - attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_l2mc_group_member_attribute_fn)(
    _In_ sai_object_id_t l2mc_group_member_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief Get L2MC Group attribute
 *
 * @param[in] sai_object_id_t - l2mc_group_member_id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_l2mc_group_member_attribute_fn)(
    _In_ sai_object_id_t l2mc_group_member_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * @brief Create IPMC group
 *
 * @param[out] ipmc_group_id IPMC group id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_ipmc_group_fn)(
        _Out_ sai_object_id_t *ipmc_group_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove IPMC group
 *
 * @param[in] ipmc_group_id IPMC group id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_ipmc_group_fn)(
        _In_ sai_object_id_t ipmc_group_id);

/**
 * @brief Set IPMC Group attribute
 *
 * @param[in] sai_object_id_t IPMC group id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_ipmc_group_attribute_fn)(
        _In_ sai_object_id_t ipmc_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get IPMC Group attribute
 *
 * @param[in] sai_object_id_t IPMC group id
 * @param[in] attr_count -Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_ipmc_group_attribute_fn)(
        _In_ sai_object_id_t ipmc_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create IPMC group member
 *
 * @param[out] ipmc_group_member_id - IPMC group member id
 * @param[in] attr_count - number of attributes
 * @param[in] attr_list - array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_ipmc_group_member_fn)(
    _Out_ sai_object_id_t* ipmc_group_member_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );

/**
 * @brief Remove IPMC group member
 *
 * @param[in] ipmc_group_member_id - IPMC group member id
 *
 * @return SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_ipmc_group_member_fn)(
    _In_ sai_object_id_t ipmc_group_member_id
    );

/**
 * @brief Set IPMC Group attribute
 *
 * @param[in] sai_object_id_t - IPMC group member id
 * @param[in] attr - attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_ipmc_group_member_attribute_fn)(
    _In_ sai_object_id_t ipmc_group_member_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief Get IPMC Group attribute
 *
 * @param[in] sai_object_id_t - ipmc_group_member_id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_ipmc_group_member_attribute_fn)(
    _In_ sai_object_id_t ipmc_group_member_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 *  @brief Next Hop methods table retrieved with sai_api_query()
 */
typedef struct _sai_next_hop_group_api_t
{
    sai_create_next_hop_group_fn               create_next_hop_group;
    sai_remove_next_hop_group_fn               remove_next_hop_group;
    sai_set_next_hop_group_attribute_fn        set_next_hop_group_attribute;
    sai_get_next_hop_group_attribute_fn        get_next_hop_group_attribute;
    sai_create_next_hop_group_member_fn        create_next_hop_group_member;
    sai_remove_next_hop_group_member_fn        remove_next_hop_group_member;
    sai_set_next_hop_group_member_attribute_fn set_next_hop_group_member_attribute;
    sai_get_next_hop_group_member_attribute_fn get_next_hop_group_member_attribute;

    sai_create_rpf_group_fn                    create_rpf_group;
    sai_remove_rpf_group_fn                    remove_rpf_group;
    sai_set_rpf_group_attribute_fn             set_rpf_group_attribute;
    sai_get_rpf_group_attribute_fn             get_rpf_group_attribute;
    sai_create_rpf_group_member_fn             create_rpf_group_member;
    sai_remove_rpf_group_member_fn             remove_rpf_group_member;
    sai_set_rpf_group_member_attribute_fn      set_rpf_group_member_attribute;
    sai_get_rpf_group_member_attribute_fn      get_rpf_group_member_attribute;

    sai_create_l2mc_group_fn                   create_l2mc_group;
    sai_remove_l2mc_group_fn                   remove_l2mc_group;
    sai_set_l2mc_group_attribute_fn            set_l2mc_group_attribute;
    sai_get_l2mc_group_attribute_fn            get_l2mc_group_attribute;
    sai_create_l2mc_group_member_fn            create_l2mc_group_member;
    sai_remove_l2mc_group_member_fn            remove_l2mc_group_member;
    sai_set_l2mc_group_member_attribute_fn     set_l2mc_group_member_attribute;
    sai_get_l2mc_group_member_attribute_fn     get_l2mc_group_member_attribute;

    sai_create_ipmc_group_fn                    create_ipmc_group;
    sai_remove_ipmc_group_fn                    remove_ipmc_group;
    sai_set_ipmc_group_attribute_fn             set_ipmc_group_attribute;
    sai_get_ipmc_group_attribute_fn             get_ipmc_group_attribute;
    sai_create_ipmc_group_member_fn             create_ipmc_group_member;
    sai_remove_ipmc_group_member_fn             remove_ipmc_group_member;
    sai_set_ipmc_group_member_attribute_fn      set_ipmc_group_member_attribute;
    sai_get_ipmc_group_member_attribute_fn      get_ipmc_group_member_attribute;

} sai_next_hop_group_api_t;

/**
 * @}
 */
#endif /** __SAINEXTHOPGROUP_H_ */
