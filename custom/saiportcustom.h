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
 * @file    saiportcustom.h
 *
 * @brief   This module defines SAI Port custom interface
 */

#if !defined (__SAIPORT_CUSTOM_H_)
#define __SAIPORT_CUSTOM_H_

#include <saiport.h>

/**
 * @brief Custom Attribute Id for Port
 *
 * @flags free
 */
typedef enum _sai_port_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_PORT_ATTR_CUSTOM_RANGE_START = SAI_PORT_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of Port attributes
     */
    SAI_PORT_ATTR_CUSTOM_RANGE_END

} sai_port_attr_custom_t;

/**
 * @brief Custom Attribute Id for Port Pool
 *
 * @flags free
 */
typedef enum _sai_port_pool_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_PORT_POOL_ATTR_CUSTOM_RANGE_START = SAI_PORT_POOL_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of Port Pool attributes
     */
    SAI_PORT_POOL_ATTR_CUSTOM_RANGE_END

} sai_port_pool_attr_custom_t;

/**
 * @brief Custom Attribute Id for Port Serdes
 *
 * @flags free
 */
typedef enum _sai_port_serdes_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_PORT_SERDES_ATTR_CUSTOM_RANGE_START = SAI_PORT_SERDES_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of Port Serdes attributes
     */
    SAI_PORT_SERDES_ATTR_CUSTOM_RANGE_END

} sai_port_serdes_attr_custom_t;

/**
 * @brief Custom Attribute Id for Port Connector
 *
 * @flags free
 */
typedef enum _sai_port_connector_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_PORT_CONNECTOR_ATTR_CUSTOM_RANGE_START = SAI_PORT_CONNECTOR_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of Port Connector attributes
     */
    SAI_PORT_CONNECTOR_ATTR_CUSTOM_RANGE_END

} sai_port_connector_attr_custom_t;

#endif /* __SAIPORT_CUSTOM_H_ */