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
 * @file    saihashcustom.h
 *
 * @brief   This module defines SAI Hash custom interface
 */

#if !defined (__SAIHASH_CUSTOM_H_)
#define __SAIHASH_CUSTOM_H_

#include <saihash.h>

/**
 * @brief Custom Attribute Id for Fine Grained Hash Field
 *
 * @flags free
 */
typedef enum _sai_fine_grained_hash_field_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_FINE_GRAINED_HASH_FIELD_ATTR_CUSTOM_RANGE_START = SAI_FINE_GRAINED_HASH_FIELD_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of Fine Grained Hash Field attributes
     */
    SAI_FINE_GRAINED_HASH_FIELD_ATTR_CUSTOM_RANGE_END

} sai_fine_grained_hash_field_attr_custom_t;

/**
 * @brief Custom Attribute Id for Hash
 *
 * @flags free
 */
typedef enum _sai_hash_attr_custom_t
{
    /**
     * @brief Custom range base value start
     */
    SAI_HASH_ATTR_CUSTOM_RANGE_START = SAI_HASH_ATTR_CUSTOM_RANGE_BASE,
    
    /**
     * @brief Custom range end of Hash attributes
     */
    SAI_HASH_ATTR_CUSTOM_RANGE_END

} sai_hash_attr_custom_t;

#endif /* __SAIHASH_CUSTOM_H_ */