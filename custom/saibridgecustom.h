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
 * @file    saibridgecustom.h
 *
 * @brief   This module defines SAI Bridge custom interface
 */

#if !defined (__SAIBRIDGE_CUSTOM_H_)
#define __SAIBRIDGE_CUSTOM_H_

#include <saibridge.h>


/**
 * @brief Custom Attribute Id for bridge port
 *
 * @flags free
 */
typedef enum _sai_bridge_port_attr_custom_t
{
    /**
     * @brief Custom range base value
     */
    SAI_BRIDGE_PORT_ATTR_CUSTOM_RANGE_START = SAI_BRIDGE_PORT_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of Bridge Port attributes
     */
    SAI_BRIDGE_PORT_ATTR_CUSTOM_RANGE_END

} sai_bridge_port_attr_custom_t;

/**
 * @brief Custom Attribute Id for bridge
 *
 * @flags free
 */
typedef enum _sai_bridge_attr_custom_t
{
    /**
     * @brief Custom range base value
     */
    SAI_BRIDGE_ATTR_CUSTOM_RANGE_START = SAI_BRIDGE_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of Bridge Port statistics attributes
     */
    SAI_BRIDGE_ATTR_CUSTOM_RANGE_END

} sai_bridge_attr_custom_t;

#endif /* __SAIBRIDGE_CUSTOM_H_ */