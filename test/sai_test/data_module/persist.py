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
from data_module.dut import Dut


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
        '''

        with open(os.path.join(self.dir, 'dut'), 'wb') as output:
            pickle.dump(dut, output, pickle.HIGHEST_PROTOCOL)

    def read_dut(self):
        '''
        read dut
        '''
        with open(os.path.join(self.dir, 'dut'), 'rb') as input_obj:
            return pickle.load(input_obj)

if __name__== '__main__':
    d = Dut()
    p = PersistHelper()
    p.persist_dut(d)