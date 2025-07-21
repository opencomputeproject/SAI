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
 * @file    saipoecustom.h
 *
 * @brief   This module defines SAI POE custom interface
 */

#if !defined (__SAIPOE_CUSTOM_H_)
#define __SAIPOE_CUSTOM_H_

#include <saipoe.h>

/**
 * @brief Custom Attribute Id for POE Device
 *
 * @flags free
 */
typedef enum _sai_poe_device_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_POE_DEVICE_ATTR_CUSTOM_RANGE_START = SAI_POE_DEVICE_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of POE Device attributes
     */
    SAI_POE_DEVICE_ATTR_CUSTOM_RANGE_END

} sai_poe_device_attr_custom_t;

/**
 * @brief Custom Attribute Id for POE PSE
 *
 * @flags free
 */
typedef enum _sai_poe_pse_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_POE_PSE_ATTR_CUSTOM_RANGE_START = SAI_POE_PSE_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of POE PSE attributes
     */
    SAI_POE_PSE_ATTR_CUSTOM_RANGE_END

} sai_poe_pse_attr_custom_t;

/**
 * @brief Custom Attribute Id for POE Port
 *
 * @flags free
 */
typedef enum _sai_poe_port_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_POE_PORT_ATTR_CUSTOM_RANGE_START = SAI_POE_PORT_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of POE Port attributes
     */
    SAI_POE_PORT_ATTR_CUSTOM_RANGE_END

} sai_poe_port_attr_custom_t;

#endif /* __SAIPOE_CUSTOM_H_ */