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
#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>

#define TEST_EMERG      LOG_EMERG
#define TEST_ALERT      LOG_ALERT
#define TEST_CRIT       LOG_CRIT
#define TEST_ERR        LOG_ERR
#define TEST_WARNING    LOG_WARNING
#define TEST_NOTICE     LOG_NOTICE
#define TEST_INFO       LOG_INFO
#define TEST_DEBUG      LOG_DEBUG

#define TESTCASE        ""
#define FRAMEWORK       "FRAMEWORK"
#define SETL3           "SETUPL3INTF"
#define ROUTE           "ROUTE"
#define NEIGHBOR        "NEIGHBOR"
#define NXTHG           "NEXTHOPGRP"
#define NEXTHOP         "NEXTHOP"
#define FDB             "FDB"

extern void LOGG(int priority, const char* title, const char* format, ...);
extern int curr_log_level;
