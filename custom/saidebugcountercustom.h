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
 * @file    saidebugcountercustom.h
 *
 * @brief   This module defines SAI Debug Counter custom interface
 */

#if !defined (__SAIDEBUGCOUNTER_CUSTOM_H_)
#define __SAIDEBUGCOUNTER_CUSTOM_H_

#include <saidebugcounter.h>

/**
 * @brief Custom Attribute Id for debug counter
 *
 * @flags free
 */
typedef enum _sai_debug_counter_attr_custom_t
{
    /**
     * @brief Custom range base value
     */
    SAI_DEBUG_COUNTER_ATTR_CUSTOM_RANGE_START = SAI_DEBUG_COUNTER_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of Debug Counter attributes
     */
    SAI_DEBUG_COUNTER_ATTR_CUSTOM_RANGE_END

} sai_debug_counter_attr_custom_t;
#endif /* __SAIDEBUGCOUNTER_CUSTOM_H_ */