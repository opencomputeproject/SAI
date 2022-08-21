from sai_test_base import T0TestBase
from sai_utils import *


class NoHostRouteTest(T0TestBase):
    """
    Verify if no_host is false, neighbor can be used directly for forwarding.
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def runTest(self):
        '''
        1. Use existing LAG1 ``host`` (no_host = false) neighbor 
        2. Check vlan interface(svi added)
        3. Send packet on port5 with ``DMAC: SWITCH_MAC`` and using LAG1 neighbor IP as dest IP
        4. verify packet received on one of LAG1 member
        '''
        print("\nHostNeighborTest")

        print("Sending IPv4 packet when host route not exists")
        self.ipv4_addr = self.t1_list[1][100].ipv4
        self.mac_addr =  self.t1_list[1][100].mac
        self.dev_port1 = self.dut.port_obj_list[5].dev_port_index

        pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                ip_dst=self.ipv4_addr,
                                ip_ttl=64)

        send_packet(self, self.dev_port1, pkt)
        verify_no_other_packets(self)
        print("Packet dropped")

    def tearDown(self):
        """
        TearDown process
        """
        super().tearDown()


class NoHostRouteTestV6(T0TestBase):
    """
    Verifies if IPv6 host route is not created according to
    SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute value
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

        self.dev_port1 = self.dut.port_obj_list[1].dev_port_index
        self.ipv6_addr = "2001:0db8::1:10"
        self.mac_addr = "00:10:10:10:10:10"
        self.nbr_entry_v6 = sai_thrift_neighbor_entry_t(
            rif_id=self.dut.lag_list[0].rif_list[0],
            ip_address=sai_ipaddress(self.ipv6_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            self.nbr_entry_v6,
            dst_mac_address=self.mac_addr,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def noHostRouteNeighborTestV6(self):
        '''
        Add a neighbor for IP 2001:0db8::1:10 on the LAG1 Route interface and a new MACX
        Send packet on port5 with DMAC: SWITCH_MAC DIP:2001:0db8::1:10
        verify packet received on one of LAG1 member
        '''
        print("\nnoHostRouteNeighborTestv6()")

        print("Sending IPv4 packet when host route not exists")

        pkt = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                  ipv6_dst=self.ipv6_addr,
                                  ipv6_hlim=64)

        send_packet(self, self.dev_port1, pkt)
        verify_no_other_packets(self)
        print("Packet dropped")

    def runTest(self):
        try:
            self.noHostRouteNeighborTestV6()
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_remove_neighbor_entry(self.client, self.nbr_entry_v6)
        super().tearDown()


class AddHostRouteTest(T0TestBase):
    """
    Verifies if IPv4 host route is created according to
    SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute value
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

        self.dev_port1 = self.dut.port_obj_list[1].dev_port_index
        self.ipv4_addr = "10.1.1.10"
        self.mac_addr = "00:10:10:10:10:10"
        self.nbr_entry_v4 = sai_thrift_neighbor_entry_t(
            rif_id=self.dut.lag_list[0].rif_list[0],
            ip_address=sai_ipaddress(self.ipv4_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            self.nbr_entry_v4,
            dst_mac_address=self.mac_addr,
            no_host_route=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def addHostRouteNeighborTest(self):
        '''
        Add a neighbor for IP 10.1.1.10 on the LAG1 Route interface, set NO_HOST_ROUTE=True
        Send packet with on port5 with DMAC: SWITCH_MAC DIP:10.1.1.10
        Verify no packet was received on any port
        '''
        print("\naddHostRouteIpv4NeighborTest()")
        self.ipv4_addr = self.t1_list[1][100].ipv4
        self.mac_addr =  self.t1_list[1][100].mac

        pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                ip_dst=self.ipv4_addr,
                                ip_ttl=64)

        exp_pkt = simple_udp_packet(eth_dst=self.mac_addr,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=self.ipv4_addr,
                                    ip_ttl=63)

        print("Sending IPv4 packet when host route exists")
        send_packet(self, self.dev_port1, pkt)
        verify_packet_any_port(self, exp_pkt, self.dut.lag_list[0].member_port_indexs)
        print("Packet forwarded")

    def runTest(self):
        try:
            self.addHostRouteNeighborTest()
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_remove_neighbor_entry(self.client, self.nbr_entry_v4)
        super().tearDown()


class AddHostRouteTestV6(T0TestBase):
    """
    Verifies if IPv6 host route is created according to
    SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute value
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

        self.dev_port1 = self.dut.port_obj_list[1].dev_port_index
        self.ipv6_addr = "2001:0db8::1:10"
        self.mac_addr = "00:10:10:10:10:10"
        self.nbr_entry_v6 = sai_thrift_neighbor_entry_t(
            rif_id=self.dut.lag_list[0].rif_list[0],
            ip_address=sai_ipaddress(self.ipv6_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            self.nbr_entry_v6,
            dst_mac_address=self.mac_addr,
            no_host_route=False)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

    def addHostRouteNeighborTestV6(self):
        '''
        Add a neighbor for IP 2001:0db8::1:10 on the LAG1 Route interface, set NO_HOST_ROUTE=True
        Send packet with on port5 with DMAC: SWITCH_MAC DIP:2001:0db8::1:10
        Verify no packet was received on any port
        '''
        print("\naddHostRouteNeighborTestV6()")

        exp_pkt_v6 = simple_udpv6_packet(eth_dst=self.mac_addr,
                                         eth_src=ROUTER_MAC,
                                         ipv6_dst=self.ipv6_addr,
                                         ipv6_hlim=63)

        pkt_v6 = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                     ipv6_dst=self.ipv6_addr,
                                     ipv6_hlim=64)

        print("Sending IPv4 packet when host route exists")
        send_packet(self, self.dev_port1, pkt_v6)
        verify_packet_any_port(
            self, exp_pkt_v6, self.dut.lag_list[0].member_port_indexs)
        print("Packet forwarded")

    def runTest(self):
        try:
            self.addHostRouteNeighborTestV6()
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_remove_neighbor_entry(self.client, self.nbr_entry_v6)
        super().tearDown()


class RemoveAddNeighborTestIPV4(T0TestBase):
    """
    Verifies if IPv4 host route is not created according to
    SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE attribute value
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

        self.dev_port1 = self.dut.port_obj_list[1].dev_port_index
        self.ipv4_addr = "10.1.1.10"
        self.mac_addr = "00:10:10:10:10:10"

        self.nbr_entry_v4 = sai_thrift_neighbor_entry_t(
            rif_id=self.dut.lag_list[0].rif_list[0],
            ip_address=sai_ipaddress(self.ipv4_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            self.nbr_entry_v4,
            dst_mac_address=self.mac_addr)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.net_route = sai_thrift_route_entry_t(
            vr_id=self.dut.default_vrf, destination=sai_ipprefix(self.ipv4_addr+'/32'))
        sai_thrift_create_route_entry(
            self.client, self.net_route, next_hop_id=self.dut.lag_list[0].rif_list[0])
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

    def RemoveAddNeighborTestV4(self):
        '''
        Check the config, make sure the CPU queue0 is created and neighbor(NO_HOST_ROUTE=True) for LAGs already created
        Create route interface for LAG1:rifx
        Create a route for DIP:10.1.1.10/32 through the new rifx
        Send packet for DIP:10.1.1.10 DMAC: SWITCH_MAC on port5
        verify packet received with SMAC: SWITCH_MAC SIP 192.168.0.1 DIP:10.1.1.10 on one of LAG1 member
        Delete the neighbor for IP:10.1.1.10
        Send packet for DIP:10.1.1.10 DMAC: SWITCH_MAC on port5
        Verify no packet on any port
        Verify the CPU queue0 get one more item
        Add the neighbor for IP:10.1.1.10 on LAG1 again
        Send packet for DIP:10.1.1.10 DMAC: SWITCH_MAC on port5
        verify packet received with SMAC: SWITCH_MAC SIP 192.168.0.1 DIP:10.1.1.10 on one of LAG1 member
        '''
        print("\nRemoveAddNeighborTest()")

        print("Sending IPv4 packet when host route not exists")

        pkt = simple_udp_packet(eth_dst=ROUTER_MAC,
                                ip_dst=self.ipv4_addr,
                                ip_ttl=64)

        exp_pkt = simple_udp_packet(eth_dst=self.mac_addr,
                                    eth_src=ROUTER_MAC,
                                    ip_dst=self.ipv4_addr,
                                    ip_ttl=63)

        send_packet(self, self.dev_port1, pkt)
        verify_packet_any_port(self, exp_pkt, self.dut.lag_list[0].member_port_indexs)

        sai_thrift_remove_neighbor_entry(self.client, self.nbr_entry_v4)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        pre_cpu_queue_state = sai_thrift_get_queue_stats(self.client, self.cpu_queue0)[
            "SAI_QUEUE_STAT_PACKETS"]
        send_packet(self, self.dev_port1, pkt)
        verify_no_other_packets(self)
        print("Packet dropped")
        post_cpu_queue_state = sai_thrift_get_queue_stats(
            self.client, self.cpu_queue0)["SAI_QUEUE_STAT_PACKETS"]
        #self.assertEqual(post_cpu_queue_state - pre_cpu_queue_state, 1)
        # Disable above check beacuse of bug 15205360
        print(str(post_cpu_queue_state - pre_cpu_queue_state))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.nbr_entry_v4,
            dst_mac_address=self.mac_addr)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        send_packet(self, self.dev_port1, pkt)
        verify_packet_any_port(self, exp_pkt, self.dut.lag_list[0].member_port_indexs)

    def runTest(self):
        try:
            self.RemoveAddNeighborTestV4()
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_remove_route_entry(self.client, self.net_route)
        sai_thrift_remove_neighbor_entry(self.client, self.nbr_entry_v4)
        super().tearDown()


class RemoveAddNeighborTestIPV6(T0TestBase):
    """
    Verify trap to cpu if remove neighbor for a rif directly binded route, and add back will recover the functionality
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

        self.dev_port1 = self.dut.port_obj_list[1].dev_port_index
        self.ipv6_addr = "2001:0db8::1:10"
        self.mac_addr = "00:10:10:10:10:10"

        self.nbr_entry_v6 = sai_thrift_neighbor_entry_t(
            rif_id=self.dut.lag_list[0].rif_list[0],
            ip_address=sai_ipaddress(self.ipv6_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            self.nbr_entry_v6,
            dst_mac_address=self.mac_addr)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.net_route = sai_thrift_route_entry_t(
            vr_id=self.dut.default_vrf, destination=sai_ipprefix(self.ipv6_addr+'/128'))
        sai_thrift_create_route_entry(
            self.client, self.net_route, next_hop_id=self.dut.lag_list[0].rif_list[0])
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

    def RemoveAddNeighborTestV6(self):
        '''
        Check the config, make sure the CPU queue0 is created and neighbor for LAGs already created
        Create route interface for LAG1:rifx
        Send packet for DIP:10.1.1.10 DMAC: SWITCH_MAC on port1
        verify packet received with SMAC: SWITCH_MAC  DIP:2001:0db8::1:10 on one of LAG1 member
        Delete the neighbor for IP:2001:0db8::1:10
        Send packet for DIP:2001:0db8::1:10 DMAC: SWITCH_MAC on port1
        Verify no packet on any port
        Verify the CPU queue0 get one more item
        Add the neighbor for IP:2001:0db8::1:10 on LAG1 again
        Send packet for DIP:10.1.1.10 DMAC: SWITCH_MAC on port1
        verify packet received with SMAC: SWITCH_MAC  DIP:2001:0db8::1:10 on one of LAG1 member
        '''
        print("\nRemoveAddNeighborTest()")

        print("Sending IPv6 packet when host route not exists")

        exp_pkt_v6 = simple_udpv6_packet(eth_dst=self.mac_addr,
                                         eth_src=ROUTER_MAC,
                                         ipv6_dst=self.ipv6_addr,
                                         ipv6_hlim=63)

        pkt_v6 = simple_udpv6_packet(eth_dst=ROUTER_MAC,
                                     ipv6_dst=self.ipv6_addr,
                                     ipv6_hlim=64)
        send_packet(self, self.dev_port1, pkt_v6)
        verify_packet_any_port(
            self, exp_pkt_v6, self.dut.lag_list[0].member_port_indexs)

        sai_thrift_remove_neighbor_entry(self.client, self.nbr_entry_v6)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        pre_cpu_queue_state = sai_thrift_get_queue_stats(self.client, self.cpu_queue0)[
            "SAI_QUEUE_STAT_PACKETS"]
        send_packet(self, self.dev_port1, pkt_v6)
        verify_no_other_packets(self)
        print("Packet dropped")
        post_cpu_queue_state = sai_thrift_get_queue_stats(
            self.client, self.cpu_queue0)["SAI_QUEUE_STAT_PACKETS"]
        #self.assertEqual(post_cpu_queue_state - pre_cpu_queue_state, 1)
        # Disable above check beacuse of bug 15205360
        print(str(post_cpu_queue_state - pre_cpu_queue_state))
        sai_thrift_create_neighbor_entry(
            self.client,
            self.nbr_entry_v6,
            dst_mac_address=self.mac_addr)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        send_packet(self, self.dev_port1, pkt_v6)
        verify_packet_any_port(
            self, exp_pkt_v6, self.dut.lag_list[0].member_port_indexs)

    def runTest(self):
        try:
            self.RemoveAddNeighborTestV6()
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_remove_route_entry(self.client, self.net_route)
        sai_thrift_remove_neighbor_entry(self.client, self.nbr_entry_v6)
        super().tearDown()


class NhopDiffPrefixRemoveLonger(T0TestBase):
    """
    Verify nexthop add and delete with different ipaddress prefix (add and delete the prefix longer than the first one) 
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

        self.ipv4_addr = "10.1.1.10"
        self.mac_addr1 = "00:10:10:10:10:10"
        self.mac_addr2 = "00:20:20:20:20:20"

    def test_neighbor_diff_prefix_add_remove_longer(self):
        '''
        Add nhop with ipprefix length 16 which contains the IPaddress for LAG1 Neighbor (exist one in common config)
        Add nhop with ipprefix length 24 which contains the IPaddress for LAG1 Neighbor (exist one in common config)
        Delete new created nhop with ipprefix length 24
        Delete new created nhop with ipprefix length 16
        '''
        self.nbr_entry_v4 = sai_thrift_neighbor_entry_t(
            rif_id=self.dut.lag_list[0].rif_list[0],
            ip_address=sai_ipaddress(self.ipv4_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            self.nbr_entry_v4,
            dst_mac_address=self.mac_addr1,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.subnet_nhop_16 = sai_thrift_create_next_hop(self.client, ip=sai_ipprefix(
            self.ipv4_addr + '/16'), router_interface_id=self.dut.lag_list[0].rif_list[0], type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.subnet_nhop_24 = sai_thrift_create_next_hop(self.client, ip=sai_ipprefix(
            self.ipv4_addr + '/24'), router_interface_id=self.dut.lag_list[0].rif_list[0], type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        sai_thrift_remove_next_hop(self.client, self.subnet_nhop_24)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        sai_thrift_remove_next_hop(self.client, self.subnet_nhop_16)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

    def runTest(self):
        try:
            self.test_neighbor_diff_prefix_add_remove_longer()
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_remove_neighbor_entry(self.client, self.nbr_entry_v4)
        super().tearDown()


class NhopDiffPrefixRemoveLongerV6(T0TestBase):
    """
    Verify nexthop add and delete with different ipaddress prefix (add and delete the prefix longer than the first one)  
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

        self.ipv6_addr = "2001:0db8::1:10"
        self.mac_addr1 = "00:10:10:10:10:10"
        self.mac_addr2 = "00:20:20:20:20:20"

    def test_neighbor_diff_prefix_add_remove_longer_v6(self):
        '''
        Add nhop with ipprefix length 64 which contains the IPaddress for LAG1 Neighbor (exist one in common config)
        Add nhop with ipprefix length 128 which contains the IPaddress for LAG1 Neighbor (exist one in common config)
        Delete new created nhop with ipprefix length 128
        Delete new created nhop with ipprefix length 64
        '''
        self.nbr_entry_128 = sai_thrift_neighbor_entry_t(
            rif_id=self.dut.lag_list[0].rif_list[0],
            ip_address=sai_ipprefix(self.ipv6_addr + '/128'))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            self.nbr_entry_128,
            dst_mac_address=self.mac_addr2)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.subnet_nhop_64 = sai_thrift_create_next_hop(self.client, ip=sai_ipprefix(
            self.ipv6_addr + '/64'), router_interface_id=self.dut.lag_list[0].rif_list[0], type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.subnet_nhop_128 = sai_thrift_create_next_hop(self.client, ip=sai_ipprefix(
            self.ipv6_addr + '/128'), router_interface_id=self.dut.lag_list[0].rif_list[0], type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        sai_thrift_remove_next_hop(self.client, self.subnet_nhop_128)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        sai_thrift_remove_next_hop(self.client, self.subnet_nhop_64)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

    def runTest(self):
        try:
            self.test_neighbor_diff_prefix_add_remove_longer_v6()
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_remove_neighbor_entry(self.client, self.nbr_entry_128)
        super().tearDown()


class NhopDiffPrefixRemoveShorter(T0TestBase):
    """
     Verify nexthop add and delete with different ipaddress prefix (add and delete the prefix shorter than the first one)   
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

        self.ipv4_addr = "10.1.1.10"
        self.mac_addr1 = "00:10:10:10:10:10"
        self.mac_addr2 = "00:20:20:20:20:20"

    def test_neighbor_diff_prefix_add_remove_shorter(self):
        '''
        Add nhop with ipprefix length 24 which contains the IPaddress for LAG1 Neighbor (exist one in common config)
        Add nhop with ipprefix length 16 which contains the IPaddress for LAG1 Neighbor (exist one in common config)
        Delete new created nhop with ipprefix length 16
        Delete new created nhop with ipprefix length 24
        '''

        self.nbr_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.dut.lag_list[0].rif_list[0],
            ip_address=sai_ipaddress(self.ipv4_addr))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            self.nbr_entry,
            dst_mac_address=self.mac_addr1,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        self.subnet_nhop_24 = sai_thrift_create_next_hop(self.client, ip=sai_ipprefix(
            self.ipv4_addr + '/24'), router_interface_id=self.dut.lag_list[0].rif_list[0], type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.subnet_nhop_16 = sai_thrift_create_next_hop(self.client, ip=sai_ipprefix(
            self.ipv4_addr + '/16'), router_interface_id=self.dut.lag_list[0].rif_list[0], type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        sai_thrift_remove_next_hop(self.client, self.subnet_nhop_16)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        sai_thrift_remove_next_hop(self.client, self.subnet_nhop_24)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

    def runTest(self):
        try:
            self.test_neighbor_diff_prefix_add_remove_shorter()
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_remove_neighbor_entry(self.client, self.nbr_entry)
        super().tearDown()


class NhopDiffPrefixRemoveShorterV6(T0TestBase):
    """
     Verify nexthop add and delete with different ipaddress prefix (add and delete the prefix shorter than the first one)   
    """

    def setUp(self):
        """
        Set up test.
        """
        T0TestBase.setUp(self)

        self.ipv6_addr = "2001:0db8::1:10"
        self.mac_addr1 = "00:10:10:10:10:10"
        self.mac_addr2 = "00:20:20:20:20:20"

    def test_neighbor_diff_prefix_add_remove_shorter_v6(self):
        '''
        Add nhop with ipprefix length 128 which contains the IPaddress for LAG1 Neighbor (exist one in common config)
        Add nhop with ipprefix length 64 which contains the IPaddress for LAG1 Neighbor (exist one in common config)
        Delete new created nhop with ipprefix length 64
        Delete new created nhop with ipprefix length 128
        '''

        self.nbr_entry_128 = sai_thrift_neighbor_entry_t(
            rif_id=self.dut.lag_list[0].rif_list[0],
            ip_address=sai_ipprefix(self.ipv6_addr + '/128'))
        status = sai_thrift_create_neighbor_entry(
            self.client,
            self.nbr_entry_128,
            dst_mac_address=self.mac_addr1,
            no_host_route=True)
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        self.subnet_nhop_128 = sai_thrift_create_next_hop(self.client, ip=sai_ipprefix(
            self.ipv6_addr + '/128'), router_interface_id=self.dut.lag_list[0].rif_list[0], type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        self.subnet_nhop_64 = sai_thrift_create_next_hop(self.client, ip=sai_ipprefix(
            self.ipv6_addr + '/64'), router_interface_id=self.dut.lag_list[0].rif_list[0], type=SAI_NEXT_HOP_TYPE_IP)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        sai_thrift_remove_next_hop(self.client, self.subnet_nhop_64)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

        sai_thrift_remove_next_hop(self.client, self.subnet_nhop_128)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)

    def runTest(self):
        try:
            self.test_neighbor_diff_prefix_add_remove_shorter_v6()
        finally:
            pass

    def tearDown(self):
        """
        TearDown process
        """
        sai_thrift_remove_neighbor_entry(self.client, self.nbr_entry_128)
        super().tearDown()
