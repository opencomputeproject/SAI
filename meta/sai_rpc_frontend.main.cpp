/**
 * Copyright (c) 2023 Microsoft Open Technologies, Inc.
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
 * @file    sai_rpc_frontend.main.cpp
 *
 * @brief   This module contains SAI RPC main function just for linkage test
 */

extern "C" {
#include "sai.h"
int start_sai_thrift_rpc_server(int port);
}

#include <unistd.h>

#include <map>
#include <set>
#include <string>

// this is just dummy frontend main, just to see if sai_rpc_frontend.cpp will
// compile and link successfully

#define SWITCH_SAI_THRIFT_RPC_SERVER_PORT 9092

std::map<std::string, std::string> gProfileMap;
std::map<std::set<int>, std::string> gPortMap;

sai_object_id_t gSwitchId;

int main(int argc, char **argv)
{
    sai_api_initialize(0, 0);

    start_sai_thrift_rpc_server(SWITCH_SAI_THRIFT_RPC_SERVER_PORT);

    while (true)
    {
        pause();
    }

    return 0;
}
