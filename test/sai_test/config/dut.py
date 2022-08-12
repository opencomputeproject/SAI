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

class Dut():
    """
    dut config
    """

    def __init__(self):

        # router
        self.default_vrf = None
        self.default_ipv6_route_entry = None
        self.default_ipv4_route_entry = None
        self.loopback_intf = None
        self.local_10v6_route_entry = None
        self.local_128v6_route_entry = None
        self.lag1_rif = None
        self.lag1_nbr = None
        self.lag1_nhop = None
        self.lag1_route = None
        self.lag2_rif = None
        self.lag2_nbr = None
        self.lag2_nhop = None
        self.lag2_route = None

        # vlan
        self.default_vlan_id = None
        self.vlans = {}

        # switch
        self.switch_id = None

        # fdb
        self.default_vlan_fdb_list = None
        self.vlan_10_fdb_list = None
        self.vlan_20_fdb_list = None

        # port
        self.bridge_port_list = None
        self.default_1q_bridge_id = None
        self.default_trap_group = None
        self.dev_port_list = None
        self.host_intf_table_id = None
        self.portConfigs = None
        self.port_list = None
        self.port_to_hostif_map = None
        self.hostif_list = None
        self.port0_rif = None

        # lag
        self.lag1 = None
        self.lag2 = None
