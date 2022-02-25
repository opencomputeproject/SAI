# Distributed under the OpenFlow Software License (see LICENSE)
# Copyright (c) 2010 The Board of Trustees of The Leland Stanford Junior University
# Copyright (c) 2012, 2013 Big Switch Networks, Inc.
"""
Wrap scapy to satisfy pylint
"""
from ptf import config
import sys
import logging

try:
    import scapy.config
    import scapy.route
    import scapy.layers.l2
    import scapy.layers.inet
    import scapy.layers.dhcp
    import scapy.packet
    import scapy.main
    if not config["disable_ipv6"]:
        import scapy.route6
        import scapy.layers.inet6
except ImportError:
    sys.exit("Need to install scapy for packet parsing")

Ether = scapy.layers.l2.Ether
LLC = scapy.layers.l2.LLC
SNAP = scapy.layers.l2.SNAP
Dot1Q = scapy.layers.l2.Dot1Q
GRE = scapy.layers.l2.GRE
IP = scapy.layers.inet.IP
IPOption = scapy.layers.inet.IPOption
ARP = scapy.layers.inet.ARP
TCP = scapy.layers.inet.TCP
UDP = scapy.layers.inet.UDP
ICMP = scapy.layers.inet.ICMP
DHCP = scapy.layers.dhcp.DHCP
BOOTP = scapy.layers.dhcp.BOOTP
PADDING = scapy.packet.Padding

if not config["disable_ipv6"]:
    IPv6 = scapy.layers.inet6.IPv6
    IPv6ExtHdrRouting = scapy.layers.inet6.IPv6ExtHdrRouting
    ICMPv6Unknown = scapy.layers.inet6.ICMPv6Unknown
    ICMPv6EchoRequest = scapy.layers.inet6.ICMPv6EchoRequest

VXLAN = None
if not config["disable_vxlan"]:
    try:
        scapy.main.load_contrib("vxlan")
        VXLAN = scapy.contrib.vxlan.VXLAN
        logging.info("VXLAN support found in Scapy")
    except:
        logging.warn("VXLAN support not found in Scapy")
        pass

ERSPAN = None
ERSPAN_III = None
PlatformSpecific = None
if not config["disable_erspan"]:
    try:
        scapy.main.load_contrib("erspan")
        ERSPAN = scapy.contrib.erspan.ERSPAN
        ERSPAN_III = scapy.contrib.erspan.ERSPAN_III
        PlatformSpecific = scapy.contrib.erspan.PlatformSpecific
        logging.info("ERSPAN support found in Scapy")
    except:
        logging.warn("ERSPAN support not found in Scapy")
        pass

GENEVE = None
if not config["disable_geneve"]:
    try:
        scapy.main.load_contrib("geneve")
        GENEVE = scapy.contrib.geneve.GENEVE
        logging.info("GENEVE support found in Scapy")
    except:
        logging.warn("GENEVE support not found in Scapy")
        pass

MPLS = None
if not config["disable_mpls"]:
    try:
        scapy.main.load_contrib("mpls")
        MPLS = scapy.contrib.mpls.MPLS
        logging.info("MPLS support found in Scapy")
    except:
        logging.warn("MPLS support not found in Scapy")
        pass

NVGRE = None
if not config["disable_nvgre"]:
    try:
        scapy.main.load_contrib("nvgre")
        NVGRE = scapy.contrib.nvgre.NVGRE
        logging.info("NVGRE support found in Scapy")
    except:
        logging.warn("NVGRE support not found in Scapy")
        pass
