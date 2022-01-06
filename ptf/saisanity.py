
from __future__ import print_function

from sai_thrift.sai_headers import *

from ptf.testutils import *
from ptf.dataplane import DataPlane

from sai_base_test import *

mac1 = '00:11:11:11:11:11'
mac2 = '00:22:22:22:22:22'
mac3 = '00:33:33:33:33:33'
mac4 = '00:12:12:12:12:13'


class L2TrunkToTrunkVlan(PlatformSaiHelper):

    def remove_bridge_port(self):
        for index in range(0, len(self.port_list)):
            port_bp = getattr(self, 'port%s_bp' % index)
            sai_thrift_remove_bridge_port(self.client, port_bp)


    def create_bridge_ports(self):
        for index in range(0, len(self.port_list)):
            port_id = getattr(self, 'port%s' % index)
            port_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=port_id,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            setattr(self, 'port%s_bp' % index, port_bp)
            self.assertNotEqual(getattr(self, 'port%s_bp' % index), 0)
            #attr = self.get_bridge_port_all_attribute(port_bp)
            #setattr(self, 'port%s_bp_attr' % index, attr)

    def setUp(self):
        #this process contains the switch_init process
        SaiHelperBase.setUp(self)

        self.create_bridge_ports()

        print("Sending L2 packet port 1 -> port 2 [access vlan=10])")
        self.vlan_id = 10        
        
        mac_action = SAI_PACKET_ACTION_FORWARD

        self.vlan_oid = sai_thrift_create_vlan(self.client, vlan_id=self.vlan_id)
        self.assertNotEqual(self.vlan_oid, 0)
        self.vlan_member1 = sai_thrift_create_vlan_member(
            self.client, vlan_id=self.vlan_oid, bridge_port_id=self.port0_bp, vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
        self.vlan_member2 = sai_thrift_create_vlan_member(
            self.client, vlan_id=self.vlan_oid, bridge_port_id=self.port1_bp, vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_port_attribute(self.client, port_oid=self.port0, port_vlan_id=self.vlan_id)
        sai_thrift_set_port_attribute(self.client, port_oid=self.port1, port_vlan_id=self.vlan_id)

        self.fdb_entry1 = sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac1, bv_id=self.vlan_oid)
        self.fdb_entry2 = sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac2, bv_id=self.vlan_oid)

        #need the bridge port inactually
        sai_thrift_create_fdb_entry(self.client, fdb_entry=self.fdb_entry1, bridge_port_id=self.port0_bp, packet_action=mac_action)
        sai_thrift_create_fdb_entry(self.client, fdb_entry=self.fdb_entry2, bridge_port_id=self.port1_bp, packet_action=mac_action)


    def runTest(self):
        pkt = simple_tcp_packet(eth_dst='00:22:22:22:22:22',
                                eth_src=mac4,
                                ip_dst='10.0.0.1',
                                ip_id=101,
                                ip_ttl=64)
        try:
            time.sleep(5)
            send_packet(self, 0, pkt)
            verify_packet(self, pkt, 1)
        finally:
            pass

    
    def tearDown(self):
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry1)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry2)

        sai_thrift_set_port_attribute(self.client, port_oid=self.port0, port_vlan_id=1)
        sai_thrift_set_port_attribute(self.client, port_oid=self.port1, port_vlan_id=1)

        sai_thrift_remove_vlan_member(self.client,self.vlan_member1)
        sai_thrift_remove_vlan_member(self.client,self.vlan_member2)
        sai_thrift_remove_vlan(self.client, self.vlan_oid)
        self.remove_bridge_port()
        #TODO resove the error for fdb entry not equals to init one
        SaiHelperBase.tearDown(self)


class L2TrunkToAccessVlan(PlatformSaiHelper):

    def remove_bridge_port(self):
        for index in range(0, len(self.port_list)):
            port_bp = getattr(self, 'port%s_bp' % index)
            sai_thrift_remove_bridge_port(self.client, port_bp)


    def create_bridge_ports(self):
        for index in range(0, len(self.port_list)):
            port_id = getattr(self, 'port%s' % index)
            port_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=port_id,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            setattr(self, 'port%s_bp' % index, port_bp)
            self.assertNotEqual(getattr(self, 'port%s_bp' % index), 0)
            #attr = self.get_bridge_port_all_attribute(port_bp)
            #setattr(self, 'port%s_bp_attr' % index, attr)

    def setUp(self):
        #this process contains the switch_init process
        SaiHelperBase.setUp(self)

        self.create_bridge_ports()

        print("Sending L2 packet port 1 -> port 2 [trunk vlan=10])")
        self.vlan_id = 10        
        
        mac_action = SAI_PACKET_ACTION_FORWARD

        self.vlan_oid = sai_thrift_create_vlan(self.client, vlan_id=self.vlan_id)
        self.assertNotEqual(self.vlan_oid, 0)
        self.vlan_member1 = sai_thrift_create_vlan_member(
            self.client, vlan_id=self.vlan_oid, bridge_port_id=self.port0_bp, vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)
        self.vlan_member2 = sai_thrift_create_vlan_member(
            self.client, vlan_id=self.vlan_oid, bridge_port_id=self.port1_bp, vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

        sai_thrift_set_port_attribute(self.client, port_oid=self.port0, port_vlan_id=self.vlan_id)
        sai_thrift_set_port_attribute(self.client, port_oid=self.port1, port_vlan_id=self.vlan_id)

        self.fdb_entry1 = sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac1, bv_id=self.vlan_oid)
        self.fdb_entry2 = sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac2, bv_id=self.vlan_oid)

        #need the bridge port inactually
        sai_thrift_create_fdb_entry(self.client, fdb_entry=self.fdb_entry1, bridge_port_id=self.port0_bp, packet_action=mac_action)
        sai_thrift_create_fdb_entry(self.client, fdb_entry=self.fdb_entry2, bridge_port_id=self.port1_bp, packet_action=mac_action)
        

    def runTest(self):
        pkt = simple_udp_packet(eth_dst=mac2,
                                eth_src=mac4,
                                dl_vlan_enable=True,
                                vlan_vid=10,
                                ip_dst='172.16.0.1',
                                ip_id=102,
                                ip_ttl=64)
        exp_pkt = simple_udp_packet(eth_dst=mac2,
                                    eth_src=mac4,
                                    ip_dst='172.16.0.1',
                                    ip_id=102,
                                    ip_ttl=64,
                                    pktlen=96)
        try:
            time.sleep(5)
            send_packet(self, 0, pkt)
            verify_packet(self, exp_pkt, 1)
        finally:
            pass

    
    def tearDown(self):
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry1)
        sai_thrift_remove_fdb_entry(self.client, self.fdb_entry2)

        sai_thrift_set_port_attribute(self.client, port_oid=self.port0, port_vlan_id=1)
        sai_thrift_set_port_attribute(self.client, port_oid=self.port1, port_vlan_id=1)

        sai_thrift_remove_vlan_member(self.client,self.vlan_member1)
        sai_thrift_remove_vlan_member(self.client,self.vlan_member2)
        sai_thrift_remove_vlan(self.client, self.vlan_oid)
        self.remove_bridge_port()
        #TODO resove the error for fdb entry not equals to init one
        SaiHelperBase.tearDown(self)


class L2SanityTest(PlatformSaiHelper):
    def gen_mac(self):
         #Gets self.portX objects for all active ports
        for index in range(0, len(self.port_list)):
            mac = "00"
            if index < 9 :
                section = ":" + "0" + str(index+1)
            else:
                section= ":" + str(index+1)
            mac += (section*5)
            setattr(self, 'mac%s' % index, mac)

    
    def create_bridge_ports(self):
        for index in range(0, len(self.port_list)):
            port_id = getattr(self, 'port%s' % index)
            port_bp = sai_thrift_create_bridge_port(
                self.client,
                bridge_id=self.default_1q_bridge,
                port_id=port_id,
                type=SAI_BRIDGE_PORT_TYPE_PORT,
                admin_state=True)
            setattr(self, 'port%s_bp' % index, port_bp)
            self.assertNotEqual(getattr(self, 'port%s_bp' % index), 0)
            attr = self.get_bridge_port_all_attribute(port_bp)
            setattr(self, 'port%s_bp_attr' % index, attr)


    def create_vlan_ports(self, vlanid, vlan_oid):
        for index in range(0, len(self.port_list)):
            port_bp = getattr(self, 'port%s_bp' % index)
            if index%2 == 0:
                vlan_member = sai_thrift_create_vlan_member(
                    self.client,
                    vlan_id=self.vlan_oid,
                    bridge_port_id=port_bp,
                    vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
            else:
                vlan_member = sai_thrift_create_vlan_member(
                    self.client,
                    vlan_id=self.vlan_oid,
                    bridge_port_id=port_bp,
                    vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_TAGGED)

            setattr(self, 'vlan%s_member%s' % (vlanid, index), vlan_member)


    def set_port_vlan(self, vlan_id):
        for index in range(0, len(self.port_list)):
            port_id=getattr(self, 'port%s' % index)
            sai_thrift_set_port_attribute(self.client, port_id, port_vlan_id=vlan_id)


    def create_port_fdb(self, vlan_id, vlan_oid, mac_action):
        for index in range(0, len(self.port_list)):
            mac=getattr(self, 'mac%s' % index)
            port_bp = getattr(self, 'port%s_bp' % index)
            fdb_entry = sai_thrift_fdb_entry_t(
                switch_id=self.switch_id, mac_address=mac, bv_id=vlan_oid)
            sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=port_bp,
                packet_action=mac_action)
            setattr(self, 'fdb_entry%s' % index, fdb_entry)


    def create_pkt(self, vlan_id):
        for index in range(0, len(self.port_list)):
            target_mac = getattr(self, 'mac%s' % index)
            if index%2 == 0:                
                pkt = simple_tcp_packet(eth_src=self.src_mac,
                                        eth_dst=target_mac,
                                        ip_dst='172.16.0.1',
                                        ip_id=101,
                                        ip_ttl=64)                
            else:
                pkt = simple_tcp_packet(eth_dst=target_mac,
                                        eth_src=self.src_mac,
                                        dl_vlan_enable=True,
                                        vlan_vid=vlan_id,
                                        ip_dst='172.16.0.1',
                                        ip_id=102,
                                        ip_ttl=64)
            setattr(self, 'pkt%s' % index, pkt)

    def create_exp_pkt(self, vlan_id):
        for index in range(0, len(self.port_list)):
            target_mac = getattr(self, 'mac%s' % index)
            if index%2 == 0:                
                exp_pkt = getattr(self, 'pkt%s' % index)                
            else:
                exp_pkt = simple_tcp_packet(eth_dst=target_mac,
                                        eth_src=self.src_mac,
                                        ip_dst='172.16.0.1',
                                        ip_id=102,
                                        dl_vlan_enable=True,
                                        vlan_vid=vlan_id,
                                        ip_ttl=64)
            setattr(self, 'exp_pkt%s' % index, exp_pkt)


    def setUp(self):
        #Init switch
        SaiHelperBase.setUp(self)

        mac4=  '00:12:12:12:12:13'

        self.vlan_id = 10
        self.gen_mac()
        self.src_mac=mac4
        mac_action = SAI_PACKET_ACTION_FORWARD

        self.src_port = self.port0
        self.dst_port = self.port1

        self.create_bridge_ports()

        # create vlan 10 with ports
        self.vlan_oid = sai_thrift_create_vlan(self.client, vlan_id=self.vlan_id)
        self.assertNotEqual(self.vlan_oid, 0)
        self.create_vlan_ports(self.vlan_id, self.vlan_oid)

        #set port vlan attribute
        self.set_port_vlan(self.vlan_id)

        #set fdb
        self.create_port_fdb(self.vlan_id, self.vlan_oid, mac_action)

        #create send pkt and rcv pkt
        self.create_pkt(self.vlan_id)
        self.create_exp_pkt(self.vlan_id)


    def runTest(self):
        try:            
            for index in range(1, len(self.port_list)):
                self.dataplane.flush()
                print("Check port{} forwarding...".format(index))
                target_pkt = getattr(self, 'pkt%s' % index)
                exp_pkt = getattr(self, 'exp_pkt%s' % index)                
                send_packet(self, self.dev_port0, target_pkt)                
                verify_packet(self, exp_pkt, index) 
                #res = verify_any_packet_on_ports_list(self, [exp_pkt], [[i for i in range(0,32)]])
        finally:
            pass

    
    def tearDown(self):
        #reset port vlan id
        #?reset to 0 or 1?
        self.reset_port_vlan(1)

        #remove fdb
        self.remove_fdb()

        #remove vlan member
        self.remove_vlan_member(self.vlan_id)        

        #remove bridge port
        self.remove_bridge_port()

        sai_thrift_remove_vlan(self.client, self.vlan_oid)
        SaiHelperBase.tearDown(self)

    def reset_port_vlan(self, reset_vlan_id):
        for index in range(0, len(self.port_list)):
            port_id=getattr(self, 'port%s' % index)
            sai_thrift_set_port_attribute(self.client, port_id, port_vlan_id=reset_vlan_id)

    def remove_fdb(self):
        for index in range(0, len(self.port_list)):
            fdb_entry=getattr(self, 'fdb_entry%s' % index)
            sai_thrift_remove_fdb_entry(self.client, fdb_entry)


    def remove_vlan_member(self, vlan_id):
        for index in range(0, len(self.port_list)):
            vlan_member=getattr(self, 'vlan%s_member%s' % (vlan_id, index))
            sai_thrift_remove_vlan_member(self.client, vlan_member)

    def remove_bridge_port(self):
        for index in range(0, len(self.port_list)):
            port_bp = getattr(self, 'port%s_bp' % index)
            sai_thrift_remove_bridge_port(self.client, port_bp)




###############################################################################
# Helper functions                                                            #
# Hack for now, need to be removed or upstreamed                              #
###############################################################################
# pylint: disable=dangerous-default-value,too-many-arguments
def verify_any_packet_on_ports_list(
        test,
        pkts=[],
        ports=[],
        device_number=0,
        timeout=2,
        no_flood=False):
    """
    Ports is list of port lists
    Check that _any_ packet is received atleast once in every sublist in
    ports belonging to the given device (default device_number is 0).

    Also verifies that the packet is ot received on any other ports for this
    device, and that no other packets are received on the device
    (unless --relax is in effect).
    Args:
        test (testcase): Test case
        pkts (packets): list of packets
        ports (ports): list of ports
        device_number (int): device under test
        timeout (int): timeout
        no_flood (int): do not flood
    Returns:
        rcv_idx: list of port indices
    """
    rcv_idx = []
    failures = {}
    pkt_cnt = 0
    for port_list in ports:
        rcv_ports = set()
        remaining_timeout = timeout
        port_sub_list_failures = []
        port_sub_list_poll_success = False
        if remaining_timeout > 0:
            port_idx = 0
            port_list_len = len(port_list)
            while remaining_timeout > 0 or port_idx > 0:
                port = port_list[port_idx]
                port_idx = (port_idx + 1) % port_list_len
                remaining_timeout = remaining_timeout - 0.1
                (rcv_device, rcv_port, rcv_pkt, _) = test.dataplane.poll(
                    port_number=port, timeout=0.1, filters=get_filters())
                print("port {} received, received port id: {}".format(port, rcv_port))
                if rcv_device != device_number:
                    continue
                for pkt in pkts:
                    logging.debug("Checking for pkt on device %d, port %d",
                                  device_number, port)
                    if ptf.dataplane.match_exp_pkt(pkt, rcv_pkt):
                        pkt_cnt += 1
                        rcv_ports.add(port_list.index(rcv_port))
                        port_sub_list_failures = []
                        port_sub_list_poll_success = True
                        break
                    else:
                        port_sub_list_failures.append(
                            (port, DataPlane.PollFailure(pkt, [rcv_pkt], 1)))
                if port_sub_list_poll_success and no_flood:
                    break
        # Either no expected packets received or unexpected packets recieved
        if not port_sub_list_poll_success or port_sub_list_failures:
            port_tuple = tuple(port_list)
            failures.setdefault(port_tuple, [])
            failures[port_tuple] = failures[port_tuple] + \
                port_sub_list_failures
        rcv_idx.append(rcv_ports)

    verify_no_other_packets(test)
    if failures:
        def format_per_port_failure(port, failure):
            # pylint: disable=missing-return-doc,missing-return-type-doc
            """
            Format one port
            Args:
              port (int): port
              failure (str): message
            """
            return "On port {}\n{}".format(port, failure.format())

        def format_per_port_failures(fail_list):
            # pylint: disable=missing-return-doc,missing-return-type-doc
            """
            Format one port all failures
            Args:
              fail_list (array): port and failures
            """
            return "\n".join([format_per_port_failure(port, failure)
                              for (port, failure) in fail_list])

        def format_port_list_failures(port_list, fail_list):
            # pylint: disable=missing-return-doc,missing-return-type-doc
            """
            Format all port all failures
            Args:
              port_list (array): ports
              fail_list (array): port and failures
            """
            return "None of the exp pkts rx'd for port list {}: \n{}".format(
                port_list, format_per_port_failures(fail_list))
        failure_report = "\n".join([
            format_port_list_failures(port_list, fail_list)
            for port_list, fail_list in list(failures.items())
        ])
        test.fail(
            "Did not receive expected packets on any of {} for device {}. \n{}"
            .format(ports, device_number, failure_report))

    test.assertTrue(
        pkt_cnt >= len(ports),
        "Did not receive pkt on one of ports %r for device %d" %
        (ports, device_number))
    return rcv_idx


def set_vlan_data(vlan_id=0, ports=None, untagged=None, large_port=0):
    """
    Creates dictionary with vlan data

    Args:
        vlan_id (int): VLAN ID number
        ports (list): ports numbers
        untagged (list): list of untagged ports
        large_port (int): the largest port in vlan

    Return:
        dictionary: vlan_data_dict
    """
    vlan_data_dict = {
        "vlan_id": vlan_id,
        "ports": ports,
        "untagged": untagged,
        "large_port": large_port
    }
    return vlan_data_dict

