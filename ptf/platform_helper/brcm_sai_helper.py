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

from platform_helper.common_sai_helper import *

class BrcmSaiHelper(CommonSaiHelper):
    """
    This class contains broadcom(brcm) specified functions for the platform setup and test context configuration.
    """
    platform = 'brcm'

    def remove_switch(self):
        '''
        Method to remove the switch.
        '''
        print("BrcmSaiHelperBase::remove_switch does not support. Cannot recreate after remove.")


    def recreate_ports(self):
        """
        Method to recreate the port.
        """

        #port recreate not support, error happened.
        #Port needs to be init and setup at same time.
        #Make the process happened in turn_up_and_check_ports
        print("BrcmSaiHelperBase::recreate_ports does not support.")


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


    def get_bridge_port_all_attribute(self, bridge_port_id):
        '''
        Gets all the attrbute from bridge port.
        '''
        print("BrcmSaiHelperBase::get_bridge_port_all_attribute")

        #Cannot get those three attributes from sai_thrift_get_bridge_port_attribute
        #ingress_filtering=True,
        #egress_filtering=True,
        #isolation_group=True
        sai_thrift_get_bridge_port_attribute(self.client,  bridge_port_oid=bridge_port_id, ingress_filtering=True,  egress_filtering=True)
        attr = sai_thrift_get_bridge_port_attribute(
            self.client, 
            bridge_port_oid=bridge_port_id,
            type=True,
            port_id=True,
            tagging_mode=True,
            vlan_id=True,
            rif_id=True,
            tunnel_id=True,
            bridge_id=True,
            fdb_learning_mode=True,
            max_learned_addresses=True,
            fdb_learning_limit_violation_packet_action=True,
            admin_state=True
            #Cannot get those three
            #ingress_filtering=True,
            #egress_filtering=True,
            #isolation_group=True
            )
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        return attr


    def turn_up_and_check_ports(self):
        '''
        Method to turn up the ports.

        In case some device not init the port after start the switch.

        Needs the following class attributes:

            self.port_list - list of all active port objects
        '''

        #For brcm devices, need to init and setup the ports at once after start the switch.
        #Skip the function :func:`BrcmSaiHelper.recreate_ports`
        retries = 10
        for port_id in self.port_list:
            try:
                sai_thrift_set_port_attribute(
                    self.client, port_oid=port_id, admin_state=True)
            except BaseException as e:
                print("Cannot setup port admin state, error {}".format(e))

        for num_of_tries in range(retries):
            all_ports_are_up = True
            time.sleep(2)
            for port_id in self.port_list:
                port_attr = sai_thrift_get_port_attribute(
                    self.client, port_id, oper_status=True)
                if port_attr['oper_status'] != SAI_PORT_OPER_STATUS_UP:
                    all_ports_are_up = False
                    time.sleep(1)
                    print("port is down: {}".format(port_attr['oper_status']))
            if all_ports_are_up:
                break
        if not all_ports_are_up:
            print("Not all the ports are up after {} rounds of retries.".format(retries))


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


    def load_default_1q_bridge_ports(self):
        """
        Loads default 1q bridge ports and set as class attribute.

        Needs the following class attributes:
            self.default_1q_bridge - default_1q_bridge oid

            self.active_ports_no - number of active ports

            self.portX objects for all active ports

        Sets the following class attributes:

            self.default_1q_bridge_port_list - list of all 1q bridge port objects

            self.portX_bp - objects for all 1q bridge ports
        """
        print("BrcmSaiHelperBase::load_default_1q_bridge_ports")
        attr = sai_thrift_get_bridge_attribute(
                    self.client, 
                    bridge_oid=self.default_1q_bridge,
                    port_list=sai_thrift_object_list_t(
                        idlist=[], count=self.active_ports_no))
        default_1q_bridge_port_list = attr['port_list'].idlist
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        #try to binding the bridge port with the port index here
        for bp in default_1q_bridge_port_list:
            attr = self.get_bridge_port_all_attribute(bp)
            for index in range(0, len(self.port_list)):
                port_id = getattr(self, 'port%s' % index)
                if port_id == attr['port_id']:
                    setattr(self, 'port%s_bp' % index, bp)
                    break
        return default_1q_bridge_port_list


    def remove_1q_bridge_port(self, default_1q_bridge_port_list):
        '''
        Removes all the bridge ports.
        '''

        for index in range(0, len(default_1q_bridge_port_list)):
            port_bp = getattr(self, 'port%s_bp' % index)
            sai_thrift_remove_bridge_port(self.client, port_bp)
            delattr(self, 'port%s_bp' % index)


    def reset_1q_bridge_ports(self):
        '''
        Reset all the 1Q bridge ports.
        Needs the following class attributes:
            self.default_1q_bridge - default_1q_bridge oid

            self.active_ports_no - number of active ports

            self.portX objects for all active ports
        '''
        #In case the bridge port will be initalized by default, clear them
        default_1q_bridge_port_list = self.load_default_1q_bridge_ports()
        self.remove_1q_bridge_port(default_1q_bridge_port_list)
