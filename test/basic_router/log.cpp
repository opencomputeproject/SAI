/*
 * Copyright (c) 2015 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc
 *
 *
 */
#include "log.h"

int curr_log_level = TEST_INFO;

void LOGG(int priority, const char* title, const char* format, ...)
{
    if (priority > curr_log_level)
        return;

    char dest[1024 * 16];
    char level[256];
    va_list ap;
    va_start(ap, format);
    vsprintf(dest, format, ap);
    va_end(ap);

    switch (priority)
    {
    case TEST_DEBUG:
        sprintf(level, "DEBUG");
        break;

    case TEST_INFO:
        sprintf(level, "INFO");
        break;

    case TEST_NOTICE:
        sprintf(level, "NOTICE");
        break;

    case TEST_WARNING:
        sprintf(level, "WARNING");
        break;

    case TEST_ERR:
        sprintf(level, "ERROR");
        break;

    case TEST_CRIT:
        sprintf(level, "CRITICAL");
        break;

    case TEST_ALERT:
        sprintf(level, "ALERT");
        break;

    case TEST_EMERG:
        sprintf(level, "EMERGENCY");
        break;

    default:
        break;
    }

    printf("%s %s ", level, title);
    printf(dest);
}
