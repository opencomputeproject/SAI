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
 * @file    saidtelcustom.h
 *
 * @brief   This module defines SAI DTEL custom interface
 */

#if !defined (__SAIDTEL_CUSTOM_H_)
#define __SAIDTEL_CUSTOM_H_

#include <saidtel.h>

/**
 * @brief Custom Attribute Id for DTEL
 *
 * @flags free
 */
typedef enum _sai_dtel_attr_custom_t {
    /**
     * @brief Custom range base value
     */
    SAI_DTEL_ATTR_CUSTOM_RANGE_START = SAI_DTEL_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of DTEL attributes
     */
    SAI_DTEL_ATTR_CUSTOM_RANGE_END

} sai_dtel_attr_custom_t;

/**
 * @brief Custom Attribute Id for DTEL queue report
 *
 * @flags free
 */
typedef enum _sai_dtel_queue_report_attr_custom_t
{
    /**
     * @brief Custom range base value
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_CUSTOM_RANGE_START = SAI_DTEL_QUEUE_REPORT_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of DTEL queue report attributes
     */
    SAI_DTEL_QUEUE_REPORT_ATTR_CUSTOM_RANGE_END

} sai_dtel_queue_report_attr_custom_t;

/**
 * @brief Custom Attribute Id for DTEL int session
 *
 * @flags free
 */
typedef enum _sai_dtel_int_session_attr_custom_t {
    /**
     * @brief Custom range base value
     */
    SAI_DTEL_INT_SESSION_ATTR_CUSTOM_RANGE_START = SAI_DTEL_INT_SESSION_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of DTEL int session attributes
     */
    SAI_DTEL_INT_SESSION_ATTR_CUSTOM_RANGE_END

} sai_dtel_int_session_attr_custom_t;

/**
 * @brief Custom Attribute Id for DTEL report session
 *
 * @flags free
 */
typedef enum _sai_dtel_report_session_attr_custom_t
{
    /**
     * @brief Custom range base value
     */
    SAI_DTEL_REPORT_SESSION_ATTR_CUSTOM_RANGE_START = SAI_DTEL_REPORT_SESSION_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of DTEL report session attributes
     */
    SAI_DTEL_REPORT_SESSION_ATTR_CUSTOM_RANGE_END

} sai_dtel_report_session_attr_custom_t;

/**
 * @brief Custom Attribute Id for DTEL event
 *
 * @flags free
 */
typedef enum _sai_dtel_event_attr_custom_t
{
    /**
     * @brief Custom range base value
     */
    SAI_DTEL_EVENT_ATTR_CUSTOM_RANGE_START = SAI_DTEL_EVENT_ATTR_CUSTOM_RANGE_BASE,

    /**
     * @brief Custom range end of DTEL event attributes
     */
    SAI_DTEL_EVENT_ATTR_CUSTOM_RANGE_END

} sai_dtel_event_attr_custom_t;
#endif /* __SAIDTEL_CUSTOM_H_ */