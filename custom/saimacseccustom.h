/**
 * Copyright (c) 2025 Microsoft Open Technologies, Inc.
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
 * @file    saimacseccustom.h
 *
 * @brief   This module defines SAI MACSEC custom interface
 */

#if !defined (__SAIMACSEC_CUSTOM_H_)
#define __SAIMACSEC_CUSTOM_H_

#include <saimacsec.h>

/**
 * @brief Custom Attribute Id for MACSEC
 *
 * @flags free
 */
typedef enum _sai_macsec_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_MACSEC_ATTR_CUSTOM_RANGE_START = SAI_MACSEC_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of MACSEC attributes
     */
    SAI_MACSEC_ATTR_CUSTOM_RANGE_END

} sai_macsec_attr_custom_t;

/**
 * @brief Custom Attribute Id for MACSEC Port
 *
 * @flags free
 */
typedef enum _sai_macsec_port_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_MACSEC_PORT_ATTR_CUSTOM_RANGE_START = SAI_MACSEC_PORT_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of MACSEC Port attributes
     */
    SAI_MACSEC_PORT_ATTR_CUSTOM_RANGE_END

} sai_macsec_port_attr_custom_t;

/**
 * @brief Custom Attribute Id for MACSEC Flow
 *
 * @flags free
 */
typedef enum _sai_macsec_flow_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_MACSEC_FLOW_ATTR_CUSTOM_RANGE_START = SAI_MACSEC_FLOW_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of MACSEC Flow attributes
     */
    SAI_MACSEC_FLOW_ATTR_CUSTOM_RANGE_END

} sai_macsec_flow_attr_custom_t;

/**
 * @brief Custom Attribute Id for MACSEC Secure Channel
 *
 * @flags free
 */
typedef enum _sai_macsec_sc_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_MACSEC_SC_ATTR_CUSTOM_RANGE_START = SAI_MACSEC_SC_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of MACSEC Secure Channel attributes
     */
    SAI_MACSEC_SC_ATTR_CUSTOM_RANGE_END

} sai_macsec_sc_attr_custom_t;

/**
 * @brief Custom Attribute Id for MACSEC Secure Association
 *
 * @flags free
 */
typedef enum _sai_macsec_sa_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_MACSEC_SA_ATTR_CUSTOM_RANGE_START = SAI_MACSEC_SA_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of MACSEC Secure Association attributes
     */
    SAI_MACSEC_SA_ATTR_CUSTOM_RANGE_END

} sai_macsec_sa_attr_custom_t;

#endif /* __SAIMACSEC_CUSTOM_H_ */