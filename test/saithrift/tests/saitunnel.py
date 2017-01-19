# (C) Copyright @ 2016 Broadcom.  All Rights Reserved.  The term Broadcom refers to Broadcom Limited and/or its subsidiaries.
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
Thrift SAI interface Tunnel tests
"""
import socket

from switch import *

import sai_base_test

@group("tunnel")
class IpIpEncapTest(sai_base_test.ThriftInterfaceDataPlane):
    '''
    performs encapsulation 
    we send normal ip packet on port2 and expected to receive encapsulated packet on port 1
    Underlay route 10.10.1.x
    inner packet dest ip:1.1.1.1 
    outer packet dest ip:10.10.1.1
    -----------------------------------------------------------------
    Ingress side[port2]           |          Egress side[port1]
    ------------------------------------------------------------------
    ip's falls in 20.20.1.0       |        ip's falls in 10.10.1.0
    ------------------------------------------------------------------
    TTL MODE : SAI_TUNNEL_TTL_PIPE_MODEL
    ##Ingress packet=Ether(dst=router_mac)/IP(src=20.20.1.x,dst=1.1.1.1)/TCP()
    ##Expected encap packet = Ether(src=router_mac,dst=00:05:06:00:00:00)/IP(src=20.20.1.1,dst=1.1.1.1)/IP(src=2.2.2.2,dst=10.10.1.1)/TCP()
    '''
    def runTest(self):
        print
        switch_init(self.client)
        port1=port_list[0] #From where we expect encapsulated packet 
        port2=port_list[1] #From where we send normal ip packet
        mac1 = '00:05:06:00:00:00' # port1 l2 entry
        mac2 = '00:05:06:00:00:02' 
        vr_mac='00:00:08:08:08:08'
        mac_action=1                
        vlan_id=0            #expecting to use unused vlans
        addr_family=0 #1 for v6
        tunnel_src_ip_addr='2.2.2.2' #src for outer ip
        encap_ip_addr='10.10.1.1'    #egress enighbhor
        ingress_ip_addr='20.20.1.1'  #ingress neighbor 
        tunnel_ip_addr_route='1.1.1.0'  #to reach tunnel nhop
        tunnel_ip_mask_route='255.255.255.0'
        ingress_nhop_ip_addr='20.20.1.1'
        egress_nhop_ip_addr='10.10.1.1'
        ip_addr_ingress_route='20.20.1.0'
        ip_mask_ingress_route='255.255.255.0'
        ip_addr_encap_route='10.10.1.0'
        ip_mask_encap_route='255.255.255.0'
        initiator_ip_addr='10.10.1.1'  #tunnel dest ip(outer header)
        neighbor_mac_ingress='00:05:06:00:00:02'
        neighbor_mac_encap='00:05:06:00:00:00' #egress side
        ip_addr_decap_src='10.10.1.1'
        ip_addr_decap_dst='2.2.2.2'
               
        ########################################################################
        #Creating Virtual router 
        #######################################################################
        vr_id=sai_thrift_create_virtual_router(self.client, 1, 1, vr_mac)
            
        #creating a underlay interface in loopback
        underlay_if = sai_thrift_create_router_interface(self.client, is_port=1, port_id=0, vr_id=vr_id, vlan_id=vlan_id, v4_enabled=1, v6_enabled=1, mac='', is_lb=1)
        #creating an overlay interface in loopback
        overlay_if = sai_thrift_create_router_interface(self.client, is_port=1, port_id=0, vr_id=vr_id, vlan_id=vlan_id, v4_enabled=1, v6_enabled=1, mac='', is_lb=1)
        #creating a tunnel
        tunnel_id=sai_thrift_create_tunnel(self.client, addr_family=addr_family, ip_addr=tunnel_src_ip_addr, underlay_if=underlay_if, overlay_if=overlay_if)
      
        ##############################################################################
        #  Egress configurations
        #  create router interface,
        #  create neighbor
        #  create nhop
        #  create route
        #   
        ##############################################################################
       
        #encap router interface
        encap_if_id=sai_thrift_create_router_interface(self.client, vr_id=vr_id, is_port=1, port_id=port1, vlan_id=vlan_id, v4_enabled = 1, v6_enabled = 1, mac='', is_lb=0)
        #egress(encap side) neighbor (ip=10.10.1.1 , mac=00:05:06:00:00:00 )
        sai_thrift_create_neighbor(self.client, addr_family=addr_family, rif_id=encap_if_id, ip_addr=encap_ip_addr, dmac=neighbor_mac_encap)
        #egress(encap) nhop and route create
        egress_nhop=sai_thrift_create_nhop(self.client, addr_family=addr_family, ip_addr=egress_nhop_ip_addr, rif_id=encap_if_id, is_tunnel=0)
        sai_thrift_create_route(self.client, vr_id=vr_id, addr_family=addr_family, ip_addr=ip_addr_encap_route, ip_mask=ip_mask_encap_route, nhop=egress_nhop)
        
        ###############################################################################
        #  Ingress configurations
        #  create router interface,
        #  create neighbor
        #  create nhop
        #  create route
        #   
        ##############################################################################
         
        #ingress router interface
        ingress_if_id=sai_thrift_create_router_interface(self.client,vr_id=vr_id,is_port=1,port_id=port2,vlan_id=vlan_id,v4_enabled=1,v6_enabled = 1, mac='', is_lb=0)
        #ingress neighbor neighbor (ip=20.20.1.1 , mac = 00:05:06:00:00:02)
        sai_thrift_create_neighbor(self.client, addr_family=addr_family, rif_id=ingress_if_id, ip_addr=ingress_ip_addr, dmac=neighbor_mac_ingress)
        #ingress nhop and route create
        ingress_nhop=sai_thrift_create_nhop(self.client, addr_family=addr_family, ip_addr=ingress_nhop_ip_addr, rif_id=ingress_if_id, is_tunnel=0)
        sai_thrift_create_route(self.client, vr_id=vr_id, addr_family=addr_family, ip_addr=ip_addr_ingress_route, ip_mask=ip_mask_ingress_route, nhop=ingress_nhop)

        
        #adding tunnel and route
        initiator_id=sai_thrift_create_nhop(self.client, addr_family=addr_family, ip_addr=initiator_ip_addr, rif_id=tunnel_id , is_tunnel=1)
        sai_thrift_create_route(self.client,vr_id=vr_id, addr_family=addr_family, ip_addr=tunnel_ip_addr_route, ip_mask=tunnel_ip_mask_route, nhop=initiator_id)
        
        #Packet to be send 
        pkt = simple_tcp_packet(eth_dst=vr_mac,
                                eth_src='00:00:00:00:00:01',
                                ip_src='20.20.1.2',
                                ip_dst='1.1.1.1',
                                ip_id=1,
                                ip_ttl=64)
        #expected packet inner IP header
        inner_hdr = simple_tcp_packet(eth_dst=vr_mac,
                                eth_src='00:00:00:00:00:01',
                                ip_src='20.20.1.2',
                                ip_dst='1.1.1.1',
                                ip_id=1,
                                ip_ttl=63)
        exp_pkt = simple_ipv4ip_packet(eth_dst='00:05:06:00:00:00',
                                eth_src=vr_mac,
                                ip_dst='10.10.1.1',
                                ip_src='2.2.2.2',
                                ip_id=0,#mask the indentifier during check because it differs every time.better chech with wireshark
                                ip_tos=0xc8,
                                ip_ttl=64,
                                inner_frame=inner_hdr['IP']
                                )
        #masking packet 
        m=Mask(exp_pkt)
        m.set_do_not_care_scapy(ptf.packet.IP, 'id')
        m.set_do_not_care_scapy(ptf.packet.IP, 'chksum')
        
        try:
            # in tuple: 0 is device number, 1 is port number
            # this tuple uniquely identifies a port
            send_packet(self, 1, pkt)
            verify_packets(self, m, [0])

        
        finally:
            sai_thrift_remove_route(self.client,vr_id,addr_family,tunnel_ip_addr_route,tunnel_ip_mask_route,initiator_id) 
            self.client.sai_thrift_remove_next_hop(initiator_id)

            sai_thrift_remove_route(self.client,vr_id,addr_family,ip_addr_encap_route,ip_mask_encap_route,egress_nhop) 
            self.client.sai_thrift_remove_next_hop(egress_nhop)

            sai_thrift_remove_route(self.client,vr_id,addr_family,ip_addr_ingress_route,ip_mask_ingress_route,ingress_nhop) 
            self.client.sai_thrift_remove_next_hop(ingress_nhop)

            sai_thrift_remove_neighbor(self.client,addr_family,rif_id=ingress_if_id,ip_addr=ingress_ip_addr,dmac=neighbor_mac_ingress)
            
            sai_thrift_remove_neighbor(self.client,addr_family,rif_id=encap_if_id,ip_addr=encap_ip_addr,dmac=neighbor_mac_encap)
            self.client.sai_thrift_remove_router_interface(ingress_if_id)

            self.client.sai_thrift_remove_router_interface(encap_if_id)

            self.client.sai_thrift_remove_tunnel(tunnel_id)


@group("tunnel")
class IpIpP2PTunnelDecapTest(sai_base_test.ThriftInterfaceDataPlane):
    '''
    performs decapsulation 
    Overlay route : 1.1.1.x
    Tunnel Term Source is 10.10.1.1
    Tunnel Term Dest is 2.2.2.2
    
    We will send encapsulated packet from port1 and expect a decapsulated packet on port2
    -----------------------------------------------------------------
    Egress side[port2]           |          ingress side[port1]
    ------------------------------------------------------------------
    ip's falls in 20.20.1.0       |        ip's falls in 10.10.1.0
    ------------------------------------------------------------------
    ##ingress encap packet=Ether(dst=router_mac)/IP(src=1.1.1.1,dst=20.20.1.2)/IP(src=10.10.1.1,dst=2.2.2.2)/TCP()
    ##expected decap packet = Ether(dst=00:05:06:00:00:02,src=router_mac)/IP(src=1.1.1.1,dst=20.20.1.2)/TCP()
    '''
    def runTest(self):
        print
        switch_init(self.client)
        port1=port_list[0] #From where we send encapsulated packet 
        port2=port_list[1] #From where we see normal ip packet
        mac1 = '00:05:06:00:00:00' # port1 l2 entry
        mac2 = '00:05:06:00:00:02' 
        mac_action=1                
        vlan_id=0            #expecting to use unused vlans
        addr_family=0 #1 for v6
        tunnel_src_ip_addr='2.2.2.2'   #src for outer ip for encapsulation
        ingress_ip_addr='10.10.1.1'    #ingress enighbhor
        egress_ip_addr='20.20.1.1'     #egress neighbor 
        tunnel_ip_addr_route='1.1.1.0'       #to reach tunnel nhop
        tunnel_ip_mask_route='255.255.255.0'
        egress_nhop_ip_addr='20.20.1.1'
        ingress_nhop_ip_addr='10.10.1.1'
        ip_addr_egress_route='20.20.1.0' #for route
        ip_mask_egress_route='255.255.255.0'
        ip_addr_ingress_route='10.10.1.0' #for route
        ip_mask_ingress_route='255.255.255.0'
        initiator_ip_addr='10.10.1.1'  #tunnel dest ip(outer header)
        neighbor_mac_egress='00:05:06:00:00:02'
        neighbor_mac_ingress='00:05:06:00:00:00' #egress side
        ip_addr_decap_src='10.10.1.1' #tunnel table decap src to macth
        ip_addr_decap_dst='2.2.2.2'   #tunnel table decap dest to match
         
        vr_id=0 #using default vr id
                
        #creating a underlay interface in loopback
        underlay_if = sai_thrift_create_router_interface(self.client, is_port=1, port_id=0, vr_id=vr_id, vlan_id=vlan_id, v4_enabled=1, v6_enabled=1, mac='', is_lb=1)
        
        #creating an overlay interface in loopback
        overlay_if = sai_thrift_create_router_interface(self.client, is_port=1, port_id=0, vr_id=vr_id, vlan_id=vlan_id, v4_enabled=1, v6_enabled=1, mac='', is_lb=1)
        
        #creating a tunnel
        tunnel_id=sai_thrift_create_tunnel(self.client, addr_family=addr_family, ip_addr=tunnel_src_ip_addr, underlay_if=underlay_if, overlay_if=overlay_if)
            
        ###############################################################################
        #  Ingress configurations
        #  create router interface,
        #  create neighbor
        #  create nhop
        #  create route
        #   
        ############################################################################## 

        #ingress router interface
        ingress_if_id=sai_thrift_create_router_interface(self.client, vr_id=vr_id, is_port=1, port_id=port1, vlan_id=vlan_id, v4_enabled=1, v6_enabled = 1, mac='', is_lb=0)
        #ingress neighbor (ip=10.10.1.1 , mac=00:05:06:00:00:00 )
        sai_thrift_create_neighbor(self.client, addr_family=addr_family, rif_id=ingress_if_id, ip_addr=ingress_ip_addr, dmac=neighbor_mac_ingress)
        #ingress(encap) nhop and route create
        ingress_nhop=sai_thrift_create_nhop(self.client, addr_family=addr_family, ip_addr=ingress_nhop_ip_addr, rif_id=ingress_if_id, is_tunnel=0)
        sai_thrift_create_route(self.client, vr_id=vr_id, addr_family=addr_family, ip_addr=ip_addr_ingress_route, ip_mask=ip_mask_ingress_route, nhop=ingress_nhop)
        
        ###############################################################################
        #  Ingress configurations
        #  create router interface,
        #  create neighbor
        #  create nhop
        #  create route
        #   
        ############################################################################## 
            
        #egress router interface
        egress_if_id=sai_thrift_create_router_interface(self.client, vr_id=vr_id, is_port=1, port_id=port2, vlan_id=vlan_id, v4_enabled=1, v6_enabled = 1, mac='', is_lb=0)
        #egress neighbor (ip=20.20.1.1 , mac = 00:05:06:00:00:02)
        sai_thrift_create_neighbor(self.client,addr_family=addr_family,rif_id=egress_if_id,ip_addr=egress_ip_addr,dmac=neighbor_mac_egress)
        #egress nhop and route create
        egress_nhop=sai_thrift_create_nhop(self.client, addr_family=addr_family, ip_addr=egress_nhop_ip_addr, rif_id=egress_if_id, is_tunnel=0)
        sai_thrift_create_route(self.client, vr_id=vr_id, addr_family=addr_family, ip_addr=ip_addr_egress_route, ip_mask=ip_mask_egress_route, nhop=egress_nhop)
        #adding tunnel and route
        initiator_id=sai_thrift_create_nhop(self.client, addr_family=addr_family, ip_addr=initiator_ip_addr, rif_id=tunnel_id, is_tunnel=1)
        sai_thrift_create_route(self.client, vr_id=vr_id, addr_family=addr_family, ip_addr=tunnel_ip_addr_route, ip_mask=tunnel_ip_mask_route, nhop=initiator_id)
            
        ###################################################################################
        #Creating a Tunnel Table entry with
        #Source match 10.10.1.10
        #Dest match 2.2.2.2
        #
        #####################################################################################
        
        #create tunnel table entry for incoming match 		
        tunnel_entry_id=sai_thrift_create_tunnel_term_table(self.client,addr_family=addr_family,vr_id=vr_id,ip_addr_dst=ip_addr_decap_dst,ip_addr_src=ip_addr_decap_src,tunnel_oid=tunnel_id)
        
            
        exp_pkt = simple_tcp_packet(eth_src=router_mac,
                                eth_dst='00:05:06:00:00:02',
                                ip_dst='20.20.1.2',
                                ip_src='1.1.1.1',
                                ip_id=108,
                                ip_ttl=63)

        inner_hdr = simple_tcp_packet(eth_dst='00:00:08:08:08:08',
                                eth_src='00:00:00:00:00:11',
                                ip_dst='20.20.1.2',
                                ip_src='1.1.1.1',
                                ip_id=108,
                                ip_ttl=63)
        pkt = simple_ipv4ip_packet(eth_dst=router_mac,
                                eth_src='00:00:08:08:08:08',
                                ip_src='10.10.1.1',
                                ip_dst='2.2.2.2',
                                ip_id=50,
                                ip_ttl=64,
                                inner_frame=inner_hdr['IP'])
        
        try:
            # in tuple: 0 is device number, 0 is port number
            # this tuple uniquely identifies a port
            send_packet(self, 0, pkt)
            verify_packets(self, exp_pkt, [1])


        finally:
            self.client.sai_thrift_remove_tunnel_term_table_entry(tunnel_entry_id)
            sai_thrift_remove_route(self.client,vr_id,addr_family,tunnel_ip_addr_route,tunnel_ip_mask_route,initiator_id) 
            self.client.sai_thrift_remove_next_hop(initiator_id)

            sai_thrift_remove_route(self.client,vr_id,addr_family,ip_addr_ingress_route,ip_mask_ingress_route,ingress_nhop) 
            self.client.sai_thrift_remove_next_hop(ingress_nhop)

            sai_thrift_remove_route(self.client,vr_id,addr_family,ip_addr_egress_route,ip_mask_egress_route,egress_nhop) 
            self.client.sai_thrift_remove_next_hop(egress_nhop)

            sai_thrift_remove_neighbor(self.client,addr_family,rif_id=egress_if_id,ip_addr=egress_ip_addr,dmac=neighbor_mac_egress)
            
            sai_thrift_remove_neighbor(self.client,addr_family,rif_id=ingress_if_id,ip_addr=ingress_ip_addr,dmac=neighbor_mac_ingress)
            self.client.sai_thrift_remove_router_interface(egress_if_id)

            self.client.sai_thrift_remove_router_interface(ingress_if_id)

            self.client.sai_thrift_remove_tunnel(tunnel_id)
