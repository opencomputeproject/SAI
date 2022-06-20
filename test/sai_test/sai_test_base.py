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

THRIFT_PORT = 9092
is_configured = False


class ThriftInterface(BaseTest):
    """
    Get and format a port map, retrieve test params, and create an RPC client
    """

    def setUp(self):
        super(ThriftInterface, self).setUp()

        self.interface_to_front_mapping = {}
        self.port_map_loaded = False

        self.transport = None
        self.test_reboot_mode = None
        self.test_reboot_stage = None

        self.test_params = test_params_get()
        self.loadTestRebootMode()
        self.loadPortMap()
        self.createRpcClient()

    def tearDown(self):
        self.transport.close()

        super(ThriftInterface, self).tearDown()

    def loadTestRebootMode(self):
        """
        Get if test the reboot mode and what's the reboot mode need to be tested

        In reboot mode, test will run many times in different reboot stage.
        Tests in different stage might be different.

        Set the following class attributes:
        self.test_reboot_loaded - if the reboot mode already loaded
        self.test_reboot_mode - reboot mode
        self.test_reboot_stage - reboot stage, can be [setup|starting|post]
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
    """

    def setUp(self):
        super(ThriftInterfaceDataPlane, self).setUp()

        self.dataplane = ptf.dataplane_instance
        if self.dataplane is not None:
            self.dataplane.flush()
            if config['log_dir'] is not None:
                filename = os.path.join(config['log_dir'], str(self)) + ".pcap"
                self.dataplane.start_pcap(filename)

    def tearDown(self):
        if config['log_dir'] is not None:
            self.dataplane.stop_pcap()
        super(ThriftInterfaceDataPlane, self).tearDown()


class T0TestBase(ThriftInterfaceDataPlane):
    """
    SAI test helper base class without initial switch ports setup

    Set the following class attributes:
        self.default_vlan_id
        self.default_vrf
        self.default_1q_bridge
        self.cpu_port_hdl
        self.active_ports_no - number of active ports
        self.port_list - list of all active port objects
        self.portX objects for all active ports (where X is a port number)
    """

    def setUp(self,
              force_config=False,
              is_create_hostIf=True,
              is_recreate_bridge=True,
              is_reset_default_vlan=True,
              is_create_vlan=True,
              is_create_fdb=True):
        super(T0TestBase, self).setUp()
        self.port_configer = PortConfiger(self)
        self.switch_configer = SwitchConfiger(self)
        self.fdb_configer = FdbConfiger(self)
        self.vlan_configer = VlanConfiger(self)

        if force_config or not is_configured:
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
