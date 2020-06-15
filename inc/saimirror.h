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
 * @file    saimirror.h
 *
 * @brief   This module defines SAI Port Mirror interface
 */

#if !defined (__SAIMIRROR_H_)
#define __SAIMIRROR_H_

#include <saitypes.h>

/**
 * @defgroup SAIMIRROR SAI - Mirror specific public APIs and data structures
 *
 * @{
 */

/**
 * @brief SAI type of mirroring
 */
typedef enum _sai_mirror_session_type_t
{
    /** Local SPAN */
    SAI_MIRROR_SESSION_TYPE_LOCAL = 0,

    /** Remote SPAN */
    SAI_MIRROR_SESSION_TYPE_REMOTE,

    /** Enhanced Remote SPAN */
    SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE,

} sai_mirror_session_type_t;

/**
 * @brief SAI type of encapsulation for RSPAN and ERSPAN
 */
typedef enum _sai_erspan_encapsulation_type_t
{
    /**
     * @brief L3 GRE Tunnel Encapsulation | L2 Ethernet header | IP header | GRE header | Original mirrored packet
     */
    SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL,

} sai_erspan_encapsulation_type_t;

/**
 * @brief Mirror session congestion mode
 */
typedef enum _sai_mirror_session_congestion_mode_t
{
    /** Mirroring traffic is independent from original traffic, original traffic not affected by congestion of mirroring traffic */
    SAI_MIRROR_SESSION_CONGESTION_MODE_INDEPENDENT,

    /** Mirroring traffic is correlated with original traffic, and can cause back pressure and discards of original traffic if there is congestion */
    SAI_MIRROR_SESSION_CONGESTION_MODE_CORRELATED,

} sai_mirror_session_congestion_mode_t;

/**
 * @brief SAI attributes for mirror session
 */
typedef enum _sai_mirror_session_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_MIRROR_SESSION_ATTR_START,

    /**
     * @brief Mirror type SPAN/RSPAN/ERSPAN
     *
     * @type sai_mirror_session_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_MIRROR_SESSION_ATTR_TYPE = SAI_MIRROR_SESSION_ATTR_START,

    /**
     * @brief Destination/Analyzer/Monitor Port.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_SYSTEM_PORT
     * @condition SAI_MIRROR_SESSION_ATTR_MONITOR_PORTLIST_VALID == false
     */
    SAI_MIRROR_SESSION_ATTR_MONITOR_PORT,

    /**
     * @brief Truncate size. Truncate mirrored packets to this size to reduce SPAN traffic bandwidth
     *
     * Value 0 to no truncation
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_MIRROR_SESSION_ATTR_TRUNCATE_SIZE,

    /**
     * @brief Mirror sample rate. Every 1/sample_rate the packets will be mirrored.
     *
     * Value 0 to no sampling
     * Value 1 to every packet sampling (normal mirror)
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1
     */
    SAI_MIRROR_SESSION_ATTR_SAMPLE_RATE,

    /**
     * @brief Controls whether mirroring traffic can cause back pressure and packet drop of the original traffic
     *
     * @type sai_mirror_session_congestion_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_MIRROR_SESSION_CONGESTION_MODE_INDEPENDENT
     */
    SAI_MIRROR_SESSION_ATTR_CONGESTION_MODE,

    /**
     * @brief Class-of-Service (Traffic Class)
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_MIRROR_SESSION_ATTR_TC,

    /**
     * @brief L2 header TPID.
     *
     * Valid for RSPAN or ERSPAN with valid Vlan header.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0x8100
     * @validonly SAI_MIRROR_SESSION_ATTR_VLAN_HEADER_VALID == true or SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_VLAN_TPID,

    /**
     * @brief L2 header VLAN Id.
     *
     * Valid for RSPAN or ERSPAN with valid Vlan header.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     * @validonly SAI_MIRROR_SESSION_ATTR_VLAN_HEADER_VALID == true or SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_VLAN_ID,

    /**
     * @brief L2 header packet priority (3 bits).
     *
     * Valid for RSPAN or ERSPAN with valid Vlan header.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_MIRROR_SESSION_ATTR_VLAN_HEADER_VALID == true or SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_VLAN_PRI,

    /**
     * @brief L2 header Vlan CFI (1 bit).
     *
     * Valid for RSPAN or ERSPAN with valid Vlan header.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_MIRROR_SESSION_ATTR_VLAN_HEADER_VALID == true or SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_VLAN_CFI,

    /*
     * All attributes below are valid only for ERSPAN
     * SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE.
     */

    /**
     * @brief Vlan header valid
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_VLAN_HEADER_VALID,

    /**
     * @brief Encapsulation type
     *
     * @type sai_erspan_encapsulation_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_ERSPAN_ENCAPSULATION_TYPE,

    /**
     * @brief Tunnel IP header version
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_IPHDR_VERSION,

    /**
     * @brief Tunnel header TOS
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_TOS,

    /**
     * @brief Tunnel header TTL
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 255
     */
    SAI_MIRROR_SESSION_ATTR_TTL,

    /**
     * @brief Tunnel source IP
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_SRC_IP_ADDRESS,

    /**
     * @brief Tunnel destination IP
     *
     * @type sai_ip_address_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_DST_IP_ADDRESS,

    /**
     * @brief L2 source MAC address
     *
     * @type sai_mac_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_SRC_MAC_ADDRESS,

    /**
     * @brief L2 destination MAC address
     *
     * @type sai_mac_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @condition SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_DST_MAC_ADDRESS,

    /**
     * @brief Valid for ERSPAN, GRE protocol Id
     *
     * @type sai_uint16_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @isvlan false
     * @condition SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_GRE_PROTOCOL_TYPE,

    /**
     * @brief Monitor port list is valid
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_MIRROR_SESSION_ATTR_MONITOR_PORTLIST_VALID,

    /**
     * @brief Destination/Analyzer/Monitor Port List.
     *
     * @type sai_object_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_SYSTEM_PORT
     * @condition SAI_MIRROR_SESSION_ATTR_MONITOR_PORTLIST_VALID == true
     */
    SAI_MIRROR_SESSION_ATTR_MONITOR_PORTLIST,

    /**
     * @brief Mirror session policer object ID
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_POLICER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_MIRROR_SESSION_ATTR_POLICER,

    /**
     * @brief End of attributes
     */
    SAI_MIRROR_SESSION_ATTR_END,

    /** Custom range base value */
    SAI_MIRROR_SESSION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_MIRROR_SESSION_ATTR_CUSTOM_RANGE_END

} sai_mirror_session_attr_t;

/**
 * @brief Create mirror session.
 *
 * @param[out] mirror_session_id Port mirror session id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_create_mirror_session_fn)(
        _Out_ sai_object_id_t *mirror_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove mirror session.
 *
 * @param[in] mirror_session_id Port mirror session id
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_remove_mirror_session_fn)(
        _In_ sai_object_id_t mirror_session_id);

/**
 * @brief Set mirror session attributes.
 *
 * @param[in] mirror_session_id Port mirror session id
 * @param[in] attr Value of attribute
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_set_mirror_session_attribute_fn)(
        _In_ sai_object_id_t mirror_session_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get mirror session attributes.
 *
 * @param[in] mirror_session_id Port mirror session id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Value of attribute
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_get_mirror_session_attribute_fn)(
        _In_ sai_object_id_t mirror_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief MIRROR method table retrieved with sai_api_query()
 */
typedef struct _sai_mirror_api_t
{
    sai_create_mirror_session_fn            create_mirror_session;
    sai_remove_mirror_session_fn            remove_mirror_session;
    sai_set_mirror_session_attribute_fn     set_mirror_session_attribute;
    sai_get_mirror_session_attribute_fn     get_mirror_session_attribute;

} sai_mirror_api_t;

/**
 * @}
 */
#endif /** __SAIMIRROR_H_ */
