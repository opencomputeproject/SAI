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
import time
from collections import OrderedDict
from threading import Thread
from typing import Dict, List

import sai_thrift.sai_adapter as adapter
from ptf import config
from ptf.base_tests import BaseTest
from ptf import testutils
from unittest import SkipTest

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

from sai_thrift import sai_rpc
from sai_thrift.sai_adapter import *
from thrift.protocol import TBinaryProtocol
from thrift.transport import TSocket, TTransport

from config.fdb_configer import (FdbConfiger, t0_fdb_config_helper,
                                 t0_fdb_tear_down_helper)
from config.lag_configer import LagConfiger, t0_lag_config_helper
from config.port_configer import (PortConfiger, t0_port_config_helper,
                                  t0_port_tear_down_helper)
from config.route_configer import RouteConfiger, t0_route_config_helper
from config.switch_configer import SwitchConfiger, t0_switch_config_helper
from config.vlan_configer import (VlanConfiger, t0_vlan_config_helper,
                                  t0_vlan_tear_down_helper)
from data_module.device import Device, DeviceType
from data_module.dut import Dut
from data_module.lag import Lag
from data_module.persist import PersistHelper
from data_module.vlan import Vlan
from sai_utils import *

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

    def setUp(self, skip_reason = None):
        """
        Set up the test env.
        """
        if skip_reason:
            print("SkipTest as {}".format(skip_reason))
            testutils.skipped_test_count=1
            raise SkipTest(skip_reason)

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
        print("\ncommon_configured is: {}".format(self.common_configured))

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

    def setUp(self, skip_reason = None):
        """
        Setup the ThriftInterfaceDataPlane.
        """
        super(ThriftInterfaceDataPlane, self).setUp(skip_reason = skip_reason)

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
        self.server_groups = [0, 1, 2, 11, 12, 13, 14, 60]
        """
        Group numbers for server
        """

        self.t1_groups = [1, 2, 3, 4]
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
        self.persist_helper = PersistHelper()
        self.ports_config = None

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
              is_create_route_for_nhopgrp=False,
              wait_sec=5,
              skip_reason = None):
        super(T0TestBase, self).setUp(skip_reason = skip_reason)

        # parse the port_config.ini, will create port, bridge port and host interface base on that file
        if 'port_config_ini' in self.test_params:
            self.ports_config = self.parsePortConfig(self.test_params['port_config_ini'])        

        self.port_configer = PortConfiger(self)
        self.switch_configer = SwitchConfiger(self)
        self.fdb_configer = FdbConfiger(self)
        self.vlan_configer = VlanConfiger(self)
        self.route_configer = RouteConfiger(self)
        self.lag_configer = LagConfiger(self)

        if force_config or not self.common_configured:
            self.create_device()
            t0_switch_config_helper(self)
            t0_port_config_helper(
                test_obj=self,
                is_create_hostIf=is_create_hostIf,
                is_recreate_bridge=is_recreate_bridge)
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
                is_create_route_for_lag=is_create_route_for_lag,
                is_create_route_for_nhopgrp=is_create_route_for_nhopgrp)
            print("common config done, persist it")
            self.persist_helper.persist_dut(self.dut)
            self.persist_helper.persist_server_list(self.servers)
            self.persist_helper.persist_t1_list(self.t1_list)
            print("Waiting for switch to get ready before test, {} seconds ...".format(
                wait_sec))
            time.sleep(wait_sec)
        else:
            print("switch keeps running, read config from storage")
            self.dut = self.persist_helper.read_dut()
            self.t1_list = self.persist_helper.read_t1_list()
            self.servers = self.persist_helper.read_server_list()

    def restore_fdb_config(self):
        """
        Restore the FDB configurations.
        """
        t0_fdb_tear_down_helper(self)
        t0_fdb_config_helper(test_obj=self)


    def parsePortConfig(self, port_config_file):
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
        ports = OrderedDict()
        try:
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
                    for i, item in enumerate(tokens):
                        if i == name_index:
                            continue
                        data[titles[i]] = item
                    data['lanes'] = [int(lane)
                                     for lane in data['lanes'].split(',')]
                    data['speed'] = int(data['speed'])
                    ports[name] = data
            return ports
        except Exception as e:
            raise e

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

    def create_vlan_interface(self, vlan: Vlan, reuse=True):
        """
        Create vlan route interface.

        Attrs:
            Vlan: vlan for the route interface
        """
        self.route_configer.create_router_interface(vlan, reuse)

    def create_lag_interface(self, lag: Lag, reuse=True):
        """
        Create lag route interface.

        Attrs:
            Lag: lag for the route interface
        """
        self.route_configer.create_router_interface(lag, reuse)

    def get_dev_port_index(self, port_index):
        """
        port_index: port index
        return dev port index from port index
        """
        return self.dut.port_obj_list[port_index].dev_port_index

    def get_dev_port_indexes(self, port_indexes:List):
        """
        port_indexes: port index list
        return dev port indexes from port indexes
        """
        dev_port_indexes = []
        for port_index in port_indexes:
            dev_port_indexes.append(self.dut.port_obj_list[port_index].dev_port_index)
        return dev_port_indexes

    def tearDown(self):
        '''
        tear down
        todo:
            if we change the common configure in ths case,
            we need persist dut again 
        '''
        super().tearDown()
