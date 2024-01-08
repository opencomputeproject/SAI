/**
 * Copyright (c) 2024 Microsoft Open Technologies, Inc.
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
 * @file    saiicmpecho.h
 *
 * @brief   This module defines SAI ICMP ECHO interface
 */

#if !defined (__SAIICMPECHO_H_)
#define __SAIICMPECHO_H_

#include <saitypes.h>

/**
 * @defgroup SAIICMPECHO SAI - ICMP ECHO specific public APIs and data structures
 *
 * @{
 */

/** 
 * @brief SAI ICMP echo session state 
 */ 
typedef enum _sai_icmp_echo_session_state_t 
{ 
   /** ICMP echo  session is DOWN */ 
    SAI_ICMP_ECHO_SESSION_DOWN, 

   /** ICMP echo  session is DOWN */ 
    SAI_ICMP_ECHO_SESSION_UP, 

} sai_icmp_echo_session_state_t;

/** 
 * @brief Defines the operational status of the ICMP echo session 
 */ 
typedef struct _sai_icmp_echo_session_state_notification_t 

{ 
    /** ICMP echo Session id */ 
    sai_object_id_t icmp_echo_session_id; 

    /** ICMP echo session state */ 
    sai_icmp_session_state_t session_state; 

} sai_icmp_echo_session_state_notification_t;

/** 
 * @brief SAI attributes for ICMP echo session 
 */ 
typedef enum _sai_icmp_echo_session_attr_t 

{ 
    /** 
     * @brief Start of attributes 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_START, 

    /** 
     * @brief Hardware lookup valid 
     * 
     * @type bool 
     * @flags CREATE_ONLY 
     * @default true 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_VIRTUAL_ROUTER = SAI_ICMP_ECHO_SESSION_ATTR_START, 

    /** 
     * @brief Virtual Router 
     * 
     * @type sai_object_id_t 
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET 
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER 
     * @condition SAI_ICMP_ECHO_SESSION_ATTR_HW_LOOKUP_VALID == true 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_VIRTUAL_ROUTER, 

    /** 
     * @brief Destination Port 
     * 
     * @type sai_object_id_t 
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET 
     * @objects SAI_OBJECT_TYPE_PORT 
     * @condition SAI_ICMP_ECHO_SESSION_ATTR_HW_LOOKUP_VALID == false 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_PORT, 

    /** 
     * @brief ICMP echo session unique identifier  
     * 
     * @type sai_uint32_t 
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_ID, 
 
    /** 
     * @brief IP header TOS 
     * 
     * @type sai_uint8_t 
     * @flags CREATE_AND_SET 
     * @default 0 
     */ 

    SAI_ICMP_ECHO_SESSION_ATTR_TOS, 

    /** 
     * @brief IP header TTL 
     * 
     * @type sai_uint8_t 
     * @flags CREATE_AND_SET 
     * @default 255 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_TTL, 

    /** 
     * @brief IP header version 
     * 
     * @type sai_uint8_t 
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_IPHDR_VERSION, 

    /** 
     * @brief Source IP 
     * 
     * @type sai_ip_address_t 
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_SRC_IP_ADDRESS, 

    /** 
     * @brief Destination IP 
     * 
     * @type sai_ip_address_t 
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_DST_IP_ADDRESS, 

    /** 
     * @brief L2 source MAC address 
     * 
     * @type sai_mac_t 
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET 
     * @condition SAI_ICMP_ECHO_SESSION_ATTR_HW_LOOKUP_VALID == false 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_SRC_MAC_ADDRESS, 

    /** 
     * @brief L2 destination MAC address 
     * 
     * @type sai_mac_t 
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET 
     * @condition SAI_ICMP_ECHO_SESSION_ATTR_HW_LOOKUP_VALID == false 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_DST_MAC_ADDRESS, 

    /** 
     * @brief Minimum Transmit interval in microseconds 
     * 
     * @type sai_uint32_t 
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_MIN_TX, 

    /** 
     * @brief Minimum Receive interval in microseconds 
     * 
     * @type sai_uint32_t 
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_MIN_RX, 

    /** 
     * @brief Detect time Multiplier 
     * 
     * @type sai_uint8_t 
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_MULTIPLIER, 

    /** 
     * @brief ICMP ECHO Session state 
     * 
     * @type sai_icmp_echo_session_state_t 
     * @flags READ_ONLY 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_STATE, 

    /** 
     * @brief End of attributes 
     */ 
    SAI_ICMP_ECHO_SESSION_ATTR_END, 

    /** Custom range base value */ 
    SAI_ICMP_ECHO_SESSION_ATTR_CUSTOM_RANGE_START = 0x10000000, 

    /** End of custom range base */ 
    SAI_ICMP_ECHO_SESSION_ATTR_CUSTOM_RANGE_END 

} sai_icmp_echo_session_attr_t; 

/** 
 * @brief ICMP ECHO Session counter IDs in sai_get_icmp_echo_session_stats() call 
 */ 
typedef enum _sai_icmp_echo_session_stat_t 
{ 
    /** Ingress packet stat count */ 
    SAI_ICMP_ECHO_SESSION_STAT_IN_PACKETS, 

    /** Egress packet stat count */ 
    SAI_ICMP_ECHO_SESSION_STAT_OUT_PACKETS  

} sai_icmp_echo_session_stat_t; 

/** 
 * @brief Create icmp echo session. 
 * 
 * @param[out] icmp_echo_session_id ICMP ECHO session id 
 * @param[in] switch_id Switch id 
 * @param[in] attr_count Number of attributes 
 * @param[in] attr_list Value of attributes 
 * 
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different 
 * error code is returned. 
 */ 
typedef sai_status_t (*sai_create_icmp_echo_session_fn)( 
        _Out_ sai_object_id_t *icmp_echo_session_id, 
        _In_ sai_object_id_t switch_id, 
        _In_ uint32_t attr_count, 
        _In_ const sai_attribute_t *attr_list);

/** 
 * @brief Remove ICMP ECHO session. 
 * 
 * @param[in] icmp_echo_session_id ICMP ECHO session id 
 * 
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different 
 * error code is returned. 
 */ 
typedef sai_status_t (*sai_remove_icmp_echo_session_fn)( 
        _In_ sai_object_id_t icmp_echo_session_id); 

/** 
 * @brief Set ICMP ECHO session attributes. 
 * 
 * @param[in] icmp_echo_session_id ICMP ECHO session id 
 * @param[in] attr Value of attribute 
 * 
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different 
 * error code is returned. 
 */ 
typedef sai_status_t (*sai_set_icmp_echo_session_attribute_fn)( 
        _In_ sai_object_id_t icmp_echo_session_id, 
        _In_ const sai_attribute_t *attr); 

/** 
 * @brief Get ICMP ECHO session attributes. 
 * 
 * @param[in] icmp_echo_session_id ICMP ECHO session id 
 * @param[in] attr_count Number of attributes 
 * @param[inout] attr_list Value of attribute 
 * 
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different 
 * error code is returned. 
 */ 
typedef sai_status_t (*sai_get_icmp_echo_session_attribute_fn)( 
        _In_ sai_object_id_t icmp_echo_session_id, 
        _In_ uint32_t attr_count, 
        _Inout_ sai_attribute_t *attr_list); 

/** 
 * @brief Get ICMP ECHO session statistics counters. 
 * 
 * @param[in] icmp_echo_session_id ICMP ECHO session id 
 * @param[in] number_of_counters Number of counters in the array 
 * @param[in] counter_ids Specifies the array of counter ids 
 * @param[out] counters Array of resulting counter values. 
 * 
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error 
 */ 
typedef sai_status_t (*sai_get_icmp_echo_session_stats_fn)( 
        _In_ sai_object_id_t icmp_echo_session_id, 
        _In_ uint32_t number_of_counters, 
        _In_ const sai_icmp_echo_session_stat_t *counter_ids, 
        _Out_ uint64_t *counters);

/** 
 * @brief Clear ICMP ECHO session statistics counters. 
 * 
 * @param[in] icmp_echo_session_id ICMP ECHO session id 
 * @param[in] number_of_counters Number of counters in the array 
 * @param[in] counter_ids Specifies the array of counter ids 
 * 
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error 
 */ 
typedef sai_status_t (*sai_clear_icmp_echo_session_stats_fn)( 
        _In_ sai_object_id_t icmp_echo_session_id, 
        _In_ uint32_t number_of_counters, 
        _In_ const sai_icmp_echo_session_stat_t *counter_ids); 

/** 
 * @brief ICMP ECHO session state change notification 
 * 
 * Passed as a parameter into sai_initialize_switch() 
 * 
 * @count data[count] 
 * 
 * @param[in] count Number of notifications 
 * @param[in] data Array of ICMP ECHO session state 
 */ 
typedef void (*sai_icmp_echo_session_state_change_notification_fn)( 
        _In_ uint32_t count, 
        _In_ sai_icmp_echo_session_state_notification_t *data); 

/** 
 * @brief ICMP ECHO method table retrieved with sai_api_query() 
 */ 
typedef struct _sai_icmp_echo_api_t 
{ 
    sai_create_icmp_echo_session_fn            create_icmp_echo_session; 
    sai_remove_icmp_echo_session_fn            remove_icmp_echo_session; 
    sai_set_icmp_echo_session_attribute_fn     set_icmp_echo_session_attribute; 
    sai_get_icmp_echo_session_attribute_fn     get_icmp_echo_session_attribute; 
    sai_get_icmp_echo_session_stats_fn         get_icmp_echo_session_stats; 
    sai_clear_icmp_echo_session_stats_fn       clear_icmp_echo_session_stats; 

} sai_icmp_echo_api_t; 

/**
 * @}
 */
#endif /** __SAIICMPECHO_H_ */
