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
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
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
    /** Next hop group is ECMP, with a dynamic number of members, unordered */
    SAI_NEXT_HOP_GROUP_TYPE_DYNAMIC_UNORDERED_ECMP,

    /** @ignore - for backward compatibility */
    SAI_NEXT_HOP_GROUP_TYPE_ECMP = SAI_NEXT_HOP_GROUP_TYPE_DYNAMIC_UNORDERED_ECMP,

    /** Next hop group is ECMP, with a dynamic number of members, sorted by priority */
    SAI_NEXT_HOP_GROUP_TYPE_DYNAMIC_ORDERED_ECMP,

    /** Next hop group is ECMP, with a fixed, usually large, number of members, sorted by index */
    SAI_NEXT_HOP_GROUP_TYPE_FINE_GRAIN_ECMP,

    /** Next hop protection group. Contains primary and backup next hops. */
    SAI_NEXT_HOP_GROUP_TYPE_PROTECTION,

    /* Other types of next hop group to be defined in the future, e.g., WCMP */

} sai_next_hop_group_type_t;

/**
 * @brief Next hop group member configured protection role
 */
typedef enum _sai_next_hop_group_member_configured_role_t
{
    /** Next hop group member is primary */
    SAI_NEXT_HOP_GROUP_MEMBER_CONFIGURED_ROLE_PRIMARY,

    /** Next hop group member is standby */
    SAI_NEXT_HOP_GROUP_MEMBER_CONFIGURED_ROLE_STANDBY,

} sai_next_hop_group_member_configured_role_t;

/**
 * @brief Next hop group member observed role
 */
typedef enum _sai_next_hop_group_member_observed_role_t
{
    /** Next hop group member is active */
    SAI_NEXT_HOP_GROUP_MEMBER_OBSERVED_ROLE_ACTIVE,

    /** Next hop group member is inactive */
    SAI_NEXT_HOP_GROUP_MEMBER_OBSERVED_ROLE_INACTIVE,

} sai_next_hop_group_member_observed_role_t;

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
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER
     */
    SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_MEMBER_LIST,

    /**
     * @brief Next hop group type
     *
     * @type sai_next_hop_group_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @isresourcetype true
     */
    SAI_NEXT_HOP_GROUP_ATTR_TYPE,

    /**
     * @brief Trigger a switch-over from primary to backup next hop
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_NEXT_HOP_GROUP_ATTR_TYPE == SAI_NEXT_HOP_GROUP_TYPE_PROTECTION
     */
    SAI_NEXT_HOP_GROUP_ATTR_SET_SWITCHOVER,

    /**
     * @brief Attach a counter
     *
     * When it is empty, then packet hits won't be counted
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_NEXT_HOP_GROUP_ATTR_COUNTER_ID,

    /**
     * @brief Configured group size
     *
     * Maximum desired number of members. The real size should
     * be queried from SAI_NEXT_HOP_GROUP_ATTR_REAL_SIZE
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     * @validonly SAI_NEXT_HOP_GROUP_ATTR_TYPE == SAI_NEXT_HOP_GROUP_TYPE_FINE_GRAIN_ECMP
     * @isresourcetype true
     */
    SAI_NEXT_HOP_GROUP_ATTR_CONFIGURED_SIZE,

    /**
     * @brief Real group size
     *
     * Can be different (greater or equal) from the configured
     * size. Application must use this value to know the exact size
     * of the group.
     * Should be used with SAI_NEXT_HOP_GROUP_TYPE_FINE_GRAIN_ECMP.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_NEXT_HOP_GROUP_ATTR_REAL_SIZE,

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
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_NEXT_HOP_GROUP
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_START,

    /**
     * @brief Next hop id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NEXT_HOP
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID,

    /**
     * @brief Member weights
     *
     * Should only be used if the type of owning group is SAI_NEXT_HOP_GROUP_TYPE_DYNAMIC_ORDERED_ECMP
     * or SAI_NEXT_HOP_GROUP_TYPE_DYNAMIC_UNORDERED_ECMP
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_WEIGHT,

    /**
     * @brief Configured role in the protection group
     *
     * Should only be used if the type of owning group is SAI_NEXT_HOP_GROUP_TYPE_PROTECTION
     *
     * @type sai_next_hop_group_member_configured_role_t
     * @flags CREATE_ONLY
     * @default SAI_NEXT_HOP_GROUP_MEMBER_CONFIGURED_ROLE_PRIMARY
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CONFIGURED_ROLE,

    /**
     * @brief The actual role in protection group
     *
     * Should only be used if the type of owning group is SAI_NEXT_HOP_GROUP_TYPE_PROTECTION
     *
     * @type sai_next_hop_group_member_observed_role_t
     * @flags READ_ONLY
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_OBSERVED_ROLE,

    /**
     * @brief The object to be monitored for this next hop.
     *
     * If the specified objects fails, the switching entity marks this
     * next hop as SAI_NEXT_HOP_GROUP_MEMBER_PROTECTION_ROLE_FAILED and does
     * not use it to forward traffic. If there is a backup next hop available
     * in this group then the backup's observed role is set to
     * SAI_NEXT_HOP_GROUP_MEMBER_PROTECTION_ROLE_FORWARDING and it is used to
     * forward traffic.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_ROUTER_INTERFACE, SAI_OBJECT_TYPE_VLAN_MEMBER, SAI_OBJECT_TYPE_TUNNEL, SAI_OBJECT_TYPE_BRIDGE_PORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_MONITORED_OBJECT,

    /**
     * @brief Object index in the fine grain ECMP table.
     *
     * Index specifying the strict member's order.
     * Allowed value range for is from 0 to SAI_NEXT_HOP_GROUP_ATTR_REAL_SIZE - 1.
     * Should only be used if the type of owning group is SAI_NEXT_HOP_GROUP_TYPE_FINE_GRAIN_ECMP.
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_INDEX,

    /**
     * @brief Object's sequence ID for enforcing the members' order.
     *
     * Loose index specifying the member's order. The index is not strict allowing for
     * the missing IDs in a sequence. It's driver's job to translate the sequence IDs
     * to the real indices in the group.
     * Should only be used if the type of owning group is SAI_NEXT_HOP_GROUP_TYPE_DYNAMIC_ORDERED_ECMP.
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_NEXT_HOP_GROUP_MEMBER_ATTR_SEQUENCE_ID,

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
 * @brief Create next hop group
 *
 * @param[out] next_hop_group_id Next hop group id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_next_hop_group_fn)(
        _Out_ sai_object_id_t *next_hop_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove next hop group
 *
 * @param[in] next_hop_group_id Next hop group id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_next_hop_group_fn)(
        _In_ sai_object_id_t next_hop_group_id);

/**
 * @brief Set Next Hop Group attribute
 *
 * @param[in] next_hop_group_id Next hop group id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_next_hop_group_attribute_fn)(
        _In_ sai_object_id_t next_hop_group_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Next Hop Group attribute
 *
 * @param[in] next_hop_group_id Next hop group ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_next_hop_group_attribute_fn)(
        _In_ sai_object_id_t next_hop_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create next hop group member
 *
 * @param[out] next_hop_group_member_id Next hop group member id
 * @param[in] switch_id Switch ID
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_next_hop_group_member_fn)(
        _Out_ sai_object_id_t *next_hop_group_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove next hop group member
 *
 * @param[in] next_hop_group_member_id Next hop group member ID
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_next_hop_group_member_fn)(
        _In_ sai_object_id_t next_hop_group_member_id);

/**
 * @brief Set Next Hop Group attribute
 *
 * @param[in] next_hop_group_member_id Next hop group member ID
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_next_hop_group_member_attribute_fn)(
        _In_ sai_object_id_t next_hop_group_member_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Next Hop Group attribute
 *
 * @param[in] next_hop_group_member_id Next hop group member ID
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_next_hop_group_member_attribute_fn)(
        _In_ sai_object_id_t next_hop_group_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Next Hop methods table retrieved with sai_api_query()
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
    sai_bulk_object_create_fn                  create_next_hop_group_members;
    sai_bulk_object_remove_fn                  remove_next_hop_group_members;
} sai_next_hop_group_api_t;

/**
 * @}
 */
#endif /** __SAINEXTHOPGROUP_H_ */
