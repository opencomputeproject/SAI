#!/usr/bin/env python

# Copyright 2013-present Barefoot Networks, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# Antonin Bas (antonin@barefootnetworks.com)
#
#

import sys
import os
import argparse
import time
import struct
import socket
import Queue
try:
    import nnpy
except ImportError:
    print "Cannot find nnpy package, please install"
    sys.exit(1)
import threading
import os
import logging
from fcntl import ioctl
from socket import AF_NETLINK, SOCK_DGRAM

# copied from ptf.netutils
# From bits/ioctls.h
SIOCGIFHWADDR  = 0x8927          # Get hardware address
SIOCGIFINDEX   = 0x8933          # name -> if_index mapping
SIOCGIFFLAGS   = 0x8913          # get the active flag word of the device
SIOCSIFFLAGS   = 0x8914          # set the active flag word of the device
IFF_UP         = 0x0001

def get_if(iff, cmd):
    s = socket.socket()
    ifreq = ioctl(s, cmd, struct.pack("16s16x",iff))
    s.close()
    return ifreq

def get_if_index(iff):
    return int(struct.unpack("I", get_if(iff, SIOCGIFINDEX)[16:20])[0])

def get_mac(iff):
    return ':'.join(
        ['%02x' % ord(char) for char in get_if(iff, SIOCGIFHWADDR)[18:24]])

def set_if_status(iff, status):
    s = socket.socket()
    ifr = struct.pack('16sh', iff, 0)
    result = ioctl(s, SIOCGIFFLAGS, ifr)
    flags = struct.unpack('16sh', result)[1]
    if status:
       flags |= IFF_UP
    else:
       flags &= ~IFF_UP
    ifr = struct.pack('16sh', iff, flags)
    ioctl(s, SIOCSIFFLAGS, ifr)
    s.close()

def get_if_status(iff):
    try:
        s = socket.socket()
        ifr = struct.pack('16sh', iff, 0)
        result = ioctl(s, SIOCGIFFLAGS, ifr)
        s.close()
    except IOError:
        return False

    flags = struct.unpack('16sh', result)[1]

    return flags & IFF_UP > 0

def if_exists(iff):
    ifaces = os.listdir('/sys/class/net')

    return iff in ifaces

# Taken from ptf parser
class ActionInterface(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        # Parse --interface
        def check_interface(value):
            try:
                dev_and_port, interface = value.split('@', 1)
                dev_and_port = dev_and_port.split("-")
                if len(dev_and_port) == 1:
                    dev, port = 0, int(dev_and_port[0])
                elif len(dev_and_port) == 2:
                    dev, port = int(dev_and_port[0]), int(dev_and_port[1])
                else:
                    raise ValueError("")
            except ValueError:
                parser.error("incorrect interface syntax (got %s, expected 'port@interface' or 'device-port@interface')" % repr(value))
            return (dev, port, interface)

        assert(type(values) is str)
        getattr(namespace, self.dest).append(check_interface(values))

class ActionDeviceSocket(argparse.Action):
    def __init__(self, *args, **kwargs):
        super(ActionDeviceSocket, self).__init__(*args, **kwargs)
        self.devices_observed = set()

    def __call__(self, parser, namespace, values, option_string=None):
        # Parse --device-socket
        def check_device_socket(value):
            try:
                dev, addr = value.split('@', 1)
                dev = int(dev)
            except ValueError:
                parser.error("incorrect device-socket syntax (got %s, expected something of the form <dev id>@<socket addr>)" % repr(value))
            if dev in self.devices_observed:
                parser.error("cannot specify the same device twice")
            else:
                self.devices_observed.add(dev)
            if addr.startswith("ipc://"):
                pass
            elif addr.startswith("tcp://"):
                pass
            else:
                parser.error("nanomsg address must start with 'ipc://' or 'tcp://'")
            return (dev, addr)

        assert(type(values) is str)
        getattr(namespace, self.dest).append(check_device_socket(values))

parser = argparse.ArgumentParser(description='PTF Nanomsg agent')
parser.add_argument(
    "--device-socket", type=str, dest="device_sockets",
    metavar="DEVICE-SOCKET", action=ActionDeviceSocket, default=[],
    help="Specify the nanomsg socket to use to send / receive packets for a given device, as well as the ports to enable on the device.May be given multiple times. Example: 0@<socket addr>")
parser.add_argument(
    "--interface", "-i", type=str, dest="interfaces",
    metavar="INTERFACE", action=ActionInterface, default=[],
    help="Specify a port number and the dataplane interface to use. May be given multiple times. Example: 0-1@eth2 (use eth2 as port 1 of device 0)")
parser.add_argument(
    "--verbose", "-v", dest="verbose", action='store_true',
    help="Specify if you need verbose output")
parser.add_argument(
    "--set-nn-rcv-buffer", type=int, dest="nn_rcv_buf",
    metavar="BUFFER_SIZE", default=0,
    help="Specify a nanomsg socket receive buffer size")
parser.add_argument(
    "--set-nn-snd-buffer", type=int, dest="nn_snd_buf",
    metavar="BUFFER_SIZE", default=0,
    help="Specify a nanomsg socket send buffer size")
parser.add_argument(
    "--set-iface-rcv-buffer", type=int, dest="iface_rcv_buf",
    metavar="BUFFER_SIZE", default=0,
    help="Specify an interface socket receive buffer size")
parser.add_argument(
    "--set-iface-snd-buffer", type=int, dest="iface_snd_buf",
    metavar="BUFFER_SIZE", default=0,
    help="Specify an interface socket send buffer size")

args = parser.parse_args()

iface_mgrs = {}
nano_mgrs = {}

logging.basicConfig(format='%(message)s')
logger = logging.getLogger('ptf_nn_agent')

class IfaceMgr(threading.Thread):
    def __init__(self, dev, port, iface_name, iface_rcv_buf=0, iface_snd_buf=0):
        threading.Thread.__init__(self)
        self.daemon = True
        self.rx_ctr = 0
        self.tx_ctr = 0
        self.dev = dev
        self.port = port
        self.iface_name = iface_name
        self.iface_rcv_buf = iface_rcv_buf
        self.iface_snd_buf = iface_snd_buf

    def forward(self, p):
        # can that conflict with sniff?
        self.socket.send(p)
        self.tx_ctr += 1

    def received(self, p):
        logger.debug("IfaceMgr {}-{} ({}) received a packet".format(
            self.dev, self.port, self.iface_name))
        if self.dev in nano_mgrs:
            nano_mgr = nano_mgrs[self.dev]
            nano_mgr.forward(str(p), self.port)
            self.rx_ctr += 1

    def get_mac(self):
        try:
            mac = get_mac(self.iface_name)
            return mac
        except:
            # if not supported on platform
            return None

    def get_ctrs(self):
        return self.rx_ctr, self.tx_ctr

    def port_up(self):
        set_if_status(self.iface_name, True)
        logger.debug("IfaceMgr {}-{} ({}) status set to UP".format(
                     self.dev, self.port, self.iface_name))

    def port_down(self):
        set_if_status(self.iface_name, False)
        logger.debug("IfaceMgr {}-{} ({}) status set to DOWN".format(
                     self.dev, self.port, self.iface_name))

    def run(self):
        # run this loop in case the interface goes down by external action
        # or the interface disappears
        while True:
            # wait until the port goes up
            while True:
                if if_exists(self.iface_name) and get_if_status(self.iface_name):
                    break
                time.sleep(1)

            logger.debug("IfaceMgr {}-{} ({}) status changed to UP".format(
                         self.dev, self.port, self.iface_name))
            try:
                self.socket = socket.socket(socket.AF_PACKET, socket.SOCK_RAW,
                                            socket.htons(0x03))
                if self.iface_rcv_buf != 0:
                    self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, self.iface_rcv_buf)

                if self.iface_snd_buf  != 0:
                    self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, self.iface_snd_buf)

                self.socket.bind((self.iface_name, 0))
                logger.debug("IfaceMgr {}-{} ({}) AF_PACKET socket is open".format(
                             self.dev, self.port, self.iface_name))
                while True:
                    msg = self.socket.recv(4096)
                    self.received(msg)
            except socket.error as err:
                logger.debug("IfaceMgr {}-{} ({}) Error reading from the socket.".format(
                             self.dev, self.port, self.iface_name))
                self.socket.close()


class NanomsgMgr(threading.Thread):
    MSG_TYPE_PORT_ADD = 0
    MSG_TYPE_PORT_REMOVE = 1
    MSG_TYPE_PORT_SET_STATUS = 2
    MSG_TYPE_PACKET_IN = 3
    MSG_TYPE_PACKET_OUT = 4
    MSG_TYPE_INFO_REQ = 5
    MSG_TYPE_INFO_REP = 6

    MSG_PORT_STATUS_UP = 0
    MSG_PORT_STATUS_DOWN = 1

    MSG_INFO_TYPE_HWADDR = 0
    MSG_INFO_TYPE_CTRS = 1

    MSG_INFO_STATUS_SUCCESS = 0
    MSG_INFO_STATUS_NOT_SUPPORTED = 1

    def __init__(self, dev, socket_addr, nn_rcv_buf=0, nn_snd_buf=0):
        threading.Thread.__init__(self)
        self.daemon = True
        self.dev = dev
        self.socket_addr = socket_addr
        self.socket = nnpy.Socket(nnpy.AF_SP, nnpy.PAIR)
        if nn_rcv_buf != 0:
            self.socket.setsockopt(nnpy.SOL_SOCKET, nnpy.RCVBUF, nn_rcv_buf)
        if nn_snd_buf != 0:
            self.socket.setsockopt(nnpy.SOL_SOCKET, nnpy.SNDBUF, nn_snd_buf)
        self.socket.bind(socket_addr)

    def forward(self, p, port):
        msg = struct.pack("<iii{}s".format(len(p)), self.MSG_TYPE_PACKET_OUT,
                          port, len(p), p)
        # because nnpy expects unicode when using str
        msg = list(msg)
        self.socket.send(msg)

    def handle_info_req(self, port_number, info_id, msg):
        def handle_not_supported():
            fmt = "<iiii"
            rep = struct.pack(fmt, self.MSG_TYPE_INFO_REP, port_number, info_id,
                              self.MSG_INFO_STATUS_NOT_SUPPORTED)
            self.socket.send(rep)

        def handle_hwaddr():
            if (self.dev, port_number) not in iface_mgrs:
                handle_not_supported()
            else:
                iface_mgr = iface_mgrs[(self.dev, port_number)]
                mac = iface_mgr.get_mac()
                fmt = "<iiii{}s".format(len(mac))
                rep = struct.pack(fmt, self.MSG_TYPE_INFO_REP, port_number,
                                  info_id, self.MSG_INFO_STATUS_SUCCESS, mac)
                self.socket.send(rep)

        def handle_ctrs():
            if (self.dev, port_number) not in iface_mgrs:
                handle_not_supported()
            else:
                iface_mgr = iface_mgrs[(self.dev, port_number)]
                rx, tx = iface_mgr.get_ctrs()
                fmt = "<iiiiii"
                rep = struct.pack(fmt, self.MSG_TYPE_INFO_REP, port_number,
                                  info_id, self.MSG_INFO_STATUS_SUCCESS, rx, tx)
                self.socket.send(rep)

        handlers = {
            self.MSG_INFO_TYPE_HWADDR: handle_hwaddr,
            self.MSG_INFO_TYPE_CTRS:   handle_ctrs,
        }
        handlers.get(info_id, handle_not_supported)()


    def handle_set_status_req(self, port_number, status):
        if (self.dev, port_number) in iface_mgrs:
            iface_mgr = iface_mgrs[(self.dev, port_number)]
            if status == self.MSG_PORT_STATUS_UP:
                iface_mgr.port_up()
            elif status == self.MSG_PORT_STATUS_DOWN:
                iface_mgr.port_down()

    def run(self):
        while True:
            msg = self.socket.recv()
            fmt = "<iii"
            msg_type, port_number, more = struct.unpack_from(fmt, msg)
            hdr_size = struct.calcsize(fmt)
            msg = msg[hdr_size:]
            if msg_type == self.MSG_TYPE_INFO_REQ:
                self.handle_info_req(port_number, more, msg)
                continue
            if msg_type == self.MSG_TYPE_PORT_SET_STATUS:
                self.handle_set_status_req(port_number, more)
                continue
            if msg_type != self.MSG_TYPE_PACKET_IN:
                continue
            assert (len(msg) == more)
            logger.debug("NanomsgMgr {}-{} ({}) received a packet".format(
                self.dev, port_number, self.socket_addr))
            if (self.dev, port_number) in iface_mgrs:
                iface_mgr = iface_mgrs[(self.dev, port_number)]
                iface_mgr.forward(msg)

def main():
    if args.verbose:
        logger.setLevel(logging.DEBUG)
    else:
        logger.setLevel(logging.INFO)

    for dev, port, iface in args.interfaces:
        i = IfaceMgr(dev, port, iface, args.iface_rcv_buf, args.iface_snd_buf)
        i.start()
        iface_mgrs[(dev, port)] = i
    for dev, addr in args.device_sockets:
        n = NanomsgMgr(dev, addr, args.nn_rcv_buf, args.nn_snd_buf)
        n.start()
        nano_mgrs[dev] = n
    logger.info("READY")
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        return

if __name__ == '__main__':
    main()
