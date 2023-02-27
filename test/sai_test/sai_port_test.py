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

from unittest import skip
from sai_test_base import T0TestBase
from sai_utils import *

CATCH_EXCEPTIONS=True
# SAI_STATUS_NOT_IMPLEMENTED
ACCEPTED_ERROR_CODE = [SAI_STATUS_NOT_IMPLEMENTED]
#SAI_STATUS_ATTR_NOT_IMPLEMENTED
ACCEPTED_ERROR_CODE += range(SAI_STATUS_ATTR_NOT_IMPLEMENTED_MAX, SAI_STATUS_ATTR_NOT_IMPLEMENTED_0)
#SAI_STATUS_ATTR_NOT_IMPLEMENTED
ACCEPTED_ERROR_CODE += range(SAI_STATUS_UNKNOWN_ATTRIBUTE_MAX, SAI_STATUS_UNKNOWN_ATTRIBUTE_0)
#SAI_STATUS_ATTR_NOT_SUPPORTED
ACCEPTED_ERROR_CODE += range(SAI_STATUS_ATTR_NOT_SUPPORTED_MAX, SAI_STATUS_ATTR_NOT_SUPPORTED_0)

def set_accepted_exception():
    """
    Set accepted exceptions.
    """
    adapter.CATCH_EXCEPTIONS=CATCH_EXCEPTIONS
    adapter.EXPECTED_ERROR_CODE += ACCEPTED_ERROR_CODE

class PortAutoNegTest(T0TestBase):
    """
    Related to PR 1848 in ado repo
    This test case will test against the auto enable and disable
    Test step:
    1. get the default AN state
    2. change to another value of the AN, if default is false, set it to true
        if it is true, set it to false
    3. restore the AN to default state
    4. restart Port (set the admin state from down to up)
    Expect:
        Port should be up
    """

    def setUp(self):
        """
        Set up test
        """
        set_accepted_exception()
        T0TestBase.setUp(self, 
            is_create_default_loopback_interface=False,
            is_create_fdb=False,
            is_create_lag=False,
            is_create_route_for_nhopgrp=False,
            is_create_tunnel=False,
            is_create_route_for_lag=False,
            is_create_default_route=False,
            is_create_vlan=False,
            is_create_hostIf=False,
            is_create_route_for_vlan_itf=False,
            is_create_vlan_itf=False)


    def runTest(self):
        # set port an state
        attr = sai_thrift_get_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, auto_neg_mode=True)
        currernt_statue = attr['auto_neg_mode']

        new_attr = not currernt_statue
        print("Set auto_neg_mode to {}".format(new_attr))
        sai_thrift_set_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, auto_neg_mode=new_attr)
        attr = sai_thrift_get_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, auto_neg_mode=True)
        self.assertEqual(attr['auto_neg_mode'], new_attr)

        print("Set auto_neg_mode to {}".format(currernt_statue))
        sai_thrift_set_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, auto_neg_mode=currernt_statue)
        attr = sai_thrift_get_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, auto_neg_mode=True)
        self.assertEqual(attr['auto_neg_mode'], currernt_statue)

        sai_thrift_set_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, admin_state=False)
        sai_thrift_set_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, admin_state=True)

        print("Wait for 5 sec for port get ready.")
        attr = sai_thrift_get_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, admin_state=True)
        self.assertEqual(attr['admin_state'], True)


    def tearDown(self):
        """
        Test the basic tearDown process
        """
        super().tearDown()


class PortAutoNegLearnTrainingTest(T0TestBase):
    """
    Related to PR 1848 in ado repo
    This test case will test against result for enanle the AN/LT
    For 100G devices we need to enable those following


        // Override advertising FEC mode
        AUTONEG_FEC_OVERRIDE = 1;

        rv = bcm_port_ability_advert_get(0, port, &port_ability);
        BRCM_SAI_API_CHK(SAI_API_PORT, "port ability advert get", rv);

        // Force speed, medium, channel, disable FEC, disable pause advertisements
        advert_ability.speed_full_duplex = 100GB;
        advert_ability.medium = MEDIUM_COPPER;
        advert_ability.channel = CHANNEL_LONG;
        advert_ability.fec = FEC_NONE;
        advert_ability.pause = 0;

        AUTO_NEG_MODE = True


    Test step:
    1. Enable the attribute as above
    2. check the result
    Expect:
        All the attribute should be set and can get the attribute correctly
    """

    def setUp(self):
        """
        Set up test
        """
        set_accepted_exception()
        T0TestBase.setUp(self, 
            is_create_default_loopback_interface=False,
            is_create_fdb=False,
            is_create_lag=False,
            is_create_route_for_nhopgrp=False,
            is_create_tunnel=False,
            is_create_route_for_lag=False,
            is_create_default_route=False,
            is_create_vlan=False,
            is_create_hostIf=False,
            is_create_route_for_vlan_itf=False,
            is_create_vlan_itf=False)


    def runTest(self):
        # set port an state
        sai_thrift_set_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, auto_neg_fec_mode_override=True)
        attr = sai_thrift_get_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, auto_neg_fec_mode_override=True)
        self.assertEqual(attr['auto_neg_fec_mode_override'], True)

        
        advertised_speed_list = sai_thrift_s32_list_t(count=1, int32list=[100000])
        sai_thrift_set_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, advertised_speed=advertised_speed_list)
        advertised_speed_list = sai_thrift_s32_list_t(count=5, int32list=[])
        attr = sai_thrift_get_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, advertised_speed=advertised_speed_list)
        self.assertEqual(len(attr['advertised_speed'].uint32list), 1)
        self.assertEqual(attr['advertised_speed'].uint32list[0], 100000)
        
        sai_thrift_set_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, advertised_media_type=SAI_PORT_MEDIA_TYPE_COPPER)
        attr = sai_thrift_get_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, advertised_media_type=SAI_PORT_MEDIA_TYPE_COPPER)
        self.assertEqual(attr['advertised_media_type'], SAI_PORT_MEDIA_TYPE_COPPER)


        sai_thrift_set_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, advertised_asymmetric_pause_mode=True)
        attr = sai_thrift_get_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, advertised_asymmetric_pause_mode=True)
        self.assertEqual(attr['advertised_asymmetric_pause_mode'], True)

        sai_thrift_set_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, auto_neg_mode=True)
        attr = sai_thrift_get_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, auto_neg_mode=True)
        self.assertEqual(attr['auto_neg_mode'], True)
        attr = sai_thrift_get_port_attribute(self.client, port_oid=self.dut.active_port_obj_list[0].oid, auto_neg_config_mode=True)
        self.assertEqual(attr['auto_neg_config_mode'], SAI_PORT_AUTO_NEG_CONFIG_MODE_AUTO)
        

    def tearDown(self):
        """
        Test the basic tearDown process
        """
        super().tearDown()
