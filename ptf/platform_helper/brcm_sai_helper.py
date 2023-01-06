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
This file contains class for brcm specified functions.
"""
from collections import OrderedDict

from typing import List, Dict
from typing import TYPE_CHECKING

from platform_helper.common_sai_helper import * # pylint: disable=wildcard-import; lgtm[py/polluting-import]
from config.port_configer import PortConfiger
from config.config_db_loader import ConfigDBLoader
from config.port_config_ini_loader import PortConfigInILoader


CATCH_EXCEPTIONS=True
# SAI_STATUS_NOT_IMPLEMENTED
ACCEPTED_ERROR_CODE = [SAI_STATUS_NOT_IMPLEMENTED]
#SAI_STATUS_ATTR_NOT_IMPLEMENTED
ACCEPTED_ERROR_CODE += range(SAI_STATUS_ATTR_NOT_IMPLEMENTED_MAX, SAI_STATUS_ATTR_NOT_IMPLEMENTED_0)
#SAI_STATUS_ATTR_NOT_IMPLEMENTED
ACCEPTED_ERROR_CODE += range(SAI_STATUS_UNKNOWN_ATTRIBUTE_MAX, SAI_STATUS_UNKNOWN_ATTRIBUTE_0)
#SAI_STATUS_ATTR_NOT_SUPPORTED
ACCEPTED_ERROR_CODE += range(SAI_STATUS_ATTR_NOT_SUPPORTED_MAX, SAI_STATUS_ATTR_NOT_SUPPORTED_0)


class BrcmSaiHelper(CommonSaiHelper):
    """
    This class contains broadcom(brcm) specified functions for the platform setup and test context configuration.
    """
    platform = 'brcm'

    def set_accepted_exception(self):
        """
        Set accepted exceptions.
        """
        adapter.CATCH_EXCEPTIONS=CATCH_EXCEPTIONS
        adapter.EXPECTED_ERROR_CODE += ACCEPTED_ERROR_CODE


    def __init__(self, *args, **kwargs) -> None:
        """
        Init the Port configer.

        Args:
            test_obj: the test object
        """
        super().__init__(*args, **kwargs)
        self.port_configer = None

        # port
        self.default_trap_group = None
        """
        Local device port index list, 0, 1, ...
        """
        self.host_intf_table_id = None
        self.port_obj_list: List['Port'] = []
        """
        Port object list
        """
        self.port_id_list = []
        """
        port id list, use to present all the port ids
        """
        self.hostif_list = None
        """
        Host interface list
        """
        self.default_bridge_port_list = []
        """
        Default bridge port list
        """
        self.host_if_port_idx_map = []
        """
        list in order of the host interface create sequence, and with the value for port index
        """
        self.host_if_name_list = []
        """
        List of the interface name
        """


    def remove_switch(self):
        '''
        Method to remove the switch.
        '''
        print("BrcmSaiHelperBase::remove_switch does not support. Cannot recreate after remove.")   


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

        This method will set the allow_mac_move to True.
        """
        #TODO confirm the SPEC. Related to RFC9014 and RFC7432
        #Context: when set SAI_FDB_ENTRY_TYPE_STATIC, allow_mac_move will be checked, and its default value is false
        #         then, when a port get different mac (i.e. arp ack from arp req, port used in previous arp req, also means other ports with different session).
        #         the packet should be dropped.
        #         but some other ASIC can transfer the packet to other ports not used to transfer the packet.

        print("BrcmSaiHelper::sai_thrift_create_fdb_entry_allow_mac_move")
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
            allow_mac_move=True)


    def start_switch(self):
        """
        Start switch and wait seconds for a warm up.
        """
        switch_init_wait = 5

        self.switch_id = sai_thrift_create_switch(
            self.client, init_switch=True, src_mac_address=ROUTER_MAC)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        print("Waiting for switch to get ready, {} seconds ...".format(switch_init_wait))
        time.sleep(switch_init_wait)


    def check_cpu_port_hdl(self):
        """
        Checks cpu port handler, expect the cpu_port_hdl equals to qos_queue port id

        Needs the following class attributes:

            self.cpu_port_hdl - cpu_port_hdl id

        Sets the following class attributes:

            self.cpu_port - cpu_port id

        """
        attr = sai_thrift_get_port_attribute(self.client,
                                             self.cpu_port_hdl,
                                             qos_number_of_queues=True)
        num_queues = attr['qos_number_of_queues']
        q_list = sai_thrift_object_list_t(count=num_queues)
        attr = sai_thrift_get_port_attribute(self.client,
                                             self.cpu_port_hdl,
                                             qos_queue_list=q_list)

        for queue in range(0, num_queues):
            queue_id = attr['qos_queue_list'].idlist[queue]
            setattr(self, 'cpu_queue%s' % queue, queue_id)
            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id,
                port=True,
                index=True,
                parent_scheduler_node=True)
            self.assertEqual(queue, q_attr['index'])
            # in broadcom platform, the q_attr["port"] is not equals to cpu_port_hdl
            # cpu_port_hdl is ahead of the cpu_port list
            self.cpu_port = q_attr["port"]



    # Port setup method below

    def config_port(self):

        self.port_list = self.port_configer.get_port_list()
        self.port_configer.get_local_mapped_ports()
        self.port_configer.assign_port_config(self.port_conifg_ini_loader.portConfigs)
        #compatiable with portx variables
        self.port_configer.set_port_attr(self.port_list)

        attr = sai_thrift_get_switch_attribute(
            self.client, default_trap_group=True)
        default_trap_group = attr['default_trap_group']
        self.port_configer.turn_on_port_admin_state_by_port_list(self.port_obj_list)
        self.port_configer.turn_up_and_check_ports_by_port_list(self.port_obj_list)


        if 'port_config_ini' in self.test_params:
            host_intf_table_id, hostif_list = self.port_configer.create_port_hostif_by_port_config_ini(
                port_list=self.port_obj_list, trap_group=default_trap_group)
        else:
            host_intf_table_id, hostif_list = self.port_configer.create_host_intf(
                port_list=self.port_obj_list, trap_group=default_trap_group)
        self.host_intf_table_id = host_intf_table_id
        self.hostif_list = hostif_list

        self.default_1q_bridge = self.port_configer.get_default_1q_bridge()
        self.port_configer.reset_1q_bridge_ports()


   #Override methods

    def recreate_ports(self):
        """
        Method to recreate the port.
        """

        #port recreate not support, error happened.
        #Port needs to be init and setup at same time.
        #Make the process happened in turn_up_and_check_ports
        print("BrcmSaiHelperBase::recreate_ports does not support. Just Parse Port Config")
        self.ports_config = self.port_conifg_ini_loader.ports_config
