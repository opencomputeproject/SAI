import sys
import copy
import logging
import types
import time
import re
import packet as scapy

import ptf
import ptf.dataplane
import ptf.parse
import ptf.ptfutils

global skipped_test_count
skipped_test_count = 0

_import_blacklist = set(locals().keys())

# Some useful defines
IP_ETHERTYPE = 0x800
TCP_PROTOCOL = 0x6
UDP_PROTOCOL = 0x11

MINSIZE = 0

_import_blacklist.add('FILTERS')
FILTERS = []

def reset_filters():
    FILTERS = []

# Needs to be a callable
def add_filter(my_filter):
    FILTERS.append(my_filter)

def get_filters():
    return FILTERS

def ether_filter(pkt_str):
    try:
        pkt = scapy.Ether(pkt_str)
        return True
    except:
        return False

def ipv6_filter(pkt_str):
    try:
        pkt = scapy.Ether(pkt_str)
        return (scapy.IPv6 in pkt)
    except:
        return False

def not_ipv6_filter(pkt_str):
    return not ipv6_filter(pkt_str)

def ip_make_tos(tos, ecn, dscp):
    if ecn is not None:
        tos = (tos & ~(0x3)) | ecn

    if dscp is not None:
        tos = (tos & ~(0xfc)) | (dscp << 2)

    return tos

def simple_tcp_packet(pktlen=100,
                      eth_dst='00:01:02:03:04:05',
                      eth_src='00:06:07:08:09:0a',
                      dl_vlan_enable=False,
                      vlan_vid=0,
                      vlan_pcp=0,
                      dl_vlan_cfi=0,
                      ip_src='192.168.0.1',
                      ip_dst='192.168.0.2',
                      ip_tos=0,
                      ip_ecn=None,
                      ip_dscp=None,
                      ip_ttl=64,
                      ip_id=0x0001,
                      ip_frag=0,
                      tcp_sport=1234,
                      tcp_dport=80,
                      tcp_flags="S",
                      ip_ihl=None,
                      ip_options=False,
                      with_tcp_chksum=True
                      ):
    """
    Return a simple dataplane TCP packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destinatino MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param ip_id IP ID
    @param tcp_dport TCP destination port
    @param tcp_sport TCP source port
    @param tcp_flags TCP Control flags
    @param with_tcp_chksum Valid TCP checksum

    Generates a simple TCP request.  Users
    shouldn't assume anything about this packet other than that
    it is a valid ethernet/IP/TCP frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    if with_tcp_chksum:
        tcp_hdr = scapy.TCP(sport=tcp_sport, dport=tcp_dport, flags=tcp_flags)
    else:
        tcp_hdr = scapy.TCP(sport=tcp_sport, dport=tcp_dport, flags=tcp_flags, chksum=0)

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
            tcp_hdr
    else:
        if not ip_options:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, frag=ip_frag)/ \
                tcp_hdr
        else:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, frag=ip_frag, options=ip_options)/ \
                tcp_hdr

    pkt = pkt/("".join([chr(x % 256) for x in xrange(pktlen - len(pkt))]))

    return pkt

def simple_tcpv6_packet(pktlen=100,
                        eth_dst='00:01:02:03:04:05',
                        eth_src='00:06:07:08:09:0a',
                        dl_vlan_enable=False,
                        vlan_vid=0,
                        vlan_pcp=0,
                        ipv6_src='2001:db8:85a3::8a2e:370:7334',
                        ipv6_dst='2001:db8:85a3::8a2e:370:7335',
                        ipv6_tc=0,
                        ipv6_ecn=None,
                        ipv6_dscp=None,
                        ipv6_hlim=64,
                        ipv6_fl=0,
                        tcp_sport=1234,
                        tcp_dport=80,
                        tcp_flags="S"):
    """
    Return a simple IPv6/TCP packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ipv6_src IPv6 source
    @param ipv6_dst IPv6 destination
    @param ipv6_tc IPv6 traffic class
    @param ipv6_ecn IPv6 traffic class ECN
    @param ipv6_dscp IPv6 traffic class DSCP
    @param ipv6_ttl IPv6 hop limit
    @param ipv6_fl IPv6 flow label
    @param tcp_dport TCP destination port
    @param tcp_sport TCP source port
    @param tcp_flags TCP Control flags

    Generates a simple TCP request. Users shouldn't assume anything about this
    packet other than that it is a valid ethernet/IPv6/TCP frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    ipv6_tc = ip_make_tos(ipv6_tc, ipv6_ecn, ipv6_dscp)

    pkt = scapy.Ether(dst=eth_dst, src=eth_src)
    if dl_vlan_enable or vlan_vid or vlan_pcp:
        pkt /= scapy.Dot1Q(vlan=vlan_vid, prio=vlan_pcp)
    pkt /= scapy.IPv6(src=ipv6_src, dst=ipv6_dst, fl=ipv6_fl, tc=ipv6_tc, hlim=ipv6_hlim)
    pkt /= scapy.TCP(sport=tcp_sport, dport=tcp_dport, flags=tcp_flags)
    pkt /= ("D" * (pktlen - len(pkt)))

    return pkt

def simple_udp_packet(pktlen=100,
                      eth_dst='00:01:02:03:04:05',
                      eth_src='00:06:07:08:09:0a',
                      dl_vlan_enable=False,
                      vlan_vid=0,
                      vlan_pcp=0,
                      dl_vlan_cfi=0,
                      ip_src='192.168.0.1',
                      ip_dst='192.168.0.2',
                      ip_tos=0,
                      ip_ecn=None,
                      ip_dscp=None,
                      ip_ttl=64,
                      udp_sport=1234,
                      udp_dport=80,
                      ip_ihl=None,
                      ip_options=False,
                      with_udp_chksum=True,
                      udp_payload=None
                      ):
    """
    Return a simple dataplane UDP packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param udp_dport UDP destination port
    @param udp_sport UDP source port
    @param with_udp_chksum Valid UDP checksum

    Generates a simple UDP packet. Users shouldn't assume anything about
    this packet other than that it is a valid ethernet/IP/UDP frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    if with_udp_chksum:
        udp_hdr = scapy.UDP(sport=udp_sport, dport=udp_dport)
    else:
        udp_hdr = scapy.UDP(sport=udp_sport, dport=udp_dport, chksum=0)

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, ihl=ip_ihl)/ \
            udp_hdr
    else:
        if not ip_options:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, ihl=ip_ihl)/ \
                udp_hdr
        else:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, ihl=ip_ihl, options=ip_options)/ \
                udp_hdr

    if udp_payload:
        pkt = pkt/udp_payload

    pkt = pkt/("".join([chr(x % 256) for x in xrange(pktlen - len(pkt))]))

    return pkt

def simple_ipv6_sr_packet(eth_dst='00:01:02:03:04:05',
                          eth_src='00:06:07:08:09:0a',
                          ipv6_src='2000::1',
                          ipv6_dst='2000::2',
                          ipv6_plen=None,
                          ipv6_tc=0,
                          ipv6_hlim=64,
                          ipv6_fl=0,
                          srh_seg_left=0,
                          srh_first_seg=0,
                          srh_flags=0,
                          srh_seg_list=[],
                          srh_nh=0,
                          inner_frame=None):

    """
    Return a simple dataplane IPv6 segment routing packet

    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param ipv6_src IPv6 source
    @param ipv6_dst IPv6 destination
    @param ipv6_tc IPv6 traffic class
    @param ipv6_ttl IPv6 hop limit
    @param ipv6_fl IPv6 flow label
    @param srh_seg_left IPV6 SRH segment left
    @param srh_first_seg IPV6 SRH first segment
    @param srh_flags IPV6 SRH flags
    @param srh_nh IPV6 SRH next header
    """

    pkt = scapy.Ether(dst=eth_dst, src=eth_src)
    pkt /= scapy.IPv6(src=ipv6_src, dst=ipv6_dst, fl=ipv6_fl, tc=ipv6_tc, nh=43,
                      hlim=ipv6_hlim, plen=ipv6_plen)
    reserved = (srh_first_seg << 24) + (srh_flags << 8)
    pkt /= scapy.IPv6ExtHdrRouting(nh=srh_nh, type=4, segleft=srh_seg_left,
                                   reserved=reserved,
                                   addresses=srh_seg_list)
    if inner_frame is not None:
        pkt /= inner_frame

    return pkt


def simple_geneve_packet(pktlen=300,
                        eth_dst='00:01:02:03:04:05',
                        eth_src='00:06:07:08:09:0a',
                        dl_vlan_enable=False,
                        vlan_vid=0,
                        vlan_pcp=0,
                        dl_vlan_cfi=0,
                        ip_src='192.168.0.1',
                        ip_dst='192.168.0.2',
                        ip_tos=0,
                        ip_ecn=None,
                        ip_dscp=None,
                        ip_ttl=64,
                        ip_id=0x0001,
                        udp_sport=1234,
                        with_udp_chksum=True,
                        ip_ihl=None,
                        ip_options=False,
			            geneve_ver=0x0,
                        geneve_reserved = 0x0,
                        geneve_vni=0x1234,
			            geneve_reserved2=0x0,
                        geneve_proto=0x6558,
                        inner_frame = None):
    """
    Return a simple dataplane GENEVE packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param ip_id IP ID
    @param udp_sport UDP source port
    @param geneve_ver version
    @param geneve_reserved reserved field
    @param geneve_vni GENEVE Network Identifier
    @param geneve_reserved2 reserved field
    @param inner_frame The inner Ethernet frame
    """
    if scapy.GENEVE is None:
        logging.error("A GENEVE packet was requested but GENEVE is not supported by your Scapy. See README for more information")
        return None

    udp_dport = 6081 # UDP port assigned by IANA for GENEVE

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    if with_udp_chksum:
        udp_hdr = scapy.UDP(sport=udp_sport, dport=udp_dport)
    else:
        udp_hdr = scapy.UDP(sport=udp_sport, dport=udp_dport, chksum=0)

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
            udp_hdr
    else:
        if not ip_options:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
                udp_hdr
        else:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, options=ip_options)/ \
                udp_hdr

    pkt = pkt / GENEVE(vni = geneve_vni, proto = geneve_proto )

    if inner_frame:
        pkt = pkt / inner_frame
    else:
        pkt = pkt / simple_tcp_packet(pktlen = pktlen - len(pkt))

    return pkt

def simple_nvgre_packet(pktlen=300,
                      eth_dst='00:01:02:03:04:05',
                      eth_src='00:06:07:08:09:0a',
                      dl_vlan_enable=False,
                      vlan_vid=0,
                      vlan_pcp=0,
                      dl_vlan_cfi=0,
                      ip_src='192.168.0.1',
                      ip_dst='192.168.0.2',
                      ip_tos=0,
                      ip_ecn=None,
                      ip_dscp=None,
                      ip_ttl=64,
                      ip_id=0x0001,
                      ip_ihl=None,
                      ip_options=False,
                      nvgre_version=0,
                      nvgre_tni=None,
                      nvgre_flowid=0,
                      inner_frame=None
                      ):
    """
    Return a simple dataplane GRE packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param ip_id IP ID
    @param nvgre_version Version
    @param nvgre_tni
    @param nvgre_flowid
    @param inner_frame payload of the GRE packet

    Generates a simple GRE packet. Users shouldn't assume anything about
    this packet other than that it is a valid ethernet/IP/NVGRE frame.
    """
    if scapy.NVGRE is None:
        logging.error("A NVGRE packet was requested but NVGRE is not supported by your Scapy. See README for more information")
        return None

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    nvgre_hdr = scapy.NVGRE(vsid=nvgre_tni, flowid=nvgre_flowid)

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
            nvgre_hdr
    else:
        if not ip_options:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
                nvgre_hdr
        else:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, options=ip_options)/ \
                nvgre_hdr

    if inner_frame:
        pkt = pkt / inner_frame
    else:
        pkt = pkt / scapy.IP()
        pkt = pkt/("D" * (pktlen - len(pkt)))

    return pkt

def simple_vxlan_packet(pktlen=300,
                        eth_dst='00:01:02:03:04:05',
                        eth_src='00:06:07:08:09:0a',
                        dl_vlan_enable=False,
                        vlan_vid=0,
                        vlan_pcp=0,
                        dl_vlan_cfi=0,
                        ip_src='192.168.0.1',
                        ip_dst='192.168.0.2',
                        ip_tos=0,
                        ip_ecn=None,
                        ip_dscp=None,
                        ip_ttl=64,
                        ip_id=0x0001,
                        udp_sport=1234,
                        udp_dport=4789,
                        with_udp_chksum=True,
                        ip_ihl=None,
                        ip_options=False,
			vxlan_reserved1=0x000000,
                        vxlan_vni = 0xaba,
			vxlan_reserved2=0x00,
                        inner_frame = None):
    """
    Return a simple dataplane VXLAN packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param ip_id IP ID
    @param udp_sport UDP source port
    @param udp_dport UDP dest port (IANA) = 4789 (VxLAN)
    @param vxlan_reserved1 reserved field (3B)
    @param vxlan_vni VXLAN Network Identifier
    @param vxlan_reserved2 reserved field (1B)
    @param inner_frame The inner Ethernet frame

    Generates a simple VXLAN packet. Users shouldn't assume anything about
    this packet other than that it is a valid ethernet/IP/UDP/VXLAN frame.
    """
    if scapy.VXLAN is None:
        logging.error("A VXLAN packet was requested but VXLAN is not supported by your Scapy. See README for more information")
        return None

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    if with_udp_chksum:
        udp_hdr = scapy.UDP(sport=udp_sport, dport=udp_dport)
    else:
        udp_hdr = scapy.UDP(sport=udp_sport, dport=udp_dport, chksum=0)

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
            udp_hdr
    else:
        if not ip_options:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
                udp_hdr
        else:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, options=ip_options)/ \
                udp_hdr

    pkt = pkt / VXLAN(vni = vxlan_vni, reserved1 = vxlan_reserved1, reserved2 = vxlan_reserved2)

    if inner_frame:
        pkt = pkt / inner_frame
    else:
        pkt = pkt / simple_tcp_packet(pktlen = pktlen - len(pkt))

    return pkt

def simple_vxlanv6_packet(pktlen=300,
                          eth_dst='00:01:02:03:04:05',
                          eth_src='00:06:07:08:09:0a',
                          dl_vlan_enable=False,
                          vlan_vid=0,
                          vlan_pcp=0,
                          dl_vlan_cfi=0,
                          ipv6_src='1::2',
                          ipv6_dst='3::4',
                          ipv6_fl=0,
                          ipv6_tc=0,
                          ipv6_ecn=None,
                          ipv6_dscp=None,
                          ipv6_hlim=64,
                          udp_sport=1234,
                          udp_dport=4789,
                          with_udp_chksum=True,
                          vxlan_reserved1=0x000000,
                          vxlan_vni = 0xaba,
                          vxlan_reserved2=0x00,
                          inner_frame = None):
    """
    Return a simple dataplane VXLAN packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ipv6_src IPv6 source
    @param ipv6_dst IPv6 destination
    @param ipv6_fl IPv6 flowlabel
    @param ipv6_tc IPv6 traffic class
    @param ipv6_ecn IPv6 traffic class ECN
    @param ipv6_dscp IPv6 traffic class DSCP
    @param ipv6_hlim IPv6 hop limit
    @param udp_sport UDP source port
    @param udp_dport UDP dest port (IANA) = 4789 (VxLAN)
    @param vxlan_reserved1 reserved field (3B)
    @param vxlan_vni VXLAN Network Identifier
    @param vxlan_reserved2 reserved field (1B)
    @param inner_frame The inner Ethernet frame

    Generates a simple VXLAN packet. Users shouldn't assume anything about
    this packet other than that it is a valid ethernet/IP/UDP/VXLAN frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    if with_udp_chksum:
        udp_hdr = scapy.UDP(sport=udp_sport, dport=udp_dport)
    else:
        udp_hdr = scapy.UDP(sport=udp_sport, dport=udp_dport, chksum=0)

    ipv6_tc = ip_make_tos(ipv6_tc, ipv6_ecn, ipv6_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IPv6(src=ipv6_src, dst=ipv6_dst, fl=ipv6_fl, tc=ipv6_tc, hlim=ipv6_hlim)/ \
            udp_hdr
    else:
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
              scapy.IPv6(src=ipv6_src, dst=ipv6_dst, fl=ipv6_fl, tc=ipv6_tc, hlim=ipv6_hlim)/ \
            udp_hdr

    pkt = pkt / VXLAN(vni = vxlan_vni, reserved1 = vxlan_reserved1, reserved2 = vxlan_reserved2)

    if inner_frame:
        pkt = pkt / inner_frame
    else:
        pkt = pkt / simple_tcp_packet(pktlen = pktlen - len(pkt))

    return pkt

def simple_gre_packet(pktlen=300,
                      eth_dst='00:01:02:03:04:05',
                      eth_src='00:06:07:08:09:0a',
                      dl_vlan_enable=False,
                      vlan_vid=0,
                      vlan_pcp=0,
                      dl_vlan_cfi=0,
                      ip_src='192.168.0.1',
                      ip_dst='192.168.0.2',
                      ip_tos=0,
                      ip_ecn=None,
                      ip_dscp=None,
                      ip_ttl=64,
                      ip_id=0x0001,
                      ip_ihl=None,
                      ip_options=False,
                      gre_chksum_present=0,
                      gre_routing_present=0, # begin reserved0
                      gre_key_present=0,
                      gre_seqnum_present=0,
                      gre_strict_route_source=0,
                      gre_flags=0, # end reserved0
                      gre_version=0,
                      gre_offset=None, # reserved1
                      gre_key=None,
                      gre_sequence_number=None,
                      inner_frame=None
                      ):
    """
    Return a simple dataplane GRE packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param ip_id IP ID
    @param gre_chkum_present with or without checksum
    @param gre_routing_present
    @param gre_key_present
    @param gre_seqnum_present
    @param gre_strict_route_source
    @param gre_flags
    @param gre_version Version
    @param gre_offset
    @param gre_key
    @param gre_sequence_number
    @param inner_frame payload of the GRE packet

    Generates a simple GRE packet. Users shouldn't assume anything about
    this packet other than that it is a valid ethernet/IP/GRE frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    # proto (ethertype) is set by Scapy based on the payload
    gre_hdr = scapy.GRE(chksum_present=gre_chksum_present,
                        routing_present=gre_routing_present,
                        key_present=gre_key_present,
                        seqnum_present=gre_seqnum_present,
                        strict_route_source=gre_strict_route_source,
                        flags=gre_flags, version=gre_version,
                        offset=gre_offset, key=gre_key,
                        seqence_number=gre_sequence_number) # typo in Scapy

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
            gre_hdr
    else:
        if not ip_options:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
                gre_hdr
        else:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, options=ip_options)/ \
                gre_hdr

    if inner_frame:
        pkt = pkt / inner_frame
        if ((ord(str(inner_frame)[0]) & 0xF0) == 0x60):
            pkt['GRE'].proto = 0x86DD
    else:
        pkt = pkt / scapy.IP()
        pkt = pkt/("D" * (pktlen - len(pkt)))

    return pkt

def simple_grev6_packet(pktlen=300,
                        eth_dst='00:01:02:03:04:05',
                        eth_src='00:06:07:08:09:0a',
                        dl_vlan_enable=False,
                        vlan_vid=0,
                        vlan_pcp=0,
                        dl_vlan_cfi=0,
                        ipv6_src='1::2',
                        ipv6_dst='3::4',
                        ipv6_fl=0,
                        ipv6_tc=0,
                        ipv6_ecn=None,
                        ipv6_dscp=None,
                        ipv6_hlim=64,
                        gre_chksum_present=0,
                        gre_routing_present=0, # begin reserved0
                        gre_key_present=0,
                        gre_seqnum_present=0,
                        gre_strict_route_source=0,
                        gre_flags=0, # end reserved0
                        gre_version=0,
                        gre_offset=None, # reserved1
                        gre_key=None,
                        gre_sequence_number=None,
                        inner_frame=None
                       ):
    """
    Return a simple dataplane GRE packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ipv6_src IPv6 source
    @param ipv6_dst IPv6 destination
    @param ipv6_fl IPv6 flowlabel
    @param ipv6_tc IPv6 traffic class
    @param ipv6_ecn IPv6 traffic class ECN
    @param ipv6_dscp IPv6 traffic class DSCP
    @param ipv6_hlim IPv6 hop limit
    @param gre_chkum_present with or without checksum
    @param gre_routing_present
    @param gre_key_present
    @param gre_seqnum_present
    @param gre_strict_route_source
    @param gre_flags
    @param gre_version Version
    @param gre_offset
    @param gre_key
    @param gre_sequence_number
    @param inner_frame payload of the GRE packet

    Generates a simple GRE packet. Users shouldn't assume anything about
    this packet other than that it is a valid ethernet/IP/GRE frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    # proto (ethertype) is set by Scapy based on the payload
    gre_hdr = scapy.GRE(chksum_present=gre_chksum_present,
                        routing_present=gre_routing_present,
                        key_present=gre_key_present,
                        seqnum_present=gre_seqnum_present,
                        strict_route_source=gre_strict_route_source,
                        flags=gre_flags, version=gre_version,
                        offset=gre_offset, key=gre_key,
                        seqence_number=gre_sequence_number) # typo in Scapy

    ipv6_tc = ip_make_tos(ipv6_tc, ipv6_ecn, ipv6_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IPv6(src=ipv6_src, dst=ipv6_dst, fl=ipv6_fl, tc=ipv6_tc, hlim=ipv6_hlim, nh=47)/ \
            gre_hdr
    else:
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.IPv6(src=ipv6_src, dst=ipv6_dst, fl=ipv6_fl, tc=ipv6_tc, hlim=ipv6_hlim, nh=47)/ \
            gre_hdr

    if inner_frame:
        pkt = pkt / inner_frame
        if ((ord(str(inner_frame)[0]) & 0xF0) == 0x60):
            pkt['GRE'].proto = 0x86DD
    else:
        pkt = pkt / scapy.IP()
        pkt = pkt/("D" * (pktlen - len(pkt)))

    return pkt

def simple_gre_erspan_packet(pktlen=300,
                             eth_dst='00:01:02:03:04:05',
                             eth_src='00:06:07:08:09:0a',
                             dl_vlan_enable=False,
                             vlan_vid=0,
                             vlan_pcp=0,
                             dl_vlan_cfi=0,
                             ip_src='192.168.0.1',
                             ip_dst='192.168.0.2',
                             ip_tos=0,
                             ip_ecn=None,
                             ip_dscp=None,
                             ip_ttl=64,
                             ip_id=0x0001,
                             ip_ihl=None,
                             ip_options=False,
                             gre_chksum_present=0,
                             gre_routing_present=0, # begin reserved0
                             gre_key_present=0,
                             gre_seqnum_present=0,
                             gre_strict_route_source=0,
                             gre_flags=0, # end reserved0
                             gre_version=0,
                             gre_offset=None, # reserved1
                             gre_key=None,
                             gre_sequence_number=None,
                             erspan_vlan=0,
                             erspan_priority=0,
                             erspan_direction=0,
                             erspan_truncated=0,
                             erspan_span_id=0,
                             erspan_unknown7=0,
                             inner_frame=None
                         ):
    """
    Return a simple dataplane GRE/ERSPAN packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param ip_id IP ID
    @param gre_chkum_present with or without checksum
    @param gre_routing_present
    @param gre_key_present
    @param gre_seqnum_present
    @param gre_strict_route_source
    @param gre_flags
    @param gre_version Version
    @param gre_offset
    @param gre_key
    @param gre_sequence_number
    @param inner_frame payload of the GRE packet
    @param erspan_vlan
    @param erspan_priority
    @param erspan_direction
    @param erspan_truncated
    @param erspan_span_id
    @param erspan_unknown7

    Generates a simple GRE/ERSPAN packet. Users shouldn't assume anything about
    this packet other than that it is a valid ethernet/IP/GRE/ERSPAN frame.
    """
    if scapy.GRE is None or scapy.ERSPAN is None:
        logging.error("A GRE/ERSPAN packet was requested but GRE or ERSPAN is not supported by your Scapy. See README for more information")
        return None

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    # proto (ethertype) is set by Scapy based on the payload
    gre_hdr = scapy.GRE(chksum_present=gre_chksum_present,
                        routing_present=gre_routing_present,
                        key_present=gre_key_present,
                        seqnum_present=gre_seqnum_present,
                        strict_route_source=gre_strict_route_source,
                        flags=gre_flags, version=gre_version,
                        offset=gre_offset, key=gre_key,
                        seqence_number=gre_sequence_number) # typo in Scapy

    erspan_hdr = scapy.ERSPAN(vlan = erspan_vlan,
                              priority = erspan_priority,
                              direction = erspan_direction,
                              truncated = erspan_truncated,
                              span_id = erspan_span_id,
                              unknown7 = erspan_unknown7)

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
            gre_hdr / erspan_hdr
    else:
        if not ip_options:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
                gre_hdr / erspan_hdr
        else:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, options=ip_options)/ \
                gre_hdr / erspan_hdr

    if inner_frame:
        pkt = pkt / inner_frame
    else:
        pkt = pkt / scapy.IP()
        pkt = pkt/("D" * (pktlen - len(pkt)))

    return pkt

def ipv4_erspan_pkt(pktlen=350,
                      eth_dst='00:01:02:03:04:05',
                      eth_src='00:06:07:08:09:0a',
                      dl_vlan_enable=False,
                      vlan_vid=0,
                      vlan_pcp=0,
                      dl_vlan_cfi=0,
                      ip_src='192.168.0.1',
                      ip_dst='192.168.0.2',
                      ip_tos=0,
                      ip_ecn=None,
                      ip_dscp=None,
                      ip_ttl=64,
                      ip_id=0x0001,
                      ip_ihl=None,
                      ip_options=False,
                      version=2,
                      mirror_id=0x3FF,
                      sgt_other=0,
                      inner_frame=None
                      ):
    """
    Return a GRE ERSPAN packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param ip_id IP ID
    @param erspan version
    @param span_id (mirror_session_id)
    @param inner_frame payload of the GRE packet
    """
    if scapy.GRE is None or scapy.ERSPAN is None or scapy.ERSPAN_III is None:
        logging.error("A GRE/ERSPAN packet was requested but GRE or ERSPAN is not supported by your Scapy. See README for more information")
        return None

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    if version == 2:
        erspan_hdr = scapy.GRE(proto=0x22eb)/scapy.ERSPAN_III(span_id=mirror_id, sgt_other = sgt_other)
    else:
        erspan_hdr = scapy.GRE(proto=0x88be)/scapy.ERSPAN(span_id=mirror_id)

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
            erspan_hdr
    else:
        if not ip_options:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
                erspan_hdr
        else:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, options=ip_options)/ \
                erspan_hdr

    if inner_frame:
        pkt = pkt / inner_frame
    else:
        pkt = pkt / scapy.IP()
        pkt = pkt/("D" * (pktlen - len(pkt)))

    return pkt

def ipv4_erspan_platform_pkt(pktlen=350,
                      eth_dst='00:01:02:03:04:05',
                      eth_src='00:06:07:08:09:0a',
                      dl_vlan_enable=False,
                      vlan_vid=0,
                      vlan_pcp=0,
                      dl_vlan_cfi=0,
                      ip_src='192.168.0.1',
                      ip_dst='192.168.0.2',
                      ip_tos=0,
                      ip_ecn=None,
                      ip_dscp=None,
                      ip_ttl=64,
                      ip_id=0x0001,
                      ip_ihl=None,
                      ip_options=False,
                      version=2,
                      mirror_id=0x3FF,
                      sgt_other=1,
                      platf_id=0,
                      info1=0,
                      info2=0,
                      inner_frame=None
                      ):
    """
    Return a GRE ERSPAN packet with Platform Specific Subheader

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param ip_id IP ID
    @param erspan version
    @param span_id (mirror_session_id)
    @param platf_id Specific Platform Subheader Platf Id
    @param info1 Specific Platform Subheader 26 bit field
    @param info2 Specific Platform Subheader 32 bit field
    @param inner_frame payload of the GRE packet
    """
    if scapy.GRE is None or scapy.ERSPAN is None or scapy.ERSPAN_III is None or scapy.PlatformSpecific is None:
        logging.error("A GRE/ERSPAN packet was requested but GRE or ERSPAN is not supported by your Scapy. See README for more information")
        return None

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    if version == 2:
        erspan_hdr = scapy.GRE(proto=0x22eb)/scapy.ERSPAN_III(span_id=mirror_id, sgt_other=sgt_other)
        if sgt_other == 1:
            erspan_hdr = erspan_hdr/scapy.PlatformSpecific(platf_id=platf_id, info1=info1, info2=info2)
    else:
        erspan_hdr = scapy.GRE(proto=0x88be)/scapy.ERSPAN(span_id=mirror_id)

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
            erspan_hdr
    else:
        if not ip_options:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)/ \
                erspan_hdr
        else:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, options=ip_options)/ \
                erspan_hdr

    if inner_frame:
        pkt = pkt / inner_frame
    else:
        pkt = pkt / scapy.IP()
        pkt = pkt/("D" * (pktlen - len(pkt)))

    return pkt

def simple_udpv6_packet(pktlen=100,
                        eth_dst='00:01:02:03:04:05',
                        eth_src='00:06:07:08:09:0a',
                        dl_vlan_enable=False,
                        vlan_vid=0,
                        vlan_pcp=0,
                        ipv6_src='2001:db8:85a3::8a2e:370:7334',
                        ipv6_dst='2001:db8:85a3::8a2e:370:7335',
                        ipv6_tc=0,
                        ipv6_ecn=None,
                        ipv6_dscp=None,
                        ipv6_hlim=64,
                        ipv6_fl=0,
                        udp_sport=1234,
                        udp_dport=80):
    """
    Return a simple IPv6/UDP packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ipv6_src IPv6 source
    @param ipv6_dst IPv6 destination
    @param ipv6_tc IPv6 traffic class
    @param ipv6_ecn IPv6 traffic class ECN
    @param ipv6_dscp IPv6 traffic class DSCP
    @param ipv6_ttl IPv6 hop limit
    @param ipv6_fl IPv6 flow label
    @param udp_dport UDP destination port
    @param udp_sport UDP source port

    Generates a simple UDP request. Users shouldn't assume anything about this
    packet other than that it is a valid ethernet/IPv6/UDP frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    ipv6_tc = ip_make_tos(ipv6_tc, ipv6_ecn, ipv6_dscp)

    pkt = scapy.Ether(dst=eth_dst, src=eth_src)
    if dl_vlan_enable or vlan_vid or vlan_pcp:
        pkt /= scapy.Dot1Q(vlan=vlan_vid, prio=vlan_pcp)
    pkt /= scapy.IPv6(src=ipv6_src, dst=ipv6_dst, fl=ipv6_fl, tc=ipv6_tc, hlim=ipv6_hlim)
    pkt /= scapy.UDP(sport=udp_sport, dport=udp_dport)
    pkt /= ("D" * (pktlen - len(pkt)))

    return pkt

def simple_ipv4ip_packet(pktlen=300,
                         eth_dst='00:01:02:03:04:05',
                         eth_src='00:06:07:08:09:0a',
                         dl_vlan_enable=False,
                         vlan_vid=0,
                         vlan_pcp=0,
                         dl_vlan_cfi=0,
                         ip_src='192.168.0.1',
                         ip_dst='192.168.0.2',
                         ip_tos=0,
                         ip_ecn=None,
                         ip_dscp=None,
                         ip_ttl=64,
                         ip_id=0x0001,
                         ip_ihl=None,
                         ip_options=False,
                         inner_frame=None
                        ):
    """
    Return a simple dataplane IPv4 encapsulated packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param ip_id IP ID
    @param inner_frame payload of the packet

    Generates a simple IPv4 encapsulated packet.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)
    else:
        if not ip_options:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl)
        else:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, options=ip_options)

    if inner_frame:
        pkt = pkt / inner_frame
        if ((ord(str(inner_frame)[0]) & 0xF0) == 0x40):
            pkt['IP'].proto = 4
        elif ((ord(str(inner_frame)[0]) & 0xF0) == 0x60):
            pkt['IP'].proto = 41
    else:
        pkt = pkt / scapy.IP()
        pkt = pkt/("D" * (pktlen - len(pkt)))
        pkt['IP'].proto = 4

    return pkt

def simple_ipv6ip_packet(pktlen=300,
                         eth_dst='00:01:02:03:04:05',
                         eth_src='00:06:07:08:09:0a',
                         dl_vlan_enable=False,
                         vlan_vid=0,
                         vlan_pcp=0,
                         dl_vlan_cfi=0,
                         ipv6_src='1::2',
                         ipv6_dst='3::4',
                         ipv6_fl=0,
                         ipv6_tc=0,
                         ipv6_ecn=None,
                         ipv6_dscp=None,
                         ipv6_hlim=64,
                         inner_frame=None
                       ):
    """
    Return a simple dataplane IPv6 encapsulated packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ipv6_src IPv6 source
    @param ipv6_dst IPv6 destination
    @param ipv6_fl IPv6 flowlabel
    @param ipv6_tc IPv6 traffic class
    @param ipv6_ecn IPv6 traffic class ECN
    @param ipv6_dscp IPv6 traffic class DSCP
    @param ipv6_hlim IPv6 hop limit
    @param inner_frame payload of the GRE packet

    Generates a simple IPv6 encapsulated packet.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    ipv6_tc = ip_make_tos(ipv6_tc, ipv6_ecn, ipv6_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IPv6(src=ipv6_src, dst=ipv6_dst, fl=ipv6_fl, tc=ipv6_tc, hlim=ipv6_hlim)
    else:
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.IPv6(src=ipv6_src, dst=ipv6_dst, fl=ipv6_fl, tc=ipv6_tc, hlim=ipv6_hlim)

    if inner_frame:
        pkt = pkt / inner_frame
        if ((ord(str(inner_frame)[0]) & 0xF0) == 0x40):
            pkt['IPv6'].nh = 4
        elif ((ord(str(inner_frame)[0]) & 0xF0) == 0x60):
            pkt['IPv6'].nh = 41
    else:
        pkt = pkt / scapy.IP()
        pkt = pkt/("D" * (pktlen - len(pkt)))
        pkt['IPv6'].nh = 4

    return pkt

def simple_icmp_packet(pktlen=60,
                      eth_dst='00:01:02:03:04:05',
                      eth_src='00:06:07:08:09:0a',
                      dl_vlan_enable=False,
                      vlan_vid=0,
                      vlan_pcp=0,
                      ip_src='192.168.0.1',
                      ip_dst='192.168.0.2',
                      ip_tos=0,
                      ip_ecn=None,
                      ip_dscp=None,
                      ip_ttl=64,
                      ip_id=1,
                      icmp_type=8,
                      icmp_code=0,
                      icmp_data=''):
    """
    Return a simple ICMP packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destinatino MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param ip_id IP Identification
    @param icmp_type ICMP type
    @param icmp_code ICMP code
    @param icmp_data ICMP data

    Generates a simple ICMP ECHO REQUEST.  Users
    shouldn't assume anything about this packet other than that
    it is a valid ethernet/ICMP frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=0, vlan=vlan_vid)/ \
            scapy.IP(src=ip_src, dst=ip_dst, ttl=ip_ttl, tos=ip_tos, id=ip_id)/ \
            scapy.ICMP(type=icmp_type, code=icmp_code)/ icmp_data
    else:
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.IP(src=ip_src, dst=ip_dst, ttl=ip_ttl, tos=ip_tos, id=ip_id)/ \
            scapy.ICMP(type=icmp_type, code=icmp_code)/ icmp_data

    pkt = pkt/("0" * (pktlen - len(pkt)))

    return pkt

def simple_icmpv6_packet(pktlen=100,
                         eth_dst='00:01:02:03:04:05',
                         eth_src='00:06:07:08:09:0a',
                         dl_vlan_enable=False,
                         vlan_vid=0,
                         vlan_pcp=0,
                         ipv6_src='2001:db8:85a3::8a2e:370:7334',
                         ipv6_dst='2001:db8:85a3::8a2e:370:7335',
                         ipv6_tc=0,
                         ipv6_ecn=None,
                         ipv6_dscp=None,
                         ipv6_hlim=64,
                         ipv6_fl=0,
                         icmp_type=8,
                         icmp_code=0):
    """
    Return a simple ICMPv6 packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ipv6_src IPv6 source
    @param ipv6_dst IPv6 destination
    @param ipv6_tc IPv6 traffic class
    @param ipv6_ecn IPv6 traffic class ECN
    @param ipv6_dscp IPv6 traffic class DSCP
    @param ipv6_ttl IPv6 hop limit
    @param ipv6_fl IPv6 flow label
    @param icmp_type ICMP type
    @param icmp_code ICMP code

    Generates a simple ICMP ECHO REQUEST. Users shouldn't assume anything
    about this packet other than that it is a valid ethernet/IPv6/ICMP frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    ipv6_tc = ip_make_tos(ipv6_tc, ipv6_ecn, ipv6_dscp)

    pkt = scapy.Ether(dst=eth_dst, src=eth_src)
    if dl_vlan_enable or vlan_vid or vlan_pcp:
        pkt /= scapy.Dot1Q(vlan=vlan_vid, prio=vlan_pcp)
    pkt /= scapy.IPv6(src=ipv6_src, dst=ipv6_dst, fl=ipv6_fl, tc=ipv6_tc, hlim=ipv6_hlim)
    pkt /= scapy.ICMPv6Unknown(type=icmp_type, code=icmp_code)
    pkt /= ("D" * (pktlen - len(pkt)))

    return pkt

def simple_arp_packet(pktlen=60,
                      eth_dst='ff:ff:ff:ff:ff:ff',
                      eth_src='00:06:07:08:09:0a',
                      vlan_vid=0,
                      vlan_pcp=0,
                      arp_op=1,
                      ip_snd='192.168.0.1',
                      ip_tgt='192.168.0.2',
                      hw_snd='00:06:07:08:09:0a',
                      hw_tgt='00:00:00:00:00:00',
                      ):
    """
    Return a simple ARP packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destinatino MAC
    @param eth_src Source MAC
    @param arp_op Operation (1=request, 2=reply)
    @param ip_snd Sender IP
    @param ip_tgt Target IP
    @param hw_snd Sender hardware address
    @param hw_tgt Target hardware address

    Generates a simple ARP REQUEST.  Users
    shouldn't assume anything about this packet other than that
    it is a valid ethernet/ARP frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    pkt = scapy.Ether(dst=eth_dst, src=eth_src)
    if vlan_vid or vlan_pcp:
        pkt /= scapy.Dot1Q(vlan=vlan_vid, prio=vlan_pcp)
    pkt /= scapy.ARP(hwsrc=hw_snd, hwdst=hw_tgt, pdst=ip_tgt, psrc=ip_snd, op=arp_op)

    pkt = pkt/("\0" * (pktlen - len(pkt)))

    return pkt

def simple_eth_packet(pktlen=60,
                      eth_dst='00:01:02:03:04:05',
                      eth_src='00:06:07:08:09:0a',
                      eth_type=0x88cc):

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    pkt = scapy.Ether(dst=eth_dst, src=eth_src, type=eth_type)

    pkt = pkt/("0" * (pktlen - len(pkt)))

    return pkt

def simple_ip_packet(pktlen=100,
                     eth_dst='00:01:02:03:04:05',
                     eth_src='00:06:07:08:09:0a',
                     dl_vlan_enable=False,
                     vlan_vid=0,
                     vlan_pcp=0,
                     dl_vlan_cfi=0,
                     ip_src='192.168.0.1',
                     ip_dst='192.168.0.2',
                     ip_tos=0,
                     ip_ecn=None,
                     ip_dscp=None,
                     ip_ttl=64,
                     ip_id=0x0001,
                     ip_ihl=None,
                     ip_options=False,
                     ip_proto=0
                     ):
    """
    Return a simple dataplane IP packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destinatino MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param ip_id IP ID

    Generates a simple IP packet.  Users
    shouldn't assume anything about this packet other than that
    it is a valid ethernet/IP frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    # Note Dot1Q.id is really CFI
    if (dl_vlan_enable):
        pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
            scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
            scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, proto=ip_proto)
    else:
        if not ip_options:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, proto=ip_proto)
        else:
            pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
                scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, proto=ip_proto, options=ip_options)

    pkt = pkt/("".join([chr(x % 256) for x in xrange(pktlen - len(pkt))]))

    return pkt

def simple_ip_only_packet(pktlen=100,
                     ip_src='192.168.0.1',
                     ip_dst='192.168.0.2',
                     ip_tos=0,
                     ip_ecn=None,
                     ip_dscp=None,
                     ip_ttl=64,
                     ip_id=0x0001,
                     ip_ihl=None,
                     ip_options=False,
                     tcp_sport=1234,
                     tcp_dport=80,
                     tcp_flags="S",
                     with_tcp_chksum=True
                     ):
    """
    Return a simple dataplane IP packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param ip_ttl IP TTL
    @param ip_id IP ID

    Generates a simple IP packet.  Users
    shouldn't assume anything about this packet other than that
    it is a valid IP frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    if with_tcp_chksum:
        tcp_hdr = scapy.TCP(sport=tcp_sport, dport=tcp_dport, flags=tcp_flags)
    else:
        tcp_hdr = scapy.TCP(sport=tcp_sport, dport=tcp_dport, flags=tcp_flags, chksum=0)

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    if not ip_options:
        pkt = scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl) / tcp_hdr
    else:
        pkt = scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, id=ip_id, ihl=ip_ihl, options=ip_options) / tcp_hdr

    pkt = pkt/("".join([chr(x % 256) for x in xrange(pktlen - len(pkt))]))

    return pkt

def simple_mpls_packet(pktlen=300,
                       eth_dst='00:01:02:03:04:05',
                       eth_src='00:06:07:08:09:0a',
                       dl_vlan_enable=False,
                       vlan_vid=0,
                       vlan_pcp=0,
                       mpls_type=0x8847,
                       mpls_tags=[],
                       dl_vlan_cfi=0,
                       inner_frame = None):
    """
    Return a simple dataplane MPLS packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destination MAC
    @param eth_src Source MAC
    @param dl_vlan_enable True if the packet is with vlan, False otherwise
    @param vlan_vid VLAN ID
    @param vlan_pcp VLAN priority
    @param mpls_tags mpls tag stack
    @param inner_frame The inner frame

    """
    if scapy.MPLS is None:
        logging.error("A MPLS packet was requested but MPLS is not supported by your Scapy. See README for more information")
        return None

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    pkt = scapy.Ether(dst=eth_dst, src=eth_src)
    pkt[Ether].setfieldval('type', mpls_type)

    if (dl_vlan_enable):
        pkt / scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)
        pkt[Dot1Q].setfieldval('type', mpls_type)

    mpls_tags = list(mpls_tags)
    while len(mpls_tags):
        tag = mpls_tags.pop(0)
        mpls = MPLS()
        if 'label' in tag:
            mpls.label = tag['label']
        if 'tc' in tag:
            mpls.cos = tag['tc']
        if 'ttl' in tag:
            mpls.ttl = tag['ttl']
        if 's' in tag:
            mpls.s = tag['s']
        pkt = pkt / mpls

    if inner_frame:
        pkt = pkt / inner_frame
    else:
        pkt = pkt / simple_tcp_packet(pktlen = pktlen - len(pkt))

    return pkt

def simple_qinq_tcp_packet(pktlen=100,
                    eth_dst='00:01:02:03:04:05',
                    eth_src='00:06:07:08:09:0a',
                    dl_vlan_outer=20,
                    dl_vlan_pcp_outer=0,
                    dl_vlan_cfi_outer=0,
                    vlan_vid=10,
                    vlan_pcp=0,
                    dl_vlan_cfi=0,
                    ip_src='192.168.0.1',
                    ip_dst='192.168.0.2',
                    ip_tos=0,
                    ip_ecn=None,
                    ip_dscp=None,
                    ip_ttl=64,
                    tcp_sport=1234,
                    tcp_dport=80,
                    ip_ihl=None,
                    ip_options=False
                    ):
    """
    Return a doubly tagged dataplane TCP packet

    Supports a few parameters:
    @param len Length of packet in bytes w/o CRC
    @param eth_dst Destinatino MAC
    @param eth_src Source MAC
    @param dl_vlan_outer Outer VLAN ID
    @param dl_vlan_pcp_outer Outer VLAN priority
    @param dl_vlan_cfi_outer Outer VLAN cfi bit
    @param vlan_vid Inner VLAN ID
    @param vlan_pcp VLAN priority
    @param dl_vlan_cfi VLAN cfi bit
    @param ip_src IP source
    @param ip_dst IP destination
    @param ip_tos IP ToS
    @param ip_ecn IP ToS ECN
    @param ip_dscp IP ToS DSCP
    @param tcp_dport TCP destination port
    @param ip_sport TCP source port

    Generates a TCP request.  Users
    shouldn't assume anything about this packet other than that
    it is a valid ethernet/IP/TCP frame.
    """

    if MINSIZE > pktlen:
        pktlen = MINSIZE

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    # Note Dot1Q.id is really CFI
    pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
          scapy.Dot1Q(prio=dl_vlan_pcp_outer, id=dl_vlan_cfi_outer, vlan=dl_vlan_outer)/ \
          scapy.Dot1Q(prio=vlan_pcp, id=dl_vlan_cfi, vlan=vlan_vid)/ \
          scapy.IP(src=ip_src, dst=ip_dst, tos=ip_tos, ttl=ip_ttl, ihl=ip_ihl)/ \
          scapy.TCP(sport=tcp_sport, dport=tcp_dport)

    pkt = pkt/("".join([chr(x % 256) for x in xrange(pktlen - len(pkt))]))

    return pkt

def dhcp_discover_packet(eth_dst='ff:ff:ff:ff:ff:ff',
            eth_src='00:01:02:03:04:05',
            ip_src='0.0.0.0',
            ip_dst='255.255.255.255',
            src_port=68,
            dst_port=67,
            bootp_chaddr='00:01:02:03:04:05',
            ):
    """
    Return a dhcp discover packet

    Supports a few parameters:
    @param eth_dst Destination MAC, should be broadcast
    @param eth_src Source MAC
    @param ip_src Source IP, should be 0.0.0.0
    @param ip_dst Destination IP, should be broadcast address
    @param src_port Source Port, 68 for DHCP Client
    @param dst_port Destination Port, 67 for DHCP Server
    @param bootp_chaddr MAC Address of client

    """

    pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
    scapy.IP(src=ip_src, dst=ip_dst)/ \
    scapy.UDP(sport=src_port, dport=dst_port)/ \
    scapy.BOOTP(chaddr=bootp_chaddr)/ \
    scapy.DHCP(options=[('message-type', 'discover'), ('end')])
    return pkt

def dhcp_offer_packet(eth_dst='00:01:02:03:04:05',
                eth_src='06:07:08:09:10:11',
                ip_src='0.0.0.0',
                ip_dst='255.255.255.255',
                ip_len=308,
                ip_tos=16,
                ip_ecn=None,
                ip_dscp=None,
                ip_ttl=128,
                ip_id=0,
                src_port=67,
                dst_port=68,
                udp_len=308,
                bootp_op=2,
                bootp_htype=1,
                bootp_hlen=6,
                bootp_hops=1,
                bootp_xid=00000000,
                bootp_secs=0,
                bootp_flags=0000,
                bootp_ciaddr='0.0.0.0',
                bootp_yiaddr='1.2.3.4',
                bootp_siaddr='0.0.0.0',
                bootp_chaddr='00:01:02:03:04:05',
                bootp_giaddr='9.8.7.6',
                dhcp_serverip='1.2.3.4',
                dhcp_lease=256,
                dhcp_netmask='255.255.255.0',
                padding=None):
    """
    Return a dhcp offer packet

    Supports a few parameters:
    @param eth_dst Destination MAC, should be address of client
    @param eth_src Source MAC, address of DHCP server or relay
    @param ip_src Source IP, should be DHCP server IP
    @param ip_dst Destination IP, should be client address
    @param src_port Source Port, 67 for DHCP Server
    @param dst_port Destination Port, 68 for DHCP Client
    @param bootp_op Operation Code, 2 indicates reply
    @param bootp_htype Hardware Type, 1 indicates ethernet
    @param bootp_hlen Hardware Address Len, 6 is value for Ethernet
    @param bootp_hops Hops, used by relay agent to forward messages
    @param bootp_xid Transaction Identifier, 32 bit field that identifies transaction
    @param bootp_secs Seconds, time elapsed since client started trying to boot
    @param bootp_flags Flags, 16 bits (1 bit is broadcast flag)
    @param bootp_ciaddr Client IP Address, if client has a current IP address otherwise set to zeros
    @param bootp_yiaddr Your IP Address, address that server is assigning to client
    @param bootp_siaddr Server IP Address, address of server
    @param bootp_giaddr Gateway IP Address, address of relay agent if used
    @param bootp_chaddr MAC Address of client
    @param dhcp_serverip IP address of DHCP server
    @param dhcp_lease Time in seconds of DHCP lease
    @param dhcp_netmask Subnet mask of client
    @param padding '\x00' padding inserted at end of packet, '\x00'*n where n is number of bytes
    """

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
    scapy.IP(src=ip_src, dst=ip_dst, len=ip_len, tos=ip_tos, ttl=ip_ttl, id=0)/ \
    scapy.UDP(sport=src_port, dport=dst_port, len=udp_len)/ \
    scapy.BOOTP(op=bootp_op, htype=bootp_htype, hlen=bootp_hlen, hops=bootp_hops, xid=bootp_xid,
            secs=bootp_secs, flags=bootp_flags, ciaddr=bootp_ciaddr, yiaddr=bootp_yiaddr, siaddr=bootp_siaddr,
            giaddr=bootp_giaddr, chaddr=bootp_chaddr)/ \
    scapy.DHCP(options=[('message-type', 'offer'), ('server_id', dhcp_serverip), ('lease_time', int(dhcp_lease)),
            ('subnet_mask', dhcp_netmask), ('end')])/ \
    scapy.PADDING(padding)
    return pkt

def dhcp_request_packet(eth_dst='ff:ff:ff:ff:ff:ff',
                    eth_src='00:01:02:03:04:05',
                    ip_src='0.0.0.0',
                    ip_dst='255.255.255.255',
                    src_port=68,
                    dst_port=67,
                    bootp_chaddr='00:01:02:03:04:05',
                    dhcp_request_ip='1.2.3.4'):
    """
    Return a dhcp request packet

    Supports a few parameters:
    @param eth_dst Destination MAC, should be broadcast address
    @param eth_src Source MAC, address of client
    @param ip_src Source IP, should be default route IP Address (0.0.0.0)
    @param ip_dst Destination IP, should be broadcast address
    @param src_port Source Port, 68 for DHCP Client
    @param dst_port Destination Port, 67 for DHCP Server
    @param bootp_chaddr MAC Address of DHCP Client
    @param dhcp_request_ip IP Address, address of client (found in DHCP Offer)
    """

    pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
    scapy.IP(src=ip_src, dst=ip_dst)/ \
    scapy.UDP(sport=src_port, dport=dst_port)/ \
    scapy.BOOTP(chaddr=bootp_chaddr)/ \
    scapy.DHCP(options=[('message-type', 'request'), ('requested_addr', dhcp_request_ip), ('end')])
    return pkt

def dhcp_ack_packet(eth_dst='00:01:02:03:04:05',
                eth_src='06:07:08:09:10:11',
                ip_src='1.2.3.4',
                ip_dst='255.255.255.255',
                ip_len=328,
                ip_tos=16,
                ip_ecn=None,
                ip_dscp=None,
                ip_ttl=128,
                ip_id=0,
                src_port=67,
                dst_port=68,
                udp_len=308,
                bootp_op=2,
                bootp_htype=1,
                bootp_hlen=6,
                bootp_hops=1,
                bootp_xid=00000000,
                bootp_secs=0,
                bootp_flags=0000,
                bootp_ciaddr='0.0.0.0',
                bootp_yiaddr='1.2.3.4',
                bootp_siaddr='0.0.0.0',
                bootp_chaddr='00:01:02:03:04:05',
                bootp_giaddr='9.8.7.6',
                dhcp_serverip='1.2.3.4',
                dhcp_lease=256,
                dhcp_netmask='255.255.255.0',
                padding=None):
    """
    Return a dhcp ack packet

    Supports a few parameters:
    @param eth_dst Destination MAC, should be address of client
    @param eth_src Source MAC, address of DHCP server or relay
    @param ip_src Source IP, should be DHCP server IP
    @param ip_dst Destination IP, should be client address
    @param src_port Source Port, 67 for DHCP Server
    @param dst_port Destination Port, 68 for DHCP Client
    @param bootp_op Operation Code, 2 indicates reply
    @param bootp_htype Hardware Type, 1 indicates ethernet
    @param bootp_hlen Hardware Address Len, 6 is value for Ethernet
    @param bootp_hops Hops, used by relay agent to forward messages
    @param bootp_xid Transaction Identifier, 32 bit field that identifies transaction
    @param bootp_secs Seconds, time elapsed since client started trying to boot
    @param bootp_flags Flags, 16 bits (1 bit is broadcast flag)
    @param bootp_ciaddr Client IP Address, if client has a current IP address otherwise set to zeros
    @param bootp_yiaddr Your IP Address, address that server is assigning to client
    @param bootp_siaddr Server IP Address, address of server
    @param bootp_giaddr Gateway IP Address, address of relay agent if used
    @param bootp_chaddr MAC Address of client
    @param dhcp_serverip IP address of DHCP server
    @param dhcp_lease Time in seconds of DHCP lease
    @param dhcp_netmask Subnet mask of client
    @param padding '\x00' padding inserted at end of packet, '\x00'*n where n is number of bytes
    """

    ip_tos = ip_make_tos(ip_tos, ip_ecn, ip_dscp)

    pkt = scapy.Ether(dst=eth_dst, src=eth_src) / \
    scapy.IP(src=ip_src, dst=ip_dst, len=ip_len, tos=ip_tos, ttl=ip_ttl, id=ip_id) / \
    scapy.UDP(sport=src_port, dport=dst_port, len=udp_len) / \
    scapy.BOOTP(op=bootp_op, htype=bootp_htype, hlen=bootp_hlen, hops=bootp_hops, xid=bootp_xid,
                flags=bootp_flags, ciaddr=bootp_ciaddr, yiaddr=bootp_yiaddr, siaddr=bootp_siaddr,
                giaddr=bootp_giaddr, chaddr=bootp_chaddr) / \
    scapy.DHCP(options=[('message-type', 'ack'), ('server_id', dhcp_serverip), ('lease_time', int(dhcp_lease)),
                ('subnet_mask', dhcp_netmask), ('end')]) / \
    scapy.PADDING(padding)
    return pkt

def dhcp_release_packet(eth_dst='ff:ff:ff:ff:ff:ff',
                     eth_src='00:01:02:03:04:05',
                     ip_src='0.0.0.0',
                     ip_dst='255.255.255.255',
                     src_port=68,
                     dst_port=67,
                     bootp_chaddr='00:01:02:03:04:05',
                     bootp_ciaddr='1.2.3.4',
                     dhcp_server_ip='1.2.3.4'):
    """
        Return a dhcp release packet

        Supports a few parameters:
        @param eth_dst Destination MAC, should be broadcast address
        @param eth_src Source MAC, should be address of client
        @param ip_src Source IP, should be default route IP address
        @param ip_dst Destination IP, broadcast IP address
        @param src_port Source Port, 68 for DHCP client
        @param dst_port Destination Port, 67 for DHCP Server
        @param bootp_chaddr MAC Address of client
        @param bootp_ciaddr Client IP Address
        @param dhcp_server_ip IP address of DHCP server
    """

    pkt = scapy.Ether(dst=eth_dst, src=eth_src)/ \
    scapy.IP(src=ip_src, dst=ip_dst)/ \
    scapy.UDP(sport=src_port, dport=dst_port)/ \
    scapy.BOOTP(chaddr=bootp_chaddr, ciaddr=bootp_ciaddr)/ \
    scapy.DHCP(options=[('message-type', 'release'), ('server_id', dhcp_server_ip), ('end')])
    return pkt

def get_egr_list(parent, ports, how_many, exclude_list=[]):
    """
    Generate a list of ports avoiding those in the exclude list
    @param parent Supplies logging
    @param ports List of OF port numbers
    @param how_many Number of ports to be added to the list
    @param exclude_list List of ports not to be used
    @returns An empty list if unable to find enough ports
    """

    if how_many == 0:
        return []

    count = 0
    egr_ports = []
    for egr_idx in range(len(ports)):
        if ports[egr_idx] not in exclude_list:
            egr_ports.append(ports[egr_idx])
            count += 1
            if count >= how_many:
                return egr_ports
    logging.debug("Could not generate enough egress ports for test")
    return []

def test_params_get(default={}):
    """
    Return all the values passed via test-params if present

    @param default Default dictionary to use if no valid params found

    WARNING: TEST PARAMETERS MUST BE PYTHON IDENTIFIERS;
    AND CANNOT START WITH "__";
    eg egr_count, not egr-count.
    """
    test_params = ptf.config["test_params"]
    params_str = "class _TestParams:\n    " + test_params
    try:
        exec params_str
    except:
        return default

    params = {}
    for k, v in vars(_TestParams).items():
        if k[:2] != "__":
            params[k] = v
    return params

def test_param_get(key, default=None):
    """
    Return value passed via test-params if present

    @param key The lookup key
    @param default Default value to use if not found

    WARNING: TEST PARAMETERS MUST BE PYTHON IDENTIFIERS;
    eg egr_count, not egr-count.
    """
    params = test_params_get()

    try:
        return params[key]
    except:
        return default

_import_blacklist.add('FILTER')
FILTER = ''.join([(len(repr(chr(x)))==3) and chr(x) or '.'
                  for x in range(256)])

def hex_dump_buffer(src, length=16):
    """
    Convert src to a hex dump string and return the string
    @param src The source buffer
    @param length The number of bytes shown in each line
    @returns A string showing the hex dump
    """
    result = ["\n"]
    for i in xrange(0, len(src), length):
       chars = src[i:i+length]
       hex = ' '.join(["%02X" % ord(x) for x in chars])
       printable = ''.join(["%s" % ((ord(x) <= 127 and
                                     FILTER[ord(x)]) or '.') for x in chars])
       result.append("%04x  %-*s  %s\n" % (i, length*3, hex, printable))
    return ''.join(result)

def format_packet(pkt):
    return "Packet length %d \n%s" % (len(str(pkt)),
                                      hex_dump_buffer(str(pkt)))

def inspect_packet(pkt):
    """
    Wrapper around scapy's show() method.
    @returns A string showing the dissected packet.
    """
    from cStringIO import StringIO
    out = None
    backup = sys.stdout
    try:
        tmp = StringIO()
        sys.stdout = tmp
        pkt.show2()
        out = tmp.getvalue()
        tmp.close()
    finally:
        sys.stdout = backup
    return out

def nonstandard(cls):
    """
    Testcase decorator that marks the test as being non-standard.
    These tests are not automatically added to the "standard" group.
    """
    cls._nonstandard = True
    return cls

def disabled(cls):
    """
    Testcase decorator that marks the test as being disabled.
    These tests are not automatically added to the "standard" group or
    their module's group.
    """
    cls._disabled = True
    return cls

def group(name):
    """
    Testcase decorator that adds the test to a group.
    """
    def fn(cls):
        if not hasattr(cls, "_groups"):
            cls._groups = []
        cls._groups.append(name)
        return cls
    return fn

def testtimeout(seconds):
    """
    Testcase decorator that adds a timeout to the test.
    """
    def fn(cls):
        cls._testtimeout = seconds
        return cls
    return fn

def ptf_ports(num=None):
    """
    Return a list of 'num' port ids (device_number, port_number)

    If 'num' is None, return all available ports. Otherwise, limit the length
    of the result to 'num' and raise an exception if not enough ports are
    available.
    """
    ports = sorted(ptf.config["port_map"].keys())
    if num != None and len(ports) < num:
        raise Exception("test requires %d ports but only %d are available" % (num, len(ports)))
    return ports[:num]

def port_to_tuple(port):
    if type(port) is int:
        return 0, port
    if type(port) is tuple:
        return port
    if type(port) is str:
        try:
            return 0, int(port)
        except:
            pass
    return None

def send_packet(test, port_id, pkt, count=1):
    """
    Send a packet (or a number of packets) out of port_id
    port_id can either be a single integer (port_number on default device 0)
    or a tuple of 2 integers (device_number, port_number)
    """
    device, port = port_to_tuple(port_id)
    pkt = str(pkt)
    sent = 0

    for n in range(count):
        test.before_send(pkt, device_number=device, port_number=port)
        sent += test.dataplane.send(device, port, pkt)

    return sent

def send(test, port_id, pkt, count=1):
    """
    See send_packet.
    """
    return send_packet(test, port_id, pkt, count=count)

def dp_poll(test, device_number=0, port_number=None, timeout=-1, exp_pkt=None):
    """
    Wrapper function around dataplane.poll
    """
    t = test.dataplane.poll(
        device_number=device_number, port_number=port_number,
        timeout=timeout, exp_pkt=exp_pkt, filters=FILTERS
    )
    (rcv_device, rcv_port, rcv_pkt, pkt_time) = t
    if rcv_pkt is not None:
        test.at_receive(rcv_pkt, device_number=rcv_device, port_number=rcv_port)
    return t

def verify_packet(test, pkt, port_id):
    """
    Check that an expected packet is received
    port_id can either be a single integer (port_number on default device 0)
    or a tuple of 2 integers (device_number, port_number)
    """
    device, port = port_to_tuple(port_id)
    logging.debug("Checking for pkt on device %d, port %d", device, port)
    (rcv_device, rcv_port, rcv_pkt, pkt_time) = dp_poll(
        test, device_number=device, port_number=port, timeout=2, exp_pkt=pkt
    )
    test.assertTrue(rcv_pkt != None, "Did not receive expected pkt on device %d, port %r" % (device, port))

def verify_no_packet(test, pkt, port_id, timeout=None):
    """
    Check that a particular packet is not received
    port_id can either be a single integer (port_number on default device 0)
    or a tuple of 2 integers (device_number, port_number)
    """
    if timeout is None:
        timeout = ptf.ptfutils.default_negative_timeout
    device, port = port_to_tuple(port_id)
    logging.debug("Negative check for pkt on device %d, port %d", device, port)
    (rcv_device, rcv_port, rcv_pkt, pkt_time) = dp_poll(
        test, device_number=device, port_number=port, exp_pkt=pkt,
        timeout=timeout
    )
    test.assertTrue(rcv_pkt == None, "Received packet on device %d, port %r" % (device, port))

def verify_no_other_packets(test, device_number=0, timeout=None):
    """
    Check that no unexpected packets are received on specified device

    This is a no-op if the --relax option is in effect.
    """
    if ptf.config["relax"]:
        return
    if timeout is None:
        timeout = ptf.ptfutils.default_negative_timeout
    logging.debug("Checking for unexpected packets on all ports of device %d" % device_number)
    (rcv_device, rcv_port, rcv_pkt, pkt_time) = dp_poll(
        test, device_number=device_number,
        timeout=timeout
    )
    if rcv_pkt != None:
        logging.debug("Received unexpected packet on device %d, port %r: %s", device_number, rcv_port, format_packet(rcv_pkt))
    test.assertTrue(rcv_pkt == None, "Unexpected packet on device %d, port %r" % (device_number, rcv_port))

def verify_packets(test, pkt, ports=[], device_number=0):
    """
    Check that a packet is received on each of the specified port numbers for a
    given device (default device number is 0).

    Also verifies that the packet is not received on any other ports for this
    device, and that no other packets are received on the device (unless --relax
    is in effect).

    This covers the common and simplest cases for checking dataplane outputs.
    For more complex usage, like multiple different packets being output, or
    multiple packets on the same port, use the primitive verify_packet,
    verify_no_packet, and verify_no_other_packets functions directly.
    """
    for device, port in ptf_ports():
        if device != device_number:
            continue
        if port in ports:
            verify_packet(test, pkt, (device, port))
        else:
            verify_no_packet(test, pkt, (device, port))
    verify_no_other_packets(test, device_number=device_number)

def verify_no_packet_any(test, pkt, ports=[], device_number=0):
    """
    Check that a packet is NOT received on _any_ of the specified ports belonging to
    the given device (default device_number is 0).
    """
    test.assertTrue(len(ports) != 0, "No port available to validate receiving packet on device %d, " % device_number)
    for device, port in ptf_ports():
        if device != device_number:
            continue
        if port in ports:
            print 'verifying packet on port device', device_number, 'port', port
            verify_no_packet(test, pkt, (device, port))

def verify_packets_any(test, pkt, ports=[], device_number=0):
    """
    Check that a packet is received on _any_ of the specified ports belonging to
    the given device (default device_number is 0).

    Also verifies that the packet is not received on any other ports for this
    device, and that no other packets are received on the device (unless --relax
    is in effect).
    """
    received = False
    for device, port in ptf_ports():
        if device != device_number:
            continue
        if port in ports:
            logging.debug("Checking for pkt on device %d, port %d", device_number, port)
            (rcv_device, rcv_port, rcv_pkt, pkt_time) = dp_poll(
                test, device_number=device, port_number=port, exp_pkt=pkt
            )
            if rcv_pkt != None:
                received = True
        else:
            verify_no_packet(test, pkt, (device, port))
    verify_no_other_packets(test)

    test.assertTrue(received == True, "Did not receive expected pkt on any of ports %r for device %d" % (ports, device_number))

def verify_packet_any_port(test, pkt, ports=[], device_number=0):
    """
    Check that the packet is received on _any_ of the specified ports belonging to
    the given device (default device_number is 0).

    The function returns when either the expected packet is received or timeout (1 second).

    Also verifies that the packet is or received on any other ports for this
    device, and that no other packets are received on the device (unless --relax
    is in effect).

    Returns the index of the port on which the packet is received and the packet.
    """
    received = False
    match_index = 0
    (rcv_device, rcv_port, rcv_pkt, pkt_time) = dp_poll(
        test,
        device_number=device_number,
        exp_pkt=pkt,
        timeout=1
    )

    logging.debug("Checking for pkt on device %d, port %r", device_number, ports)
    if rcv_port in ports:
        match_index = ports.index(rcv_port)
        received = True
    verify_no_other_packets(test, device_number=device_number)

    test.assertTrue(received == True, "Did not receive expected pkt(s) on any of ports %r for device %d" % (ports, device_number))
    return (match_index, rcv_pkt)

def verify_any_packet_any_port(test, pkts=[], ports=[], device_number=0):
    """
    Check that _any_ of the packet is received on _any_ of the specified ports belonging to
    the given device (default device_number is 0).

    Also verifies that the packet is not received on any other ports for this
    device, and that no other packets are received on the device (unless --relax
    is in effect).

    Returns the index of the port on which the packet is received.
    """
    received = False
    match_index = 0
    (rcv_device, rcv_port, rcv_pkt, pkt_time) = dp_poll(
        test,
        device_number=device_number,
        timeout=1
    )

    logging.debug("Checking for pkt on device %d, port %r", device_number, ports)
    if rcv_port in ports:
        for pkt in pkts:
            if str(pkt) == str(rcv_pkt):
                match_index = ports.index(rcv_port)
                received = True
    verify_no_other_packets(test, device_number=device_number)

    test.assertTrue(received == True, "Did not receive expected pkt(s) on any of ports %r for device %d" % (ports, device_number))
    return match_index

def verify_each_packet_on_each_port(test, pkts=[], ports=[], device_number=0):
    """
    Check that each packet is received on corresponding port in the port list belonging to
    the given device (default device_number is 0).

    Also verifies that the packet is not received on any other ports for this
    device, and that no other packets are received on the device (unless --relax
    is in effect).
    """
    pkt_cnt = 0
    test.assertTrue(len(pkts) == len(ports), "packet list count does not match port list count")
    for port, pkt in zip(ports, pkts):
        logging.debug("Checking for pkt on device %d, port %d", device_number, port)
        (rcv_device, rcv_port, rcv_pkt, pkt_time) = dp_poll(
            test,
            device_number=device_number,
            port_number=port,
            exp_pkt=pkt
        )
        test.assertTrue(rcv_pkt != None, "Did not receive expected pkt(s) on port %d for device %d" %(port, device_number))

    verify_no_other_packets(test, device_number=device_number)

def verify_packet_prefix(test, pkt, port, len, device_number=0):
    """
    Check that an expected packet is received
    """
    logging.debug("Checking for pkt on port %r", port)
    (rcv_device, rcv_port, rcv_pkt, pkt_time) = test.dataplane.poll(port_number=port, timeout=2, exp_pkt=str(pkt)[:len])
    test.assertTrue(rcv_pkt != None, "Did not receive expected pkt on %r" % port)

def count_matched_packets(test, exp_packet, port, device_number=0, timeout=1):
    """
    Receive all packets on the port and count how many expected packets were received.
    As soon as the packets stop arriving, the function waits for the timeout value and returns the counter
    """
    total_rcv_pkt_cnt = 0
    while True:
        (rcv_device, rcv_port, rcv_pkt, pkt_time) = dp_poll(test, device_number=device_number, port_number=port, timeout=timeout)
        if rcv_pkt is not None:
            if ptf.dataplane.match_exp_pkt(exp_packet, rcv_pkt):
                total_rcv_pkt_cnt += 1
        else:
            break

    return total_rcv_pkt_cnt


__all__ = list(set(locals()) - _import_blacklist)
