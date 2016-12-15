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
 * @file    saimcfdb.h
 *
 * @brief   This module defines SAI multicast FDB interface
 */

#if !defined (__SAIMCFDB_H_)
#define __SAIMCFDB_H_

#include <saitypes.h>

/**
 * @defgroup SAIMCFDB SAI - Multicast FDB specific API definitions
 *
 * @{
 */

/**
 * @brief MCAST FDB entry key
 */
typedef struct _sai_mcast_fdb_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /** Mac address */
    sai_mac_t mac_address;

    /** Vlan ID */
    sai_vlan_id_t vlan_id;

} sai_mcast_fdb_entry_t;

/**
 * @brief Attribute Id for multicast fdb entry
 */
typedef enum _sai_mcast_fdb_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_MCAST_FDB_ENTRY_ATTR_START,

    /**
     * @brief Multicast FDB entry group id
     *
     * The group id refers to a L2MC group object. In case of empty group,
     * packets will be discarded.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MCAST_FDB_ENTRY_ATTR_GROUP_ID = SAI_MCAST_FDB_ENTRY_ATTR_START,

    /**
     * @brief Multicast FDB entry packet action
     *
     * @type sai_packet_action_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MCAST_FDB_ENTRY_ATTR_PACKET_ACTION,

    /**
     * @brief User based Meta Data
     *
     * Value Range #SAI_SWITCH_ATTR_FDB_DST_USER_META_DATA_RANGE
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_MCAST_FDB_ENTRY_ATTR_META_DATA,

    /**
     * @brief End of attributes
     */
    SAI_MCAST_FDB_ENTRY_ATTR_END,

    /** Start of custom range base value */
    SAI_MCAST_FDB_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range */
    SAI_MCAST_FDB_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_mcast_fdb_entry_attr_t;

/**
 * @brief Create Multicast FDB entry
 *
 * @param[in] fdb_entry FDB entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_mcast_fdb_entry_fn)(
        _In_ const sai_mcast_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove Multicast FDB entry
 *
 * @param[in] fdb_entry FDB entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_mcast_fdb_entry_fn)(
        _In_ const sai_mcast_fdb_entry_t *fdb_entry);

/**
 * @brief Set multicast fdb entry attribute value
 *
 * @param[in] fdb_entry FDB entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_mcast_fdb_entry_attribute_fn)(
        _In_ const sai_mcast_fdb_entry_t *fdb_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get fdb entry attribute value
 *
 * @param[in] fdb_entry FDB entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_mcast_fdb_entry_attribute_fn)(
        _In_ const sai_mcast_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Multicast FDB method table retrieved with sai_api_query()
 */
typedef struct _sai_mcast_fdb_api_t
{
    sai_create_mcast_fdb_entry_fn                     create_mcast_fdb_entry;
    sai_remove_mcast_fdb_entry_fn                     remove_mcast_fdb_entry;
    sai_set_mcast_fdb_entry_attribute_fn              set_mcast_fdb_entry_attribute;
    sai_get_mcast_fdb_entry_attribute_fn              get_mcast_fdb_entry_attribute;

} sai_mcast_fdb_api_t;

/**
 * @}
 */
#endif /** __SAIMCFDB_H_ */
