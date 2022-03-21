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

from warm_test_utils import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]

"""
Thrift SAI interface Switch tests
"""

class WarmAvailableIPv4RouteEntryTest(AvailableIPv4RouteEntryTest):
    """
    Warm boot Test for available IPv4 Route Entry.
    """
    @warm_setup
    def setUp(self):
        print("setUp WarmAvailableIPv4RouteEntryTest")
        AvailableIPv4RouteEntryTest.setUp(self)


    def setUp_starting(self):
        print("Skip setUp_starting WarmAvailableIPv4RouteEntryTest")


    def setUp_post_start(self):
        print("setUp_post_start WarmAvailableIPv4RouteEntryTest")
        SaiHelperBase.setUp(self)


    @warm_test
    def runTest(self):
        print("Run test WarmAvailableIPv4RouteEntryTest")
        super().runTest()


    def test_starting(self):
        print("Skip test_starting WarmAvailableIPv4RouteEntryTest")


    def test_post_start(self):
        print("test_post_start WarmAvailableIPv4RouteEntryTest")
        super().check_amount()


    @warm_teardown
    def tearDown(self):
        print("tearDown WarmAvailableIPv4RouteEntryTest")
        print("Skip the teardown and make a warm shut down for warm boot testing")
        self.warm_shutdown()


    def tearDown_starting(self):
        print("tearDown_starting WarmAvailableIPv4RouteEntryTest")
        print("Skip the teardown for warm boot testing")


    def tearDown_post_start(self):
        print("tearDown_post_start WarmAvailableIPv4RouteEntryTest")
        print("Skip the teardown after warm boot testing")
