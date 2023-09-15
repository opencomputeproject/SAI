/**
 * Copyright (c) 2023 Microsoft Open Technologies, Inc.
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
 * @file    saiflow.h
 *
 * @brief   This module defines SAI Flow interface
 */

#if !defined (__SAIFLOW_H_)
#define __SAIFLOW_H_

#include <saitypes.h>

#ifndef _DASH_FLOW_SAI_H_
#define _DASH_FLOW_SAI_H_

#include <stdint.h>

typedef enum _sai_flow_table_attr_t {
    /* Start attribute marker */
    SAI_FLOW_TABLE_ATTR_START,

    /* Size of the table */
    SAI_FLOW_TABLE_ATTR_SIZE,
    /* Reset attribute */
    SAI_FLOW_TABLE_ATTR_RESET,
    /* Expiration time for table entries */
    SAI_FLOW_TABLE_ATTR_EXPIRE_TIME,
    /* Behavior to track TCP states */
    SAI_FLOW_TABLE_ATTR_BEHAVIOR_TRACK_TCP_STATES,
    /* Version attribute */
    SAI_FLOW_TABLE_ATTR_VERSION,
    /* Key Enable flag */
    SAI_FLOW_TABLE_ATTR_KEY_ENABLE_FLAG,

    /* End attribute marker */
    SAI_FLOW_TABLE_ATTR_END

} _sai_flow_table_attr_t;

/**
 * @brief Flow key for network and transport layer information.
 */
typedef struct _sai_dash_flow_key_t {
    /** @brief Source IP address */
    sai_ip_address_t src_ip;

    /** @brief Destination IP address */
    sai_ip_address_t dst_ip;

    /** @brief IP Protocol (e.g., TCP(6) or UDP(17)) */
    sai_uint8_t ip_protocol;

    /** @brief Transport Layer Information (TCP/UDP/ICMP) */
    sai_dash_ha_flow_l4_info_t l4_info;
} sai_dash_flow_key_t;

/**
 * @brief Union representing L4 flow information for various protocols.
 */
typedef union _sai_dash_ha_flow_l4_info_t {
    /** @brief TCP/UDP information */
    sai_dash_ha_flow_tcp_udp_info_t tcp_udp;

    /** @brief ICMP information */
    sai_dash_ha_flow_icmp_info_t icmp;
} sai_dash_ha_flow_l4_info_t;

/**
 * @brief Structure representing L4 information for TCP and UDP flows.
 */
typedef struct _sai_dash_ha_flow_tcp_udp_info_t {
    /** @brief Source port */
    sai_uint16_t src_port;

    /** @brief Destination port */
    sai_uint16_t dst_port;
} sai_dash_ha_flow_tcp_udp_info_t;

/**
 * @brief Structure representing L4 information for ICMP flows.
 */
typedef struct _sai_dash_ha_flow_icmp_info_t {
    /** @brief ICMP Type */
    sai_uint32_t type;

    /** @brief ICMP code */
    sai_uint32_t code;

    /** @brief ICMP ID */
    sai_uint32_t id;
} sai_dash_ha_flow_icmp_info_t;

typedef enum _sai_flow_state_metadata_attr_t {
    /* Starting marker for attributes */
    SAI_FLOW_ATTR_START,

    /* Version of the policy for this flow. Type: [uint32_t] */
    SAI_FLOW_STATE_ATTR_VERSION,

    /* Metadata of the policy results in protobuf format. Type: [sai_protobuf_t] */
    SAI_FLOW_STATE_ATTR_METADATA_PROTOBUF,

    /* Indicates if the flow entry is bi-directional. Type: [sai_uint8_t] Default: True (1) */
    SAI_FLOW_STATE_ATTR_BIDIRECTIONAL,
    
    /* Direction of the flow entry. Type: [sai_uint8_t] */
    SAI_FLOW_STATE_ATTR_DIRECTION,

    /* For single directional entries, this represents the key of the reverse direction. Type: [sai_dash_flow_key_t] */
    SAI_FLOW_STATE_ATTR_REVERSE_DIRECTION_KEY,
  
    /* Result of the policy. Type: [sai_dash_policy_result_t] */
    SAI_FLOW_METADATA_ATTR_POLICY_RESULT,

    /* Destination Protocol Address. Type: [sai_ip_address_t] */
    SAI_FLOW_METADATA_ATTR_DEST_PA,

    /* ID for metering class. Type: [sai_uint64_t] */
    SAI_FLOW_METADATA_ATTR_METERING_CLASS,

    /* Information required for rewriting. Type: [sai_dash_rewrite_info_t] */
    SAI_FLOW_METADATA_ATTR_REWRITE_INFO,

    /* Vendor-specific metadata details. Type: [sai_u8_list_t] */
    SAI_FLOW_METADATA_ATTR_VENDOR_METADATA,

    /* Ending marker for attributes */
    SAI_FLOW_METADATA_ATTR_END

} sai_flow_metadata_attr_t;


syntax = "proto3";

message SaiDashFlowMetadata {
    uint32 version = 1;
    SaiDashPolicyResult policy_result = 2;
    /* Destination PA IP address */
    string dest_pa = 3; 
    uint64 metering_class = 4;
    SaiDashHaRewriteInfo rewrite_info = 5;
    /* Vendor specific metadata */
    bytes vendor_metadata = 6; 
}

enum SaiDashPolicyResult {
    SAI_DASH_HA_POLICY_RESULT_NONE = 0;
    SAI_DASH_HA_POLICY_RESULT_ALLOW = 1;
    SAI_DASH_HA_POLICY_RESULT_DENY = 2;
}

enum SaiDashHaRewriteFlags {
    SAI_DASH_HA_REWRITE_NONE = 0; /* Default, unused value */
    SAI_DASH_HA_REWRITE_IFLOW_DMAC = 1;
    SAI_DASH_HA_REWRITE_IFLOW_SIP = 2;
    SAI_DASH_HA_REWRITE_IFLOW_SPORT = 4;
    SAI_DASH_HA_REWRITE_IFLOW_VNI = 8;
    SAI_DASH_HA_REWRITE_RFLOW_SIP = 16;
    SAI_DASH_HA_REWRITE_RFLOW_DIP = 32;
    SAI_DASH_HA_REWRITE_RFLOW_DPORT = 64;
    SAI_DASH_HA_REWRITE_RFLOW_SPORT = 128;
    SAI_DASH_HA_REWRITE_RFLOW_VNI = 256;
}

message SaiDashHaRewriteInfo {
    /* Bitmap of SaiDashHaRewriteFlags */
    uint64 rewrite_flags = 1; 
    /* Initiator Flow DMAC */
    string iflow_dmac = 2; 
    /* Initiator Flow Source IP address */
    string iflow_sip = 3; 
    /* Initiator Flow L4 Source Port */
    uint32 iflow_sport = 4; 
    /* Initiator Flow VNID */
    uint32 iflow_vni = 5; 
    /* Reverse Flow Source IP address */
    string rflow_sip = 6; 
    /* Reverse Flow Destination IP address */
    string rflow_dip = 7; 
    /* Reverse Flow Destination Port */
    uint32 rflow_dport = 8;
    /* Reverse Flow Source Port */
    uint32 rflow_sport = 9; 
    /* Reverse Flow VNID */
    uint32 rflow_vni = 10; 
}

typedef enum _sai_flow_state_query_type_t{
    SAI_FLOW_TABLE_ENTRY_QUERY_SRCIP,
    SAI_FLOW_TABLE_ENTRY_QUERY_SRCPORT,
    SAI_FLOW_TABLE_ENTRY_QUERY_DSTIP,
    SAI_FLOW_TABLE_ENTRY_QUERY_DSTPORT,
    SAI_FLOW_TABLE_ENTRY_QUERY_PROTO,
    SAI_FLOW_TABLE_ENTRY_QUERY_ALL,
    SAI_FLOW_TABLE_ENTRY_QUERY_ALL_AGED,
} sai_flow_state_query_type_t;


/**
 * @brief Create a new flow table
 *
 * @param[out] flow_table_id Flow table id allocated by the vendor
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 * @return sai_status_t Status code
 */
sai_status_t (*sai_flow_create_table_fn)(
    sai_object_id_t *flow_table_id,
    uint32_t attr_count,
    sai_attribute_t *attr_list
);

/**
 * @brief Remove a flow table
 *
 * @param[in] flow_table_id Flow table id to be removed
 * @return sai_status_t Status code
 */
sai_status_t (*sai_flow_remove_table_fn)(
    sai_object_id_t flow_table_id
);

/**
 * @brief Obtain the count of flow states in a flow table
 *
 * @param[in] flow_table_id Flow table id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list attr_list Array of attributes
 * @return sai_status_t Status code
 */
sai_status_t (*sai_flow_get_table_attribute_fn)(
    sai_object_id_t flow_table_id,
    uint32_t attr_count,
    sai_attribute_t *attr_list
);

/**
 * @brief Add single new entry to a certain flow table
 *
 * @param[in] flow_table_id Flow table id 
 * @param[in] flow_key Key of the flow
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list attr_list Array of attributes
 * @return sai_status_t Status code
 */
sai_status_t (*sai_flow_create_entry_fn)(
    sai_object_id_t flow_table_id,
    const sai_flow_key_t *flow_key,
    uint32_t attr_count,
    sai_attribute_t *attr_list
);

/**
 * @brief Add single new entry to a certain flow table
 *
 * @param[in] flow_table_id Flow table id 
 * @param[in] flow_count Count of entries
 * @param[in] flow_key Key of the flow
 * @param[in] attr_count[] Number of attributes
 * @param[in] attr_list[] Array of attributes
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to allocate the buffer.
 * @return sai_status_t Status code
 */
sai_status_t (*sai_flow_bulk_create_entry_fn)(
    sai_object_id_t flow_table_id,
    uint32_t flow_count,
    const sai_flow_key_t flow_key[],
    uint32_t attr_count[],
    sai_attribute_t *attr_list[],
    sai_bulk_op_error_mode_t mode,
    sai_status_t *object_statuses
);

/**
 * @brief Remove single entry in a certain flow table
 *
 * @param[in] flow_table_id Flow table id 
 * @param[in] flow_key Key of the flow
 * @return sai_status_t Status code
 */
sai_status_t (*sai_flow_remove_entry_fn)(
    sai_object_id_t flow_table_id,
    const sai_flow_key_t *flow_key
);

/**
 * @brief Set single entry in a certain flow table
 *
 * @param[in] flow_table_id Flow table id 
 * @param[in] flow_key Key of the flow
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list attr_list Array of attributes
 * @return sai_status_t Status code
 */
sai_status_t (*sai_flow_set_entry_fn)(
    sai_object_id_t flow_table_id,
    const sai_flow_key_t *flow_key,
    uint32_t *attr_count,
    sai_attribute_t *attr_list
);

/**
 * @brief Get single entry in a certain flow table
 *
 * @param[in] flow_table_id Flow table id 
 * @param[in] flow_key Key of the flow
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list attr_list Array of attributes
 * @return sai_status_t Status code
 */
sai_status_t (*sai_flow_get_entry_fn)(
    sai_object_id_t flow_table_id,
    const sai_flow_key_t *flow_key,
    uint32_t *attr_count,
    sai_attribute_t *attr_list
);

/**
 * @brief Request the vendor to iterate the flow_table and call the callback function.
 *
 * @param[in] flow_table_id Flow table id 
 * @param[in] flow_key Key of the flow
 * @param[in] type The type of queried flow state
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list attr_list Array of attributes
 * @param[in] callback_function The function to callback when iterate the flow table
 * @param[in] timeout The timeout expires
 * @param[out] finish Indicate if the bulk get is done
 * @return sai_status_t Status code
 * 
 * For the call_backfunction
 * @param[out] flow_table_id Flow table id
 * @param[out] flow_key Array of pointers to flow keys
 * @param[out] attr_count Array of pointers representing number of attributes for each key
 * @param[out] attr_list Array of pointers to attributes for each key
 */
sai_status_t (*sai_flow_bulk_get_entry_callback_fn)(
    sai_object_id_t flow_table_id,
    const sai_flow_key_t *flow_key,
    const sai_flow_state_query_type_t type,
    uint32_t *attr_count,
    sai_attribute_t *attr_list,
    void *callback_function(sai_object_id_t flow_table_id, 
                            const sai_flow_key_t flow_key[],
                            uint32_t *attr_count[],
                            sai_attribute_t *attr_list[]),
    int timeout,
    int *finish
);

/**
 * @brief Router entry methods table retrieved with sai_api_query()
 */
typedef struct _sai_flow_api_t
{
sai_flow_create_table_fn sai_flow_create_table;
sai_flow_remove_table_fn sai_flow_remove_table;
sai_flow_get_table_attribute_fn sai_flow_get_table_attribute;
sai_flow_create_entry_fn sai_flow_create_entry;
sai_flow_bulk_create_entry_fn sai_flow_bulk_create_entry;
sai_flow_remove_entry_fn sai_flow_remove_entry;
sai_flow_set_entry_fn sai_flow_set_entry;
sai_flow_get_entry_fn sai_flow_get_entry;
sai_flow_bulk_get_entry_callback_fn sai_flow_bulk_get_entry_callback;
} sai_flow_api_t;


/**
 * @}
 */
#endif /** __SAIFLOW_H_ */