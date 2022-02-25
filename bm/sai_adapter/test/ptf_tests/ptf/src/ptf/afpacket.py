"""
AF_PACKET receive support

When VLAN offload is enabled on the NIC Linux will not deliver the VLAN tag
in the data returned by recv. Instead, it delivers the VLAN TCI in a control
message. Python 2.x doesn't have built-in support for recvmsg, so we have to
use ctypes to call it. The recv function exported by this module reconstructs
the VLAN tag if it was offloaded.
"""

import socket
import struct
from ctypes import *

ETH_P_8021Q = 0x8100
SOL_PACKET = 263
PACKET_AUXDATA = 8
TP_STATUS_VLAN_VALID = 1 << 4

class struct_iovec(Structure):
    _fields_ = [
        ("iov_base", c_void_p),
        ("iov_len", c_size_t),
    ]

class struct_msghdr(Structure):
    _fields_ = [
        ("msg_name", c_void_p),
        ("msg_namelen", c_uint32),
        ("msg_iov", POINTER(struct_iovec)),
        ("msg_iovlen", c_size_t),
        ("msg_control", c_void_p),
        ("msg_controllen", c_size_t),
        ("msg_flags", c_int),
    ]

class struct_cmsghdr(Structure):
    _fields_ = [
        ("cmsg_len", c_size_t),
        ("cmsg_level", c_int),
        ("cmsg_type", c_int),
    ]

class struct_tpacket_auxdata(Structure):
    _fields_ = [
        ("tp_status", c_uint),
        ("tp_len", c_uint),
        ("tp_snaplen", c_uint),
        ("tp_mac", c_ushort),
        ("tp_net", c_ushort),
        ("tp_vlan_tci", c_ushort),
        ("tp_padding", c_ushort),
    ]

libc = CDLL("libc.so.6")
recvmsg = libc.recvmsg
recvmsg.argtypes = [c_int, POINTER(struct_msghdr), c_int]
recvmsg.retype = c_int

def enable_auxdata(sk):
    """
    Ask the kernel to return the VLAN tag in a control message

    Must be called on the socket before afpacket.recv.
    """
    sk.setsockopt(SOL_PACKET, PACKET_AUXDATA, 1)

def recv(sk, bufsize):
    """
    Receive a packet from an AF_PACKET socket
    @sk Socket
    @bufsize Maximum packet size
    """
    buf = create_string_buffer(bufsize)

    ctrl_bufsize = sizeof(struct_cmsghdr) + sizeof(struct_tpacket_auxdata) + sizeof(c_size_t)
    ctrl_buf = create_string_buffer(ctrl_bufsize)

    iov = struct_iovec()
    iov.iov_base = cast(buf, c_void_p)
    iov.iov_len = bufsize

    msghdr = struct_msghdr()
    msghdr.msg_name = None
    msghdr.msg_namelen = 0
    msghdr.msg_iov = pointer(iov)
    msghdr.msg_iovlen = 1
    msghdr.msg_control = cast(ctrl_buf, c_void_p)
    msghdr.msg_controllen = ctrl_bufsize
    msghdr.msg_flags = 0

    rv = recvmsg(sk.fileno(), byref(msghdr), 0)
    if rv < 0:
        raise RuntimeError("recvmsg failed: rv=%d" % rv)

    # The kernel only delivers control messages we ask for. We
    # only enabled PACKET_AUXDATA, so we can assume it's the
    # only control message.
    assert msghdr.msg_controllen >= sizeof(struct_cmsghdr)

    cmsghdr = struct_cmsghdr.from_buffer(ctrl_buf) # pylint: disable=E1101
    assert cmsghdr.cmsg_level == SOL_PACKET
    assert cmsghdr.cmsg_type == PACKET_AUXDATA

    auxdata = struct_tpacket_auxdata.from_buffer(ctrl_buf, sizeof(struct_cmsghdr)) # pylint: disable=E1101

    if auxdata.tp_vlan_tci != 0 or auxdata.tp_status & TP_STATUS_VLAN_VALID:
        # Insert VLAN tag
        tag = struct.pack("!HH", ETH_P_8021Q, auxdata.tp_vlan_tci)
        return buf.raw[:12] + tag + buf.raw[12:rv]
    else:
        return buf.raw[:rv]
