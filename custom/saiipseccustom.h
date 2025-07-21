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
 * @file    saiipseccustom.h
 *
 * @brief   This module defines SAI IPSec custom interface
 */

#if !defined (__SAIIPSEC_CUSTOM_H_)
#define __SAIIPSEC_CUSTOM_H_

#include <saiipsec.h>

/**
 * @brief Custom Attribute Id for IPSec
 *
 * @flags free
 */
typedef enum _sai_ipsec_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_IPSEC_ATTR_CUSTOM_RANGE_START = SAI_IPSEC_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of IPSec attributes
     */
    SAI_IPSEC_ATTR_CUSTOM_RANGE_END

} sai_ipsec_attr_custom_t;

/**
 * @brief Custom Attribute Id for IPSec Port
 *
 * @flags free
 */
typedef enum _sai_ipsec_port_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_IPSEC_PORT_ATTR_CUSTOM_RANGE_START = SAI_IPSEC_PORT_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of IPSec Port attributes
     */
    SAI_IPSEC_PORT_ATTR_CUSTOM_RANGE_END

} sai_ipsec_port_attr_custom_t;

/**
 * @brief Custom Attribute Id for IPSec Security Association
 *
 * @flags free
 */
typedef enum _sai_ipsec_sa_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_IPSEC_SA_ATTR_CUSTOM_RANGE_START = SAI_IPSEC_SA_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of IPSec Security Association attributes
     */
    SAI_IPSEC_SA_ATTR_CUSTOM_RANGE_END

} sai_ipsec_sa_attr_custom_t;

#endif /* __SAIIPSEC_CUSTOM_H_ */