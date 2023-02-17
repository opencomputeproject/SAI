from collections import OrderedDict
from ptf import config
from sai_utils import *  # pylint: disable=wildcard-import; lgtm[py/polluting-import]
from sai_thrift.sai_adapter import *
from typing import TYPE_CHECKING
from data_module.port import Port

from typing import Dict, List

from data_module.port_config import PortConfig
if TYPE_CHECKING:
    from sai_test_base import SaiHelperBase


class PortConfiger(object):
    """
    Class use to make all the basic configurations.
    """

    def __init__(self, test_obj: 'SaiHelperBase') -> None:
        """
        Init the Port configer.

        Args:
            test_obj: the test object
        """
        self.test_obj = test_obj
        self.client = test_obj.client

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
        self.set_test_bridge_port_attr(bp_list)
        return bp_list

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
        # sai_thrift_get_bridge_port_attribute(
        #     self.client,  
        #     bridge_port_oid=bridge_port_id, 
        #     # ingress_filtering=True,  # 11
        #     # egress_filtering=True    # 12
        # )
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


    def load_default_active_1q_bridge_ports(self):
        """
        Loads default 1q bridge ports, bind to port object and set as class attribute.

        Needs the following class attributes:
            self.default_1q_bridge - default_1q_bridge oid

            self.active_ports_no - number of active ports

            self.portX objects for all active ports

        Sets the following class attributes:

            self.default_1q_bridge_port_list - list of all 1q bridge port objects

            self.portX_bp - objects for all 1q bridge ports
        """
        bridge_id = self.test_obj.default_1q_bridge
        default_1q_bridge_port_list = self.get_all_bridge_ports(bridge_id)

        #try to binding the bridge port with the port index here
        print("Assign bridge to port objects...")
        port_list = self.test_obj.active_port_obj_list
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

    def get_all_bridge_ports(self, bridge_id):
        """
        Loads default 1q bridge ports.

        /* Get bridge ports in default 1Q bridge
            * By default, there will be (m_portCount + m_systemPortCount) number of SAI_BRIDGE_PORT_TYPE_PORT
            * ports and one SAI_BRIDGE_PORT_TYPE_1Q_ROUTER port. The former type of
            * ports will be removed. */
        vector<sai_object_id_t> bridge_port_list(m_portCount + m_systemPortCount + 1);
        """
        print("Get bridge ports...")

        bridge_size = self.test_obj.system_port_no + self.test_obj.active_ports_no + 1
        attr = sai_thrift_get_bridge_attribute(
                    self.client,
                    bridge_oid=bridge_id,
                    port_list=sai_thrift_object_list_t(
                        idlist=[], count=bridge_size))
        default_1q_bridge_port_list = attr['port_list'].idlist
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)
        return default_1q_bridge_port_list

    def remove_all_bridge_ports(self):
        """
        Remove bridge ports (bridge will not be removed).
        """
        print("Remove all bridge ports...")
        bp_ports = self.test_obj.def_bridge_port_list
        removed_list = []
        for index, port in enumerate(bp_ports):
            sai_thrift_remove_bridge_port(self.client, port)
            #print("Removed bridge port {}".format(port))
            removed_list.append(port)
        for bridge_id in removed_list:
            self.test_obj.def_bridge_port_list.remove(bridge_id)
        print("Removed bridge {}, left {}".format(len(removed_list), len(self.test_obj.def_bridge_port_list)))
        self.test_obj.assertEqual(self.test_obj.status(), SAI_STATUS_SUCCESS)

    def reset_1q_bridge_ports(self):
        '''
        Reset all the 1Q bridge ports.
        Needs the following class attributes:
            self.default_1q_bridge - default_1q_bridge oid

            self.active_ports_no - number of active ports

            self.portX objects for all active ports
        '''
        #In case the bridge port will be initalized by default, clear them
        bridge_id = self.test_obj.default_1q_bridge
        self.test_obj.def_bridge_port_list \
            = self.get_all_bridge_ports(bridge_id)
        self.remove_all_bridge_ports()
        # self.test_obj.def_bridge_port_list \
        #     = self.create_bridge_ports(
        #         bridge_id, self.test_obj.active_port_obj_list)


    def remove_1q_bridge_port(self, default_1q_bridge_port_list):
        '''
        Removes all the bridge ports.
        '''
        for index in range(0, len(default_1q_bridge_port_list)):
            port_bp = getattr(self.test_obj, 'port%s_bp' % index)
            sai_thrift_remove_bridge_port(self.client, port_bp)
            delattr(self.test_obj, 'port%s_bp' % index)

    # Local methods

    def get_port_id_list(self, port_obj_list:List[Port]):
        port_id_list = []
        for item in port_obj_list:
            port_id_list.append(item.oid)
        return port_id_list


    def create_bridge_ports_by_bridge_and_ports(self, bridge_id, port_list: List['Port']):
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
                    name=item.config.name)
                sai_thrift_set_hostif_attribute(
                    self.client, hostif_oid=hostif, oper_status=False)
                hostif_list[index] = hostif
                item.host_itf_id = hostif
            except BaseException as e:
                print("Cannot create hostif, error : {}".format(e))
        return host_intf_table_id, hostif_list


    def create_port_hostif_by_port_config_ini(self, port_list: List['Port'], trap_group=None):
        print("Create Host intfs...")
        host_intf_table_id = sai_thrift_create_hostif_table_entry(
            self.client, type=SAI_HOSTIF_TABLE_ENTRY_TYPE_WILDCARD,
            channel_type=SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_PHYSICAL_PORT)
        sai_thrift_create_hostif_trap(
            self.client, trap_type=SAI_HOSTIF_TRAP_TYPE_TTL_ERROR, packet_action=SAI_PACKET_ACTION_TRAP,
            trap_group=trap_group, trap_priority=0)
        hostif_list = [None]*len(port_list)
        for index, port in enumerate(port_list):
            try:
                hostif = sai_thrift_create_hostif(
                    self.client,
                    type=SAI_HOSTIF_TYPE_NETDEV,
                    obj_id=port.oid,
                    name=port.config.name)
                sai_thrift_set_hostif_attribute(
                    self.client, hostif_oid=hostif, oper_status=False)
                hostif_list[index] = hostif
                port.host_itf_id = hostif
                # print("Create hostitf: name:{} port hardIdx: {} port lane: {}".format(
                #     port.config.name, port.port_index, port.config.lanes)
                # )
            except BaseException as e:
                print("Cannot create hostif, error : {}".format(e))
        return host_intf_table_id, hostif_list


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
            self.test_obj.active_port_obj_list[index].dev_port_index = port
            self.test_obj.active_port_obj_list[index].dev_port_eth = eth
            dev_port_list.append(port)
        return dev_port_list

    def get_port_default_lane(self) -> List[Port]:
        """
        Get Port default lane.

        Returns:
            port_list
        """
        port_obj_list_with_lane: List[Port] = []
        # the active number must match the actual account, or might return null
        port_list = sai_thrift_object_list_t(
            idlist=[], count=self.test_obj.active_ports_no)
        p_list = sai_thrift_get_switch_attribute(self.client, port_list=port_list)
        for index, item in enumerate(p_list['port_list'].idlist):
            port: Port = Port(oid=item, port_index=index)
            temp_list = sai_thrift_object_list_t(
                count=self.test_obj.active_ports_no)
            attr = sai_thrift_get_port_attribute(
                self.client, port_oid=port.oid, hw_lane_list=temp_list)
            port.default_lane_list = attr['hw_lane_list'].uint32list
            port_obj_list_with_lane.append(port)
        return port_obj_list_with_lane

    def get_lane_sorted_port_list(self, port_obj_list_with_lane: List[Port]):
        """
        Get the port list sorted by lanes name(defined in port_config.ini).
        This method will cut the ports list base on the lanes.
        i.e. If device has 64 ports but just define 32 in lanes, then just generate 32 ports

        Args:
            port_obj_list_with_lane: port obj with default lane info
        Returns:
            port_list
        """
        port_id_list = []
        self.test_obj.active_port_obj_list = self.sort_port_list_by_config(self.test_obj.ports_config, port_obj_list_with_lane)
        for item in self.test_obj.active_port_obj_list:
            port_id_list.append(item.oid)
        print("Base on lanes config file[{}], init {} ports from {} device ports"
        .format("configdb.json",
                len(self.test_obj.active_port_obj_list),
                len(port_obj_list_with_lane)
                ))
        return port_id_list

    def sort_port_list_by_config(self, ports_config: List[PortConfig], port_list: List[Port]):
        """
        Sort the port list base on the port_config.ini.
        This method will match the default_lane_list in the port object with the lane defined in 
        port config for a ordered port list.
        This method will create the port list base on the mini number of interfaces from
        command parameters or port_config.ini

        Attrs:
            ports_config: port config, which gets from the port_config.ini
            port_list: port list
        """
        sorted_port_list: List[Port] = []
        for index, item in enumerate(ports_config):
            # index: interface name, item: PortConfig
            lane_list = item.lanes
            for port in port_list:
                if lane_list == port.default_lane_list:
                    sorted_port_list.append(port)
                    port.config = ports_config[index]
                    break
        return sorted_port_list

    def remove_bridge_port(self):
        """
        Remove bridge ports base on the active port list.(bridge will not be removed).
        Bridge relation will be removed from port as well.

        """
        print("Remove bridge ports...")
        bp_ports = self.test_obj.def_bridge_port_list
        for index, port in enumerate(bp_ports):
            sai_thrift_remove_bridge_port(self.client, port)
            #print("Removed bridge port {}".format(port))
            if self.test_obj.active_port_obj_list[index].bridge_port_oid != port:
                print("WARN! BUG! Bridge not as expected, not equals to port record.")
            self.test_obj.active_port_obj_list[index].bridge_port_oid = None
            self.test_obj.def_bridge_port_list.remove(port)
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


    def set_port_attribute(self, port_list: List['Port']):
        """
        Turn on port admin state

        Args:
            post_list: post list
        """
        print("Set port...")
        for index, port in enumerate(port_list):
            #self.log_port_state(port, index)
            sai_thrift_set_port_attribute(
                self.client, port_oid=port.oid, mtu=port.config.mtu,
                fec_mode=self.get_fec_mode(port),
                speed=port.config.speed)


    def turn_up_and_get_checked_ports(self, port_list: List['Port']):
        '''
        Method to turn up the ports.
        In case some device not init the port after start the switch.

        Args:
            port_list - list of all active port objects

        Return:
            Port list for testing.
        '''

        # For brcm devices, need to init and setup the ports at once after start the switch.
        retries = 10
        down_port_list = []
        test_port_list:List[Port] = []

        print("Turn up ports...")        
        for index, port in enumerate(port_list):
            if not port.dev_port_index and index != 0:
                print("Skip turn up port {} , local index {} id {} name{}.".format(
                        index, port.port_index, port.oid, port.config.name))
                continue
            test_port_list.append(port)
            sai_thrift_set_port_attribute(
                        self.client,
                        port_oid=port.oid,
                        admin_state=True)

        for index, port in enumerate(test_port_list):            
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
                    # self.log_port_state(port, index)
                    print("port {} , local index {} id {} is not up, status: {}. Retry. Reset Admin State.".format(
                        index, port.port_index, port.oid, port_attr['oper_status']))
                    sai_thrift_set_port_attribute(
                        self.client,
                        port_oid=port.oid,
                        admin_state=True)
            if not port_up:
                down_port_list.append(index)
        if down_port_list:
            print("Ports {} are  down after retries.".format(down_port_list))
        return test_port_list

    def get_fec_mode(self, port: Port):
        '''
        get fec mode from config_db.json
        RETURN:
             int: SAI_PORT_FEC_MODE_X
        '''
        fec_change = {
            None: SAI_PORT_FEC_MODE_NONE,
            'rs': SAI_PORT_FEC_MODE_RS,
            'fc': SAI_PORT_FEC_MODE_FC,
        }
        return fec_change[port.config.fec]

    def log_port_state(self, port:Port, index):
        print("port index:{} hardIdx: {} "
        .join("ptf_dev_idx: {} name:{}")
        .join("config lane:{} mtu:{} fec:{} speed:{}").format(
            index,
            port.port_index,
            port.dev_port_index,
            port.config.name,
            port.config.lanes,
            port.config.mtu,
            self.get_fec_mode(port),
            port.config.speed))

    # Method to compatiable with test structure

    def set_test_port_attr(self, port_list):
        for index, oid in enumerate(port_list):
            setattr(self.test_obj, 'port%s' % index, oid)

    def set_test_bridge_port_attr(self, bridge_port_list):
        for index, oid in enumerate(bridge_port_list):
            setattr(self.test_obj, 'port%s_bp' % index, oid)
