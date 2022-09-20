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

#if !defined (__SAICALLBACK_H_)
#define __SAICALLBACK_H_

#ifndef __section
# define __section(NAME)                  \
   __attribute__((section(NAME), used))
#endif

typedef struct _sai_ethernet_t {
    sai_u8_t[6] src_mac;
    sai_u8_t[6] src_mac;
} _sai_ethernet_t;

typedef struct _sai_ipv4_t {
    sai_u32_t src_addr;
    sai_u32_t dst_addr;
    sai_u8_t ipproto;
} _sai_ipv4_t;

typedef struct _sai_tcp_t {
    sai_u16_t sport;
    sai_u16_t dport;
    sai_u16_t flags;
} _sai_tcp_t;

/**
 * @brief Defines parsed headers
 */
typedef struct _sai_parsed_headers_t
{
    sai_ethernet_t ethernet;
    
    sai_ipv4_t ipv4;
    
    sai_tcp_t tcp;
    
    sai_ethernet_t inner_ethernet;
    
    sai_ipv4_t inner_ipv4;
    
    sai_tcp_t inner_tcp;

} sai_parsed_headers_t;

/**
 * @brief State callback signature
 */
void handler(void *flow_ctx, void *global_ctx, const struct sai_parsed_headers_t *parsed_headers, sai_u32_t *packet_metatata);

/**
 * @brief Set the next state
 *
 * Set the state callback to be executed when the stateful
 * table is applied to the next packet classified to
 * this flow.
 *
 * @param[in] state_handler state handler callback
 */
void sai_set_state(char *state);

#endif /** __SAICALLBACK_H_ */
