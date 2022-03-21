# Copyright (c) 2021 Microsoft Open Technologies, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing
#    permissions and limitations under the License.
#
#    Microsoft would like to thank the following companies for their review and
#    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
#    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.


from platform_helper.common_helper.saiswitch_helper import *
from sai_base_test import * # pylint: disable=wildcard-import; lgtm[py/polluting-import]

"""
Brcm sai switch helper for sai switch tests.
"""

class BrcmSaiSwitchHelper(SaiSwitchHelper):
    
    def create_route_entry_from_default_vrf(self, client):
        print("BrcmSaiSwitchHelper::create_route_entry_from_default_vrf.")
        route_entry = sai_thrift_route_entry_t(
            switch_id=client.switch_id,
            vr_id = client.default_vrf,
            destination=sai_ipprefix('0.0.0.0/0'))
        status = sai_thrift_create_route_entry(
            client.client, route_entry, next_hop_id=client.nhop)
        client.assertEqual(status, SAI_STATUS_SUCCESS)
        client.route_number += 1
