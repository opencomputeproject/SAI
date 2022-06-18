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
#

from sai_test_base import T0TestBase
from sai_thrift.sai_headers import *
from ptf import config
from ptf.testutils import *
from ptf.thriftutils import *
from sai_utils import *

class Common_Forwarding_Test(T0TestBase):
    """
    Verify the basic fdb forwarding.
    """

    def setUp(self):
        """
        Test the basic setup process
        """
        # this process contains the switch_init process
        T0TestBase.setUp(self, is_remove_vlan=False)

    def runTest(self):
        """
        Test fdb forwarding
        """
        try:
            print("FDB basic forwarding test.")
            for index in range(2, len(self.local_server_mac_list)):
                print("L2 Forwarding from {} to port: {}".format(
                    self.dev_port_list[1], 
                    self.dev_port_list[index]))
                pkt = simple_udp_packet(eth_dst=self.local_server_mac_list[index],
                                        eth_src=self.local_server_mac_list[1],
                                        ip_id=101,
                                        ip_ttl=64)
                    
                send_packet(self, self.dev_port_list[1], pkt)
                verify_packet(self, pkt, self.dev_port_list[index])
        finally:
            pass

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        sai_thrift_flush_fdb_entries(
            self.client, entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)
