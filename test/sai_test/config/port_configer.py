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

from collections import OrderedDict
from ptf import config
from sai_utils import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]
from sai_thrift.sai_adapter import *
from typing import TYPE_CHECKING
from data_module.port import Port

from typing import Dict, List
if TYPE_CHECKING:
    from sai_test_base import T0TestBase


def t0_port_config_helper(test_obj: 'T0TestBase', is_recreate_bridge=True, is_create_hostIf=True):
    """
    Make t0 Port configurations base on the configuration in the test plan.
    Set the configuration in test directly.

    Set the following test_obj attributes:
        int: default_trap_group
        list: port_list
        int: default_1q_bridge_id
        int: host_intf_table_id
        list: hostif_list

    """
    configer = PortConfiger(test_obj)
    test_obj.dut.port_id_list = configer.get_lane_sorted_port_list()
    configer.generate_port_obj_list_by_interface_config()
    configer.assign_port_config(test_obj.port_conifg_ini_loader.portConfigs)

    attr = sai_thrift_get_switch_attribute(
        configer.client, default_trap_group=True)
    default_trap_group = attr['default_trap_group']
    configer.turn_on_port_admin_state(test_obj.dut.port_obj_list)
    configer.turn_up_and_check_ports(test_obj.dut.port_obj_list)

    if is_create_hostIf:
        if 'port_config_ini' in test_obj.test_params:
            host_intf_table_id, hostif_list = configer.create_port_hostif_by_port_config_ini(
                port_list=test_obj.dut.port_obj_list, trap_group=default_trap_group)
        else:
            host_intf_table_id, hostif_list = configer.create_host_intf(
                port_list=test_obj.dut.port_obj_list, trap_group=default_trap_group)
        test_obj.dut.host_intf_table_id = host_intf_table_id
        test_obj.dut.hostif_list = hostif_list

    default_1q_bridge_id = configer.get_default_1q_bridge()
    configer.load_default_active_1q_bridge_ports(default_1q_bridge_id)

    if is_recreate_bridge:
        configer.remove_bridge_port(default_1q_bridge_id)
        configer.create_bridge_ports(
            default_1q_bridge_id, test_obj.dut.port_obj_list)

    configer.get_cpu_port_queue()
    test_obj.dut.default_trap_group = default_trap_group
    test_obj.dut.default_1q_bridge_id = default_1q_bridge_id


def t0_port_tear_down_helper(test_obj: 'T0TestBase'):
    '''
    Args:
        test_obj: test object
    '''
    configer = PortConfiger(test_obj)
    default_1q_bridge_id = configer.get_default_1q_bridge()
    configer.remove_bridge_port(default_1q_bridge_id)
    configer.remove_host_inf(
        test_obj.dut.host_intf_table_id, test_obj.dut.hostif_list)


class PortConfiger(object):
    """
    Class use to make all the basic configurations.
    """

    def __init__(self, test_obj: 'T0TestBase') -> None:
        """
        Init the Port configer.

        Args:
            test_obj: the test object
        """
        self.test_obj = test_obj
        self.client = test_obj.client        
        self.config_db = test_obj.config_db_loader.get_port_config()

    def create_bridge_ports(self, bridge_id, port_list: List['Port']):
        """
        Create bridge ports base on port_list.

        Args:
            bridge_id: bridge object id
            port_list: port list oid

        Returns:
            list: bridge port list
        """
        print("Create bridge ports...")
        bp_list = []
        for index, item in enumerate(port_list):
            port_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=bridge_id,
                port_id=item.oid,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            bp_list.append(port_bp)
            item.bridge_port_oid = port_bp
        #print("create bridge port list : {}".format(bp_list))
        return bp_list

    def create_host_intf(self, port_list: List['Port'], trap_group=None):
        """
        Create host interface.

        Steps:
         1. create host table entry
         2. create host interface trap
         3. set host interface base on the port_config.int (this file contains the lanes, name and index information.)

        Args:
            ports_config: port configs, which is got from the local config.
            trap_group: host interface trap group(optional)
            port_list: port list 

        Returns:
            host_intf_table_id
            hostif_list
        """
        print("Create Host intfs...")
        host_intf_table_id = sai_thrift_create_hostif_table_entry(
            self.client, type=SAI_HOSTIF_TABLE_ENTRY_TYPE_WILDCARD,
            channel_type=SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_PHYSICAL_PORT)
        sai_thrift_create_hostif_trap(
            self.client, trap_type=SAI_HOSTIF_TRAP_TYPE_TTL_ERROR, packet_action=SAI_PACKET_ACTION_TRAP,
            trap_group=trap_group, trap_priority=0)
        hostif_list = [None]*len(port_list)
        for index, item in enumerate(port_list):
            try:
                hostif = sai_thrift_create_hostif(
                    self.client,
                    type=SAI_HOSTIF_TYPE_NETDEV,
                    obj_id=item.oid,
                    name=item.port_config.name)
                sai_thrift_set_hostif_attribute(
                    self.client, hostif_oid=hostif, oper_status=False)
                hostif_list[index] = hostif
                item.host_itf_idx = hostif
            except BaseException as e:
                print("Cannot create hostif, error : {}".format(e))
        return host_intf_table_id, hostif_list

    def create_port_hostif_by_port_config_ini(self, port_list: List['Port'], trap_group=None):
        lane_orderd_dict = sorted(
            self.test_obj.ports_config.items(), key=lambda kv: kv[1]['lanes'])
        index_orderd_dict = sorted(
            self.test_obj.ports_config.items(), key=lambda kv: kv[1]['index'])

        min = int(index_orderd_dict[0][1]['index'])
        for item in lane_orderd_dict:
            self.test_obj.dut.host_if_port_idx_map.append(
                int(item[1]['index'])-min)
        for index, key in enumerate(self.test_obj.ports_config):
            self.test_obj.dut.host_if_name_list.append(key)
        print("Create Host intfs...")
        host_intf_table_id = sai_thrift_create_hostif_table_entry(
            self.client, type=SAI_HOSTIF_TABLE_ENTRY_TYPE_WILDCARD,
            channel_type=SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_PHYSICAL_PORT)
        sai_thrift_create_hostif_trap(
            self.client, trap_type=SAI_HOSTIF_TRAP_TYPE_TTL_ERROR, packet_action=SAI_PACKET_ACTION_TRAP,
            trap_group=trap_group, trap_priority=0)
        hostif_list = [None]*len(port_list)
        min = int(index_orderd_dict[0][1]['index'])
        for index, item in enumerate(lane_orderd_dict):
            port_index = self.test_obj.dut.host_if_port_idx_map[index]
            port = self.test_obj.dut.port_obj_list[port_index]
            name = item[0]
            try:
                hostif = sai_thrift_create_hostif(
                    self.client,
                    type=SAI_HOSTIF_TYPE_NETDEV,
                    obj_id=port.oid,
                    name=name)
                sai_thrift_set_hostif_attribute(
                    self.client, hostif_oid=hostif, oper_status=False)
                hostif_list[index] = hostif
                port.host_itf_idx = index
            except BaseException as e:
                print("Cannot create hostif, error : {}".format(e))
        return host_intf_table_id, hostif_list

    def get_bridge_port_all_attribute(self, bridge_port_id):
        '''
        Gets all the attrbute from bridge port.

        Args:
            bridge_port_id: bridge port object id

        Returns:
            dict: bridge attributes

        '''

        # Cannot get those three attributes from sai_thrift_get_bridge_port_attribute
        # ingress_filtering=True,
        # egress_filtering=True,
        # isolation_group=True
        sai_thrift_get_bridge_port_attribute(
            self.client,  
            bridge_port_oid=bridge_port_id, 
            # ingress_filtering=True,  # 11
            # egress_filtering=True    # 12
        )
        # Todo check the attribute before use
        attr = sai_thrift_get_bridge_port_attribute(
            self.client,
            bridge_port_oid=bridge_port_id,
            type=True,
            port_id=True,
            tagging_mode=True,  # 2
            vlan_id=True,  # 3
            rif_id=True,  # 4
            tunnel_id=True,  # 5
            bridge_id=True,
            fdb_learning_mode=True,
            max_learned_addresses=True,  # 8
            fdb_learning_limit_violation_packet_action=True,  # 9
            admin_state=True
            # Cannot get those three
            # ingress_filtering=True,
            # egress_filtering=True,
            # isolation_group=True
        )
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
        return attr

    def load_default_active_1q_bridge_ports(self, bridge_id):
        """
        Loads default 1q bridge ports and set as class attribute.
        Args:
            bridge_id: bridge id.

        Returns:
            list: bridge_port_list
        """

        print("Get bridge ports...")
        attr = sai_thrift_get_bridge_attribute(
                    self.client, 
                    bridge_oid=bridge_id,
                    port_list=sai_thrift_object_list_t(idlist=[], count=100))
        default_1q_bridge_port_list = attr['port_list'].idlist
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

        #try to binding the bridge port with the port index here
        port_list = self.test_obj.dut.port_obj_list
        active_1q_bridge_ports = []
        for index in range(0, len(port_list)):
            for bp in default_1q_bridge_port_list:
                attr = self.get_bridge_port_all_attribute(bp)            
                port_id = port_list[index].oid
                if port_id == attr['port_id']:
                    port_list[index].bridge_port_oid = bp
                    active_1q_bridge_ports.append(bp)
                    break
        return active_1q_bridge_ports


    def get_default_1q_bridge(self):
        """
        Get defaule 1Q bridge.

        Returns:
            default_1q_bridge_id
        """
        print("Get default 1Q bridge...")
        def_attr = sai_thrift_get_switch_attribute(
            self.client, default_1q_bridge_id=True)
        return def_attr['default_1q_bridge_id']


    def generate_port_obj_list_by_interface_config(self):
        """
        Get device port numbers from ptf config.
        Those port number should map to the remote DUT port base on the configuration file.

        Following the sequence of the parameters: --interface '0-16@eth0' --interface '0-18@eth1'
        item[0]=16, item[1]=18

        Returns:
            list: port numbers
        """
        dev_port_list = []
        for index, item in enumerate(config['interfaces']):
            device, port, eth = item
            # device, the device index, will be used in multi devices
            # Port, local port index
            # eth, eth name
            self.test_obj.dut.port_obj_list[index].dev_port_index = port
            self.test_obj.dut.port_obj_list[index].dev_port_eth = eth
            dev_port_list.append(port)
        return dev_port_list

    def get_lane_sorted_port_list(self):
        """
        Get the port list sorted by lanes name(defined in port_config.ini).
        This method will shrime the ports base on the lanes.
        i.e. If device has 64 ports but just define 32 in lanes, then just generate 32 ports

        Returns:
            port_list
        """
        port_obj_list: List[Port] = []
        port_id_list = []
        port_list = sai_thrift_object_list_t(count=100)
        p_list = sai_thrift_get_switch_attribute(
            self.client, port_list=port_list)
        for index, item in enumerate(p_list['port_list'].idlist):
            port: Port = Port(oid=item, port_index=index, rif_list=[
            ], nexthopv4_list=[], nexthopv6_list=[])
            temp_list = sai_thrift_object_list_t(count=100)
            attr = sai_thrift_get_port_attribute(
                self.client, port_oid=port.oid, hw_lane_list=temp_list)
            port.default_lane_list = attr['hw_lane_list'].uint32list
            port_obj_list.append(port)
        self.test_obj.dut.port_obj_list = self.sort_port_list_by_config(
            self.test_obj.ports_config, port_obj_list)
        for item in self.test_obj.dut.port_obj_list:
            port_id_list.append(item.oid)
        print("Base on lanes config file[{}], init {} ports from {} device posts"
        .format("port_config.ini",
                len(self.test_obj.dut.port_obj_list),
                len(port_obj_list)
             ))
        #print("port_list {}".format(port_id_list))

        return port_id_list

    def sort_port_list_by_config(self, ports_config, port_list: List[Port]):
        """
        Sort the port list base on the port_config.ini.
        This method will match the default_lane_list in the port object with the lane defined in 
        port config for a ordered port list.

        Attrs:
            ports_config: port config, which gets from the port_config.ini
            port_list: port list
        """
        sorted_port_list: List[Port] = []
        for index, key in enumerate(ports_config):
            lane_list = ports_config[key]['lanes']
            for port in port_list:
                if lane_list == port.default_lane_list:
                    sorted_port_list.append(port)
                    break
        return sorted_port_list

    def assign_port_config(self, portConfigs: Dict):
        """
        Assign the PortConfig object to the Port Object
        """
        for index, portconfig in enumerate(portConfigs):
            self.test_obj.dut.port_obj_list[index].port_config = portconfig


    def remove_bridge_port(self, bridge_id):
        """
        Remove bridge ports (bridge will not be removed).

        Args:
            bridge_id: bridge id.

        """
        print("Remove bridge ports...")
        bp_ports = self.load_default_active_1q_bridge_ports(bridge_id)
        for index, port in enumerate(bp_ports):
            sai_thrift_remove_bridge_port(self.client, port)
            print("Removed bridge port {}".format(port))
            if self.test_obj.dut.port_obj_list[index].bridge_port_oid != port:
                print("WARN! BUG! Bridge not as expected, not equals to port record.")
            self.test_obj.dut.port_obj_list[index].bridge_port_oid = None
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

    def remove_host_inf(self, host_intf_table_id, hostif_list):
        """
        Remove host interface.
         Steps:
         2. remove host interface
         1. remove host table entry
        Args:
            host_intf_table_id
            hostif_list 
        """

        for _, hostif in enumerate(hostif_list):
            sai_thrift_remove_hostif(self.client, hostif)
        sai_thrift_remove_hostif_table_entry(self.client, host_intf_table_id)

    def turn_on_port_admin_state(self, port_list: List['Port']):
        """
        Turn on port admin state

        Args:
            post_list: post list
        """
        print("Set port...")
        for _, port in enumerate(port_list):
            sai_thrift_set_port_attribute(
                self.client, port_oid=port.oid, mtu=self.get_mtu(), admin_state=True,
                fec_mode=self.get_fec_mode())

    def turn_up_and_check_ports(self, port_list: List['Port']):
        '''
        Method to turn up the ports.
        In case some device not init the port after start the switch.

        Args:
            port_list - list of all active port objects
        '''

        # For brcm devices, need to init and setup the ports at once after start the switch.
        retries = 10
        down_port_list = []
        for index, port in enumerate(port_list):
            port_attr = sai_thrift_get_port_attribute(
                self.client, port.oid, oper_status=True)
            print("Turn up port {}".format(index))
            port_up = True
            if port_attr['oper_status'] != SAI_PORT_OPER_STATUS_UP:
                port_up = False
                for num_of_tries in range(retries):
                    port_attr = sai_thrift_get_port_attribute(
                        self.client, port.oid, oper_status=True)
                    if port_attr['oper_status'] == SAI_PORT_OPER_STATUS_UP:
                        port_up = True
                        break
                    time.sleep(3)
                    print("port {} , local index {} id {} is not up, status: {}. Retry. Reset Admin State.".format(
                        index, port.port_index, port.oid, port_attr['oper_status']))
                    sai_thrift_set_port_attribute(
                        self.client,
                        port_oid=port.oid,
                        mtu=self.get_mtu(),
                        admin_state=True,
                        fec_mode=self.get_fec_mode())
            if not port_up:
                down_port_list.append(index)
        if down_port_list:
            print("Ports {} are  down after retries.".format(down_port_list))

    def get_fec_mode(self):
        '''
        get fec mode from config_db.json

        RETURN:
             int: SAI_PORT_FEC_MODE_X
        '''
        fec_mode = self.config_db.get('fec')
        fec_change = {
            None: SAI_PORT_FEC_MODE_NONE,
            'rs': SAI_PORT_FEC_MODE_RS,
            'fc': SAI_PORT_FEC_MODE_FC,
        }
        return fec_change[fec_mode]

    def get_mtu(self):
        '''
        get mtu from config_db.json

        RETURN:
            int: mtu number
        '''
        return int(self.config_db.get('mtu'))

    def get_cpu_port_queue(self):

        attr = sai_thrift_get_switch_attribute(self.client, cpu_port=True)
        self.test_obj.dut.cpu_port = attr['cpu_port']

        attr = sai_thrift_get_port_attribute(self.client,
                                             self.test_obj.dut.cpu_port,
                                             qos_number_of_queues=True)
        num_queues = attr['qos_number_of_queues']
        q_list = sai_thrift_object_list_t(count=num_queues)
        attr = sai_thrift_get_port_attribute(self.client,
                                             self.test_obj.dut.cpu_port,
                                             qos_queue_list=q_list)
        for queue in range(0, num_queues):
            queue_id = attr['qos_queue_list'].idlist[queue]
            setattr(self.test_obj, 'cpu_queue%s' % queue, queue_id)
            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id,
                port=True,
                index=True,
                parent_scheduler_node=True)
            self.test_obj.assertEqual(queue, q_attr['index'])
