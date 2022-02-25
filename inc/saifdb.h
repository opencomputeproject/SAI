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
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /** MAC address */
    sai_mac_t mac_address;

    /**
     * @brief Bridge ID. for .1D and Vlan ID for .1Q
     *
     * @objects SAI_OBJECT_TYPE_BRIDGE, SAI_OBJECT_TYPE_VLAN
     */
    sai_object_id_t bv_id;

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

    /** FDB entry move */
    SAI_FDB_EVENT_MOVE,

    /** FDB entry flushed */
    SAI_FDB_EVENT_FLUSHED,

} sai_fdb_event_t;

/**
 * @brief Attribute Id for FDB entry
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
     * @type sai_fdb_entry_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_FDB_ENTRY_ATTR_TYPE = SAI_FDB_ENTRY_ATTR_START,

    /**
     * @brief FDB entry packet action
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_FDB_ENTRY_ATTR_PACKET_ACTION,

    /**
     * @brief Generate User Defined Trap ID for trap/log actions
     *
     * When it is SAI_NULL_OBJECT_ID, then packet will not be trapped.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_HOSTIF_USER_DEFINED_TRAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_FDB_ENTRY_ATTR_USER_TRAP_ID,

    /**
     * @brief FDB entry bridge port id
     *
     * The port id is only effective when the packet action is one of the
     * following: FORWARD, COPY, LOG, TRANSIT
     *
     * When it is SAI_NULL_OBJECT_ID, then packet will be dropped.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_BRIDGE_PORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_FDB_ENTRY_ATTR_BRIDGE_PORT_ID,

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
     * @brief Tunnel Endpoint IP. valid for SAI_BRIDGE_PORT_TYPE_TUNNEL
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_FDB_ENTRY_ATTR_ENDPOINT_IP,

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
    SAI_FDB_ENTRY_ATTR_COUNTER_ID,

    /**
     * @brief Specifies whether a MAC move is allowed
     * When MAC_MOVE is explicitly disabled for a static MAC entry via this
     * attribute, the trap introduced in #696 would also not be generated.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_FDB_ENTRY_ATTR_TYPE == SAI_FDB_ENTRY_TYPE_STATIC
     */
    SAI_FDB_ENTRY_ATTR_ALLOW_MAC_MOVE,

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

    /** Flush static and dynamic FDB entries */
    SAI_FDB_FLUSH_ENTRY_TYPE_ALL,

} sai_fdb_flush_entry_type_t;

/**
 * @brief Attribute for FDB flush API to specify the type of FDB entries being flushed.
 *
 * For example, if you want to flush all static entries, set #SAI_FDB_FLUSH_ATTR_ENTRY_TYPE
 * = #SAI_FDB_FLUSH_ENTRY_TYPE_STATIC. If you want to flush both static and dynamic entries,
 * then set #SAI_FDB_FLUSH_ATTR_ENTRY_TYPE = SAI_FDB_FLUSH_ENTRY_TYPE_ALL.
 * The API uses AND operation when multiple attributes are specified.
 *
 * For example:
 *
 * 1) Flush all entries in FDB table - Do not specify any attribute
 * 2) Flush all entries by bridge port - Set #SAI_FDB_FLUSH_ATTR_BRIDGE_PORT_ID
 * 3) Flush all entries by VLAN - Set #SAI_FDB_FLUSH_ATTR_BV_ID with object id as vlan object
 * 3) Flush all entries by bridge - Set #SAI_FDB_FLUSH_ATTR_BV_ID with object id as bridge object
 * 4) Flush all entries by bridge port and VLAN - Set #SAI_FDB_FLUSH_ATTR_BRIDGE_PORT_ID
 *    and #SAI_FDB_FLUSH_ATTR_BV_ID
 * 5) Flush all static entries by bridge port and VLAN - Set #SAI_FDB_FLUSH_ATTR_ENTRY_TYPE,
 *    #SAI_FDB_FLUSH_ATTR_BRIDGE_PORT_ID, and #SAI_FDB_FLUSH_ATTR_BV_ID
 */
typedef enum _sai_fdb_flush_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FDB_FLUSH_ATTR_START,

    /**
     * @brief Flush based on bridge port
     *
     * @type sai_object_id_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE_PORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_FDB_FLUSH_ATTR_BRIDGE_PORT_ID = SAI_FDB_FLUSH_ATTR_START,

    /**
     * @brief Flush based on VLAN or Bridge
     *
     * @type sai_object_id_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE, SAI_OBJECT_TYPE_VLAN
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_FDB_FLUSH_ATTR_BV_ID,

    /**
     * @brief Flush based on entry type
     *
     * @type sai_fdb_flush_entry_type_t
     * @flags CREATE_ONLY
     * @default SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC
     */
    SAI_FDB_FLUSH_ATTR_ENTRY_TYPE,

    /**
     * @brief End of attributes
     */
    SAI_FDB_FLUSH_ATTR_END,

    /** Custom range base value */
    SAI_FDB_FLUSH_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_FDB_FLUSH_ATTR_CUSTOM_RANGE_END

} sai_fdb_flush_attr_t;

/**
 * @brief Notification data format received from SAI FDB callback
 *
 * When FDB flush API is called (for example with no parameters) and switch
 * learned a lot of MAC addresses, then calling this API can cause to generate
 * a lot of notifications.
 *
 * Vendor can decide whether in that case send notifications 1 by 1 and
 * populating all the data for sai_fdb_event_notification_data_t or to send
 * consolidated event notification which will indicate that FDB flush operation
 * was performed.
 *
 * Consolidated flush event will:
 *
 * Set data.fdb_entry.mac_address to 00:00:00:00:00:00.
 *
 * Set data.fdb_event to SAI_FDB_EVENT_FLUSHED.
 *
 * Add SAI_FDB_ENTRY_ATTR_TYPE to data.attr list and value set to
 * SAI_FDB_FLUSH_ATTR_ENTRY_TYPE, if SAI_FDB_FLUSH_ATTR_ENTRY_TYPE was not
 * provided to flush API, then 2 notifications will be sent (or 1 notification
 * with 2 data entries) where data.attr will contain SAI_FDB_ENTRY_ATTR_TYPE
 * set accordingly for specific entry types.
 *
 * Set data.fdb_entry.bv_id to SAI_FDB_FLUSH_ATTR_BV_ID value if attribute was
 * provided to flush API.
 *
 * Add SAI_FDB_ENTRY_ATTR_BRIDGE_PORT_ID to data.attr list and value set to
 * SAI_FDB_FLUSH_ATTR_BRIDGE_PORT_ID if that attribute was provided to flush
 * API.
 *
 * All other attributes in consolidated FDB event notification are irrelevant
 * and should be zero.
 *
 * @count attr[attr_count]
 */
typedef struct _sai_fdb_event_notification_data_t
{
    /** Event type */
    sai_fdb_event_t event_type;

    /** FDB entry */
    sai_fdb_entry_t fdb_entry;

    /** Attributes count */
    uint32_t attr_count;

    /**
     * @brief Attributes
     *
     * @objects SAI_OBJECT_TYPE_FDB_ENTRY
     */
    sai_attribute_t *attr;

} sai_fdb_event_notification_data_t;

/**
 * @brief Create FDB entry
 *
 * @param[in] fdb_entry FDB entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
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
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_fdb_entry_fn)(
        _In_ const sai_fdb_entry_t *fdb_entry);

/**
 * @brief Set FDB entry attribute value
 *
 * @param[in] fdb_entry FDB entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_fdb_entry_attribute_fn)(
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get FDB entry attribute value
 *
 * @param[in] fdb_entry FDB entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_fdb_entry_attribute_fn)(
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Remove all FDB entries by attribute set in sai_fdb_flush_attr
 *
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_flush_fdb_entries_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief FDB notifications
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Pointer to FDB event notification data array
 */
typedef void (*sai_fdb_event_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_fdb_event_notification_data_t *data);

/**
 * @brief Bulk create FDB entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] fdb_entry List of object to create
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to create.
 * @param[in] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are created or
 * #SAI_STATUS_FAILURE when any of the objects fails to create. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_create_fdb_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove FDB entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] fdb_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_fdb_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk set attribute on FDB entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] fdb_entry List of objects to set attribute
 * @param[in] attr_list List of attributes to set on objects, one attribute per object
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are set or
 * #SAI_STATUS_FAILURE when any of the objects fails to set. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_set_fdb_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk get attribute on FDB entry
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] fdb_entry List of objects to get attribute
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get
 * @param[inout] attr_list List of attributes to get on objects, one attribute per object
 * @param[in] mode Bulk operation error handling mode
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are get or
 * #SAI_STATUS_FAILURE when any of the objects fails to get. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_fdb_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

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
    sai_bulk_create_fdb_entry_fn                create_fdb_entries;
    sai_bulk_remove_fdb_entry_fn                remove_fdb_entries;
    sai_bulk_set_fdb_entry_attribute_fn         set_fdb_entries_attribute;
    sai_bulk_get_fdb_entry_attribute_fn         get_fdb_entries_attribute;

} sai_fdb_api_t;

/**
 * @}
 */
#endif /** __SAIFDB_H_ */
