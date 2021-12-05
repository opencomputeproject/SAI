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
 * @file    saiacl.h
 *
 * @brief   This module defines SAI EBPF interface
 */

#if !defined (__SAIEBPF_H_)
#define __SAIEBPF_H_

/**
 * @brief State callback signature
 */
typedef void (* state_handler)(void *flow_ctx, void *global_ctx);

/**
 * @brief Set the next state
 *
 * Set the state callback to be executed when the stateful
 * table is applied to the next packet classified to
 * this flow.
 *
 * @param[in] state_handler state handler callback
 */
void sai_set_state(state_handler);

/**
 * @brief Defines a packet field ID
 */
typedef enum _sai_packet_field_t
{
    SAI_PACKET_FIELD_SRC_IPV6,

    SAI_PACKET_FIELD_DST_IPV6,

    SAI_PACKET_FIELD_INNER_SRC_IPV6,

    SAI_PACKET_FIELD_INNER_DST_IPV6,

    SAI_PACKET_FIELD_SRC_MAC,

    SAI_PACKET_FIELD_DST_MAC,

    SAI_PACKET_FIELD_SRC_IP,

    SAI_PACKET_FIELD_DST_IP,

    SAI_PACKET_FIELD_INNER_SRC_IP,

    SAI_PACKET_FIELD_INNER_DST_IP,

    SAI_PACKET_FIELD_L4_SRC_PORT,

    SAI_PACKET_FIELD_L4_DST_PORT,

    SAI_PACKET_FIELD_INNER_L4_SRC_PORT,

    SAI_PACKET_FIELD_INNER_L4_DST_PORT,

    SAI_PACKET_FIELD_IP_PROTOCOL,

    SAI_PACKET_FIELD_INNER_IP_PROTOCOL,

    SAI_PACKET_FIELD_TCP_FLAGS,

    SAI_PACKET_FIELD_INNER_TCP_FLAGS,

} sai_packet_field_t;

/**
 * @brief Get byte packet field
 *
 * @param[in] field_id field ID
 *
 * @return value of that field in the packet
 */
unsigned char sai_load_packet_field_u8(sai_packet_field_t field_id);

/**
 * @brief Get 2 byte packet field
 *
 * @param[in] field_id field ID
 *
 * @return value of that field in the packet
 */
unsigned short sai_load_packet_field_u16(sai_packet_field_t field_id);

/**
 * @brief Get 4 byte packet field
 *
 * @param[in] field_id field ID
 *
 * @return value of that field in the packet
 */
unsigned int sai_load_packet_field_u32(sai_packet_field_t field_id);

#endif /** __SAIEBPF_H_ */
