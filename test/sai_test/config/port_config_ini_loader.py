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
import os
from os.path import exists

from typing import TYPE_CHECKING
from typing import List, Dict

from data_module.data_obj import auto_str

DEFAULT_CONFIG_DB = "../resources/port_config.ini"

class PortConfigInILoader():
    '''
    Read config from port_config.ini.
    Load the data from a port_config.ini file.
    '''

    def __init__(self, file_path: str = None):
        """
            Init the ConfigDBLoader.

            Args:
                file_path: port_config.ini file path
        """


        self.file_path = self.__validate_file_path__(file_path)
        self.ports_config: Dict = None
        """
        ports_config dict, use to compatiable with old data module
        """
        self.portConfigs: List[PortConfig] = None
        """
        PortConfig object list
        """


    def __validate_file_path__(self, file_path):
        """
        Validate if the file exists.
        Return:
            A validated file path
        """
        config_path = None
        if file_path:
            config_path = file_path
            print("port_config.ini path is {}".format(config_path))
        else:
            config_path = os.path.join(os.path.dirname(__file__),
                            DEFAULT_CONFIG_DB)
            print("port_config.ini uses default path {}".format(config_path))
        file_exists = exists(config_path)
        if not file_exists:
            raise FileNotFoundError("File not found:{}. Please refer to {} for how to set it.".format(
                "https://github.com/opencomputeproject/SAI/blob/master/ptf/docs/SAI-PTFv2Overview.md#run-test",
                 config_path))
        return config_path


    def parse_port_config(self):
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

        Returns:
            dict and PortConfig: port configuation from file

        Raises:
            e: exit if file not found
        """
        ports = OrderedDict()
        portConfigs = OrderedDict()
        try:
            index = 0
            with open(self.file_path) as conf:
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

                    portConfig = PortConfig()
                    portConfig.lanes = data['lanes']
                    portConfig.speed = data['speed']
                    portConfig.name = name
                    portConfigs[index] = portConfig
                    index = index + 1
                    index = index + 1
            self.ports_config = ports
            self.portConfigs = portConfigs
            return ports, portConfigs
        except Exception as e:
            raise e

@auto_str
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
