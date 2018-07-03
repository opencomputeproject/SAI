
"""
Network utilities for the OpenFlow test framework
"""

###########################################################################
##                                                                         ##
## Promiscuous mode enable/disable                                         ##
##                                                                         ##
## Based on code from Scapy by Phillippe Biondi                            ##
##                                                                         ##
##                                                                         ##
## This program is free software; you can redistribute it and/or modify it ##
## under the terms of the GNU General Public License as published by the   ##
## Free Software Foundation; either version 2, or (at your option) any     ##
## later version.                                                          ##
##                                                                         ##
## This program is distributed in the hope that it will be useful, but     ##
## WITHOUT ANY WARRANTY; without even the implied warranty of              ##
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       ##
## General Public License for more details.                                ##
##                                                                         ##
#############################################################################

import socket
from fcntl import ioctl
import struct

# From net/if_arp.h
ARPHDR_ETHER = 1
ARPHDR_LOOPBACK = 772

# From bits/ioctls.h
SIOCGIFHWADDR  = 0x8927          # Get hardware address
SIOCGIFINDEX   = 0x8933          # name -> if_index mapping

# From netpacket/packet.h
PACKET_ADD_MEMBERSHIP  = 1
PACKET_DROP_MEMBERSHIP = 2
PACKET_MR_PROMISC      = 1

# From bits/socket.h
SOL_PACKET = 263

def get_if(iff,cmd):
  s=socket.socket()
  ifreq = ioctl(s, cmd, struct.pack("16s16x",iff))
  s.close()
  return ifreq

def get_if_index(iff):
  return int(struct.unpack("I",get_if(iff, SIOCGIFINDEX)[16:20])[0])

def get_mac(iff):
  return ':'.join(['%02x' % ord(char) for char in get_if(iff, SIOCGIFHWADDR)[18:24]])

def set_promisc(s,iff,val=1):
  mreq = struct.pack("IHH8s", get_if_index(iff), PACKET_MR_PROMISC, 0, "")
  if val:
      cmd = PACKET_ADD_MEMBERSHIP
  else:
      cmd = PACKET_DROP_MEMBERSHIP
  s.setsockopt(SOL_PACKET, cmd, mreq)

