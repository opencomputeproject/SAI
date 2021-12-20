# Copyright 2021-present Intel Corporation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Thrift SAI interface HASH tests
"""

import binascii

from ptf.testutils import *
from sai_base_test import *

ROUTER_MAC = '00:77:66:55:44:00'
MAX_ITRS = 50
L3_MAX_ITRS = 200
LAG_MAX_ITRS = 120
DEBUG = False


# Default hash seeds values.
TEST_ECMP_SEED = 211  # test default ECMP seed value
TEST_ECMP_SEED1 = 411  # ecmp seed tests second value
TEST_LAG_SEED = 2557  # test default LAG seed value
TEST_LAG_SEED1 = 411  # LAG seed tests second value

# accepted LB minumum count level per port.
#       as min = LB avrg * 0.5
# e.g for 3 ports and 50 iterations
#    avrg = 13
#    min = 13 * 0.5 = 6.5
# value reduced to 0.4 due to high chance of test failing
TEST_HASH_CHECK_BASE = 0.4
SEED_TEST_CHECK_BASE = 0.2


def test_header(test_name, module=None):
    """
    Prints test header

    Args:
        test_name (str): test case name
        module (str): module name
    """
    if module is None:
        print("\n\n**** TEST - %s ****\n" % (test_name))
    else:
        print("\n\n**** TEST - %s.%s ****\n" % (module, test_name))


def hash_to_hash_fields(hash_dict):
    """
    Creates hash fields list

    Args:
        hash_dict (dict): dictionary with variables that defines the list
                          of hash test fields and traffic header hashed fields

    Returns:
        list: hash fields list
    """
    hash_fields = []

    if ('hash_src_ip' in hash_dict) and \
            (hash_dict['hash_src_ip'] is not None):
        hash_fields.append(SAI_NATIVE_HASH_FIELD_SRC_IP)
    if ('hash_dst_ip' in hash_dict) and (hash_dict['hash_dst_ip'] is not None):
        hash_fields.append(SAI_NATIVE_HASH_FIELD_DST_IP)
    if ('hash_dst_mac' in hash_dict) and \
            (hash_dict['hash_dst_mac'] is not None):
        hash_fields.append(SAI_NATIVE_HASH_FIELD_DST_MAC)
    if ('hash_src_mac' in hash_dict) and \
            (hash_dict['hash_src_mac'] is not None):
        hash_fields.append(SAI_NATIVE_HASH_FIELD_SRC_MAC)
    if ('hash_udp_dport' in hash_dict) and \
            (hash_dict['hash_udp_dport'] is not None):
        hash_fields.append(SAI_NATIVE_HASH_FIELD_L4_DST_PORT)
    if ('hash_udp_sport' in hash_dict) and \
            (hash_dict['hash_udp_sport'] is not None):
        hash_fields.append(SAI_NATIVE_HASH_FIELD_L4_SRC_PORT)
    if ('hash_ether_type' in hash_dict) and \
            (hash_dict['hash_ether_type'] is not None):
        hash_fields.append(SAI_NATIVE_HASH_FIELD_ETHERTYPE)
    if ('hash_flow_label' in hash_dict) and \
            (hash_dict['hash_flow_label'] is not None):
        hash_fields.append(SAI_NATIVE_HASH_FIELD_IPV6_FLOW_LABEL)

    return hash_fields


def hash_fields_to_hash_names(hash_list):
    """
    Converts hash fields list to hash names list

    Args:
        hash_list (list): list of hash fields

    Returns:
        list: hash names list
    """
    hash_names = []
    for hash_from_list in hash_list:
        if hash_from_list == SAI_NATIVE_HASH_FIELD_L4_DST_PORT:
            hash_names.append("L4_DST_PORT")
        elif hash_from_list == SAI_NATIVE_HASH_FIELD_L4_SRC_PORT:
            hash_names.append("L4_SRC_PORT")
        elif hash_from_list == SAI_NATIVE_HASH_FIELD_SRC_IP:
            hash_names.append("SRC_IP")
        elif hash_from_list == SAI_NATIVE_HASH_FIELD_DST_IP:
            hash_names.append("DST_IP")
        elif hash_from_list == SAI_NATIVE_HASH_FIELD_DST_MAC:
            hash_names.append("DST_MAC")
        elif hash_from_list == SAI_NATIVE_HASH_FIELD_SRC_MAC:
            hash_names.append("SRC_MAC")
        elif hash_from_list == SAI_NATIVE_HASH_FIELD_ETHERTYPE:
            hash_names.append("ETHER_TYPE")
        else:
            hash_names.append("ALL")
    return hash_names


def verify_equaly_balanced(ecmp_count, pkt_count=MAX_ITRS,
                           expected_base=TEST_HASH_CHECK_BASE):
    """
    Verifies if ecmp paths are egualy balanced

    Args:
        ecmp_count (list): ecmp hashed port count list
        pkt_count (int): traffic number
        expected_base (int): percentage minimum accepted value of the port
                             count for the equally balanced traffic

    Returns:
        boolean: True if traffic considerd equally balanced,
                 False otherwise
    """
    base = ((pkt_count / len(ecmp_count)) * expected_base)
    for count in ecmp_count:
        if count < base:
            # "Ecmp paths are not equally balanced"
            print("verify_equaly_balanced: ecmp_count=",
                  ecmp_count, "count=", count,
                  "expected count min=", base)
            return False

    return True


def verify_similary_balanced(ecmp_count1, ecmp_count2):
    """
    Checks if two results of LB are similar

    Args:
        ecmp_count1 (list): ecmp count list for the first LB result
        ecmp_count2 (list): ecmp count list for the second LB result

    Returns:
        boolean: True if traffic considerd similary balanced,
                 False otherwise
    """

    if len(ecmp_count1) != len(ecmp_count2):
        # list must have equal len
        return False

    diff = 0
    for count, value in enumerate(ecmp_count1):
        print(count, value)
        diff += abs(ecmp_count1[count] - ecmp_count2[count])

    if diff == 0:
        # value 0 indicate the hashing did not change
        return False

    # value other then 0 means detected hasing change
    return True


def verify_lb_active_ports(lb_counts):
    """
    Counts how mamy ports were balanced

    Args:
        lb_counts (list): load balancing counts list

    Returns:
        int: balanced ports counter
    """
    cnt = 0
    for port_cnt in lb_counts:
        if port_cnt != 0:
            cnt += 1
    return cnt


def verify_no_lb(lb_counts, max_iters=MAX_ITRS):
    """
    Verifies if LB has no effect and all traffic
    is directed to single port only

    Args:
        lb_counts (list): load balancing counts list
        max_iters (int): maximum number of iterations

    Returns:
        boolean: True if load balancing has no effect,
                 False otherwise
    """
    cnt = 0
    for port_cnt in lb_counts:
        if port_cnt != 0:
            if port_cnt != max_iters:
                # should be 0 or max_iters only
                return False
            cnt += 1
        if cnt > 1:
            return False
    return True


def hash_dict_negation(hash_dict):
    """
    Negates the content of the input hash dictionary

    Args:
        hash_dict (dict): input dictionary

    Returns:
        dict: input dictionary negation
    """
    hash_dict_not = {}
    for key in hash_dict:
        if hash_dict[key] is not None:
            hash_dict_not[key] = None
        else:
            hash_dict_not[key] = True
    return hash_dict_not


@group("draft")
class SAIHashTestBase(SaiHelper):
    """
    Sets base configuration for tests
    """

    def setUp(self):

        super(SAIHashTestBase, self).setUp()

        self.ipv4_hash_id = 0
        self.ipv6_hash_id = 0
        self.lag_hash_ipv4 = 0
        self.lag_hash_ipv6 = 0

        # set default ECMP and LAG hash seeds
        # this has an effect only if hash is not reconfigured later
        self.setupECMPSeed(seed=TEST_ECMP_SEED)
        self.setupLagSeed(seed=TEST_LAG_SEED)

    def tearDown(self):
        try:
            if self.ipv6_hash_id != 0:
                sai_thrift_set_switch_attribute(
                    self.client, ecmp_hash_ipv6=0)
                sai_thrift_remove_hash(self.client, self.ipv6_hash_id)
                self.ipv6_hash_id = 0
            if self.ipv4_hash_id != 0:
                sai_thrift_set_switch_attribute(
                    self.client, ecmp_hash_ipv4=0)
                sai_thrift_remove_hash(self.client, self.ipv4_hash_id)
                self.ipv4_hash_id = 0
            if self.lag_hash_ipv4 != 0:
                sai_thrift_set_switch_attribute(
                    self.client, lag_hash_ipv4=0)
                sai_thrift_remove_hash(self.client, self.lag_hash_ipv4)
                self.lag_hash_ipv4 = 0
            if self.lag_hash_ipv6 != 0:
                sai_thrift_set_switch_attribute(
                    self.client, lag_hash_ipv6=0)
                sai_thrift_remove_hash(self.client, self.lag_hash_ipv6)
                self.lag_hash_ipv6 = 0

        finally:
            super(SAIHashTestBase, self).tearDown()

    def setupNonIPECMPHash(self, hash_fields_list=None):
        """
        Sets base configuration for the non IP ECMP case

        Args:
            hash_fields_list (list): hash fields list
        """
        attr_list = sai_thrift_get_switch_attribute(
            self.client, ecmp_hash=True, lag_hash=True)
        lag_hash_id = attr_list['SAI_SWITCH_ATTR_LAG_HASH']
        self.assertNotEqual(lag_hash_id, 0)
        if hash_fields_list is None:
            hash_fields_list = [SAI_NATIVE_HASH_FIELD_SRC_MAC,
                                SAI_NATIVE_HASH_FIELD_DST_MAC,
                                SAI_NATIVE_HASH_FIELD_ETHERTYPE]

        hash_attr_list = sai_thrift_s32_list_t(
            count=len(hash_fields_list),
            int32list=hash_fields_list)
        status = sai_thrift_set_hash_attribute(
            self.client,
            lag_hash_id,
            native_hash_field_list=hash_attr_list)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # verify if the hash_fields_list saved correctly
        self.assertEqual(
            True,
            self.verifyHashFieldsList(
                lag_hash_id,
                hash_fields_list))

    def setupFGECMPIPv4Hash(self, hash_fields_list=None, p_ipv4_mask=None):
        '''
        Sets base configuration for the FG ECMP IPv4 case

        Args:
            hash_fields_list (list): hash fields list
            p_ipv4_mask (string): ip mask to use

        Fine Grained ECMP IPV4 Hash configuraton is Three Step process.

        Step 1: Create object list for each hash field.
                [This step is create only, hence each
                 time we have to create with field name, mask
                 and sequence]

        Step 2: Creating the Hash object. This is both create/Set
                If not created before then we have to create a new
                hash object using the object list created in step 1.
                We can use set opertation if already created before.

        Step 3: Assigning a switch hash type with the above hash object id.

        '''
        if hash_fields_list is None:
            hash_fields_list = [SAI_NATIVE_HASH_FIELD_SRC_IP,
                                SAI_NATIVE_HASH_FIELD_DST_IP,
                                SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                                SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                                SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]

        attr_list = sai_thrift_get_switch_attribute(
            self.client, ecmp_hash_ipv4=True)
        ipv4_hash_id = attr_list['SAI_SWITCH_ATTR_ECMP_HASH_IPV4']

        order = 1
        hash_filed_id_list = []
        for hash_field in hash_fields_list:
            if((hash_field == SAI_NATIVE_HASH_FIELD_SRC_IP) or
               (hash_field == SAI_NATIVE_HASH_FIELD_DST_IP)):
                hash_field = sai_thrift_create_fine_grained_hash_field(
                    self.client,
                    native_hash_field=hash_field,
                    sequence_id=order,
                    ipv4_mask=p_ipv4_mask)
            else:
                hash_field = sai_thrift_create_fine_grained_hash_field(
                    self.client,
                    native_hash_field=hash_field,
                    sequence_id=order)

            hash_filed_id_list.append(hash_field)

        fg_hash_obj_list = sai_thrift_object_list_t(
            count=len(hash_filed_id_list),
            idlist=hash_filed_id_list)

        if ipv4_hash_id == 0:
            self.ipv4_hash_id = sai_thrift_create_hash(
                self.client, fine_grained_hash_field_list=fg_hash_obj_list)

            sai_thrift_set_switch_attribute(
                self.client,
                ecmp_hash_ipv4=self.ipv4_hash_id)
        else:
            # update existing hash
            status = sai_thrift_set_hash_attribute(
                self.client,
                ipv4_hash_id,
                fine_grained_hash_field_list=fg_hash_obj_list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def setupECMPIPv4Hash(self, hash_fields_list=None, seed=None):
        """
        Sets base configuration for the ECMP IPv4 case

        Args:
            hash_fields_list (list): hash fields list
            seed (int): hash seed value
        """
        if hash_fields_list is None:
            hash_fields_list = [SAI_NATIVE_HASH_FIELD_SRC_IP,
                                SAI_NATIVE_HASH_FIELD_DST_IP,
                                SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                                SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                                SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]

        attr_list = sai_thrift_get_switch_attribute(
            self.client, ecmp_hash_ipv4=True)
        ipv4_hash_id = attr_list['SAI_SWITCH_ATTR_ECMP_HASH_IPV4']

        if ipv4_hash_id == 0:
            # create new hash
            s32list = sai_thrift_s32_list_t(
                count=len(hash_fields_list),
                int32list=hash_fields_list)
            self.ipv4_hash_id = sai_thrift_create_hash(
                self.client, native_hash_field_list=s32list)
            self.assertTrue(
                self.ipv4_hash_id != 0,
                "Failed to create IPv4 hash")
            sai_thrift_set_switch_attribute(
                self.client,
                ecmp_hash_ipv4=self.ipv4_hash_id)
            # verify if the hash_fields_list saved correctly
            self.assertEqual(
                True,
                self.verifyHashFieldsList(
                    self.ipv4_hash_id,
                    hash_fields_list))
        else:
            # update existing hash
            s32list = sai_thrift_s32_list_t(
                count=len(hash_fields_list),
                int32list=hash_fields_list)
            status = sai_thrift_set_hash_attribute(
                self.client,
                ipv4_hash_id,
                native_hash_field_list=s32list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
        if seed is not None:
            self.setupECMPSeed(seed=seed)

    def setupECMPIPv6Hash(self, hash_fields_list=None, seed=None):
        """
        Sets base configuration for the ECMP IPv6 case

        Args:
            hash_fields_list (list): hash fields list
            seed (int): hash seed value
        """
        print("Setting the ECMP IPv6 hash fields")
        if hash_fields_list is None:
            hash_fields_list = [SAI_NATIVE_HASH_FIELD_SRC_IP,
                                SAI_NATIVE_HASH_FIELD_DST_IP,
                                SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                                SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                                SAI_NATIVE_HASH_FIELD_L4_SRC_PORT,
                                SAI_NATIVE_HASH_FIELD_IPV6_FLOW_LABEL]
        attr_list = sai_thrift_get_switch_attribute(
            self.client,
            ecmp_hash_ipv6=True,
            ecmp_hash_ipv4=True)
        ipv6_hash_id_old = attr_list['SAI_SWITCH_ATTR_ECMP_HASH_IPV6']

        if ipv6_hash_id_old == 0:
            # create new hash
            s32list = sai_thrift_s32_list_t(
                count=len(hash_fields_list),
                int32list=hash_fields_list)
            self.ipv6_hash_id = sai_thrift_create_hash(
                self.client, native_hash_field_list=s32list)
            self.assertTrue(
                self.ipv6_hash_id != 0,
                "Failed to create IPv6 hash")
            status = sai_thrift_set_switch_attribute(
                self.client,
                ecmp_hash_ipv6=self.ipv6_hash_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

            # verify if the hash_fields_list saved correctly
            self.assertEqual(
                True,
                self.verifyHashFieldsList(
                    self.ipv6_hash_id,
                    hash_fields_list))
        else:
            # update existing hash
            s32list = sai_thrift_s32_list_t(
                count=len(hash_fields_list),
                int32list=hash_fields_list)
            status = sai_thrift_set_hash_attribute(
                self.client,
                ipv6_hash_id_old,
                native_hash_field_list=s32list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

        if seed is not None:
            self.setupECMPSeed(seed=seed)

    def setupLAGIPv6Hash(self, hash_fields_list=None, seed=None):
        """
        Sets base configuration for the LAG IPv6 case

        Args:
            hash_fields_list (list): hash fields list
            seed (int): hash seed value
        """
        if hash_fields_list is None:
            hash_fields_list = [SAI_NATIVE_HASH_FIELD_SRC_IP,
                                SAI_NATIVE_HASH_FIELD_DST_IP,
                                SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                                SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                                SAI_NATIVE_HASH_FIELD_L4_SRC_PORT,
                                SAI_NATIVE_HASH_FIELD_IPV6_FLOW_LABEL]

        attr_list = sai_thrift_get_switch_attribute(
            self.client, lag_hash_ipv6=True)
        lag_hash_ipv6 = attr_list['SAI_SWITCH_ATTR_LAG_HASH_IPV6']

        if lag_hash_ipv6 == 0:
            # create new hash
            s32list = sai_thrift_s32_list_t(
                count=len(hash_fields_list),
                int32list=hash_fields_list)
            self.lag_hash_ipv6 = sai_thrift_create_hash(
                self.client, native_hash_field_list=s32list)
            self.assertTrue(
                self.lag_hash_ipv6 != 0,
                "Failed to create IPv6 lag hash")
            sai_thrift_set_switch_attribute(
                self.client,
                lag_hash_ipv6=self.lag_hash_ipv6)
            # verify if the hash_fields_list saved correctly
            print("setupLAGIPv6Hash=", hash_fields_list)
            self.assertEqual(
                True,
                self.verifyHashFieldsList(
                    self.lag_hash_ipv6,
                    hash_fields_list))
        else:
            # update existing hash
            s32list = sai_thrift_s32_list_t(
                count=len(hash_fields_list),
                int32list=hash_fields_list)
            status = sai_thrift_set_hash_attribute(
                self.client,
                lag_hash_ipv6,
                native_hash_field_list=s32list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

        if seed is not None:
            self.setupLagSeed(seed=seed)

    def setupLAGIPv4Hash(self, hash_fields_list=None, seed=None):
        """
        Sets base configuration for the LAG IPv4 case

        Args:
            hash_fields_list (list): hash fields list
            seed (int): hash seed value
        """
        if hash_fields_list is None:
            hash_fields_list = [SAI_NATIVE_HASH_FIELD_SRC_IP,
                                SAI_NATIVE_HASH_FIELD_DST_IP,
                                SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                                SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                                SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]

        attr_list = sai_thrift_get_switch_attribute(
            self.client, lag_hash_ipv4=True)
        lag_hash_ipv4 = attr_list['SAI_SWITCH_ATTR_LAG_HASH_IPV4']

        if lag_hash_ipv4 == 0:
            # create new hash
            s32list = sai_thrift_s32_list_t(
                count=len(hash_fields_list),
                int32list=hash_fields_list)
            self.lag_hash_ipv4 = sai_thrift_create_hash(
                self.client, native_hash_field_list=s32list)
            self.assertTrue(
                self.lag_hash_ipv4 != 0,
                "Failed to create IPv4 lag hash")
            sai_thrift_set_switch_attribute(
                self.client,
                lag_hash_ipv4=self.lag_hash_ipv4)
            # verify if the hash_fields_list saved correctly
            self.assertEqual(
                True,
                self.verifyHashFieldsList(
                    self.lag_hash_ipv4,
                    hash_fields_list))
        else:
            # update existing hash
            s32list = sai_thrift_s32_list_t(
                count=len(hash_fields_list),
                int32list=hash_fields_list)
            status = sai_thrift_set_hash_attribute(
                self.client,
                lag_hash_ipv4,
                native_hash_field_list=s32list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
        if seed is not None:
            self.setupLagSeed(seed=seed)

    def setupECMPSeed(self, seed=200):
        """
        Sets ECMP seed

        Args:
            seed (int): seed value
        """
        sai_thrift_set_switch_attribute(
            self.client, ecmp_default_hash_seed=seed)

    def setupLagSeed(self, seed=200):
        """
        Sets Lag seed

        Args:
            seed (int): seed value
        """
        sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_seed=seed)

    def verifyHashFieldsList(self, hash_id, fields_list):
        """
        Verifies hash fields list

        Args:
            hash_id (int): hash id value
            fields_list (list): hash fields list

        Returns:
            boolean: True if hw saved hash_id field list equals fields_list
                     False if field lists not equal
        """
        hash_fields_list = []
        s32list = sai_thrift_s32_list_t(count=100, int32list=hash_fields_list)
        hash_data = sai_thrift_get_hash_attribute(
            self.client,
            hash_id,
            native_hash_field_list=s32list)
        hash_fields_list = hash_data['native_hash_field_list'].int32list
        if len(hash_fields_list) != len(fields_list):
            return False

        # compare two hash field lists test_fields_list and hash_fields_list of
        # the given hash id
        for field1 in fields_list:
            found = False
            for field2 in hash_fields_list:
                if field1 == field2:
                    found = True
            if found is not True:
                return False

        return True

    def setupLagAlgorithm(self, algo=SAI_HASH_ALGORITHM_CRC):
        """
        Sets lag algorithm

        Args:
            algo (int): hash algorithm id
        """
        print("setup Lag Algorithm algorithm=%d" % algo)
        sai_thrift_set_switch_attribute(
            self.client, lag_default_hash_algorithm=algo)

    def setupECMPAlgorithm(self, algo=SAI_HASH_ALGORITHM_CRC):
        """
        Sets ECMP algorithm

        Args:
            algo (int): hash algorithm id
        """
        print("setup ECMP Algorithm algorithm=%d" % algo)
        sai_thrift_set_switch_attribute(
            self.client, ecmp_default_hash_algorithm=algo)


@group("draft")
class SAIHashTest(SAIHashTestBase):
    """
    Runs hash test cases
    """

    def setUp(self):

        super(SAIHashTest, self).setUp()

        dmac1 = '00:11:11:11:11:11'
        dmac2 = '00:22:22:22:22:22'
        dmac3 = '00:33:33:33:33:33'
        dmac4 = '00:44:44:44:44:44'

        dmac5 = '00:55:55:55:55:55'
        dmac6 = '00:66:66:66:66:66'
        dmac7 = '00:77:77:77:77:77'
        nhop_ip1 = '11.11.11.11'
        nhop_ip2 = '22.22.22.22'
        nhop_ip3 = '33.33.33.33'
        nhop_ip4 = '44.44.44.44'
        nhop_ip5 = '44.55.55.55'
        nhop_ip6 = '44.66.66.66'
        nhop_ip7 = '44.77.77.77'

        # set switch src mac address
        sai_thrift_set_switch_attribute(
            self.client,
            src_mac_address=ROUTER_MAC)
        self.lag1_rif = sai_thrift_create_router_interface(
            self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf, port_id=self.lag1,
            admin_v4_state=True)
        self.lag2_rif = sai_thrift_create_router_interface(
            self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf, port_id=self.lag2,
            admin_v4_state=True)
        self.port15_rif = sai_thrift_create_router_interface(
            self.client, type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf, port_id=self.port15,
            admin_v4_state=True)

        # create vlan 200 with port0 and lag1
        self.vlan200 = sai_thrift_create_vlan(self.client, vlan_id=200)
        self.assertNotEqual(self.vlan200, 0)
        self.vlan200_member1 = sai_thrift_create_vlan_member(
            self.client, vlan_id=self.vlan200, bridge_port_id=self.port0_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan200_member2 = sai_thrift_create_vlan_member(
            self.client, vlan_id=self.vlan200, bridge_port_id=self.lag1_bp,
            vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_port_attribute(
            self.client,
            self.port0,
            port_vlan_id=200)
        sai_thrift_set_lag_attribute(self.client, self.lag1, port_vlan_id=200)

        # test neighbor creation
        self.neighbor_entry10 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.port10_rif,
            sai_ipaddress(nhop_ip1))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry10,
            dst_mac_address=dmac1)
        self.neighbor_entry11 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.port11_rif,
            sai_ipaddress(nhop_ip2))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry11,
            dst_mac_address=dmac2)
        self.neighbor_entry12 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.port12_rif,
            sai_ipaddress(nhop_ip3))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry12,
            dst_mac_address=dmac3)
        self.neighbor_entry13 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.port13_rif,
            sai_ipaddress(nhop_ip4))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry13,
            dst_mac_address=dmac4)
        self.neighbor_entry15 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.lag1_rif,
            sai_ipaddress(nhop_ip5))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry15,
            dst_mac_address=dmac5)
        self.neighbor_entry16 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.lag2_rif,
            sai_ipaddress(nhop_ip6))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry16,
            dst_mac_address=dmac6)

        self.neighbor_entry17 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.lag1_rif,
            sai_ipaddress(nhop_ip7))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry17,
            dst_mac_address=dmac7)

        self.nhop1 = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port10_rif, ip=sai_ipaddress(nhop_ip1))
        self.nhop2 = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port11_rif, ip=sai_ipaddress(nhop_ip2))
        self.nhop3 = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port12_rif, ip=sai_ipaddress(nhop_ip3))
        self.nhop4 = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port13_rif, ip=sai_ipaddress(nhop_ip4))

        self.nhop3_lag1 = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag1_rif, ip=sai_ipaddress(nhop_ip7))

        self.nhop4_lag2 = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag2_rif, ip=sai_ipaddress(nhop_ip4))

        self.nhop5_lag1 = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag1_rif, ip=sai_ipaddress(nhop_ip5))
        self.nhop6_lag2 = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag2_rif, ip=sai_ipaddress(nhop_ip6))

        self.nhop_group1 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)

        self.nh_group1_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop1)
        self.nh_group1_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop2)
        self.nh_group1_member3 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop3)
        self.nh_group1_member4 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop4)

        self.route0 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('10.10.10.1/16'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client,
            self.route0,
            next_hop_id=self.nhop_group1)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.nhop_group2 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group2_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop5_lag1)
        self.nh_group2_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.nhop6_lag2)

        # create route entry for nhop_group2
        self.route1 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('20.20.20.1/16'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client,
            self.route1,
            next_hop_id=self.nhop_group2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # create route entry for nhop_group3 Lag1 only
        self.nhop_group3 = sai_thrift_create_next_hop_group(
            self.client,
            type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group3_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group3,
            next_hop_id=self.nhop3_lag1)

        self.route2 = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            destination=sai_ipprefix('10.70.70.1/16'),
            vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client,
            self.route2,
            next_hop_id=self.nhop_group3)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def tearDown(self):
        try:
            # retrieve original settings
            sai_thrift_flush_fdb_entries(
                self.client,
                entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            sai_thrift_set_port_attribute(
                self.client,
                self.port0,
                port_vlan_id=0)
            sai_thrift_set_lag_attribute(
                self.client,
                self.lag1,
                port_vlan_id=0)

            sai_thrift_remove_neighbor_entry(
                self.client,
                self.neighbor_entry10)
            sai_thrift_remove_neighbor_entry(
                self.client,
                self.neighbor_entry11)
            sai_thrift_remove_neighbor_entry(
                self.client,
                self.neighbor_entry12)
            sai_thrift_remove_neighbor_entry(
                self.client,
                self.neighbor_entry13)
            sai_thrift_remove_neighbor_entry(
                self.client,
                self.neighbor_entry15)
            sai_thrift_remove_neighbor_entry(
                self.client,
                self.neighbor_entry16)
            sai_thrift_remove_neighbor_entry(
                self.client,
                self.neighbor_entry17)

            sai_thrift_remove_next_hop_group_member(
                self.client, self.nh_group1_member1)
            sai_thrift_remove_next_hop_group_member(
                self.client, self.nh_group1_member2)
            sai_thrift_remove_next_hop_group_member(
                self.client, self.nh_group1_member3)
            sai_thrift_remove_next_hop_group_member(
                self.client, self.nh_group1_member4)

            sai_thrift_remove_next_hop_group_member(
                self.client, self.nh_group2_member1)
            sai_thrift_remove_next_hop_group_member(
                self.client, self.nh_group2_member2)

            sai_thrift_remove_next_hop_group_member(
                self.client, self.nh_group3_member1)

            sai_thrift_remove_route_entry(self.client, self.route0)
            sai_thrift_remove_route_entry(self.client, self.route1)
            sai_thrift_remove_route_entry(self.client, self.route2)

            sai_thrift_remove_next_hop_group(self.client, self.nhop_group1)
            sai_thrift_remove_next_hop_group(self.client, self.nhop_group2)
            sai_thrift_remove_next_hop_group(self.client, self.nhop_group3)

            sai_thrift_remove_next_hop(self.client, self.nhop1)
            sai_thrift_remove_next_hop(self.client, self.nhop2)
            sai_thrift_remove_next_hop(self.client, self.nhop3)
            sai_thrift_remove_next_hop(self.client, self.nhop4)

            sai_thrift_remove_next_hop(self.client, self.nhop3_lag1)
            sai_thrift_remove_next_hop(self.client, self.nhop4_lag2)
            sai_thrift_remove_next_hop(self.client, self.nhop5_lag1)
            sai_thrift_remove_next_hop(self.client, self.nhop6_lag2)

            sai_thrift_remove_vlan_member(self.client, self.vlan200_member1)
            sai_thrift_remove_vlan_member(self.client, self.vlan200_member2)

            sai_thrift_remove_vlan(self.client, self.vlan200)

            sai_thrift_remove_router_interface(self.client, self.lag1_rif)
            sai_thrift_remove_router_interface(self.client, self.lag2_rif)
            sai_thrift_remove_router_interface(self.client, self.port15_rif)

        finally:
            super(SAIHashTest, self).tearDown()

    def l3IPv4LagPacketTest(self, hash_dict, traffic=True, max_itrs=MAX_ITRS):
        """
        Function that performs the IPv4 LAG hash test with L3 hashed traffic

        Args:
            hash_dict (dict): dictionary with variables that defines the list
            of hash test fields and traffic header hashed fields
            traffic (boolean): informs if traffic is expected on egress ports
            max_itrs (int): maximum number of iterations

        Returns:
            list: list of numbers of packet egressed on specific test member
        """
        count = [0, 0, 0]
        src_mac_start = '00:22:22:22:{0}:{1}'
        dst_mac_start = '00:99:99:99:{0}:{1}'
        dst_mac = '00:99:99:99:99:99'
        src_mac = '00:22:22:22:22:22'
        dst_ip = int(binascii.hexlify(socket.inet_aton('10.70.70.1')), 16)
        src_ip = int(binascii.hexlify(socket.inet_aton('192.168.8.1')), 16)
        udp_sport = 7
        udp_dport = 7
        for i in range(0, max_itrs):
            dst_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
            src_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(hex(src_ip)[2:].zfill(8)))
            if ('hash_dst_ip' in hash_dict) and (hash_dict['hash_dst_ip']):
                dst_ip += i * 7
            if ('hash_src_ip' in hash_dict) and (hash_dict['hash_src_ip']):
                src_ip += i * 17
            if ('hash_src_mac' in hash_dict) and (hash_dict['hash_src_mac']):
                src_mac = src_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
            if ('hash_dst_mac' in hash_dict) and (hash_dict['hash_dst_mac']):
                dst_mac = dst_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
            if ('hash_udp_sport' in hash_dict) and \
                    (hash_dict['hash_udp_sport']):
                udp_sport += 13
            if ('hash_udp_dport' in hash_dict) and \
                    (hash_dict['hash_udp_dport']):
                udp_dport += 13

            pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip_addr,
                                    ip_src=src_ip_addr,
                                    udp_sport=udp_sport,
                                    udp_dport=udp_dport,
                                    ip_id=106,
                                    ip_ttl=64)
            exp_pkt = simple_udp_packet(eth_dst='00:77:77:77:77:77',
                                        eth_src=ROUTER_MAC,
                                        ip_dst=dst_ip_addr,
                                        ip_src=src_ip_addr,
                                        udp_sport=udp_sport,
                                        udp_dport=udp_dport,
                                        ip_id=106,
                                        ip_ttl=63)

            send_packet(self, self.dev_port4, pkt)
            if traffic:
                exp_ports = [self.dev_port4, self.dev_port5, self.dev_port6]
                rcv_idx = verify_any_packet_any_port(
                    self, [exp_pkt], exp_ports)
                count[rcv_idx] += 1
                if DEBUG:
                    print("idx:", rcv_idx, "dst_ip:", dst_ip_addr, " src_ip:",
                          src_ip_addr, " smac:", src_mac, " dmac: ", dst_mac,
                          " dport", udp_dport, " sport", udp_sport)

            else:
                verify_no_other_packets(self)

        return count

    def l3IPv4EcmpPacketTest(self, hash_dict, traffic=True, max_itrs=MAX_ITRS):
        """
        Function that performs the IPv4 ECMP test with L3 hashed traffic

        Args:
            hash_dict (dict): dictionary with variables that defines the list
            of hash test fields and traffic header hashed fields
            traffic (boolean): informs if traffic is expected on egress ports
            max_itrs (int): maximum number of iterations

        Returns:
            list: list of numbers of packet egressed on specific test port
        """
        count = [0, 0, 0, 0]
        src_mac_start = '00:22:22:22:{0}:{1}'
        dst_mac_start = '00:99:99:99:{0}:{1}'
        dst_mac = ROUTER_MAC
        src_mac = '00:22:22:22:22:22'
        dst_ip = int(binascii.hexlify(socket.inet_aton('10.10.10.1')), 16)
        src_ip = int(binascii.hexlify(socket.inet_aton('192.168.8.1')), 16)
        udp_sport = 7
        udp_dport = 7
        for i in range(0, max_itrs):
            dst_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(hex(dst_ip)[2:].zfill(8)))
            src_ip_addr = socket.inet_ntoa(
                binascii.unhexlify(hex(src_ip)[2:].zfill(8)))
            if ('hash_dst_ip' in hash_dict) and (hash_dict['hash_dst_ip']):
                dst_ip += i * 7
            if ('hash_src_ip' in hash_dict) and (hash_dict['hash_src_ip']):
                src_ip += i * 7
            if ('hash_src_mac' in hash_dict) and (hash_dict['hash_src_mac']):
                src_mac = src_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
            if ('hash_dst_mac' in hash_dict) and (hash_dict['hash_dst_mac']):
                dst_mac = dst_mac_start.format(
                    str(i).zfill(4)[:2],
                    str(i).zfill(4)[2:])
            if ('hash_udp_sport' in hash_dict) and \
                    (hash_dict['hash_udp_sport']):
                udp_sport += 13
            if ('hash_udp_dport' in hash_dict) and \
                    (hash_dict['hash_udp_dport']):
                udp_dport += 13

            pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                    eth_src=src_mac,
                                    ip_dst=dst_ip_addr,
                                    ip_src=src_ip_addr,
                                    udp_sport=udp_sport,
                                    udp_dport=udp_dport,
                                    ip_id=106,
                                    ip_ttl=64)

            exp_pkt1 = simple_udp_packet(eth_dst='00:11:11:11:11:11',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src=src_ip_addr,
                                         udp_sport=udp_sport,
                                         udp_dport=udp_dport,
                                         ip_id=106,
                                         ip_ttl=63)

            exp_pkt2 = simple_udp_packet(eth_dst='00:22:22:22:22:22',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src=src_ip_addr,
                                         udp_sport=udp_sport,
                                         udp_dport=udp_dport,
                                         ip_id=106,
                                         ip_ttl=63)

            exp_pkt3 = simple_udp_packet(eth_dst='00:33:33:33:33:33',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src=src_ip_addr,
                                         udp_sport=udp_sport,
                                         udp_dport=udp_dport,
                                         ip_id=106,
                                         ip_ttl=63)

            exp_pkt4 = simple_udp_packet(eth_dst='00:44:44:44:44:44',
                                         eth_src=ROUTER_MAC,
                                         ip_dst=dst_ip_addr,
                                         ip_src=src_ip_addr,
                                         udp_sport=udp_sport,
                                         udp_dport=udp_dport,
                                         ip_id=106,
                                         ip_ttl=63)

            send_packet(self, self.dev_port15, pkt)
            if traffic:
                ports_to_verify = [
                    self.dev_port10,
                    self.dev_port11,
                    self.dev_port12,
                    self.dev_port13]
                rcv_idx = verify_any_packet_any_port(self,
                                                     [exp_pkt1,
                                                      exp_pkt2,
                                                      exp_pkt3,
                                                      exp_pkt4],
                                                     ports_to_verify)
                count[rcv_idx] += 1
                if DEBUG:
                    print("idx:", rcv_idx, "dst_ip:", dst_ip_addr, " src_ip:",
                          src_ip_addr, " smac:", src_mac, " dmac: ", dst_mac,
                          " dport", udp_dport, " sport", udp_sport)
            else:
                verify_no_other_packets(self)

        return count

    def l2hashTraffic(self,
                      hash_dst_mac=False,
                      hash_src_mac=False,
                      hash_ether_type=False):
        """
        Function that performs the LAG hash test with L2 hashed traffic

        Args:
            hash_dst_mac (boolean): indicates if test uses dst mac hashed
                                    traffic
            hash_src_mac (boolean): indicates if test uses src mac hashed
                                    traffic
            hash_ether_type (boolean): indicates if test uses ether type hashed
                                       traffic

        Returns:
            list: list of number of packet egressed on specific test port
        """

        src_mac_start = '00:11:11:11:11:'
        dst_mac_start = '00:22:22:22:22:'
        src_mac = '00:11:11:11:11:11'
        dst_mac = '00:22:22:22:22:00'
        eth_type = 0x88cc
        pkt = simple_eth_packet(eth_dst=dst_mac,
                                eth_src=src_mac,
                                eth_type=eth_type,
                                pktlen=100)
        fdb_entries = []
        try:
            for i in range(0, MAX_ITRS):
                dst_mac = dst_mac_start + hex(i)[2:]
                fdb_entry = sai_thrift_fdb_entry_t(
                    switch_id=self.switch_id,
                    mac_address=dst_mac,
                    bv_id=self.vlan200)
                fdb_entries.append(fdb_entry)
                sai_thrift_create_fdb_entry(
                    self.client,
                    fdb_entry,
                    type=SAI_FDB_ENTRY_TYPE_STATIC,
                    bridge_port_id=self.lag1_bp)

            count = [0, 0, 0]
            for i in range(0, MAX_ITRS):
                if hash_src_mac:
                    pkt['Ethernet'].src = src_mac_start + hex(i)[2:]
                if hash_dst_mac:
                    dst_mac = dst_mac_start + hex(i)[2:]
                    pkt['Ethernet'].dst = dst_mac
                if hash_ether_type:
                    pkt['Ethernet'].type = eth_type + i

                send_packet(self, self.dev_port0, pkt)
                ports_to_verify = [
                    self.dev_port4,
                    self.dev_port5,
                    self.dev_port6]
                rcv_idx = verify_any_packet_any_port(
                    self, [pkt, pkt, pkt], ports_to_verify)
                count[rcv_idx] += 1

            return count

        finally:
            for fdb_entry in fdb_entries:
                sai_thrift_remove_fdb_entry(self.client, fdb_entry)

    def ecmpIPv4HashSaveRestoreTest(self):
        """
        Verfies modification of the switch ECMP IPv4 hash fields
        """
        test_header("ecmpIPv4HashSaveRestoreTest")

        hash_fields_list = [SAI_NATIVE_HASH_FIELD_SRC_IP,
                            SAI_NATIVE_HASH_FIELD_DST_IP,
                            SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                            SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                            SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]

        attr_list = sai_thrift_get_switch_attribute(
            self.client, ecmp_hash_ipv4=True)
        ipv4_hash_id = attr_list['SAI_SWITCH_ATTR_ECMP_HASH_IPV4']
        if ipv4_hash_id == 0:
            if self.ipv4_hash_id != 0:
                ipv4_hash_id = self.ipv4_hash_id
            else:
                s32list = sai_thrift_s32_list_t(
                    count=len(hash_fields_list),
                    int32list=hash_fields_list)
                ipv4_hash_id = sai_thrift_create_hash(
                    self.client,
                    native_hash_field_list=s32list)
                self.assertTrue(
                    ipv4_hash_id != 0,
                    "Failed to create IPv4 hash")
                status = sai_thrift_set_switch_attribute(
                    self.client, ecmp_hash_ipv4=ipv4_hash_id)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
                attr_list = sai_thrift_get_switch_attribute(
                    self.client, ecmp_hash_ipv4=True)
                ipv4_hash_id = attr_list['SAI_SWITCH_ATTR_ECMP_HASH_IPV4']
                self.assertTrue(
                    ipv4_hash_id != 0,
                    "failed to get the switch IPv4 ecmp hash")
                self.ipv4_hash_id = ipv4_hash_id

        s32list = sai_thrift_s32_list_t(
            count=len(hash_fields_list),
            int32list=hash_fields_list)
        status = sai_thrift_set_hash_attribute(
            self.client,
            ipv4_hash_id,
            native_hash_field_list=s32list)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # vefify the native hash fields list
        self.assertEqual(
            True,
            self.verifyHashFieldsList(
                ipv4_hash_id,
                hash_fields_list))

        # verify new hash create
        test_fields_list = [[SAI_NATIVE_HASH_FIELD_SRC_IP],
                            [SAI_NATIVE_HASH_FIELD_DST_IP],
                            [SAI_NATIVE_HASH_FIELD_SRC_IP,
                             SAI_NATIVE_HASH_FIELD_DST_IP],
                            [SAI_NATIVE_HASH_FIELD_IP_PROTOCOL],
                            [SAI_NATIVE_HASH_FIELD_SRC_IP,
                             SAI_NATIVE_HASH_FIELD_DST_IP,
                             SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                             SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                             SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]]

        for field_list in test_fields_list:
            s32list = sai_thrift_s32_list_t(
                count=len(field_list),
                int32list=field_list)
            new_ipv4_hash_id = sai_thrift_create_hash(
                self.client,
                native_hash_field_list=s32list)
            self.assertTrue(
                new_ipv4_hash_id != 0,
                "Failed to create IPv4 hash")
            self.assertEqual(
                True,
                self.verifyHashFieldsList(
                    new_ipv4_hash_id,
                    field_list))
            status = sai_thrift_remove_hash(self.client, new_ipv4_hash_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def lagIPv6HashSaveRestoreTest(self):
        """
        Verfies modification of the switch lag IPv6 hash fields
        """
        test_header("lagIPv6HashSaveRestoreTest")

        hash_fields_list = [
            SAI_NATIVE_HASH_FIELD_SRC_IP,
            SAI_NATIVE_HASH_FIELD_DST_IP,
            SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
            SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
            SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]

        attr_list = sai_thrift_get_switch_attribute(
            self.client, lag_hash_ipv6=True)
        lag_hash_ipv6 = attr_list['SAI_SWITCH_ATTR_LAG_HASH_IPV6']
        if lag_hash_ipv6 == 0:
            if self.lag_hash_ipv6 != 0:
                lag_hash_ipv6 = self.lag_hash_ipv6
            else:
                s32list = sai_thrift_s32_list_t(
                    count=len(hash_fields_list),
                    int32list=hash_fields_list)
                lag_hash_ipv6 = sai_thrift_create_hash(
                    self.client,
                    native_hash_field_list=s32list)
                self.assertTrue(
                    lag_hash_ipv6 != 0,
                    "Failed to create IPv6 hash")
                status = sai_thrift_set_switch_attribute(
                    self.client, lag_hash_ipv6=lag_hash_ipv6)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
                sai_thrift_get_switch_attribute(
                    self.client, lag_hash_ipv6=True)
                self.assertTrue(
                    lag_hash_ipv6 != 0,
                    "failed to restore the switch IPv6 LAG hash")
                self.lag_hash_ipv6 = lag_hash_ipv6

        s32list = sai_thrift_s32_list_t(
            count=len(hash_fields_list),
            int32list=hash_fields_list)
        status = sai_thrift_set_hash_attribute(
            self.client,
            lag_hash_ipv6,
            native_hash_field_list=s32list)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # vefify the native hash fields list
        self.assertEqual(
            True,
            self.verifyHashFieldsList(
                lag_hash_ipv6,
                hash_fields_list))

        # verify new hash create
        test_fields_list = [[SAI_NATIVE_HASH_FIELD_SRC_IP],
                            [SAI_NATIVE_HASH_FIELD_DST_IP],
                            [SAI_NATIVE_HASH_FIELD_SRC_IP,
                             SAI_NATIVE_HASH_FIELD_DST_IP],
                            [SAI_NATIVE_HASH_FIELD_IP_PROTOCOL],
                            [SAI_NATIVE_HASH_FIELD_SRC_IP,
                             SAI_NATIVE_HASH_FIELD_DST_IP,
                             SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                             SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                             SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]]

        for field_list in test_fields_list:
            s32list = sai_thrift_s32_list_t(
                count=len(field_list),
                int32list=field_list)
            status = sai_thrift_set_hash_attribute(
                self.client,
                lag_hash_ipv6,
                native_hash_field_list=s32list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            self.assertEqual(
                True,
                self.verifyHashFieldsList(
                    lag_hash_ipv6,
                    field_list))

    def lagIPv4HashSaveRestoreTest(self):
        """
        Verfies modification of the switch lag IPv4 hash fields
        """
        test_header("lagIPv4HashSaveRestoreTest")

        hash_fields_list = [
            SAI_NATIVE_HASH_FIELD_SRC_IP,
            SAI_NATIVE_HASH_FIELD_DST_IP,
            SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
            SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
            SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]

        attr_list = sai_thrift_get_switch_attribute(
            self.client, lag_hash_ipv4=True)
        lag_hash_ipv4 = attr_list['SAI_SWITCH_ATTR_LAG_HASH_IPV4']
        if lag_hash_ipv4 == 0:
            if self.lag_hash_ipv4 != 0:
                lag_hash_ipv4 = self.lag_hash_ipv4
            else:
                s32list = sai_thrift_s32_list_t(
                    count=len(hash_fields_list),
                    int32list=hash_fields_list)
                lag_hash_ipv4 = sai_thrift_create_hash(
                    self.client,
                    native_hash_field_list=s32list)
                self.assertTrue(
                    lag_hash_ipv4 != 0,
                    "Failed to create IPv4 hash")
                status = sai_thrift_set_switch_attribute(
                    self.client, lag_hash_ipv4=lag_hash_ipv4)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
                attr_list = sai_thrift_get_switch_attribute(
                    self.client, lag_hash_ipv4=True)
                lag_hash_ipv4 = attr_list['SAI_SWITCH_ATTR_LAG_HASH_IPV4']
                self.assertTrue(
                    lag_hash_ipv4 != 0,
                    "failed to restore the switch IPv4 LAG hash")
                self.lag_hash_ipv4 = lag_hash_ipv4

        s32list = sai_thrift_s32_list_t(
            count=len(hash_fields_list),
            int32list=hash_fields_list)
        status = sai_thrift_set_hash_attribute(
            self.client,
            lag_hash_ipv4,
            native_hash_field_list=s32list)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # vefify the native hash fields list
        self.assertEqual(
            True,
            self.verifyHashFieldsList(
                lag_hash_ipv4,
                hash_fields_list))

        # verify new hash create
        test_fields_list = [[SAI_NATIVE_HASH_FIELD_SRC_IP],
                            [SAI_NATIVE_HASH_FIELD_DST_IP],
                            [SAI_NATIVE_HASH_FIELD_SRC_IP,
                             SAI_NATIVE_HASH_FIELD_DST_IP],
                            [SAI_NATIVE_HASH_FIELD_IP_PROTOCOL],
                            [SAI_NATIVE_HASH_FIELD_SRC_IP,
                             SAI_NATIVE_HASH_FIELD_DST_IP,
                             SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                             SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                             SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]]

        for field_list in test_fields_list:
            s32list = sai_thrift_s32_list_t(
                count=len(field_list),
                int32list=field_list)
            status = sai_thrift_set_hash_attribute(
                self.client,
                lag_hash_ipv4,
                native_hash_field_list=s32list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            self.assertEqual(
                True,
                self.verifyHashFieldsList(
                    lag_hash_ipv4,
                    field_list))

    def lagHashSaveRestoreTest(self):
        """
        Verfies modification of the switch lag hash fields
        """
        test_header("lagHashSaveRestoreTest")

        hash_fields_list = [
            SAI_NATIVE_HASH_FIELD_SRC_MAC,
            SAI_NATIVE_HASH_FIELD_DST_MAC,
            SAI_NATIVE_HASH_FIELD_ETHERTYPE,
            SAI_NATIVE_HASH_FIELD_SRC_IP,
            SAI_NATIVE_HASH_FIELD_DST_IP,
            SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
            SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
            SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]

        attr_list = sai_thrift_get_switch_attribute(
            self.client,
            lag_hash=True)
        lag_hash_id = attr_list['SAI_SWITCH_ATTR_LAG_HASH']
        self.assertTrue(
            lag_hash_id != 0,
            "switch lag hash not configured correctly")

        s32list = sai_thrift_s32_list_t(
            count=len(hash_fields_list),
            int32list=hash_fields_list)
        status = sai_thrift_set_hash_attribute(
            self.client,
            lag_hash_id,
            native_hash_field_list=s32list)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # vefify the native hash fields list
        self.assertEqual(
            True,
            self.verifyHashFieldsList(
                lag_hash_id,
                hash_fields_list))

        # verify new hash create
        test_fields_list = [[SAI_NATIVE_HASH_FIELD_SRC_MAC],
                            [SAI_NATIVE_HASH_FIELD_SRC_MAC,
                             SAI_NATIVE_HASH_FIELD_DST_MAC],
                            [SAI_NATIVE_HASH_FIELD_SRC_MAC,
                             SAI_NATIVE_HASH_FIELD_DST_MAC,
                             SAI_NATIVE_HASH_FIELD_ETHERTYPE],
                            [SAI_NATIVE_HASH_FIELD_ETHERTYPE]]

        for field_list in test_fields_list:
            s32list = sai_thrift_s32_list_t(
                count=len(field_list),
                int32list=field_list)
            status = sai_thrift_set_hash_attribute(
                self.client,
                lag_hash_id,
                native_hash_field_list=s32list)
            self.assertEqual(status, SAI_STATUS_SUCCESS)
            self.assertEqual(
                True,
                self.verifyHashFieldsList(
                    lag_hash_id,
                    field_list))

    def ecmpIPv4BasicHashNoLBTest(self):
        """
        Verfies ECMP IPv4 hash defined for single not modified IPv4 hash field
        with traffic and no load balancing
        """
        test_header("ecmpIPv4BasicHashNoLBTest")

        hash_dict = {'hash_src_ip': True, 'hash_dst_ip': True,
                     'hash_src_mac': True}

        self.setupECMPIPv4Hash([SAI_NATIVE_HASH_FIELD_DST_MAC])
        print("Verify no load balancing when packet fields changes"
              "do not match hash fields.")
        # should NOT ballance
        ecmp_count = self.l3IPv4EcmpPacketTest(hash_dict)
        print("ECMP count:", ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertEqual(nbr_active_ports, 1)

    def fgEcmpIPv4HashTest(self,
                           hash_src_ip=None,
                           hash_dst_ip=None,
                           hash_udp_sport=None,
                           hash_udp_dport=None):
        """
        Verifies traffic distribution using all the fields selected
        for ECMP IPv4 Hash

        Args:
            hash_src_ip (boolean): indicates if test uses src ip hashed traffic
            hash_dst_ip (boolean): indicates if test uses dst ip hashed traffic
            hash_udp_sport (boolean): indicates if test uses udp sport hashed
                                      traffic
            hash_udp_dport (boolean): indicates if test uses udp dport hashed
                                      traffic
        """

        hash_dict = {'hash_src_ip': hash_src_ip, 'hash_dst_ip': hash_dst_ip,
                     'hash_udp_dport': hash_udp_dport,
                     'hash_udp_sport': hash_udp_sport}

        hash_fields = hash_to_hash_fields(hash_dict)
        print("Verify IPv4 ECMP load balancing for hash fields: %s"
              % (hash_fields_to_hash_names(hash_fields)))

        # setup IPv4 hash fields for all fields
        self.setupFGECMPIPv4Hash(hash_fields)

        # should ballance equally
        ecmp_count = self.l3IPv4EcmpPacketTest(hash_dict)
        print("ECMP LB count:", ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertEqual(nbr_active_ports, 4)

        equaly_balanced = verify_equaly_balanced(ecmp_count)
        self.assertTrue(equaly_balanced,
                        "Ecmp paths are not equally balanced")

        ecmp_count = self.l3IPv4EcmpPacketTest(hash_dict_negation(
            hash_dict))
        print("ECMP LB count (LB NOT expected):", ecmp_count)
        no_lb = verify_no_lb(ecmp_count, max_iters=MAX_ITRS)
        self.assertTrue(no_lb, "Not expected to balance")

        # Few bits of IP is masked
        self.setupFGECMPIPv4Hash(hash_fields, p_ipv4_mask='255.255.255.0')
        ecmp_count = self.l3IPv4EcmpPacketTest(hash_dict)
        print("ECMP LB count:", ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertEqual(nbr_active_ports, 4)

        equaly_balanced = verify_equaly_balanced(ecmp_count)
        self.assertTrue(equaly_balanced,
                        "Ecmp paths are not equally balanced")

    def ecmpIPv4HashTest(self,
                         hash_src_ip=None,
                         hash_dst_ip=None,
                         hash_udp_sport=None,
                         hash_udp_dport=None):
        """
        Verifies traffic distribution using all the fields selected
        for ECMP IPv4 Hash

        Args:
            hash_src_ip (boolean): indicates if test uses src ip hashed traffic
            hash_dst_ip (boolean): indicates if test uses dst ip hashed traffic
            hash_udp_sport (boolean): indicates if test uses udp sport hashed
                                      traffic
            hash_udp_dport (boolean): indicates if test uses udp dport hashed
                                      traffic
        """

        hash_dict = {'hash_src_ip': hash_src_ip, 'hash_dst_ip': hash_dst_ip,
                     'hash_udp_dport': hash_udp_dport,
                     'hash_udp_sport': hash_udp_sport}

        hash_fields = hash_to_hash_fields(hash_dict)
        print("Verify IPv4 ECMP load balancing for hash fields: %s"
              % (hash_fields_to_hash_names(hash_fields)))

        # setup IPv4 hash fields for all fields
        self.setupECMPIPv4Hash(hash_fields)

        # should ballance equally
        ecmp_count = self.l3IPv4EcmpPacketTest(hash_dict)
        print("ECMP LB count:", ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertEqual(nbr_active_ports, 4)

        equaly_balanced = verify_equaly_balanced(ecmp_count)
        self.assertTrue(equaly_balanced,
                        "Ecmp paths are not equally balanced")
        ecmp_count = self.l3IPv4EcmpPacketTest(hash_dict_negation(
            hash_dict))
        print("ECMP LB count (LB NOT expected):", ecmp_count)
        no_lb = verify_no_lb(ecmp_count, max_iters=MAX_ITRS)
        self.assertTrue(no_lb, "Not expected to balance")

    def ecmpIPv4vsIPv6HashTest(self):
        """
        Verifies IPv6 hash does not impact IPv4 hashing
        """
        test_header("ecmpIPv4vsIPv6HashTest")

        hash_dict_src = {'hash_src_ip': True}
        hash_dict_dst = {'hash_dst_ip': True}

        self.setupECMPIPv4Hash([SAI_NATIVE_HASH_FIELD_SRC_IP])
        # verify if IPv6 hash affects IPv4 hash
        self.setupECMPIPv6Hash([SAI_NATIVE_HASH_FIELD_DST_IP])
        print("Verify load balancing equaly for all next_hops when"
              "packet fields changes and do match hash fields.")

        # IPV4 should ballance equally independently of IPv6 hash
        ecmp_count = self.l3IPv4EcmpPacketTest(hash_dict_src)
        print("ECMP count:", ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertEqual(nbr_active_ports, 4)

        equaly_balanced = verify_equaly_balanced(ecmp_count)
        self.assertTrue(equaly_balanced,
                        "Ecmp paths are not equally balanced")

        # IPv4 should not ballance
        ecmp_count = self.l3IPv4EcmpPacketTest(hash_dict_dst)
        print("ECMP count:", ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertTrue(nbr_active_ports == 1, "LB not expected")

    def l3EcmpIPv4HashSeedTest(self):
        """
        Verifies IPv4 ECMP with varing seed values
        """
        test_header("l3EcmpIPv4HashSeedTest")

        hash_dict = {'hash_src_ip': True, 'hash_dst_ip': True,
                     'hash_udp_dport': True, 'hash_udp_sport': True}

        hash_fields = hash_to_hash_fields(hash_dict)
        self.setupECMPIPv4Hash(hash_fields, seed=TEST_ECMP_SEED)

        print("Verify load balancing equaly for all next_hops when"
              "all packets fields changes")
        ecmp_count = self.l3IPv4EcmpPacketTest(hash_dict)
        print("ECMP count seed=%d:" % (TEST_ECMP_SEED), ecmp_count)
        equaly_balanced = verify_equaly_balanced(
            ecmp_count, expected_base=SEED_TEST_CHECK_BASE)
        self.assertTrue(equaly_balanced,
                        "Ecmp paths are not equally balanced with seed=17")
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertEqual(nbr_active_ports, 4)

        # set the ecmp hash seed
        self.setupECMPSeed(seed=TEST_ECMP_SEED1)
        ecmp_count_seed1 = self.l3IPv4EcmpPacketTest(hash_dict)
        print("ECMP count seed=%d:" % (TEST_ECMP_SEED1), ecmp_count_seed1)
        equaly_balanced = verify_equaly_balanced(
            ecmp_count_seed1, expected_base=SEED_TEST_CHECK_BASE)
        self.assertTrue(equaly_balanced,
                        "Ecmp paths are not equally balanced with seed=%d"
                        % (TEST_ECMP_SEED))
        nbr_active_ports = verify_lb_active_ports(ecmp_count_seed1)
        self.assertEqual(nbr_active_ports, 4)

        # vefiry if LB factor changed
        similary_balanced = verify_similary_balanced(
            ecmp_count, ecmp_count_seed1)
        self.assertTrue(similary_balanced,
                        "ECMP Seed change has no effect on LB")

    def ecmpHashAlgorithmTest(self):
        """
        Verifies ECMP hash algorithm
        """
        test_header("ecmpHashAlgorithmTest")

        hash_dict = {'hash_src_ip': True}

        try:
            # configure ECMP hash for all fields
            self.setupECMPIPv4Hash()
            print("Verify L3 load balancing for ECMP members"
                  "for all fields hashing")

            self.setupECMPSeed(seed=0)
            self.setupECMPAlgorithm()
            hash_attr_get = sai_thrift_get_switch_attribute(
                self.client, ecmp_default_hash_algorithm=True)
            self.assertEqual(hash_attr_get[
                'SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM'],
                             SAI_HASH_ALGORITHM_CRC)
            print('Read value of algorithm: ', hash_attr_get)

            # should ballance equally
            ecmp_count = self.l3IPv4EcmpPacketTest(hash_dict,
                                                   max_itrs=L3_MAX_ITRS)
            print("\nECMP count: %s \n" % (ecmp_count))
            nbr_active_ports = verify_lb_active_ports(ecmp_count)
            self.assertEqual(nbr_active_ports, 4)

            equaly_balanced = verify_equaly_balanced(
                ecmp_count, pkt_count=L3_MAX_ITRS,
                expected_base=0.6)
            self.assertTrue(equaly_balanced,
                            "Ecmp paths are not equally balanced")

            # Setting the ECMP default hash algorithm to RANDOM
            self.setupECMPSeed(seed=0)
            self.setupECMPAlgorithm(SAI_HASH_ALGORITHM_RANDOM)
            hash_attr_get = sai_thrift_get_switch_attribute(
                self.client, ecmp_default_hash_algorithm=True)
            print('Read value of algorithm: ', hash_attr_get)
            self.assertEqual(hash_attr_get[
                'SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM'],
                             SAI_HASH_ALGORITHM_RANDOM)

            ecmp_random_count = self.l3IPv4EcmpPacketTest(hash_dict,
                                                          max_itrs=L3_MAX_ITRS)
            print("\nECMP count: %s \n" % (ecmp_random_count))
            nbr_active_ports_rd = verify_lb_active_ports(
                ecmp_random_count)
            self.assertEqual(nbr_active_ports_rd, 4)

            equaly_balanced_rd = verify_equaly_balanced(
                ecmp_random_count, pkt_count=L3_MAX_ITRS,
                expected_base=0.6)

            self.assertTrue(equaly_balanced_rd,
                            "Ecmp paths are not equally balanced")
            similary_balanced = verify_similary_balanced(
                ecmp_count,
                ecmp_random_count)
            self.assertTrue(similary_balanced,
                            "ECMP Algoritm change has no effect on LB")

        finally:
            self.setupECMPSeed(seed=0)
            self.setupECMPAlgorithm()

    def nonIpHashTest(self,
                      hash_dst_mac=None,
                      hash_src_mac=None,
                      hash_ether_type=None):
        """
        Verifies the hashing on LAG members for L2 traffic

        Args:
            hash_dst_mac (boolean): indicates if test uses dst mac hashed
                                    traffic
            hash_src_mac (boolean): indicates if test uses src mac hashed
                                    traffic
            hash_ether_type (boolean): indicates if test uses ether type hashed
                                       traffic
        """

        hash_dict = {'hash_dst_mac': hash_dst_mac,
                     'hash_src_mac': hash_src_mac,
                     'hash_ether_type': hash_ether_type}

        try:
            hash_fields = hash_to_hash_fields(hash_dict)
            self.setupNonIPECMPHash(hash_fields)

            print("Setting Non-IP hash object with hash fields: %s" % (
                hash_fields_to_hash_names(hash_fields)))
            lb_count = self.l2hashTraffic(
                hash_dst_mac=hash_dst_mac,
                hash_src_mac=hash_src_mac,
                hash_ether_type=hash_ether_type)
            print('NonIP-pkt-count:', lb_count)
            nbr_active_ports = verify_lb_active_ports(lb_count)
            self.assertEqual(nbr_active_ports, 3)

            equaly_balanced = verify_equaly_balanced(lb_count)
            self.assertTrue(equaly_balanced,
                            "paths are not equally balanced")

            lb_count = self.l2hashTraffic(hash_src_mac=not hash_src_mac,
                                          hash_dst_mac=not hash_dst_mac,
                                          hash_ether_type=not hash_ether_type)
            print('NonIP-pkt-count:', lb_count)
            nbr_active_ports = verify_lb_active_ports(lb_count)
            self.assertTrue(
                nbr_active_ports == 1,
                "Expected no LB, traffic on single port only")
        finally:
            sai_thrift_flush_fdb_entries(
                self.client,
                entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

    def l3LagIPv4HashTest(self,
                          hash_src_ip=None,
                          hash_dst_ip=None,
                          hash_udp_sport=None,
                          hash_udp_dport=None):
        """
        Verfies L3 IPv4 traffic distribution using all the field selected
        for IPv4 LAG Hash

        Args:
            hash_src_ip (boolean): indicates if test uses src ip hashed traffic
            hash_dst_ip (boolean): indicates if test uses dst ip hashed traffic
            hash_udp_sport (boolean): indicates if test uses udp sport hashed
                                      traffic
            hash_udp_dport (boolean): indicates if test uses udp dport hashed
                                      traffic
        """

        hash_dict = {'hash_src_ip': hash_src_ip,
                     'hash_dst_ip': hash_dst_ip,
                     'hash_udp_dport': hash_udp_dport,
                     'hash_udp_sport': hash_udp_sport}

        hash_fields = hash_to_hash_fields(hash_dict)
        print("Verify IPv4 LAG load balancing for hash fields: %s"
              % (hash_fields_to_hash_names(hash_fields)))
        self.setupLAGIPv4Hash(hash_fields)

        lag_hashing_counts = self.l3IPv4LagPacketTest(hash_dict)
        print("LAG hash LB count (LB expected):", lag_hashing_counts)
        nbr_active_ports = verify_lb_active_ports(lag_hashing_counts)
        self.assertEqual(nbr_active_ports, 3)
        equaly_balanced = verify_equaly_balanced(lag_hashing_counts)
        self.assertTrue(equaly_balanced,
                        "LAG members are not equally balanced")
        lag_hashing_counts = self.l3IPv4LagPacketTest(
            hash_dict_negation(hash_dict))
        print("LAG hash LB count (LB NOT expected):", lag_hashing_counts)
        no_lb = verify_no_lb(lag_hashing_counts, max_iters=MAX_ITRS)
        self.assertTrue(no_lb, "Not expected to balance")

    def lagHashAlgorithmTest(self):
        """
        Verifies lag hash algorithm
        """
        test_header("lagHashAlgorithmTest")

        try:
            print("Verify L3 load balancing for all LAG members "
                  "for all fields hashing")
            self.setupLAGIPv4Hash()

            self.setupLagSeed(seed=0)
            self.setupLagAlgorithm()
            hash_attr_get = sai_thrift_get_switch_attribute(
                self.client, lag_default_hash_algorithm=True)
            self.assertEqual(hash_attr_get[
                'SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_ALGORITHM'],
                             SAI_HASH_ALGORITHM_CRC)
            print('Read value of algorithm: ', hash_attr_get)
            hash_dict = {'hash_src_ip': True, 'hash_udp_dport': True,
                         'hash_udp_sport': True}
            lag_pkt_count = self.l3IPv4LagPacketTest(hash_dict,
                                                     max_itrs=LAG_MAX_ITRS)
            print("\nPacket count: %s \n" % (lag_pkt_count))
            nbr_active_ports = verify_lb_active_ports(lag_pkt_count)
            self.assertEqual(nbr_active_ports, 3)

            equaly_balanced = verify_equaly_balanced(
                lag_pkt_count, pkt_count=LAG_MAX_ITRS, expected_base=0.6)
            self.assertTrue(equaly_balanced,
                            "Ecmp paths are not equally balanced")

            # Setting the ECMP default hash algorithm to RANDOM
            self.setupLagSeed(seed=0)
            self.setupLagAlgorithm(SAI_HASH_ALGORITHM_RANDOM)
            hash_attr_get = sai_thrift_get_switch_attribute(
                self.client, lag_default_hash_algorithm=True)
            self.assertEqual(hash_attr_get[
                'SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_ALGORITHM'],
                             SAI_HASH_ALGORITHM_RANDOM)
            print('Read value of algorithm: ', hash_attr_get)

            pkt_count_random = self.l3IPv4LagPacketTest(hash_dict,
                                                        max_itrs=LAG_MAX_ITRS)
            print("\nPacket count: %s \n" % (pkt_count_random))
            nbr_active_ports_rd = verify_lb_active_ports(pkt_count_random)
            self.assertEqual(nbr_active_ports_rd, 3)

            equaly_balanced_rd = verify_equaly_balanced(
                pkt_count_random, pkt_count=LAG_MAX_ITRS, expected_base=0.6)
            self.assertTrue(equaly_balanced_rd,
                            "Ecmp paths are not equally balanced")

            similary_balanced = verify_similary_balanced(
                lag_pkt_count, pkt_count_random)
            self.assertTrue(similary_balanced,
                            "LAG Algoritm change has no effect on LB")

        finally:
            self.setupLagAlgorithm()
            self.setupLagSeed(seed=0)

    def l2LagHashSeedTest(self):
        """
        Verifies basic L2 Lag configuration with varing seed values
        """
        test_header("l2LagHashSeedTest")

        hash_dict = {'hash_dst_mac': True, 'hash_src_mac': True,
                     'hash_ether_type': True}

        print("Verify L2 load balancing for all LAG members "
              "for all fields hashing")
        hash_fields = hash_to_hash_fields(hash_dict)

        self.setupNonIPECMPHash(hash_fields)

        self.setupLagSeed(seed=TEST_LAG_SEED1)
        lb_count = self.l2hashTraffic(
            hash_dst_mac=True,
            hash_src_mac=True,
            hash_ether_type=True)
        print("LAG count seed=%d:" % (TEST_LAG_SEED1), lb_count)
        nbr_active_ports = verify_lb_active_ports(lb_count)
        self.assertEqual(nbr_active_ports, 3)

        new_seed = TEST_LAG_SEED1 * 2
        self.setupLagSeed(seed=new_seed)
        lb_count_seed1 = self.l2hashTraffic(
            hash_dst_mac=True,
            hash_src_mac=True,
            hash_ether_type=True)

        print("LAG count seed=%d:" % (new_seed), lb_count_seed1)
        nbr_active_ports = verify_lb_active_ports(lb_count_seed1)
        self.assertEqual(nbr_active_ports, 3)

        # verify that changing seed changes modifies the LB
        # vefiry if LB factor changed
        similary_balanced = verify_similary_balanced(
            lb_count, lb_count_seed1)
        self.assertTrue(similary_balanced,
                        "LAG Seed change has no effect on LB")

    def l3LagIPv4HashSeedTest(self):
        """
        Verifies basic L3 Lag configuration with varing seed values
        """

        hash_dict = {'hash_src_ip': True, 'hash_dst_ip': True,
                     'hash_udp_dport': True, 'hash_udp_sport': True}

        hash_fields = hash_to_hash_fields(hash_dict)
        print("Verify IPv4 LAG load balancing for hash fields: %s"
              % (hash_fields_to_hash_names(hash_fields)))
        self.setupLAGIPv4Hash(hash_fields)

        self.setupLagSeed(seed=TEST_LAG_SEED)

        ecmp_count = self.l3IPv4LagPacketTest(hash_dict)

        print("ECMP count seed=%d:" % (TEST_LAG_SEED), ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertEqual(nbr_active_ports, 3)
        equaly_balanced = verify_equaly_balanced(
            ecmp_count, expected_base=SEED_TEST_CHECK_BASE)
        self.assertTrue(equaly_balanced,
                        "LAG paths are not equally balanced")

        self.setupLagSeed(seed=TEST_LAG_SEED1)
        print("Verify load balancing for IPv4 hash seed = %d"
              % (TEST_LAG_SEED1))
        ecmp_count_seed1 = self.l3IPv4LagPacketTest(hash_dict)
        print("ECMP count seed=%d:" % (TEST_LAG_SEED1), ecmp_count_seed1)
        nbr_active_ports = verify_lb_active_ports(ecmp_count_seed1)
        self.assertEqual(nbr_active_ports, 3)
        equaly_balanced = verify_equaly_balanced(
            ecmp_count_seed1, expected_base=SEED_TEST_CHECK_BASE)
        self.assertTrue(equaly_balanced,
                        "LAG paths are not equally balanced")
        # verify if LB distribution has changed
        similary_balanced = verify_similary_balanced(
            ecmp_count, ecmp_count_seed1)
        self.assertTrue(similary_balanced,
                        "LAG Seed change has no effect on LB")


@group("draft")
class SAIIPv6HashTest(SAIHashTestBase):
    """
    Runs IPv6 hash test cases
    """

    def setUp(self):

        super(SAIIPv6HashTest, self).setUp()

        self.ipv4_hash_id = 0
        self.ipv6_hash_id = 0
        dmac5 = '00:55:55:55:55:55'
        dmac6 = '00:66:66:66:66:66'
        dmac7 = '00:77:77:77:77:77'
        dmac8 = '00:88:88:88:88:88'

        nhop_ip5 = '5000:1:1:0:0:0:0:1'
        nhop_ip6 = '6000:1:1:0:0:0:0:1'
        nhop_ip7 = '7000:1:1:0:0:0:0:1'
        nhop_ip8 = '0001:1:1:0:0:0:0:1'

        # set switch src mac address
        sai_thrift_set_switch_attribute(
            self.client,
            src_mac_address=ROUTER_MAC)
        self.port25_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port25,
            admin_v6_state=True)
        self.port26_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port26,
            admin_v6_state=True)
        self.port27_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.port27,
            admin_v6_state=True)
        self.lag1_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_PORT,
            virtual_router_id=self.default_vrf,
            port_id=self.lag1, admin_v6_state=True)

        self.neighbor_entry25 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.port25_rif,
            sai_ipaddress(nhop_ip5))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry25,
            dst_mac_address=dmac5)
        self.neighbor_entry26 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.port26_rif,
            sai_ipaddress(nhop_ip6))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry26,
            dst_mac_address=dmac6)
        self.neighbor_entry27 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.port27_rif,
            sai_ipaddress(nhop_ip7))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry27,
            dst_mac_address=dmac7)

        self.neighbor_entry_lag1 = sai_thrift_neighbor_entry_t(
            self.switch_id,
            self.lag1_rif,
            sai_ipaddress(nhop_ip8))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry_lag1,
            dst_mac_address=dmac8)

        self.nhop5 = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port25_rif, ip=sai_ipaddress(nhop_ip5))
        self.nhop6 = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port26_rif, ip=sai_ipaddress(nhop_ip6))
        self.nhop7 = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.port27_rif, ip=sai_ipaddress(nhop_ip7))
        self.lag1_nhop1 = sai_thrift_create_next_hop(
            self.client, type=SAI_NEXT_HOP_TYPE_IP,
            router_interface_id=self.lag1_rif, ip=sai_ipaddress(nhop_ip8))

        self.nhop_group1 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group1_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop5)
        self.nh_group1_member2 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop6)
        self.nh_group1_member3 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group1,
            next_hop_id=self.nhop7)

        self.nhop_group2 = sai_thrift_create_next_hop_group(
            self.client, type=SAI_NEXT_HOP_GROUP_TYPE_ECMP)
        self.nh_group2_member1 = sai_thrift_create_next_hop_group_member(
            self.client,
            next_hop_group_id=self.nhop_group2,
            next_hop_id=self.lag1_nhop1)

        # create route entries
        self.route0 = sai_thrift_route_entry_t(switch_id=self.switch_id,
                                               destination=sai_ipprefix(
                                                   '1000:1:1:0:0:0:0:0/65'),
                                               vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client,
            self.route0,
            next_hop_id=self.nhop_group1)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        # create route entries for IPv6 Lag test
        self.route1 = sai_thrift_route_entry_t(switch_id=self.switch_id,
                                               destination=sai_ipprefix(
                                                   '8000:1:1:0:0:0:0:0/65'),
                                               vr_id=self.default_vrf)
        status = sai_thrift_create_route_entry(
            self.client,
            self.route1,
            next_hop_id=self.nhop_group2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def tearDown(self):
        try:
            sai_thrift_flush_fdb_entries(
                self.client,
                entry_type=SAI_FDB_FLUSH_ENTRY_TYPE_ALL)

            sai_thrift_remove_neighbor_entry(
                self.client,
                self.neighbor_entry25)
            sai_thrift_remove_neighbor_entry(
                self.client,
                self.neighbor_entry26)
            sai_thrift_remove_neighbor_entry(
                self.client,
                self.neighbor_entry27)
            sai_thrift_remove_neighbor_entry(
                self.client,
                self.neighbor_entry_lag1)

            sai_thrift_remove_next_hop_group_member(
                self.client, self.nh_group1_member1)
            sai_thrift_remove_next_hop_group_member(
                self.client, self.nh_group1_member2)
            sai_thrift_remove_next_hop_group_member(
                self.client, self.nh_group1_member3)
            sai_thrift_remove_next_hop_group_member(
                self.client, self.nh_group2_member1)

            sai_thrift_remove_route_entry(self.client, self.route0)
            sai_thrift_remove_route_entry(self.client, self.route1)

            sai_thrift_remove_next_hop_group(self.client, self.nhop_group1)
            sai_thrift_remove_next_hop_group(self.client, self.nhop_group2)
            sai_thrift_remove_next_hop(self.client, self.nhop5)
            sai_thrift_remove_next_hop(self.client, self.nhop6)
            sai_thrift_remove_next_hop(self.client, self.nhop7)
            sai_thrift_remove_next_hop(self.client, self.lag1_nhop1)

            sai_thrift_remove_router_interface(self.client, self.port25_rif)
            sai_thrift_remove_router_interface(self.client, self.port26_rif)
            sai_thrift_remove_router_interface(self.client, self.port27_rif)
            sai_thrift_remove_router_interface(self.client, self.lag1_rif)

        finally:
            super(SAIIPv6HashTest, self).tearDown()

    def l3IPv6EcmpPacketTest(self, hash_dict):
        """
        Function that performs the IPv6 ECMP test with L3 hashed traffic
        Args:
            hash_dict (dict): dictionary with variables that defines the list
            of hash test fields and traffic header hashed fields
        Returns:
            list: list of numbers of packet egressed on specific test port
        """
        ecmp_count = [0, 0, 0]
        src_mac_start = '00:22:22:22:{0}:{1}'
        src_mac = '00:22:22:22:22:22'
        dst_mac = ROUTER_MAC
        udp_sport = 7
        udp_dport = 7
        dst_ip = socket.inet_pton(socket.AF_INET6, '1000:1:1:0:0:0:0:1')
        dst_ip_arr = list(dst_ip)
        src_ip = socket.inet_pton(socket.AF_INET6, '6000:1:1:0:0:0:0:100')
        src_ip_arr = list(src_ip)
        for i in range(0, MAX_ITRS):
            if ('hash_src_mac' in hash_dict) and (hash_dict['hash_src_mac']):
                src_mac = src_mac_start.format(
                    str(0xFF & (i * 117)).zfill(4)[:2],
                    str(0xFF & (i * 13)).zfill(4)[2:])
            if ('hash_dst_ip' in hash_dict) and (hash_dict['hash_dst_ip']):
                dst_ip_arr[15] = 0xFF & (dst_ip_arr[15] + 17)
                dst_ip = binascii.unhexlify(''.join(
                    '%02x' % x for x in dst_ip_arr))
            if ('hash_src_ip' in hash_dict) and (hash_dict['hash_src_ip']):
                src_ip_arr[15] = 0xFF & (src_ip_arr[15] + 13)
                src_ip = binascii.unhexlify(''.join(
                    '%02x' % x for x in src_ip_arr))
            dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
            src_ip_addr = socket.inet_ntop(socket.AF_INET6, src_ip)
            if ('hash_udp_sport' in hash_dict) and \
                    (hash_dict['hash_udp_sport']):
                udp_sport += 13
            if ('hash_udp_dport' in hash_dict) and \
                    (hash_dict['hash_udp_dport']):
                udp_dport += 13

            pkt = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=src_mac,
                                      ipv6_dst=dst_ip_addr,
                                      ipv6_src=src_ip_addr,
                                      udp_sport=udp_sport,
                                      udp_dport=udp_dport,
                                      ipv6_hlim=64)

            exp_pkt1 = simple_udpv6_packet(eth_dst='00:55:55:55:55:55',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src=src_ip_addr,
                                           udp_sport=udp_sport,
                                           udp_dport=udp_dport,
                                           ipv6_hlim=63)
            exp_pkt2 = simple_udpv6_packet(eth_dst='00:66:66:66:66:66',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src=src_ip_addr,
                                           udp_sport=udp_sport,
                                           udp_dport=udp_dport,
                                           ipv6_hlim=63)
            exp_pkt3 = simple_udpv6_packet(eth_dst='00:77:77:77:77:77',
                                           eth_src=ROUTER_MAC,
                                           ipv6_dst=dst_ip_addr,
                                           ipv6_src=src_ip_addr,
                                           udp_sport=udp_sport,
                                           udp_dport=udp_dport,
                                           ipv6_hlim=63)

            send_packet(self, self.dev_port11, pkt)
            ports_to_verify = [
                self.dev_port25,
                self.dev_port26,
                self.dev_port27]

            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt1, exp_pkt2, exp_pkt3], ports_to_verify)
            if DEBUG:
                print("idx:", rcv_idx, "dst_ip:", dst_ip_addr, " src_ip:",
                      src_ip_addr, " smac:", src_mac, " dmac: ", dst_mac)
            ecmp_count[rcv_idx] += 1

        return ecmp_count

    def lagIPv6PacketTest(self, hash_dict):
        """
        Function that performs the IPv6 Lag test with L3 hashed traffic
        Args:
            hash_dict (dict): dictionary with variables that defines the list
            of hash test fields and traffic header hashed fields
        Returns:
            list: list of numbers of packet egressed on specific ecmp group
                  member
        """
        ecmp_count = [0, 0, 0]
        src_mac_start = '00:22:22:22:{0}:{1}'
        src_mac = '00:22:22:22:22:22'
        dst_mac = ROUTER_MAC
        udp_sport = 7
        udp_dport = 7
        dst_ip = socket.inet_pton(socket.AF_INET6, '8000:1:1:0:0:0:0:1')
        dst_ip_arr = list(dst_ip)
        src_ip = socket.inet_pton(socket.AF_INET6, '6000:1:1:0:0:0:0:100')
        src_ip_arr = list(src_ip)
        ipv6_flow_label = 0

        if ('hash_flow_label' in hash_dict) and (hash_dict['hash_flow_label']):
            ipv6_flow_label = 10

        for i in range(0, MAX_ITRS):
            if ('hash_src_mac' in hash_dict) and (hash_dict['hash_src_mac']):
                src_mac = src_mac_start.format(
                    str(0xFF & (i * 117)).zfill(4)[:2],
                    str(0xFF & (i * 13)).zfill(4)[2:])
            if ('hash_dst_ip' in hash_dict) and (hash_dict['hash_dst_ip']):
                dst_ip_arr[15] = 0xFF & (dst_ip_arr[15] + 17)
                dst_ip = binascii.unhexlify(''.join(
                    '%02x' % x for x in dst_ip_arr))
            if ('hash_src_ip' in hash_dict) and (hash_dict['hash_src_ip']):
                src_ip_arr[15] = 0xFF & (src_ip_arr[15] + 13)
                src_ip = binascii.unhexlify(''.join(
                    '%02x' % x for x in src_ip_arr))
            dst_ip_addr = socket.inet_ntop(socket.AF_INET6, dst_ip)
            src_ip_addr = socket.inet_ntop(socket.AF_INET6, src_ip)
            if ('hash_udp_sport' in hash_dict) and \
                    (hash_dict['hash_udp_sport']):
                udp_sport += 13
            if ('hash_udp_dport' in hash_dict) and \
                    (hash_dict['hash_udp_dport']):
                udp_dport += 13

            pkt = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                      eth_src=src_mac,
                                      ipv6_dst=dst_ip_addr,
                                      ipv6_src=src_ip_addr,
                                      udp_sport=udp_sport,
                                      udp_dport=udp_dport,
                                      ipv6_hlim=64,
                                      ipv6_fl=ipv6_flow_label)

            exp_pkt = simple_udpv6_packet(eth_dst='00:88:88:88:88:88',
                                          eth_src=ROUTER_MAC,
                                          ipv6_dst=dst_ip_addr,
                                          ipv6_src=src_ip_addr,
                                          udp_sport=udp_sport,
                                          udp_dport=udp_dport,
                                          ipv6_hlim=63,
                                          ipv6_fl=ipv6_flow_label)

            send_packet(self, self.dev_port4, pkt)
            ports_to_verify = [
                self.dev_port4,
                self.dev_port5,
                self.dev_port6]

            rcv_idx = verify_any_packet_any_port(
                self, [exp_pkt], ports_to_verify)
            if DEBUG:
                print("idx:", rcv_idx, "dst_ip:", dst_ip_addr, " src_ip:",
                      src_ip_addr, " smac:", src_mac, " dmac: ", dst_mac,
                      "ipv6_flow_label: ", ipv6_flow_label)
            ecmp_count[rcv_idx] += 1

        return ecmp_count

    def lagIPv6HashTest(self, hash_dict):
        """
        Verifies IPv6 LAG load balancing for hash fields

        Args:
            hash_dict (dict): dictionary with variables that defines the list
        """
        hash_fields = hash_to_hash_fields(hash_dict)
        print("Verify IPv6 LAG load balancing for hash fields: %s"
              % (hash_fields_to_hash_names(hash_fields)))

        self.setupLAGIPv6Hash(hash_fields)

        ecmp_count = self.lagIPv6PacketTest(hash_dict)

        print("ECMP count:", ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertTrue(
            nbr_active_ports == 3,
            "Expected to balance equally on all next hops.")
        equaly_balanced = verify_equaly_balanced(ecmp_count)
        self.assertTrue(equaly_balanced, "Ecmp paths are not equally"
                        "balanced for Lag IPv6 hashed traffic")

        print("Verify no load balancing for traffic "
              "with hash fields unchanged.")
        ecmp_count = self.lagIPv6PacketTest(hash_dict_negation(hash_dict))
        print("ECMP count:", ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertTrue(
            nbr_active_ports == 1,
            "Does not expect to balance on other hash fields.")

    def lagIPv6FlowLabelHashTest(self, hash_dict):
        """
        Verifies IPv6 LAG load balancing for hash fields

        Args:
            hash_dict (dict): dictionary with variables that defines the list
        """
        try:
            hash_fields = hash_to_hash_fields(hash_dict)
            print("Verify IPv6 Flow Label LAG load balancing "
                  "for hash fields: %s"
                  % (hash_fields_to_hash_names(hash_fields)))

            self.setupLAGIPv6Hash(hash_fields)

            packet_count = self.lagIPv6PacketTest(hash_dict)
            print("Packet count:", packet_count)

            no_lb = verify_no_lb(packet_count, max_iters=MAX_ITRS)
            self.assertTrue(no_lb, "Not expected to balance")

        finally:
            pass

    def lagIPv6HashSeedTest(self):
        """
        Verfies L3 IPv6 Lag basic case with varing seed values
        """

        hash_dict = {'hash_src_ip': True, 'hash_dst_ip': True,
                     'hash_udp_dport': True, 'hash_udp_sport': True}

        hash_fields = hash_to_hash_fields(hash_dict)
        print("Verify IPv6 LAG load balancing for hash fields: %s"
              % (hash_fields_to_hash_names(hash_fields)))
        self.setupLAGIPv6Hash(hash_fields)

        self.setupLagSeed(seed=TEST_LAG_SEED)

        ecmp_count = self.lagIPv6PacketTest(hash_dict)

        print("ECMP count seed=%d:" % (TEST_LAG_SEED), ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertTrue(
            nbr_active_ports == 3,
            "Expected to balance equally on all next hops.")
        equaly_balanced = verify_equaly_balanced(
            ecmp_count, expected_base=SEED_TEST_CHECK_BASE)
        self.assertTrue(equaly_balanced, "Ecmp paths are not equally"
                        "balanced for Lag IPv6 hashed traffic")

        self.setupLagSeed(seed=TEST_LAG_SEED1)

        print("Verify load balancing for IPv6 hash seed = %d"
              % (TEST_LAG_SEED1))
        ecmp_count_seed1 = self.lagIPv6PacketTest(hash_dict)
        print("ECMP count seed=%d:" % (TEST_LAG_SEED1), ecmp_count_seed1)
        nbr_active_ports = verify_lb_active_ports(ecmp_count_seed1)
        self.assertEqual(nbr_active_ports, 3)
        equaly_balanced = verify_equaly_balanced(
            ecmp_count_seed1, expected_base=SEED_TEST_CHECK_BASE)
        self.assertTrue(equaly_balanced,
                        "LAG paths are not equally balanced")
        # verify if LB distribution has changed
        similary_balanced = verify_similary_balanced(
            ecmp_count, ecmp_count_seed1)
        self.assertTrue(similary_balanced,
                        "LAG Seed change has no effect on LB")

    def ecmpIPv6HashTest(self,
                         hash_src_ip=None,
                         hash_dst_ip=None,
                         hash_udp_sport=None,
                         hash_udp_dport=None):
        """
        Verifies traffic distribution using all the fields selected
        for ECMP IPv6 Hash

        Args:
            hash_src_ip (boolean): indicates if test uses src ip hashed traffic
            hash_dst_ip (boolean): indicates if test uses dst ip hashed traffic
            hash_udp_sport (boolean): indicates if test uses udp sport hashed
                                      traffic
            hash_udp_dport (boolean): indicates if test uses udp dport hashed
                                      traffic
        """

        hash_dict = {'hash_src_ip': hash_src_ip, 'hash_dst_ip': hash_dst_ip,
                     'hash_udp_dport': hash_udp_dport,
                     'hash_udp_sport': hash_udp_sport}

        hash_fields = hash_to_hash_fields(hash_dict)
        print("Verify IPv6 ECMP load balancing for hash fields: %s"
              % (hash_fields_to_hash_names(hash_fields)))

        # setup IPv6 hash fields for all fields
        self.setupECMPIPv6Hash(hash_fields)

        ecmp_count = self.l3IPv6EcmpPacketTest(hash_dict)

        print("ECMP LB count:", ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertTrue(
            nbr_active_ports == 3,
            "Expected to balance equally on all next hops.")
        equaly_balanced = verify_equaly_balanced(ecmp_count)
        self.assertTrue(equaly_balanced,
                        "Ecmp paths are not equally balanced")

        print("Verify no load balancing for non hashed traffic.")
        ecmp_count = self.l3IPv6EcmpPacketTest(hash_dict_negation(
            hash_dict))

        print("ECMP no LB count:", ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertTrue(
            nbr_active_ports == 1,
            "Does not expect to balance on other hash fields.")

    def l3EcmpIPv6HashSeedTest(self):
        """
        Verifies IPv6 ECMP with varing seed values
        """
        test_header("l3EcmpIPv6HashSeedTest")

        hash_dict = {'hash_src_ip': True, 'hash_dst_ip': True,
                     'hash_udp_dport': True, 'hash_udp_sport': True}

        hash_fields = hash_to_hash_fields(hash_dict)
        self.setupECMPIPv6Hash(hash_fields)

        self.setupECMPSeed(seed=TEST_ECMP_SEED)

        print("Verify load balancing equaly for all next_hops when "
              "all packets fields changes")
        ecmp_count = self.l3IPv6EcmpPacketTest(hash_dict)
        print("ECMP count seed=%d:" % (TEST_ECMP_SEED), ecmp_count)
        equaly_balanced = verify_equaly_balanced(
            ecmp_count, expected_base=SEED_TEST_CHECK_BASE)
        self.assertTrue(equaly_balanced,
                        "Ecmp paths are not equally balanced with seed=%d"
                        % (TEST_ECMP_SEED1))
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertEqual(nbr_active_ports, 3)

        # set the ecmp hash seed
        self.setupECMPSeed(seed=TEST_ECMP_SEED1)
        ecmp_count_seed1 = self.l3IPv6EcmpPacketTest(hash_dict)
        print("ECMP count seed=%d:" % (TEST_ECMP_SEED1), ecmp_count_seed1)
        equaly_balanced = verify_equaly_balanced(
            ecmp_count_seed1, expected_base=SEED_TEST_CHECK_BASE)
        self.assertTrue(equaly_balanced,
                        "Ecmp paths are not equally balanced with seed=1")
        nbr_active_ports = verify_lb_active_ports(ecmp_count_seed1)
        self.assertEqual(nbr_active_ports, 3)

        # vefiry if LB factor changed
        similary_balanced = verify_similary_balanced(
            ecmp_count, ecmp_count_seed1)
        self.assertTrue(similary_balanced,
                        "ECMP Seed change has no effect on LB")

    def ecmpIPv6HashSaveRestoreTest(self):
        """
        Verfies modification of the switch ECMP IPv6 hash fields
        """
        test_header("ecmpIPv6HashSaveRestoreTest")
        hash_fields_list = [SAI_NATIVE_HASH_FIELD_SRC_IP,
                            SAI_NATIVE_HASH_FIELD_DST_IP,
                            SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                            SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                            SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]

        attr_list = sai_thrift_get_switch_attribute(
            self.client, ecmp_hash_ipv6=True)
        ipv6_hash_id = attr_list['SAI_SWITCH_ATTR_ECMP_HASH_IPV6']
        if ipv6_hash_id == 0:
            if self.ipv6_hash_id != 0:
                ipv6_hash_id = self.ipv6_hash_id
            else:
                s32list = sai_thrift_s32_list_t(
                    count=len(hash_fields_list),
                    int32list=hash_fields_list)
                ipv6_hash_id = sai_thrift_create_hash(
                    self.client,
                    native_hash_field_list=s32list)
                self.assertTrue(
                    ipv6_hash_id != 0,
                    "Failed to create IPv6 hash")
                status = sai_thrift_set_switch_attribute(
                    self.client, ecmp_hash_ipv6=ipv6_hash_id)
                self.assertEqual(status, SAI_STATUS_SUCCESS)
                attr_list = sai_thrift_get_switch_attribute(
                    self.client, ecmp_hash_ipv6=True)
                ipv6_hash_id = attr_list['SAI_SWITCH_ATTR_ECMP_HASH_IPV6']
                self.assertTrue(
                    ipv6_hash_id != 0,
                    "failed to restore the switch IPv6 ecmp hash")
                self.ipv6_hash_id = ipv6_hash_id

        # update existing IPv6 hash
        s32list = sai_thrift_s32_list_t(
            count=len(hash_fields_list),
            int32list=hash_fields_list)
        status = sai_thrift_set_hash_attribute(
            self.client,
            ipv6_hash_id,
            native_hash_field_list=s32list)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        # verify the native hash fields list
        self.assertEqual(
            True,
            self.verifyHashFieldsList(
                ipv6_hash_id,
                hash_fields_list))

        # verify new hash create
        test_fields_list = [[SAI_NATIVE_HASH_FIELD_SRC_IP],
                            [SAI_NATIVE_HASH_FIELD_DST_IP],
                            [SAI_NATIVE_HASH_FIELD_SRC_IP,
                             SAI_NATIVE_HASH_FIELD_DST_IP],
                            [SAI_NATIVE_HASH_FIELD_IP_PROTOCOL],
                            [SAI_NATIVE_HASH_FIELD_SRC_IP,
                             SAI_NATIVE_HASH_FIELD_DST_IP,
                             SAI_NATIVE_HASH_FIELD_IP_PROTOCOL,
                             SAI_NATIVE_HASH_FIELD_L4_DST_PORT,
                             SAI_NATIVE_HASH_FIELD_L4_SRC_PORT]]

        for field_list in test_fields_list:
            s32list = sai_thrift_s32_list_t(
                count=len(field_list),
                int32list=field_list)
            new_ipv6_hash_id = sai_thrift_create_hash(
                self.client,
                native_hash_field_list=s32list)
            self.assertTrue(
                new_ipv6_hash_id != 0,
                "Failed to create IPv6 hash")
            self.assertEqual(
                True,
                self.verifyHashFieldsList(
                    new_ipv6_hash_id,
                    field_list))
            status = sai_thrift_remove_hash(self.client, new_ipv6_hash_id)
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def ecmpIPv6vsIPv4HashTest(self):
        """
        Verifies IPv4 hash does not impact IPv6 hashing
        """
        test_header("ecmpIPv6vsIPv4HashTest")

        hash_dict_src = {'hash_src_ip': True}

        hash_dict = {'hash_dst_ip': True, 'hash_udp_dport': True,
                     'hash_udp_sport': True}

        self.setupECMPIPv6Hash([SAI_NATIVE_HASH_FIELD_SRC_IP])
        # verify if IPv4 hash update overwrites IPv6 hash
        self.setupECMPIPv4Hash([SAI_NATIVE_HASH_FIELD_DST_IP])
        print("Verify load balancing equaly for all next_hops when "
              "packet fields changes and do match hash fields.")

        # should ballance equally
        ecmp_count = self.l3IPv6EcmpPacketTest(hash_dict_src)
        print("ECMP count:", ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertEqual(nbr_active_ports, 3)

        equaly_balanced = verify_equaly_balanced(ecmp_count)
        self.assertTrue(equaly_balanced,
                        "Ecmp paths are not equally balanced")

        # should not ballance
        ecmp_count = self.l3IPv6EcmpPacketTest(hash_dict)
        print("ECMP count:", ecmp_count)
        nbr_active_ports = verify_lb_active_ports(ecmp_count)
        self.assertTrue(nbr_active_ports == 1, "LB not expected")


@group("draft")
class NonIPSrcMacHashTest(SAIHashTest):
    """
    Verfies traffic distribution using src MAC selection for Non-IP Hash
    """

    def runTest(self):
        test_header("NonIPSrcMacHashTest")
        self.nonIpHashTest(hash_src_mac=True)


@group("draft")
class NonIPDstMacHashTest(SAIHashTest):
    """
    Verfies traffic distribution using dst MAC selection for Non-IP Hash
    """

    def runTest(self):
        test_header("NonIPDstMacHashTest")
        self.nonIpHashTest(hash_dst_mac=True)


@group("draft")
class L2LagHashEtherTypeTest(SAIHashTest):
    """
    Verfies traffic distribution using the Ether type selection for L2 Lag Hash
    """

    def runTest(self):
        test_header("L2LagHashEtherTypeTest")
        self.nonIpHashTest(hash_ether_type=True)


@group("draft")
class L2LagHashSrcMACTest(SAIHashTest):
    '''
    Verfies traffic distribution using the src MAC selection for L2 Lag Hash
    '''

    def runTest(self):
        test_header("L2LagHashSrcMACTest")
        self.nonIpHashTest(hash_src_mac=True)


@group("draft")
class L2LagHashDstMACTest(SAIHashTest):
    '''
    Verfies traffic distribution using the dst MAC selection for L2 Lag Hash
    '''

    def runTest(self):
        test_header("L2LagHashDstMACTest")
        self.nonIpHashTest(hash_dst_mac=True)


@group("draft")
class L2LagHashTest(SAIHashTest):
    '''
    Verfies traffic distribution using all fields selection for L2 Lag Hash
    '''

    def runTest(self):
        test_header("L2LagHashTest")
        self.nonIpHashTest(hash_dst_mac=True, hash_src_mac=True,
                           hash_ether_type=True)


@group("draft")
class NonIpHashTest(SAIHashTest):
    """
    Verfies traffic distribution using all fields selection for Non-IP Hash
    """

    def runTest(self):
        test_header("nonIpHashTest")
        self.nonIpHashTest(hash_dst_mac=True, hash_src_mac=True,
                           hash_ether_type=True)


@group("draft")
class L2LagHashSeedTest(SAIHashTest):
    """
    Verfies L2 Lag basic case with varing seed values
    """

    def runTest(self):
        self.l2LagHashSeedTest()


@group("draft")
class EcmpIPv4SrcIPHashTest(SAIHashTest):
    """
    Verfies traffic distribution using src IP selection
    for ECMP IPv4 Hash
    """

    def runTest(self):
        test_header("EcmpIPv4SrcIPHashTest")
        self.ecmpIPv4HashTest(hash_src_ip=True)


@group("draft")
class FGEcmpIPv4SrcIPHashTest(SAIHashTest):
    """
    Verfies traffic distribution using src IP selection
    for Fine-Grained ECMP IPv4 Hash
    """

    def runTest(self):
        test_header("FGEcmpIPv4SrcIPHashTest")
        self.fgEcmpIPv4HashTest(hash_src_ip=True)


@group("draft")
class EcmpIPv4DstIPHashTest(SAIHashTest):
    """
    Verfies traffic distribution using dst IP selection
    for ECMP IPv4 Hash
    """

    def runTest(self):
        test_header("EcmpIPv4DstIPHashTest")
        self.ecmpIPv4HashTest(hash_dst_ip=True)


@group("draft")
class EcmpIPv4DstPortHashTest(SAIHashTest):
    """
    Verfies L3 IPv4 traffic distribution using dst port selection
    for ECMP hash
    """

    def runTest(self):
        test_header("EcmpIPv4DstPortHashTest")
        self.ecmpIPv4HashTest(hash_udp_dport=True)


@group("draft")
class EcmpIPv4SrcPortHashTest(SAIHashTest):
    """
    Verfies L3 IPv4 traffic distribution using src port selection
    for ECMP Hash
    """

    def runTest(self):
        test_header("EcmpIPv4SrcPortHashTest")
        self.ecmpIPv4HashTest(hash_udp_sport=True)


@group("draft")
class EcmpIPv4BasicHashNoLBTest(SAIHashTest):
    """
    Verfies ECMP IPv4 hash defined for single not modified IPv4 hash field
    with traffic and no load balancing
    """

    def runTest(self):
        self.ecmpIPv4BasicHashNoLBTest()


@group("draft")
class EcmpIPv4HashTest(SAIHashTest):
    """
    Verfies traffic distribution using all the fields selected
    for ECMP IPv4 Hash
    """

    def runTest(self):
        test_header("EcmpIPv4HashTest")
        self.ecmpIPv4HashTest(
            hash_src_ip=True,
            hash_dst_ip=True,
            hash_udp_sport=True,
            hash_udp_dport=True)


@group("draft")
class EcmpIPv4vsIPv6HashTest(SAIHashTest):
    """
    Verifies if IPv6 hash does not impact IPv4 hashing
    """

    def runTest(self):
        self.ecmpIPv4vsIPv6HashTest()


@group("draft")
class L3EcmpIPv4HashSeedTest(SAIHashTest):
    """
    Verfies IPv4 ECMP seed case with varing values
    """

    def runTest(self):
        self.l3EcmpIPv4HashSeedTest()


@group("draft")
class EcmpIPv4HashSaveRestoreTest(SAIHashTest):
    """
    Verfies ECMP for IPv4 standard
    creates, modifies and removes ECMP hash fields
    """

    def runTest(self):
        self.ecmpIPv4HashSaveRestoreTest()


@group("draft")
class EcmpHashAlgorithmTest(SAIHashTest):
    """
    Verfies ECMP hash algorithm
    """

    def runTest(self):
        self.ecmpHashAlgorithmTest()


@group("draft")
class L3LagIPv4HashTest(SAIHashTest):
    """
    Verfies L3 IPv4 traffic distribution using all the fields selected
    for IPv4 LAG Hash
    """

    def runTest(self):

        test_header("l3LagIPv4HashTest")
        self.l3LagIPv4HashTest(
            hash_src_ip=True,
            hash_dst_ip=True,
            hash_udp_sport=True,
            hash_udp_dport=True)


@group("draft")
class LagHashAlgorithmTest(SAIHashTest):
    """
    Verfies lag hash algorithm
    """

    def runTest(self):
        self.lagHashAlgorithmTest()


@group("draft")
class L3LagIPv4SrcIPHashTest(SAIHashTest):
    """
    Verfies L3 IPv4 traffic distribution using src IP selection
    for IPv4 LAG Hash
    """

    def runTest(self):
        test_header("L3LagIPv4SrcIPHashTest")
        self.l3LagIPv4HashTest(hash_src_ip=True)


@group("draft")
class L3LagIPv4DstIPHashTest(SAIHashTest):
    """
    Verfies L3 IPv4 traffic distribution using dst IP selection
    for IPv4 LAG Hash
    """

    def runTest(self):
        test_header("L3LagIPv4DstIPHashTest")
        self.l3LagIPv4HashTest(hash_dst_ip=True)


@group("draft")
class L3LagIPv4SrcPortHashTest(SAIHashTest):
    """
    Verfies L3 IPv4 traffic distribution using src port selection
    for LAG Hash
    """

    def runTest(self):
        test_header("L3LagIPv4SrcPortHashTest")
        self.l3LagIPv4HashTest(hash_udp_sport=True)


@group("draft")
class L3LagIPv4DstPortHashTest(SAIHashTest):
    """
    Verifies L3 IPv4 traffic distribution using dst port selection
    for IPv4 LAG Hash
    """

    def runTest(self):
        test_header("L3LagIPv4DstPortHashTest")
        self.l3LagIPv4HashTest(hash_udp_dport=True)


@group("draft")
class L3LagIPv4HashSeedTest(SAIHashTest):
    """
    Verfies L3 Lag basic case with varing seed values
    """

    def runTest(self):
        test_header("l3LagIPv4HashSeedTest")
        self.l3LagIPv4HashSeedTest()


@group("draft")
class LagHashSaveRestoreTest(SAIHashTest):
    """
    Verfies modification of the switch lag hash fields
    """

    def runTest(self):
        self.lagHashSaveRestoreTest()


@group("draft")
class LagIPv4HashSaveRestoreTest(SAIHashTest):
    """
    Verfies modification of the switch lag IPv4 hash fields
    """

    def runTest(self):
        self.lagIPv4HashSaveRestoreTest()


@group("draft")
class LagIPv6HashSaveRestoreTest(SAIHashTest):
    """
    Varifies modification of the switch lag IPv6 hash fields
    """

    def runTest(self):
        self.lagIPv6HashSaveRestoreTest()


@group("draft")
class L3LagIPv6HashSeedTest(SAIIPv6HashTest):
    """
    Verfies L3 IPv6 Lag basic case with varing seed values
    """

    def runTest(self):
        test_header("L3LagIPv6HashSeedTest")
        self.lagIPv6HashSeedTest()


@group("draft")
class L3LagIPv6HashTest(SAIIPv6HashTest):
    """
    Verfies L3 IPv6 traffic distribution using all the fields selected
    for LAG Hash
    """

    def runTest(self):
        hash_dict = {'hash_src_ip': True, 'hash_dst_ip': True,
                     'hash_src_mac': True, 'hash_udp_dport': True,
                     'hash_udp_sport': True}
        test_header("L3LagIPv6HashTest")
        self.lagIPv6HashTest(hash_dict)


@group("draft")
class L3LagIPv6FlowLabelHashTest(SAIIPv6HashTest):
    """
    Verfies L3 IPv6 traffic distribution using all ipv6 flow label
    fields selected for LAG Hash
    """

    def runTest(self):
        hash_dict = {'hash_flow_label': True}
        test_header("L3LagIPv6FlowLabelHashTest")
        self.lagIPv6FlowLabelHashTest(hash_dict)


@group("draft")
class L3LagIPv6SrcIPHashTest(SAIIPv6HashTest):
    """
    Verfies L3 IPv6 traffic distribution using src IP selection
    for LAG Hash
    """

    def runTest(self):
        hash_dict = {'hash_src_ip': True}
        test_header("L3LagIPv6SrcIPHashTest")
        self.lagIPv6HashTest(hash_dict)


@group("draft")
class L3LagIPv6DstIPHashTest(SAIIPv6HashTest):
    """
    Verfies L3 IPv6 traffic distribution using dst IP selection
    for LAG Hash
    """

    def runTest(self):
        hash_dict = {'hash_dst_ip': True}
        test_header("L3LagIPv6DstIPHashTest")
        self.lagIPv6HashTest(hash_dict)


@group("draft")
class L3LagIPv6SrcPortHashTest(SAIIPv6HashTest):
    """
    Verfies L3 IPv6 traffic distribution using src port selection
    for LAG Hash
    """

    def runTest(self):
        hash_dict = {'hash_udp_sport': True}
        test_header("L3LagIPv6SrcPortHashTest")
        self.lagIPv6HashTest(hash_dict)


@group("draft")
class L3LagIPv6DstPortHashTest(SAIIPv6HashTest):
    """
    Verfies L3 IPv6 traffic distribution using dst port selection
    for LAG Hash
    """

    def runTest(self):
        hash_dict = {'hash_udp_dport': True}
        test_header("L3LagIPv6DstPortHashTest")
        self.lagIPv6HashTest(hash_dict)


@group("draft")
class EcmpIPv6HashSaveRestoreTest(SAIIPv6HashTest):
    """
    Verfies ECMP for IPv6 standard
    creates, modifies and removes ECMP hash fields
    """

    def runTest(self):
        self.ecmpIPv6HashSaveRestoreTest()


@group("draft")
class L3EcmpIPv6HashSeedTest(SAIIPv6HashTest):
    """
    Verfies IPv6 ECMP seed case with varing values
    """

    def runTest(self):
        self.l3EcmpIPv6HashSeedTest()


@group("draft")
class EcmpIPv6HashTest(SAIIPv6HashTest):
    """
    Verfies traffic distribution using all the field selected
    for ECMP IPv6 Hash
    """

    def runTest(self):
        test_header("EcmpIPv6HashTest")
        self.ecmpIPv6HashTest(
            hash_src_ip=True,
            hash_dst_ip=True,
            hash_udp_sport=True,
            hash_udp_dport=True)


@group("draft")
class EcmpIPv6DstIPHashTest(SAIIPv6HashTest):
    """
    Verfies traffic distribution using dst IP selection
    for ECMP IPv6 Hash
    """

    def runTest(self):
        test_header("EcmpIPv6DstIPHashTest")
        self.ecmpIPv6HashTest(hash_dst_ip=True)


@group("draft")
class EcmpIPv6SrcIPHashTest(SAIIPv6HashTest):
    """
    Verfies traffic distribution using src IP selection
    for ECMP IPv6 Hash
    """

    def runTest(self):
        test_header("EcmpIPv6SrcIPHashTest")
        self.ecmpIPv6HashTest(hash_src_ip=True)


@group("draft")
class EcmpIPv6SrcPortHashTest(SAIIPv6HashTest):
    """
    Verfies L3 IPv6 traffic distribution using src port selection
    for ECMP Hash
    """

    def runTest(self):
        test_header("EcmpIPv6SrcPortHashTest")
        self.ecmpIPv6HashTest(hash_udp_sport=True)


@group("draft")
class EcmpIPv6DstPortHashTest(SAIIPv6HashTest):
    """
    Verfies L3 IPv6 traffic distribution using dst port selection
    for ECMP Hash
    """

    def runTest(self):
        test_header("EcmpIPv6DstPortHashTest")
        self.ecmpIPv6HashTest(hash_udp_dport=True)


@group("draft")
class EcmpIPv6vsIPv4HashTest(SAIIPv6HashTest):
    """
    Verifies if IPv4 hash does not impact IPv6 hashing
    """

    def runTest(self):
        self.ecmpIPv6vsIPv4HashTest()
