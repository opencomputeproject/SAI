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
#

from sai_test_base import T0TestBase
from sai_thrift.sai_headers import *
from ptf import config
from ptf.testutils import *
from ptf.thriftutils import *
import time


class SaiSanityTest(T0TestBase):
    """
    This is a test class use to trigger some basic verification when set up the basic t0 data configuration.
    """

    def setUp(self):
        """
        Test the basic setup proecss
        """
        T0TestBase.setUp(self,
                         is_reset_default_vlan=False,
                         is_recreate_bridge=False)

    def runTest(self):
        """
        Test the basic runTest proecss
        """
        self.test_flooding_to_ports()

    def tearDown(self):
        """
        Test the basic tearDown proecss
        """
        pass

    def test_flooding_to_ports(self):
        """
        Test fdb forwarding
        """
        unknown_mac1 = "00:01:01:99:99:99"
        unknown_mac2 = "00:01:02:99:99:99"
        pkt = simple_udp_packet(eth_dst=unknown_mac1,
                                eth_src=unknown_mac2,
                                ip_id=101,
                                ip_ttl=64)
        try:
            # Unknown mac, flooding to all the other ports.
            print("Sanity test, check all the ports be flooded.")
            send_packet(self, 1, pkt)
            verify_each_packet_on_multiple_port_lists(
                self, [pkt], [self.dev_port_list[2:]])
        finally:
            pass
