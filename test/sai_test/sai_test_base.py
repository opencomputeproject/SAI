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

import os
from threading import Thread

from ptf import config
from ptf.base_tests import BaseTest

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

from sai_thrift import sai_rpc

import sai_thrift.sai_adapter as adapter
from sai_thrift.sai_adapter import *
from sai_utils import *

import time

from config.port_configer import t0_port_config_helper
from config.port_configer import t0_port_tear_down_helper
from config.port_configer import PortConfiger
from config.switch_configer import t0_switch_config_helper
from config.switch_configer import SwitchConfiger
from config.vlan_configer import t0_vlan_config_helper
from config.vlan_configer import t0_vlan_tear_down_helper
from config.vlan_configer import VlanConfiger
from config.fdb_configer import t0_fdb_config_helper
from config.fdb_configer import t0_fdb_tear_down_helper
from config.vlan_configer import VlanConfiger
from config.fdb_configer import t0_fdb_config_helper
from config.fdb_configer import FdbConfiger
from config.lag_configer import t0_lag_config_helper
from config.lag_configer import LagConfiger
from config.route_configer import t0_route_config_helper
from config.route_configer import RouteConfiger
from data_module.dut import Dut
from data_module.vlan import Vlan
from data_module.lag import Lag
from data_module.device import Device
from data_module.device import DeviceType
from typing import List
from typing import Dict

THRIFT_PORT = 9092


class ThriftInterface(BaseTest):
    """
    Get and format a port map, retrieve test params, and create an RPC client

    Have the following class attributes:
        port_map_loaded: If the Port map loaded when Test init
        transport: Thrift socket object
        test_reboot_mode: reboot mode, which will be read from system env
        test_reboot_stage: reboot stage, which will be read from system env
        test_params: All the values passed via test-params if present
        interface_to_front_mapping: Config from port_map_file for the interface (local) to front(PTF) mapping 
        common_configured: represent if the common_configured in test-params has been loaded
        protocol: Thrift protocol object
        client: RPC client which used in Thrift
    """

    def __init__(self, *args, **kwargs):
        """
        Init ThriftInterface.

        Set the following class attributes:
            port_map_loaded: If the Port map loaded when Test init
            transport: Thrift socket object
            test_reboot_mode: reboot mode, which will be read from system env
            test_reboot_stage: reboot stage, which will be read from system env
            test_params: All the values passed via test-params if present
            interface_to_front_mapping: Config from port_map_file for the interface (local) to front(PTF) mapping 
            common_configured: represent if the common_configured in test-params has been loaded
            protocol: Thrift protocol object
            client: RPC client which used in Thrift
        """
        super().__init__(*args, **kwargs)
        self.port_map_loaded = False
        """
        If the Port map loaded when Test init
        """

        self.transport = None
        """
        Thrift socket object
        """

        self.test_reboot_mode = None
        """
        reboot mode, which will be read from system env
        """
        self.test_reboot_stage = None
        """
        reboot stage, which will be read from system env
        """

        self.interface_to_front_mapping = {}
        """
        Config from port_map_file for the interface (local) to front(PTF) mapping 
        """

        self.test_params = None
        """
        All the values passed via test-params if present
        """

        self.common_configured = None
        """
        Represent if the common_configured in test-params has been loaded
        """

        self.protocol = None
        """
        Thrift protocol object
        """

        self.client = None
        """
        RPC client which used in Thrift
        """

    def setUp(self):
        """
        Set up the test env.
        """
        super(ThriftInterface, self).setUp()

        self.test_params = test_params_get()
        self.loadCommonConfigured()
        self.loadTestRebootMode()
        self.loadPortMap()
        self.createRpcClient()

    def tearDown(self):
        """
        Clean up all the test env
        """
        self.transport.close()

        super(ThriftInterface, self).tearDown()

    def loadTestRebootMode(self):
        """
        Get if test the reboot mode and what's the reboot mode need to be tested

        In reboot mode, test will run many times in different reboot stage.
        Tests in different stage might be different.

        Set the following class attributes:
            test_reboot_loaded - if the reboot mode already loaded
            test_reboot_mode - reboot mode
            test_reboot_stage - reboot stage, can be [setup|starting|post]
        """
        if "test_reboot_mode" in self.test_params:
            self.test_reboot_mode = self.test_params['test_reboot_mode']
            if "test_reboot_stage" in self.test_params:
                self.test_reboot_stage = self.test_params['test_reboot_stage']
            else:
                raise ValueError('test_reboot_stage is Null!')
        else:
            self.test_reboot_mode = 'cold'

        print("Reboot mode is: {}".format(self.test_reboot_mode))

    def loadPortMap(self):
        """
        Get and format port_map

        port_map_file is a port map with following lines format:
        [test_port_no]@[device_port_name]
        e.g.:
             0@Veth1
             1@Veth2
             2@Veth3  ...
        """
        if self.port_map_loaded:
            print("port_map already loaded")
            return

        if "port_map" in self.test_params:
            user_input = self.test_params['port_map']
            splitted_map = user_input.split(",")
            for item in splitted_map:
                iface_front_pair = item.split("@")
                self.interface_to_front_mapping[iface_front_pair[0]] =  \
                    iface_front_pair[1]
        elif "port_map_file" in self.test_params:
            user_input = self.test_params['port_map_file']
            with open(user_input, 'r') as map_file:
                for line in map_file:
                    if (line and (line[0] == '#' or
                                  line[0] == ';' or line[0] == '/')):
                        continue
                    iface_front_pair = line.split("@")
                    self.interface_to_front_mapping[iface_front_pair[0]] =  \
                        iface_front_pair[1].strip()
        self.port_map_loaded = True

    def loadCommonConfigured(self):
        '''
        if common_configured = true:
                set up common config
        else:
                skip commmon config
        '''
        if "common_configured" in self.test_params:
            self.common_configured = True if self.test_params['common_configured'] == 'true' else False
        else:
            self.common_configured = False
        print("common_configured is: {}".format(self.common_configured))

    def createRpcClient(self):
        """
        Set up thrift client and contact RPC server
        """

        if 'thrift_server' in self.test_params:
            server = self.test_params['thrift_server']
        else:
            server = 'localhost'

        self.transport = TSocket.TSocket(server, THRIFT_PORT)
        self.transport = TTransport.TBufferedTransport(self.transport)
        self.protocol = TBinaryProtocol.TBinaryProtocol(self.transport)
        if self.test_reboot_stage == 'starting':
            return
        self.client = sai_rpc.Client(self.protocol)
        self.transport.open()


class ThriftInterfaceDataPlane(ThriftInterface):
    """
    Sets up the thrift interface and dataplane

    class attributes:
        dataplane: Represent the dataplane used in test, pcap to manipulate the data
    """

    def __init__(self, *args, **kwargs):
        """
        Init ThriftInterfaceDataPlane

        Set the following class attributes:
            dataplane: Represent the dataplane used in test, pcap to manipulate the data
        """
        super().__init__(*args, **kwargs)
        self.dataplane = None
        """
        Represent the dataplane used in test, pcap to manipulate the data
        """

    def setUp(self):
        """
        Setup the ThriftInterfaceDataPlane.
        """
        super(ThriftInterfaceDataPlane, self).setUp()

        self.dataplane = ptf.dataplane_instance
        if self.dataplane is not None:
            self.dataplane.flush()
            if config['log_dir'] is not None:
                filename = os.path.join(config['log_dir'], str(self)) + ".pcap"
                self.dataplane.start_pcap(filename)

    def tearDown(self):
        """
        Clean up ThriftInterfaceDataPlane.
        """
        if config['log_dir'] is not None:
            self.dataplane.stop_pcap()
        super(ThriftInterfaceDataPlane, self).tearDown()


class T0TestBase(ThriftInterfaceDataPlane):
    """
    SAI test helper base class

    class attributes:
        dut: Dut object in test.
        servers: Simulating the server Objects in Test.
        t1_list: Simulating the T1 objects in test

    """

    def __init__(self, *args, **kwargs):
        """
        Init the T0 Test Object.
        Set the following class attributes:
            dut: Dut object in test.
            servers: Simulating the server Objects in Test.
            t1_list: Simulating the T1 objects in test

        """
        super().__init__(*args, **kwargs)
        self.dut = Dut()
        """
        Represent the DUT in test.
        """
        self.server_groups = [0, 1, 2, 11, 12]
        """
        Group numbers for server
        """

        self.t1_groups = [1, 2]
        """
        Group numbers for server
        """

        self.num_device_each_group = 101
        """
        Device numbers in each group
        """
        # Dict key: group id, Value: devices list
        self.servers: Dict[int, List[Device]] = {}
        """
        Simulating the server Objects in Test.
        Key: group id
        Value: List, servers
        """
        self.t1_list: Dict[int, List[Device]] = {}
        """
        Simulating the T1 objects in test
        Key: group id
        Value: List, servers
        """

    def setUp(self,
              force_config=False,
              is_create_hostIf=True,
              is_recreate_bridge=True,
              is_reset_default_vlan=True,
              is_create_vlan=True,
              is_create_fdb=True,
              is_create_default_route=True,
              is_create_default_loopback_interface=False,
              is_create_lag=True,
              is_create_route_for_lag=True,
              wait_sec=5):
        super(T0TestBase, self).setUp()

        self.create_device()

        self.port_configer = PortConfiger(self)

        self.switch_configer = SwitchConfiger(self)
        self.fdb_configer = FdbConfiger(self)
        self.vlan_configer = VlanConfiger(self)
        self.route_configer = RouteConfiger(self)
        self.lag_configer = LagConfiger(self)

        if force_config or not self.common_configured:
            t0_switch_config_helper(self)
            t0_port_config_helper(
                test_obj=self,
                is_create_hostIf=is_create_hostIf,
                is_recreate_bridge=is_recreate_bridge)
            # init port rif list
            self.dut.port_rif_list = [None] * len(self.dut.dev_port_list)
            # init bridge port rif list
            self.dut.bridge_port_rif_list = [None] * len(self.dut.bridge_port_list)
            t0_vlan_config_helper(
                test_obj=self,
                is_reset_default_vlan=is_reset_default_vlan,
                is_create_vlan=is_create_vlan)
            t0_fdb_config_helper(
                test_obj=self,
                is_create_fdb=is_create_fdb)
            t0_lag_config_helper(
                test_obj=self,
                is_create_lag=is_create_lag)
            t0_route_config_helper(
                test_obj=self,
                is_create_default_route=is_create_default_route,
                is_create_default_loopback_interface=is_create_default_loopback_interface,
                is_create_route_for_lag=is_create_route_for_lag)

        print("Waiting for switch to get ready before test, {} seconds ...".format(
            wait_sec))
        time.sleep(wait_sec)

    def restore_fdb_config(self):
        """
        Restore the FDB configurations.
        """
        t0_fdb_tear_down_helper(self)
        t0_fdb_config_helper(test_obj=self)

    def shell(self):
        '''
        Method use to start a sai shell in a thread.
        '''
        def start_shell():
            sai_thrift_set_switch_attribute(
                self.client, switch_shell_enable=True)
        thread = Thread(target=start_shell)
        thread.start()

    @staticmethod
    def status():
        """
        Returns the last operation status.

        Returns:
            int: sai call result
        """
        return adapter.status

    @staticmethod
    def saiWaitFdbAge(timeout):
        """
        Wait for fdb entry to ageout

        Args:
            timeout (int): Timeout value in seconds
        """
        print("Waiting for fdb entry to age")
        aging_interval_buffer = 10
        time.sleep(timeout + aging_interval_buffer)

    def create_device(self):
        """
        Init the device data.

        Server in format 192.168.[group_id].[nums_index]
        T1 in format 10.1.[[group_id].[nums_index]]
        group_id: group id for the server
        nums_index: index number among nums, start from 0
        """

        for srv_grp_idx in self.server_groups:
            self.servers[srv_grp_idx] = [Device(DeviceType.server, index, srv_grp_idx)
                                         for index in range(0, self.num_device_each_group)]
        for t1_grp_idx in self.t1_groups:
            self.t1_list[t1_grp_idx] = [Device(DeviceType.t1, index, t1_grp_idx)
                                        for index in range(0, self.num_device_each_group)]

    def create_vlan_interface(self, vlan: Vlan):
        """
        Create vlan route interface.

        Attrs:
            Vlan: vlan for the route interface
        """
        self.route_configer.create_router_interface_by_vlan(vlan)

    def create_lag_interface(self, lag: Lag):
        """
        Create lag route interface.

        Attrs:
            Lag: lag for the route interface
        """
        self.route_configer.create_router_interface_by_lag(lag)

    def tearDown(self):
        '''
        tear down
        '''
        t0_fdb_tear_down_helper(self)
        t0_vlan_tear_down_helper(self)
        t0_port_tear_down_helper(self)
        super().tearDown()
