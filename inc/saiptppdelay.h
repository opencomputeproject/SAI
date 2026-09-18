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
 * @file    saiptppdelay.h
 *
 * @brief   This module defines SAI PTP peer delay Mechanism
 */

#if !defined (__SAIPTPPDELAY_H_)
#define __SAIPTPPDELAY_H_

#include <saitypes.h>

/**
 * @defgroup SAIPTP SAI - PTP specific API definitions
 *
 * @{
 */

/**
 * @brief Peer delay type. Specifies if the port initiates, or responds, or both.
 */
typedef enum _sai_ptp_pdelay_type_t
{
    /** Port does not send peer delay request */
    SAI_PTP_PDELAY_TYPE_NONE,

    /** Port initiates peer delay requests */
    SAI_PTP_PDELAY_TYPE_INITIATOR,

    /** Port responds to peer delay requests */
    SAI_PTP_PDELAY_TYPE_RESPONDER,

    /** Port initiates and responds to peer delay requests */
    SAI_PTP_PDELAY_TYPE_BOTH,
} sai_ptp_pdelay_type_t;

/**
 * @brief PTP Network Protocol enumeration (as per IEEE 1588)
 */
typedef enum _sai_ptp_protocol_t
{
    SAI_PTP_PROTOCOL_NONE = 0,
    SAI_PTP_PROTOCOL_UDP_IPV4 = 1,
    SAI_PTP_PROTOCOL_UDP_IPV6 = 2,
    SAI_PTP_PROTOCOL_IEEE8023 = 3
} sai_ptp_protocol_t;

typedef enum _sai_ptp_pdelay_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PTP_PDELAY_ATTR_START,

    /**
     * @brief PTP Peer delay Port Address type
     *
     * @type sai_ptp_protocol_t
     * @flags CREATE_AND_SET
     * @default SAI_PTP_PROTOCOL_IEEE8023
     */
    SAI_PTP_PDELAY_ATTR_PTP_PROTOCOL = SAI_PTP_PDELAY_ATTR_START,

    /**
     * @brief MAC address of the port, to be used as source address in peer delay packet.
     *
     * If not specified, transmitted peer delay frames will use a source MAC of all zeros.
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default 00:00:00:00:00:00
     */
    SAI_PTP_PDELAY_ATTR_SRC_MAC,

    /**
     * @brief Peer delay L2 header length in bytes
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PTP_PDELAY_ATTR_L2_HEADER_LEN,

    /**
     * @brief Peer delay L2 header
     *
     * The number of valid header bytes in the list is specified by
     * #SAI_PTP_PDELAY_ATTR_L2_HEADER_LEN.
     *
     * @type sai_u8_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PTP_PDELAY_ATTR_PTP_L2_HEADER,

    /**
     * @brief Peer delay source IPv4 or IPv6 address
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     * @validonly SAI_PTP_PDELAY_ATTR_PTP_PROTOCOL == SAI_PTP_PROTOCOL_UDP_IPV4 or SAI_PTP_PDELAY_ATTR_PTP_PROTOCOL == SAI_PTP_PROTOCOL_UDP_IPV6
     */
    SAI_PTP_PDELAY_ATTR_NETWORK_ADDR,

    /**
     * @brief Peer delay packet Port Ingress VLAN (outer)
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 0
     */
    SAI_PTP_PDELAY_ATTR_INGRESS_VLAN_ID,

    /**
     * @brief Peer delay Packets TTL value
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 1
     * @validonly SAI_PTP_PDELAY_ATTR_PTP_PROTOCOL == SAI_PTP_PROTOCOL_UDP_IPV4 or SAI_PTP_PDELAY_ATTR_PTP_PROTOCOL == SAI_PTP_PROTOCOL_UDP_IPV6
     */
    SAI_PTP_PDELAY_ATTR_TTL,

    /**
     * @brief Peer delay Packets IP DSCP value
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_PTP_PDELAY_ATTR_PTP_PROTOCOL == SAI_PTP_PROTOCOL_UDP_IPV4 or SAI_PTP_PDELAY_ATTR_PTP_PROTOCOL == SAI_PTP_PROTOCOL_UDP_IPV6
     */
    SAI_PTP_PDELAY_ATTR_IP_DSCP,

    /**
     * @brief Peer delay Packets Domain Number
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PTP_PDELAY_ATTR_DOMAIN_NUMBER,

    /**
     * @brief Peer delay interval, logarithmic
     *
     * @type sai_int32_t
     * @flags CREATE_AND_SET
     * @default -3
     */
    SAI_PTP_PDELAY_ATTR_LOG_INTERVAL,

    /**
     * @brief Peer delay port type
     *
     * @type sai_ptp_pdelay_type_t
     * @flags CREATE_AND_SET
     * @default SAI_PTP_PDELAY_TYPE_NONE
     */
    SAI_PTP_PDELAY_ATTR_PORT_TYPE,

    /**
     * @brief Enable or disable the local peer delay measurement engine
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PTP_PDELAY_ATTR_ENABLE,

    /**
     * @brief End of attributes
     */
    SAI_PTP_PDELAY_ATTR_END,

    /** Custom range base value */
    SAI_PTP_PDELAY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PTP_PDELAY_ATTR_CUSTOM_RANGE_END

} sai_ptp_pdelay_attr_t;

/**
 * @brief Create a PTP peer delay instance.
 *
 * @param[out] ptp_pdelay_id Peer delay Instance id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a different
 * error code is returned.
 */
typedef sai_status_t (*sai_create_ptp_pdelay_fn)(
        _Out_ sai_object_id_t *ptp_pdelay_id,
        _In_ sai_object_id_t switch_id,
        _In_ sai_uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove a PTP peer delay instance.
 *
 * @param[in] ptp_pdelay_id Peer delay instance id
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a
 * different error code is returned.
 */
typedef sai_status_t (*sai_remove_ptp_pdelay_fn)(
        _In_ sai_object_id_t ptp_pdelay_id);

/**
 * @brief Set PTP peer delay instance attributes.
 *
 * @param[in] ptp_pdelay_id Peer delay instance id
 * @param[in] attr Structure containing ID and value of attribute
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a
 * different error code is returned.
 */
typedef sai_status_t (*sai_set_ptp_pdelay_attribute_fn)(
        _In_ sai_object_id_t ptp_pdelay_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get PTP peer delay instance attributes.
 *
 * @param[in] ptp_pdelay_id Peer delay instance id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Value of attribute
 *
 * @return #SAI_STATUS_SUCCESS if operation is successful otherwise a
 * different error code is returned.
 */
typedef sai_status_t (*sai_get_ptp_pdelay_attribute_fn)(
        _In_ sai_object_id_t ptp_pdelay_id,
        _In_ sai_uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_ptp_pdelay_api_t
{
    sai_create_ptp_pdelay_fn        create_ptp_pdelay;
    sai_remove_ptp_pdelay_fn        remove_ptp_pdelay;
    sai_set_ptp_pdelay_attribute_fn set_ptp_pdelay_attribute;
    sai_get_ptp_pdelay_attribute_fn get_ptp_pdelay_attribute;
} sai_ptp_pdelay_api_t;

/**
 * @}
 */
#endif /** __SAIPTPPDELAY_H_ */
