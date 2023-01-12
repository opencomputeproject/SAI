from sai_test_base import T0TestBase
from sai_utils import *
from ptf.mask import Mask
class IpInIpTnnnelBase(T0TestBase):
    '''
    This class contains base setup for IP in IP tunnel tests
    '''


    def setUp(self):
        T0TestBase.setUp(self)

        self.oport = self.dut.port_id_list[1]
        self.oport_dev = self.dut.port_obj_list[1].dev_port_index
        self.uport_dev = self.dut.port_obj_list[18].dev_port_index

        self.tun_ip = self.servers[11][1].ipv4
        self.lpb_ip = LOOPBACK_IPV4
        self.tun_lpb_mask = "/32"

        self.recv_dev_port_idxs = self.get_dev_port_indexes(self.servers[11][1].l3_lag_obj.member_port_indexs)
        self.vm_ip = "100.100.1.1"
        self.vm_ipv6 = "2001:0db8::100:1"
        self.customer_ip = self.servers[1][1].ipv4
        self.customer_ipv6 = self.servers[1][1].ipv6
        self.inner_dmac = "00:41:11:14:11:14"
        self.customer_mac = self.servers[1][1].mac
        self.unbor_mac = self.servers[11][1].l3_lag_obj.neighbor_mac
 
        # underlay configuration
        self.uvrf = self.dut.default_vrf

        # overlay configuration
        self.ovrf = self.dut.default_vrf
        tunnel_type = SAI_TUNNEL_TYPE_IPINIP
        term_type = SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P

        # loopback RIFs for tunnel
        self.urif_lpb = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.uvrf)

        self.orif_lpb = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_LOOPBACK,
            virtual_router_id=self.ovrf)

        # tunnel
        self.tunnel = sai_thrift_create_tunnel(
            self.client,
            type=tunnel_type,
            encap_src_ip=sai_ipaddress(self.lpb_ip),
            underlay_interface=self.urif_lpb,
            overlay_interface=self.orif_lpb)

        # tunnel termination entry
        self.tunnel_term = sai_thrift_create_tunnel_term_table_entry(
            self.client,
            tunnel_type=tunnel_type,
            vr_id=self.uvrf,
            action_tunnel_id=self.tunnel,
            type=term_type,
            dst_ip=sai_ipaddress(self.lpb_ip),
            src_ip=sai_ipaddress(self.tun_ip))

        # tunnel nexthop for VM
        self.tunnel_nhop = sai_thrift_create_next_hop(
            self.client,
            type=SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP,
            tunnel_id=self.tunnel,
            ip=sai_ipaddress(self.tun_ip),
            tunnel_mac=self.inner_dmac)

        # routes to VM via tunnel nexthop
        self.vm_route = sai_thrift_route_entry_t(
            vr_id=self.ovrf, destination=sai_ipprefix(self.vm_ip + '/32'))
        sai_thrift_create_route_entry(self.client,
                                      self.vm_route,
                                      next_hop_id=self.tunnel_nhop)

        self.vm_v6_route = sai_thrift_route_entry_t(
            vr_id=self.ovrf, destination=sai_ipprefix(self.vm_ipv6 + '/128'))
        sai_thrift_create_route_entry(self.client,
                                      self.vm_v6_route,
                                      next_hop_id=self.tunnel_nhop)

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
        self.oport = self.dut.port_id_list[0]
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
        self.oport = self.dut.port_id_list[0]
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
        
        self.oport = self.dut.port_id_list[0]
        self.oport_dev = self.dut.port_obj_list[0].dev_port_index
        self.orif = self.dut.port_obj_list[0].rif_list[-1]
        self.customer_ip = "100.100.2.1"
        self.customer_mac = "00:22:24:22:22:11"
        
        import pdb
        pdb.set_trace()

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
            vr_id=self.ovrf,
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
