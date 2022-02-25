#!/usr/bin/env python2

# Copyright 2013-present Barefoot Networks, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# Antonin Bas (antonin@barefootnetworks.com)
#
#
import sys
sys.path.append('../../tools/')
import runtime_CLI as cli
from subprocess import Popen, call
import shlex

class SwitchThriftClient():
    def __init__(self, ip='localhost', port=9090,services=cli.PreType.SimplePreLAG,
                 json='../../../p4-softswitch/targets/P4-SAI/sai.json',
                 default_config='../../../p4-softswitch/targets/P4-SAI/p4src/DefaultConfig.txt'):
        self.pre = services
        self.standard_client, self.mc_client = self.ConnectToThrift(ip, port, services, json)
        self.json = json
        self.default_config = default_config

    def ConnectToThrift(self, ip, port, services, json):
        standard_client, mc_client = cli.thrift_connect(
            ip, port,
            cli.RuntimeAPI.get_thrift_services(services)
        )
        cli.load_json_config(standard_client, json)
        return standard_client, mc_client

    def AddTable(self, table_name, action_name, match_string, value_string):
        cmd = '%s %s %s => %s' % (table_name, action_name, match_string, value_string)
        print 'table_add ' + cmd
        cli.RuntimeAPI(self.pre, self.standard_client, self.mc_client).do_table_add(cmd)


    def RemoveTableEntry(self, table_name, match_string):
        return cli.RuntimeAPI(self.pre, self.standard_client, self.mc_client).do_table_delete_entry_from_key('%s %s' % (table_name, match_string))

    def ReloadDefaultConfig(self):
        cli.RuntimeAPI(self.pre, self.standard_client, self.mc_client).do_load_new_config_file(self.json)
        with open(self.default_config,'r') as def_file:
            for line in def_file:
                cli.RuntimeAPI(self.pre, self.standard_client, self.mc_client).onecmd(line.strip('\n'))
        cli.RuntimeAPI(self.pre, self.standard_client, self.mc_client).do_swap_configs('')

def main():
    args = cli.get_parser().parse_args()
    client = SwitchThriftClient()
    client.AddTable('table_ingress_lag', 'action_set_l2if', '20', '')
    client.ReloadDefaultConfig()
    client.AddTable('table_ingress_lag', 'action_set_l2if', '11', '')
    # parse_match_key(0)
    # entry_handle = standard_client.bm_mt_add_entry(
            # 0, 'table_ingress_lag', 0, 'action_set_lag_l2if', [0, 0, 0],
            # BmAddEntryOptions(priority = 0)
        # )

    #RuntimeAPI(args.pre, standard_client, mc_client).cmdloop()
    
    # table_name = 'table_ingress_lag'
    # match_key = []
    # bitwidths = [16]
    # key_fields=['0']
    # for bw, field in zip(bitwidths, key_fields):
    #     key = cli.bytes_to_string(cli.parse_param(field, bw))
    #     param = cli.BmMatchParam(type = cli.BmMatchParamType.EXACT,
    #                              exact = cli.BmMatchParamExact(key))
    #     match_key.append(param)
    # # match_key = [cli.BmMatchParam(type = cli.BmMatchParamType.EXACT,
    #                          # exact = cli.BmMatchParamExact('0'))]
    # print match_key
    # action_name = 'action_set_lag_l2if'
    # params = ['0', '0', '0']
    # bitwidths = [1, 16, 3]
    # byte_array = []
    # for input_str, bitwidth in zip(params, bitwidths):
    #     byte_array += [cli.bytes_to_string(cli.parse_param(input_str, bitwidth))]
    # runtime_data = byte_array
    # priority = 0
    # entry_handle = standard_client.bm_mt_add_entry(0, table_name, match_key, action_name, runtime_data,
    #                                                cli.BmAddEntryOptions(priority = priority))
if __name__ == '__main__':
    main()
