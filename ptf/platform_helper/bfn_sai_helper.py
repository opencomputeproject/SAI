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


"""
This file contains class for bfn specified functions.
"""
from platform_helper.common_sai_helper import *

class BfnSaiHelper(CommonSaiHelper):
    """
    This class contains Barefoot(bfn) specified functions for the platform setup and test context configuration.
    """
    platform = 'bfn'

    def recreate_ports(self):
        print("BfnSaiHelper::recreate_ports")
        if 'port_config_ini' in self.test_params:
            if 'createPorts_has_been_called' not in config:
                self.createPorts()
                # check if ports became UP
                #self.checkPortsUp()
                config['createPorts_has_been_called'] = 1
        wait_sec = 5
        print("Waiting for ports to get ready, {} seconds ...".format(wait_sec))
        time.sleep(wait_sec)
