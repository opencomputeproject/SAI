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


from sai_thrift.sai_adapter import *
from constant import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]
from sai_utils import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]
from typing import TYPE_CHECKING
import sai_thrift.sai_adapter as adapter

if TYPE_CHECKING:
    from sai_test_base import T0TestBase


def t0_switch_config_helper(test_obj: 'T0TestBase'):
    """
    Make t0 switch configurations base on the configuration in the test plan.
    Set the configuration in test directly.

    Set the following test_obj attributes:
        int: switch_id

    """
    configer = SwitchConfiger(test_obj)
    test_obj.dut.switch_id = configer.start_switch()


class SwitchConfiger(object):
    """
    Class use to make all the Switch configurations.
    """

    def __init__(self, test_obj: 'T0TestBase') -> None:
        """
        Init the Switch configer.

        Args:
            test_obj: the test object
        """

        self.test_obj = test_obj
        self.client = test_obj.client

    def start_switch(self, switch_init_wait=3, route_mac=ROUTER_MAC):
        """
        Start switch and wait seconds for a warm up.

        Args:
            switch_init_wait: switch init wait time (sec)
            route_mac: route mac (switch mac)

        Returns:
            Vlan: vlan object
        """
        if self.test_obj.test_reboot_stage  == WARM_TEST_POST_REBOOT:
            switch_id = sai_thrift_create_switch(
                self.test_obj.client, init_switch=True, warm_recover=True)
        else:
            switch_id = sai_thrift_create_switch(
                self.test_obj.client, init_switch=True, src_mac_address=route_mac)
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

        print("Waiting for switch to get ready, {} seconds ...".format(
            switch_init_wait))
        time.sleep(switch_init_wait)
        self.get_default_switch_attr()
        return switch_id

    def get_default_switch_attr(self):
        """
        Get default switch attr.
        includes:
            system_port_no: number_of_system_ports
            active_ports_no: number_of_active_ports

        """

        print("Ignore all the expect error code and exception captures.")
        capture_status = adapter.CATCH_EXCEPTIONS
        expected_code = adapter.EXPECTED_ERROR_CODE

        adapter.CATCH_EXCEPTIONS = True
        adapter.EXPECTED_ERROR_CODE = []

        attr = sai_thrift_get_switch_attribute(
            self.client, number_of_system_ports=True)
        number_of_system_ports = 0
        if attr and 'number_of_system_ports' in attr:
            number_of_system_ports = attr['number_of_system_ports']
        self.test_obj.dut.system_port_no = number_of_system_ports
        print("Get number_of_system_ports {}".format(self.test_obj.dut.system_port_no))

        attr = sai_thrift_get_switch_attribute(
            self.client, number_of_active_ports=True)
        self.test_obj.dut.active_ports_no = attr['number_of_active_ports']
        print("Get number_of_active_ports {}".format(self.test_obj.dut.active_ports_no))
        print("Restore all the expect error code and exception captures.")
