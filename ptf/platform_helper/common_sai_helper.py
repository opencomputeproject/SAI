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
Class contains common functions.

This file contains base class for other platform classes.
"""
from  sai_base_test import *

class CommonSaiHelper(SaiHelper):
    """
    This class contains the common functions for the platform setup and test context configuration.
    """
    #TODO move the common methods from the sai_base_test.

    platform = 'common'

    def sai_thrift_create_fdb_entry_allow_mac_move(self,
                                client,
                                fdb_entry,
                                type=None,
                                packet_action=None,
                                user_trap_id=None,
                                bridge_port_id=None,
                                meta_data=None,
                                endpoint_ip=None,
                                counter_id=None,
                                allow_mac_move=None):
        """
        Override the sai_thrift_create_fdb_entry when check the functionality related to allow_mac_move.

        This method will transfer allow_mac_move directly(not override).
        For the encounter function, please refer to \r
        \t :func:`BrcmSaiHelper.sai_thrift_create_fdb_entry_allow_mac_move`
        """
        #TODO confirm the SPEC. Related to RFC9014 and RFC7432
        print("CommonSaiHelper::sai_thrift_create_fdb_entry_allow_mac_move")
        sai_thrift_create_fdb_entry(
            client=client,
            fdb_entry=fdb_entry,
            type=type,
            packet_action=packet_action,
            user_trap_id=user_trap_id,
            bridge_port_id=bridge_port_id,
            meta_data=meta_data,
            endpoint_ip=endpoint_ip,
            counter_id=counter_id,
            allow_mac_move=allow_mac_move)


    def remove_bridge_port(self):
        """
        Remove all bridge ports.
        """
        for index in range(0, len(self.port_list)):
            port_bp = getattr(self, 'port%s_bp' % index)
            sai_thrift_remove_bridge_port(self.client, port_bp)


    def create_bridge_ports(self):
        """
        Create bridge ports base on port_list.
        """
        for index in range(0, len(self.port_list)):
            port_id = getattr(self, 'port%s' % index)
            port_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=port_id,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            setattr(self, 'port%s_bp' % index, port_bp)
            self.assertNotEqual(getattr(self, 'port%s_bp' % index), 0)
