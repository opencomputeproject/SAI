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
import pickle
import os
from typing import Dict
from typing import List
from data_module.dut import Dut
from data_module.device import Device


class PersistHelper:
    '''
    store and restore data model
    '''

    def __init__(self) -> None:
        self.dir = '/tmp/sai_model'
        if not os.path.exists(self.dir):
            os.makedirs(self.dir)

    def persist_dut(self, dut: Dut):
        '''
        persist dut intance
        Args:
            dut: Dut instance,including all dut configure
        '''
        with open(os.path.join(self.dir, 'dut'), 'wb') as output:
            pickle.dump(dut, output, pickle.HIGHEST_PROTOCOL)

    def persist_server_list(self, server_list: Dict[int, List[Device]]):
        '''
        persist server list
        Args:
            server_list: server(type: device) list
        '''
        with open(os.path.join(self.dir, 'server_list'), 'wb') as output:
            pickle.dump(server_list, output, pickle.HIGHEST_PROTOCOL)

    def persist_t1_list(self, t1_list: Dict[int, List[Device]]):
        '''
        persist t1 list
        Args:
            t1_list: server(type: device) list
        '''
        with open(os.path.join(self.dir, 't1_list'), 'wb') as output:
            pickle.dump(t1_list, output, pickle.HIGHEST_PROTOCOL)

    def read_dut(self) -> Dut:
        '''
        read dut
        Return:
            dut: Dut instance, init by reading from file
        '''
        with open(os.path.join(self.dir, 'dut'), 'rb') as input_obj:
            return pickle.load(input_obj)

    def read_server_list(self) -> Dict[int, List[Device]]:
        '''
        read server list
        Return:
            server_list: init server list by reading from file
        '''
        with open(os.path.join(self.dir, 'server_list'), 'rb') as input_obj:
            return pickle.load(input_obj)

    def read_t1_list(self) -> Dict[int, List[Device]]:
        '''
        read dut
        Return:
            t1_list: init t1 list by reading from file
        '''
        with open(os.path.join(self.dir, 't1_list'), 'rb') as input_obj:
            return pickle.load(input_obj)
