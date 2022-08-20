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

from typing import Dict, List
if TYPE_CHECKING:
    from sai_test_base import T0TestBase
    from data_module.port import Port


def t0_port_config_helper(test_obj: 'T0TestBase', is_recreate_bridge=True, is_create_hostIf=True):
    """
    Make t0 Port configurations base on the configuration in the test plan.
    Set the configuration in test directly.

    Set the following test_obj attributes:
        list: dev_port_list
        dict: portConfigs - index:PortConfig
        int: default_trap_group
        list: bridge_port_list
        list: port_list
        dict: port_to_hostif_map - port_oid:hostIf_oid
        int: default_1q_bridge_id        
        int: host_intf_table_id
        list: hostif_list

    """
    configer = PortConfiger(test_obj)
    port_list = configer.get_port_list()
    dev_port_list = configer.get_local_mapped_ports()
    portConfigs = configer.parse_port_config(
        test_obj.test_params['port_config_ini'])

    attr = sai_thrift_get_switch_attribute(
        configer.client, default_trap_group=True)
    default_trap_group = attr['default_trap_group']
    configer.turn_on_port_admin_state(test_obj.dut.port_obj_list)
    configer.turn_up_and_check_ports(test_obj.dut.port_obj_list)
    default_1q_bridge_id = configer.get_default_1q_bridge()
    bridge_port_list = configer.get_bridge_port_list(default_1q_bridge_id)

    if is_recreate_bridge:
        configer.remove_bridge_port(default_1q_bridge_id)
        bridge_port_list = configer.create_bridge_ports(
            default_1q_bridge_id, test_obj.dut.port_obj_list)

    if is_create_hostIf:
        host_intf_table_id, hostif_list = configer.create_host_intf(
            ports_config=portConfigs, trap_group=default_trap_group, port_list=port_list)
        # Todo try to get the host interface if not create the hostif (need to check if already created or not)
        port_to_hostif_map = configer.generate_port_to_hostif_map(
            port_list, hostif_list)
        test_obj.dut.host_intf_table_id = host_intf_table_id
        test_obj.dut.hostif_list = hostif_list

    configer.get_cpu_port_queue()
    test_obj.dut.dev_port_list = dev_port_list
    test_obj.dut.portConfigs = portConfigs
    test_obj.dut.default_trap_group = default_trap_group
    test_obj.dut.port_list = port_list
    test_obj.dut.port_to_hostif_map = port_to_hostif_map
    test_obj.dut.default_1q_bridge_id = default_1q_bridge_id
    test_obj.dut.bridge_port_list = bridge_port_list


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
        config_driver = ConfigDBOpertion()
        self.config = config_driver.get_port_config()

    def create_bridge_ports(self, bridge_id, port_list: List[Port]):
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
                port_id=item,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            bp_list.append(port_bp)
            item.bridge_port_oid = port_bp
        return bp_list

    def create_host_intf(self, port_list:List[Port], trap_group=None):
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

        hostif_list = []
        for index, item in enumerate(port_list):
            try:
                hostif = sai_thrift_create_hostif(
                    self.client,
                    type=SAI_HOSTIF_TYPE_NETDEV,
                    obj_id=item.oid,
                    name=item.port_config.name)
                sai_thrift_set_hostif_attribute(
                    self.client, hostif_oid=hostif, oper_status=False)
                hostif_list.append(hostif)
                item.host_itf = hostif
            except BaseException as e:
                print("Cannot create hostif, error : {}".format(e))
        return host_intf_table_id, hostif_list

    def generate_port_to_hostif_map(self, port_list, hostif_list):
        """
        Generate the port to hostif map, base on the port list sequence

        Args:
            port_list: port obj id list
            hostif_list: host interface obj list

        Returns:
            dict: port_to_hostif_map - port_oid:hostif_oid
        """
        port_to_hostif_map = {}
        for i, port in enumerate(port_list):
            port_to_hostif_map[port] = hostif_list[i]
        return port_to_hostif_map

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
            self.client,  bridge_port_oid=bridge_port_id, ingress_filtering=True,  egress_filtering=True)
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
            # Cannot get those three
            # ingress_filtering=True,
            # egress_filtering=True,
            # isolation_group=True
        )
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
        return attr

    def get_bridge_port_list(self, bridge_id):
        """
        Get bridge ports.

        Args:
            bridge_id: bridge id.

        Returns:
            list: bridge_port_list

        """
        print("Get bridge ports...")
        bridge_port_list = sai_thrift_object_list_t(count=100)
        bp_list = sai_thrift_get_bridge_attribute(
            self.client, bridge_id, port_list=bridge_port_list)
        for index, item in enumerate(bp_list['port_list'].idlist):
            self.test_obj.dut.port_obj_list[index].bridge_port_oid = item
        return bp_list['port_list'].idlist

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

    def get_local_mapped_ports(self):
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
            self.test_obj.dut.port_obj_list[index].dev_port_index = port
            self.test_obj.dut.port_obj_list[index].dev_port_eth = eth
            dev_port_list.append(port)
        return dev_port_list

    def get_port_list(self):
        """
        Set the class variable port_list.

        Returns:
            port_list
        """
        port_list = sai_thrift_object_list_t(count=100)
        p_list = sai_thrift_get_switch_attribute(
            self.client, port_list=port_list)
        for index, item in enumerate(p_list['port_list'].idlist):
            port: Port = Port(oid=item, index=index, rif_list=[], nexthopv4_list=[], nexthopv6_list=[])
            self.test_obj.dut.port_obj_list.append(port)
        return p_list['port_list'].idlist

    def parse_port_config(self, port_config_file):
        """
        Parse port_config.ini file

        Example of supported format for port_config.ini:
        # name        lanes       alias       index    speed    autoneg   fec
        Ethernet0       0         Ethernet0     1      25000      off     none
        Ethernet1       1         Ethernet1     1      25000      off     none
        Ethernet2       2         Ethernet2     1      25000      off     none
        Ethernet3       3         Ethernet3     1      25000      off     none
        Ethernet4       4         Ethernet4     2      25000      off     none
        Ethernet5       5         Ethernet5     2      25000      off     none
        Ethernet6       6         Ethernet6     2      25000      off     none
        Ethernet7       7         Ethernet7     2      25000      off     none
        Ethernet8       8         Ethernet8     3      25000      off     none
        Ethernet9       9         Ethernet9     3      25000      off     none
        Ethernet10      10        Ethernet10    3      25000      off     none
        Ethernet11      11        Ethernet11    3      25000      off     none
        etc

        Args:
            port_config_file (string): path to port config file

        Returns:
            dict: port configuation from file

        Raises:
            e: exit if file not found
        """
        portConfigs = OrderedDict()
        try:
            index = 0
            with open(port_config_file) as conf:
                for line in conf:
                    if line.startswith('#'):
                        if "name" in line:
                            titles = line.strip('#').split()
                        continue
                    tokens = line.split()
                    if len(tokens) < 2:
                        continue

                    name_index = titles.index('name')
                    name = tokens[name_index]
                    data = {}
                    portConfig = PortConfig()
                    for i, item in enumerate(tokens):
                        if i == name_index:
                            continue
                        data[titles[i]] = item
                    portConfig.lanes = [int(lane)
                                        for lane in data['lanes'].split(',')]
                    portConfig.speed = int(data['speed'])
                    portConfig.name = name
                    portConfigs[index] = portConfig
                    self.test_obj.dut.port_obj_list[index].port_config = portConfig
                    index = index + 1
            return portConfigs
        except Exception as e:
            raise e

    def remove_bridge_port(self, bridge_id):
        """
        Remove bridge ports (bridge will not be removed).

        Args:
            bridge_id: bridge id.

        """
        print("Remove bridge ports...")
        bridge_port_list = sai_thrift_object_list_t(count=100)
        bp_list = sai_thrift_get_bridge_attribute(
            self.client, bridge_id, port_list=bridge_port_list)
        bp_ports = bp_list['port_list'].idlist
        for index, port in enumerate(bp_ports):
            sai_thrift_remove_bridge_port(self.client, port)
            self.test_obj.dut.port_obj_list[index].bridge_port_oid=None
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
        fec_mode = self.config.get('fec')
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
        return int(self.config.get('mtu'))

    def get_cpu_port_queue(self):

        attr = sai_thrift_get_switch_attribute(self.client, cpu_port=True)
        self.test_obj.cpu_port = attr['cpu_port']

        attr = sai_thrift_get_port_attribute(self.client,
                                             self.test_obj.cpu_port,
                                             qos_number_of_queues=True)
        num_queues = attr['qos_number_of_queues']
        q_list = sai_thrift_object_list_t(count=num_queues)
        attr = sai_thrift_get_port_attribute(self.client,
                                             self.test_obj.cpu_port,
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


class PortConfig(object):
    """
    Represent the PortConfig Object

    Attrs:
        name: interface name
        lanes: lanes
        speed: port speed
    """

    def __init__(self, name=None, lanes=None, speed=None):
        self.name = name
        self.lanes = lanes
        self.speed = speed
