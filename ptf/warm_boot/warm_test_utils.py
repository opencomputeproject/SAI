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


import os
import sys

THIS_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(THIS_DIR, '..'))
from sai_base_test import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]
from saisanity import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]
from saiswitch import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]

def warm_test(f):
    """
    Method decorator for the method on warm testing.
    
    Depends on parameters [test_reboot_mode] and [test_reboot_stage].
    Runs different method, test_starting, setUp_post_start and runTest
    """
    def test_director(inst, *args):
        if inst.test_reboot_stage == 'starting':
            return inst.test_starting()
        if inst.test_reboot_stage == 'post':
            return inst.setUp_post_start()
        return f(inst)
    return test_director


def warm_setup(f):
    """
    Method decorator for the method on warm setup.
    
    Depends on parameters [test_reboot_mode] and [test_reboot_stage].
    Runs different method, setUp_starting, setUp_post_start and setUp
    """
    def test_director(inst, *args):
        inst.test_params = test_params_get()
        inst.loadTestRebootMode()
        if inst.test_reboot_stage == 'starting':
            return inst.setUp_starting()
        if inst.test_reboot_stage == 'post':
            return inst.setUp_post_start()
        return f(inst)
    return test_director


def warm_teardown(f):
    """
    Method decorator for the method on warm tearDown.
    
    Depends on parameters [test_reboot_mode] and [test_reboot_stage].
    Runs different method, tearDown_starting, tearDown_post_start and tearDown
    """
    def test_director(inst, *args):
        if inst.test_reboot_stage == 'starting':
            return inst.tearDown_starting()
        if inst.test_reboot_stage == 'post':
            return inst.tearDown_post_start()
        return f(inst)
    return test_director

