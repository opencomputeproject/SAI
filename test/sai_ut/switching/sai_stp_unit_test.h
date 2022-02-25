/************************************************************************
* Copyright (c) 2015 Dell Inc.
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
*
*
* Module Name:
*
*    sai_stp_unit_test.h
*
* Abstract:
*
*    This file contains declarations to be used to STP testing module
*
*************************************************************************/

#ifndef __SAI_STP_UNIT_TEST_H__
#define __SAI_STP_UNIT_TEST_H__

#include "gtest/gtest.h"

extern "C" {
#include "saiswitch.h"
#include "saivlan.h"
#include "saistp.h"
#include "saitypes.h"
}

class stpTest : public ::testing::Test
{
    public:
        static void SetUpTestCase();

        static sai_object_id_t          sai_stp_port_id_get (uint32_t port_index);
        static sai_object_id_t          sai_stp_invalid_port_id_get ();

        static sai_switch_api_t         *p_sai_switch_api_tbl;
        static sai_stp_api_t            *p_sai_stp_api_tbl;
        static sai_vlan_api_t           *p_sai_vlan_api_tbl;
        static sai_object_id_t          sai_port_id_1;
        static sai_object_id_t          sai_invalid_port_id;

};
#endif /* __SAI_STP_UNIT_TEST_H__ */
