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

from typing import List


class Lag(object):
    """
    Represent the lag object.
    Attrs:
        lag_id: lag id
        lag_members: lag members
        member_port_indexs: lag port member indexes
        lag_rif: lag related route interface
    """

    def __init__(self, lag_id=None, lag_members: List = [], member_port_indexs: List = []):
        """
        Init Lag Object
        Init following attrs:
            lag_id
            lag_members
            member_port_indexs
            lag_rif
        """
        self.lag_id = None
        self.lag_members: List = lag_members
        self.member_port_indexs: List = member_port_indexs
        self.lag_rif = None
