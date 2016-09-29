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
 * @file    saifdb.h
 *
 * @brief   This module defines SAI FDB interface
 */

#if !defined (__SAIFDB_H_)
#define __SAIFDB_H_

#include <saitypes.h>

/**
 * @defgroup SAIFDB SAI - FDB specific API definitions
 *
 * @{
 */

/**
 * @brief FDB entry type.
 */
typedef enum _sai_fdb_entry_type_t
{
    /** Dynamic FDB Entry */
    SAI_FDB_ENTRY_TYPE_DYNAMIC,

    /** Static FDB Entry */
    SAI_FDB_ENTRY_TYPE_STATIC,

} sai_fdb_entry_type_t;

/**
 * @brief FDB entry key
 */
typedef struct _sai_fdb_entry_t
{
    /** Mac address */
    sai_mac_t mac_address;

    /** Vlan ID */
    sai_vlan_id_t vlan_id;

} sai_fdb_entry_t;

/**
 * @brief FDB event type
 */
typedef enum _sai_fdb_event_t
{
    /** New FDB entry learned */
    SAI_FDB_EVENT_LEARNED,

    /** FDB entry aged */
    SAI_FDB_EVENT_AGED,

    /** FDB entry flushd */
    SAI_FDB_EVENT_FLUSHED,

} sai_fdb_event_t;

/**
 * @brief Attribute Id for fdb entry
 */
typedef enum _sai_fdb_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FDB_ENTRY_ATTR_START,

    /**
     * @brief FDB entry type
     *
     *
     * @type sai_fdb_entry_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_FDB_ENTRY_ATTR_TYPE = SAI_FDB_ENTRY_ATTR_START,

    /**
     * @brief FDB entry port id
     *
     * The port id here can refer to a generic port object such as SAI port object id,
     * SAI LAG object id and etc. or to a tunnel next hop object in case the entry is
     * l2 tunnel
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_TUNNEL
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_FDB_ENTRY_ATTR_PORT_ID,

    /**
     * @brief FDB entry packet action
     *
     * @type sai_packet_action_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_FDB_ENTRY_ATTR_PACKET_ACTION,

    /**
     * @brief User based Meta Data
     *
     * Value Range #SAI_SWITCH_ATTR_FDB_DST_USER_META_DATA_RANGE
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FDB_ENTRY_ATTR_META_DATA,

    /**
     * @brief End of attributes
     */
    SAI_FDB_ENTRY_ATTR_END,

    /** Start of custom range base value */
    SAI_FDB_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range */
    SAI_FDB_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_fdb_entry_attr_t;

/**
 * @brief FDB Flush entry type.
 */
typedef enum _sai_fdb_flush_entry_type_t
{
    /** Flush dynamic FDB entries */
    SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC,

    /** Flush static FDB entries */
    SAI_FDB_FLUSH_ENTRY_TYPE_STATIC,

} sai_fdb_flush_entry_type_t;

/**
 * @brief Attribute for FDB flush API to specify the type of FDB entries being flushed.
 *
 * For example, if you want to flush all static entries, set #SAI_FDB_FLUSH_ATTR_ENTRY_TYPE
 * = #SAI_FDB_FLUSH_ENTRY_TYPE_STATIC. If you want to flush both static and dynamic entries,
 * then there is no need to specify the #SAI_FDB_FLUSH_ATTR_ENTRY_TYPE attribute.
 * The API uses AND operation when multiple attributes are specified. For
 * exmaple,
 * 1) Flush all entries in fdb table - Do not specify any attribute
 * 2) Flush all entries by port - Set #SAI_FDB_FLUSH_ATTR_PORT_ID
 * 3) Flush all entries by VLAN - Set #SAI_FDB_FLUSH_ATTR_VLAN_ID
 * 4) Flush all entries by port and VLAN - Set #SAI_FDB_FLUSH_ATTR_PORT_ID and
 *    #SAI_FDB_FLUSH_ATTR_VLAN_ID
 * 5) Flush all static entries by port and VLAN - Set #SAI_FDB_FLUSH_ATTR_ENTRY_TYPE,
 *    #SAI_FDB_FLUSH_ATTR_PORT_ID, and #SAI_FDB_FLUSH_ATTR_VLAN_ID
 */
typedef enum _sai_fdb_flush_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FDB_FLUSH_ATTR_START,

    /**
     * @brief Flush based on port
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_PORT
     * @flags SPECIAL
     * @ignore
     */
    SAI_FDB_FLUSH_ATTR_PORT_ID = SAI_FDB_FLUSH_ATTR_START,

    /**
     * @brief Flush based on VLAN
     *
     * @type sai_uint16_t
     * @flags SPECIAL
     * @isvlan true
     * @ignore
     */
    SAI_FDB_FLUSH_ATTR_VLAN_ID,

    /**
     * @brief Flush based on entry type
     *
     * @type sai_fdb_flush_entry_type_t
     * @flags SPECIAL
     * @ignore
     */
    SAI_FDB_FLUSH_ATTR_ENTRY_TYPE,

    /**
     * @brief End of attributes
     */
    SAI_FDB_FLUSH_ATTR_END,

} sai_fdb_flush_attr_t;

/**
 * @brief Notification data format received from SAI FDB callback
 */
typedef struct _sai_fdb_event_notification_data_t
{
    /** Event type */
    sai_fdb_event_t event_type;

    /** FDB entry */
    sai_fdb_entry_t fdb_entry;

    /** Attributes count */
    uint32_t attr_count;

    /** Attributes */
    sai_attribute_t *attr;

} sai_fdb_event_notification_data_t;

/**
 * @brief Create FDB entry
 *
 * @param[in] fdb_entry FDB entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_fdb_entry_fn)(
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove FDB entry
 *
 * @param[in] fdb_entry FDB entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_fdb_entry_fn)(
        _In_ const sai_fdb_entry_t *fdb_entry);

/**
 * @brief Set fdb entry attribute value
 *
 * @param[in] fdb_entry FDB entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_fdb_entry_attribute_fn)(
        _In_ const sai_fdb_entry_t *fdb_entry,
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
typedef sai_status_t (*sai_get_fdb_entry_attribute_fn)(
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Remove all FDB entries by attribute set in sai_fdb_flush_attr
 *
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_flush_fdb_entries_fn)(
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief FDB notifications
 *
 * @param[in] count Number of notifications
 * @param[in] data Pointer to fdb event notification data array
 */
typedef void (*sai_fdb_event_notification_fn)(
        _In_ uint32_t count,
        _In_ sai_fdb_event_notification_data_t *data);

/**
 * @brief FDB method table retrieved with sai_api_query()
 */
typedef struct _sai_fdb_api_t
{
    sai_create_fdb_entry_fn                     create_fdb_entry;
    sai_remove_fdb_entry_fn                     remove_fdb_entry;
    sai_set_fdb_entry_attribute_fn              set_fdb_entry_attribute;
    sai_get_fdb_entry_attribute_fn              get_fdb_entry_attribute;
    sai_flush_fdb_entries_fn                    flush_fdb_entries;

} sai_fdb_api_t;

/**
 * @}
 */
#endif /** __SAIFDB_H_ */
