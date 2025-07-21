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
 * @file    saifdbcustom.h
 *
 * @brief   This module defines SAI FDB custom interface
 */

#if !defined (__SAIFDB_CUSTOM_H_)
#define __SAIFDB_CUSTOM_H_

#include <saifdb.h>

/**
 * @brief Custom Attribute Id for FDB entry
 *
 * @flags free
 */
typedef enum _sai_fdb_entry_attr_custom_t {
    /**
     * @brief Custom range start of FDB entry attributes
     */
    SAI_FDB_ENTRY_ATTR_CUSTOM_RANGE_START = SAI_FDB_ENTRY_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of FDB entry attributes
     */
    SAI_FDB_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_fdb_entry_attr_custom_t;

/**
 * @brief Custom Attribute Id for FDB flush
 *
 * @flags free
 */
typedef enum _sai_fdb_flush_attr_custom_t {
    /**
     * @brief Custom range start of FDB flush attributes
     */
    SAI_FDB_FLUSH_ATTR_CUSTOM_RANGE_START = SAI_FDB_FLUSH_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of FDB flush attributes
     */
    SAI_FDB_FLUSH_ATTR_CUSTOM_RANGE_END

} sai_fdb_flush_attr_custom_t;

#endif /* __SAIFDB_CUSTOM_H_ */