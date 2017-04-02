"""
Utility parsing functions
"""

import sys
import socket
import packet as scapy

def parse_mac(mac_str):
    """
    Parse a MAC address

    Parse a MAC address ':' separated string of hex digits to an
    array of integer values.  '00:d0:05:5d:24:00' => [0, 208, 5, 93, 36, 0]
    @param mac_str The string to convert
    @return Array of 6 integer values
    """
    return map(lambda val: int(val, 16), mac_str.split(":"))

def parse_ip(ip_str):
    """
    Parse an IP address

    Parse an IP address '.' separated string of decimal digits to an
    host ordered integer.  '172.24.74.77' => 
    @param ip_str The string to convert
    @return Integer value
    """
    array = map(lambda val: int(val), ip_str.split("."))
    val = 0
    for a in array:
        val <<= 8
        val += a
    return val

def parse_ipv6(ip_str):
    """
    Parse an IPv6 address

    Parse a textual IPv6 address and return a 16 byte binary string.
    """
    return socket.inet_pton(socket.AF_INET6, ip_str)

def packet_type_classify(ether):
    try:
        dot1q = ether[scapy.Dot1Q]
    except:
        dot1q = None

    try:
        ip = ether[scapy.IP]
    except:
        ip = None

    try:
        tcp = ether[scapy.TCP]
    except:
        tcp = None

    try:
        udp = ether[scapy.UDP]
    except:
        udp = None

    try:
        icmp = ether[scapy.ICMP]
    except:
        icmp = None

    try:
        arp = ether[scapy.ARP]
    except:
        arp = None
    return (dot1q, ip, tcp, udp, icmp, arp)
