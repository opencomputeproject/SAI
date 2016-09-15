# Copyright 2013-present Barefoot Networks, Inc.
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
Thrift SAI interface ACL tests
"""
from switch import *
import sai_base_test

# Define test data
IP_SRC =        "192.168.0.1"
IP_DST =        "10.10.10.1"
IP_MASK =       "255.255.255.0"
IP_ID =         105
IP_TTL =        64
PORT_IN =       2
PORT_OUT =      1
PORT_REDIRECT = 3

# TCP packets
PKT = simple_tcp_packet(eth_dst = router_mac,
                        eth_src = rewrite_mac1,
                        ip_dst = IP_DST,
                        ip_src = IP_SRC,
                        ip_id = IP_ID,
                        ip_ttl = IP_TTL)

EXP_PKT = simple_tcp_packet(eth_dst = rewrite_mac2,
                            eth_src = router_mac,
                            ip_dst = IP_DST,
                            ip_src = IP_SRC,
                            ip_id = IP_ID,
                            ip_ttl = IP_TTL - 1)


###############################################################################################################
############################################ SAI ACL PTF Base Class ###########################################
###############################################################################################################

@group('acl')
class aclTestBase(object):

    def setupRouting(self, port1, port2, port3):
        v4_enabled = 1
        v6_enabled = 1
        mac = ''

        self.vr_id = sai_thrift_create_virtual_router(self.client, v4_enabled, v6_enabled)

        self.rif_id1 = sai_thrift_create_router_interface(self.client, self.vr_id, 1, port1, 0, v4_enabled, v6_enabled, mac)
        self.rif_id2 = sai_thrift_create_router_interface(self.client, self.vr_id, 1, port2, 0, v4_enabled, v6_enabled, mac)
        self.rif_id3 = sai_thrift_create_router_interface(self.client, self.vr_id, 1, port3, 0, v4_enabled, v6_enabled, mac)

        self.addr_family = SAI_IP_ADDR_FAMILY_IPV4
        self.ip_addr1 = IP_DST
        self.ip_mask1 = IP_MASK
        self.dmac1 = rewrite_mac2
        sai_thrift_create_neighbor(self.client, self.addr_family, self.rif_id1, self.ip_addr1, self.dmac1)
        self.nhop1 = sai_thrift_create_nhop(self.client, self.addr_family, self.ip_addr1, self.rif_id1)
        sai_thrift_create_route(self.client, self.vr_id, self.addr_family, self.ip_addr1, self.ip_mask1, self.nhop1)

    def cleanupRouting(self):
        sai_thrift_remove_route(self.client, self.vr_id, self.addr_family, self.ip_addr1, self.ip_mask1, self.nhop1)
        self.client.sai_thrift_remove_next_hop(self.nhop1)
        sai_thrift_remove_neighbor(self.client, self.addr_family, self.rif_id1, self.ip_addr1, self.dmac1)

        self.client.sai_thrift_remove_router_interface(self.rif_id1)
        self.client.sai_thrift_remove_router_interface(self.rif_id2)
        self.client.sai_thrift_remove_router_interface(self.rif_id3)

        self.client.sai_thrift_remove_virtual_router(self.vr_id)

    def setupAclTable(self, stage = None, mac_src = None, mac_dst = None, mac_src_mask = None, mac_dst_mask = None,
                      in_port = None, out_port = None, in_ports = None, out_ports = None, priority = None, action = None,
                      ip_src = None, ip_src_mask = None, ip_dst = None, ip_dst_mask= None, ip_proto = None,
                      ingress_mirror_id = None, egress_mirror_id = None, counter = None, redirect_port = None):

        stage = SAI_ACL_STAGE_INGRESS
        priority = SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY

        # attributes for acl_table

        enable_mac_src = False
        if mac_src != None:
            enable_mac_src = True

        enable_mac_dst = False
        if mac_dst != None:
            enable_mac_dst = True

        enable_ip_src = False
        if ip_src != None:
            enable_ip_src = True

        enable_ip_dst = False
        if ip_dst != None:
            enable_ip_dst = True

        enable_ip_proto = False
        if ip_proto != None:
            enable_ip_proto = True

        enable_in_ports = False
        if in_ports != None:
            enable_in_ports = True

        enable_out_ports = False
        if out_ports != None:
            enable_out_ports = True

        enable_in_port = False
        if in_port != None:
            enable_in_port = True

        enable_out_port = False
        if out_port != None:
            enable_out_port = True

        self.acl_table_id = sai_thrift_create_acl_table(self.client,
                                                        self.addr_family,
                                                        stage,
                                                        priority,
                                                        enable_mac_src,
                                                        enable_mac_dst,
                                                        enable_ip_src,
                                                        enable_ip_dst,
                                                        enable_ip_proto,
                                                        enable_in_ports,
                                                        enable_out_ports,
                                                        enable_in_port,
                                                        enable_out_port)

        return True

    def setupAclRule(self, stage = None, mac_src = None, mac_dst = None, mac_src_mask = None, mac_dst_mask = None,
                     in_port = None, out_port = None, in_ports = None, out_ports = None, priority = None, action = None,
                     ip_src = None, ip_src_mask = None, ip_dst = None, ip_dst_mask= None, ip_proto = None,
                     ingress_mirror_id = None, egress_mirror_id = None, counter = None, redirect_port = None):

        self.acl_entry_id = sai_thrift_create_acl_entry(self.client, self.acl_table_id, priority,
                                                        action, self.addr_family,
                                                        mac_src, mac_src_mask,
                                                        mac_dst, mac_dst_mask,
                                                        ip_src, ip_src_mask,
                                                        ip_dst, ip_dst_mask,
                                                        ip_proto,
                                                        in_ports, out_ports,
                                                        in_port, out_port,
                                                        redirect_port,
                                                        ingress_mirror_id,
                                                        egress_mirror_id, counter)

        return True

    def cleanupAcl(self):
        print "----------------------------------------------------------------------"
        print "Clearing all rules"
        print "----------------------------------------------------------------------"
        self.client.sai_thrift_delete_acl_entry(self.acl_entry_id)
        self.client.sai_thrift_delete_acl_table(self.acl_table_id)

    def verifyRouting(self, pkt, exp_pkt, port_in, port_out):
        print "Sending packet"
        send_packet(self, port_in, str(pkt))
        verify_packets(self, exp_pkt, [port_out])
        print "Packet received on port %d" %port_out

    def verifyNoRouting(self, pkt, exp_pkt, port_in, port_out):
        print "Sending packet"
        send_packet(self, port_in, str(pkt))
        # ensure packet is dropped
        # check for absence of packet here!
        verify_no_packet(self, exp_pkt, port_out, 1)
        print "Packet not received on port %d" %port_out

    def verifyPacketOnPort(self, pkt, exp_pkt, port_in, port_out):
        verify_packets(self, exp_pkt, [port_out])
        print "Packet verified on port %s" %port_out

    def runTestFunc(self, action = 0, setup_acltbl=True, **kwargs):

        print kwargs

        if setup_acltbl:
            self.verifyRouting(PKT, EXP_PKT, PORT_IN, PORT_OUT)
            self.setupAclTable(**kwargs)

        self.setupAclRule(action=action, **kwargs)
        if action == SAI_PACKET_ACTION_DROP:
            self.verifyNoRouting(PKT, EXP_PKT, PORT_IN, PORT_OUT)
        elif action == SAI_PACKET_ACTION_FORWARD:
            self.verifyRouting(PKT, EXP_PKT, PORT_IN, PORT_OUT)
        elif action == SAI_PACKET_ACTION_COPY:
            self.verifyRouting(PKT, EXP_PKT, PORT_IN, PORT_OUT)
        elif action == SAI_PACKET_ACTION_TRAP:
            self.verifyNoRouting(PKT, EXP_PKT, PORT_IN, PORT_OUT)

    def runTest(self):
        print
        switch_init(self.client)

        self.setupRouting(port_list[PORT_OUT], port_list[PORT_IN], port_list[PORT_REDIRECT])

        try:
            self.runTestFunc()
        except Exception as err:
            print err
            raise err
        finally:
            self.cleanupAcl()

        self.cleanupRouting()

###############################################################################################################
################################## Ip address testing (SAI_ACL_STAGE_INGRESS) #################################
###############################################################################################################

@group('acl')
class testAclSrcIpDrop(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclSrcIpDrop, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                  action = SAI_PACKET_ACTION_DROP,
                                                  in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                  ip_src = IP_SRC,
                                                  ip_src_mask = IP_MASK)
        print "Source IPv4 address dropped successfully"

@group('acl')
class testAclSrcIpForward(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclSrcIpForward, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                     action = SAI_PACKET_ACTION_FORWARD,
                                                     in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                     ip_src = IP_SRC,
                                                     ip_src_mask = IP_MASK)
        print "Source IPv4 address forwarded successfully"

@group('acl')
class testAclDstIpDrop(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclDstIpDrop, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                  action = SAI_PACKET_ACTION_DROP,
                                                  in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                  ip_dst = IP_DST,
                                                  ip_dst_mask = IP_MASK)
        print "Destination IPv4 address dropped successfully"

@group('acl')
class testAclDstIpForward(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclDstIpForward, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                     action = SAI_PACKET_ACTION_FORWARD,
                                                     in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                     ip_dst = IP_DST,
                                                     ip_dst_mask = IP_MASK)
        print "Destination IPv4 address forwarded successfully"

###############################################################################################################
################################# MAC address testing (SAI_ACL_STAGE_INGRESS) #################################
###############################################################################################################

@group('acl')
class testAclSrcMacDrop(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclSrcMacDrop, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                   action = SAI_PACKET_ACTION_DROP,
                                                   in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                   mac_src = rewrite_mac1)
        print "Source MAC address dropped successfully"

@group('acl')
class testAclSrcMacForward(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclSrcMacForward, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                      action = SAI_PACKET_ACTION_FORWARD,
                                                      in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                      mac_src = rewrite_mac1)
        print "Source MAC address forwarded successfully"

@group('acl')
class testAclDstMacDrop(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclDstMacDrop, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                   action = SAI_PACKET_ACTION_DROP,
                                                   in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                   mac_dst = rewrite_mac2)
        print "Destination MAC address dropped successfully"

@group('acl')
class testAclDstMacForward(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclDstMacForward, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                      action = SAI_PACKET_ACTION_FORWARD,
                                                      in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                      mac_dst = rewrite_mac2)
        print "Destination MAC address forwarded successfully"


###############################################################################################################
################################# testing COPY action (SAI_ACL_STAGE_INGRESS) #################################
###############################################################################################################

@group('acl')
class testAclSrcIpCopy(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclSrcIpCopy, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                  action = SAI_PACKET_ACTION_COPY,
                                                  in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                  ip_src = IP_SRC,
                                                  ip_src_mask = IP_MASK)
        print "Source IPv4 address copied to CPU"

@group('acl')
class testAclDstIpCopy(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclDstIpCopy, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                  action = SAI_PACKET_ACTION_COPY,
                                                  in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                  ip_dst = IP_DST,
                                                  ip_dst_mask = IP_MASK)
        print "Destination IPv4 address copied to CPU"

@group('acl')
class testAclSrcMacCopy(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclSrcMacCopy, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                   action = SAI_PACKET_ACTION_COPY,
                                                   in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                   mac_src = rewrite_mac1)
        print "Source MAC address copied to CPU"

@group('acl')
class testAclDstMacCopy(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclDstMacCopy, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                   action = SAI_PACKET_ACTION_COPY,
                                                   in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                   mac_dst = rewrite_mac2)
        print "Destination MAC address copied to CPU"

###############################################################################################################
################################# testing TRAP action (SAI_ACL_STAGE_INGRESS) #################################
###############################################################################################################

@group('acl')
class testAclSrcIpTrap(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclSrcIpTrap, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                  action = SAI_PACKET_ACTION_TRAP,
                                                  in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                  ip_src = IP_SRC,
                                                  ip_src_mask = IP_MASK)
        print "Source IPv4 address droped in port 1 and forwarded to CPU"

@group('acl')
class testAclDstIpTrap(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclDstIpTrap, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                  action = SAI_PACKET_ACTION_TRAP,
                                                  in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                  ip_dst = IP_DST,
                                                  ip_dst_mask = IP_MASK)
        print "Destination IPv4 address droped in port 1 and forwarded to CPU"

@group('acl')
class testAclSrcMacTrap(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclSrcMacTrap, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                   action = SAI_PACKET_ACTION_TRAP,
                                                   in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                   mac_src = rewrite_mac1)
        print "Source MAC address droped in port 1 and forwarded to CPU"

@group('acl')
class testAclDstMacTrap(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclDstMacTrap, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                   action = SAI_PACKET_ACTION_TRAP,
                                                   in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                   mac_dst = rewrite_mac2)
        print "Destination MAC address droped in port 1 and forwarded to CPU"

###############################################################################################################
############################### testing REDIRECT action (SAI_ACL_STAGE_INGRESS) ###############################
###############################################################################################################

@group('acl')
class testAclSrcIpRedirect(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclSrcIpRedirect, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                      in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                      redirect_port = port_list[PORT_REDIRECT],
                                                      ip_src = IP_SRC,
                                                      ip_src_mask = IP_MASK)
        self.verifyPacketOnPort(PKT, EXP_PKT, PORT_IN, port_list[PORT_REDIRECT])
        print "Source IPv4 address redirected to redirect port"

@group('acl')
class testAclDstIpRedirect(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclDstIpRedirect, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                      in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                      redirect_port = port_list[PORT_REDIRECT],
                                                      ip_dst = IP_DST,
                                                      ip_dst_mask = IP_MASK)
        self.verifyPacketOnPort(PKT, EXP_PKT, PORT_IN, port_list[PORT_REDIRECT])
        print "Destination IPv4 address redirected to redirect port"

@group('acl')
class testAclSrcMacRedirect(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclSrcMacRedirect, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                       in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                       redirect_port = port_list[PORT_REDIRECT],
                                                       mac_src = rewrite_mac1)
        self.verifyPacketOnPort(PKT, EXP_PKT, PORT_IN, port_list[PORT_REDIRECT])
        print "Source MAC address redirected to redirect port"

@group('acl')
class testAclDstMacRedirect(sai_base_test.ThriftInterfaceDataPlane, aclTestBase):
    def runTestFunc(self):
        super(testAclDstMacRedirect, self).runTestFunc(stage = SAI_ACL_STAGE_INGRESS,
                                                       in_ports = [port_list[PORT_OUT], port_list[PORT_IN]],
                                                       redirect_port = port_list[PORT_REDIRECT],
                                                       mac_dst = rewrite_mac2)
        self.verifyPacketOnPort(PKT, EXP_PKT, PORT_IN, [PORT_REDIRECT])
        print "Destination MAC address redirected to redirect port"

