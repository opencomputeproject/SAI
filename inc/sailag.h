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
 * @file    sailag.h
 *
 * @brief   This module defines SAI LAG interface
 */

#if !defined (__SAILAG_H_)
#define __SAILAG_H_

#include <saitypes.h>

/**
 * @defgroup SAILAG SAI - LAG specific API definitions
 *
 * @{
 */

/**
 * @brief LAG attribute: List of attributes for LAG object
 */
typedef enum _sai_lag_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_LAG_ATTR_START,

    /**
     * @brief SAI port list
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_LAG_MEMBER
     */
    SAI_LAG_ATTR_PORT_LIST = SAI_LAG_ATTR_START,

    /** READ_WRITE */

    /**
     * @brief LAG bind point for ingress ACL object
     *
     * Bind (or unbind) an ingress ACL table or ACL group on a LAG. Enable/Update
     * ingress ACL table or ACL group filtering by assigning the list of valid
     * object id. Disable ingress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_LAG_ATTR_INGRESS_ACL,

    /**
     * @brief LAG bind point for egress ACL object
     *
     * Bind (or unbind) an egress ACL tables or ACL groups on a LAG. Enable/Update
     * egress ACL table or ACL group filtering by assigning the list of valid
     * object id. Disable egress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_LAG_ATTR_EGRESS_ACL,

    /**
     * @brief Port VLAN ID
     *
     * Untagged ingress frames are tagged with Port VLAN ID (PVID)
     *
     * When a port joins a LAG:
     * SAI automatically sets the joining port PVID to that of the LAG.
     * SAI also saves in its internal database the original PVID state of the port.
     *
     * While a port is a member of a LAG, it is not possible to change the value of
     * the following 4 attributes for the port:
     * SAI_PORT_ATTR_PORT_VLAN_ID
     * SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY
     * SAI_PORT_ATTR_DROP_UNTAGGED
     * SAI_PORT_ATTR_DROP_TAGGED
     *
     * When a port leaves the LAG:
     * PVID is set to the original PVID by SAI
     * Since the port is not associated with a bridge port or a router port at that
     * point, it will not transfer traffic, until such object is attached to it.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 1
     */
    SAI_LAG_ATTR_PORT_VLAN_ID,

    /**
     * @brief Default VLAN Priority
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_LAG_ATTR_DEFAULT_VLAN_PRIORITY,

    /**
     * @brief Dropping of untagged frames on ingress
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_LAG_ATTR_DROP_UNTAGGED,

    /**
     * @brief Dropping of tagged frames on ingress
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_LAG_ATTR_DROP_TAGGED,

    /**
     * @brief TPID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0x8100
     */
    SAI_LAG_ATTR_TPID,

    /**
     * @brief LAG system port ID
     *
     * The application must manage the allocation of the system port aggregate IDs
     * associated with the LAG for consistency across all switches in a VOQ based
     * system. The system port aggregate ID range is from 1 to SAI_SWITCH_ATTR_NUMBER_OF_LAGS.
     * The default value of 0 means this field is not used and SAI will allocate the system
     * port aggregate ID internally.
     * Valid only when SAI_SWITCH_ATTR_TYPE == SAI_SWITCH_TYPE_VOQ
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY
     * @default 0
     */
    SAI_LAG_ATTR_SYSTEM_PORT_AGGREGATE_ID,

    /**
     * @brief End of attributes
     */
    SAI_LAG_ATTR_END,

    /** Custom range base value */
    SAI_LAG_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_LAG_ATTR_CUSTOM_RANGE_END

} sai_lag_attr_t;

/**
 * @brief Create LAG
 *
 * @param[out] lag_id LAG id
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_lag_fn)(
        _Out_ sai_object_id_t *lag_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove LAG
 *
 * @param[in] lag_id LAG id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_lag_fn)(
        _In_ sai_object_id_t lag_id);

/**
 * @brief Set LAG Attribute
 *
 * @param[in] lag_id LAG id
 * @param[in] attr Structure containing ID and value to be set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_lag_attribute_fn)(
        _In_ sai_object_id_t lag_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get LAG Attribute
 *
 * @param[in] lag_id LAG id
 * @param[in] attr_count Number of attributes to be get
 * @param[inout] attr_list List of structures containing ID and value to be get
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_lag_attribute_fn)(
        _In_ sai_object_id_t lag_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief List of LAG member attributes
 */
typedef enum _sai_lag_member_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_LAG_MEMBER_ATTR_START,

    /**
     * @brief LAG ID
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_LAG
     */
    SAI_LAG_MEMBER_ATTR_LAG_ID = SAI_LAG_MEMBER_ATTR_START,

    /**
     * @brief Logical port ID
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_SYSTEM_PORT
     */
    SAI_LAG_MEMBER_ATTR_PORT_ID,

    /**
     * @brief Disable traffic distribution to this port as part of LAG
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_LAG_MEMBER_ATTR_EGRESS_DISABLE,

    /**
     * @brief Disable traffic collection from this port as part of LAG
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_LAG_MEMBER_ATTR_INGRESS_DISABLE,

    /**
     * @brief End of attributes
     */
    SAI_LAG_MEMBER_ATTR_END,

    /** Custom range base value */
    SAI_LAG_MEMBER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_LAG_MEMBER_ATTR_CUSTOM_RANGE_END

} sai_lag_member_attr_t;

/**
 * @brief Create LAG Member
 *
 * @param[out] lag_member_id LAG Member id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_lag_member_fn)(
        _Out_ sai_object_id_t *lag_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove LAG Member
 *
 * @param[in] lag_member_id LAG Member id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_lag_member_fn)(
        _In_ sai_object_id_t lag_member_id);

/**
 * @brief Set LAG Member Attribute
 *
 * @param[in] lag_member_id LAG Member id
 * @param[in] attr Structure containing ID and value to be set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_lag_member_attribute_fn)(
        _In_ sai_object_id_t lag_member_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get LAG Member Attribute
 *
 * @param[in] lag_member_id LAG Member id
 * @param[in] attr_count Number of attributes to be get
 * @param[inout] attr_list List of structures containing ID and value to be get
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_lag_member_attribute_fn)(
        _In_ sai_object_id_t lag_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief LAG methods table retrieved with sai_api_query()
 */
typedef struct _sai_lag_api_t
{
    sai_create_lag_fn                create_lag;
    sai_remove_lag_fn                remove_lag;
    sai_set_lag_attribute_fn         set_lag_attribute;
    sai_get_lag_attribute_fn         get_lag_attribute;
    sai_create_lag_member_fn         create_lag_member;
    sai_remove_lag_member_fn         remove_lag_member;
    sai_set_lag_member_attribute_fn  set_lag_member_attribute;
    sai_get_lag_member_attribute_fn  get_lag_member_attribute;
    sai_bulk_object_create_fn        create_lag_members;
    sai_bulk_object_remove_fn        remove_lag_members;
} sai_lag_api_t;

/**
 * @}
 */
#endif /** __SAILAG_H_ */
