from sai_test_base import T0TestBase
from sai_utils import *
from ptf.mask import Mask
class IpInIpTnnnelBase(T0TestBase):
    '''
    This class contains base setup for IP in IP tunnel tests
    '''
    def setUp(self,
              ttl_mode=SAI_TUNNEL_TTL_MODE_PIPE_MODEL,
              peer_mode=SAI_TUNNEL_PEER_MODE_P2MP,
              packet_loop_action=None,
              decap_ecn_mode=None,
              encap_ecn_mode=None):
        
        T0TestBase.setUp(self, 
                         is_create_tunnel= True,
                         peer_mode= peer_mode,
                         packet_loop_action=packet_loop_action,
                         decap_ecn_mode=decap_ecn_mode,
                         encap_ecn_mode=encap_ecn_mode)

        tunnel_config = self.dut.tunnel_list[-1]
        self.tunnel = tunnel_config
        self.oport_dev = self.dut.port_obj_list[1].dev_port_index
        self.uport_dev = self.dut.port_obj_list[18].dev_port_index
        self.tun_ip = tunnel_config.tun_ips[0]
        self.lpb_ip = tunnel_config.lpb_ips[0]

        self.recv_dev_port_idxs = self.get_dev_port_indexes(self.servers[11][1].l3_lag_obj.member_port_indexs)
        self.vm_ip = "100.100.1.1"
        self.vm_ipv6 = "2001:0db8::100:1"
        self.customer_ip = self.servers[1][1].ipv4
        self.customer_ipv6 = self.servers[1][1].ipv6
        self.inner_dmac = tunnel_config.inner_dmac
        self.customer_mac = self.servers[1][1].mac
        self.unbor_mac = self.servers[11][1].l3_lag_obj.neighbor_mac
        
        tunnel_config.create_tunnel_route(self, vm_ip=self.vm_ip, vm_ipv6=self.vm_ipv6)
        
    def tearDown(self):
        T0TestBase.tearDown(self)

class BasicIPInIPTunnelEncapv4Inv4Test(IpInIpTnnnelBase):
    """
    We will send normal packet from port1 and verify that packet goes into tunnel, getting a ininip packet, recievinfg a encap packet on lag1.
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        IpInIpTnnnelBase.setUp(self)
        self.oport_dev = self.dut.port_obj_list[0].dev_port_index


    def ipv4inipv4encap(self):
        """
        Generate ingress normal packet as decribed by Testing Data Packet
        Send normal packet from port1.
        expected encap packet as decribed by Testing Data Packet.
        Recieve encap packet from lag1, compare it with expected encap packet.
        """
       
        pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.customer_mac,
                                ip_dst=self.vm_ip,
                                ip_src=self.customer_ip,
                                ip_id=108,
                                ip_ttl=64,
                                )
        inner_pkt = simple_udp_packet(eth_dst=self.inner_dmac,
                                      eth_src=ROUTER_MAC,
                                      ip_dst=self.vm_ip,
                                      ip_src=self.customer_ip,
                                      ip_id=108,
                                      ip_ttl=63)


        ipip_pkt = simple_ipv4ip_packet(eth_dst=self.unbor_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_id=0,
                                            ip_src=self.lpb_ip,
                                            ip_dst=self.tun_ip,
                                            ip_ttl= 63,
                                            inner_frame=inner_pkt['IP'])
   
        print("Verifying IP4inIP4 encapsulation")
        m = Mask(ipip_pkt)
        m.set_do_not_care_scapy(ptf.packet.IP, "len")
        m.set_do_not_care_scapy(ptf.packet.IP, "chksum")
        m.set_do_not_care_scapy(ptf.packet.IP, "flags")
        
        self.dataplane.flush()
        send_packet(self, self.oport_dev, pkt)
        verify_packet_any_port(self, m, self.recv_dev_port_idxs)
        
        print("\tOK")

        
    def runTest(self):
        try:
            self.ipv4inipv4encap()
        finally:
            pass

class BasicIPInIPTunnelEncapv6Inv4Test(IpInIpTnnnelBase):
    """
    We will send normal packet from port1 and verify that packet goes into tunnel, getting a ininip packet, recievinfg a encap packet on lag1.
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        IpInIpTnnnelBase.setUp(self)
        self.oport_dev = self.dut.port_obj_list[0].dev_port_index

    def ipv6inipv4encap(self):
        """
        Generate ingress normal packet as decribed by Testing Data Packet
        Send normal packet from port1.
        expected encap packet as decribed by Testing Data Packet
        Recieve encap packet from lag1, compare it with expected encap packet.
        """
       
        pkt = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                  eth_src=self.customer_mac,
                                  ipv6_dst=self.vm_ipv6,
                                  ipv6_src=self.customer_ipv6,
                                  ipv6_hlim=64)

        inner_pkt = simple_udpv6_packet(eth_dst=self.inner_dmac,
                                        eth_src=ROUTER_MAC,
                                        ipv6_dst=self.vm_ipv6,
                                        ipv6_src=self.customer_ipv6,
                                        ipv6_hlim=63)

        ipip_pkt = simple_ipv4ip_packet(eth_dst=self.unbor_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_id=0,
                                            ip_src=self.lpb_ip,
                                            ip_dst=self.tun_ip,
                                            ip_ttl= 63,
                                            inner_frame=inner_pkt['IPv6'])
                                            
        print("Verifying IP6inIP4 encapsulation")
        m = Mask(ipip_pkt)
        m.set_do_not_care_scapy(ptf.packet.IP, "len")
        m.set_do_not_care_scapy(ptf.packet.IP, "chksum")
        m.set_do_not_care_scapy(ptf.packet.IP, "plen")     
        m.set_do_not_care_scapy(ptf.packet.IP, "flags")
        self.dataplane.flush()
        send_packet(self, self.oport_dev, pkt)
        verify_packet_any_port(self, m, self.recv_dev_port_idxs)
        print("\tOK")


    def runTest(self):
        try:
            self.ipv6inipv4encap()
        finally:
            pass

class BasicIPInIPTunnelDecapv4Inv4Test(IpInIpTnnnelBase):
    """
    We will send ipinip packet from lag1 and verify getting inner packet by matching tunnel term table entry, recieving the inner packet on port1 by  matching route entry
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        IpInIpTnnnelBase.setUp(self)
        
        self.oport_dev = self.dut.port_obj_list[0].dev_port_index
        self.orif = self.dut.port_obj_list[0].rif_list[-1]
        self.customer_ip = "100.100.2.1"
        self.customer_mac = "00:22:24:22:22:11"

        self.onbor = sai_thrift_neighbor_entry_t(
            rif_id=self.orif, ip_address=sai_ipaddress(self.customer_ip))
        sai_thrift_create_neighbor_entry(self.client,
                                         self.onbor,
                                         dst_mac_address=self.customer_mac,
                                         no_host_route=True)

        self.onhop = sai_thrift_create_next_hop(
            self.client, ip=sai_ipaddress(self.customer_ip),
            router_interface_id=self.orif, type=SAI_NEXT_HOP_TYPE_IP)

        self.customer_route = sai_thrift_route_entry_t(
            vr_id=self.dut.default_vrf,
            destination=sai_ipprefix(self.customer_ip + '/32'))

        sai_thrift_create_route_entry(self.client,
                                      self.customer_route,
                                      next_hop_id=self.onhop)

    def ipv4inipv4decap(self):
        """
        Generate ingress ipinip packet as decribed by Testing Data Packet
        Send encap packet from lag1.
        Generate expected decap packet as decribed by Testing Data Packet.
        Recieve decap packet from port1, compare it with expected decap packet.
        """
        pkt = simple_udp_packet(eth_dst=self.customer_mac,
                                eth_src=ROUTER_MAC,
                                ip_dst=self.customer_ip,
                                ip_src=self.vm_ip,
                                ip_id=108,
                                ip_ttl=63)
        inner_pkt = simple_udp_packet(eth_dst=self.inner_dmac,
                                      eth_src=ROUTER_MAC,
                                      ip_dst=self.customer_ip,
                                      ip_src=self.vm_ip,
                                      ip_id=108,
                                      ip_ttl=64)

        ipip_pkt = simple_ipv4ip_packet(eth_dst=ROUTER_MAC,
                                            eth_src=self.unbor_mac,
                                            ip_id=0,
                                            ip_src=self.tun_ip,
                                            ip_dst=self.lpb_ip,
                                            inner_frame=inner_pkt['IP'])

        print("Verifying IP4inIP4 decapsulation")
        send_packet(self, self.uport_dev, ipip_pkt)
        verify_packet(self, pkt, self.oport_dev)
        print("\tOK")
        
    def runTest(self):
        try:
            self.ipv4inipv4decap()
        finally:
            pass


class SviIPInIPTunnelDecapv4Inv4Test(IpInIpTnnnelBase):
    """
    We will send ipinip packet from lag1 and verify getting inner packet by matching tunnel term table entry, recieving the inner packet on port1 by  matching route entry
                              |  tunnel table      |                  |  route table      |
    ingress ipinip pkt->lag2->|term dst: 10.10.10.1|->   inner pkt -> |192.168.1.0/24,SVI:VLAN10|
                              |term src: 10.1.2.100|   
 
    ->nb ->|  fdb table             |->Port1   
           |00:01:01:99:01:a0, port1|        
   
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        IpInIpTnnnelBase.setUp(self)

    def ipv4inipv4decap(self):
        """
        Generate ingress ipinip packet as decribed by Testing Data Packet
        Send encap packet from lag1.
        Generate expected decap packet as decribed by Testing Data Packet.
        Recieve decap packet from port1, compare it with expected decap packet.
        """
        pkt = simple_udp_packet(eth_dst=self.customer_mac,
                                eth_src=ROUTER_MAC,
                                ip_dst=self.customer_ip,
                                ip_src=self.vm_ip,
                                ip_id=108,
                                ip_ttl=63)
        inner_pkt = simple_udp_packet(eth_dst=self.inner_dmac,
                                      eth_src=ROUTER_MAC,
                                      ip_dst=self.customer_ip,
                                      ip_src=self.vm_ip,
                                      ip_id=108,
                                      ip_ttl=64)

        ipip_pkt = simple_ipv4ip_packet(eth_dst=ROUTER_MAC,
                                            eth_src=self.unbor_mac,
                                            ip_id=0,
                                            ip_src=self.tun_ip,
                                            ip_dst=self.lpb_ip,
                                            inner_frame=inner_pkt['IP'])

        print("Verifying IP4inIP4 decapsulation")
        send_packet(self, self.uport_dev, ipip_pkt)
        verify_packet(self, pkt, self.oport_dev)
        print("\tOK")
        
    def runTest(self):
        try:
            self.ipv4inipv4decap()
        finally:
            pass

class SviIPInIPTunnelDecapV6InV4Test(IpInIpTnnnelBase):
    """
    We will send ipinip packet from lag1 and verify getting inner packet by matching tunnel term table entry, recieving the inner packet on port2 by  matching svi route entry (192.168.1.0/24,SVI:VLAN10), querying neighbor table and fdb entry (00:01:01:99:01:a0, port1).
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        IpInIpTnnnelBase.setUp(self)

    def ipv6inipv4decap(self):
        """
        Generate ingress ipinip packet as decribed by Testing Data Packet
        Send encap packet from lag1.
        Generate expected decap packet as decribed by Testing Data Packet.
        Recieve decap packet from port1, compare it with expected decap packet.
        """
        pkt = simple_udpv6_packet(eth_dst=self.customer_mac,
                                  eth_src=ROUTER_MAC,
                                  ipv6_dst=self.customer_ipv6,
                                  ipv6_src=self.vm_ipv6,
                                  ipv6_hlim=63)

        inner_pkt = simple_udpv6_packet(eth_dst=self.inner_dmac,
                                        eth_src=ROUTER_MAC,
                                        ipv6_dst=self.customer_ipv6,
                                        ipv6_src=self.vm_ipv6,
                                        ipv6_hlim=64)

        ipip_pkt = simple_ipv4ip_packet(eth_dst=ROUTER_MAC,
                                            eth_src=self.unbor_mac,
                                            ip_id=0,
                                            ip_src=self.tun_ip,
                                            ip_dst=self.lpb_ip,
                                            inner_frame=inner_pkt['IPv6'])


        print("Verifying IPinIP6 decapsulation")
        send_packet(self, self.uport_dev, ipip_pkt)
        verify_packet(self, pkt, self.oport_dev)
        print("\tOK")

    def runTest(self):
        try:
            self.ipv6inipv4decap()
        finally:
            pass


class SviIPInIPTunnelEncapv4Inv4Test(IpInIpTnnnelBase):
    """
    We will send normal packet from SVI:VLAN10 member of port1 and verify that packet goes into tunnel, getting a ininip packet, recievinfg a encap packet on lag1.
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        IpInIpTnnnelBase.setUp(self)
        
    def ipv4inipv4encap(self):
        """
        Generate ingress normal packet as decribed by Testing Data Packet
        Send normal packet from port1.
        expected encap packet as decribed by Testing Data Packet.
        Recieve encap packet from lag1, compare it with expected encap packet.
        """
       
        pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.customer_mac,
                                ip_dst=self.vm_ip,
                                ip_src=self.customer_ip,
                                ip_id=108,
                                ip_ttl=64,
                                )
        inner_pkt = simple_udp_packet(eth_dst=self.inner_dmac,
                                      eth_src=ROUTER_MAC,
                                      ip_dst=self.vm_ip,
                                      ip_src=self.customer_ip,
                                      ip_id=108,
                                      ip_ttl=63)


        ipip_pkt = simple_ipv4ip_packet(eth_dst=self.unbor_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_id=0,
                                            ip_src=self.lpb_ip,
                                            ip_dst=self.tun_ip,
                                            ip_ttl= 63,
                                            inner_frame=inner_pkt['IP'])
   
        print("Verifying IP4inIP4 encapsulation")
        m = Mask(ipip_pkt)
        m.set_do_not_care_scapy(ptf.packet.IP, "len")
        m.set_do_not_care_scapy(ptf.packet.IP, "chksum")
        m.set_do_not_care_scapy(ptf.packet.IP, "flags")
        
        self.dataplane.flush()
        send_packet(self, self.oport_dev, pkt)
        verify_packet_any_port(self, m, self.recv_dev_port_idxs)
        
        print("\tOK")

        
    def runTest(self):
        try:
            self.ipv4inipv4encap()
        finally:
            pass

class SviIPInIPTunnelEncapv6Inv4Test(IpInIpTnnnelBase):
    """
    We will send normal packet from SVI:VLAN10 member of port1 and verify that packet goes into tunnel, getting a ininip packet, recievinfg a encap packet on lag1.
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        IpInIpTnnnelBase.setUp(self)
       
    def ipv6inipv4encap(self):
        """
        Generate ingress normal packet as decribed by Testing Data Packet
        Send normal packet from port1.
        expected encap packet as decribed by Testing Data Packet
        Recieve encap packet from lag1, compare it with expected encap packet.
        """
       
        pkt = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                  eth_src=self.customer_mac,
                                  ipv6_dst=self.vm_ipv6,
                                  ipv6_src=self.customer_ipv6,
                                  ipv6_hlim=64)

        inner_pkt = simple_udpv6_packet(eth_dst=self.inner_dmac,
                                        eth_src=ROUTER_MAC,
                                        ipv6_dst=self.vm_ipv6,
                                        ipv6_src=self.customer_ipv6,
                                        ipv6_hlim=63)

        ipip_pkt = simple_ipv4ip_packet(eth_dst=self.unbor_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_id=0,
                                            ip_src=self.lpb_ip,
                                            ip_dst=self.tun_ip,
                                            ip_ttl= 63,
                                            inner_frame=inner_pkt['IPv6'])
                                            
        print("Verifying IP6inIP4 encapsulation")
        m = Mask(ipip_pkt)
        m.set_do_not_care_scapy(ptf.packet.IP, "len")
        m.set_do_not_care_scapy(ptf.packet.IP, "chksum")
        m.set_do_not_care_scapy(ptf.packet.IP, "plen")     
        m.set_do_not_care_scapy(ptf.packet.IP, "flags")
        self.dataplane.flush()
        send_packet(self, self.oport_dev, pkt)
        verify_packet_any_port(self, m, self.recv_dev_port_idxs)
        print("\tOK")


    def runTest(self):
        try:
            self.ipv6inipv4encap()
        finally:
            pass



class IPInIPTunnelEncapPipeTtlv4Inv4Test(IpInIpTnnnelBase):
    """
    This verifies the TTL field is preserved end-to-end by copying into the outer header on encapsulation and copying from the outer header on decapsulation when using TTL unifrom mode.     
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        IpInIpTnnnelBase.setUp(self)
        
    def encap_ttl_set_pipe_mode_v4(self):
        """
        1.Generate input packet with ip_ttl field as 64.
        2.Send input packet from port1.
        3.Create Expected ipinip packet with 50 field in outer ip header as ttl_val,inner ip_ttl as 63.
        4.Recieve ipinip packet from lag2, compare it with expected ipinip packet.
        """
       
        pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.customer_mac,
                                ip_dst=self.vm_ip,
                                ip_src=self.customer_ip,
                                ip_id=108,
                                ip_ttl=64,
                                )
        inner_pkt = simple_udp_packet(eth_dst=self.inner_dmac,
                                      eth_src=ROUTER_MAC,
                                      ip_dst=self.vm_ip,
                                      ip_src=self.customer_ip,
                                      ip_id=108,
                                      ip_ttl=63)


        ipip_pkt = simple_ipv4ip_packet(eth_dst=self.unbor_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_id=0,
                                            ip_src=self.lpb_ip,
                                            ip_dst=self.tun_ip,
                                            ip_ttl= TTL_VAL,
                                            inner_frame=inner_pkt['IP'])
   
        m = Mask(ipip_pkt)
        m.set_do_not_care_scapy(ptf.packet.IP, "len")
        m.set_do_not_care_scapy(ptf.packet.IP, "chksum")
        m.set_do_not_care_scapy(ptf.packet.IP, "flags")
        
        self.dataplane.flush()
        send_packet(self, self.oport_dev, pkt)
        verify_packet_any_port(self, m, self.recv_dev_port_idxs)
        
        print("\tOK")

        
    def runTest(self):
        self.encap_ttl_set_pipe_mode_v4()


class IPInIPTunnelDecapPipeTtlv4Inv4Test(IpInIpTnnnelBase):
    """
    This verifies if TTL field is user-defined for outer header on encapsulation 
    and TTL field of inner header remains the same on decapsulation when using TTL pipe mode.
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        IpInIpTnnnelBase.setUp(self)
        self.ttl_val = TTL_VAL
    def decap_ttl_set_pipe_mode_v4(self):
        """
        1.Generate input ipinip packet with ip_ttl field in outer ip header as 64 , one in inner ip header as 51, expected recieved packet with ip_ttl field as 50.
        2.Send packet from lag1.
        3.Recieve ipinip packet from port1, compare it with expected packet.
        """
        pkt = simple_udp_packet(eth_dst=self.customer_mac,
                                eth_src=ROUTER_MAC,
                                ip_dst=self.customer_ip,
                                ip_src=self.vm_ip,
                                ip_id=108,
                                ip_ttl=self.ttl_val-1)
        inner_pkt = simple_udp_packet(eth_dst=self.inner_dmac,
                                      eth_src=ROUTER_MAC,
                                      ip_dst=self.customer_ip,
                                      ip_src=self.vm_ip,
                                      ip_id=108,
                                      ip_ttl=self.ttl_val)

        ipip_pkt = simple_ipv4ip_packet(eth_dst=ROUTER_MAC,
                                            eth_src=self.unbor_mac,
                                            ip_id=0,
                                            ip_src=self.tun_ip,
                                            ip_dst=self.lpb_ip,
                                            ip_ttl=64,
                                            inner_frame=inner_pkt['IP'])


        send_packet(self, self.uport_dev, ipip_pkt)
        verify_packet(self, pkt, self.oport_dev)
        print("\tOK")
        
    def runTest(self):
        try:
            self.decap_ttl_set_pipe_mode_v4()
        finally:
            pass


class SviIPInIPTunnelDecapFloodv4Inv4Test(IpInIpTnnnelBase):
    """
    Verify removing fdb entry and expecting flood or drop on all ports of vlan 10.
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        IpInIpTnnnelBase.setUp(self)
        for item in self.dut.fdb_entry_list:
            sai_thrift_remove_fdb_entry(self.client, item)
            self.dut.fdb_entry_list.remove(item)
            
    def ipip_tunnel_decap_svi_flood_test(self):
        """
        1.Remove vlan10 fdb entry (dstmac=00:01:01:99:01:a0, port=port1);.
        2.Generate ingress ipinip packet as decribed by Testing Data Packet
        3.Send encap packet from lag1.
        4.Check packet drops or flood on port1.
        """
        pkt = simple_udp_packet(eth_dst=self.customer_mac,
                                eth_src=ROUTER_MAC,
                                ip_dst=self.customer_ip,
                                ip_src=self.vm_ip,
                                ip_id=108,
                                ip_ttl=63)
        inner_pkt = simple_udp_packet(eth_dst=self.inner_dmac,
                                      eth_src=ROUTER_MAC,
                                      ip_dst=self.customer_ip,
                                      ip_src=self.vm_ip,
                                      ip_id=108,
                                      ip_ttl=64)

        ipip_pkt = simple_ipv4ip_packet(eth_dst=ROUTER_MAC,
                                            eth_src=self.unbor_mac,
                                            ip_id=0,
                                            ip_src=self.tun_ip,
                                            ip_dst=self.lpb_ip,
                                            inner_frame=inner_pkt['IP'])

        send_packet(self, self.uport_dev, ipip_pkt)
        vlan10_ports = self.get_dev_port_indexes(self.dut.vlans[10].port_idx_list)
        verify_each_packet_on_multiple_port_lists(self, [pkt], [self.get_dev_port_indexes(
                list(filter(lambda item: item != 1, self.dut.vlans[10].port_idx_list)))])
        
        print("\tOK")
        
    def runTest(self):
        try:
            self.ipip_tunnel_decap_svi_flood_test()
        finally:
            pass


class SviIPInIPTunnelDecapFloodV6InV4Test(IpInIpTnnnelBase):
    """
    Verify removing fdb entry and expecting flood or drop on all ports of vlan 10.
    """

    def setUp(self):
        """
        Test the basic setup process.
        """
        IpInIpTnnnelBase.setUp(self)
        for item in self.dut.fdb_entry_list:
            sai_thrift_remove_fdb_entry(self.client, item)
            self.dut.fdb_entry_list.remove(item)
            
    def ipip_tunnel_decap_svi_flood_test_v6(self):
        """
        1.Remove vlan10 fdb entry (dstmac=00:01:01:99:01:a0, port=port1);.
        2.Generate ingress ipinip packet as decribed by Testing Data Packet
        3.Send encap packet from lag1.
        4.Check packet drops or flood on port1.
        """
        pkt = simple_udpv6_packet(eth_dst=self.customer_mac,
                                  eth_src=ROUTER_MAC,
                                  ipv6_dst=self.customer_ipv6,
                                  ipv6_src=self.vm_ipv6,
                                  ipv6_hlim=63)

        inner_pkt = simple_udpv6_packet(eth_dst=self.inner_dmac,
                                        eth_src=ROUTER_MAC,
                                        ipv6_dst=self.customer_ipv6,
                                        ipv6_src=self.vm_ipv6,
                                        ipv6_hlim=64)

        ipip_pkt = simple_ipv4ip_packet(eth_dst=ROUTER_MAC,
                                            eth_src=self.unbor_mac,
                                            ip_id=0,
                                            ip_src=self.tun_ip,
                                            ip_dst=self.lpb_ip,
                                            inner_frame=inner_pkt['IPv6'])

        send_packet(self, self.uport_dev, ipip_pkt)
        vlan10_ports = self.get_dev_port_indexes(self.dut.vlans[10].port_idx_list)
        verify_each_packet_on_multiple_port_lists(self, [pkt], [self.get_dev_port_indexes(
                list(filter(lambda item: item != 1, self.dut.vlans[10].port_idx_list)))])
        print("\tOK")

    def runTest(self):
        try:
            self.ipip_tunnel_decap_svi_flood_test_v6()
        finally:
            pass
        
class PeerModeTest(IpInIpTnnnelBase):
    '''
    This class test SAI_TUNNEL_PEER_MODE_P2P
    '''
    def setUp(self):
        peer_mode =  SAI_TUNNEL_PEER_MODE_P2P
        IpInIpTnnnelBase.setUp(self, peer_mode=SAI_TUNNEL_PEER_MODE_P2P)

    def runTest(self):
        pkt = simple_udp_packet(eth_dst=self.customer_mac,
                                eth_src=ROUTER_MAC,
                                ip_dst=self.customer_ip,
                                ip_src=self.vm_ip,
                                ip_id=108,
                                ip_ttl=63)
        inner_pkt = simple_udp_packet(eth_dst=self.inner_dmac,
                                      eth_src=ROUTER_MAC,
                                      ip_dst=self.customer_ip,
                                      ip_src=self.vm_ip,
                                      ip_id=108,
                                      ip_ttl=64)

        ipip_pkt = simple_ipv4ip_packet(eth_dst=ROUTER_MAC,
                                            eth_src=self.unbor_mac,
                                            ip_id=0,
                                            ip_src=self.tun_ip,
                                            ip_dst=self.lpb_ip,
                                            inner_frame=inner_pkt['IP'])

        print("Verifying IP4inIP4 decapsulation")
        send_packet(self, self.uport_dev, ipip_pkt)
        verify_packet(self, pkt, self.oport_dev)
        print("\tOK")
        
    def tearDown(self):
        T0TestBase.tearDown(self)

class LoopBackPacketActionTest(IpInIpTnnnelBase):
    '''
    This class test SAI_TUNNEL_PEER_MODE_P2P
    '''
    def setUp(self):
        action = SAI_PACKET_ACTION_DROP
        IpInIpTnnnelBase.setUp(self, packet_loop_action=action)

    def runTest(self):
        
        pkt = simple_udp_packet(eth_dst=self.customer_mac,
                                eth_src=ROUTER_MAC,
                                ip_dst=self.customer_ip,
                                ip_src=self.vm_ip,
                                ip_id=108,
                                ip_ttl=63)
        inner_pkt = simple_udp_packet(eth_dst=self.inner_dmac,
                                      eth_src=ROUTER_MAC,
                                      ip_dst=self.customer_ip,
                                      ip_src=self.vm_ip,
                                      ip_id=108,
                                      ip_ttl=64)

        ipip_pkt = simple_ipv4ip_packet(eth_dst=ROUTER_MAC,
                                            eth_src=self.unbor_mac,
                                            ip_id=0,
                                            ip_src=self.tun_ip,
                                            ip_dst=self.lpb_ip,
                                            inner_frame=inner_pkt['IP'])

        print("Verifying IP4inIP4 decapsulation")
        send_packet(self, self.uport_dev, ipip_pkt)
        verify_packet(self, pkt, self.oport_dev)
        print("\tOK")
        
    def tearDown(self):
        T0TestBase.tearDown(self)

class DecapEcnModeTest(IpInIpTnnnelBase):
    '''
    This class test decap_ecn_mode
    '''
    def setUp(self):
        decap_ecn = SAI_TUNNEL_DECAP_ECN_MODE_COPY_FROM_OUTER
        IpInIpTnnnelBase.setUp(self, decap_ecn_mode=decap_ecn)

    def runTest(self):
        pkt = simple_udp_packet(eth_dst=self.customer_mac,
                                eth_src=ROUTER_MAC,
                                ip_dst=self.customer_ip,
                                ip_src=self.vm_ip,
                                ip_id=108,
                                ip_ttl=63)
        inner_pkt = simple_udp_packet(eth_dst=self.inner_dmac,
                                      eth_src=ROUTER_MAC,
                                      ip_dst=self.customer_ip,
                                      ip_src=self.vm_ip,
                                      ip_id=108,
                                      ip_ttl=64)

        ipip_pkt = simple_ipv4ip_packet(eth_dst=ROUTER_MAC,
                                            eth_src=self.unbor_mac,
                                            ip_id=0,
                                            ip_src=self.tun_ip,
                                            ip_dst=self.lpb_ip,
                                            inner_frame=inner_pkt['IP'])

        print("Verifying IP4inIP4 decapsulation")
        send_packet(self, self.uport_dev, ipip_pkt)
        verify_packet(self, pkt, self.oport_dev)
        print("\tOK")
        
    def tearDown(self):
        T0TestBase.tearDown(self)

class EncapEcnModeTest(IpInIpTnnnelBase):
    '''
    This class test encap_ecn_mode
    '''
    def setUp(self):
        encap_ecn = SAI_TUNNEL_DECAP_ECN_MODE_COPY_FROM_OUTER
        IpInIpTnnnelBase.setUp(self, encap_ecn_mode=SAI_TUNNEL_ENCAP_ECN_MODE_STANDARD)

    def runTest(self):
        pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                eth_src=self.customer_mac,
                                ip_dst=self.vm_ip,
                                ip_src=self.customer_ip,
                                ip_id=108,
                                ip_ttl=64,
                                )
        inner_pkt = simple_udp_packet(eth_dst=self.inner_dmac,
                                      eth_src=ROUTER_MAC,
                                      ip_dst=self.vm_ip,
                                      ip_src=self.customer_ip,
                                      ip_id=108,
                                      ip_ttl=63)


        ipip_pkt = simple_ipv4ip_packet(eth_dst=self.unbor_mac,
                                            eth_src=ROUTER_MAC,
                                            ip_id=0,
                                            ip_src=self.lpb_ip,
                                            ip_dst=self.tun_ip,
                                            ip_ttl= 63,
                                            inner_frame=inner_pkt['IP'])
   
        print("Verifying IP4inIP4 encapsulation")
        m = Mask(ipip_pkt)
        m.set_do_not_care_scapy(ptf.packet.IP, "len")
        m.set_do_not_care_scapy(ptf.packet.IP, "chksum")
        m.set_do_not_care_scapy(ptf.packet.IP, "flags")
        
        self.dataplane.flush()
        send_packet(self, self.oport_dev, pkt)
        verify_packet_any_port(self, m, self.recv_dev_port_idxs)
        
        print("\tOK")
        
    def tearDown(self):
        T0TestBase.tearDown(self)
